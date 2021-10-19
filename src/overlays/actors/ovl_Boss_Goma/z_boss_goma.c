#include "z_boss_goma.h"
#include "objects/object_goma/object_goma.h"
#include "overlays/actors/ovl_En_Goma/z_en_goma.h"
#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"

#define FLAGS 0x00000035

#define THIS ((BossGoma*)thisx)

// IRIS_FOLLOW: gohma looks towards the player (iris rotation)
// BONUS_IFRAMES: gain invincibility frames when the player does something (throwing things?), or
// randomly (see BossGoma_UpdateEye)
typedef enum {
    EYESTATE_IRIS_FOLLOW_BONUS_IFRAMES, // default, allows not drawing lens and iris when eye is closed
    EYESTATE_IRIS_NO_FOLLOW_NO_IFRAMES,
    EYESTATE_IRIS_FOLLOW_NO_IFRAMES
} GohmaEyeState;

typedef enum {
    VISUALSTATE_RED,         // main/eye: red
    VISUALSTATE_DEFAULT,     // main: greenish cyan, blinks with dark gray every 16 frames; eye: white
    VISUALSTATE_DEFEATED,    // main/eye: dark gray
    VISUALSTATE_STUNNED = 4, // main: greenish cyan, alternates with blue; eye: greenish cyan
    VISUALSTATE_HIT          // main: greenish cyan, alternates with red; eye: greenish cyan
} GohmaVisualState;

void BossGoma_Init(Actor* thisx, GlobalContext* globalCtx);
void BossGoma_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BossGoma_Update(Actor* thisx, GlobalContext* globalCtx);
void BossGoma_Draw(Actor* thisx, GlobalContext* globalCtx);

void BossGoma_SetupEncounter(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_Encounter(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_Defeated(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorAttackPosture(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorPrepareAttack(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorAttack(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorDamaged(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorLandStruckDown(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorLand(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorStunned(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FallJump(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FallStruckDown(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_CeilingSpawnGohmas(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_CeilingPrepareSpawnGohmas(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorIdle(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_CeilingIdle(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_FloorMain(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_WallClimb(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_CeilingMoveToCenter(BossGoma* self, GlobalContext* globalCtx);
void BossGoma_SpawnChildGohma(BossGoma* self, GlobalContext* globalCtx, s16 i);

const ActorInit Boss_Goma_InitVars = {
    ACTOR_BOSS_GOMA,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_GOMA,
    sizeof(BossGoma),
    (ActorFunc)BossGoma_Init,
    (ActorFunc)BossGoma_Destroy,
    (ActorFunc)BossGoma_Update,
    (ActorFunc)BossGoma_Draw,
};

static ColliderJntSphElementInit sColliderJntSphElementInit[13] = {
    {
        {
            ELEMTYPE_UNK3,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_EYE, { { 0, 0, 1200 }, 20 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_TAIL4, { { 0, 0, 0 }, 20 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_TAIL3, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_TAIL2, { { 0, 0, 0 }, 12 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_TAIL1, { { 0, 0, 0 }, 25 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_R_FEET, { { 0, 0, 0 }, 30 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_R_SHIN, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_R_THIGH_SHELL, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_L_ANTENNA_CLAW, { { 0, 0, 0 }, 20 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_R_ANTENNA_CLAW, { { 0, 0, 0 }, 20 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_L_FEET, { { 0, 0, 0 }, 30 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_L_SHIN, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { BOSSGOMA_LIMB_L_THIGH_SHELL, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sColliderJntSphInit = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    13,
    sColliderJntSphElementInit,
};

static u8 sClearPixelTableFirstPass[16 * 16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00,
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00
};

static u8 sClearPixelTableSecondPass[16 * 16] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
};

// indexed by limb (where the root limb is 1)
static u8 sDeadLimbLifetime[] = {
    0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    30, // tail end/last part
    40, // tail 2nd to last part
    0,  0, 0, 0, 0, 0, 0, 0,
    10, // back of right claw/hand
    15, // front of right claw/hand
    21, // part of right arm (inner)
    0,  0,
    25, // part of right arm (shell)
    0,  0,
    31, // part of right arm (shell on shoulder)
    35, // part of right arm (shoulder)
    0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    43, // end of left antenna
    48, // middle of left antenna
    53, // start of left antenna
    0,  0, 0, 0,
    42, // end of right antenna
    45, // middle of right antenna
    53, // start of right antenna
    0,  0, 0, 0, 0, 0,
    11, // back of left claw/hand
    15, // front of left claw/hand
    21, // part of left arm (inner)
    0,  0,
    25, // part of left arm (shell)
    0,  0,
    30, // part of left arm (shell on shoulder)
    35, // part of left arm (shoulder)
    0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/**
 * Sets the `i`th pixel of a 16x16 RGBA16 image to 0 (transparent black)
 * according to the `clearPixelTable`
 */
void BossGoma_ClearPixels16x16Rgba16(s16* rgba16image, u8* clearPixelTable, s16 i) {
    if (clearPixelTable[i]) {
        rgba16image[i] = 0;
    }
}

/**
 * Sets the `i`th 2x2 pixels block of a 32x32 RGBA16 image to 0 (transparent black)
 * according to the `clearPixelTable`
 */
void BossGoma_ClearPixels32x32Rgba16(s16* rgba16image, u8* clearPixelTable, s16 i) {
    s16* targetPixel;

    if (clearPixelTable[i]) {
        // address of the top left pixel in a 2x2 pixels block located at
        // (i & 0xF, i >> 4) in a 16x16 grid of 2x2 pixels
        targetPixel = (s32)rgba16image + (s16)((i & 0xF) * 2 + (i & 0xF0) * 4) * 2;
        // set the 2x2 block of pixels to 0
        targetPixel[0] = 0;
        targetPixel[1] = 0;
        targetPixel[32 + 0] = 0;
        targetPixel[32 + 1] = 0;
    }
}

/**
 * Clear pixels from Gohma's textures
 */
void BossGoma_ClearPixels(u8* clearPixelTable, s16 i) {
    BossGoma_ClearPixels16x16Rgba16(SEGMENTED_TO_VIRTUAL(gGohmaBodyTex), clearPixelTable, i);
    BossGoma_ClearPixels16x16Rgba16(SEGMENTED_TO_VIRTUAL(gGohmaShellUndersideTex), clearPixelTable, i);
    BossGoma_ClearPixels16x16Rgba16(SEGMENTED_TO_VIRTUAL(gGohmaDarkShellTex), clearPixelTable, i);
    BossGoma_ClearPixels16x16Rgba16(SEGMENTED_TO_VIRTUAL(gGohmaEyeTex), clearPixelTable, i);

    BossGoma_ClearPixels32x32Rgba16(SEGMENTED_TO_VIRTUAL(gGohmaShellTex), clearPixelTable, i);
    BossGoma_ClearPixels32x32Rgba16(SEGMENTED_TO_VIRTUAL(gGohmaIrisTex), clearPixelTable, i);
}

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x01, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_STOP),
};

void BossGoma_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossGoma* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 4000.0f, ActorShadow_DrawCircle, 150.0f);
    SkelAnime_Init(globalCtx, &self->skelanime, &gGohmaSkel, &gGohmaIdleCrouchedAnim, NULL, NULL, 0);
    Animation_PlayLoop(&self->skelanime, &gGohmaIdleCrouchedAnim);
    self->actor.shape.rot.x = -0x8000; // upside-down
    self->eyeIrisScaleX = 1.0f;
    self->eyeIrisScaleY = 1.0f;
    self->unusedInitX = self->actor.world.pos.x;
    self->unusedInitZ = self->actor.world.pos.z;
    self->actor.world.pos.y = -300.0f; // ceiling
    self->actor.gravity = 0.0f;
    BossGoma_SetupEncounter(self, globalCtx);
    self->actor.colChkInfo.health = 10;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sColliderJntSphInit, self->colliderItems);

    if (Flags_GetClear(globalCtx, globalCtx->roomCtx.curRoom.num)) {
        Actor_Kill(&self->actor);
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, 0.0f, -640.0f, 0.0f, 0, 0,
                           0, WARP_DUNGEON_CHILD);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, 141.0f, -640.0f, -84.0f, 0, 0, 0, 0);
    }
}

void BossGoma_PlayEffectsAndSfx(BossGoma* self, GlobalContext* globalCtx, s16 arg2, s16 amountMinus1) {
    if (arg2 == 0 || arg2 == 1 || arg2 == 3) {
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->rightHandBackLimbWorldPos, 25.0f, amountMinus1, 8.0f,
                                 500, 10, 1);
    }

    if (arg2 == 0 || arg2 == 2 || arg2 == 3) {
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->leftHandBackLimbWorldPos, 25.0f, amountMinus1, 8.0f,
                                 500, 10, 1);
    }

    if (arg2 == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_DOWN);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_WALK);
    }
}

void BossGoma_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BossGoma* self = THIS;

    SkelAnime_Free(&self->skelanime, globalCtx);
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

/**
 * When Gohma is hit and its health drops to 0
 */
void BossGoma_SetupDefeated(BossGoma* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelanime, &gGohmaDeathAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaDeathAnim),
                     ANIMMODE_ONCE, -2.0f);
    self->actionFunc = BossGoma_Defeated;
    self->disableGameplayLogic = true;
    self->decayingProgress = 0;
    self->noBackfaceCulling = false;
    self->framesUntilNextAction = 1200;
    self->actionState = 0;
    self->actor.flags &= ~5;
    self->actor.speedXZ = 0.0f;
    self->actor.shape.shadowScale = 0.0f;
    Audio_QueueSeqCmd(0x100100FF);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_DEAD);
}

/**
 * Initial action setup, with Gohma waiting on the ceiling for the fight to start.
 */
void BossGoma_SetupEncounter(BossGoma* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(&gGohmaWalkAnim);

    Animation_Change(&self->skelanime, &gGohmaWalkAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP, -15.0f);
    self->actionFunc = BossGoma_Encounter;
    self->actionState = 0;
    self->disableGameplayLogic = true;
    globalCtx->envCtx.unk_BF = 4;
    globalCtx->envCtx.unk_D6 = 0xFF;
}

/**
 * On the floor and not doing anything for 20-30 frames, before going back to BossGoma_FloorMain
 */
void BossGoma_SetupFloorIdle(BossGoma* self) {
    f32 lastFrame = Animation_GetLastFrame(&gGohmaIdleCrouchedAnim);

    self->framesUntilNextAction = Rand_S16Offset(20, 30);
    Animation_Change(&self->skelanime, &gGohmaIdleCrouchedAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP, -5.0f);
    self->actionFunc = BossGoma_FloorIdle;
}

/**
 * On the ceiling and not doing anything for 20-30 frames, leads to spawning children gohmas
 */
void BossGoma_SetupCeilingIdle(BossGoma* self) {
    self->framesUntilNextAction = Rand_S16Offset(20, 30);
    Animation_Change(&self->skelanime, &gGohmaHangAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaHangAnim),
                     ANIMMODE_LOOP, -5.0f);
    self->actionFunc = BossGoma_CeilingIdle;
}

/**
 * When the player killed all children gohmas
 */
void BossGoma_SetupFallJump(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaLandAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -5.0f);
    self->actionFunc = BossGoma_FallJump;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = -2.0f;
}

/**
 * When the player successfully hits Gohma on the ceiling
 */
void BossGoma_SetupFallStruckDown(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaCrashAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -5.0f);
    self->actionFunc = BossGoma_FallStruckDown;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = -2.0f;
}

void BossGoma_SetupCeilingSpawnGohmas(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaLayEggsAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaLayEggsAnim),
                     ANIMMODE_LOOP, -15.0f);
    self->actionFunc = BossGoma_CeilingSpawnGohmas;
    self->spawnGohmasActionTimer = 0;
}

void BossGoma_SetupCeilingPrepareSpawnGohmas(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaPrepareEggsAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gGohmaPrepareEggsAnim), ANIMMODE_LOOP, -10.0f);
    self->actionFunc = BossGoma_CeilingPrepareSpawnGohmas;
    self->framesUntilNextAction = 70;
}

void BossGoma_SetupWallClimb(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaClimbAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaClimbAnim),
                     ANIMMODE_LOOP, -10.0f);
    self->actionFunc = BossGoma_WallClimb;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = 0.0f;
}

/**
 * Gohma either reached the ceiling after climbing a wall, or is waiting for the player to kill the (children) Gohmas.
 */
void BossGoma_SetupCeilingMoveToCenter(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaWalkAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaWalkAnim),
                     ANIMMODE_LOOP, -5.0f);
    self->actionFunc = BossGoma_CeilingMoveToCenter;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = 0.0f;
    self->framesUntilNextAction = Rand_S16Offset(30, 60);
}

/**
 * Root action when on the floor, leads to attacking or climbing.
 */
void BossGoma_SetupFloorMain(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaWalkCrouchedAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gGohmaWalkCrouchedAnim), ANIMMODE_LOOP, -5.0f);
    self->actionFunc = BossGoma_FloorMain;
    self->framesUntilNextAction = Rand_S16Offset(70, 110);
}

/**
 * Gohma jumped to the floor on its own, after the player has killed its children Gohmas.
 */
void BossGoma_SetupFloorLand(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaLandAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaLandAnim),
                     ANIMMODE_ONCE, -2.0f);
    self->actionFunc = BossGoma_FloorLand;
    self->currentAnimFrameCount = Animation_GetLastFrame(&gGohmaLandAnim);
}

