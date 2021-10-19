/*
 * File: z_en_floormas
 * Overlay: En_Floormas
 * Description: Floormaster
 */

#include "z_en_floormas.h"
#include "objects/object_wallmaster/object_wallmaster.h"

#define FLAGS 0x00000405

#define THIS ((EnFloormas*)thisx)

#define SPAWN_INVISIBLE 0x8000
#define SPAWN_SMALL 0x10

#define MERGE_MASTER 0x40
#define MERGE_SLAVE 0x20

void EnFloormas_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFloormas_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFloormas_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFloormas_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnFloormas_GrabLink(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Split(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Recover(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_DrawHighlighted(Actor* self, GlobalContext* globalCtx);
void EnFloormas_SmWait(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_SetupBigDecideAction(EnFloormas* self);
void EnFloormas_Freeze(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_TakeDamage(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Merge(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_JumpAtLink(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_SmSlaveJumpAtMaster(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_SmShrink(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_SmDecideAction(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_SmWalk(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Land(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Hover(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Turn(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Run(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_BigStopWalk(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_BigWalk(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Stand(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_BigDecideAction(EnFloormas* self, GlobalContext* globalCtx);
void EnFloormas_Charge(EnFloormas* self, GlobalContext* globalCtx);

const ActorInit En_Floormas_InitVars = {
    ACTOR_EN_FLOORMAS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_WALLMASTER,
    sizeof(EnFloormas),
    (ActorFunc)EnFloormas_Init,
    (ActorFunc)EnFloormas_Destroy,
    (ActorFunc)EnFloormas_Update,
    (ActorFunc)EnFloormas_Draw,
};

static ColliderCylinderInit sCylinderInit = {
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
        { 0xFFCFFFFF, 0x04, 0x10 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 25, 40, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 4, 30, 40, 150 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x1),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(4, 0x2),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(4, 0x4),
    /* Unk arrow 1   */ DMG_ENTRY(4, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(4, 0x4),
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
    ICHAIN_S8(naviEnemyId, 0x31, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 5500, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_STOP),
};

void EnFloormas_Init(Actor* thisx, GlobalContext* globalCtx2) {
    EnFloormas* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    s32 invisble;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 50.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gWallmasterSkel, &gWallmasterWaitAnim, self->jointTable,
                       self->morphTable, 25);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    self->zOffset = -1600;
    invisble = self->actor.params & SPAWN_INVISIBLE;

    // s16 cast needed
    self->actor.params &= (s16) ~(SPAWN_INVISIBLE);
    if (invisble) {
        self->actor.flags |= 0x80;
        self->actor.draw = EnFloormas_DrawHighlighted;
    }

    if (self->actor.params == SPAWN_SMALL) {
        self->actor.draw = NULL;
        self->actor.flags &= ~1;
        self->actionFunc = EnFloormas_SmWait;
    } else {
        // spawn first small floormaster
        self->actor.parent =
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_FLOORMAS, self->actor.world.pos.x,
                        self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, invisble + SPAWN_SMALL);
        if (self->actor.parent == NULL) {
            Actor_Kill(&self->actor);
            return;
        }
        // spawn 2nd small floormaster
        self->actor.child =
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_FLOORMAS, self->actor.world.pos.x,
                        self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, invisble + SPAWN_SMALL);
        if (self->actor.child == NULL) {
            Actor_Kill(self->actor.parent);
            Actor_Kill(&self->actor);
            return;
        }

        // link floormasters together
        self->actor.parent->child = &self->actor;
        self->actor.parent->parent = self->actor.child;
        self->actor.child->parent = &self->actor;
        self->actor.child->child = self->actor.parent;
        EnFloormas_SetupBigDecideAction(self);
    }
}

void EnFloormas_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFloormas* self = THIS;
    ColliderCylinder* col = &self->collider;
    Collider_DestroyCylinder(globalCtx, col);
}

void EnFloormas_MakeInvulnerable(EnFloormas* self) {
    self->collider.base.colType = COLTYPE_HARD;
    self->collider.base.acFlags |= AC_HARD;
    self->actionTarget = 0x28;
}

void EnFloormas_MakeVulnerable(EnFloormas* self) {
    self->collider.base.colType = COLTYPE_HIT0;
    self->actionTarget = 0;
    self->collider.base.acFlags &= ~AC_HARD;
}

void EnFloormas_SetupBigDecideAction(EnFloormas* self) {
    Animation_PlayOnce(&self->skelAnime, &gWallmasterWaitAnim);
    self->actionFunc = EnFloormas_BigDecideAction;
    self->actor.speedXZ = 0.0f;
}

void EnFloormas_SetupStand(EnFloormas* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gWallmasterStandUpAnim, -3.0f);
    self->actionFunc = EnFloormas_Stand;
}

void EnFloormas_SetupBigWalk(EnFloormas* self) {
    if (self->actionFunc != EnFloormas_Run) {
        Animation_PlayLoopSetSpeed(&self->skelAnime, &gWallmasterWalkAnim, 1.5f);
    } else {
        self->skelAnime.playSpeed = 1.5f;
    }

    self->actionTimer = Rand_S16Offset(2, 4);
    self->actionFunc = EnFloormas_BigWalk;
    self->actor.speedXZ = 1.5f;
}

void EnFloormas_SetupBigStopWalk(EnFloormas* self) {
    Animation_PlayOnce(&self->skelAnime, &gWallmasterStopWalkAnim);
    self->actionFunc = EnFloormas_BigStopWalk;
    self->actor.speedXZ = 0.0f;
}

void EnFloormas_SetupRun(EnFloormas* self) {
    self->actionTimer = 0;
    self->actionFunc = EnFloormas_Run;
    self->actor.speedXZ = 5.0f;
    self->skelAnime.playSpeed = 3.0f;
}

void EnFloormas_SetupTurn(EnFloormas* self) {
    s16 rotDelta = self->actionTarget - self->actor.shape.rot.y;
    self->actor.speedXZ = 0.0f;
    if (rotDelta > 0) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gFloormasterTurnAnim, -3.0f);
    } else {
        Animation_Change(&self->skelAnime, &gFloormasterTurnAnim, -1.0f, Animation_GetLastFrame(&gFloormasterTurnAnim),
                         0.0f, ANIMMODE_ONCE, -3.0f);
    }

    if (self->actor.scale.x > 0.004f) {
        self->actionTarget = (rotDelta * (2.0f / 30.0f));
    } else {
        self->skelAnime.playSpeed = self->skelAnime.playSpeed + self->skelAnime.playSpeed;
        self->actionTarget = rotDelta * (2.0f / 15.0f);
    }
    self->actionFunc = EnFloormas_Turn;
}

void EnFloormas_SetupHover(EnFloormas* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &gWallmasterHoverAnim, 3.0f, 0, Animation_GetLastFrame(&gWallmasterHoverAnim),
                     ANIMMODE_ONCE, -3.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = 0.0f;
    EnFloormas_MakeInvulnerable(self);
    Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 15.0f, 6, 20.0f, 0x12C, 0x64, 1);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_ATTACK);
    self->actionFunc = EnFloormas_Hover;
}

void EnFloormas_SetupCharge(EnFloormas* self) {
    self->actionTimer = 25;
    self->actor.gravity = -0.15f;
    self->actionFunc = EnFloormas_Charge;
    self->actor.speedXZ = 0.5f;
}

void EnFloormas_SetupLand(EnFloormas* self) {
    Animation_Change(&self->skelAnime, &gWallmasterJumpAnim, 1.0f, 41.0f, 42.0f, ANIMMODE_ONCE, 5.0f);
    if ((self->actor.speedXZ < 0.0f) || (self->actionFunc != EnFloormas_Charge)) {
        self->actionTimer = 30;
    } else {
        self->actionTimer = 45;
    }
    self->actor.gravity = -1.0f;
    self->actionFunc = EnFloormas_Land;
}

void EnFloormas_SetupSplit(EnFloormas* self) {

    Actor_SetScale(&self->actor, 0.004f);
    self->actor.flags |= 0x10;
    if ((self->actor.flags & 0x80) == 0x80) {
        self->actor.draw = EnFloormas_DrawHighlighted;
    } else {
        self->actor.draw = EnFloormas_Draw;
    }
    self->actor.shape.rot.y = self->actor.parent->shape.rot.y + 0x5555;
    self->actor.world.pos = self->actor.parent->world.pos;
    self->actor.params = 0x10;
    Animation_Change(&self->skelAnime, &gWallmasterJumpAnim, 1.0f, 41.0f, Animation_GetLastFrame(&gWallmasterJumpAnim),
                     ANIMMODE_ONCE, 0.0f);
    self->collider.dim.radius = sCylinderInit.dim.radius * 0.6f;
    self->collider.dim.height = sCylinderInit.dim.height * 0.6f;
    self->collider.info.bumperFlags &= ~BUMP_HOOKABLE;
    self->actor.speedXZ = 4.0f;
    self->actor.velocity.y = 7.0f;
    // using div creates a signed check.
    self->actor.colChkInfo.health = sColChkInfoInit.health >> 1;
    self->actionFunc = EnFloormas_Split;
}

void EnFloormas_SetupSmWalk(EnFloormas* self) {
    Animation_PlayLoopSetSpeed(&self->skelAnime, &gWallmasterWalkAnim, 4.5f);
    self->actionFunc = EnFloormas_SmWalk;
    self->actor.speedXZ = 5.0f;
}

void EnFloormas_SetupSmDecideAction(EnFloormas* self) {
    if (self->actionFunc != EnFloormas_SmWalk) {
        Animation_PlayLoopSetSpeed(&self->skelAnime, &gWallmasterWalkAnim, 4.5f);
    }
    self->actionFunc = EnFloormas_SmDecideAction;
    self->actor.speedXZ = 5.0f;
}

void EnFloormas_SetupSmShrink(EnFloormas* self, GlobalContext* globalCtx) {
    static Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    static Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;

    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + 15.0f;
    pos.z = self->actor.world.pos.z;
    EffectSsDeadDb_Spawn(globalCtx, &pos, &velocity, &accel, 150, -10, 255, 255, 255, 255, 0, 0, 255, 1, 9, true);
    self->actionFunc = EnFloormas_SmShrink;
}

void EnFloormas_SetupSlaveJumpAtMaster(EnFloormas* self) {
    Animation_Change(&self->skelAnime, &gWallmasterJumpAnim, 2.0f, 0.0f, 41.0f, ANIMMODE_ONCE, 0.0f);
    self->actionFunc = EnFloormas_SmSlaveJumpAtMaster;
    self->actor.speedXZ = 0.0f;
}

void EnFloormas_SetupJumpAtLink(EnFloormas* self) {
    Animation_Change(&self->skelAnime, &gWallmasterJumpAnim, 2.0f, 0.0f, 41.0f, ANIMMODE_ONCE, 0.0f);
    self->actionFunc = EnFloormas_JumpAtLink;
    self->actor.speedXZ = 0.0f;
}

void EnFloormas_SetupGrabLink(EnFloormas* self, Player* player) {
    f32 yDelta;
    f32 xzDelta;

    Animation_Change(&self->skelAnime, &gWallmasterJumpAnim, 1.0f, 36.0f, 45.0f, ANIMMODE_ONCE, -3.0f);
    self->actor.flags &= ~1;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    EnFloormas_MakeInvulnerable(self);
    if (!LINK_IS_ADULT) {
        yDelta = CLAMP(-self->actor.yDistToPlayer, 20.0f, 30.0f);
        xzDelta = -10.0f;
    } else {
        yDelta = CLAMP(-self->actor.yDistToPlayer, 25.0f, 45.0f);
        xzDelta = -70.0f;
    }
    self->actor.world.pos.y = player->actor.world.pos.y + yDelta;
    self->actor.world.pos.x = Math_SinS(self->actor.shape.rot.y) * (xzDelta * 0.1f) + player->actor.world.pos.x;
    self->actor.world.pos.z = Math_CosS(self->actor.shape.rot.y) * (xzDelta * 0.1f) + player->actor.world.pos.z;
    self->actor.shape.rot.x = -0x4CE0;
    self->actionFunc = EnFloormas_GrabLink;
}

void EnFloormas_SetupMerge(EnFloormas* self) {
    Animation_PlayOnce(&self->skelAnime, &gWallmasterWaitAnim);
    self->actionTimer = 0;
    self->smActionTimer += 1500;
    EnFloormas_MakeInvulnerable(self);
    self->actionFunc = EnFloormas_Merge;
}

void EnFloormas_SetupSmWait(EnFloormas* self) {
    EnFloormas* parent = (EnFloormas*)self->actor.parent;
    EnFloormas* child = (EnFloormas*)self->actor.child;

    // if self is the last remaining small floor master, kill all.
    if ((parent->actionFunc == EnFloormas_SmWait) && (child->actionFunc == EnFloormas_SmWait)) {
        Actor_Kill(&parent->actor);
        Actor_Kill(&child->actor);
        Actor_Kill(&self->actor);
        return;
    }
    self->actor.draw = NULL;
    self->actionFunc = EnFloormas_SmWait;
    self->actor.flags &= ~0x11;
}

void EnFloormas_SetupTakeDamage(EnFloormas* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gWallmasterDamageAnim, -3.0f);
    if (self->collider.info.acHitInfo->toucher.dmgFlags & 0x1F824) {
        self->actor.world.rot.y = self->collider.base.ac->world.rot.y;
    } else {
        self->actor.world.rot.y = Actor_WorldYawTowardActor(&self->actor, self->collider.base.ac) + 0x8000;
    }
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0x14);
    self->actionFunc = EnFloormas_TakeDamage;
    self->actor.speedXZ = 5.0f;
    self->actor.velocity.y = 5.5f;
}

void EnFloormas_SetupRecover(EnFloormas* self) {
    Animation_PlayOnce(&self->skelAnime, &gWallmasterRecoverFromDamageAnim);
    self->actor.velocity.y = self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actionFunc = EnFloormas_Recover;
}

void EnFloormas_SetupFreeze(EnFloormas* self) {
    Animation_Change(&self->skelAnime, &gWallmasterJumpAnim, 1.5f, 0, 20.0f, ANIMMODE_ONCE, -3.0f);
    self->actor.speedXZ = 0.0f;
    if (self->actor.colChkInfo.damageEffect == 4) {
        Actor_SetColorFilter(&self->actor, -0x8000, 0xFF, 0, 0x50);
    } else {
        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
        if (self->actor.scale.x > 0.004f) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
        }
    }
    self->actionTimer = 80;
    self->actionFunc = EnFloormas_Freeze;
}

void EnFloormas_Die(EnFloormas* self, GlobalContext* globalCtx) {
    if (self->actor.scale.x > 0.004f) {
        // split
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer + 0x8000;
        EnFloormas_SetupSplit((EnFloormas*)self->actor.child);
        EnFloormas_SetupSplit((EnFloormas*)self->actor.parent);
        EnFloormas_SetupSplit(self);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SPLIT);
    } else {
        // Die
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x90);
        EnFloormas_SetupSmShrink(self, globalCtx);
    }
}

void EnFloormas_BigDecideAction(EnFloormas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        // within 400 units of link and within 90 degrees rotation of him
        if (self->actor.xzDistToPlayer < 400.0f && !Actor_IsFacingPlayer(&self->actor, 0x4000)) {
            self->actionTarget = self->actor.yawTowardsPlayer;
            EnFloormas_SetupTurn(self);
            // within 280 units of link and within 45 degrees rotation of him
        } else if (self->actor.xzDistToPlayer < 280.0f && Actor_IsFacingPlayer(&self->actor, 0x2000)) {
            EnFloormas_SetupHover(self, globalCtx);
        } else {
            EnFloormas_SetupStand(self);
        }
    }
}

void EnFloormas_Stand(EnFloormas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.scale.x > 0.004f) {
            EnFloormas_SetupBigWalk(self);
        } else if (self->actor.params == MERGE_SLAVE) {
            EnFloormas_SetupSmDecideAction(self);
        } else {
            EnFloormas_SetupSmWalk(self);
        }
    }
}

void EnFloormas_BigWalk(EnFloormas* self, GlobalContext* globalCtx) {
    s32 animPastFrame;
    SkelAnime_Update(&self->skelAnime);
    animPastFrame = Animation_OnFrame(&self->skelAnime, 0.0f);
    if (animPastFrame) {
        if (self->actionTimer != 0) {
            self->actionTimer--;
        }
    }
    if (((animPastFrame || (Animation_OnFrame(&self->skelAnime, 12.0f))) ||
         (Animation_OnFrame(&self->skelAnime, 24.0f) != 0)) ||
        (Animation_OnFrame(&self->skelAnime, 36.0f) != 0)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_WALK);
    }

    if ((self->actor.xzDistToPlayer < 320.0f) && (Actor_IsFacingPlayer(&self->actor, 0x4000))) {
        EnFloormas_SetupRun(self);
    } else if (self->actor.bgCheckFlags & 8) {
        // set target rotation to the colliding wall's rotation
        self->actionTarget = self->actor.wallYaw;
        EnFloormas_SetupTurn(self);
    } else if ((self->actor.xzDistToPlayer < 400.0f) && !Actor_IsFacingPlayer(&self->actor, 0x4000)) {
        // set target rotation to link.
        self->actionTarget = self->actor.yawTowardsPlayer;
        EnFloormas_SetupTurn(self);
    } else if (self->actionTimer == 0) {
        EnFloormas_SetupBigStopWalk(self);
    }
}

void EnFloormas_BigStopWalk(EnFloormas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnFloormas_SetupBigDecideAction(self);
    }
}

void EnFloormas_Run(EnFloormas* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 12.0f) ||
        Animation_OnFrame(&self->skelAnime, 24.0f) || Animation_OnFrame(&self->skelAnime, 36.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_WALK);
    }

    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x71C);

    if ((self->actor.xzDistToPlayer < 280.0f) && Actor_IsFacingPlayer(&self->actor, 0x2000) &&
        !(self->actor.bgCheckFlags & 8)) {
        EnFloormas_SetupHover(self, globalCtx);
    } else if (self->actor.xzDistToPlayer > 400.0f) {
        EnFloormas_SetupBigWalk(self);
    }
}

