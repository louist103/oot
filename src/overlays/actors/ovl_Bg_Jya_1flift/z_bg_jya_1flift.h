#ifndef _Z_BG_JYA_1FLIFT_H_
#define _Z_BG_JYA_1FLIFT_H_

#include "ultra64.h"
#include "global.h"

struct BgJya1flift;
typedef void (*BgJya1fliftActionFunc)(struct BgJya1flift*, GlobalContext*);
typedef struct BgJya1flift {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x01B0 */ BgJya1fliftActionFunc actionFunc;
    /* 0x01B6 */ u8 direction; //1 is moving down and 0 is moving up
    /* 0x01B7 */ u8 hasInitialized;
    /* 0x01B8 */ u8 isLinkRiding;
} BgJya1flift;

extern const ActorInit Bg_Jya_1flift_InitVars;

#endif
