/*
 * File: z_en_st.c
 * Overlay: ovl_En_St
 * Description: Skulltula (normal, big, invisible)
 */

#include "z_en_st.h"
#include "objects/object_st/object_st.h"

#define FLAGS 0x00000035

#define THIS ((EnSt*)thisx)

void EnSt_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSt_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSt_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSt_Draw(Actor* thisx, GlobalContext* globalCtx);
void EnSt_ReturnToCeiling(EnSt* self, GlobalContext* globalCtx);
void EnSt_MoveToGround(EnSt* self, GlobalContext* globalCtx);
void EnSt_StartOnCeilingOrGround(EnSt* self, GlobalContext* globalCtx);
void EnSt_WaitOnGround(EnSt* self, GlobalContext* globalCtx);
void EnSt_Die(EnSt* self, GlobalContext* globalCtx);
void EnSt_BounceAround(EnSt* self, GlobalContext* globalCtx);
void EnSt_FinishBouncing(EnSt* self, GlobalContext* globalCtx);

static Vtx sUnusedVertices[] = {
    VTX(-1, 0, 0, 0, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(1, 0, 0, 1024, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(1, 100, 0, 1024, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(-1, 100, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF),
};

static Gfx sUnusedDList[] = {
    gsDPPipeSync(),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(sUnusedVertices, 4, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSPEndDisplayList(),
};

const ActorInit En_St_InitVars = {
    ACTOR_EN_ST,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ST,
    sizeof(EnSt),
    (ActorFunc)EnSt_Init,
    (ActorFunc)EnSt_Destroy,
    (ActorFunc)EnSt_Update,
    (ActorFunc)EnSt_Draw,
};

static ColliderCylinderInit sCylinderInit = {
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

static CollisionCheckInfoInit2 sColChkInit = { 2, 0, 0, 0, MASS_IMMOVABLE };

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

static struct_80034EC0_Entry sAnimations[] = {
    { &object_st_Anim_000304, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, 0.0f },
    { &object_st_Anim_005B98, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -8.0f },
    { &object_st_Anim_000304, 4.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -8.0f },
    { &object_st_Anim_000304, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
    { &object_st_Anim_0055A8, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE_INTERP, -8.0f },
    { &object_st_Anim_000304, 8.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
    { &object_st_Anim_000304, 6.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
    { &object_st_Anim_005B98, 2.0f, 0.0f, -1.0f, ANIMMODE_LOOP_INTERP, -8.0f },
};

void EnSt_SetupAction(EnSt* self, EnStActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

/**
 * Spawns `dustCnt` dust particles in a random pattern around the Skulltula
 */
void EnSt_SpawnDust(EnSt* self, GlobalContext* globalCtx, s32 dustCnt) {
    Color_RGBA8 primColor = { 170, 130, 90, 255 };
    Color_RGBA8 envColor = { 100, 60, 20, 0 };
    Vec3f dustVel = { 0.0f, 0.0f, 0.0f };
    Vec3f dustAccel = { 0.0f, 0.3f, 0.0f };
    Vec3f dustPos;
    s16 yAngle;
    s32 i;

    yAngle = (Rand_ZeroOne() - 0.5f) * 65536.0f;
    dustPos.y = self->actor.floorHeight;
    for (i = dustCnt; i >= 0; i--, yAngle += (s16)(0x10000 / dustCnt)) {
        dustAccel.x = (Rand_ZeroOne() - 0.5f) * 4.0f;
        dustAccel.z = (Rand_ZeroOne() - 0.5f) * 4.0f;
        dustPos.x = self->actor.world.pos.x + (Math_SinS(yAngle) * 22.0f);
        dustPos.z = self->actor.world.pos.z + (Math_CosS(yAngle) * 22.0f);
        func_8002836C(globalCtx, &dustPos, &dustVel, &dustAccel, &primColor, &envColor, 120, 40, 10);
    }
}

void EnSt_SpawnBlastEffect(EnSt* self, GlobalContext* globalCtx) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    Vec3f blastPos;

    blastPos.x = self->actor.world.pos.x;
    blastPos.y = self->actor.floorHeight;
    blastPos.z = self->actor.world.pos.z;

    EffectSsBlast_SpawnWhiteCustomScale(globalCtx, &blastPos, &zeroVec, &zeroVec, 100, 220, 8);
}

void EnSt_SpawnDeadEffect(EnSt* self, GlobalContext* globalCtx) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    Vec3f firePos;

    firePos.x = self->actor.world.pos.x + ((Rand_ZeroOne() - 0.5f) * 60.0f);
    firePos.y = (self->actor.world.pos.y + 10.0f) + ((Rand_ZeroOne() - 0.5f) * 45.0f);
    firePos.z = self->actor.world.pos.z + ((Rand_ZeroOne() - 0.5f) * 60.0f);
    EffectSsDeadDb_Spawn(globalCtx, &firePos, &zeroVec, &zeroVec, 100, 0, 255, 255, 255, 255, 255, 0, 0, 1, 9, true);
}

s32 EnSt_CreateBlureEffect(GlobalContext* globalCtx) {
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

/**
 * Checks for the position of the ceiling above the Skulltula.
 * If no ceiling is found it is set to 1000 units above the Skulltula
 */
s32 EnSt_CheckCeilingPos(EnSt* self, GlobalContext* globalCtx) {
    CollisionPoly* poly;
    s32 bgId;
    Vec3f checkPos;

    checkPos.x = self->actor.world.pos.x;
    checkPos.y = self->actor.world.pos.y + 1000.0f;
    checkPos.z = self->actor.world.pos.z;
    if (!BgCheck_EntityLineTest1(&globalCtx->colCtx, &self->actor.world.pos, &checkPos, &self->ceilingPos, &poly, false,
                                 false, true, true, &bgId)) {
        return false;
    }
    self->unusedPos = self->actor.world.pos;
    self->unusedPos.y -= 100.0f;
    return true;
}

void EnSt_AddBlurVertex(EnSt* self) {
    Vec3f v1 = { 834.0f, 834.0f, 0.0f };
    Vec3f v2 = { 834.0f, -584.0f, 0.0f };
    Vec3f v1Pos;
    Vec3f v2Pos;

    v1.x *= self->colliderScale;
    v1.y *= self->colliderScale;
    v1.z *= self->colliderScale;

    v2.x *= self->colliderScale;
    v2.y *= self->colliderScale;
    v2.z *= self->colliderScale;

    Matrix_Push();
    Matrix_MultVec3f(&v1, &v1Pos);
    Matrix_MultVec3f(&v2, &v2Pos);
    Matrix_Pop();
    EffectBlure_AddVertex(Effect_GetByIndex(self->blureIdx), &v1Pos, &v2Pos);
}

void EnSt_AddBlurSpace(EnSt* self) {
    EffectBlure_AddSpace(Effect_GetByIndex(self->blureIdx));
}

void EnSt_SetWaitingAnimation(EnSt* self) {
    func_80034EC0(&self->skelAnime, sAnimations, 3);
}

void EnSt_SetReturnToCeilingAnimation(EnSt* self) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_UP);
    func_80034EC0(&self->skelAnime, sAnimations, 2);
}

void EnSt_SetLandAnimation(EnSt* self) {
    self->actor.world.pos.y = self->actor.floorHeight + self->floorHeightOffset;
    func_80034EC0(&self->skelAnime, sAnimations, 4);
    self->sfxTimer = 0;
    self->animFrames = self->skelAnime.animLength;
}

void EnSt_SetDropAnimAndVel(EnSt* self) {
    if (self->takeDamageSpinTimer == 0) {
        func_80034EC0(&self->skelAnime, sAnimations, 4);
        self->animFrames = self->skelAnime.animLength;
    }
    self->sfxTimer = 0;
    self->actor.velocity.y = -10.0f;
}

/**
 * Initalizes the Skulltula's 6 cylinders, and sphere collider.
 */
void EnSt_InitColliders(EnSt* self, GlobalContext* globalCtx) {
    ColliderCylinderInit* cylinders[6] = {
        &sCylinderInit, &sCylinderInit, &sCylinderInit, &sCylinderInit2, &sCylinderInit2, &sCylinderInit2,
    };

    s32 i;
    s32 pad;

    for (i = 0; i < ARRAY_COUNT(cylinders); i++) {
        Collider_InitCylinder(globalCtx, &self->colCylinder[i]);
        Collider_SetCylinder(globalCtx, &self->colCylinder[i], &self->actor, cylinders[i]);
    }

    self->colCylinder[0].info.bumper.dmgFlags = 0x0003F8F9;
    self->colCylinder[1].info.bumper.dmgFlags = 0xFFC00706;
    self->colCylinder[2].base.colType = COLTYPE_METAL;
    self->colCylinder[2].info.bumperFlags = BUMP_ON | BUMP_HOOKABLE | BUMP_NO_AT_INFO;
    self->colCylinder[2].info.elemType = ELEMTYPE_UNK2;
    self->colCylinder[2].info.bumper.dmgFlags = 0xFFCC0706;

    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(2), &sColChkInit);

    Collider_InitJntSph(globalCtx, &self->colSph);
    Collider_SetJntSph(globalCtx, &self->colSph, &self->actor, &sJntSphInit, self->colSphItems);
}

void EnSt_CheckBodyStickHit(EnSt* self, GlobalContext* globalCtx) {
    ColliderInfo* body = &self->colCylinder[0].info;
    Player* player = GET_PLAYER(globalCtx);

    if (player->unk_860 != 0) {
        body->bumper.dmgFlags |= 2;
        self->colCylinder[1].info.bumper.dmgFlags &= ~2;
        self->colCylinder[2].info.bumper.dmgFlags &= ~2;
    } else {
        body->bumper.dmgFlags &= ~2;
        self->colCylinder[1].info.bumper.dmgFlags |= 2;
        self->colCylinder[2].info.bumper.dmgFlags |= 2;
    }
}

void EnSt_SetBodyCylinderAC(EnSt* self, GlobalContext* globalCtx) {
    Collider_UpdateCylinder(&self->actor, &self->colCylinder[0]);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder[0].base);
}

void EnSt_SetLegsCylinderAC(EnSt* self, GlobalContext* globalCtx) {
    s16 angleTowardsLink = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y));

    if (angleTowardsLink < 0x3FFC) {
        Collider_UpdateCylinder(&self->actor, &self->colCylinder[2]);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder[2].base);
    } else {
        Collider_UpdateCylinder(&self->actor, &self->colCylinder[1]);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder[1].base);
    }
}

