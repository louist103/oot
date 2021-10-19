/*
 * File: z_bg_gate_shutter.c
 * Overlay: Bg_Gate_Shutter
 * Description: Death Mountain Trail Gate
 */

#include "z_bg_gate_shutter.h"
#include "objects/object_spot01_matoyab/object_spot01_matoyab.h"
#include "vt.h"

#define FLAGS 0x00000000

#define THIS ((BgGateShutter*)thisx)

void BgGateShutter_Init(Actor* thisx, GlobalContext* globalCtx);
void BgGateShutter_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgGateShutter_Update(Actor* thisx, GlobalContext* globalCtx);
void BgGateShutter_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8087828C(BgGateShutter* self, GlobalContext* globalCtx);
void func_80878300(BgGateShutter* self, GlobalContext* globalCtx);
void func_808783AC(BgGateShutter* self, GlobalContext* globalCtx);
void func_808783D4(BgGateShutter* self, GlobalContext* globalCtx);

const ActorInit Bg_Gate_Shutter_InitVars = {
    ACTOR_BG_GATE_SHUTTER,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_SPOT01_MATOYAB,
    sizeof(BgGateShutter),
    (ActorFunc)BgGateShutter_Init,
    (ActorFunc)BgGateShutter_Destroy,
    (ActorFunc)BgGateShutter_Update,
    (ActorFunc)BgGateShutter_Draw,
};

void BgGateShutter_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgGateShutter* self = THIS;
    s32 pad[2];
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gKakarikoGuardGateCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    self->somePos.x = thisx->world.pos.x;
    self->somePos.y = thisx->world.pos.y;
    self->somePos.z = thisx->world.pos.z;
    if (((gSaveContext.infTable[7] & 0x40) || (gSaveContext.eventChkInf[4] & 0x20)) &&
        (globalCtx->sceneNum == SCENE_SPOT01)) {
        thisx->world.pos.x = -89.0f;
        thisx->world.pos.z = -1375.0f;
    }
    thisx->scale.x = 1.0f;
    thisx->scale.y = 1.0f;
    thisx->scale.z = 1.0f;
    osSyncPrintf("\n\n");
    osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 柵でたなぁ ☆☆☆☆☆ \n" VT_RST);
    self->actionFunc = func_8087828C;
}

void BgGateShutter_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgGateShutter* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_8087828C(BgGateShutter* self, GlobalContext* globalCtx) {
    if (self->openingState == 1 && !(gSaveContext.infTable[7] & 0x40)) {
        self->unk_178 = 2;
        self->actionFunc = func_80878300;
    } else if (self->openingState == 2) {
        self->unk_178 = 2;
        self->actionFunc = func_80878300;
    } else if (self->openingState < 0) {
        self->unk_178 = 2;
        self->actionFunc = func_808783D4;
    }
}

void func_80878300(BgGateShutter* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;

    if (self->unk_178 == 0) {
        Audio_PlayActorSound2(thisx, NA_SE_EV_METALGATE_OPEN - SFX_FLAG);
        thisx->world.pos.x -= 2.0f;
        Math_ApproachF(&thisx->world.pos.z, -1375.0f, 0.8f, 0.3f);
        if (thisx->world.pos.x < -89.0f) {
            Audio_PlayActorSound2(thisx, NA_SE_EV_BRIDGE_OPEN_STOP);
            self->unk_178 = 0x1E;
            self->actionFunc = func_808783AC;
        }
    }
}

void func_808783AC(BgGateShutter* self, GlobalContext* globalCtx) {
    if (self->unk_178 == 0) {
        self->openingState = 0;
        self->actionFunc = func_8087828C;
    }
}

void func_808783D4(BgGateShutter* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;

    if (self->unk_178 == 0) {
        Audio_PlayActorSound2(thisx, NA_SE_EV_METALGATE_OPEN - SFX_FLAG);
        thisx->world.pos.x += 2.0f;
        Math_ApproachF(&thisx->world.pos.z, -1350.0f, 0.8f, 0.3f);
        if (thisx->world.pos.x > 90.0f) {
            thisx->world.pos.x = 91.0f;
            Audio_PlayActorSound2(thisx, NA_SE_EV_BRIDGE_OPEN_STOP);
            self->unk_178 = 30;
            self->actionFunc = func_808783AC;
        }
    }
}

void BgGateShutter_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgGateShutter* self = THIS;

    if (self->unk_178 != 0) {
        self->unk_178 -= 1;
    }
    self->actionFunc(self, globalCtx);
}

void BgGateShutter_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_gate_shutter.c", 323);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_gate_shutter.c", 328),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gKakarikoGuardGateDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_gate_shutter.c", 333);
}
