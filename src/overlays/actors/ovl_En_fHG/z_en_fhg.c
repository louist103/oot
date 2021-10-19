/*
 * File: z_en_fhg.c
 * Overlay: ovl_En_fHG
 * Description: Phantom Ganon's Horse
 */

#include "z_en_fhg.h"
#include "objects/object_fhg/object_fhg.h"
#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"
#include "overlays/actors/ovl_Boss_Ganondrof/z_boss_ganondrof.h"
#include "overlays/actors/ovl_En_Fhg_Fire/z_en_fhg_fire.h"

#define FLAGS 0x00000010

#define THIS ((EnfHG*)thisx)

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 yRot;
} EnfHGPainting; // size = 0x10;

typedef enum {
    /*  0 */ INTRO_WAIT,
    /*  1 */ INTRO_START,
    /*  2 */ INTRO_FENCE,
    /*  3 */ INTRO_BACK,
    /*  4 */ INTRO_REVEAL,
    /*  5 */ INTRO_CUT,
    /*  6 */ INTRO_LAUGH,
    /*  7 */ INTRO_TITLE,
    /*  8 */ INTRO_RETREAT,
    /*  9 */ INTRO_FINISH,
    /* 15 */ INTRO_READY = 15
} EnfHGIntroState;

void EnfHG_Init(Actor* thisx, GlobalContext* globalCtx);
void EnfHG_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnfHG_Update(Actor* thisx, GlobalContext* globalCtx);
void EnfHG_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnfHG_SetupIntro(EnfHG* self, GlobalContext* globalCtx);
void EnfHG_Intro(EnfHG* self, GlobalContext* globalCtx);
void EnfHG_SetupApproach(EnfHG* self, GlobalContext* globalCtx, s16 paintingIndex);
void EnfHG_Approach(EnfHG* self, GlobalContext* globalCtx);
void EnfHG_Attack(EnfHG* self, GlobalContext* globalCtx);
void EnfHG_Damage(EnfHG* self, GlobalContext* globalCtx);
void EnfHG_Retreat(EnfHG* self, GlobalContext* globalCtx);
void EnfHG_Done(EnfHG* self, GlobalContext* globalCtx);

void EnfHG_Noop(Actor* thisx, GlobalContext* globalCtx, PSkinAwb* skin);

const ActorInit En_fHG_InitVars = {
    ACTOR_EN_FHG,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_FHG,
    sizeof(EnfHG),
    (ActorFunc)EnfHG_Init,
    (ActorFunc)EnfHG_Destroy,
    (ActorFunc)EnfHG_Update,
    (ActorFunc)EnfHG_Draw,
};

static EnfHGPainting sPaintings[] = {
    { { 0.0f, 60.0f, -315.0f }, 0x0000 },   { { -260.0f, 60.0f, -145.0f }, 0x2AAA },
    { { -260.0f, 60.0f, 165.0f }, 0x5554 }, { { 0.0f, 60.0f, 315.0f }, 0x7FFE },
    { { 260.0f, 60.0f, 155.0f }, 0xAAA8 },  { { 260.0f, 60.0f, -155.0f }, 0xD552 },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x1A, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_STOP),
};

void EnfHG_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnfHG* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Flags_SetSwitch(globalCtx, 0x14);
    Actor_SetScale(&self->actor, 0.011499999f);
    self->actor.gravity = -3.5f;
    ActorShape_Init(&self->actor.shape, -2600.0f, NULL, 20.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    func_800A663C(globalCtx, &self->skin, &gPhantomHorseSkel, &gPhantomHorseRunningAnim);

    if (self->actor.params >= GND_FAKE_BOSS) {
        EnfHG_SetupApproach(self, globalCtx, self->actor.params - GND_FAKE_BOSS);
    } else {
        EnfHG_SetupIntro(self, globalCtx);
    }
}

void EnfHG_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnfHG* self = THIS;

    osSyncPrintf("F DT1\n");
    func_800A6888(globalCtx, &self->skin);
    osSyncPrintf("F DT2\n");
}

void EnfHG_SetupIntro(EnfHG* self, GlobalContext* globalCtx) {
    Animation_PlayLoop(&self->skin.skelAnime, &gPhantomHorseIdleAnim);
    self->actionFunc = EnfHG_Intro;
    self->actor.world.pos.x = GND_BOSSROOM_CENTER_X;
    self->actor.world.pos.y = GND_BOSSROOM_CENTER_Y - 267.0f;
    self->actor.world.pos.z = GND_BOSSROOM_CENTER_Z;
}

