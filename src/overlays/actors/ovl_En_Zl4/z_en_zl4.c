/*
 * File: z_en_zl4.c
 * Overlay: ovl_En_Zl4
 * Description: Child Princess Zelda
 */

#include "z_en_zl4.h"
#include "objects/object_zl4/object_zl4.h"
#include "scenes/indoors/nakaniwa/nakaniwa_scene.h"

#define FLAGS 0x00000019

#define THIS ((EnZl4*)thisx)

typedef enum {
    /* 0 */ ZL4_CS_WAIT,
    /* 1 */ ZL4_CS_START,
    /* 2 */ ZL4_CS_MEET,
    /* 3 */ ZL4_CS_STONE,
    /* 4 */ ZL4_CS_NAMES,
    /* 5 */ ZL4_CS_LEGEND,
    /* 6 */ ZL4_CS_WINDOW,
    /* 7 */ ZL4_CS_GANON,
    /* 8 */ ZL4_CS_PLAN
} EnZl4CutsceneState;

typedef enum {
    /* 0 */ ZL4_EYES_NEUTRAL,
    /* 1 */ ZL4_EYES_SHUT,
    /* 2 */ ZL4_EYES_LOOK_LEFT,
    /* 3 */ ZL4_EYES_LOOK_RIGHT,
    /* 4 */ ZL4_EYES_WIDE,
    /* 5 */ ZL4_EYES_SQUINT,
    /* 6 */ ZL4_EYES_OPEN
} EnZl4EyeExpression;

typedef enum {
    /* 0 */ ZL4_MOUTH_NEUTRAL,
    /* 1 */ ZL4_MOUTH_HAPPY,
    /* 2 */ ZL4_MOUTH_WORRIED,
    /* 3 */ ZL4_MOUTH_SURPRISED
} EnZl4MouthExpression;

typedef enum {
    /* 0 */ ZL4_EYE_OPEN,
    /* 1 */ ZL4_EYE_BLINK,
    /* 2 */ ZL4_EYE_SHUT,
    /* 3 */ ZL4_EYE_WIDE,
    /* 4 */ ZL4_EYE_SQUINT,
    /* 5 */ ZL4_EYE_LOOK_OUT,
    /* 6 */ ZL4_EYE_LOOK_IN
} EnZl4EyeState;

void EnZl4_Init(Actor* thisx, GlobalContext* globalCtx);
void EnZl4_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnZl4_Update(Actor* thisx, GlobalContext* globalCtx);
void EnZl4_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnZl4_Cutscene(EnZl4* self, GlobalContext* globalCtx);
void EnZl4_Idle(EnZl4* self, GlobalContext* globalCtx);
void EnZl4_TheEnd(EnZl4* self, GlobalContext* globalCtx);

