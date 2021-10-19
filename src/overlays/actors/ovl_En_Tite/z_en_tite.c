/*
 * File: z_en_tite.c
 * Overlay: ovl_En_Tite
 * Description: Tektite
 */

#include "z_en_tite.h"
#include "overlays/actors/ovl_En_Encount1/z_en_encount1.h"
#include "overlays/effects/ovl_Effect_Ss_Dead_Sound/z_eff_ss_dead_sound.h"
#include "vt.h"
#include "objects/object_tite/object_tite.h"

#define FLAGS 0x00000015

#define THIS ((EnTite*)thisx)

// EnTite_Idle
#define vIdleTimer actionVar1

// EnTite_Attack (vQueuedJumps also used by EnTite_MoveTowardPlayer)
#define vAttackState actionVar1
#define vQueuedJumps actionVar2

//  EnTite_FlipOnBack
#define vOnBackTimer actionVar1
#define vLegTwitchTimer actionVar2

typedef enum {
    /* 0x0 */ TEKTITE_DEATH_CRY,
    /* 0x1 */ TEKTITE_UNK_1,
    /* 0x2 */ TEKTITE_UNK_2,
    /* 0x3 */ TEKTITE_RECOIL,
    /* 0x4 */ TEKTITE_UNK_4,
    /* 0x5 */ TEKTITE_FALL_APART,
    /* 0x6 */ TEKTITE_IDLE,
    /* 0x7 */ TEKTITE_STUNNED,
    /* 0x8 */ TEKTITE_UNK_8,
    /* 0x9 */ TEKTITE_ATTACK,
    /* 0xA */ TEKTITE_TURN_TOWARD_PLAYER,
    /* 0xB */ TEKTITE_UNK9,
    /* 0xC */ TEKTITE_MOVE_TOWARD_PLAYER
} EnTiteAction;

typedef enum {
    /* 0x0 */ TEKTITE_BEGIN_LUNGE,
    /* 0x1 */ TEKTITE_MID_LUNGE,
    /* 0x2 */ TEKTITE_LANDED,
    /* 0x2 */ TEKTITE_SUBMERGED
} EnTiteAttackState;

typedef enum {
    /* 0x0 */ TEKTITE_INITIAL,
    /* 0x1 */ TEKTITE_UNFLIPPED,
    /* 0x2 */ TEKTITE_FLIPPED
} EnTiteFlipState;

void EnTite_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTite_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTite_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTite_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnTite_SetupIdle(EnTite* self);
void EnTite_SetupTurnTowardPlayer(EnTite* self);
void EnTite_SetupMoveTowardPlayer(EnTite* self);
void EnTite_SetupDeathCry(EnTite* self);
void EnTite_SetupFlipUpright(EnTite* self);

void EnTite_Idle(EnTite* self, GlobalContext* globalCtx);
void EnTite_Attack(EnTite* self, GlobalContext* globalCtx);
void EnTite_TurnTowardPlayer(EnTite* self, GlobalContext* globalCtx);
void EnTite_MoveTowardPlayer(EnTite* self, GlobalContext* globalCtx);
void EnTite_Recoil(EnTite* self, GlobalContext* globalCtx);
void EnTite_Stunned(EnTite* self, GlobalContext* globalCtx);
void EnTite_DeathCry(EnTite* self, GlobalContext* globalCtx);
void EnTite_FallApart(EnTite* self, GlobalContext* globalCtx);
void EnTite_FlipOnBack(EnTite* self, GlobalContext* globalCtx);
void EnTite_FlipUpright(EnTite* self, GlobalContext* globalCtx);

