#include "z_en_geldb.h"
#include "objects/object_geldb/object_geldb.h"

#define FLAGS 0x00000015

#define THIS ((EnGeldB*)thisx)

typedef enum {
    /*  0 */ GELDB_WAIT,
    /*  1 */ GELDB_DEFEAT,
    /*  2 */ GELDB_DAMAGED,
    /*  3 */ GELDB_JUMP,
    /*  4 */ GELDB_ROLL_BACK,
    /*  5 */ GELDB_READY,
    /*  6 */ GELDB_BLOCK,
    /*  7 */ GELDB_SLASH,
    /*  8 */ GELDB_ADVANCE,
    /*  9 */ GELDB_PIVOT,
    /* 10 */ GELDB_CIRCLE,
    /* 11 */ GELDB_UNUSED,
    /* 12 */ GELDB_SPIN_ATTACK,
    /* 13 */ GELDB_SIDESTEP,
    /* 14 */ GELDB_ROLL_FORWARD,
    /* 15 */ GELDB_STUNNED,
    /* 16 */ GELDB_SPIN_DODGE
} EnGeldBAction;

void EnGeldB_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGeldB_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGeldB_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGeldB_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 EnGeldB_DodgeRanged(GlobalContext* globalCtx, EnGeldB* self);

void EnGeldB_SetupWait(EnGeldB* self);
void EnGeldB_SetupReady(EnGeldB* self);
void EnGeldB_SetupAdvance(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_SetupPivot(EnGeldB* self);
void EnGeldB_SetupRollForward(EnGeldB* self);
void EnGeldB_SetupCircle(EnGeldB* self);
void EnGeldB_SetupSpinDodge(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_SetupSlash(EnGeldB* self);
void EnGeldB_SetupSpinAttack(EnGeldB* self);
void EnGeldB_SetupRollBack(EnGeldB* self);
void EnGeldB_SetupJump(EnGeldB* self);
void EnGeldB_SetupBlock(EnGeldB* self);
void EnGeldB_SetupSidestep(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_SetupDefeated(EnGeldB* self);

void EnGeldB_Wait(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Flee(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Ready(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Advance(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_RollForward(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Pivot(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Circle(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_SpinDodge(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Slash(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_SpinAttack(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_RollBack(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Stunned(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Damaged(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Jump(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Block(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Sidestep(EnGeldB* self, GlobalContext* globalCtx);
void EnGeldB_Defeated(EnGeldB* self, GlobalContext* globalCtx);

const ActorInit En_GeldB_InitVars = {
    ACTOR_EN_GELDB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GELDB,
    sizeof(EnGeldB),
    (ActorFunc)EnGeldB_Init,
    (ActorFunc)EnGeldB_Destroy,
    (ActorFunc)EnGeldB_Update,
    (ActorFunc)EnGeldB_Draw,
};

static ColliderCylinderInit sBodyCylInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 50, 0, { 0, 0, 0 } },
};

static ColliderTrisElementInit sBlockTrisElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK2,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFC1FFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { -10.0f, 14.0f, 2.0f }, { -10.0f, -6.0f, 2.0f }, { 9.0f, 14.0f, 2.0f } } },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFC1FFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { -10.0f, -6.0f, 2.0f }, { 9.0f, -6.0f, 2.0f }, { 9.0f, 14.0f, 2.0f } } },
    },
};

static ColliderTrisInit sBlockTrisInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    sBlockTrisElementsInit,
};

static ColliderQuadInit sSwordQuadInit = {
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
        TOUCH_ON | TOUCH_SFX_NORMAL | TOUCH_UNK7,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum {
    /* 0x0 */ GELDB_DMG_NORMAL,
    /* 0x1 */ GELDB_DMG_STUN,
    /* 0x6 */ GELDB_DMG_UNK_6 = 0x6,
    /* 0xD */ GELDB_DMG_UNK_D = 0xD,
    /* 0xE */ GELDB_DMG_UNK_E,
    /* 0xF */ GELDB_DMG_FREEZE
} EnGeldBDamageEffects;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, GELDB_DMG_STUN),
    /* Deku stick    */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Slingshot     */ DMG_ENTRY(1, GELDB_DMG_NORMAL),
    /* Explosive     */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Boomerang     */ DMG_ENTRY(0, GELDB_DMG_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Hammer swing  */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Hookshot      */ DMG_ENTRY(0, GELDB_DMG_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, GELDB_DMG_NORMAL),
    /* Master sword  */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Giant's Knife */ DMG_ENTRY(4, GELDB_DMG_NORMAL),
    /* Fire arrow    */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Ice arrow     */ DMG_ENTRY(2, GELDB_DMG_FREEZE),
    /* Light arrow   */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Unk arrow 1   */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Unk arrow 2   */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Unk arrow 3   */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Fire magic    */ DMG_ENTRY(4, GELDB_DMG_UNK_E),
    /* Ice magic     */ DMG_ENTRY(0, GELDB_DMG_UNK_6),
    /* Light magic   */ DMG_ENTRY(3, GELDB_DMG_UNK_D),
    /* Shield        */ DMG_ENTRY(0, GELDB_DMG_NORMAL),
    /* Mirror Ray    */ DMG_ENTRY(0, GELDB_DMG_NORMAL),
    /* Kokiri spin   */ DMG_ENTRY(1, GELDB_DMG_NORMAL),
    /* Giant spin    */ DMG_ENTRY(4, GELDB_DMG_NORMAL),
    /* Master spin   */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Kokiri jump   */ DMG_ENTRY(2, GELDB_DMG_NORMAL),
    /* Giant jump    */ DMG_ENTRY(8, GELDB_DMG_NORMAL),
    /* Master jump   */ DMG_ENTRY(4, GELDB_DMG_NORMAL),
    /* Unknown 1     */ DMG_ENTRY(4, GELDB_DMG_NORMAL),
    /* Unblockable   */ DMG_ENTRY(0, GELDB_DMG_NORMAL),
    /* Hammer jump   */ DMG_ENTRY(4, GELDB_DMG_NORMAL),
    /* Unknown 2     */ DMG_ENTRY(0, GELDB_DMG_NORMAL),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3000, ICHAIN_STOP),
};

static Vec3f sUnusedOffset = { 1100.0f, -700.0f, 0.0f };

void EnGeldB_SetupAction(EnGeldB* self, EnGeldBActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnGeldB_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EffectBlureInit1 blureInit;
    EnGeldB* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    thisx->colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawFeet, 0.0f);
    self->actor.colChkInfo.mass = MASS_HEAVY;
    thisx->colChkInfo.health = 20;
    thisx->colChkInfo.cylRadius = 50;
    thisx->colChkInfo.cylHeight = 100;
    thisx->naviEnemyId = 0x54;
    self->keyFlag = thisx->params & 0xFF00;
    thisx->params &= 0xFF;
    self->blinkState = 0;
    self->unkFloat = 10.0f;
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGerudoRedSkel, &gGerudoRedNeutralAnim, self->jointTable,
                       self->morphTable, GELDB_LIMB_MAX);
    Collider_InitCylinder(globalCtx, &self->bodyCollider);
    Collider_SetCylinder(globalCtx, &self->bodyCollider, thisx, &sBodyCylInit);
    Collider_InitTris(globalCtx, &self->blockCollider);
    Collider_SetTris(globalCtx, &self->blockCollider, thisx, &sBlockTrisInit, self->blockElements);
    Collider_InitQuad(globalCtx, &self->swordCollider);
    Collider_SetQuad(globalCtx, &self->swordCollider, thisx, &sSwordQuadInit);
    blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p1StartColor[2] = blureInit.p1StartColor[3] =
        blureInit.p2StartColor[0] = blureInit.p2StartColor[1] = blureInit.p2StartColor[2] = blureInit.p1EndColor[0] =
            blureInit.p1EndColor[1] = blureInit.p1EndColor[2] = blureInit.p2EndColor[0] = blureInit.p2EndColor[1] =
                blureInit.p2EndColor[2] = 255;
    blureInit.p2StartColor[3] = 64;
    blureInit.p1EndColor[3] = blureInit.p2EndColor[3] = 0;
    blureInit.elemDuration = 8;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 2;

    Effect_Add(globalCtx, &self->blureIndex, EFFECT_BLURE1, 0, 0, &blureInit);
    Actor_SetScale(thisx, 0.012499999f);
    EnGeldB_SetupWait(self);
    if ((self->keyFlag != 0) && Flags_GetCollectible(globalCtx, self->keyFlag >> 8)) {
        Actor_Kill(thisx);
    }
}

void EnGeldB_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnGeldB* self = THIS;

    func_800F5B58();
    Effect_Delete(globalCtx, self->blureIndex);
    Collider_DestroyTris(globalCtx, &self->blockCollider);
    Collider_DestroyCylinder(globalCtx, &self->bodyCollider);
    Collider_DestroyQuad(globalCtx, &self->swordCollider);
}

s32 EnGeldB_ReactToPlayer(GlobalContext* globalCtx, EnGeldB* self, s16 arg2) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* thisx = &self->actor;
    s16 angleToWall;
    s16 angleToLink;
    Actor* bomb;

    angleToWall = thisx->wallYaw - thisx->shape.rot.y;
    angleToWall = ABS(angleToWall);
    angleToLink = thisx->yawTowardsPlayer - thisx->shape.rot.y;
    angleToLink = ABS(angleToLink);

    if (func_800354B4(globalCtx, thisx, 100.0f, 0x2710, 0x3E80, thisx->shape.rot.y)) {
        if (player->swordAnimation == 0x11) {
            EnGeldB_SetupSpinDodge(self, globalCtx);
            return true;
        } else if (globalCtx->gameplayFrames & 1) {
            EnGeldB_SetupBlock(self);
            return true;
        }
    }
    if (func_800354B4(globalCtx, thisx, 100.0f, 0x5DC0, 0x2AA8, thisx->shape.rot.y)) {
        thisx->shape.rot.y = thisx->world.rot.y = thisx->yawTowardsPlayer;
        if ((thisx->bgCheckFlags & 8) && (ABS(angleToWall) < 0x2EE0) && (thisx->xzDistToPlayer < 90.0f)) {
            EnGeldB_SetupJump(self);
            return true;
        } else if (player->swordAnimation == 0x11) {
            EnGeldB_SetupSpinDodge(self, globalCtx);
            return true;
        } else if ((thisx->xzDistToPlayer < 90.0f) && (globalCtx->gameplayFrames & 1)) {
            EnGeldB_SetupBlock(self);
            return true;
        } else {
            EnGeldB_SetupRollBack(self);
            return true;
        }
    } else if ((bomb = Actor_FindNearby(globalCtx, thisx, -1, ACTORCAT_EXPLOSIVE, 80.0f)) != NULL) {
        thisx->shape.rot.y = thisx->world.rot.y = thisx->yawTowardsPlayer;
        if (((thisx->bgCheckFlags & 8) && (angleToWall < 0x2EE0)) || (bomb->id == ACTOR_EN_BOM_CHU)) {
            if ((bomb->id == ACTOR_EN_BOM_CHU) && (Actor_WorldDistXYZToActor(thisx, bomb) < 80.0f) &&
                ((s16)(thisx->shape.rot.y - (bomb->world.rot.y - 0x8000)) < 0x3E80)) {
                EnGeldB_SetupJump(self);
                return true;
            } else {
                EnGeldB_SetupSidestep(self, globalCtx);
                return true;
            }
        } else {
            EnGeldB_SetupRollBack(self);
            return true;
        }
    } else if (arg2) {
        if (angleToLink >= 0x1B58) {
            EnGeldB_SetupSidestep(self, globalCtx);
            return true;
        } else {
            s16 angleToFacingLink = player->actor.shape.rot.y - thisx->shape.rot.y;

            if ((thisx->xzDistToPlayer <= 45.0f) && !Actor_OtherIsTargeted(globalCtx, thisx) &&
                ((globalCtx->gameplayFrames & 7) || (ABS(angleToFacingLink) < 0x38E0))) {
                EnGeldB_SetupSlash(self);
                return true;
            } else {
                EnGeldB_SetupCircle(self);
                return true;
            }
        }
    }
    return false;
}

void EnGeldB_SetupWait(EnGeldB* self) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, &gGerudoRedJumpAnim, 0.0f);
    self->actor.world.pos.y = self->actor.home.pos.y + 120.0f;
    self->timer = 10;
    self->invisible = true;
    self->action = GELDB_WAIT;
    self->actor.bgCheckFlags &= ~3;
    self->actor.gravity = -2.0f;
    self->actor.flags &= ~1;
    EnGeldB_SetupAction(self, EnGeldB_Wait);
}

void EnGeldB_Wait(EnGeldB* self, GlobalContext* globalCtx) {
    if ((self->invisible && !Flags_GetSwitch(globalCtx, self->actor.home.rot.z)) ||
        self->actor.xzDistToPlayer > 300.0f) {
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.world.pos.y = self->actor.floorHeight + 120.0f;
    } else {
        self->invisible = false;
        self->actor.shape.shadowScale = 90.0f;
        func_800F5ACC(0x38);
    }
    if (self->actor.bgCheckFlags & 2) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_DOWN);
        self->skelAnime.playSpeed = 1.0f;
        self->actor.world.pos.y = self->actor.floorHeight;
        self->actor.flags |= 1;
        self->actor.focus.pos = self->actor.world.pos;
        self->actor.bgCheckFlags &= ~2;
        self->actor.velocity.y = 0.0f;
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->leftFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->rightFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
    }
    if (SkelAnime_Update(&self->skelAnime)) {
        EnGeldB_SetupReady(self);
    }
}

