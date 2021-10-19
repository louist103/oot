/*
 * File: z_eff_ss_g_fire.c
 * Overlay: ovl_Effect_Ss_G_Fire
 * Description: Flame Footprints
 */

#include "z_eff_ss_g_fire.h"
#include "objects/gameplay_keep/gameplay_keep.h"

u32 EffectSsGFire_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsGFire_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsGFire_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_G_Fire_InitVars = {
    EFFECT_SS_G_FIRE,
    EffectSsGFire_Init,
};

u32 EffectSsGFire_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsGFireInitParams* initParams = (EffectSsGFireInitParams*)initParamsx;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    self->velocity = self->accel = zeroVec;
    self->pos = initParams->pos;
    self->draw = EffectSsGFire_Draw;
    self->update = EffectSsGFire_Update;
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffFireFootprintDL);
    self->life = 8;
    self->flags = 0;
    self->rgScale = 200;
    self->rgTexIdx = 0;
    self->rgTexIdxStep = 50;
    self->rgPrimColorR = 255;
    self->rgPrimColorG = 220;
    self->rgPrimColorB = 80;
    self->rgPrimColorA = 255;
    self->rgEnvColorR = 130;
    self->rgEnvColorG = 30;
    self->rgEnvColorB = 0;
    self->rgEnvColorA = 0;

    return 1;
}

void EffectSsGFire_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    void* fireFootprintTextures[] = {
        gEffFireFootprint1Tex, gEffFireFootprint2Tex, gEffFireFootprint3Tex, gEffFireFootprint4Tex,
        gEffFireFootprint5Tex, gEffFireFootprint6Tex, gEffFireFootprint7Tex, gEffFireFootprint8Tex,
    };
    s16 texIdx = (self->rgTexIdx / 100) % 7;

    EffectSs_DrawGEffect(globalCtx, self, fireFootprintTextures[texIdx]);
}

void EffectSsGFire_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->rgTexIdx += self->rgTexIdxStep;
}
