#include "z_en_bubble.h"
#include "objects/object_bubble/object_bubble.h"

#define FLAGS 0x00000001

#define THIS ((EnBubble*)thisx)

void EnBubble_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBubble_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBubble_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBubble_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnBubble_Wait(EnBubble* self, GlobalContext* globalCtx);
void EnBubble_Pop(EnBubble* self, GlobalContext* globalCtx);
void EnBubble_Regrow(EnBubble* self, GlobalContext* globalCtx);

const ActorInit En_Bubble_InitVars = {
    ACTOR_EN_BUBBLE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BUBBLE,
    sizeof(EnBubble),
    (ActorFunc)EnBubble_Init,
    (ActorFunc)EnBubble_Destroy,
    (ActorFunc)EnBubble_Update,
    (ActorFunc)EnBubble_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x04 },
            { 0xFFCFD753, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 16 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00002824, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 16 }, 100 },
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
    2,
    sJntSphElementsInit,
};

static CollisionCheckInfoInit2 sColChkInfoInit2 = {
    1, 2, 25, 25, MASS_IMMOVABLE,
};

static Vec3f sEffectAccel = { 0.0f, -0.5f, 0.0f };

static Color_RGBA8 sEffectPrimColor = { 255, 255, 255, 255 };

static Color_RGBA8 sEffectEnvColor = { 150, 150, 150, 0 };

void EnBubble_SetDimensions(EnBubble* self, f32 dim) {
    f32 a;
    f32 b;
    f32 c;
    f32 d;

    self->actor.flags |= 1;
    Actor_SetScale(&self->actor, 1.0f);
    self->actor.shape.yOffset = 16.0f;
    self->graphicRotSpeed = 16.0f;
    self->graphicEccentricity = 0.08f;
    self->expansionWidth = dim;
    self->expansionHeight = dim;
    a = Rand_ZeroOne();
    b = Rand_ZeroOne();
    c = Rand_ZeroOne();
    self->unk_218 = 1.0f;
    self->unk_21C = 1.0f;
    d = (a * a) + (b * b) + (c * c);
    self->unk_1FC.x = a / d;
    self->unk_1FC.y = b / d;
    self->unk_1FC.z = c / d;
}

u32 func_809CBCBC(EnBubble* self) {
    ColliderInfo* info = &self->colliderSphere.elements[0].info;

    info->toucher.dmgFlags = 0x8;
    info->toucher.effect = 0;
    info->toucher.damage = 4;
    info->toucherFlags = TOUCH_ON;
    self->actor.velocity.y = 0.0f;
    return 6;
}

// only called in an unused actionFunc
u32 func_809CBCEC(EnBubble* self) {
    EnBubble_SetDimensions(self, -1.0f);
    return 12;
}

void EnBubble_DamagePlayer(EnBubble* self, GlobalContext* globalCtx) {
    s32 damage = -self->colliderSphere.elements[0].info.toucher.damage;

    globalCtx->damagePlayer(globalCtx, damage);
    func_8002F7A0(globalCtx, &self->actor, 6.0f, self->actor.yawTowardsPlayer, 6.0f);
}

s32 EnBubble_Explosion(EnBubble* self, GlobalContext* globalCtx) {
    u32 i;
    Vec3f effectAccel;
    Vec3f effectVel;
    Vec3f effectPos;

    effectAccel = sEffectAccel;
    Math_SmoothStepToF(&self->expansionWidth, 4.0f, 0.1f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->expansionHeight, 4.0f, 0.1f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->graphicRotSpeed, 54.0f, 0.1f, 1000.0f, 0.0f);
    Math_SmoothStepToF(&self->graphicEccentricity, 0.2f, 0.1f, 1000.0f, 0.0f);
    self->actor.shape.yOffset = ((self->expansionHeight + 1.0f) * 16.0f);

    if (DECR(self->explosionCountdown) != 0) {
        return -1;
    }
    effectPos.x = self->actor.world.pos.x;
    effectPos.y = self->actor.world.pos.y + self->actor.shape.yOffset;
    effectPos.z = self->actor.world.pos.z;
    for (i = 0; i < 20; i++) {
        effectVel.x = (Rand_ZeroOne() - 0.5f) * 7.0f;
        effectVel.y = Rand_ZeroOne() * 7.0f;
        effectVel.z = (Rand_ZeroOne() - 0.5f) * 7.0f;
        EffectSsDtBubble_SpawnCustomColor(globalCtx, &effectPos, &effectVel, &effectAccel, &sEffectPrimColor,
                                          &sEffectEnvColor, Rand_S16Offset(100, 50), 0x19, 0);
    }
    Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, 0x50);
    self->actor.flags &= ~0x1;
    return Rand_S16Offset(90, 60);
}

