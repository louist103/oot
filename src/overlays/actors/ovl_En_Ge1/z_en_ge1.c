/*
 * File: z_en_ge1.c
 * Overlay: ovl_En_Ge1
 * Description: White-clothed Gerudo
 */

#include "z_en_ge1.h"
#include "vt.h"
#include "objects/object_ge1/object_ge1.h"

#define FLAGS 0x00000009

#define THIS ((EnGe1*)thisx)

#define GE1_STATE_TALKING (1 << 0)
#define GE1_STATE_GIVE_QUIVER (1 << 1)
#define GE1_STATE_IDLE_ANIM (1 << 2)
#define GE1_STATE_STOP_FIDGET (1 << 3)

typedef enum {
    /* 00 */ GE1_HAIR_BOB,
    /* 01 */ GE1_HAIR_STRAIGHT,
    /* 02 */ GE1_HAIR_SPIKY
} EnGe1Hairstyle;

void EnGe1_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGe1_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGe1_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGe1_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 EnGe1_CheckCarpentersFreed(void);
void EnGe1_WatchForPlayerFrontOnly(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_SetNormalText(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_WatchForAndSensePlayer(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_GetReaction_ValleyFloor(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_CheckForCard_GTGGuard(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_CheckGate_GateOp(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_GetReaction_GateGuard(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_TalkAfterGame_Archery(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_Wait_Archery(EnGe1* self, GlobalContext* globalCtx);
void EnGe1_CueUpAnimation(EnGe1* self);
void EnGe1_StopFidget(EnGe1* self);

const ActorInit En_Ge1_InitVars = {
    ACTOR_EN_GE1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GE1,
    sizeof(EnGe1),
    (ActorFunc)EnGe1_Init,
    (ActorFunc)EnGe1_Destroy,
    (ActorFunc)EnGe1_Update,
    (ActorFunc)EnGe1_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000702, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 40, 0, { 0, 0, 0 } },
};

static Gfx* sHairstyleDLists[] = {
    gGerudoWhiteHairstyleBobDL,
    gGerudoWhiteHairstyleStraightFringeDL,
    gGerudoWhiteHairstyleSpikyDL,
};

static Vec3f D_80A327A8 = { 600.0f, 700.0f, 0.0f };

static void* sEyeTextures[] = {
    gGerudoWhiteEyeOpenTex,
    gGerudoWhiteEyeHalfTex,
    gGerudoWhiteEyeClosedTex,
};

void EnGe1_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnGe1* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGerudoWhiteSkel, &gGerudoWhiteIdleAnim, self->jointTable,
                       self->morphTable, GE1_LIMB_MAX);
    Animation_PlayOnce(&self->skelAnime, &gGerudoWhiteIdleAnim);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->animation = &gGerudoWhiteIdleAnim;
    self->animFunc = EnGe1_CueUpAnimation;
    self->actor.targetMode = 6;
    Actor_SetScale(&self->actor, 0.01f);

    // In Gerudo Valley
    self->actor.uncullZoneForward = ((globalCtx->sceneNum == SCENE_SPOT09) ? 1000.0f : 1200.0f);

    switch (self->actor.params & 0xFF) {

        case GE1_TYPE_GATE_GUARD:
            self->hairstyle = GE1_HAIR_SPIKY;
            self->actionFunc = EnGe1_GetReaction_GateGuard;
            break;

        case GE1_TYPE_GATE_OPERATOR:
            self->hairstyle = GE1_HAIR_STRAIGHT;

            if (EnGe1_CheckCarpentersFreed()) {
                self->actionFunc = EnGe1_CheckGate_GateOp;
            } else {
                self->actionFunc = EnGe1_WatchForPlayerFrontOnly;
            }
            break;

        case GE1_TYPE_NORMAL:
            self->hairstyle = GE1_HAIR_STRAIGHT;

            if (EnGe1_CheckCarpentersFreed()) {
                self->actionFunc = EnGe1_SetNormalText;
            } else {
                self->actionFunc = EnGe1_WatchForAndSensePlayer;
            }
            break;

        case GE1_TYPE_VALLEY_FLOOR:
            if (LINK_IS_ADULT) {
                // "Valley floor Gerudo withdrawal"
                osSyncPrintf(VT_FGCOL(CYAN) "谷底 ゲルド 撤退 \n" VT_RST);
                Actor_Kill(&self->actor);
                return;
            }
            self->hairstyle = GE1_HAIR_BOB;
            self->actionFunc = EnGe1_GetReaction_ValleyFloor;
            break;

        case GE1_TYPE_HORSEBACK_ARCHERY:
            if (INV_CONTENT(SLOT_BOW) == ITEM_NONE) {
                Actor_Kill(&self->actor);
                return;
            }
            self->actor.targetMode = 3;
            self->hairstyle = GE1_HAIR_BOB;
            // "Horseback archery Gerudo EVENT_INF(0) ="
            osSyncPrintf(VT_FGCOL(CYAN) "やぶさめ ゲルド EVENT_INF(0) = %x\n" VT_RST, gSaveContext.eventInf[0]);

            if (gSaveContext.eventInf[0] & 0x100) {
                self->actionFunc = EnGe1_TalkAfterGame_Archery;
            } else if (EnGe1_CheckCarpentersFreed()) {
                self->actionFunc = EnGe1_Wait_Archery;
            } else {
                self->actionFunc = EnGe1_WatchForPlayerFrontOnly;
            }
            break;

        case GE1_TYPE_TRAINING_GROUNDS_GUARD:
            self->hairstyle = GE1_HAIR_STRAIGHT;

            if (EnGe1_CheckCarpentersFreed()) {
                self->actionFunc = EnGe1_CheckForCard_GTGGuard;
            } else {
                self->actionFunc = EnGe1_WatchForPlayerFrontOnly;
            }
            break;
    }

    self->stateFlags = 0;
}

void EnGe1_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGe1* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 EnGe1_SetTalkAction(EnGe1* self, GlobalContext* globalCtx, u16 textId, f32 arg3, EnGe1ActionFunc actionFunc) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = actionFunc;
        self->animFunc = EnGe1_StopFidget;
        self->stateFlags &= ~GE1_STATE_IDLE_ANIM;
        self->animation = &gGerudoWhiteIdleAnim;
        Animation_Change(&self->skelAnime, &gGerudoWhiteIdleAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gGerudoWhiteIdleAnim), ANIMMODE_ONCE, -8.0f);
        return true;
    }

    self->actor.textId = textId;

    if (self->actor.xzDistToPlayer < arg3) {
        func_8002F2CC(&self->actor, globalCtx, arg3);
    }

    return false;
}

void EnGe1_SetAnimationIdle(EnGe1* self) {
    Animation_Change(&self->skelAnime, &gGerudoWhiteIdleAnim, -1.0f, Animation_GetLastFrame(&gGerudoWhiteIdleAnim),
                     0.0f, ANIMMODE_ONCE, 8.0f);
    self->animation = &gGerudoWhiteIdleAnim;
    self->animFunc = EnGe1_CueUpAnimation;
}

s32 EnGe1_CheckCarpentersFreed(void) {
    u16 carpenterFlags = gSaveContext.eventChkInf[9];

    if (!((carpenterFlags & 1) && (carpenterFlags & 2) && (carpenterFlags & 4) && (carpenterFlags & 8))) {
        return 0;
    }
    return 1;
}

/**
 * Sends player to different places depending on if has hookshot, and if self is the first time captured
 */
void EnGe1_KickPlayer(EnGe1* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (self->cutsceneTimer > 0) {
        self->cutsceneTimer--;
    } else {
        func_8006D074(globalCtx);

        if ((INV_CONTENT(ITEM_HOOKSHOT) == ITEM_NONE) || (INV_CONTENT(ITEM_LONGSHOT) == ITEM_NONE)) {
            globalCtx->nextEntranceIndex = 0x1A5;
        } else if (gSaveContext.eventChkInf[12] & 0x80) { // Caught previously
            globalCtx->nextEntranceIndex = 0x5F8;
        } else {
            globalCtx->nextEntranceIndex = 0x3B4;
        }

        globalCtx->fadeTransition = 0x26;
        globalCtx->sceneLoadFlag = 0x14;
    }
}

void EnGe1_SpotPlayer(EnGe1* self, GlobalContext* globalCtx) {
    self->cutsceneTimer = 30;
    self->actionFunc = EnGe1_KickPlayer;
    func_8002DF54(globalCtx, &self->actor, 0x5F);
    func_80078884(NA_SE_SY_FOUND);
    func_8010B680(globalCtx, 0x6000, &self->actor);
}

void EnGe1_WatchForPlayerFrontOnly(EnGe1* self, GlobalContext* globalCtx) {
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if ((ABS(angleDiff) <= 0x4300) && (self->actor.xzDistToPlayer < 100.0f)) {
        EnGe1_SpotPlayer(self, globalCtx);
    }

    if (self->collider.base.acFlags & AC_HIT) {
        EnGe1_SpotPlayer(self, globalCtx);
    }

    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnGe1_ChooseActionFromTextId(EnGe1* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (func_8002F334(&self->actor, globalCtx)) {
        switch (self->actor.textId) {
            case 0x6001:
                self->actionFunc = EnGe1_SetNormalText;
                break;

            case 0x601A:
            case 0x6019:
                self->actionFunc = EnGe1_GetReaction_ValleyFloor;
                break;

            case 0x6018:
                self->actionFunc = EnGe1_CheckGate_GateOp;
                break;

            default:
                self->actionFunc = EnGe1_GetReaction_ValleyFloor;
                break;
        }
    }
}

void EnGe1_SetNormalText(EnGe1* self, GlobalContext* globalCtx) {
    EnGe1_SetTalkAction(self, globalCtx, 0x6001, 100.0f, EnGe1_ChooseActionFromTextId);
}

void EnGe1_WatchForAndSensePlayer(EnGe1* self, GlobalContext* globalCtx) {
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if ((self->actor.xzDistToPlayer < 50.0f) || ((ABS(angleDiff) <= 0x4300) && (self->actor.xzDistToPlayer < 400.0f))) {
        EnGe1_SpotPlayer(self, globalCtx);
    }

    if (self->collider.base.acFlags & AC_HIT) {
        EnGe1_SpotPlayer(self, globalCtx);
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnGe1_GetReaction_ValleyFloor(EnGe1* self, GlobalContext* globalCtx) {
    u16 reactionText = Text_GetFaceReaction(globalCtx, 0x22);

    if (reactionText == 0) {
        reactionText = 0x6019;
    }

    EnGe1_SetTalkAction(self, globalCtx, reactionText, 100.0f, EnGe1_ChooseActionFromTextId);
}

// Gerudo Training Ground Guard functions

void EnGe1_WaitTillOpened_GTGGuard(EnGe1* self, GlobalContext* globalCtx) {
    if (self->cutsceneTimer > 0) {
        self->cutsceneTimer--;
    } else {
        EnGe1_SetAnimationIdle(self);
        self->actionFunc = EnGe1_SetNormalText;
    }

    self->stateFlags |= GE1_STATE_STOP_FIDGET;
}

void EnGe1_Open_GTGGuard(EnGe1* self, GlobalContext* globalCtx) {
    if (self->stateFlags & GE1_STATE_IDLE_ANIM) {
        self->actionFunc = EnGe1_WaitTillOpened_GTGGuard;
        Flags_SetSwitch(globalCtx, (self->actor.params >> 8) & 0x3F);
        self->cutsceneTimer = 50;
        func_80106CCC(globalCtx);
    } else if ((self->skelAnime.curFrame == 15.0f) || (self->skelAnime.curFrame == 19.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_IT_HAND_CLAP);
    }
}

void EnGe1_SetupOpen_GTGGuard(EnGe1* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        self->actionFunc = EnGe1_Open_GTGGuard;
        Animation_Change(&self->skelAnime, &gGerudoWhiteClapAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gGerudoWhiteClapAnim), ANIMMODE_ONCE, -3.0f);
        self->animation = &gGerudoWhiteClapAnim;
        self->animFunc = EnGe1_StopFidget;
        self->stateFlags &= ~GE1_STATE_IDLE_ANIM;
    }
}

void EnGe1_RefuseEntryTooPoor_GTGGuard(EnGe1* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        self->actionFunc = EnGe1_CheckForCard_GTGGuard;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_OfferOpen_GTGGuard(EnGe1* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE1_STATE_TALKING;
    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);

        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                if (gSaveContext.rupees < 10) {
                    func_8010B720(globalCtx, 0x6016);
                    self->actionFunc = EnGe1_RefuseEntryTooPoor_GTGGuard;
                } else {
                    Rupees_ChangeBy(-10);
                    func_8010B720(globalCtx, 0x6015);
                    self->actionFunc = EnGe1_SetupOpen_GTGGuard;
                }
                break;
            case 1:
                self->actionFunc = EnGe1_CheckForCard_GTGGuard;
                EnGe1_SetAnimationIdle(self);
                break;
        }
    }
}

void EnGe1_RefuseOpenNoCard_GTGGuard(EnGe1* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE1_STATE_TALKING;
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actionFunc = EnGe1_CheckForCard_GTGGuard;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_CheckForCard_GTGGuard(EnGe1* self, GlobalContext* globalCtx) {
    if (CHECK_QUEST_ITEM(QUEST_GERUDO_CARD)) {
        EnGe1_SetTalkAction(self, globalCtx, 0x6014, 100.0f, EnGe1_OfferOpen_GTGGuard);
    } else {
        //! @bug This outcome is inaccessible in normal gameplay since self function it is unreachable without
        //! obtaining the card in the first place.
        EnGe1_SetTalkAction(self, globalCtx, 0x6013, 100.0f, EnGe1_RefuseOpenNoCard_GTGGuard);
    }
}

// Gate Operator functions

void EnGe1_WaitGateOpen_GateOp(EnGe1* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE1_STATE_TALKING;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);
        self->actionFunc = EnGe1_CheckGate_GateOp;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_WaitUntilGateOpened_GateOp(EnGe1* self, GlobalContext* globalCtx) {
    if (self->cutsceneTimer > 0) {
        self->cutsceneTimer--;
    } else {
        EnGe1_SetAnimationIdle(self);
        self->actionFunc = EnGe1_CheckGate_GateOp;
    }
    self->stateFlags |= GE1_STATE_STOP_FIDGET;
}

void EnGe1_OpenGate_GateOp(EnGe1* self, GlobalContext* globalCtx) {
    if (self->stateFlags & GE1_STATE_IDLE_ANIM) {
        self->actionFunc = EnGe1_WaitUntilGateOpened_GateOp;
        Flags_SetSwitch(globalCtx, (self->actor.params >> 8) & 0x3F);
        self->cutsceneTimer = 50;
        func_80106CCC(globalCtx);
    } else if ((self->skelAnime.curFrame == 15.0f) || (self->skelAnime.curFrame == 19.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_IT_HAND_CLAP);
    }
}

void EnGe1_SetupOpenGate_GateOp(EnGe1* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE1_STATE_TALKING;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        self->actionFunc = EnGe1_OpenGate_GateOp;
        Animation_Change(&self->skelAnime, &gGerudoWhiteClapAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gGerudoWhiteClapAnim), ANIMMODE_ONCE, -3.0f);
        self->animation = &gGerudoWhiteClapAnim;
        self->animFunc = EnGe1_StopFidget;
        self->stateFlags &= ~GE1_STATE_IDLE_ANIM;
    }
}

