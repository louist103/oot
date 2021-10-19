/*
 * File: z_en_skjneedle.c
 * Overlay: ovl_En_Skjneedle
 * Description: Skullkid Needle Attack
 */

#include "z_en_skjneedle.h"
#include "objects/object_skj/object_skj.h"

#define FLAGS 0x00000205

#define THIS ((EnSkjneedle*)thisx)

void EnSkjneedle_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSkjneedle_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSkjneedle_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSkjneedle_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 EnSkjNeedle_CollisionCheck(EnSkjneedle* self);

const ActorInit En_Skjneedle_InitVars = {
    ACTOR_EN_SKJNEEDLE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SKJ,
    sizeof(EnSkjneedle),
    (ActorFunc)EnSkjneedle_Init,
    (ActorFunc)EnSkjneedle_Destroy,
    (ActorFunc)EnSkjneedle_Update,
    (ActorFunc)EnSkjneedle_Draw,
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_HIT1,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 10, 4, -2, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

void EnSkjneedle_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSkjneedle* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    ActorShape_Init(&self->actor.shape, 0, ActorShadow_DrawCircle, 20.0f);
    thisx->flags &= ~0x1;
    Actor_SetScale(&self->actor, 0.01f);
}

void EnSkjneedle_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnSkjneedle* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 EnSkjNeedle_CollisionCheck(EnSkjneedle* self) {
    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        return 1;
    }
    return 0;
}

void EnSkjneedle_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnSkjneedle* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    self->unusedTimer1++;
    if (self->killTimer != 0) {
        self->killTimer--;
    }
    if (EnSkjNeedle_CollisionCheck(self) || self->killTimer == 0) {
        Actor_Kill(&self->actor);
    } else {
        Actor_SetScale(&self->actor, 0.01f);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 20.0f, 7);
    }
}

void EnSkjneedle_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_skj_needle.c", 200);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_skj_needle.c", 205),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gSKJNeedleDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_skj_needle.c", 210);
}
