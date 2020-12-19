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

void func_809C1E00(EnBird* this, s16 params);
void func_809C1E40(EnBird* this, GlobalContext* globalCtx);
void func_809C1D60(EnBird* this, GlobalContext* globalCtx);
void func_809C1CAC(EnBird* this, s16 params);

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

void EnBird_Init(Actor* thisx, GlobalContext* globalCtx) {
    thisx->posRot.pos.y -= thisx->groundY;
}

void EnBird_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnBird_Update(Actor* thisx, GlobalContext* globalCtx) {
}
static Gfx* gfx[] = {
    0x04039C00, 0x04039C00, 0x04039C00, 0x0403A2D0, 0x0403A2D0, 0x0403A630,
    0x06000210, 0x0403AB80, 0x0403A9B0, 0x0403C050, 0x0403C5B0, 0x0400D340,
};
extern Gfx* D_060001A0[];
void EnBird_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBird* this = THIS;

    s32 pad;
    u32 pad1;
    u32 spEC;
    f32 spE8;
    f32 spE4;
    f32 temp_f12;
    f32 temp_f0;

    spEC = globalCtx->state.frames * 10;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2173);

    temp_f0 = 1.0f - (2.0f - 1.0f) / 1.7f;

    func_80093D84(globalCtx->state.gfxCtx);
    if (thisx->params == 0) {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 255);
    } else {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 255);
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
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 255);
    } else {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 255);
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