void EnGe1_CheckGate_GateOp(EnGe1* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (self->actor.params >> 8) & 0x3F)) {
        EnGe1_SetTalkAction(self, globalCtx, 0x6018, 100.0f, EnGe1_WaitGateOpen_GateOp);
    } else {
        EnGe1_SetTalkAction(self, globalCtx, 0x6017, 100.0f, EnGe1_SetupOpenGate_GateOp);
    }
}

// Gate guard functions

void EnGe1_Talk_GateGuard(EnGe1* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE1_STATE_TALKING;

    if (func_8002F334(&self->actor, globalCtx)) {
        self->actionFunc = EnGe1_GetReaction_GateGuard;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_GetReaction_GateGuard(EnGe1* self, GlobalContext* globalCtx) {
    u16 reactionText;

    reactionText = Text_GetFaceReaction(globalCtx, 0x22);

    if (reactionText == 0) {
        reactionText = 0x6069;
    }

    if (EnGe1_SetTalkAction(self, globalCtx, reactionText, 100.0f, EnGe1_Talk_GateGuard)) {
        self->animFunc = EnGe1_CueUpAnimation;
        self->animation = &gGerudoWhiteDismissiveAnim;
        Animation_Change(&self->skelAnime, &gGerudoWhiteDismissiveAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gGerudoWhiteDismissiveAnim), ANIMMODE_ONCE, -8.0f);
    }
}

// Archery functions

void EnGe1_SetupWait_Archery(EnGe1* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actionFunc = EnGe1_Wait_Archery;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_WaitTillItemGiven_Archery(EnGe1* self, GlobalContext* globalCtx) {
    s32 getItemId;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actionFunc = EnGe1_SetupWait_Archery;
        if (self->stateFlags & GE1_STATE_GIVE_QUIVER) {
            gSaveContext.itemGetInf[0] |= 0x8000;
        } else {
            gSaveContext.infTable[25] |= 1;
        }
    } else {
        if (self->stateFlags & GE1_STATE_GIVE_QUIVER) {
            switch (CUR_UPG_VALUE(UPG_QUIVER)) {
                //! @bug Asschest. See next function for details
                case 1:
                    getItemId = GI_QUIVER_40;
                    break;
                case 2:
                    getItemId = GI_QUIVER_50;
                    break;
            }
        } else {
            getItemId = GI_HEART_PIECE;
        }
        func_8002F434(&self->actor, globalCtx, getItemId, 10000.0f, 50.0f);
    }
}

void EnGe1_BeginGiveItem_Archery(EnGe1* self, GlobalContext* globalCtx) {
    s32 getItemId;

    if (func_8002F334(&self->actor, globalCtx)) {
        self->actor.flags &= ~0x10000;
        self->actionFunc = EnGe1_WaitTillItemGiven_Archery;
    }

    if (self->stateFlags & GE1_STATE_GIVE_QUIVER) {
        switch (CUR_UPG_VALUE(UPG_QUIVER)) {
            //! @bug Asschest: the compiler inserts a default assigning *(sp+0x24) to getItemId, which is junk data left
            //! over from the previous function run in EnGe1_Update, namely EnGe1_CueUpAnimation. The top stack variable
            //! in that function is &self->skelAnime = thisx + 198, and depending on where self loads in memory, the
            //! getItemId changes.
            case 1:
                getItemId = GI_QUIVER_40;
                break;
            case 2:
                getItemId = GI_QUIVER_50;
                break;
        }
    } else {
        getItemId = GI_HEART_PIECE;
    }

    func_8002F434(&self->actor, globalCtx, getItemId, 10000.0f, 50.0f);
}

void EnGe1_TalkWinPrize_Archery(EnGe1* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnGe1_BeginGiveItem_Archery;
        self->actor.flags &= ~0x10000;
    } else {
        func_8002F2CC(&self->actor, globalCtx, 200.0f);
    }
}

