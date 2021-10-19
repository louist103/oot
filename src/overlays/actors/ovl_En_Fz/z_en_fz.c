#include "z_en_fz.h"
#include "objects/object_fz/object_fz.h"

#define FLAGS 0x00000415

#define THIS ((EnFz*)thisx)

void EnFz_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFz_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFz_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFz_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnFz_UpdateTargetPos(EnFz* self, GlobalContext* globalCtx);

// Stationary Freezard
void EnFz_SetupBlowSmokeStationary(EnFz* self);
void EnFz_BlowSmokeStationary(EnFz* self, GlobalContext* globalCtx);

// Moving Freezard that can vanish and reappear
void EnFz_Wait(EnFz* self, GlobalContext* globalCtx);
void EnFz_SetupAppear(EnFz* self);
void EnFz_Appear(EnFz* self, GlobalContext* globalCtx);
void EnFz_SetupAimForMove(EnFz* self);
void EnFz_AimForMove(EnFz* self, GlobalContext* globalCtx);
void EnFz_SetupMoveTowardsPlayer(EnFz* self);
void EnFz_MoveTowardsPlayer(EnFz* self, GlobalContext* globalCtx);
void EnFz_SetupAimForFreeze(EnFz* self);
void EnFz_AimForFreeze(EnFz* self, GlobalContext* globalCtx);
void EnFz_SetupBlowSmoke(EnFz* self, GlobalContext* globalCtx);
void EnFz_BlowSmoke(EnFz* self, GlobalContext* globalCtx);
void EnFz_SetupDisappear(EnFz* self);
void EnFz_Disappear(EnFz* self, GlobalContext* globalCtx);
void EnFz_SetupWait(EnFz* self);

// Killed with fire source
void EnFz_SetupMelt(EnFz* self);
void EnFz_Melt(EnFz* self, GlobalContext* globalCtx);

// Death
void EnFz_SetupDespawn(EnFz* self, GlobalContext* globalCtx);
void EnFz_Despawn(EnFz* self, GlobalContext* globalCtx);

// Ice Smoke Effects
void EnFz_SpawnIceSmokeNoFreeze(EnFz* self, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 xyScale);
void EnFz_SpawnIceSmokeFreeze(EnFz* self, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 xyScale, f32 xyScaleTarget,
                              s16 primAlpha, u8 isTimerMod8);
void EnFz_UpdateIceSmoke(EnFz* self, GlobalContext* globalCtx);
void EnFz_DrawIceSmoke(EnFz* self, GlobalContext* globalCtx);

const ActorInit En_Fz_InitVars = {
    ACTOR_EN_FZ,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_FZ,
    sizeof(EnFz),
    (ActorFunc)EnFz_Init,
    (ActorFunc)EnFz_Destroy,
    (ActorFunc)EnFz_Update,
    (ActorFunc)EnFz_Draw,
};

static ColliderCylinderInitType1 sCylinderInit1 = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCE0FDB, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 30, 80, 0, { 0, 0, 0 } },
};

static ColliderCylinderInitType1 sCylinderInit2 = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0x0001F024, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 35, 80, 0, { 0, 0, 0 } },
};

