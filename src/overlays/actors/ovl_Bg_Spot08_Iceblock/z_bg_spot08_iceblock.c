/*
 * File: z_bg_spot08_iceblock.c
 * Overlay: ovl_Bg_Spot08_Iceblock
 * Description: Floating ice platforms
 */

#include "z_bg_spot08_iceblock.h"
#include "objects/object_spot08_obj/object_spot08_obj.h"

#define FLAGS 0x00000000

#define THIS ((BgSpot08Iceblock*)thisx)

void BgSpot08Iceblock_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Iceblock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Iceblock_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Iceblock_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgSpot08Iceblock_SetupFloatNonrotating(BgSpot08Iceblock* self);
void BgSpot08Iceblock_FloatNonrotating(BgSpot08Iceblock* self, GlobalContext* globalCtx);
void BgSpot08Iceblock_SetupFloatRotating(BgSpot08Iceblock* self);
void BgSpot08Iceblock_FloatRotating(BgSpot08Iceblock* self, GlobalContext* globalCtx);
void BgSpot08Iceblock_SetupFloatOrbitingTwins(BgSpot08Iceblock* self);
void BgSpot08Iceblock_FloatOrbitingTwins(BgSpot08Iceblock* self, GlobalContext* globalCtx);
void BgSpot08Iceblock_SetupNoAction(BgSpot08Iceblock* self);

const ActorInit Bg_Spot08_Iceblock_InitVars = {
    ACTOR_BG_SPOT08_ICEBLOCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT08_OBJ,
    sizeof(BgSpot08Iceblock),
    (ActorFunc)BgSpot08Iceblock_Init,
    (ActorFunc)BgSpot08Iceblock_Destroy,
    (ActorFunc)BgSpot08Iceblock_Update,
    (ActorFunc)BgSpot08Iceblock_Draw,
};

void BgSpot08Iceblock_SetupAction(BgSpot08Iceblock* self, BgSpot08IceblockActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgSpot08Iceblock_InitDynaPoly(BgSpot08Iceblock* self, GlobalContext* globalCtx, CollisionHeader* collision,
                                   s32 flags) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, flags);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning: move BG registration failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_spot08_iceblock.c", 0xD9,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

// Sets params to 0x10 (medium, nonrotating) if not in the cases listed.
void BgSpot08Iceblock_CheckParams(BgSpot08Iceblock* self) {
    switch (self->dyna.actor.params & 0xFF) {
        case 0xFF:
            self->dyna.actor.params = 0x10;
            break;
        default:
            // "Error: arg_data setting error"
            osSyncPrintf("Error : arg_data 設定ミスです。(%s %d)(arg_data 0x%04x)\n", "../z_bg_spot08_iceblock.c", 0xF6,
                         self->dyna.actor.params);
            self->dyna.actor.params = 0x10;
            break;
        case 1:
        case 4:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x14:
        case 0x20:
        case 0x23:
        case 0x24:
            break;
    }
}

void BgSpot08Iceblock_Bobbing(BgSpot08Iceblock* self) {
    self->bobOffset = (Math_SinS(self->bobPhaseSlow) * 4.0f) + (Math_SinS(self->bobPhaseFast) * 3.0f);
}

void BgSpot08Iceblock_SinkUnderPlayer(BgSpot08Iceblock* self) {
    f32 target;
    f32 step;

    switch (self->dyna.actor.params & 0xF0) {
        case 0:
            step = 0.15f;
            break;
        case 0x10:
            step = 0.2f;
            break;
        case 0x20:
            step = 0.4f;
            break;
    }

    // Sink under Player's weight if standing on it
    target = (func_80043548(&self->dyna) ? -4.0f : 0.0f);

    Math_StepToF(&self->sinkOffset, target, step);
}

void BgSpot08Iceblock_SetWaterline(BgSpot08Iceblock* self) {
    self->dyna.actor.world.pos.y = self->sinkOffset + self->bobOffset + self->dyna.actor.home.pos.y;
}

void BgSpot08Iceblock_MultVectorScalar(Vec3f* dest, Vec3f* v, f32 scale) {
    dest->x = v->x * scale;
    dest->y = v->y * scale;
    dest->z = v->z * scale;
}

