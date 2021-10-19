#include "global.h"
#include "vt.h"

void func_80092320(PreNMIContext* self) {
    self->state.running = false;
    self->state.init = NULL;
    self->state.size = 0;
}

void PreNMI_Update(PreNMIContext* self) {
    osSyncPrintf(VT_COL(YELLOW, BLACK) "prenmi_move\n" VT_RST);

    // Strings existing only in rodata
    if (0) {
        osSyncPrintf("../z_prenmi.c");
        osSyncPrintf("(int)volume = %d\n");
    }

    if (self->timer == 0) {
        ViConfig_UpdateVi(1);
        func_80092320(self);
        return;
    }

    self->timer--;
}

void PreNMI_Draw(PreNMIContext* self) {
    GraphicsContext* gfxCtx = self->state.gfxCtx;

    osSyncPrintf(VT_COL(YELLOW, BLACK) "prenmi_draw\n" VT_RST);

    OPEN_DISPS(gfxCtx, "../z_prenmi.c", 96);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(gfxCtx, 0, 0, 0);
    func_800940B0(gfxCtx);
    gDPSetFillColor(POLY_OPA_DISP++, (GPACK_RGBA5551(255, 255, 255, 1) << 16) | GPACK_RGBA5551(255, 255, 255, 1));
    gDPFillRectangle(POLY_OPA_DISP++, 0, self->timer + 100, SCREEN_WIDTH - 1, self->timer + 100);

    CLOSE_DISPS(gfxCtx, "../z_prenmi.c", 112);
}

void PreNMI_Main(GameState* thisx) {
    PreNMIContext* self = (PreNMIContext*)thisx;

    PreNMI_Update(self);
    PreNMI_Draw(self);

    self->state.unk_A0 = 1;
}

void PreNMI_Destroy(GameState* thisx) {
}

void PreNMI_Init(GameState* thisx) {
    PreNMIContext* self = (PreNMIContext*)thisx;

    self->state.main = PreNMI_Main;
    self->state.destroy = PreNMI_Destroy;
    self->timer = 30;
    self->unk_A8 = 10;

    R_UPDATE_RATE = 1;
}
