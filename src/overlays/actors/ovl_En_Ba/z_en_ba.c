/*
 * File: z_en_ba.c
 * Overlay: ovl_En_Ba
 * Description: Tentacle from inside Lord Jabu-Jabu
 */

#include "z_en_ba.h"
#include "objects/object_bxa/object_bxa.h"

#define FLAGS 0x00000015

#define THIS ((EnBa*)thisx)

void EnBa_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBa_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBa_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBa_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnBa_SetupIdle(EnBa* self);
void EnBa_SetupFallAsBlob(EnBa* self);
void EnBa_Idle(EnBa* self, GlobalContext* globalCtx);
void EnBa_FallAsBlob(EnBa* self, GlobalContext* globalCtx);
void EnBa_SwingAtPlayer(EnBa* self, GlobalContext* globalCtx);
void EnBa_RecoilFromDamage(EnBa* self, GlobalContext* globalCtx);
void EnBa_Die(EnBa* self, GlobalContext* globalCtx);
void EnBa_SetupSwingAtPlayer(EnBa* self);

const ActorInit En_Ba_InitVars = {
    ACTOR_EN_BA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BXA,
    sizeof(EnBa),
    (ActorFunc)EnBa_Init,
    (ActorFunc)EnBa_Destroy,
    (ActorFunc)EnBa_Update,
    (ActorFunc)EnBa_Draw,
};

static Vec3f D_809B8080 = { 0.0f, 0.0f, 32.0f };

static ColliderJntSphElementInit sJntSphElementInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000010, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 8, { { 0, 0, 0 }, 20 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x20000000, 0x00, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 13, { { 0, 0, 0 }, 25 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT0,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_JNTSPH,
    },
    2,
    sJntSphElementInit,
};

void EnBa_SetupAction(EnBa* self, EnBaActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

static Vec3f D_809B80E4 = { 0.01f, 0.01f, 0.01f };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x15, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2500, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 0, ICHAIN_STOP),
};

void EnBa_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBa* self = THIS;
    Vec3f sp38 = D_809B80E4;
    s32 pad;
    s16 i;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.world.pos.y = self->actor.home.pos.y + 100.0f;
    for (i = 13; i >= 0; i--) {
        self->unk200[i] = sp38;
        self->unk2A8[i].x = -0x4000;
        self->unk158[i] = self->actor.world.pos;
        self->unk158[i].y = self->actor.world.pos.y - (i + 1) * 32.0f;
    }

    self->actor.targetMode = 4;
    self->upperParams = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;

    if (self->actor.params < EN_BA_DEAD_BLOB) {
        if (Flags_GetSwitch(globalCtx, self->upperParams)) {
            Actor_Kill(&self->actor);
            return;
        }
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 48.0f);
        Actor_SetScale(&self->actor, 0.01f);
        EnBa_SetupIdle(self);
        self->actor.colChkInfo.health = 4;
        self->actor.colChkInfo.mass = MASS_HEAVY;
        Collider_InitJntSph(globalCtx, &self->collider);
        Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItems);
    } else {
        Actor_SetScale(&self->actor, 0.021f);
        EnBa_SetupFallAsBlob(self);
    }
}

void EnBa_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBa* self = THIS;
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnBa_SetupIdle(EnBa* self) {
    self->unk14C = 2;
    self->unk31C = 1500;
    self->actor.speedXZ = 10.0f;
    EnBa_SetupAction(self, EnBa_Idle);
}

