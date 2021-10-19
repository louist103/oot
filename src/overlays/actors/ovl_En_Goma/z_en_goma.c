#include "z_en_goma.h"
#include "objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"
#include "objects/object_gol/object_gol.h"
#include "overlays/actors/ovl_Boss_Goma/z_boss_goma.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"

#define FLAGS 0x00000035

#define THIS ((EnGoma*)thisx)

void EnGoma_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGoma_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGoma_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGoma_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnGoma_Flee(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_EggFallToGround(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Egg(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Hatch(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Hurt(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Die(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Dead(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_PrepareJump(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Land(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Jump(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Stand(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_ChasePlayer(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Stunned(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_LookAtPlayer(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_UpdateHit(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_Debris(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_SpawnHatchDebris(EnGoma* self, GlobalContext* globalCtx2);
void EnGoma_BossLimb(EnGoma* self, GlobalContext* globalCtx);

void EnGoma_SetupFlee(EnGoma* self);
void EnGoma_SetupHatch(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_SetupHurt(EnGoma* self, GlobalContext* globalCtx);
void EnGoma_SetupDie(EnGoma* self);
void EnGoma_SetupDead(EnGoma* self);
void EnGoma_SetupStand(EnGoma* self);
void EnGoma_SetupChasePlayer(EnGoma* self);
void EnGoma_SetupPrepareJump(EnGoma* self);
void EnGoma_SetupLand(EnGoma* self);
void EnGoma_SetupJump(EnGoma* self);
void EnGoma_SetupStunned(EnGoma* self, GlobalContext* globalCtx);

const ActorInit En_Goma_InitVars = {
    ACTOR_BOSS_GOMA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GOL,
    sizeof(EnGoma),
    (ActorFunc)EnGoma_Init,
    (ActorFunc)EnGoma_Destroy,
    (ActorFunc)EnGoma_Update,
    (ActorFunc)EnGoma_Draw,
};

static ColliderCylinderInit D_80A4B7A0 = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFDFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 15, 30, 10, { 0, 0, 0 } },
};

static ColliderCylinderInit D_80A4B7CC = {
    {
        COLTYPE_HIT3,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFDFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 15, 30, 10, { 0, 0, 0 } },
};

static u8 sSpawnNum = 0;
static Vec3f sDeadEffectVel = { 0.0f, 0.0f, 0.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 3, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x03, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 20, ICHAIN_STOP),
};

void EnGoma_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGoma* self = THIS;
    s16 params;

    self->eggTimer = Rand_ZeroOne() * 200.0f;
    Actor_ProcessInitChain(&self->actor, sInitChain);
    Actor_SetScale(&self->actor, 0.01f);
    params = self->actor.params;

    if (params >= 100) { // piece of boss goma
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_BOSS);
        self->actionFunc = EnGoma_BossLimb;
        self->gomaType = ENGOMA_BOSSLIMB;
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
        self->actionTimer = self->actor.params + 150;
        self->actor.flags &= ~1;
    } else if (params >= 10) { // Debris when hatching
        self->actor.gravity = -1.3f;
        self->actor.flags &= ~1;
        self->actionTimer = 50;
        self->gomaType = ENGOMA_HATCH_DEBRIS;
        self->eggScale = 1.0f;
        self->actor.velocity.y = Rand_ZeroOne() * 5.0f + 5.0f;
        self->actionFunc = EnGoma_Debris;
        self->actor.speedXZ = Rand_ZeroOne() * 2.3f + 1.5f;
        self->actionTimer = 30;
        self->actor.scale.x = Rand_ZeroOne() * 0.005f + 0.01f;
        self->actor.scale.y = Rand_ZeroOne() * 0.005f + 0.01f;
        self->actor.scale.z = Rand_ZeroOne() * 0.005f + 0.01f;
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    } else { // Egg
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);
        SkelAnime_Init(globalCtx, &self->skelanime, &gObjectGolSkel, &gObjectGolStandAnim, self->jointTable,
                       self->morphTable, GOMA_LIMB_MAX);
        Animation_PlayLoop(&self->skelanime, &gObjectGolStandAnim);
        self->actor.colChkInfo.health = 2;

        if (self->actor.params < 3) { // Spawned by boss
            self->actionFunc = EnGoma_EggFallToGround;
            self->invincibilityTimer = 10;
            self->actor.speedXZ = 1.5f;
        } else if (self->actor.params == 8 || self->actor.params == 6) {
            self->actionFunc = EnGoma_Egg;
            self->spawnNum = sSpawnNum++;
        } else if (self->actor.params == 9 || self->actor.params == 7) {
            self->actionFunc = EnGoma_Egg;
        }

        if (self->actor.params >= 8) { // on ceiling
            self->eggYOffset = -1500.0f;
        } else {
            self->eggYOffset = 1500.0f;
        }

        self->gomaType = ENGOMA_EGG;
        self->eggScale = 1.0f;
        self->eggSquishAngle = Rand_ZeroOne() * 1000.0f;
        self->actionTimer = 50;
        Collider_InitCylinder(globalCtx, &self->colCyl1);
        Collider_SetCylinder(globalCtx, &self->colCyl1, &self->actor, &D_80A4B7A0);
        Collider_InitCylinder(globalCtx, &self->colCyl2);
        Collider_SetCylinder(globalCtx, &self->colCyl2, &self->actor, &D_80A4B7CC);
    }
}

void EnGoma_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGoma* self = THIS;

    if (self->actor.params < 10) {
        Collider_DestroyCylinder(globalCtx, &self->colCyl1);
        Collider_DestroyCylinder(globalCtx, &self->colCyl2);
    }
}

void EnGoma_SetupFlee(EnGoma* self) {
    Animation_Change(&self->skelanime, &gObjectGolRunningAnim, 2.0f, 0.0f,
                     Animation_GetLastFrame(&gObjectGolRunningAnim), ANIMMODE_LOOP, -2.0f);
    self->actionFunc = EnGoma_Flee;
    self->actionTimer = 20;

    if (self->actor.params < 6) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_DAM2);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_DAM2);
    }
}

void EnGoma_Flee(EnGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    Math_ApproachF(&self->actor.speedXZ, 6.6666665f, 0.5f, 2.0f);
    Math_ApproachS(&self->actor.world.rot.y,
                   Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) + 0x8000, 3, 2000);
    Math_ApproachS(&self->actor.shape.rot.y, self->actor.world.rot.y, 2, 3000);

    if (self->actionTimer == 0) {
        EnGoma_SetupStand(self);
    }
}

void EnGoma_EggFallToGround(EnGoma* self, GlobalContext* globalCtx) {
    self->actor.gravity = -1.3f;
    self->eggSquishAccel += 0.03f;
    self->eggSquishAngle += 1.0f + self->eggSquishAccel;
    Math_ApproachZeroF(&self->eggSquishAmount, 1.0f, 0.005f);
    Math_ApproachF(&self->eggYOffset, 1500.0f, 1.0f, 150.0f);

    switch (self->hatchState) {
        case 0:
            if (self->actor.bgCheckFlags & 1) { // floor
                if (self->actor.params < 6) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_EGG1);
                } else {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_EGG1);
                }

                if (self->actor.params > 5) {
                    EnGoma_SetupHatch(self, globalCtx);
                } else {
                    self->hatchState = 1;
                    self->actionTimer = 3;
                    Math_ApproachF(&self->eggScale, 1.5f, 0.5f, 1.0f);
                }
            }
            break;

        case 1:
            if (self->actionTimer == 0) {
                self->hatchState = 2;
                self->actionTimer = 3;
                Math_ApproachF(&self->eggScale, 0.75f, 0.5f, 1.0f);
                self->actor.velocity.y = 5.0f;
                self->actor.speedXZ = 2.0f;
            } else {
                Math_ApproachF(&self->eggScale, 1.5f, 0.5f, 1.0f);
            }
            break;

        case 2:
            if (self->actionTimer == 0) {
                self->hatchState = 3;
                self->actionTimer = 80;
            } else {
                Math_ApproachF(&self->eggScale, 0.75f, 0.5f, 1.0f);
            }
            break;

        case 3:
            Math_ApproachF(&self->eggScale, 1.0f, 0.1f, 0.1f);
            if (self->actionTimer == 0) {
                EnGoma_SetupHatch(self, globalCtx);
            }
            break;
    }

    if (self->actor.bgCheckFlags & 1) {
        Math_ApproachZeroF(&self->actor.speedXZ, 0.2f, 0.05f);
    }
    self->eggPitch += (self->actor.speedXZ * 0.1f);
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnGoma_Egg(EnGoma* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 i;

    self->eggSquishAngle += 1.0f;
    Math_ApproachF(&self->eggSquishAmount, 0.1f, 1.0f, 0.005f);
    if (fabsf(self->actor.world.pos.x - player->actor.world.pos.x) < 100.0f &&
        fabsf(self->actor.world.pos.z - player->actor.world.pos.z) < 100.0f) {
        if (++self->playerDetectionTimer > 9) {
            self->actionFunc = EnGoma_EggFallToGround;
        }
    } else {
        self->playerDetectionTimer = 0;
    }

    if (!(self->eggTimer & 0xF) && Rand_ZeroOne() < 0.5f) {
        for (i = 0; i < 2; i++) {
            Vec3f vel = { 0.0f, 0.0f, 0.0f };
            Vec3f acc = { 0.0f, -0.5f, 0.0f };
            Vec3f pos;

            pos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
            pos.y = Rand_ZeroFloat(30.0f) + self->actor.world.pos.y;
            pos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;
            EffectSsHahen_Spawn(globalCtx, &pos, &vel, &acc, 0, (s16)(Rand_ZeroOne() * 5.0f) + 10, HAHEN_OBJECT_DEFAULT,
                                10, NULL);
        }
    }
}

void EnGoma_SetupHatch(EnGoma* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelanime, &gObjectGolJumpHeadbuttAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gObjectGolJumpHeadbuttAnim), ANIMMODE_ONCE, 0.0f);
    self->actionFunc = EnGoma_Hatch;
    Actor_SetScale(&self->actor, 0.005f);
    self->gomaType = ENGOMA_NORMAL;
    self->actionTimer = 5;
    self->actor.shape.rot.y = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnGoma_SpawnHatchDebris(self, globalCtx);
    self->eggScale = 1.0f;
    self->actor.speedXZ = 0.0f;
}

void EnGoma_Hatch(EnGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    if (self->actionTimer == 0) {
        EnGoma_SetupStand(self);
    }
}

void EnGoma_SetupHurt(EnGoma* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelanime, &gObjectGolDamagedAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gObjectGolDamagedAnim), ANIMMODE_ONCE, -2.0f);
    self->actionFunc = EnGoma_Hurt;

    if ((s8)self->actor.colChkInfo.health <= 0) {
        self->actionTimer = 5;
        Enemy_StartFinishingBlow(globalCtx, &self->actor);
    } else {
        self->actionTimer = 10;
    }

    self->actor.speedXZ = 20.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer + 0x8000;
    if (self->actor.params < 6) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_DAM1);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_DAM1);
    }
}

