/*
 * File: z_eff_ss_g_magma2.c
 * Overlay: ovl_Effect_Ss_G_Magma2
 * Description:
 */

#include "z_eff_ss_g_magma2.h"
#include "objects/object_kingdodongo/object_kingdodongo.h"

#define rPrimColorR regs[0]
#define rPrimColorG regs[1]
#define rPrimColorA regs[2]
#define rEnvColorR regs[3]
#define rEnvColorG regs[4]
#define rEnvColorA regs[5]
#define rTexIdx regs[6]
#define rTimer regs[7]
#define rUpdateRate regs[8]
#define rDrawMode regs[9]
#define rObjBankIdx regs[10]
#define rScale regs[11]

u32 EffectSsGMagma2_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsGMagma2_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsGMagma2_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

static void* sTextures[] = {
    0x0602E4E0, 0x0602E8E0, 0x0602ECE0, 0x0602F0E0, 0x0602F4E0, 0x0602F8E0, 0x0602FCE0,
    0x060300E0, 0x060304E0, 0x060308E0, 0x060308E0, 0x060308E0, 0x060308E0,
};

EffectSsInit Effect_Ss_G_Magma2_InitVars = {
    EFFECT_SS_G_MAGMA2,
    EffectSsGMagma2_Init,
};

u32 EffectSsGMagma2_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    s32 objBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_KINGDODONGO);
    s32 pad;

    if ((objBankIndex >= 0) && Object_IsLoaded(&globalCtx->objectCtx, objBankIndex)) {
        Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
        EffectSsGMagma2InitParams* initParams = (EffectSsGMagma2InitParams*)initParamsx;

        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[objBankIndex].segment);
        self->rObjBankIdx = objBankIndex;
        self->pos = initParams->pos;
        self->velocity = zeroVec;
        self->accel = zeroVec;
        self->life = 100;
        self->draw = EffectSsGMagma2_Draw;
        self->update = EffectSsGMagma2_Update;
        self->gfx = SEGMENTED_TO_VIRTUAL(object_kingdodongo_DL_025A90);
        self->rTexIdx = 0;
        self->rDrawMode = initParams->drawMode;
        self->rUpdateRate = initParams->updateRate;
        self->rScale = initParams->scale;
        self->rPrimColorR = initParams->primColor.r;
        self->rPrimColorG = initParams->primColor.g;
        self->rPrimColorA = initParams->primColor.a;
        self->rEnvColorR = initParams->envColor.r;
        self->rEnvColorG = initParams->envColor.g;
        self->rEnvColorA = initParams->envColor.a;

        return 1;
    }

    return 0;
}

void EffectSsGMagma2_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad;
    f32 scale;
    void* object;

    scale = self->rScale / 100.0f;
    object = globalCtx->objectCtx.status[self->rObjBankIdx].segment;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_g_magma2.c", 261);

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSegments[6] = VIRTUAL_TO_PHYSICAL(object);
    gSPSegment(POLY_XLU_DISP++, 0x06, object);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_ss_g_magma2.c", 282),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (self->rDrawMode == 0) {
        POLY_XLU_DISP = Gfx_CallSetupDL(gfxCtx->polyXlu.p, 0x3D);
    } else {
        POLY_XLU_DISP = Gfx_CallSetupDL(gfxCtx->polyXlu.p, 0);
    }

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, 0, self->rPrimColorA);
    gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, 0, self->rEnvColorA);
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sTextures[self->rTexIdx]));
    gSPDisplayList(POLY_XLU_DISP++, self->gfx);

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_g_magma2.c", 311);
}

void EffectSsGMagma2_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->rTimer += self->rUpdateRate;

    if (self->rTimer >= 10) {
        self->rTimer -= 10;
        self->rTexIdx++;

        if (self->rTexIdx >= 10) {
            self->life = 0;
        }

        if (self->rDrawMode == 0) {
            self->rPrimColorG -= 26;

            if (self->rPrimColorG <= 0) {
                self->rPrimColorG = 0;
            }

            self->rEnvColorR -= 26;

            if (self->rEnvColorR <= 0) {
                self->rEnvColorR = 0;
            }
        }
    }
}
