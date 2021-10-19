/*
 * File: z_en_am.c
 * Overlay: ovl_En_Am
 * Description: Armos
 */

#include "z_en_am.h"
#include "objects/object_am/object_am.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"

#define FLAGS 0x04000015

#define THIS ((EnAm*)thisx)

void EnAm_Init(Actor* thisx, GlobalContext* globalCtx);
void EnAm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnAm_Update(Actor* thisx, GlobalContext* globalCtx);
void EnAm_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnAm_SetupStatue(EnAm* self);
void EnAm_SetupSleep(EnAm* self);
void EnAm_Statue(EnAm* self, GlobalContext* globalCtx);
void EnAm_Sleep(EnAm* self, GlobalContext* globalCtx);
void EnAm_Lunge(EnAm* self, GlobalContext* globalCtx);
void EnAm_RotateToHome(EnAm* self, GlobalContext* globalCtx);
void EnAm_MoveToHome(EnAm* self, GlobalContext* globalCtx);
void EnAm_RotateToInit(EnAm* self, GlobalContext* globalCtx);
void EnAm_Cooldown(EnAm* self, GlobalContext* globalCtx);
void EnAm_Ricochet(EnAm* self, GlobalContext* globalCtx);
void EnAm_Stunned(EnAm* self, GlobalContext* globalCtx);
void EnAm_RecoilFromDamage(EnAm* self, GlobalContext* globalCtx);

typedef enum {
    /* 00 */ AM_BEHAVIOR_NONE,
    /* 01 */ AM_BEHAVIOR_DAMAGED,
    /* 03 */ AM_BEHAVIOR_DO_NOTHING = 3,
    /* 05 */ AM_BEHAVIOR_5 = 5, // checked but never set
    /* 06 */ AM_BEHAVIOR_STUNNED,
    /* 07 */ AM_BEHAVIOR_GO_HOME,
    /* 08 */ AM_BEHAVIOR_RICOCHET,
    /* 10 */ AM_BEHAVIOR_AGGRO = 10
} ArmosBehavior;

const ActorInit En_Am_InitVars = {
    ACTOR_EN_AM,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_AM,
    sizeof(EnAm),
    (ActorFunc)EnAm_Init,
    (ActorFunc)EnAm_Destroy,
    (ActorFunc)EnAm_Update,
    (ActorFunc)EnAm_Draw,
};

static ColliderCylinderInit sHurtCylinderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 15, 70, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sBlockCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00400106, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 15, 70, 0, { 0, 0, 0 } },
};

