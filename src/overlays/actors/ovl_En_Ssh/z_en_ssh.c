#include "z_en_ssh.h"
#include "objects/object_ssh/object_ssh.h"

#define FLAGS 0x00000035

#define THIS ((EnSsh*)thisx)

#define SSH_STATE_STUNNED (1 << 0)
#define SSH_STATE_GROUND_START (1 << 2)
#define SSH_STATE_ATTACKED (1 << 3)
#define SSH_STATE_SPIN (1 << 4)

typedef enum {
    SSH_ANIM_UNK0, // Unused animation. Possibly being knocked back?
    SSH_ANIM_UP,
    SSH_ANIM_WAIT,
    SSH_ANIM_LAND,
    SSH_ANIM_DROP,
    SSH_ANIM_UNK5, // Slower version of ANIM_DROP
    SSH_ANIM_UNK6  // Faster repeating version of ANIM_UNK0
} EnSshAnimation;

void EnSsh_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSsh_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSsh_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSsh_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnSsh_Idle(EnSsh* self, GlobalContext* globalCtx);
void EnSsh_Drop(EnSsh* self, GlobalContext* globalCtx);
void EnSsh_Return(EnSsh* self, GlobalContext* globalCtx);
void EnSsh_Start(EnSsh* self, GlobalContext* globalCtx);