void BgSpot08Iceblock_CrossProduct(Vec3f* dest, Vec3f* v1, Vec3f* v2) {
    dest->x = (v1->y * v2->z) - (v1->z * v2->y);
    dest->y = (v1->z * v2->x) - (v1->x * v2->z);
    dest->z = (v1->x * v2->y) - (v1->y * v2->x);
}

s32 BgSpot08Iceblock_NormalizeVector(Vec3f* dest, Vec3f* v) {
    f32 magnitude;

    magnitude = Math3D_Vec3fMagnitude(v);
    if (magnitude < 0.001f) {
        dest->x = dest->y = 0.0f;
        dest->z = 1.0f;
        return false;
    } else {
        dest->x = v->x * (1.0f / magnitude);
        dest->y = v->y * (1.0f / magnitude);
        dest->z = v->z * (1.0f / magnitude);
        return true;
    }
}

static Vec3f sVerticalVector = { 0.0f, 1.0f, 0.0f };
static Vec3f sZeroVector = { 0.0f, 0.0f, 0.0f };
static f32 sInertias[] = { 1.0f / 70000000, 1.0f / 175000000, 1.0f / 700000000 };
static f32 sDampingFactors[] = { 0.96f, 0.96f, 0.98f };

static f32 sRollSins[] = {
    0.22495104f, // sin(13 degrees)
    0.22495104f, // sin(13 degrees)
    0.03489947f, // sin(2 degrees)
};

static f32 sRollCoss[] = {
    0.97437006f, // cos(13 degrees)
    0.97437006f, // cos(13 degrees)
    0.99939084f, // cos(2 degrees)
};

/**
 *  Handles all the factors that influence rolling: inertia, random oscillations, and most significantly, player weight,
 * and combines them to produce a matrix that rotates the actor to match the surface normal
 */
