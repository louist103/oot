/*
 * File: z_eff_ss_dust.c
 * Overlay: ovl_Effect_Ss_Dust
 * Description: Dust Particle Effect
 */

#include "z_eff_ss_dust.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rPrimColorR regs[0]
#define rPrimColorG regs[1]
#define rPrimColorB regs[2]
#define rPrimColorA regs[3]
#define rEnvColorR regs[4]
#define rEnvColorG regs[5]
#define rEnvColorB regs[6]
#define rEnvColorA regs[7]
#define rTexIdx regs[8] // self reg is also used to set specific colors in the fire update function
#define rScale regs[9]
#define rScaleStep regs[10]
#define rDrawFlags regs[11]
#define rLifespan regs[12]

u32 EffectSsDust_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsDust_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsBlast_UpdateFire(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsDust_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Dust_InitVars = {
    EFFECT_SS_DUST,
    EffectSsDust_Init,
};

static EffectSsUpdateFunc sUpdateFuncs[] = {
    EffectSsDust_Update,
    EffectSsBlast_UpdateFire,
};

u32 EffectSsDust_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    s32 randColorOffset;
    EffectSsDustInitParams* initParams = (EffectSsDustInitParams*)initParamsx;

    Math_Vec3f_Copy(&self->pos, &initParams->pos);
    Math_Vec3f_Copy(&self->velocity, &initParams->velocity);
    Math_Vec3f_Copy(&self->accel, &initParams->accel);
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffDustDL);
    self->life = initParams->life;
    self->update = sUpdateFuncs[initParams->updateMode];
    self->draw = EffectSsDust_Draw;

    if (initParams->drawFlags & 4) {
        randColorOffset = Rand_ZeroOne() * 20.0f - 10.0f;
        self->rPrimColorR = initParams->primColor.r + randColorOffset;
        self->rPrimColorG = initParams->primColor.g + randColorOffset;
        self->rPrimColorB = initParams->primColor.b + randColorOffset;
        self->rEnvColorR = initParams->envColor.r + randColorOffset;
        self->rEnvColorG = initParams->envColor.g + randColorOffset;
        self->rEnvColorB = initParams->envColor.b + randColorOffset;
    } else {
        self->rPrimColorR = initParams->primColor.r;
        self->rPrimColorG = initParams->primColor.g;
        self->rPrimColorB = initParams->primColor.b;
        self->rEnvColorR = initParams->envColor.r;
        self->rEnvColorG = initParams->envColor.g;
        self->rEnvColorB = initParams->envColor.b;
    }

    self->rPrimColorA = initParams->primColor.a;
    self->rEnvColorA = initParams->envColor.a;
    self->rTexIdx = 0;
    self->rScale = initParams->scale;
    self->rScaleStep = initParams->scaleStep;
    self->rLifespan = initParams->life;
    self->rDrawFlags = initParams->drawFlags;

    return 1;
}

void EffectSsDust_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    static void* dustTextures[] = {
        gDust1Tex, gDust2Tex, gDust3Tex, gDust4Tex, gDust5Tex, gDust6Tex, gDust7Tex, gDust8Tex,
    };
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    MtxF mfTrans11DA0;
    s32 pad;
    Mtx* mtx;
    f32 scale;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_dust.c", 321);

    scale = self->rScale * 0.0025f;
    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    SkinMatrix_SetScale(&mfScale, scale, scale, 1.0f);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &globalCtx->mf_11DA0, &mfTrans11DA0);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0, &mfScale, &mfResult);
    gSPMatrix(POLY_XLU_DISP++, &gMtxClear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPPipeSync(POLY_XLU_DISP++);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(dustTextures[self->rTexIdx]));
        POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);
        gDPPipeSync(POLY_XLU_DISP++);

        if (self->rDrawFlags & 1) {
            gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0,
                              COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED);
            gDPSetRenderMode(POLY_XLU_DISP++, G_RM_FOG_SHADE_A, G_RM_ZB_CLD_SURF2);
            gSPSetGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);
        } else if (self->rDrawFlags & 2) {
            gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_ZB_CLD_SURF2);
            gSPClearGeometryMode(POLY_XLU_DISP++, G_FOG | G_LIGHTING);
        } else {
            gSPClearGeometryMode(POLY_XLU_DISP++, G_LIGHTING);
        }

        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rEnvColorA);
        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_dust.c", 389);
}

void EffectSsDust_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->accel.x = (Rand_ZeroOne() * 0.4f) - 0.2f;
    self->accel.z = (Rand_ZeroOne() * 0.4f) - 0.2f;

    if ((self->life <= self->rLifespan) && (self->life >= (self->rLifespan - 7))) {
        if (self->rLifespan >= 5) {
            self->rTexIdx = self->rLifespan - self->life;
        } else {
            self->rTexIdx = ((self->rLifespan - self->life) * (8 / self->rLifespan));
        }
    } else {
        self->rTexIdx = 7;
    }

    self->rScale += self->rScaleStep;
}

// self update mode is unused in the original game
void EffectSsBlast_UpdateFire(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->accel.x = (Rand_ZeroOne() * 0.4f) - 0.2f;
    self->accel.z = (Rand_ZeroOne() * 0.4f) - 0.2f;

    switch (self->rTexIdx) {
        case 0:
            self->rPrimColorR = 255;
            self->rPrimColorG = 150;
            self->rPrimColorB = 0;
            self->rEnvColorR = 150;
            self->rEnvColorG = 50;
            self->rEnvColorB = 0;
            break;
        case 1:
            self->rPrimColorR = 200;
            self->rPrimColorG = 50;
            self->rPrimColorB = 0;
            self->rEnvColorR = 100;
            self->rEnvColorG = 0;
            self->rEnvColorB = 0;
            break;
        case 2:
            self->rPrimColorR = 50;
            self->rPrimColorG = 0;
            self->rPrimColorB = 0;
            self->rEnvColorR = 0;
            self->rEnvColorG = 0;
            self->rEnvColorB = 0;
            break;
        case 3:
            self->rPrimColorR = 50;
            self->rEnvColorR = self->rPrimColorG = self->rEnvColorG = self->rPrimColorB = self->rEnvColorB = 0;
            break;
    }

    if (self->rTexIdx < 7) {
        self->rTexIdx++;
    }

    self->rScale += self->rScaleStep;
}
