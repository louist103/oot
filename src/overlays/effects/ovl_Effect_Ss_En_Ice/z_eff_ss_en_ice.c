/*
 * File: z_eff_ss_en_ice.c
 * Overlay: ovl_Effect_Ss_En_Ice
 * Description: Ice clumps
 */

#include "z_eff_ss_en_ice.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rLifespan regs[0]
#define rYaw regs[1]
#define rPitch regs[2]
#define rRotSpeed regs[3]
#define rPrimColorR regs[4]
#define rPrimColorG regs[5]
#define rPrimColorB regs[6]
#define rPrimColorA regs[7]
#define rEnvColorR regs[8]
#define rEnvColorG regs[9]
#define rEnvColorB regs[10]
#define rAlphaMode regs[11]
#define rScale regs[12]

u32 EffectSsEnIce_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsEnIce_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsEnIce_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsEnIce_UpdateFlying(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_En_Ice_InitVars = {
    EFFECT_SS_EN_ICE,
    EffectSsEnIce_Init,
};

u32 EffectSsEnIce_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsEnIceInitParams* initParams = (EffectSsEnIceInitParams*)initParamsx;

    if (initParams->type == 0) {
        Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

        self->pos = initParams->pos;
        self->vec.x = self->pos.x - initParams->actor->world.pos.x;
        self->vec.y = self->pos.y - initParams->actor->world.pos.y;
        self->vec.z = self->pos.z - initParams->actor->world.pos.z;
        self->velocity = zeroVec;
        self->accel = zeroVec;
        self->life = 10;
        self->actor = initParams->actor;
        self->draw = EffectSsEnIce_Draw;
        self->update = EffectSsEnIce_UpdateFlying;
        self->rScale = initParams->scale * 100.0f;
        self->rPrimColorR = initParams->primColor.r;
        self->rPrimColorG = initParams->primColor.g;
        self->rPrimColorB = initParams->primColor.b;
        self->rPrimColorA = initParams->primColor.a;
        self->rEnvColorR = initParams->envColor.r;
        self->rEnvColorG = initParams->envColor.g;
        self->rEnvColorB = initParams->envColor.b;
        self->rAlphaMode = 1;
        self->rPitch = Rand_CenteredFloat(65536.0f);
    } else if (initParams->type == 1) {
        self->pos = initParams->pos;
        self->vec = initParams->pos;
        self->velocity = initParams->velocity;
        self->accel = initParams->accel;
        self->life = initParams->life;
        self->draw = EffectSsEnIce_Draw;
        self->update = EffectSsEnIce_Update;
        self->rLifespan = initParams->life;
        self->rScale = initParams->scale * 100.0f;
        self->rYaw = Math_Atan2S(initParams->velocity.z, initParams->velocity.x);
        self->rPitch = 0;
        self->rPrimColorR = initParams->primColor.r;
        self->rPrimColorG = initParams->primColor.g;
        self->rPrimColorB = initParams->primColor.b;
        self->rPrimColorA = initParams->primColor.a;
        self->rEnvColorR = initParams->envColor.r;
        self->rEnvColorG = initParams->envColor.g;
        self->rEnvColorB = initParams->envColor.b;
        self->rAlphaMode = 0;
    } else {
        osSyncPrintf("Effect_Ss_En_Ice_ct():pid->mode_swがエラーです。\n");
        return 0;
    }

    return 1;
}

void EffectSsEnIce_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad;
    f32 scale;
    Vec3f hiliteLightDir;
    u32 gameplayFrames;
    f32 alpha;

    scale = self->rScale * 0.01f;
    gameplayFrames = globalCtx->gameplayFrames;

    OPEN_DISPS(gfxCtx, "../z_eff_en_ice.c", 235);

    if (self->rAlphaMode != 0) {
        alpha = self->life * 12;
    } else {
        if ((self->rLifespan > 0) && (self->life < (self->rLifespan >> 1))) {
            alpha = ((self->life * 2.0f) / self->rLifespan);
            alpha *= 255.0f;
        } else {
            alpha = 255.0f;
        }
    }

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_RotateY(self->rYaw * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_RotateX(self->rPitch * (M_PI / 0x8000), MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_en_ice.c", 261),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    hiliteLightDir.x = 89.8f;
    hiliteLightDir.y = 0.0f;
    hiliteLightDir.z = 89.8f;

    func_80093D84(globalCtx->state.gfxCtx);
    func_8002EB44(&self->pos, &globalCtx->view.eye, &hiliteLightDir, globalCtx->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, gameplayFrames & 0xFF, 0x20, 0x10, 1, 0,
                                (gameplayFrames * 2) & 0xFF, 0x40, 0x20));
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB,
                    self->rPrimColorA);
    gDPSetEnvColor(POLY_XLU_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, (u32)alpha);
    gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment2DL);

    CLOSE_DISPS(gfxCtx, "../z_eff_en_ice.c", 294);
}

void EffectSsEnIce_UpdateFlying(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s16 rand;

    if ((self->actor != NULL) && (self->actor->update != NULL)) {
        if ((self->life >= 9) && (self->actor->colorFilterTimer != 0) && (!(self->actor->colorFilterParams & 0xC000))) {
            self->pos.x = self->actor->world.pos.x + self->vec.x;
            self->pos.y = self->actor->world.pos.y + self->vec.y;
            self->pos.z = self->actor->world.pos.z + self->vec.z;
            self->life++;
        } else if (self->life == 9) {
            self->accel.x = Math_SinS(Math_Vec3f_Yaw(&self->actor->world.pos, &self->pos)) * (Rand_ZeroOne() + 1.0f);
            self->accel.z = Math_CosS(Math_Vec3f_Yaw(&self->actor->world.pos, &self->pos)) * (Rand_ZeroOne() + 1.0f);
            self->accel.y = -1.5f;
            self->velocity.y = 5.0f;
        }
    } else {
        if (self->life >= 9) {
            rand = Rand_CenteredFloat(65535.0f);
            self->accel.x = Math_SinS(rand) * (Rand_ZeroOne() + 1.0f);
            self->accel.z = Math_CosS(rand) * (Rand_ZeroOne() + 1.0f);
            self->life = 8;
            self->accel.y = -1.5f;
            self->velocity.y = 5.0f;
        }
    }
}

void EffectSsEnIce_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    self->rPitch += self->rRotSpeed; // rRotSpeed is not initialized so self does nothing
}
