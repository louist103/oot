/*
 * File: z_en_cow.c
 * Overlay: ovl_En_Cow
 * Description: Cow
 */

#include "z_en_cow.h"
#include "objects/object_cow/object_cow.h"

#define FLAGS 0x00000009

#define THIS ((EnCow*)thisx)

void EnCow_Init(Actor* thisx, GlobalContext* globalCtx);
void EnCow_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnCow_Update(Actor* thisx, GlobalContext* globalCtx);
void EnCow_Draw(Actor* thisx, GlobalContext* globalCtx);
void func_809DFE98(Actor* thisx, GlobalContext* globalCtx);
void func_809E0070(Actor* thisx, GlobalContext* globalCtx);

void func_809DF494(EnCow* self, GlobalContext* globalCtx);
void func_809DF6BC(EnCow* self, GlobalContext* globalCtx);
void func_809DF778(EnCow* self, GlobalContext* globalCtx);
void func_809DF7D8(EnCow* self, GlobalContext* globalCtx);
void func_809DF870(EnCow* self, GlobalContext* globalCtx);
void func_809DF8FC(EnCow* self, GlobalContext* globalCtx);
void func_809DF96C(EnCow* self, GlobalContext* globalCtx);
void func_809DFA84(EnCow* self, GlobalContext* globalCtx);

const ActorInit En_Cow_InitVars = {
    ACTOR_EN_COW,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_COW,
    sizeof(EnCow),
    (ActorFunc)EnCow_Init,
    (ActorFunc)EnCow_Destroy,
    (ActorFunc)EnCow_Update,
    (ActorFunc)EnCow_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
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
    { 30, 40, 0, { 0, 0, 0 } },
};

static Vec3f D_809E010C = { 0.0f, -1300.0f, 1100.0f };

void func_809DEE00(Vec3f* vec, s16 rotY) {
    f32 xCalc;
    f32 rotCalcTemp;

    rotCalcTemp = Math_CosS(rotY);
    xCalc = (Math_SinS(rotY) * vec->z) + (rotCalcTemp * vec->x);
    rotCalcTemp = Math_SinS(rotY);
    vec->z = (Math_CosS(rotY) * vec->z) + (-rotCalcTemp * vec->x);
    vec->x = xCalc;
}

void func_809DEE9C(EnCow* self) {
    Vec3f vec;

    vec.y = 0.0f;
    vec.x = 0.0f;
    vec.z = 30.0f;
    func_809DEE00(&vec, self->actor.shape.rot.y);
    self->colliders[0].dim.pos.x = self->actor.world.pos.x + vec.x;
    self->colliders[0].dim.pos.y = self->actor.world.pos.y;
    self->colliders[0].dim.pos.z = self->actor.world.pos.z + vec.z;

    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = -20.0f;
    func_809DEE00(&vec, self->actor.shape.rot.y);
    self->colliders[1].dim.pos.x = self->actor.world.pos.x + vec.x;
    self->colliders[1].dim.pos.y = self->actor.world.pos.y;
    self->colliders[1].dim.pos.z = self->actor.world.pos.z + vec.z;
}

void func_809DEF94(EnCow* self) {
    Vec3f vec;

    VEC_SET(vec, 0.0f, 57.0f, -36.0f);

    func_809DEE00(&vec, self->actor.shape.rot.y);
    self->actor.world.pos.x += vec.x;
    self->actor.world.pos.y += vec.y;
    self->actor.world.pos.z += vec.z;
}

void EnCow_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnCow* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 72.0f);
    switch (self->actor.params) {
        case 0:
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gCowBodySkel, NULL, self->jointTable, self->morphTable, 6);
            Animation_PlayLoop(&self->skelAnime, &gCowBodyChewAnim);
            Collider_InitCylinder(globalCtx, &self->colliders[0]);
            Collider_SetCylinder(globalCtx, &self->colliders[0], &self->actor, &sCylinderInit);
            Collider_InitCylinder(globalCtx, &self->colliders[1]);
            Collider_SetCylinder(globalCtx, &self->colliders[1], &self->actor, &sCylinderInit);
            func_809DEE9C(self);
            self->actionFunc = func_809DF96C;
            if (globalCtx->sceneNum == SCENE_LINK_HOME) {
                if (!LINK_IS_ADULT) {
                    Actor_Kill(&self->actor);
                    return;
                }
                if (!(gSaveContext.eventChkInf[1] & 0x4000)) {
                    Actor_Kill(&self->actor);
                    return;
                }
            }
            Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_COW, self->actor.world.pos.x,
                               self->actor.world.pos.y, self->actor.world.pos.z, 0, self->actor.shape.rot.y, 0, 1);
            self->unk_278 = Rand_ZeroFloat(1000.0f) + 40.0f;
            self->unk_27A = 0;
            self->actor.targetMode = 6;
            DREG(53) = 0;
            break;
        case 1:
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gCowTailSkel, NULL, self->jointTable, self->morphTable, 6);
            Animation_PlayLoop(&self->skelAnime, &gCowTailIdleAnim);
            self->actor.update = func_809DFE98;
            self->actor.draw = func_809E0070;
            self->actionFunc = func_809DFA84;
            func_809DEF94(self);
            self->actor.flags &= ~0x1;
            self->unk_278 = ((u32)(Rand_ZeroFloat(1000.0f)) & 0xFFFF) + 40.0f;
            break;
    }
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&self->actor, 0.01f);
    self->unk_276 = 0;
}

