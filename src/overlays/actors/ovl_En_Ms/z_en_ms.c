/*
 * File: z_en_ms.c
 * Overlay: ovl_En_Ms
 * Description: Magic Bean Salesman
 */

#include "z_en_ms.h"
#include "objects/object_ms/object_ms.h"

#define FLAGS 0x00000009

#define THIS ((EnMs*)thisx)

void EnMs_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMs_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMs_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMs_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnMs_SetOfferText(EnMs* self, GlobalContext* globalCtx);
void EnMs_Wait(EnMs* self, GlobalContext* globalCtx);
void EnMs_Talk(EnMs* self, GlobalContext* globalCtx);
void EnMs_Sell(EnMs* self, GlobalContext* globalCtx);
void EnMs_TalkAfterPurchase(EnMs* self, GlobalContext* globalCtx);

const ActorInit En_Ms_InitVars = {
    ACTOR_EN_MS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MS,
    sizeof(EnMs),
    (ActorFunc)EnMs_Init,
    (ActorFunc)EnMs_Destroy,
    (ActorFunc)EnMs_Update,
    (ActorFunc)EnMs_Draw,
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        COLSHAPE_CYLINDER,
    },
    { 0x00, { 0x00000000, 0x00, 0x00 }, { 0xFFCFFFFF, 0x00, 0x00 }, 0x00, 0x01, 0x01 },
    { 22, 37, 0, { 0 } },
};

static s16 sPrices[] = {
    10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
};

static u16 sOfferTextIDs[] = {
    0x405E, 0x405F, 0x4060, 0x4061, 0x4062, 0x4063, 0x4064, 0x4065, 0x4066, 0x4067,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 500, ICHAIN_STOP),
};

void EnMs_SetOfferText(EnMs* self, GlobalContext* globalCtx) {
    self->actor.textId = Text_GetFaceReaction(globalCtx, 0x1B);
    if (self->actor.textId == 0) {
        if (BEANS_BOUGHT >= 10) {
            self->actor.textId = 0x406B;
        } else {
            self->actor.textId = sOfferTextIDs[BEANS_BOUGHT];
        }
    }
}

void EnMs_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnMs* self = THIS;
    s32 pad;

    if (LINK_AGE_IN_YEARS != YEARS_CHILD) {
        Actor_Kill(&self->actor);
        return;
    }
    Actor_ProcessInitChain(&self->actor, sInitChain);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gBeanSalesmanSkel, &gBeanSalesmanEatingAnim, self->jointTable,
                       self->morphTable, 9);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
    Actor_SetScale(&self->actor, 0.015f);

    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = -1.0f;

    EnMs_SetOfferText(self, globalCtx);

    self->actionFunc = EnMs_Wait;
}

void EnMs_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnMs* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnMs_Wait(EnMs* self, GlobalContext* globalCtx) {
    s16 yawDiff;

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    EnMs_SetOfferText(self, globalCtx);

    if (func_8002F194(&self->actor, globalCtx)) { // if talk is initiated
        self->actionFunc = EnMs_Talk;
    } else if ((self->actor.xzDistToPlayer < 90.0f) && (ABS(yawDiff) < 0x2000)) { // talk range
        func_8002F2CC(&self->actor, globalCtx, 90.0f);
    }
}

void EnMs_Talk(EnMs* self, GlobalContext* globalCtx) {
    u8 dialogState;

    dialogState = func_8010BDBC(&globalCtx->msgCtx);
    if (dialogState != 4) {
        if ((dialogState == 6) && (func_80106BC8(globalCtx) != 0)) { // advanced final textbox
            self->actionFunc = EnMs_Wait;
        }
    } else if (func_80106BC8(globalCtx) != 0) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // yes
                if (gSaveContext.rupees < sPrices[BEANS_BOUGHT]) {
                    func_8010B720(globalCtx, 0x4069); // not enough rupees text
                    return;
                }
                func_8002F434(&self->actor, globalCtx, GI_BEAN, 90.0f, 10.0f);
                self->actionFunc = EnMs_Sell;
                return;
            case 1: // no
                func_8010B720(globalCtx, 0x4068);
            default:
                return;
        }
    }
}

void EnMs_Sell(EnMs* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        Rupees_ChangeBy(-sPrices[BEANS_BOUGHT]);
        self->actor.parent = NULL;
        self->actionFunc = EnMs_TalkAfterPurchase;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_BEAN, 90.0f, 10.0f);
    }
}

void EnMs_TalkAfterPurchase(EnMs* self, GlobalContext* globalCtx) {
    // if dialog state is 6 and player responded to textbox
    if ((func_8010BDBC(&globalCtx->msgCtx)) == 6 && (func_80106BC8(globalCtx) != 0)) {
        func_8010B720(globalCtx, 0x406C);
        self->actionFunc = EnMs_Talk;
    }
}

void EnMs_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnMs* self = THIS;
    s32 pad;

    self->activeTimer += 1;
    Actor_SetFocus(&self->actor, 20.0f);
    self->actor.targetArrowOffset = 500.0f;
    Actor_SetScale(&self->actor, 0.015f);
    SkelAnime_Update(&self->skelAnime);
    self->actionFunc(self, globalCtx);

    if (gSaveContext.entranceIndex == 0x157 && gSaveContext.sceneSetupIndex == 8) { // ride carpet if in credits
        Actor_MoveForward(&self->actor);
        osSyncPrintf("OOOHHHHHH %f\n", self->actor.velocity.y);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    }
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnMs_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnMs* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);
}
