/*
 * File: z_boss_ganondrof.c
 * Overlay: ovl_Boss_Ganondrof
 * Description: Phantom Ganon
 */

#include "z_boss_ganondrof.h"
#include "objects/object_gnd/object_gnd.h"
#include "overlays/actors/ovl_En_fHG/z_en_fhg.h"
#include "overlays/actors/ovl_En_Fhg_Fire/z_en_fhg_fire.h"
#include "overlays/effects/ovl_Effect_Ss_Fhg_Flash/z_eff_ss_fhg_flash.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"

#define FLAGS 0x00000035

#define THIS ((BossGanondrof*)thisx)

typedef enum {
    /* 0 */ NOT_DEAD,
    /* 1 */ DEATH_START,
    /* 2 */ DEATH_THROES,
    /* 3 */ DEATH_WARP,
    /* 4 */ DEATH_SCREAM,
    /* 5 */ DEATH_DISINTEGRATE,
    /* 6 */ DEATH_FINISH
} BossGanondrofDeathState;

typedef enum {
    /* 0 */ THROW_NORMAL,
    /* 1 */ THROW_SLOW
} BossGanondrofThrowAction;

typedef enum {
    /* 0 */ STUNNED_FALL,
    /* 1 */ STUNNED_GROUND
} BossGanondrofStunnedAction;

typedef enum {
    /* 0 */ CHARGE_WINDUP,
    /* 1 */ CHARGE_START,
    /* 2 */ CHARGE_ATTACK,
    /* 3 */ CHARGE_FINISH
} BossGanondrofChargeAction;

typedef enum {
    /* 0 */ DEATH_SPASM,
    /* 1 */ DEATH_LIMP,
    /* 2 */ DEATH_HUNCHED
} BossGanondrofDeathAction;

void BossGanondrof_Init(Actor* thisx, GlobalContext* globalCtx);
void BossGanondrof_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BossGanondrof_Update(Actor* thisx, GlobalContext* globalCtx);
void BossGanondrof_Draw(Actor* thisx, GlobalContext* globalCtx);

void BossGanondrof_SetupIntro(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_Intro(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_SetupPaintings(BossGanondrof* self);
void BossGanondrof_Paintings(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_SetupNeutral(BossGanondrof* self, f32 arg1);
void BossGanondrof_Neutral(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_SetupThrow(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_Throw(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_SetupBlock(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_Block(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_SetupReturn(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_Return(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_SetupCharge(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_Charge(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_Stunned(BossGanondrof* self, GlobalContext* globalCtx);
void BossGanondrof_Death(BossGanondrof* self, GlobalContext* globalCtx);

const ActorInit Boss_Ganondrof_InitVars = {
    ACTOR_BOSS_GANONDROF,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_GND,
    sizeof(BossGanondrof),
    (ActorFunc)BossGanondrof_Init,
    (ActorFunc)BossGanondrof_Destroy,
    (ActorFunc)BossGanondrof_Update,
    (ActorFunc)BossGanondrof_Draw,
};

static ColliderCylinderInit sCylinderInitBody = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x10 },
        { 0xFFCFFFFE, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 30, 90, -50, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInitSpear = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x30 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 30, -20, { 0, 0, 0 } },
};

// clang-format off
static u8 sDecayMaskHigh[16 * 16] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,
    1,0,1,1,0,0,0,0,1,1,1,1,1,1,0,1,
    1,0,1,1,1,0,0,0,0,1,1,1,1,1,0,1,
    1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,
    1,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,
    1,0,1,1,1,1,0,0,0,0,0,1,0,0,0,1,
    1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,1,
    1,0,1,1,1,1,1,0,0,0,0,1,1,1,0,1,
    1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,
    1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,
    1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,
    1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1,
    1,0,1,1,1,1,1,0,0,1,1,1,1,1,0,1,
    1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

static u8 sDecayMaskLow[16 * 16] = {
    1,1,1,0,1,0,0,1,0,0,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,
    1,0,1,1,0,0,0,0,0,1,1,0,0,1,0,0,
    1,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,
    0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,1,
    0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,
    1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
    1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
    1,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,
    1,0,0,0,0,1,0,0,0,0,1,0,1,1,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,
    1,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1,
    1,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,
};

static u8 sDecayMaskTotal[16 * 16] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};
// clang-format on

// These are Phantom Ganon's body textures, but I don't know which is which.
static void* sLimbTex_rgba16_8x8[] = {
    gPhantomGanonLimbTex_00A800, gPhantomGanonLimbTex_00AE80, gPhantomGanonLimbTex_00AF00,
    gPhantomGanonLimbTex_00C180, gPhantomGanonLimbTex_00C400,
};
static void* sLimbTex_rgba16_16x8[] = {
    gPhantomGanonLimbTex_00B980, gPhantomGanonLimbTex_00C480, gPhantomGanonLimbTex_00BC80,
    gPhantomGanonLimbTex_00BD80, gPhantomGanonLimbTex_00C080,
};
static void* sLimbTex_rgba16_16x16[] = {
    gPhantomGanonLimbTex_00C200, gPhantomGanonLimbTex_00A000, gPhantomGanonLimbTex_00A200,
    gPhantomGanonLimbTex_00A400, gPhantomGanonLimbTex_00A600, gPhantomGanonLimbTex_00A880,
    gPhantomGanonLimbTex_00B780, gPhantomGanonLimbTex_00BA80, gPhantomGanonLimbTex_00BE80,
};
static void* sLimbTex_rgba16_16x32[] = { gPhantomGanonLimbTex_00AA80, gPhantomGanonLimbTex_00AF80 };

static void* sMouthTex_ci8_16x16[] = { gPhantomGanonMouthTex, gPhantomGanonSmileTex };

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x2B, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 0, ICHAIN_STOP),
};

static Vec3f sAudioVec = { 0.0f, 0.0f, 50.0f };

void BossGanondrof_ClearPixels8x8(s16* texture, u8* mask, s16 index) {
    if (mask[index]) {
        texture[index / 4] = 0;
    }
}

void BossGanondrof_ClearPixels16x8(s16* texture, u8* mask, s16 index) {
    if (mask[index]) {
        texture[index / 2] = 0;
    }
}

void BossGanondrof_ClearPixels16x16(s16* texture, u8* mask, s16 index) {
    if (mask[index]) {
        texture[index] = 0;
    }
}

void BossGanondrof_ClearPixels32x16(s16* texture, u8* mask, s16 index) {
    if (mask[index]) {
        s16 i = (index & 0xF) + ((index & 0xF0) << 1);

        texture[i + 0x10] = 0;
        texture[i] = 0;
    }
}

void BossGanondrof_ClearPixels16x32(s16* texture, u8* mask, s16 index) {
    if (mask[index]) {
        s16 i = ((index & 0xF) * 2) + ((index & 0xF0) * 2);

        texture[i + 1] = 0;
        texture[i] = 0;
    }
}

void BossGanondrof_ClearPixels(u8* mask, s16 index) {
    s16 i;

    for (i = 0; i < 5; i++) {
        // ARRAY_COUNT can't be used here because the arrays aren't guaranteed to be the same size.
        BossGanondrof_ClearPixels8x8(SEGMENTED_TO_VIRTUAL(sLimbTex_rgba16_8x8[i]), mask, index);
        BossGanondrof_ClearPixels16x8(SEGMENTED_TO_VIRTUAL(sLimbTex_rgba16_16x8[i]), mask, index);
    }

    for (i = 0; i < ARRAY_COUNT(sLimbTex_rgba16_16x16); i++) {
        BossGanondrof_ClearPixels16x16(SEGMENTED_TO_VIRTUAL(sLimbTex_rgba16_16x16[i]), mask, index);
    }

    for (i = 0; i < ARRAY_COUNT(sLimbTex_rgba16_16x32); i++) {
        BossGanondrof_ClearPixels16x32(SEGMENTED_TO_VIRTUAL(sLimbTex_rgba16_16x32[i]), mask, index);
    }

    BossGanondrof_ClearPixels32x16(SEGMENTED_TO_VIRTUAL(gPhantomGanonLimbTex_00B380), mask, index);
    BossGanondrof_ClearPixels16x32(SEGMENTED_TO_VIRTUAL(gPhantomGanonEyeTex), mask, index);
    for (i = 0; i < ARRAY_COUNT(sMouthTex_ci8_16x16); i++) {
        BossGanondrof_ClearPixels16x16(SEGMENTED_TO_VIRTUAL(sMouthTex_ci8_16x16[i]), mask, index);
    }
}

void BossGanondrof_SetColliderPos(Vec3f* pos, ColliderCylinder* collider) {
    collider->dim.pos.x = pos->x;
    collider->dim.pos.y = pos->y;
    collider->dim.pos.z = pos->z;
}

void BossGanondrof_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossGanondrof* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    Actor_SetScale(&self->actor, 0.01f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gPhantomGanonSkel, &gPhantomGanonRideAnim, NULL, NULL, 0);
    if (self->actor.params < GND_FAKE_BOSS) {
        self->actor.params = GND_REAL_BOSS;
        self->actor.colChkInfo.health = 30;
        self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);
        Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 255, 255, 255, 255);
        BossGanondrof_SetupIntro(self, globalCtx);
    } else {
        BossGanondrof_SetupPaintings(self);
    }

    Collider_InitCylinder(globalCtx, &self->colliderBody);
    Collider_InitCylinder(globalCtx, &self->colliderSpear);
    Collider_SetCylinder(globalCtx, &self->colliderBody, &self->actor, &sCylinderInitBody);
    Collider_SetCylinder(globalCtx, &self->colliderSpear, &self->actor, &sCylinderInitSpear);
    self->actor.flags &= ~1;
    if (Flags_GetClear(globalCtx, globalCtx->roomCtx.curRoom.num)) {
        Actor_Kill(&self->actor);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_WARP1, GND_BOSSROOM_CENTER_X, GND_BOSSROOM_CENTER_Y,
                    GND_BOSSROOM_CENTER_Z, 0, 0, 0, WARP_DUNGEON_ADULT);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, 200.0f + GND_BOSSROOM_CENTER_X,
                    GND_BOSSROOM_CENTER_Y, GND_BOSSROOM_CENTER_Z, 0, 0, 0, 0);
    } else {
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG, self->actor.world.pos.x,
                           self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, self->actor.params);
    }
}

void BossGanondrof_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossGanondrof* self = THIS;

    osSyncPrintf("DT1\n");
    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyCylinder(globalCtx, &self->colliderBody);
    Collider_DestroyCylinder(globalCtx, &self->colliderSpear);
    if (self->actor.params == GND_REAL_BOSS) {
        LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
    }

    osSyncPrintf("DT2\n");
}

