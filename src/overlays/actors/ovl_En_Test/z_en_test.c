/*
 * File: z_en_test.c
 * Overlay: ovl_En_Test
 * Description: Stalfos
 */

#include "z_en_test.h"
#include "objects/object_sk2/object_sk2.h"

#define FLAGS 0x00000015

#define THIS ((EnTest*)thisx)

void EnTest_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTest_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTest_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTest_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnTest_SetupWaitGround(EnTest* self);
void EnTest_SetupWaitAbove(EnTest* self);
void EnTest_SetupJumpBack(EnTest* self);
void EnTest_SetupSlashDownEnd(EnTest* self);
void EnTest_SetupSlashUp(EnTest* self);
void EnTest_SetupJumpslash(EnTest* self);
void EnTest_SetupWalkAndBlock(EnTest* self);
void func_80860EC0(EnTest* self);
void EnTest_SetupSlashDown(EnTest* self);
void func_80860BDC(EnTest* self);
void EnTest_SetupIdleFromBlock(EnTest* self);
void EnTest_SetupRecoil(EnTest* self);
void func_80862398(EnTest* self);
void func_80862154(EnTest* self);
void EnTest_SetupStopAndBlock(EnTest* self);
void func_808627C4(EnTest* self, GlobalContext* globalCtx);

void EnTest_WaitGround(EnTest* self, GlobalContext* globalCtx);
void EnTest_WaitAbove(EnTest* self, GlobalContext* globalCtx);
void EnTest_Fall(EnTest* self, GlobalContext* globalCtx);
void EnTest_Land(EnTest* self, GlobalContext* globalCtx);
void EnTest_Rise(EnTest* self, GlobalContext* globalCtx);
void EnTest_Idle(EnTest* self, GlobalContext* globalCtx);
void EnTest_WalkAndBlock(EnTest* self, GlobalContext* globalCtx);
void func_80860C24(EnTest* self, GlobalContext* globalCtx);
void func_80860F84(EnTest* self, GlobalContext* globalCtx);
void EnTest_SlashDown(EnTest* self, GlobalContext* globalCtx);
void EnTest_SlashDownEnd(EnTest* self, GlobalContext* globalCtx);
void EnTest_SlashUp(EnTest* self, GlobalContext* globalCtx);
void EnTest_JumpBack(EnTest* self, GlobalContext* globalCtx);
void EnTest_Jumpslash(EnTest* self, GlobalContext* globalCtx);
void EnTest_JumpUp(EnTest* self, GlobalContext* globalCtx);
void EnTest_StopAndBlock(EnTest* self, GlobalContext* globalCtx);
void EnTest_IdleFromBlock(EnTest* self, GlobalContext* globalCtx);
void func_808621D4(EnTest* self, GlobalContext* globalCtx);
void func_80862418(EnTest* self, GlobalContext* globalCtx);
void EnTest_Stunned(EnTest* self, GlobalContext* globalCtx);
void func_808628C8(EnTest* self, GlobalContext* globalCtx);
void func_80862E6C(EnTest* self, GlobalContext* globalCtx);
void func_80863044(EnTest* self, GlobalContext* globalCtx);
void func_8086318C(EnTest* self, GlobalContext* globalCtx);
void EnTest_Recoil(EnTest* self, GlobalContext* globalCtx);
void func_808633E8(EnTest* self, GlobalContext* globalCtx);
void func_80862FA8(EnTest* self, GlobalContext* globalCtx);

s32 EnTest_ReactToProjectile(GlobalContext* globalCtx, EnTest* self);

static u8 sJointCopyFlags[] = {
    false, // STALFOS_LIMB_NONE
    false, // STALFOS_LIMB_ROOT
    false, // STALFOS_LIMB_UPPERBODY_ROOT
    false, // STALFOS_LIMB_CORE_LOWER_ROOT
    true,  // STALFOS_LIMB_CORE_UPPER_ROOT
    true,  // STALFOS_LIMB_NECK_ROOT
    true,  // STALFOS_LIMB_HEAD_ROOT
    true,  // STALFOS_LIMB_7
    true,  // STALFOS_LIMB_8
    true,  // STALFOS_LIMB_JAW_ROOT
    true,  // STALFOS_LIMB_JAW
    true,  // STALFOS_LIMB_HEAD
    true,  // STALFOS_LIMB_NECK_UPPER
    true,  // STALFOS_LIMB_NECK_LOWER
    true,  // STALFOS_LIMB_CORE_UPPER
    true,  // STALFOS_LIMB_CHEST
    true,  // STALFOS_LIMB_SHOULDER_R_ROOT
    true,  // STALFOS_LIMB_SHOULDER_ARMOR_R_ROOT
    true,  // STALFOS_LIMB_SHOULDER_ARMOR_R
    true,  // STALFOS_LIMB_SHOULDER_L_ROOT
    true,  // STALFOS_LIMB_SHOULDER_ARMOR_L_ROOT
    true,  // STALFOS_LIMB_SHOULDER_ARMOR_L
    true,  // STALFOS_LIMB_ARM_L_ROOT
    true,  // STALFOS_LIMB_UPPERARM_L_ROOT
    true,  // STALFOS_LIMB_FOREARM_L_ROOT
    true,  // STALFOS_LIMB_HAND_L_ROOT
    true,  // STALFOS_LIMB_HAND_L
    true,  // STALFOS_LIMB_SHIELD
    true,  // STALFOS_LIMB_FOREARM_L
    true,  // STALFOS_LIMB_UPPERARM_L
    true,  // STALFOS_LIMB_ARM_R_ROOT
    true,  // STALFOS_LIMB_UPPERARM_R_ROOT
    true,  // STALFOS_LIMB_FOREARM_R_ROOT
    true,  // STALFOS_LIMB_HAND_R_ROOT
    true,  // STALFOS_LIMB_SWORD
    true,  // STALFOS_LIMB_HAND_R
    true,  // STALFOS_LIMB_FOREARM_R
    true,  // STALFOS_LIMB_UPPERARM_R
    true,  // STALFOS_LIMB_CORE_LOWER
    false, // STALFOS_LIMB_LOWERBODY_ROOT
    false, // STALFOS_LIMB_WAIST_ROOT
    false, // STALFOS_LIMB_LEGS_ROOT
    false, // STALFOS_LIMB_LEG_L_ROOT
    false, // STALFOS_LIMB_THIGH_L_ROOT
    false, // STALFOS_LIMB_LOWERLEG_L_ROOT
    false, // STALFOS_LIMB_ANKLE_L_ROOT
    false, // STALFOS_LIMB_ANKLE_L
    false, // STALFOS_LIMB_FOOT_L_ROOT
    false, // STALFOS_LIMB_FOOT_L
    false, // STALFOS_LIMB_LOWERLEG_L
    false, // STALFOS_LIMB_THIGH_L
    false, // STALFOS_LIMB_LEG_R_ROOT
    false, // STALFOS_LIMB_THIGH_R_ROOT
    false, // STALFOS_LIMB_LOWERLEG_R_ROOT
    false, // STALFOS_LIMB_ANKLE_R_ROOT
    false, // STALFOS_LIMB_ANKLE_R
    false, // STALFOS_LIMB_FOOT_R_ROOT
    false, // STALFOS_LIMB_FOOT_R
    false, // STALFOS_LIMB_LOWERLEG_R
    false, // STALFOS_LIMB_THIGH_R
    false, // STALFOS_LIMB_WAIST
};

const ActorInit En_Test_InitVars = {
    ACTOR_EN_TEST,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SK2,
    sizeof(EnTest),
    (ActorFunc)EnTest_Init,
    (ActorFunc)EnTest_Destroy,
    (ActorFunc)EnTest_Update,
    (ActorFunc)EnTest_Draw,
};

static ColliderCylinderInit sBodyColliderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 25, 65, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sShieldColliderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFC1FFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 20, 70, -50, { 0, 0, 0 } },
};

static ColliderQuadInit sSwordColliderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x10 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL | TOUCH_UNK7,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum {
    /* 0x0 */ STALFOS_DMGEFF_NORMAL,
    /* 0x1 */ STALFOS_DMGEFF_STUN,
    /* 0x6 */ STALFOS_DMGEFF_FIREMAGIC = 6,
    /* 0xD */ STALFOS_DMGEFF_SLING = 0xD,
    /* 0xE */ STALFOS_DMGEFF_LIGHT,
    /* 0xF */ STALFOS_DMGEFF_FREEZE
} StalfosDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, STALFOS_DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Slingshot     */ DMG_ENTRY(1, STALFOS_DMGEFF_SLING),
    /* Explosive     */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Boomerang     */ DMG_ENTRY(0, STALFOS_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Hammer swing  */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Hookshot      */ DMG_ENTRY(0, STALFOS_DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, STALFOS_DMGEFF_NORMAL),
    /* Master sword  */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Giant's Knife */ DMG_ENTRY(4, STALFOS_DMGEFF_NORMAL),
    /* Fire arrow    */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Ice arrow     */ DMG_ENTRY(4, STALFOS_DMGEFF_FREEZE),
    /* Light arrow   */ DMG_ENTRY(2, STALFOS_DMGEFF_LIGHT),
    /* Unk arrow 1   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Unk arrow 2   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Unk arrow 3   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Fire magic    */ DMG_ENTRY(0, STALFOS_DMGEFF_FIREMAGIC),
    /* Ice magic     */ DMG_ENTRY(3, STALFOS_DMGEFF_FREEZE),
    /* Light magic   */ DMG_ENTRY(0, STALFOS_DMGEFF_LIGHT),
    /* Shield        */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
    /* Mirror Ray    */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
    /* Kokiri spin   */ DMG_ENTRY(1, STALFOS_DMGEFF_NORMAL),
    /* Giant spin    */ DMG_ENTRY(4, STALFOS_DMGEFF_NORMAL),
    /* Master spin   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Kokiri jump   */ DMG_ENTRY(2, STALFOS_DMGEFF_NORMAL),
    /* Giant jump    */ DMG_ENTRY(8, STALFOS_DMGEFF_NORMAL),
    /* Master jump   */ DMG_ENTRY(4, STALFOS_DMGEFF_NORMAL),
    /* Unknown 1     */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
    /* Unblockable   */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
    /* Hammer jump   */ DMG_ENTRY(4, STALFOS_DMGEFF_NORMAL),
    /* Unknown 2     */ DMG_ENTRY(0, STALFOS_DMGEFF_NORMAL),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x1B, ICHAIN_CONTINUE),    ICHAIN_F32(targetArrowOffset, 500, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 15, ICHAIN_CONTINUE), ICHAIN_F32(scale.y, 0, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1500, ICHAIN_STOP),
};