static ColliderCylinderInitType1 sCylinderInit3 = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x20000000, 0x02, 0x08 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { 20, 30, -15, { 0, 0, 0 } },
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0xF),
    /* Slingshot     */ DMG_ENTRY(0, 0xF),
    /* Explosive     */ DMG_ENTRY(2, 0xF),
    /* Boomerang     */ DMG_ENTRY(0, 0xF),
    /* Normal arrow  */ DMG_ENTRY(0, 0xF),
    /* Hammer swing  */ DMG_ENTRY(2, 0xF),
    /* Hookshot      */ DMG_ENTRY(2, 0xF),
    /* Kokiri sword  */ DMG_ENTRY(0, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(4, 0x2),
    /* Ice arrow     */ DMG_ENTRY(0, 0xF),
    /* Light arrow   */ DMG_ENTRY(0, 0xF),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0xF),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0xF),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0xF),
    /* Fire magic    */ DMG_ENTRY(4, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0xF),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(0, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x3B, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

void EnFz_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFz* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.colChkInfo.damageTable = &sDamageTable;
    self->actor.colChkInfo.health = 6;

    Collider_InitCylinder(globalCtx, &self->collider1);
    Collider_SetCylinderType1(globalCtx, &self->collider1, &self->actor, &sCylinderInit1);

    Collider_InitCylinder(globalCtx, &self->collider2);
    Collider_SetCylinderType1(globalCtx, &self->collider2, &self->actor, &sCylinderInit2);

    Collider_InitCylinder(globalCtx, &self->collider3);
    Collider_SetCylinderType1(globalCtx, &self->collider3, &self->actor, &sCylinderInit3);

    Actor_SetScale(&self->actor, 0.008f);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.flags &= ~1;
    self->unusedTimer1 = 0;
    self->unusedCounter = 0;
    self->updateBgInfo = true;
    self->isMoving = false;
    self->isFreezing = false;
    self->isActive = true;
    self->isDespawning = false;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->posOrigin.y = self->actor.world.pos.y;
    self->iceSmokeFreezingSpawnHeight = self->actor.world.pos.y;
    self->posOrigin.x = self->actor.world.pos.x;
    self->posOrigin.z = self->actor.world.pos.z;
    self->unusedFloat = 135.0f;

    if (self->actor.params < 0) {
        self->envAlpha = 0;
        self->actor.scale.y = 0.0f;
        EnFz_SetupWait(self);
    } else {
        self->envAlpha = 255;
        EnFz_SetupBlowSmokeStationary(self);
    }

    EnFz_UpdateTargetPos(self, globalCtx);
}

void EnFz_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFz* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider1);
    Collider_DestroyCylinder(globalCtx, &self->collider2);
    Collider_DestroyCylinder(globalCtx, &self->collider3);
}

void EnFz_UpdateTargetPos(EnFz* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Vec3f hitPos;
    Vec3f vec1;
    s32 bgId;
    CollisionPoly* hitPoly;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + 20.0f;
    pos.z = self->actor.world.pos.z;

    Matrix_Translate(pos.x, pos.y, pos.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z, MTXMODE_APPLY);
    vec1.x = vec1.y = 0.0f;
    vec1.z = 220.0f;
    Matrix_MultVec3f(&vec1, &self->wallHitPos);

    if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &pos, &self->wallHitPos, &hitPos, &hitPoly, true, false, false,
                                true, &bgId)) {
        Math_Vec3f_Copy(&self->wallHitPos, &hitPos);
    }

    pos.x = self->actor.world.pos.x - self->wallHitPos.x;
    pos.z = self->actor.world.pos.z - self->wallHitPos.z;

    self->distToTargetSq = SQ(pos.x) + SQ(pos.z);
}

s32 EnFz_ReachedTarget(EnFz* self, Vec3f* vec) {
    if (self->distToTargetSq <= (SQ(self->actor.world.pos.x - vec->x) + SQ(self->actor.world.pos.z - vec->z))) {
        return true;
    } else {
        return false;
    }
}

void EnFz_Damaged(EnFz* self, GlobalContext* globalCtx, Vec3f* vec, s32 numEffects, f32 unkFloat) {
    s32 i;
    Vec3f pos;
    Vec3f vel;
    Vec3f accel;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;
    f32 scale;
    s32 life;

    primColor.r = 155;
    primColor.g = 255;
    primColor.b = 255;
    primColor.a = 255;
    envColor.r = 200;
    envColor.g = 200;
    envColor.b = 200;
    accel.x = accel.z = 0.0f;
    accel.y = -1.0f;

    for (i = 0; i < numEffects; i++) {
        scale = Rand_CenteredFloat(0.3f) + 0.6f;
        life = (s32)Rand_CenteredFloat(5.0f) + 12;
        pos.x = Rand_CenteredFloat(unkFloat) + vec->x;
        pos.y = Rand_ZeroFloat(unkFloat) + vec->y;
        pos.z = Rand_CenteredFloat(unkFloat) + vec->z;
        vel.x = Rand_CenteredFloat(10.0f);
        vel.y = Rand_ZeroFloat(10.0f) + 2.0f;
        vel.z = Rand_CenteredFloat(10.0f);
        EffectSsEnIce_Spawn(globalCtx, &pos, scale, &vel, &accel, &primColor, &envColor, life);
    }

    CollisionCheck_SpawnShieldParticles(globalCtx, vec);
}

void EnFz_SpawnIceSmokeHiddenState(EnFz* self) {
}