void BossGanondrof_SetupIntro(BossGanondrof* self, GlobalContext* globalCtx) {
    Animation_PlayLoop(&self->skelAnime, &gPhantomGanonRidePoseAnim);
    self->actionFunc = BossGanondrof_Intro;
    self->work[GND_MASK_OFF] = true;
}

void BossGanondrof_Intro(BossGanondrof* self, GlobalContext* globalCtx) {
    s16 i;
    s32 pad;
    EnfHG* horse = (EnfHG*)self->actor.child;

    SkelAnime_Update(&self->skelAnime);
    self->actor.world.pos = horse->actor.world.pos;
    self->actor.shape.rot.y = self->actor.world.rot.y = horse->actor.world.rot.y;

    osSyncPrintf("SW %d------------------------------------------------\n", horse->bossGndSignal);

    if ((self->timers[1] != 0) && (self->timers[1] < 25)) {
        Vec3f pos;
        Vec3f vel = { 0.0f, 0.0f, 0.0f };
        Vec3f accel = { 0.0f, 0.0f, 0.0f };

        pos.x = self->bodyPartsPos[14].x + Rand_CenteredFloat(10.0f);
        pos.y = self->bodyPartsPos[14].y + Rand_ZeroFloat(-5.0f);
        pos.z = self->bodyPartsPos[14].z + Rand_CenteredFloat(10.0f) + 5.0f;
        accel.y = 0.03f;
        EffectSsKFire_Spawn(globalCtx, &pos, &vel, &accel, (s16)Rand_ZeroFloat(10.0f) + 5, 0);
    }

    if (self->timers[1] == 20) {
        self->work[GND_MASK_OFF] = false;
    }

    if (self->timers[1] == 30) {
        func_80078914(&sAudioVec, NA_SE_EN_FANTOM_TRANSFORM);
    }

    if (horse->bossGndSignal == FHG_LIGHTNING) {
        Animation_Change(&self->skelAnime, &gPhantomGanonMaskOnAnim, 0.5f, 0.0f,
                         Animation_GetLastFrame(&gPhantomGanonMaskOnAnim), ANIMMODE_ONCE_INTERP, 0.0f);
        self->timers[1] = 40;
    }

    if (horse->bossGndSignal == FHG_REAR) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonHorseRearingAnim, -3.0f);
    }

    if (horse->bossGndSignal == FHG_RIDE) {
        Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonRidePoseAnim, -13.0f);
    }

    if (horse->bossGndSignal == FHG_SPUR) {
        EnfHG* horseTemp;

        Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonRideSpearRaiseAnim, -7.0f);
        horseTemp = (EnfHG*)self->actor.child;
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE, self->spearTip.x,
                           self->spearTip.y, self->spearTip.z, 50, FHGFIRE_LIGHT_GREEN, 0, FHGFIRE_SPEAR_LIGHT);
        self->actor.child = &horseTemp->actor;
    }

    if (horse->bossGndSignal == FHG_FINISH) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonRideSpearResetAnim, -5.0f);
    }

    switch (self->work[GND_EYE_STATE]) {
        case GND_EYESTATE_FADE:
            self->fwork[GND_EYE_ALPHA] += 40.0f;
            if (self->fwork[GND_EYE_ALPHA] >= 255.0f) {
                self->fwork[GND_EYE_ALPHA] = 255.0f;
            }
            break;

        case GND_EYESTATE_BRIGHTEN:
            self->fwork[GND_EYE_BRIGHTNESS] += 20.0f;
            if (self->fwork[GND_EYE_BRIGHTNESS] > 255.0f) {
                self->fwork[GND_EYE_BRIGHTNESS] = 255.0f;
            }
            break;
    }

    self->armRotY = Math_SinS(self->work[GND_VARIANCE_TIMER] * 0x6E8) * 0;
    self->armRotZ = Math_CosS(self->work[GND_VARIANCE_TIMER] * 0x8DC) * 300.0f;
    for (i = 0; i < 30; i++) {
        self->rideRotY[i] = Math_SinS(self->work[GND_VARIANCE_TIMER] * ((i * 50) + 0x7B0)) * 100.0f;
        self->rideRotZ[i] = Math_CosS(self->work[GND_VARIANCE_TIMER] * ((i * 50) + 0x8DC)) * 100.0f;
    }

    if (horse->bossGndSignal == FHG_START_FIGHT) {
        BossGanondrof_SetupPaintings(self);
        for (i = 0; i < 30; i++) {
            self->rideRotY[i] = self->rideRotZ[i] = 0.0f;
        }
    }

    horse->bossGndSignal = FHG_NO_SIGNAL;
}

void BossGanondrof_SetupPaintings(BossGanondrof* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonRideAnim, -5.0f);
    self->actionFunc = BossGanondrof_Paintings;
}

void BossGanondrof_Paintings(BossGanondrof* self, GlobalContext* globalCtx) {
    EnfHG* horse = (EnfHG*)self->actor.child;

    osSyncPrintf("RUN 1\n");
    SkelAnime_Update(&self->skelAnime);
    osSyncPrintf("RUN 2\n");

    if (horse->bossGndSignal == FHG_RAISE_SPEAR) {
        EnfHG* horseTemp;

        Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonRideSpearRaiseAnim, -2.0f);
        self->actor.flags |= 1;
        horseTemp = (EnfHG*)self->actor.child;
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE, self->spearTip.x,
                           self->spearTip.y, self->spearTip.z, 30, FHGFIRE_LIGHT_GREEN, 0, FHGFIRE_SPEAR_LIGHT);
        self->actor.child = &horseTemp->actor;
    } else if (horse->bossGndSignal == FHG_LIGHTNING) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonRideSpearStrikeAnim, -2.0f);
    } else if (horse->bossGndSignal == FHG_RESET) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonRideSpearResetAnim, -2.0f);
    } else if (horse->bossGndSignal == FHG_RIDE) {
        Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonRideAnim, -2.0f);
        self->actor.flags &= ~1;
    }

    osSyncPrintf("RUN 3\n");
    self->actor.world.pos = horse->actor.world.pos;
    self->actor.world.pos.y = horse->actor.world.pos.y;
    self->actor.shape.rot.y = self->actor.world.rot.y = horse->actor.world.rot.y;
    if (self->flyMode != GND_FLY_PAINTING) {
        BossGanondrof_SetupNeutral(self, -20.0f);
        self->timers[0] = 100;
        self->colliderBody.dim.radius = 20;
        self->colliderBody.dim.height = 60;
        self->colliderBody.dim.yShift = -33;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_LAUGH);
        self->actor.naviEnemyId = 0x1A;
    } else {
        horse->bossGndSignal = FHG_NO_SIGNAL;
        self->actor.scale.x = horse->actor.scale.x / 1.15f;
        self->actor.scale.y = horse->actor.scale.y / 1.15f;
        self->actor.scale.z = horse->actor.scale.z / 1.15f;
        osSyncPrintf("RUN 4\n");
    }
}

