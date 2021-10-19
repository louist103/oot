/*
 * File: z_bg_jya_1flift.c
 * Overlay: ovl_Bg_Jya_1flift
 * Description: Shortcut Elevator used in the vanilla version of the Spirit Temple
 */

#include "z_bg_jya_1flift.h"
#include "objects/object_jya_obj/object_jya_obj.h"

#define FLAGS 0x00000010

#define THIS ((BgJya1flift*)thisx)

void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgJya1flift_SetupWaitForSwitch(BgJya1flift* self);
void BgJya1flift_WaitForSwitch(BgJya1flift* self, GlobalContext* globalCtx);
void BgJya1flift_DoNothing(BgJya1flift* self, GlobalContext* globalCtx);
void BgJya1flift_ChangeDirection(BgJya1flift* self);
void BgJya1flift_Move(BgJya1flift* self, GlobalContext* globalCtx);
void BgJya1flift_SetupDoNothing(BgJya1flift* self);
void BgJya1flift_ResetMoveDelay(BgJya1flift* self);
void BgJya1flift_DelayMove(BgJya1flift* self, GlobalContext* globalCtx);

static u8 sIsSpawned = false;

const ActorInit Bg_Jya_1flift_InitVars = {
    ACTOR_BG_JYA_1FLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJya1flift),
    (ActorFunc)BgJya1flift_Init,
    (ActorFunc)BgJya1flift_Destroy,
    (ActorFunc)BgJya1flift_Update,
    (ActorFunc)BgJya1flift_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 70, 80, -82, { 0, 0, 0 } },
};

static f32 sFinalPositions[] = { 443.0f, -50.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_STOP),
};

void BgJya1flift_InitDynapoly(BgJya1flift* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning : move BG login failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_1flift.c", 179,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgJya1flift_InitCollision(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->dyna.actor, &sCylinderInit);
    self->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;
}

void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* self = THIS;
    // "1 F lift"
    osSyncPrintf("(１Ｆリフト)(flag %d)(room %d)\n", sIsSpawned, globalCtx->roomCtx.curRoom.num);
    self->hasInitialized = false;
    if (sIsSpawned) {
        Actor_Kill(thisx);
        return;
    }
    BgJya1flift_InitDynapoly(self, globalCtx, &g1fliftCol, 0);
    Actor_ProcessInitChain(thisx, sInitChain);
    BgJya1flift_InitCollision(thisx, globalCtx);
    if (Flags_GetSwitch(globalCtx, (thisx->params & 0x3F))) {
        LINK_AGE_IN_YEARS == YEARS_ADULT ? BgJya1flift_ChangeDirection(self) : BgJya1flift_SetupDoNothing(self);
    } else {
        BgJya1flift_SetupWaitForSwitch(self);
    }
    thisx->room = -1;
    sIsSpawned = true;
    self->hasInitialized = true;
}

void BgJya1flift_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* self = THIS;

    if (self->hasInitialized) {
        sIsSpawned = false;
        Collider_DestroyCylinder(globalCtx, &self->collider);
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
}

void BgJya1flift_SetupWaitForSwitch(BgJya1flift* self) {
    self->actionFunc = BgJya1flift_WaitForSwitch;
    self->dyna.actor.world.pos.y = sFinalPositions[0];
}

void BgJya1flift_WaitForSwitch(BgJya1flift* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params & 0x3F))) {
        BgJya1flift_ChangeDirection(self);
    }
}

void BgJya1flift_SetupDoNothing(BgJya1flift* self) {
    self->actionFunc = BgJya1flift_DoNothing;
    self->dyna.actor.world.pos.y = sFinalPositions[0];
}

void BgJya1flift_DoNothing(BgJya1flift* self, GlobalContext* globalCtx) {
}

void BgJya1flift_ChangeDirection(BgJya1flift* self) {
    self->actionFunc = BgJya1flift_Move;
    self->isMovingDown ^= true;
    self->dyna.actor.velocity.y = 0.0f;
}

void BgJya1flift_Move(BgJya1flift* self, GlobalContext* globalCtx) {
    f32 tempVelocity;

    Math_StepToF(&self->dyna.actor.velocity.y, 6.0f, 0.4f);
    if (self->dyna.actor.velocity.y < 1.0f) {
        tempVelocity = 1.0f;
    } else {
        tempVelocity = self->dyna.actor.velocity.y;
    }
    if (fabsf(Math_SmoothStepToF(&self->dyna.actor.world.pos.y, (sFinalPositions[self->isMovingDown]), 0.5f,
                                 tempVelocity, 1.0f)) < 0.001f) {
        self->dyna.actor.world.pos.y = sFinalPositions[self->isMovingDown];
        BgJya1flift_ResetMoveDelay(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
}

void BgJya1flift_ResetMoveDelay(BgJya1flift* self) {
    self->actionFunc = BgJya1flift_DelayMove;
    self->moveDelay = 0;
}

void BgJya1flift_DelayMove(BgJya1flift* self, GlobalContext* globalCtx) {
    self->moveDelay++;
    if (self->moveDelay >= 21) {
        BgJya1flift_ChangeDirection(self);
    }
}

void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx2) {
    BgJya1flift* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    s32 tempIsRiding;

    // Room 0 is the first room and 6 is the room that the lift starts on
    if (globalCtx->roomCtx.curRoom.num == 6 || globalCtx->roomCtx.curRoom.num == 0) {
        self->actionFunc(self, globalCtx);
        tempIsRiding = func_8004356C(&self->dyna) ? true : false;
        if ((self->actionFunc == BgJya1flift_Move) || (self->actionFunc == BgJya1flift_DelayMove)) {
            if (tempIsRiding) {
                Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_HIDAN1);
            } else if (!tempIsRiding && self->isLinkRiding) {
                Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON0);
            }
        }
        self->isLinkRiding = tempIsRiding;
        Collider_UpdateCylinder(thisx, &self->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    } else {
        Actor_Kill(thisx);
    }
}

void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, g1fliftDL);
}
