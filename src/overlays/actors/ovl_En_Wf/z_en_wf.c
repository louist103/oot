/*
 * File: z_en_wf.c
 * Overlay: ovl_En_Wf
 * Description: Wolfos (Normal and White)
 */

#include "z_en_wf.h"
#include "vt.h"
#include "overlays/actors/ovl_En_Encount1/z_en_encount1.h"
#include "objects/object_wf/object_wf.h"

#define FLAGS 0x00000015

#define THIS ((EnWf*)thisx)

void EnWf_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWf_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWf_Update(Actor* thisx, GlobalContext* globalCtx);
void EnWf_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnWf_SetupWaitToAppear(EnWf* self);
void EnWf_WaitToAppear(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupWait(EnWf* self);
void EnWf_Wait(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupRunAtPlayer(EnWf* self, GlobalContext* globalCtx);
void EnWf_RunAtPlayer(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupSearchForPlayer(EnWf* self);
void EnWf_SearchForPlayer(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupRunAroundPlayer(EnWf* self);
void EnWf_RunAroundPlayer(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupSlash(EnWf* self);
void EnWf_Slash(EnWf* self, GlobalContext* globalCtx);
void EnWf_RecoilFromBlockedSlash(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupBackflipAway(EnWf* self);
void EnWf_BackflipAway(EnWf* self, GlobalContext* globalCtx);
void EnWf_Stunned(EnWf* self, GlobalContext* globalCtx);
void EnWf_Damaged(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupSomersaultAndAttack(EnWf* self);
void EnWf_SomersaultAndAttack(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupBlocking(EnWf* self);
void EnWf_Blocking(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupSidestep(EnWf* self, GlobalContext* globalCtx);
void EnWf_Sidestep(EnWf* self, GlobalContext* globalCtx);
void EnWf_SetupDie(EnWf* self);
void EnWf_Die(EnWf* self, GlobalContext* globalCtx);
s32 EnWf_DodgeRanged(GlobalContext* globalCtx, EnWf* self);

static ColliderJntSphElementInit sJntSphItemsInit[4] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { WOLFOS_LIMB_FRONT_RIGHT_CLAW, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { WOLFOS_LIMB_FRONT_LEFT_CLAW, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFC1FFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { WOLFOS_LIMB_HEAD, { { 800, 0, 0 }, 25 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK1,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFC1FFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { WOLFOS_LIMB_THORAX, { { 0, 0, 0 }, 30 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphItemsInit),
    sJntSphItemsInit,
};

static ColliderCylinderInit sBodyCylinderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 20, 50, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sTailCylinderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 15, 20, -15, { 0, 0, 0 } },
};

typedef enum {
    /*  0 */ ENWF_DMGEFF_NONE,
    /*  1 */ ENWF_DMGEFF_STUN,
    /*  6 */ ENWF_DMGEFF_ICE_MAGIC = 6,
    /* 13 */ ENWF_DMGEFF_LIGHT_MAGIC = 13,
    /* 14 */ ENWF_DMGEFF_FIRE,
    /* 15 */ ENWF_DMGEFF_UNDEF // used like STUN in the code, but not in the table
} EnWfDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, ENWF_DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(0, ENWF_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Hookshot      */ DMG_ENTRY(0, ENWF_DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Master sword  */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Giant's Knife */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Fire arrow    */ DMG_ENTRY(4, ENWF_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Light arrow   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(4, ENWF_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(0, ENWF_DMGEFF_ICE_MAGIC),
    /* Light magic   */ DMG_ENTRY(3, ENWF_DMGEFF_LIGHT_MAGIC),
    /* Shield        */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, ENWF_DMGEFF_NONE),
    /* Giant spin    */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Master spin   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Kokiri jump   */ DMG_ENTRY(2, ENWF_DMGEFF_NONE),
    /* Giant jump    */ DMG_ENTRY(8, ENWF_DMGEFF_NONE),
    /* Master jump   */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Unknown 1     */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, ENWF_DMGEFF_NONE),
    /* Unknown 2     */ DMG_ENTRY(0, ENWF_DMGEFF_NONE),
};

const ActorInit En_Wf_InitVars = {
    ACTOR_EN_WF,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_WF,
    sizeof(EnWf),
    (ActorFunc)EnWf_Init,
    (ActorFunc)EnWf_Destroy,
    (ActorFunc)EnWf_Update,
    (ActorFunc)EnWf_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3000, ICHAIN_STOP),
};

void EnWf_SetupAction(EnWf* self, EnWfActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnWf_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnWf* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    thisx->colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    thisx->focus.pos = thisx->world.pos;
    thisx->colChkInfo.mass = MASS_HEAVY;
    thisx->colChkInfo.health = 8;
    thisx->colChkInfo.cylRadius = 50;
    thisx->colChkInfo.cylHeight = 100;
    self->switchFlag = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    self->eyeIndex = 0;
    self->unk_2F4 = 10.0f; // Set and not used

    Collider_InitJntSph(globalCtx, &self->colliderSpheres);
    Collider_SetJntSph(globalCtx, &self->colliderSpheres, thisx, &sJntSphInit, self->colliderSpheresElements);
    Collider_InitCylinder(globalCtx, &self->colliderCylinderBody);
    Collider_SetCylinder(globalCtx, &self->colliderCylinderBody, thisx, &sBodyCylinderInit);
    Collider_InitCylinder(globalCtx, &self->colliderCylinderTail);
    Collider_SetCylinder(globalCtx, &self->colliderCylinderTail, thisx, &sTailCylinderInit);

    if (thisx->params == WOLFOS_NORMAL) {
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gWolfosNormalSkel, &gWolfosWaitingAnim, self->jointTable,
                           self->morphTable, WOLFOS_LIMB_MAX);
        Actor_SetScale(thisx, 0.0075f);
        thisx->naviEnemyId = 0x4C; // Wolfos
    } else {                       // WOLFOS_WHITE
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gWolfosWhiteSkel, &gWolfosWaitingAnim, self->jointTable,
                           self->morphTable, WOLFOS_LIMB_MAX);
        Actor_SetScale(thisx, 0.01f);
        self->colliderSpheres.elements[0].info.toucher.damage = self->colliderSpheres.elements[1].info.toucher.damage =
            8;
        thisx->naviEnemyId = 0x57; // White Wolfos
    }

    EnWf_SetupWaitToAppear(self);

    if ((self->switchFlag != 0xFF) && Flags_GetSwitch(globalCtx, self->switchFlag)) {
        Actor_Kill(thisx);
    }
}

void EnWf_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnWf* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->colliderSpheres);
    Collider_DestroyCylinder(globalCtx, &self->colliderCylinderBody);
    Collider_DestroyCylinder(globalCtx, &self->colliderCylinderTail);

    if ((self->actor.params != WOLFOS_NORMAL) && (self->switchFlag != 0xFF)) {
        func_800F5B58();
    }

    if (self->actor.parent != NULL) {
        EnEncount1* parent = (EnEncount1*)self->actor.parent;

        if (parent->actor.update != NULL) {
            if (parent->curNumSpawn > 0) {
                parent->curNumSpawn--;
            }

            osSyncPrintf("\n\n");
            // "☆☆☆☆☆ Number of concurrent events ☆☆☆☆☆"
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 同時発生数 ☆☆☆☆☆%d\n" VT_RST, parent->curNumSpawn);
            osSyncPrintf("\n\n");
        }
    }
}

s32 EnWf_ChangeAction(GlobalContext* globalCtx, EnWf* self, s16 mustChoose) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    s16 wallYawDiff;
    s16 playerYawDiff;
    Actor* explosive;

    wallYawDiff = self->actor.wallYaw - self->actor.shape.rot.y;
    wallYawDiff = ABS(wallYawDiff);
    playerYawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    playerYawDiff = ABS(playerYawDiff);

    if (func_800354B4(globalCtx, &self->actor, 100.0f, 0x2710, 0x2EE0, self->actor.shape.rot.y)) {
        if (player->swordAnimation == 0x11) {
            EnWf_SetupBlocking(self);
            return true;
        }

        if ((globalCtx->gameplayFrames % 2) != 0) {
            EnWf_SetupBlocking(self);
            return true;
        }
    }

    if (func_800354B4(globalCtx, &self->actor, 100.0f, 0x5DC0, 0x2AA8, self->actor.shape.rot.y)) {
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;

        if ((self->actor.bgCheckFlags & 8) && (ABS(wallYawDiff) < 0x2EE0) && (self->actor.xzDistToPlayer < 120.0f)) {
            EnWf_SetupSomersaultAndAttack(self);
            return true;
        } else if (player->swordAnimation == 0x11) {
            EnWf_SetupBlocking(self);
            return true;
        } else if ((self->actor.xzDistToPlayer < 80.0f) && (globalCtx->gameplayFrames % 2) != 0) {
            EnWf_SetupBlocking(self);
            return true;
        } else {
            EnWf_SetupBackflipAway(self);
            return true;
        }
    }

    explosive = Actor_FindNearby(globalCtx, &self->actor, -1, ACTORCAT_EXPLOSIVE, 80.0f);

    if (explosive != NULL) {
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;

        if (((self->actor.bgCheckFlags & 8) && (wallYawDiff < 0x2EE0)) || (explosive->id == ACTOR_EN_BOM_CHU)) {
            if ((explosive->id == ACTOR_EN_BOM_CHU) && (Actor_WorldDistXYZToActor(&self->actor, explosive) < 80.0f) &&
                (s16)((self->actor.shape.rot.y - explosive->world.rot.y) + 0x8000) < 0x3E80) {
                EnWf_SetupSomersaultAndAttack(self);
                return true;
            } else {
                EnWf_SetupSidestep(self, globalCtx);
                return true;
            }
        } else {
            EnWf_SetupBackflipAway(self);
            return true;
        }
    }

    if (mustChoose) {
        s16 playerFacingAngleDiff;

        if (playerYawDiff >= 0x1B58) {
            EnWf_SetupSidestep(self, globalCtx);
            return true;
        }

        playerFacingAngleDiff = player->actor.shape.rot.y - self->actor.shape.rot.y;

        if ((self->actor.xzDistToPlayer <= 80.0f) && !Actor_OtherIsTargeted(globalCtx, &self->actor) &&
            (((globalCtx->gameplayFrames % 8) != 0) || (ABS(playerFacingAngleDiff) < 0x38E0))) {
            EnWf_SetupSlash(self);
            return true;
        }

        EnWf_SetupRunAroundPlayer(self);
        return true;
    }
    return false;
}

void EnWf_SetupWaitToAppear(EnWf* self) {
    Animation_Change(&self->skelAnime, &gWolfosRearingUpFallingOverAnim, 0.5f, 0.0f, 7.0f, ANIMMODE_ONCE_INTERP, 0.0f);
    self->actor.world.pos.y = self->actor.home.pos.y - 5.0f;
    self->actionTimer = 20;
    self->unk_300 = false;
    self->action = WOLFOS_ACTION_WAIT_TO_APPEAR;
    self->actor.flags &= ~1;
    self->actor.scale.y = 0.0f;
    self->actor.gravity = 0.0f;
    EnWf_SetupAction(self, EnWf_WaitToAppear);
}

void EnWf_WaitToAppear(EnWf* self, GlobalContext* globalCtx) {
    if (self->actionTimer >= 6) {
        self->actor.world.pos.y = self->actor.home.pos.y - 5.0f;

        if (self->actor.xzDistToPlayer < 240.0f) {
            self->actionTimer = 5;
            self->actor.flags |= 1;

            if ((self->actor.params != WOLFOS_NORMAL) && (self->switchFlag != 0xFF)) {
                func_800F5ACC(0x38); // Mini-Boss Battle Theme
            }
        }
    } else if (self->actionTimer != 0) {
        self->actor.scale.y += self->actor.scale.x * 0.2f;
        self->actor.world.pos.y += 0.5f;
        Math_SmoothStepToF(&self->actor.shape.shadowScale, 70.0f, 1.0f, 14.0f, 0.0f);
        self->actionTimer--;

        if (self->actionTimer == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_APPEAR);
        }
    } else { // actionTimer == 0
        if (SkelAnime_Update(&self->skelAnime)) {
            self->actor.scale.y = self->actor.scale.x;
            self->actor.gravity = -2.0f;
            EnWf_SetupWait(self);
        }
    }
}

void EnWf_SetupWait(EnWf* self) {
    Animation_MorphToLoop(&self->skelAnime, &gWolfosWaitingAnim, -4.0f);
    self->action = WOLFOS_ACTION_WAIT;
    self->actionTimer = (Rand_ZeroOne() * 10.0f) + 2.0f;
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnWf_SetupAction(self, EnWf_Wait);
}

void EnWf_Wait(EnWf* self, GlobalContext* globalCtx) {
    Player* player;
    s32 pad;
    s16 angle;

    player = GET_PLAYER(globalCtx);
    SkelAnime_Update(&self->skelAnime);

    if (self->unk_2E2 != 0) {
        angle = (self->actor.yawTowardsPlayer - self->actor.shape.rot.y) - self->unk_4D4.y;

        if (ABS(angle) > 0x2000) {
            self->unk_2E2--;
            return;
        }

        self->unk_2E2 = 0;
    }

    angle = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    angle = ABS(angle);

    if (!EnWf_DodgeRanged(globalCtx, self)) {
        // Only use of unk_2E0: never not zero, so self if block never runs
        if (self->unk_2E0 != 0) {
            self->unk_2E0--;

            if (angle >= 0x1FFE) {
                return;
            }
            self->unk_2E0 = 0;
        } else {
            if (EnWf_ChangeAction(globalCtx, self, false)) {
                return;
            }
        }

        angle = player->actor.shape.rot.y - self->actor.shape.rot.y;
        angle = ABS(angle);

        if ((self->actor.xzDistToPlayer < 80.0f) && (player->swordState != 0) && (angle >= 0x1F40)) {
            self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            EnWf_SetupRunAroundPlayer(self);
        } else {
            self->actionTimer--;

            if (self->actionTimer == 0) {
                if (Actor_IsFacingPlayer(&self->actor, 0x1555)) {
                    if (Rand_ZeroOne() > 0.3f) {
                        EnWf_SetupRunAtPlayer(self, globalCtx);
                    } else {
                        EnWf_SetupRunAroundPlayer(self);
                    }
                } else {
                    EnWf_SetupSearchForPlayer(self);
                }
                if ((globalCtx->gameplayFrames & 95) == 0) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_CRY);
                }
            }
        }
    }
}

void EnWf_SetupRunAtPlayer(EnWf* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(&gWolfosRunningAnim);

    Animation_Change(&self->skelAnime, &gWolfosRunningAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, -4.0f);
    self->action = WOLFOS_ACTION_RUN_AT_PLAYER;
    EnWf_SetupAction(self, EnWf_RunAtPlayer);
}

void EnWf_RunAtPlayer(EnWf* self, GlobalContext* globalCtx) {
    s32 animPrevFrame;
    s32 sp58;
    s32 pad;
    f32 baseRange = 0.0f;
    s16 playerFacingAngleDiff;
    Player* player = GET_PLAYER(globalCtx);
    s32 playSpeed;

    if (!EnWf_DodgeRanged(globalCtx, self)) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        self->actor.world.rot.y = self->actor.shape.rot.y;

        if (Actor_OtherIsTargeted(globalCtx, &self->actor)) {
            baseRange = 150.0f;
        }

        if (self->actor.xzDistToPlayer <= (50.0f + baseRange)) {
            Math_SmoothStepToF(&self->actor.speedXZ, -8.0f, 1.0f, 1.5f, 0.0f);
        } else if ((65.0f + baseRange) < self->actor.xzDistToPlayer) {
            Math_SmoothStepToF(&self->actor.speedXZ, 8.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 6.65f, 0.0f);
        }

        self->skelAnime.playSpeed = self->actor.speedXZ * 0.175f;
        playerFacingAngleDiff = player->actor.shape.rot.y - self->actor.shape.rot.y;
        playerFacingAngleDiff = ABS(playerFacingAngleDiff);

        if ((self->actor.xzDistToPlayer < (150.0f + baseRange)) && (player->swordState != 0) &&
            (playerFacingAngleDiff >= 8000)) {
            self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;

            if (Rand_ZeroOne() > 0.7f) {
                EnWf_SetupRunAroundPlayer(self);
                return;
            }
        }

        animPrevFrame = self->skelAnime.curFrame;
        SkelAnime_Update(&self->skelAnime);
        sp58 = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
        playSpeed = (f32)ABS(self->skelAnime.playSpeed);

        if (!Actor_IsFacingPlayer(&self->actor, 0x11C7)) {
            if (Rand_ZeroOne() > 0.5f) {
                EnWf_SetupRunAroundPlayer(self);
            } else {
                EnWf_SetupWait(self);
            }
        } else if (self->actor.xzDistToPlayer < (90.0f + baseRange)) {
            s16 temp_v1 = player->actor.shape.rot.y - self->actor.shape.rot.y;

            if (!Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                ((Rand_ZeroOne() > 0.03f) || ((self->actor.xzDistToPlayer <= 80.0f) && (ABS(temp_v1) < 0x38E0)))) {
                EnWf_SetupSlash(self);
            } else if (Actor_OtherIsTargeted(globalCtx, &self->actor) && (Rand_ZeroOne() > 0.5f)) {
                EnWf_SetupBackflipAway(self);
            } else {
                EnWf_SetupRunAroundPlayer(self);
            }
        }

        if (!EnWf_ChangeAction(globalCtx, self, false)) {
            if ((globalCtx->gameplayFrames & 95) == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_CRY);
            }
            if ((animPrevFrame != (s32)self->skelAnime.curFrame) && (sp58 <= 0) && ((playSpeed + animPrevFrame) > 0)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_WALK);
                Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 20.0f, 3, 3.0f, 50, 50, 1);
            }
        }
    }
}

