/*
 * File: z_en_fd.c
 * Overlay: ovl_En_Fd
 * Description: Flare Dancer (enflamed form)
 */

#include "z_en_fd.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_fw/object_fw.h"

#define FLAGS 0x00000215

#define THIS ((EnFd*)thisx)

#define FLG_COREDEAD (0x4000)
#define FLG_COREDONE (0x8000)

void EnFd_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFd_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFd_Draw(Actor* thisx, GlobalContext* globalCtx);
void EnFd_Run(EnFd* self, GlobalContext* globalCtx);
void EnFd_SpinAndSpawnFire(EnFd* self, GlobalContext* globalCtx);
void EnFd_Reappear(EnFd* self, GlobalContext* globalCtx);
void EnFd_SpinAndGrow(EnFd* self, GlobalContext* globalCtx);
void EnFd_JumpToGround(EnFd* self, GlobalContext* globalCtx);
void EnFd_WaitForCore(EnFd* self, GlobalContext* globalCtx);
void EnFd_UpdateFlames(EnFd* self);
void EnFd_UpdateDots(EnFd* self);
void EnFd_AddEffect(EnFd*, u8, Vec3f*, Vec3f*, Vec3f*, u8, f32, f32);
void EnFd_DrawDots(EnFd* self, GlobalContext* globalCtx);
void EnFd_DrawFlames(EnFd* self, GlobalContext* globalCtx);
void EnFd_Land(EnFd* self, GlobalContext* globalCtx);

