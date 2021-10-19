/*
 * File: z_en_mb.c
 * Overlay: ovl_En_Mb
 * Description: Moblins
 */

#include "z_en_mb.h"
#include "objects/object_mb/object_mb.h"

/*
 * This actor can have three behaviors:
 * - "Spear Guard" (variable -1): uses a spear, walks around home point, charges player if too close
 * - "Club" (variable 0): uses a club, stands still and smashes its club on the ground when the player approaches
 * - "Spear Patrol" (variable 0xPP00 PP=pathId): uses a spear, patrols following a path, charges
 */

#define FLAGS 0x00000015

#define THIS ((EnMb*)thisx)

typedef enum {
    /* -1 */ ENMB_TYPE_SPEAR_GUARD = -1,
    /*  0 */ ENMB_TYPE_CLUB,
    /*  1 */ ENMB_TYPE_SPEAR_PATROL
} EnMbType;

#define ENMB_ATTACK_NONE 0
#define ENMB_ATTACK_SPEAR 1
#define ENMB_ATTACK_CLUB_RIGHT 1
#define ENMB_ATTACK_CLUB_MIDDLE 2
#define ENMB_ATTACK_CLUB_LEFT 3

/* Spear and Club moblins use a different skeleton but the limbs are organized the same */
typedef enum {
    /*  1 */ ENMB_LIMB_ROOT = 1,
    /*  3 */ ENMB_LIMB_WAIST = 3,
    /*  6 */ ENMB_LIMB_CHEST = 6,
    /*  7 */ ENMB_LIMB_HEAD,
    /*  9 */ ENMB_LIMB_LSHOULDER = 9,
    /* 11 */ ENMB_LIMB_LFOREARM = 11,
    /* 12 */ ENMB_LIMB_LHAND,
    /* 14 */ ENMB_LIMB_RSHOULDER = 14,
    /* 16 */ ENMB_LIMB_RFOREARM = 16,
    /* 17 */ ENMB_LIMB_RHAND,
    /* 20 */ ENMB_LIMB_LTHIGH = 20,
    /* 21 */ ENMB_LIMB_LSHIN,
    /* 22 */ ENMB_LIMB_LFOOT,
    /* 25 */ ENMB_LIMB_RTHIGH = 25,
    /* 26 */ ENMB_LIMB_RSHIN,
    /* 27 */ ENMB_LIMB_RFOOT
} EnMbLimb;

void EnMb_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMb_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMb_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Mb_InitVars = {
    ACTOR_EN_MB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_MB,
    sizeof(EnMb),
    (ActorFunc)EnMb_Init,
    (ActorFunc)EnMb_Destroy,
    (ActorFunc)EnMb_Update,
    (ActorFunc)EnMb_Draw,
};

void EnMb_SetupSpearPatrolTurnTowardsWaypoint(EnMb* self, GlobalContext* globalCtx);
void EnMb_SetupClubWaitPlayerNear(EnMb* self);
void EnMb_SpearGuardLookAround(EnMb* self, GlobalContext* globalCtx);
void EnMb_SetupSpearGuardLookAround(EnMb* self);
void EnMb_SetupSpearDamaged(EnMb* self);
void EnMb_SpearGuardWalk(EnMb* self, GlobalContext* globalCtx);
void EnMb_SpearGuardPrepareAndCharge(EnMb* self, GlobalContext* globalCtx);
void EnMb_SpearPatrolPrepareAndCharge(EnMb* self, GlobalContext* globalCtx);
void EnMb_SpearEndChargeQuick(EnMb* self, GlobalContext* globalCtx);
void EnMb_Stunned(EnMb* self, GlobalContext* globalCtx);
void EnMb_ClubDead(EnMb* self, GlobalContext* globalCtx);
void EnMb_ClubDamagedWhileKneeling(EnMb* self, GlobalContext* globalCtx);
void EnMb_ClubWaitPlayerNear(EnMb* self, GlobalContext* globalCtx);
void EnMb_ClubAttack(EnMb* self, GlobalContext* globalCtx);
void EnMb_SpearDead(EnMb* self, GlobalContext* globalCtx);
void EnMb_SpearDamaged(EnMb* self, GlobalContext* globalCtx);
void EnMb_SetupSpearDead(EnMb* self);
void EnMb_SpearPatrolTurnTowardsWaypoint(EnMb* self, GlobalContext* globalCtx);
void EnMb_SpearPatrolWalkTowardsWaypoint(EnMb* self, GlobalContext* globalCtx);
void EnMb_SpearPatrolEndCharge(EnMb* self, GlobalContext* globalCtx);
void EnMb_SpearPatrolImmediateCharge(EnMb* self, GlobalContext* globalCtx);
void EnMb_ClubWaitAfterAttack(EnMb* self, GlobalContext* globalCtx);
void EnMb_ClubDamaged(EnMb* self, GlobalContext* globalCtx);

static ColliderCylinderInit sHitboxInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
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
    { 20, 70, 0, { 0, 0, 0 } },
};

static ColliderTrisElementInit sFrontShieldingTrisInit[2] = {
    {
        {
            ELEMTYPE_UNK2,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE | BUMP_NO_AT_INFO,
            OCELEM_NONE,
        },
        { { { -10.0f, 14.0f, 2.0f }, { -10.0f, -6.0f, 2.0f }, { 9.0f, 14.0f, 2.0f } } },
    },
    {
        {
            ELEMTYPE_UNK2,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE | BUMP_NO_AT_INFO,
            OCELEM_NONE,
        },
        { { { -10.0f, -6.0f, 2.0f }, { 9.0f, -6.0f, 2.0f }, { 9.0f, 14.0f, 2.0f } } },
    },
};

static ColliderTrisInit sFrontShieldingInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    sFrontShieldingTrisInit,
};

static ColliderQuadInit sAttackColliderInit = {
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
    /* 0x0 */ ENMB_DMGEFF_IGNORE,
    /* 0x1 */ ENMB_DMGEFF_STUN,
    /* 0x5 */ ENMB_DMGEFF_FREEZE = 0x5,
    /* 0x6 */ ENMB_DMGEFF_STUN_ICE,
    /* 0xF */ ENMB_DMGEFF_DEFAULT = 0xF
} EnMbDamageEffect;

static DamageTable sSpearMoblinDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, ENMB_DMGEFF_FREEZE),
    /* Deku stick    */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Slingshot     */ DMG_ENTRY(1, ENMB_DMGEFF_DEFAULT),
    /* Explosive     */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Boomerang     */ DMG_ENTRY(0, ENMB_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Hammer swing  */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Hookshot      */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Kokiri sword  */ DMG_ENTRY(1, ENMB_DMGEFF_DEFAULT),
    /* Master sword  */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Giant's Knife */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Fire arrow    */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Ice arrow     */ DMG_ENTRY(4, ENMB_DMGEFF_STUN_ICE),
    /* Light arrow   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Unk arrow 1   */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Unk arrow 2   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Unk arrow 3   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Fire magic    */ DMG_ENTRY(0, ENMB_DMGEFF_FREEZE),
    /* Ice magic     */ DMG_ENTRY(3, ENMB_DMGEFF_STUN_ICE),
    /* Light magic   */ DMG_ENTRY(0, ENMB_DMGEFF_FREEZE),
    /* Shield        */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
    /* Mirror Ray    */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
    /* Kokiri spin   */ DMG_ENTRY(1, ENMB_DMGEFF_DEFAULT),
    /* Giant spin    */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Master spin   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Kokiri jump   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Giant jump    */ DMG_ENTRY(8, ENMB_DMGEFF_DEFAULT),
    /* Master jump   */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Unknown 1     */ DMG_ENTRY(0, ENMB_DMGEFF_FREEZE),
    /* Unblockable   */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
    /* Hammer jump   */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Unknown 2     */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
};