void EnWf_SetupSearchForPlayer(EnWf* self) {
    Animation_MorphToLoop(&self->skelAnime, &gWolfosSidesteppingAnim, -4.0f);
    self->action = WOLFOS_ACTION_SEARCH_FOR_PLAYER;
    EnWf_SetupAction(self, EnWf_SearchForPlayer);
}

void EnWf_SearchForPlayer(EnWf* self, GlobalContext* globalCtx) {
    s16 yawDiff;
    s16 phi_v1;
    f32 phi_f2;

    if (!EnWf_DodgeRanged(globalCtx, self) && !EnWf_ChangeAction(globalCtx, self, false)) {
        yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        phi_v1 = (yawDiff > 0) ? (yawDiff * 0.25f) + 2000.0f : (yawDiff * 0.25f) - 2000.0f;
        self->actor.shape.rot.y += phi_v1;
        self->actor.world.rot.y = self->actor.shape.rot.y;

        if (yawDiff > 0) {
            phi_f2 = phi_v1 * 0.5f;
            phi_f2 = CLAMP_MAX(phi_f2, 1.0f);
        } else {
            phi_f2 = phi_v1 * 0.5f;
            phi_f2 = CLAMP_MIN(phi_f2, -1.0f);
        }

        self->skelAnime.playSpeed = -phi_f2;
        SkelAnime_Update(&self->skelAnime);

        if (Actor_IsFacingPlayer(&self->actor, 0x1555)) {
            if (Rand_ZeroOne() > 0.8f) {
                EnWf_SetupRunAroundPlayer(self);
            } else {
                EnWf_SetupRunAtPlayer(self, globalCtx);
            }
        }

        if ((globalCtx->gameplayFrames & 95) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_CRY);
        }
    }
}