void EnGe1_TalkTooPoor_Archery(EnGe1* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);
        self->actionFunc = EnGe1_Wait_Archery;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_WaitDoNothing(EnGe1* self, GlobalContext* globalCtx) {
}

void EnGe1_BeginGame_Archery(EnGe1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* horse;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx)) {
        self->actor.flags &= ~0x10000;

        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                if (gSaveContext.rupees < 20) {
                    func_8010B720(globalCtx, 0x85);
                    self->actionFunc = EnGe1_TalkTooPoor_Archery;
                } else {
                    Rupees_ChangeBy(-20);
                    globalCtx->nextEntranceIndex = 0x129;
                    gSaveContext.nextCutsceneIndex = 0xFFF0;
                    globalCtx->fadeTransition = 0x26;
                    globalCtx->sceneLoadFlag = 0x14;
                    gSaveContext.eventInf[0] |= 0x100;
                    gSaveContext.eventChkInf[6] |= 0x100;

                    if (!(player->stateFlags1 & 0x800000)) {
                        func_8002DF54(globalCtx, &self->actor, 1);
                    } else {
                        horse = Actor_FindNearby(globalCtx, &player->actor, ACTOR_EN_HORSE, ACTORCAT_BG, 1200.0f);
                        player->actor.freezeTimer = 1200;

                        if (horse != NULL) {
                            horse->freezeTimer = 1200;
                        }
                    }

                    self->actionFunc = EnGe1_WaitDoNothing;
                }
                break;

            case 1:
                self->actionFunc = EnGe1_Wait_Archery;
                func_80106CCC(globalCtx);
                break;
        }
    }
}