void EnGeldB_SetupFlee(EnGeldB* self) {
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedJumpAnim);

    Animation_Change(&self->skelAnime, &gGerudoRedJumpAnim, -2.0f, lastFrame, 0.0f, ANIMMODE_ONCE_INTERP, -4.0f);
    self->timer = 20;
    self->invisible = false;
    self->action = GELDB_WAIT;
    self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    EnGeldB_SetupAction(self, EnGeldB_Flee);
}

void EnGeldB_Flee(EnGeldB* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame == 10.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    }
    if (self->skelAnime.curFrame == 2.0f) {
        self->actor.gravity = 0.0f;
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->leftFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->rightFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
    }
    if (SkelAnime_Update(&self->skelAnime)) {
        Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.floorHeight + 300.0f, 1.0f, 20.5f, 0.0f);
        self->timer--;
        if (self->timer == 0) {
            Actor_Kill(&self->actor);
        }
    }
}

void EnGeldB_SetupReady(EnGeldB* self) {
    Animation_MorphToLoop(&self->skelAnime, &gGerudoRedNeutralAnim, -4.0f);
    self->action = GELDB_READY;
    self->timer = Rand_ZeroOne() * 10.0f + 5.0f;
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnGeldB_SetupAction(self, EnGeldB_Ready);
}

void EnGeldB_Ready(EnGeldB* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    s16 angleToLink;

    SkelAnime_Update(&self->skelAnime);
    if (self->lookTimer != 0) {
        angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y - self->headRot.y;
        if (ABS(angleToLink) > 0x2000) {
            self->lookTimer--;
            return;
        }
        self->lookTimer = 0;
    }
    angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (!EnGeldB_DodgeRanged(globalCtx, self)) {
        if (self->unkTimer != 0) {
            self->unkTimer--;

            if (ABS(angleToLink) >= 0x1FFE) {
                return;
            }
            self->unkTimer = 0;
        } else if (EnGeldB_ReactToPlayer(globalCtx, self, 0)) {
            return;
        }
        angleToLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
        if ((self->actor.xzDistToPlayer < 100.0f) && (player->swordState != 0) && (ABS(angleToLink) >= 0x1F40)) {
            self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            EnGeldB_SetupCircle(self);
        } else if (--self->timer == 0) {
            if (Actor_IsFacingPlayer(&self->actor, 30 * 0x10000 / 360)) {
                if ((210.0f > self->actor.xzDistToPlayer) && (self->actor.xzDistToPlayer > 150.0f) &&
                    (Rand_ZeroOne() < 0.3f)) {
                    if (Actor_OtherIsTargeted(globalCtx, &self->actor) || (Rand_ZeroOne() > 0.5f) ||
                        (ABS(angleToLink) < 0x38E0)) {
                        EnGeldB_SetupRollForward(self);
                    } else {
                        EnGeldB_SetupSpinAttack(self);
                    }
                } else if (Rand_ZeroOne() > 0.3f) {
                    EnGeldB_SetupAdvance(self, globalCtx);
                } else {
                    EnGeldB_SetupCircle(self);
                }
            } else {
                EnGeldB_SetupPivot(self);
            }
            if ((globalCtx->gameplayFrames & 0x5F) == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_BREATH);
            }
        }
    }
}

