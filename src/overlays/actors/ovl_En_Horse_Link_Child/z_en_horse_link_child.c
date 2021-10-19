/*
 * File: z_en_horse_link_child.c
 * Overlay: ovl_En_Horse_Link_Child
 * Description: Young Epona
 */

#include "z_en_horse_link_child.h"
#include "objects/object_horse_link_child/object_horse_link_child.h"

#define FLAGS 0x02000010

#define THIS ((EnHorseLinkChild*)thisx)

void EnHorseLinkChild_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHorseLinkChild_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHorseLinkChild_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHorseLinkChild_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A69B7C(EnHorseLinkChild* self);
void func_80A69EC0(EnHorseLinkChild* self);
void func_80A6A4DC(EnHorseLinkChild* self);
void func_80A6A724(EnHorseLinkChild* self);

const ActorInit En_Horse_Link_Child_InitVars = {
    ACTOR_EN_HORSE_LINK_CHILD,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HORSE_LINK_CHILD,
    sizeof(EnHorseLinkChild),
    (ActorFunc)EnHorseLinkChild_Init,
    (ActorFunc)EnHorseLinkChild_Destroy,
    (ActorFunc)EnHorseLinkChild_Update,
    (ActorFunc)EnHorseLinkChild_Draw,
};

static AnimationHeader* sAnimations[] = {
    &gChildEponaIdleAnim,     &gChildEponaWhinnyAnim,    &gChildEponaWalkingAnim,
    &gChildEponaTrottingAnim, &gChildEponaGallopingAnim,
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
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
    { 20, 100, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 13, { { 0, 0, 0 }, 10 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1 | OC2_UNK1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementInit,
};

static CollisionCheckInfoInit sColCheckInfoInit = { 10, 35, 100, MASS_HEAVY };

void func_80A693D0(EnHorseLinkChild* self) {
    static s32 D_80A6AF5C[] = { 1, 19 };

    if ((self->skin.skelAnime.curFrame > D_80A6AF5C[self->unk_1F0]) &&
        !((self->unk_1F0 == 0) && (self->skin.skelAnime.curFrame > D_80A6AF5C[1]))) {
        Audio_PlaySoundGeneral(NA_SE_EV_KID_HORSE_WALK, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        self->unk_1F0++;
        if (self->unk_1F0 >= ARRAY_COUNT(D_80A6AF5C)) {
            self->unk_1F0 = 0;
        }
    }
}

void func_80A6948C(EnHorseLinkChild* self) {
    if (self->animationIdx == 2) {
        func_80A693D0(self);
    } else if (self->skin.skelAnime.curFrame == 0.0f) {
        if ((self->animationIdx == 3) || (self->animationIdx == 4)) {
            Audio_PlaySoundGeneral(NA_SE_EV_KID_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        } else if (self->animationIdx == 1) {
            if (Rand_ZeroOne() > 0.5f) {
                Audio_PlaySoundGeneral(NA_SE_EV_KID_HORSE_GROAN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                       &D_801333E8);
            } else {
                Audio_PlaySoundGeneral(NA_SE_EV_KID_HORSE_NEIGH, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                       &D_801333E8);
            }
        }
    }
}

static f32 D_80A6AF64[] = { 1.0f, 1.0f, 1.5f, 1.5f, 1.5f };

f32 func_80A695A4(EnHorseLinkChild* self) {
    f32 result;

    if (self->animationIdx == 2) {
        result = D_80A6AF64[self->animationIdx] * self->actor.speedXZ * (1.0f / 2.0f);
    } else if (self->animationIdx == 3) {
        result = D_80A6AF64[self->animationIdx] * self->actor.speedXZ * (1.0f / 3.0f);
    } else if (self->animationIdx == 4) {
        result = D_80A6AF64[self->animationIdx] * self->actor.speedXZ * (1.0f / 5.0f);
    } else {
        result = D_80A6AF64[self->animationIdx];
    }

    return result;
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_STOP),
};

void EnHorseLinkChild_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseLinkChild* self = THIS;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Actor_SetScale(&self->actor, 0.005f);
    self->actor.gravity = -3.5f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawHorse, 20.0f);
    self->actor.speedXZ = 0.0f;
    self->action = 1;
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    func_800A663C(globalCtx, &self->skin, &gChildEponaSkel, &gChildEponaGallopingAnim);
    self->animationIdx = 0;
    Animation_PlayOnce(&self->skin.skelAnime, sAnimations[0]);
    Collider_InitCylinder(globalCtx, &self->bodyCollider);
    Collider_SetCylinderType1(globalCtx, &self->bodyCollider, &self->actor, &sCylinderInit);
    Collider_InitJntSph(globalCtx, &self->headCollider);
    Collider_SetJntSph(globalCtx, &self->headCollider, &self->actor, &sJntSphInit, self->headElements);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColCheckInfoInit);
    self->unk_1F0 = 0;
    self->eyeTexIndex = 0;

    if (gSaveContext.sceneSetupIndex > 3) {
        func_80A69EC0(self);
    } else if (globalCtx->sceneNum == SCENE_SPOT20) {
        if (!Flags_GetEventChkInf(0x14)) {
            Actor_Kill(&self->actor);
            return;
        }
        self->unk_2A0 = gSaveContext.eventChkInf[1] & 0x40;
        func_80A69EC0(self);
    } else {
        func_80A69EC0(self);
    }

    self->actor.home.rot.z = self->actor.world.rot.z = self->actor.shape.rot.z = 0;
}

