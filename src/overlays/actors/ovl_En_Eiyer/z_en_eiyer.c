#include "z_en_eiyer.h"
#include "objects/object_ei/object_ei.h"

#define FLAGS 0x00000005

#define THIS ((EnEiyer*)thisx)

void EnEiyer_Init(Actor* thisx, GlobalContext* globalCtx);
void EnEiyer_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnEiyer_Update(Actor* thisx, GlobalContext* globalCtx);
void EnEiyer_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnEiyer_SetupAppearFromGround(EnEiyer* self);
void EnEiyer_SetupUnderground(EnEiyer* self);
void EnEiyer_SetupInactive(EnEiyer* self);
void EnEiyer_SetupAmbush(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_SetupGlide(EnEiyer* self);
void EnEiyer_SetupStartAttack(EnEiyer* self);
void EnEiyer_SetupDiveAttack(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_SetupLand(EnEiyer* self);
void EnEiyer_SetupHurt(EnEiyer* self);
void EnEiyer_SetupDie(EnEiyer* self);
void EnEiyer_SetupDead(EnEiyer* self);
void EnEiyer_SetupStunned(EnEiyer* self);

void EnEiyer_AppearFromGround(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_WanderUnderground(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_CircleUnderground(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_Inactive(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_Ambush(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_Glide(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_StartAttack(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_DiveAttack(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_Land(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_Hurt(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_Die(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_Dead(EnEiyer* self, GlobalContext* globalCtx);
void EnEiyer_Stunned(EnEiyer* self, GlobalContext* globalCtx);

const ActorInit En_Eiyer_InitVars = {
    ACTOR_EN_EIYER,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_EI,
    sizeof(EnEiyer),
    (ActorFunc)EnEiyer_Init,
    (ActorFunc)EnEiyer_Destroy,
    (ActorFunc)EnEiyer_Update,
    (ActorFunc)EnEiyer_Draw,
};

static ColliderCylinderInit sColCylInit = {
    {
        COLTYPE_HIT0,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x04, 0x08 },
        { 0x00000019, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_ON,
        OCELEM_ON,
    },
    { 27, 17, -10, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 2, 45, 15, 100 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x1),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(2, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(2, 0x0),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(4, 0x0),
    /* Light magic   */ DMG_ENTRY(4, 0x0),
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
    ICHAIN_S8(naviEnemyId, 0x19, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 5, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2500, ICHAIN_STOP),
};

/**
 * params    0: Spawn 3 clones and circle around spawn point
 * params  1-3: Clone, spawn another clone for the main Eiyer if params < 3
 * params   10: Normal Eiyer, wander around spawn point
 */
void EnEiyer_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnEiyer* self = THIS;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 600.0f, ActorShadow_DrawCircle, 65.0f);
    SkelAnime_Init(globalCtx, &self->skelanime, &gStingerSkel, &gStingerIdleAnim, self->jointTable, self->morphTable,
                   19);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sColCylInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    if (self->actor.params < 3) {
        // Each clone spawns another clone
        if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_EIYER, self->actor.home.pos.x,
                               self->actor.home.pos.y, self->actor.home.pos.z, 0, self->actor.shape.rot.y + 0x4000, 0,
                               self->actor.params + 1) == NULL) {
            Actor_Kill(&self->actor);
            return;
        }

        if (self->actor.params == 0) {
            Actor* child = self->actor.child;
            s32 clonesSpawned;

            for (clonesSpawned = 0; clonesSpawned != 3; clonesSpawned++) {
                if (child == NULL) {
                    break;
                }
                child = child->child;
            }

            if (clonesSpawned != 3) {
                for (child = &self->actor; child != NULL; child = child->child) {
                    Actor_Kill(child);
                }
                return;
            } else {
                self->actor.child->parent = &self->actor;
                self->actor.child->child->parent = &self->actor;
                self->actor.child->child->child->parent = &self->actor;
            }
        }
    }

    if (self->actor.params == 0 || self->actor.params == 10) {
        EnEiyer_SetupAppearFromGround(self);
    } else {
        EnEiyer_SetupInactive(self);
    }
}

void EnEiyer_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnEiyer* self = THIS;
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnEiyer_RotateAroundHome(EnEiyer* self) {
    self->actor.world.pos.x = Math_SinS(self->actor.world.rot.y) * 80.0f + self->actor.home.pos.x;
    self->actor.world.pos.z = Math_CosS(self->actor.world.rot.y) * 80.0f + self->actor.home.pos.z;
    self->actor.shape.rot.y = self->actor.world.rot.y + 0x4000;
}

void EnEiyer_SetupAppearFromGround(EnEiyer* self) {
    self->collider.info.bumper.dmgFlags = 0x19;
    Animation_PlayLoop(&self->skelanime, &gStingerIdleAnim);

    self->actor.world.pos.x = self->actor.home.pos.x;
    self->actor.world.pos.y = self->actor.home.pos.y - 40.0f;
    self->actor.world.pos.z = self->actor.home.pos.z;
    self->actor.velocity.y = 0.0f;
    self->actor.speedXZ = 0.0f;

    if (self->actor.params != 0xA) {
        if (self->actor.params == 0) {
            self->actor.world.rot.y = Rand_ZeroOne() * 0x10000;
        } else {
            self->actor.world.rot.y = self->actor.parent->world.rot.y + self->actor.params * 0x4000;
        }
        EnEiyer_RotateAroundHome(self);
    } else {
        self->actor.world.rot.y = self->actor.shape.rot.y = Rand_ZeroOne() * 0x10000;
    }

    self->collider.base.atFlags &= ~AT_ON;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.flags &= ~0x1001;
    self->actor.shape.shadowScale = 0.0f;
    self->actor.shape.yOffset = 0.0f;
    self->actionFunc = EnEiyer_AppearFromGround;
}

void EnEiyer_SetupUnderground(EnEiyer* self) {
    if (self->actor.params == 0xA) {
        self->actor.speedXZ = -0.5f;
        self->actionFunc = EnEiyer_WanderUnderground;
    } else {
        self->actionFunc = EnEiyer_CircleUnderground;
    }

    self->collider.base.acFlags |= AC_ON;
    self->actor.flags &= ~0x10;
    self->actor.flags |= 1;
}

void EnEiyer_SetupInactive(EnEiyer* self) {
    self->actor.flags &= ~1;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actionFunc = EnEiyer_Inactive;
}

void EnEiyer_SetupAmbush(EnEiyer* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 0.0f;
    Animation_PlayOnce(&self->skelanime, &gStingerBackflipAnim);
    self->collider.info.bumper.dmgFlags = ~0x00300000;
    self->basePos = self->actor.world.pos;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.flags |= 0x1000;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.shape.shadowScale = 65.0f;
    self->actor.shape.yOffset = 600.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_JUMP);
    EffectSsGSplash_Spawn(globalCtx, &self->actor.world.pos, NULL, NULL, 1, 700);
    self->actionFunc = EnEiyer_Ambush;
}

void EnEiyer_SetupGlide(EnEiyer* self) {
    self->targetYaw = self->actor.shape.rot.y;
    self->basePos.y = (cosf(-M_PI / 8) * 5.0f) + self->actor.world.pos.y;
    Animation_MorphToLoop(&self->skelanime, &gStingerHitAnim, -5.0f);
    self->timer = 60;
    self->actionFunc = EnEiyer_Glide;
}

void EnEiyer_SetupStartAttack(EnEiyer* self) {
    self->actionFunc = EnEiyer_StartAttack;
}

void EnEiyer_SetupDiveAttack(EnEiyer* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->actor.velocity.y = 0.0f;
    self->basePos.y = player->actor.world.pos.y + 15.0f;
    self->collider.base.atFlags |= AT_ON;
    self->collider.base.atFlags &= ~AT_HIT;
    self->actionFunc = EnEiyer_DiveAttack;
}

void EnEiyer_SetupLand(EnEiyer* self) {
    Animation_MorphToPlayOnce(&self->skelanime, &gStingerDiveAnim, -3.0f);
    self->collider.base.atFlags &= ~AT_ON;
    self->actor.flags |= 0x10;

    // Update BgCheck info, play sound, and spawn effect on the first frame of the land action
    self->timer = -1;
    self->actor.gravity = 0.0f;
    self->collider.dim.height = sColCylInit.dim.height;
    self->actionFunc = EnEiyer_Land;
}

void EnEiyer_SetupHurt(EnEiyer* self) {
    self->basePos.y = self->actor.world.pos.y;
    Animation_Change(&self->skelanime, &gStingerHitAnim, 2.0f, 0.0f, 0.0f, 0, -3.0f);
    self->timer = 40;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.speedXZ = 5.0f;
    Actor_SetColorFilter(&self->actor, 0x4000, 200, 0, 40);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnEiyer_Hurt;
}

void EnEiyer_SetupDie(EnEiyer* self) {
    self->timer = 20;
    Actor_SetColorFilter(&self->actor, 0x4000, 200, 0, 40);

    if (self->collider.info.bumper.dmgFlags != 0x19) {
        self->actor.speedXZ = 6.0f;
        Animation_MorphToLoop(&self->skelanime, &gStingerHitAnim, -3.0f);
    } else {
        self->actor.speedXZ -= 6.0f;
    }

    self->collider.info.bumper.dmgFlags = ~0x00300000;
    self->collider.base.atFlags &= ~AT_ON;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnEiyer_Die;
}

void EnEiyer_SetupDead(EnEiyer* self) {
    self->actor.colorFilterParams |= 0x2000;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actionFunc = EnEiyer_Dead;
}

void EnEiyer_SetupStunned(EnEiyer* self) {
    Animation_Change(&self->skelanime, &gStingerPopOutAnim, 2.0f, 0.0f, 0.0f, 0, -8.0f);
    self->timer = 80;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = -1.0f;
    self->collider.dim.height = sColCylInit.dim.height + 8;
    Actor_SetColorFilter(&self->actor, 0, 200, 0, 80);
    self->collider.base.atFlags &= ~AT_ON;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->actionFunc = EnEiyer_Stunned;
}

void EnEiyer_AppearFromGround(EnEiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.75f)) {
        EnEiyer_SetupUnderground(self);
    }
}

void EnEiyer_CheckPlayerCollision(EnEiyer* self, GlobalContext* globalCtx) {
    if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
        self->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        EnEiyer_SetupAmbush(self, globalCtx);
    }
}

void EnEiyer_CircleUnderground(EnEiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    self->actor.world.rot.y += -0x60;
    EnEiyer_RotateAroundHome(self);
    EnEiyer_CheckPlayerCollision(self, globalCtx);

    // Clones disappear when the main Eiyer leaves the ground
    if (self->actor.params != 0 && ((EnEiyer*)self->actor.parent)->actionFunc != EnEiyer_CircleUnderground) {
        EnEiyer_SetupInactive(self);
    }
}

void EnEiyer_WanderUnderground(EnEiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) > 100.0f) {
        self->targetYaw = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos) + 0x8000;
    } else if (self->targetYaw == self->actor.world.rot.y && Rand_ZeroOne() > 0.99f) {
        self->targetYaw =
            self->actor.world.rot.y + (Rand_ZeroOne() < 0.5f ? -1 : 1) * (Rand_ZeroOne() * 0x2000 + 0x2000);
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, self->targetYaw, 0xB6);
    EnEiyer_CheckPlayerCollision(self, globalCtx);
}

void EnEiyer_Inactive(EnEiyer* self, GlobalContext* globalCtx) {
    EnEiyer* parent;

    if (self->actor.home.pos.y - 50.0f < self->actor.world.pos.y) {
        self->actor.world.pos.y -= 0.5f;
    }

    parent = (EnEiyer*)self->actor.parent;
    if (parent->actionFunc == EnEiyer_Dead) {
        Actor_Kill(&self->actor);
    } else if (parent->actionFunc == EnEiyer_AppearFromGround) {
        EnEiyer_SetupAppearFromGround(self);
    }
}

void EnEiyer_Ambush(EnEiyer* self, GlobalContext* globalCtx) {
    s32 animFinished;
    f32 curFrame;
    f32 xzOffset;
    s32 bgId;

    animFinished = SkelAnime_Update(&self->skelanime);
    curFrame = self->skelanime.curFrame;

    if (self->skelanime.curFrame < 12.0f) {
        self->actor.world.pos.y = ((1.0f - cosf((0.996f * M_PI / 12.0f) * curFrame)) * 40.0f) + self->actor.home.pos.y;
        xzOffset = sinf((0.996f * M_PI / 12.0f) * curFrame) * -40.0f;
        self->actor.world.pos.x = (Math_SinS(self->actor.shape.rot.y) * xzOffset) + self->basePos.x;
        self->actor.world.pos.z = (Math_CosS(self->actor.shape.rot.y) * xzOffset) + self->basePos.z;
    } else {
        Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 80.0f, 0.5f);
        self->actor.speedXZ = 0.8f;
    }

    if (animFinished) {
        self->collider.base.acFlags |= AC_ON;
        EnEiyer_SetupGlide(self);
    } else {
        self->actor.floorHeight = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->actor.floorPoly, &bgId,
                                                              &self->actor, &self->actor.world.pos);
    }
}

void EnEiyer_Glide(EnEiyer* self, GlobalContext* globalCtx) {
    f32 curFrame;
    s32 pad;
    s16 yawChange;

    SkelAnime_Update(&self->skelanime);

    if (self->timer != 0) {
        self->timer--;
    }

    curFrame = self->skelanime.curFrame;
    Math_ApproachF(&self->basePos.y, self->actor.floorHeight + 80.0f + 5.0f, 0.3f, self->actor.speedXZ);
    self->actor.world.pos.y = self->basePos.y - cosf((curFrame - 5.0f) * (M_PI / 40)) * 5.0f;

    if (curFrame <= 45.0f) {
        Math_StepToF(&self->actor.speedXZ, 1.0f, 0.03f);
    } else {
        Math_StepToF(&self->actor.speedXZ, 1.5f, 0.03f);
    }

    if (self->actor.bgCheckFlags & 8) {
        self->targetYaw = self->actor.wallYaw;
    }

    if (Math_ScaledStepToS(&self->actor.world.rot.y, self->targetYaw, 0xB6)) {
        if (self->timer != 0 || Rand_ZeroOne() > 0.05f) {
            self->actor.world.rot.y += 0x100;
        } else {
            yawChange = Rand_S16Offset(0x2000, 0x2000);
            self->targetYaw = (Rand_ZeroOne() < 0.5f ? -1 : 1) * yawChange + self->actor.world.rot.y;
        }
    }

    if (self->timer == 0 && self->actor.yDistToPlayer < 0.0f && self->actor.xzDistToPlayer < 120.0f) {
        EnEiyer_SetupStartAttack(self);
    }

    func_8002F974(&self->actor, NA_SE_EN_EIER_FLY - SFX_FLAG);
}

void EnEiyer_StartAttack(EnEiyer* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f focus;

    SkelAnime_Update(&self->skelanime);

    if (self->actor.shape.rot.x > 0 && self->actor.shape.rot.x < 0x8000) {
        focus.x = player->actor.world.pos.x;
        focus.y = player->actor.world.pos.y + 20.0f;
        focus.z = player->actor.world.pos.z;

        if (Math_ScaledStepToS(&self->actor.shape.rot.x, Actor_WorldPitchTowardPoint(&self->actor, &focus), 0x1000)) {
            EnEiyer_SetupDiveAttack(self, globalCtx);
        }
    } else {
        self->actor.shape.rot.x -= 0x1000;
    }

    self->actor.world.rot.x = -self->actor.shape.rot.x;
    Math_StepToF(&self->actor.speedXZ, 5.0f, 0.3f);
    Math_ApproachS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 2, 0x71C);
    func_8002F974(&self->actor, NA_SE_EN_EIER_FLY - SFX_FLAG);
}

