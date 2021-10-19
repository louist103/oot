#include "global.h"

void Sample_HandleStateChange(SampleContext* self) {
    if (CHECK_BTN_ALL(self->state.input[0].press.button, BTN_START)) {
        SET_NEXT_GAMESTATE(&self->state, Gameplay_Init, GlobalContext);
        self->state.running = false;
    }
}

void Sample_Draw(SampleContext* self) {
    GraphicsContext* gfxCtx = self->state.gfxCtx;
    View* view = &self->view;

    OPEN_DISPS(gfxCtx, "../z_sample.c", 62);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    gSPSegment(POLY_OPA_DISP++, 0x01, self->staticSegment);

    func_80095248(gfxCtx, 0, 0, 0);

    view->flags = 1 | 2 | 4;
    func_800AAA50(view, 15);

    {
        Mtx* mtx = Graph_Alloc(gfxCtx, sizeof(Mtx));
        guPosition(mtx, SREG(37), SREG(38), SREG(39), 1.0f, SREG(40), SREG(41), SREG(42));
        gSPMatrix(POLY_OPA_DISP++, mtx, G_MTX_LOAD);
    }

    POLY_OPA_DISP = Gfx_SetFog2(POLY_OPA_DISP, 255, 255, 255, 0, 0, 0);
    func_80093D18(gfxCtx);

    gDPSetCycleType(POLY_OPA_DISP++, G_CYC_1CYCLE);
    gDPSetRenderMode(POLY_OPA_DISP++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 0, 0);

    CLOSE_DISPS(gfxCtx, "../z_sample.c", 111);
}

void Sample_Main(GameState* thisx) {
    SampleContext* self = (SampleContext*)thisx;

    Sample_Draw(self);
    Sample_HandleStateChange(self);
}

void Sample_Destroy(GameState* thisx) {
}

void Sample_SetupView(SampleContext* self) {
    View* view = &self->view;
    GraphicsContext* gfxCtx = self->state.gfxCtx;

    View_Init(view, gfxCtx);
    SET_FULLSCREEN_VIEWPORT(view);
    func_800AA460(view, 60.0f, 10.0f, 12800.0f);

    {
        Vec3f eye;
        Vec3f lookAt;
        Vec3f up;

        eye.x = 0.0f;
        eye.y = 0.0f;
        eye.z = 3000.0f;
        lookAt.x = 0.0f;
        lookAt.y = 0.0f;
        lookAt.z = 0.0f;
        up.x = 0.0f;
        up.z = 0.0f;
        up.y = 1.0f;

        func_800AA358(view, &eye, &lookAt, &up);
    }
}

void Sample_LoadTitleStatic(SampleContext* self) {
    u32 size = _title_staticSegmentRomEnd - _title_staticSegmentRomStart;

    self->staticSegment = GameState_Alloc(&self->state, size, "../z_sample.c", 163);
    DmaMgr_SendRequest1(self->staticSegment, _title_staticSegmentRomStart, size, "../z_sample.c", 164);
}

void Sample_Init(GameState* thisx) {
    SampleContext* self = (SampleContext*)thisx;

    self->state.main = Sample_Main;
    self->state.destroy = Sample_Destroy;
    R_UPDATE_RATE = 1;
    Sample_SetupView(self);
    Sample_LoadTitleStatic(self);
    SREG(37) = 0;
    SREG(38) = 0;
    SREG(39) = 0;
    SREG(40) = 0;
    SREG(41) = 0;
    SREG(42) = 0;
}
