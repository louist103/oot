/*
 * File: z_en_dekunuts.c
 * Overlay: ovl_En_Dekunuts
 * Description: Mad Scrub
 */

#include "z_en_dekunuts.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"
#include "objects/object_dekunuts/object_dekunuts.h"

#define FLAGS 0x00000005

#define THIS ((EnDekunuts*)thisx)

#define DEKUNUTS_FLOWER 10

void EnDekunuts_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDekunuts_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDekunuts_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDekunuts_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDekunuts_SetupWait(EnDekunuts* self);
void EnDekunuts_Wait(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_LookAround(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_Stand(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_ThrowNut(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_Burrow(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_BeginRun(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_Run(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_Gasp(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_BeDamaged(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_BeStunned(EnDekunuts* self, GlobalContext* globalCtx);
void EnDekunuts_Die(EnDekunuts* self, GlobalContext* globalCtx);

const ActorInit En_Dekunuts_InitVars = {
    ACTOR_EN_DEKUNUTS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DEKUNUTS,
    sizeof(EnDekunuts),
    (ActorFunc)EnDekunuts_Init,
    (ActorFunc)EnDekunuts_Destroy,
    (ActorFunc)EnDekunuts_Update,
    (ActorFunc)EnDekunuts_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT6,
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
    { 18, 32, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0x01, 0x0012, 0x0020, MASS_IMMOVABLE };

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
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
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

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x4D, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -1, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2600, ICHAIN_STOP),
};

void EnDekunuts_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDekunuts* self = THIS;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    if (thisx->params == DEKUNUTS_FLOWER) {
        thisx->flags &= ~0x5;
    } else {
        ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
        SkelAnime_Init(globalCtx, &self->skelAnime, &gDekuNutsSkel, &gDekuNutsStandAnim, self->jointTable,
                       self->morphTable, 25);
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        CollisionCheck_SetInfo(&thisx->colChkInfo, &sDamageTable, &sColChkInfoInit);
        self->shotsPerRound = ((thisx->params >> 8) & 0xFF);
        thisx->params &= 0xFF;
        if ((self->shotsPerRound == 0xFF) || (self->shotsPerRound == 0)) {
            self->shotsPerRound = 1;
        }
        EnDekunuts_SetupWait(self);
        Actor_SpawnAsChild(&globalCtx->actorCtx, thisx, globalCtx, ACTOR_EN_DEKUNUTS, thisx->world.pos.x,
                           thisx->world.pos.y, thisx->world.pos.z, 0, thisx->world.rot.y, 0, DEKUNUTS_FLOWER);
    }
}

void EnDekunuts_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDekunuts* self = THIS;

    if (self->actor.params != DEKUNUTS_FLOWER) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void EnDekunuts_SetupWait(EnDekunuts* self) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, &gDekuNutsUpAnim, 0.0f);
    self->animFlagAndTimer = Rand_S16Offset(100, 50);
    self->collider.dim.height = 5;
    Math_Vec3f_Copy(&self->actor.world.pos, &self->actor.home.pos);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnDekunuts_Wait;
}

void EnDekunuts_SetupLookAround(EnDekunuts* self) {
    Animation_PlayLoop(&self->skelAnime, &gDekuNutsLookAroundAnim);
    self->animFlagAndTimer = 2;
    self->actionFunc = EnDekunuts_LookAround;
}

void EnDekunuts_SetupThrowNut(EnDekunuts* self) {
    Animation_PlayOnce(&self->skelAnime, &gDekuNutsSpitAnim);
    self->animFlagAndTimer = self->shotsPerRound;
    self->actionFunc = EnDekunuts_ThrowNut;
}

void EnDekunuts_SetupStand(EnDekunuts* self) {
    Animation_MorphToLoop(&self->skelAnime, &gDekuNutsStandAnim, -3.0f);
    if (self->actionFunc == EnDekunuts_ThrowNut) {
        self->animFlagAndTimer = 2 | 0x1000; // sets timer and flag
    } else {
        self->animFlagAndTimer = 1;
    }
    self->actionFunc = EnDekunuts_Stand;
}

void EnDekunuts_SetupBurrow(EnDekunuts* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gDekuNutsBurrowAnim, -5.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DOWN);
    self->actionFunc = EnDekunuts_Burrow;
}

void EnDekunuts_SetupBeginRun(EnDekunuts* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gDekuNutsUnburrowAnim, -3.0f);
    self->collider.dim.height = 37;
    self->actor.colChkInfo.mass = 0x32;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DAMAGE);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnDekunuts_BeginRun;
}

void EnDekunuts_SetupRun(EnDekunuts* self) {
    Animation_PlayLoop(&self->skelAnime, &gDekuNutsRunAnim);
    self->animFlagAndTimer = 2;
    self->playWalkSound = false;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = EnDekunuts_Run;
}

void EnDekunuts_SetupGasp(EnDekunuts* self) {
    Animation_PlayLoop(&self->skelAnime, &gDekuNutsGaspAnim);
    self->animFlagAndTimer = 3;
    self->actor.speedXZ = 0.0f;
    if (self->runAwayCount != 0) {
        self->runAwayCount--;
    }
    self->actionFunc = EnDekunuts_Gasp;
}

void EnDekunuts_SetupBeDamaged(EnDekunuts* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gDekuNutsDamageAnim, -3.0f);
    if ((self->collider.info.acHitInfo->toucher.dmgFlags & 0x1F824) != 0) {
        self->actor.world.rot.y = self->collider.base.ac->world.rot.y;
    } else {
        self->actor.world.rot.y = Actor_WorldYawTowardActor(&self->actor, self->collider.base.ac) + 0x8000;
    }
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnDekunuts_BeDamaged;
    self->actor.speedXZ = 10.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DAMAGE);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_CUTBODY);
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, Animation_GetLastFrame(&gDekuNutsDamageAnim));
}

