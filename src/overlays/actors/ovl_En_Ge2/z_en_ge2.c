/*
 * File: z_en_ge2.c
 * Overlay: ovl_En_Ge2
 * Description: Purple-clothed Gerudo
 */

#include "z_en_ge2.h"
#include "vt.h"
#include "objects/object_gla/object_gla.h"

#define FLAGS 0x00000019

#define THIS ((EnGe2*)thisx)

#define GE2_STATE_ANIMCOMPLETE (1 << 1)
#define GE2_STATE_KO (1 << 2)
#define GE2_STATE_CAPTURING (1 << 3)
#define GE2_STATE_TALKED (1 << 4)

typedef enum {
    /* 0 */ GE2_TYPE_PATROLLING,
    /* 1 */ GE2_TYPE_STATIONARY,
    /* 2 */ GE2_TYPE_GERUDO_CARD_GIVER
} EnGe2Type;

typedef enum {
    /* 0 */ GE2_ACTION_WALK,
    /* 1 */ GE2_ACTION_ABOUTTURN,
    /* 2 */ GE2_ACTION_TURNPLAYERSPOTTED,
    /* 3 */ GE2_ACTION_KNOCKEDOUT,
    /* 4 */ GE2_ACTION_CAPTURETURN,
    /* 5 */ GE2_ACTION_CAPTURECHARGE,
    /* 6 */ GE2_ACTION_CAPTURECLOSE,
    /* 7 */ GE2_ACTION_STAND,
    /* 8 */ GE2_ACTION_WAITLOOKATPLAYER
} EnGe2Action;

