/*
 * File: z_obj_bean.c
 * Overlay: ovl_Obj_Bean
 * Description: Bean plant spot
 */

#include "z_obj_bean.h"
#include "objects/object_mamenoki/object_mamenoki.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "vt.h"

#define FLAGS 0x00400000

#define THIS ((ObjBean*)thisx)

void ObjBean_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjBean_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjBean_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjBean_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjBean_WaitForPlayer(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_Fly(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_SetupFly(ObjBean* self);
void ObjBean_WaitForWater(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_SetupWaitForWater(ObjBean* self);
void ObjBean_SetupGrowWaterPhase1(ObjBean* self);
void ObjBean_GrowWaterPhase1(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_GrowWaterPhase2(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_SetupGrowWaterPhase2(ObjBean* self);
void ObjBean_GrowWaterPhase3(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_SetupGrowWaterPhase3(ObjBean* self);
void ObjBean_SetupGrown(ObjBean* self);
void ObjBean_FlattenLeaves(ObjBean* self);
void ObjBean_Grown(ObjBean* self);
void ObjBean_LeavesStill(ObjBean* self);
void ObjBean_SetupShakeLeaves(ObjBean* self);
void ObjBean_ShakeLeaves(ObjBean* self);
void ObjBean_SetupWaitForBean(ObjBean* self);
void ObjBean_WaitForBean(ObjBean* self, GlobalContext* globalCtx);
void func_80B8FE3C(ObjBean* self, GlobalContext* globalCtx);
void func_80B8FE00(ObjBean* self);
void func_80B8FE6C(ObjBean* self);
void func_80B8FEAC(ObjBean* self, GlobalContext* globalCtx);
void func_80B8FF50(ObjBean* self);
void ObjBean_SetupGrowWaterPhase4(ObjBean* self);
void func_80B8FF8C(ObjBean* self, GlobalContext* globalCtx);
void func_80B90050(ObjBean* self, GlobalContext* globalCtx);
void func_80B90010(ObjBean* self);
void func_80B908EC(ObjBean* self);
void func_80B90918(ObjBean* self, GlobalContext* globalCtx);
void func_80B90970(ObjBean* self);
void func_80B909B0(ObjBean* self, GlobalContext* globalCtx);
void func_80B909F8(ObjBean* self);
void func_80B90A34(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_SetupWaitForPlayer(ObjBean* self);
void ObjBean_GrowWaterPhase4(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_GrowWaterPhase5(ObjBean* self, GlobalContext* globalCtx);
void ObjBean_SetupGrowWaterPhase5(ObjBean* self);
void ObjBean_SetupShakeLeavesFast(ObjBean* self);
void ObjBean_ShakeLeavesFast(ObjBean* self);
void ObjBean_Grow(ObjBean* self);
void ObjBean_SetupGrow(ObjBean* self);
void ObjBean_SetupWaitForStepOff(ObjBean* self);
void ObjBean_WaitForStepOff(ObjBean* self, GlobalContext* globalCtx);

#define BEAN_STATE_DRAW_LEAVES (1 << 0)
#define BEAN_STATE_DRAW_SOIL (1 << 1)
#define BEAN_STATE_DRAW_PLANT (1 << 2)
#define BEAN_STATE_DRAW_STALK (1 << 3)
#define BEAN_STATE_COLLIDER_SET (1 << 4)
#define BEAN_STATE_DYNAPOLY_SET (1 << 5)
#define BEAN_STATE_BEEN_WATERED (1 << 6)
#define BEAN_STATE_PLAYER_ON_TOP (1 << 7)

static ObjBean* D_80B90E30 = NULL;

const ActorInit Obj_Bean_InitVars = {
    ACTOR_OBJ_BEAN,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MAMENOKI,
    sizeof(ObjBean),
    (ActorFunc)ObjBean_Init,
    (ActorFunc)ObjBean_Destroy,
    (ActorFunc)ObjBean_Update,
    (ActorFunc)ObjBean_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
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
    { 64, 30, -31, { 0, 0, 0 } },
};

typedef struct {
    f32 velocity;
    f32 accel;
} BeenSpeedInfo;

static BeenSpeedInfo sBeanSpeeds[] = {
    { 3.0f, 0.3f },
    { 10.0f, 0.5f },
    { 30.0f, 0.5f },
    { 3.0f, 0.3f },
};

static Gfx* sBreakDlists[] = { gCuttableShrubStalkDL, gCuttableShrubTipDL };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1600, ICHAIN_STOP),
};

void ObjBean_InitCollider(Actor* thisx, GlobalContext* globalCtx) {
    ObjBean* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->dyna.actor, &sCylinderInit);
    Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
}

void ObjBean_InitDynaPoly(ObjBean* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader;
    s32 pad2;

    colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    CollisionHeader_GetVirtual(collision, &colHeader);

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_obj_bean.c", 374,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void ObjBean_FindFloor(ObjBean* self, GlobalContext* globalCtx) {
    Vec3f vec;
    s32 sp20;

    vec.x = self->dyna.actor.world.pos.x;
    vec.y = self->dyna.actor.world.pos.y + 29.999998f;
    vec.z = self->dyna.actor.world.pos.z;
    self->dyna.actor.floorHeight =
        BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->dyna.actor.floorPoly, &sp20, &self->dyna.actor, &vec);
}

void func_80B8EBC8(ObjBean* self) {
    self->unk_1B6.x = self->unk_1B6.y = self->unk_1B6.z = 0;
    self->unk_1E4 = 0.0f;
}

void ObjBean_UpdatePosition(ObjBean* self) {
    f32 temp_f20;

    self->unk_1B6.x += 0xB6;
    self->unk_1B6.y += 0xFB;
    self->unk_1B6.z += 0x64;

    Math_StepToF(&self->unk_1E4, 2.0f, 0.1f);
    temp_f20 = Math_SinS(self->unk_1B6.x * 3);
    self->posOffsetX = (Math_SinS(((self->unk_1B6.y * 3))) + temp_f20) * self->unk_1E4;
    temp_f20 = Math_CosS(self->unk_1B6.x * 4);
    self->posOffsetZ = (Math_CosS((self->unk_1B6.y * 4)) + temp_f20) * self->unk_1E4;
    temp_f20 = Math_SinS(self->unk_1B6.z * 5);

    self->dyna.actor.scale.x = self->dyna.actor.scale.z =
        ((Math_SinS((self->unk_1B6.y * 8)) * 0.01f) + (temp_f20 * 0.06f) + 1.07f) * 0.1f;

    self->dyna.actor.scale.y = ((Math_CosS(((self->unk_1B6.z * 10))) * 0.2f) + 1.0f) * 0.1f;
    temp_f20 = Math_SinS(self->unk_1B6.x * 3);
    self->dyna.actor.shape.rot.y =
        (Math_SinS((s16)(self->unk_1B6.z * 2)) * 2100.0f) + ((f32)self->dyna.actor.home.rot.y + (temp_f20 * 1000.0f));
}

void func_80B8EDF4(ObjBean* self) {
    self->unk_1B6.x = self->unk_1B6.y = self->unk_1B6.z = 0;

    Actor_SetScale(&self->dyna.actor, 0.0f);
}

void func_80B8EE24(ObjBean* self) {
    self->unk_1B6.x += 0x384;
    if (self->unk_1B6.x > 0x5FFF) {
        self->unk_1B6.x = 0x5FFF;
    }
    self->unk_1B6.y += 0x258;
    if (self->unk_1B6.y > 0x4000) {
        self->unk_1B6.y = 0x4000;
    }
    self->dyna.actor.scale.y = Math_SinS(self->unk_1B6.x) * 0.16970563f;

    self->dyna.actor.scale.x = self->dyna.actor.scale.z = Math_SinS(self->unk_1B6.y) * 0.10700001f;

    Math_StepToF(&self->posOffsetX, 0.0f, 0.1f);
    Math_StepToF(&self->posOffsetZ, 0.0f, 0.1f);
    Math_ScaledStepToS(&self->dyna.actor.shape.rot.y, self->dyna.actor.home.rot.y, 0x64);
}

void ObjBean_Move(ObjBean* self) {
    self->dyna.actor.world.pos.x = self->pathPoints.x + self->posOffsetX;
    self->dyna.actor.world.pos.y = self->pathPoints.y;
    self->dyna.actor.world.pos.z = self->pathPoints.z + self->posOffsetZ;
}

void ObjBean_SetDrawMode(ObjBean* self, u8 drawFlag) {
    self->stateFlags &=
        ~(BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_PLANT | BEAN_STATE_DRAW_STALK | BEAN_STATE_DRAW_SOIL);
    self->stateFlags |= drawFlag;
}

void ObjBean_SetupPathCount(ObjBean* self, GlobalContext* globalCtx) {
    self->pathCount = globalCtx->setupPathList[(self->dyna.actor.params >> 8) & 0x1F].count - 1;
    self->currentPointIndex = 0;
    self->nextPointIndex = 1;
}

void ObjBean_SetupPath(ObjBean* self, GlobalContext* globalCtx) {
    Path* path = &globalCtx->setupPathList[(self->dyna.actor.params >> 8) & 0x1F];
    Math_Vec3s_ToVec3f(&self->pathPoints, SEGMENTED_TO_VIRTUAL(path->points));
}

void ObjBean_FollowPath(ObjBean* self, GlobalContext* globalCtx) {
    Path* path;
    Vec3f acell;
    Vec3f pathPointsFloat;
    f32 speed;
    Vec3s* nextPathPoint;
    Vec3s* currentPoint;
    Vec3s* sp4C;
    Vec3f sp40;
    Vec3f sp34;
    f32 sp30;
    f32 mag;

    Math_StepToF(&self->dyna.actor.speedXZ, sBeanSpeeds[self->unk_1F6].velocity, sBeanSpeeds[self->unk_1F6].accel);
    path = &globalCtx->setupPathList[(self->dyna.actor.params >> 8) & 0x1F];
    nextPathPoint = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[self->nextPointIndex];

    Math_Vec3s_ToVec3f(&pathPointsFloat, nextPathPoint);

    Math_Vec3f_Diff(&pathPointsFloat, &self->pathPoints, &acell);
    mag = Math3D_Vec3fMagnitude(&acell);
    speed = CLAMP_MIN(self->dyna.actor.speedXZ, 0.5f);
    if (speed > mag) {
        currentPoint = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[self->currentPointIndex];

        Math_Vec3f_Copy(&self->pathPoints, &pathPointsFloat);
        self->currentPointIndex = self->nextPointIndex;

        if (self->pathCount <= self->currentPointIndex) {
            self->nextPointIndex = 0;
        } else {
            self->nextPointIndex++;
        }
        sp4C = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[self->nextPointIndex];
        Math_Vec3s_DiffToVec3f(&sp40, nextPathPoint, currentPoint);
        Math_Vec3s_DiffToVec3f(&sp34, sp4C, nextPathPoint);
        if (Math3D_CosOut(&sp40, &sp34, &sp30)) {
            self->dyna.actor.speedXZ = 0.0f;
        } else {
            self->dyna.actor.speedXZ *= (sp30 + 1.0f) * 0.5f;
        }
    } else {
        Math_Vec3f_Scale(&acell, self->dyna.actor.speedXZ / mag);
        self->pathPoints.x += acell.x;
        self->pathPoints.y += acell.y;
        self->pathPoints.z += acell.z;
    }
}

s32 ObjBean_CheckForHorseTrample(ObjBean* self, GlobalContext* globalCtx) {
    Actor* currentActor = globalCtx->actorCtx.actorLists[ACTORCAT_BG].head;

    while (currentActor != NULL) {
        if ((currentActor->id == ACTOR_EN_HORSE) &&
            (Math3D_Vec3fDistSq(&currentActor->world.pos, &self->dyna.actor.world.pos) < 10000.0f)) {
            return true;
        }
        currentActor = currentActor->next;
    }

    return false;
}

void ObjBean_Break(ObjBean* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Vec3f velocity;
    f32 temp_f20;
    s16 angle;
    s32 scale;
    s32 i;
    s16 gravity;
    s16 arg5;

    angle = 0;
    for (i = 0; i < 36; i++) {
        angle += 0x4E20;
        temp_f20 = Rand_ZeroOne() * 60.0f;

        pos.x = (Math_SinS(angle) * temp_f20) + self->dyna.actor.world.pos.x;
        pos.y = self->dyna.actor.world.pos.y;
        pos.z = (Math_CosS(angle) * temp_f20) + self->dyna.actor.world.pos.z;

        velocity.x = Math_SinS(angle) * 3.5f;
        velocity.y = Rand_ZeroOne() * 13.0f;
        velocity.z = Math_CosS(angle) * 3.5f;

        velocity.x += self->dyna.actor.world.pos.x - self->dyna.actor.prevPos.x;
        velocity.y += self->dyna.actor.world.pos.y - self->dyna.actor.prevPos.y;
        velocity.z += self->dyna.actor.world.pos.z - self->dyna.actor.prevPos.z;

        scale = (s32)(Rand_ZeroOne() * 180.0f) + 30;
        if (scale < 90) {
            if (Rand_ZeroOne() < 0.1f) {
                gravity = -80;
                arg5 = 96;
            } else {
                gravity = -80;
                arg5 = 64;
            }
        } else {
            gravity = -100;
            arg5 = 64;
        }
        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &pos, gravity, arg5, 40, 3, 0, scale, 0, 0,
                             (s16)((scale >> 3) + 40), -1, 1, sBreakDlists[i & 1]);
    }
}

void ObjBean_UpdateLeaves(ObjBean* self) {
    Math_StepToS(&self->unk_1C2, self->unk_1C4, self->unk_1C6);
    Math_StepToS(&self->unk_1C8, self->unk_1CA, self->unk_1CC);
    self->unk_1CE += self->unk_1C8;
    self->leafRotFactor = 6372.0f - Math_SinS(self->unk_1CE) * (f32)self->unk_1C2;
    self->dyna.actor.scale.y = Math_SinS(self->leafRotFactor) * 0.17434467f;
    self->dyna.actor.scale.x = self->dyna.actor.scale.z = Math_CosS(self->leafRotFactor) * 0.12207746f;
}

void ObjBean_SetupLeavesStill(ObjBean* self) {
    self->transformFunc = ObjBean_LeavesStill;
    self->unk_1C0 = Rand_S16Offset(12, 40);
    self->unk_1C4 = Rand_S16Offset(0xC8, 0x190);
    self->unk_1C6 = 0x14;
    self->unk_1CA = Rand_S16Offset(0x64, 0x320);
    self->unk_1CC = 0x14;
}

void ObjBean_LeavesStill(ObjBean* self) {
    self->unk_1C0--;
    ObjBean_UpdateLeaves(self);
    if (self->unk_1C0 < 0) {
        ObjBean_SetupShakeLeaves(self);
    }
}

void ObjBean_SetupShakeLeaves(ObjBean* self) {
    self->transformFunc = ObjBean_ShakeLeaves;
    self->unk_1C0 = Rand_S16Offset(30, 4);
    self->unk_1C4 = Rand_S16Offset(0x7D0, 0x3E8);
    self->unk_1C6 = 0xC8;
    self->unk_1CA = Rand_S16Offset(0x36B0, 0x1770);
    self->unk_1CC = 0xFA0;
    self->leafRotFactor = 0x18E4;
}

void ObjBean_ShakeLeaves(ObjBean* self) {
    self->unk_1C0 += -1;
    if (self->unk_1C0 == 14) {
        self->unk_1C4 = Rand_S16Offset(0xC8, 0x190);
        self->unk_1CA = Rand_S16Offset(0x64, 0x1F4);
        self->unk_1CC = 0x7D0;
    }
    ObjBean_UpdateLeaves(self);
    if (self->unk_1C0 < 0) {
        ObjBean_SetupLeavesStill(self);
    }
}

void ObjBean_SetupShakeLeavesFast(ObjBean* self) {
    self->transformFunc = ObjBean_ShakeLeavesFast;
    self->unk_1C0 = 0x28;
    self->unk_1C4 = 0xBB8;
    self->unk_1C6 = 0x12C;
    self->unk_1CA = 0x3A98;
    self->unk_1CC = 0xFA0;
    self->leafRotFactor = 0x18E4;
}

void ObjBean_ShakeLeavesFast(ObjBean* self) {
    self->unk_1C0 += -1;
    if (Rand_ZeroOne() < 0.1f) {
        self->unk_1C4 = Rand_S16Offset(0x898, 0x3E8);
        self->unk_1CA = Rand_S16Offset(0x2EE0, 0x1F40);
    }
    ObjBean_UpdateLeaves(self);
    if ((s32)self->unk_1C0 < 0) {
        ObjBean_SetupGrow(self);
    }
}

void ObjBean_SetupGrow(ObjBean* self) {
    self->transformFunc = ObjBean_Grow;
}

void ObjBean_Grow(ObjBean* self) {
    Math_StepToS(&self->leafRotFactor, 0x33E9, 0x168);
    self->dyna.actor.scale.y = Math_SinS(self->leafRotFactor) * 0.17434467f;
    self->dyna.actor.scale.x = self->dyna.actor.scale.z = Math_CosS(self->leafRotFactor) * 0.12207746f;
    ;
}

void ObjBean_SetupFlattenLeaves(ObjBean* self) {
    self->transformFunc = ObjBean_FlattenLeaves;
    self->leafRotFactor = 0x33E9;
}

void ObjBean_FlattenLeaves(ObjBean* self) {
    self->leafRotFactor -= 0x960;
    self->dyna.actor.scale.y = Math_SinS(self->leafRotFactor) * 0.17434467f;
    self->dyna.actor.scale.x = self->dyna.actor.scale.z = Math_CosS(self->leafRotFactor) * 0.12207746f;

    if (self->leafRotFactor < 0x18E4) {
        ObjBean_SetupGrown(self);
    }
}

void ObjBean_SetupGrown(ObjBean* self) {
    self->transformFunc = ObjBean_Grown;
    self->unk_1C2 = 0xBB8;
    self->unk_1C4 = 0;
    self->unk_1C6 = 0xC8;
    self->unk_1C8 = 0x3E80;
    self->unk_1CA = 0x1F4;
    self->unk_1CC = 0;
    self->unk_1C0 = 0x10;
}

void ObjBean_Grown(ObjBean* self) {
    self->unk_1C0--;
    if (self->unk_1C0 == 6) {
        self->unk_1CC = 0x7D0;
    }
    ObjBean_UpdateLeaves(self);
    if (self->unk_1C2 <= 0) {
        ObjBean_SetupLeavesStill(self);
    }
}

void ObjBean_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 path;
    s32 linkAge;
    ObjBean* self = THIS;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
        if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F) || (mREG(1) == 1)) {
            path = (self->dyna.actor.params >> 8) & 0x1F;
            if (path == 0x1F) {
                osSyncPrintf(VT_COL(RED, WHITE));
                // "No path data?"
                osSyncPrintf("パスデータが無い？(%s %d)(arg_data %xH)\n", "../z_obj_bean.c", 909,
                             self->dyna.actor.params);
                osSyncPrintf(VT_RST);
                Actor_Kill(&self->dyna.actor);
                return;
            }
            if (globalCtx->setupPathList[path].count < 3) {
                osSyncPrintf(VT_COL(RED, WHITE));
                // "Incorrect number of path data"
                osSyncPrintf("パスデータ数が不正(%s %d)(arg_data %xH)\n", "../z_obj_bean.c", 921,
                             self->dyna.actor.params);
                osSyncPrintf(VT_RST);
                Actor_Kill(&self->dyna.actor);
                return;
            }
            ObjBean_SetupPathCount(self, globalCtx);
            ObjBean_SetupPath(self, globalCtx);
            ObjBean_Move(self);
            ObjBean_SetupWaitForPlayer(self);

            ObjBean_InitDynaPoly(self, globalCtx, &gMagicBeanPlatformCol, DPM_UNK3);
            self->stateFlags |= BEAN_STATE_DYNAPOLY_SET;
            ObjBean_InitCollider(&self->dyna.actor, globalCtx);
            self->stateFlags |= BEAN_STATE_COLLIDER_SET;

            ActorShape_Init(&self->dyna.actor.shape, 0.0f, ActorShadow_DrawCircle, 8.8f);
            ObjBean_FindFloor(self, globalCtx);
            self->unk_1F6 = self->dyna.actor.home.rot.z & 3;
        } else {
            Actor_Kill(&self->dyna.actor);
            return;
        }
    } else if ((Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F) != 0) || (mREG(1) == 1)) {
        ObjBean_SetupWaitForWater(self);
    } else {
        ObjBean_SetupWaitForBean(self);
    }
    self->dyna.actor.world.rot.z = self->dyna.actor.home.rot.z = self->dyna.actor.shape.rot.z = 0;
    // "Magic bean tree lift"
    osSyncPrintf("(魔法の豆の木リフト)(arg_data 0x%04x)\n", self->dyna.actor.params);
}