const ActorInit En_Fd_InitVars = {
    ACTOR_EN_FD,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_FW,
    sizeof(EnFd),
    (ActorFunc)EnFd_Init,
    (ActorFunc)EnFd_Destroy,
    (ActorFunc)EnFd_Update,
    (ActorFunc)EnFd_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[12] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040088, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { 21, { { 1600, 0, 0 }, 5 }, 300 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 12, { { 1600, 0, 0 }, 5 }, 400 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 14, { { 800, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 15, { { 1600, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 16, { { 2000, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 17, { { 800, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 18, { { 1600, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 19, { { 2000, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 4, { { 2200, 0, 0 }, 4 }, 400 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 4, { { 5000, 0, 0 }, 4 }, 300 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 8, { { 2200, 0, 0 }, 4 }, 400 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x01, 0x04 },
            { 0x00040008, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 8, { { 5000, 0, 0 }, 4 }, 300 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    12,
    sJntSphElementsInit,
};

static CollisionCheckInfoInit2 sColChkInit = { 24, 2, 25, 25, MASS_IMMOVABLE };

static struct_80034EC0_Entry sAnimations[] = {
    { &gFlareDancerCastingFireAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, 0.0f },
    { &gFlareDancerBackflipAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -10.0f },
    { &gFlareDancerGettingUpAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -10.0f },
    { &gFlareDancerChasingAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -10.0f },
    { &gFlareDancerTwirlAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -10.0f },
};

s32 EnFd_SpawnCore(EnFd* self, GlobalContext* globalCtx) {
    if (self->invincibilityTimer != 0) {
        return false;
    }

    if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FW, self->corePos.x, self->corePos.y,
                           self->corePos.z, 0, self->actor.shape.rot.y, 0, self->runDir) == NULL) {
        return false;
    }

    self->actor.child->colChkInfo.health = self->actor.colChkInfo.health % 8;

    if (self->actor.child->colChkInfo.health == 0) {
        self->actor.child->colChkInfo.health = 8;
    }

    if ((self->actor.flags & 0x2000) == 0x2000) {
        func_8002DE04(globalCtx, &self->actor, self->actor.child);
    }

    self->coreActive = true;

    return true;
}

void EnFd_SpawnChildFire(EnFd* self, GlobalContext* globalCtx, s16 fireCnt, s16 color) {
    s32 i;

    for (i = 0; i < fireCnt; i++) {
        s16 angle = (s16)((((i * 360.0f) / fireCnt) * (0x10000 / 360.0f))) + self->actor.yawTowardsPlayer;
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_FD_FIRE, self->actor.world.pos.x,
                           self->actor.world.pos.y, self->actor.world.pos.z, 0, angle, 0, (color << 0xF) | i);
    }
}

void EnFd_SpawnDot(EnFd* self, GlobalContext* globalCtx) {
    Vec3f pos = { 0.0f, 0.0f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };

    if (self->actionFunc == EnFd_Run || self->actionFunc == EnFd_SpinAndSpawnFire) {
        pos.x = self->actor.world.pos.x;
        pos.y = self->actor.floorHeight + 4.0f;
        pos.z = self->actor.world.pos.z;
        accel.x = (Rand_ZeroOne() - 0.5f) * 2.0f;
        accel.y = ((Rand_ZeroOne() - 0.5f) * 0.2f) + 0.3f;
        accel.z = (Rand_ZeroOne() - 0.5f) * 2.0f;
        EnFd_AddEffect(self, FD_EFFECT_FLAME, &pos, &velocity, &accel, 8, 0.6f, 0.2f);
    }
}

/**
 * Checks to see if the hammer effect is active, and if it should be applied
 */
s32 EnFd_CheckHammer(EnFd* self, GlobalContext* globalCtx) {
    if (self->actionFunc == EnFd_Reappear || self->actionFunc == EnFd_SpinAndGrow ||
        self->actionFunc == EnFd_JumpToGround || self->actionFunc == EnFd_WaitForCore) {
        return false;
    } else if (globalCtx->actorCtx.unk_02 != 0 && self->actor.xzDistToPlayer < 300.0f &&
               self->actor.yDistToPlayer < 60.0f) {
        return true;
    } else {
        return false;
    }
}

s32 EnFd_ColliderCheck(EnFd* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    ColliderInfo* info;

    if (self->collider.base.acFlags & AC_HIT || EnFd_CheckHammer(self, globalCtx)) {
        self->collider.base.acFlags &= ~AC_HIT;
        if (self->invincibilityTimer != 0) {
            return false;
        }
        info = &self->collider.elements[0].info;
        if (info->acHitInfo != NULL && (info->acHitInfo->toucher.dmgFlags & 0x80)) {
            return false;
        }

        if (!EnFd_SpawnCore(self, globalCtx)) {
            return false;
        }
        self->invincibilityTimer = 30;
        self->actor.flags &= ~1;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_DAMAGE);
        Enemy_StartFinishingBlow(globalCtx, &self->actor);
        return true;
    } else if (DECR(self->attackTimer) == 0 && self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        if (self->invincibilityTimer != 0) {
            return false;
        }

        if (self->collider.base.atFlags & AT_BOUNCED) {
            return false;
        }
        self->attackTimer = 30;
        Audio_PlayActorSound2(&player->actor, NA_SE_PL_BODY_HIT);
        func_8002F71C(globalCtx, &self->actor, self->actor.speedXZ + 2.0f, self->actor.yawTowardsPlayer, 6.0f);
    }
    return false;
}

/**
 * Determines if `actor` is within an acceptable range for `self` to be able to "see" `actor`
 * `actor` must be within 400 units of `self`, `actor` must be within +/- 40 degrees facing angle
 * towards `actor`, and there must not be a collision poly between `self` and `actor`
 */
s32 EnFd_CanSeeActor(EnFd* self, Actor* actor, GlobalContext* globalCtx) {
    CollisionPoly* colPoly;
    s32 bgId;
    Vec3f colPoint;
    s16 angle;
    s32 pad;

    // Check to see if `actor` is within 400 units of `self`
    if (Math_Vec3f_DistXYZ(&self->actor.world.pos, &actor->world.pos) > 400.0f) {
        return false;
    }

    // Check to see if the angle between self facing angle and `actor` is withing ~40 degrees
    angle = (f32)Math_Vec3f_Yaw(&self->actor.world.pos, &actor->world.pos) - self->actor.shape.rot.y;
    if (ABS(angle) > 0x1C70) {
        return false;
    }

    // check to see if the line between `self` and `actor` does not intersect a collision poly
    if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &self->actor.world.pos, &actor->world.pos, &colPoint, &colPoly,
                                true, false, false, true, &bgId)) {
        return false;
    }

    return true;
}

Actor* EnFd_FindBomb(EnFd* self, GlobalContext* globalCtx) {
    Actor* actor = globalCtx->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].head;

    while (actor != NULL) {
        if (actor->params != 0 || actor->parent != NULL) {
            actor = actor->next;
            continue;
        }

        if (actor->id != ACTOR_EN_BOM) {
            actor = actor->next;
            continue;
        }

        if (EnFd_CanSeeActor(self, actor, globalCtx) != 1) {
            actor = actor->next;
            continue;
        }

        return actor;
    }
    return NULL;
}

Actor* EnFd_FindPotentialTheat(EnFd* self, GlobalContext* globalCtx) {
    Player* player;
    Actor* bomb = EnFd_FindBomb(self, globalCtx);

    if (bomb != NULL) {
        return bomb;
    }

    if (self->attackTimer != 0) {
        return NULL;
    }

    player = GET_PLAYER(globalCtx);
    if (!EnFd_CanSeeActor(self, &player->actor, globalCtx)) {
        return NULL;
    }

    return &player->actor;
}

/**
 * Creates a delta in `dst` for the position from `self`'s current position to the next
 * position in a circle formed by `radius` with center at `self`'s initial position.
 */
Vec3f* EnFd_GetPosAdjAroundCircle(Vec3f* dst, EnFd* self, f32 radius, s16 dir) {
    s16 angle;
    Vec3f newPos;

    angle = Math_Vec3f_Yaw(&self->actor.home.pos, &self->actor.world.pos) + (dir * 0x1554); // ~30 degrees
    newPos.x = (Math_SinS(angle) * radius) + self->actor.home.pos.x;
    newPos.z = (Math_CosS(angle) * radius) + self->actor.home.pos.z;
    newPos.x -= self->actor.world.pos.x;
    newPos.z -= self->actor.world.pos.z;
    *dst = newPos;
    return dst;
}

s32 EnFd_ShouldStopRunning(EnFd* self, GlobalContext* globalCtx, f32 radius, s16* runDir) {
    CollisionPoly* poly;
    s32 bgId;
    Vec3f colPoint;
    Vec3f pos;

    // Check to see if the next position on the rotation around the circle
    // will result in a background collision
    EnFd_GetPosAdjAroundCircle(&pos, self, radius, *runDir);

    pos.x += self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y;
    pos.z += self->actor.world.pos.z;

    if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &self->actor.world.pos, &pos, &colPoint, &poly, true, false, false,
                                true, &bgId)) {
        *runDir = -*runDir;
        return true;
    }

    if (self->circlesToComplete != 0 || DECR(self->spinTimer) != 0) {
        return false;
    }

    if (Rand_ZeroOne() > 0.5f) {
        *runDir = -*runDir;
    }
    return true;
}

void EnFd_Fade(EnFd* self, GlobalContext* globalCtx) {
    if (self->invincibilityTimer != 0) {
        Math_SmoothStepToF(&self->fadeAlpha, 0.0f, 0.3f, 10.0f, 0.0f);
        self->actor.shape.shadowAlpha = self->fadeAlpha;
        if (!(self->fadeAlpha >= 0.9f)) {
            self->invincibilityTimer = 0;
            self->spinTimer = 0;
            self->actionFunc = EnFd_WaitForCore;
            self->actor.speedXZ = 0.0f;
        }
    }
}

void EnFd_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFd* self = THIS;

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gFlareDancerSkel, NULL, self->jointTable, self->morphTable, 27);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 32.0f);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colSphs);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(0xF), &sColChkInit);
    self->actor.flags &= ~1;
    self->actor.flags |= 0x1000000;
    Actor_SetScale(&self->actor, 0.01f);
    self->firstUpdateFlag = true;
    self->actor.gravity = -1.0f;
    self->runDir = Rand_ZeroOne() < 0.5f ? -1 : 1;
    self->actor.naviEnemyId = 0x22;
    self->actionFunc = EnFd_Reappear;
}

