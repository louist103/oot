/*
 * File: z_title.c
 * Overlay: ovl_title
 * Description: Displays the Nintendo Logo
 */

#include "global.h"
#include "alloca.h"
#include "textures/nintendo_rogo_static/nintendo_rogo_static.h"

void Title_PrintBuildInfo(Gfx** gfxp) {
    Gfx* g;
    GfxPrint* printer;

    g = *gfxp;
    g = func_8009411C(g);
    printer = alloca(sizeof(GfxPrint));
    GfxPrint_Init(printer);
    GfxPrint_Open(printer, g);
    GfxPrint_SetColor(printer, 255, 155, 255, 255);
    GfxPrint_SetPos(printer, 9, 21);
    GfxPrint_Printf(printer, "GCC VERSION");
    GfxPrint_SetColor(printer, 255, 255, 255, 255);
    GfxPrint_SetPos(printer, 7, 23);
    GfxPrint_Printf(printer, "[Creator:%s]", gBuildTeam);
    GfxPrint_SetPos(printer, 7, 24);
    GfxPrint_Printf(printer, "[Date:%s]", gBuildDate);
    g = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);
    *gfxp = g;
}

// Note: In other rom versions self function also updates unk_1D4, coverAlpha, addAlpha, visibleDuration to calculate
// the fade-in/fade-out + the duration of the n64 logo animation
void Title_Calc(TitleContext* self) {
#ifdef NORMAL_GAMEPLAY
    if ((self->coverAlpha == 0) && (self->visibleDuration != 0)) {
        self->visibleDuration--;
        self->unk_1D4--;
        if (self->unk_1D4 == 0) {
            self->unk_1D4 = 400;
        }
    } else {
        self->coverAlpha += self->addAlpha;
        if (self->coverAlpha <= 0) {
            self->coverAlpha = 0;
            self->addAlpha = 3;
        } else if (self->coverAlpha >= 0xFF) {
            self->coverAlpha = 0xFF;
            self->exit = 1;
        }
    }
    self->uls = self->ult & 0x7F;
    self->ult++;
#else
    self->exit = 1;
#endif
}

void Title_SetupView(TitleContext* self, f32 x, f32 y, f32 z) {
    View* view = &self->view;
    Vec3f eye;
    Vec3f lookAt;
    Vec3f up;

    eye.x = x;
    eye.y = y;
    eye.z = z;
    up.x = up.z = 0.0f;
    lookAt.x = lookAt.y = lookAt.z = 0.0f;
    up.y = 1.0f;

    func_800AA460(view, 30.0f, 10.0f, 12800.0f);
    func_800AA358(view, &eye, &lookAt, &up);
    func_800AAA50(view, 0xF);
}