const ActorInit En_Zl4_InitVars = {
    ACTOR_EN_ZL4,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZL4,
    sizeof(EnZl4),
    (ActorFunc)EnZl4_Init,
    (ActorFunc)EnZl4_Destroy,
    (ActorFunc)EnZl4_Update,
    (ActorFunc)EnZl4_Draw,
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
    { 10, 44, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

typedef enum {
    /*  0 */ ZL4_ANIM_0,
    /*  1 */ ZL4_ANIM_1,
    /*  2 */ ZL4_ANIM_2,
    /*  3 */ ZL4_ANIM_3,
    /*  4 */ ZL4_ANIM_4,
    /*  5 */ ZL4_ANIM_5,
    /*  6 */ ZL4_ANIM_6,
    /*  7 */ ZL4_ANIM_7,
    /*  8 */ ZL4_ANIM_8,
    /*  9 */ ZL4_ANIM_9,
    /* 10 */ ZL4_ANIM_10,
    /* 11 */ ZL4_ANIM_11,
    /* 12 */ ZL4_ANIM_12,
    /* 13 */ ZL4_ANIM_13,
    /* 14 */ ZL4_ANIM_14,
    /* 15 */ ZL4_ANIM_15,
    /* 16 */ ZL4_ANIM_16,
    /* 17 */ ZL4_ANIM_17,
    /* 18 */ ZL4_ANIM_18,
    /* 19 */ ZL4_ANIM_19,
    /* 20 */ ZL4_ANIM_20,
    /* 21 */ ZL4_ANIM_21,
    /* 22 */ ZL4_ANIM_22,
    /* 23 */ ZL4_ANIM_23,
    /* 24 */ ZL4_ANIM_24,
    /* 25 */ ZL4_ANIM_25,
    /* 26 */ ZL4_ANIM_26,
    /* 27 */ ZL4_ANIM_27,
    /* 28 */ ZL4_ANIM_28,
    /* 29 */ ZL4_ANIM_29,
    /* 30 */ ZL4_ANIM_30,
    /* 31 */ ZL4_ANIM_31,
    /* 32 */ ZL4_ANIM_32,
    /* 33 */ ZL4_ANIM_33
} EnZl4Animation;

static struct_80034EC0_Entry sAnimationEntries[] = {
    /*  0 */ /* standing idle */ { &gChildZeldaAnim_000654, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /*  1 */ /* moves to introduce herself */ { &gChildZeldaAnim_00E5C8, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    /*  2 */ /* introducing herself */ { &gChildZeldaAnim_00EBC4, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /*  3 */ /* turns away from window surprised */
    { &gChildZeldaAnim_010DF8, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    /*  4 */ /* standing with hand in front of mouth */
    { &gChildZeldaAnim_011248, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /*  5 */ /* surprise, moves hand to mouth */ { &gChildZeldaAnim_011698, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /*  6 */ /* uncrosses arms, leans toward link with hands together */
    { &gChildZeldaAnim_011B34, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    /*  7 */ /* turns to write letter */ { &gChildZeldaAnim_0125E4, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    /*  8 */ /* writing letter */ { &gChildZeldaAnim_012E58, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /*  9 */ /* pulls back, looks askew */ { &gChildZeldaAnim_013280, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    /* 10 */ /* looks askew at Link */ { &gChildZeldaAnim_013628, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /* 11 */ /* crosses arms, looks to the side */ { &gChildZeldaAnim_013A50, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    /* 12 */ /* arms crossed, looking away */ { &gChildZeldaAnim_013EA0, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /* 13 */ /* turns away, hands behind back, looks up */
    { &gChildZeldaAnim_015F14, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 14 */ /* turns back to link, hands on top of each other */
    { &gChildZeldaAnim_0169B4, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 15 */ /* hands behind back looking up */ { &gChildZeldaAnim_016D08, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 16 */ /* leans toward link, looks askew */ { &gChildZeldaAnim_01726C, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    /* 17 */ /* leaning toward link, looking askew */
    { &gChildZeldaAnim_017818, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    /* 18 */ /* neutral, looking at Link */ { &gChildZeldaAnim_01805C, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 19 */ /* moves towards link, hands clasped */
    { &gChildZeldaAnim_018898, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    /* 20 */ /* facing link, hands clasped */ { &gChildZeldaAnim_01910C, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /* 21 */ /* look in window */ { &gChildZeldaAnim_019600, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 22 */ /* leans forward, hands together */ { &gChildZeldaAnim_01991C, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    /* 23 */ /* turns to link, hands on top of each other */
    { &gChildZeldaAnim_01A2FC, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 24 */ /* stands, hands on top of each other */
    { &gChildZeldaAnim_01AAE0, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 25 */ /* leaning forward, hands together */ { &gChildZeldaAnim_01AE88, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /* 26 */ /* walks aside, points to window */ { &gChildZeldaAnim_01B874, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    /* 27 */ /* stands pointing at window */ { &gChildZeldaAnim_01BCF0, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 28 */ /* laughs, hands together */ { &gChildZeldaAnim_01C494, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /* 29 */ /* happy, hands together */ { &gChildZeldaAnim_01C7B0, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    /* 30 */ /* standing hands behind back looking down*/
    { &gChildZeldaAnim_01CE08, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 31 */ /* cocks head, hands clasped */ { &gChildZeldaAnim_00F0A4, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    /* 32 */ /* happy, hands clasped */ { &gChildZeldaAnim_00F894, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    /* 33 */ /* transition to standing */ { &gChildZeldaAnim_000654, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
};

#include "z_en_zl4_cutscene_data.c"

void EnZl4_SetCsCameraAngle(GlobalContext* globalCtx, s16 index) {
    Camera* activeCam = GET_ACTIVE_CAM(globalCtx);

    Camera_ChangeSetting(activeCam, CAM_SET_FREE0);
    activeCam->at = sCsCameraAngle[index].at;
    activeCam->eye = activeCam->eyeNext = sCsCameraAngle[index].eye;
    activeCam->roll = sCsCameraAngle[index].roll;
    activeCam->fov = sCsCameraAngle[index].fov;
}

void EnZl4_SetCsCameraMove(GlobalContext* globalCtx, s16 index) {
    Camera* activeCam = GET_ACTIVE_CAM(globalCtx);
    Player* player = GET_PLAYER(globalCtx);

    Camera_ChangeSetting(activeCam, CAM_SET_DEMO0);
    Camera_ResetAnim(activeCam);
    Camera_SetCSParams(activeCam, sCsCameraMove[index].atPoints, sCsCameraMove[index].eyePoints, player,
                       sCsCameraMove[index].relativeToPlayer);
}

u16 EnZl4_GetText(GlobalContext* globalCtx, Actor* thisx) {
    u16 faceReaction = Text_GetFaceReaction(globalCtx, 22);
    u16 stoneCount;
    s16 ret;

    if (faceReaction != 0) {
        return faceReaction;
    }

    stoneCount = 0;
    if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) {
        stoneCount = 1;
    }
    if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
        stoneCount++;
    }
    if (CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
        stoneCount++;
    }

    if (stoneCount > 1) {
        ret = 0x703D;
    } else {
        ret = 0x703C;
    }
    return ret;
}

s16 func_80B5B9B0(GlobalContext* globalCtx, Actor* thisx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        return false;
    }
    return true;
}

void EnZl4_UpdateFace(EnZl4* self) {
    if (self->blinkTimer > 0) {
        self->blinkTimer--;
    } else {
        self->blinkTimer = 0;
    }
    if (self->blinkTimer <= 2) {
        self->leftEyeState = self->rightEyeState = self->blinkTimer;
    }
    switch (self->eyeExpression) {
        case ZL4_EYES_NEUTRAL:
            if (self->blinkTimer == 0) {
                self->blinkTimer = Rand_S16Offset(30, 30);
            }
            break;
        case ZL4_EYES_SHUT:
            if (self->blinkTimer == 0) {
                self->leftEyeState = self->rightEyeState = ZL4_EYE_SHUT;
            }
            break;
        case ZL4_EYES_LOOK_LEFT:
            if (self->blinkTimer == 0) {
                self->leftEyeState = ZL4_EYE_LOOK_OUT;
                self->rightEyeState = ZL4_EYE_LOOK_IN;
            }
            break;
        case ZL4_EYES_LOOK_RIGHT:
            if (self->blinkTimer == 0) {
                self->leftEyeState = ZL4_EYE_LOOK_IN;
                self->rightEyeState = ZL4_EYE_LOOK_OUT;
            }
            break;
        case ZL4_EYES_WIDE:
            if (self->blinkTimer == 0) {
                self->leftEyeState = self->rightEyeState = ZL4_EYE_WIDE;
            }
            break;
        case ZL4_EYES_SQUINT:
            if (self->blinkTimer == 0) {
                self->leftEyeState = self->rightEyeState = ZL4_EYE_SQUINT;
            }
            break;
        case ZL4_EYES_OPEN:
            if (self->blinkTimer >= 3) {
                self->blinkTimer = ZL4_EYE_OPEN;
            }
            break;
    }
    switch (self->mouthExpression) {
        case ZL4_MOUTH_HAPPY:
            self->mouthState = 1;
            break;
        case ZL4_MOUTH_WORRIED:
            self->mouthState = 2;
            break;
        case ZL4_MOUTH_SURPRISED:
            self->mouthState = 3;
            break;
        default:
            self->mouthState = 0;
            break;
    }
}

void EnZl4_SetMove(EnZl4* self, GlobalContext* globalCtx) {
    self->skelAnime.moveFlags |= 1;
    AnimationContext_SetMoveActor(globalCtx, &self->actor, &self->skelAnime, 1.0f);
}

void func_80B5BB78(EnZl4* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_1E0.unk_18 = player->actor.world.pos;
    func_80034A14(&self->actor, &self->unk_1E0, 2, 2);
}

void EnZl4_GetActionStartPos(CsCmdActorAction* action, Vec3f* vec) {
    vec->x = action->startPos.x;
    vec->y = action->startPos.y;
    vec->z = action->startPos.z;
}

s32 EnZl4_SetupFromLegendCs(EnZl4* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* playerx = &GET_PLAYER(globalCtx)->actor;
    s16 rotY;

    func_8002DF54(globalCtx, &self->actor, 8);
    playerx->world.pos = self->actor.world.pos;
    rotY = self->actor.shape.rot.y;
    playerx->world.pos.x += 56.0f * Math_SinS(rotY);
    playerx->world.pos.z += 56.0f * Math_CosS(rotY);

    player->linearVelocity = playerx->speedXZ = 0.0f;

    EnZl4_SetCsCameraMove(globalCtx, 5);
    ShrinkWindow_SetVal(0x20);
    Interface_ChangeAlpha(2);
    self->talkTimer2 = 0;
    return true;
}

s32 EnZl4_InMovingAnim(EnZl4* self) {
    if ((self->skelAnime.animation == &gChildZeldaAnim_01B874) ||
        (self->skelAnime.animation == &gChildZeldaAnim_01BCF0) ||
        (self->skelAnime.animation == &gChildZeldaAnim_0125E4) ||
        (self->skelAnime.animation == &gChildZeldaAnim_012E58) ||
        (self->skelAnime.animation == &gChildZeldaAnim_015F14) ||
        (self->skelAnime.animation == &gChildZeldaAnim_0169B4) ||
        (self->skelAnime.animation == &gChildZeldaAnim_016D08) ||
        (self->skelAnime.animation == &gChildZeldaAnim_01805C) ||
        (self->skelAnime.animation == &gChildZeldaAnim_01A2FC) ||
        (self->skelAnime.animation == &gChildZeldaAnim_01AAE0) ||
        (self->skelAnime.animation == &gChildZeldaAnim_01CE08) ||
        (self->skelAnime.animation == &gChildZeldaAnim_018898) ||
        (self->skelAnime.animation == &gChildZeldaAnim_01910C) ||
        (self->skelAnime.animation == &gChildZeldaAnim_00F0A4) ||
        (self->skelAnime.animation == &gChildZeldaAnim_00F894)) {
        return true;
    }
    return false;
}

void EnZl4_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnZl4* self = THIS;

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gChildZeldaSkel, NULL, self->jointTable, self->morphTable, 18);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
    func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_21);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.targetMode = 6;
    self->actor.textId = -1;
    self->eyeExpression = self->mouthExpression = ZL4_MOUTH_NEUTRAL;

    if (gSaveContext.sceneSetupIndex >= 4) {
        func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_0);
        self->actionFunc = EnZl4_TheEnd;
    } else if (gSaveContext.eventChkInf[4] & 1) {
        func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_0);
        self->actionFunc = EnZl4_Idle;
    } else {
        if (gSaveContext.entranceIndex != 0x5F0) {
            func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_21);
            self->csState = ZL4_CS_WAIT;
            self->talkState = 0;
        } else {
            EnZl4_SetupFromLegendCs(self, globalCtx);
            func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_0);
            self->csState = ZL4_CS_LEGEND;
            self->talkState = 0;
        }
        self->actionFunc = EnZl4_Cutscene;
    }
}

void EnZl4_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnZl4* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 EnZl4_SetNextAnim(EnZl4* self, s32 nextAnim) {
    if (!Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
        return false;
    }
    func_80034EC0(&self->skelAnime, sAnimationEntries, nextAnim);
    return true;
}

void EnZl4_ReverseAnimation(EnZl4* self) {
    f32 tempFrame = self->skelAnime.startFrame;

    self->skelAnime.startFrame = self->skelAnime.endFrame;
    self->skelAnime.curFrame = self->skelAnime.endFrame;
    self->skelAnime.endFrame = tempFrame;
    self->skelAnime.playSpeed = -1.0f;
}

s32 EnZl4_CsWaitForPlayer(EnZl4* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* playerx = &GET_PLAYER(globalCtx)->actor;
    s16 rotY;
    s16 yawDiff;
    s16 absYawDiff;

    if (!func_8002F194(&self->actor, globalCtx)) {
        yawDiff = (f32)self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        absYawDiff = ABS(yawDiff);
        if ((playerx->world.pos.y != self->actor.world.pos.y) || (absYawDiff >= 0x3FFC)) {
            return false;
        } else {
            func_8002F2CC(&self->actor, globalCtx, self->collider.dim.radius + 60.0f);
            return false;
        }
    }
    playerx->world.pos = self->actor.world.pos;
    rotY = self->actor.shape.rot.y;
    playerx->world.pos.x += 56.0f * Math_SinS(rotY);
    playerx->world.pos.z += 56.0f * Math_CosS(rotY);
    playerx->speedXZ = 0.0f;
    player->linearVelocity = 0.0f;
    return true;
}

s32 EnZl4_CsMeetPlayer(EnZl4* self, GlobalContext* globalCtx) {
    switch (self->talkState) {
        case 0:
            if (self->skelAnime.curFrame == 50.0f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_VO_Z0_MEET);
            }
            if (!EnZl4_SetNextAnim(self, ZL4_ANIM_4)) {
                break;
            } else {
                func_8010B680(globalCtx, 0x702E, NULL);
                self->talkState++;
            }
            break;
        case 1:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraAngle(globalCtx, 1);
                func_8010B680(globalCtx, 0x702F, NULL);
                self->talkTimer2 = 0;
                self->talkState++;
            }
            break;
        case 2:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gZeldasCourtyardMeetCs);
                gSaveContext.cutsceneTrigger = 1;
                EnZl4_SetCsCameraMove(globalCtx, 0);
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkTimer2 = 0;
                self->talkState++;
            }
            break;
        case 3:
            self->talkTimer2++;
            if (self->talkTimer2 >= 45) {
                func_8010B680(globalCtx, 0x70F9, NULL);
                self->talkState++;
            }
            break;
        case 4:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraMove(globalCtx, 1);
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkTimer2 = 0;
                self->talkState++;
            }
            break;
        case 5:
            self->talkTimer2++;
            if (self->talkTimer2 >= 10) {
                func_8010B680(globalCtx, 0x70FA, NULL);
                self->talkState++;
            }
            break;
        case 6:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraAngle(globalCtx, 2);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_22);
                self->mouthExpression = ZL4_MOUTH_NEUTRAL;
                self->talkTimer2 = 0;
                self->talkState++;
                func_8010B680(globalCtx, 0x70FB, NULL);
            }
            break;
    }
    return (self->talkState == 7) ? 1 : 0;
}

s32 EnZl4_CsAskStone(EnZl4* self, GlobalContext* globalCtx) {
    switch (self->talkState) {
        case 0:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_25)) {
                self->talkState++;
            }
        case 1:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraAngle(globalCtx, 3);
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkTimer1 = 40;
                self->talkState = 2;
            }
            break;
        case 2:
            if (DECR(self->talkTimer1) == 0) {
                func_8010B680(globalCtx, 0x7030, NULL);
                self->talkState++;
            }
            break;
        case 3:
            if (!((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx))) {
                break;
            } else if (globalCtx->msgCtx.choiceIndex == 0) {
                EnZl4_SetCsCameraAngle(globalCtx, 4);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_28);
                self->blinkTimer = 0;
                self->eyeExpression = ZL4_EYES_SQUINT;
                self->mouthExpression = ZL4_MOUTH_HAPPY;
                func_8010B680(globalCtx, 0x7032, NULL);
                self->talkState = 7;
            } else {
                EnZl4_SetCsCameraAngle(globalCtx, 2);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_9);
                self->mouthExpression = ZL4_MOUTH_WORRIED;
                func_8010B680(globalCtx, 0x7031, NULL);
                self->talkState++;
            }
            break;
        case 4:
            if (self->skelAnime.curFrame == 16.0f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_VO_Z0_QUESTION);
            }
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_10)) {
                self->talkState++;
            }
        case 5:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                globalCtx->msgCtx.msgMode = 0x37;
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_9);
                self->mouthExpression = ZL4_MOUTH_WORRIED;
                EnZl4_ReverseAnimation(self);
                self->talkState = 6;
            }
            break;
        case 6:
            self->mouthExpression = ZL4_MOUTH_NEUTRAL;
            EnZl4_SetCsCameraAngle(globalCtx, 3);
            func_8010B680(globalCtx, 0x7030, NULL);
            self->talkState = 12;
            break;
        case 12:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_25)) {
                self->talkState = 13;
            }
        case 13:
            if (!((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx))) {
                break;
            } else if (globalCtx->msgCtx.choiceIndex == 0) {
                EnZl4_SetCsCameraAngle(globalCtx, 4);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_28);
                self->blinkTimer = 0;
                self->eyeExpression = ZL4_EYES_SQUINT;
                self->mouthExpression = ZL4_MOUTH_HAPPY;
                func_8010B680(globalCtx, 0x7032, NULL);
                self->talkState = 7;
            } else {
                EnZl4_SetCsCameraAngle(globalCtx, 2);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_9);
                self->mouthExpression = ZL4_MOUTH_WORRIED;
                func_8010B680(globalCtx, 0x7031, NULL);
                self->talkState = 4;
            }
            break;
        case 7:
            if (self->skelAnime.curFrame == 17.0f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_VO_Z0_SMILE_0);
            }
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_29)) {
                self->talkState++;
            }
        case 8:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraMove(globalCtx, 2);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_0);
                self->blinkTimer = 0;
                self->eyeExpression = ZL4_EYES_NEUTRAL;
                self->mouthExpression = ZL4_MOUTH_NEUTRAL;
                func_8010B680(globalCtx, 0x70FC, NULL);
                self->talkState = 9;
            }
            break;
        case 9:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraAngle(globalCtx, 5);
                func_8010B680(globalCtx, 0x70FD, NULL);
                self->talkState++;
            }
            break;
        case 10:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_5);
                self->eyeExpression = ZL4_EYES_OPEN;
                self->mouthExpression = ZL4_MOUTH_SURPRISED;
                func_8010B680(globalCtx, 0x70FE, NULL);
                self->talkState++;
            }
            break;
    }
    return (self->talkState == 11) ? 1 : 0;
}