static DamageTable sClubMoblinDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, ENMB_DMGEFF_FREEZE),
    /* Deku stick    */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Slingshot     */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
    /* Explosive     */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Boomerang     */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
    /* Normal arrow  */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Hammer swing  */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Hookshot      */ DMG_ENTRY(0, ENMB_DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, ENMB_DMGEFF_DEFAULT),
    /* Master sword  */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Giant's Knife */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Fire arrow    */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Ice arrow     */ DMG_ENTRY(4, ENMB_DMGEFF_STUN_ICE),
    /* Light arrow   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Unk arrow 1   */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Unk arrow 2   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Unk arrow 3   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Fire magic    */ DMG_ENTRY(0, ENMB_DMGEFF_FREEZE),
    /* Ice magic     */ DMG_ENTRY(3, ENMB_DMGEFF_STUN_ICE),
    /* Light magic   */ DMG_ENTRY(0, ENMB_DMGEFF_FREEZE),
    /* Shield        */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
    /* Mirror Ray    */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
    /* Kokiri spin   */ DMG_ENTRY(1, ENMB_DMGEFF_DEFAULT),
    /* Giant spin    */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Master spin   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Kokiri jump   */ DMG_ENTRY(2, ENMB_DMGEFF_DEFAULT),
    /* Giant jump    */ DMG_ENTRY(8, ENMB_DMGEFF_DEFAULT),
    /* Master jump   */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Unknown 1     */ DMG_ENTRY(0, ENMB_DMGEFF_FREEZE),
    /* Unblockable   */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
    /* Hammer jump   */ DMG_ENTRY(4, ENMB_DMGEFF_DEFAULT),
    /* Unknown 2     */ DMG_ENTRY(0, ENMB_DMGEFF_IGNORE),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x4A, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 5300, ICHAIN_STOP),
};

void EnMb_SetupAction(EnMb* self, EnMbActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnMb_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnMb* self = THIS;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s16 relYawFromPlayer;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 46.0f);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.colChkInfo.damageTable = &sSpearMoblinDamageTable;
    Collider_InitCylinder(globalCtx, &self->hitbox);
    Collider_SetCylinder(globalCtx, &self->hitbox, &self->actor, &sHitboxInit);
    Collider_InitTris(globalCtx, &self->frontShielding);
    Collider_SetTris(globalCtx, &self->frontShielding, &self->actor, &sFrontShieldingInit, self->frontShieldingTris);
    Collider_InitQuad(globalCtx, &self->attackCollider);
    Collider_SetQuad(globalCtx, &self->attackCollider, &self->actor, &sAttackColliderInit);

    switch (self->actor.params) {
        case ENMB_TYPE_SPEAR_GUARD:
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gEnMbSpearSkel, &gEnMbSpearStandStillAnim,
                               self->jointTable, self->morphTable, 28);
            self->actor.colChkInfo.health = 2;
            self->actor.colChkInfo.mass = MASS_HEAVY;
            self->maxHomeDist = 1000.0f;
            self->playerDetectionRange = 1750.0f;
            EnMb_SetupSpearGuardLookAround(self);
            break;
        case ENMB_TYPE_CLUB:
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gEnMbClubSkel, &gEnMbClubStandStillClubDownAnim,
                               self->jointTable, self->morphTable, 28);

            self->actor.colChkInfo.health = 6;
            self->actor.colChkInfo.mass = MASS_IMMOVABLE;
            self->actor.colChkInfo.damageTable = &sClubMoblinDamageTable;
            Actor_SetScale(&self->actor, 0.02f);
            self->hitbox.dim.height = 170;
            self->hitbox.dim.radius = 45;
            self->actor.uncullZoneForward = 4000.0f;
            self->actor.uncullZoneScale = 800.0f;
            self->actor.uncullZoneDownward = 1800.0f;
            self->playerDetectionRange = 710.0f;
            self->attackCollider.info.toucher.dmgFlags = 0x20000000;

            relYawFromPlayer =
                self->actor.world.rot.y - Math_Vec3f_Yaw(&self->actor.world.pos, &player->actor.world.pos);
            if (ABS(relYawFromPlayer) > 0x4000) {
                self->actor.world.rot.y = thisx->world.rot.y + 0x8000;
                self->actor.shape.rot.y = thisx->world.rot.y;
                self->actor.world.pos.z = thisx->world.pos.z + 600.0f;
            }

            ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);
            self->actor.flags &= ~1;
            self->actor.naviEnemyId += 1;
            EnMb_SetupClubWaitPlayerNear(self);
            break;
        default: /* Spear Patrol */
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gEnMbSpearSkel, &gEnMbSpearStandStillAnim,
                               self->jointTable, self->morphTable, 28);

            Actor_SetScale(&self->actor, 0.014f);
            self->path = (thisx->params & 0xFF00) >> 8;
            self->actor.params = ENMB_TYPE_SPEAR_PATROL;
            self->waypoint = 0;
            self->actor.colChkInfo.health = 1;
            self->actor.colChkInfo.mass = MASS_HEAVY;
            self->maxHomeDist = 350.0f;
            self->playerDetectionRange = 1750.0f;
            self->actor.flags &= ~1;
            EnMb_SetupSpearPatrolTurnTowardsWaypoint(self, globalCtx);
            break;
    }
}

void EnMb_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnMb* self = THIS;

    Collider_DestroyTris(globalCtx, &self->frontShielding);
    Collider_DestroyCylinder(globalCtx, &self->hitbox);
    Collider_DestroyQuad(globalCtx, &self->attackCollider);
}

void EnMb_FaceWaypoint(EnMb* self, GlobalContext* globalCtx) {
    s16 yawToWaypoint = Math_Vec3f_Yaw(&self->actor.world.pos, &self->waypointPos);

    self->actor.shape.rot.y = yawToWaypoint;
    self->actor.world.rot.y = yawToWaypoint;
}

void EnMb_NextWaypoint(EnMb* self, GlobalContext* globalCtx) {
    Path* path;
    Vec3s* waypointPos;

    path = &globalCtx->setupPathList[self->path];

    if (self->waypoint == 0) {
        self->direction = 1;
    } else if (self->waypoint == (s8)(path->count - 1)) {
        self->direction = -1;
    }

    self->waypoint += self->direction;
    waypointPos = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->waypoint;
    self->waypointPos.x = waypointPos->x;
    self->waypointPos.y = waypointPos->y;
    self->waypointPos.z = waypointPos->z;
}

/**
 * Checks if the player is in a 800*74 units XZ area centered on self actor,
 * the area being directed along its line of sight snapped to a cardinal angle.
 * Note: the longest corridor in Sacred Forest Meadows is 800 units long,
 *       and they all are 100 units wide.
 */
s32 EnMb_IsPlayerInCorridor(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 xFromPlayer;
    f32 zFromPlayer;
    f32 cos;
    f32 sin;
    f32 xFromPlayerAbs;
    f32 zFromPlayerAbs;
    s16 alignedYaw = 0;

    if ((self->actor.world.rot.y < -0x62A2) || (self->actor.world.rot.y > 0x62A2)) {
        alignedYaw = -0x8000;
    } else if (self->actor.world.rot.y < -0x20E0) {
        alignedYaw = -0x4000;
    } else if (self->actor.world.rot.y > 0x20E0) {
        alignedYaw = 0x4000;
    }

    cos = Math_CosS(alignedYaw);
    sin = Math_SinS(alignedYaw);
    cos = ABS(cos);
    sin = ABS(sin);
    xFromPlayer = self->actor.world.pos.x - player->actor.world.pos.x;
    zFromPlayer = self->actor.world.pos.z - player->actor.world.pos.z;
    xFromPlayerAbs = ABS(xFromPlayer);
    if (xFromPlayerAbs < (cos * 37.0f + sin * 400.0f)) {
        zFromPlayerAbs = ABS(zFromPlayer);
        if (zFromPlayerAbs < (sin * 37.0f + cos * 400.0f)) {
            return true;
        }
    }
    return false;
}

