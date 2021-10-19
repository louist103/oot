/*
 * File: z_eff_ss_k_fire.c
 * Overlay: ovl_Effect_Ss_K_Fire
 * Description:
 */

#include "z_eff_ss_k_fire.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rAlpha regs[0]
#define rScroll regs[2]
#define rType regs[3]
#define rYScale regs[4]
#define rXZScale regs[5]
#define rScaleMax regs[6]

u32 EffectSsKFire_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsKFire_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsKFire_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_K_Fire_InitVars = {
    EFFECT_SS_K_FIRE,
    EffectSsKFire_Init,
};

u32 EffectSsKFire_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsKFireInitParams* initParams = (EffectSsKFireInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->life = 100;
    self->rScaleMax = initParams->scaleMax;
    self->rAlpha = 255;
    self->rScroll = (s16)Rand_ZeroFloat(5.0f) - 0x19;
    self->rType = initParams->type;
    self->draw = EffectSsKFire_Draw;
    self->update = EffectSsKFire_Update;

    return 1;
}

void EffectSsKFire_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad;
    f32 xzScale;
    f32 yScale;

    xzScale = self->rXZScale / 10000.0f;
    yScale = self->rYScale / 10000.0f;

    OPEN_DISPS(gfxCtx, "../z_eff_k_fire.c", 152);

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_Scale(xzScale, yScale, xzScale, MTXMODE_APPLY);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0,
                                globalCtx->state.frames * self->rScroll, 0x20, 0x80));

    if (self->rType >= 100) {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 0, self->rAlpha);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 10, 0, 0);
    } else {
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 255, self->rAlpha);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, 0);
    }

    if (1) {}

    gDPPipeSync(POLY_XLU_DISP++);
    func_800D1FD4(&globalCtx->mf_11DA0);

    if ((index & 1) != 0) {
        Matrix_RotateY(M_PI, MTXMODE_APPLY);
    }

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_eff_k_fire.c", 215),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);

    CLOSE_DISPS(gfxCtx, "../z_eff_k_fire.c", 220);
}

void EffectSsKFire_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    if (self->rXZScale < self->rScaleMax) {
        self->rXZScale += 4;
        self->rYScale += 4;

        if (self->rXZScale > self->rScaleMax) {
            self->rXZScale = self->rScaleMax;

            if (self->rType != 3) {
                self->rYScale = self->rScaleMax;
            }
        }
    } else {
        if (self->rAlpha > 0) {
            self->rAlpha -= 10;
            if (self->rAlpha <= 0) {
                self->rAlpha = 0;
                self->life = 0;
            }
        }
    }

    if (self->rType == 3) {
        self->rYScale++;
    }
}
