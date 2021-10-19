/*
 * File: z_eff_ss_en_fire.c
 * Overlay: ovl_Effect_Ss_En_Fire
 * Description:
 */

#include "z_eff_ss_en_fire.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rScaleMax regs[0]
#define rScale regs[1]
#define rLifespan regs[2]
#define rUnused regs[3]
#define rPitch regs[4]
#define rYaw regs[5]
#define rReg6 regs[6]
#define rBodyPart regs[7]
#define rFlags regs[8]
#define rScroll regs[9]

u32 EffectSsEnFire_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsEnFire_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsEnFire_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_En_Fire_InitVars = {
    EFFECT_SS_EN_FIRE,
    EffectSsEnFire_Init,
};

u32 EffectSsEnFire_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsEnFireInitParams* initParams = (EffectSsEnFireInitParams*)initParamsx;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    self->pos = initParams->pos;
    self->velocity = self->accel = zeroVec;
    self->life = 20;
    self->rLifespan = self->life;
    self->actor = initParams->actor;
    self->rScroll = Rand_ZeroOne() * 20.0f;
    self->draw = EffectSsEnFire_Draw;
    self->update = EffectSsEnFire_Update;
    self->rUnused = -15;

    if (initParams->bodyPart < 0) {
        self->rYaw = Math_Vec3f_Yaw(&initParams->actor->world.pos, &initParams->pos) - initParams->actor->shape.rot.y;
        self->rPitch =
            Math_Vec3f_Pitch(&initParams->actor->world.pos, &initParams->pos) - initParams->actor->shape.rot.x;
        self->vec.z = Math_Vec3f_DistXYZ(&initParams->pos, &initParams->actor->world.pos);
    }

    self->rScaleMax = initParams->scale;

    if ((initParams->unk_12 & 0x8000) != 0) {
        self->rScale = initParams->scale;
    } else {
        self->rScale = 0;
    }

    self->rReg6 = initParams->unk_12 & 0x7FFF;
    self->rBodyPart = initParams->bodyPart;
    self->rFlags = initParams->flags;

    return 1;
}

void EffectSsEnFire_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    f32 scale;
    s16 camYaw;
    s32 pad[3];
    s16 redGreen;

    OPEN_DISPS(gfxCtx, "../z_eff_en_fire.c", 169);

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    camYaw = (Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000);
    Matrix_RotateY(camYaw * (M_PI / 0x8000), MTXMODE_APPLY);

    scale = Math_SinS(self->life * 0x333) * (self->rScale * 0.00005f);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_eff_en_fire.c", 180),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    redGreen = self->life - 5;

    if (redGreen < 0) {
        redGreen = 0;
    }

    func_80093D84(globalCtx->state.gfxCtx);
    gDPSetEnvColor(POLY_XLU_DISP++, redGreen * 12.7f, 0, 0, 0);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x0, 0x80, redGreen * 12.7f, redGreen * 12.7f, 0, 255);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0, (self->rScroll * -0x14) & 0x1FF,
                                0x20, 0x80));

    if (((self->rFlags & 0x7FFF) != 0) || (self->life < 18)) {
        gSPDisplayList(POLY_XLU_DISP++, gEffFire2DL);
    } else {
        gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_en_fire.c", 213);
}

typedef struct {
    /* 0x000 */ Actor actor;
    /* 0x14C */ Vec3f firePos[10];
} FireActorF;

typedef struct {
    /* 0x000 */ Actor actor;
    /* 0x14C */ Vec3s firePos[10];
} FireActorS;

void EffectSsEnFire_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {

    self->rScroll++;

    if (self->actor != NULL) {
        if (self->actor->colorFilterTimer >= 22) {
            self->life++;
        }
        if (self->actor->update != NULL) {
            Math_SmoothStepToS(&self->rScale, self->rScaleMax, 1, self->rScaleMax >> 3, 0);

            if (self->rBodyPart < 0) {
                Matrix_Translate(self->actor->world.pos.x, self->actor->world.pos.y, self->actor->world.pos.z,
                                 MTXMODE_NEW);
                Matrix_RotateY((self->rYaw + self->actor->shape.rot.y) * (M_PI / 0x8000), MTXMODE_APPLY);
                Matrix_RotateX((self->rPitch + self->actor->shape.rot.x) * (M_PI / 0x8000), MTXMODE_APPLY);
                Matrix_MultVec3f(&self->vec, &self->pos);
            } else {
                if ((self->rFlags & 0x8000)) {
                    self->pos.x = ((FireActorS*)self->actor)->firePos[self->rBodyPart].x;
                    self->pos.y = ((FireActorS*)self->actor)->firePos[self->rBodyPart].y;
                    self->pos.z = ((FireActorS*)self->actor)->firePos[self->rBodyPart].z;
                } else {
                    self->pos.x = ((FireActorF*)self->actor)->firePos[self->rBodyPart].x;
                    self->pos.y = ((FireActorF*)self->actor)->firePos[self->rBodyPart].y;
                    self->pos.z = ((FireActorF*)self->actor)->firePos[self->rBodyPart].z;
                }
            }
        } else if (self->rReg6 != 0) {
            self->life = 0;
        } else {
            self->actor = NULL;
        }
    }
}
