/*
 * File: z_eff_ss_stick.c
 * Overlay: ovl_Effect_Ss_Stick
 * Description: Broken stick as child, broken sword as adult
 */

#include "z_eff_ss_stick.h"

#define rObjBankIdx regs[0]
#define rYaw regs[1]

u32 EffectSsStick_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsStick_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsStick_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Stick_InitVars = {
    EFFECT_SS_STICK,
    EffectSsStick_Init,
};

typedef struct {
    /* 0x00 */ s16 objectID;
    /* 0x04 */ Gfx* displayList;
} StickDrawInfo;

u32 EffectSsStick_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    StickDrawInfo drawInfo[] = {
        { OBJECT_LINK_BOY, 0x0602BA38 },   // adult, broken sword
        { OBJECT_LINK_CHILD, 0x06006CC0 }, // child, broken stick
    };
    StickDrawInfo* ageInfoEntry = gSaveContext.linkAge + drawInfo;
    EffectSsStickInitParams* initParams = (EffectSsStickInitParams*)initParamsx;

    self->rObjBankIdx = Object_GetIndex(&globalCtx->objectCtx, ageInfoEntry->objectID);
    self->gfx = ageInfoEntry->displayList;
    self->vec = self->pos = initParams->pos;
    self->rYaw = initParams->yaw;
    self->velocity.x = Math_SinS(initParams->yaw) * 6.0f;
    self->velocity.z = Math_CosS(initParams->yaw) * 6.0f;
    self->life = 20;
    self->draw = EffectSsStick_Draw;
    self->update = EffectSsStick_Update;
    self->velocity.y = 26.0f;
    self->accel.y = -4.0f;

    return 1;
}

void EffectSsStick_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_stick.c", 153);

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);

    if (!LINK_IS_ADULT) {
        Matrix_Scale(0.01f, 0.0025f, 0.01f, MTXMODE_APPLY);
        Matrix_RotateRPY(0, self->rYaw, 0, MTXMODE_APPLY);
    } else {
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
        Matrix_RotateRPY(0, self->rYaw, globalCtx->state.frames * 10000, MTXMODE_APPLY);
    }

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_ss_stick.c", 176),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80093D18(gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->rObjBankIdx].segment);
    gSPSegment(POLY_OPA_DISP++, 0x0C, gCullBackDList);
    gSPDisplayList(POLY_OPA_DISP++, self->gfx);

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_stick.c", 188);
}

void EffectSsStick_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
}