s32 EnSt_SetCylinderOC(EnSt* self, GlobalContext* globalCtx) {
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
        Matrix_RotateY((self->initalYaw / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_MultVec3f(&cyloffsets[i], &cylPos);
        Matrix_Pop();
        self->colCylinder[i + 3].dim.pos.x = cylPos.x;
        self->colCylinder[i + 3].dim.pos.y = cylPos.y;
        self->colCylinder[i + 3].dim.pos.z = cylPos.z;
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder[i + 3].base);
    }

    return true;
}

void EnSt_UpdateCylinders(EnSt* self, GlobalContext* globalCtx) {
    if ((self->actor.colChkInfo.health != 0) || (self->actionFunc == EnSt_FinishBouncing)) {
        if (DECR(self->gaveDamageSpinTimer) == 0) {
            EnSt_SetCylinderOC(self, globalCtx);
        }

        DECR(self->invulnerableTimer);
        DECR(self->takeDamageSpinTimer);

        if (self->invulnerableTimer == 0 && self->takeDamageSpinTimer == 0) {
            EnSt_SetBodyCylinderAC(self, globalCtx);
            EnSt_SetLegsCylinderAC(self, globalCtx);
        }
    }
}

s32 EnSt_CheckHitLink(EnSt* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 hit;
    s32 i;

    for (i = 0, hit = 0; i < 3; i++) {
        if (((self->colCylinder[i + 3].base.ocFlags2 & OC2_HIT_PLAYER) != 0) == 0) {
            continue;
        }
        self->colCylinder[i + 3].base.ocFlags2 &= ~OC2_HIT_PLAYER;
        hit = true;
    }

    if (!hit) {
        return false;
    }

    if (self->swayTimer == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_ROLL);
    }

    self->gaveDamageSpinTimer = 30;
    globalCtx->damagePlayer(globalCtx, -8);
    Audio_PlayActorSound2(&player->actor, NA_SE_PL_BODY_HIT);
    func_8002F71C(globalCtx, &self->actor, 4.0f, self->actor.yawTowardsPlayer, 6.0f);
    return true;
}

