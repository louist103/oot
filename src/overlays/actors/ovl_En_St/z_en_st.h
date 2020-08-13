#ifndef _Z_EN_ST_H_
#define _Z_EN_ST_H_

#include <ultra64.h>
#include <global.h>

struct EnSt;

typedef struct EnSt {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ char unk_14C[0x430];
} EnSt; // size = 0x057C

extern const ActorInit En_St_InitVars;

#endif