void EnFd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFd* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnFd_Reappear(EnFd* self, GlobalContext* globalCtx) {
    self->actor.world.pos = self->actor.home.pos;
    self->actor.params = 0;
    self->actor.shape.shadowAlpha = 0xFF;
    self->coreActive = false;
    self->actor.scale.y = 0.0f;
    self->fadeAlpha = 255.0f;
    func_80034EC0(&self->skelAnime, sAnimations, 0);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_LAUGH);
    self->actionFunc = EnFd_SpinAndGrow;
}

void EnFd_SpinAndGrow(EnFd* self, GlobalContext* globalCtx) {
    if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
        self->actor.velocity.y = 6.0f;
        self->actor.scale.y = 0.01f;
        self->actor.world.rot.y ^= 0x8000;
        self->actor.flags |= 1;
        self->actor.speedXZ = 8.0f;
        func_80034EC0(&self->skelAnime, sAnimations, 1);
        self->actionFunc = EnFd_JumpToGround;
    } else {
        self->actor.scale.y = self->skelAnime.curFrame * (0.01f / self->skelAnime.animLength);
        self->actor.shape.rot.y += 0x2000;
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
}

void EnFd_JumpToGround(EnFd* self, GlobalContext* globalCtx) {
    if ((self->actor.bgCheckFlags & 1) && !(self->actor.velocity.y > 0.0f)) {
        self->actor.velocity.y = 0.0f;
        self->actor.speedXZ = 0.0f;
        self->actor.world.rot.y = self->actor.shape.rot.y;
        func_80034EC0(&self->skelAnime, sAnimations, 2);
        self->actionFunc = EnFd_Land;
    }
}

