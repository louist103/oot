/*
 * File: z_en_nb.c
 * Overlay: ovl_En_Nb
 * Description: Nabooru
 */

#include "z_en_nb.h"
#include "vt.h"
#include "objects/object_nb/object_nb.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"

#define FLAGS 0x00000010

#define THIS ((EnNb*)thisx)

typedef enum {
    /* 0x00 */ NB_CHAMBER_INIT,
    /* 0x01 */ NB_CHAMBER_UNDERGROUND,
    /* 0x02 */ NB_CHAMBER_APPEAR,
    /* 0x03 */ NB_CHAMBER_IDLE,
    /* 0x04 */ NB_CHAMBER_RAISE_ARM,
    /* 0x05 */ NB_CHAMBER_RAISE_ARM_TRANSITION,
    /* 0x06 */ NB_GIVE_MEDALLION,
    /* 0x07 */ NB_ACTION_7,
    /* 0x08 */ NB_SEAL_HIDE,
    /* 0x09 */ NB_ACTION_9,
    /* 0x0A */ NB_KIDNAPPED,
    /* 0x0B */ NB_KIDNAPPED_LOOK_AROUND,
    /* 0x0C */ NB_PORTAL_FALLTHROUGH,
    /* 0x0D */ NB_IN_CONFRONTATION,
    /* 0x0E */ NB_ACTION_14,
    /* 0x0F */ NB_KNEEL,
    /* 0x10 */ NB_LOOK_RIGHT,
    /* 0x11 */ NB_LOOK_LEFT,
    /* 0x12 */ NB_RUN,
    /* 0x13 */ NB_CONFRONTATION_DESTROYED,
    /* 0x14 */ NB_CREDITS_INIT,
    /* 0x15 */ NB_CREDITS_FADEIN,
    /* 0x16 */ NB_CREDITS_SIT,
    /* 0x17 */ NB_CREDITS_HEAD_TURN,
    /* 0x18 */ NB_CROUCH_CRAWLSPACE,
    /* 0x19 */ NB_NOTICE_PLAYER,
    /* 0x1A */ NB_IDLE_CRAWLSPACE,
    /* 0x1B */ NB_IN_DIALOG,
    /* 0x1C */ NB_IN_PATH,
    /* 0x1D */ NB_IDLE_AFTER_TALK,
    /* 0x1E */ NB_ACTION_30
} EnNbAction;

typedef enum {
    /* 0x00 */ NB_DRAW_NOTHING,
    /* 0x01 */ NB_DRAW_DEFAULT,
    /* 0x02 */ NB_DRAW_HIDE,
    /* 0x03 */ NB_DRAW_KNEEL,
    /* 0x04 */ NB_DRAW_LOOK_DIRECTION
} EnNbDrawMode;

void EnNb_Init(Actor* thisx, GlobalContext* globalCtx);
void EnNb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnNb_Update(Actor* thisx, GlobalContext* globalCtx);
void EnNb_Draw(Actor* thisx, GlobalContext* globalCtx);

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
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
    { 25, 80, 0, { 0, 0, 0 } },
};

static void* sEyeTextures[] = {
    gNabooruEyeOpenTex,
    gNabooruEyeHalfTex,
    gNabooruEyeClosedTex,
};

static s32 D_80AB4318 = 0;

#include "z_en_nb_cutscene_data.c" EARLY

s32 EnNb_GetPath(EnNb* self) {
    s32 path = self->actor.params >> 8;

    return path & 0xFF;
}

s32 EnNb_GetType(EnNb* self) {
    s32 type = self->actor.params;

    return type & 0xFF;
}

void EnNb_UpdatePath(EnNb* self, GlobalContext* globalCtx) {
    Vec3s* pointPos;
    Path* pathList;
    s32 pad;
    s32 path;

    pathList = globalCtx->setupPathList;

    if (pathList != NULL) {
        path = EnNb_GetPath(self);
        pathList += path;
        pointPos = SEGMENTED_TO_VIRTUAL(pathList->points);
        self->initialPos.x = pointPos[0].x;
        self->initialPos.y = pointPos[0].y;
        self->initialPos.z = pointPos[0].z;
        self->finalPos.x = pointPos[1].x;
        self->finalPos.y = pointPos[1].y;
        self->finalPos.z = pointPos[1].z;
        self->pathYaw = (Math_FAtan2F(self->finalPos.x - self->initialPos.x, self->finalPos.z - self->initialPos.z) *
                         (0x8000 / M_PI));
        // "En_Nb_Get_path_info Rail Data Get! = %d!!!!!!!!!!!!!!"
        osSyncPrintf("En_Nb_Get_path_info レールデータをゲットだぜ = %d!!!!!!!!!!!!!!\n", path);
    } else {
        // "En_Nb_Get_path_info Rail Data Doesn't Exist!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf("En_Nb_Get_path_info レールデータが無い!!!!!!!!!!!!!!!!!!!!\n");
    }
}

void EnNb_SetupCollider(Actor* thisx, GlobalContext* globalCtx) {
    EnNb* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, thisx, &sCylinderInit);
}

void EnNb_UpdateCollider(EnNb* self, GlobalContext* globalCtx) {
    s32 pad[4];
    ColliderCylinder* collider = &self->collider;

    Collider_UpdateCylinder(&self->actor, collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &collider->base);
}

void EnNb_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnNb* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80AB0FBC(EnNb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_300.unk_18 = player->actor.world.pos;
    self->unk_300.unk_14 = kREG(16) + 9.0f;
    func_80034A14(&self->actor, &self->unk_300, kREG(17) + 0xC, 2);
}

void func_80AB1040(EnNb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_300.unk_18 = player->actor.world.pos;
    self->unk_300.unk_14 = kREG(16) + 9.0f;
    func_80034A14(&self->actor, &self->unk_300, kREG(17) + 0xC, 4);
}

void func_80AB10C4(EnNb* self) {
    s32 pad2[2];
    Vec3s* tempPtr;
    Vec3s* tempPtr2;

    tempPtr = &self->unk_300.unk_08;
    Math_SmoothStepToS(&tempPtr->x, 0, 20, 6200, 100);
    Math_SmoothStepToS(&tempPtr->y, 0, 20, 6200, 100);
    tempPtr2 = &self->unk_300.unk_0E;
    Math_SmoothStepToS(&tempPtr2->x, 0, 20, 6200, 100);
    Math_SmoothStepToS(&tempPtr2->y, 0, 20, 6200, 100);
}

