/*
 * File: z_bg_spot12_saku.c
 * Overlay: ovl_Bg_Spot12_Saku
 * Description:
 */

#include "z_bg_spot12_saku.h"
#include "objects/object_spot12_obj/object_spot12_obj.h"

#define FLAGS 0x00000000

#define THIS ((BgSpot12Saku*)thisx)

void BgSpot12Saku_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot12Saku_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot12Saku_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot12Saku_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808B3550(BgSpot12Saku* self);
void func_808B357C(BgSpot12Saku* self, GlobalContext* globalCtx);
void func_808B35E4(BgSpot12Saku* self);
void func_808B3604(BgSpot12Saku* self, GlobalContext* globalCtx);
void func_808B3714(BgSpot12Saku* self);
void func_808B37AC(BgSpot12Saku* self, GlobalContext* globalCtx);

const ActorInit Bg_Spot12_Saku_InitVars = {
    ACTOR_BG_SPOT12_SAKU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT12_OBJ,
    sizeof(BgSpot12Saku),
    (ActorFunc)BgSpot12Saku_Init,
    (ActorFunc)BgSpot12Saku_Destroy,
    (ActorFunc)BgSpot12Saku_Update,
    (ActorFunc)BgSpot12Saku_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void func_808B3420(BgSpot12Saku* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 flags) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, flags);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_spot12_saku.c", 140,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgSpot12Saku_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot12Saku* self = THIS;

    func_808B3420(self, globalCtx, &gGerudoFortressGTGShutterCol, DPM_UNK);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
        func_808B3714(self);
    } else {
        func_808B3550(self);
    }
}

void BgSpot12Saku_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot12Saku* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808B3550(BgSpot12Saku* self) {
    self->actionFunc = func_808B357C;
    self->dyna.actor.scale.x = 0.1f;
    self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
}

void func_808B357C(BgSpot12Saku* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
        func_808B35E4(self);
        self->timer = 20;
        OnePointCutscene_Init(globalCtx, 4170, -99, &self->dyna.actor, MAIN_CAM);
    }
}

void func_808B35E4(BgSpot12Saku* self) {
    if (self->timer == 0) {
        self->actionFunc = func_808B3604;
    }
}

void func_808B3604(BgSpot12Saku* self, GlobalContext* globalCtx) {
    f32 temp_ret = Math_SmoothStepToF(&self->dyna.actor.scale.x, 0.001f / 0.14f, 0.16f, 0.0022f, 0.001f);
    f32 temp_f18 = ((0.1f - self->dyna.actor.scale.x) * 840.0f);

    self->dyna.actor.world.pos.x =
        self->dyna.actor.home.pos.x - (Math_SinS(self->dyna.actor.shape.rot.y + 0x4000) * temp_f18);
    self->dyna.actor.world.pos.z =
        self->dyna.actor.home.pos.z - (Math_CosS(self->dyna.actor.shape.rot.y + 0x4000) * temp_f18);
    if (fabsf(temp_ret) < 0.0001f) {
        func_808B3714(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BRIDGE_OPEN_STOP);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_METALGATE_OPEN - SFX_FLAG);
    }
}

void func_808B3714(BgSpot12Saku* self) {
    self->actionFunc = func_808B37AC;
    self->dyna.actor.scale.x = 0.001f / 0.14f;
    self->dyna.actor.world.pos.x =
        self->dyna.actor.home.pos.x - (Math_SinS(self->dyna.actor.shape.rot.y + 0x4000) * 78.0f);
    self->dyna.actor.world.pos.z =
        self->dyna.actor.home.pos.z - (Math_CosS(self->dyna.actor.shape.rot.y + 0x4000) * 78.0f);
}

void func_808B37AC(BgSpot12Saku* self, GlobalContext* globalCtx) {
}

void BgSpot12Saku_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot12Saku* self = THIS;

    if (self->timer > 0) {
        self->timer--;
    }
    self->actionFunc(self, globalCtx);
}

void BgSpot12Saku_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gGerudoFortressGTGShutterDL);
}
