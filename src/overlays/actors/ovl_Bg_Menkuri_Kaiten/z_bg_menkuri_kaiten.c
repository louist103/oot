/*
 * File: z_bg_menkuri_kaiten.c
 * Overlay: Bg_Menkuri_Kaiten
 * Description: Large rotating stone ring used in Gerudo Training Grounds and Forest Temple.
 */

#include "z_bg_menkuri_kaiten.h"
#include "objects/object_menkuri_objects/object_menkuri_objects.h"

#define FLAGS 0x00000030

#define THIS ((BgMenkuriKaiten*)thisx)

void BgMenkuriKaiten_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMenkuriKaiten_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMenkuriKaiten_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMenkuriKaiten_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Bg_Menkuri_Kaiten_InitVars = {
    ACTOR_BG_MENKURI_KAITEN,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MENKURI_OBJECTS,
    sizeof(BgMenkuriKaiten),
    (ActorFunc)BgMenkuriKaiten_Init,
    (ActorFunc)BgMenkuriKaiten_Destroy,
    (ActorFunc)BgMenkuriKaiten_Update,
    (ActorFunc)BgMenkuriKaiten_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

extern Gfx gGTGRotatingRingPlatformDL[];
extern CollisionHeader gGTGRotatingRingPlatformCol;

void BgMenkuriKaiten_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgMenkuriKaiten* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK3);
    CollisionHeader_GetVirtual(&gGTGRotatingRingPlatformCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
}

void BgMenkuriKaiten_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgMenkuriKaiten* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgMenkuriKaiten_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMenkuriKaiten* self = THIS;

    if (!Flags_GetSwitch(globalCtx, self->dyna.actor.params) && func_80043590(&self->dyna)) {
        func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
        self->dyna.actor.shape.rot.y += 0x80;
    }
}

void BgMenkuriKaiten_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gGTGRotatingRingPlatformDL);
}
