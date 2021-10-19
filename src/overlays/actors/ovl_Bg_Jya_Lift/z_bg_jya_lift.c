/*
 * File: z_bg_jya_lift.c
 * Overlay: ovl_Bg_Jya_Lift
 * Description: Chain Platform (Spirit Temple)
 */

#include "z_bg_jya_lift.h"
#include "objects/object_jya_obj/object_jya_obj.h"

#define FLAGS 0x00000010

#define THIS ((BgJyaLift*)thisx)

void BgJyaLift_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaLift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaLift_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaLift_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgJyaLift_SetFinalPosY(BgJyaLift* self);
void BgJyaLift_SetInitPosY(BgJyaLift* self);
void BgJyaLift_DelayMove(BgJyaLift* self, GlobalContext* globalCtx);
void BgJyaLift_SetupMove(BgJyaLift* self);
void BgJyaLift_Move(BgJyaLift* self, GlobalContext* globalCtx);

static s16 sIsSpawned = false;

const ActorInit Bg_Jya_Lift_InitVars = {
    ACTOR_BG_JYA_LIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaLift),
    (ActorFunc)BgJyaLift_Init,
    (ActorFunc)BgJyaLift_Destroy,
    (ActorFunc)BgJyaLift_Update,
    (ActorFunc)BgJyaLift_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2500, ICHAIN_STOP),
};

void BgJyaLift_InitDynapoly(BgJyaLift* self, GlobalContext* globalCtx, CollisionHeader* collisionHeader, s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    CollisionHeader_GetVirtual(collisionHeader, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
}

void BgJyaLift_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaLift* self = THIS;

    self->isSpawned = false;
    if (sIsSpawned) {
        Actor_Kill(thisx);
        return;
    }

    // "Goddess lift CT"
    osSyncPrintf("女神リフト CT\n");
    BgJyaLift_InitDynapoly(self, globalCtx, &gLiftCol, DPM_UNK);
    Actor_ProcessInitChain(thisx, sInitChain);
    if (Flags_GetSwitch(globalCtx, (thisx->params & 0x3F))) {
        BgJyaLift_SetFinalPosY(self);
    } else {
        BgJyaLift_SetInitPosY(self);
    }
    thisx->room = -1;
    sIsSpawned = true;
    self->isSpawned = true;
}

void BgJyaLift_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaLift* self = THIS;

    if (self->isSpawned) {

        // "Goddess Lift DT"
        osSyncPrintf("女神リフト DT\n");
        sIsSpawned = false;
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
}

void BgJyaLift_SetInitPosY(BgJyaLift* self) {
    self->actionFunc = BgJyaLift_DelayMove;
    self->dyna.actor.world.pos.y = 1613.0f;
    self->moveDelay = 0;
}

void BgJyaLift_DelayMove(BgJyaLift* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F) || (self->moveDelay > 0)) {
        self->moveDelay++;
        if (self->moveDelay >= 20) {
            OnePointCutscene_Init(globalCtx, 3430, -99, &self->dyna.actor, MAIN_CAM);
            BgJyaLift_SetupMove(self);
        }
    }
}

void BgJyaLift_SetupMove(BgJyaLift* self) {
    self->actionFunc = BgJyaLift_Move;
}

void BgJyaLift_Move(BgJyaLift* self, GlobalContext* globalCtx) {
    f32 distFromBottom;
    f32 tempVelocity;

    Math_SmoothStepToF(&self->dyna.actor.velocity.y, 4.0f, 0.1f, 1.0f, 0.0f);
    tempVelocity = (self->dyna.actor.velocity.y < 0.2f) ? 0.2f : self->dyna.actor.velocity.y;
    distFromBottom = Math_SmoothStepToF(&self->dyna.actor.world.pos.y, 973.0f, 0.1f, tempVelocity, 0.2f);
    if ((self->dyna.actor.world.pos.y < 1440.0f) && (1440.0f <= self->dyna.actor.prevPos.y)) {
        func_8005B1A4(GET_ACTIVE_CAM(globalCtx));
    }
    if (fabsf(distFromBottom) < 0.001f) {
        BgJyaLift_SetFinalPosY(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ELEVATOR_STOP);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_BRIDGE_OPEN - SFX_FLAG);
    }
}

void BgJyaLift_SetFinalPosY(BgJyaLift* self) {
    self->actionFunc = NULL;
    self->dyna.actor.world.pos.y = 973.0f;
}

void BgJyaLift_Update(Actor* thisx, GlobalContext* globalCtx2) {
    BgJyaLift* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
    if ((self->dyna.unk_160 & 4) && ((self->unk_16B & 4) == 0)) {
        Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_TEPPEN);
    } else if (((self->dyna.unk_160) & 4) == 0 && ((self->unk_16B & 4)) &&
               (globalCtx->cameraPtrs[MAIN_CAM]->setting == CAM_SET_TEPPEN)) {
        Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON0);
    }
    self->unk_16B = self->dyna.unk_160;

    // Spirit Temple room 5 is the main room with the statue room 25 is directly above room 5
    if ((globalCtx->roomCtx.curRoom.num != 5) && (globalCtx->roomCtx.curRoom.num != 25)) {
        Actor_Kill(thisx);
    }
}

void BgJyaLift_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gLiftDL);
}