void EnGe1_TalkOfferPlay_Archery(EnGe1* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0x6041);
        self->actionFunc = EnGe1_BeginGame_Archery;
    }
}

void EnGe1_TalkNoPrize_Archery(EnGe1* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnGe1_TalkOfferPlay_Archery;
    } else {
        func_8002F2CC(&self->actor, globalCtx, 300.0f);
    }
}

void EnGe1_TalkAfterGame_Archery(EnGe1* self, GlobalContext* globalCtx) {
    gSaveContext.eventInf[0] &= ~0x100;
    LOG_NUM("z_common_data.yabusame_total", gSaveContext.minigameScore, "../z_en_ge1.c", 1110);
    LOG_NUM("z_common_data.memory.information.room_inf[127][ 0 ]", gSaveContext.highScores[HS_HBA], "../z_en_ge1.c",
            1111);
    self->actor.flags |= 0x10000;

    if (gSaveContext.highScores[HS_HBA] < gSaveContext.minigameScore) {
        gSaveContext.highScores[HS_HBA] = gSaveContext.minigameScore;
    }

    if (gSaveContext.minigameScore < 1000) {
        self->actor.textId = 0x6045;
        self->actionFunc = EnGe1_TalkNoPrize_Archery;
    } else if (!(gSaveContext.infTable[25] & 1)) {
        self->actor.textId = 0x6046;
        self->actionFunc = EnGe1_TalkWinPrize_Archery;
        self->stateFlags &= ~GE1_STATE_GIVE_QUIVER;
    } else if (gSaveContext.minigameScore < 1500) {
        self->actor.textId = 0x6047;
        self->actionFunc = EnGe1_TalkNoPrize_Archery;
    } else if (gSaveContext.itemGetInf[0] & 0x8000) {
        self->actor.textId = 0x6047;
        self->actionFunc = EnGe1_TalkNoPrize_Archery;
    } else {
        self->actor.textId = 0x6044;
        self->actionFunc = EnGe1_TalkWinPrize_Archery;
        self->stateFlags |= GE1_STATE_GIVE_QUIVER;
    }
}

