#ifndef _Z_BG_JYA_BOMBIWA_H_
#define _Z_BG_JYA_BOMBIWA_H_

#include <ultra64.h>
#include <global.h>

struct BgJyaBombiwa;

typedef struct BgJyaBombiwa {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ char unk_164[0x64];
} BgJyaBombiwa; // size = 0x01C8

extern const ActorInit Bg_Jya_Bombiwa_InitVars;

#endif