static ColliderQuadInit sQuadInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum {
    /* 00 */ AM_DMGEFF_NONE, // used by anything that cant kill the armos
    /* 01 */ AM_DMGEFF_NUT,
    /* 06 */ AM_DMGEFF_STUN = 6, // doesnt include deku nuts
    /* 13 */ AM_DMGEFF_ICE = 13,
    /* 14 */ AM_DMGEFF_MAGIC_FIRE_LIGHT,
    /* 15 */ AM_DMGEFF_KILL // any damage source that can kill the armos (and isnt a special case)
} ArmosDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, AM_DMGEFF_NUT),
    /* Deku stick    */ DMG_ENTRY(2, AM_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(1, AM_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(2, AM_DMGEFF_KILL),
    /* Boomerang     */ DMG_ENTRY(0, AM_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, AM_DMGEFF_KILL),
    /* Hammer swing  */ DMG_ENTRY(2, AM_DMGEFF_KILL),
    /* Hookshot      */ DMG_ENTRY(0, AM_DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, AM_DMGEFF_NONE),
    /* Master sword  */ DMG_ENTRY(2, AM_DMGEFF_KILL),
    /* Giant's Knife */ DMG_ENTRY(4, AM_DMGEFF_KILL),
    /* Fire arrow    */ DMG_ENTRY(2, AM_DMGEFF_KILL),
    /* Ice arrow     */ DMG_ENTRY(4, AM_DMGEFF_ICE),
    /* Light arrow   */ DMG_ENTRY(2, AM_DMGEFF_KILL),
    /* Unk arrow 1   */ DMG_ENTRY(2, AM_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, AM_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, AM_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(0, AM_DMGEFF_MAGIC_FIRE_LIGHT),
    /* Ice magic     */ DMG_ENTRY(3, AM_DMGEFF_ICE),
    /* Light magic   */ DMG_ENTRY(0, AM_DMGEFF_MAGIC_FIRE_LIGHT),
    /* Shield        */ DMG_ENTRY(0, AM_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, AM_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, AM_DMGEFF_NONE),
    /* Giant spin    */ DMG_ENTRY(4, AM_DMGEFF_KILL),
    /* Master spin   */ DMG_ENTRY(2, AM_DMGEFF_KILL),
    /* Kokiri jump   */ DMG_ENTRY(2, AM_DMGEFF_NONE),
    /* Giant jump    */ DMG_ENTRY(8, AM_DMGEFF_KILL),
    /* Master jump   */ DMG_ENTRY(4, AM_DMGEFF_KILL),
    /* Unknown 1     */ DMG_ENTRY(0, AM_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, AM_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, AM_DMGEFF_KILL),
    /* Unknown 2     */ DMG_ENTRY(0, AM_DMGEFF_NONE),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x13, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -4000, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 5300, ICHAIN_STOP),
};

void EnAm_SetupAction(EnAm* self, EnAmActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

/**
 * Checks bgCheckFlags in the direction of current yaw at the specified distance.
 *
 * Returns true if the armos would land on the ground in the resulting position.
 *
 * If it won't land on the ground, it will return true anyway if the floor the armos will be on
 * is no more than 20 units lower than the home position. This prevents the armos from going down steep slopes.
 */
s32 EnAm_CanMove(EnAm* self, GlobalContext* globalCtx, f32 distance, s16 yaw) {
    s16 ret;
    s16 curBgCheckFlags;
    f32 sin;
    f32 cos;
    Vec3f curPos;

    // save current position and bgCheckFlags to restore later
    curPos = self->dyna.actor.world.pos;
    curBgCheckFlags = self->dyna.actor.bgCheckFlags;

    sin = Math_SinS(yaw) * distance;
    cos = Math_CosS(yaw) * distance;

    self->dyna.actor.world.pos.x += sin;
    self->dyna.actor.world.pos.z += cos;

    Actor_UpdateBgCheckInfo(globalCtx, &self->dyna.actor, 0.0f, 0.0f, 0.0f, 4);
    self->dyna.actor.world.pos = curPos;
    ret = self->dyna.actor.bgCheckFlags & 1;

    if (!ret && (self->dyna.actor.floorHeight >= (self->dyna.actor.home.pos.y - 20.0f))) {
        ret = true;
    }

    self->dyna.actor.bgCheckFlags = curBgCheckFlags;

    return ret;
}

void EnAm_Init(Actor* thisx, GlobalContext* globalCtx) {
    CollisionHeader* colHeader = NULL;
    s32 pad;
    EnAm* self = THIS;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    ActorShape_Init(&self->dyna.actor.shape, 0.0f, ActorShadow_DrawCircle, 48.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gArmosSkel, &gArmosRicochetAnim, self->jointTable, self->morphTable,
                   14);
    Actor_SetScale(&self->dyna.actor, 0.01f);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    Collider_InitCylinder(globalCtx, &self->hurtCollider);
    Collider_InitCylinder(globalCtx, &self->blockCollider);
    Collider_SetCylinder(globalCtx, &self->hurtCollider, &self->dyna.actor, &sHurtCylinderInit);

    if (self->dyna.actor.params == ARMOS_STATUE) {
        self->dyna.actor.colChkInfo.health = 1;
        Collider_SetCylinder(globalCtx, &self->blockCollider, &self->dyna.actor, &sHurtCylinderInit);
        self->hurtCollider.base.ocFlags1 = (OC1_ON | OC1_NO_PUSH | OC1_TYPE_1 | OC1_TYPE_2);
        self->blockCollider.base.ocFlags1 = (OC1_ON | OC1_NO_PUSH | OC1_TYPE_PLAYER);
        CollisionHeader_GetVirtual(&gArmosCol, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->dyna.actor, ACTORCAT_BG);
        EnAm_SetupStatue(self);
    } else {
        Collider_SetCylinder(globalCtx, &self->blockCollider, &self->dyna.actor, &sBlockCylinderInit);
        Collider_InitQuad(globalCtx, &self->hitCollider);
        Collider_SetQuad(globalCtx, &self->hitCollider, &self->dyna.actor, &sQuadInit);
        self->dyna.actor.colChkInfo.health = 1;
        self->dyna.actor.colChkInfo.damageTable = &sDamageTable;
        EnAm_SetupSleep(self);
        self->unk_258 = 0;
    }

    self->dyna.actor.colChkInfo.mass = MASS_HEAVY;
}

void EnAm_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnAm* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(globalCtx, &self->hurtCollider);
    Collider_DestroyCylinder(globalCtx, &self->blockCollider);
    //! @bug Quad collider is not destroyed (though destroy doesnt really do anything anyway)
}

void EnAm_SpawnEffects(EnAm* self, GlobalContext* globalCtx) {
    static Vec3f velocity = { 0.0f, -1.5f, 0.0f };
    static Vec3f accel = { 0.0f, -0.2f, 0.0f };
    s32 i;
    Vec3f pos;
    Color_RGBA8 primColor = { 100, 100, 100, 0 };
    Color_RGBA8 envColor = { 40, 40, 40, 0 };
    s32 pad;

    for (i = 4; i > 0; i--) {
        pos.x = self->dyna.actor.world.pos.x + ((Rand_ZeroOne() - 0.5f) * 65.0f);
        pos.y = (self->dyna.actor.world.pos.y + 40.0f) + ((Rand_ZeroOne() - 0.5f) * 10.0f);
        pos.z = self->dyna.actor.world.pos.z + ((Rand_ZeroOne() - 0.5f) * 65.0f);

        EffectSsKiraKira_SpawnSmall(globalCtx, &pos, &velocity, &accel, &primColor, &envColor);
    }

    Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_AMOS_WALK);
    Actor_SpawnFloorDustRing(globalCtx, &self->dyna.actor, &self->dyna.actor.world.pos, 4.0f, 3, 8.0f, 0x12C, 0xF, 0);
}

