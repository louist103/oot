#include "z_demo_effect.h"
#include "vt.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_efc_crystal_light/object_efc_crystal_light.h"
#include "objects/object_efc_fire_ball/object_efc_fire_ball.h"
#include "objects/object_efc_lgt_shower/object_efc_lgt_shower.h"
#include "objects/object_god_lgt/object_god_lgt.h"
#include "objects/object_light_ring/object_light_ring.h"
#include "objects/object_triforce_spot/object_triforce_spot.h"
#include "objects/object_efc_tw/object_efc_tw.h"
#include "objects/object_gi_jewel/object_gi_jewel.h"

#define FLAGS 0x00000030

#define THIS ((DemoEffect*)thisx)

void DemoEffect_Init(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_Update(Actor* thisx, GlobalContext* globalCtx);

void DemoEffect_DrawCrystalLight(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawFireBall(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawBlueOrb(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawLgtShower(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawGodLgt(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawLightRing(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawTriforceSpot(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawGetItem(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawLightEffect(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawTimeWarp(Actor* thisx, GlobalContext* globalCtx);
void DemoEffect_DrawJewel(Actor* thisx, GlobalContext* globalCtx);

void DemoEffect_Wait(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_InitTimeWarp(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_InitTimeWarpTimeblock(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_InitCreationFireball(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_InitJewel(GlobalContext* globalCtx, DemoEffect* self);
void DemoEffect_InitJewelColor(DemoEffect* self);

void DemoEffect_UpdateCrystalLight(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdatePositionToParent(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateBlueOrbGrow(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateBlueOrbShrink(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateLgtShower(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateGodLgtDin(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateGodLgtNayru(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateGodLgtFarore(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateLightRingExpanding(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateTriforceSpot(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateGetItem(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateLightRingShrinking(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateLightRingTriforce(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateLightEffect(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateJewelChild(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateJewelAdult(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateDust(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateCreationFireball(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateTimeWarpReturnFromChamberOfSages(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateTimeWarpPullMasterSword(DemoEffect* self, GlobalContext* globalCtx);
void DemoEffect_UpdateTimeWarpTimeblock(DemoEffect* self, GlobalContext* globalCtx);

s32 DemoEffect_CheckCsAction(DemoEffect* self, GlobalContext* globalCtx, s32 csActionCompareId);
void DemoEffect_InitPositionFromCsAction(DemoEffect* self, GlobalContext* globalCtx, s32 csActionIndex);
void DemoEffect_MoveToCsEndpoint(DemoEffect* self, GlobalContext* globalCtx, s32 csActionId, s32 shouldUpdateFacing);
void DemoEffect_MoveGetItem(DemoEffect* self, GlobalContext* globalCtx, s32 csActionId, f32 speed);

const ActorInit Demo_Effect_InitVars = {
    ACTOR_DEMO_EFFECT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(DemoEffect),
    (ActorFunc)DemoEffect_Init,
    (ActorFunc)DemoEffect_Destroy,
    (ActorFunc)DemoEffect_Update,
    NULL,
};

// This variable assures only one jewel will play SFX
static s16 sSfxJewelId[] = { 0 };

// The object used by the effectType
static s16 sEffectTypeObjects[] = {
    /* 0x00 */ OBJECT_EFC_CRYSTAL_LIGHT,
    /* 0x01 */ OBJECT_EFC_FIRE_BALL,
    /* 0x02 */ OBJECT_GAMEPLAY_KEEP,
    /* 0x03 */ OBJECT_EFC_LGT_SHOWER,
    /* 0x04 */ OBJECT_GOD_LGT,
    /* 0x05 */ OBJECT_GOD_LGT,
    /* 0x06 */ OBJECT_GOD_LGT,
    /* 0x07 */ OBJECT_LIGHT_RING,
    /* 0x08 */ OBJECT_TRIFORCE_SPOT,
    /* 0x09 */ OBJECT_GI_MEDAL,
    /* 0x0A */ OBJECT_GI_MEDAL,
    /* 0x0B */ OBJECT_GI_MEDAL,
    /* 0x0C */ OBJECT_GI_MEDAL,
    /* 0x0D */ OBJECT_GI_MEDAL,
    /* 0x0E */ OBJECT_GI_MEDAL,
    /* 0x0F */ OBJECT_EFC_TW,
    /* 0x10 */ OBJECT_LIGHT_RING,
    /* 0x11 */ OBJECT_LIGHT_RING,
    /* 0x12 */ OBJECT_GAMEPLAY_KEEP,
    /* 0x13 */ OBJECT_GI_JEWEL,
    /* 0x14 */ OBJECT_GI_JEWEL,
    /* 0x15 */ OBJECT_GI_JEWEL,
    /* 0x16 */ OBJECT_GI_JEWEL,
    /* 0x17 */ OBJECT_GI_M_ARROW,
    /* 0x18 */ OBJECT_EFC_TW,
    /* 0x19 */ OBJECT_EFC_TW,
};

static u8 sTimewarpVertexSizeIndices[] = { 1, 1, 2, 0, 1, 1, 2, 0, 1, 2, 0, 2, 1, 0, 1, 0, 2, 0, 2, 2, 0 };

static Color_RGB8 sJewelSparkleColors[5][2] = {
    { { 255, 255, 255 }, { 100, 255, 0 } }, { { 255, 255, 255 }, { 200, 0, 150 } },
    { { 255, 255, 255 }, { 0, 100, 255 } }, { { 0, 0, 0 }, { 0, 0, 0 } },
    { { 223, 0, 0 }, { 0, 0, 0 } },
};

/**
 * Sets up the update function.
 */
void DemoEffect_SetupUpdate(DemoEffect* self, DemoEffectFunc updateFunc) {
    self->updateFunc = updateFunc;
}

/**
 * Gives a number on the range of 0.0f - 1.0f representing current cutscene action completion percentage.
 */
f32 DemoEffect_InterpolateCsFrames(GlobalContext* globalCtx, s32 csActionId) {
    f32 interpolated =
        Environment_LerpWeight(globalCtx->csCtx.npcActions[csActionId]->endFrame,
                               globalCtx->csCtx.npcActions[csActionId]->startFrame, globalCtx->csCtx.frames);
    if (interpolated > 1.0f) {
        interpolated = 1.0f;
    }
    return interpolated;
}

/**
 * Initializes information for Jewel/Spritual Stone actors.
 */
void DemoEffect_InitJewel(GlobalContext* globalCtx, DemoEffect* self) {
    self->initDrawFunc = DemoEffect_DrawJewel;
    if (!LINK_IS_ADULT) {
        self->initUpdateFunc = DemoEffect_UpdateJewelChild;
    } else {
        self->initUpdateFunc = DemoEffect_UpdateJewelAdult;
    }
    if (globalCtx->sceneNum == SCENE_TOKINOMA) {
        Actor_SetScale(&self->actor, 0.35f);
    } else {
        Actor_SetScale(&self->actor, 0.10f);
    }
    self->csActionId = 1;
    self->actor.shape.rot.x = 16384;
    DemoEffect_InitJewelColor(self);
    self->jewel.alpha = 0;
    self->jewelCsRotation.x = self->jewelCsRotation.y = self->jewelCsRotation.z = 0;
    sSfxJewelId[0] = 0;
}

/**
 * Initializes information for Get Item actors.
 * These are the Medal and Light Arrow actors.
 */
void DemoEffect_InitGetItem(DemoEffect* self) {
    self->getItem.isPositionInit = 0;
    self->getItem.isLoaded = 0;
    self->initDrawFunc = DemoEffect_DrawGetItem;
    self->initUpdateFunc = DemoEffect_UpdateGetItem;
    Actor_SetScale(&self->actor, 0.25f);
    self->csActionId = 6;
}

/**
 * Main Actor Init function
 */
void DemoEffect_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    DemoEffect* self = THIS;
    s32 effectType;
    s32 lightEffect;
    s32 objectIndex;
    DemoEffect* crystalLight;
    DemoEffect* lightRing;

    effectType = (self->actor.params & 0x00FF);
    lightEffect = ((self->actor.params & 0xF000) >> 12);

    osSyncPrintf(VT_FGCOL(CYAN) " no = %d\n" VT_RST, effectType);

    objectIndex = sEffectTypeObjects[effectType] == OBJECT_GAMEPLAY_KEEP
                      ? 0
                      : Object_GetIndex(&globalCtx->objectCtx, sEffectTypeObjects[effectType]);

    osSyncPrintf(VT_FGCOL(CYAN) " bank_ID = %d\n" VT_RST, objectIndex);

    if (objectIndex < 0) {
        ASSERT(0, "0", "../z_demo_effect.c", 723);
    } else {
        self->initObjectBankIndex = objectIndex;
    }

    self->effectFlags = 0;
    Actor_SetScale(&self->actor, 0.2f);

    switch (effectType) {
        case DEMO_EFFECT_CRYSTAL_LIGHT:
            self->initDrawFunc = DemoEffect_DrawCrystalLight;
            self->initUpdateFunc = DemoEffect_UpdateCrystalLight;
            break;

        case DEMO_EFFECT_FIRE_BALL:
            self->initDrawFunc = DemoEffect_DrawFireBall;
            self->initUpdateFunc = DemoEffect_UpdatePositionToParent;
            Actor_SetScale(&self->actor, 0.1f);
            break;

        case DEMO_EFFECT_BLUE_ORB:
            self->initDrawFunc = DemoEffect_DrawBlueOrb;
            self->initUpdateFunc = DemoEffect_UpdateBlueOrbGrow;
            self->blueOrb.alpha = 255;
            self->blueOrb.scale = 5;
            self->blueOrb.rotation = 0;
            Actor_SetScale(&self->actor, 0.05f);
            self->primXluColor[0] = 188;
            self->primXluColor[1] = 255;
            self->primXluColor[2] = 255;
            self->envXluColor[1] = 100;
            self->envXluColor[2] = 255;
            self->envXluColor[0] = 0;
            break;

        case DEMO_EFFECT_LIGHT:
            self->initDrawFunc = DemoEffect_DrawLightEffect;
            self->initUpdateFunc = DemoEffect_UpdateLightEffect;
            self->light.alpha = 255;
            self->light.scaleFlag = 0;
            self->light.flicker = 0;
            self->light.rotation = 0;
            switch (lightEffect) {
                case DEMO_EFFECT_LIGHT_RED:
                    self->primXluColor[0] = 255;
                    self->primXluColor[1] = 255;
                    self->primXluColor[2] = 255;
                    self->envXluColor[1] = 50;
                    self->envXluColor[0] = 255;
                    self->envXluColor[2] = 0;
                    break;

                case DEMO_EFFECT_LIGHT_BLUE:
                    self->primXluColor[0] = 255;
                    self->primXluColor[1] = 255;
                    self->primXluColor[2] = 255;
                    self->envXluColor[1] = 150;
                    self->envXluColor[0] = 0;
                    self->envXluColor[2] = 255;
                    break;

                case DEMO_EFFECT_LIGHT_GREEN:
                    self->primXluColor[0] = 255;
                    self->primXluColor[1] = 255;
                    self->primXluColor[2] = 255;
                    self->envXluColor[1] = 200;
                    self->envXluColor[0] = 0;
                    self->envXluColor[2] = 0;
                    break;

                case DEMO_EFFECT_LIGHT_ORANGE:
                    self->primXluColor[0] = 255;
                    self->primXluColor[1] = 255;
                    self->primXluColor[2] = 255;
                    self->envXluColor[1] = 150;
                    self->envXluColor[0] = 255;
                    self->envXluColor[2] = 0;
                    break;

                case DEMO_EFFECT_LIGHT_YELLOW:
                    self->primXluColor[0] = 255;
                    self->primXluColor[1] = 255;
                    self->primXluColor[2] = 255;
                    self->envXluColor[0] = 200;
                    self->envXluColor[1] = 255;
                    self->envXluColor[2] = 0;
                    break;

                case DEMO_EFFECT_LIGHT_PURPLE:
                    self->primXluColor[0] = 255;
                    self->primXluColor[1] = 255;
                    self->primXluColor[2] = 255;
                    // clang-format off
                    self->envXluColor[0] = 200; self->envXluColor[1] = 50; self->envXluColor[2] = 255; // Sameline prevents reordering
                    // clang-format on
                    break;

                case DEMO_EFFECT_LIGHT_GREEN2:
                    self->primXluColor[0] = 255;
                    self->primXluColor[1] = 255;
                    self->primXluColor[2] = 255;
                    self->envXluColor[1] = 200;
                    self->envXluColor[0] = 0;
                    self->envXluColor[2] = 0;
                    break;
            }
            self->csActionId = 7;
            Actor_SetScale(thisx, 0.0f);
            break;

        case DEMO_EFFECT_LGT_SHOWER:
            self->lgtShower.alpha = 255;
            self->initDrawFunc = DemoEffect_DrawLgtShower;
            self->initUpdateFunc = DemoEffect_UpdateLgtShower;
            break;

        case DEMO_EFFECT_GOD_LGT_DIN:
            Actor_SetScale(&self->actor, 0.1f);
            self->initDrawFunc = DemoEffect_DrawGodLgt;
            self->primXluColor[1] = 170;
            self->primXluColor[0] = 255;
            self->primXluColor[2] = 255;
            self->envXluColor[0] = 255;
            self->envXluColor[2] = 255;
            self->envXluColor[1] = 0;
            self->godLgt.type = GOD_LGT_DIN;
            self->godLgt.rotation = 0;
            self->initUpdateFunc = DemoEffect_UpdateGodLgtDin;
            self->csActionId = 0;
            break;

        case DEMO_EFFECT_GOD_LGT_NAYRU:
            if (gSaveContext.entranceIndex == 0x013D) {
                Actor_SetScale(&self->actor, 1.0f);
            } else {
                Actor_SetScale(&self->actor, 0.1f);
            }
            self->initDrawFunc = DemoEffect_DrawGodLgt;
            self->primXluColor[0] = 170;
            self->primXluColor[1] = 255;
            self->primXluColor[2] = 255;
            self->envXluColor[1] = 40;
            self->envXluColor[2] = 255;
            self->envXluColor[0] = 0;
            self->godLgt.type = GOD_LGT_NAYRU;
            self->godLgt.lightRingSpawnDelay = 4;
            self->godLgt.rotation = 0;
            self->godLgt.lightRingSpawnTimer = 0;
            self->initUpdateFunc = DemoEffect_UpdateGodLgtNayru;
            self->csActionId = 1;
            break;

        case DEMO_EFFECT_GOD_LGT_FARORE:
            if (gSaveContext.entranceIndex == 0x00EE) {
                Actor_SetScale(&self->actor, 2.4f);
            } else {
                Actor_SetScale(&self->actor, 0.1f);
            }
            self->initDrawFunc = DemoEffect_DrawGodLgt;
            self->primXluColor[0] = 170;
            self->primXluColor[2] = 170;
            self->primXluColor[1] = 255;
            self->envXluColor[1] = 200;
            self->envXluColor[0] = 0;
            self->envXluColor[2] = 0;
            self->godLgt.type = GOD_LGT_FARORE;
            self->godLgt.rotation = 0;
            self->initUpdateFunc = DemoEffect_UpdateGodLgtFarore;
            self->csActionId = 2;
            break;

        case DEMO_EFFECT_LIGHTRING_EXPANDING:
            self->initDrawFunc = DemoEffect_DrawLightRing;
            self->initUpdateFunc = DemoEffect_UpdateLightRingExpanding;
            self->lightRing.timer = 20;
            self->lightRing.timerIncrement = 4;
            self->lightRing.alpha = 255;
            break;

        case DEMO_EFFECT_LIGHTRING_TRIFORCE:
            self->initDrawFunc = DemoEffect_DrawLightRing;
            self->initUpdateFunc = DemoEffect_UpdateLightRingTriforce;
            self->lightRing.timer = 20;
            self->lightRing.timerIncrement = 4;
            self->lightRing.alpha = 0;
            self->csActionId = 4;
            break;

        case DEMO_EFFECT_LIGHTRING_SHRINKING:
            self->initDrawFunc = DemoEffect_DrawLightRing;
            self->initUpdateFunc = DemoEffect_UpdateLightRingShrinking;
            self->lightRing.timer = 351;
            self->lightRing.timerIncrement = 2;
            self->lightRing.alpha = 0;
            break;

        case DEMO_EFFECT_TRIFORCE_SPOT:
            self->initDrawFunc = DemoEffect_DrawTriforceSpot;
            self->initUpdateFunc = DemoEffect_UpdateTriforceSpot;
            self->triforceSpot.crystalLightOpacity = 0;
            self->triforceSpot.lightColumnOpacity = 0;
            self->triforceSpot.triforceSpotOpacity = 0;
            self->triforceSpot.rotation = 0;
            self->primXluColor[0] = 0;
            self->csActionId = 3;

            Actor_SetScale(&self->actor, 0.020f);

            crystalLight = (DemoEffect*)Actor_SpawnAsChild(
                &globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_EFFECT, self->actor.world.pos.x,
                self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, DEMO_EFFECT_CRYSTAL_LIGHT);

            if (crystalLight != NULL) {
                Actor_SetScale(&crystalLight->actor, 0.6f);
            }

            lightRing = (DemoEffect*)Actor_SpawnAsChild(
                &globalCtx->actorCtx, &crystalLight->actor, globalCtx, ACTOR_DEMO_EFFECT, self->actor.world.pos.x,
                self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, DEMO_EFFECT_LIGHTRING_TRIFORCE);

            if (lightRing != NULL) {
                Actor_SetScale(&lightRing->actor, 0.4f);
            }
            break;

        case DEMO_EFFECT_MEDAL_FIRE:
            DemoEffect_InitGetItem(self);
            self->getItem.drawId = GID_MEDALLION_FIRE;
            break;

        case DEMO_EFFECT_MEDAL_WATER:
            DemoEffect_InitGetItem(self);
            self->getItem.drawId = GID_MEDALLION_WATER;
            break;

        case DEMO_EFFECT_MEDAL_FOREST:
            DemoEffect_InitGetItem(self);
            self->getItem.drawId = GID_MEDALLION_FOREST;
            break;

        case DEMO_EFFECT_MEDAL_SPIRIT:
            DemoEffect_InitGetItem(self);
            self->getItem.drawId = GID_MEDALLION_SPIRIT;
            break;

        case DEMO_EFFECT_MEDAL_SHADOW:
            DemoEffect_InitGetItem(self);
            self->getItem.drawId = GID_MEDALLION_SHADOW;
            break;

        case DEMO_EFFECT_MEDAL_LIGHT:
            DemoEffect_InitGetItem(self);
            self->getItem.drawId = GID_MEDALLION_LIGHT;
            break;

        case DEMO_EFFECT_LIGHTARROW:
            DemoEffect_InitGetItem(self);
            self->getItem.drawId = GID_ARROW_LIGHT;
            break;

        case DEMO_EFFECT_TIMEWARP_TIMEBLOCK_LARGE:
        case DEMO_EFFECT_TIMEWARP_TIMEBLOCK_SMALL:
            self->actor.flags |= 0x2000000;
        case DEMO_EFFECT_TIMEWARP_MASTERSWORD:
            self->initDrawFunc = DemoEffect_DrawTimeWarp;
            self->initUpdateFunc = DemoEffect_InitTimeWarp;
            self->envXluColor[0] = 0;
            self->envXluColor[1] = 100;
            self->envXluColor[2] = 255;
            SkelCurve_Clear(&self->skelCurve);
            self->timeWarp.shrinkTimer = 0;
            break;

        case DEMO_EFFECT_JEWEL_KOKIRI:
            self->jewelDisplayList = gGiKokiriEmeraldGemDL;
            self->jewelHolderDisplayList = gGiKokiriEmeraldSettingDL;
            self->jewel.type = DEMO_EFFECT_JEWEL_KOKIRI;
            self->jewel.isPositionInit = 0;
            DemoEffect_InitJewel(globalCtx, self);
            break;

        case DEMO_EFFECT_JEWEL_GORON:
            self->jewelDisplayList = gGiGoronRubyGemDL;
            self->jewelHolderDisplayList = gGiGoronRubySettingDL;
            self->jewel.type = DEMO_EFFECT_JEWEL_GORON;
            self->jewel.isPositionInit = 0;
            DemoEffect_InitJewel(globalCtx, self);
            break;

        case DEMO_EFFECT_JEWEL_ZORA:
            self->jewelDisplayList = gGiZoraSapphireGemDL;
            self->jewelHolderDisplayList = gGiZoraSapphireSettingDL;
            self->jewel.type = DEMO_EFFECT_JEWEL_ZORA;
            self->jewel.isPositionInit = 0;
            DemoEffect_InitJewel(globalCtx, self);
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTOR_EN_DOOR);
            if ((globalCtx->sceneNum == SCENE_BDAN) && (gSaveContext.infTable[20] & 0x20)) {
                Actor_Kill(&self->actor);
                return;
            }
            break;

        case DEMO_EFFECT_DUST:
            self->initDrawFunc = NULL;
            self->initUpdateFunc = DemoEffect_UpdateDust;
            self->dust.timer = 0;
            self->csActionId = 2;
            break;

        default:
            ASSERT(0, "0", "../z_demo_effect.c", 1062);
            break;
    }

    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    DemoEffect_SetupUpdate(self, DemoEffect_Wait);
}

/**
 * Main Actor Destroy function
 */
void DemoEffect_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    s32 effectType = (self->actor.params & 0x00FF);

    if (effectType == DEMO_EFFECT_TIMEWARP_MASTERSWORD || effectType == DEMO_EFFECT_TIMEWARP_TIMEBLOCK_LARGE ||
        effectType == DEMO_EFFECT_TIMEWARP_TIMEBLOCK_SMALL) {
        SkelCurve_Destroy(globalCtx, &self->skelCurve);
    }
}

/**
 * This update function waits until the associate object is loaded.
 * Once the object is loaded, it will copy over the initUpdateFunc/initDrawFunc funcs to be active.
 * They are copied to actor.draw and updateFunc.
 * initUpdateFunc/initDrawFunc are set during initialization and are NOT executed.
 */
void DemoEffect_Wait(DemoEffect* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->initObjectBankIndex)) {
        self->actor.objBankIndex = self->initObjectBankIndex;
        self->actor.draw = self->initDrawFunc;
        self->updateFunc = self->initUpdateFunc;

        osSyncPrintf(VT_FGCOL(CYAN) " 転送終了 move_wait " VT_RST);
    }
}

/**
 * Copies the current Actor's position to the parent Actor's position.
 */
void DemoEffect_UpdatePositionToParent(DemoEffect* self, GlobalContext* globalCtx) {
    if (self->actor.parent != NULL) {
        // Struct copy affects regalloc
        self->actor.world.pos.x = self->actor.parent->world.pos.x;
        self->actor.world.pos.y = self->actor.parent->world.pos.y;
        self->actor.world.pos.z = self->actor.parent->world.pos.z;
    }
}

/**
 * Update function for the Crystal Light actor.
 * The Crystal Light actor is the three beams of light under the Triforce that converge on it.
 * The Crystal Light's position is set to the parent Actor (Triforce) each frame.
 * If the Crystal Light has no parent Actor, then it will raise into the sky.
 */
void DemoEffect_UpdateCrystalLight(DemoEffect* self, GlobalContext* globalCtx) {
    DemoEffect_UpdatePositionToParent(self, globalCtx);
    self->actor.world.pos.y += 14.0f;
}

/**
 * Spawns sparkle effects for Medals
 */
void DemoEffect_MedalSparkle(DemoEffect* self, GlobalContext* globalCtx, s32 isSmallSpawner) {
    Vec3f velocity;
    Vec3f accel;
    Vec3f pos;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;

    if (isSmallSpawner != 1 || (globalCtx->gameplayFrames & 1) == 0) {
        primColor.r = 255;
        primColor.g = 255;
        primColor.b = 255;
        envColor.r = 255;
        envColor.g = 255;
        envColor.b = 100;
        primColor.a = 0;

        velocity.y = 0.0f;

        accel.x = 0.0f;
        accel.y = -0.1f;
        accel.z = 0.0f;

        if (isSmallSpawner) {
            velocity.x = Rand_ZeroOne() - 0.5f;
            velocity.z = Rand_ZeroOne() - 0.5f;
        } else {
            velocity.x = (Rand_ZeroOne() - 0.5f) * 2.0f;
            velocity.z = (Rand_ZeroOne() - 0.5f) * 2.0f;
        }

        pos.x = Rand_CenteredFloat(10.0f) + self->actor.world.pos.x;
        pos.y = Rand_CenteredFloat(10.0f) + self->actor.world.pos.y;
        pos.z = Rand_CenteredFloat(10.0f) + self->actor.world.pos.z;

        EffectSsKiraKira_SpawnDispersed(globalCtx, &pos, &velocity, &accel, &primColor, &envColor, 1000, 16);
    }
}

/**
 * Update function for the GetItem Actors.
 * Medals and Light Arrows.
 * It spawns Medal Sparkle Effects  and scales/moves the Actor based on the current Cutscene Action
 */
void DemoEffect_UpdateGetItem(DemoEffect* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->actor;

    if (globalCtx->csCtx.state != CS_STATE_IDLE && globalCtx->csCtx.npcActions[self->csActionId] != NULL) {
        if (self->getItem.isPositionInit) {
            DemoEffect_MoveGetItem(self, globalCtx, self->csActionId, 0.1f);
        } else {
            DemoEffect_InitPositionFromCsAction(self, globalCtx, self->csActionId);
            self->getItem.isPositionInit = 1;
        }

        if (self->getItem.drawId != GID_ARROW_LIGHT) {
            self->actor.shape.rot.x = 0xE0C0;
        } else {
            self->actor.shape.rot.y += 0x0400;
        }

        Actor_SetScale(thisx, 0.20f);

        if (gSaveContext.entranceIndex == 0x0053) {
            switch (globalCtx->csCtx.npcActions[self->csActionId]->action) {
                case 2:
                    DemoEffect_MedalSparkle(self, globalCtx, 0);
                    break;
                case 3:
                    DemoEffect_MedalSparkle(self, globalCtx, 1);
                    break;
            }
        }
        switch (globalCtx->csCtx.npcActions[self->csActionId]->action) {
            case 2:
                if (gSaveContext.entranceIndex == 0x0053) {
                    Audio_PlayActorSound2(thisx, NA_SE_EV_MEDAL_APPEAR_L - SFX_FLAG);
                } else {
                    func_800788CC(NA_SE_EV_MEDAL_APPEAR_S - SFX_FLAG);
                }
                if (self->getItem.drawId != GID_ARROW_LIGHT) {
                    self->actor.shape.rot.y += 0x3E80;
                }
                self->getItem.rotation = 0x3E80;
                break;
            case 3:
                self->getItem.rotation -= (s16)((self->getItem.rotation - 0x03E8) * 0.10f);
                if (self->getItem.drawId != GID_ARROW_LIGHT) {
                    self->actor.shape.rot.y += self->getItem.rotation;
                }
                if (gSaveContext.entranceIndex == 0x0053) {
                    Audio_PlayActorSound2(thisx, NA_SE_EV_MEDAL_APPEAR_L - SFX_FLAG);
                } else {
                    func_800788CC(NA_SE_EV_MEDAL_APPEAR_S - SFX_FLAG);
                }
                break;
            case 4:
                Audio_PlayActorSound2(thisx, NA_SE_EV_MEDAL_APPEAR_S - SFX_FLAG);
                break;
        }
    }
}

/**
 * Initializes Timewarp Actors.
 * This is an Update Function that is only ran for one frame.
 * Timewarp actors are spawned when Link...
 * 1) Pulls the Master Sword
 * 2) Returns from the Chamber of Sages for the first time
 * 3) Timeblock is cleared with the Song of Time (Large and Small have different versions of Timewarp)
 */
void DemoEffect_InitTimeWarp(DemoEffect* self, GlobalContext* globalCtx) {
    s32 effectType = (self->actor.params & 0x00FF);

    if (!SkelCurve_Init(globalCtx, &self->skelCurve, &gTimeWarpSkel, &gTimeWarpAnim)) {
        ASSERT(0, "0", "../z_demo_effect.c", 1283);
    }

    if (effectType == DEMO_EFFECT_TIMEWARP_TIMEBLOCK_LARGE || effectType == DEMO_EFFECT_TIMEWARP_TIMEBLOCK_SMALL) {
        SkelCurve_SetAnim(&self->skelCurve, &gTimeWarpAnim, 1.0f, 59.0f, 1.0f, 1.7f);
        SkelCurve_Update(globalCtx, &self->skelCurve);
        self->updateFunc = DemoEffect_InitTimeWarpTimeblock;

        if (effectType == DEMO_EFFECT_TIMEWARP_TIMEBLOCK_LARGE) {
            Actor_SetScale(&self->actor, 0.14f);
        } else {
            Actor_SetScale(&self->actor, 84 * 0.001f);
        }
    } else if (gSaveContext.sceneSetupIndex == 5 || gSaveContext.sceneSetupIndex == 4 ||
               (gSaveContext.entranceIndex == 0x0324 && !((gSaveContext.eventChkInf[12] & 0x200)))) {
        SkelCurve_SetAnim(&self->skelCurve, &gTimeWarpAnim, 1.0f, 59.0f, 59.0f, 0.0f);
        SkelCurve_Update(globalCtx, &self->skelCurve);
        self->updateFunc = DemoEffect_UpdateTimeWarpReturnFromChamberOfSages;
        osSyncPrintf(VT_FGCOL(CYAN) " 縮むバージョン \n" VT_RST);
    } else {
        SkelCurve_SetAnim(&self->skelCurve, &gTimeWarpAnim, 1.0f, 59.0f, 1.0f, 1.0f);
        SkelCurve_Update(globalCtx, &self->skelCurve);
        self->updateFunc = DemoEffect_UpdateTimeWarpPullMasterSword;
        osSyncPrintf(VT_FGCOL(CYAN) " 通常 バージョン \n" VT_RST);
    }
}

/**
 * Update function for the Timewarp Actor that is used when Link pulls the Mastersword
 * It changes the Background Music and updates its SkelCurve animation.
 */
void DemoEffect_UpdateTimeWarpPullMasterSword(DemoEffect* self, GlobalContext* globalCtx) {
    if (Flags_GetEnv(globalCtx, 1)) {
        if (!(self->effectFlags & 0x2)) {
            func_800F3F3C(0);
            self->effectFlags |= 0x2;
        }

        if (SkelCurve_Update(globalCtx, &self->skelCurve)) {
            SkelCurve_SetAnim(&self->skelCurve, &gTimeWarpAnim, 1.0f, 60.0f, 59.0f, 0.0f);
        }
    }
}

/**
 * Shrinks the Timewarp object vertices.
 * Used by the Chamber of Sages return timewarp and Timeblock clear timewarp.
 */
void DemoEffect_TimewarpShrink(f32 size) {
    Vtx* vertices;
    s32 i;
    u8 sizes[3];

    // This function uses the data in obj_efc_tw offset 0x0060 to 0x01B0
    vertices = SEGMENTED_TO_VIRTUAL(gTimeWarpVtx);

    sizes[0] = 0;
    sizes[1] = (s32)(202.0f * size);
    sizes[2] = (s32)(255.0f * size);

    for (i = 0; i < 21; i++) {
        if (sTimewarpVertexSizeIndices[i] != 0) {
            vertices[i].v.cn[3] = sizes[sTimewarpVertexSizeIndices[i]];
        }
    }
}

/**
 * Update function for the Timewarp Actor that is used when Link returns from the Chamber of Sages for the first time.
 * It shrinks the timewarp vertices and scales the Actor.
 */
void DemoEffect_UpdateTimeWarpReturnFromChamberOfSages(DemoEffect* self, GlobalContext* globalCtx) {
    f32 shrinkProgress;

    self->timeWarp.shrinkTimer++;

    if (self->timeWarp.shrinkTimer > 250) {
        if (gSaveContext.entranceIndex == 0x0324) {
            gSaveContext.eventChkInf[12] |= 0x200;
        }

        Actor_Kill(&self->actor);
        return;
    }

    if (self->timeWarp.shrinkTimer > 100) {
        shrinkProgress = (250 - self->timeWarp.shrinkTimer) * (1.0f / 750.0f);
        self->actor.scale.x = shrinkProgress;
        self->actor.scale.z = shrinkProgress;
        DemoEffect_TimewarpShrink(shrinkProgress * 5.0f);
    }

    func_8002F948(&self->actor, NA_SE_EV_TIMETRIP_LIGHT - SFX_FLAG);
}

/**
 * Update function for the Timewarp Actor that is used when a Timeblock is cleared.
 * It shrinks the timewarp vertices and scales the Actor.
 */
void DemoEffect_UpdateTimeWarpTimeblock(DemoEffect* self, GlobalContext* globalCtx) {
    f32 shrinkProgress;
    f32 scale;

    self->timeWarp.shrinkTimer++;

    if (self->timeWarp.shrinkTimer <= 100) {
        shrinkProgress = (100 - self->timeWarp.shrinkTimer) * 0.010f;
        scale = shrinkProgress * 0.14f;

        if ((self->actor.params & 0x00FF) == DEMO_EFFECT_TIMEWARP_TIMEBLOCK_SMALL) {
            scale *= 0.6f;
        }

        self->actor.scale.x = scale;
        self->actor.scale.z = scale;
        DemoEffect_TimewarpShrink(shrinkProgress);
        func_8002F948(&self->actor, NA_SE_EV_TIMETRIP_LIGHT - SFX_FLAG);
        return;
    }

    DemoEffect_TimewarpShrink(1.0f);
    Actor_Kill(&self->actor);
}

/**
 * Initializes information for the Timewarp Actor used for the Timeblock clear effect.
 * This is an Update Func that is only ran for one frame.
 */
void DemoEffect_InitTimeWarpTimeblock(DemoEffect* self, GlobalContext* globalCtx) {
    func_8002F948(&self->actor, NA_SE_EV_TIMETRIP_LIGHT - SFX_FLAG);

    if (SkelCurve_Update(globalCtx, &self->skelCurve)) {
        SkelCurve_SetAnim(&self->skelCurve, &gTimeWarpAnim, 1.0f, 60.0f, 59.0f, 0.0f);
        self->updateFunc = DemoEffect_UpdateTimeWarpTimeblock;
        self->timeWarp.shrinkTimer = 0;
    }
}

/**
 * Update function for the Triforce Actor.
 * It rotates and updates the alpha of the Triforce and child actors.
 */
void DemoEffect_UpdateTriforceSpot(DemoEffect* self, GlobalContext* globalCtx) {
    self->triforceSpot.rotation += 0x03E8;

    if (globalCtx->csCtx.state != CS_STATE_IDLE && globalCtx->csCtx.npcActions[self->csActionId] != NULL) {
        DemoEffect_MoveToCsEndpoint(self, globalCtx, self->csActionId, 0);

        if (globalCtx->csCtx.npcActions[self->csActionId]->action == 2) {
            if (self->primXluColor[0] < 140) {
                self->primXluColor[0]++;
            }

            if (self->primXluColor[0] < 30) {
                self->triforceSpot.triforceSpotOpacity = ((s32)self->primXluColor[0]) * 8.5f;
            } else {
                self->triforceSpot.triforceSpotOpacity = 255;

                if (self->primXluColor[0] < 60) {
                    self->triforceSpot.lightColumnOpacity = (((s32)self->primXluColor[0]) - 30) * 8.5f;
                } else {
                    if (self->primXluColor[0] <= 140) {
                        self->triforceSpot.lightColumnOpacity = 255;
                        self->triforceSpot.crystalLightOpacity = (((s32)self->primXluColor[0]) - 60) * 3.1875f;
                    }
                }
            }
        }

        if (gSaveContext.entranceIndex == 0x00A0 && gSaveContext.sceneSetupIndex == 6 &&
            globalCtx->csCtx.frames == 143) {
            Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_RING_EXPLOSION);
        }
    }
}

/**
 * Update function for the LightRing actor that shrinks.
 * This is used in the creation cutscene when Din leaves a fireball that explodes into Death Mountain.
 */
void DemoEffect_UpdateLightRingShrinking(DemoEffect* self, GlobalContext* globalCtx) {
    if (self->lightRing.timer < self->lightRing.timerIncrement) {
        Actor_Kill(&self->actor);
        self->lightRing.timer = 0;
    } else {
        self->lightRing.timer -= self->lightRing.timerIncrement;
    }

    if (self->lightRing.timer <= 255) {
        if (self->lightRing.timer >= 225) {
            self->lightRing.alpha = (-self->lightRing.timer * 8) + 2048;
        } else {
            self->lightRing.alpha = 255;
        }
    }

    if (self->lightRing.timer == 255) {
        func_800F3F3C(5);
    }
}

/**
 * Update function for the Lightring Actor that expands.
 * These are spawned by Nayru.
 * These are also used by Din in the creation cutscene when she leaves a fireball that explodes into Death Mountain.
 */
void DemoEffect_UpdateLightRingExpanding(DemoEffect* self, GlobalContext* globalCtx) {
    DemoEffect_UpdatePositionToParent(self, globalCtx);
    self->lightRing.timer += self->lightRing.timerIncrement;

    if (self->lightRing.timer >= 225) {
        self->lightRing.alpha = (-self->lightRing.timer * 8) + 2048;
    }
    if (self->lightRing.timer > 255) {
        self->lightRing.timer = 255;
        Actor_Kill(&self->actor);
        self->lightRing.timer = 0;
    }
}

/**
 * Update function for the Lightring Actor that expands. This is a special version for the Triforce Actor.
 * This version spawns a blue orb when the cutscene action state is set to 2.
 * Once the Blue Orb Actor is spawned the Update Function is changed to the regular Light Ring Expanding Update Func.
 */
void DemoEffect_UpdateLightRingTriforce(DemoEffect* self, GlobalContext* globalCtx) {
    DemoEffect* blueOrb;

    DemoEffect_UpdatePositionToParent(self, globalCtx);

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if (globalCtx->csCtx.npcActions[self->csActionId] != NULL &&
            globalCtx->csCtx.npcActions[self->csActionId]->action == 2) {
            blueOrb = (DemoEffect*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT,
                                               self->actor.world.pos.x, self->actor.world.pos.y,
                                               self->actor.world.pos.z, 0, 0, 0, DEMO_EFFECT_BLUE_ORB);

            if (blueOrb != NULL) {
                Actor_SetScale(&blueOrb->actor, 0.0f);
            }

            self->updateFunc = DemoEffect_UpdateLightRingExpanding;
            self->lightRing.alpha = 255;
        }
    }
}

/**
 * Update function for the FireBall Actor.
 * This is a special version that is used in the creation cutscene.
 * It moves based on gravity and decrements a timer until zero. Once the timer is zero it will spawn other Actors:
 * A Blue Orb Actor, and a Light Ring Expanding Actor, and a Light Ring Shrinking Actor.
 */
void DemoEffect_UpdateCreationFireball(DemoEffect* self, GlobalContext* globalCtx) {
    DemoEffect* effect;

    Actor_MoveForward(&self->actor);
    self->actor.speedXZ = self->actor.speedXZ + (self->actor.gravity * 0.5f);

    if (self->fireBall.timer != 0) {
        self->fireBall.timer--;
        return;
    }

    effect = (DemoEffect*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, self->actor.world.pos.x,
                                      self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, DEMO_EFFECT_BLUE_ORB);
    if (effect != NULL) {
        Actor_SetScale(&effect->actor, 0.0f);
    }

    effect = (DemoEffect*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, self->actor.world.pos.x,
                                      self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0,
                                      DEMO_EFFECT_LIGHTRING_EXPANDING);
    if (effect != NULL) {
        Actor_SetScale(&effect->actor, 0.1f);
    }

    effect = (DemoEffect*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, self->actor.world.pos.x,
                                      self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0,
                                      DEMO_EFFECT_LIGHTRING_SHRINKING);
    if (effect != NULL) {
        Actor_SetScale(&effect->actor, 0.2f);
    }

    func_800788CC(NA_SE_IT_DM_RING_EXPLOSION);
    Actor_Kill(&self->actor);
}

/**
 * Initialization function for the FireBall Actor.
 * This is a special version that is used in the creation cutscene.
 * It is an Update Function only executed for one frame. The Update Function is then changed to UpdateCreationFireball.
 */
void DemoEffect_InitCreationFireball(DemoEffect* self, GlobalContext* globalCtx) {
    Actor* parent = self->actor.parent;

    self->actor.world.rot.y = parent->shape.rot.y;
    self->fireBall.timer = 50;
    self->actor.speedXZ = 1.5f;
    self->actor.minVelocityY = -1.5f;
    self->actor.gravity = -0.03f;
    self->updateFunc = DemoEffect_UpdateCreationFireball;
}

/**
 * Update action for the Blue Orb Actor.
 * This Update Function is run while the Blue Orb is Shrinking.
 * The Blue Orb Actor is the blue light sparkle that is in Din's creation cutscene.
 * It's spawned in the middle of the expanding Light Ring.
 * The Blue Orb Actor shrinks after it grows to max size.
 */
void DemoEffect_UpdateBlueOrbShrink(DemoEffect* self, GlobalContext* globalCtx) {
    self->blueOrb.alpha = self->blueOrb.scale * 16;
    self->blueOrb.scale--;
    Actor_SetScale(&self->actor, self->actor.scale.x * 0.9f);
    if (self->blueOrb.scale == 0) {
        Actor_Kill(&self->actor);
    }
}

/**
 * Update action for the Blue Orb Actor.
 * This Update Function is run while the Blue Orb is Growing.
 * The Blue Orb Actor is the blue light sparkle that is in Din's creation cutscene.
 * It's spawned in the middle of the expanding Light Ring.
 * When the scale timer value reaches 0 the Blue Orb's Update Function changes to UpdateBlueOrbShrink.
 */
void DemoEffect_UpdateBlueOrbGrow(DemoEffect* self, GlobalContext* globalCtx) {
    if (self->actor.parent != NULL) {
        // s32 cast necessary to match codegen. Without the explicit cast to u32 the compiler generates complex cast of
        // u8 to float
        Actor_SetScale(&self->actor,
                       (((5.0f - (s32)self->blueOrb.scale) * 0.01f) * 10.0f) * self->actor.parent->scale.x);
    } else {
        Actor_SetScale(&self->actor, (5.0f - (s32)self->blueOrb.scale) * 0.01f);
    }

    if (self->blueOrb.scale != 0) {
        self->blueOrb.scale--;
    } else {
        self->blueOrb.scale = 15;
        self->updateFunc = DemoEffect_UpdateBlueOrbShrink;
    }
}

/**
 * Update action for the Light Effect Actor.
 * The Light Effect has various use cases.
 * This function updates the position and scale of the actor based on the current cutscene command.
 */
void DemoEffect_UpdateLightEffect(DemoEffect* self, GlobalContext* globalCtx) {
    u16 action;
    s32 isLargeSize;

    isLargeSize = ((self->actor.params & 0x0F00) >> 8);

    if (globalCtx->csCtx.state != CS_STATE_IDLE && globalCtx->csCtx.npcActions[self->csActionId] != NULL) {
        DemoEffect_MoveToCsEndpoint(self, globalCtx, self->csActionId, 0);
        switch (globalCtx->csCtx.npcActions[self->csActionId]->action) {
            case 2:
                if (self->light.rotation < 240) {
                    if (!isLargeSize) {
                        if (self->actor.scale.x < 0.23f) {
                            self->actor.scale.x += 0.001f;
                            Actor_SetScale(&self->actor, self->actor.scale.x);
                        }
                    } else {
                        if (self->actor.scale.x < 2.03f) {
                            self->actor.scale.x += 0.05f;
                            Actor_SetScale(&self->actor, self->actor.scale.x);
                        }
                    }
                }
                self->light.rotation += 6;
                self->light.scaleFlag += 1;
                break;

            case 3:
                Math_SmoothStepToF(&self->actor.scale.x, 0.0f, 0.1f, 0.1f, 0.005f);
                Actor_SetScale(&self->actor, self->actor.scale.x);
                break;

            default:
                break;
        }

        if (globalCtx->sceneNum == SCENE_SPOT04 && gSaveContext.sceneSetupIndex == 6 &&
            globalCtx->csCtx.frames == 197) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_WHITE_OUT);
        }

        if (globalCtx->sceneNum == SCENE_SPOT16 && gSaveContext.sceneSetupIndex == 5) {
            if (!DemoEffect_CheckCsAction(self, globalCtx, 1)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_LIGHT_GATHER - SFX_FLAG);
            }
            if (globalCtx->csCtx.frames == 640) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_WHITE_OUT);
            }

            if (0) {}
        }

        if (globalCtx->sceneNum == SCENE_SPOT08 && gSaveContext.sceneSetupIndex == 4) {
            if (!DemoEffect_CheckCsAction(self, globalCtx, 1)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_LIGHT_GATHER - SFX_FLAG);
            }
            if (globalCtx->csCtx.frames == 648) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_WHITE_OUT);
            }

            // Necessary to match
            if (0) {}
        }

        if (globalCtx->sceneNum == SCENE_TOKINOMA && gSaveContext.sceneSetupIndex == 14) {
            if (1) {}

            if (globalCtx->csCtx.npcActions[self->csActionId]->action == 2) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_LIGHT_GATHER - SFX_FLAG);
            }
        }

        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI || globalCtx->sceneNum == SCENE_YOUSEI_IZUMI_YOKO) {
            if (globalCtx->csCtx.npcActions[self->csActionId]->action == 2) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_LIGHT_GATHER - SFX_FLAG);
            }
        }
    }
}

/**
 * Update action for the Lgt Shower Actor.
 * The Lgt Shower Actor is the green light effect spawned by Farore in the Kokiri Forst creation cutscene.
 * This function updates the scale and alpha of the Actor.
 */
void DemoEffect_UpdateLgtShower(DemoEffect* self, GlobalContext* globalCtx) {
    if (self->lgtShower.alpha > 3) {
        self->lgtShower.alpha -= 3;
        self->actor.scale.x *= 1.05f;
        self->actor.scale.y *= 1.05f;
        self->actor.scale.z *= 1.05f;
    } else {
        Actor_Kill(&self->actor);
    }
}

/**
 * Update action for the God Lgt Din Actor.
 * This is the Goddess Din.
 * This function moves God Lgt Din based on the current cutscene command.
 * This function also spawns a Fireball Actor and sets it's update function to the special InitCreationFireball.
 * The spawned Fireball Actor is also scaled to be smaller than regular by self function.
 */
void DemoEffect_UpdateGodLgtDin(DemoEffect* self, GlobalContext* globalCtx) {
    DemoEffect* fireBall;

    if (globalCtx->csCtx.state != CS_STATE_IDLE && globalCtx->csCtx.npcActions[self->csActionId] != NULL) {
        DemoEffect_MoveToCsEndpoint(self, globalCtx, self->csActionId, 1);

        if (globalCtx->csCtx.npcActions[self->csActionId]->action == 3) {
            fireBall = (DemoEffect*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_EFFECT,
                                                       self->actor.world.pos.x, self->actor.world.pos.y,
                                                       self->actor.world.pos.z, 0, 0, 0, DEMO_EFFECT_FIRE_BALL);

            if (fireBall != NULL) {
                fireBall->initUpdateFunc = DemoEffect_InitCreationFireball;
                Actor_SetScale(&fireBall->actor, 0.020f);
            }
        }

        if (gSaveContext.entranceIndex == 0x00A0) {
            switch (gSaveContext.sceneSetupIndex) {
                case 4:
                    if (globalCtx->csCtx.frames == 288) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_PASS);
                    }
                    if (globalCtx->csCtx.frames == 635) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_PASS);
                    }
                    break;

                case 6:
                    if (globalCtx->csCtx.frames == 55) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_DASH);
                    }
                    break;

                case 11:
                    if (globalCtx->csCtx.frames == 350) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_DASH);
                    }
                    break;
            }
        }
    }
}

/**
 * Update action for the God Lgt Nayru Actor.
 * This is the Goddess Nayru.
 * This function moves God Lgt Nayure based on the current cutscene command.
 * This function also spawns expanding light rings around Nayru in the creation cutscene
 */
void DemoEffect_UpdateGodLgtNayru(DemoEffect* self, GlobalContext* globalCtx) {
    DemoEffect* lightRing;

    if (globalCtx->csCtx.state != CS_STATE_IDLE && globalCtx->csCtx.npcActions[self->csActionId] != NULL) {
        DemoEffect_MoveToCsEndpoint(self, globalCtx, self->csActionId, 1);

        if (globalCtx->csCtx.npcActions[self->csActionId]->action == 3) {
            if (self->godLgt.lightRingSpawnTimer != 0) {
                self->godLgt.lightRingSpawnTimer--;
            } else {
                self->godLgt.lightRingSpawnTimer = self->godLgt.lightRingSpawnDelay;
                lightRing = (DemoEffect*)Actor_Spawn(
                    &globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, self->actor.world.pos.x,
                    self->actor.world.pos.y, self->actor.world.pos.z, self->actor.world.rot.x + 0x4000,
                    self->actor.world.rot.y, self->actor.world.rot.z, DEMO_EFFECT_LIGHTRING_EXPANDING);

                if (lightRing != NULL) {
                    Actor_SetScale(&lightRing->actor, 1.0f);
                }
            }
        }

        if (gSaveContext.entranceIndex == 0x00A0) {
            switch (gSaveContext.sceneSetupIndex) {
                case 4:
                    if (globalCtx->csCtx.frames == 298) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_PASS);
                    }
                    break;

                case 6:
                    if (globalCtx->csCtx.frames == 105) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_DASH);
                    }
                    break;

                case 11:
                    if (globalCtx->csCtx.frames == 360) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_DASH);
                    }
                    break;
            }
        }

        if (gSaveContext.entranceIndex == 0x013D && gSaveContext.sceneSetupIndex == 4) {
            if (globalCtx->csCtx.frames == 72) {
                Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_DASH);
            }
            if (globalCtx->csCtx.frames == 80) {
                func_800F3F3C(4);
            }
        }
    }
}

