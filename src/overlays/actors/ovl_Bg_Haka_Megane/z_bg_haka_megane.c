/*
 * File: z_bg_haka_megane.c
 * Overlay: ovl_Bg_Haka_Megane
 * Description: Shadow Temple Fake Walls
 */

#include "z_bg_haka_megane.h"
#include "objects/object_hakach_objects/object_hakach_objects.h"

#define FLAGS 0x000000B0

#define THIS ((BgHakaMegane*)thisx)

void BgHakaMegane_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaMegane_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaMegane_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaMegane_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8087DB24(BgHakaMegane* self, GlobalContext* globalCtx);
void func_8087DBF0(BgHakaMegane* self, GlobalContext* globalCtx);
void BgHakaMegane_DoNothing(BgHakaMegane* self, GlobalContext* globalCtx);

const ActorInit Bg_Haka_Megane_InitVars = {
    ACTOR_BG_HAKA_MEGANE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHakaMegane),
    (ActorFunc)BgHakaMegane_Init,
    (ActorFunc)BgHakaMegane_Destroy,
    (ActorFunc)BgHakaMegane_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static CollisionHeader* sCollisionHeaders[] = {
    &gBotw1Col, &gBotw2Col, NULL,       0x06004330, 0x060044D0, NULL, 0x06004780,
    0x06004940, NULL,       0x06004B00, NULL,       0x06004CC0, NULL,
};

static Gfx* sDLists[] = {
    gBotwFakeWallsAndFloorsDL,
    gBotwThreeFakeFloorsDL,
    gBotwHoleTrap2DL,
    0x060040F0,
    0x060043B0,
    0x06001120,
    0x060045A0,
    0x060047F0,
    0x060018F0,
    0x060049B0,
    0x06003CF0,
    0x06004B70,
    0x06002ED0,
};

void BgHakaMegane_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaMegane* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);

    if (thisx->params < 3) {
        self->objBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_HAKACH_OBJECTS);
    } else {
        self->objBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_HAKA_OBJECTS);
    }

    if (self->objBankIndex < 0) {
        Actor_Kill(thisx);
    } else {
        self->actionFunc = func_8087DB24;
    }
}

void BgHakaMegane_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaMegane* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_8087DB24(BgHakaMegane* self, GlobalContext* globalCtx) {
    CollisionHeader* colHeader;
    CollisionHeader* collision;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->objBankIndex)) {
        self->dyna.actor.objBankIndex = self->objBankIndex;
        self->dyna.actor.draw = BgHakaMegane_Draw;
        Actor_SetObjectDependency(globalCtx, &self->dyna.actor);
        if (globalCtx->roomCtx.curRoom.showInvisActors) {
            self->actionFunc = func_8087DBF0;
            collision = sCollisionHeaders[self->dyna.actor.params];
            if (collision != NULL) {
                CollisionHeader_GetVirtual(collision, &colHeader);
                self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
            }
        } else {
            self->actionFunc = BgHakaMegane_DoNothing;
        }
    }
}

void func_8087DBF0(BgHakaMegane* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;

    if (globalCtx->actorCtx.unk_03 != 0) {
        thisx->flags |= 0x80;
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    } else {
        thisx->flags &= ~0x80;
        func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
}

void BgHakaMegane_DoNothing(BgHakaMegane* self, GlobalContext* globalCtx) {
}

void BgHakaMegane_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaMegane* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgHakaMegane_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaMegane* self = THIS;

    if ((thisx->flags & 0x80) == 0x80) {
        Gfx_DrawDListXlu(globalCtx, sDLists[thisx->params]);
    } else {
        Gfx_DrawDListOpa(globalCtx, sDLists[thisx->params]);
    }

    if (thisx->params == 0) {
        Gfx_DrawDListXlu(globalCtx, gBotwBloodSplatterDL);
    }
}
