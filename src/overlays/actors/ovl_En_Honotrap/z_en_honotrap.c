/**
 * File: z_en_honotrap.c
 * Overlay: ovl_En_Honotrap
 * Description: Fake eye switches and Dampe flames
 */

#include "z_en_honotrap.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"

#define FLAGS 0x00000010

#define THIS ((EnHonotrap*)thisx)

#define HONOTRAP_AT_ACTIVE (1 << 0)
#define HONOTRAP_AC_ACTIVE (1 << 1)
#define HONOTRAP_OC_ACTIVE (1 << 2)

typedef enum {
    /* 0 */ HONOTRAP_EYE_OPEN,
    /* 1 */ HONOTRAP_EYE_HALF,
    /* 2 */ HONOTRAP_EYE_CLOSE,
    /* 3 */ HONOTRAP_EYE_SHUT
} EnHonotrapEyeState;

void EnHonotrap_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHonotrap_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHonotrap_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHonotrap_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnHonotrap_SetupEyeIdle(EnHonotrap* self);
void EnHonotrap_EyeIdle(EnHonotrap* self, GlobalContext* globalCtx);
void EnHonotrap_SetupEyeOpen(EnHonotrap* self);
void EnHonotrap_EyeOpen(EnHonotrap* self, GlobalContext* globalCtx);
void EnHonotrap_SetupEyeAttack(EnHonotrap* self);
void EnHonotrap_EyeAttack(EnHonotrap* self, GlobalContext* globalCtx);
void EnHonotrap_SetupEyeClose(EnHonotrap* self);
void EnHonotrap_EyeClose(EnHonotrap* self, GlobalContext* globalCtx);

void EnHonotrap_SetupFlame(EnHonotrap* self);
void EnHonotrap_Flame(EnHonotrap* self, GlobalContext* globalCtx);
void EnHonotrap_SetupFlameDrop(EnHonotrap* self);
void EnHonotrap_FlameDrop(EnHonotrap* self, GlobalContext* globalCtx);

void EnHonotrap_SetupFlameMove(EnHonotrap* self);
void EnHonotrap_FlameMove(EnHonotrap* self, GlobalContext* globalCtx);
void EnHonotrap_SetupFlameChase(EnHonotrap* self);
void EnHonotrap_FlameChase(EnHonotrap* self, GlobalContext* globalCtx);
void EnHonotrap_SetupFlameVanish(EnHonotrap* self);
void EnHonotrap_FlameVanish(EnHonotrap* self, GlobalContext* globalCtx);

const ActorInit En_Honotrap_InitVars = {
    ACTOR_EN_HONOTRAP,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(EnHonotrap),
    (ActorFunc)EnHonotrap_Init,
    (ActorFunc)EnHonotrap_Destroy,
    (ActorFunc)EnHonotrap_Update,
    (ActorFunc)EnHonotrap_Draw,
};

static ColliderTrisElementInit sTrisElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK4,
            { 0x00000000, 0x00, 0x00 },
            { 0x0001F824, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 0.0f, 23.0f, 8.5f }, { -23.0f, 0.0f, 8.5f }, { 0.0f, -23.0f, 8.5f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x0001F824, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 0.0f, 23.0f, 8.5f }, { 0.0f, -23.0f, 8.5f }, { 23.0f, 0.0f, 8.5f } } },
    },
};

static ColliderTrisInit sTrisInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    sTrisElementsInit,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x01, 0x04 },
        { 0x00100000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 10, 25, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 9, 23, 1 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void EnHonotrap_FlameCollisionCheck(EnHonotrap* self, GlobalContext* globalCtx) {
    s32 pad[3];

    Collider_UpdateCylinder(&self->actor, &self->collider.cyl);
    CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.cyl.base);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.cyl.base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.cyl.base);
    self->colChkFlags |= HONOTRAP_AT_ACTIVE;
    self->colChkFlags |= HONOTRAP_AC_ACTIVE;
    self->colChkFlags |= HONOTRAP_OC_ACTIVE;
}

