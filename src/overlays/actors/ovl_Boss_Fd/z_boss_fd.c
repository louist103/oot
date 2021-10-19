/*
 * File: z_boss_fd.c
 * Overlay: ovl_Boss_Fd
 * Description: Volvagia, flying form
 */

#include "z_boss_fd.h"
#include "objects/object_fd/object_fd.h"
#include "overlays/actors/ovl_En_Vb_Ball/z_en_vb_ball.h"
#include "overlays/actors/ovl_Bg_Vb_Sima/z_bg_vb_sima.h"
#include "overlays/actors/ovl_Boss_Fd2/z_boss_fd2.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000035

#define THIS ((BossFd*)thisx)

typedef enum {
    /* 0 */ INTRO_FLY_EMERGE,
    /* 1 */ INTRO_FLY_HOLE,
    /* 2 */ INTRO_FLY_CAMERA,
    /* 3 */ INTRO_FLY_RETRAT
} BossFdIntroFlyState;

typedef enum {
    /* 0 */ MANE_CENTER,
    /* 1 */ MANE_RIGHT,
    /* 2 */ MANE_LEFT
} BossFdManeIndex;

typedef enum {
    /* 0 */ EYE_OPEN,
    /* 1 */ EYE_HALF,
    /* 2 */ EYE_CLOSED
} BossFdEyeState;

void BossFd_Init(Actor* thisx, GlobalContext* globalCtx);
void BossFd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BossFd_Update(Actor* thisx, GlobalContext* globalCtx);
void BossFd_Draw(Actor* thisx, GlobalContext* globalCtx);

void BossFd_SetupFly(BossFd* self, GlobalContext* globalCtx);
void BossFd_Fly(BossFd* self, GlobalContext* globalCtx);
void BossFd_Wait(BossFd* self, GlobalContext* globalCtx);
void BossFd_UpdateEffects(BossFd* self, GlobalContext* globalCtx);
void BossFd_DrawBody(GlobalContext* globalCtx, BossFd* self);

const ActorInit Boss_Fd_InitVars = {
    ACTOR_BOSS_FD,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_FD,
    sizeof(BossFd),
    (ActorFunc)BossFd_Init,
    (ActorFunc)BossFd_Destroy,
    (ActorFunc)BossFd_Update,
    (ActorFunc)BossFd_Draw,
};

#include "z_boss_fd_colchk.c"

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x21, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 0, ICHAIN_STOP),
};

void BossFd_SpawnEmber(BossFdEffect* effect, Vec3f* position, Vec3f* velocity, Vec3f* acceleration, f32 scale) {
    s16 i;

    for (i = 0; i < 150; i++, effect++) {
        if (effect->type == BFD_FX_NONE) {
            effect->type = BFD_FX_EMBER;
            effect->pos = *position;
            effect->velocity = *velocity;
            effect->accel = *acceleration;
            effect->scale = scale / 1000.0f;
            effect->alpha = 255;
            effect->timer1 = (s16)Rand_ZeroFloat(10.0f);
            break;
        }
    }
}

void BossFd_SpawnDebris(BossFdEffect* effect, Vec3f* position, Vec3f* velocity, Vec3f* acceleration, f32 scale) {
    s16 i;

    for (i = 0; i < 150; i++, effect++) {
        if (effect->type == BFD_FX_NONE) {
            effect->type = BFD_FX_DEBRIS;
            effect->pos = *position;
            effect->velocity = *velocity;
            effect->accel = *acceleration;
            effect->scale = scale / 1000.0f;
            effect->vFdFxRotX = Rand_ZeroFloat(100.0f);
            effect->vFdFxRotY = Rand_ZeroFloat(100.0f);
            break;
        }
    }
}

void BossFd_SpawnDust(BossFdEffect* effect, Vec3f* position, Vec3f* velocity, Vec3f* acceleration, f32 scale) {
    s16 i;

    for (i = 0; i < 150; i++, effect++) {
        if (effect->type == BFD_FX_NONE) {
            effect->type = BFD_FX_DUST;
            effect->pos = *position;
            effect->velocity = *velocity;
            effect->accel = *acceleration;
            effect->timer2 = 0;
            effect->scale = scale / 400.0f;
            break;
        }
    }
}

void BossFd_SpawnFireBreath(BossFdEffect* effect, Vec3f* position, Vec3f* velocity, Vec3f* acceleration, f32 scale,
                            s16 alpha, s16 kbAngle) {
    s16 i;

    for (i = 0; i < 180; i++, effect++) {
        if (effect->type == BFD_FX_NONE) {
            effect->type = BFD_FX_FIRE_BREATH;
            effect->timer1 = 0;
            effect->pos = *position;
            effect->velocity = *velocity;
            effect->accel = *acceleration;
            effect->pos.x -= effect->velocity.x;
            effect->pos.y -= effect->velocity.y;
            effect->pos.z -= effect->velocity.z;
            effect->vFdFxScaleMod = 0.0f;
            effect->alpha = alpha;
            effect->vFdFxYStop = Rand_ZeroFloat(10.0f);
            effect->timer2 = 0;
            effect->scale = scale / 400.0f;
            effect->kbAngle = kbAngle;
            break;
        }
    }
}

void BossFd_SetCameraSpeed(BossFd* self, f32 speedMod) {
    self->camData.eyeVel.x = fabsf(self->camData.eye.x - self->camData.nextEye.x) * speedMod;
    self->camData.eyeVel.y = fabsf(self->camData.eye.y - self->camData.nextEye.y) * speedMod;
    self->camData.eyeVel.z = fabsf(self->camData.eye.z - self->camData.nextEye.z) * speedMod;
    self->camData.atVel.x = fabsf(self->camData.at.x - self->camData.nextAt.x) * speedMod;
    self->camData.atVel.y = fabsf(self->camData.at.y - self->camData.nextAt.y) * speedMod;
    self->camData.atVel.z = fabsf(self->camData.at.z - self->camData.nextAt.z) * speedMod;
}

void BossFd_UpdateCamera(BossFd* self, GlobalContext* globalCtx) {
    if (self->introCamera != SUBCAM_FREE) {
        Math_ApproachF(&self->camData.eye.x, self->camData.nextEye.x, self->camData.eyeMaxVel.x,
                       self->camData.eyeVel.x * self->camData.speedMod);
        Math_ApproachF(&self->camData.eye.y, self->camData.nextEye.y, self->camData.eyeMaxVel.y,
                       self->camData.eyeVel.y * self->camData.speedMod);
        Math_ApproachF(&self->camData.eye.z, self->camData.nextEye.z, self->camData.eyeMaxVel.z,
                       self->camData.eyeVel.z * self->camData.speedMod);
        Math_ApproachF(&self->camData.at.x, self->camData.nextAt.x, self->camData.atMaxVel.x,
                       self->camData.atVel.x * self->camData.speedMod);
        Math_ApproachF(&self->camData.at.y, self->camData.nextAt.y, self->camData.atMaxVel.y,
                       self->camData.atVel.y * self->camData.speedMod);
        Math_ApproachF(&self->camData.at.z, self->camData.nextAt.z, self->camData.atMaxVel.z,
                       self->camData.atVel.z * self->camData.speedMod);
        Math_ApproachF(&self->camData.speedMod, 1.0f, 1.0f, self->camData.accel);
        self->camData.at.y += self->camData.yMod;
        Gameplay_CameraSetAtEye(globalCtx, self->introCamera, &self->camData.at, &self->camData.eye);
        Math_ApproachZeroF(&self->camData.yMod, 1.0f, 0.1f);
    }
}

void BossFd_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossFd* self = THIS;
    s16 i;

    Flags_SetSwitch(globalCtx, 0x14);
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_BG_VB_SIMA, 680.0f, -100.0f, 0.0f, 0, 0, 0,
                       100);
    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    Actor_SetScale(&self->actor, 0.05f);
    SkelAnime_Init(globalCtx, &self->skelAnimeHead, &gVolvagiaHeadSkel, &gVolvagiaHeadEmergeAnim, NULL, NULL, 0);
    SkelAnime_Init(globalCtx, &self->skelAnimeRightArm, &gVolvagiaRightArmSkel, &gVolvagiaRightArmEmergeAnim, NULL,
                   NULL, 0);
    SkelAnime_Init(globalCtx, &self->skelAnimeLeftArm, &gVolvagiaLeftArmSkel, &gVolvagiaLeftArmEmergeAnim, NULL, NULL,
                   0);
    self->introState = BFD_CS_WAIT;
    if (self->introState == BFD_CS_NONE) {
        Audio_QueueSeqCmd(0x6B);
    }

    self->actor.world.pos.x = self->actor.world.pos.z = 0.0f;
    self->actor.world.pos.y = -200.0f;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->elements);

    for (i = 0; i < 100; i++) {
        self->bodySegsPos[i].x = self->actor.world.pos.x;
        self->bodySegsPos[i].y = self->actor.world.pos.y;
        self->bodySegsPos[i].z = self->actor.world.pos.z;
        if (i < 30) {
            self->centerMane.pos[i].x = self->actor.world.pos.x;
            self->centerMane.pos[i].y = self->actor.world.pos.y;
            self->centerMane.pos[i].z = self->actor.world.pos.z;
        }
    }

    self->actor.colChkInfo.health = 24;
    self->skinSegments = 18;
    if (self->introState == BFD_CS_NONE) {
        self->actionFunc = BossFd_Wait;
    } else {
        BossFd_SetupFly(self, globalCtx);
    }

    if (Flags_GetClear(globalCtx, globalCtx->roomCtx.curRoom.num)) {
        Actor_Kill(&self->actor);
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, 0.0f, 100.0f, 0.0f, 0, 0, 0,
                           WARP_DUNGEON_ADULT);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, 0.0f, 100.0f, 200.0f, 0, 0, 0, 0);
    } else {
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_BOSS_FD2, self->actor.world.pos.x,
                           self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, self->introState);
    }
}

void BossFd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossFd* self = THIS;

    SkelAnime_Free(&self->skelAnimeHead, globalCtx);
    SkelAnime_Free(&self->skelAnimeRightArm, globalCtx);
    SkelAnime_Free(&self->skelAnimeLeftArm, globalCtx);
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

s32 BossFd_IsFacingLink(BossFd* self) {
    return ABS((s16)(self->actor.yawTowardsPlayer - self->actor.world.rot.y)) < 0x2000;
}

void BossFd_SetupFly(BossFd* self, GlobalContext* globalCtx) {
    Animation_PlayOnce(&self->skelAnimeHead, &gVolvagiaHeadEmergeAnim);
    Animation_PlayOnce(&self->skelAnimeRightArm, &gVolvagiaRightArmEmergeAnim);
    Animation_PlayOnce(&self->skelAnimeLeftArm, &gVolvagiaLeftArmEmergeAnim);
    self->actionFunc = BossFd_Fly;
    self->fwork[BFD_TURN_RATE_MAX] = 1000.0f;
}

static Vec3f sHoleLocations[] = {
    { 0.0f, 90.0f, -243.0f },    { 0.0f, 90.0f, 0.0f },    { 0.0f, 90.0f, 243.0f },
    { -243.0f, 90.0f, -243.0f }, { -243.0f, 90.0f, 0.0f }, { -243.0f, 90.0f, 243.0f },
    { 243.0f, 90.0f, -243.0f },  { 243.0f, 90.0f, 0.0f },  { 243.0f, 90.0f, 243.0f },
};

static Vec3f sCeilingTargets[] = {
    { 0.0f, 900.0f, -243.0f }, { 243.0, 900.0f, -100.0f },  { 243.0f, 900.0f, 100.0f },
    { 0.0f, 900.0f, 243.0f },  { -243.0f, 900.0f, 100.0f }, { -243.0, 900.0f, -100.0f },
};