void EnfHG_Intro(EnfHG* self, GlobalContext* globalCtx) {
    static Vec3f audioVec = { 0.0f, 0.0f, 50.0f };
    s32 pad64;
    Player* player = GET_PLAYER(globalCtx);
    BossGanondrof* bossGnd = (BossGanondrof*)self->actor.parent;
    s32 pad58;
    s32 pad54;

    if (self->cutsceneState != INTRO_FINISH) {
        SkelAnime_Update(&self->skin.skelAnime);
    }
    switch (self->cutsceneState) {
        case INTRO_WAIT:
            if ((fabsf(player->actor.world.pos.x - (GND_BOSSROOM_CENTER_X + 0.0f)) < 150.0f) &&
                (fabsf(player->actor.world.pos.z - (GND_BOSSROOM_CENTER_Z + 0.0f)) < 150.0f)) {
                self->cutsceneState = INTRO_READY;
            }
            break;
        case INTRO_READY:
            if ((fabsf(player->actor.world.pos.x - (GND_BOSSROOM_CENTER_X + 0.0f)) < 100.0f) &&
                (fabsf(player->actor.world.pos.z - (GND_BOSSROOM_CENTER_Z + 315.0f)) < 100.0f)) {
                self->cutsceneState = INTRO_START;
                if (gSaveContext.eventChkInf[7] & 4) {
                    self->timers[0] = 57;
                }
            }
            break;
        case INTRO_START:
            if (gSaveContext.eventChkInf[7] & 4) {
                if (self->timers[0] == 55) {
                    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_SHUTTER,
                                       GND_BOSSROOM_CENTER_X + 0.0f, GND_BOSSROOM_CENTER_Y - 97.0f,
                                       GND_BOSSROOM_CENTER_Z + 308.0f, 0, 0, 0, (SHUTTER_PG_BARS << 6));
                }
                if (self->timers[0] == 51) {
                    Audio_PlayActorSound2(self->actor.child, NA_SE_EV_SPEAR_FENCE);
                    Audio_QueueSeqCmd(0x1B);
                }
                if (self->timers[0] == 0) {
                    EnfHG_SetupApproach(self, globalCtx, Rand_ZeroOne() * 5.99f);
                    self->bossGndSignal = FHG_START_FIGHT;
                }
                break;
            }
            func_80064520(globalCtx, &globalCtx->csCtx);
            func_8002DF54(globalCtx, &self->actor, 8);
            self->cutsceneCamera = Gameplay_CreateSubCamera(globalCtx);
            Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
            Gameplay_ChangeCameraStatus(globalCtx, self->cutsceneCamera, CAM_STAT_ACTIVE);
            self->cutsceneState = INTRO_FENCE;
            self->timers[0] = 60;
            self->actor.world.pos.y = GND_BOSSROOM_CENTER_Y - 7.0f;
            Audio_QueueSeqCmd(0x100100FF);
            gSaveContext.eventChkInf[7] |= 4;
            Flags_SetSwitch(globalCtx, 0x23);
        case INTRO_FENCE:
            player->actor.world.pos.x = GND_BOSSROOM_CENTER_X + 0.0f;
            player->actor.world.pos.y = GND_BOSSROOM_CENTER_Y + 7.0f;
            player->actor.world.pos.z = GND_BOSSROOM_CENTER_Z + 155.0f;
            player->actor.world.rot.y = player->actor.shape.rot.y = 0;
            player->actor.speedXZ = 0.0f;
            self->cameraEye.x = GND_BOSSROOM_CENTER_X + 0.0f;
            self->cameraEye.y = GND_BOSSROOM_CENTER_Y + 37.0f;
            self->cameraEye.z = GND_BOSSROOM_CENTER_Z + 170.0f;
            self->cameraAt.x = GND_BOSSROOM_CENTER_X + 0.0f;
            self->cameraAt.y = GND_BOSSROOM_CENTER_Y + 47.0f;
            self->cameraAt.z = GND_BOSSROOM_CENTER_Z + 315.0f;
            if (self->timers[0] == 25) {
                Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_SHUTTER,
                                   GND_BOSSROOM_CENTER_X + 0.0f, GND_BOSSROOM_CENTER_Y - 97.0f,
                                   GND_BOSSROOM_CENTER_Z + 308.0f, 0, 0, 0, (SHUTTER_PG_BARS << 6));
            }
            if (self->timers[0] == 21) {
                Audio_PlayActorSound2(self->actor.child, NA_SE_EV_SPEAR_FENCE);
            }
            if (self->timers[0] == 0) {
                self->cutsceneState = INTRO_BACK;
                self->timers[0] = 80;
            }
            break;
        case INTRO_BACK:
            if (self->timers[0] == 25) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_GANON_HORSE_GROAN);
            }
            if (self->timers[0] == 20) {
                func_8002DF54(globalCtx, &self->actor, 9);
            }
            if (self->timers[0] == 1) {
                Audio_QueueSeqCmd(0x23);
            }
            Math_ApproachF(&self->cameraEye.x, GND_BOSSROOM_CENTER_X + 40.0f, 0.05f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraEye.y, GND_BOSSROOM_CENTER_Y + 37.0f, 0.05f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraEye.z, GND_BOSSROOM_CENTER_Z + 80.0f, 0.05f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraAt.x, GND_BOSSROOM_CENTER_X - 100.0f, 0.05f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraAt.y, GND_BOSSROOM_CENTER_Y + 47.0f, 0.05f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraAt.z, GND_BOSSROOM_CENTER_Z + 335.0f, 0.05f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraSpeedMod, 1.0f, 1.0f, 0.01f);
            if (self->timers[0] == 0) {
                self->cutsceneState = INTRO_REVEAL;
                self->timers[0] = 50;
                self->cameraSpeedMod = 0.0f;
            }
            break;
        case INTRO_REVEAL:
            Math_ApproachF(&self->cameraEye.x, GND_BOSSROOM_CENTER_X + 70.0f, 0.1f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraEye.y, GND_BOSSROOM_CENTER_Y + 7.0f, 0.1f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraEye.z, GND_BOSSROOM_CENTER_Z + 200.0f, 0.1f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraAt.x, GND_BOSSROOM_CENTER_X - 150.0f, 0.1f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraAt.y, GND_BOSSROOM_CENTER_Y + 107.0f, 0.1f, self->cameraSpeedMod * 20.0f);
            Math_ApproachF(&self->cameraAt.z, GND_BOSSROOM_CENTER_Z - 65.0f, 0.1f, self->cameraSpeedMod * 40.0f);
            Math_ApproachF(&self->cameraSpeedMod, 1.0f, 1.0f, 0.05f);
            if (self->timers[0] == 5) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_HORSE_SANDDUST);
            }
            if (self->timers[0] == 0) {
                self->cutsceneState = INTRO_CUT;
                self->timers[0] = 50;
                self->cameraSpeedMod = 0.0f;
            }
            break;
        case INTRO_CUT:
            self->cutsceneState = INTRO_LAUGH;
            self->cameraEye.x = GND_BOSSROOM_CENTER_X + 50.0f;
            self->cameraEye.y = GND_BOSSROOM_CENTER_Y + 17.0f;
            self->cameraEye.z = GND_BOSSROOM_CENTER_Z + 110.0f;
            self->cameraAt.x = GND_BOSSROOM_CENTER_X - 150.0f;
            self->cameraAt.y = GND_BOSSROOM_CENTER_Y + 207.0f;
            self->cameraAt.z = GND_BOSSROOM_CENTER_Z - 155.0f;
            self->cameraEyeVel.x = fabsf(self->cameraEye.x - (GND_BOSSROOM_CENTER_X + 20.0f));
            self->cameraEyeVel.y = fabsf(self->cameraEye.y - (GND_BOSSROOM_CENTER_Y + 102.0f));
            self->cameraEyeVel.z = fabsf(self->cameraEye.z - (GND_BOSSROOM_CENTER_Z + 25.0f));
            self->cameraAtVel.x = fabsf(self->cameraAt.x - (GND_BOSSROOM_CENTER_X - 150.0f));
            self->cameraAtVel.y = fabsf(self->cameraAt.y - (GND_BOSSROOM_CENTER_Y + 197.0f));
            self->cameraAtVel.z = fabsf(self->cameraAt.z - (GND_BOSSROOM_CENTER_Z - 65.0f));
            self->timers[0] = 250;
        case INTRO_LAUGH:
            Math_ApproachF(&self->cameraEye.x, GND_BOSSROOM_CENTER_X + 20.0f, 0.05f,
                           self->cameraSpeedMod * self->cameraEyeVel.x);
            Math_ApproachF(&self->cameraEye.y, GND_BOSSROOM_CENTER_Y + 102.0f, 0.05f,
                           self->cameraSpeedMod * self->cameraEyeVel.y);
            Math_ApproachF(&self->cameraEye.z, GND_BOSSROOM_CENTER_Z + 25.0f, 0.05f,
                           self->cameraSpeedMod * self->cameraEyeVel.z);
            Math_ApproachF(&self->cameraAt.x, GND_BOSSROOM_CENTER_X - 150.0f, 0.05f,
                           self->cameraSpeedMod * self->cameraAtVel.x);
            Math_ApproachF(&self->cameraAt.y, GND_BOSSROOM_CENTER_Y + 197.0f, 0.05f,
                           self->cameraSpeedMod * self->cameraAtVel.y);
            Math_ApproachF(&self->cameraAt.z, GND_BOSSROOM_CENTER_Z - 65.0f, 0.05f,
                           self->cameraSpeedMod * self->cameraAtVel.z);
            Math_ApproachF(&self->cameraSpeedMod, 0.01f, 1.0f, 0.001f);
            if ((self->timers[0] == 245) || (self->timers[0] == 3)) {
                Animation_MorphToPlayOnce(&self->skin.skelAnime, &gPhantomHorseRearingAnim, -8.0f);
                self->bossGndSignal = FHG_REAR;
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_GANON_HORSE_NEIGH);
                if (self->timers[0] == 3) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_VOICE);
                }
            }
            if (self->timers[0] == 192) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_HORSE_SANDDUST);
            }
            if (self->timers[0] == 212) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_HORSE_LAND2);
                Animation_Change(&self->skin.skelAnime, &gPhantomHorseIdleAnim, 0.3f, 0.0f, 5.0f, ANIMMODE_LOOP_INTERP,
                                 -10.0f);
            }
            if (self->timers[0] == 90) {
                globalCtx->envCtx.unk_BF = 2;
                globalCtx->envCtx.unk_D6 = 0x14;
            }
            if (self->timers[0] == 100) {
                self->bossGndSignal = FHG_LIGHTNING;
            }
            if (self->timers[0] == 60) {
                self->bossGndSignal = FHG_RIDE;
            }
            if (self->timers[0] == 130) {
                Audio_QueueSeqCmd(0x105000FF);
            }
            if (self->timers[0] == 30) {
                bossGnd->work[GND_EYE_STATE] = GND_EYESTATE_BRIGHTEN;
            }
            if (self->timers[0] == 35) {
                func_80078914(&audioVec, NA_SE_EN_FANTOM_EYE);
            }
            if (self->timers[0] == 130) {
                bossGnd->work[GND_EYE_STATE] = GND_EYESTATE_FADE;
                func_80078914(&audioVec, NA_SE_EN_FANTOM_ST_LAUGH);
            }
            if (self->timers[0] == 20) {
                Audio_QueueSeqCmd(0x1B);
            }
            if (self->timers[0] == 2) {
                self->cameraSpeedMod = 0.0f;
                self->cutsceneState = INTRO_TITLE;
                self->cameraEyeVel.x = fabsf(self->cameraEye.x - (GND_BOSSROOM_CENTER_X + 180.0f));
                self->cameraEyeVel.y = fabsf(self->cameraEye.y - (GND_BOSSROOM_CENTER_Y + 7.0f));
                self->cameraEyeVel.z = fabsf(self->cameraEye.z - (GND_BOSSROOM_CENTER_Z + 140.0f));
                self->timers[0] = 100;
                self->timers[1] = 34;
                self->cameraAtVel.x = fabsf(self->cameraAt.x - self->actor.world.pos.x);
                self->cameraAtVel.y = fabsf(self->cameraAt.y - ((self->actor.world.pos.y + 70.0f) - 20.0f));
                self->cameraAtVel.z = fabsf(self->cameraAt.z - self->actor.world.pos.z);
            }
            break;
        case INTRO_TITLE:
            if (self->timers[1] == 1) {
                Animation_Change(&self->skin.skelAnime, &gPhantomHorseIdleAnim, 0.5f, 0.0f,
                                 Animation_GetLastFrame(&gPhantomHorseIdleAnim), ANIMMODE_LOOP_INTERP, -3.0f);
            }
            Math_ApproachF(&self->cameraEye.x, GND_BOSSROOM_CENTER_X + 180.0f, 0.1f,
                           self->cameraSpeedMod * self->cameraEyeVel.x);
            Math_ApproachF(&self->cameraEye.y, GND_BOSSROOM_CENTER_Y + 7.0f, 0.1f,
                           self->cameraSpeedMod * self->cameraEyeVel.y);
            Math_ApproachF(&self->cameraEye.z, self->cameraPanZ + (GND_BOSSROOM_CENTER_Z + 140.0f), 0.1f,
                           self->cameraSpeedMod * self->cameraEyeVel.z);
            Math_ApproachF(&self->cameraPanZ, -100.0f, 0.1f, 1.0f);
            Math_ApproachF(&self->cameraAt.x, self->actor.world.pos.x, 0.1f, self->cameraSpeedMod * 10.0f);
            Math_ApproachF(&self->cameraAt.y, (self->actor.world.pos.y + 70.0f) - 20.0f, 0.1f,
                           self->cameraSpeedMod * 10.0f);
            Math_ApproachF(&self->cameraAt.z, self->actor.world.pos.z, 0.1f, self->cameraSpeedMod * 10.0f);
            Math_ApproachF(&self->actor.world.pos.y, 60.0f, 0.1f, 2.0f);
            self->actor.world.pos.y += 2.0f * Math_SinS(self->gallopTimer * 0x5DC);
            Math_ApproachF(&self->cameraSpeedMod, 1.0f, 1.0f, 0.05f);
            if (self->timers[0] == 75) {
                TitleCard_InitBossName(globalCtx, &globalCtx->actorCtx.titleCtx,
                                       SEGMENTED_TO_VIRTUAL(&gPhantomGanonTitleCardTex), 160, 180, 128, 40);
            }
            if (self->timers[0] == 0) {
                self->cutsceneState = INTRO_RETREAT;
                self->timers[0] = 200;
                self->timers[1] = 23;
                self->cameraSpeedMod = 0.0f;
                Animation_Change(&self->skin.skelAnime, &gPhantomHorseLeapAnim, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&gPhantomHorseLeapAnim), ANIMMODE_ONCE_INTERP, -4.0f);
                self->bossGndSignal = FHG_SPUR;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_VOICE);
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_GANON_HORSE_NEIGH);
            }
            break;
        case INTRO_RETREAT:
            if (self->timers[1] == 1) {
                Animation_Change(&self->skin.skelAnime, &gPhantomHorseLandAnim, 0.5f, 0.0f,
                                 Animation_GetLastFrame(&gPhantomHorseLandAnim), ANIMMODE_ONCE_INTERP, -3.0f);
                self->bossGndSignal = FHG_FINISH;
            }
            if (self->timers[0] == 170) {
                func_8002DF54(globalCtx, &self->actor, 8);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_MASIC2);
            }
            Math_ApproachF(&self->cameraEye.z, self->cameraPanZ + (GND_BOSSROOM_CENTER_Z + 100.0f), 0.1f,
                           self->cameraSpeedMod * 1.5f);
            Math_ApproachF(&self->cameraPanZ, -100.0f, 0.1f, 1.0f);
            Math_ApproachF(&self->actor.world.pos.z, GND_BOSSROOM_CENTER_Z + 400.0f - 0.5f, 1.0f,
                           self->cameraSpeedMod * 10.0f);
            Math_ApproachF(&self->cameraSpeedMod, 1.0f, 1.0f, 0.05f);
            if ((fabsf(self->actor.world.pos.z - (GND_BOSSROOM_CENTER_Z + 400.0f - 0.5f)) < 300.0f) &&
                !self->spawnedWarp) {
                self->spawnedWarp = true;
                Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE,
                                   GND_BOSSROOM_CENTER_X + 0.0f, self->actor.world.pos.y + 50.0f,
                                   GND_BOSSROOM_CENTER_Z + 400.0f - 0.5f, 0, self->actor.shape.rot.y, 0,
                                   FHGFIRE_WARP_RETREAT);
                self->fhgFireKillWarp = true;
            }
            Math_ApproachF(&self->cameraAt.x, self->actor.world.pos.x, 0.2f, 50.0f);
            Math_ApproachF(&self->cameraAt.z, self->actor.world.pos.z, 0.2f, 50.0f);
            osSyncPrintf("TIME %d-------------------------------------------------\n", self->timers[0]);
            if (fabsf(self->actor.world.pos.z - (GND_BOSSROOM_CENTER_Z + 400.0f - 0.5f)) < 1.0f) {
                globalCtx->envCtx.unk_BF = 0;
                globalCtx->envCtx.unk_D6 = 0x14;
                self->cutsceneState = INTRO_FINISH;
                Animation_MorphToLoop(&self->skin.skelAnime, &gPhantomHorseRunningAnim, -3.0f);
                self->bossGndSignal = FHG_START_FIGHT;
                self->timers[1] = 75;
                self->timers[0] = 140;
            }
            break;
        case INTRO_FINISH:
            EnfHG_Retreat(self, globalCtx);
            Math_ApproachF(&self->cameraEye.z, self->cameraPanZ + (GND_BOSSROOM_CENTER_Z + 100.0f), 0.1f,
                           self->cameraSpeedMod * 1.5f);
            Math_ApproachF(&self->cameraPanZ, -100.0f, 0.1f, 1.0f);
            Math_ApproachF(&self->cameraAt.y, (self->actor.world.pos.y + 70.0f) - 20.0f, 0.1f,
                           self->cameraSpeedMod * 10.0f);
            if (self->timers[1] == 0) {
                Camera* camera = Gameplay_GetCamera(globalCtx, 0);

                camera->eye = self->cameraEye;
                camera->eyeNext = self->cameraEye;
                camera->at = self->cameraAt;
                func_800C08AC(globalCtx, self->cutsceneCamera, 0);
                self->cutsceneCamera = 0;
                func_80064534(globalCtx, &globalCtx->csCtx);
                func_8002DF54(globalCtx, &self->actor, 7);
                self->actionFunc = EnfHG_Retreat;
            }
            break;
    }
    if (self->cutsceneCamera != 0) {
        Gameplay_CameraSetAtEye(globalCtx, self->cutsceneCamera, &self->cameraAt, &self->cameraEye);
    }
}

