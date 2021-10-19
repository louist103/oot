/*
 * File: z_en_vm.c
 * Overlay: ovl_En_Vm
 * Description: Beamos
 */

#include "z_en_vm.h"
#include "objects/object_vm/object_vm.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000011

#define THIS ((EnVm*)thisx)

void EnVm_Init(Actor* thisx, GlobalContext* globalCtx);
void EnVm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnVm_Update(Actor* thisx, GlobalContext* globalCtx);
void EnVm_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnVm_SetupWait(EnVm* self);
void EnVm_Wait(EnVm* self, GlobalContext* globalCtx);
void EnVm_SetupAttack(EnVm* self);
void EnVm_Attack(EnVm* self, GlobalContext* globalCtx);
void EnVm_Stun(EnVm* self, GlobalContext* globalCtx);
void EnVm_Die(EnVm* self, GlobalContext* globalCtx);

const ActorInit En_Vm_InitVars = {
    ACTOR_EN_VM,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_VM,
    sizeof(EnVm),
    (ActorFunc)EnVm_Init,
    (ActorFunc)EnVm_Destroy,
    (ActorFunc)EnVm_Update,
    (ActorFunc)EnVm_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
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
    { 25, 70, 0, { 0, 0, 0 } },
};

static ColliderQuadInit sQuadInit1 = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x10 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL | TOUCH_UNK7,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static ColliderQuadInit sQuadInit2 = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static Vec3f D_80B2EAEC = { 0.0f, 0.0f, 0.0f };

