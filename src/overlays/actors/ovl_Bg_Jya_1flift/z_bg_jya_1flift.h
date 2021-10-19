#ifndef Z_BG_JYA_1FLIFT_H
#define Z_BG_JYA_1FLIFT_H

#include "ultra64.h"
#include "global.h"

struct BgJya1flift;

typedef void (*BgJya1fliftActionFunc)(struct BgJya1flift*, GlobalContext*);

typedef struct BgJya1flift {
    /* 0x0000 */ DynaPolyActor dyna;
    BgJya1fliftActionFunc actionFunc;
    u8 isMovingDown;
    u8 isLinkRiding;
    u8 waitingForStepOff;
} BgJya1flift;

#endif
