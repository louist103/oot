/*
 * File: z_bg_zg.c
 * Overlay: ovl_Bg_Zg
 * Description: Metal bars (Ganon's Castle)
 */

#include "z_bg_zg.h"
#include "objects/object_zg/object_zg.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((BgZg*)thisx)

void BgZg_Init(Actor* thisx, GlobalContext* globalCtx);
void BgZg_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgZg_Update(Actor* thisx, GlobalContext* globalCtx);
void BgZg_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808C0C50(BgZg* self);
s32 func_808C0C98(BgZg* self, GlobalContext* globalCtx);
s32 func_808C0CC8(BgZg* self);
void func_808C0CD4(BgZg* self, GlobalContext* globalCtx);
void func_808C0D08(BgZg* self, GlobalContext* globalCtx);
void func_808C0EEC(BgZg* self, GlobalContext* globalCtx);

static BgZgActionFunc sActionFuncs[] = {
    func_808C0CD4,
    func_808C0D08,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

static BgZgDrawFunc sDrawFuncs[] = {
    func_808C0EEC,
};

const ActorInit Bg_Zg_InitVars = {
    ACTOR_BG_ZG,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZG,
    sizeof(BgZg),
    (ActorFunc)BgZg_Init,
    (ActorFunc)BgZg_Destroy,
    (ActorFunc)BgZg_Update,
    (ActorFunc)BgZg_Draw,
};

void BgZg_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgZg* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808C0C50(BgZg* self) {
    Audio_PlaySoundGeneral(NA_SE_EV_METALDOOR_OPEN, &self->dyna.actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);
}

s32 func_808C0C98(BgZg* self, GlobalContext* globalCtx) {
    s32 flag = (self->dyna.actor.params >> 8) & 0xFF;

    return Flags_GetSwitch(globalCtx, flag);
}

s32 func_808C0CC8(BgZg* self) {
    s32 flag = self->dyna.actor.params & 0xFF;

    return flag;
}

void func_808C0CD4(BgZg* self, GlobalContext* globalCtx) {
    if (func_808C0C98(self, globalCtx) != 0) {
        self->action = 1;
        func_808C0C50(self);
    }
}

void func_808C0D08(BgZg* self, GlobalContext* globalCtx) {
    self->dyna.actor.world.pos.y += (kREG(16) + 20.0f) * 1.2f;
    if ((((kREG(17) + 200.0f) * 1.2f) + self->dyna.actor.home.pos.y) <= self->dyna.actor.world.pos.y) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgZg_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgZg* self = THIS;
    s32 action = self->action;

    if (((action < 0) || (1 < action)) || (sActionFuncs[action] == NULL)) {
        // "Main Mode is wrong!!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sActionFuncs[action](self, globalCtx);
    }
}

void BgZg_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad[2];
    BgZg* self = THIS;
    CollisionHeader* colHeader;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    colHeader = NULL;
    CollisionHeader_GetVirtual(&gTowerCollapseBarsCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if ((func_808C0CC8(self) == 8) || (func_808C0CC8(self) == 9)) {
        self->dyna.actor.scale.x = self->dyna.actor.scale.x * 1.3f;
        self->dyna.actor.scale.z = self->dyna.actor.scale.z * 1.3f;
        self->dyna.actor.scale.y = self->dyna.actor.scale.y * 1.2f;
    }

    self->action = 0;
    self->drawConfig = 0;
    if (func_808C0C98(self, globalCtx)) {
        Actor_Kill(&self->dyna.actor);
    }
}

void func_808C0EEC(BgZg* self, GlobalContext* globalCtx) {
    GraphicsContext* localGfxCtx = globalCtx->state.gfxCtx;

    OPEN_DISPS(localGfxCtx, "../z_bg_zg.c", 311);

    func_80093D18(localGfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(localGfxCtx, "../z_bg_zg.c", 315),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gTowerCollapseBarsDL);

    CLOSE_DISPS(localGfxCtx, "../z_bg_zg.c", 320);
}

void BgZg_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgZg* self = THIS;
    s32 drawConfig = self->drawConfig;

    if (((drawConfig < 0) || (drawConfig > 0)) || sDrawFuncs[drawConfig] == NULL) {
        // "Drawing mode is wrong !!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sDrawFuncs[drawConfig](self, globalCtx);
    }
}