// only called in an unused actionFunc
u32 func_809CBFD4(EnBubble* self) {
    if (DECR(self->explosionCountdown) != 0) {
        return -1;
    }
    return func_809CBCEC(self);
}

// only called in an unused actionFunc
s32 func_809CC020(EnBubble* self) {
    self->expansionWidth += 1.0f / 12.0f;
    self->expansionHeight += 1.0f / 12.0f;

    if (DECR(self->explosionCountdown) != 0) {
        return false;
    }
    return true;
}

void EnBubble_Vec3fNormalizedRelfect(Vec3f* vec1, Vec3f* vec2, Vec3f* ret) {
    f32 norm;

    Math3D_Vec3fReflect(vec1, vec2, ret);
    norm = sqrtf((ret->x * ret->x) + (ret->y * ret->y) + (ret->z * ret->z));
    if (norm != 0.0f) {
        ret->x /= norm;
        ret->y /= norm;
        ret->z /= norm;
    } else {
        ret->x = ret->y = ret->z = 0.0f;
    }
}

void EnBubble_Vec3fNormalize(Vec3f* vec) {
    f32 norm = sqrt((vec->x * vec->x) + (vec->y * vec->y) + (vec->z * vec->z));

    if (norm != 0.0f) {
        vec->x /= norm;
        vec->y /= norm;
        vec->z /= norm;
    } else {
        vec->x = vec->y = vec->z = 0.0f;
    }
}

