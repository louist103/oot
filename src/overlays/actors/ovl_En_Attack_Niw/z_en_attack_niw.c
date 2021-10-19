/*
 * File: z_en_attack_niw.c
 * Overlay: ovl_En_Attack_Niw
 * Description: Attacking Cucco, not solid
 */

#include "z_en_attack_niw.h"
#include "objects/object_niw/object_niw.h"
#include "overlays/actors/ovl_En_Niw/z_en_niw.h"

#define FLAGS 0x00000010

#define THIS ((EnAttackNiw*)thisx)

void EnAttackNiw_Init(Actor* thisx, GlobalContext* globalCtx);
void EnAttackNiw_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnAttackNiw_Update(Actor* thisx, GlobalContext* globalCtx);
void EnAttackNiw_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809B5670(EnAttackNiw* self, GlobalContext* globalCtx);
void func_809B5C18(EnAttackNiw* self, GlobalContext* globalCtx);
void func_809B59B0(EnAttackNiw* self, GlobalContext* globalCtx);

const ActorInit En_Attack_Niw_InitVars = {
    ACTOR_EN_ATTACK_NIW,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_NIW,
    sizeof(EnAttackNiw),
    (ActorFunc)EnAttackNiw_Init,
    (ActorFunc)EnAttackNiw_Destroy,
    (ActorFunc)EnAttackNiw_Update,
    (ActorFunc)EnAttackNiw_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 1, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 0, ICHAIN_STOP),
};

void EnAttackNiw_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnAttackNiw* self = THIS;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gCuccoSkel, &gCuccoAnim, self->jointTable, self->morphTable, 16);
    if (self->actor.params < 0) {
        self->actor.params = 0;
    }
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.gravity = 0.0f;
    self->unk_298.x = Rand_CenteredFloat(100.0f);
    self->unk_298.y = Rand_CenteredFloat(10.0f);
    self->unk_298.z = Rand_CenteredFloat(100.0f);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.flags &= ~1;
    self->actor.shape.rot.y = self->actor.world.rot.y = (Rand_ZeroOne() - 0.5f) * 60000.0f;
    self->actionFunc = func_809B5670;
}

void EnAttackNiw_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnAttackNiw* self = THIS;
    EnNiw* cucco = (EnNiw*)self->actor.parent;

    if (self->actor.parent != NULL) {
        if ((cucco->actor.update != NULL) && (cucco->unk_296 > 0)) {
            cucco->unk_296--;
        }
    }
}