void EnBa_Idle(EnBa* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 i;
    s32 pad;
    Vec3s sp5C;

    if ((self->actor.colChkInfo.mass == MASS_IMMOVABLE) && (self->actor.xzDistToPlayer > 175.0f)) {
        Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 330.0f, 1.0f, 7.0f, 0.0f);
    } else {
        self->actor.flags |= 1;
        Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 100.0f, 1.0f, 10.0f, 0.0f);
    }
    self->unk2FC = self->actor.world.pos;
    if (globalCtx->gameplayFrames % 16 == 0) {
        self->unk308.z += Rand_CenteredFloat(180.0f);
        self->unk314 += Rand_CenteredFloat(180.0f);
        self->unk308.x = Math_SinF(self->unk308.z) * 80.0f;
        self->unk308.y = Math_CosF(self->unk314) * 80.0f;
    }
    self->unk2FC.y -= 448.0f;
    self->unk2FC.x += self->unk308.x;
    self->unk2FC.z += self->unk308.y;
    func_80033AEC(&self->unk2FC, &self->unk158[13], 1.0f, self->actor.speedXZ, 0.0f, 0.0f);
    for (i = 12; i >= 0; i--) {
        func_80035844(&self->unk158[i + 1], &self->unk158[i], &sp5C, 0);
        Matrix_Translate(self->unk158[i + 1].x, self->unk158[i + 1].y, self->unk158[i + 1].z, MTXMODE_NEW);
        Matrix_RotateRPY(sp5C.x, sp5C.y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&D_809B8080, &self->unk158[i]);
    }
    func_80035844(&self->unk158[0], &self->unk2FC, &sp5C, 0);
    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->unk2A8[0].y, 3, self->unk31C, 182);
    Math_SmoothStepToS(&self->actor.shape.rot.x, self->unk2A8[0].x, 3, self->unk31C, 182);
    Matrix_RotateRPY(self->actor.shape.rot.x - 0x8000, self->actor.shape.rot.y, 0, MTXMODE_APPLY);
    Matrix_MultVec3f(&D_809B8080, &self->unk158[0]);
    self->unk2A8[13].y = sp5C.y;
    self->unk2A8[13].x = sp5C.x + 0x8000;

    for (i = 0; i < 13; i++) {
        Matrix_Translate(self->unk158[i].x, self->unk158[i].y, self->unk158[i].z, MTXMODE_NEW);
        Math_SmoothStepToS(&self->unk2A8[i].y, self->unk2A8[i + 1].y, 3, self->unk31C, 182);
        Math_SmoothStepToS(&self->unk2A8[i].x, self->unk2A8[i + 1].x, 3, self->unk31C, 182);
        Matrix_RotateRPY(self->unk2A8[i].x - 0x8000, self->unk2A8[i].y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&D_809B8080, &self->unk158[i + 1]);
    }
    self->unk2A8[13].x = self->unk2A8[12].x;
    self->unk2A8[13].y = self->unk2A8[12].y;
    if (!(player->stateFlags1 & 0x4000000) && (self->actor.xzDistToPlayer <= 175.0f) &&
        (self->actor.world.pos.y == self->actor.home.pos.y + 100.0f)) {
        EnBa_SetupSwingAtPlayer(self);
    }
}

void EnBa_SetupFallAsBlob(EnBa* self) {
    self->unk14C = 0;
    self->actor.speedXZ = Rand_CenteredFloat(8.0f);
    self->actor.world.rot.y = Rand_CenteredFloat(65535.0f);
    self->unk318 = 20;
    self->actor.gravity = -2.0f;
    EnBa_SetupAction(self, EnBa_FallAsBlob);
}

/**
 * Action function of the pink fleshy blobs that spawn and fall to the floor when a tentacle dies
 */
void EnBa_FallAsBlob(EnBa* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 1) {
        self->actor.scale.y -= 0.001f;
        self->actor.scale.x += 0.0005f;
        self->actor.scale.z += 0.0005f;
        self->unk318--;
        if (self->unk318 == 0) {
            Actor_Kill(&self->actor);
        }
    } else {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 30.0f, 28.0f, 80.0f, 5);
    }
}

void EnBa_SetupSwingAtPlayer(EnBa* self) {
    self->unk14C = 3;
    self->unk318 = 20;
    self->unk31A = 0;
    self->unk31C = 1500;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.speedXZ = 20.0f;
    EnBa_SetupAction(self, EnBa_SwingAtPlayer);
}

