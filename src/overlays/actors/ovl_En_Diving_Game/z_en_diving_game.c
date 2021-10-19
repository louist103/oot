/*
 * File: z_en_diving_game.c
 * Overlay: ovl_En_Diving_Game
 * Description: Diving minigame
 */

#include "z_en_diving_game.h"
#include "overlays/actors/ovl_En_Ex_Ruppy/z_en_ex_ruppy.h"
#include "objects/object_zo/object_zo.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnDivingGame*)thisx)

void EnDivingGame_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDivingGame_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDivingGame_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDivingGame_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809EDCB0(EnDivingGame* self, GlobalContext* globalCtx);
void EnDivingGame_Talk(EnDivingGame* self, GlobalContext* globalCtx);
void EnDivingGame_HandlePlayChoice(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EE048(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EE0FC(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EE194(EnDivingGame* self, GlobalContext* globalCtx);
void EnDivingGame_SetupRupeeThrow(EnDivingGame* self, GlobalContext* globalCtx);
void EnDivingGame_RupeeThrow(EnDivingGame* self, GlobalContext* globalCtx);
void EnDivingGame_SetupUnderwaterViewCs(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EE780(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EE800(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EE8F0(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EE96C(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EEA00(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EEA90(EnDivingGame* self, GlobalContext* globalCtx);
void func_809EEAF8(EnDivingGame* self, GlobalContext* globalCtx);

const ActorInit En_Diving_Game_InitVars = {
    ACTOR_EN_DIVING_GAME,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZO,
    sizeof(EnDivingGame),
    (ActorFunc)EnDivingGame_Init,
    (ActorFunc)EnDivingGame_Destroy,
    (ActorFunc)EnDivingGame_Update,
    (ActorFunc)EnDivingGame_Draw,
};

// used to ensure there's only one instance of self actor.
static u8 D_809EF0B0 = false;

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 10, 10, 0, { 0, 0, 0 } },
};

static void* sEyeTextures[] = {
    gZoraEyeOpenTex,
    gZoraEyeHalfTex,
    gZoraEyeClosedTex,
};

void EnDivingGame_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDivingGame* self = THIS;

    self->actor.gravity = -3.0f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gZoraSkel, &gZoraIdleAnim, self->jointTable, self->morphTable, 20);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 素もぐりＧＯ ☆☆☆☆☆ \n" VT_RST);
    self->actor.room = -1;
    self->actor.scale.x = 0.01f;
    self->actor.scale.y = 0.012999999f;
    self->actor.scale.z = 0.0139999995f;
    if (D_809EF0B0) {
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ もういてる原 ☆☆☆☆☆ \n" VT_RST);
        self->unk_31F = 1;
        Actor_Kill(&self->actor);
    } else {
        D_809EF0B0 = true;
        self->actor.targetMode = 0;
        self->actor.colChkInfo.mass = MASS_IMMOVABLE;
        self->actionFunc = func_809EDCB0;
    }
}

void EnDivingGame_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDivingGame* self = THIS;

    if (self->unk_31F == 0) {
        gSaveContext.timer1State = 0;
    }
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnDivingGame_SpawnRuppy(EnDivingGame* self, GlobalContext* globalCtx) {
    EnExRuppy* rupee;
    Vec3f rupeePos;

    rupeePos.x = (Rand_ZeroOne() - 0.5f) * 30.0f + self->actor.world.pos.x;
    rupeePos.y = (Rand_ZeroOne() - 0.5f) * 20.0f + (self->actor.world.pos.y + 30.0f);
    rupeePos.z = (Rand_ZeroOne() - 0.5f) * 20.0f + self->actor.world.pos.z;
    rupee = (EnExRuppy*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_EX_RUPPY, rupeePos.x,
                                           rupeePos.y, rupeePos.z, 0, (s16)Rand_CenteredFloat(3500.0f) - 1000,
                                           self->rupeesLeftToThrow, 0);
    if (rupee != NULL) {
        rupee->actor.speedXZ = 12.0f;
        rupee->actor.velocity.y = 6.0f;
    }
}

s32 EnDivingGame_HasMinigameFinished(EnDivingGame* self, GlobalContext* globalCtx) {
    if (gSaveContext.timer1State == 10 && !Gameplay_InCsMode(globalCtx)) {
        // Failed.
        gSaveContext.timer1State = 0;
        func_800F5B58();
        func_80078884(NA_SE_SY_FOUND);
        self->actor.textId = 0x71AD;
        func_8010B680(globalCtx, self->actor.textId, NULL);
        self->unk_292 = 5;
        self->allRupeesThrown = self->state = self->phase = self->unk_2A2 = self->grabbedRupeesCounter = 0;
        func_8002DF54(globalCtx, NULL, 8);
        self->actionFunc = func_809EE048;
        return true;
    } else {
        s32 rupeesNeeded = 5;

        if (gSaveContext.eventChkInf[3] & 0x100) {
            rupeesNeeded = 10;
        }
        if (self->grabbedRupeesCounter >= rupeesNeeded) {
            // Won.
            gSaveContext.timer1State = 0;
            self->allRupeesThrown = self->state = self->phase = self->unk_2A2 = self->grabbedRupeesCounter = 0;
            if (!(gSaveContext.eventChkInf[3] & 0x100)) {
                self->actor.textId = 0x4055;
            } else {
                self->actor.textId = 0x405D;
                if (self->extraWinCount < 100) {
                    self->extraWinCount++;
                }
            }
            func_8010B680(globalCtx, self->actor.textId, NULL);
            self->unk_292 = 5;
            func_800F5B58();
            func_800F5C64(0x39);
            func_8002DF54(globalCtx, NULL, 8);
            if (!(gSaveContext.eventChkInf[3] & 0x100)) {
                self->actionFunc = func_809EE96C;
            } else {
                self->actionFunc = func_809EE048;
            }
            return true;
        }
    }
    return false;
}

// EnDivingGame_FinishMinigame ? // Reset probably
void func_809EDCB0(EnDivingGame* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gZoraIdleAnim);

    Animation_Change(&self->skelAnime, &gZoraIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->notPlayingMinigame = true;
    self->actionFunc = EnDivingGame_Talk;
}

void EnDivingGame_Talk(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->state != ENDIVINGGAME_STATE_PLAYING || !EnDivingGame_HasMinigameFinished(self, globalCtx)) {
        if (func_8002F194(&self->actor, globalCtx)) {
            if (self->unk_292 != 6) {
                switch (self->state) {
                    case ENDIVINGGAME_STATE_NOTPLAYING:
                        func_8002DF54(globalCtx, NULL, 8);
                        self->actionFunc = EnDivingGame_HandlePlayChoice;
                        break;
                    case ENDIVINGGAME_STATE_AWARDPRIZE:
                        self->actionFunc = func_809EEA00;
                        break;
                    case ENDIVINGGAME_STATE_PLAYING:
                        self->actionFunc = func_809EE8F0;
                        break;
                }
            }
        } else {
            if (Text_GetFaceReaction(globalCtx, 0x1D) != 0) {
                self->actor.textId = Text_GetFaceReaction(globalCtx, 0x1D);
                self->unk_292 = 6;
            } else {
                switch (self->state) {
                    case ENDIVINGGAME_STATE_NOTPLAYING:
                        self->unk_292 = 4;
                        if (!(gSaveContext.eventChkInf[3] & 0x100)) {
                            self->actor.textId = 0x4053;
                            self->phase = ENDIVINGGAME_PHASE_1;
                        } else {
                            self->actor.textId = 0x405C;
                            self->phase = ENDIVINGGAME_PHASE_2;
                        }
                        break;
                    case ENDIVINGGAME_STATE_AWARDPRIZE:
                        self->actor.textId = 0x4056;
                        self->unk_292 = 5;
                        break;
                    case ENDIVINGGAME_STATE_PLAYING:
                        self->actor.textId = 0x405B;
                        self->unk_292 = 5;
                        break;
                }
            }
            func_8002F2CC(&self->actor, globalCtx, 80.0f);
        }
    }
}

void EnDivingGame_HandlePlayChoice(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_292 == func_8010BDBC(&globalCtx->msgCtx) &&
        func_80106BC8(globalCtx)) { // Did player selected an answer?
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // Yes
                if (gSaveContext.rupees >= 20) {
                    Rupees_ChangeBy(-20);
                    self->actor.textId = 0x4054;
                } else {
                    self->actor.textId = 0x85;
                    self->allRupeesThrown = self->state = self->phase = self->unk_2A2 = self->grabbedRupeesCounter = 0;
                }
                break;
            case 1: // No
                self->actor.textId = 0x2D;
                self->allRupeesThrown = self->state = self->phase = self->unk_2A2 = self->grabbedRupeesCounter = 0;
                break;
        }
        if (!(gSaveContext.eventChkInf[3] & 0x100) || self->actor.textId == 0x85 || self->actor.textId == 0x2D) {
            func_8010B720(globalCtx, self->actor.textId);
            self->unk_292 = 5;
            self->actionFunc = func_809EE048;
        } else {
            globalCtx->msgCtx.msgMode = 0x37;
            func_8002DF54(globalCtx, NULL, 8);
            self->actionFunc = func_809EE0FC;
        }
    }
}

// Waits for the message to close
void func_809EE048(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_292 == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx)) {
        if (self->phase == ENDIVINGGAME_PHASE_ENDED) {
            func_80106CCC(globalCtx);
            func_8002DF54(globalCtx, NULL, 7);
            self->actionFunc = func_809EDCB0;
        } else {
            globalCtx->msgCtx.msgMode = 0x37;
            func_8002DF54(globalCtx, NULL, 8);
            self->actionFunc = func_809EE0FC;
        }
    }
}