/**
 * Update action for the God Lgt Farore Actor.
 * This is the Goddess Farore.
 * This function moves God Lgt Farore based on the current cutscene command.
 * This function also spawns an Lgt Shower Actor during the Kokiri creation cutscene.
 */
void DemoEffect_UpdateGodLgtFarore(DemoEffect* self, GlobalContext* globalCtx) {
    DemoEffect* lgtShower;

    if (globalCtx->csCtx.state != CS_STATE_IDLE && globalCtx->csCtx.npcActions[self->csActionId] != NULL) {
        DemoEffect_MoveToCsEndpoint(self, globalCtx, self->csActionId, 1);

        if (globalCtx->csCtx.npcActions[self->csActionId]->action == 3) {
            lgtShower = (DemoEffect*)Actor_SpawnAsChild(
                &globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_EFFECT, self->actor.world.pos.x,
                self->actor.world.pos.y - 150.0f, self->actor.world.pos.z, 0, 0, 0, DEMO_EFFECT_LGT_SHOWER);

            if (lgtShower != NULL) {
                lgtShower->actor.scale.x = 0.23f;
                lgtShower->actor.scale.y = 0.15f;
                lgtShower->actor.scale.z = 0.23f;
            }

            Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_DASH);
            func_800F3F3C(3);
        }

        if (gSaveContext.entranceIndex == 0x00A0) {
            switch (gSaveContext.sceneSetupIndex) {
                case 4:
                    if (globalCtx->csCtx.frames == 315) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_PASS);
                    }
                    break;

                case 6:
                    if (globalCtx->csCtx.frames == 80) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_DASH);
                    }
                    break;

                case 11:
                    if (globalCtx->csCtx.frames == 370) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_IT_DM_FLYING_GOD_DASH);
                    }
                    break;
            }
        }
    }
}