s32 EnSt_CheckHitFrontside(EnSt* self) {
    u8 acFlags = self->colCylinder[2].base.acFlags;

    if (!!(acFlags & AC_HIT) == 0) {
        // not hit
        return false;
    } else {
        self->colCylinder[2].base.acFlags &= ~AC_HIT;
        self->invulnerableTimer = 8;
        self->playSwayFlag = 0;
        self->swayTimer = 60;
        return true;
    }
}

s32 EnSt_CheckHitBackside(EnSt* self, GlobalContext* globalCtx) {
    ColliderCylinder* cyl = &self->colCylinder[0];
    s32 flags = 0; // ac hit flags from colliders 0 and 1
    s32 hit = false;

    if (cyl->base.acFlags & AC_HIT) {
        cyl->base.acFlags &= ~AC_HIT;
        hit = true;
        flags |= cyl->info.acHitInfo->toucher.dmgFlags;
    }

    cyl = &self->colCylinder[1];
    if (cyl->base.acFlags & AC_HIT) {
        cyl->base.acFlags &= ~AC_HIT;
        hit = true;
        flags |= cyl->info.acHitInfo->toucher.dmgFlags;
    }

    if (!hit) {
        return false;
    }

    self->invulnerableTimer = 8;
    if (self->actor.colChkInfo.damageEffect == 1) {
        if (self->stunTimer == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
            self->stunTimer = 120;
            Actor_SetColorFilter(&self->actor, 0, 0xC8, 0, self->stunTimer);
        }
        return false;
    }

    self->swayTimer = self->stunTimer = 0;
    self->gaveDamageSpinTimer = 1;
    func_80034EC0(&self->skelAnime, sAnimations, 3);
    self->takeDamageSpinTimer = self->skelAnime.animLength;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xC8, 0, self->takeDamageSpinTimer);
    if (Actor_ApplyDamage(&self->actor)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_DAMAGE);
        return false;
    }
    Enemy_StartFinishingBlow(globalCtx, &self->actor);
    self->actor.flags &= ~1;
    self->groundBounces = 3;
    self->deathTimer = 20;
    self->actor.gravity = -1.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALWALL_DEAD);

    if (flags & 0x1F820) {
        // arrow, fire arrow, ice arrow, light arrow,
        // and three unknows, unused arrows?
        EnSt_SetupAction(self, EnSt_Die);
        self->finishDeathTimer = 8;
    } else {
        EnSt_SetupAction(self, EnSt_BounceAround);
    }

    return true;
}