void EnBa_SwingAtPlayer(EnBa* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 temp;
    s16 i;
    Vec3s sp58;
    s16 phi_fp;

    Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 60.0f, 1.0f, 10.0f, 0.0f);
    if ((self->actor.xzDistToPlayer <= 175.0f) || (self->unk31A != 0)) {
        if (self->unk318 == 20) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BALINADE_HAND_UP);
            self->unk31C = 1500;
        }
        if (self->unk318 != 0) {
            self->unk31A = 10;
            self->unk318--;
            if (self->unk318 >= 11) {
                self->unk2FC = player->actor.world.pos;
                self->unk2FC.y += 30.0f;
                phi_fp = self->actor.yawTowardsPlayer;
            } else {
                phi_fp = Math_Vec3f_Yaw(&self->actor.world.pos, &self->unk2FC);
            }
            Math_SmoothStepToS(&self->unk31C, 1500, 1, 30, 0);
            func_80035844(&self->actor.world.pos, &self->unk158[0], &sp58, 0);
            Math_SmoothStepToS(&self->actor.shape.rot.y, sp58.y, 1, self->unk31C, 0);
            Math_SmoothStepToS(&self->actor.shape.rot.x, (sp58.x + 0x8000), 1, self->unk31C, 0);
            Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
            Matrix_RotateRPY((self->actor.shape.rot.x - 0x8000), self->actor.shape.rot.y, 0, MTXMODE_APPLY);
            Matrix_MultVec3f(&D_809B8080, &self->unk158[0]);

            for (i = 0; i < 13; i++) {
                Math_SmoothStepToS(&self->unk2A8[i].x, (i * 1200) - 0x4000, 1, self->unk31C, 0);
                Math_SmoothStepToS(&self->unk2A8[i].y, phi_fp, 1, self->unk31C, 0);
                Matrix_Translate(self->unk158[i].x, self->unk158[i].y, self->unk158[i].z, MTXMODE_NEW);
                Matrix_RotateRPY((self->unk2A8[i].x - 0x8000), self->unk2A8[i].y, 0, MTXMODE_APPLY);
                Matrix_MultVec3f(&D_809B8080, &self->unk158[i + 1]);
            }
        } else {
            if (self->unk31A == 10) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_BALINADE_HAND_DOWN);
            }
            if (self->unk31A != 0) {
                self->unk31C = 8000;
                self->actor.speedXZ = 30.0f;
                phi_fp = Math_Vec3f_Yaw(&self->actor.world.pos, &self->unk2FC);
                temp = Math_Vec3f_Pitch(&self->actor.world.pos, &self->unk158[0]) + 0x8000;
                Math_SmoothStepToS(&self->actor.shape.rot.y, phi_fp, 1, self->unk31C, 0);
                Math_SmoothStepToS(&self->actor.shape.rot.x, temp, 1, self->unk31C, 0);
                Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                 MTXMODE_NEW);
                Matrix_RotateRPY(self->actor.shape.rot.x - 0x8000, self->actor.shape.rot.y, 0, MTXMODE_APPLY);
                Matrix_MultVec3f(&D_809B8080, self->unk158);

                for (i = 0; i < 13; i++) {
                    temp = -Math_CosS(self->unk31A * 0xCCC) * (i * 1200);
                    Math_SmoothStepToS(&self->unk2A8[i].x, temp - 0x4000, 1, self->unk31C, 0);
                    Math_SmoothStepToS(&self->unk2A8[i].y, phi_fp, 1, self->unk31C, 0);
                    Matrix_Translate(self->unk158[i].x, self->unk158[i].y, self->unk158[i].z, MTXMODE_NEW);
                    Matrix_RotateRPY(self->unk2A8[i].x - 0x8000, self->unk2A8[i].y, 0, MTXMODE_APPLY);
                    Matrix_MultVec3f(&D_809B8080, &self->unk158[i + 1]);
                }
                self->unk31A--;
            } else if ((self->actor.xzDistToPlayer > 175.0f) || (player->stateFlags1 & 0x4000000)) {
                EnBa_SetupIdle(self);
            } else {
                EnBa_SetupSwingAtPlayer(self);
                self->unk318 = 27;
                self->unk31C = 750;
            }
        }
        self->unk2A8[13].x = self->unk2A8[12].x;
        self->unk2A8[13].y = self->unk2A8[12].y;
        if (self->collider.base.atFlags & 2) {
            self->collider.base.atFlags &= ~2;
            if (self->collider.base.at == &player->actor) {
                func_8002F71C(globalCtx, &self->actor, 8.0f, self->actor.yawTowardsPlayer, 8.0f);
            }
        }
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        return;
    }
    if ((self->actor.xzDistToPlayer > 175.0f) || (player->stateFlags1 & 0x4000000)) {
        EnBa_SetupIdle(self);
    } else {
        EnBa_SetupSwingAtPlayer(self);
        self->unk318 = 27;
        self->unk31C = 750;
    }
}

void func_809B7174(EnBa* self) {
    self->unk14C = 1;
    self->unk31C = 1500;
    self->unk318 = 20;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.speedXZ = 10.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_BALINADE_HAND_DAMAGE);
    Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 12);
    EnBa_SetupAction(self, EnBa_RecoilFromDamage);
}