void EnGoma_Hurt(EnGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->actor.bgCheckFlags & 1) {
        Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);
    }

    if (self->actionTimer == 0) {
        if ((s8)self->actor.colChkInfo.health <= 0) {
            EnGoma_SetupDie(self);
        } else {
            EnGoma_SetupFlee(self);
        }
    }
}

void EnGoma_SetupDie(EnGoma* self) {
    Animation_Change(&self->skelanime, &gObjectGolDeathAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gObjectGolDeathAnim),
                     ANIMMODE_ONCE, -2.0f);
    self->actionFunc = EnGoma_Die;
    self->actionTimer = 30;

    if (self->actor.params < 6) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_DEAD);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_DEAD);
    }

    self->invincibilityTimer = 100;
    self->actor.flags &= ~1;
}

void EnGoma_Die(EnGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->actor.bgCheckFlags & 1) {
        Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);
    }

    if (self->actionTimer == 17) {
        if (self->actor.params < 6) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_LAND);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_LAND);
        }
    }

    if (self->actionTimer == 0) {
        EnGoma_SetupDead(self);
    }
}

void EnGoma_SetupDead(EnGoma* self) {
    Animation_Change(&self->skelanime, &gObjectGolDeadTwitchingAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gObjectGolDeadTwitchingAnim), ANIMMODE_LOOP, -2.0f);
    self->actionFunc = EnGoma_Dead;
    self->actionTimer = 3;
}

