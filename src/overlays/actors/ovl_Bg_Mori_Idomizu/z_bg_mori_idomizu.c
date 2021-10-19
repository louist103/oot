/*
 * File: z_bg_mori_idomizu.c
 * Overlay: ovl_Bg_Mori_Idomizu
 * Description: Square of water in Forest Temple well
 */

#include "z_bg_mori_idomizu.h"
#include "objects/object_mori_objects/object_mori_objects.h"

#define FLAGS 0x00000030

#define THIS ((BgMoriIdomizu*)thisx)

void BgMoriIdomizu_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMoriIdomizu_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMoriIdomizu_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMoriIdomizu_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMoriIdomizu_SetupWaitForMoriTex(BgMoriIdomizu* self);
void BgMoriIdomizu_WaitForMoriTex(BgMoriIdomizu* self, GlobalContext* globalCtx);
void BgMoriIdomizu_SetupMain(BgMoriIdomizu* self);
void BgMoriIdomizu_Main(BgMoriIdomizu* self, GlobalContext* globalCtx);

static s16 sIsSpawned = false;

const ActorInit Bg_Mori_Idomizu_InitVars = {
    ACTOR_BG_MORI_IDOMIZU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriIdomizu),
    (ActorFunc)BgMoriIdomizu_Init,
    (ActorFunc)BgMoriIdomizu_Destroy,
    (ActorFunc)BgMoriIdomizu_Update,
    NULL,
};

void BgMoriIdomizu_SetupAction(BgMoriIdomizu* self, BgMoriIdomizuActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgMoriIdomizu_SetWaterLevel(GlobalContext* globalCtx, s16 waterLevel) {
    WaterBox* waterBox = globalCtx->colCtx.colHeader->waterBoxes;

    waterBox[2].ySurface = waterLevel;
    waterBox[3].ySurface = waterLevel;
    waterBox[4].ySurface = waterLevel;
}

void BgMoriIdomizu_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriIdomizu* self = THIS;

    if (sIsSpawned) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actor.scale.x = 1.1f;
    self->actor.scale.y = 1.0f;
    self->actor.scale.z = 1.0f;
    self->actor.world.pos.x = 119.0f;
    self->actor.world.pos.z = -1820.0f;
    self->prevSwitchFlagSet = Flags_GetSwitch(globalCtx, self->actor.params & 0x3F);
    if (self->prevSwitchFlagSet != 0) {
        self->actor.world.pos.y = -282.0f;
        BgMoriIdomizu_SetWaterLevel(globalCtx, -282);
    } else {
        self->actor.world.pos.y = 184.0f;
        BgMoriIdomizu_SetWaterLevel(globalCtx, 184);
    }
    self->moriTexObjIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjIndex < 0) {
        Actor_Kill(&self->actor);
        // "Bank danger!"
        osSyncPrintf("Error : バンク危険！(arg_data 0x%04x)(%s %d)\n", self->actor.params, "../z_bg_mori_idomizu.c",
                     202);
        return;
    }
    BgMoriIdomizu_SetupWaitForMoriTex(self);
    sIsSpawned = true;
    self->isLoaded = true;
    self->actor.room = -1;
    // "Forest Temple well water"
    osSyncPrintf("(森の神殿 井戸水)(arg_data 0x%04x)\n", self->actor.params);
}

void BgMoriIdomizu_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriIdomizu* self = THIS;

    if (self->isLoaded) {
        sIsSpawned = false;
    }
}

void BgMoriIdomizu_SetupWaitForMoriTex(BgMoriIdomizu* self) {
    BgMoriIdomizu_SetupAction(self, BgMoriIdomizu_WaitForMoriTex);
}

void BgMoriIdomizu_WaitForMoriTex(BgMoriIdomizu* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->moriTexObjIndex)) {
        BgMoriIdomizu_SetupMain(self);
        self->actor.draw = BgMoriIdomizu_Draw;
    }
}

void BgMoriIdomizu_SetupMain(BgMoriIdomizu* self) {
    BgMoriIdomizu_SetupAction(self, BgMoriIdomizu_Main);
}

void BgMoriIdomizu_Main(BgMoriIdomizu* self, GlobalContext* globalCtx) {
    s8 roomNum;
    Actor* thisx = &self->actor;
    s32 switchFlagSet;

    roomNum = globalCtx->roomCtx.curRoom.num;
    switchFlagSet = Flags_GetSwitch(globalCtx, thisx->params & 0x3F);
    if (switchFlagSet) {
        self->targetWaterLevel = -282.0f;
    } else {
        self->targetWaterLevel = 184.0f;
    }
    if (switchFlagSet && !self->prevSwitchFlagSet) {
        OnePointCutscene_Init(globalCtx, 3240, 70, thisx, MAIN_CAM);
        self->drainTimer = 90;
    } else if (!switchFlagSet && self->prevSwitchFlagSet) {
        OnePointCutscene_Init(globalCtx, 3240, 70, thisx, MAIN_CAM);
        self->drainTimer = 90;
        thisx->world.pos.y = 0.0f;
    }
    self->drainTimer--;
    if ((roomNum == 7) || (roomNum == 8) || (roomNum == 9)) {
        if (self->drainTimer < 70) {
            Math_StepToF(&thisx->world.pos.y, self->targetWaterLevel, 3.5f);
            BgMoriIdomizu_SetWaterLevel(globalCtx, thisx->world.pos.y);
            if (self->drainTimer > 0) {
                if (switchFlagSet) {
                    func_800788CC(NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG);
                } else {
                    func_800788CC(NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG);
                }
            }
        }
    } else {
        thisx->world.pos.y = self->targetWaterLevel;
        BgMoriIdomizu_SetWaterLevel(globalCtx, thisx->world.pos.y);
        Actor_Kill(thisx);
        return;
    }
    self->prevSwitchFlagSet = switchFlagSet;
}

void BgMoriIdomizu_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriIdomizu* self = THIS;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
}

void BgMoriIdomizu_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriIdomizu* self = THIS;
    u32 gameplayFrames = globalCtx->gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_idomizu.c", 356);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_idomizu.c", 360),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPSegment(POLY_XLU_DISP++, 0x08, globalCtx->objectCtx.status[self->moriTexObjIndex].segment);

    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 128);

    gSPSegment(POLY_XLU_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0x7F - (gameplayFrames & 0x7F), gameplayFrames % 0x80, 0x20,
                                0x20, 1, gameplayFrames & 0x7F, gameplayFrames % 0x80, 0x20, 0x20));

    gSPDisplayList(POLY_XLU_DISP++, gMoriIdomizuWaterDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_idomizu.c", 382);
}