void ObjBean_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ObjBean* self = THIS;

    if (self->stateFlags & BEAN_STATE_DYNAPOLY_SET) {
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
    if (self->stateFlags & BEAN_STATE_COLLIDER_SET) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
    if (D_80B90E30 == self) {
        D_80B90E30 = NULL;
    }
}

void ObjBean_SetupWaitForBean(ObjBean* self) {
    self->actionFunc = ObjBean_WaitForBean;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_LEAVES);
    self->dyna.actor.textId = 0x2F;
}

void ObjBean_WaitForBean(ObjBean* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->dyna.actor, globalCtx)) {
        if (func_8002F368(globalCtx) == EXCH_ITEM_BEAN) {
            func_80B8FE00(self);
            Flags_SetSwitch(globalCtx, self->dyna.actor.params & 0x3F);
        }
    } else {
        func_8002F298(&self->dyna.actor, globalCtx, 40.0f, EXCH_ITEM_BEAN);
    }
}

void func_80B8FE00(ObjBean* self) {
    self->actionFunc = func_80B8FE3C;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_LEAVES);
    self->timer = 60;
}

// Link is looking at the soft soil
void func_80B8FE3C(ObjBean* self, GlobalContext* globalCtx) {
    if (self->timer <= 0) {
        func_80B8FE6C(self);
    }
}

