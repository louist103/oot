/*
 * File: z_en_anubice_fire.c
 * Overlay: ovl_En_Anubice_Fire
 * Description: Anubis Fire Attack
 */

#include "z_en_anubice_fire.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_anubice/object_anubice.h"

#define FLAGS 0x00000010

#define THIS ((EnAnubiceFire*)thisx)

void EnAnubiceFire_Init(Actor* thisx, GlobalContext* globalCtx);
void EnAnubiceFire_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnAnubiceFire_Update(Actor* thisx, GlobalContext* globalCtx);
void EnAnubiceFire_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809B26EC(EnAnubiceFire* self, GlobalContext* globalCtx);
void func_809B27D8(EnAnubiceFire* self, GlobalContext* globalCtx);
void func_809B2B48(EnAnubiceFire* self, GlobalContext* globalCtx);

const ActorInit En_Anubice_Fire_InitVars = {
    ACTOR_EN_ANUBICE_FIRE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ANUBICE,
    sizeof(EnAnubiceFire),
    (ActorFunc)EnAnubiceFire_Init,
    (ActorFunc)EnAnubiceFire_Destroy,
    (ActorFunc)EnAnubiceFire_Update,
    (ActorFunc)EnAnubiceFire_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x01, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 0, 0, 0, { 0, 0, 0 } },
};

void EnAnubiceFire_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubiceFire* self = THIS;
    s32 i;

    Collider_InitCylinder(globalCtx, &self->cylinder);
    Collider_SetCylinder(globalCtx, &self->cylinder, &self->actor, &sCylinderInit);

    self->unk_15A = 30;
    self->unk_154 = 2.0f;
    self->scale = 0.0f;

    for (i = 0; i < 6; i++) {
        self->unk_160[i] = self->actor.world.pos;
    }

    self->unk_15E = 0;
    self->actionFunc = func_809B26EC;
}

void EnAnubiceFire_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubiceFire* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->cylinder);
}

void func_809B26EC(EnAnubiceFire* self, GlobalContext* globalCtx) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };

    Matrix_Push();
    Matrix_RotateY(BINANG_TO_RAD(self->actor.world.rot.y), MTXMODE_NEW);
    Matrix_RotateX(BINANG_TO_RAD(self->actor.world.rot.x), MTXMODE_APPLY);
    velocity.z = 15.0f;
    Matrix_MultVec3f(&velocity, &self->actor.velocity);
    Matrix_Pop();

    self->actionFunc = func_809B27D8;
    self->actor.world.rot.x = self->actor.world.rot.y = self->actor.world.rot.z = 0;
}

void func_809B27D8(EnAnubiceFire* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;
    Color_RGBA8 primColor = { 255, 255, 0, 255 };
    Color_RGBA8 envColor = { 255, 0, 0, 255 };
    Vec3f sp84 = { 0.0f, 0.0f, 0.0f };
    Vec3f sp78 = { 0.0f, 0.0f, 0.0f };

    self->actor.world.rot.z += 5000;
    if (self->unk_15A == 0) {
        self->unk_154 = 0.0f;
    }

    Math_ApproachF(&self->scale, self->unk_154, 0.2f, 0.4f);
    if ((self->unk_15A == 0) && (self->scale < 0.1f)) {
        Actor_Kill(&self->actor);
    } else if ((self->actor.params == 0) && (self->cylinder.base.atFlags & 4)) {
        if (Player_HasMirrorShieldEquipped(globalCtx)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_IT_SHIELD_REFLECT_SW);
            self->cylinder.base.atFlags &= 0xFFE9;
            self->cylinder.base.atFlags |= 8;
            self->cylinder.info.toucher.dmgFlags = 2;
            self->unk_15A = 30;
            self->actor.params = 1;
            self->actor.velocity.x *= -1.0f;
            self->actor.velocity.y *= -0.5f;
            self->actor.velocity.z *= -1.0f;
        } else {
            self->unk_15A = 0;
            EffectSsBomb2_SpawnLayered(globalCtx, &self->actor.world.pos, &sp78, &sp84, 10, 5);
            self->actor.velocity.x = self->actor.velocity.y = self->actor.velocity.z = 0.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_ANUBIS_FIREBOMB);
            self->actionFunc = func_809B2B48;
        }
    } else if (!(self->scale < .4f)) {
        f32 scale = 1000.0f;
        f32 life = 10.0f;
        s32 i;

        for (i = 0; i < 10; i++) {
            pos.x = self->actor.world.pos.x + (Rand_ZeroOne() - 0.5f) * (self->scale * 20.0f);
            pos.y = self->actor.world.pos.y + (Rand_ZeroOne() - 0.5f) * (self->scale * 20.0f);
            pos.z = self->actor.world.pos.z;
            EffectSsKiraKira_SpawnDispersed(globalCtx, &pos, &velocity, &accel, &primColor, &envColor, scale, life);
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_ANUBIS_FIRE - SFX_FLAG);
    }
}

