/*
 * File: z_en_sb.c
 * Overlay: ovl_En_Sb
 * Description: Shellblade
 */

#include "z_en_sb.h"
#include "vt.h"
#include "objects/object_sb/object_sb.h"

#define FLAGS 0x00000005

#define THIS ((EnSb*)thisx)

void EnSb_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSb_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSb_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnSb_SetupWaitClosed(EnSb* self);

void EnSb_WaitClosed(EnSb* self, GlobalContext* globalCtx);
void EnSb_Open(EnSb* self, GlobalContext* globalCtx);
void EnSb_WaitOpen(EnSb* self, GlobalContext* globalCtx);
void EnSb_TurnAround(EnSb* self, GlobalContext* globalCtx);
void EnSb_Lunge(EnSb* self, GlobalContext* globalCtx);
void EnSb_Bounce(EnSb* self, GlobalContext* globalCtx);
void EnSb_Cooldown(EnSb* self, GlobalContext* globalCtx);

const ActorInit En_Sb_InitVars = {
    ACTOR_EN_SB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SB,
    sizeof(EnSb),
    (ActorFunc)EnSb_Init,
    (ActorFunc)EnSb_Destroy,
    (ActorFunc)EnSb_Update,
    (ActorFunc)EnSb_Draw,
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        COLSHAPE_CYLINDER,
    },
    { 0x00, { 0xFFCFFFFF, 0x04, 0x08 }, { 0xFFCFFFFF, 0x00, 0x00 }, 0x01, 0x01, 0x01 },
    { 30, 40, 0, { 0, 0, 0 } },
};

static DamageTable sDamageTable[] = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0xF),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(2, 0xF),
    /* Hammer swing  */ DMG_ENTRY(2, 0xF),
    /* Hookshot      */ DMG_ENTRY(2, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xD),
    /* Master sword  */ DMG_ENTRY(2, 0xD),
    /* Giant's Knife */ DMG_ENTRY(4, 0xD),
    /* Fire arrow    */ DMG_ENTRY(4, 0x2),
    /* Ice arrow     */ DMG_ENTRY(2, 0xF),
    /* Light arrow   */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 1   */ DMG_ENTRY(4, 0xE),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0xF),
    /* Fire magic    */ DMG_ENTRY(4, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xD),
    /* Giant spin    */ DMG_ENTRY(4, 0xD),
    /* Master spin   */ DMG_ENTRY(2, 0xD),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xD),
    /* Giant jump    */ DMG_ENTRY(8, 0xD),
    /* Master jump   */ DMG_ENTRY(4, 0xD),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x27, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

static Vec3f sFlamePosOffsets[] = {
    { 5.0f, 0.0f, 0.0f },
    { -5.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 5.0f },
    { 0.0f, 0.0f, -5.0f },
};

typedef enum {
    /* 0x00 */ SHELLBLADE_OPEN,
    /* 0x01 */ SHELLBLADE_WAIT_CLOSED,
    /* 0x02 */ SHELLBLADE_WAIT_OPEN,
    /* 0x03 */ SHELLBLADE_LUNGE,
    /* 0x04 */ SHELLBLADE_BOUNCE
} ShellbladeBehavior;

void EnSb_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSb* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.colChkInfo.damageTable = sDamageTable;
    self->actor.colChkInfo.health = 2;
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_sb_Skel_002BF0, &object_sb_Anim_000194, NULL, NULL, 0);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->isDead = false;
    self->actor.colChkInfo.mass = 0;
    Actor_SetScale(&self->actor, 0.006f);
    self->actor.shape.rot.y = 0;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = -0.35f;
    self->fire = 0;
    self->hitByWindArrow = false;
    self->actor.velocity.y = -1.0f;
    EnSb_SetupWaitClosed(self);
}

void EnSb_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnSb* self = THIS;
    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnSb_SpawnBubbles(GlobalContext* globalCtx, EnSb* self) {
    s32 i;

    if (self->actor.yDistToWater > 0) {
        for (i = 0; i < 10; i++) {
            EffectSsBubble_Spawn(globalCtx, &self->actor.world.pos, 10.0f, 10.0f, 30.0f, 0.25f);
        }
    }
}

void EnSb_SetupWaitClosed(EnSb* self) {
    Animation_Change(&self->skelAnime, &object_sb_Anim_00004C, 1.0f, 0, Animation_GetLastFrame(&object_sb_Anim_00004C),
                     ANIMMODE_ONCE, 0.0f);
    self->behavior = SHELLBLADE_WAIT_CLOSED;
    self->actionFunc = EnSb_WaitClosed;
}

