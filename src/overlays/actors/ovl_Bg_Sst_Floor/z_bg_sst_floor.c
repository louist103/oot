/*
 * File: z_bg_sst_floor.c
 * Overlay: ovl_Bg_Sst_Floor
 * Description: Bongo Bongo's drum
 */

#include "z_bg_sst_floor.h"
#include "objects/object_sst/object_sst.h"

#define FLAGS 0x00000030

#define THIS ((BgSstFloor*)thisx)

void BgSstFloor_Init(BgSstFloor* self, GlobalContext* globalCtx);
void BgSstFloor_Destroy(BgSstFloor* self, GlobalContext* globalCtx);
void BgSstFloor_Update(BgSstFloor* self, GlobalContext* globalCtx);
void BgSstFloor_Draw(BgSstFloor* self, GlobalContext* globalCtx);

static s32 sUnkValues[] = { 0, 0, 0 }; // Unused, probably a zero vector

const ActorInit Bg_Sst_Floor_InitVars = {
    ACTOR_BG_SST_FLOOR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SST,
    sizeof(BgSstFloor),
    (ActorFunc)BgSstFloor_Init,
    (ActorFunc)BgSstFloor_Destroy,
    (ActorFunc)BgSstFloor_Update,
    (ActorFunc)BgSstFloor_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale.x, 100, ICHAIN_STOP),
};

void BgSstFloor_Init(BgSstFloor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgSstFloor* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gBongoDrumCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
}

void BgSstFloor_Destroy(BgSstFloor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgSstFloor* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgSstFloor_Update(BgSstFloor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgSstFloor* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    CollisionHeader* colHeader = SEGMENTED_TO_VIRTUAL(&gBongoDrumCol);

    colHeader->vtxList = SEGMENTED_TO_VIRTUAL(colHeader->vtxList);

    if (1) {}

    if (func_80043590(&self->dyna) && (self->dyna.actor.yDistToPlayer < 1000.0f)) {
        Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_BOSS_SHADES);
    } else {
        Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON0);
    }

    if (func_8004356C(&self->dyna) && (player->fallDistance > 1000.0f)) {
        self->dyna.actor.params = 1;
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_SHADEST_TAIKO_HIGH);
    }

    if (self->dyna.actor.params == BONGOFLOOR_HIT) {
        Actor* item00 = globalCtx->actorCtx.actorLists[ACTORCAT_MISC].head;
        f32 distFromRim;
        f32 xzDist;

        self->drumAmp = 80;
        self->dyna.actor.params = BONGOFLOOR_REST;
        self->drumPhase = 28;

        if (func_8004356C(&self->dyna) && !(player->stateFlags1 & 0x6000)) {
            distFromRim = 600.0f - self->dyna.actor.xzDistToPlayer;
            if (distFromRim > 0.0f) {
                if (distFromRim > 350.0f) {
                    distFromRim = 350.0f;
                }
                player->actor.bgCheckFlags &= ~1;
                player->actor.velocity.y = 9.0f * distFromRim * (1.0f / 350.0f);
            }
        }

        while (item00 != NULL) {
            if ((item00->id == ACTOR_EN_ITEM00) && (item00->world.pos.y == 0.0f)) {
                xzDist = Actor_WorldDistXZToActor(&self->dyna.actor, item00);
                distFromRim = 600.0f - xzDist;
                if (xzDist < 600.0f) {
                    if (distFromRim > 350.0f) {
                        distFromRim = 350.0f;
                    }
                    item00->bgCheckFlags &= ~3;
                    item00->velocity.y = 9.0f * distFromRim * (1.0f / 350.0f);
                }
            }
            item00 = item00->next;
        }
    }
    self->drumHeight = sinf(self->drumPhase * (M_PI / 2)) * (-self->drumAmp);
    Math_StepToS(&self->drumAmp, 0, 5);

    colHeader->vtxList[1].y = colHeader->vtxList[0].y = colHeader->vtxList[2].y = colHeader->vtxList[3].y =
        colHeader->vtxList[4].y = colHeader->vtxList[7].y = colHeader->vtxList[9].y = colHeader->vtxList[11].y =
            colHeader->vtxList[13].y = self->dyna.actor.home.pos.y + self->drumHeight;

    if (self->drumPhase != 0) {
        self->drumPhase--;
    }
    if (1) {}
    func_8003EE6C(globalCtx, &globalCtx->colCtx.dyna);
}

void BgSstFloor_Draw(BgSstFloor* thisx, GlobalContext* globalCtx) {
    BgSstFloor* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_sst_floor.c", 277);
    func_80093D18(globalCtx->state.gfxCtx);
    Matrix_Scale(1.0f, self->drumHeight * -0.0025f, 1.0f, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_sst_floor.c", 283),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, gBongoDrumDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_sst_floor.c", 287);
}