void EnAm_SetupSleep(EnAm* self) {
    f32 lastFrame = Animation_GetLastFrame(&gArmosRicochetAnim);

    Animation_Change(&self->skelAnime, &gArmosRicochetAnim, 0.0f, lastFrame, lastFrame, ANIMMODE_LOOP, 0.0f);
    self->behavior = AM_BEHAVIOR_DO_NOTHING;
    self->dyna.actor.speedXZ = 0.0f;
    self->unk_258 = (self->textureBlend == 255) ? 0 : 1;
    EnAm_SetupAction(self, EnAm_Sleep);
}

void EnAm_SetupStatue(EnAm* self) {
    f32 lastFrame = Animation_GetLastFrame(&gArmosRicochetAnim);

    Animation_Change(&self->skelAnime, &gArmosRicochetAnim, 0.0f, lastFrame, lastFrame, ANIMMODE_LOOP, 0.0f);
    self->dyna.actor.flags &= ~1;
    self->behavior = AM_BEHAVIOR_DO_NOTHING;
    self->dyna.actor.speedXZ = 0.0f;
    EnAm_SetupAction(self, EnAm_Statue);
}

void EnAm_SetupLunge(EnAm* self) {
    Animation_PlayLoopSetSpeed(&self->skelAnime, &gArmosHopAnim, 4.0f);
    self->unk_258 = 3;
    self->behavior = AM_BEHAVIOR_AGGRO;
    self->dyna.actor.speedXZ = 0.0f;
    self->dyna.actor.world.rot.y = self->dyna.actor.shape.rot.y;
    EnAm_SetupAction(self, EnAm_Lunge);
}

void EnAm_SetupCooldown(EnAm* self) {
    Animation_PlayLoopSetSpeed(&self->skelAnime, &gArmosHopAnim, 4.0f);
    self->unk_258 = 3;
    self->cooldownTimer = 40;
    self->behavior = AM_BEHAVIOR_AGGRO;
    self->dyna.actor.speedXZ = 0.0f;
    self->dyna.actor.world.rot.y = self->dyna.actor.shape.rot.y;
    EnAm_SetupAction(self, EnAm_Cooldown);
}

void EnAm_SetupMoveToHome(EnAm* self) {
    Animation_PlayLoopSetSpeed(&self->skelAnime, &gArmosHopAnim, 4.0f);
    self->behavior = AM_BEHAVIOR_GO_HOME;
    self->unk_258 = 1;
    self->dyna.actor.speedXZ = 0.0f;
    EnAm_SetupAction(self, EnAm_MoveToHome);
}

void EnAm_SetupRotateToInit(EnAm* self) {
    Animation_PlayLoopSetSpeed(&self->skelAnime, &gArmosHopAnim, 4.0f);
    self->behavior = AM_BEHAVIOR_GO_HOME;
    self->unk_258 = 1;
    self->dyna.actor.speedXZ = 0.0f;
    EnAm_SetupAction(self, EnAm_RotateToInit);
}

void EnAm_SetupRotateToHome(EnAm* self) {
    Animation_PlayLoopSetSpeed(&self->skelAnime, &gArmosHopAnim, 4.0f);
    self->behavior = AM_BEHAVIOR_GO_HOME;
    self->dyna.actor.speedXZ = 0.0f;
    self->dyna.actor.world.rot.y = self->dyna.actor.shape.rot.y;
    EnAm_SetupAction(self, EnAm_RotateToHome);
}

void EnAm_SetupRecoilFromDamage(EnAm* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &gArmosDamagedAnim, 1.0f, 4.0f,
                     Animation_GetLastFrame(&gArmosDamagedAnim) - 6.0f, ANIMMODE_ONCE, 0.0f);
    self->behavior = AM_BEHAVIOR_DAMAGED;
    self->dyna.actor.world.rot.y = self->dyna.actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_AMOS_DAMAGE);

    if (EnAm_CanMove(self, globalCtx, -6.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = -6.0f;
    }

    self->dyna.actor.colorFilterTimer = 0;
    Enemy_StartFinishingBlow(globalCtx, &self->dyna.actor);
    EnAm_SetupAction(self, EnAm_RecoilFromDamage);
}

