/*
 * File: z_boss_mo.c
 * Overlay: ovl_Boss_Mo
 * Description: Morpha
 */

#include "z_boss_mo.h"
#include "objects/object_mo/object_mo.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "vt.h"

#define FLAGS 0x00000035

#define THIS ((BossMo*)thisx)

#define MO_WATER_LEVEL(globalCtx) globalCtx->colCtx.colHeader->waterBoxes[0].ySurface

#define HAS_LINK(tent) \
    ((tent != NULL) && \
     ((tent->work[MO_TENT_ACTION_STATE] == MO_TENT_GRAB) || (tent->work[MO_TENT_ACTION_STATE] == MO_TENT_SHAKE)))

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ Vec3f accel;
    /* 0x24 */ u8 type;
    /* 0x25 */ u8 timer;
    /* 0x26 */ u8 stopTimer;
    /* 0x28 */ s16 unk_28; // unused?
    /* 0x2A */ s16 alpha;
    /* 0x2C */ s16 rippleMode;
    /* 0x2E */ s16 maxAlpha;
    /* 0x30 */ f32 scale;
    /* 0x30 */ f32 fwork[2];
    /* 0x3C */ Vec3f* targetPos;
} BossMoEffect; // size = 0x40

#define MO_FX_MAX_SIZE 0
#define MO_FX_SHIMMER 0
#define MO_FX_SUCTION 0

#define MO_FX_SPREAD_RATE 1
#define MO_FX_STRETCH 1
#define MO_FX_MAX_SCALE 1

void BossMo_Init(Actor* thisx, GlobalContext* globalCtx);
void BossMo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BossMo_UpdateCore(Actor* thisx, GlobalContext* globalCtx);
void BossMo_UpdateTent(Actor* thisx, GlobalContext* globalCtx);
void BossMo_DrawCore(Actor* thisx, GlobalContext* globalCtx);
void BossMo_DrawTent(Actor* thisx, GlobalContext* globalCtx);

void BossMo_UpdateEffects(BossMo* self, GlobalContext* globalCtx);
void BossMo_DrawEffects(BossMoEffect* effect, GlobalContext* globalCtx);

void BossMo_SetupTentacle(BossMo* self, GlobalContext* globalCtx);
void BossMo_Tentacle(BossMo* self, GlobalContext* globalCtx);

void BossMo_Unknown(void);

typedef enum {
    /* 0 */ MO_FX_NONE,
    /* 1 */ MO_FX_SMALL_RIPPLE,
    /* 2 */ MO_FX_BIG_RIPPLE,
    /* 3 */ MO_FX_DROPLET,
    /* 4 */ MO_FX_SPLASH,
    /* 5 */ MO_FX_SPLASH_TRAIL,
    /* 6 */ MO_FX_WET_SPOT,
    /* 7 */ MO_FX_BUBBLE
} BossMoEffectType;

typedef enum {
    /*   0 */ MO_TENT_READY,
    /*   1 */ MO_TENT_SWING,
    /*   2 */ MO_TENT_ATTACK,
    /*   3 */ MO_TENT_CURL,
    /*   4 */ MO_TENT_GRAB,
    /*   5 */ MO_TENT_SHAKE,
    /*  10 */ MO_TENT_WAIT = 10,
    /*  11 */ MO_TENT_SPAWN,
    /* 100 */ MO_TENT_CUT = 100,
    /* 101 */ MO_TENT_RETREAT,
    /* 102 */ MO_TENT_DESPAWN,
    /* 200 */ MO_TENT_DEATH_START = 200,
    /* 201 */ MO_TENT_DEATH_1,
    /* 202 */ MO_TENT_DEATH_2,
    /* 203 */ MO_TENT_DEATH_3,
    /* 205 */ MO_TENT_DEATH_5 = 205,
    /* 206 */ MO_TENT_DEATH_6
} BossMoTentState;

typedef enum {
    /* -11 */ MO_CORE_UNUSED = -11,
    /*   0 */ MO_CORE_MOVE = 0,
    /*   1 */ MO_CORE_MAKE_TENT,
    /*   2 */ MO_CORE_UNDERWATER,
    /*   5 */ MO_CORE_STUNNED = 5,
    /*  10 */ MO_CORE_ATTACK = 10,
    /*  11 */ MO_CORE_RETREAT,
    /*  20 */ MO_CORE_INTRO_WAIT = 20,
    /*  21 */ MO_CORE_INTRO_REVEAL
} BossMoCoreState;

typedef enum {
    /*   0 */ MO_BATTLE,
    /*   1 */ MO_INTRO_WAIT,
    /*   2 */ MO_INTRO_START,
    /*   3 */ MO_INTRO_SWIM,
    /*   4 */ MO_INTRO_REVEAL,
    /*   5 */ MO_INTRO_FINISH,
    /* 100 */ MO_DEATH_START = 100,
    /* 101 */ MO_DEATH_DRAIN_WATER_1,
    /* 102 */ MO_DEATH_DRAIN_WATER_2,
    /* 103 */ MO_DEATH_CEILING,
    /* 104 */ MO_DEATH_DROPLET,
    /* 105 */ MO_DEATH_FINISH,
    /* 150 */ MO_DEATH_MO_CORE_BURST = 150
} BossMoCsState;

const ActorInit Boss_Mo_InitVars = {
    ACTOR_BOSS_MO,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_MO,
    sizeof(BossMo),
    (ActorFunc)BossMo_Init,
    (ActorFunc)BossMo_Destroy,
    (ActorFunc)BossMo_UpdateTent,
    (ActorFunc)BossMo_DrawTent,
};

static BossMo* sMorphaCore = NULL;
static BossMo* sMorphaTent1 = NULL;
static BossMo* sMorphaTent2 = NULL;

static f32 sFlatWidth[41] = {
    15.0f, 12.0f, 9.0f, 6.5f, 4.8f, 4.0f, 3.4f, 3.1f, 3.0f, 3.1f, 3.2f, 3.4f, 3.6f, 3.8f,
    4.0f,  4.6f,  5.1f, 5.5f, 6.1f, 6.6f, 7.3f, 7.7f, 8.4f, 8.5f, 8.7f, 8.8f, 8.8f, 8.7f,
    8.6f,  8.3f,  8.2f, 8.1f, 7.2f, 6.7f, 5.9f, 4.9f, 2.7f, 0.0f, 0.0f, 0.0f, 0.0f,
};

#include "z_boss_mo_colchk.c"

static BossMoEffect sEffects[300];
static s32 sSeed1;
static s32 sSeed2;
static s32 sSeed3;

void BossMo_InitRand(s32 seedInit0, s32 seedInit1, s32 seedInit2) {
    sSeed1 = seedInit0;
    sSeed2 = seedInit1;
    sSeed3 = seedInit2;
}

f32 BossMo_RandZeroOne(void) {
    // Wichmann-Hill algorithm
    f32 randFloat;

    sSeed1 = (sSeed1 * 171) % 30269;
    sSeed2 = (sSeed2 * 172) % 30307;
    sSeed3 = (sSeed3 * 170) % 30323;

    randFloat = (sSeed1 / 30269.0f) + (sSeed2 / 30307.0f) + (sSeed3 / 30323.0f);
    while (randFloat >= 1.0f) {
        randFloat -= 1.0f;
    }
    return fabsf(randFloat);
}

s32 BossMo_NearLand(Vec3f* pos, f32 margin) {
    if (450.0f - margin <= fabsf(pos->x)) {
        return true;
    }
    if (450.0f - margin <= fabsf(pos->z)) {
        return true;
    }
    if ((fabsf(pos->x - 180.0f) < 90.0f + margin) || (fabsf(pos->x - -180.0f) < 90.0f + margin)) {
        if (fabsf(pos->z - 180.0f) < 90.0f + margin) {
            return true;
        }
        if (fabsf(pos->z - -180.0f) < 90.0f + margin) {
            return true;
        }
    }
    return false;
}

void BossMo_SpawnRipple(BossMoEffect* effect, Vec3f* pos, f32 scale, f32 maxScale, s16 maxAlpha, s16 partLimit,
                        u8 type) {
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    s16 i;

    for (i = 0; i < partLimit; i++, effect++) {
        if (effect->type == MO_FX_NONE) {
            effect->stopTimer = 0;
            effect->type = type;
            effect->pos = *pos;
            effect->vel = zeroVec;
            effect->accel = zeroVec;
            effect->scale = scale * 0.0025f;
            effect->fwork[MO_FX_MAX_SIZE] = maxScale * 0.0025f;
            if (scale > 300.0f) {
                effect->alpha = 0;
                effect->maxAlpha = maxAlpha;
                effect->rippleMode = 0;
                effect->fwork[MO_FX_SPREAD_RATE] = (effect->fwork[MO_FX_MAX_SIZE] - effect->scale) * 0.05f;
            } else {
                effect->alpha = maxAlpha;
                effect->rippleMode = 1;
                effect->fwork[MO_FX_SPREAD_RATE] = (effect->fwork[MO_FX_MAX_SIZE] - effect->scale) * 0.1f;
            }
            break;
        }
    }
}

void BossMo_SpawnDroplet(s16 type, BossMoEffect* effect, Vec3f* pos, Vec3f* vel, f32 scale) {
    s16 i;
    Vec3f gravity = { 0.0f, -1.0f, 0.0f };

    for (i = 0; i < 290; i++, effect++) {
        if (effect->type == MO_FX_NONE) {
            effect->type = type;
            effect->pos = *pos;
            effect->vel = *vel;
            effect->accel = gravity;
            if (type == MO_FX_SPLASH_TRAIL) {
                effect->accel.y = 0.0f;
            }
            effect->scale = scale;
            effect->fwork[MO_FX_SPREAD_RATE] = 1.0f;
            effect->stopTimer = 0;
            break;
        }
    }
}

void BossMo_SpawnStillDroplet(BossMoEffect* effect, Vec3f* pos, f32 scale) {
    s16 i;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    for (i = 0; i < 290; i++, effect++) {
        if (effect->type == MO_FX_NONE) {
            effect->type = MO_FX_DROPLET;
            effect->stopTimer = 2;
            effect->pos = *pos;
            effect->vel = zeroVec;
            effect->accel = zeroVec;
            effect->scale = scale;
            effect->fwork[MO_FX_SPREAD_RATE] = 1.0f;
            break;
        }
    }
}

void BossMo_SpawnBubble(BossMoEffect* effect, Vec3f* pos, Vec3f* vel, Vec3f* accel, f32 scale, Vec3f* targetPos) {
    s16 i;

    for (i = 0; i < 280; i++, effect++) {
        if (effect->type == MO_FX_NONE) {
            effect->type = MO_FX_BUBBLE;
            effect->stopTimer = 0;
            effect->pos = *pos;
            effect->vel = *vel;
            effect->accel = *accel;
            effect->scale = scale;
            effect->fwork[MO_FX_SUCTION] = 0.0f;
            effect->targetPos = targetPos;
            if (targetPos == NULL) {
                effect->alpha = 255;
            } else {
                effect->alpha = 0;
            }
            effect->timer = 0;
            break;
        }
    }
}

static s16 sCurlRot[41] = {
    0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  4, 8, 8, 8, 9, 9, 9,
    9, 9, 9, 12, 15, 15, 15, 15, 15, 15, 15, 20, 20, 20, 0, 0, 0, 0, 0, 0,
};
static s16 sGrabRot[41] = {
    0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, -5, -5, -5,
    0, 5, 10, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 0, 0, 0, 0,  0,
};
static s16 sAttackRot[41] = {
    0, 5, 6, 7, 8, 8, 7, 6, 6, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x25, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 0, ICHAIN_STOP),
};

static Vec3f sAudioZeroVec = { 0.0f, 0.0f, 0.0f };

static u8 sTentSpawnIndex[21] = { 0, 1, 2, 3, 4, 15, 19, 5, 14, 16, 17, 18, 6, 13, 20, 7, 12, 11, 10, 9, 8 };

static Vec2f sTentSpawnPos[21] = {
    { -360.0f, -360.0f }, { -180.0f, -360.0f }, { 0.0f, -360.0f },   { 180.0f, -360.0f }, { 360.0f, -360.0f },
    { -360.0f, -180.0f }, { 0.0f, -180.0f },    { 360.0f, -180.0f }, { -360.0f, 0.0f },   { -180.0f, 0.0f },
    { 0.0f, 0.0f },       { 180.0f, 0.0f },     { 360.0f, 0.0f },    { -360.0f, 180.0f }, { 0.0f, 180.0f },
    { 360.0f, 180.0f },   { -360.0f, 360.0f },  { -180.0f, 360.0f }, { 0.0f, 360.0f },    { 180.0f, 360.0f },
    { 360.0f, 360.0f },
};

static f32 sTentWidth[41] = {
    3.56f, 3.25f, 2.96f, 2.69f, 2.44f, 2.21f, 2.0f, 1.81f, 1.64f, 1.49f, 1.36f, 1.25f, 1.16f, 1.09f,
    1.04f, 1.01f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  0.98f, 0.95f, 0.9f, 0.8f,  0.6f,  1.0f,  1.0f,  1.0f,  1.0f,
};

static f32 sDropletWidth[41] = {
    0.0f,      2.95804f,  4.123106f, 4.974937f, 5.656854f, 6.22495f,  6.708204f, 7.123903f, 7.483315f,
    7.794229f, 8.062258f, 8.291562f, 8.485281f, 8.645808f, 8.774964f, 8.87412f,  8.944272f, 8.9861f,
    9.0f,      8.9861f,   8.944272f, 8.87412f,  8.774964f, 8.645808f, 8.485281f, 8.291562f, 8.062258f,
    7.794229f, 7.483315f, 7.123903f, 6.708204f, 6.22495f,  5.656854f, 4.974937f, 4.123106f, 2.95804f,
    0.0f,      0.0f,      0.0f,      0.0f,      0.0f,
}; // These are sqrt(9^2 - (i/2 - 9)^2), a sphere of radius 9.

void BossMo_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BossMo* self = THIS;
    u16 i;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    if (self->actor.params != BOSSMO_TENTACLE) {
        Flags_SetSwitch(globalCtx, 0x14);
        sMorphaCore = self;
        MO_WATER_LEVEL(globalCtx) = self->waterLevel = MO_WATER_LEVEL(globalCtx);
        globalCtx->roomCtx.unk_74[0] = 0xA0;
        globalCtx->specialEffects = sEffects;
        for (i = 0; i < ARRAY_COUNT(sEffects); i++) {
            sEffects[i].type = MO_FX_NONE;
        }
        self->actor.world.pos.x = 200.0f;
        self->actor.world.pos.y = MO_WATER_LEVEL(globalCtx) + 50.0f;
        self->fwork[MO_TENT_SWING_SIZE_X] = 5.0f;
        self->drawActor = true;
        self->actor.colChkInfo.health = 20;
        self->actor.colChkInfo.mass = 0;
        self->actor.params = 0;
        Actor_SetScale(&self->actor, 0.01f);
        Collider_InitCylinder(globalCtx, &self->coreCollider);
        Collider_SetCylinder(globalCtx, &self->coreCollider, &self->actor, &sCylinderInit);
        if (Flags_GetClear(globalCtx, globalCtx->roomCtx.curRoom.num)) {
            Actor_Kill(&self->actor);
            Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, 0.0f, -280.0f, 0.0f, 0,
                               0, 0, WARP_DUNGEON_ADULT);
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, -200.0f, -280.0f, 0.0f, 0, 0, 0, 0);
            globalCtx->roomCtx.unk_74[0] = 0xFF;
            MO_WATER_LEVEL(globalCtx) = -500;
            return;
        }
        if (gSaveContext.eventChkInf[7] & 0x10) {
            Audio_QueueSeqCmd(0x1B);
            self->tentMaxAngle = 5.0f;
            self->timers[0] = 50;
        } else {
            self->csState = MO_INTRO_WAIT;
            self->work[MO_TENT_ACTION_STATE] = MO_CORE_INTRO_WAIT;
            self->actor.world.pos.x = 1000.0f;
            self->timers[0] = 60;
        }
        sMorphaTent1 = (BossMo*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_BOSS_MO,
                                                   self->actor.world.pos.x, self->actor.world.pos.y,
                                                   self->actor.world.pos.z, 0, 0, 0, BOSSMO_TENTACLE);
        self->actor.draw = BossMo_DrawCore;
        self->actor.update = BossMo_UpdateCore;
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_BOSS);
    } else {
        Actor_SetScale(&self->actor, 0.01f);
        BossMo_SetupTentacle(self, globalCtx);
        self->actor.colChkInfo.mass = 0xFF;
        MO_WATER_LEVEL(globalCtx) = -50;
        self->waterTexAlpha = 90.0f;
        self->actor.world.pos.y = MO_WATER_LEVEL(globalCtx);
        self->actor.prevPos = self->targetPos = self->actor.world.pos;
        Collider_InitJntSph(globalCtx, &self->tentCollider);
        Collider_SetJntSph(globalCtx, &self->tentCollider, &self->actor, &sJntSphInit, self->tentElements);
        self->tentMaxAngle = 1.0f;
    }
}

void BossMo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossMo* self = THIS;

    if (self->actor.params >= BOSSMO_TENTACLE) {
        Collider_DestroyJntSph(globalCtx, &self->tentCollider);
    } else {
        Collider_DestroyCylinder(globalCtx, &self->coreCollider);
    }
}

void BossMo_SetupTentacle(BossMo* self, GlobalContext* globalCtx) {
    self->actionFunc = BossMo_Tentacle;
    self->work[MO_TENT_ACTION_STATE] = MO_TENT_WAIT;
    self->timers[0] = 50 + (s16)Rand_ZeroFloat(20.0f);
}

