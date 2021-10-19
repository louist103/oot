/*
 * File: z_en_goroiwa.c
 * Overlay: ovl_En_Goroiwa
 * Description: Rolling boulders
 */

#include "z_en_goroiwa.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_goroiwa/object_goroiwa.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((EnGoroiwa*)thisx)

typedef s32 (*EnGoroiwaUnkFunc1)(EnGoroiwa* self, GlobalContext* globalCtx);
typedef void (*EnGoroiwaUnkFunc2)(EnGoroiwa* self);

#define ENGOROIWA_ENABLE_AT (1 << 0)
#define ENGOROIWA_ENABLE_OC (1 << 1)
#define ENGOROIWA_PLAYER_IN_THE_WAY (1 << 2)
#define ENGOROIWA_RETAIN_ROT_SPEED (1 << 3)
#define ENGOROIWA_IN_WATER (1 << 4)

#define ENGOROIWA_LOOPMODE_ONEWAY 0
/* same as ENGOROIWA_LOOPMODE_ONEWAY but display rock fragments as if the boulder broke at the end of the path*/
#define ENGOROIWA_LOOPMODE_ONEWAY_BREAK 1
#define ENGOROIWA_LOOPMODE_ROUNDTRIP 3

void EnGoroiwa_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGoroiwa_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGoroiwa_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGoroiwa_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnGoroiwa_SetupRoll(EnGoroiwa* self);
void EnGoroiwa_Roll(EnGoroiwa* self, GlobalContext* globalCtx);
void EnGoroiwa_SetupMoveAndFallToGround(EnGoroiwa* self);
void EnGoroiwa_MoveAndFallToGround(EnGoroiwa* self, GlobalContext* globalCtx);
void EnGoroiwa_SetupWait(EnGoroiwa* self);
void EnGoroiwa_Wait(EnGoroiwa* self, GlobalContext* globalCtx);
void EnGoroiwa_SetupMoveUp(EnGoroiwa* self);
void EnGoroiwa_MoveUp(EnGoroiwa* self, GlobalContext* globalCtx);
void EnGoroiwa_SetupMoveDown(EnGoroiwa* self);
void EnGoroiwa_MoveDown(EnGoroiwa* self, GlobalContext* globalCtx);

const ActorInit En_Goroiwa_InitVars = {
    ACTOR_EN_GOROIWA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GOROIWA,
    sizeof(EnGoroiwa),
    (ActorFunc)EnGoroiwa_Init,
    (ActorFunc)EnGoroiwa_Destroy,
    (ActorFunc)EnGoroiwa_Update,
    (ActorFunc)EnGoroiwa_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x20000000, 0x00, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 58 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_HEAVY };

static f32 sUnused[] = { 10.0f, 9.2f };

void EnGoroiwa_UpdateCollider(EnGoroiwa* self) {
    static f32 yOffsets[] = { 0.0f, 59.5f };
    Sphere16* worldSphere = &self->collider.elements[0].dim.worldSphere;

    worldSphere->center.x = self->actor.world.pos.x;
    worldSphere->center.y = self->actor.world.pos.y + yOffsets[(self->actor.params >> 10) & 1];
    worldSphere->center.z = self->actor.world.pos.z;
}

void EnGoroiwa_InitCollider(EnGoroiwa* self, GlobalContext* globalCtx) {
    s32 pad;

    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItems);
    EnGoroiwa_UpdateCollider(self);
    self->collider.elements[0].dim.worldSphere.radius = 58;
}

void EnGoroiwa_UpdateFlags(EnGoroiwa* self, u8 setFlags) {
    self->stateFlags &= ~(ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC);
    self->stateFlags |= setFlags;
}

s32 EnGoroiwa_Vec3fNormalize(Vec3f* ret, Vec3f* a) {
    f32 magnitude = Math3D_Vec3fMagnitude(a);
    f32 scale;

    if (magnitude < 0.001f) {
        return false;
    }

    scale = 1.0f / magnitude;

    ret->x = a->x * scale;
    ret->y = a->y * scale;
    ret->z = a->z * scale;

    return true;
}

void EnGoroiwa_SetSpeed(EnGoroiwa* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_SPOT04) {
        self->isInKokiri = true;
        R_EN_GOROIWA_SPEED = 920;
    } else {
        self->isInKokiri = false;
        R_EN_GOROIWA_SPEED = 1000;
    }
}