void EnFloormas_Turn(EnFloormas* self, GlobalContext* globalCtx) {
    char pad[4];
    f32 sp30;
    f32 sp2C;

    if (SkelAnime_Update(&self->skelAnime)) {
        EnFloormas_SetupStand(self);
    }

    if (((self->skelAnime.playSpeed > 0.0f) && Animation_OnFrame(&self->skelAnime, 21.0f)) ||
        ((self->skelAnime.playSpeed < 0.0f) && Animation_OnFrame(&self->skelAnime, 6.0f))) {
        if (self->actor.scale.x > 0.004f) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_WALK);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_WALK);
        }
    }
    // Needed to match
    if (!self->skelAnime.curFrame) {}
    if (self->skelAnime.curFrame >= 7.0f && self->skelAnime.curFrame < 22.0f) {
        sp30 = Math_SinS(self->actor.shape.rot.y + 0x4268);
        sp2C = Math_CosS(self->actor.shape.rot.y + 0x4268);
        self->actor.shape.rot.y += self->actionTarget;
        self->actor.world.pos.x -=
            (self->actor.scale.x * 2700.0f) * (Math_SinS(self->actor.shape.rot.y + 0x4268) - sp30);
        self->actor.world.pos.z -=
            (self->actor.scale.x * 2700.0f) * (Math_CosS(self->actor.shape.rot.y + 0x4268) - sp2C);
    }
}

