/*
 * File: z_en_fw.c
 * Overlay: ovl_En_Fw
 * Description: Flare Dancer Core
 */

#include "z_en_fw.h"
#include "objects/object_fw/object_fw.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000215

#define THIS ((EnFw*)thisx)

void EnFw_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFw_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFw_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFw_Draw(Actor* thisx, GlobalContext* globalCtx);
void EnFw_UpdateDust(EnFw* self);
void EnFw_DrawDust(EnFw* self, GlobalContext* globalCtx);
void EnFw_AddDust(EnFw* self, Vec3f* initialPos, Vec3f* initialSpeed, Vec3f* accel, u8 initialTimer, f32 scale,
                  f32 scaleStep);
void EnFw_Bounce(EnFw* self, GlobalContext* globalCtx);
void EnFw_Run(EnFw* self, GlobalContext* globalCtx);
void EnFw_JumpToParentInitPos(EnFw* self, GlobalContext* globalCtx);
void EnFw_TurnToParentInitPos(EnFw* self, GlobalContext* globalCtx);

const ActorInit En_Fw_InitVars = {
    ACTOR_EN_FW,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_FW,
    sizeof(EnFw),
    (ActorFunc)EnFw_Init,
    (ActorFunc)EnFw_Destroy,
    (ActorFunc)EnFw_Update,
    (ActorFunc)EnFw_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x04 },
            { 0xFFCFFFFE, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { 2, { { 1200, 0, 0 }, 16 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static CollisionCheckInfoInit2 D_80A1FB94 = { 8, 2, 25, 25, MASS_IMMOVABLE };

static struct_80034EC0_Entry D_80A1FBA0[] = {
    { &gFlareDancerCoreInitRunCycleAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, 0.0f },
    { &gFlareDancerCoreRunCycleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -8.0f },
    { &gFlareDancerCoreEndRunCycleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
};

s32 EnFw_DoBounce(EnFw* self, s32 totalBounces, f32 yVelocity) {
    s16 temp_v1;

    if (!(self->actor.bgCheckFlags & 1) || (self->actor.velocity.y > 0.0f)) {
        // not on the ground or moving upwards.
        return false;
    }

    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
    self->bounceCnt--;
    if (self->bounceCnt <= 0) {
        if (self->bounceCnt == 0) {
            self->bounceCnt = 0;
            self->actor.velocity.y = 0.0f;
            return true;
        }
        self->bounceCnt = totalBounces;
    }
    self->actor.velocity.y = yVelocity;
    self->actor.velocity.y *= ((f32)self->bounceCnt / totalBounces);
    return 1;
}

s32 EnFw_PlayerInRange(EnFw* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    CollisionPoly* poly;
    s32 bgId;
    Vec3f collisionPos;

    if (self->actor.xzDistToPlayer > 300.0f) {
        return false;
    }

    if (ABS((s16)((f32)self->actor.yawTowardsPlayer - (f32)self->actor.shape.rot.y)) > 0x1C70) {
        return false;
    }

    if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &self->actor.world.pos, &player->actor.world.pos, &collisionPos,
                                &poly, true, false, false, true, &bgId)) {
        return false;
    }

    return true;
}

Vec3f* EnFw_GetPosAdjAroundCircle(Vec3f* dst, EnFw* self, f32 radius, s16 dir) {
    s16 angle;
    Vec3f posAdj;

    // increase rotation around circle ~30 degrees.
    angle = Math_Vec3f_Yaw(&self->actor.parent->home.pos, &self->actor.world.pos) + (dir * 0x1554);
    posAdj.x = (Math_SinS(angle) * radius) + self->actor.parent->home.pos.x;
    posAdj.z = (Math_CosS(angle) * radius) + self->actor.parent->home.pos.z;
    posAdj.x -= self->actor.world.pos.x;
    posAdj.z -= self->actor.world.pos.z;
    *dst = posAdj;
    return dst;
}

s32 EnFw_CheckCollider(EnFw* self, GlobalContext* globalCtx) {
    ColliderInfo* info;
    s32 phi_return;

    if (self->collider.base.acFlags & AC_HIT) {
        info = &self->collider.elements[0].info;
        if (info->acHitInfo->toucher.dmgFlags & 0x80) {
            self->lastDmgHook = true;
        } else {
            self->lastDmgHook = false;
        }
        self->collider.base.acFlags &= ~AC_HIT;
        if (Actor_ApplyDamage(&self->actor) <= 0) {
            if (self->actor.parent->colChkInfo.health <= 8) {
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                self->actor.parent->colChkInfo.health = 0;
            } else {
                self->actor.parent->colChkInfo.health -= 8;
            }
            self->returnToParentTimer = 0;
        }
        return true;
    } else {
        return false;
    }
}

s32 EnFw_SpawnDust(EnFw* self, u8 timer, f32 scale, f32 scaleStep, s32 dustCnt, f32 radius, f32 xzAccel, f32 yAccel) {
    Vec3f pos = { 0.0f, 0.0f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    s16 angle;
    s32 i;

    pos = self->actor.world.pos;
    pos.y = self->actor.floorHeight + 2.0f;
    angle = ((Rand_ZeroOne() - 0.5f) * 0x10000);
    i = dustCnt;
    while (i >= 0) {
        accel.x = (Rand_ZeroOne() - 0.5f) * xzAccel;
        accel.y = yAccel;
        accel.z = (Rand_ZeroOne() - 0.5f) * xzAccel;
        pos.x = (Math_SinS(angle) * radius) + self->actor.world.pos.x;
        pos.z = (Math_CosS(angle) * radius) + self->actor.world.pos.z;
        EnFw_AddDust(self, &pos, &velocity, &accel, timer, scale, scaleStep);
        angle += (s16)(0x10000 / dustCnt);
        i--;
    }
    return 0;
}

void EnFw_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFw* self = THIS;

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gFlareDancerCoreSkel, NULL, self->jointTable, self->morphTable,
                       11);
    func_80034EC0(&self->skelAnime, D_80A1FBA0, 0);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->sphs);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(0x10), &D_80A1FB94);
    Actor_SetScale(&self->actor, 0.01f);
    self->runDirection = -self->actor.params;
    self->actionFunc = EnFw_Bounce;
    self->actor.gravity = -1.0f;
}