/**
 * Gohma was shot by the player down from the ceiling.
 */
void BossGoma_SetupFloorLandStruckDown(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaCrashAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaCrashAnim),
                     ANIMMODE_ONCE, -2.0f);
    self->currentAnimFrameCount = Animation_GetLastFrame(&gGohmaCrashAnim);
    self->actionFunc = BossGoma_FloorLandStruckDown;
    self->currentAnimFrameCount = Animation_GetLastFrame(&gGohmaCrashAnim);
}

/**
 * Gohma is vulnerable, from being struck down from the ceiling or on the ground.
 */
void BossGoma_SetupFloorStunned(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaStunnedAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaStunnedAnim),
                     ANIMMODE_LOOP, -2.0f);
    self->actionFunc = BossGoma_FloorStunned;
}

/**
 * Take an attack posture, when the player is close enough.
 */
void BossGoma_SetupFloorAttackPosture(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaPrepareAttackAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gGohmaPrepareAttackAnim), ANIMMODE_ONCE, -10.0f);
    self->actionFunc = BossGoma_FloorAttackPosture;
}

/**
 * Leads to BossGoma_FloorAttack after 1 frame
 */
void BossGoma_SetupFloorPrepareAttack(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaStandAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaStandAnim),
                     ANIMMODE_LOOP, -10.0f);
    self->actionFunc = BossGoma_FloorPrepareAttack;
    self->framesUntilNextAction = 0;
}

void BossGoma_SetupFloorAttack(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaAttackAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaAttackAnim),
                     ANIMMODE_ONCE, -10.0f);
    self->actionFunc = BossGoma_FloorAttack;
    self->actionState = 0;
    self->framesUntilNextAction = 0;
}

/**
 * Plays an animation for Gohma being hit (while stunned)
 * The setup and the action preserve timers apart from the patience one, notably `framesUntilNextAction` which is used
 * as the stun duration
 */
void BossGoma_SetupFloorDamaged(BossGoma* self) {
    Animation_Change(&self->skelanime, &gGohmaDamageAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaDamageAnim),
                     ANIMMODE_ONCE, -2.0f);
    self->actionFunc = BossGoma_FloorDamaged;
}

void BossGoma_UpdateCeilingMovement(BossGoma* self, GlobalContext* globalCtx, f32 dz, f32 targetSpeedXZ,
                                    s16 rotateTowardsCenter) {
    static Vec3f velInit = { 0.0f, 0.0f, 0.0f };
    static Vec3f accelInit = { 0.0f, -0.5f, 0.0f };
    static Vec3f roomCenter = { -150.0f, 0.0f, -350.0f };
    Vec3f* basePos = NULL;
    s16 i;
    Vec3f vel;
    Vec3f accel;
    Vec3f pos;

    roomCenter.z += dz; // dz is always 0
    SkelAnime_Update(&self->skelanime);
    Math_ApproachF(&self->actor.speedXZ, targetSpeedXZ, 0.5f, 2.0f);

    if (rotateTowardsCenter) {
        Math_ApproachS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &roomCenter) + 0x8000, 3,
                       0x3E8);
    }

    if (Animation_OnFrame(&self->skelanime, 9.0f)) {
        basePos = &self->rightHandBackLimbWorldPos;
    } else if (Animation_OnFrame(&self->skelanime, 1.0f)) {
        basePos = &self->leftHandBackLimbWorldPos;
    }

    if (basePos != NULL) {
        for (i = 0; i < 5; i++) {
            vel = velInit;
            accel = accelInit;
            pos.x = Rand_CenteredFloat(70.0f) + basePos->x;
            pos.y = Rand_ZeroFloat(30.0f) + basePos->y;
            pos.z = Rand_CenteredFloat(70.0f) + basePos->z;
            EffectSsHahen_Spawn(globalCtx, &pos, &vel, &accel, 0, (s16)(Rand_ZeroOne() * 5.0f) + 10, -1, 10, NULL);
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_HIGH);
    }
}

void BossGoma_SetupEncounterState4(BossGoma* self, GlobalContext* globalCtx) {
    Player* player;
    Camera* camera;

    camera = Gameplay_GetCamera(globalCtx, 0);
    player = GET_PLAYER(globalCtx);
    self->actionState = 4;
    self->actor.flags |= 1;
    func_80064520(globalCtx, &globalCtx->csCtx);
    func_8002DF54(globalCtx, &self->actor, 1);
    self->subCameraId = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, 0, 3);
    Gameplay_ChangeCameraStatus(globalCtx, self->subCameraId, 7);
    Animation_Change(&self->skelanime, &gGohmaEyeRollAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaEyeRollAnim),
                     ANIMMODE_ONCE, 0.0f);
    self->currentAnimFrameCount = Animation_GetLastFrame(&gGohmaEyeRollAnim);

    // room center (todo: defines for hardcoded positions relative to room center)
    self->actor.world.pos.x = -150.0f;
    self->actor.world.pos.z = -350.0f;

    // room entrance, towards center
    player->actor.world.pos.x = 150.0f;
    player->actor.world.pos.z = 300.0f;

    player->actor.world.rot.y = player->actor.shape.rot.y = -0x705C;
    self->actor.world.rot.y = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) + 0x8000;

    // room entrance, closer to room center
    self->subCameraEye.x = 90.0f;
    self->subCameraEye.z = 170.0f;
    self->subCameraEye.y = camera->eye.y + 20.0f;

    self->framesUntilNextAction = 50;

    self->subCameraAt.x = self->actor.world.pos.x;
    self->subCameraAt.y = self->actor.world.pos.y;
    self->subCameraAt.z = self->actor.world.pos.z;

    Audio_QueueSeqCmd(0x100100FF);
}

/**
 * Spawns the door once the player entered
 * Wait for the player to look at Gohma on the ceiling
 * Handles the "meeting Gohma" cutscene, including boss card
 *
 * Skips the door and look-at-Gohma puzzle if the player already reached the boss card part before
 */