void EnGoma_Dead(EnGoma* self, GlobalContext* globalCtx) {
    Vec3f accel;
    Vec3f pos;

    SkelAnime_Update(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);

    if (self->actionTimer == 2) {
        pos.x = self->actor.world.pos.x;
        pos.y = (self->actor.world.pos.y + 5.0f) - 10.0f;
        pos.z = self->actor.world.pos.z;
        accel = sDeadEffectVel;
        accel.y = 0.03f;
        EffectSsKFire_Spawn(globalCtx, &pos, &sDeadEffectVel, &accel, 40, 0);
    }

    if (self->actionTimer == 0 && Math_SmoothStepToF(&self->actor.scale.y, 0.0f, 0.5f, 0.00225f, 0.00001f) <= 0.001f) {
        if (self->actor.params < 6) {
            BossGoma* parent = (BossGoma*)self->actor.parent;
            parent->childrenGohmaState[self->actor.params] = -1;
        }
        Audio_PlaySoundGeneral(NA_SE_EN_EXTINCT, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        Actor_Kill(&self->actor);
        Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, 0x30);
    }
    self->visualState = 2;
}

void EnGoma_SetupStand(EnGoma* self) {
    f32 lastFrame;

    lastFrame = Animation_GetLastFrame(&gObjectGolStandAnim);
    self->actionTimer = Rand_S16Offset(10, 30);
    Animation_Change(&self->skelanime, &gObjectGolStandAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP, -5.0f);
    self->actionFunc = EnGoma_Stand;
    self->gomaType = ENGOMA_NORMAL;
}