void EnGe2_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGe2_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGe2_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGe2_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 EnGe2_CheckCarpentersFreed(void);
void EnGe2_CaptureClose(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_CaptureCharge(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_CaptureTurn(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_KnockedOut(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_TurnPlayerSpotted(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_AboutTurn(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_Walk(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_Stand(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_WaitLookAtPlayer(EnGe2* self, GlobalContext* globalCtx);
void EnGe2_ForceTalk(EnGe2* self, GlobalContext* globalCtx);

// Update functions
void EnGe2_UpdateFriendly(Actor* thisx, GlobalContext* globalCtx);
void EnGe2_UpdateAfterTalk(Actor* thisx, GlobalContext* globalCtx);
void EnGe2_UpdateStunned(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Ge2_InitVars = {
    ACTOR_EN_GE2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GLA,
    sizeof(EnGe2),
    (ActorFunc)EnGe2_Init,
    (ActorFunc)EnGe2_Destroy,
    (ActorFunc)EnGe2_Update,
    (ActorFunc)EnGe2_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x000007A2, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 60, 0, { 0, 0, 0 } },
};

static EnGe2ActionFunc sActionFuncs[] = {
    EnGe2_Walk,         EnGe2_AboutTurn,   EnGe2_TurnPlayerSpotted,
    EnGe2_KnockedOut,   EnGe2_CaptureTurn, EnGe2_CaptureCharge,
    EnGe2_CaptureClose, EnGe2_Stand,       EnGe2_WaitLookAtPlayer,
};

static AnimationHeader* sAnimations[] = {
    &gGerudoPurpleWalkingAnim,         &gGerudoPurpleLookingAboutAnim, &gGerudoPurpleLookingAboutAnim,
    &gGerudoPurpleFallingToGroundAnim, &gGerudoPurpleLookingAboutAnim, &gGerudoPurpleChargingAnim,
    &gGerudoPurpleLookingAboutAnim,    &gGerudoPurpleLookingAboutAnim, &gGerudoPurpleLookingAboutAnim,
};

static u8 sAnimModes[] = {
    ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_LOOP, ANIMMODE_ONCE, ANIMMODE_LOOP,
    ANIMMODE_LOOP, ANIMMODE_LOOP, ANIMMODE_LOOP, ANIMMODE_ONCE,
};

void EnGe2_ChangeAction(EnGe2* self, s32 i) {
    self->actionFunc = sActionFuncs[i];
    Animation_Change(&self->skelAnime, sAnimations[i], 1.0f, 0.0f, Animation_GetLastFrame(sAnimations[i]),
                     sAnimModes[i], -8.0f);
    self->stateFlags &= ~GE2_STATE_ANIMCOMPLETE;
}

void EnGe2_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnGe2* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGerudoPurpleSkel, NULL, self->jointTable, self->morphTable, 22);
    Animation_PlayLoop(&self->skelAnime, &gGerudoPurpleWalkingAnim);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&self->actor, 0.01f);

    if (globalCtx->sceneNum == SCENE_SPOT09) {
        self->actor.uncullZoneForward = 1000.0f;
    } else {
        self->actor.uncullZoneForward = 1200.0f;
    }

    self->yDetectRange = (self->actor.world.rot.z + 1) * 40.0f;
    self->actor.world.rot.z = 0;
    self->actor.shape.rot.z = 0;

    switch (self->actor.params & 0xFF) {
        case GE2_TYPE_PATROLLING:
            EnGe2_ChangeAction(self, GE2_ACTION_WALK);
            if (EnGe2_CheckCarpentersFreed()) {
                self->actor.update = EnGe2_UpdateFriendly;
                self->actor.targetMode = 6;
            }
            break;
        case GE2_TYPE_STATIONARY:
            EnGe2_ChangeAction(self, GE2_ACTION_STAND);
            if (EnGe2_CheckCarpentersFreed()) {
                self->actor.update = EnGe2_UpdateFriendly;
                self->actor.targetMode = 6;
            }
            break;
        case GE2_TYPE_GERUDO_CARD_GIVER:
            EnGe2_ChangeAction(self, GE2_ACTION_WAITLOOKATPLAYER);
            self->actor.update = EnGe2_UpdateAfterTalk;
            self->actionFunc = EnGe2_ForceTalk;
            self->actor.targetMode = 6;
            break;
        default:
            ASSERT(0, "0", "../z_en_ge2.c", 418);
    }

    self->stateFlags = 0;
    self->unk_304 = 0; // Set and not used
    self->walkTimer = 0;
    self->playerSpottedParam = 0;
    self->actor.minVelocityY = -4.0f;
    self->actor.gravity = -1.0f;
    self->walkDirection = self->actor.world.rot.y;
    self->walkDuration = ((self->actor.params & 0xFF00) >> 8) * 10;
}

void EnGe2_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGe2* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

// Detection/check functions

s32 Ge2_DetectPlayerInAction(GlobalContext* globalCtx, EnGe2* self) {
    f32 visionScale;

    visionScale = (!IS_DAY ? 0.75f : 1.5f);

    if ((250.0f * visionScale) < self->actor.xzDistToPlayer) {
        return 0;
    }

    if (self->actor.xzDistToPlayer < 50.0f) {
        return 2;
    }

    if (func_8002DDE4(globalCtx)) {
        return 1;
    }
    return 0;
}

s32 Ge2_DetectPlayerInUpdate(GlobalContext* globalCtx, EnGe2* self, Vec3f* pos, s16 yRot, f32 yDetectRange) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f posResult;
    CollisionPoly* outPoly;
    f32 visionScale;

    visionScale = (!IS_DAY ? 0.75f : 1.5f);

    if ((250.0f * visionScale) < self->actor.xzDistToPlayer) {
        return 0;
    }

    if (yDetectRange < ABS(self->actor.yDistToPlayer)) {
        return 0;
    }

    if (ABS((s16)(self->actor.yawTowardsPlayer - yRot)) > 0x2000) {
        return 0;
    }

    if (BgCheck_AnyLineTest1(&globalCtx->colCtx, pos, &player->bodyPartsPos[7], &posResult, &outPoly, 0)) {
        return 0;
    }
    return 1;
}

s32 EnGe2_CheckCarpentersFreed(void) {
    if ((u8)(gSaveContext.eventChkInf[9] & 0xF) == 0xF) {
        return 1;
    }
    return 0;
}

// Actions

void EnGe2_CaptureClose(EnGe2* self, GlobalContext* globalCtx) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        func_8006D074(globalCtx);

        if ((INV_CONTENT(ITEM_HOOKSHOT) == ITEM_NONE) || (INV_CONTENT(ITEM_LONGSHOT) == ITEM_NONE)) {
            globalCtx->nextEntranceIndex = 0x1A5;
        } else if (gSaveContext.eventChkInf[12] & 0x80) {
            globalCtx->nextEntranceIndex = 0x5F8;
        } else {
            globalCtx->nextEntranceIndex = 0x3B4;
        }

        globalCtx->fadeTransition = 0x26;
        globalCtx->sceneLoadFlag = 0x14;
    }
}

void EnGe2_CaptureCharge(EnGe2* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2, 0x400, 0x100);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (self->actor.xzDistToPlayer < 50.0f) {
        EnGe2_ChangeAction(self, GE2_ACTION_CAPTURECLOSE);
        self->actor.speedXZ = 0.0f;
    }

    if (self->timer > 0) {
        self->timer--;
    } else {
        func_8006D074(globalCtx);

        if ((INV_CONTENT(ITEM_HOOKSHOT) == ITEM_NONE) || (INV_CONTENT(ITEM_LONGSHOT) == ITEM_NONE)) {
            globalCtx->nextEntranceIndex = 0x1A5;
        } else if (gSaveContext.eventChkInf[12] & 0x80) {
            globalCtx->nextEntranceIndex = 0x5F8;
        } else {
            globalCtx->nextEntranceIndex = 0x3B4;
        }

        globalCtx->fadeTransition = 0x26;
        globalCtx->sceneLoadFlag = 0x14;
    }
}

void EnGe2_CaptureTurn(EnGe2* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2, 0x400, 0x100);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->actor.world.rot.y == self->actor.yawTowardsPlayer) {
        EnGe2_ChangeAction(self, GE2_ACTION_CAPTURECHARGE);
        self->timer = 50;
        self->actor.speedXZ = 4.0f;
    }
}

