/*
 * File: z_en_vb_ball.c
 * Overlay: ovl_En_Vb_Ball
 * Description: Volvagia's rocks and bones
 */

#include "z_en_vb_ball.h"
#include "objects/object_fd/object_fd.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "overlays/actors/ovl_Boss_Fd/z_boss_fd.h"

#define FLAGS 0x00000030

#define THIS ((EnVbBall*)thisx)

void EnVbBall_Init(Actor* thisx, GlobalContext* globalCtx);
void EnVbBall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnVbBall_Update(Actor* thisx, GlobalContext* globalCtx);
void EnVbBall_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Vb_Ball_InitVars = {
    0,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_FD,
    sizeof(EnVbBall),
    (ActorFunc)EnVbBall_Init,
    (ActorFunc)EnVbBall_Destroy,
    (ActorFunc)EnVbBall_Update,
    (ActorFunc)EnVbBall_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK6,
        { 0x00100700, 0x00, 0x20 },
        { 0x00100700, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 30, 10, { 0, 0, 0 } },
};

void EnVbBall_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnVbBall* self = THIS;
    s32 pad2;
    f32 angle;

    if (self->actor.params >= 200) { // Volvagia's bones
        self->yRotVel = Rand_CenteredFloat(0x300);
        self->xRotVel = Rand_CenteredFloat(0x300);
        angle = Math_FAtan2F(self->actor.world.pos.x, self->actor.world.pos.z);
        self->actor.velocity.y = Rand_ZeroFloat(3.0f);
        self->actor.velocity.x = 2.0f * sinf(angle);
        self->actor.velocity.z = 2.0f * cosf(angle);
        self->actor.gravity = -0.8f;
    } else { // Volvagia's rocks
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        Actor_SetScale(&self->actor, self->actor.world.rot.z / 10000.0f);
        self->collider.dim.radius = self->actor.scale.y * 3000.0f;
        self->collider.dim.height = self->actor.scale.y * 5000.0f;
        self->collider.dim.yShift = self->actor.scale.y * -2500.0f;
        self->xRotVel = Rand_CenteredFloat(0x2000);
        self->yRotVel = Rand_CenteredFloat(0x2000);
        self->shadowSize = self->actor.scale.y * 68.0f;
    }
}

void EnVbBall_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnVbBall* self = THIS;

    if (self->actor.params < 200) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void EnVbBall_SpawnDebris(GlobalContext* globalCtx, BossFdEffect* effect, Vec3f* position, Vec3f* velocity,
                          Vec3f* acceleration, f32 scale) {
    s16 i;

    for (i = 0; i < 180; i++, effect++) {
        if (effect->type == BFD_FX_NONE) {
            effect->type = BFD_FX_DEBRIS;
            effect->pos = *position;
            effect->velocity = *velocity;
            effect->accel = *acceleration;
            effect->scale = scale / 1000.0f;
            effect->vFdFxRotX = Rand_ZeroFloat(100.0f);
            effect->vFdFxRotY = Rand_ZeroFloat(100.0f);
            break;
        }
    }
}

void EnVbBall_SpawnDust(GlobalContext* globalCtx, BossFdEffect* effect, Vec3f* position, Vec3f* velocity,
                        Vec3f* acceleration, f32 scale) {
    s16 i;

    for (i = 0; i < 180; i++, effect++) {
        if (effect->type == BFD_FX_NONE) {
            effect->type = BFD_FX_DUST;
            effect->pos = *position;
            effect->velocity = *velocity;
            effect->accel = *acceleration;
            effect->timer2 = 0;
            effect->scale = scale / 400.0f;
            break;
        }
    }
}

