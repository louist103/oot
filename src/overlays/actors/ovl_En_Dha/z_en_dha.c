/*
 * File: z_en_dha.c
 * Overlay: ovl_En_Dha
 * Description: Dead Hand's Hand
 */

#include "z_en_dha.h"
#include "overlays/actors/ovl_En_Dh/z_en_dh.h"
#include "objects/object_dh/object_dh.h"

#define FLAGS 0x00000015

#define THIS ((EnDha*)thisx)

void EnDha_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDha_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDha_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDha_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDha_SetupWait(EnDha* self);
void EnDha_Wait(EnDha* self, GlobalContext* globalCtx);
void EnDha_SetupTakeDamage(EnDha* self);
void EnDha_TakeDamage(EnDha* self, GlobalContext* globalCtx);
void EnDha_SetupDeath(EnDha* self);
void EnDha_Die(EnDha* self, GlobalContext* globalCtx);
void EnDha_UpdateHealth(EnDha* self, GlobalContext* globalCtx);

const ActorInit En_Dha_InitVars = {
    ACTOR_EN_DHA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DH,
    sizeof(EnDha),
    (ActorFunc)EnDha_Init,
    (ActorFunc)EnDha_Destroy,
    (ActorFunc)EnDha_Update,
    (ActorFunc)EnDha_Draw,
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(2, 0xF),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(2, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(2, 0xF),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(4, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 1, { { 0, 0, 0 }, 12 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 2, { { 3200, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 3, { { 1200, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 4, { { 2700, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 5, { { 1200, 0, 0 }, 10 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT6,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER | OC1_TYPE_1,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    5,
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x2E, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_STOP),
};

void EnDha_SetupAction(EnDha* self, EnDhaActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnDha_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDha* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.colChkInfo.damageTable = &sDamageTable;
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_dh_Skel_000BD8, &object_dh_Anim_0015B0, self->jointTable,
                       self->morphTable, 4);
    ActorShape_Init(&self->actor.shape, 0, ActorShadow_DrawFeet, 90.0f);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 50.0f;
    self->actor.colChkInfo.mass = MASS_HEAVY;
    self->actor.colChkInfo.health = 8;
    self->limbAngleX[0] = -0x4000;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItem);
    self->actor.flags &= ~1;

    EnDha_SetupWait(self);
}

void EnDha_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDha* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnDha_SetupWait(EnDha* self) {
    Animation_PlayLoop(&self->skelAnime, &object_dh_Anim_0015B0);
    self->unk_1C0 = 0;
    self->actionTimer = ((Rand_ZeroOne() * 10.0f) + 5.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.home.rot.z = 1;
    EnDha_SetupAction(self, EnDha_Wait);
}

void EnDha_Wait(EnDha* self, GlobalContext* globalCtx) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f }; // unused
    Vec3f armPosMultiplier1 = { 0.0f, 0.0f, 55.0f };
    Vec3f armPosMultiplier2 = { 0.0f, 0.0f, -54.0f };
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    s32 pad2;
    Vec3f playerPos = player->actor.world.pos;
    Vec3s angle;
    s16 yaw;

    playerPos.x += Math_SinS(player->actor.shape.rot.y) * -5.0f;
    playerPos.z += Math_CosS(player->actor.shape.rot.y) * -5.0f;

    if (!LINK_IS_ADULT) {
        playerPos.y += 38.0f;
    } else {
        playerPos.y += 56.0f;
    }

    if (self->actor.xzDistToPlayer <= 100.0f) {
        self->handAngle.y = self->handAngle.x = self->limbAngleY = 0;

        if (Math_Vec3f_DistXYZ(&playerPos, &self->handPos[0]) <= 12.0f) {
            if (self->unk_1CC == 0) {
                if (globalCtx->grabPlayer(globalCtx, player)) {
                    self->timer = 0;
                    self->unk_1CC++;

                    if (self->actor.parent != NULL) {
                        self->actor.parent->params = ENDH_START_ATTACK_GRAB;
                    }

                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_GRIP);
                }
            } else {
                self->timer += 0x1194;
                self->limbAngleY = Math_SinS(self->timer) * 1820.0f;

                if (!(player->stateFlags2 & 0x80)) {
                    self->unk_1CC = 0;
                    EnDha_SetupTakeDamage(self);
                    return;
                }

                if (self->timer < -0x6E6B) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_GRIP);
                }
            }

            func_80035844(&self->handPos[1], &playerPos, &self->handAngle, 0);
            self->handAngle.y -= self->actor.shape.rot.y + self->limbAngleY;
            self->handAngle.x -= self->actor.shape.rot.x + self->limbAngleX[0] + self->limbAngleX[1];
        } else {
            if ((player->stateFlags2 & 0x80) && (&self->actor == player->actor.parent)) {
                player->stateFlags2 &= ~0x80;
                player->actor.parent = NULL;
                player->unk_850 = 200;
            }

            if (self->actor.home.rot.z != 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_HAND_AT);
                self->actor.home.rot.z = 0;
            }
        }

        self->actor.shape.rot.y = Math_Vec3f_Yaw(&self->actor.world.pos, &playerPos);

        Math_SmoothStepToF(&self->handPos[0].x, playerPos.x, 1.0f, 16.0f, 0.0f);
        Math_SmoothStepToF(&self->handPos[0].y, playerPos.y, 1.0f, 16.0f, 0.0f);
        Math_SmoothStepToF(&self->handPos[0].z, playerPos.z, 1.0f, 16.0f, 0.0f);

        func_80035844(&self->armPos, &self->handPos[0], &angle, 0);
        Matrix_Translate(self->handPos[0].x, self->handPos[0].y, self->handPos[0].z, MTXMODE_NEW);
        Matrix_RotateRPY(angle.x, angle.y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&armPosMultiplier2, &self->armPos);
        Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
        func_80035844(&self->actor.world.pos, &self->armPos, &angle, 0);
        Matrix_RotateRPY(angle.x, angle.y, 0, MTXMODE_APPLY);
        Matrix_MultVec3f(&armPosMultiplier1, &self->armPos);
        self->limbAngleX[0] = Math_Vec3f_Pitch(&self->actor.world.pos, &self->armPos);
        yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->armPos) - self->actor.shape.rot.y;

        if (ABS(yaw) >= 0x4000) {
            self->limbAngleX[0] = -0x8000 - self->limbAngleX[0];
        }

        self->limbAngleX[1] = (Math_Vec3f_Pitch(&self->armPos, &self->handPos[0]) - self->limbAngleX[0]);

        if (self->limbAngleX[1] < 0) {
            self->limbAngleX[0] += self->limbAngleX[1] * 2;
            self->limbAngleX[1] *= -2;
        }
    } else {
        if ((player->stateFlags2 & 0x80) && (&self->actor == player->actor.parent)) {
            player->stateFlags2 &= ~0x80;
            player->actor.parent = NULL;
            player->unk_850 = 200;
        }

        self->actor.home.rot.z = 1;
        Math_SmoothStepToS(&self->limbAngleX[1], 0, 1, 0x3E8, 0);
        Math_SmoothStepToS(&self->limbAngleX[0], -0x4000, 1, 0x3E8, 0);
        SkelAnime_Update(&self->skelAnime);
    }
}

void EnDha_SetupTakeDamage(EnDha* self) {
    self->actionTimer = 15;
    EnDha_SetupAction(self, EnDha_TakeDamage);
}

void EnDha_TakeDamage(EnDha* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((player->stateFlags2 & 0x80) && (&self->actor == player->actor.parent)) {
        player->stateFlags2 &= ~0x80;
        player->actor.parent = NULL;
        player->unk_850 = 200;
    }

    Math_SmoothStepToS(&self->limbAngleX[1], 0, 1, 2000, 0);
    Math_SmoothStepToS(&self->limbAngleY, 0, 1, 600, 0);
    Math_SmoothStepToS(&self->limbAngleX[0], -0x4000, 1, 2000, 0);
    SkelAnime_Update(&self->skelAnime);
    self->actionTimer--;

    if (self->actionTimer == 0) {
        EnDha_SetupWait(self);
    }
}

void EnDha_SetupDeath(EnDha* self) {
    self->unk_1C0 = 8;
    self->actionTimer = 300;

    if (self->actor.parent != NULL) {
        if (self->actor.parent->params != ENDH_DEATH) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_HAND_DEAD);
        }
        if (self->actor.parent->params <= ENDH_WAIT_UNDERGROUND) {
            self->actor.parent->params--;
        }
    }

    EnDha_SetupAction(self, EnDha_Die);
}