void BossFd_Fly(BossFd* self, GlobalContext* globalCtx) {
    u8 sp1CF = false;
    u8 temp_rand;
    s16 i1;
    s16 i2;
    s16 i3;
    f32 dx;
    f32 dy;
    f32 dz;
    Player* player = GET_PLAYER(globalCtx);
    f32 angleToTarget;
    f32 pitchToTarget;
    Vec3f* holePosition1;
    f32 temp_y;
    f32 temp_x;
    f32 temp_z;
    f32 temp;

    SkelAnime_Update(&self->skelAnimeHead);
    SkelAnime_Update(&self->skelAnimeRightArm);
    SkelAnime_Update(&self->skelAnimeLeftArm);
    dx = self->targetPosition.x - self->actor.world.pos.x;
    dy = self->targetPosition.y - self->actor.world.pos.y;
    dz = self->targetPosition.z - self->actor.world.pos.z;
    dx += Math_SinS((2096.0f + self->fwork[BFD_FLY_WOBBLE_RATE]) * self->work[BFD_MOVE_TIMER]) *
          self->fwork[BFD_FLY_WOBBLE_AMP];
    dy += Math_SinS((1096.0f + self->fwork[BFD_FLY_WOBBLE_RATE]) * self->work[BFD_MOVE_TIMER]) *
          self->fwork[BFD_FLY_WOBBLE_AMP];
    dz += Math_SinS((1796.0f + self->fwork[BFD_FLY_WOBBLE_RATE]) * self->work[BFD_MOVE_TIMER]) *
          self->fwork[BFD_FLY_WOBBLE_AMP];
    angleToTarget = (s16)(Math_FAtan2F(dx, dz) * (0x8000 / M_PI));
    pitchToTarget = (s16)(Math_FAtan2F(dy, sqrtf(SQ(dx) + SQ(dz))) * (0x8000 / M_PI));

    osSyncPrintf("MODE %d\n", self->work[BFD_ACTION_STATE]);

    Math_ApproachF(&self->fwork[BFD_BODY_PULSE], 0.1f, 1.0f, 0.02);

    //                                        Boss Intro Cutscene

    if (self->introState != BFD_CS_NONE) {
        Player* player2 = GET_PLAYER(globalCtx);
        Camera* mainCam = Gameplay_GetCamera(globalCtx, MAIN_CAM);

        switch (self->introState) {
            case BFD_CS_WAIT:
                self->fogMode = 3;
                self->targetPosition.x = 0.0f;
                self->targetPosition.y = -110.0f;
                self->targetPosition.z = 0.0;
                self->fwork[BFD_TURN_RATE_MAX] = 10000.0f;
                self->work[BFD_ACTION_STATE] = BOSSFD_WAIT_INTRO;
                if ((fabsf(player2->actor.world.pos.z) < 80.0f) &&
                    (fabsf(player2->actor.world.pos.x - 340.0f) < 60.0f)) {

                    self->introState = BFD_CS_START;
                    func_80064520(globalCtx, &globalCtx->csCtx);
                    func_8002DF54(globalCtx, &self->actor, 8);
                    self->introCamera = Gameplay_CreateSubCamera(globalCtx);
                    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
                    Gameplay_ChangeCameraStatus(globalCtx, self->introCamera, CAM_STAT_ACTIVE);
                    player2->actor.world.pos.x = 380.0f;
                    player2->actor.world.pos.y = 100.0f;
                    player2->actor.world.pos.z = 0.0f;
                    player2->actor.shape.rot.y = player2->actor.world.rot.y = -0x4000;
                    player2->actor.speedXZ = 0.0f;
                    self->camData.eye.x = player2->actor.world.pos.x - 70.0f;
                    self->camData.eye.y = player2->actor.world.pos.y + 40.0f;
                    self->camData.eye.z = player2->actor.world.pos.z + 70.0f;
                    self->camData.at.x = player2->actor.world.pos.x;
                    self->camData.at.y = player2->actor.world.pos.y + 30.0f;
                    self->camData.at.z = player2->actor.world.pos.z;
                    self->camData.nextEye.x = player2->actor.world.pos.x - 50.0f + 18.0f;
                    self->camData.nextEye.y = player2->actor.world.pos.y + 40;
                    self->camData.nextEye.z = player2->actor.world.pos.z + 50.0f - 18.0f;
                    self->camData.nextAt.x = player2->actor.world.pos.x;
                    self->camData.nextAt.y = player2->actor.world.pos.y + 50.0f;
                    self->camData.nextAt.z = player2->actor.world.pos.z;
                    BossFd_SetCameraSpeed(self, 1.0f);
                    self->camData.atMaxVel.x = self->camData.atMaxVel.y = self->camData.atMaxVel.z = 0.05f;
                    self->camData.eyeMaxVel.x = self->camData.eyeMaxVel.y = self->camData.eyeMaxVel.z = 0.05f;
                    self->timers[0] = 0;
                    self->camData.speedMod = 0.0f;
                    self->camData.accel = 0.0f;
                    if (gSaveContext.eventChkInf[7] & 8) {
                        self->introState = BFD_CS_EMERGE;
                        self->camData.nextEye.x = player2->actor.world.pos.x + 100.0f + 300.0f - 600.0f;
                        self->camData.nextEye.y = player2->actor.world.pos.y + 100.0f - 50.0f;
                        self->camData.nextEye.z = player2->actor.world.pos.z + 200.0f - 150.0f;
                        self->camData.nextAt.x = 0.0f;
                        self->camData.nextAt.y = 120.0f;
                        self->camData.nextAt.z = 0.0f;
                        BossFd_SetCameraSpeed(self, 0.5f);
                        self->camData.eyeMaxVel.x = self->camData.eyeMaxVel.y = self->camData.eyeMaxVel.z = 0.1f;
                        self->camData.atMaxVel.x = self->camData.atMaxVel.y = self->camData.atMaxVel.z = 0.1f;
                        self->camData.accel = 0.005f;
                        self->timers[0] = 0;
                        self->holeIndex = 1;
                        self->targetPosition.x = sHoleLocations[self->holeIndex].x;
                        self->targetPosition.y = sHoleLocations[self->holeIndex].y - 200.0f;
                        self->targetPosition.z = sHoleLocations[self->holeIndex].z;
                        self->timers[0] = 50;
                        self->work[BFD_ACTION_STATE] = BOSSFD_EMERGE;
                        self->actor.world.rot.x = 0x4000;
                        self->work[BFD_MOVE_TIMER] = 0;
                        self->timers[3] = 250;
                        self->timers[2] = 470;
                        self->fwork[BFD_FLY_SPEED] = 5.0f;
                    }
                }
                break;
            case BFD_CS_START:
                if (self->timers[0] == 0) {
                    self->camData.accel = 0.0010000002f;
                    self->timers[0] = 100;
                    self->introState = BFD_CS_LOOK_LINK;
                }
            case BFD_CS_LOOK_LINK:
                player2->actor.world.pos.x = 380.0f;
                player2->actor.world.pos.y = 100.0f;
                player2->actor.world.pos.z = 0.0f;
                player2->actor.speedXZ = 0.0f;
                player2->actor.shape.rot.y = player2->actor.world.rot.y = -0x4000;
                if (self->timers[0] == 50) {
                    self->fogMode = 1;
                }
                if (self->timers[0] < 50) {
                    Audio_PlaySoundGeneral(NA_SE_EN_DODO_K_ROLL - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0,
                                           &D_801333E0, &D_801333E8);
                    self->camData.yMod = Math_CosS(self->work[BFD_MOVE_TIMER] * 0x8000) * self->camData.shake;
                    Math_ApproachF(&self->camData.shake, 2.0f, 1.0f, 0.8 * 0.01f);
                }
                if (self->timers[0] == 40) {
                    func_8002DF54(globalCtx, &self->actor, 0x13);
                }
                if (self->timers[0] == 0) {
                    self->introState = BFD_CS_LOOK_GROUND;
                    self->camData.nextAt.y = player2->actor.world.pos.y + 10.0f;
                    self->camData.atMaxVel.y = 0.2f;
                    self->camData.speedMod = 0.0f;
                    self->camData.accel = 0.02f;
                    self->timers[0] = 70;
                    self->work[BFD_MOVE_TIMER] = 0;
                }
                break;
            case BFD_CS_LOOK_GROUND:
                self->camData.yMod = Math_CosS(self->work[BFD_MOVE_TIMER] * 0x8000) * self->camData.shake;
                Math_ApproachF(&self->camData.shake, 2.0f, 1.0f, 0.8 * 0.01f);
                Audio_PlaySoundGeneral(NA_SE_EN_DODO_K_ROLL - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0,
                                       &D_801333E0, &D_801333E8);
                if (self->timers[0] == 0) {
                    self->introState = BFD_CS_COLLAPSE;
                    self->camData.nextEye.x = player2->actor.world.pos.x + 100.0f + 300.0f;
                    self->camData.nextEye.y = player2->actor.world.pos.y + 100.0f;
                    self->camData.nextEye.z = player2->actor.world.pos.z + 200.0f;
                    self->camData.nextAt.x = player2->actor.world.pos.x;
                    self->camData.nextAt.y = player2->actor.world.pos.y - 150.0f;
                    self->camData.nextAt.z = player2->actor.world.pos.z - 50.0f;
                    BossFd_SetCameraSpeed(self, 0.1f);
                    self->timers[0] = 170;
                    self->camData.speedMod = 0.0f;
                    self->camData.accel = 0.0f;
                    func_8002DF54(globalCtx, &self->actor, 0x14);
                }
                break;
            case BFD_CS_COLLAPSE:
                self->camData.accel = 0.005f;
                self->camData.yMod = Math_CosS(self->work[BFD_MOVE_TIMER] * 0x8000) * self->camData.shake;
                Math_ApproachF(&self->camData.shake, 2.0f, 1.0f, 0.8 * 0.01f);
                Audio_PlaySoundGeneral(NA_SE_EN_DODO_K_ROLL - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0,
                                       &D_801333E0, &D_801333E8);
                if (self->timers[0] == 100) {
                    self->platformSignal = VBSIMA_COLLAPSE;
                }
                if (self->timers[0] == 0) {
                    self->introState = BFD_CS_EMERGE;
                    self->camData.speedMod = 0.0f;
                    self->camData.nextEye.x = player2->actor.world.pos.x + 100.0f + 300.0f - 600.0f;
                    self->camData.nextEye.y = player2->actor.world.pos.y + 100.0f - 50.0f;
                    self->camData.nextEye.z = player2->actor.world.pos.z + 200.0f - 150.0f;
                    self->camData.nextAt.x = 0.0f;
                    self->camData.nextAt.y = 120.0f;
                    self->camData.nextAt.z = 0.0f;
                    BossFd_SetCameraSpeed(self, 0.5f);
                    self->camData.atMaxVel.x = self->camData.atMaxVel.y = self->camData.atMaxVel.z = 0.1f;
                    self->camData.eyeMaxVel.x = self->camData.eyeMaxVel.y = self->camData.eyeMaxVel.z = 0.1f;
                    self->camData.accel = 0.005f;
                    self->timers[0] = 0;
                    self->holeIndex = 1;
                    self->targetPosition.x = sHoleLocations[self->holeIndex].x;
                    self->targetPosition.y = sHoleLocations[self->holeIndex].y - 200.0f;
                    self->targetPosition.z = sHoleLocations[self->holeIndex].z;
                    self->timers[0] = 50;
                    self->work[BFD_ACTION_STATE] = BOSSFD_EMERGE;
                    self->actor.world.rot.x = 0x4000;
                    self->work[BFD_MOVE_TIMER] = 0;
                    self->timers[3] = 250;
                    self->timers[2] = 470;
                    self->fwork[BFD_FLY_SPEED] = 5.0f;
                }
                break;
            case BFD_CS_EMERGE:
                osSyncPrintf("WAY_SPD X = %f\n", self->camData.atVel.x);
                osSyncPrintf("WAY_SPD Y = %f\n", self->camData.atVel.y);
                osSyncPrintf("WAY_SPD Z = %f\n", self->camData.atVel.z);
                if ((self->timers[3] > 190) && !(gSaveContext.eventChkInf[7] & 8)) {
                    Audio_PlaySoundGeneral(NA_SE_EN_DODO_K_ROLL - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0,
                                           &D_801333E0, &D_801333E8);
                }
                if (self->timers[3] == 190) {
                    self->camData.atMaxVel.x = self->camData.atMaxVel.y = self->camData.atMaxVel.z = 0.05f;
                    self->platformSignal = VBSIMA_KILL;
                    func_8002DF54(globalCtx, &self->actor, 1);
                }
                if (self->actor.world.pos.y > 120.0f) {
                    self->camData.nextAt = self->actor.world.pos;
                    self->camData.atVel.x = 190.0f;
                    self->camData.atVel.y = 85.56f;
                    self->camData.atVel.z = 25.0f;
                } else {
                    // the following `temp` stuff is probably fake but is required to match
                    // it's optimized to 1.0f because sp1CF is false at self point, but the 0.1f ends up in rodata
                    temp = 0.1f;
                    if (!sp1CF) {
                        temp = 1.0f;
                    }
                    Math_ApproachF(&self->camData.shake, 2.0f, temp, 0.1 * 0.08f);
                    self->camData.yMod = Math_CosS(self->work[BFD_MOVE_TIMER] * 0x8000) * self->camData.shake;
                }
                if (self->timers[3] == 160) {
                    Audio_QueueSeqCmd(0x6B);
                }
                if ((self->timers[3] == 130) && !(gSaveContext.eventChkInf[7] & 8)) {
                    TitleCard_InitBossName(globalCtx, &globalCtx->actorCtx.titleCtx,
                                           SEGMENTED_TO_VIRTUAL(&gVolvagiaBossTitleCardTex), 0xA0, 0xB4, 0x80, 0x28);
                }
                if (self->timers[3] <= 100) {
                    self->camData.eyeVel.x = self->camData.eyeVel.y = self->camData.eyeVel.z = 2.0f;
                    self->camData.nextEye.x = player2->actor.world.pos.x + 50.0f;
                    self->camData.nextEye.y = player2->actor.world.pos.y + 50.0f;
                    self->camData.nextEye.z = player2->actor.world.pos.z + 50.0f;
                }
                if (self->work[BFD_ACTION_STATE] == BOSSFD_FLY_HOLE) {
                    switch (self->introFlyState) {
                        case INTRO_FLY_EMERGE:
                            self->timers[5] = 100;
                            self->introFlyState = INTRO_FLY_HOLE;
                        case INTRO_FLY_HOLE:
                            if (self->timers[5] == 0) {
                                self->introFlyState = INTRO_FLY_CAMERA;
                                self->timers[5] = 75;
                            }
                            break;
                        case INTRO_FLY_CAMERA:
                            self->targetPosition = self->camData.eye;
                            if (self->timers[5] == 0) {
                                self->timers[0] = 0;
                                self->holeIndex = 7;
                                self->targetPosition.x = sHoleLocations[self->holeIndex].x;
                                self->targetPosition.y = sHoleLocations[self->holeIndex].y + 200.0f + 50.0f;
                                self->targetPosition.z = sHoleLocations[self->holeIndex].z;
                                self->introFlyState = INTRO_FLY_RETRAT;
                            }
                            if (self->timers[5] == 30) {
                                self->work[BFD_ROAR_TIMER] = 40;
                                self->fireBreathTimer = 20;
                            }
                        case INTRO_FLY_RETRAT:
                            break;
                    }
                }
                osSyncPrintf("self->timer[2] = %d\n", self->timers[2]);
                osSyncPrintf("self->timer[5] = %d\n", self->timers[5]);
                if (self->timers[2] == 0) {
                    mainCam->eye = self->camData.eye;
                    mainCam->eyeNext = self->camData.eye;
                    mainCam->at = self->camData.at;
                    func_800C08AC(globalCtx, self->introCamera, 0);
                    self->introState = self->introFlyState = self->introCamera = BFD_CS_NONE;
                    func_80064534(globalCtx, &globalCtx->csCtx);
                    func_8002DF54(globalCtx, &self->actor, 7);
                    self->actionFunc = BossFd_Wait;
                    self->handoffSignal = FD2_SIGNAL_GROUND;
                    gSaveContext.eventChkInf[7] |= 8;
                }
                break;
        }
        BossFd_UpdateCamera(self, globalCtx);
    } else {
        self->fwork[BFD_FLY_SPEED] = 5.0f;
    }

    //                             Attacks and Death Cutscene

    switch (self->work[BFD_ACTION_STATE]) {
        case BOSSFD_FLY_MAIN:
            sp1CF = true;
            if (self->timers[0] == 0) {
                if (self->actor.colChkInfo.health == 0) {
                    self->work[BFD_ACTION_STATE] = BOSSFD_DEATH_START;
                    self->timers[0] = 0;
                    self->timers[1] = 100;
                } else {
                    if (self->introState != BFD_CS_NONE) {
                        self->holeIndex = 6;
                    } else {
                        do {
                            temp_rand = Rand_ZeroFloat(8.9f);
                        } while (temp_rand == self->holeIndex);
                        self->holeIndex = temp_rand;
                    }
                    self->targetPosition.x = sHoleLocations[self->holeIndex].x;
                    self->targetPosition.y = sHoleLocations[self->holeIndex].y + 200.0f + 50.0f;
                    self->targetPosition.z = sHoleLocations[self->holeIndex].z;
                    self->fwork[BFD_TURN_RATE] = 0.0f;
                    self->fwork[BFD_TURN_RATE_MAX] = 1000.0f;
                    if (self->introState != BFD_CS_NONE) {
                        self->timers[0] = 10050;
                    } else {
                        self->timers[0] = 20;
                    }
                    self->fwork[BFD_FLY_WOBBLE_AMP] = 100.0f;
                    self->work[BFD_ACTION_STATE] = BOSSFD_FLY_HOLE;

                    if (self->work[BFD_START_ATTACK]) {
                        self->work[BFD_START_ATTACK] = false;
                        self->work[BFD_FLY_COUNT]++;
                        if (self->work[BFD_FLY_COUNT] & 1) {
                            self->work[BFD_ACTION_STATE] = BOSSFD_FLY_CHASE;
                            self->timers[0] = 300;
                            self->fwork[BFD_TURN_RATE_MAX] = 900.0f;
                            self->fwork[BFD_TARGET_Y_OFFSET] = 300.0f;
                            self->work[BFD_UNK_234] = self->work[BFD_UNK_236] = 0;
                        } else {
                            self->work[BFD_ACTION_STATE] = BOSSFD_FLY_CEILING;
                        }
                    }
                }
            }
            break;
        case BOSSFD_FLY_HOLE:
            if ((self->timers[0] == 0) && (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 100.0f)) {
                self->work[BFD_ACTION_STATE] = BOSSFD_BURROW;
                self->targetPosition.y = sHoleLocations[self->holeIndex].y - 70.0f;
                self->fwork[BFD_TURN_RATE_MAX] = 10000.0f;
                self->fwork[BFD_FLY_WOBBLE_AMP] = 0.0f;
                self->timers[0] = 150;
                self->work[BFD_ROAR_TIMER] = 40;
                self->holePosition.x = self->targetPosition.x;
                self->holePosition.z = self->targetPosition.z;
            }
            break;
        case BOSSFD_BURROW:
            sp1CF = true;
            if (self->timers[0] == 0) {
                self->actionFunc = BossFd_Wait;
                self->handoffSignal = FD2_SIGNAL_GROUND;
            }
            break;
        case BOSSFD_EMERGE:
            if ((self->timers[0] == 0) && (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 100.0f)) {
                self->actor.world.pos = self->targetPosition;
                self->work[BFD_ACTION_STATE] = BOSSFD_FLY_MAIN;
                self->actor.world.rot.x = 0x4000;
                self->targetPosition.y = sHoleLocations[self->holeIndex].y + 200.0f;
                self->timers[4] = 80;
                self->fwork[BFD_TURN_RATE_MAX] = 1000.0f;
                self->fwork[BFD_FLY_WOBBLE_AMP] = 0.0f;
                self->holePosition.x = self->targetPosition.x;
                self->holePosition.z = self->targetPosition.z;

                func_80033E1C(globalCtx, 1, 0x50, 0x5000);
                if (self->introState != BFD_CS_NONE) {
                    self->timers[0] = 50;
                } else {
                    self->timers[0] = 50;
                }
            }
            break;
        case BOSSFD_FLY_CEILING:
            self->fwork[BFD_FLY_SPEED] = 8;
            self->targetPosition.x = 0.0f;
            self->targetPosition.y = 700.0f;
            self->targetPosition.z = -300.0f;
            self->fwork[BFD_FLY_WOBBLE_AMP] = 200.0f;
            self->fwork[BFD_TURN_RATE_MAX] = 3000.0f;
            if (self->actor.world.pos.y > 700.0f) {
                self->work[BFD_ACTION_STATE] = BOSSFD_DROP_ROCKS;
                self->timers[0] = 25;
                self->timers[2] = 150;
                self->work[BFD_CEILING_TARGET] = 0;
            }
            break;
        case BOSSFD_DROP_ROCKS:
            self->fwork[BFD_FLY_SPEED] = 8;
            self->fwork[BFD_FLY_WOBBLE_AMP] = 200.0f;
            self->fwork[BFD_TURN_RATE_MAX] = 10000.0f;
            self->targetPosition.x = sCeilingTargets[self->work[BFD_CEILING_TARGET]].x;
            self->targetPosition.y = sCeilingTargets[self->work[BFD_CEILING_TARGET]].y + 900.0f;
            self->targetPosition.z = sCeilingTargets[self->work[BFD_CEILING_TARGET]].z;
            if (self->timers[0] == 0) {
                self->timers[0] = 25;
                self->work[BFD_CEILING_TARGET]++;
                if (self->work[BFD_CEILING_TARGET] >= 6) {
                    self->work[BFD_CEILING_TARGET] = 0;
                }
            }
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 50.0f, 100.0f, 2);
            if (self->timers[1] == 0) {
                osSyncPrintf("BGCHECKKKKKKKKKKKKKKKKKKKKKKK\n");
                if (self->actor.bgCheckFlags & 0x10) {
                    self->fwork[BFD_CEILING_BOUNCE] = -18384.0f;
                    self->timers[1] = 10;
                    Audio_PlaySoundGeneral(NA_SE_EV_EXPLOSION, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                           &D_801333E8);
                    func_80033E1C(globalCtx, 3, 0xA, 0x7530);
                    self->work[BFD_ROCK_TIMER] = 300;
                }
            } else {
                pitchToTarget = self->fwork[BFD_CEILING_BOUNCE];
                Math_ApproachZeroF(&self->fwork[BFD_CEILING_BOUNCE], 1.0f, 1000.0f);
            }
            if (self->timers[2] == 0) {
                self->work[BFD_ACTION_STATE] = BOSSFD_FLY_MAIN;
                self->timers[0] = 0;
                self->work[BFD_START_ATTACK] = false;
            }
            break;
        case BOSSFD_FLY_CHASE:
            self->actor.flags |= 0x1000000;
            temp_y = Math_SinS(self->work[BFD_MOVE_TIMER] * 2396.0f) * 30.0f + self->fwork[BFD_TARGET_Y_OFFSET];
            self->targetPosition.x = player->actor.world.pos.x;
            self->targetPosition.y = player->actor.world.pos.y + temp_y + 30.0f;
            self->targetPosition.z = player->actor.world.pos.z;
            self->fwork[BFD_FLY_WOBBLE_AMP] = 0.0f;
            if (((self->timers[0] % 64) == 0) && (self->timers[0] < 450)) {
                self->work[BFD_ROAR_TIMER] = 40;
                if (BossFd_IsFacingLink(self)) {
                    self->fireBreathTimer = 20;
                }
            }
            if ((self->work[BFD_DAMAGE_FLASH_TIMER] != 0) || (self->timers[0] == 0) ||
                (player->actor.world.pos.y < 70.0f)) {
                self->work[BFD_ACTION_STATE] = BOSSFD_FLY_MAIN;
                self->timers[0] = 0;
                self->work[BFD_START_ATTACK] = false;
            } else {
                Math_ApproachF(&self->fwork[BFD_TARGET_Y_OFFSET], 50.0, 1.0f, 2.0f);
            }
            break;
        case BOSSFD_DEATH_START:
            if (sqrtf(SQ(dx) + SQ(dz)) < 50.0f) {
                self->timers[0] = 0;
            }
            if (self->timers[0] == 0) {
                self->timers[0] = (s16)Rand_ZeroFloat(10.0f) + 10;
                do {
                    self->targetPosition.x = Rand_CenteredFloat(200.0f);
                    self->targetPosition.y = 390.0f;
                    self->targetPosition.z = Rand_CenteredFloat(200.0f);
                    temp_x = self->targetPosition.x - self->actor.world.pos.x;
                    temp_z = self->targetPosition.z - self->actor.world.pos.z;
                } while (!(sqrtf(SQ(temp_x) + SQ(temp_z)) > 100.0f));
            }
            self->fwork[BFD_FLY_WOBBLE_AMP] = 200.0f;
            self->fwork[BFD_FLY_WOBBLE_RATE] = 1000.0f;
            self->fwork[BFD_TURN_RATE_MAX] = 10000.0f;
            Math_ApproachF(&self->fwork[BFD_BODY_PULSE], 0.3f, 1.0f, 0.05f);
            if (self->timers[1] == 0) {
                self->work[BFD_ACTION_STATE] = BOSSFD_SKIN_BURN;
                self->timers[0] = 30;
            }
            break;
        case BOSSFD_SKIN_BURN:
            self->targetPosition.x = 0.0f;
            self->targetPosition.y = 390.0f;
            self->targetPosition.z = 0.0f;
            self->fwork[BFD_FLY_WOBBLE_AMP] = 200.0f;
            self->fwork[BFD_FLY_WOBBLE_RATE] = 1000.0f;
            self->fwork[BFD_TURN_RATE_MAX] = 2000.0f;
            Math_ApproachF(&self->fwork[BFD_BODY_PULSE], 0.3f, 1.0f, 0.05f);
            if ((self->timers[0] == 0) && ((self->work[BFD_MOVE_TIMER] % 4) == 0)) {
                if (self->skinSegments != 0) {
                    self->skinSegments--;
                    if (self->skinSegments == 0) {
                        Audio_QueueSeqCmd(0x21);
                    }
                } else {
                    self->work[BFD_ACTION_STATE] = BOSSFD_BONES_FALL;
                    self->timers[0] = 30;
                }
            }
            if ((self->work[BFD_MOVE_TIMER] % 32) == 0) {
                self->work[BFD_ROAR_TIMER] = 40;
            }

            if (self->skinSegments != 0) {
                Vec3f sp188;
                Vec3f sp17C = { 0.0f, 0.0f, 0.0f };
                Vec3f sp170;
                Vec3f sp164 = { 0.0f, 0.03f, 0.0f };
                Vec3f sp158;
                f32 pad154;
                s16 temp_rand2;
                s16 sp150;

                if (self->fogMode == 0) {
                    globalCtx->envCtx.unk_D8 = 0;
                }
                self->fogMode = 0xA;

                sp150 = 1;
                if (self->work[BFD_MOVE_TIMER] & 0x1C) {
                    Audio_PlaySoundGeneral(NA_SE_EN_VALVAISA_BURN - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0,
                                           &D_801333E0, &D_801333E8);
                }
                for (i1 = 0; i1 < sp150; i1++) {
                    if (sp150) { // Needed for matching
                        temp_rand2 = Rand_ZeroFloat(99.9f);

                        sp188.x = self->bodySegsPos[temp_rand2].x;
                        sp188.y = self->bodySegsPos[temp_rand2].y - 10.0f;
                        sp188.z = self->bodySegsPos[temp_rand2].z;

                        sp164.y = 0.03f;

                        EffectSsKFire_Spawn(globalCtx, &sp188, &sp17C, &sp164, (s16)Rand_ZeroFloat(20.0f) + 40, 0x64);

                        for (i2 = 0; i2 < 15; i2++) {
                            sp170.x = Rand_CenteredFloat(20.0f);
                            sp170.y = Rand_CenteredFloat(20.0f);
                            sp170.z = Rand_CenteredFloat(20.0f);

                            sp158.y = 0.4f;
                            sp158.x = Rand_CenteredFloat(0.5f);
                            sp158.z = Rand_CenteredFloat(0.5f);

                            BossFd_SpawnEmber(self->effects, &sp188, &sp170, &sp158, (s16)Rand_ZeroFloat(3.0f) + 8);
                        }
                    }
                }
            }
            break;
        case BOSSFD_BONES_FALL:
            self->work[BFD_STOP_FLAG] = true;
            self->fogMode = 3;
            if (self->timers[0] < 18) {
                self->bodyFallApart[self->timers[0]] = 1;
            }
            if (self->timers[0] == 0) {
                self->work[BFD_ACTION_STATE] = BOSSFD_SKULL_PAUSE;
                self->timers[0] = 15;
                self->work[BFD_CEILING_TARGET] = 0;
                player->actor.world.pos.y = 90.0f;
                player->actor.world.pos.x = 40.0f;
                player->actor.world.pos.z = 150.0f;
            }
            break;
        case BOSSFD_SKULL_PAUSE:
            if (self->timers[0] == 0) {
                self->work[BFD_ACTION_STATE] = BOSSFD_SKULL_FALL;
                self->timers[0] = 20;
                self->work[BFD_STOP_FLAG] = false;
            }
            break;
        case BOSSFD_SKULL_FALL:
            self->fwork[BFD_TURN_RATE] = self->fwork[BFD_TURN_RATE_MAX] = self->actor.speedXZ =
                self->fwork[BFD_FLY_SPEED] = 0;

            if (self->timers[0] == 1) {
                self->actor.world.pos.x = 0;
                self->actor.world.pos.y = 900.0f;
                self->actor.world.pos.z = 150.0f;
                self->actor.world.rot.x = self->actor.world.rot.y = 0;
                self->actor.shape.rot.z = 0x1200;
                self->actor.velocity.x = 0;
                self->actor.velocity.z = 0;
            }
            if (self->timers[0] == 0) {
                if (self->actor.world.pos.y <= 110.0f) {
                    self->actor.world.pos.y = 110.0f;
                    self->actor.velocity.y = 0;
                    if (self->work[BFD_CEILING_TARGET] == 0) {
                        self->work[BFD_CEILING_TARGET]++;
                        self->timers[1] = 60;
                        self->work[BFD_CAM_SHAKE_TIMER] = 20;
                        Audio_PlaySoundGeneral(NA_SE_EN_VALVAISA_LAND2, &self->actor.projectedPos, 4, &D_801333E0,
                                               &D_801333E0, &D_801333E8);
                        func_8002DF54(globalCtx, &self->actor, 5);
                        for (i1 = 0; i1 < 15; i1++) {
                            Vec3f sp144 = { 0.0f, 0.0f, 0.0f };
                            Vec3f sp138 = { 0.0f, 0.0f, 0.0f };
                            Vec3f sp12C;

                            sp144.x = Rand_CenteredFloat(8.0f);
                            sp144.y = Rand_ZeroFloat(1.0f);
                            sp144.z = Rand_CenteredFloat(8.0f);

                            sp138.y = 0.3f;

                            sp12C.x = Rand_CenteredFloat(10.0f) + self->actor.world.pos.x;
                            sp12C.y = Rand_CenteredFloat(10.0f) + self->actor.world.pos.y;
                            sp12C.z = Rand_CenteredFloat(10.0f) + self->actor.world.pos.z;
                            BossFd_SpawnDust(self->effects, &sp12C, &sp144, &sp138, Rand_ZeroFloat(100.0f) + 300);
                        }
                    }
                } else {
                    self->actor.velocity.y -= 1.0f;
                }
            } else {
                self->actor.velocity.y = 0;
            }
            if (self->timers[1] == 1) {
                self->work[BFD_ACTION_STATE] = BOSSFD_SKULL_BURN;
                self->timers[0] = 70;
            }
            break;
        case BOSSFD_SKULL_BURN:
            self->actor.velocity.y = 0.0f;
            self->actor.world.pos.y = 110.0f;
            self->fwork[BFD_TURN_RATE] = self->fwork[BFD_TURN_RATE_MAX] = self->actor.speedXZ =
                self->fwork[BFD_FLY_SPEED] = 0.0f;

            if ((50 > self->timers[0]) && (self->timers[0] > 0)) {
                Vec3f sp120;
                Vec3f sp114 = { 0.0f, 0.0f, 0.0f };
                Vec3f sp108 = { 0.0f, 0.03f, 0.0f };

                Audio_PlaySoundGeneral(NA_SE_EN_GOMA_LAST - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0,
                                       &D_801333E0, &D_801333E8);

                sp120.x = Rand_CenteredFloat(40.0f) + self->actor.world.pos.x;
                sp120.y = (Rand_CenteredFloat(10.0f) + self->actor.world.pos.y) - 10.0f;
                sp120.z = (Rand_CenteredFloat(40.0f) + self->actor.world.pos.z) + 5.0f;

                sp108.y = 0.03f;

                EffectSsKFire_Spawn(globalCtx, &sp120, &sp114, &sp108, (s16)Rand_ZeroFloat(15.0f) + 30, 0);
            }
            if (self->timers[0] < 20) {
                Math_ApproachZeroF(&self->actor.scale.x, 1.0f, 0.0025f);
                Actor_SetScale(&self->actor, self->actor.scale.x);
            }
            if (self->timers[0] == 0) {
                self->actionFunc = BossFd_Wait;
                self->actor.world.pos.y -= 1000.0f;
            }
            if (self->timers[0] == 7) {
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, self->actor.world.pos.x,
                            self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
            }
            break;
        case BOSSFD_WAIT_INTRO:
            break;
    }

    //                                 Update body segments and mane

    if (!self->work[BFD_STOP_FLAG]) {
        s16 i4;
        Vec3f spE0[3];
        Vec3f spBC[3];
        f32 phi_f20;
        f32 padB4;
        f32 padB0;
        f32 padAC;

        Math_ApproachS(&self->actor.world.rot.y, angleToTarget, 0xA, self->fwork[BFD_TURN_RATE]);

        if (((self->work[BFD_ACTION_STATE] == BOSSFD_FLY_CHASE) ||
             (self->work[BFD_ACTION_STATE] == BOSSFD_FLY_UNUSED)) &&
            (self->actor.world.pos.y < 110.0f) && (pitchToTarget < 0)) {
            pitchToTarget = 0;
            Math_ApproachF(&self->actor.world.pos.y, 110.0f, 1.0f, 5.0f);
        }

        Math_ApproachS(&self->actor.world.rot.x, pitchToTarget, 0xA, self->fwork[BFD_TURN_RATE]);
        Math_ApproachF(&self->fwork[BFD_TURN_RATE], self->fwork[BFD_TURN_RATE_MAX], 1.0f, 20000.0f);
        Math_ApproachF(&self->actor.speedXZ, self->fwork[BFD_FLY_SPEED], 1.0f, 0.1f);
        if (self->work[BFD_ACTION_STATE] < BOSSFD_SKULL_FALL) {
            func_8002D908(&self->actor);
        }
        func_8002D7EC(&self->actor);

        self->work[BFD_LEAD_BODY_SEG]++;
        if (self->work[BFD_LEAD_BODY_SEG] >= 100) {
            self->work[BFD_LEAD_BODY_SEG] = 0;
        }
        i4 = self->work[BFD_LEAD_BODY_SEG];
        self->bodySegsPos[i4].x = self->actor.world.pos.x;
        self->bodySegsPos[i4].y = self->actor.world.pos.y;
        self->bodySegsPos[i4].z = self->actor.world.pos.z;
        self->bodySegsRot[i4].x = (self->actor.world.rot.x / (f32)0x8000) * M_PI;
        self->bodySegsRot[i4].y = (self->actor.world.rot.y / (f32)0x8000) * M_PI;
        self->bodySegsRot[i4].z = (self->actor.world.rot.z / (f32)0x8000) * M_PI;

        self->work[BFD_LEAD_MANE_SEG]++;
        if (self->work[BFD_LEAD_MANE_SEG] >= 30) {
            self->work[BFD_LEAD_MANE_SEG] = 0;
        }
        i4 = self->work[BFD_LEAD_MANE_SEG];
        self->centerMane.scale[i4] = (Math_SinS(self->work[BFD_MOVE_TIMER] * 5596.0f) * 0.3f) + 1.0f;
        self->rightMane.scale[i4] = (Math_SinS(self->work[BFD_MOVE_TIMER] * 5496.0f) * 0.3f) + 1.0f;
        self->leftMane.scale[i4] = (Math_CosS(self->work[BFD_MOVE_TIMER] * 5696.0f) * 0.3f) + 1.0f;
        self->centerMane.pos[i4] = self->centerMane.head;
        self->fireManeRot[i4].x = (self->actor.world.rot.x / (f32)0x8000) * M_PI;
        self->fireManeRot[i4].y = (self->actor.world.rot.y / (f32)0x8000) * M_PI;
        self->fireManeRot[i4].z = (self->actor.world.rot.z / (f32)0x8000) * M_PI;
        self->rightMane.pos[i4] = self->rightMane.head;
        self->leftMane.pos[i4] = self->leftMane.head;

        if ((0x3000 > self->actor.world.rot.x) && (self->actor.world.rot.x > -0x3000)) {
            Math_ApproachF(&self->flattenMane, 1.0f, 1.0f, 0.05f);
        } else {
            Math_ApproachF(&self->flattenMane, 0.5f, 1.0f, 0.05f);
        }

        if (self->work[BFD_ACTION_STATE] < BOSSFD_SKULL_FALL) {
            if ((self->actor.prevPos.y < 90.0f) && (90.0f <= self->actor.world.pos.y)) {
                self->timers[4] = 80;
                func_80033E1C(globalCtx, 1, 80, 0x5000);
                self->work[BFD_ROAR_TIMER] = 40;
                self->work[BFD_MANE_EMBERS_TIMER] = 30;
                self->work[BFD_SPLASH_TIMER] = 10;
            }
            if ((self->actor.prevPos.y > 90.0f) && (90.0f >= self->actor.world.pos.y)) {
                self->timers[4] = 80;
                func_80033E1C(globalCtx, 1, 80, 0x5000);
                self->work[BFD_MANE_EMBERS_TIMER] = 30;
                self->work[BFD_SPLASH_TIMER] = 10;
            }
        }

        if (!sp1CF) {
            spE0[0].x = spE0[0].y = Math_SinS(self->work[BFD_MOVE_TIMER] * 1500.0f) * 3000.0f;
            spE0[1].x = Math_SinS(self->work[BFD_MOVE_TIMER] * 2000.0f) * 4000.0f;
            spE0[1].y = Math_SinS(self->work[BFD_MOVE_TIMER] * 2200.0f) * 4000.0f;
            spE0[2].x = Math_SinS(self->work[BFD_MOVE_TIMER] * 1700.0f) * 2000.0f;
            spE0[2].y = Math_SinS(self->work[BFD_MOVE_TIMER] * 1900.0f) * 2000.0f;
            spBC[0].x = spBC[0].y = Math_SinS(self->work[BFD_MOVE_TIMER] * 1500.0f) * -3000.0f;
            spBC[1].x = Math_SinS(self->work[BFD_MOVE_TIMER] * 2200.0f) * -4000.0f;
            spBC[1].y = Math_SinS(self->work[BFD_MOVE_TIMER] * 2000.0f) * -4000.0f;
            spBC[2].x = Math_SinS(self->work[BFD_MOVE_TIMER] * 1900.0f) * -2000.0f;
            spBC[2].y = Math_SinS(self->work[BFD_MOVE_TIMER] * 1700.0f) * -2000.0f;

            for (i3 = 0; i3 < 3; i3++) {
                Math_ApproachF(&self->rightArmRot[i3].x, spE0[i3].x, 1.0f, 1000.0f);
                Math_ApproachF(&self->rightArmRot[i3].y, spE0[i3].y, 1.0f, 1000.0f);
                Math_ApproachF(&self->leftArmRot[i3].x, spBC[i3].x, 1.0f, 1000.0f);
                Math_ApproachF(&self->leftArmRot[i3].y, spBC[i3].y, 1.0f, 1000.0f);
            }
        } else {
            for (i2 = 0; i2 < 3; i2++) {
                phi_f20 = 0.0f;
                Math_ApproachZeroF(&self->rightArmRot[i2].y, 0.1f, 100.0f);
                Math_ApproachZeroF(&self->leftArmRot[i2].y, 0.1f, 100.0f);
                if (i2 == 0) {
                    phi_f20 = -3000.0f;
                }
                Math_ApproachF(&self->rightArmRot[i2].x, phi_f20, 0.1f, 100.0f);
                Math_ApproachF(&self->leftArmRot[i2].x, -phi_f20, 0.1f, 100.0f);
            }
        }
    }
}