void EnTest_SetupAction(EnTest* self, EnTestActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnTest_Init(Actor* thisx, GlobalContext* globalCtx) {
    EffectBlureInit1 slashBlure;
    EnTest* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);

    SkelAnime_Init(globalCtx, &self->skelAnime, &gStalfosSkel, &gStalfosMiddleGuardAnim, self->jointTable,
                   self->morphTable, STALFOS_LIMB_MAX);
    SkelAnime_Init(globalCtx, &self->upperSkelanime, &gStalfosSkel, &gStalfosMiddleGuardAnim, self->upperJointTable,
                   self->upperMorphTable, STALFOS_LIMB_MAX);

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);

    self->actor.colChkInfo.cylRadius = 40;
    self->actor.colChkInfo.cylHeight = 100;
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 45.0f;
    self->actor.colChkInfo.damageTable = &sDamageTable;

    Collider_InitCylinder(globalCtx, &self->bodyCollider);
    Collider_SetCylinder(globalCtx, &self->bodyCollider, &self->actor, &sBodyColliderInit);

    Collider_InitCylinder(globalCtx, &self->shieldCollider);
    Collider_SetCylinder(globalCtx, &self->shieldCollider, &self->actor, &sShieldColliderInit);

    Collider_InitQuad(globalCtx, &self->swordCollider);
    Collider_SetQuad(globalCtx, &self->swordCollider, &self->actor, &sSwordColliderInit);

    self->actor.colChkInfo.mass = MASS_HEAVY;
    self->actor.colChkInfo.health = 10;

    slashBlure.p1StartColor[0] = slashBlure.p1StartColor[1] = slashBlure.p1StartColor[2] = slashBlure.p1StartColor[3] =
        slashBlure.p2StartColor[0] = slashBlure.p2StartColor[1] = slashBlure.p2StartColor[2] =
            slashBlure.p1EndColor[0] = slashBlure.p1EndColor[1] = slashBlure.p1EndColor[2] = slashBlure.p2EndColor[0] =
                slashBlure.p2EndColor[1] = slashBlure.p2EndColor[2] = 255;

    slashBlure.p1EndColor[3] = 0;
    slashBlure.p2EndColor[3] = 0;
    slashBlure.p2StartColor[3] = 64;

    slashBlure.elemDuration = 4;
    slashBlure.unkFlag = 0;
    slashBlure.calcMode = 2;

    Effect_Add(globalCtx, &self->effectIndex, EFFECT_BLURE1, 0, 0, &slashBlure);

    if (self->actor.params != STALFOS_TYPE_CEILING) {
        EnTest_SetupWaitGround(self);
    } else {
        EnTest_SetupWaitAbove(self);
    }

    if (self->actor.params == STALFOS_TYPE_INVISIBLE) {
        self->actor.flags |= 0x80;
    }
}

void EnTest_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnTest* self = THIS;

    if ((self->actor.params != STALFOS_TYPE_2) &&
        !Actor_FindNearby(globalCtx, &self->actor, ACTOR_EN_TEST, ACTORCAT_ENEMY, 8000.0f)) {
        func_800F5B58();
    }

    Effect_Delete(globalCtx, self->effectIndex);
    Collider_DestroyCylinder(globalCtx, &self->shieldCollider);
    Collider_DestroyCylinder(globalCtx, &self->bodyCollider);
    Collider_DestroyQuad(globalCtx, &self->swordCollider);
}

/**
 * If EnTest_ChooseAction failed to pick a new action, self function will unconditionally pick
 * a new action as a last resort
 */
void EnTest_ChooseRandomAction(EnTest* self, GlobalContext* globalCtx) {
    switch ((u32)(Rand_ZeroOne() * 10.0f)) {
        case 0:
        case 1:
        case 5:
        case 6:
            if ((self->actor.xzDistToPlayer < 220.0f) && (self->actor.xzDistToPlayer > 170.0f) &&
                Actor_IsFacingPlayer(&self->actor, 0x71C) && Actor_IsTargeted(globalCtx, &self->actor)) {
                EnTest_SetupJumpslash(self);
                break;
            }
            // fallthrough
        case 8:
            EnTest_SetupWalkAndBlock(self);
            break;

        case 3:
        case 4:
        case 7:
            func_808627C4(self, globalCtx);
            break;

        case 2:
        case 9:
        case 10:
            EnTest_SetupStopAndBlock(self);
            break;
    }
}

void EnTest_ChooseAction(EnTest* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s16 yawDiff = player->actor.shape.rot.y - self->actor.shape.rot.y;

    yawDiff = ABS(yawDiff);

    if (yawDiff >= 0x61A8) {
        switch ((u32)(Rand_ZeroOne() * 10.0f)) {
            case 0:
            case 3:
            case 7:
                EnTest_SetupStopAndBlock(self);
                break;

            case 1:
            case 5:
            case 6:
            case 8:
                func_808627C4(self, globalCtx);
                break;

            case 2:
            case 4:
            case 9:
                if (self->actor.params != STALFOS_TYPE_CEILING) {
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    EnTest_SetupJumpBack(self);
                }
                break;
        }
    } else if (yawDiff <= 0x3E80) {
        if (ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) > 0x3E80) {
            if (((globalCtx->gameplayFrames % 2) != 0) && (self->actor.params != STALFOS_TYPE_CEILING)) {
                self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                EnTest_SetupJumpBack(self);
            } else if ((self->actor.xzDistToPlayer < 220.0f) && (self->actor.xzDistToPlayer > 170.0f)) {
                if (Actor_IsFacingPlayer(&self->actor, 0x71C) && !Actor_IsTargeted(globalCtx, &self->actor)) {
                    EnTest_SetupJumpslash(self);
                }
            } else {
                EnTest_SetupWalkAndBlock(self);
            }
        } else {
            if (self->actor.xzDistToPlayer < 110.0f) {
                if (Rand_ZeroOne() > 0.2f) {
                    if (player->stateFlags1 & 0x10) {
                        if (self->actor.isTargeted) {
                            EnTest_SetupSlashDown(self);
                        } else {
                            func_808627C4(self, globalCtx);
                        }
                    } else {
                        EnTest_SetupSlashDown(self);
                    }
                }
            } else {
                EnTest_ChooseRandomAction(self, globalCtx);
            }
        }
    } else {
        EnTest_ChooseRandomAction(self, globalCtx);
    }
}

void EnTest_SetupWaitGround(EnTest* self) {
    Animation_PlayLoop(&self->skelAnime, &gStalfosMiddleGuardAnim);
    self->unk_7C8 = 0;
    self->timer = 15;
    self->actor.scale.y = 0.0f;
    self->actor.world.pos.y = self->actor.home.pos.y - 3.5f;
    self->actor.flags &= ~1;
    EnTest_SetupAction(self, EnTest_WaitGround);
}

void EnTest_WaitGround(EnTest* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if ((self->timer == 0) && (ABS(self->actor.yDistToPlayer) < 150.0f)) {
        self->unk_7C8 = 3;
        EnTest_SetupAction(self, EnTest_Rise);
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer;

        if (self->actor.params != STALFOS_TYPE_2) {
            func_800F5ACC(0x38);
        }
    } else {
        if (self->timer != 0) {
            self->timer--;
        }

        self->actor.world.pos.y = self->actor.home.pos.y - 3.5f;
    }
}

void EnTest_SetupWaitAbove(EnTest* self) {
    Animation_PlayLoop(&self->skelAnime, &gStalfosMiddleGuardAnim);
    self->unk_7C8 = 0;
    self->actor.world.pos.y = self->actor.home.pos.y + 150.0f;
    Actor_SetScale(&self->actor, 0.0f);
    self->actor.flags &= ~1;
    EnTest_SetupAction(self, EnTest_WaitAbove);
}

void EnTest_WaitAbove(EnTest* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    self->actor.world.pos.y = self->actor.home.pos.y + 150.0f;

    if ((self->actor.xzDistToPlayer < 200.0f) && (ABS(self->actor.yDistToPlayer) < 450.0f)) {
        EnTest_SetupAction(self, EnTest_Fall);
        self->actor.flags |= 1;
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        Actor_SetScale(&self->actor, 0.015f);
    }
}