void EnGe1_TalkNoHorse_Archery(EnGe1* self, GlobalContext* globalCtx) {
    self->stateFlags |= GE1_STATE_TALKING;
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actionFunc = EnGe1_Wait_Archery;
        EnGe1_SetAnimationIdle(self);
    }
}

void EnGe1_Wait_Archery(EnGe1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    u16 textId;

    if (!(player->stateFlags1 & 0x800000)) {
        EnGe1_SetTalkAction(self, globalCtx, 0x603F, 100.0f, EnGe1_TalkNoHorse_Archery);
    } else {
        if (gSaveContext.eventChkInf[6] & 0x100) {
            if (gSaveContext.infTable[25] & 1) {
                textId = 0x6042;
            } else {
                textId = 0x6043;
            }
        } else {
            textId = 0x6040;
        }
        EnGe1_SetTalkAction(self, globalCtx, textId, 200.0f, EnGe1_TalkOfferPlay_Archery);
    }
}

// General functions

void EnGe1_TurnToFacePlayer(EnGe1* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(angleDiff) <= 0x4000) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 6, 4000, 100);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        func_80038290(globalCtx, &self->actor, &self->headRot, &self->unk_2A2, self->actor.focus.pos);
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

void EnGe1_LookAtPlayer(EnGe1* self, GlobalContext* globalCtx) {
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if ((ABS(angleDiff) <= 0x4300) && (self->actor.xzDistToPlayer < 100.0f)) {
        func_80038290(globalCtx, &self->actor, &self->headRot, &self->unk_2A2, self->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&self->headRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->headRot.y, 0, 6, 6200, 100);
    }
}