void BossMo_Tentacle(BossMo* self, GlobalContext* globalCtx) {
    // variables marked real are put on the stack and cannot be reordered
    s16 tentXrot;
    s16 sp1B4 = 0; // real
    s32 buttons;
    Player* player = GET_PLAYER(globalCtx);
    s16 indS0;
    s16 indS1;
    Camera* camera1;
    Camera* camera2;
    BossMo* otherTent = (BossMo*)self->otherTent; // real
    f32 maxSwingRateX;                            // real
    f32 maxSwingLagX;                             // real
    f32 maxSwingSizeX;                            // real
    f32 maxSwingRateZ;                            // real
    f32 maxSwingLagZ;                             // real
    f32 maxSwingSizeZ;                            // real
    f32 swingRateAccel;                           // real
    f32 swingSizeAccel;                           // real
    s16 rippleCount;                              // real
    s16 indT5;
    Vec3f ripplePos; // real
    f32 randAngle;
    f32 randFloat;
    f32 tempf1;
    f32 tempf2;
    f32 sin;
    f32 cos;
    f32 temp;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f sp138; // real
    Vec3f sp12C; // real
    Vec3f sp120; // real
    s32 pad11C;
    s32 pad118;
    s32 pad114;
    s32 pad110;
    s32 pad10C;
    s32 pad108;
    Vec3f spFC; // real
    Vec3f spF0; // real
    f32 padEC;
    Vec3f spE0; // real
    Vec3f spD4; // real
    Vec3f spC8; // real

    if (self->work[MO_TENT_ACTION_STATE] <= MO_TENT_DEATH_3) {
        self->actor.world.pos.y = MO_WATER_LEVEL(globalCtx);
    }
    if ((self->work[MO_TENT_ACTION_STATE] == MO_TENT_READY) ||
        (self->work[MO_TENT_ACTION_STATE] >= MO_TENT_DEATH_START) ||
        (self->work[MO_TENT_ACTION_STATE] == MO_TENT_RETREAT) || (self->work[MO_TENT_ACTION_STATE] == MO_TENT_SWING) ||
        (self->work[MO_TENT_ACTION_STATE] == MO_TENT_SHAKE)) {
        if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_READY) {
            if (sMorphaCore->csState != MO_BATTLE) {
                maxSwingRateX = 2000.0f;
                maxSwingLagX = 3000.0f;
                maxSwingSizeX = 1000.0f;
                maxSwingRateZ = 1500.0f;
                maxSwingLagZ = 2500.0f;
                maxSwingSizeZ = 1000.0f;
                swingRateAccel = 10.0f;
                swingSizeAccel = 10.0f;
            } else {
                maxSwingRateX = 2000.0f;
                maxSwingLagX = 3000.0f;
                maxSwingSizeX = 1000.0f;
                maxSwingRateZ = 1500.0f;
                maxSwingLagZ = 2500.0f;
                maxSwingSizeZ = 1000.0f;
                swingRateAccel = 20.0f;
                swingSizeAccel = 30.0f;
            }
        } else if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_SWING) {
            maxSwingRateX = 2500.0f;
            maxSwingLagX = -1000.0f;
            maxSwingSizeX = 3000.0f;
            maxSwingRateZ = 1500.0f;
            maxSwingLagZ = 2500.0f;
            maxSwingSizeZ = 0.0;
            swingRateAccel = 30.0f;
            swingSizeAccel = 60.0f;
            if (((self->sfxTimer % 16) == 0) && (self->timers[0] < 30)) {
                Audio_PlaySoundIncreasinglyTransposed(&self->tentTipPos, NA_SE_EN_MOFER_WAVE, gMorphaTransposeTable);
            }
        } else if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_SHAKE) {
            if (self->timers[0] > 40) {
                maxSwingRateX = 1300.0f;
                maxSwingLagX = -3200.0f;
                maxSwingSizeX = 7000.0f;
                maxSwingRateZ = 800.0f;
                maxSwingLagZ = 2500.0f;
                maxSwingSizeZ = 5000.0f;
                swingRateAccel = 30.0f;
                swingSizeAccel = 60.0f;
                if ((self->sfxTimer % 32) == 0) {
                    Audio_PlaySoundIncreasinglyTransposed(&self->tentTipPos, NA_SE_EN_MOFER_WAVE,
                                                          gMorphaTransposeTable);
                    func_800AA000(0, 100, 5, 2);
                    func_8002F7DC(&player->actor, NA_SE_VO_LI_FREEZE + player->ageProperties->unk_92);
                }
            } else {
                maxSwingRateX = 2000.0f;
                maxSwingLagX = -1000.0f;
                maxSwingSizeX = 5000.0f;
                maxSwingRateZ = 1500.0f;
                maxSwingLagZ = 2500.0f;
                maxSwingSizeZ = 100.0f;
                swingRateAccel = 70.0f;
                swingSizeAccel = 70.0f;
                if ((self->sfxTimer % 16) == 0) {
                    Audio_PlaySoundIncreasinglyTransposed(&self->tentTipPos, NA_SE_EN_MOFER_WAVE,
                                                          gMorphaTransposeTable);
                    func_800AA000(0, 160, 5, 4);
                    func_8002F7DC(&player->actor, NA_SE_VO_LI_FREEZE + player->ageProperties->unk_92);
                }
            }
        } else if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_RETREAT) {
            maxSwingRateX = 1300.0f;
            maxSwingLagX = 3200.0f;
            maxSwingSizeX = 7000.0f;
            maxSwingRateZ = 800.0f;
            maxSwingLagZ = 2500.0f;
            maxSwingSizeZ = 5000.0f;
            swingRateAccel = 30.0f;
            swingSizeAccel = 30.0f;
        } else if (self->work[MO_TENT_ACTION_STATE] >= MO_TENT_DEATH_START) {
            maxSwingRateX = -400.0f;
            maxSwingLagX = -3200.0f;
            maxSwingSizeX = 0.0f;
            maxSwingRateZ = 2300.0f;
            maxSwingLagZ = 3200.0f;
            maxSwingSizeZ = 1000.0;
            swingRateAccel = 30.0f;
            swingSizeAccel = 60.0f;
        }
        Math_ApproachF(&self->fwork[MO_TENT_SWING_RATE_X], maxSwingRateX, 1.0f, swingRateAccel);
        Math_ApproachF(&self->fwork[MO_TENT_SWING_LAG_X], maxSwingLagX, 1.0f, 30.0f);
        Math_ApproachF(&self->fwork[MO_TENT_SWING_SIZE_X], maxSwingSizeX, 1.0f, swingSizeAccel);
        Math_ApproachF(&self->fwork[MO_TENT_SWING_RATE_Z], maxSwingRateZ, 1.0f, swingRateAccel);
        Math_ApproachF(&self->fwork[MO_TENT_SWING_LAG_Z], maxSwingLagZ, 1.0f, 30.0f);
        Math_ApproachF(&self->fwork[MO_TENT_SWING_SIZE_Z], maxSwingSizeZ, 1.0f, swingSizeAccel);
        self->xSwing += (s16)self->fwork[MO_TENT_SWING_RATE_X];
        self->zSwing += (s16)self->fwork[MO_TENT_SWING_RATE_Z];
    }
    switch (self->work[MO_TENT_ACTION_STATE]) {
        case MO_TENT_WAIT:
            self->actor.flags &= ~1;
            if (self == sMorphaTent2) {
                self->work[MO_TENT_ACTION_STATE] = MO_TENT_SPAWN;
                self->timers[0] = 70;
                self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
            }
            break;
        case MO_TENT_SPAWN:
            self->drawActor = true;
            self->baseBubblesTimer = 20;
            if (self->timers[0] < 20) {
                Math_ApproachF(&self->tentRippleSize, 0.15f, 0.5f, 0.01);
                Math_ApproachF(&self->baseAlpha, 150.0f, 1.0f, 5.0f);
                if (self->baseAlpha >= 150.0f) {
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_READY;
                    self->timers[0] = 60;
                }
            }
            if (self->timers[0] > 50) {
                rippleCount = 1;
            } else if (self->timers[0] > 40) {
                rippleCount = 3;
            } else if (self->timers[0] > 30) {
                rippleCount = 5;
            } else if (self->timers[0] > 20) {
                rippleCount = 8;
            } else {
                rippleCount = 3;
            }
            for (indS1 = 0; indS1 < rippleCount; indS1++) {
                randFloat = Rand_ZeroFloat(50.0f);
                randAngle = Rand_ZeroFloat(0x10000);
                ripplePos = self->actor.world.pos;
                ripplePos.x += sinf(randAngle) * randFloat;
                ripplePos.z += cosf(randAngle) * randFloat;
                ripplePos.y = MO_WATER_LEVEL(globalCtx);
                BossMo_SpawnRipple(globalCtx->specialEffects, &ripplePos, 40.0f, 110.0f, 80, 290, MO_FX_SMALL_RIPPLE);
            }
            break;
        case MO_TENT_READY:
        case MO_TENT_SWING:
            if (sMorphaCore->csState == MO_BATTLE) {
                func_80078914(&self->tentTipPos, NA_SE_EN_MOFER_APPEAR - SFX_FLAG);
            }
            Math_ApproachF(&self->waterLevelMod, -5.0f, 0.1f, 0.4f);
            for (indS1 = 0; indS1 < 41; indS1++) {

                sin = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_X] * indS1) + self->xSwing);
                tempf1 = self->fwork[MO_TENT_SWING_SIZE_X] * (indS1 * 0.025f * sin);

                cos = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_Z] * indS1) + self->zSwing);
                tempf2 = self->fwork[MO_TENT_SWING_SIZE_Z] * (indS1 * 0.025f * cos);

                Math_ApproachF(&self->tentStretch[indS1].y, self->fwork[MO_TENT_MAX_STRETCH] * 5.0f, 0.1f, 0.4f);
                if (indS1 == 28) {
                    sp1B4 = self->tentRot[indS1].x;
                }
                Math_ApproachS(&self->tentRot[indS1].x, tempf1, 1.0f / self->tentMaxAngle, self->tentSpeed);
                Math_ApproachS(&self->tentRot[indS1].z, tempf2, 1.0f / self->tentMaxAngle, self->tentSpeed);
            }
            self->targetPos = self->actor.world.pos;
            Math_ApproachF(&self->actor.speedXZ, 0.75f, 1.0f, 0.04f);
            if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_SWING) {
                Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer + self->attackAngleMod, 0xA,
                               0x1F4);
            }
            Math_ApproachF(&self->fwork[MO_TENT_MAX_STRETCH], 1.0f, 0.5f, 0.04);
            if (sMorphaCore->csState != MO_BATTLE) {
                Math_ApproachF(&self->tentMaxAngle, 1.0f, 1.0f, 0.001f);
                Math_ApproachF(&self->tentSpeed, 240.0f, 1.0f, 3.0);
            } else {
                Math_ApproachF(&self->tentMaxAngle, 1.0f, 1.0f, 0.002f);
                Math_ApproachF(&self->tentSpeed, 400.0f, 1.0f, 6.0f);
            }
            if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_READY) {
                if ((self->timers[0] == 0) && !HAS_LINK(otherTent)) {
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_SWING;
                    self->timers[0] = 50;
                    Audio_ResetIncreasingTranspose();
                    self->attackAngleMod = Rand_CenteredFloat(0x1000);
                }
            } else {
                tentXrot = self->tentRot[28].x;
                if ((self->timers[0] == 0) && (tentXrot >= 0) && (sp1B4 < 0)) {
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_ATTACK;
                    if (self == sMorphaTent1) {
                        self->timers[0] = 175;
                    } else {
                        self->timers[0] = 55;
                    }
                }
            }
            break;
        case MO_TENT_ATTACK:
            self->actor.flags |= 0x1000000;
            func_80078914(&self->tentTipPos, NA_SE_EN_MOFER_ATTACK - SFX_FLAG);
            Math_ApproachF(&self->waterLevelMod, -5.0f, 0.1f, 0.4f);
            for (indS1 = 0; indS1 < 41; indS1++) {
                Math_ApproachF(&self->tentStretch[indS1].y,
                               self->fwork[MO_TENT_MAX_STRETCH] * ((((40 - indS1) * 25.0f) / 100.0f) + 5.0f), 0.5f,
                               0.7f);
                Math_ApproachS(&self->tentRot[indS1].x, sAttackRot[indS1] * 0x100, 1.0f / self->tentMaxAngle,
                               self->tentSpeed);
                Math_ApproachS(&self->tentRot[indS1].z, 0, 1.0f / self->tentMaxAngle, self->tentSpeed);
            }
            self->targetPos = self->actor.world.pos;
            Math_ApproachF(&self->tentMaxAngle, 0.5f, 1.0f, 0.01);
            Math_ApproachF(&self->tentSpeed, 160.0f, 1.0f, 50.0f);
            if ((self->timers[0] == 0) || (self->linkHitTimer != 0)) {
                dx = self->tentPos[22].x - player->actor.world.pos.x;
                dy = self->tentPos[22].y - player->actor.world.pos.y;
                dz = self->tentPos[22].z - player->actor.world.pos.z;
                if ((fabsf(dy) < 50.0f) && !HAS_LINK(otherTent) && (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 120.0f)) {
                    self->tentMaxAngle = .001f;
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_CURL;
                    self->timers[0] = 40;
                    self->tentSpeed = 0;
                    if ((s16)(self->actor.shape.rot.y - self->actor.yawTowardsPlayer) >= 0) {
                        self->linkToLeft = false;
                    } else {
                        self->linkToLeft = true;
                    }
                } else {
                    self->tentMaxAngle = .001f;
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_READY;
                    self->tentSpeed = 0;
                    self->fwork[MO_TENT_SWING_RATE_X] = 0;
                    self->fwork[MO_TENT_SWING_RATE_Z] = 0;
                    self->fwork[MO_TENT_SWING_SIZE_X] = 0;
                    self->fwork[MO_TENT_SWING_SIZE_Z] = 0;
                    self->timers[0] = 30;
                    if ((fabsf(player->actor.world.pos.x - self->actor.world.pos.x) > 300.0f) ||
                        (player->actor.world.pos.y < MO_WATER_LEVEL(globalCtx)) || HAS_LINK(otherTent) ||
                        (fabsf(player->actor.world.pos.z - self->actor.world.pos.z) > 300.0f)) {

                        self->work[MO_TENT_ACTION_STATE] = MO_TENT_RETREAT;
                        self->timers[0] = 75;
                    }
                }
            }
            break;
        case MO_TENT_CURL:
        case MO_TENT_GRAB:
            Math_ApproachF(&self->waterLevelMod, -5.0f, 0.1f, 0.4f);
            if (self->timers[0] == 125) {
                self->tentMaxAngle = .001f;
                self->tentSpeed = 0;
            }
            for (indS1 = 0; indS1 < 41; indS1++) {
                if (self->timers[0] > 25) {
                    if (!self->linkToLeft) {
                        Math_ApproachS(&self->tentRot[indS1].z, sCurlRot[indS1] * 0x100, 1.0f / self->tentMaxAngle,
                                       self->tentSpeed);
                    } else {
                        Math_ApproachS(&self->tentRot[indS1].z, sCurlRot[indS1] * -0x100, 1.0f / self->tentMaxAngle,
                                       self->tentSpeed);
                    }
                } else {
                    if (!self->linkToLeft) {
                        Math_ApproachS(&self->tentRot[indS1].z, sGrabRot[indS1] * 0x100, 1.0f / self->tentMaxAngle,
                                       self->tentSpeed);
                    } else {
                        Math_ApproachS(&self->tentRot[indS1].z, sGrabRot[indS1] * -0x100, 1.0f / self->tentMaxAngle,
                                       self->tentSpeed);
                    }
                }
            }
            Math_ApproachF(&self->tentMaxAngle, 0.1f, 1.0f, 0.01f);
            Math_ApproachF(&self->tentSpeed, 960.0f, 1.0f, 30.0f);
            if (self->timers[0] >= 30) {
                Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, 0xC8);
            }
            if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_CURL) {
                if ((self->timers[0] >= 5) && (self->linkHitTimer != 0) && (player->actor.parent == NULL)) {
                    if (globalCtx->grabPlayer(globalCtx, player)) {
                        player->actor.parent = &self->actor;
                        self->work[MO_TENT_ACTION_STATE] = MO_TENT_GRAB;
                        func_80078914(&self->tentTipPos, NA_SE_EN_MOFER_CATCH);
                        Audio_PlaySoundGeneral(NA_SE_VO_LI_DAMAGE_S, &player->actor.projectedPos, 4, &D_801333E0,
                                               &D_801333E0, &D_801333E8);
                    } else {
                        self->work[MO_TENT_ACTION_STATE] = MO_TENT_READY;
                        self->tentMaxAngle = .001f;
                        self->tentSpeed = 0;
                        self->fwork[MO_TENT_SWING_SIZE_Z] = 0;
                        self->fwork[MO_TENT_SWING_SIZE_X] = 0;
                        self->fwork[MO_TENT_SWING_RATE_Z] = 0;
                        self->fwork[MO_TENT_SWING_RATE_X] = 0;
                        self->timers[0] = 30;
                    }
                }
                if (self->timers[0] == 4) {
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_READY;
                    self->tentMaxAngle = .001f;
                    self->tentSpeed = 0;
                    self->fwork[MO_TENT_SWING_SIZE_Z] = 0;
                    self->fwork[MO_TENT_SWING_SIZE_X] = 0;
                    self->fwork[MO_TENT_SWING_RATE_Z] = 0;
                    self->fwork[MO_TENT_SWING_RATE_X] = 0;
                    self->timers[0] = 30;
                }
            }
            if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_GRAB) {
                player->unk_850 = 0xA;
                player->actor.speedXZ = player->actor.velocity.y = 0;
                Math_ApproachF(&player->actor.world.pos.x, self->grabPosRot.pos.x, 0.5f, 20.0f);
                Math_ApproachF(&player->actor.world.pos.y, self->grabPosRot.pos.y, 0.5f, 20.0f);
                Math_ApproachF(&player->actor.world.pos.z, self->grabPosRot.pos.z, 0.5f, 20.0f);
                Math_ApproachS(&player->actor.shape.rot.x, self->grabPosRot.rot.x, 2, 0x7D0);
                Math_ApproachS(&player->actor.shape.rot.y, self->grabPosRot.rot.y, 2, 0x7D0);
                Math_ApproachS(&player->actor.shape.rot.z, self->grabPosRot.rot.z, 2, 0x7D0);
                if (self->timers[0] == 0) {
                    camera1 = Gameplay_GetCamera(globalCtx, MAIN_CAM);
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_SHAKE;
                    self->tentMaxAngle = .001f;
                    self->fwork[MO_TENT_SWING_RATE_X] = self->fwork[MO_TENT_SWING_RATE_Z] =
                        self->fwork[MO_TENT_SWING_SIZE_X] = self->fwork[MO_TENT_SWING_SIZE_Z] = self->tentSpeed = 0;
                    self->timers[0] = 150;
                    self->mashCounter = 0;
                    self->sfxTimer = 30;
                    Audio_ResetIncreasingTranspose();
                    func_80064520(globalCtx, &globalCtx->csCtx);
                    self->csCamera = Gameplay_CreateSubCamera(globalCtx);
                    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
                    Gameplay_ChangeCameraStatus(globalCtx, self->csCamera, CAM_STAT_ACTIVE);
                    self->cameraEye = camera1->eye;
                    self->cameraAt = camera1->at;
                    self->cameraYaw = Math_FAtan2F(self->cameraEye.x - self->actor.world.pos.x,
                                                   self->cameraEye.z - self->actor.world.pos.z);
                    self->cameraYawRate = 0;
                    goto tent_shake;
                }
            }
            break;
        tent_shake:
        case MO_TENT_SHAKE:
            if (self->timers[0] == 138) {
                ShrinkWindow_SetVal(0);
                Interface_ChangeAlpha(0xB);
            }
            if ((self->timers[0] % 8) == 0) {
                globalCtx->damagePlayer(globalCtx, -1);
            }
            Math_ApproachF(&self->waterLevelMod, -5.0f, 0.1f, 0.4f);
            sp1B4 = self->tentRot[15].x;
            buttons = globalCtx->state.input[0].press.button;
            if (CHECK_BTN_ALL(buttons, BTN_A) || CHECK_BTN_ALL(buttons, BTN_B)) {
                self->mashCounter++;
            }
            for (indS1 = 0; indS1 < 41; indS1++) {
                if (indS1 < 20) {
                    sin = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_X] * indS1) + self->xSwing);
                    tempf1 = self->fwork[MO_TENT_SWING_SIZE_X] * (indS1 * 0.025f * sin);
                    cos = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_Z] * indS1) + self->zSwing);
                    tempf2 = self->fwork[MO_TENT_SWING_SIZE_Z] * (indS1 * 0.025f * cos);
                    temp = ((((40 - indS1) * 25.0f) / 100.0f) + 5.0f);
                    Math_ApproachF(&self->tentStretch[indS1].y, self->fwork[MO_TENT_MAX_STRETCH] * temp, 0.1f, 0.1f);
                    Math_ApproachS(&self->tentRot[indS1].x, tempf1, 1.0f / self->tentMaxAngle, self->tentSpeed);
                    Math_ApproachS(&self->tentRot[indS1].z, tempf2, 1.0f / self->tentMaxAngle, self->tentSpeed);
                }
            }
            player->unk_850 = 0xA;
            player->actor.world.pos.x = self->grabPosRot.pos.x;
            player->actor.world.pos.y = self->grabPosRot.pos.y;
            player->actor.world.pos.z = self->grabPosRot.pos.z;
            player->actor.world.rot.x = player->actor.shape.rot.x = self->grabPosRot.rot.x;
            player->actor.world.rot.y = player->actor.shape.rot.y = self->grabPosRot.rot.y;
            player->actor.world.rot.z = player->actor.shape.rot.z = self->grabPosRot.rot.z;
            player->actor.velocity.y = 0;
            player->actor.speedXZ = 0;
            Math_ApproachF(&self->fwork[MO_TENT_MAX_STRETCH], 1.0f, 0.5f, 0.01);
            Math_ApproachF(&self->tentMaxAngle, 0.5f, 1.0f, 0.005f);
            Math_ApproachF(&self->tentSpeed, 480.0f, 1.0f, 10.0f);
            Math_ApproachF(&self->tentPulse, 0.3f, 0.5f, 0.03f);
            if ((self->mashCounter >= 40) || (self->timers[0] == 0)) {
                tentXrot = self->tentRot[15].x;
                if ((tentXrot < 0) && (sp1B4 >= 0)) {
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_RETREAT;
                    self->work[MO_TENT_INVINC_TIMER] = 50;
                    if (&self->actor == player->actor.parent) {
                        player->unk_850 = 0x65;
                        player->actor.parent = NULL;
                        player->csMode = 0;
                        if (self->timers[0] == 0) {
                            func_8002F6D4(globalCtx, &self->actor, 20.0f, self->actor.shape.rot.y + 0x8000, 10.0f, 0);
                        }
                    }
                    self->timers[0] = 75;
                }
            }
            if (self->csCamera != 0) {
                sp138.x = 0;
                sp138.y = 100.0f;
                sp138.z = 200.0f;
                self->cameraYaw -= self->cameraYawRate;
                Math_ApproachF(&self->cameraYawRate, 0.01, 1.0f, 0.002f);
                Matrix_RotateY(self->cameraYaw, MTXMODE_NEW);
                Matrix_MultVec3f(&sp138, &sp12C);
                Math_ApproachF(&self->cameraEye.x, self->actor.world.pos.x + sp12C.x, 0.1f, 10.0f);
                Math_ApproachF(&self->cameraEye.y, self->actor.world.pos.y + sp12C.y, 0.1f, 10.0f);
                Math_ApproachF(&self->cameraEye.z, self->actor.world.pos.z + sp12C.z, 0.1f, 10.0f);
                Math_ApproachF(&self->cameraAt.x, player->actor.world.pos.x, 0.5f, 50.0f);
                Math_ApproachF(&self->cameraAt.y, player->actor.world.pos.y, 0.5f, 50.0f);
                Math_ApproachF(&self->cameraAt.z, player->actor.world.pos.z, 0.5f, 50.0f);
                Gameplay_CameraSetAtEye(globalCtx, self->csCamera, &self->cameraAt, &self->cameraEye);
            }
            break;
        case MO_TENT_CUT:
            func_80078914(&self->tentTipPos, NA_SE_EV_WATER_WALL - SFX_FLAG);
            if (&self->actor == player->actor.parent) {
                player->unk_850 = 0x65;
                player->actor.parent = NULL;
                player->csMode = 0;
            }
            Math_ApproachF(&self->tentRippleSize, 0.15f, 0.5f, 0.01);
            if (self->meltIndex < 41) {
                for (indS0 = 0; indS0 < 10; indS0++) {
                    sp120 = self->tentPos[self->meltIndex];
                    sp120.x += Rand_CenteredFloat(30.0f);
                    sp120.y += Rand_CenteredFloat(30.0f);
                    sp120.z += Rand_CenteredFloat(30.0f);
                    BossMo_SpawnStillDroplet(globalCtx->specialEffects, &sp120, Rand_ZeroFloat(0.1f) + .2f);
                }
                self->meltIndex++;
            }
            Math_ApproachF(&self->cutScale, 0.0, 1.0f, 0.2f);
            if ((self->meltIndex >= 41) || (self->timers[0] == 0)) {
                self->work[MO_TENT_ACTION_STATE] = MO_TENT_RETREAT;
                self->timers[0] = 75;
                self->tentMaxAngle = 0.005f;
                self->tentSpeed = 50.0f;
                self->fwork[MO_TENT_SWING_SIZE_X] = 7000.0f;
                self->fwork[MO_TENT_SWING_SIZE_Z] = 5000.0f;
            }
            break;
        case MO_TENT_RETREAT:
            if (self->csCamera != 0) {
                Math_ApproachF(&self->cameraAt.x, player->actor.world.pos.x, 0.5f, 50.0f);
                Math_ApproachF(&self->cameraAt.y, player->actor.world.pos.y, 0.5f, 50.0f);
                Math_ApproachF(&self->cameraAt.z, player->actor.world.pos.z, 0.5f, 50.0f);
                Gameplay_CameraSetAtEye(globalCtx, self->csCamera, &self->cameraAt, &self->cameraEye);
                if (player->actor.world.pos.y <= 42.0f) {
                    camera2 = Gameplay_GetCamera(globalCtx, MAIN_CAM);
                    camera2->eye = self->cameraEye;
                    camera2->eyeNext = self->cameraEye;
                    camera2->at = self->cameraAt;
                    func_800C08AC(globalCtx, self->csCamera, 0);
                    self->csCamera = 0;
                    func_80064534(globalCtx, &globalCtx->csCtx);
                }
            }
            for (indS1 = 0; indS1 < 41; indS1++) {
                sin = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_X] * indS1) + self->xSwing);
                tempf1 = (indS1 * 0.025f * sin * self->fwork[MO_TENT_SWING_SIZE_X]) * self->fwork[MO_TENT_MAX_STRETCH];
                cos = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_Z] * indS1) + self->zSwing);
                tempf2 = (indS1 * 0.025f * cos * self->fwork[MO_TENT_SWING_SIZE_Z]) * self->fwork[MO_TENT_MAX_STRETCH];
                Math_ApproachF(&self->tentStretch[indS1].y, self->fwork[MO_TENT_MAX_STRETCH] * 5.0f, 0.5f, 0.2f);
                Math_ApproachS(&self->tentRot[indS1].x, tempf1, 1.0f / self->tentMaxAngle, self->tentSpeed);
                Math_ApproachS(&self->tentRot[indS1].z, tempf2, 1.0f / self->tentMaxAngle, self->tentSpeed);
            }
            Math_ApproachF(&self->fwork[MO_TENT_MAX_STRETCH], 0, 0.5f, 0.02f);
            Math_ApproachF(&self->tentMaxAngle, 0.5f, 1.0f, 0.01);
            Math_ApproachF(&self->tentSpeed, 320.0f, 1.0f, 50.0f);
            if (self->timers[0] == 0) {
                self->actor.flags &= ~1;
                Math_ApproachF(&self->baseAlpha, 0.0, 1.0f, 5.0f);
                for (indS1 = 0; indS1 < 40; indS1++) {
                    if (sMorphaTent2->tentSpawnPos) {}
                    indT5 = Rand_ZeroFloat(20.9f);
                    indS0 = sTentSpawnIndex[indT5];
                    spFC.x = 0;
                    spFC.y = 0;
                    spFC.z = 0;
                    Matrix_RotateY((player->actor.world.rot.y / (f32)0x8000) * M_PI, MTXMODE_NEW);
                    Matrix_MultVec3f(&spFC, &spF0);
                    spF0.x = player->actor.world.pos.x + spF0.x;
                    spF0.z = player->actor.world.pos.z + spF0.z;
                    if ((fabsf(spF0.x - sTentSpawnPos[indS0].x) <= 320) &&
                        (fabsf(spF0.z - sTentSpawnPos[indS0].y) <= 320) &&
                        ((sMorphaTent2 == NULL) || (sMorphaTent2->tentSpawnPos != indS0))) {
                        self->targetPos.x = sTentSpawnPos[indS0].x;
                        self->targetPos.z = sTentSpawnPos[indS0].y;
                        self->tentSpawnPos = indS0;
                        self->timers[0] = (s16)Rand_ZeroFloat(20.0f) + 30;
                        self->work[MO_TENT_ACTION_STATE] = MO_TENT_DESPAWN;
                        break;
                    }
                }
            }
            if ((self == sMorphaTent1) && (sMorphaCore->hitCount >= 3) && (sMorphaTent2 == NULL)) {
                sMorphaTent2 =
                    (BossMo*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_BOSS_MO, self->actor.world.pos.x,
                                         self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, BOSSMO_TENTACLE);

                sMorphaTent2->tentSpawnPos = self->tentSpawnPos;
                if (sMorphaTent2->tentSpawnPos > 10) {
                    sMorphaTent2->tentSpawnPos--;
                } else {
                    sMorphaTent2->tentSpawnPos++;
                }
                sMorphaTent2->targetPos.x = sTentSpawnPos[sMorphaTent2->tentSpawnPos].x;
                sMorphaTent2->targetPos.z = sTentSpawnPos[sMorphaTent2->tentSpawnPos].y;
                sMorphaTent2->timers[0] = 100;
                sMorphaTent2->work[MO_TENT_ACTION_STATE] = MO_TENT_DESPAWN;
                sMorphaTent2->otherTent = &sMorphaTent1->actor;
                sMorphaTent1->otherTent = &sMorphaTent2->actor;
            }
            break;
        case MO_TENT_DESPAWN:
            self->actor.flags &= ~1;
            Math_ApproachF(&self->baseAlpha, 0, 1.0f, 5.0f);
            if ((self->baseAlpha <= 0.5f) && (self->timers[0] == 0)) {
                self->meltIndex = 0;
                self->actor.world.pos.x = self->targetPos.x;
                self->actor.world.pos.z = self->targetPos.z;
                self->actor.prevPos = self->actor.world.pos;
                self->cutScale = 1.0f;
                self->cutIndex = self->meltIndex;
                self->work[MO_TENT_ACTION_STATE] = MO_TENT_WAIT;
                self->timers[0] = (s16)Rand_ZeroFloat(20.0f) + 10;

                self->tentSpeed = 0;
                self->fwork[MO_TENT_SWING_RATE_X] = 0;
                self->fwork[MO_TENT_SWING_RATE_Z] = 0;
                self->fwork[MO_TENT_SWING_SIZE_X] = 0;
                self->fwork[MO_TENT_SWING_SIZE_Z] = 0;

                self->tentMaxAngle = .001f;
            }
            break;
        case MO_TENT_DEATH_START:
            self->actor.shape.rot.y = 0x4000;
            break;
        case MO_TENT_DEATH_3:
            self->baseBubblesTimer = 20;
            Math_ApproachF(&sMorphaCore->waterLevel, -300.0f, 0.1f, 0.8f);
            self->actor.flags &= ~1;
            for (indS1 = 0; indS1 < 41; indS1++) {
                sin = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_X] * indS1) + self->xSwing);
                tempf1 = self->fwork[MO_TENT_SWING_SIZE_X] * (indS1 * 0.025f * sin);
                cos = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_Z] * indS1) + self->zSwing);
                tempf2 = self->fwork[MO_TENT_SWING_SIZE_Z] * (indS1 * 0.025f * cos);
                Math_ApproachF(&self->tentStretch[indS1].y, self->fwork[MO_TENT_MAX_STRETCH] * 5.0f, 0.1f, 0.4f);
                Math_ApproachS(&self->tentRot[indS1].x, tempf1, 1.0f / self->tentMaxAngle, self->tentSpeed);
                Math_ApproachS(&self->tentRot[indS1].z, tempf2, 1.0f / self->tentMaxAngle, self->tentSpeed);
            }
            self->actor.speedXZ = 0.0;
            Math_ApproachF(&self->fwork[MO_TENT_MAX_STRETCH], 4.3f, 0.5f, 0.04);
            Math_ApproachF(&self->tentPulse, 1.3f, 0.5f, 0.05f);
            break;
        case MO_TENT_DEATH_1:
            self->baseBubblesTimer = 20;
            self->actor.shape.rot.y = 0x4000;
            self->actor.shape.rot.x = -0x8000;
            self->actor.world.pos.y = sMorphaCore->waterLevel + 650.0f;
            Math_ApproachF(&sMorphaCore->waterLevel, -300.0f, 0.1f, 1.3f);
            for (indS1 = 0; indS1 < 41; indS1++) {
                sin = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_X] * indS1) + self->xSwing);
                tempf1 = self->fwork[MO_TENT_SWING_SIZE_X] * (indS1 * 0.025f * sin);
                cos = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_Z] * indS1) + self->zSwing);
                tempf2 = self->fwork[MO_TENT_SWING_SIZE_Z] * (indS1 * 0.025f * cos);
                Math_ApproachF(&self->tentStretch[indS1].y, self->fwork[MO_TENT_MAX_STRETCH] * 5.0f, 0.1f, 0.4f);
                Math_ApproachS(&self->tentRot[indS1].x, tempf1, 1.0f / self->tentMaxAngle, self->tentSpeed);
                Math_ApproachS(&self->tentRot[indS1].z, tempf2, 1.0f / self->tentMaxAngle, self->tentSpeed);
            }
            self->actor.speedXZ = 0.0;
            Math_ApproachF(&self->tentPulse, 1.3f, 0.5f, 0.05f);
            break;
        case MO_TENT_DEATH_2:
            self->baseBubblesTimer = 20;
            Math_ApproachF(&sMorphaCore->waterLevel, -295.0f, 0.1f, 1.3f);
            self->actor.world.pos.y = sMorphaCore->waterLevel + 650.0f;
            for (indS1 = 0; indS1 < 41; indS1++) {
                sin = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_X] * indS1) + self->xSwing);
                tempf1 = self->fwork[MO_TENT_SWING_SIZE_X] * (indS1 * 0.025f * sin);
                cos = Math_SinS(((s16)self->fwork[MO_TENT_SWING_LAG_Z] * indS1) + self->zSwing);
                tempf2 = self->fwork[MO_TENT_SWING_SIZE_Z] * (indS1 * 0.025f * cos);
                Math_ApproachF(&self->tentStretch[indS1].y, self->fwork[MO_TENT_MAX_STRETCH] * 5.0f, 0.1f, 0.4f);
                Math_ApproachS(&self->tentRot[indS1].x, tempf1, 1.0f / self->tentMaxAngle, self->tentSpeed);
                Math_ApproachS(&self->tentRot[indS1].z, tempf2, 1.0f / self->tentMaxAngle, self->tentSpeed);
            }
            self->actor.speedXZ = 0.0;
            self->noBubbles--;
            Math_ApproachF(&self->fwork[MO_TENT_MAX_STRETCH], 0.1f, 0.1f, 0.03);
            Math_ApproachF(&self->tentPulse, 0.02f, 0.5f, 0.015f);
            if ((self->timers[0] > 0) && (self->timers[0] < 40)) {
                Math_ApproachF(&self->actor.scale.x, 0.035f, 0.05f, self->flattenRate);
                if (self->timers[0] == 1) {
                    self->flattenRate = 0.0;
                }
            } else if (self->timers[0] == 0) {
                Math_ApproachF(&self->actor.scale.x, .001f, 0.05f, self->flattenRate);
            }
            Math_ApproachF(&self->flattenRate, 0.00045f, 0.1f, 0.00001f);
            break;
        case MO_TENT_DEATH_5:
            for (indS1 = 0; indS1 < 41; indS1++) {
                if (self->timers[0] != 0) {
                    Math_ApproachF(&self->tentStretch[indS1].y, self->fwork[MO_TENT_MAX_STRETCH] * 5.0f, 0.05f,
                                   self->tentSpeed);
                } else {
                    Math_ApproachF(&self->tentStretch[indS1].y, self->fwork[MO_TENT_MAX_STRETCH] * 5.0f, 0.3f, 100.0f);
                }
                self->tentRot[indS1].x = self->tentRot[indS1].z = 0;
            }
            self->tentPulse = 0.0;
            if (self->timers[0] != 0) {
                self->actor.world.pos.y = sMorphaCore->waterLevel + 650.0f;
                self->fwork[MO_TENT_MAX_STRETCH] = 0.5f;
                Math_ApproachF(&self->actor.scale.x, 0.0015f, 0.05f, self->tentMaxAngle);
                Math_ApproachF(&self->tentMaxAngle, 0.00035f, 1.0f, 0.0000175f);
                Math_ApproachF(&self->tentSpeed, 0.1f, 1.0f, 0.005f);
                self->actor.velocity.y = 0.0;
            } else {
                self->fwork[MO_TENT_MAX_STRETCH] = 0.2f;
                self->fwork[MO_TENT_MAX_STRETCH] += Math_SinS(self->work[MO_TENT_MOVE_TIMER] * 0x2000) * 0.05f;
                padEC = Math_CosS(self->work[MO_TENT_MOVE_TIMER] * 0x2000) * 0.0005f;
                Math_ApproachF(&self->actor.scale.x, 0.002f + padEC, 0.5f, 0.0005f);
                self->actor.world.pos.y += self->actor.velocity.y;
                self->actor.velocity.y -= 1.0f;
                if (self->actor.world.pos.y < -250.0f) {
                    self->actor.world.pos.y = -250.0f;
                    self->actor.velocity.y = 0.0;
                    self->drawActor = false;
                    self->work[MO_TENT_ACTION_STATE] = MO_TENT_DEATH_6;
                    self->timers[0] = 60;
                    func_80078914(&self->tentTipPos, NA_SE_EN_MOFER_CORE_JUMP);
                    for (indS1 = 0; indS1 < 300; indS1++) {
                        spC8.x = 0.0;
                        spC8.y = 0.0;
                        spC8.z = indS1 * 0.03f;
                        Matrix_RotateY(indS1 * 0.23f, MTXMODE_NEW);
                        Matrix_MultVec3f(&spC8, &spE0);
                        spE0.y = Rand_ZeroFloat(7.0f) + 4.0f;
                        spD4 = self->actor.world.pos;
                        spD4.x += spE0.x * 3.0f;
                        spD4.y += (spE0.y * 3.0f) - 30.0f;
                        if (spD4.y < -280.0f) {
                            spD4.y = -280.0f;
                        }
                        spD4.z += spE0.z * 3.0f;
                        BossMo_SpawnDroplet(MO_FX_DROPLET, (BossMoEffect*)globalCtx->specialEffects, &spD4, &spE0,
                                            ((300 - indS1) * .0015f) + 0.13f);
                    }
                    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1,
                                       self->actor.world.pos.x, -280.0f, self->actor.world.pos.z, 0, 0, 0,
                                       WARP_DUNGEON_ADULT);
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, self->actor.world.pos.x + 200.0f,
                                -280.0f, self->actor.world.pos.z, 0, 0, 0, 0);
                    Audio_QueueSeqCmd(0x21);
                    Flags_SetClear(globalCtx, globalCtx->roomCtx.curRoom.num);
                }
            }
            break;
        case MO_TENT_DEATH_6:
            break;
    }
    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;
    if (((self->work[MO_TENT_ACTION_STATE] == MO_TENT_ATTACK) ||
         (self->work[MO_TENT_ACTION_STATE] == MO_TENT_DEATH_2) || (self->work[MO_TENT_ACTION_STATE] == MO_TENT_CURL) ||
         (self->work[MO_TENT_ACTION_STATE] == MO_TENT_GRAB)) &&
        (Rand_ZeroOne() < 0.8f) && (self->actor.scale.x > 0.001f)) {
        Vec3f pos;                             // real
        Vec3f velocity = { 0.0f, 0.0f, 0.0f }; // real
        f32 scale;
        f32 temp;

        if (self->work[MO_TENT_ACTION_STATE] >= MO_TENT_DEATH_2) {
            indS1 = 38;
            scale = Rand_ZeroFloat(0.1f) + 0.1f;
            pos.y = self->tentPos[indS1].y;
        } else {
            indS1 = (s16)Rand_ZeroFloat(20.0f) + 18;
            scale = Rand_ZeroFloat(0.02f) + .05f;
            pos.y = self->tentPos[indS1].y - 10.0f;
        }
        temp = (self->actor.scale.x * 100.0f) * 20.0f;
        pos.x = self->tentPos[indS1].x + Rand_CenteredFloat(temp);
        pos.z = self->tentPos[indS1].z + Rand_CenteredFloat(temp);
        BossMo_SpawnDroplet(MO_FX_DROPLET, (BossMoEffect*)globalCtx->specialEffects, &pos, &velocity, scale);
    }
}