static Vtx D_80B043C0[] = {
    VTX(-1, 0, 0, 0, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(1, 0, 0, 1024, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(1, 100, 0, 1024, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(-1, 100, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF),
};

static Gfx D_80B04400[] = {
    gsDPPipeSync(),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(D_80B043C0, 4, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSPEndDisplayList(),
};

const ActorInit En_Ssh_InitVars = {
    ACTOR_EN_SSH,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SSH,
    sizeof(EnSsh),
    (ActorFunc)EnSsh_Init,
    (ActorFunc)EnSsh_Destroy,
    (ActorFunc)EnSsh_Update,
    (ActorFunc)EnSsh_Draw,
};

static ColliderCylinderInit sCylinderInit1 = {
    {
        COLTYPE_HIT6,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 32, 50, -24, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 1, 0, 0, 0, MASS_IMMOVABLE };

static ColliderCylinderInit sCylinderInit2 = {
    {
        COLTYPE_HIT6,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 20, 60, -30, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 1, { { 0, -240, 0 }, 28 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

void EnSsh_SetupAction(EnSsh* self, EnSshActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnSsh_SpawnShockwave(EnSsh* self, GlobalContext* globalCtx) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.floorHeight;
    pos.z = self->actor.world.pos.z;
    EffectSsBlast_SpawnWhiteCustomScale(globalCtx, &pos, &zeroVec, &zeroVec, 100, 220, 8);
}

s32 EnSsh_CreateBlureEffect(GlobalContext* globalCtx) {
    EffectBlureInit1 blureInit;
    u8 p1StartColor[] = { 255, 255, 255, 75 };
    u8 p2StartColor[] = { 255, 255, 255, 75 };
    u8 p1EndColor[] = { 255, 255, 255, 0 };
    u8 p2EndColor[] = { 255, 255, 255, 0 };
    s32 i;
    s32 blureIdx;

    for (i = 0; i < 4; i++) {
        blureInit.p1StartColor[i] = p1StartColor[i];
        blureInit.p2StartColor[i] = p2StartColor[i];
        blureInit.p1EndColor[i] = p1EndColor[i];
        blureInit.p2EndColor[i] = p2EndColor[i];
    }

    blureInit.elemDuration = 6;
    blureInit.unkFlag = 0;
    blureInit.calcMode = 3;

    Effect_Add(globalCtx, &blureIdx, EFFECT_BLURE1, 0, 0, &blureInit);
    return blureIdx;
}

s32 EnSsh_CheckCeilingPos(EnSsh* self, GlobalContext* globalCtx) {
    CollisionPoly* poly;
    s32 bgId;
    Vec3f posB;

    posB.x = self->actor.world.pos.x;
    posB.y = self->actor.world.pos.y + 1000.0f;
    posB.z = self->actor.world.pos.z;
    if (!BgCheck_EntityLineTest1(&globalCtx->colCtx, &self->actor.world.pos, &posB, &self->ceilingPos, &poly, false,
                                 false, true, true, &bgId)) {
        return false;
    } else {
        return true;
    }
}

void EnSsh_AddBlureVertex(EnSsh* self) {
    Vec3f p1base = { 834.0f, 834.0f, 0.0f };
    Vec3f p2base = { 834.0f, -584.0f, 0.0f };
    Vec3f p1;
    Vec3f p2;

    p1base.x *= self->colliderScale;
    p1base.y *= self->colliderScale;
    p1base.z *= self->colliderScale;
    p2base.x *= self->colliderScale;
    p2base.y *= self->colliderScale;
    p2base.z *= self->colliderScale;
    Matrix_Push();
    Matrix_MultVec3f(&p1base, &p1);
    Matrix_MultVec3f(&p2base, &p2);
    Matrix_Pop();
    EffectBlure_AddVertex(Effect_GetByIndex(self->blureIdx), &p1, &p2);
}

void EnSsh_AddBlureSpace(EnSsh* self) {
    EffectBlure_AddSpace(Effect_GetByIndex(self->blureIdx));
}

void EnSsh_InitColliders(EnSsh* self, GlobalContext* globalCtx) {
    ColliderCylinderInit* cylinders[6] = {
        &sCylinderInit1, &sCylinderInit1, &sCylinderInit1, &sCylinderInit2, &sCylinderInit2, &sCylinderInit2,
    };
    s32 i;
    s32 pad;

    for (i = 0; i < ARRAY_COUNT(cylinders); i++) {
        Collider_InitCylinder(globalCtx, &self->colCylinder[i]);
        Collider_SetCylinder(globalCtx, &self->colCylinder[i], &self->actor, cylinders[i]);
    }

    self->colCylinder[0].info.bumper.dmgFlags = 0x0003F8E9;
    self->colCylinder[1].info.bumper.dmgFlags = 0xFFC00716;
    self->colCylinder[2].base.colType = COLTYPE_METAL;
    self->colCylinder[2].info.bumperFlags = BUMP_ON | BUMP_HOOKABLE | BUMP_NO_AT_INFO;
    self->colCylinder[2].info.elemType = ELEMTYPE_UNK2;
    self->colCylinder[2].info.bumper.dmgFlags = 0xFFCC0716;

    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(2), &sColChkInfoInit);

    Collider_InitJntSph(globalCtx, &self->colSph);
    Collider_SetJntSph(globalCtx, &self->colSph, &self->actor, &sJntSphInit, self->colSphElements);
}

f32 EnSsh_SetAnimation(EnSsh* self, s32 animIndex) {
    AnimationHeader* animation[] = {
        0x06005BE8, 0x06000304, 0x06000304, 0x060055F8, 0x06000304, 0x06000304, 0x06005BE8
    };
    f32 playbackSpeed[] = { 1.0f, 4.0f, 1.0f, 1.0f, 8.0f, 6.0f, 2.0f };
    u8 mode[] = { 3, 3, 1, 3, 1, 1, 1 };
    f32 frameCount = Animation_GetLastFrame(animation[animIndex]);
    s32 pad;

    Animation_Change(&self->skelAnime, animation[animIndex], playbackSpeed[animIndex], 0.0f, frameCount,
                     mode[animIndex], -6.0f);

    return frameCount;
}

void EnSsh_SetWaitAnimation(EnSsh* self) {
    EnSsh_SetAnimation(self, SSH_ANIM_WAIT);
}

void EnSsh_SetReturnAnimation(EnSsh* self) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_UP);
    EnSsh_SetAnimation(self, SSH_ANIM_UP);
}

void EnSsh_SetLandAnimation(EnSsh* self) {
    self->actor.world.pos.y = self->floorHeightOffset + self->actor.floorHeight;
    self->animTimer = EnSsh_SetAnimation(self, SSH_ANIM_LAND);
}

void EnSsh_SetDropAnimation(EnSsh* self) {
    if (self->unkTimer == 0) {
        self->animTimer = EnSsh_SetAnimation(self, SSH_ANIM_DROP);
    }
    self->actor.velocity.y = -10.0f;
}

void EnSsh_SetStunned(EnSsh* self) {
    if (self->stunTimer == 0) {
        self->stateFlags |= SSH_STATE_ATTACKED;
        self->stunTimer = 120;
        self->actor.colorFilterTimer = 0;
    }
}

void EnSsh_SetColliderScale(EnSsh* self, f32 scale, f32 radiusMod) {
    f32 radius;
    f32 height;
    f32 yShift;
    s32 i;

    radius = self->colSph.elements[0].dim.modelSphere.radius;
    radius *= scale;
    self->colSph.elements[0].dim.modelSphere.radius = radius;

    for (i = 0; i < 6; i++) {
        yShift = self->colCylinder[i].dim.yShift;
        radius = self->colCylinder[i].dim.radius;
        height = self->colCylinder[i].dim.height;
        yShift *= scale;
        radius *= scale * radiusMod;
        height *= scale;

        self->colCylinder[i].dim.yShift = yShift;
        self->colCylinder[i].dim.radius = radius;
        self->colCylinder[i].dim.height = height;
    }
    Actor_SetScale(&self->actor, 0.04f * scale);
    self->floorHeightOffset = 40.0f * scale;
    self->colliderScale = scale * 1.5f;
}

s32 EnSsh_Damaged(EnSsh* self) {
    if ((self->stunTimer == 120) && (self->stateFlags & SSH_STATE_STUNNED)) {
        Actor_SetColorFilter(&self->actor, 0, 0xC8, 0, self->stunTimer);
    }
    if (DECR(self->stunTimer) != 0) {
        Math_SmoothStepToS(&self->maxTurnRate, 0x2710, 0xA, 0x3E8, 1);
        return false;
    } else {
        self->stunTimer = 0;
        self->stateFlags &= ~SSH_STATE_STUNNED;
        self->spinTimer = 0;
        if (self->swayTimer == 0) {
            self->spinTimer = 30;
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_ROLL);
        Audio_PlayActorSound2(&self->actor, NA_SE_VO_ST_ATTACK);
        return true;
    }
}

void EnSsh_Turn(EnSsh* self, GlobalContext* globalCtx) {
    if (self->hitTimer != 0) {
        self->hitTimer--;
    }
    if (DECR(self->spinTimer) != 0) {
        self->actor.world.rot.y += 10000.0f * (self->spinTimer / 30.0f);
    } else if ((self->swayTimer == 0) && (self->stunTimer == 0)) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 4, 0x2710, 1);
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnSsh_Stunned(EnSsh* self, GlobalContext* globalCtx) {
    if ((self->swayTimer == 0) && (self->stunTimer == 0)) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer ^ 0x8000, 4, self->maxTurnRate, 1);
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (self->stunTimer < 30) {
        if (self->stunTimer & 1) {
            self->actor.shape.rot.y += 0x7D0;
        } else {
            self->actor.shape.rot.y -= 0x7D0;
        }
    }
}

void EnSsh_UpdateYaw(EnSsh* self, GlobalContext* globalCtx) {
    if (self->stunTimer != 0) {
        EnSsh_Stunned(self, globalCtx);
    } else {
        EnSsh_Turn(self, globalCtx);
    }
}

void EnSsh_Bob(EnSsh* self, GlobalContext* globalCtx) {
    f32 bobVel = 0.5f;

    if ((globalCtx->state.frames & 8) != 0) {
        bobVel *= -1.0f;
    }
    Math_SmoothStepToF(&self->actor.velocity.y, bobVel, 0.4f, 1000.0f, 0.0f);
}

s32 EnSsh_IsCloseToLink(EnSsh* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 yDist;

    if (self->stateFlags & SSH_STATE_GROUND_START) {
        return true;
    }
    if (self->unkTimer != 0) {
        return true;
    }
    if (self->swayTimer != 0) {
        return true;
    }
    if (self->animTimer != 0) {
        return true;
    }

    if (self->actor.xzDistToPlayer > 160.0f) {
        return false;
    }

    yDist = self->actor.world.pos.y - player->actor.world.pos.y;
    if (yDist < 0.0f || yDist > 400.0f) {
        return false;
    }

    if (player->actor.world.pos.y < self->actor.floorHeight) {
        return false;
    }
    return true;
}

s32 EnSsh_IsCloseToHome(EnSsh* self) {
    f32 vel = self->actor.velocity.y;
    f32 nextY = self->actor.world.pos.y + 2.0f * self->actor.velocity.y;

    if (nextY >= self->actor.home.pos.y) {
        return 1;
    }
    return 0;
}

s32 EnSsh_IsCloseToGround(EnSsh* self) {
    f32 vel = self->actor.velocity.y;
    f32 nextY = self->actor.world.pos.y + 2.0f * self->actor.velocity.y;

    if ((nextY - self->actor.floorHeight) <= self->floorHeightOffset) {
        return 1;
    }
    return 0;
}

void EnSsh_Sway(EnSsh* self) {
    Vec3f swayVecBase;
    Vec3f swayVec;
    f32 temp;
    s16 swayAngle;

    if (self->swayTimer != 0) {
        self->swayAngle += 0x640;
        self->swayTimer--;
        if (self->swayTimer == 0) {
            self->swayAngle = 0;
        }
        temp = self->swayTimer * (1.0f / 6);
        swayAngle = temp * (0x10000 / 360.0f) * Math_SinS(self->swayAngle);
        temp = self->actor.world.pos.y - self->ceilingPos.y;
        swayVecBase.x = Math_SinS(swayAngle) * temp;
        swayVecBase.y = Math_CosS(swayAngle) * temp;
        swayVecBase.z = 0.0f;
        Matrix_Push();
        Matrix_Translate(self->ceilingPos.x, self->ceilingPos.y, self->ceilingPos.z, MTXMODE_NEW);
        Matrix_RotateY(self->actor.world.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_MultVec3f(&swayVecBase, &swayVec);
        Matrix_Pop();
        self->actor.shape.rot.z = -(swayAngle * 2);
        self->actor.world.pos.x = swayVec.x;
        self->actor.world.pos.z = swayVec.z;
    }
}

void EnSsh_CheckBodyStickHit(EnSsh* self, GlobalContext* globalCtx) {
    ColliderInfo* info0 = &self->colCylinder[0].info;
    Player* player = GET_PLAYER(globalCtx);

    if (player->unk_860 != 0) {
        info0->bumper.dmgFlags |= 2;
        self->colCylinder[1].info.bumper.dmgFlags &= ~2;
        self->colCylinder[2].info.bumper.dmgFlags &= ~2;
    } else {
        info0->bumper.dmgFlags &= ~2;
        self->colCylinder[1].info.bumper.dmgFlags |= 2;
        self->colCylinder[2].info.bumper.dmgFlags |= 2;
    }
}

s32 EnSsh_CheckHitLink(EnSsh* self, GlobalContext* globalCtx) {
    s32 i;
    s32 hit = false;

    if ((self->hitCount == 0) && (self->spinTimer == 0)) {
        return false;
    }
    for (i = 0; i < 3; i++) {
        if (self->colCylinder[i + 3].base.ocFlags2 & OC2_HIT_PLAYER) {
            self->colCylinder[i + 3].base.ocFlags2 &= ~OC2_HIT_PLAYER;
            hit = true;
        }
    }
    if (!hit) {
        return false;
    }
    self->hitTimer = 30;
    if (self->swayTimer == 0) {
        self->spinTimer = self->hitTimer;
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_ROLL);
    Audio_PlayActorSound2(&self->actor, NA_SE_VO_ST_ATTACK);
    globalCtx->damagePlayer(globalCtx, -8);
    func_8002F71C(globalCtx, &self->actor, 4.0f, self->actor.yawTowardsPlayer, 6.0f);
    self->hitCount--;
    return true;
}

s32 EnSsh_CheckHitFront(EnSsh* self) {
    u32 acFlags;

    if (self->colCylinder[2].base.acFlags) {} // Needed for matching
    acFlags = self->colCylinder[2].base.acFlags;

    if (!!(acFlags & AC_HIT) == 0) {
        return 0;
    } else {
        self->colCylinder[2].base.acFlags &= ~AC_HIT;
        self->invincibilityTimer = 8;
        if ((self->swayTimer == 0) && (self->hitTimer == 0) && (self->stunTimer == 0)) {
            self->swayTimer = 60;
        }
        return 1;
    }
}

s32 EnSsh_CheckHitBack(EnSsh* self, GlobalContext* globalCtx) {
    ColliderCylinder* cyl = &self->colCylinder[0];
    s32 hit = false;

    if (cyl->base.acFlags & AC_HIT) {
        cyl->base.acFlags &= ~AC_HIT;
        hit = true;
    }
    cyl = &self->colCylinder[1];
    if (cyl->base.acFlags & AC_HIT) {
        cyl->base.acFlags &= ~AC_HIT;
        hit = true;
    }
    if (!hit) {
        return false;
    }
    self->invincibilityTimer = 8;
    if (self->hitCount <= 0) {
        self->hitCount++;
    }
    if (self->stunTimer == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
        Audio_PlayActorSound2(&self->actor, NA_SE_VO_ST_DAMAGE);
    }
    EnSsh_SetStunned(self);
    self->stateFlags |= SSH_STATE_STUNNED;
    return false;
}

s32 EnSsh_CollisionCheck(EnSsh* self, GlobalContext* globalCtx) {
    if (self->stunTimer == 0) {
        EnSsh_CheckHitLink(self, globalCtx);
    }
    if (EnSsh_CheckHitFront(self)) {
        return false;
    } else if (globalCtx->actorCtx.unk_02 != 0) {
        self->invincibilityTimer = 8;
        if (self->stunTimer == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
            Audio_PlayActorSound2(&self->actor, NA_SE_VO_ST_DAMAGE);
        }
        EnSsh_SetStunned(self);
        self->stateFlags |= SSH_STATE_STUNNED;
        return false;
    } else {
        return EnSsh_CheckHitBack(self, globalCtx);
        // Always returns false
    }
}

void EnSsh_SetBodyCylinderAC(EnSsh* self, GlobalContext* globalCtx) {
    Collider_UpdateCylinder(&self->actor, &self->colCylinder[0]);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder[0].base);
}

void EnSsh_SetLegsCylinderAC(EnSsh* self, GlobalContext* globalCtx) {
    s16 angleTowardsLink = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y));

    if (angleTowardsLink < 90 * (0x10000 / 360)) {
        Collider_UpdateCylinder(&self->actor, &self->colCylinder[2]);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder[2].base);
    } else {
        Collider_UpdateCylinder(&self->actor, &self->colCylinder[1]);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder[1].base);
    }
}

