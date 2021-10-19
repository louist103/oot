/*
 * File: z_bg_haka_water.c
 * Overlay: ovl_Bg_Haka_Water
 * Description: Bottom of the Well water level changer
 */

#include "z_bg_haka_water.h"
#include "objects/object_hakach_objects/object_hakach_objects.h"

#define FLAGS 0x00000030

#define THIS ((BgHakaWater*)thisx)

void BgHakaWater_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaWater_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaWater_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaWater_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgHakaWater_LowerWater(BgHakaWater* self, GlobalContext* globalCtx);
void BgHakaWater_Wait(BgHakaWater* self, GlobalContext* globalCtx);
void BgHakaWater_ChangeWaterLevel(BgHakaWater* self, GlobalContext* globalCtx);

const ActorInit Bg_Haka_Water_InitVars = {
    ACTOR_BG_HAKA_WATER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HAKACH_OBJECTS,
    sizeof(BgHakaWater),
    (ActorFunc)BgHakaWater_Init,
    (ActorFunc)BgHakaWater_Destroy,
    (ActorFunc)BgHakaWater_Update,
    (ActorFunc)BgHakaWater_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaWater_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaWater* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    if (Flags_GetSwitch(globalCtx, self->actor.params)) {
        self->isLowered = true;
        self->actor.home.pos.y -= 200.0f;
        self->actor.world.pos.y = self->actor.home.pos.y;
    } else {
        self->isLowered = false;
    }
    BgHakaWater_LowerWater(self, globalCtx);
    self->actionFunc = BgHakaWater_Wait;
}

void BgHakaWater_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void BgHakaWater_LowerWater(BgHakaWater* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i < 9; i++) {
        globalCtx->colCtx.colHeader->waterBoxes[i].ySurface = (s16)self->actor.world.pos.y - 8;
    }
}

void BgHakaWater_Wait(BgHakaWater* self, GlobalContext* globalCtx) {
    if ((!self->isLowered && Flags_GetSwitch(globalCtx, self->actor.params)) ||
        (self->isLowered && !Flags_GetSwitch(globalCtx, self->actor.params))) {
        if (self->isLowered) {
            self->isLowered = false;
            self->actor.draw = BgHakaWater_Draw;
            self->actor.home.pos.y += 200.0f;
        } else {
            self->isLowered = true;
            self->actor.home.pos.y -= 200.0f;
        }
        self->actionFunc = BgHakaWater_ChangeWaterLevel;
    }
}

void BgHakaWater_ChangeWaterLevel(BgHakaWater* self, GlobalContext* globalCtx) {
    if (!self->isLowered && Flags_GetSwitch(globalCtx, self->actor.params)) {
        self->isLowered = true;
        self->actor.home.pos.y -= 200.0f;
    } else if (self->isLowered && !Flags_GetSwitch(globalCtx, self->actor.params)) {
        self->isLowered = false;
        self->actor.home.pos.y += 200.0f;
    }

    if (self->actor.home.pos.y < self->actor.world.pos.y) {
        func_8002F948(&self->actor, NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG);
    } else {
        func_8002F948(&self->actor, NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG);
    }

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.5f) != 0) {
        self->actionFunc = BgHakaWater_Wait;
        if (self->isLowered) {
            self->actor.draw = NULL;
        }
    }
    BgHakaWater_LowerWater(self, globalCtx);
}

void BgHakaWater_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaWater* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgHakaWater_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaWater* self = THIS;
    s32 pad;
    f32 temp;
    s32 pad2;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_haka_water.c", 287);
    func_80093D84(globalCtx->state.gfxCtx);

    if (self->isLowered) {
        temp = self->actor.world.pos.y - self->actor.home.pos.y;
    } else {
        temp = self->actor.world.pos.y - (self->actor.home.pos.y - 200.0f);
    }

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (u8)(0.765f * temp));
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, globalCtx->gameplayFrames % 128,
                                globalCtx->gameplayFrames % 128, 32, 32, 1, 0, (0 - globalCtx->gameplayFrames) % 128,
                                32, 32));

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_water.c", 312),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gBotwWaterRingDL);

    Matrix_Translate(0.0f, 92.0f, -1680.0f, MTXMODE_NEW);
    Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);
    temp -= 170.0f;
    if (temp < 0.0f) {
        temp = 0.0f;
    }

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (u8)(5.1f * temp));
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_water.c", 328),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gBotwWaterFallDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_haka_water.c", 332);
}