void EnGe1_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnGe1* self = THIS;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 40.0f, 25.0f, 40.0f, 5);
    self->animFunc(self);
    self->actionFunc(self, globalCtx);

    if (self->stateFlags & GE1_STATE_TALKING) {
        EnGe1_TurnToFacePlayer(self, globalCtx);
        self->stateFlags &= ~GE1_STATE_TALKING;
    } else {
        EnGe1_LookAtPlayer(self, globalCtx);
    }
    self->unk_2A2.x = self->unk_2A2.y = self->unk_2A2.z = 0;

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset(60, 60);
    }
    self->eyeIndex = self->blinkTimer;

    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}

// Animation functions

void EnGe1_CueUpAnimation(EnGe1* self) {
    if (SkelAnime_Update(&self->skelAnime)) {
        Animation_PlayOnce(&self->skelAnime, self->animation);
    }
}

void EnGe1_StopFidget(EnGe1* self) {
    if (!(self->stateFlags & GE1_STATE_IDLE_ANIM)) {
        if (SkelAnime_Update(&self->skelAnime)) {
            self->stateFlags |= GE1_STATE_IDLE_ANIM;
        }
        self->stateFlags |= GE1_STATE_STOP_FIDGET;
    }
}

s32 EnGe1_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    s32 pad;
    EnGe1* self = THIS;

    if (limbIndex == GE1_LIMB_HEAD) {
        rot->x += self->headRot.y;
        rot->z += self->headRot.x;
    }

    if (self->stateFlags & GE1_STATE_STOP_FIDGET) {
        self->stateFlags &= ~GE1_STATE_STOP_FIDGET;
        return 0;
    }

    // The purpose of the state flag GE1_STATE_STOP_FIDGET is to skip self code, which self actor has in lieu of an idle
    // animation.
    if ((limbIndex == GE1_LIMB_TORSO) || (limbIndex == GE1_LIMB_L_FOREARM) || (limbIndex == GE1_LIMB_R_FOREARM)) {
        rot->y += Math_SinS(globalCtx->state.frames * (limbIndex * 50 + 0x814)) * 200.0f;
        rot->z += Math_CosS(globalCtx->state.frames * (limbIndex * 50 + 0x940)) * 200.0f;
    }
    return 0;
}

void EnGe1_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnGe1* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge1.c", 1419);

    if (limbIndex == GE1_LIMB_HEAD) {
        gSPDisplayList(POLY_OPA_DISP++, sHairstyleDLists[self->hairstyle]);
        Matrix_MultVec3f(&D_80A327A8, &self->actor.focus.pos);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge1.c", 1427);
}

void EnGe1_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnGe1* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge1.c", 1442);

    func_800943C8(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeIndex]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnGe1_OverrideLimbDraw, EnGe1_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge1.c", 1459);
}