const ActorInit En_Tite_InitVars = {
    ACTOR_EN_TITE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_TITE,
    sizeof(EnTite),
    (ActorFunc)EnTite_Init,
    (ActorFunc)EnTite_Destroy,
    (ActorFunc)EnTite_Update,
    (ActorFunc)EnTite_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { 0, { { 0, 1500, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static DamageTable sDamageTable[] = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x1),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(2, 0x0),
    /* Ice arrow     */ DMG_ENTRY(4, 0xF),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0xE),
    /* Ice magic     */ DMG_ENTRY(3, 0xF),
    /* Light magic   */ DMG_ENTRY(0, 0xE),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0x0),
    /* Master spin   */ DMG_ENTRY(2, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0x0),
    /* Master jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x45, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(minVelocityY, -40, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_STOP),
};

static AnimationHeader* D_80B1B634[] = {
    &object_tite_Anim_00083C, &object_tite_Anim_0004F8, &object_tite_Anim_00069C, NULL, NULL, NULL,
};

// Some kind of offset for the position of each tektite foot
static Vec3f sFootOffset = { 2800.0f, -200.0f, 0.0f };

// Relative positions to spawn ice chunks when tektite is frozen
static Vec3f sIceChunks[12] = {
    { 20.0f, 20.0f, 0.0f },   { 10.0f, 40.0f, 10.0f },   { -10.0f, 40.0f, 10.0f }, { -20.0f, 20.0f, 0.0f },
    { 10.0f, 40.0f, -10.0f }, { -10.0f, 40.0f, -10.0f }, { 0.0f, 20.0f, -20.0f },  { 10.0f, 0.0f, 10.0f },
    { 10.0f, 0.0f, -10.0f },  { 0.0f, 20.0f, 20.0f },    { -10.0f, 0.0f, 10.0f },  { -10.0f, 0.0f, -10.0f },
};

void EnTite_SetupAction(EnTite* self, EnTiteActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnTite_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnTite* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    thisx->targetMode = 3;
    Actor_SetScale(thisx, 0.01f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &object_tite_Skel_003A20, &object_tite_Anim_0012E4, self->jointTable,
                   self->morphTable, 25);
    ActorShape_Init(&thisx->shape, -200.0f, ActorShadow_DrawCircle, 70.0f);
    self->flipState = TEKTITE_INITIAL;
    thisx->colChkInfo.damageTable = sDamageTable;
    self->actionVar1 = 0;
    self->bodyBreak.val = BODYBREAK_STATUS_FINISHED;
    thisx->focus.pos = thisx->world.pos;
    thisx->focus.pos.y += 20.0f;
    thisx->colChkInfo.health = 2;
    thisx->colChkInfo.mass = MASS_HEAVY;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, thisx, &sJntSphInit, &self->colliderItem);
    self->unk_2DC = 0x1D;
    if (self->actor.params == TEKTITE_BLUE) {
        self->unk_2DC |= 0x40; // Don't use the actor engine's ripple spawning code
        thisx->colChkInfo.health = 4;
        thisx->naviEnemyId += 1;
    }
    EnTite_SetupIdle(self);
}

void EnTite_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnTite* self = THIS;
    EnEncount1* spawner;

    if (thisx->parent != NULL) {
        spawner = (EnEncount1*)thisx->parent;
        if (spawner->curNumSpawn > 0) {
            spawner->curNumSpawn--;
        }
        osSyncPrintf("\n\n");
        // "Number of simultaneous occurrences"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 同時発生数 ☆☆☆☆☆%d\n" VT_RST, spawner->curNumSpawn);
        osSyncPrintf("\n\n");
    }
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnTite_SetupIdle(EnTite* self) {
    Animation_MorphToLoop(&self->skelAnime, &object_tite_Anim_0012E4, 4.0f);
    self->action = TEKTITE_IDLE;
    self->vIdleTimer = Rand_S16Offset(15, 30);
    self->actor.speedXZ = 0.0f;
    EnTite_SetupAction(self, EnTite_Idle);
}

void EnTite_Idle(EnTite* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    if (self->actor.params == TEKTITE_BLUE) {
        if (self->actor.bgCheckFlags & 0x20) {
            // Float on water surface
            self->actor.gravity = 0.0f;
            Math_SmoothStepToF(&self->actor.velocity.y, 0.0f, 1.0f, 2.0f, 0.0f);
            Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.world.pos.y + self->actor.yDistToWater, 1.0f, 2.0f,
                               0.0f);
        } else {
            self->actor.gravity = -1.0f;
        }
    }
    if ((self->actor.bgCheckFlags & 3) && (self->actor.velocity.y <= 0.0f)) {
        self->actor.velocity.y = 0.0f;
    }
    if (self->vIdleTimer > 0) {
        self->vIdleTimer--;
    } else if ((self->actor.xzDistToPlayer < 300.0f) && (self->actor.yDistToPlayer <= 80.0f)) {
        EnTite_SetupTurnTowardPlayer(self);
    }
}

void EnTite_SetupAttack(EnTite* self) {
    Animation_PlayOnce(&self->skelAnime, &object_tite_Anim_00083C);
    self->action = TEKTITE_ATTACK;
    self->vAttackState = TEKTITE_BEGIN_LUNGE;
    self->vQueuedJumps = Rand_S16Offset(1, 3);
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnTite_SetupAction(self, EnTite_Attack);
}

void EnTite_Attack(EnTite* self, GlobalContext* globalCtx) {
    s16 angleToPlayer;
    s32 attackState;
    Vec3f ripplePos;

    if (SkelAnime_Update(&self->skelAnime) != 0) {
        attackState = self->vAttackState; // for deciding whether to change animation
        switch (self->vAttackState) {
            case TEKTITE_BEGIN_LUNGE:
                // Snap to ground or water, then lunge into the air with some initial speed
                self->vAttackState = TEKTITE_MID_LUNGE;
                if ((self->actor.params != TEKTITE_BLUE) || !(self->actor.bgCheckFlags & 0x20)) {
                    if (self->actor.floorHeight > BGCHECK_Y_MIN) {
                        self->actor.world.pos.y = self->actor.floorHeight;
                    }
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
                } else {
                    self->actor.world.pos.y += self->actor.yDistToWater;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_JUMP_WATER);
                }
                self->actor.velocity.y = 8.0f;
                self->actor.gravity = -1.0f;
                self->actor.speedXZ = 4.0f;
                break;
            case TEKTITE_MID_LUNGE:
                // Continue trajectory until tektite has negative velocity and has landed on ground/water surface
                // Snap to ground/water surface, or if falling fast dip into the water and slow fall speed
                self->actor.flags |= 0x1000000;
                if ((self->actor.bgCheckFlags & 3) ||
                    ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20))) {
                    if (self->actor.velocity.y <= 0.0f) {
                        self->vAttackState = TEKTITE_LANDED;
                        if ((self->actor.params != TEKTITE_BLUE) || !(self->actor.bgCheckFlags & 0x20)) {
                            if (BGCHECK_Y_MIN < self->actor.floorHeight) {
                                self->actor.world.pos.y = self->actor.floorHeight;
                            }
                            self->actor.velocity.y = 0.0f;
                            self->actor.speedXZ = 0.0f;
                        } else {
                            self->actor.gravity = 0.0f;
                            if (self->actor.velocity.y < -8.0f) {
                                ripplePos = self->actor.world.pos;
                                ripplePos.y += self->actor.yDistToWater;
                                self->vAttackState++; // TEKTITE_SUBMERGED
                                self->actor.velocity.y *= 0.75f;
                                attackState = self->vAttackState;
                                EffectSsGRipple_Spawn(globalCtx, &ripplePos, 0, 500, 0);
                            } else {
                                self->actor.velocity.y = 0.0f;
                                self->actor.speedXZ = 0.0f;
                            }
                        }
                        self->actor.world.rot.y = self->actor.shape.rot.y;
                    }
                }
                break;
            case TEKTITE_LANDED:
                // Get ready to begin another lunge if more lunges are queued, otherwise start turning
                if (self->vQueuedJumps != 0) {
                    self->vQueuedJumps--;
                    self->vAttackState = TEKTITE_BEGIN_LUNGE;
                    self->collider.base.atFlags &= ~AT_HIT;
                } else {
                    EnTite_SetupTurnTowardPlayer(self);
                }
                break;
            case TEKTITE_SUBMERGED:
                // Check if floated to surface
                if (self->actor.yDistToWater == 0.0f) {
                    self->vAttackState = TEKTITE_LANDED;
                    attackState = self->vAttackState;
                }
                break;
        }
        // If switching attack state, change animation (unless tektite is switching between submerged and landed)
        if (attackState != self->vAttackState) {
            Animation_PlayOnce(&self->skelAnime, D_80B1B634[self->vAttackState]);
        }
    }

    switch (self->vAttackState) {
        case TEKTITE_BEGIN_LUNGE:
            // Slightly turn to player and switch to turning/idling action if the player is too far
            Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 1000, 0);
            self->actor.shape.rot.y = self->actor.world.rot.y;
            angleToPlayer = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
            if ((self->actor.xzDistToPlayer > 300.0f) && (self->actor.yDistToPlayer > 80.0f)) {
                EnTite_SetupIdle(self);
            } else if (ABS(angleToPlayer) >= 9000) {
                EnTite_SetupTurnTowardPlayer(self);
            }
            break;
        case TEKTITE_MID_LUNGE:
            // Generate sparkles at feet upon landing, set jumping animation and hurtbox and check if hit player
            if (self->actor.velocity.y >= 5.0f) {
                if (self->actor.bgCheckFlags & 1) {
                    func_800355B8(globalCtx, &self->frontLeftFootPos);
                    func_800355B8(globalCtx, &self->frontRightFootPos);
                    func_800355B8(globalCtx, &self->backRightFootPos);
                    func_800355B8(globalCtx, &self->backLeftFootPos);
                }
            }
            if (!(self->collider.base.atFlags & AT_HIT) && (self->actor.flags & 0x40)) {
                CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            } else {
                Player* player = GET_PLAYER(globalCtx);
                self->collider.base.atFlags &= ~AT_HIT;
                Animation_MorphToLoop(&self->skelAnime, &object_tite_Anim_0012E4, 4.0f);
                self->actor.speedXZ = -6.0f;
                self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                if (&player->actor == self->collider.base.at) {
                    if (!(self->collider.base.atFlags & AT_BOUNCED)) {
                        Audio_PlayActorSound2(&player->actor, NA_SE_PL_BODY_HIT);
                    }
                }
                EnTite_SetupAction(self, EnTite_Recoil);
            }
            break;
        case TEKTITE_LANDED:
            // Slightly turn to player
            Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 1500, 0);
            break;
        case TEKTITE_SUBMERGED:
            // Float up to water surface
            Math_SmoothStepToF(&self->actor.velocity.y, 0.0f, 1.0f, 2.0f, 0.0f);
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
            Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.world.pos.y + self->actor.yDistToWater, 1.0f, 2.0f,
                               0.0f);
            break;
    }
    // Create ripples on water surface where tektite feet landed
    if (self->actor.bgCheckFlags & 2) {
        if (!(self->actor.bgCheckFlags & 0x20)) {
            func_80033480(globalCtx, &self->frontLeftFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->frontRightFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->backRightFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->backLeftFootPos, 1.0f, 2, 80, 15, 1);
        }
    }
    // if landed, kill XZ speed and play appropriate sounds
    if (self->actor.params == TEKTITE_BLUE) {
        if (self->actor.bgCheckFlags & 0x40) {
            self->actor.speedXZ = 0.0f;
            if (self->vAttackState == TEKTITE_SUBMERGED) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_LAND_WATER);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_LAND_WATER2);
            }
            self->actor.bgCheckFlags &= ~0x40;
        } else if (self->actor.bgCheckFlags & 2) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        }
    } else if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
    }
}

void EnTite_SetupTurnTowardPlayer(EnTite* self) {
    Animation_PlayLoop(&self->skelAnime, &object_tite_Anim_000A14);
    self->action = TEKTITE_TURN_TOWARD_PLAYER;
    if ((self->actor.bgCheckFlags & 3) || ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20))) {
        if (self->actor.velocity.y <= 0.0f) {
            self->actor.gravity = 0.0f;
            self->actor.velocity.y = 0.0f;
            self->actor.speedXZ = 0.0f;
        }
    }
    EnTite_SetupAction(self, EnTite_TurnTowardPlayer);
}

void EnTite_TurnTowardPlayer(EnTite* self, GlobalContext* globalCtx) {
    s16 angleToPlayer;
    s16 turnVelocity;

    if (((self->actor.bgCheckFlags & 3) ||
         ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20))) &&
        (self->actor.velocity.y <= 0.0f)) {
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->actor.speedXZ = 0.0f;
    }
    // Calculate turn velocity and animation speed based on angle to player
    if ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20)) {
        self->actor.world.pos.y += self->actor.yDistToWater;
    }
    angleToPlayer = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.world.rot.y;
    if (angleToPlayer > 0) {
        turnVelocity = (angleToPlayer / 42.0f) + 10.0f;
        self->actor.world.rot.y += (turnVelocity * 2);
    } else {
        turnVelocity = (angleToPlayer / 42.0f) - 10.0f;
        self->actor.world.rot.y += (turnVelocity * 2);
    }
    if (angleToPlayer > 0) {
        self->skelAnime.playSpeed = turnVelocity * 0.01f;
    } else {
        self->skelAnime.playSpeed = turnVelocity * 0.01f;
    }

    /**
     * Play sounds once every animation cycle
     */
    SkelAnime_Update(&self->skelAnime);
    if (((s16)self->skelAnime.curFrame & 7) == 0) {
        if ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_WALK_WATER);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_WALK);
        }
    }

    // Idle if player is far enough away from the tektite, move or attack if almost facing player
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if ((self->actor.xzDistToPlayer > 300.0f) && (self->actor.yDistToPlayer > 80.0f)) {
        EnTite_SetupIdle(self);
    } else if (Actor_IsFacingPlayer(&self->actor, 3640)) {
        if ((self->actor.xzDistToPlayer <= 180.0f) && (self->actor.yDistToPlayer <= 80.0f)) {
            EnTite_SetupAttack(self);
        } else {
            EnTite_SetupMoveTowardPlayer(self);
        }
    }
}