void EnNb_UpdateEyes(EnNb* self) {
    s32 pad[3];
    s16* blinkTimer = &self->blinkTimer;
    s16* eyeIdx = &self->eyeIdx;

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset(60, 60);
    }

    *eyeIdx = *blinkTimer;
    if (*eyeIdx >= ARRAY_COUNT(sEyeTextures)) {
        *eyeIdx = 0;
    }
}

void func_80AB11EC(EnNb* self) {
    self->action = NB_ACTION_7;
    self->drawMode = NB_DRAW_NOTHING;
    self->alpha = 0;
    self->flag = 0;
    self->actor.shape.shadowAlpha = 0;
    self->alphaTimer = 0.0f;
}

void func_80AB1210(EnNb* self, GlobalContext* globalCtx) {
    s32 one; // required to match

    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        if (D_80AB4318) {
            if (self->actor.params == NB_TYPE_DEMO02) {
                func_80AB11EC(self);
            }

            D_80AB4318 = 0;
        }
    } else {
        one = 1;
        if (!D_80AB4318) {
            D_80AB4318 = one;
        }
    }
}

void func_80AB1284(EnNb* self, GlobalContext* globalCtx) {
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 75.0f, 30.0f, 30.0f, 4);
}

s32 EnNb_FrameUpdateMatrix(EnNb* self) {
    return SkelAnime_Update(&self->skelAnime);
}

CsCmdActorAction* EnNb_GetNpcCsAction(GlobalContext* globalCtx, s32 npcActionIdx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        return globalCtx->csCtx.npcActions[npcActionIdx];
    }
    return NULL;
}

void EnNb_SetupCsPosRot(EnNb* self, GlobalContext* globalCtx, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction = EnNb_GetNpcCsAction(globalCtx, npcActionIdx);
    s16 newRotY;
    Actor* thisx = &self->actor;

    if (csCmdNPCAction != NULL) {
        thisx->world.pos.x = csCmdNPCAction->startPos.x;
        thisx->world.pos.y = csCmdNPCAction->startPos.y;
        thisx->world.pos.z = csCmdNPCAction->startPos.z;
        thisx->world.rot.y = thisx->shape.rot.y = csCmdNPCAction->rot.y;
    }
}

s32 func_80AB1390(EnNb* self, GlobalContext* globalCtx, u16 arg2, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction;

    if ((globalCtx->csCtx.state != CS_STATE_IDLE) &&
        (csCmdNPCAction = globalCtx->csCtx.npcActions[npcActionIdx], csCmdNPCAction != NULL) &&
        (csCmdNPCAction->action == arg2)) {
        return true;
    }
    return false;
}

s32 func_80AB13D8(EnNb* self, GlobalContext* globalCtx, u16 arg2, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction;

    if ((globalCtx->csCtx.state != CS_STATE_IDLE) &&
        (csCmdNPCAction = globalCtx->csCtx.npcActions[npcActionIdx], csCmdNPCAction != NULL) &&
        (csCmdNPCAction->action != arg2)) {
        return true;
    }
    return false;
}

void EnNb_SetInitialCsPosRot(EnNb* self, GlobalContext* globalCtx, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction = EnNb_GetNpcCsAction(globalCtx, npcActionIdx);
    Actor* thisx = &self->actor;

    if (csCmdNPCAction != NULL) {
        thisx->world.pos.x = csCmdNPCAction->startPos.x;
        thisx->world.pos.y = csCmdNPCAction->startPos.y;
        thisx->world.pos.z = csCmdNPCAction->startPos.z;
        thisx->world.rot.y = thisx->shape.rot.y = csCmdNPCAction->rot.y;
    }
}

void EnNb_SetCurrentAnim(EnNb* self, AnimationHeader* animation, u8 mode, f32 transitionRate, s32 arg4) {
    f32 frameCount = Animation_GetLastFrame(animation);
    f32 playbackSpeed;
    f32 unk0;
    f32 fc;

    if (!arg4) {
        unk0 = 0.0f;
        fc = frameCount;
        playbackSpeed = 1.0f;
    } else {
        unk0 = frameCount;
        fc = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change(&self->skelAnime, animation, playbackSpeed, unk0, fc, mode, transitionRate);
}

void EnNb_SetChamberAnim(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetCurrentAnim(self, &gNabooruStandingHandsOnHipsChamberOfSagesAnim, 0, 0, 0);
    self->actor.shape.yOffset = -10000.0f;
}

void EnNb_SpawnBlueWarp(EnNb* self, GlobalContext* globalCtx) {
    f32 posX = self->actor.world.pos.x;
    f32 posY = self->actor.world.pos.y;
    f32 posZ = self->actor.world.pos.z;

    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, posX, posY, posZ, 0, 0, 0,
                       WARP_SAGES);
}

void EnNb_GiveMedallion(EnNb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 posX = player->actor.world.pos.x;
    f32 posY = player->actor.world.pos.y + 50.0f;
    f32 posZ = player->actor.world.pos.z;

    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_EFFECT, posX, posY, posZ, 0, 0, 0,
                       0xC);
    Item_Give(globalCtx, ITEM_MEDALLION_SPIRIT);
}

void EnNb_ComeUpImpl(EnNb* self, GlobalContext* globalCtx) {
    self->actor.shape.yOffset += 250.0f / 3.0f;
}

void EnNb_SetupChamberCsImpl(EnNb* self, GlobalContext* globalCtx) {
    s32 pad[2];
    Player* player;

    if ((gSaveContext.chamberCutsceneNum == 3) && (gSaveContext.sceneSetupIndex < 4)) {
        player = GET_PLAYER(globalCtx);
        self->action = NB_CHAMBER_UNDERGROUND;
        globalCtx->csCtx.segment = &D_80AB431C;
        gSaveContext.cutsceneTrigger = 2;
        Item_Give(globalCtx, ITEM_MEDALLION_SPIRIT);
        player->actor.world.rot.y = player->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
    }
}