void EnGoroiwa_FaceNextWaypoint(EnGoroiwa* self, GlobalContext* globalCtx) {
    Path* path = &globalCtx->setupPathList[self->actor.params & 0xFF];
    Vec3s* nextPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->nextWaypoint;
    Vec3f nextPosF;

    nextPosF.x = nextPos->x;
    nextPosF.y = nextPos->y;
    nextPosF.z = nextPos->z;

    self->actor.world.rot.y = Math_Vec3f_Yaw(&self->actor.world.pos, &nextPosF);
}

void EnGoroiwa_GetPrevWaypointDiff(EnGoroiwa* self, GlobalContext* globalCtx, Vec3f* dest) {
    s16 loopMode = (self->actor.params >> 8) & 3;
    Path* path = &globalCtx->setupPathList[self->actor.params & 0xFF];
    s16 prevWaypoint = self->currentWaypoint - self->pathDirection;
    Vec3s* prevPointPos;
    Vec3s* currentPointPos;

    if (prevWaypoint < 0) {
        if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY || loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK) {
            prevWaypoint = self->endWaypoint;
        } else if (loopMode == ENGOROIWA_LOOPMODE_ROUNDTRIP) {
            prevWaypoint = 1;
        }
    } else if (prevWaypoint > self->endWaypoint) {
        if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY || loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK) {
            prevWaypoint = 0;
        } else if (loopMode == ENGOROIWA_LOOPMODE_ROUNDTRIP) {
            prevWaypoint = self->endWaypoint - 1;
        }
    }

    currentPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->currentWaypoint;
    prevPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + prevWaypoint;
    dest->x = currentPointPos->x - prevPointPos->x;
    dest->y = currentPointPos->x - prevPointPos->y;
    dest->z = currentPointPos->x - prevPointPos->z;
}

void EnGoroiw_CheckEndOfPath(EnGoroiwa* self) {
    s16 loopMode = (self->actor.params >> 8) & 3;

    if (self->nextWaypoint < 0) {
        if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY || loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK) {
            self->currentWaypoint = self->endWaypoint;
            self->nextWaypoint = self->endWaypoint - 1;
            self->pathDirection = -1;
        } else if (loopMode == ENGOROIWA_LOOPMODE_ROUNDTRIP) {
            self->currentWaypoint = 0;
            self->nextWaypoint = 1;
            self->pathDirection = 1;
        }
    } else if (self->nextWaypoint > self->endWaypoint) {
        if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY || loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK) {
            self->currentWaypoint = 0;
            self->nextWaypoint = 1;
            self->pathDirection = 1;
        } else if (loopMode == ENGOROIWA_LOOPMODE_ROUNDTRIP) {
            self->currentWaypoint = self->endWaypoint;
            self->nextWaypoint = self->endWaypoint - 1;
            self->pathDirection = -1;
        }
    }
}

void EnGoroiwa_SetNextWaypoint(EnGoroiwa* self) {
    self->currentWaypoint = self->nextWaypoint;
    self->nextWaypoint += self->pathDirection;
    EnGoroiw_CheckEndOfPath(self);
}

void EnGoroiwa_ReverseDirection(EnGoroiwa* self) {
    self->pathDirection *= -1;
    self->currentWaypoint = self->nextWaypoint;
    self->nextWaypoint += self->pathDirection;
}

void EnGoroiwa_InitPath(EnGoroiwa* self, GlobalContext* globalCtx) {
    self->endWaypoint = globalCtx->setupPathList[self->actor.params & 0xFF].count - 1;
    self->currentWaypoint = 0;
    self->nextWaypoint = 1;
    self->pathDirection = 1;
}

void EnGoroiwa_TeleportToWaypoint(EnGoroiwa* self, GlobalContext* globalCtx, s32 waypoint) {
    Path* path = &globalCtx->setupPathList[self->actor.params & 0xFF];
    Vec3s* pointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + waypoint;

    self->actor.world.pos.x = pointPos->x;
    self->actor.world.pos.y = pointPos->y;
    self->actor.world.pos.z = pointPos->z;
}

void EnGoroiwa_InitRotation(EnGoroiwa* self) {
    self->prevUnitRollAxis.x = 1.0f;
    self->rollRotSpeed = 1.0f;
}