void EnAm_SetupRicochet(EnAm* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &gArmosRicochetAnim, 1.0f, 0.0f, 8.0f, ANIMMODE_ONCE, 0.0f);
    self->dyna.actor.world.rot.y = self->dyna.actor.yawTowardsPlayer;

    if (EnAm_CanMove(self, globalCtx, -6.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = -6.0f;
    }

    self->unk_264 = 0;
    self->unk_258 = 0;
    self->cooldownTimer = 5;
    self->behavior = AM_BEHAVIOR_RICOCHET;
    EnAm_SetupAction(self, EnAm_Ricochet);
}

void EnAm_Sleep(EnAm* self, GlobalContext* globalCtx) {
    f32 cos;
    s32 pad;
    f32 rand;
    f32 sin;
    Player* player = GET_PLAYER(globalCtx);

    if ((self->unk_258 != 0) ||
        ((self->hurtCollider.base.ocFlags1 & OC1_HIT) && (self->hurtCollider.base.oc == &player->actor)) ||
        (self->hurtCollider.base.acFlags & AC_HIT)) {
        self->hurtCollider.base.acFlags &= ~AC_HIT;

        if (self->textureBlend == 0) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_AMOS_WAVE);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_AMOS_VOICE);
            Actor_SetColorFilter(&self->dyna.actor, 0x4000, 255, 0, 8);
        }

        if (self->textureBlend >= 240) {
            self->attackTimer = 200;
            self->textureBlend = 255;
            self->dyna.actor.flags |= 1;
            self->dyna.actor.shape.yOffset = 0.0f;
            EnAm_SetupLunge(self);
        } else {
            // shake randomly about the origin while waking up
            rand = (Rand_ZeroOne() - 0.5f) * 10.0f;

            cos = Math_CosS(self->dyna.actor.shape.rot.y) * rand;
            sin = Math_SinS(self->dyna.actor.shape.rot.y) * rand;

            self->dyna.actor.world.pos.x = self->shakeOrigin.x + cos;
            self->dyna.actor.world.pos.z = self->shakeOrigin.z + sin;

            self->textureBlend += 20;
            self->unk_258 = 1;
        }
    } else {
        if (self->textureBlend > 10) {
            self->textureBlend -= 10;
        } else {
            self->textureBlend = 0;
            self->dyna.actor.flags &= ~1;

            if (self->dyna.bgId < 0) {
                self->unk_264 = 0;
            }

            self->dyna.actor.speedXZ += self->dyna.unk_150;
            self->shakeOrigin = self->dyna.actor.world.pos;
            self->dyna.actor.world.rot.y = self->dyna.unk_158;
            self->dyna.actor.speedXZ = CLAMP(self->dyna.actor.speedXZ, -2.5f, 2.5f);
            Math_SmoothStepToF(&self->dyna.actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);

            if (self->dyna.actor.speedXZ != 0.0f) {
                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ROCK_SLIDE - SFX_FLAG);
            }

            self->dyna.unk_154 = 0.0f;
            self->dyna.unk_150 = 0.0f;
        }
    }
}

/**
 * Spin toward the direction of the home position to start moving back to it.
 */
void EnAm_RotateToHome(EnAm* self, GlobalContext* globalCtx) {
    s16 yawToHome = Math_Vec3f_Yaw(&self->dyna.actor.world.pos, &self->dyna.actor.home.pos);

    if (self->skelAnime.curFrame == 8.0f) {
        Math_SmoothStepToS(&self->dyna.actor.world.rot.y, yawToHome, 1, 0x1F40, 0);
        self->dyna.actor.velocity.y = 12.0f;
    } else if (self->skelAnime.curFrame > 11.0f) {
        if (!(self->dyna.actor.bgCheckFlags & 1)) {
            self->skelAnime.curFrame = 11;
        } else {
            EnAm_SpawnEffects(self, globalCtx);

            if (self->dyna.actor.world.rot.y == yawToHome) {
                self->unk_258 = 0;
            }

            self->dyna.actor.velocity.y = 0.0f;
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        }
    }

    SkelAnime_Update(&self->skelAnime);

    if (self->unk_258 == 0) {
        EnAm_SetupMoveToHome(self);
    }

    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
}

/**
 * After reaching the home position, spin back to the starting rotation.
 */
void EnAm_RotateToInit(EnAm* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame == 8.0f) {
        if ((self->dyna.actor.world.pos.x == self->dyna.actor.home.pos.x) &&
            (self->dyna.actor.world.pos.z == self->dyna.actor.home.pos.z)) {
            Math_SmoothStepToS(&self->dyna.actor.world.rot.y, self->dyna.actor.home.rot.y, 1, 0x1F40, 0);
        }
        self->unk_258 = 2;
        self->dyna.actor.velocity.y = 12.0f;
    } else if (self->skelAnime.curFrame > 11.0f) {
        if (!(self->dyna.actor.bgCheckFlags & 1)) {
            self->skelAnime.curFrame = 11;
        } else {
            self->unk_258 = 1;
            EnAm_SpawnEffects(self, globalCtx);

            if (self->dyna.actor.home.rot.y == self->dyna.actor.world.rot.y) {
                self->unk_258 = 0;
            }

            self->dyna.actor.velocity.y = 0.0f;
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        }
    }

    if (self->unk_258 == 2) {
        Math_SmoothStepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x, 1.0f, 8.0f, 0.0f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.z, self->dyna.actor.home.pos.z, 1.0f, 8.0f, 0.0f);
    }

    SkelAnime_Update(&self->skelAnime);

    if (self->unk_258 == 0) {
        EnAm_SetupSleep(self);
    }

    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
}