void BossFd_Wait(BossFd* self, GlobalContext* globalCtx) {
    if (self->handoffSignal == FD2_SIGNAL_FLY) { // Set by BossFd2
        u8 temp_rand;

        self->handoffSignal = FD2_SIGNAL_NONE;
        BossFd_SetupFly(self, globalCtx);
        do {
            temp_rand = Rand_ZeroFloat(8.9f);
        } while (temp_rand == self->holeIndex);
        self->holeIndex = temp_rand;
        if (1) {} // Needed for matching
        self->targetPosition.x = sHoleLocations[self->holeIndex].x;
        self->targetPosition.y = sHoleLocations[self->holeIndex].y - 200.0f;
        self->targetPosition.z = sHoleLocations[self->holeIndex].z;
        self->actor.world.pos = self->targetPosition;

        self->timers[0] = 10;
        self->work[BFD_ACTION_STATE] = BOSSFD_EMERGE;
        self->work[BFD_START_ATTACK] = true;
    }
    if (self->handoffSignal == FD2_SIGNAL_DEATH) {
        self->handoffSignal = FD2_SIGNAL_NONE;
        BossFd_SetupFly(self, globalCtx);
        self->holeIndex = 1;
        self->targetPosition.x = sHoleLocations[1].x;
        self->targetPosition.y = sHoleLocations[1].y - 200.0f;
        self->targetPosition.z = sHoleLocations[1].z;
        self->actor.world.pos = self->targetPosition;
        self->timers[0] = 10;
        self->work[BFD_ACTION_STATE] = BOSSFD_EMERGE;
    }
}