void EnfHG_SetupApproach(EnfHG* self, GlobalContext* globalCtx, s16 paintingIndex) {
    s16 oppositeIndex[6] = { 3, 4, 5, 0, 1, 2 };

    Animation_MorphToLoop(&self->skin.skelAnime, &gPhantomHorseRunningAnim, 0.0f);
    self->actionFunc = EnfHG_Approach;
    self->curPainting = paintingIndex;
    self->targetPainting = oppositeIndex[self->curPainting];

    osSyncPrintf("KABE NO 1 = %d\n", self->curPainting);
    osSyncPrintf("KABE NO 2 = %d\n", self->targetPainting);

    self->actor.world.pos.x = (1.3f * sPaintings[self->curPainting].pos.x) + (GND_BOSSROOM_CENTER_X - 4.0f);
    self->actor.world.pos.y = sPaintings[self->curPainting].pos.y + (GND_BOSSROOM_CENTER_Y + 153.0f);
    self->actor.world.pos.z = (1.3f * sPaintings[self->curPainting].pos.z) - -(GND_BOSSROOM_CENTER_Z - 10.0f);
    self->actor.world.rot.y = sPaintings[self->curPainting].yRot;

    osSyncPrintf("XP1  = %f\n", self->actor.world.pos.x);
    osSyncPrintf("ZP1  = %f\n", self->actor.world.pos.z);

    self->inPaintingPos.x = (sPaintings[self->targetPainting].pos.x * 1.3f) + (GND_BOSSROOM_CENTER_X - 4.0f);
    self->inPaintingPos.y = sPaintings[self->targetPainting].pos.y + (GND_BOSSROOM_CENTER_Y + 33.0f);
    self->inPaintingPos.z = (sPaintings[self->targetPainting].pos.z * 1.3f) - -(GND_BOSSROOM_CENTER_Z - 10.0f);
    self->inPaintingVelX = (fabsf(self->inPaintingPos.x - self->actor.world.pos.x) * 2) * 0.01f;

    if (self->inPaintingVelX < 1.0f) {
        self->inPaintingVelX = 1.0f;
    }
    self->inPaintingVelZ = (fabsf(self->inPaintingPos.z - self->actor.world.pos.z) * 2) * 0.01f;
    if (self->inPaintingVelZ < 1.0f) {
        self->inPaintingVelZ = 1.0f;
    }

    self->timers[0] = 100;
    self->actor.scale.x = 0.002f;
    self->actor.scale.y = 0.002f;
    self->actor.scale.z = 0.001f;
    self->approachRate = 0.0f;

    self->warpColorFilterR = globalCtx->lightCtx.fogColor[0];
    self->warpColorFilterG = globalCtx->lightCtx.fogColor[1];
    self->warpColorFilterB = globalCtx->lightCtx.fogColor[2];
    self->warpColorFilterUnk1 = 0.0f;
    self->warpColorFilterUnk2 = 0.0f;
    self->turnRot = 0;
    self->turnTarget = 0;
    self->spawnedWarp = false;
}