void EnAm_MoveToHome(EnAm* self, GlobalContext* globalCtx) {
    s16 yawToHome = Math_Vec3f_Yaw(&self->dyna.actor.world.pos, &self->dyna.actor.home.pos);

    if (self->skelAnime.curFrame == 8.0f) {
        self->dyna.actor.velocity.y = 12.0f;
        self->dyna.actor.speedXZ = 6.0f;
    } else if (self->skelAnime.curFrame > 11.0f) {
        if (!(self->dyna.actor.bgCheckFlags & 1)) {
            self->skelAnime.curFrame = 11;
        } else {
            Math_SmoothStepToS(&self->dyna.actor.world.rot.y, yawToHome, 1, 0xBB8, 0);

            if (self->dyna.actor.bgCheckFlags & 2) {
                self->unk_258--;
            }

            self->dyna.actor.velocity.y = 0.0f;
            self->dyna.actor.speedXZ = 0.0f;
            self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
            EnAm_SpawnEffects(self, globalCtx);

            if (Actor_WorldDistXYZToPoint(&self->dyna.actor, &self->dyna.actor.home.pos) < 80.0f) {
                EnAm_SetupRotateToInit(self);
            }
        }
    }

    // turn away from a wall if touching one
    if ((self->dyna.actor.speedXZ != 0.0f) && (self->dyna.actor.bgCheckFlags & 8)) {
        self->dyna.actor.world.rot.y = self->dyna.actor.wallYaw;
        Actor_MoveForward(&self->dyna.actor);
    }

    SkelAnime_Update(&self->skelAnime);

    self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
}