static Vec3f sFireAudioVec = { 0.0f, 0.0f, 50.0f };

void BossFd_Effects(BossFd* self, GlobalContext* globalCtx) {
    static Color_RGBA8 colorYellow = { 255, 255, 0, 255 };
    static Color_RGBA8 colorRed = { 255, 10, 0, 255 };
    s16 breathOpacity = 0;
    f32 jawAngle;
    f32 jawSpeed;
    f32 emberRate;
    f32 emberSpeed;
    s16 eyeStates[] = { EYE_OPEN, EYE_HALF, EYE_CLOSED, EYE_CLOSED, EYE_HALF };
    f32 temp_x;
    f32 temp_z;
    s16 i;

    if (1) {} // Needed for match

    if (self->fogMode == 0) {
        globalCtx->envCtx.unk_BF = 0;
        globalCtx->envCtx.unk_D8 = 0.5f + 0.5f * Math_SinS(self->work[BFD_VAR_TIMER] * 0x500);
        globalCtx->envCtx.unk_DC = 2;
        globalCtx->envCtx.unk_BD = 1;
        globalCtx->envCtx.unk_BE = 0;
    } else if (self->fogMode == 3) {
        globalCtx->envCtx.unk_BF = 0;
        globalCtx->envCtx.unk_DC = 2;
        globalCtx->envCtx.unk_BD = 2;
        globalCtx->envCtx.unk_BE = 0;
        Math_ApproachF(&globalCtx->envCtx.unk_D8, 1.0f, 1.0f, 0.05f);
    } else if (self->fogMode == 2) {
        self->fogMode--;
        globalCtx->envCtx.unk_BF = 0;
        Math_ApproachF(&globalCtx->envCtx.unk_D8, 0.55f + 0.05f * Math_SinS(self->work[BFD_VAR_TIMER] * 0x3E00), 1.0f,
                       0.15f);
        globalCtx->envCtx.unk_DC = 2;
        globalCtx->envCtx.unk_BD = 3;
        globalCtx->envCtx.unk_BE = 0;
    } else if (self->fogMode == 10) {
        self->fogMode = 1;
        globalCtx->envCtx.unk_BF = 0;
        Math_ApproachF(&globalCtx->envCtx.unk_D8, 0.21f + 0.07f * Math_SinS(self->work[BFD_VAR_TIMER] * 0xC00), 1.0f,
                       0.05f);
        globalCtx->envCtx.unk_DC = 2;
        globalCtx->envCtx.unk_BD = 3;
        globalCtx->envCtx.unk_BE = 0;
    } else if (self->fogMode == 1) {
        Math_ApproachF(&globalCtx->envCtx.unk_D8, 0.0f, 1.0f, 0.03f);
        if (globalCtx->envCtx.unk_D8 <= 0.01f) {
            self->fogMode = 0;
        }
    }

    if (self->work[BFD_MANE_EMBERS_TIMER] != 0) {
        self->work[BFD_MANE_EMBERS_TIMER]--;
        emberSpeed = emberRate = 20.0f;
    } else {
        emberRate = 3.0f;
        emberSpeed = 5.0f;
    }
    Math_ApproachF(&self->fwork[BFD_MANE_EMBER_RATE], emberRate, 1.0f, 0.1f);
    Math_ApproachF(&self->fwork[BFD_MANE_EMBER_SPEED], emberSpeed, 1.0f, 0.5f);

    if (((self->work[BFD_VAR_TIMER] % 8) == 0) && (Rand_ZeroOne() < 0.3f)) {
        self->work[BFD_BLINK_TIMER] = 4;
    }
    self->eyeState = eyeStates[self->work[BFD_BLINK_TIMER]];

    if (self->work[BFD_BLINK_TIMER] != 0) {
        self->work[BFD_BLINK_TIMER]--;
    }

    if (self->work[BFD_ROAR_TIMER] != 0) {
        if (self->work[BFD_ROAR_TIMER] == 37) {
            Audio_PlaySoundGeneral(NA_SE_EN_VALVAISA_ROAR, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
        jawAngle = 6000.0f;
        jawSpeed = 1300.0f;
    } else {
        jawAngle = (self->work[BFD_VAR_TIMER] & 0x10) ? 0.0f : 1000.0f;
        jawSpeed = 500.0f;
    }
    Math_ApproachF(&self->jawOpening, jawAngle, 0.3f, jawSpeed);

    if (self->work[BFD_ROAR_TIMER] != 0) {
        self->work[BFD_ROAR_TIMER]--;
    }

    if (self->timers[4] != 0) {
        Vec3f spawnVel1;
        Vec3f spawnAccel1;
        Vec3f spawnPos1;
        s32 pad;

        Audio_PlaySoundGeneral(NA_SE_EN_VALVAISA_APPEAR - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0,
                               &D_801333E0, &D_801333E8);
        if (self->work[BFD_SPLASH_TIMER] != 0) {
            self->work[BFD_SPLASH_TIMER]--;
            if ((self->actor.colChkInfo.health == 0) ||
                ((self->introState == BFD_CS_EMERGE) && (self->actor.world.rot.x > 0x3000))) {
                if ((u8)self->fogMode == 0) {
                    globalCtx->envCtx.unk_D8 = 0.0f;
                }
                self->fogMode = 2;
            }
            for (i = 0; i < 5; i++) {
                spawnVel1.x = Rand_CenteredFloat(20.0f);
                spawnVel1.y = Rand_ZeroFloat(5.0f) + 4.0f;
                spawnVel1.z = Rand_CenteredFloat(20.0f);

                spawnAccel1.x = spawnAccel1.z = 0.0f;
                spawnAccel1.y = -0.3f;

                temp_x = (spawnVel1.x * 20) / 10.0f;
                temp_z = (spawnVel1.z * 20) / 10.0f;
                spawnPos1.x = temp_x + self->holePosition.x;
                spawnPos1.y = 100.0f;
                spawnPos1.z = temp_z + self->holePosition.z;

                func_8002836C(globalCtx, &spawnPos1, &spawnVel1, &spawnAccel1, &colorYellow, &colorRed,
                              (s16)Rand_ZeroFloat(150.0f) + 800, 10, (s16)Rand_ZeroFloat(5.0f) + 17);
            }
        } else {
            for (i = 0; i < 2; i++) {
                spawnVel1.x = Rand_CenteredFloat(10.0f);
                spawnVel1.y = Rand_ZeroFloat(3.0f) + 3.0f;
                spawnVel1.z = Rand_CenteredFloat(10.0f);

                spawnAccel1.x = spawnAccel1.z = 0.0f;
                spawnAccel1.y = -0.3f;
                temp_x = (spawnVel1.x * 50) / 10.0f;
                temp_z = (spawnVel1.z * 50) / 10.0f;

                spawnPos1.x = temp_x + self->holePosition.x;
                spawnPos1.y = 100.0f;
                spawnPos1.z = temp_z + self->holePosition.z;

                func_8002836C(globalCtx, &spawnPos1, &spawnVel1, &spawnAccel1, &colorYellow, &colorRed, 500, 10, 20);
            }
        }

        for (i = 0; i < 8; i++) {
            spawnVel1.x = Rand_CenteredFloat(20.0f);
            spawnVel1.y = Rand_ZeroFloat(10.0f);
            spawnVel1.z = Rand_CenteredFloat(20.0f);

            spawnAccel1.y = 0.4f;
            spawnAccel1.x = Rand_CenteredFloat(0.5f);
            spawnAccel1.z = Rand_CenteredFloat(0.5f);

            spawnPos1.x = Rand_CenteredFloat(60.0) + self->holePosition.x;
            spawnPos1.y = Rand_ZeroFloat(40.0f) + 100.0f;
            spawnPos1.z = Rand_CenteredFloat(60.0) + self->holePosition.z;

            BossFd_SpawnEmber(self->effects, &spawnPos1, &spawnVel1, &spawnAccel1, (s16)Rand_ZeroFloat(1.5f) + 6);
        }
    }

    if ((self->fireBreathTimer != 0) && (self->fireBreathTimer < 17)) {
        breathOpacity = (self->fireBreathTimer >= 6) ? 255 : self->fireBreathTimer * 50;
    }
    if (breathOpacity != 0) {
        f32 spawnAngleX;
        f32 spawnAngleY;
        Vec3f spawnSpeed2 = { 0.0f, 0.0f, 0.0f };
        Vec3f spawnVel2;
        Vec3f spawnAccel2 = { 0.0f, 0.0f, 0.0f };
        Vec3f spawnPos2;

        self->fogMode = 2;
        spawnSpeed2.z = 30.0f;

        Audio_PlaySoundGeneral(NA_SE_EN_VALVAISA_FIRE - SFX_FLAG, &sFireAudioVec, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        spawnPos2 = self->headPos;

        spawnAngleY = (self->actor.world.rot.y / (f32)0x8000) * M_PI;
        spawnAngleX = (((-self->actor.world.rot.x) / (f32)0x8000) * M_PI) + 0.3f;
        Matrix_RotateY(spawnAngleY, MTXMODE_NEW);
        Matrix_RotateX(spawnAngleX, MTXMODE_APPLY);
        Matrix_MultVec3f(&spawnSpeed2, &spawnVel2);

        BossFd_SpawnFireBreath(self->effects, &spawnPos2, &spawnVel2, &spawnAccel2,
                               50.0f * Math_SinS(self->work[BFD_VAR_TIMER] * 0x2000) + 300.0f, breathOpacity,
                               self->actor.world.rot.y);

        spawnPos2.x += spawnVel2.x * 0.5f;
        spawnPos2.y += spawnVel2.y * 0.5f;
        spawnPos2.z += spawnVel2.z * 0.5f;

        BossFd_SpawnFireBreath(self->effects, &spawnPos2, &spawnVel2, &spawnAccel2,
                               50.0f * Math_SinS(self->work[BFD_VAR_TIMER] * 0x2000) + 300.0f, breathOpacity,
                               self->actor.world.rot.y);
        spawnSpeed2.x = 0.0f;
        spawnSpeed2.y = 17.0f;
        spawnSpeed2.z = 0.0f;

        for (i = 0; i < 6; i++) {
            spawnAngleY = Rand_ZeroFloat(2.0f * M_PI);
            spawnAngleX = Rand_ZeroFloat(2.0f * M_PI);
            Matrix_RotateY(spawnAngleY, MTXMODE_NEW);
            Matrix_RotateX(spawnAngleX, MTXMODE_APPLY);
            Matrix_MultVec3f(&spawnSpeed2, &spawnVel2);

            spawnAccel2.x = (spawnVel2.x * -10) / 100;
            spawnAccel2.y = (spawnVel2.y * -10) / 100;
            spawnAccel2.z = (spawnVel2.z * -10) / 100;

            BossFd_SpawnEmber(self->effects, &self->headPos, &spawnVel2, &spawnAccel2, (s16)Rand_ZeroFloat(2.0f) + 8);
        }
    }

    if ((self->actor.world.pos.y < 90.0f) || (700.0f < self->actor.world.pos.y) || (self->actionFunc == BossFd_Wait)) {
        self->actor.flags &= ~1;
    } else {
        self->actor.flags |= 1;
    }
}

void BossFd_CollisionCheck(BossFd* self, GlobalContext* globalCtx) {
    ColliderJntSphElement* headCollider = &self->collider.elements[0];
    ColliderInfo* hurtbox;

    if (headCollider->info.bumperFlags & BUMP_HIT) {
        headCollider->info.bumperFlags &= ~BUMP_HIT;
        hurtbox = headCollider->info.acHitInfo;
        self->actor.colChkInfo.health -= 2;
        if (hurtbox->toucher.dmgFlags & 0x1000) {
            self->actor.colChkInfo.health -= 2;
        }
        if ((s8)self->actor.colChkInfo.health <= 2) {
            self->actor.colChkInfo.health = 2;
        }
        self->work[BFD_DAMAGE_FLASH_TIMER] = 10;
        self->work[BFD_INVINC_TIMER] = 20;
        Audio_PlaySoundGeneral(NA_SE_EN_VALVAISA_DAMAGE1, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

void BossFd_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossFd* self = THIS;
    f32 headGlow;
    f32 rManeGlow;
    f32 lManeGlow;
    s16 i;

    osSyncPrintf("FD MOVE START \n");
    self->work[BFD_VAR_TIMER]++;
    self->work[BFD_MOVE_TIMER]++;
    self->actionFunc(self, globalCtx);

    for (i = 0; i < ARRAY_COUNT(self->timers); i++) {
        if (self->timers[i] != 0) {
            self->timers[i]--;
        }
    }
    if (self->fireBreathTimer != 0) {
        self->fireBreathTimer--;
    }
    if (self->work[BFD_DAMAGE_FLASH_TIMER] != 0) {
        self->work[BFD_DAMAGE_FLASH_TIMER]--;
    }
    if (self->work[BFD_INVINC_TIMER] != 0) {
        self->work[BFD_INVINC_TIMER]--;
    }
    if (self->work[BFD_ACTION_STATE] < BOSSFD_DEATH_START) {
        if (self->work[BFD_INVINC_TIMER] == 0) {
            BossFd_CollisionCheck(self, globalCtx);
        }
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    BossFd_Effects(self, globalCtx);
    self->fwork[BFD_TEX1_SCROLL_X] += 4.0f;
    self->fwork[BFD_TEX1_SCROLL_Y] = 120.0f;
    self->fwork[BFD_TEX2_SCROLL_X] += 3.0f;
    self->fwork[BFD_TEX2_SCROLL_Y] -= 2.0f;

    Math_ApproachF(&self->fwork[BFD_BODY_TEX2_ALPHA], (self->work[BFD_VAR_TIMER] & 0x10) ? 30.0f : 158.0f, 1.0f, 8.0f);
    if (self->skinSegments == 0) {
        self->fwork[BFD_HEAD_TEX2_ALPHA] = self->fwork[BFD_BODY_TEX2_ALPHA];
    } else {
        headGlow = (self->work[BFD_VAR_TIMER] & 4) ? 0.0f : 255.0f;
        Math_ApproachF(&self->fwork[BFD_HEAD_TEX2_ALPHA], headGlow, 1.0f, 64.0f);
    }

    headGlow = (self->work[BFD_VAR_TIMER] & 8) ? 128.0f : 255.0f;
    rManeGlow = ((self->work[BFD_VAR_TIMER] + 3) & 8) ? 128.0f : 255.0f;
    lManeGlow = ((self->work[BFD_VAR_TIMER] + 6) & 8) ? 128.0f : 255.0f;

    Math_ApproachF(&self->fwork[BFD_MANE_COLOR_CENTER], headGlow, 1.0f, 16.0f);
    Math_ApproachF(&self->fwork[BFD_MANE_COLOR_RIGHT], rManeGlow, 1.0f, 16.0f);
    Math_ApproachF(&self->fwork[BFD_MANE_COLOR_LEFT], lManeGlow, 1.0f, 16.0f);

    if (self->work[BFD_ROCK_TIMER] != 0) {
        self->work[BFD_ROCK_TIMER]--;
        if ((self->work[BFD_ROCK_TIMER] % 16) == 0) {
            EnVbBall* bossFdRock = (EnVbBall*)Actor_SpawnAsChild(
                &globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_VB_BALL, self->actor.world.pos.x, 1000.0f,
                self->actor.world.pos.z, 0, 0, (s16)Rand_ZeroFloat(50.0f) + 130, 100);

            if (bossFdRock != NULL) {
                for (i = 0; i < 10; i++) {
                    Vec3f debrisVel = { 0.0f, 0.0f, 0.0f };
                    Vec3f debrisAccel = { 0.0f, -1.0f, 0.0f };
                    Vec3f debrisPos;

                    debrisPos.x = Rand_CenteredFloat(300.0f) + bossFdRock->actor.world.pos.x;
                    debrisPos.y = Rand_CenteredFloat(300.0f) + bossFdRock->actor.world.pos.y;
                    debrisPos.z = Rand_CenteredFloat(300.0f) + bossFdRock->actor.world.pos.z;

                    BossFd_SpawnDebris(self->effects, &debrisPos, &debrisVel, &debrisAccel,
                                       (s16)Rand_ZeroFloat(15.0f) + 20);
                }
            }
        }
    }

    if (1) { // Needed for matching, and also to define new variables
        Vec3f emberVel = { 0.0f, 0.0f, 0.0f };
        Vec3f emberAccel = { 0.0f, 0.0f, 0.0f };
        Vec3f emberPos;
        s16 temp_rand;

        for (i = 0; i < 6; i++) {
            emberAccel.y = 0.4f;
            emberAccel.x = Rand_CenteredFloat(0.5f);
            emberAccel.z = Rand_CenteredFloat(0.5f);

            temp_rand = Rand_ZeroFloat(8.9f);

            emberPos.x = sHoleLocations[temp_rand].x + Rand_CenteredFloat(60.0f);
            emberPos.y = (sHoleLocations[temp_rand].y + 10.0f) + Rand_ZeroFloat(40.0f);
            emberPos.z = sHoleLocations[temp_rand].z + Rand_CenteredFloat(60.0f);

            BossFd_SpawnEmber(self->effects, &emberPos, &emberVel, &emberAccel, (s16)Rand_ZeroFloat(2.0f) + 6);
        }

        if (self->skinSegments != 0) {
            for (i = 0; i < (s16)self->fwork[BFD_MANE_EMBER_RATE]; i++) {
                temp_rand = Rand_ZeroFloat(29.9f);
                emberPos.y = self->centerMane.pos[temp_rand].y + Rand_CenteredFloat(20.0f);

                if (emberPos.y >= 90.0f) {
                    emberPos.x = self->centerMane.pos[temp_rand].x + Rand_CenteredFloat(20.0f);
                    emberPos.z = self->centerMane.pos[temp_rand].z + Rand_CenteredFloat(20.0f);

                    emberVel.x = Rand_CenteredFloat(self->fwork[BFD_MANE_EMBER_SPEED]);
                    emberVel.y = Rand_CenteredFloat(self->fwork[BFD_MANE_EMBER_SPEED]);
                    emberVel.z = Rand_CenteredFloat(self->fwork[BFD_MANE_EMBER_SPEED]);

                    emberAccel.y = 0.4f;
                    emberAccel.x = Rand_CenteredFloat(0.5f);
                    emberAccel.z = Rand_CenteredFloat(0.5f);

                    BossFd_SpawnEmber(self->effects, &emberPos, &emberVel, &emberAccel, (s16)Rand_ZeroFloat(2.0f) + 8);
                }
            }
        }
    }
    osSyncPrintf("FD MOVE END 1\n");
    BossFd_UpdateEffects(self, globalCtx);
    osSyncPrintf("FD MOVE END 2\n");
}

void BossFd_UpdateEffects(BossFd* self, GlobalContext* globalCtx) {
    BossFdEffect* effect = self->effects;
    Player* player = GET_PLAYER(globalCtx);
    Color_RGB8 colors[4] = { { 255, 128, 0 }, { 255, 0, 0 }, { 255, 255, 0 }, { 255, 0, 0 } };
    Vec3f diff;
    s16 i1;
    s16 i2;

    for (i1 = 0; i1 < 180; i1++, effect++) {
        if (effect->type != BFD_FX_NONE) {
            effect->timer1++;

            effect->pos.x += effect->velocity.x;
            effect->pos.y += effect->velocity.y;
            effect->pos.z += effect->velocity.z;

            effect->velocity.x += effect->accel.x;
            effect->velocity.y += effect->accel.y;
            effect->velocity.z += effect->accel.z;
            if (effect->type == BFD_FX_EMBER) {
                s16 cInd = effect->timer1 % 4;

                effect->color.r = colors[cInd].r;
                effect->color.g = colors[cInd].g;
                effect->color.b = colors[cInd].b;
                effect->alpha -= 20;
                if (effect->alpha <= 0) {
                    effect->alpha = 0;
                    effect->type = 0;
                }
            } else if ((effect->type == BFD_FX_DEBRIS) || (effect->type == BFD_FX_SKULL_PIECE)) {
                effect->vFdFxRotX += 0.55f;
                effect->vFdFxRotY += 0.1f;
                if (effect->pos.y <= 100.0f) {
                    effect->type = 0;
                }
            } else if (effect->type == BFD_FX_DUST) {
                if (effect->timer2 >= 8) {
                    effect->timer2 = 8;
                    effect->type = 0;
                } else if (((effect->timer1 % 2) != 0) || (Rand_ZeroOne() < 0.3f)) {
                    effect->timer2++;
                }
            } else if (effect->type == BFD_FX_FIRE_BREATH) {
                diff.x = player->actor.world.pos.x - effect->pos.x;
                diff.y = player->actor.world.pos.y + 30.0f - effect->pos.y;
                diff.z = player->actor.world.pos.z - effect->pos.z;
                if ((self->timers[3] == 0) && (sqrtf(SQ(diff.x) + SQ(diff.y) + SQ(diff.z)) < 20.0f)) {
                    self->timers[3] = 50;
                    func_8002F6D4(globalCtx, NULL, 5.0f, effect->kbAngle, 0.0f, 0x30);
                    if (player->isBurning == false) {
                        for (i2 = 0; i2 < ARRAY_COUNT(player->flameTimers); i2++) {
                            player->flameTimers[i2] = Rand_S16Offset(0, 200);
                        }
                        player->isBurning = true;
                    }
                }
                if (effect->timer2 == 0) {
                    if (effect->scale < 2.5f) {
                        effect->scale += effect->vFdFxScaleMod;
                        effect->vFdFxScaleMod += 0.08f;
                    }
                    if ((effect->pos.y <= (effect->vFdFxYStop + 130.0f)) || (effect->timer1 >= 10)) {
                        effect->accel.y = 5.0f;
                        effect->timer2++;
                        effect->velocity.y = 0.0f;
                        effect->accel.x = (effect->velocity.x * -25.0f) / 100.0f;
                        effect->accel.z = (effect->velocity.z * -25.0f) / 100.0f;
                    }
                } else {
                    if (effect->scale < 2.5f) {
                        Math_ApproachF(&effect->scale, 2.5f, 0.5f, 0.5f);
                    }
                    effect->timer2++;
                    if (effect->timer2 >= 9) {
                        effect->type = 0;
                    }
                }
            }
        }
    }
}

void BossFd_DrawEffects(BossFdEffect* effect, GlobalContext* globalCtx) {
    static void* dustTex[] = {
        gDust1Tex, gDust1Tex, gDust2Tex, gDust3Tex, gDust4Tex, gDust5Tex, gDust6Tex, gDust7Tex, gDust8Tex,
    };
    u8 flag = false;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s16 i;
    BossFdEffect* firstEffect = effect;

    OPEN_DISPS(gfxCtx, "../z_boss_fd.c", 4023);

    for (i = 0; i < 180; i++, effect++) {
        if (effect->type == BFD_FX_EMBER) {
            if (!flag) {
                func_80093D84(globalCtx->state.gfxCtx);
                gSPDisplayList(POLY_XLU_DISP++, gVolvagiaEmberSetupDL);
                flag++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, effect->color.r, effect->color.g, effect->color.b, effect->alpha);
            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(effect->scale, effect->scale, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_fd.c", 4046),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gVolvagiaEmberVtxDL);
        }
    }

    effect = firstEffect;
    flag = false;
    for (i = 0; i < 180; i++, effect++) {
        if (effect->type == BFD_FX_DEBRIS) {
            if (!flag) {
                func_80093D18(globalCtx->state.gfxCtx);
                gSPDisplayList(POLY_OPA_DISP++, gVolvagiaDebrisSetupDL);
                flag++;
            }

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_RotateY(effect->vFdFxRotY, MTXMODE_APPLY);
            Matrix_RotateX(effect->vFdFxRotX, MTXMODE_APPLY);
            Matrix_Scale(effect->scale, effect->scale, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_fd.c", 4068),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gVolvagiaDebrisVtxDL);
        }
    }

    effect = firstEffect;
    flag = false;
    for (i = 0; i < 180; i++, effect++) {
        if (effect->type == BFD_FX_DUST) {
            if (!flag) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);
                gSPDisplayList(POLY_XLU_DISP++, gVolvagiaDustSetupDL);
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 90, 30, 0, 255);
                gDPSetEnvColor(POLY_XLU_DISP++, 90, 30, 0, 0);
                flag++;
            }

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);
            func_800D1FD4(&globalCtx->mf_11DA0);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_fd.c", 4104),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(dustTex[effect->timer2]));
            gSPDisplayList(POLY_XLU_DISP++, gVolvagiaDustVtxDL);
        }
    }

    effect = firstEffect;
    flag = false;
    for (i = 0; i < 180; i++, effect++) {
        if (effect->type == BFD_FX_FIRE_BREATH) {
            if (!flag) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);
                gSPDisplayList(POLY_XLU_DISP++, gVolvagiaDustSetupDL);
                gDPSetEnvColor(POLY_XLU_DISP++, 255, 10, 0, 255);
                flag++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 0, effect->alpha);
            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_Scale(effect->scale, effect->scale, effect->scale, MTXMODE_APPLY);
            func_800D1FD4(&globalCtx->mf_11DA0);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_fd.c", 4154),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(dustTex[effect->timer2]));
            gSPDisplayList(POLY_XLU_DISP++, gVolvagiaDustVtxDL);
        }
    }

    effect = firstEffect;
    flag = false;
    for (i = 0; i < 180; i++, effect++) {
        if (effect->type == BFD_FX_SKULL_PIECE) {
            if (!flag) {
                func_80093D84(globalCtx->state.gfxCtx);
                gSPDisplayList(POLY_XLU_DISP++, gVolvagiaSkullPieceSetupDL);
                flag++;
            }

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_RotateY(effect->vFdFxRotY, MTXMODE_APPLY);
            Matrix_RotateX(effect->vFdFxRotX, MTXMODE_APPLY);
            Matrix_Scale(effect->scale, effect->scale, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_fd.c", 4192),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gVolvagiaSkullPieceVtxDL);
        }
    }

    CLOSE_DISPS(gfxCtx, "../z_boss_fd.c", 4198);
}

void BossFd_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossFd* self = THIS;

    osSyncPrintf("FD DRAW START\n");
    if (self->actionFunc != BossFd_Wait) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_fd.c", 4217);
        func_80093D18(globalCtx->state.gfxCtx);
        if (self->work[BFD_DAMAGE_FLASH_TIMER] & 2) {
            POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 255, 255, 0, 900, 1099);
        }

        BossFd_DrawBody(globalCtx, self);
        POLY_OPA_DISP = Gameplay_SetFog(globalCtx, POLY_OPA_DISP);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_fd.c", 4243);
    }

    osSyncPrintf("FD DRAW END\n");
    BossFd_DrawEffects(self->effects, globalCtx);
    osSyncPrintf("FD DRAW END2\n");
}