void EnTest_SetupIdle(EnTest* self) {
    Animation_PlayLoop(&self->skelAnime, &gStalfosMiddleGuardAnim);
    self->unk_7C8 = 0xA;
    self->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnTest_SetupAction(self, EnTest_Idle);
}

void EnTest_Idle(EnTest* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 yawDiff;

    SkelAnime_Update(&self->skelAnime);

    if (!EnTest_ReactToProjectile(globalCtx, self)) {
        yawDiff = player->actor.shape.rot.y - self->actor.shape.rot.y;

        if (self->actor.xzDistToPlayer < 100.0f) {
            if ((player->swordState != 0) && (ABS(yawDiff) >= 0x1F40)) {
                self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;

                if (Rand_ZeroOne() > 0.7f && player->swordAnimation != 0x11) {
                    EnTest_SetupJumpBack(self);
                } else {
                    func_808627C4(self, globalCtx);
                }
                return;
            }
        }

        if (self->timer != 0) {
            self->timer--;
        } else {
            if (Actor_IsFacingPlayer(&self->actor, 0x1555)) {
                if ((self->actor.xzDistToPlayer < 220.0f) && (self->actor.xzDistToPlayer > 160.0f) &&
                    (Rand_ZeroOne() < 0.3f)) {
                    if (Actor_IsTargeted(globalCtx, &self->actor)) {
                        EnTest_SetupJumpslash(self);
                    } else {
                        func_808627C4(self, globalCtx);
                    }
                } else {
                    if (Rand_ZeroOne() > 0.3f) {
                        EnTest_SetupWalkAndBlock(self);
                    } else {
                        func_808627C4(self, globalCtx);
                    }
                }
            } else {
                if (Rand_ZeroOne() > 0.7f) {
                    func_80860BDC(self);
                } else {
                    EnTest_ChooseAction(self, globalCtx);
                }
            }
        }
    }
}

void EnTest_Fall(EnTest* self, GlobalContext* globalCtx) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, &gStalfosLandFromLeapAnim, 0.0f);
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.world.pos.y <= self->actor.floorHeight) {
        self->skelAnime.playSpeed = 1.0f;
        self->unk_7C8 = 0xC;
        self->timer = self->unk_7E4 * 0.15f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_DOWN);
        EnTest_SetupAction(self, EnTest_Land);
    }
}

void EnTest_Land(EnTest* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnTest_SetupIdle(self);
        self->timer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    }
}

void EnTest_SetupWalkAndBlock(EnTest* self) {
    Animation_Change(&self->upperSkelanime, &gStalfosBlockWithShieldAnim, 2.0f, 0.0f,
                     Animation_GetLastFrame(&gStalfosBlockWithShieldAnim), 2, 2.0f);
    Animation_PlayLoop(&self->skelAnime, &gStalfosSlowAdvanceAnim);
    self->timer = (s16)(Rand_ZeroOne() * 5.0f);
    self->unk_7C8 = 0xD;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnTest_SetupAction(self, EnTest_WalkAndBlock);
}

void EnTest_WalkAndBlock(EnTest* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 checkDist = 0.0f;
    s32 pad1;
    s32 prevFrame;
    s32 temp_f16;
    f32 playSpeed;
    Player* player = GET_PLAYER(globalCtx);
    f32 absPlaySpeed;
    s16 yawDiff;
    s32 temp_v0_2;

    if (!EnTest_ReactToProjectile(globalCtx, self)) {
        self->timer++;

        if (Actor_OtherIsTargeted(globalCtx, &self->actor)) {
            checkDist = 150.0f;
        }

        if (self->actor.xzDistToPlayer <= (80.0f + checkDist)) {
            Math_SmoothStepToF(&self->actor.speedXZ, -5.0f, 1.0f, 0.8f, 0.0f);
        } else if (self->actor.xzDistToPlayer > (110.0f + checkDist)) {
            Math_SmoothStepToF(&self->actor.speedXZ, 5.0f, 1.0f, 0.8f, 0.0f);
        }

        if (self->actor.speedXZ >= 5.0f) {
            self->actor.speedXZ = 5.0f;
        } else if (self->actor.speedXZ < -5.0f) {
            self->actor.speedXZ = -5.0f;
        }

        if ((self->actor.params == STALFOS_TYPE_CEILING) &&
            !Actor_TestFloorInDirection(&self->actor, globalCtx, self->actor.speedXZ, self->actor.world.rot.y)) {
            self->actor.speedXZ *= -1.0f;
        }

        if (ABS(self->actor.speedXZ) < 3.0f) {
            Animation_Change(&self->skelAnime, &gStalfosSlowAdvanceAnim, 0.0f, self->skelAnime.curFrame,
                             Animation_GetLastFrame(&gStalfosSlowAdvanceAnim), 0, -6.0f);
            playSpeed = self->actor.speedXZ * 10.0f;
        } else {
            Animation_Change(&self->skelAnime, &gStalfosFastAdvanceAnim, 0.0f, self->skelAnime.curFrame,
                             Animation_GetLastFrame(&gStalfosFastAdvanceAnim), 0, -4.0f);
            playSpeed = self->actor.speedXZ * 10.0f * 0.02f;
        }

        if (self->actor.speedXZ >= 0.0f) {
            if (self->unk_7DE == 0) {
                self->unk_7DE++;
            }

            playSpeed = CLAMP_MAX(playSpeed, 2.5f);
            self->skelAnime.playSpeed = playSpeed;
        } else {
            playSpeed = CLAMP_MIN(playSpeed, -2.5f);
            self->skelAnime.playSpeed = playSpeed;
        }

        yawDiff = player->actor.shape.rot.y - self->actor.shape.rot.y;

        if ((self->actor.xzDistToPlayer < 100.0f) && (player->swordState != 0)) {
            if (ABS(yawDiff) >= 0x1F40) {
                self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;

                if ((Rand_ZeroOne() > 0.7f) && (player->swordAnimation != 0x11)) {
                    EnTest_SetupJumpBack(self);
                } else {
                    EnTest_SetupStopAndBlock(self);
                }

                return;
            }
        }

        prevFrame = self->skelAnime.curFrame;
        SkelAnime_Update(&self->skelAnime);

        temp_f16 = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
        absPlaySpeed = ABS(self->skelAnime.playSpeed);

        if ((s32)self->skelAnime.curFrame != prevFrame) {
            temp_v0_2 = (s32)absPlaySpeed + prevFrame;

            if (((temp_v0_2 > 1) && (temp_f16 <= 0)) || ((temp_f16 < 7) && (temp_v0_2 >= 8))) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_WALK);
            }
        }

        if ((self->timer % 32) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_WARAU);
            self->timer += (s16)(Rand_ZeroOne() * 5.0f);
        }

        if ((self->actor.xzDistToPlayer < 220.0f) && (self->actor.xzDistToPlayer > 160.0f) &&
            (Actor_IsFacingPlayer(&self->actor, 0x71C))) {
            if (Actor_IsTargeted(globalCtx, &self->actor)) {
                if (Rand_ZeroOne() < 0.1f) {
                    EnTest_SetupJumpslash(self);
                    return;
                }
            } else if (player->heldItemActionParam != PLAYER_AP_NONE) {
                if (self->actor.isTargeted) {
                    if ((globalCtx->gameplayFrames % 2) != 0) {
                        func_808627C4(self, globalCtx);
                        return;
                    }

                    EnTest_ChooseAction(self, globalCtx);
                } else {
                    func_80860EC0(self);
                }
            }
        }

        if (Rand_ZeroOne() < 0.4f) {
            self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        }

        if (!Actor_IsFacingPlayer(&self->actor, 0x11C7)) {
            EnTest_SetupIdle(self);
            self->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
            return;
        }

        if (self->actor.xzDistToPlayer < 110.0f) {
            if (Rand_ZeroOne() > 0.2f) {
                if (player->stateFlags1 & 0x10) {
                    if (self->actor.isTargeted) {
                        EnTest_SetupSlashDown(self);
                    } else {
                        func_808627C4(self, globalCtx);
                    }
                } else {
                    EnTest_SetupSlashDown(self);
                }
            } else {
                EnTest_SetupStopAndBlock(self);
            }
        } else if (Rand_ZeroOne() < 0.1f) {
            self->actor.speedXZ = 5.0f;
        }
    }
}

// a variation of sidestep
void func_80860BDC(EnTest* self) {
    Animation_PlayLoop(&self->skelAnime, &gStalfosSidestepAnim);
    self->unk_7C8 = 0xE;
    EnTest_SetupAction(self, func_80860C24);
}

