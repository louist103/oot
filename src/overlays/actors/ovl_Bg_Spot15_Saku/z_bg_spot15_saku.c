/*
 * File: z_bg_spot15_saku.c
 * Overlay: ovl_Bg_Spot15_Saku
 * Description: Hyrule Castle Gate
 */

#include "z_bg_spot15_saku.h"
#include "objects/object_spot15_obj/object_spot15_obj.h"

#define FLAGS 0x00000000

#define THIS ((BgSpot15Saku*)thisx)

void BgSpot15Saku_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot15Saku_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot15Saku_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot15Saku_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808B4930(BgSpot15Saku* self, GlobalContext* globalCtx);
void func_808B4978(BgSpot15Saku* self, GlobalContext* globalCtx);
void func_808B4A04(BgSpot15Saku* self, GlobalContext* globalCtx);

const ActorInit Bg_Spot15_Saku_InitVars = {
    ACTOR_BG_SPOT15_SAKU,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_SPOT15_OBJ,
    sizeof(BgSpot15Saku),
    (ActorFunc)BgSpot15Saku_Init,
    (ActorFunc)BgSpot15Saku_Destroy,
    (ActorFunc)BgSpot15Saku_Update,
    (ActorFunc)BgSpot15Saku_Draw,
};

void BgSpot15Saku_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgSpot15Saku* self = THIS;
    s32 pad2;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gLonLonCorralFenceCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    self->dyna.actor.scale.x = 0.1f;
    self->dyna.actor.scale.y = 0.1f;
    self->dyna.actor.scale.z = 0.1f;
    self->unk_170.x = self->dyna.actor.world.pos.x;
    self->unk_170.y = self->dyna.actor.world.pos.y;
    self->unk_170.z = self->dyna.actor.world.pos.z;
    if (gSaveContext.infTable[7] & 2) {
        self->dyna.actor.world.pos.z = 2659.0f;
    }
    self->actionFunc = func_808B4930;
}

void BgSpot15Saku_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot15Saku* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808B4930(BgSpot15Saku* self, GlobalContext* globalCtx) {
    if (self->unk_168 && !(gSaveContext.infTable[7] & 2)) {
        self->timer = 2;
        self->actionFunc = func_808B4978;
    }
}

void func_808B4978(BgSpot15Saku* self, GlobalContext* globalCtx) {
    if (self->timer == 0) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_METALGATE_OPEN - SFX_FLAG);
        self->dyna.actor.world.pos.z -= 2.0f;
        if (self->dyna.actor.world.pos.z < 2660.0f) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BRIDGE_OPEN_STOP);
            self->timer = 30;
            self->actionFunc = func_808B4A04;
        }
    }
}

void func_808B4A04(BgSpot15Saku* self, GlobalContext* globalCtx) {
    if (self->timer == 0) {
        self->unk_168 = 0;
        self->actionFunc = func_808B4930;
    }
}

void BgSpot15Saku_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot15Saku* self = THIS;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actionFunc(self, globalCtx);
}

void BgSpot15Saku_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_spot15_saku.c", 259);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_spot15_saku.c", 263),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gLonLonCorralFenceDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_spot15_saku.c", 268);
}