void EnfHG_Approach(EnfHG* self, GlobalContext* globalCtx) {
    osSyncPrintf("STANDBY !!\n");
    osSyncPrintf("XP2  = %f\n", self->actor.world.pos.x);
    osSyncPrintf("ZP2  = %f\n", self->actor.world.pos.z);
    if (self->actor.params == GND_REAL_BOSS) {
        self->hoofSfxPos.x = self->actor.projectedPos.x / (self->actor.scale.x * 100.0f);
        self->hoofSfxPos.y = self->actor.projectedPos.y / (self->actor.scale.x * 100.0f);
        self->hoofSfxPos.z = self->actor.projectedPos.z / (self->actor.scale.x * 100.0f);
        if ((self->gallopTimer % 8) == 0) {
            func_80078914(&self->hoofSfxPos, NA_SE_EV_HORSE_RUN);
        }
    }
    SkelAnime_Update(&self->skin.skelAnime);
    Math_ApproachF(&self->actor.scale.x, 0.011499999f, 1.0f, self->approachRate);
    Math_ApproachF(&self->approachRate, 0.0002f, 1.0f, 0.0000015f);
    Math_ApproachF(&self->actor.world.pos.y, 60.0f, 0.1f, 1.0f);
    self->actor.scale.y = self->actor.scale.x;
    if (self->timers[0] == 0) {
        osSyncPrintf("arg_data ------------------------------------>%d\n", self->actor.params);
        if (self->actor.params != GND_REAL_BOSS) {
            self->timers[0] = 140;
            self->actionFunc = EnfHG_Retreat;
            Animation_MorphToLoop(&self->skin.skelAnime, &gPhantomHorseRunningAnim, 0.0f);
            self->turnTarget = -0x8000;
        } else {
            self->actionFunc = EnfHG_Attack;
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_GANON_HORSE_NEIGH);
            self->timers[0] = 40;
            Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE,
                               self->actor.world.pos.x, self->actor.world.pos.y + 50.0f, self->actor.world.pos.z, 0,
                               self->actor.shape.rot.y + 0x8000, 0, FHGFIRE_WARP_EMERGE);
            self->fhgFireKillWarp = false;
        }
    }
}