void EnHonotrap_GetNormal(Vec3f* normal, Vec3f* vec) {
    f32 mag = Math3D_Vec3fMagnitude(vec);

    if (mag < 0.001f) {
        osSyncPrintf("Warning : vector size zero (%s %d)\n", "../z_en_honotrap.c", 328, normal);

        normal->x = normal->y = 0.0f;
        normal->z = 1.0f;
    } else {
        normal->x = vec->x * (1.0f / mag);
        normal->y = vec->y * (1.0f / mag);
        normal->z = vec->z * (1.0f / mag);
    }
}

void EnHonotrap_InitEye(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHonotrap* self = THIS;
    s32 i;
    s32 j;
    Vec3f* vtx;
    Vec3f triangle[3];
    f32 cos;
    f32 sin;

    Actor_SetScale(thisx, 0.1f);
    sin = Math_SinS(thisx->home.rot.y);
    cos = Math_CosS(thisx->home.rot.y);
    Collider_InitTris(globalCtx, &self->collider.tris);
    Collider_SetTris(globalCtx, &self->collider.tris, thisx, &sTrisInit, self->collider.elements);

    for (i = 0; i < 2; i++) {
        for (j = 0, vtx = triangle; j < 3; j++, vtx++) {
            Vec3f* baseVtx = &sTrisInit.elements[i].dim.vtx[j];

            vtx->x = baseVtx->z * sin + baseVtx->x * cos;
            vtx->y = baseVtx->y;
            vtx->z = baseVtx->z * cos - baseVtx->x * sin;
            Math_Vec3f_Sum(vtx, &thisx->world.pos, vtx);
        }
        Collider_SetTrisVertices(&self->collider.tris, i, &triangle[0], &triangle[1], &triangle[2]);
    }
    EnHonotrap_SetupEyeIdle(self);
    Actor_SetFocus(thisx, 0.0f);
}

void EnHonotrap_InitFlame(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHonotrap* self = THIS;

    Actor_SetScale(&self->actor, 0.0001f);
    Collider_InitCylinder(globalCtx, &self->collider.cyl);
    Collider_SetCylinder(globalCtx, &self->collider.cyl, &self->actor, &sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider.cyl);
    self->actor.minVelocityY = -1.0f;
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->actor.shape.shadowAlpha = 128;
    self->targetPos = GET_PLAYER(globalCtx)->actor.world.pos;
    self->targetPos.y += 10.0f;
    self->flameScroll = Rand_ZeroOne() * 511.0f;
    EnHonotrap_SetupFlame(self);
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_FLAME_IGNITION);
    if (self->actor.params == HONOTRAP_FLAME_DROP) {
        self->actor.room = -1;
        self->collider.cyl.dim.radius = 12;
        self->collider.cyl.dim.height = 30;
        self->actor.shape.yOffset = -1000.0f;
    }
}

void EnHonotrap_Init(Actor* thisx, GlobalContext* globalCtx) {
    Actor_ProcessInitChain(thisx, sInitChain);
    if (thisx->params == HONOTRAP_EYE) {
        EnHonotrap_InitEye(thisx, globalCtx);
    } else {
        EnHonotrap_InitFlame(thisx, globalCtx);
    }
}

void EnHonotrap_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHonotrap* self = THIS;

    if (self->actor.params == HONOTRAP_EYE) {
        Collider_DestroyTris(globalCtx, &self->collider.tris);
    } else {
        Collider_DestroyCylinder(globalCtx, &self->collider.cyl);
    }
}

void EnHonotrap_SetupEyeIdle(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_EyeIdle;
    self->eyeState = HONOTRAP_EYE_SHUT;
}