// Fully grown
void EnFz_SpawnIceSmokeGrowingState(EnFz* self) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    if ((self->counter % 16) == 0) {
        pos.x = Rand_CenteredFloat(40.0f) + self->actor.world.pos.x;
        pos.y = Rand_CenteredFloat(40.0f) + self->actor.world.pos.y + 30.0f;
        pos.z = Rand_CenteredFloat(40.0f) + self->actor.world.pos.z;
        accel.x = accel.z = 0.0f;
        accel.y = 0.1f;
        velocity.x = velocity.y = velocity.z = 0.0f;
        EnFz_SpawnIceSmokeNoFreeze(self, &pos, &velocity, &accel, Rand_ZeroFloat(7.5f) + 15.0f);
    }
}

// (2) Growing or Shrinking to/from hiding or (3) melting from fire
void EnFz_SpawnIceSmokeActiveState(EnFz* self) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;

    if ((self->counter % 4) == 0) {
        pos.x = Rand_CenteredFloat(40.0f) + self->actor.world.pos.x;
        pos.y = self->iceSmokeFreezingSpawnHeight;
        pos.z = Rand_CenteredFloat(40.0f) + self->actor.world.pos.z;
        accel.x = accel.z = 0.0f;
        accel.y = 0.1f;
        velocity.x = velocity.y = velocity.z = 0.0f;
        EnFz_SpawnIceSmokeNoFreeze(self, &pos, &velocity, &accel, Rand_ZeroFloat(7.5f) + 15.0f);
    }
}

void EnFz_ApplyDamage(EnFz* self, GlobalContext* globalCtx) {
    Vec3f vec;

    if (self->isMoving &&
        ((self->actor.bgCheckFlags & 8) ||
         (Actor_TestFloorInDirection(&self->actor, globalCtx, 60.0f, self->actor.world.rot.y) == 0))) {
        self->actor.bgCheckFlags &= ~8;
        self->isMoving = false;
        self->speedXZ = 0.0f;
        self->actor.speedXZ = 0.0f;
    }

    if (self->isFreezing) {
        if ((self->actor.params < 0) && (self->collider1.base.atFlags & 2)) {
            self->isMoving = false;
            self->collider1.base.acFlags &= ~2;
            self->actor.speedXZ = self->speedXZ = 0.0f;
            self->timer = 10;
            EnFz_SetupDisappear(self);
        } else if (self->collider2.base.acFlags & 0x80) {
            self->collider2.base.acFlags &= ~0x80;
            self->collider1.base.acFlags &= ~2;
        } else if (self->collider1.base.acFlags & 2) {
            self->collider1.base.acFlags &= ~2;
            if (self->actor.colChkInfo.damageEffect != 2) {
                if (self->actor.colChkInfo.damageEffect == 0xF) {
                    Actor_ApplyDamage(&self->actor);
                    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 8);
                    if (self->actor.colChkInfo.health) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FREEZAD_DAMAGE);
                        vec.x = self->actor.world.pos.x;
                        vec.y = self->actor.world.pos.y;
                        vec.z = self->actor.world.pos.z;
                        EnFz_Damaged(self, globalCtx, &vec, 10, 0.0f);
                        self->unusedCounter++;
                    } else {
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FREEZAD_DEAD);
                        Audio_PlayActorSound2(&self->actor, NA_SE_EV_ICE_BROKEN);
                        vec.x = self->actor.world.pos.x;
                        vec.y = self->actor.world.pos.y;
                        vec.z = self->actor.world.pos.z;
                        EnFz_Damaged(self, globalCtx, &vec, 30, 10.0f);
                        EnFz_SetupDespawn(self, globalCtx);
                    }
                }
            } else {
                Actor_ApplyDamage(&self->actor);
                Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 8);
                if (self->actor.colChkInfo.health == 0) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FREEZAD_DEAD);
                    EnFz_SetupMelt(self);
                } else {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FREEZAD_DAMAGE);
                }
            }
        }
    }
}

void EnFz_SetYawTowardsPlayer(EnFz* self) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 10, 2000, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
}

void EnFz_SetupDisappear(EnFz* self) {
    self->state = 2;
    self->isFreezing = false;
    self->actor.flags &= ~1;
    self->actionFunc = EnFz_Disappear;
}

void EnFz_Disappear(EnFz* self, GlobalContext* globalCtx) {
    self->envAlpha -= 16;

    if (self->envAlpha > 255) {
        self->envAlpha = 0;
    }

    if (Math_SmoothStepToF(&self->actor.scale.y, 0.0f, 1.0f, 0.0005f, 0) == 0.0f) {
        EnFz_SetupWait(self);
    }
}

