/*
 * File: z_eff_ss_blast.c
 * Overlay: ovl_Effect_Ss_Blast
 * Description: Shockwave Effect
 */

#include "z_eff_ss_blast.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rPrimColorR regs[0]
#define rPrimColorG regs[1]
#define rPrimColorB regs[2]
#define rPrimColorA regs[3]
#define rEnvColorR regs[4]
#define rEnvColorG regs[5]
#define rEnvColorB regs[6]
#define rEnvColorA regs[7]
#define rAlphaTarget regs[8]
#define rScale regs[9]
#define rScaleStep regs[10]
#define rScaleStepDecay regs[11]

u32 EffectSsBlast_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsBlast_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsBlast_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Blast_InitVars = {
    EFFECT_SS_BLAST,
    EffectSsBlast_Init,
};

u32 EffectSsBlast_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsBlastParams* initParams = (EffectSsBlastParams*)initParamsx;

    self->pos = initParams->pos;
    self->pos.y += 5.0f;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffShockwaveDL);
    self->life = initParams->life;
    self->draw = EffectSsBlast_Draw;
    self->update = EffectSsBlast_Update;
    self->rPrimColorR = initParams->primColor.r;
    self->rPrimColorG = initParams->primColor.g;
    self->rPrimColorB = initParams->primColor.b;
    self->rPrimColorA = initParams->primColor.a;
    self->rEnvColorR = initParams->envColor.r;
    self->rEnvColorG = initParams->envColor.g;
    self->rEnvColorB = initParams->envColor.b;
    self->rEnvColorA = initParams->envColor.a;
    self->rAlphaTarget = initParams->primColor.a / initParams->life;
    self->rScale = initParams->scale;
    self->rScaleStep = initParams->scaleStep;
    self->rScaleStepDecay = initParams->sclaeStepDecay;
    return 1;
}

void EffectSsBlast_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mf;
    s32 pad;
    f32 radius;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_blast.c", 170);

    radius = self->rScale * 0.0025f;

    func_80093D84(globalCtx->state.gfxCtx);
    gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rEnvColorA);
    func_800BFCB8(globalCtx, &mf, &self->pos);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB, self->rPrimColorA);
    Matrix_Put(&mf);
    Matrix_Scale(radius, radius, radius, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_ss_blast.c", 199),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, self->gfx);

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_blast.c", 204);
}

void EffectSsBlast_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    Math_StepToS(&self->rPrimColorA, 0, self->rAlphaTarget);
    self->rScale += self->rScaleStep;

    if (self->rScaleStep != 0) {
        self->rScaleStep -= self->rScaleStepDecay;
    }
}
