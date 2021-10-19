/*
 * File: z_eff_ss_extra.c
 * Overlay: ovl_Effect_Ss_Extra
 * Description: Minigame Score Points
 */

#include "z_eff_ss_extra.h"
#include "objects/object_yabusame_point/object_yabusame_point.h"

#define rObjBankIdx regs[0]
#define rTimer regs[1]
#define rScoreIdx regs[2]
#define rScale regs[3]

u32 EffectSsExtra_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsExtra_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsExtra_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

static s16 sScores[] = { 30, 60, 100 };

EffectSsInit Effect_Ss_Extra_InitVars = {
    EFFECT_SS_EXTRA,
    EffectSsExtra_Init,
};

u32 EffectSsExtra_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsExtraInitParams* initParams = (EffectSsExtraInitParams*)initParamsx;
    s32 pad;
    s32 objBankIndex;
    u32 oldSeg6;

    objBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_YABUSAME_POINT);

    if ((objBankIndex >= 0) && Object_IsLoaded(&globalCtx->objectCtx, objBankIndex)) {
        oldSeg6 = gSegments[6];
        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[objBankIndex].segment);
        self->pos = initParams->pos;
        self->velocity = initParams->velocity;
        self->accel = initParams->accel;
        self->draw = EffectSsExtra_Draw;
        self->update = EffectSsExtra_Update;
        self->life = 50;
        self->rScoreIdx = initParams->scoreIdx;
        self->rScale = initParams->scale;
        self->rTimer = 5;
        self->rObjBankIdx = objBankIndex;
        gSegments[6] = oldSeg6;

        return 1;
    }

    return 0;
}

static void* sTextures[] = { 0x06000000, 0x06000480, 0x06000900 };

void EffectSsExtra_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s32 pad;
    f32 scale = self->rScale / 100.0f;
    void* object = globalCtx->objectCtx.status[self->rObjBankIdx].segment;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_eff_ss_extra.c", 168);

    gSegments[6] = VIRTUAL_TO_PHYSICAL(object);
    gSPSegment(POLY_XLU_DISP++, 0x06, object);
    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    func_80093D84(globalCtx->state.gfxCtx);
    func_800D1FD4(&globalCtx->mf_11DA0);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_eff_ss_extra.c", 186),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sTextures[self->rScoreIdx]));
    gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(object_yabusame_point_DL_000DC0));

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_eff_ss_extra.c", 194);
}

void EffectSsExtra_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    if (self->rTimer != 0) {
        self->rTimer -= 1;
    } else {
        self->velocity.y = 0.0f;
    }

    if (self->rTimer == 1) {
        globalCtx->interfaceCtx.unk_23C = sScores[self->rScoreIdx];
    }
}