void EnDha_Die(EnDha* self, GlobalContext* globalCtx) {
    s16 angle;
    Vec3f vec;
    Player* player = GET_PLAYER(globalCtx);

    if ((player->stateFlags2 & 0x80) && (&self->actor == player->actor.parent)) {
        player->stateFlags2 &= ~0x80;
        player->actor.parent = NULL;
        player->unk_850 = 200;
    }

    Math_SmoothStepToS(&self->limbAngleX[1], 0, 1, 0x7D0, 0);
    angle = Math_SmoothStepToS(&self->limbAngleX[0], -0x4000, 1, 0x7D0, 0);
    SkelAnime_Update(&self->skelAnime);

    if (angle == 0) {
        vec = self->actor.world.pos;

        if (self->actionTimer != 0) {
            if (-12000.0f < self->actor.shape.yOffset) {
                self->actor.shape.yOffset -= 1000.0f;
                func_80033480(globalCtx, &vec, 7.0f, 1, 0x5A, 0x14, 1);
            } else {
                self->actionTimer--;

                if ((self->actor.parent != NULL) && (self->actor.parent->params == ENDH_DEATH)) {
                    Actor_Kill(&self->actor);
                }
            }
        } else {
            self->actor.shape.yOffset += 500.0f;
            func_80033480(globalCtx, &vec, 7.0f, 1, 0x5A, 0x14, 1);

            if (self->actor.shape.yOffset == 0.0f) {
                EnDha_SetupWait(self);
            }
        }
    }
}

