/*
 * File: z_bg_mizu_uzu.c
 * Overlay: ovl_Bg_Mizu_Uzu
 * Description: Water Noise
 */

#include "z_bg_mizu_uzu.h"
#include "objects/object_mizu_objects/object_mizu_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgMizuUzu*)thisx)

void BgMizuUzu_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMizuUzu_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMizuUzu_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMizuUzu_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8089F788(BgMizuUzu* self, GlobalContext* globalCtx);

const ActorInit Bg_Mizu_Uzu_InitVars = {
    ACTOR_BG_MIZU_UZU,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuUzu),
    (ActorFunc)BgMizuUzu_Init,
    (ActorFunc)BgMizuUzu_Destroy,
    (ActorFunc)BgMizuUzu_Update,
    (ActorFunc)BgMizuUzu_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgMizuUzu_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuUzu* self = THIS;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gObjectMizuObjectsUzuCol_0074EC, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    self->actionFunc = func_8089F788;
}

void BgMizuUzu_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgMizuUzu* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_8089F788(BgMizuUzu* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;

    if (GET_PLAYER(globalCtx)->currentBoots == PLAYER_BOOTS_IRON) {
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    } else {
        func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
    Audio_PlayActorSound2(thisx, NA_SE_EV_WATER_CONVECTION - SFX_FLAG);
    thisx->shape.rot.y += 0x1C0;
}

void BgMizuUzu_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMizuUzu* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgMizuUzu_Draw(Actor* thisx, GlobalContext* globalCtx) {
}