void EnMb_FindWaypointTowardsPlayer(EnMb* self, GlobalContext* globalCtx) {
    Path* path = &globalCtx->setupPathList[self->path];
    s16 yawToWaypoint;
    Vec3f waypointPosF;
    Vec3s* waypointPosS;
    s16 yawPlayerToWaypoint;
    s32 i;
    s32 waypoint;

    for (waypoint = 0, i = path->count - 1; i >= 0; i--, waypoint++) {
        waypointPosS = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + waypoint;
        waypointPosF.x = waypointPosS->x;
        waypointPosF.y = waypointPosS->y;
        waypointPosF.z = waypointPosS->z;
        yawToWaypoint = Math_Vec3f_Yaw(&self->actor.world.pos, &waypointPosF);
        yawPlayerToWaypoint = yawToWaypoint - self->actor.yawTowardsPlayer;
        if (ABS(yawPlayerToWaypoint) <= 0x1770) {
            self->actor.world.rot.y = yawToWaypoint;
            if (waypoint == self->waypoint) {
                self->direction = -self->direction;
            }
            self->waypointPos = waypointPosF;
            self->waypoint = waypoint;
            break;
        }
    }
}

void EnMb_SetupSpearGuardLookAround(EnMb* self) {
    Animation_MorphToLoop(&self->skelAnime, &gEnMbSpearLookLeftAndRightAnim, -4.0f);
    self->actor.speedXZ = 0.0f;
    self->timer1 = Rand_S16Offset(30, 50);
    self->state = ENMB_STATE_IDLE;
    EnMb_SetupAction(self, EnMb_SpearGuardLookAround);
}

void EnMb_SetupClubWaitPlayerNear(EnMb* self) {
    Animation_PlayLoop(&self->skelAnime, &gEnMbClubStandStillClubDownAnim);
    self->actor.speedXZ = 0.0f;
    self->timer1 = Rand_S16Offset(30, 50);
    self->state = ENMB_STATE_IDLE;
    EnMb_SetupAction(self, EnMb_ClubWaitPlayerNear);
}

void EnMb_SetupSpearPatrolTurnTowardsWaypoint(EnMb* self, GlobalContext* globalCtx) {
    Animation_MorphToLoop(&self->skelAnime, &gEnMbSpearLookLeftAndRightAnim, -4.0f);
    self->actor.speedXZ = 0.0f;
    self->timer1 = Rand_S16Offset(40, 80);
    self->state = ENMB_STATE_IDLE;
    EnMb_NextWaypoint(self, globalCtx);
    EnMb_SetupAction(self, EnMb_SpearPatrolTurnTowardsWaypoint);
}

void EnMb_SetupSpearGuardWalk(EnMb* self) {
    Animation_Change(&self->skelAnime, &gEnMbSpearWalkAnim, 0.0f, 0.0f, Animation_GetLastFrame(&gEnMbSpearWalkAnim),
                     ANIMMODE_LOOP, -4.0f);
    self->actor.speedXZ = 0.59999996f;
    self->timer1 = Rand_S16Offset(50, 70);
    self->unk_332 = 1;
    self->state = ENMB_STATE_WALK;
    EnMb_SetupAction(self, EnMb_SpearGuardWalk);
}

void EnMb_SetupSpearPatrolWalkTowardsWaypoint(EnMb* self) {
    f32 frameCount = Animation_GetLastFrame(&gEnMbSpearWalkAnim);

    self->actor.speedXZ = 0.59999996f;
    self->timer1 = Rand_S16Offset(50, 70);
    self->unk_332 = 1;
    self->state = ENMB_STATE_WALK;
    Animation_Change(&self->skelAnime, &gEnMbSpearWalkAnim, 0.0f, 0.0f, frameCount, ANIMMODE_LOOP_INTERP, -4.0f);
    EnMb_SetupAction(self, EnMb_SpearPatrolWalkTowardsWaypoint);
}

void EnMb_SetupSpearPrepareAndCharge(EnMb* self) {
    f32 frameCount = Animation_GetLastFrame(&gEnMbSpearPrepareChargeAnim);

    Animation_MorphToPlayOnce(&self->skelAnime, &gEnMbSpearPrepareChargeAnim, -4.0f);
    self->state = ENMB_STATE_ATTACK;
    self->actor.speedXZ = 0.0f;
    self->timer3 = (s16)frameCount + 6;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_SPEAR_AT);
    if (self->actor.params == ENMB_TYPE_SPEAR_GUARD) {
        EnMb_SetupAction(self, EnMb_SpearGuardPrepareAndCharge);
    } else {
        EnMb_SetupAction(self, EnMb_SpearPatrolPrepareAndCharge);
    }
}

void EnMb_SetupSpearPatrolImmediateCharge(EnMb* self) {
    Animation_PlayLoop(&self->skelAnime, &gEnMbSpearChargeAnim);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_ATTACK);
    self->attack = ENMB_ATTACK_SPEAR;
    self->state = ENMB_STATE_ATTACK;
    self->timer3 = 3;
    self->actor.speedXZ = 10.0f;
    EnMb_SetupAction(self, EnMb_SpearPatrolImmediateCharge);
}

void EnMb_SetupClubAttack(EnMb* self) {
    f32 frames = Animation_GetLastFrame(&gEnMbClubLiftClubAnim);
    s16 relYawFromPlayer;

    self->state = ENMB_STATE_ATTACK;
    Animation_Change(&self->skelAnime, &gEnMbClubLiftClubAnim, 3.0f, 0.0f, frames, ANIMMODE_ONCE_INTERP, 0.0f);
    self->timer3 = 1;
    relYawFromPlayer = self->actor.world.rot.y - self->actor.yawTowardsPlayer;

    if (ABS(relYawFromPlayer) <= 0x258) {
        self->attack = ENMB_ATTACK_CLUB_MIDDLE;
    } else if (relYawFromPlayer >= 0) {
        self->attack = ENMB_ATTACK_CLUB_RIGHT;
    } else {
        self->attack = ENMB_ATTACK_CLUB_LEFT;
    }

    EnMb_SetupAction(self, EnMb_ClubAttack);
}

void EnMb_SetupSpearEndChargeQuick(EnMb* self) {
    Animation_PlayOnce(&self->skelAnime, &gEnMbSpearSlowDownAnim);
    self->state = ENMB_STATE_ATTACK_END;
    self->timer1 = 0;
    self->timer3 = 5;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_SLIDE);
    EnMb_SetupAction(self, EnMb_SpearEndChargeQuick);
}

void EnMb_SetupSpearPatrolEndCharge(EnMb* self) {
    Animation_PlayOnce(&self->skelAnime, &gEnMbSpearSlowDownAnim);
    self->state = ENMB_STATE_ATTACK_END;
    self->actor.bgCheckFlags &= ~1;
    self->timer1 = 0;
    self->timer3 = 50;
    self->actor.speedXZ = -8.0f;
    self->actor.velocity.y = 6.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_SLIDE);
    EnMb_SetupAction(self, EnMb_SpearPatrolEndCharge);
}

void EnMb_SetupClubWaitAfterAttack(EnMb* self) {
    f32 frameCount = Animation_GetLastFrame(&gEnMbClubStandStillClubDownAnim);

    self->state = ENMB_STATE_ATTACK_END;
    Animation_Change(&self->skelAnime, &gEnMbClubStandStillClubDownAnim, 5.0f, 0.0f, frameCount, ANIMMODE_ONCE_INTERP,
                     0.0f);
    EnMb_SetupAction(self, EnMb_ClubWaitAfterAttack);
}

void EnMb_SetupClubDamaged(EnMb* self) {
    Animation_PlayOnce(&self->skelAnime, &gEnMbClubDamagedKneelAnim);
    self->state = ENMB_STATE_CLUB_KNEELING;
    self->timer1 = 0;
    self->timer3 = 20;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_DEAD);
    EnMb_SetupAction(self, EnMb_ClubDamaged);
}

void EnMb_SetupClubDamagedWhileKneeling(EnMb* self) {
    f32 frames = Animation_GetLastFrame(&gEnMbClubBeatenKneelingAnim);

    self->state = ENMB_STATE_CLUB_KNEELING_DAMAGED;
    self->timer1 = 0;
    self->timer3 = 6;
    Animation_Change(&self->skelAnime, &gEnMbClubBeatenKneelingAnim, 1.0f, 4.0f, frames, ANIMMODE_ONCE_INTERP, 0.0f);
    EnMb_SetupAction(self, EnMb_ClubDamagedWhileKneeling);
}