void EnFd_Land(EnFd* self, GlobalContext* globalCtx) {
    Vec3f adjPos;

    Math_SmoothStepToF(&self->skelAnime.playSpeed, 1.0f, 0.1f, 1.0f, 0.0f);
    if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
        self->spinTimer = Rand_S16Offset(60, 90);
        self->runRadius = Math_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.home.pos);
        EnFd_GetPosAdjAroundCircle(&adjPos, self, self->runRadius, self->runDir);
        self->actor.world.rot.y = Math_FAtan2F(adjPos.x, adjPos.z) * (0x8000 / M_PI);
        func_80034EC0(&self->skelAnime, sAnimations, 4);
        self->actionFunc = EnFd_SpinAndSpawnFire;
    }
}

void EnFd_SpinAndSpawnFire(EnFd* self, GlobalContext* globalCtx) {
    f32 deceleration;
    f32 tgtSpeed;
    f32 rotSpeed;

    if ((self->spinTimer < 31) && (self->invincibilityTimer == 0)) {
        func_8002F974(&self->actor, NA_SE_EN_FLAME_FIRE_ATTACK - SFX_FLAG);
    } else {
        func_8002F974(&self->actor, NA_SE_EN_FLAME_ROLL - SFX_FLAG);
    }

    if (DECR(self->spinTimer) != 0) {
        self->actor.shape.rot.y += (self->runDir * 0x2000);
        if (self->spinTimer == 30 && self->invincibilityTimer == 0) {
            if (self->actor.xzDistToPlayer > 160.0f) {
                // orange flames
                EnFd_SpawnChildFire(self, globalCtx, 8, 0);
            } else {
                // blue flames
                EnFd_SpawnChildFire(self, globalCtx, 8, 1);
            }
        }
    } else {
        // slow shape rotation down to meet `self` rotation within ~1.66 degrees
        deceleration = self->actor.world.rot.y;
        deceleration -= self->actor.shape.rot.y;
        rotSpeed = 0.0f;
        tgtSpeed = fabsf(deceleration);
        deceleration /= tgtSpeed;
        Math_ApproachF(&rotSpeed, tgtSpeed, 0.6f, 0x2000);
        rotSpeed *= deceleration;
        self->actor.shape.rot.y += (s16)rotSpeed;
        rotSpeed = fabsf(rotSpeed);
        if ((s32)rotSpeed <= 300) {
            // ~1.6 degrees
            self->actor.shape.rot.y = self->actor.world.rot.y;
        }

        if (self->actor.shape.rot.y == self->actor.world.rot.y) {
            self->initYawToInitPos = Math_Vec3f_Yaw(&self->actor.home.pos, &self->actor.world.pos);
            self->curYawToInitPos = self->runDir < 0 ? 0xFFFF : 0;
            self->circlesToComplete = (globalCtx->state.frames & 7) + 2;
            self->spinTimer = Rand_S16Offset(30, 120);
            func_80034EC0(&self->skelAnime, sAnimations, 3);
            self->actionFunc = EnFd_Run;
        }
    }
}