void EnFz_SetupWait(EnFz* self) {
    self->state = 0;
    self->unusedNum2 = 0;
    self->unusedNum1 = 0;
    self->timer = 100;
    self->actionFunc = EnFz_Wait;
    self->actor.world.pos.x = self->posOrigin.x;
    self->actor.world.pos.y = self->posOrigin.y;
    self->actor.world.pos.z = self->posOrigin.z;
}

void EnFz_Wait(EnFz* self, GlobalContext* globalCtx) {
    if ((self->timer == 0) && (self->actor.xzDistToPlayer < 400.0f)) {
        EnFz_SetupAppear(self);
    }
}

void EnFz_SetupAppear(EnFz* self) {
    self->state = 2;
    self->timer = 20;
    self->unusedNum2 = 4000;
    self->actionFunc = EnFz_Appear;
}

void EnFz_Appear(EnFz* self, GlobalContext* globalCtx) {
    if (self->timer == 0) {
        self->envAlpha += 8;
        if (self->envAlpha > 255) {
            self->envAlpha = 255;
        }

        if (Math_SmoothStepToF(&self->actor.scale.y, 0.008f, 1.0f, 0.0005f, 0.0f) == 0.0f) {
            EnFz_SetupAimForMove(self);
        }
    }
}

void EnFz_SetupAimForMove(EnFz* self) {
    self->state = 1;
    self->timer = 40;
    self->updateBgInfo = true;
    self->isFreezing = true;
    self->actor.flags |= 1;
    self->actionFunc = EnFz_AimForMove;
    self->actor.gravity = -1.0f;
}

void EnFz_AimForMove(EnFz* self, GlobalContext* globalCtx) {
    EnFz_SetYawTowardsPlayer(self);

    if (self->timer == 0) {
        EnFz_SetupMoveTowardsPlayer(self);
    }
}

void EnFz_SetupMoveTowardsPlayer(EnFz* self) {
    self->state = 1;
    self->isMoving = true;
    self->timer = 100;
    self->actionFunc = EnFz_MoveTowardsPlayer;
    self->speedXZ = 4.0f;
}

void EnFz_MoveTowardsPlayer(EnFz* self, GlobalContext* globalCtx) {
    if ((self->timer == 0) || !self->isMoving) {
        EnFz_SetupAimForFreeze(self);
    }
}

void EnFz_SetupAimForFreeze(EnFz* self) {
    self->state = 1;
    self->timer = 40;
    self->actionFunc = EnFz_AimForFreeze;
    self->speedXZ = 0.0f;
    self->actor.speedXZ = 0.0f;
}

void EnFz_AimForFreeze(EnFz* self, GlobalContext* globalCtx) {
    EnFz_SetYawTowardsPlayer(self);

    if (self->timer == 0) {
        EnFz_SetupBlowSmoke(self, globalCtx);
    }
}

void EnFz_SetupBlowSmoke(EnFz* self, GlobalContext* globalCtx) {
    self->state = 1;
    self->timer = 80;
    self->actionFunc = EnFz_BlowSmoke;
    EnFz_UpdateTargetPos(self, globalCtx);
}

void EnFz_BlowSmoke(EnFz* self, GlobalContext* globalCtx) {
    Vec3f vec1;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    u8 isTimerMod8;
    s16 primAlpha;

    if (self->timer == 0) {
        EnFz_SetupDisappear(self);
    } else if (self->timer >= 11) {
        isTimerMod8 = false;
        primAlpha = 150;
        func_8002F974(&self->actor, NA_SE_EN_FREEZAD_BREATH - SFX_FLAG);

        if ((self->timer - 10) < 16) { // t < 26
            primAlpha = (self->timer * 10) - 100;
        }

        accel.x = accel.z = 0.0f;
        accel.y = 0.6f;

        pos.x = self->actor.world.pos.x;
        pos.y = self->actor.world.pos.y + 20.0f;
        pos.z = self->actor.world.pos.z;

        Matrix_RotateY((self->actor.shape.rot.y / (f32)0x8000) * M_PI, MTXMODE_NEW);

        vec1.x = 0.0f;
        vec1.y = -2.0f;
        vec1.z = 20.0f; // xz velocity

        Matrix_MultVec3f(&vec1, &velocity);

        if ((self->timer % 8) == 0) {
            isTimerMod8 = true;
        }

        EnFz_SpawnIceSmokeFreeze(self, &pos, &velocity, &accel, 2.0f, 25.0f, primAlpha, isTimerMod8);

        pos.x += (velocity.x * 0.5f);
        pos.y += (velocity.y * 0.5f);
        pos.z += (velocity.z * 0.5f);

        EnFz_SpawnIceSmokeFreeze(self, &pos, &velocity, &accel, 2.0f, 25.0f, primAlpha, false);
    }
}