void EnSb_SetupOpen(EnSb* self) {
    Animation_Change(&self->skelAnime, &object_sb_Anim_000194, 1.0f, 0, Animation_GetLastFrame(&object_sb_Anim_000194),
                     ANIMMODE_ONCE, 0.0f);
    self->behavior = SHELLBLADE_OPEN;
    self->actionFunc = EnSb_Open;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHELL_MOUTH);
}

void EnSb_SetupWaitOpen(EnSb* self) {
    Animation_Change(&self->skelAnime, &object_sb_Anim_002C8C, 1.0f, 0, Animation_GetLastFrame(&object_sb_Anim_002C8C),
                     ANIMMODE_LOOP, 0.0f);
    self->behavior = SHELLBLADE_WAIT_OPEN;
    self->actionFunc = EnSb_WaitOpen;
}

void EnSb_SetupLunge(EnSb* self) {
    f32 frameCount = Animation_GetLastFrame(&object_sb_Anim_000124);
    f32 playbackSpeed = self->actor.yDistToWater > 0.0f ? 1.0f : 0.0f;

    Animation_Change(&self->skelAnime, &object_sb_Anim_000124, playbackSpeed, 0.0f, frameCount, ANIMMODE_ONCE, 0);
    self->behavior = SHELLBLADE_LUNGE;
    self->actionFunc = EnSb_Lunge;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHELL_MOUTH);
}

void EnSb_SetupBounce(EnSb* self) {
    Animation_Change(&self->skelAnime, &object_sb_Anim_0000B4, 1.0f, 0, Animation_GetLastFrame(&object_sb_Anim_0000B4),
                     ANIMMODE_ONCE, 0.0f);
    self->behavior = SHELLBLADE_BOUNCE;
    self->actionFunc = EnSb_Bounce;
}

void EnSb_SetupCooldown(EnSb* self, s32 changeSpeed) {
    f32 frameCount = Animation_GetLastFrame(&object_sb_Anim_00004C);

    if (self->behavior != SHELLBLADE_WAIT_CLOSED) {
        Animation_Change(&self->skelAnime, &object_sb_Anim_00004C, 1.0f, 0, frameCount, ANIMMODE_ONCE, 0.0f);
    }
    self->behavior = SHELLBLADE_WAIT_CLOSED;
    if (changeSpeed) {
        if (self->actor.yDistToWater > 0.0f) {
            self->actor.speedXZ = -5.0f;
            if (self->actor.velocity.y < 0.0f) {
                self->actor.velocity.y = 2.1f;
            }
        } else {
            self->actor.speedXZ = -6.0f;
            if (self->actor.velocity.y < 0.0f) {
                self->actor.velocity.y = 1.4f;
            }
        }
    }
    self->timer = 60;
    self->actionFunc = EnSb_Cooldown;
}

void EnSb_WaitClosed(EnSb* self, GlobalContext* globalCtx) {
    // always face toward link
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, 0x7D0, 0x0);

    if ((self->actor.xzDistToPlayer <= 160.0f) && (self->actor.xzDistToPlayer > 40.0f)) {
        EnSb_SetupOpen(self);
    }
}

void EnSb_Open(EnSb* self, GlobalContext* globalCtx) {
    f32 currentFrame = self->skelAnime.curFrame;

    if (Animation_GetLastFrame(&object_sb_Anim_000194) <= currentFrame) {
        self->timer = 15;
        EnSb_SetupWaitOpen(self);
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, 0x7D0, 0x0);
        if ((self->actor.xzDistToPlayer > 160.0f) || (self->actor.xzDistToPlayer <= 40.0f)) {
            EnSb_SetupWaitClosed(self);
        }
    }
}

void EnSb_WaitOpen(EnSb* self, GlobalContext* globalCtx) {
    s16 timer = self->timer;

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, 0x7D0, 0x0);

    if ((self->actor.xzDistToPlayer > 160.0f) || (self->actor.xzDistToPlayer <= 40.0f)) {
        EnSb_SetupWaitClosed(self);
    }

    if (timer != 0) {
        self->timer = timer - 1;
    } else {
        self->timer = 0;
        self->attackYaw = self->actor.yawTowardsPlayer;
        self->actionFunc = EnSb_TurnAround;
    }
}