void EnFloormas_Hover(EnFloormas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnFloormas_SetupCharge(self);
    }
    self->actor.shape.rot.x += 0x140;
    self->actor.world.pos.y += 10.0f;
    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 2730);
    Math_StepToS(&self->zOffset, 1200, 100);
}

void EnFloormas_Slide(EnFloormas* self, GlobalContext* globalCtx) {
    static Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;
    Vec3f velocity;

    pos.x = self->actor.world.pos.x;
    pos.z = self->actor.world.pos.z;
    pos.y = self->actor.floorHeight;

    velocity.y = 2.0f;
    velocity.x = Math_SinS(self->actor.shape.rot.y + 0x6000) * 7.0f;
    velocity.z = Math_CosS(self->actor.shape.rot.y + 0x6000) * 7.0f;

    func_800286CC(globalCtx, &pos, &velocity, &accel, 450, 100);

    velocity.x = Math_SinS(self->actor.shape.rot.y - 0x6000) * 7.0f;
    velocity.z = Math_CosS(self->actor.shape.rot.y - 0x6000) * 7.0f;

    func_800286CC(globalCtx, &pos, &velocity, &accel, 450, 100);

    func_8002F974(&self->actor, NA_SE_EN_FLOORMASTER_SLIDING);
}

void EnFloormas_Charge(EnFloormas* self, GlobalContext* globalCtx) {
    f32 distFromGround;

    if (self->actionTimer != 0) {
        self->actionTimer--;
    }

    Math_StepToF(&self->actor.speedXZ, 15.0f, SQ(self->actor.speedXZ) * (1.0f / 3.0f));
    Math_ScaledStepToS(&self->actor.shape.rot.x, -0x1680, 0x140);

    distFromGround = self->actor.world.pos.y - self->actor.floorHeight;
    if (distFromGround < 10.0f) {
        self->actor.world.pos.y = self->actor.floorHeight + 10.0f;
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
    }

    if (distFromGround < 12.0f) {
        EnFloormas_Slide(self, globalCtx);
    }

    if ((self->actor.bgCheckFlags & 8) || (self->actionTimer == 0)) {
        EnFloormas_SetupLand(self);
    }
}