void BgSpot08Iceblock_Roll(BgSpot08Iceblock* self, GlobalContext* globalCtx) {
    f32 deviationFromVertSq;
    f32 stabilityCorrection;
    Vec3f surfaceNormalHorizontal;
    Vec3f playerCentroidDiff;
    Vec3f playerMoment;
    Vec3f surfaceNormalHorizontalScaled;
    Vec3f randomNutation;
    Vec3f tempVec; // reused with different meanings
    Vec3f torqueDirection;
    f32 playerCentroidDist;
    s32 rollDataIndex;
    MtxF mtx;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    switch (self->dyna.actor.params & 0xFF) {
        case 0x11: // Medium nonrotating
            rollDataIndex = 0;
            break;
        case 1:
            rollDataIndex = 1; // Large nonrotating
            break;
        default:
            rollDataIndex = 2;
            break;
    }

    Math_Vec3f_Diff(&player->actor.world.pos, &self->dyna.actor.world.pos, &playerCentroidDiff);
    playerCentroidDiff.y -= (150.0f * self->dyna.actor.scale.y);
    playerCentroidDist = Math3D_Vec3fMagnitude(&playerCentroidDiff);

    randomNutation.x = (Rand_ZeroOne() - 0.5f) * (1.0f / 625);
    randomNutation.y = 0.0f;
    randomNutation.z = (Rand_ZeroOne() - 0.5f) * (1.0f / 625);

    surfaceNormalHorizontal.x = self->surfaceNormal.x;
    surfaceNormalHorizontal.y = 0.0f;
    surfaceNormalHorizontal.z = self->surfaceNormal.z;

    // If player is standing on it or holding the edge
    if (func_8004356C(&self->dyna) && (playerCentroidDist > 3.0f)) {
        Math_Vec3f_Diff(&playerCentroidDiff, &surfaceNormalHorizontal, &playerMoment);
        BgSpot08Iceblock_MultVectorScalar(&playerMoment, &playerMoment,
                                          (sInertias[rollDataIndex] * playerCentroidDist) / self->dyna.actor.scale.x);
    } else {
        playerMoment = sZeroVector;
    }

    BgSpot08Iceblock_MultVectorScalar(&surfaceNormalHorizontalScaled, &surfaceNormalHorizontal, -0.01f);

    // Add all three deviations
    Math_Vec3f_Sum(&self->normalDelta, &playerMoment, &self->normalDelta);
    Math_Vec3f_Sum(&self->normalDelta, &surfaceNormalHorizontalScaled, &self->normalDelta);
    Math_Vec3f_Sum(&self->normalDelta, &randomNutation, &self->normalDelta);

    self->normalDelta.y = 0.0f;

    Math_Vec3f_Sum(&self->surfaceNormal, &self->normalDelta, &tempVec);

    tempVec.x *= sDampingFactors[rollDataIndex];
    tempVec.z *= sDampingFactors[rollDataIndex];

    // Set up roll axis and final new angle
    if (BgSpot08Iceblock_NormalizeVector(&self->surfaceNormal, &tempVec)) {
        deviationFromVertSq = Math3D_Dist1DSq(self->surfaceNormal.z, self->surfaceNormal.x);

        // Prevent overrolling
        if (sRollSins[rollDataIndex] < deviationFromVertSq) {
            stabilityCorrection = sRollSins[rollDataIndex] / deviationFromVertSq;

            self->surfaceNormal.x *= stabilityCorrection;
            self->surfaceNormal.y = sRollCoss[rollDataIndex];
            self->surfaceNormal.z *= stabilityCorrection;
        }

        BgSpot08Iceblock_CrossProduct(&tempVec, &sVerticalVector, &self->surfaceNormal);

        if (BgSpot08Iceblock_NormalizeVector(&torqueDirection, &tempVec)) {
            self->rotationAxis = torqueDirection;
        }
    } else {
        self->surfaceNormal = sVerticalVector;
    }

    // Rotation by the angle between surfaceNormal and the vertical about rotationAxis
    Matrix_RotateAxis(Math_FAcosF(Math3D_Cos(&sVerticalVector, &self->surfaceNormal)), &self->rotationAxis,
                      MTXMODE_NEW);
    Matrix_RotateY(self->dyna.actor.shape.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_Get(&mtx);
    Matrix_MtxFToYXZRotS(&mtx, &self->dyna.actor.shape.rot, 0);
}

void BgSpot08Iceblock_SpawnTwinFloe(BgSpot08Iceblock* self, GlobalContext* globalCtx) {
    s32 pad[2];
    f32 sin;
    f32 cos;

    sin = Math_SinS(self->dyna.actor.home.rot.y) * 100.0f;
    cos = Math_CosS(self->dyna.actor.home.rot.y) * 100.0f;

    if (!(self->dyna.actor.params & 0x100)) {
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_BG_SPOT08_ICEBLOCK,
                           self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y, self->dyna.actor.home.pos.z,
                           self->dyna.actor.home.rot.x, self->dyna.actor.home.rot.y, self->dyna.actor.home.rot.z,
                           0x123);

        self->dyna.actor.world.pos.x += sin;
        self->dyna.actor.world.pos.z += cos;
    } else {
        self->dyna.actor.world.pos.x -= sin;
        self->dyna.actor.world.pos.z -= cos;
    }
    BgSpot08Iceblock_SetupFloatOrbitingTwins(self);
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2200, ICHAIN_STOP),
};

void BgSpot08Iceblock_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Iceblock* self = THIS;
    CollisionHeader* colHeader;

    // "spot08 ice floe"
    osSyncPrintf("(spot08 流氷)(arg_data 0x%04x)\n", self->dyna.actor.params);
    BgSpot08Iceblock_CheckParams(self);

    switch (self->dyna.actor.params & 0x200) {
        case 0:
            colHeader = &gZorasFountainIcebergCol;
            break;
        case 0x200:
            colHeader = &gZorasFountainIceRampCol;
            break;
    }

    switch (self->dyna.actor.params & 0xF) {
        case 2:
        case 3:
            BgSpot08Iceblock_InitDynaPoly(self, globalCtx, colHeader, DPM_UNK3);
            break;
        default:
            BgSpot08Iceblock_InitDynaPoly(self, globalCtx, colHeader, DPM_UNK);
            break;
    }

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        Actor_Kill(&self->dyna.actor);
        return;
    }

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);

    switch (self->dyna.actor.params & 0xF0) {
        case 0:
            Actor_SetScale(&self->dyna.actor, 0.2f);
            break;
        case 0x10:
            Actor_SetScale(&self->dyna.actor, 0.1f);
            break;
        case 0x20:
            Actor_SetScale(&self->dyna.actor, 0.05f);
            break;
    }

    self->bobPhaseSlow = (s32)(Rand_ZeroOne() * (0xFFFF + 0.5f));
    self->bobPhaseFast = (s32)(Rand_ZeroOne() * (0xFFFF + 0.5f));
    self->surfaceNormal.y = 1.0f;
    self->rotationAxis.x = 1.0f;

    switch (self->dyna.actor.params & 0xF) {
        case 0:
        case 1:
            BgSpot08Iceblock_SetupFloatNonrotating(self);
            break;
        case 2:
            BgSpot08Iceblock_SetupFloatRotating(self);
            break;
        case 3:
            BgSpot08Iceblock_SpawnTwinFloe(self, globalCtx);
            break;
        case 4:
            BgSpot08Iceblock_SetupNoAction(self);
            break;
    }
}

