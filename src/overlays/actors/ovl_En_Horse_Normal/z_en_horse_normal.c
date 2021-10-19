/*
 * File: z_en_horse_normal.c
 * Overlay: ovl_En_Horse_Normal
 * Description: Non-rideable horses (Lon Lon Ranch and Stable)
 */

#include "z_en_horse_normal.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_horse_normal/object_horse_normal.h"

#define FLAGS 0x00000000

#define THIS ((EnHorseNormal*)thisx)

typedef struct {
    Vec3s pos;
    u8 unk_06; // self may be a s16 if the always-0 following byte is actually not padding
} EnHorseNormalUnkStruct1;

typedef struct {
    s32 len;
    EnHorseNormalUnkStruct1* items;
} EnHorseNormalUnkStruct2;

typedef enum {
    /* 0x00 */ HORSE_CYCLE_ANIMATIONS,
    /* 0x01 */ HORSE_WANDER,
    /* 0x02 */ HORSE_WAIT,
    /* 0x03 */ HORSE_WAIT_CLONE,
    /* 0x04 */ HORSE_FOLLOW_PATH
} EnHorseNormalAction;

void EnHorseNormal_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHorseNormal_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHorseNormal_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHorseNormal_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A6B91C(EnHorseNormal* self, GlobalContext* globalCtx);
void func_80A6BC48(EnHorseNormal* self);
void func_80A6BCEC(EnHorseNormal* self);
void func_80A6C4CC(EnHorseNormal* self);
void func_80A6C6B0(EnHorseNormal* self);

const ActorInit En_Horse_Normal_InitVars = {
    ACTOR_EN_HORSE_NORMAL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HORSE_NORMAL,
    sizeof(EnHorseNormal),
    (ActorFunc)EnHorseNormal_Init,
    (ActorFunc)EnHorseNormal_Destroy,
    (ActorFunc)EnHorseNormal_Update,
    (ActorFunc)EnHorseNormal_Draw,
};

static AnimationHeader* sAnimations[] = {
    &gHorseNormalIdleAnim,      &gHorseNormalWhinnyAnim,  &gHorseNormalRefuseAnim,
    &gHorseNormalRearingAnim,   &gHorseNormalWalkingAnim, &gHorseNormalTrottingAnim,
    &gHorseNormalGallopingAnim, &gHorseNormalJumpingAnim, &gHorseNormalJumpingHighAnim,
};

static ColliderCylinderInit sCylinderInit1 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 40, 100, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInit2 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 60, 100, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 11, { { 0, 0, 0 }, 20 }, 100 },
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
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static CollisionCheckInfoInit sColChkInfoInit = { 10, 35, 100, MASS_HEAVY };

// Unused
static EnHorseNormalUnkStruct1 D_80A6D428[] = {
    { { 1058, 1, 384 }, 7 },    { { 1653, 39, -381 }, 6 }, { { 1606, 1, -1048 }, 6 }, { { 1053, 1, -1620 }, 6 },
    { { -1012, 1, -1633 }, 7 }, { { -1655, 1, -918 }, 6 }, { { -1586, 1, -134 }, 6 }, { { -961, 1, 403 }, 7 },
};

// Unused
static EnHorseNormalUnkStruct2 D_80A6D468 = { ARRAY_COUNT(D_80A6D428), D_80A6D428 };

// Unused
static EnHorseNormalUnkStruct1 D_80A6D470[] = {
    { { 88, 0, 2078 }, 10 },       { { 2482, 376, 4631 }, 7 },    { { 2228, -28, 6605 }, 12 },
    { { 654, -100, 8864 }, 7 },    { { -297, -500, 10667 }, 12 }, { { -5303, -420, 10640 }, 10 },
    { { -6686, -500, 7760 }, 10 }, { { -5260, 100, 5411 }, 7 },   { { -3573, -269, 3893 }, 10 },
};

// Unused
static EnHorseNormalUnkStruct2 D_80A6D4B8 = { ARRAY_COUNT(D_80A6D470), D_80A6D470 };

void func_80A6B250(EnHorseNormal* self) {
    static s32 D_80A6D4C0[] = { 0, 16 };

    if (D_80A6D4C0[self->unk_200] < self->skin.skelAnime.curFrame &&
        ((self->unk_200 != 0) || !(D_80A6D4C0[1] < self->skin.skelAnime.curFrame))) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_WALK, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        self->unk_200++;
        if (self->unk_200 >= ARRAY_COUNT(D_80A6D4C0)) {
            self->unk_200 = 0;
        }
    }
}

