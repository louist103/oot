#include "z_demo_du.h"
#include "objects/object_du/object_du.h"
#include "overlays/actors/ovl_Demo_Effect/z_demo_effect.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((DemoDu*)thisx)

typedef void (*DemoDuActionFunc)(DemoDu*, GlobalContext*);
typedef void (*DemoDuDrawFunc)(Actor*, GlobalContext*);

void DemoDu_Init(Actor* thisx, GlobalContext* globalCtx);
void DemoDu_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DemoDu_Update(Actor* thisx, GlobalContext* globalCtx);
void DemoDu_Draw(Actor* thisx, GlobalContext* globalCtx);

static s32 sUnused = 0;

#include "z_demo_du_cutscene_data.c" EARLY

static void* sEyeTextures[] = { gDaruniaEyeOpenTex, gDaruniaEyeOpeningTex, gDaruniaEyeShutTex, gDaruniaEyeClosingTex };
static void* sMouthTextures[] = { gDaruniaMouthSeriousTex, gDaruniaMouthGrinningTex, gDaruniaMouthOpenTex,
                                  gDaruniaMouthHappyTex };

/**
 * Cs => Cutscene
 *
 * FM => Fire Medallion
 * GR => Goron's Ruby
 * AG => In the chamber of sages, just After the final blow on Ganon.
 * CR => Credits
 *
 */

// Each macro maps its argument to an index of sUpdateFuncs.
#define CS_FIREMEDALLION_SUBSCENE(x) (0 + (x))      // DEMO_DU_CS_FIREMEDALLION
#define CS_GORONSRUBY_SUBSCENE(x) (7 + (x))         // DEMO_DU_CS_GORONS_RUBY
#define CS_CHAMBERAFTERGANON_SUBSCENE(x) (21 + (x)) // DEMO_DU_CS_CHAMBER_AFTER_GANON
#define CS_CREDITS_SUBSCENE(x) (24 + (x))           // DEMO_DU_CS_CREDITS

void DemoDu_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    DemoDu* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
}

void DemoDu_UpdateEyes(DemoDu* self) {
    s16* blinkTimer = &self->blinkTimer;
    s16* eyeTexIndex = &self->eyeTexIndex;
    s32 pad[3];

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset(60, 60);
    }

    *eyeTexIndex = *blinkTimer;
    if (*eyeTexIndex >= 3) {
        *eyeTexIndex = 0;
    }
}

void DemoDu_SetEyeTexIndex(DemoDu* self, s16 eyeTexIndex) {
    self->eyeTexIndex = eyeTexIndex;
}

void DemoDu_SetMouthTexIndex(DemoDu* self, s16 mouthTexIndex) {
    self->mouthTexIndex = mouthTexIndex;
}

// Resets all the values used in self cutscene.
void DemoDu_CsAfterGanon_Reset(DemoDu* self) {
    self->updateIndex = CS_CHAMBERAFTERGANON_SUBSCENE(0);
    self->drawIndex = 0;
    self->shadowAlpha = 0;
    self->demo6KSpawned = 0;
    self->actor.shape.shadowAlpha = 0;
    self->unk_1A4 = 0.0f;
}

void DemoDu_CsAfterGanon_CheckIfShouldReset(DemoDu* self, GlobalContext* globalCtx) {
    static s32 D_8096CE94 = false;

    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        if (D_8096CE94) {
            if (self->actor.params == DEMO_DU_CS_CHAMBER_AFTER_GANON) {
                DemoDu_CsAfterGanon_Reset(self);
            }
            D_8096CE94 = false;
            return;
        }
    } else if (!D_8096CE94) {
        D_8096CE94 = true;
    }
}

s32 DemoDu_UpdateSkelAnime(DemoDu* self) {
    return SkelAnime_Update(&self->skelAnime);
}

void DemoDu_UpdateBgCheckInfo(DemoDu* self, GlobalContext* globalCtx) {
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 75.0f, 30.0f, 30.0f, 5);
}

CsCmdActorAction* DemoDu_GetNpcAction(GlobalContext* globalCtx, s32 idx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        return globalCtx->csCtx.npcActions[idx];
    }
    return NULL;
}

s32 DemoDu_IsNpcDoingThisAction(DemoDu* self, GlobalContext* globalCtx, u16 action, s32 idx) {
    CsCmdActorAction* npcAction = DemoDu_GetNpcAction(globalCtx, idx);

    if ((npcAction != NULL) && (npcAction->action == action)) {
        return true;
    }
    return false;
}

s32 DemoDu_IsNpcNotDoingThisAction(DemoDu* self, GlobalContext* globalCtx, u16 action, s32 idx) {
    CsCmdActorAction* npcAction = DemoDu_GetNpcAction(globalCtx, idx);

    if ((npcAction != NULL) && (npcAction->action != action)) {
        return true;
    }
    return false;
}