void BgSpot08Iceblock_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Iceblock* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgSpot08Iceblock_SetupFloatNonrotating(BgSpot08Iceblock* self) {
    BgSpot08Iceblock_SetupAction(self, BgSpot08Iceblock_FloatNonrotating);
}

void BgSpot08Iceblock_FloatNonrotating(BgSpot08Iceblock* self, GlobalContext* globalCtx) {
    BgSpot08Iceblock_Bobbing(self);
    BgSpot08Iceblock_SinkUnderPlayer(self);
    BgSpot08Iceblock_SetWaterline(self);
    self->dyna.actor.shape.rot.y = self->dyna.actor.home.rot.y;
    BgSpot08Iceblock_Roll(self, globalCtx);
}

void BgSpot08Iceblock_SetupFloatRotating(BgSpot08Iceblock* self) {
    BgSpot08Iceblock_SetupAction(self, BgSpot08Iceblock_FloatRotating);
}

void BgSpot08Iceblock_FloatRotating(BgSpot08Iceblock* self, GlobalContext* globalCtx) {
    BgSpot08Iceblock_Bobbing(self);
    BgSpot08Iceblock_SinkUnderPlayer(self);
    BgSpot08Iceblock_SetWaterline(self);
    self->dyna.actor.world.rot.y = self->dyna.actor.world.rot.y + 0x190;
    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
    BgSpot08Iceblock_Roll(self, globalCtx);
}

void BgSpot08Iceblock_SetupFloatOrbitingTwins(BgSpot08Iceblock* self) {
    BgSpot08Iceblock_SetupAction(self, BgSpot08Iceblock_FloatOrbitingTwins);
}

void BgSpot08Iceblock_FloatOrbitingTwins(BgSpot08Iceblock* self, GlobalContext* globalCtx) {
    f32 cos;
    f32 sin;

    BgSpot08Iceblock_Bobbing(self);
    BgSpot08Iceblock_SinkUnderPlayer(self);
    BgSpot08Iceblock_SetWaterline(self);

    // parent handles rotations of both
    if (!(self->dyna.actor.params & 0x100)) {
        self->dyna.actor.world.rot.y += 0x190;
        sin = Math_SinS(self->dyna.actor.world.rot.y) * 100.0f;
        cos = Math_CosS(self->dyna.actor.world.rot.y) * 100.0f;

        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x + sin;
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z + cos;

        if (self->dyna.actor.child != NULL) {
            self->dyna.actor.child->world.pos.x = self->dyna.actor.home.pos.x - sin;
            self->dyna.actor.child->world.pos.z = self->dyna.actor.home.pos.z - cos;
        }
    }

    self->dyna.actor.shape.rot.y = self->dyna.actor.home.rot.y;
    BgSpot08Iceblock_Roll(self, globalCtx);
}

void BgSpot08Iceblock_SetupNoAction(BgSpot08Iceblock* self) {
    BgSpot08Iceblock_SetupAction(self, NULL);
}

void BgSpot08Iceblock_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Iceblock* self = THIS;

    if (Rand_ZeroOne() < 0.05f) {
        self->bobIncrSlow = Rand_S16Offset(300, 100);
        self->bobIncrFast = Rand_S16Offset(800, 400);
    }

    self->bobPhaseSlow += self->bobIncrSlow;
    self->bobPhaseFast += self->bobIncrFast;
    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
}

void BgSpot08Iceblock_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx* dList;
    BgSpot08Iceblock* self = THIS;

    switch (self->dyna.actor.params & 0x200) {
        case 0:
            dList = gZorasFountainIcebergDL;
            break;
        case 0x200:
            dList = gZorasFountainIceRampDL;
            break;
    }

    Gfx_DrawDListOpa(globalCtx, dList);
}