void EnFloormas_Land(EnFloormas* self, GlobalContext* globalCtx) {
    s32 isOnGround;

    isOnGround = self->actor.bgCheckFlags & 1;
    if (self->actor.bgCheckFlags & 2) {
        if (self->actor.params != MERGE_MASTER) {
            EnFloormas_MakeVulnerable(self);
        }

        if (self->actor.velocity.y < -4.0f) {
            if (self->actor.scale.x > 0.004f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_LAND);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_LAND);
            }
        }
    }
    if (self->actor.bgCheckFlags & 8) {
        self->actor.speedXZ = 0.0f;
    }

    if (isOnGround) {
        Math_StepToF(&self->actor.speedXZ, 0.0f, 2.0f);
    }

    if ((self->actor.speedXZ > 0.0f) && ((self->actor.world.pos.y - self->actor.floorHeight) < 12.0f)) {
        EnFloormas_Slide(self, globalCtx);
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actionTimer != 0) {
            self->actionTimer--;
        }

        if (self->actionTimer == 0 && isOnGround) {
            if (self->skelAnime.endFrame < 45.0f) {
                self->skelAnime.endFrame = Animation_GetLastFrame(&gWallmasterJumpAnim);
            } else if (self->actor.params == MERGE_MASTER) {
                EnFloormas_SetupMerge(self);
            } else {
                EnFloormas_SetupStand(self);
                self->smActionTimer = 50;
            }
        }
    }

    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x140);
    Math_StepToS(&self->zOffset, -1600, 100);
}