void BossGoma_Encounter(BossGoma* self, GlobalContext* globalCtx) {
    Camera* cam;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad[2];

    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);

    switch (self->actionState) {
        case 0: // wait for the player to enter the room
            // entrance of the boss room
            if (fabsf(player->actor.world.pos.x - 150.0f) < 60.0f &&
                fabsf(player->actor.world.pos.z - 350.0f) < 60.0f) {
                if (gSaveContext.eventChkInf[7] & 1) {
                    BossGoma_SetupEncounterState4(self, globalCtx);
                    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_SHUTTER, 164.72f,
                                       -480.0f, 397.68002f, 0, -0x705C, 0, 0x180);
                } else {
                    func_8002DF54(globalCtx, &self->actor, 8);
                    self->actionState = 1;
                }
            }
            break;

        case 1: // player entered the room
            func_80064520(globalCtx, &globalCtx->csCtx);
            self->subCameraId = Gameplay_CreateSubCamera(globalCtx);
            osSyncPrintf("MAKE CAMERA !!!   1   !!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            Gameplay_ChangeCameraStatus(globalCtx, 0, 1);
            Gameplay_ChangeCameraStatus(globalCtx, self->subCameraId, 7);
            self->actionState = 2;
            // ceiling center
            self->actor.world.pos.x = -150.0f;
            self->actor.world.pos.y = -320.0f;
            self->actor.world.pos.z = -350.0f;
            // room entrance
            player->actor.world.pos.x = 150.0f;
            player->actor.world.pos.z = 300.0f;
            // near ceiling center
            self->subCameraEye.x = -350.0f;
            self->subCameraEye.y = -310.0f;
            self->subCameraEye.z = -350.0f;
            // below room entrance
            self->subCameraAt.x = player->actor.world.pos.x;
            self->subCameraAt.y = player->actor.world.pos.y - 200.0f + 25.0f;
            self->subCameraAt.z = player->actor.world.pos.z;
            self->framesUntilNextAction = 50;
            self->timer = 80;
            self->frameCount = 0;
            self->actor.flags &= ~1;
            // fall-through
        case 2: // zoom on player from room center
            // room entrance, towards center
            player->actor.shape.rot.y = -0x705C;
            player->actor.world.pos.x = 150.0f;
            player->actor.world.pos.z = 300.0f;
            player->actor.world.rot.y = player->actor.shape.rot.y;
            player->actor.speedXZ = 0.0f;

            if (self->framesUntilNextAction == 0) {
                // (-20, 25, -65) is towards room center
                Math_ApproachF(&self->subCameraEye.x, player->actor.world.pos.x - 20.0f, 0.049999997f,
                               self->subCameraFollowSpeed * 50.0f);
                Math_ApproachF(&self->subCameraEye.y, player->actor.world.pos.y + 25.0f, 0.099999994f,
                               self->subCameraFollowSpeed * 130.0f);
                Math_ApproachF(&self->subCameraEye.z, player->actor.world.pos.z - 65.0f, 0.049999997f,
                               self->subCameraFollowSpeed * 30.0f);
                Math_ApproachF(&self->subCameraFollowSpeed, 0.29999998f, 1.0f, 0.0050000004f);
                if (self->timer == 0) {
                    Math_ApproachF(&self->subCameraAt.y, player->actor.world.pos.y + 35.0f, 0.099999994f,
                                   self->subCameraFollowSpeed * 30.0f);
                }
                self->subCameraAt.x = player->actor.world.pos.x;
                self->subCameraAt.z = player->actor.world.pos.z;
            }

            Gameplay_CameraSetAtEye(globalCtx, 0, &self->subCameraAt, &self->subCameraEye);

            if (self->frameCount == 176) {
                Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_SHUTTER, 164.72f, -480.0f,
                                   397.68002f, 0, -0x705C, 0, SHUTTER_GOHMA_BLOCK << 6);
            }

            if (self->frameCount == 176) {
                globalCtx->envCtx.unk_BF = 3;
                globalCtx->envCtx.unk_D6 = 0xFFFF;
            }

            if (self->frameCount == 190) {
                func_8002DF54(globalCtx, &self->actor, 2);
            }

            if (self->frameCount >= 228) {
                cam = Gameplay_GetCamera(globalCtx, 0);
                cam->eye = self->subCameraEye;
                cam->eyeNext = self->subCameraEye;
                cam->at = self->subCameraAt;
                func_800C08AC(globalCtx, self->subCameraId, 0);
                self->subCameraId = 0;
                func_80064534(globalCtx, &globalCtx->csCtx);
                func_8002DF54(globalCtx, &self->actor, 7);
                self->actionState = 3;
            }
            break;

        case 3: // wait for the player to look at Gohma
            if (fabsf(self->actor.projectedPos.x) < 150.0f && fabsf(self->actor.projectedPos.y) < 250.0f &&
                self->actor.projectedPos.z < 800.0f && self->actor.projectedPos.z > 0.0f) {
                self->lookedAtFrames++;
                Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);
                Math_ApproachS(&self->actor.world.rot.y,
                               Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) + 0x8000, 2,
                               0xBB8);
                self->eyeLidBottomRotX = self->eyeLidTopRotX = self->eyeIrisRotX = self->eyeIrisRotY = 0;
            } else {
                self->lookedAtFrames = 0;
                BossGoma_UpdateCeilingMovement(self, globalCtx, 0.0f, -5.0f, true);
            }

            if (self->lookedAtFrames > 15) {
                BossGoma_SetupEncounterState4(self, globalCtx);
            }
            break;

        case 4: // focus Gohma on the ceiling
            if (Animation_OnFrame(&self->skelanime, 15.0f)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_DEMO_EYE);
            }

            if (self->framesUntilNextAction <= 40) {
                // (22, -25, 45) is towards room entrance
                Math_ApproachF(&self->subCameraEye.x, self->actor.world.pos.x + 22.0f, 0.2f, 100.0f);
                Math_ApproachF(&self->subCameraEye.y, self->actor.world.pos.y - 25.0f, 0.2f, 100.0f);
                Math_ApproachF(&self->subCameraEye.z, self->actor.world.pos.z + 45.0f, 0.2f, 100.0f);
                Math_ApproachF(&self->subCameraAt.x, self->actor.world.pos.x, 0.2f, 100.0f);
                Math_ApproachF(&self->subCameraAt.y, self->actor.world.pos.y + 5.0f, 0.2f, 100.0f);
                Math_ApproachF(&self->subCameraAt.z, self->actor.world.pos.z, 0.2f, 100.0f);

                if (self->framesUntilNextAction == 30) {
                    globalCtx->envCtx.unk_BF = 4;
                }

                if (self->framesUntilNextAction < 20) {
                    SkelAnime_Update(&self->skelanime);
                    Math_ApproachF(&self->eyeIrisScaleX, 1.0f, 0.8f, 0.4f);
                    Math_ApproachF(&self->eyeIrisScaleY, 1.0f, 0.8f, 0.4f);

                    if (Animation_OnFrame(&self->skelanime, 36.0f)) {
                        self->eyeIrisScaleX = 1.8f;
                        self->eyeIrisScaleY = 1.8f;
                    }

                    if (Animation_OnFrame(&self->skelanime, self->currentAnimFrameCount)) {
                        self->actionState = 5;
                        Animation_Change(&self->skelanime, &gGohmaWalkAnim, 2.0f, 0.0f,
                                         Animation_GetLastFrame(&gGohmaWalkAnim), ANIMMODE_LOOP, -5.0f);
                        self->framesUntilNextAction = 30;
                        self->subCameraFollowSpeed = 0.0f;
                    }
                }
            }
            break;

        case 5: // running on the ceiling
            // (98, 0, 85) is towards room entrance
            Math_ApproachF(&self->subCameraEye.x, self->actor.world.pos.x + 8.0f + 90.0f, 0.1f,
                           self->subCameraFollowSpeed * 30.0f);
            Math_ApproachF(&self->subCameraEye.y, player->actor.world.pos.y, 0.1f, self->subCameraFollowSpeed * 30.0f);
            Math_ApproachF(&self->subCameraEye.z, self->actor.world.pos.z + 45.0f + 40.0f, 0.1f,
                           self->subCameraFollowSpeed * 30.0f);
            Math_ApproachF(&self->subCameraFollowSpeed, 1.0f, 1.0f, 0.05f);
            self->subCameraAt.x = self->actor.world.pos.x;
            self->subCameraAt.y = self->actor.world.pos.y;
            self->subCameraAt.z = self->actor.world.pos.z;

            if (self->framesUntilNextAction < 0) {
                //! @bug ? unreachable, timer is >= 0
                SkelAnime_Update(&self->skelanime);
                Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);
            } else {
                BossGoma_UpdateCeilingMovement(self, globalCtx, 0.0f, -7.5f, false);
            }

            if (self->framesUntilNextAction == 0) {
                Animation_Change(&self->skelanime, &gGohmaHangAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGohmaHangAnim),
                                 ANIMMODE_LOOP, -5.0f);
            }

            if (self->framesUntilNextAction == 0) {
                self->actionState = 9;
                self->actor.speedXZ = 0.0f;
                self->actor.velocity.y = 0.0f;
                self->actor.gravity = -2.0f;
                Animation_Change(&self->skelanime, &gGohmaInitialLandingAnim, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&gGohmaInitialLandingAnim), ANIMMODE_ONCE, -5.0f);
                player->actor.world.pos.x = 0.0f;
                player->actor.world.pos.z = -30.0f;
            }
            break;

        case 9: // falling from the ceiling
            Math_ApproachF(&self->subCameraEye.x, self->actor.world.pos.x + 8.0f + 90.0f, 0.1f,
                           self->subCameraFollowSpeed * 30.0f);
            Math_ApproachF(&self->subCameraEye.y, player->actor.world.pos.y + 10.0f, 0.1f,
                           self->subCameraFollowSpeed * 30.0f);
            Math_ApproachF(&self->subCameraEye.z, self->actor.world.pos.z + 45.0f + 40.0f, 0.1f,
                           self->subCameraFollowSpeed * 30.0f);
            self->subCameraAt.x = self->actor.world.pos.x;
            self->subCameraAt.y = self->actor.world.pos.y;
            self->subCameraAt.z = self->actor.world.pos.z;
            SkelAnime_Update(&self->skelanime);
            Math_ApproachS(&self->actor.shape.rot.x, 0, 2, 0xBB8);
            Math_ApproachS(&self->actor.world.rot.y,
                           Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor), 2, 0x7D0);

            if (self->actor.bgCheckFlags & 1) {
                self->actionState = 130;
                self->actor.velocity.y = 0.0f;
                Animation_Change(&self->skelanime, &gGohmaInitialLandingAnim, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&gGohmaInitialLandingAnim), ANIMMODE_ONCE, -2.0f);
                self->currentAnimFrameCount = Animation_GetLastFrame(&gGohmaInitialLandingAnim);
                BossGoma_PlayEffectsAndSfx(self, globalCtx, 0, 5);
                self->framesUntilNextAction = 15;
                func_800A9F6C(0.0f, 0xC8, 0x14, 0x14);
            }
            break;

        case 130: // focus Gohma on the ground
            Math_ApproachF(&self->subCameraEye.x, self->actor.world.pos.x + 8.0f + 90.0f, 0.1f,
                           self->subCameraFollowSpeed * 30.0f);
            Math_ApproachF(&self->subCameraEye.y, player->actor.world.pos.y + 10.0f, 0.1f,
                           self->subCameraFollowSpeed * 30.0f);
            Math_ApproachF(&self->subCameraEye.z, self->actor.world.pos.z + 45.0f + 40.0f, 0.1f,
                           self->subCameraFollowSpeed * 30.0f);
            Math_ApproachS(&self->actor.shape.rot.x, 0, 2, 0xBB8);
            Math_ApproachS(&self->actor.world.rot.y,
                           Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor), 2, 0x7D0);
            SkelAnime_Update(&self->skelanime);
            self->subCameraAt.x = self->actor.world.pos.x;
            self->subCameraAt.z = self->actor.world.pos.z;

            if (self->framesUntilNextAction != 0) {
                f32 s = sinf(self->framesUntilNextAction * 3.1415f * 0.5f);
                self->subCameraAt.y = self->framesUntilNextAction * s * 0.7f + self->actor.world.pos.y;
            } else {
                Math_ApproachF(&self->subCameraAt.y, self->actor.focus.pos.y, 0.1f, 10.0f);
            }

            if (Animation_OnFrame(&self->skelanime, 40.0f)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_CRY1);

                if (!(gSaveContext.eventChkInf[7] & 1)) {
                    TitleCard_InitBossName(globalCtx, &globalCtx->actorCtx.titleCtx,
                                           SEGMENTED_TO_VIRTUAL(gGohmaTitleCardTex), 0xA0, 0xB4, 0x80, 0x28);
                }

                Audio_QueueSeqCmd(0x1B);
                gSaveContext.eventChkInf[7] |= 1;
            }

            if (Animation_OnFrame(&self->skelanime, self->currentAnimFrameCount)) {
                self->actionState = 140;
                Animation_Change(&self->skelanime, &gGohmaStandAnim, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&gGohmaStandAnim), ANIMMODE_LOOP, -10.0f);
                self->framesUntilNextAction = 20;
            }
            break;

        case 140:
            SkelAnime_Update(&self->skelanime);
            Math_ApproachF(&self->subCameraAt.y, self->actor.focus.pos.y, 0.1f, 10.0f);

            if (self->framesUntilNextAction == 0) {
                self->framesUntilNextAction = 30;
                self->actionState = 150;
                Gameplay_ChangeCameraStatus(globalCtx, 0, 3);
            }
            break;

        case 150:
            SkelAnime_Update(&self->skelanime);
            Math_SmoothStepToF(&self->subCameraEye.x, self->actor.world.pos.x + 150.0f, 0.2f, 100.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraEye.y, self->actor.world.pos.y + 20.0f, 0.2f, 100.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraEye.z, self->actor.world.pos.z + 220.0f, 0.2f, 100.0f, 0.1f);

            if (self->framesUntilNextAction == 0) {
                cam = Gameplay_GetCamera(globalCtx, 0);
                cam->eye = self->subCameraEye;
                cam->eyeNext = self->subCameraEye;
                cam->at = self->subCameraAt;
                func_800C08AC(globalCtx, self->subCameraId, 0);
                self->subCameraId = 0;
                BossGoma_SetupFloorMain(self);
                self->disableGameplayLogic = false;
                self->patienceTimer = 200;
                func_80064534(globalCtx, &globalCtx->csCtx);
                func_8002DF54(globalCtx, &self->actor, 7);
            }
            break;
    }

    if (self->subCameraId != 0) {
        Gameplay_CameraSetAtEye(globalCtx, self->subCameraId, &self->subCameraAt, &self->subCameraEye);
    }
}

