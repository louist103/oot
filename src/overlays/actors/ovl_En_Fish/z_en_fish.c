/*
 * File: z_en_fish.c
 * Overlay: ovl_En_Fish
 * Description: Fish
 */

#include "z_en_fish.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "vt.h"

#define FLAGS 0x00000000

#define THIS ((EnFish*)thisx)

void EnFish_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFish_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFish_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFish_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnFish_Respawning_SetupSlowDown(EnFish* self);
void EnFish_Respawning_SlowDown(EnFish* self, GlobalContext* globalCtx);
void EnFish_Respawning_SetupFollowChild(EnFish* self);
void EnFish_Respawning_FollowChild(EnFish* self, GlobalContext* globalCtx);
void EnFish_Respawning_SetupFleePlayer(EnFish* self);
void EnFish_Respawning_FleePlayer(EnFish* self, GlobalContext* globalCtx);
void EnFish_Respawning_SetupApproachPlayer(EnFish* self);
void EnFish_Respawning_ApproachPlayer(EnFish* self, GlobalContext* globalCtx);
void EnFish_Dropped_SetupFall(EnFish* self);
void EnFish_Dropped_Fall(EnFish* self, GlobalContext* globalCtx);
void EnFish_Dropped_SetupFlopOnGround(EnFish* self);
void EnFish_Dropped_FlopOnGround(EnFish* self, GlobalContext* globalCtx);
void EnFish_Dropped_SetupSwimAway(EnFish* self);
void EnFish_Dropped_SwimAway(EnFish* self, GlobalContext* globalCtx);
void EnFish_Unique_SetupSwimIdle(EnFish* self);
void EnFish_Unique_SwimIdle(EnFish* self, GlobalContext* globalCtx);

// Used in the cutscene functions
static Actor* D_80A17010 = NULL;
static f32 D_80A17014 = 0.0f;
static f32 D_80A17018 = 0.0f;

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 5 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

const ActorInit En_Fish_InitVars = {
    ACTOR_EN_FISH,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnFish),
    (ActorFunc)EnFish_Init,
    (ActorFunc)EnFish_Destroy,
    (ActorFunc)EnFish_Update,
    (ActorFunc)EnFish_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 900, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 40, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 700, ICHAIN_STOP),
};

f32 EnFish_XZDistanceSquared(Vec3f* v1, Vec3f* v2) {
    return SQ(v1->x - v2->x) + SQ(v1->z - v2->z);
}

void EnFish_SetInWaterAnimation(EnFish* self) {
    Animation_Change(&self->skelAnime, &gFishInWaterAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gFishInWaterAnim),
                     ANIMMODE_LOOP_INTERP, 2.0f);
}

void EnFish_SetOutOfWaterAnimation(EnFish* self) {
    Animation_Change(&self->skelAnime, &gFishOutOfWaterAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gFishOutOfWaterAnim),
                     ANIMMODE_LOOP_INTERP, 2.0f);
}

void EnFish_BeginRespawn(EnFish* self) {
    self->respawnTimer = 400;
    Actor_SetScale(&self->actor, 0.001f);
    self->actor.draw = NULL;
}

void EnFish_SetCutsceneData(EnFish* self) {
    Actor* thisx = &self->actor;

    if (D_80A17010 == NULL) {
        D_80A17010 = thisx;
        Actor_SetScale(thisx, 0.01f);
        thisx->draw = EnFish_Draw;
        thisx->shape.rot.x = 0;
        thisx->shape.rot.y = -0x6410;
        thisx->shape.rot.z = 0x4000;
        thisx->shape.yOffset = 600.0f;
        D_80A17014 = 10.0f;
        D_80A17018 = 0.0f;
        thisx->flags |= 0x10;
        EnFish_SetOutOfWaterAnimation(self);
    }
}

void EnFish_ClearCutsceneData(EnFish* self) {
    D_80A17010 = NULL;
    D_80A17014 = 0.0f;
    D_80A17018 = 0.0f;
}