/**
 * Checks if the Skulltula's colliders have been hit, returns true if the hit has dealt damage to the Skulltula
 */
s32 EnSt_CheckColliders(EnSt* self, GlobalContext* globalCtx) {
    if (EnSt_CheckHitFrontside(self)) {
        // player has hit the front shield area of the Skulltula
        return false;
    }

    if (globalCtx->actorCtx.unk_02 != 0) {
        return true;
    }

    if (EnSt_CheckHitBackside(self, globalCtx)) {
        // player has hit the backside of the Skulltula
        return true;
    }

    if (self->stunTimer == 0 && self->takeDamageSpinTimer == 0) {
        // check if the Skulltula has hit link.
        EnSt_CheckHitLink(self, globalCtx);
    }
    return false;
}

void EnSt_SetColliderScale(EnSt* self) {
    f32 scaleAmount = 1.0f;
    f32 radius;
    f32 height;
    f32 yShift;
    s32 i;

    if (self->actor.params == 1) {
        scaleAmount = 1.4f;
    }

    radius = self->colSph.elements[0].dim.modelSphere.radius;
    radius *= scaleAmount;
    self->colSph.elements[0].dim.modelSphere.radius = radius;

    for (i = 0; i < 6; i++) {
        yShift = self->colCylinder[i].dim.yShift;
        radius = self->colCylinder[i].dim.radius;
        height = self->colCylinder[i].dim.height;
        yShift *= scaleAmount;
        radius *= scaleAmount;
        height *= scaleAmount;

        self->colCylinder[i].dim.yShift = yShift;
        self->colCylinder[i].dim.radius = radius;
        self->colCylinder[i].dim.height = height;
    }
    Actor_SetScale(&self->actor, 0.04f * scaleAmount);
    self->colliderScale = scaleAmount;
    self->floorHeightOffset = 32.0f * scaleAmount;
}

s32 EnSt_SetTeethColor(EnSt* self, s16 redTarget, s16 greenTarget, s16 blueTarget, s16 minMaxStep) {
    s16 red = self->teethR;
    s16 green = self->teethG;
    s16 blue = self->teethB;

    minMaxStep = 255 / (s16)(0.6f * minMaxStep);
    if (minMaxStep <= 0) {
        minMaxStep = 1;
    }

    Math_SmoothStepToS(&red, redTarget, 1, minMaxStep, minMaxStep);
    Math_SmoothStepToS(&green, greenTarget, 1, minMaxStep, minMaxStep);
    Math_SmoothStepToS(&blue, blueTarget, 1, minMaxStep, minMaxStep);
    self->teethR = red;
    self->teethG = green;
    self->teethB = blue;
    return 1;
}

s32 EnSt_DecrStunTimer(EnSt* self) {
    if (self->stunTimer == 0) {
        return 0;
    }
    self->stunTimer--; //! @bug no return but v0 ends up being stunTimer before decrement
#ifdef AVOID_UB
    return self->stunTimer + 1; // to match IDO behavior
#endif
}