void EnNb_SetupChamberWarpImpl(EnNb* self, GlobalContext* globalCtx) {
    CutsceneContext* csCtx = &globalCtx->csCtx;
    CsCmdActorAction* csCmdNPCAction;

    if (csCtx->state != CS_STATE_IDLE) {
        csCmdNPCAction = csCtx->npcActions[1];
        if (csCmdNPCAction != NULL && csCmdNPCAction->action == 2) {
            self->action = NB_CHAMBER_APPEAR;
            self->drawMode = NB_DRAW_DEFAULT;
            EnNb_SpawnBlueWarp(self, globalCtx);
        }
    }
}

void EnNb_SetupDefaultChamberIdle(EnNb* self) {
    if (self->actor.shape.yOffset >= 0.0f) {
        self->action = NB_CHAMBER_IDLE;
        self->actor.shape.yOffset = 0.0f;
    }
}

void EnNb_SetupArmRaise(EnNb* self, GlobalContext* globalCtx) {
    AnimationHeader* animation = &gNabooruRaisingArmsGivingMedallionAnim;
    CsCmdActorAction* csCmdNPCAction;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        csCmdNPCAction = globalCtx->csCtx.npcActions[1];
        if (csCmdNPCAction != NULL && csCmdNPCAction->action == 3) {
            Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_ONCE,
                             0.0f);
            self->action = NB_CHAMBER_RAISE_ARM;
        }
    }
}

void EnNb_SetupRaisedArmTransition(EnNb* self, s32 animFinished) {
    AnimationHeader* animation = &gNabooruArmsRaisedGivingMedallionAnim;

    if (animFinished) {
        Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_LOOP,
                         0.0f);
        self->action = NB_CHAMBER_RAISE_ARM_TRANSITION;
    }
}

void EnNb_SetupMedallion(EnNb* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        csCmdNPCAction = globalCtx->csCtx.npcActions[6];
        if (csCmdNPCAction != NULL && csCmdNPCAction->action == 2) {
            self->action = NB_GIVE_MEDALLION;
            EnNb_GiveMedallion(self, globalCtx);
        }
    }
}

// Action func is never explicitly set to self, but it runs when the memory gets zero cleared
void EnNb_SetupChamberCs(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetupChamberCsImpl(self, globalCtx);
}

void EnNb_SetupChamberWarp(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetupChamberWarpImpl(self, globalCtx);
}

void EnNb_ComeUp(EnNb* self, GlobalContext* globalCtx) {
    EnNb_ComeUpImpl(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_SetupDefaultChamberIdle(self);
}

void func_80AB193C(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_SetupArmRaise(self, globalCtx);
}

void EnNb_RaiseArm(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    func_80AB1284(self, globalCtx);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_SetupRaisedArmTransition(self, animFinished);
}

void func_80AB19BC(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_SetupMedallion(self, globalCtx);
}

void func_80AB19FC(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
}

void EnNb_SetupLightArrowOrSealingCs(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetCurrentAnim(self, &gNabooruPuttingHandsTogetherCastingMagicAnim, 2, 0.0f, 0);
    self->action = NB_ACTION_7;
    self->actor.shape.shadowAlpha = 0;
}

void EnNb_PlaySealingSound(void) {
    func_800788CC(NA_SE_SY_WHITE_OUT_T);
}

void EnNb_InitializeDemo6K(EnNb* self, GlobalContext* globalCtx) {
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_6K, self->actor.world.pos.x,
                       kREG(21) + 22.0f + self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 7);
}

void EnNb_SetupHide(EnNb* self, GlobalContext* globalCtx) {
    if (func_80AB1390(self, globalCtx, 4, 1)) {
        self->action = NB_SEAL_HIDE;
        self->drawMode = NB_DRAW_HIDE;
        self->alpha = 0;
        self->actor.shape.shadowAlpha = 0;
        self->alphaTimer = 0.0f;
        EnNb_PlaySealingSound();
    }
}

void EnNb_CheckToFade(EnNb* self, GlobalContext* globalCtx) {
    f32* alphaTimer = &self->alphaTimer;
    s32 alpha;

    if (func_80AB1390(self, globalCtx, 4, 1)) {
        *alphaTimer += 1.0f;
        if (*alphaTimer >= kREG(5) + 10.0f) {
            self->action = NB_ACTION_9;
            self->drawMode = NB_DRAW_DEFAULT;
            *alphaTimer = kREG(5) + 10.0f;
            self->alpha = 255;
            self->actor.shape.shadowAlpha = 0xFF;
            return;
        }
    } else {
        *alphaTimer -= 1.0f;
        if (*alphaTimer <= 0.0f) {
            self->action = NB_ACTION_7;
            self->drawMode = NB_DRAW_NOTHING;
            *alphaTimer = 0.0f;
            self->alpha = 0;
            self->actor.shape.shadowAlpha = 0;
            return;
        }
    }

    alpha = (*alphaTimer / (kREG(5) + 10.0f)) * 255.0f;
    self->alpha = alpha;
    self->actor.shape.shadowAlpha = alpha;
}

void EnNb_SetupLightOrb(EnNb* self, GlobalContext* globalCtx) {
    if (func_80AB13D8(self, globalCtx, 4, 1)) {
        self->action = NB_SEAL_HIDE;
        self->drawMode = NB_DRAW_HIDE;
        self->alphaTimer = kREG(5) + 10.0f;
        self->alpha = 255;

        if (self->flag == 0) {
            EnNb_InitializeDemo6K(self, globalCtx);
            self->flag = 1;
        }

        self->actor.shape.shadowAlpha = 0xFF;
    }
}

void EnNb_Hide(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetupHide(self, globalCtx);
    func_80AB1210(self, globalCtx);
}

void EnNb_Fade(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_CheckToFade(self, globalCtx);
    func_80AB1210(self, globalCtx);
}

void EnNb_CreateLightOrb(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_SetupLightOrb(self, globalCtx);
    func_80AB1210(self, globalCtx);
}

void EnNb_DrawTransparency(EnNb* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s16 eyeSegIdx = self->eyeIdx;
    void* eyeTex = sEyeTextures[eyeSegIdx];
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_nb_inKenjyanomaDemo02.c", 263);

    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTex));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->alpha);
    gSPSegment(POLY_XLU_DISP++, 0x0C, &D_80116280[0]);
    POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       NULL, NULL, NULL, POLY_XLU_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_nb_inKenjyanomaDemo02.c", 290);
}

