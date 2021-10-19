/*
 * File: z_en_bili.c
 * Overlay: ovl_En_Bili
 * Description: Biri (small jellyfish-like enemy)
 */

#include "z_en_bili.h"
#include "objects/object_bl/object_bl.h"

#define FLAGS 0x00005005

#define THIS ((EnBili*)thisx)

void EnBili_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBili_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBili_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBili_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnBili_SetupFloatIdle(EnBili* self);
void EnBili_SetupSpawnedFlyApart(EnBili* self);
void EnBili_FloatIdle(EnBili* self, GlobalContext* globalCtx);
void EnBili_SpawnedFlyApart(EnBili* self, GlobalContext* globalCtx);
void EnBili_DischargeLightning(EnBili* self, GlobalContext* globalCtx);
void EnBili_Climb(EnBili* self, GlobalContext* globalCtx);
void EnBili_ApproachPlayer(EnBili* self, GlobalContext* globalCtx);
void EnBili_SetNewHomeHeight(EnBili* self, GlobalContext* globalCtx);
void EnBili_Recoil(EnBili* self, GlobalContext* globalCtx);
void EnBili_Burnt(EnBili* self, GlobalContext* globalCtx);
void EnBili_Die(EnBili* self, GlobalContext* globalCtx);
void EnBili_Stunned(EnBili* self, GlobalContext* globalCtx);
void EnBili_Frozen(EnBili* self, GlobalContext* globalCtx);

const ActorInit En_Bili_InitVars = {
    ACTOR_EN_BILI,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BL,
    sizeof(EnBili),
    (ActorFunc)EnBili_Init,
    (ActorFunc)EnBili_Destroy,
    (ActorFunc)EnBili_Update,
    (ActorFunc)EnBili_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT8,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x03, 0x08 },
        { 0xFFCFFFFF, 0x01, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 9, 28, -20, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 1, 9, 28, -20, 30 };

typedef enum {
    /* 0x0 */ BIRI_DMGEFF_NONE,
    /* 0x1 */ BIRI_DMGEFF_DEKUNUT,
    /* 0x2 */ BIRI_DMGEFF_FIRE,
    /* 0x3 */ BIRI_DMGEFF_ICE,
    /* 0xE */ BIRI_DMGEFF_SLINGSHOT = 0xE,
    /* 0xF */ BIRI_DMGEFF_SWORD
} BiriDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, BIRI_DMGEFF_DEKUNUT),
    /* Deku stick    */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(0, BIRI_DMGEFF_SLINGSHOT),
    /* Explosive     */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(1, BIRI_DMGEFF_NONE),
    /* Normal arrow  */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Hookshot      */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Kokiri sword  */ DMG_ENTRY(1, BIRI_DMGEFF_SWORD),
    /* Master sword  */ DMG_ENTRY(2, BIRI_DMGEFF_SWORD),
    /* Giant's Knife */ DMG_ENTRY(4, BIRI_DMGEFF_SWORD),
    /* Fire arrow    */ DMG_ENTRY(4, BIRI_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(4, BIRI_DMGEFF_ICE),
    /* Light arrow   */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(4, BIRI_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(4, BIRI_DMGEFF_ICE),
    /* Light magic   */ DMG_ENTRY(0, BIRI_DMGEFF_NONE),
    /* Shield        */ DMG_ENTRY(0, BIRI_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, BIRI_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, BIRI_DMGEFF_NONE),
    /* Giant spin    */ DMG_ENTRY(4, BIRI_DMGEFF_NONE),
    /* Master spin   */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Kokiri jump   */ DMG_ENTRY(2, BIRI_DMGEFF_NONE),
    /* Giant jump    */ DMG_ENTRY(8, BIRI_DMGEFF_NONE),
    /* Master jump   */ DMG_ENTRY(4, BIRI_DMGEFF_NONE),
    /* Unknown 1     */ DMG_ENTRY(0, BIRI_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, BIRI_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, BIRI_DMGEFF_NONE),
    /* Unknown 2     */ DMG_ENTRY(0, BIRI_DMGEFF_NONE),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x17, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_STOP),
};

void EnBili_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBili* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 17.0f);
    self->actor.shape.shadowAlpha = 155;
    SkelAnime_Init(globalCtx, &self->skelAnime, &gBiriSkel, &gBiriDefaultAnim, self->jointTable, self->morphTable,
                   EN_BILI_LIMB_MAX);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    self->playFlySound = false;

    if (self->actor.params == EN_BILI_TYPE_NORMAL) {
        EnBili_SetupFloatIdle(self);
    } else {
        EnBili_SetupSpawnedFlyApart(self);
    }
}