s32 EnGoroiwa_GetAscendDirection(EnGoroiwa* self, GlobalContext* globalCtx) {
    s32 pad;
    Path* path = &globalCtx->setupPathList[self->actor.params & 0xFF];
    Vec3s* nextPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->nextWaypoint;
    Vec3s* currentPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->currentWaypoint;

    if (nextPointPos->x == currentPointPos->x && nextPointPos->z == currentPointPos->z) {
        if (nextPointPos->y == currentPointPos->y) {
            // "Error: Invalid path data (points overlap)"
            osSyncPrintf("Error : レールデータ不正(点が重なっている)");
            osSyncPrintf("(%s %d)(arg_data 0x%04x)\n", "../z_en_gr.c", 559, self->actor.params);
        }

        if (nextPointPos->y > currentPointPos->y) {
            return 1;
        } else {
            return -1;
        }
    }

    return 0;
}

void EnGoroiwa_SpawnDust(GlobalContext* globalCtx, Vec3f* pos) {
    static Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    static Vec3f accel = { 0.0f, 0.3f, 0.0f };
    Vec3f randPos;
    s32 i;
    s16 angle = 0;

    for (i = 0; i < 8; i++) {
        angle += 0x4E20;
        randPos.x = pos->x + (47.0f * (Rand_ZeroOne() * 0.5f + 0.5f)) * Math_SinS(angle);
        randPos.y = pos->y + (Rand_ZeroOne() - 0.5f) * 40.0f;
        randPos.z = pos->z + ((47.0f * (Rand_ZeroOne() * 0.5f + 0.5f))) * Math_CosS(angle);
        func_800286CC(globalCtx, &randPos, &velocity, &accel, (s16)(Rand_ZeroOne() * 30.0f) + 100, 80);
        func_800286CC(globalCtx, &randPos, &velocity, &accel, (s16)(Rand_ZeroOne() * 20.0f) + 80, 80);
    }
}

void EnGoroiwa_SpawnWaterEffects(GlobalContext* globalCtx, Vec3f* contactPos) {
    Vec3f splashPos;
    s32 i;
    s16 angle = 0;

    for (i = 0; i < 11; i++) {
        angle += 0x1746;
        splashPos.x = contactPos->x + (Math_SinS(angle) * 55.0f);
        splashPos.y = contactPos->y;
        splashPos.z = contactPos->z + (Math_CosS(angle) * 55.0f);
        EffectSsGSplash_Spawn(globalCtx, &splashPos, 0, 0, 0, 350);
    }

    EffectSsGRipple_Spawn(globalCtx, contactPos, 300, 700, 0);
    EffectSsGRipple_Spawn(globalCtx, contactPos, 500, 900, 4);
    EffectSsGRipple_Spawn(globalCtx, contactPos, 500, 1300, 8);
}

s32 EnGoroiwa_MoveAndFall(EnGoroiwa* self, GlobalContext* globalCtx) {
    Path* path;
    s32 result;
    s32 pad;
    Vec3s* nextPointPos;

    Math_StepToF(&self->actor.speedXZ, R_EN_GOROIWA_SPEED * 0.01f, 0.3f);
    func_8002D868(&self->actor);
    path = &globalCtx->setupPathList[self->actor.params & 0xFF];
    nextPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->nextWaypoint;
    result = true;
    result &= Math_StepToF(&self->actor.world.pos.x, nextPointPos->x, fabsf(self->actor.velocity.x));
    result &= Math_StepToF(&self->actor.world.pos.z, nextPointPos->z, fabsf(self->actor.velocity.z));
    self->actor.world.pos.y += self->actor.velocity.y;
    return result;
}

