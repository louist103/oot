/*
 * File: z_bg_haka_gate.c
 * Overlay: ovl_Bg_Haka_Gate
 * Description: Truth Spinner Puzzle (Shadow Temple)
 */

#include "z_bg_haka_gate.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_haka_objects/object_haka_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgHakaGate*)thisx)

// general purpose timer
#define vTimer actionVar1

// variables for turning the statue. Deg10 rotations are in tenths of a degree
#define vTurnDirection actionVar1
#define vTurnRateDeg10 actionVar2
#define vTurnAngleDeg10 actionVar3
#define vRotYDeg10 actionVar4
#define vInitTurnAngle actionVar5

// opening angle for floor
#define vOpenAngle actionVar2

// variables for the skull flames
#define vFlameScale actionVar3
#define vIsSkullOfTruth actionVar4
#define vScrollTimer actionVar5

#define SKULL_OF_TRUTH_FOUND 100

void BgHakaGate_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaGate_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaGate_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaGate_Draw(Actor* self, GlobalContext* globalCtx);

void BgHakaGate_DoNothing(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_StatueInactive(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_StatueIdle(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_StatueTurn(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_FloorClosed(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_FloorOpen(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_GateWait(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_GateOpen(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_SkullOfTruth(BgHakaGate* self, GlobalContext* globalCtx);
void BgHakaGate_FalseSkull(BgHakaGate* self, GlobalContext* globalCtx);

static s16 sSkullOfTruthRotY = 0x100;
static u8 sPuzzleState = 1;
static f32 sStatueDistToPlayer = 0;

static s16 sStatueRotY;

const ActorInit Bg_Haka_Gate_InitVars = {
    ACTOR_BG_HAKA_GATE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HAKA_OBJECTS,
    sizeof(BgHakaGate),
    (ActorFunc)BgHakaGate_Init,
    (ActorFunc)BgHakaGate_Destroy,
    (ActorFunc)BgHakaGate_Update,
    (ActorFunc)BgHakaGate_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaGate_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHakaGate* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(thisx, sInitChain);
    self->switchFlag = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    if (thisx->params == BGHAKAGATE_SKULL) {
        if (sSkullOfTruthRotY != 0x100) {
            self->actionFunc = BgHakaGate_FalseSkull;
        } else if (ABS(thisx->shape.rot.y) < 0x4000) {
            if ((Rand_ZeroOne() * 3.0f) < sPuzzleState) {
                self->vIsSkullOfTruth = true;
                sSkullOfTruthRotY = thisx->shape.rot.y + 0x8000;
                if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
                    self->actionFunc = BgHakaGate_DoNothing;
                } else {
                    self->actionFunc = BgHakaGate_SkullOfTruth;
                }
            } else {
                sPuzzleState++;
                self->actionFunc = BgHakaGate_FalseSkull;
            }
        } else {
            self->actionFunc = BgHakaGate_FalseSkull;
        }
        self->vScrollTimer = Rand_ZeroOne() * 20.0f;
        thisx->flags |= 0x10;
        if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
            self->vFlameScale = 350;
        }
    } else {
        if (thisx->params == BGHAKAGATE_STATUE) {
            CollisionHeader_GetVirtual(&object_haka_objects_Col_0131C4, &colHeader);
            self->vTimer = 0;
            sStatueDistToPlayer = 0.0f;
            if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
                self->actionFunc = BgHakaGate_StatueInactive;
            } else {
                self->actionFunc = BgHakaGate_StatueIdle;
            }
        } else if (thisx->params == BGHAKAGATE_FLOOR) {
            CollisionHeader_GetVirtual(&object_haka_objects_Col_010E10, &colHeader);
            if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
                self->actionFunc = BgHakaGate_DoNothing;
            } else {
                self->actionFunc = BgHakaGate_FloorClosed;
            }
        } else { // BGHAKAGATE_GATE
            CollisionHeader_GetVirtual(&object_haka_objects_Col_00A938, &colHeader);
            if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
                self->actionFunc = BgHakaGate_DoNothing;
                thisx->world.pos.y += 80.0f;
            } else {
                thisx->flags |= 0x10;
                Actor_SetFocus(thisx, 30.0f);
                self->actionFunc = BgHakaGate_GateWait;
            }
        }
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    }
}

void BgHakaGate_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHakaGate* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    if (self->dyna.actor.params == BGHAKAGATE_STATUE) {
        sSkullOfTruthRotY = 0x100;
        sPuzzleState = 1;
    }
}

void BgHakaGate_DoNothing(BgHakaGate* self, GlobalContext* globalCtx) {
}

void BgHakaGate_StatueInactive(BgHakaGate* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->dyna.unk_150 != 0.0f) {
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_150 = 0.0f;
    }
}

void BgHakaGate_StatueIdle(BgHakaGate* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 linkDirection;
    f32 forceDirection;

    if (self->dyna.unk_150 != 0.0f) {
        if (self->vTimer == 0) {
            self->vInitTurnAngle = self->dyna.actor.shape.rot.y - self->dyna.actor.yawTowardsPlayer;
            sStatueDistToPlayer = self->dyna.actor.xzDistToPlayer;
            forceDirection = (self->dyna.unk_150 >= 0.0f) ? 1.0f : -1.0f;
            linkDirection = ((s16)(self->dyna.actor.yawTowardsPlayer - player->actor.shape.rot.y) > 0) ? -1 : 1;
            self->vTurnDirection = linkDirection * forceDirection;
            self->actionFunc = BgHakaGate_StatueTurn;
        } else {
            player->stateFlags2 &= ~0x10;
            self->dyna.unk_150 = 0.0f;
            if (self->vTimer != 0) {
                self->vTimer--;
            }
        }
    } else {
        if (sPuzzleState == SKULL_OF_TRUTH_FOUND) {
            self->actionFunc = BgHakaGate_StatueInactive;
        } else {
            self->vTimer = 0;
        }
    }
}

void BgHakaGate_StatueTurn(BgHakaGate* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 turnFinished;
    s16 turnAngle;

    self->vTurnRateDeg10++;
    self->vTurnRateDeg10 = CLAMP_MAX(self->vTurnRateDeg10, 5);
    turnFinished = Math_StepToS(&self->vTurnAngleDeg10, 600, self->vTurnRateDeg10);
    turnAngle = self->vTurnAngleDeg10 * self->vTurnDirection;
    self->dyna.actor.shape.rot.y = (self->vRotYDeg10 + turnAngle) * 0.1f * (0x10000 / 360.0f);
    if ((player->stateFlags2 & 0x10) && (sStatueDistToPlayer > 0.0f)) {
        player->actor.world.pos.x =
            self->dyna.actor.home.pos.x +
            (Math_SinS(self->dyna.actor.shape.rot.y - self->vInitTurnAngle) * sStatueDistToPlayer);
        player->actor.world.pos.z =
            self->dyna.actor.home.pos.z +
            (Math_CosS(self->dyna.actor.shape.rot.y - self->vInitTurnAngle) * sStatueDistToPlayer);
    } else {
        sStatueDistToPlayer = 0.0f;
    }
    sStatueRotY = self->dyna.actor.shape.rot.y;
    if (turnFinished) {
        player->stateFlags2 &= ~0x10;
        self->vRotYDeg10 = (self->vRotYDeg10 + turnAngle) % 3600;
        self->vTurnRateDeg10 = 0;
        self->vTurnAngleDeg10 = 0;
        self->vTimer = 5;
        self->actionFunc = BgHakaGate_StatueIdle;
        self->dyna.unk_150 = 0.0f;
    }
    func_8002F974(&self->dyna.actor, NA_SE_EV_ROCK_SLIDE - SFX_FLAG);
}

void BgHakaGate_FloorClosed(BgHakaGate* self, GlobalContext* globalCtx) {
    if ((sStatueDistToPlayer > 1.0f) && (sStatueRotY != 0)) {
        Player* player = GET_PLAYER(globalCtx);
        f32 radialDist;
        f32 angDist;
        f32 cos = Math_CosS(sStatueRotY);
        f32 sin = Math_SinS(sStatueRotY);
        f32 dx = player->actor.world.pos.x - self->dyna.actor.world.pos.x;
        f32 dz = player->actor.world.pos.z - self->dyna.actor.world.pos.z;

        radialDist = dx * cos - dz * sin;
        angDist = dx * sin + dz * cos;

        if ((radialDist > 110.0f) || (fabsf(angDist) > 40.0f)) {
            s16 yawDiff = sSkullOfTruthRotY - sStatueRotY;

            sStatueDistToPlayer = 0.0f;
            if (ABS(yawDiff) < 0x80) {
                Flags_SetSwitch(globalCtx, self->switchFlag);
                sPuzzleState = SKULL_OF_TRUTH_FOUND;
                self->actionFunc = BgHakaGate_DoNothing;
            } else {
                func_80078884(NA_SE_SY_ERROR);
                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_GROUND_GATE_OPEN);
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                self->vTimer = 60;
                self->actionFunc = BgHakaGate_FloorOpen;
            }
        }
    }
}

