/*
 * File: z_eff_ss_kirakira.c
 * Overlay: ovl_Effect_Ss_KiraKira
 * Description: Sparkles
 */

#include "z_eff_ss_kirakira.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rRotSpeed regs[0]
#define rYaw regs[1]
#define rPrimColorR regs[2]
#define rPrimColorG regs[3]
#define rPrimColorB regs[4]
#define rPrimColorA regs[5]
#define rEnvColorR regs[6]
#define rEnvColorG regs[7]
#define rEnvColorB regs[8]
#define rEnvColorA regs[9]
#define rAlphaStep regs[10]
#define rScale regs[11]
#define rLifespan regs[12]

u32 EffectSsKiraKira_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsKiraKira_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void func_809AABF0(GlobalContext* globalCtx, u32 index, EffectSs* self);
void func_809AACAC(GlobalContext* globalCtx, u32 index, EffectSs* self);
void func_809AAD6C(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_KiraKira_InitVars = {
    EFFECT_SS_KIRAKIRA,
    EffectSsKiraKira_Init,
};

u32 EffectSsKiraKira_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsKiraKiraInitParams* initParams = (EffectSsKiraKiraInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;

    if ((self->life = initParams->life) < 0) {
        self->life = -self->life;
        self->gfx = SEGMENTED_TO_VIRTUAL(gEffSparklesDL);
        self->update = func_809AAD6C;
        self->rEnvColorA = initParams->scale;
        self->rScale = 0;
    } else {
        self->gfx = SEGMENTED_TO_VIRTUAL(gEffSparklesDL);

        if (initParams->updateMode == 0) {
            self->update = func_809AABF0;
        } else {
            self->update = func_809AACAC;
        }

        self->rEnvColorA = initParams->envColor.a;
        self->rScale = initParams->scale;
    }

    self->draw = EffectSsKiraKira_Draw;
    self->rRotSpeed = initParams->rotSpeed;
    self->rYaw = initParams->yaw;
    self->rPrimColorR = initParams->primColor.r;
    self->rPrimColorG = initParams->primColor.g;
    self->rPrimColorB = initParams->primColor.b;
    self->rPrimColorA = initParams->primColor.a;
    self->rEnvColorR = initParams->envColor.r;
    self->rEnvColorG = initParams->envColor.g;
    self->rEnvColorB = initParams->envColor.b;
    self->rAlphaStep = initParams->alphaStep;
    self->rLifespan = initParams->life;

    return 1;
}

void EffectSsKiraKira_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx;
    f32 scale;
    s32 pad;
    MtxF mfTrans;
    MtxF mfRotY;
    MtxF mfScale;
    MtxF mfTrans11DA0;
    MtxF mfTrans11DA0RotY;
    MtxF mfResult;
    Mtx* mtx;

    scale = self->rScale / 10000.0f;
    gfxCtx = globalCtx->state.gfxCtx;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_kirakira.c", 257);

    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    SkinMatrix_SetRotateRPY(&mfRotY, 0, 0, self->rYaw);
    SkinMatrix_SetScale(&mfScale, scale, scale, 1.0f);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &globalCtx->mf_11DA0, &mfTrans11DA0);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0, &mfRotY, &mfTrans11DA0RotY);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0RotY, &mfScale, &mfResult);
    gSPMatrix(POLY_XLU_DISP++, &gMtxClear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_80093C14(gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                        (((s8)((55.0f / self->rLifespan) * self->life) + 200)));
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rEnvColorA);
        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_kirakira.c", 301);
}

void func_809AABF0(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->accel.x = (Rand_ZeroOne() * 0.4f) - 0.2f;
    self->accel.z = (Rand_ZeroOne() * 0.4f) - 0.2f;
    self->rEnvColorA += self->rAlphaStep;

    if (self->rEnvColorA < 0) {
        self->rEnvColorA = 0;
        self->rAlphaStep = -self->rAlphaStep;
    } else if (self->rEnvColorA > 255) {
        self->rEnvColorA = 255;
        self->rAlphaStep = -self->rAlphaStep;
    }

    self->rYaw += self->rRotSpeed;
}

void func_809AACAC(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->velocity.x *= 0.95f;
    self->velocity.z *= 0.95f;
    self->accel.x = Rand_CenteredFloat(0.2f);
    self->accel.z = Rand_CenteredFloat(0.2f);
    self->rEnvColorA += self->rAlphaStep;

    if (self->rEnvColorA < 0) {
        self->rEnvColorA = 0;
        self->rAlphaStep = -self->rAlphaStep;
    } else if (self->rEnvColorA > 255) {
        self->rEnvColorA = 255;
        self->rAlphaStep = -self->rAlphaStep;
    }

    self->rYaw += self->rRotSpeed;
}

void func_809AAD6C(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->rScale = self->rEnvColorA * Math_SinS((32768.0f / self->rLifespan) * self->life);
}