void EnDekunuts_SetupBeStunned(EnDekunuts* self) {
    Animation_MorphToLoop(&self->skelAnime, &gDekuNutsDamageAnim, -3.0f);
    self->animFlagAndTimer = 5;
    self->actionFunc = EnDekunuts_BeStunned;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0,
                         Animation_GetLastFrame(&gDekuNutsDamageAnim) * self->animFlagAndTimer);
}

void EnDekunuts_SetupDie(EnDekunuts* self) {
    Animation_PlayOnce(&self->skelAnime, &gDekuNutsDieAnim);
    self->actionFunc = EnDekunuts_Die;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DEAD);
}

void EnDekunuts_Wait(EnDekunuts* self, GlobalContext* globalCtx) {
    s32 hasSlowPlaybackSpeed = false;

    if (self->skelAnime.playSpeed < 0.5f) {
        hasSlowPlaybackSpeed = true;
    }
    if (hasSlowPlaybackSpeed && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (Animation_OnFrame(&self->skelAnime, 9.0f)) {
        self->collider.base.acFlags |= AC_ON;
    } else if (Animation_OnFrame(&self->skelAnime, 8.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
    }

    self->collider.dim.height = ((CLAMP(self->skelAnime.curFrame, 9.0f, 12.0f) - 9.0f) * 9.0f) + 5.0f;
    if (!hasSlowPlaybackSpeed && (self->actor.xzDistToPlayer < 120.0f)) {
        EnDekunuts_SetupBurrow(self);
    } else if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.xzDistToPlayer < 120.0f) {
            EnDekunuts_SetupBurrow(self);
        } else if ((self->animFlagAndTimer == 0) && (self->actor.xzDistToPlayer > 320.0f)) {
            EnDekunuts_SetupLookAround(self);
        } else {
            EnDekunuts_SetupStand(self);
        }
    }
    if (hasSlowPlaybackSpeed &&
        ((self->actor.xzDistToPlayer > 160.0f) && (fabsf(self->actor.yDistToPlayer) < 120.0f)) &&
        ((self->animFlagAndTimer == 0) || (self->actor.xzDistToPlayer < 480.0f))) {
        self->skelAnime.playSpeed = 1.0f;
    }
}

void EnDekunuts_LookAround(EnDekunuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if ((self->actor.xzDistToPlayer < 120.0f) || (self->animFlagAndTimer == 0)) {
        EnDekunuts_SetupBurrow(self);
    }
}

void EnDekunuts_Stand(EnDekunuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (!(self->animFlagAndTimer & 0x1000)) {
        Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    }
    if (self->animFlagAndTimer == 0x1000) {
        if ((self->actor.xzDistToPlayer > 480.0f) || (self->actor.xzDistToPlayer < 120.0f)) {
            EnDekunuts_SetupBurrow(self);
        } else {
            EnDekunuts_SetupThrowNut(self);
        }
    } else if (self->animFlagAndTimer == 0) {
        EnDekunuts_SetupThrowNut(self);
    }
}

