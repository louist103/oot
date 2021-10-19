/**
 * File: z_en_karebaba.c
 * Overlay: ovl_En_Karebaba
 * Description: Withered Deku Baba
 */

#include "z_en_karebaba.h"
#include "objects/object_dekubaba/object_dekubaba.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"

#define FLAGS 0x00000005

#define THIS ((EnKarebaba*)thisx)

void EnKarebaba_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKarebaba_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKarebaba_Update(Actor* thisx, GlobalContext* globalCtx);
void EnKarebaba_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnKarebaba_SetupGrow(EnKarebaba* self);
void EnKarebaba_SetupIdle(EnKarebaba* self);
void EnKarebaba_Grow(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_Idle(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_Awaken(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_Spin(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_Dying(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_DeadItemDrop(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_Retract(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_Dead(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_Regrow(EnKarebaba* self, GlobalContext* globalCtx);
void EnKarebaba_Upright(EnKarebaba* self, GlobalContext* globalCtx);

const ActorInit En_Karebaba_InitVars = {
    ACTOR_EN_KAREBABA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DEKUBABA,
    sizeof(EnKarebaba),
    (ActorFunc)EnKarebaba_Init,
    (ActorFunc)EnKarebaba_Destroy,
    (ActorFunc)EnKarebaba_Update,
    (ActorFunc)EnKarebaba_Draw,
};

static ColliderCylinderInit sBodyColliderInit = {
    {
        COLTYPE_HARD,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 7, 25, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sHeadColliderInit = {
    {
        COLTYPE_HARD,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 4, 25, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColCheckInfoInit = { 1, 15, 80, MASS_HEAVY };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2500, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 1, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x09, ICHAIN_STOP),
};

void EnKarebaba_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnKarebaba* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 22.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gDekuBabaSkel, &gDekuBabaFastChompAnim, self->jointTable,
                   self->morphTable, 8);
    Collider_InitCylinder(globalCtx, &self->bodyCollider);
    Collider_SetCylinder(globalCtx, &self->bodyCollider, &self->actor, &sBodyColliderInit);
    Collider_UpdateCylinder(&self->actor, &self->bodyCollider);
    Collider_InitCylinder(globalCtx, &self->headCollider);
    Collider_SetCylinder(globalCtx, &self->headCollider, &self->actor, &sHeadColliderInit);
    Collider_UpdateCylinder(&self->actor, &self->headCollider);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, DamageTable_Get(1), &sColCheckInfoInit);

    self->boundFloor = NULL;

    if (self->actor.params == 0) {
        EnKarebaba_SetupGrow(self);
    } else {
        EnKarebaba_SetupIdle(self);
    }
}

void EnKarebaba_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnKarebaba* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->bodyCollider);
    Collider_DestroyCylinder(globalCtx, &self->headCollider);
}

void EnKarebaba_ResetCollider(EnKarebaba* self) {
    self->bodyCollider.dim.radius = 7;
    self->bodyCollider.dim.height = 25;
    self->bodyCollider.base.colType = COLTYPE_HARD;
    self->bodyCollider.base.acFlags |= AC_HARD;
    self->bodyCollider.info.bumper.dmgFlags = ~0x00300000;
    self->headCollider.dim.height = 25;
}

void EnKarebaba_SetupGrow(EnKarebaba* self) {
    Actor_SetScale(&self->actor, 0.0f);
    self->actor.shape.rot.x = -0x4000;
    self->actionFunc = EnKarebaba_Grow;
    self->actor.world.pos.y = self->actor.home.pos.y + 14.0f;
}

void EnKarebaba_SetupIdle(EnKarebaba* self) {
    Actor_SetScale(&self->actor, 0.005f);
    self->actor.shape.rot.x = -0x4000;
    self->actionFunc = EnKarebaba_Idle;
    self->actor.world.pos.y = self->actor.home.pos.y + 14.0f;
}

void EnKarebaba_SetupAwaken(EnKarebaba* self) {
    Animation_Change(&self->skelAnime, &gDekuBabaFastChompAnim, 4.0f, 0.0f,
                     Animation_GetLastFrame(&gDekuBabaFastChompAnim), ANIMMODE_LOOP, -3.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DUMMY482);
    self->actionFunc = EnKarebaba_Awaken;
}

void EnKarebaba_SetupUpright(EnKarebaba* self) {
    if (self->actionFunc != EnKarebaba_Spin) {
        Actor_SetScale(&self->actor, 0.01f);
        self->bodyCollider.base.colType = COLTYPE_HIT6;
        self->bodyCollider.base.acFlags &= ~AC_HARD;
        self->bodyCollider.info.bumper.dmgFlags = !LINK_IS_ADULT ? 0x07C00710 : 0x0FC00710;
        self->bodyCollider.dim.radius = 15;
        self->bodyCollider.dim.height = 80;
        self->headCollider.dim.height = 80;
    }

    self->actor.params = 40;
    self->actionFunc = EnKarebaba_Upright;
}

void EnKarebaba_SetupSpin(EnKarebaba* self) {
    self->actor.params = 40;
    self->actionFunc = EnKarebaba_Spin;
}

void EnKarebaba_SetupDying(EnKarebaba* self) {
    self->actor.params = 0;
    self->actor.gravity = -0.8f;
    self->actor.velocity.y = 4.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
    self->actor.speedXZ = 3.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_JR_DEAD);
    self->actor.flags |= 0x30;
    self->actionFunc = EnKarebaba_Dying;
}

void EnKarebaba_SetupDeadItemDrop(EnKarebaba* self, GlobalContext* globalCtx) {
    Actor_SetScale(&self->actor, 0.03f);
    self->actor.shape.rot.x -= 0x4000;
    self->actor.shape.yOffset = 1000.0f;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.shape.shadowScale = 3.0f;
    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_MISC);
    self->actor.params = 200;
    self->actor.flags &= ~0x20;
    self->actionFunc = EnKarebaba_DeadItemDrop;
}

void EnKarebaba_SetupRetract(EnKarebaba* self) {
    Animation_Change(&self->skelAnime, &gDekuBabaFastChompAnim, -3.0f, Animation_GetLastFrame(&gDekuBabaFastChompAnim),
                     0.0f, ANIMMODE_ONCE, -3.0f);
    EnKarebaba_ResetCollider(self);
    self->actionFunc = EnKarebaba_Retract;
}

void EnKarebaba_SetupDead(EnKarebaba* self) {
    Animation_Change(&self->skelAnime, &gDekuBabaFastChompAnim, 0.0f, 0.0f, 0.0f, ANIMMODE_ONCE, 0.0f);
    EnKarebaba_ResetCollider(self);
    self->actor.shape.rot.x = -0x4000;
    self->actor.params = 200;
    self->actor.parent = NULL;
    self->actor.shape.shadowScale = 0.0f;
    Math_Vec3f_Copy(&self->actor.world.pos, &self->actor.home.pos);
    self->actionFunc = EnKarebaba_Dead;
}

void EnKarebaba_SetupRegrow(EnKarebaba* self) {
    self->actor.shape.yOffset = 0.0f;
    self->actor.shape.shadowScale = 22.0f;
    self->headCollider.dim.radius = sHeadColliderInit.dim.radius;
    Actor_SetScale(&self->actor, 0.0f);
    self->actionFunc = EnKarebaba_Regrow;
}

void EnKarebaba_Grow(EnKarebaba* self, GlobalContext* globalCtx) {
    f32 scale;

    self->actor.params++;
    scale = self->actor.params * 0.05f;
    Actor_SetScale(&self->actor, 0.005f * scale);
    self->actor.world.pos.y = self->actor.home.pos.y + (14.0f * scale);
    if (self->actor.params == 20) {
        EnKarebaba_SetupIdle(self);
    }
}

void EnKarebaba_Idle(EnKarebaba* self, GlobalContext* globalCtx) {
    if (self->actor.xzDistToPlayer < 200.0f && fabsf(self->actor.yDistToPlayer) < 30.0f) {
        EnKarebaba_SetupAwaken(self);
    }
}

void EnKarebaba_Awaken(EnKarebaba* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&self->actor.scale.x, 0.01f, 0.0005f);
    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 60.0f, 5.0f)) {
        EnKarebaba_SetupUpright(self);
    }
    self->actor.shape.rot.y += 0x1999;
    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.home.pos, 3.0f, 0, 12, 5, 1, HAHEN_OBJECT_DEFAULT, 10, NULL);
}