void func_80B8FE6C(ObjBean* self) {
    self->actionFunc = func_80B8FEAC;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_SOIL);
    Actor_SetScale(&self->dyna.actor, 0.01f);
}

// The leaves are visable and growing
void func_80B8FEAC(ObjBean* self, GlobalContext* globalCtx) {
    s32 temp_v1 = true;

    temp_v1 &= Math_StepToF(&self->dyna.actor.scale.y, 0.16672663f, 0.01f);
    temp_v1 &= Math_StepToF(&self->dyna.actor.scale.x, 0.03569199f, 0.00113f);

    self->dyna.actor.scale.z = self->dyna.actor.scale.x;
    if (temp_v1) {
        if (self->timer <= 0) {
            func_80B8FF50(self);
        }
    } else {
        self->timer = 1;
    }
    func_8002F974(&self->dyna.actor, NA_SE_PL_PLANT_GROW_UP - SFX_FLAG);
}

void func_80B8FF50(ObjBean* self) {
    self->actionFunc = func_80B8FF8C;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_SOIL);
    self->unk_1B6.x = 0x33E9;
}

void func_80B8FF8C(ObjBean* self, GlobalContext* globalCtx) {
    self->unk_1B6.x -= 0x960;
    self->dyna.actor.scale.y = Math_SinS(self->unk_1B6.x) * 0.17434467f;
    self->dyna.actor.scale.x = self->dyna.actor.scale.z = Math_CosS(self->unk_1B6.x) * 0.12207746f;
    if (self->unk_1B6.x < 0x18E4) {
        func_80B90010(self);
    }
}

