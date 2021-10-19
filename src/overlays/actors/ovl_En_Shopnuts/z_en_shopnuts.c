#include "z_en_shopnuts.h"
#include "objects/object_shopnuts/object_shopnuts.h"

#define FLAGS 0x00000005

#define THIS ((EnShopnuts*)thisx)

void EnShopnuts_Init(Actor* thisx, GlobalContext* globalCtx);
void EnShopnuts_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnShopnuts_Update(Actor* thisx, GlobalContext* globalCtx);
void EnShopnuts_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnShopnuts_SetupWait(EnShopnuts* self);
void EnShopnuts_Wait(EnShopnuts* self, GlobalContext* globalCtx);
void EnShopnuts_LookAround(EnShopnuts* self, GlobalContext* globalCtx);
void EnShopnuts_Stand(EnShopnuts* self, GlobalContext* globalCtx);
void EnShopnuts_ThrowNut(EnShopnuts* self, GlobalContext* globalCtx);
void EnShopnuts_Burrow(EnShopnuts* self, GlobalContext* globalCtx);
void EnShopnuts_SpawnSalesman(EnShopnuts* self, GlobalContext* globalCtx);

const ActorInit En_Shopnuts_InitVars = {
    ACTOR_EN_SHOPNUTS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SHOPNUTS,
    sizeof(EnShopnuts),
    (ActorFunc)EnShopnuts_Init,
    (ActorFunc)EnShopnuts_Destroy,
    (ActorFunc)EnShopnuts_Update,
    (ActorFunc)EnShopnuts_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT6,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
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
    { 20, 40, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 1, 20, 40, 0xFE };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x4E, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -1, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2600, ICHAIN_STOP),
};

void EnShopnuts_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnShopnuts* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gBusinessScrubSkel, &gBusinessScrubAnim_4574, self->jointTable,
                       self->morphTable, 18);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);

    if (((self->actor.params == 0x0002) && (gSaveContext.itemGetInf[0] & 0x800)) ||
        ((self->actor.params == 0x0009) && (gSaveContext.infTable[25] & 4)) ||
        ((self->actor.params == 0x000A) && (gSaveContext.infTable[25] & 8))) {
        Actor_Kill(&self->actor);
    } else {
        EnShopnuts_SetupWait(self);
    }
}

void EnShopnuts_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnShopnuts* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnShopnuts_SetupWait(EnShopnuts* self) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, &gBusinessScrubAnim_139C, 0.0f);
    self->animFlagAndTimer = Rand_S16Offset(100, 50);
    self->collider.dim.height = 5;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnShopnuts_Wait;
}

void EnShopnuts_SetupLookAround(EnShopnuts* self) {
    Animation_PlayLoop(&self->skelAnime, &gBusinessScrubLookAroundAnim);
    self->animFlagAndTimer = 2;
    self->actionFunc = EnShopnuts_LookAround;
}

void EnShopnuts_SetupThrowNut(EnShopnuts* self) {
    Animation_PlayOnce(&self->skelAnime, &gBusinessScrubAnim_1EC);
    self->actionFunc = EnShopnuts_ThrowNut;
}

void EnShopnuts_SetupStand(EnShopnuts* self) {
    Animation_MorphToLoop(&self->skelAnime, &gBusinessScrubAnim_4574, -3.0f);
    if (self->actionFunc == EnShopnuts_ThrowNut) {
        self->animFlagAndTimer = 2 | 0x1000; // sets timer and flag
    } else {
        self->animFlagAndTimer = 1;
    }
    self->actionFunc = EnShopnuts_Stand;
}

void EnShopnuts_SetupBurrow(EnShopnuts* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBusinessScrubAnim_39C, -5.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DOWN);
    self->actionFunc = EnShopnuts_Burrow;
}

void EnShopnuts_SetupSpawnSalesman(EnShopnuts* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBusinessScrubRotateAnim, -3.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DAMAGE);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnShopnuts_SpawnSalesman;
}

void EnShopnuts_Wait(EnShopnuts* self, GlobalContext* globalCtx) {
    s32 hasSlowPlaybackSpeed = false;

    if (self->skelAnime.playSpeed < 0.5f) {
        hasSlowPlaybackSpeed = true;
    }
    if (hasSlowPlaybackSpeed && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (Animation_OnFrame(&self->skelAnime, 9.0f)) {
        self->collider.base.acFlags |= AC_ON;
    } else if (Animation_OnFrame(&self->skelAnime, 8.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
    }

    self->collider.dim.height = ((CLAMP(self->skelAnime.curFrame, 9.0f, 13.0f) - 9.0f) * 9.0f) + 5.0f;
    if (!hasSlowPlaybackSpeed && (self->actor.xzDistToPlayer < 120.0f)) {
        EnShopnuts_SetupBurrow(self);
    } else if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.xzDistToPlayer < 120.0f) {
            EnShopnuts_SetupBurrow(self);
        } else if ((self->animFlagAndTimer == 0) && (self->actor.xzDistToPlayer > 320.0f)) {
            EnShopnuts_SetupLookAround(self);
        } else {
            EnShopnuts_SetupStand(self);
        }
    }
    if (hasSlowPlaybackSpeed &&
        ((self->actor.xzDistToPlayer > 160.0f) && (fabsf(self->actor.yDistToPlayer) < 120.0f)) &&
        ((self->animFlagAndTimer == 0) || (self->actor.xzDistToPlayer < 480.0f))) {
        self->skelAnime.playSpeed = 1.0f;
    }
}

