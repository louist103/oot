/*
 * File: z_bg_mori_hashira4
 * Overlay: ovl_Bg_Mori_Hashira4
 * Description: Forest Temple gates and rotating pillars
 */

#include "z_bg_mori_hashira4.h"
#include "objects/object_mori_objects/object_mori_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgMoriHashira4*)thisx)

void BgMoriHashira4_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHashira4_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHashira4_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHashira4_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMoriHashira4_SetupWaitForMoriTex(BgMoriHashira4* self);
void BgMoriHashira4_WaitForMoriTex(BgMoriHashira4* self, GlobalContext* globalCtx);
void BgMoriHashira4_SetupPillarsRotate(BgMoriHashira4* self);
void BgMoriHashira4_PillarsRotate(BgMoriHashira4* self, GlobalContext* globalCtx);
void BgMoriHashira4_GateWait(BgMoriHashira4* self, GlobalContext* globalCtx);
void BgMoriHashira4_GateOpen(BgMoriHashira4* self, GlobalContext* globalCtx);

const ActorInit Bg_Mori_Hashira4_InitVars = {
    ACTOR_BG_MORI_HASHIRA4,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriHashira4),
    (ActorFunc)BgMoriHashira4_Init,
    (ActorFunc)BgMoriHashira4_Destroy,
    (ActorFunc)BgMoriHashira4_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 700, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

static Gfx* sDisplayLists[] = { gMoriHashiraPlatformsDL, gMoriHashiraGateDL };

static s16 sUnkTimer; // seems to be unused

void BgMoriHashira4_SetupAction(BgMoriHashira4* self, BgMoriHashira4ActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgMoriHashira4_InitDynaPoly(BgMoriHashira4* self, GlobalContext* globalCtx, CollisionHeader* collision,
                                 s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader;
    s32 pad2;

    colHeader = NULL;
    DynaPolyActor_Init(&self->dyna, moveFlag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning : move BG login failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_mori_hashira4.c", 155,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgMoriHashira4_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriHashira4* self = THIS;

    self->switchFlag = (self->dyna.actor.params >> 8) & 0x3F;
    self->dyna.actor.params &= 0xFF;

    if (self->dyna.actor.params == 0) {
        BgMoriHashira4_InitDynaPoly(self, globalCtx, &gMoriHashira1Col, DPM_UNK3);
    } else {
        BgMoriHashira4_InitDynaPoly(self, globalCtx, &gMoriHashira2Col, DPM_UNK);
    }
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->moriTexObjIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjIndex < 0) {
        Actor_Kill(&self->dyna.actor);
        // "Bank danger!"
        osSyncPrintf("Error : バンク危険！(arg_data 0x%04x)(%s %d)\n", self->dyna.actor.params,
                     "../z_bg_mori_hashira4.c", 196);
        return;
    }
    if ((self->dyna.actor.params != 0) && Flags_GetSwitch(globalCtx, self->switchFlag)) {
        Actor_Kill(&self->dyna.actor);
        return;
    }
    Actor_SetFocus(&self->dyna.actor, 50.0f);
    BgMoriHashira4_SetupWaitForMoriTex(self);
    // "(4 pillars of the Forest Temple) Bank danger"
    osSyncPrintf("(森の神殿 ４本柱)(arg_data 0x%04x)\n", self->dyna.actor.params);
    sUnkTimer = 0;
}

void BgMoriHashira4_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriHashira4* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgMoriHashira4_SetupWaitForMoriTex(BgMoriHashira4* self) {
    BgMoriHashira4_SetupAction(self, BgMoriHashira4_WaitForMoriTex);
}

void BgMoriHashira4_WaitForMoriTex(BgMoriHashira4* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->moriTexObjIndex)) {
        self->gateTimer = 0;
        if (self->dyna.actor.params == 0) {
            BgMoriHashira4_SetupPillarsRotate(self);
        } else {
            BgMoriHashira4_SetupAction(self, BgMoriHashira4_GateWait);
        }
        self->dyna.actor.draw = BgMoriHashira4_Draw;
    }
}

void BgMoriHashira4_SetupPillarsRotate(BgMoriHashira4* self) {
    BgMoriHashira4_SetupAction(self, BgMoriHashira4_PillarsRotate);
}

void BgMoriHashira4_PillarsRotate(BgMoriHashira4* self, GlobalContext* globalCtx) {
    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y += 0x96;
    Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ROLL_STAND_2 - SFX_FLAG);
}

void BgMoriHashira4_GateWait(BgMoriHashira4* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag) || (self->gateTimer != 0)) {
        self->gateTimer++;
        if (self->gateTimer > 30) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_METALDOOR_OPEN);
            BgMoriHashira4_SetupAction(self, BgMoriHashira4_GateOpen);
            OnePointCutscene_Init(globalCtx, 6010, 20, &self->dyna.actor, MAIN_CAM);
            sUnkTimer++;
        }
    }
}

void BgMoriHashira4_GateOpen(BgMoriHashira4* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 120.0f, 10.0f)) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgMoriHashira4_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriHashira4* self = THIS;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
}

void BgMoriHashira4_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriHashira4* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_hashira4.c", 339);
    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, globalCtx->objectCtx.status[self->moriTexObjIndex].segment);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_hashira4.c", 344),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, sDisplayLists[self->dyna.actor.params]);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_hashira4.c", 348);
}