/**
 * Handles the "Gohma defeated" cutscene and effects
 * Spawns the heart container and blue warp actors
 */
void BossGoma_Defeated(BossGoma* self, GlobalContext* globalCtx) {
    static Vec3f roomCenter = { -150.0f, 0.0f, -350.0f };
    f32 dx;
    f32 dz;
    s16 j;
    Vec3f vel1 = { 0.0f, 0.0f, 0.0f };
    Vec3f accel1 = { 0.0f, 1.0f, 0.0f };
    Color_RGBA8 color1 = { 255, 255, 255, 255 };
    Color_RGBA8 color2 = { 0, 100, 255, 255 };
    Vec3f vel2 = { 0.0f, 0.0f, 0.0f };
    Vec3f accel2 = { 0.0f, -0.5f, 0.0f };
    Vec3f pos;
    Camera* camera;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f childPos;
    s16 i;

    SkelAnime_Update(&self->skelanime);
    Math_ApproachS(&self->actor.shape.rot.x, 0, 2, 0xBB8);

    if (Animation_OnFrame(&self->skelanime, 107.0f)) {
        BossGoma_PlayEffectsAndSfx(self, globalCtx, 0, 8);
        func_800A9F6C(0.0f, 0x96, 0x14, 0x14);
    }

    self->visualState = VISUALSTATE_DEFEATED;
    self->eyeState = EYESTATE_IRIS_NO_FOLLOW_NO_IFRAMES;

    if (self->framesUntilNextAction == 1001) {
        for (i = 0; i < 90; i++) {
            if (sDeadLimbLifetime[i] != 0) {
                self->deadLimbsState[i] = 1;
            }
        }
    }

    if (self->framesUntilNextAction < 1200 && self->framesUntilNextAction > 1100 &&
        self->framesUntilNextAction % 8 == 0) {
        EffectSsSibuki_SpawnBurst(globalCtx, &self->actor.focus.pos);
    }

    if (self->framesUntilNextAction < 1080 && self->actionState < 3) {
        if (self->framesUntilNextAction < 1070) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_LAST - SFX_FLAG);
        }

        for (i = 0; i < 4; i++) {
            //! @bug self 0-indexes into self->defeatedLimbPositions which is initialized with
            // self->defeatedLimbPositions[limb], but limb is 1-indexed in skelanime callbacks, self means effects
            // should spawn at self->defeatedLimbPositions[0] too, which is uninitialized, so map origin?
            j = (s16)(Rand_ZeroOne() * (BOSSGOMA_LIMB_MAX - 1));
            if (self->defeatedLimbPositions[j].y < 10000.0f) {
                pos.x = Rand_CenteredFloat(20.0f) + self->defeatedLimbPositions[j].x;
                pos.y = Rand_CenteredFloat(10.0f) + self->defeatedLimbPositions[j].y;
                pos.z = Rand_CenteredFloat(20.0f) + self->defeatedLimbPositions[j].z;
                func_8002836C(globalCtx, &pos, &vel1, &accel1, &color1, &color2, 500, 10, 10);
            }
        }

        for (i = 0; i < 15; i++) {
            //! @bug same as above
            j = (s16)(Rand_ZeroOne() * (BOSSGOMA_LIMB_MAX - 1));
            if (self->defeatedLimbPositions[j].y < 10000.0f) {
                pos.x = Rand_CenteredFloat(20.0f) + self->defeatedLimbPositions[j].x;
                pos.y = Rand_CenteredFloat(10.0f) + self->defeatedLimbPositions[j].y;
                pos.z = Rand_CenteredFloat(20.0f) + self->defeatedLimbPositions[j].z;
                EffectSsHahen_Spawn(globalCtx, &pos, &vel2, &accel2, 0, (s16)(Rand_ZeroOne() * 5.0f) + 10, -1, 10,
                                    NULL);
            }
        }
    }

    switch (self->actionState) {
        case 0:
            self->actionState = 1;
            func_80064520(globalCtx, &globalCtx->csCtx);
            func_8002DF54(globalCtx, &self->actor, 1);
            self->subCameraId = Gameplay_CreateSubCamera(globalCtx);
            Gameplay_ChangeCameraStatus(globalCtx, 0, 3);
            Gameplay_ChangeCameraStatus(globalCtx, self->subCameraId, 7);
            camera = Gameplay_GetCamera(globalCtx, 0);
            self->subCameraEye.x = camera->eye.x;
            self->subCameraEye.y = camera->eye.y;
            self->subCameraEye.z = camera->eye.z;
            self->subCameraAt.x = camera->at.x;
            self->subCameraAt.y = camera->at.y;
            self->subCameraAt.z = camera->at.z;
            dx = self->subCameraEye.x - self->actor.world.pos.x;
            dz = self->subCameraEye.z - self->actor.world.pos.z;
            self->defeatedCameraEyeDist = sqrtf(SQ(dx) + SQ(dz));
            self->defeatedCameraEyeAngle = Math_FAtan2F(dx, dz);
            self->timer = 270;
            break;

        case 1:
            dx = Math_SinS(self->actor.shape.rot.y) * 100.0f;
            dz = Math_CosS(self->actor.shape.rot.y) * 100.0f;
            Math_ApproachF(&player->actor.world.pos.x, self->actor.world.pos.x + dx, 0.5f, 5.0f);
            Math_ApproachF(&player->actor.world.pos.z, self->actor.world.pos.z + dz, 0.5f, 5.0f);

            if (self->framesUntilNextAction < 1080) {
                self->noBackfaceCulling = true;

                for (i = 0; i < 4; i++) {
                    BossGoma_ClearPixels(sClearPixelTableFirstPass, self->decayingProgress);
                    //! @bug self allows self->decayingProgress = 0x100 = 256 which is out of bounds when accessing
                    // sClearPixelTableFirstPass, though timers may prevent self from ever happening?
                    if (self->decayingProgress < 0x100) {
                        self->decayingProgress++;
                    }
                }
            }

            if (self->framesUntilNextAction < 1070 && self->frameCount % 4 == 0 && Rand_ZeroOne() < 0.5f) {
                self->blinkTimer = 3;
            }

            self->defeatedCameraEyeAngle += 0.022f;
            Math_ApproachF(&self->defeatedCameraEyeDist, 150.0f, 0.1f, 5.0f);
            dx = sinf(self->defeatedCameraEyeAngle);
            dx = dx * self->defeatedCameraEyeDist;
            dz = cosf(self->defeatedCameraEyeAngle);
            dz = dz * self->defeatedCameraEyeDist;
            Math_SmoothStepToF(&self->subCameraEye.x, self->actor.world.pos.x + dx, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraEye.y, self->actor.world.pos.y + 20.0f, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraEye.z, self->actor.world.pos.z + dz, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraAt.x, self->firstTailLimbWorldPos.x, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraAt.y, self->actor.focus.pos.y, 0.5f, 100.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraAt.z, self->firstTailLimbWorldPos.z, 0.2f, 50.0f, 0.1f);

            if (self->timer == 80) {
                Audio_QueueSeqCmd(0x21);
            }

            if (self->timer == 0) {
                self->actionState = 2;
                Gameplay_ChangeCameraStatus(globalCtx, 0, 3);
                self->timer = 70;
                self->decayingProgress = 0;
                self->subCameraFollowSpeed = 0.0f;
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, self->actor.world.pos.x,
                            self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
            }
            break;

        case 2:
            camera = Gameplay_GetCamera(globalCtx, 0);
            Math_SmoothStepToF(&self->subCameraEye.x, camera->eye.x, 0.2f, self->subCameraFollowSpeed * 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraEye.y, camera->eye.y, 0.2f, self->subCameraFollowSpeed * 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraEye.z, camera->eye.z, 0.2f, self->subCameraFollowSpeed * 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraAt.x, camera->at.x, 0.2f, self->subCameraFollowSpeed * 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraAt.y, camera->at.y, 0.2f, self->subCameraFollowSpeed * 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraAt.z, camera->at.z, 0.2f, self->subCameraFollowSpeed * 50.0f, 0.1f);
            Math_SmoothStepToF(&self->subCameraFollowSpeed, 1.0f, 1.0f, 0.02f, 0.0f);

            if (self->timer == 0) {
                childPos = roomCenter;
                self->timer = 30;
                self->actionState = 3;

                for (i = 0; i < 10000; i++) {
                    if ((fabsf(childPos.x - player->actor.world.pos.x) < 100.0f &&
                         fabsf(childPos.z - player->actor.world.pos.z) < 100.0f) ||
                        (fabsf(childPos.x - self->actor.world.pos.x) < 150.0f &&
                         fabsf(childPos.z - self->actor.world.pos.z) < 150.0f)) {
                        childPos.x = Rand_CenteredFloat(400.0f) + -150.0f;
                        childPos.z = Rand_CenteredFloat(400.0f) + -350.0f;
                    } else {
                        break;
                    }
                }

                Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, childPos.x,
                                   self->actor.world.pos.y, childPos.z, 0, 0, 0, WARP_DUNGEON_CHILD);
                Flags_SetClear(globalCtx, globalCtx->roomCtx.curRoom.num);
            }

            for (i = 0; i < 4; i++) {
                BossGoma_ClearPixels(sClearPixelTableSecondPass, self->decayingProgress);
                //! @bug same as sClearPixelTableFirstPass
                if (self->decayingProgress < 0x100) {
                    self->decayingProgress++;
                }
            }
            break;

        case 3:
            for (i = 0; i < 4; i++) {
                BossGoma_ClearPixels(sClearPixelTableSecondPass, self->decayingProgress);
                //! @bug same as sClearPixelTableFirstPass
                if (self->decayingProgress < 0x100) {
                    self->decayingProgress++;
                }
            }

            if (self->timer == 0) {
                if (Math_SmoothStepToF(&self->actor.scale.y, 0, 1.0f, 0.00075f, 0.0f) <= 0.001f) {
                    camera = Gameplay_GetCamera(globalCtx, 0);
                    camera->eye = self->subCameraEye;
                    camera->eyeNext = self->subCameraEye;
                    camera->at = self->subCameraAt;
                    func_800C08AC(globalCtx, self->subCameraId, 0);
                    self->subCameraId = 0;
                    func_80064534(globalCtx, &globalCtx->csCtx);
                    func_8002DF54(globalCtx, &self->actor, 7);
                    Actor_Kill(&self->actor);
                }

                self->actor.scale.x = self->actor.scale.z = self->actor.scale.y;
            }
            break;
    }

    if (self->subCameraId != 0) {
        Gameplay_CameraSetAtEye(globalCtx, self->subCameraId, &self->subCameraAt, &self->subCameraEye);
    }

    if (self->blinkTimer != 0) {
        self->blinkTimer--;
        globalCtx->envCtx.adjAmbientColor[0] += 40;
        globalCtx->envCtx.adjAmbientColor[1] += 40;
        globalCtx->envCtx.adjAmbientColor[2] += 80;
        globalCtx->envCtx.adjFogColor[0] += 10;
        globalCtx->envCtx.adjFogColor[1] += 10;
        globalCtx->envCtx.adjFogColor[2] += 20;
    } else {
        globalCtx->envCtx.adjAmbientColor[0] -= 20;
        globalCtx->envCtx.adjAmbientColor[1] -= 20;
        globalCtx->envCtx.adjAmbientColor[2] -= 40;
        globalCtx->envCtx.adjFogColor[0] -= 5;
        globalCtx->envCtx.adjFogColor[1] -= 5;
        globalCtx->envCtx.adjFogColor[2] -= 10;
    }

    if (globalCtx->envCtx.adjAmbientColor[0] > 200) {
        globalCtx->envCtx.adjAmbientColor[0] = 200;
    }
    if (globalCtx->envCtx.adjAmbientColor[1] > 200) {
        globalCtx->envCtx.adjAmbientColor[1] = 200;
    }
    if (globalCtx->envCtx.adjAmbientColor[2] > 200) {
        globalCtx->envCtx.adjAmbientColor[2] = 200;
    }
    if (globalCtx->envCtx.adjFogColor[0] > 70) {
        globalCtx->envCtx.adjFogColor[0] = 70;
    }
    if (globalCtx->envCtx.adjFogColor[1] > 70) {
        globalCtx->envCtx.adjFogColor[1] = 70;
    }
    if (globalCtx->envCtx.adjFogColor[2] > 140) {
        globalCtx->envCtx.adjFogColor[2] = 140;
    }

    if (globalCtx->envCtx.adjAmbientColor[0] < 0) {
        globalCtx->envCtx.adjAmbientColor[0] = 0;
    }
    if (globalCtx->envCtx.adjAmbientColor[1] < 0) {
        globalCtx->envCtx.adjAmbientColor[1] = 0;
    }
    if (globalCtx->envCtx.adjAmbientColor[2] < 0) {
        globalCtx->envCtx.adjAmbientColor[2] = 0;
    }
    if (globalCtx->envCtx.adjFogColor[0] < 0) {
        globalCtx->envCtx.adjFogColor[0] = 0;
    }
    if (globalCtx->envCtx.adjFogColor[1] < 0) {
        globalCtx->envCtx.adjFogColor[1] = 0;
    }
    if (globalCtx->envCtx.adjFogColor[2] < 0) {
        globalCtx->envCtx.adjFogColor[2] = 0;
    }
}