static Vec3f D_80B2EAF8 = { 0.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB04 = { 500.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB10 = { -500.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB1C = { 0.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB28 = { 0.0f, 0.0f, 1600.0f };

static Vec3f D_80B2EB34 = { 1000.0f, 700.0f, 2000.0f };

static Vec3f D_80B2EB40 = { 1000.0f, -700.0f, 2000.0f };

static Vec3f D_80B2EB4C = { -1000.0f, 700.0f, 1500.0f };

static Vec3f D_80B2EB58 = { -1000.0f, -700.0f, 1500.0f };

static Vec3f D_80B2EB64 = { 500.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB70 = { -500.0f, 0.0f, 0.0f };

static Vec3f D_80B2EB7C = { 0.4f, 0.4f, 0.4f };

static void* D_80B2EB88[] = {
    gEffEnemyDeathFlame1Tex, gEffEnemyDeathFlame2Tex,  gEffEnemyDeathFlame3Tex, gEffEnemyDeathFlame4Tex,
    gEffEnemyDeathFlame5Tex, gEffEnemyDeathFlame6Tex,  gEffEnemyDeathFlame7Tex, gEffEnemyDeathFlame8Tex,
    gEffEnemyDeathFlame9Tex, gEffEnemyDeathFlame10Tex,
};

void EnVm_SetupAction(EnVm* self, EnVmActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnVm_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnVm* self = THIS;

    SkelAnime_Init(globalCtx, &self->skelAnime, &gBeamosSkel, &gBeamosAnim, self->jointTable, self->morphTable, 11);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    Collider_InitCylinder(globalCtx, &self->colliderCylinder);
    Collider_SetCylinder(globalCtx, &self->colliderCylinder, thisx, &sCylinderInit);
    Collider_InitQuad(globalCtx, &self->colliderQuad1);
    Collider_SetQuad(globalCtx, &self->colliderQuad1, thisx, &sQuadInit1);
    Collider_InitQuad(globalCtx, &self->colliderQuad2);
    Collider_SetQuad(globalCtx, &self->colliderQuad2, thisx, &sQuadInit2);
    self->beamSightRange = (thisx->params >> 8) * 40.0f;
    thisx->params &= 0xFF;
    thisx->naviEnemyId = 0x39;

    if (thisx->params == BEAMOS_LARGE) {
        thisx->colChkInfo.health = 2;
        Actor_SetScale(thisx, 0.014f);
    } else {
        thisx->colChkInfo.health = 1;
        Actor_SetScale(thisx, 0.01f);
    }

    EnVm_SetupWait(self);
}

void EnVm_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnVm* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->colliderCylinder);
}

void EnVm_SetupWait(EnVm* self) {
    f32 frameCount = Animation_GetLastFrame(&gBeamosAnim);

    Animation_Change(&self->skelAnime, &gBeamosAnim, 1.0f, frameCount, frameCount, ANIMMODE_ONCE, 0.0f);
    self->unk_25E = self->unk_260 = 0;
    self->unk_21C = 0;
    self->timer = 10;
    EnVm_SetupAction(self, EnVm_Wait);
}

void EnVm_Wait(EnVm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 dist;
    s16 headRot;
    s16 pad;
    s16 pitch;

    switch (self->unk_25E) {
        case 0:
            Math_SmoothStepToS(&self->beamRot.x, 0, 10, 1500, 0);
            headRot = self->actor.yawTowardsPlayer - self->headRotY - self->actor.shape.rot.y;
            pitch = Math_Vec3f_Pitch(&self->beamPos1, &player->actor.world.pos);

            if (pitch > 0x1B91) {
                pitch = 0x1B91;
            }

            dist = self->beamSightRange - self->actor.xzDistToPlayer;

            if (self->actor.xzDistToPlayer <= self->beamSightRange && ABS(headRot) <= 0x2710 && pitch >= 0xE38 &&
                self->actor.yDistToPlayer <= 80.0f && self->actor.yDistToPlayer >= -160.0f) {
                Math_SmoothStepToS(&self->beamRot.x, pitch, 10, 0xFA0, 0);
                if (Math_SmoothStepToS(&self->headRotY, self->actor.yawTowardsPlayer - self->actor.shape.rot.y, 1,
                                       (ABS((s16)(dist * 180.0f)) / 3) + 0xFA0, 0) <= 5460) {
                    self->timer--;
                    if (self->timer == 0) {
                        self->unk_25E++;
                        self->skelAnime.curFrame = 0.0f;
                        self->skelAnime.startFrame = 0.0f;
                        self->skelAnime.playSpeed = 2.0f;
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BIMOS_AIM);
                    }
                }
            } else {
                self->headRotY -= 0x1F4;
            }

            SkelAnime_Update(&self->skelAnime);
            return;
        case 1:
            break;
        default:
            return;
    }

    Math_SmoothStepToS(&self->headRotY, self->actor.yawTowardsPlayer - self->actor.shape.rot.y, 1, 0x1F40, 0);

    if (SkelAnime_Update(&self->skelAnime)) {
        self->unk_260++;
        self->skelAnime.curFrame = 0.0f;
    }

    if (self->unk_260 == 2) {
        self->beamRot.y = self->actor.yawTowardsPlayer;
        self->beamRot.x = Math_Vec3f_Pitch(&self->beamPos1, &player->actor.world.pos);

        if (self->beamRot.x > 0x1B91) {
            self->beamRot.x = 0x1B91;
        }

        if (self->beamRot.x < 0xAAA) {
            self->skelAnime.startFrame = self->skelAnime.curFrame = self->skelAnime.endFrame;
            self->unk_25E = self->unk_260 = 0;
            self->timer = 10;
            self->skelAnime.playSpeed = 1.0f;
        } else {
            self->skelAnime.curFrame = 6.0f;
            EffectSsDeadDd_Spawn(globalCtx, &self->beamPos2, &D_80B2EAEC, &D_80B2EAEC, 150, -25, 0, 0, 255, 0, 255, 255,
                                 255, 16, 20);
            EnVm_SetupAttack(self);
        }
    }
}

void EnVm_SetupAttack(EnVm* self) {
    Animation_Change(&self->skelAnime, &gBeamosAnim, 3.0f, 3.0f, 7.0f, ANIMMODE_ONCE, 0.0f);
    self->timer = 305;
    self->beamScale.x = 0.6f;
    self->beamSpeed = 40.0f;
    self->unk_21C = 1;
    self->colliderQuad1.base.atFlags &= ~AT_HIT;
    EnVm_SetupAction(self, EnVm_Attack);
}

void EnVm_Attack(EnVm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 pitch = Math_Vec3f_Pitch(&self->beamPos1, &player->actor.world.pos);
    f32 dist;
    Vec3f playerPos;

    if (pitch > 0x1B91) {
        pitch = 0x1B91;
    }

    if (self->colliderQuad1.base.atFlags & AT_HIT) {
        self->colliderQuad1.base.atFlags &= ~AT_HIT;
        self->timer = 0;

        if (self->beamScale.x > 0.1f) {
            self->beamScale.x = 0.1f;
        }
    }

    if (self->beamRot.x < 0xAAA || self->timer == 0) {
        Math_SmoothStepToF(&self->beamScale.x, 0.0f, 1.0f, 0.03f, 0.0f);
        self->unk_260 = 0;

        if (self->beamScale.x == 0.0f) {
            self->beamScale.y = self->beamScale.z = 0.0f;
            EnVm_SetupWait(self);
        }
    } else {
        if (--self->timer > 300) {
            return;
        }

        Math_SmoothStepToS(&self->headRotY, -self->actor.shape.rot.y + self->actor.yawTowardsPlayer, 10, 0xDAC, 0);
        Math_SmoothStepToS(&self->beamRot.y, self->actor.yawTowardsPlayer, 10, 0xDAC, 0);
        Math_SmoothStepToS(&self->beamRot.x, pitch, 10, 0xDAC, 0);
        playerPos = player->actor.world.pos;

        if (player->actor.floorHeight > BGCHECK_Y_MIN) {
            playerPos.y = player->actor.floorHeight;
        }

        dist = Math_Vec3f_DistXYZ(&self->beamPos1, &playerPos);
        Math_SmoothStepToF(&self->beamScale.z, dist, 1.0f, self->beamSpeed, 0.0f);
        Math_SmoothStepToF(&self->beamScale.x, 0.1f, 1.0f, 0.12f, 0.0f);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BIMOS_LAZER - SFX_FLAG);

        if (self->unk_260 > 2) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderQuad1.base);
        }

        self->unk_260 = 3;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        self->skelAnime.curFrame = self->skelAnime.startFrame;
    }
}

