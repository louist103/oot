/*
 * File: z_en_bombf.c
 * Overlay: ovl_En_Bombf
 * Description: Bomb Flower
 */

#include "z_en_bombf.h"
#include "objects/object_bombf/object_bombf.h"
#include "overlays/effects/ovl_Effect_Ss_Dead_Sound/z_eff_ss_dead_sound.h"

#define FLAGS 0x00000011

#define THIS ((EnBombf*)thisx)

void EnBombf_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBombf_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBombf_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBombf_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnBombf_Move(EnBombf* self, GlobalContext* globalCtx);
void EnBombf_GrowBomb(EnBombf* self, GlobalContext* globalCtx);
void EnBombf_WaitForRelease(EnBombf* self, GlobalContext* globalCtx);
void EnBombf_Explode(EnBombf* self, GlobalContext* globalCtx);
void EnBombf_SetupGrowBomb(EnBombf* self, s16 params);

const ActorInit En_Bombf_InitVars = {
    ACTOR_EN_BOMBF,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BOMBF,
    sizeof(EnBombf),
    (ActorFunc)EnBombf_Init,
    (ActorFunc)EnBombf_Destroy,
    (ActorFunc)EnBombf_Update,
    (ActorFunc)EnBombf_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER | AC_TYPE_OTHER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
        { 0x0003F828, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 9, 18, 10, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000008, 0x00, 0x08 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 0 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ALL,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

void EnBombf_SetupAction(EnBombf* self, EnBombfActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnBombf_Init(Actor* thisx, GlobalContext* globalCtx) {
    f32 shapeUnk10 = 0.0f;
    s32 pad;
    EnBombf* self = THIS;

    Actor_SetScale(thisx, 0.01f);
    self->unk_200 = 1;
    Collider_InitCylinder(globalCtx, &self->bombCollider);
    Collider_InitJntSph(globalCtx, &self->explosionCollider);
    Collider_SetCylinder(globalCtx, &self->bombCollider, thisx, &sCylinderInit);
    Collider_SetJntSph(globalCtx, &self->explosionCollider, thisx, &sJntSphInit, &self->explosionColliderItems[0]);

    if (thisx->params == BOMBFLOWER_BODY) {
        shapeUnk10 = 1000.0f;
    }

    ActorShape_Init(&thisx->shape, shapeUnk10, ActorShadow_DrawCircle, 12.0f);
    thisx->focus.pos = thisx->world.pos;

    if (Actor_FindNearby(globalCtx, thisx, ACTOR_BG_DDAN_KD, ACTORCAT_BG, 10000.0f) != NULL) {
        thisx->flags |= 0x20;
    }

    thisx->colChkInfo.cylRadius = 10.0f;
    thisx->colChkInfo.cylHeight = 10;
    thisx->targetMode = 0;

    if (thisx->params == BOMBFLOWER_BODY) {
        self->timer = 140;
        self->flashSpeedScale = 15;
        thisx->gravity = -1.5f;
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, thisx, ACTORCAT_EXPLOSIVE);
        thisx->colChkInfo.mass = 200;
        thisx->flags &= ~1;
        EnBombf_SetupAction(self, EnBombf_Move);
    } else {
        thisx->colChkInfo.mass = MASS_IMMOVABLE;
        self->bumpOn = true;
        self->flowerBombScale = 1.0f;
        EnBombf_SetupGrowBomb(self, thisx->params);
    }

    thisx->uncullZoneScale += 31000.0f;
    thisx->uncullZoneForward += 31000.0f;
}

void EnBombf_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBombf* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->bombCollider);
    Collider_DestroyJntSph(globalCtx, &self->explosionCollider);
}

void EnBombf_SetupGrowBomb(EnBombf* self, s16 params) {
    EnBombf_SetupAction(self, EnBombf_GrowBomb);
}