void EnWf_SetupRunAroundPlayer(EnWf* self) {
    f32 lastFrame = Animation_GetLastFrame(&gWolfosRunningAnim);

    Animation_Change(&self->skelAnime, &gWolfosRunningAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, -4.0f);

    if (Rand_ZeroOne() > 0.5f) {
        self->runAngle = 16000;
    } else {
        self->runAngle = -16000;
    }

    self->skelAnime.playSpeed = self->actor.speedXZ = 6.0f;
    self->skelAnime.playSpeed *= 0.175f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actionTimer = (Rand_ZeroOne() * 30.0f) + 30.0f;
    self->action = WOLFOS_ACTION_RUN_AROUND_PLAYER;
    self->runSpeed = 0.0f;

    EnWf_SetupAction(self, EnWf_RunAroundPlayer);
}

void EnWf_RunAroundPlayer(EnWf* self, GlobalContext* globalCtx) {
    s16 angle1;
    s16 angle2;
    s32 pad;
    f32 baseRange = 0.0f;
    s32 animPrevFrame;
    s32 animFrameSpeedDiff;
    s32 animSpeed;
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer + self->runAngle, 1, 4000, 1);

    if (!EnWf_DodgeRanged(globalCtx, self) && !EnWf_ChangeAction(globalCtx, self, false)) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        angle1 = player->actor.shape.rot.y + self->runAngle + 0x8000;

        // Actor_TestFloorInDirection is useless here (see comment below)
        if ((self->actor.bgCheckFlags & 8) ||
            !Actor_TestFloorInDirection(&self->actor, globalCtx, self->actor.speedXZ, self->actor.shape.rot.y)) {
            angle2 = (self->actor.bgCheckFlags & 8)
                         ? (self->actor.wallYaw - self->actor.yawTowardsPlayer) - self->runAngle
                         : 0;

            // This is probably meant to reverse direction if the edge of a floor is encountered, but does nothing
            // unless bgCheckFlags & 8 anyway, since angle2 = 0 otherwise
            if (ABS(angle2) > 0x2EE0) {
                self->runAngle = -self->runAngle;
            }
        }

        if (Actor_OtherIsTargeted(globalCtx, &self->actor)) {
            baseRange = 150.0f;
        }

        if (self->actor.xzDistToPlayer <= (60.0f + baseRange)) {
            Math_SmoothStepToF(&self->runSpeed, -4.0f, 1.0f, 1.5f, 0.0f);
        } else if ((80.0f + baseRange) < self->actor.xzDistToPlayer) {
            Math_SmoothStepToF(&self->runSpeed, 4.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->runSpeed, 0.0f, 1.0f, 6.65f, 0.0f);
        }

        if (self->runSpeed != 0.0f) {
            self->actor.world.pos.x += Math_SinS(self->actor.shape.rot.y) * self->runSpeed;
            self->actor.world.pos.z += Math_CosS(self->actor.shape.rot.y) * self->runSpeed;
        }

        if (ABS(self->runSpeed) < ABS(self->actor.speedXZ)) {
            self->skelAnime.playSpeed = self->actor.speedXZ * 0.175f;
        } else {
            self->skelAnime.playSpeed = self->runSpeed * 0.175f;
        }

        self->skelAnime.playSpeed = CLAMP(self->skelAnime.playSpeed, -3.0f, 3.0f);
        animPrevFrame = self->skelAnime.curFrame;
        SkelAnime_Update(&self->skelAnime);
        animFrameSpeedDiff = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
        animSpeed = (f32)ABS(self->skelAnime.playSpeed);

        if ((animPrevFrame != (s32)self->skelAnime.curFrame) && (animFrameSpeedDiff <= 0) &&
            (animSpeed + animPrevFrame > 0)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_WALK);
            Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 20.0f, 3, 3.0f, 50, 50, 1);
        }

        if ((globalCtx->gameplayFrames & 95) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_CRY);
        }

        if ((Math_CosS(angle1 - self->actor.shape.rot.y) < -0.85f) && !Actor_OtherIsTargeted(globalCtx, &self->actor) &&
            (self->actor.xzDistToPlayer <= 80.0f)) {
            EnWf_SetupSlash(self);
        } else {
            self->actionTimer--;

            if (self->actionTimer == 0) {
                if (Actor_OtherIsTargeted(globalCtx, &self->actor) && (Rand_ZeroOne() > 0.5f)) {
                    EnWf_SetupBackflipAway(self);
                } else {
                    EnWf_SetupWait(self);
                    self->actionTimer = (Rand_ZeroOne() * 3.0f) + 1.0f;
                }
            }
        }
    }
}

