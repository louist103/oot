/*
 * File: z_en_firefly.c
 * Overlay: ovl_En_Firefly
 * Description: Keese (Normal, Fire, Ice)
 */

#include "z_en_firefly.h"
#include "objects/object_firefly/object_firefly.h"
#include "overlays/actors/ovl_Obj_Syokudai/z_obj_syokudai.h"

#define FLAGS 0x00005005

#define THIS ((EnFirefly*)thisx)

void EnFirefly_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFirefly_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFirefly_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFirefly_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnFirefly_DrawInvisible(Actor* thisx, GlobalContext* globalCtx);

void EnFirefly_FlyIdle(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_Fall(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_Die(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_DiveAttack(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_Rebound(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_FlyAway(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_Stunned(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_FrozenFall(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_Perch(EnFirefly* self, GlobalContext* globalCtx);
void EnFirefly_DisturbDiveAttack(EnFirefly* self, GlobalContext* globalCtx);

typedef enum {
    /* 0 */ KEESE_AURA_NONE,
    /* 1 */ KEESE_AURA_FIRE,
    /* 2 */ KEESE_AURA_ICE
} KeeseAuraType;

const ActorInit En_Firefly_InitVars = {
    ACTOR_EN_FIREFLY,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_FIREFLY,
    sizeof(EnFirefly),
    (ActorFunc)EnFirefly_Init,
    (ActorFunc)EnFirefly_Destroy,
    (ActorFunc)EnFirefly_Update,
    (ActorFunc)EnFirefly_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_HARD,
            BUMP_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 1000, 0 }, 15 }, 100 },
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

static CollisionCheckInfoInit sColChkInfoInit = { 1, 10, 10, 30 };

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
    /* Fire arrow    */ DMG_ENTRY(2, 0xF),
    /* Ice arrow     */ DMG_ENTRY(4, 0x3),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x2),
    /* Ice magic     */ DMG_ENTRY(4, 0x3),
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

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 5, ICHAIN_CONTINUE),  ICHAIN_F32_DIV1000(gravity, -500, ICHAIN_CONTINUE),
    ICHAIN_F32(minVelocityY, -4, ICHAIN_CONTINUE),    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 4000, ICHAIN_STOP),
};

void EnFirefly_Extinguish(EnFirefly* self) {
    self->actor.params += 2;
    self->collider.elements[0].info.toucher.effect = 0; // None
    self->auraType = KEESE_AURA_NONE;
    self->onFire = false;
    self->actor.naviEnemyId = 0x12; // Keese
}

void EnFirefly_Ignite(EnFirefly* self) {
    if (self->actor.params == KEESE_ICE_FLY) {
        self->actor.params = KEESE_FIRE_FLY;
    } else {
        self->actor.params -= 2;
    }
    self->collider.elements[0].info.toucher.effect = 1; // Fire
    self->auraType = KEESE_AURA_FIRE;
    self->onFire = true;
    self->actor.naviEnemyId = 0x11; // Fire Keese
}

void EnFirefly_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFirefly* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gKeeseSkeleton, &gKeeseFlyAnim, self->jointTable, self->morphTable,
                   28);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItems);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    if ((self->actor.params & 0x8000) != 0) {
        self->actor.flags |= 0x80;
        if (1) {}
        self->actor.draw = EnFirefly_DrawInvisible;
        self->actor.params &= 0x7FFF;
    }

    if (self->actor.params <= KEESE_FIRE_PERCH) {
        self->onFire = true;
    } else {
        self->onFire = false;
    }

    if (self->onFire) {
        self->actionFunc = EnFirefly_FlyIdle;
        self->timer = Rand_S16Offset(20, 60);
        self->actor.shape.rot.x = 0x1554;
        self->auraType = KEESE_AURA_FIRE;
        self->actor.naviEnemyId = 0x11; // Fire Keese
        self->maxAltitude = self->actor.home.pos.y;
    } else {
        if (self->actor.params == KEESE_NORMAL_PERCH) {
            self->actionFunc = EnFirefly_Perch;
        } else {
            self->actionFunc = EnFirefly_FlyIdle;
        }

        if (self->actor.params == KEESE_ICE_FLY) {
            self->collider.elements[0].info.toucher.effect = 2; // Ice
            self->actor.naviEnemyId = 0x56;                     // Ice Keese
        } else {
            self->collider.elements[0].info.toucher.effect = 0; // Nothing
            self->actor.naviEnemyId = 0x12;                     // Keese
        }

        self->maxAltitude = self->actor.home.pos.y + 100.0f;

        if (self->actor.params == KEESE_ICE_FLY) {
            self->auraType = KEESE_AURA_ICE;
        } else {
            self->auraType = KEESE_AURA_NONE;
        }
    }

    self->collider.elements[0].dim.worldSphere.radius = sJntSphInit.elements[0].dim.modelSphere.radius;
}