void EnGeldB_SetupAdvance(EnGeldB* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedWalkAnim);

    Animation_Change(&self->skelAnime, &gGerudoRedWalkAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, -4.0f);
    self->action = GELDB_ADVANCE;
    EnGeldB_SetupAction(self, EnGeldB_Advance);
}

void EnGeldB_Advance(EnGeldB* self, GlobalContext* globalCtx) {
    s32 thisKeyFrame;
    s32 prevKeyFrame;
    s32 pad3C;
    s16 facingAngletoLink;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad30;
    s32 pad2C;
    f32 playSpeed;

    if (!EnGeldB_DodgeRanged(globalCtx, self)) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (self->actor.xzDistToPlayer <= 40.0f) {
            Math_SmoothStepToF(&self->actor.speedXZ, -8.0f, 1.0f, 1.5f, 0.0f);
        } else if (self->actor.xzDistToPlayer > 55.0f) {
            Math_SmoothStepToF(&self->actor.speedXZ, 8.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 6.65f, 0.0f);
        }
        self->skelAnime.playSpeed = self->actor.speedXZ / 8.0f;
        facingAngletoLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
        facingAngletoLink = ABS(facingAngletoLink);
        if ((self->actor.xzDistToPlayer < 150.0f) && (player->swordState != 0) && (facingAngletoLink >= 0x1F40)) {
            self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnGeldB_SetupCircle(self);
                return;
            }
        }
        thisKeyFrame = (s32)self->skelAnime.curFrame;
        SkelAnime_Update(&self->skelAnime);
        playSpeed = ABS(self->skelAnime.playSpeed);
        prevKeyFrame = (s32)(self->skelAnime.curFrame - playSpeed);
        playSpeed = ABS(self->skelAnime.playSpeed); // yes it does self twice.
        if (!Actor_IsFacingPlayer(&self->actor, 0x11C7)) {
            if (Rand_ZeroOne() > 0.5f) {
                EnGeldB_SetupCircle(self);
            } else {
                EnGeldB_SetupReady(self);
            }
        } else if (self->actor.xzDistToPlayer < 90.0f) {
            if (!Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                (Rand_ZeroOne() > 0.03f || (self->actor.xzDistToPlayer <= 45.0f && facingAngletoLink < 0x38E0))) {
                EnGeldB_SetupSlash(self);
            } else if (Actor_OtherIsTargeted(globalCtx, &self->actor) && (Rand_ZeroOne() > 0.5f)) {
                EnGeldB_SetupRollBack(self);
            } else {
                EnGeldB_SetupCircle(self);
            }
        }
        if (!EnGeldB_ReactToPlayer(globalCtx, self, 0)) {
            if ((210.0f > self->actor.xzDistToPlayer) && (self->actor.xzDistToPlayer > 150.0f) &&
                Actor_IsFacingPlayer(&self->actor, 0x71C)) {
                if (Actor_IsTargeted(globalCtx, &self->actor)) {
                    if (Rand_ZeroOne() > 0.5f) {
                        EnGeldB_SetupRollForward(self);
                    } else {
                        EnGeldB_SetupSpinAttack(self);
                    }
                } else {
                    EnGeldB_SetupCircle(self);
                    return;
                }
            }
            if ((globalCtx->gameplayFrames & 0x5F) == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_BREATH);
            }
            if ((thisKeyFrame != (s32)self->skelAnime.curFrame) &&
                ((prevKeyFrame < 0 && (s32)playSpeed + thisKeyFrame > 0) ||
                 (prevKeyFrame < 4 && (s32)playSpeed + thisKeyFrame > 4))) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_MUSI_LAND);
            }
        }
    }
}

void EnGeldB_SetupRollForward(EnGeldB* self) {
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedFlipAnim);

    Animation_Change(&self->skelAnime, &gGerudoRedFlipAnim, -1.0f, lastFrame, 0.0f, ANIMMODE_ONCE, -3.0f);
    self->timer = 0;
    self->invisible = true;
    self->action = GELDB_ROLL_FORWARD;
    self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    self->actor.speedXZ = 10.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    EnGeldB_SetupAction(self, EnGeldB_RollForward);
}

void EnGeldB_RollForward(EnGeldB* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 facingAngleToLink = player->actor.shape.rot.y - self->actor.shape.rot.y;

    if (SkelAnime_Update(&self->skelAnime)) {
        self->invisible = false;
        self->actor.speedXZ = 0.0f;
        if (!Actor_IsFacingPlayer(&self->actor, 0x1554)) {
            EnGeldB_SetupReady(self);
            self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
            if (ABS(facingAngleToLink) < 0x38E0) {
                self->lookTimer = 20;
            }
        } else if (!Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                   (Rand_ZeroOne() > 0.5f || (ABS(facingAngleToLink) < 0x3FFC))) {
            EnGeldB_SetupSlash(self);
        } else {
            EnGeldB_SetupAdvance(self, globalCtx);
        }
    }
    if ((globalCtx->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_BREATH);
    }
}

void EnGeldB_SetupPivot(EnGeldB* self) {
    Animation_MorphToLoop(&self->skelAnime, &gGerudoRedSidestepAnim, -4.0f);
    self->action = GELDB_PIVOT;
    EnGeldB_SetupAction(self, EnGeldB_Pivot);
}

void EnGeldB_Pivot(EnGeldB* self, GlobalContext* globalCtx) {
    s16 angleToLink;
    s16 turnRate;
    f32 playSpeed;

    if (!EnGeldB_DodgeRanged(globalCtx, self) && !EnGeldB_ReactToPlayer(globalCtx, self, 0)) {
        angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        turnRate = (angleToLink > 0) ? ((angleToLink * 0.25f) + 2000.0f) : ((angleToLink * 0.25f) - 2000.0f);
        self->actor.world.rot.y = self->actor.shape.rot.y += turnRate;
        if (angleToLink > 0) {
            playSpeed = turnRate * 0.5f;
            playSpeed = CLAMP_MAX(playSpeed, 1.0f);
        } else {
            playSpeed = turnRate * 0.5f;
            playSpeed = CLAMP_MIN(playSpeed, -1.0f);
        }
        self->skelAnime.playSpeed = -playSpeed;
        SkelAnime_Update(&self->skelAnime);
        if (Actor_IsFacingPlayer(&self->actor, 30 * 0x10000 / 360)) {
            if (Rand_ZeroOne() > 0.8f) {
                EnGeldB_SetupCircle(self);
            } else {
                EnGeldB_SetupAdvance(self, globalCtx);
            }
        }
        if ((globalCtx->gameplayFrames & 0x5F) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_BREATH);
        }
    }
}

void EnGeldB_SetupCircle(EnGeldB* self) {
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedSidestepAnim);

    Animation_Change(&self->skelAnime, &gGerudoRedSidestepAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, 0.0f);
    self->actor.speedXZ = Rand_CenteredFloat(12.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->skelAnime.playSpeed = -self->actor.speedXZ * 0.5f;
    self->timer = Rand_ZeroOne() * 30.0f + 30.0f;
    self->action = GELDB_CIRCLE;
    self->approachRate = 0.0f;
    EnGeldB_SetupAction(self, EnGeldB_Circle);
}