void EnFz_SetupDespawn(EnFz* self, GlobalContext* globalCtx) {
    self->state = 0;
    self->updateBgInfo = true;
    self->isFreezing = false;
    self->isDespawning = true;
    self->actor.flags &= ~1;
    self->isActive = false;
    self->timer = 60;
    self->speedXZ = 0.0f;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.speedXZ = 0.0f;
    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
    Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x60);
    self->actionFunc = EnFz_Despawn;
}

void EnFz_Despawn(EnFz* self, GlobalContext* globalCtx) {
    if (self->timer == 0) {
        Actor_Kill(&self->actor);
    }
}

void EnFz_SetupMelt(EnFz* self) {
    self->state = 3;
    self->isFreezing = false;
    self->isDespawning = true;
    self->actor.flags &= ~1;
    self->actionFunc = EnFz_Melt;
    self->actor.speedXZ = 0.0f;
    self->speedXZ = 0.0f;
}

void EnFz_Melt(EnFz* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->actor.scale.y, 0.0006f, 0.0002f);

    if (self->actor.scale.y < 0.006f) {
        self->actor.scale.x += 0.0004f;
        self->actor.scale.z += 0.0004f;
    }

    if (self->actor.scale.y < 0.004f) {
        self->envAlpha -= 8;
        if (self->envAlpha > 255) {
            self->envAlpha = 0;
        }
    }

    if (self->envAlpha == 0) {
        EnFz_SetupDespawn(self, globalCtx);
    }
}

void EnFz_SetupBlowSmokeStationary(EnFz* self) {
    self->state = 1;
    self->timer = 40;
    self->updateBgInfo = true;
    self->isFreezing = true;
    self->actor.flags |= 1;
    self->actionFunc = EnFz_BlowSmokeStationary;
    self->actor.gravity = -1.0f;
}

void EnFz_BlowSmokeStationary(EnFz* self, GlobalContext* globalCtx) {
    Vec3f vec1;
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    u8 isTimerMod8;
    s16 primAlpha;

    if (self->counter & 0xC0) {
        EnFz_SetYawTowardsPlayer(self);
        EnFz_UpdateTargetPos(self, globalCtx);
    } else {
        isTimerMod8 = false;
        primAlpha = 150;
        func_8002F974(&self->actor, NA_SE_EN_FREEZAD_BREATH - SFX_FLAG);

        if ((self->counter & 0x3F) >= 48) {
            primAlpha = 630 - ((self->counter & 0x3F) * 10);
        }

        accel.x = accel.z = 0.0f;
        accel.y = 0.6f;

        pos.x = self->actor.world.pos.x;
        pos.y = self->actor.world.pos.y + 20.0f;
        pos.z = self->actor.world.pos.z;

        Matrix_RotateY((self->actor.shape.rot.y / (f32)0x8000) * M_PI, MTXMODE_NEW);

        vec1.x = 0.0f;
        vec1.y = -2.0f;
        vec1.z = 20.0f;

        Matrix_MultVec3f(&vec1, &velocity);

        if ((self->counter % 8) == 0) {
            isTimerMod8 = true;
        }

        EnFz_SpawnIceSmokeFreeze(self, &pos, &velocity, &accel, 2.0f, 25.0f, primAlpha, isTimerMod8);

        pos.x += (velocity.x * 0.5f);
        pos.y += (velocity.y * 0.5f);
        pos.z += (velocity.z * 0.5f);

        EnFz_SpawnIceSmokeFreeze(self, &pos, &velocity, &accel, 2.0f, 25.0f, primAlpha, false);
    }
}

static EnFzSpawnIceSmokeFunc iceSmokeSpawnFuncs[] = {
    EnFz_SpawnIceSmokeHiddenState,
    EnFz_SpawnIceSmokeGrowingState,
    EnFz_SpawnIceSmokeActiveState,
    EnFz_SpawnIceSmokeActiveState,
};