// a variation of sidestep
void func_80860C24(EnTest* self, GlobalContext* globalCtx) {
    s16 yawDiff;
    s16 yawChange;
    f32 playSpeed;
    s32 prevFrame;
    s32 temp1;
    s32 temp2;
    f32 absPlaySpeed;

    if (!EnTest_ReactToProjectile(globalCtx, self)) {
        yawDiff = self->actor.yawTowardsPlayer;
        yawDiff -= self->actor.shape.rot.y;

        if (yawDiff > 0) {
            yawChange = (yawDiff / 42.0f) + 300.0f;
            self->actor.shape.rot.y += yawChange * 2;
        } else {
            yawChange = (yawDiff / 42.0f) - 300.0f;
            self->actor.shape.rot.y += yawChange * 2;
        }

        self->actor.world.rot.y = self->actor.shape.rot.y;

        if (yawDiff > 0) {
            playSpeed = yawChange * 0.02f;
            playSpeed = CLAMP_MAX(playSpeed, 1.0f);
            self->skelAnime.playSpeed = playSpeed;
        } else {
            playSpeed = yawChange * 0.02f;
            playSpeed = CLAMP_MIN(playSpeed, -1.0f);
            self->skelAnime.playSpeed = playSpeed;
        }

        prevFrame = self->skelAnime.curFrame;

        SkelAnime_Update(&self->skelAnime);

        temp1 = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
        absPlaySpeed = ABS(self->skelAnime.playSpeed);

        if (1) {}

        if ((s32)self->skelAnime.curFrame != prevFrame) {
            temp2 = (s32)absPlaySpeed + prevFrame;

            if (((temp2 > 2) && (temp1 <= 0)) || ((temp1 < 7) && (temp2 >= 9))) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_WALK);
            }
        }

        if (Actor_IsFacingPlayer(&self->actor, 0x71C)) {
            if (Rand_ZeroOne() > 0.8f) {
                if ((Rand_ZeroOne() > 0.7f)) {
                    func_80860EC0(self);
                } else {
                    EnTest_ChooseAction(self, globalCtx);
                }
            } else {
                EnTest_SetupWalkAndBlock(self);
            }
        }
    }
}

// a variation of sidestep
void func_80860EC0(EnTest* self) {
    Animation_PlayLoop(&self->skelAnime, &gStalfosSidestepAnim);
    self->unk_7C8 = 0xF;
    self->actor.speedXZ = (Rand_ZeroOne() > 0.5f) ? -0.5f : 0.5f;
    self->timer = (s16)((Rand_ZeroOne() * 15.0f) + 25.0f);
    self->unk_7EC = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnTest_SetupAction(self, func_80860F84);
}

// a variation of sidestep
void func_80860F84(EnTest* self, GlobalContext* globalCtx) {
    s16 playerYaw180;
    s32 pad;
    s32 prevFrame;
    s32 temp_f16;
    s16 yawDiff;
    Player* player = GET_PLAYER(globalCtx);
    f32 checkDist = 0.0f;
    s16 newYaw;
    f32 absPlaySpeed;
    s32 temp_v0_2;

    if (!EnTest_ReactToProjectile(globalCtx, self)) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
        self->actor.world.rot.y = self->actor.shape.rot.y + 0x3E80;
        playerYaw180 = player->actor.shape.rot.y + 0x8000;

        if (self->actor.speedXZ >= 0.0f) {
            if (self->actor.speedXZ < 6.0f) {
                self->actor.speedXZ += 0.5f;
            } else {
                self->actor.speedXZ = 6.0f;
            }
        } else {
            if (self->actor.speedXZ > -6.0f) {
                self->actor.speedXZ -= 0.5f;
            } else {
                self->actor.speedXZ = -6.0f;
            }
        }

        if ((self->actor.bgCheckFlags & 8) ||
            ((self->actor.params == STALFOS_TYPE_CEILING) &&
             !Actor_TestFloorInDirection(&self->actor, globalCtx, self->actor.speedXZ, self->actor.world.rot.y))) {
            if (self->actor.bgCheckFlags & 8) {
                if (self->actor.speedXZ >= 0.0f) {
                    newYaw = self->actor.shape.rot.y + 0x3FFF;
                } else {
                    newYaw = self->actor.shape.rot.y - 0x3FFF;
                }

                newYaw = self->actor.wallYaw - newYaw;
            } else {
                self->actor.speedXZ *= -0.8f;
                newYaw = 0;
            }

            if (ABS(newYaw) > 0x4000) {
                self->actor.speedXZ *= -0.8f;

                if (self->actor.speedXZ < 0.0f) {
                    self->actor.speedXZ -= 0.5f;
                } else {
                    self->actor.speedXZ += 0.5f;
                }
            }
        }

        if (Actor_OtherIsTargeted(globalCtx, &self->actor)) {
            checkDist = 200.0f;
        }

        if (self->actor.xzDistToPlayer <= (80.0f + checkDist)) {
            Math_SmoothStepToF(&self->unk_7EC, -2.5f, 1.0f, 0.8f, 0.0f);
        } else if (self->actor.xzDistToPlayer > (110.0f + checkDist)) {
            Math_SmoothStepToF(&self->unk_7EC, 2.5f, 1.0f, 0.8f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->unk_7EC, 0.0f, 1.0f, 6.65f, 0.0f);
        }

        if (self->unk_7EC != 0.0f) {
            self->actor.world.pos.x += Math_SinS(self->actor.shape.rot.y) * self->unk_7EC;
            self->actor.world.pos.z += Math_CosS(self->actor.shape.rot.y) * self->unk_7EC;
        }

        self->skelAnime.playSpeed = self->actor.speedXZ * 0.5f;
        prevFrame = self->skelAnime.curFrame;
        SkelAnime_Update(&self->skelAnime);

        temp_f16 = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
        absPlaySpeed = ABS(self->skelAnime.playSpeed);

        if ((s32)self->skelAnime.curFrame != prevFrame) {
            temp_v0_2 = (s32)absPlaySpeed + prevFrame;

            if (((temp_v0_2 > 1) && (temp_f16 <= 0)) || ((temp_f16 < 7) && (temp_v0_2 >= 8))) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_WALK);
            }
        }

        if ((globalCtx->gameplayFrames & 95) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_WARAU);
        }

        yawDiff = playerYaw180 - self->actor.shape.rot.y;
        yawDiff = ABS(yawDiff);

        if ((yawDiff > 0x6800) || (self->timer == 0)) {
            EnTest_ChooseAction(self, globalCtx);
        } else if (self->timer != 0) {
            self->timer--;
        }
    }
}

void EnTest_SetupSlashDown(EnTest* self) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosDownSlashAnim);
    func_800F8A44(&self->actor.projectedPos, NA_SE_EN_STAL_WARAU);
    self->swordCollider.base.atFlags &= ~AT_BOUNCED;
    self->unk_7C8 = 0x10;
    self->actor.speedXZ = 0.0f;
    EnTest_SetupAction(self, EnTest_SlashDown);
    self->swordCollider.info.toucher.damage = 16;

    if (self->unk_7DE != 0) {
        self->unk_7DE = 3;
    }
}

void EnTest_SlashDown(EnTest* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 0.0f;

    if ((s32)self->skelAnime.curFrame < 4) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xBB8, 0);
    }

    if ((s32)self->skelAnime.curFrame == 7) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_SAKEBI);
    }

    if ((self->skelAnime.curFrame > 7.0f) && (self->skelAnime.curFrame < 11.0f)) {
        self->swordState = 1;
    } else {
        self->swordState = 0;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        if ((globalCtx->gameplayFrames % 2) != 0) {
            EnTest_SetupSlashDownEnd(self);
        } else {
            EnTest_SetupSlashUp(self);
        }
    }
}

void EnTest_SetupSlashDownEnd(EnTest* self) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosRecoverFromDownSlashAnim);
    self->unk_7C8 = 0x12;
    self->actor.speedXZ = 0.0f;
    EnTest_SetupAction(self, EnTest_SlashDownEnd);
}

void EnTest_SlashDownEnd(EnTest* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 yawDiff;

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->swordCollider.base.atFlags & AT_HIT) {
            self->swordCollider.base.atFlags &= ~AT_HIT;
            if (self->actor.params != STALFOS_TYPE_CEILING) {
                EnTest_SetupJumpBack(self);
                return;
            }
        }

        if (Rand_ZeroOne() > 0.7f) {
            EnTest_SetupIdle(self);
            self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
            return;
        }

        self->actor.world.rot.y = Actor_WorldYawTowardActor(&self->actor, &player->actor);

        if (Rand_ZeroOne() > 0.7f) {
            if (self->actor.params != STALFOS_TYPE_CEILING) {
                EnTest_SetupJumpBack(self);
                return;
            }
        }

        yawDiff = player->actor.shape.rot.y - self->actor.shape.rot.y;

        if (ABS(yawDiff) <= 0x2710) {
            yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

            if ((ABS(yawDiff) > 0x3E80) && (self->actor.params != STALFOS_TYPE_CEILING)) {
                self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                EnTest_SetupJumpBack(self);
            } else if (player->stateFlags1 & 0x10) {
                if (self->actor.isTargeted) {
                    EnTest_SetupSlashDown(self);
                } else if ((globalCtx->gameplayFrames % 2) != 0) {
                    func_808627C4(self, globalCtx);
                } else {
                    EnTest_SetupJumpBack(self);
                }
            } else {
                EnTest_SetupSlashDown(self);
            }
        } else {
            func_808627C4(self, globalCtx);
        }
    }
}

void EnTest_SetupSlashUp(EnTest* self) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosUpSlashAnim);
    self->swordCollider.base.atFlags &= ~AT_BOUNCED;
    self->unk_7C8 = 0x11;
    self->swordCollider.info.toucher.damage = 16;
    self->actor.speedXZ = 0.0f;
    EnTest_SetupAction(self, EnTest_SlashUp);

    if (self->unk_7DE != 0) {
        self->unk_7DE = 3;
    }
}