void EnCow_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnCow* self = THIS;

    if (self->actor.params == 0) {
        Collider_DestroyCylinder(globalCtx, &self->colliders[0]);
        Collider_DestroyCylinder(globalCtx, &self->colliders[1]);
    }
}

void func_809DF494(EnCow* self, GlobalContext* globalCtx) {
    if (self->unk_278 > 0) {
        self->unk_278 -= 1;
    } else {
        self->unk_278 = Rand_ZeroFloat(500.0f) + 40.0f;
        Animation_Change(&self->skelAnime, &gCowBodyChewAnim, 1.0f, self->skelAnime.curFrame,
                         Animation_GetLastFrame(&gCowBodyChewAnim), ANIMMODE_ONCE, 1.0f);
    }

    if ((self->actor.xzDistToPlayer < 150.0f) && (!(self->unk_276 & 2))) {
        self->unk_276 |= 2;
        if (self->skelAnime.animation == &gCowBodyChewAnim) {
            self->unk_278 = 0;
        }
    }

    self->unk_27A += 1;
    if (self->unk_27A >= 0x31) {
        self->unk_27A = 0;
    }

    // (1.0f / 100.0f) instead of 0.01f below is necessary so 0.01f doesn't get reused mistakenly
    if (self->unk_27A < 0x20) {
        self->actor.scale.x = ((Math_SinS(self->unk_27A << 0xA) * (1.0f / 100.0f)) + 1.0f) * 0.01f;
    } else {
        self->actor.scale.x = 0.01f;
    }

    if (self->unk_27A >= 0x11) {
        self->actor.scale.y = ((Math_SinS((self->unk_27A << 0xA) - 0x4000) * (1.0f / 100.0f)) + 1.0f) * 0.01f;
    } else {
        self->actor.scale.y = 0.01f;
    }
}

void func_809DF6BC(EnCow* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        self->actor.flags &= ~0x10000;
        func_80106CCC(globalCtx);
        self->actionFunc = func_809DF96C;
    }
}

void func_809DF730(EnCow* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actor.flags &= ~0x10000;
        self->actionFunc = func_809DF96C;
    }
}

void func_809DF778(EnCow* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = func_809DF730;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_MILK, 10000.0f, 100.0f);
    }
}

void func_809DF7D8(EnCow* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        self->actor.flags &= ~0x10000;
        func_80106CCC(globalCtx);
        self->actionFunc = func_809DF778;
        func_8002F434(&self->actor, globalCtx, GI_MILK, 10000.0f, 100.0f);
    }
}

void func_809DF870(EnCow* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        if (Inventory_HasEmptyBottle()) {
            func_8010B720(globalCtx, 0x2007);
            self->actionFunc = func_809DF7D8;
        } else {
            func_8010B720(globalCtx, 0x2013);
            self->actionFunc = func_809DF6BC;
        }
    }
}

void func_809DF8FC(EnCow* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = func_809DF870;
    } else {
        self->actor.flags |= 0x10000;
        func_8002F2CC(&self->actor, globalCtx, 170.0f);
        self->actor.textId = 0x2006;
    }
    func_809DF494(self, globalCtx);
}