s32 EnZl4_CsAskName(EnZl4* self, GlobalContext* globalCtx) {
    switch (self->talkState) {
        case 0:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_4)) {
                self->talkState++;
            }
            break;
        case 1:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraAngle(globalCtx, 6);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_1);
                self->blinkTimer = 11;
                self->eyeExpression = ZL4_EYES_SQUINT;
                self->mouthExpression = ZL4_MOUTH_NEUTRAL;
                globalCtx->msgCtx.msgMode = 0x37;
                func_8010B680(globalCtx, 0x70FF, NULL);
                self->talkState++;
            }
            break;
        case 2:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_2)) {
                self->talkState++;
            }
        case 3:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_16);
                self->blinkTimer = 0;
                self->eyeExpression = ZL4_EYES_NEUTRAL;
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState = 4;
            }
            break;
        case 4:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_17)) {
                func_8010B680(globalCtx, 0x2073, NULL);
                self->talkState++;
            }
            break;
        case 5:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraMove(globalCtx, 3);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_0);
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkTimer2 = 0;
                self->talkState = 6;
            }
            break;
        case 6:
            self->talkTimer2++;
            if (self->talkTimer2 >= 15) {
                func_8010B680(globalCtx, 0x2074, NULL);
                self->talkState++;
            }
            break;
        case 7:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_6);
                self->mouthExpression = ZL4_MOUTH_HAPPY;
                func_8010B680(globalCtx, 0x2075, NULL);
                self->talkState++;
            }
            break;
        case 8:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_25)) {
                self->talkState++;
            }
        case 9:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_8010B680(globalCtx, 0x7033, NULL);
                self->talkState = 10;
            }
            break;
        case 10:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx)) {
                if (globalCtx->msgCtx.choiceIndex == 0) {
                    EnZl4_SetCsCameraMove(globalCtx, 4);
                    func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_33);
                    self->mouthExpression = ZL4_MOUTH_NEUTRAL;
                    globalCtx->msgCtx.msgMode = 0x37;
                    self->talkTimer2 = 0;
                    self->talkState = 15;
                } else {
                    EnZl4_SetCsCameraAngle(globalCtx, 6);
                    globalCtx->msgCtx.msgMode = 0x37;
                    self->talkTimer1 = 20;
                    self->talkState++;
                    self->skelAnime.playSpeed = 0.0f;
                }
            }
            break;
        case 11:
            if (DECR(self->talkTimer1) == 0) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_11);
                self->blinkTimer = 11;
                self->eyeExpression = ZL4_EYES_LOOK_RIGHT;
                self->mouthExpression = ZL4_MOUTH_WORRIED;
                func_8010B680(globalCtx, 0x7034, NULL);
                self->talkState++;
            }
            break;
        case 12:
            if (self->skelAnime.curFrame == 5.0f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_VO_Z0_SIGH_0);
            }
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_12)) {
                self->talkState++;
            }
        case 13:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_6);
                self->blinkTimer = 3;
                self->eyeExpression = ZL4_EYES_NEUTRAL;
                self->mouthExpression = ZL4_MOUTH_HAPPY;
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState = 14;
            }
            break;
        case 14:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_25)) {
                func_8010B680(globalCtx, 0x7033, NULL);
                self->talkState = 10;
            }
            break;
        case 15:
            self->talkTimer2++;
            if (self->talkTimer2 >= 30) {
                func_8010B680(globalCtx, 0x7035, NULL);
                self->talkState++;
            }
            break;
        case 16:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState++;
            }
        case 17:
            self->talkTimer2++;
            if (self->talkTimer2 == 130) {
                globalCtx->msgCtx.msgMode = 0x37;
                globalCtx->nextEntranceIndex = 0xA0;
                gSaveContext.nextCutsceneIndex = 0xFFF7;
                globalCtx->sceneLoadFlag = 0x14;
                globalCtx->fadeTransition = 3;
            }
            break;
    }
    if ((self->talkTimer2 == 17) && (self->talkTimer2 > 130)) {
        return true;
    }
    return false;
}

