/*
 * File: z_bg_ydan_hasi.c
 * Overlay: ovl_Bg_Ydan_Hasi
 * Description: Deku Tree Puzzle elements. Water plane and floating block in B1, and 3 blocks on 2F
 */

#include "z_bg_ydan_hasi.h"
#include "objects/object_ydan_objects/object_ydan_objects.h"

#define FLAGS 0x00000030

#define THIS ((BgYdanHasi*)thisx)

void BgYdanHasi_Init(Actor* thisx, GlobalContext* globalCtx);
void BgYdanHasi_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgYdanHasi_Update(Actor* thisx, GlobalContext* globalCtx);
void BgYdanHasi_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgYdanHasi_InitWater(BgYdanHasi* self, GlobalContext* globalCtx);
void BgYdanHasi_UpdateFloatingBlock(BgYdanHasi* self, GlobalContext* globalCtx);
void BgYdanHasi_SetupThreeBlocks(BgYdanHasi* self, GlobalContext* globalCtx);
void BgYdanHasi_MoveWater(BgYdanHasi* self, GlobalContext* globalCtx);
void BgYdanHasi_DecWaterTimer(BgYdanHasi* self, GlobalContext* globalCtx);
void BgYdanHasi_UpdateThreeBlocks(BgYdanHasi* self, GlobalContext* globalCtx);

const ActorInit Bg_Ydan_Hasi_InitVars = {
    ACTOR_BG_YDAN_HASI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_YDAN_OBJECTS,
    sizeof(BgYdanHasi),
    (ActorFunc)BgYdanHasi_Init,
    (ActorFunc)BgYdanHasi_Destroy,
    (ActorFunc)BgYdanHasi_Update,
    (ActorFunc)BgYdanHasi_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgYdanHasi_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgYdanHasi* self = THIS;
    CollisionHeader* colHeader = NULL;
    WaterBox* waterBox;

    Actor_ProcessInitChain(thisx, sInitChain);
    self->type = ((thisx->params >> 8) & 0x3F);
    thisx->params = thisx->params & 0xFF;
    waterBox = &globalCtx->colCtx.colHeader->waterBoxes[1];
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    if (thisx->params == HASI_WATER) {
        // Water the moving platform floats on in B1. Never runs in Master Quest
        waterBox->ySurface = thisx->world.pos.y = thisx->home.pos.y += -5.0f;
        self->actionFunc = BgYdanHasi_InitWater;
    } else {
        if (thisx->params == HASI_WATER_BLOCK) {
            // Moving platform on the water in B1
            CollisionHeader_GetVirtual(&gDTSlidingPlatformCol, &colHeader);
            thisx->scale.z = 0.15f;
            thisx->scale.x = 0.15f;
            thisx->world.pos.y = (waterBox->ySurface + 20.0f);
            self->actionFunc = BgYdanHasi_UpdateFloatingBlock;
        } else {
            // 3 platforms on 2F
            CollisionHeader_GetVirtual(&gDTRisingPlatformsCol, &colHeader);
            thisx->draw = NULL;
            self->actionFunc = BgYdanHasi_SetupThreeBlocks;
            Actor_SetFocus(&self->dyna.actor, 40.0f);
        }
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    }
    self->timer = 0;
}

void BgYdanHasi_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanHasi* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgYdanHasi_UpdateFloatingBlock(BgYdanHasi* self, GlobalContext* globalCtx) {
    WaterBox* waterBox;
    f32 framesAfterMath;

    framesAfterMath = sinf((globalCtx->gameplayFrames & 0xFF) * (M_PI / 128)) * 165.0f;
    self->dyna.actor.world.pos.x =
        ((Math_SinS(self->dyna.actor.world.rot.y) * framesAfterMath) + self->dyna.actor.home.pos.x);
    self->dyna.actor.world.pos.z =
        ((Math_CosS(self->dyna.actor.world.rot.y) * framesAfterMath) + self->dyna.actor.home.pos.z);
    waterBox = &globalCtx->colCtx.colHeader->waterBoxes[1];
    self->dyna.actor.world.pos.y = waterBox->ySurface + 20.0f;
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->timer = 50;
    }
    self->dyna.actor.world.pos.y += 2.0f * sinf(self->timer * (M_PI / 25));
}

void BgYdanHasi_InitWater(BgYdanHasi* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->type)) {
        self->timer = 600;
        self->actionFunc = BgYdanHasi_MoveWater;
    }
}

void BgYdanHasi_MoveWater(BgYdanHasi* self, GlobalContext* globalCtx) {
    WaterBox* waterBox;

    if (self->timer == 0) {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 1.0f) != 0) {
            Flags_UnsetSwitch(globalCtx, self->type);
            self->actionFunc = BgYdanHasi_InitWater;
        }
        func_8002F948(&self->dyna.actor, NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG);
    } else {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 47.0f, 0.5f)) {
            self->actionFunc = BgYdanHasi_DecWaterTimer;
        }
        func_8002F948(&self->dyna.actor, NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG);
    }
    waterBox = &globalCtx->colCtx.colHeader->waterBoxes[1];
    waterBox->ySurface = self->dyna.actor.world.pos.y;
}

void BgYdanHasi_DecWaterTimer(BgYdanHasi* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    func_8002F994(&self->dyna.actor, self->timer);
    if (self->timer == 0) {
        self->actionFunc = BgYdanHasi_MoveWater;
    }
}

void BgYdanHasi_SetupThreeBlocks(BgYdanHasi* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->type)) {
        self->timer = 260;
        self->dyna.actor.draw = BgYdanHasi_Draw;
        self->actionFunc = BgYdanHasi_UpdateThreeBlocks;
        OnePointCutscene_Init(globalCtx, 3040, 30, &self->dyna.actor, MAIN_CAM);
    }
}

void BgYdanHasi_UpdateThreeBlocks(BgYdanHasi* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 3.0f) != 0) {
            Flags_UnsetSwitch(globalCtx, self->type);
            self->dyna.actor.draw = NULL;
            self->actionFunc = BgYdanHasi_SetupThreeBlocks;
        } else {
            func_8002F948(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
        }
    } else if (!Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 120.0f, 3.0f)) {
        func_8002F948(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);

    } else {
        func_8002F994(&self->dyna.actor, self->timer);
    }
}

void BgYdanHasi_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanHasi* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgYdanHasi_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* dLists[] = { gDTSlidingPlatformDL, gDTWaterPlaneDL, gDTRisingPlatformsDL };
    BgYdanHasi* self = THIS;

    if (self->dyna.actor.params == HASI_WATER_BLOCK || self->dyna.actor.params == HASI_THREE_BLOCKS) {
        Gfx_DrawDListOpa(globalCtx, dLists[self->dyna.actor.params]);
    } else {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_ydan_hasi.c", 577);

        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, -globalCtx->gameplayFrames % 128,
                                    globalCtx->gameplayFrames % 128, 0x20, 0x20, 1, globalCtx->gameplayFrames % 128,
                                    globalCtx->gameplayFrames % 128, 0x20, 0x20));
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_ydan_hasi.c", 592),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gDTWaterPlaneDL);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_ydan_hasi.c", 597);
    }
}