void EnKarebaba_Upright(EnKarebaba* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);

    if (self->actor.params != 0) {
        self->actor.params--;
    }

    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 12.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_JR_MOUTH);
    }

    if (self->bodyCollider.base.acFlags & AC_HIT) {
        EnKarebaba_SetupDying(self);
        Enemy_StartFinishingBlow(globalCtx, &self->actor);
    } else if (Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) > 240.0f) {
        EnKarebaba_SetupRetract(self);
    } else if (self->actor.params == 0) {
        EnKarebaba_SetupSpin(self);
    }
}

void EnKarebaba_Spin(EnKarebaba* self, GlobalContext* globalCtx) {
    s32 value;
    f32 cos60;

    if (self->actor.params != 0) {
        self->actor.params--;
    }

    SkelAnime_Update(&self->skelAnime);

    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 12.0f)) {
        if (1) {} // Here for matching purposes only.

        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_JR_MOUTH);
    }

    value = 20 - self->actor.params;
    value = 20 - ABS(value);

    if (value > 10) {
        value = 10;
    }

    self->headCollider.dim.radius = sHeadColliderInit.dim.radius + (value * 2);
    self->actor.shape.rot.x = 0xC000 - (value * 0x100);
    self->actor.shape.rot.y += value * 0x2C0;
    self->actor.world.pos.y = (Math_SinS(self->actor.shape.rot.x) * -60.0f) + self->actor.home.pos.y;

    cos60 = Math_CosS(self->actor.shape.rot.x) * 60.0f;

    self->actor.world.pos.x = (Math_SinS(self->actor.shape.rot.y) * cos60) + self->actor.home.pos.x;
    self->actor.world.pos.z = (Math_CosS(self->actor.shape.rot.y) * cos60) + self->actor.home.pos.z;

    if (self->bodyCollider.base.acFlags & AC_HIT) {
        EnKarebaba_SetupDying(self);
        Enemy_StartFinishingBlow(globalCtx, &self->actor);
    } else if (self->actor.params == 0) {
        EnKarebaba_SetupUpright(self);
    }
}

