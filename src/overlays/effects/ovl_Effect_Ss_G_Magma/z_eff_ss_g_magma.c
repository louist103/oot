/*
 * File: z_eff_ss_g_magma.c
 * Overlay: ovl_Effect_Ss_G_Magma
 * Description: Magma Bubbles
 */

#include "z_eff_ss_g_magma.h"
#include "objects/gameplay_keep/gameplay_keep.h"

u32 EffectSsGMagma_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsGMagma_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsGMagma_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_G_Magma_InitVars = {
    EFFECT_SS_G_MAGMA,
    EffectSsGMagma_Init,
};

u32 EffectSsGMagma_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsGMagmaInitParams* initParams = (EffectSsGMagmaInitParams*)initParamsx;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    self->velocity = self->accel = zeroVec;
    self->pos = initParams->pos;
    self->draw = EffectSsGMagma_Draw;
    self->update = EffectSsGMagma_Update;
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffMagmaBubbleDL);
    self->life = 16;
    self->rgScale = (s16)(Rand_ZeroOne() * 100.0f) + 200;
    self->rgTexIdx = 0;
    self->rgTexIdxStep = 50;
    self->rgPrimColorR = 255;
    self->rgPrimColorG = 255;
    self->rgPrimColorB = 0;
    self->rgPrimColorA = 255;
    self->rgEnvColorR = 255;
    self->rgEnvColorG = 0;
    self->rgEnvColorB = 0;
    self->rgEnvColorA = 0;

    return 1;
}

static void* sTextures[] = {
    gEffMagmaBubble1Tex, gEffMagmaBubble2Tex, gEffMagmaBubble3Tex, gEffMagmaBubble4Tex,
    gEffMagmaBubble5Tex, gEffMagmaBubble6Tex, gEffMagmaBubble7Tex, gEffMagmaBubble8Tex,
};

void EffectSsGMagma_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s16 texIdx = self->rgTexIdx / 100;

    if (texIdx > 7) {
        texIdx = 7;
    }

    EffectSs_DrawGEffect(globalCtx, self, sTextures[texIdx]);
}

void EffectSsGMagma_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->rgTexIdx += self->rgTexIdxStep;
}