void EnMb_SetupClubDead(EnMb* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gEnMbClubFallOnItsBackAnim, -4.0f);
    self->state = ENMB_STATE_CLUB_DEAD;
    self->actor.flags &= ~1;
    self->hitbox.dim.height = 80;
    self->hitbox.dim.radius = 95;
    self->timer1 = 30;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_DEAD);
    EnMb_SetupAction(self, EnMb_ClubDead);
}

void EnMb_SetupStunned(EnMb* self) {
    self->state = ENMB_STATE_STUNNED;
    self->actor.speedXZ = 0.0f;
    Actor_SetColorFilter(&self->actor, 0, 0x78, 0, 0x50);
    if (self->damageEffect == ENMB_DMGEFF_STUN_ICE) {
        self->iceEffectTimer = 40;
    } else {
        if (self->actor.params != ENMB_TYPE_CLUB) {
            Animation_PlayOnceSetSpeed(&self->skelAnime, &gEnMbSpearDamagedFromFrontAnim, 0.0f);
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    }
    EnMb_SetupAction(self, EnMb_Stunned);
}

void EnMb_Stunned(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
        player->stateFlags2 &= ~0x80;
        player->actor.parent = NULL;
        player->unk_850 = 200;
        func_8002F71C(globalCtx, &self->actor, 4.0f, self->actor.world.rot.y, 4.0f);
        self->attack = ENMB_ATTACK_NONE;
    }

    if (self->actor.colorFilterTimer == 0) {
        if (self->actor.params == ENMB_TYPE_CLUB) {
            if (self->actor.colChkInfo.health == 0) {
                EnMb_SetupClubDead(self);
            } else if (self->state == ENMB_STATE_CLUB_KNEELING) {
                /* dead code: the setup for self action sets state to something else */
                EnMb_SetupClubDamagedWhileKneeling(self);
            } else {
                EnMb_SetupClubWaitPlayerNear(self);
            }
        } else {
            if (self->actor.colChkInfo.health == 0) {
                EnMb_SetupSpearDead(self);
            } else {
                EnMb_SetupSpearDamaged(self);
            }
        }
    }
}

void EnMb_SpearGuardLookAround(EnMb* self, GlobalContext* globalCtx) {
    s16 timer1;

    SkelAnime_Update(&self->skelAnime);
    if (self->timer1 == 0) {
        timer1 = 0;
    } else {
        self->timer1--;
        timer1 = self->timer1;
    }
    if (timer1 == 0 && Animation_OnFrame(&self->skelAnime, 0.0f)) {
        EnMb_SetupSpearGuardWalk(self);
    }
}

void EnMb_SpearPatrolTurnTowardsWaypoint(EnMb* self, GlobalContext* globalCtx) {
    s16 relYawFromPlayer;

    SkelAnime_Update(&self->skelAnime);

    if (self->timer1 == 0) {
        self->yawToWaypoint = Math_Vec3f_Yaw(&self->actor.world.pos, &self->waypointPos);
        if (Math_SmoothStepToS(&self->actor.shape.rot.y, self->yawToWaypoint, 1, 0x3E8, 0) == 0) {
            self->actor.world.rot.y = self->actor.shape.rot.y;
            EnMb_SetupSpearPatrolWalkTowardsWaypoint(self);
        }
    } else {
        self->timer1--;
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, 1, 0x3E8, 0);
    }

    if (ABS(self->actor.yDistToPlayer) <= 20.0f && EnMb_IsPlayerInCorridor(self, globalCtx)) {
        relYawFromPlayer = self->actor.shape.rot.y - self->actor.yawTowardsPlayer;
        if (ABS(relYawFromPlayer) <= 0x4000 || (func_8002DDE4(globalCtx) && self->actor.xzDistToPlayer < 160.0f)) {
            EnMb_FindWaypointTowardsPlayer(self, globalCtx);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_VOICE);
            EnMb_SetupSpearPrepareAndCharge(self);
        }
    }
}

/**
 * Slow down and resume walking.
 */
void EnMb_SpearEndChargeQuick(EnMb* self, GlobalContext* globalCtx) {
    s32 pad;

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.5f, 1.0f, 0.0f);
    if (self->actor.speedXZ > 1.0f) {
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, false);
    }
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->timer1 == 0) {
            self->timer3--;
            if (self->timer3 == 0) {
                /* Play the charge animation in reverse: let go of the spear and stand normally */
                Animation_Change(&self->skelAnime, &gEnMbSpearPrepareChargeAnim, -1.0f,
                                 Animation_GetLastFrame(&gEnMbSpearPrepareChargeAnim), 0.0f, ANIMMODE_ONCE, 0.0f);
                self->timer1 = 1;
                self->actor.speedXZ = 0.0f;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_SPEAR_NORM);
            }
        } else {
            if (self->actor.params <= ENMB_TYPE_SPEAR_GUARD) {
                EnMb_SetupSpearGuardWalk(self);
                self->timer1 = self->timer2 = self->timer3 = 80;
            } else {
                EnMb_SetupSpearPatrolTurnTowardsWaypoint(self, globalCtx);
            }
        }
    }
}

void EnMb_ClubWaitAfterAttack(EnMb* self, GlobalContext* globalCtx) {
    self->attack = ENMB_ATTACK_NONE;
    if (SkelAnime_Update(&self->skelAnime)) {
        EnMb_SetupClubWaitPlayerNear(self);
    }
}

/**
 * Slow down, charge again if the player is near, or resume walking.
 */
void EnMb_SpearPatrolEndCharge(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 lastFrame;
    s16 relYawFromPlayer;
    s16 yawPlayerToWaypoint;

    if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
        player->stateFlags2 &= ~0x80;
        player->actor.parent = NULL;
        player->unk_850 = 200;
        func_8002F71C(globalCtx, &self->actor, 4.0f, self->actor.world.rot.y, 4.0f);
    }

    if (self->actor.bgCheckFlags & 1) {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.5f, 0.0f);

        if (self->actor.speedXZ > 1.0f) {
            Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, false);
        }

        if (self->timer1 != 0) {
            self->timer3--;
            if (self->timer3 == 0) {
                relYawFromPlayer = self->actor.shape.rot.y - self->actor.yawTowardsPlayer;

                if (ABS(self->actor.yDistToPlayer) <= 20.0f && EnMb_IsPlayerInCorridor(self, globalCtx) &&
                    ABS(relYawFromPlayer) <= 0x4000 && self->actor.xzDistToPlayer <= 200.0f) {
                    EnMb_SetupSpearPrepareAndCharge(self);
                } else {
                    lastFrame = Animation_GetLastFrame(&gEnMbSpearPrepareChargeAnim);
                    /* Play the charge animation in reverse: let go of the spear and stand normally */
                    Animation_Change(&self->skelAnime, &gEnMbSpearPrepareChargeAnim, -1.0f, lastFrame, 0.0f,
                                     ANIMMODE_ONCE, 0.0f);
                    self->actor.speedXZ = 0.0f;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_SPEAR_NORM);
                }
            } else {
                if (self->actor.xzDistToPlayer <= 160.0f) {
                    self->actor.speedXZ = -5.0f;
                } else {
                    self->actor.speedXZ = 0.0f;
                }
            }
        }

        if (SkelAnime_Update(&self->skelAnime)) {
            if (self->timer1 == 0) {
                lastFrame = Animation_GetLastFrame(&gEnMbSpearChargeAnim);
                Animation_Change(&self->skelAnime, &gEnMbSpearChargeAnim, 0.5f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP,
                                 0.0f);
                self->timer1 = 1;
            } else {
                yawPlayerToWaypoint =
                    Math_Vec3f_Yaw(&self->actor.world.pos, &self->waypointPos) - self->actor.yawTowardsPlayer;

                if (ABS(yawPlayerToWaypoint) <= 0x4000) {
                    EnMb_SetupSpearPatrolTurnTowardsWaypoint(self, globalCtx);
                } else {
                    EnMb_SetupSpearPatrolWalkTowardsWaypoint(self);
                }
            }
        }
    }
}

/**
 * Prepare charge (animation), then charge until the player isn't in front.
 */