void BossGanondrof_SetupNeutral(BossGanondrof* self, f32 arg1) {
    Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonNeutralAnim, arg1);
    self->actionFunc = BossGanondrof_Neutral;
    self->actor.flags |= 1;
    self->fwork[GND_FLOAT_SPEED] = 0.0f;
    self->timers[0] = (s16)(Rand_ZeroOne() * 64.0f) + 30;
}

void BossGanondrof_Neutral(BossGanondrof* self, GlobalContext* globalCtx) {
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    Player* player = GET_PLAYER(globalCtx);
    Actor* playerx = &player->actor;
    Actor* thisx = &self->actor;
    f32 rand01;
    s16 i;

    SkelAnime_Update(&self->skelAnime);
    switch (self->flyMode) {
        case GND_FLY_NEUTRAL:
            if (self->timers[0] == 0) {
                self->timers[0] = (s16)(Rand_ZeroOne() * 64.0f) + 30;
                rand01 = Rand_ZeroOne();
                if (thisx->colChkInfo.health < 5) {
                    if (rand01 < 0.25f) {
                        BossGanondrof_SetupThrow(self, globalCtx);
                    } else if (rand01 >= 0.8f) {
                        self->flyMode = GND_FLY_CHARGE;
                        self->timers[0] = 60;
                        self->fwork[GND_FLOAT_SPEED] = 0.0f;
                        Audio_PlayActorSound2(thisx, NA_SE_EN_FANTOM_LAUGH);
                    } else {
                        self->flyMode = GND_FLY_VOLLEY;
                        self->timers[0] = 60;
                        self->fwork[GND_FLOAT_SPEED] = 0.0f;
                        Audio_PlayActorSound2(thisx, NA_SE_EN_FANTOM_LAUGH);
                    }
                } else if ((rand01 < 0.5f) || (self->work[GND_THROW_COUNT] < 5)) {
                    BossGanondrof_SetupThrow(self, globalCtx);
                } else {
                    self->flyMode = GND_FLY_VOLLEY;
                    self->timers[0] = 60;
                    self->fwork[GND_FLOAT_SPEED] = 0.0f;
                    Audio_PlayActorSound2(thisx, NA_SE_EN_FANTOM_LAUGH);
                }
            }

            if (self->timers[1] != 0) {
                targetX = GND_BOSSROOM_CENTER_X;
                targetZ = GND_BOSSROOM_CENTER_Z;
            } else {
                targetX = playerx->world.pos.x + (180.0f * Math_SinS(playerx->shape.rot.y));
                targetZ = playerx->world.pos.z + (180.0f * Math_CosS(playerx->shape.rot.y));
                if (sqrtf(SQ(targetX - GND_BOSSROOM_CENTER_X) + SQ(targetZ - GND_BOSSROOM_CENTER_Z)) > 280.0f) {
                    self->timers[1] = 50;
                    self->fwork[GND_FLOAT_SPEED] = 0.0f;
                }
            }

            targetY = playerx->world.pos.y + 100.0f + 0.0f;
            targetX += Math_SinS(self->work[GND_VARIANCE_TIMER] * 0x500) * 100.0f;
            targetZ += Math_CosS(self->work[GND_VARIANCE_TIMER] * 0x700) * 100.0f;
            break;
        case GND_FLY_VOLLEY:
            targetX = GND_BOSSROOM_CENTER_X - 14.0f;
            targetZ = GND_BOSSROOM_CENTER_Z + 265.0f;

            targetY = playerx->world.pos.y + 100.0f + 100.0f;
            targetX += Math_SinS(self->work[GND_VARIANCE_TIMER] * 0x500) * 100.0f;
            targetZ += Math_CosS(self->work[GND_VARIANCE_TIMER] * 0x700) * 100.0f;
            if (self->timers[0] == 0) {
                self->flyMode = GND_FLY_RETURN;
                self->returnSuccess = false;
                BossGanondrof_SetupThrow(self, globalCtx);
                self->timers[0] = 80;
            }
            break;
        case GND_FLY_RETURN:
            targetX = GND_BOSSROOM_CENTER_X - 14.0f;
            targetZ = GND_BOSSROOM_CENTER_Z + 265.0f;

            targetY = playerx->world.pos.y + 100.0f + 100.0f;
            targetX += Math_SinS(self->work[GND_VARIANCE_TIMER] * 0x500) * 50.0f;
            targetZ += Math_CosS(self->work[GND_VARIANCE_TIMER] * 0x700) * 50.0f;
            if (self->returnSuccess) {
                self->returnSuccess = false;
                BossGanondrof_SetupReturn(self, globalCtx);
                self->timers[0] = 80;
            }

            if (self->timers[0] == 0) {
                self->flyMode = GND_FLY_NEUTRAL;
            }
            break;
        case GND_FLY_CHARGE:
            targetX = GND_BOSSROOM_CENTER_X - 14.0f;
            targetZ = GND_BOSSROOM_CENTER_Z + 215.0f;

            targetY = playerx->world.pos.y + 100.0f + 50.0f;
            targetX += Math_SinS(self->work[GND_VARIANCE_TIMER] * 0x500) * 100.0f;
            targetZ += Math_CosS(self->work[GND_VARIANCE_TIMER] * 0x700) * 100.0f;
            if (self->timers[0] == 0) {
                BossGanondrof_SetupCharge(self, globalCtx);
            }
            break;
    }

    Math_ApproachF(&thisx->world.pos.x, targetX, 0.05f, self->fwork[GND_FLOAT_SPEED]);
    if (self->timers[2] != 0) {
        Math_ApproachF(&thisx->world.pos.y, targetY + 100.0f, 0.1f, 50.0f);
    } else {
        Math_ApproachF(&thisx->world.pos.y, targetY, 0.05f, 10.0f);
    }

    Math_ApproachF(&thisx->world.pos.z, targetZ, 0.05f, self->fwork[GND_FLOAT_SPEED]);
    Math_ApproachF(&self->fwork[GND_FLOAT_SPEED], 50.0f, 1.0f, 0.5f);
    thisx->velocity.x = thisx->world.pos.x - thisx->prevPos.x;
    thisx->velocity.z = thisx->world.pos.z - thisx->prevPos.z;
    thisx->world.pos.y += 2.0f * Math_SinS(self->work[GND_VARIANCE_TIMER] * 1500);
    Math_ApproachS(&thisx->shape.rot.y, thisx->yawTowardsPlayer, 5, 0xBB8);
    if ((self->work[GND_VARIANCE_TIMER] & 1) == 0) {
        Vec3f pos;
        Vec3f vel = { 0.0f, 0.0f, 0.0f };
        Vec3f accel = { 0.0f, 0.0f, 0.0f };

        for (i = 0; i < 3; i++) {
            pos.x = Rand_CenteredFloat(20.0f) + self->spearTip.x;
            pos.y = Rand_CenteredFloat(20.0f) + self->spearTip.y;
            pos.z = Rand_CenteredFloat(20.0f) + self->spearTip.z;
            accel.y = -0.08f;
            EffectSsFhgFlash_SpawnLightBall(globalCtx, &pos, &vel, &accel, (s16)(Rand_ZeroOne() * 80.0f) + 150,
                                            FHGFLASH_LIGHTBALL_GREEN);
        }
    }

    if (player->unk_A73 != 0) {
        BossGanondrof_SetupBlock(self, globalCtx);
    }

    Audio_PlayActorSound2(thisx, NA_SE_EN_FANTOM_FLOAT - SFX_FLAG);
}

void BossGanondrof_SetupThrow(BossGanondrof* self, GlobalContext* globalCtx) {
    EnfHG* horseTemp;
    s16 lightTime;

    self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonThrowAnim);
    Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonThrowAnim, -5.0f);
    self->actionFunc = BossGanondrof_Throw;
    if ((Rand_ZeroOne() <= 0.1f) && (self->work[GND_THROW_COUNT] >= 10) && (self->flyMode == GND_FLY_NEUTRAL)) {
        self->work[GND_ACTION_STATE] = THROW_SLOW;
        self->work[GND_THROW_FRAME] = 1000;
        lightTime = 32;
    } else {
        self->work[GND_ACTION_STATE] = THROW_NORMAL;
        self->work[GND_THROW_FRAME] = 25;
        lightTime = 25;
    }

    horseTemp = (EnfHG*)self->actor.child;
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE, self->spearTip.x,
                       self->spearTip.y, self->spearTip.z, lightTime, FHGFIRE_LIGHT_GREEN, 0, FHGFIRE_SPEAR_LIGHT);
    self->actor.child = &horseTemp->actor;
    self->work[GND_THROW_COUNT]++;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_STICK);
}