f32 func_80A6B30C(EnHorseNormal* self) {
    static f32 D_80A6D4C8[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.5f, 1.5f, 1.5f, 1.5f, 1.0f };
    f32 result;

    if (self->animationIdx == 4) {
        result = D_80A6D4C8[self->animationIdx] * self->actor.speedXZ * (1 / 2.0f);
    } else if (self->animationIdx == 5) {
        result = D_80A6D4C8[self->animationIdx] * self->actor.speedXZ * (1 / 3.0f);
    } else if (self->animationIdx == 6) {
        result = D_80A6D4C8[self->animationIdx] * self->actor.speedXZ * (1 / 5.0f);
    } else {
        result = D_80A6D4C8[self->animationIdx];
    }

    return result;
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 300, ICHAIN_STOP),
};

void EnHorseNormal_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseNormal* self = THIS;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.gravity = -3.5f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawHorse, 20.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    self->action = HORSE_CYCLE_ANIMATIONS;
    self->animationIdx = 0;
    Collider_InitCylinder(globalCtx, &self->bodyCollider);
    Collider_SetCylinder(globalCtx, &self->bodyCollider, &self->actor, &sCylinderInit1);
    Collider_InitJntSph(globalCtx, &self->headCollider);
    Collider_SetJntSph(globalCtx, &self->headCollider, &self->actor, &sJntSphInit, self->headElements);
    Collider_InitCylinder(globalCtx, &self->cloneCollider);
    Collider_SetCylinder(globalCtx, &self->cloneCollider, &self->actor, &sCylinderInit2);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    if (globalCtx->sceneNum == SCENE_SPOT20) {
        if (self->actor.world.rot.z == 0 || gSaveContext.nightFlag) {
            Actor_Kill(&self->actor);
            return;
        }
        if (!LINK_IS_ADULT) {
            if (Flags_GetEventChkInf(0x14)) {
                if (self->actor.world.rot.z != 3) {
                    Actor_Kill(&self->actor);
                    return;
                }
            } else if (self->actor.world.rot.z != 1) {
                Actor_Kill(&self->actor);
                return;
            }
        } else if (Flags_GetEventChkInf(0x18) || (DREG(1) != 0)) {
            if (self->actor.world.rot.z != 7) {
                Actor_Kill(&self->actor);
                return;
            }
        } else if (self->actor.world.rot.z != 5) {
            Actor_Kill(&self->actor);
            return;
        }
        self->actor.home.rot.z = self->actor.world.rot.z = self->actor.shape.rot.z = 0;
        func_800A663C(globalCtx, &self->skin, &gHorseNormalSkel, &gHorseNormalIdleAnim);
        Animation_PlayOnce(&self->skin.skelAnime, sAnimations[self->animationIdx]);
        if ((self->actor.world.pos.x == -730.0f && self->actor.world.pos.y == 0.0f &&
             self->actor.world.pos.z == -1100.0f) ||
            (self->actor.world.pos.x == 880.0f && self->actor.world.pos.y == 0.0f &&
             self->actor.world.pos.z == -1170.0f)) {
            func_80A6C6B0(self);
            return;
        }
    } else if (globalCtx->sceneNum == SCENE_MALON_STABLE) {
        if (IS_DAY) {
            Actor_Kill(&self->actor);
            return;
        } else {
            func_800A663C(globalCtx, &self->skin, &gHorseNormalSkel, &gHorseNormalIdleAnim);
            Animation_PlayOnce(&self->skin.skelAnime, sAnimations[self->animationIdx]);
            func_80A6C6B0(self);
            return;
        }
    } else if (globalCtx->sceneNum == SCENE_SPOT12) {
        if (self->actor.world.pos.x == 3707.0f && self->actor.world.pos.y == 1413.0f &&
            self->actor.world.pos.z == -665.0f) {
            func_800A663C(globalCtx, &self->skin, &gHorseNormalSkel, &gHorseNormalIdleAnim);
            Animation_PlayOnce(&self->skin.skelAnime, sAnimations[self->animationIdx]);
            func_80A6C4CC(self);
            return;
        }
        func_800A663C(globalCtx, &self->skin, &gHorseNormalSkel, &gHorseNormalIdleAnim);
        Animation_PlayOnce(&self->skin.skelAnime, sAnimations[self->animationIdx]);
    } else {
        func_800A663C(globalCtx, &self->skin, &gHorseNormalSkel, &gHorseNormalIdleAnim);
        Animation_PlayOnce(&self->skin.skelAnime, sAnimations[self->animationIdx]);
    }
    if ((self->actor.params & 0xF0) == 0x10 && (self->actor.params & 0xF) != 0xF) {
        func_80A6B91C(self, globalCtx);
    } else {
        func_80A6BC48(self);
    }
}