void EnFz_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnFz* self = THIS;
    s32 pad;

    self->counter++;

    if (self->unusedTimer1 != 0) {
        self->unusedTimer1--;
    }

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->unusedTimer2 != 0) {
        self->unusedTimer2--;
    }

    Actor_SetFocus(&self->actor, 50.0f);
    EnFz_ApplyDamage(self, globalCtx);
    self->actionFunc(self, globalCtx);
    if (self->isDespawning == false) {
        Collider_UpdateCylinder(&self->actor, &self->collider1);
        Collider_UpdateCylinder(&self->actor, &self->collider2);
        if (self->isFreezing) {
            if (self->actor.colorFilterTimer == 0) {
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider1.base);
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider2.base);
            }
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider1.base);
        }
    }

    Math_StepToF(&self->actor.speedXZ, self->speedXZ, 0.2f);
    Actor_MoveForward(&self->actor);

    if (self->updateBgInfo) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 20.0f, 5);
    }

    iceSmokeSpawnFuncs[self->state](self);
    EnFz_UpdateIceSmoke(self, globalCtx);
}

void EnFz_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* displayLists[] = {
        gFreezardIntactDL,              // Body fully intact           (5 or 6 health)
        gFreezardTopRightHornChippedDL, // Top right horn chipped off  (from Freezards perspective)   (3 or 4 health)
        gFreezardHeadChippedDL,         // Entire head chipped off     (1 or 2 health)
    };
    EnFz* self = THIS;
    s32 pad;
    s32 index;

    index = (6 - self->actor.colChkInfo.health) >> 1;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fz.c", 1167);

    if (1) {}

    if (self->actor.colChkInfo.health == 0) {
        index = 2;
    }

    if (self->isActive) {
        func_8002ED80(&self->actor, globalCtx, 0);
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, globalCtx->state.frames & 0x7F, 32, 32, 1, 0,
                                    (2 * globalCtx->state.frames) & 0x7F, 32, 32));
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_fz.c", 1183),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetCombineLERP(POLY_XLU_DISP++, TEXEL1, PRIMITIVE, PRIM_LOD_FRAC, TEXEL0, TEXEL1, TEXEL0, PRIMITIVE, TEXEL0,
                          PRIMITIVE, ENVIRONMENT, COMBINED, ENVIRONMENT, COMBINED, 0, ENVIRONMENT, 0);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, 155, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 200, 200, 200, self->envAlpha);
        gSPDisplayList(POLY_XLU_DISP++, displayLists[index]);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fz.c", 1200);
    EnFz_DrawIceSmoke(self, globalCtx);
}

void EnFz_SpawnIceSmokeNoFreeze(EnFz* self, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 xyScale) {
    EnFzEffectSsIceSmoke* iceSmoke = self->iceSmoke;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(self->iceSmoke); i++) {
        if (iceSmoke->type == 0) {
            iceSmoke->type = 1;
            iceSmoke->pos = *pos;
            iceSmoke->velocity = *velocity;
            iceSmoke->accel = *accel;
            iceSmoke->primAlphaState = 0;
            iceSmoke->xyScale = xyScale / 1000.0f;
            iceSmoke->primAlpha = 0;
            iceSmoke->timer = 0;
            break;
        }

        iceSmoke++;
    }
}

void EnFz_SpawnIceSmokeFreeze(EnFz* self, Vec3f* pos, Vec3f* velocity, Vec3f* accel, f32 xyScale, f32 xyScaleTarget,
                              s16 primAlpha, u8 isTimerMod8) {
    EnFzEffectSsIceSmoke* iceSmoke = self->iceSmoke;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(self->iceSmoke); i++) {
        if (iceSmoke->type == 0) {
            iceSmoke->type = 2;
            iceSmoke->pos = *pos;
            iceSmoke->velocity = *velocity;
            iceSmoke->accel = *accel;
            iceSmoke->primAlphaState = 0;
            iceSmoke->xyScale = xyScale / 1000.0f;
            iceSmoke->xyScaleTarget = xyScaleTarget / 1000.0f;
            iceSmoke->primAlpha = primAlpha;
            iceSmoke->timer = 0;
            iceSmoke->isTimerMod8 = isTimerMod8;
            break;
        }

        iceSmoke++;
    }
}