void EnNb_InitKidnap(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetCurrentAnim(self, &gNabooruTrappedInVortexPushingGroundAnim, 0, 0.0f, 0);
    self->action = NB_KIDNAPPED;
    self->actor.shape.shadowAlpha = 0;
    gSaveContext.eventChkInf[9] |= 0x20;
}

void EnNb_PlayCrySFX(EnNb* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.frames == 3) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_NB_CRY_0);
    }
}

void EnNb_PlayAgonySFX(EnNb* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.frames == 420) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_NB_AGONY);
    }
}

void EnNb_SetPosInPortal(EnNb* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction = EnNb_GetNpcCsAction(globalCtx, 1);
    Vec3f* pos = &self->actor.world.pos;
    f32 f0;
    s32 pad;
    Vec3f startPos;
    Vec3f endPos;

    if (csCmdNPCAction != NULL) {
        f0 = Environment_LerpWeightAccelDecel(csCmdNPCAction->endFrame, csCmdNPCAction->startFrame,
                                              globalCtx->csCtx.frames, 4, 4);
        startPos.x = csCmdNPCAction->startPos.x;
        startPos.y = csCmdNPCAction->startPos.y;
        startPos.z = csCmdNPCAction->startPos.z;
        endPos.x = csCmdNPCAction->endPos.x;
        endPos.y = csCmdNPCAction->endPos.y;
        endPos.z = csCmdNPCAction->endPos.z;
        pos->x = ((endPos.x - startPos.x) * f0) + startPos.x;
        pos->y = ((endPos.y - startPos.y) * f0) + startPos.y;
        pos->z = ((endPos.z - startPos.z) * f0) + startPos.z;
    }
}

void EnNb_SetupCaptureCutsceneState(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetupCsPosRot(self, globalCtx, 1);
    self->action = NB_KIDNAPPED;
    self->drawMode = NB_DRAW_NOTHING;
    self->actor.shape.shadowAlpha = 0;
}

void EnNb_SetRaisedArmCaptureAnim(EnNb* self, s32 animFinished) {
    AnimationHeader* animation = &gNabooruSuckedByVortexAnim;

    if (animFinished) {
        Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_LOOP,
                         0.0f);
    }
}

void EnNb_SetupLookAroundInKidnap(EnNb* self) {
    AnimationHeader* animation = &gNabooruTrappedInVortexPushingGroundAnim;

    Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_LOOP, -8.0f);
    self->action = NB_KIDNAPPED_LOOK_AROUND;
    self->drawMode = NB_DRAW_DEFAULT;
}

void EnNb_SetupKidnap(EnNb* self) {
    AnimationHeader* animation = &gNabooruTrappedInVortexRaisingArmAnim;

    Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_ONCE, -8.0f);
    self->action = NB_PORTAL_FALLTHROUGH;
    self->drawMode = NB_DRAW_DEFAULT;
}

void EnNb_CheckKidnapCsMode(EnNb* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction = EnNb_GetNpcCsAction(globalCtx, 1);
    s32 action;
    s32 previousCsAction;

    if (csCmdNPCAction != NULL) {
        action = csCmdNPCAction->action;
        previousCsAction = self->previousCsAction;
        if (action != previousCsAction) {
            switch (action) {
                case 1:
                    EnNb_SetupCaptureCutsceneState(self, globalCtx);
                    break;
                case 7:
                    EnNb_SetupLookAroundInKidnap(self);
                    break;
                case 8:
                    EnNb_SetupKidnap(self);
                    break;
                case 9:
                    Actor_Kill(&self->actor);
                    break;
                default:
                    // "Operation Doesn't Exist!!!!!!!!"
                    osSyncPrintf("En_Nb_Kidnap_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
                    break;
            }
            self->previousCsAction = action;
        }
    }
}

void func_80AB23A8(EnNb* self, GlobalContext* globalCtx) {
    EnNb_PlayCrySFX(self, globalCtx);
    EnNb_CheckKidnapCsMode(self, globalCtx);
}

void EnNb_MovingInPortal(EnNb* self, GlobalContext* globalCtx) {
    EnNb_PlayCrySFX(self, globalCtx);
    EnNb_PlayAgonySFX(self, globalCtx);
    EnNb_UpdateEyes(self);
    EnNb_FrameUpdateMatrix(self);
    EnNb_CheckKidnapCsMode(self, globalCtx);
}

void EnNb_SuckedInByPortal(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    EnNb_UpdateEyes(self);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_SetRaisedArmCaptureAnim(self, animFinished);
    EnNb_SetPosInPortal(self, globalCtx);
    EnNb_CheckKidnapCsMode(self, globalCtx);
}

void EnNb_SetupConfrontation(EnNb* self, GlobalContext* globalCtx) {
    AnimationHeader* animation = &gNabooruCollapseFromStandingToKneelingTransitionAnim;

    EnNb_SetCurrentAnim(self, animation, 0, 0.0f, 0);
    self->action = NB_IN_CONFRONTATION;
    self->actor.shape.shadowAlpha = 0;
}

void EnNb_PlayKnuckleDefeatSFX(EnNb* self, GlobalContext* globalCtx) {
    s32 pad[2];

    if (globalCtx->csCtx.frames == 548) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_NB_CRY_0);
        func_80078914(&self->actor.projectedPos, NA_SE_EN_FANTOM_HIT_THUNDER);
    }
}

void EnNb_PlayKneelingOnGroundSFX(EnNb* self) {
    s32 pad[2];

    if ((self->skelAnime.mode == 2) &&
        (Animation_OnFrame(&self->skelAnime, 18.0f) || Animation_OnFrame(&self->skelAnime, 25.0f))) {
        func_80078914(&self->actor.projectedPos, NA_SE_EV_HUMAN_BOUND);
    }
}

void EnNb_PlayLookRightSFX(EnNb* self) {
    s32 pad[2];

    if ((self->skelAnime.mode == 2) && Animation_OnFrame(&self->skelAnime, 9.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_WALK_CONCRETE);
    }
}

void EnNb_PlayLookLeftSFX(EnNb* self) {
    s32 pad[2];

    if (Animation_OnFrame(&self->skelAnime, 9.0f) || Animation_OnFrame(&self->skelAnime, 13.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_WALK_CONCRETE);
    }
}