void EnHorseLinkChild_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseLinkChild* self = THIS;

    func_800A6888(globalCtx, &self->skin);
    Collider_DestroyCylinder(globalCtx, &self->bodyCollider);
    Collider_DestroyJntSph(globalCtx, &self->headCollider);
}

void func_80A6988C(EnHorseLinkChild* self) {
    self->action = 0;
    self->animationIdx++;
    if (self->animationIdx >= ARRAY_COUNT(sAnimations)) {
        self->animationIdx = 0;
    }

    Animation_PlayOnce(&self->skin.skelAnime, sAnimations[self->animationIdx]);
    self->skin.skelAnime.playSpeed = func_80A695A4(self);
}

void func_80A698F4(EnHorseLinkChild* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 0.0f;
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        func_80A6988C(self);
    }
}

void func_80A6993C(EnHorseLinkChild* self, s32 newAnimationIdx) {
    self->action = 2;
    self->actor.speedXZ = 0.0f;

    if (!((newAnimationIdx == 0) || (newAnimationIdx == 1))) {
        newAnimationIdx = 0;
    }

    if (self->animationIdx != newAnimationIdx) {
        self->animationIdx = newAnimationIdx;
        Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                         Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
    }
}

void func_80A699FC(EnHorseLinkChild* self, GlobalContext* globalCtx) {
    f32 distFromLink;
    s32 newAnimationIdx;

    distFromLink = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(globalCtx)->actor);

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        if ((distFromLink < 1000.0f) && (distFromLink > 70.0f)) {
            func_80A69B7C(self);
        } else {
            newAnimationIdx = self->animationIdx == 1 ? 0 : 1;
            if (self->animationIdx != newAnimationIdx) {
                self->animationIdx = newAnimationIdx;
                Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                                 Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
            } else {
                Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                                 Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0);
            }
        }
    }
}

void func_80A69B7C(EnHorseLinkChild* self) {
    self->action = 1;
    self->animationIdx = 0;
    self->actor.speedXZ = 0.0f;
    Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                     Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
}