void EnMb_SpearGuardPrepareAndCharge(EnMb* self, GlobalContext* globalCtx) {
    s32 prevFrame;
    s16 relYawTowardsPlayerAbs = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (relYawTowardsPlayerAbs < 0) {
        relYawTowardsPlayerAbs = -relYawTowardsPlayerAbs;
    }

    prevFrame = self->skelAnime.curFrame;

    if (SkelAnime_Update(&self->skelAnime)) {
        Animation_PlayLoop(&self->skelAnime, &gEnMbSpearChargeAnim);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_ATTACK);
    }

    if (self->timer3 != 0) {
        self->timer3--;
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 0xBB8, 0);
    } else {
        self->actor.speedXZ = 10.0f;
        self->attack = ENMB_ATTACK_SPEAR;
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, false);
        if (prevFrame != (s32)self->skelAnime.curFrame &&
            ((s32)self->skelAnime.curFrame == 2 || (s32)self->skelAnime.curFrame == 6)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_DASH);
        }
    }

    if (relYawTowardsPlayerAbs > 0x1388) {
        self->attack = ENMB_ATTACK_NONE;
        EnMb_SetupSpearEndChargeQuick(self);
    }
}

void EnMb_ClubAttack(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    Vec3f effSpawnPos;
    Vec3f effWhiteShockwaveDynamics = { 0.0f, 0.0f, 0.0f };
    f32 flamesParams[] = { 18.0f, 18.0f, 0.0f };
    s16 flamesUnused[] = { 20, 40, 0 };
    s16 relYawTarget[] = { -0x9C4, 0, 0xDAC };

    Math_SmoothStepToS(&self->actor.shape.rot.y, relYawTarget[self->attack - 1] + self->actor.world.rot.y, 1, 0x2EE, 0);

    if (self->attackCollider.base.atFlags & AT_HIT) {
        self->attackCollider.base.atFlags &= ~AT_HIT;
        if (self->attackCollider.base.at == &player->actor) {
            u8 prevPlayerInvincibilityTimer = player->invincibilityTimer;

            if (player->invincibilityTimer < 0) {
                if (player->invincibilityTimer <= -40) {
                    player->invincibilityTimer = 0;
                } else {
                    player->invincibilityTimer = 0;
                    globalCtx->damagePlayer(globalCtx, -8);
                }
            }

            func_8002F71C(globalCtx, &self->actor, (650.0f - self->actor.xzDistToPlayer) * 0.04f + 4.0f,
                          self->actor.world.rot.y, 8.0f);

            player->invincibilityTimer = prevPlayerInvincibilityTimer;
        }
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->timer3 != 0) {
            self->timer3--;
            if (self->timer3 == 0) {
                f32 lastAnimFrame = Animation_GetLastFrame(&gEnMbClubStrikeDownAnim);
                Animation_Change(&self->skelAnime, &gEnMbClubStrikeDownAnim, 1.5f, 0.0f, lastAnimFrame,
                                 ANIMMODE_ONCE_INTERP, 0.0f);
            }
        } else {
            effSpawnPos = self->effSpawnPos;
            effSpawnPos.y = self->actor.floorHeight;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MONBLIN_HAM_LAND);
            func_800AA000(self->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
            EffectSsBlast_SpawnWhiteShockwave(globalCtx, &effSpawnPos, &effWhiteShockwaveDynamics,
                                              &effWhiteShockwaveDynamics);
            func_80033480(globalCtx, &effSpawnPos, 2.0f, 3, 0x12C, 0xB4, 1);
            Camera_AddQuake(&globalCtx->mainCamera, 2, 0x19, 5);
            func_800358DC(&self->actor, &effSpawnPos, &self->actor.world.rot, flamesParams, 20, flamesUnused, globalCtx,
                          -1, 0);
            EnMb_SetupClubWaitAfterAttack(self);
        }
    } else {
        if (self->timer3 != 0 && self->skelAnime.curFrame == 6.0f) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MONBLIN_HAM_UP);

        } else if (self->timer3 == 0 && self->skelAnime.curFrame == 3.0f) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MONBLIN_HAM_DOWN);
        }
    }
}

/**
 * Prepare charge (animation), then charge to the end of the floor collision.
 */
void EnMb_SpearPatrolPrepareAndCharge(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 prevFrame;
    s32 hasHitPlayer = false;
    s32 endCharge = !Actor_TestFloorInDirection(&self->actor, globalCtx, 110.0f, self->actor.world.rot.y);

    prevFrame = (s32)self->skelAnime.curFrame;
    if (SkelAnime_Update(&self->skelAnime)) {
        Animation_PlayLoop(&self->skelAnime, &gEnMbSpearChargeAnim);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_ATTACK);
    }

    if (self->timer3 != 0) {
        self->timer3--;
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.world.rot.y, 1, 0x1F40, 0);
        endCharge = false;
    } else {
        self->actor.speedXZ = 10.0f;
        self->attack = ENMB_ATTACK_SPEAR;
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, false);
        if (prevFrame != (s32)self->skelAnime.curFrame &&
            ((s32)self->skelAnime.curFrame == 2 || (s32)self->skelAnime.curFrame == 6)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_DASH);
        }
    }

    if (self->attackCollider.base.atFlags & AT_HIT) {
        if (self->attackCollider.base.at == &player->actor) {
            if (!endCharge && !(player->stateFlags2 & 0x80)) {
                if (player->invincibilityTimer < 0) {
                    if (player->invincibilityTimer < -39) {
                        player->invincibilityTimer = 0;
                    } else {
                        player->invincibilityTimer = 0;
                        globalCtx->damagePlayer(globalCtx, -8);
                    }
                }
                if (!(self->attackCollider.base.atFlags & AT_BOUNCED)) {
                    Audio_PlayActorSound2(&player->actor, NA_SE_PL_BODY_HIT);
                }
                if (globalCtx->grabPlayer(globalCtx, player)) {
                    player->actor.parent = &self->actor;
                }
            }
            hasHitPlayer = true;
        } else {
            self->attackCollider.base.atFlags &= ~AT_HIT;
        }
    }

    if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
        player->actor.world.pos.x = self->actor.world.pos.x + Math_CosS(self->actor.shape.rot.y) * 10.0f +
                                    Math_SinS(self->actor.shape.rot.y) * 89.0f;
        hasHitPlayer = true;
        player->actor.world.pos.z = self->actor.world.pos.z + Math_SinS(self->actor.shape.rot.y) * 10.0f +
                                    Math_CosS(self->actor.shape.rot.y) * 89.0f;
        player->unk_850 = 0;
        player->actor.speedXZ = 0.0f;
        player->actor.velocity.y = 0.0f;
    }

    if (endCharge) {
        if (hasHitPlayer || (player->stateFlags2 & 0x80)) {
            self->attackCollider.base.atFlags &= ~AT_HIT;
            if (player->stateFlags2 & 0x80) {
                player->stateFlags2 &= ~0x80;
                player->actor.parent = NULL;
                player->unk_850 = 200;
                func_8002F71C(globalCtx, &self->actor, 4.0f, self->actor.world.rot.y, 4.0f);
            }
        }
        self->attack = ENMB_ATTACK_NONE;
        self->actor.speedXZ = -10.0f;
        EnMb_SetupSpearPatrolEndCharge(self);
    }
}

/**
 * Charge and follow the path, until hitting the player or, after some time, reaching home.
 */