void BossGanondrof_Throw(BossGanondrof* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    osSyncPrintf("self->fwork[GND_END_FRAME] = %d\n", (s16)self->fwork[GND_END_FRAME]);
    osSyncPrintf("self->work[GND_SHOT_FRAME] = %d\n", self->work[GND_THROW_FRAME]);
    if (Animation_OnFrame(&self->skelAnime, self->fwork[GND_END_FRAME])) {
        BossGanondrof_SetupNeutral(self, -6.0f);
    }

    if ((self->work[GND_ACTION_STATE] != THROW_NORMAL) && Animation_OnFrame(&self->skelAnime, 21.0f)) {
        self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonThrowEndAnim);
        Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonThrowEndAnim, 0.0f);
        self->work[GND_THROW_FRAME] = 10;
    }

    if (Animation_OnFrame(&self->skelAnime, self->work[GND_THROW_FRAME])) {
        if (self->flyMode <= GND_FLY_NEUTRAL) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_MASIC2);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_MASIC1);
        }

        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_VOICE);
    }

    if (Animation_OnFrame(&self->skelAnime, self->work[GND_THROW_FRAME])) {
        EnfHG* horseTemp = (EnfHG*)self->actor.child;

        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE, self->spearTip.x,
                           self->spearTip.y, self->spearTip.z, self->work[GND_ACTION_STATE], 0, 0, FHGFIRE_ENERGY_BALL);
        self->actor.child = &horseTemp->actor;
    }

    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, 0x7D0);
    self->actor.world.pos.x += self->actor.velocity.x;
    self->actor.world.pos.z += self->actor.velocity.z;
    Math_ApproachZeroF(&self->actor.velocity.x, 1.0f, 0.5f);
    Math_ApproachZeroF(&self->actor.velocity.z, 1.0f, 0.5f);
    self->actor.world.pos.y += 2.0f * Math_SinS(self->work[GND_VARIANCE_TIMER] * 1500);
}

void BossGanondrof_SetupReturn(BossGanondrof* self, GlobalContext* globalCtx) {
    static AnimationHeader* returnAnim[] = { &gPhantomGanonReturn1Anim, &gPhantomGanonReturn2Anim };
    s16 rand = Rand_ZeroOne() * 1.99f;

    self->fwork[GND_END_FRAME] = Animation_GetLastFrame(returnAnim[rand]);
    Animation_MorphToPlayOnce(&self->skelAnime, returnAnim[rand], 0.0f);
    self->actionFunc = BossGanondrof_Return;
}

void BossGanondrof_Return(BossGanondrof* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 5.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_VOICE);
        osSyncPrintf("VOISE               2  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        osSyncPrintf("VOISE               2  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    }

    if (Animation_OnFrame(&self->skelAnime, self->fwork[GND_END_FRAME])) {
        BossGanondrof_SetupNeutral(self, 0.0f);
    }

    self->actor.world.pos.x += self->actor.velocity.x;
    self->actor.world.pos.z += self->actor.velocity.z;
    Math_ApproachZeroF(&self->actor.velocity.x, 1.0f, 0.5f);
    Math_ApproachZeroF(&self->actor.velocity.z, 1.0f, 0.5f);
    self->actor.world.pos.y += 2.0f * Math_SinS(self->work[GND_VARIANCE_TIMER] * 1500);
    if (self->returnSuccess) {
        self->returnSuccess = false;
        BossGanondrof_SetupReturn(self, globalCtx);
        self->timers[0] = 80;
    }
}

void BossGanondrof_SetupStunned(BossGanondrof* self, GlobalContext* globalCtx) {
    if (self->actionFunc != BossGanondrof_Stunned) {
        self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonAirDamageAnim);
        Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonAirDamageAnim, 0.0f);
        self->timers[0] = 50;
        self->shockTimer = 60;
    } else {
        self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonGroundDamageAnim);
        Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonGroundDamageAnim, 0.0f);
    }

    self->actionFunc = BossGanondrof_Stunned;
    self->work[GND_ACTION_STATE] = STUNNED_FALL;
    self->actor.velocity.x = 0.0f;
    self->actor.velocity.z = 0.0f;
}

void BossGanondrof_Stunned(BossGanondrof* self, GlobalContext* globalCtx) {
    osSyncPrintf("DAMAGE   .................................\n");
    SkelAnime_Update(&self->skelAnime);
    self->actor.gravity = -0.2f;
    if (self->actor.world.pos.y <= 5.0f) {
        if (self->work[GND_ACTION_STATE] == STUNNED_FALL) {
            self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonStunnedAnim);
            Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonStunnedAnim, -10.0f);
            self->work[GND_ACTION_STATE] = STUNNED_GROUND;
        }

        self->actor.velocity.y = 0.0f;
        self->actor.gravity = 0.0f;
        if (Animation_OnFrame(&self->skelAnime, self->fwork[GND_END_FRAME])) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_DAMAGE2);
        }

        self->actor.flags |= 0x400;
    }

    osSyncPrintf("TIME0 %d ********************************************\n", self->timers[0]);
    if (self->timers[0] == 0) {
        BossGanondrof_SetupNeutral(self, -5.0f);
        self->timers[0] = 30;
        self->timers[2] = 30;
        self->flyMode = GND_FLY_NEUTRAL;
        self->actor.velocity.y = 0.0f;
        self->actor.gravity = 0.0f;
    }

    Actor_MoveForward(&self->actor);
}

void BossGanondrof_SetupBlock(BossGanondrof* self, GlobalContext* globalCtx) {
    self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonBlockAnim);
    Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonBlockAnim, -3.0f);
    self->actionFunc = BossGanondrof_Block;
    self->timers[0] = 10;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_STICK);
}

void BossGanondrof_Block(BossGanondrof* self, GlobalContext* globalCtx) {
    self->colliderBody.base.colType = COLTYPE_METAL;
    SkelAnime_Update(&self->skelAnime);
    self->actor.world.pos.x += self->actor.velocity.x;
    self->actor.world.pos.z += self->actor.velocity.z;
    Math_ApproachZeroF(&self->actor.velocity.x, 1.0f, 0.5f);
    Math_ApproachZeroF(&self->actor.velocity.z, 1.0f, 0.5f);
    self->actor.world.pos.y += 2.0f * Math_SinS(self->work[GND_VARIANCE_TIMER] * 1500);
    if (self->timers[0] == 0) {
        BossGanondrof_SetupNeutral(self, -5.0f);
        self->timers[0] = 10;
        self->flyMode = GND_FLY_NEUTRAL;
    }
}

void BossGanondrof_SetupCharge(BossGanondrof* self, GlobalContext* globalCtx) {
    self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonChargeWindupAnim);
    Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonChargeWindupAnim, -3.0f);
    self->actionFunc = BossGanondrof_Charge;
    self->timers[0] = 20;
    self->work[GND_ACTION_STATE] = CHARGE_WINDUP;
}