void EnNb_InitDemo6KInConfrontation(EnNb* self, GlobalContext* globalCtx) {
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_6K, self->actor.world.pos.x,
                kREG(21) + 22.0f + self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0xB);
}

void func_80AB2688(EnNb* self, GlobalContext* globalCtx) {
    self->skelAnime.moveFlags |= 1;
    AnimationContext_SetMoveActor(globalCtx, &self->actor, &self->skelAnime, 1.0f);
}

void func_80AB26C8(EnNb* self) {
    self->action = NB_IN_CONFRONTATION;
    self->drawMode = NB_DRAW_NOTHING;
    self->actor.shape.shadowAlpha = 0;
}

void func_80AB26DC(EnNb* self, GlobalContext* globalCtx) {
    s32 pad;
    AnimationHeader* animation = &gNabooruCollapseFromStandingToKneelingTransitionAnim;
    f32 lastFrame = Animation_GetLastFrame(animation);

    EnNb_SetupCsPosRot(self, globalCtx, 1);
    Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, 0.0f);
    self->action = NB_ACTION_14;
    self->drawMode = NB_DRAW_KNEEL;
    self->actor.shape.shadowAlpha = 0xFF;
}

void EnNb_SetupKneel(EnNb* self) {
    AnimationHeader* animation = &gNabooruCollapseFromStandingToKneelingTransitionAnim;
    f32 lastFrame = Animation_GetLastFrame(animation);

    Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, 0.0f);
    self->action = NB_KNEEL;
    self->drawMode = NB_DRAW_KNEEL;
    self->actor.shape.shadowAlpha = 0xFF;
}

void EnNb_CheckIfKneeling(EnNb* self, s32 animFinished) {
    AnimationHeader* animation = &gNabooruOnAllFoursAnim;

    if (animFinished) {
        Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_LOOP,
                         0.0f);
        self->drawMode = NB_DRAW_KNEEL;
    }
}

void EnNb_SetupLookRight(EnNb* self) {
    AnimationHeader* animation = &gNabooruOnAllFoursToOnOneKneeLookingRightTransitionAnim;
    f32 lastFrame = Animation_GetLastFrame(animation);

    Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -8.0f);
    self->action = NB_LOOK_RIGHT;
    self->drawMode = NB_DRAW_DEFAULT;
    self->actor.shape.shadowAlpha = 0xFF;
}

void EnNb_CheckIfLookingRight(EnNb* self, s32 animFinished) {
    AnimationHeader* animation = &gNabooruOnOneKneeLookingRightAnim;

    if (animFinished) {
        Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_LOOP,
                         0.0f);
        self->drawMode = NB_DRAW_LOOK_DIRECTION;
    }
}

void EnNb_SetupLookLeft(EnNb* self) {
    AnimationHeader* animation = &gNabooruOnOneKneeTurningHeadRightToLeftTransitionAnim;
    f32 lastFrame = Animation_GetLastFrame(animation);

    Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -8.0f);
    self->action = NB_LOOK_LEFT;
    self->drawMode = NB_DRAW_LOOK_DIRECTION;
    self->actor.shape.shadowAlpha = 0xFF;
}

void EnNb_CheckIfLookLeft(EnNb* self, s32 animFinished) {
    AnimationHeader* animation = &gNabooruOnOneKneeLookingLeftAnim;

    if (animFinished) {
        Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_LOOP,
                         0.0f);
    }
}

void EnNb_SetupDemo6KInConfrontation(EnNb* self, GlobalContext* globalCtx, s32 animFinished) {
    if (!self->flag && animFinished) {
        EnNb_InitDemo6KInConfrontation(self, globalCtx);
        self->flag = 1;
    }
}

void EnNb_SetupRun(EnNb* self) {
    AnimationHeader* animation = &gNabooruKneeingToRunningToHitAnim;
    f32 lastFrame = Animation_GetLastFrame(animation);

    Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -8.0f);
    self->action = NB_RUN;
    self->drawMode = NB_DRAW_LOOK_DIRECTION;
    self->actor.shape.shadowAlpha = 0xFF;
}

void EnNb_SetupConfrontationDestroy(EnNb* self) {
    self->action = NB_CONFRONTATION_DESTROYED;
    self->drawMode = NB_DRAW_NOTHING;
    self->actor.shape.shadowAlpha = 0;
}

void EnNb_CheckConfrontationCsMode(EnNb* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction;
    s32 csAction;
    s32 previousCsAction;

    csCmdNPCAction = EnNb_GetNpcCsAction(globalCtx, 1);
    if (csCmdNPCAction != NULL) {
        csAction = csCmdNPCAction->action;
        previousCsAction = self->previousCsAction;

        if (csAction != previousCsAction) {
            switch (csAction) {
                case 1:
                    func_80AB26C8(self);
                    break;
                case 10:
                    func_80AB26DC(self, globalCtx);
                    break;
                case 11:
                    EnNb_SetupKneel(self);
                    break;
                case 12:
                    EnNb_SetupLookRight(self);
                    break;
                case 13:
                    EnNb_SetupLookLeft(self);
                    break;
                case 14:
                    EnNb_SetupRun(self);
                    break;
                case 9:
                    EnNb_SetupConfrontationDestroy(self);
                    break;
                default:
                    // "En_Nb_Confrontion_Check_DemoMode: Operation doesn't exist!!!!!!!!"
                    osSyncPrintf("En_Nb_Confrontion_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
                    break;
            }
            self->previousCsAction = csAction;
        }
    }
}

void EnNb_CheckConfrontationCsModeWrapper(EnNb* self, GlobalContext* globalCtx) {
    EnNb_CheckConfrontationCsMode(self, globalCtx);
}

void func_80AB2C18(EnNb* self, GlobalContext* globalCtx) {
    EnNb_UpdateEyes(self);
    func_80AB2688(self, globalCtx);
    func_80AB1284(self, globalCtx);
    EnNb_CheckConfrontationCsMode(self, globalCtx);
}

void EnNb_Kneel(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    EnNb_UpdateEyes(self);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_CheckIfKneeling(self, animFinished);
    EnNb_PlayKneelingOnGroundSFX(self);
    func_80AB2688(self, globalCtx);
    func_80AB1284(self, globalCtx);
    EnNb_CheckConfrontationCsMode(self, globalCtx);
}

void EnNb_LookRight(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    EnNb_UpdateEyes(self);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_CheckIfLookingRight(self, animFinished);
    EnNb_PlayLookRightSFX(self);
    func_80AB2688(self, globalCtx);
    func_80AB1284(self, globalCtx);
    EnNb_CheckConfrontationCsMode(self, globalCtx);
}

void EnNb_LookLeft(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    EnNb_UpdateEyes(self);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_CheckIfLookLeft(self, animFinished);
    func_80AB2688(self, globalCtx);
    func_80AB1284(self, globalCtx);
    EnNb_CheckConfrontationCsMode(self, globalCtx);
}

void EnNb_Run(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    EnNb_PlayKnuckleDefeatSFX(self, globalCtx);
    EnNb_UpdateEyes(self);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_PlayLookLeftSFX(self);
    func_80AB2688(self, globalCtx);
    func_80AB1284(self, globalCtx);
    EnNb_SetupDemo6KInConfrontation(self, globalCtx, animFinished);
    EnNb_CheckConfrontationCsMode(self, globalCtx);
}

void EnNb_ConfrontationDestroy(EnNb* self, GlobalContext* globalCtx) {
    self->timer++;

    if (self->timer > 60.0f) {
        Actor_Kill(&self->actor);
    }
}

void func_80AB2E70(EnNb* self, GlobalContext* globalCtx) {
    s32 pad;
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_nb_inConfrontion.c", 572);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(&gNabooruEyeWideTex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(&gNabooruEyeWideTex));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);
    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL, NULL,
                          &self->actor);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_nb_inConfrontion.c", 593);
}