void EnFish_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFish* self = THIS;
    s16 params = self->actor.params;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gFishSkel, &gFishInWaterAnim, self->jointTable, self->morphTable,
                       7);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItems);
    self->actor.colChkInfo.mass = 50;
    self->slowPhase = Rand_ZeroOne() * (0xFFFF + 0.5f);
    self->fastPhase = Rand_ZeroOne() * (0xFFFF + 0.5f);

    if (params == FISH_DROPPED) {
        self->actor.flags |= 0x10;
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 8.0f);
        EnFish_Dropped_SetupFall(self);
    } else if (params == FISH_SWIMMING_UNIQUE) {
        EnFish_Unique_SetupSwimIdle(self);
    } else {
        EnFish_Respawning_SetupSlowDown(self);
    }
}

void EnFish_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnFish* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnFish_SetYOffset(EnFish* self) {
    self->actor.shape.yOffset += (Math_SinS(self->slowPhase) * 10.0f + Math_SinS(self->fastPhase) * 5.0f);
    self->actor.shape.yOffset = CLAMP(self->actor.shape.yOffset, -200.0f, 200.0f);
}

s32 EnFish_InBottleRange(EnFish* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f sp1C;

    if (self->actor.xzDistToPlayer < 32.0f) {
        sp1C.x = (Math_SinS(self->actor.yawTowardsPlayer + 0x8000) * 16.0f) + player->actor.world.pos.x;
        sp1C.y = player->actor.world.pos.y;
        sp1C.z = (Math_CosS(self->actor.yawTowardsPlayer + 0x8000) * 16.0f) + player->actor.world.pos.z;

        //! @bug: self check is superfluous: it is automatically satisfied if the coarse check is satisfied. It may have
        //! been intended to check the actor is in front of Player, but yawTowardsPlayer does not depend on Player's
        //! world rotation.
        if (EnFish_XZDistanceSquared(&sp1C, &self->actor.world.pos) <= SQ(20.0f)) {
            return true;
        }
    }

    return false;
}

s32 EnFish_CheckXZDistanceToPlayer(EnFish* self, GlobalContext* globalCtx) {
    return (self->actor.xzDistToPlayer < 60.0f);
}

// Respawning type functions

void EnFish_Respawning_SetupSlowDown(EnFish* self) {
    self->actor.gravity = 0.0f;
    self->actor.minVelocityY = 0.0f;
    self->timer = Rand_S16Offset(5, 35);
    self->unk_250 = 0;
    EnFish_SetInWaterAnimation(self);
    self->actionFunc = EnFish_Respawning_SlowDown;
}

void EnFish_Respawning_SlowDown(EnFish* self, GlobalContext* globalCtx) {
    EnFish_SetYOffset(self);
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.05f, 0.3f, 0.0f);
    self->skelAnime.playSpeed = CLAMP_MAX(self->actor.speedXZ * 1.4f + 0.8f, 2.0f);
    SkelAnime_Update(&self->skelAnime);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->timer <= 0) {
        EnFish_Respawning_SetupFollowChild(self);
    } else if (&self->actor == self->actor.child) {
        EnFish_Respawning_SetupApproachPlayer(self);
    } else if (EnFish_CheckXZDistanceToPlayer(self, globalCtx)) {
        EnFish_Respawning_SetupFleePlayer(self);
    }
}

// The three following actionfunctions also turn the yaw to home if the fish is too far from it.

void EnFish_Respawning_SetupFollowChild(EnFish* self) {
    self->actor.gravity = 0.0f;
    self->actor.minVelocityY = 0.0f;
    self->timer = Rand_S16Offset(15, 45);
    self->unk_250 = 0;
    EnFish_SetInWaterAnimation(self);
    self->actionFunc = EnFish_Respawning_FollowChild;
}