void EnGe2_KnockedOut(EnGe2* self, GlobalContext* globalCtx) {
    static Vec3f effectVelocity = { 0.0f, -0.05f, 0.0f };
    static Vec3f effectAccel = { 0.0f, -0.025f, 0.0f };
    static Color_RGBA8 effectPrimColor = { 255, 255, 255, 0 };
    static Color_RGBA8 effectEnvColor = { 255, 150, 0, 0 };
    s32 effectAngle;
    Vec3f effectPos;

    self->actor.flags &= ~1;
    if (self->stateFlags & GE2_STATE_ANIMCOMPLETE) {
        effectAngle = (globalCtx->state.frames) * 0x2800;
        effectPos.x = self->actor.focus.pos.x + (Math_CosS(effectAngle) * 5.0f);
        effectPos.y = self->actor.focus.pos.y + 10.0f;
        effectPos.z = self->actor.focus.pos.z + (Math_SinS(effectAngle) * 5.0f);
        EffectSsKiraKira_SpawnDispersed(globalCtx, &effectPos, &effectVelocity, &effectAccel, &effectPrimColor,
                                        &effectEnvColor, 1000, 16);
    }
}

void EnGe2_TurnPlayerSpotted(EnGe2* self, GlobalContext* globalCtx) {
    s32 playerSpotted;

    self->actor.speedXZ = 0.0f;

    if (self->stateFlags & GE2_STATE_TALKED) {
        self->stateFlags &= ~GE2_STATE_TALKED;
    } else {
        playerSpotted = Ge2_DetectPlayerInAction(globalCtx, self);

        if (playerSpotted != 0) {
            self->timer = 100;
            self->yawTowardsPlayer = self->actor.yawTowardsPlayer;

            if (self->playerSpottedParam < playerSpotted) {
                self->playerSpottedParam = playerSpotted;
            }
        } else if (self->actor.world.rot.y == self->yawTowardsPlayer) {
            self->playerSpottedParam = 0;
            EnGe2_ChangeAction(self, GE2_ACTION_ABOUTTURN);
            return;
        }
    }

    switch (self->playerSpottedParam) {
        case 1:
            Math_SmoothStepToS(&self->actor.world.rot.y, self->yawTowardsPlayer, 2, 0x200, 0x100);
            break;
        case 2:
            Math_SmoothStepToS(&self->actor.world.rot.y, self->yawTowardsPlayer, 2, 0x600, 0x180);
            break;
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnGe2_AboutTurn(EnGe2* self, GlobalContext* globalCtx) {
    s32 playerSpotted;

    self->actor.speedXZ = 0.0f;
    playerSpotted = Ge2_DetectPlayerInAction(globalCtx, self);

    if (playerSpotted != 0) {
        EnGe2_ChangeAction(self, GE2_ACTION_TURNPLAYERSPOTTED);
        self->timer = 100;
        self->playerSpottedParam = playerSpotted;
        self->yawTowardsPlayer = self->actor.yawTowardsPlayer;
    } else if (self->stateFlags & GE2_STATE_ANIMCOMPLETE) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->walkDirection, 2, 0x400, 0x200);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    if (self->actor.shape.rot.y == self->walkDirection) {
        EnGe2_ChangeAction(self, GE2_ACTION_WALK);
    }
}

void EnGe2_Walk(EnGe2* self, GlobalContext* globalCtx) {
    u8 playerSpotted;

    playerSpotted = Ge2_DetectPlayerInAction(globalCtx, self);
    if (playerSpotted != 0) {
        self->actor.speedXZ = 0.0f;
        EnGe2_ChangeAction(self, GE2_ACTION_TURNPLAYERSPOTTED);
        self->timer = 100;
        self->playerSpottedParam = playerSpotted;
        self->yawTowardsPlayer = self->actor.yawTowardsPlayer;
    } else if (self->walkTimer >= self->walkDuration) {
        self->walkTimer = 0;
        self->walkDirection += 0x8000;
        EnGe2_ChangeAction(self, GE2_ACTION_ABOUTTURN);
        self->actor.speedXZ = 0.0f;
    } else {
        self->walkTimer++;
        self->actor.speedXZ = 2.0f;
    }
}

void EnGe2_Stand(EnGe2* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.world.rot.y, self->walkDirection, 2, 0x400, 0x200);
}