void EnDekunuts_ThrowNut(EnDekunuts* self, GlobalContext* globalCtx) {
    Vec3f spawnPos;

    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    if (SkelAnime_Update(&self->skelAnime)) {
        EnDekunuts_SetupStand(self);
    } else if (Animation_OnFrame(&self->skelAnime, 6.0f)) {
        spawnPos.x = self->actor.world.pos.x + (Math_SinS(self->actor.shape.rot.y) * 23.0f);
        spawnPos.y = self->actor.world.pos.y + 12.0f;
        spawnPos.z = self->actor.world.pos.z + (Math_CosS(self->actor.shape.rot.y) * 23.0f);
        if (Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_NUTSBALL, spawnPos.x, spawnPos.y, spawnPos.z,
                        self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z, 0) != NULL) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_THROW);
        }
    } else if ((self->animFlagAndTimer > 1) && Animation_OnFrame(&self->skelAnime, 12.0f)) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gDekuNutsSpitAnim, -3.0f);
        if (self->animFlagAndTimer != 0) {
            self->animFlagAndTimer--;
        }
    }
}

void EnDekunuts_Burrow(EnDekunuts* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnDekunuts_SetupWait(self);
    } else {
        self->collider.dim.height = ((3.0f - CLAMP(self->skelAnime.curFrame, 1.0f, 3.0f)) * 12.0f) + 5.0f;
    }
    if (Animation_OnFrame(&self->skelAnime, 4.0f)) {
        self->collider.base.acFlags &= ~AC_ON;
    }
    Math_ApproachF(&self->actor.world.pos.x, self->actor.home.pos.x, 0.5f, 3.0f);
    Math_ApproachF(&self->actor.world.pos.z, self->actor.home.pos.z, 0.5f, 3.0f);
}

void EnDekunuts_BeginRun(EnDekunuts* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->runDirection = self->actor.yawTowardsPlayer + 0x8000;
        self->runAwayCount = 3;
        EnDekunuts_SetupRun(self);
    }
    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
}

void EnDekunuts_Run(EnDekunuts* self, GlobalContext* globalCtx) {
    s16 diffRotInit;
    s16 diffRot;
    f32 phi_f0;

    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (self->playWalkSound) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
        self->playWalkSound = false;
    } else {
        self->playWalkSound = true;
    }

    Math_StepToF(&self->actor.speedXZ, 7.5f, 1.0f);
    if (Math_SmoothStepToS(&self->actor.world.rot.y, self->runDirection, 1, 0xE38, 0xB6) == 0) {
        if (self->actor.bgCheckFlags & 0x20) {
            self->runDirection = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
        } else if (self->actor.bgCheckFlags & 8) {
            self->runDirection = self->actor.wallYaw;
        } else if (self->runAwayCount == 0) {
            diffRotInit = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
            diffRot = diffRotInit - self->actor.yawTowardsPlayer;
            if (ABS(diffRot) > 0x2000) {
                self->runDirection = diffRotInit;
            } else {
                phi_f0 = (diffRot >= 0.0f) ? 1.0f : -1.0f;
                self->runDirection = (phi_f0 * -0x2000) + self->actor.yawTowardsPlayer;
            }
        } else {
            self->runDirection = self->actor.yawTowardsPlayer + 0x8000;
        }
    }

    self->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
    if ((self->runAwayCount == 0) && Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) < 20.0f &&
        fabsf(self->actor.world.pos.y - self->actor.home.pos.y) < 2.0f) {
        self->actor.colChkInfo.mass = MASS_IMMOVABLE;
        self->actor.speedXZ = 0.0f;
        EnDekunuts_SetupBurrow(self);
    } else if (self->animFlagAndTimer == 0) {
        EnDekunuts_SetupGasp(self);
    }
}

void EnDekunuts_Gasp(EnDekunuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (self->animFlagAndTimer == 0) {
        EnDekunuts_SetupRun(self);
    }
}

void EnDekunuts_BeDamaged(EnDekunuts* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->actor.speedXZ, 0.0f, 1.0f);
    if (SkelAnime_Update(&self->skelAnime)) {
        EnDekunuts_SetupDie(self);
    }
}

void EnDekunuts_BeStunned(EnDekunuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
        if (self->animFlagAndTimer != 0) {
            self->animFlagAndTimer--;
        }
        if (self->animFlagAndTimer == 0) {
            EnDekunuts_SetupRun(self);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_FAINT);
        }
    }
}