/**
 * Updates the yaw of the Skulltula, used for the shaking animation right before
 * turning, and the actual turning to face away from the player, and then back to
 * face the player
 */
void EnSt_UpdateYaw(EnSt* self, GlobalContext* globalCtx) {
    u16 yawDir = 0;
    Vec3s rot;
    s16 yawDiff;
    s16 timer;
    s16 yawTarget;

    // Shake towards the end of the stun.
    if (self->stunTimer != 0) {
        if (self->stunTimer < 30) {
            if ((self->stunTimer % 2) != 0) {
                self->actor.shape.rot.y += 0x800;
            } else {
                self->actor.shape.rot.y -= 0x800;
            }
        }
        return;
    }

    if (self->swayTimer == 0 && self->deathTimer == 0 && self->finishDeathTimer == 0) {
        // not swaying or dying
        if (self->takeDamageSpinTimer != 0 || self->gaveDamageSpinTimer != 0) {
            // Skulltula is doing a spinning animation
            self->actor.shape.rot.y += 0x2000;
            return;
        }

        if (self->actionFunc != EnSt_WaitOnGround) {
            // set the timers to turn away or turn towards the player
            self->rotAwayTimer = 30;
            self->rotTowardsTimer = 0;
        }

        if (self->rotAwayTimer != 0) {
            // turn away from the player
            self->rotAwayTimer--;
            if (self->rotAwayTimer == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_ROLL);
                self->rotTowardsTimer = 30;
            }
        } else if (self->rotTowardsTimer != 0) {
            // turn towards the player
            self->rotTowardsTimer--;
            if (self->rotTowardsTimer == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_ROLL);
                self->rotAwayTimer = 30;
            }
            yawDir = 0x8000;
        }

        // calculate the new yaw to or away from the player.
        rot = self->actor.shape.rot;
        yawTarget = (self->actionFunc == EnSt_WaitOnGround ? self->actor.yawTowardsPlayer : self->initalYaw);
        yawDiff = rot.y - (yawTarget ^ yawDir);
        if (ABS(yawDiff) <= 0x4000) {
            Math_SmoothStepToS(&rot.y, yawTarget ^ yawDir, 4, 0x2000, 1);
        } else {
            rot.y += 0x2000;
        }

        self->actor.shape.rot = self->actor.world.rot = rot;

        // Do the shaking animation.
        if (yawDir == 0 && self->rotAwayTimer < 0xA) {
            timer = self->rotAwayTimer;
        } else if (yawDir == 0x8000 && self->rotTowardsTimer < 0xA) {
            timer = self->rotTowardsTimer;
        } else {
            return;
        }

        if ((timer % 2) != 0) {
            self->actor.shape.rot.y += 0x800;
        } else {
            self->actor.shape.rot.y -= 0x800;
        }
    }
}

/**
 * Checks to see if the Skulltula is done bouncing on the ground,
 * spawns dust particles as the Skulltula hits the ground
 */
s32 EnSt_IsDoneBouncing(EnSt* self, GlobalContext* globalCtx) {
    if (self->actor.velocity.y > 0.0f || self->groundBounces == 0) {
        // the Skulltula is moving upwards or the groundBounces is 0
        return false;
    }

    if (!(self->actor.bgCheckFlags & 1)) {
        // the Skulltula is not on the ground.
        return false;
    }

    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
    EnSt_SpawnDust(self, globalCtx, 10);
    // creates an elastic bouncing effect, boucing up less for each hit on the ground.
    self->actor.velocity.y = 6.0f / (4 - self->groundBounces);
    self->groundBounces--;
    if (self->groundBounces != 0) {
        return false;
    } else {
        // make sure the Skulltula stays on the ground.
        self->actor.velocity.y = 0.0f;
    }
    return true;
}

void EnSt_Bob(EnSt* self, GlobalContext* globalCtx) {
    f32 ySpeedTarget = 0.5f;

    if ((globalCtx->state.frames & 8) != 0) {
        ySpeedTarget *= -1.0f;
    }
    Math_SmoothStepToF(&self->actor.velocity.y, ySpeedTarget, 0.4f, 1000.0f, 0.0f);
}

s32 EnSt_IsCloseToPlayer(EnSt* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 yDist;

    if (self->takeDamageSpinTimer != 0) {
        // skull is spinning from damage.
        return false;
    } else if (self->actor.xzDistToPlayer > 160.0f) {
        // player is more than 160 xz units from the Skulltula
        return false;
    }

    yDist = self->actor.world.pos.y - player->actor.world.pos.y;
    if (yDist < 0.0f || yDist > 400.0f) {
        // player is above the Skulltula or more than 400 units below
        // the Skulltula
        return false;
    }

    if (player->actor.world.pos.y < self->actor.floorHeight) {
        // player is below the Skulltula's ground position
        return false;
    }
    return true;
}

