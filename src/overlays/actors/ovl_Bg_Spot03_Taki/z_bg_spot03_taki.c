/*
 * File: z_bg_spot03_taki.c
 * Overlay: ovl_Bg_Spot03_Taki
 * Description: Zora's River Waterfall
 */

#include "z_bg_spot03_taki.h"
#include "objects/object_spot03_object/object_spot03_object.h"

#define FLAGS 0x00000030

#define THIS ((BgSpot03Taki*)thisx)

void BgSpot03Taki_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot03Taki_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot03Taki_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot03Taki_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808ADEF0(BgSpot03Taki* self, GlobalContext* globalCtx);

const ActorInit Bg_Spot03_Taki_InitVars = {
    ACTOR_BG_SPOT03_TAKI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT03_OBJECT,
    sizeof(BgSpot03Taki),
    (ActorFunc)BgSpot03Taki_Init,
    (ActorFunc)BgSpot03Taki_Destroy,
    (ActorFunc)BgSpot03Taki_Update,
    (ActorFunc)BgSpot03Taki_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgSpot03Taki_ApplyOpeningAlpha(BgSpot03Taki* self, s32 bufferIndex) {
    s32 i;
    Vtx* vtx = (bufferIndex == 0) ? SEGMENTED_TO_VIRTUAL(object_spot03_object_Vtx_000800)
                                  : SEGMENTED_TO_VIRTUAL(object_spot03_object_Vtx_000990);

    for (i = 0; i < 5; i++) {
        vtx[i + 10].v.cn[3] = self->openingAlpha;
    }
}

void BgSpot03Taki_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot03Taki* self = THIS;
    s16 pad;
    CollisionHeader* colHeader = NULL;

    self->switchFlag = (self->dyna.actor.params & 0x3F);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&object_spot03_object_Col_000C98, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->bufferIndex = 0;
    self->openingAlpha = 255.0f;
    BgSpot03Taki_ApplyOpeningAlpha(self, 0);
    BgSpot03Taki_ApplyOpeningAlpha(self, 1);
    self->actionFunc = func_808ADEF0;
}

void BgSpot03Taki_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot03Taki* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808ADEF0(BgSpot03Taki* self, GlobalContext* globalCtx) {
    if (self->state == WATERFALL_CLOSED) {
        if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
            self->state = WATERFALL_OPENING_ANIMATED;
            self->timer = 40;
            OnePointCutscene_Init(globalCtx, 4100, -99, NULL, MAIN_CAM);
        }
    } else if (self->state == WATERFALL_OPENING_IDLE) {
        self->timer--;
        if (self->timer < 0) {
            self->state = WATERFALL_OPENING_ANIMATED;
        }
    } else if (self->state == WATERFALL_OPENING_ANIMATED) {
        if (self->openingAlpha > 0) {
            self->openingAlpha -= 5;
            if (self->openingAlpha <= 0.0f) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                self->timer = 400;
                self->state = WATERFALL_OPENED;
                self->openingAlpha = 0;
            }
        }
    } else if (self->state == WATERFALL_OPENED) {
        self->timer--;
        if (self->timer < 0) {
            self->state = WATERFALL_CLOSING;
        }
    } else if (self->state == WATERFALL_CLOSING) {
        if (self->openingAlpha < 255.0f) {
            self->openingAlpha += 5.0f;
            if (self->openingAlpha >= 255.0f) {
                func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                self->state = WATERFALL_CLOSED;
                self->openingAlpha = 255.0f;
                Flags_UnsetSwitch(globalCtx, self->switchFlag);
            }
        }
    }

    BgSpot03Taki_ApplyOpeningAlpha(self, self->bufferIndex);
}

void BgSpot03Taki_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot03Taki* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgSpot03Taki_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot03Taki* self = THIS;
    s32 pad;
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_spot03_taki.c", 321);

    gameplayFrames = globalCtx->gameplayFrames;

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_spot03_taki.c", 325),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(
        POLY_XLU_DISP++, 0x08,
        Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, gameplayFrames * 5, 64, 64, 1, 0, gameplayFrames * 5, 64, 64));

    gSPDisplayList(POLY_XLU_DISP++, object_spot03_object_DL_000B20);

    if (self->bufferIndex == 0) {
        gSPVertex(POLY_XLU_DISP++, object_spot03_object_Vtx_000800, 25, 0);
    } else {
        gSPVertex(POLY_XLU_DISP++, object_spot03_object_Vtx_000990, 25, 0);
    }

    gSPDisplayList(POLY_XLU_DISP++, object_spot03_object_DL_000BC0);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, gameplayFrames * 1, gameplayFrames * 3, 64, 64, 1,
                                -gameplayFrames, gameplayFrames * 3, 64, 64));

    gSPDisplayList(POLY_XLU_DISP++, object_spot03_object_DL_001580);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_spot03_taki.c", 358);

    self->bufferIndex = self->bufferIndex == 0;

    if (self->state >= WATERFALL_OPENING_IDLE && self->state <= WATERFALL_OPENED) {
        Audio_PlaySoundWaterfall(&self->dyna.actor.projectedPos, 0.5f);
    } else {
        Audio_PlaySoundWaterfall(&self->dyna.actor.projectedPos, 1.0f);
    }
}