s32 EnGoroiwa_Move(EnGoroiwa* self, GlobalContext* globalCtx) {
    Path* path = &globalCtx->setupPathList[self->actor.params & 0xFF];
    s32 pad;
    Vec3s* nextPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->nextWaypoint;
    Vec3s* currentPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->currentWaypoint;
    s32 nextPointReached;
    Vec3f posDiff;
    Vec3f nextPointPosF;

    nextPointPosF.x = nextPointPos->x;
    nextPointPosF.y = nextPointPos->y;
    nextPointPosF.z = nextPointPos->z;
    Math_StepToF(&self->actor.speedXZ, R_EN_GOROIWA_SPEED * 0.01f, 0.3f);
    if (Math3D_Vec3fDistSq(&nextPointPosF, &self->actor.world.pos) < SQ(5.0f)) {
        Math_Vec3f_Diff(&nextPointPosF, &self->actor.world.pos, &posDiff);
    } else {
        posDiff.x = nextPointPosF.x - currentPointPos->x;
        posDiff.y = nextPointPosF.y - currentPointPos->y;
        posDiff.z = nextPointPosF.z - currentPointPos->z;
    }
    EnGoroiwa_Vec3fNormalize(&self->actor.velocity, &posDiff);
    self->actor.velocity.x *= self->actor.speedXZ;
    self->actor.velocity.y *= self->actor.speedXZ;
    self->actor.velocity.z *= self->actor.speedXZ;
    nextPointReached = true;
    nextPointReached &= Math_StepToF(&self->actor.world.pos.x, nextPointPosF.x, fabsf(self->actor.velocity.x));
    nextPointReached &= Math_StepToF(&self->actor.world.pos.y, nextPointPosF.y, fabsf(self->actor.velocity.y));
    nextPointReached &= Math_StepToF(&self->actor.world.pos.z, nextPointPosF.z, fabsf(self->actor.velocity.z));
    return nextPointReached;
}

s32 EnGoroiwa_MoveUpToNextWaypoint(EnGoroiwa* self, GlobalContext* globalCtx) {
    s32 pad;
    Path* path = &globalCtx->setupPathList[self->actor.params & 0xFF];
    Vec3s* nextPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->nextWaypoint;

    Math_StepToF(&self->actor.velocity.y, (R_EN_GOROIWA_SPEED * 0.01f) * 0.5f, 0.18f);
    self->actor.world.pos.x = nextPointPos->x;
    self->actor.world.pos.z = nextPointPos->z;
    return Math_StepToF(&self->actor.world.pos.y, nextPointPos->y, fabsf(self->actor.velocity.y));
}

s32 EnGoroiwa_MoveDownToNextWaypoint(EnGoroiwa* self, GlobalContext* globalCtx) {
    s32 pad;
    Path* path = &globalCtx->setupPathList[self->actor.params & 0xFF];
    Vec3s* nextPointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->nextWaypoint;
    f32 nextPointY;
    f32 thisY;
    f32 yDistToFloor;
    s32 quakeIdx;
    CollisionPoly* floorPoly;
    Vec3f raycastFrom;
    f32 floorY;
    s32 pad2;
    s32 floorBgId;
    Vec3f dustPos;
    WaterBox* waterBox;
    f32 ySurface;
    Vec3f waterHitPos;

    nextPointY = nextPointPos->y;
    Math_StepToF(&self->actor.velocity.y, -14.0f, 1.0f);
    self->actor.world.pos.x = nextPointPos->x;
    self->actor.world.pos.z = nextPointPos->z;
    thisY = self->actor.world.pos.y;
    if (1) {}
    self->actor.world.pos.y += self->actor.velocity.y;
    if (self->actor.velocity.y < 0.0f && self->actor.world.pos.y <= nextPointY) {
        if (self->bounceCount == 0) {
            if (self->actor.xzDistToPlayer < 600.0f) {
                quakeIdx = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
                Quake_SetSpeed(quakeIdx, -0x3CB0);
                Quake_SetQuakeValues(quakeIdx, 3, 0, 0, 0);
                Quake_SetCountdown(quakeIdx, 7);
            }
            self->rollRotSpeed = 0.0f;
            if (!(self->stateFlags & ENGOROIWA_IN_WATER)) {
                raycastFrom.x = self->actor.world.pos.x;
                raycastFrom.y = self->actor.world.pos.y + 50.0f;
                raycastFrom.z = self->actor.world.pos.z;
                floorY = BgCheck_EntityRaycastFloor5(globalCtx, &globalCtx->colCtx, &floorPoly, &floorBgId,
                                                     &self->actor, &raycastFrom);
                yDistToFloor = floorY - (self->actor.world.pos.y - 59.5f);
                if (fabsf(yDistToFloor) < 15.0f) {
                    dustPos.x = self->actor.world.pos.x;
                    dustPos.y = floorY + 10.0f;
                    dustPos.z = self->actor.world.pos.z;
                    EnGoroiwa_SpawnDust(globalCtx, &dustPos);
                }
            }
        }
        if (self->bounceCount >= 1) {
            return true;
        }
        self->bounceCount++;
        self->actor.velocity.y *= -0.3f;
        self->actor.world.pos.y = nextPointY - ((self->actor.world.pos.y - nextPointY) * 0.3f);
    }
    if (self->bounceCount == 0 &&
        WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->actor.world.pos.x, self->actor.world.pos.z,
                                &ySurface, &waterBox) &&
        self->actor.world.pos.y <= ySurface) {
        self->stateFlags |= ENGOROIWA_IN_WATER;
        if (ySurface < thisY) {
            waterHitPos.x = self->actor.world.pos.x;
            waterHitPos.y = ySurface;
            waterHitPos.z = self->actor.world.pos.z;
            EnGoroiwa_SpawnWaterEffects(globalCtx, &waterHitPos);
            self->actor.velocity.y *= 0.2f;
        }
        if (self->actor.velocity.y < -8.0f) {
            self->actor.velocity.y = -8.0f;
        }
    }
    return false;
}

