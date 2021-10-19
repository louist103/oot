#include "z_en_daiku.h"
#include "overlays/actors/ovl_En_GeldB/z_en_geldb.h"
#include "objects/object_daiku/object_daiku.h"

#define FLAGS 0x00000019

#define THIS ((EnDaiku*)thisx)

typedef struct {
    AnimationHeader* anim;
    f32 unk_4;
    u8 mode;
    f32 transitionRate;
} EnDaikuAnimation;

typedef enum {
    /* 0 */ ENDAIKU_ANIM_SHOUT,
    /* 1 */ ENDAIKU_ANIM_STAND,
    /* 2 */ ENDAIKU_ANIM_CELEBRATE,
    /* 3 */ ENDAIKU_ANIM_RUN,
    /* 4 */ ENDAIKU_ANIM_SIT
} EnDaikuAnimationIdx;

typedef struct {
    Vec3f eyePosDeltaLocal;
    s32 maxFramesActive;
} EnDaikuEscapeSubCamParam;

// state flags

// probably related to animating torso and head to look towards the player
#define ENDAIKU_STATEFLAG_1 (1 << 1)
// same
#define ENDAIKU_STATEFLAG_2 (1 << 2)
// the gerudo guard appeared (after talking to the carpenter)
#define ENDAIKU_STATEFLAG_GERUDOFIGHTING (1 << 3)
// the gerudo guard was defeated
#define ENDAIKU_STATEFLAG_GERUDODEFEATED (1 << 4)

typedef enum {
    /* 0 */ ENDAIKU_STATE_CAN_TALK,
    /* 2 */ ENDAIKU_STATE_TALKING = 2,
    /* 3 */ ENDAIKU_STATE_NO_TALK
} EnDaikuTalkState;

void EnDaiku_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDaiku_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDaiku_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDaiku_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDaiku_TentIdle(EnDaiku* self, GlobalContext* globalCtx);
void EnDaiku_Jailed(EnDaiku* self, GlobalContext* globalCtx);
void EnDaiku_WaitFreedom(EnDaiku* self, GlobalContext* globalCtx);
void EnDaiku_InitEscape(EnDaiku* self, GlobalContext* globalCtx);
void EnDaiku_EscapeRotate(EnDaiku* self, GlobalContext* globalCtx);
void EnDaiku_InitSubCamera(EnDaiku* self, GlobalContext* globalCtx);
void EnDaiku_EscapeRun(EnDaiku* self, GlobalContext* globalCtx);
s32 EnDaiku_OverrideLimbDraw(GlobalContext* globalCtx, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx);
void EnDaiku_PostLimbDraw(GlobalContext* globalCtx, s32 limb, Gfx** dList, Vec3s* rot, void* thisx);

