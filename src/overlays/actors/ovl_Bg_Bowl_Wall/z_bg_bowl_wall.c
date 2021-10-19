/*
 * File: z_bg_bowl_wall.c
 * Overlay: Bg_Bowl_Wall
 * Description: Bombchu Bowling Alley Wall
 */

#include "z_bg_bowl_wall.h"
#include "overlays/actors/ovl_En_Wall_Tubo/z_en_wall_tubo.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"
#include "objects/object_bowl/object_bowl.h"
#include "vt.h"

#define FLAGS 0x00000030

#define THIS ((BgBowlWall*)thisx)

void BgBowlWall_Init(Actor* thisx, GlobalContext* globalCtx);
void BgBowlWall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgBowlWall_Update(Actor* thisx, GlobalContext* globalCtx);
void BgBowlWall_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgBowlWall_SpawnBullseyes(BgBowlWall* self, GlobalContext* globalCtx);
void BgBowlWall_WaitForHit(BgBowlWall* self, GlobalContext* globalCtx);
void BgBowlWall_FallDoEffects(BgBowlWall* self, GlobalContext* globalCtx);
void BgBowlWall_FinishFall(BgBowlWall* self, GlobalContext* globalCtx);
void BgBowlWall_Reset(BgBowlWall* self, GlobalContext* globalCtx);

const ActorInit Bg_Bowl_Wall_InitVars = {
    ACTOR_BG_BOWL_WALL,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BOWL,
    sizeof(BgBowlWall),
    (ActorFunc)BgBowlWall_Init,
    (ActorFunc)BgBowlWall_Destroy,
    (ActorFunc)BgBowlWall_Update,
    (ActorFunc)BgBowlWall_Draw,
};

static Vec3f sBullseyeOffset[] = {
    { 0.0f, 210.0f, -20.0f },
    { 0.0f, 170.0f, -20.0f },
    { -170.0f, 0.0f, -20.0f },
    { 170.0f, 0.0f, -20.0f },
};

static s16 sTargetRot[] = { 0x0000, 0x0000, 0x3FFF, -0x3FFF };

void BgBowlWall_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgBowlWall* self = THIS;
    s32 pad1;
    s32 pad2;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);

    if (self->dyna.actor.params == 0) {
        CollisionHeader_GetVirtual(&gBowlingFirstAndFinalRoundCol, &colHeader);
    } else {
        CollisionHeader_GetVirtual(&gBowlingSecondRoundCol, &colHeader);
    }

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    self->initPos = self->dyna.actor.world.pos;
    osSyncPrintf("\n\n");
    osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ ボーリングおじゃま壁発生 ☆☆☆☆☆ %d\n" VT_RST, self->dyna.actor.params);
    self->actionFunc = BgBowlWall_SpawnBullseyes;
    self->dyna.actor.scale.x = self->dyna.actor.scale.y = self->dyna.actor.scale.z = 1.0f;
}

void BgBowlWall_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgBowlWall* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgBowlWall_SpawnBullseyes(BgBowlWall* self, GlobalContext* globalCtx) {
    s32 pad;
    EnWallTubo* bullseye;
    Actor* lookForGirl;
    s16 type;

    type = self->dyna.actor.params;
    if (type != 0) {
        type += (s16)Rand_ZeroFloat(2.99f);
        self->dyna.actor.shape.rot.z = self->dyna.actor.world.rot.z = sTargetRot[type];
        osSyncPrintf("\n\n");
    }
    self->bullseyeCenter.x = sBullseyeOffset[type].x + self->dyna.actor.world.pos.x;
    self->bullseyeCenter.y = sBullseyeOffset[type].y + self->dyna.actor.world.pos.y;
    self->bullseyeCenter.z = sBullseyeOffset[type].z + self->dyna.actor.world.pos.z;
    if (1) {}
    bullseye = (EnWallTubo*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_EN_WALL_TUBO,
                                               self->bullseyeCenter.x, self->bullseyeCenter.y, self->bullseyeCenter.z,
                                               0, 0, 0, self->dyna.actor.params);
    if (bullseye != NULL) {
        bullseye->explosionCenter = self->bullseyeCenter;
        if (type != 0) {
            bullseye->explosionCenter = self->bullseyeCenter = self->dyna.actor.world.pos;
        }
        if (self->chuGirl == NULL) {
            lookForGirl = globalCtx->actorCtx.actorLists[ACTORCAT_NPC].head;
            while (lookForGirl != NULL) {
                if (lookForGirl->id != ACTOR_EN_BOM_BOWL_MAN) {
                    lookForGirl = lookForGirl->next;
                } else {
                    self->chuGirl = (EnBomBowlMan*)lookForGirl;
                    break;
                }
            }
        }
        self->actionFunc = BgBowlWall_WaitForHit;
    }
}