void EnGoroiwa_UpdateRotation(EnGoroiwa* self, GlobalContext* globalCtx) {
    static Vec3f unitY = { 0.0f, 1.0f, 0.0f };
    s32 pad;
    Vec3f* rollAxisPtr;
    f32 rollAngleDiff;
    Vec3f rollAxis;
    Vec3f unitRollAxis;
    MtxF mtx;
    Vec3f unusedDiff;

    if (self->stateFlags & ENGOROIWA_RETAIN_ROT_SPEED) {
        rollAngleDiff = self->prevRollAngleDiff;
    } else {
        self->prevRollAngleDiff = Math3D_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.prevPos) * (1.0f / 59.5f);
        rollAngleDiff = self->prevRollAngleDiff;
    }
    rollAngleDiff *= self->rollRotSpeed;
    rollAxisPtr = &rollAxis;
    if (self->stateFlags & ENGOROIWA_RETAIN_ROT_SPEED) {
        /*
         * EnGoroiwa_GetPrevWaypointDiff has no side effects and its result goes unused,
         * its result was probably meant to be used instead of the actor's velocity in the
         * Math3D_Vec3f_Cross call.
         */
        EnGoroiwa_GetPrevWaypointDiff(self, globalCtx, &unusedDiff);
        Math3D_Vec3f_Cross(&unitY, &self->actor.velocity, rollAxisPtr);
    } else {
        Math3D_Vec3f_Cross(&unitY, &self->actor.velocity, rollAxisPtr);
    }

    if (EnGoroiwa_Vec3fNormalize(&unitRollAxis, rollAxisPtr)) {
        self->prevUnitRollAxis = unitRollAxis;
    } else {
        unitRollAxis = self->prevUnitRollAxis;
    }

    Matrix_RotateAxis(rollAngleDiff, &unitRollAxis, MTXMODE_NEW);
    Matrix_RotateY(self->actor.shape.rot.y * (2.0f * M_PI / 0x10000), MTXMODE_APPLY);
    Matrix_RotateX(self->actor.shape.rot.x * (2.0f * M_PI / 0x10000), MTXMODE_APPLY);
    Matrix_RotateZ(self->actor.shape.rot.z * (2.0f * M_PI / 0x10000), MTXMODE_APPLY);
    Matrix_Get(&mtx);
    Matrix_MtxFToYXZRotS(&mtx, &self->actor.shape.rot, 0);
}

void EnGoroiwa_NextWaypoint(EnGoroiwa* self, GlobalContext* globalCtx) {
    s16 loopMode = (self->actor.params >> 8) & 3;

    EnGoroiwa_SetNextWaypoint(self);

    if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY || loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK) {
        if (self->currentWaypoint == 0 || self->currentWaypoint == self->endWaypoint) {
            EnGoroiwa_TeleportToWaypoint(self, globalCtx, self->currentWaypoint);
        }
    }

    EnGoroiwa_FaceNextWaypoint(self, globalCtx);
}

