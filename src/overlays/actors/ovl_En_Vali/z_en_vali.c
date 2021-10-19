/*
 * File: z_en_vali.c
 * Overlay: ovl_En_Vali
 * Description: Bari (Big Jellyfish)
 */

#include "z_en_vali.h"
#include "objects/object_vali/object_vali.h"

#define FLAGS 0x00001015

#define THIS ((EnVali*)thisx)

void EnVali_Init(Actor* thisx, GlobalContext* globalCtx);
void EnVali_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnVali_Update(Actor* thisx, GlobalContext* globalCtx);
void EnVali_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnVali_SetupLurk(EnVali* self);
void EnVali_SetupDropAppear(EnVali* self);

void EnVali_Lurk(EnVali* self, GlobalContext* globalCtx);
void EnVali_DropAppear(EnVali* self, GlobalContext* globalCtx);
void EnVali_FloatIdle(EnVali* self, GlobalContext* globalCtx);
void EnVali_Attacked(EnVali* self, GlobalContext* globalCtx);
void EnVali_Retaliate(EnVali* self, GlobalContext* globalCtx);
void EnVali_MoveArmsDown(EnVali* self, GlobalContext* globalCtx);
void EnVali_Burnt(EnVali* self, GlobalContext* globalCtx);
void EnVali_DivideAndDie(EnVali* self, GlobalContext* globalCtx);
void EnVali_Stunned(EnVali* self, GlobalContext* globalCtx);
void EnVali_Frozen(EnVali* self, GlobalContext* globalCtx);
void EnVali_ReturnToLurk(EnVali* self, GlobalContext* globalCtx);

const ActorInit En_Vali_InitVars = {
    ACTOR_EN_VALI,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_VALI,
    sizeof(EnVali),
    (ActorFunc)EnVali_Init,
    (ActorFunc)EnVali_Destroy,
    (ActorFunc)EnVali_Update,
    (ActorFunc)EnVali_Draw,
};

static ColliderQuadInit sQuadInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x07, 0x08 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT8,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x07, 0x08 },
        { 0xFFCFFFFF, 0x01, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 17, 35, -15, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 2, 18, 32, MASS_HEAVY };

typedef enum {
    /* 0x0 */ BARI_DMGEFF_NONE,
    /* 0x1 */ BARI_DMGEFF_STUN,
    /* 0x2 */ BARI_DMGEFF_FIRE,
    /* 0x3 */ BARI_DMGEFF_ICE,
    /* 0xE */ BARI_DMGEFF_SLINGSHOT = 0xE,
    /* 0xF */ BARI_DMGEFF_SWORD
} BariDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, BARI_DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(0, BARI_DMGEFF_SLINGSHOT),
    /* Explosive     */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(0, BARI_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Hookshot      */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Kokiri sword  */ DMG_ENTRY(1, BARI_DMGEFF_SWORD),
    /* Master sword  */ DMG_ENTRY(2, BARI_DMGEFF_SWORD),
    /* Giant's Knife */ DMG_ENTRY(4, BARI_DMGEFF_SWORD),
    /* Fire arrow    */ DMG_ENTRY(4, BARI_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(4, BARI_DMGEFF_ICE),
    /* Light arrow   */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(4, BARI_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(4, BARI_DMGEFF_ICE),
    /* Light magic   */ DMG_ENTRY(0, BARI_DMGEFF_NONE),
    /* Shield        */ DMG_ENTRY(0, BARI_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, BARI_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, BARI_DMGEFF_NONE),
    /* Giant spin    */ DMG_ENTRY(4, BARI_DMGEFF_NONE),
    /* Master spin   */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Kokiri jump   */ DMG_ENTRY(2, BARI_DMGEFF_NONE),
    /* Giant jump    */ DMG_ENTRY(8, BARI_DMGEFF_NONE),
    /* Master jump   */ DMG_ENTRY(4, BARI_DMGEFF_NONE),
    /* Unknown 1     */ DMG_ENTRY(0, BARI_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, BARI_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, BARI_DMGEFF_NONE),
    /* Unknown 2     */ DMG_ENTRY(0, BARI_DMGEFF_NONE),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x18, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 5000, ICHAIN_STOP),
};

void EnVali_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnVali* self = THIS;
    s32 bgId;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 27.0f);
    self->actor.shape.shadowAlpha = 155;
    SkelAnime_Init(globalCtx, &self->skelAnime, &gBariSkel, &gBariLurkingAnim, self->jointTable, self->morphTable,
                   EN_VALI_LIMB_MAX);

    Collider_InitQuad(globalCtx, &self->leftArmCollider);
    Collider_SetQuad(globalCtx, &self->leftArmCollider, &self->actor, &sQuadInit);
    Collider_InitQuad(globalCtx, &self->rightArmCollider);
    Collider_SetQuad(globalCtx, &self->rightArmCollider, &self->actor, &sQuadInit);
    Collider_InitCylinder(globalCtx, &self->bodyCollider);
    Collider_SetCylinder(globalCtx, &self->bodyCollider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    EnVali_SetupLurk(self);

    self->actor.flags &= ~1;
    self->actor.floorHeight = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->actor.floorPoly, &bgId,
                                                          &self->actor, &self->actor.world.pos);
    self->actor.params = BARI_TYPE_NORMAL;

    if (self->actor.floorHeight == BGCHECK_Y_MIN) {
        Actor_Kill(&self->actor);
    }
}

