/*
 * File: z_en_bird.c
 * Overlay: ovl_En_Bird
 * Description: An unused brown bird
 */

#include "z_en_bird.h"

#define FLAGS 0x00000030

#define THIS ((EnBird*)thisx)

void EnBird_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Bird_InitVars = {
    ACTOR_EN_BIRD,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_WARP1,
    sizeof(EnBird),
    (ActorFunc)EnBird_Init,
    (ActorFunc)EnBird_Destroy,
    (ActorFunc)EnBird_Update,
    (ActorFunc)EnBird_Draw,
};
static GfxPrint* printer;
void EnBird_Init(Actor* thisx, GlobalContext* globalCtx) {
    printer = DebugArena_Malloc(sizeof(GfxPrint));
    thisx->posRot.pos.y -= thisx->groundY;
}

void EnBird_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    DebugArena_Free(printer);
}

void EnBird_Update(Actor* thisx, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    thisx->child = Portal_Find(&globalCtx->actorCtx, ACTOR_EN_BIRD, ACTORTYPE_PROP, thisx->params ^ 1);
    if ((thisx->child != NULL) && (thisx->xzDistFromLink < 40.0f)) {
        player->actor.posRot.pos.x = thisx->child->posRot.pos.x + 80.0f;
        player->actor.posRot.pos.y = thisx->child->posRot.pos.y;
        player->actor.posRot.pos.z = thisx->child->posRot.pos.z + 80.0f;
        player->actor.velocity.y = 6.0f;
    }
}

/*void printInfo(EnBird* this, GlobalContext* globalCtx) {

    Player* player = PLAYER;

    GfxPrint_Init(printer);
    GfxPrint_Open(printer, globalCtx->state.gfxCtx->polyOpa.p);
    GfxPrint_SetColor(printer, 255, 255, 255, 255);
    if (this->actor.params == 0) {
        GfxPrint_SetPos(printer, 0, 6);
        GfxPrint_Printf(printer, "Portal param 0 pos x %.0f y %.0f z %.0f", this->actor.posRot.pos.x,
                        this->actor.posRot.pos.y, this->actor.posRot.pos.z);
        GfxPrint_SetPos(printer, 0, 7);
        GfxPrint_Printf(printer, "Portal param 0 dist to player %0.f", this->actor.xzDistFromLink);
        if (this->actor.child != NULL) {
            GfxPrint_SetPos(printer, 0, 8);
            GfxPrint_Printf(printer, "Portal 0's child pos x %.0f y %.0f z %.0f", this->actor.child->posRot.pos.x,
                            this->actor.child->posRot.pos.y, this->actor.child->posRot.pos.z);
        }
    } else {
        GfxPrint_SetPos(printer, 0, 9);
        GfxPrint_Printf(printer, "Portal param 1 pos x %.0f y %.0f z %.0f", this->actor.posRot.pos.x,
                        this->actor.posRot.pos.y, this->actor.posRot.pos.z);
        GfxPrint_SetPos(printer, 0, 10);
        GfxPrint_Printf(printer, "Portal param 1 dist to player %0.f", this->actor.xzDistFromLink);
        if (this->actor.child != NULL) {
            GfxPrint_SetPos(printer, 0, 11);
            GfxPrint_Printf(printer, "Portal 1's child pos x %.0f y %.0f z %.0f", this->actor.child->posRot.pos.x,
                            this->actor.child->posRot.pos.y, this->actor.child->posRot.pos.z);
        }
    }
    globalCtx->state.gfxCtx->polyOpa.p = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);
}*/
extern Gfx* D_060001A0[];
void EnBird_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBird* this = THIS;

    s32 pad;
    u32 spEC;

    spEC = globalCtx->state.frames * 10;
    //printInfo(this, globalCtx);
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2173);

    func_80093D84(globalCtx->state.gfxCtx);
    if (thisx->params == 0) {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 175);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 175);
    } else {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 175);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 175);
    }
    gDPSetColorDither(POLY_XLU_DISP++, G_CD_DISABLE);
    gDPSetColorDither(POLY_XLU_DISP++, G_AD_NOTPATTERN | G_CD_MAGICSQ);

    Matrix_Translate(this->actor.posRot.pos.x, this->actor.posRot.pos.y + 1.0f, this->actor.posRot.pos.z, MTXMODE_NEW);
    gSPSegment(POLY_XLU_DISP++, 0x0A, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2247));
    Matrix_Push();

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, spEC & 0xFF, -((s16)(pad + pad) & 511), 0x100, 0x100, 1,
                                spEC & 0xFF, -((s16)(pad + pad) & 511), 0x100, 0x100));

    Matrix_Translate(0.0f, 230.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(2.0f, 1.0f, 2.0f, MTXMODE_APPLY);
    gSPSegment(POLY_XLU_DISP++, 0x09, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2267));
    gSPDisplayList(POLY_XLU_DISP++, D_060001A0);
    Matrix_Pull();

    if (thisx->params == 0) {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 175);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 175);
    } else {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 175);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 175);
    }

    spEC *= 2;

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, spEC & 0xFF, -((s16)pad & 511), 0x100, 0x100, 1,
                                spEC & 0xFF, -((s16)pad & 511), 0x100, 0x100));

    Matrix_Translate(0.0f, 1 * 60.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(2.0f, 1.0f, 2.0f, MTXMODE_APPLY);
    gSPSegment(POLY_XLU_DISP++, 0x09, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2336));
    gSPDisplayList(POLY_XLU_DISP++, D_060001A0);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2340);
}
