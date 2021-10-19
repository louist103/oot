/*
 * File: z_eff_ss_hahen.c
 * Overlay: ovl_Effect_Ss_Hahen
 * Description: Fragments
 */

#include "z_eff_ss_hahen.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rPitch regs[0]
#define rYaw regs[1]
#define rUnused regs[2]
#define rScale regs[3]
#define rObjId regs[4]
#define rObjBankIdx regs[5]
#define rMinLife regs[6]

u32 EffectSsHahen_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsHahen_DrawGray(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsHahen_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsHahen_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Hahen_InitVars = {
    EFFECT_SS_HAHEN,
    EffectSsHahen_Init,
};

void EffectSsHahen_CheckForObject(EffectSs* self, GlobalContext* globalCtx) {
    if (((self->rObjBankIdx = Object_GetIndex(&globalCtx->objectCtx, self->rObjId)) < 0) ||
        !Object_IsLoaded(&globalCtx->objectCtx, self->rObjBankIdx)) {
        self->life = -1;
        self->draw = NULL;
    }
}

u32 EffectSsHahen_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsHahenInitParams* initParams = (EffectSsHahenInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->life = 200;

    if (initParams->dList != NULL) {
        self->gfx = initParams->dList;
        self->rObjId = initParams->objId;
        EffectSsHahen_CheckForObject(self, globalCtx);
    } else {
        self->gfx = SEGMENTED_TO_VIRTUAL(gEffFragments1DL);
        self->rObjId = -1;
    }

    if ((self->rObjId == OBJECT_HAKA_OBJECTS) && (self->gfx == gEffFragments2DL)) {
        self->draw = EffectSsHahen_DrawGray;
    } else {
        self->draw = EffectSsHahen_Draw;
    }

    self->update = EffectSsHahen_Update;
    self->rUnused = initParams->unused;
    self->rScale = initParams->scale;
    self->rPitch = Rand_ZeroOne() * 314.0f;
    self->rYaw = Rand_ZeroOne() * 314.0f;
    self->rMinLife = 200 - initParams->life;

    return 1;
}

void EffectSsHahen_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad;
    f32 scale = self->rScale * 0.001f;

    OPEN_DISPS(gfxCtx, "../z_eff_hahen.c", 208);

    if (self->rObjId != -1) {
        gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->rObjBankIdx].segment);
    }

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_RotateY(self->rYaw * 0.01f, MTXMODE_APPLY);
    Matrix_RotateX(self->rPitch * 0.01f, MTXMODE_APPLY);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_hahen.c", 228),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, self->gfx);

    CLOSE_DISPS(gfxCtx, "../z_eff_hahen.c", 236);
}

// in the original game self function is hardcoded to be used only by the skull pots in Shadow Temple
void EffectSsHahen_DrawGray(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad;
    f32 scale = self->rScale * 0.001f;

    OPEN_DISPS(gfxCtx, "../z_eff_hahen.c", 253);

    if (self->rObjId != -1) {
        gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->rObjBankIdx].segment);
    }

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_RotateY(self->rYaw * 0.01f, MTXMODE_APPLY);
    Matrix_RotateX(self->rPitch * 0.01f, MTXMODE_APPLY);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_hahen.c", 271),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetCombineLERP(POLY_OPA_DISP++, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0,
                      PRIMITIVE, 0);
    gDPSetPrimColor(POLY_OPA_DISP++, 0x0, 0x01, 100, 100, 120, 255);
    gSPDisplayList(POLY_OPA_DISP++, self->gfx);

    CLOSE_DISPS(gfxCtx, "../z_eff_hahen.c", 288);
}

void EffectSsHahen_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    Player* player = GET_PLAYER(globalCtx);

    self->rPitch += 55;
    self->rYaw += 10;

    if ((self->pos.y <= player->actor.floorHeight) && (self->life < self->rMinLife)) {
        self->life = 0;
    }

    if (self->rObjId != -1) {
        EffectSsHahen_CheckForObject(self, globalCtx);
    }
}