void EnfHG_Attack(EnfHG* self, GlobalContext* globalCtx) {
    osSyncPrintf("KABE OUT !!\n");
    self->bossGndInPainting = false;
    SkelAnime_Update(&self->skin.skelAnime);
    if (self->timers[0] != 0) {
        Math_ApproachF(&self->actor.scale.z, 0.011499999f, 1.0f, 0.0002f);
        if (self->timers[0] == 1) {
            self->bossGndSignal = FHG_RAISE_SPEAR;
            self->timers[1] = 50;
            Animation_MorphToPlayOnce(&self->skin.skelAnime, &gPhantomHorseLeapAnim, 0.0f);
        }
        Math_ApproachF(&self->warpColorFilterR, 255.0f, 1.0f, 10.0f);
        Math_ApproachF(&self->warpColorFilterG, 255.0f, 1.0f, 10.0f);
        Math_ApproachF(&self->warpColorFilterB, 255.0f, 1.0f, 10.0f);
        Math_ApproachF(&self->warpColorFilterUnk1, -60.0f, 1.0f, 5.0f);
    } else {
        Math_ApproachF(&self->warpColorFilterR, globalCtx->lightCtx.fogColor[0], 1.0f, 10.0f);
        Math_ApproachF(&self->warpColorFilterG, globalCtx->lightCtx.fogColor[0], 1.0f, 10.0f);
        Math_ApproachF(&self->warpColorFilterB, globalCtx->lightCtx.fogColor[0], 1.0f, 10.0f);
        Math_ApproachF(&self->warpColorFilterUnk1, 0.0f, 1.0f, 5.0f);
        if (self->timers[1] == 29) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_MASIC2);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_VOICE);
        }
        if (self->hitTimer == 0) {
            if (self->timers[1] == 24) {
                Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE,
                                   self->actor.world.pos.x, (self->actor.world.pos.y + 100.0f) + 25.0f,
                                   self->actor.world.pos.z, 0, 0, 0, FHGFIRE_LIGHTNING_STRIKE);
            }
            if (self->timers[1] == 45) {
                Animation_MorphToLoop(&self->skin.skelAnime, &gPhantomHorseAirAnim, 0.0f);
            }
            if (self->timers[1] == 38) {
                self->bossGndSignal = FHG_LIGHTNING;
            }
            if (self->timers[1] == 16) {
                Animation_MorphToPlayOnce(&self->skin.skelAnime, &gPhantomHorseLandAnim, 0.0f);
                self->bossGndSignal = FHG_RESET;
            }
        }
        Math_ApproachF(&self->actor.scale.z, 0.011499999f, 1.0f, 0.002f);
        Math_ApproachF(&self->actor.world.pos.x, self->inPaintingPos.x, 1.0f, self->inPaintingVelX);
        Math_ApproachF(&self->actor.world.pos.y, 60.0f, 0.1f, 1.0f);
        Math_ApproachF(&self->actor.world.pos.z, self->inPaintingPos.z, 1.0f, self->inPaintingVelZ);
    }
    if (self->hitTimer == 20) {
        self->actionFunc = EnfHG_Damage;
        self->spawnedWarp = false;
        Animation_Change(&self->skin.skelAnime, &gPhantomHorseLandAnim, -1.0f, 0.0f,
                         Animation_GetLastFrame(&gPhantomHorseLandAnim), ANIMMODE_ONCE, -5.0f);
        self->timers[0] = 10;
        self->bossGndSignal = FHG_RESET;
        self->damageSpeedMod = 1.0f;
    } else {
        f32 dx = self->actor.world.pos.x - self->inPaintingPos.x;
        f32 dz = self->actor.world.pos.z - self->inPaintingPos.z;
        f32 dxz = sqrtf(SQ(dx) + SQ(dz));

        if (dxz < 350.0f) {
            self->bossGndInPainting = true;
        }
        if ((dxz < 300.0f) && !self->spawnedWarp) {
            self->spawnedWarp = true;
            Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE, self->inPaintingPos.x,
                               self->actor.world.pos.y + 50.0f, self->inPaintingPos.z, 0, self->actor.shape.rot.y, 0,
                               FHGFIRE_WARP_RETREAT);
            self->fhgFireKillWarp = true;
        }
        osSyncPrintf("SPD X %f\n", self->inPaintingVelX);
        osSyncPrintf("SPD Z %f\n", self->inPaintingVelZ);
        osSyncPrintf("X=%f\n", dx);
        osSyncPrintf("Z=%f\n", dz);
        if (dxz == 0.0f) {
            self->timers[0] = 140;
            self->actionFunc = EnfHG_Retreat;
            Animation_MorphToLoop(&self->skin.skelAnime, &gPhantomHorseRunningAnim, 0.0f);
            self->bossGndSignal = FHG_RIDE;
        }
    }
}