void BgBowlWall_WaitForHit(BgBowlWall* self, GlobalContext* globalCtx) {
    if (self->isHit) {
        self->actionFunc = BgBowlWall_FallDoEffects;
    }
}

void BgBowlWall_FallDoEffects(BgBowlWall* self, GlobalContext* globalCtx) {
    s16 pad;
    Vec3f effectAccel = { 0.0f, 0.1f, 0.0f };
    Vec3f effectVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effectPos;
    s16 quakeIndex;
    s32 wallFallen;
    s32 i;

    wallFallen = false;

    if (self->dyna.actor.params == 0) { // wall collapses backwards
        Math_SmoothStepToS(&self->dyna.actor.shape.rot.x, -0x3E80, 3, 500, 0);
        self->dyna.actor.world.rot.x = self->dyna.actor.shape.rot.x;
        if (self->dyna.actor.shape.rot.x < -0x3C1E) {
            wallFallen = true;
        }
    } else { // wall slides downwards
        Math_ApproachF(&self->dyna.actor.world.pos.y, self->initPos.y - 450.0f, 0.3f, 10.0f);
        if (self->dyna.actor.world.pos.y < (self->initPos.y - 400.0f)) {
            wallFallen = true;
        }
    }

    if (wallFallen) {
        for (i = 0; i < 15; i++) {
            effectPos.x = Rand_CenteredFloat(300.0f) + self->bullseyeCenter.x;
            effectPos.y = -100.0f;
            effectPos.z = Rand_CenteredFloat(400.0f) + self->bullseyeCenter.z;
            EffectSsBomb2_SpawnLayered(globalCtx, &effectPos, &effectVelocity, &effectAccel, 100, 30);
            effectPos.y = -50.0f;
            EffectSsHahen_SpawnBurst(globalCtx, &effectPos, 10.0f, 0, 50, 15, 3, HAHEN_OBJECT_DEFAULT, 10, NULL);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_IT_BOMB_EXPLOSION);
        }
        quakeIndex = Quake_Add(GET_ACTIVE_CAM(globalCtx), 1);
        Quake_SetSpeed(quakeIndex, 0x7FFF);
        Quake_SetQuakeValues(quakeIndex, 300, 0, 0, 0);
        Quake_SetCountdown(quakeIndex, 30);
        self->timer = 20;
        self->actionFunc = BgBowlWall_FinishFall;
    }
}

void BgBowlWall_FinishFall(BgBowlWall* self, GlobalContext* globalCtx) {
    if (self->timer >= 2) {
        if (self->dyna.actor.params == 0) {
            Math_SmoothStepToS(&self->dyna.actor.shape.rot.x, -0x3E80, 1, 200, 0);
        } else {
            Math_ApproachF(&self->dyna.actor.world.pos.y, self->initPos.y - 450.0f, 0.3f, 10.0f);
        }
    } else if (self->timer == 1) {
        self->dyna.actor.world.rot.x = self->dyna.actor.shape.rot.x = 0;
        self->dyna.actor.world.pos.y = self->initPos.y - 450.0f;
        self->chuGirl->wallStatus[self->dyna.actor.params] = 2;
        self->actionFunc = BgBowlWall_Reset;
    }
}

void BgBowlWall_Reset(BgBowlWall* self, GlobalContext* globalCtx) {
    if (self->chuGirl->wallStatus[self->dyna.actor.params] != 2) {
        Math_ApproachF(&self->dyna.actor.world.pos.y, self->initPos.y, 0.3f, 50.0f);
        if (fabsf(self->dyna.actor.world.pos.y - self->initPos.y) <= 10.0f) {
            self->dyna.actor.world.pos.y = self->initPos.y;
            self->isHit = false;
            self->actionFunc = BgBowlWall_SpawnBullseyes;
        }
    }
}

void BgBowlWall_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgBowlWall* self = THIS;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actionFunc(self, globalCtx);
}

void BgBowlWall_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgBowlWall* self = THIS;
    u32 frames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_bowl_wall.c", 441);

    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x8,
               Gfx_TexScroll(globalCtx->state.gfxCtx, 0, -2 * (frames = globalCtx->state.frames), 16, 16));
    gDPPipeSync(POLY_OPA_DISP++);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_bowl_wall.c", 453),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (self->dyna.actor.params == 0) {
        gSPDisplayList(POLY_OPA_DISP++, gBowlingRound1WallDL);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, gBowlingRound2WallDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_bowl_wall.c", 464);
}