void EnBa_RecoilFromDamage(EnBa* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3s sp6C;

    Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 330.0f, 1.0f, 30.0f, 0.0f);
    self->unk2FC = self->actor.world.pos;
    if (globalCtx->gameplayFrames % 16 == 0) {
        self->unk308.z += Rand_CenteredFloat(180.0f);
        self->unk314 += Rand_CenteredFloat(180.0f);
        self->unk308.x = Math_SinF(self->unk308.z) * 80.0f;
        self->unk308.y = Math_CosF(self->unk314) * 80.0f;
    }
    self->unk2FC.y -= 448.0f;
    self->unk2FC.x += self->unk308.x;
    self->unk2FC.z += self->unk308.y;
    func_80033AEC(&self->unk2FC, &self->unk158[13], 1.0f, self->actor.speedXZ, 0.0f, 0.0f);
    for (i = 12; i >= 0; i--) {
        func_80035844(&self->unk158[i + 1], &self->unk158[i], &sp6C, 0);
        Matrix_Translate(self->unk158[i + 1].x, self->unk158[i + 1].y, self->unk158[i + 1].z, MTXMODE_NEW);
        Matrix_RotateRPY(sp6C.x, sp6C.y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&D_809B8080, &self->unk158[i]);
    }
    func_80035844(&self->actor.world.pos, &self->unk158[0], &sp6C, 0);
    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
    Math_SmoothStepToS(&self->actor.shape.rot.y, sp6C.y, 3, self->unk31C, 182);
    Math_SmoothStepToS(&self->actor.shape.rot.x, sp6C.x + 0x8000, 3, self->unk31C, 182);
    Matrix_RotateRPY(self->actor.shape.rot.x - 0x8000, self->actor.shape.rot.y, 0, MTXMODE_APPLY);
    Matrix_MultVec3f(&D_809B8080, &self->unk158[0]);

    for (i = 0; i < 13; i++) {
        func_80035844(&self->unk158[i], &self->unk158[i + 1], &sp6C, 0);
        Matrix_Translate(self->unk158[i].x, self->unk158[i].y, self->unk158[i].z, MTXMODE_NEW);
        Math_SmoothStepToS(&self->unk2A8[i].y, sp6C.y, 3, self->unk31C, 182);
        Math_SmoothStepToS(&self->unk2A8[i].x, sp6C.x + 0x8000, 3, self->unk31C, 182);
        Matrix_RotateRPY(self->unk2A8[i].x - 0x8000, self->unk2A8[i].y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&D_809B8080, &self->unk158[i + 1]);
    }

    self->unk2A8[13].x = self->unk2A8[12].x;
    self->unk2A8[13].y = self->unk2A8[12].y;
    self->unk318--;
    if (self->unk318 == 0) {
        EnBa_SetupIdle(self);
    }
}

void func_809B75A0(EnBa* self, GlobalContext* globalCtx2) {
    s16 unk_temp;
    s32 i;
    Vec3f sp74 = { 0.0f, 0.0f, 0.0f };
    GlobalContext* globalCtx = globalCtx2;

    self->unk31C = 2500;
    EffectSsDeadSound_SpawnStationary(globalCtx, &self->actor.projectedPos, NA_SE_EN_BALINADE_HAND_DEAD, 1, 1, 40);
    self->unk14C = 0;

    for (i = 7; i < 14; i++) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BA, self->unk158[i].x, self->unk158[i].y,
                    self->unk158[i].z, 0, 0, 0, EN_BA_DEAD_BLOB);
    }
    unk_temp = Math_Vec3f_Pitch(&self->actor.world.pos, &self->unk158[0]) + 0x8000;
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, self->unk31C, 0);
    Math_SmoothStepToS(&self->actor.shape.rot.x, unk_temp, 1, self->unk31C, 0);
    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->actor.shape.rot.x - 0x8000, self->actor.shape.rot.y, 0, MTXMODE_APPLY);
    Matrix_MultVec3f(&D_809B8080, &self->unk158[0]);
    self->actor.flags &= ~1;
    for (i = 5; i < 13; i++) {
        Math_SmoothStepToS(&self->unk2A8[i].x, self->unk2A8[5].x, 1, self->unk31C, 0);
        Math_SmoothStepToS(&self->unk2A8[i].y, self->unk2A8[5].y, 1, self->unk31C, 0);
        Matrix_Translate(self->unk158[i].x, self->unk158[i].y, self->unk158[i].z, MTXMODE_NEW);
        Matrix_RotateRPY(self->unk2A8[i].x - 0x8000, self->unk2A8[i].y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&sp74, &self->unk158[i + 1]);
    }
    self->unk31A = 15;
    EnBa_SetupAction(self, EnBa_Die);
}