void EnFw_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFw* self = THIS;
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnFw_Bounce(EnFw* self, GlobalContext* globalCtx) {
    if (EnFw_DoBounce(self, 3, 8.0f) && self->bounceCnt == 0) {
        self->returnToParentTimer = Rand_S16Offset(300, 150);
        self->actionFunc = EnFw_Run;
    }
}

void EnFw_Run(EnFw* self, GlobalContext* globalCtx) {
    f32 tmpAngle;
    s16 phi_v0;
    f32 facingDir;
    EnBom* bomb;
    Actor* flareDancer;

    Math_SmoothStepToF(&self->skelAnime.playSpeed, 1.0f, 0.1f, 1.0f, 0.0f);
    if (self->skelAnime.animation == &gFlareDancerCoreInitRunCycleAnim) {
        if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame) == 0) {
            self->runRadius = Math_Vec3f_DistXYZ(&self->actor.world.pos, &self->actor.parent->world.pos);
            func_80034EC0(&self->skelAnime, D_80A1FBA0, 2);
        }
        return;
    }

    if (self->damageTimer == 0 && self->explosionTimer == 0 && EnFw_CheckCollider(self, globalCtx)) {
        if (self->actor.parent->colChkInfo.health > 0) {
            if (!self->lastDmgHook) {
                self->actor.velocity.y = 6.0f;
            }
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_MAN_DAMAGE);
            self->damageTimer = 20;
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_MAN_DAMAGE);
            self->explosionTimer = 6;
        }
        self->actor.speedXZ = 0.0f;
    }

    if (self->explosionTimer != 0) {
        self->skelAnime.playSpeed = 0.0f;
        Math_SmoothStepToF(&self->actor.scale.x, 0.024999999f, 0.08f, 0.6f, 0.0f);
        Actor_SetScale(&self->actor, self->actor.scale.x);
        if (self->actor.colorFilterTimer == 0) {
            Actor_SetColorFilter(&self->actor, 0x4000, 0xC8, 0, self->explosionTimer);
            self->explosionTimer--;
        }

        if (self->explosionTimer == 0) {
            bomb = (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, self->bompPos.x, self->bompPos.y,
                                       self->bompPos.z, 0, 0, 0x600, 0);
            if (bomb != NULL) {
                bomb->timer = 0;
            }
            flareDancer = self->actor.parent;
            flareDancer->params |= 0x4000;
            Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, 0xA0);
            Actor_Kill(&self->actor);
            return;
        }
    } else {
        if (!(self->actor.bgCheckFlags & 1) || self->actor.velocity.y > 0.0f) {
            Actor_SetColorFilter(&self->actor, 0x4000, 0xC8, 0, self->damageTimer);
            return;
        }
        DECR(self->damageTimer);
        if ((200.0f - self->runRadius) < 0.9f) {
            if (DECR(self->returnToParentTimer) == 0) {
                self->actor.speedXZ = 0.0f;
                self->actionFunc = EnFw_TurnToParentInitPos;
                return;
            }
        }

        // Run outwards until the radius of the run circle is 200
        Math_SmoothStepToF(&self->runRadius, 200.0f, 0.3f, 100.0f, 0.0f);

        if (self->turnAround) {
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 1.0f, 0.0f);
            tmpAngle = (s16)(self->actor.world.rot.y ^ 0x8000);
            facingDir = self->actor.shape.rot.y;
            tmpAngle = Math_SmoothStepToF(&facingDir, tmpAngle, 0.1f, 10000.0f, 0.0f);
            self->actor.shape.rot.y = facingDir;
            if (tmpAngle > 0x1554) {
                return;
            }
            self->turnAround = false;
        } else {
            Vec3f sp48;
            EnFw_GetPosAdjAroundCircle(&sp48, self, self->runRadius, self->runDirection);
            Math_SmoothStepToS(&self->actor.shape.rot.y, (Math_FAtan2F(sp48.x, sp48.z) * (0x8000 / M_PI)), 4, 0xFA0, 1);
        }

        self->actor.world.rot = self->actor.shape.rot;

        if (self->slideTimer == 0 && EnFw_PlayerInRange(self, globalCtx)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_MAN_SURP);
            self->slideSfxTimer = 8;
            self->slideTimer = 8;
        }

        if (self->slideTimer != 0) {
            if (DECR(self->slideSfxTimer) == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_MAN_SLIDE);
                self->slideSfxTimer = 4;
            }
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 1.0f, 0.0f);
            self->skelAnime.playSpeed = 0.0f;
            EnFw_SpawnDust(self, 8, 0.16f, 0.2f, 3, 8.0f, 20.0f, ((Rand_ZeroOne() - 0.5f) * 0.2f) + 0.3f);
            self->slideTimer--;
            if (self->slideTimer == 0) {
                self->turnAround = true;
                self->runDirection = -self->runDirection;
            }
        } else {
            Math_SmoothStepToF(&self->actor.speedXZ, 6.0f, 0.1f, 1.0f, 0.0f);
            phi_v0 = self->skelAnime.curFrame;
            if (phi_v0 == 1 || phi_v0 == 4) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FLAME_MAN_RUN);
                EnFw_SpawnDust(self, 8, 0.16f, 0.1f, 1, 0.0f, 20.0f, 0.0f);
            }
        }
    }
}