void EnGoroiwa_SpawnFragments(EnGoroiwa* self, GlobalContext* globalCtx) {
    static f32 yOffsets[] = { 0.0f, 59.5f };
    s16 angle1;
    s16 angle2;
    s32 pad;
    Vec3f* thisPos = &self->actor.world.pos;
    Vec3f effectPos;
    Vec3f fragmentVelocity;
    f32 cos1;
    f32 sin1;
    f32 sin2;
    s16 yOffsetIdx = (self->actor.params >> 10) & 1;
    s32 i;

    for (i = 0, angle1 = 0; i < 16; i++, angle1 += 0x4E20) {
        sin1 = Math_SinS(angle1);
        cos1 = Math_CosS(angle1);
        angle2 = Rand_ZeroOne() * 0xFFFF;
        effectPos.x = Rand_ZeroOne() * 50.0f * sin1 * Math_SinS(angle2);
        sin2 = Math_SinS(angle2);
        effectPos.y = (Rand_ZeroOne() - 0.5f) * 100.0f * sin2 + yOffsets[yOffsetIdx];
        effectPos.z = Rand_ZeroOne() * 50.0f * cos1 * Math_SinS(angle2);
        fragmentVelocity.x = effectPos.x * 0.2f;
        fragmentVelocity.y = Rand_ZeroOne() * 15.0f + 2.0f;
        fragmentVelocity.z = effectPos.z * 0.2f;
        Math_Vec3f_Sum(&effectPos, thisPos, &effectPos);
        EffectSsKakera_Spawn(globalCtx, &effectPos, &fragmentVelocity, &effectPos, -340, 33, 28, 2, 0,
                             Rand_ZeroOne() * 7.0f + 1.0f, 1, 0, 70, KAKERA_COLOR_NONE, 1, gBoulderFragmentsDL);
    }

    effectPos.x = thisPos->x;
    effectPos.y = thisPos->y + yOffsets[yOffsetIdx];
    effectPos.z = thisPos->z;
    func_80033480(globalCtx, &effectPos, 80.0f, 5, 70, 110, 1);
    func_80033480(globalCtx, &effectPos, 90.0f, 5, 110, 160, 1);
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -860, ICHAIN_CONTINUE), ICHAIN_F32_DIV1000(minVelocityY, -15000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),  ICHAIN_F32(uncullZoneForward, 1500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 150, ICHAIN_CONTINUE),  ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_STOP),
};

void EnGoroiwa_Init(Actor* thisx, GlobalContext* globalCtx) {
    static f32 yOffsets[] = { 0.0f, 595.0f };
    EnGoroiwa* self = THIS;
    s32 pathIdx;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    EnGoroiwa_InitCollider(self, globalCtx);
    pathIdx = self->actor.params & 0xFF;
    if (pathIdx == 0xFF) {
        // "Error: Invalid arg_data"
        osSyncPrintf("Ｅｒｒｏｒ : arg_data が不正(%s %d)(arg_data 0x%04x)\n", "../z_en_gr.c", 1033,
                     self->actor.params);
        Actor_Kill(&self->actor);
        return;
    }
    if (globalCtx->setupPathList[pathIdx].count < 2) {
        // "Error: Invalid Path Data"
        osSyncPrintf("Ｅｒｒｏｒ : レールデータ が不正(%s %d)\n", "../z_en_gr.c", 1043);
        Actor_Kill(&self->actor);
        return;
    }
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    ActorShape_Init(&self->actor.shape, yOffsets[(self->actor.params >> 10) & 1], ActorShadow_DrawCircle, 9.4f);
    self->actor.shape.shadowAlpha = 200;
    EnGoroiwa_SetSpeed(self, globalCtx);
    EnGoroiwa_InitPath(self, globalCtx);
    EnGoroiwa_TeleportToWaypoint(self, globalCtx, 0);
    EnGoroiwa_InitRotation(self);
    EnGoroiwa_FaceNextWaypoint(self, globalCtx);
    EnGoroiwa_SetupRoll(self);
    // "(Goroiwa)"
    osSyncPrintf("(ごろ岩)(arg 0x%04x)(rail %d)(end %d)(bgc %d)(hit %d)\n", self->actor.params,
                 self->actor.params & 0xFF, (self->actor.params >> 8) & 3, (self->actor.params >> 10) & 1,
                 self->actor.home.rot.z & 1);
}

void EnGoroiwa_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnGoroiwa* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnGoroiwa_SetupRoll(EnGoroiwa* self) {
    self->actionFunc = EnGoroiwa_Roll;
    EnGoroiwa_UpdateFlags(self, ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC);
    self->rollRotSpeed = 1.0f;
}