void EnBombf_GrowBomb(EnBombf* self, GlobalContext* globalCtx) {
    EnBombf* bombFlower;
    s32 pad;
    s32 pad1;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad2;

    if (self->flowerBombScale >= 1.0f) {
        if (Actor_HasParent(&self->actor, globalCtx)) {
            bombFlower = (EnBombf*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOMBF, self->actor.world.pos.x,
                                               self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
            if (bombFlower != NULL) {
                func_8002F5C4(&self->actor, &bombFlower->actor, globalCtx);
                self->timer = 180;
                self->flowerBombScale = 0.0f;
                Audio_PlayActorSound2(&self->actor, NA_SE_PL_PULL_UP_ROCK);
                self->actor.flags &= ~1;
            } else {
                player->actor.child = NULL;
                player->heldActor = NULL;
                player->interactRangeActor = NULL;
                self->actor.parent = NULL;
                player->stateFlags1 &= ~0x800;
            }
        } else if (self->bombCollider.base.acFlags & AC_HIT) {
            self->bombCollider.base.acFlags &= ~AC_HIT;

            if (self->bombCollider.base.ac->category != ACTORCAT_BOSS) {
                bombFlower =
                    (EnBombf*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOMBF, self->actor.world.pos.x,
                                          self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
                if (bombFlower != NULL) {
                    bombFlower->unk_200 = 1;
                    bombFlower->timer = 0;
                    self->timer = 180;
                    self->actor.flags &= ~1;
                    self->flowerBombScale = 0.0f;
                }
            }
        } else {
            if (Player_IsBurningStickInRange(globalCtx, &self->actor.world.pos, 30.0f, 50.0f)) {
                bombFlower =
                    (EnBombf*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOMBF, self->actor.world.pos.x,
                                          self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
                if (bombFlower != NULL) {
                    bombFlower->timer = 100;
                    self->timer = 180;
                    self->actor.flags &= ~1;
                    self->flowerBombScale = 0.0f;
                }
            } else {
                if (!Actor_HasParent(&self->actor, globalCtx)) {
                    func_8002F580(&self->actor, globalCtx);
                } else {
                    player->actor.child = NULL;
                    player->heldActor = NULL;
                    player->interactRangeActor = NULL;
                    self->actor.parent = NULL;
                    player->stateFlags1 &= ~0x800;
                    self->actor.world.pos = self->actor.home.pos;
                }
            }
        }
    } else {
        if (self->timer == 0) {
            self->flowerBombScale += 0.05f;
            if (self->flowerBombScale >= 1.0f) {
                self->actor.flags |= 1;
            }
        }

        if (Actor_HasParent(&self->actor, globalCtx)) {
            player->actor.child = NULL;
            player->heldActor = NULL;
            player->interactRangeActor = NULL;
            self->actor.parent = NULL;
            player->stateFlags1 &= ~0x800;
            self->actor.world.pos = self->actor.home.pos;
        }
    }
}

void EnBombf_Move(EnBombf* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        // setting flowerBombScale does not do anything in the context of a bomb that link picks up
        // self and the assignment below are probably left overs
        self->flowerBombScale = 0.0f;
        EnBombf_SetupAction(self, EnBombf_WaitForRelease);
        self->actor.room = -1;
        return;
    }

    self->flowerBombScale = 1.0f;

    if (!(self->actor.bgCheckFlags & 1)) {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.025f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.5f, 0.0f);
        if ((self->actor.bgCheckFlags & 2) && (self->actor.velocity.y < -6.0f)) {
            func_8002F850(globalCtx, &self->actor);
            self->actor.velocity.y *= -0.5f;
        } else if (self->timer >= 4) {
            func_8002F580(&self->actor, globalCtx);
        }
    }
}

void EnBombf_WaitForRelease(EnBombf* self, GlobalContext* globalCtx) {
    // if parent is NULL bomb has been released
    if (Actor_HasNoParent(&self->actor, globalCtx)) {
        EnBombf_SetupAction(self, EnBombf_Move);
        EnBombf_Move(self, globalCtx);
    } else {
        self->actor.velocity.y = 0.0f;
    }
}

void EnBombf_Explode(EnBombf* self, GlobalContext* globalCtx) {
    Player* player;

    if (self->explosionCollider.elements[0].dim.modelSphere.radius == 0) {
        self->actor.flags |= 0x20;
        func_800AA000(self->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
    }

    self->explosionCollider.elements[0].dim.modelSphere.radius += 8;
    self->explosionCollider.elements[0].dim.worldSphere.radius =
        self->explosionCollider.elements[0].dim.modelSphere.radius;

    if (self->actor.params == BOMBFLOWER_EXPLOSION) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->explosionCollider.base);
    }

    if (globalCtx->envCtx.adjLight1Color[0] != 0) {
        globalCtx->envCtx.adjLight1Color[0] -= 25;
    }

    if (globalCtx->envCtx.adjLight1Color[1] != 0) {
        globalCtx->envCtx.adjLight1Color[1] -= 25;
    }

    if (globalCtx->envCtx.adjLight1Color[2] != 0) {
        globalCtx->envCtx.adjLight1Color[2] -= 25;
    }

    if (globalCtx->envCtx.adjAmbientColor[0] != 0) {
        globalCtx->envCtx.adjAmbientColor[0] -= 25;
    }

    if (globalCtx->envCtx.adjAmbientColor[1] != 0) {
        globalCtx->envCtx.adjAmbientColor[1] -= 25;
    }

    if (globalCtx->envCtx.adjAmbientColor[2] != 0) {
        globalCtx->envCtx.adjAmbientColor[2] -= 25;
    }

    if (self->timer == 0) {
        player = GET_PLAYER(globalCtx);

        if ((player->stateFlags1 & 0x800) && (player->heldActor == &self->actor)) {
            player->actor.child = NULL;
            player->heldActor = NULL;
            player->interactRangeActor = NULL;
            player->stateFlags1 &= ~0x800;
        }

        Actor_Kill(&self->actor);
    }
}