void EnFirefly_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFirefly* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnFirefly_SetupFlyIdle(EnFirefly* self) {
    self->timer = Rand_S16Offset(70, 100);
    self->actor.speedXZ = (Rand_ZeroOne() * 1.5f) + 1.5f;
    Math_ScaledStepToS(&self->actor.shape.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos), 0x300);
    self->targetPitch = ((self->maxAltitude < self->actor.world.pos.y) ? 0xC00 : -0xC00) + 0x1554;
    self->skelAnime.playSpeed = 1.0f;
    self->actionFunc = EnFirefly_FlyIdle;
}

void EnFirefly_SetupFall(EnFirefly* self) {
    self->timer = 40;
    self->actor.velocity.y = 0.0f;
    Animation_Change(&self->skelAnime, &gKeeseFlyAnim, 0.5f, 0.0f, 0.0f, ANIMMODE_LOOP_INTERP, -3.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FFLY_DEAD);
    self->actor.flags |= 0x10;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 40);
    self->actionFunc = EnFirefly_Fall;
}

void EnFirefly_SetupDie(EnFirefly* self) {
    self->timer = 15;
    self->actor.speedXZ = 0.0f;
    self->actionFunc = EnFirefly_Die;
}

void EnFirefly_SetupRebound(EnFirefly* self) {
    self->actor.world.rot.x = 0x7000;
    self->timer = 18;
    self->skelAnime.playSpeed = 1.0f;
    self->actor.speedXZ = 2.5f;
    self->actionFunc = EnFirefly_Rebound;
}

void EnFirefly_SetupDiveAttack(EnFirefly* self) {
    self->timer = Rand_S16Offset(70, 100);
    self->skelAnime.playSpeed = 1.0f;
    self->targetPitch = ((self->actor.yDistToPlayer > 0.0f) ? -0xC00 : 0xC00) + 0x1554;
    self->actionFunc = EnFirefly_DiveAttack;
}

void EnFirefly_SetupFlyAway(EnFirefly* self) {
    self->timer = 150;
    self->skelAnime.playSpeed = 1.0f;
    self->targetPitch = 0x954;
    self->actionFunc = EnFirefly_FlyAway;
}

void EnFirefly_SetupStunned(EnFirefly* self) {
    self->timer = 80;
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 80);
    self->auraType = KEESE_AURA_NONE;
    self->actor.velocity.y = 0.0f;
    self->skelAnime.playSpeed = 3.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->actionFunc = EnFirefly_Stunned;
}

void EnFirefly_SetupFrozenFall(EnFirefly* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3f iceParticlePos;

    self->actor.flags |= 0x10;
    self->auraType = KEESE_AURA_NONE;
    self->actor.speedXZ = 0.0f;
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0xFF);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FFLY_DEAD);

    for (i = 0; i <= 7; i++) {
        iceParticlePos.x = (i & 1 ? 7.0f : -7.0f) + self->actor.world.pos.x;
        iceParticlePos.y = (i & 2 ? 7.0f : -7.0f) + self->actor.world.pos.y;
        iceParticlePos.z = (i & 4 ? 7.0f : -7.0f) + self->actor.world.pos.z;
        EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &iceParticlePos, 150, 150, 150, 250, 235, 245, 255,
                                       (Rand_ZeroOne() * 0.15f) + 0.85f);
    }

    self->actionFunc = EnFirefly_FrozenFall;
}

