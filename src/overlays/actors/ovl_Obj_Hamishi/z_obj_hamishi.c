/*
 * File: z_obj_hamishi.c
 * Overlay: ovl_Obj_Hamishi
 * Description: Bronze Boulder
 */

#include "z_obj_hamishi.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

#define FLAGS 0x00000000

#define THIS ((ObjHamishi*)thisx)

void ObjHamishi_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjHamishi_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjHamishi_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjHamishi_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Obj_Hamishi_InitVars = {
    ACTOR_OBJ_HAMISHI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(ObjHamishi),
    (ActorFunc)ObjHamishi_Init,
    (ActorFunc)ObjHamishi_Destroy,
    (ActorFunc)ObjHamishi_Update,
    (ActorFunc)ObjHamishi_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HARD,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x4FC1FFF6, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 50, 70, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE };

static s16 sEffectScales[] = {
    145, 135, 115, 85, 75, 53, 45, 40, 35,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 250, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 500, ICHAIN_STOP),
};

void ObjHamishi_InitCollision(Actor* thisx, GlobalContext* globalCtx) {
    ObjHamishi* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);
}

void ObjHamishi_Shake(ObjHamishi* self) {
    if (self->shakeFrames > 0) {
        self->shakeFrames--;
        self->shakePosPhase += 5000;
        self->shakeRotPhase += 0xE10;

        Math_StepToF(&self->shakePosSize, 0.0f, 0.15f);
        Math_StepToF(&self->shakeRotSize, 0.0f, 40.0f);

        self->actor.world.pos.x = self->actor.home.pos.x + (Math_SinS(self->shakePosPhase * 4) * self->shakePosSize);
        self->actor.world.pos.z = self->actor.home.pos.z + (Math_CosS(self->shakePosPhase * 7) * self->shakePosSize);
        self->actor.shape.rot.x =
            self->actor.home.rot.x + (s16)(Math_SinS(self->shakeRotPhase * 4) * self->shakeRotSize);
        self->actor.shape.rot.z =
            self->actor.home.rot.z + (s16)(Math_CosS(self->shakeRotPhase * 7) * self->shakeRotSize);
    } else {
        Math_StepToF(&self->actor.world.pos.x, self->actor.home.pos.x, 1.0f);
        Math_StepToF(&self->actor.world.pos.z, self->actor.home.pos.z, 1.0f);
        Math_ScaledStepToS(&self->actor.shape.rot.x, self->actor.home.rot.x, 0xBB8);
        Math_ScaledStepToS(&self->actor.shape.rot.z, self->actor.home.rot.z, 0xBB8);
    }
}

void ObjHamishi_Break(ObjHamishi* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f velocity;
    Vec3f pos;
    s16 phi_s0 = 1000;
    s16 gravity;
    s16 phi_v0;
    f32 temp_f20;
    f32 temp_f22;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sEffectScales); i++) {
        phi_s0 += 20000;

        temp_f20 = Rand_ZeroOne() * 10.0f;
        pos.x = (Math_SinS(phi_s0) * temp_f20) + self->actor.world.pos.x;
        pos.y = (Rand_ZeroOne() * 40.0f) + self->actor.world.pos.y + 5.0f;
        pos.z = (Math_CosS(phi_s0) * temp_f20) + self->actor.world.pos.z;

        temp_f20 = (Rand_ZeroOne() * 10.0f) + 2.0f;
        velocity.x = Math_SinS(phi_s0) * temp_f20;
        temp_f22 = Rand_ZeroOne();
        velocity.y = (Rand_ZeroOne() * i * 2.5f) + (temp_f22 * 15.0f);
        velocity.z = Math_CosS(phi_s0) * temp_f20;

        if (i == 0) {
            phi_v0 = 41;
            gravity = -450;
        } else if (i < 4) {
            phi_v0 = 37;
            gravity = -380;
        } else {
            phi_v0 = 69;
            gravity = -320;
        }

        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &self->actor.world.pos, gravity, phi_v0, 30, 5, 0,
                             sEffectScales[i], 3, 0, 70, 1, OBJECT_GAMEPLAY_FIELD_KEEP, gSilverRockFragmentsDL);
    }

    func_80033480(globalCtx, &self->actor.world.pos, 140.0f, 6, 180, 90, 1);
    func_80033480(globalCtx, &self->actor.world.pos, 140.0f, 12, 80, 90, 1);
}

void ObjHamishi_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjHamishi* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        self->actor.uncullZoneForward += 1000.0f;
    }
    if (self->actor.shape.rot.y == 0) {
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.home.rot.y = Rand_ZeroFloat(65536.0f);
    }

    ObjHamishi_InitCollision(&self->actor, globalCtx);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);

    if (Flags_GetSwitch(globalCtx, self->actor.params & 0x3F)) {
        Actor_Kill(&self->actor);
        return;
    }

    self->actor.shape.yOffset = 80.0f;
}

void ObjHamishi_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    ObjHamishi* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void ObjHamishi_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjHamishi* self = THIS;
    CollisionCheckContext* colChkCtx = &globalCtx->colChkCtx;

    ObjHamishi_Shake(self);

    if ((self->collider.base.acFlags & AC_HIT) && (self->collider.info.acHitInfo->toucher.dmgFlags & 0x40000040)) {
        self->collider.base.acFlags &= ~AC_HIT;
        self->hitCount++;
        if (self->hitCount < 2) {
            self->shakeFrames = 15;
            self->shakePosSize = 2.0f;
            self->shakeRotSize = 400.0f;
        } else {
            ObjHamishi_Break(self, globalCtx);
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
            Flags_SetSwitch(globalCtx, self->actor.params & 0x3F);
            Actor_Kill(&self->actor);
        }
    } else {
        self->collider.base.acFlags &= ~AC_HIT;

        if (self->actor.xzDistToPlayer < 600.0f) {
            CollisionCheck_SetAC(globalCtx, colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(globalCtx, colChkCtx, &self->collider.base);
        }
    }
}

void ObjHamishi_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_hamishi.c", 399);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_hamishi.c", 404),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 170, 130, 255);
    gSPDisplayList(POLY_OPA_DISP++, gSilverRockDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_hamishi.c", 411);
}