/**
 * Run around in a circle with the center being the initial position, and
 * the radius being the distance from the initial postion to the nearest
 * threat (bomb or player).
 */
void EnFd_Run(EnFd* self, GlobalContext* globalCtx) {
    Actor* potentialThreat;
    s16 yawToYawTarget;
    f32 runRadiusTarget;
    Vec3f adjPos;

    if (EnFd_ShouldStopRunning(self, globalCtx, self->runRadius, &self->runDir)) {
        if (self->invincibilityTimer == 0) {
            self->actor.world.rot.y ^= 0x8000;
            self->actor.velocity.y = 6.0f;
            self->actor.speedXZ = 0.0f;
            func_80034EC0(&self->skelAnime, sAnimations, 1);
            self->actionFunc = EnFd_JumpToGround;
            return;
        }
    }

    yawToYawTarget = Math_Vec3f_Yaw(&self->actor.home.pos, &self->actor.world.pos) - self->initYawToInitPos;
    if (self->runDir > 0) {
        if ((u16)self->curYawToInitPos > (u16)(yawToYawTarget)) {
            self->circlesToComplete--;
        }
    } else if ((u16)self->curYawToInitPos < (u16)(yawToYawTarget)) {
        self->circlesToComplete--;
    }

    if (self->circlesToComplete < 0) {
        self->circlesToComplete = 0;
    }
    self->curYawToInitPos = yawToYawTarget;

    // If there is a bomb out, or if the player exists, set radius to
    // the distance to that threat, otherwise default to 200.
    potentialThreat = EnFd_FindPotentialTheat(self, globalCtx);
    if ((potentialThreat != NULL) && (self->invincibilityTimer == 0)) {
        runRadiusTarget = Math_Vec3f_DistXYZ(&self->actor.home.pos, &potentialThreat->world.pos);
    } else {
        runRadiusTarget = 200.0f;
    }
    Math_SmoothStepToF(&self->runRadius, runRadiusTarget, 0.3f, 100.0f, 0.0f);
    EnFd_GetPosAdjAroundCircle(&adjPos, self, self->runRadius, self->runDir);
    Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(adjPos.x, adjPos.z) * (0x8000 / M_PI), 4, 0xFA0, 1);
    self->actor.world.rot = self->actor.shape.rot;
    func_8002F974(&self->actor, NA_SE_EN_FLAME_RUN - SFX_FLAG);
    if (self->skelAnime.curFrame == 6.0f || self->skelAnime.curFrame == 13.0f || self->skelAnime.curFrame == 28.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_KICK);
    }
    Math_SmoothStepToF(&self->actor.speedXZ, 8.0f, 0.1f, 1.0f, 0.0f);
}