void EnFirefly_SetupPerch(EnFirefly* self) {
    self->timer = 1;
    self->actor.speedXZ = 0.0f;
    self->actionFunc = EnFirefly_Perch;
}

void EnFirefly_SetupDisturbDiveAttack(EnFirefly* self) {
    self->skelAnime.playSpeed = 3.0f;
    self->actor.shape.rot.x = 0x1554;
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    self->actor.speedXZ = 3.0f;
    self->timer = 50;
    self->actionFunc = EnFirefly_DisturbDiveAttack;
}

s32 EnFirefly_ReturnToPerch(EnFirefly* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 distFromHome;

    if (self->actor.params != KEESE_NORMAL_PERCH) {
        return 0;
    }

    if (Actor_WorldDistXZToPoint(&player->actor, &self->actor.home.pos) > 300.0f) {
        distFromHome = Actor_WorldDistXYZToPoint(&self->actor, &self->actor.home.pos);

        if (distFromHome < 5.0f) {
            EnFirefly_SetupPerch(self);
            return 1;
        }

        distFromHome *= 0.05f;

        if (distFromHome < 1.0f) {
            self->actor.speedXZ *= distFromHome;
        }

        Math_ScaledStepToS(&self->actor.shape.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos),
                           0x300);
        Math_ScaledStepToS(&self->actor.shape.rot.x,
                           Actor_WorldPitchTowardPoint(&self->actor, &self->actor.home.pos) + 0x1554, 0x100);
        return 1;
    }

    return 0;
}

s32 EnFirefly_SeekTorch(EnFirefly* self, GlobalContext* globalCtx) {
    ObjSyokudai* findTorch;
    ObjSyokudai* closestTorch;
    f32 torchDist;
    f32 currentMinDist;
    Vec3f flamePos;

    findTorch = (ObjSyokudai*)globalCtx->actorCtx.actorLists[ACTORCAT_PROP].head;
    closestTorch = NULL;
    currentMinDist = 35000.0f;

    while (findTorch != NULL) {
        if ((findTorch->actor.id == ACTOR_OBJ_SYOKUDAI) && (findTorch->litTimer != 0)) {
            torchDist = Actor_WorldDistXYZToActor(&self->actor, &findTorch->actor);
            if (torchDist < currentMinDist) {
                currentMinDist = torchDist;
                closestTorch = findTorch;
            }
        }
        findTorch = (ObjSyokudai*)findTorch->actor.next;
    }

    if (closestTorch != NULL) {
        flamePos.x = closestTorch->actor.world.pos.x;
        flamePos.y = closestTorch->actor.world.pos.y + 52.0f + 15.0f;
        flamePos.z = closestTorch->actor.world.pos.z;
        if (Actor_WorldDistXYZToPoint(&self->actor, &flamePos) < 15.0f) {
            EnFirefly_Ignite(self);
            return 1;
        } else {
            Math_ScaledStepToS(&self->actor.shape.rot.y, Actor_WorldYawTowardActor(&self->actor, &closestTorch->actor),
                               0x300);
            Math_ScaledStepToS(&self->actor.shape.rot.x, Actor_WorldPitchTowardPoint(&self->actor, &flamePos) + 0x1554,
                               0x100);
            return 1;
        }
    }
    return 0;
}