void EnSb_TurnAround(EnSb* self, GlobalContext* globalCtx) {
    s16 invertedYaw;

    invertedYaw = self->attackYaw + 0x8000;
    Math_SmoothStepToS(&self->actor.shape.rot.y, invertedYaw, 0x1, 0x1F40, 0xA);

    if (self->actor.shape.rot.y == invertedYaw) {
        self->actor.world.rot.y = self->attackYaw;
        if (self->actor.yDistToWater > 0.0f) {
            self->actor.velocity.y = 3.0f;
            self->actor.speedXZ = 5.0f;
            self->actor.gravity = -0.35f;
        } else {
            self->actor.velocity.y = 2.0f;
            self->actor.speedXZ = 6.0f;
            self->actor.gravity = -2.0f;
        }
        EnSb_SpawnBubbles(globalCtx, self);
        self->bouncesLeft = 3;
        EnSb_SetupLunge(self);
        // "Attack!!"
        osSyncPrintf("アタァ〜ック！！\n");
    }
}

void EnSb_Lunge(EnSb* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f);
    if ((self->actor.velocity.y <= -0.1f) || ((self->actor.bgCheckFlags & 2))) {
        if (!(self->actor.yDistToWater > 0.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        }
        self->actor.bgCheckFlags = self->actor.bgCheckFlags & ~2;
        EnSb_SetupBounce(self);
    }
}

void EnSb_Bounce(EnSb* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 currentFrame;
    f32 frameCount;

    currentFrame = self->skelAnime.curFrame;
    frameCount = Animation_GetLastFrame(&object_sb_Anim_0000B4);
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f);

    if (currentFrame == frameCount) {
        if (self->bouncesLeft != 0) {
            self->bouncesLeft--;
            self->timer = 1;
            if (self->actor.yDistToWater > 0.0f) {
                self->actor.velocity.y = 3.0f;
                self->actor.speedXZ = 5.0f;
                self->actor.gravity = -0.35f;
            } else {
                self->actor.velocity.y = 2.0f;
                self->actor.speedXZ = 6.0f;
                self->actor.gravity = -2.0f;
            }
            EnSb_SpawnBubbles(globalCtx, self);
            EnSb_SetupLunge(self);
        } else if (self->actor.bgCheckFlags & 1) {
            self->actor.bgCheckFlags &= ~2;
            self->actor.speedXZ = 0.0f;
            self->timer = 1;
            EnSb_SetupWaitClosed(self);
            osSyncPrintf(VT_FGCOL(RED) "攻撃終了！！" VT_RST "\n"); // "Attack Complete!"
        }
    }
}

void EnSb_Cooldown(EnSb* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
        if (self->actor.bgCheckFlags & 1) {
            self->actor.bgCheckFlags &= ~1;
            self->actor.speedXZ = 0.0f;
        }
    } else {
        if (self->actor.bgCheckFlags & 1) {
            self->actor.bgCheckFlags &= ~1;
            self->actionFunc = EnSb_WaitClosed;
            self->actor.speedXZ = 0.0f;
        }
    }
}

s32 EnSb_IsVulnerable(EnSb* self) {
    switch (self->behavior) {
        case SHELLBLADE_OPEN:
            if ((self->skelAnime.curFrame >= 2.0f) && (self->skelAnime.curFrame <= 5.0f)) {
                return true;
            }
            break;
        case SHELLBLADE_WAIT_CLOSED:
            if ((self->skelAnime.curFrame >= 0.0f) && (self->skelAnime.curFrame <= 1.0f)) {
                return true;
            }
            break;
        case SHELLBLADE_WAIT_OPEN:
            if ((self->skelAnime.curFrame >= 0.0f) && (self->skelAnime.curFrame <= 19.0f)) {
                return true;
            }
            break;
        case SHELLBLADE_LUNGE:
            if (self->skelAnime.curFrame == 0.0f) {
                return true;
            }
            break;
        case SHELLBLADE_BOUNCE:
            if ((self->skelAnime.curFrame >= 3.0f) && (self->skelAnime.curFrame <= 5.0f)) {
                return true;
            }
            break;
    }
    return false;
}