void EnBili_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBili* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

// Setup Action Functions

void EnBili_SetupFloatIdle(EnBili* self) {
    self->actor.speedXZ = 0.7f;
    self->collider.info.bumper.effect = 1; // Shock?
    self->timer = 32;
    self->collider.base.atFlags |= AT_ON;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = EnBili_FloatIdle;
    self->actor.home.pos.y = self->actor.world.pos.y;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
}

/**
 * Separates the Biri spawned by a dying EnVali.
 */
void EnBili_SetupSpawnedFlyApart(EnBili* self) {
    Animation_PlayLoop(&self->skelAnime, &gBiriDefaultAnim);
    self->timer = 25;
    self->actor.velocity.y = 6.0f;
    self->actor.gravity = -0.3f;
    self->collider.base.atFlags &= ~AT_ON;
    self->actionFunc = EnBili_SpawnedFlyApart;
    self->actor.speedXZ = 3.0f;
}

/**
 * Used for both touching player/player's shield and being hit with sword. What to do next is determined by params.
 */
void EnBili_SetupDischargeLightning(EnBili* self) {
    Animation_PlayLoop(&self->skelAnime, &gBiriDischargeLightningAnim);
    self->timer = 10;
    self->actionFunc = EnBili_DischargeLightning;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = -1.0f;
}

void EnBili_SetupClimb(EnBili* self) {
    Animation_PlayOnce(&self->skelAnime, &gBiriClimbAnim);
    self->collider.base.atFlags &= ~AT_ON;
    self->actionFunc = EnBili_Climb;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
}

void EnBili_SetupApproachPlayer(EnBili* self) {
    self->actor.speedXZ = 1.2f;
    self->actionFunc = EnBili_ApproachPlayer;
}

void EnBili_SetupSetNewHomeHeight(EnBili* self) {
    Animation_PlayLoop(&self->skelAnime, &gBiriDefaultAnim);
    self->timer = 96;
    self->actor.speedXZ = 0.9f;
    self->collider.base.atFlags |= AT_ON;
    self->actionFunc = EnBili_SetNewHomeHeight;
    self->actor.home.pos.y = self->actor.world.pos.y;
}

void EnBili_SetupRecoil(EnBili* self) {
    if (self->skelAnime.animation != &gBiriDefaultAnim) {
        Animation_PlayLoop(&self->skelAnime, &gBiriDefaultAnim);
    }

    self->actor.world.rot.y = Actor_WorldYawTowardPoint(&self->actor, &self->collider.base.ac->prevPos) + 0x8000;
    self->actor.world.rot.x = Actor_WorldPitchTowardPoint(&self->actor, &self->collider.base.ac->prevPos);
    self->actionFunc = EnBili_Recoil;
    self->actor.speedXZ = 5.0f;
}

/**
 * Used for both fire damage and generic damage
 */
void EnBili_SetupBurnt(EnBili* self) {
    if (self->actionFunc == EnBili_Climb) {
        Animation_PlayLoop(&self->skelAnime, &gBiriDefaultAnim);
    }

    self->timer = 20;
    self->collider.base.atFlags &= ~AT_ON;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.flags |= 0x10;
    self->actor.speedXZ = 0.0f;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xC8, 0x2000, 0x14);
    self->actionFunc = EnBili_Burnt;
}

void EnBili_SetupDie(EnBili* self) {
    self->timer = 18;
    self->actor.flags &= ~1;
    self->actionFunc = EnBili_Die;
    self->actor.speedXZ = 0.0f;
}

/**
 * Falls to ground
 */
void EnBili_SetupStunned(EnBili* self) {
    self->timer = 80;
    self->collider.info.bumper.effect = 0;
    self->actor.gravity = -1.0f;
    self->actor.speedXZ = 0.0f;
    Actor_SetColorFilter(&self->actor, 0, 0x96, 0x2000, 0x50);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->collider.base.atFlags &= ~AT_ON;
    self->actionFunc = EnBili_Stunned;
}