void EnFirefly_FlyIdle(EnFirefly* self, GlobalContext* globalCtx) {
    s32 skelanimeUpdated;
    f32 rand;

    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }
    skelanimeUpdated = Animation_OnFrame(&self->skelAnime, 0.0f);
    self->actor.speedXZ = (Rand_ZeroOne() * 1.5f) + 1.5f;
    if (self->onFire || (self->actor.params == KEESE_ICE_FLY) ||
        ((EnFirefly_ReturnToPerch(self, globalCtx) == 0) && (EnFirefly_SeekTorch(self, globalCtx) == 0))) {
        if (skelanimeUpdated) {
            rand = Rand_ZeroOne();
            if (rand < 0.5f) {
                Math_ScaledStepToS(&self->actor.shape.rot.y,
                                   Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos), 0x300);
            } else if (rand < 0.8f) {
                self->actor.shape.rot.y += Rand_CenteredFloat(1536.0f);
            }
            // Climb if too close to ground
            if (self->actor.world.pos.y < (self->actor.floorHeight + 20.0f)) {
                self->targetPitch = 0x954;
                // Descend if above maxAltitude
            } else if (self->maxAltitude < self->actor.world.pos.y) {
                self->targetPitch = 0x2154;
                // Otherwise ascend or descend at random, biased towards ascending
            } else if (0.35f < Rand_ZeroOne()) {
                self->targetPitch = 0x954;
            } else {
                self->targetPitch = 0x2154;
            }
        } else {
            if (self->actor.bgCheckFlags & 1) {
                self->targetPitch = 0x954;
            } else if ((self->actor.bgCheckFlags & 0x10) || (self->maxAltitude < self->actor.world.pos.y)) {
                self->targetPitch = 0x2154;
            }
        }
        Math_ScaledStepToS(&self->actor.shape.rot.x, self->targetPitch, 0x100);
    }
    if (self->actor.bgCheckFlags & 8) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.wallYaw, 2, 0xC00, 0x300);
    }
    if ((self->timer == 0) && (self->actor.xzDistToPlayer < 200.0f) &&
        (Player_GetMask(globalCtx) != PLAYER_MASK_SKULL)) {
        EnFirefly_SetupDiveAttack(self);
    }
}

// Fall to the ground after being hit
void EnFirefly_Fall(EnFirefly* self, GlobalContext* globalCtx) {
    if (Animation_OnFrame(&self->skelAnime, 6.0f)) {
        self->skelAnime.playSpeed = 0.0f;
    }
    self->actor.colorFilterTimer = 40;
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    if (self->actor.flags & 0x8000) {
        self->actor.colorFilterTimer = 40;
    } else {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x6800, 0x200);
        self->actor.shape.rot.y = self->actor.shape.rot.y - 0x300;
        if (self->timer != 0) {
            self->timer--;
        }
        if ((self->actor.bgCheckFlags & 1) || (self->timer == 0)) {
            EnFirefly_SetupDie(self);
        }
    }
}

// Hit the ground or burn up, spawn drops
void EnFirefly_Die(EnFirefly* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    Math_StepToF(&self->actor.scale.x, 0.0f, 0.00034f);
    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;
    if (self->timer == 0) {
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xE0);
        Actor_Kill(&self->actor);
    }
}

void EnFirefly_DiveAttack(EnFirefly* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f preyPos;

    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }
    Math_StepToF(&self->actor.speedXZ, 4.0f, 0.5f);
    if (self->actor.bgCheckFlags & 8) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.wallYaw, 2, 0xC00, 0x300);
        Math_ScaledStepToS(&self->actor.shape.rot.x, self->targetPitch, 0x100);
    } else if (Actor_IsFacingPlayer(&self->actor, 0x2800)) {
        if (Animation_OnFrame(&self->skelAnime, 4.0f)) {
            self->skelAnime.playSpeed = 0.0f;
            self->skelAnime.curFrame = 4.0f;
        }
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xC00, 0x300);
        preyPos.x = player->actor.world.pos.x;
        preyPos.y = player->actor.world.pos.y + 20.0f;
        preyPos.z = player->actor.world.pos.z;
        Math_SmoothStepToS(&self->actor.shape.rot.x, Actor_WorldPitchTowardPoint(&self->actor, &preyPos) + 0x1554, 2,
                           0x400, 0x100);
    } else {
        self->skelAnime.playSpeed = 1.5f;
        if (self->actor.xzDistToPlayer > 80.0f) {
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xC00, 0x300);
        }
        if (self->actor.bgCheckFlags & 1) {
            self->targetPitch = 0x954;
        }
        if ((self->actor.bgCheckFlags & 0x10) || (self->maxAltitude < self->actor.world.pos.y)) {
            self->targetPitch = 0x2154;
        } else {
            self->targetPitch = 0x954;
        }
        Math_ScaledStepToS(&self->actor.shape.rot.x, self->targetPitch, 0x100);
    }
    if ((self->timer == 0) || (Player_GetMask(globalCtx) == PLAYER_MASK_SKULL)) {
        EnFirefly_SetupFlyAway(self);
    }
}

