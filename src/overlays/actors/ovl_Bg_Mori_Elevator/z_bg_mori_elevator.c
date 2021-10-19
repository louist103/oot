#include "z_bg_mori_elevator.h"
#include "objects/object_mori_objects/object_mori_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgMoriElevator*)thisx)

void BgMoriElevator_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMoriElevator_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMoriElevator_Update(Actor* thisx, GlobalContext* globalCtx);

void BgMoriElevator_SetupWaitAfterInit(BgMoriElevator* self);
void BgMoriElevator_WaitAfterInit(BgMoriElevator* self, GlobalContext* globalCtx);
void BgMoriElevator_SetupSetPosition(BgMoriElevator* self);
void BgMoriElevator_SetPosition(BgMoriElevator* self, GlobalContext* globalCtx);
void BgMoriElevator_Draw(Actor* thisx, GlobalContext* globalCtx);
void BgMoriElevator_StopMovement(BgMoriElevator* self);
void func_808A2008(BgMoriElevator* self, GlobalContext* globalCtx);
void BgMoriElevator_MoveIntoGround(BgMoriElevator* self, GlobalContext* globalCtx);
void BgMoriElevator_MoveAboveGround(BgMoriElevator* self, GlobalContext* globalCtx);

static s16 sIsSpawned = false;

const ActorInit Bg_Mori_Elevator_InitVars = {
    ACTOR_BG_MORI_ELEVATOR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriElevator),
    (ActorFunc)BgMoriElevator_Init,
    (ActorFunc)BgMoriElevator_Destroy,
    (ActorFunc)BgMoriElevator_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 3000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

f32 func_808A1800(f32* pValue, f32 target, f32 scale, f32 maxStep, f32 minStep) {
    f32 var = (target - *pValue) * scale;

    if (*pValue < target) {
        if (maxStep < var) {
            var = maxStep;
        } else {
            if (var < minStep) {
                var = minStep;
            }
        }
        *pValue = (*pValue + var);

        if (target < *pValue) {
            *pValue = target;
        }
    } else {
        if (target < *pValue) {
            if (var < (-maxStep)) {
                var = (-maxStep);
            } else {
                if ((-minStep) < var) {
                    var = (-minStep);
                }
            }
            *pValue = (*pValue + var);
            if (*pValue < target) {
                *pValue = target;
            }
        } else {
            var = 0.0f;
        }
    }
    return var;
}

void func_808A18FC(BgMoriElevator* self, f32 distTo) {
    f32 temp;

    temp = fabsf(distTo) * 0.09f;
    func_800F436C(&self->dyna.actor.projectedPos, NA_SE_EV_ELEVATOR_MOVE2 - SFX_FLAG, CLAMP(temp, 0.0f, 1.0f));
}

void BgMoriElevator_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgMoriElevator* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    self->unk_172 = sIsSpawned;
    self->moriTexObjIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjIndex < 0) {
        Actor_Kill(thisx);
        // "Forest Temple obj elevator Bank Danger!"
        osSyncPrintf("Error : 森の神殿 obj elevator バンク危険！(%s %d)\n", "../z_bg_mori_elevator.c", 277);
    } else {
        switch (sIsSpawned) {
            case false:
                // "Forest Temple elevator CT"
                osSyncPrintf("森の神殿 elevator CT\n");
                sIsSpawned = true;
                self->dyna.actor.room = -1;
                Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
                DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
                CollisionHeader_GetVirtual(&gMoriElevatorCol, &colHeader);
                self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
                BgMoriElevator_SetupWaitAfterInit(self);
                break;
            case true:
                Actor_Kill(thisx);
                break;
        }
    }
}

void BgMoriElevator_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgMoriElevator* self = THIS;

    if (self->unk_172 == 0) {
        // "Forest Temple elevator DT"
        osSyncPrintf("森の神殿 elevator DT\n");
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        sIsSpawned = false;
    }
}

s32 BgMoriElevator_IsPlayerRiding(BgMoriElevator* self, GlobalContext* globalCtx) {
    return ((self->dyna.unk_160 & 2) && !(self->unk_170 & 2) &&
            ((GET_PLAYER(globalCtx)->actor.world.pos.y - self->dyna.actor.world.pos.y) < 80.0f));
}

void BgMoriElevator_SetupWaitAfterInit(BgMoriElevator* self) {
    self->actionFunc = BgMoriElevator_WaitAfterInit;
}

void BgMoriElevator_WaitAfterInit(BgMoriElevator* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->moriTexObjIndex)) {
        if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
            if (globalCtx->roomCtx.curRoom.num == 2) {
                self->dyna.actor.world.pos.y = 73.0f;
                BgMoriElevator_SetupSetPosition(self);
            } else {
                // "Error: Forest Temple obj elevator Room setting is dangerous"
                osSyncPrintf("Error : 森の神殿 obj elevator 部屋設定が危険(%s %d)\n", "../z_bg_mori_elevator.c", 371);
            }
        } else {
            BgMoriElevator_SetupSetPosition(self);
        }
        self->dyna.actor.draw = BgMoriElevator_Draw;
    }
}