void EnFish_Respawning_FollowChild(EnFish* self, GlobalContext* globalCtx) {
    s32 pad;

    EnFish_SetYOffset(self);
    Math_SmoothStepToF(&self->actor.speedXZ, 1.8f, 0.08f, 0.4f, 0.0f);

    if ((EnFish_XZDistanceSquared(&self->actor.world.pos, &self->actor.home.pos) > SQ(80.0f)) || (self->timer < 4)) {
        Math_StepToAngleS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos),
                          3000);
    } else if ((self->actor.child != NULL) && (&self->actor != self->actor.child)) {
        Math_StepToAngleS(&self->actor.world.rot.y,
                          Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.child->world.pos), 3000);
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
    self->skelAnime.playSpeed = CLAMP_MAX(self->actor.speedXZ * 1.5f + 0.8f, 4.0f);
    SkelAnime_Update(&self->skelAnime);

    if (self->timer <= 0) {
        EnFish_Respawning_SetupSlowDown(self);
    } else if (&self->actor == self->actor.child) {
        EnFish_Respawning_SetupApproachPlayer(self);
    } else if (EnFish_CheckXZDistanceToPlayer(self, globalCtx)) {
        EnFish_Respawning_SetupFleePlayer(self);
    }
}

void EnFish_Respawning_SetupFleePlayer(EnFish* self) {
    self->actor.gravity = 0.0f;
    self->actor.minVelocityY = 0.0f;
    self->timer = Rand_S16Offset(10, 40);
    self->unk_250 = 0;
    EnFish_SetInWaterAnimation(self);
    self->actionFunc = EnFish_Respawning_FleePlayer;
}

void EnFish_Respawning_FleePlayer(EnFish* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 pad2;
    s16 frames;
    s16 yaw;
    s16 playerClose;

    EnFish_SetYOffset(self);
    playerClose = EnFish_CheckXZDistanceToPlayer(self, globalCtx);
    Math_SmoothStepToF(&self->actor.speedXZ, 4.2f, 0.08f, 1.4f, 0.0f);

    if (EnFish_XZDistanceSquared(&self->actor.world.pos, &self->actor.home.pos) > SQ(160.0f)) {
        yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
        Math_StepToAngleS(&self->actor.world.rot.y, yaw, 3000);
    } else if ((self->actor.child != NULL) && (&self->actor != self->actor.child)) {
        yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.child->world.pos);
        Math_StepToAngleS(&self->actor.world.rot.y, yaw, 2000);
    } else if (playerClose) {
        yaw = self->actor.yawTowardsPlayer + 0x8000;
        frames = globalCtx->state.frames;

        if (frames & 0x10) {
            if (frames & 0x20) {
                yaw += 0x2000;
            }
        } else {
            if (frames & 0x20) {
                yaw -= 0x2000;
            }
        }
        if (globalCtx) {}
        Math_StepToAngleS(&self->actor.world.rot.y, yaw, 2000);
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
    self->skelAnime.playSpeed = CLAMP_MAX(self->actor.speedXZ * 1.5f + 0.8f, 4.0f);

    SkelAnime_Update(&self->skelAnime);

    if ((self->timer <= 0) || !playerClose) {
        EnFish_Respawning_SetupSlowDown(self);
    } else if (&self->actor == self->actor.child) {
        EnFish_Respawning_SetupApproachPlayer(self);
    }
}

void EnFish_Respawning_SetupApproachPlayer(EnFish* self) {
    self->actor.gravity = 0.0f;
    self->actor.minVelocityY = 0.0f;
    EnFish_SetInWaterAnimation(self);
    self->timer = Rand_S16Offset(10, 40);
    self->unk_250 = 0;
    self->actionFunc = EnFish_Respawning_ApproachPlayer;
}