void EnVali_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnVali* self = THIS;

    Collider_DestroyQuad(globalCtx, &self->leftArmCollider);
    Collider_DestroyQuad(globalCtx, &self->rightArmCollider);
    Collider_DestroyCylinder(globalCtx, &self->bodyCollider);
}

void EnVali_SetupLurk(EnVali* self) {
    Animation_PlayLoop(&self->skelAnime, &gBariLurkingAnim);
    self->actor.draw = NULL;
    self->bodyCollider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnVali_Lurk;
}

void EnVali_SetupDropAppear(EnVali* self) {
    self->actor.draw = EnVali_Draw;
    self->actor.flags |= 1;
    self->actor.velocity.y = 1.0f;
    self->actionFunc = EnVali_DropAppear;
}

void EnVali_SetupFloatIdle(EnVali* self) {
    Animation_MorphToLoop(&self->skelAnime, &gBariWaitingAnim, -3.0f);
    self->leftArmCollider.dim.quad[2] = self->leftArmCollider.dim.quad[3] = self->rightArmCollider.dim.quad[2] =
        self->rightArmCollider.dim.quad[3] = self->leftArmCollider.dim.quad[0] = self->leftArmCollider.dim.quad[1] =
            self->rightArmCollider.dim.quad[0] = self->rightArmCollider.dim.quad[1] = self->actor.world.pos;

    self->leftArmCollider.dim.quad[2].y = self->leftArmCollider.dim.quad[3].y = self->rightArmCollider.dim.quad[2].y =
        self->rightArmCollider.dim.quad[3].y = self->leftArmCollider.dim.quad[0].y =
            self->leftArmCollider.dim.quad[1].y = self->rightArmCollider.dim.quad[0].y =
                self->rightArmCollider.dim.quad[1].y = self->actor.world.pos.y - 10.0f;

    self->actor.flags &= ~0x10;
    self->bodyCollider.base.acFlags |= AC_ON;
    self->slingshotReactionTimer = 0;
    self->floatHomeHeight = self->actor.world.pos.y;
    self->actionFunc = EnVali_FloatIdle;
}

/**
 * Used for both touching player/player's shield and being hit with sword. What to do next is determined by params.
 */
void EnVali_SetupAttacked(EnVali* self) {
    self->lightningTimer = 20;
    self->actor.flags &= ~1;
    self->bodyCollider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnVali_Attacked;
}

void EnVali_SetupRetaliate(EnVali* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBariRetaliatingAnim, -5.0f);
    Actor_SetColorFilter(&self->actor, 0x4000, 150, 0x2000, 30);
    self->actor.params = BARI_TYPE_NORMAL;
    self->bodyCollider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnVali_Retaliate;
}

void EnVali_SetupMoveArmsDown(EnVali* self) {
    Animation_PlayOnce(&self->skelAnime, &gBariMovingArmsDownAnim);
    self->actionFunc = EnVali_MoveArmsDown;
}