void EnBombf_Update(Actor* thisx, GlobalContext* globalCtx) {
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f bomb2Accel = { 0.0f, 0.1f, 0.0f };
    Vec3f effAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f effPos;
    Vec3f dustAccel = { 0.0f, 0.6f, 0.0f };
    Color_RGBA8 dustColor = { 255, 255, 255, 255 };
    s32 pad[2];
    EnBombf* self = THIS;

    if ((self->unk_200 != 0) && (self->timer != 0)) {
        self->timer--;
    }

    if ((!self->bumpOn) && (!Actor_HasParent(thisx, globalCtx)) &&
        ((thisx->xzDistToPlayer >= 20.0f) || (ABS(thisx->yDistToPlayer) >= 80.0f))) {
        self->bumpOn = true;
    }

    self->actionFunc(self, globalCtx);

    if (thisx->params == BOMBFLOWER_BODY) {
        Actor_MoveForward(thisx);
    }

    if (thisx->gravity != 0.0f) {
        DREG(6) = 1;
        Actor_UpdateBgCheckInfo(globalCtx, thisx, 5.0f, 10.0f, 0.0f, 0x1F);
        DREG(6) = 0;
    }

    if (thisx->params == BOMBFLOWER_BODY) {

        if ((thisx->velocity.y > 0.0f) && (thisx->bgCheckFlags & 0x10)) {
            thisx->velocity.y = -thisx->velocity.y;
        }

        // rebound bomb off the wall it hits
        if ((thisx->speedXZ != 0.0f) && (thisx->bgCheckFlags & 8)) {

            if (ABS((s16)(thisx->wallYaw - thisx->world.rot.y)) > 0x4000) {
                if (1) {}
                thisx->world.rot.y = ((thisx->wallYaw - thisx->world.rot.y) + thisx->wallYaw) - 0x8000;
            }
            Audio_PlayActorSound2(thisx, NA_SE_EV_BOMB_BOUND);
            Actor_MoveForward(thisx);
            DREG(6) = 1;
            Actor_UpdateBgCheckInfo(globalCtx, thisx, 5.0f, 10.0f, 0.0f, 0x1F);
            DREG(6) = 0;
            thisx->speedXZ *= 0.7f;
            thisx->bgCheckFlags &= ~8;
        }

        if ((self->bombCollider.base.acFlags & AC_HIT) || ((self->bombCollider.base.ocFlags1 & OC1_HIT) &&
                                                           (self->bombCollider.base.oc->category == ACTORCAT_ENEMY))) {
            self->unk_200 = 1;
            self->timer = 0;
        } else {
            // if a lit stick touches the bomb, set timer to 100
            if ((self->timer > 100) && Player_IsBurningStickInRange(globalCtx, &thisx->world.pos, 30.0f, 50.0f)) {
                self->timer = 100;
            }
        }

        if (self->unk_200 != 0) {
            dustAccel.y = 0.2f;
            effPos = thisx->world.pos;
            effPos.y += 25.0f;
            if (self->timer < 127) {
                // spawn spark effect on even frames
                if ((globalCtx->gameplayFrames % 2) == 0) {
                    EffectSsGSpk_SpawnFuse(globalCtx, thisx, &effPos, &effVelocity, &effAccel);
                }
                Audio_PlayActorSound2(thisx, NA_SE_IT_BOMB_IGNIT - SFX_FLAG);

                effPos.y += 3.0f;
                func_8002829C(globalCtx, &effPos, &effVelocity, &dustAccel, &dustColor, &dustColor, 50, 5);
            }

            // double bomb flash speed and adjust red color at certain times during the countdown
            if ((self->timer == 3) || (self->timer == 30) || (self->timer == 50) || (self->timer == 70)) {
                self->flashSpeedScale >>= 1;
            }

            if ((self->timer < 100) && ((self->timer & (self->flashSpeedScale + 1)) != 0)) {
                Math_SmoothStepToF(&self->flashIntensity, 150.0f, 1.0f, 150.0f / self->flashSpeedScale, 0.0f);
            } else {
                Math_SmoothStepToF(&self->flashIntensity, 0.0f, 1.0f, 150.0f / self->flashSpeedScale, 0.0f);
            }

            if (self->timer < 3) {
                Actor_SetScale(thisx, thisx->scale.x + 0.002f);
            }

            if (self->timer == 0) {
                effPos = thisx->world.pos;

                effPos.y += 10.0f;

                if (Actor_HasParent(thisx, globalCtx)) {
                    effPos.y += 30.0f;
                }

                EffectSsBomb2_SpawnLayered(globalCtx, &effPos, &effVelocity, &bomb2Accel, 100, 19);

                effPos.y = thisx->floorHeight;
                if (thisx->floorHeight > BGCHECK_Y_MIN) {
                    EffectSsBlast_SpawnWhiteShockwave(globalCtx, &effPos, &effVelocity, &effAccel);
                }

                Audio_PlayActorSound2(thisx, NA_SE_IT_BOMB_EXPLOSION);
                globalCtx->envCtx.adjLight1Color[0] = globalCtx->envCtx.adjLight1Color[1] =
                    globalCtx->envCtx.adjLight1Color[2] = 250;
                globalCtx->envCtx.adjAmbientColor[0] = globalCtx->envCtx.adjAmbientColor[1] =
                    globalCtx->envCtx.adjAmbientColor[2] = 250;
                Camera_AddQuake(&globalCtx->mainCamera, 2, 0xB, 8);
                thisx->params = BOMBFLOWER_EXPLOSION;
                self->timer = 10;
                thisx->flags |= 0x20;
                EnBombf_SetupAction(self, EnBombf_Explode);
            }
        }
    }

    thisx->focus.pos = thisx->world.pos;
    thisx->focus.pos.y += 10.0f;

    if (thisx->params <= BOMBFLOWER_BODY) {

        Collider_UpdateCylinder(thisx, &self->bombCollider);

        if ((self->flowerBombScale >= 1.0f) && (self->bumpOn)) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->bombCollider.base);
        }

        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->bombCollider.base);
    }

    if ((thisx->scale.x >= 0.01f) && (thisx->params != BOMBFLOWER_EXPLOSION)) {
        if (thisx->yDistToWater >= 20.0f) {
            EffectSsDeadSound_SpawnStationary(globalCtx, &thisx->projectedPos, NA_SE_IT_BOMB_UNEXPLOSION, true,
                                              DEADSOUND_REPEAT_MODE_OFF, 10);
            Actor_Kill(thisx);
            return;
        }
        if (thisx->bgCheckFlags & 0x40) {
            thisx->bgCheckFlags &= ~0x40;
            Audio_PlayActorSound2(thisx, NA_SE_EV_BOMB_DROP_WATER);
        }
    }
}