void EnGeldB_Circle(EnGeldB* self, GlobalContext* globalCtx) {
    s16 angleBehindLink;
    s16 phi_v1;
    s32 nextKeyFrame;
    s32 thisKeyFrame;
    s32 pad;
    s32 prevKeyFrame;
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (!EnGeldB_DodgeRanged(globalCtx, self) && !EnGeldB_ReactToPlayer(globalCtx, self, 0)) {
        self->actor.world.rot.y = self->actor.shape.rot.y + 0x3A98;
        angleBehindLink = player->actor.shape.rot.y + 0x8000;
        if (Math_SinS(angleBehindLink - self->actor.shape.rot.y) >= 0.0f) {
            self->actor.speedXZ -= 0.25f;
            if (self->actor.speedXZ < -8.0f) {
                self->actor.speedXZ = -8.0f;
            }
        } else if (Math_SinS(angleBehindLink - self->actor.shape.rot.y) < 0.0f) {
            self->actor.speedXZ += 0.25f;
            if (self->actor.speedXZ > 8.0f) {
                self->actor.speedXZ = 8.0f;
            }
        }
        if ((self->actor.bgCheckFlags & 8) || !Actor_TestFloorInDirection(&self->actor, globalCtx, self->actor.speedXZ,
                                                                          self->actor.shape.rot.y + 0x3E80)) {
            if (self->actor.bgCheckFlags & 8) {
                if (self->actor.speedXZ >= 0.0f) {
                    phi_v1 = self->actor.shape.rot.y + 0x3E80;
                } else {
                    phi_v1 = self->actor.shape.rot.y - 0x3E80;
                }
                phi_v1 = self->actor.wallYaw - phi_v1;
            } else {
                self->actor.speedXZ *= -0.8f;
                phi_v1 = 0;
            }
            if (ABS(phi_v1) > 0x4000) {
                self->actor.speedXZ *= -0.8f;
                if (self->actor.speedXZ < 0.0f) {
                    self->actor.speedXZ -= 0.5f;
                } else {
                    self->actor.speedXZ += 0.5f;
                }
            }
        }
        if (self->actor.xzDistToPlayer <= 45.0f) {
            Math_SmoothStepToF(&self->approachRate, -4.0f, 1.0f, 1.5f, 0.0f);
        } else if (self->actor.xzDistToPlayer > 40.0f) {
            Math_SmoothStepToF(&self->approachRate, 4.0f, 1.0f, 1.5f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->approachRate, 0.0f, 1.0f, 6.65f, 0.0f);
        }
        if (self->approachRate != 0.0f) {
            self->actor.world.pos.x += Math_SinS(self->actor.shape.rot.y) * self->approachRate;
            self->actor.world.pos.z += Math_CosS(self->actor.shape.rot.y) * self->approachRate;
        }
        if (ABS(self->approachRate) < ABS(self->actor.speedXZ)) {
            self->skelAnime.playSpeed = -self->actor.speedXZ * 0.5f;
        } else {
            self->skelAnime.playSpeed = -self->approachRate * 0.5f;
        }
        self->skelAnime.playSpeed = CLAMP(self->skelAnime.playSpeed, -3.0f, 3.0f);

        thisKeyFrame = self->skelAnime.curFrame;
        SkelAnime_Update(&self->skelAnime);

        prevKeyFrame = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
        nextKeyFrame = (s32)ABS(self->skelAnime.playSpeed) + thisKeyFrame;
        if ((thisKeyFrame != (s32)self->skelAnime.curFrame) &&
            ((prevKeyFrame < 0 && 0 < nextKeyFrame) || (prevKeyFrame < 5 && 5 < nextKeyFrame))) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MUSI_LAND);
        }
        if ((globalCtx->gameplayFrames & 0x5F) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_BREATH);
        }
        if ((Math_CosS(angleBehindLink - self->actor.shape.rot.y) < -0.85f) &&
            !Actor_OtherIsTargeted(globalCtx, &self->actor) && (self->actor.xzDistToPlayer <= 45.0f)) {
            EnGeldB_SetupSlash(self);
        } else if (--self->timer == 0) {
            if (Actor_OtherIsTargeted(globalCtx, &self->actor) && (Rand_ZeroOne() > 0.5f)) {
                EnGeldB_SetupRollBack(self);
            } else {
                EnGeldB_SetupReady(self);
            }
        }
    }
}

void EnGeldB_SetupSpinDodge(EnGeldB* self, GlobalContext* globalCtx) {
    s16 sp3E;
    Player* player = GET_PLAYER(globalCtx);
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedSidestepAnim);

    Animation_Change(&self->skelAnime, &gGerudoRedSidestepAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, 0.0f);
    sp3E = player->actor.shape.rot.y;
    if (Math_SinS(sp3E - self->actor.shape.rot.y) > 0.0f) {
        self->actor.speedXZ = -10.0f;
    } else if (Math_SinS(sp3E - self->actor.shape.rot.y) < 0.0f) {
        self->actor.speedXZ = 10.0f;
    } else if (Rand_ZeroOne() > 0.5f) {
        self->actor.speedXZ = 10.0f;
    } else {
        self->actor.speedXZ = -10.0f;
    }
    self->skelAnime.playSpeed = -self->actor.speedXZ * 0.5f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->timer = 6;
    self->approachRate = 0.0f;
    self->unkFloat = 0.0f;
    self->action = GELDB_SPIN_DODGE;

    EnGeldB_SetupAction(self, EnGeldB_SpinDodge);
}

void EnGeldB_SpinDodge(EnGeldB* self, GlobalContext* globalCtx) {
    s16 phi_v1;
    s32 thisKeyFrame;
    s32 pad;
    s32 lastKeyFrame;
    s32 nextKeyFrame;

    self->actor.world.rot.y = self->actor.yawTowardsPlayer + 0x3A98;
    if ((self->actor.bgCheckFlags & 8) ||
        !Actor_TestFloorInDirection(&self->actor, globalCtx, self->actor.speedXZ, self->actor.shape.rot.y + 0x3E80)) {
        if (self->actor.bgCheckFlags & 8) {
            if (self->actor.speedXZ >= 0.0f) {
                phi_v1 = self->actor.shape.rot.y + 0x3E80;
            } else {
                phi_v1 = self->actor.shape.rot.y - 0x3E80;
            }
            phi_v1 = self->actor.wallYaw - phi_v1;
        } else {
            self->actor.speedXZ *= -0.8f;
            phi_v1 = 0;
        }
        if (ABS(phi_v1) > 0x4000) {
            EnGeldB_SetupJump(self);
            return;
        }
    }
    if (self->actor.xzDistToPlayer <= 45.0f) {
        Math_SmoothStepToF(&self->approachRate, -4.0f, 1.0f, 1.5f, 0.0f);
    } else if (self->actor.xzDistToPlayer > 40.0f) {
        Math_SmoothStepToF(&self->approachRate, 4.0f, 1.0f, 1.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->approachRate, 0.0f, 1.0f, 6.65f, 0.0f);
    }
    if (self->approachRate != 0.0f) {
        self->actor.world.pos.x += Math_SinS(self->actor.yawTowardsPlayer) * self->approachRate;
        self->actor.world.pos.z += Math_CosS(self->actor.yawTowardsPlayer) * self->approachRate;
    }
    if (ABS(self->approachRate) < ABS(self->actor.speedXZ)) {
        self->skelAnime.playSpeed = -self->actor.speedXZ * 0.5f;
    } else {
        self->skelAnime.playSpeed = -self->approachRate * 0.5f;
    }
    self->skelAnime.playSpeed = CLAMP(self->skelAnime.playSpeed, -3.0f, 3.0f);
    thisKeyFrame = self->skelAnime.curFrame;
    SkelAnime_Update(&self->skelAnime);
    lastKeyFrame = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);
    nextKeyFrame = (s32)ABS(self->skelAnime.playSpeed) + thisKeyFrame;
    if ((thisKeyFrame != (s32)self->skelAnime.curFrame) &&
        ((lastKeyFrame < 0 && 0 < nextKeyFrame) || (lastKeyFrame < 5 && 5 < nextKeyFrame))) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_MUSI_LAND);
    }
    if ((globalCtx->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_BREATH);
    }
    self->timer--;
    if (self->timer == 0) {
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        if (!EnGeldB_DodgeRanged(globalCtx, self)) {
            if (!Actor_OtherIsTargeted(globalCtx, &self->actor) && (self->actor.xzDistToPlayer <= 70.0f)) {
                EnGeldB_SetupSlash(self);
            } else {
                EnGeldB_SetupRollBack(self);
            }
        }
    } else {
        if (self->actor.speedXZ >= 0.0f) {
            self->actor.shape.rot.y += 0x4000;
        } else {
            self->actor.shape.rot.y -= 0x4000;
        }
    }
}

void EnGeldB_SetupSlash(EnGeldB* self) {
    Animation_PlayOnce(&self->skelAnime, &gGerudoRedSlashAnim);
    self->swordCollider.base.atFlags &= ~AT_BOUNCED;
    self->action = GELDB_SLASH;
    self->spinAttackState = 0;
    self->actor.speedXZ = 0.0f;
    func_800F8A44(&self->actor.projectedPos, NA_SE_EN_GERUDOFT_BREATH);
    EnGeldB_SetupAction(self, EnGeldB_Slash);
}

