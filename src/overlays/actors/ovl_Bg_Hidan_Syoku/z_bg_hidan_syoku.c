/*
 * File: z_bg_hidan_syoku.c
 * Overlay: ovl_Bg_Hidan_Syoku
 * Description: Stone Elevator in the Fire Temple
 */

#include "z_bg_hidan_syoku.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgHidanSyoku*)thisx)

void BgHidanSyoku_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanSyoku_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanSyoku_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanSyoku_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8088F4B8(BgHidanSyoku* self, GlobalContext* globalCtx);
void func_8088F514(BgHidanSyoku* self, GlobalContext* globalCtx);
void func_8088F62C(BgHidanSyoku* self, GlobalContext* globalCtx);

const ActorInit Bg_Hidan_Syoku_InitVars = {
    ACTOR_BG_HIDAN_SYOKU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanSyoku),
    (ActorFunc)BgHidanSyoku_Init,
    (ActorFunc)BgHidanSyoku_Destroy,
    (ActorFunc)BgHidanSyoku_Update,
    (ActorFunc)BgHidanSyoku_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHidanSyoku_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHidanSyoku* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gFireTempleFlareDancerPlatformCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    self->actionFunc = func_8088F4B8;
    self->dyna.actor.home.pos.y += 540.0f;
}

void BgHidanSyoku_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanSyoku* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_8088F47C(BgHidanSyoku* self) {
    self->timer = 60;
    Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
    self->actionFunc = func_8088F62C;
}

void func_8088F4B8(BgHidanSyoku* self, GlobalContext* globalCtx) {
    if (Flags_GetClear(globalCtx, self->dyna.actor.room) && func_8004356C(&self->dyna)) {
        self->timer = 140;
        self->actionFunc = func_8088F514;
    }
}

void func_8088F514(BgHidanSyoku* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    self->dyna.actor.world.pos.y = (cosf(self->timer * (M_PI / 140)) * 540.0f) + self->dyna.actor.home.pos.y;
    if (self->timer == 0) {
        func_8088F47C(self);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
}

void func_8088F5A0(BgHidanSyoku* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - (cosf(self->timer * (M_PI / 140)) * 540.0f);
    if (self->timer == 0) {
        func_8088F47C(self);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
}

void func_8088F62C(BgHidanSyoku* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->timer = 140;
        if (self->dyna.actor.world.pos.y < self->dyna.actor.home.pos.y) {
            self->actionFunc = func_8088F514;
        } else {
            self->actionFunc = func_8088F5A0;
        }
    }
}

void BgHidanSyoku_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanSyoku* self = THIS;

    self->actionFunc(self, globalCtx);
    if (func_8004356C(&self->dyna)) {
        if (self->unk_168 == 0) {
            self->unk_168 = 3;
        }
        Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_HIDAN1);
    } else if (!func_8004356C(&self->dyna)) {
        if (self->unk_168 != 0) {
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON0);
        }
        self->unk_168 = 0;
    }
}

void BgHidanSyoku_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gFireTempleFlareDancerPlatformDL);
}