void BossMo_TentCollisionCheck(BossMo* self, GlobalContext* globalCtx) {
    s16 i1;

    for (i1 = 0; i1 < ARRAY_COUNT(self->tentElements); i1++) {
        if (self->tentCollider.elements[i1].info.bumperFlags & BUMP_HIT) {
            s16 i2;
            ColliderInfo* hurtbox;

            for (i2 = 0; i2 < 19; i2++) {
                self->tentCollider.elements[i2].info.bumperFlags &= ~BUMP_HIT;
                self->tentCollider.elements[i2].info.toucherFlags &= ~TOUCH_HIT;
            }
            hurtbox = self->tentCollider.elements[i1].info.acHitInfo;
            self->work[MO_TENT_INVINC_TIMER] = 5;
            if (hurtbox->toucher.dmgFlags & 0x00020000) {
                func_80078914(&self->tentTipPos, NA_SE_EN_MOFER_CUT);
                self->cutIndex = 15;
                self->meltIndex = self->cutIndex + 1;
                self->work[MO_TENT_ACTION_STATE] = MO_TENT_CUT;
                self->timers[0] = 40;
                self->cutScale = 1.0f;
            } else if (hurtbox->toucher.dmgFlags & 0x0D800600) {
                self->linkHitTimer = 5;
            }
            self->tentRippleSize = 0.2f;
            for (i2 = 0; i2 < 10; i2++) {
                Vec3f pos;
                Vec3f velocity;

                velocity.x = Rand_CenteredFloat(8.0f);
                velocity.y = Rand_ZeroFloat(7.0f) + 4.0f;
                velocity.z = Rand_CenteredFloat(8.0f);
                pos = self->tentPos[2 * i1];
                pos.x += velocity.x * 3.0f;
                pos.z += velocity.z * 3.0f;
                BossMo_SpawnDroplet(MO_FX_DROPLET, (BossMoEffect*)globalCtx->specialEffects, &pos, &velocity,
                                    Rand_ZeroFloat(0.08f) + 0.13f);
            }
            break;
        } else if (self->tentCollider.elements[i1].info.toucherFlags & TOUCH_HIT) {
            self->tentCollider.elements[i1].info.toucherFlags &= ~TOUCH_HIT;
            self->linkHitTimer = 5;
            break;
        }
    }
}