void EnfHG_Damage(EnfHG* self, GlobalContext* globalCtx) {
    f32 dx;
    f32 dz;
    f32 dxz2;

    osSyncPrintf("REVISE !!\n");
    SkelAnime_Update(&self->skin.skelAnime);
    Math_ApproachF(&self->warpColorFilterR, globalCtx->lightCtx.fogColor[0], 1.0f, 10.0f);
    Math_ApproachF(&self->warpColorFilterG, globalCtx->lightCtx.fogColor[0], 1.0f, 10.0f);
    Math_ApproachF(&self->warpColorFilterB, globalCtx->lightCtx.fogColor[0], 1.0f, 10.0f);
    Math_ApproachF(&self->warpColorFilterUnk1, 0.0f, 1.0f, 5.0f);
    Math_ApproachF(&self->actor.scale.z, 0.011499999f, 1.0f, 0.002f);
    if (self->timers[0] != 0) {
        Math_ApproachZeroF(&self->damageSpeedMod, 1.0f, 0.1f);
        if (self->timers[0] == 1) {
            self->targetPainting = self->curPainting;
            self->inPaintingPos.x = (sPaintings[self->targetPainting].pos.x * 1.3f) + (GND_BOSSROOM_CENTER_X - 4.0f);
            self->inPaintingPos.y = sPaintings[self->targetPainting].pos.y;
            self->inPaintingPos.z = (sPaintings[self->targetPainting].pos.z * 1.3f) - -(GND_BOSSROOM_CENTER_Z - 10.0f);
        }
    } else {
        Math_ApproachF(&self->damageSpeedMod, 1.0f, 1.0f, 0.1f);
    }
    Math_ApproachF(&self->actor.world.pos.x, self->inPaintingPos.x, 1.0f, self->damageSpeedMod * self->inPaintingVelX);
    Math_ApproachF(&self->actor.world.pos.y, 60.0f, 0.1f, 1.0f);
    Math_ApproachF(&self->actor.world.pos.z, self->inPaintingPos.z, 1.0f, self->damageSpeedMod * self->inPaintingVelZ);
    dx = self->actor.world.pos.x - self->inPaintingPos.x;
    dz = self->actor.world.pos.z - self->inPaintingPos.z;
    dxz2 = sqrtf(SQ(dx) + SQ(dz));
    if ((dxz2 < 300.0f) && (!self->spawnedWarp)) {
        self->spawnedWarp = true;
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FHG_FIRE, self->inPaintingPos.x,
                           self->actor.world.pos.y + 50.0f, self->inPaintingPos.z, 0, self->actor.shape.rot.y + 0x8000,
                           0, FHGFIRE_WARP_RETREAT);
    }
    if (dxz2 == 0.0f) {
        BossGanondrof* bossGnd = (BossGanondrof*)self->actor.parent;

        self->timers[0] = 140;
        self->actionFunc = EnfHG_Retreat;
        Animation_MorphToLoop(&self->skin.skelAnime, &gPhantomHorseRunningAnim, 0.0f);
        if (bossGnd->actor.colChkInfo.health > 24) {
            self->bossGndSignal = FHG_RIDE;
        } else {
            bossGnd->flyMode = GND_FLY_NEUTRAL;
        }
        self->turnTarget = -0x8000;
    }
}