void EnFish_Respawning_ApproachPlayer(EnFish* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad2;
    Vec3f sp38;
    s16 yaw;
    s16 temp_a0_2;

    EnFish_SetYOffset(self);
    Math_SmoothStepToF(&self->actor.speedXZ, 1.8f, 0.1f, 0.5f, 0.0f);

    if (EnFish_XZDistanceSquared(&self->actor.world.pos, &self->actor.home.pos) > SQ(80.0f)) {
        yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
        Math_StepToAngleS(&self->actor.world.rot.y, yaw, 3000);
    } else {
        if ((s16)globalCtx->state.frames & 0x40) {
            temp_a0_2 = (self->actor.yawTowardsPlayer + 0x9000);
        } else {
            temp_a0_2 = (self->actor.yawTowardsPlayer + 0x7000);
        }

        sp38.x = player->actor.world.pos.x + (Math_SinS(temp_a0_2) * 20.0f);
        sp38.y = player->actor.world.pos.y;
        sp38.z = player->actor.world.pos.z + (Math_CosS(temp_a0_2) * 20.0f);

        yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &sp38);
        Math_StepToAngleS(&self->actor.world.rot.y, yaw, 3000);
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
    self->skelAnime.playSpeed = CLAMP_MAX((self->actor.speedXZ * 1.5f) + 0.8f, 4.0f);

    SkelAnime_Update(&self->skelAnime);

    if (self->timer <= 0) {
        EnFish_Respawning_SetupSlowDown(self);
    }
}

// Dropped type functions

void EnFish_Dropped_SetupFall(EnFish* self) {
    self->actor.gravity = -1.0f;
    self->actor.minVelocityY = -10.0f;
    self->actor.shape.yOffset = 0.0f;
    EnFish_SetOutOfWaterAnimation(self);
    self->unk_250 = 5;
    self->actionFunc = EnFish_Dropped_Fall;
    self->timer = 300;
}

void EnFish_Dropped_Fall(EnFish* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 0.1f, 0.0f);
    Math_StepToAngleS(&self->actor.world.rot.x, 0x4000, 100);
    Math_StepToAngleS(&self->actor.world.rot.z, -0x4000, 100);
    self->actor.shape.rot.x = self->actor.world.rot.x;
    self->actor.shape.rot.y = self->actor.world.rot.y;
    self->actor.shape.rot.z = self->actor.world.rot.z;
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.bgCheckFlags & 1) { // On floor
        self->timer = 400;
        EnFish_Dropped_SetupFlopOnGround(self);
    } else if (self->actor.bgCheckFlags & 0x20) { // In water
        EnFish_Dropped_SetupSwimAway(self);
    } else if ((self->timer <= 0) && (self->actor.params == FISH_DROPPED) &&
               (self->actor.floorHeight < BGCHECK_Y_MIN + 10.0f)) {
        osSyncPrintf(VT_COL(YELLOW, BLACK));
        // "BG missing? Running Actor_delete"
        osSyncPrintf("BG 抜け？ Actor_delete します(%s %d)\n", "../z_en_sakana.c", 822);
        osSyncPrintf(VT_RST);
        Actor_Kill(&self->actor);
    }
}

/**
 * If the fish is on a floor, self function is looped back to by EnFish_Dropped_FlopOnGround to set a new flopping
 * height and whether the sound should play again.
 */
void EnFish_Dropped_SetupFlopOnGround(EnFish* self) {
    s32 pad;
    f32 randomFloat;
    s32 playSound;

    self->actor.gravity = -1.0f;
    self->actor.minVelocityY = -10.0f;
    randomFloat = Rand_ZeroOne();

    if (randomFloat < 0.1f) {
        self->actor.velocity.y = (Rand_ZeroOne() * 3.0f) + 2.5f;
        playSound = true;
    } else if (randomFloat < 0.2f) {
        self->actor.velocity.y = (Rand_ZeroOne() * 1.2f) + 0.2f;
        playSound = true;
    } else {
        self->actor.velocity.y = 0.0f;

        if (Rand_ZeroOne() < 0.2f) {
            playSound = true;
        } else {
            playSound = false;
        }
    }

    self->actor.shape.yOffset = 300.0f;
    EnFish_SetOutOfWaterAnimation(self);
    self->actionFunc = EnFish_Dropped_FlopOnGround;
    self->unk_250 = 5;

    if (playSound && (self->actor.draw != NULL)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_FISH_LEAP);
    }
}