void EnTite_SetupMoveTowardPlayer(EnTite* self) {
    Animation_PlayLoop(&self->skelAnime, &object_tite_Anim_000C70);
    self->action = TEKTITE_MOVE_TOWARD_PLAYER;
    self->actor.velocity.y = 10.0f;
    self->actor.gravity = -1.0f;
    self->actor.speedXZ = 4.0f;
    self->vQueuedJumps = Rand_S16Offset(1, 3);
    if ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_JUMP_WATER);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    }
    EnTite_SetupAction(self, EnTite_MoveTowardPlayer);
}

/**
 *  Jumping toward player as a method of travel (different from attacking, has no hitbox)
 */
void EnTite_MoveTowardPlayer(EnTite* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 1.0f, 0.0f);
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.bgCheckFlags & 0x42) {
        if (!(self->actor.bgCheckFlags & 0x40)) {
            func_80033480(globalCtx, &self->frontLeftFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->frontRightFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->backRightFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->backLeftFootPos, 1.0f, 2, 80, 15, 1);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_LAND_WATER);
        }
    }

    if ((self->actor.bgCheckFlags & 2) || ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x40))) {
        if (self->vQueuedJumps != 0) {
            self->vQueuedJumps--;
        } else {
            EnTite_SetupIdle(self);
        }
    }

    if (((self->actor.bgCheckFlags & 3) || (self->actor.params == TEKTITE_BLUE && (self->actor.bgCheckFlags & 0x60))) &&
        (self->actor.velocity.y <= 0.0f)) {
        // slightly turn toward player upon landing and snap to ground or water.
        self->actor.speedXZ = 0.0f;
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 4000, 0);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if ((self->actor.params != TEKTITE_BLUE) || !(self->actor.bgCheckFlags & 0x20)) {
            if (self->actor.floorHeight > BGCHECK_Y_MIN) {
                self->actor.world.pos.y = self->actor.floorHeight;
            }
        } else if (self->actor.bgCheckFlags & 0x40) {
            Vec3f ripplePos = self->actor.world.pos;
            self->actor.bgCheckFlags &= ~0x40;
            ripplePos.y += self->actor.yDistToWater;
            self->actor.gravity = 0.0f;
            self->actor.velocity.y *= 0.75f;
            EffectSsGRipple_Spawn(globalCtx, &ripplePos, 0, 500, 0);
            return;
        } else {
            // If submerged, float to surface
            Math_SmoothStepToF(&self->actor.velocity.y, 0.0f, 1.0f, 2.0f, 0.0f);
            Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.world.pos.y + self->actor.yDistToWater, 1.0f, 2.0f,
                               0.0f);
            if (self->actor.yDistToWater != 0.0f) {
                // Do not change state until tekite has floated to surface
                return;
            }
        }

        // Idle or turn if player is too far away, otherwise keep jumping
        if (((self->actor.xzDistToPlayer > 300.0f) && (self->actor.yDistToPlayer > 80.0f))) {
            EnTite_SetupIdle(self);
        } else if (((self->actor.xzDistToPlayer <= 180.0f)) && ((self->actor.yDistToPlayer <= 80.0f))) {
            if (self->vQueuedJumps <= 0) {
                EnTite_SetupTurnTowardPlayer(self);
            } else {
                self->actor.velocity.y = 10.0f;
                self->actor.speedXZ = 4.0f;
                self->actor.flags |= 0x1000000;
                self->actor.gravity = -1.0f;
                if ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20)) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_JUMP_WATER);
                } else {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
                }
            }
        } else {
            self->actor.velocity.y = 10.0f;
            self->actor.speedXZ = 4.0f;
            self->actor.flags |= 0x1000000;
            self->actor.gravity = -1.0f;
            if ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_JUMP_WATER);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
            }
        }
        // If in midair:
    } else {
        // Turn slowly toward player
        self->actor.flags |= 0x1000000;
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 1000, 0);
        if (self->actor.velocity.y >= 6.0f) {
            if (self->actor.bgCheckFlags & 1) {
                func_800355B8(globalCtx, &self->frontLeftFootPos);
                func_800355B8(globalCtx, &self->frontRightFootPos);
                func_800355B8(globalCtx, &self->backRightFootPos);
                func_800355B8(globalCtx, &self->backLeftFootPos);
            }
        }
    }
}