void EnVm_SetupStun(EnVm* self) {
    Animation_Change(&self->skelAnime, &gBeamosAnim, -1.0f, Animation_GetLastFrame(&gBeamosAnim), 0.0f, ANIMMODE_ONCE,
                     0.0f);
    self->unk_260 = 0;
    self->timer = 180;
    self->unk_25E = self->unk_260;
    self->unk_21C = 2;
    self->beamScale.z = 0.0f;
    self->beamScale.y = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnVm_SetupAction(self, EnVm_Stun);
}

void EnVm_Stun(EnVm* self, GlobalContext* globalCtx) {
    if (self->timer == 0) {
        if (SkelAnime_Update(&self->skelAnime)) {
            self->unk_25E++;
            if (self->unk_25E == 3) {
                EnVm_SetupWait(self);
            } else if (self->unk_25E == 1) {
                Animation_Change(&self->skelAnime, &gBeamosAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gBeamosAnim),
                                 ANIMMODE_ONCE, 0.0f);
            } else {
                self->timer = 10;
                self->skelAnime.curFrame = 0.0f;
                self->skelAnime.playSpeed = 2.0f;
            }
        }
    } else {
        Math_SmoothStepToS(&self->beamRot.x, 0, 10, 0x5DC, 0);
        self->timer--;
        SkelAnime_Update(&self->skelAnime);
    }
}

void EnVm_SetupDie(EnVm* self) {
    Animation_Change(&self->skelAnime, &gBeamosAnim, -1.0f, Animation_GetLastFrame(&gBeamosAnim), 0.0f, ANIMMODE_ONCE,
                     0.0f);
    self->timer = 33;
    self->unk_25E = self->unk_260 = 0;
    self->unk_21C = 3;
    self->beamScale.z = 0.0f;
    self->beamScale.y = 0.0f;
    self->actor.shape.yOffset = -5000.0f;
    self->actor.world.pos.y += 5000.0f * self->actor.scale.y;
    self->actor.velocity.y = 8.0f;
    self->actor.gravity = -0.5f;
    self->actor.speedXZ = Rand_ZeroOne() + 1.0f;
    self->actor.world.rot.y = Rand_CenteredFloat(65535.0f);
    EnVm_SetupAction(self, EnVm_Die);
}