void func_80B90010(ObjBean* self) {
    self->actionFunc = func_80B90050;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_SOIL);
    self->unk_1B6.x = 0;
    self->unk_1B6.y = 0xBB8;
}

// Control is returned to the player and the leaves start to flatten out
void func_80B90050(ObjBean* self, GlobalContext* globalCtx) {
    s16 temp_a0;
    f32 temp_f2;

    self->unk_1B6.x += 0x3E80;
    self->unk_1B6.y += -0xC8;
    temp_a0 = 6372.0f - Math_SinS(self->unk_1B6.x) * self->unk_1B6.y;

    self->dyna.actor.scale.y = Math_SinS(temp_a0) * 0.17434467f;
    self->dyna.actor.scale.x = self->dyna.actor.scale.z = Math_CosS(temp_a0) * 0.12207746f;
    if (self->unk_1B6.y < 0) {
        ObjBean_SetupWaitForWater(self);
    }
}

void ObjBean_SetupWaitForWater(ObjBean* self) {
    self->actionFunc = ObjBean_WaitForWater;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_SOIL);
    Actor_SetScale(&self->dyna.actor, 0.1f);
    ObjBean_SetupLeavesStill(self);
}

void ObjBean_WaitForWater(ObjBean* self, GlobalContext* globalCtx) {
    self->transformFunc(self);

    if (!(self->stateFlags & BEAN_STATE_BEEN_WATERED) && Flags_GetEnv(globalCtx, 5) && (D_80B90E30 == NULL) &&
        (self->dyna.actor.xzDistToPlayer < 50.0f)) {
        ObjBean_SetupGrowWaterPhase1(self);
        D_80B90E30 = self;
        OnePointCutscene_Init(globalCtx, 2210, -99, &self->dyna.actor, MAIN_CAM);
        self->dyna.actor.flags |= 0x10;
        return;
    }

    if ((D_80B90E30 == self) && !Flags_GetEnv(globalCtx, 5)) {
        D_80B90E30 = NULL;
        if (D_80B90E30) {}
    }
}

