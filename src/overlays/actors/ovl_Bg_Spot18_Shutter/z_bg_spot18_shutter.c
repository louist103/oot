/*
 * File: z_bg_spot18_shutter.c
 * Overlay: Bg_Spot18_Shutter
 * Description:
 */

#include "z_bg_spot18_shutter.h"
#include "objects/object_spot18_obj/object_spot18_obj.h"

#define FLAGS 0x00000030

#define THIS ((BgSpot18Shutter*)thisx)

void BgSpot18Shutter_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Shutter_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Shutter_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Shutter_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808B95AC(BgSpot18Shutter* self, GlobalContext* globalCtx);
void func_808B95B8(BgSpot18Shutter* self, GlobalContext* globalCtx);
void func_808B9618(BgSpot18Shutter* self, GlobalContext* globalCtx);
void func_808B9698(BgSpot18Shutter* self, GlobalContext* globalCtx);
void func_808B971C(BgSpot18Shutter* self, GlobalContext* globalCtx);

const ActorInit Bg_Spot18_Shutter_InitVars = {
    ACTOR_BG_SPOT18_SHUTTER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT18_OBJ,
    sizeof(BgSpot18Shutter),
    (ActorFunc)BgSpot18Shutter_Init,
    (ActorFunc)BgSpot18Shutter_Destroy,
    (ActorFunc)BgSpot18Shutter_Update,
    (ActorFunc)BgSpot18Shutter_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgSpot18Shutter_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgSpot18Shutter* self = THIS;
    s32 param = (self->dyna.actor.params >> 8) & 1;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);

    if (param == 0) {
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            if (gSaveContext.infTable[16] & 0x200) {
                self->actionFunc = func_808B95AC;
                self->dyna.actor.world.pos.y += 180.0f;
            } else {
                self->actionFunc = func_808B9618;
            }
        } else {
            if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
                self->actionFunc = func_808B95AC;
                self->dyna.actor.world.pos.y += 180.0f;
            } else {
                self->actionFunc = func_808B95B8;
            }
        }
    } else {
        if (gSaveContext.infTable[16] & 0x200) {
            self->dyna.actor.world.pos.x += 125.0f * Math_CosS(self->dyna.actor.world.rot.y);
            self->dyna.actor.world.pos.z -= 125.0f * Math_SinS(self->dyna.actor.world.rot.y);
            self->actionFunc = func_808B95AC;
        } else {
            self->actionFunc = func_808B9618;
        }
    }

    CollisionHeader_GetVirtual(&gGoronCityDoorCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
}

void BgSpot18Shutter_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot18Shutter* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808B95AC(BgSpot18Shutter* self, GlobalContext* globalCtx) {
}

void func_808B95B8(BgSpot18Shutter* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
        Actor_SetFocus(&self->dyna.actor, 70.0f);
        OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        self->actionFunc = func_808B9698;
    }
}

void func_808B9618(BgSpot18Shutter* self, GlobalContext* globalCtx) {
    if (gSaveContext.infTable[16] & 0x200) {
        Actor_SetFocus(&self->dyna.actor, 70.0f);
        if (((self->dyna.actor.params >> 8) & 1) == 0) {
            self->actionFunc = func_808B9698;
        } else {
            self->actionFunc = func_808B971C;
            OnePointCutscene_Init(globalCtx, 4221, 140, &self->dyna.actor, MAIN_CAM);
        }
    }
}

void func_808B9698(BgSpot18Shutter* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 180.0f, 1.44f)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_STONEDOOR_STOP);
        self->actionFunc = func_808B95AC;
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG);
    }
}

void func_808B971C(BgSpot18Shutter* self, GlobalContext* globalCtx) {
    f32 sin = Math_SinS(self->dyna.actor.world.rot.y);
    f32 cos = Math_CosS(self->dyna.actor.world.rot.y);
    s32 flag = true;

    flag &= Math_StepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x + (125.0f * cos), fabsf(cos));
    flag &= Math_StepToF(&self->dyna.actor.world.pos.z, self->dyna.actor.home.pos.z - (125.0f * sin), fabsf(sin));

    if (flag) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_STONEDOOR_STOP);
        self->actionFunc = func_808B95AC;
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG);
    }
}

void BgSpot18Shutter_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot18Shutter* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgSpot18Shutter_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gGoronCityDoorDL);
}