Gfx* EnBombf_NewMtxDList(GraphicsContext* gfxCtx, GlobalContext* globalCtx) {
    Gfx* displayList;
    Gfx* displayListHead;

    displayList = Graph_Alloc(gfxCtx, 5 * sizeof(Gfx));
    displayListHead = displayList;
    func_800D1FD4(&globalCtx->mf_11DA0);
    gSPMatrix(displayListHead++, Matrix_NewMtx(gfxCtx, "../z_en_bombf.c", 1021),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPEndDisplayList(displayListHead);
    return displayList;
}

void EnBombf_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnBombf* self = THIS;

    if (1) {}

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bombf.c", 1034);

    if (thisx->params <= BOMBFLOWER_BODY) {
        func_80093D18(globalCtx->state.gfxCtx);

        if (thisx->params != BOMBFLOWER_BODY) {
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_bombf.c", 1041),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gBombFlowerLeavesDL);
            gSPDisplayList(POLY_OPA_DISP++, gBombFlowerBaseLeavesDL);

            Matrix_Translate(0.0f, 1000.0f, 0.0f, MTXMODE_APPLY);
            Matrix_Scale(self->flowerBombScale, self->flowerBombScale, self->flowerBombScale, MTXMODE_APPLY);
        }

        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 200, 255, 200, 255);
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, (s16)self->flashIntensity, 20, 10, 0);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_bombf.c", 1054),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   SEGMENTED_TO_VIRTUAL(EnBombf_NewMtxDList(globalCtx->state.gfxCtx, globalCtx)));
        gSPDisplayList(POLY_OPA_DISP++, gBombFlowerBombAndSparkDL);
    } else {
        Collider_UpdateSpheres(0, &self->explosionCollider);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bombf.c", 1063);
}
