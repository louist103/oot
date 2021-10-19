/*
 * File: z_eff_ss_g_splash.c
 * Overlay: ovl_Effect_Ss_G_Splash
 * Description: Splash
 */

#include "z_eff_ss_g_splash.h"
#include "objects/gameplay_keep/gameplay_keep.h"

//! @bug the reuse of regs[11] means that EffectSs_DrawGEffect will treat the type as an object bank index
// self ends up having no effect because the texture provided does not use segment 6
#define rType regs[11]

u32 EffectSsGSplash_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParams);
void EffectSsGSplash_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsGSplash_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_G_Splash_InitVars = {
    EFFECT_SS_G_SPLASH,
    EffectSsGSplash_Init,
};

u32 EffectSsGSplash_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsGSplashInitParams* initParams = (EffectSsGSplashInitParams*)initParamsx;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    self->velocity = self->accel = zeroVec;
    self->pos = initParams->pos;
    self->draw = EffectSsGSplash_Draw;
    self->update = EffectSsGSplash_Update;

    if (initParams->scale == 0) {
        initParams->scale = 600;
    }

    self->gfx = SEGMENTED_TO_VIRTUAL(gEffWaterSplashDL);
    self->life = 8;
    self->rgScale = initParams->scale;
    self->rgTexIdx = 0;
    self->rgTexIdxStep = 100;

    if (initParams->customColor) {
        self->rgPrimColorR = initParams->primColor.r;
        self->rgPrimColorG = initParams->primColor.g;
        self->rgPrimColorB = initParams->primColor.b;
        self->rgPrimColorA = initParams->primColor.a;
        self->rgEnvColorR = initParams->envColor.r;
        self->rgEnvColorG = initParams->envColor.g;
        self->rgEnvColorB = initParams->envColor.b;
        self->rgEnvColorA = initParams->envColor.a;
        self->rType = initParams->type;
    } else {
        switch (initParams->type) {
            case 0:
                self->rgPrimColorR = 255;
                self->rgPrimColorG = 255;
                self->rgPrimColorB = 255;
                self->rgPrimColorA = 200;
                self->rgEnvColorR = 255;
                self->rgEnvColorG = 255;
                self->rgEnvColorB = 255;
                self->rgEnvColorA = 200;
                self->rType = 0;
                break;
            case 1:
                self->rgPrimColorR = 255;
                self->rgPrimColorG = 255;
                self->rgPrimColorB = 255;
                self->rgPrimColorA = 255;
                self->rgEnvColorR = 255;
                self->rgEnvColorG = 255;
                self->rgEnvColorB = 255;
                self->rgEnvColorA = 255;
                self->rType = 1;
                break;
            case 2:
                self->rgPrimColorR = 255;
                self->rgPrimColorG = 255;
                self->rgPrimColorB = 255;
                self->rgPrimColorA = 200;
                self->rgEnvColorR = 255;
                self->rgEnvColorG = 255;
                self->rgEnvColorB = 255;
                self->rgEnvColorA = 200;
                self->rType = 2;
                break;
        }
    }
    return 1;
}

void EffectSsGSplash_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    static void* waterSplashTextures[] = {
        gEffWaterSplash1Tex, gEffWaterSplash2Tex, gEffWaterSplash3Tex, gEffWaterSplash4Tex,
        gEffWaterSplash5Tex, gEffWaterSplash6Tex, gEffWaterSplash7Tex, gEffWaterSplash8Tex,
    };
    s16 texIdx;

    switch (self->rType) {
        case 0:
            texIdx = self->rgTexIdx / 100;
            if (texIdx > 7) {
                texIdx = 7;
            }
            EffectSs_DrawGEffect(globalCtx, self, waterSplashTextures[texIdx]);
            break;

        case 1:
            texIdx = self->rgTexIdx / 100;
            if (texIdx > 7) {
                texIdx = 7;
            }
            EffectSs_DrawGEffect(globalCtx, self, waterSplashTextures[texIdx]);
            break;

        case 2:
            texIdx = self->rgTexIdx / 100;
            if (texIdx > 7) {
                texIdx = 7;
            }
            EffectSs_DrawGEffect(globalCtx, self, waterSplashTextures[texIdx]);
            break;

        default:
            break;
    }
}

void EffectSsGSplash_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    Vec3f newSplashPos;

    if ((self->rType == 1) && (self->life == 5)) {
        newSplashPos = self->pos;
        newSplashPos.y += ((self->rgScale * 20) * 0.002f);
        EffectSsGSplash_Spawn(globalCtx, &newSplashPos, 0, 0, 2, self->rgScale / 2);
    }

    self->rgTexIdx += self->rgTexIdxStep;
}
