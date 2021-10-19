/*
 * File: z_obj_lift.c
 * Overlay: ovl_Obj_Lift
 * Description: Square, collapsing platform
 */

#include "z_obj_lift.h"
#include "objects/object_d_lift/object_d_lift.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"

#define FLAGS 0x00000010

#define THIS ((ObjLift*)thisx)

void ObjLift_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjLift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjLift_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjLift_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B9651C(ObjLift* self);
void func_80B9664C(ObjLift* self);
void func_80B967C0(ObjLift* self);

void func_80B96560(ObjLift* self, GlobalContext* globalCtx);
void func_80B96678(ObjLift* self, GlobalContext* globalCtx);
void func_80B96840(ObjLift* self, GlobalContext* globalCtx);

const ActorInit Obj_Lift_InitVars = {
    ACTOR_OBJ_LIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_D_LIFT,
    sizeof(ObjLift),
    (ActorFunc)ObjLift_Init,
    (ActorFunc)ObjLift_Destroy,
    (ActorFunc)ObjLift_Update,
    (ActorFunc)ObjLift_Draw,
};

static s16 sFallTimerDurations[] = { 0, 10, 20, 30, 40, 50, 60 };

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 z;
} ObjLiftFramgentScale; // size = 0x4

static ObjLiftFramgentScale sFragmentScales[] = {
    { 120, -120 }, { 120, 0 },     { 120, 120 }, { 0, -120 },   { 0, 0 },
    { 0, 120 },    { -120, -120 }, { -120, 0 },  { -120, 120 },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -600, ICHAIN_CONTINUE),   ICHAIN_F32_DIV1000(minVelocityY, -15000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE), ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2000, ICHAIN_STOP),
};

static f32 sScales[] = { 0.1f, 0.05f };
static f32 sMaxFallDistances[] = { -18.0f, -9.0f };

void ObjLift_SetupAction(ObjLift* self, ObjLiftActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void ObjLift_InitDynaPoly(ObjLift* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 flags) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, flags);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_obj_lift.c", 188,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void func_80B96160(ObjLift* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f* temp_s3;
    s32 pad0;
    s32 i;

    temp_s3 = &self->dyna.actor.world.pos;

    for (i = 0; i < ARRAY_COUNT(sFragmentScales); i++) {
        pos.x = sFragmentScales[i].x * self->dyna.actor.scale.x + temp_s3->x;
        pos.y = temp_s3->y;
        pos.z = sFragmentScales[i].z * self->dyna.actor.scale.z + temp_s3->z;
        velocity.x = sFragmentScales[i].x * self->dyna.actor.scale.x * 0.8f;
        velocity.y = Rand_ZeroOne() * 10.0f + 6.0f;
        velocity.z = sFragmentScales[i].z * self->dyna.actor.scale.z * 0.8f;
        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, temp_s3, -256, (Rand_ZeroOne() < 0.5f) ? 64 : 32, 15, 15, 0,
                             (Rand_ZeroOne() * 50.0f + 50.0f) * self->dyna.actor.scale.x, 0, 32, 50, KAKERA_COLOR_NONE,
                             OBJECT_D_LIFT, gCollapsingPlatformDL);
    }

    if (((self->dyna.actor.params >> 1) & 1) == 0) {
        func_80033480(globalCtx, &self->dyna.actor.world.pos, 120.0f, 12, 120, 100, 1);
    } else if (((self->dyna.actor.params >> 1) & 1) == 1) {
        func_80033480(globalCtx, &self->dyna.actor.world.pos, 60.0f, 8, 60, 100, 1);
    }
}

void ObjLift_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjLift* self = THIS;

    ObjLift_InitDynaPoly(self, globalCtx, &gCollapsingPlatformCol, DPM_PLAYER);

    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 2) & 0x3F)) {
        Actor_Kill(&self->dyna.actor);
        return;
    }

    Actor_SetScale(&self->dyna.actor, sScales[(self->dyna.actor.params >> 1) & 1]);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->unk168.x = Rand_ZeroOne() * 65535.5f;
    self->unk168.y = Rand_ZeroOne() * 65535.5f;
    self->unk168.z = Rand_ZeroOne() * 65535.5f;
    func_80B9651C(self);
    osSyncPrintf("(Dungeon Lift)(arg_data 0x%04x)\n", self->dyna.actor.params);
}