s32 EnSt_IsCloseToInitalPos(EnSt* self) {
    f32 velY = self->actor.velocity.y;
    f32 checkY = self->actor.world.pos.y + (velY * 2.0f);

    if (checkY >= self->actor.home.pos.y) {
        return true;
    }
    return false;
}

s32 EnSt_IsCloseToGround(EnSt* self) {
    f32 velY = self->actor.velocity.y;
    f32 checkY = self->actor.world.pos.y + (velY * 2.0f);

    if (checkY - self->actor.floorHeight <= self->floorHeightOffset) {
        return true;
    }
    return false;
}

/**
 * Does the animation of the Skulltula swaying back and forth after the Skulltula
 * has been hit in the front by a sword
 */
void EnSt_Sway(EnSt* self) {
    Vec3f amtToTranslate;
    Vec3f translatedPos;
    f32 swayAmt;
    s16 rotAngle;

    if (self->swayTimer != 0) {

        self->swayAngle += 0xA28;
        self->swayTimer--;

        if (self->swayTimer == 0) {
            self->swayAngle = 0;
        }

        swayAmt = self->swayTimer * (7.0f / 15.0f);
        rotAngle = Math_SinS(self->swayAngle) * (swayAmt * (65536.0f / 360.0f));

        if (self->absPrevSwayAngle >= ABS(rotAngle) && self->playSwayFlag == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_WAVE);
            self->playSwayFlag = 1;
        }

        if (self->absPrevSwayAngle < ABS(rotAngle)) {
            self->playSwayFlag = 0;
        }

        self->absPrevSwayAngle = ABS(rotAngle);
        amtToTranslate.x = Math_SinS(rotAngle) * -200.0f;
        amtToTranslate.y = Math_CosS(rotAngle) * -200.0f;
        amtToTranslate.z = 0.0f;
        Matrix_Push();
        Matrix_Translate(self->ceilingPos.x, self->ceilingPos.y, self->ceilingPos.z, MTXMODE_NEW);
        Matrix_RotateY(self->actor.world.rot.y * (M_PI / 32768.0f), MTXMODE_APPLY);
        Matrix_MultVec3f(&amtToTranslate, &translatedPos);
        Matrix_Pop();
        self->actor.shape.rot.z = -(rotAngle * 2);
        self->actor.world.pos.x = translatedPos.x;
        self->actor.world.pos.z = translatedPos.z;
    }
}

void EnSt_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSt* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 14.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &object_st_Skel_005298, NULL, self->jointTable, self->morphTable, 30);
    func_80034EC0(&self->skelAnime, sAnimations, 0);
    self->blureIdx = EnSt_CreateBlureEffect(globalCtx);
    EnSt_InitColliders(self, globalCtx);
    if (thisx->params == 2) {
        self->actor.flags |= 0x80;
    }
    if (self->actor.params == 1) {
        self->actor.naviEnemyId = 0x05;
    } else {
        self->actor.naviEnemyId = 0x04;
    }
    EnSt_CheckCeilingPos(self, globalCtx);
    self->actor.flags |= 0x4000;
    self->actor.flags |= 0x1000000;
    EnSt_SetColliderScale(self);
    self->actor.gravity = 0.0f;
    self->initalYaw = self->actor.world.rot.y;
    EnSt_SetupAction(self, EnSt_StartOnCeilingOrGround);
}

void EnSt_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnSt* self = THIS;
    s32 i;

    Effect_Delete(globalCtx, self->blureIdx);
    for (i = 0; i < 6; i++) {
        Collider_DestroyCylinder(globalCtx, &self->colCylinder[i]);
    }
    Collider_DestroyJntSph(globalCtx, &self->colSph);
}

void EnSt_WaitOnCeiling(EnSt* self, GlobalContext* globalCtx) {
    if (EnSt_IsCloseToPlayer(self, globalCtx)) {
        EnSt_SetDropAnimAndVel(self);
        EnSt_SetupAction(self, EnSt_MoveToGround);
    } else {
        EnSt_Bob(self, globalCtx);
    }
}

/**
 * Skulltula is waiting on the ground for the player to move away, or for
 * a collider to have contact
 */