void func_809B2B48(EnAnubiceFire* self, GlobalContext* globalCtx) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;
    Color_RGBA8 primColor = { 255, 255, 0, 255 };
    Color_RGBA8 envColor = { 255, 0, 0, 255 };
    s32 pad;
    s32 i;

    if (self->unk_15C == 0) {
        for (i = 0; i < 20; i++) {
            pos.x = self->actor.world.pos.x;
            pos.y = self->actor.world.pos.y;
            pos.z = self->actor.world.pos.z;
            accel.x = Rand_CenteredFloat(8.0f);
            accel.y = Rand_CenteredFloat(2.0f);
            accel.z = Rand_CenteredFloat(8.0f);
            EffectSsKiraKira_SpawnDispersed(globalCtx, &pos, &velocity, &accel, &primColor, &envColor, 2000, 10);
        }

        self->unk_15C = 2;
        self->unk_15E++;
        if (self->unk_15E >= 6) {
            Actor_Kill(&self->actor);
        }
    }
}

void EnAnubiceFire_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubiceFire* self = THIS;
    s32 pad;
    s32 i;

    Actor_SetScale(&self->actor, self->scale);
    self->actionFunc(self, globalCtx);
    func_8002D7EC(&self->actor);
    self->unk_160[0] = self->actor.world.pos;

    if (1) {}

    for (i = 4; i >= 0; i--) {
        self->unk_160[i + 1] = self->unk_160[i];
    }

    if (self->unk_15A != 0) {
        self->unk_15A--;
    }

    if (self->unk_15C != 0) {
        self->unk_15C--;
    }

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 5.0f, 5.0f, 10.0f, 0x1D);
    if (!(self->scale < 0.6f || self->actionFunc == func_809B2B48)) {
        self->cylinder.dim.radius = self->scale * 15.0f + 5.0f;
        self->cylinder.dim.height = self->scale * 15.0f + 5.0f;
        self->cylinder.dim.yShift = self->scale * -0.75f + -15.0f;

        if (self->unk_15A != 0) {
            Collider_UpdateCylinder(&self->actor, &self->cylinder);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->cylinder.base);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->cylinder.base);
        }

        if (BgCheck_SphVsFirstPoly(&globalCtx->colCtx, &self->actor.world.pos, 30.0f)) {
            self->actor.velocity.x = self->actor.velocity.y = self->actor.velocity.z = 0.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_ANUBIS_FIREBOMB);
            self->actionFunc = func_809B2B48;
        }
    }
}

void EnAnubiceFire_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* D_809B3270[] = {
        gDust4Tex, gDust5Tex, gDust6Tex, gDust7Tex, gDust8Tex, gDust7Tex, gDust6Tex, gDust5Tex,
    };
    EnAnubiceFire* self = THIS;
    s32 pad[2];
    s32 i;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_anubice_fire.c", 503);
    func_80093D84(globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 0, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
    gDPPipeSync(POLY_XLU_DISP++);
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_809B3270[0]));

    Matrix_Push();
    for (i = self->unk_15E; i < 6; ++i) {
        f32 scale = self->actor.scale.x - (i * 0.2f);
        if (scale < 0.0f) {
            scale = 0.0f;
        }

        if (scale >= 0.1f) {
            Matrix_Translate(self->unk_160[i].x, self->unk_160[i].y, self->unk_160[i].z, MTXMODE_NEW);
            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_RotateZ(self->actor.world.rot.z + i * 1000.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_anubice_fire.c", 546),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_XLU_DISP++, gAnubiceFireAttackDL);
        }

        if (self->scale < 0.1f) {
            break;
        }
    }
    Matrix_Pop();

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_anubice_fire.c", 556);
}