void EnKarebaba_Dying(EnKarebaba* self, GlobalContext* globalCtx) {
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    s32 i;
    Vec3f position;
    Vec3f rotation;

    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.1f);

    if (self->actor.params == 0) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4800, 0x71C);
        EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 3.0f, 0, 12, 5, 1, HAHEN_OBJECT_DEFAULT, 10, NULL);

        if (self->actor.scale.x > 0.005f && ((self->actor.bgCheckFlags & 2) || (self->actor.bgCheckFlags & 8))) {
            self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = 0.0f;
            self->actor.speedXZ = 0.0f;
            self->actor.flags &= ~5;
            EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 3.0f, 0, 12, 5, 15, HAHEN_OBJECT_DEFAULT, 10,
                                     NULL);
        }

        if (self->actor.bgCheckFlags & 2) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
            self->actor.params = 1;
        }
    } else if (self->actor.params == 1) {
        Math_Vec3f_Copy(&position, &self->actor.world.pos);
        rotation.z = Math_SinS(self->actor.shape.rot.x) * 20.0f;
        rotation.x = -20.0f * Math_CosS(self->actor.shape.rot.x) * Math_SinS(self->actor.shape.rot.y);
        rotation.y = -20.0f * Math_CosS(self->actor.shape.rot.x) * Math_CosS(self->actor.shape.rot.y);

        for (i = 0; i < 4; i++) {
            func_800286CC(globalCtx, &position, &zeroVec, &zeroVec, 500, 50);
            position.x += rotation.x;
            position.y += rotation.z;
            position.z += rotation.y;
        }

        func_800286CC(globalCtx, &self->actor.home.pos, &zeroVec, &zeroVec, 500, 100);
        EnKarebaba_SetupDeadItemDrop(self, globalCtx);
    }
}

void EnKarebaba_DeadItemDrop(EnKarebaba* self, GlobalContext* globalCtx) {
    if (self->actor.params != 0) {
        self->actor.params--;
    }

    if (Actor_HasParent(&self->actor, globalCtx) || self->actor.params == 0) {
        EnKarebaba_SetupDead(self);
    } else {
        func_8002F554(&self->actor, globalCtx, GI_STICKS_1);
    }
}

void EnKarebaba_Retract(EnKarebaba* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&self->actor.scale.x, 0.005f, 0.0005f);
    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 14.0f, 5.0f)) {
        EnKarebaba_SetupIdle(self);
    }

    self->actor.shape.rot.y += 0x1999;
    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.home.pos, 3.0f, 0, 12, 5, 1, HAHEN_OBJECT_DEFAULT, 10, NULL);
}

void EnKarebaba_Dead(EnKarebaba* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.params != 0) {
        self->actor.params--;
    }
    if (self->actor.params == 0) {
        EnKarebaba_SetupRegrow(self);
    }
}

