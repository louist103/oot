/*
 * File: z_bg_jya_amishutter.c
 * Overlay: Bg_Jya_Amishutter
 * Description: Circular metal grate. Lifts up when you get close to it.
 */

#include "z_bg_jya_amishutter.h"
#include "objects/object_jya_obj/object_jya_obj.h"

#define FLAGS 0x00000000

#define THIS ((BgJyaAmishutter*)thisx)

void BgJyaAmishutter_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaAmishutter_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaAmishutter_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaAmishutter_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgJyaAmishutter_SetupWaitForPlayer(BgJyaAmishutter* self);
void BgJyaAmishutter_WaitForPlayer(BgJyaAmishutter* self);
void func_80893428(BgJyaAmishutter* self);
void func_80893438(BgJyaAmishutter* self);
void func_808934B0(BgJyaAmishutter* self);
void func_808934C0(BgJyaAmishutter* self);
void func_808934FC(BgJyaAmishutter* self);
void func_8089350C(BgJyaAmishutter* self);

const ActorInit Bg_Jya_Amishutter_InitVars = {
    ACTOR_BG_JYA_AMISHUTTER,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaAmishutter),
    (ActorFunc)BgJyaAmishutter_Init,
    (ActorFunc)BgJyaAmishutter_Destroy,
    (ActorFunc)BgJyaAmishutter_Update,
    (ActorFunc)BgJyaAmishutter_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgJyaAmishutter_InitDynaPoly(BgJyaAmishutter* self, GlobalContext* globalCtx, CollisionHeader* collision,
                                  s32 flag) {
    s32 pad1;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, flag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_amishutter.c", 129,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgJyaAmishutter_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaAmishutter* self = THIS;

    BgJyaAmishutter_InitDynaPoly(self, globalCtx, &gAmishutterCol, DPM_UNK);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    BgJyaAmishutter_SetupWaitForPlayer(self);
}

void BgJyaAmishutter_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaAmishutter* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgJyaAmishutter_SetupWaitForPlayer(BgJyaAmishutter* self) {
    self->actionFunc = BgJyaAmishutter_WaitForPlayer;
}

void BgJyaAmishutter_WaitForPlayer(BgJyaAmishutter* self) {
    if ((self->dyna.actor.xzDistToPlayer < 60.0f) && (fabsf(self->dyna.actor.yDistToPlayer) < 30.0f)) {
        func_80893428(self);
    }
}

void func_80893428(BgJyaAmishutter* self) {
    self->actionFunc = func_80893438;
}

void func_80893438(BgJyaAmishutter* self) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 100.0f, 3.0f)) {
        func_808934B0(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_METALDOOR_STOP);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
    }
}

void func_808934B0(BgJyaAmishutter* self) {
    self->actionFunc = func_808934C0;
}

void func_808934C0(BgJyaAmishutter* self) {
    if (self->dyna.actor.xzDistToPlayer > 300.0f) {
        func_808934FC(self);
    }
}

void func_808934FC(BgJyaAmishutter* self) {
    self->actionFunc = func_8089350C;
}

void func_8089350C(BgJyaAmishutter* self) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 3.0f)) {
        BgJyaAmishutter_SetupWaitForPlayer(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_METALDOOR_STOP);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG);
    }
}

void BgJyaAmishutter_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaAmishutter* self = THIS;

    self->actionFunc(self);
}

void BgJyaAmishutter_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gAmishutterDL);
}