void EnGeldB_Slash(EnGeldB* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 angleFacingLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
    s16 angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    angleFacingLink = ABS(angleFacingLink);
    angleToLink = ABS(angleToLink);

    self->actor.speedXZ = 0.0f;
    if ((s32)self->skelAnime.curFrame == 1) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_ATTACK);
        self->swordState = 1;
    } else if ((s32)self->skelAnime.curFrame == 6) {
        self->swordState = -1;
    }
    if (self->swordCollider.base.atFlags & AT_BOUNCED) {
        self->swordState = -1;
        self->swordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        EnGeldB_SetupRollBack(self);
    } else if (SkelAnime_Update(&self->skelAnime)) {
        if (!Actor_IsFacingPlayer(&self->actor, 0x1554)) {
            EnGeldB_SetupReady(self);
            self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
            if (angleToLink > 0x4000) {
                self->lookTimer = 20;
            }
        } else if (Rand_ZeroOne() > 0.7f || (self->actor.xzDistToPlayer >= 120.0f)) {
            EnGeldB_SetupReady(self);
            self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
        } else {
            self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnGeldB_SetupSidestep(self, globalCtx);
            } else if (angleFacingLink <= 0x2710) {
                if (angleToLink > 0x3E80) {
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    EnGeldB_SetupCircle(self);
                } else {
                    EnGeldB_ReactToPlayer(globalCtx, self, 1);
                }
            } else {
                EnGeldB_SetupCircle(self);
            }
        }
    }
}

void EnGeldB_SetupSpinAttack(EnGeldB* self) {
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedSpinAttackAnim);

    Animation_Change(&self->skelAnime, &gGerudoRedSpinAttackAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE_INTERP, 0.0f);
    self->swordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
    self->action = GELDB_SPIN_ATTACK;
    self->spinAttackState = 0;
    self->actor.speedXZ = 0.0f;
    EnGeldB_SetupAction(self, EnGeldB_SpinAttack);
}

void EnGeldB_SpinAttack(EnGeldB* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 angleFacingLink;
    s16 angleToLink;

    if (self->spinAttackState < 2) {
        if (self->swordCollider.base.atFlags & AT_BOUNCED) {
            self->swordCollider.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
            self->spinAttackState = 1;
            self->skelAnime.playSpeed = 1.5f;
        } else if (self->swordCollider.base.atFlags & AT_HIT) {
            self->swordCollider.base.atFlags &= ~AT_HIT;
            if (&player->actor == self->swordCollider.base.at) {
                func_8002F71C(globalCtx, &self->actor, 6.0f, self->actor.yawTowardsPlayer, 6.0f);
                self->spinAttackState = 2;
                func_8002DF54(globalCtx, &self->actor, 0x18);
                func_8010B680(globalCtx, 0x6003, &self->actor);
                self->timer = 30;
                self->actor.speedXZ = 0.0f;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_TWINROBA_YOUNG_LAUGH);
                return;
            }
        }
    }
    if ((s32)self->skelAnime.curFrame < 9) {
        self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    } else if ((s32)self->skelAnime.curFrame == 13) {
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->leftFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->rightFootPos, 3.0f, 2, 2.0f, 0, 0, 0);
        self->swordState = 1;
        self->actor.speedXZ = 10.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_ATTACK);
    } else if ((s32)self->skelAnime.curFrame == 21) {
        self->actor.speedXZ = 0.0f;
    } else if ((s32)self->skelAnime.curFrame == 24) {
        self->swordState = -1;
    }
    if (SkelAnime_Update(&self->skelAnime) && (self->spinAttackState < 2)) {
        if (!Actor_IsFacingPlayer(&self->actor, 0x1554)) {
            EnGeldB_SetupReady(self);
            self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
            self->lookTimer = 46;
        } else if (self->spinAttackState != 0) {
            EnGeldB_SetupRollBack(self);
        } else if (Rand_ZeroOne() > 0.7f || (self->actor.xzDistToPlayer >= 120.0f)) {
            EnGeldB_SetupReady(self);
            self->timer = (Rand_ZeroOne() * 5.0f) + 5.0f;
        } else {
            self->actor.world.rot.y = self->actor.yawTowardsPlayer;
            if (Rand_ZeroOne() > 0.7f) {
                EnGeldB_SetupSidestep(self, globalCtx);
            } else {
                angleFacingLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
                angleFacingLink = ABS(angleFacingLink);
                if (angleFacingLink <= 0x2710) {
                    angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
                    angleToLink = ABS(angleToLink);
                    if (angleToLink > 0x3E80) {
                        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                        EnGeldB_SetupCircle(self);
                    } else {
                        EnGeldB_ReactToPlayer(globalCtx, self, 1);
                    }
                } else {
                    EnGeldB_SetupCircle(self);
                }
            }
        }
    }
}

void EnGeldB_SetupRollBack(EnGeldB* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gGerudoRedFlipAnim, -3.0f);
    self->timer = 0;
    self->invisible = true;
    self->action = GELDB_ROLL_BACK;
    self->actor.speedXZ = -8.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    EnGeldB_SetupAction(self, EnGeldB_RollBack);
}

void EnGeldB_RollBack(EnGeldB* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if (!Actor_OtherIsTargeted(globalCtx, &self->actor) && (self->actor.xzDistToPlayer < 170.0f) &&
            (self->actor.xzDistToPlayer > 140.0f) && (Rand_ZeroOne() < 0.2f)) {
            EnGeldB_SetupSpinAttack(self);
        } else if (globalCtx->gameplayFrames & 1) {
            EnGeldB_SetupSidestep(self, globalCtx);
        } else {
            EnGeldB_SetupReady(self);
        }
    }
    if ((globalCtx->state.frames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_BREATH);
    }
}

void EnGeldB_SetupStunned(EnGeldB* self) {
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = 0.0f;
    }
    if ((self->damageEffect != GELDB_DMG_FREEZE) || (self->action == GELDB_SPIN_ATTACK)) {
        Animation_PlayOnceSetSpeed(&self->skelAnime, &gGerudoRedDamageAnim, 0.0f);
    }
    if (self->damageEffect == GELDB_DMG_FREEZE) {
        self->iceTimer = 36;
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->action = GELDB_STUNNED;
    EnGeldB_SetupAction(self, EnGeldB_Stunned);
}

void EnGeldB_Stunned(EnGeldB* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }
        self->invisible = false;
    }
    if ((self->actor.colorFilterTimer == 0) && (self->actor.bgCheckFlags & 1)) {
        if (self->actor.colChkInfo.health == 0) {
            EnGeldB_SetupDefeated(self);
        } else {
            EnGeldB_ReactToPlayer(globalCtx, self, 1);
        }
    }
}

void EnGeldB_SetupDamaged(EnGeldB* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gGerudoRedDamageAnim, -4.0f);
    if (self->actor.bgCheckFlags & 1) {
        self->invisible = false;
        self->actor.speedXZ = -4.0f;
    } else {
        self->invisible = true;
    }
    self->lookTimer = 0;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_DAMAGE);
    self->action = GELDB_DAMAGED;
    EnGeldB_SetupAction(self, EnGeldB_Damaged);
}

void EnGeldB_Damaged(EnGeldB* self, GlobalContext* globalCtx) {
    s16 angleToWall;

    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }
        self->invisible = false;
    }
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x1194, 0);
    if (!EnGeldB_DodgeRanged(globalCtx, self) && !EnGeldB_ReactToPlayer(globalCtx, self, 0) &&
        SkelAnime_Update(&self->skelAnime) && (self->actor.bgCheckFlags & 1)) {
        angleToWall = self->actor.wallYaw - self->actor.shape.rot.y;
        if ((self->actor.bgCheckFlags & 8) && (ABS(angleToWall) < 0x2EE0) && (self->actor.xzDistToPlayer < 90.0f)) {
            EnGeldB_SetupJump(self);
        } else if (!EnGeldB_DodgeRanged(globalCtx, self)) {
            if ((self->actor.xzDistToPlayer <= 45.0f) && !Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                (globalCtx->gameplayFrames & 7)) {
                EnGeldB_SetupSlash(self);
            } else {
                EnGeldB_SetupRollBack(self);
            }
        }
    }
}