void func_809DF96C(EnCow* self, GlobalContext* globalCtx) {
    if ((globalCtx->msgCtx.unk_E3EE == 0) || (globalCtx->msgCtx.unk_E3EE == 4)) {
        if (DREG(53) != 0) {
            if (self->unk_276 & 4) {
                self->unk_276 &= ~0x4;
                DREG(53) = 0;
            } else {
                if ((self->actor.xzDistToPlayer < 150.0f) &&
                    (ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) < 0x61A8)) {
                    DREG(53) = 0;
                    self->actionFunc = func_809DF8FC;
                    self->actor.flags |= 0x10000;
                    func_8002F2CC(&self->actor, globalCtx, 170.0f);
                    self->actor.textId = 0x2006;
                } else {
                    self->unk_276 |= 4;
                }
            }
        } else {
            self->unk_276 &= ~0x4;
        }
    }
    func_809DF494(self, globalCtx);
}

void func_809DFA84(EnCow* self, GlobalContext* globalCtx) {
    if (self->unk_278 > 0) {
        self->unk_278--;
    } else {
        self->unk_278 = Rand_ZeroFloat(200.0f) + 40.0f;
        Animation_Change(&self->skelAnime, &gCowTailIdleAnim, 1.0f, self->skelAnime.curFrame,
                         Animation_GetLastFrame(&gCowTailIdleAnim), ANIMMODE_ONCE, 1.0f);
    }

    if ((self->actor.xzDistToPlayer < 150.0f) &&
        (ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) >= 0x61A9) && (!(self->unk_276 & 2))) {
        self->unk_276 |= 2;
        if (self->skelAnime.animation == &gCowTailIdleAnim) {
            self->unk_278 = 0;
        }
    }
}

void EnCow_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnCow* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    s16 targetX;
    s16 targetY;
    Player* player = GET_PLAYER(globalCtx);

    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliders[0].base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliders[1].base);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(globalCtx, thisx, 0.0f, 0.0f, 0.0f, 4);
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        if (self->skelAnime.animation == &gCowBodyChewAnim) {
            Audio_PlayActorSound2(thisx, NA_SE_EV_COW_CRY);
            Animation_Change(&self->skelAnime, &gCowBodyMoveHeadAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gCowBodyMoveHeadAnim), ANIMMODE_ONCE, 1.0f);
        } else {
            Animation_Change(&self->skelAnime, &gCowBodyChewAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gCowBodyChewAnim),
                             ANIMMODE_LOOP, 1.0f);
        }
    }
    self->actionFunc(self, globalCtx);
    if ((thisx->xzDistToPlayer < 150.0f) &&
        (ABS(Math_Vec3f_Yaw(&thisx->world.pos, &player->actor.world.pos)) < 0xC000)) {
        targetX = Math_Vec3f_Pitch(&thisx->focus.pos, &player->actor.focus.pos);
        targetY = Math_Vec3f_Yaw(&thisx->focus.pos, &player->actor.focus.pos) - thisx->shape.rot.y;

        if (targetX > 0x1000) {
            targetX = 0x1000;
        } else if (targetX < -0x1000) {
            targetX = -0x1000;
        }

        if (targetY > 0x2500) {
            targetY = 0x2500;
        } else if (targetY < -0x2500) {
            targetY = -0x2500;
        }

    } else {
        targetY = 0;
        targetX = 0;
    }
    Math_SmoothStepToS(&self->someRot.x, targetX, 0xA, 0xC8, 0xA);
    Math_SmoothStepToS(&self->someRot.y, targetY, 0xA, 0xC8, 0xA);
}

void func_809DFE98(Actor* thisx, GlobalContext* globalCtx) {
    EnCow* self = THIS;
    s32 pad;

    if (SkelAnime_Update(&self->skelAnime) != 0) {
        if (self->skelAnime.animation == &gCowTailIdleAnim) {
            Animation_Change(&self->skelAnime, &gCowTailSwishAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gCowTailSwishAnim), ANIMMODE_ONCE, 1.0f);
        } else {
            Animation_Change(&self->skelAnime, &gCowTailIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gCowTailIdleAnim),
                             ANIMMODE_LOOP, 1.0f);
        }
    }
    self->actionFunc(self, globalCtx);
}

s32 EnCow_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnCow* self = THIS;

    if (limbIndex == 2) {
        rot->y += self->someRot.y;
        rot->x += self->someRot.x;
    }
    if (limbIndex == 5) {
        *dList = NULL;
    }
    return false;
}

void EnCow_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnCow* self = THIS;

    if (limbIndex == 2) {
        Matrix_MultVec3f(&D_809E010C, &self->actor.focus.pos);
    }
}

void EnCow_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnCow* self = THIS;

    func_800943C8(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnCow_OverrideLimbDraw, EnCow_PostLimbDraw, self);
}

void func_809E0070(Actor* thisx, GlobalContext* globalCtx) {
    EnCow* self = THIS;

    func_800943C8(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);
}
