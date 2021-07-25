#ifndef _Z_EN_BIRD_H_
#define _Z_EN_BIRD_H_

#include "ultra64.h"
#include "global.h"

struct EnBoxGame;

typedef void (*EnBoxGameActionFunc)(struct EnBoxGame*, GlobalContext*);

typedef struct EnBoxGame {
    DynaPolyActor dyna;
    EnBoxGameActionFunc actionFunc;
} EnBoxGame; 

extern const ActorInit En_Box_Game_InitVars;

#endif
