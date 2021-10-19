/*
 * File: z_eff_ss_dead_sound.c
 * Overlay: ovl_Effect_Ss_Dead_Sound
 * Description: Plays a sound effect
 */

#include "z_eff_ss_dead_sound.h"

#define rSfxId regs[10]
#define rRepeatMode regs[11] // sound is replayed every update. unused in the original game

u32 EffectSsDeadSound_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsDeadSound_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

EffectSsInit Effect_Ss_Dead_Sound_InitVars = {
    EFFECT_SS_DEAD_SOUND,
    EffectSsDeadSound_Init,
};

u32 EffectSsDeadSound_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsDeadSoundInitParams* initParams = (EffectSsDeadSoundInitParams*)initParamsx;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->accel = initParams->accel;
    self->flags = 2;
    self->life = initParams->life;
    self->draw = NULL;
    self->update = EffectSsDeadSound_Update;
    self->rRepeatMode = initParams->repeatMode;
    self->rSfxId = initParams->sfxId;
    osSyncPrintf("コンストラクター3\n"); // "constructor 3"

    return 1;
}

void EffectSsDeadSound_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    switch (self->rRepeatMode) {
        case DEADSOUND_REPEAT_MODE_OFF:
            self->rRepeatMode--; // decrement to 0 so sound only plays once
            break;
        case DEADSOUND_REPEAT_MODE_ON:
            break;
        default:
            return;
    }

    Audio_PlaySoundGeneral(self->rSfxId, &self->pos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
}