void EnGoroiwa_Roll(EnGoroiwa* self, GlobalContext* globalCtx) {
    static EnGoroiwaUnkFunc1 moveFuncs[] = { EnGoroiwa_Move, EnGoroiwa_MoveAndFall };
    static EnGoroiwaUnkFunc2 onHitSetupFuncs[] = { EnGoroiwa_SetupWait, EnGoroiwa_SetupMoveAndFallToGround };

    s32 ascendDirection;
    s16 yawDiff;
    s16 loopMode;

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        self->stateFlags &= ~ENGOROIWA_PLAYER_IN_THE_WAY;
        yawDiff = self->actor.yawTowardsPlayer - self->actor.world.rot.y;
        if (yawDiff > -0x4000 && yawDiff < 0x4000) {
            self->stateFlags |= ENGOROIWA_PLAYER_IN_THE_WAY;
            if (((self->actor.params >> 10) & 1) || (self->actor.home.rot.z & 1) != 1) {
                EnGoroiwa_ReverseDirection(self);
                EnGoroiwa_FaceNextWaypoint(self, globalCtx);
            }
        }
        func_8002F6D4(globalCtx, &self->actor, 2.0f, self->actor.yawTowardsPlayer, 0.0f, 0);
        osSyncPrintf(VT_FGCOL(CYAN));
        osSyncPrintf("Player ぶっ飛ばし\n"); // "Player knocked down"
        osSyncPrintf(VT_RST);
        onHitSetupFuncs[(self->actor.params >> 10) & 1](self);
        func_8002F7DC(&GET_PLAYER(globalCtx)->actor, NA_SE_PL_BODY_HIT);
        if ((self->actor.home.rot.z & 1) == 1) {
            self->collisionDisabledTimer = 50;
        }
    } else if (moveFuncs[(self->actor.params >> 10) & 1](self, globalCtx)) {
        loopMode = (self->actor.params >> 8) & 3;
        if (loopMode == ENGOROIWA_LOOPMODE_ONEWAY_BREAK &&
            (self->nextWaypoint == 0 || self->nextWaypoint == self->endWaypoint)) {
            EnGoroiwa_SpawnFragments(self, globalCtx);
        }
        EnGoroiwa_NextWaypoint(self, globalCtx);
        if ((loopMode == ENGOROIWA_LOOPMODE_ROUNDTRIP) &&
            (self->currentWaypoint == 0 || self->currentWaypoint == self->endWaypoint)) {
            EnGoroiwa_SetupWait(self);
        } else if (!((self->actor.params >> 10) & 1) && self->currentWaypoint != 0 &&
                   self->currentWaypoint != self->endWaypoint) {
            ascendDirection = EnGoroiwa_GetAscendDirection(self, globalCtx);
            if (ascendDirection > 0) {
                EnGoroiwa_SetupMoveUp(self);
            } else if (ascendDirection < 0) {
                EnGoroiwa_SetupMoveDown(self);
            } else {
                EnGoroiwa_SetupRoll(self);
            }
        } else {
            EnGoroiwa_SetupRoll(self);
        }
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_BIGBALL_ROLL - SFX_FLAG);
}

void EnGoroiwa_SetupMoveAndFallToGround(EnGoroiwa* self) {
    self->actionFunc = EnGoroiwa_MoveAndFallToGround;
    EnGoroiwa_UpdateFlags(self, ENGOROIWA_ENABLE_OC);
    self->actor.gravity = -0.86f;
    self->actor.minVelocityY = -15.0f;
    self->actor.speedXZ *= 0.15f;
    self->actor.velocity.y = 5.0f;
    self->rollRotSpeed = 1.0f;
}

void EnGoroiwa_MoveAndFallToGround(EnGoroiwa* self, GlobalContext* globalCtx) {
    EnGoroiwa_MoveAndFall(self, globalCtx);
    if ((self->actor.bgCheckFlags & 1) && self->actor.velocity.y < 0.0f) {
        if ((self->stateFlags & ENGOROIWA_PLAYER_IN_THE_WAY) && (self->actor.home.rot.z & 1) == 1) {
            EnGoroiwa_ReverseDirection(self);
            EnGoroiwa_FaceNextWaypoint(self, globalCtx);
        }
        EnGoroiwa_SetupWait(self);
    }
}