void EnWf_SetupSlash(EnWf* self) {
    Animation_PlayOnce(&self->skelAnime, &gWolfosSlashingAnim);
    self->colliderSpheres.base.atFlags &= ~AT_BOUNCED;
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    self->action = WOLFOS_ACTION_SLASH;
    self->unk_2FA = 0; // Set and not used
    self->actionTimer = 7;
    self->skelAnime.endFrame = 20.0f;
    self->actor.speedXZ = 0.0f;

    EnWf_SetupAction(self, EnWf_Slash);
}

void EnWf_Slash(EnWf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 shapeAngleDiff = player->actor.shape.rot.y - self->actor.shape.rot.y;
    s16 yawAngleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    s32 curFrame = self->skelAnime.curFrame;

    shapeAngleDiff = ABS(shapeAngleDiff);
    yawAngleDiff = ABS(yawAngleDiff);
    self->actor.speedXZ = 0.0f;

    if (((curFrame >= 9) && (curFrame <= 12)) || ((curFrame >= 17) && (curFrame <= 19))) {
        if (self->slashStatus == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_ATTACK);
        }

        self->slashStatus = 1;
    } else {
        self->slashStatus = 0;
    }

    if (((curFrame == 15) && !Actor_IsTargeted(globalCtx, &self->actor) &&
         (!Actor_IsFacingPlayer(&self->actor, 0x2000) || (self->actor.xzDistToPlayer >= 100.0f))) ||
        SkelAnime_Update(&self->skelAnime)) {
        if ((curFrame != 15) && (self->actionTimer != 0)) {
            self->actor.shape.rot.y += (s16)(3276.0f * (1.5f + (self->actionTimer - 4) * 0.4f));
            Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 15.0f, 1, 2.0f, 50, 50, 1);
            self->actionTimer--;
        } else if (!Actor_IsFacingPlayer(&self->actor, 0x1554) && (curFrame != 15)) {
            EnWf_SetupWait(self);
            self->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;

            if (yawAngleDiff > 13000) {
                self->unk_2E2 = 7;
            }
        } else if ((Rand_ZeroOne() > 0.7f) || (self->actor.xzDistToPlayer >= 120.0f)) {
            EnWf_SetupWait(self);
            self->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;
        } else {
            self->actor.world.rot.y = self->actor.yawTowardsPlayer;

            if (Rand_ZeroOne() > 0.7f) {
                EnWf_SetupSidestep(self, globalCtx);
            } else if (shapeAngleDiff <= 10000) {
                if (yawAngleDiff > 16000) {
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    EnWf_SetupRunAroundPlayer(self);
                } else {
                    EnWf_ChangeAction(globalCtx, self, true);
                }
            } else {
                EnWf_SetupRunAroundPlayer(self);
            }
        }
    }
}

