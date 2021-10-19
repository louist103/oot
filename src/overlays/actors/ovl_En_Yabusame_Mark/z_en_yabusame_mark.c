/*
 * File: z_en_yabusame_mark.c
 * Overlay: ovl_En_Yabusame_Mark
 * Description: Horseback Archery Target (arrow hitbox)
 */

#include "z_en_yabusame_mark.h"
#include "vt.h"

#define FLAGS 0x00000000

#define THIS ((EnYabusameMark*)thisx)

void EnYabusameMark_Init(Actor* thisx, GlobalContext* globalCtx);
void EnYabusameMark_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnYabusameMark_Update(Actor* thisx, GlobalContext* globalCtx);
void func_80B42F74(EnYabusameMark* self, GlobalContext* globalCtx);

static ColliderQuadInit sQuadInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x0001F824, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

const ActorInit En_Yabusame_Mark_InitVars = {
    ACTOR_EN_YABUSAME_MARK,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnYabusameMark),
    (ActorFunc)EnYabusameMark_Init,
    (ActorFunc)EnYabusameMark_Destroy,
    (ActorFunc)EnYabusameMark_Update,
    NULL,
};

static Vec3f sCollisionVertices[] = {
    { 70.0f, 70.0f, 0.0f },      { 70.0f, -70.0f, 0.0f },      { -70.0f, 70.0f, 0.0f },    { -70.0f, -70.0f, 0.0f },
    { 90.0f, 130.0f, -120.0f },  { -25.0f, -80.0f, -130.0f },  { 90.0f, 130.0f, 120.0f },  { -25.0f, -80.0, 130.0f },
    { 115.0f, 160.0f, -150.0f }, { -50.0f, -140.0f, -160.0f }, { 115.0f, 160.0f, 150.0f }, { -50.0f, -140.0f, 160.0f },
};

static Vec3f sTargetPos[] = {
    { 3382.0f, 1734.0f, -4946.0f }, // small, furthest from entrance
    { 3360.0f, 1734.0f, 495.0f },   // small, closest to entrance
    { 4517.0f, 1682.0f, -1779.0f }, // medium, on the right
    { 4517.0f, 1682.0f, -2813.0f }, // medium, on the left
    { 4522.0f, 1727.0f, -2296.0f }, // large in the center
};

// 0: first ring
// 1: second ring
// 2: outside edge
static f32 sRingDistance[] = {
    20.0f, 40.0f,  60.0f,  777.0f, // small
    40.0f, 80.0f,  120.0f, 777.0f, // medium
    40.0f, 120.0f, 160.0f, 777.0f, // large
};

void EnYabusameMark_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnYabusameMark* self = THIS;

    Collider_DestroyQuad(globalCtx, &self->collider);
}

void EnYabusameMark_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnYabusameMark* self = THIS;

    osSyncPrintf("\n\n");
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ やぶさめまと ☆☆☆☆☆ %x\n" VT_RST, self->actor.params);
    self->actor.flags &= ~1;
    self->typeIndex = self->actor.params;
    self->actor.targetMode = 5;
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 種類インデックス \t   ☆☆☆☆☆ %d\n" VT_RST, self->typeIndex);
    switch (self->typeIndex) {
        case 0:
            self->subTypeIndex = 0;
            if (self->actor.world.pos.z > 0.0f) {
                self->subTypeIndex = 1;
            }
            break;
        case 1:
            self->subTypeIndex = 2;
            if (self->actor.world.pos.z < -2000.0f) {
                self->subTypeIndex = 3;
            }
            break;
        case 2:
            self->subTypeIndex = 4;
            break;
    }
    Collider_InitQuad(globalCtx, &self->collider);
    Collider_SetQuad(globalCtx, &self->collider, &self->actor, &sQuadInit);
    self->worldPos = self->actor.world.pos;
    self->actor.flags |= 0x10;
    if (gSaveContext.sceneSetupIndex != 4) {
        Actor_Kill(&self->actor);
        return;
    }
    osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 種類       ☆☆☆☆☆ %d\n" VT_RST, self->typeIndex);
    osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ さらに分類 ☆☆☆☆☆ %d\n" VT_RST, self->subTypeIndex);
    self->actionFunc = func_80B42F74;
}