void EnMb_SpearPatrolImmediateCharge(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 prevFrame;
    s32 hasHitPlayer = false;
    s32 endCharge = !Actor_TestFloorInDirection(&self->actor, globalCtx, 110.0f, self->actor.world.rot.y);

    prevFrame = (s32)self->skelAnime.curFrame;
    SkelAnime_Update(&self->skelAnime);

    Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 5.0f, 3, 4.0f, 100, 15, false);
    if (prevFrame != (s32)self->skelAnime.curFrame &&
        ((s32)self->skelAnime.curFrame == 2 || (s32)self->skelAnime.curFrame == 6)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_DASH);
    }

    if (self->attackCollider.base.atFlags & AT_HIT) {
        if (self->attackCollider.base.at == &player->actor) {
            if (!endCharge && !(player->stateFlags2 & 0x80)) {
                if (player->invincibilityTimer < 0) {
                    if (player->invincibilityTimer <= -40) {
                        player->invincibilityTimer = 0;
                    } else {
                        player->invincibilityTimer = 0;
                        globalCtx->damagePlayer(globalCtx, -8);
                    }
                }
                if (!(self->attackCollider.base.atFlags & AT_BOUNCED)) {
                    Audio_PlayActorSound2(&player->actor, NA_SE_PL_BODY_HIT);
                }
                if (globalCtx->grabPlayer(globalCtx, player)) {
                    player->actor.parent = &self->actor;
                }
            }
            hasHitPlayer = true;
        } else {
            self->attackCollider.base.atFlags &= ~AT_HIT;
        }
    }

    if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
        player->actor.world.pos.x = self->actor.world.pos.x + Math_CosS(self->actor.shape.rot.y) * 10.0f +
                                    Math_SinS(self->actor.shape.rot.y) * 89.0f;
        hasHitPlayer = true;
        player->actor.world.pos.z = self->actor.world.pos.z + Math_SinS(self->actor.shape.rot.y) * 10.0f +
                                    Math_CosS(self->actor.shape.rot.y) * 89.0f;
        player->unk_850 = 0;
        player->actor.speedXZ = 0.0f;
        player->actor.velocity.y = 0.0f;
    }

    if (endCharge) {
        if (hasHitPlayer || (player->stateFlags2 & 0x80)) {
            self->attackCollider.base.atFlags &= ~AT_HIT;
            if (player->stateFlags2 & 0x80) {
                player->stateFlags2 &= ~0x80;
                player->actor.parent = NULL;
                player->unk_850 = 200;
                func_8002F71C(globalCtx, &self->actor, 4.0f, self->actor.world.rot.y, 4.0f);
            }
            self->attack = ENMB_ATTACK_NONE;
            self->actor.speedXZ = -10.0f;
            EnMb_SetupSpearPatrolEndCharge(self);
            self->timer3 = 1;
        } else {
            self->timer3--;
            EnMb_NextWaypoint(self, globalCtx);
        }
    }

    EnMb_FaceWaypoint(self, globalCtx);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->timer3 == 0 && Math_Vec3f_DistXZ(&self->actor.home.pos, &self->actor.world.pos) < 80.0f) {
        self->attack = ENMB_ATTACK_NONE;
        EnMb_SetupSpearEndChargeQuick(self);
    }
}

void EnMb_ClubDamaged(EnMb* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->timer3 != 0) {
            Animation_PlayOnce(&self->skelAnime, &gEnMbClubStandUpAnim);
            self->timer3 = 0;
            func_800AA000(self->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
            Camera_AddQuake(&globalCtx->mainCamera, 2, 25, 5);
        } else {
            EnMb_SetupClubWaitPlayerNear(self);
        }
    }
}

void EnMb_ClubDamagedWhileKneeling(EnMb* self, GlobalContext* globalCtx) {
    s32 pad;

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->timer3 != 0) {
            self->timer3--;
            if (self->timer3 == 0) {
                if (self->timer1 == 0) {
                    Animation_Change(&self->skelAnime, &gEnMbClubStandUpAnim, 3.0f, 0.0f,
                                     Animation_GetLastFrame(&gEnMbClubStandUpAnim), ANIMMODE_ONCE_INTERP, 0.0f);
                    self->timer1 = 1;
                    self->timer3 = 6;
                } else {
                    Animation_Change(&self->skelAnime, &gEnMbClubStandUpAnim, 3.0f, 0.0f,
                                     Animation_GetLastFrame(&gEnMbClubStandUpAnim), ANIMMODE_ONCE_INTERP, 0.0f);
                }
            }
        } else {
            EnMb_SetupClubWaitPlayerNear(self);
        }
    }
}

void EnMb_ClubDead(EnMb* self, GlobalContext* globalCtx) {
    Vec3f effPos;
    Vec3f effPosBase;

    effPos = self->actor.world.pos;
    effPos.x += Math_SinS(self->actor.shape.rot.y) * -70.0f;
    effPos.z += Math_CosS(self->actor.shape.rot.y) * -70.0f;
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    effPosBase = effPos;

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->timer1 > 0) {
            Vec3f effZeroVec = { 0.0f, 0.0f, 0.0f };
            s32 i;

            self->timer1--;
            for (i = 4; i >= 0; i--) {
                effPos.x = Rand_CenteredFloat(240.0f) + effPosBase.x;
                effPos.y = Rand_CenteredFloat(15.0f) + (effPosBase.y + 20.0f);
                effPos.z = Rand_CenteredFloat(240.0f) + effPosBase.z;
                EffectSsDeadDb_Spawn(globalCtx, &effPos, &effZeroVec, &effZeroVec, 230, 7, 255, 255, 255, 255, 0, 255,
                                     0, 1, 9, true);
            }
        } else {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &effPos, 0xC0);
            Actor_Kill(&self->actor);
        }
    } else if ((s32)self->skelAnime.curFrame == 15 || (s32)self->skelAnime.curFrame == 22) {
        func_800AA000(self->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &effPos, 50.0f, 10, 3.0f, 400, 60, false);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_DOWN);
        Camera_AddQuake(&globalCtx->mainCamera, 2, 25, 5);
    }
}

/**
 * Walk around the home point, face and charge the player if close.
 */
void EnMb_SpearGuardWalk(EnMb* self, GlobalContext* globalCtx) {
    s32 prevFrame;
    s32 beforeCurFrame;
    s32 pad1;
    s32 pad2;
    Player* player = GET_PLAYER(globalCtx);
    s16 relYawTowardsPlayer = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    s16 yawTowardsHome;
    f32 playSpeedAbs;

    relYawTowardsPlayer = ABS(relYawTowardsPlayer);
    Math_SmoothStepToF(&self->actor.speedXZ, 0.59999996f, 0.1f, 1.0f, 0.0f);
    self->skelAnime.playSpeed = self->actor.speedXZ;
    prevFrame = self->skelAnime.curFrame;
    SkelAnime_Update(&self->skelAnime);

    playSpeedAbs = ABS(self->skelAnime.playSpeed);
    beforeCurFrame = self->skelAnime.curFrame - playSpeedAbs;
    playSpeedAbs = ABS(self->skelAnime.playSpeed);
    if (self->timer3 == 0 &&
        Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < self->playerDetectionRange) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        self->actor.flags |= 1;
        if (self->actor.xzDistToPlayer < 500.0f && relYawTowardsPlayer < 0x1388) {
            EnMb_SetupSpearPrepareAndCharge(self);
        }
    } else {
        self->actor.flags &= ~1;
        if (Math_Vec3f_DistXZ(&self->actor.world.pos, &self->actor.home.pos) > self->maxHomeDist || self->timer2 != 0) {
            yawTowardsHome = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
            Math_SmoothStepToS(&self->actor.world.rot.y, yawTowardsHome, 1, 0x2EE, 0);
        }
        if (self->timer2 != 0) {
            self->timer2--;
        }
        if (self->timer3 != 0) {
            self->timer3--;
        }
        if (self->timer2 == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_VOICE);
        }
        self->timer1--;
        if (self->timer1 == 0) {
            if (Rand_ZeroOne() > 0.7f) {
                self->timer1 = Rand_S16Offset(50, 70);
                self->timer2 = Rand_S16Offset(15, 40);
            } else {
                EnMb_SetupSpearGuardLookAround(self);
            }
        }
    }

    if (prevFrame != (s32)self->skelAnime.curFrame) {
        if ((beforeCurFrame <= 1 && prevFrame + (s32)playSpeedAbs >= 1) ||
            (beforeCurFrame <= 20 && prevFrame + (s32)playSpeedAbs >= 20)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_WALK);
        }
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnMb_SpearPatrolWalkTowardsWaypoint(EnMb* self, GlobalContext* globalCtx) {
    s32 prevFrame;
    s32 beforeCurFrame;
    s16 relYawTowardsPlayer;
    f32 yDistToPlayerAbs;
    f32 playSpeedABS;

    if (Math_Vec3f_DistXZ(&self->waypointPos, &self->actor.world.pos) <= 8.0f ||
        (Rand_ZeroOne() < 0.1f && Math_Vec3f_DistXZ(&self->actor.home.pos, &self->actor.world.pos) <= 4.0f)) {
        EnMb_SetupSpearPatrolTurnTowardsWaypoint(self, globalCtx);
    } else {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.59999996f, 0.1f, 1.0f, 0.0f);
        self->skelAnime.playSpeed = 2.0f * self->actor.speedXZ;
    }

    self->yawToWaypoint = Math_Vec3f_Yaw(&self->actor.world.pos, &self->waypointPos);
    Math_SmoothStepToS(&self->actor.world.rot.y, self->yawToWaypoint, 1, 0x5DC, 0);

    yDistToPlayerAbs = (self->actor.yDistToPlayer >= 0.0f) ? self->actor.yDistToPlayer : -self->actor.yDistToPlayer;
    if (yDistToPlayerAbs <= 20.0f && EnMb_IsPlayerInCorridor(self, globalCtx)) {
        relYawTowardsPlayer = (self->actor.shape.rot.y - self->actor.yawTowardsPlayer);
        if (ABS(relYawTowardsPlayer) <= 0x4000 || (func_8002DDE4(globalCtx) && self->actor.xzDistToPlayer < 160.0f)) {
            EnMb_FindWaypointTowardsPlayer(self, globalCtx);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_VOICE);
            EnMb_SetupSpearPrepareAndCharge(self);
            return;
        }
    }

    if (self->timer2 != 0) {
        self->timer2--;
    }
    if (self->timer3 != 0) {
        self->timer3--;
    }
    if (self->timer2 == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_VOICE);
        self->timer2 = Rand_S16Offset(30, 70);
    }

    prevFrame = self->skelAnime.curFrame;
    SkelAnime_Update(&self->skelAnime);
    playSpeedABS = ABS(self->skelAnime.playSpeed);
    beforeCurFrame = self->skelAnime.curFrame - playSpeedABS;
    playSpeedABS = (self->skelAnime.playSpeed >= 0.0f) ? self->skelAnime.playSpeed : -self->skelAnime.playSpeed;
    if (prevFrame != (s32)self->skelAnime.curFrame) {
        if ((beforeCurFrame <= 1 && (s32)playSpeedABS + prevFrame >= 1) ||
            (beforeCurFrame <= 20 && (s32)playSpeedABS + prevFrame >= 20)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_WALK);
        }
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnMb_ClubWaitPlayerNear(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    s16 relYawFromPlayer = self->actor.world.rot.y - self->actor.yawTowardsPlayer;

    SkelAnime_Update(&self->skelAnime);
    if (Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < self->playerDetectionRange &&
        !(player->stateFlags1 & 0x4000000) && ABS(relYawFromPlayer) < 0x3E80) {
        EnMb_SetupClubAttack(self);
    }
}