void EnWf_SetupRecoilFromBlockedSlash(EnWf* self) {
    f32 endFrame = 1.0f;

    if ((s32)self->skelAnime.curFrame >= 16) {
        endFrame = 15.0f;
    }

    Animation_Change(&self->skelAnime, &gWolfosSlashingAnim, -0.5f, self->skelAnime.curFrame - 1.0f, endFrame,
                     ANIMMODE_ONCE_INTERP, 0.0f);
    self->action = WOLFOS_ACTION_RECOIL_FROM_BLOCKED_SLASH;
    self->slashStatus = 0;
    EnWf_SetupAction(self, EnWf_RecoilFromBlockedSlash);
}

void EnWf_RecoilFromBlockedSlash(EnWf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 angle1 = player->actor.shape.rot.y - self->actor.shape.rot.y;
    s16 angle2 = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    angle1 = ABS(angle1);
    angle2 = ABS(angle2);

    if (SkelAnime_Update(&self->skelAnime)) {
        if (!Actor_IsFacingPlayer(&self->actor, 0x1554)) {
            EnWf_SetupWait(self);
            self->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;

            if (angle2 > 0x32C8) {
                self->unk_2E2 = 30;
            }
        } else {
            if ((Rand_ZeroOne() > 0.7f) || (self->actor.xzDistToPlayer >= 120.0f)) {
                EnWf_SetupWait(self);
                self->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;
            } else {
                self->actor.world.rot.y = self->actor.yawTowardsPlayer;

                if (Rand_ZeroOne() > 0.7f) {
                    EnWf_SetupSidestep(self, globalCtx);
                } else if (angle1 <= 0x2710) {
                    if (angle2 > 0x3E80) {
                        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                        EnWf_SetupRunAroundPlayer(self);
                    } else {
                        EnWf_ChangeAction(globalCtx, self, true);
                    }
                } else {
                    EnWf_SetupRunAroundPlayer(self);
                }
            }
        }
    }
}

void EnWf_SetupBackflipAway(EnWf* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gWolfosBackflippingAnim, -3.0f);
    self->actor.speedXZ = -6.0f;
    self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->actionTimer = 0;
    self->unk_300 = true;
    self->action = WOLFOS_ACTION_BACKFLIP_AWAY;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    EnWf_SetupAction(self, EnWf_BackflipAway);
}

void EnWf_BackflipAway(EnWf* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if (!Actor_OtherIsTargeted(globalCtx, &self->actor) && (self->actor.xzDistToPlayer < 170.0f) &&
            (self->actor.xzDistToPlayer > 140.0f) && (Rand_ZeroOne() < 0.2f)) {
            EnWf_SetupRunAtPlayer(self, globalCtx);
        } else if ((globalCtx->gameplayFrames % 2) != 0) {
            EnWf_SetupSidestep(self, globalCtx);
        } else {
            EnWf_SetupWait(self);
        }
    }
    if ((globalCtx->state.frames & 95) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_CRY);
    }
}

void EnWf_SetupStunned(EnWf* self) {
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = 0.0f;
    }

    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    Animation_PlayOnceSetSpeed(&self->skelAnime, &gWolfosDamagedAnim, 0.0f);
    self->action = WOLFOS_ACTION_STUNNED;
    EnWf_SetupAction(self, EnWf_Stunned);
}

void EnWf_Stunned(EnWf* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }

    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }

        self->unk_300 = false;
    }

    if ((self->actor.colorFilterTimer == 0) && (self->actor.bgCheckFlags & 1)) {
        if (self->actor.colChkInfo.health == 0) {
            EnWf_SetupDie(self);
        } else {
            EnWf_ChangeAction(globalCtx, self, true);
        }
    }
}

void EnWf_SetupDamaged(EnWf* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gWolfosDamagedAnim, -4.0f);

    if (self->actor.bgCheckFlags & 1) {
        self->unk_300 = false;
        self->actor.speedXZ = -4.0f;
    } else {
        self->unk_300 = true;
    }

    self->unk_2E2 = 0;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_DAMAGE);
    self->action = WOLFOS_ACTION_DAMAGED;
    EnWf_SetupAction(self, EnWf_Damaged);
}

void EnWf_Damaged(EnWf* self, GlobalContext* globalCtx) {
    s16 angleToWall;

    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }

    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }

        self->unk_300 = false;
    }

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 4500, 0);

    if (!EnWf_ChangeAction(globalCtx, self, false) && SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.bgCheckFlags & 1) {
            angleToWall = self->actor.wallYaw - self->actor.shape.rot.y;
            angleToWall = ABS(angleToWall);

            if ((self->actor.bgCheckFlags & 8) && (ABS(angleToWall) < 12000) && (self->actor.xzDistToPlayer < 120.0f)) {
                EnWf_SetupSomersaultAndAttack(self);
            } else if (!EnWf_DodgeRanged(globalCtx, self)) {
                if ((self->actor.xzDistToPlayer <= 80.0f) && !Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                    ((globalCtx->gameplayFrames % 8) != 0)) {
                    EnWf_SetupSlash(self);
                } else if (Rand_ZeroOne() > 0.5f) {
                    EnWf_SetupWait(self);
                    self->actionTimer = (Rand_ZeroOne() * 5.0f) + 5.0f;
                    self->unk_2E2 = 30;
                } else {
                    EnWf_SetupBackflipAway(self);
                }
            }
        }
    }
}

void EnWf_SetupSomersaultAndAttack(EnWf* self) {
    f32 lastFrame = Animation_GetLastFrame(&gWolfosBackflippingAnim);

    Animation_Change(&self->skelAnime, &gWolfosBackflippingAnim, -1.0f, lastFrame, 0.0f, ANIMMODE_ONCE, -3.0f);
    self->actionTimer = 0;
    self->unk_300 = false;
    self->action = WOLFOS_ACTION_TURN_TOWARDS_PLAYER;
    self->actor.speedXZ = 6.5f;
    self->actor.velocity.y = 15.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnWf_SetupAction(self, EnWf_SomersaultAndAttack);
}