void EnBubble_Fly(EnBubble* self, GlobalContext* globalCtx) {
    CollisionPoly* sp94;
    Actor* bumpActor;
    Vec3f sp84;
    Vec3f sp78;
    Vec3f sp6C;
    Vec3f sp60;
    Vec3f sp54;
    f32 bounceSpeed;
    s32 bgId;
    u8 bounceCount;

    if (self->colliderSphere.elements[1].info.bumperFlags & BUMP_HIT) {
        bumpActor = self->colliderSphere.base.ac;
        self->normalizedBumpVelocity = bumpActor->velocity;
        EnBubble_Vec3fNormalize(&self->normalizedBumpVelocity);
        self->velocityFromBump.x += (self->normalizedBumpVelocity.x * 3.0f);
        self->velocityFromBump.y += (self->normalizedBumpVelocity.y * 3.0f);
        self->velocityFromBump.z += (self->normalizedBumpVelocity.z * 3.0f);
    }
    self->sinkSpeed -= 0.1f;
    if (self->sinkSpeed < self->actor.minVelocityY) {
        self->sinkSpeed = self->actor.minVelocityY;
    }
    sp54.x = self->velocityFromBounce.x + self->velocityFromBump.x;
    sp54.y = self->velocityFromBounce.y + self->velocityFromBump.y + self->sinkSpeed;
    sp54.z = self->velocityFromBounce.z + self->velocityFromBump.z;
    EnBubble_Vec3fNormalize(&sp54);

    sp78.x = self->actor.world.pos.x;
    sp78.y = self->actor.world.pos.y + self->actor.shape.yOffset;
    sp78.z = self->actor.world.pos.z;
    sp6C = sp78;

    sp6C.x += (sp54.x * 24.0f);
    sp6C.y += (sp54.y * 24.0f);
    sp6C.z += (sp54.z * 24.0f);
    if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &sp78, &sp6C, &sp84, &sp94, true, true, true, false, &bgId)) {
        sp60.x = COLPOLY_GET_NORMAL(sp94->normal.x);
        sp60.y = COLPOLY_GET_NORMAL(sp94->normal.y);
        sp60.z = COLPOLY_GET_NORMAL(sp94->normal.z);
        EnBubble_Vec3fNormalizedRelfect(&sp54, &sp60, &sp54);
        self->bounceDirection = sp54;
        bounceCount = self->bounceCount;
        self->bounceCount = ++bounceCount;
        if (bounceCount > (s16)(Rand_ZeroOne() * 10.0f)) {
            self->bounceCount = 0;
        }
        bounceSpeed = (self->bounceCount == 0) ? 3.6000001f : 3.0f;
        self->velocityFromBump.x = self->velocityFromBump.y = self->velocityFromBump.z = 0.0f;
        self->velocityFromBounce.x = (self->bounceDirection.x * bounceSpeed);
        self->velocityFromBounce.y = (self->bounceDirection.y * bounceSpeed);
        self->velocityFromBounce.z = (self->bounceDirection.z * bounceSpeed);
        self->sinkSpeed = 0.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_AWA_BOUND);
        self->graphicRotSpeed = 128.0f;
        self->graphicEccentricity = 0.48f;
    } else if (self->actor.bgCheckFlags & 0x20 && sp54.y < 0.0f) {
        sp60.x = sp60.z = 0.0f;
        sp60.y = 1.0f;
        EnBubble_Vec3fNormalizedRelfect(&sp54, &sp60, &sp54);
        self->bounceDirection = sp54;
        bounceCount = self->bounceCount;
        self->bounceCount = ++bounceCount;
        if (bounceCount > (s16)(Rand_ZeroOne() * 10.0f)) {
            self->bounceCount = 0;
        }
        bounceSpeed = (self->bounceCount == 0) ? 3.6000001f : 3.0f;
        self->velocityFromBump.x = self->velocityFromBump.y = self->velocityFromBump.z = 0.0f;
        self->velocityFromBounce.x = (self->bounceDirection.x * bounceSpeed);
        self->velocityFromBounce.y = (self->bounceDirection.y * bounceSpeed);
        self->velocityFromBounce.z = (self->bounceDirection.z * bounceSpeed);
        self->sinkSpeed = 0.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_AWA_BOUND);
        self->graphicRotSpeed = 128.0f;
        self->graphicEccentricity = 0.48f;
    }
    self->actor.velocity.x = self->velocityFromBounce.x + self->velocityFromBump.x;
    self->actor.velocity.y = self->velocityFromBounce.y + self->velocityFromBump.y + self->sinkSpeed;
    self->actor.velocity.z = self->velocityFromBounce.z + self->velocityFromBump.z;
    Math_ApproachF(&self->velocityFromBump.x, 0.0f, 0.3f, 0.1f);
    Math_ApproachF(&self->velocityFromBump.y, 0.0f, 0.3f, 0.1f);
    Math_ApproachF(&self->velocityFromBump.z, 0.0f, 0.3f, 0.1f);
}

u32 func_809CC648(EnBubble* self) {
    if (((self->colliderSphere.base.acFlags & AC_HIT) != 0) == false) {
        return false;
    }
    self->colliderSphere.base.acFlags &= ~AC_HIT;
    if (self->colliderSphere.elements[1].info.bumperFlags & BUMP_HIT) {
        self->unk_1F0.x = self->colliderSphere.base.ac->velocity.x / 10.0f;
        self->unk_1F0.y = self->colliderSphere.base.ac->velocity.y / 10.0f;
        self->unk_1F0.z = self->colliderSphere.base.ac->velocity.z / 10.0f;
        self->graphicRotSpeed = 128.0f;
        self->graphicEccentricity = 0.48f;
        return false;
    }
    self->unk_208 = 8;
    return true;
}

u32 EnBubble_DetectPop(EnBubble* self, GlobalContext* globalCtx) {
    if (DECR(self->unk_208) != 0 || self->actionFunc == EnBubble_Pop) {
        return false;
    }
    if (self->colliderSphere.base.ocFlags2 & OC2_HIT_PLAYER) {
        self->colliderSphere.base.ocFlags2 &= ~OC2_HIT_PLAYER;
        EnBubble_DamagePlayer(self, globalCtx);
        self->unk_208 = 8;
        return true;
    }
    return func_809CC648(self);
}

void func_809CC774(EnBubble* self) {
    ColliderJntSphElementDim* dim;
    Vec3f src;
    Vec3f dest;

    dim = &self->colliderSphere.elements[0].dim;
    src.x = dim->modelSphere.center.x;
    src.y = dim->modelSphere.center.y;
    src.z = dim->modelSphere.center.z;

    Matrix_MultVec3f(&src, &dest);
    dim->worldSphere.center.x = dest.x;
    dim->worldSphere.center.y = dest.y;
    dim->worldSphere.center.z = dest.z;
    dim->worldSphere.radius = dim->modelSphere.radius * (1.0f + self->expansionWidth);
    self->colliderSphere.elements[1].dim = *dim;
}