void EnVm_Die(EnVm* self, GlobalContext* globalCtx) {
    EnBom* bomb;

    self->beamRot.x += 0x5DC;
    self->headRotY += 0x9C4;
    Actor_MoveForward(&self->actor);

    if (--self->timer == 0) {
        bomb = (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, self->actor.world.pos.x,
                                   self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0x6FF, BOMB_BODY);

        if (bomb != NULL) {
            bomb->timer = 0;
        }

        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xA0);
        Actor_Kill(&self->actor);
    }
}

void EnVm_CheckHealth(EnVm* self, GlobalContext* globalCtx) {
    EnBom* bomb;

    if (Actor_GetCollidedExplosive(globalCtx, &self->colliderCylinder.base) != NULL) {
        self->actor.colChkInfo.health--;
        osSyncPrintf("hp down %d\n", self->actor.colChkInfo.health);
    } else {
        if (!(self->colliderQuad2.base.acFlags & AC_HIT) || self->unk_21C == 2) {
            return;
        }
        self->colliderQuad2.base.acFlags &= ~AC_HIT;
    }

    if (self->actor.colChkInfo.health != 0) {
        Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
        EnVm_SetupStun(self);
    } else {
        bomb = (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, self->actor.world.pos.x,
                                   self->actor.world.pos.y + 20.0f, self->actor.world.pos.z, 0, 0, 0x601, BOMB_BODY);

        if (bomb != NULL) {
            bomb->timer = 0;
        }

        EnVm_SetupDie(self);
    }
}

void EnVm_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnVm* self = THIS;
    CollisionCheckContext* colChkCtx = &globalCtx->colChkCtx;

    if (self->actor.colChkInfo.health != 0) {
        EnVm_CheckHealth(self, globalCtx);
    }

    if (self->unk_260 == 4) {
        EffectSsDeadDs_SpawnStationary(globalCtx, &self->beamPos3, 20, -1, 255, 20);
        func_80033480(globalCtx, &self->beamPos3, 6.0f, 1, 120, 20, 1);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BIMOS_LAZER_GND - SFX_FLAG);
    }

    self->actionFunc(self, globalCtx);
    self->beamTexScroll += 0xC;

    if (self->actor.colChkInfo.health != 0 && self->unk_21C != 2) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BIMOS_ROLL_HEAD - SFX_FLAG);
    }

    Collider_UpdateCylinder(&self->actor, &self->colliderCylinder);
    CollisionCheck_SetOC(globalCtx, colChkCtx, &self->colliderCylinder.base);

    if (self->actor.colorFilterTimer == 0 && self->actor.colChkInfo.health != 0) {
        CollisionCheck_SetAC(globalCtx, colChkCtx, &self->colliderCylinder.base);
    }

    CollisionCheck_SetAC(globalCtx, colChkCtx, &self->colliderQuad2.base);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += (6500.0f + self->actor.shape.yOffset) * self->actor.scale.y;
}

s32 EnVm_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnVm* self = THIS;

    if (limbIndex == 2) {
        rot->x += self->beamRot.x;
        rot->y += self->headRotY;
    } else if (limbIndex == 10) {
        if (self->unk_21C == 3) {
            *dList = NULL;
        }
    }

    return false;
}