void BossMo_IntroCs(BossMo* self, GlobalContext* globalCtx) {
    static Vec3f cutsceneTargets[6] = {
        { -360.0f, -190.0f, 0.0f },  { 250.0f, -190.0f, 0.0f }, { 300.0f, -120.0f, -278.0f },
        { 180.0f, -80.0f, -340.0f }, { 180.0f, 0.0f, -340.0f }, { 180.0f, 60.0f, -230.0f },
    };
    u8 sp9F = 0;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 tempX;
    f32 tempY;
    s32 pad84;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    Player* player = GET_PLAYER(globalCtx);
    Camera* camera = Gameplay_GetCamera(globalCtx, MAIN_CAM);
    Vec3f bubblePos;
    Vec3f bubblePos2;
    Camera* camera2;
    f32 pad50;
    f32 pad4C;
    f32 pad48;

    if (self->csState < MO_INTRO_REVEAL) {
        self->cameraZoom = 80.0f;
    }
    switch (self->csState) {
        case MO_INTRO_WAIT:
            if (self->timers[0] == 1) {
                func_8010B680(globalCtx, 0x403F, NULL);
            }
            if (((fabsf(player->actor.world.pos.z - 180.0f) < 40.0f) &&
                 (fabsf(player->actor.world.pos.x - 180.0f) < 40.0f)) ||
                ((fabsf(player->actor.world.pos.z - -180.0f) < 40.0f) &&
                 (fabsf(player->actor.world.pos.x - 180.0f) < 40.0f)) ||
                ((fabsf(player->actor.world.pos.z - 180.0f) < 40.0f) &&
                 (fabsf(player->actor.world.pos.x - -180.0f) < 40.0f)) ||
                ((fabsf(player->actor.world.pos.z - -180.0f) < 40.0f) &&
                 (fabsf(player->actor.world.pos.x - -180.0f) < 40.0f))) {
                // checks if Link is on one of the four platforms
                func_80064520(globalCtx, &globalCtx->csCtx);
                func_8002DF54(globalCtx, &self->actor, 8);
                self->csCamera = Gameplay_CreateSubCamera(globalCtx);
                Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
                Gameplay_ChangeCameraStatus(globalCtx, self->csCamera, CAM_STAT_ACTIVE);
                self->actor.speedXZ = 0.0f;
                self->csState = MO_INTRO_START;
                self->timers[2] = 50;
                self->work[MO_TENT_VAR_TIMER] = self->work[MO_TENT_MOVE_TIMER] = 0;
                self->actor.world.rot.y = 0x721A;
                sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_READY;
                sMorphaTent1->timers[0] = 30000;
                Audio_QueueSeqCmd(0x103200FF);
                func_80106CCC(globalCtx);
            } else {
                break;
            }
        case MO_INTRO_START:
            player->actor.world.pos.x = 180.0f;
            player->actor.world.pos.z = -130.0f;
            player->actor.shape.rot.y = player->actor.world.rot.y = 0;
            player->actor.speedXZ = 0.0f;
            self->cameraEye.x = -424.0f;
            self->cameraEye.y = -190.0f;
            self->cameraEye.z = 180.0f;
            self->cameraAt.x = player->actor.world.pos.x;
            self->cameraAt.y = -330.0f;
            self->cameraAt.z = 0.0f;
            if (self->timers[2] == 0) {
                self->csState = MO_INTRO_SWIM;
                self->work[MO_TENT_MOVE_TIMER] = 0;
            } else if (self->timers[2] < 50) {
                bubblePos.x = (self->cameraEye.x + 20.0f) + 10.0f;
                bubblePos.y = -250.0f;
                bubblePos.z = self->cameraEye.z;
                EffectSsBubble_Spawn(globalCtx, &bubblePos, 0.0f, 10.0f, 50.0f, Rand_ZeroFloat(0.05f) + 0.13f);
            }
            if (self->timers[2] == 40) {
                func_80078914(&sAudioZeroVec, NA_SE_EN_MOFER_BUBLE_DEMO);
            }
            break;
        case MO_INTRO_SWIM:
            Math_ApproachF(&self->cameraYawShake, 0.1f, 1.0f, 0.002f);
            self->targetPos = cutsceneTargets[self->targetIndex];
            if (self->targetIndex == 5) {
                tempY = Math_SinS(self->work[MO_TENT_MOVE_TIMER] * 0x500) * 20.0f;
            } else {
                tempY = Math_SinS(self->work[MO_TENT_MOVE_TIMER] * 0x500) * 5.0f;
            }
            dx = self->targetPos.x - self->cameraEye.x;
            dy = self->targetPos.y - self->cameraEye.y + tempY;
            dz = self->targetPos.z - self->cameraEye.z;
            tempY = Math_FAtan2F(dx, dz);
            tempX = Math_FAtan2F(dy, sqrtf(SQ(dx) + SQ(dz)));
            Math_ApproachS(&self->actor.world.rot.y, tempY * (0x8000 / M_PI), 5, self->cameraYawRate);
            Math_ApproachS(&self->actor.world.rot.x, tempX * (0x8000 / M_PI), 5, self->cameraYawRate);
            if (self->work[MO_TENT_MOVE_TIMER] == 150) {
                self->cameraAtVel.x = fabsf(self->cameraAt.x - player->actor.world.pos.x);
                self->cameraAtVel.y = fabsf(self->cameraAt.y - player->actor.world.pos.y);
                self->cameraAtVel.z = fabsf(self->cameraAt.z - player->actor.world.pos.z);
            }
            if (self->work[MO_TENT_MOVE_TIMER] >= 150) {
                Math_ApproachF(&self->cameraAt.x, player->actor.world.pos.x, 0.1f,
                               self->cameraAtVel.x * self->cameraSpeedMod);
                Math_ApproachF(&self->cameraAt.y, player->actor.world.pos.y + 50.0f, 0.1f,
                               self->cameraAtVel.y * self->cameraSpeedMod);
                Math_ApproachF(&self->cameraAt.z, player->actor.world.pos.z, 0.1f,
                               self->cameraAtVel.z * self->cameraSpeedMod);
                Math_ApproachF(&self->cameraSpeedMod, 0.02f, 1.0f, 0.001f);
            }
            if (self->work[MO_TENT_MOVE_TIMER] == 190) {
                func_80078914(&sAudioZeroVec, NA_SE_EN_MOFER_BUBLE_DEMO);
            }
            if ((self->work[MO_TENT_MOVE_TIMER] > 150) && (self->work[MO_TENT_MOVE_TIMER] < 180)) {
                bubblePos2.x = (self->cameraEye.x + 20.0f) + 10.0f;
                bubblePos2.y = -250.0f;
                bubblePos2.z = self->cameraEye.z;
                EffectSsBubble_Spawn(globalCtx, &bubblePos2, 0.0f, 10.0f, 50.0f, Rand_ZeroFloat(0.05f) + 0.13f);
            }
            sp7C = (f32)0x1000;
            sp78 = 0.1f;
            if ((self->work[MO_TENT_MOVE_TIMER] > 100) && (self->work[MO_TENT_MOVE_TIMER] < 220)) {
                sp80 = 0.0f;
            } else if (self->work[MO_TENT_MOVE_TIMER] > 350) {
                sp80 = 2.0f;
                sp78 = 0.4f;
            } else if (self->work[MO_TENT_MOVE_TIMER] > 220) {
                sp80 = 7.0f;
                sp78 = 0.3f;
                sp7C = (f32)0x2000;
            } else {
                sp80 = 4.0f;
            }

            if (self->work[MO_TENT_MOVE_TIMER] > 250) {
                Math_ApproachF(&self->fwork[MO_CORE_INTRO_WATER_ALPHA], 100.0f, 1.0f, 1.0f);
            }
            if (self->targetIndex < 5) {
                if (sqrtf(SQ(dx) + SQ(dz) + SQ(dy)) < 40.0f) {
                    self->targetIndex++;
                    self->cameraYawRate = 0.0f;
                }
            } else {
                sp80 = 1.5f;
                sp7C = (f32)0x600;
            }
            Math_ApproachF(&self->actor.speedXZ, sp80, 1.0f, sp78);
            Math_ApproachF(&self->cameraYawRate, sp7C, 1.0f, 128.0f);
            if (self->work[MO_TENT_MOVE_TIMER] == 525) {
                func_8002DF54(globalCtx, &self->actor, 2);
            }
            if (self->work[MO_TENT_MOVE_TIMER] > 540) {
                self->csState = MO_INTRO_REVEAL;
                func_8002DF54(globalCtx, &self->actor, 1);
                sMorphaTent1->drawActor = true;
                player->actor.world.pos.x = 180.0f;
                player->actor.world.pos.z = -210.0f;
                player->actor.world.rot.y = -0x8000;
                player->actor.shape.rot.y = player->actor.world.rot.y;
                self->cameraYawShake = 0.0f;
                sMorphaTent1->baseAlpha = 150.0;
                self->actor.speedXZ = 0.0f;
                self->timers[2] = 200;
                self->cameraZoom = 60.0f;
                self->actor.world.pos = sMorphaTent1->actor.world.pos;
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_INTRO_REVEAL;
                self->actor.flags &= ~1;
                sMorphaTent1->actor.flags |= 1;
            } else {
                sMorphaTent1->xSwing = 0xCEC;
                sMorphaTent1->fwork[MO_TENT_SWING_RATE_X] = 0.0f;
                sMorphaTent1->fwork[MO_TENT_SWING_LAG_X] = 1000.0f;
                sMorphaTent1->fwork[MO_TENT_SWING_SIZE_X] = 2500.0f;
                break;
            }
        case MO_INTRO_REVEAL:
            if (self->timers[2] >= 160) {
                self->cameraEye.x = 150.0f;
                self->cameraEye.y = 60.0f;
                self->cameraEye.z = -230.0f;
                self->cameraAt.x = 170.0f;
                self->cameraAt.y = 40.0;
                self->cameraAt.z = -280.0f;
                sMorphaTent1->xSwing = 0xCEC;
                sMorphaTent1->fwork[MO_TENT_SWING_RATE_X] = 0.0f;
                sMorphaTent1->fwork[MO_TENT_SWING_LAG_X] = 1000.0f;
                sMorphaTent1->fwork[MO_TENT_SWING_SIZE_X] = 2500.0f;
                if (self->timers[2] == 160) {
                    self->cameraNextAt.y = 65.0f;
                    self->cameraNextAt.z = -280.0f;
                    self->cameraEyeVel.x = fabsf(self->cameraEye.x - 150.0f) * 0.1f;
                    self->cameraEyeVel.y = fabsf(self->cameraEye.y - 60.0f) * 0.1f;
                    self->cameraEyeVel.z = fabsf(self->cameraEye.z - -260.0f) * 0.1f;
                    self->cameraNextEye.x = 150.0f;
                    self->cameraNextEye.y = 60.0f;
                    self->cameraNextEye.z = -260.0f;
                    self->cameraNextAt.x = 155.0f;
                    self->cameraAtMaxVel.x = self->cameraAtMaxVel.y = self->cameraAtMaxVel.z = 0.1f;
                    self->cameraAtVel.x = fabsf(self->cameraAt.x - self->cameraNextAt.x) * 0.1f;
                    self->cameraAtVel.y = fabsf(self->cameraAt.y - self->cameraNextAt.y) * 0.1f;
                    self->cameraAtVel.z = fabsf(self->cameraAt.z - self->cameraNextAt.z) * 0.1f;
                    self->cameraEyeMaxVel.x = self->cameraEyeMaxVel.y = self->cameraEyeMaxVel.z = 0.1f;
                    self->cameraSpeedMod = 0.0f;
                    self->cameraAccel = 0.01f;
                    self->tentMaxAngle = 0.001f;
                    self->tentSpeed = 0.0f;
                    sp9F = 1;
                }
            } else {
                sp9F = 1;
            }
            if (self->timers[2] == 50) {
                self->cameraNextAt.x = 160.0f;
                self->cameraNextAt.y = 58.0f;
                self->cameraNextAt.z = -247.0f;
                self->cameraEyeVel.x = fabsf(self->cameraEye.x - 111.0f) * 0.1f;
                self->cameraEyeVel.y = fabsf(self->cameraEye.y - 133.0f) * 0.1f;
                self->cameraEyeVel.z = fabsf(self->cameraEye.z - -191.0f) * 0.1f;
                if (1) {}
                self->csState = MO_INTRO_FINISH;
                self->timers[2] = 110;
                self->cameraNextEye.x = 111.0f;
                self->cameraNextEye.y = 133.0f;
                self->cameraNextEye.z = -191.0f;
                self->cameraAtVel.x = fabsf(self->cameraAt.x - self->cameraNextAt.x) * 0.1f;
                self->cameraAtVel.y = fabsf(self->cameraAt.y - self->cameraNextAt.y) * 0.1f;
                self->cameraAtVel.z = fabsf(self->cameraAt.z - self->cameraNextAt.z) * 0.1f;
                self->cameraEyeMaxVel.y = 0.03f;
                self->cameraAtMaxVel.y = 0.03f;
                self->cameraSpeedMod = 0.0f;
                self->cameraAccel = 0.01f;
            }
            if (self->timers[2] == 150) {
                Audio_QueueSeqCmd(0x1B);
            }
            if (self->timers[2] == 130) {
                TitleCard_InitBossName(globalCtx, &globalCtx->actorCtx.titleCtx,
                                       SEGMENTED_TO_VIRTUAL(gMorphaTitleCardTex), 0xA0, 0xB4, 0x80, 0x28);
                gSaveContext.eventChkInf[7] |= 0x10;
            }
            break;
        case MO_INTRO_FINISH:
            sp9F = 1;
            self->cameraNextEye.x = 111.0f;
            self->cameraNextEye.y = 133.0f;
            self->cameraNextEye.z = -191.0f;
            self->cameraNextAt.x = 160.0f;
            self->cameraNextAt.y = 58.0f;
            self->cameraNextAt.z = -247.0f;
            if (self->timers[2] == 100) {
                sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_RETREAT;
                sMorphaTent1->timers[0] = 50;
            }
            if (self->timers[2] == 20) {
                camera2 = Gameplay_GetCamera(globalCtx, MAIN_CAM);
                camera2->eye = self->cameraEye;
                camera2->eyeNext = self->cameraEye;
                camera2->at = self->cameraAt;
                func_800C08AC(globalCtx, self->csCamera, 0);
                self->csState = self->csCamera = MO_BATTLE;
                func_80064534(globalCtx, &globalCtx->csCtx);
                func_8002DF54(globalCtx, &self->actor, 7);
            }
            break;
    }
    if (sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_READY) {
        sMorphaTent1->actor.world.pos.x = 180.0f;
        sMorphaTent1->actor.world.pos.z = -360.0f;
        sMorphaTent1->actor.prevPos = sMorphaTent1->actor.world.pos;
        sMorphaTent1->actor.speedXZ = 0.0f;
        sMorphaTent1->actor.shape.rot.y = sMorphaTent1->actor.yawTowardsPlayer;
    }
    if (self->csCamera != 0) {
        if (sp9F) {
            Math_ApproachF(&self->cameraEye.x, self->cameraNextEye.x, self->cameraEyeMaxVel.x,
                           self->cameraEyeVel.x * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraEye.y, self->cameraNextEye.y, self->cameraEyeMaxVel.y,
                           self->cameraEyeVel.y * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraEye.z, self->cameraNextEye.z, self->cameraEyeMaxVel.z,
                           self->cameraEyeVel.z * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraAt.x, self->cameraNextAt.x, self->cameraAtMaxVel.x,
                           self->cameraAtVel.x * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraAt.y, self->cameraNextAt.y, self->cameraAtMaxVel.y,
                           self->cameraAtVel.y * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraAt.z, self->cameraNextAt.z, self->cameraAtMaxVel.z,
                           self->cameraAtVel.z * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraSpeedMod, 1.0f, 1.0f, self->cameraAccel);
        } else if (self->csState < MO_INTRO_REVEAL) {
            func_8002D908(&self->actor);
            self->cameraEye.x += self->actor.velocity.x;
            self->cameraEye.y += self->actor.velocity.y;
            self->cameraEye.z += self->actor.velocity.z;
        }
        self->cameraUp.x = self->cameraUp.z =
            sinf(self->work[MO_TENT_VAR_TIMER] * 0.03f) * self->cameraYawShake * (-2.0f);
        self->cameraUp.y = 1.0f;
        Gameplay_CameraSetAtEyeUp(globalCtx, self->csCamera, &self->cameraAt, &self->cameraEye, &self->cameraUp);
        camera->eye = self->cameraEye;
        camera->eyeNext = self->cameraEye;
        camera->at = self->cameraAt;
        Gameplay_CameraSetFov(globalCtx, self->csCamera, self->cameraZoom);
    }

    if ((self->csState > MO_INTRO_START) && (self->work[MO_TENT_MOVE_TIMER] > 540)) {
        func_80078914(&sMorphaTent1->tentTipPos, NA_SE_EN_MOFER_APPEAR - SFX_FLAG);
    } else if (self->csState >= MO_INTRO_START) {
        func_80078914(&sAudioZeroVec, NA_SE_EN_MOFER_MOVE_DEMO - SFX_FLAG);
    }
}

void BossMo_DeathCs(BossMo* self, GlobalContext* globalCtx) {
    s16 i;
    s16 one;
    f32 dx;
    f32 dz;
    f32 sp80;
    f32 sp7C;
    Vec3f sp70;
    Vec3f sp64;
    Camera* camera = Gameplay_GetCamera(globalCtx, MAIN_CAM);
    Vec3f velocity;
    Vec3f pos;

    switch (self->csState) {
        case MO_DEATH_START:
            func_80064520(globalCtx, &globalCtx->csCtx);
            func_8002DF54(globalCtx, &self->actor, 8);
            self->csCamera = Gameplay_CreateSubCamera(globalCtx);
            Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
            Gameplay_ChangeCameraStatus(globalCtx, self->csCamera, CAM_STAT_ACTIVE);
            self->csState = MO_DEATH_MO_CORE_BURST;
            self->cameraEye = camera->eye;
            self->timers[0] = 90;
            dx = self->actor.world.pos.x - self->cameraEye.x;
            dz = self->actor.world.pos.z - self->cameraEye.z;
            self->cameraYaw = Math_FAtan2F(dx, dz);
            self->cameraDist = sqrtf(SQ(dx) + SQ(dz));
            self->cameraYawRate = 0.0f;
        case MO_DEATH_MO_CORE_BURST:
            self->baseAlpha = 0.0f;
            if (self->timers[0] & 4) {
                sp80 = 0.005f;
                sp7C = 0.015f;
            } else {
                sp80 = 0.015f;
                sp7C = 0.005f;
            }
            Math_ApproachF(&self->actor.scale.x, sp80, 0.5f, 0.002f);
            self->actor.scale.z = self->actor.scale.x;
            Math_ApproachF(&self->actor.scale.y, sp7C, 0.5f, 0.002f);
            self->cameraYaw += self->cameraYawRate;
            if (self->timers[0] >= 30) {
                Math_ApproachF(&self->cameraYawRate, 0.05f, 1.0f, 0.002f);
            } else {
                Math_ApproachF(&self->cameraYawRate, 0.0f, 1.0f, 0.002f);
            }
            Math_ApproachF(&self->actor.world.pos.y, 150.0f, 0.05f, 5.0f);
            Math_ApproachF(&self->cameraEye.y, 100.0f, 0.05f, 2.0f);
            self->cameraAt = self->cameraNextAt = self->actor.world.pos;
            if (self->timers[0] > 20) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_MOFER_DEAD - SFX_FLAG);
            }
            if (self->timers[0] == 20) {
                for (i = 0; i < 300; i++) {
                    velocity.x = Rand_CenteredFloat(10.0f);
                    velocity.y = Rand_CenteredFloat(10.0f);
                    velocity.z = Rand_CenteredFloat(10.0f);
                    pos = self->actor.world.pos;
                    pos.x += 2.0f * velocity.x;
                    pos.y += 2.0f * velocity.y;
                    pos.z += 2.0f * velocity.z;
                    BossMo_SpawnDroplet(MO_FX_DROPLET, (BossMoEffect*)globalCtx->specialEffects, &pos, &velocity,
                                        Rand_ZeroFloat(0.08f) + 0.13f);
                }
                self->drawActor = false;
                self->actor.flags &= ~1;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_MOFER_CORE_JUMP);
                Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 70, NA_SE_EN_MOFER_LASTVOICE);
            }
            if (self->timers[0] == 0) {
                self->csState = MO_DEATH_DRAIN_WATER_1;
                self->cameraDist = 490.0f;
                self->actor.world.pos.y = -1000.0f;
                self->fwork[MO_TENT_SWING_SIZE_X] = 15.0f;
                self->cameraYaw = 0.0f;
                self->cameraEye.x = 490.0f;
                self->cameraEye.y = 50.0f;
                self->cameraEye.z = 0.0f;
                self->cameraAt.x = 0;
                self->cameraAt.y = -100.0f;
                self->cameraAt.z = 0.0f;
                self->work[MO_TENT_VAR_TIMER] = self->work[MO_TENT_MOVE_TIMER] = 0;
                self->cameraAtMaxVel.y = 0.05f;
                self->cameraAtVel.y = 4.0f;
                self->cameraSpeedMod = 0.0f;
                self->cameraAccel = 0.02f;
                self->cameraNextAt.y = 320.0f;
                if (1) {}
                self->timers[0] = 100;
                sMorphaTent1->drawActor = true;
                sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_DEATH_3;
                sMorphaTent1->actor.shape.rot.x = 0;
                sMorphaTent1->actor.world.pos.x = 0.0f;
                sMorphaTent1->actor.world.pos.y = -50.0f;
                sMorphaTent1->actor.world.pos.z = 0.0f;
                sMorphaTent1->fwork[MO_TENT_MAX_STRETCH] = 1.0f;
                sMorphaTent1->tentPulse = 0.2f;
                sMorphaCore->waterLevel = -50.0f;
                sMorphaTent1->flattenRate = 0.0f;
                sMorphaTent1->noBubbles = 0;
                for (i = 0; i < 41; i++) {
                    sMorphaTent1->tentStretch[i].y = 5.0f;
                }
                sMorphaTent1->fwork[MO_TENT_SWING_RATE_X] = -400.0f;
                sMorphaTent1->fwork[MO_TENT_SWING_LAG_X] = -3200.0f;
                sMorphaTent1->fwork[MO_TENT_SWING_SIZE_X] = .0f;
                sMorphaTent1->fwork[MO_TENT_SWING_RATE_Z] = 3000.0f;
                sMorphaTent1->fwork[MO_TENT_SWING_LAG_Z] = 2500.0f;
                sMorphaTent1->fwork[MO_TENT_SWING_SIZE_Z] = 4000.0f;
                sMorphaTent1->tentMaxAngle = 1.0f;
                sMorphaTent1->tentSpeed = 20480.0f;
                sMorphaTent1->baseAlpha = 150.0f;
                sMorphaTent1->cutIndex = sMorphaTent1->meltIndex = 0;
                sMorphaTent1->cutScale = 1.0f;
                Actor_SetScale(&sMorphaTent1->actor, 0.01f);
            }
            break;
        case MO_DEATH_DRAIN_WATER_1:
            if (self->timers[0] == 0) {
                self->csState = MO_DEATH_DRAIN_WATER_2;
                self->cameraAt.y = -200.0f;
                self->cameraNextAt.y = 320.0f;
                self->cameraAtMaxVel.y = 0.05f;
                self->cameraAtVel.y = 4.0f;
                self->cameraSpeedMod = 0.0f;
                self->cameraAccel = 0.0f;
                sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_DEATH_1;
                self->timers[0] = 125;
                sMorphaTent1->fwork[MO_TENT_MAX_STRETCH] = 3.7000003f;
                self->cameraYaw = 0.5f;
                self->cameraDist = 200.0f;
                return;
            }
            break;
        case MO_DEATH_DRAIN_WATER_2:
            if (self->timers[0] == 0) {
                self->cameraAccel = 0.02f;
                sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_DEATH_2;
                self->csState = MO_DEATH_CEILING;
                sMorphaTent1->timers[0] = 120;
                self->timers[0] = 150;
            }
        case MO_DEATH_CEILING:
            Math_ApproachF(&self->cameraYaw, 0.0f, 0.05f, 0.0029999996f);
            Math_ApproachF(&self->cameraDist, 490.0f, 0.1f, 1.0f);
            if (self->timers[0] == 0) {
                self->csState = MO_DEATH_DROPLET;
                self->timers[0] = 140;
                self->cameraYawRate = 0.0f;
                self->cameraSpeed = 0.0f;
            }
            break;
        case MO_DEATH_DROPLET:
            if (self->timers[0] == 30) {
                sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_DEATH_5;
                sMorphaTent1->timers[0] = 30;
                sMorphaTent1->tentMaxAngle = 0.0f;
                sMorphaTent1->tentSpeed = sMorphaTent1->tentMaxAngle;
            }
            if (self->timers[0] == 0) {
                if (-100.0f < self->cameraEye.y) {
                    Math_ApproachF(&self->cameraEye.y, sMorphaTent1->actor.world.pos.y - 100.0f, 0.1f, 2000.0f);
                } else {
                    Math_ApproachF(&self->cameraEye.y, -200.0f, 0.1f, 2000.0f);
                }

                Math_ApproachF(&self->cameraAt.y, (sMorphaTent1->actor.world.pos.y - 50.0f) + 30.0f, 0.5f, 2000.0f);
                self->cameraNextAt.y = self->cameraAt.y;
            } else {
                Math_ApproachF(&self->cameraEye.y, 300.0f, 0.05f, self->cameraSpeed);
            }
            Math_ApproachF(&self->cameraYaw, -M_PI / 2.0f, 0.05f, self->cameraYawRate);
            Math_ApproachF(&self->cameraSpeed, 3.0f, 1.0f, 0.05f);
            Math_ApproachF(&self->cameraYawRate, 0.012999999f, 1.0f, 0.0005f);
            if (sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_DEATH_6) {
                Math_ApproachF(&self->cameraDist, 200.0f, 0.02f, self->cameraSpeed);
                if (sMorphaTent1->timers[0] == 0) {
                    self->csState = MO_DEATH_FINISH;
                    camera->eye = self->cameraEye;
                    camera->eyeNext = self->cameraEye;
                    camera->at = self->cameraAt;
                    func_800C08AC(globalCtx, self->csCamera, 0);
                    self->csCamera = 0;
                    func_80064534(globalCtx, &globalCtx->csCtx);
                    func_8002DF54(globalCtx, &self->actor, 7);
                    sMorphaTent1->actor.world.pos.y = -1000.0f;
                }
            } else {
                Math_ApproachF(&self->cameraDist, 150.0f, 0.05f, self->cameraSpeed);
            }
            break;
        case MO_DEATH_FINISH:
            break;
    }
    if ((self->csState > MO_DEATH_START) && (self->csState < MO_DEATH_FINISH)) {
        if (self->work[MO_TENT_MOVE_TIMER] < 500) {
            func_80078914(&sAudioZeroVec, NA_SE_EN_MOFER_APPEAR - SFX_FLAG);
        }
        if ((self->work[MO_TENT_MOVE_TIMER] < 490) && (self->work[MO_TENT_MOVE_TIMER] > 230)) {
            func_80078914(&sAudioZeroVec, NA_SE_EV_DROP_FALL - SFX_FLAG);
        }
        if (self->work[MO_TENT_MOVE_TIMER] < 220) {
            func_80078914(&sAudioZeroVec, NA_SE_EV_SCOOPUP_WATER - SFX_FLAG);
        }
    }
    if (sMorphaCore->waterLevel < -200.0f) {
        globalCtx->roomCtx.unk_74[0]++;
        if (globalCtx->roomCtx.unk_74[0] >= 0xFF) {
            globalCtx->roomCtx.unk_74[0] = 0xFF;
        }
    }
    if (sMorphaCore->waterLevel < -250.0f) {
        Math_ApproachF(&sMorphaTent1->waterTexAlpha, 0.0f, 1.0f, 3.0f);
    }
    Math_ApproachF(&self->fwork[MO_TENT_SWING_SIZE_X], 0.0f, 0.1f, 0.05f);

    sp70.x = self->cameraDist;
    sp70.y = 0.0f;
    sp70.z = 0.0f;
    Matrix_RotateY(self->cameraYaw, MTXMODE_NEW);
    Matrix_MultVec3f(&sp70, &sp64);
    self->cameraEye.x = sp64.x + self->cameraAt.x;
    self->cameraEye.z = sp64.z + self->cameraAt.z;
    one = 1; // Super fake, but it works
    if (self->csCamera != 0) {
        if (one) {
            Math_ApproachF(&self->cameraAt.y, self->cameraNextAt.y, self->cameraAtMaxVel.y,
                           self->cameraAtVel.y * self->cameraSpeedMod);
            Math_ApproachF(&self->cameraSpeedMod, 1.0f, 1.0f, self->cameraAccel);
        }
        Gameplay_CameraSetAtEye(globalCtx, self->csCamera, &self->cameraAt, &self->cameraEye);
    }
}