void EnVali_SetupBurnt(EnVali* self) {
    self->timer = 2;
    self->bodyCollider.base.acFlags &= ~AC_ON;
    Actor_SetColorFilter(&self->actor, 0x4000, 150, 0x2000, 30);
    self->actionFunc = EnVali_Burnt;
}

void EnVali_SetupDivideAndDie(EnVali* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i < 3; i++) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BILI, self->actor.world.pos.x, self->actor.world.pos.y,
                    self->actor.world.pos.z, 0, self->actor.world.rot.y, 0, 0);

        self->actor.world.rot.y += 0x10000 / 3;
    }

    Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x50);
    self->timer = Rand_S16Offset(10, 10);
    self->bodyCollider.base.acFlags &= ~AC_ON;
    Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 40, NA_SE_EN_BARI_SPLIT);
    self->actor.flags &= ~1;
    self->actor.draw = NULL;
    self->actionFunc = EnVali_DivideAndDie;
}

void EnVali_SetupStunned(EnVali* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBariWaitingAnim, 10.0f);
    self->timer = 80;
    self->actor.velocity.y = 0.0f;
    Actor_SetColorFilter(&self->actor, 0, 255, 0x2000, 80);
    self->bodyCollider.info.bumper.effect = 0;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->actor.velocity.y = 1.0f;
    self->actionFunc = EnVali_Stunned;
}

void EnVali_SetupFrozen(EnVali* self) {
    self->actor.velocity.y = 0.0f;
    Actor_SetColorFilter(&self->actor, 0, 255, 0x2000, 36);
    self->bodyCollider.base.acFlags &= ~AC_ON;
    self->timer = 36;
    self->actionFunc = EnVali_Frozen;
}

void EnVali_SetupReturnToLurk(EnVali* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBariLurkingAnim, 10.0f);
    self->actor.flags |= 0x10;
    self->actor.flags &= ~1;
    self->actionFunc = EnVali_ReturnToLurk;
}

void EnVali_DischargeLightning(EnVali* self, GlobalContext* globalCtx) {
    static Color_RGBA8 primColor = { 255, 255, 255, 255 };
    static Color_RGBA8 envColor = { 200, 255, 255, 255 };
    Vec3f pos;
    s32 i;
    f32 cos;
    f32 sin;
    s16 yaw;

    for (i = 0; i < 4; i++) {
        cos = -Math_CosS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)));
        sin = Math_SinS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)));
        if (!((self->lightningTimer + (i << 1)) % 4)) {
            yaw = (s16)Rand_CenteredFloat(12288.0f) + (i * 0x4000) + 0x2000;
            pos.x = self->actor.world.pos.x + (Math_SinS(yaw) * 12.0f * cos);
            pos.y = self->actor.world.pos.y - (Math_CosS(yaw) * 12.0f) + 10.0f;
            pos.z = self->actor.world.pos.z + (Math_SinS(yaw) * 12.0f * sin);

            EffectSsLightning_Spawn(globalCtx, &pos, &primColor, &envColor, 17, yaw, 6, 2);
        }
    }

    func_8002F974(&self->actor, NA_SE_EN_BIRI_SPARK - SFX_FLAG);
}

void EnVali_Lurk(EnVali* self, GlobalContext* globalCtx) {
    if (self->actor.xzDistToPlayer < 150.0f) {
        EnVali_SetupDropAppear(self);
    }
}

void EnVali_DropAppear(EnVali* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    self->actor.velocity.y *= 1.5f;
    self->actor.velocity.y = CLAMP_MAX(self->actor.velocity.y, 40.0f);

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, self->actor.velocity.y)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        EnVali_SetupFloatIdle(self);
    }
}

