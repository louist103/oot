
/*
 * File: z_bg_spot18_obj.c
 * Overlay: ovl_Bg_Spot18_Obj
 * Description:
 */

#include "z_bg_spot18_obj.h"
#include "objects/object_spot18_obj/object_spot18_obj.h"

#define FLAGS 0x00000000

#define THIS ((BgSpot18Obj*)thisx)

void BgSpot18Obj_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Obj_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Obj_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Obj_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 func_808B8910(BgSpot18Obj* self, GlobalContext* globalCtx);
s32 func_808B8A5C(BgSpot18Obj* self, GlobalContext* globalCtx);
s32 func_808B8A98(BgSpot18Obj* self, GlobalContext* globalCtx);
s32 func_808B8B08(BgSpot18Obj* self, GlobalContext* globalCtx);
s32 func_808B8BB4(BgSpot18Obj* self, GlobalContext* globalCtx);
s32 func_808B8C90(BgSpot18Obj* self, GlobalContext* globalCtx);
void func_808B8DC0(BgSpot18Obj* self);
void func_808B8DD0(BgSpot18Obj* self, GlobalContext* globalCtx);
void func_808B8E64(BgSpot18Obj* self);
void func_808B8E7C(BgSpot18Obj* self, GlobalContext* globalCtx);
void func_808B8EE0(BgSpot18Obj* self);
void func_808B8F08(BgSpot18Obj* self, GlobalContext* globalCtx);
void func_808B9030(BgSpot18Obj* self);
void func_808B9040(BgSpot18Obj* self, GlobalContext* globalCtx);

const ActorInit Bg_Spot18_Obj_InitVars = {
    ACTOR_BG_SPOT18_OBJ,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT18_OBJ,
    sizeof(BgSpot18Obj),
    (ActorFunc)BgSpot18Obj_Init,
    (ActorFunc)BgSpot18Obj_Destroy,
    (ActorFunc)BgSpot18Obj_Update,
    (ActorFunc)BgSpot18Obj_Draw,
};

static u8 D_808B90F0[2][2] = { { 0x01, 0x01 }, { 0x01, 0x00 } };

static f32 D_808B90F4[] = {
    0.1f,
    0.1f,
};

static CollisionHeader* D_808B90FC[] = {
    &gGoronCityStatueCol,
    &gGoronCityStatueSpearCol,
};

static u32 D_808B9104[] = {
    0,
    0,
};

static BgSpot18ObjInitFunc D_808B910C[] = {
    func_808B8A98,
    func_808B8910,
    func_808B8A5C,
    func_808B8B08,
};

static InitChainEntry sInitChain1[] = {
    ICHAIN_F32(minVelocityY, -10, ICHAIN_CONTINUE),       ICHAIN_F32(gravity, -4, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1400, ICHAIN_CONTINUE), ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

static InitChainEntry sInitChain2[] = {
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 700, ICHAIN_STOP),
};

static BgSpot18ObjInitFunc D_808B913C[] = {
    func_808B8BB4,
    func_808B8C90,
};

static Gfx(*sDlists[]) = {
    gGoronCityStatueDL,
    gGoronCityStatueSpearDL,
};

s32 func_808B8910(BgSpot18Obj* self, GlobalContext* globalCtx) {
    s32 age;

    if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
        age = 1;
    } else if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        age = 0;
    } else {
        osSyncPrintf("Error : リンク年齢不詳 (%s %d)(arg_data 0x%04x)\n", "../z_bg_spot18_obj.c", 182,
                     self->dyna.actor.params);
        return 0;
    }

    switch (D_808B90F0[self->dyna.actor.params & 0xF][age]) {
        case 0:
        case 1:
            if (D_808B90F0[self->dyna.actor.params & 0xF][age] == 0) {
                osSyncPrintf("出現しない Object (0x%04x)\n", self->dyna.actor.params);
            }
            return D_808B90F0[self->dyna.actor.params & 0xF][age];
        case 2:
            osSyncPrintf("Error : Obj出現判定が設定されていない(%s %d)(arg_data 0x%04x)\n", "../z_bg_spot18_obj.c", 202,
                         self->dyna.actor.params);
            break;
        default:
            osSyncPrintf("Error : Obj出現判定失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_spot18_obj.c", 210,
                         self->dyna.actor.params);
    }
    return 0;
}

s32 func_808B8A5C(BgSpot18Obj* self, GlobalContext* globalCtx) {
    Actor_SetScale(&self->dyna.actor, D_808B90F4[self->dyna.actor.params & 0xF]);
    return 1;
}