void EnBili_SetupFrozen(EnBili* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3f effectPos;

    if (!(self->actor.flags & 0x8000)) {
        self->actor.gravity = -1.0f;
    }

    self->actor.velocity.y = 0.0f;
    effectPos.y = self->actor.world.pos.y - 15.0f;

    for (i = 0; i < 8; i++) {

        effectPos.x = self->actor.world.pos.x + ((i & 1) ? 7.0f : -7.0f);
        effectPos.y += 2.5f;
        effectPos.z = self->actor.world.pos.z + ((i & 4) ? 7.0f : -7.0f);

        EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &effectPos, 150, 150, 150, 250, 235, 245, 255,
                                       (Rand_ZeroOne() * 0.2f) + 0.7f);
    }

    self->actor.speedXZ = 0.0f;
    Actor_SetColorFilter(&self->actor, 0, 0x96, 0x2000, 0xA);
    self->collider.base.atFlags &= ~AT_ON;
    self->collider.base.acFlags &= ~AC_ON;
    self->timer = 300;
    self->actionFunc = EnBili_Frozen;
}

// Miscellaneous

/**
 * Changes the texture displayed on the oral arms limb using the current frame.
 */
void EnBili_UpdateTentaclesIndex(EnBili* self) {
    s16 curFrame = self->skelAnime.curFrame;
    s16 temp; // Not strictly necessary, but avoids a few s16 casts

    if (self->actionFunc == EnBili_DischargeLightning) {
        temp = 3 - curFrame;
        self->tentaclesTexIndex = (ABS(temp) + 5) % 8;
    } else if (self->actionFunc == EnBili_Climb) {
        if (curFrame <= 9) {
            temp = curFrame >> 1;
            self->tentaclesTexIndex = CLAMP_MAX(temp, 3);
        } else if (curFrame <= 18) {
            temp = 17 - curFrame;
            self->tentaclesTexIndex = CLAMP_MIN(temp, 0) >> 1;
        } else if (curFrame <= 36) {
            self->tentaclesTexIndex = ((36 - curFrame) / 3) + 2;
        } else {
            self->tentaclesTexIndex = (40 - curFrame) >> 1;
        }
    } else {
        self->tentaclesTexIndex = curFrame >> 1;
    }
}

/**
 * Tracks Player height, with oscillation, and moves away from walls
 */
void EnBili_UpdateFloating(EnBili* self) {
    f32 playerHeight = self->actor.world.pos.y + self->actor.yDistToPlayer;
    f32 heightOffset = ((self->actionFunc == EnBili_SetNewHomeHeight) ? 100.0f : 40.0f);
    f32 baseHeight = CLAMP_MIN(self->actor.floorHeight, playerHeight);

    Math_StepToF(&self->actor.home.pos.y, baseHeight + heightOffset, 1.0f);
    self->actor.world.pos.y = self->actor.home.pos.y + (sinf(self->timer * (M_PI / 16)) * 3.0f);

    // Turn around if touching wall
    if (self->actor.bgCheckFlags & 8) {
        self->actor.world.rot.y = self->actor.wallYaw;
    }
}

// Action functions

void EnBili_FloatIdle(EnBili* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->timer != 0) {
        self->timer--;
    }

    if (!(self->timer % 4)) {
        self->actor.world.rot.y += Rand_CenteredFloat(1820.0f);
    }

    EnBili_UpdateFloating(self);

    if (self->timer == 0) {
        self->timer = 32;
    }

    if ((self->actor.xzDistToPlayer < 160.0f) && (fabsf(self->actor.yDistToPlayer) < 45.0f)) {
        EnBili_SetupApproachPlayer(self);
    }
}

void EnBili_SpawnedFlyApart(EnBili* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        EnBili_SetupFloatIdle(self);
    }
}

void EnBili_DischargeLightning(EnBili* self, GlobalContext* globalCtx) {
    static Color_RGBA8 primColor = { 255, 255, 255, 255 };
    static Color_RGBA8 envColor = { 200, 255, 255, 255 };
    s32 i;
    Vec3f effectPos;
    s16 effectYaw;

    for (i = 0; i < 4; i++) {
        if (!((self->timer + (i << 1)) % 4)) {
            effectYaw = (s16)Rand_CenteredFloat(12288.0f) + (i * 0x4000) + 0x2000;
            effectPos.x = Rand_CenteredFloat(5.0f) + self->actor.world.pos.x;
            effectPos.y = (Rand_ZeroOne() * 5.0f) + self->actor.world.pos.y + 2.5f;
            effectPos.z = Rand_CenteredFloat(5.0f) + self->actor.world.pos.z;
            EffectSsLightning_Spawn(globalCtx, &effectPos, &primColor, &envColor, 15, effectYaw, 6, 2);
        }
    }

    SkelAnime_Update(&self->skelAnime);
    func_8002F974(&self->actor, NA_SE_EN_BIRI_SPARK - SFX_FLAG);

    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.velocity.y *= -1.0f;

    if ((self->timer == 0) && Animation_OnFrame(&self->skelAnime, 0.0f)) {
        if (self->actor.params == EN_BILI_TYPE_DYING) {
            EnBili_SetupDie(self);
        } else {
            EnBili_SetupClimb(self);
        }
    }
}