void func_809B5268(EnAttackNiw* self, GlobalContext* globalCtx, s16 arg2) {
    if (self->unk_254 == 0) {
        if (arg2 == 0) {
            self->unk_264 = 0.0f;
        } else {
            self->unk_264 = -10000.0f;
        }
        self->unk_28E++;
        self->unk_254 = 3;
        if ((self->unk_28E & 1) == 0) {
            self->unk_264 = 0.0f;
            if (arg2 == 0) {
                self->unk_254 = Rand_ZeroFloat(30.0f);
            }
        }
    }

    if (self->unk_258 == 0) {
        self->unk_292++;
        self->unk_292 &= 1;
        switch (arg2) {
            case 0:
                self->unk_26C = 0.0f;
                self->unk_268 = 0.0f;
                break;
            case 1:
                self->unk_258 = 3;
                self->unk_26C = 7000.0f;
                self->unk_268 = 7000.0f;
                if (self->unk_292 == 0) {
                    self->unk_26C = 0.0f;
                    self->unk_268 = 0.0f;
                }
                break;
            case 2:
                self->unk_258 = 2;
                self->unk_268 = self->unk_26C = -10000.0f;
                self->unk_280 = self->unk_278 = 25000.0f;
                self->unk_284 = self->unk_27C = 6000.0f;
                if (self->unk_292 == 0) {
                    self->unk_278 = 8000.0f;
                    self->unk_280 = 8000.0f;
                }
                break;
            case 3:
                self->unk_258 = 2;
                self->unk_278 = 10000.0f;
                self->unk_280 = 10000.0f;
                if (self->unk_292 == 0) {
                    self->unk_278 = 3000.0f;
                    self->unk_280 = 3000.0f;
                }
                break;
            case 4:
                self->unk_254 = self->unk_256 = 5;
                break;
            case 5:
                self->unk_258 = 5;
                self->unk_278 = 14000.0f;
                self->unk_280 = 14000.0f;
                if (self->unk_292 == 0) {
                    self->unk_278 = 10000.0f;
                    self->unk_280 = 10000.0f;
                }
                break;
        }
    }

    if (self->unk_288 != self->unk_2C0) {
        Math_ApproachF(&self->unk_2C0, self->unk_288, 0.5f, 4000.0f);
    }
    if (self->unk_264 != self->unk_2BC) {
        Math_ApproachF(&self->unk_2BC, self->unk_264, 0.5f, 4000.0f);
    }
    if (self->unk_26C != self->unk_2A4.x) {
        Math_ApproachF(&self->unk_2A4.x, self->unk_26C, 0.8f, 7000.0f);
    }
    if (self->unk_280 != self->unk_2A4.y) {
        Math_ApproachF(&self->unk_2A4.y, self->unk_280, 0.8f, 7000.0f);
    }
    if (self->unk_284 != self->unk_2A4.z) {
        Math_ApproachF(&self->unk_2A4.z, self->unk_284, 0.8f, 7000.0f);
    }
    if (self->unk_268 != self->unk_2B0.x) {
        Math_ApproachF(&self->unk_2B0.x, self->unk_268, 0.8f, 7000.0f);
    }
    if (self->unk_278 != self->unk_2B0.y) {
        Math_ApproachF(&self->unk_2B0.y, self->unk_278, 0.8f, 7000.0f);
    }
    if (self->unk_27C != self->unk_2B0.z) {
        Math_ApproachF(&self->unk_2B0.z, self->unk_27C, 0.8f, 7000.0f);
    }
}

s32 func_809B55EC(EnAttackNiw* self, GlobalContext* globalCtx) {
    s16 sp1E;
    s16 sp1C;

    Actor_SetFocus(&self->actor, self->unk_2E4);
    func_8002F374(globalCtx, &self->actor, &sp1E, &sp1C);
    if ((self->actor.projectedPos.z < -20.0f) || (sp1E < 0) || (sp1E > SCREEN_WIDTH) || (sp1C < 0) ||
        (sp1C > SCREEN_HEIGHT)) {
        return 0;
    } else {
        return 1;
    }
}

