/*
 * File: z_bg_relay_objects.c
 * Overlay: ovl_Bg_Relay_Objects
 * Description: Windmill Setpieces
 */

#include "z_bg_relay_objects.h"
#include "objects/object_relay_objects/object_relay_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgRelayObjects*)thisx)

typedef enum {
    /* 0 */ WINDMILL_ROTATING_GEAR,
    /* 1 */ WINDMILL_DAMPE_STONE_DOOR
} WindmillSetpiecesMode;

void BgRelayObjects_Init(Actor* thisx, GlobalContext* globalCtx);
void BgRelayObjects_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgRelayObjects_Update(Actor* thisx, GlobalContext* globalCtx);
void BgRelayObjects_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808A90F4(BgRelayObjects* self, GlobalContext* globalCtx);
void func_808A91AC(BgRelayObjects* self, GlobalContext* globalCtx);
void func_808A9234(BgRelayObjects* self, GlobalContext* globalCtx);
void BgRelayObjects_DoNothing(BgRelayObjects* self, GlobalContext* globalCtx);
void func_808A932C(BgRelayObjects* self, GlobalContext* globalCtx);
void func_808A939C(BgRelayObjects* self, GlobalContext* globalCtx);

const ActorInit Bg_Relay_Objects_InitVars = {
    ACTOR_BG_RELAY_OBJECTS,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_RELAY_OBJECTS,
    sizeof(BgRelayObjects),
    (ActorFunc)BgRelayObjects_Init,
    (ActorFunc)BgRelayObjects_Destroy,
    (ActorFunc)BgRelayObjects_Update,
    (ActorFunc)BgRelayObjects_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(gravity, 5, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgRelayObjects_Init(Actor* thisx, GlobalContext* globalCtx) {
    static u32 D_808A9508 = 0;
    BgRelayObjects* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(thisx, sInitChain);
    self->switchFlag = thisx->params & 0x3F;
    thisx->params = (thisx->params >> 8) & 0xFF;
    DynaPolyActor_Init(&self->dyna, 3);
    if (thisx->params == WINDMILL_ROTATING_GEAR) {
        CollisionHeader_GetVirtual(&gWindmillRotatingPlatformCol, &colHeader);
        if (gSaveContext.eventChkInf[6] & 0x20) {
            thisx->world.rot.y = 0x400;
        } else {
            thisx->world.rot.y = 0x80;
        }
        func_800F5718();
        thisx->room = -1;
        thisx->flags |= 0x20;
        if (D_808A9508 & 2) {
            thisx->params = 0xFF;
            Actor_Kill(thisx);
        } else {
            D_808A9508 |= 2;
            self->actionFunc = func_808A939C;
        }
    } else {
        CollisionHeader_GetVirtual(&gDampeRaceDoorCol, &colHeader);
        if (thisx->room == 0) {
            self->unk_169 = self->switchFlag - 0x33;
        } else {
            self->unk_169 = thisx->room + 1;
        }
        thisx->room = -1;
        self->timer = 1;
        if (self->unk_169 >= 6) {
            if (D_808A9508 & 1) {
                Actor_Kill(thisx);
            } else {
                D_808A9508 |= 1;
                self->actionFunc = BgRelayObjects_DoNothing;
            }
        } else if (self->unk_169 != 5) {
            Flags_UnsetSwitch(globalCtx, self->switchFlag);
            if (D_808A9508 & (1 << self->unk_169)) {
                Actor_Kill(thisx);
            } else {
                D_808A9508 |= (1 << self->unk_169);
                self->actionFunc = func_808A90F4;
            }
        } else {
            Flags_SetSwitch(globalCtx, self->switchFlag);
            self->actionFunc = func_808A91AC;
            thisx->world.pos.y += 120.0f;
            D_808A9508 |= 1;
        }
    }
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
}

void BgRelayObjects_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgRelayObjects* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    if ((self->dyna.actor.params == WINDMILL_ROTATING_GEAR) && (gSaveContext.cutsceneIndex < 0xFFF0)) {
        gSaveContext.eventChkInf[6] &= ~0x20;
    }
}

void func_808A90F4(BgRelayObjects* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
        if (self->timer != 0) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_SLIDE_DOOR_OPEN);
            if (INV_CONTENT(ITEM_HOOKSHOT) != ITEM_NONE) {
                self->timer = 120;
            } else {
                self->timer = 160;
            }
        }
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 120.0f, 12.0f)) {
            self->actionFunc = func_808A91AC;
        }
    }
}

void func_808A91AC(BgRelayObjects* self, GlobalContext* globalCtx) {
    if (self->unk_169 != 5) {
        if (self->timer != 0) {
            self->timer--;
        }
        func_8002F994(&self->dyna.actor, self->timer);
    }
    if ((self->timer == 0) || (self->unk_169 == globalCtx->roomCtx.curRoom.num)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_SLIDE_DOOR_CLOSE);
        self->actionFunc = func_808A9234;
    }
}

void func_808A9234(BgRelayObjects* self, GlobalContext* globalCtx) {
    self->dyna.actor.velocity.y += self->dyna.actor.gravity;
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, self->dyna.actor.velocity.y)) {
        func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 180, 20, 100);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_STONE_BOUND);
        if (self->unk_169 != globalCtx->roomCtx.curRoom.num) {
            func_800788CC(NA_SE_EN_PO_LAUGH);
            self->timer = 5;
            self->actionFunc = func_808A932C;
            return;
        }
        Flags_UnsetSwitch(globalCtx, self->switchFlag);
        self->dyna.actor.flags &= ~0x10;
        if (globalCtx->roomCtx.curRoom.num == 4) {
            gSaveContext.timer1State = 0xF;
        }
        self->actionFunc = BgRelayObjects_DoNothing;
    }
}

void BgRelayObjects_DoNothing(BgRelayObjects* self, GlobalContext* globalCtx) {
}

void func_808A932C(BgRelayObjects* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        if (!Player_InCsMode(globalCtx)) {
            func_80078884(NA_SE_OC_ABYSS);
            Gameplay_TriggerRespawn(globalCtx);
            self->actionFunc = BgRelayObjects_DoNothing;
        }
    }
}

void func_808A939C(BgRelayObjects* self, GlobalContext* globalCtx) {
    if (Flags_GetEnv(globalCtx, 5)) {
        gSaveContext.eventChkInf[6] |= 0x20;
    }
    if (gSaveContext.eventChkInf[6] & 0x20) {
        Math_ScaledStepToS(&self->dyna.actor.world.rot.y, 0x400, 8);
    } else {
        Math_ScaledStepToS(&self->dyna.actor.world.rot.y, 0x80, 8);
    }
    self->dyna.actor.shape.rot.y += self->dyna.actor.world.rot.y;
    func_800F436C(&self->dyna.actor.projectedPos, NA_SE_EV_WOOD_GEAR - SFX_FLAG,
                  ((self->dyna.actor.world.rot.y - 0x80) * (1.0f / 0x380)) + 1.0f);
}

void BgRelayObjects_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgRelayObjects* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgRelayObjects_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgRelayObjects* self = THIS;

    if (self->dyna.actor.params == WINDMILL_ROTATING_GEAR) {
        Gfx_DrawDListOpa(globalCtx, gWindmillRotatingPlatformDL);
    } else {
        Gfx_DrawDListOpa(globalCtx, gDampeRaceDoorDL);
    }
}