void EnFloormas_Split(EnFloormas* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 1) {
        if (SkelAnime_Update(&self->skelAnime)) {
            self->actor.flags |= 1;
            self->smActionTimer = 50;
            EnFloormas_SetupStand(self);
        }
        Math_StepToF(&self->actor.speedXZ, 0.0f, 1.0f);
    }

    if (self->actor.bgCheckFlags & 2) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_LAND);
    }
}

void EnFloormas_SmWalk(EnFloormas* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    DECR(self->smActionTimer);

    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 18.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_WALK);
    }

    if (self->smActionTimer == 0) {
        EnFloormas_SetupSmDecideAction(self);
    } else if (self->actor.bgCheckFlags & 8) {
        self->actionTarget = self->actor.wallYaw;
        EnFloormas_SetupTurn(self);
    } else if (self->actor.xzDistToPlayer < 120.0f) {
        Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer + 0x8000, 0x38E);
    }
}

void EnFloormas_SmDecideAction(EnFloormas* self, GlobalContext* globalCtx) {
    Actor* primaryFloormas;
    s32 isAgainstWall;

    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 18.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_WALK);
    }
    isAgainstWall = self->actor.bgCheckFlags & 8;
    if (isAgainstWall) {
        self->actionTarget = self->actor.wallYaw;
        EnFloormas_SetupTurn(self);
        return;
    }

    if (self->actor.params == MERGE_SLAVE) {
        if (self->actor.parent->params == MERGE_MASTER) {
            primaryFloormas = self->actor.parent;
        } else if (self->actor.child->params == MERGE_MASTER) {
            primaryFloormas = self->actor.child;
        } else {
            self->actor.params = 0x10;
            return;
        }

        Math_ScaledStepToS(&self->actor.shape.rot.y, Actor_WorldYawTowardActor(&self->actor, primaryFloormas), 0x38E);
        if (Actor_WorldDistXZToActor(&self->actor, primaryFloormas) < 80.0f) {
            EnFloormas_SetupSlaveJumpAtMaster(self);
        }
    } else {
        Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x71C);
        if (self->actor.xzDistToPlayer < 80.0f) {
            EnFloormas_SetupJumpAtLink(self);
        }
    }
}