void EnHorseNormal_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseNormal* self = THIS;

    func_800A6888(globalCtx, &self->skin);
    Collider_DestroyCylinder(globalCtx, &self->bodyCollider);
    Collider_DestroyCylinder(globalCtx, &self->cloneCollider);
    Collider_DestroyJntSph(globalCtx, &self->headCollider);
}

void func_80A6B91C(EnHorseNormal* self, GlobalContext* globalCtx) {
    self->actor.flags |= 0x10;
    self->action = HORSE_FOLLOW_PATH;
    self->animationIdx = 6;
    self->waypoint = 0;
    self->actor.speedXZ = 7.0f;
    Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                     Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
}

void EnHorseNormal_FollowPath(EnHorseNormal* self, GlobalContext* globalCtx) {
    Path* path = &globalCtx->setupPathList[self->actor.params & 0xF];
    Vec3s* pointPos = SEGMENTED_TO_VIRTUAL(path->points);
    f32 dx;
    f32 dz;
    s32 pad;

    pointPos += self->waypoint;
    dx = pointPos->x - self->actor.world.pos.x;
    dz = pointPos->z - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.world.rot.y, Math_FAtan2F(dx, dz) * (0x8000 / M_PI), 0xA, 0x7D0, 1);
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (SQ(dx) + SQ(dz) < 600.0f) {
        self->waypoint++;
        if (self->waypoint >= path->count) {
            self->waypoint = 0;
        }
    }
    self->skin.skelAnime.playSpeed = func_80A6B30C(self);
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                         Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
        func_80A6BCEC(self);
    }
}

void EnHorseNormal_NextAnimation(EnHorseNormal* self) {
    self->action = HORSE_CYCLE_ANIMATIONS;
    self->animationIdx++;

    if (self->animationIdx >= ARRAY_COUNT(sAnimations)) {
        self->animationIdx = 0;
    }

    Animation_PlayOnce(&self->skin.skelAnime, sAnimations[self->animationIdx]);
}

void EnHorseNormal_CycleAnimations(EnHorseNormal* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 0.0f;

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        EnHorseNormal_NextAnimation(self);
    }
}

void func_80A6BC48(EnHorseNormal* self) {
    self->action = HORSE_WANDER;
    self->animationIdx = 0;
    self->unk_21C = 0;
    self->unk_21E = 0;
    self->actor.speedXZ = 0.0f;
    self->unk_218 = 0.0f;
    Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                     Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
}

void func_80A6BCEC(EnHorseNormal* self) {
    if (self->animationIdx == 5) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    } else if (self->animationIdx == 6) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
}