void EnFish_Dropped_FlopOnGround(EnFish* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 frames = globalCtx->state.frames;
    s16 targetXRot;

    Math_SmoothStepToF(&self->actor.speedXZ, Rand_ZeroOne() * 0.2f, 0.1f, 0.1f, 0.0f);

    targetXRot = (s16)((((frames >> 5) & 2) | ((frames >> 2) & 1)) << 0xB) * 0.3f;

    if (frames & 4) {
        targetXRot = -targetXRot;
    }

    Math_StepToAngleS(&self->actor.world.rot.x, targetXRot, 4000);
    Math_StepToAngleS(&self->actor.world.rot.z, 0x4000, 1000);
    self->actor.world.rot.y +=
        (s16)(((Math_SinS(self->slowPhase) * 2000.0f) + (Math_SinS(self->fastPhase) * 1000.0f)) * Rand_ZeroOne());
    self->actor.shape.rot = self->actor.world.rot;

    SkelAnime_Update(&self->skelAnime);

    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->timer <= 60) {
        // Blink when about to disappear
        if (frames & 4) {
            self->actor.draw = EnFish_Draw;
        } else {
            self->actor.draw = NULL;
        }
    } else if (self->actor.bgCheckFlags & 0x20) { // In water
        EnFish_Dropped_SetupSwimAway(self);
    } else if (self->actor.bgCheckFlags & 1) { // On floor
        EnFish_Dropped_SetupFlopOnGround(self);
    }
}

void EnFish_Dropped_SetupSwimAway(EnFish* self) {
    self->actor.home.pos = self->actor.world.pos;
    self->actor.flags |= 0x10;
    self->timer = 200;
    self->actor.gravity = 0.0f;
    self->actor.minVelocityY = 0.0f;
    self->actor.shape.yOffset = 0.0f;
    EnFish_SetInWaterAnimation(self);
    self->actionFunc = EnFish_Dropped_SwimAway;
    self->unk_250 = 5;
}

void EnFish_Dropped_SwimAway(EnFish* self, GlobalContext* globalCtx) {
    s32 pad;

    Math_SmoothStepToF(&self->actor.speedXZ, 2.8f, 0.1f, 0.4f, 0.0f);

    // If touching wall or not in water, turn back and slow down for one frame.
    if ((self->actor.bgCheckFlags & 8) || !(self->actor.bgCheckFlags & 0x20)) {
        self->actor.home.rot.y = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
        self->actor.speedXZ *= 0.5f;
    }

    Math_StepToAngleS(&self->actor.world.rot.x, 0, 1500);
    Math_StepToAngleS(&self->actor.world.rot.y, self->actor.home.rot.y, 3000);
    Math_StepToAngleS(&self->actor.world.rot.z, 0, 1000);

    self->actor.shape.rot = self->actor.world.rot;

    // Raise if on a floor.
    if (self->actor.bgCheckFlags & 1) {
        Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - 4.0f, 2.0f);
    } else {
        Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - 10.0f, 2.0f);
    }

    // Shrink when close to disappearing.
    if (self->timer < 100) {
        Actor_SetScale(&self->actor, self->actor.scale.x * 0.982f);
    }

    self->skelAnime.playSpeed = CLAMP_MAX((self->actor.speedXZ * 1.5f) + 1.0f, 4.0f);
    SkelAnime_Update(&self->skelAnime);

    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
    }
}

// Unique type functions

void EnFish_Unique_SetupSwimIdle(EnFish* self) {
    self->actor.gravity = 0.0f;
    self->actor.minVelocityY = 0.0f;
    self->timer = Rand_S16Offset(5, 35);
    self->unk_250 = 0;
    EnFish_SetInWaterAnimation(self);
    self->actionFunc = EnFish_Unique_SwimIdle;
}

