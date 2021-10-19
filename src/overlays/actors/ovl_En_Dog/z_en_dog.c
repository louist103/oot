/*
 * File: z_en_dog.c
 * Overlay: ovl_En_Dog
 * Description: Dog
 */

#include "z_en_dog.h"
#include "objects/object_dog/object_dog.h"

#define FLAGS 0x00000000

#define THIS ((EnDog*)thisx)

void EnDog_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDog_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDog_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDog_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDog_FollowPath(EnDog* self, GlobalContext* globalCtx);
void EnDog_ChooseMovement(EnDog* self, GlobalContext* globalCtx);
void EnDog_FollowLink(EnDog* self, GlobalContext* globalCtx);
void EnDog_RunAway(EnDog* self, GlobalContext* globalCtx);
void EnDog_FaceLink(EnDog* self, GlobalContext* globalCtx);
void EnDog_Wait(EnDog* self, GlobalContext* globalCtx);

const ActorInit En_Dog_InitVars = {
    ACTOR_EN_DOG,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DOG,
    sizeof(EnDog),
    (ActorFunc)EnDog_Init,
    (ActorFunc)EnDog_Destroy,
    (ActorFunc)EnDog_Update,
    (ActorFunc)EnDog_Draw,
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
    { 16, 20, 0, { 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, 50 };

static struct_80034EC0_Entry sAnimations[] = {
    { &object_dog_Anim_001368, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    { &object_dog_Anim_001368, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -6.0f },
    { &object_dog_Anim_000D78, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -6.0f },
    { &object_dog_Anim_000278, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -6.0f },
    { &object_dog_Anim_001150, 1.0f, 0.0f, 4.0f, ANIMMODE_ONCE, -6.0f },
    { &object_dog_Anim_001150, 1.0f, 5.0f, 25.0f, ANIMMODE_LOOP_PARTIAL, -6.0f },
    { &object_dog_Anim_000928, 1.0f, 0.0f, 6.0f, ANIMMODE_ONCE, -6.0f },
    { &object_dog_Anim_000C28, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -6.0f },
};

typedef enum {
    /* 0x00 */ DOG_WALK,
    /* 0x01 */ DOG_RUN,
    /* 0x02 */ DOG_BARK,
    /* 0x03 */ DOG_SIT,
    /* 0x04 */ DOG_SIT_2,
    /* 0x05 */ DOG_BOW,
    /* 0x06 */ DOG_BOW_2
} DogBehavior;

void EnDog_PlayWalkSFX(EnDog* self) {
    AnimationHeader* walk = &object_dog_Anim_001368;

    if (self->skelAnime.animation == walk) {
        if ((self->skelAnime.curFrame == 1.0f) || (self->skelAnime.curFrame == 7.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_CHIBI_WALK);
        }
    }
}

void EnDog_PlayRunSFX(EnDog* self) {
    AnimationHeader* run = &object_dog_Anim_000D78;

    if (self->skelAnime.animation == run) {
        if ((self->skelAnime.curFrame == 2.0f) || (self->skelAnime.curFrame == 4.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_CHIBI_WALK);
        }
    }
}

void EnDog_PlayBarkSFX(EnDog* self) {
    AnimationHeader* bark = &object_dog_Anim_000278;

    if (self->skelAnime.animation == bark) {
        if ((self->skelAnime.curFrame == 13.0f) || (self->skelAnime.curFrame == 19.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_SMALL_DOG_BARK);
        }
    }
}

s32 EnDog_PlayAnimAndSFX(EnDog* self) {
    s32 animation;

    if (self->behavior != self->nextBehavior) {
        if (self->nextBehavior == DOG_SIT_2) {
            self->nextBehavior = DOG_SIT;
        }
        if (self->nextBehavior == DOG_BOW_2) {
            self->nextBehavior = DOG_BOW;
        }

        self->behavior = self->nextBehavior;
        switch (self->behavior) {
            case DOG_WALK:
                animation = 1;
                break;
            case DOG_RUN:
                animation = 2;
                break;
            case DOG_BARK:
                animation = 3;
                break;
            case DOG_SIT:
                animation = 4;
                break;
            case DOG_BOW:
                animation = 6;
                break;
        }
        func_80034EC0(&self->skelAnime, sAnimations, animation);
    }

    switch (self->behavior) {
        case DOG_SIT:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame) != 0) {
                func_80034EC0(&self->skelAnime, sAnimations, 5);
                self->behavior = self->nextBehavior = DOG_SIT_2;
            }
            break;
        case DOG_BOW:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame) != 0) {
                func_80034EC0(&self->skelAnime, sAnimations, 7);
                self->behavior = self->nextBehavior = DOG_BOW_2;
            }
            break;
        case DOG_WALK:
            EnDog_PlayWalkSFX(self);
            break;
        case DOG_RUN:
            EnDog_PlayRunSFX(self);
            break;
        case DOG_BARK:
            EnDog_PlayBarkSFX(self);
            if (self) {} // needed for regalloc
            break;
    }
    return 0;
}