void EnFloormas_SmShrink(EnFloormas* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->actor.scale.x, 0.0f, 0.0015f)) {
        EnFloormas_SetupSmWait(self);
    }
    self->actor.scale.z = self->actor.scale.x;
    self->actor.scale.y = self->actor.scale.x;
}

void EnFloormas_JumpAtLink(EnFloormas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    if (self->skelAnime.curFrame < 20.0f) {
        Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    } else if (Animation_OnFrame(&self->skelAnime, 20.0f)) {
        self->actor.speedXZ = 5.0f;
        self->actor.velocity.y = 7.0f;
    } else if (self->actor.bgCheckFlags & 2) {
        self->actionTimer = 0x32;
        self->actor.speedXZ = 0.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_LAND);
        EnFloormas_SetupLand(self);
    } else if ((self->actor.yDistToPlayer < -10.0f) && (self->collider.base.ocFlags1 & OC1_HIT) &&
               (&player->actor == self->collider.base.oc)) {
        globalCtx->grabPlayer(globalCtx, player);
        EnFloormas_SetupGrabLink(self, player);
    }
}

void EnFloormas_GrabLink(EnFloormas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    EnFloormas* parent;
    EnFloormas* child;
    f32 yDelta;
    f32 xzDelta;

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->skelAnime.playSpeed > 0.0f) {
            self->skelAnime.playSpeed = -1.0f;
            self->skelAnime.endFrame = 36.0f;
            self->skelAnime.startFrame = 45.0f;
        } else {
            self->skelAnime.playSpeed = 1.0f;
            self->skelAnime.endFrame = 45.0f;
            self->skelAnime.startFrame = 36.0f;
        }
    }

    if (!LINK_IS_ADULT) {
        yDelta = CLAMP(-self->actor.yDistToPlayer, 20.0f, 30.0f);
        xzDelta = -10.0f;
    } else {
        yDelta = CLAMP(-self->actor.yDistToPlayer, 25.0f, 45.0f);
        xzDelta = -30.0f;
    }

    self->actor.world.pos.y = player->actor.world.pos.y + yDelta;
    self->actor.world.pos.x = Math_SinS(self->actor.shape.rot.y) * (xzDelta * 0.1f) + player->actor.world.pos.x;
    self->actor.world.pos.z = Math_CosS(self->actor.shape.rot.y) * (xzDelta * 0.1f) + player->actor.world.pos.z;

    // let go
    if (!(player->stateFlags2 & 0x80) || (player->invincibilityTimer < 0)) {
        parent = (EnFloormas*)self->actor.parent;
        child = (EnFloormas*)self->actor.child;

        if (((parent->actionFunc == EnFloormas_GrabLink) || parent->actionFunc == EnFloormas_SmWait) &&
            (child->actionFunc == EnFloormas_GrabLink || child->actionFunc == EnFloormas_SmWait)) {

            parent->actor.params = MERGE_SLAVE;
            child->actor.params = MERGE_SLAVE;
            self->actor.params = MERGE_MASTER;
        }

        self->actor.shape.rot.x = 0;
        self->actor.velocity.y = 6.0f;
        self->actor.flags |= 1;
        self->actor.speedXZ = -3.0f;
        EnFloormas_SetupLand(self);
    } else {
        // Damage link every 20 frames
        if ((self->actionTarget % 20) == 0) {
            if (!LINK_IS_ADULT) {
                func_8002F7DC(&player->actor, NA_SE_VO_LI_DAMAGE_S_KID);
            } else {
                func_8002F7DC(&player->actor, NA_SE_VO_LI_DAMAGE_S);
            }
            globalCtx->damagePlayer(globalCtx, -8);
        }
    }

    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_STICK);
}

void EnFloormas_SmSlaveJumpAtMaster(EnFloormas* self, GlobalContext* globalCtx) {
    Actor* primFloormas;

    SkelAnime_Update(&self->skelAnime);
    if (self->actor.parent->params == MERGE_MASTER) {
        primFloormas = self->actor.parent;
    } else if (self->actor.child->params == MERGE_MASTER) {
        primFloormas = self->actor.child;
    } else {
        if (self->actor.bgCheckFlags & 2) {
            self->actor.params = 0x10;
            EnFloormas_SetupLand(self);
        }
        return;
    }
    if (Animation_OnFrame(&self->skelAnime, 20.0f)) {
        self->actor.speedXZ = 5.0f;
        self->actor.velocity.y = 7.0f;
    } else if (self->skelAnime.curFrame < 20.0f) {
        Math_ApproachS(&self->actor.shape.rot.y, Actor_WorldYawTowardActor(&self->actor, primFloormas), 2, 0xE38);
    } else if ((((primFloormas->world.pos.y - self->actor.world.pos.y) < -10.0f) &&
                (fabsf(self->actor.world.pos.x - primFloormas->world.pos.x) < 10.0f)) &&
               (fabsf(self->actor.world.pos.z - primFloormas->world.pos.z) < 10.0f)) {
        EnFloormas_SetupSmWait(self);
        self->collider.base.ocFlags1 |= OC1_ON;
    } else if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_LAND);
        EnFloormas_SetupLand(self);
    }

    if (fabsf(self->actor.world.pos.x - primFloormas->world.pos.x) < 5.0f &&
        fabsf(self->actor.world.pos.z - primFloormas->world.pos.z) < 5.0f) {
        Math_StepToF(&self->actor.speedXZ, 0, 2.0f);
    }
}