void EnTite_SetupRecoil(EnTite* self) {
    self->action = TEKTITE_RECOIL;
    Animation_MorphToLoop(&self->skelAnime, &object_tite_Anim_0012E4, 4.0f);
    self->actor.speedXZ = -6.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->actor.gravity = -1.0f;
    EnTite_SetupAction(self, EnTite_Recoil);
}

/**
 * After tektite hits or gets hit, recoils backwards and slides a bit upon landing
 */
void EnTite_Recoil(EnTite* self, GlobalContext* globalCtx) {
    s16 angleToPlayer;

    // Snap to ground or water surface upon landing
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    if (((self->actor.bgCheckFlags & 3) || (self->actor.params == TEKTITE_BLUE && (self->actor.bgCheckFlags & 0x20))) &&
        (self->actor.velocity.y <= 0.0f)) {
        if ((self->actor.params != TEKTITE_BLUE) || !(self->actor.bgCheckFlags & 0x20)) {
            if (self->actor.floorHeight > BGCHECK_Y_MIN) {
                self->actor.world.pos.y = self->actor.floorHeight;
            }
        } else {
            self->actor.velocity.y = 0.0f;
            self->actor.gravity = 0.0f;
            self->actor.world.pos.y += self->actor.yDistToWater;
        }
    }

    // play sound and generate ripples
    if (self->actor.bgCheckFlags & 0x42) {
        if (!(self->actor.bgCheckFlags & 0x40)) {
            func_80033480(globalCtx, &self->frontLeftFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->frontRightFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->backRightFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->backLeftFootPos, 1.0f, 2, 80, 15, 1);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        } else {
            self->actor.bgCheckFlags &= ~0x40;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_LAND_WATER2);
        }
    }

    // If player is far away, idle. Otherwise attack or move
    angleToPlayer = (self->actor.yawTowardsPlayer - self->actor.shape.rot.y);
    if ((self->actor.speedXZ == 0.0f) && ((self->actor.bgCheckFlags & 1) || ((self->actor.params == TEKTITE_BLUE) &&
                                                                             (self->actor.bgCheckFlags & 0x20)))) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->collider.base.atFlags &= ~AT_HIT;
        if ((self->actor.xzDistToPlayer > 300.0f) && (self->actor.yDistToPlayer > 80.0f) &&
            (ABS(self->actor.shape.rot.x) < 4000) && (ABS(self->actor.shape.rot.z) < 4000) &&
            ((self->actor.bgCheckFlags & 1) ||
             ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20)))) {
            EnTite_SetupIdle(self);
        } else if ((self->actor.xzDistToPlayer < 180.0f) && (self->actor.yDistToPlayer <= 80.0f) &&
                   (ABS(angleToPlayer) <= 6000)) {
            EnTite_SetupAttack(self);
        } else {
            EnTite_SetupMoveTowardPlayer(self);
        }
    }
    SkelAnime_Update(&self->skelAnime);
}