void ObjBean_SetupGrowWaterPhase1(ObjBean* self) {
    self->actionFunc = ObjBean_GrowWaterPhase1;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_SOIL);
    ObjBean_SetupShakeLeavesFast(self);
    self->timer = 50;
}

// Camera moves and leaves move quickly
void ObjBean_GrowWaterPhase1(ObjBean* self, GlobalContext* globalCtx) {
    self->transformFunc(self);
    if (self->timer <= 0) {
        ObjBean_SetupGrowWaterPhase2(self);
    }
}

void ObjBean_SetupGrowWaterPhase2(ObjBean* self) {
    self->actionFunc = ObjBean_GrowWaterPhase2;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_SOIL | BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_STALK);
    self->stalkSizeMultiplier = 0.0f;
}

// BeanStalk is visable and is growing
void ObjBean_GrowWaterPhase2(ObjBean* self, GlobalContext* globalCtx) {
    self->transformFunc(self);
    self->stalkSizeMultiplier += 0.001f;
    self->dyna.actor.shape.rot.y = self->dyna.actor.home.rot.y + (s16)(self->stalkSizeMultiplier * 700000.0f);
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + self->stalkSizeMultiplier * 800.0f;
    if (self->stalkSizeMultiplier >= 0.1f) { // 100 Frames
        ObjBean_SetupGrowWaterPhase3(self);
    }
    func_8002F974(&self->dyna.actor, NA_SE_PL_PLANT_TALLER - SFX_FLAG);
}