void func_80B42F74(EnYabusameMark* self, GlobalContext* globalCtx) {
    Vec3f effectAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effectVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f arrowHitPos;
    Vec3f distanceFromCenter;
    s32 pad;
    s32 scoreIndex;
    f32 scoreDistance100;
    f32 scoreDistance60;
    f32 scoreDistance30;

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;

        arrowHitPos.x = self->collider.info.bumper.hitPos.x;
        arrowHitPos.y = self->collider.info.bumper.hitPos.y;
        arrowHitPos.z = self->collider.info.bumper.hitPos.z;

        effectVelocity.y = 15.0f;

        EffectSsHitMark_SpawnCustomScale(globalCtx, 0, 700, &arrowHitPos);

        scoreIndex = 2;

        scoreDistance100 = sRingDistance[self->typeIndex * 4 + 0];
        scoreDistance60 = sRingDistance[self->typeIndex * 4 + 1];
        scoreDistance30 = sRingDistance[self->typeIndex * 4 + 2];

        distanceFromCenter.x = fabsf(sTargetPos[self->subTypeIndex].x - arrowHitPos.x);
        distanceFromCenter.y = fabsf(sTargetPos[self->subTypeIndex].y - arrowHitPos.y);
        distanceFromCenter.z = fabsf(sTargetPos[self->subTypeIndex].z - arrowHitPos.z);

        if (distanceFromCenter.x > scoreDistance100 || distanceFromCenter.y > scoreDistance100 ||
            distanceFromCenter.z > scoreDistance100) {
            scoreIndex = 1;
            if (distanceFromCenter.x > scoreDistance60 || distanceFromCenter.y > scoreDistance60 ||
                distanceFromCenter.z > scoreDistance60) {
                scoreIndex = 0;
            }
            if (distanceFromCenter.x > scoreDistance30 || distanceFromCenter.y > scoreDistance30 ||
                distanceFromCenter.z > scoreDistance30) {
                return;
            }
        }

        osSyncPrintf("\n\n");
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ posＸ ☆☆☆☆☆ %f\n" VT_RST, arrowHitPos.x);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ posＹ ☆☆☆☆☆ %f\n" VT_RST, arrowHitPos.y);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ posＺ ☆☆☆☆☆ %f\n" VT_RST, arrowHitPos.z);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ hitＸ ☆☆☆☆☆ %f\n" VT_RST, sTargetPos[self->subTypeIndex].x);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ hitＹ ☆☆☆☆☆ %f\n" VT_RST, sTargetPos[self->subTypeIndex].y);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ hitＺ ☆☆☆☆☆ %f\n" VT_RST, sTargetPos[self->subTypeIndex].z);
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 小    ☆☆☆☆☆ %f\n" VT_RST, scoreDistance100);
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 大    ☆☆☆☆☆ %f\n" VT_RST, scoreDistance60);
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ point ☆☆☆☆☆ %d\n" VT_RST, scoreIndex);
        osSyncPrintf("\n\n");

        if (scoreIndex == 2) {
            func_800F5C64(0x922);
        }
        if (scoreIndex == 1) {
            Audio_StopSfx(NA_SE_SY_TRE_BOX_APPEAR);
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
        }
        if (scoreIndex == 0) {
            func_80078884(NA_SE_SY_DECIDE);
        }
        EffectSsExtra_Spawn(globalCtx, &arrowHitPos, &effectVelocity, &effectAccel, 5, scoreIndex);
    }
}

void EnYabusameMark_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnYabusameMark* self = THIS;
    Vec3f* vertexArray;
    u32 arrayIndex;

    self->actionFunc(self, globalCtx);
    arrayIndex = self->typeIndex * 4;
    vertexArray = &sCollisionVertices[arrayIndex];

    self->vertexA.x = vertexArray[0].x + self->actor.world.pos.x;
    self->vertexA.y = vertexArray[0].y + self->actor.world.pos.y;
    self->vertexA.z = vertexArray[0].z + self->actor.world.pos.z;

    self->vertexB.x = vertexArray[1].x + self->actor.world.pos.x;
    self->vertexB.y = vertexArray[1].y + self->actor.world.pos.y;
    self->vertexB.z = vertexArray[1].z + self->actor.world.pos.z;

    self->vertexC.x = vertexArray[2].x + self->actor.world.pos.x;
    self->vertexC.y = vertexArray[2].y + self->actor.world.pos.y;
    self->vertexC.z = vertexArray[2].z + self->actor.world.pos.z;

    self->vertexD.x = vertexArray[3].x + self->actor.world.pos.x;
    self->vertexD.y = vertexArray[3].y + self->actor.world.pos.y;
    self->vertexD.z = vertexArray[3].z + self->actor.world.pos.z;

    Collider_SetQuadVertices(&self->collider, &self->vertexA, &self->vertexB, &self->vertexC, &self->vertexD);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    if (BREG(0)) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, 0, 0xFF, 0, 0xFF, 4, globalCtx->state.gfxCtx);
    }
}