s32 func_80AB2FC0(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnNb* self = THIS;

    if (limbIndex == NB_LIMB_HEAD) {
        *dList = gNabooruHeadMouthOpenDL;
    }

    return 0;
}

void func_80AB2FE4(EnNb* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 eyeIdx = self->eyeIdx;
    SkelAnime* skelAnime = &self->skelAnime;
    void* eyeTexture = sEyeTextures[eyeIdx];
    s32 pad1;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_nb_inConfrontion.c", 623);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);
    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, func_80AB2FC0,
                          NULL, &self->actor);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_nb_inConfrontion.c", 644);
}

void EnNb_SetupCreditsSpawn(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetCurrentAnim(self, &gNabooruSittingCrossLeggedAnim, 0, 0.0f, 0);
    self->action = NB_CREDITS_INIT;
    self->drawMode = NB_DRAW_NOTHING;
    self->actor.shape.shadowAlpha = 0;
}

void EnNb_SetAlphaInCredits(EnNb* self) {
    f32* alphaTimer = &self->alphaTimer;
    s32 alpha;

    self->alphaTimer++;

    if ((kREG(17) + 10.0f) <= self->alphaTimer) {
        self->alpha = 255;
        self->actor.shape.shadowAlpha = 255;
    } else {
        alpha = (*alphaTimer / (kREG(17) + 10.0f)) * 255.0f;
        self->alpha = alpha;
        self->actor.shape.shadowAlpha = alpha;
    }
}

void EnNb_SetupCreditsFadeIn(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetInitialCsPosRot(self, globalCtx, 1);
    self->action = NB_CREDITS_FADEIN;
    self->drawMode = NB_DRAW_HIDE;
}

void EnNb_SetupCreditsSit(EnNb* self) {
    if (self->alphaTimer >= kREG(17) + 10.0f) {
        self->action = NB_CREDITS_SIT;
        self->drawMode = NB_DRAW_DEFAULT;
    }
}

void EnNb_SetupCreditsHeadTurn(EnNb* self) {
    EnNb_SetCurrentAnim(self, &gNabooruSittingCrossLeggedTurningToLookUpRightTransitionAnim, 2, -8.0f, 0);
    self->action = NB_CREDITS_HEAD_TURN;
}

void EnNb_CheckIfLookingUp(EnNb* self, s32 animFinished) {
    if (animFinished) {
        EnNb_SetCurrentAnim(self, &gNabooruSittingCrossLeggedLookingUpRightAnim, 0, 0.0f, 0);
    }
}

void EnNb_CheckCreditsCsModeImpl(EnNb* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction = EnNb_GetNpcCsAction(globalCtx, 1);
    s32 action;
    s32 previousCsAction;

    if (csCmdNPCAction != NULL) {
        action = csCmdNPCAction->action;
        previousCsAction = self->previousCsAction;
        if (action != previousCsAction) {
            switch (action) {
                case 15:
                    EnNb_SetupCreditsFadeIn(self, globalCtx);
                    break;
                case 16:
                    EnNb_SetupCreditsHeadTurn(self);
                    break;
                default:
                    // "En_Nb_inEnding_Check_DemoMode: Operation doesn't exist!!!!!!!!"
                    osSyncPrintf("En_Nb_inEnding_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
                    break;
            }
            self->previousCsAction = action;
        }
    }
}

void EnNb_CheckCreditsCsMode(EnNb* self, GlobalContext* globalCtx) {
    EnNb_CheckCreditsCsModeImpl(self, globalCtx);
}

void EnNb_CreditsFade(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_SetAlphaInCredits(self);
    EnNb_SetupCreditsSit(self);
}

void func_80AB3428(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_CheckCreditsCsModeImpl(self, globalCtx);
}

void EnNb_LookUp(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    func_80AB1284(self, globalCtx);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_CheckIfLookingUp(self, animFinished);
}

void EnNb_CrawlspaceSpawnCheck(EnNb* self, GlobalContext* globalCtx) {
    if (!(gSaveContext.eventChkInf[9] & 0x20) && LINK_IS_CHILD) {
        EnNb_UpdatePath(self, globalCtx);

        // looking into crawlspace
        if (!(gSaveContext.eventChkInf[9] & 0x10)) {
            EnNb_SetCurrentAnim(self, &gNabooruKneeingAtCrawlspaceAnim, 0, 0.0f, 0);
            self->action = NB_CROUCH_CRAWLSPACE;
            self->drawMode = NB_DRAW_DEFAULT;
        } else {
            EnNb_SetCurrentAnim(self, &gNabooruStandingHandsOnHipsAnim, 0, 0.0f, 0);
            self->headTurnFlag = 1;
            self->actor.flags |= 9;
            self->actor.world.pos = self->finalPos;
            self->action = NB_IDLE_AFTER_TALK;
            self->drawMode = NB_DRAW_DEFAULT;
        }
    } else {
        Actor_Kill(&self->actor);
    }
}

void func_80AB359C(EnNb* self) {
    PosRot* world = &self->actor.world;
    Vec3f* initialPos = &self->initialPos;
    Vec3f* finalPos = &self->finalPos;
    f32 f0;
    u16 temp_t1;
    s16 temp_2;

    self->movementTimer++;
    temp_2 = kREG(17);
    temp_t1 = temp_2;
    temp_t1 += 25;

    if (temp_t1 >= self->movementTimer) {
        f0 = Environment_LerpWeightAccelDecel(temp_t1, 0, self->movementTimer, 3, 3);
        world->pos.x = initialPos->x + (f0 * (finalPos->x - initialPos->x));
        world->pos.y = initialPos->y + (f0 * (finalPos->y - initialPos->y));
        world->pos.z = initialPos->z + (f0 * (finalPos->z - initialPos->z));
    }
}

void EnNb_SetNoticeSFX(EnNb* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_VO_NB_NOTICE);
}