void EnGe2_TurnToFacePlayer(EnGe2* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(angleDiff) <= 0x4000) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 6, 4000, 100);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        func_80038290(globalCtx, &self->actor, &self->headRot, &self->unk_2EE, self->actor.focus.pos);
    } else {
        if (angleDiff < 0) {
            Math_SmoothStepToS(&self->headRot.y, -0x2000, 6, 6200, 0x100);
        } else {
            Math_SmoothStepToS(&self->headRot.y, 0x2000, 6, 6200, 0x100);
        }

        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 12, 1000, 100);
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
}

void EnGe2_LookAtPlayer(EnGe2* self, GlobalContext* globalCtx) {
    if ((ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) <= 0x4300) &&
        (self->actor.xzDistToPlayer < 200.0f)) {
        func_80038290(globalCtx, &self->actor, &self->headRot, &self->unk_2EE, self->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&self->headRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->headRot.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_2EE.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_2EE.y, 0, 6, 6200, 100);
    }
}

void EnGe2_SetActionAfterTalk(EnGe2* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {

        switch (self->actor.params & 0xFF) {
            case GE2_TYPE_PATROLLING:
                EnGe2_ChangeAction(self, GE2_ACTION_ABOUTTURN);
                break;
            case GE2_TYPE_STATIONARY:
                EnGe2_ChangeAction(self, GE2_ACTION_STAND);
                break;
            case GE2_TYPE_GERUDO_CARD_GIVER:
                self->actionFunc = EnGe2_WaitLookAtPlayer;
                break;
        }
        self->actor.update = EnGe2_UpdateFriendly;
        self->actor.flags &= ~0x10000;
    }
    EnGe2_TurnToFacePlayer(self, globalCtx);
}

