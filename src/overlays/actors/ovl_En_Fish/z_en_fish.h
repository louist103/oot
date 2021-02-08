#ifndef _Z_EN_FISH_H_
#define _Z_EN_FISH_H_

#include "ultra64.h"
#include "global.h"

struct EnFish;

typedef void (*EnFishActionFunc)(struct EnFish*, struct GlobalContext*);

typedef struct EnFish {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ ColliderJntSph collider;
    /* 0x016C */ ColliderJntSphElement colliderItems;
    /* 0x01AC */ SkelAnime skelAnime;
    /* 0x01F0 */ Vec3s limbDrawTable[5];
    /* 0x01F6 */ char unk_1F6[0x24];
    /* 0x021A */ Vec3s unk_21A[5];
    /* 0x0220 */ char unk_220[0x8];
    /* 0x022C */ s16 unk_22C;
    /* 0x022E */ s16 unk_22E;
    /* 0x0230 */ char unk_230[0x14];
    /* 0x0244 */ EnFishActionFunc actionFunc;
    /* 0x0248 */ s16 unk_248;
    /* 0x024A */ char unk_24A[0x2];
    /* 0x024C */ s16 unk24C;
    /* 0x024E */ s16 unk24E;
    /* 0x0250 */ void* unk_250;
} EnFish; // size = 0x0254

extern const ActorInit En_Fish_InitVars;

#endif