s32 EnZl4_CsTellLegend(EnZl4* self, GlobalContext* globalCtx) {
    Camera* activeCam = GET_ACTIVE_CAM(globalCtx);

    switch (self->talkState) {
        case 0:
            self->talkTimer2++;
            if (self->talkTimer2 >= 60) {
                func_8010B680(globalCtx, 0x7037, NULL);
                self->talkState++;
            }
            break;
        case 1:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraAngle(globalCtx, 7);
                func_8010B680(globalCtx, 0x2076, NULL);
                self->talkState++;
            }
            break;
        case 2:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraMove(globalCtx, 6);
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState++;
            }
            break;
        case 3:
            if (activeCam->animState == 2) {
                func_8010B680(globalCtx, 0x2077, NULL);
                self->talkState++;
            }
            break;
        case 4:
            if (!((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx))) {
                break;
            } else if (globalCtx->msgCtx.choiceIndex == 0) {
                EnZl4_SetCsCameraAngle(globalCtx, 8);
                func_8010B680(globalCtx, 0x7005, NULL);
                self->talkState = 9;
            } else {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_5);
                self->mouthExpression = ZL4_MOUTH_SURPRISED;
                func_8010B680(globalCtx, 0x7038, NULL);
                self->talkState++;
                Audio_PlayActorSound2(&self->actor, NA_SE_VO_Z0_HURRY);
            }
            break;
        case 5:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_4)) {
                self->talkState++;
            }
        case 6:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_33);
                self->mouthExpression = ZL4_MOUTH_NEUTRAL;
                func_8010B680(globalCtx, 0x7037, NULL);
                self->talkState++;
            }
            break;
        case 7:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_8010B680(globalCtx, 0x2076, NULL);
                self->talkState++;
            }
            break;
        case 8:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_8010B680(globalCtx, 0x2077, NULL);
                self->talkState = 4;
            }
            break;
        case 9:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_26);
                func_8010B680(globalCtx, 0x2078, NULL);
                self->talkState++;
            }
            break;
        case 10:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_27)) {
                self->talkState++;
            }
        case 11:
            if (!((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx))) {
                break;
            } else if (globalCtx->msgCtx.choiceIndex == 0) {
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState = 13;
            } else {
                func_8010B680(globalCtx, 0x700B, NULL);
                self->talkState = 12;
            }
            break;
        case 12:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState = 13;
            }
            break;
    }
    return (self->talkState == 13) ? 1 : 0;
}