void EnSt_WaitOnGround(EnSt* self, GlobalContext* globalCtx) {
    if (self->takeDamageSpinTimer != 0) {
        self->takeDamageSpinTimer--;
        if (self->takeDamageSpinTimer == 0) {
            func_80034EC0(&self->skelAnime, sAnimations, 3);
        }
    }

    if (self->animFrames != 0) {
        self->animFrames--;
        if (self->animFrames == 0) {
            func_80034EC0(&self->skelAnime, sAnimations, 3);
        }
    }

    if (!EnSt_IsCloseToPlayer(self, globalCtx)) {
        // Player is no longer within range, return to ceiling.
        EnSt_SetReturnToCeilingAnimation(self);
        EnSt_SetupAction(self, EnSt_ReturnToCeiling);
        return;
    }

    if (DECR(self->sfxTimer) == 0) {
        // play the "laugh" sfx every 64 frames.
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_LAUGH);
        self->sfxTimer = 64;
    }

    // simply bob up and down.
    EnSt_Bob(self, globalCtx);
}

void EnSt_LandOnGround(EnSt* self, GlobalContext* globalCtx) {
    if (self->animFrames != 0) {
        self->animFrames--;
        if (self->animFrames == 0) {
            func_80034EC0(&self->skelAnime, sAnimations, 3);
        }
    }

    if (self->takeDamageSpinTimer != 0) {
        self->takeDamageSpinTimer--;
        if (self->takeDamageSpinTimer == 0) {
            func_80034EC0(&self->skelAnime, sAnimations, 3);
        }
    }

    self->sfxTimer++;
    if (self->sfxTimer == 14) {
        // play the sound effect of the Skulltula hitting the ground.
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_DOWN_SET);
    }

    if ((self->actor.floorHeight + self->floorHeightOffset) < self->actor.world.pos.y) {
        // the Skulltula has hit the ground.
        self->sfxTimer = 0;
        EnSt_SetupAction(self, EnSt_WaitOnGround);
    } else {
        Math_SmoothStepToF(&self->actor.velocity.y, 2.0f, 0.3f, 1.0f, 0.0f);
    }
}

void EnSt_MoveToGround(EnSt* self, GlobalContext* globalCtx) {
    if (self->takeDamageSpinTimer != 0) {
        self->takeDamageSpinTimer--;
        if (self->takeDamageSpinTimer == 0) {
            func_80034EC0(&self->skelAnime, sAnimations, 5);
        }
    }

    if (!EnSt_IsCloseToPlayer(self, globalCtx)) {
        // the player moved out of range, return to the ceiling.
        EnSt_SetReturnToCeilingAnimation(self);
        EnSt_SetupAction(self, EnSt_ReturnToCeiling);
    } else if (EnSt_IsCloseToGround(self)) {
        // The Skulltula has become close to the ground.
        EnSt_SpawnBlastEffect(self, globalCtx);
        EnSt_SetLandAnimation(self);
        EnSt_SetupAction(self, EnSt_LandOnGround);
    } else if (DECR(self->sfxTimer) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALTU_DOWN);
        self->sfxTimer = 3;
    }
}

void EnSt_ReturnToCeiling(EnSt* self, GlobalContext* globalCtx) {
    f32 animPctDone = self->skelAnime.curFrame / (self->skelAnime.animLength - 1.0f);

    if (animPctDone == 1.0f) {
        EnSt_SetReturnToCeilingAnimation(self);
    }

    if (EnSt_IsCloseToPlayer(self, globalCtx)) {
        // player came back into range
        EnSt_SetDropAnimAndVel(self);
        EnSt_SetupAction(self, EnSt_MoveToGround);
    } else if (EnSt_IsCloseToInitalPos(self)) {
        // the Skulltula is close to the initial postion.
        EnSt_SetWaitingAnimation(self);
        EnSt_SetupAction(self, EnSt_WaitOnCeiling);
    } else {
        // accelerate based on the current animation frame.
        self->actor.velocity.y = 4.0f * animPctDone;
    }
}

/**
 * The Skulltula has been killed, bounce around
 */
void EnSt_BounceAround(EnSt* self, GlobalContext* globalCtx) {
    self->actor.colorFilterTimer = self->deathTimer;
    func_8002D868(&self->actor);
    self->actor.world.rot.x += 0x800;
    self->actor.world.rot.z -= 0x800;
    self->actor.shape.rot = self->actor.world.rot;
    if (EnSt_IsDoneBouncing(self, globalCtx)) {
        self->actor.shape.yOffset = 400.0f;
        self->actor.speedXZ = 1.0f;
        self->actor.gravity = -2.0f;
        EnSt_SetupAction(self, EnSt_FinishBouncing);
    } else {
        Math_SmoothStepToF(&self->actor.shape.yOffset, 400.0f, 0.4f, 10000.0f, 0.0f);
    }
}