void EnFz_UpdateIceSmoke(EnFz* self, GlobalContext* globalCtx) {
    EnFzEffectSsIceSmoke* iceSmoke = self->iceSmoke;
    s16 i;
    Vec3f pos;

    for (i = 0; i < ARRAY_COUNT(self->iceSmoke); i++) {
        if (iceSmoke->type) {
            iceSmoke->pos.x += iceSmoke->velocity.x;
            iceSmoke->pos.y += iceSmoke->velocity.y;
            iceSmoke->pos.z += iceSmoke->velocity.z;
            iceSmoke->timer++;
            iceSmoke->velocity.x += iceSmoke->accel.x;
            iceSmoke->velocity.y += iceSmoke->accel.y;
            iceSmoke->velocity.z += iceSmoke->accel.z;
            if (iceSmoke->type == 1) {
                if (iceSmoke->primAlphaState == 0) { // Becoming more opaque
                    iceSmoke->primAlpha += 10;
                    if (iceSmoke->primAlpha >= 100) {
                        iceSmoke->primAlphaState++;
                    }
                } else { // Becoming more transparent
                    iceSmoke->primAlpha -= 3;
                    if (iceSmoke->primAlpha <= 0) {
                        iceSmoke->primAlpha = 0;
                        iceSmoke->type = 0;
                    }
                }
            } else if (iceSmoke->type == 2) { // Freezing
                Math_ApproachF(&iceSmoke->xyScale, iceSmoke->xyScaleTarget, 0.1f, iceSmoke->xyScaleTarget / 10.0f);
                if (iceSmoke->primAlphaState == 0) { // Becoming more opaque
                    if (iceSmoke->timer >= 7) {
                        iceSmoke->primAlphaState++;
                    }
                } else { // Becoming more transparent, slows down
                    iceSmoke->accel.y = 2.0f;
                    iceSmoke->primAlpha -= 17;
                    iceSmoke->velocity.x *= 0.75f;
                    iceSmoke->velocity.z *= 0.75f;
                    if (iceSmoke->primAlpha <= 0) {
                        iceSmoke->primAlpha = 0;
                        iceSmoke->type = 0;
                    }
                }

                if ((self->unusedTimer2 == 0) && (iceSmoke->primAlpha >= 101) && iceSmoke->isTimerMod8) {
                    self->collider3.dim.pos.x = (s16)iceSmoke->pos.x;
                    self->collider3.dim.pos.y = (s16)iceSmoke->pos.y;
                    self->collider3.dim.pos.z = (s16)iceSmoke->pos.z;
                    CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider3.base);
                }

                pos.x = iceSmoke->pos.x;
                pos.y = iceSmoke->pos.y + 10.0f;
                pos.z = iceSmoke->pos.z;

                if ((iceSmoke->primAlphaState != 2) && EnFz_ReachedTarget(self, &pos)) {
                    iceSmoke->primAlphaState = 2;
                    iceSmoke->velocity.x = 0.0f;
                    iceSmoke->velocity.z = 0.0f;
                }
            }
        }
        iceSmoke++;
    }
}

void EnFz_DrawIceSmoke(EnFz* self, GlobalContext* globalCtx) {
    EnFzEffectSsIceSmoke* iceSmoke = self->iceSmoke;
    s16 i;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    u8 texLoaded = false;

    OPEN_DISPS(gfxCtx, "../z_en_fz.c", 1384);

    func_80093D84(globalCtx->state.gfxCtx);

    for (i = 0; i < ARRAY_COUNT(self->iceSmoke); i++) {
        if (iceSmoke->type > 0) {
            gDPPipeSync(POLY_XLU_DISP++);

            if (!texLoaded) {
                gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(&gFreezardSteamStartDL));
                texLoaded++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 195, 225, 235, iceSmoke->primAlpha);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 3 * (iceSmoke->timer + (3 * i)),
                                        15 * (iceSmoke->timer + (3 * i)), 32, 64, 1, 0, 0, 32, 32));
            Matrix_Translate(iceSmoke->pos.x, iceSmoke->pos.y, iceSmoke->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(iceSmoke->xyScale, iceSmoke->xyScale, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_en_fz.c", 1424),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(&gFreezardSteamDL));
        }

        iceSmoke++;
    }

    CLOSE_DISPS(gfxCtx, "../z_en_fz.c", 1430);
}