void EnVbBall_UpdateBones(EnVbBall* self, GlobalContext* globalCtx) {
    BossFd* bossFd = (BossFd*)self->actor.parent;
    f32 pad2;
    f32 pad1;
    f32 angle;
    s16 i;

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 50.0f, 100.0f, 4);
    if ((self->actor.bgCheckFlags & 1) && (self->actor.velocity.y <= 0.0f)) {
        self->xRotVel = Rand_CenteredFloat((f32)0x4000);
        self->yRotVel = Rand_CenteredFloat((f32)0x4000);
        angle = Math_FAtan2F(self->actor.world.pos.x, self->actor.world.pos.z);
        self->actor.velocity.x = sinf(angle) * 10.0f;
        self->actor.velocity.z = cosf(angle) * 10.0f;
        self->actor.velocity.y *= -0.5f;
        if (self->actor.params & 1) {
            Audio_PlaySoundGeneral(NA_SE_EN_VALVAISA_LAND, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
        for (i = 0; i < 10; i++) {
            Vec3f dustVel = { 0.0f, 0.0f, 0.0f };
            Vec3f dustAcc = { 0.0f, 0.0f, 0.0f };
            Vec3f dustPos;

            dustVel.x = Rand_CenteredFloat(8.0f);
            dustVel.y = Rand_ZeroFloat(1.0f);
            dustVel.z = Rand_CenteredFloat(8.0f);

            dustAcc.y = 0.3f;

            dustPos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
            dustPos.y = self->actor.floorHeight + 10.0f;
            dustPos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;

            EnVbBall_SpawnDust(globalCtx, bossFd->effects, &dustPos, &dustVel, &dustAcc,
                               Rand_ZeroFloat(80.0f) + 200.0f);
        }
    }
    if (self->actor.world.pos.y < 50.0f) {
        Actor_Kill(&self->actor);
    }
}

void EnVbBall_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnVbBall* self = THIS;
    BossFd* bossFd = (BossFd*)self->actor.parent;
    f32 radius;
    f32 pad2;
    s16 spawnNum;
    s16 i;

    self->unkTimer2++;
    if (self->unkTimer1 != 0) {
        self->unkTimer1--;
    }
    self->actor.shape.rot.x += (s16)self->xRotVel;
    self->actor.shape.rot.y += (s16)self->yRotVel;
    self->actor.velocity.y += -1.0f;
    self->actor.gravity = -1.0f;
    func_8002D7EC(&self->actor);
    if (self->actor.params >= 200) {
        EnVbBall_UpdateBones(self, globalCtx);
    } else {
        Math_ApproachF(&self->shadowOpacity, 175.0f, 1.0f, 40.0f);
        radius = self->actor.scale.y * 1700.0f;
        self->actor.world.pos.y -= radius;
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 50.0f, 100.0f, 4);
        self->actor.world.pos.y += radius;
        if ((self->actor.bgCheckFlags & 1) && (self->actor.velocity.y <= 0.0f)) {
            if ((self->actor.params == 100) || (self->actor.params == 101)) {
                Actor_Kill(&self->actor);
                if (self->actor.params == 100) {
                    func_80033E88(&self->actor, globalCtx, 5, 0xA);
                }
                if (self->actor.params == 100) {
                    spawnNum = 2;
                } else {
                    spawnNum = 2;
                }
                for (i = 0; i < spawnNum; i++) {
                    Vec3f spawnOffset;
                    EnVbBall* newActor;
                    f32 xRotVel;

                    if (self->actor.params == 100) {
                        spawnOffset.x = Rand_CenteredFloat(13.0f);
                        spawnOffset.y = Rand_ZeroFloat(5.0f) + 6.0f;
                        spawnOffset.z = Rand_CenteredFloat(13);
                    } else {
                        spawnOffset.x = Rand_CenteredFloat(10.0f);
                        spawnOffset.y = Rand_ZeroFloat(3.0f) + 4.0f;
                        spawnOffset.z = Rand_CenteredFloat(10.0f);
                    }
                    newActor = (EnVbBall*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx,
                                                             ACTOR_EN_VB_BALL, self->actor.world.pos.x + spawnOffset.x,
                                                             self->actor.world.pos.y + spawnOffset.y,
                                                             self->actor.world.pos.z + spawnOffset.z, 0, 0,
                                                             self->actor.world.rot.z * 0.5f, self->actor.params + 1);
                    if (newActor != NULL) {
                        if ((i == 0) && (self->actor.params == 100)) {
                            Audio_PlaySoundGeneral(NA_SE_EN_VALVAISA_ROCK, &newActor->actor.projectedPos, 4,
                                                   &D_801333E0, &D_801333E0, &D_801333E8);
                        }
                        newActor->actor.parent = self->actor.parent;
                        newActor->actor.velocity = spawnOffset;
                        newActor->yRotVel = 0.0f;
                        xRotVel = sqrtf(SQ(spawnOffset.x) + SQ(spawnOffset.z));
                        newActor->xRotVel = 0x1000 / 10.0f * xRotVel;
                        newActor->actor.shape.rot.y = Math_FAtan2F(spawnOffset.x, spawnOffset.z) * ((f32)0x8000 / M_PI);
                        newActor->shadowOpacity = 200.0f;
                    }
                }
                for (i = 0; i < 15; i++) {
                    Vec3f debrisVel1 = { 0.0f, 0.0f, 0.0f };
                    Vec3f debrisAcc1 = { 0.0f, -1.0f, 0.0f };
                    Vec3f debrisPos1;

                    debrisVel1.x = Rand_CenteredFloat(25.0f);
                    debrisVel1.y = Rand_ZeroFloat(5.0f) + 8;
                    debrisVel1.z = Rand_CenteredFloat(25.0f);

                    debrisPos1.x = Rand_CenteredFloat(10.0f) + self->actor.world.pos.x;
                    debrisPos1.y = Rand_CenteredFloat(10.0f) + self->actor.world.pos.y;
                    debrisPos1.z = Rand_CenteredFloat(10.0f) + self->actor.world.pos.z;

                    EnVbBall_SpawnDebris(globalCtx, bossFd->effects, &debrisPos1, &debrisVel1, &debrisAcc1,
                                         (s16)Rand_ZeroFloat(12.0f) + 15);
                }
                for (i = 0; i < 10; i++) {
                    Vec3f dustVel = { 0.0f, 0.0f, 0.0f };
                    Vec3f dustAcc = { 0.0f, 0.0f, 0.0f };
                    Vec3f dustPos;

                    dustVel.x = Rand_CenteredFloat(8.0f);
                    dustVel.y = Rand_ZeroFloat(1.0f);
                    dustVel.z = Rand_CenteredFloat(8.0f);

                    dustAcc.y = 1.0f / 2;

                    dustPos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
                    dustPos.y = Rand_CenteredFloat(30.0f) + self->actor.world.pos.y;
                    dustPos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;

                    EnVbBall_SpawnDust(globalCtx, bossFd->effects, &dustPos, &dustVel, &dustAcc,
                                       Rand_ZeroFloat(100.0f) + 350.0f);
                }
            } else {
                for (i = 0; i < 5; i++) {
                    Vec3f debrisVel2 = { 0.0f, 0.0f, 0.0f };
                    Vec3f debrisAcc2 = { 0.0f, -1.0f, 0.0f };
                    Vec3f debrisPos2;

                    debrisVel2.x = Rand_CenteredFloat(10.0f);
                    debrisVel2.y = Rand_ZeroFloat(3.0f) + 3.0f;
                    debrisVel2.z = Rand_CenteredFloat(10.0f);

                    debrisPos2.x = Rand_CenteredFloat(5.0f) + self->actor.world.pos.x;
                    debrisPos2.y = Rand_CenteredFloat(5.0f) + self->actor.world.pos.y;
                    debrisPos2.z = Rand_CenteredFloat(5.0f) + self->actor.world.pos.z;

                    EnVbBall_SpawnDebris(globalCtx, bossFd->effects, &debrisPos2, &debrisVel2, &debrisAcc2,
                                         (s16)Rand_ZeroFloat(12.0f) + 15);
                }
                Actor_Kill(&self->actor);
            }
        }
        if (self->collider.base.atFlags & AT_HIT) {
            Player* player = GET_PLAYER(globalCtx);

            self->collider.base.atFlags &= ~AT_HIT;
            Audio_PlayActorSound2(&player->actor, NA_SE_PL_BODY_HIT);
        }
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void EnVbBall_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnVbBall* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_vb_ball.c", 604);
    if (1) {} // needed for match
    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vb_ball.c", 607),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (self->actor.params >= 200) {
        gSPDisplayList(POLY_OPA_DISP++, SEGMENTED_TO_VIRTUAL(gVolvagiaRibsDL));
    } else {
        gSPDisplayList(POLY_OPA_DISP++, SEGMENTED_TO_VIRTUAL(gVolvagiaRockDL));
        func_80094044(globalCtx->state.gfxCtx);

        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, (s8)self->shadowOpacity);
        Matrix_Translate(self->actor.world.pos.x, 100.0f, self->actor.world.pos.z, MTXMODE_NEW);
        Matrix_Scale(self->shadowSize, 1.0f, self->shadowSize, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_vb_ball.c", 626),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gCircleShadowDL));
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_vb_ball.c", 632);
}