void EnWf_SomersaultAndAttack(EnWf* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 4000, 1);

    if (self->actor.velocity.y >= 5.0f) {
        //! @bug unk_4C8 and unk_4BC are used but not set (presumably intended to be feet positions like other actors)
        func_800355B8(globalCtx, &self->unk_4C8);
        func_800355B8(globalCtx, &self->unk_4BC);
    }

    if (SkelAnime_Update(&self->skelAnime) && (self->actor.bgCheckFlags & (1 | 2))) {
        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->actor.shape.rot.x = 0;
        self->actor.speedXZ = self->actor.velocity.y = 0.0f;
        self->actor.world.pos.y = self->actor.floorHeight;

        if (!Actor_OtherIsTargeted(globalCtx, &self->actor)) {
            EnWf_SetupSlash(self);
        } else {
            EnWf_SetupWait(self);
        }
    }
}

void EnWf_SetupBlocking(EnWf* self) {
    f32 lastFrame = Animation_GetLastFrame(&gWolfosBlockingAnim);

    if (self->slashStatus != 0) {
        self->slashStatus = -1;
    }

    self->actor.speedXZ = 0.0f;
    self->action = WOLFOS_ACTION_BLOCKING;
    self->actionTimer = 10;

    Animation_Change(&self->skelAnime, &gWolfosBlockingAnim, 0.0f, 0.0f, lastFrame, ANIMMODE_ONCE_INTERP, -4.0f);
    EnWf_SetupAction(self, EnWf_Blocking);
}

void EnWf_Blocking(EnWf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;

    if (self->actionTimer != 0) {
        self->actionTimer--;
    } else {
        self->skelAnime.playSpeed = 1.0f;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        s16 yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if ((ABS(yawDiff) <= 0x4000) && (self->actor.xzDistToPlayer < 60.0f) &&
            (ABS(self->actor.yDistToPlayer) < 50.0f)) {
            if (func_800354B4(globalCtx, &self->actor, 100.0f, 10000, 0x4000, self->actor.shape.rot.y)) {
                if (player->swordAnimation == 0x11) {
                    EnWf_SetupBlocking(self);
                } else if ((globalCtx->gameplayFrames % 2) != 0) {
                    EnWf_SetupBlocking(self);
                } else {
                    EnWf_SetupBackflipAway(self);
                }

            } else {
                s16 angleFacingLink = player->actor.shape.rot.y - self->actor.shape.rot.y;

                if (!Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                    (((globalCtx->gameplayFrames % 2) != 0) || (ABS(angleFacingLink) < 0x38E0))) {
                    EnWf_SetupSlash(self);
                } else {
                    EnWf_SetupRunAroundPlayer(self);
                }
            }
        } else {
            EnWf_SetupRunAroundPlayer(self);
        }
    } else if (self->actionTimer == 0) {
        if (func_800354B4(globalCtx, &self->actor, 100.0f, 10000, 0x4000, self->actor.shape.rot.y)) {
            if (player->swordAnimation == 0x11) {
                EnWf_SetupBlocking(self);
            } else if ((globalCtx->gameplayFrames % 2) != 0) {
                EnWf_SetupBlocking(self);
            } else {
                EnWf_SetupBackflipAway(self);
            }
        }
    }
}

void EnWf_SetupSidestep(EnWf* self, GlobalContext* globalCtx) {
    s16 angle;
    Player* player;
    f32 lastFrame = Animation_GetLastFrame(&gWolfosRunningAnim);

    Animation_Change(&self->skelAnime, &gWolfosRunningAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, -4.0f);

    player = GET_PLAYER(globalCtx);
    angle = player->actor.shape.rot.y + self->runAngle;

    if (Math_SinS(angle - self->actor.yawTowardsPlayer) > 0.0f) {
        self->runAngle = 16000;
    } else if (Math_SinS(angle - self->actor.yawTowardsPlayer) < 0.0f) {
        self->runAngle = -16000;
    } else if (Rand_ZeroOne() > 0.5f) {
        self->runAngle = 16000;
    } else {
        self->runAngle = -16000;
    }

    self->skelAnime.playSpeed = self->actor.speedXZ = 6.0f;
    self->skelAnime.playSpeed *= 0.175f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->runSpeed = 0.0f;
    self->actionTimer = (Rand_ZeroOne() * 10.0f) + 5.0f;
    self->action = WOLFOS_ACTION_SIDESTEP;

    EnWf_SetupAction(self, EnWf_Sidestep);
}

void EnWf_Sidestep(EnWf* self, GlobalContext* globalCtx) {
    s16 angleDiff1;
    Player* player = GET_PLAYER(globalCtx);
    s32 animPrevFrame;
    s32 animFrameSpeedDiff;
    s32 animSpeed;
    f32 baseRange = 0.0f;

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer + self->runAngle, 1, 3000, 1);

    // Actor_TestFloorInDirection is useless here (see comment below)
    if ((self->actor.bgCheckFlags & 8) ||
        !Actor_TestFloorInDirection(&self->actor, globalCtx, self->actor.speedXZ, self->actor.shape.rot.y)) {
        s16 angle =
            (self->actor.bgCheckFlags & 8) ? (self->actor.wallYaw - self->actor.yawTowardsPlayer) - self->runAngle : 0;

        // This is probably meant to reverse direction if the edge of a floor is encountered, but does nothing
        // unless bgCheckFlags & 8 anyway, since angle = 0 otherwise
        if (ABS(angle) > 0x2EE0) {
            self->runAngle = -self->runAngle;
        }
    }

    self->actor.world.rot.y = self->actor.shape.rot.y;

    if (Actor_OtherIsTargeted(globalCtx, &self->actor)) {
        baseRange = 150.0f;
    }

    if (self->actor.xzDistToPlayer <= (60.0f + baseRange)) {
        Math_SmoothStepToF(&self->runSpeed, -4.0f, 1.0f, 1.5f, 0.0f);
    } else if ((80.0f + baseRange) < self->actor.xzDistToPlayer) {
        Math_SmoothStepToF(&self->runSpeed, 4.0f, 1.0f, 1.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->runSpeed, 0.0f, 1.0f, 6.65f, 0.0f);
    }

    if (self->runSpeed != 0.0f) {
        self->actor.world.pos.x += Math_SinS(self->actor.shape.rot.y) * self->runSpeed;
        self->actor.world.pos.z += Math_CosS(self->actor.shape.rot.y) * self->runSpeed;
    }

    if (ABS(self->runSpeed) < ABS(self->actor.speedXZ)) {
        self->skelAnime.playSpeed = self->actor.speedXZ * 0.175f;
    } else {
        self->skelAnime.playSpeed = self->runSpeed * 0.175f;
    }

    self->skelAnime.playSpeed = CLAMP(self->skelAnime.playSpeed, -3.0f, 3.0f);

    animPrevFrame = self->skelAnime.curFrame;
    SkelAnime_Update(&self->skelAnime);
    animFrameSpeedDiff = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
    animSpeed = (f32)ABS(self->skelAnime.playSpeed);

    if (!EnWf_ChangeAction(globalCtx, self, false)) {
        self->actionTimer--;

        if (self->actionTimer == 0) {
            angleDiff1 = player->actor.shape.rot.y - self->actor.yawTowardsPlayer;
            angleDiff1 = ABS(angleDiff1);

            if (angleDiff1 >= 0x3A98) {
                EnWf_SetupWait(self);
                self->actionTimer = (Rand_ZeroOne() * 3.0f) + 1.0f;
            } else {
                Player* player2 = GET_PLAYER(globalCtx);
                s16 angleDiff2 = player2->actor.shape.rot.y - self->actor.yawTowardsPlayer;

                self->actor.world.rot.y = self->actor.shape.rot.y;

                if ((self->actor.xzDistToPlayer <= 80.0f) && !Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                    (((globalCtx->gameplayFrames % 4) == 0) || (ABS(angleDiff2) < 0x38E0))) {
                    EnWf_SetupSlash(self);
                } else {
                    EnWf_SetupRunAtPlayer(self, globalCtx);
                }
            }
        }

        if ((animPrevFrame != (s32)self->skelAnime.curFrame) && (animFrameSpeedDiff <= 0) &&
            ((animSpeed + animPrevFrame) > 0)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_WALK);
            Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 20.0f, 3, 3.0f, 50, 50, 1);
        }

        if ((globalCtx->gameplayFrames & 95) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_CRY);
        }
    }
}