const ActorInit En_Daiku_InitVars = {
    ACTOR_EN_DAIKU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DAIKU,
    sizeof(EnDaiku),
    (ActorFunc)EnDaiku_Init,
    (ActorFunc)EnDaiku_Destroy,
    (ActorFunc)EnDaiku_Update,
    (ActorFunc)EnDaiku_Draw,
};

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
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 18, 66, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit2 = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(0, 0x0),
    /* Master sword  */ DMG_ENTRY(0, 0x0),
    /* Giant's Knife */ DMG_ENTRY(0, 0x0),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0x0),
    /* Giant spin    */ DMG_ENTRY(0, 0x0),
    /* Master spin   */ DMG_ENTRY(0, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(0, 0x0),
    /* Giant jump    */ DMG_ENTRY(0, 0x0),
    /* Master jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static EnDaikuAnimation sAnimations[] = {
    { &object_daiku_Anim_001AB0, 1.0f, 0, 0 }, { &object_daiku_Anim_007DE0, 1.0f, 0, 0 },
    { &object_daiku_Anim_00885C, 1.0f, 0, 0 }, { &object_daiku_Anim_000C44, 1.0f, 0, 0 },
    { &object_daiku_Anim_008164, 1.0f, 0, 0 },
};

static EnDaikuEscapeSubCamParam sEscapeSubCamParams[] = {
    { { 0, 130, 220 }, 100 },
    { { -20, 22, 280 }, 110 },
    { { 50, 180, 350 }, 100 },
    { { -40, 60, 60 }, 120 },
};

void EnDaiku_Change(EnDaiku* self, s32 animIndex, s32* currentAnimIndex) {
    f32 transitionRate;

    if (*currentAnimIndex < 0 || *currentAnimIndex == animIndex) {
        transitionRate = 0.0f;
    } else {
        transitionRate = sAnimations[animIndex].transitionRate;
    }

    Animation_Change(&self->skelAnime, sAnimations[animIndex].anim, 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimations[animIndex].anim), sAnimations[animIndex].mode, transitionRate);

    *currentAnimIndex = animIndex;
}

void EnDaiku_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDaiku* self = THIS;
    s32 pad;
    s32 noKill = true;
    s32 isFree = false;

    if ((self->actor.params & 3) == 0 && (gSaveContext.eventChkInf[9] & 1)) {
        isFree = true;
    } else if ((self->actor.params & 3) == 1 && (gSaveContext.eventChkInf[9] & 2)) {
        isFree = true;
    } else if ((self->actor.params & 3) == 2 && (gSaveContext.eventChkInf[9] & 4)) {
        isFree = true;
    } else if ((self->actor.params & 3) == 3 && (gSaveContext.eventChkInf[9] & 8)) {
        isFree = true;
    }

    if (isFree == true && globalCtx->sceneNum == SCENE_GERUDOWAY) {
        noKill = false;
    } else if (isFree == false && globalCtx->sceneNum == SCENE_TENT) {
        noKill = false;
    }

    self->startFightSwitchFlag = self->actor.shape.rot.z & 0x3F;
    self->actor.shape.rot.z = 0;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_daiku_Skel_007958, NULL, self->jointTable, self->morphTable,
                       17);

    if (!noKill) {
        Actor_Kill(&self->actor);
        return;
    }

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit2);

    Animation_Change(&self->skelAnime, sAnimations[0].anim, 1.0f, 0.0f, Animation_GetLastFrame(sAnimations[0].anim),
                     sAnimations[0].mode, sAnimations[0].transitionRate);

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    self->actor.targetMode = 6;
    self->currentAnimIndex = -1;
    self->runSpeed = 5.0f;
    self->initRot = self->actor.world.rot;
    self->initPos = self->actor.world.pos;

    if (globalCtx->sceneNum == SCENE_GERUDOWAY) {
        EnDaiku_Change(self, ENDAIKU_ANIM_STAND, &self->currentAnimIndex);
        self->stateFlags |= ENDAIKU_STATEFLAG_1 | ENDAIKU_STATEFLAG_2;
        self->actionFunc = EnDaiku_Jailed;
    } else {
        if ((self->actor.params & 3) == 1 || (self->actor.params & 3) == 3) {
            EnDaiku_Change(self, ENDAIKU_ANIM_SIT, &self->currentAnimIndex);
            self->stateFlags |= ENDAIKU_STATEFLAG_1;
        } else {
            EnDaiku_Change(self, ENDAIKU_ANIM_SHOUT, &self->currentAnimIndex);
            self->stateFlags |= ENDAIKU_STATEFLAG_1 | ENDAIKU_STATEFLAG_2;
        }

        self->skelAnime.curFrame = (s32)(Rand_ZeroOne() * self->skelAnime.endFrame);
        self->actionFunc = EnDaiku_TentIdle;
    }
}

void EnDaiku_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDaiku* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 EnDaiku_UpdateTalking(EnDaiku* self, GlobalContext* globalCtx) {
    s32 newTalkState = ENDAIKU_STATE_TALKING;

    if (func_8010BDBC(&globalCtx->msgCtx) == 6) {
        if (globalCtx->sceneNum == SCENE_GERUDOWAY) {
            if (func_80106BC8(globalCtx) != 0) {
                if (self->actor.textId == 0x6007) {
                    Flags_SetSwitch(globalCtx, self->startFightSwitchFlag);
                    newTalkState = ENDAIKU_STATE_CAN_TALK;
                } else {
                    self->actionFunc = EnDaiku_InitEscape;
                    newTalkState = ENDAIKU_STATE_NO_TALK;
                }
            }
        } else if (globalCtx->sceneNum == SCENE_TENT) {
            if (func_80106BC8(globalCtx) != 0) {
                switch (self->actor.textId) {
                    case 0x6061:
                        gSaveContext.infTable[23] |= 0x40;
                        break;
                    case 0x6064:
                        gSaveContext.infTable[23] |= 0x100;
                        break;
                }

                newTalkState = ENDAIKU_STATE_CAN_TALK;
            }
        }
    }

    return newTalkState;
}

void EnDaiku_UpdateText(EnDaiku* self, GlobalContext* globalCtx) {
    s32 carpenterType;
    s32 freedCount;
    s16 sp2E;
    s16 sp2C;

    if (self->talkState == ENDAIKU_STATE_TALKING) {
        self->talkState = EnDaiku_UpdateTalking(self, globalCtx);
    } else if (func_8002F194(&self->actor, globalCtx)) {
        self->talkState = ENDAIKU_STATE_TALKING;
    } else {
        func_8002F374(globalCtx, &self->actor, &sp2E, &sp2C);
        if (sp2E >= 0 && sp2E <= 320 && sp2C >= 0 && sp2C <= 240 && self->talkState == ENDAIKU_STATE_CAN_TALK &&
            func_8002F2CC(&self->actor, globalCtx, 100.0f) == 1) {
            if (globalCtx->sceneNum == SCENE_GERUDOWAY) {
                if (self->stateFlags & ENDAIKU_STATEFLAG_GERUDODEFEATED) {
                    freedCount = 0;
                    for (carpenterType = 0; carpenterType < 4; carpenterType++) {
                        if (gSaveContext.eventChkInf[9] & (1 << carpenterType)) {
                            freedCount++;
                        }
                    }

                    switch (freedCount) {
                        case 0:
                            self->actor.textId = 0x605B;
                            break;
                        case 1:
                            self->actor.textId = 0x605C;
                            break;
                        case 2:
                            self->actor.textId = 0x605D;
                            break;
                        case 3:
                            self->actor.textId = 0x605E;
                            break;
                    }
                } else if (!(self->stateFlags &
                             (ENDAIKU_STATEFLAG_GERUDOFIGHTING | ENDAIKU_STATEFLAG_GERUDODEFEATED))) {
                    self->actor.textId = 0x6007;
                }
            } else if (globalCtx->sceneNum == SCENE_TENT) {
                switch (self->actor.params & 3) {
                    case 0:
                        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                            self->actor.textId = 0x6060;
                        } else {
                            self->actor.textId = 0x605F;
                        }
                        break;
                    case 1:
                        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                            self->actor.textId = 0x6063;
                        } else {
                            if (!(gSaveContext.infTable[23] & 0x40)) {
                                self->actor.textId = 0x6061;
                            } else {
                                self->actor.textId = 0x6062;
                            }
                        }
                        break;
                    case 2:
                        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                            self->actor.textId = 0x6066;
                        } else {
                            if (!(gSaveContext.infTable[23] & 0x100)) {
                                self->actor.textId = 0x6064;
                            } else {
                                self->actor.textId = 0x6065;
                            }
                        }
                        break;
                    case 3:
                        if (CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                            self->actor.textId = 0x6068;
                        } else {
                            self->actor.textId = 0x6067;
                        }
                        break;
                }
            }
        }
    }
}

/**
 * The carpenter is idling in the tent.
 */
void EnDaiku_TentIdle(EnDaiku* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    EnDaiku_UpdateText(self, globalCtx);
}

/**
 * The carpenter is jailed in a Gerudo fortress cell, talking to him starts a fight against a gerudo guard
 */
void EnDaiku_Jailed(EnDaiku* self, GlobalContext* globalCtx) {
    EnGeldB* gerudo;
    s32 temp_t9;
    s32 temp_v1;

    if (!(self->stateFlags & ENDAIKU_STATEFLAG_GERUDOFIGHTING)) {
        EnDaiku_UpdateText(self, globalCtx);
    }
    SkelAnime_Update(&self->skelAnime);

    gerudo = (EnGeldB*)Actor_Find(&globalCtx->actorCtx, ACTOR_EN_GELDB, ACTORCAT_ENEMY);
    if (gerudo == NULL) {
        self->stateFlags |= ENDAIKU_STATEFLAG_GERUDODEFEATED;
        self->stateFlags &= ~ENDAIKU_STATEFLAG_GERUDOFIGHTING;
        EnDaiku_Change(self, ENDAIKU_ANIM_CELEBRATE, &self->currentAnimIndex);
        self->actionFunc = EnDaiku_WaitFreedom;
    } else if (!(self->stateFlags & ENDAIKU_STATEFLAG_GERUDOFIGHTING) && !gerudo->invisible) {
        self->stateFlags |= ENDAIKU_STATEFLAG_GERUDOFIGHTING;
        self->actor.flags &= ~9;
    }
}

/**
 * The player defeated the gerudo guard and the carpenter is waiting for the cell door to be opened, and for the player
 * to then talk to him
 */
void EnDaiku_WaitFreedom(EnDaiku* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (Flags_GetSwitch(globalCtx, self->actor.params >> 8 & 0x3F)) {
        self->actor.flags |= 9;
        EnDaiku_UpdateText(self, globalCtx);
    }
}

/**
 * The carpenter is free, initializes his running away animation
 */
void EnDaiku_InitEscape(EnDaiku* self, GlobalContext* globalCtx) {
    Path* path;
    f32 dxz;
    f32 dx;
    f32 dz;
    Vec3s* pointPos;
    s32 exitLoop;

    func_800F5C64(0x51);
    EnDaiku_Change(self, ENDAIKU_ANIM_RUN, &self->currentAnimIndex);
    self->stateFlags &= ~(ENDAIKU_STATEFLAG_1 | ENDAIKU_STATEFLAG_2);

    gSaveContext.eventChkInf[9] |= 1 << (self->actor.params & 3);

    self->actor.gravity = -1.0f;
    self->escapeSubCamTimer = sEscapeSubCamParams[self->actor.params & 3].maxFramesActive;
    EnDaiku_InitSubCamera(self, globalCtx);

    exitLoop = false;
    path = &globalCtx->setupPathList[self->actor.params >> 4 & 0xF];
    while (!exitLoop) {
        pointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->waypoint;
        dx = pointPos->x - self->actor.world.pos.x;
        dz = pointPos->z - self->actor.world.pos.z;
        self->rotYtowardsPath = Math_FAtan2F(dx, dz) * (0x8000 / M_PI);
        dxz = sqrtf(SQ(dx) + SQ(dz));
        if (dxz > 10.0f) {
            exitLoop = true;
        } else {
            self->waypoint++;
        }
    }

    self->actionFunc = EnDaiku_EscapeRotate;
}

/**
 * The carpenter is rotating towards where he is going next
 */
void EnDaiku_EscapeRotate(EnDaiku* self, GlobalContext* globalCtx) {
    s16 diff;

    diff = Math_SmoothStepToS(&self->actor.shape.rot.y, self->rotYtowardsPath, 1, 0x1388, 0);
    SkelAnime_Update(&self->skelAnime);
    if (diff == 0) {
        self->actionFunc = EnDaiku_EscapeRun;
        self->actionFunc(self, globalCtx);
    }
}

void EnDaiku_InitSubCamera(EnDaiku* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f eyePosDeltaLocal;
    Vec3f eyePosDeltaWorld;

    self->subCamActive = true;
    self->escapeSubCamTimer = sEscapeSubCamParams[self->actor.params & 3].maxFramesActive;

    eyePosDeltaLocal.x = sEscapeSubCamParams[self->actor.params & 3].eyePosDeltaLocal.x;
    eyePosDeltaLocal.y = sEscapeSubCamParams[self->actor.params & 3].eyePosDeltaLocal.y;
    eyePosDeltaLocal.z = sEscapeSubCamParams[self->actor.params & 3].eyePosDeltaLocal.z;
    Matrix_RotateY(self->actor.world.rot.y * (M_PI / 0x8000), MTXMODE_NEW);
    Matrix_MultVec3f(&eyePosDeltaLocal, &eyePosDeltaWorld);

    self->subCamEyeInit.x = self->subCamEye.x = self->actor.world.pos.x + eyePosDeltaWorld.x;
    self->subCamEyeInit.y = self->subCamEye.y = self->actor.world.pos.y + eyePosDeltaWorld.y;
    if (1) {}
    self->subCamEyeInit.z = self->subCamEye.z = self->actor.world.pos.z + eyePosDeltaWorld.z;

    if (1) {}
    self->subCamAtTarget.x = self->subCamAt.x = self->actor.world.pos.x;
    self->subCamAtTarget.y = self->subCamAt.y = self->actor.world.pos.y + 60.0f;
    if (1) {}
    self->subCamAtTarget.z = self->subCamAt.z = self->actor.world.pos.z;

    self->subCamId = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, self->subCamId, CAM_STAT_ACTIVE);

    Gameplay_CameraSetAtEye(globalCtx, self->subCamId, &self->subCamAt, &self->subCamEye);
    Gameplay_CameraSetFov(globalCtx, self->subCamId, globalCtx->mainCamera.fov);
    func_8002DF54(globalCtx, &self->actor, 1);
}

