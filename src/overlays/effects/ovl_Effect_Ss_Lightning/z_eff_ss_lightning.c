/*
 * File: z_eff_ss_lightning.c
 * Overlay: ovl_Effect_Ss_Lightning
 * Description: Lightning
 */

#include "z_eff_ss_lightning.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rPrimColorR regs[0]
#define rPrimColorG regs[1]
#define rPrimColorB regs[2]
#define rPrimColorA regs[3]
#define rEnvColorR regs[4]
#define rEnvColorG regs[5]
#define rEnvColorB regs[6]
#define rEnvColorA regs[7]
#define rNumBolts regs[8]
#define rScale regs[9]
#define rYaw regs[10]
#define rLifespan regs[11]

u32 EffectSsLightning_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsLightning_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsLightning_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Lightning_InitVars = {
    EFFECT_SS_LIGHTNING,
    EffectSsLightning_Init,
};

u32 EffectSsLightning_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsLightningInitParams* initParams = (EffectSsLightningInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffLightningDL);
    self->life = initParams->life;
    self->draw = EffectSsLightning_Draw;
    self->update = EffectSsLightning_Update;
    self->rPrimColorR = initParams->primColor.r;
    self->rPrimColorG = initParams->primColor.g;
    self->rPrimColorB = initParams->primColor.b;
    self->rPrimColorA = initParams->primColor.a;
    self->rEnvColorR = initParams->envColor.r;
    self->rEnvColorG = initParams->envColor.g;
    self->rEnvColorB = initParams->envColor.b;
    self->rEnvColorA = initParams->envColor.a;
    self->rNumBolts = initParams->numBolts;
    self->rScale = initParams->scale;
    self->rYaw = initParams->yaw;
    self->rLifespan = initParams->life;

    return 1;
}

void EffectSsLightning_NewLightning(GlobalContext* globalCtx, Vec3f* pos, s16 yaw, EffectSs* self) {
    EffectSs newLightning;

    EffectSs_Delete(&newLightning);
    newLightning = *self;
    newLightning.pos = *pos;
    newLightning.rNumBolts--;
    newLightning.rYaw = yaw;
    newLightning.life = newLightning.rLifespan;

    EffectSs_Insert(globalCtx, &newLightning);
}

void EffectSsLightning_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    static void* lightningTextures[] = {
        gEffLightning1Tex, gEffLightning2Tex, gEffLightning3Tex, gEffLightning4Tex,
        gEffLightning5Tex, gEffLightning6Tex, gEffLightning7Tex, gEffLightning8Tex,
    };
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfResult;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfRotate;
    MtxF mfTrans11DA0;
    MtxF mfTrans11DA0Rotate;
    Mtx* mtx;
    f32 yScale;
    s16 texIdx;
    f32 xzScale;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_lightning.c", 233);

    yScale = self->rScale * 0.01f;
    texIdx = self->rLifespan - self->life;

    if (texIdx > 7) {
        texIdx = 7;
    }

    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    xzScale = yScale * 0.6f;
    SkinMatrix_SetScale(&mfScale, xzScale, yScale, xzScale);
    SkinMatrix_SetRotateRPY(&mfRotate, self->vec.x, self->vec.y, self->rYaw);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &globalCtx->mf_11DA0, &mfTrans11DA0);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0, &mfRotate, &mfTrans11DA0Rotate);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0Rotate, &mfScale, &mfResult);

    gSPMatrix(POLY_XLU_DISP++, &gMtxClear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_80094C50(gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(lightningTextures[texIdx]));
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                        self->rPrimColorA);
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rEnvColorA);
        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_lightning.c", 281);
}

void EffectSsLightning_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s32 pad;
    Vec3f pos;
    s16 yaw;
    f32 scale;

    if ((self->rNumBolts != 0) && ((self->life + 1) == self->rLifespan)) {

        yaw = self->rYaw + (((Rand_ZeroOne() < 0.5f) ? -1 : 1) * ((s16)((Rand_ZeroOne() * 3640.0f)) + 0xE38));

        scale = (self->rScale * 0.01f) * 80.0f;
        pos.y = self->pos.y + (Math_SinS(self->rYaw - 0x4000) * scale);

        scale = Math_CosS(self->rYaw - 0x4000) * scale;
        pos.x = self->pos.x - (Math_CosS(Camera_GetInputDirYaw(GET_ACTIVE_CAM(globalCtx))) * scale);
        pos.z = self->pos.z + (Math_SinS(Camera_GetInputDirYaw(GET_ACTIVE_CAM(globalCtx))) * scale);

        EffectSsLightning_NewLightning(globalCtx, &pos, yaw, self);

        if (Rand_ZeroOne() < 0.1f) {
            EffectSsLightning_NewLightning(globalCtx, &pos, (self->rYaw * 2) - yaw, self);
        }
    }
}