void EnDekunuts_Die(EnDekunuts* self, GlobalContext* globalCtx) {
    static Vec3f effectVelAndAccel = { 0.0f, 0.0f, 0.0f };

    s32 pad;
    Vec3f effectPos;

    if (SkelAnime_Update(&self->skelAnime)) {
        effectPos.x = self->actor.world.pos.x;
        effectPos.y = self->actor.world.pos.y + 18.0f;
        effectPos.z = self->actor.world.pos.z;
        EffectSsDeadDb_Spawn(globalCtx, &effectPos, &effectVelAndAccel, &effectVelAndAccel, 200, 0, 255, 255, 255, 255,
                             150, 150, 150, 1, 13, 1);
        effectPos.y = self->actor.world.pos.y + 10.0f;
        EffectSsHahen_SpawnBurst(globalCtx, &effectPos, 3.0f, 0, 12, 3, 15, HAHEN_OBJECT_DEFAULT, 10, NULL);
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x30);
        if (self->actor.child != NULL) {
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, self->actor.child, ACTORCAT_PROP);
        }
        Actor_Kill(&self->actor);
    }
}

void EnDekunuts_ColliderCheck(EnDekunuts* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);
        if (self->actor.colChkInfo.mass == 0x32) {
            if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
                if (self->actor.colChkInfo.damageEffect != 1) {
                    if (self->actor.colChkInfo.damageEffect == 2) {
                        EffectSsFCircle_Spawn(globalCtx, &self->actor, &self->actor.world.pos, 40, 50);
                    }
                    EnDekunuts_SetupBeDamaged(self);
                    if (Actor_ApplyDamage(&self->actor) == 0) {
                        Enemy_StartFinishingBlow(globalCtx, &self->actor);
                    }
                } else if (self->actionFunc != EnDekunuts_BeStunned) {
                    EnDekunuts_SetupBeStunned(self);
                }
            }
        } else {
            EnDekunuts_SetupBeginRun(self);
        }
    } else if ((self->actor.colChkInfo.mass == MASS_IMMOVABLE) && (globalCtx->actorCtx.unk_02 != 0)) {
        EnDekunuts_SetupBeginRun(self);
    }
}

void EnDekunuts_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDekunuts* self = THIS;
    s32 pad;

    if (self->actor.params != DEKUNUTS_FLOWER) {
        EnDekunuts_ColliderCheck(self, globalCtx);
        self->actionFunc(self, globalCtx);
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, self->collider.dim.radius, self->collider.dim.height,
                                0x1D);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        if (self->actionFunc == EnDekunuts_Wait) {
            Actor_SetFocus(&self->actor, self->skelAnime.curFrame);
        } else if (self->actionFunc == EnDekunuts_Burrow) {
            Actor_SetFocus(&self->actor,
                           20.0f - ((self->skelAnime.curFrame * 20.0f) / Animation_GetLastFrame(&gDekuNutsBurrowAnim)));
        } else {
            Actor_SetFocus(&self->actor, 20.0f);
        }
    }
}

s32 EnDekunuts_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx) {
    EnDekunuts* self = THIS;
    f32 x;
    f32 y;
    f32 z;
    f32 curFrame;

    if ((limbIndex == 7) && (self->actionFunc == EnDekunuts_ThrowNut)) {
        curFrame = self->skelAnime.curFrame;
        if (curFrame <= 6.0f) {
            x = 1.0f - (curFrame * 0.0833f);
            z = 1.0f + (curFrame * 0.1167f);
            y = 1.0f + (curFrame * 0.1167f);
        } else if (curFrame <= 7.0f) {
            curFrame -= 6.0f;
            x = 0.5f + curFrame;
            z = 1.7f - (curFrame * 0.7f);
            y = 1.7f - (curFrame * 0.7f);
        } else if (curFrame <= 10.0f) {
            x = 1.5f - ((curFrame - 7.0f) * 0.1667f);
            z = 1.0f;
            y = 1.0f;
        } else {
            return false;
        }
        Matrix_Scale(x, y, z, MTXMODE_APPLY);
    }
    return false;
}

void EnDekunuts_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDekunuts* self = THIS;

    if (self->actor.params == DEKUNUTS_FLOWER) {
        Gfx_DrawDListOpa(globalCtx, gDekuNutsFlowerDL);
    } else {
        SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnDekunuts_OverrideLimbDraw,
                          NULL, self);
    }
}