void EnfHG_Retreat(EnfHG* self, GlobalContext* globalCtx) {
    osSyncPrintf("KABE IN !!\n");
    if (self->turnTarget != 0) {
        Math_ApproachS(&self->turnRot, self->turnTarget, 5, 2000);
    }
    if (self->actor.params == GND_REAL_BOSS) {
        self->hoofSfxPos.x = self->actor.projectedPos.x / (self->actor.scale.x * 100.0f);
        self->hoofSfxPos.y = self->actor.projectedPos.y / (self->actor.scale.x * 100.0f);
        self->hoofSfxPos.z = self->actor.projectedPos.z / (self->actor.scale.x * 100.0f);
        if ((self->gallopTimer % 8) == 0) {
            func_80078914(&self->hoofSfxPos, NA_SE_EV_HORSE_RUN);
        }
    }
    SkelAnime_Update(&self->skin.skelAnime);
    Math_ApproachF(&self->actor.scale.z, 0.001f, 1.0f, 0.001f);
    Math_ApproachF(&self->actor.scale.x, 0.002f, 0.05f, 0.0001f);
    Math_ApproachF(&self->actor.world.pos.y, 200.0f, 0.05f, 1.0f);
    self->actor.scale.y = self->actor.scale.x;
    if ((self->timers[0] == 80) && (self->actor.params == GND_REAL_BOSS)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_LAUGH);
    }
    if (self->timers[0] == 0) {
        BossGanondrof* bossGnd = (BossGanondrof*)self->actor.parent;
        s16 paintingIdxReal;
        s16 paintingIdxFake;

        if (self->actor.params != GND_REAL_BOSS) {
            self->killActor = true;
            bossGnd->killActor = true;
        } else if (bossGnd->flyMode != GND_FLY_PAINTING) {
            self->actionFunc = EnfHG_Done;
            self->actor.draw = NULL;
        } else {
            paintingIdxReal = Rand_ZeroOne() * 5.99f;
            EnfHG_SetupApproach(self, globalCtx, paintingIdxReal);
            do {
                paintingIdxFake = Rand_ZeroOne() * 5.99f;
            } while (paintingIdxFake == paintingIdxReal);
            osSyncPrintf("ac1 = %x `````````````````````````````````````````````````\n",
                         Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_BOSS_GANONDROF,
                                            self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                            0, 0, 0, paintingIdxFake + GND_FAKE_BOSS));
        }
    }
}

