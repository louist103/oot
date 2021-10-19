/*
 * File: z_bg_haka_meganebg.c
 * Overlay: ovl_Bg_Haka_MeganeBG
 * Description:
 */

#include "z_bg_haka_meganebg.h"
#include "objects/object_haka_objects/object_haka_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgHakaMeganeBG*)thisx)

void BgHakaMeganeBG_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaMeganeBG_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaMeganeBG_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaMeganeBG_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8087DFF8(BgHakaMeganeBG* self, GlobalContext* globalCtx);
void func_8087E040(BgHakaMeganeBG* self, GlobalContext* globalCtx);
void func_8087E10C(BgHakaMeganeBG* self, GlobalContext* globalCtx);
void func_8087E1E0(BgHakaMeganeBG* self, GlobalContext* globalCtx);
void func_8087E258(BgHakaMeganeBG* self, GlobalContext* globalCtx);
void func_8087E288(BgHakaMeganeBG* self, GlobalContext* globalCtx);
void func_8087E2D8(BgHakaMeganeBG* self, GlobalContext* globalCtx);
void func_8087E34C(BgHakaMeganeBG* self, GlobalContext* globalCtx);

const ActorInit Bg_Haka_MeganeBG_InitVars = {
    ACTOR_BG_HAKA_MEGANEBG,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKA_OBJECTS,
    sizeof(BgHakaMeganeBG),
    (ActorFunc)BgHakaMeganeBG_Init,
    (ActorFunc)BgHakaMeganeBG_Destroy,
    (ActorFunc)BgHakaMeganeBG_Update,
    (ActorFunc)BgHakaMeganeBG_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

// Unused
static u32 D_8087E3FC[] = {
    0x00000000, 0x00000000, 0x00000000, 0xC8C800FF, 0xFF0000FF,
};

static Gfx* D_8087E410[] = {
    object_haka_objects_DL_008EB0,
    object_haka_objects_DL_00A1A0,
    object_haka_objects_DL_005000,
    object_haka_objects_DL_000040,
};

void BgHakaMeganeBG_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHakaMeganeBG* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->unk_168 = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;

    if (thisx->params == 2) {
        DynaPolyActor_Init(&self->dyna, DPM_UNK3);
        thisx->flags |= 0x10;
        CollisionHeader_GetVirtual(&object_haka_objects_Col_005334, &colHeader);
        self->actionFunc = func_8087E258;
    } else {
        DynaPolyActor_Init(&self->dyna, DPM_PLAYER);

        if (thisx->params == 0) {
            CollisionHeader_GetVirtual(&object_haka_objects_Col_009168, &colHeader);
            thisx->flags |= 0x80;
            self->unk_16A = 20;
            self->actionFunc = func_8087DFF8;
        } else if (thisx->params == 3) {
            CollisionHeader_GetVirtual(&object_haka_objects_Col_000118, &colHeader);
            thisx->home.pos.y += 100.0f;

            if (Flags_GetSwitch(globalCtx, self->unk_168)) {
                self->actionFunc = func_8087E34C;
                thisx->world.pos.y = thisx->home.pos.y;
            } else {
                thisx->flags |= 0x10;
                self->actionFunc = func_8087E288;
            }
        } else {
            CollisionHeader_GetVirtual(&object_haka_objects_Col_00A7F4, &colHeader);
            self->unk_16A = 80;
            self->actionFunc = func_8087E10C;
            thisx->uncullZoneScale = 3000.0f;
            thisx->uncullZoneDownward = 3000.0f;
        }
    }

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
}

void BgHakaMeganeBG_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaMeganeBG* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_8087DFF8(BgHakaMeganeBG* self, GlobalContext* globalCtx) {
    if (self->unk_16A != 0) {
        self->unk_16A--;
    }

    if (self->unk_16A == 0) {
        self->unk_16A = 40;
        self->dyna.actor.world.rot.y += 0x8000;
        self->actionFunc = func_8087E040;
    }
}

void func_8087E040(BgHakaMeganeBG* self, GlobalContext* globalCtx) {
    f32 xSub;

    if (self->unk_16A != 0) {
        self->unk_16A--;
    }

    xSub = (sinf(((self->unk_16A * 0.025f) + 0.5f) * M_PI) + 1.0f) * 160.0f;

    if (self->dyna.actor.world.rot.y != self->dyna.actor.shape.rot.y) {
        xSub = 320.0f - xSub;
    }

    self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x - xSub;

    if (self->unk_16A == 0) {
        self->unk_16A = 20;
        self->actionFunc = func_8087DFF8;
    }
}

void func_8087E10C(BgHakaMeganeBG* self, GlobalContext* globalCtx) {
    self->dyna.actor.velocity.y += 1.0f;

    if (self->dyna.actor.velocity.y > 20.0f) {
        self->dyna.actor.velocity.y = 20.0f;
    } else {
        self->dyna.actor.velocity.y = self->dyna.actor.velocity.y;
    }

    if (self->unk_16A != 0) {
        self->unk_16A--;
    }

    if (!Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 640.0f,
                      self->dyna.actor.velocity.y)) {
        func_8002F974(&self->dyna.actor, NA_SE_EV_CHINETRAP_DOWN - SFX_FLAG);
    }

    if (self->unk_16A == 0) {
        self->unk_16A = 120;
        self->actionFunc = func_8087E1E0;
        self->dyna.actor.velocity.y = 0.0f;
    }
}

void func_8087E1E0(BgHakaMeganeBG* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 16.0f / 3.0f);
    func_8002F974(&self->dyna.actor, NA_SE_EV_BRIDGE_CLOSE - SFX_FLAG);

    if (self->unk_16A != 0) {
        self->unk_16A--;
    }

    if (self->unk_16A == 0) {
        self->unk_16A = 80;
        self->actionFunc = func_8087E10C;
    }
}

void func_8087E258(BgHakaMeganeBG* self, GlobalContext* globalCtx) {
    self->dyna.actor.shape.rot.y += 0x180;
    func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
}

void func_8087E288(BgHakaMeganeBG* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->unk_168)) {
        OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        self->actionFunc = func_8087E2D8;
    }
}

void func_8087E2D8(BgHakaMeganeBG* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->dyna.actor.speedXZ, 30.0f, 2.0f);

    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, self->dyna.actor.speedXZ)) {
        Actor_SetFocus(&self->dyna.actor, 50.0f);
        self->actionFunc = func_8087E34C;
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_METALDOOR_OPEN);
    }
}

void func_8087E34C(BgHakaMeganeBG* self, GlobalContext* globalCtx) {
}

void BgHakaMeganeBG_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaMeganeBG* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgHakaMeganeBG_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaMeganeBG* self = THIS;
    s16 params = self->dyna.actor.params;

    if (params == 0) {
        Gfx_DrawDListXlu(globalCtx, object_haka_objects_DL_008EB0);
    } else {
        Gfx_DrawDListOpa(globalCtx, D_8087E410[params]);
    }
}