void EnBa_Die(EnBa* self, GlobalContext* globalCtx) {
    Vec3f sp6C = { 0.0f, 0.0f, 0.0f };
    s16 temp;
    s32 i;

    if (self->unk31A != 0) {
        self->actor.speedXZ = 30.0f;
        self->unk31C = 8000;
        self->actor.world.pos.y += 8.0f;
        temp = Math_Vec3f_Pitch(&self->actor.world.pos, &self->unk158[0]) + 0x8000;
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, self->unk31C, 0);
        Math_SmoothStepToS(&self->actor.shape.rot.x, temp, 1, self->unk31C, 0);
        Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
        Matrix_RotateRPY(self->actor.shape.rot.x - 0x8000, self->actor.shape.rot.y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&D_809B8080, &self->unk158[0]);
        for (i = 0; i < 5; i++) {
            temp = -Math_CosS(self->unk31A * 0x444) * (i * 400);
            Math_SmoothStepToS(&self->unk2A8[i].x, temp - 0x4000, 1, self->unk31C, 0);
            Math_SmoothStepToS(&self->unk2A8[i].y, self->actor.yawTowardsPlayer, 1, self->unk31C, 0);
            Matrix_Translate(self->unk158[i].x, self->unk158[i].y, self->unk158[i].z, MTXMODE_NEW);
            Matrix_RotateRPY(self->unk2A8[i].x - 0x8000, self->unk2A8[i].y, 0, MTXMODE_APPLY);
            Matrix_MultVec3f(&D_809B8080, &self->unk158[i + 1]);
        }
        for (i = 5; i < 13; i++) {
            Math_SmoothStepToS(&self->unk2A8[i].x, self->unk2A8[5].x, 1, self->unk31C, 0);
            Math_SmoothStepToS(&self->unk2A8[i].y, self->unk2A8[5].y, 1, self->unk31C, 0);
            Matrix_Translate(self->unk158[i].x, self->unk158[i].y, self->unk158[i].z, MTXMODE_NEW);
            Matrix_RotateRPY(self->unk2A8[i].x - 0x8000, self->unk2A8[i].y, 0, MTXMODE_APPLY);
            Matrix_MultVec3f(&sp6C, &self->unk158[i + 1]);
        }
        self->unk31A--;
    } else {
        Flags_SetSwitch(globalCtx, self->upperParams);
        Actor_Kill(&self->actor);
    }
}

void EnBa_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBa* self = THIS;

    if ((self->actor.params < EN_BA_DEAD_BLOB) && (self->collider.base.acFlags & 2)) {
        self->collider.base.acFlags &= ~2;
        self->actor.colChkInfo.health--;
        if (self->actor.colChkInfo.health == 0) {
            func_809B75A0(self, globalCtx);
        } else {
            func_809B7174(self);
        }
    }
    self->actionFunc(self, globalCtx);
    if (self->actor.params < EN_BA_DEAD_BLOB) {
        self->actor.focus.pos = self->unk158[6];
    }
    if (self->unk14C >= 2) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

static Gfx* D_809B8118[] = { 0x060024F0, 0x060027F0, 0x060029F0 };

void EnBa_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBa* self = THIS;
    s32 pad;
    s16 i;
    Mtx* mtx = Graph_Alloc(globalCtx->state.gfxCtx, sizeof(Mtx) * 14);
    Vec3f unused = { 0.0f, 0.0f, 448.0f };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ba.c", 933);
    func_80093D18(globalCtx->state.gfxCtx);
    if (self->actor.params < EN_BA_DEAD_BLOB) {
        Matrix_Push();
        gSPSegment(POLY_OPA_DISP++, 0x0C, mtx);
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_809B8118[self->actor.params]));
        gSPSegment(POLY_OPA_DISP++, 0x09,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 16, 16, 1, 0,
                                    (globalCtx->gameplayFrames * -10) % 128, 32, 32));
        for (i = 0; i < 14; i++, mtx++) {
            Matrix_Translate(self->unk158[i].x, self->unk158[i].y, self->unk158[i].z, MTXMODE_NEW);
            Matrix_RotateRPY(self->unk2A8[i].x, self->unk2A8[i].y, self->unk2A8[i].z, MTXMODE_APPLY);
            Matrix_Scale(self->unk200[i].x, self->unk200[i].y, self->unk200[i].z, MTXMODE_APPLY);
            if ((i == 6) || (i == 13)) {
                switch (i) {
                    case 13:
                        Collider_UpdateSpheres(i, &self->collider);
                        break;
                    default:
                        Matrix_Scale(0.5f, 0.5f, 1.0f, MTXMODE_APPLY);
                        Collider_UpdateSpheres(8, &self->collider);
                        break;
                }
            }
            Matrix_ToMtx(mtx, "../z_en_ba.c", 970);
        }
        Matrix_Pop();
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ba.c", 973),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, object_bxa_DL_000890);
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (globalCtx->gameplayFrames * 2) % 128,
                                    (globalCtx->gameplayFrames * 2) % 128, 32, 32, 1,
                                    (globalCtx->gameplayFrames * -5) % 128, (globalCtx->gameplayFrames * -5) % 128, 32,
                                    32));
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 125, 100, 255);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ba.c", 991),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, object_bxa_DL_001D80);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ba.c", 995);
}