void ObjLift_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ObjLift* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_80B9651C(ObjLift* self) {
    self->timer = sFallTimerDurations[(self->dyna.actor.params >> 8) & 7];
    ObjLift_SetupAction(self, func_80B96560);
}

void func_80B96560(ObjLift* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 quakeIndex;

    if (func_8004356C(&self->dyna)) {
        if (self->timer <= 0) {
            if (((self->dyna.actor.params >> 8) & 7) == 7) {
                func_80B967C0(self);
            } else {
                quakeIndex = Quake_Add(GET_ACTIVE_CAM(globalCtx), 1);
                Quake_SetSpeed(quakeIndex, 10000);
                Quake_SetQuakeValues(quakeIndex, 2, 0, 0, 0);
                Quake_SetCountdown(quakeIndex, 20);
                func_80B9664C(self);
            }
        }
    } else {
        self->timer = sFallTimerDurations[(self->dyna.actor.params >> 8) & 7];
    }
}

void func_80B9664C(ObjLift* self) {
    self->timer = 20;
    ObjLift_SetupAction(self, func_80B96678);
}

void func_80B96678(ObjLift* self, GlobalContext* globalCtx) {
    if (self->timer <= 0) {
        func_80B967C0(self);
    } else {
        self->unk168.x += 10000;
        self->dyna.actor.world.rot.x = (s16)(Math_SinS(self->unk168.x) * 300.0f) + self->dyna.actor.home.rot.x;
        self->dyna.actor.world.rot.z = (s16)(Math_CosS(self->unk168.x) * 300.0f) + self->dyna.actor.home.rot.z;
        self->dyna.actor.shape.rot.x = self->dyna.actor.world.rot.x;
        self->dyna.actor.shape.rot.z = self->dyna.actor.world.rot.z;
        self->unk168.y += 18000;
        self->dyna.actor.world.pos.y = Math_SinS(self->unk168.y) + self->dyna.actor.home.pos.y;
        self->unk168.z += 18000;
        self->dyna.actor.world.pos.x = Math_SinS(self->unk168.z) * 3.0f + self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = Math_CosS(self->unk168.z) * 3.0f + self->dyna.actor.home.pos.z;
    }

    if ((self->timer & 3) == 3) {
        Audio_PlaySoundAtPosition(globalCtx, &self->dyna.actor.world.pos, 16, NA_SE_EV_BLOCK_SHAKE);
    }
}

void func_80B967C0(ObjLift* self) {
    ObjLift_SetupAction(self, func_80B96840);
    Math_Vec3f_Copy(&self->dyna.actor.world.pos, &self->dyna.actor.home.pos);
    self->dyna.actor.shape.rot = self->dyna.actor.world.rot = self->dyna.actor.home.rot;
}

void func_80B96840(ObjLift* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 bgId;
    Vec3f sp2C;

    Actor_MoveForward(&self->dyna.actor);
    Math_Vec3f_Copy(&sp2C, &self->dyna.actor.prevPos);
    sp2C.y += sMaxFallDistances[(self->dyna.actor.params >> 1) & 1];
    self->dyna.actor.floorHeight =
        BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->dyna.actor.floorPoly, &bgId, &self->dyna.actor, &sp2C);

    if ((self->dyna.actor.floorHeight - self->dyna.actor.world.pos.y) >=
        (sMaxFallDistances[(self->dyna.actor.params >> 1) & 1] - 0.001f)) {
        func_80B96160(self, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &self->dyna.actor.world.pos, 20, NA_SE_EV_BOX_BREAK);
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params >> 2) & 0x3F);
        Actor_Kill(&self->dyna.actor);
    }
}

void ObjLift_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjLift* self = THIS;

    if (self->timer > 0) {
        self->timer--;
    }

    self->actionFunc(self, globalCtx);
}

void ObjLift_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gCollapsingPlatformDL);
}
