/*
 * File: z_opening.c
 * Overlay: ovl_opening
 * Description: Initializes the game into the title screen
 */

#include "global.h"

void Opening_SetupTitleScreen(OpeningContext* self) {
    gSaveContext.gameMode = 1;
    self->state.running = false;
    gSaveContext.linkAge = 0;
    Sram_InitDebugSave();
    gSaveContext.cutsceneIndex = 0xFFF3;
    gSaveContext.sceneSetupIndex = 7;
    SET_NEXT_GAMESTATE(&self->state, Gameplay_Init, GlobalContext);
}

void func_80803C5C(OpeningContext* self) {
}

void Opening_Main(GameState* thisx) {
    OpeningContext* self = (OpeningContext*)thisx;

    func_80095248(self->state.gfxCtx, 0, 0, 0);
    Opening_SetupTitleScreen(self);
    func_80803C5C(self);
}

void Opening_Destroy(GameState* thisx) {
}

void Opening_Init(GameState* thisx) {
    OpeningContext* self = (OpeningContext*)thisx;

    R_UPDATE_RATE = 1;
    Matrix_Init(&self->state);
    View_Init(&self->view, self->state.gfxCtx);
    self->state.main = Opening_Main;
    self->state.destroy = Opening_Destroy;
}