void EnTite_SetupStunned(EnTite* self) {
    Animation_Change(&self->skelAnime, &object_tite_Anim_0012E4, 0.0f, 0.0f,
                     (f32)Animation_GetLastFrame(&object_tite_Anim_0012E4), ANIMMODE_LOOP, 4.0f);
    self->action = TEKTITE_STUNNED;
    self->actor.speedXZ = -6.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    if (self->damageEffect == 0xF) {
        self->spawnIceTimer = 48;
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnTite_SetupAction(self, EnTite_Stunned);
}

/**
 * stunned or frozen
 */
void EnTite_Stunned(EnTite* self, GlobalContext* globalCtx) {
    s16 angleToPlayer;

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    // Snap to ground or water
    if (((self->actor.bgCheckFlags & 3) ||
         ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20))) &&
        (self->actor.velocity.y <= 0.0f)) {
        if (((self->actor.params != TEKTITE_BLUE) || !(self->actor.bgCheckFlags & 0x20))) {
            if (self->actor.floorHeight > BGCHECK_Y_MIN) {
                self->actor.world.pos.y = self->actor.floorHeight;
            }
        } else {
            self->actor.velocity.y = 0.0f;
            self->actor.gravity = 0.0f;
            self->actor.world.pos.y += self->actor.yDistToWater;
        }
    }
    // Play sounds and spawn dirt effects upon landing
    if (self->actor.bgCheckFlags & 0x42) {
        if (!(self->actor.bgCheckFlags & 0x40)) {
            func_80033480(globalCtx, &self->frontLeftFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->frontRightFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->backRightFootPos, 1.0f, 2, 80, 15, 1);
            func_80033480(globalCtx, &self->backLeftFootPos, 1.0f, 2, 80, 15, 1);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        } else {
            self->actor.bgCheckFlags &= ~0x40;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_LAND_WATER2);
        }
    }
    // Decide on next action based on health, flip state and player distance
    angleToPlayer = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (((self->actor.colorFilterTimer == 0) && (self->actor.speedXZ == 0.0f)) &&
        ((self->actor.bgCheckFlags & 1) ||
         ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20)))) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (self->actor.colChkInfo.health == 0) {
            EnTite_SetupDeathCry(self);
        } else if (self->flipState == TEKTITE_FLIPPED) {
            EnTite_SetupFlipUpright(self);
        } else if (((self->actor.xzDistToPlayer > 300.0f) && (self->actor.yDistToPlayer > 80.0f) &&
                    (ABS(self->actor.shape.rot.x) < 4000) && (ABS(self->actor.shape.rot.z) < 4000)) &&
                   ((self->actor.bgCheckFlags & 1) ||
                    ((self->actor.params == TEKTITE_BLUE) && (self->actor.bgCheckFlags & 0x20)))) {
            EnTite_SetupIdle(self);
        } else if ((self->actor.xzDistToPlayer < 180.0f) && (self->actor.yDistToPlayer <= 80.0f) &&
                   (ABS(angleToPlayer) <= 6000)) {
            EnTite_SetupAttack(self);
        } else {
            EnTite_SetupMoveTowardPlayer(self);
        }
    }
    SkelAnime_Update(&self->skelAnime);
}