// another "start minigame" step
void func_809EE0FC(EnDivingGame* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gZoraThrowRupeesAnim);

    Animation_Change(&self->skelAnime, &gZoraThrowRupeesAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_ONCE, -10.0f);
    self->notPlayingMinigame = false;
    self->actionFunc = func_809EE194;
}

// Wait a bit before start throwing the rupees.
void func_809EE194(EnDivingGame* self, GlobalContext* globalCtx) {
    f32 currentFrame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if (currentFrame >= 15.0f) {
        self->actionFunc = EnDivingGame_SetupRupeeThrow;
    }
}

void EnDivingGame_SetupRupeeThrow(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    self->subCamId = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, 0, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, self->subCamId, CAM_STAT_ACTIVE);
    self->spawnRuppyTimer = 10;
    self->unk_2F4.x = -210.0f;
    self->unk_2F4.y = -80.0f;
    self->unk_2F4.z = -1020.0f;
    self->unk_2D0.x = -280.0f;
    self->unk_2D0.y = -20.0f;
    self->unk_2D0.z = -240.0f;
    if (!(gSaveContext.eventChkInf[3] & 0x100)) {
        self->rupeesLeftToThrow = 5;
    } else {
        self->rupeesLeftToThrow = 10;
    }
    self->unk_2DC.x = self->unk_2DC.y = self->unk_2DC.z = self->unk_300.x = self->unk_300.y = self->unk_300.z = 0.1f;
    self->camLookAt.x = globalCtx->view.lookAt.x;
    self->camLookAt.y = globalCtx->view.lookAt.y;
    self->camLookAt.z = globalCtx->view.lookAt.z;
    self->camEye.x = globalCtx->view.eye.x;
    self->camEye.y = globalCtx->view.eye.y + 80.0f;
    self->camEye.z = globalCtx->view.eye.z + 250.0f;
    self->unk_2E8.x = fabsf(self->camEye.x - self->unk_2D0.x) * 0.04f;
    self->unk_2E8.y = fabsf(self->camEye.y - self->unk_2D0.y) * 0.04f;
    self->unk_2E8.z = fabsf(self->camEye.z - self->unk_2D0.z) * 0.04f;
    self->unk_30C.x = fabsf(self->camLookAt.x - self->unk_2F4.x) * 0.04f;
    self->unk_30C.y = fabsf(self->camLookAt.y - self->unk_2F4.y) * 0.04f;
    self->unk_30C.z = fabsf(self->camLookAt.z - self->unk_2F4.z) * 0.04f;
    Gameplay_CameraSetAtEye(globalCtx, self->subCamId, &self->camLookAt, &self->camEye);
    Gameplay_CameraSetFov(globalCtx, self->subCamId, globalCtx->mainCamera.fov);
    self->csCameraTimer = 60;
    self->actionFunc = EnDivingGame_RupeeThrow;
    self->unk_318 = 0.0f;
}

