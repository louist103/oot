/*
 * File: z_bg_spot05_soko.c
 * Overlay: ovl_Bg_Spot05_Soko
 * Description: Sacred Forest Meadow Entities
 */

#include "z_bg_spot05_soko.h"
#include "objects/object_spot05_objects/object_spot05_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgSpot05Soko*)thisx)

void BgSpot05Soko_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot05Soko_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot05Soko_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot05Soko_Draw(Actor* thisx, GlobalContext* globalCtx);
void func_808AE5A8(BgSpot05Soko* self, GlobalContext* globalCtx);
void func_808AE5B4(BgSpot05Soko* self, GlobalContext* globalCtx);
void func_808AE630(BgSpot05Soko* self, GlobalContext* globalCtx);

const ActorInit Bg_Spot05_Soko_InitVars = {
    ACTOR_BG_SPOT05_SOKO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT05_OBJECTS,
    sizeof(BgSpot05Soko),
    (ActorFunc)BgSpot05Soko_Init,
    (ActorFunc)BgSpot05Soko_Destroy,
    (ActorFunc)BgSpot05Soko_Update,
    (ActorFunc)BgSpot05Soko_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static Gfx* sDLists[] = {
    object_spot05_objects_DL_000840,
    object_spot05_objects_DL_001190,
};

void BgSpot05Soko_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad1;
    BgSpot05Soko* self = THIS;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    Actor_ProcessInitChain(thisx, sInitChain);
    self->switchFlag = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    if (thisx->params == 0) {
        CollisionHeader_GetVirtual(&object_spot05_objects_Col_000918, &colHeader);
        if (LINK_IS_ADULT) {
            Actor_Kill(thisx);
        } else {
            self->actionFunc = func_808AE5A8;
        }
    } else {
        CollisionHeader_GetVirtual(&object_spot05_objects_Col_0012C0, &colHeader);
        if (Flags_GetSwitch(globalCtx, self->switchFlag) != 0) {
            Actor_Kill(thisx);
        } else {
            self->actionFunc = func_808AE5B4;
            thisx->flags |= 0x10;
        }
    }
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
}

void BgSpot05Soko_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot05Soko* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808AE5A8(BgSpot05Soko* self, GlobalContext* globalCtx) {
}

void func_808AE5B4(BgSpot05Soko* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
        Audio_PlaySoundAtPosition(globalCtx, &self->dyna.actor.world.pos, 30, NA_SE_EV_METALDOOR_CLOSE);
        Actor_SetFocus(&self->dyna.actor, 50.0f);
        OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        self->actionFunc = func_808AE630;
        self->dyna.actor.speedXZ = 0.5f;
    }
}

void func_808AE630(BgSpot05Soko* self, GlobalContext* globalCtx) {
    self->dyna.actor.speedXZ *= 1.5f;
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 120.0f, self->dyna.actor.speedXZ) !=
        0) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgSpot05Soko_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot05Soko* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgSpot05Soko_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, sDLists[thisx->params]);
}