void EnGeldB_SetupJump(EnGeldB* self) {
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedFlipAnim);

    Animation_Change(&self->skelAnime, &gGerudoRedFlipAnim, -1.0f, lastFrame, 0.0f, ANIMMODE_ONCE, -3.0f);
    self->timer = 0;
    self->invisible = false;
    self->action = GELDB_JUMP;
    self->actor.speedXZ = 6.5f;
    self->actor.velocity.y = 15.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnGeldB_SetupAction(self, EnGeldB_Jump);
}

void EnGeldB_Jump(EnGeldB* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    if (self->actor.velocity.y >= 5.0f) {
        func_800355B8(globalCtx, &self->leftFootPos);
        func_800355B8(globalCtx, &self->rightFootPos);
    }
    if (SkelAnime_Update(&self->skelAnime) && (self->actor.bgCheckFlags & 3)) {
        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->actor.shape.rot.x = 0;
        self->actor.speedXZ = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->actor.world.pos.y = self->actor.floorHeight;
        if (!Actor_OtherIsTargeted(globalCtx, &self->actor)) {
            EnGeldB_SetupSlash(self);
        } else {
            EnGeldB_SetupReady(self);
        }
    }
}

void EnGeldB_SetupBlock(EnGeldB* self) {
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedBlockAnim);

    if (self->swordState != 0) {
        self->swordState = -1;
    }
    self->actor.speedXZ = 0.0f;
    self->action = GELDB_BLOCK;
    self->timer = (s32)Rand_CenteredFloat(10.0f) + 10;
    Animation_Change(&self->skelAnime, &gGerudoRedBlockAnim, 0.0f, 0.0f, lastFrame, ANIMMODE_ONCE, 0.0f);
    EnGeldB_SetupAction(self, EnGeldB_Block);
}

void EnGeldB_Block(EnGeldB* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    s16 angleToLink;
    s16 angleFacingLink;

    if (self->timer != 0) {
        self->timer--;
    } else {
        self->skelAnime.playSpeed = 1.0f;
    }
    if (SkelAnime_Update(&self->skelAnime)) {
        angleToLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        if ((ABS(angleToLink) <= 0x4000) && (self->actor.xzDistToPlayer < 40.0f) &&
            (ABS(self->actor.yDistToPlayer) < 50.0f)) {
            if (func_800354B4(globalCtx, &self->actor, 100.0f, 0x2710, 0x4000, self->actor.shape.rot.y)) {
                if (player->swordAnimation == 0x11) {
                    EnGeldB_SetupSpinDodge(self, globalCtx);
                } else if (globalCtx->gameplayFrames & 1) {
                    EnGeldB_SetupBlock(self);
                } else {
                    EnGeldB_SetupRollBack(self);
                }
            } else {
                angleFacingLink = player->actor.shape.rot.y - self->actor.shape.rot.y;
                if (!Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                    ((globalCtx->gameplayFrames & 1) || (ABS(angleFacingLink) < 0x38E0))) {
                    EnGeldB_SetupSlash(self);
                } else {
                    EnGeldB_SetupCircle(self);
                }
            }
        } else {
            EnGeldB_SetupCircle(self);
        }
    } else if ((self->timer == 0) &&
               func_800354B4(globalCtx, &self->actor, 100.0f, 0x2710, 0x4000, self->actor.shape.rot.y)) {
        if (player->swordAnimation == 0x11) {
            EnGeldB_SetupSpinDodge(self, globalCtx);
        } else if (!EnGeldB_DodgeRanged(globalCtx, self)) {
            if ((globalCtx->gameplayFrames & 1)) {
                if ((self->actor.xzDistToPlayer < 100.0f) && (Rand_ZeroOne() > 0.7f)) {
                    EnGeldB_SetupJump(self);
                } else {
                    EnGeldB_SetupRollBack(self);
                }
            } else {
                EnGeldB_SetupBlock(self);
            }
        }
    }
}

void EnGeldB_SetupSidestep(EnGeldB* self, GlobalContext* globalCtx) {
    s16 linkAngle;
    Player* player;
    f32 lastFrame = Animation_GetLastFrame(&gGerudoRedSidestepAnim);

    Animation_Change(&self->skelAnime, &gGerudoRedSidestepAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, 0.0f);
    player = GET_PLAYER(globalCtx);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 1);
    linkAngle = player->actor.shape.rot.y;
    if (Math_SinS(linkAngle - self->actor.shape.rot.y) > 0.0f) {
        self->actor.speedXZ = -6.0f;
    } else if (Math_SinS(linkAngle - self->actor.shape.rot.y) < 0.0f) {
        self->actor.speedXZ = 6.0f;
    } else {
        self->actor.speedXZ = Rand_CenteredFloat(12.0f);
    }
    self->skelAnime.playSpeed = -self->actor.speedXZ * 0.5f;
    self->approachRate = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
    self->timer = Rand_ZeroOne() * 10.0f + 5.0f;
    self->action = GELDB_SIDESTEP;
    EnGeldB_SetupAction(self, EnGeldB_Sidestep);
}

void EnGeldB_Sidestep(EnGeldB* self, GlobalContext* globalCtx) {
    s16 behindLinkAngle;
    s16 phi_v1;
    Player* player = GET_PLAYER(globalCtx);
    s32 thisKeyFrame;
    s32 prevKeyFrame;
    f32 playSpeed;

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xBB8, 1);
    behindLinkAngle = player->actor.shape.rot.y + 0x8000;
    if (Math_SinS(behindLinkAngle - self->actor.shape.rot.y) > 0.0f) {
        self->actor.speedXZ += 0.125f;
    } else if (Math_SinS(behindLinkAngle - self->actor.shape.rot.y) <= 0.0f) {
        self->actor.speedXZ -= 0.125f;
    }

    if ((self->actor.bgCheckFlags & 8) ||
        !Actor_TestFloorInDirection(&self->actor, globalCtx, self->actor.speedXZ, self->actor.shape.rot.y + 0x3E80)) {
        if (self->actor.bgCheckFlags & 8) {
            if (self->actor.speedXZ >= 0.0f) {
                phi_v1 = self->actor.shape.rot.y + 0x3E80;
            } else {
                phi_v1 = self->actor.shape.rot.y - 0x3E80;
            }
            phi_v1 = self->actor.wallYaw - phi_v1;
        } else {
            self->actor.speedXZ *= -0.8f;
            phi_v1 = 0;
        }
        if (ABS(phi_v1) > 0x4000) {
            self->actor.speedXZ *= -0.8f;
            if (self->actor.speedXZ < 0.0f) {
                self->actor.speedXZ -= 0.5f;
            } else {
                self->actor.speedXZ += 0.5f;
            }
        }
    }
    if (self->actor.speedXZ >= 0.0f) {
        self->actor.world.rot.y = self->actor.shape.rot.y + 0x3E80;
    } else {
        self->actor.world.rot.y = self->actor.shape.rot.y - 0x3E80;
    }
    if (self->actor.xzDistToPlayer <= 45.0f) {
        Math_SmoothStepToF(&self->approachRate, -4.0f, 1.0f, 1.5f, 0.0f);
    } else if (self->actor.xzDistToPlayer > 40.0f) {
        Math_SmoothStepToF(&self->approachRate, 4.0f, 1.0f, 1.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->approachRate, 0.0f, 1.0f, 6.65f, 0.0f);
    }
    if (self->approachRate != 0.0f) {
        self->actor.world.pos.x += Math_SinS(self->actor.shape.rot.y) * self->approachRate;
        self->actor.world.pos.z += Math_CosS(self->actor.shape.rot.y) * self->approachRate;
    }
    if (ABS(self->approachRate) < ABS(self->actor.speedXZ)) {
        self->skelAnime.playSpeed = -self->actor.speedXZ * 0.5f;
    } else {
        self->skelAnime.playSpeed = -self->approachRate * 0.5f;
    }
    self->skelAnime.playSpeed = CLAMP(self->skelAnime.playSpeed, -3.0f, 3.0f);
    thisKeyFrame = self->skelAnime.curFrame;
    SkelAnime_Update(&self->skelAnime);
    prevKeyFrame = self->skelAnime.curFrame - ABS(self->skelAnime.playSpeed);

    playSpeed = ((void)0, ABS(self->skelAnime.playSpeed)); // Needed to match for some reason

    if (!EnGeldB_DodgeRanged(globalCtx, self) && !EnGeldB_ReactToPlayer(globalCtx, self, 0)) {
        if (--self->timer == 0) {
            s16 angleFacingPlayer = player->actor.shape.rot.y - self->actor.shape.rot.y;

            angleFacingPlayer = ABS(angleFacingPlayer);
            if (angleFacingPlayer >= 0x3A98) {
                EnGeldB_SetupReady(self);
                self->timer = (Rand_ZeroOne() * 5.0f) + 1.0f;
            } else {
                Player* player2 = GET_PLAYER(globalCtx);
                s16 angleFacingPlayer2 = player2->actor.shape.rot.y - self->actor.shape.rot.y;

                self->actor.world.rot.y = self->actor.shape.rot.y;
                if ((self->actor.xzDistToPlayer <= 45.0f) && !Actor_OtherIsTargeted(globalCtx, &self->actor) &&
                    (!(globalCtx->gameplayFrames & 3) || (ABS(angleFacingPlayer2) < 0x38E0))) {
                    EnGeldB_SetupSlash(self);
                } else if ((210.0f > self->actor.xzDistToPlayer) && (self->actor.xzDistToPlayer > 150.0f) &&
                           !(globalCtx->gameplayFrames & 1)) {
                    if (Actor_OtherIsTargeted(globalCtx, &self->actor) || (Rand_ZeroOne() > 0.5f) ||
                        (ABS(angleFacingPlayer2) < 0x38E0)) {
                        EnGeldB_SetupRollForward(self);
                    } else {
                        EnGeldB_SetupSpinAttack(self);
                    }
                } else {
                    EnGeldB_SetupAdvance(self, globalCtx);
                }
            }
        }
        if ((thisKeyFrame != (s32)self->skelAnime.curFrame) &&
            (((prevKeyFrame < 0) && (((s32)playSpeed + thisKeyFrame) > 0)) ||
             ((prevKeyFrame < 5) && (((s32)playSpeed + thisKeyFrame) > 5)))) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MUSI_LAND);
        }
        if ((globalCtx->gameplayFrames & 0x5F) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_BREATH);
        }
    }
}