void EnBili_Climb(EnBili* self, GlobalContext* globalCtx) {
    s32 skelAnimeUpdate = SkelAnime_Update(&self->skelAnime);
    f32 curFrame = self->skelAnime.curFrame;

    if (Animation_OnFrame(&self->skelAnime, 9.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BIRI_JUMP);
    }

    if (curFrame > 9.0f) {
        Math_ApproachF(&self->actor.world.pos.y, self->actor.world.pos.y + self->actor.yDistToPlayer + 100.0f, 0.5f,
                       5.0f);
    }

    if (skelAnimeUpdate) {
        EnBili_SetupSetNewHomeHeight(self);
    }
}

void EnBili_ApproachPlayer(EnBili* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ApproachS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2, 1820);

    if (self->timer != 0) {
        self->timer--;
    }

    EnBili_UpdateFloating(self);

    if (self->timer == 0) {
        self->timer = 32;
    }

    if (self->actor.xzDistToPlayer > 200.0f) {
        EnBili_SetupFloatIdle(self);
    }
}

void EnBili_SetNewHomeHeight(EnBili* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->timer != 0) {
        self->timer--;
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, (s16)(self->actor.yawTowardsPlayer + 0x8000), 910);
    EnBili_UpdateFloating(self);

    if (self->timer == 0) {
        EnBili_SetupFloatIdle(self);
    }
}

void EnBili_Recoil(EnBili* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (Math_StepToF(&self->actor.speedXZ, 0.0f, 0.3f)) {
        self->actor.world.rot.y += 0x8000;
        EnBili_SetupFloatIdle(self);
    }
}

void EnBili_Burnt(EnBili* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.flags & 0x8000) {
        self->actor.colorFilterTimer = 20;
    } else {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            EnBili_SetupDie(self);
        }
    }
}

void EnBili_Die(EnBili* self, GlobalContext* globalCtx) {
    static Vec3f effectVelocity = { 0.0f, 0.0f, 0.0f };
    static Vec3f effectAccel = { 0.0f, 0.0f, 0.0f };
    s16 effectScale;
    Vec3f effectPos;
    s32 i;

    if (self->actor.draw != NULL) {
        if (self->actor.flags & 0x8000) {
            return;
        }
        self->actor.draw = NULL;
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x50);
    }

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer != 0) {
        for (i = 0; i < 2; i++) {
            effectPos.x = ((Rand_ZeroOne() * 10.0f) + self->actor.world.pos.x) - 5.0f;
            effectPos.y = ((Rand_ZeroOne() * 5.0f) + self->actor.world.pos.y) - 2.5f;
            effectPos.z = ((Rand_ZeroOne() * 10.0f) + self->actor.world.pos.z) - 5.0f;

            effectVelocity.y = Rand_ZeroOne() + 1.0f;
            effectScale = Rand_S16Offset(40, 40);

            if (Rand_ZeroOne() < 0.7f) {
                EffectSsDtBubble_SpawnColorProfile(globalCtx, &effectPos, &effectVelocity, &effectAccel, effectScale,
                                                   25, 2, 1);
            } else {
                EffectSsDtBubble_SpawnColorProfile(globalCtx, &effectPos, &effectVelocity, &effectAccel, effectScale,
                                                   25, 0, 1);
            }
        }
    } else {
        Actor_Kill(&self->actor);
    }

    if (self->timer == 14) {
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 40, NA_SE_EN_BIRI_BUBLE);
    }
}

void EnBili_Stunned(EnBili* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->actor.bgCheckFlags & 2) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
    }

    if (self->timer == 0) {
        EnBili_SetupFloatIdle(self);
    }
}

void EnBili_Frozen(EnBili* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (!(self->actor.flags & 0x8000)) {
        self->actor.gravity = -1.0f;
    }

    if ((self->actor.bgCheckFlags & 1) || (self->actor.floorHeight == BGCHECK_Y_MIN)) {
        self->actor.colorFilterTimer = 0;
        EnBili_SetupDie(self);
    } else {
        self->actor.colorFilterTimer = 10;
    }
}