s8 EnDog_CanFollow(EnDog* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        return 2;
    }

    if (globalCtx->sceneNum == SCENE_MARKET_DAY) {
        return 0;
    }

    if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
        self->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        if (gSaveContext.dogParams != 0) {
            return 0;
        }
        gSaveContext.dogParams = (self->actor.params & 0x7FFF);
        return 1;
    }

    return 0;
}

s32 EnDog_UpdateWaypoint(EnDog* self, GlobalContext* globalCtx) {
    s32 change;

    if (self->path == NULL) {
        return 0;
    }

    if (self->reverse) {
        change = -1;
    } else {
        change = 1;
    }

    self->waypoint += change;

    if (self->reverse) {
        if (self->waypoint < 0) {
            self->waypoint = self->path->count - 1;
        }
    } else {
        if ((self->path->count - 1) < self->waypoint) {
            self->waypoint = 0;
        }
    }

    return 1;
}

s32 EnDog_Orient(EnDog* self, GlobalContext* globalCtx) {
    s16 targetYaw;
    f32 waypointDistSq;

    waypointDistSq = Path_OrientAndGetDistSq(&self->actor, self->path, self->waypoint, &targetYaw);
    Math_SmoothStepToS(&self->actor.world.rot.y, targetYaw, 10, 1000, 1);

    if ((waypointDistSq > 0.0f) && (waypointDistSq < 1000.0f)) {
        return EnDog_UpdateWaypoint(self, globalCtx);
    } else {
        return 0;
    }
}

void EnDog_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDog* self = THIS;
    s16 followingDog;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_dog_Skel_007290, NULL, self->jointTable, self->morphTable,
                       13);
    func_80034EC0(&self->skelAnime, sAnimations, 0);

    if ((self->actor.params & 0x8000) == 0) {
        self->actor.params = (self->actor.params & 0xF0FF) | ((((self->actor.params & 0x0F00) >> 8) + 1) << 8);
    }

    followingDog = ((gSaveContext.dogParams & 0x0F00) >> 8);
    if (followingDog == ((self->actor.params & 0x0F00) >> 8) && ((self->actor.params & 0x8000) == 0)) {
        Actor_Kill(&self->actor);
        return;
    }

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, 0, &sColChkInfoInit);
    Actor_SetScale(&self->actor, 0.0075f);
    self->waypoint = 0;
    self->actor.gravity = -1.0f;
    self->path = Path_GetByIndex(globalCtx, (self->actor.params & 0x00F0) >> 4, 0xF);

    switch (globalCtx->sceneNum) {
        case SCENE_MARKET_NIGHT:
            if ((!gSaveContext.dogIsLost) && (((self->actor.params & 0x0F00) >> 8) == 1)) {
                Actor_Kill(&self->actor);
            }
            break;
        case SCENE_IMPA: // Richard's Home
            if ((u32)(self->actor.params & 0x8000) == 0) {
                if (!gSaveContext.dogIsLost) {
                    self->nextBehavior = DOG_SIT;
                    self->actionFunc = EnDog_Wait;
                    self->actor.speedXZ = 0.0f;
                    return;
                } else {
                    Actor_Kill(&self->actor);
                    return;
                }
            }
            break;
    }

    if ((u32)(self->actor.params & 0x8000) != 0) {
        self->nextBehavior = DOG_WALK;
        self->actionFunc = EnDog_FollowLink;
    } else {
        self->nextBehavior = DOG_SIT;
        self->actionFunc = EnDog_ChooseMovement;
    }
}

void EnDog_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDog* self = THIS;
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnDog_FollowPath(EnDog* self, GlobalContext* globalCtx) {
    s32 behaviors[] = { DOG_SIT, DOG_BOW, DOG_BARK };
    s32 unused[] = { 40, 80, 20 };
    f32 speed;
    s32 frame;

    if (EnDog_CanFollow(self, globalCtx) == 1) {
        self->actionFunc = EnDog_FollowLink;
    }

    if (DECR(self->behaviorTimer) != 0) {
        if (self->nextBehavior == DOG_WALK) {
            speed = 1.0f;
        } else {
            speed = 4.0f;
        }
        Math_SmoothStepToF(&self->actor.speedXZ, speed, 0.4f, 1.0f, 0.0f);
        EnDog_Orient(self, globalCtx);
        self->actor.shape.rot = self->actor.world.rot;

        // Used to change between two text boxes for Richard's owner in the Market Day scene
        // depending on where he is on his path. En_Hy checks these event flags.
        if (self->waypoint < 9) {
            // Richard is close to her, text says something about his coat
            gSaveContext.eventInf[3] |= 1;
        } else {
            // Richard is far, text says something about running fast
            gSaveContext.eventInf[3] &= ~1;
        }
    } else {
        frame = globalCtx->state.frames % 3;
        self->nextBehavior = behaviors[frame];
        // no clue why they're using the behavior id to calculate timer. possibly meant to use the unused array?
        self->behaviorTimer = Rand_S16Offset(60, behaviors[frame]);
        self->actionFunc = EnDog_ChooseMovement;
    }
}

