/*
 * File: z_eff_ss_g_spk.c
 * Overlay: ovl_Effect_Ss_G_Spk
 * Description: Sparks
 */

#include "z_eff_ss_g_spk.h"
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
#define rScaleStep regs[10]

u32 EffectSsGSpk_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsGSpk_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsGSpk_UpdateNoAccel(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsGSpk_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_G_Spk_InitVars = {
    EFFECT_SS_G_SPK,
    EffectSsGSpk_Init,
};

u32 EffectSsGSpk_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsGSpkInitParams* initParams = (EffectSsGSpkInitParams*)initParamsx;

    Math_Vec3f_Copy(&self->pos, &initParams->pos);
    Math_Vec3f_Copy(&self->velocity, &initParams->velocity);
    Math_Vec3f_Copy(&self->accel, &initParams->accel);
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffSparkDL);

    if (initParams->updateMode == 0) {
        self->life = 10;
        self->vec.x = initParams->pos.x - initParams->actor->world.pos.x;
        self->vec.y = initParams->pos.y - initParams->actor->world.pos.y;
        self->vec.z = initParams->pos.z - initParams->actor->world.pos.z;
        self->update = EffectSsGSpk_Update;
    } else {
        self->life = 5;
        self->update = EffectSsGSpk_UpdateNoAccel;
    }

    self->draw = EffectSsGSpk_Draw;
    self->rPrimColorR = initParams->primColor.r;
    self->rPrimColorG = initParams->primColor.g;
    self->rPrimColorB = initParams->primColor.b;
    self->rPrimColorA = initParams->primColor.a;
    self->rEnvColorR = initParams->envColor.r;
    self->rEnvColorG = initParams->envColor.g;
    self->rEnvColorB = initParams->envColor.b;
    self->rEnvColorA = initParams->envColor.a;
    self->rTexIdx = 0;
    self->rScale = initParams->scale;
    self->rScaleStep = initParams->scaleStep;
    self->actor = initParams->actor;

    return 1;
}

void EffectSsGSpk_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    static void* sparkTextures[] = {
        gEffSpark1Tex,
        gEffSpark2Tex,
        gEffSpark3Tex,
        gEffSpark4Tex,
    };
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    MtxF mfTrans11DA0;
    Mtx* mtx;
    f32 scale;
    s32 pad;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_g_spk.c", 208);

    scale = self->rScale * 0.0025f;
    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    SkinMatrix_SetScale(&mfScale, scale, scale, 1.0f);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &globalCtx->mf_11DA0, &mfTrans11DA0);
    SkinMatrix_MtxFMtxFMult(&mfTrans11DA0, &mfScale, &mfResult);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sparkTextures[self->rTexIdx]));
        func_80094BC4(gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rEnvColorA);
        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    }

    if (1) {}
    if (1) {}

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_g_spk.c", 255);
}

void EffectSsGSpk_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {

    self->accel.x = (Rand_ZeroOne() - 0.5f) * 3.0f;
    self->accel.z = (Rand_ZeroOne() - 0.5f) * 3.0f;

    if (self->actor != NULL) {
        if ((self->actor->category == ACTORCAT_EXPLOSIVE) && (self->actor->update != NULL)) {
            self->pos.x = self->actor->world.pos.x + self->vec.x;
            self->pos.y = self->actor->world.pos.y + self->vec.y;
            self->pos.z = self->actor->world.pos.z + self->vec.z;
        }
    }

    self->vec.x += self->accel.x;
    self->vec.z += self->accel.z;

    self->rTexIdx++;
    self->rTexIdx &= 3;
    self->rScale += self->rScaleStep;
}

// self update mode is unused in the original game
// with self update mode, the sparks dont move randomly in the xz plane, appearing to be on top of each other
void EffectSsGSpk_UpdateNoAccel(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    if (self->actor != NULL) {
        if ((self->actor->category == ACTORCAT_EXPLOSIVE) && (self->actor->update != NULL)) {
            self->pos.x += (Math_SinS(self->actor->world.rot.y) * self->actor->speedXZ);
            self->pos.z += (Math_CosS(self->actor->world.rot.y) * self->actor->speedXZ);
        }
    }

    self->rTexIdx++;
    self->rTexIdx &= 3;
    self->rScale += self->rScaleStep;
}