/**
 * En_Fw will set `self` params when it is done with its action.
 * It will set FLG_COREDONE when the core has returned to `self`'s initial
 * position, and FLG_COREDEAD when there is no health left
 */
void EnFd_WaitForCore(EnFd* self, GlobalContext* globalCtx) {
    if (self->spinTimer != 0) {
        self->spinTimer--;
        if (self->spinTimer == 0) {
            Actor_Kill(&self->actor);
        }
    } else if (self->actor.params & FLG_COREDONE) {
        self->actionFunc = EnFd_Reappear;
    } else if (self->actor.params & FLG_COREDEAD) {
        self->actor.params = 0;
        self->spinTimer = 30;
    }
}

void EnFd_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnFd* self = THIS;
    s32 pad;

    if (self->firstUpdateFlag) {
        func_800F5ACC(0x38);
        self->firstUpdateFlag = false;
    }

    if (self->actionFunc != EnFd_Reappear) {
        SkelAnime_Update(&self->skelAnime);
        EnFd_SpawnDot(self, globalCtx);
    }

    if ((self->actor.flags & 0x2000) == 0x2000) {
        // has been hookshoted
        if (EnFd_SpawnCore(self, globalCtx)) {
            self->actor.flags &= ~1;
            self->invincibilityTimer = 30;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_DAMAGE);
            Enemy_StartFinishingBlow(globalCtx, &self->actor);
        } else {
            self->actor.flags &= ~0x2000;
        }
    } else if (self->actionFunc != EnFd_WaitForCore) {
        EnFd_ColliderCheck(self, globalCtx);
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    EnFd_Fade(self, globalCtx);
    self->actionFunc(self, globalCtx);
    EnFd_UpdateDots(self);
    EnFd_UpdateFlames(self);
    if (self->actionFunc != EnFd_Reappear && self->actionFunc != EnFd_SpinAndGrow &&
        self->actionFunc != EnFd_WaitForCore) {
        if (self->attackTimer == 0 && self->invincibilityTimer == 0) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }

        if ((self->actionFunc == EnFd_Run) || (self->actionFunc == EnFd_SpinAndSpawnFire)) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

s32 EnFd_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                          Gfx** gfxP) {
    EnFd* self = THIS;

    if (self->invincibilityTimer != 0) {
        switch (limbIndex) {
            case 13:
            case 21:
                *dList = NULL;
                break;
        }
    }

    return false;
}

void EnFd_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfxP) {
    EnFd* self = THIS;
    Vec3f unused0 = { 6800.0f, 0.0f, 0.0f };
    Vec3f unused1 = { 6800.0f, 0.0f, 0.0f };
    Vec3f initialPos = { 0.0f, 0.0f, 0.0f };
    Vec3f pos = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    s32 i;

    if (limbIndex == 21) {
        Matrix_MultVec3f(&initialPos, &self->corePos);
    }

    if (limbIndex == 13) {
        Matrix_MultVec3f(&initialPos, &self->actor.focus.pos);
    }

    if (limbIndex == 3 || limbIndex == 6 || limbIndex == 7 || limbIndex == 10 || limbIndex == 14 || limbIndex == 15 ||
        limbIndex == 17 || limbIndex == 18 || limbIndex == 20 || limbIndex == 22 || limbIndex == 23 ||
        limbIndex == 24 || limbIndex == 25 || limbIndex == 26) {
        if ((globalCtx->state.frames % 2) != 0) {
            for (i = 0; i < 1; i++) {
                Matrix_MultVec3f(&initialPos, &pos);
                pos.x += (Rand_ZeroOne() - 0.5f) * 20.0f;
                pos.y += (Rand_ZeroOne() - 0.5f) * 40.0f;
                pos.z += (Rand_ZeroOne() - 0.5f) * 20.0f;
                accel.x = (Rand_ZeroOne() - 0.5f) * 0.4f;
                accel.y = ((Rand_ZeroOne() - 0.5f) * 0.2f) + 0.6f;
                accel.z = (Rand_ZeroOne() - 0.5f) * 0.4f;
                EnFd_AddEffect(self, FD_EFFECT_DOT, &pos, &velocity, &accel, 0, 0.006f, 0.0f);
            }
        }
    }

    Collider_UpdateSpheres(limbIndex, &self->collider);
}