void BossMo_CoreCollisionCheck(BossMo* self, GlobalContext* globalCtx) {
    s16 i;
    Player* player = GET_PLAYER(globalCtx);

    osSyncPrintf(VT_FGCOL(YELLOW));
    osSyncPrintf("Core_Damage_check START\n");
    if (self->coreCollider.base.atFlags & AT_HIT) {
        self->coreCollider.base.atFlags &= ~AT_HIT;
        if (self->work[MO_TENT_ACTION_STATE] == MO_CORE_UNDERWATER) {
            self->work[MO_CORE_WAIT_IN_WATER] = true;
            self->timers[0] = 150;
        }
    }
    if (self->coreCollider.base.acFlags & AC_HIT) {
        ColliderInfo* hurtbox = self->coreCollider.info.acHitInfo;
        // "hit!!"
        osSyncPrintf("Core_Damage_check 当り！！\n");
        self->coreCollider.base.acFlags &= ~AC_HIT;
        if ((hurtbox->toucher.dmgFlags & 0x00020000) && (self->work[MO_TENT_ACTION_STATE] == MO_CORE_ATTACK)) {
            self->work[MO_TENT_ACTION_STATE] = MO_CORE_RETREAT;
        }
        // "hit 2 !!"
        osSyncPrintf("Core_Damage_check 当り 2 ！！\n");
        if ((self->work[MO_TENT_ACTION_STATE] != MO_CORE_UNDERWATER) && (self->work[MO_TENT_INVINC_TIMER] == 0)) {
            u8 damage = CollisionCheck_GetSwordDamage(hurtbox->toucher.dmgFlags);

            if ((damage != 0) && (self->work[MO_TENT_ACTION_STATE] < MO_CORE_ATTACK)) {
                // "sword hit !!"
                osSyncPrintf("Core_Damage_check 剣 当り！！\n");
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_STUNNED;
                self->timers[0] = 25;

                self->actor.speedXZ = 15.0f;

                self->actor.world.rot.y = self->actor.yawTowardsPlayer + 0x8000;
                self->work[MO_CORE_DMG_FLASH_TIMER] = 15;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_MOFER_CORE_DAMAGE);
                self->actor.colChkInfo.health -= damage;
                self->hitCount++;
                if ((s8)self->actor.colChkInfo.health <= 0) {
                    if (((sMorphaTent1->csCamera == 0) && (sMorphaTent2 == NULL)) ||
                        ((sMorphaTent1->csCamera == 0) && (sMorphaTent2 != NULL) && (sMorphaTent2->csCamera == 0))) {
                        Enemy_StartFinishingBlow(globalCtx, &self->actor);
                        Audio_QueueSeqCmd(0x100100FF);
                        self->csState = MO_DEATH_START;
                        sMorphaTent1->drawActor = false;
                        sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_DEATH_START;
                        sMorphaTent1->baseAlpha = 0.0f;
                        if (sMorphaTent2 != NULL) {
                            sMorphaTent2->tent2KillTimer = 1;
                        }
                        if (player->actor.parent != NULL) {
                            player->unk_850 = 0x65;
                            player->actor.parent = NULL;
                            player->csMode = 0;
                        }
                    } else {
                        self->actor.colChkInfo.health = 1;
                    }
                }
                self->work[MO_TENT_INVINC_TIMER] = 10;
            } else if (!(hurtbox->toucher.dmgFlags & 0x00100000) && (hurtbox->toucher.dmgFlags & 0x80)) {
                if (self->work[MO_TENT_ACTION_STATE] >= MO_CORE_ATTACK) {
                    func_80078914(&sMorphaTent1->tentTipPos, NA_SE_EN_MOFER_CUT);
                    sMorphaTent1->cutIndex = self->work[MO_CORE_POS_IN_TENT];
                    sMorphaTent1->meltIndex = sMorphaTent1->cutIndex + 1;
                    sMorphaTent1->cutScale = 1.0f;
                    sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_CUT;
                    sMorphaTent1->timers[0] = 40;
                    sMorphaTent1->actor.flags &= ~1;
                    if (player->actor.parent == &sMorphaTent1->actor) {
                        player->unk_850 = 0x65;
                        player->actor.parent = NULL;
                        player->csMode = 0;
                    }
                }
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_STUNNED;
                self->timers[0] = 30;
                self->work[MO_TENT_INVINC_TIMER] = 10;
                self->actor.speedXZ = 0.0f;
            }
            for (i = 0; i < 10; i++) {
                Vec3f pos;
                Vec3f velocity;

                velocity.x = Rand_CenteredFloat(4.0f);
                velocity.y = Rand_ZeroFloat(2.0f) + 3.0f;
                velocity.z = Rand_CenteredFloat(4.0f);
                pos = self->actor.world.pos;
                pos.x += (velocity.x * 3.0f);
                pos.z += (velocity.z * 3.0f);
                BossMo_SpawnDroplet(MO_FX_DROPLET, (BossMoEffect*)globalCtx->specialEffects, &pos, &velocity,
                                    Rand_ZeroFloat(0.08f) + 0.13f);
            }
        }
    }
    // "end !!"
    osSyncPrintf("Core_Damage_check 終わり ！！\n");
    osSyncPrintf(VT_RST);
}

void BossMo_Core(BossMo* self, GlobalContext* globalCtx) {
    static f32 coreBulge[11] = {
        0.1f, 0.15f, 0.2f, 0.3f, 0.4f, 0.43f, 0.4f, 0.3f, 0.2f, 0.15f, 0.1f,
    };
    u8 nearLand;
    s16 i;                                  // not on stack
    Player* player = GET_PLAYER(globalCtx); // not on stack
    f32 spDC;
    f32 spD8;
    f32 spD4;
    f32 spD0;
    f32 spCC;
    s32 padC8;
    s32 temp;         // not on stack
    f32 xScaleTarget; // not on stack
    f32 yScaleTarget;
    Vec3f effectPos;
    Vec3f effectVelocity;
    Vec3f effectAccel;
    s32 pad94;
    s32 pad90;
    s16 j;
    s16 index; // not on stack
    f32 sp88;
    s32 pad84;
    f32 sp80;
    f32 sp7C;
    Vec3f sp70;
    Vec3f sp64;
    f32 sp60;
    f32 sp5C;
    f32 sp58;

    self->waterTex1x += -1.0f;
    self->waterTex1y += -1.0f;
    self->waterTex2x = self->waterTex2x;
    self->waterTex2y++;

    Math_ApproachF(&self->baseAlpha, 255.0f, 1.0f, 10.0f);
    if ((self->csState != MO_BATTLE) && (self->csState < MO_DEATH_START)) {
        BossMo_IntroCs(self, globalCtx);
        if (self->work[MO_TENT_ACTION_STATE] == MO_CORE_INTRO_WAIT) {
            self->actor.flags &= ~1;
            return;
        }
    } else if (self->csState >= MO_DEATH_START) {
        BossMo_DeathCs(self, globalCtx);
        return;
    }
    if ((self->work[MO_TENT_ACTION_STATE] < MO_CORE_ATTACK) && (self->work[MO_TENT_ACTION_STATE] >= MO_CORE_MOVE) &&
        (self->actor.world.pos.y > MO_WATER_LEVEL(globalCtx))) {
        if (self->actor.velocity.y > 0.0f) {
            xScaleTarget = 0.005f;
            yScaleTarget = 0.015f;
        } else {
            xScaleTarget = 0.015f;
            yScaleTarget = 0.005f;
        }
    } else {
        xScaleTarget = yScaleTarget = 0.008f;
    }
    Math_ApproachF(&self->actor.scale.x, xScaleTarget, 0.2f, 0.001f);
    self->actor.scale.z = self->actor.scale.x;
    Math_ApproachF(&self->actor.scale.y, yScaleTarget, 0.2f, 0.001f);
    self->work[MO_CORE_DRAW_SHADOW] = BossMo_NearLand(&self->actor.world.pos, 15.0f);
    nearLand = BossMo_NearLand(&self->actor.world.pos, 0.0f);
    if ((player->actor.world.pos.y < (MO_WATER_LEVEL(globalCtx) - 50.0f)) &&
        ((self->work[MO_TENT_ACTION_STATE] == MO_CORE_MOVE) ||
         (self->work[MO_TENT_ACTION_STATE] == MO_CORE_MAKE_TENT))) {
        self->work[MO_TENT_ACTION_STATE] = MO_CORE_UNDERWATER;
        self->actor.speedXZ = 0.0f;
        self->work[MO_CORE_WAIT_IN_WATER] = 0;
    }
    switch (self->work[MO_TENT_ACTION_STATE]) {
        case MO_CORE_MOVE:
            self->actor.flags |= 1;
            if ((self->timers[0] == 0) &&
                ((sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_WAIT) ||
                 (sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_READY)) &&
                (self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx))) {
                self->actor.speedXZ = 0.0f;
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_MAKE_TENT;
                if (sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_WAIT) {
                    sMorphaTent1->work[MO_TENT_ACTION_STATE] = MO_TENT_SPAWN;
                    sMorphaTent1->timers[0] = 70;
                    sMorphaTent1->actor.shape.rot.y = sMorphaTent1->actor.yawTowardsPlayer;
                }
            }
            break;
        case MO_CORE_MAKE_TENT:
            if ((sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_DESPAWN) ||
                (sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_WAIT)) {
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_MOVE;
                self->timers[0] = 70;
            }
            if (sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_CUT) {
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_ATTACK;
                self->work[MO_CORE_POS_IN_TENT] = 0;
                self->timers[0] = 0;
            }
            if (sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_ATTACK) {
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_ATTACK;
                self->work[MO_CORE_POS_IN_TENT] = 0;
                self->timers[0] = 0;
                self->actor.speedXZ = 0.0f;
            }
            break;
        case MO_CORE_UNDERWATER:
            if (player->actor.world.pos.y >= MO_WATER_LEVEL(globalCtx)) {
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_MOVE;
                self->actor.speedXZ = 0.0f;
            }
            break;
        case MO_CORE_STUNNED:
            self->actor.flags |= 1;
            if (self->timers[0] == 0) {
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_MOVE;
                self->timers[0] = 30;
            }
            if (self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx)) {
                self->work[MO_TENT_ACTION_STATE] = MO_CORE_MAKE_TENT;
                self->timers[0] = 50;
                self->actor.speedXZ = 0.0f;
            }
            break;
        case MO_CORE_UNUSED:
            break;
    }
    if (self->timers[0] == 0) {
        switch (self->work[MO_TENT_ACTION_STATE]) {
            case MO_CORE_ATTACK:
                self->actor.flags |= 1;
                self->work[MO_CORE_POS_IN_TENT]++;
                if (sMorphaTent1->work[MO_TENT_ACTION_STATE] == MO_TENT_ATTACK) {
                    temp = (s16)(Math_SinS(self->work[MO_TENT_MOVE_TIMER] * 0x300) * 10.0f) + 15;
                    if (self->work[MO_CORE_POS_IN_TENT] >= temp) {
                        self->work[MO_CORE_POS_IN_TENT] = temp;
                    }
                }
                if ((sMorphaTent1->work[MO_TENT_ACTION_STATE] != MO_TENT_ATTACK) &&
                    (sMorphaTent1->work[MO_TENT_ACTION_STATE] != MO_TENT_CUT)) {
                    self->work[MO_TENT_ACTION_STATE] = MO_CORE_RETREAT;
                    self->timers[0] = 0;
                }
                break;
            case MO_CORE_RETREAT:
                self->work[MO_CORE_POS_IN_TENT]--;
                if (self->work[MO_CORE_POS_IN_TENT] <= 0) {
                    self->work[MO_TENT_ACTION_STATE] = MO_CORE_MAKE_TENT;
                    self->timers[0] = 100;
                    self->tentSpeed = 0.0f;
                    self->actor.speedXZ = 0.0f;
                }
                self->timers[0] = 0;
                break;
            case MO_CORE_INTRO_REVEAL:
                self->actor.flags &= ~1;
                self->work[MO_CORE_POS_IN_TENT]++;
                temp = (s16)(Math_SinS(self->work[MO_TENT_MOVE_TIMER] * 0x500) * 10.0f) + 15;
                if (self->work[MO_CORE_POS_IN_TENT] >= temp) {
                    self->work[MO_CORE_POS_IN_TENT] = temp;
                }
                if (sMorphaTent1->work[MO_TENT_ACTION_STATE] != MO_TENT_READY) {
                    self->work[MO_TENT_ACTION_STATE] = MO_CORE_RETREAT;
                    self->timers[0] = 0;
                }
                break;
        }
    }
    if (self->work[MO_TENT_ACTION_STATE] >= MO_CORE_ATTACK) {
        if (self->work[MO_CORE_POS_IN_TENT] < 0) {
            self->work[MO_CORE_POS_IN_TENT] = 0;
        } else if (self->work[MO_CORE_POS_IN_TENT] >= 41) {
            self->work[MO_CORE_POS_IN_TENT] = 40;
        }
        index = (300 - (self->work[MO_CORE_POS_IN_TENT] * 2) + sMorphaTent1->widthIndex) % 300;
        sp88 = sMorphaTent1->tentWidth[index] * sTentWidth[self->work[MO_CORE_POS_IN_TENT]];
        for (j = -5; j < 6; j++) {
            index = (self->work[MO_CORE_POS_IN_TENT] + j) - 2;
            if ((0 <= index) && (index < 41)) {
                Math_ApproachF(&sMorphaTent1->tentScale[index].x, ((coreBulge[j + 5] * 300.0f) / 100.0f) + sp88, 0.75f,
                               5.0f);
            }
        }
        self->targetPos.x = sMorphaTent1->tentPos[self->work[MO_CORE_POS_IN_TENT]].x;
        self->targetPos.y = sMorphaTent1->tentPos[self->work[MO_CORE_POS_IN_TENT]].y;
        self->targetPos.z = sMorphaTent1->tentPos[self->work[MO_CORE_POS_IN_TENT]].z;
        if (self->work[MO_CORE_POS_IN_TENT] <= 1) {
            self->targetPos.y -= 20.0f;
        }
        Math_ApproachF(&self->actor.world.pos.x, self->targetPos.x, 0.5f, self->actor.speedXZ);
        Math_ApproachF(&self->actor.world.pos.y, self->targetPos.y, 0.5f, self->actor.speedXZ);
        Math_ApproachF(&self->actor.world.pos.z, self->targetPos.z, 0.5f, self->actor.speedXZ);
        Math_ApproachF(&self->actor.speedXZ, 30.0f, 1.0f, 1.0f);
    } else {
        switch (self->work[MO_TENT_ACTION_STATE]) {
            case MO_CORE_MOVE:
                sp80 = Math_SinS(self->work[MO_TENT_VAR_TIMER] * 0x800) * 100.0f;
                sp7C = Math_CosS(self->work[MO_TENT_VAR_TIMER] * 0x800) * 100.0f;
                Math_ApproachF(&self->actor.world.pos.x, sMorphaTent1->targetPos.x + sp80, 0.05f, self->actor.speedXZ);
                Math_ApproachF(&self->actor.world.pos.z, sMorphaTent1->targetPos.z + sp7C, 0.05f, self->actor.speedXZ);
                Math_ApproachF(&self->actor.speedXZ, 10.0f, 1.0f, 0.5f);
                break;
            case MO_CORE_STUNNED:
                self->actor.velocity.x = Math_SinS(self->actor.world.rot.y) * self->actor.speedXZ;
                self->actor.velocity.z = Math_CosS(self->actor.world.rot.y) * self->actor.speedXZ;
                self->actor.world.pos.x += self->actor.velocity.x;
                self->actor.world.pos.z += self->actor.velocity.z;
                break;
        }
        if ((self->work[MO_TENT_ACTION_STATE] == MO_CORE_MOVE) ||
            (self->work[MO_TENT_ACTION_STATE] == MO_CORE_STUNNED)) {
            self->actor.world.pos.y += self->actor.velocity.y;
            self->actor.velocity.y -= 1.0f;
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 20.0f, 100.0f, 1);
            effectVelocity.x = effectVelocity.y = effectVelocity.z = 0.0f;
            for (i = 0; i < 1; i++) {
                effectPos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
                effectPos.y = Rand_CenteredFloat(20.0f) + self->actor.world.pos.y;
                effectPos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
                BossMo_SpawnDroplet(MO_FX_DROPLET, (BossMoEffect*)globalCtx->specialEffects, &effectPos,
                                    &effectVelocity, Rand_ZeroFloat(0.02f) + 0.05f);
            };

            if (nearLand) {
                if (self->actor.world.pos.y <= 10) {
                    self->actor.world.pos.y = 10;
                    self->actor.velocity.y = -0.01f;
                    if (self->timers[1] != 0) {
                        if (self->timers[1] == 1) {
                            self->actor.velocity.y = 6.0f;
                        }
                    } else {
                        self->timers[1] = 2;
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_MOFER_CORE_LAND);
                        for (i = 0; i < 10; i++) {
                            effectVelocity.x = Rand_CenteredFloat(4.0f);
                            effectVelocity.y = Rand_ZeroFloat(2.0f) + 3.0f;
                            effectVelocity.z = Rand_CenteredFloat(4.0f);
                            effectPos = self->actor.world.pos;
                            effectPos.x += effectVelocity.x;
                            effectPos.z += effectVelocity.z;
                            BossMo_SpawnDroplet(MO_FX_DROPLET, (BossMoEffect*)globalCtx->specialEffects, &effectPos,
                                                &effectVelocity, Rand_ZeroFloat(0.08f) + 0.13f);
                        }
                        effectVelocity.x = effectVelocity.y = effectVelocity.z = 0.0f;
                        effectPos = self->actor.world.pos;
                        effectPos.y = 0.0f;
                        BossMo_SpawnDroplet(MO_FX_DROPLET, (BossMoEffect*)globalCtx->specialEffects, &effectPos,
                                            &effectVelocity, 0.4f);
                    }
                }
            } else if (self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx)) {
                self->actor.velocity.y = BossMo_NearLand(&self->actor.world.pos, 40.0f) ? 15.0f : 6.0f;
                if ((self->actor.world.pos.y + 15.0f) >= MO_WATER_LEVEL(globalCtx)) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MOFER_CORE_JUMP);
                }
            }
        } else if (self->work[MO_TENT_ACTION_STATE] >= MO_CORE_MOVE) {
            if (self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx)) {
                if (self->work[MO_TENT_ACTION_STATE] == MO_CORE_MAKE_TENT) {
                    self->targetPos.x = sMorphaTent1->targetPos.x;
                    self->targetPos.y = sMorphaTent1->actor.world.pos.y - 40.0f;
                    self->targetPos.z = sMorphaTent1->targetPos.z;
                    Math_ApproachF(&self->actor.speedXZ, 10.0f, 1.0f, 0.5f);
                } else if (self->work[MO_TENT_ACTION_STATE] == MO_CORE_UNDERWATER) {
                    switch (self->work[MO_CORE_WAIT_IN_WATER]) {
                        case false:
                            self->targetPos = player->actor.world.pos;
                            self->targetPos.y += 30.0f;
                            sp70.x = 0.0f;
                            sp70.y = 0.0f;
                            sp70.z = 100.0f;
                            Matrix_RotateY((player->actor.world.rot.y / (f32)0x8000) * M_PI, MTXMODE_NEW);
                            Matrix_MultVec3f(&sp70, &sp64);
                            self->targetPos.x = player->actor.world.pos.x + sp64.x;
                            self->targetPos.y = player->actor.world.pos.y + 30.0f;
                            self->targetPos.z = player->actor.world.pos.z + sp64.z;
                            Math_ApproachF(&self->actor.speedXZ, 10.0f, 1.0f, 1.0f);
                            if (self->timers[0] == 0) {
                                self->work[MO_CORE_WAIT_IN_WATER] = true;
                                self->timers[0] = (s16)Rand_ZeroFloat(50.0f) + 50;
                            }
                            break;
                        case true:
                            Math_ApproachF(&self->actor.speedXZ, 1.0f, 1.0f, 0.5f);
                            if (self->timers[0] == 0) {
                                self->work[MO_CORE_WAIT_IN_WATER] = false;
                                self->timers[0] = (s16)Rand_ZeroFloat(20.0f) + 20;
                                Audio_PlayActorSound2(&self->actor, NA_SE_EN_MOFER_CORE_MOVE_WT);
                            }
                            break;
                    }
                }
                self->targetPos.x += Math_SinS(self->work[MO_TENT_MOVE_TIMER] * 3096.0f) * 30.0f;
                self->targetPos.y += Math_SinS(self->work[MO_TENT_MOVE_TIMER] * 2096.0f) * 30.0f;
                self->targetPos.z += Math_SinS(self->work[MO_TENT_MOVE_TIMER] * 2796.0f) * 30.0f;
                self->tentMaxAngle = 5.0f;
                self->tentSpeed = 4000.0f;
                spDC = self->targetPos.x - self->actor.world.pos.x;
                spD8 = self->targetPos.y - self->actor.world.pos.y;
                spD4 = self->targetPos.z - self->actor.world.pos.z;
                spCC = (s16)(Math_FAtan2F(spDC, spD4) * (0x8000 / M_PI));
                spD0 = (s16)(Math_FAtan2F(spD8, sqrtf(SQ(spDC) + SQ(spD4))) * (0x8000 / M_PI));
                Math_ApproachS(&self->actor.world.rot.y, spCC, self->tentMaxAngle, self->tentSpeed);
                Math_ApproachS(&self->actor.world.rot.x, spD0, self->tentMaxAngle, self->tentSpeed);
                func_8002D908(&self->actor);
            } else {
                self->actor.world.pos.y += self->actor.velocity.y;
                self->actor.velocity.y -= 1.0f;
            }
            func_8002D7EC(&self->actor);
            temp = (self->actor.world.pos.y < -200.0f) ? 5 : 1;
            self->actor.world.pos.y -= 20.0f;
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 20.0f, 100.0f, temp);
            self->actor.world.pos.y += 20.0f;
        }
    }
    if ((self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx)) && (MO_WATER_LEVEL(globalCtx) <= self->actor.prevPos.y)) {
        if (self->actor.velocity.y < -5.0f) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MOFER_CORE_JUMP);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MOFER_CORE_SMJUMP);
        }
        if ((self->timers[3] != 0) || ((sMorphaTent1->fwork[MO_TENT_MAX_STRETCH] > 0.2f) &&
                                       (fabsf(self->actor.world.pos.x - sMorphaTent1->actor.world.pos.x) < 30.0f) &&
                                       (fabsf(self->actor.world.pos.z - sMorphaTent1->actor.world.pos.z) < 30.0f))) {
            // This space intentionally left blank.
        } else {
            self->timers[3] = 8;
            for (i = 0; i < 10; i++) {
                sp5C = Rand_ZeroFloat(3.14f);
                sp60 = Rand_ZeroFloat(0.6f) + 1.6f;
                effectVelocity.x = Math_SinS(((i * (f32)0x10000) / 10.0f) + sp5C) * sp60;
                effectVelocity.z = Math_CosS(((i * (f32)0x10000) / 10.0f) + sp5C) * sp60;
                effectVelocity.y = Rand_ZeroFloat(0.3f) + 3.0f;

                effectPos = self->actor.world.pos;
                effectPos.x += effectVelocity.x * 3.0f;
                effectPos.y = MO_WATER_LEVEL(globalCtx);
                effectPos.z += effectVelocity.z * 3.0f;
                BossMo_SpawnDroplet(MO_FX_SPLASH, (BossMoEffect*)globalCtx->specialEffects, &effectPos, &effectVelocity,
                                    Rand_ZeroFloat(0.075f) + 0.15f);
            }
            effectPos = self->actor.world.pos;
            effectPos.y = MO_WATER_LEVEL(globalCtx);
            BossMo_SpawnRipple(globalCtx->specialEffects, &effectPos, 100.0f, 800.0f, 100, 290, MO_FX_SMALL_RIPPLE);
            BossMo_SpawnRipple(globalCtx->specialEffects, &effectPos, 50.0f, 600.0f, 70, 290, MO_FX_SMALL_RIPPLE);
            BossMo_SpawnRipple(globalCtx->specialEffects, &effectPos, 0, 400.0f, 50, 290, MO_FX_SMALL_RIPPLE);
        }
    }
    if ((self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx)) || (self->work[MO_TENT_ACTION_STATE] >= MO_CORE_ATTACK)) {
        for (i = 0; i < 3; i++) {
            effectAccel.x = effectAccel.z = 0.0f;
            effectVelocity.x = effectVelocity.y = effectVelocity.z = 0.0f;
            if (self->work[MO_TENT_ACTION_STATE] >= MO_CORE_ATTACK) {
                effectAccel.y = 0.0f;
                sp58 = 10.0f;
            } else {
                effectAccel.y = 0.1f;
                sp58 = 20.0f;
            }
            effectPos.x = Rand_CenteredFloat(sp58) + self->actor.world.pos.x;
            effectPos.y = Rand_CenteredFloat(sp58) + self->actor.world.pos.y;
            effectPos.z = Rand_CenteredFloat(sp58) + self->actor.world.pos.z;
            BossMo_SpawnBubble(globalCtx->specialEffects, &effectPos, &effectVelocity, &effectAccel,
                               Rand_ZeroFloat(0.05f) + 0.1f, NULL);
        }
    }
    BossMo_CoreCollisionCheck(self, globalCtx);
}

