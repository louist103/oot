/*
 * File: z_bg_ice_shutter.c
 * Overlay: ovl_Bg_Ice_Shutter
 * Description: Vertical Ice Bars (Doors) in Ice Cavern
 */

#include "z_bg_ice_shutter.h"
#include "objects/object_ice_objects/object_ice_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgIceShutter*)thisx)

void BgIceShutter_Init(Actor* thisx, GlobalContext* globalCtx);
void BgIceShutter_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgIceShutter_Update(Actor* thisx, GlobalContext* globalCtx);
void BgIceShutter_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80891CF4(BgIceShutter* thisx, GlobalContext* globalCtx);
void func_80891D6C(BgIceShutter* thisx, GlobalContext* globalCtx);
void func_80891DD4(BgIceShutter* thisx, GlobalContext* globalCtx);

const ActorInit Bg_Ice_Shutter_InitVars = {
    ACTOR_BG_ICE_SHUTTER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_ICE_OBJECTS,
    sizeof(BgIceShutter),
    (ActorFunc)BgIceShutter_Init,
    (ActorFunc)BgIceShutter_Destroy,
    (ActorFunc)BgIceShutter_Update,
    (ActorFunc)BgIceShutter_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void func_80891AC0(BgIceShutter* self) {
    f32 sp24;

    sp24 = Math_SinS(self->dyna.actor.shape.rot.x) * self->dyna.actor.velocity.y;
    self->dyna.actor.world.pos.y =
        (Math_CosS(self->dyna.actor.shape.rot.x) * self->dyna.actor.velocity.y) + self->dyna.actor.home.pos.y;
    self->dyna.actor.world.pos.x = (Math_SinS(self->dyna.actor.shape.rot.y) * sp24) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = (Math_CosS(self->dyna.actor.shape.rot.y) * sp24) + self->dyna.actor.home.pos.z;
}

void BgIceShutter_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgIceShutter* self = THIS;
    f32 sp24;
    CollisionHeader* colHeader;
    s32 sp28;
    f32 temp_f6;

    colHeader = NULL;
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    sp28 = self->dyna.actor.params & 0xFF;
    self->dyna.actor.params = (self->dyna.actor.params >> 8) & 0xFF;
    CollisionHeader_GetVirtual(&object_ice_objects_Col_002854, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (sp28 == 2) {
        self->dyna.actor.shape.rot.x = -0x4000;
    }

    if (sp28 != 1) {
        if (Flags_GetClear(globalCtx, self->dyna.actor.room)) {
            Actor_Kill(&self->dyna.actor);
        } else {
            self->actionFunc = func_80891CF4;
        }

    } else {
        if (Flags_GetSwitch(globalCtx, self->dyna.actor.params)) {
            Actor_Kill(&self->dyna.actor);
        } else {
            self->actionFunc = func_80891D6C;
        }
    }

    if (sp28 == 2) {
        temp_f6 = Math_SinS(self->dyna.actor.shape.rot.x) * 50.0f;
        self->dyna.actor.focus.pos.x =
            (Math_SinS(self->dyna.actor.shape.rot.y) * temp_f6) + self->dyna.actor.home.pos.x;
        self->dyna.actor.focus.pos.y = self->dyna.actor.home.pos.y;
        self->dyna.actor.focus.pos.z =
            self->dyna.actor.home.pos.z + (Math_CosS(self->dyna.actor.shape.rot.y) * temp_f6);
    } else {
        Actor_SetFocus(&self->dyna.actor, 50.0f);
    }
}

void BgIceShutter_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgIceShutter* self = THIS;
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_80891CF4(BgIceShutter* self, GlobalContext* globalCtx) {
    if (Flags_GetTempClear(globalCtx, self->dyna.actor.room)) {
        Flags_SetClear(globalCtx, self->dyna.actor.room);
        Audio_PlaySoundAtPosition(globalCtx, &self->dyna.actor.world.pos, 30, NA_SE_EV_SLIDE_DOOR_OPEN);
        self->actionFunc = func_80891DD4;
        if (self->dyna.actor.shape.rot.x == 0) {
            OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        }
    }
}

void func_80891D6C(BgIceShutter* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params)) {
        Audio_PlaySoundAtPosition(globalCtx, &self->dyna.actor.world.pos, 30, NA_SE_EV_SLIDE_DOOR_OPEN);
        self->actionFunc = func_80891DD4;
        OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
    }
}

void func_80891DD4(BgIceShutter* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->dyna.actor.speedXZ, 30.0f, 2.0f);
    if (Math_StepToF(&self->dyna.actor.velocity.y, 210.0f, self->dyna.actor.speedXZ)) {
        Actor_Kill(&self->dyna.actor);
        return;
    }

    func_80891AC0(self);
}

void BgIceShutter_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgIceShutter* self = THIS;
    self->actionFunc(self, globalCtx);
}

void BgIceShutter_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, object_ice_objects_DL_002740);
}