void EnGoma_SetupChasePlayer(EnGoma* self) {
    Animation_Change(&self->skelanime, &gObjectGolRunningAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gObjectGolRunningAnim), ANIMMODE_LOOP, -5.0f);
    self->actionFunc = EnGoma_ChasePlayer;
    self->actionTimer = Rand_S16Offset(70, 110);
}

void EnGoma_SetupPrepareJump(EnGoma* self) {
    Animation_Change(&self->skelanime, &gObjectGolPrepareJumpAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gObjectGolPrepareJumpAnim), ANIMMODE_ONCE, -5.0f);
    self->actionFunc = EnGoma_PrepareJump;
    self->actionTimer = 30;
}

void EnGoma_PrepareJump(EnGoma* self, GlobalContext* globalCtx) {
    s16 targetAngle;

    SkelAnime_Update(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);

    targetAngle = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor);
    Math_ApproachS(&self->actor.world.rot.y, targetAngle, 2, 4000);
    Math_ApproachS(&self->actor.shape.rot.y, targetAngle, 2, 3000);

    if (self->actionTimer == 0) {
        EnGoma_SetupJump(self);
    }
    self->visualState = 0;
}

void EnGoma_SetupLand(EnGoma* self) {
    Animation_Change(&self->skelanime, &gObjectGolLandFromJumpAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gObjectGolLandFromJumpAnim), ANIMMODE_ONCE, 0.0f);
    self->actionFunc = EnGoma_Land;
    self->actionTimer = 10;
}

void EnGoma_Land(EnGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->actor.bgCheckFlags & 1) {
        Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 2.0f);
    }
    if (self->actionTimer == 0) {
        EnGoma_SetupStand(self);
    }
}

void EnGoma_SetupJump(EnGoma* self) {
    Animation_Change(&self->skelanime, &gObjectGolJumpHeadbuttAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gObjectGolJumpHeadbuttAnim), ANIMMODE_ONCE, 0.0f);
    self->actionFunc = EnGoma_Jump;
    self->actor.velocity.y = 8.0f;

    if (self->actor.params < 6) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_CRY);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_CRY);
    }
}