// Knockback after hitting player
void EnFirefly_Rebound(EnFirefly* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x100);
    Math_StepToF(&self->actor.velocity.y, 0.0f, 0.4f);
    if (Math_StepToF(&self->actor.speedXZ, 0.0f, 0.15f)) {
        if (self->timer != 0) {
            self->timer--;
        }
        if (self->timer == 0) {
            EnFirefly_SetupFlyAway(self);
        }
    }
}

void EnFirefly_FlyAway(EnFirefly* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }
    if (((fabsf(self->actor.world.pos.y - self->maxAltitude) < 10.0f) &&
         (Math_Vec3f_DistXZ(&self->actor.world.pos, &self->actor.home.pos) < 20.0f)) ||
        (self->timer == 0)) {
        EnFirefly_SetupFlyIdle(self);
        return;
    }
    Math_StepToF(&self->actor.speedXZ, 3.0f, 0.3f);
    if (self->actor.bgCheckFlags & 1) {
        self->targetPitch = 0x954;
    } else if ((self->actor.bgCheckFlags & 0x10) || (self->maxAltitude < self->actor.world.pos.y)) {
        self->targetPitch = 0x2154;
    } else {
        self->targetPitch = 0x954;
    }
    if (self->actor.bgCheckFlags & 8) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.wallYaw, 2, 0xC00, 0x300);
    } else {
        Math_ScaledStepToS(&self->actor.shape.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos),
                           0x300);
    }
    Math_ScaledStepToS(&self->actor.shape.rot.x, self->targetPitch, 0x100);
}

void EnFirefly_Stunned(EnFirefly* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0x1554, 0x100);
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        if (self->onFire) {
            self->auraType = KEESE_AURA_FIRE;
        } else if (self->actor.params == KEESE_ICE_FLY) {
            self->auraType = KEESE_AURA_ICE;
        }
        EnFirefly_SetupFlyIdle(self);
    }
}

void EnFirefly_FrozenFall(EnFirefly* self, GlobalContext* globalCtx) {
    if ((self->actor.bgCheckFlags & 1) || (self->actor.floorHeight == BGCHECK_Y_MIN)) {
        self->actor.colorFilterTimer = 0;
        EnFirefly_SetupDie(self);
    } else {
        self->actor.colorFilterTimer = 255;
    }
}

// When perching, sit on collision and flap at random intervals
void EnFirefly_Perch(EnFirefly* self, GlobalContext* globalCtx) {
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x100);

    if (self->timer != 0) {
        SkelAnime_Update(&self->skelAnime);
        if (Animation_OnFrame(&self->skelAnime, 6.0f)) {
            self->timer--;
        }
    } else if (Rand_ZeroOne() < 0.02f) {
        self->timer = 1;
    }

    if (self->actor.xzDistToPlayer < 120.0f) {
        EnFirefly_SetupDisturbDiveAttack(self);
    }
}

void EnFirefly_DisturbDiveAttack(EnFirefly* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f preyPos;

    SkelAnime_Update(&self->skelAnime);

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer < 40) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0xAAC, 0x100);
    } else {
        preyPos.x = player->actor.world.pos.x;
        preyPos.y = player->actor.world.pos.y + 20.0f;
        preyPos.z = player->actor.world.pos.z;
        Math_ScaledStepToS(&self->actor.shape.rot.x, Actor_WorldPitchTowardPoint(&self->actor, &preyPos) + 0x1554,
                           0x100);
        Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0x300);
    }

    if (self->timer == 0) {
        EnFirefly_SetupFlyIdle(self);
    }
}

void EnFirefly_Combust(EnFirefly* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i <= 2; i++) {
        EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &self->actor.world.pos, 40, 0, 0, i);
    }

    self->auraType = KEESE_AURA_NONE;
}

