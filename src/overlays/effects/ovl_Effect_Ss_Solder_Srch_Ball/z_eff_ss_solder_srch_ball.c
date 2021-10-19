/*
 * File: z_eff_ss_solder_srch_ball.c
 * Overlay: ovl_Effect_Ss_Solder_Srch_Ball
 * Description: Vision sphere for courtyard guards
 */

#include "z_eff_ss_solder_srch_ball.h"

#define rUnused regs[1]

u32 EffectSsSolderSrchBall_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsSolderSrchBall_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Solder_Srch_Ball_InitVars = {
    EFFECT_SS_SOLDER_SRCH_BALL,
    EffectSsSolderSrchBall_Init,
};

u32 EffectSsSolderSrchBall_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsSolderSrchBallInitParams* initParams = (EffectSsSolderSrchBallInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->update = EffectSsSolderSrchBall_Update;
    self->life = 100;
    self->rUnused = initParams->unused;
    self->actor = initParams->linkDetected; // actor field was incorrectly used as a pointer to something else
    return 1;
}

void EffectSsSolderSrchBall_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    s32 pad;
    f32 playerPosDiffX;
    f32 playerPosDiffY;
    f32 playerPosDiffZ;
    s16* linkDetected;
    Player* player = GET_PLAYER(globalCtx);

    linkDetected = self->actor;

    playerPosDiffX = player->actor.world.pos.x - self->pos.x;
    playerPosDiffY = player->actor.world.pos.y - self->pos.y;
    playerPosDiffZ = player->actor.world.pos.z - self->pos.z;

    if (!BgCheck_SphVsFirstPoly(&globalCtx->colCtx, &self->pos, 30.0f)) {
        if (sqrtf(SQ(playerPosDiffX) + SQ(playerPosDiffY) + SQ(playerPosDiffZ)) < 70.0f) {
            *linkDetected = true;
        }
    } else {
        if (self->life > 1) {
            self->life = 1;
        }
    }
}
