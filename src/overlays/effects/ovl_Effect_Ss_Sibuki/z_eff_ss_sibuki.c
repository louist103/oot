/*
 * File: z_eff_ss_sibuki.c
 * Overlay: ovl_Effect_Ss_Sibuki
 * Description: Bubbles (only used by gohma and gohmalings in the original game)
 */

#include "z_eff_ss_sibuki.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define rPrimColorR regs[0]
#define rPrimColorG regs[1]
#define rPrimColorB regs[2]
#define rPrimColorA regs[3]
#define rEnvColorR regs[4]
#define rEnvColorG regs[5]
#define rEnvColorB regs[6]
#define rEnvColorA regs[7]
#define rMoveDelay regs[8]
#define rDirection regs[9]
#define rScale regs[10]

u32 EffectSsSibuki_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsSibuki_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsSibuki_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Sibuki_InitVars = {
    EFFECT_SS_SIBUKI,
    EffectSsSibuki_Init,
};

u32 EffectSsSibuki_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsSibukiInitParams* initParams = (EffectSsSibukiInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;

    if (KREG(2) != 0) {
        self->gfx = SEGMENTED_TO_VIRTUAL(&gEffBubble2Tex);
    } else {
        self->gfx = SEGMENTED_TO_VIRTUAL(&gEffBubble1Tex);
    }

    self->life = ((s16)((Rand_ZeroOne() * (500.0f + KREG(64))) * 0.01f)) + KREG(65) + 10;
    self->rMoveDelay = initParams->moveDelay + 1;
    self->draw = EffectSsSibuki_Draw;
    self->update = EffectSsSibuki_Update;
    self->rDirection = initParams->direction;
    self->rScale = initParams->scale;
    self->rPrimColorR = 100;
    self->rPrimColorG = 100;
    self->rPrimColorB = 100;
    self->rPrimColorA = 100;
    self->rEnvColorR = 255;
    self->rEnvColorG = 255;
    self->rEnvColorB = 255;
    self->rEnvColorA = 255;

    return 1;
}

void EffectSsSibuki_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    f32 scale = self->rScale / 100.0f;

    OPEN_DISPS(gfxCtx, "../z_eff_ss_sibuki.c", 163);

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_ss_sibuki.c", 176),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80093D18(gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, self->rPrimColorR, self->rPrimColorG, self->rPrimColorB, self->rPrimColorA);
    gDPSetEnvColor(POLY_OPA_DISP++, self->rEnvColorR, self->rEnvColorG, self->rEnvColorB, self->rEnvColorA);
    gSPSegment(POLY_OPA_DISP++, 0x08, self->gfx);
    gSPDisplayList(POLY_OPA_DISP++, SEGMENTED_TO_VIRTUAL(gEffBubbleDL));

    CLOSE_DISPS(gfxCtx, "../z_eff_ss_sibuki.c", 198);
}

void EffectSsSibuki_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s32 pad[3];
    f32 xzVelScale;
    s16 yaw;
    Player* player = GET_PLAYER(globalCtx);

    if (self->pos.y <= player->actor.floorHeight) {
        self->life = 0;
    }

    if (self->rMoveDelay != 0) {
        self->rMoveDelay--;

        if (self->rMoveDelay == 0) {
            yaw = Camera_GetInputDirYaw(Gameplay_GetCamera(globalCtx, 0));
            xzVelScale = ((200.0f + KREG(20)) * 0.01f) + ((0.1f * Rand_ZeroOne()) * (KREG(23) + 20.0f));

            if (self->rDirection != 0) {
                xzVelScale *= -1.0f;
            }

            self->velocity.x = Math_CosS(yaw) * xzVelScale;
            self->velocity.z = -Math_SinS(yaw) * xzVelScale;

            self->velocity.y = ((700.0f + KREG(21)) * 0.01f) + ((0.1f * Rand_ZeroOne()) * (KREG(24) + 20.0f));
            self->accel.y = ((-100.0f + KREG(22)) * 0.01f) + ((0.1f * Rand_ZeroOne()) * KREG(25));

            if (KREG(3) != 0) {
                self->velocity.x *= (KREG(3) * 0.01f);
                self->velocity.y *= (KREG(3) * 0.01f);
                self->velocity.z *= (KREG(3) * 0.01f);
                self->accel.y *= (KREG(4) * 0.01f);
            }
        }
    } else {
        if (self->rScale != 0) {
            self->rScale = (self->rScale - KREG(26)) - 3;
        }
    }
}
