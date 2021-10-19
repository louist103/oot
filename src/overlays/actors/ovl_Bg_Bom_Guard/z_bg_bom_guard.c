/*
 * File: z_bg_bom_guard.c
 * Overlay: Bg_Bom_Guard
 * Description: Bombchu Bowling Alley Walls
 */

#include "z_bg_bom_guard.h"
#include "overlays/actors/ovl_En_Bom_Bowl_Man/z_en_bom_bowl_man.h"
#include "objects/object_bowl/object_bowl.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((BgBomGuard*)thisx)

void BgBomGuard_Init(Actor* thisx, GlobalContext* globalCtx);
void BgBomGuard_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgBomGuard_Update(Actor* thisx, GlobalContext* globalCtx);

void func_8086E638(BgBomGuard* self, GlobalContext* globalCtx);

const ActorInit Bg_Bom_Guard_InitVars = {
    ACTOR_BG_BOM_GUARD,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BOWL,
    sizeof(BgBomGuard),
    (ActorFunc)BgBomGuard_Init,
    (ActorFunc)BgBomGuard_Destroy,
    (ActorFunc)BgBomGuard_Update,
    NULL,
};

void BgBomGuard_SetupAction(BgBomGuard* self, BgBomGuardActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgBomGuard_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgBomGuard* self = THIS;
    s32 pad[2];
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gBowlingDefaultCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);

    osSyncPrintf("\n\n");
    osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 透明ガード出現 ☆☆☆☆☆ \n" VT_RST);

    thisx->scale.x = 1.0f;
    thisx->scale.y = 1.0f;
    thisx->scale.z = 1.0f;
    self->unk_16C = thisx->world.pos;
    BgBomGuard_SetupAction(self, func_8086E638);
}

void BgBomGuard_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgBomGuard* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_8086E638(BgBomGuard* self, GlobalContext* globalCtx) {
    Actor* it = globalCtx->actorCtx.actorLists[ACTORCAT_NPC].head;
    Actor* thisx = &self->dyna.actor;

    self->unk_168 = 0;

    while (it != 0) {
        if (it->id == ACTOR_EN_BOM_BOWL_MAN) {
            if ((((EnBomBowlMan*)it)->minigamePlayStatus != 0) && (fabsf(globalCtx->view.eye.x) > -20.0f) &&
                (fabsf(globalCtx->view.eye.y) > 110.0f)) {
                self->unk_168 = 1;
            }
            break;
        }
        it = it->next;
    }

    if (self->unk_168 == 0) {
        thisx->world.pos.y = sREG(64) + -200.0f;
    } else {
        thisx->world.pos.y = 0.0f;
    }
}

void BgBomGuard_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgBomGuard* self = THIS;

    self->actionFunc(self, globalCtx);
}