void func_809B5670(EnAttackNiw* self, GlobalContext* globalCtx) {
    s16 sp4E;
    s16 sp4C;
    f32 tmpf1;
    f32 tmpf2;
    f32 tmpf3;
    Vec3f sp34;

    self->actor.speedXZ = 10.0f;

    tmpf1 = (self->unk_298.x + globalCtx->view.lookAt.x) - globalCtx->view.eye.x;
    tmpf2 = (self->unk_298.y + globalCtx->view.lookAt.y) - globalCtx->view.eye.y;
    tmpf3 = (self->unk_298.z + globalCtx->view.lookAt.z) - globalCtx->view.eye.z;

    sp34.x = globalCtx->view.lookAt.x + tmpf1;
    sp34.y = globalCtx->view.lookAt.y + tmpf2;
    sp34.z = globalCtx->view.lookAt.z + tmpf3;

    self->unk_2D4 = Math_Vec3f_Yaw(&self->actor.world.pos, &sp34);
    self->unk_2D0 = Math_Vec3f_Pitch(&self->actor.world.pos, &sp34) * -1.0f;

    Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_2D4, 5, self->unk_2DC, 0);
    Math_SmoothStepToS(&self->actor.world.rot.x, self->unk_2D0, 5, self->unk_2DC, 0);
    Math_ApproachF(&self->unk_2DC, 5000.0f, 1.0f, 100.0f);

    Actor_SetFocus(&self->actor, self->unk_2E4);
    func_8002F374(globalCtx, &self->actor, &sp4E, &sp4C);

    if (self->actor.bgCheckFlags & 8) {
        self->unk_2D4 = self->actor.yawTowardsPlayer;
        self->unk_2D0 = self->actor.world.rot.x - 3000.0f;
        self->unk_2DC = 0.0f;
        self->unk_284 = 0.0f;
        self->unk_27C = 0.0f;
        self->unk_254 = self->unk_256 = self->unk_258 = self->unk_25A = 0;
        self->unk_25C = 0x64;
        self->actor.gravity = -0.2f;
        self->unk_2E0 = 5.0f;
        self->unk_288 = 0.0f;
        self->actionFunc = func_809B59B0;
    } else if (((self->actor.projectedPos.z > 0.0f) && (fabsf(sp34.x - self->actor.world.pos.x) < 50.0f) &&
                (fabsf(sp34.y - self->actor.world.pos.y) < 50.0f) &&
                (fabsf(sp34.z - self->actor.world.pos.z) < 50.0f)) ||
               (self->actor.bgCheckFlags & 1)) {

        self->unk_2D4 = self->actor.yawTowardsPlayer;
        self->unk_2D0 = self->actor.world.rot.x - 2000.0f;
        self->unk_2DC = 0.0f;
        self->unk_27C = 0.0f;
        self->unk_284 = 0.0f;
        self->unk_254 = self->unk_256 = self->unk_258 = self->unk_25A = 0;
        self->actor.gravity = -0.2f;
        self->unk_2E0 = 5.0f;
        self->unk_288 = 0.0f;
        self->actionFunc = func_809B59B0;
    } else {
        self->unk_254 = 10;
        self->unk_264 = -10000.0f;
        self->unk_288 = -3000.0f;
        func_809B5268(self, globalCtx, 2);
    }
}

void func_809B59B0(EnAttackNiw* self, GlobalContext* globalCtx) {
    if (!func_809B55EC(self, globalCtx)) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->actor.bgCheckFlags & 1) {
        if (self->unk_25A == 0) {
            self->unk_25A = 3;
            self->actor.velocity.y = 3.5f;
        }
        if (self->actor.gravity != -2.0f) {
            self->unk_280 = self->unk_278 = 14000.0f;
            self->unk_2D0 = self->unk_26C = self->unk_268 = self->unk_284 = self->unk_27C = 0.0f;
            self->unk_2D4 = self->actor.yawTowardsPlayer;
            self->unk_262 = 0x32;
            self->unk_25C = 0x64;
            self->actor.gravity = -2.0f;
        }
    }
    if (self->unk_25C == 0x32) {
        self->unk_2D4 = Rand_CenteredFloat(200.0f) + self->actor.yawTowardsPlayer;
    }
    Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_2D4, 2, self->unk_2DC, 0);
    Math_SmoothStepToS(&self->actor.world.rot.x, self->unk_2D0, 2, self->unk_2DC, 0);
    Math_ApproachF(&self->unk_2DC, 10000.0f, 1.0f, 1000.0f);
    Math_ApproachF(&self->actor.speedXZ, self->unk_2E0, 0.9f, 1.0f);
    if ((self->actor.gravity == -2.0f) && (self->unk_262 == 0) &&
        ((self->actor.bgCheckFlags & 8) || (self->unk_25C == 0))) {
        self->unk_2E0 = 0.0f;
        self->actor.gravity = 0.0f;
        self->unk_2DC = 0.0f;
        self->unk_2D0 = self->actor.world.rot.x - 5000.0f;
        self->actionFunc = func_809B5C18;
    } else if (self->actor.bgCheckFlags & 1) {
        func_809B5268(self, globalCtx, 5);
    } else {
        func_809B5268(self, globalCtx, 2);
    }
}