void func_80A69C18(EnHorseLinkChild* self, GlobalContext* globalCtx) {
    s16 yawDiff;
    f32 distFromLink;
    s32 newAnimationIdx;

    if ((self->animationIdx == 4) || (self->animationIdx == 3) || (self->animationIdx == 2)) {
        yawDiff = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.world.rot.y;

        if (yawDiff > 0x12C) {
            self->actor.world.rot.y += 0x12C;
        } else if (yawDiff < -0x12C) {
            self->actor.world.rot.y -= 0x12C;
        } else {
            self->actor.world.rot.y += yawDiff;
        }

        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        distFromLink = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(globalCtx)->actor);

        if (distFromLink > 1000.0f) {
            func_80A6993C(self, 0);
            return;
        } else if ((distFromLink < 1000.0f) && (distFromLink >= 300.0f)) {
            newAnimationIdx = 4;
            self->actor.speedXZ = 6.0f;
        } else if ((distFromLink < 300.0f) && (distFromLink >= 150.0f)) {
            newAnimationIdx = 3;
            self->actor.speedXZ = 4.0f;
        } else if ((distFromLink < 150.0f) && (distFromLink >= 70.0f)) {
            newAnimationIdx = 2;
            self->actor.speedXZ = 2.0f;
            self->unk_1F0 = 0;
        } else {
            func_80A6993C(self, 1);
            return;
        }

        if (self->animationIdx != newAnimationIdx) {
            self->animationIdx = newAnimationIdx;
            Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                             Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
        } else {
            Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                             Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
        }
    }
}

void func_80A69EC0(EnHorseLinkChild* self) {
    self->action = 3;
    self->animationIdx = 0;
    self->actor.speedXZ = 0.0f;
    Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                     Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
}

void func_80A69F5C(EnHorseLinkChild* self, GlobalContext* globalCtx) {
    Player* player;
    s16 yawDiff;
    s32 yawSign;
    s32 yawOffset;

    if ((self->animationIdx == 4) || (self->animationIdx == 3) || (self->animationIdx == 2)) {
        player = GET_PLAYER(globalCtx);

        if (Math3D_Vec3f_DistXYZ(&player->actor.world.pos, &self->actor.home.pos) < 250.0f) {
            yawDiff = player->actor.shape.rot.y;
            yawSign = Actor_WorldYawTowardActor(&self->actor, &player->actor) > 0 ? 1 : -1;
            yawOffset = yawSign << 0xE;
            yawDiff += yawOffset;
        } else {
            yawDiff = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos) - self->actor.world.rot.y;
        }

        if (yawDiff > 0x12C) {
            self->actor.world.rot.y += 0x12C;
        } else if (yawDiff < -0x12C) {
            self->actor.world.rot.y -= 0x12C;
        } else {
            self->actor.world.rot.y += yawDiff;
        }

        self->actor.shape.rot.y = self->actor.world.rot.y;
    }
}

void func_80A6A068(EnHorseLinkChild* self, GlobalContext* globalCtx) {
    Player* player;
    f32 distFromLink;
    s32 animationEnded;
    s32 newAnimationIdx;
    f32 distFromHome;
    f32 distLinkFromHome;

    func_80A69F5C(self, globalCtx);
    player = GET_PLAYER(globalCtx);
    distFromLink = Actor_WorldDistXZToActor(&self->actor, &player->actor);

    if (gSaveContext.entranceIndex == 0x2AE) {
        Audio_PlaySoundGeneral(NA_SE_EV_KID_HORSE_NEIGH, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        func_80A6A724(self);
        return;
    }

    if (((gSaveContext.eventChkInf[1] & 0x40) && (DREG(53) != 0)) ||
        ((globalCtx->sceneNum == SCENE_SPOT20) && (gSaveContext.cutsceneIndex == 0xFFF1))) {
        func_80A6A4DC(self);
    } else {
        self->unk_2A0 = gSaveContext.eventChkInf[1] & 0x40;
    }

    newAnimationIdx = self->animationIdx;
    animationEnded = SkelAnime_Update(&self->skin.skelAnime);
    if (animationEnded || (self->animationIdx == 1) || (self->animationIdx == 0)) {
        if (gSaveContext.eventChkInf[1] & 0x20) {
            distFromHome = Math3D_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.home.pos);
            distLinkFromHome = Math3D_Vec3f_DistXYZ(&player->actor.world.pos, &self->actor.home.pos);
            if (distLinkFromHome > 250.0f) {
                if (distFromHome >= 300.0f) {
                    newAnimationIdx = 4;
                    self->actor.speedXZ = 6.0f;
                } else if ((distFromHome < 300.0f) && (distFromHome >= 150.0f)) {
                    newAnimationIdx = 3;
                    self->actor.speedXZ = 4.0f;
                } else if ((distFromHome < 150.0f) && (distFromHome >= 70.0f)) {
                    newAnimationIdx = 2;
                    self->actor.speedXZ = 2.0f;
                    self->unk_1F0 = 0;
                } else {
                    self->actor.speedXZ = 0.0f;
                    if (self->animationIdx == 0) {
                        newAnimationIdx = animationEnded == true ? 1 : 0;
                    } else {
                        newAnimationIdx = animationEnded == true ? 0 : 1;
                    }
                }
            } else {
                if (distFromLink < 200.0f) {
                    newAnimationIdx = 4;
                    self->actor.speedXZ = 6.0f;
                } else if (distFromLink < 300.0f) {
                    newAnimationIdx = 3;
                    self->actor.speedXZ = 4.0f;
                } else if (distFromLink < 400.0f) {
                    newAnimationIdx = 2;
                    self->actor.speedXZ = 2.0f;
                    self->unk_1F0 = 0;
                } else {
                    self->actor.speedXZ = 0.0f;
                    if (self->animationIdx == 0) {
                        newAnimationIdx = animationEnded == true ? 1 : 0;
                    } else {
                        newAnimationIdx = animationEnded == true ? 0 : 1;
                    }
                }
            }
        } else {
            self->actor.speedXZ = 0.0f;
            if (self->animationIdx == 0) {
                newAnimationIdx = animationEnded == true ? 1 : 0;
            } else {
                newAnimationIdx = animationEnded == true ? 0 : 1;
            }
        }
    }

    if ((self->animationIdx != newAnimationIdx) || (animationEnded == true)) {
        self->animationIdx = newAnimationIdx;
        Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                         Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
    } else {
        Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self),
                         self->skin.skelAnime.curFrame, Animation_GetLastFrame(sAnimations[self->animationIdx]), 2,
                         0.0f);
    }
}

