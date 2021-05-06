#include "z64_state.h"
#include "global.h"

SaveState state1;
SaveState* state2;
SaveState* state3;

void SaveState_Save(GlobalContext* globalCtx) {
    if (state1.saveGlobalCtx == NULL) {
        
        state1.saveGlobalCtx = DebugArena_MallocRDebug(sizeof(GlobalContext), __FILE__, __LINE__);
        if (state1.saveGlobalCtx == NULL) {
            __assert("state1.saveGlobalCtx != NULL", __FILE__, __LINE__);
        }
        osSyncPrintf("Allocated %x bytes for saveGlobalCtx\n", sizeof(GlobalContext));
    }

    if (state1.saveGfxCtx == NULL) {
        
        state1.saveGfxCtx = DebugArena_MallocRDebug(sizeof(GraphicsContext), __FILE__, __LINE__);
        if (state1.saveGfxCtx == NULL) {
            __assert("state1.saveGfxCtx != NULL", __FILE__, __LINE__);
        }
        osSyncPrintf("Allocated %x bytes for saveGfxCtx\n", sizeof(GraphicsContext));
    }


    if (state1.saveSaveContext == NULL) {
        state1.saveSaveContext = DebugArena_MallocRDebug(sizeof(SaveContext), __FILE__, __LINE__);
        if (state1.saveSaveContext == NULL) {
            __assert("state1.saveGlobalCtx != NULL", __FILE__, __LINE__);
        }
        osSyncPrintf("Allocated %x bytes for saveSaveContext\n", sizeof(SaveContext));
    }

    if (state1.saveGameInfo == NULL) {
        state1.saveGameInfo = DebugArena_MallocRDebug(sizeof(GameInfo), __FILE__, __LINE__);
        if (state1.saveGameInfo == NULL) {
            __assert("state1.saveGameInfo != NULL", __FILE__, __LINE__);
        }
        osSyncPrintf("Allocated %x bytes for saveGameInfo\n", sizeof(GameInfo));
    }

    if (state1.savePlayer == NULL) {
        state1.savePlayer = DebugArena_MallocRDebug(sizeof(Player), __FILE__, __LINE__);
        if (state1.savePlayer == NULL) {
            __assert("state1.savePlayer != NULL", __FILE__, __LINE__);
        }
        osSyncPrintf("Allocated %x bytes for savePlayer\n", sizeof(Player));
    }
    
    if (state1.saveMatrixStack == NULL) {
        state1.saveMatrixStack = DebugArena_MallocRDebug(sizeof(MtxF)*20, __FILE__, __LINE__);
        if (state1.saveMatrixStack == NULL) {
            __assert("state1.saveMatrixStack != NULL", __FILE__, __LINE__);
        }
        osSyncPrintf("Allocated %x bytes for saveMatrixStack\n", sizeof(MtxF)*20);
    }


    bcopy(globalCtx, state1.saveGlobalCtx, sizeof(GlobalContext));
    osSyncPrintf("Copied saveGlobalCtx\n");
    bcopy(&globalCtx->state.gfxCtx, state1.saveGfxCtx, sizeof(GraphicsContext));
    osSyncPrintf("Copied saveGraphicsContext");
    bcopy(&gSaveContext, state1.saveSaveContext, sizeof(SaveContext));
    osSyncPrintf("Copied gSaveContext\n");
    bcopy(gGameInfo, state1.saveGameInfo, sizeof(GameInfo));
    osSyncPrintf("Copied saveGameInfo\n");
    bcopy(PLAYER, state1.savePlayer, sizeof(Player));
    osSyncPrintf("Copied savePlayer\n");
    // memcpy(&state1.saveGlobalCtx, globalCtx, sizeof(GlobalContext));
    // memcpy(&state1.saveSaveContext, &gSaveContext, sizeof(SaveContext));
    // memcpy(&state1.saveGameInfo, gGameInfo, sizeof(GameInfo));
    // memcpy(&state1.savePlayer, PLAYER, sizeof(Player));
}

void SaveState_ClearState() {
    // if (state1 != NULL) {
    //    ZeldaArena_FreeDebug(state1, __FILE__, __LINE__);
    //    state1 = NULL;
    //}
}

void SaveState_Load() {
    GlobalContext* globalCtx = Effect_GetGlobalCtx();
    // if(state1 != NULL){
    bcopy(state1.saveGlobalCtx, globalCtx, sizeof(GlobalContext));
    osSyncPrintf("Loaded saveGlobalCtx\n");
    bcopy(state1.saveGfxCtx, &globalCtx->state.gfxCtx, sizeof(GraphicsContext));
    osSyncPrintf("Loaded saveGlobalCtx\n");
    bcopy(state1.saveSaveContext, &gSaveContext, sizeof(SaveContext));
    osSyncPrintf("Loaded saveGlobalCtx\n");
    bcopy(state1.saveGameInfo, gGameInfo, sizeof(GameInfo));
    osSyncPrintf("Loaded saveGameInfo\n");
    bcopy(state1.savePlayer, PLAYER, sizeof(Player));
    osSyncPrintf("Loaded savePlayer\n");
    // memcpy(&gSaveContext, &state1.saveSaveContext, sizeof(SaveContext));
    // memcpy(globalCtx, &state1.saveGlobalCtx, sizeof(GlobalContext));
    // memcpy(gGameInfo, &state1.saveGameInfo, sizeof(GameInfo));
    // memcpy(PLAYER, &state1.savePlayer, sizeof(Player));
    //}
}