s32 EnSsh_SetCylinderOC(EnSsh* self, GlobalContext* globalCtx) {
    Vec3f cyloffsets[] = {
        { 40.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { -40.0f, 0.0f, 0.0f },
    };
    Vec3f cylPos;
    s32 i;

    for (i = 0; i < 3; i++) {
        cylPos = self->actor.world.pos;
        cyloffsets[i].x *= self->colliderScale;
        cyloffsets[i].y *= self->colliderScale;
        cyloffsets[i].z *= self->colliderScale;
        Matrix_Push();
        Matrix_Translate(cylPos.x, cylPos.y, cylPos.z, MTXMODE_NEW);
        Matrix_RotateY((self->initialYaw / (f32)0x8000) * M_PI, MTXMODE_APPLY);
        Matrix_MultVec3f(&cyloffsets[i], &cylPos);
        Matrix_Pop();
        self->colCylinder[i + 3].dim.pos.x = cylPos.x;
        self->colCylinder[i + 3].dim.pos.y = cylPos.y;
        self->colCylinder[i + 3].dim.pos.z = cylPos.z;
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder[i + 3].base);
    }
    return 1;
}

void EnSsh_SetColliders(EnSsh* self, GlobalContext* globalCtx) {
    if (self->actor.colChkInfo.health == 0) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colSph.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colSph.base);
    } else {
        if (self->hitTimer == 0) {
            EnSsh_SetCylinderOC(self, globalCtx);
        }
        if (DECR(self->invincibilityTimer) == 0) {
            EnSsh_SetBodyCylinderAC(self, globalCtx);
            EnSsh_SetLegsCylinderAC(self, globalCtx);
        }
    }
}

