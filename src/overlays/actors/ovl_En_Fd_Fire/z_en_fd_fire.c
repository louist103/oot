#include "z_en_fd_fire.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000015

#define THIS ((EnFdFire*)thisx)

void EnFdFire_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFdFire_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFdFire_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFdFire_Draw(Actor* thisx, GlobalContext* globalCtx);
void EnFdFire_Disappear(EnFdFire* self, GlobalContext* globalCtx);
void func_80A0E70C(EnFdFire* self, GlobalContext* globalCtx);
void EnFdFire_DanceTowardsPlayer(EnFdFire* self, GlobalContext* globalCtx);
void EnFdFire_WaitToDie(EnFdFire* self, GlobalContext* globalCtx);

const ActorInit En_Fd_Fire_InitVars = {
    ACTOR_EN_FD_FIRE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(EnFdFire),
    (ActorFunc)EnFdFire_Init,
    (ActorFunc)EnFdFire_Destroy,
    (ActorFunc)EnFdFire_Update,
    (ActorFunc)EnFdFire_Draw,
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
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x01, 0x08 },
        { 0x0D840008, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 12, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInit = { 1, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(0, 0x0),
    /* Master sword  */ DMG_ENTRY(0, 0x0),
    /* Giant's Knife */ DMG_ENTRY(0, 0x0),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(4, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0x0),
    /* Giant spin    */ DMG_ENTRY(0, 0x0),
    /* Master spin   */ DMG_ENTRY(0, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(0, 0x0),
    /* Giant jump    */ DMG_ENTRY(0, 0x0),
    /* Master jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

void EnFdFire_UpdatePos(EnFdFire* self, Vec3f* targetPos) {
    f32 dist;
    f32 xDiff = targetPos->x - self->actor.world.pos.x;
    f32 yDiff = targetPos->y - self->actor.world.pos.y;
    f32 zDiff = targetPos->z - self->actor.world.pos.z;

    dist = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
    if (fabsf(dist) > fabsf(self->actor.speedXZ)) {
        self->actor.velocity.x = (xDiff / dist) * self->actor.speedXZ;
        self->actor.velocity.z = (zDiff / dist) * self->actor.speedXZ;
    } else {
        self->actor.velocity.x = 0.0f;
        self->actor.velocity.z = 0.0f;
    }

    self->actor.velocity.y += self->actor.gravity;
    if (!(self->actor.minVelocityY <= self->actor.velocity.y)) {
        self->actor.velocity.y = self->actor.minVelocityY;
    }
}

s32 EnFdFire_CheckCollider(EnFdFire* self, GlobalContext* globalCtx) {
    if (self->actionFunc == EnFdFire_Disappear) {
        return false;
    }

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        return true;
    }

    if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
        self->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        return true;
    }
    return false;
}

void EnFdFire_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFdFire* self = THIS;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &sDamageTable, &sColChkInit);
    self->actor.flags &= ~1;
    self->actor.gravity = -0.6f;
    self->actor.speedXZ = 5.0f;
    self->actor.velocity.y = 12.0f;
    self->spawnRadius = Math_Vec3f_DistXYZ(&self->actor.world.pos, &player->actor.world.pos);
    self->scale = 3.0f;
    self->tile2Y = (s16)Rand_ZeroFloat(5.0f) - 25;
    self->actionFunc = func_80A0E70C;
}

void EnFdFire_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFdFire* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80A0E70C(EnFdFire* self, GlobalContext* globalCtx) {
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f targetPos = self->actor.parent->world.pos;

    targetPos.x += self->spawnRadius * Math_SinS(self->actor.world.rot.y);
    targetPos.z += self->spawnRadius * Math_CosS(self->actor.world.rot.y);
    EnFdFire_UpdatePos(self, &targetPos);
    if (self->actor.bgCheckFlags & 1 && (!(self->actor.velocity.y > 0.0f))) {
        self->actor.velocity = velocity;
        self->actor.speedXZ = 0.0f;
        self->actor.bgCheckFlags &= ~1;
        if (self->actor.params & 0x8000) {
            self->deathTimer = 200;
            self->actionFunc = EnFdFire_DanceTowardsPlayer;
        } else {
            self->deathTimer = 300;
            self->actionFunc = EnFdFire_WaitToDie;
        }
    }
}

void EnFdFire_WaitToDie(EnFdFire* self, GlobalContext* globalCtx) {
    if (DECR(self->deathTimer) == 0) {
        self->actionFunc = EnFdFire_Disappear;
    }
}

void EnFdFire_DanceTowardsPlayer(EnFdFire* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 angles[] = {
        0.0f, 210.0f, 60.0f, 270.0f, 120.0f, 330.0f, 180.0f, 30.0f, 240.0f, 90.0f, 300.0f, 150.0f,
    };
    Vec3f pos;
    s16 idx;

    idx = ((globalCtx->state.frames / 10) + (self->actor.params & 0x7FFF)) % ARRAY_COUNT(angles);
    pos = player->actor.world.pos;
    pos.x += 120.0f * sinf(angles[idx]);
    pos.z += 120.0f * cosf(angles[idx]);

    if (DECR(self->deathTimer) == 0) {
        self->actionFunc = EnFdFire_Disappear;
    } else {
        Math_SmoothStepToS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &pos), 8, 0xFA0, 1);
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.4f, 1.0f, 0.0f);
        if (self->actor.speedXZ < 0.1f) {
            self->actor.speedXZ = 5.0f;
        }
        func_8002D868(&self->actor);
    }
}

void EnFdFire_Disappear(EnFdFire* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.6f, 9.0f, 0.0f);
    func_8002D868(&self->actor);
    Math_SmoothStepToF(&self->scale, 0.0f, 0.3f, 0.1f, 0.0f);
    self->actor.shape.shadowScale = 20.0f;
    self->actor.shape.shadowScale *= (self->scale / 3.0f);
    if (!(self->scale > 0.01f)) {
        Actor_Kill(&self->actor);
    }
}

void EnFdFire_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnFdFire* self = THIS;
    s32 pad;

    if (self->actionFunc != EnFdFire_Disappear) {
        if ((self->actor.parent->update == NULL) || EnFdFire_CheckCollider(self, globalCtx)) {
            self->actionFunc = EnFdFire_Disappear;
        }
    }

    func_8002D7EC(&self->actor);
    self->actionFunc(self, globalCtx);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 12.0f, 10.0f, 0.0f, 5);

    if (self->actionFunc != EnFdFire_Disappear) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void EnFdFire_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Color_RGBA8 primColors[] = {
        { 255, 255, 0, 255 },
        { 255, 255, 255, 255 },
    };
    Color_RGBA8 envColors[] = {
        { 255, 10, 0, 255 },
        { 0, 10, 255, 255 },
    };
    s32 pad;
    EnFdFire* self = THIS;
    Vec3f scale = { 0.0f, 0.0f, 0.0f };
    Vec3f sp90 = { 0.0f, 0.0f, 0.0f };
    s16 sp8E;
    f32 sp88;
    f32 sp84;
    f32 sp80;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fd_fire.c", 572);

    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
    sp8E = Math_Vec3f_Yaw(&scale, &self->actor.velocity) - Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx));
    sp84 = fabsf(Math_CosS(sp8E));
    sp88 = Math_SinS(sp8E);
    sp80 = Math_Vec3f_DistXZ(&scale, &self->actor.velocity) / 1.5f;
    if (1) {}
    if (1) {}
    if (1) {}
    Matrix_RotateY((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000) * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_RotateZ(((sp88 * -10.0f) * sp80) * (M_PI / 180.0f), MTXMODE_APPLY);
    scale.x = scale.y = scale.z = self->scale * 0.001f;
    Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);
    sp84 = sp80 * ((0.01f * -15.0f) * sp84) + 1.0f;
    if (sp84 < 0.1f) {
        sp84 = 0.1f;
    }
    Matrix_Scale(1.0f, sp84, 1.0f / sp84, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_fd_fire.c", 623),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x8,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0,
                                globalCtx->state.frames * self->tile2Y, 0x20, 0x80));
    gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, primColors[((self->actor.params & 0x8000) >> 0xF)].r,
                    primColors[((self->actor.params & 0x8000) >> 0xF)].g,
                    primColors[((self->actor.params & 0x8000) >> 0xF)].b,
                    primColors[((self->actor.params & 0x8000) >> 0xF)].a);
    gDPSetEnvColor(POLY_XLU_DISP++, envColors[((self->actor.params & 0x8000) >> 0xF)].r,
                   envColors[((self->actor.params & 0x8000) >> 0xF)].g,
                   envColors[((self->actor.params & 0x8000) >> 0xF)].b,
                   envColors[((self->actor.params & 0x8000) >> 0xF)].a);
    gDPPipeSync(POLY_XLU_DISP++);
    gSPDisplayList(POLY_XLU_DISP++, &gEffFire1DL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fd_fire.c", 672);
}
