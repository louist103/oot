/*
 * File: z_bg_ice_turara.c
 * Overlay: ovl_Bg_Ice_Turara
 * Description: Icicles
 */

#include "z_bg_ice_turara.h"
#include "objects/object_ice_objects/object_ice_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgIceTurara*)thisx)

void BgIceTurara_Init(Actor* thisx, GlobalContext* globalCtx);
void BgIceTurara_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgIceTurara_Update(Actor* thisx, GlobalContext* globalCtx);
void BgIceTurara_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgIceTurara_Stalagmite(BgIceTurara* self, GlobalContext* globalCtx);
void BgIceTurara_Wait(BgIceTurara* self, GlobalContext* globalCtx);
void BgIceTurara_Shiver(BgIceTurara* self, GlobalContext* globalCtx);
void BgIceTurara_Fall(BgIceTurara* self, GlobalContext* globalCtx);
void BgIceTurara_Regrow(BgIceTurara* self, GlobalContext* globalCtx);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x04 },
        { 0x4FC007CA, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 13, 120, 0, { 0, 0, 0 } },
};

const ActorInit Bg_Ice_Turara_InitVars = {
    ACTOR_BG_ICE_TURARA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_ICE_OBJECTS,
    sizeof(BgIceTurara),
    (ActorFunc)BgIceTurara_Init,
    (ActorFunc)BgIceTurara_Destroy,
    (ActorFunc)BgIceTurara_Update,
    (ActorFunc)BgIceTurara_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 600, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -3, ICHAIN_CONTINUE),
    ICHAIN_F32(minVelocityY, -30, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgIceTurara_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgIceTurara* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&object_ice_objects_Col_002594, &colHeader);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->dyna.actor, &sCylinderInit);
    Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.actor.params == TURARA_STALAGMITE) {
        self->actionFunc = BgIceTurara_Stalagmite;
    } else {
        self->dyna.actor.shape.rot.x = -0x8000;
        self->dyna.actor.shape.yOffset = 1200.0f;
        self->actionFunc = BgIceTurara_Wait;
    }
}

void BgIceTurara_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgIceTurara* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void BgIceTurara_Break(BgIceTurara* self, GlobalContext* globalCtx, f32 arg2) {
    static Vec3f accel = { 0.0f, -1.0f, 0.0f };
    static Color_RGBA8 primColor = { 170, 255, 255, 255 };
    static Color_RGBA8 envColor = { 0, 50, 100, 255 };
    Vec3f vel;
    Vec3f pos;
    s32 j;
    s32 i;

    Audio_PlaySoundAtPosition(globalCtx, &self->dyna.actor.world.pos, 30, NA_SE_EV_ICE_BROKEN);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 10; j++) {
            pos.x = self->dyna.actor.world.pos.x + Rand_CenteredFloat(8.0f);
            pos.y = self->dyna.actor.world.pos.y + (Rand_ZeroOne() * arg2) + (i * arg2);
            pos.z = self->dyna.actor.world.pos.z + Rand_CenteredFloat(8.0f);

            vel.x = Rand_CenteredFloat(7.0f);
            vel.z = Rand_CenteredFloat(7.0f);
            vel.y = (Rand_ZeroOne() * 4.0f) + 8.0f;

            EffectSsEnIce_Spawn(globalCtx, &pos, (Rand_ZeroOne() * 0.2f) + 0.1f, &vel, &accel, &primColor, &envColor,
                                30);
        }
    }
}

void BgIceTurara_Stalagmite(BgIceTurara* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        BgIceTurara_Break(self, globalCtx, 50.0f);
        Actor_Kill(&self->dyna.actor);
        return;
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void BgIceTurara_Wait(BgIceTurara* self, GlobalContext* globalCtx) {
    if (self->dyna.actor.xzDistToPlayer < 60.0f) {
        self->shiverTimer = 10;
        self->actionFunc = BgIceTurara_Shiver;
    }
}

void BgIceTurara_Shiver(BgIceTurara* self, GlobalContext* globalCtx) {
    s16 phi_v0_3;
    s16 phi_v0_2;
    f32 sp28;

    if (self->shiverTimer != 0) {
        self->shiverTimer--;
    }
    if (!(self->shiverTimer % 4)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ICE_SWING);
    }
    if (self->shiverTimer == 0) {
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
        Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        self->actionFunc = BgIceTurara_Fall;
    } else {
        sp28 = Rand_ZeroOne();
        phi_v0_2 = (Rand_ZeroOne() < 0.5f ? -1 : 1);
        self->dyna.actor.world.pos.x = (phi_v0_2 * ((0.5f * sp28) + 0.5f)) + self->dyna.actor.home.pos.x;
        sp28 = Rand_ZeroOne();
        phi_v0_3 = (Rand_ZeroOne() < 0.5f ? -1 : 1);
        self->dyna.actor.world.pos.z = (phi_v0_3 * ((0.5f * sp28) + 0.5f)) + self->dyna.actor.home.pos.z;
    }
}

void BgIceTurara_Fall(BgIceTurara* self, GlobalContext* globalCtx) {
    if ((self->collider.base.atFlags & AT_HIT) || (self->dyna.actor.bgCheckFlags & 1)) {
        self->collider.base.atFlags &= ~AT_HIT;
        self->dyna.actor.bgCheckFlags &= ~1;
        if (self->dyna.actor.world.pos.y < self->dyna.actor.floorHeight) {
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        }
        BgIceTurara_Break(self, globalCtx, 40.0f);
        if (self->dyna.actor.params == TURARA_STALACTITE_REGROW) {
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + 120.0f;
            func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            self->actionFunc = BgIceTurara_Regrow;
        } else {
            Actor_Kill(&self->dyna.actor);
            return;
        }
    } else {
        Actor_MoveForward(&self->dyna.actor);
        self->dyna.actor.world.pos.y += 40.0f;
        Actor_UpdateBgCheckInfo(globalCtx, &self->dyna.actor, 0.0f, 0.0f, 0.0f, 4);
        self->dyna.actor.world.pos.y -= 40.0f;
        Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void BgIceTurara_Regrow(BgIceTurara* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 1.0f)) {
        self->actionFunc = BgIceTurara_Wait;
        self->dyna.actor.velocity.y = 0.0f;
    }
}

void BgIceTurara_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgIceTurara* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgIceTurara_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, object_ice_objects_DL_0023D0);
}