void EnSsh_Init(Actor* thisx, GlobalContext* globalCtx) {
    f32 frameCount;
    s32 pad;
    EnSsh* self = THIS;

    frameCount = Animation_GetLastFrame(&object_ssh_Anim_000304);
    if (self->actor.params == ENSSH_FATHER) {
        if (gSaveContext.inventory.gsTokens >= 100) {
            Actor_Kill(&self->actor);
            return;
        }
    } else if (gSaveContext.inventory.gsTokens >= (self->actor.params * 10)) {
        Actor_Kill(&self->actor);
        return;
    }
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &object_ssh_Skel_0052E0, NULL, self->jointTable, self->morphTable, 30);
    Animation_Change(&self->skelAnime, &object_ssh_Anim_000304, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP_INTERP, 0.0f);
    self->blureIdx = EnSsh_CreateBlureEffect(globalCtx);
    EnSsh_InitColliders(self, globalCtx);
    self->stateFlags = 0;
    self->hitCount = 0;
    EnSsh_CheckCeilingPos(self, globalCtx);
    if (self->actor.params != ENSSH_FATHER) {
        EnSsh_SetColliderScale(self, 0.5f, 1.0f);
    } else {
        EnSsh_SetColliderScale(self, 0.75f, 1.0f);
    }
    self->actor.gravity = 0.0f;
    self->initialYaw = self->actor.world.rot.y;
    EnSsh_SetupAction(self, EnSsh_Start);
}