void EnTite_SetupDeathCry(EnTite* self) {
    self->action = TEKTITE_DEATH_CRY;
    self->actor.colorFilterTimer = 0;
    self->actor.speedXZ = 0.0f;
    EnTite_SetupAction(self, EnTite_DeathCry);
}

/**
 * First frame of death. Scream in pain and allocate memory for EnPart data
 */
void EnTite_DeathCry(EnTite* self, GlobalContext* globalCtx) {
    EffectSsDeadSound_SpawnStationary(globalCtx, &self->actor.projectedPos, NA_SE_EN_TEKU_DEAD, true,
                                      DEADSOUND_REPEAT_MODE_OFF, 40);
    self->action = TEKTITE_FALL_APART;
    EnTite_SetupAction(self, EnTite_FallApart);
    BodyBreak_Alloc(&self->bodyBreak, 24, globalCtx);
}

/**
 * Spawn EnPart and drop items
 */
void EnTite_FallApart(EnTite* self, GlobalContext* globalCtx) {
    if (BodyBreak_SpawnParts(&self->actor, &self->bodyBreak, globalCtx, self->actor.params + 0xB)) {
        if (self->actor.params == TEKTITE_BLUE) {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xE0);
        } else {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x40);
        }
        Actor_Kill(&self->actor);
    }
}

void EnTite_SetupFlipOnBack(EnTite* self) {

    Animation_PlayLoopSetSpeed(&self->skelAnime, &object_tite_Anim_000A14, 1.5f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_REVERSE);
    self->flipState = TEKTITE_FLIPPED;
    self->vOnBackTimer = 500;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = -1.0f;
    self->vLegTwitchTimer = (Rand_ZeroOne() * 50.0f);
    self->actor.velocity.y = 11.0f;
    EnTite_SetupAction(self, EnTite_FlipOnBack);
}

/**
 * During the flip animation and also while idling on back
 */