/**
 * If the player backs off, cancel the attack, or attack.
 */
void BossGoma_FloorAttackPosture(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);

    if (self->skelanime.curFrame >= (19.0f + 1.0f / 3.0f) && self->skelanime.curFrame <= 30.0f) {
        Math_ApproachS(&self->actor.world.rot.y, Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor),
                       3, 0xBB8);
    }

    if (Animation_OnFrame(&self->skelanime, Animation_GetLastFrame(&gGohmaPrepareAttackAnim))) {
        if (self->actor.xzDistToPlayer < 250.0f) {
            BossGoma_SetupFloorPrepareAttack(self);
        } else {
            BossGoma_SetupFloorMain(self);
        }
    }

    self->eyeState = EYESTATE_IRIS_FOLLOW_NO_IFRAMES;
    self->visualState = VISUALSTATE_RED;
}

/**
 * Only lasts 1 frame. Plays a sound.
 */
void BossGoma_FloorPrepareAttack(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->framesUntilNextAction == 0) {
        BossGoma_SetupFloorAttack(self);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_CRY1);
    }

    self->eyeState = EYESTATE_IRIS_FOLLOW_NO_IFRAMES;
    self->visualState = VISUALSTATE_RED;
}

/**
 * Gohma attacks, then the action eventually goes back to BossGoma_FloorMain
 */
void BossGoma_FloorAttack(BossGoma* self, GlobalContext* globalCtx) {
    s16 i;

    self->actor.flags |= 0x1000000;
    SkelAnime_Update(&self->skelanime);

    switch (self->actionState) {
        case 0:
            for (i = 0; i < self->collider.count; i++) {
                if (self->collider.elements[i].info.toucherFlags & 2) {
                    self->framesUntilNextAction = 10;
                    break;
                }
            }

            if (Animation_OnFrame(&self->skelanime, 10.0f)) {
                BossGoma_PlayEffectsAndSfx(self, globalCtx, 3, 5);
                func_80033E88(&self->actor, globalCtx, 5, 15);
            }

            if (Animation_OnFrame(&self->skelanime, Animation_GetLastFrame(&gGohmaAttackAnim))) {
                self->actionState = 1;
                Animation_Change(&self->skelanime, &gGohmaRestAfterAttackAnim, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&gGohmaRestAfterAttackAnim), ANIMMODE_LOOP, -1.0f);

                if (self->framesUntilNextAction == 0) {
                    self->timer = (s16)(Rand_ZeroOne() * 30.0f) + 30;
                }
            }
            break;

        case 1:
            if (Animation_OnFrame(&self->skelanime, 3.0f)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_UNARI2);
            }

            if (self->timer == 0) {
                self->actionState = 2;
                Animation_Change(&self->skelanime, &gGohmaRecoverAfterAttackAnim, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&gGohmaRecoverAfterAttackAnim), ANIMMODE_ONCE, -5.0f);
            }
            break;

        case 2:
            if (Animation_OnFrame(&self->skelanime, Animation_GetLastFrame(&gGohmaRecoverAfterAttackAnim))) {
                BossGoma_SetupFloorIdle(self);
            }
            break;
    }

    self->eyeState = EYESTATE_IRIS_FOLLOW_NO_IFRAMES;
    self->visualState = VISUALSTATE_RED;
}

/**
 * Plays the animation to its end, then goes back to BossGoma_FloorStunned
 */
void BossGoma_FloorDamaged(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (Animation_OnFrame(&self->skelanime, Animation_GetLastFrame(&gGohmaDamageAnim))) {
        BossGoma_SetupFloorStunned(self);
        self->patienceTimer = 0;
    }

    self->eyeState = EYESTATE_IRIS_NO_FOLLOW_NO_IFRAMES;
    Math_ApproachF(&self->eyeIrisScaleX, 0.4f, 0.5f, 0.2f);
    self->visualState = VISUALSTATE_HIT;
}