void EnSsh_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnSsh* self = THIS;
    s32 i;

    Effect_Delete(globalCtx, self->blureIdx);
    for (i = 0; i < 6; i++) {
        Collider_DestroyCylinder(globalCtx, &self->colCylinder[i]);
    }
    Collider_DestroyJntSph(globalCtx, &self->colSph);
}

void EnSsh_Wait(EnSsh* self, GlobalContext* globalCtx) {
    if (EnSsh_IsCloseToLink(self, globalCtx)) {
        EnSsh_SetDropAnimation(self);
        EnSsh_SetupAction(self, EnSsh_Drop);
    } else {
        EnSsh_Bob(self, globalCtx);
    }
}

void EnSsh_Talk(EnSsh* self, GlobalContext* globalCtx) {
    EnSsh_Bob(self, globalCtx);
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actionFunc = EnSsh_Idle;
    }
}

void EnSsh_Idle(EnSsh* self, GlobalContext* globalCtx) {
    if (1) {}
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnSsh_Talk;
        if (self->actor.params == ENSSH_FATHER) {
            gSaveContext.eventChkInf[9] |= 0x40;
        }
        if ((self->actor.textId == 0x26) || (self->actor.textId == 0x27)) {
            gSaveContext.infTable[25] |= 0x40;
        }
        if ((self->actor.textId == 0x24) || (self->actor.textId == 0x25)) {
            gSaveContext.infTable[25] |= 0x80;
        }
    } else {
        if ((self->unkTimer != 0) && (DECR(self->unkTimer) == 0)) {
            EnSsh_SetAnimation(self, SSH_ANIM_WAIT);
        }
        if ((self->animTimer != 0) && (DECR(self->animTimer) == 0)) {
            EnSsh_SetAnimation(self, SSH_ANIM_WAIT);
        }
        if (!EnSsh_IsCloseToLink(self, globalCtx)) {
            EnSsh_SetReturnAnimation(self);
            EnSsh_SetupAction(self, EnSsh_Return);
        } else {
            if (DECR(self->sfxTimer) == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_LAUGH);
                self->sfxTimer = 64;
            }
            EnSsh_Bob(self, globalCtx);
            if ((self->unkTimer == 0) && (self->animTimer == 0)) {
                self->actor.textId = Text_GetFaceReaction(globalCtx, 0xD);
                if (self->actor.textId == 0) {
                    if (self->actor.params == ENSSH_FATHER) {
                        if (gSaveContext.inventory.gsTokens >= 50) {
                            self->actor.textId = 0x29;
                        } else if (gSaveContext.inventory.gsTokens >= 10) {
                            if (gSaveContext.infTable[25] & 0x80) {
                                self->actor.textId = 0x24;
                            } else {
                                self->actor.textId = 0x25;
                            }
                        } else {
                            if (gSaveContext.infTable[25] & 0x40) {
                                self->actor.textId = 0x27;
                            } else {
                                self->actor.textId = 0x26;
                            }
                        }
                    } else {
                        self->actor.textId = 0x22;
                    }
                }
                func_8002F2CC(&self->actor, globalCtx, 100.0f);
            }
        }
    }
}