void EnTite_FlipOnBack(EnTite* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.z, 0x7FFF, 1, 4000, 0);
    // randomly reset the leg wiggling animation whenever timer reaches 0 to give illusion of twitching legs
    self->vLegTwitchTimer--;
    if (self->vLegTwitchTimer == 0) {
        self->vLegTwitchTimer = Rand_ZeroOne() * 30.0f;
        self->skelAnime.curFrame = Rand_ZeroOne() * 5.0f;
    }
    SkelAnime_Update(&self->skelAnime);
    if (self->actor.bgCheckFlags & 3) {
        // Upon landing, spawn dust and make noise
        if (self->actor.bgCheckFlags & 2) {
            Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 20.0f, 0xB, 4.0f, 0, 0, 0);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        }
        self->vOnBackTimer--;
        if (self->vOnBackTimer == 0) {
            EnTite_SetupFlipUpright(self);
        }
    } else {
        // Gradually increase y offset during flip so that the actor position is at tektite's back instead of feet
        if (self->actor.shape.yOffset < 2800.0f) {
            self->actor.shape.yOffset += 400.0f;
        }
    }
}

void EnTite_SetupFlipUpright(EnTite* self) {
    self->flipState = TEKTITE_UNFLIPPED;
    self->actionVar1 = 1000; // value unused here and overwritten in SetupIdle
    //! @bug flying tektite: water sets gravity to 0 so y velocity will never decrease from 13
    self->actor.velocity.y = 13.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_TEKU_REVERSE);
    EnTite_SetupAction(self, EnTite_FlipUpright);
}

void EnTite_FlipUpright(EnTite* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.z, 0, 1, 0xFA0, 0);
    SkelAnime_Update(&self->skelAnime);
    //! @bug flying tektite: the following condition is never met and tektite stays stuck in self action forever
    if (self->actor.bgCheckFlags & 2) {
        func_80033480(globalCtx, &self->frontLeftFootPos, 1.0f, 2, 80, 15, 1);
        func_80033480(globalCtx, &self->frontRightFootPos, 1.0f, 2, 80, 15, 1);
        func_80033480(globalCtx, &self->backRightFootPos, 1.0f, 2, 80, 15, 1);
        func_80033480(globalCtx, &self->backLeftFootPos, 1.0f, 2, 80, 15, 1);
        self->actor.shape.yOffset = 0.0f;
        self->actor.world.pos.y = self->actor.floorHeight;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        EnTite_SetupIdle(self);
    }
}

void EnTite_CheckDamage(Actor* thisx, GlobalContext* globalCtx) {
    EnTite* self = THIS;

    if ((self->collider.base.acFlags & AC_HIT) && (self->action >= TEKTITE_IDLE)) {
        self->collider.base.acFlags &= ~AC_HIT;
        if (thisx->colChkInfo.damageEffect != 0xE) { // Immune to fire magic
            self->damageEffect = thisx->colChkInfo.damageEffect;
            Actor_SetDropFlag(thisx, &self->collider.elements[0].info, 0);
            // Stun if Tektite hit by nut, boomerang, hookshot, ice arrow or ice magic
            if ((thisx->colChkInfo.damageEffect == 1) || (thisx->colChkInfo.damageEffect == 0xF)) {
                if (self->action != TEKTITE_STUNNED) {
                    Actor_SetColorFilter(thisx, 0, 0x78, 0, 0x50);
                    Actor_ApplyDamage(thisx);
                    EnTite_SetupStunned(self);
                }
                // Otherwise apply damage and handle death where necessary
            } else {
                if ((thisx->colorFilterTimer == 0) || ((thisx->colorFilterParams & 0x4000) == 0)) {
                    Actor_SetColorFilter(thisx, 0x4000, 0xFF, 0, 8);
                    Actor_ApplyDamage(thisx);
                }
                if (thisx->colChkInfo.health == 0) {
                    EnTite_SetupDeathCry(self);
                } else {
                    // Flip tektite back up if it's on its back
                    Audio_PlayActorSound2(thisx, NA_SE_EN_TEKU_DAMAGE);
                    if (self->flipState != TEKTITE_FLIPPED) {
                        EnTite_SetupRecoil(self);
                    } else {
                        EnTite_SetupFlipUpright(self);
                    }
                }
            }
        }
        // If hammer has recently hit the floor and player is close to tektite, flip over
    } else if ((thisx->colChkInfo.health != 0) && (globalCtx->actorCtx.unk_02 != 0) &&
               (thisx->xzDistToPlayer <= 400.0f) && (thisx->bgCheckFlags & 1)) {
        if (self->flipState == TEKTITE_FLIPPED) {
            EnTite_SetupFlipUpright(self);
        } else if ((self->action >= TEKTITE_IDLE) || (self->action >= TEKTITE_IDLE)) {
            if (1) {}
            EnTite_SetupFlipOnBack(self);
        }
    }
}