void DemoDu_MoveToNpcPos(DemoDu* self, GlobalContext* globalCtx, s32 idx) {
    CsCmdActorAction* npcAction = DemoDu_GetNpcAction(globalCtx, idx);
    s32 pad;

    if (npcAction != NULL) {
        self->actor.world.pos.x = npcAction->startPos.x;
        self->actor.world.pos.y = npcAction->startPos.y;
        self->actor.world.pos.z = npcAction->startPos.z;

        self->actor.world.rot.y = self->actor.shape.rot.y = npcAction->rot.y;
    }
}

void func_80969DDC(DemoDu* self, AnimationHeader* animation, u8 mode, f32 morphFrames, s32 arg4) {
    f32 startFrame;
    s16 lastFrame = Animation_GetLastFrame(animation);
    f32 endFrame;
    f32 playSpeed;

    if (arg4 == 0) {
        startFrame = 0.0f;
        endFrame = lastFrame;
        playSpeed = 1.0f;
    } else {
        endFrame = 0.0f;
        playSpeed = -1.0f;
        startFrame = lastFrame;
    }
    Animation_Change(&self->skelAnime, animation, playSpeed, startFrame, endFrame, mode, morphFrames);
}

void DemoDu_InitCs_FireMedallion(DemoDu* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gDaruniaSkel, &gDaruniaIdleAnim, NULL, NULL, 0);
    self->actor.shape.yOffset = -10000.0f;
    DemoDu_SetEyeTexIndex(self, 1);
    DemoDu_SetMouthTexIndex(self, 3);
}

// A.k.a Warp portal
void DemoDu_CsFireMedallion_SpawnDoorWarp(DemoDu* self, GlobalContext* globalCtx) {
    f32 posX = self->actor.world.pos.x;
    f32 posY = self->actor.world.pos.y;
    f32 posZ = self->actor.world.pos.z;

    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, posX, posY, posZ, 0, 0, 0,
                       WARP_SAGES);
}

// Gives the Fire Medallion to Link.
void func_80969F38(DemoDu* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 posX = player->actor.world.pos.x;
    f32 posY = player->actor.world.pos.y + 80.0f;
    f32 posZ = player->actor.world.pos.z;

    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_EFFECT, posX, posY, posZ, 0, 0, 0,
                       DEMO_EFFECT_MEDAL_FIRE);
    Item_Give(globalCtx, ITEM_MEDALLION_FIRE);
}

void func_80969FB4(DemoDu* self, GlobalContext* globalCtx) {
    self->actor.shape.yOffset += 250.0f / 3.0f;
}

// Gives the Fire Medallion to Link too.
void DemoDu_CsFireMedallion_AdvanceTo01(DemoDu* self, GlobalContext* globalCtx) {
    s32 pad[2];

    if ((gSaveContext.chamberCutsceneNum == 1) && (gSaveContext.sceneSetupIndex < 4)) {
        Player* player = GET_PLAYER(globalCtx);

        self->updateIndex = CS_FIREMEDALLION_SUBSCENE(1);
        globalCtx->csCtx.segment = D_8096C1A4;
        gSaveContext.cutsceneTrigger = 2;
        Item_Give(globalCtx, ITEM_MEDALLION_FIRE);

        player->actor.world.rot.y = player->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
    }
}

void DemoDu_CsFireMedallion_AdvanceTo02(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[2];

        if ((npcAction != NULL) && (npcAction->action != 1)) {
            self->updateIndex = CS_FIREMEDALLION_SUBSCENE(2);
            self->drawIndex = 1;
            DemoDu_CsFireMedallion_SpawnDoorWarp(self, globalCtx);
        }
    }
}

void DemoDu_CsFireMedallion_AdvanceTo03(DemoDu* self) {
    if (self->actor.shape.yOffset >= 0.0f) {
        self->updateIndex = CS_FIREMEDALLION_SUBSCENE(3);
        self->actor.shape.yOffset = 0.0f;
    }
}

void DemoDu_CsFireMedallion_AdvanceTo04(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[2];

        if ((npcAction != NULL) && (npcAction->action != 2)) {
            Animation_Change(&self->skelAnime, &gDaruniaItemGiveAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gDaruniaItemGiveAnim), 2, 0.0f);
            self->updateIndex = CS_FIREMEDALLION_SUBSCENE(4);
        }
    }
}

void DemoDu_CsFireMedallion_AdvanceTo05(DemoDu* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gDaruniaItemGiveIdleAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gDaruniaItemGiveIdleAnim), 0, 0.0f);
        self->updateIndex = CS_FIREMEDALLION_SUBSCENE(5);
    }
}