void ObjBean_SetupGrowWaterPhase3(ObjBean* self) {
    self->actionFunc = ObjBean_GrowWaterPhase3;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_SOIL | BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_STALK);
    self->timer = 60;
}

// Fully grown and drops items
void ObjBean_GrowWaterPhase3(ObjBean* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3f itemDropPos;

    self->transformFunc(self);
    if (self->timer == 40) {
        ObjBean_SetupFlattenLeaves(self);
    } else if (self->timer == 30) {
        if (!(self->stateFlags & BEAN_STATE_BEEN_WATERED)) {
            itemDropPos.x = self->dyna.actor.world.pos.x;
            itemDropPos.y = self->dyna.actor.world.pos.y - 25.0f;
            itemDropPos.z = self->dyna.actor.world.pos.z;
            for (i = 0; i < 3; i++) {
                Item_DropCollectible(globalCtx, &itemDropPos, ITEM00_FLEXIBLE);
            }
            self->stateFlags |= BEAN_STATE_BEEN_WATERED;
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
        }
    } else if (self->timer <= 0) {
        ObjBean_SetupGrowWaterPhase4(self);
    }
}

void ObjBean_SetupGrowWaterPhase4(ObjBean* self) {
    self->actionFunc = ObjBean_GrowWaterPhase4;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_SOIL | BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_STALK);
    ObjBean_SetupGrow(self);
}