s32 EnZl4_CsLookWindow(EnZl4* self, GlobalContext* globalCtx) {
    switch (self->talkState) {
        case 0:
            EnZl4_SetCsCameraMove(globalCtx, 7);
            globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gZeldasCourtyardWindowCs);
            gSaveContext.cutsceneTrigger = 1;
            self->talkState++;
            break;
        case 1:
            if (globalCtx->csCtx.state != CS_STATE_IDLE) {
                if (globalCtx->csCtx.frames == 90) {
                    globalCtx->csCtx.state = CS_STATE_UNSKIPPABLE_INIT;
                }
            } else {
                globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gZeldasCourtyardGanonCs);
                gSaveContext.cutsceneTrigger = 1;
                self->talkState++;
                func_8002DF54(globalCtx, &self->actor, 8);
            }
            break;
        case 2:
            if (globalCtx->csCtx.state != CS_STATE_IDLE) {
                if (globalCtx->csCtx.frames == 209) {
                    globalCtx->csCtx.state = CS_STATE_UNSKIPPABLE_INIT;
                }
            } else {
                func_800AA000(0.0f, 0xA0, 0xA, 0x28);
                func_8002DF54(globalCtx, &self->actor, 1);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_30);
                EnZl4_SetCsCameraAngle(globalCtx, 11);
                func_8010B680(globalCtx, 0x7039, NULL);
                self->talkState++;
            }
            break;
        case 3:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState++;
            }
            break;
    }
    return (self->talkState == 4) ? 1 : 0;
}