s32 EnNb_GetNoticedStatus(EnNb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 playerX = player->actor.world.pos.x;
    f32 playerZ = player->actor.world.pos.z;
    f32 thisX = self->actor.world.pos.x;
    f32 thisZ = self->actor.world.pos.z;

    if (SQ(playerX - thisX) + SQ(playerZ - thisZ) < SQ(80.0f)) {
        return true;
    } else {
        return false;
    }
}

void func_80AB36DC(EnNb* self, GlobalContext* globalCtx) {
    u16 moveTime = self->movementTimer;

    if ((((u16)((u16)(kREG(17) + 25) - 4))) > moveTime) {
        s16 invScale = 4 - moveTime;
        if (invScale > 0) {
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->pathYaw, invScale, 6200, 100);
        }
    } else {
        s16 invScale = (u16)(kREG(17) + 25) - moveTime;
        if (invScale > 0) {
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, invScale, 6200, 100);
        }
    }
}

void EnNb_CheckNoticed(EnNb* self, GlobalContext* globalCtx) {
    if (EnNb_GetNoticedStatus(self, globalCtx)) {
        EnNb_SetCurrentAnim(self, &gNabooruStandingToWalkingTransitionAnim, 2, -8.0f, 0);
        self->action = NB_NOTICE_PLAYER;
        EnNb_SetNoticeSFX(self);
    }
}

void EnNb_SetupIdleCrawlspace(EnNb* self, s32 animFinished) {
    if (animFinished) {
        EnNb_SetCurrentAnim(self, &gNabooruStandingHandsOnHipsAnim, 0, -8.0f, 0);
        self->headTurnFlag = 1;
        self->actor.flags |= 9;
        self->action = NB_IDLE_CRAWLSPACE;
    }
}

void func_80AB3838(EnNb* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->action = NB_IN_DIALOG;
    } else {
        self->actor.flags |= 9;

        if (!(gSaveContext.infTable[22] & 0x1000)) {
            self->actor.textId = 0x601D;
        } else {
            self->actor.textId = 0x6024;
        }

        func_8002F2F4(&self->actor, globalCtx);
    }
}

void EnNb_SetupPathMovement(EnNb* self, GlobalContext* globalCtx) {
    EnNb_SetCurrentAnim(self, &gNabooruStandingToWalkingTransitionAnim, 2, -8.0f, 0);
    gSaveContext.eventChkInf[9] |= 0x10;
    self->action = NB_IN_PATH;
    self->actor.flags &= ~9;
}

void EnNb_SetTextIdAsChild(EnNb* self, GlobalContext* globalCtx) {
    s32 pad;
    u8 choiceIndex;
    s32 pad1;
    u16 textId;

    textId = self->actor.textId;

    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        if (textId == 0x6025) {
            EnNb_SetupPathMovement(self, globalCtx);
        } else {
            if (textId == 0x6027) {
                gSaveContext.infTable[22] |= 0x1000;
            }
            self->action = NB_IDLE_CRAWLSPACE;
        }
        self->actor.flags &= ~9;
    } else if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx) != 0)) {
        choiceIndex = globalCtx->msgCtx.choiceIndex;

        if (textId == 0x601D) {
            switch (choiceIndex) {
                case 0:
                    self->actor.textId = 0x601E;
                    break;
                case 1:
                    self->actor.textId = 0x601F;
                    break;
                default:
                    self->actor.textId = 0x6020;
            }
        } else if (textId == 0x6020) {
            switch (choiceIndex) {
                case 0:
                    self->actor.textId = 0x6021;
                    break;
                default:
                    self->actor.textId = 0x6022;
                    break;
            }
        } else {
            switch (choiceIndex) {
                case 0:
                    self->actor.textId = 0x6025;
                    break;
                default:
                    self->actor.textId = 0x6027;
                    break;
            }
        }

        func_8010B720(globalCtx, self->actor.textId);
    }
}

void func_80AB3A7C(EnNb* self, GlobalContext* globalCtx, s32 animFinished) {
    u16 movementTimer = self->movementTimer;

    if ((u16)(kREG(17) + 25) > movementTimer) {
        if (animFinished) {
            EnNb_SetCurrentAnim(self, &gNabooruWalkingAnim, 0, 0.0f, 0);
        }
    } else {
        EnNb_SetCurrentAnim(self, &gNabooruStandingHandsOnHipsAnim, 0, -8.0f, 0);
        self->action = NB_IDLE_AFTER_TALK;
    }
}

void func_80AB3B04(EnNb* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->action = NB_ACTION_30;
    } else {
        self->actor.flags |= 9;
        self->actor.textId = Text_GetFaceReaction(globalCtx, 0x23);

        if ((self->actor.textId) == 0) {
            self->actor.textId = 0x6026;
        }

        func_8002F2F4(&self->actor, globalCtx);
    }
}

void func_80AB3B7C(EnNb* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        self->action = NB_IDLE_AFTER_TALK;
        self->actor.flags &= ~9;
    }
}