void EnAm_RecoilFromDamage(EnAm* self, GlobalContext* globalCtx) {
    if (self->dyna.actor.speedXZ < 0.0f) {
        self->dyna.actor.speedXZ += 0.5f;
    }

    if ((self->dyna.actor.velocity.y <= 0.0f) && !EnAm_CanMove(self, globalCtx, -8.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = 0.0f;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        EnAm_SetupLunge(self);
        self->deathTimer = 64;
        self->panicSpinRot = 0;
    }
}

/**
 * After doing 3 lunges, wait for 2 seconds before attacking again.
 * Turn toward the player before lunging again.
 */
void EnAm_Cooldown(EnAm* self, GlobalContext* globalCtx) {
    s16 yawDiff = self->dyna.actor.yawTowardsPlayer - self->dyna.actor.world.rot.y;

    yawDiff = ABS(yawDiff);

    if (self->cooldownTimer != 0) {
        self->cooldownTimer--;
    } else {
        if (self->skelAnime.curFrame == 8.0f) {
            Math_SmoothStepToS(&self->dyna.actor.world.rot.y, self->dyna.actor.yawTowardsPlayer, 1, 0x1F40, 0);
            self->dyna.actor.velocity.y = 12.0f;
        } else if (self->skelAnime.curFrame > 11.0f) {
            if (!(self->dyna.actor.bgCheckFlags & 1)) {
                self->skelAnime.curFrame = 11;
            } else {
                if (yawDiff < 3500) {
                    self->unk_258 = 0;
                }
                self->dyna.actor.velocity.y = 0.0f;
                self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
                EnAm_SpawnEffects(self, globalCtx);
            }
        }

        SkelAnime_Update(&self->skelAnime);

        if (self->unk_258 == 0) {
            EnAm_SetupLunge(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_AMOS_VOICE);
        }

        self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
    }
}

/**
 * Lunge toward the player in an attempt to deal damage. Hop 3 times.
 * Used for both normal attacks and the death sequence.
 */
void EnAm_Lunge(EnAm* self, GlobalContext* globalCtx) {
    if (self->deathTimer < 52) {
        if (self->skelAnime.curFrame == 8.0f) {
            self->dyna.actor.velocity.y = 12.0f;

            if (EnAm_CanMove(self, globalCtx, 80.0f, self->dyna.actor.world.rot.y)) {
                self->dyna.actor.speedXZ = 6.0f;
            } else {
                self->dyna.actor.speedXZ = 0.0f;
            }

            self->unk_264 = 1;
            self->hitCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        } else if (self->skelAnime.curFrame > 11.0f) {
            if (!(self->dyna.actor.bgCheckFlags & 1)) {
                self->skelAnime.curFrame = 11;
            } else {
                Math_SmoothStepToS(&self->dyna.actor.world.rot.y, self->dyna.actor.yawTowardsPlayer, 1, 0x1770, 0);

                if (self->dyna.actor.bgCheckFlags & 2) {
                    self->unk_258--;
                }

                self->dyna.actor.velocity.y = 0.0f;
                self->dyna.actor.speedXZ = 0.0f;
                self->unk_264 = 0;
                self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
                EnAm_SpawnEffects(self, globalCtx);

                if (((Actor_WorldDistXZToPoint(&self->dyna.actor, &self->dyna.actor.home.pos) > 240.0f) ||
                     (self->attackTimer == 0)) &&
                    (self->deathTimer == 0)) {
                    EnAm_SetupRotateToHome(self);
                }
            }
        }

        // turn and move away from a wall if contact is made with one
        if ((self->dyna.actor.speedXZ != 0.0f) && (self->dyna.actor.bgCheckFlags & 8)) {
            self->dyna.actor.world.rot.y =
                (self->dyna.actor.wallYaw - self->dyna.actor.world.rot.y) + self->dyna.actor.wallYaw;
            Actor_MoveForward(&self->dyna.actor);
            self->dyna.actor.bgCheckFlags &= ~8;
        }

        SkelAnime_Update(&self->skelAnime);

        if ((self->unk_258 == 0) && (self->deathTimer == 0)) {
            EnAm_SetupCooldown(self);
        }

        if (self->deathTimer == 0) {
            self->dyna.actor.shape.rot.y = self->dyna.actor.world.rot.y;
        } else {
            if (self->panicSpinRot < 8000) {
                self->panicSpinRot += 800;
            }
            self->dyna.actor.shape.rot.y += self->panicSpinRot;
        }
    }
}

void EnAm_Statue(EnAm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 temp158f = self->dyna.unk_158;
    s16 moveDir = 0;

    if (self->unk_258 == 0) {
        if (self->dyna.unk_150 != 0.0f) {
            self->unk_258 = 0x8000;
        }
    } else {
        self->unk_258 -= 0x800;
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ROCK_SLIDE - SFX_FLAG);

        if (self->dyna.unk_150 < 0.0f) {
            temp158f = self->dyna.unk_158 + 0x8000;
        }

        if (self->hurtCollider.base.ocFlags1 & OC1_HIT) {
            moveDir = Math_Vec3f_Yaw(&self->dyna.actor.world.pos, &self->hurtCollider.base.oc->world.pos) - temp158f;
        }

        if ((self->dyna.unk_150 == 0.0f) || (self->unk_258 == 0) || !(self->dyna.actor.bgCheckFlags & 1) ||
            !func_800435D8(globalCtx, &self->dyna, 0x14,
                           (Math_SinS(self->unk_258) * (self->dyna.unk_150 * 0.5f)) + 40.0f, 0xA) ||
            ((self->hurtCollider.base.ocFlags1 & OC1_HIT) && (ABS(moveDir) <= 0x2000))) {

            self->unk_258 = 0;
            player->stateFlags2 &= ~0x151;
            player->actor.speedXZ = 0.0f;
            self->dyna.unk_150 = self->dyna.unk_154 = 0.0f;
        }

        self->dyna.actor.world.rot.y = self->dyna.unk_158;
        self->dyna.actor.speedXZ = Math_SinS(self->unk_258) * (self->dyna.unk_150 * 0.5f);
    }

    if (self->dyna.actor.bgCheckFlags & 2) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
    }

    self->dyna.unk_150 = self->dyna.unk_154 = 0.0f;
}

void EnAm_SetupStunned(EnAm* self, GlobalContext* globalCtx) {
    // animation is set but SkelAnime_Update is not called in the action
    // likely copy pasted from EnAm_SetupRecoilFromDamage
    Animation_Change(&self->skelAnime, &gArmosDamagedAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gArmosDamagedAnim),
                     ANIMMODE_ONCE, 0.0f);

    self->dyna.actor.world.rot.y = self->dyna.actor.yawTowardsPlayer;

    if (EnAm_CanMove(self, globalCtx, -6.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = -6.0f;
    }

    Actor_SetColorFilter(&self->dyna.actor, 0, 120, 0, 100);

    if (self->damageEffect == AM_DMGEFF_ICE) {
        self->iceTimer = 48;
    }

    self->behavior = AM_BEHAVIOR_STUNNED;
    Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnAm_SetupAction(self, EnAm_Stunned);
}

void EnAm_Stunned(EnAm* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->dyna.actor.shape.rot.y, self->dyna.actor.world.rot.y, 1, 0xFA0, 0);

    if (self->dyna.actor.speedXZ < 0.0f) {
        self->dyna.actor.speedXZ += 0.5f;
    }

    if ((self->dyna.actor.velocity.y <= 0.0f) && !EnAm_CanMove(self, globalCtx, -9.0f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = 0.0f;
    }

    if (self->dyna.actor.colorFilterTimer == 0) {
        if (self->dyna.actor.colChkInfo.health != 0) {
            EnAm_SetupLunge(self);
        } else {
            EnAm_SetupRecoilFromDamage(self, globalCtx);
        }
    }
}