// Throws rupee when self->spawnRuppyTimer == 0
void EnDivingGame_RupeeThrow(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (func_800C0DB4(globalCtx, &self->actor.projectedPos)) {
        func_800F6828(0);
    }
    if (self->subCamId != 0) {
        Math_ApproachF(&self->camEye.x, self->unk_2D0.x, self->unk_2DC.x, self->unk_2E8.x * self->unk_318);
        Math_ApproachF(&self->camEye.z, self->unk_2D0.z, self->unk_2DC.z, self->unk_2E8.z * self->unk_318);
        Math_ApproachF(&self->camLookAt.x, self->unk_2F4.x, self->unk_300.x, self->unk_30C.x * self->unk_318);
        Math_ApproachF(&self->camLookAt.y, self->unk_2F4.y, self->unk_300.y, self->unk_30C.y * self->unk_318);
        Math_ApproachF(&self->camLookAt.z, self->unk_2F4.z, self->unk_300.z, self->unk_30C.z * self->unk_318);
        Math_ApproachF(&self->unk_318, 1.0f, 1.0f, 0.02f);
    }
    Gameplay_CameraSetAtEye(globalCtx, self->subCamId, &self->camLookAt, &self->camEye);
    if (!self->allRupeesThrown && self->spawnRuppyTimer == 0) {
        self->spawnRuppyTimer = 5;
        EnDivingGame_SpawnRuppy(self, globalCtx);
        self->rupeesLeftToThrow--;
        if (!(gSaveContext.eventChkInf[3] & 0x100)) {
            self->unk_296 = 30;
        } else {
            self->unk_296 = 5;
        }
        if (self->rupeesLeftToThrow <= 0) {
            self->rupeesLeftToThrow = 0;
            self->allRupeesThrown = true;
        }
    }
    if (self->csCameraTimer == 0 ||
        ((fabsf(self->camEye.x - self->unk_2D0.x) < 2.0f) && (fabsf(self->camEye.y - self->unk_2D0.y) < 2.0f) &&
         (fabsf(self->camEye.z - self->unk_2D0.z) < 2.0f) && (fabsf(self->camLookAt.x - self->unk_2F4.x) < 2.0f) &&
         (fabsf(self->camLookAt.y - self->unk_2F4.y) < 2.0f) && (fabsf(self->camLookAt.z - self->unk_2F4.z) < 2.0f))) {
        if (self->unk_2A2 != 0) {
            self->csCameraTimer = 70;
            self->unk_2A2 = 2;
            self->actionFunc = func_809EE780;
        } else {
            self->actionFunc = EnDivingGame_SetupUnderwaterViewCs;
        }
    }
}