void EnEiyer_DiveAttack(EnEiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    self->actor.speedXZ *= 1.1f;

    if (self->actor.bgCheckFlags & 8 || self->actor.bgCheckFlags & 1) {
        EnEiyer_SetupLand(self);
    }

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~(AT_ON | AT_HIT);
    }

    func_8002F974(&self->actor, NA_SE_EN_EIER_FLY - SFX_FLAG);
}

void EnEiyer_Land(EnEiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);
    Math_ScaledStepToS(&self->actor.world.rot.x, -0x4000, 0x450);
    Math_StepToF(&self->actor.speedXZ, 7.0f, 1.0f);

    if (self->timer == -1) {
        if (self->actor.bgCheckFlags & 8 || self->actor.bgCheckFlags & 1) {
            self->timer = 10;
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 30, NA_SE_EN_OCTAROCK_SINK);

            if (self->actor.bgCheckFlags & 1) {
                EffectSsGSplash_Spawn(globalCtx, &self->actor.world.pos, NULL, NULL, 1, 700);
            }
        }
    } else {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            self->actor.shape.rot.x = 0;
            self->actor.world.rot.x = 0;
            EnEiyer_SetupAppearFromGround(self);
        }
    }
}

void EnEiyer_Hurt(EnEiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->timer != 0) {
        self->timer--;
    }

    Math_ApproachF(&self->basePos.y, self->actor.floorHeight + 80.0f + 5.0f, 0.5f, self->actor.speedXZ);
    self->actor.world.pos.y = self->basePos.y - 5.0f;

    if (self->actor.bgCheckFlags & 8) {
        self->targetYaw = self->actor.wallYaw;
    } else {
        self->targetYaw = self->actor.yawTowardsPlayer + 0x8000;
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, self->targetYaw, 0x38E);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x200);
    self->actor.shape.rot.z = sinf(self->timer * (M_PI / 5)) * 5120.0f;

    if (self->timer == 0) {
        self->actor.shape.rot.x = 0;
        self->actor.shape.rot.z = 0;
        self->collider.base.acFlags |= AC_ON;
        EnEiyer_SetupGlide(self);
    }
    self->actor.world.rot.x = -self->actor.shape.rot.x;
}