/**
 * Moves self actor towards the target position with a given speed.
 */
void DemoEffect_MoveTowardTarget(Vec3f targetPos, DemoEffect* self, f32 speed) {
    self->actor.world.pos.x += (targetPos.x - self->actor.world.pos.x) * speed;
    self->actor.world.pos.y += (targetPos.y - self->actor.world.pos.y) * speed;
    self->actor.world.pos.z += (targetPos.z - self->actor.world.pos.z) * speed;
}

/**
 * Initializes Jewel colors.
 */
void DemoEffect_InitJewelColor(DemoEffect* self) {
    u8 jewelType = self->jewel.type;

    switch (jewelType) {
        case DEMO_EFFECT_JEWEL_KOKIRI:
            self->primXluColor[2] = 160;
            self->primXluColor[0] = 255;
            self->primXluColor[1] = 255;
            self->envXluColor[0] = 0;
            self->envXluColor[1] = 255;
            self->envXluColor[2] = 0;
            self->primOpaColor[2] = 170;
            self->primOpaColor[0] = 255;
            self->primOpaColor[1] = 255;
            self->envOpaColor[1] = 120;
            self->envOpaColor[0] = 150;
            self->envOpaColor[2] = 0;
            break;

        case DEMO_EFFECT_JEWEL_GORON:
            self->primXluColor[1] = 170;
            self->primXluColor[0] = 255;
            self->primXluColor[2] = 255;
            self->envXluColor[2] = 100;
            self->envXluColor[0] = 255;
            self->envXluColor[1] = 0;
            self->primOpaColor[2] = 170;
            self->primOpaColor[0] = 255;
            self->primOpaColor[1] = 255;
            self->envOpaColor[1] = 120;
            self->envOpaColor[0] = 150;
            self->envOpaColor[2] = 0;
            break;

        case DEMO_EFFECT_JEWEL_ZORA:
            self->primXluColor[0] = 50;
            self->primXluColor[1] = 255;
            self->primXluColor[2] = 255;
            self->envXluColor[2] = 150;
            self->envXluColor[0] = 50;
            self->envXluColor[1] = 0;
            self->primOpaColor[2] = 170;
            self->primOpaColor[0] = 255;
            self->primOpaColor[1] = 255;
            self->envOpaColor[1] = 120;
            self->envOpaColor[0] = 150;
            self->envOpaColor[2] = 0;
            break;
    }
}