void EnHonotrap_EyeIdle(EnHonotrap* self, GlobalContext* globalCtx) {
    if (self->actor.child != NULL) {
        self->timer = 200;
    } else if ((self->timer <= 0) && (self->actor.xzDistToPlayer < 750.0f) && (0.0f > self->actor.yDistToPlayer) &&
               (self->actor.yDistToPlayer > -700.0f) &&
               (-0x4000 < (self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) &&
               ((self->actor.yawTowardsPlayer - self->actor.shape.rot.y) < 0x4000)) {
        EnHonotrap_SetupEyeOpen(self);
    }
}

void EnHonotrap_SetupEyeOpen(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_EyeOpen;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0x28);
    self->timer = 30;
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_RED_EYE);
}

void EnHonotrap_EyeOpen(EnHonotrap* self, GlobalContext* globalCtx) {
    f32 cos;
    f32 sin;

    self->eyeState--;
    if (self->eyeState <= HONOTRAP_EYE_OPEN) {
        EnHonotrap_SetupEyeAttack(self);
        sin = Math_SinS(self->actor.shape.rot.y);
        cos = Math_CosS(self->actor.shape.rot.y);
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_HONOTRAP,
                           (sin * 12.0f) + self->actor.home.pos.x, self->actor.home.pos.y - 10.0f,
                           (cos * 12.0f) + self->actor.home.pos.z, self->actor.home.rot.x, self->actor.home.rot.y,
                           self->actor.home.rot.z, HONOTRAP_FLAME_MOVE);
    }
}

void EnHonotrap_SetupEyeAttack(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_EyeAttack;
    self->eyeState = HONOTRAP_EYE_OPEN;
}

void EnHonotrap_EyeAttack(EnHonotrap* self, GlobalContext* globalCtx) {
    if (self->timer <= 0) {
        EnHonotrap_SetupEyeClose(self);
    }
}

void EnHonotrap_SetupEyeClose(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_EyeClose;
}

void EnHonotrap_EyeClose(EnHonotrap* self, GlobalContext* globalCtx) {
    self->eyeState++;
    if (self->eyeState >= HONOTRAP_EYE_SHUT) {
        EnHonotrap_SetupEyeIdle(self);
        self->timer = 200;
    }
}

void EnHonotrap_SetupFlame(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_Flame;
}

void EnHonotrap_Flame(EnHonotrap* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 ready =
        Math_StepToF(&self->actor.scale.x, (self->actor.params == HONOTRAP_FLAME_MOVE) ? 0.004f : 0.0048f, 0.0006f);

    self->actor.scale.z = self->actor.scale.y = self->actor.scale.x;
    if (ready) {
        if (self->actor.params == HONOTRAP_FLAME_MOVE) {
            EnHonotrap_SetupFlameMove(self);
        } else {
            EnHonotrap_SetupFlameDrop(self);
        }
    }
}

void EnHonotrap_SetupFlameDrop(EnHonotrap* self) {
    self->timer = 40;
    self->actor.velocity.y = 1.0f;
    self->actor.velocity.x = 2.0f * Math_SinS(self->actor.world.rot.y);
    self->actor.velocity.z = 2.0f * Math_CosS(self->actor.world.rot.y);
    self->actionFunc = EnHonotrap_FlameDrop;
}

void EnHonotrap_FlameDrop(EnHonotrap* self, GlobalContext* globalCtx) {
    if ((self->collider.cyl.base.atFlags & AT_HIT) || (self->timer <= 0)) {
        if ((self->collider.cyl.base.atFlags & AT_HIT) && !(self->collider.cyl.base.atFlags & AT_BOUNCED)) {
            func_8002F71C(globalCtx, &self->actor, 5.0f, self->actor.yawTowardsPlayer, 0.0f);
        }
        self->actor.velocity.x = self->actor.velocity.y = self->actor.velocity.z = 0.0f;
        EnHonotrap_SetupFlameVanish(self);
    } else {
        if (self->actor.velocity.y > 0.0f) {
            self->actor.world.pos.x += self->actor.velocity.x;
            self->actor.world.pos.z += self->actor.velocity.z;
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 7.0f, 12.0f, 0.0f, 5);
        }
        if (!Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight + 1.0f, self->actor.velocity.y)) {
            self->actor.velocity.y += 1.0f;
        } else {
            self->actor.velocity.y = 0.0f;
        }
        EnHonotrap_FlameCollisionCheck(self, globalCtx);
    }
}

