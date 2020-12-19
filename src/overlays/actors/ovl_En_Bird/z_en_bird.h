#ifndef _Z_EN_BIRD_H_
#define _Z_EN_BIRD_H_

#include "ultra64.h"
#include "global.h"

struct EnBird;

typedef void (*EnBirdActionFunc)(struct EnBird*, GlobalContext*);

typedef struct EnBird {
    /* 0x0000 */ Actor actor;

} EnBird; // size = 0x01DC

extern const ActorInit En_Bird_InitVars;

#endif