void func_80A6BD7C(EnHorseNormal* self) {
    f32 frame = self->skin.skelAnime.curFrame;

    if (self->animationIdx == 0 && frame > 28.0f && !(self->unk_1E4 & 1)) {
        self->unk_1E4 |= 1;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_SANDDUST, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else if (self->animationIdx == 3 && frame > 25.0f && !(self->unk_1E4 & 2)) {
        self->unk_1E4 |= 2;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND2, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

void EnHorseNormal_Wander(EnHorseNormal* self, GlobalContext* globalCtx) {
    static s32 D_80A6D4F4[] = { 0, 1, 4, 5, 6, 2, 3 };
    static s32 D_80A6D510[] = { 0, 0, 2, 2, 1, 1, 1, 3, 3 };
    s32 phi_t0 = self->animationIdx;
    s32 pad;

    switch (D_80A6D510[self->animationIdx]) {
        case 0:
            func_80A6BD7C(self);
            self->actor.speedXZ = 0.0f;
            self->unk_218 = 0.0f;
            break;
        case 1:
            if (Rand_ZeroOne() < 0.1f) {
                self->unk_218 = 2.0f * Rand_ZeroOne() - 1.0f;
            }
            self->actor.speedXZ += self->unk_218;
            if (self->actor.speedXZ <= 0.0f) {
                self->actor.speedXZ = 0.0f;
                self->unk_218 = 0.0f;
                phi_t0 = 0;
            } else if (self->actor.speedXZ < 3.0f) {
                func_80A6B250(self);
                phi_t0 = 4;
            } else if (self->actor.speedXZ < 6.0f) {
                phi_t0 = 5;
            } else if (self->actor.speedXZ < 8.0f) {
                phi_t0 = 6;
            } else {
                self->actor.speedXZ = 8.0f;
                phi_t0 = 6;
            }
            if (Rand_ZeroOne() < 0.1f || (self->unk_21E == 0 && ((self->actor.bgCheckFlags & 8) ||
                                                                 (self->bodyCollider.base.ocFlags1 & OC1_HIT) ||
                                                                 (self->headCollider.base.ocFlags1 & OC1_HIT)))) {
                self->unk_21E += (Rand_ZeroOne() * 30.0f) - 15.0f;
                if (self->unk_21E > 50) {
                    self->unk_21E = 50;
                } else if (self->unk_21E < -50) {
                    self->unk_21E = -50;
                }
            }
            self->unk_21C += self->unk_21E;
            if (self->unk_21C < -300) {
                self->unk_21C = -300;
            } else if (self->unk_21C > 300) {
                self->unk_21C = 300;
            } else if (Rand_ZeroOne() < 0.25f && fabsf(self->unk_21C) < 100.0f) {
                self->unk_21C = 0;
                self->unk_21E = 0;
            }
            self->actor.world.rot.y += self->unk_21C;
            self->actor.shape.rot.y = self->actor.world.rot.y;
            break;
        case 2:
        case 3:
            break;
    }

    if (phi_t0 != self->animationIdx || SkelAnime_Update(&self->skin.skelAnime)) {
        if (phi_t0 != self->animationIdx) {
            self->animationIdx = phi_t0;
            self->unk_1E4 &= ~1;
            self->unk_1E4 &= ~2;
            if (phi_t0 == 1) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_GROAN, &self->unk_204, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            } else if (phi_t0 == 3) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_204, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            } else {
                func_80A6BCEC(self);
            }
            Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                             Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -3.0f);
        } else {
            switch (D_80A6D510[self->animationIdx]) {
                case 0:
                    if (Rand_ZeroOne() < 0.25f) {
                        self->unk_218 = 1.0f;
                        phi_t0 = 4;
                    } else {
                        phi_t0 = D_80A6D4F4[(s32)(Rand_ZeroOne() * 2)];
                        self->actor.speedXZ = 0.0f;
                        self->unk_218 = 0.0f;
                    }
                    break;
                case 1:
                case 2:
                case 3:
                    break;
            }

            self->unk_1E4 &= ~1;
            self->unk_1E4 &= ~2;
            if (phi_t0 == 1) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_GROAN, &self->unk_204, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            } else if (phi_t0 == 3) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_204, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            } else {
                func_80A6BCEC(self);
            }
            if (phi_t0 != self->animationIdx) {
                self->animationIdx = phi_t0;
                Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                                 Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -3.0f);
            } else {
                Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                                 Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
            }
        }
    }
}

void func_80A6C4CC(EnHorseNormal* self) {
    self->action = HORSE_WAIT;
    self->animationIdx = 0;
    self->unk_21C = 0;
    self->unk_21E = 0;
    self->actor.speedXZ = 0.0f;
    self->unk_218 = 0.0f;
    Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                     Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
}

void EnHorseNormal_Wait(EnHorseNormal* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        f32 rand = Rand_ZeroOne();

        if (rand < 0.4f) {
            self->animationIdx = 0;
        } else if (rand < 0.8f) {
            self->animationIdx = 1;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_GROAN, &self->unk_204, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        } else {
            self->animationIdx = 3;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_204, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        }

        Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                         Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
    }
}

void func_80A6C6B0(EnHorseNormal* self) {
    self->action = HORSE_WAIT_CLONE;
    self->animationIdx = 0;
    self->unk_21C = 0;
    self->unk_21E = 0;
    self->actor.flags |= 0x30;
    self->actor.speedXZ = 0.0f;
    self->unk_218 = 0.0f;
    Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                     Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
}