void func_809B5C18(EnAttackNiw* self, GlobalContext* globalCtx) {
    if (!func_809B55EC(self, globalCtx)) {
        Actor_Kill(&self->actor);
        return;
    }
    Math_SmoothStepToS(&self->actor.world.rot.x, self->unk_2D0, 5, self->unk_2DC, 0);
    Math_ApproachF(&self->unk_2DC, 5000.0f, 1.0f, 100.0f);
    Math_ApproachF(&self->actor.velocity.y, 5.0f, 0.3f, 1.0f);
    func_809B5268(self, globalCtx, 2);
}

void EnAttackNiw_Update(Actor* thisx, GlobalContext* globalCtx) {
    f32 tmpf1;
    EnAttackNiw* self = THIS;
    EnNiw* cucco;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    Vec3f sp30;
    GlobalContext* globalCtx2 = globalCtx;

    self->unk_28C++;
    if (self->unk_254 != 0) {
        self->unk_254--;
    }
    if (self->unk_258 != 0) {
        self->unk_258--;
    }
    if (self->unk_25A != 0) {
        self->unk_25A--;
    }
    if (self->unk_25E != 0) {
        self->unk_25E--;
    }
    if (self->unk_260 != 0) {
        self->unk_260--;
    }
    if (self->unk_25C != 0) {
        self->unk_25C--;
    }
    if (self->unk_262 != 0) {
        self->unk_262--;
    }

    self->actor.shape.rot = self->actor.world.rot;
    self->actor.shape.shadowScale = 15.0f;
    self->actionFunc(self, globalCtx2);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 60.0f, 0x1D);

    if (self->actionFunc == func_809B5670) {
        func_8002D97C(&self->actor);
    } else {
        Actor_MoveForward(&self->actor);
    }

    if (self->actor.floorHeight <= BGCHECK_Y_MIN) {
        Actor_Kill(&self->actor);
        return;
    }

    if ((self->actor.bgCheckFlags & 0x20) && (self->actionFunc != func_809B5C18)) {
        Math_Vec3f_Copy(&sp30, &self->actor.world.pos);
        sp30.y += self->actor.yDistToWater;
        EffectSsGSplash_Spawn(globalCtx, &sp30, 0, 0, 0, 0x190);
        self->unk_2DC = 0.0f;
        self->actor.gravity = 0.0f;
        self->unk_2E0 = 0.0f;
        self->unk_2D0 = self->actor.world.rot.x - 5000.0f;
        self->actionFunc = func_809B5C18;
        return;
    }

    tmpf1 = 20.0f;
    if (self->actor.xyzDistToPlayerSq < SQ(tmpf1)) {
        cucco = (EnNiw*)self->actor.parent;
        if ((self->actor.parent->update != NULL) && (self->actor.parent != NULL) && (cucco != NULL) &&
            (cucco->timer9 == 0) && (player->invincibilityTimer == 0)) {
            func_8002F6D4(globalCtx, &self->actor, 2.0f, self->actor.world.rot.y, 0.0f, 0x10);
            cucco->timer9 = 0x46;
        }
    }
    if (self->unk_25E == 0) {
        self->unk_25E = 30;
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_CHICKEN_CRY_A);
    }
    if (self->unk_260 == 0) {
        self->unk_260 = 7;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_WAKEUP);
    }
}

s32 func_809B5F98(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnAttackNiw* self = THIS;
    Vec3f sp0 = { 0.0f, 0.0f, 0.0f };

    if (limbIndex == 13) {
        rot->y += (s16)self->unk_2BC;
    }
    if (limbIndex == 15) {
        rot->z += (s16)self->unk_2C0;
    }
    if (limbIndex == 11) {
        rot->x += (s16)self->unk_2B0.z;
        rot->y += (s16)self->unk_2B0.y;
        rot->z += (s16)self->unk_2B0.x;
    }
    if (limbIndex == 7) {
        rot->x += (s16)self->unk_2A4.z;
        rot->y += (s16)self->unk_2A4.y;
        rot->z += (s16)self->unk_2A4.x;
    }
    return 0;
}

void EnAttackNiw_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnAttackNiw* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          func_809B5F98, NULL, self);
}