// Called just before changing the camera to focus the underwater rupees.
void EnDivingGame_SetupUnderwaterViewCs(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_296 == 0) {
        self->unk_2A2 = 1;
        self->csCameraTimer = 100;
        self->actionFunc = EnDivingGame_RupeeThrow;
        self->camLookAt.x = self->unk_2F4.x = -210.0f;
        self->camLookAt.y = self->unk_2F4.y = -80.0f;
        self->camLookAt.z = self->unk_2F4.z = -1020.0f;
        self->camEye.x = self->unk_2D0.x = -280.0f;
        self->camEye.y = self->unk_2D0.y = -20.0f;
        self->camEye.z = self->unk_2D0.z = -240.0f;
    }
}

// EnDivingGame_SayStartAndWait ?
void func_809EE780(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->csCameraTimer == 0) {
        Gameplay_ClearCamera(globalCtx, self->subCamId);
        Gameplay_ChangeCameraStatus(globalCtx, 0, CAM_STAT_ACTIVE);
        self->actor.textId = 0x405A;
        func_8010B720(globalCtx, self->actor.textId);
        self->unk_292 = 5;
        self->actionFunc = func_809EE800;
    }
}

// EnDivingGame_TalkDuringMinigame
void func_809EE800(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_292 == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);
        if (!(gSaveContext.eventChkInf[3] & 0x100)) {
            func_80088B34(BREG(2) + 50);
        } else {
            func_80088B34(BREG(2) + 50);
        }
        func_800F5ACC(0x6C);
        func_8002DF54(globalCtx, NULL, 7);
        self->actor.textId = 0x405B;
        self->unk_292 = 5;
        self->state = ENDIVINGGAME_STATE_PLAYING;
        self->actionFunc = EnDivingGame_Talk;
    }
}