void EnVali_FloatIdle(EnVali* self, GlobalContext* globalCtx) {
    s32 curFrame;

    SkelAnime_Update(&self->skelAnime);

    if (self->slingshotReactionTimer != 0) {
        self->slingshotReactionTimer--;
    }

    curFrame = self->skelAnime.curFrame;

    Math_StepToF(&self->floatHomeHeight, self->actor.floorHeight + 40.0f, 1.2f);
    self->actor.world.pos.y = self->floatHomeHeight - (sinf(curFrame * M_PI * 0.0125f) * 8.0f);

    if (self->slingshotReactionTimer) {
        self->actor.shape.rot.y += 0x800;

        if (((self->slingshotReactionTimer % 6) == 0) && (curFrame > 15) && (curFrame <= 55)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BARI_ROLL);
        }
    } else if ((curFrame == 16) || (curFrame == 30) || (curFrame == 42) || (curFrame == 55)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BARI_ROLL);
    }

    curFrame = ((curFrame > 40) ? (80 - curFrame) : curFrame);

    self->actor.shape.rot.y += (s16)((curFrame + 4) * 0.4f * (0x10000 / 360.0f));
    if (self->actor.xzDistToPlayer > 250.0f) {
        EnVali_SetupReturnToLurk(self);
    }
}

void EnVali_Attacked(EnVali* self, GlobalContext* globalCtx) {
    if (self->lightningTimer != 0) {
        self->lightningTimer--;
    }

    EnVali_DischargeLightning(self, globalCtx);

    if (self->lightningTimer == 0) {
        self->actor.flags |= 1;
        self->bodyCollider.base.acFlags |= AC_ON;
        if (self->actor.params == BARI_TYPE_SWORD_DAMAGE) {
            EnVali_SetupRetaliate(self);
        } else {
            self->actionFunc = EnVali_FloatIdle;
        }
    } else if ((self->lightningTimer % 2) != 0) {
        self->actor.world.pos.y += 1.0f;
    } else {
        self->actor.world.pos.y -= 1.0f;
    }
}

void EnVali_Retaliate(EnVali* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.colChkInfo.health != 0) {
            EnVali_SetupMoveArmsDown(self);
        } else {
            EnVali_SetupDivideAndDie(self, globalCtx);
        }
    }
}

void EnVali_MoveArmsDown(EnVali* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnVali_SetupFloatIdle(self);
    }
}

void EnVali_Burnt(EnVali* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        EnVali_SetupDivideAndDie(self, globalCtx);
    }
}

void EnVali_DivideAndDie(EnVali* self, GlobalContext* globalCtx) {
    static Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    static Vec3f accel = { 0.0f, 0.0f, 0.0f };
    s16 scale;
    Vec3f pos;
    s32 i;

    if (self->timer != 0) {
        self->timer--;
    }

    for (i = 0; i < 2; i++) {
        pos.x = self->actor.world.pos.x + Rand_CenteredFloat(20.0f);
        pos.y = self->actor.world.pos.y + Rand_CenteredFloat(8.0f);
        pos.z = self->actor.world.pos.z + Rand_CenteredFloat(20.0f);
        velocity.y = (Rand_ZeroOne() + 1.0f);
        scale = Rand_S16Offset(40, 40);

        if (Rand_ZeroOne() < 0.7f) {
            EffectSsDtBubble_SpawnColorProfile(globalCtx, &pos, &velocity, &accel, scale, 25, 2, 1);
        } else {
            EffectSsDtBubble_SpawnColorProfile(globalCtx, &pos, &velocity, &accel, scale, 25, 0, 1);
        }
    }

    if (self->timer == 0) {
        Actor_Kill(&self->actor);
    }
}

void EnVali_Stunned(EnVali* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->actor.velocity.y != 0.0f) {
        if (Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, self->actor.velocity.y)) {
            self->actor.velocity.y = 0.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        } else {
            self->actor.velocity.y += 1.0f;
        }
    }

    if (self->timer == 0) {
        self->bodyCollider.info.bumper.effect = 1; // Shock?
        EnVali_SetupFloatIdle(self);
    }
}