void EnFloormas_Merge(EnFloormas* self, GlobalContext* globalCtx) {
    EnFloormas* parent;
    EnFloormas* child;
    s32 mergeCnt;
    f32 prevScale;
    f32 curScale;

    mergeCnt = 0;

    DECR(self->smActionTimer);

    parent = (EnFloormas*)self->actor.parent;
    child = (EnFloormas*)self->actor.child;

    if (self->smActionTimer == 0) {
        if (parent->actionFunc != EnFloormas_SmWait) {
            EnFloormas_SetupSmShrink(parent, globalCtx);
        }

        if (child->actionFunc != EnFloormas_SmWait) {
            EnFloormas_SetupSmShrink(child, globalCtx);
        }
    } else {
        if ((parent->actionFunc != EnFloormas_SmWait) && (parent->actionFunc != EnFloormas_SmShrink)) {
            mergeCnt++;
        }

        if ((child->actionFunc != EnFloormas_SmWait) && (child->actionFunc != EnFloormas_SmShrink)) {
            mergeCnt++;
        }
    }

    prevScale = self->actor.scale.x;

    if (mergeCnt == 1) {
        Math_StepToF(&self->actor.scale.x, 0.007f, 0.0005f);
    } else if (mergeCnt == 0) {
        Math_StepToF(&self->actor.scale.x, 0.01f, 0.0005f);
    }

    curScale = self->actor.scale.x;
    self->actor.scale.y = self->actor.scale.z = curScale;

    if (((prevScale == 0.007f) || (prevScale == 0.004f)) && (prevScale != self->actor.scale.x)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_EXPAND);
    }

    self->collider.dim.radius = (sCylinderInit.dim.radius * 100.0f) * self->actor.scale.x;
    self->collider.dim.height = (sCylinderInit.dim.height * 100.0f) * self->actor.scale.x;

    if (SkelAnime_Update(&self->skelAnime) != 0) {
        if (self->actor.scale.x >= 0.01f) {
            self->actor.flags &= ~0x10;
            EnFloormas_MakeVulnerable(self);
            self->actor.params = 0;
            self->collider.info.bumperFlags |= BUMP_HOOKABLE;
            self->actor.colChkInfo.health = sColChkInfoInit.health;
            EnFloormas_SetupStand(self);
        } else {
            if (self->actionTimer == 0) {
                Animation_PlayOnce(&self->skelAnime, &gFloormasterTapFingerAnim);
                self->actionTimer = 1;
            } else {
                Animation_PlayOnce(&self->skelAnime, &gWallmasterWaitAnim);
                self->actionTimer = 0;
            }
        }
    }
    func_8002F974(&self->actor, NA_SE_EN_FLOORMASTER_RESTORE - SFX_FLAG);
}

void EnFloormas_SmWait(EnFloormas* self, GlobalContext* globalCtx) {
}

void EnFloormas_TakeDamage(EnFloormas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        if (self->actor.colChkInfo.health == 0) {
            EnFloormas_Die(self, globalCtx);
        } else {
            EnFloormas_SetupRecover(self);
        }
    }

    if (Animation_OnFrame(&self->skelAnime, 13.0f)) {
        if (self->actor.scale.x > 0.004f) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        }
    }
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f);
}

void EnFloormas_Recover(EnFloormas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        EnFloormas_SetupStand(self);
    }
}

void EnFloormas_Freeze(EnFloormas* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (self->actionTimer == 0) {
        if (self->actor.colChkInfo.health == 0) {
            EnFloormas_Die(self, globalCtx);
            return;
        }
        EnFloormas_SetupStand(self);
    }
}