void EnFish_Unique_SwimIdle(EnFish* self, GlobalContext* globalCtx) {
    static f32 speedStopping[] = { 0.0f, 0.04f, 0.09f };
    static f32 speedMoving[] = { 0.5f, 0.1f, 0.15f };
    f32 playSpeed;
    u32 frames = globalCtx->gameplayFrames;
    f32* speed;
    s32 pad2;
    f32 extraPlaySpeed;
    s32 pad3;

    if (self->actor.xzDistToPlayer < 60.0f) {
        if (self->timer < 12) {
            speed = speedMoving;
        } else {
            speed = speedStopping;
        }
    } else {
        if (self->timer < 4) {
            speed = speedMoving;
        } else {
            speed = speedStopping;
        }
    }

    EnFish_SetYOffset(self);
    Math_SmoothStepToF(&self->actor.speedXZ, speed[0], speed[1], speed[2], 0.0f);

    extraPlaySpeed = 0.0f;

    if ((EnFish_XZDistanceSquared(&self->actor.world.pos, &self->actor.home.pos) > SQ(15.0f))) {
        if (!Math_ScaledStepToS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos),
                                200)) {
            extraPlaySpeed = 0.5f;
        }
    } else if ((self->timer < 4) && !Math_ScaledStepToS(&self->actor.world.rot.y, frames * 0x80, 100)) {
        extraPlaySpeed = 0.5f;
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
    playSpeed = (self->actor.speedXZ * 1.2f) + 0.2f + extraPlaySpeed;
    self->skelAnime.playSpeed = CLAMP(playSpeed, 1.5f, 0.5);
    SkelAnime_Update(&self->skelAnime);

    if (self->timer <= 0) {
        self->timer = Rand_S16Offset(5, 80);
    }
}

// Cutscene functions

void EnFish_Cutscene_FlopOnGround(EnFish* self, GlobalContext* globalCtx) {
    f32 sp24 = Math_SinS(self->slowPhase);
    f32 sp20 = Math_SinS(self->fastPhase);

    D_80A17014 += D_80A17018;

    if (D_80A17014 <= 1.0f) {
        D_80A17014 = 1.0f;

        if (Rand_ZeroOne() < 0.1f) {
            D_80A17018 = (Rand_ZeroOne() * 3.0f) + 2.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_FISH_LEAP);
        } else {
            D_80A17018 = 0.0f;
        }
    } else {
        D_80A17018 -= 0.4f;
    }

    self->skelAnime.playSpeed = ((sp24 + sp20) * 0.5f) + 2.0f;
    SkelAnime_Update(&self->skelAnime);
}

void EnFish_Cutscene_WiggleFlyingThroughAir(EnFish* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 sp28 = Math_SinS(self->slowPhase);
    f32 sp24 = Math_SinS(self->fastPhase);

    self->actor.shape.rot.x -= 500;
    self->actor.shape.rot.z += 100;
    Math_StepToF(&D_80A17014, 0.0f, 1.0f);
    self->skelAnime.playSpeed = ((sp28 + sp24) * 0.5f) + 2.0f;
    SkelAnime_Update(&self->skelAnime);
}

void EnFish_UpdateCutscene(EnFish* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;
    CsCmdActorAction* csAction = globalCtx->csCtx.npcActions[1];
    Vec3f startPos;
    Vec3f endPos;
    f32 progress;
    s32 bgId;

    if (csAction == NULL) {
        // "Warning : DEMO ended without dousa (action) 3 termination being called"
        osSyncPrintf("Warning : dousa 3 消滅 が呼ばれずにデモが終了した(%s %d)(arg_data 0x%04x)\n", "../z_en_sakana.c",
                     1169, self->actor.params);
        EnFish_ClearCutsceneData(self);
        Actor_Kill(&self->actor);
        return;
    }

    self->slowPhase += 0x111;
    self->fastPhase += 0x500;

    switch (csAction->action) {
        case 1:
            EnFish_Cutscene_FlopOnGround(self, globalCtx);
            break;
        case 2:
            EnFish_Cutscene_WiggleFlyingThroughAir(self, globalCtx);
            break;
        case 3:
            // "DEMO fish termination"
            osSyncPrintf("デモ魚消滅\n");
            EnFish_ClearCutsceneData(self);
            Actor_Kill(&self->actor);
            return;
        default:
            // "Improper DEMO action"
            osSyncPrintf("不正なデモ動作(%s %d)(arg_data 0x%04x)\n", "../z_en_sakana.c", 1200, self->actor.params);
            break;
    }

    startPos.x = csAction->startPos.x;
    startPos.y = csAction->startPos.y;
    startPos.z = csAction->startPos.z;
    endPos.x = csAction->endPos.x;
    endPos.y = csAction->endPos.y;
    endPos.z = csAction->endPos.z;

    progress = Environment_LerpWeight(csAction->endFrame, csAction->startFrame, globalCtx->csCtx.frames);

    self->actor.world.pos.x = (endPos.x - startPos.x) * progress + startPos.x;
    self->actor.world.pos.y = (endPos.y - startPos.y) * progress + startPos.y + D_80A17014;
    self->actor.world.pos.z = (endPos.z - startPos.z) * progress + startPos.z;

    self->actor.floorHeight = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->actor.floorPoly, &bgId,
                                                          &self->actor, &self->actor.world.pos);
}