void EnHonotrap_SetupFlameMove(EnHonotrap* self) {
    f32 distFrac;

    self->actionFunc = EnHonotrap_FlameMove;

    distFrac = 1.0f / (Actor_WorldDistXYZToPoint(&self->actor, &self->targetPos) + 1.0f);
    self->actor.velocity.x = (self->targetPos.x - self->actor.world.pos.x) * distFrac;
    self->actor.velocity.y = (self->targetPos.y - self->actor.world.pos.y) * distFrac;
    self->actor.velocity.z = (self->targetPos.z - self->actor.world.pos.z) * distFrac;
    self->speedMod = 0.0f;

    self->timer = 160;
}

void EnHonotrap_FlameMove(EnHonotrap* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f speed;
    s32 ready;

    Math_StepToF(&self->speedMod, 13.0f, 0.5f);
    speed.x = fabsf(self->speedMod * self->actor.velocity.x);
    speed.y = fabsf(self->speedMod * self->actor.velocity.y);
    speed.z = fabsf(self->speedMod * self->actor.velocity.z);
    ready = true;
    ready &= Math_StepToF(&self->actor.world.pos.x, self->targetPos.x, speed.x);
    ready &= Math_StepToF(&self->actor.world.pos.y, self->targetPos.y, speed.y);
    ready &= Math_StepToF(&self->actor.world.pos.z, self->targetPos.z, speed.z);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 7.0f, 10.0f, 0.0f, 0x1D);

    if (self->collider.tris.base.atFlags & AT_BOUNCED) {
        Player* player = GET_PLAYER(globalCtx);
        Vec3f shieldNorm;
        Vec3f tempVel;
        Vec3f shieldVec;

        shieldVec.x = -player->shieldMf.xz;
        shieldVec.y = -player->shieldMf.yz;
        shieldVec.z = -player->shieldMf.zz;
        EnHonotrap_GetNormal(&shieldNorm, &shieldVec);

        tempVel = self->actor.velocity;
        Math3D_Vec3fReflect(&tempVel, &shieldNorm, &self->actor.velocity);
        self->actor.speedXZ = self->speedMod * 0.5f;
        self->actor.world.rot.y = Math_Atan2S(self->actor.velocity.z, self->actor.velocity.x);
        EnHonotrap_SetupFlameVanish(self);
    } else if (self->collider.tris.base.atFlags & AT_HIT) {
        self->actor.velocity.y = self->actor.speedXZ = 0.0f;
        EnHonotrap_SetupFlameVanish(self);
    } else if (self->timer <= 0) {
        EnHonotrap_SetupFlameVanish(self);
    } else {
        EnHonotrap_FlameCollisionCheck(self, globalCtx);
        if (ready) {
            EnHonotrap_SetupFlameChase(self);
        }
    }
}

void EnHonotrap_SetupFlameChase(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_FlameChase;

    self->actor.velocity.x = self->actor.velocity.y = self->actor.velocity.z = self->actor.speedXZ = 0.0f;
    self->actor.world.rot.x = self->actor.world.rot.y = self->actor.world.rot.z = 0;

    self->timer = 100;
}