void EnBubble_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBubble* self = THIS;
    u32 pad;

    ActorShape_Init(&self->actor.shape, 16.0f, ActorShadow_DrawCircle, 0.2f);
    Collider_InitJntSph(globalCtx, &self->colliderSphere);
    Collider_SetJntSph(globalCtx, &self->colliderSphere, &self->actor, &sJntSphInit, self->colliderSphereItems);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(9), &sColChkInfoInit2);
    self->actor.naviEnemyId = 0x16;
    self->bounceDirection.x = Rand_ZeroOne();
    self->bounceDirection.y = Rand_ZeroOne();
    self->bounceDirection.z = Rand_ZeroOne();
    EnBubble_Vec3fNormalize(&self->bounceDirection);
    self->velocityFromBounce.x = self->bounceDirection.x * 3.0f;
    self->velocityFromBounce.y = self->bounceDirection.y * 3.0f;
    self->velocityFromBounce.z = self->bounceDirection.z * 3.0f;
    EnBubble_SetDimensions(self, 0.0f);
    self->actionFunc = EnBubble_Wait;
}

void EnBubble_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBubble* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->colliderSphere);
}

void EnBubble_Wait(EnBubble* self, GlobalContext* globalCtx) {
    if (EnBubble_DetectPop(self, globalCtx)) {
        self->explosionCountdown = func_809CBCBC(self);
        self->actionFunc = EnBubble_Pop;
    } else {
        EnBubble_Fly(self, globalCtx);
        self->actor.shape.yOffset = ((self->expansionHeight + 1.0f) * 16.0f);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderSphere.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderSphere.base);
    }
}

void EnBubble_Pop(EnBubble* self, GlobalContext* globalCtx) {
    if (EnBubble_Explosion(self, globalCtx) >= 0) {
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 60, NA_SE_EN_AWA_BREAK);
        Actor_Kill(&self->actor);
    }
}

// unused
void EnBubble_Disappear(EnBubble* self, GlobalContext* globalCtx) {
    s32 temp_v0;

    temp_v0 = func_809CBFD4(self);
    if (temp_v0 >= 0) {
        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        self->explosionCountdown = temp_v0;
        self->actionFunc = EnBubble_Regrow;
    }
}

// unused
void EnBubble_Regrow(EnBubble* self, GlobalContext* globalCtx) {
    if (func_809CC020(self)) {
        self->actionFunc = EnBubble_Wait;
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderSphere.base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderSphere.base);
}

void EnBubble_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBubble* self = THIS;

    func_8002D7EC(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 16.0f, 16.0f, 0.0f, 7);
    self->actionFunc(self, globalCtx);
    Actor_SetFocus(&self->actor, self->actor.shape.yOffset);
}

void EnBubble_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBubble* self = THIS;
    u32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bubble.c", 1175);

    if (self->actionFunc != EnBubble_Disappear) {
        func_80093D84(globalCtx->state.gfxCtx);
        Math_SmoothStepToF(&self->graphicRotSpeed, 16.0f, 0.2f, 1000.0f, 0.0f);
        Math_SmoothStepToF(&self->graphicEccentricity, 0.08f, 0.2f, 1000.0f, 0.0f);
        func_800D1FD4(&globalCtx->mf_11DA0);

        Matrix_Scale(self->expansionWidth + 1.0f, self->expansionHeight + 1.0f, 1.0f, MTXMODE_APPLY);
        Matrix_RotateZ(((f32)globalCtx->state.frames * (M_PI / 180.0f)) * self->graphicRotSpeed, MTXMODE_APPLY);
        Matrix_Scale(self->graphicEccentricity + 1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
        Matrix_RotateZ((-(f32)globalCtx->state.frames * (M_PI / 180.0f)) * self->graphicRotSpeed, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_bubble.c", 1220),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gBubbleDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bubble.c", 1226);

    if (self->actionFunc != EnBubble_Disappear) {
        self->actor.shape.shadowScale = (f32)((self->expansionWidth + 1.0f) * 0.2f);
        func_809CC774(self);
    }
}
