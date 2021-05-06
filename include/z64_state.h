#ifndef Z64_STATE_H
#define Z64_STATE

#include "z64.h"

typedef struct {
    GlobalContext* saveGlobalCtx;
    GraphicsContext* saveGfxCtx;
    SaveContext* saveSaveContext;
    Player* savePlayer;
    GameInfo* saveGameInfo;
    MtxF* saveMatrixStack;
    u32 saveLastRandInt;
} SaveState;

#endif