void EnHonotrap_FlameChase(EnHonotrap* self, GlobalContext* globalCtx) {
    s32 pad;

    Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0x300);
    Math_StepToF(&self->actor.speedXZ, 3.0f, 0.1f);
    self->actor.gravity = (-self->actor.yDistToPlayer < 10.0f) ? 0.08f : -0.08f;
    func_8002D868(&self->actor);
    if (self->actor.velocity.y > 1.0f) {
        self->actor.velocity.y = 1.0f;
    }
    self->actor.velocity.y *= 0.95f;
    func_8002D7EC(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 7.0f, 10.0f, 0.0f, 0x1D);
    if (self->collider.cyl.base.atFlags & AT_BOUNCED) {
        Player* player = GET_PLAYER(globalCtx);
        Vec3s shieldRot;

        Matrix_MtxFToYXZRotS(&player->shieldMf, &shieldRot, false);
        self->actor.world.rot.y = ((shieldRot.y * 2) - self->actor.world.rot.y) + 0x8000;
        EnHonotrap_SetupFlameVanish(self);
    } else if (self->collider.cyl.base.atFlags & AT_HIT) {
        self->actor.speedXZ *= 0.1f;
        self->actor.velocity.y *= 0.1f;
        EnHonotrap_SetupFlameVanish(self);
    } else if ((self->actor.bgCheckFlags & 8) || (self->timer <= 0)) {
        EnHonotrap_SetupFlameVanish(self);
    } else {
        EnHonotrap_FlameCollisionCheck(self, globalCtx);
    }
}

void EnHonotrap_SetupFlameVanish(EnHonotrap* self) {
    self->actionFunc = EnHonotrap_FlameVanish;
}

void EnHonotrap_FlameVanish(EnHonotrap* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 ready = Math_StepToF(&self->actor.scale.x, 0.0001f, 0.00015f);

    self->actor.scale.z = self->actor.scale.y = self->actor.scale.x;
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 7.0f, 10.0f, 0.0f, 0x1D);
    if (ready) {
        Actor_Kill(&self->actor);
    }
}

void EnHonotrap_Update(Actor* thisx, GlobalContext* globalCtx) {
    static Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    static Vec3f accel = { 0.0f, 0.1f, 0.0f };
    s32 pad;
    EnHonotrap* self = THIS;

    if (self->timer > 0) {
        self->timer--;
    }
    if (self->actor.params == HONOTRAP_EYE) {
        if ((self->actor.child != NULL) && (self->actor.child->update == NULL)) {
            self->actor.child = NULL;
        }
    } else {
        self->colChkFlags = 0;
        self->bobPhase += 0x640;
        self->actor.shape.yOffset = (Math_SinS(self->bobPhase) * 1000.0f) + 600.0f;
        Actor_SetFocus(&self->actor, 5.0f);
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_BURN_OUT - SFX_FLAG);
    }
    self->actionFunc(self, globalCtx);
    if (self->actor.params == HONOTRAP_EYE) {
        if (self->collider.tris.base.acFlags & AC_HIT) {
            EffectSsBomb2_SpawnLayered(globalCtx, &self->actor.world.pos, &velocity, &accel, 15, 8);
            Actor_Kill(&self->actor);
        } else if (self->eyeState < HONOTRAP_EYE_SHUT) {
            self->collider.tris.base.acFlags &= ~AC_HIT;
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.tris.base);
        }
    }
}

void EnHonotrap_DrawEye(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = {
        gEyeSwitchSilverOpenTex,
        gEyeSwitchSilverHalfTex,
        gEyeSwitchSilverClosedTex,
        gEyeSwitchSilverClosedTex,
    };
    EnHonotrap* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_honotrap.c", 982);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeState]));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_honotrap.c", 987),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gEyeSwitch2DL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_honotrap.c", 991);
}

void EnHonotrap_DrawFlame(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHonotrap* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_honotrap.c", 1000);

    func_80093D84(globalCtx->state.gfxCtx);
    self->flameScroll -= 20;
    self->flameScroll &= 0x1FF;
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0, self->flameScroll, 0x20, 0x80));
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 200, 0, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
    Matrix_RotateY((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) - self->actor.shape.rot.y + 0x8000) *
                       (M_PI / 0x8000),
                   MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_honotrap.c", 1024),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_honotrap.c", 1028);
}

void EnHonotrap_Draw(Actor* thisx, GlobalContext* globalCtx) {
    switch (thisx->params) {
        case HONOTRAP_EYE:
            EnHonotrap_DrawEye(thisx, globalCtx);
            break;
        case HONOTRAP_FLAME_MOVE:
        case HONOTRAP_FLAME_DROP:
            EnHonotrap_DrawFlame(thisx, globalCtx);
            break;
    }
}