void BossGanondrof_Charge(BossGanondrof* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* playerx = &player->actor;
    Actor* thisx = &self->actor;
    f32 dxCenter = thisx->world.pos.x - GND_BOSSROOM_CENTER_X;
    f32 dzCenter = thisx->world.pos.z - GND_BOSSROOM_CENTER_Z;

    self->colliderBody.base.colType = COLTYPE_METAL;
    SkelAnime_Update(&self->skelAnime);
    switch (self->work[GND_ACTION_STATE]) {
        case CHARGE_WINDUP:
            if (self->timers[0] == 218) {
                Audio_PlayActorSound2(thisx, NA_SE_EN_FANTOM_STICK);
            }

            if (self->timers[0] == 19) {
                Audio_PlayActorSound2(thisx, NA_SE_EN_FANTOM_ATTACK);
            }

            thisx->world.pos.x += thisx->velocity.x;
            thisx->world.pos.z += thisx->velocity.z;
            Math_ApproachZeroF(&thisx->velocity.x, 1.0f, 0.5f);
            Math_ApproachZeroF(&thisx->velocity.z, 1.0f, 0.5f);
            if (self->timers[0] == 0) {
                self->work[GND_ACTION_STATE] = CHARGE_START;
                self->timers[0] = 10;
                thisx->speedXZ = 0.0f;
                self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonChargeStartAnim);
                Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonChargeStartAnim, 0.0f);
            }

            Math_ApproachS(&thisx->shape.rot.y, thisx->yawTowardsPlayer, 5, 0x7D0);
            break;
        case CHARGE_START:
            if (Animation_OnFrame(&self->skelAnime, self->fwork[GND_END_FRAME])) {
                self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonChargeAnim);
                Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonChargeAnim, 0.0f);
                self->work[GND_ACTION_STATE] = CHARGE_ATTACK;
            }
        case CHARGE_ATTACK:
            if (self->timers[0] != 0) {
                Vec3f vecToLink;

                Math_ApproachS(&thisx->shape.rot.y, thisx->yawTowardsPlayer, 5, 0x7D0);
                vecToLink.x = playerx->world.pos.x - thisx->world.pos.x;
                vecToLink.y = playerx->world.pos.y + 40.0f - thisx->world.pos.y;
                vecToLink.z = playerx->world.pos.z - thisx->world.pos.z;
                thisx->world.rot.y = thisx->shape.rot.y;
                thisx->world.rot.x =
                    Math_FAtan2F(vecToLink.y, sqrtf(SQ(vecToLink.x) + SQ(vecToLink.z))) * (0x8000 / M_PI);
            }

            func_8002D908(thisx);
            func_8002D7EC(thisx);
            Math_ApproachF(&thisx->speedXZ, 10.0f, 1.0f, 0.5f);
            if ((sqrtf(SQ(dxCenter) + SQ(dzCenter)) > 280.0f) || (thisx->xyzDistToPlayerSq < SQ(100.0f))) {
                self->work[GND_ACTION_STATE] = CHARGE_FINISH;
                self->timers[0] = 20;
            }
            break;
        case CHARGE_FINISH:
            thisx->gravity = 0.2f;
            Actor_MoveForward(thisx);
            osSyncPrintf("YP %f @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n", thisx->world.pos.y);
            if (thisx->world.pos.y < 5.0f) {
                thisx->world.pos.y = 5.0f;
                thisx->velocity.y = 0.0f;
            }

            if (sqrtf(SQ(dxCenter) + SQ(dzCenter)) > 280.0f) {
                Math_ApproachZeroF(&thisx->speedXZ, 1.0f, 2.0f);
                self->timers[0] = 0;
            }

            if (self->timers[0] == 0) {
                Math_ApproachZeroF(&thisx->speedXZ, 1.0f, 2.0f);
                Math_ApproachZeroF(&thisx->velocity.y, 1.0f, 2.0f);
                Math_ApproachS(&thisx->shape.rot.y, thisx->yawTowardsPlayer, 5, 0x7D0);
                if ((thisx->speedXZ <= 0.5f) && (fabsf(thisx->velocity.y) <= 0.1f)) {
                    BossGanondrof_SetupNeutral(self, -10.0f);
                    self->timers[0] = 30;
                    self->flyMode = GND_FLY_NEUTRAL;
                }
            }
            break;
    }

    if (thisx->world.pos.y > (GND_BOSSROOM_CENTER_Y + 83.0f)) {
        thisx->world.pos.y += 2.0f * Math_SinS(self->work[GND_VARIANCE_TIMER] * 1500);
    }
    {
        s16 i;
        Vec3f pos;
        Vec3f vel = { 0.0f, 0.0f, 0.0f };
        Vec3f accel = { 0.0f, 0.0f, 0.0f };
        Vec3f baseOffset = { 0.0f, 50.0f, 0.0f };
        Vec3f offset;

        baseOffset.y = 10.0f;
        for (i = 0; i < 10; i++) {
            Matrix_Push();
            Matrix_RotateY((thisx->shape.rot.y / (f32)0x8000) * M_PI, MTXMODE_NEW);
            Matrix_RotateX((thisx->shape.rot.x / (f32)0x8000) * M_PI, MTXMODE_APPLY);
            Matrix_RotateZ((self->work[GND_PARTICLE_ANGLE] / (f32)0x8000) * M_PI, MTXMODE_APPLY);
            Matrix_MultVec3f(&baseOffset, &offset);
            Matrix_Pop();
            pos.x = self->spearTip.x + offset.x;
            pos.y = self->spearTip.y + offset.y;
            pos.z = self->spearTip.z + offset.z;
            vel.x = (offset.x * 500.0f) / 1000.0f;
            vel.y = (offset.y * 500.0f) / 1000.0f;
            vel.z = (offset.z * 500.0f) / 1000.0f;
            accel.x = (offset.x * -50.0f) / 1000.0f;
            accel.y = (offset.y * -50.0f) / 1000.0f;
            accel.z = (offset.z * -50.0f) / 1000.0f;
            EffectSsFhgFlash_SpawnLightBall(globalCtx, &pos, &vel, &accel, 150, i % 7);
            self->work[GND_PARTICLE_ANGLE] += 0x1A5C;
        }
    }

    if (!(self->work[GND_VARIANCE_TIMER] & 7)) {
        EnfHG* horse = (EnfHG*)thisx->child;

        Actor_SpawnAsChild(&globalCtx->actorCtx, thisx, globalCtx, ACTOR_EN_FHG_FIRE, self->spearTip.x,
                           self->spearTip.y, self->spearTip.z, 8, FHGFIRE_LIGHT_BLUE, 0, FHGFIRE_SPEAR_LIGHT);
        thisx->child = &horse->actor;
    }
}

void BossGanondrof_SetupDeath(BossGanondrof* self, GlobalContext* globalCtx) {
    Animation_PlayOnce(&self->skelAnime, &gPhantomGanonDeathBlowAnim);
    self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonDeathBlowAnim);
    self->actionFunc = BossGanondrof_Death;
    Audio_QueueSeqCmd(0x100100FF);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_DEAD);
    self->deathState = DEATH_START;
    self->actor.flags &= ~1;
    self->work[GND_VARIANCE_TIMER] = 0;
    self->shockTimer = 50;
}