void EnGoma_Jump(EnGoma* self, GlobalContext* globalCtx) {
    self->actor.flags |= 0x1000000;
    SkelAnime_Update(&self->skelanime);
    Math_ApproachF(&self->actor.speedXZ, 10.0f, 0.5f, 5.0f);

    if (self->actor.velocity.y <= 0.0f && (self->actor.bgCheckFlags & 1)) {
        EnGoma_SetupLand(self);
        if (self->actor.params < 6) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_LAND2);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_LAND2);
        }
    }
    self->visualState = 0;
}

void EnGoma_Stand(EnGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);
    Math_ApproachS(&self->actor.shape.rot.y, Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor), 2,
                   3000);

    if (self->actionTimer == 0) {
        EnGoma_SetupChasePlayer(self);
    }
}

void EnGoma_ChasePlayer(EnGoma* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (Animation_OnFrame(&self->skelanime, 1.0f) || Animation_OnFrame(&self->skelanime, 5.0f)) {
        if (self->actor.params < 6) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_WALK);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_WALK);
        }
    }

    Math_ApproachF(&self->actor.speedXZ, 3.3333333f, 0.5f, 2.0f);
    Math_ApproachS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 3, 2000);
    Math_ApproachS(&self->actor.shape.rot.y, self->actor.world.rot.y, 2, 3000);

    if (self->actor.bgCheckFlags & 1) {
        self->actor.velocity.y = 0.0f;
    }
    if (self->actor.xzDistToPlayer <= 150.0f) {
        EnGoma_SetupPrepareJump(self);
    }
}

void EnGoma_SetupStunned(EnGoma* self, GlobalContext* globalCtx) {
    self->actionFunc = EnGoma_Stunned;
    self->stunTimer = 100;
    Animation_MorphToLoop(&self->skelanime, &gObjectGolStandAnim, -5.0f);
    self->actionTimer = (s16)Rand_ZeroFloat(15.0f) + 3;

    if (self->actor.params < 6) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_BJR_FREEZE);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    }
}

void EnGoma_Stunned(EnGoma* self, GlobalContext* globalCtx) {
    Actor_SetColorFilter(&self->actor, 0, 180, 0, 2);
    self->visualState = 2;

    if (self->actionTimer != 0) {
        SkelAnime_Update(&self->skelanime);
    }

    if (self->actor.bgCheckFlags & 1) {
        self->actor.velocity.y = 0.0f;
        Math_ApproachZeroF(&self->actor.speedXZ, 0.5f, 2.0f);
    }

    if (self->stunTimer == 0) {
        EnGoma_SetupStand(self);
    } else if (--self->stunTimer < 30) {
        if (self->stunTimer & 1) {
            self->actor.world.pos.x += 1.5f;
            self->actor.world.pos.z += 1.5f;
        } else {
            self->actor.world.pos.x -= 1.5f;
            self->actor.world.pos.z -= 1.5f;
        }
    }
}

void EnGoma_LookAtPlayer(EnGoma* self, GlobalContext* globalCtx) {
    s16 eyePitch;
    s16 eyeYaw;

    eyeYaw = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.shape.rot.y;
    eyePitch = Actor_WorldPitchTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.shape.rot.x;

    if (eyeYaw > 6000) {
        eyeYaw = 6000;
    }
    if (eyeYaw < -6000) {
        eyeYaw = -6000;
    }
    Math_ApproachS(&self->eyeYaw, eyeYaw, 3, 2000);
    Math_ApproachS(&self->eyePitch, eyePitch, 3, 2000);
}