s32 EnSb_UpdateDamage(EnSb* self, GlobalContext* globalCtx) {
    Vec3f hitPoint;
    f32 hitY;
    s16 yawDiff;
    s32 tookDamage;
    u8 hitByWindArrow;

    // hit box collided, switch to cool down
    if ((self->collider.base.atFlags & AT_HIT)) {
        EnSb_SetupCooldown(self, 1);
        return 1;
    }

    // hurt box collided, take damage if appropriate
    if ((self->collider.base.acFlags & AC_HIT)) {
        hitByWindArrow = false;
        tookDamage = false;
        self->collider.base.acFlags &= ~AC_HIT;

        switch (self->actor.colChkInfo.damageEffect) {
            case 14: // wind arrow
                hitByWindArrow = true;
            case 15: // explosions, arrow, hammer, ice arrow, light arrow, spirit arrow, shadow arrow
                if (EnSb_IsVulnerable(self)) {
                    hitY = self->collider.info.bumper.hitPos.y - self->actor.world.pos.y;
                    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
                    if ((hitY < 30.0f) && (hitY > 10.0f) && (yawDiff >= -0x1FFF) && (yawDiff < 0x2000)) {
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 0x50);
                        tookDamage = true;
                    }
                }
                break;
            case 2: // fire arrow, dins fire
                self->fire = 4;
                Actor_ApplyDamage(&self->actor);
                Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 0x50);
                tookDamage = true;
                break;
            case 1:  // hookshot/longshot
            case 13: // all sword damage
                if (EnSb_IsVulnerable(self)) {
                    hitY = self->collider.info.bumper.hitPos.y - self->actor.world.pos.y;
                    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
                    if ((hitY < 30.0f) && (hitY > 10.0f) && (yawDiff >= -0x1FFF) && (yawDiff < 0x2000)) {
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 0x50);
                        tookDamage = true;
                        EnSb_SetupCooldown(self, 0);
                    }
                }
                break;
            default:
                break;
        }
        if (self->actor.colChkInfo.health == 0) {
            self->hitByWindArrow = hitByWindArrow;
            BodyBreak_Alloc(&self->bodyBreak, 8, globalCtx);
            self->isDead = true;
            Enemy_StartFinishingBlow(globalCtx, &self->actor);
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 40, NA_SE_EN_SHELL_DEAD);
            return 1;
        }

        // if player attack didn't do damage, play recoil sound and spawn sparks
        if (!tookDamage) {
            hitPoint.x = self->collider.info.bumper.hitPos.x;
            hitPoint.y = self->collider.info.bumper.hitPos.y;
            hitPoint.z = self->collider.info.bumper.hitPos.z;
            CollisionCheck_SpawnShieldParticlesMetal2(globalCtx, &hitPoint);
        }
    }

    return 0;
}

void EnSb_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnSb* self = THIS;
    s32 pad;

    if (self->isDead) {
        if (self->actor.yDistToWater > 0.0f) {
            self->actor.params = 4;
        } else {
            self->actor.params = 1;
        }
        if (BodyBreak_SpawnParts(&self->actor, &self->bodyBreak, globalCtx, self->actor.params)) {
            if (!self->hitByWindArrow) {
                Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x80);
            } else {
                Item_DropCollectible(globalCtx, &self->actor.world.pos, 8);
            }
            Actor_Kill(&self->actor);
        }
    } else {
        Actor_SetFocus(&self->actor, 20.0f);
        Actor_SetScale(&self->actor, 0.006f);
        Actor_MoveForward(&self->actor);
        self->actionFunc(self, globalCtx);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 20.0f, 5);
        EnSb_UpdateDamage(self, globalCtx);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        SkelAnime_Update(&self->skelAnime);
    }
}

void EnSb_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnSb* self = THIS;

    BodyBreak_SetInfo(&self->bodyBreak, limbIndex, 0, 6, 8, dList, BODYBREAK_OBJECT_DEFAULT);
}

void EnSb_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnSb* self = THIS;
    Vec3f flamePos;
    Vec3f* offset;
    s16 fireDecr;

    func_8002EBCC(&self->actor, globalCtx, 1);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, EnSb_PostLimbDraw, self);
    if (self->fire != 0) {
        self->actor.colorFilterTimer++;
        fireDecr = self->fire - 1;
        // self is intended to draw flames after being burned, but the condition is never met to run self code
        // fire gets set to 4 when burned, decrements to 3 and fails the "& 1" check and never stores the decrement
        if ((fireDecr & 1) == 0) {
            offset = &sFlamePosOffsets[(fireDecr & 3)];
            flamePos.x = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.x + offset->x);
            flamePos.y = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.y + offset->y);
            flamePos.z = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.z + offset->z);
            EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &flamePos, 100, 0, 0, -1);
        }
    }
}
