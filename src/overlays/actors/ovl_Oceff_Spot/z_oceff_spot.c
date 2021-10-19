/*
 * File: z_oceff_spot.c
 * Overlay: ovl_Oceff_Spot
 * Description: Sun's Song Effect
 */

#include "z_oceff_spot.h"
#include "vt.h"

#define FLAGS 0x02000010

#define THIS ((OceffSpot*)thisx)

void OceffSpot_Init(Actor* thisx, GlobalContext* globalCtx);
void OceffSpot_Destroy(Actor* thisx, GlobalContext* globalCtx);
void OceffSpot_Update(Actor* thisx, GlobalContext* globalCtx);
void OceffSpot_Draw(Actor* thisx, GlobalContext* globalCtx);

void OceffSpot_GrowCylinder(OceffSpot* self, GlobalContext* globalCtx);

const ActorInit Oceff_Spot_InitVars = {
    ACTOR_OCEFF_SPOT,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(OceffSpot),
    (ActorFunc)OceffSpot_Init,
    (ActorFunc)OceffSpot_Destroy,
    (ActorFunc)OceffSpot_Update,
    (ActorFunc)OceffSpot_Draw,
};

#include "z_oceff_spot_gfx.c"

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1500, ICHAIN_STOP),
};

void OceffSpot_SetupAction(OceffSpot* self, OceffSpotActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void OceffSpot_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    OceffSpot* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    OceffSpot_SetupAction(self, OceffSpot_GrowCylinder);

    Lights_PointNoGlowSetInfo(&self->lightInfo1, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, 0, 0, 0, 0);
    self->lightNode1 = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo1);

    Lights_PointNoGlowSetInfo(&self->lightInfo2, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, 0, 0, 0, 0);
    self->lightNode2 = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo2);
    if (YREG(15)) {
        self->actor.scale.y = 2.4f;
    } else {
        self->actor.scale.y = 0.3f;
    }

    self->unk_174 = 0;
}

void OceffSpot_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    OceffSpot* self = THIS;
    Player* player = GET_PLAYER(globalCtx);

    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode1);
    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode2);
    func_800876C8(globalCtx);
    if ((gSaveContext.nayrusLoveTimer != 0) && (globalCtx->actorCtx.actorLists[ACTORCAT_PLAYER].length != 0)) {
        player->stateFlags3 |= 0x40;
    }
}

void OceffSpot_End(OceffSpot* self, GlobalContext* globalCtx) {
    if (self->unk_174 > 0) {
        self->unk_174 -= 0.05f;
    } else {
        Actor_Kill(&self->actor);
        if (gTimeIncrement != 400 && globalCtx->msgCtx.unk_E40E == 0 && (gSaveContext.eventInf[0] & 0xF) != 1) {
            if (globalCtx->msgCtx.unk_E3F0 != 0x31 || globalCtx->msgCtx.unk_E3EE != 8) {
                gSaveContext.sunsSongState = SUNSSONG_START;
                osSyncPrintf(VT_FGCOL(YELLOW));
                // "Sun's Song Flag"
                osSyncPrintf("z_oceff_spot  太陽の歌フラグ\n");
                osSyncPrintf(VT_RST);
            }
        } else {
            globalCtx->msgCtx.unk_E3EE = 4;
            osSyncPrintf(VT_FGCOL(YELLOW));
            // "Ocarina End"
            osSyncPrintf("z_oceff_spot  オカリナ終了\n");
            osSyncPrintf(VT_RST);
        }
    }
}

void OceffSpot_Wait(OceffSpot* self, GlobalContext* globalCtx) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        OceffSpot_SetupAction(self, OceffSpot_End);
    }
}

void OceffSpot_GrowCylinder(OceffSpot* self, GlobalContext* globalCtx) {
    if (self->unk_174 < 1.0f) {
        self->unk_174 += 0.05f;
    } else {
        OceffSpot_SetupAction(self, OceffSpot_Wait);
        self->timer = 60;
    }
}

void OceffSpot_Update(Actor* thisx, GlobalContext* globalCtx) {
    OceffSpot* self = THIS;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    f32 temp;

    temp = (1.0f - cosf(self->unk_174 * M_PI)) * 0.5f;
    self->actionFunc(self, globalCtx);

    self->actor.scale.z = 0.42f * temp;
    self->actor.scale.x = 0.42f * temp;

    self->actor.world.pos = player->actor.world.pos;
    self->actor.world.pos.y += 5.0f;

    temp = (2.0f - self->unk_174) * self->unk_174;
    Environment_AdjustLights(globalCtx, temp * 0.5F, 880.0f, 0.2f, 0.9f);

    Lights_PointNoGlowSetInfo(&self->lightInfo1, (s16)self->actor.world.pos.x, (s16)self->actor.world.pos.y + 55.0f,
                              (s16)self->actor.world.pos.z, (s32)(255.0f * temp), (s32)(255.0f * temp),
                              (s32)(200.0f * temp), (s16)(100.0f * temp));

    Lights_PointNoGlowSetInfo(&self->lightInfo2,
                              (s16)self->actor.world.pos.x + Math_SinS(player->actor.shape.rot.y) * 20.0f,
                              (s16)self->actor.world.pos.y + 20.0f,
                              (s16)self->actor.world.pos.z + Math_CosS(player->actor.shape.rot.y) * 20.0f,
                              (s32)(255.0f * temp), (s32)(255.0f * temp), (s32)(200.0f * temp), (s16)(100.0f * temp));
}

void OceffSpot_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OceffSpot* self = THIS;
    u32 scroll = globalCtx->state.frames & 0xFFFF;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_oceff_spot.c", 466);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_oceff_spot.c", 469),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, sTextureDL);
    gSPDisplayList(POLY_XLU_DISP++, Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, scroll * 2, scroll * (-2), 32, 32, 1,
                                                     0, scroll * (-8), 32, 32));
    gSPDisplayList(POLY_XLU_DISP++, sCylinderDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_oceff_spot.c", 485);
}
