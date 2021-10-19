/*
 * File: z_eff_ss_bubble.c
 * Overlay: ovl_Effect_Ss_Bubble
 * Description:
 */

#include "z_eff_ss_bubble.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rScale regs[0]

u32 EffectSsBubble_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsBubble_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsBubble_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Bubble_InitVars = {
    EFFECT_SS_BUBBLE,
    EffectSsBubble_Init,
};

u32 EffectSsBubble_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsBubbleInitParams* initParams = (EffectSsBubbleInitParams*)initParamsx;

    //! @bug Rand_ZeroOne in the macro means a random number is generated for both parts of the macro.
    // In the base game self works out because both addresses are segment 4, but it may break if
    // the addresses were changed to refer to different segments
    self->gfx = SEGMENTED_TO_VIRTUAL(Rand_ZeroOne() < 0.5f ? &gEffBubble1Tex : &gEffBubble2Tex);
    self->pos.x = ((Rand_ZeroOne() - 0.5f) * initParams->xzPosRandScale) + initParams->pos.x;
    self->pos.y = (((Rand_ZeroOne() - 0.5f) * initParams->yPosRandScale) + initParams->yPosOffset) + initParams->pos.y;
    self->pos.z = ((Rand_ZeroOne() - 0.5f) * initParams->xzPosRandScale) + initParams->pos.z;
    Math_Vec3f_Copy(&self->vec, &self->pos);
    self->life = 1;
    self->rScale = (((Rand_ZeroOne() * 0.5f) + 1.0f) * initParams->scale) * 100;
    self->draw = EffectSsBubble_Draw;
    self->update = EffectSsBubble_Update;

    return 1;
}

void EffectSsBubble_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    f32 scale = self->rScale / 100.0f;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_bubble.c", 154);

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_ss_bubble.c", 167),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80093D18(gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, 150, 150, 150, 0);
    gSPSegment(POLY_OPA_DISP++, 0x08, self->gfx);
    gSPDisplayList(POLY_OPA_DISP++, SEGMENTED_TO_VIRTUAL(gEffBubbleDL));

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_bubble.c", 179);
}

void EffectSsBubble_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    WaterBox* waterBox;
    f32 waterSurfaceY;
    Vec3f ripplePos;

    waterSurfaceY = self->pos.y;

    // kill bubble if its out of range of a water box
    if (!WaterBox_GetSurface1(globalCtx, &globalCtx->colCtx, self->pos.x, self->pos.z, &waterSurfaceY, &waterBox)) {
        self->life = -1;
        return;
    }

    if (waterSurfaceY < self->pos.y) {
        ripplePos.x = self->pos.x;
        ripplePos.y = waterSurfaceY;
        ripplePos.z = self->pos.z;
        EffectSsGRipple_Spawn(globalCtx, &ripplePos, 0, 80, 0);
        self->life = -1;
    } else {
        self->life++;
        self->pos.x = ((Rand_ZeroOne() * 0.5f) - 0.25f) + self->vec.x;
        self->accel.y = (Rand_ZeroOne() - 0.3f) * 0.2f;
        self->pos.z = ((Rand_ZeroOne() * 0.5f) - 0.25f) + self->vec.z;
    }
}