void EnEiyer_Die(EnEiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->actor.speedXZ > 0.0f) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x4000, 0x400);
    } else {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4000, 0x400);
    }

    self->actor.shape.rot.z += 0x1000;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.world.rot.x = -self->actor.shape.rot.x;

    if (self->timer == 0 || self->actor.bgCheckFlags & 0x10) {
        EnEiyer_SetupDead(self);
    }
}

void EnEiyer_Dead(EnEiyer* self, GlobalContext* globalCtx) {
    self->actor.shape.shadowAlpha = CLAMP_MIN((s16)(self->actor.shape.shadowAlpha - 5), 0);
    self->actor.world.pos.y -= 2.0f;

    if (self->actor.shape.shadowAlpha == 0) {
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 80);
        Actor_Kill(&self->actor);
    }
}

void EnEiyer_Stunned(EnEiyer* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x200);
    SkelAnime_Update(&self->skelanime);

    if (Animation_OnFrame(&self->skelanime, 0.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_FLUTTER);
    }

    if (self->actor.bgCheckFlags & 2) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
    }

    if (self->timer == 0) {
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->collider.dim.height = sColCylInit.dim.height;
        EnEiyer_SetupGlide(self);
    }
}

void EnEiyer_UpdateDamage(EnEiyer* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);

        if (self->actor.colChkInfo.damageEffect != 0 || self->actor.colChkInfo.damage != 0) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_DEAD);
                self->actor.flags &= ~1;
            }

            // If underground, one hit kill
            if (self->collider.info.bumper.dmgFlags == 0x19) {
                if (self->actor.colChkInfo.damage == 0) {
                    EnEiyer_SetupAmbush(self, globalCtx);
                } else {
                    EnEiyer_SetupDie(self);
                }
            } else if (self->actor.colChkInfo.damageEffect == 1) {
                if (self->actionFunc != EnEiyer_Stunned) {
                    EnEiyer_SetupStunned(self);
                }
            } else if (self->actor.colChkInfo.health != 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_DAMAGE);
                EnEiyer_SetupHurt(self);
            } else {
                self->collider.dim.height = sColCylInit.dim.height;
                EnEiyer_SetupDie(self);
            }
        }
    }
}