void EnFw_TurnToParentInitPos(EnFw* self, GlobalContext* globalCtx) {
    s16 angleToParentInit;

    angleToParentInit = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.parent->home.pos);
    Math_SmoothStepToS(&self->actor.shape.rot.y, angleToParentInit, 4, 0xFA0, 1);
    if (ABS(angleToParentInit - self->actor.shape.rot.y) < 0x65) {
        // angle to parent init pos is ~0.5 degrees
        self->actor.world.rot = self->actor.shape.rot;
        self->actor.velocity.y = 14.0f;
        self->actor.home.pos = self->actor.world.pos;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
        func_80034EC0(&self->skelAnime, D_80A1FBA0, 1);
        self->actionFunc = EnFw_JumpToParentInitPos;
    }
}

void EnFw_JumpToParentInitPos(EnFw* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 1 && self->actor.velocity.y <= 0.0f) {
        self->actor.parent->params |= 0x8000;
        Actor_Kill(&self->actor);
    } else {
        Math_SmoothStepToF(&self->actor.world.pos.x, self->actor.parent->home.pos.x, 0.6f, 8.0f, 0.0f);
        Math_SmoothStepToF(&self->actor.world.pos.z, self->actor.parent->home.pos.z, 0.6f, 8.0f, 0.0f);
    }
}