void BgHakaGate_FloorOpen(BgHakaGate* self, GlobalContext* globalCtx) {
    if (self->vTimer != 0) {
        self->vTimer--;
    }
    if (self->vTimer == 0) {
        if (Math_ScaledStepToS(&self->vOpenAngle, 0, 0x800)) {
            func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            self->actionFunc = BgHakaGate_FloorClosed;
        }
    } else {
        Math_ScaledStepToS(&self->vOpenAngle, 0x3000, 0x800);
    }
}

void BgHakaGate_GateWait(BgHakaGate* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
        OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        self->actionFunc = BgHakaGate_GateOpen;
    }
}

void BgHakaGate_GateOpen(BgHakaGate* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 80.0f, 1.0f)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_METALDOOR_STOP);
        self->dyna.actor.flags &= ~0x10;
        self->actionFunc = BgHakaGate_DoNothing;
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
    }
}

void BgHakaGate_SkullOfTruth(BgHakaGate* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag) && Math_StepToS(&self->vFlameScale, 350, 20)) {
        self->actionFunc = BgHakaGate_DoNothing;
    }
}

void BgHakaGate_FalseSkull(BgHakaGate* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
        Math_StepToS(&self->vFlameScale, 350, 20);
    }
    if (globalCtx->actorCtx.unk_03) {
        self->dyna.actor.flags |= 0x80;
    } else {
        self->dyna.actor.flags &= ~0x80;
    }
}