void EnFloormas_ColliderCheck(EnFloormas* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 isSmall;

    if ((self->collider.base.acFlags & AC_HIT) != 0) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);
        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (self->collider.base.colType != COLTYPE_HARD) {
                isSmall = 0;
                if (self->actor.scale.x < 0.01f) {
                    isSmall = 1;
                }
                if (isSmall && self->collider.info.acHitInfo->toucher.dmgFlags & 0x80) {
                    self->actor.colChkInfo.damage = 2;
                    self->actor.colChkInfo.damageEffect = 0;
                }
                if (Actor_ApplyDamage(&self->actor) == 0) {
                    if (isSmall) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLOORMASTER_SM_DEAD);
                    } else {
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_DEAD);
                    }
                    Enemy_StartFinishingBlow(globalCtx, &self->actor);
                    self->actor.flags &= ~1;
                } else if (self->actor.colChkInfo.damage != 0) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_DAMAGE);
                }

                if ((self->actor.colChkInfo.damageEffect == 4) || (self->actor.colChkInfo.damageEffect == 1)) {
                    if (self->actionFunc != EnFloormas_Freeze) {
                        EnFloormas_SetupFreeze(self);
                    }
                } else {
                    if (self->actor.colChkInfo.damageEffect == 2) {
                        EffectSsFCircle_Spawn(globalCtx, &self->actor, &self->actor.world.pos,
                                              self->actor.scale.x * 4000.0f, self->actor.scale.x * 4000.0f);
                    }
                    EnFloormas_SetupTakeDamage(self);
                }
            }
        }
    }
}

void EnFloormas_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnFloormas* self = THIS;
    s32 pad;

    if (self->actionFunc != EnFloormas_SmWait) {
        if (self->collider.base.atFlags & AT_HIT) {
            self->collider.base.atFlags &= ~AT_HIT;
            self->actor.speedXZ *= -0.5f;

            if (-5.0f < self->actor.speedXZ) {
                self->actor.speedXZ = -5.0f;
            } else {
                self->actor.speedXZ = self->actor.speedXZ;
            }

            self->actor.velocity.y = 5.0f;

            EnFloormas_SetupLand(self);
        }
        EnFloormas_ColliderCheck(self, globalCtx);
        self->actionFunc(self, globalCtx);

        if (self->actionFunc != EnFloormas_TakeDamage) {
            self->actor.world.rot.y = self->actor.shape.rot.y;
        }

        if (self->actionFunc != EnFloormas_GrabLink) {
            Actor_MoveForward(&self->actor);
        }

        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, self->actor.scale.x * 3000.0f, 0.0f, 0x1D);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        if (self->actionFunc == EnFloormas_Charge) {
            self->actor.flags |= 0x1000000;
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        if (self->actionFunc != EnFloormas_GrabLink) {
            if (self->actionFunc != EnFloormas_Split && self->actionFunc != EnFloormas_TakeDamage &&
                self->actor.freezeTimer == 0) {
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            }

            if ((self->actionFunc != EnFloormas_SmSlaveJumpAtMaster) || (self->skelAnime.curFrame < 20.0f)) {
                CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            }
        }

        Actor_SetFocus(&self->actor, self->actor.scale.x * 2500.0f);

        if (self->collider.base.colType == COLTYPE_HARD) {
            if (self->actionTarget != 0) {
                self->actionTarget--;
            }

            if (self->actionTarget == 0) {
                self->actionTarget = 0x28;
            }
        }
    }
}

s32 EnFloormas_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx, Gfx** gfx) {
    EnFloormas* self = THIS;
    if (limbIndex == 1) {
        pos->z += self->zOffset;
    }
    return false;
}

void EnFloormas_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    if (limbIndex == 2) {
        Matrix_Push();
        Matrix_Translate(1600.0f, -700.0f, -1700.0f, MTXMODE_APPLY);
        Matrix_RotateY(DEGTORAD(60.0f), MTXMODE_APPLY);
        Matrix_RotateZ(DEGTORAD(15.0f), MTXMODE_APPLY);
        Matrix_Scale(2.0f, 2.0f, 2.0f, MTXMODE_APPLY);
        gSPMatrix((*gfx)++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_floormas.c", 2299), G_MTX_LOAD);
        gSPDisplayList((*gfx)++, gWallmasterFingerDL);
        Matrix_Pop();
    }
}

static Color_RGBA8 sMergeColor = { 0, 255, 0, 0 };

void EnFloormas_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnFloormas* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_floormas.c", 2318);

    func_80093D18(globalCtx->state.gfxCtx);
    if (self->collider.base.colType == COLTYPE_HARD) {
        func_80026230(globalCtx, &sMergeColor, self->actionTarget % 0x28, 0x28);
    }

    POLY_OPA_DISP =
        SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                           EnFloormas_OverrideLimbDraw, EnFloormas_PostLimbDraw, self, POLY_OPA_DISP);
    if (self->collider.base.colType == COLTYPE_HARD) {
        func_80026608(globalCtx);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_floormas.c", 2340);
}

void EnFloormas_DrawHighlighted(Actor* thisx, GlobalContext* globalCtx) {
    EnFloormas* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_floormas.c", 2352);

    func_80093D84(globalCtx->state.gfxCtx);
    if (self->collider.base.colType == COLTYPE_HARD) {
        func_80026690(globalCtx, &sMergeColor, self->actionTarget % 0x28, 0x28);
    }
    POLY_XLU_DISP =
        SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                           EnFloormas_OverrideLimbDraw, EnFloormas_PostLimbDraw, self, POLY_XLU_DISP);
    if (self->collider.base.colType == COLTYPE_HARD) {
        func_80026A6C(globalCtx);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_floormas.c", 2374);
}