/**
 * Sets the Jewel color based on the alpha variable.
 * This function if a value of less than 1.0f is supplied will drain the color from the Jewels.
 * This effect can be seen in prerelease screenshots.
 */
void DemoEffect_SetJewelColor(DemoEffect* self, f32 alpha) {
    DemoEffect_InitJewelColor(self);

    self->primXluColor[0] = (((s32)self->primXluColor[0]) * alpha) + (255.0f * (1.0f - alpha));
    self->primXluColor[1] = (((s32)self->primXluColor[1]) * alpha) + (255.0f * (1.0f - alpha));
    self->primXluColor[2] = (((s32)self->primXluColor[2]) * alpha) + (255.0f * (1.0f - alpha));
    self->primOpaColor[0] = (((s32)self->primOpaColor[0]) * alpha) + (255.0f * (1.0f - alpha));
    self->primOpaColor[1] = (((s32)self->primOpaColor[1]) * alpha) + (255.0f * (1.0f - alpha));
    self->primOpaColor[2] = (((s32)self->primOpaColor[2]) * alpha) + (255.0f * (1.0f - alpha));
    self->envXluColor[0] = ((s32)self->envXluColor[0]) * alpha;
    self->envXluColor[1] = ((s32)self->envXluColor[1]) * alpha;
    self->envXluColor[2] = ((s32)self->envXluColor[2]) * alpha;
    self->envOpaColor[0] = ((s32)self->envOpaColor[0]) * alpha;
    self->envOpaColor[1] = ((s32)self->envOpaColor[1]) * alpha;
    self->envOpaColor[2] = ((s32)self->envOpaColor[2]) * alpha;
}

