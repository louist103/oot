/*
 * File: z_eff_ss_dead_dd.c
 * Overlay: ovl_Effect_Ss_Dead_Dd
 * Description:
 */

#include "z_eff_ss_dead_dd.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rScale regs[0]
#define rPrimColorR regs[2]
#define rPrimColorG regs[3]
#define rPrimColorB regs[4]
#define rAlpha regs[5]
#define rEnvColorR regs[6]
#define rEnvColorG regs[7]
#define rEnvColorB regs[8]
#define rScaleStep regs[9]
#define rAlphaStep regs[10]
#define rAlphaMode regs[11] // if mode is 0 alpha decreases over time, otherwise it increases

u32 EffectSsDeadDd_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsDeadDd_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsDeadDd_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Dead_Dd_InitVars = {
    EFFECT_SS_DEAD_DD,
    EffectSsDeadDd_Init,
};

u32 EffectSsDeadDd_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    s32 i;
    EffectSsDeadDdInitParams* initParams = (EffectSsDeadDdInitParams*)initParamsx;

    if (initParams->type == 0) {
        self->pos = initParams->pos;
        self->velocity = initParams->velocity;
        self->accel = initParams->accel;
        self->life = initParams->life;
        self->rScaleStep = initParams->scaleStep;
        self->rAlphaMode = initParams->alphaStep;

        if (initParams->alphaStep != 0) {
            self->rAlphaStep = initParams->alphaStep;
        } else {
            self->rAlphaStep = initParams->alpha / initParams->life;
        }

        self->draw = EffectSsDeadDd_Draw;
        self->update = EffectSsDeadDd_Update;
        self->rScale = initParams->scale;
        self->rPrimColorR = initParams->primColor.r;
        self->rPrimColorG = initParams->primColor.g;
        self->rPrimColorB = initParams->primColor.b;
        self->rAlpha = initParams->alpha;
        self->rEnvColorR = initParams->envColor.r;
        self->rEnvColorG = initParams->envColor.g;
        self->rEnvColorB = initParams->envColor.b;

    } else if (initParams->type == 1) {
        self->life = initParams->life;
        self->rScaleStep = initParams->scaleStep;
        self->rAlphaMode = 0;
        self->rAlphaStep = 155 / initParams->life;
        self->rScale = initParams->scale;
        self->rPrimColorR = 255;
        self->rPrimColorG = 255;
        self->rPrimColorB = 155;
        self->rAlpha = 155;
        self->rEnvColorR = 250;
        self->rEnvColorG = 180;
        self->rEnvColorB = 0;
        self->draw = EffectSsDeadDd_Draw;
        self->update = EffectSsDeadDd_Update;

        for (i = initParams->randIter; i > 0; i--) {
            self->pos.x = ((Rand_ZeroOne() - 0.5f) * initParams->randPosScale) + initParams->pos.x;
            self->pos.y = ((Rand_ZeroOne() - 0.5f) * initParams->randPosScale) + initParams->pos.y;
            self->pos.z = ((Rand_ZeroOne() - 0.5f) * initParams->randPosScale) + initParams->pos.z;
            self->accel.x = self->velocity.x = (Rand_ZeroOne() - 0.5f) * 2.0f;
            self->accel.y = self->velocity.y = (Rand_ZeroOne() - 0.5f) * 2.0f;
            self->accel.z = self->velocity.z = (Rand_ZeroOne() - 0.5f) * 2.0f;
        }
    } else {
        osSyncPrintf("Effect_SS_Dd_disp_mode():mode_swが変です。\n");
        return 0;
    }

    return 1;
}

void EffectSsDeadDd_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    Mtx* mtx;
    f32 scale;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_dead_dd.c", 214);

    scale = self->rScale * 0.01f;
    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    SkinMatrix_SetScale(&mfScale, scale, scale, scale);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &mfScale, &mfResult);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        func_80094BC4(gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB, self->rAlpha);
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rAlpha);
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(POLY_XLU_DISP++, &D_01000000, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gDPSetCombineLERP(POLY_XLU_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
                          PRIMITIVE, 0);
        gSPDisplayList(POLY_XLU_DISP++, gLensFlareCircleDL);
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_dead_dd.c", 259);
}

void EffectSsDeadDd_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {

    self->rScale += self->rScaleStep;

    if (self->rScale < 0) {
        self->rScale = 0;
    }

    if (self->rAlphaMode != 0) {
        self->rAlpha += self->rAlphaStep;
        if (self->rAlpha > 255) {
            self->rAlpha = 255;
        }
    } else {
        if (self->rAlpha < self->rAlphaStep) {
            self->rAlpha = 0;
        } else {
            self->rAlpha -= self->rAlphaStep;
        }
    }
}