void EnFd_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnFd* self = THIS;
    s32 clampedHealth;
    Color_RGBA8 primColors[] = {
        { 255, 255, 200, 255 },
        { 200, 200, 200, 255 },
        { 255, 255, 0, 255 },
    };
    Color_RGBA8 envColors[] = {
        { 0, 255, 0, 255 },
        { 0, 0, 255, 255 },
        { 255, 0, 0, 255 },
    };
    u32 frames;
    s32 pad;

    frames = globalCtx->state.frames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fd.c", 1751);

    Matrix_Push();
    EnFd_DrawDots(self, globalCtx);
    EnFd_DrawFlames(self, globalCtx);
    Matrix_Pop();
    if (self->actionFunc != EnFd_Reappear && !(self->fadeAlpha < 0.9f)) {
        if (1) {}
        func_80093D84(globalCtx->state.gfxCtx);
        clampedHealth = CLAMP(thisx->colChkInfo.health - 1, 0, 23);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 128, primColors[clampedHealth / 8].r, primColors[clampedHealth / 8].g,
                        primColors[clampedHealth / 8].b, (u8)self->fadeAlpha);
        gDPSetEnvColor(POLY_XLU_DISP++, envColors[clampedHealth / 8].r, envColors[clampedHealth / 8].g,
                       envColors[clampedHealth / 8].b, (u8)self->fadeAlpha);
        gSPSegment(
            POLY_XLU_DISP++, 0x8,
            Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0, 0xFF - (u8)(frames * 6), 8, 0x40));
        gDPPipeSync(POLY_XLU_DISP++);
        gSPSegment(POLY_XLU_DISP++, 0x9, D_80116280);

        POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           self->skelAnime.dListCount, EnFd_OverrideLimbDraw, EnFd_PostLimbDraw, self,
                                           POLY_XLU_DISP);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fd.c", 1822);
}

void EnFd_AddEffect(EnFd* self, u8 type, Vec3f* pos, Vec3f* velocity, Vec3f* accel, u8 timer, f32 scale,
                    f32 scaleStep) {
    EnFdEffect* eff = self->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(self->effects); i++, eff++) {
        if (eff->type != FD_EFFECT_NONE) {
            continue;
        }
        eff->scale = scale;
        eff->scaleStep = scaleStep;
        eff->initialTimer = eff->timer = timer;
        eff->type = type;
        eff->pos = *pos;
        eff->accel = *accel;
        eff->velocity = *velocity;
        if (eff->type == FD_EFFECT_DOT) {
            eff->color.a = 255;
            eff->timer = (s16)(Rand_ZeroOne() * 10.0f);
        }
        return;
    }
}

void EnFd_UpdateFlames(EnFd* self) {
    s16 i;
    EnFdEffect* eff = self->effects;

    for (i = 0; i < ARRAY_COUNT(self->effects); i++, eff++) {
        if (eff->type == FD_EFFECT_FLAME) {
            eff->timer--;
            if (eff->timer == 0) {
                eff->type = FD_EFFECT_NONE;
            }
            eff->accel.x = (Rand_ZeroOne() * 0.4f) - 0.2f;
            eff->accel.z = (Rand_ZeroOne() * 0.4f) - 0.2f;
            eff->pos.x += eff->velocity.x;
            eff->pos.y += eff->velocity.y;
            eff->pos.z += eff->velocity.z;
            eff->velocity.x += eff->accel.x;
            eff->velocity.y += eff->accel.y;
            eff->velocity.z += eff->accel.z;
            eff->scale += eff->scaleStep;
        }
    }
}