s32 BossFd_OverrideRightArmDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx) {
    BossFd* self = THIS;

    switch (limbIndex) {
        case 1:
            rot->y += 4000.0f + self->rightArmRot[0].x;
            break;
        case 2:
            rot->y += self->rightArmRot[1].x;
            rot->z += self->rightArmRot[1].y;
            break;
        case 3:
            rot->y += self->rightArmRot[2].x;
            rot->z += self->rightArmRot[2].y;
            break;
    }
    if (self->skinSegments < limbIndex) {
        *dList = NULL;
    }
    return false;
}

s32 BossFd_OverrideLeftArmDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                               void* thisx) {
    BossFd* self = THIS;

    switch (limbIndex) {
        case 1:
            rot->y += -4000.0f + self->leftArmRot[0].x;
            break;
        case 2:
            rot->y += self->leftArmRot[1].x;
            rot->z += self->leftArmRot[1].y;
            break;
        case 3:
            rot->y += self->leftArmRot[2].x;
            rot->z += self->leftArmRot[2].y;
            break;
    }
    if (self->skinSegments < limbIndex) {
        *dList = NULL;
    }
    return false;
}

static s16 sBodyIndex[] = { 0, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5 };
static s16 sManeIndex[] = { 0, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10 }; // Unused

void BossFd_DrawMane(GlobalContext* globalCtx, BossFd* self, Vec3f* manePos, Vec3f* maneRot, f32* maneScale, u8 mode) {
    f32 sp140[] = { 0.0f, 10.0f, 17.0f, 20.0f, 19.5f, 18.0f, 17.0f, 15.0f, 15.0f, 15.0f };
    f32 sp118[] = { 0.0f, 10.0f, 17.0f, 20.0f, 21.0f, 21.0f, 21.0f, 21.0f, 21.0f, 21.0f };
    f32 spF0[] = { 0.4636457f, 0.33661291f, 0.14879614f, 0.04995025f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    // arctan of {0.5, 0.35, 0.15, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    f32 spC8[] = { -0.4636457f, -0.33661291f, -0.14879614f, 0.024927188f, 0.07478157f,
                   0.04995025f, 0.09961288f,  0.0f,         0.0f,         0.0f };
    // arctan of {-0.5, -0.35, -0.15, 0.025, 0.075, 0.05, 0.1, 0.0, 0.0}
    s16 maneIndex;
    s16 i;
    s16 maneLength;
    Vec3f spB4;
    Vec3f spA8;
    f32 phi_f20;
    f32 phi_f22;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_fd.c", 4419);

    maneLength = self->skinSegments;
    maneLength = CLAMP_MAX(maneLength, 10);

    for (i = 0; i < maneLength; i++) {
        maneIndex = (self->work[BFD_LEAD_MANE_SEG] - (i * 2) + 30) % 30;

        if (mode == 0) {
            spB4.x = spB4.z = 0.0f;
            spB4.y = ((sp140[i] * 0.1f) * 10.0f) * self->flattenMane;
            phi_f20 = 0.0f;
            phi_f22 = spC8[i] * self->flattenMane;
        } else if (mode == 1) {
            phi_f22 = (spC8[i] * self->flattenMane) * 0.7f;
            phi_f20 = spF0[i] * self->flattenMane;

            spB4.y = (sp140[i] * self->flattenMane) * 0.7f;
            spB4.x = -sp118[i] * self->flattenMane;
            spB4.z = 0.0f;
        } else {
            phi_f22 = (spC8[i] * self->flattenMane) * 0.7f;
            phi_f20 = -spF0[i] * self->flattenMane;

            spB4.y = (sp140[i] * self->flattenMane) * 0.7f;
            spB4.x = sp118[i] * self->flattenMane;
            spB4.z = 0.0f;
        }

        Matrix_RotateY((maneRot + maneIndex)->y, MTXMODE_NEW);
        Matrix_RotateX(-(maneRot + maneIndex)->x, MTXMODE_APPLY);

        Matrix_MultVec3f(&spB4, &spA8);

        Matrix_Translate((manePos + maneIndex)->x + spA8.x, (manePos + maneIndex)->y + spA8.y,
                         (manePos + maneIndex)->z + spA8.z, MTXMODE_NEW);
        Matrix_RotateY((maneRot + maneIndex)->y + phi_f20, MTXMODE_APPLY);
        Matrix_RotateX(-((maneRot + maneIndex)->x + phi_f22), MTXMODE_APPLY);
        Matrix_Scale(maneScale[maneIndex] * (0.01f - (i * 0.0008f)), maneScale[maneIndex] * (0.01f - (i * 0.0008f)),
                     0.01f, MTXMODE_APPLY);
        Matrix_RotateX(-M_PI / 2.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_fd.c", 4480),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gVolvagiaManeVtxDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_fd.c", 4483);
}

s32 BossFd_OverrideHeadDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    BossFd* self = THIS;

    switch (limbIndex) {
        case 5:
        case 6:
            rot->z -= self->jawOpening * 0.1f;
            break;
        case 2:
            rot->z += self->jawOpening;
            break;
    }
    if ((self->faceExposed == true) && (limbIndex == 5)) {
        *dList = gVolvagiaBrokenFaceDL;
    }
    if (self->skinSegments == 0) {
        if (limbIndex == 6) {
            *dList = gVolvagiaSkullDL;
        } else if (limbIndex == 2) {
            *dList = gVolvagiaJawboneDL;
        } else {
            *dList = NULL;
        }
    }
    return false;
}