void EnTite_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnTite* self = THIS;
    char pad[0x4];
    CollisionPoly* floorPoly;
    WaterBox* waterBox;
    f32 waterSurfaceY;

    EnTite_CheckDamage(thisx, globalCtx);
    // Stay still if hit by immunity damage type self frame
    if (thisx->colChkInfo.damageEffect != 0xE) {
        self->actionFunc(self, globalCtx);
        Actor_MoveForward(thisx);
        Actor_UpdateBgCheckInfo(globalCtx, thisx, 25.0f, 40.0f, 20.0f, self->unk_2DC);
        // If on water, snap feet to surface and spawn ripples
        if ((self->actor.params == TEKTITE_BLUE) && (thisx->bgCheckFlags & 0x20)) {
            floorPoly = thisx->floorPoly;
            if ((((globalCtx->gameplayFrames % 8) == 0) || (thisx->velocity.y < 0.0f)) &&
                (WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->backRightFootPos.x,
                                         self->backRightFootPos.z, &waterSurfaceY, &waterBox)) &&
                (self->backRightFootPos.y <= waterSurfaceY)) {
                self->backRightFootPos.y = waterSurfaceY;
                EffectSsGRipple_Spawn(globalCtx, &self->backRightFootPos, 0, 220, 0);
            }
            if (((((globalCtx->gameplayFrames + 2) % 8) == 0) || (thisx->velocity.y < 0.0f)) &&
                (WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->backLeftFootPos.x,
                                         self->backLeftFootPos.z, &waterSurfaceY, &waterBox)) &&
                (self->backLeftFootPos.y <= waterSurfaceY)) {
                self->backLeftFootPos.y = waterSurfaceY;
                EffectSsGRipple_Spawn(globalCtx, &self->backLeftFootPos, 0, 220, 0);
            }
            if (((((globalCtx->gameplayFrames + 4) % 8) == 0) || (thisx->velocity.y < 0.0f)) &&
                (WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->frontLeftFootPos.x,
                                         self->frontLeftFootPos.z, &waterSurfaceY, &waterBox)) &&
                (self->frontLeftFootPos.y <= waterSurfaceY)) {
                self->frontLeftFootPos.y = waterSurfaceY;
                EffectSsGRipple_Spawn(globalCtx, &self->frontLeftFootPos, 0, 220, 0);
            }
            if (((((globalCtx->gameplayFrames + 1) % 8) == 0) || (thisx->velocity.y < 0.0f)) &&
                (WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->frontRightFootPos.x,
                                         self->frontRightFootPos.z, &waterSurfaceY, &waterBox)) &&
                (self->frontRightFootPos.y <= waterSurfaceY)) {
                self->frontRightFootPos.y = waterSurfaceY;
                EffectSsGRipple_Spawn(globalCtx, &self->frontRightFootPos, 0, 220, 0);
            }
            thisx->floorPoly = floorPoly;
        }

        // If on ground and currently flipped over, set tektite to be fully upside-down
        if (thisx->bgCheckFlags & 3) {
            func_800359B8(thisx, thisx->shape.rot.y, &thisx->shape.rot);
            if (self->flipState >= TEKTITE_FLIPPED) {
                thisx->shape.rot.z += 0x7FFF;
            }
            // Otherwise ensure the tektite is rotating back upright
        } else {
            Math_SmoothStepToS(&thisx->shape.rot.x, 0, 1, 1000, 0);
            if (self->flipState <= TEKTITE_UNFLIPPED) {
                Math_SmoothStepToS(&thisx->shape.rot.z, 0, 1, 1000, 0);
                if (thisx->shape.yOffset > 0) {
                    thisx->shape.yOffset -= 400.0f;
                }
            }
        }
    }
    thisx->focus.pos = thisx->world.pos;
    thisx->focus.pos.y += 20.0f;

    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnTite_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** limbDList, Vec3s* rot, void* thisx) {
    EnTite* self = THIS;

    switch (limbIndex) {
        case 8:
            Matrix_MultVec3f(&sFootOffset, &self->backRightFootPos);
            break;
        case 13:
            Matrix_MultVec3f(&sFootOffset, &self->frontRightFootPos);
            break;
        case 18:
            Matrix_MultVec3f(&sFootOffset, &self->backLeftFootPos);
            break;
        case 23:
            Matrix_MultVec3f(&sFootOffset, &self->frontLeftFootPos);
            break;
    }

    BodyBreak_SetInfo(&self->bodyBreak, limbIndex, 0, 24, 24, limbDList, BODYBREAK_OBJECT_DEFAULT);
}

void EnTite_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnTite* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tite.c", 1704);
    func_80093D18(globalCtx->state.gfxCtx);
    Collider_UpdateSpheres(0, &self->collider);
    if (self->actor.params == TEKTITE_BLUE) {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(object_tite_Tex_001300));
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(object_tite_Tex_001700));
        gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(object_tite_Tex_001900));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(object_tite_Tex_001B00));
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(object_tite_Tex_001F00));
        gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(object_tite_Tex_002100));
    }
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, EnTite_PostLimbDraw,
                      thisx);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tite.c", 1735);

    if (self->spawnIceTimer != 0) {
        // Spawn chunks of ice all over the tektite's body
        thisx->colorFilterTimer++;
        self->spawnIceTimer--;
        if ((self->spawnIceTimer & 3) == 0) {
            Vec3f iceChunk;
            s32 idx = self->spawnIceTimer >> 2;
            iceChunk.x = thisx->world.pos.x + sIceChunks[idx].x;
            iceChunk.y = thisx->world.pos.y + sIceChunks[idx].y;
            iceChunk.z = thisx->world.pos.z + sIceChunks[idx].z;
            EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &iceChunk, 150, 150, 150, 250, 235, 245, 255, 1.0f);
        }
    }
}