void EnGe2_WaitLookAtPlayer(EnGe2* self, GlobalContext* globalCtx) {
    EnGe2_LookAtPlayer(self, globalCtx);
}

void EnGe2_WaitTillCardGiven(EnGe2* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = EnGe2_SetActionAfterTalk;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_GERUDO_CARD, 10000.0f, 50.0f);
    }
}

void EnGe2_GiveCard(EnGe2* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->actor.flags &= ~0x10000;
        self->actionFunc = EnGe2_WaitTillCardGiven;
        func_8002F434(&self->actor, globalCtx, GI_GERUDO_CARD, 10000.0f, 50.0f);
    }
}

void EnGe2_ForceTalk(EnGe2* self, GlobalContext* globalCtx) {

    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnGe2_GiveCard;
    } else {
        self->actor.textId = 0x6004;
        self->actor.flags |= 0x10000;
        func_8002F1C4(&self->actor, globalCtx, 300.0f, 300.0f, 0);
    }
    EnGe2_LookAtPlayer(self, globalCtx);
}

void EnGe2_SetupCapturePlayer(EnGe2* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE2_STATE_CAPTURING;
    self->actor.speedXZ = 0.0f;
    EnGe2_ChangeAction(self, GE2_ACTION_CAPTURETURN);
    func_8002DF54(globalCtx, &self->actor, 95);
    func_80078884(NA_SE_SY_FOUND);
    func_8010B680(globalCtx, 0x6000, &self->actor);
}

void EnGe2_MaintainColliderAndSetAnimState(EnGe2* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 40.0f, 25.0f, 40.0f, 5);

    if (!(self->stateFlags & GE2_STATE_ANIMCOMPLETE) && SkelAnime_Update(&self->skelAnime)) {
        self->stateFlags |= GE2_STATE_ANIMCOMPLETE;
    }
}

void EnGe2_MoveAndBlink(EnGe2* self, GlobalContext* globalCtx) {
    Actor_MoveForward(&self->actor);

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset(60, 60);
    }
    self->eyeIndex = self->blinkTimer;

    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}

// Update functions

void EnGe2_UpdateFriendly(Actor* thisx, GlobalContext* globalCtx) {
    EnGe2* self = THIS;

    EnGe2_MaintainColliderAndSetAnimState(self, globalCtx);
    self->actionFunc(self, globalCtx);

    if (func_8002F194(&self->actor, globalCtx)) {
        if ((self->actor.params & 0xFF) == GE2_TYPE_PATROLLING) {
            self->actor.speedXZ = 0.0f;
            EnGe2_ChangeAction(self, GE2_ACTION_WAITLOOKATPLAYER);
        }
        self->actionFunc = EnGe2_SetActionAfterTalk;
        self->actor.update = EnGe2_UpdateAfterTalk;
    } else {
        self->actor.textId = 0x6005;

        if (self->actor.xzDistToPlayer < 100.0f) {
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
        }
    }
    EnGe2_MoveAndBlink(self, globalCtx);
}

void EnGe2_UpdateAfterTalk(Actor* thisx, GlobalContext* globalCtx) {
    EnGe2* self = THIS;

    self->stateFlags |= GE2_STATE_TALKED;
    EnGe2_MaintainColliderAndSetAnimState(self, globalCtx);
    self->actionFunc(self, globalCtx);
    EnGe2_MoveAndBlink(self, globalCtx);
}