void EnTest_SlashUp(EnTest* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 0.0f;

    if ((s32)self->skelAnime.curFrame == 2) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_SAKEBI);
    }

    if ((self->skelAnime.curFrame > 1.0f) && (self->skelAnime.curFrame < 8.0f)) {
        self->swordState = 1;
    } else {
        self->swordState = 0;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        EnTest_SetupSlashDown(self);
    }
}

void EnTest_SetupJumpBack(EnTest* self) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosJumpBackwardsAnim);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    self->unk_7C8 = 0x14;
    self->timer = 5;
    EnTest_SetupAction(self, EnTest_JumpBack);

    if (self->unk_7DE != 0) {
        self->unk_7DE = 3;
    }

    if (self->actor.params != STALFOS_TYPE_CEILING) {
        self->actor.speedXZ = -11.0f;
    } else {
        self->actor.speedXZ = -7.0f;
    }
}

void EnTest_JumpBack(EnTest* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xBB8, 1);

    if (self->timer == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_WARAU);
    } else {
        self->timer--;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        if (!EnTest_ReactToProjectile(globalCtx, self)) {
            if (self->actor.xzDistToPlayer <= 100.0f) {
                if (Actor_IsFacingPlayer(&self->actor, 0x1555)) {
                    EnTest_SetupSlashDown(self);
                } else {
                    EnTest_SetupIdle(self);
                    self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
                }
            } else {
                if ((self->actor.xzDistToPlayer <= 220.0f) && Actor_IsFacingPlayer(&self->actor, 0xE38)) {
                    EnTest_SetupJumpslash(self);
                } else {
                    EnTest_SetupIdle(self);
                    self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
                }
            }
            self->actor.flags |= 1;
        }
    } else if (self->skelAnime.curFrame == (self->skelAnime.endFrame - 4.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
    }
}

void EnTest_SetupJumpslash(EnTest* self) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosJumpAnim);
    func_800F8A44(&self->actor.projectedPos, NA_SE_EN_STAL_WARAU);
    self->timer = 0;
    self->unk_7C8 = 0x17;
    self->actor.velocity.y = 10.0f;
    self->actor.speedXZ = 8.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->swordCollider.base.atFlags &= ~AT_BOUNCED;
    EnTest_SetupAction(self, EnTest_Jumpslash);
    self->swordCollider.info.toucher.damage = 32;

    if (self->unk_7DE != 0) {
        self->unk_7DE = 3;
    }
}

void EnTest_Jumpslash(EnTest* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->timer == 0) {
            Animation_PlayOnce(&self->skelAnime, &gStalfosJumpslashAnim);
            self->timer = 1;
            self->swordState = 1;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_SAKEBI);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
        } else {
            self->actor.speedXZ = 0.0f;
            EnTest_SetupIdle(self);
        }
    }

    if ((self->timer != 0) && (self->skelAnime.curFrame >= 5.0f)) {
        self->swordState = 0;
    }

    if (self->actor.world.pos.y <= self->actor.floorHeight) {
        if (self->actor.speedXZ != 0.0f) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        }

        self->actor.world.pos.y = self->actor.floorHeight;
        self->actor.velocity.y = 0.0f;
        self->actor.speedXZ = 0.0f;
    }
}

void EnTest_SetupJumpUp(EnTest* self) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosJumpAnim);
    self->timer = 0;
    self->unk_7C8 = 4;
    self->actor.velocity.y = 14.0f;
    self->actor.speedXZ = 6.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnTest_SetupAction(self, EnTest_JumpUp);
}

void EnTest_JumpUp(EnTest* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.world.pos.y <= self->actor.floorHeight) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->actor.world.pos.y = self->actor.floorHeight;
        self->unk_7E4 = -(s32)self->actor.velocity.y;

        if (self->unk_7E4 == 0) {
            self->unk_7E4 = 1;
        }

        self->actor.velocity.y = 0.0f;
        self->actor.speedXZ = 0.0f;
        self->unk_7C8 = 0xC;
        self->timer = 4;
        Animation_Change(&self->skelAnime, &gStalfosLandFromLeapAnim, 0.0f, 0.0f, 0.0f, 2, 0.0f);
        EnTest_SetupAction(self, EnTest_Land);
    }
}

void EnTest_SetupStopAndBlock(EnTest* self) {
    Animation_Change(&self->skelAnime, &gStalfosBlockWithShieldAnim, 2.0f, 0.0f,
                     Animation_GetLastFrame(&gStalfosBlockWithShieldAnim), 2, 2.0f);
    self->unk_7C8 = 0x15;
    self->actor.speedXZ = 0.0f;
    self->timer = (Rand_ZeroOne() * 10.0f) + 11.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->unk_7DE = 5;
    EnTest_SetupAction(self, EnTest_StopAndBlock);
}

void EnTest_StopAndBlock(EnTest* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    SkelAnime_Update(&self->skelAnime);

    if ((ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) > 0x3E80) &&
        (self->actor.params != STALFOS_TYPE_CEILING) && ((globalCtx->gameplayFrames % 2) != 0)) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        EnTest_SetupJumpBack(self);
    }

    if (self->timer == 0) {
        EnTest_SetupIdleFromBlock(self);
    } else {
        self->timer--;
    }
}

void EnTest_SetupIdleFromBlock(EnTest* self) {
    Animation_MorphToLoop(&self->skelAnime, &gStalfosMiddleGuardAnim, -4.0f);
    self->unk_7C8 = 0x16;
    EnTest_SetupAction(self, EnTest_IdleFromBlock);
}

void EnTest_IdleFromBlock(EnTest* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.5f, 0.0f);
    SkelAnime_Update(&self->skelAnime);

    if (self->skelAnime.morphWeight == 0.0f) {
        self->actor.speedXZ = 0.0f;
        self->unk_7DE = 0;

        if (!EnTest_ReactToProjectile(globalCtx, self)) {
            if (self->actor.xzDistToPlayer < 500.0f) {
                EnTest_ChooseAction(self, globalCtx);
            } else {
                func_808627C4(self, globalCtx);
            }
        }
    }
}

void func_80862154(EnTest* self) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosFlinchFromHitFrontAnim);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_DAMAGE);
    self->unk_7C8 = 8;
    self->actor.speedXZ = -2.0f;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
    EnTest_SetupAction(self, func_808621D4);
}

void func_808621D4(EnTest* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.1f, 0.0f);

    if (SkelAnime_Update(&self->skelAnime)) {
        self->actor.speedXZ = 0.0f;

        if ((self->actor.bgCheckFlags & 8) && ((ABS((s16)(self->actor.wallYaw - self->actor.shape.rot.y)) < 0x38A4) &&
                                               (self->actor.xzDistToPlayer < 80.0f))) {
            EnTest_SetupJumpUp(self);
        } else if (!EnTest_ReactToProjectile(globalCtx, self)) {
            EnTest_ChooseAction(self, globalCtx);
        } else {
            return;
        }
    }

    if (player->swordState != 0) {
        if ((self->actor.bgCheckFlags & 8) && ((ABS((s16)(self->actor.wallYaw - self->actor.shape.rot.y)) < 0x38A4) &&
                                               (self->actor.xzDistToPlayer < 80.0f))) {
            EnTest_SetupJumpUp(self);
        } else if ((Rand_ZeroOne() > 0.7f) && (self->actor.params != STALFOS_TYPE_CEILING) &&
                   (player->swordAnimation != 0x11)) {
            EnTest_SetupJumpBack(self);
        } else {
            EnTest_SetupStopAndBlock(self);
        }

        self->unk_7C8 = 8;
    }
}

void func_80862398(EnTest* self) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosFlinchFromHitBehindAnim);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_DAMAGE);
    self->unk_7C8 = 9;
    self->actor.speedXZ = -2.0f;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
    EnTest_SetupAction(self, func_80862418);
}

void func_80862418(EnTest* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.1f, 0.0f);

    if (SkelAnime_Update(&self->skelAnime)) {
        self->actor.speedXZ = 0.0f;

        if (!EnTest_ReactToProjectile(globalCtx, self)) {
            EnTest_ChooseAction(self, globalCtx);
        } else {
            return;
        }
    }

    if (player->swordState != 0) {
        if ((self->actor.bgCheckFlags & 8) && ((ABS((s16)(self->actor.wallYaw - self->actor.shape.rot.y)) < 0x38A4) &&
                                               (self->actor.xzDistToPlayer < 80.0f))) {
            EnTest_SetupJumpUp(self);
        } else if ((Rand_ZeroOne() > 0.7f) && (self->actor.params != STALFOS_TYPE_CEILING) &&
                   (player->swordAnimation != 0x11)) {
            EnTest_SetupJumpBack(self);
        } else {
            EnTest_SetupStopAndBlock(self);
        }

        self->unk_7C8 = 8;
    }
}

void EnTest_SetupStunned(EnTest* self) {
    self->unk_7C8 = 0xB;
    self->unk_7DE = 0;
    self->swordState = 0;
    self->skelAnime.playSpeed = 0.0f;
    self->actor.speedXZ = -4.0f;

    if (self->lastDamageEffect == STALFOS_DMGEFF_LIGHT) {
        Actor_SetColorFilter(&self->actor, -0x8000, 0x78, 0, 0x50);
    } else {
        Actor_SetColorFilter(&self->actor, 0, 0x78, 0, 0x50);

        if (self->lastDamageEffect == STALFOS_DMGEFF_FREEZE) {
            self->iceTimer = 36;
        } else {
            Animation_PlayOnceSetSpeed(&self->skelAnime, &gStalfosFlinchFromHitFrontAnim, 0.0f);
        }
    }

    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnTest_SetupAction(self, EnTest_Stunned);
}