void BgHakaGate_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHakaGate* self = THIS;

    self->actionFunc(self, globalCtx);
    if (self->dyna.actor.params == BGHAKAGATE_SKULL) {
        self->vScrollTimer++;
    }
}

void BgHakaGate_DrawFlame(BgHakaGate* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;
    f32 scale;

    if (self->vFlameScale > 0) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_haka_gate.c", 716);

        if (1) {}

        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0,
                                    (self->vScrollTimer * -20) & 0x1FF, 0x20, 0x80));
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 0, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);

        Matrix_Translate(thisx->world.pos.x, thisx->world.pos.y + 15.0f, thisx->world.pos.z, MTXMODE_NEW);
        Matrix_RotateY(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) * (M_PI / 0x8000), MTXMODE_APPLY);
        scale = self->vFlameScale * 0.00001f;
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_gate.c", 744),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_haka_gate.c", 749);
    }
}

void BgHakaGate_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* displayLists[] = { 0x06012270, 0x06010A10, 0x0600A860, 0x0600F1B0 };
    BgHakaGate* self = THIS;
    MtxF currentMtxF;

    if ((thisx->flags & 0x80) == 0x80) {
        Gfx_DrawDListXlu(globalCtx, object_haka_objects_DL_00F1B0);
    } else {
        func_80093D18(globalCtx->state.gfxCtx);
        if (thisx->params == BGHAKAGATE_FLOOR) {
            OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_haka_gate.c", 781);
            Matrix_Get(&currentMtxF);
            Matrix_Translate(0.0f, 0.0f, -2000.0f, MTXMODE_APPLY);
            Matrix_RotateX(self->vOpenAngle * (M_PI / 0x8000), MTXMODE_APPLY);
            Matrix_Translate(0.0f, 0.0f, 2000.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_gate.c", 788),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, object_haka_objects_DL_010A10);
            Matrix_Put(&currentMtxF);
            Matrix_Translate(0.0f, 0.0f, 2000.0f, MTXMODE_APPLY);
            Matrix_RotateX(-self->vOpenAngle * (M_PI / 0x8000), MTXMODE_APPLY);
            Matrix_Translate(0.0f, 0.0f, -2000.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_gate.c", 796),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, object_haka_objects_DL_010C10);
            CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_haka_gate.c", 800);
        } else {
            Gfx_DrawDListOpa(globalCtx, displayLists[thisx->params]);
        }
    }
    if (thisx->params == BGHAKAGATE_SKULL) {
        BgHakaGate_DrawFlame(self, globalCtx);
    }
}