void func_80A6A4DC(EnHorseLinkChild* self) {
    self->action = 5;
    self->animationIdx = Rand_ZeroOne() > 0.5f ? 0 : 1;
    DREG(53) = 0;
    Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                     Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
}

void func_80A6A5A4(EnHorseLinkChild* self, GlobalContext* globalCtx) {
    s16 yawDiff;

    if (DREG(53) != 0) {
        DREG(53) = 0;
        Audio_PlaySoundGeneral(NA_SE_EV_KID_HORSE_NEIGH, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        func_80A6A724(self);
    } else {
        self->actor.speedXZ = 0.0f;
        yawDiff = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.world.rot.y;
        // 0.7071 = cos(pi/4)
        if ((Math_CosS(yawDiff) < 0.7071f) && (self->animationIdx == 2)) {
            func_8006DD9C(&self->actor, &GET_PLAYER(globalCtx)->actor.world.pos, 300);
        }

        if (SkelAnime_Update(&self->skin.skelAnime)) {
            if (Math_CosS(yawDiff) < 0.0f) {
                self->animationIdx = 2;
                Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], D_80A6AF64[self->animationIdx],
                                 0.0f, Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
            } else {
                func_80A6A4DC(self);
            }
        }
    }
}

void func_80A6A724(EnHorseLinkChild* self) {
    self->timer = 0;
    self->action = 4;
    self->animationIdx = 2;
    self->unk_1E8 = false;
    self->actor.speedXZ = 2.0f;
    Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                     Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
}