// Return control back to the player and start to shrink back down
void ObjBean_GrowWaterPhase4(ObjBean* self, GlobalContext* globalCtx) {
    self->transformFunc(self);
    self->stalkSizeMultiplier -= 0.001f;
    self->dyna.actor.shape.rot.y = self->dyna.actor.home.rot.y + (s16)(self->stalkSizeMultiplier * 700000.0f);
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + (self->stalkSizeMultiplier * 800.0f);
    if (self->stalkSizeMultiplier <= 0.0f) {
        self->stalkSizeMultiplier = 0.0f;
        self->dyna.actor.shape.rot.y = self->dyna.actor.home.rot.y;
        ObjBean_SetupGrowWaterPhase5(self);
    }
}

void ObjBean_SetupGrowWaterPhase5(ObjBean* self) {
    self->actionFunc = ObjBean_GrowWaterPhase5;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_LEAVES | BEAN_STATE_DRAW_SOIL);
    self->timer = 30;
}

void ObjBean_GrowWaterPhase5(ObjBean* self, GlobalContext* globalCtx) {
    self->transformFunc(self);
    if (self->timer <= 0) {
        func_80B8FF50(self);
        self->dyna.actor.flags &= ~0x10;
    }
}

void ObjBean_SetupWaitForPlayer(ObjBean* self) {
    self->actionFunc = ObjBean_WaitForPlayer;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_PLANT);
}

void ObjBean_WaitForPlayer(ObjBean* self, GlobalContext* globalCtx) {
    if (func_8004356C(&self->dyna)) { // Player is standing on
        ObjBean_SetupFly(self);
        if (globalCtx->sceneNum == SCENE_SPOT10) { // Lost woods
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_LIFTBEAN);
        } else {
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_UFOBEAN);
        }
    }
    ObjBean_UpdatePosition(self);
}

void ObjBean_SetupFly(ObjBean* self) {
    self->actionFunc = ObjBean_Fly;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_PLANT);
    self->dyna.actor.speedXZ = 0.0f;
    self->dyna.actor.flags |= 0x10; // Never stop updating
}

void ObjBean_Fly(ObjBean* self, GlobalContext* globalCtx) {
    Camera* camera;

    ObjBean_FollowPath(self, globalCtx);
    if (self->currentPointIndex == self->pathCount) {
        ObjBean_SetupPathCount(self, globalCtx);
        ObjBean_SetupPath(self, globalCtx);
        ObjBean_SetupWaitForStepOff(self);

        self->dyna.actor.flags &= ~0x10; // Never stop updating (disable)
        camera = globalCtx->cameraPtrs[MAIN_CAM];

        if ((camera->setting == CAM_SET_LIFTBEAN) || (camera->setting == CAM_SET_UFOBEAN)) {
            Camera_ChangeSetting(camera, CAM_SET_NORMAL0);
        }

    } else if (func_8004356C(&self->dyna) != 0) { // Player is on top

        func_8002F974(&self->dyna.actor, NA_SE_PL_PLANT_MOVE - SFX_FLAG);

        if (globalCtx->sceneNum == SCENE_SPOT10) {
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_LIFTBEAN);
        } else {
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_UFOBEAN);
        }
    } else if (self->stateFlags & BEAN_STATE_PLAYER_ON_TOP) {
        camera = globalCtx->cameraPtrs[MAIN_CAM];

        if ((camera->setting == CAM_SET_LIFTBEAN) || (camera->setting == CAM_SET_UFOBEAN)) {
            Camera_ChangeSetting(camera, CAM_SET_NORMAL0);
        }
    }

    ObjBean_UpdatePosition(self);
}

void ObjBean_SetupWaitForStepOff(ObjBean* self) {
    self->actionFunc = ObjBean_WaitForStepOff;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_PLANT);
}

void ObjBean_WaitForStepOff(ObjBean* self, GlobalContext* globalCtx) {
    if (!func_80043590(&self->dyna)) {
        ObjBean_SetupWaitForPlayer(self);
    }
    ObjBean_UpdatePosition(self);
}