void EnSsh_Land(EnSsh* self, GlobalContext* globalCtx) {
    if ((self->unkTimer != 0) && (DECR(self->unkTimer) == 0)) {
        EnSsh_SetAnimation(self, SSH_ANIM_WAIT);
    }
    if ((self->animTimer != 0) && (DECR(self->animTimer) == 0)) {
        EnSsh_SetAnimation(self, SSH_ANIM_WAIT);
    }
    if ((self->actor.floorHeight + self->floorHeightOffset) <= self->actor.world.pos.y) {
        EnSsh_SetupAction(self, EnSsh_Idle);
    } else {
        Math_SmoothStepToF(&self->actor.velocity.y, 2.0f, 0.6f, 1000.0f, 0.0f);
    }
}

void EnSsh_Drop(EnSsh* self, GlobalContext* globalCtx) {
    if ((self->unkTimer != 0) && (DECR(self->unkTimer) == 0)) {
        EnSsh_SetAnimation(self, SSH_ANIM_DROP);
    }
    if (!EnSsh_IsCloseToLink(self, globalCtx)) {
        EnSsh_SetReturnAnimation(self);
        EnSsh_SetupAction(self, EnSsh_Return);
    } else if (EnSsh_IsCloseToGround(self)) {
        EnSsh_SpawnShockwave(self, globalCtx);
        EnSsh_SetLandAnimation(self);
        EnSsh_SetupAction(self, EnSsh_Land);
    } else if (DECR(self->sfxTimer) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_DOWN);
        self->sfxTimer = 3;
    }
}