void EnDaiku_UpdateSubCamera(EnDaiku* self, GlobalContext* globalCtx) {
    s32 pad;

    self->subCamAtTarget.x = self->actor.world.pos.x;
    self->subCamAtTarget.y = self->actor.world.pos.y + 60.0f;
    self->subCamAtTarget.z = self->actor.world.pos.z;

    Math_SmoothStepToF(&self->subCamAt.x, self->subCamAtTarget.x, 1.0f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->subCamAt.y, self->subCamAtTarget.y, 1.0f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->subCamAt.z, self->subCamAtTarget.z, 1.0f, 1000.0f, 0.0f);

    Gameplay_CameraSetAtEye(globalCtx, self->subCamId, &self->subCamAt, &self->subCamEye);
}

void EnDaiku_EscapeSuccess(EnDaiku* self, GlobalContext* globalCtx) {
    static Vec3f D_809E4148 = { 0.0f, 0.0f, 120.0f };
    Actor* gerudoGuard;
    Vec3f vec;

    Gameplay_ClearCamera(globalCtx, self->subCamId);
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
    self->subCamActive = false;

    if ((gSaveContext.eventChkInf[9] & 0xF) == 0xF) {
        Matrix_RotateY(self->initRot.y * (M_PI / 0x8000), MTXMODE_NEW);
        Matrix_MultVec3f(&D_809E4148, &vec);
        gerudoGuard =
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_GE3, self->initPos.x + vec.x, self->initPos.y + vec.y,
                        self->initPos.z + vec.z, 0, Math_FAtan2F(-vec.x, -vec.z) * (0x8000 / M_PI), 0, 2);

        if (gerudoGuard == NULL) {
            Actor_Kill(&self->actor);
        }
    } else {
        func_8002DF54(globalCtx, &self->actor, 7);
    }
}

