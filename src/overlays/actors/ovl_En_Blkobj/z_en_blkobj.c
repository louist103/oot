/*
 * File: z_en_blkobj.c
 * Overlay: ovl_En_Blkobj
 * Description: Dark Link's Illusion Room
 */

#include "z_en_blkobj.h"
#include "objects/object_blkobj/object_blkobj.h"

#define FLAGS 0x00000030

#define THIS ((EnBlkobj*)thisx)

void EnBlkobj_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBlkobj_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBlkobj_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBlkobj_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnBlkobj_Wait(EnBlkobj* self, GlobalContext* globalCtx);
void EnBlkobj_SpawnDarkLink(EnBlkobj* self, GlobalContext* globalCtx);
void EnBlkobj_DarkLinkFight(EnBlkobj* self, GlobalContext* globalCtx);
void EnBlkobj_DoNothing(EnBlkobj* self, GlobalContext* globalCtx);

const ActorInit En_Blkobj_InitVars = {
    ACTOR_EN_BLKOBJ,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BLKOBJ,
    sizeof(EnBlkobj),
    (ActorFunc)EnBlkobj_Init,
    (ActorFunc)EnBlkobj_Destroy,
    (ActorFunc)EnBlkobj_Update,
    (ActorFunc)EnBlkobj_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 300, ICHAIN_STOP),
};

static Gfx sSetupOpaDL[] = {
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

static Gfx sSetupXluDL[] = {
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2),
    gsSPEndDisplayList(),
};

void EnBlkobj_SetupAction(EnBlkobj* self, EnBlkobjActionFunc actionFunc) {
    self->actionFunc = actionFunc;
    self->timer = 0;
}

void EnBlkobj_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnBlkobj* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    if (Flags_GetClear(globalCtx, self->dyna.actor.room)) {
        self->alpha = 255;
        EnBlkobj_SetupAction(self, EnBlkobj_DoNothing);
    } else {
        CollisionHeader_GetVirtual(&gIllusionRoomCol, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
        EnBlkobj_SetupAction(self, EnBlkobj_Wait);
    }
}

void EnBlkobj_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnBlkobj* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void EnBlkobj_Wait(EnBlkobj* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->dyna.actor.xzDistToPlayer < 120.0f) {
        EnBlkobj_SetupAction(self, EnBlkobj_SpawnDarkLink);
    }
    player->stateFlags2 |= 0x04000000;
}

void EnBlkobj_SpawnDarkLink(EnBlkobj* self, GlobalContext* globalCtx) {
    if (!(self->dyna.actor.flags & 0x40)) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_TORCH2, self->dyna.actor.world.pos.x,
                    self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z, 0, self->dyna.actor.yawTowardsPlayer, 0,
                    0);
        EnBlkobj_SetupAction(self, EnBlkobj_DarkLinkFight);
    }
}

void EnBlkobj_DarkLinkFight(EnBlkobj* self, GlobalContext* globalCtx) {
    s32 alphaMod;

    if (self->timer == 0) {
        if (Actor_Find(&globalCtx->actorCtx, ACTOR_EN_TORCH2, ACTORCAT_BOSS) == NULL) {
            Flags_SetClear(globalCtx, self->dyna.actor.room);
            self->timer++;
        }
    } else if (self->timer++ > 100) {
        alphaMod = (self->timer - 100) >> 2;
        if (alphaMod > 5) {
            alphaMod = 5;
        }
        self->alpha += alphaMod;
        if (self->alpha > 255) {
            self->alpha = 255;
            EnBlkobj_SetupAction(self, EnBlkobj_DoNothing);
            DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        }
    }
}

void EnBlkobj_DoNothing(EnBlkobj* self, GlobalContext* globalCtx) {
}

void EnBlkobj_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnBlkobj* self = THIS;

    self->actionFunc(self, globalCtx);
}

void EnBlkobj_DrawAlpha(GlobalContext* globalCtx, Gfx* dList, s32 alpha) {
    Gfx* segment;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_blkobj.c", 322);

    if (alpha == 255) {
        segment = sSetupOpaDL;
    } else {
        segment = sSetupXluDL;
    }

    gSPSegment(POLY_XLU_DISP++, 0x08, segment);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, alpha);
    gSPDisplayList(POLY_XLU_DISP++, dList);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_blkobj.c", 330);
}

void EnBlkobj_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnBlkobj* self = THIS;
    s32 illusionAlpha;
    u32 gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_blkobj.c", 349);

    func_80093D84(globalCtx->state.gfxCtx);

    gameplayFrames = globalCtx->gameplayFrames % 128;

    gSPSegment(POLY_XLU_DISP++, 0x0D,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, gameplayFrames, 0, 32, 32, 1, gameplayFrames, 0, 32, 32));
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_blkobj.c", 363),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (self->alpha != 0) {
        EnBlkobj_DrawAlpha(globalCtx, gIllusionRoomNormalDL, self->alpha);
    }
    illusionAlpha = 255 - self->alpha;
    if (illusionAlpha != 0) {
        EnBlkobj_DrawAlpha(globalCtx, gIllusionRoomIllusionDL, illusionAlpha);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_blkobj.c", 375);
}
