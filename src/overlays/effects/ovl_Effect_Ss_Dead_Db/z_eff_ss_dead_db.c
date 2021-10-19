/*
 * File: z_eff_ss_dead_db.c
 * Overlay: ovl_Effect_Ss_Dead_Db
 * Description: Flames and sound used when an enemy dies
 */

#include "z_eff_ss_dead_db.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rScale regs[0]
#define rTextIdx regs[1]
#define rPrimColorR regs[2]
#define rPrimColorG regs[3]
#define rPrimColorB regs[4]
#define rPrimColorA regs[5]
#define rEnvColorR regs[6]
#define rEnvColorG regs[7]
#define rEnvColorB regs[8]
#define rScaleStep regs[9]
#define rPlaySound regs[10]
#define rReg11 regs[11]

u32 EffectSsDeadDb_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsDeadDb_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsDeadDb_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Dead_Db_InitVars = {
    EFFECT_SS_DEAD_DB,
    EffectSsDeadDb_Init,
};

u32 EffectSsDeadDb_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsDeadDbInitParams* initParams = (EffectSsDeadDbInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->gfx = SEGMENTED_TO_VIRTUAL(gEffEnemyDeathFlameDL);
    self->life = initParams->unk_34;
    self->flags = 4;
    self->rScaleStep = initParams->scaleStep;
    self->rReg11 = initParams->unk_34;
    self->draw = EffectSsDeadDb_Draw;
    self->update = EffectSsDeadDb_Update;
    self->rScale = initParams->scale;
    self->rTextIdx = 0;
    self->rPlaySound = initParams->playSound;
    self->rPrimColorR = initParams->primColor.r;
    self->rPrimColorG = initParams->primColor.g;
    self->rPrimColorB = initParams->primColor.b;
    self->rPrimColorA = initParams->primColor.a;
    self->rEnvColorR = initParams->envColor.r;
    self->rEnvColorG = initParams->envColor.g;
    self->rEnvColorB = initParams->envColor.b;

    return 1;
}

static void* sTextures[] = {
    gEffEnemyDeathFlame1Tex, gEffEnemyDeathFlame2Tex,  gEffEnemyDeathFlame3Tex, gEffEnemyDeathFlame4Tex,
    gEffEnemyDeathFlame5Tex, gEffEnemyDeathFlame6Tex,  gEffEnemyDeathFlame7Tex, gEffEnemyDeathFlame8Tex,
    gEffEnemyDeathFlame9Tex, gEffEnemyDeathFlame10Tex,
};

void EffectSsDeadDb_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    MtxF mfTrans;
    MtxF mfScale;
    MtxF mfResult;
    Mtx* mtx;
    f32 scale;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_dead_db.c", 201);

    scale = self->rScale * 0.01f;

    SkinMatrix_SetTranslate(&mfTrans, self->pos.x, self->pos.y, self->pos.z);
    SkinMatrix_SetScale(&mfScale, scale, scale, scale);
    SkinMatrix_MtxFMtxFMult(&mfTrans, &mfScale, &mfResult);

    mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &mfResult);

    if (mtx != NULL) {
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_80094BC4(gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, 0);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                        self->rPrimColorA);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sTextures[self->rTextIdx]));
        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_dead_db.c", 247);
}

void EffectSsDeadDb_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    f32 w;
    f32 pad;

    self->rTextIdx = (f32)((self->rReg11 - self->life) * 9) / self->rReg11;
    self->rScale += self->rScaleStep;

    self->rPrimColorR -= 10;
    if (self->rPrimColorR < 0) {
        self->rPrimColorR = 0;
    }

    self->rPrimColorG -= 10;
    if (self->rPrimColorG < 0) {
        self->rPrimColorG = 0;
    }

    self->rPrimColorB -= 10;
    if (self->rPrimColorB < 0) {
        self->rPrimColorB = 0;
    }

    self->rEnvColorR -= 10;
    if (self->rEnvColorR < 0) {
        self->rEnvColorR = 0;
    }

    self->rEnvColorG -= 10;
    if (self->rEnvColorG < 0) {
        self->rEnvColorG = 0;
    }

    self->rEnvColorB -= 10;
    if (self->rEnvColorB < 0) {
        self->rEnvColorB = 0;
    }

    if (self->rPlaySound && (self->rTextIdx == 1)) {
        SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, &self->pos, &self->vec, &w);
        Audio_PlaySoundGeneral(NA_SE_EN_EXTINCT, &self->vec, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
}