void DemoDu_CsFireMedallion_AdvanceTo06(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[6];

        if ((npcAction != NULL) && (npcAction->action == 2)) {
            self->updateIndex = CS_FIREMEDALLION_SUBSCENE(6);
            func_80969F38(self, globalCtx);
        }
    }
}

void DemoDu_UpdateCs_FM_00(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_CsFireMedallion_AdvanceTo01(self, globalCtx);
}

void DemoDu_UpdateCs_FM_01(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_CsFireMedallion_AdvanceTo02(self, globalCtx);
}

void DemoDu_UpdateCs_FM_02(DemoDu* self, GlobalContext* globalCtx) {
    func_80969FB4(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_CsFireMedallion_AdvanceTo03(self);
}

void DemoDu_UpdateCs_FM_03(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_CsFireMedallion_AdvanceTo04(self, globalCtx);
}

void DemoDu_UpdateCs_FM_04(DemoDu* self, GlobalContext* globalCtx) {
    s32 animFinished;

    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    animFinished = DemoDu_UpdateSkelAnime(self);
    DemoDu_CsFireMedallion_AdvanceTo05(self, animFinished);
}

void DemoDu_UpdateCs_FM_05(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_CsFireMedallion_AdvanceTo06(self, globalCtx);
}

void DemoDu_UpdateCs_FM_06(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
}

void DemoDu_InitCs_GoronsRuby(DemoDu* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gDaruniaSkel, NULL, NULL, NULL, 0);
    self->updateIndex = CS_GORONSRUBY_SUBSCENE(0);
}

// Cutscene: Darunia gives Link the Goron's Ruby.
// Sfx played when Darunia lands at the floor at the start of the cutscene.
void DemoDu_CsPlaySfx_GoronLanding(DemoDu* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_EN_GOLON_LAND_BIG);
}

// Cutscene: Darunia gives Link the Goron's Ruby.
// Sfx played when Darunia is falling at the start of the cutscene.
void DemoDu_CsPlaySfx_DaruniaFalling(GlobalContext* globalCtx) {
    if (globalCtx->csCtx.frames == 160) {
        func_800788CC(NA_SE_EV_OBJECT_FALL);
    }
}

// Cutscene: Darunia gives Link the Goron's Ruby.
void DemoDu_CsPlaySfx_DaruniaHitsLink(GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;

    func_80078914(&player->actor.projectedPos, NA_SE_EN_DARUNIA_HIT_LINK);
    Audio_PlaySoundGeneral(NA_SE_VO_LI_DAMAGE_S_KID, &player->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);
}

// Cutscene: Darunia gives Link the Goron's Ruby.
void DemoDu_CsPlaySfx_HitBreast(DemoDu* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_EN_DARUNIA_HIT_BREAST - SFX_FLAG);
}