void EnWf_SetupDie(EnWf* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gWolfosRearingUpFallingOverAnim, -4.0f);
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;

    if (self->actor.bgCheckFlags & 1) {
        self->unk_300 = false;
        self->actor.speedXZ = -6.0f;
    } else {
        self->unk_300 = true;
    }

    self->action = WOLFOS_ACTION_DIE;
    self->actor.flags &= ~1;
    self->actionTimer = self->skelAnime.animLength;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_WOLFOS_DEAD);
    EnWf_SetupAction(self, EnWf_Die);
}

void EnWf_Die(EnWf* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }

    if (self->actor.bgCheckFlags & 1) {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
        self->unk_300 = false;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xD0);

        if (self->switchFlag != 0xFF) {
            Flags_SetSwitch(globalCtx, self->switchFlag);
        }

        Actor_Kill(&self->actor);
    } else {
        s32 i;
        Vec3f pos;
        Vec3f velAndAccel = { 0.0f, 0.5f, 0.0f };

        self->actionTimer--;

        for (i = ((s32)self->skelAnime.animLength - self->actionTimer) >> 1; i >= 0; i--) {
            pos.x = Rand_CenteredFloat(60.0f) + self->actor.world.pos.x;
            pos.z = Rand_CenteredFloat(60.0f) + self->actor.world.pos.z;
            pos.y = Rand_CenteredFloat(50.0f) + (self->actor.world.pos.y + 20.0f);
            EffectSsDeadDb_Spawn(globalCtx, &pos, &velAndAccel, &velAndAccel, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1,
                                 9, true);
        }
    }
}

void func_80B36F40(EnWf* self, GlobalContext* globalCtx) {
    if ((self->action == WOLFOS_ACTION_WAIT) && (self->unk_2E2 != 0)) {
        self->unk_4D4.y = Math_SinS(self->unk_2E2 * 4200) * 8920.0f;
    } else if (self->action != WOLFOS_ACTION_STUNNED) {
        if (self->action != WOLFOS_ACTION_SLASH) {
            Math_SmoothStepToS(&self->unk_4D4.y, self->actor.yawTowardsPlayer - self->actor.shape.rot.y, 1, 1500, 0);
            self->unk_4D4.y = CLAMP(self->unk_4D4.y, -0x3127, 0x3127);
        } else {
            self->unk_4D4.y = 0;
        }
    }
}

void EnWf_UpdateDamage(EnWf* self, GlobalContext* globalCtx) {
    if (self->colliderSpheres.base.acFlags & AC_BOUNCED) {
        self->colliderSpheres.base.acFlags &= ~(AC_HIT | AC_BOUNCED);
        self->colliderCylinderBody.base.acFlags &= ~AC_HIT;
        self->colliderCylinderTail.base.acFlags &= ~AC_HIT;
    } else if ((self->colliderCylinderBody.base.acFlags & AC_HIT) ||
               (self->colliderCylinderTail.base.acFlags & AC_HIT)) {
        if (self->action >= WOLFOS_ACTION_WAIT) {
            s16 yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

            if ((!(self->colliderCylinderBody.base.acFlags & AC_HIT) &&
                 (self->colliderCylinderTail.base.acFlags & AC_HIT)) ||
                (ABS(yawDiff) > 19000)) {
                self->actor.colChkInfo.damage *= 4;
            }

            self->colliderCylinderBody.base.acFlags &= ~AC_HIT;
            self->colliderCylinderTail.base.acFlags &= ~AC_HIT;

            if (self->actor.colChkInfo.damageEffect != ENWF_DMGEFF_ICE_MAGIC) {
                self->damageEffect = self->actor.colChkInfo.damageEffect;
                Actor_SetDropFlag(&self->actor, &self->colliderCylinderBody.info, 1);
                self->slashStatus = 0;

                if ((self->actor.colChkInfo.damageEffect == ENWF_DMGEFF_STUN) ||
                    (self->actor.colChkInfo.damageEffect == ENWF_DMGEFF_UNDEF)) {
                    if (self->action != WOLFOS_ACTION_STUNNED) {
                        Actor_SetColorFilter(&self->actor, 0, 120, 0, 80);
                        Actor_ApplyDamage(&self->actor);
                        EnWf_SetupStunned(self);
                    }
                } else { // LIGHT_MAGIC, FIRE, NONE
                    Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 8);

                    if (self->damageEffect == ENWF_DMGEFF_FIRE) {
                        self->fireTimer = 40;
                    }

                    if (Actor_ApplyDamage(&self->actor) == 0) {
                        EnWf_SetupDie(self);
                        Enemy_StartFinishingBlow(globalCtx, &self->actor);
                    } else {
                        EnWf_SetupDamaged(self);
                    }
                }
            }
        }
    }
}