void EnGe2_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGe2* self = THIS;
    s32 paramsType;

    EnGe2_MaintainColliderAndSetAnimState(self, globalCtx);

    if ((self->stateFlags & GE2_STATE_KO) || (self->stateFlags & GE2_STATE_CAPTURING)) {
        self->actionFunc(self, globalCtx);
    } else if (self->collider.base.acFlags & 2) {
        if ((self->collider.info.acHitInfo != NULL) && (self->collider.info.acHitInfo->toucher.dmgFlags & 0x80)) {
            Actor_SetColorFilter(&self->actor, 0, 120, 0, 400);
            self->actor.update = EnGe2_UpdateStunned;
            return;
        }

        EnGe2_ChangeAction(self, GE2_ACTION_KNOCKEDOUT);
        self->timer = 100;
        self->stateFlags |= GE2_STATE_KO;
        self->actor.speedXZ = 0.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_VO_SK_CRASH);
    } else {
        self->actionFunc(self, globalCtx);

        if (Ge2_DetectPlayerInUpdate(globalCtx, self, &self->actor.focus.pos, self->actor.shape.rot.y,
                                     self->yDetectRange)) {
            // "Discovered!"
            osSyncPrintf(VT_FGCOL(GREEN) "発見!!!!!!!!!!!!\n" VT_RST);
            EnGe2_SetupCapturePlayer(self, globalCtx);
        }

        if (((self->actor.params & 0xFF) == GE2_TYPE_STATIONARY) && (self->actor.xzDistToPlayer < 100.0f)) {
            // "Discovered!"
            osSyncPrintf(VT_FGCOL(GREEN) "発見!!!!!!!!!!!!\n" VT_RST);
            EnGe2_SetupCapturePlayer(self, globalCtx);
        }
    }

    if (!(self->stateFlags & GE2_STATE_KO)) {
        paramsType = self->actor.params & 0xFF; // Not necessary, but looks a bit nicer
        if ((paramsType == GE2_TYPE_PATROLLING) || (paramsType == GE2_TYPE_STATIONARY)) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
    EnGe2_MoveAndBlink(self, globalCtx);

    if (EnGe2_CheckCarpentersFreed() && !(self->stateFlags & GE2_STATE_KO)) {
        self->actor.update = EnGe2_UpdateFriendly;
        self->actor.targetMode = 6;
    }
}

void EnGe2_UpdateStunned(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnGe2* self = THIS;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 40.0f, 25.0f, 40.0f, 5);

    if ((self->collider.base.acFlags & 2) &&
        ((self->collider.info.acHitInfo == NULL) || !(self->collider.info.acHitInfo->toucher.dmgFlags & 0x80))) {
        self->actor.colorFilterTimer = 0;
        EnGe2_ChangeAction(self, GE2_ACTION_KNOCKEDOUT);
        self->timer = 100;
        self->stateFlags |= GE2_STATE_KO;
        self->actor.speedXZ = 0.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_VO_SK_CRASH);
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

    if (EnGe2_CheckCarpentersFreed()) {
        self->actor.update = EnGe2_UpdateFriendly;
        self->actor.targetMode = 6;
        self->actor.colorFilterTimer = 0;
    } else if (self->actor.colorFilterTimer == 0) {
        self->actor.update = EnGe2_Update;
    }
}

s32 EnGe2_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnGe2* self = THIS;

    if (limbIndex == 3) {
        rot->x += self->headRot.y;
        rot->z += self->headRot.x;
    }
    return 0;
}

void EnGe2_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f D_80A343B0 = { 600.0f, 700.0f, 0.0f };
    EnGe2* self = THIS;

    if (limbIndex == 6) {
        Matrix_MultVec3f(&D_80A343B0, &self->actor.focus.pos);
    }
}

void EnGe2_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { gGerudoPurpleEyeOpenTex, gGerudoPurpleEyeHalfTex, gGerudoPurpleEyeClosedTex };
    s32 pad;
    EnGe2* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge2.c", 1274);

    func_800943C8(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeIndex]));
    func_8002EBCC(&self->actor, globalCtx, 0);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnGe2_OverrideLimbDraw, EnGe2_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge2.c", 1291);
}