// Cutscene: Darunia gives Link the Goron's Ruby.
// Sfx played when Link is escaping from the gorons at the end of the scene.
void DemoDu_CsPlaySfx_LinkEscapeFromGorons(GlobalContext* globalCtx) {
    if (globalCtx->csCtx.frames == 1400) {
        Player* player = GET_PLAYER(globalCtx);

        Audio_PlaySoundGeneral(NA_SE_VO_LI_FALL_L_KID, &player->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

// Cutscene: Darunia gives Link the Goron's Ruby.
// Sfx played when Link is surprised by Darunia falling from the sky.
void DemoDu_CsPlaySfx_LinkSurprised(GlobalContext* globalCtx) {
    if (globalCtx->csCtx.frames == 174) {
        Player* player = GET_PLAYER(globalCtx);

        Audio_PlaySoundGeneral(NA_SE_VO_LI_SURPRISE_KID, &player->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

void DemoDu_CsGoronsRuby_UpdateFaceTextures(DemoDu* self, GlobalContext* globalCtx) {
    u16* frames = &globalCtx->csCtx.frames;

    if (*frames < 260) {
        DemoDu_UpdateEyes(self);
        DemoDu_SetMouthTexIndex(self, 0);
    } else if (*frames < 335) {
        DemoDu_UpdateEyes(self);
        DemoDu_SetMouthTexIndex(self, 3);
    } else if (*frames < 365) {
        DemoDu_SetEyeTexIndex(self, 3);
        DemoDu_SetMouthTexIndex(self, 1);
    } else if (*frames < 395) {
        DemoDu_SetEyeTexIndex(self, 0);
        DemoDu_SetMouthTexIndex(self, 3);
    } else if (*frames < 410) {
        DemoDu_UpdateEyes(self);
        DemoDu_SetMouthTexIndex(self, 0);
    } else {
        DemoDu_UpdateEyes(self);
        DemoDu_SetMouthTexIndex(self, 3);
    }
}

void func_8096A630(DemoDu* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f pos = self->actor.world.pos;

    pos.y += kREG(5);
    func_80033480(globalCtx, &pos, kREG(1) + 100.0f, kREG(2) + 10, kREG(3) + 300, kREG(4), 0);
    DemoDu_CsPlaySfx_GoronLanding(self);
}

void DemoDu_CsGoronsRuby_SpawnDustWhenHittingLink(DemoDu* self, GlobalContext* globalCtx) {
    static Vec3f dustPosOffsets[] = {
        { 11.0f, -11.0f, -6.0f }, { 0.0f, 14.0f, -13.0f },  { 14.0f, -2.0f, -10.0f }, { 10.0f, -6.0f, -8.0f },
        { 8.0f, 6.0f, 8.0f },     { 13.0f, 8.0f, -10.0f },  { -14.0f, 1.0f, -14.0f }, { 5.0f, 12.0f, -9.0f },
        { 11.0f, 6.0f, -7.0f },   { 14.0f, 14.0f, -14.0f },
    };

    if (Animation_OnFrame(&self->skelAnime, 31.0f) || Animation_OnFrame(&self->skelAnime, 41.0f)) {
        s32 pad[2];
        s32 i;
        Player* player = GET_PLAYER(globalCtx);
        Vec3f* headPos = &player->bodyPartsPos[PLAYER_LIMB_HEAD];
        Vec3f velocity = { 0.0f, 0.0f, 0.0f };
        Vec3f accel = { 0.0f, 0.3f, 0.0f };
        s32 pad2;

        for (i = 4; i >= 0; --i) {
            Color_RGBA8 primColor = { 190, 150, 110, 255 };
            Color_RGBA8 envColor = { 120, 80, 40, 255 };
            s32 colorDelta;
            Vec3f position;

            if (Animation_OnFrame(&self->skelAnime, 31.0f)) {
                position.x = dustPosOffsets[i + 5].x + headPos->x;
                position.y = dustPosOffsets[i + 5].y + headPos->y;
                position.z = dustPosOffsets[i + 5].z + headPos->z;
            } else {
                position.x = dustPosOffsets[i + 0].x + headPos->x;
                position.y = dustPosOffsets[i + 0].y + headPos->y;
                position.z = dustPosOffsets[i + 0].z + headPos->z;
            }

            colorDelta = Rand_ZeroOne() * 20.0f - 10.0f;

            primColor.r += colorDelta;
            primColor.g += colorDelta;
            primColor.b += colorDelta;
            envColor.r += colorDelta;
            envColor.g += colorDelta;
            envColor.b += colorDelta;

            func_8002829C(globalCtx, &position, &velocity, &accel, &primColor, &envColor,
                          Rand_ZeroOne() * 40.0f + 200.0f, 0);
        }

        DemoDu_CsPlaySfx_DaruniaHitsLink(globalCtx);
    }
}

void DemoDu_CsGoronsRuby_DaruniaFalling(DemoDu* self, GlobalContext* globalCtx) {
    s32 pad;
    CutsceneContext* csCtx = &globalCtx->csCtx;

    if (csCtx->state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = csCtx->npcActions[2];
        Vec3f startPos;
        Vec3f endPos;
        Vec3f* pos = &self->actor.world.pos;

        if (npcAction != NULL) {
            f32 traveledPercent = Environment_LerpWeight(npcAction->endFrame, npcAction->startFrame, csCtx->frames);

            startPos.x = npcAction->startPos.x;
            startPos.y = npcAction->startPos.y;
            startPos.z = npcAction->startPos.z;

            endPos.x = npcAction->endPos.x;
            endPos.y = npcAction->endPos.y;
            endPos.z = npcAction->endPos.z;

            pos->x = ((endPos.x - startPos.x) * traveledPercent) + startPos.x;
            pos->y = ((endPos.y - startPos.y) * traveledPercent) + startPos.y;
            pos->z = ((endPos.z - startPos.z) * traveledPercent) + startPos.z;
        }
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo01(DemoDu* self, GlobalContext* globalCtx) {
    self->updateIndex = CS_GORONSRUBY_SUBSCENE(1);
}

void DemoDu_CsGoronsRuby_AdvanceTo02(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[2];

        if ((npcAction != NULL) && (npcAction->action != 1)) {
            Animation_Change(&self->skelAnime, &gDaruniaStandUpAfterFallingAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gDaruniaStandUpAfterFallingAnim), 2, 0.0f);
            self->updateIndex = CS_GORONSRUBY_SUBSCENE(2);
            self->drawIndex = 1;
            DemoDu_CsGoronsRuby_DaruniaFalling(self, globalCtx);
        }
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo03(DemoDu* self, GlobalContext* globalCtx) {
    CutsceneContext* csCtx = &globalCtx->csCtx;

    if (csCtx->state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = csCtx->npcActions[2];

        if ((npcAction != NULL) && (csCtx->frames >= npcAction->endFrame)) {
            self->updateIndex = CS_GORONSRUBY_SUBSCENE(3);
            func_8096A630(self, globalCtx);
        }
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo04(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[2];

        if ((npcAction != NULL) && (npcAction->action != 2)) {
            self->updateIndex = CS_GORONSRUBY_SUBSCENE(4);
        }
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo05(DemoDu* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gDaruniaIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gDaruniaIdleAnim),
                         ANIMMODE_LOOP, 0.0f);
        self->updateIndex = CS_GORONSRUBY_SUBSCENE(5);
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo06(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[2];

        if ((npcAction != NULL) && (npcAction->action != 3)) {
            Animation_Change(&self->skelAnime, &gDaruniaHitBreastAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gDaruniaHitBreastAnim), 2, -4.0f);
            self->updateIndex = CS_GORONSRUBY_SUBSCENE(6);
        }
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo07(DemoDu* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gDaruniaIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gDaruniaIdleAnim),
                         ANIMMODE_LOOP, 0.0f);
        self->updateIndex = CS_GORONSRUBY_SUBSCENE(7);
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo08(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[2];

        if ((npcAction != NULL) && (npcAction->action != 4)) {
            Animation_Change(&self->skelAnime, &gDaruniaHitLinkAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gDaruniaHitLinkAnim), 2, 0.0f);
            self->updateIndex = CS_GORONSRUBY_SUBSCENE(8);
        }
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo09(DemoDu* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gDaruniaHitBreastAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gDaruniaHitBreastAnim), 2, 0.0f);
        self->updateIndex = CS_GORONSRUBY_SUBSCENE(9);
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo10(DemoDu* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gDaruniaIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gDaruniaIdleAnim),
                         ANIMMODE_LOOP, 0.0f);
        self->updateIndex = CS_GORONSRUBY_SUBSCENE(10);
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo11(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[2];

        if ((npcAction != NULL) && (npcAction->action != 5)) {
            Animation_Change(&self->skelAnime, &gDaruniaItemGiveAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gDaruniaItemGiveAnim), 2, 0.0f);
            self->updateIndex = CS_GORONSRUBY_SUBSCENE(11);
        }
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo12(DemoDu* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gDaruniaItemGiveIdleAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gDaruniaItemGiveIdleAnim), 0, 0.0f);
        self->updateIndex = CS_GORONSRUBY_SUBSCENE(12);
    }
}

void DemoDu_CsGoronsRuby_AdvanceTo13(DemoDu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[2];

        if ((npcAction != NULL) && (npcAction->action != 6)) {
            Animation_Change(&self->skelAnime, &gDaruniaIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gDaruniaIdleAnim),
                             ANIMMODE_LOOP, 0.0f);
            self->updateIndex = CS_GORONSRUBY_SUBSCENE(13);
        }
    }
}

void DemoDu_UpdateCs_GR_00(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_CsPlaySfx_DaruniaFalling(globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo01(self, globalCtx);
}

void DemoDu_UpdateCs_GR_01(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_CsPlaySfx_DaruniaFalling(globalCtx);
    DemoDu_CsPlaySfx_LinkSurprised(globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo02(self, globalCtx);
}

void DemoDu_UpdateCs_GR_02(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_CsGoronsRuby_DaruniaFalling(self, globalCtx);
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_CsPlaySfx_DaruniaFalling(globalCtx);
    DemoDu_CsPlaySfx_LinkSurprised(globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo03(self, globalCtx);
}

void DemoDu_UpdateCs_GR_03(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_CsPlaySfx_LinkSurprised(globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo04(self, globalCtx);
}

void DemoDu_UpdateCs_GR_04(DemoDu* self, GlobalContext* globalCtx) {
    s32 animFinished;

    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    animFinished = DemoDu_UpdateSkelAnime(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo05(self, animFinished);
}

void DemoDu_UpdateCs_GR_05(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo06(self, globalCtx);
}

void DemoDu_UpdateCs_GR_06(DemoDu* self, GlobalContext* globalCtx) {
    s32 animFinished;

    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    animFinished = DemoDu_UpdateSkelAnime(self);
    DemoDu_CsPlaySfx_HitBreast(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo07(self, animFinished);
}

void DemoDu_UpdateCs_GR_07(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo08(self, globalCtx);
}

void DemoDu_UpdateCs_GR_08(DemoDu* self, GlobalContext* globalCtx) {
    s32 animFinished;

    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    animFinished = DemoDu_UpdateSkelAnime(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_SpawnDustWhenHittingLink(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo09(self, animFinished);
}

void DemoDu_UpdateCs_GR_09(DemoDu* self, GlobalContext* globalCtx) {
    s32 animFinished;

    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    animFinished = DemoDu_UpdateSkelAnime(self);
    DemoDu_CsPlaySfx_HitBreast(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo10(self, animFinished);
}

void DemoDu_UpdateCs_GR_10(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo11(self, globalCtx);
}

void DemoDu_UpdateCs_GR_11(DemoDu* self, GlobalContext* globalCtx) {
    s32 animFinished;

    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    animFinished = DemoDu_UpdateSkelAnime(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo12(self, animFinished);
}

void DemoDu_UpdateCs_GR_12(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsGoronsRuby_AdvanceTo13(self, globalCtx);
}

void DemoDu_UpdateCs_GR_13(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_CsGoronsRuby_UpdateFaceTextures(self, globalCtx);
    DemoDu_CsPlaySfx_LinkEscapeFromGorons(globalCtx);
}

void DemoDu_InitCs_AfterGanon(DemoDu* self, GlobalContext* globalCtx) {
    s32 pad[3];
    f32 lastFrame = Animation_GetLastFrame(&gDaruniaSageFormationAnim);

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gDaruniaSkel, NULL, NULL, NULL, 0);
    Animation_Change(&self->skelAnime, &gDaruniaSageFormationAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, 0.0f);
    self->updateIndex = CS_CHAMBERAFTERGANON_SUBSCENE(0);
    self->actor.shape.shadowAlpha = 0;
}

void DemoDu_CsPlaySfx_WhiteOut() {
    func_800788CC(NA_SE_SY_WHITE_OUT_T);
}

void DemoDu_CsAfterGanon_SpawnDemo6K(DemoDu* self, GlobalContext* globalCtx) {
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_6K, self->actor.world.pos.x,
                       kREG(16) + 22.0f + self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 3);
}

void DemoDu_CsAfterGanon_AdvanceTo01(DemoDu* self, GlobalContext* globalCtx) {
    if (DemoDu_IsNpcDoingThisAction(self, globalCtx, 4, 2)) {
        self->updateIndex = CS_CHAMBERAFTERGANON_SUBSCENE(1);
        self->drawIndex = 2;
        self->shadowAlpha = 0;
        self->actor.shape.shadowAlpha = 0;
        self->unk_1A4 = 0.0f;
        DemoDu_CsPlaySfx_WhiteOut();
    }
}

void DemoDu_CsAfterGanon_AdvanceTo02(DemoDu* self, GlobalContext* globalCtx) {
    f32* unk_1A4 = &self->unk_1A4;
    s32 shadowAlpha = 255;

    if (DemoDu_IsNpcDoingThisAction(self, globalCtx, 4, 2)) {
        *unk_1A4 += 1.0f;
        if (*unk_1A4 >= kREG(5) + 10.0f) {
            self->updateIndex = CS_CHAMBERAFTERGANON_SUBSCENE(2);
            self->drawIndex = 1;
            *unk_1A4 = kREG(5) + 10.0f;
            self->shadowAlpha = shadowAlpha;
            self->actor.shape.shadowAlpha = shadowAlpha;
            return;
        }
    } else {
        *unk_1A4 -= 1.0f;
        if (*unk_1A4 <= 0.0f) {
            self->updateIndex = CS_CHAMBERAFTERGANON_SUBSCENE(0);
            self->drawIndex = 0;
            *unk_1A4 = 0.0f;
            self->shadowAlpha = 0;
            self->actor.shape.shadowAlpha = 0;
            return;
        }
    }
    shadowAlpha = (*unk_1A4 / (kREG(5) + 10.0f)) * 255.0f;
    self->shadowAlpha = shadowAlpha;
    self->actor.shape.shadowAlpha = shadowAlpha;
}

void DemoDu_CsAfterGanon_BackTo01(DemoDu* self, GlobalContext* globalCtx) {
    if (DemoDu_IsNpcNotDoingThisAction(self, globalCtx, 4, 2)) {
        self->updateIndex = CS_CHAMBERAFTERGANON_SUBSCENE(1);
        self->drawIndex = 2;
        self->unk_1A4 = kREG(5) + 10.0f;
        self->shadowAlpha = 255;
        if (!self->demo6KSpawned) {
            DemoDu_CsAfterGanon_SpawnDemo6K(self, globalCtx);
            self->demo6KSpawned = 1;
        }
        self->actor.shape.shadowAlpha = 255;
    }
}

void DemoDu_UpdateCs_AG_00(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_CsAfterGanon_AdvanceTo01(self, globalCtx);
    DemoDu_CsAfterGanon_CheckIfShouldReset(self, globalCtx);
}

void DemoDu_UpdateCs_AG_01(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_UpdateEyes(self);
    DemoDu_CsAfterGanon_AdvanceTo02(self, globalCtx);
    DemoDu_CsAfterGanon_CheckIfShouldReset(self, globalCtx);
}

void DemoDu_UpdateCs_AG_02(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_UpdateEyes(self);
    DemoDu_CsAfterGanon_BackTo01(self, globalCtx);
    DemoDu_CsAfterGanon_CheckIfShouldReset(self, globalCtx);
}

// Similar to DemoDu_Draw_01, but self uses POLY_XLU_DISP. Also uses self->shadowAlpha for setting the env color.
void DemoDu_Draw_02(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    DemoDu* self = THIS;
    s16 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = sEyeTextures[eyeTexIndex];
    s32 pad;
    s16 mouthTexIndex = self->mouthTexIndex;
    void* mouthTexture = sMouthTextures[mouthTexIndex];
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_du_inKenjyanomaDemo02.c", 275);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTexture));
    gSPSegment(POLY_XLU_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(&gDaruniaNoseSeriousTex));

    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->shadowAlpha);

    gSPSegment(POLY_XLU_DISP++, 0x0C, &D_80116280[0]);

    POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, 0,
                                       0, 0, POLY_XLU_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_du_inKenjyanomaDemo02.c", 304);
}

void DemoDu_InitCs_Credits(DemoDu* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gDaruniaSkel, &gDaruniaCreditsIdleAnim, NULL, NULL, 0);
    self->updateIndex = CS_CREDITS_SUBSCENE(0);
    self->drawIndex = 0;
    self->actor.shape.shadowAlpha = 0;
    DemoDu_SetMouthTexIndex(self, 3);
}

void DemoDu_CsCredits_UpdateShadowAlpha(DemoDu* self) {
    s32 shadowAlpha = 255;
    f32 temp_f0;
    f32* unk_1A4;

    self->unk_1A4 += 1.0f;
    temp_f0 = kREG(17) + 10.0f;
    unk_1A4 = &self->unk_1A4;

    if (temp_f0 <= *unk_1A4) {
        self->shadowAlpha = shadowAlpha;
        self->actor.shape.shadowAlpha = shadowAlpha;
    } else {
        shadowAlpha = *unk_1A4 / temp_f0 * 255.0f;
        self->shadowAlpha = shadowAlpha;
        self->actor.shape.shadowAlpha = shadowAlpha;
    }
}

void DemoDu_CsCredits_AdvanceTo01(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_MoveToNpcPos(self, globalCtx, 2);
    self->updateIndex = CS_CREDITS_SUBSCENE(1);
    self->drawIndex = 2;
}

void DemoDu_CsCredits_AdvanceTo02(DemoDu* self) {
    if (self->unk_1A4 >= kREG(17) + 10.0f) {
        self->updateIndex = CS_CREDITS_SUBSCENE(2);
        self->drawIndex = 1;
    }
}

void DemoDu_CsCredits_AdvanceTo03(DemoDu* self) {
    func_80969DDC(self, &gDaruniaLookingUpToSariaAnim, ANIMMODE_ONCE, -8.0f, 0);
    self->updateIndex = CS_CREDITS_SUBSCENE(3);
}

void DemoDu_CsCredits_AdvanceTo04(DemoDu* self) {
    func_80969DDC(self, &gDaruniaCreditsHitBreastAnim, ANIMMODE_ONCE, 0.0f, 0);
    self->updateIndex = CS_CREDITS_SUBSCENE(4);
}

void DemoDu_CsCredits_BackTo02(DemoDu* self, s32 animFinished) {
    if (animFinished) {
        func_80969DDC(self, &gDaruniaCreditsIdleAnim, ANIMMODE_LOOP, 0.0f, 0);
        self->updateIndex = CS_CREDITS_SUBSCENE(2);
    }
}

void DemoDu_CsCredits_HandleSubscenesByNpcAction(DemoDu* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = DemoDu_GetNpcAction(globalCtx, 2);

    if (npcAction != NULL) {
        s32 action = npcAction->action;
        s32 lastAction = self->lastAction;

        if (action != lastAction) {
            switch (action) {
                case 9:
                    DemoDu_CsCredits_AdvanceTo01(self, globalCtx);
                    break;
                case 10:
                    DemoDu_CsCredits_AdvanceTo03(self);
                    break;
                case 11:
                    DemoDu_CsCredits_AdvanceTo04(self);
                    break;
                default:
                    // "Demo_Du_inEnding_Check_DemoMode:There is no such operation!!!!!!!!"
                    osSyncPrintf("Demo_Du_inEnding_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
                    break;
            }
            self->lastAction = action;
        }
    }
}

void DemoDu_UpdateCs_CR_00(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_CsCredits_HandleSubscenesByNpcAction(self, globalCtx);
}

void DemoDu_UpdateCs_CR_01(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_UpdateEyes(self);
    DemoDu_CsCredits_UpdateShadowAlpha(self);
    DemoDu_CsCredits_AdvanceTo02(self);
}

void DemoDu_UpdateCs_CR_02(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_UpdateEyes(self);
    DemoDu_CsCredits_HandleSubscenesByNpcAction(self, globalCtx);
}

void DemoDu_UpdateCs_CR_03(DemoDu* self, GlobalContext* globalCtx) {
    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    DemoDu_UpdateSkelAnime(self);
    DemoDu_UpdateEyes(self);
    DemoDu_CsCredits_HandleSubscenesByNpcAction(self, globalCtx);
}

void DemoDu_UpdateCs_CR_04(DemoDu* self, GlobalContext* globalCtx) {
    s32 animFinished;

    DemoDu_UpdateBgCheckInfo(self, globalCtx);
    animFinished = DemoDu_UpdateSkelAnime(self);
    DemoDu_UpdateEyes(self);
    DemoDu_CsCredits_BackTo02(self, animFinished);
}

static DemoDuActionFunc sUpdateFuncs[] = {
    DemoDu_UpdateCs_FM_00, DemoDu_UpdateCs_FM_01, DemoDu_UpdateCs_FM_02, DemoDu_UpdateCs_FM_03, DemoDu_UpdateCs_FM_04,
    DemoDu_UpdateCs_FM_05, DemoDu_UpdateCs_FM_06, DemoDu_UpdateCs_GR_00, DemoDu_UpdateCs_GR_01, DemoDu_UpdateCs_GR_02,
    DemoDu_UpdateCs_GR_03, DemoDu_UpdateCs_GR_04, DemoDu_UpdateCs_GR_05, DemoDu_UpdateCs_GR_06, DemoDu_UpdateCs_GR_07,
    DemoDu_UpdateCs_GR_08, DemoDu_UpdateCs_GR_09, DemoDu_UpdateCs_GR_10, DemoDu_UpdateCs_GR_11, DemoDu_UpdateCs_GR_12,
    DemoDu_UpdateCs_GR_13, DemoDu_UpdateCs_AG_00, DemoDu_UpdateCs_AG_01, DemoDu_UpdateCs_AG_02, DemoDu_UpdateCs_CR_00,
    DemoDu_UpdateCs_CR_01, DemoDu_UpdateCs_CR_02, DemoDu_UpdateCs_CR_03, DemoDu_UpdateCs_CR_04,
};

void DemoDu_Update(Actor* thisx, GlobalContext* globalCtx) {
    DemoDu* self = THIS;

    if (self->updateIndex < 0 || self->updateIndex >= 29 || sUpdateFuncs[self->updateIndex] == NULL) {
        // "The main mode is abnormal!!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sUpdateFuncs[self->updateIndex](self, globalCtx);
}

void DemoDu_Init(Actor* thisx, GlobalContext* globalCtx) {
    DemoDu* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    switch (self->actor.params) {
        case DEMO_DU_CS_GORONS_RUBY:
            DemoDu_InitCs_GoronsRuby(self, globalCtx);
            break;

        case DEMO_DU_CS_CHAMBER_AFTER_GANON:
            DemoDu_InitCs_AfterGanon(self, globalCtx);
            break;

        case DEMO_DU_CS_CREDITS:
            DemoDu_InitCs_Credits(self, globalCtx);
            break;

        default:
            DemoDu_InitCs_FireMedallion(self, globalCtx);
            break;
    }
}

void DemoDu_Draw_NoDraw(Actor* thisx, GlobalContext* globalCtx2) {
}

// Similar to DemoDu_Draw_02, but self uses POLY_OPA_DISP. Sets the env color to 255.
void DemoDu_Draw_01(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    DemoDu* self = THIS;
    s16 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = sEyeTextures[eyeTexIndex];
    s32 pad;
    s16 mouthTexIndex = self->mouthTexIndex;
    void* mouthTexture = sMouthTextures[mouthTexIndex];
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_du.c", 615);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTexture));
    gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(&gDaruniaNoseSeriousTex));

    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);

    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);

    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL, NULL,
                          self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_du.c", 638);
}

static DemoDuDrawFunc sDrawFuncs[] = {
    DemoDu_Draw_NoDraw,
    DemoDu_Draw_01,
    DemoDu_Draw_02,
};

void DemoDu_Draw(Actor* thisx, GlobalContext* globalCtx) {
    DemoDu* self = THIS;

    if (self->drawIndex < 0 || self->drawIndex >= 3 || sDrawFuncs[self->drawIndex] == NULL) {
        // "The drawing mode is abnormal!!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sDrawFuncs[self->drawIndex](thisx, globalCtx);
}

const ActorInit Demo_Du_InitVars = {
    ACTOR_DEMO_DU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DU,
    sizeof(DemoDu),
    (ActorFunc)DemoDu_Init,
    (ActorFunc)DemoDu_Destroy,
    (ActorFunc)DemoDu_Update,
    (ActorFunc)DemoDu_Draw,
};
