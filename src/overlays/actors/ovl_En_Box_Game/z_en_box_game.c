/*
 * File: z_en_bird.c
 * Overlay: ovl_En_Bird
 * Description: An unused brown bird
 */

#include "z_en_box_game.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000030

#define THIS ((EnBoxGame*)thisx)

void EnBoxGame_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBoxGame_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBoxGame_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBoxGame_Draw(Actor* thisx, GlobalContext* globalCtx);

typedef enum BoxType {
    BOX_MANAGER,
    BOX_PHYSICAL,
    BOX_MAGIC,
    BOX_BIG,
} BoxType;

const ActorInit En_Box_Game_InitVars = {
    ACTOR_EN_BOX_GAME,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBoxGame),
    (ActorFunc)EnBoxGame_Init,
    (ActorFunc)EnBoxGame_Destroy,
    (ActorFunc)EnBoxGame_Update,
    (ActorFunc)EnBoxGame_Draw,
};

void EnBoxGame_Init(Actor* thisx, GlobalContext* globalCtx) {
    CollisionHeader* colHeader;
    EnBoxGame* this = THIS;

    switch (this->dyna.actor.params) {
        case BOX_MANAGER:
            this->dyna.actor.draw = NULL;
            this->dyna.actor.flags |= 0x30; // Never stop drawing, Never stop updating.
            break;
        case BOX_BIG:
            Actor_SetScale(&this->dyna.actor, this->dyna.actor.scale.y * 1.5f); // Same scale for all 3 is assumed
        case BOX_PHYSICAL:
        case BOX_MAGIC:
            // DynaPolyActor_Init(&this->dyna, DPM_UNK);
            // CollisionHeader_GetVirtual(&gBoxGameBoxCol_collisionHeader, &colHeader);
            // this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);
            this->dyna.actor.gravity = -2.0f;
            break;
    }
}

void EnBoxGame_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBoxGame* this = THIS;

    if (this->dyna.actor.params != 0x0000) {
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
    }
}

void EnBoxGame_Update(Actor* thisx, GlobalContext* globalCtx) {
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(globalCtx, thisx, 0.0f, 0.0f, 0.0f, 0x1C);
}

void EnBoxGame_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, __FILE__, __LINE__),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP, 0x08, SEGMENTED_TO_VIRTUAL(gBoxGamePlaceHolderBox));
    gDPSetPrimColor(POLY_OPA_DISP, 0, 0, 0, 0, 255, 255);
    gDPSetEnvColor(POLY_OPA_DISP, 0, 0, 255, 255);
    gSPDisplayList(POLY_OPA_DISP, gBoxGamePlaceHolderBox);

    // Gfx_DrawDListOpa(globalCtx, gBoxGamePlaceHolderBox);
    CLOSE_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);
}