void EnGoma_UpdateHit(EnGoma* self, GlobalContext* globalCtx) {
    static Vec3f sShieldKnockbackVel = { 0.0f, 0.0f, 20.0f };
    Player* player = GET_PLAYER(globalCtx);

    if (self->hurtTimer != 0) {
        self->hurtTimer--;
    } else {
        ColliderInfo* acHitInfo;
        u8 swordDamage;

        if ((self->colCyl1.base.atFlags & 2) && self->actionFunc == EnGoma_Jump) {
            EnGoma_SetupLand(self);
            self->actor.speedXZ = 0.0f;
            self->actor.velocity.y = 0.0f;
        }

        if ((self->colCyl2.base.acFlags & AC_HIT) && (s8)self->actor.colChkInfo.health > 0) {
            acHitInfo = self->colCyl2.info.acHitInfo;
            self->colCyl2.base.acFlags &= ~AC_HIT;

            if (self->gomaType == ENGOMA_NORMAL) {
                u32 dmgFlags = acHitInfo->toucher.dmgFlags;

                if (dmgFlags & 0x100000) {
                    if (self->actionFunc == EnGoma_Jump) {
                        EnGoma_SetupLand(self);
                        self->actor.velocity.y = 0.0f;
                        self->actor.speedXZ = -5.0f;
                    } else {
                        Matrix_RotateY(player->actor.shape.rot.y / 32768.0f * 3.1415927f, MTXMODE_NEW);
                        Matrix_MultVec3f(&sShieldKnockbackVel, &self->shieldKnockbackVel);
                        self->invincibilityTimer = 5;
                    }
                } else if (dmgFlags & 1) { // stun
                    if (self->actionFunc != EnGoma_Stunned) {
                        EnGoma_SetupStunned(self, globalCtx);
                        self->hurtTimer = 8;
                    }
                } else {
                    swordDamage = CollisionCheck_GetSwordDamage(dmgFlags);

                    if (swordDamage) {
                        EffectSsSibuki_SpawnBurst(globalCtx, &self->actor.focus.pos);
                    } else {
                        swordDamage = 1;
                    }

                    self->actor.colChkInfo.health -= swordDamage;
                    EnGoma_SetupHurt(self, globalCtx);
                    Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 5);
                    self->hurtTimer = 13;
                }
            } else {
                // die if still an egg
                if (self->actor.params <= 5) { //! BossGoma only has 3 children
                    BossGoma* parent = (BossGoma*)self->actor.parent;
                    parent->childrenGohmaState[self->actor.params] = -1;
                }

                EnGoma_SpawnHatchDebris(self, globalCtx);
                Actor_Kill(&self->actor);
            }
        }
    }
}

void EnGoma_UpdateEyeEnvColor(EnGoma* self) {
    static f32 sTargetEyeEnvColors[][3] = {
        { 255.0f, 0.0f, 50.0f },
        { 17.0f, 255.0f, 50.0f },
        { 0.0f, 170.0f, 50.0f },
    };

    Math_ApproachF(&self->eyeEnvColor[0], sTargetEyeEnvColors[0][self->visualState], 0.5f, 20.0f);
    Math_ApproachF(&self->eyeEnvColor[1], sTargetEyeEnvColors[1][self->visualState], 0.5f, 20.0f);
    Math_ApproachF(&self->eyeEnvColor[2], sTargetEyeEnvColors[2][self->visualState], 0.5f, 20.0f);
}

void EnGoma_SetFloorRot(EnGoma* self) {
    f32 nx;
    f32 ny;
    f32 nz;

    if (self->actor.floorPoly != NULL) {
        nx = COLPOLY_GET_NORMAL(self->actor.floorPoly->normal.x);
        ny = COLPOLY_GET_NORMAL(self->actor.floorPoly->normal.y);
        nz = COLPOLY_GET_NORMAL(self->actor.floorPoly->normal.z);
        Math_ApproachS(&self->slopePitch, -Math_FAtan2F(-nz * ny, 1.0f) * 10430.378f, 1, 1000);
        Math_ApproachS(&self->slopeRoll, Math_FAtan2F(-nx * ny, 1.0f) * 10430.378f, 1, 1000);
    }
}