void EnTest_Stunned(EnTest* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);

    if (self->actor.colorFilterTimer == 0) {
        if (self->actor.colChkInfo.health == 0) {
            func_80862FA8(self, globalCtx);
        } else if (player->swordState != 0) {
            if ((self->actor.bgCheckFlags & 8) &&
                ((ABS((s16)(self->actor.wallYaw - self->actor.shape.rot.y)) < 0x38A4) &&
                 (self->actor.xzDistToPlayer < 80.0f))) {
                EnTest_SetupJumpUp(self);
            } else if ((Rand_ZeroOne() > 0.7f) && (player->swordAnimation != 0x11)) {
                EnTest_SetupJumpBack(self);
            } else {
                EnTest_SetupStopAndBlock(self);
            }

            self->unk_7C8 = 8;
        } else {
            self->actor.speedXZ = 0.0f;
            if (!EnTest_ReactToProjectile(globalCtx, self)) {
                EnTest_ChooseAction(self, globalCtx);
            }
        }
    }
}

// a variation of sidestep
void func_808627C4(EnTest* self, GlobalContext* globalCtx) {
    if (Actor_OtherIsTargeted(globalCtx, &self->actor)) {
        func_80860EC0(self);
        return;
    }

    Animation_MorphToLoop(&self->skelAnime, &gStalfosSidestepAnim, -2.0f);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    self->actor.speedXZ = ((globalCtx->gameplayFrames % 2) != 0) ? -4.0f : 4.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
    self->timer = (Rand_ZeroOne() * 20.0f) + 20.0f;
    self->unk_7C8 = 0x18;
    EnTest_SetupAction(self, func_808628C8);
    self->unk_7EC = 0.0f;
}

// a variation of sidestep
void func_808628C8(EnTest* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad1;
    s32 prevFrame;
    s32 temp_f16;
    s32 pad2;
    f32 checkDist = 0.0f;
    s16 newYaw;
    f32 absPlaySpeed;
    s32 temp_v0_2;

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);

    if (self->unk_7DE == 0) {
        self->unk_7DE++;
    }

    if (self->actor.speedXZ >= 0.0f) {
        if (self->actor.speedXZ < 6.0f) {
            self->actor.speedXZ += 0.125f;
        } else {
            self->actor.speedXZ = 6.0f;
        }
    } else {
        if (self->actor.speedXZ > -6.0f) {
            self->actor.speedXZ -= 0.125f;
        } else {
            self->actor.speedXZ = -6.0f;
        }
    }

    if ((self->actor.bgCheckFlags & 8) ||
        ((self->actor.params == STALFOS_TYPE_CEILING) &&
         !Actor_TestFloorInDirection(&self->actor, globalCtx, self->actor.speedXZ, self->actor.shape.rot.y + 0x3FFF))) {
        if (self->actor.bgCheckFlags & 8) {
            if (self->actor.speedXZ >= 0.0f) {
                newYaw = (self->actor.shape.rot.y + 0x3FFF);
            } else {
                newYaw = (self->actor.shape.rot.y - 0x3FFF);
            }

            newYaw = self->actor.wallYaw - newYaw;
        } else {
            self->actor.speedXZ *= -0.8f;
            newYaw = 0;
        }

        if (ABS(newYaw) > 0x4000) {
            self->actor.speedXZ *= -0.8f;

            if (self->actor.speedXZ < 0.0f) {
                self->actor.speedXZ -= 0.5f;
            } else {
                self->actor.speedXZ += 0.5f;
            }
        }
    }

    self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;

    if (Actor_OtherIsTargeted(globalCtx, &self->actor)) {
        checkDist = 200.0f;
    }

    if (self->actor.xzDistToPlayer <= (80.0f + checkDist)) {
        Math_SmoothStepToF(&self->unk_7EC, -2.5f, 1.0f, 0.8f, 0.0f);
    } else if (self->actor.xzDistToPlayer > (110.0f + checkDist)) {
        Math_SmoothStepToF(&self->unk_7EC, 2.5f, 1.0f, 0.8f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->unk_7EC, 0.0f, 1.0f, 6.65f, 0.0f);
    }

    if (self->unk_7EC != 0.0f) {
        self->actor.world.pos.x += (Math_SinS(self->actor.shape.rot.y) * self->unk_7EC);
        self->actor.world.pos.z += (Math_CosS(self->actor.shape.rot.y) * self->unk_7EC);
    }

    self->skelAnime.playSpeed = self->actor.speedXZ * 0.5f;
    prevFrame = self->skelAnime.curFrame;
    SkelAnime_Update(&self->skelAnime);

    temp_f16 = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
    absPlaySpeed = ABS(self->skelAnime.playSpeed);

    if ((self->timer % 32) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_WARAU);
    }
    if ((s32)self->skelAnime.curFrame != prevFrame) {
        temp_v0_2 = (s32)absPlaySpeed + prevFrame;

        if (((temp_v0_2 > 1) && (temp_f16 <= 0)) || ((temp_f16 < 7) && (temp_v0_2 >= 8))) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_WALK);
        }
    }

    if (self->timer == 0) {
        if (Actor_OtherIsTargeted(globalCtx, &self->actor)) {
            EnTest_SetupIdle(self);
        } else if (Actor_IsTargeted(globalCtx, &self->actor)) {
            if (!EnTest_ReactToProjectile(globalCtx, self)) {
                EnTest_ChooseAction(self, globalCtx);
            }
        } else if (player->heldItemActionParam != PLAYER_AP_NONE) {
            if ((globalCtx->gameplayFrames % 2) != 0) {
                EnTest_SetupIdle(self);
            } else {
                EnTest_SetupWalkAndBlock(self);
            }
        } else {
            EnTest_SetupWalkAndBlock(self);
        }

    } else {
        self->timer--;
    }
}

void func_80862DBC(EnTest* self, GlobalContext* globalCtx) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_DAMAGE);
    self->unk_7C8 = 2;
    BodyBreak_Alloc(&self->bodyBreak, 60, globalCtx);
    self->actor.home.rot.x = 0;

    if (self->swordState >= 0) {
        EffectBlure_AddSpace(Effect_GetByIndex(self->effectIndex));
        self->swordState = -1;
    }

    self->actor.flags &= ~1;

    if (self->actor.params == STALFOS_TYPE_5) {
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
    }

    EnTest_SetupAction(self, func_80862E6C);
}

void func_80862E6C(EnTest* self, GlobalContext* globalCtx) {
    if (self->actor.child == NULL) {
        if (self->actor.home.rot.x == 0) {
            self->actor.home.rot.x = self->bodyBreak.count;
        }

        if (BodyBreak_SpawnParts(&self->actor, &self->bodyBreak, globalCtx, self->actor.params + 8)) {
            self->actor.child = &self->actor;
        }
    } else {
        if (self->actor.home.rot.x == 0) {
            self->actor.colChkInfo.health = 10;

            if (self->actor.params == STALFOS_TYPE_4) {
                self->actor.params = -1;
            } else {
                Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_ENEMY);
            }

            self->actor.child = NULL;
            self->actor.flags |= 1;
            EnTest_SetupJumpBack(self);
        } else if ((self->actor.params == STALFOS_TYPE_5) &&
                   !Actor_FindNearby(globalCtx, &self->actor, ACTOR_EN_TEST, ACTORCAT_ENEMY, 8000.0f)) {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xD0);

            if (self->actor.parent != NULL) {
                self->actor.parent->home.rot.z--;
            }

            Actor_Kill(&self->actor);
        }
    }
}

void func_80862FA8(EnTest* self, GlobalContext* globalCtx) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosFallOverBackwardsAnim);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_DEAD);
    self->unk_7DE = 0;
    self->actor.flags &= ~1;
    self->actor.colorFilterTimer = 0;
    self->actor.speedXZ = 0.0f;

    if (self->actor.params <= STALFOS_TYPE_CEILING) {
        self->unk_7C8 = 5;
        EnTest_SetupAction(self, func_80863044);
    } else {
        func_80862DBC(self, globalCtx);
    }
}

void func_80863044(EnTest* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
        self->unk_7C8 = 7;
        EnTest_SetupAction(self, func_808633E8);
        BodyBreak_Alloc(&self->bodyBreak, 60, globalCtx);
    }

    if ((s32)self->skelAnime.curFrame == 15) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_DOWN);
    }
}

void func_808630F0(EnTest* self, GlobalContext* globalCtx) {
    Animation_PlayOnce(&self->skelAnime, &gStalfosFallOverForwardsAnim);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_DEAD);
    self->unk_7C8 = 6;
    self->actor.colorFilterTimer = 0;
    self->unk_7DE = 0;
    self->actor.speedXZ = 0.0f;

    if (self->actor.params <= STALFOS_TYPE_CEILING) {
        self->actor.flags &= ~1;
        EnTest_SetupAction(self, func_8086318C);
    } else {
        func_80862DBC(self, globalCtx);
    }
}

