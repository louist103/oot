/*
 * File: z_eff_ss_dead_ds.c
 * Overlay: ovl_Effect_Ss_Dead_Ds
 * Description: Burn mark on the floor
 */

#include "z_eff_ss_dead_ds.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rScale regs[0]
#define rTimer regs[1]
#define rRoll regs[2]
#define rPitch regs[3]
#define rYaw regs[4]
#define rAlpha regs[5]
#define rScaleStep regs[9]
#define rAlphaStep regs[10]
#define rHalfOfLife regs[11]

u32 EffectSsDeadDs_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsDeadDs_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsDeadDs_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Dead_Ds_InitVars = {
    EFFECT_SS_DEAD_DS,
    EffectSsDeadDs_Init,
};

u32 EffectSsDeadDs_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsDeadDsInitParams* initParams = (EffectSsDeadDsInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->life = initParams->life;
    self->rScaleStep = initParams->scaleStep;
    self->rHalfOfLife = initParams->life / 2;
    self->rAlphaStep = initParams->alpha / self->rHalfOfLife;
    self->draw = EffectSsDeadDs_Draw;
    self->update = EffectSsDeadDs_Update;
    self->rScale = initParams->scale;
    self->rAlpha = initParams->alpha;
    self->rTimer = 0;

    return 1;
}

void EffectSsDeadDs_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s32 pad;
    f32 scale;
    s32 pad1;
    s32 pad2;
    MtxF mf;
    f32 temp;
    Vec3f pos;
    CollisionPoly* floorPoly;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_eff_ss_dead_ds.c", 157);

    scale = self->rScale * 0.01f;
    func_80094BC4(globalCtx->state.gfxCtx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, self->rAlpha);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 0);
    pos = self->pos;

    if (self->rTimer == 0) {
        Vec3s rpy;
        Vec3f sp44;

        sp44.x = pos.x - self->velocity.x;
        sp44.y = pos.y - self->velocity.y;
        sp44.z = pos.z - self->velocity.z;

        if (BgCheck_EntitySphVsWall1(&globalCtx->colCtx, &self->pos, &pos, &sp44, 1.5f, &floorPoly, 1.0f)) {
            func_80038A28(floorPoly, self->pos.x, self->pos.y, self->pos.z, &mf);
            Matrix_Put(&mf);
        } else {
            pos.y++;
            temp = BgCheck_EntityRaycastFloor1(&globalCtx->colCtx, &floorPoly, &pos);

            if (floorPoly != NULL) {
                func_80038A28(floorPoly, self->pos.x, temp + 1.5f, self->pos.z, &mf);
                Matrix_Put(&mf);
            } else {
                Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
                Matrix_Get(&mf);
            }
        }

        Matrix_MtxFToZYXRotS(&mf, &rpy, 0);
        self->rRoll = rpy.x;
        self->rPitch = rpy.y;
        self->rYaw = rpy.z;
        self->pos.y = mf.yw;
        self->rTimer++;
    }

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->rRoll, self->rPitch, self->rYaw, MTXMODE_APPLY);
    Matrix_RotateX(1.57f, MTXMODE_APPLY);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_eff_ss_dead_ds.c", 246),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetCombineLERP(POLY_XLU_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
                      PRIMITIVE, 0);
    gSPDisplayList(POLY_XLU_DISP++, gLensFlareCircleDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_eff_ss_dead_ds.c", 255);
}

void EffectSsDeadDs_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    if (self->life < self->rHalfOfLife) {

        self->rScale += self->rScaleStep;
        if (self->rScale < 0) {
            self->rScale = 0;
        }

        self->rAlpha -= self->rAlphaStep;
        if (self->rAlpha < 0) {
            self->rAlpha = 0;
        }
    }
}
