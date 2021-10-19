/*
 * File: z_eff_ss_hitmark.c
 * Overlay: ovl_Effect_Ss_HitMark
 * Description: Hit Marks
 */

#include "z_eff_ss_hitmark.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rTexIdx regs[0]
#define rType regs[1]
#define rPrimColorR regs[2]
#define rPrimColorG regs[3]
#define rPrimColorB regs[4]
#define rEnvColorR regs[5]
#define rEnvColorG regs[6]
#define rEnvColorB regs[7]
#define rScale regs[8]

u32 EffectSsHitMark_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsHitMark_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsHitMark_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

static Color_RGB8 sColors[] = {
    { 255, 255, 255 }, { 255, 255, 0 }, { 255, 255, 255 }, { 255, 0, 0 },   { 255, 200, 100 }, { 200, 150, 0 },
    { 150, 100, 0 },   { 100, 50, 0 },  { 255, 255, 255 }, { 255, 0, 0 },   { 255, 255, 0 },   { 255, 0, 0 },
    { 255, 255, 255 }, { 0, 255, 200 }, { 255, 255, 255 }, { 150, 0, 255 },
};

static void* sTextures[] = {
    gEffHitMark1Tex,  gEffHitMark2Tex,  gEffHitMark3Tex,  gEffHitMark4Tex,  gEffHitMark5Tex,  gEffHitMark6Tex,
    gEffHitMark7Tex,  gEffHitMark8Tex,  gEffHitMark9Tex,  gEffHitMark10Tex, gEffHitMark11Tex, gEffHitMark12Tex,
    gEffHitMark13Tex, gEffHitMark14Tex, gEffHitMark15Tex, gEffHitMark16Tex, gEffHitMark17Tex, gEffHitMark18Tex,
    gEffHitMark19Tex, gEffHitMark20Tex, gEffHitMark21Tex, gEffHitMark22Tex, gEffHitMark23Tex, gEffHitMark24Tex,
    gEffHitMark1Tex,  gEffHitMark2Tex,  gEffHitMark3Tex,  gEffHitMark4Tex,  gEffHitMark5Tex,  gEffHitMark6Tex,
    gEffHitMark7Tex,  gEffHitMark8Tex,
};

EffectSsInit Effect_Ss_HitMark_InitVars = {
    EFFECT_SS_HITMARK,
    EffectSsHitMark_Init,
};

u32 EffectSsHitMark_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    s32 colorIdx;
    EffectSsHitMarkInitParams* initParams = (EffectSsHitMarkInitParams*)initParamsx;
    self->pos = initParams->pos;
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffHitMarkDL);

    if (initParams->type == EFFECT_HITMARK_DUST) {
        self->life = 16;
    } else {
        self->life = 8;
    }

    self->draw = EffectSsHitMark_Draw;
    self->update = EffectSsHitMark_Update;
    colorIdx = initParams->type * 4;
    self->rTexIdx = 0;
    self->rType = initParams->type;
    self->rPrimColorR = sColors[colorIdx].r;
    self->rPrimColorG = sColors[colorIdx].g;
    self->rPrimColorB = sColors[colorIdx].b;
    self->rEnvColorR = sColors[colorIdx + 1].r;
    self->rEnvColorG = sColors[colorIdx + 1].g;
    self->rEnvColorB = sColors[colorIdx + 1].b;
    self->rScale = initParams->scale;

    return 1;
}

void EffectSsHitMark_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    MtxF mfTrans11DA0;
    Mtx* mtx;
    f32 scale;
    s32 pad;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_hitmark.c", 297);

    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    scale = self->rScale / 100.0f;
    SkinMatrix_SetScale(&mfScale, scale, scale, 1.0f);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &globalCtx->mf_11DA0, &mfTrans11DA0);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0, &mfScale, &mfResult);
    gSPMatrix(POLY_XLU_DISP++, &gMtxClear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sTextures[(self->rType * 8) + (self->rTexIdx)]));
        func_80094C50(gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, 0);
        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    }
    CLOSE_DISPS(gfxCtx, "../z_eff_ss_hitmark.c", 341);
}

void EffectSsHitMark_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s32 colorIdx;

    if (self->rType == EFFECT_HITMARK_DUST) {
        self->rTexIdx = (15 - self->life) / 2;
    } else {
        self->rTexIdx = 7 - self->life;
    }

    if (self->rTexIdx != 0) {
        colorIdx = self->rType * 4 + 2;
        self->rPrimColorR = func_80027DD4(self->rPrimColorR, sColors[colorIdx].r, self->life + 1);
        self->rPrimColorG = func_80027DD4(self->rPrimColorG, sColors[colorIdx].g, self->life + 1);
        self->rPrimColorB = func_80027DD4(self->rPrimColorB, sColors[colorIdx].b, self->life + 1);
        self->rEnvColorR = func_80027DD4(self->rEnvColorR, sColors[colorIdx + 1].r, self->life + 1);
        self->rEnvColorG = func_80027DD4(self->rEnvColorG, sColors[colorIdx + 1].g, self->life + 1);
        self->rEnvColorB = func_80027DD4(self->rEnvColorB, sColors[colorIdx + 1].b, self->life + 1);
    }
}