void func_8086318C(EnTest* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->timer = (Rand_ZeroOne() * 10.0f) + 10.0f;
        self->unk_7C8 = 7;
        EnTest_SetupAction(self, func_808633E8);
        BodyBreak_Alloc(&self->bodyBreak, 60, globalCtx);
    }

    if (((s32)self->skelAnime.curFrame == 10) || ((s32)self->skelAnime.curFrame == 25)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_DOWN);
    }
}

void EnTest_SetupRecoil(EnTest* self) {
    self->swordState = 0;
    self->skelAnime.moveFlags = 2;
    self->unk_7C8 = 0x13;
    self->skelAnime.playSpeed = -1.0f;
    self->skelAnime.startFrame = self->skelAnime.curFrame;
    self->skelAnime.endFrame = 0.0f;
    EnTest_SetupAction(self, EnTest_Recoil);
}

void EnTest_Recoil(EnTest* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if (Rand_ZeroOne() > 0.7f) {
            EnTest_SetupIdle(self);
            self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
        } else if (((globalCtx->gameplayFrames % 2) != 0) && (self->actor.params != STALFOS_TYPE_CEILING)) {
            EnTest_SetupJumpBack(self);
        } else {
            func_808627C4(self, globalCtx);
        }
    }
}

void EnTest_Rise(EnTest* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.scale.y < 0.015f) {
        self->actor.scale.y += 0.002f;
        self->actor.world.pos.y = self->actor.home.pos.y - 3.5f;
    } else {
        self->actor.world.pos.y = self->actor.home.pos.y;
        EnTest_SetupJumpBack(self);
    }
}

void func_808633E8(EnTest* self, GlobalContext* globalCtx) {
    self->actor.params = STALFOS_TYPE_1;

    if (BodyBreak_SpawnParts(&self->actor, &self->bodyBreak, globalCtx, self->actor.params)) {
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xD0);

        if (self->actor.parent != NULL) {
            self->actor.parent->home.rot.z--;
        }

        Actor_Kill(&self->actor);
    }
}

void EnTest_UpdateHeadRot(EnTest* self, GlobalContext* globalCtx) {
    s16 lookAngle = self->actor.yawTowardsPlayer;

    lookAngle -= (s16)(self->headRot.y + self->actor.shape.rot.y);

    self->headRotOffset.y = CLAMP(lookAngle, -0x7D0, 0x7D0);
    self->headRot.y += self->headRotOffset.y;
    self->headRot.y = CLAMP(self->headRot.y, -0x382F, 0x382F);
}

void EnTest_UpdateDamage(EnTest* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->shieldCollider.base.acFlags & AC_BOUNCED) {
        self->shieldCollider.base.acFlags &= ~AC_BOUNCED;
        self->bodyCollider.base.acFlags &= ~AC_HIT;

        if (self->unk_7C8 >= 0xA) {
            self->actor.speedXZ = -4.0f;
        }
    } else if (self->bodyCollider.base.acFlags & AC_HIT) {
        self->bodyCollider.base.acFlags &= ~AC_HIT;

        if ((self->actor.colChkInfo.damageEffect != STALFOS_DMGEFF_SLING) &&
            (self->actor.colChkInfo.damageEffect != STALFOS_DMGEFF_FIREMAGIC)) {
            self->lastDamageEffect = self->actor.colChkInfo.damageEffect;
            if (self->swordState >= 1) {
                self->swordState = 0;
            }
            self->unk_7DC = player->unk_845;
            self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            Actor_SetDropFlag(&self->actor, &self->bodyCollider.info, false);
            func_800F8A44(&self->actor.projectedPos, NA_SE_EN_STAL_WARAU);

            if ((self->actor.colChkInfo.damageEffect == STALFOS_DMGEFF_STUN) ||
                (self->actor.colChkInfo.damageEffect == STALFOS_DMGEFF_FREEZE) ||
                (self->actor.colChkInfo.damageEffect == STALFOS_DMGEFF_LIGHT)) {
                if (self->unk_7C8 != 0xB) {
                    Actor_ApplyDamage(&self->actor);
                    EnTest_SetupStunned(self);
                }
            } else {
                if (Actor_IsFacingPlayer(&self->actor, 0x4000)) {
                    if (Actor_ApplyDamage(&self->actor) == 0) {
                        Enemy_StartFinishingBlow(globalCtx, &self->actor);
                        func_80862FA8(self, globalCtx);
                    } else {
                        func_80862154(self);
                    }
                } else if (Actor_ApplyDamage(&self->actor) == 0) {
                    func_808630F0(self, globalCtx);
                    Enemy_StartFinishingBlow(globalCtx, &self->actor);
                } else {
                    func_80862398(self);
                }
            }
        }
    }
}

void EnTest_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnTest* self = THIS;
    f32 oldWeight;
    u32 floorProperty;
    s32 pad;

    EnTest_UpdateDamage(self, globalCtx);

    if (self->actor.colChkInfo.damageEffect != STALFOS_DMGEFF_FIREMAGIC) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 75.0f, 30.0f, 30.0f, 0x1D);

        if (self->actor.params == STALFOS_TYPE_1) {
            if (self->actor.world.pos.y <= self->actor.home.pos.y) {
                self->actor.world.pos.y = self->actor.home.pos.y;
                self->actor.velocity.y = 0.0f;
            }

            if (self->actor.floorHeight <= self->actor.home.pos.y) {
                self->actor.floorHeight = self->actor.home.pos.y;
            }
        } else if (self->actor.bgCheckFlags & 2) {
            floorProperty = func_80041EA4(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);

            if ((floorProperty == 5) || (floorProperty == 0xC) ||
                func_80041D4C(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId) == 9) {
                Actor_Kill(&self->actor);
                return;
            }
        }

        self->actionFunc(self, globalCtx);

        switch (self->unk_7DE) {
            case 0:
                break;

            case 1:
                Animation_Change(&self->upperSkelanime, &gStalfosBlockWithShieldAnim, 2.0f, 0.0f,
                                 Animation_GetLastFrame(&gStalfosBlockWithShieldAnim), 2, 2.0f);
                AnimationContext_SetCopyTrue(globalCtx, self->skelAnime.limbCount, self->skelAnime.jointTable,
                                             self->upperSkelanime.jointTable, sJointCopyFlags);
                self->unk_7DE++;
                break;

            case 2:
                SkelAnime_Update(&self->upperSkelanime);
                SkelAnime_CopyFrameTableTrue(&self->skelAnime, self->skelAnime.jointTable,
                                             self->upperSkelanime.jointTable, sJointCopyFlags);
                break;

            case 3:
                self->unk_7DE++;
                self->upperSkelanime.morphWeight = 4.0f;
                // fallthrough
            case 4:
                oldWeight = self->upperSkelanime.morphWeight;
                self->upperSkelanime.morphWeight -= 1.0f;

                if (self->upperSkelanime.morphWeight <= 0.0f) {
                    self->unk_7DE = 0;
                }

                SkelAnime_InterpFrameTable(self->skelAnime.limbCount, self->upperSkelanime.jointTable,
                                           self->upperSkelanime.jointTable, self->skelAnime.jointTable,
                                           1.0f - (self->upperSkelanime.morphWeight / oldWeight));
                SkelAnime_CopyFrameTableTrue(&self->skelAnime, self->skelAnime.jointTable,
                                             self->upperSkelanime.jointTable, sJointCopyFlags);
                break;
        }

        if ((self->actor.colorFilterTimer == 0) && (self->actor.colChkInfo.health != 0)) {
            if ((self->unk_7C8 != 0x10) && (self->unk_7C8 != 0x17)) {
                EnTest_UpdateHeadRot(self, globalCtx);
            } else {
                Math_SmoothStepToS(&self->headRot.y, 0, 1, 0x3E8, 0);
            }
        }
    }

    Collider_UpdateCylinder(&self->actor, &self->bodyCollider);

    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 45.0f;

    if ((self->actor.colChkInfo.health > 0) || (self->actor.colorFilterTimer != 0)) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);

        if ((self->unk_7C8 >= 0xA) &&
            ((self->actor.colorFilterTimer == 0) || (!(self->actor.colorFilterParams & 0x4000)))) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
        }

        if (self->unk_7DE != 0) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->shieldCollider.base);
        }
    }

    if (self->swordState >= 1) {
        if (!(self->swordCollider.base.atFlags & AT_BOUNCED)) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->swordCollider.base);
        } else {
            self->swordCollider.base.atFlags &= ~AT_BOUNCED;
            EnTest_SetupRecoil(self);
        }
    }

    if (self->actor.params == STALFOS_TYPE_INVISIBLE) {
        if (globalCtx->actorCtx.unk_03 != 0) {
            self->actor.flags |= 0x81;
            self->actor.shape.shadowDraw = ActorShadow_DrawFeet;
        } else {
            self->actor.flags &= ~0x81;
            self->actor.shape.shadowDraw = NULL;
        }
    }
}

s32 EnTest_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnTest* self = THIS;
    s32 pad;

    if (limbIndex == STALFOS_LIMB_HEAD_ROOT) {
        rot->x += self->headRot.y;
        rot->y -= self->headRot.x;
        rot->z += self->headRot.z;
    } else if (limbIndex == STALFOS_LIMB_HEAD) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_test.c", 3582);

        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, 80 + ABS((s16)(Math_SinS(globalCtx->gameplayFrames * 2000) * 175.0f)), 0, 0,
                       255);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_test.c", 3587);
    }

    if ((self->actor.params == STALFOS_TYPE_INVISIBLE) && ((self->actor.flags & 0x80) != 0x80)) {
        *dList = NULL;
    }

    return false;
}