/**
 * Moves the Jewel Actor during the activation of the Door of Time cutscene.
 * This is used once the Jewel Actor is done orbiting Link and split up to move into the pedastal slots.
 */
void DemoEffect_MoveJewelSplit(PosRot* world, DemoEffect* self) {
    switch (self->jewel.type) {
        case DEMO_EFFECT_JEWEL_KOKIRI:
            world->pos.x -= 40.0f;
            break;
        case DEMO_EFFECT_JEWEL_GORON:
            break;
        case DEMO_EFFECT_JEWEL_ZORA:
            world->pos.x += 40.0f;
            break;
    }
}

/**
 * Moves the Jewel Actor spherically from startPos to endPos.
 * This is used by the Jewel Actor during the Door of Time activation cutscene.
 * This is run when the Jewels merge from Link and begin orbiting him.
 */
void DemoEffect_MoveJewelSpherical(f32 degrees, f32 frameDivisor, Vec3f startPos, Vec3f endPos, f32 radius,
                                   Vec3s rotation, DemoEffect* self) {
    s32 pad;
    s32 pad2;
    f32 distance;
    f32 xPos;
    f32 ySpherical;
    f32 xzSpherical;

    distance = frameDivisor * sqrtf(SQ(endPos.x - startPos.x) + SQ(endPos.y - startPos.y) + SQ(endPos.z - startPos.z));

    self->actor.world.pos.x = radius * cosf(degrees * (M_PI / 180.0f));
    self->actor.world.pos.y = distance;
    self->actor.world.pos.z = radius * sinf(degrees * (M_PI / 180.0f));

    xPos = self->actor.world.pos.x;
    ySpherical = (self->actor.world.pos.y * cosf(rotation.x * (M_PI / 0x8000))) -
                 (sinf(rotation.x * (M_PI / 0x8000)) * self->actor.world.pos.z);
    xzSpherical = (self->actor.world.pos.z * cosf(rotation.x * (M_PI / 0x8000))) +
                  (sinf(rotation.x * (M_PI / 0x8000)) * self->actor.world.pos.y);

    self->actor.world.pos.x =
        (xPos * cosf(rotation.y * (M_PI / 0x8000))) - (sinf(rotation.y * (M_PI / 0x8000)) * xzSpherical);
    self->actor.world.pos.y = ySpherical;
    self->actor.world.pos.z =
        (xzSpherical * cosf(rotation.y * (M_PI / 0x8000))) + (sinf(rotation.y * (M_PI / 0x8000)) * xPos);

    self->actor.world.pos.x += startPos.x;
    self->actor.world.pos.y += startPos.y;
    self->actor.world.pos.z += startPos.z;
}