/**
 * Gohma is back on the floor after the player struck it down from the ceiling.
 * Sets patience to 0
 * Gohma is then stunned (BossGoma_FloorStunned)
 */
void BossGoma_FloorLandStruckDown(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (Animation_OnFrame(&self->skelanime, self->currentAnimFrameCount)) {
        BossGoma_SetupFloorStunned(self);
        self->sfxFaintTimer = 92;
        self->patienceTimer = 0;
        self->framesUntilNextAction = 150;
    }

    Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 55.0f, 4, 8.0f, 500, 10, 1);
}

/**
 * Gohma is back on the floor after the player has killed its children Gohmas.
 * Plays an animation then goes to usual floor behavior, with refilled patience.
 */
void BossGoma_FloorLand(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (Animation_OnFrame(&self->skelanime, self->currentAnimFrameCount)) {
        BossGoma_SetupFloorIdle(self);
        self->patienceTimer = 200;
    }
}

/**
 * Gohma is stunned and vulnerable. It can only be damaged during self action.
 */
void BossGoma_FloorStunned(BossGoma* self, GlobalContext* globalCtx) {
    if (self->sfxFaintTimer <= 90) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_FAINT - 0x800);
    }
    SkelAnime_Update(&self->skelanime);

    if (self->timer == 1) {
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 55.0f, 4, 8.0f, 500, 10, 1);
    }

    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 1.0f);

    if (self->framesUntilNextAction == 0) {
        BossGoma_SetupFloorMain(self);
        if (self->patienceTimer == 0 && self->actor.xzDistToPlayer < 130.0f) {
            self->timer = 20;
        }
    }

    Math_ApproachS(&self->actor.shape.rot.x, 0, 2, 0xBB8);
    self->eyeState = EYESTATE_IRIS_NO_FOLLOW_NO_IFRAMES;
    Math_ApproachF(&self->eyeIrisScaleX, 0.4f, 0.5f, 0.2f);
    self->visualState = VISUALSTATE_STUNNED;
}

/**
 * Gohma goes back to the floor after the player killed the three gohmas it spawned
 */
void BossGoma_FallJump(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    Math_ApproachS(&self->actor.shape.rot.x, 0, 2, 0xBB8);
    Math_ApproachS(&self->actor.world.rot.y, Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor), 2,
                   0x7D0);

    if (self->actor.bgCheckFlags & 1) {
        BossGoma_SetupFloorLand(self);
        self->actor.velocity.y = 0.0f;
        BossGoma_PlayEffectsAndSfx(self, globalCtx, 0, 8);
        func_80033E88(&self->actor, globalCtx, 5, 0xF);
    }
}

/**
 * Gohma falls to the floor after the player hit it
 */
void BossGoma_FallStruckDown(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    Math_ApproachS(&self->actor.shape.rot.x, 0, 2, 0xBB8);
    Math_ApproachS(&self->actor.world.rot.y, Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor), 3,
                   0x7D0);

    if (self->actor.bgCheckFlags & 1) {
        BossGoma_SetupFloorLandStruckDown(self);
        self->actor.velocity.y = 0.0f;
        BossGoma_PlayEffectsAndSfx(self, globalCtx, 0, 8);
        func_80033E88(&self->actor, globalCtx, 0xA, 0xF);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_DAM1);
    }
}

/**
 * Spawn three gohmas, one after the other. Cannot be interrupted
 */
void BossGoma_CeilingSpawnGohmas(BossGoma* self, GlobalContext* globalCtx) {
    s16 i;

    SkelAnime_Update(&self->skelanime);

    if (self->frameCount % 16 == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_UNARI);
    }

    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);
    self->spawnGohmasActionTimer++;

    switch (self->spawnGohmasActionTimer) {
        case 24:
            // BOSSGOMA_LIMB_TAIL1, the tail limb closest to the body
            self->tailLimbsScaleTimers[3] = 10;
            break;
        case 32:
            // BOSSGOMA_LIMB_TAIL2
            self->tailLimbsScaleTimers[2] = 10;
            break;
        case 40:
            // BOSSGOMA_LIMB_TAIL3
            self->tailLimbsScaleTimers[1] = 10;
            break;
        case 48:
            // BOSSGOMA_LIMB_TAIL4, the furthest from the body
            self->tailLimbsScaleTimers[0] = 10;
            break;
    }

    if (self->tailLimbsScaleTimers[0] == 2) {
        for (i = 0; i < ARRAY_COUNT(self->childrenGohmaState); i++) {
            if (self->childrenGohmaState[i] == 0) {
                BossGoma_SpawnChildGohma(self, globalCtx, i);
                break;
            }
        }

        if (self->childrenGohmaState[0] == 0 || self->childrenGohmaState[1] == 0 || self->childrenGohmaState[2] == 0) {
            self->spawnGohmasActionTimer = 23;
        }
    }

    if (self->spawnGohmasActionTimer >= 64) {
        BossGoma_SetupCeilingIdle(self);
    }

    self->eyeState = EYESTATE_IRIS_NO_FOLLOW_NO_IFRAMES;
}

/**
 * Prepare to spawn children gohmas, red eye for 70 frames
 * During self time, the player can interrupt by hitting Gohma and make it fall from the ceiling
 */
void BossGoma_CeilingPrepareSpawnGohmas(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->framesUntilNextAction == 0) {
        BossGoma_SetupCeilingSpawnGohmas(self);
    }

    self->eyeState = EYESTATE_IRIS_NO_FOLLOW_NO_IFRAMES;
    self->visualState = VISUALSTATE_RED;
}

/**
 * On the floor, not doing anything special.
 */
void BossGoma_FloorIdle(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);
    Math_ApproachS(&self->actor.shape.rot.x, 0, 2, 0xBB8);

    if (self->framesUntilNextAction == 0) {
        BossGoma_SetupFloorMain(self);
    }
}

/**
 * On the ceiling, not doing anything special.
 * Eventually spawns children gohmas, jumping down to the floor when they are killed, or staying on the ceiling as long
 * as any is still alive.
 */
void BossGoma_CeilingIdle(BossGoma* self, GlobalContext* globalCtx) {
    s16 i;

    SkelAnime_Update(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);

    if (self->framesUntilNextAction == 0) {
        if (self->childrenGohmaState[0] == 0 && self->childrenGohmaState[1] == 0 && self->childrenGohmaState[2] == 0) {
            // if no child gohma has been spawned
            BossGoma_SetupCeilingPrepareSpawnGohmas(self);
        } else if (self->childrenGohmaState[0] < 0 && self->childrenGohmaState[1] < 0 &&
                   self->childrenGohmaState[2] < 0) {
            // if all children gohmas are dead
            BossGoma_SetupFallJump(self);
        } else {
            for (i = 0; i < ARRAY_COUNT(self->childrenGohmaState); i++) {
                if (self->childrenGohmaState[i] == 0) {
                    // if any child gohma hasn't been spawned
                    // self seems unreachable since BossGoma_CeilingSpawnGohmas spawns all three and can't be
                    // interrupted
                    BossGoma_SetupCeilingSpawnGohmas(self);
                    return;
                }
            }
            // if all children gohmas have been spawned
            BossGoma_SetupCeilingMoveToCenter(self);
        }
    }
}

/**
 * Gohma approaches the player as long as it has patience (see patienceTimer), then moves away from the player
 * Gohma climbs any wall it collides with
 * Uses the "walk cautiously" animation
 */
void BossGoma_FloorMain(BossGoma* self, GlobalContext* globalCtx) {
    s16 rot;

    SkelAnime_Update(&self->skelanime);

    if (Animation_OnFrame(&self->skelanime, 1.0f)) {
        self->doNotMoveThisFrame = true;
    } else if (Animation_OnFrame(&self->skelanime, 30.0f)) {
        self->doNotMoveThisFrame = true;
    } else if (Animation_OnFrame(&self->skelanime, 15.0f)) {
        self->doNotMoveThisFrame = true;
    } else if (Animation_OnFrame(&self->skelanime, 16.0f)) {
        self->doNotMoveThisFrame = true;
    }

    if (Animation_OnFrame(&self->skelanime, 15.0f)) {
        BossGoma_PlayEffectsAndSfx(self, globalCtx, 1, 3);
    } else if (Animation_OnFrame(&self->skelanime, 30.0f)) {
        BossGoma_PlayEffectsAndSfx(self, globalCtx, 2, 3);
    }

    if (self->frameCount % 64 == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_CRY2);
    }

    if (!self->doNotMoveThisFrame) {
        rot = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor);

        if (self->patienceTimer != 0) {
            self->patienceTimer--;

            if (self->actor.xzDistToPlayer < 150.0f) {
                BossGoma_SetupFloorAttackPosture(self);
            }

            Math_ApproachF(&self->actor.speedXZ, 3.3333333f, 0.5f, 2.0f);
            Math_ApproachS(&self->actor.world.rot.y, rot, 5, 0x3E8);
        } else {
            if (self->timer != 0) {
                // move away from the player, walking backwards
                Math_ApproachF(&self->actor.speedXZ, -10.0f, 0.5f, 2.0f);
                self->skelanime.playSpeed = -3.0f;
                if (self->timer == 1) {
                    self->actor.speedXZ = 0.0f;
                }
            } else {
                // move away from the player, walking forwards
                Math_ApproachF(&self->actor.speedXZ, 6.6666665f, 0.5f, 2.0f);
                self->skelanime.playSpeed = 2.0f;
                rot += 0x8000;
            }

            Math_ApproachS(&self->actor.world.rot.y, rot, 3, 0x9C4);
        }
    }

    if (self->actor.bgCheckFlags & 1) {
        self->actor.velocity.y = 0.0f;
    }

    if (self->actor.bgCheckFlags & 8) {
        BossGoma_SetupWallClimb(self);
    }

    if (self->framesUntilNextAction == 0 && self->patienceTimer != 0) {
        BossGoma_SetupFloorIdle(self);
    }
}

/**
 * Gohma moves up until it reaches the ceiling
 */