void EnTest_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f unused1 = { 1100.0f, -700.0f, 0.0f };
    static Vec3f D_80864658 = { 300.0f, 0.0f, 0.0f };
    static Vec3f D_80864664 = { 3400.0f, 0.0f, 0.0f };
    static Vec3f D_80864670 = { 0.0f, 0.0f, 0.0f };
    static Vec3f D_8086467C = { 7000.0f, 1000.0f, 0.0f };
    static Vec3f D_80864688 = { 3000.0f, -2000.0f, -1000.0f };
    static Vec3f D_80864694 = { 3000.0f, -2000.0f, 1000.0f };
    static Vec3f D_808646A0 = { -1300.0f, 1100.0f, 0.0f };
    static Vec3f unused2 = { -3000.0f, 1900.0f, 800.0f };
    static Vec3f unused3 = { -3000.0f, -1100.0f, 800.0f };
    static Vec3f unused4 = { 1900.0f, 1900.0f, 800.0f };
    static Vec3f unused5 = { -3000.0f, -1100.0f, 800.0f };
    static Vec3f unused6 = { 1900.0f, -1100.0f, 800.0f };
    static Vec3f unused7 = { 1900.0f, 1900.0f, 800.0f };
    s32 bodyPart = -1;
    Vec3f sp70;
    Vec3f sp64;
    EnTest* self = THIS;
    s32 pad;
    Vec3f sp50;

    BodyBreak_SetInfo(&self->bodyBreak, limbIndex, 0, 60, 60, dList, BODYBREAK_OBJECT_DEFAULT);

    if (limbIndex == STALFOS_LIMB_SWORD) {
        Matrix_MultVec3f(&D_8086467C, &self->swordCollider.dim.quad[1]);
        Matrix_MultVec3f(&D_80864688, &self->swordCollider.dim.quad[0]);
        Matrix_MultVec3f(&D_80864694, &self->swordCollider.dim.quad[3]);
        Matrix_MultVec3f(&D_808646A0, &self->swordCollider.dim.quad[2]);

        Collider_SetQuadVertices(&self->swordCollider, &self->swordCollider.dim.quad[0],
                                 &self->swordCollider.dim.quad[1], &self->swordCollider.dim.quad[2],
                                 &self->swordCollider.dim.quad[3]);

        Matrix_MultVec3f(&D_80864664, &sp70);
        Matrix_MultVec3f(&D_80864670, &sp64);

        if ((self->swordState >= 1) &&
            ((self->actor.params != STALFOS_TYPE_INVISIBLE) || (globalCtx->actorCtx.unk_03 != 0))) {
            EffectBlure_AddVertex(Effect_GetByIndex(self->effectIndex), &sp70, &sp64);
        } else if (self->swordState >= 0) {
            EffectBlure_AddSpace(Effect_GetByIndex(self->effectIndex));
            self->swordState = -1;
        }

    } else if ((limbIndex == STALFOS_LIMB_SHIELD) && (self->unk_7DE != 0)) {
        Matrix_MultVec3f(&D_80864670, &sp64);

        self->shieldCollider.dim.pos.x = sp64.x;
        self->shieldCollider.dim.pos.y = sp64.y;
        self->shieldCollider.dim.pos.z = sp64.z;
    } else {
        Actor_SetFeetPos(&self->actor, limbIndex, STALFOS_LIMB_FOOT_L, &D_80864658, STALFOS_LIMB_ANKLE_R, &D_80864658);

        if ((limbIndex == STALFOS_LIMB_FOOT_L) || (limbIndex == STALFOS_LIMB_ANKLE_R)) {
            if ((self->unk_7C8 == 0x15) || (self->unk_7C8 == 0x16)) {
                if (self->actor.speedXZ != 0.0f) {
                    Matrix_MultVec3f(&D_80864658, &sp64);
                    Actor_SpawnFloorDustRing(globalCtx, &self->actor, &sp64, 10.0f, 1, 8.0f, 0x64, 0xF, 0);
                }
            }
        }
    }

    if (self->iceTimer != 0) {
        switch (limbIndex) {
            case STALFOS_LIMB_HEAD:
                bodyPart = 0;
                break;
            case STALFOS_LIMB_CHEST:
                bodyPart = 1;
                break;
            case STALFOS_LIMB_SWORD:
                bodyPart = 2;
                break;
            case STALFOS_LIMB_SHIELD:
                bodyPart = 3;
                break;
            case STALFOS_LIMB_UPPERARM_R:
                bodyPart = 4;
                break;
            case STALFOS_LIMB_UPPERARM_L:
                bodyPart = 5;
                break;
            case STALFOS_LIMB_WAIST:
                bodyPart = 6;
                break;
            case STALFOS_LIMB_FOOT_L:
                bodyPart = 7;
                break;
            case STALFOS_LIMB_FOOT_R:
                bodyPart = 8;
                break;
        }

        if (bodyPart >= 0) {
            Matrix_MultVec3f(&D_80864670, &sp50);

            self->bodyPartsPos[bodyPart].x = sp50.x;
            self->bodyPartsPos[bodyPart].y = sp50.y;
            self->bodyPartsPos[bodyPart].z = sp50.z;
        }
    }
}

void EnTest_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnTest* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    func_8002EBCC(&self->actor, globalCtx, 1);

    if ((thisx->params <= STALFOS_TYPE_CEILING) || (thisx->child == NULL)) {
        SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnTest_OverrideLimbDraw,
                          EnTest_PostLimbDraw, self);
    }

    if (self->iceTimer != 0) {
        thisx->colorFilterTimer++;
        self->iceTimer--;

        if ((self->iceTimer % 4) == 0) {
            s32 iceIndex = self->iceTimer >> 2;

            EffectSsEnIce_SpawnFlyingVec3s(globalCtx, thisx, &self->bodyPartsPos[iceIndex], 150, 150, 150, 250, 235,
                                           245, 255, 1.5f);
        }
    }
}

// a variation of sidestep
void func_80864158(EnTest* self, f32 xzSpeed) {
    Animation_MorphToLoop(&self->skelAnime, &gStalfosSidestepAnim, -2.0f);
    self->actor.speedXZ = xzSpeed;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
    self->timer = (Rand_ZeroOne() * 20.0f) + 15.0f;
    self->unk_7C8 = 0x18;
    EnTest_SetupAction(self, func_808628C8);
}

/**
 * Check if a projectile actor is within 300 units and react accordingly.
 * Returns true if the projectile test passes and a new action is performed.
 */
s32 EnTest_ReactToProjectile(GlobalContext* globalCtx, EnTest* self) {
    Actor* projectileActor;
    s16 yawToProjectile;
    s16 wallYawDiff;
    s16 touchingWall;
    s16 directionFlag;

    projectileActor = Actor_GetProjectileActor(globalCtx, &self->actor, 300.0f);

    if (projectileActor != NULL) {
        yawToProjectile = Actor_WorldYawTowardActor(&self->actor, projectileActor) - (u16)self->actor.shape.rot.y;

        if ((u8)(self->actor.bgCheckFlags & 8)) {
            wallYawDiff = ((u16)self->actor.wallYaw - (u16)self->actor.shape.rot.y);
            touchingWall = true;
        } else {
            touchingWall = false;
        }

        if (Math_Vec3f_DistXYZ(&self->actor.world.pos, &projectileActor->world.pos) < 200.0f) {
            if (Actor_IsTargeted(globalCtx, &self->actor) && (projectileActor->id == ACTOR_ARMS_HOOK)) {
                EnTest_SetupJumpUp(self);
            } else if (ABS(yawToProjectile) < 0x2000) {
                EnTest_SetupStopAndBlock(self);
            } else if (ABS(yawToProjectile) < 0x6000) {
                EnTest_SetupJumpBack(self);
            } else {
                EnTest_SetupJumpUp(self);
            }

            return true;
        }

        if (Actor_IsTargeted(globalCtx, &self->actor) && (projectileActor->id == ACTOR_ARMS_HOOK)) {
            EnTest_SetupJumpUp(self);
            return true;
        }

        if ((ABS(yawToProjectile) < 0x2000) || (ABS(yawToProjectile) > 0x6000)) {
            directionFlag = globalCtx->gameplayFrames % 2;

            if (touchingWall && (wallYawDiff > 0x2000) && (wallYawDiff < 0x6000)) {
                directionFlag = false;
            } else if (touchingWall && (wallYawDiff < -0x2000) && (wallYawDiff > -0x6000)) {
                directionFlag = true;
            }

            if (directionFlag) {
                func_80864158(self, 4.0f);
            } else {
                func_80864158(self, -4.0f);
            }
        } else if (ABS(yawToProjectile) < 0x6000) {
            directionFlag = globalCtx->gameplayFrames % 2;

            if (touchingWall && (ABS(wallYawDiff) > 0x6000)) {
                directionFlag = false;
            } else if (touchingWall && (ABS(wallYawDiff) < 0x2000)) {
                directionFlag = true;
            }

            if (directionFlag) {
                EnTest_SetupJumpBack(self);
            } else {
                EnTest_SetupJumpUp(self);
            }
        }

        return true;
    }

    return false;
}