void EnfHG_Done(EnfHG* self, GlobalContext* globalCtx) {
    self->bossGndInPainting = false;
}

void EnfHG_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnfHG* self = THIS;
    u8 i;

    if (self->killActor) {
        Actor_Kill(&self->actor);
        return;
    }
    self->gallopTimer++;
    self->bossGndInPainting = true;
    for (i = 0; i < 5; i++) {
        if (self->timers[i] != 0) {
            self->timers[i]--;
        }
    }

    self->actionFunc(self, globalCtx);

    if (self->hitTimer != 0) {
        self->hitTimer--;
    }

    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    self->actor.shape.rot.y = self->actor.world.rot.y;

    self->actor.shape.yOffset = Math_SinS(self->hitTimer * 0x9000) * 700.0f * (self->hitTimer / 20.0f);
    self->actor.shape.rot.z = (s16)(Math_SinS(self->hitTimer * 0x7000) * 1500.0f) * (self->hitTimer / 20.0f);
}

void EnfHG_Noop(Actor* thisx, GlobalContext* globalCtx, PSkinAwb* skin) {
}

void EnfHG_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnfHG* self = THIS;
    BossGanondrof* bossGnd = (BossGanondrof*)self->actor.parent;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fhg.c", 2439);
    func_80093D18(globalCtx->state.gfxCtx);

    POLY_OPA_DISP = ((bossGnd->work[GND_INVINC_TIMER] & 4) && (bossGnd->flyMode == GND_FLY_PAINTING))
                        ? Gfx_SetFog(POLY_OPA_DISP, 255, 50, 0, 0, 900, 1099)
                        : Gfx_SetFog(POLY_OPA_DISP, (u32)self->warpColorFilterR, (u32)self->warpColorFilterG,
                                     (u32)self->warpColorFilterB, 0, (s32)self->warpColorFilterUnk1 + 995,
                                     (s32)self->warpColorFilterUnk2 + 1000);
    func_800A6330(&self->actor, globalCtx, &self->skin, EnfHG_Noop, 0x23);
    POLY_OPA_DISP = Gameplay_SetFog(globalCtx, POLY_OPA_DISP);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fhg.c", 2480);
}