void EnGoma_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGoma* self = THIS;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (self->invincibilityTimer != 0) {
        self->invincibilityTimer--;
    }

    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    self->actor.world.pos.x = self->actor.world.pos.x + self->shieldKnockbackVel.x;
    self->actor.world.pos.z = self->actor.world.pos.z + self->shieldKnockbackVel.z;
    Math_ApproachZeroF(&self->shieldKnockbackVel.x, 1.0f, 3.0f);
    Math_ApproachZeroF(&self->shieldKnockbackVel.z, 1.0f, 3.0f);

    if (self->actor.params < 10) {
        self->eggTimer++;
        Math_SmoothStepToF(&self->actor.scale.x, 0.01f, 0.5f, 0.00075f, 0.000001f);
        Math_SmoothStepToF(&self->actor.scale.y, 0.01f, 0.5f, 0.00075f, 0.000001f);
        Math_SmoothStepToF(&self->actor.scale.z, 0.01f, 0.5f, 0.00075f, 0.000001f);
        EnGoma_UpdateHit(self, globalCtx);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 50.0f, 100.0f, 5);
        EnGoma_SetFloorRot(self);
        Actor_SetFocus(&self->actor, 20.0f);
        EnGoma_LookAtPlayer(self, globalCtx);
        EnGoma_UpdateEyeEnvColor(self);
        self->visualState = 1;
        if (player->swordState != 0) {
            self->colCyl2.dim.radius = 35;
            self->colCyl2.dim.height = 35;
            self->colCyl2.dim.yShift = 0;
        } else {
            self->colCyl2.dim.radius = 15;
            self->colCyl2.dim.height = 30;
            self->colCyl2.dim.yShift = 10;
        }
        if (self->invincibilityTimer == 0) {
            Collider_UpdateCylinder(&self->actor, &self->colCyl1);
            Collider_UpdateCylinder(&self->actor, &self->colCyl2);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colCyl1.base);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCyl2.base);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colCyl1.base);
        }
    }
}

s32 EnGoma_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnGoma* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_goma.c", 1976);
    gDPSetEnvColor(POLY_OPA_DISP++, (s16)self->eyeEnvColor[0], (s16)self->eyeEnvColor[1], (s16)self->eyeEnvColor[2],
                   255);

    if (limbIndex == GOMA_LIMB_EYE_IRIS_ROOT1) {
        rot->x += self->eyePitch;
        rot->y += self->eyeYaw;
    } else if (limbIndex == GOMA_LIMB_BODY && self->hurtTimer != 0) {
        gDPSetEnvColor(POLY_OPA_DISP++, (s16)(Rand_ZeroOne() * 255.0f), (s16)(Rand_ZeroOne() * 255.0f),
                       (s16)(Rand_ZeroOne() * 255.0f), 255);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_goma.c", 2011);
    return 0;
}

