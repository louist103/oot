/*
 * File: z_en_bdfire.c
 * Overlay: ovl_En_Bdfire
 * Description: King Dodongo's Fire
 */

#include "z_en_bdfire.h"

#define FLAGS 0x00000030

#define THIS ((EnBdfire*)thisx)

void EnBdfire_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBdfire_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBdfire_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBdfire_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809BC8C8(EnBdfire* this, GlobalContext* globalCtx);
void func_809BC2A4(EnBdfire* this, GlobalContext* globalCtx);
void func_809BC598(EnBdfire* this, GlobalContext* globalCtx);

extern Gfx* D_0601D950[];

const ActorInit En_Bdfire_InitVars = {
    0,
    ACTORTYPE_ENEMY,
    FLAGS,
    OBJECT_KINGDODONGO,
    sizeof(EnBdfire),
    (ActorFunc)EnBdfire_Init,
    (ActorFunc)EnBdfire_Destroy,
    (ActorFunc)EnBdfire_Update,
    (ActorFunc)EnBdfire_Draw,
};

Gfx* D_809BCB10[] = { 0x060264E0, 0x060274E0, 0x060284E0, 0x060294E0, 0x0602A4E0, 0x0602B4E0, 0x0602C4E0, 0x0602D4E0 };

void func_809BC030(EnBdfire* this, EnBdfireActionFunc actionFunc) {
    this->actionFunc2 = actionFunc;
}

void func_809BC038(EnBdfire* this, EnBdfireActionFunc actionFunc) { // Set Draw func?
    this->actionFunc = actionFunc;
}

void EnBdfire_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBdfire* this = THIS;
    s32 pad;

    Actor_SetScale(this, 0.6f);
    func_809BC038(this, func_809BC8C8);
    if (this->actor.params < 0) {
        func_809BC030(this, func_809BC2A4);
        this->actor.scale.x = 2.8f;
        this->unk_154 = 90;
        Lights_PointNoGlowSetInfo(&this->lightInfoNoGlow, this->actor.posRot.pos.x, this->actor.posRot.pos.y,
                                  this->actor.posRot.pos.z, 255, 255, 255, 300);
        this->lightInfo2 = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &this->lightInfoNoGlow);
        return;
    }
    func_809BC030(this, func_809BC598);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFunc_Circle, 0.0f);
    this->actor.speedXZ = 30.0f;
    this->unk_154 = (25 - (s32)(this->actor.params * 0.8f));
    if (this->unk_154 < 0) {
        this->unk_154 = 0;
    }
    this->unk_188 = 4.2000003f - (this->actor.params * 0.25f * 0.6f);

    if (this->unk_188 < 0.90000004f) {
        this->unk_188 = 0.90000004f;
    }
    this->unk_18C = 255.0f - (this->actor.params * 10.0f);
    if (this->unk_18C < 20.0f) {
        this->unk_18C = 20.0f;
    }
    this->unk_156 = (Math_Rand_ZeroOne() * 8.0f);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Bdfire/EnBdfire_Destroy.s")

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Bdfire/func_809BC2A4.s")
void func_809BC2A4(EnBdfire* this, GlobalContext* globalCtx) {
    BossDodongo* kingDodongo;
    s32 temp;

    kingDodongo = (BossDodongo*)this->actor.parent;
    this->actor.posRot.pos.x = kingDodongo->unk_3F8.x;
    this->actor.posRot.pos.y = kingDodongo->unk_3F8.y;
    this->actor.posRot.pos.z = kingDodongo->unk_3F8.z;
    if (kingDodongo->unk_1E2 == 0) {
        Math_SmoothScaleMaxMinF(&this->actor.scale.x, 0.0f, 1.0f, 0.6f, 0.0f);
        if (Math_SmoothScaleMaxMinF(&this->unk_18C, 0.0f, 1.0f, 20.0f, 0.0f) == 0.0f) {
            Actor_Kill(&this->actor);
            return;
        }
    } else {
        if (this->unk_154 < 0x46) {
            Math_SmoothScaleMaxMinF(&this->unk_18C, 128.0f, 0.1f, 1.5f, 0.0f);
            Math_SmoothScaleMaxMinF(&this->unk_190, 255.0f, 1.0f, 3.8249998f, 0.0f);
            Math_SmoothScaleMaxMinF(&this->unk_194, 100.0f, 1.0f, 1.5f, 0.0f);
        }
        if (this->unk_154 == 0) {
            temp = 0;
        } else {
            this->unk_154--;
            temp = this->unk_154;
        }
        if (temp == 0) {
            Math_SmoothScaleMaxMinF(&this->actor.scale.x, 0.0f, 1.0f, 0.3f, 0.0f);
            Math_SmoothScaleMaxMinF(&this->unk_190, 0.0f, 1.0f, 25.5f, 0.0f);
            Math_SmoothScaleMaxMinF(&this->unk_194, 0.0f, 1.0f, 10.0f, 0.0f);
            if (Math_SmoothScaleMaxMinF(&this->unk_18C, 0.0f, 1.0f, 10.0f, 0.0f) == 0.0f) {
                Actor_Kill(&this->actor);
            }
        }
        Actor_SetScale(&this->actor, this->actor.scale.x);
        Lights_PointSetColorAndRadius(&this->lightInfoNoGlow, this->unk_190, this->unk_194, 0, 0x12C);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Bdfire/func_809BC598.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Bdfire/EnBdfire_Update.s")

void func_809BC8C8(EnBdfire* this, GlobalContext* globalCtx) {
    s16 temp;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bdfire.c", 612);
    temp = this->unk_156 & 7;
    func_800D1FD4(&globalCtx->mf_11DA0);
    func_80094BC4(globalCtx->state.gfxCtx);
    POLY_XLU_DISP = func_80094968(POLY_XLU_DISP);
    gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0,
                      ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0,
                      ENVIRONMENT);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 100, (s8)this->unk_18C);
    gDPSetEnvColor(POLY_XLU_DISP++, 0xC8, 0x00, 0x00, 0x00);
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_809BCB10[temp]));
    Matrix_Translate(0.0f, 11.0f, 0.0f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_bdfire.c", 0x287),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, D_0601D950);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bdfire.c", 0x28B);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Bdfire/EnBdfire_Draw.s")