void func_80B908EC(ObjBean* self) {
    self->actionFunc = func_80B90918;
    ObjBean_SetDrawMode(self, 0);
}

void func_80B90918(ObjBean* self, GlobalContext* globalCtx) {
    if (!func_8004356C(&self->dyna)) {
        ObjBean_SetupPathCount(self, globalCtx);
        ObjBean_SetupPath(self, globalCtx);
        ObjBean_Move(self);
        func_80B90970(self);
    }
}

void func_80B90970(ObjBean* self) {
    self->actionFunc = func_80B909B0;
    ObjBean_SetDrawMode(self, 0);
    self->timer = 100;
    func_80B8EDF4(self);
}

void func_80B909B0(ObjBean* self, GlobalContext* globalCtx) {
    if (ObjBean_CheckForHorseTrample(self, globalCtx)) {
        self->timer = 100;
    } else if (self->timer <= 0) {
        func_80B909F8(self);
    }
}

void func_80B909F8(ObjBean* self) {
    self->actionFunc = func_80B90A34;
    ObjBean_SetDrawMode(self, BEAN_STATE_DRAW_PLANT);
    self->timer = 30;
}

void func_80B90A34(ObjBean* self, GlobalContext* globalCtx) {
    s32 trampled = ObjBean_CheckForHorseTrample(self, globalCtx);

    func_80B8EE24(self);
    if (trampled) {
        func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    } else {
        func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
    if ((self->timer <= 0) && (!trampled)) {
        func_80B8EBC8(self);
        ObjBean_SetupWaitForPlayer(self);
    }
}
void ObjBean_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    ObjBean* self = THIS;

    if (self->timer > 0) {
        self->timer--;
    }

    self->actionFunc(self, globalCtx);

    if (self->stateFlags & BEAN_STATE_DRAW_PLANT) {
        ObjBean_Move(self);
        if (self->dyna.actor.xzDistToPlayer < 150.0f) {
            self->collider.dim.radius = self->dyna.actor.scale.x * 640.0f + 0.5f;
            Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }

        ObjBean_FindFloor(self, globalCtx);

        self->dyna.actor.shape.shadowDraw = ActorShadow_DrawCircle;
        self->dyna.actor.shape.shadowScale = self->dyna.actor.scale.x * 88.0f;

        if (ObjBean_CheckForHorseTrample(self, globalCtx)) {
            osSyncPrintf(VT_FGCOL(CYAN));
            // "Horse and bean tree lift collision"
            osSyncPrintf("馬と豆の木リフト衝突！！！\n");
            osSyncPrintf(VT_RST);
            ObjBean_Break(self, globalCtx);
            func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            func_80B908EC(self);
        }
    } else {
        self->dyna.actor.shape.shadowDraw = NULL;
    }
    Actor_SetFocus(&self->dyna.actor, 6.0f);
    if (self->stateFlags & BEAN_STATE_DYNAPOLY_SET) {
        if (func_8004356C(&self->dyna)) {
            self->stateFlags |= BEAN_STATE_PLAYER_ON_TOP;
        } else {
            self->stateFlags &= ~BEAN_STATE_PLAYER_ON_TOP;
        }
    }
}

void ObjBean_DrawSoftSoilSpot(ObjBean* self, GlobalContext* globalCtx) {
    Matrix_Translate(self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y, self->dyna.actor.home.pos.z,
                     MTXMODE_NEW);
    Matrix_RotateY(self->dyna.actor.home.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);
    Gfx_DrawDListOpa(globalCtx, gMagicBeanSoftSoilDL);
}

void ObjBean_DrawBeanstalk(ObjBean* self, GlobalContext* globalCtx) {
    Matrix_Translate(self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z,
                     MTXMODE_NEW);
    Matrix_RotateY(self->dyna.actor.shape.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_Scale(0.1f, self->stalkSizeMultiplier, 0.1f, MTXMODE_APPLY);
    Gfx_DrawDListOpa(globalCtx, gMagicBeanStemDL);
}

void ObjBean_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ObjBean* self = THIS;

    if (self->stateFlags & BEAN_STATE_DRAW_SOIL) {
        Gfx_DrawDListOpa(globalCtx, gMagicBeanSeedlingDL);
    }
    if (self->stateFlags & BEAN_STATE_DRAW_PLANT) {
        Gfx_DrawDListOpa(globalCtx, gMagicBeanPlatformDL);
    }
    if (self->stateFlags & BEAN_STATE_DRAW_LEAVES) {
        ObjBean_DrawSoftSoilSpot(self, globalCtx);
    }
    if (self->stateFlags & BEAN_STATE_DRAW_STALK) {
        ObjBean_DrawBeanstalk(self, globalCtx);
    }
}