s32 EnZl4_CsWarnAboutGanon(EnZl4* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 rotY;

    switch (self->talkState) {
        case 0:
            player->actor.world.pos = self->actor.world.pos;
            rotY = self->actor.shape.rot.y - 0x3FFC;
            player->actor.world.pos.x += 34.0f * Math_SinS(rotY);
            player->actor.world.pos.z += 34.0f * Math_CosS(rotY);
            EnZl4_SetCsCameraMove(globalCtx, 8);
            self->blinkTimer = 0;
            self->eyeExpression = ZL4_EYES_WIDE;
            self->mouthExpression = ZL4_MOUTH_WORRIED;
            self->talkTimer2 = 0;
            self->talkState++;
            func_8010B680(globalCtx, 0x2079, NULL);
        case 1:
            self->talkTimer2++;
            if (self->talkTimer2 >= 20) {
                self->talkState++;
            }
            break;
        case 2:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraMove(globalCtx, 9);
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkTimer2 = 0;
                self->talkState++;
            }
            break;
        case 3:
            self->talkTimer2++;
            if (self->talkTimer2 >= 20) {
                func_8010B680(globalCtx, 0x207A, NULL);
                self->talkState++;
            }
            break;
        case 4:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraAngle(globalCtx, 12);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_23);
                self->blinkTimer = 0;
                self->eyeExpression = ZL4_EYES_NEUTRAL;
                self->mouthExpression = ZL4_MOUTH_SURPRISED;
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState++;
            }
            break;
        case 5:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_24)) {
                func_8010B680(globalCtx, 0x207B, NULL);
                self->talkState++;
            }
            break;
        case 6:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_8010B680(globalCtx, 0x703A, NULL);
                self->talkState++;
            }
            break;
        case 7:
            if (!((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx))) {
                break;
            } else if (globalCtx->msgCtx.choiceIndex == 0) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_31);
                self->blinkTimer = 11;
                self->eyeExpression = ZL4_EYES_SQUINT;
                self->mouthExpression = ZL4_MOUTH_HAPPY;
                func_8010B680(globalCtx, 0x703B, NULL);
                self->talkState = 11;
            } else {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_13);
                self->blinkTimer = 11;
                self->eyeExpression = ZL4_EYES_LOOK_LEFT;
                self->mouthExpression = ZL4_MOUTH_WORRIED;
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState++;
            }
            break;
        case 8:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_15)) {
                self->blinkTimer = 3;
                self->eyeExpression = ZL4_EYES_NEUTRAL;
                self->mouthExpression = ZL4_MOUTH_SURPRISED;
                func_8010B680(globalCtx, 0x7073, NULL);
                self->talkState++;
            }
            break;
        case 9:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_14);
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState++;
            }
            break;
        case 10:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_24)) {
                func_8010B680(globalCtx, 0x703A, NULL);
                self->talkState = 7;
            }
            break;
        case 11:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_32)) {
                self->talkState++;
            }
        case 12:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                globalCtx->msgCtx.msgMode = 0x37;
                self->talkState = 13;
            }
            break;
    }
    return (self->talkState == 13) ? 1 : 0;
}

