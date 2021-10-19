#include "z_en_crow.h"
#include "objects/object_crow/object_crow.h"

#define FLAGS 0x00005005

#define THIS ((EnCrow*)thisx)

void EnCrow_Init(Actor* thisx, GlobalContext* globalCtx);
void EnCrow_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnCrow_Update(Actor* thisx, GlobalContext* globalCtx);
void EnCrow_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnCrow_SetupFlyIdle(EnCrow* self);
void EnCrow_FlyIdle(EnCrow* self, GlobalContext* globalCtx);
void EnCrow_Respawn(EnCrow* self, GlobalContext* globalCtx);
void EnCrow_DiveAttack(EnCrow* self, GlobalContext* globalCtx);
void EnCrow_Die(EnCrow* self, GlobalContext* globalCtx);
void EnCrow_TurnAway(EnCrow* self, GlobalContext* globalCtx);
void EnCrow_Damaged(EnCrow* self, GlobalContext* globalCtx);

static Vec3f sZeroVecAccel = { 0.0f, 0.0f, 0.0f };

const ActorInit En_Crow_InitVars = {
    ACTOR_EN_CROW,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_CROW,
    sizeof(EnCrow),
    (ActorFunc)EnCrow_Init,
    (ActorFunc)EnCrow_Destroy,
    (ActorFunc)EnCrow_Update,
    (ActorFunc)EnCrow_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_HARD,
            BUMP_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static CollisionCheckInfoInit sColChkInfoInit = { 1, 15, 30, 30 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(1, 0x0),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(2, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(4, 0x2),
    /* Ice arrow     */ DMG_ENTRY(2, 0x3),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(4, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0x0),
    /* Master spin   */ DMG_ENTRY(2, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0x0),
    /* Master jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static u32 sDeathCount = 0;

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 3000, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x58, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -200, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_STOP),
};

static Vec3f sHeadVec = { 2500.0f, 0.0f, 0.0f };

void EnCrow_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnCrow* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGuaySkel, &gGuayFlyAnim, self->jointTable, self->morphTable, 9);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItems);
    self->collider.elements[0].dim.worldSphere.radius = sJntSphInit.elements[0].dim.modelSphere.radius;
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    ActorShape_Init(&self->actor.shape, 2000.0f, ActorShadow_DrawCircle, 20.0f);
    sDeathCount = 0;
    EnCrow_SetupFlyIdle(self);
}

void EnCrow_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnCrow* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

// Setup Action functions

void EnCrow_SetupFlyIdle(EnCrow* self) {
    self->timer = 100;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = EnCrow_FlyIdle;
    self->skelAnime.playSpeed = 1.0f;
}

void EnCrow_SetupDiveAttack(EnCrow* self) {
    self->timer = 300;
    self->actor.speedXZ = 4.0f;
    self->skelAnime.playSpeed = 2.0f;
    self->actionFunc = EnCrow_DiveAttack;
}

void EnCrow_SetupDamaged(EnCrow* self, GlobalContext* globalCtx) {
    s32 i;
    f32 scale;
    Vec3f iceParticlePos;

    self->actor.speedXZ *= Math_CosS(self->actor.world.rot.x);
    self->actor.velocity.y = 0.0f;
    Animation_Change(&self->skelAnime, &gGuayFlyAnim, 0.4f, 0.0f, 0.0f, ANIMMODE_LOOP_INTERP, -3.0f);
    scale = self->actor.scale.x * 100.0f;
    self->actor.world.pos.y += 20.0f * scale;
    self->actor.bgCheckFlags &= ~1;
    self->actor.shape.yOffset = 0.0f;
    self->actor.targetArrowOffset = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_KAICHO_DEAD);

    if (self->actor.colChkInfo.damageEffect == 3) { // Ice arrows
        Actor_SetColorFilter(&self->actor, 0, 255, 0, 40);
        for (i = 0; i < 8; i++) {
            iceParticlePos.x = ((i & 1 ? 7.0f : -7.0f) * scale) + self->actor.world.pos.x;
            iceParticlePos.y = ((i & 2 ? 7.0f : -7.0f) * scale) + self->actor.world.pos.y;
            iceParticlePos.z = ((i & 4 ? 7.0f : -7.0f) * scale) + self->actor.world.pos.z;
            EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &iceParticlePos, 150, 150, 150, 250, 235, 245, 255,
                                           ((Rand_ZeroOne() * 0.15f) + 0.85f) * scale);
        }
    } else if (self->actor.colChkInfo.damageEffect == 2) { // Fire arrows and Din's Fire
        Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 40);

        for (i = 0; i < 4; i++) {
            EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &self->actor.world.pos, 50.0f * scale, 0, 0, i);
        }
    } else {
        Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 40);
    }

    if (self->actor.flags & 0x8000) {
        self->actor.speedXZ = 0.0f;
    }

    self->collider.base.acFlags &= ~AC_ON;
    self->actor.flags |= 0x10;

    self->actionFunc = EnCrow_Damaged;
}

void EnCrow_SetupDie(EnCrow* self) {
    self->actor.colorFilterTimer = 0;
    self->actionFunc = EnCrow_Die;
}

void EnCrow_SetupTurnAway(EnCrow* self) {
    self->timer = 100;
    self->actor.speedXZ = 3.5f;
    self->aimRotX = -0x1000;
    self->aimRotY = self->actor.yawTowardsPlayer + 0x8000;
    self->skelAnime.playSpeed = 2.0f;
    Actor_SetColorFilter(&self->actor, 0, 255, 0, 5);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->actionFunc = EnCrow_TurnAway;
}

void EnCrow_SetupRespawn(EnCrow* self) {
    if (sDeathCount == 10) {
        self->actor.params = 1;
        sDeathCount = 0;
        self->collider.elements[0].dim.worldSphere.radius =
            sJntSphInit.elements[0].dim.modelSphere.radius * 0.03f * 100.0f;
    } else {
        self->actor.params = 0;
        self->collider.elements[0].dim.worldSphere.radius = sJntSphInit.elements[0].dim.modelSphere.radius;
    }

    Animation_PlayLoop(&self->skelAnime, &gGuayFlyAnim);
    Math_Vec3f_Copy(&self->actor.world.pos, &self->actor.home.pos);
    self->actor.shape.rot.x = 0;
    self->actor.shape.rot.z = 0;
    self->timer = 300;
    self->actor.shape.yOffset = 2000;
    self->actor.targetArrowOffset = 2000.0f;
    self->actor.draw = NULL;
    self->actionFunc = EnCrow_Respawn;
}

// Action functions

void EnCrow_FlyIdle(EnCrow* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 skelanimeUpdated;
    s16 var;

    SkelAnime_Update(&self->skelAnime);
    skelanimeUpdated = Animation_OnFrame(&self->skelAnime, 0.0f);
    self->actor.speedXZ = (Rand_ZeroOne() * 1.5f) + 3.0f;

    if (self->actor.bgCheckFlags & 8) {
        self->aimRotY = self->actor.wallYaw;
    } else if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) > 300.0f) {
        self->aimRotY = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
    }

    if ((Math_SmoothStepToS(&self->actor.shape.rot.y, self->aimRotY, 5, 0x300, 0x10) == 0) && skelanimeUpdated &&
        (Rand_ZeroOne() < 0.1f)) {
        var = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos) - self->actor.shape.rot.y;
        if (var > 0) {
            self->aimRotY += 0x1000 + (0x1000 * Rand_ZeroOne());
        } else {
            self->aimRotY -= 0x1000 + (0x1000 * Rand_ZeroOne());
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_KAICHO_CRY);
    }

    if (self->actor.yDistToWater > -40.0f) {
        self->aimRotX = -0x1000;
    } else if (self->actor.world.pos.y < (self->actor.home.pos.y - 50.0f)) {
        self->aimRotX = -0x800 - (Rand_ZeroOne() * 0x800);
    } else if (self->actor.world.pos.y > (self->actor.home.pos.y + 50.0f)) {
        self->aimRotX = 0x800 + (Rand_ZeroOne() * 0x800);
    }

    if ((Math_SmoothStepToS(&self->actor.shape.rot.x, self->aimRotX, 10, 0x100, 8) == 0) && (skelanimeUpdated) &&
        (Rand_ZeroOne() < 0.1f)) {
        if (self->actor.home.pos.y < self->actor.world.pos.y) {
            self->aimRotX -= (0x400 * Rand_ZeroOne()) + 0x400;
        } else {
            self->aimRotX += (0x400 * Rand_ZeroOne()) + 0x400;
        }
        self->aimRotX = CLAMP(self->aimRotX, -0x1000, 0x1000);
    }

    if (self->actor.bgCheckFlags & 1) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x100, 0x400);
    }

    if (self->timer != 0) {
        self->timer--;
    }
    if ((self->timer == 0) && (self->actor.xzDistToPlayer < 300.0f) && !(player->stateFlags1 & 0x00800000) &&
        (self->actor.yDistToWater < -40.0f) && (Player_GetMask(globalCtx) != PLAYER_MASK_SKULL)) {
        EnCrow_SetupDiveAttack(self);
    }
}

void EnCrow_DiveAttack(EnCrow* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 facingPlayer;
    Vec3f pos;
    s16 target;

    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    facingPlayer = Actor_IsFacingPlayer(&self->actor, 0x2800);

    if (facingPlayer) {
        pos.x = player->actor.world.pos.x;
        pos.y = player->actor.world.pos.y + 20.0f;
        pos.z = player->actor.world.pos.z;
        target = Actor_WorldPitchTowardPoint(&self->actor, &pos);
        if (target > 0x3000) {
            target = 0x3000;
        }
        Math_ApproachS(&self->actor.shape.rot.x, target, 2, 0x400);
    } else {
        Math_ApproachS(&self->actor.shape.rot.x, -0x1000, 2, 0x100);
    }

    if (facingPlayer || (self->actor.xzDistToPlayer > 80.0f)) {
        Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 4, 0xC00);
    }

    if ((self->timer == 0) || (Player_GetMask(globalCtx) == PLAYER_MASK_SKULL) ||
        (self->collider.base.atFlags & AT_HIT) || (self->actor.bgCheckFlags & 9) ||
        (player->stateFlags1 & 0x00800000) || (self->actor.yDistToWater > -40.0f)) {
        if (self->collider.base.atFlags & AT_HIT) {
            self->collider.base.atFlags &= ~AT_HIT;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_KAICHO_ATTACK);
        }

        EnCrow_SetupFlyIdle(self);
    }
}

void EnCrow_Damaged(EnCrow* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    self->actor.colorFilterTimer = 40;

    if (!(self->actor.flags & 0x8000)) {
        if (self->actor.colorFilterParams & 0x4000) {
            Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4000, 0x200);
            self->actor.shape.rot.z += 0x1780;
        }
        if ((self->actor.bgCheckFlags & 1) || (self->actor.floorHeight == BGCHECK_Y_MIN)) {
            EffectSsDeadDb_Spawn(globalCtx, &self->actor.world.pos, &sZeroVecAccel, &sZeroVecAccel,
                                 self->actor.scale.x * 10000.0f, 0, 255, 255, 255, 255, 255, 0, 0, 1, 9, 1);
            EnCrow_SetupDie(self);
        }
    }
}

void EnCrow_Die(EnCrow* self, GlobalContext* globalCtx) {
    f32 step;

    if (self->actor.params != 0) {
        step = 0.006f;
    } else {
        step = 0.002f;
    }

    if (Math_StepToF(&self->actor.scale.x, 0.0f, step)) {
        if (self->actor.params == 0) {
            sDeathCount++;
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0);
        } else {
            Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_RUPEE_RED);
        }
        EnCrow_SetupRespawn(self);
    }

    self->actor.scale.z = self->actor.scale.y = self->actor.scale.x;
}

void EnCrow_TurnAway(EnCrow* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.bgCheckFlags & 8) {
        self->aimRotY = self->actor.wallYaw;
    } else {
        self->aimRotY = self->actor.yawTowardsPlayer + 0x8000;
    }

    Math_ApproachS(&self->actor.shape.rot.y, self->aimRotY, 3, 0xC00);
    Math_ApproachS(&self->actor.shape.rot.x, self->aimRotX, 5, 0x100);

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        EnCrow_SetupFlyIdle(self);
    }
}

void EnCrow_Respawn(EnCrow* self, GlobalContext* globalCtx) {
    f32 target;

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        SkelAnime_Update(&self->skelAnime);
        self->actor.draw = EnCrow_Draw;
        if (self->actor.params != 0) {
            target = 0.03f;
        } else {
            target = 0.01f;
        }
        if (Math_StepToF(&self->actor.scale.x, target, target * 0.1f)) {
            self->actor.flags |= 1;
            self->actor.flags &= ~0x10;
            self->actor.colChkInfo.health = 1;
            EnCrow_SetupFlyIdle(self);
        }
        self->actor.scale.z = self->actor.scale.y = self->actor.scale.x;
    }
}

void EnCrow_UpdateDamage(EnCrow* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.elements[0].info, 1);
        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (self->actor.colChkInfo.damageEffect == 1) { // Deku Nuts
                EnCrow_SetupTurnAway(self);
            } else {
                Actor_ApplyDamage(&self->actor);
                self->actor.flags &= ~1;
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                EnCrow_SetupDamaged(self, globalCtx);
            }
        }
    }
}

void EnCrow_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnCrow* self = THIS;
    f32 pad;
    f32 height;
    f32 scale;

    EnCrow_UpdateDamage(self, globalCtx);
    self->actionFunc(self, globalCtx);
    scale = self->actor.scale.x * 100.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.world.rot.x = -self->actor.shape.rot.x;

    if (self->actionFunc != EnCrow_Respawn) {
        if (self->actor.colChkInfo.health != 0) {
            height = 20.0f * scale;
            func_8002D97C(&self->actor);
        } else {
            height = 0.0f;
            Actor_MoveForward(&self->actor);
        }
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 12.0f * scale, 25.0f * scale, 50.0f * scale, 7);
    } else {
        height = 0.0f;
    }

    self->collider.elements[0].dim.worldSphere.center.x = self->actor.world.pos.x;
    self->collider.elements[0].dim.worldSphere.center.y = self->actor.world.pos.y + height;
    self->collider.elements[0].dim.worldSphere.center.z = self->actor.world.pos.z;

    if (self->actionFunc == EnCrow_DiveAttack) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if (self->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if (self->actionFunc != EnCrow_Respawn) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    Actor_SetFocus(&self->actor, height);

    if (self->actor.colChkInfo.health != 0 && Animation_OnFrame(&self->skelAnime, 3.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_KAICHO_FLUTTER);
    }
}

s32 EnCrow_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnCrow* self = THIS;

    if (self->actor.colChkInfo.health != 0) {
        if (limbIndex == 7) {
            rot->y += 0xC00 * sinf(self->skelAnime.curFrame * (M_PI / 4));
        } else if (limbIndex == 8) {
            rot->y += 0x1400 * sinf((self->skelAnime.curFrame + 2.5f) * (M_PI / 4));
        }
    }
    return false;
}

void EnCrow_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnCrow* self = THIS;
    Vec3f* vec;

    if (limbIndex == 2) {
        Matrix_MultVec3f(&sHeadVec, &self->bodyPartsPos[0]);
        self->bodyPartsPos[0].y -= 20.0f;
    } else if ((limbIndex == 4) || (limbIndex == 6) || (limbIndex == 8)) {
        vec = &self->bodyPartsPos[(limbIndex >> 1) - 1];
        Matrix_MultVec3f(&sZeroVecAccel, vec);
        vec->y -= 20.0f;
    }
}

void EnCrow_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnCrow* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnCrow_OverrideLimbDraw, EnCrow_PostLimbDraw, self);
}
