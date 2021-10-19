/*
 * File: z_eff_ss_bomb2.c
 * Overlay: ovl_Effect_Ss_Bomb2
 * Description: Bomb Blast
 */

#include "z_eff_ss_bomb2.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rScale regs[0]
#define rTexIdx regs[1]
#define rPrimColorR regs[2]
#define rPrimColorG regs[3]
#define rPrimColorB regs[4]
#define rPrimColorA regs[5]
#define rEnvColorR regs[6]
#define rEnvColorG regs[7]
#define rEnvColorB regs[8]
#define rScaleStep regs[9]
#define rDepth regs[10]

u32 EffectSsBomb2_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsBomb2_DrawFade(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsBomb2_DrawLayered(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsBomb2_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Bomb2_InitVars = {
    EFFECT_SS_BOMB2,
    EffectSsBomb2_Init,
};

static EffectSsDrawFunc sDrawFuncs[] = {
    EffectSsBomb2_DrawFade,
    EffectSsBomb2_DrawLayered,
};

u32 EffectSsBomb2_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {

    EffectSsBomb2InitParams* initParams = (EffectSsBomb2InitParams*)initParamsx;

    Math_Vec3f_Copy(&self->pos, &initParams->pos);
    Math_Vec3f_Copy(&self->velocity, &initParams->velocity);
    Math_Vec3f_Copy(&self->accel, &initParams->accel);
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffBombExplosion1DL);
    self->life = 24;
    self->update = EffectSsBomb2_Update;
    self->draw = sDrawFuncs[initParams->drawMode];
    self->rScale = initParams->scale;
    self->rScaleStep = initParams->scaleStep;
    self->rPrimColorR = 255;
    self->rPrimColorG = 255;
    self->rPrimColorB = 255;
    self->rPrimColorA = 255;
    self->rEnvColorR = 0;
    self->rEnvColorG = 0;
    self->rEnvColorB = 200;

    return 1;
}

// unused in the original game. looks like EffectSsBomb but with color
void EffectSsBomb2_DrawFade(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    static void* textures[] = {
        gEffBombExplosion1Tex, gEffBombExplosion2Tex, gEffBombExplosion3Tex, gEffBombExplosion4Tex,
        gEffBombExplosion5Tex, gEffBombExplosion6Tex, gEffBombExplosion7Tex, gEffBombExplosion8Tex,
    };
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    MtxF mfTrans11DA0;
    Mtx* mtx;
    s32 pad;
    f32 scale;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_bomb2.c", 298);

    scale = self->rScale * 0.01f;
    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    SkinMatrix_SetScale(&mfScale, scale, scale, 1.0f);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &globalCtx->mf_11DA0, &mfTrans11DA0);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0, &mfScale, &mfResult);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_80094BC4(gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                        self->rPrimColorA);
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, 0);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(textures[self->rTexIdx]));
        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    }

    if (1) {}
    if (1) {}

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_bomb2.c", 345);
}

void EffectSsBomb2_DrawLayered(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    static void* textures[] = {
        gEffBombExplosion1Tex, gEffBombExplosion2Tex, gEffBombExplosion3Tex, gEffBombExplosion4Tex,
        gEffBombExplosion5Tex, gEffBombExplosion6Tex, gEffBombExplosion7Tex, gEffBombExplosion8Tex,
    };
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    MtxF mfTrans11DA0;
    MtxF mtx2F;
    Mtx* mtx2;
    Mtx* mtx;
    s32 pad[3];
    f32 scale;
    f32 depth;
    f32 layer2Scale = 0.925f;
    s32 i;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_bomb2.c", 386);

    depth = self->rDepth;
    scale = self->rScale * 0.01f;
    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    SkinMatrix_SetScale(&mfScale, scale, scale, 1.0f);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &globalCtx->mf_11DA0, &mfTrans11DA0);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0, &mfScale, &mfResult);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        mtx2 = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

        if (mtx2 != NULL) {
            func_80094BC4(gfxCtx);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                            self->rPrimColorA);
            gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, 0);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(textures[self->rTexIdx]));
            gSPDisplayList(POLY_XLU_DISP++, gEffBombExplosion2DL);
            gSPDisplayList(POLY_XLU_DISP++, gEffBombExplosion3DL);

            Matrix_MtxToMtxF(mtx2, &mtx2F);
            Matrix_Put(&mtx2F);

            for (i = 1; i >= 0; i--) {
                Matrix_Translate(0.0f, 0.0f, depth, MTXMODE_APPLY);
                Matrix_RotateZ((self->life * 0.02f) + 180.0f, MTXMODE_APPLY);
                Matrix_Scale(layer2Scale, layer2Scale, layer2Scale, MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_eff_ss_bomb2.c", 448),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, gEffBombExplosion3DL);
                layer2Scale -= 0.15f;
            }
        }
    }

    if (1) {}
    if (1) {}

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_bomb2.c", 456);
}

void EffectSsBomb2_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s32 divisor;

    self->rTexIdx = (23 - self->life) / 3;
    self->rScale += self->rScaleStep;

    if (self->rScaleStep == 30) {
        self->rDepth += 4.0f;
    } else {
        self->rDepth += 2.0f;
    }

    if ((self->life < 23) && (self->life > 13)) {
        divisor = self->life - 13;
        self->rPrimColorR = func_80027DD4(self->rPrimColorR, 255, divisor);
        self->rPrimColorG = func_80027DD4(self->rPrimColorG, 255, divisor);
        self->rPrimColorB = func_80027DD4(self->rPrimColorB, 150, divisor);
        self->rPrimColorA = func_80027DD4(self->rPrimColorA, 255, divisor);
        self->rEnvColorR = func_80027DD4(self->rEnvColorR, 150, divisor);
        self->rEnvColorG = func_80027DD4(self->rEnvColorG, 0, divisor);
        self->rEnvColorB = func_80027DD4(self->rEnvColorB, 0, divisor);
    } else if ((self->life < 14) && (self->life > -1)) {
        divisor = self->life + 1;
        self->rPrimColorR = func_80027DD4(self->rPrimColorR, 50, divisor);
        self->rPrimColorG = func_80027DD4(self->rPrimColorG, 50, divisor);
        self->rPrimColorB = func_80027DD4(self->rPrimColorB, 50, divisor);
        self->rPrimColorA = func_80027DD4(self->rPrimColorA, 150, divisor);
        self->rEnvColorR = func_80027DD4(self->rEnvColorR, 10, divisor);
        self->rEnvColorG = func_80027DD4(self->rEnvColorG, 10, divisor);
        self->rEnvColorB = func_80027DD4(self->rEnvColorB, 10, divisor);
    }
}