void BossFd_PostHeadDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f targetMod = { 4500.0f, 0.0f, 0.0f };
    static Vec3f headMod = { 4000.0f, 0.0f, 0.0f };
    BossFd* self = THIS;

    if (limbIndex == 5) {
        Matrix_MultVec3f(&targetMod, &self->actor.focus.pos);
        Matrix_MultVec3f(&headMod, &self->headPos);
    }
}

static void* sEyeTextures[] = {
    gVolvagiaEyeOpenTex,
    gVolvagiaEyeHalfTex,
    gVolvagiaEyeClosedTex,
};

static Gfx* sBodyDLists[] = {
    gVolvagiaBodySeg1DL,  gVolvagiaBodySeg2DL,  gVolvagiaBodySeg3DL,  gVolvagiaBodySeg4DL,  gVolvagiaBodySeg5DL,
    gVolvagiaBodySeg6DL,  gVolvagiaBodySeg7DL,  gVolvagiaBodySeg8DL,  gVolvagiaBodySeg9DL,  gVolvagiaBodySeg10DL,
    gVolvagiaBodySeg11DL, gVolvagiaBodySeg12DL, gVolvagiaBodySeg13DL, gVolvagiaBodySeg14DL, gVolvagiaBodySeg15DL,
    gVolvagiaBodySeg16DL, gVolvagiaBodySeg17DL, gVolvagiaBodySeg18DL,
};