s32 func_808B8A98(BgSpot18Obj* self, GlobalContext* globalCtx) {
    s32 pad[2];
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(D_808B90FC[self->dyna.actor.params & 0xF], &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    return 1;
}

s32 func_808B8B08(BgSpot18Obj* self, GlobalContext* globalCtx) {
    self->dyna.actor.flags |= D_808B9104[self->dyna.actor.params & 0xF];
    return 1;
}

s32 func_808B8B38(BgSpot18Obj* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(D_808B910C); i++) {
        if (D_808B910C[i](self, globalCtx) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 func_808B8BB4(BgSpot18Obj* self, GlobalContext* globalCtx) {
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain1);

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        func_808B9030(self);
    } else if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
        func_808B9030(self);
        self->dyna.actor.world.pos.x = (Math_SinS(self->dyna.actor.world.rot.y) * 80.0f) + self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = (Math_CosS(self->dyna.actor.world.rot.y) * 80.0f) + self->dyna.actor.home.pos.z;
    } else {
        func_808B8E64(self);
    }
    return 1;
}

s32 func_808B8C90(BgSpot18Obj* self, GlobalContext* globalCtx) {
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain2);
    func_808B8DC0(self);
    return 1;
}

s32 func_808B8CC8(BgSpot18Obj* self, GlobalContext* globalCtx) {
    if ((D_808B913C[self->dyna.actor.params & 0xF] != NULL) &&
        (!D_808B913C[self->dyna.actor.params & 0xF](self, globalCtx))) {
        return 0;
    }
    return 1;
}

void BgSpot18Obj_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot18Obj* self = THIS;

    osSyncPrintf("Spot18 Object [arg_data : 0x%04x]\n", self->dyna.actor.params);
    if (!func_808B8B38(self, globalCtx)) {
        Actor_Kill(&self->dyna.actor);
    } else if (!func_808B8CC8(self, globalCtx)) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgSpot18Obj_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot18Obj* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808B8DC0(BgSpot18Obj* self) {
    self->actionFunc = func_808B8DD0;
}

void func_808B8DD0(BgSpot18Obj* self, GlobalContext* globalCtx) {
}

void func_808B8DDC(BgSpot18Obj* self, GlobalContext* globalCtx) {
    Actor_UpdateBgCheckInfo(globalCtx, &self->dyna.actor, 20.0f, 46.0f, 0.0f, 28);
}

void func_808B8E20(BgSpot18Obj* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (fabsf(self->dyna.unk_150) > 0.001f) {
        self->dyna.unk_150 = 0.0f;
        player->stateFlags2 &= ~0x10;
    }
}

void func_808B8E64(BgSpot18Obj* self) {
    self->unk_168 = 20;
    self->actionFunc = func_808B8E7C;
}

void func_808B8E7C(BgSpot18Obj* self, GlobalContext* globalCtx) {
    if (self->dyna.unk_150 < -0.001f) {
        if (self->unk_168 <= 0) {
            func_808B8EE0(self);
        }
    } else {
        self->unk_168 = 20;
    }
    func_808B8E20(self, globalCtx);
}

void func_808B8EE0(BgSpot18Obj* self) {
    self->actionFunc = func_808B8F08;
    self->dyna.actor.world.rot.y = 0;
    self->dyna.actor.speedXZ = 0.0f;
    self->dyna.actor.velocity.z = 0.0f;
    self->dyna.actor.velocity.y = 0.0f;
    self->dyna.actor.velocity.x = 0.0f;
}

void func_808B8F08(BgSpot18Obj* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    Math_StepToF(&self->dyna.actor.speedXZ, 1.2f, 0.1f);
    Actor_MoveForward(&self->dyna.actor);
    func_808B8DDC(self, globalCtx);

    if (Math3D_Dist2DSq(self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.z, self->dyna.actor.home.pos.x,
                        self->dyna.actor.home.pos.z) >= 6400.0f) {
        func_808B9030(self);
        self->dyna.actor.world.pos.x = (Math_SinS(self->dyna.actor.world.rot.y) * 80.0f) + self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = (Math_CosS(self->dyna.actor.world.rot.y) * 80.0f) + self->dyna.actor.home.pos.z;
        self->dyna.unk_150 = 0.0f;
        player->stateFlags2 &= ~0x10;
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_ROCK_SLIDE - SFX_FLAG);
    }
}

void func_808B9030(BgSpot18Obj* self) {
    self->actionFunc = func_808B9040;
}

void func_808B9040(BgSpot18Obj* self, GlobalContext* globalCtx) {
    func_808B8E20(self, globalCtx);
}

void BgSpot18Obj_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot18Obj* self = THIS;

    if (self->unk_168 > 0) {
        self->unk_168 -= 1;
    }
    self->actionFunc(self, globalCtx);
}

void BgSpot18Obj_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, sDlists[thisx->params & 0xF]);
}