void EnFd_UpdateDots(EnFd* self) {
    EnFdEffect* eff = self->effects;
    s16 i;
    Color_RGBA8 dotColors[] = {
        { 255, 128, 0, 0 },
        { 255, 0, 0, 0 },
        { 255, 255, 0, 0 },
        { 255, 0, 0, 0 },
    };

    for (i = 0; i < ARRAY_COUNT(self->effects); i++, eff++) {
        if (eff->type == FD_EFFECT_DOT) {
            eff->pos.x += eff->velocity.x;
            eff->pos.y += eff->velocity.y;
            eff->pos.z += eff->velocity.z;
            eff->timer++;
            eff->velocity.x += eff->accel.x;
            eff->velocity.y += eff->accel.y;
            eff->velocity.z += eff->accel.z;

            eff->color.r = dotColors[eff->timer % 4].r;
            eff->color.g = dotColors[eff->timer % 4].g;
            eff->color.b = dotColors[eff->timer % 4].b;
            if (eff->color.a > 30) {
                eff->color.a -= 30;
            } else {
                eff->color.a = 0;
                eff->type = FD_EFFECT_NONE;
            }
        }
    }
}

void EnFd_DrawFlames(EnFd* self, GlobalContext* globalCtx) {
    static void* dustTextures[] = {
        gDust8Tex, gDust7Tex, gDust6Tex, gDust5Tex, gDust4Tex, gDust3Tex, gDust2Tex, gDust1Tex,
    };
    s32 firstDone;
    s16 i;
    s16 idx;
    EnFdEffect* eff = self->effects;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fd.c", 1969);
    firstDone = false;
    if (1) {}
    func_80093D84(globalCtx->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(self->effects); i++, eff++) {
        if (eff->type == FD_EFFECT_FLAME) {
            if (!firstDone) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);
                gSPDisplayList(POLY_XLU_DISP++, gFlareDancerDL_7928);
                gDPSetEnvColor(POLY_XLU_DISP++, 255, 10, 0, (u8)((self->fadeAlpha / 255.0f) * 255));
                firstDone = true;
            }
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 0, (u8)((self->fadeAlpha / 255.0f) * 255));
            gDPPipeSync(POLY_XLU_DISP++);
            Matrix_Translate(eff->pos.x, eff->pos.y, eff->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(eff->scale, eff->scale, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_fd.c", 2006),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            idx = eff->timer * (8.0f / eff->initialTimer);
            gSPSegment(POLY_XLU_DISP++, 0x8, SEGMENTED_TO_VIRTUAL(dustTextures[idx]));
            gSPDisplayList(POLY_XLU_DISP++, gFlareDancerSquareParticleDL);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fd.c", 2020);
}

void EnFd_DrawDots(EnFd* self, GlobalContext* globalCtx) {
    s16 i;
    s16 firstDone;
    EnFdEffect* eff = self->effects;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fd.c", 2034);

    firstDone = false;
    func_80093D84(globalCtx->state.gfxCtx);

    for (i = 0; i < ARRAY_COUNT(self->effects); i++, eff++) {
        if (eff->type == FD_EFFECT_DOT) {
            if (!firstDone) {
                func_80093D84(globalCtx->state.gfxCtx);
                gSPDisplayList(POLY_XLU_DISP++, gFlareDancerDL_79F8);
                firstDone = true;
            }
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, eff->color.r, eff->color.g, eff->color.b,
                            (u8)(eff->color.a * (self->fadeAlpha / 255.0f)));
            gDPPipeSync(POLY_XLU_DISP++);
            if (1) {}
            Matrix_Translate(eff->pos.x, eff->pos.y, eff->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(eff->scale, eff->scale, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_fd.c", 2064),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gFlareDancerTriangleParticleDL);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fd.c", 2071);
}