void BossGanondrof_Death(BossGanondrof* self, GlobalContext* globalCtx) {
    u8 holdCamera = false;
    u8 bodyDecayLevel = 0;
    f32 camX;
    f32 camZ;
    f32 pad;
    Player* player = GET_PLAYER(globalCtx);
    Camera* camera = Gameplay_GetCamera(globalCtx, 0);

    osSyncPrintf("PYP %f\n", player->actor.floorHeight);
    SkelAnime_Update(&self->skelAnime);
    self->work[GND_DEATH_SFX_TIMER]++;
    if (((60 < self->work[GND_DEATH_SFX_TIMER]) && (self->work[GND_DEATH_SFX_TIMER] < 500)) ||
        ((501 < self->work[GND_DEATH_SFX_TIMER]) && (self->work[GND_DEATH_SFX_TIMER] < 620))) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_LAST - SFX_FLAG);
    }

    switch (self->deathState) {
        case DEATH_START:
            func_80064520(globalCtx, &globalCtx->csCtx);
            func_8002DF54(globalCtx, &self->actor, 1);
            self->deathCamera = Gameplay_CreateSubCamera(globalCtx);
            Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
            osSyncPrintf("7\n");
            Gameplay_ChangeCameraStatus(globalCtx, self->deathCamera, CAM_STAT_ACTIVE);
            osSyncPrintf("8\n");
            self->deathState = DEATH_THROES;
            player->actor.speedXZ = 0.0f;
            self->timers[0] = 50;
            self->cameraEye = camera->eye;
            self->cameraAt = camera->at;
            self->cameraNextEye.x = self->targetPos.x;
            self->cameraNextEye.y = GND_BOSSROOM_CENTER_Y + 83.0f;
            self->cameraNextEye.z = (self->targetPos.z + 100.0f) + 50;
            self->cameraNextAt.x = self->targetPos.x;
            self->cameraNextAt.y = self->targetPos.y - 10.0f;
            self->cameraNextAt.z = self->targetPos.z;
            self->cameraEyeVel.x = fabsf(camera->eye.x - self->cameraNextEye.x);
            self->cameraEyeVel.y = fabsf(camera->eye.y - self->cameraNextEye.y);
            self->cameraEyeVel.z = fabsf(camera->eye.z - self->cameraNextEye.z);
            self->cameraAtVel.x = fabsf(camera->at.x - self->cameraNextAt.x);
            self->cameraAtVel.y = fabsf(camera->at.y - self->cameraNextAt.y);
            self->cameraAtVel.z = fabsf(camera->at.z - self->cameraNextAt.z);
            self->cameraAccel = 0.02f;
            self->cameraEyeMaxVel.x = self->cameraEyeMaxVel.y = self->cameraEyeMaxVel.z = 0.05f;
            self->work[GND_ACTION_STATE] = DEATH_SPASM;
            self->timers[0] = 150;
            self->cameraAtMaxVel.x = 0.2f;
            self->cameraAtMaxVel.y = 0.2f;
            self->cameraAtMaxVel.z = 0.2f;
        case DEATH_THROES:
            switch (self->work[GND_ACTION_STATE]) {
                case DEATH_SPASM:
                    if (Animation_OnFrame(&self->skelAnime, self->fwork[GND_END_FRAME])) {
                        self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonAirDamageAnim);
                        Animation_Change(&self->skelAnime, &gPhantomGanonAirDamageAnim, 0.5f, 0.0f,
                                         self->fwork[GND_END_FRAME], ANIMMODE_ONCE_INTERP, 0.0f);
                        self->work[GND_ACTION_STATE] = DEATH_LIMP;
                    }
                    break;
                case DEATH_LIMP:
                    if (Animation_OnFrame(&self->skelAnime, self->fwork[GND_END_FRAME])) {
                        self->fwork[GND_END_FRAME] = Animation_GetLastFrame(&gPhantomGanonLimpAnim);
                        Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonLimpAnim, -20.0f);
                        self->work[GND_ACTION_STATE] = DEATH_HUNCHED;
                    }
                case DEATH_HUNCHED:
                    bodyDecayLevel = 1;
                    break;
            }
            Math_ApproachS(&self->actor.shape.rot.y, self->work[GND_VARIANCE_TIMER] * -100, 5, 0xBB8);
            Math_ApproachF(&self->cameraNextEye.z, self->targetPos.z + 60.0f, 0.02f, 0.5f);
            Math_ApproachF(&self->actor.world.pos.y, GND_BOSSROOM_CENTER_Y + 133.0f, 0.05f, 100.0f);
            self->actor.world.pos.y += Math_SinS(self->work[GND_VARIANCE_TIMER] * 1500);
            self->cameraNextAt.x = self->targetPos.x;
            self->cameraNextAt.y = self->targetPos.y - 10.0f;
            self->cameraNextAt.z = self->targetPos.z;
            if (self->timers[0] == 0) {
                self->deathState = DEATH_WARP;
                self->timers[0] = 350;
                self->timers[1] = 50;
                self->fwork[GND_CAMERA_ZOOM] = 300.0f;
                self->cameraNextEye.y = GND_BOSSROOM_CENTER_Y + 233.0f;
                player->actor.world.pos.x = GND_BOSSROOM_CENTER_X - 200.0f;
                player->actor.world.pos.z = GND_BOSSROOM_CENTER_Z;
                holdCamera = true;
                bodyDecayLevel = 1;
            }
            break;
        case DEATH_WARP:
            if (self->timers[1] == 1) {
                EnfHG* horseTemp = (EnfHG*)self->actor.child;

                Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE,
                                   GND_BOSSROOM_CENTER_X, GND_BOSSROOM_CENTER_Y + 3.0f, GND_BOSSROOM_CENTER_Z, 0x4000,
                                   0, 0, FHGFIRE_WARP_DEATH);
                self->actor.child = &horseTemp->actor;
                func_8010B680(globalCtx, 0x108E, NULL);
            }

            self->actor.shape.rot.y -= 0xC8;
            self->actor.world.pos.y += Math_SinS(self->work[GND_VARIANCE_TIMER] * 1500);
            self->fwork[GND_CAMERA_ANGLE] += 0x78;
            camX = Math_SinS(self->fwork[GND_CAMERA_ANGLE]) * self->fwork[GND_CAMERA_ZOOM];
            camZ = Math_CosS(self->fwork[GND_CAMERA_ANGLE]) * self->fwork[GND_CAMERA_ZOOM];
            self->cameraEye.x = GND_BOSSROOM_CENTER_X + camX;
            self->cameraEye.y = self->cameraNextEye.y;
            self->cameraEye.z = GND_BOSSROOM_CENTER_Z + camZ;
            self->cameraAt.x = GND_BOSSROOM_CENTER_X;
            self->cameraAt.y = GND_BOSSROOM_CENTER_Y + 23.0f;
            self->cameraAt.z = GND_BOSSROOM_CENTER_Z;
            Math_ApproachF(&self->cameraNextEye.y, GND_BOSSROOM_CENTER_Y + 33.0f, 0.05f, 0.5f);
            Math_ApproachF(&self->fwork[GND_CAMERA_ZOOM], 170.0f, 0.05f, 1.0f);
            Math_ApproachF(&self->actor.world.pos.x, GND_BOSSROOM_CENTER_X, 0.05f, 1.5f);
            Math_ApproachF(&self->actor.world.pos.y, GND_BOSSROOM_CENTER_Y + 83.0f, 0.05f, 1.0f);
            Math_ApproachF(&self->actor.world.pos.z, GND_BOSSROOM_CENTER_Z, 0.05f, 1.5f);
            if (self->timers[0] == 0) {
                self->deathState = DEATH_SCREAM;
                self->timers[0] = 50;
                Animation_MorphToLoop(&self->skelAnime, &gPhantomGanonScreamAnim, -10.0f);
                self->actor.world.pos.x = GND_BOSSROOM_CENTER_X;
                self->actor.world.pos.y = GND_BOSSROOM_CENTER_Y + 83.0f;
                self->actor.world.pos.z = GND_BOSSROOM_CENTER_Z;
                self->actor.shape.rot.y = 0;
                self->work[GND_BODY_DECAY_INDEX] = 0;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_LAST);
            }

            holdCamera = true;
            bodyDecayLevel = 1;
            break;
        case DEATH_SCREAM:
            holdCamera = true;
            bodyDecayLevel = 2;
            self->actor.world.pos.y = GND_BOSSROOM_CENTER_Y + 83.0f;
            self->cameraEye.x = GND_BOSSROOM_CENTER_X;
            self->cameraEye.y = GND_BOSSROOM_CENTER_Y + 83.0f;
            self->cameraEye.z = GND_BOSSROOM_CENTER_Z + 50.0f;
            self->cameraAt.x = GND_BOSSROOM_CENTER_X;
            self->cameraAt.y = GND_BOSSROOM_CENTER_Y + 103.0f;
            self->cameraAt.z = GND_BOSSROOM_CENTER_Z;
            if (self->timers[0] == 0) {
                self->deathState = DEATH_DISINTEGRATE;
                Animation_MorphToPlayOnce(&self->skelAnime, &gPhantomGanonLastPoseAnim, -10.0f);
                self->work[GND_BODY_DECAY_INDEX] = 0;
                self->timers[0] = 40;
            }
            break;
        case DEATH_DISINTEGRATE:
            holdCamera = true;
            bodyDecayLevel = 3;
            Math_ApproachZeroF(&self->cameraEye.y, 0.05f, 1.0f); // approaches GND_BOSSROOM_CENTER_Y + 33.0f
            Math_ApproachF(&self->cameraEye.z, GND_BOSSROOM_CENTER_Z + 170.0f, 0.05f, 2.0f);
            Math_ApproachF(&self->cameraAt.y, GND_BOSSROOM_CENTER_Y + 53.0f, 0.05f, 1.0f);
            if (self->timers[0] == 0) {
                self->timers[0] = 250;
                self->deathState = DEATH_FINISH;
            }
            break;
        case DEATH_FINISH:
            holdCamera = true;
            bodyDecayLevel = 10;
            if (self->timers[0] == 150) {
                Audio_QueueSeqCmd(0x21);
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_WARP1, GND_BOSSROOM_CENTER_X,
                            GND_BOSSROOM_CENTER_Y, GND_BOSSROOM_CENTER_Z, 0, 0, 0, WARP_DUNGEON_ADULT);
            }

            Math_ApproachZeroF(&self->cameraEye.y, 0.05f, 1.0f); // GND_BOSSROOM_CENTER_Y + 33.0f
            Math_ApproachF(&self->cameraEye.z, GND_BOSSROOM_CENTER_Z + 170.0f, 0.05f, 2.0f);
            Math_ApproachF(&self->cameraAt.y, GND_BOSSROOM_CENTER_Y + 53.0f, 0.05f, 1.0f);
            if (self->timers[0] == 0) {
                EnfHG* horse = (EnfHG*)self->actor.child;

                camera->eye = self->cameraEye;
                camera->eyeNext = self->cameraEye;
                camera->at = self->cameraAt;
                func_800C08AC(globalCtx, self->deathCamera, 0);
                self->deathCamera = 0;
                func_80064534(globalCtx, &globalCtx->csCtx);
                func_8002DF54(globalCtx, &self->actor, 7);
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, GND_BOSSROOM_CENTER_X,
                            GND_BOSSROOM_CENTER_Y, GND_BOSSROOM_CENTER_Z + 200.0f, 0, 0, 0, 0);
                self->actor.child = &horse->actor;
                self->killActor = true;
                horse->killActor = true;
                Flags_SetClear(globalCtx, globalCtx->roomCtx.curRoom.num);
                Flags_SetSwitch(globalCtx, 0x22);
            }
            break;
    }

    if (bodyDecayLevel) {
        Vec3f pos;
        Vec3f vel = { 0.0f, 0.0f, 0.0f };
        Vec3f accelKFire = { 0.0f, 0.0f, 0.0f };
        Vec3f accelHahen = { 0.0f, -0.5f, 0.0f };
        s16 limbDecayIndex;
        s16 i;

        vel.x = self->actor.world.pos.x - self->actor.prevPos.x;
        vel.z = self->actor.world.pos.z - self->actor.prevPos.z;
        if (bodyDecayLevel < 10) {
            if (self->work[GND_DEATH_ENV_TIMER] == 0) {
                if (globalCtx->envCtx.unk_BF == 0) {
                    globalCtx->envCtx.unk_BF = 3;
                    self->work[GND_DEATH_ENV_TIMER] = (s16)Rand_ZeroFloat(5.0f) + 4.0f;
                    globalCtx->envCtx.unk_D6 = 0x28;
                } else {
                    globalCtx->envCtx.unk_BF = 0;
                    self->work[GND_DEATH_ENV_TIMER] = (s16)Rand_ZeroFloat(2.0f) + 2.0f;
                    globalCtx->envCtx.unk_D6 = 0x14;
                }
            } else {
                self->work[GND_DEATH_ENV_TIMER]--;
            }

            for (i = 0; i <= 0; i++) {
                limbDecayIndex = self->work[GND_LIMB_DECAY_INDEX];
                self->work[GND_LIMB_DECAY_INDEX]++;
                self->work[GND_LIMB_DECAY_INDEX] %= 25;
                pos.x = self->bodyPartsPos[limbDecayIndex].x + Rand_CenteredFloat(5.0f);
                pos.y = self->bodyPartsPos[limbDecayIndex].y + Rand_CenteredFloat(5.0f);
                pos.z = self->bodyPartsPos[limbDecayIndex].z + Rand_CenteredFloat(5.0f);
                accelKFire.y = 0.0f;

                if (bodyDecayLevel == 3) {
                    accelKFire.y = -0.2f;
                    accelKFire.x = (GND_BOSSROOM_CENTER_X - pos.x) * 0.002f;
                    accelKFire.z = (GND_BOSSROOM_CENTER_Z - pos.z) * 0.002f;
                    accelHahen.x = (GND_BOSSROOM_CENTER_X - pos.x) * 0.001f;
                    accelHahen.y = -1.0f;
                    accelHahen.z = (GND_BOSSROOM_CENTER_Z - pos.z) * 0.001f;
                }

                EffectSsKFire_Spawn(globalCtx, &pos, &vel, &accelKFire, (s16)Rand_ZeroFloat(20.0f) + 15,
                                    bodyDecayLevel);
                if ((Rand_ZeroOne() < 0.5f) || (bodyDecayLevel == 3)) {
                    EffectSsHahen_Spawn(globalCtx, &pos, &vel, &accelHahen, 0, (s16)Rand_ZeroFloat(4.0f) + 7,
                                        HAHEN_OBJECT_DEFAULT, 10, NULL);
                }
            }
        } else {
            globalCtx->envCtx.unk_BF = 0;
            globalCtx->envCtx.unk_D6 = 0x14;
        }

        self->work[GND_BODY_DECAY_FLAG] = true;
        for (i = 0; i < 5; i++) {
            if (bodyDecayLevel == 1) {
                BossGanondrof_ClearPixels(sDecayMaskLow, self->work[GND_BODY_DECAY_INDEX]);
            } else if (bodyDecayLevel == 2) {
                BossGanondrof_ClearPixels(sDecayMaskHigh, self->work[GND_BODY_DECAY_INDEX]);
            } else {
                BossGanondrof_ClearPixels(sDecayMaskTotal, self->work[GND_BODY_DECAY_INDEX]);
            }

            if (self->work[GND_BODY_DECAY_INDEX] < 0x100) {
                self->work[GND_BODY_DECAY_INDEX]++;
            }
        }
    }

    if (self->deathCamera != 0) {
        if (!holdCamera) {
            Math_ApproachF(&self->cameraEye.x, self->cameraNextEye.x, self->cameraEyeMaxVel.x,
                           self->cameraEyeVel.x * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraEye.y, self->cameraNextEye.y, self->cameraEyeMaxVel.y,
                           self->cameraEyeVel.y * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraEye.z, self->cameraNextEye.z, self->cameraEyeMaxVel.z,
                           self->cameraEyeVel.z * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraAt.x, self->cameraNextAt.x, self->cameraAtMaxVel.x,
                           self->cameraAtVel.x * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraAt.y, self->cameraNextAt.y, self->cameraAtMaxVel.y,
                           self->cameraAtVel.y * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraAt.z, self->cameraNextAt.z, self->cameraAtMaxVel.z,
                           self->cameraAtVel.z * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraSpeedMod, 1.0f, 1.0f, self->cameraAccel);
        }

        Gameplay_CameraSetAtEye(globalCtx, self->deathCamera, &self->cameraAt, &self->cameraEye);
    }
}