void EnFirefly_UpdateDamage(EnFirefly* self, GlobalContext* globalCtx) {
    u8 damageEffect;

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.elements[0].info, 1);

        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                self->actor.flags &= ~1;
            }

            damageEffect = self->actor.colChkInfo.damageEffect;

            if (damageEffect == 2) { // Din's Fire
                if (self->actor.params == KEESE_ICE_FLY) {
                    self->actor.colChkInfo.health = 0;
                    Enemy_StartFinishingBlow(globalCtx, &self->actor);
                    EnFirefly_Combust(self, globalCtx);
                    EnFirefly_SetupFall(self);
                } else if (!self->onFire) {
                    EnFirefly_Ignite(self);
                    if (self->actionFunc == EnFirefly_Perch) {
                        EnFirefly_SetupFlyIdle(self);
                    }
                }
            } else if (damageEffect == 3) { // Ice Arrows or Ice Magic
                if (self->actor.params == KEESE_ICE_FLY) {
                    EnFirefly_SetupFall(self);
                } else {
                    EnFirefly_SetupFrozenFall(self, globalCtx);
                }
            } else if (damageEffect == 1) { // Deku Nuts
                if (self->actionFunc != EnFirefly_Stunned) {
                    EnFirefly_SetupStunned(self);
                }
            } else { // Fire Arrows
                if ((damageEffect == 0xF) && (self->actor.params == KEESE_ICE_FLY)) {
                    EnFirefly_Combust(self, globalCtx);
                }
                EnFirefly_SetupFall(self);
            }
        }
    }
}

void EnFirefly_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnFirefly* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FFLY_ATTACK);
        if (self->onFire) {
            EnFirefly_Extinguish(self);
        }
        if (self->actionFunc != EnFirefly_DisturbDiveAttack) {
            EnFirefly_SetupRebound(self);
        }
    }

    EnFirefly_UpdateDamage(self, globalCtx);

    self->actionFunc(self, globalCtx);

    if (!(self->actor.flags & 0x8000)) {
        if ((self->actor.colChkInfo.health == 0) || (self->actionFunc == EnFirefly_Stunned)) {
            Actor_MoveForward(&self->actor);
        } else {
            if (self->actionFunc != EnFirefly_Rebound) {
                self->actor.world.rot.x = 0x1554 - self->actor.shape.rot.x;
            }
            func_8002D97C(&self->actor);
        }
    }

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 15.0f, 7);
    self->collider.elements[0].dim.worldSphere.center.x = self->actor.world.pos.x;
    self->collider.elements[0].dim.worldSphere.center.y = self->actor.world.pos.y + 10.0f;
    self->collider.elements[0].dim.worldSphere.center.z = self->actor.world.pos.z;

    if ((self->actionFunc == EnFirefly_DiveAttack) || (self->actionFunc == EnFirefly_DisturbDiveAttack)) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if (self->actor.colChkInfo.health != 0) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (Animation_OnFrame(&self->skelAnime, 5.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FFLY_FLY);
        }
    }

    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    self->actor.focus.pos.x =
        (10.0f * Math_SinS(self->actor.shape.rot.x) * Math_SinS(self->actor.shape.rot.y)) + self->actor.world.pos.x;
    self->actor.focus.pos.y = (10.0f * Math_CosS(self->actor.shape.rot.x)) + self->actor.world.pos.y;
    self->actor.focus.pos.z =
        (10.0f * Math_SinS(self->actor.shape.rot.x) * Math_CosS(self->actor.shape.rot.y)) + self->actor.world.pos.z;
}

s32 EnFirefly_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                               void* thisx, Gfx** gfx) {
    EnFirefly* self = THIS;

    if ((self->actor.draw == EnFirefly_DrawInvisible) && (globalCtx->actorCtx.unk_03 == 0)) {
        *dList = NULL;
    } else if (limbIndex == 1) {
        pos->y += 2300.0f;
    }
    return false;
}