void EnWf_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnWf* self = THIS;

    EnWf_UpdateDamage(self, globalCtx);

    if (self->actor.colChkInfo.damageEffect != ENWF_DMGEFF_ICE_MAGIC) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 32.0f, 30.0f, 60.0f, 0x1D);
        self->actionFunc(self, globalCtx);
        func_80B36F40(self, globalCtx);
    }

    if (self->actor.bgCheckFlags & (1 | 2)) {
        func_800359B8(&self->actor, self->actor.shape.rot.y, &self->actor.shape.rot);
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 1000, 0);
        Math_SmoothStepToS(&self->actor.shape.rot.z, 0, 1, 1000, 0);
    }

    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderSpheres.base);

    if (self->action >= WOLFOS_ACTION_WAIT) {
        if ((self->actor.colorFilterTimer == 0) || !(self->actor.colorFilterParams & 0x4000)) {
            Collider_UpdateCylinder(&self->actor, &self->colliderCylinderBody);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderCylinderTail.base);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderCylinderBody.base);
        }
    }

    if (self->action == WOLFOS_ACTION_BLOCKING) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderSpheres.base);
    }

    if (self->slashStatus > 0) {
        if (!(self->colliderSpheres.base.atFlags & AT_BOUNCED)) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderSpheres.base);
        } else {
            EnWf_SetupRecoilFromBlockedSlash(self);
        }
    }

    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 25.0f;

    if (self->eyeIndex == 0) {
        if ((Rand_ZeroOne() < 0.2f) && ((globalCtx->gameplayFrames % 4) == 0) && (self->actor.colorFilterTimer == 0)) {
            self->eyeIndex++;
        }
    } else {
        self->eyeIndex = (self->eyeIndex + 1) & 3;
    }
}

s32 EnWf_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnWf* self = THIS;

    if ((limbIndex == WOLFOS_LIMB_HEAD) || (limbIndex == WOLFOS_LIMB_EYES)) {
        rot->y -= self->unk_4D4.y;
    }

    return false;
}

void EnWf_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f colliderVec = { 1200.0f, 0.0f, 0.0f };
    static Vec3f bodyPartVec = { 0.0f, 0.0f, 0.0f };
    EnWf* self = THIS;
    s32 bodyPartIndex = -1;

    Collider_UpdateSpheres(limbIndex, &self->colliderSpheres);

    if (limbIndex == WOLFOS_LIMB_TAIL) {
        Vec3f colliderPos;

        bodyPartIndex = -1;
        Matrix_MultVec3f(&colliderVec, &colliderPos);
        self->colliderCylinderTail.dim.pos.x = colliderPos.x;
        self->colliderCylinderTail.dim.pos.y = colliderPos.y;
        self->colliderCylinderTail.dim.pos.z = colliderPos.z;
    }

    if ((self->fireTimer != 0) || ((self->actor.colorFilterTimer != 0) && (self->actor.colorFilterParams & 0x4000))) {
        switch (limbIndex) {
            case WOLFOS_LIMB_EYES:
                bodyPartIndex = 0;
                break;
            case WOLFOS_LIMB_FRONT_RIGHT_LOWER_LEG:
                bodyPartIndex = 1;
                break;
            case WOLFOS_LIMB_FRONT_LEFT_LOWER_LEG:
                bodyPartIndex = 2;
                break;
            case WOLFOS_LIMB_THORAX:
                bodyPartIndex = 3;
                break;
            case WOLFOS_LIMB_ABDOMEN:
                bodyPartIndex = 4;
                break;
            case WOLFOS_LIMB_TAIL:
                bodyPartIndex = 5;
                break;
            case WOLFOS_LIMB_BACK_RIGHT_SHIN:
                bodyPartIndex = 6;
                break;
            case 37:
                //! @bug There is no limb with index self large, so bodyPartsPos[7] is uninitialised. Thus a flame will
                //! be drawn at 0,0,0 when the Wolfos is on fire.
                bodyPartIndex = 7;
                break;
            case WOLFOS_LIMB_BACK_RIGHT_PASTERN:
                bodyPartIndex = 8;
                break;
            case WOLFOS_LIMB_BACK_LEFT_PAW:
                bodyPartIndex = 9;
                break;
        }

        if (bodyPartIndex >= 0) {
            Vec3f bodyPartPos;

            Matrix_MultVec3f(&bodyPartVec, &bodyPartPos);
            self->bodyPartsPos[bodyPartIndex].x = bodyPartPos.x;
            self->bodyPartsPos[bodyPartIndex].y = bodyPartPos.y;
            self->bodyPartsPos[bodyPartIndex].z = bodyPartPos.z;
        }
    }
}

static void* sWolfosNormalEyeTextures[] = { gWolfosNormalEyeOpenTex, gWolfosNormalEyeHalfTex, gWolfosNormalEyeNarrowTex,
                                            gWolfosNormalEyeHalfTex };
static void* sWolfosWhiteEyeTextures[] = { gWolfosWhiteEyeOpenTex, gWolfosWhiteEyeHalfTex, gWolfosWhiteEyeNarrowTex,
                                           gWolfosWhiteEyeHalfTex };

void EnWf_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnWf* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_wf.c", 2157);

    // This conditional will always evaluate to true, since unk_300 is false whenever action is WOLFOS_ACTION_WAIT_TO_APPEAR.
    if ((self->action != WOLFOS_ACTION_WAIT_TO_APPEAR) || !self->unk_300) {
        func_80093D18(globalCtx->state.gfxCtx);

        if (self->actor.params == WOLFOS_NORMAL) {
            gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sWolfosNormalEyeTextures[self->eyeIndex]));
        } else {
            gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sWolfosWhiteEyeTextures[self->eyeIndex]));
        }

        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnWf_OverrideLimbDraw, EnWf_PostLimbDraw, &self->actor);

        if (self->fireTimer != 0) {
            self->actor.colorFilterTimer++;
            if (1) {}
            self->fireTimer--;

            if ((self->fireTimer % 4) == 0) {
                s32 fireIndex = self->fireTimer >> 2;

                EffectSsEnFire_SpawnVec3s(globalCtx, &self->actor, &self->bodyPartsPos[fireIndex], 75, 0, 0, fireIndex);
            }
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_wf.c", 2190);
}

s32 EnWf_DodgeRanged(GlobalContext* globalCtx, EnWf* self) {
    Actor* actor = Actor_GetProjectileActor(globalCtx, &self->actor, 600.0f);

    if (actor != NULL) {
        s16 angleToFacing;
        s16 pad;
        f32 dist;

        angleToFacing = Actor_WorldYawTowardActor(&self->actor, actor) - self->actor.shape.rot.y;
        self->actor.world.rot.y = (u16)self->actor.shape.rot.y & 0xFFFF;
        dist = Actor_WorldDistXYZToPoint(&self->actor, &actor->world.pos);

        if ((ABS(angleToFacing) < 0x2EE0) && (sqrt(dist) < 400.0)) {
            EnWf_SetupBlocking(self);
        } else {
            self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
            if ((ABS(angleToFacing) < 0x2000) || (ABS(angleToFacing) > 0x5FFF)) {
                EnWf_SetupSidestep(self, globalCtx);
                self->actor.speedXZ *= 2.0f;
            } else if (ABS(angleToFacing) < 0x5FFF) {
                EnWf_SetupBackflipAway(self);
            }
        }
        return true;
    }

    return false;
}
