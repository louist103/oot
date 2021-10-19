#include "global.h"

#define COLD_RESET 0
#define NMI 1

void PreNmiBuff_Init(PreNmiBuff* self) {
    self->resetting = false;

    if (osResetType == COLD_RESET) {
        self->resetCount = 0;
        self->duration = 0;
    } else {
        self->resetCount++;
        self->duration += self->resetTime;
    }

    self->resetTime = 0;
}

void PreNmiBuff_SetReset(PreNmiBuff* self) {
    self->resetting = true;
    self->resetTime = osGetTime();
}

u32 PreNmiBuff_IsResetting(PreNmiBuff* self) {
    return self->resetting;
}