/**
 * The carpenter is running away
 */
void EnDaiku_EscapeRun(EnDaiku* self, GlobalContext* globalCtx) {
    s32 pad1;
    Path* path;
    s16 ry;
    f32 dx;
    f32 dz;
    s32 pad2;
    f32 dxz;
    Vec3s* pointPos;

    path = &globalCtx->setupPathList[self->actor.params >> 4 & 0xF];
    pointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->waypoint;
    dx = pointPos->x - self->actor.world.pos.x;
    dz = pointPos->z - self->actor.world.pos.z;
    ry = Math_FAtan2F(dx, dz) * (0x8000 / M_PI);
    dxz = sqrtf(SQ(dx) + SQ(dz));
    if (dxz <= 20.88f) {
        self->waypoint++;
        if (self->waypoint >= path->count) {
            if (self->subCamActive) {
                EnDaiku_EscapeSuccess(self, globalCtx);
            }
            Actor_Kill(&self->actor);
            return;
        }
    }

    Math_SmoothStepToS(&self->actor.shape.rot.y, ry, 1, 0xFA0, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    Math_SmoothStepToF(&self->actor.speedXZ, self->runSpeed, 0.6f, dxz, 0.0f);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    if (self->subCamActive) {
        EnDaiku_UpdateSubCamera(self, globalCtx);
        if (self->escapeSubCamTimer-- <= 0) {
            EnDaiku_EscapeSuccess(self, globalCtx);
        }
    }

    SkelAnime_Update(&self->skelAnime);
}

void EnDaiku_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDaiku* self = THIS;
    s32 curFrame;
    Player* player = GET_PLAYER(globalCtx);

    if (self->currentAnimIndex == ENDAIKU_ANIM_RUN) {
        curFrame = self->skelAnime.curFrame;
        if (curFrame == 6 || curFrame == 15) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_WALK);
        }
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

    self->actionFunc(self, globalCtx);

    if (self->stateFlags & ENDAIKU_STATEFLAG_1) {
        self->unk_244.unk_18.x = player->actor.focus.pos.x;
        self->unk_244.unk_18.y = player->actor.focus.pos.y;
        self->unk_244.unk_18.z = player->actor.focus.pos.z;

        if (self->stateFlags & ENDAIKU_STATEFLAG_2) {
            func_80034A14(&self->actor, &self->unk_244, 0, 4);
        } else {
            func_80034A14(&self->actor, &self->unk_244, 0, 2);
        }
    }
}