void EnHorseNormal_WaitClone(EnHorseNormal* self, GlobalContext* globalCtx) {
    func_80A6BD7C(self);

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        f32 rand = Rand_ZeroOne();

        if (rand < 0.4f) {
            self->animationIdx = 0;
        } else if (rand < 0.8f) {
            self->animationIdx = 1;
            self->unk_1E4 |= 0x20;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_GROAN, &self->unk_204, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        } else {
            self->animationIdx = 3;
            self->unk_1E4 |= 0x20;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_204, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        }

        Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A6B30C(self), 0.0f,
                         Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);

        self->unk_1E4 &= ~1;
        self->unk_1E4 &= ~2;
        self->unk_1E4 &= ~8;
        self->unk_1E4 &= ~0x10;
    }
}

void func_80A6C8E0(EnHorseNormal* self, GlobalContext* globalCtx) {
    s32 pad;
    CollisionPoly* sp38;
    s32 pad2;
    Vec3f sp28;
    s32 sp24;

    sp28.x = (Math_SinS(self->actor.shape.rot.y) * 30.0f) + self->actor.world.pos.x;
    sp28.y = self->actor.world.pos.y + 60.0f;
    sp28.z = (Math_CosS(self->actor.shape.rot.y) * 30.0f) + self->actor.world.pos.z;
    self->unk_220 = BgCheck_EntityRaycastFloor3(&globalCtx->colCtx, &sp38, &sp24, &sp28);
    self->actor.shape.rot.x = Math_FAtan2F(self->actor.world.pos.y - self->unk_220, 30.0f) * (0x8000 / M_PI);
}

static EnHorseNormalActionFunc sActionFuncs[] = {
    EnHorseNormal_CycleAnimations, EnHorseNormal_Wander,     EnHorseNormal_Wait,
    EnHorseNormal_WaitClone,       EnHorseNormal_FollowPath,
};

void EnHorseNormal_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseNormal* self = THIS;
    s32 pad;

    sActionFuncs[self->action](self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 35.0f, 100.0f, 0x1D);
    if (globalCtx->sceneNum == SCENE_SPOT20 && self->actor.world.pos.z < -2400.0f) {
        self->actor.world.pos.z = -2400.0f;
    }
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    self->unk_204 = self->actor.projectedPos;
    self->unk_204.y += 120.0f;
    Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
    if (self->actor.speedXZ == 0.0f) {
        self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    } else {
        self->actor.colChkInfo.mass = MASS_HEAVY;
    }
}

void func_80A6CAFC(Actor* thisx, GlobalContext* globalCtx, PSkinAwb* skin) {
    Vec3f sp4C;
    Vec3f sp40;
    EnHorseNormal* self = THIS;
    s32 i;

    for (i = 0; i < self->headCollider.count; i++) {
        sp4C.x = self->headCollider.elements[i].dim.modelSphere.center.x;
        sp4C.y = self->headCollider.elements[i].dim.modelSphere.center.y;
        sp4C.z = self->headCollider.elements[i].dim.modelSphere.center.z;
        func_800A6408(skin, self->headCollider.elements[i].dim.limb, &sp4C, &sp40);
        self->headCollider.elements[i].dim.worldSphere.center.x = sp40.x;
        self->headCollider.elements[i].dim.worldSphere.center.y = sp40.y;
        self->headCollider.elements[i].dim.worldSphere.center.z = sp40.z;
        self->headCollider.elements[i].dim.worldSphere.radius =
            self->headCollider.elements[i].dim.modelSphere.radius * self->headCollider.elements[i].dim.scale;
    }

    //! @bug see relevant comment in `EnHorse_SkinCallback1`
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->headCollider.base);
}

void func_80A6CC88(GlobalContext* globalCtx, EnHorseNormal* self, Vec3f* arg2) {
    f32 curFrame = self->skin.skelAnime.curFrame;
    f32 wDest;

    SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, arg2, &self->unk_1E8, &wDest);
    self->unk_1F4 = self->unk_1E8;
    self->unk_1F4.y += 120.0f;

    if (self->animationIdx == 0 && curFrame > 28.0f && !(self->unk_1E4 & 8)) {
        self->unk_1E4 |= 8;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_SANDDUST, &self->unk_1E8, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    } else if (self->animationIdx == 3 && curFrame > 25.0f && !(self->unk_1E4 & 0x10)) {
        self->unk_1E4 |= 0x10;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND2, &self->unk_1E8, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    } else if (self->animationIdx == 3 && self->unk_1E4 & 0x20) {
        self->unk_1E4 &= ~0x20;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_1F4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    } else if (self->animationIdx == 1 && self->unk_1E4 & 0x20) {
        self->unk_1E4 &= ~0x20;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_GROAN, &self->unk_1F4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
}