void EnVali_Frozen(EnVali* self, GlobalContext* globalCtx) {
    Vec3f pos;
    s32 temp_v0;
    s32 temp_v1;

    if (self->timer != 0) {
        self->timer--;
    }

    temp_v1 = self->timer - 20;
    self->actor.colorFilterTimer = 36;

    if (temp_v1 > 0) {
        temp_v0 = temp_v1 >> 1;

        if ((self->timer % 2) != 0) {
            pos.y = self->actor.world.pos.y - 20.0f + (-temp_v0 * 5 + 40);
            pos.x = self->actor.world.pos.x + ((temp_v0 & 2) ? 12.0f : -12.0f);
            pos.z = self->actor.world.pos.z + ((temp_v0 & 1) ? 12.0f : -12.0f);

            EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &pos, 150, 150, 150, 250, 235, 245, 255,
                                           (Rand_ZeroOne() * 0.2f) + 1.3f);
        }
    } else if (self->timer == 0) {
        self->actor.velocity.y += 1.0f;
        if (Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, self->actor.velocity.y)) {
            EnVali_SetupDivideAndDie(self, globalCtx);
            self->actor.colorFilterTimer = 0;
        }
    }
}

void EnVali_ReturnToLurk(EnVali* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.5f, 15.0f, 0.1f) < 0.01f) {
        EnVali_SetupLurk(self);
    }
}

void EnVali_UpdateDamage(EnVali* self, GlobalContext* globalCtx) {
    if (self->bodyCollider.base.acFlags & AC_HIT) {
        self->bodyCollider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->bodyCollider.info, 1);

        if ((self->actor.colChkInfo.damageEffect != BARI_DMGEFF_NONE) || (self->actor.colChkInfo.damage != 0)) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_BARI_DEAD);
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                self->actor.flags &= ~1;
            } else if ((self->actor.colChkInfo.damageEffect != BARI_DMGEFF_STUN) &&
                       (self->actor.colChkInfo.damageEffect != BARI_DMGEFF_SLINGSHOT)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_BARI_DAMAGE);
            }

            if (self->actor.colChkInfo.damageEffect == BARI_DMGEFF_STUN) {
                if (self->actionFunc != EnVali_Stunned) {
                    EnVali_SetupStunned(self);
                }
            } else if (self->actor.colChkInfo.damageEffect == BARI_DMGEFF_SWORD) {
                if (self->actionFunc != EnVali_Stunned) {
                    Actor_SetColorFilter(&self->actor, 0x4000, 150, 0x2000, 30);
                    self->actor.params = BARI_TYPE_SWORD_DAMAGE;
                    EnVali_SetupAttacked(self);
                } else {
                    EnVali_SetupRetaliate(self);
                }
            } else if (self->actor.colChkInfo.damageEffect == BARI_DMGEFF_FIRE) {
                EnVali_SetupBurnt(self);
            } else if (self->actor.colChkInfo.damageEffect == BARI_DMGEFF_ICE) {
                EnVali_SetupFrozen(self);
            } else if (self->actor.colChkInfo.damageEffect == BARI_DMGEFF_SLINGSHOT) {
                if (self->slingshotReactionTimer == 0) {
                    self->slingshotReactionTimer = 20;
                }
            } else {
                EnVali_SetupRetaliate(self);
            }
        }
    }
}

void EnVali_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnVali* self = THIS;

    if ((self->bodyCollider.base.atFlags & AT_HIT) || (self->leftArmCollider.base.atFlags & AT_HIT) ||
        (self->rightArmCollider.base.atFlags & AT_HIT)) {
        self->leftArmCollider.base.atFlags &= ~AT_HIT;
        self->rightArmCollider.base.atFlags &= ~AT_HIT;
        self->bodyCollider.base.atFlags &= ~AT_HIT;
        EnVali_SetupAttacked(self);
    }

    EnVali_UpdateDamage(self, globalCtx);
    self->actionFunc(self, globalCtx);

    if ((self->actionFunc != EnVali_DivideAndDie) && (self->actionFunc != EnVali_Lurk)) {
        Collider_UpdateCylinder(&self->actor, &self->bodyCollider);

        if (self->actionFunc == EnVali_FloatIdle) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->leftArmCollider.base);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->rightArmCollider.base);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
        }

        if (self->bodyCollider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
        }

        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
        Actor_SetFocus(&self->actor, 0.0f);
    }
}

// Draw and associated functions