void EnSsh_Return(EnSsh* self, GlobalContext* globalCtx) {
    f32 frameRatio = self->skelAnime.curFrame / (self->skelAnime.animLength - 1.0f);

    if (frameRatio == 1.0f) {
        EnSsh_SetReturnAnimation(self);
    }
    if (EnSsh_IsCloseToLink(self, globalCtx)) {
        EnSsh_SetDropAnimation(self);
        EnSsh_SetupAction(self, EnSsh_Drop);
    } else if (EnSsh_IsCloseToHome(self)) {
        EnSsh_SetWaitAnimation(self);
        EnSsh_SetupAction(self, EnSsh_Wait);
    } else {
        self->actor.velocity.y = 4.0f * frameRatio;
    }
}

void EnSsh_UpdateColliderScale(EnSsh* self) {
    if (self->stateFlags & SSH_STATE_SPIN) {
        if (self->spinTimer == 0) {
            self->stateFlags &= ~SSH_STATE_SPIN;
            if (self->actor.params != ENSSH_FATHER) {
                EnSsh_SetColliderScale(self, 0.5f, 1.0f);
            } else {
                EnSsh_SetColliderScale(self, 0.75f, 1.0f);
            }
        }
    } else {
        if (self->spinTimer != 0) {
            self->stateFlags |= SSH_STATE_SPIN;
            if (self->actor.params != ENSSH_FATHER) {
                EnSsh_SetColliderScale(self, 0.5f, 2.0f);
            } else {
                EnSsh_SetColliderScale(self, 0.75f, 2.0f);
            }
        }
    }
}