void EnGeldB_SetupDefeated(EnGeldB* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gGerudoRedDefeatAnim, -4.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    if (self->actor.bgCheckFlags & 1) {
        self->invisible = false;
        self->actor.speedXZ = -6.0f;
    } else {
        self->invisible = true;
    }
    self->action = GELDB_DEFEAT;
    self->actor.flags &= ~1;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GERUDOFT_DEAD);
    EnGeldB_SetupAction(self, EnGeldB_Defeated);
}

void EnGeldB_Defeated(EnGeldB* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
        self->invisible = false;
    }
    if (SkelAnime_Update(&self->skelAnime)) {
        EnGeldB_SetupFlee(self);
    } else if ((s32)self->skelAnime.curFrame == 10) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_DOWN);
        func_800F5B58();
    }
}

void EnGeldB_TurnHead(EnGeldB* self, GlobalContext* globalCtx) {
    if ((self->action == GELDB_READY) && (self->lookTimer != 0)) {
        self->headRot.y = Math_SinS(self->lookTimer * 0x1068) * 8920.0f;
    } else if (self->action != GELDB_STUNNED) {
        if ((self->action != GELDB_SLASH) && (self->action != GELDB_SPIN_ATTACK)) {
            Math_SmoothStepToS(&self->headRot.y, self->actor.yawTowardsPlayer - self->actor.shape.rot.y, 1, 0x1F4, 0);
            self->headRot.y = CLAMP(self->headRot.y, -0x256F, 0x256F);
        } else {
            self->headRot.y = 0;
        }
    }
}

void EnGeldB_CollisionCheck(EnGeldB* self, GlobalContext* globalCtx) {
    s32 pad;
    EnItem00* key;

    if (self->blockCollider.base.acFlags & AC_BOUNCED) {
        self->blockCollider.base.acFlags &= ~AC_BOUNCED;
        self->bodyCollider.base.acFlags &= ~AC_HIT;
    } else if ((self->bodyCollider.base.acFlags & AC_HIT) && (self->action >= GELDB_READY) &&
               (self->spinAttackState < 2)) {
        self->bodyCollider.base.acFlags &= ~AC_HIT;
        if (self->actor.colChkInfo.damageEffect != GELDB_DMG_UNK_6) {
            self->damageEffect = self->actor.colChkInfo.damageEffect;
            Actor_SetDropFlag(&self->actor, &self->bodyCollider.info, 1);
            func_800F8A44(&self->actor.projectedPos, NA_SE_EN_GERUDOFT_BREATH);
            if ((self->actor.colChkInfo.damageEffect == GELDB_DMG_STUN) ||
                (self->actor.colChkInfo.damageEffect == GELDB_DMG_FREEZE)) {
                if (self->action != GELDB_STUNNED) {
                    Actor_SetColorFilter(&self->actor, 0, 0x78, 0, 0x50);
                    Actor_ApplyDamage(&self->actor);
                    EnGeldB_SetupStunned(self);
                }
            } else {
                Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
                if (Actor_ApplyDamage(&self->actor) == 0) {
                    if (self->keyFlag != 0) {
                        key = Item_DropCollectible(globalCtx, &self->actor.world.pos, self->keyFlag | ITEM00_SMALL_KEY);
                        if (key != NULL) {
                            key->actor.world.rot.y = Math_Vec3f_Yaw(&key->actor.world.pos, &self->actor.home.pos);
                            key->actor.speedXZ = 6.0f;
                            Audio_PlaySoundGeneral(NA_SE_SY_TRE_BOX_APPEAR, &D_801333D4, 4, &D_801333E0, &D_801333E0,
                                                   &D_801333E8);
                        }
                    }
                    EnGeldB_SetupDefeated(self);
                    Enemy_StartFinishingBlow(globalCtx, &self->actor);
                } else {
                    EnGeldB_SetupDamaged(self);
                }
            }
        }
    }
}

void EnGeldB_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnGeldB* self = THIS;

    EnGeldB_CollisionCheck(self, globalCtx);
    if (self->actor.colChkInfo.damageEffect != GELDB_DMG_UNK_6) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 15.0f, 30.0f, 60.0f, 0x1D);
        self->actionFunc(self, globalCtx);
        self->actor.focus.pos = self->actor.world.pos;
        self->actor.focus.pos.y += 40.0f;
        EnGeldB_TurnHead(self, globalCtx);
    }
    Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
    if ((self->action >= GELDB_READY) && (self->spinAttackState < 2) &&
        ((self->actor.colorFilterTimer == 0) || !(self->actor.colorFilterParams & 0x4000))) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
    }
    if ((self->action == GELDB_BLOCK) && (self->skelAnime.curFrame == 0.0f)) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->blockCollider.base);
    }
    if (self->swordState > 0) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->swordCollider.base);
    }
    if (self->blinkState == 0) {
        if ((Rand_ZeroOne() < 0.1f) && ((globalCtx->gameplayFrames % 4) == 0)) {
            self->blinkState++;
        }
    } else {
        self->blinkState = (self->blinkState + 1) & 3;
    }
}

s32 EnGeldB_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                             void* thisx) {
    EnGeldB* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_geldB.c", 2507);
    if (limbIndex == GELDB_LIMB_NECK) {
        rot->z += self->headRot.x;
        rot->x += self->headRot.y;
        rot->y += self->headRot.z;
    } else if (limbIndex == GELDB_LIMB_HEAD) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, 80, 60, 10, 255);
    } else if ((limbIndex == GELDB_LIMB_R_SWORD) || (limbIndex == GELDB_LIMB_L_SWORD)) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, 140, 170, 230, 255);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    } else {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, 140, 0, 0, 255);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_geldB.c", 2529);
    return false;
}