void EnNb_WaitForNotice(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_UpdateCollider(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_CheckNoticed(self, globalCtx);
}

void EnNb_StandUpAfterNotice(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    func_80AB1284(self, globalCtx);
    EnNb_UpdateCollider(self, globalCtx);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_SetupIdleCrawlspace(self, animFinished);
}

void EnNb_BlockCrawlspace(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_UpdateCollider(self, globalCtx);
    func_80AB0FBC(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    func_80AB3838(self, globalCtx);
}

void EnNb_InitCrawlspaceDialogue(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_UpdateCollider(self, globalCtx);
    func_80AB0FBC(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    EnNb_SetTextIdAsChild(self, globalCtx);
}

void EnNb_FollowPath(EnNb* self, GlobalContext* globalCtx) {
    s32 animFinished;

    func_80AB359C(self);
    func_80AB1284(self, globalCtx);
    EnNb_UpdateCollider(self, globalCtx);
    func_80AB36DC(self, globalCtx);
    func_80AB10C4(self);
    animFinished = EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    func_80AB3A7C(self, globalCtx, animFinished);
}

void func_80AB3DB0(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_UpdateCollider(self, globalCtx);
    func_80AB0FBC(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    func_80AB3B04(self, globalCtx);
}

void func_80AB3E10(EnNb* self, GlobalContext* globalCtx) {
    func_80AB1284(self, globalCtx);
    EnNb_UpdateCollider(self, globalCtx);
    func_80AB1040(self, globalCtx);
    EnNb_FrameUpdateMatrix(self);
    EnNb_UpdateEyes(self);
    func_80AB3B7C(self, globalCtx);
}

static EnNbActionFunc sActionFuncs[] = {
    EnNb_SetupChamberCs,
    EnNb_SetupChamberWarp,
    EnNb_ComeUp,
    func_80AB193C,
    EnNb_RaiseArm,
    func_80AB19BC,
    func_80AB19FC,
    EnNb_Hide,
    EnNb_Fade,
    EnNb_CreateLightOrb,
    func_80AB23A8,
    EnNb_MovingInPortal,
    EnNb_SuckedInByPortal,
    EnNb_CheckConfrontationCsModeWrapper,
    func_80AB2C18,
    EnNb_Kneel,
    EnNb_LookRight,
    EnNb_LookLeft,
    EnNb_Run,
    EnNb_ConfrontationDestroy,
    EnNb_CheckCreditsCsMode,
    EnNb_CreditsFade,
    func_80AB3428,
    EnNb_LookUp,
    EnNb_WaitForNotice,
    EnNb_StandUpAfterNotice,
    EnNb_BlockCrawlspace,
    EnNb_InitCrawlspaceDialogue,
    EnNb_FollowPath,
    func_80AB3DB0,
    func_80AB3E10,
};

void EnNb_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnNb* self = THIS;

    if (self->action < 0 || self->action > 30 || sActionFuncs[self->action] == NULL) {
        // "Main mode is wrong!!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }

    sActionFuncs[self->action](self, globalCtx);
}

void EnNb_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnNb* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    EnNb_SetupCollider(thisx, globalCtx);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gNabooruSkel, NULL, self->jointTable, self->morphTable,
                       NB_LIMB_MAX);

    switch (EnNb_GetType(self)) {
        case NB_TYPE_DEMO02:
            EnNb_SetupLightArrowOrSealingCs(self, globalCtx);
            break;
        case NB_TYPE_KIDNAPPED:
            EnNb_InitKidnap(self, globalCtx);
            break;
        case NB_TYPE_KNUCKLE:
            EnNb_SetupConfrontation(self, globalCtx);
            break;
        case NB_TYPE_CREDITS:
            EnNb_SetupCreditsSpawn(self, globalCtx);
            break;
        case NB_TYPE_CRAWLSPACE:
            EnNb_CrawlspaceSpawnCheck(self, globalCtx);
            break;
        default: // giving medallion
            EnNb_SetChamberAnim(self, globalCtx);
            break;
    }
}

s32 EnNb_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnNb* self = THIS;
    struct_80034A14_arg1* unk_300 = &self->unk_300;
    s32 ret = false;

    if (self->headTurnFlag != 0) {
        if (limbIndex == NB_LIMB_TORSO) {
            rot->x += unk_300->unk_0E.y;
            rot->y -= unk_300->unk_0E.x;
            ret = false;
        } else if (limbIndex == NB_LIMB_HEAD) {
            rot->x += unk_300->unk_08.y;
            rot->z += unk_300->unk_08.x;
            ret = false;
        }
    }

    return ret;
}

void EnNb_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnNb* self = THIS;

    if (limbIndex == NB_LIMB_HEAD) {
        Vec3f vec1 = { 0.0f, 10.0f, 0.0f };
        Vec3f vec2;

        Matrix_MultVec3f(&vec1, &vec2);
        self->actor.focus.pos.x = vec2.x;
        self->actor.focus.pos.y = vec2.y;
        self->actor.focus.pos.z = vec2.z;
        self->actor.focus.rot.x = self->actor.world.rot.x;
        self->actor.focus.rot.y = self->actor.world.rot.y;
        self->actor.focus.rot.z = self->actor.world.rot.z;
    }
}

void EnNb_DrawNothing(EnNb* self, GlobalContext* globalCtx) {
}

void EnNb_DrawDefault(EnNb* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 eyeIdx = self->eyeIdx;
    SkelAnime* skelAnime = &self->skelAnime;
    void* eyeTexture = sEyeTextures[eyeIdx];
    s32 pad1;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_nb.c", 992);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);
    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                          EnNb_OverrideLimbDraw, EnNb_PostLimbDraw, &self->actor);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_nb.c", 1013);
}

static EnNbDrawFunc sDrawFuncs[] = {
    EnNb_DrawNothing, EnNb_DrawDefault, EnNb_DrawTransparency, func_80AB2E70, func_80AB2FE4,
};

void EnNb_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnNb* self = THIS;

    if (self->drawMode < 0 || self->drawMode >= 5 || sDrawFuncs[self->drawMode] == NULL) {
        // "Draw mode is wrong!!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }

    sDrawFuncs[self->drawMode](self, globalCtx);
}

const ActorInit En_Nb_InitVars = {
    ACTOR_EN_NB,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_NB,
    sizeof(EnNb),
    (ActorFunc)EnNb_Init,
    (ActorFunc)EnNb_Destroy,
    (ActorFunc)EnNb_Update,
    (ActorFunc)EnNb_Draw,
};