void EnShopnuts_LookAround(EnShopnuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if ((self->actor.xzDistToPlayer < 120.0f) || (self->animFlagAndTimer == 0)) {
        EnShopnuts_SetupBurrow(self);
    }
}

void EnShopnuts_Stand(EnShopnuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (!(self->animFlagAndTimer & 0x1000)) {
        Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    }
    if ((self->actor.xzDistToPlayer < 120.0f) || (self->animFlagAndTimer == 0x1000)) {
        EnShopnuts_SetupBurrow(self);
    } else if (self->animFlagAndTimer == 0) {
        EnShopnuts_SetupThrowNut(self);
    }
}

void EnShopnuts_ThrowNut(EnShopnuts* self, GlobalContext* globalCtx) {
    Vec3f spawnPos;

    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    if (self->actor.xzDistToPlayer < 120.0f) {
        EnShopnuts_SetupBurrow(self);
    } else if (SkelAnime_Update(&self->skelAnime)) {
        EnShopnuts_SetupStand(self);
    } else if (Animation_OnFrame(&self->skelAnime, 6.0f)) {
        spawnPos.x = self->actor.world.pos.x + (Math_SinS(self->actor.shape.rot.y) * 23.0f);
        spawnPos.y = self->actor.world.pos.y + 12.0f;
        spawnPos.z = self->actor.world.pos.z + (Math_CosS(self->actor.shape.rot.y) * 23.0f);
        if (Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_NUTSBALL, spawnPos.x, spawnPos.y, spawnPos.z,
                        self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z, 2) != NULL) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_THROW);
        }
    }
}

void EnShopnuts_Burrow(EnShopnuts* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnShopnuts_SetupWait(self);
    } else {
        self->collider.dim.height = ((4.0f - CLAMP_MAX(self->skelAnime.curFrame, 4.0f)) * 10.0f) + 5.0f;
    }
    if (Animation_OnFrame(&self->skelAnime, 4.0f)) {
        self->collider.base.acFlags &= ~AC_ON;
    }
}

void EnShopnuts_SpawnSalesman(EnShopnuts* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_DNS, self->actor.world.pos.x, self->actor.world.pos.y,
                    self->actor.world.pos.z, self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z,
                    self->actor.params);
        Actor_Kill(&self->actor);
    } else {
        Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    }
}

void EnShopnuts_ColliderCheck(EnShopnuts* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);
        EnShopnuts_SetupSpawnSalesman(self);
    } else if (globalCtx->actorCtx.unk_02 != 0) {
        EnShopnuts_SetupSpawnSalesman(self);
    }
}

void EnShopnuts_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnShopnuts* self = THIS;

    EnShopnuts_ColliderCheck(self, globalCtx);
    self->actionFunc(self, globalCtx);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, self->collider.dim.radius, self->collider.dim.height, 4);
    if (self->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    if (self->actionFunc == EnShopnuts_Wait) {
        Actor_SetFocus(&self->actor, self->skelAnime.curFrame);
    } else if (self->actionFunc == EnShopnuts_Burrow) {
        Actor_SetFocus(&self->actor,
                       20.0f - ((self->skelAnime.curFrame * 20.0f) / Animation_GetLastFrame(&gBusinessScrubAnim_39C)));
    } else {
        Actor_SetFocus(&self->actor, 20.0f);
    }
}

s32 EnShopnuts_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx) {
    EnShopnuts* self = THIS;

    if ((limbIndex == 9) && (self->actionFunc == EnShopnuts_ThrowNut)) {
        *dList = NULL;
    }
    return 0;
}

void EnShopnuts_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnShopnuts* self = THIS;

    f32 curFrame;
    f32 x;
    f32 y;
    f32 z;

    if ((limbIndex == 9) && (self->actionFunc == EnShopnuts_ThrowNut)) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_shopnuts.c", 682);
        curFrame = self->skelAnime.curFrame;
        if (curFrame <= 6.0f) {
            y = 1.0f - (curFrame * 0.0833f);
            x = z = (curFrame * 0.1167f) + 1.0f;
        } else if (curFrame <= 7.0f) {
            curFrame -= 6.0f;
            y = 0.5f + curFrame;
            x = z = 1.7f - (curFrame * 0.7f);
        } else if (curFrame <= 10.0f) {
            y = 1.5f - ((curFrame - 7.0f) * 0.1667f);
            x = z = 1.0f;
        } else {
            x = y = z = 1.0f;
        }

        Matrix_Scale(x, y, z, MTXMODE_APPLY);
        if (1) {}
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_shopnuts.c", 714),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gBusinessScrubNoseDL);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_shopnuts.c", 717);
    }
}

void EnShopnuts_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnShopnuts* self = THIS;

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnShopnuts_OverrideLimbDraw, EnShopnuts_PostLimbDraw, self);
}