/**
 * Moves the Jewel Actor spherically from startPos to endPos.
 * This is used by the Jewel Actor during the Door of Time activation cutscene.
 * This is run when the Jewels merge from Link and begin orbiting him.
 */
void DemoEffect_MoveJewelActivateDoorOfTime(DemoEffect* self, GlobalContext* globalCtx) {
    Vec3f startPos;
    Vec3f endPos;
    f32 frameDivisor;
    f32 degrees;
    f32 radius;
    s32 csActionId;

    csActionId = self->csActionId;
    startPos.x = globalCtx->csCtx.npcActions[csActionId]->startPos.x;
    startPos.y = globalCtx->csCtx.npcActions[csActionId]->startPos.y;
    startPos.z = globalCtx->csCtx.npcActions[csActionId]->startPos.z;
    endPos.x = globalCtx->csCtx.npcActions[csActionId]->endPos.x;
    endPos.y = globalCtx->csCtx.npcActions[csActionId]->endPos.y;
    endPos.z = globalCtx->csCtx.npcActions[csActionId]->endPos.z;

    frameDivisor = DemoEffect_InterpolateCsFrames(globalCtx, csActionId);

    switch (self->jewel.type) {
        case DEMO_EFFECT_JEWEL_KOKIRI:
            degrees = 0.0f;
            break;
        case DEMO_EFFECT_JEWEL_GORON:
            degrees = 120.0f;
            break;
        case DEMO_EFFECT_JEWEL_ZORA:
            degrees = 240.0f;
            break;
    }

    radius = 50.0f * frameDivisor;
    if (radius > 30.0f) {
        radius = 30.0f;
    }

    if (startPos.x != endPos.x || startPos.y != endPos.y || startPos.z != endPos.z) {
        self->jewelCsRotation.x = Math_Atan2F(endPos.z - startPos.z, -(endPos.x - startPos.x)) * (0x8000 / M_PI);
        self->jewelCsRotation.y = Math_Vec3f_Yaw(&startPos, &endPos);
    }

    self->jewelCsRotation.z += 0x0400;

    degrees += self->jewelCsRotation.z * (360.0f / 65536.0f);
    DemoEffect_MoveJewelSpherical(degrees, frameDivisor, startPos, endPos, radius, self->jewelCsRotation, self);
}

/**
 * Spawns Sparkle Effects for the Jewel Actor.
 */
void DemoEffect_JewelSparkle(DemoEffect* self, GlobalContext* globalCtx, s32 spawnerCount) {
    Vec3f velocity;
    Vec3f accel;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;
    Color_RGB8* sparkleColors;
    s32 i;

    velocity.y = 0.0f;

    accel.x = 0.0f;
    accel.y = -0.1f;
    accel.z = 0.0f;

    sparkleColors = sJewelSparkleColors[self->jewel.type - DEMO_EFFECT_JEWEL_KOKIRI];

    primColor.r = sparkleColors[0].r;
    primColor.g = sparkleColors[0].g;
    primColor.b = sparkleColors[0].b;
    envColor.r = sparkleColors[1].r;
    envColor.g = sparkleColors[1].g;
    envColor.b = sparkleColors[1].b;
    primColor.a = 0;

    for (i = 0; i < spawnerCount; i++) {
        velocity.x = (Rand_ZeroOne() - 0.5f) * 1.5f;
        velocity.z = (Rand_ZeroOne() - 0.5f) * 1.5f;

        EffectSsKiraKira_SpawnDispersed(globalCtx, &self->actor.world.pos, &velocity, &accel, &primColor, &envColor,
                                        3000, 16);
    }
}

/**
 * Plays Jewel sound effects.
 * The sSfxJewelId global variable is used to ensure only one Jewel Actor is playing SFX when all are spawned.
 */
void DemoEffect_PlayJewelSfx(DemoEffect* self, GlobalContext* globalCtx) {
    if (!DemoEffect_CheckCsAction(self, globalCtx, 1)) {
        if (self->actor.params == sSfxJewelId[0]) {
            func_8002F974(&self->actor, NA_SE_EV_SPIRIT_STONE - SFX_FLAG);
        } else if (sSfxJewelId[0] == 0) {
            sSfxJewelId[0] = self->actor.params;
            func_8002F974(&self->actor, NA_SE_EV_SPIRIT_STONE - SFX_FLAG);
        }
    }
}

/**
 * Update Function for the Jewel Actor that is run when Link is an adult.
 * This rotates the Jewel and updates a timer that is used to scroll Jewel textures.
 * There is a call SetJewelColor that does nothing since 1.0f is passed.
 * If a value of less than 1.0f were passed to SetJewelColor, then it would appear to drain the Jewel's color.
 * This can be seen in preprelease screenshots.
 */
void DemoEffect_UpdateJewelAdult(DemoEffect* self, GlobalContext* globalCtx) {
    self->jewel.timer++;
    self->actor.shape.rot.y += 0x0400;
    DemoEffect_PlayJewelSfx(self, globalCtx);
    DemoEffect_SetJewelColor(self, 1.0f);
}

/**
 * Update Function for the Jewel Actor that is run when Link is a child.
 * This rotates the Jewel and updates a timer that is used to scroll Jewel textures.
 * This also updates the Jewel's position based on different cutscenes.
 */
void DemoEffect_UpdateJewelChild(DemoEffect* self, GlobalContext* globalCtx) {
    s32 hasCmdAction;
    Actor* thisx = &self->actor;

    self->jewel.timer++;

    if (globalCtx->csCtx.state && globalCtx->csCtx.npcActions[self->csActionId]) {
        switch (globalCtx->csCtx.npcActions[self->csActionId]->action) {
            case 3:
                if (gSaveContext.eventChkInf[4] & 0x800) {
                    gSaveContext.eventChkInf[4] |= 0x800;
                }
                DemoEffect_MoveJewelActivateDoorOfTime(self, globalCtx);
                if ((globalCtx->gameplayFrames & 1) == 0) {
                    DemoEffect_JewelSparkle(self, globalCtx, 1);
                }
                break;
            case 4:
                if (self->jewel.isPositionInit) {
                    DemoEffect_MoveToCsEndpoint(self, globalCtx, self->csActionId, 0);
                    DemoEffect_MoveJewelSplit(&thisx->world, self);
                    if ((globalCtx->gameplayFrames & 1) == 0) {
                        DemoEffect_JewelSparkle(self, globalCtx, 1);
                    }
                } else {
                    DemoEffect_InitPositionFromCsAction(self, globalCtx, self->csActionId);
                    DemoEffect_MoveJewelSplit(&thisx->world, self);
                    self->jewel.isPositionInit = 1;
                }
                break;
            case 6:
                Actor_Kill(thisx);
                return;
            default:
                DemoEffect_MoveToCsEndpoint(self, globalCtx, self->csActionId, 0);
                if (gSaveContext.entranceIndex == 0x0053) {
                    DemoEffect_MoveJewelSplit(&thisx->world, self);
                }
                break;
        }
    }

    if (gSaveContext.entranceIndex == 0x0053) {
        if (!(gSaveContext.eventChkInf[4] & 0x800)) {
            hasCmdAction = globalCtx->csCtx.state && globalCtx->csCtx.npcActions[self->csActionId];
            if (!hasCmdAction) {
                self->effectFlags |= 0x1;
                return;
            }
        }
    }

    thisx->shape.rot.y += 0x0400;
    DemoEffect_PlayJewelSfx(self, globalCtx);
    self->effectFlags &= ~1;
}