void EnBili_UpdateDamage(EnBili* self, GlobalContext* globalCtx) {
    u8 damageEffect;

    if ((self->actor.colChkInfo.health != 0) && (self->collider.base.acFlags & AC_HIT)) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);

        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_BIRI_DEAD);
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                self->actor.flags &= ~1;
            }

            damageEffect = self->actor.colChkInfo.damageEffect;

            if (damageEffect == BIRI_DMGEFF_DEKUNUT) {
                if (self->actionFunc != EnBili_Stunned) {
                    EnBili_SetupStunned(self);
                }
            } else if (damageEffect == BIRI_DMGEFF_SWORD) {
                if (self->actionFunc != EnBili_Stunned) {
                    Actor_SetColorFilter(&self->actor, 0x4000, 0xC8, 0x2000, 0xA);

                    if (self->actor.colChkInfo.health == 0) {
                        self->actor.params = EN_BILI_TYPE_DYING;
                    }
                    EnBili_SetupDischargeLightning(self);
                } else {
                    EnBili_SetupBurnt(self);
                }
            } else if (damageEffect == BIRI_DMGEFF_FIRE) {
                EnBili_SetupBurnt(self);
                self->timer = 2;
            } else if (damageEffect == BIRI_DMGEFF_ICE) {
                EnBili_SetupFrozen(self, globalCtx);
            } else if (damageEffect == BIRI_DMGEFF_SLINGSHOT) {
                EnBili_SetupRecoil(self);
            } else {
                EnBili_SetupBurnt(self);
            }

            if (self->collider.info.acHitInfo->toucher.dmgFlags & 0x1F820) { // DMG_ARROW
                self->actor.flags |= 0x10;
            }
        }
    }
}

void EnBili_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnBili* self = THIS;

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        EnBili_SetupDischargeLightning(self);
    }

    EnBili_UpdateDamage(self, globalCtx);
    self->actionFunc(self, globalCtx);

    if (self->actionFunc != EnBili_Die) {
        EnBili_UpdateTentaclesIndex(self);
        if (Animation_OnFrame(&self->skelAnime, 9.0f)) {
            if ((self->actionFunc == EnBili_FloatIdle) || (self->actionFunc == EnBili_SetNewHomeHeight) ||
                (self->actionFunc == EnBili_ApproachPlayer) || (self->actionFunc == EnBili_Recoil)) {
                if (self->playFlySound) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_BIRI_FLY);
                    self->playFlySound = false;
                } else {
                    self->playFlySound = true;
                }
            }
        }
        if (self->actionFunc == EnBili_Recoil) {
            func_8002D97C(&self->actor);
        } else {
            Actor_MoveForward(&self->actor);
        }

        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 5.0f, self->collider.dim.radius, self->collider.dim.height, 7);
        Collider_UpdateCylinder(&self->actor, &self->collider);

        if (self->collider.base.atFlags & AT_ON) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }

        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        Actor_SetFocus(&self->actor, 0.0f);
    }
}

// Draw and associated functions

void EnBili_PulseLimb3(EnBili* self, f32 frame, Vec3f* arg2) {
    f32 cos;
    f32 sin;

    if (self->actionFunc == EnBili_DischargeLightning) {
        arg2->y = 1.0f - (sinf((M_PI * 0.16667f) * frame) * 0.26f);
    } else if (self->actionFunc == EnBili_Climb) {
        if (frame <= 8.0f) {
            arg2->y = (cosf((M_PI * 0.125f) * frame) * 0.15f) + 0.85f;
        } else if (frame <= 18.0f) {
            cos = cosf((frame - 8.0f) * (M_PI * 0.1f));
            arg2->y = 1.0f - (0.3f * cos);
            arg2->x = (0.2f * cos) + 0.8f;
        } else {
            cos = cosf((frame - 18.0f) * (M_PI * 0.0227f));
            arg2->y = (0.31f * cos) + 1.0f;
            arg2->x = 1.0f - (0.4f * cos);
        }

        arg2->z = arg2->x;
    } else if (self->actionFunc == EnBili_Stunned) {
        sin = sinf((M_PI * 0.1f) * self->timer) * 0.08f;
        arg2->x -= sin;
        arg2->y += sin;
        arg2->z -= sin;
    } else {
        arg2->y = (cosf((M_PI * 0.125f) * frame) * 0.13f) + 0.87f;
    }
}