void EnDog_ChooseMovement(EnDog* self, GlobalContext* globalCtx) {
    if (EnDog_CanFollow(self, globalCtx) == 1) {
        self->actionFunc = EnDog_FollowLink;
    }

    if (DECR(self->behaviorTimer) == 0) {
        self->behaviorTimer = Rand_S16Offset(200, 100);
        if (globalCtx->state.frames % 2) {
            self->nextBehavior = DOG_WALK;
        } else {
            self->nextBehavior = DOG_RUN;
        }

        if (self->nextBehavior == DOG_RUN) {
            self->behaviorTimer /= 2;
        }
        self->actionFunc = EnDog_FollowPath;
    }
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.4f, 1.0f, 0.0f);
}

void EnDog_FollowLink(EnDog* self, GlobalContext* globalCtx) {
    f32 speed;

    if (gSaveContext.dogParams == 0) {
        self->nextBehavior = DOG_SIT;
        self->actionFunc = EnDog_Wait;
        self->actor.speedXZ = 0.0f;
        return;
    }

    if (self->actor.xzDistToPlayer > 400.0f) {
        if (self->nextBehavior != DOG_SIT && self->nextBehavior != DOG_SIT_2) {
            self->nextBehavior = DOG_BOW;
        }
        gSaveContext.dogParams = 0;
        speed = 0.0f;
    } else if (self->actor.xzDistToPlayer > 100.0f) {
        self->nextBehavior = DOG_RUN;
        speed = 4.0f;
    } else if (self->actor.xzDistToPlayer < 40.0f) {
        if (self->nextBehavior != DOG_BOW && self->nextBehavior != DOG_BOW_2) {
            self->nextBehavior = DOG_BOW;
        }
        speed = 0.0f;
    } else {
        self->nextBehavior = DOG_WALK;
        speed = 1.0f;
    }

    Math_ApproachF(&self->actor.speedXZ, speed, 0.6f, 1.0f);

    if (!(self->actor.xzDistToPlayer > 400.0f)) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 10, 1000, 1);
        self->actor.shape.rot = self->actor.world.rot;
    }
}

void EnDog_RunAway(EnDog* self, GlobalContext* globalCtx) {
    if (self->actor.xzDistToPlayer < 200.0f) {
        Math_ApproachF(&self->actor.speedXZ, 4.0f, 0.6f, 1.0f);
        Math_SmoothStepToS(&self->actor.world.rot.y, (self->actor.yawTowardsPlayer ^ 0x8000), 10, 1000, 1);
    } else {
        self->actionFunc = EnDog_FaceLink;
    }
    self->actor.shape.rot = self->actor.world.rot;
}

void EnDog_FaceLink(EnDog* self, GlobalContext* globalCtx) {
    s16 rotTowardLink;
    s16 prevRotY;
    f32 absAngleDiff;

    // if the dog is more than 200 units away from Link, turn to face him then wait
    if (200.0f <= self->actor.xzDistToPlayer) {
        self->nextBehavior = DOG_WALK;

        Math_ApproachF(&self->actor.speedXZ, 1.0f, 0.6f, 1.0f);

        rotTowardLink = self->actor.yawTowardsPlayer;
        prevRotY = self->actor.world.rot.y;
        Math_SmoothStepToS(&self->actor.world.rot.y, rotTowardLink, 10, 1000, 1);

        absAngleDiff = self->actor.world.rot.y;
        absAngleDiff -= prevRotY;
        absAngleDiff = fabsf(absAngleDiff);
        if (absAngleDiff < 200.0f) {
            self->nextBehavior = DOG_SIT;
            self->actionFunc = EnDog_Wait;
            self->actor.speedXZ = 0.0f;
        }
    } else {
        self->nextBehavior = DOG_RUN;
        self->actionFunc = EnDog_RunAway;
    }
    self->actor.shape.rot = self->actor.world.rot;
}

void EnDog_Wait(EnDog* self, GlobalContext* globalCtx) {
    self->unusedAngle = (self->actor.yawTowardsPlayer - self->actor.shape.rot.y);

    // If another dog is following Link and he gets within 200 units of waiting dog, run away
    if ((gSaveContext.dogParams != 0) && (self->actor.xzDistToPlayer < 200.0f)) {
        self->nextBehavior = DOG_RUN;
        self->actionFunc = EnDog_RunAway;
    }
}

void EnDog_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDog* self = THIS;
    s32 pad;

    EnDog_PlayAnimAndSFX(self);
    SkelAnime_Update(&self->skelAnime);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, self->collider.dim.radius, self->collider.dim.height * 0.5f, 0.0f,
                            5);
    Actor_MoveForward(&self->actor);
    self->actionFunc(self, globalCtx);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

s32 EnDog_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    return false;
}

void EnDog_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
}

void EnDog_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDog* self = THIS;
    Color_RGBA8 colors[] = { { 255, 255, 200, 0 }, { 150, 100, 50, 0 } };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dog.c", 972);

    func_80093D18(globalCtx->state.gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, colors[self->actor.params & 0xF].r, colors[self->actor.params & 0xF].g,
                   colors[self->actor.params & 0xF].b, colors[self->actor.params & 0xF].a);

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnDog_OverrideLimbDraw, EnDog_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dog.c", 994);
}