void EnSsh_Start(EnSsh* self, GlobalContext* globalCtx) {
    if (!EnSsh_IsCloseToGround(self)) {
        EnSsh_SetupAction(self, EnSsh_Wait);
        EnSsh_Wait(self, globalCtx);
    } else {
        EnSsh_SetLandAnimation(self);
        self->stateFlags |= 4;
        EnSsh_SetupAction(self, EnSsh_Land);
        EnSsh_Land(self, globalCtx);
    }
}

void EnSsh_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnSsh* self = THIS;

    EnSsh_UpdateColliderScale(self);
    if (EnSsh_CollisionCheck(self, globalCtx)) {
        return; // EnSsh_CollisionCheck always returns false, so self never happens
    }
    if (self->stunTimer != 0) {
        EnSsh_Damaged(self);
    } else {
        SkelAnime_Update(&self->skelAnime);
        func_8002D7EC(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        self->actionFunc(self, globalCtx);
    }
    EnSsh_UpdateYaw(self, globalCtx);
    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset(60, 60);
    }
    self->blinkState = self->blinkTimer;
    if (self->blinkState >= 3) {
        self->blinkState = 0;
    }
    EnSsh_SetColliders(self, globalCtx);
    Actor_SetFocus(&self->actor, 0.0f);
}

s32 EnSsh_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnSsh* self = THIS;

    switch (limbIndex) {
        case 1:
            if ((self->spinTimer != 0) && (self->swayTimer == 0)) {
                if (self->spinTimer >= 2) {
                    EnSsh_AddBlureVertex(self);
                } else {
                    EnSsh_AddBlureSpace(self);
                }
            }
            break;
        case 4:
            if (self->actor.params == ENSSH_FATHER) {
                *dList = object_ssh_DL_0046C0;
            }
            break;
        case 5:
            if (self->actor.params == ENSSH_FATHER) {
                *dList = object_ssh_DL_004080;
            }
            break;
        case 8:
            if (self->actor.params == ENSSH_FATHER) {
                *dList = object_ssh_DL_004DE8;
            }
            break;
    }
    return false;
}

void EnSsh_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnSsh* self = THIS;

    Collider_UpdateSpheres(limbIndex, &self->colSph);
}

void EnSsh_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static u32* blinkTex[] = { 0x060007E0, 0x06000C60, 0x06001060 };
    s32 pad;
    EnSsh* self = THIS;

    EnSsh_CheckBodyStickHit(self, globalCtx);
    EnSsh_Sway(self);
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ssh.c", 2333);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(blinkTex[self->blinkState]));
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ssh.c", 2336);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnSsh_OverrideLimbDraw,
                      EnSsh_PostLimbDraw, &self->actor);
}
