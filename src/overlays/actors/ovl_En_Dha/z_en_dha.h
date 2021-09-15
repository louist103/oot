#ifndef Z_EN_DHA_H
#define Z_EN_DHA_H

#include "ultra64.h"
#include "global.h"

struct EnDha;

typedef struct EnDha {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ char unk_14C[0x214];
} EnDha; // size = 0x0360

#endif