void EnBili_PulseLimb2(EnBili* self, f32 frame, Vec3f* arg2) {
    f32 cos;
    f32 sin;

    if (self->actionFunc == EnBili_DischargeLightning) {
        arg2->y = (sinf((M_PI * 0.16667f) * frame) * 0.2f) + 1.0f;
    } else if (self->actionFunc == EnBili_Climb) {
        if (frame <= 8.0f) {
            arg2->x = 1.125f - (cosf((M_PI * 0.125f) * frame) * 0.125f);
        } else if (frame <= 18.0f) {
            cos = cosf((frame - 8.0f) * (M_PI * 0.1f));
            arg2->x = (0.275f * cos) + 0.975f;
            arg2->y = 1.25f - (0.25f * cos);
        } else {
            cos = cosf((frame - 18.0f) * (M_PI * 0.0227f));
            arg2->x = 1.0f - (0.3f * cos);
            arg2->y = (0.48f * cos) + 1.0f;
        }
        arg2->z = arg2->x;
    } else if (self->actionFunc == EnBili_Stunned) {
        sin = sinf((M_PI * 0.1f) * self->timer) * 0.08f;
        arg2->x += sin;
        arg2->y -= sin;
        arg2->z += sin;
    } else {
        arg2->y = 1.1f - (cosf((M_PI * 0.125f) * frame) * 0.1f);
    }
}

void EnBili_PulseLimb4(EnBili* self, f32 frame, Vec3f* arg2) {
    f32 cos;

    if (self->actionFunc == EnBili_Climb) {
        if (frame <= 8.0f) {
            cos = cosf((M_PI * 0.125f) * frame);
            arg2->x = 1.125f - (0.125f * cos);
            arg2->y = (0.3f * cos) + 0.7f;
        } else if (frame <= 18.0f) {
            cos = cosf((frame - 8.0f) * (M_PI * 0.1f));
            arg2->x = (0.325f * cos) + 0.925f;
            arg2->y = 0.95f - (0.55f * cos);
        } else {
            cos = cosf((frame - 18.0f) * (M_PI * 0.0227f));
            arg2->x = 1.0f - (0.4f * cos);
            arg2->y = (0.52f * cos) + 1.0f;
        }
        arg2->z = arg2->x;
    }
}

s32 EnBili_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                            Gfx** gfx) {
    EnBili* self = THIS;
    Vec3f limbScale = { 1.0f, 1.0f, 1.0f };
    f32 curFrame = self->skelAnime.curFrame;

    if (limbIndex == EN_BILI_LIMB_OUTER_HOOD) {
        EnBili_PulseLimb3(self, curFrame, &limbScale);
    } else if (limbIndex == EN_BILI_LIMB_INNER_HOOD) {
        EnBili_PulseLimb2(self, curFrame, &limbScale);
    } else if (limbIndex == EN_BILI_LIMB_TENTACLES) {
        EnBili_PulseLimb4(self, curFrame, &limbScale);
        rot->y = (Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) - self->actor.shape.rot.y) + 0x8000;
    }

    Matrix_Scale(limbScale.x, limbScale.y, limbScale.z, MTXMODE_APPLY);
    return false;
}

static void* sTentaclesTextures[] = {
    gBiriTentacles0Tex, gBiriTentacles1Tex, gBiriTentacles2Tex, gBiriTentacles3Tex,
    gBiriTentacles4Tex, gBiriTentacles5Tex, gBiriTentacles6Tex, gBiriTentacles7Tex,
};

static Gfx D_809C16F0[] = {
    gsDPSetCombineLERP(1, TEXEL0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, TEXEL1, 0,
                       PRIM_LOD_FRAC, COMBINED),
    gsSPEndDisplayList(),
};

static Gfx D_809C1700[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, TEXEL1, 0,
                       PRIM_LOD_FRAC, COMBINED),
    gsSPEndDisplayList(),
};

void EnBili_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBili* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bili.c", 1521);
    func_80093D84(globalCtx->state.gfxCtx);

    self->tentaclesTexIndex = CLAMP_MAX(self->tentaclesTexIndex, 7);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sTentaclesTextures[self->tentaclesTexIndex]));

    if ((self->actionFunc == EnBili_DischargeLightning) && ((self->timer & 1) != 0)) {
        gSPSegment(POLY_XLU_DISP++, 0x09, &D_809C16F0);
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x09, &D_809C1700);
    }

    POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                   &EnBili_OverrideLimbDraw, NULL, self, POLY_XLU_DISP);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bili.c", 1552);
}