void func_809EE8F0(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((self->unk_292 == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx))) {
        func_80106CCC(globalCtx);
        self->actionFunc = EnDivingGame_Talk;
    } else {
        EnDivingGame_HasMinigameFinished(self, globalCtx);
    }
}

// EnDivingGame_SayCongratsAndWait ? // EnDivingGame_PlayerWonPhase1
void func_809EE96C(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((self->unk_292 == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx))) {
        func_80106CCC(globalCtx);
        func_8002DF54(globalCtx, NULL, 7);
        self->actor.textId = 0x4056;
        self->unk_292 = 5;
        self->state = ENDIVINGGAME_STATE_AWARDPRIZE;
        self->actionFunc = EnDivingGame_Talk;
    }
}

void func_809EEA00(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((self->unk_292 == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx))) {
        func_80106CCC(globalCtx);
        self->actor.parent = NULL;
        func_8002F434(&self->actor, globalCtx, GI_SCALE_SILVER, 90.0f, 10.0f);
        self->actionFunc = func_809EEA90;
    }
}

void func_809EEA90(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actionFunc = func_809EEAF8;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_SCALE_SILVER, 90.0f, 10.0f);
    }
}

// Award the scale?
void func_809EEAF8(EnDivingGame* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        // "Successful completion"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 正常終了 ☆☆☆☆☆ \n" VT_RST);
        self->allRupeesThrown = self->state = self->phase = self->unk_2A2 = self->grabbedRupeesCounter = 0;
        gSaveContext.eventChkInf[3] |= 0x100;
        self->actionFunc = func_809EDCB0;
    }
}

void EnDivingGame_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnDivingGame* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f pos;

    if (self->csCameraTimer != 0) {
        self->csCameraTimer--;
    }
    if (self->unk_296 != 0) {
        self->unk_296--;
    }
    if (self->eyeTimer != 0) {
        self->eyeTimer--;
    }
    if (self->spawnRuppyTimer != 0) {
        self->spawnRuppyTimer--;
    }

    if (1) {}

    if (gSaveContext.timer1Value == 10) {
        func_800F5918();
    }
    if (self->eyeTimer == 0) {
        self->eyeTimer = 2;
        self->eyeTexIndex++;
        if (self->eyeTexIndex >= 3) {
            self->eyeTexIndex = 0;
            self->eyeTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }
    self->actionFunc(self, globalCtx);
    Actor_SetFocus(&self->actor, 80.0f);
    self->unk_324.unk_18 = player->actor.world.pos;
    self->unk_324.unk_18.y = player->actor.world.pos.y;
    func_80034A14(&self->actor, &self->unk_324, 2, 4);
    self->vec_284 = self->unk_324.unk_08;
    self->vec_28A = self->unk_324.unk_0E;
    if ((globalCtx->gameplayFrames % 16) == 0) {
        pos = self->actor.world.pos;
        pos.y += 20.0f;
        EffectSsGRipple_Spawn(globalCtx, &pos, 100, 500, 30);
    }
    self->unk_290++;
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 60.0f, 29);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

Gfx* EnDivingGame_EmptyDList(GraphicsContext* gfxCtx) {
    Gfx* displayList = Graph_Alloc(gfxCtx, sizeof(Gfx));

    gSPEndDisplayList(displayList);
    return displayList;
}

s32 EnDivingGame_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                  void* thisx) {
    EnDivingGame* self = THIS;
    s32 pad;

    if (limbIndex == 6) {
        rot->x += self->vec_28A.y;
    }

    if (limbIndex == 15) {
        rot->x += self->vec_284.y;
        rot->z += self->vec_284.z;
    }

    if (self->notPlayingMinigame && (limbIndex == 8 || limbIndex == 9 || limbIndex == 12)) {
        rot->y += Math_SinS((globalCtx->state.frames * (limbIndex * 50 + 0x814))) * 200.0f;
        rot->z += Math_CosS((globalCtx->state.frames * (limbIndex * 50 + 0x940))) * 200.0f;
    }

    return 0;
}

void EnDivingGame_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDivingGame* self = THIS;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_diving_game.c", 1212);
    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, EnDivingGame_EmptyDList(globalCtx->state.gfxCtx));
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeTexIndex]));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnDivingGame_OverrideLimbDraw, NULL, self);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_diving_game.c", 1232);
}
