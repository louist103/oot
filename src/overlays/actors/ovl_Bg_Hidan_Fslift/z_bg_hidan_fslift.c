/*
 * File: z_bg_hidan_fslift.c
 * Overlay: ovl_Bg_Hidan_Fslift
 * Description: Hookshot Elevator
 */

#include "z_bg_hidan_fslift.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgHidanFslift*)thisx)

void BgHidanFslift_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFslift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFslift_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFslift_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80886FCC(BgHidanFslift* self, GlobalContext* globalCtx);
void func_8088706C(BgHidanFslift* self, GlobalContext* globalCtx);
void func_808870D8(BgHidanFslift* self, GlobalContext* globalCtx);

const ActorInit Bg_Hidan_Fslift_InitVars = {
    ACTOR_BG_HIDAN_FSLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanFslift),
    (ActorFunc)BgHidanFslift_Init,
    (ActorFunc)BgHidanFslift_Destroy,
    (ActorFunc)BgHidanFslift_Update,
    (ActorFunc)BgHidanFslift_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 350, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_STOP),
};

void BgHidanFslift_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad1;
    BgHidanFslift* self = THIS;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gFireTempleHookshotElevatorCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_OBJ_HSBLOCK,
                           self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y + 40.0f,
                           self->dyna.actor.world.pos.z + -28.0f, 0, 0, 0, 2) == NULL) {
        Actor_Kill(&self->dyna.actor);
        return;
    }
    self->actionFunc = func_80886FCC;
}

void func_80886F24(BgHidanFslift* self) {
    if (self->dyna.actor.child != NULL && self->dyna.actor.child->update != NULL) {
        self->dyna.actor.child->world.pos.x = self->dyna.actor.world.pos.x;
        self->dyna.actor.child->world.pos.y = self->dyna.actor.world.pos.y + 40.0f;
        self->dyna.actor.child->world.pos.z = self->dyna.actor.world.pos.z + -28.0f;
    } else {
        self->dyna.actor.child = NULL;
    }
}

void BgHidanFslift_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanFslift* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_80886FB4(BgHidanFslift* self) {
    self->timer = 40;
    self->actionFunc = func_80886FCC;
}

void func_80886FCC(BgHidanFslift* self, GlobalContext* globalCtx) {
    s32 heightBool;

    if (self->timer) {
        self->timer--;
    }

    if (self->timer == 0) {
        heightBool = false;
        if ((self->dyna.actor.world.pos.y - self->dyna.actor.home.pos.y) < 0.5f) {
            heightBool = true;
        }
        if (func_80043590(&self->dyna) && (heightBool)) {
            self->actionFunc = func_808870D8;
        } else if (!heightBool) {
            self->actionFunc = func_8088706C;
        }
    }
}

void func_8088706C(BgHidanFslift* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 4.0f)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        func_80886FB4(self);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
    func_80886F24(self);
}

void func_808870D8(BgHidanFslift* self, GlobalContext* globalCtx) {
    if (func_80043590(&self->dyna)) {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 790.0f, 4.0f)) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
            func_80886FB4(self);
        } else {
            func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
        }
    } else {
        func_80886FB4(self);
    }
    func_80886F24(self);
}

void BgHidanFslift_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanFslift* self = THIS;

    self->actionFunc(self, globalCtx);
    if (func_8004356C(&self->dyna)) {
        if (self->unk_16A == 0) {
            self->unk_16A = 3;
        }
        Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_HIDAN1);
    } else if (!func_8004356C(&self->dyna)) {
        if (self->unk_16A != 0) {
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON0);
        }
        self->unk_16A = 0;
    }
}

void BgHidanFslift_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gFireTempleHookshotElevatorDL);
}
