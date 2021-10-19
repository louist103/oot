/*
 * File: z_eff_ss_sibuki2.c
 * Overlay: ovl_Effect_Ss_Sibuki2
 * Description: Unfinished and unused bubble effect
 */

#include "z_eff_ss_sibuki2.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rPrimColorR regs[0]
#define rPrimColorG regs[1]
#define rPrimColorB regs[2]
#define rPrimColorA regs[3]
#define rEnvColorR regs[4]
#define rEnvColorG regs[5]
#define rEnvColorB regs[6]
#define rEnvColorA regs[7]
#define rTexIdx regs[8]
#define rScale regs[9]

u32 EffectSsSibuki2_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsSibuki2_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsSibuki2_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Sibuki2_InitVars = {
    EFFECT_SS_SIBUKI2,
    EffectSsSibuki2_Init,
};

u32 EffectSsSibuki2_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsSibuki2InitParams* initParams = (EffectSsSibuki2InitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->life = 10;
    self->draw = EffectSsSibuki2_Draw;
    self->update = EffectSsSibuki2_Update;
    self->rScale = initParams->scale;
    self->rPrimColorR = 255;
    self->rPrimColorG = 255;
    self->rPrimColorB = 255;
    self->rPrimColorA = 255;
    self->rEnvColorR = 100;
    self->rEnvColorG = 100;
    self->rEnvColorB = 100;
    self->rEnvColorA = 255;
    self->rTexIdx = 0;

    return 1;
}

void EffectSsSibuki2_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    static void* bubbleTextures[] = {
        gEffUnusedBubbles1Tex, gEffUnusedBubbles1Tex, gEffUnusedBubbles2Tex,
        gEffUnusedBubbles3Tex, gEffUnusedBubbles4Tex, gEffUnusedBubbles5Tex,
        gEffUnusedBubbles6Tex, gEffUnusedBubbles7Tex, gEffUnusedBubbles8Tex,
    };
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    f32 scale = self->rScale / 100.0f;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_sibuki2.c", 158);

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_ss_sibuki2.c", 171),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80093D18(gfxCtx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB, self->rPrimColorA);
    gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rEnvColorA);
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(bubbleTextures[self->rTexIdx]));
    gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gEffUnusedBubblesDL));

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_sibuki2.c", 198);
}

void EffectSsSibuki2_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    if (self->rTexIdx < 8) {
        self->rTexIdx++;
    }
}
