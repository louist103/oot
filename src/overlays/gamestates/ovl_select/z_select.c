/*
 * File: z_select.c
 * Overlay: ovl_select
 * Description: Debug Scene Select Menu
 */

#include "ultra64.h"
#include "global.h"
#include "vt.h"
#include "alloca.h"

OSTime start;
OSTime end;

OSTime start2;
OSTime end2;

void testmanual() {
    s32 test[100];

    osInvalICache(NULL, 0x7FFF);
    start = osGetTime();
    test[0] = 0;
    test[1] = 0;
    test[2] = 0;
    test[3] = 0;
    test[4] = 0;
    test[5] = 0;
    test[6] = 0;
    test[7] = 0;
    test[8] = 0;
    test[9] = 0;
    test[10] = 0;
    test[11] = 0;
    test[12] = 0;
    test[13] = 0;
    test[14] = 0;
    test[15] = 0;
    test[16] = 0;
    test[17] = 0;
    test[18] = 0;
    test[19] = 0;
    test[20] = 0;
    test[21] = 0;
    test[22] = 0;
    test[23] = 0;
    test[24] = 0;
    test[25] = 0;
    test[26] = 0;
    test[27] = 0;
    test[28] = 0;
    test[29] = 0;
    test[30] = 0;
    test[31] = 0;
    test[32] = 0;
    test[33] = 0;
    test[34] = 0;
    test[35] = 0;
    test[36] = 0;
    test[37] = 0;
    test[38] = 0;
    test[39] = 0;
    test[40] = 0;
    test[41] = 0;
    test[42] = 0;
    test[43] = 0;
    test[44] = 0;
    test[45] = 0;
    test[46] = 0;
    test[47] = 0;
    test[48] = 0;
    test[49] = 0;
    test[50] = 0;
    test[51] = 0;
    test[52] = 0;
    test[53] = 0;
    test[54] = 0;
    test[55] = 0;
    test[56] = 0;
    test[57] = 0;
    test[58] = 0;
    test[59] = 0;
    test[60] = 0;
    test[61] = 0;
    test[62] = 0;
    test[63] = 0;
    test[64] = 0;
    test[65] = 0;
    test[66] = 0;
    test[67] = 0;
    test[68] = 0;
    test[69] = 0;
    test[70] = 0;
    test[71] = 0;
    test[72] = 0;
    test[73] = 0;
    test[74] = 0;
    test[75] = 0;
    test[76] = 0;
    test[77] = 0;
    test[78] = 0;
    test[79] = 0;
    test[80] = 0;
    test[81] = 0;
    test[82] = 0;
    test[83] = 0;
    test[84] = 0;
    test[85] = 0;
    test[86] = 0;
    test[87] = 0;
    test[88] = 0;
    test[89] = 0;
    test[90] = 0;
    test[91] = 0;
    test[92] = 0;
    test[93] = 0;
    test[94] = 0;
    test[95] = 0;
    test[96] = 0;
    test[97] = 0;
    test[98] = 0;
    test[99] = 0;
    end = osGetTime();
}


void testLoop() {
    s32 test[100];
    u32 i = 0;
    osInvalICache(NULL, 0x7FFF);
    start2 = osGetTime();
    for (i = 0; i < 100; i++) {
        test[i] = 0;
    }
    end2 = osGetTime();

}


void Select_Draw(SelectContext* this) {
    GraphicsContext* gfxCtx = this->state.gfxCtx;
    GfxPrint printer;

    OPEN_DISPS(gfxCtx, "../z_select.c", 1013);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(gfxCtx, 0, 0, 0);
    SET_FULLSCREEN_VIEWPORT(&this->view);
    func_800AAA50(&this->view, 0xF);
    func_80094140(gfxCtx);

    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer, this->state.gfxCtx->overlay.p);
    GfxPrint_SetColor(&printer, 255, 255, 255, 255);
    GfxPrint_SetPos(&printer, 3, 3);
    GfxPrint_Printf(&printer, "Looping %d\n", end2 - start2);
    GfxPrint_Printf(&printer, "non-looping %d", end - start);
    this->state.gfxCtx->overlay.p = GfxPrint_Close(&printer);

    CLOSE_DISPS(gfxCtx, "../z_select.c", 1037);
}

void Select_Main(GameState* thisx) {
    SelectContext* this = (SelectContext*)thisx;

    Select_Draw(this);
}

void Select_Destroy(GameState* thisx) {
    osSyncPrintf("%c", 7);
    // "view_cleanup will hang, so it won't be called"
    osSyncPrintf("*** view_cleanupはハングアップするので、呼ばない ***\n");
}

void Select_Init(GameState* thisx) {
    SelectContext* this = (SelectContext*)thisx;
    u32 size;
    s32 pad;

    this->state.main = Select_Main;
    this->state.destroy = Select_Destroy;

    View_Init(&this->view, this->state.gfxCtx);

    size = (u32)_z_select_staticSegmentRomEnd - (u32)_z_select_staticSegmentRomStart;

    testmanual();
    testLoop();

    this->staticSegment = GameState_Alloc(&this->state, size, "../z_select.c", 1114);
    DmaMgr_SendRequest1(this->staticSegment, _z_select_staticSegmentRomStart, size, "../z_select.c", 1115);
}