void BossFd_DrawBody(GlobalContext* globalCtx, BossFd* self) {
    s16 segIndex;
    s16 i;
    f32 temp_float;
    Mtx* tempMat = Graph_Alloc(globalCtx->state.gfxCtx, 18 * sizeof(Mtx));

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_fd.c", 4589);
    if (self->skinSegments != 0) {
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeState]));
    }
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (s16)self->fwork[BFD_TEX1_SCROLL_X],
                                (s16)self->fwork[BFD_TEX1_SCROLL_Y], 0x20, 0x20, 1, (s16)self->fwork[BFD_TEX2_SCROLL_X],
                                (s16)self->fwork[BFD_TEX2_SCROLL_Y], 0x20, 0x20));
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, (s8)self->fwork[BFD_BODY_TEX2_ALPHA]);

    osSyncPrintf("LH\n");
    Matrix_Push();
    segIndex = (self->work[BFD_LEAD_BODY_SEG] + sBodyIndex[2]) % 100;
    Matrix_Translate(self->bodySegsPos[segIndex].x, self->bodySegsPos[segIndex].y, self->bodySegsPos[segIndex].z,
                     MTXMODE_NEW);
    Matrix_RotateY(self->bodySegsRot[segIndex].y, MTXMODE_APPLY);
    Matrix_RotateX(-self->bodySegsRot[segIndex].x, MTXMODE_APPLY);
    Matrix_Translate(-13.0f, -5.0f, 13.0f, MTXMODE_APPLY);
    Matrix_Scale(self->actor.scale.x * 0.1f, self->actor.scale.y * 0.1f, self->actor.scale.z * 0.1f, MTXMODE_APPLY);
    SkelAnime_DrawOpa(globalCtx, self->skelAnimeRightArm.skeleton, self->skelAnimeRightArm.jointTable,
                      BossFd_OverrideRightArmDraw, NULL, self);
    Matrix_Pop();
    osSyncPrintf("RH\n");
    Matrix_Push();
    segIndex = (self->work[BFD_LEAD_BODY_SEG] + sBodyIndex[2]) % 100;
    Matrix_Translate(self->bodySegsPos[segIndex].x, self->bodySegsPos[segIndex].y, self->bodySegsPos[segIndex].z,
                     MTXMODE_NEW);
    Matrix_RotateY(self->bodySegsRot[segIndex].y, MTXMODE_APPLY);
    Matrix_RotateX(-self->bodySegsRot[segIndex].x, MTXMODE_APPLY);
    Matrix_Translate(13.0f, -5.0f, 13.0f, MTXMODE_APPLY);
    Matrix_Scale(self->actor.scale.x * 0.1f, self->actor.scale.y * 0.1f, self->actor.scale.z * 0.1f, MTXMODE_APPLY);
    SkelAnime_DrawOpa(globalCtx, self->skelAnimeLeftArm.skeleton, self->skelAnimeLeftArm.jointTable,
                      BossFd_OverrideLeftArmDraw, NULL, self);
    Matrix_Pop();
    osSyncPrintf("BD\n");
    gSPSegment(POLY_OPA_DISP++, 0x0D, tempMat);

    Matrix_Push();
    for (i = 0; i < 18; i++, tempMat++) {
        segIndex = (self->work[BFD_LEAD_BODY_SEG] + sBodyIndex[i + 1]) % 100;
        Matrix_Translate(self->bodySegsPos[segIndex].x, self->bodySegsPos[segIndex].y, self->bodySegsPos[segIndex].z,
                         MTXMODE_NEW);
        Matrix_RotateY(self->bodySegsRot[segIndex].y, MTXMODE_APPLY);
        Matrix_RotateX(-self->bodySegsRot[segIndex].x, MTXMODE_APPLY);
        Matrix_Translate(0.0f, 0.0f, 35.0f, MTXMODE_APPLY);
        Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
        if (i < self->skinSegments) {
            Matrix_Scale(1.0f + (Math_SinS((self->work[BFD_LEAD_BODY_SEG] * 5000.0f) + (i * 7000.0f)) *
                                 self->fwork[BFD_BODY_PULSE]),
                         1.0f + (Math_SinS((self->work[BFD_LEAD_BODY_SEG] * 5000.0f) + (i * 7000.0f)) *
                                 self->fwork[BFD_BODY_PULSE]),
                         1.0f, MTXMODE_APPLY);
            Matrix_RotateY(M_PI / 2.0f, MTXMODE_APPLY);
            Matrix_ToMtx(tempMat, "../z_boss_fd.c", 4719);
            gSPMatrix(POLY_OPA_DISP++, tempMat, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, sBodyDLists[i]);
        } else {
            MtxF spFC;
            Vec3f spF0 = { 0.0f, 0.0f, 0.0f };
            Vec3f spE4;
            Vec3s spDC;
            f32 padD8;

            if (self->bodyFallApart[i] < 2) {
                f32 spD4 = 0.1f;

                temp_float = 0.1f;
                Matrix_Translate(0.0f, 0.0f, -1100.0f, MTXMODE_APPLY);
                Matrix_RotateY(-M_PI, MTXMODE_APPLY);
                if (i >= 14) {
                    f32 sp84 = 1.0f - ((i - 14) * 0.2f);

                    Matrix_Scale(sp84, sp84, 1.0f, MTXMODE_APPLY);
                    spD4 = 0.1f * sp84;
                    temp_float = 0.1f * sp84;
                }
                Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);
                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_fd.c", 4768),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_OPA_DISP++, gVolvagiaRibsDL);

                if (self->bodyFallApart[i] == 1) {
                    EnVbBall* bones;

                    self->bodyFallApart[i] = 2;
                    Matrix_MultVec3f(&spF0, &spE4);
                    Matrix_Get(&spFC);
                    Matrix_MtxFToYXZRotS(&spFC, &spDC, 0);
                    bones =
                        (EnVbBall*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_VB_BALL,
                                                      spE4.x, spE4.y, spE4.z, spDC.x, spDC.y, spDC.z, i + 200);

                    bones->actor.scale.x = self->actor.scale.x * temp_float;
                    bones->actor.scale.y = self->actor.scale.y * spD4;
                    bones->actor.scale.z = self->actor.scale.z * 0.1f;
                }
            }
        }
        if (i > 0) {
            Collider_UpdateSpheres(i + 1, &self->collider);
        }
    }
    Matrix_Pop();
    osSyncPrintf("BH\n");

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, (s8)self->fwork[BFD_HEAD_TEX2_ALPHA]);
    Matrix_Push();
    temp_float =
        (self->work[BFD_ACTION_STATE] >= BOSSFD_SKULL_FALL) ? -20.0f : -10.0f - ((self->actor.speedXZ - 5.0f) * 10.0f);
    segIndex = (self->work[BFD_LEAD_BODY_SEG] + sBodyIndex[0]) % 100;
    Matrix_Translate(self->bodySegsPos[segIndex].x, self->bodySegsPos[segIndex].y, self->bodySegsPos[segIndex].z,
                     MTXMODE_NEW);
    Matrix_RotateY(self->bodySegsRot[segIndex].y, MTXMODE_APPLY);
    Matrix_RotateX(-self->bodySegsRot[segIndex].x, MTXMODE_APPLY);
    Matrix_RotateZ((self->actor.shape.rot.z / (f32)0x8000) * M_PI, MTXMODE_APPLY);
    Matrix_Translate(0.0f, 0.0f, temp_float, MTXMODE_APPLY);
    Matrix_Push();
    Matrix_Translate(0.0f, 0.0f, 25.0f, MTXMODE_APPLY);
    osSyncPrintf("BHC\n");
    Collider_UpdateSpheres(0, &self->collider);
    Matrix_Pop();
    osSyncPrintf("BHCE\n");
    Matrix_Scale(self->actor.scale.x * 0.1f, self->actor.scale.y * 0.1f, self->actor.scale.z * 0.1f, MTXMODE_APPLY);
    SkelAnime_DrawOpa(globalCtx, self->skelAnimeHead.skeleton, self->skelAnimeHead.jointTable, BossFd_OverrideHeadDraw,
                      BossFd_PostHeadDraw, &self->actor);
    osSyncPrintf("SK\n");
    {
        Vec3f spB0 = { 0.0f, 1700.0f, 7000.0f };
        Vec3f spA4 = { -1000.0f, 700.0f, 7000.0f };

        func_80093D84(globalCtx->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, gVolvagiaManeSetupDL);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, self->fwork[BFD_MANE_COLOR_CENTER], 0, 255);
        Matrix_Push();
        Matrix_MultVec3f(&spB0, &self->centerMane.head);
        BossFd_DrawMane(globalCtx, self, self->centerMane.pos, self->fireManeRot, self->centerMane.scale, MANE_CENTER);
        Matrix_Pop();
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, self->fwork[BFD_MANE_COLOR_RIGHT], 0, 255);
        Matrix_Push();
        Matrix_MultVec3f(&spA4, &self->rightMane.head);
        BossFd_DrawMane(globalCtx, self, self->rightMane.pos, self->fireManeRot, self->rightMane.scale, MANE_RIGHT);
        Matrix_Pop();
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, self->fwork[BFD_MANE_COLOR_LEFT], 0, 255);
        Matrix_Push();
        spA4.x *= -1.0f;
        Matrix_MultVec3f(&spA4, &self->leftMane.head);
        BossFd_DrawMane(globalCtx, self, self->leftMane.pos, self->fireManeRot, self->leftMane.scale, MANE_LEFT);
        Matrix_Pop();
    }

    Matrix_Pop();
    osSyncPrintf("END\n");
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_fd.c", 4987);
}