void BossMo_UpdateCore(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossMo* self = THIS;
    s16 i;
    Player* player = GET_PLAYER(globalCtx);

    osSyncPrintf("CORE mode = <%d>\n", self->work[MO_TENT_ACTION_STATE]);
    if (sMorphaTent2 == NULL) {
        MO_WATER_LEVEL(globalCtx) = sMorphaTent1->waterLevelMod + (s16)self->waterLevel;
    } else {
        MO_WATER_LEVEL(globalCtx) = sMorphaTent2->waterLevelMod + ((s16)self->waterLevel + sMorphaTent1->waterLevelMod);
    }
    self->actor.flags |= 0x200;
    self->actor.focus.pos = self->actor.world.pos;
    self->work[MO_TENT_VAR_TIMER]++;

    if (self->work[MO_CORE_DMG_FLASH_TIMER] != 0) {
        self->work[MO_CORE_DMG_FLASH_TIMER]--;
    }
    if (self->work[MO_TENT_INVINC_TIMER] != 0) {
        self->work[MO_TENT_INVINC_TIMER]--;
    }
    self->work[MO_TENT_MOVE_TIMER]++;

    for (i = 0; i < ARRAY_COUNT(self->timers); i++) {
        if (self->timers[i] != 0) {
            self->timers[i]--;
        }
    }

    BossMo_Core(self, globalCtx);
    Collider_UpdateCylinder(&self->actor, &self->coreCollider);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->coreCollider.base);
    if ((self->work[MO_TENT_ACTION_STATE] != MO_CORE_STUNNED) ||
        (self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx))) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->coreCollider.base);
    } else {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->coreCollider.base);
    }
    BossMo_UpdateEffects(self, globalCtx);
    if (player->actor.parent != NULL) {
        self->actor.flags &= ~1;
    }
    BossMo_Unknown();
}

void BossMo_UpdateTent(Actor* thisx, GlobalContext* globalCtx) {
    s16 i;
    s16 index;
    s32 pad;
    BossMo* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    f32 phi_f0;

    if ((self == sMorphaTent2) && (self->tent2KillTimer != 0)) {
        self->tent2KillTimer++;
        self->actor.draw = NULL;
        if (self->tent2KillTimer > 20) {
            Actor_Kill(&self->actor);
            func_800F89E8(&self->tentTipPos);
            sMorphaTent2 = NULL;
        }
        return;
    }

    SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, &self->tentPos[40], &self->tentTipPos, &self->actor.projectedW);
    osSyncPrintf("MO : Move mode = <%d>\n", self->work[MO_TENT_ACTION_STATE]);
    Math_ApproachS(&player->actor.shape.rot.x, 0, 5, 0x3E8);
    Math_ApproachS(&player->actor.shape.rot.z, 0, 5, 0x3E8);
    self->work[MO_TENT_VAR_TIMER]++;
    self->sfxTimer++;
    self->work[MO_TENT_MOVE_TIMER]++;
    self->widthIndex++;
    if (self->widthIndex >= 300) {
        self->widthIndex = 0;
    }
    self->pulsePhase -= 3000;
    index = self->widthIndex;
    self->tentWidth[index] = (Math_SinS(self->pulsePhase) * self->tentPulse) + (1.0f + self->tentPulse);
    for (i = 0; i < 41; i++) {
        if (self->work[MO_TENT_ACTION_STATE] >= MO_TENT_DEATH_START) {
            if (self->work[MO_TENT_ACTION_STATE] >= MO_TENT_DEATH_1) {
                if (self->work[MO_TENT_ACTION_STATE] == MO_TENT_DEATH_5) {
                    phi_f0 = (self->timers[0] != 0) ? sFlatWidth[i] : sDropletWidth[i];
                    Math_ApproachF(&self->tentScale[i].x, phi_f0, 0.5f, 100.0f);
                } else {
                    index = ((self->widthIndex + (i * 2)) + 220) % 300;
                    phi_f0 = self->tentWidth[index] + SQ(sTentWidth[i]);
                    Math_ApproachF(&self->tentScale[i].x, phi_f0, 0.5f, 0.3f);
                }
            } else {
                index = ((self->widthIndex - (i * 2)) + 300) % 300;
                phi_f0 = self->tentWidth[index] * sTentWidth[i];
                self->tentScale[i].x = phi_f0;
            }
        } else {
            index = ((self->widthIndex - (i * 2)) + 300) % 300;
            phi_f0 = self->tentWidth[index] * sTentWidth[i];
            Math_ApproachF(&self->tentScale[i].x, phi_f0, 0.5f, 0.3f);
        }
        phi_f0 = Math_SinS((self->work[MO_TENT_VAR_TIMER] * 12000.0f) + (i * 20000.0f));
        self->tentRipple[i].x = (1.0f * phi_f0) * self->tentRippleSize;
        self->tentScale[i].y = self->tentScale[i].z = self->tentScale[i].x;
        self->tentRipple[i].y = self->tentRipple[i].z = self->tentRipple[i].x;
    }

    Math_ApproachF(&self->tentRippleSize, 0.0f, 0.1f, 0.005f);
    Math_ApproachF(&self->tentPulse, 0.2f, 0.5f, 0.01f);
    self->actionFunc(self, globalCtx);
    for (i = 0; i < ARRAY_COUNT(self->timers); i++) {
        if (self->timers[i] != 0) {
            self->timers[i]--;
        }
    }
    Math_ApproachS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0xA, 0xC8);
    Actor_MoveForward(&self->actor);
    Math_ApproachF(&self->actor.speedXZ, 0.0, 1.0f, 0.02f);

    if (BossMo_NearLand(&self->actor.world.pos, 40)) {
        self->actor.world.pos = self->actor.prevPos;
    }
    if ((self->work[MO_TENT_VAR_TIMER] % 8) == 0) {
        f32 rippleScale;
        Vec3f pos = self->actor.world.pos;

        if (self->work[MO_TENT_ACTION_STATE] < MO_TENT_DEATH_START) {
            rippleScale = 400.0f;
        } else {
            rippleScale = 0.0;
            if (self->work[MO_TENT_ACTION_STATE] >= MO_TENT_DEATH_1) {
                pos = self->tentPos[38];
            }
        }
        BossMo_SpawnRipple(globalCtx->specialEffects, &pos, rippleScale, rippleScale * 3.0f, self->baseAlpha * 0.6666f,
                           300, MO_FX_BIG_RIPPLE);
    }
    if (self->baseBubblesTimer != 0) {
        Vec3f sp88;
        Vec3f sp7C;
        Vec3f bubblePos;
        Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
        s32 pad;

        self->baseBubblesTimer--;
        sp88.x = 0.0;
        sp88.y = 0.0;
        sp88.z = 100.0f;
        Matrix_RotateY(Rand_ZeroFloat(2.0f * M_PI), MTXMODE_NEW);
        Matrix_MultVec3f(&sp88, &sp7C);
        if ((self->work[MO_TENT_ACTION_STATE] >= MO_TENT_DEATH_1) &&
            (self->work[MO_TENT_ACTION_STATE] != MO_TENT_DEATH_3)) {
            i = 38;
        } else {
            i = 0;
            if (self->work[MO_TENT_ACTION_STATE] < MO_TENT_CUT) {
                func_80078914(&self->tentTipPos, NA_SE_EN_MOFER_CORE_ROLL - SFX_FLAG);
            }
        }
        bubblePos.x = self->tentPos[i].x + sp7C.x;
        bubblePos.y = (MO_WATER_LEVEL(globalCtx) - 40.0f) + Rand_ZeroFloat(20.0f);
        bubblePos.z = self->tentPos[i].z + sp7C.z;
        BossMo_SpawnBubble(globalCtx->specialEffects, &bubblePos, &zeroVec, &zeroVec, Rand_ZeroFloat(0.05f) + 0.2f,
                           &self->tentPos[i]);
    }

    if (self->work[MO_CORE_DMG_FLASH_TIMER] != 0) {
        self->work[MO_CORE_DMG_FLASH_TIMER]--;
    }
    if (self->work[MO_TENT_INVINC_TIMER] != 0) {
        self->work[MO_TENT_INVINC_TIMER]--;
    }
    if (self->linkHitTimer != 0) {
        self->linkHitTimer--;
    }

    if (self->drawActor) {
        BossMo_TentCollisionCheck(self, globalCtx);
        if ((self->work[MO_TENT_INVINC_TIMER] == 0) && (self->work[MO_TENT_ACTION_STATE] != MO_TENT_GRAB) &&
            (self->work[MO_TENT_ACTION_STATE] != MO_TENT_SHAKE)) {
            BossMo* otherTent = (BossMo*)self->otherTent;

            if (!HAS_LINK(otherTent) && (self->cutIndex == 0)) {
                CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->tentCollider.base);
                CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->tentCollider.base);
            }
        }
        if (self->cutIndex == 0) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->tentCollider.base);
        }
    }
    self->work[MO_TENT_BASE_TEX1_X]++;
    self->work[MO_TENT_BASE_TEX1_Y]++;
    self->work[MO_TENT_BASE_TEX2_X] -= 3;
    self->work[MO_TENT_BASE_TEX2_Y]++;
    Math_ApproachZeroF(&self->waterLevelMod, 0.1f, 0.2f);
}

void BossMo_UpdateTentColliders(BossMo* self, s32 item, ColliderJntSph* tentCollider, Vec3f* center) {
    tentCollider->elements[item].dim.worldSphere.center.x = center->x;
    tentCollider->elements[item].dim.worldSphere.center.y = center->y;
    tentCollider->elements[item].dim.worldSphere.center.z = center->z;
    if (self->work[MO_TENT_ACTION_STATE] <= MO_TENT_SHAKE) {
        tentCollider->elements[item].dim.worldSphere.radius =
            tentCollider->elements[item].dim.modelSphere.radius * tentCollider->elements[item].dim.scale;
    } else {
        tentCollider->elements[item].dim.worldSphere.radius = 0;
    }
}

static Gfx* sTentDLists[41] = {
    gMorphaTentaclePart0DL,  gMorphaTentaclePart1DL,  gMorphaTentaclePart2DL,  gMorphaTentaclePart3DL,
    gMorphaTentaclePart4DL,  gMorphaTentaclePart5DL,  gMorphaTentaclePart6DL,  gMorphaTentaclePart7DL,
    gMorphaTentaclePart8DL,  gMorphaTentaclePart9DL,  gMorphaTentaclePart10DL, gMorphaTentaclePart11DL,
    gMorphaTentaclePart12DL, gMorphaTentaclePart13DL, gMorphaTentaclePart14DL, gMorphaTentaclePart15DL,
    gMorphaTentaclePart16DL, gMorphaTentaclePart17DL, gMorphaTentaclePart18DL, gMorphaTentaclePart19DL,
    gMorphaTentaclePart20DL, gMorphaTentaclePart21DL, gMorphaTentaclePart22DL, gMorphaTentaclePart23DL,
    gMorphaTentaclePart24DL, gMorphaTentaclePart25DL, gMorphaTentaclePart26DL, gMorphaTentaclePart27DL,
    gMorphaTentaclePart28DL, gMorphaTentaclePart29DL, gMorphaTentaclePart30DL, gMorphaTentaclePart31DL,
    gMorphaTentaclePart32DL, gMorphaTentaclePart33DL, gMorphaTentaclePart34DL, gMorphaTentaclePart35DL,
    gMorphaTentaclePart36DL, gMorphaTentaclePart37DL, gMorphaTentaclePart38DL, gMorphaTentaclePart39DL,
    gMorphaTentaclePart40DL,
};

void BossMo_DrawTentacle(BossMo* self, GlobalContext* globalCtx) {
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    s16 i;
    s16 notCut;
    s16 index;
    Mtx* matrix = Graph_Alloc(globalCtx->state.gfxCtx, 41 * sizeof(Mtx));
    f32 phi_f20;
    f32 phi_f22;
    Vec3f sp110;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6366);

    sp110.x = globalCtx->envCtx.dirLight1.params.dir.x;
    sp110.y = globalCtx->envCtx.dirLight1.params.dir.y;
    sp110.z = globalCtx->envCtx.dirLight1.params.dir.z;

    Matrix_Push();

    gDPPipeSync(POLY_XLU_DISP++);

    gSPSegment(POLY_XLU_DISP++, 0x0C, matrix);

    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateY((self->actor.shape.rot.y / (f32)0x8000) * M_PI, MTXMODE_APPLY);
    Matrix_RotateX((self->actor.shape.rot.x / (f32)0x8000) * M_PI, MTXMODE_APPLY);

    BossMo_InitRand(1, 29100, 9786);

    for (i = 0; i < 41; i++, matrix++) {
        s32 pad;
        s32 pad2;

        if (i < 2) {
            Matrix_Push();
            Matrix_Scale(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
            notCut = true;
        } else {
            if (i >= 3) {
                Matrix_Translate(0.0f, self->tentStretch[i - 2].y, 0.0f, MTXMODE_APPLY);
                Matrix_RotateX((self->tentRot[i - 2].x / (f32)0x8000) * M_PI, MTXMODE_APPLY);
                Matrix_RotateZ((self->tentRot[i - 2].z / (f32)0x8000) * M_PI, MTXMODE_APPLY);
            }
            Matrix_Push();
            Matrix_Scale((self->tentScale[i - 2].x + self->tentRipple[i - 2].x) * self->actor.scale.x,
                         (self->tentScale[i - 2].y + self->tentRipple[i - 2].y) * self->actor.scale.y,
                         (self->tentScale[i - 2].z + self->tentRipple[i - 2].z) * self->actor.scale.z, MTXMODE_APPLY);
            notCut = true;
            if ((i >= self->cutIndex) && (self->meltIndex >= i)) {
                Matrix_Scale(self->cutScale, self->cutScale, self->cutScale, MTXMODE_APPLY);
                notCut = false;
            }
        }

        index = ((self->widthIndex - (i * 2)) + 300) % 300;
        if (self->work[MO_TENT_ACTION_STATE] < MO_TENT_DEATH_START) {
            Matrix_RotateY((((self->tentWidth[index] - 1.0f - self->tentPulse) * 1000) / 1000.0f) *
                               self->fwork[MO_TENT_MAX_STRETCH],
                           MTXMODE_APPLY);
        }
        Matrix_RotateX(M_PI / 2.0f, MTXMODE_APPLY);
        Matrix_ToMtx(matrix, "../z_boss_mo.c", 6452);

        gSPMatrix(POLY_XLU_DISP++, matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (i == 0) {
            func_8002EB44(&self->tentPos[i], &globalCtx->view.eye, &sp110, globalCtx->state.gfxCtx);
        }

        if (i == 0) {
            gSPDisplayList(POLY_XLU_DISP++, gMorphaTentacleBaseDL);
        } else {
            gSPDisplayList(POLY_XLU_DISP++, sTentDLists[i]);
        }

        Matrix_Pop();

        if ((i >= 2) && notCut && (i < (self->noBubbles + 38))) {
            if ((self->work[MO_TENT_ACTION_STATE] == MO_TENT_DEATH_1) ||
                (self->work[MO_TENT_ACTION_STATE] == MO_TENT_DEATH_2)) {
                phi_f20 = self->work[MO_TENT_MOVE_TIMER] & 3;
                phi_f20 *= -15.0f;
                phi_f22 = ((0.18f + BossMo_RandZeroOne() * 0.1f) * self->actor.scale.x) * 100.0f;
            } else {
                phi_f20 = 0.0f;
                phi_f22 = (((BossMo_RandZeroOne() * (0.08f)) + .08f) * self->actor.scale.x) * 100.0f;
            }
            Matrix_Push();
            Matrix_Translate(((BossMo_RandZeroOne() - 0.5f) * 10.0f) * self->tentScale[i - 2].x,
                             ((BossMo_RandZeroOne() - 0.5f) * 3.0f) + phi_f20,
                             ((BossMo_RandZeroOne() - 0.5f) * 10.0f) * self->tentScale[i - 2].z, MTXMODE_APPLY);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(phi_f22, phi_f22, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6511),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_OPA_DISP++, gMorphaBubbleDL);

            Matrix_Pop();
        }

        Matrix_MultVec3f(&zeroVec, &self->tentPos[i]);
        if (i == 36) {
            Matrix_MultVec3f(&zeroVec, &self->actor.focus.pos);
        }
        if (i == 24) {
            MtxF sp98;
            Vec3f sp8C = { -16.0f, 13.0f, 30.0f };
            Vec3s sp84;

            Matrix_Push();
            if (self->linkToLeft) {
                sp8C.x *= -1.0f;
            }
            Matrix_MultVec3f(&sp8C, &self->grabPosRot.pos);
            Matrix_RotateX(-35 * M_PI / 64, MTXMODE_APPLY);
            Matrix_Get(&sp98);
            Matrix_MtxFToYXZRotS(&sp98, &sp84, 0);
            self->grabPosRot.rot.x = sp84.x;
            self->grabPosRot.rot.y = sp84.y;
            self->grabPosRot.rot.z = sp84.z;
            Matrix_Pop();
        }
        if ((i < 38) && ((i & 1) == 1)) {
            BossMo_UpdateTentColliders(self, i / 2, &self->tentCollider, &self->tentPos[i]);
        }
    }

    Matrix_Pop();
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6571);
}

void BossMo_DrawWater(BossMo* self, GlobalContext* globalCtx) {
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6582);
    if (1) {}

    Matrix_Push();
    func_80093D84(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, MO_WATER_LEVEL(globalCtx), 0.0f, MTXMODE_NEW);

    gSPSegment(POLY_XLU_DISP++, 0x0D,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (s16)self->waterTex1x, (s16)self->waterTex1y, 32, 32, 1,
                                (s16)self->waterTex2x, (s16)self->waterTex2y, 32, 32));

    gDPPipeSync(POLY_XLU_DISP++);

    gDPSetPrimColor(POLY_XLU_DISP++, 0xFF, 0xFF, 200, 255, 255, (s8)sMorphaTent1->waterTexAlpha);

    gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, 80);

    Matrix_Scale(0.5f, 1.0f, 0.5f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6675),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_XLU_DISP++, gMorphaWaterDL);

    Matrix_Pop();
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6680);
}