void BossGanondrof_CollisionCheck(BossGanondrof* self, GlobalContext* globalCtx) {
    s32 acHit;
    EnfHG* horse = (EnfHG*)self->actor.child;
    ColliderInfo* hurtbox;

    if (self->work[GND_INVINC_TIMER] != 0) {
        self->work[GND_INVINC_TIMER]--;
        self->returnCount = 0;
        self->colliderBody.base.acFlags &= ~AC_HIT;
    } else {
        acHit = self->colliderBody.base.acFlags & AC_HIT;
        if ((acHit && ((s8)self->actor.colChkInfo.health > 0)) || (self->returnCount != 0)) {
            if (acHit) {
                self->colliderBody.base.acFlags &= ~AC_HIT;
                hurtbox = self->colliderBody.info.acHitInfo;
            }
            if (self->flyMode != GND_FLY_PAINTING) {
                if (acHit && (self->actionFunc != BossGanondrof_Stunned) && (hurtbox->toucher.dmgFlags & 0x0001F8A4)) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_PL_WALK_GROUND - SFX_FLAG);
                    osSyncPrintf("hit != 0 \n");
                } else if (self->actionFunc != BossGanondrof_Charge) {
                    if (self->returnCount == 0) {
                        u8 dmg;
                        u8 canKill = false;
                        s32 dmgFlags = hurtbox->toucher.dmgFlags;

                        if (dmgFlags & 0x80) {
                            return;
                        }
                        dmg = CollisionCheck_GetSwordDamage(dmgFlags);
                        (dmg == 0) ? (dmg = 2) : (canKill = true);
                        if (((s8)self->actor.colChkInfo.health > 2) || canKill) {
                            self->actor.colChkInfo.health -= dmg;
                        }

                        if ((s8)self->actor.colChkInfo.health <= 0) {
                            BossGanondrof_SetupDeath(self, globalCtx);
                            Enemy_StartFinishingBlow(globalCtx, &self->actor);
                            return;
                        }
                    }
                    BossGanondrof_SetupStunned(self, globalCtx);
                    if (self->returnCount >= 2) {
                        self->timers[0] = 120;
                    }
                    self->work[GND_INVINC_TIMER] = 10;
                    horse->hitTimer = 20;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_DAMAGE);
                } else {
                    Audio_PlayActorSound2(&self->actor, NA_SE_PL_WALK_GROUND - SFX_FLAG);
                }
            } else if (acHit && (hurtbox->toucher.dmgFlags & 0x0001F8A4)) {
                self->work[GND_INVINC_TIMER] = 10;
                self->actor.colChkInfo.health -= 2;
                horse->hitTimer = 20;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_DAMAGE);
            }
            self->returnCount = 0;
        }
    }
}