void EnKarebaba_Regrow(EnKarebaba* self, GlobalContext* globalCtx) {
    f32 scaleFactor;

    self->actor.params++;
    scaleFactor = self->actor.params * 0.05f;
    Actor_SetScale(&self->actor, 0.005f * scaleFactor);
    self->actor.world.pos.y = self->actor.home.pos.y + (14.0f * scaleFactor);

    if (self->actor.params == 20) {
        self->actor.flags &= ~0x10;
        self->actor.flags |= 5;
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_ENEMY);
        EnKarebaba_SetupIdle(self);
    }
}

void EnKarebaba_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnKarebaba* self = THIS;
    f32 height;

    self->actionFunc(self, globalCtx);

    if (self->actionFunc != EnKarebaba_Dead) {
        if (self->actionFunc == EnKarebaba_Dying) {
            Actor_MoveForward(&self->actor);
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 15.0f, 10.0f, 5);
        } else {
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
            if (self->boundFloor == NULL) {
                self->boundFloor = self->actor.floorPoly;
            }
        }
        if (self->actionFunc != EnKarebaba_Dying && self->actionFunc != EnKarebaba_DeadItemDrop) {
            if (self->actionFunc != EnKarebaba_Regrow && self->actionFunc != EnKarebaba_Grow) {
                CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->headCollider.base);
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->bodyCollider.base);
            }
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->headCollider.base);
            Actor_SetFocus(&self->actor, (self->actor.scale.x * 10.0f) / 0.01f);
            height = self->actor.home.pos.y + 40.0f;
            self->actor.focus.pos.x = self->actor.home.pos.x;
            self->actor.focus.pos.y = CLAMP_MAX(self->actor.focus.pos.y, height);
            self->actor.focus.pos.z = self->actor.home.pos.z;
        }
    }
}

void EnKarebaba_DrawBaseShadow(EnKarebaba* self, GlobalContext* globalCtx) {
    MtxF mf;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1013);

    func_80094044(globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, 255);
    func_80038A28(self->boundFloor, self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, &mf);
    Matrix_Mult(&mf, MTXMODE_NEW);
    Matrix_Scale(0.15f, 1.0f, 0.15f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1029),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gCircleShadowDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1034);
}

void EnKarebaba_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Color_RGBA8 black = { 0, 0, 0, 0 };
    static Gfx* stemDLists[] = { gDekuBabaStemTopDL, gDekuBabaStemMiddleDL, gDekuBabaStemBaseDL };
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    EnKarebaba* self = THIS;
    s32 i;
    s32 stemSections;
    f32 scale;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1056);

    func_80093D18(globalCtx->state.gfxCtx);

    if (self->actionFunc == EnKarebaba_DeadItemDrop) {
        if (self->actor.params > 40 || (self->actor.params & 1)) {
            Matrix_Translate(0.0f, 0.0f, 200.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1066),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gDekuBabaStickDropDL);
        }
    } else if (self->actionFunc != EnKarebaba_Dead) {
        func_80026230(globalCtx, &black, 1, 2);
        SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, NULL, NULL);
        Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);

        if ((self->actionFunc == EnKarebaba_Regrow) || (self->actionFunc == EnKarebaba_Grow)) {
            scale = self->actor.params * 0.0005f;
        } else {
            scale = 0.01f;
        }

        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        Matrix_RotateRPY(self->actor.shape.rot.x, self->actor.shape.rot.y, 0, MTXMODE_APPLY);

        if (self->actionFunc == EnKarebaba_Dying) {
            stemSections = 2;
        } else {
            stemSections = 3;
        }

        for (i = 0; i < stemSections; i++) {
            Matrix_Translate(0.0f, 0.0f, -2000.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1116),
                      G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, stemDLists[i]);

            if (i == 0 && self->actionFunc == EnKarebaba_Dying) {
                Matrix_MultVec3f(&zeroVec, &self->actor.focus.pos);
            }
        }

        func_80026608(globalCtx);
    }

    func_80026230(globalCtx, &black, 1, 2);
    Matrix_Translate(self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, MTXMODE_NEW);

    if (self->actionFunc != EnKarebaba_Grow) {
        scale = 0.01f;
    }

    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_RotateY(self->actor.home.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1144),
              G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gDekuBabaBaseLeavesDL);

    if (self->actionFunc == EnKarebaba_Dying) {
        Matrix_RotateRPY(-0x4000, (s16)(self->actor.shape.rot.y - self->actor.home.rot.y), 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1155),
                  G_MTX_LOAD | G_MTX_NOPUSH | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gDekuBabaStemBaseDL);
    }

    func_80026608(globalCtx);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_karebaba.c", 1163);

    if (self->boundFloor != NULL) {
        EnKarebaba_DrawBaseShadow(self, globalCtx);
    }
}