void func_80A6A7D0(EnHorseLinkChild* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 dist;
    s32 newAnimationIdx;

    self->timer++;
    if (self->timer > 300) {
        self->unk_1E8 = true;
    }

    if ((self->animationIdx == 4) || (self->animationIdx == 3) || (self->animationIdx == 2)) {
        if (!self->unk_1E8) {
            func_8006DD9C(&self->actor, &player->actor.world.pos, 300);
        } else {
            func_8006DD9C(&self->actor, &self->actor.home.pos, 300);
        }
    }

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        if (!self->unk_1E8) {
            dist = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(globalCtx)->actor);
        } else {
            dist = Math3D_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.home.pos);
        }

        if (!self->unk_1E8) {
            if (dist >= 300.0f) {
                newAnimationIdx = 4;
                self->actor.speedXZ = 6.0f;
            } else if (dist >= 150.0f) {
                newAnimationIdx = 3;
                self->actor.speedXZ = 4.0f;
            } else {
                newAnimationIdx = 2;
                self->actor.speedXZ = 2.0f;
                self->unk_1F0 = 0;
            }
        } else {
            if (dist >= 300.0f) {
                newAnimationIdx = 4;
                self->actor.speedXZ = 6.0f;
            } else if (dist >= 150.0f) {
                newAnimationIdx = 3;
                self->actor.speedXZ = 4.0f;
            } else if (dist >= 70.0f) {
                newAnimationIdx = 2;
                self->actor.speedXZ = 2.0f;
                self->unk_1F0 = 0;
            } else {
                func_80A6A4DC(self);
                return;
            }
        }

        if (self->animationIdx != newAnimationIdx) {
            self->animationIdx = newAnimationIdx;
            Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                             Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, -5.0f);
        } else {
            Animation_Change(&self->skin.skelAnime, sAnimations[self->animationIdx], func_80A695A4(self), 0.0f,
                             Animation_GetLastFrame(sAnimations[self->animationIdx]), ANIMMODE_ONCE, 0.0f);
        }
    }
}

static EnHorseLinkChildActionFunc sActionFuncs[] = {
    func_80A698F4, func_80A69C18, func_80A699FC, func_80A6A068, func_80A6A7D0, func_80A6A5A4,
};

static void* sEyeTextures[] = { gChildEponaEyeOpenTex, gChildEponaEyeHalfTex, gChildEponaEyeCloseTex };
static u8 sEyeIndexOrder[] = { 0, 1, 2, 1 };

void EnHorseLinkChild_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseLinkChild* self = THIS;
    s32 pad;

    sActionFuncs[self->action](self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 55.0f, 100.0f, 0x1D);

    if ((globalCtx->sceneNum == SCENE_SPOT20) && (self->actor.world.pos.z < -2400.0f)) {
        self->actor.world.pos.z = -2400.0f;
    }

    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;

    if ((Rand_ZeroOne() < 0.025f) && (self->eyeTexIndex == 0)) {
        self->eyeTexIndex++;
    } else if (self->eyeTexIndex > 0) {
        self->eyeTexIndex++;
        if (self->eyeTexIndex >= ARRAY_COUNT(sEyeIndexOrder)) {
            self->eyeTexIndex = 0;
        }
    }

    Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
    func_80A6948C(self);
}

void func_80A6ABF8(Actor* thisx, GlobalContext* globalCtx, PSkinAwb* skin) {
    Vec3f center;
    Vec3f newCenter;
    EnHorseLinkChild* self = THIS;
    s32 i;

    for (i = 0; i < self->headCollider.count; i++) {
        center.x = self->headCollider.elements[i].dim.modelSphere.center.x;
        center.y = self->headCollider.elements[i].dim.modelSphere.center.y;
        center.z = self->headCollider.elements[i].dim.modelSphere.center.z;
        func_800A6408(skin, self->headCollider.elements[i].dim.limb, &center, &newCenter);
        self->headCollider.elements[i].dim.worldSphere.center.x = newCenter.x;
        self->headCollider.elements[i].dim.worldSphere.center.y = newCenter.y;
        self->headCollider.elements[i].dim.worldSphere.center.z = newCenter.z;
        self->headCollider.elements[i].dim.worldSphere.radius =
            self->headCollider.elements[i].dim.modelSphere.radius * self->headCollider.elements[i].dim.scale;
    }

    //! @bug see relevant comment in `EnHorse_SkinCallback1`
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->headCollider.base);
}

s32 func_80A6AD84(Actor* thisx, GlobalContext* globalCtx, s32 arg2, PSkinAwb* arg3) {
    EnHorseLinkChild* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_horse_link_child.c", 1467);

    if (arg2 == 0xD) {
        u8 index = sEyeIndexOrder[self->eyeTexIndex];
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[index]));
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_horse_link_child.c", 1479);

    return 1;
}

void EnHorseLinkChild_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseLinkChild* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    func_800A6360(&self->actor, globalCtx, &self->skin, func_80A6ABF8, func_80A6AD84, 1);
}