void EnAm_Ricochet(EnAm* self, GlobalContext* globalCtx) {
    if (self->dyna.actor.speedXZ < 0.0f) {
        self->dyna.actor.speedXZ += 0.5f;
    }

    if ((self->dyna.actor.velocity.y <= 0.0f) &&
        !EnAm_CanMove(self, globalCtx, self->dyna.actor.speedXZ * 1.5f, self->dyna.actor.world.rot.y)) {
        self->dyna.actor.speedXZ = 0.0f;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        self->dyna.actor.speedXZ = 0.0f;
        EnAm_SetupLunge(self);
    }
}

void EnAm_TransformSwordHitbox(Actor* thisx, GlobalContext* globalCtx) {
    static Vec3f D_809B0074 = { 2500.0f, 7000.0f, 0.0f };
    static Vec3f D_809B0080 = { -2500.0f, 0.0f, 0.0f };
    static Vec3f D_809B008C = { 2500.0f, 7000.0f, 4000.0f };
    static Vec3f D_809B0098 = { -2500.0f, 0.0f, 4000.0f };
    EnAm* self = THIS;

    Matrix_MultVec3f(&D_809B0074, &self->hitCollider.dim.quad[1]);
    Matrix_MultVec3f(&D_809B0080, &self->hitCollider.dim.quad[0]);
    Matrix_MultVec3f(&D_809B008C, &self->hitCollider.dim.quad[3]);
    Matrix_MultVec3f(&D_809B0098, &self->hitCollider.dim.quad[2]);

    Collider_SetQuadVertices(&self->hitCollider, &self->hitCollider.dim.quad[0], &self->hitCollider.dim.quad[1],
                             &self->hitCollider.dim.quad[2], &self->hitCollider.dim.quad[3]);
}

void EnAm_UpdateDamage(EnAm* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f sparkPos;

    if (self->deathTimer == 0) {
        if (self->blockCollider.base.acFlags & AC_BOUNCED) {
            self->blockCollider.base.acFlags &= ~(AC_HIT | AC_BOUNCED);
            self->hurtCollider.base.acFlags &= ~AC_HIT;

            if (self->behavior >= AM_BEHAVIOR_5) {
                EnAm_SetupRicochet(self, globalCtx);
            }
        } else if ((self->hurtCollider.base.acFlags & AC_HIT) && (self->behavior >= AM_BEHAVIOR_5)) {
            self->hurtCollider.base.acFlags &= ~AC_HIT;

            if (self->dyna.actor.colChkInfo.damageEffect != AM_DMGEFF_MAGIC_FIRE_LIGHT) {
                self->unk_264 = 0;
                self->damageEffect = self->dyna.actor.colChkInfo.damageEffect;
                Actor_SetDropFlag(&self->dyna.actor, &self->hurtCollider.info, 0);

                if ((self->dyna.actor.colChkInfo.damageEffect == AM_DMGEFF_NUT) ||
                    (self->dyna.actor.colChkInfo.damageEffect == AM_DMGEFF_STUN) ||
                    (self->dyna.actor.colChkInfo.damageEffect == AM_DMGEFF_ICE)) {
                    if (self->behavior != AM_BEHAVIOR_STUNNED) {
                        EnAm_SetupStunned(self, globalCtx);

                        if (self->dyna.actor.colChkInfo.damage != 0) {
                            self->dyna.actor.colChkInfo.health = 0;
                        }
                    } else if (self->dyna.actor.colChkInfo.damageEffect == AM_DMGEFF_STUN) {
                        sparkPos = self->dyna.actor.world.pos;
                        sparkPos.y += 50.0f;
                        CollisionCheck_SpawnShieldParticlesMetal(globalCtx, &sparkPos);
                    }
                } else if ((self->dyna.actor.colChkInfo.damageEffect == AM_DMGEFF_KILL) ||
                           (self->behavior == AM_BEHAVIOR_STUNNED)) {
                    self->dyna.actor.colChkInfo.health = 0;

                    EnAm_SetupRecoilFromDamage(self, globalCtx);
                } else {
                    EnAm_SetupRicochet(self, globalCtx);
                }
            }
        }
    }
}

