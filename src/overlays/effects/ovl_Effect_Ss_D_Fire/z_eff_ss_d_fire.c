/*
 * File: z_eff_ss_d_fire.c
 * Overlay: ovl_Effect_Ss_D_Fire
 * Description: Dodongo Fire
 */

#include "z_eff_ss_d_fire.h"
#include "objects/object_dodongo/object_dodongo.h"

#define rScale regs[0]
#define rTexIdx regs[1]
#define rPrimColorR regs[2]
#define rPrimColorG regs[3]
#define rPrimColorB regs[4]
#define rPrimColorA regs[5]
#define rFadeDelay regs[6]
#define rScaleStep regs[9]
#define rObjBankIdx regs[10]
#define rYAccelStep regs[11] // has no effect due to how its implemented

u32 EffectSsDFire_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsDFire_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsDFire_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_D_Fire_InitVars = {
    EFFECT_SS_D_FIRE,
    EffectSsDFire_Init,
};

u32 EffectSsDFire_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsDFireInitParams* initParams = (EffectSsDFireInitParams*)initParamsx;
    s32 objBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_DODONGO);

    if (objBankIndex >= 0) {
        self->pos = initParams->pos;
        self->velocity = initParams->velocity;
        self->accel = initParams->accel;
        self->gfx = SEGMENTED_TO_VIRTUAL(gDodongoFireDL);
        self->life = initParams->life;
        self->rScale = initParams->scale;
        self->rScaleStep = initParams->scaleStep;
        self->rYAccelStep = 0;
        self->rObjBankIdx = objBankIndex;
        self->draw = EffectSsDFire_Draw;
        self->update = EffectSsDFire_Update;
        self->rTexIdx = ((s16)(globalCtx->state.frames % 4) ^ 3);
        self->rPrimColorR = 255;
        self->rPrimColorG = 255;
        self->rPrimColorB = 50;
        self->rPrimColorA = initParams->alpha;
        self->rFadeDelay = initParams->fadeDelay;

        return 1;
    }

    return 0;
}

static void* sTextures[] = { gDodongoFire0Tex, gDodongoFire1Tex, gDodongoFire2Tex, gDodongoFire3Tex };

void EffectSsDFire_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    MtxF mfTrans11DA0;
    s32 pad;
    void* object;
    Mtx* mtx;
    f32 scale;

    object = globalCtx->objectCtx.status[self->rObjBankIdx].segment;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_d_fire.c", 276);

    if (Object_GetIndex(&globalCtx->objectCtx, OBJECT_DODONGO) > -1) {
        gSegments[6] = VIRTUAL_TO_PHYSICAL(object);
        gSPSegment(POLY_XLU_DISP++, 0x06, object);
        scale = self->rScale / 100.0f;
        SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
        SkinMatrix_SetScale(&mfScale, scale, scale, 1.0f);
        SkinMatrix_MtxFMtxFMult(&mfTrans, &globalCtx->mf_11DA0, &mfTrans11DA0);
        SkinMatrix_MtxFMtxFMult(&mfTrans11DA0, &mfScale, &mfResult);

        mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

        if (mtx != NULL) {
            gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            func_80094BC4(gfxCtx);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                            self->rPrimColorA);
            gSegments[6] = VIRTUAL_TO_PHYSICAL(object);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sTextures[self->rTexIdx]));
            gSPDisplayList(POLY_XLU_DISP++, self->gfx);
        }
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_d_fire.c", 330);
}

void EffectSsDFire_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->rTexIdx++;
    self->rTexIdx &= 3;
    self->rScale += self->rScaleStep;

    if (self->rFadeDelay >= self->life) {
        self->rPrimColorA -= 5;
        if (self->rPrimColorA < 0) {
            self->rPrimColorA = 0;
        }
    } else {
        self->rPrimColorA += 15;
        if (self->rPrimColorA > 255) {
            self->rPrimColorA = 255;
        }
    }

    if (self->accel.y < 0.0f) {
        self->accel.y += self->rYAccelStep * 0.01f;
    }

    if (self->life <= 0) {
        self->rYAccelStep += 0;
    }
}