void EnHorseNormal_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseNormal* self = THIS;
    Mtx* mtx2;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_horse_normal.c", 2224);

    if (globalCtx->sceneNum != SCENE_SPOT20 || globalCtx->sceneNum != SCENE_MALON_STABLE) {
        func_80A6C8E0(self, globalCtx);
    }
    func_80093D18(globalCtx->state.gfxCtx);
    func_800A6330(&self->actor, globalCtx, &self->skin, func_80A6CAFC, 1);

    if (self->action == HORSE_WAIT_CLONE) {
        MtxF skinMtx;
        Mtx* mtx1;
        Vec3f clonePos = { 0.0f, 0.0f, 0.0f };
        s16 cloneRotY;
        f32 distFromGround = self->actor.world.pos.y - self->actor.floorHeight;
        f32 temp_f0_4;

        if (globalCtx->sceneNum == SCENE_MALON_STABLE) {
            if (self->actor.world.pos.x == 355.0f && self->actor.world.pos.y == 0.0f &&
                self->actor.world.pos.z == -245.0f) {
                clonePos.x = 235.0f;
                clonePos.y = 0.0f;
                clonePos.z = 100.0f;
                cloneRotY = 0x7FFF;
            } else if (self->actor.world.pos.x == 238.0f && self->actor.world.pos.y == 0.0f &&
                       self->actor.world.pos.z == -245.0f) {
                clonePos.x = 478.0f;
                clonePos.y = 0.0f;
                clonePos.z = 100.0f;
                cloneRotY = 0x7FFF;
            }
        } else if (globalCtx->sceneNum == SCENE_SPOT20) {
            if (self->actor.world.pos.x == -730.0f && self->actor.world.pos.y == 0.0f &&
                self->actor.world.pos.z == -1100.0f) {
                clonePos.x = 780.0f;
                clonePos.y = 0.0f;
                clonePos.z = -80.0f;
                cloneRotY = 0;
            } else if (self->actor.world.pos.x == 880.0f && self->actor.world.pos.y == 0.0f &&
                       self->actor.world.pos.z == -1170.0f) {
                clonePos.x = -1000.0f;
                clonePos.y = 0.0f;
                clonePos.z = -70.0f;
                cloneRotY = 0;
            }
        }
        func_80A6CC88(globalCtx, self, &clonePos);
        SkinMatrix_SetScaleRotateYRPTranslate(&skinMtx, self->actor.scale.x, self->actor.scale.y, self->actor.scale.z,
                                              self->actor.shape.rot.x, cloneRotY, self->actor.shape.rot.z, clonePos.x,
                                              (self->actor.shape.yOffset * self->actor.scale.y) + clonePos.y,
                                              clonePos.z);
        mtx1 = SkinMatrix_MtxFToNewMtx(globalCtx->state.gfxCtx, &skinMtx);
        if (mtx1 == NULL) {
            return;
        }
        gSPMatrix(POLY_OPA_DISP++, &gMtxClear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPMatrix(POLY_OPA_DISP++, mtx1, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_800A63CC(&self->actor, globalCtx, &self->skin, NULL, NULL, 1, 0, 3);
        self->cloneCollider.dim.pos.x = clonePos.x;
        self->cloneCollider.dim.pos.y = clonePos.y;
        self->cloneCollider.dim.pos.z = clonePos.z;
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->cloneCollider.base);
        func_80094044(globalCtx->state.gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, 255);
        Matrix_Translate(clonePos.x, clonePos.y, clonePos.z, MTXMODE_NEW);
        temp_f0_4 = (1.0f - (distFromGround * 0.01f)) * self->actor.shape.shadowScale;
        Matrix_Scale(self->actor.scale.x * temp_f0_4, 1.0f, self->actor.scale.z * temp_f0_4, MTXMODE_APPLY);
        Matrix_RotateY(cloneRotY * (2.0f * M_PI / 0x10000), MTXMODE_APPLY);
        mtx2 = Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_horse_normal.c", 2329);
        if (mtx2 != NULL) {
            gSPMatrix(POLY_XLU_DISP++, mtx2, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gHorseShadowDL);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_horse_normal.c", 2339);
}