/**
 * Update Function for the Dust Actor.
 * This is the dust that is spawned in the Temple of Time during the Light Arrows cutscene.
 * This spawns the dust particles and increments a timer
 */
void DemoEffect_UpdateDust(DemoEffect* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    if (globalCtx->csCtx.state != CS_STATE_IDLE && globalCtx->csCtx.npcActions[self->csActionId] != NULL &&
        globalCtx->csCtx.npcActions[self->csActionId]->action == 2) {
        pos = self->actor.world.pos;

        pos.y += 600.0f;
        pos.x += Rand_CenteredFloat(300.0f);
        pos.z += 200.0f + Rand_CenteredFloat(300.0f);

        velocity.z = 0.0f;
        velocity.x = 0.0f;
        velocity.y = -20.0f;

        accel.z = 0.0f;
        accel.x = 0.0f;
        accel.y = 0.2f;

        func_8002873C(globalCtx, &pos, &velocity, &accel, 300, 0, 30);

        self->dust.timer++;
    }
}

/**
 * This is the main Actor Update Function.
 */
void DemoEffect_Update(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    self->updateFunc(self, globalCtx);
}

/**
 * Check if the current cutscene action matches the passed in cutscene action ID.
 */
s32 DemoEffect_CheckCsAction(DemoEffect* self, GlobalContext* globalCtx, s32 csActionCompareId) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE && globalCtx->csCtx.npcActions[self->csActionId] != NULL &&
        globalCtx->csCtx.npcActions[self->csActionId]->action == csActionCompareId) {
        return 1;
    }

    return 0;
}

/**
 * Draw function for the Jewel Actor.
 */
void DemoEffect_DrawJewel(Actor* thisx, GlobalContext* globalCtx2) {
    DemoEffect* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    u32 frames = self->jewel.timer;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2543);

    if (!DemoEffect_CheckCsAction(self, globalCtx, 1)) {
        if (1) {}

        if (!(self->effectFlags & 0x1)) {
            switch (self->jewel.type) {
                case DEMO_EFFECT_JEWEL_KOKIRI:
                    gSPSegment(POLY_XLU_DISP++, 9,
                               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (frames * 4) % 256,
                                                (256 - ((frames * 2) % 256)) - 1, 64, 64, 1, (frames * 2) % 256,
                                                (256 - (frames % 256)) - 1, 16, 16));
                    break;

                case DEMO_EFFECT_JEWEL_GORON:
                    gSPSegment(POLY_XLU_DISP++, 9,
                               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (frames * 4) % 128,
                                                (256 - ((frames * 2) % 256)) - 1, 32, 64, 1, (frames * 2) % 256,
                                                (256 - (frames % 256)) - 1, 16, 8));
                    break;

                case DEMO_EFFECT_JEWEL_ZORA:
                    gSPSegment(POLY_XLU_DISP++, 9,
                               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (frames * 4) % 256,
                                                (256 - ((frames * 2) % 256)) - 1, 32, 32, 1, (frames * 2) % 256,
                                                (256 - (frames % 256)) - 1, 16, 16));
                    break;
            }

            if (!frames) {}

            gSPSegment(POLY_OPA_DISP++, 8, Gfx_TexScroll(globalCtx->state.gfxCtx, (u8)frames, (u8)frames, 16, 16));
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2597),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2599),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            func_80093D84(globalCtx->state.gfxCtx);
            func_8002ED80(&self->actor, globalCtx, 0);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, self->primXluColor[0], self->primXluColor[1],
                            self->primXluColor[2], 255);
            gDPSetEnvColor(POLY_XLU_DISP++, self->envXluColor[0], self->envXluColor[1], self->envXluColor[2], 255);
            gSPDisplayList(POLY_XLU_DISP++, self->jewelDisplayList);
            func_80093D18(globalCtx->state.gfxCtx);
            func_8002EBCC(&self->actor, globalCtx, 0);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 128, self->primOpaColor[0], self->primOpaColor[1],
                            self->primOpaColor[2], 255);
            gDPSetEnvColor(POLY_OPA_DISP++, self->envOpaColor[0], self->envOpaColor[1], self->envOpaColor[2], 255);
            gSPDisplayList(POLY_OPA_DISP++, self->jewelHolderDisplayList);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2620);
}

/**
 * Draw function for the Crystal Light Actor.
 */
void DemoEffect_DrawCrystalLight(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    DemoEffect* parent = (DemoEffect*)self->actor.parent;
    u32 frames = globalCtx->gameplayFrames & 0xFFFF;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2634);

    if (parent != NULL) {
        gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 255, 255, 170, parent->triforceSpot.crystalLightOpacity);
    } else {
        gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 255, 255, 170, 255);
    }

    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 8,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (frames * 2) % 512, 512 - (frames % 512) - 1, 128, 128, 1,
                                512 - ((frames * 2) % 512) - 1, 0, 64, 64));
    Matrix_Push();
    Matrix_RotateY(0.0f, MTXMODE_APPLY);
    Matrix_RotateX((11.0 * M_PI) / 180.0, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 150.0f, 0.0f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2661),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gCrystalLightDL);
    Matrix_Pop();
    Matrix_Push();
    Matrix_RotateY((2.0f * M_PI) / 3.0f, MTXMODE_APPLY);
    Matrix_RotateX((11.0 * M_PI) / 180.0, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 150.0f, 0.0f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2672),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gCrystalLightDL);
    Matrix_Pop();
    Matrix_Push();
    Matrix_RotateY((4.0f * M_PI) / 3.0f, MTXMODE_APPLY);
    Matrix_RotateX((11.0 * M_PI) / 180.0, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 150.0f, 0.0f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2683),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gCrystalLightDL);
    Matrix_Pop();
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2688);
}

/**
 * Draw function for the Fire Ball Actor.
 */
void DemoEffect_DrawFireBall(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    u32 frames = globalCtx->gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2701);
    gDPSetPrimColor(POLY_XLU_DISP++, 64, 64, 255, 200, 0, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 255);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2709),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(POLY_XLU_DISP++, globalCtx->unk_11DE0, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPSegment(
        POLY_XLU_DISP++, 8,
        Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 32, 1, 0, 128 - ((frames * 20) % 128) - 1, 32, 32));
    gSPDisplayList(POLY_XLU_DISP++, gCreationFireBallDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2723);
}

/**
 * Draw function for the God Lgt Actors.
 * This draws either Din, Nayru, or Farore based on the colors set in the DemoEffect struct.
 */
void DemoEffect_DrawGodLgt(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    s32 pad;
    u32 frames = globalCtx->gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2737);

    if (!DemoEffect_CheckCsAction(self, globalCtx, 2)) {
        if (gSaveContext.entranceIndex == 0x00A0) {
            if (gSaveContext.sceneSetupIndex == 4) {
                if (globalCtx->csCtx.frames <= 680) {
                    func_80078914(&self->actor.projectedPos, NA_SE_EV_GOD_FLYING - SFX_FLAG);
                }
            } else {
                func_80078914(&self->actor.projectedPos, NA_SE_EV_GOD_FLYING - SFX_FLAG);
            }
        } else {
            func_80078914(&self->actor.projectedPos, NA_SE_EV_GOD_FLYING - SFX_FLAG);
        }

        gSPSegment(POLY_XLU_DISP++, 8,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (frames * 4) % 512, 0, 128, 64, 1, (frames * 2) % 256,
                                    512 - ((frames * 70) % 512) - 1, 64, 32));
        gSPSegment(POLY_XLU_DISP++, 9,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 16, 96, 1, (frames * 10) % 256,
                                    256 - ((frames * 30) % 512) - 1, 8, 32));
        gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, self->primXluColor[0], self->primXluColor[1], self->primXluColor[2],
                        255);
        gDPSetEnvColor(POLY_XLU_DISP++, self->envXluColor[0], self->envXluColor[1], self->envXluColor[2], 255);
        func_80093D84(globalCtx->state.gfxCtx);
        Matrix_Push();
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2801),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gGoldenGoddessAuraDL);
        func_80093D18(globalCtx->state.gfxCtx);
        func_8002EBCC(&self->actor, globalCtx, 0);
        Matrix_Pop();

        self->godLgt.rotation++;
        if (self->godLgt.rotation > 120) {
            self->godLgt.rotation = 0;
            if (1) {}
        }

        Matrix_RotateZ((((s32)self->godLgt.rotation) * 3.0f) * (M_PI / 180.0f), MTXMODE_APPLY);
        Matrix_RotateX(M_PI / 2.0f, MTXMODE_APPLY);
        Matrix_Translate(0.0f, -140.0f, 0.0f, MTXMODE_APPLY);
        Matrix_Scale(0.03f, 0.03f, 0.03f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2824),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gGoldenGoddessBodyDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2829);
}

/**
 * Draw function for the Light Effect Actor.
 */
void DemoEffect_DrawLightEffect(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    u8* alpha;
    Gfx* disp;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2842);

    if (!DemoEffect_CheckCsAction(self, globalCtx, 1)) {

        if (self->light.flicker == 0) {
            self->light.flicker = 1;
        } else {
            disp = (u32)gEffFlash1DL; // necessary to match, should be able to remove after fake matches are fixed
            alpha = &self->light.alpha;
            func_80093D84(globalCtx->state.gfxCtx);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, self->primXluColor[0], self->primXluColor[1],
                            self->primXluColor[2], *alpha);
            gDPSetEnvColor(POLY_XLU_DISP++, self->envXluColor[0], self->envXluColor[1], self->envXluColor[2], 255);
            Matrix_Scale(((self->light.scaleFlag & 1) * 0.05f) + 1.0f, ((self->light.scaleFlag & 1) * 0.05f) + 1.0f,
                         ((self->light.scaleFlag & 1) * 0.05f) + 1.0f, MTXMODE_APPLY);
            Matrix_Push();
            Matrix_Mult(&globalCtx->mf_11DA0, MTXMODE_APPLY);
            Matrix_RotateZ(self->light.rotation * (M_PI / 180.0f), MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2866),
                      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
            if (disp) {};
            gSPDisplayList(POLY_XLU_DISP++, disp);
            Matrix_Pop();
            Matrix_Mult(&globalCtx->mf_11DA0, MTXMODE_APPLY);
            Matrix_RotateZ(-(f32)self->light.rotation * (M_PI / 180.0f), MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2874),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, disp);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2881);
}