void EnVali_PulseOutside(EnVali* self, f32 curFrame, Vec3f* scale) {
    f32 scaleChange;

    if (self->actionFunc == EnVali_Attacked) {
        s32 scalePhase = 20 - (self->lightningTimer % 20);

        if (scalePhase >= 10) {
            scalePhase -= 10;
        }

        scale->y -= 0.2f * sinf((M_PI / 10) * scalePhase);
    } else if (self->actionFunc == EnVali_Retaliate) {
        scaleChange = sinf((M_PI / 10) * curFrame);
        scale->y -= 0.24f * scaleChange;
        scale->x -= 0.13f * scaleChange;
        scale->z = scale->x;
    } else if (self->actionFunc == EnVali_MoveArmsDown) {
        scaleChange = cosf((M_PI / 50) * curFrame);
        scale->y -= 0.24f * scaleChange;
        scale->x -= 0.13f * scaleChange;
        scale->z = scale->x;
    } else if (self->actionFunc == EnVali_Stunned) {
        scaleChange = sinf((M_PI / 10) * self->timer) * 0.08f;
        scale->x += scaleChange;
        scale->y -= scaleChange;
        scale->z += scaleChange;
    } else {
        if (curFrame >= 40.0f) {
            curFrame -= 40.0f;
        }

        scale->y -= 0.2f * sinf((M_PI / 40) * curFrame);
    }
}

void EnVali_PulseInsides(EnVali* self, f32 curFrame, Vec3f* scale) {
    f32 scaleChange;

    if (self->actionFunc == EnVali_Attacked) {
        s32 scalePhase = 20 - (self->lightningTimer % 20);

        if (scalePhase >= 10) {
            scalePhase -= 10;
        }

        scale->y -= 0.13f * sinf((M_PI / 10) * scalePhase);
    } else if (self->actionFunc == EnVali_Retaliate) {
        scaleChange = sinf((M_PI / 10) * curFrame);
        scale->y -= 0.18f * scaleChange;
        scale->x -= 0.1f * scaleChange;
        scale->z = scale->x;
    } else if (self->actionFunc == EnVali_MoveArmsDown) {
        scaleChange = cosf((M_PI / 50) * curFrame);
        scale->y -= 0.18f * scaleChange;
        scale->x -= 0.1f * scaleChange;
        scale->z = scale->x;
    } else if (self->actionFunc == EnVali_Stunned) {
        scaleChange = sinf((M_PI / 10) * self->timer) * 0.08f;
        scale->x -= scaleChange;
        scale->y += scaleChange;
        scale->z -= scaleChange;
    } else {
        if (curFrame >= 40.0f) {
            curFrame -= 40.0f;
        }

        scale->y -= 0.13f * sinf((M_PI / 40) * curFrame);
    }
}

s32 EnVali_SetArmLength(EnVali* self, f32 curFrame) {
    f32 targetArmScale;

    if (self->actionFunc == EnVali_FloatIdle) {
        if (curFrame <= 10.0f) {
            targetArmScale = curFrame * 0.05f + 1.0f;
        } else if (curFrame > 70.0f) {
            targetArmScale = (80.0f - curFrame) * 0.05f + 1.0f;
        } else {
            targetArmScale = 1.5f;
        }
    } else if (self->actionFunc == EnVali_Retaliate) {
        targetArmScale = 1.0f - sinf((M_PI / 10) * curFrame) * 0.35f;
    } else if (self->actionFunc == EnVali_MoveArmsDown) {
        targetArmScale = 1.0f - cosf((M_PI / 50) * curFrame) * 0.35f;
    } else if ((self->actionFunc == EnVali_Attacked) || (self->actionFunc == EnVali_Frozen)) {
        targetArmScale = self->armScale;
    } else {
        targetArmScale = 1.0f;
    }

    Math_StepToF(&self->armScale, targetArmScale, 0.1f);

    if (self->armScale == 1.0f) {
        return false;
    } else {
        return true;
    }
}

s32 EnVali_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                            Gfx** gfx) {
    EnVali* self = THIS;
    f32 curFrame;

    if ((limbIndex == EN_VALI_LIMB_NUCLEUS) || (limbIndex == EN_VALI_LIMB_OUTER_HOOD) ||
        (limbIndex == EN_VALI_LIMB_INNER_HOOD)) {
        *dList = NULL;
        return false;
    } else {
        curFrame = self->skelAnime.curFrame;

        if ((limbIndex == EN_VALI_LIMB_LEFT_ARM_BASE) || (limbIndex == EN_VALI_LIMB_RIGHT_ARM_BASE)) {
            if (EnVali_SetArmLength(self, curFrame)) {
                Matrix_Scale(self->armScale, 1.0f, 1.0f, MTXMODE_APPLY);
            }
        }

        return false;
    }
}