void EnVm_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnVm* self = THIS;
    Vec3f sp80 = D_80B2EAF8;
    Vec3f sp74 = D_80B2EB04;
    Vec3f sp68 = D_80B2EB10;
    s32 pad;
    Vec3f posResult;
    CollisionPoly* poly;
    s32 bgId;
    f32 dist;

    if (limbIndex == 2) {
        Matrix_MultVec3f(&D_80B2EB1C, &self->beamPos1);
        Matrix_MultVec3f(&D_80B2EB28, &self->beamPos2);

        if (self->unk_260 >= 3) {
            poly = NULL;
            sp80.z = (self->beamScale.z + 500.0f) * (self->actor.scale.y * 10000.0f);
            Matrix_MultVec3f(&sp80, &self->beamPos3);

            if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &self->beamPos1, &self->beamPos3, &posResult, &poly, true,
                                        true, false, true, &bgId) == true) {
                self->beamScale.z = Math_Vec3f_DistXYZ(&self->beamPos1, &posResult) - 5.0f;
                self->unk_260 = 4;
                self->beamPos3 = posResult;
            }
            if (self->beamScale.z != 0.0f) {
                dist = 100.0f;
                if (self->actor.scale.y > 0.01f) {
                    dist = 70.0f;
                }
                sp74.z = sp68.z = Math_Vec3f_DistXYZ(&self->beamPos1, &self->beamPos3) * dist;
                Matrix_MultVec3f(&D_80B2EB64, &self->colliderQuad1.dim.quad[3]);
                Matrix_MultVec3f(&D_80B2EB70, &self->colliderQuad1.dim.quad[2]);
                Matrix_MultVec3f(&sp74, &self->colliderQuad1.dim.quad[1]);
                Matrix_MultVec3f(&sp68, &self->colliderQuad1.dim.quad[0]);
                Collider_SetQuadVertices(&self->colliderQuad1, &self->colliderQuad1.dim.quad[0],
                                         &self->colliderQuad1.dim.quad[1], &self->colliderQuad1.dim.quad[2],
                                         &self->colliderQuad1.dim.quad[3]);
            }
        }
        Matrix_MultVec3f(&D_80B2EB34, &self->colliderQuad2.dim.quad[1]);
        Matrix_MultVec3f(&D_80B2EB40, &self->colliderQuad2.dim.quad[0]);
        Matrix_MultVec3f(&D_80B2EB4C, &self->colliderQuad2.dim.quad[3]);
        Matrix_MultVec3f(&D_80B2EB58, &self->colliderQuad2.dim.quad[2]);
        Collider_SetQuadVertices(&self->colliderQuad2, &self->colliderQuad2.dim.quad[0],
                                 &self->colliderQuad2.dim.quad[1], &self->colliderQuad2.dim.quad[2],
                                 &self->colliderQuad2.dim.quad[3]);
    }
}

void EnVm_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    EnVm* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    Vec3f actorPos;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_vm.c", 1014);

    func_80093D18(globalCtx->state.gfxCtx);
    func_80093D84(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnVm_OverrideLimbDraw,
                      EnVm_PostLimbDraw, self);
    actorPos = self->actor.world.pos;
    func_80033C30(&actorPos, &D_80B2EB7C, 255, globalCtx);

    if (self->unk_260 >= 3) {
        Matrix_Translate(self->beamPos3.x, self->beamPos3.y + 10.0f, self->beamPos3.z, MTXMODE_NEW);
        Matrix_Scale(0.8f, 0.8f, 0.8f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vm.c", 1033),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 168);
        func_80094BC4(globalCtx->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 0);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80B2EB88[globalCtx->gameplayFrames % 8]));
        gSPDisplayList(POLY_XLU_DISP++, gEffEnemyDeathFlameDL);
        Matrix_RotateY(32767.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vm.c", 1044),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80B2EB88[(globalCtx->gameplayFrames + 4) % 8]));
        gSPDisplayList(POLY_XLU_DISP++, gEffEnemyDeathFlameDL);
    }
    gSPSegment(POLY_OPA_DISP++, 0x08, func_80094E78(globalCtx->state.gfxCtx, 0, self->beamTexScroll));
    Matrix_Translate(self->beamPos1.x, self->beamPos1.y, self->beamPos1.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->beamRot.x, self->beamRot.y, self->beamRot.z, MTXMODE_APPLY);
    Matrix_Scale(self->beamScale.x * 0.1f, self->beamScale.x * 0.1f, self->beamScale.z * 0.0015f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vm.c", 1063),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gBeamosLaserDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_vm.c", 1068);
}