void BossGoma_WallClimb(BossGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->frameCount % 8 == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_CLIM);
    }

    Math_ApproachF(&self->actor.velocity.y, 5.0f, 0.5f, 2.0f);
    Math_ApproachS(&self->actor.shape.rot.x, -0x4000, 2, 0x7D0);
    Math_ApproachS(&self->actor.world.rot.y, self->actor.wallYaw + 0x8000, 2, 0x5DC);

    // -320 is a bit below boss room ceiling
    if (self->actor.world.pos.y > -320.0f) {
        BossGoma_SetupCeilingMoveToCenter(self);
        // allow new spawns
        self->childrenGohmaState[0] = self->childrenGohmaState[1] = self->childrenGohmaState[2] = 0;
    }
}

/**
 * Goes to BossGoma_CeilingIdle after enough time and after being close enough to the center of the ceiling.
 */
void BossGoma_CeilingMoveToCenter(BossGoma* self, GlobalContext* globalCtx) {
    s16 angle;
    s16 absDiff;

    BossGoma_UpdateCeilingMovement(self, globalCtx, 0.0f, -5.0f, true);

    if (self->frameCount % 64 == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_CRY2);
    }

    Math_ApproachS(&self->actor.shape.rot.x, -0x8000, 3, 0x3E8);

    // avoid walking into a wall?
    if (self->actor.bgCheckFlags & 8) {
        angle = self->actor.shape.rot.y + 0x8000;

        if (angle < self->actor.wallYaw) {
            absDiff = self->actor.wallYaw - angle;
            angle = angle + absDiff / 2;
        } else {
            absDiff = angle - self->actor.wallYaw;
            angle = self->actor.wallYaw + absDiff / 2;
        }

        self->actor.world.pos.z += Math_CosS(angle) * (5.0f + Rand_ZeroOne() * 5.0f) + Rand_CenteredFloat(2.0f);
        self->actor.world.pos.x += Math_SinS(angle) * (5.0f + Rand_ZeroOne() * 5.0f) + Rand_CenteredFloat(2.0f);
    }

    // timer setup to 30-60
    if (self->framesUntilNextAction == 0 && fabsf(-150.0f - self->actor.world.pos.x) < 100.0f &&
        fabsf(-350.0f - self->actor.world.pos.z) < 100.0f) {
        BossGoma_SetupCeilingIdle(self);
    }
}

/**
 * Update eye-related properties
 *  - open/close (eye lid rotation)
 *  - look at the player (iris rotation)
 *  - iris scale, when menacing or damaged
 */
void BossGoma_UpdateEye(BossGoma* self, GlobalContext* globalCtx) {
    s16 targetEyeIrisRotX;
    s16 targetEyeIrisRotY;

    if (!self->disableGameplayLogic) {
        Player* player = GET_PLAYER(globalCtx);

        if (self->eyeState == EYESTATE_IRIS_FOLLOW_BONUS_IFRAMES) {
            // player + 0xA73 seems to be related to "throwing something"
            if (player->unk_A73 != 0) {
                player->unk_A73 = 0;
                self->eyeClosedTimer = 12;
            }

            if (self->frameCount % 16 == 0 && Rand_ZeroOne() < 0.3f) {
                self->eyeClosedTimer = 7;
            }
        }

        if (self->childrenGohmaState[0] > 0 || self->childrenGohmaState[1] > 0 || self->childrenGohmaState[2] > 0) {
            self->eyeClosedTimer = 7;
        }

        if (self->eyeClosedTimer != 0) {
            self->eyeClosedTimer--;
            // close eye
            Math_ApproachS(&self->eyeLidBottomRotX, -0xA98, 1, 0x7D0);
            Math_ApproachS(&self->eyeLidTopRotX, 0x1600, 1, 0x7D0);
        } else {
            // open eye
            Math_ApproachS(&self->eyeLidBottomRotX, 0, 1, 0x7D0);
            Math_ApproachS(&self->eyeLidTopRotX, 0, 1, 0x7D0);
        }

        if (self->eyeState != EYESTATE_IRIS_NO_FOLLOW_NO_IFRAMES) {
            targetEyeIrisRotY =
                Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.shape.rot.y;
            targetEyeIrisRotX =
                Actor_WorldPitchTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.shape.rot.x;

            if (self->actor.shape.rot.x > 0x4000 || self->actor.shape.rot.x < -0x4000) {
                targetEyeIrisRotY = -(s16)(targetEyeIrisRotY + 0x8000);
                targetEyeIrisRotX = -0xBB8;
            }

            if (targetEyeIrisRotY > 0x1770) {
                targetEyeIrisRotY = 0x1770;
            }

            if (targetEyeIrisRotY < -0x1770) {
                targetEyeIrisRotY = -0x1770;
            }

            Math_ApproachS(&self->eyeIrisRotY, targetEyeIrisRotY, 3, 0x7D0);
            Math_ApproachS(&self->eyeIrisRotX, targetEyeIrisRotX, 3, 0x7D0);
        } else {
            Math_ApproachS(&self->eyeIrisRotY, 0, 3, 0x3E8);
            Math_ApproachS(&self->eyeIrisRotX, 0, 3, 0x3E8);
        }

        Math_ApproachF(&self->eyeIrisScaleX, 1.0f, 0.2f, 0.07f);
        Math_ApproachF(&self->eyeIrisScaleY, 1.0f, 0.2f, 0.07f);
    }
}

/**
 * Part of achieving visual effects when spawning children gohmas,
 * inflating each tail limb one after the other.
 */
void BossGoma_UpdateTailLimbsScale(BossGoma* self) {
    s16 i;

    if (self->frameCount % 128 == 0) {
        self->unusedTimer++;
        if (self->unusedTimer >= 3) {
            self->unusedTimer = 0;
        }
    }

    // See BossGoma_CeilingSpawnGohmas for `tailLimbsScaleTimers` usage
    for (i = 0; i < ARRAY_COUNT(self->tailLimbsScaleTimers); i++) {
        if (self->tailLimbsScaleTimers[i] != 0) {
            self->tailLimbsScaleTimers[i]--;
            Math_ApproachF(&self->tailLimbsScale[i], 1.5f, 0.2f, 0.1f);
        } else {
            Math_ApproachF(&self->tailLimbsScale[i], 1.0f, 0.2f, 0.1f);
        }
    }
}

void BossGoma_UpdateHit(BossGoma* self, GlobalContext* globalCtx) {
    if (self->invincibilityFrames != 0) {
        self->invincibilityFrames--;
    } else {
        ColliderInfo* acHitInfo = self->collider.elements[0].info.acHitInfo;
        s32 damage;

        if (self->eyeClosedTimer == 0 && self->actionFunc != BossGoma_CeilingSpawnGohmas &&
            (self->collider.elements[0].info.bumperFlags & BUMP_HIT)) {
            self->collider.elements[0].info.bumperFlags &= ~BUMP_HIT;

            if (self->actionFunc == BossGoma_CeilingMoveToCenter || self->actionFunc == BossGoma_CeilingIdle ||
                self->actionFunc == BossGoma_CeilingPrepareSpawnGohmas) {
                BossGoma_SetupFallStruckDown(self);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_DAM2);
            } else if (self->actionFunc == BossGoma_FloorStunned &&
                       (damage = CollisionCheck_GetSwordDamage(acHitInfo->toucher.dmgFlags)) != 0) {
                self->actor.colChkInfo.health -= damage;

                if ((s8)self->actor.colChkInfo.health > 0) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_DAM1);
                    BossGoma_SetupFloorDamaged(self);
                    EffectSsSibuki_SpawnBurst(globalCtx, &self->actor.focus.pos);
                } else {
                    BossGoma_SetupDefeated(self, globalCtx);
                    Enemy_StartFinishingBlow(globalCtx, &self->actor);
                }

                self->invincibilityFrames = 10;
            } else if (self->actionFunc != BossGoma_FloorStunned && self->patienceTimer != 0 &&
                       (acHitInfo->toucher.dmgFlags & 0x00000005)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_DAM2);
                Audio_StopSfx(NA_SE_EN_GOMA_CRY1);
                self->invincibilityFrames = 10;
                BossGoma_SetupFloorStunned(self);
                self->sfxFaintTimer = 100;

                if (acHitInfo->toucher.dmgFlags & 1) {
                    self->framesUntilNextAction = 40;
                } else {
                    self->framesUntilNextAction = 90;
                }

                self->timer = 4;
                func_80033E88(&self->actor, globalCtx, 4, 0xC);
            }
        }
    }
}

void BossGoma_UpdateMainEnvColor(BossGoma* self) {
    static f32 colors1[][3] = {
        { 255.0f, 17.0f, 0.0f },  { 0.0f, 255.0f, 170.0f }, { 50.0f, 50.0f, 50.0f },
        { 0.0f, 255.0f, 170.0f }, { 0.0f, 255.0f, 170.0f }, { 0.0f, 255.0f, 170.0f },
    };
    static f32 colors2[][3] = {
        { 255.0f, 17.0f, 0.0f },  { 0.0f, 255.0f, 170.0f }, { 50.0f, 50.0f, 50.0f },
        { 0.0f, 255.0f, 170.0f }, { 0.0f, 0.0f, 255.0f },   { 255.0f, 17.0f, 0.0f },
    };

    if (self->visualState == VISUALSTATE_DEFAULT && self->frameCount & 0x10) {
        Math_ApproachF(&self->mainEnvColor[0], 50.0f, 0.5f, 20.0f);
        Math_ApproachF(&self->mainEnvColor[1], 50.0f, 0.5f, 20.0f);
        Math_ApproachF(&self->mainEnvColor[2], 50.0f, 0.5f, 20.0f);
    } else if (self->invincibilityFrames != 0) {
        if (self->invincibilityFrames & 2) {
            self->mainEnvColor[0] = colors2[self->visualState][0];
            self->mainEnvColor[1] = colors2[self->visualState][1];
            self->mainEnvColor[2] = colors2[self->visualState][2];
        } else {
            self->mainEnvColor[0] = colors1[self->visualState][0];
            self->mainEnvColor[1] = colors1[self->visualState][1];
            self->mainEnvColor[2] = colors1[self->visualState][2];
        }
    } else {
        Math_ApproachF(&self->mainEnvColor[0], colors1[self->visualState][0], 0.5f, 20.0f);
        Math_ApproachF(&self->mainEnvColor[1], colors1[self->visualState][1], 0.5f, 20.0f);
        Math_ApproachF(&self->mainEnvColor[2], colors1[self->visualState][2], 0.5f, 20.0f);
    }
}