void EnFw_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnFw* self = THIS;
    SkelAnime_Update(&self->skelAnime);
    if ((self->actor.flags & 0x2000) != 0x2000) {
        // not attached to hookshot.
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 20.0f, 0.0f, 5);
        self->actionFunc(self, globalCtx);
        if (self->damageTimer == 0 && self->explosionTimer == 0 && self->actionFunc == EnFw_Run) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

s32 EnFw_OverrideLimbDraw(GlobalContext* globalContext, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                          void* thisx) {
    return false;
}

void EnFw_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnFw* self = THIS;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    if (limbIndex == 2) {
        // body
        Matrix_MultVec3f(&zeroVec, &self->bompPos);
    }

    if (limbIndex == 3) {
        // head
        Matrix_MultVec3f(&zeroVec, &self->actor.focus.pos);
    }

    Collider_UpdateSpheres(limbIndex, &self->collider);
}

void EnFw_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnFw* self = THIS;

    EnFw_UpdateDust(self);
    Matrix_Push();
    EnFw_DrawDust(self, globalCtx);
    Matrix_Pop();
    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnFw_OverrideLimbDraw, EnFw_PostLimbDraw, self);
}

void EnFw_AddDust(EnFw* self, Vec3f* initialPos, Vec3f* initialSpeed, Vec3f* accel, u8 initialTimer, f32 scale,
                  f32 scaleStep) {
    EnFwEffect* eff = self->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(self->effects); i++, eff++) {
        if (eff->type != 1) {
            eff->scale = scale;
            eff->scaleStep = scaleStep;
            eff->initialTimer = eff->timer = initialTimer;
            eff->type = 1;
            eff->pos = *initialPos;
            eff->accel = *accel;
            eff->velocity = *initialSpeed;
            return;
        }
    }
}

void EnFw_UpdateDust(EnFw* self) {
    EnFwEffect* eff = self->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(self->effects); i++, eff++) {
        if (eff->type != 0) {
            if ((--eff->timer) == 0) {
                eff->type = 0;
            }
            eff->accel.x = (Rand_ZeroOne() * 0.4f) - 0.2f;
            eff->accel.z = (Rand_ZeroOne() * 0.4f) - 0.2f;
            eff->pos.x += eff->velocity.x;
            eff->pos.y += eff->velocity.y;
            eff->pos.z += eff->velocity.z;
            eff->velocity.x += eff->accel.x;
            eff->velocity.y += eff->accel.y;
            eff->velocity.z += eff->accel.z;
            eff->scale += eff->scaleStep;
        }
    }
}

void EnFw_DrawDust(EnFw* self, GlobalContext* globalCtx) {
    static void* dustTextures[] = {
        gDust8Tex, gDust7Tex, gDust6Tex, gDust5Tex, gDust4Tex, gDust3Tex, gDust2Tex, gDust1Tex,
    };
    EnFwEffect* eff = self->effects;
    s16 firstDone;
    s16 alpha;
    s16 i;
    s16 idx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fw.c", 1191);

    firstDone = false;
    func_80093D84(globalCtx->state.gfxCtx);
    if (1) {}

    for (i = 0; i < ARRAY_COUNT(self->effects); i++, eff++) {
        if (eff->type != 0) {
            if (!firstDone) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0U);
                gSPDisplayList(POLY_XLU_DISP++, gFlareDancerDL_7928);
                gDPSetEnvColor(POLY_XLU_DISP++, 100, 60, 20, 0);
                firstDone = true;
            }

            alpha = eff->timer * (255.0f / eff->initialTimer);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 170, 130, 90, alpha);
            gDPPipeSync(POLY_XLU_DISP++);
            Matrix_Translate(eff->pos.x, eff->pos.y, eff->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(eff->scale, eff->scale, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_fw.c", 1229),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            idx = eff->timer * (8.0f / eff->initialTimer);
            gSPSegment(POLY_XLU_DISP++, 0x8, SEGMENTED_TO_VIRTUAL(dustTextures[idx]));
            gSPDisplayList(POLY_XLU_DISP++, &gFlareDancerSquareParticleDL);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fw.c", 1243);
}