void BossMo_DrawCore(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossMo* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6688);
    if (self->actor.world.pos.y > MO_WATER_LEVEL(globalCtx)) {
        BossMo_DrawWater(self, globalCtx);
    }
    if (self->drawActor) {
        func_80093D84(globalCtx->state.gfxCtx);

        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, sMorphaTent1->work[MO_TENT_VAR_TIMER] * 3,
                                    sMorphaTent1->work[MO_TENT_VAR_TIMER] * 3, 32, 32, 1,
                                    sMorphaTent1->work[MO_TENT_VAR_TIMER] * -3,
                                    sMorphaTent1->work[MO_TENT_VAR_TIMER] * -3, 32, 32));
        gSPSegment(POLY_XLU_DISP++, 0x09,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, sMorphaTent1->work[MO_TENT_VAR_TIMER] * 5, 0, 32, 32, 1,
                                    0, sMorphaTent1->work[MO_TENT_VAR_TIMER] * -10, 32, 32));

        Matrix_RotateX(self->work[MO_TENT_MOVE_TIMER] * 0.5f, MTXMODE_APPLY);
        Matrix_RotateZ(self->work[MO_TENT_MOVE_TIMER] * 0.8f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6735),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 255, (s8)self->baseAlpha);

        func_8002ED80(&self->actor, globalCtx, 0);

        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gMorphaCoreMembraneDL));

        gDPPipeSync(POLY_XLU_DISP++);

        gDPSetEnvColor(POLY_XLU_DISP++, 0, 220, 255, 128);
        if ((self->work[MO_CORE_DMG_FLASH_TIMER] % 2) != 0) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 60, 0, 255);
        } else {
            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 255, 255);
        }
        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gMorphaCoreNucleusDL));

        if ((self->work[MO_CORE_DRAW_SHADOW] && (self->actor.world.pos.y >= 0.0f)) ||
            (self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx))) {
            f32 groundLevel;
            s16 shadowAlpha;

            if (self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx)) {
                groundLevel = -280.0f;
                shadowAlpha = 100;
            } else {
                groundLevel = 0.0f;
                shadowAlpha = 160;
            }

            func_80094044(globalCtx->state.gfxCtx);

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, shadowAlpha);

            Matrix_Translate(self->actor.world.pos.x, groundLevel, self->actor.world.pos.z, MTXMODE_NEW);
            Matrix_Scale(0.23f, 1.0f, 0.23f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6820),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gCircleShadowDL));
        }
    }

    if (self->actor.world.pos.y < MO_WATER_LEVEL(globalCtx)) {
        BossMo_DrawWater(self, globalCtx);
    }

    if ((self->csCamera != 0) && (self->csState < MO_INTRO_REVEAL)) {
        f32 sp8C;
        f32 sp88;
        f32 sp84;
        f32 temp;
        f32 sp7C;
        f32 sp78;
        Vec3f sp6C;
        Vec3f sp60;

        func_80093D84(globalCtx->state.gfxCtx);

        gDPSetPrimColor(POLY_XLU_DISP++, 0xFF, 0xFF, 200, 255, 255, (s8)self->fwork[MO_CORE_INTRO_WATER_ALPHA]);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, (s8)self->fwork[MO_CORE_INTRO_WATER_ALPHA]);

        gSPSegment(POLY_XLU_DISP++, 0x0D,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (s16)sMorphaTent1->waterTex1x,
                                    (s16)sMorphaTent1->waterTex1y, 32, 32, 1, (s16)sMorphaTent1->waterTex2x,
                                    (s16)sMorphaTent1->waterTex2y, 32, 32));

        sp8C = self->cameraAt.x - self->cameraEye.x;
        sp88 = self->cameraAt.y - self->cameraEye.y;
        sp84 = self->cameraAt.z - self->cameraEye.z;
        temp = SQ(sp8C) + SQ(sp84);
        sp7C = Math_FAtan2F(sp8C, sp84);
        sp78 = -Math_FAtan2F(sp88, sqrtf(temp));

        sp6C.x = 0.0f;
        sp6C.y = 0.0f;
        sp6C.z = 10.0f;

        Matrix_RotateY(sp7C, MTXMODE_NEW);
        Matrix_RotateX(sp78, MTXMODE_APPLY);
        Matrix_MultVec3f(&sp6C, &sp60);
        sp8C = sp60.x + self->cameraEye.x;
        sp88 = sp60.y + self->cameraEye.y;
        sp84 = sp60.z + self->cameraEye.z;
        Matrix_Translate(sp8C, sp88, sp84, MTXMODE_NEW);
        Matrix_RotateY(sp7C, MTXMODE_APPLY);
        Matrix_RotateX(sp78, MTXMODE_APPLY);
        Matrix_RotateZ(-(0.01f * self->work[MO_TENT_VAR_TIMER]), MTXMODE_APPLY);
        Matrix_RotateZ(0.1f * self->work[MO_TENT_VAR_TIMER], MTXMODE_APPLY);
        Matrix_Scale(0.825f, 1.175f, 0.825f, MTXMODE_APPLY);
        Matrix_RotateZ(-(self->work[MO_TENT_VAR_TIMER] * 0.1f), MTXMODE_APPLY);
        Matrix_RotateX(M_PI / 2.0f, MTXMODE_APPLY);
        Matrix_Scale(0.05f, 1.0f, 0.05f, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6941),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(POLY_XLU_DISP++, gMorphaWaterDL);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6945);

    BossMo_DrawEffects(globalCtx->specialEffects, globalCtx);
}

void BossMo_DrawTent(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossMo* self = THIS;
    u16 scroll;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_mo.c", 6958);
    if (1) {}
    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, (s8)(self->baseAlpha * 1.5f));
    gDPSetEnvColor(POLY_OPA_DISP++, 150, 150, 150, 0);

    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, self->work[MO_TENT_BASE_TEX1_X],
                                self->work[MO_TENT_BASE_TEX1_Y], 32, 32, 1, self->work[MO_TENT_BASE_TEX2_X],
                                self->work[MO_TENT_BASE_TEX2_Y], 32, 32));
    gDPSetPrimColor(POLY_XLU_DISP++, 0xFF, 0xFF, 200, 255, 255, (s8)((self->baseAlpha * 12.0f) / 10.0f));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, (s8)self->baseAlpha);
    scroll = (s16)(Math_SinS(self->work[MO_TENT_VAR_TIMER] * 0xB00) * 30.0f) + 350;
    gSPTexture(POLY_XLU_DISP++, scroll, scroll, 0, G_TX_RENDERTILE, G_ON);

    if (self->drawActor) {
        BossMo_DrawTentacle(self, globalCtx);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_mo.c", 7023);
}

void BossMo_UpdateEffects(BossMo* self, GlobalContext* globalCtx) {
    BossMoEffect* effect = globalCtx->specialEffects;
    s16 i;
    Vec3f* targetPos;
    f32 dx;
    f32 dz;
    Vec3f bubbleSpeed = { 0.0f, 0.0f, 0.0f };
    Vec3f bubbleVel;

    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type != MO_FX_NONE) {
            effect->timer++;
            if (effect->stopTimer == 0) {
                effect->pos.x += effect->vel.x;
                effect->pos.y += effect->vel.y;
                effect->pos.z += effect->vel.z;
                effect->vel.x += effect->accel.x;
                effect->vel.y += effect->accel.y;
                effect->vel.z += effect->accel.z;
            } else {
                effect->stopTimer--;
            }
            if (effect->type <= MO_FX_BIG_RIPPLE) {
                if (self->csState >= MO_DEATH_START) {
                    effect->pos.y = MO_WATER_LEVEL(globalCtx);
                }
                Math_ApproachF(&effect->scale, effect->fwork[MO_FX_MAX_SIZE], 0.2f, effect->fwork[MO_FX_SPREAD_RATE]);
                if (effect->rippleMode == 0) {
                    effect->alpha += 15;
                    if (effect->alpha >= effect->maxAlpha) {
                        effect->alpha = effect->maxAlpha;
                        effect->rippleMode++;
                    }
                } else {
                    effect->alpha -= 5;
                    if (effect->alpha <= 0) {
                        effect->alpha = 0;
                        effect->type = MO_FX_NONE;
                    }
                }
            } else if (effect->type == MO_FX_BUBBLE) {
                if (effect->targetPos == NULL) {
                    if ((effect->accel.y > 0.0f) && (effect->pos.y >= MO_WATER_LEVEL(globalCtx))) {
                        effect->type = MO_FX_NONE;
                    } else {
                        if (effect->vel.y > 2.0f) {
                            effect->vel.y = 2.0f;
                        }
                        effect->alpha -= 20;
                        if (effect->alpha <= 0) {
                            effect->alpha = 0;
                            effect->type = MO_FX_NONE;
                        }
                    }
                } else {
                    if ((effect->timer % 4) == 0) {
                        targetPos = effect->targetPos;
                        dx = targetPos->x - effect->pos.x;
                        dz = targetPos->z - effect->pos.z;
                        bubbleSpeed.z = effect->fwork[MO_FX_SUCTION];
                        Matrix_RotateY(Math_FAtan2F(dx, dz), MTXMODE_NEW);
                        Matrix_MultVec3f(&bubbleSpeed, &bubbleVel);
                        effect->vel.x = bubbleVel.x;
                        effect->vel.z = bubbleVel.z;
                    }
                    Math_ApproachF(&effect->fwork[MO_FX_SUCTION], 5.0f, 1.0f, 0.5f);
                    if (effect->timer > 20) {
                        effect->alpha -= 30;
                        effect->accel.y = 1.5f;
                        if ((effect->alpha <= 0) || (effect->pos.y >= MO_WATER_LEVEL(globalCtx))) {
                            effect->alpha = 0;
                            effect->type = MO_FX_NONE;
                        }
                    } else {
                        effect->alpha += 30;
                        if (effect->alpha >= 255) {
                            effect->alpha = 255;
                        }
                    }
                }
            } else if ((effect->type == MO_FX_DROPLET) || (effect->type == MO_FX_SPLASH) ||
                       (effect->type == MO_FX_SPLASH_TRAIL) || (effect->type == MO_FX_WET_SPOT)) {
                f32 shimmer = (effect->timer & 6) ? 80.0f : 200.0f;

                Math_ApproachF(&effect->fwork[MO_FX_SHIMMER], shimmer, 1.0f, 80.0f);
                if (effect->type == MO_FX_WET_SPOT) {
                    Math_ApproachF(&effect->scale, effect->fwork[MO_FX_MAX_SCALE], 0.1f, 0.6f);
                    effect->alpha -= 15;
                    if (effect->alpha <= 0) {
                        effect->alpha = 0;
                        effect->type = MO_FX_NONE;
                    }
                } else {
                    effect->alpha = effect->fwork[MO_FX_SHIMMER];
                    if (effect->type == MO_FX_SPLASH_TRAIL) {
                        Math_ApproachF(&effect->scale, 0.0f, 1.0f, 0.02f);
                        if (effect->scale <= 0.0f) {
                            effect->type = MO_FX_NONE;
                        }
                    } else {
                        if (effect->type == MO_FX_SPLASH) {
                            Vec3f velocity = { 0.0f, 0.0f, 0.0f };

                            BossMo_SpawnDroplet(MO_FX_SPLASH_TRAIL, (BossMoEffect*)globalCtx->specialEffects,
                                                &effect->pos, &velocity, effect->scale);
                        }
                        if (effect->vel.y < -20.0f) {
                            effect->vel.y = -20.0f;
                            effect->accel.y = 0.0f;
                        }
                        if (effect->stopTimer == 0) {
                            if (effect->vel.y < -5.0f) {
                                Math_ApproachF(&effect->fwork[MO_FX_STRETCH], 5.0f, 0.1f, 0.15f);
                            }
                        } else if (effect->stopTimer == 1) {
                            effect->vel.x = Rand_CenteredFloat(3.0f);
                            effect->vel.z = Rand_CenteredFloat(3.0f);
                            effect->accel.y = -1.0f;
                        }
                        if ((effect->pos.y <= -280.0f) || ((1.0f >= effect->pos.y) && (effect->pos.y >= -20.0f) &&
                                                           BossMo_NearLand(&effect->pos, 0.0f))) {
                            effect->accel.y = 0.0f;
                            effect->vel.z = 0.0f;
                            effect->vel.y = 0.0f;
                            effect->vel.x = 0.0f;
                            if (effect->pos.y <= -280.0f) {
                                effect->pos.y = -280.0f;
                            } else {
                                effect->pos.y = 0.0f;
                            }
                            effect->type = MO_FX_WET_SPOT;
                            effect->alpha = 150;
                            effect->fwork[MO_FX_STRETCH] = (effect->scale * 15.0f) * 0.15f;
                        } else if (effect->pos.y <= MO_WATER_LEVEL(globalCtx)) {
                            Vec3f pos = effect->pos;

                            pos.y = MO_WATER_LEVEL(globalCtx);
                            if (effect->type == MO_FX_SPLASH) {
                                BossMo_SpawnRipple(globalCtx->specialEffects, &pos, 60.0f, 160.0f, 80, 290,
                                                   MO_FX_SMALL_RIPPLE);
                            } else {
                                BossMo_SpawnRipple(globalCtx->specialEffects, &pos, 40.0f, 110.0f, 80, 290,
                                                   MO_FX_SMALL_RIPPLE);
                            }
                            effect->type = MO_FX_NONE;
                        }
                    }
                }
            }
        }
    }
}

void BossMo_DrawEffects(BossMoEffect* effect, GlobalContext* globalCtx) {
    u8 flag = 0;
    s16 i;
    s32 pad;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    BossMoEffect* effectHead = effect;

    OPEN_DISPS(gfxCtx, "../z_boss_mo.c", 7264);
    Matrix_Push();

    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type == MO_FX_BIG_RIPPLE) {
            if (flag == 0) {
                func_80094BC4(gfxCtx);

                gDPSetEnvColor(POLY_XLU_DISP++, 155, 155, 255, 0);

                flag++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, effect->alpha);

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_Scale(effect->scale, 1.0f, effect->scale, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_mo.c", 7294),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_XLU_DISP++, gEffWaterRippleDL);
        }
    }

    effect = effectHead;
    flag = 0;
    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type == MO_FX_SMALL_RIPPLE) {
            if (flag == 0) {
                func_80093D84(globalCtx->state.gfxCtx);

                gDPSetEnvColor(POLY_XLU_DISP++, 155, 155, 255, 0);

                flag++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, effect->alpha);

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_Scale(effect->scale, 1.0f, effect->scale, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_mo.c", 7330),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_XLU_DISP++, gEffShockwaveDL);
        }
    }

    effect = effectHead;
    flag = 0;
    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (((effect->type == MO_FX_DROPLET) || (effect->type == MO_FX_SPLASH)) ||
            (effect->type == MO_FX_SPLASH_TRAIL)) {
            if (flag == 0) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);

                gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gDust1Tex));
                gSPDisplayList(POLY_XLU_DISP++, gMorphaDropletSetupDL);
                gDPSetEnvColor(POLY_XLU_DISP++, 250, 250, 255, 0);

                flag++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (s16)effect->fwork[MO_FX_SHIMMER], (s16)effect->fwork[MO_FX_SHIMMER],
                            255, effect->alpha);

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(effect->scale / effect->fwork[MO_FX_STRETCH], effect->fwork[MO_FX_STRETCH] * effect->scale,
                         1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_mo.c", 7373),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_XLU_DISP++, gMorphaDropletVtxDL);
        }
    }

    effect = effectHead;
    flag = 0;
    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type == MO_FX_WET_SPOT) {
            if (flag == 0) {
                func_80094044(gfxCtx);

                gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gDust1Tex));
                gDPSetEnvColor(POLY_XLU_DISP++, 250, 250, 255, 0);
                gSPDisplayList(POLY_XLU_DISP++, gMorphaDropletSetupDL);

                flag++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (s16)effect->fwork[MO_FX_SHIMMER], (s16)effect->fwork[MO_FX_SHIMMER],
                            0xFF, effect->alpha);

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_Scale(effect->scale, 1.0f, effect->scale, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_mo.c", 7441),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_XLU_DISP++, gMorphaWetSpotVtxDL);
        }
    }

    effect = effectHead;
    flag = 0;
    for (i = 0; i < ARRAY_COUNT(sEffects); i++, effect++) {
        if (effect->type == MO_FX_BUBBLE) {
            if (flag == 0) {
                func_80093D18(globalCtx->state.gfxCtx);

                gDPSetEnvColor(POLY_OPA_DISP++, 150, 150, 150, 0);

                flag++;
            }

            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, effect->alpha);

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(effect->scale, effect->scale, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_mo.c", 7476),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_OPA_DISP++, gMorphaBubbleDL);
        }
    }

    Matrix_Pop();
    CLOSE_DISPS(gfxCtx, "../z_boss_mo.c", 7482);
}