void BossGoma_UpdateEyeEnvColor(BossGoma* self) {
    static f32 targetEyeEnvColors[][3] = {
        { 255.0f, 17.0f, 0.0f },  { 255.0f, 255.0f, 255.0f }, { 50.0f, 50.0f, 50.0f },
        { 0.0f, 255.0f, 170.0f }, { 0.0f, 255.0f, 170.0f },   { 0.0f, 255.0f, 170.0f },
    };

    Math_ApproachF(&self->eyeEnvColor[0], targetEyeEnvColors[self->visualState][0], 0.5f, 20.0f);
    Math_ApproachF(&self->eyeEnvColor[1], targetEyeEnvColors[self->visualState][1], 0.5f, 20.0f);
    Math_ApproachF(&self->eyeEnvColor[2], targetEyeEnvColors[self->visualState][2], 0.5f, 20.0f);
}

void BossGoma_Update(Actor* thisx, GlobalContext* globalCtx) {
    BossGoma* self = THIS;
    s32 pad;

    self->visualState = VISUALSTATE_DEFAULT;
    self->frameCount++;

    if (self->framesUntilNextAction != 0) {
        self->framesUntilNextAction--;
    }

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->sfxFaintTimer != 0) {
        self->sfxFaintTimer--;
    }

    if (1) {}

    self->eyeState = EYESTATE_IRIS_FOLLOW_BONUS_IFRAMES;
    self->actionFunc(self, globalCtx);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (!self->doNotMoveThisFrame) {
        Actor_MoveForward(&self->actor);
    } else {
        self->doNotMoveThisFrame = false;
    }

    if (self->actor.world.pos.y < -400.0f) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 30.0f, 30.0f, 80.0f, 5);
    } else {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 30.0f, 80.0f, 1);
    }

    BossGoma_UpdateEye(self, globalCtx);
    BossGoma_UpdateMainEnvColor(self);
    BossGoma_UpdateEyeEnvColor(self);
    BossGoma_UpdateTailLimbsScale(self);

    if (!self->disableGameplayLogic) {
        BossGoma_UpdateHit(self, globalCtx);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

        if (self->actionFunc != BossGoma_FloorStunned && self->actionFunc != BossGoma_FloorDamaged &&
            (self->actionFunc != BossGoma_FloorMain || self->timer == 0)) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

s32 BossGoma_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                              void* thisx) {
    BossGoma* self = THIS;
    s32 doNotDrawLimb = false;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_goma.c", 4685);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, (s16)self->mainEnvColor[0], (s16)self->mainEnvColor[1], (s16)self->mainEnvColor[2],
                   255);

    if (self->deadLimbsState[limbIndex] >= 2) {
        *dList = NULL;
    }

    switch (limbIndex) {
        case BOSSGOMA_LIMB_EYE:
            if (self->eyeState == EYESTATE_IRIS_FOLLOW_BONUS_IFRAMES && self->eyeLidBottomRotX < -0xA8C) {
                *dList = NULL;
            } else if (self->invincibilityFrames != 0) {
                gDPSetEnvColor(POLY_OPA_DISP++, (s16)(Rand_ZeroOne() * 255.0f), (s16)(Rand_ZeroOne() * 255.0f),
                               (s16)(Rand_ZeroOne() * 255.0f), 63);
            } else {
                gDPSetEnvColor(POLY_OPA_DISP++, (s16)self->eyeEnvColor[0], (s16)self->eyeEnvColor[1],
                               (s16)self->eyeEnvColor[2], 63);
            }
            break;

        case BOSSGOMA_LIMB_EYE_LID_BOTTOM_ROOT2:
            rot->x += self->eyeLidBottomRotX;
            break;

        case BOSSGOMA_LIMB_EYE_LID_TOP_ROOT2:
            rot->x += self->eyeLidTopRotX;
            break;

        case BOSSGOMA_LIMB_IRIS_ROOT2:
            rot->x += self->eyeIrisRotX;
            rot->y += self->eyeIrisRotY;
            break;

        case BOSSGOMA_LIMB_IRIS:
            if (self->eyeState == EYESTATE_IRIS_FOLLOW_BONUS_IFRAMES && self->eyeLidBottomRotX < -0xA8C) {
                *dList = NULL;
            } else {
                if (self->visualState == VISUALSTATE_DEFEATED) {
                    gDPSetEnvColor(POLY_OPA_DISP++, 50, 50, 50, 255);
                } else {
                    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
                }

                Matrix_JointPosition(pos, rot);

                if (*dList != NULL) {
                    Matrix_Push();
                    Matrix_Scale(self->eyeIrisScaleX, self->eyeIrisScaleY, 1.0f, MTXMODE_APPLY);
                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_goma.c", 4815),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(POLY_OPA_DISP++, *dList);
                    Matrix_Pop();
                }

                doNotDrawLimb = true;
            }
            break;

        case BOSSGOMA_LIMB_TAIL4:
        case BOSSGOMA_LIMB_TAIL3:
        case BOSSGOMA_LIMB_TAIL2:
        case BOSSGOMA_LIMB_TAIL1:
            Matrix_JointPosition(pos, rot);

            if (*dList != NULL) {
                Matrix_Push();
                Matrix_Scale(self->tailLimbsScale[limbIndex - BOSSGOMA_LIMB_TAIL4],
                             self->tailLimbsScale[limbIndex - BOSSGOMA_LIMB_TAIL4],
                             self->tailLimbsScale[limbIndex - BOSSGOMA_LIMB_TAIL4], MTXMODE_APPLY);
                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_goma.c", 4836),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_OPA_DISP++, *dList);
                Matrix_Pop();
            }

            doNotDrawLimb = true;
            break;
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_goma.c", 4858);

    return doNotDrawLimb;
}

void BossGoma_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f tailZero = { 0.0f, 0.0f, 0.0f };
    static Vec3f clawBackLocalPos = { 0.0f, 0.0f, 0.0f };
    static Vec3f focusEyeLocalPos = { 0.0f, 300.0f, 2650.0f }; // in the center of the surface of the lens
    static Vec3f zero = { 0.0f, 0.0f, 0.0f };
    Vec3f childPos;
    Vec3s childRot;
    EnGoma* babyGohma;
    BossGoma* self = THIS;
    s32 pad;
    MtxF mtx;

    if (limbIndex == BOSSGOMA_LIMB_TAIL4) { // tail end/last part
        Matrix_MultVec3f(&tailZero, &self->lastTailLimbWorldPos);
    } else if (limbIndex == BOSSGOMA_LIMB_TAIL1) { // tail start/first part
        Matrix_MultVec3f(&tailZero, &self->firstTailLimbWorldPos);
    } else if (limbIndex == BOSSGOMA_LIMB_EYE) {
        Matrix_MultVec3f(&focusEyeLocalPos, &self->actor.focus.pos);
    } else if (limbIndex == BOSSGOMA_LIMB_R_FEET_BACK) {
        Matrix_MultVec3f(&clawBackLocalPos, &self->rightHandBackLimbWorldPos);
    } else if (limbIndex == BOSSGOMA_LIMB_L_FEET_BACK) {
        Matrix_MultVec3f(&clawBackLocalPos, &self->leftHandBackLimbWorldPos);
    }

    if (self->visualState == VISUALSTATE_DEFEATED) {
        if (*dList != NULL) {
            Matrix_MultVec3f(&clawBackLocalPos, &self->defeatedLimbPositions[limbIndex]);
        } else {
            self->defeatedLimbPositions[limbIndex].y = 10000.0f;
        }
    }

    if (self->deadLimbsState[limbIndex] == 1) {
        self->deadLimbsState[limbIndex] = 2;
        Matrix_MultVec3f(&zero, &childPos);
        Matrix_Get(&mtx);
        Matrix_MtxFToYXZRotS(&mtx, &childRot, 0);
        // These are the pieces of Gohma as it falls apart. It appears to use the same actor as the baby gohmas.
        babyGohma = (EnGoma*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_GOMA,
                                                childPos.x, childPos.y, childPos.z, childRot.x, childRot.y, childRot.z,
                                                sDeadLimbLifetime[limbIndex] + 100);
        if (babyGohma != NULL) {
            babyGohma->bossLimbDl = *dList;
            babyGohma->actor.objBankIndex = self->actor.objBankIndex;
        }
    }

    Collider_UpdateSpheres(limbIndex, &self->collider);
}

Gfx* BossGoma_EmptyDlist(GraphicsContext* gfxCtx) {
    Gfx* dListHead;
    Gfx* dList;

    dList = dListHead = Graph_Alloc(gfxCtx, sizeof(Gfx) * 1);

    gSPEndDisplayList(dListHead++);

    return dList;
}

Gfx* BossGoma_NoBackfaceCullingDlist(GraphicsContext* gfxCtx) {
    Gfx* dListHead;
    Gfx* dList;

    dList = dListHead = Graph_Alloc(gfxCtx, sizeof(Gfx) * 4);

    gDPPipeSync(dListHead++);
    gDPSetRenderMode(dListHead++, G_RM_PASS, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(dListHead++, G_CULL_BACK);
    gSPEndDisplayList(dListHead++);

    return dList;
}

void BossGoma_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BossGoma* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_goma.c", 4991);

    func_80093D18(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, -4000.0f, 0.0f, MTXMODE_APPLY);

    if (self->noBackfaceCulling) {
        gSPSegment(POLY_OPA_DISP++, 0x08, BossGoma_NoBackfaceCullingDlist(globalCtx->state.gfxCtx));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x08, BossGoma_EmptyDlist(globalCtx->state.gfxCtx));
    }

    SkelAnime_DrawOpa(globalCtx, self->skelanime.skeleton, self->skelanime.jointTable, BossGoma_OverrideLimbDraw,
                      BossGoma_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_goma.c", 5012);
}

void BossGoma_SpawnChildGohma(BossGoma* self, GlobalContext* globalCtx, s16 i) {
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_GOMA, self->lastTailLimbWorldPos.x,
                       self->lastTailLimbWorldPos.y - 50.0f, self->lastTailLimbWorldPos.z, 0, i * (0x10000 / 3), 0, i);

    self->childrenGohmaState[i] = 1;
}