// Update functions and Draw

void EnFish_OrdinaryUpdate(EnFish* self, GlobalContext* globalCtx) {
    if (self->timer > 0) {
        self->timer--;
    }

    self->slowPhase += 0x111;
    self->fastPhase += 0x500;

    if ((self->actor.child != NULL) && (self->actor.child->update == NULL) && (&self->actor != self->actor.child)) {
        self->actor.child = NULL;
    }

    if ((self->actionFunc == NULL) || (self->actionFunc(self, globalCtx), (self->actor.update != NULL))) {
        Actor_MoveForward(&self->actor);

        if (self->unk_250 != 0) {
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 17.5f, 4.0f, 0.0f, self->unk_250);
        }

        if (self->actor.xzDistToPlayer < 70.0f) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }

        Actor_SetFocus(&self->actor, self->actor.shape.yOffset * 0.01f);

        if (Actor_HasParent(&self->actor, globalCtx)) {
            self->actor.parent = NULL;

            if (self->actor.params == FISH_DROPPED) {
                Actor_Kill(&self->actor);
                return;
            }

            EnFish_BeginRespawn(self);
        } else if (EnFish_InBottleRange(self, globalCtx)) {
            // GI_MAX in self case allows the player to catch the actor in a bottle
            func_8002F434(&self->actor, globalCtx, GI_MAX, 80.0f, 20.0f);
        }
    }
}

void EnFish_RespawningUpdate(EnFish* self, GlobalContext* globalCtx) {
    if (self->actor.params == FISH_SWIMMING_UNIQUE) {
        Actor_Kill(&self->actor);
        return;
    }

    if ((self->actor.child != NULL) && (self->actor.child->update == NULL) && (&self->actor != self->actor.child)) {
        self->actor.child = NULL;
    }

    if ((self->actionFunc == NULL) || (self->actionFunc(self, globalCtx), (self->actor.update != NULL))) {
        Actor_MoveForward(&self->actor);

        if (self->respawnTimer == 20) {
            self->actor.draw = EnFish_Draw;
        } else if (self->respawnTimer == 0) {
            Actor_SetScale(&self->actor, 0.01f);
        } else if (self->respawnTimer < 20) {
            Actor_SetScale(&self->actor, CLAMP_MAX(self->actor.scale.x + 0.001f, 0.01f));
        }
    }
}

void EnFish_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnFish* self = THIS;

    if ((D_80A17010 == NULL) && (self->actor.params == FISH_DROPPED) && (globalCtx->csCtx.state != 0) &&
        (globalCtx->csCtx.npcActions[1] != NULL)) {
        EnFish_SetCutsceneData(self);
    }

    if ((D_80A17010 != NULL) && (&self->actor == D_80A17010)) {
        EnFish_UpdateCutscene(self, globalCtx);
    } else if (self->respawnTimer > 0) {
        self->respawnTimer--;
        EnFish_RespawningUpdate(self, globalCtx);
    } else {
        EnFish_OrdinaryUpdate(self, globalCtx);
    }
}

void EnFish_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnFish* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, NULL);
    Collider_UpdateSpheres(0, &self->collider);
}
