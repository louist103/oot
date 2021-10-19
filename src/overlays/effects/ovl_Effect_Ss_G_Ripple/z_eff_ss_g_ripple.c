/*
 * File: z_eff_ss_g_ripple.c
 * Overlay: ovl_Effect_Ss_G_Ripple
 * Description: Water Ripple
 */

#include "z_eff_ss_g_ripple.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rWaterBoxNum regs[0]
#define rRadius regs[1]
#define rRadiusMax regs[2]
#define rPrimColorR regs[3]
#define rPrimColorG regs[4]
#define rPrimColorB regs[5]
#define rPrimColorA regs[6]
#define rEnvColorR regs[7]
#define rEnvColorG regs[8]
#define rEnvColorB regs[9]
#define rEnvColorA regs[10]
#define rLifespan regs[11]

u32 EffectSsGRipple_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsGRipple_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsGRipple_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_G_Ripple_InitVars = {
    EFFECT_SS_G_RIPPLE,
    EffectSsGRipple_Init,
};

u32 EffectSsGRipple_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    s32 pad;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    WaterBox* waterBox;
    EffectSsGRippleInitParams* initParams = (EffectSsGRippleInitParams*)initParamsx;

    waterBox = NULL;
    self->velocity = self->accel = zeroVec;
    self->pos = initParams->pos;
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffWaterRippleDL);
    self->life = initParams->life + 20;
    self->flags = 0;
    self->draw = EffectSsGRipple_Draw;
    self->update = EffectSsGRipple_Update;
    self->rRadius = initParams->radius;
    self->rRadiusMax = initParams->radiusMax;
    self->rLifespan = initParams->life;
    self->rPrimColorR = 255;
    self->rPrimColorG = 255;
    self->rPrimColorB = 255;
    self->rPrimColorA = 255;
    self->rEnvColorR = 255;
    self->rEnvColorG = 255;
    self->rEnvColorB = 255;
    self->rEnvColorA = 255;
    self->rWaterBoxNum = WaterBox_GetSurface2(globalCtx, &globalCtx->colCtx, &initParams->pos, 3.0f, &waterBox);

    return 1;
}

void EffectSsGRipple_DrawRipple(GlobalContext* globalCtx, EffectSs* self, UNK_PTR segment) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    f32 radius;
    s32 pad;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    Mtx* mtx;
    f32 yPos;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_g_ripple.c", 199);

    if (globalCtx) {}

    radius = self->rRadius * 0.0025f;

    if ((self->rWaterBoxNum != -1) && (self->rWaterBoxNum < globalCtx->colCtx.colHeader->numWaterBoxes)) {
        yPos = (self->rWaterBoxNum + globalCtx->colCtx.colHeader->waterBoxes)->ySurface;
    } else {
        yPos = self->pos.y;
    }

    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, yPos, self->pos.z);
    SkinMatrix_SetScale(&mfScale, radius, radius, radius);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &mfScale, &mfResult);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_80094BC4(gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                        self->rPrimColorA);
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rEnvColorA);
        gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_NOISE);
        gDPSetColorDither(POLY_XLU_DISP++, G_CD_NOISE);
        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_g_ripple.c", 247);
}

void EffectSsGRipple_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    if (self->rLifespan == 0) {
        EffectSsGRipple_DrawRipple(globalCtx, self, gEffWaterRippleTex);
    }
}

void EffectSsGRipple_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    f32 radius;
    f32 primAlpha;
    f32 envAlpha;

    if (DECR(self->rLifespan) == 0) {
        radius = self->rRadius;
        Math_SmoothStepToF(&radius, self->rRadiusMax, 0.2f, 30.0f, 1.0f);
        self->rRadius = radius;

        primAlpha = self->rPrimColorA;
        envAlpha = self->rEnvColorA;

        Math_SmoothStepToF(&primAlpha, 0.0f, 0.2f, 15.0f, 7.0f);
        Math_SmoothStepToF(&envAlpha, 0.0f, 0.2f, 15.0f, 7.0f);

        self->rPrimColorA = primAlpha;
        self->rEnvColorA = envAlpha;
    }
}