void EnFirefly_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    static Color_RGBA8 fireAuraPrimColor = { 255, 255, 100, 255 };
    static Color_RGBA8 fireAuraEnvColor = { 255, 50, 0, 0 };
    static Color_RGBA8 iceAuraPrimColor = { 100, 200, 255, 255 };
    static Color_RGBA8 iceAuraEnvColor = { 0, 0, 255, 0 };
    static Vec3f effVelocity = { 0.0f, 0.5f, 0.0f };
    static Vec3f effAccel = { 0.0f, 0.5f, 0.0f };
    static Vec3f limbSrc = { 0.0f, 0.0f, 0.0f };
    Vec3f effPos;
    Vec3f* limbDest;
    Color_RGBA8* effPrimColor;
    Color_RGBA8* effEnvColor;
    MtxF mtx;
    s16 effScaleStep;
    s16 effLife;
    EnFirefly* self = THIS;

    if (!self->onFire && (limbIndex == 27)) {
        gSPDisplayList((*gfx)++, gKeeseEyesDL);
    } else {
        if ((self->auraType == KEESE_AURA_FIRE) || (self->auraType == KEESE_AURA_ICE)) {
            if ((limbIndex == 15) || (limbIndex == 21)) {
                if (self->actionFunc != EnFirefly_Die) {
                    Matrix_Get(&mtx);
                    effPos.x = (Rand_ZeroOne() * 5.0f) + mtx.xw;
                    effPos.y = (Rand_ZeroOne() * 5.0f) + mtx.yw;
                    effPos.z = (Rand_ZeroOne() * 5.0f) + mtx.zw;
                    effScaleStep = -40;
                    effLife = 3;
                } else {
                    if (limbIndex == 15) {
                        effPos.x = (Math_SinS(9100 * self->timer) * self->timer) + self->actor.world.pos.x;
                        effPos.z = (Math_CosS(9100 * self->timer) * self->timer) + self->actor.world.pos.z;
                    } else {
                        effPos.x = self->actor.world.pos.x - (Math_SinS(9100 * self->timer) * self->timer);
                        effPos.z = self->actor.world.pos.z - (Math_CosS(9100 * self->timer) * self->timer);
                    }

                    effPos.y = self->actor.world.pos.y + ((15 - self->timer) * 1.5f);
                    effScaleStep = -5;
                    effLife = 10;
                }

                if (self->auraType == KEESE_AURA_FIRE) {
                    effPrimColor = &fireAuraPrimColor;
                    effEnvColor = &fireAuraEnvColor;
                } else {
                    effPrimColor = &iceAuraPrimColor;
                    effEnvColor = &iceAuraEnvColor;
                }

                func_8002843C(globalCtx, &effPos, &effVelocity, &effAccel, effPrimColor, effEnvColor, 250, effScaleStep,
                              effLife);
            }
        }
    }
    if ((limbIndex == 15) || (limbIndex == 21) || (limbIndex == 10)) {
        if (limbIndex == 15) {
            limbDest = &self->bodyPartsPos[0];
        } else if (limbIndex == 21) {
            limbDest = &self->bodyPartsPos[1];
        } else {
            limbDest = &self->bodyPartsPos[2];
        }

        Matrix_MultVec3f(&limbSrc, limbDest);
        limbDest->y -= 5.0f;
    }
}

void EnFirefly_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnFirefly* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_firefly.c", 1733);
    func_80093D18(globalCtx->state.gfxCtx);

    if (self->onFire) {
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0);
    } else {
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    }

    POLY_OPA_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                   EnFirefly_OverrideLimbDraw, EnFirefly_PostLimbDraw, &self->actor, POLY_OPA_DISP);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_firefly.c", 1763);
}

void EnFirefly_DrawInvisible(Actor* thisx, GlobalContext* globalCtx) {
    EnFirefly* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_firefly.c", 1775);
    func_80093D84(globalCtx->state.gfxCtx);

    if (self->onFire) {
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 0);
    } else {
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 255);
    }

    POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                   EnFirefly_OverrideLimbDraw, EnFirefly_PostLimbDraw, self, POLY_XLU_DISP);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_firefly.c", 1805);
}