void EnGoroiwa_SetupWait(EnGoroiwa* self) {
    static s16 waitDurations[] = { 20, 6 };

    self->actionFunc = EnGoroiwa_Wait;
    self->actor.speedXZ = 0.0f;
    EnGoroiwa_UpdateFlags(self, ENGOROIWA_ENABLE_OC);
    self->waitTimer = waitDurations[self->actor.home.rot.z & 1];
    self->rollRotSpeed = 0.0f;
}

void EnGoroiwa_Wait(EnGoroiwa* self, GlobalContext* globalCtx) {
    if (self->waitTimer > 0) {
        self->waitTimer--;
    } else {
        self->collider.base.atFlags &= ~AT_HIT;
        EnGoroiwa_SetupRoll(self);
    }
}

void EnGoroiwa_SetupMoveUp(EnGoroiwa* self) {
    self->actionFunc = EnGoroiwa_MoveUp;
    EnGoroiwa_UpdateFlags(self, ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC);
    self->rollRotSpeed = 0.0f;
    self->actor.velocity.y = fabsf(self->actor.speedXZ) * 0.1f;
}

void EnGoroiwa_MoveUp(EnGoroiwa* self, GlobalContext* globalCtx) {
    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        func_8002F6D4(globalCtx, &self->actor, 2.0f, self->actor.yawTowardsPlayer, 0.0f, 4);
        func_8002F7DC(&GET_PLAYER(globalCtx)->actor, NA_SE_PL_BODY_HIT);
        if ((self->actor.home.rot.z & 1) == 1) {
            self->collisionDisabledTimer = 50;
        }
    } else if (EnGoroiwa_MoveUpToNextWaypoint(self, globalCtx)) {
        EnGoroiwa_NextWaypoint(self, globalCtx);
        EnGoroiwa_SetupRoll(self);
        self->actor.speedXZ = 0.0f;
    }
}

void EnGoroiwa_SetupMoveDown(EnGoroiwa* self) {
    self->actionFunc = EnGoroiwa_MoveDown;
    EnGoroiwa_UpdateFlags(self, ENGOROIWA_ENABLE_AT | ENGOROIWA_ENABLE_OC);
    self->rollRotSpeed = 0.3f;
    self->bounceCount = 0;
    self->actor.velocity.y = fabsf(self->actor.speedXZ) * -0.3f;
    self->stateFlags |= ENGOROIWA_RETAIN_ROT_SPEED;
    self->stateFlags &= ~ENGOROIWA_IN_WATER;
}

void EnGoroiwa_MoveDown(EnGoroiwa* self, GlobalContext* globalCtx) {
    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        func_8002F6D4(globalCtx, &self->actor, 2.0f, self->actor.yawTowardsPlayer, 0.0f, 4);
        func_8002F7DC(&GET_PLAYER(globalCtx)->actor, NA_SE_PL_BODY_HIT);
        if ((self->actor.home.rot.z & 1) == 1) {
            self->collisionDisabledTimer = 50;
        }
    } else if (EnGoroiwa_MoveDownToNextWaypoint(self, globalCtx)) {
        EnGoroiwa_NextWaypoint(self, globalCtx);
        EnGoroiwa_SetupRoll(self);
        self->stateFlags &= ~ENGOROIWA_RETAIN_ROT_SPEED;
        self->actor.speedXZ = 0.0f;
    }
}

void EnGoroiwa_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGoroiwa* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    UNK_TYPE sp30;

    if (!(player->stateFlags1 & 0x300000C0)) {
        if (self->collisionDisabledTimer > 0) {
            self->collisionDisabledTimer--;
        }
        self->actionFunc(self, globalCtx);
        switch ((self->actor.params >> 10) & 1) {
            case 1:
                Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 0x1C);
                break;
            case 0:
                self->actor.floorHeight = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->actor.floorPoly, &sp30,
                                                                      &self->actor, &self->actor.world.pos);
                break;
        }
        EnGoroiwa_UpdateRotation(self, globalCtx);
        if (self->actor.xzDistToPlayer < 300.0f) {
            EnGoroiwa_UpdateCollider(self);
            if ((self->stateFlags & ENGOROIWA_ENABLE_AT) && self->collisionDisabledTimer <= 0) {
                CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            }
            if ((self->stateFlags & ENGOROIWA_ENABLE_OC) && self->collisionDisabledTimer <= 0) {
                CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            }
        }
    }
}

void EnGoroiwa_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gRollingRockDL);
}