Gfx* EnGoma_NoBackfaceCullingDlist(GraphicsContext* gfxCtx) {
    Gfx* dListHead;
    Gfx* dList;

    dListHead = dList = Graph_Alloc(gfxCtx, sizeof(Gfx) * 4);
    gDPPipeSync(dListHead++);
    gDPSetRenderMode(dListHead++, G_RM_PASS, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(dListHead++, G_CULL_BACK);
    gSPEndDisplayList(dListHead++);
    return dList;
}

void EnGoma_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnGoma* self = THIS;
    s32 y;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_goma.c", 2040);
    func_80093D18(globalCtx->state.gfxCtx);

    switch (self->gomaType) {
        case ENGOMA_NORMAL:
            self->actor.naviEnemyId = 0x03;
            Matrix_Translate(self->actor.world.pos.x,
                             self->actor.world.pos.y + ((self->actor.shape.yOffset * self->actor.scale.y) +
                                                        globalCtx->mainCamera.skyboxOffset.y),
                             self->actor.world.pos.z, MTXMODE_NEW);
            Matrix_RotateX(self->slopePitch / 32768.0f * 3.1415927f, MTXMODE_APPLY);
            Matrix_RotateZ(self->slopeRoll / 32768.0f * 3.1415927f, MTXMODE_APPLY);
            Matrix_RotateY(self->actor.shape.rot.y / 32768.0f * 3.1415927f, MTXMODE_APPLY);
            Matrix_RotateX(self->actor.shape.rot.x / 32768.0f * 3.1415927f, MTXMODE_APPLY);
            Matrix_RotateZ(self->actor.shape.rot.z / 32768.0f * 3.1415927f, MTXMODE_APPLY);
            Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
            SkelAnime_DrawOpa(globalCtx, self->skelanime.skeleton, self->skelanime.jointTable, EnGoma_OverrideLimbDraw,
                              NULL, self);
            break;

        case ENGOMA_EGG:
            self->actor.naviEnemyId = 0x02;
            y = (s16)(sinf((self->eggTimer * 5.0f * 3.1415f) / 180.0f) * 31.9f);
            y = (s16)(y + 31);
            gSPSegment(POLY_OPA_DISP++, 0x08, func_80094E78(globalCtx->state.gfxCtx, 0, y));
            Matrix_Push();
            Matrix_Scale(self->eggScale, 1.0f / self->eggScale, self->eggScale, MTXMODE_APPLY);
            Matrix_RotateY(self->eggSquishAngle * 0.15f, MTXMODE_APPLY);
            Matrix_RotateZ(self->eggSquishAngle * 0.1f, MTXMODE_APPLY);
            Matrix_Scale(0.95f - self->eggSquishAmount, self->eggSquishAmount + 1.05f, 0.95f - self->eggSquishAmount,
                         MTXMODE_APPLY);
            Matrix_RotateZ(-(self->eggSquishAngle * 0.1f), MTXMODE_APPLY);
            Matrix_RotateY(-(self->eggSquishAngle * 0.15f), MTXMODE_APPLY);
            Matrix_Translate(0.0f, self->eggYOffset, 0.0f, MTXMODE_APPLY);
            Matrix_RotateX(self->eggPitch, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_goma.c", 2101),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gObjectGolEggDL);
            Matrix_Pop();
            break;

        case ENGOMA_HATCH_DEBRIS:
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_goma.c", 2107),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gBrownFragmentDL);
            break;

        case ENGOMA_BOSSLIMB:
            if (self->bossLimbDl != NULL) {
                gSPSegment(POLY_OPA_DISP++, 0x08, EnGoma_NoBackfaceCullingDlist(globalCtx->state.gfxCtx));
                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_goma.c", 2114),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_OPA_DISP++, self->bossLimbDl);
            }
            break;
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_goma.c", 2119);
}

void EnGoma_Debris(EnGoma* self, GlobalContext* globalCtx) {
    self->actor.shape.rot.y += 2500;
    self->actor.shape.rot.x += 3500;
    if (self->actionTimer == 0) {
        Actor_Kill(&self->actor);
    }
}

void EnGoma_SpawnHatchDebris(EnGoma* self, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    s16 i;

    if (self->actor.params < 6) {
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 0x28, NA_SE_EN_GOMA_BJR_EGG2);
    } else {
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 0x28, NA_SE_EN_GOMA_EGG2);
    }

    for (i = 0; i < 15; i++) {
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_GOMA,
                           Rand_CenteredFloat(10.0f) + self->actor.world.pos.x,
                           Rand_CenteredFloat(10.0f) + self->actor.world.pos.y + 15.0f,
                           Rand_CenteredFloat(10.0f) + self->actor.world.pos.z, 0, Rand_CenteredFloat(65535.99f), 0,
                           i + 10);
    }
}

void EnGoma_BossLimb(EnGoma* self, GlobalContext* globalCtx) {
    Vec3f vel = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 1.0f, 0.0f };
    Color_RGBA8 primColor = { 255, 255, 255, 255 };
    Color_RGBA8 envColor = { 0, 100, 255, 255 };
    Vec3f pos;

    self->actor.world.pos.y -= 5.0f;
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 50.0f, 100.0f, 4);
    self->actor.world.pos.y += 5.0f;

    if (self->actor.bgCheckFlags & 1) {
        self->actor.velocity.y = 0.0f;
    } else if (self->actionTimer < 250) {
        self->actor.shape.rot.y += 2000;
    }

    if (self->actionTimer == 250) {
        self->actor.gravity = -1.0f;
    }

    if (self->actionTimer < 121) {
        if (Math_SmoothStepToF(&self->actor.scale.y, 0.0f, 1.0f, 0.00075f, 0) <= 0.001f) {
            Actor_Kill(&self->actor);
        }
        self->actor.scale.x = self->actor.scale.z = self->actor.scale.y;
    }

    if (self->actionTimer % 8 == 0 && self->actionTimer != 0) {
        pos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
        pos.y = Rand_CenteredFloat(10.0f) + self->actor.world.pos.y;
        pos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
        func_8002836C(globalCtx, &pos, &vel, &accel, &primColor, &envColor, 500, 10, 10);
    }
}