void func_808A1C30(BgMoriElevator* self) {
    self->actionFunc = BgMoriElevator_MoveIntoGround;
}

void BgMoriElevator_MoveIntoGround(BgMoriElevator* self, GlobalContext* globalCtx) {
    f32 distToTarget;

    func_808A1800(&self->dyna.actor.velocity.y, 2.0f, 0.05f, 1.0f, 0.0f);
    distToTarget = func_808A1800(&self->dyna.actor.world.pos.y, 73.0f, 0.08f, self->dyna.actor.velocity.y, 1.5f);
    if (fabsf(distToTarget) < 0.001f) {
        BgMoriElevator_SetupSetPosition(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ELEVATOR_STOP);
    } else {
        func_808A18FC(self, distToTarget);
    }
}

void func_808A1CF4(BgMoriElevator* self, GlobalContext* globalCtx) {
    self->actionFunc = BgMoriElevator_MoveAboveGround;
    OnePointCutscene_Init(globalCtx, 3230, 70, &self->dyna.actor, MAIN_CAM);
    OnePointCutscene_Init(globalCtx, 1020, 15, &self->dyna.actor, MAIN_CAM);
}

void BgMoriElevator_MoveAboveGround(BgMoriElevator* self, GlobalContext* globalCtx) {
    f32 distToTarget;

    func_808A1800(&self->dyna.actor.velocity.y, 2.0f, 0.05f, 1.0f, 0.0f);
    distToTarget = func_808A1800(&self->dyna.actor.world.pos.y, 233.0f, 0.08f, self->dyna.actor.velocity.y, 1.5f);
    if (fabsf(distToTarget) < 0.001f) {
        BgMoriElevator_SetupSetPosition(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ELEVATOR_STOP);
    } else {
        func_808A18FC(self, distToTarget);
    }
}

void BgMoriElevator_SetupSetPosition(BgMoriElevator* self) {
    self->actionFunc = BgMoriElevator_SetPosition;
}

void BgMoriElevator_SetPosition(BgMoriElevator* self, GlobalContext* globalCtx) {
    s32 pad;

    if (BgMoriElevator_IsPlayerRiding(self, globalCtx)) {
        if (globalCtx->roomCtx.curRoom.num == 2) {
            self->targetY = -779.0f;
            BgMoriElevator_StopMovement(self);
        } else if (globalCtx->roomCtx.curRoom.num == 17) {
            self->targetY = 233.0f;
            BgMoriElevator_StopMovement(self);
        } else {
            // "Error:Forest Temple obj elevator Room setting is dangerous(%s %d)"
            osSyncPrintf("Error : 森の神殿 obj elevator 部屋設定が危険(%s %d)\n", "../z_bg_mori_elevator.c", 479);
        }
    } else if ((globalCtx->roomCtx.curRoom.num == 2) && (self->dyna.actor.world.pos.y < -275.0f)) {
        self->targetY = 233.0f;
        BgMoriElevator_StopMovement(self);
    } else if ((globalCtx->roomCtx.curRoom.num == 17) && (-275.0f < self->dyna.actor.world.pos.y)) {
        self->targetY = -779.0f;
        BgMoriElevator_StopMovement(self);
    } else if ((globalCtx->roomCtx.curRoom.num == 2) && Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F) &&
               (self->unk_16C == 0)) {
        self->targetY = 73.0f;
        func_808A1C30(self);
    } else if ((globalCtx->roomCtx.curRoom.num == 2) && !Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F) &&
               (self->unk_16C != 0)) {
        self->targetY = 233.0f;
        func_808A1CF4(self, globalCtx);
    }
}

void BgMoriElevator_StopMovement(BgMoriElevator* self) {
    self->actionFunc = func_808A2008;
    self->dyna.actor.velocity.y = 0.0f;
}

void func_808A2008(BgMoriElevator* self, GlobalContext* globalCtx) {
    f32 distTo;

    func_808A1800(&self->dyna.actor.velocity.y, 12.0f, 0.1f, 1.0f, 0.0f);
    distTo = func_808A1800(&self->dyna.actor.world.pos.y, self->targetY, 0.1f, self->dyna.actor.velocity.y, 0.3f);
    if (fabsf(distTo) < 0.001f) {
        BgMoriElevator_SetupSetPosition(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ELEVATOR_STOP);

    } else {
        func_808A18FC(self, distTo);
    }
}

void BgMoriElevator_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMoriElevator* self = THIS;

    self->actionFunc(self, globalCtx);
    self->unk_170 = self->dyna.unk_160;
    self->unk_16C = Flags_GetSwitch(globalCtx, (thisx->params & 0x3F));
}

void BgMoriElevator_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriElevator* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_elevator.c", 575);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, globalCtx->objectCtx.status[self->moriTexObjIndex].segment);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_elevator.c", 580),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gMoriElevatorDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_elevator.c", 584);
}