void EnDha_UpdateHealth(EnDha* self, GlobalContext* globalCtx) {
    if (!((self->unk_1C0 >= 8) || !(self->collider.base.acFlags & AC_HIT))) {
        self->collider.base.acFlags &= ~AC_HIT;

        if (self->actor.colChkInfo.damageEffect == 0 || self->actor.colChkInfo.damageEffect == 6) {
            return;
        } else {
            Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
            if (Actor_ApplyDamage(&self->actor) == 0) {
                EnDha_SetupDeath(self);
                self->actor.colChkInfo.health = 8;
                Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xE0);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_DAMAGE);
                self->unk_1C0 = 9;
                EnDha_SetupTakeDamage(self);
            }
        }
    }

    if ((self->actor.parent != NULL) && (self->actor.parent->params == ENDH_DEATH)) {
        EnDha_SetupDeath(self);
    }
}

void EnDha_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDha* self = THIS;

    if (self->actor.parent == NULL) {
        self->actor.parent = Actor_FindNearby(globalCtx, &self->actor, ACTOR_EN_DH, ACTORCAT_ENEMY, 10000.0f);
    }

    EnDha_UpdateHealth(self, globalCtx);
    self->actionFunc(self, globalCtx);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

s32 EnDha_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnDha* self = THIS;

    if (limbIndex == 1) {
        rot->y = -(s16)(self->limbAngleX[0] + 0x4000);
        rot->z += self->limbAngleY;
    } else if (limbIndex == 2) {
        rot->z = self->limbAngleX[1];
        rot->y -= self->limbAngleY;
    } else if (limbIndex == 3) {
        rot->y = -self->handAngle.y;
        rot->z = -self->handAngle.x;
    }

    return false;
}

void EnDha_OverridePostDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    Vec3f handVec = { 1100.0f, 0.0f, 0.0f };
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    EnDha* self = THIS;

    switch (limbIndex) {
        case 1:
            Collider_UpdateSpheres(2, &self->collider);
            Collider_UpdateSpheres(3, &self->collider);
            break;
        case 2:
            Collider_UpdateSpheres(4, &self->collider);
            Collider_UpdateSpheres(5, &self->collider);
            Matrix_MultVec3f(&zeroVec, &self->armPos);
            break;
        case 3:
            Collider_UpdateSpheres(1, &self->collider);
            Matrix_MultVec3f(&handVec, &self->handPos[0]);
            Matrix_MultVec3f(&zeroVec, &self->handPos[1]);
            break;
    }
}

void EnDha_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDha* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnDha_OverrideLimbDraw, EnDha_OverridePostDraw, self);
}