s32 EnZl4_CsMakePlan(EnZl4* self, GlobalContext* globalCtx) {
    switch (self->talkState) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_18);
            self->blinkTimer = 0;
            self->eyeExpression = ZL4_EYES_NEUTRAL;
            self->mouthExpression = ZL4_MOUTH_WORRIED;
            EnZl4_SetCsCameraMove(globalCtx, 10);
            self->talkTimer2 = 0;
            self->talkState++;
        case 1:
            self->talkTimer2++;
            if (self->talkTimer2 >= 10) {
                func_8010B680(globalCtx, 0x7123, NULL);
                self->talkState++;
            }
            break;
        case 2:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                EnZl4_SetCsCameraAngle(globalCtx, 13);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_19);
                self->blinkTimer = 0;
                self->eyeExpression = ZL4_EYES_NEUTRAL;
                self->mouthExpression = ZL4_MOUTH_SURPRISED;
                func_8010B680(globalCtx, 0x207C, NULL);
                self->talkState++;
            }
            break;
        case 3:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_20)) {
                self->talkState++;
            }
        case 4:
            if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
                func_8010B680(globalCtx, 0x207D, NULL);
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_7);
                self->blinkTimer = 0;
                self->eyeExpression = ZL4_EYES_NEUTRAL;
                self->mouthExpression = ZL4_MOUTH_NEUTRAL;
                self->talkState = 5;
                self->unk_20F = self->lastAction = 0;
            }
            break;
        case 5:
            if (EnZl4_SetNextAnim(self, ZL4_ANIM_8)) {
                self->talkState++;
            }
        case 6:
            if (!((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx))) {
                break;
            } else {
                Camera_ChangeSetting(GET_ACTIVE_CAM(globalCtx), 1);
                self->talkState = 7;
                globalCtx->talkWithPlayer(globalCtx, &self->actor);
                func_8002F434(&self->actor, globalCtx, GI_LETTER_ZELDA, fabsf(self->actor.xzDistToPlayer) + 1.0f,
                              fabsf(self->actor.yDistToPlayer) + 1.0f);
                globalCtx->msgCtx.unk_E3E7 = 4;
                globalCtx->msgCtx.msgMode = 0x36;
            }
            break;
        case 7:
            if (Actor_HasParent(&self->actor, globalCtx)) {
                func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_0);
                self->talkState++;
            } else {
                func_8002F434(&self->actor, globalCtx, GI_LETTER_ZELDA, fabsf(self->actor.xzDistToPlayer) + 1.0f,
                              fabsf(self->actor.yDistToPlayer) + 1.0f);
            }
            // no break here is required for matching
    }
    return (self->talkState == 8) ? 1 : 0;
}

void EnZl4_Cutscene(EnZl4* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    switch (self->csState) {
        case ZL4_CS_WAIT:
            if (EnZl4_CsWaitForPlayer(self, globalCtx)) {
                self->talkState = 0;
                self->csState++;
            }
            break;
        case ZL4_CS_START:
            func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_3);
            self->blinkTimer = 0;
            self->eyeExpression = ZL4_EYES_NEUTRAL;
            self->mouthExpression = ZL4_MOUTH_SURPRISED;
            func_800F5C64(0x51);
            EnZl4_SetCsCameraAngle(globalCtx, 0);
            Interface_ChangeAlpha(2);
            ShrinkWindow_SetVal(0x20);
            self->talkState = 0;
            self->csState++;
            break;
        case ZL4_CS_MEET:
            if (EnZl4_CsMeetPlayer(self, globalCtx)) {
                self->talkState = 0;
                self->csState++;
            }
            break;
        case ZL4_CS_STONE:
            if (EnZl4_CsAskStone(self, globalCtx)) {
                self->talkState = 0;
                self->csState++;
            }
            break;
        case ZL4_CS_NAMES:
            if (EnZl4_CsAskName(self, globalCtx)) {
                self->talkState = 0;
                self->csState++;
            }
            break;
        case ZL4_CS_LEGEND:
            if (EnZl4_CsTellLegend(self, globalCtx)) {
                self->talkState = 0;
                self->csState++;
            }
            break;
        case ZL4_CS_WINDOW:
            if (EnZl4_CsLookWindow(self, globalCtx)) {
                self->talkState = 0;
                self->csState++;
            }
            break;
        case ZL4_CS_GANON:
            if (EnZl4_CsWarnAboutGanon(self, globalCtx)) {
                self->talkState = 0;
                self->csState++;
            }
            break;
        case ZL4_CS_PLAN:
            if (EnZl4_CsMakePlan(self, globalCtx)) {
                func_8002DF54(globalCtx, &self->actor, 7);
                gSaveContext.unk_13EE = 0x32;
                gSaveContext.eventChkInf[4] |= 1;
                self->actionFunc = EnZl4_Idle;
            }
            break;
    }
    self->unk_1E0.unk_18 = player->actor.world.pos;
    func_80034A14(&self->actor, &self->unk_1E0, 2, (self->csState == ZL4_CS_WINDOW) ? 2 : 1);
    if (EnZl4_InMovingAnim(self)) {
        EnZl4_SetMove(self, globalCtx);
    }
}