void EnAm_Update(Actor* thisx, GlobalContext* globalCtx) {
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    static Color_RGBA8 dustPrimColor = { 150, 150, 150, 255 };
    static Color_RGBA8 dustEnvColor = { 100, 100, 100, 150 };
    s32 pad;
    EnAm* self = THIS;
    EnBom* bomb;
    Vec3f dustPos;
    s32 i;
    f32 dustPosScale;
    s32 pad1;

    if (self->dyna.actor.params != ARMOS_STATUE) {
        EnAm_UpdateDamage(self, globalCtx);
    }

    if (self->dyna.actor.colChkInfo.damageEffect != AM_DMGEFF_MAGIC_FIRE_LIGHT) {
        if (self->attackTimer != 0) {
            self->attackTimer--;
        }

        self->actionFunc(self, globalCtx);

        if (self->deathTimer != 0) {
            self->deathTimer--;

            if (self->deathTimer == 0) {
                dustPosScale = globalCtx->gameplayFrames * 10;

                EnAm_SpawnEffects(self, globalCtx);
                bomb =
                    (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, self->dyna.actor.world.pos.x,
                                        self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z, 0, 0, 2, BOMB_BODY);
                if (bomb != NULL) {
                    bomb->timer = 0;
                }

                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_AMOS_DEAD);
                Item_DropCollectibleRandom(globalCtx, &self->dyna.actor, &self->dyna.actor.world.pos, 0xA0);

                for (i = 9; i >= 0; i--) {
                    dustPos.x = (sinf(dustPosScale) * 7.0f) + self->dyna.actor.world.pos.x;
                    dustPos.y = (Rand_CenteredFloat(10.0f) * 6.0f) + (self->dyna.actor.world.pos.y + 40.0f);
                    dustPos.z = (cosf(dustPosScale) * 7.0f) + self->dyna.actor.world.pos.z;

                    func_8002836C(globalCtx, &dustPos, &zeroVec, &zeroVec, &dustPrimColor, &dustEnvColor, 200, 45, 12);
                    dustPosScale += 60.0f;
                }

                Actor_Kill(&self->dyna.actor);
                return;
            }

            if ((self->deathTimer % 4) == 0) {
                Actor_SetColorFilter(&self->dyna.actor, 0x4000, 255, 0, 4);
            }
        }

        Actor_MoveForward(&self->dyna.actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->dyna.actor, 20.0f, 28.0f, 80.0f, 0x1D);
    }

    Collider_UpdateCylinder(&self->dyna.actor, &self->hurtCollider);
    Collider_UpdateCylinder(&self->dyna.actor, &self->blockCollider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->hurtCollider.base);

    if (self->dyna.actor.params != ARMOS_STATUE) {
        Actor_SetFocus(&self->dyna.actor, self->dyna.actor.scale.x * 4500.0f);

        if (self->dyna.actor.colorFilterTimer == 0) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->blockCollider.base);
        }

        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->hurtCollider.base);

        if ((self->behavior >= 4) && (self->unk_264 > 0)) {
            if (!(self->hitCollider.base.atFlags & AT_BOUNCED)) {
                if (self->hitCollider.base.atFlags & AT_HIT) {
                    Player* player = GET_PLAYER(globalCtx);

                    if (self->hitCollider.base.at == &player->actor) {
                        Audio_PlayActorSound2(&player->actor, NA_SE_PL_BODY_HIT);
                    }
                }
                CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->hitCollider.base);
            } else {
                self->hitCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                self->hitCollider.base.at = NULL;
                EnAm_SetupRicochet(self, globalCtx);
            }
        }
    } else {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->blockCollider.base);
    }
}

static Vec3f sUnused1 = { 1100.0f, -700.0f, 0.0f };
static Vec3f sUnused2 = { 0.0f, 0.0f, 0.0f };

void EnAm_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnAm* self = THIS;

    if ((limbIndex == 1) && (self->unk_264 != 0)) {
        EnAm_TransformSwordHitbox(&self->dyna.actor, globalCtx);
    }
}

static Vec3f sIcePosOffsets[] = {
    { 20.0f, 40.0f, 0.0f },   { 10.0f, 60.0f, 10.0f },   { -10.0f, 60.0f, 10.0f }, { -20.0f, 40.0f, 0.0f },
    { 10.0f, 60.0f, -10.0f }, { -10.0f, 60.0f, -10.0f }, { 0.0f, 40.0f, -20.0f },  { 10.0f, 20.0f, 10.0f },
    { 10.0f, 20.0f, -10.0f }, { 0.0f, 40.0f, 20.0f },    { -10.0f, 20.0f, 10.0f }, { -10.0f, 20.0f, -10.0f },
};

void EnAm_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f sp68;
    EnAm* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_am.c", 1580);

    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, self->textureBlend);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, EnAm_PostLimbDraw, self);

    if (self->iceTimer != 0) {
        self->dyna.actor.colorFilterTimer++;
        if (1) {};
        self->iceTimer--;

        if ((self->iceTimer % 4) == 0) {
            s32 index;

            index = self->iceTimer >> 2;

            sp68.x = self->dyna.actor.world.pos.x + sIcePosOffsets[index].x;
            sp68.y = self->dyna.actor.world.pos.y + sIcePosOffsets[index].y;
            sp68.z = self->dyna.actor.world.pos.z + sIcePosOffsets[index].z;

            EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->dyna.actor, &sp68, 150, 150, 150, 250, 235, 245, 255,
                                           1.4f);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_am.c", 1605);
}