void BossGanondrof_Update(Actor* thisx, GlobalContext* globalCtx) {
    f32 cs;
    f32 sn;
    f32 legRotTargetY;
    f32 legRotTargetZ;
    f32 legSplitTarget;
    s32 pad2;
    s16 i;
    s32 pad;
    BossGanondrof* self = THIS;
    EnfHG* horse;

    osSyncPrintf("MOVE START %d\n", self->actor.params);
    self->actor.flags &= ~0x400;
    self->colliderBody.base.colType = COLTYPE_HIT3;
    if (self->killActor) {
        Actor_Kill(&self->actor);
        return;
    }
    self->work[GND_VARIANCE_TIMER]++;
    horse = (EnfHG*)self->actor.child;
    osSyncPrintf("MOVE START EEEEEEEEEEEEEEEEEEEEEE%d\n", self->actor.params);

    self->actionFunc(self, globalCtx);

    for (i = 0; i < ARRAY_COUNT(self->timers); i++) {
        if (self->timers[i]) {
            self->timers[i]--;
        }
    }
    if (self->work[GND_UNKTIMER_1]) {
        self->work[GND_UNKTIMER_1]--;
    }
    if (self->work[GND_UNKTIMER_2]) {
        self->work[GND_UNKTIMER_2]--;
    }

    if (self->actionFunc != BossGanondrof_Death) {
        BossGanondrof_CollisionCheck(self, globalCtx);
    }

    osSyncPrintf("MOVE END\n");
    BossGanondrof_SetColliderPos(&self->targetPos, &self->colliderBody);
    BossGanondrof_SetColliderPos(&self->spearTip, &self->colliderSpear);
    if ((self->flyMode == GND_FLY_PAINTING) && !horse->bossGndInPainting) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderBody.base);
    }
    if ((self->actionFunc == BossGanondrof_Stunned) && (self->timers[0] > 1)) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderBody.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderBody.base);
    } else if (self->actionFunc == BossGanondrof_Block) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderBody.base);
    } else if (self->actionFunc == BossGanondrof_Charge) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderBody.base);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderBody.base);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderSpear.base);
    }

    self->actor.focus.pos = self->targetPos;

    sn = Math_SinS(-self->actor.shape.rot.y);
    cs = Math_CosS(-self->actor.shape.rot.y);
    legRotTargetY = ((sn * self->actor.velocity.z) + (cs * self->actor.velocity.x)) * 300.0f;
    legRotTargetZ = ((-sn * self->actor.velocity.x) + (cs * self->actor.velocity.z)) * 300.0f;
    Math_ApproachF(&self->legRotY, legRotTargetY, 1.0f, 600.0f);
    Math_ApproachF(&self->legRotZ, legRotTargetZ, 1.0f, 600.0f);
    if ((self->flyMode != GND_FLY_PAINTING) && (self->actionFunc != BossGanondrof_Stunned) &&
        (self->deathState == NOT_DEAD)) {
        legSplitTarget = (Math_SinS(self->work[GND_VARIANCE_TIMER] * 0x8DC) * -500.0f) - 500.0f;
    } else {
        legSplitTarget = 0.0f;
    }

    Math_ApproachF(&self->legSplitY, legSplitTarget, 1.0f, 100.0f);
    if (self->shockTimer != 0) {
        s16 j;

        self->shockTimer--;
        osSyncPrintf("F 1\n");
        for (j = 0; j < 7; j++) {
            osSyncPrintf("F 15\n");
            EffectSsFhgFlash_SpawnShock(globalCtx, &self->actor, &self->actor.world.pos, 45, FHGFLASH_SHOCK_PG);
        }
        osSyncPrintf("F 2\n");
    }

    if (self->actor.params == GND_REAL_BOSS) {
        Lights_PointNoGlowSetInfo(&self->lightInfo, self->spearTip.x, self->spearTip.y, self->spearTip.z, 255, 255, 255,
                                  200);
    }
}

s32 BossGanondrof_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                   void* thisx) {
    BossGanondrof* self = THIS;

    switch (limbIndex) {
        case 15:
            if ((self->actionFunc == BossGanondrof_Intro) && self->work[GND_MASK_OFF]) {
                *dList = gPhantomGanonFaceDL;
            }
            rot->y += self->rideRotY[limbIndex];
            rot->z += self->rideRotZ[limbIndex];
            break;

        case 19:
            rot->y += self->legRotY + self->legSplitY;
            rot->z += self->legRotZ;
            break;

        case 20:
            rot->y += self->legRotY + self->legSplitY;
            rot->z += self->legRotZ;
            break;

        case 21:
            rot->y += self->legRotY + self->legSplitY;
            rot->z += self->legRotZ;
            break;

        case 23:
            rot->y += self->legRotY - self->legSplitY;
            rot->z += self->legRotZ;
            break;

        case 24:
            rot->y += self->legRotY - self->legSplitY;
            rot->z += self->legRotZ;
            break;

        case 25:
            rot->y += self->legRotY - self->legSplitY;
            rot->z += self->legRotZ;
            break;

        case 5:
        case 6:
        case 7:
            rot->y += self->armRotY;
            rot->z += self->armRotZ;
            break;

        case 8:
        case 9:
        case 10:
            rot->y += self->armRotY;
            rot->z += self->armRotZ;
            break;

        case 13:
            if (self->deathState != NOT_DEAD) {
                *dList = NULL;
            }
        default:
            rot->y += self->rideRotY[limbIndex];
            rot->z += self->rideRotZ[limbIndex];
            break;
    }

    return 0;
}

void BossGanondrof_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    static Vec3f spearVec = { 0.0f, 0.0f, 6000.0f };

    BossGanondrof* self = THIS;

    if (limbIndex == 14) {
        Matrix_MultVec3f(&zeroVec, &self->targetPos);
    } else if (limbIndex == 13) {
        Matrix_MultVec3f(&spearVec, &self->spearTip);
    }

    if (((self->flyMode != GND_FLY_PAINTING) || (self->actionFunc == BossGanondrof_Intro)) && (limbIndex <= 25)) {
        Matrix_MultVec3f(&zeroVec, &self->bodyPartsPos[limbIndex - 1]);
    }
}

Gfx* BossGanondrof_GetClearPixelDList(GraphicsContext* gfxCtx) {
    Gfx* dList = (Gfx*)Graph_Alloc(gfxCtx, sizeof(Gfx) * 4);
    Gfx* dListHead = dList;

    gDPPipeSync(dListHead++);
    gDPSetRenderMode(dListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(dListHead++, G_CULL_BACK);
    gSPEndDisplayList(dListHead++);

    return dList;
}

Gfx* BossGanondrof_GetNullDList(GraphicsContext* gfxCtx) {
    Gfx* dList = (Gfx*)Graph_Alloc(gfxCtx, sizeof(Gfx) * 1);
    Gfx* dListHead = dList;

    gSPEndDisplayList(dListHead++);
    return dList;
}

void BossGanondrof_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossGanondrof* self = THIS;
    EnfHG* horse;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_ganondrof.c", 3716);
    osSyncPrintf("MOVE P = %x\n", self->actor.update);
    osSyncPrintf("STOP TIMER = %d ==============\n", self->actor.freezeTimer);
    horse = (EnfHG*)self->actor.child;
    if (self->flyMode == GND_FLY_PAINTING) {
        Matrix_RotateY((horse->turnRot * 3.1416f) / (f32)0x8000, MTXMODE_APPLY);
    }

    osSyncPrintf("YP %f\n", self->actor.world.pos.y);
    func_80093D18(globalCtx->state.gfxCtx);
    if (self->work[GND_INVINC_TIMER] & 4) {
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 50, 0, 0, 900, 1099);
    } else {
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, (u32)horse->warpColorFilterR, (u32)horse->warpColorFilterG,
                                   (u32)horse->warpColorFilterB, 0, (s32)horse->warpColorFilterUnk1 + 995,
                                   (s32)horse->warpColorFilterUnk2 + 1000);
    }

    osSyncPrintf("DRAW 11\n");
    osSyncPrintf("EYE_COL %d\n", (s16)self->fwork[GND_EYE_BRIGHTNESS]);
    gDPSetEnvColor(POLY_OPA_DISP++, (s16)self->fwork[GND_EYE_BRIGHTNESS], (s16)self->fwork[GND_EYE_BRIGHTNESS],
                   (s16)self->fwork[GND_EYE_BRIGHTNESS], (s16)self->fwork[GND_EYE_ALPHA]);
    if (self->work[GND_BODY_DECAY_FLAG]) {
        gSPSegment(POLY_OPA_DISP++, 0x08, BossGanondrof_GetClearPixelDList(globalCtx->state.gfxCtx));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08, BossGanondrof_GetNullDList(globalCtx->state.gfxCtx));
    }

    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, BossGanondrof_OverrideLimbDraw,
                      BossGanondrof_PostLimbDraw, self);
    osSyncPrintf("DRAW 22\n");
    POLY_OPA_DISP = Gameplay_SetFog(globalCtx, POLY_OPA_DISP);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_ganondrof.c", 3814);
    osSyncPrintf("DRAW END %d\n", self->actor.params);
}
