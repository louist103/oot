/*
 * File: z_bg_jya_kanaami.c
 * Overlay: ovl_Bg_Jya_Kanaami
 * Description: Climbable grating/bridge (Spirit Temple)
 */

#include "z_bg_jya_kanaami.h"
#include "objects/object_jya_obj/object_jya_obj.h"

#define FLAGS 0x00000000

#define THIS ((BgJyaKanaami*)thisx)

void BgJyaKanaami_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaKanaami_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaKanaami_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaKanaami_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80899880(BgJyaKanaami* self);
void func_80899894(BgJyaKanaami* self, GlobalContext* globalCtx);
void func_8089993C(BgJyaKanaami* self);
void func_80899950(BgJyaKanaami* self, GlobalContext* globalCtx);
void func_80899A08(BgJyaKanaami* self);

const ActorInit Bg_Jya_Kanaami_InitVars = {
    ACTOR_BG_JYA_KANAAMI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaKanaami),
    (ActorFunc)BgJyaKanaami_Init,
    (ActorFunc)BgJyaKanaami_Destroy,
    (ActorFunc)BgJyaKanaami_Update,
    (ActorFunc)BgJyaKanaami_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 700, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgJyaKanaami_InitDynaPoly(BgJyaKanaami* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 flag) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, flag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_kanaami.c", 145,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgJyaKanaami_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaKanaami* self = THIS;

    BgJyaKanaami_InitDynaPoly(self, globalCtx, &gKanaamiCol, DPM_UNK);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
        func_80899A08(self);
    } else {
        func_80899880(self);
    }
    osSyncPrintf("(jya 金網)(arg_data 0x%04x)\n", self->dyna.actor.params);
}

void BgJyaKanaami_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaKanaami* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_80899880(BgJyaKanaami* self) {
    self->actionFunc = func_80899894;
    self->unk_16A = 0;
}

void func_80899894(BgJyaKanaami* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F) || self->unk_16A > 0) {
        if (self->dyna.actor.world.pos.x > -1000.0f && self->unk_16A == 0) {
            OnePointCutscene_Init(globalCtx, 3450, -99, &self->dyna.actor, MAIN_CAM);
        }
        self->unk_16A += 1;
        if (self->unk_16A >= 0xA) {
            func_8089993C(self);
        }
    }
}

void func_8089993C(BgJyaKanaami* self) {
    self->actionFunc = func_80899950;
    self->unk_168 = 0;
}

void func_80899950(BgJyaKanaami* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s32 quakeId;

    self->unk_168 += 0x20;
    if (Math_ScaledStepToS(&self->dyna.actor.world.rot.x, 0x4000, self->unk_168)) {
        func_80899A08(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_TRAP_BOUND);
        quakeId = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
        Quake_SetSpeed(quakeId, 25000);
        Quake_SetQuakeValues(quakeId, 2, 0, 0, 0);
        Quake_SetCountdown(quakeId, 16);
    }
}

void func_80899A08(BgJyaKanaami* self) {
    self->actionFunc = 0;
    self->dyna.actor.world.rot.x = 0x4000;
}

void BgJyaKanaami_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaKanaami* self = THIS;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
    self->dyna.actor.shape.rot.x = self->dyna.actor.world.rot.x;
}

void BgJyaKanaami_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gKanaamiDL);
}