void BossMo_Unknown(void) {
    // Appears to be a test function for sound effects.
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    static u16 unkSfx[] = {
        NA_SE_PL_WALK_GROUND,
        NA_SE_PL_WALK_GROUND,
        NA_SE_PL_WALK_GROUND,
        NA_SE_PL_WALK_SAND,
        NA_SE_PL_WALK_CONCRETE,
        NA_SE_PL_WALK_DIRT,
        NA_SE_PL_WALK_WATER0,
        NA_SE_PL_WALK_WATER1,
        NA_SE_PL_WALK_WATER2,
        NA_SE_PL_WALK_MAGMA,
        NA_SE_PL_WALK_GRASS,
        NA_SE_PL_WALK_GLASS,
        NA_SE_PL_WALK_LADDER,
        NA_SE_PL_WALK_GLASS,
        NA_SE_PL_WALK_WALL,
        NA_SE_PL_WALK_HEAVYBOOTS,
        NA_SE_PL_WALK_ICE,
        NA_SE_PL_JUMP,
        NA_SE_PL_JUMP,
        NA_SE_PL_JUMP_SAND,
        NA_SE_PL_JUMP_CONCRETE,
        NA_SE_PL_JUMP_DIRT,
        NA_SE_PL_JUMP_WATER0,
        NA_SE_PL_JUMP_WATER1,
        NA_SE_PL_JUMP_WATER2,
        NA_SE_PL_JUMP_MAGMA,
        NA_SE_PL_JUMP_GRASS,
        NA_SE_PL_JUMP_GLASS,
        NA_SE_PL_JUMP_LADDER,
        NA_SE_PL_JUMP_GLASS,
        NA_SE_PL_JUMP_HEAVYBOOTS,
        NA_SE_PL_JUMP_ICE,
        NA_SE_PL_LAND,
        NA_SE_PL_LAND,
        NA_SE_PL_LAND_SAND,
        NA_SE_PL_LAND_CONCRETE,
        NA_SE_PL_LAND_DIRT,
        NA_SE_PL_LAND_WATER0,
        NA_SE_PL_LAND_WATER1,
        NA_SE_PL_LAND_WATER2,
        NA_SE_PL_LAND_MAGMA,
        NA_SE_PL_LAND_GRASS,
        NA_SE_PL_LAND_GLASS,
        NA_SE_PL_LAND_LADDER,
        NA_SE_PL_LAND_GLASS,
        NA_SE_PL_LAND_HEAVYBOOTS,
        NA_SE_PL_LAND_ICE,
        NA_SE_PL_SLIPDOWN,
        NA_SE_PL_CLIMB_CLIFF,
        NA_SE_PL_CLIMB_CLIFF,
        NA_SE_PL_SIT_ON_HORSE,
        NA_SE_PL_GET_OFF_HORSE,
        NA_SE_PL_TAKE_OUT_SHIELD,
        NA_SE_PL_TAKE_OUT_SHIELD,
        NA_SE_PL_TAKE_OUT_SHIELD,
        NA_SE_PL_TAKE_OUT_SHIELD,
        NA_SE_PL_TAKE_OUT_SHIELD,
        NA_SE_PL_TAKE_OUT_SHIELD,
        NA_SE_PL_CHANGE_ARMS,
        NA_SE_PL_CHANGE_ARMS,
        NA_SE_PL_CHANGE_ARMS,
        NA_SE_PL_CHANGE_ARMS,
        NA_SE_PL_CHANGE_ARMS,
        NA_SE_PL_CHANGE_ARMS,
        NA_SE_PL_CATCH_BOOMERANG,
        NA_SE_EV_DIVE_INTO_WATER,
        NA_SE_EV_JUMP_OUT_WATER,
        NA_SE_PL_SWIM,
        NA_SE_PL_THROW,
        NA_SE_PL_BODY_BOUND,
        NA_SE_PL_ROLL,
        NA_SE_PL_SKIP,
        NA_SE_PL_BODY_HIT,
        NA_SE_PL_DAMAGE,
        NA_SE_PL_SLIP,
        NA_SE_PL_SLIP,
        NA_SE_PL_SLIP,
        NA_SE_PL_SLIP_SAND,
        NA_SE_PL_SLIP_CONCRETE,
        NA_SE_PL_SLIP_DIRT,
        NA_SE_PL_SLIP_WATER0,
        NA_SE_PL_SLIP_WATER1,
        NA_SE_PL_SLIP_WATER2,
        NA_SE_PL_SLIP_MAGMA,
        NA_SE_PL_SLIP_GRASS,
        NA_SE_PL_SLIP_GLASS,
        NA_SE_PL_SLIP_LADDER,
        NA_SE_PL_SLIP_GLASS,
        NA_SE_PL_SLIP_HEAVYBOOTS,
        NA_SE_PL_SLIP_ICE,
        NA_SE_PL_BOUND,
        NA_SE_PL_BOUND,
        NA_SE_PL_BOUND_SAND,
        NA_SE_PL_BOUND_CONCRETE,
        NA_SE_PL_BOUND_DIRT,
        NA_SE_PL_BOUND_WATER0,
        NA_SE_PL_BOUND_WATER1,
        NA_SE_PL_BOUND_WATER2,
        NA_SE_PL_BOUND_MAGMA,
        NA_SE_PL_BOUND_GRASS,
        NA_SE_PL_BOUND_WOOD,
        NA_SE_PL_BOUND_LADDER,
        NA_SE_PL_BOUND_WOOD,
        NA_SE_PL_BOUND_HEAVYBOOTS,
        NA_SE_PL_BOUND_ICE,
        NA_SE_PL_FACE_UP,
        NA_SE_PL_DIVE_BUBBLE,
        NA_SE_PL_MOVE_BUBBLE,
        NA_SE_PL_METALEFFECT_KID,
        NA_SE_PL_METALEFFECT_ADULT,
        NA_SE_PL_SPARK - SFX_FLAG,
        NA_SE_IT_SWORD_IMPACT,
        NA_SE_IT_SWORD_SWING,
        NA_SE_IT_SWORD_PUTAWAY,
        NA_SE_IT_SWORD_PICKOUT,
        NA_SE_IT_ARROW_SHOT,
        NA_SE_IT_BOOMERANG_THROW,
        NA_SE_IT_SHIELD_BOUND,
        NA_SE_IT_SHIELD_BOUND,
        NA_SE_IT_BOW_DRAW,
        NA_SE_IT_SHIELD_REFLECT_SW,
        NA_SE_IT_ARROW_STICK_HRAD,
        NA_SE_IT_HAMMER_HIT,
        NA_SE_IT_HOOKSHOT_CHAIN - SFX_FLAG,
        NA_SE_IT_SHIELD_REFLECT_MG,
        NA_SE_IT_BOMB_IGNIT - SFX_FLAG,
        NA_SE_IT_BOMB_EXPLOSION,
        NA_SE_IT_BOMB_UNEXPLOSION,
        NA_SE_IT_BOOMERANG_FLY - SFX_FLAG,
        NA_SE_IT_SWORD_STRIKE,
        NA_SE_IT_HAMMER_SWING,
        NA_SE_IT_HOOKSHOT_REFLECT,
        NA_SE_IT_ARROW_STICK_CRE,
        NA_SE_IT_ARROW_STICK_CRE,
        NA_SE_IT_ARROW_STICK_OBJ,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_IT_SWORD_SWING_HARD,
        NA_SE_IT_WALL_HIT_HARD,
        NA_SE_IT_WALL_HIT_SOFT,
        NA_SE_IT_WALL_HIT_SOFT,
        NA_SE_IT_STONE_HIT,
        NA_SE_IT_WOODSTICK_BROKEN,
        NA_SE_IT_LASH,
        NA_SE_IT_SHIELD_POSTURE,
        NA_SE_IT_SLING_SHOT,
        NA_SE_IT_SLING_DRAW,
        NA_SE_IT_SWORD_CHARGE - SFX_FLAG,
        NA_SE_IT_ROLLING_CUT,
        NA_SE_IT_SWORD_STRIKE_HARD,
        NA_SE_IT_SLING_REFLECT,
        NA_SE_IT_SHIELD_REMOVE,
        NA_SE_IT_HOOKSHOT_READY,
        NA_SE_IT_HOOKSHOT_RECEIVE,
        NA_SE_IT_HOOKSHOT_STICK_OBJ,
        NA_SE_IT_SWORD_REFLECT_MG,
        NA_SE_IT_DEKU,
        NA_SE_IT_BOW_FLICK,
        NA_SE_IT_BOW_FLICK,
        NA_SE_IT_BOW_FLICK,
        NA_SE_IT_BOMBCHU_MOVE,
        NA_SE_IT_SHIELD_CHARGE_LV1,
        NA_SE_IT_SHIELD_CHARGE_LV2,
        NA_SE_IT_SHIELD_CHARGE_LV3,
        NA_SE_IT_SLING_FLICK,
        NA_SE_IT_SWORD_STICK_STN,
        NA_SE_IT_REFLECTION_WOOD,
        NA_SE_IT_SHIELD_REFLECT_MG2,
        NA_SE_IT_MAGIC_ARROW_SHOT,
        NA_SE_IT_EXPLOSION_FRAME,
        NA_SE_IT_EXPLOSION_ICE,
        NA_SE_IT_YOBI19 - SFX_FLAG,
        NA_SE_FISHING_REEL_SLOW2 - SFX_FLAG,
        NA_SE_OC_DOOR_OPEN,
        NA_SE_EV_DOOR_CLOSE,
        NA_SE_EV_EXPLOSION,
        NA_SE_EV_HORSE_WALK,
        NA_SE_EV_HORSE_RUN,
        NA_SE_EV_HORSE_NEIGH,
        NA_SE_EV_RIVER_STREAM - SFX_FLAG,
        NA_SE_EV_WATER_WALL_BIG - SFX_FLAG,
        NA_SE_EV_DIVE_WATER,
        NA_SE_EV_OUT_OF_WATER,
        NA_SE_EV_ROCK_SLIDE - SFX_FLAG,
        NA_SE_EV_MAGMA_LEVEL - SFX_FLAG,
        NA_SE_EV_MAGMA_LEVEL - SFX_FLAG,
        NA_SE_EV_BRIDGE_OPEN - SFX_FLAG,
        NA_SE_EV_BRIDGE_CLOSE - SFX_FLAG,
        NA_SE_EV_BRIDGE_OPEN_STOP,
        NA_SE_EV_BRIDGE_CLOSE_STOP,
        NA_SE_EV_WALL_BROKEN,
        NA_SE_EV_CHICKEN_CRY_N,
        NA_SE_EV_CHICKEN_CRY_A,
        NA_SE_EV_CHICKEN_CRY_M,
        NA_SE_EV_SLIDE_DOOR_OPEN,
        NA_SE_EV_FOOT_SWITCH,
        NA_SE_EV_HORSE_GROAN,
        NA_SE_EV_BOMB_DROP_WATER,
        NA_SE_EV_BOMB_DROP_WATER,
        NA_SE_EV_HORSE_JUMP,
        NA_SE_EV_HORSE_LAND,
        NA_SE_EV_HORSE_SLIP,
        NA_SE_EV_FAIRY_DASH,
        NA_SE_EV_SLIDE_DOOR_CLOSE,
        NA_SE_EV_STONE_BOUND,
        NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG,
        NA_SE_EV_TBOX_UNLOCK,
        NA_SE_EV_TBOX_OPEN,
        NA_SE_SY_TIMER - SFX_FLAG,
        NA_SE_EV_FLAME_IGNITION,
        NA_SE_EV_SPEAR_HIT,
        NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG,
        NA_SE_EV_WARP_HOLE - SFX_FLAG,
        NA_SE_EV_LINK_WARP,
        NA_SE_EV_PILLAR_SINK - SFX_FLAG,
        NA_SE_EV_WATER_WALL - SFX_FLAG,
        NA_SE_EV_RIVER_STREAM_S - SFX_FLAG,
        NA_SE_EV_RIVER_STREAM_F - SFX_FLAG,
        NA_SE_EV_HORSE_LAND2,
        NA_SE_EV_HORSE_SANDDUST,
        NA_SE_EV_BOMB_BOUND,
        NA_SE_EV_BOMB_BOUND,
        NA_SE_EV_WATERDROP - SFX_FLAG,
        NA_SE_EV_TORCH - SFX_FLAG,
        NA_SE_EV_MAGMA_LEVEL_M - SFX_FLAG,
        NA_SE_EV_FIRE_PILLAR - SFX_FLAG,
        NA_SE_EV_FIRE_PLATE - SFX_FLAG,
        NA_SE_EV_BLOCK_BOUND,
        NA_SE_EV_METALDOOR_SLIDE - SFX_FLAG,
        NA_SE_EV_METALDOOR_STOP,
        NA_SE_EV_BLOCK_SHAKE,
        NA_SE_EV_BOX_BREAK,
        NA_SE_EV_HAMMER_SWITCH,
        NA_SE_EV_MAGMA_LEVEL_L - SFX_FLAG,
        NA_SE_EV_SPEAR_FENCE,
        NA_SE_EV_GANON_HORSE_NEIGH,
        NA_SE_EV_GANON_HORSE_GROAN,
        NA_SE_EV_FANTOM_WARP_S,
        NA_SE_EV_FANTOM_WARP_L - SFX_FLAG,
        NA_SE_EV_FOUNTAIN - SFX_FLAG,
        NA_SE_EV_KID_HORSE_WALK,
        NA_SE_EV_KID_HORSE_RUN,
        NA_SE_EV_KID_HORSE_NEIGH,
        NA_SE_EV_KID_HORSE_GROAN,
        NA_SE_EV_WHITE_OUT,
        NA_SE_EV_LIGHT_GATHER - SFX_FLAG,
        NA_SE_EV_TREE_CUT,
        NA_SE_EV_WATERDROP,
        NA_SE_EV_TORCH,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_EN_DODO_J_WALK,
        NA_SE_EN_DODO_J_CRY,
        NA_SE_EN_DODO_J_FIRE - SFX_FLAG,
        NA_SE_EN_DODO_J_DAMAGE,
        NA_SE_EN_DODO_J_DEAD,
        NA_SE_EN_DODO_K_CRY,
        NA_SE_EN_DODO_K_DAMAGE,
        NA_SE_EN_DODO_K_DEAD,
        NA_SE_EN_DODO_K_WALK,
        NA_SE_EN_DODO_K_FIRE - SFX_FLAG,
        NA_SE_EN_GOMA_WALK,
        NA_SE_EN_GOMA_HIGH,
        NA_SE_EN_GOMA_CLIM,
        NA_SE_EN_GOMA_DOWN,
        NA_SE_EN_GOMA_CRY1,
        NA_SE_EN_GOMA_CRY2,
        NA_SE_EN_GOMA_DAM1,
        NA_SE_EN_GOMA_DAM2,
        NA_SE_EN_GOMA_DEAD,
        NA_SE_EN_GOMA_UNARI,
        NA_SE_EN_GOMA_EGG1,
        NA_SE_EN_GOMA_EGG2,
        NA_SE_EN_GOMA_JR_WALK,
        NA_SE_EN_GOMA_JR_CRY,
        NA_SE_EN_GOMA_JR_DAM1,
        NA_SE_EN_GOMA_JR_DAM2,
        NA_SE_EN_GOMA_JR_DEAD,
        NA_SE_EN_GOMA_DEMO_EYE,
        NA_SE_EN_GOMA_LAST - SFX_FLAG,
        NA_SE_EN_GOMA_UNARI2,
        NA_SE_EN_DODO_M_CRY,
        NA_SE_EN_DODO_M_DEAD,
        NA_SE_EN_DODO_M_MOVE,
        NA_SE_EN_DODO_M_DOWN,
        NA_SE_EN_DODO_M_UP,
        NA_SE_EN_DODO_M_GND,
        NA_SE_EN_RIZA_CRY,
        NA_SE_EN_RIZA_ATTACK,
        NA_SE_EN_RIZA_DAMAGE,
        NA_SE_EN_RIZA_WARAU,
        NA_SE_EN_RIZA_DEAD,
        NA_SE_EN_RIZA_WALK,
        NA_SE_EN_RIZA_JUMP,
        NA_SE_EN_RIZA_ONGND,
        NA_SE_EN_RIZA_DOWN,
        NA_SE_EN_STAL_WARAU,
        NA_SE_EN_STAL_SAKEBI,
        NA_SE_EN_STAL_DAMAGE,
        NA_SE_EN_STAL_DEAD,
        NA_SE_EN_STAL_JUMP,
        NA_SE_EN_STAL_WALK,
        NA_SE_EN_RIZA_DOWN,
        NA_SE_EN_FFLY_ATTACK,
        NA_SE_EN_FFLY_FLY,
        NA_SE_EN_FFLY_DEAD,
        NA_SE_EN_AMOS_WALK,
        NA_SE_EN_AMOS_WAVE,
        NA_SE_EN_AMOS_DEAD,
        NA_SE_EN_AMOS_DAMAGE,
        NA_SE_EN_AMOS_VOICE,
        NA_SE_EN_DODO_K_COLI,
        NA_SE_EN_DODO_K_COLI2,
        NA_SE_EN_DODO_K_ROLL - SFX_FLAG,
        NA_SE_EN_DODO_K_BREATH - SFX_FLAG,
        NA_SE_EN_DODO_K_DRINK,
        NA_SE_EN_DODO_K_DOWN - SFX_FLAG,
        NA_SE_EN_DODO_K_OTAKEBI,
        NA_SE_EN_DODO_K_END,
        NA_SE_EN_DODO_K_LAST - SFX_FLAG,
        NA_SE_EN_DODO_K_LAVA,
        NA_SE_EN_DODO_J_BREATH - SFX_FLAG,
        NA_SE_EN_DODO_J_TAIL,
        NA_SE_EN_RIZA_DOWN,
        NA_SE_EN_DEKU_MOUTH,
        NA_SE_EN_DEKU_ATTACK,
        NA_SE_EN_DEKU_DAMAGE,
        NA_SE_EN_DEKU_DEAD,
        NA_SE_EN_DEKU_JR_MOUTH,
        NA_SE_EN_DEKU_JR_ATTACK,
        NA_SE_EN_DEKU_JR_DEAD,
        NA_SE_EN_DODO_M_GND,
        NA_SE_EN_TAIL_FLY - SFX_FLAG,
        NA_SE_EN_TAIL_CRY,
        NA_SE_EN_STALTU_DOWN,
        NA_SE_EN_STALTU_UP,
        NA_SE_EN_STALTU_LAUGH,
        NA_SE_EN_STALTU_DAMAGE,
        NA_SE_EN_STAL_JUMP,
        NA_SE_EN_DODO_M_GND,
        NA_SE_EN_TEKU_DEAD,
        NA_SE_EN_TEKU_WALK,
        NA_SE_EN_PO_KANTERA,
        NA_SE_EN_PO_FLY - SFX_FLAG,
        NA_SE_EN_PO_AWAY - SFX_FLAG,
        NA_SE_EN_PO_APPEAR,
        NA_SE_EN_PO_DISAPPEAR,
        NA_SE_EN_PO_DAMAGE,
        NA_SE_EN_PO_DEAD,
        NA_SE_EN_PO_DEAD2,
        NA_SE_EN_EXTINCT,
        NA_SE_EN_NUTS_UP,
        NA_SE_EN_NUTS_DOWN,
        NA_SE_EN_NUTS_THROW,
        NA_SE_EN_NUTS_WALK,
        NA_SE_EN_NUTS_DAMAGE,
        NA_SE_EN_NUTS_DEAD,
        NA_SE_EN_STALTU_ROLL,
        NA_SE_EN_STALWALL_DEAD,
        NA_SE_EN_TEKU_DAMAGE,
        NA_SE_EN_FALL_AIM,
        NA_SE_EN_FALL_UP,
        NA_SE_EN_FALL_CATCH,
        NA_SE_EN_FALL_LAND,
        NA_SE_EN_FALL_WALK,
        NA_SE_EN_FALL_DAMAGE,
        NA_SE_EN_BIRI_FLY,
        NA_SE_EN_BIRI_JUMP,
        NA_SE_EN_BIRI_SPARK - SFX_FLAG,
        NA_SE_EN_FANTOM_TRANSFORM,
        NA_SE_EN_FANTOM_TRANSFORM,
        NA_SE_EN_FANTOM_THUNDER,
        NA_SE_EN_FANTOM_SPARK,
        NA_SE_EN_FANTOM_FLOAT - SFX_FLAG,
        NA_SE_EN_FANTOM_MASIC1,
        NA_SE_EN_FANTOM_MASIC2,
        NA_SE_EN_FANTOM_FIRE - SFX_FLAG,
        NA_SE_EN_FANTOM_HIT_THUNDER,
        NA_SE_EN_FANTOM_ATTACK,
        NA_SE_EN_FANTOM_STICK,
        NA_SE_EN_FANTOM_EYE,
        NA_SE_EN_FANTOM_LAST,
        NA_SE_EN_FANTOM_THUNDER_GND,
        NA_SE_EN_FANTOM_DAMAGE,
        NA_SE_EN_FANTOM_DEAD,
        NA_SE_EN_FANTOM_LAUGH,
        NA_SE_EN_FANTOM_DAMAGE2,
        NA_SE_EN_FANTOM_VOICE,
        NA_SE_EN_MORIBLIN_WALK,
        NA_SE_EN_MORIBLIN_SLIDE,
        NA_SE_EN_MORIBLIN_ATTACK,
        NA_SE_EN_MORIBLIN_VOICE,
        NA_SE_EN_MORIBLIN_SPEAR_AT,
        NA_SE_EN_MORIBLIN_SPEAR_NORM,
        NA_SE_EN_MORIBLIN_DEAD,
        NA_SE_EN_NUTS_THROW,
        NA_SE_EN_OCTAROCK_FLOAT,
        NA_SE_EN_OCTAROCK_JUMP,
        NA_SE_EN_OCTAROCK_LAND,
        NA_SE_EN_OCTAROCK_SINK,
        NA_SE_EN_OCTAROCK_BUBLE,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_SY_WIN_OPEN,
        NA_SE_SY_WIN_CLOSE,
        NA_SE_SY_CORRECT_CHIME,
        NA_SE_SY_GET_RUPY,
        NA_SE_SY_MESSAGE_WOMAN,
        NA_SE_SY_MESSAGE_MAN,
        NA_SE_SY_ERROR,
        NA_SE_SY_TRE_BOX_APPEAR,
        NA_SE_SY_TRE_BOX_APPEAR,
        NA_SE_SY_DECIDE,
        NA_SE_SY_CURSOR,
        NA_SE_SY_CANCEL,
        NA_SE_SY_HP_RECOVER,
        NA_SE_SY_ATTENTION_ON,
        NA_SE_SY_ATTENTION_ON,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_SY_LOCK_ON,
        NA_SE_SY_LOCK_ON,
        NA_SE_SY_LOCK_OFF,
        NA_SE_SY_LOCK_ON_HUMAN,
        NA_SE_SY_CAMERA_ZOOM_UP,
        NA_SE_SY_CAMERA_ZOOM_DOWN,
        NA_SE_SY_ATTENTION_ON_OLD,
        NA_SE_SY_ATTENTION_URGENCY,
        NA_SE_SY_MESSAGE_PASS,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_SY_PIECE_OF_HEART,
        NA_SE_SY_GET_ITEM,
        NA_SE_SY_WIN_SCROLL_LEFT,
        NA_SE_SY_WIN_SCROLL_RIGHT,
        NA_SE_SY_OCARINA_ERROR,
        NA_SE_SY_CAMERA_ZOOM_UP_2,
        NA_SE_SY_CAMERA_ZOOM_DOWN_2,
        NA_SE_SY_GLASSMODE_ON,
        NA_SE_SY_GLASSMODE_OFF,
        NA_SE_SY_ATTENTION_ON,
        NA_SE_SY_ATTENTION_URGENCY,
        NA_SE_OC_OCARINA,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_LAND - SFX_FLAG,
        NA_SE_VO_LI_SWORD_N,
        NA_SE_VO_LI_SWORD_N,
        NA_SE_VO_LI_SWORD_N,
        NA_SE_VO_LI_SWORD_N,
        NA_SE_VO_LI_SWORD_N,
        NA_SE_VO_LI_SWORD_N,
        NA_SE_VO_LI_SWORD_N,
        NA_SE_VO_LI_SWORD_L,
        NA_SE_VO_LI_SWORD_L,
        NA_SE_VO_LI_MAGIC_ATTACK,
        NA_SE_VO_LI_LASH,
        NA_SE_VO_LI_HANG,
        NA_SE_VO_LI_AUTO_JUMP,
        NA_SE_VO_LI_CLIMB_END,
        NA_SE_VO_LI_CLIMB_END,
        NA_SE_VO_LI_CLIMB_END,
        NA_SE_VO_LI_CLIMB_END,
        NA_SE_VO_LI_DAMAGE_S,
        NA_SE_VO_LI_DAMAGE_S,
        NA_SE_VO_LI_FALL_L,
        NA_SE_VO_LI_FALL_S,
        NA_SE_VO_LI_FALL_L,
        NA_SE_VO_LI_FALL_L,
        NA_SE_VO_LI_BREATH_REST,
        NA_SE_VO_LI_BREATH_REST,
        NA_SE_VO_LI_DOWN,
        NA_SE_VO_LI_TAKEN_AWAY,
        NA_SE_VO_LI_HELD,
        NA_SE_VO_NAVY_HELLO,
        NA_SE_VO_NAVY_HEAR,
        NA_SE_VO_NAVY_ENEMY,
        NA_SE_VO_NAVY_HELLO,
        NA_SE_VO_NAVY_HEAR,
        NA_SE_VO_NAVY_ENEMY,
        NA_SE_VO_TA_SLEEP,
        NA_SE_EN_VALVAISA_APPEAR - SFX_FLAG,
        NA_SE_EN_VALVAISA_ROAR,
        NA_SE_EN_VALVAISA_MAHI1,
        NA_SE_EN_VALVAISA_MAHI2,
        NA_SE_EN_VALVAISA_KNOCKOUT,
        NA_SE_EN_VALVAISA_DAMAGE1,
        NA_SE_EN_VALVAISA_DAMAGE2,
        NA_SE_EN_VALVAISA_ROCK,
        NA_SE_EN_VALVAISA_LAND,
        NA_SE_EN_VALVAISA_DEAD,
        NA_SE_EN_VALVAISA_BURN - SFX_FLAG,
        NA_SE_EN_VALVAISA_FIRE - SFX_FLAG,
        NA_SE_EN_VALVAISA_LAND2,
        NA_SE_EN_MONBLIN_HAM_LAND,
        NA_SE_EN_MONBLIN_HAM_DOWN,
        NA_SE_EN_MONBLIN_HAM_UP,
        NA_SE_EN_REDEAD_CRY,
        NA_SE_EN_REDEAD_AIM,
        NA_SE_EN_REDEAD_DAMAGE,
        NA_SE_EN_RIZA_DOWN,
        NA_SE_EN_REDEAD_DEAD,
        NA_SE_EN_REDEAD_ATTACK,
        NA_SE_EN_PO_LAUGH,
        NA_SE_EN_PO_CRY,
        NA_SE_EN_PO_ROLL,
        NA_SE_EN_PO_LAUGH2,
        NA_SE_EN_MOFER_APPEAR - SFX_FLAG,
        NA_SE_EN_MOFER_ATTACK - SFX_FLAG,
        NA_SE_EN_MOFER_WAVE,
        NA_SE_EN_MOFER_CATCH,
        NA_SE_EN_MOFER_CORE_DAMAGE,
        NA_SE_EN_MOFER_CUT,
        NA_SE_EN_MOFER_MOVE_DEMO - SFX_FLAG,
        NA_SE_EN_MOFER_BUBLE_DEMO,
        NA_SE_EN_MOFER_CORE_JUMP,
        NA_SE_EN_GOLON_WAKE_UP,
        NA_SE_EN_GOLON_SIT_DOWN,
        NA_SE_EN_DODO_M_GND,
        NA_SE_EN_DEADHAND_BITE,
        NA_SE_EN_DEADHAND_WALK,
        NA_SE_EN_DEADHAND_GRIP,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
        NA_SE_PL_WALK_GROUND - SFX_FLAG,
    };

    if (BREG(32) != 0) {
        BREG(32)--;
        Audio_QueueSeqCmd(0x100100FF);
        func_80078914(&zeroVec, unkSfx[BREG(33)]);
    }
    if (BREG(34) != 0) {
        BREG(34) = 0;
        Audio_QueueSeqCmd((u16)BREG(35));
    }
}
