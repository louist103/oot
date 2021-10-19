/*
 * File: z_eff_ss_dt_bubble.c
 * Overlay: ovl_Effect_Ss_Dt_Bubble
 * Description: Bubbles (a random mix of translucent and opaque)
 */

#include "z_eff_ss_dt_bubble.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rPrimColorR regs[0]
#define rPrimColorG regs[1]
#define rPrimColorB regs[2]
#define rPrimColorA regs[3]
#define rEnvColorR regs[4]
#define rEnvColorG regs[5]
#define rEnvColorB regs[6]
#define rEnvColorA regs[7]
#define rRandXZ regs[8]
#define rScale regs[9]
#define rLifespan regs[10]

u32 EffectSsDtBubble_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsDtBubble_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsDtBubble_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

static Color_RGBA8 sPrimColors[] = { { 255, 255, 100, 255 }, { 150, 255, 255, 255 }, { 100, 255, 255, 255 } };
static Color_RGBA8 sEnvColors[] = { { 170, 0, 0, 255 }, { 0, 100, 0, 255 }, { 0, 0, 255, 255 } };

EffectSsInit Effect_Ss_Dt_Bubble_InitVars = {
    EFFECT_SS_DT_BUBBLE,
    EffectSsDtBubble_Init,
};

u32 EffectSsDtBubble_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsDtBubbleInitParams* initParams = (EffectSsDtBubbleInitParams*)initParamsx;

    //! @bug Rand_ZeroOne in the macro means a random number is generated for both parts of the macro.
    // In the base game self works out because both addresses are segment 4, but it may break if
    // the addresses were changed to refer to different segments
    self->gfx = SEGMENTED_TO_VIRTUAL(Rand_ZeroOne() < 0.5f ? &gEffBubble1Tex : &gEffBubble2Tex);
    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->life = initParams->life;

    if (!initParams->customColor) {
        self->rPrimColorR = sPrimColors[initParams->colorProfile].r;
        self->rPrimColorG = sPrimColors[initParams->colorProfile].g;
        self->rPrimColorB = sPrimColors[initParams->colorProfile].b;
        self->rPrimColorA = sPrimColors[initParams->colorProfile].a;
        self->rEnvColorR = sEnvColors[initParams->colorProfile].r;
        self->rEnvColorG = sEnvColors[initParams->colorProfile].g;
        self->rEnvColorB = sEnvColors[initParams->colorProfile].b;
        self->rEnvColorA = sEnvColors[initParams->colorProfile].a;
    } else {
        self->rPrimColorR = initParams->primColor.r;
        self->rPrimColorG = initParams->primColor.g;
        self->rPrimColorB = initParams->primColor.b;
        self->rPrimColorA = initParams->primColor.a;
        self->rEnvColorR = initParams->envColor.r;
        self->rEnvColorG = initParams->envColor.g;
        self->rEnvColorB = initParams->envColor.b;
        self->rEnvColorA = initParams->envColor.a;
    }

    self->rRandXZ = initParams->randXZ;
    self->rScale = initParams->scale;
    self->rLifespan = initParams->life;
    self->draw = EffectSsDtBubble_Draw;
    self->update = EffectSsDtBubble_Update;

    return 1;
}

void EffectSsDtBubble_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    f32 scale;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_dt_bubble.c", 201);

    scale = self->rScale * 0.004f;
    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_ss_dt_bubble.c", 213),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80093C14(gfxCtx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                    (self->rPrimColorA * self->life) / self->rLifespan);
    gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB,
                   (self->rEnvColorA * self->life) / self->rLifespan);
    gSPSegment(POLY_XLU_DISP++, 0x08, self->gfx);
    gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gEffBubbleDL));

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_dt_bubble.c", 236);
}

void EffectSsDtBubble_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    f32 rand;

    if (self->rRandXZ == 1) {
        rand = Rand_ZeroOne();
        self->pos.x += (rand * 2.0f) - 1.0f;

        rand = Rand_ZeroOne();
        self->pos.z += (rand * 2.0f) - 1.0f;
    }
}