void EnGeldB_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f footOffset = { 300.0f, 0.0f, 0.0f };
    static Vec3f swordTipOffset = { 0.0f, -3000.0f, 0.0f };
    static Vec3f swordHiltOffset = { 400.0f, 0.0f, 0.0f };
    static Vec3f swordQuadOffset1 = { 1600.0f, -4000.0f, 0.0f };
    static Vec3f swordQuadOffset0 = { -3000.0f, -2000.0f, 1300.0f };
    static Vec3f swordQuadOffset3 = { -3000.0f, -2000.0f, -1300.0f };
    static Vec3f swordQuadOffset2 = { 1000.0f, 1000.0f, 0.0f };
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    Vec3f swordTip;
    Vec3f swordHilt;
    EnGeldB* self = THIS;
    s32 bodyPart = -1;

    if (limbIndex == GELDB_LIMB_R_SWORD) {
        Matrix_MultVec3f(&swordQuadOffset1, &self->swordCollider.dim.quad[1]);
        Matrix_MultVec3f(&swordQuadOffset0, &self->swordCollider.dim.quad[0]);
        Matrix_MultVec3f(&swordQuadOffset3, &self->swordCollider.dim.quad[3]);
        Matrix_MultVec3f(&swordQuadOffset2, &self->swordCollider.dim.quad[2]);
        Collider_SetQuadVertices(&self->swordCollider, &self->swordCollider.dim.quad[0],
                                 &self->swordCollider.dim.quad[1], &self->swordCollider.dim.quad[2],
                                 &self->swordCollider.dim.quad[3]);
        Matrix_MultVec3f(&swordTipOffset, &swordTip);
        Matrix_MultVec3f(&swordHiltOffset, &swordHilt);

        if ((self->swordState < 0) || ((self->action != GELDB_SLASH) && (self->action != GELDB_SPIN_ATTACK))) {
            EffectBlure_AddSpace(Effect_GetByIndex(self->blureIndex));
            self->swordState = 0;
        } else if (self->swordState > 0) {
            EffectBlure_AddVertex(Effect_GetByIndex(self->blureIndex), &swordTip, &swordHilt);
        }
    } else {
        Actor_SetFeetPos(&self->actor, limbIndex, GELDB_LIMB_L_FOOT, &footOffset, GELDB_LIMB_R_FOOT, &footOffset);
    }

    if (limbIndex == GELDB_LIMB_L_FOOT) {
        Matrix_MultVec3f(&footOffset, &self->leftFootPos);
    } else if (limbIndex == GELDB_LIMB_R_FOOT) {
        Matrix_MultVec3f(&footOffset, &self->rightFootPos);
    }

    if (self->iceTimer != 0) {
        switch (limbIndex) {
            case GELDB_LIMB_NECK:
                bodyPart = 0;
                break;
            case GELDB_LIMB_L_SWORD:
                bodyPart = 1;
                break;
            case GELDB_LIMB_R_SWORD:
                bodyPart = 2;
                break;
            case GELDB_LIMB_L_UPPER_ARM:
                bodyPart = 3;
                break;
            case GELDB_LIMB_R_UPPER_ARM:
                bodyPart = 4;
                break;
            case GELDB_LIMB_TORSO:
                bodyPart = 5;
                break;
            case GELDB_LIMB_WAIST:
                bodyPart = 6;
                break;
            case GELDB_LIMB_L_FOOT:
                bodyPart = 7;
                break;
            case GELDB_LIMB_R_FOOT:
                bodyPart = 8;
                break;
            default:
                break;
        }

        if (bodyPart >= 0) {
            Vec3f limbPos;

            Matrix_MultVec3f(&zeroVec, &limbPos);
            self->bodyPartsPos[bodyPart].x = limbPos.x;
            self->bodyPartsPos[bodyPart].y = limbPos.y;
            self->bodyPartsPos[bodyPart].z = limbPos.z;
        }
    }
}

void EnGeldB_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Vec3f blockTrisOffsets0[3] = {
        { -3000.0f, 6000.0f, 1600.0f },
        { -3000.0f, 0.0f, 1600.0f },
        { 3000.0f, 6000.0f, 1600.0f },
    };
    static Vec3f blockTrisOffsets1[3] = {
        { -3000.0f, 0.0f, 1600.0f },
        { 3000.0f, 0.0f, 1600.0f },
        { 3000.0f, 6000.0f, 1600.0f },
    };
    static void* eyeTextures[] = { gGerudoRedEyeOpenTex, gGerudoRedEyeHalfTex, gGerudoRedEyeShutTex,
                                   gGerudoRedEyeHalfTex };
    s32 pad;
    EnGeldB* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_geldB.c", 2672);
    if (1) {}

    if ((self->spinAttackState >= 2) && SkelAnime_Update(&self->skelAnime)) {
        if (self->spinAttackState == 2) {
            Animation_Change(&self->skelAnime, &gGerudoRedSpinAttackAnim, 0.5f, 0.0f, 12.0f, ANIMMODE_ONCE_INTERP,
                             4.0f);
            self->spinAttackState++;
            thisx->world.rot.y = thisx->shape.rot.y = thisx->yawTowardsPlayer;
        } else {
            self->timer--;
            if (self->timer == 0) {
                if ((INV_CONTENT(ITEM_HOOKSHOT) == ITEM_NONE) || (INV_CONTENT(ITEM_LONGSHOT) == ITEM_NONE)) {
                    globalCtx->nextEntranceIndex = 0x1A5;
                } else if (gSaveContext.eventChkInf[12] & 0x80) {
                    globalCtx->nextEntranceIndex = 0x5F8;
                } else {
                    globalCtx->nextEntranceIndex = 0x3B4;
                }
                globalCtx->fadeTransition = 0x26;
                globalCtx->sceneLoadFlag = 0x14;
            }
        }
    }

    if ((self->action != GELDB_WAIT) || !self->invisible) {
        func_80093D18(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->blinkState]));
        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnGeldB_OverrideLimbDraw, EnGeldB_PostLimbDraw, self);
        if (self->action == GELDB_BLOCK) {
            s32 i;
            Vec3f blockTrisVtx0[3];
            Vec3f blockTrisVtx1[3];

            for (i = 0; i < 3; i++) {
                Matrix_MultVec3f(&blockTrisOffsets0[i], &blockTrisVtx0[i]);
                Matrix_MultVec3f(&blockTrisOffsets1[i], &blockTrisVtx1[i]);
            }
            Collider_SetTrisVertices(&self->blockCollider, 0, &blockTrisVtx0[0], &blockTrisVtx0[1], &blockTrisVtx0[2]);
            Collider_SetTrisVertices(&self->blockCollider, 1, &blockTrisVtx1[0], &blockTrisVtx1[1], &blockTrisVtx1[2]);
        }

        if (self->iceTimer != 0) {
            thisx->colorFilterTimer++;
            self->iceTimer--;
            if ((self->iceTimer % 4) == 0) {
                s32 iceIndex = self->iceTimer >> 2;

                EffectSsEnIce_SpawnFlyingVec3s(globalCtx, thisx, &self->bodyPartsPos[iceIndex], 150, 150, 150, 250, 235,
                                               245, 255, 1.5f);
            }
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_geldB.c", 2744);
}

s32 EnGeldB_DodgeRanged(GlobalContext* globalCtx, EnGeldB* self) {
    Actor* actor = Actor_GetProjectileActor(globalCtx, &self->actor, 800.0f);

    if (actor != NULL) {
        s16 angleToFacing;
        s16 pad18;
        f32 dist;

        angleToFacing = Actor_WorldYawTowardActor(&self->actor, actor) - self->actor.shape.rot.y;
        self->actor.world.rot.y = (u16)self->actor.shape.rot.y & 0xFFFF;
        dist = Actor_WorldDistXYZToPoint(&self->actor, &actor->world.pos);
        //! @bug
        // Actor_WorldDistXYZToPoint already sqrtfs the distance, so self actually checks for a
        // distance of 360000. Also it's a double calculation because no f on sqrt.
        if ((ABS(angleToFacing) < 0x2EE0) && (sqrt(dist) < 600.0)) {
            if (actor->id == ACTOR_ARMS_HOOK) {
                EnGeldB_SetupJump(self);
            } else {
                EnGeldB_SetupBlock(self);
            }
        } else {
            self->actor.world.rot.y = self->actor.shape.rot.y + 0x3FFF;
            if ((ABS(angleToFacing) < 0x2000) || (ABS(angleToFacing) > 0x5FFF)) {
                EnGeldB_SetupSidestep(self, globalCtx);
                self->actor.speedXZ *= 3.0f;
            } else if (ABS(angleToFacing) < 0x5FFF) {
                EnGeldB_SetupRollBack(self);
            }
        }
        return true;
    }
    return false;
}