/**
 * Draw function for the Blue Orb Actor.
 */
void DemoEffect_DrawBlueOrb(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    s32 pad2;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2892);
    gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 188, 255, 255, self->blueOrb.alpha);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, 255);
    func_80093D84(globalCtx->state.gfxCtx);
    Matrix_Mult(&globalCtx->mf_11DA0, MTXMODE_APPLY);
    Matrix_RotateZ(self->blueOrb.rotation * (M_PI / 0x8000), MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2901),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    self->blueOrb.rotation += 0x01F4;
    gSPDisplayList(POLY_XLU_DISP++, gEffFlash1DL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2907);
}

/**
 * Draw function for the Lgt Shower Actor.
 */
void DemoEffect_DrawLgtShower(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    s32 pad;
    u32 frames = globalCtx->gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2921);
    gDPSetPrimColor(POLY_XLU_DISP++, 64, 64, 255, 255, 160, self->lgtShower.alpha);
    gDPSetEnvColor(POLY_XLU_DISP++, 50, 200, 0, 255);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2927),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 8,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (frames * 5) % 1024, 0, 256, 64, 1, (frames * 10) % 128,
                                512 - ((frames * 50) % 512), 32, 16));
    gSPDisplayList(POLY_XLU_DISP++, gEnliveningLightDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2942);
}

/**
 * Draw function for the Light Ring Actor.
 */
void DemoEffect_DrawLightRing(Actor* thisx, GlobalContext* globalCtx2) {
    DemoEffect* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    u32 frames = self->lightRing.timer;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2956);

    func_80093D84(globalCtx->state.gfxCtx);
    gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 170, 255, 255, self->lightRing.alpha);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, 255);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2963),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 8,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (frames * 5) % 64, 512 - ((frames * 2) % 512) - 1, 16, 128,
                                1, 0, 0, 8, 1024));
    gSPDisplayList(POLY_XLU_DISP++, gGoldenGoddessLightRingDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2978);
}

/**
 * Draw function for the Triforce Spot Actor.
 */
void DemoEffect_DrawTriforceSpot(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    s32 pad;
    Vtx* vertices = SEGMENTED_TO_VIRTUAL(gTriforceVtx);
    u32 frames = globalCtx->gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 2994);
    if (gSaveContext.entranceIndex != 0x0400 || globalCtx->csCtx.frames < 885) {
        func_80093D84(globalCtx->state.gfxCtx);

        if (self->triforceSpot.lightColumnOpacity > 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_AURORA - SFX_FLAG);
            Matrix_Push();
            Matrix_Scale(1.0f, 2.4f, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 3011),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(POLY_XLU_DISP++, 9,
                       Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 256 - ((frames * 4) % 256) - 1, 64, 64, 1, 0,
                                        256 - ((frames * 2) % 256) - 1, 64, 32));
            vertices[86].n.a = vertices[87].n.a = vertices[88].n.a = vertices[89].n.a = vertices[92].n.a =
                vertices[93].n.a = vertices[94].n.a = vertices[95].n.a = (s8)self->triforceSpot.lightColumnOpacity;
            gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 180, 255, 255, self->triforceSpot.lightColumnOpacity);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 150, 255);
            gSPDisplayList(POLY_XLU_DISP++, gTriforceLightColumnDL);
            Matrix_Pop();
        }

        if (self->triforceSpot.triforceSpotOpacity != 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_TRIFORCE - SFX_FLAG);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 3042),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            if (self->triforceSpot.triforceSpotOpacity < 250) {
                func_8002ED80(&self->actor, globalCtx, 0);
                func_80093D84(globalCtx->state.gfxCtx);
                gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_AA_ZB_XLU_SURF2);
                Matrix_RotateY(self->triforceSpot.rotation * (M_PI / 0x8000), MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 3053),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPSegment(POLY_XLU_DISP++, 8,
                           Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 16, 1, 0, 0, 16, 8));
                gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 255, 255, 160, self->triforceSpot.triforceSpotOpacity);
                gDPSetEnvColor(POLY_XLU_DISP++, 170, 140, 0, 255);
                gSPDisplayList(POLY_XLU_DISP++, gTriforceDL);
            } else {
                func_8002EBCC(&self->actor, globalCtx, 0);
                func_80093D18(globalCtx->state.gfxCtx);
                gDPSetRenderMode(POLY_OPA_DISP++, G_RM_PASS, G_RM_AA_ZB_OPA_SURF2);
                Matrix_RotateY(self->triforceSpot.rotation * (M_PI / 0x8000), MTXMODE_APPLY);
                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_demo_effect.c", 3085),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPSegment(POLY_OPA_DISP++, 8,
                           Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 16, 1, 0, 0, 16, 8));
                gDPSetPrimColor(POLY_OPA_DISP++, 128, 128, 255, 255, 160, 255);
                gDPSetEnvColor(POLY_OPA_DISP++, 170, 140, 0, 255);
                gSPDisplayList(POLY_OPA_DISP++, gTriforceDL);
            }
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 3112);
}

/**
 * Draw function for the Get Item Actors.
 * This is either Medals or Light Arrows based on the drawId.
 */
void DemoEffect_DrawGetItem(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    if (!DemoEffect_CheckCsAction(self, globalCtx, 1) && !DemoEffect_CheckCsAction(self, globalCtx, 4)) {
        if (!self->getItem.isLoaded) {
            self->getItem.isLoaded = 1;
            return;
        }
        func_8002EBCC(thisx, globalCtx, 0);
        func_8002ED80(thisx, globalCtx, 0);
        GetItem_Draw(globalCtx, self->getItem.drawId);
    }
}

/**
 * Callback for the SkelCurve system to draw the animated limbs.
 */
s32 DemoEffect_DrawTimewarpLimbs(GlobalContext* globalCtx, SkelAnimeCurve* skelCuve, s32 limbIndex, void* thisx) {
    s32 pad;
    DemoEffect* self = THIS;
    u32 frames = globalCtx->gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 3154);
    func_80093D84(globalCtx->state.gfxCtx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, 170, 255, 255, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, self->envXluColor[0], self->envXluColor[1], self->envXluColor[2], 255);
    gSPSegment(POLY_XLU_DISP++, 8,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (frames * 6) % 1024, 256 - ((frames * 16) % 256) - 1, 256,
                                64, 1, (frames * 4) % 512, 128 - ((frames * 12) % 128) - 1, 128, 32));
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_effect.c", 3172);

    if (limbIndex == 0) {
        LimbTransform* transform = &skelCuve->transforms[0];

        transform->scale.y = 1024;
        transform->scale.z = transform->scale.x = 1024;
    }

    return 1;
}

/**
 * Draw function for the Time Warp Actors.
 */
void DemoEffect_DrawTimeWarp(Actor* thisx, GlobalContext* globalCtx) {
    DemoEffect* self = THIS;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    u8 effectType = (self->actor.params & 0x00FF);

    if (effectType == DEMO_EFFECT_TIMEWARP_TIMEBLOCK_LARGE || effectType == DEMO_EFFECT_TIMEWARP_TIMEBLOCK_SMALL ||
        Flags_GetEnv(globalCtx, 1) || gSaveContext.sceneSetupIndex >= 4 || gSaveContext.entranceIndex == 0x0324) {
        OPEN_DISPS(gfxCtx, "../z_demo_effect.c", 3201);
        POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 25);
        Matrix_Scale(2.0f, 2.0f, 2.0f, MTXMODE_APPLY);
        SkelCurve_Draw(thisx, globalCtx, &self->skelCurve, DemoEffect_DrawTimewarpLimbs, NULL, 1, self);
        CLOSE_DISPS(gfxCtx, "../z_demo_effect.c", 3216);
    }
}

/**
 * Faces/rotates the Actor towards the current cutscene action end point.
 */
void DemoEffect_FaceToCsEndpoint(DemoEffect* self, Vec3f startPos, Vec3f endPos) {
    s32 pad;
    f32 x = endPos.x - startPos.x;
    f32 z = endPos.z - startPos.z;
    f32 xzDistance = sqrtf(SQ(x) + SQ(z));

    self->actor.shape.rot.y = Math_FAtan2F(x, z) * (32768.0f / M_PI);
    self->actor.shape.rot.x = Math_FAtan2F(-(endPos.y - startPos.y), xzDistance) * (32768.0f / M_PI);
}

/**
 * Moves the Actor towards the current cutscene action end point.
 * Will only update the Actor's facing/rotation if the shouldUpdateFacing argument is true.
 * The speed is based on the current progress in the cutscene action.
 */
void DemoEffect_MoveToCsEndpoint(DemoEffect* self, GlobalContext* globalCtx, s32 csActionId, s32 shouldUpdateFacing) {
    Vec3f startPos;
    Vec3f endPos;
    f32 speed;

    startPos.x = globalCtx->csCtx.npcActions[csActionId]->startPos.x;
    startPos.y = globalCtx->csCtx.npcActions[csActionId]->startPos.y;
    startPos.z = globalCtx->csCtx.npcActions[csActionId]->startPos.z;
    endPos.x = globalCtx->csCtx.npcActions[csActionId]->endPos.x;
    endPos.y = globalCtx->csCtx.npcActions[csActionId]->endPos.y;
    endPos.z = globalCtx->csCtx.npcActions[csActionId]->endPos.z;

    speed = DemoEffect_InterpolateCsFrames(globalCtx, csActionId);

    self->actor.world.pos.x = ((endPos.x - startPos.x) * speed) + startPos.x;
    self->actor.world.pos.y = ((endPos.y - startPos.y) * speed) + startPos.y;
    self->actor.world.pos.z = ((endPos.z - startPos.z) * speed) + startPos.z;

    if (shouldUpdateFacing) {
        DemoEffect_FaceToCsEndpoint(self, startPos, endPos);
    }
}

/**
 * Moves a GetItem actor towards the current cutscene action's endpoint.
 */
void DemoEffect_MoveGetItem(DemoEffect* self, GlobalContext* globalCtx, s32 csActionId, f32 speed) {
    Vec3f endPos;
    endPos.x = globalCtx->csCtx.npcActions[csActionId]->endPos.x;
    endPos.y = globalCtx->csCtx.npcActions[csActionId]->endPos.y;
    endPos.z = globalCtx->csCtx.npcActions[csActionId]->endPos.z;
    DemoEffect_MoveTowardTarget(endPos, self, speed);
}

/**
 * Initializes the Actor's position to the current cutscene action's start point.
 */
void DemoEffect_InitPositionFromCsAction(DemoEffect* self, GlobalContext* globalCtx, s32 csActionIndex) {
    f32 x = globalCtx->csCtx.npcActions[csActionIndex]->startPos.x;
    f32 y = globalCtx->csCtx.npcActions[csActionIndex]->startPos.y;
    f32 z = globalCtx->csCtx.npcActions[csActionIndex]->startPos.z;

    self->actor.world.pos.x = x;
    self->actor.world.pos.y = y;
    self->actor.world.pos.z = z;
}