void EnEiyer_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnEiyer* self = THIS;
    s32 pad;

    EnEiyer_UpdateDamage(self, globalCtx);
    self->actionFunc(self, globalCtx);

    if (self->actor.world.rot.x == 0 || self->actionFunc == EnEiyer_Stunned) {
        Actor_MoveForward(&self->actor);
    } else {
        func_8002D97C(&self->actor);
    }

    if (self->actionFunc == EnEiyer_Glide || self->actionFunc == EnEiyer_DiveAttack ||
        self->actionFunc == EnEiyer_Stunned || self->actionFunc == EnEiyer_Die || self->actionFunc == EnEiyer_Hurt ||
        (self->actionFunc == EnEiyer_Land && self->timer == -1)) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 5.0f, 27.0f, 30.0f, 7);
    }

    if (self->actor.params == 0xA ||
        (self->actionFunc != EnEiyer_AppearFromGround && self->actionFunc != EnEiyer_CircleUnderground)) {
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    // only the main Eiyer can ambush the player
    if (self->actor.params == 0 || self->actor.params == 0xA) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        if (self->collider.base.atFlags & AT_ON) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        if (self->actionFunc != EnEiyer_Ambush) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }

    if (self->actor.flags & 1) {
        self->actor.focus.pos.x = self->actor.world.pos.x + Math_SinS(self->actor.shape.rot.y) * 12.5f;
        self->actor.focus.pos.z = self->actor.world.pos.z + Math_CosS(self->actor.shape.rot.y) * 12.5f;
        self->actor.focus.pos.y = self->actor.world.pos.y;
    }
}

s32 EnEiyer_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                             Gfx** gfx) {
    EnEiyer* self = THIS;

    if (limbIndex == 1) {
        pos->z += 2500.0f;
    }

    if (self->collider.info.bumper.dmgFlags == 0x19 && limbIndex != 9 && limbIndex != 10) {
        *dList = NULL;
    }
    return 0;
}

void EnEiyer_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnEiyer* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_eiyer.c", 1494);
    if (self->actionFunc != EnEiyer_Dead) {
        func_80093D18(globalCtx->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, &D_80116280[2]);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);

        POLY_OPA_DISP = SkelAnime_Draw(globalCtx, self->skelanime.skeleton, self->skelanime.jointTable,
                                       EnEiyer_OverrideLimbDraw, NULL, self, POLY_OPA_DISP);
    } else {
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08, D_80116280);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, self->actor.shape.shadowAlpha);

        POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelanime.skeleton, self->skelanime.jointTable,
                                       EnEiyer_OverrideLimbDraw, NULL, self, POLY_XLU_DISP);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_eiyer.c", 1541);
}