void Title_Draw(TitleContext* self) {
    static s16 sTitleRotY = 0;
    static Lights1 sTitleLights = gdSPDefLights1(0x64, 0x64, 0x64, 0xFF, 0xFF, 0xFF, 0x45, 0x45, 0x45);

    u16 y;
    u16 idx;
    s32 pad1;
    Vec3f v3;
    Vec3f v1;
    Vec3f v2;
    s32 pad2[2];

    OPEN_DISPS(self->state.gfxCtx, "../z_title.c", 395);

    v3.x = 69;
    v3.y = 69;
    v3.z = 69;
    v2.x = -4949.148;
    v2.y = 4002.5417;
    v1.x = 0;
    v1.y = 0;
    v1.z = 0;
    v2.z = 1119.0837;

    func_8002EABC(&v1, &v2, &v3, self->state.gfxCtx);
    gSPSetLights1(POLY_OPA_DISP++, sTitleLights);
    Title_SetupView(self, 0, 150.0, 300.0);
    func_80093D18(self->state.gfxCtx);
    Matrix_Translate(-53.0, -5.0, 0, MTXMODE_NEW);
    Matrix_Scale(1.0, 1.0, 1.0, MTXMODE_APPLY);
    Matrix_RotateRPY(0, sTitleRotY, 0, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(self->state.gfxCtx, "../z_title.c", 424), G_MTX_LOAD);
    gSPDisplayList(POLY_OPA_DISP++, gNintendo64LogoDL);
    func_800944C4(self->state.gfxCtx);
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetCycleType(POLY_OPA_DISP++, G_CYC_2CYCLE);
    gDPSetRenderMode(POLY_OPA_DISP++, G_RM_XLU_SURF2, G_RM_OPA_CI | CVG_DST_WRAP);
    gDPSetCombineLERP(POLY_OPA_DISP++, TEXEL1, PRIMITIVE, ENV_ALPHA, TEXEL0, 0, 0, 0, TEXEL0, PRIMITIVE, ENVIRONMENT,
                      COMBINED, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 170, 255, 255, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 255, 128);

    gDPLoadMultiBlock(POLY_OPA_DISP++, nintendo_rogo_static_Tex_001800, 0x100, 1, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                      G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, 2, 11);

    for (idx = 0, y = 94; idx < 16; idx++, y += 2) {
        gDPLoadTextureBlock(POLY_OPA_DISP++, &((u8*)nintendo_rogo_static_Tex_000000)[0x180 * idx], G_IM_FMT_I,
                            G_IM_SIZ_8b, 192, 2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                            G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gDPSetTileSize(POLY_OPA_DISP++, 1, self->uls, (self->ult & 0x7F) - idx * 4, 0, 0);
        gSPTextureRectangle(POLY_OPA_DISP++, 388, y << 2, 1156, (y + 2) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
    }

    Environment_FillScreen(self->state.gfxCtx, 0, 0, 0, (s16)self->coverAlpha, FILL_SCREEN_XLU);

    sTitleRotY += 300;

    CLOSE_DISPS(self->state.gfxCtx, "../z_title.c", 483);
}

void Title_Main(GameState* thisx) {
    TitleContext* self = (TitleContext*)thisx;

    OPEN_DISPS(self->state.gfxCtx, "../z_title.c", 494);

    gSPSegment(POLY_OPA_DISP++, 0, NULL);
    gSPSegment(POLY_OPA_DISP++, 1, self->staticSegment);
    func_80095248(self->state.gfxCtx, 0, 0, 0);
    Title_Calc(self);
    Title_Draw(self);

    if (1) {
        Gfx* gfx = POLY_OPA_DISP;
        s32 pad;

        Title_PrintBuildInfo(&gfx);
        POLY_OPA_DISP = gfx;
    }

    if (self->exit) {
        gSaveContext.seqIndex = 0xFF;
        gSaveContext.nightSeqIndex = 0xFF;
        gSaveContext.gameMode = 1;
        self->state.running = false;
        SET_NEXT_GAMESTATE(&self->state, Opening_Init, OpeningContext);
    }

    CLOSE_DISPS(self->state.gfxCtx, "../z_title.c", 541);
}

void Title_Destroy(GameState* thisx) {
    TitleContext* self = (TitleContext*)thisx;

    Sram_InitSram(&self->state, &self->sramCtx);
}

void Title_Init(GameState* thisx) {
    u32 size = (u32)_nintendo_rogo_staticSegmentRomEnd - (u32)_nintendo_rogo_staticSegmentRomStart;
    TitleContext* self = (TitleContext*)thisx;

    self->staticSegment = GameState_Alloc(&self->state, size, "../z_title.c", 611);
    osSyncPrintf("z_title.c\n");
    ASSERT(self->staticSegment != NULL, "self->staticSegment != NULL", "../z_title.c", 614);
    DmaMgr_SendRequest1(self->staticSegment, (u32)_nintendo_rogo_staticSegmentRomStart, size, "../z_title.c", 615);
    R_UPDATE_RATE = 1;
    Matrix_Init(&self->state);
    View_Init(&self->view, self->state.gfxCtx);
    self->state.main = Title_Main;
    self->state.destroy = Title_Destroy;
    self->exit = false;
    gSaveContext.fileNum = 0xFF;
    Sram_Alloc(&self->state, &self->sramCtx);
    self->ult = 0;
    self->unk_1D4 = 0x14;
    self->coverAlpha = 255;
    self->addAlpha = -3;
    self->visibleDuration = 0x3C;
}