void EnMb_SetupSpearDamaged(EnMb* self) {
    s16 relYawTowardsPlayer = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(relYawTowardsPlayer) <= 0x4000) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gEnMbSpearDamagedFromFrontAnim, -4.0f);
        self->actor.speedXZ = -8.0f;
    } else {
        Animation_MorphToPlayOnce(&self->skelAnime, &gEnMbSpearDamagedFromBehindAnim, -4.0f);
        self->actor.speedXZ = 8.0f;
    }

    self->timer1 = 30;
    self->state = ENMB_STATE_SPEAR_SPEARPATH_DAMAGED;
    self->actor.shape.rot.y = self->actor.world.rot.y;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_DEAD);
    EnMb_SetupAction(self, EnMb_SpearDamaged);
}

void EnMb_SpearDamaged(EnMb* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.params <= ENMB_TYPE_SPEAR_GUARD) {
            EnMb_SetupSpearGuardLookAround(self);
        } else {
            EnMb_SetupSpearPatrolImmediateCharge(self);
        }
    }
}

void EnMb_SetupSpearDead(EnMb* self) {
    s16 relYawTowardsPlayer = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(relYawTowardsPlayer) <= 0x4000) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gEnMbSpearFallOnItsBackAnim, -4.0f);
        self->actor.speedXZ = -8.0f;
    } else {
        /* The gEnMbSpearFallFaceDownAnim animation was probably meant to be used here */
        Animation_MorphToPlayOnce(&self->skelAnime, &gEnMbSpearFallOnItsBackAnim, -4.0f);
        self->actor.speedXZ = 8.0f;
    }

    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->timer1 = 30;
    self->state = ENMB_STATE_SPEAR_SPEARPATH_DAMAGED;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_DEAD);
    self->actor.flags &= ~1;
    EnMb_SetupAction(self, EnMb_SpearDead);
}

void EnMb_SpearDead(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);

    if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
        player->stateFlags2 &= ~0x80;
        player->actor.parent = NULL;
        player->unk_850 = 200;
        func_8002F71C(globalCtx, &self->actor, 4.0f, self->actor.world.rot.y, 4.0f);
        self->attack = ENMB_ATTACK_NONE;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->timer1 > 0) {
            Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
            s32 i;
            Vec3f effPos;

            self->actor.shape.shadowScale = 0.0f;
            self->timer1--;
            for (i = 4; i >= 0; i--) {
                effPos.x = Rand_CenteredFloat(110.0f) + self->actor.world.pos.x;
                effPos.y = Rand_CenteredFloat(15.0f) + (self->actor.world.pos.y + 20.0f);
                effPos.z = Rand_CenteredFloat(110.0f) + self->actor.world.pos.z;

                EffectSsDeadDb_Spawn(globalCtx, &effPos, &zeroVec, &zeroVec, 100, 7, 255, 255, 255, 255, 0, 255, 0, 1,
                                     9, true);
            }
        } else {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xE0);
            Actor_Kill(&self->actor);
        }
    }
}

void EnMb_SpearUpdateAttackCollider(Actor* thisx, GlobalContext* globalCtx) {
    Vec3f quadModel0 = { 1000.0f, 1500.0f, 0.0f };
    Vec3f quadModel1 = { -1000.0f, 1500.0f, 0.0f };
    Vec3f quadModel2 = { 1000.0f, 1500.0f, 4500.0f };
    Vec3f quadModel3 = { -1000.0f, 1500.0f, 4500.0f };
    EnMb* self = THIS;

    if (self->actor.params >= ENMB_TYPE_SPEAR_PATROL) {
        quadModel0.x += 2000.0f;
        quadModel0.z = -4000.0f;
        quadModel1.z = -4000.0f;
        quadModel2.x += 2000.0f;
        quadModel1.x -= 2000.0f;
        quadModel3.x -= 2000.0f;
        quadModel2.z += 4000.0f;
        quadModel3.z += 4000.0f;
    }
    Matrix_MultVec3f(&quadModel0, &self->attackCollider.dim.quad[1]);
    Matrix_MultVec3f(&quadModel1, &self->attackCollider.dim.quad[0]);
    Matrix_MultVec3f(&quadModel2, &self->attackCollider.dim.quad[3]);
    Matrix_MultVec3f(&quadModel3, &self->attackCollider.dim.quad[2]);
    Collider_SetQuadVertices(&self->attackCollider, &self->attackCollider.dim.quad[0],
                             &self->attackCollider.dim.quad[1], &self->attackCollider.dim.quad[2],
                             &self->attackCollider.dim.quad[3]);
}

void EnMb_ClubUpdateAttackCollider(Actor* thisx, GlobalContext* globalCtx) {
    static Vec3f quadModel[] = { { 1000.0f, 0.0f, 0.0f },
                                 { 1000.0f, 0.0f, 0.0f },
                                 { 1000.0f, -8000.0f, -1500.0f },
                                 { 1000.0f, -9000.0f, 2000.0f } };
    EnMb* self = THIS;

    Matrix_MultVec3f(&quadModel[0], &self->attackCollider.dim.quad[1]);
    Matrix_MultVec3f(&quadModel[1], &self->attackCollider.dim.quad[0]);
    Matrix_MultVec3f(&quadModel[2], &self->attackCollider.dim.quad[3]);
    Matrix_MultVec3f(&quadModel[3], &self->attackCollider.dim.quad[2]);
    Collider_SetQuadVertices(&self->attackCollider, &self->attackCollider.dim.quad[0],
                             &self->attackCollider.dim.quad[1], &self->attackCollider.dim.quad[2],
                             &self->attackCollider.dim.quad[3]);
}

