/*
 * File: z_bg_mjin.c
 * Overlay: ovl_Bg_Mjin
 * Description: Warp Pad
 */

#include "z_bg_mjin.h"
#include "objects/object_mjin/object_mjin.h"
#include "objects/object_mjin_wind/object_mjin_wind.h"
#include "objects/object_mjin_soul/object_mjin_soul.h"
#include "objects/object_mjin_dark/object_mjin_dark.h"
#include "objects/object_mjin_ice/object_mjin_ice.h"
#include "objects/object_mjin_flame/object_mjin_flame.h"
#include "objects/object_mjin_flash/object_mjin_flash.h"
#include "objects/object_mjin_oka/object_mjin_oka.h"

#define FLAGS 0x00000010

#define THIS ((BgMjin*)thisx)

void BgMjin_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMjin_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMjin_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMjin_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808A0850(BgMjin* self, GlobalContext* globalCtx);
void BgMjin_DoNothing(BgMjin* self, GlobalContext* globalCtx);

const ActorInit Bg_Mjin_InitVars = {
    ACTOR_BG_MJIN,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgMjin),
    (ActorFunc)BgMjin_Init,
    (ActorFunc)BgMjin_Destroy,
    (ActorFunc)BgMjin_Update,
    NULL,
};

extern UNK_TYPE D_06000000;

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 400, ICHAIN_STOP),
};

static s16 sObjectIDs[] = { OBJECT_MJIN_FLASH, OBJECT_MJIN_DARK, OBJECT_MJIN_FLAME,
                            OBJECT_MJIN_ICE,   OBJECT_MJIN_SOUL, OBJECT_MJIN_WIND };

void BgMjin_SetupAction(BgMjin* self, BgMjinActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgMjin_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgMjin* self = THIS;
    s8 objBankIndex;

    Actor_ProcessInitChain(thisx, sInitChain);
    objBankIndex = Object_GetIndex(&globalCtx->objectCtx, (thisx->params != 0 ? OBJECT_MJIN : OBJECT_MJIN_OKA));
    self->objBankIndex = objBankIndex;
    if (objBankIndex < 0) {
        Actor_Kill(thisx);
    } else {
        BgMjin_SetupAction(self, func_808A0850);
    }
}

void BgMjin_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgMjin* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808A0850(BgMjin* self, GlobalContext* globalCtx) {
    CollisionHeader* colHeader;
    CollisionHeader* collision;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->objBankIndex)) {
        colHeader = NULL;
        self->dyna.actor.flags &= ~0x10;
        self->dyna.actor.objBankIndex = self->objBankIndex;
        Actor_SetObjectDependency(globalCtx, &self->dyna.actor);
        DynaPolyActor_Init(&self->dyna, 0);
        collision = self->dyna.actor.params != 0 ? &gWarpPadCol : &gOcarinaWarpPadCol;
        CollisionHeader_GetVirtual(collision, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
        BgMjin_SetupAction(self, BgMjin_DoNothing);
        self->dyna.actor.draw = BgMjin_Draw;
    }
}

void BgMjin_DoNothing(BgMjin* self, GlobalContext* globalCtx) {
}

void BgMjin_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMjin* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgMjin_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgMjin* self = THIS;
    u32 dlist;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mjin.c", 250);

    if (thisx->params != 0) {
        // thisx is required
        s32 objBankIndex = Object_GetIndex(&globalCtx->objectCtx, sObjectIDs[thisx->params - 1]);
        if (objBankIndex >= 0) {
            gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[objBankIndex].segment);
        }
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(&D_06000000));
        dlist = gWarpPadBaseDL;
    } else {
        dlist = gOcarinaWarpPadDL;
    }

    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mjin.c", 285),
              G_MTX_NOPUSH | G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(POLY_OPA_DISP++, dlist);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mjin.c", 288);
}
