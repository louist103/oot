#ifndef _Z_EN_EX_RUPPY_H_
#define _Z_EN_EX_RUPPY_H_

#include <ultra64.h>
#include <global.h>
#include "../ovl_En_Diving_Game/z_en_diving_game.h"

struct EnExRuppy;

typedef void (*EnExRuppyActionFunc)(struct EnExRuppy*, GlobalContext*);

typedef struct EnExRuppy {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ EnExRuppyActionFunc actionFunc;
    /* 0x0150 */ s16 unk_150;
    /* 0x0152 */ s16 unk_152;
    /* 0x0154 */ s16 isFalling;
    /* 0x0156 */ s16 timer;
    /* 0x0158 */ s16 value;
    /* 0x015A */ s16 unk_15A; 
    /* 0x015C */ s16 unk_15C;
    /* 0x0160 */ f32 unk_160;
} EnExRuppy; // size = 0x0164

extern const ActorInit En_Ex_Ruppy_InitVars;

#endif