/**
 * Finish up the bouncing animation, and rotate towards the final position
 */
void EnSt_FinishBouncing(EnSt* self, GlobalContext* globalCtx) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    if (DECR(self->deathTimer) == 0) {
        self->actor.velocity = zeroVec;
        self->finishDeathTimer = 8;
        EnSt_SetupAction(self, &EnSt_Die);
        return;
    }

    if (DECR(self->setTargetYawTimer) == 0) {
        self->deathYawTarget = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
        self->setTargetYawTimer = 8;
    }

    Math_SmoothStepToS(&self->actor.world.rot.x, 0x3FFC, 4, 0x2710, 1);
    Math_SmoothStepToS(&self->actor.world.rot.z, 0, 4, 0x2710, 1);
    Math_SmoothStepToS(&self->actor.world.rot.y, self->deathYawTarget, 0xA, 0x2710, 1);

    self->actor.shape.rot = self->actor.world.rot;

    func_8002D868(&self->actor);
    self->groundBounces = 2;
    EnSt_IsDoneBouncing(self, globalCtx);
}

/**
 * Spawn the enemy dying effects, and drop a random item
 */
void EnSt_Die(EnSt* self, GlobalContext* globalCtx) {
    if (DECR(self->finishDeathTimer) != 0) {
        EnSt_SpawnDeadEffect(self, globalCtx);
    } else {
        Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, 0xE0);
        Actor_Kill(&self->actor);
    }
}

void EnSt_StartOnCeilingOrGround(EnSt* self, GlobalContext* globalCtx) {
    if (!EnSt_IsCloseToGround(self)) {
        self->rotAwayTimer = 60;
        EnSt_SetupAction(self, EnSt_WaitOnCeiling);
        EnSt_WaitOnCeiling(self, globalCtx);
    } else {
        EnSt_SetLandAnimation(self);
        EnSt_SetupAction(self, EnSt_LandOnGround);
        EnSt_LandOnGround(self, globalCtx);
    }
}

void EnSt_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnSt* self = THIS;
    s32 pad;
    Color_RGBA8 color = { 0, 0, 0, 0 };

    if (self->actor.flags & 0x8000) {
        SkelAnime_Update(&self->skelAnime);
    } else if (!EnSt_CheckColliders(self, globalCtx)) {
        // no collision has been detected.

        if (self->stunTimer == 0) {
            SkelAnime_Update(&self->skelAnime);
        }

        if (self->swayTimer == 0 && self->stunTimer == 0) {
            func_8002D7EC(&self->actor);
        }

        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

        if ((self->stunTimer == 0) && (self->swayTimer == 0)) {
            // run the current action if the Skulltula isn't stunned
            // or swaying.
            self->actionFunc(self, globalCtx);
        } else if (self->stunTimer != 0) {
            // decrement the stun timer.
            EnSt_DecrStunTimer(self);
        } else {
            // sway the Skulltula.
            EnSt_Sway(self);
        }

        EnSt_UpdateYaw(self, globalCtx);

        if (self->actionFunc == EnSt_WaitOnGround) {
            if ((globalCtx->state.frames & 0x10) != 0) {
                color.r = 255;
            }
        }

        EnSt_SetTeethColor(self, color.r, color.g, color.b, 8);
        EnSt_UpdateCylinders(self, globalCtx);
        Actor_SetFocus(&self->actor, 0.0f);
    }
}

s32 EnSt_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dListP, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnSt* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_st.c", 2260);
    switch (limbIndex) {
        case 1:
            if (self->gaveDamageSpinTimer != 0 && self->swayTimer == 0) {
                if (self->gaveDamageSpinTimer >= 2) {
                    EnSt_AddBlurVertex(self);
                } else {
                    EnSt_AddBlurSpace(self);
                }
            }
            break;
        case 4:
            // teeth
            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, self->teethR, self->teethG, self->teethB, 0);
            break;
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_st.c", 2295);
    return false;
}

void EnSt_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dListP, Vec3s* rot, void* thisx) {
    EnSt* self = THIS;

    Collider_UpdateSpheres(limbIndex, &self->colSph);
}

void EnSt_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnSt* self = THIS;

    EnSt_CheckBodyStickHit(self, globalCtx);
    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnSt_OverrideLimbDraw,
                      EnSt_PostLimbDraw, self);
}