void EnDaiku_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDaiku* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_daiku.c", 1227);

    func_80093D18(globalCtx->state.gfxCtx);

    if ((thisx->params & 3) == 0) {
        gDPSetEnvColor(POLY_OPA_DISP++, 170, 10, 70, 255);
    } else if ((thisx->params & 3) == 1) {
        gDPSetEnvColor(POLY_OPA_DISP++, 170, 200, 255, 255);
    } else if ((thisx->params & 3) == 2) {
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 230, 70, 255);
    } else if ((thisx->params & 3) == 3) {
        gDPSetEnvColor(POLY_OPA_DISP++, 200, 0, 150, 255);
    }

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnDaiku_OverrideLimbDraw, EnDaiku_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_daiku.c", 1255);
}

s32 EnDaiku_OverrideLimbDraw(GlobalContext* globalCtx, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnDaiku* self = THIS;

    switch (limb) {
        case 8: // torso
            rot->x += self->unk_244.unk_0E.y;
            rot->y -= self->unk_244.unk_0E.x;
            break;
        case 15: // head
            rot->x += self->unk_244.unk_08.y;
            rot->z += self->unk_244.unk_08.x;
            break;
    }

    return false;
}

void EnDaiku_PostLimbDraw(GlobalContext* globalCtx, s32 limb, Gfx** dList, Vec3s* rot, void* thisx) {
    static Gfx* hairDLists[] = { object_daiku_DL_005BD0, object_daiku_DL_005AC0, object_daiku_DL_005990,
                                 object_daiku_DL_005880 };
    static Vec3f targetPosHeadLocal = { 700, 1100, 0 };
    EnDaiku* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_daiku.c", 1323);

    if (limb == 15) { // head
        Matrix_MultVec3f(&targetPosHeadLocal, &self->actor.focus.pos);
        gSPDisplayList(POLY_OPA_DISP++, hairDLists[self->actor.params & 3]);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_daiku.c", 1330);
}
