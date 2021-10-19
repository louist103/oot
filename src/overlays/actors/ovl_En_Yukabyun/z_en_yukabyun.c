/*
 * File: z_en_yukabyun.c
 * Overlay: ovl_En_Yukabyun
 * Description: Flying floor tile
 */

#include "z_en_yukabyun.h"
#include "objects/object_yukabyun/object_yukabyun.h"

#define FLAGS 0x00000010

#define THIS ((EnYukabyun*)thisx)

void EnYukabyun_Init(Actor* thisx, GlobalContext* globalCtx);
void EnYukabyun_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnYukabyun_Update(Actor* thisx, GlobalContext* globalCtx);
void EnYukabyun_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B43A94(EnYukabyun* self, GlobalContext* globalCtx);
void func_80B43AD4(EnYukabyun* self, GlobalContext* globalCtx);
void func_80B43B6C(EnYukabyun* self, GlobalContext* globalCtx);

const ActorInit En_Yukabyun_InitVars = {
    ACTOR_EN_YUKABYUN,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_YUKABYUN,
    sizeof(EnYukabyun),
    (ActorFunc)EnYukabyun_Init,
    (ActorFunc)EnYukabyun_Destroy,
    (ActorFunc)EnYukabyun_Update,
    (ActorFunc)EnYukabyun_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_NO_PUSH | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_ON,
        OCELEM_ON,
    },
    { 28, 8, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 16, ICHAIN_STOP),
};

static void* D_80B43F64[] = { gFloorTileEnemyTopTex, gFloorTileEnemyBottomTex };

void EnYukabyun_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnYukabyun* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.4f);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.params++;
    self->unk_152 = 0;
    self->unk_150 = (u8)(self->actor.params) * 0xA + 0x14;
    self->actionfunc = func_80B43A94;
}

void EnYukabyun_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnYukabyun* self = THIS;
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80B43A94(EnYukabyun* self, GlobalContext* globalCtx) {
    if (self->unk_150 != 0) {
        self->unk_150--;
    }
    if (self->unk_150 == 0) {
        self->actor.flags |= 0x1005;
        self->actionfunc = func_80B43AD4;
    }
}

void func_80B43AD4(EnYukabyun* self, GlobalContext* globalCtx) {
    self->unk_150 += 0xA0;
    self->actor.shape.rot.y += self->unk_150;
    if (self->unk_150 >= 0x2000) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.speedXZ = 10.0f;
        self->actionfunc = func_80B43B6C;
    }
    Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 30.0f, 1.0f);
    func_8002F974(&self->actor, NA_SE_EN_YUKABYUN_FLY - SFX_FLAG);
}

void func_80B43B6C(EnYukabyun* self, GlobalContext* globalCtx) {
    self->actor.shape.rot.y += self->unk_150;
    if (self->actor.xzDistToPlayer > 5000.0f) {
        Actor_Kill(&self->actor);
        return;
    }
    func_8002F974(&self->actor, NA_SE_EN_YUKABYUN_FLY - SFX_FLAG);
}

void EnYukabyun_Break(EnYukabyun* self, GlobalContext* globalCtx) {
    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 8.0f, 0, 1300, 300, 15, OBJECT_YUKABYUN, 10,
                             gFloorTileEnemyFragmentDL);
    Actor_Kill(&self->actor);
}

void EnYukabyun_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnYukabyun* self = THIS;
    s32 pad;

    if (((self->collider.base.atFlags & AT_HIT) || (self->collider.base.acFlags & AC_HIT) ||
         ((self->collider.base.ocFlags1 & OC1_HIT) && !(self->collider.base.oc->id == ACTOR_EN_YUKABYUN))) ||
        ((self->actionfunc == func_80B43B6C) && (self->actor.bgCheckFlags & 8))) {
        self->collider.base.atFlags &= ~AT_HIT;
        self->collider.base.acFlags &= ~AC_HIT;
        self->collider.base.ocFlags1 &= ~OC1_HIT;
        self->actor.flags &= ~0x5;
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 30, NA_SE_EN_OCTAROCK_ROCK);
        self->actionfunc = EnYukabyun_Break;
    }

    self->actionfunc(self, globalCtx);
    Actor_MoveForward(&self->actor);

    if (!(self->actionfunc == func_80B43A94 || self->actionfunc == EnYukabyun_Break)) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 5.0f, 20.0f, 8.0f, 5);
        Collider_UpdateCylinder(&self->actor, &self->collider);

        self->actor.flags |= 0x1000000;

        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    Actor_SetFocus(&self->actor, 4.0f);
}

void EnYukabyun_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnYukabyun* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_yukabyun.c", 366);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80B43F64[self->unk_152]));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_yukabyun.c", 373),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gFloorTileEnemyDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_yukabyun.c", 378);
}