void EnVali_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    static Vec3f D_80B28970 = { 3000.0f, 0.0f, 0.0f };
    static Vec3f D_80B2897C = { -1000.0f, 0.0f, 0.0f };
    Vec3f sp3C;
    Vec3f sp30;
    EnVali* self = THIS;

    if (self->actionFunc == EnVali_FloatIdle) {
        if ((limbIndex == EN_VALI_LIMB_LEFT_FOREARM_BASE) || (limbIndex == EN_VALI_LIMB_RIGHT_FOREARM_BASE)) {
            Matrix_MultVec3f(&D_80B28970, &sp3C);
            Matrix_MultVec3f(&D_80B2897C, &sp30);

            if (limbIndex == EN_VALI_LIMB_LEFT_FOREARM_BASE) {
                Collider_SetQuadVertices(&self->leftArmCollider, &sp30, &sp3C, &self->leftArmCollider.dim.quad[0],
                                         &self->leftArmCollider.dim.quad[1]);
            } else {
                Collider_SetQuadVertices(&self->rightArmCollider, &sp30, &sp3C, &self->rightArmCollider.dim.quad[0],
                                         &self->rightArmCollider.dim.quad[1]);
            }
        }
    }
}

void EnVali_DrawBody(EnVali* self, GlobalContext* globalCtx) {
    MtxF mtx;
    f32 cos;
    f32 sin;
    f32 curFrame;
    Vec3f scale = { 1.0f, 1.0f, 1.0f };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_vali.c", 1428);

    Matrix_Get(&mtx);
    curFrame = self->skelAnime.curFrame;
    EnVali_PulseInsides(self, curFrame, &scale);
    Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vali.c", 1436),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gBariInnerHoodDL);

    Matrix_Put(&mtx);
    Matrix_RotateY(-self->actor.shape.rot.y * (M_PI / 32768.0f), MTXMODE_APPLY);

    cos = Math_CosS(self->actor.shape.rot.y);
    sin = Math_SinS(self->actor.shape.rot.y);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vali.c", 1446),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gBariNucleusDL);

    Matrix_Translate((506.0f * cos) + (372.0f * sin), 1114.0f, (372.0f * cos) - (506.0f * sin), MTXMODE_APPLY);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vali.c", 1455),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gBariNucleusDL);

    Matrix_Translate((-964.0f * cos) - (804.0f * sin), -108.0f, (-804.0f * cos) + (964.0f * sin), MTXMODE_APPLY);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vali.c", 1463),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gBariNucleusDL);

    Matrix_Put(&mtx);

    scale.x = scale.y = scale.z = 1.0f;

    EnVali_PulseOutside(self, curFrame, &scale);
    Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vali.c", 1471),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gBariOuterHoodDL);

    Matrix_Put(&mtx);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_vali.c", 1477);
}

static Gfx D_80B28998[] = {
    gsDPSetCombineLERP(1, TEXEL0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, TEXEL1, 0,
                       PRIM_LOD_FRAC, COMBINED),
    gsSPEndDisplayList(),
};

static Gfx D_80B289A8[] = {
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, TEXEL1, 0,
                       PRIM_LOD_FRAC, COMBINED),
    gsSPEndDisplayList(),
};

void EnVali_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnVali* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_vali.c", 1505);
    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (127 - (globalCtx->gameplayFrames * 12)) % 128, 32, 32));

    if ((self->lightningTimer % 2) != 0) {
        gSPSegment(POLY_XLU_DISP++, 0x09, D_80B28998);
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x09, D_80B289A8);
    }

    EnVali_DrawBody(self, globalCtx);

    POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                   EnVali_OverrideLimbDraw, EnVali_PostLimbDraw, self, POLY_XLU_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_vali.c", 1538);
}