void EnZl4_Idle(EnZl4* self, GlobalContext* globalCtx) {
    func_800343CC(globalCtx, &self->actor, &self->unk_1E0.unk_00, self->collider.dim.radius + 60.0f, EnZl4_GetText,
                  func_80B5B9B0);
    func_80B5BB78(self, globalCtx);
}

void EnZl4_TheEnd(EnZl4* self, GlobalContext* globalCtx) {
    s32 animIndex[] = { ZL4_ANIM_0, ZL4_ANIM_0, ZL4_ANIM_0,  ZL4_ANIM_0,  ZL4_ANIM_0,
                        ZL4_ANIM_0, ZL4_ANIM_0, ZL4_ANIM_26, ZL4_ANIM_21, ZL4_ANIM_3 };
    CsCmdActorAction* npcAction;
    Vec3f pos;

    if (SkelAnime_Update(&self->skelAnime) && (self->skelAnime.animation == &gChildZeldaAnim_010DF8)) {
        func_80034EC0(&self->skelAnime, sAnimationEntries, ZL4_ANIM_4);
    }
    if (EnZl4_InMovingAnim(self)) {
        EnZl4_SetMove(self, globalCtx);
    }
    if (globalCtx->csCtx.frames == 100) {
        self->eyeExpression = ZL4_EYES_LOOK_LEFT;
    }
    if (globalCtx->csCtx.frames == 450) {
        self->blinkTimer = 3;
        self->eyeExpression = ZL4_EYES_NEUTRAL;
        self->mouthExpression = ZL4_MOUTH_SURPRISED;
    }
    npcAction = globalCtx->csCtx.npcActions[0];
    if (npcAction != NULL) {
        EnZl4_GetActionStartPos(npcAction, &pos);
        if (self->lastAction == 0) {
            self->actor.world.pos = self->actor.home.pos = pos;
        }
        if (self->lastAction != npcAction->action) {
            func_80034EC0(&self->skelAnime, sAnimationEntries, animIndex[npcAction->action]);
            self->lastAction = npcAction->action;
        }
        self->actor.velocity.x = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->actor.velocity.z = 0.0f;
    }
}

void EnZl4_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnZl4* self = THIS;

    if (self->actionFunc != EnZl4_TheEnd) {
        SkelAnime_Update(&self->skelAnime);
    }
    EnZl4_UpdateFace(self);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    self->actionFunc(self, globalCtx);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

s32 EnZl4_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnZl4* self = THIS;
    Vec3s sp1C;

    if (limbIndex == 17) {
        sp1C = self->unk_1E0.unk_08;
        Matrix_Translate(900.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateX((sp1C.y / (f32)0x8000) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((sp1C.x / (f32)0x8000) * M_PI, MTXMODE_APPLY);
        Matrix_Translate(-900.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }
    if (limbIndex == 10) {
        sp1C = self->unk_1E0.unk_0E;
        Matrix_RotateY((sp1C.y / (f32)0x8000) * M_PI, MTXMODE_APPLY);
        Matrix_RotateX((sp1C.x / (f32)0x8000) * M_PI, MTXMODE_APPLY);
    }
    if ((limbIndex >= 3) && (limbIndex < 7)) {
        *dList = NULL;
    }
    return false;
}

void EnZl4_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    EnZl4* self = THIS;

    if (limbIndex == 17) {
        Matrix_MultVec3f(&zeroVec, &self->actor.focus.pos);
    }
}

void EnZl4_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnZl4* self = THIS;
    void* mouthTex[] = { gChildZeldaMouthNeutralTex, gChildZeldaMouthHappyTex, gChildZeldaMouthWorriedTex,
                         gChildZeldaMouthSurprisedTex };
    void* eyeTex[] = {
        gChildZeldaEyeOpenTex,   gChildZeldaEyeBlinkTex, gChildZeldaEyeShutTex, gChildZeldaEyeWideTex,
        gChildZeldaEyeSquintTex, gChildZeldaEyeOutTex,   gChildZeldaEyeInTex,
    };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_zl4.c", 2012);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTex[self->rightEyeState]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTex[self->leftEyeState]));
    gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(mouthTex[self->mouthState]));
    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnZl4_OverrideLimbDraw, EnZl4_PostLimbDraw, self);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_zl4.c", 2043);
}