void EnMb_CheckColliding(EnMb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->frontShielding.base.acFlags & AC_HIT) {
        self->frontShielding.base.acFlags &= ~(AC_HIT | AC_BOUNCED);
        self->hitbox.base.acFlags &= ~AC_HIT;
    } else if ((self->hitbox.base.acFlags & AC_HIT) && self->state >= ENMB_STATE_STUNNED) {
        self->hitbox.base.acFlags &= ~AC_HIT;
        if (self->actor.colChkInfo.damageEffect != ENMB_DMGEFF_IGNORE &&
            self->actor.colChkInfo.damageEffect != ENMB_DMGEFF_FREEZE) {
            if ((player->stateFlags2 & 0x80) && player->actor.parent == &self->actor) {
                player->stateFlags2 &= ~0x80;
                player->actor.parent = NULL;
                player->unk_850 = 200;
                func_8002F71C(globalCtx, &self->actor, 6.0f, self->actor.world.rot.y, 6.0f);
            }
            self->damageEffect = self->actor.colChkInfo.damageEffect;
            self->attack = ENMB_ATTACK_NONE;
            Actor_SetDropFlag(&self->actor, &self->hitbox.info, false);
            if (self->actor.colChkInfo.damageEffect == ENMB_DMGEFF_STUN ||
                self->actor.colChkInfo.damageEffect == ENMB_DMGEFF_STUN_ICE) {
                if (self->state != ENMB_STATE_STUNNED) {
                    Actor_ApplyDamage(&self->actor);
                    EnMb_SetupStunned(self);
                }
            } else {
                Actor_ApplyDamage(&self->actor);
                Actor_SetColorFilter(&self->actor, 0x4000, 0xFA, 0, 0xC);
                if (self->actor.params == ENMB_TYPE_CLUB) {
                    if (self->actor.colChkInfo.health == 0) {
                        EnMb_SetupClubDead(self);
                    } else if (self->state != ENMB_STATE_CLUB_KNEELING) {
                        EnMb_SetupClubDamaged(self);
                    }
                } else {
                    if (self->actor.colChkInfo.health == 0) {
                        EnMb_SetupSpearDead(self);
                    } else {
                        EnMb_SetupSpearDamaged(self);
                    }
                }
            }
        }
    }
}

void EnMb_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnMb* self = THIS;
    s32 pad;

    EnMb_CheckColliding(self, globalCtx);
    if (thisx->colChkInfo.damageEffect != ENMB_DMGEFF_FREEZE) {
        self->actionFunc(self, globalCtx);
        Actor_MoveForward(thisx);
        Actor_UpdateBgCheckInfo(globalCtx, thisx, 40.0f, 40.0f, 70.0f, 0x1D);
        Actor_SetFocus(thisx, thisx->scale.x * 4500.0f);
        Collider_UpdateCylinder(thisx, &self->hitbox);
        if (thisx->colChkInfo.health <= 0) {
            self->hitbox.dim.pos.x += Math_SinS(thisx->shape.rot.y) * (-4400.0f * thisx->scale.y);
            self->hitbox.dim.pos.z += Math_CosS(thisx->shape.rot.y) * (-4400.0f * thisx->scale.y);
        }
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->hitbox.base);
        if (self->state >= ENMB_STATE_STUNNED &&
            (thisx->params == ENMB_TYPE_CLUB || self->state != ENMB_STATE_ATTACK)) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->hitbox.base);
        }
        if (self->state >= ENMB_STATE_IDLE) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->frontShielding.base);
        }
        if (self->attack > ENMB_ATTACK_NONE) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->attackCollider.base);
        }
    }
}

void EnMb_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f unused = { 1100.0f, -700.0f, 0.0f };
    static Vec3f feetPos = { 0.0f, 0.0f, 0.0f };
    static Vec3f effSpawnPosModel = { 0.0f, -8000.0f, 0.0f };
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    s32 bodyPart = -1;
    EnMb* self = THIS;
    Vec3f bodyPartPos;

    if (self->actor.params == ENMB_TYPE_CLUB) {
        if (limbIndex == ENMB_LIMB_LHAND) {
            Matrix_MultVec3f(&effSpawnPosModel, &self->effSpawnPos);
            if (self->attack > ENMB_ATTACK_NONE) {
                EnMb_ClubUpdateAttackCollider(&self->actor, globalCtx);
            }
        }
        Actor_SetFeetPos(&self->actor, limbIndex, ENMB_LIMB_LFOOT, &feetPos, ENMB_LIMB_RFOOT, &feetPos);
    }

    if (self->iceEffectTimer != 0) {
        switch (limbIndex) {
            case ENMB_LIMB_HEAD:
                bodyPart = 0;
                break;
            case ENMB_LIMB_LHAND:
                bodyPart = 1;
                break;
            case ENMB_LIMB_RHAND:
                bodyPart = 2;
                break;
            case ENMB_LIMB_LSHOULDER:
                bodyPart = 3;
                break;
            case ENMB_LIMB_RSHOULDER:
                bodyPart = 4;
                break;
            case ENMB_LIMB_CHEST:
                bodyPart = 5;
                break;
            case ENMB_LIMB_LTHIGH:
                bodyPart = 6;
                break;
            case ENMB_LIMB_RTHIGH:
                bodyPart = 7;
                break;
            case ENMB_LIMB_LFOOT:
                bodyPart = 8;
                break;
            case ENMB_LIMB_RFOOT:
                bodyPart = 9;
                break;
        }
        if (bodyPart >= 0) {
            Matrix_MultVec3f(&zeroVec, &bodyPartPos);
            self->bodyPartsPos[bodyPart].x = bodyPartPos.x;
            self->bodyPartsPos[bodyPart].y = bodyPartPos.y;
            self->bodyPartsPos[bodyPart].z = bodyPartPos.z;
        }
    }
}

void EnMb_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Vec3f frontShieldingTriModel0[] = {
        { 4000.0f, 7000.0f, 3500.0f },
        { 4000.0f, 0.0f, 3500.0f },
        { -4000.0f, 7000.0f, 3500.0f },
    };
    static Vec3f frontShieldingTriModel1[] = {
        { -4000.0f, 7000.0f, 3500.0f },
        { -4000.0f, 0.0f, 3500.0f },
        { 4000.0f, 0.0f, 3500.0f },
    };
    s32 i;
    f32 scale;
    Vec3f frontShieldingTri0[3];
    Vec3f frontShieldingTri1[3];
    s32 bodyPartIdx;
    EnMb* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, EnMb_PostLimbDraw, thisx);

    if (thisx->params != ENMB_TYPE_CLUB) {
        if (self->attack > ENMB_ATTACK_NONE) {
            EnMb_SpearUpdateAttackCollider(thisx, globalCtx);
        }
        for (i = 0; i < 3; i++) {
            Matrix_MultVec3f(&frontShieldingTriModel0[i], &frontShieldingTri0[i]);
            Matrix_MultVec3f(&frontShieldingTriModel1[i], &frontShieldingTri1[i]);
        }
        Collider_SetTrisVertices(&self->frontShielding, 0, &frontShieldingTri0[0], &frontShieldingTri0[1],
                                 &frontShieldingTri0[2]);
        Collider_SetTrisVertices(&self->frontShielding, 1, &frontShieldingTri1[0], &frontShieldingTri1[1],
                                 &frontShieldingTri1[2]);
    }

    if (self->iceEffectTimer != 0) {
        thisx->colorFilterTimer++;
        if (self->iceEffectTimer >= 0) {
            self->iceEffectTimer--;
        }
        if ((self->iceEffectTimer % 4) == 0) {
            scale = 2.5f;
            if (thisx->params == ENMB_TYPE_CLUB) {
                scale = 4.0f;
            }
            bodyPartIdx = self->iceEffectTimer >> 2;
            EffectSsEnIce_SpawnFlyingVec3s(globalCtx, thisx, &self->bodyPartsPos[bodyPartIdx], 150, 150, 150, 250, 235,
                                           245, 255, scale);
        }
    }
}
