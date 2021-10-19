#include "z_en_okuta.h"
#include "objects/object_okuta/object_okuta.h"

#define FLAGS 0x00000005

#define THIS ((EnOkuta*)thisx)

void EnOkuta_Init(Actor* thisx, GlobalContext* globalCtx);
void EnOkuta_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnOkuta_Update(Actor* thisx, GlobalContext* globalCtx);
void EnOkuta_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnOkuta_SetupWaitToAppear(EnOkuta* self);
void EnOkuta_WaitToAppear(EnOkuta* self, GlobalContext* globalCtx);
void EnOkuta_Appear(EnOkuta* self, GlobalContext* globalCtx);
void EnOkuta_Hide(EnOkuta* self, GlobalContext* globalCtx);
void EnOkuta_WaitToShoot(EnOkuta* self, GlobalContext* globalCtx);
void EnOkuta_Shoot(EnOkuta* self, GlobalContext* globalCtx);
void EnOkuta_WaitToDie(EnOkuta* self, GlobalContext* globalCtx);
void EnOkuta_Die(EnOkuta* self, GlobalContext* globalCtx);
void EnOkuta_Freeze(EnOkuta* self, GlobalContext* globalCtx);
void EnOkuta_ProjectileFly(EnOkuta* self, GlobalContext* globalCtx);

const ActorInit En_Okuta_InitVars = {
    ACTOR_EN_OKUTA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_OKUTA,
    sizeof(EnOkuta),
    (ActorFunc)EnOkuta_Init,
    (ActorFunc)EnOkuta_Destroy,
    (ActorFunc)EnOkuta_Update,
    (ActorFunc)EnOkuta_Draw,
};

static ColliderCylinderInit sProjectileColliderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_ON,
        OCELEM_ON,
    },
    { 13, 20, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sOctorockColliderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 40, -30, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 1, 15, 60, 100 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(1, 0x0),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(2, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(2, 0x0),
    /* Ice arrow     */ DMG_ENTRY(4, 0x3),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0x0),
    /* Master spin   */ DMG_ENTRY(2, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0x0),
    /* Master jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x42, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 6500, ICHAIN_STOP),
};

void EnOkuta_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnOkuta* self = THIS;
    s32 pad;
    WaterBox* outWaterBox;
    f32 ySurface;
    UNK_TYPE sp30;

    Actor_ProcessInitChain(thisx, sInitChain);
    self->numShots = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    if (thisx->params == 0) {
        SkelAnime_Init(globalCtx, &self->skelAnime, &gOctorokSkel, &gOctorokAppearAnim, self->jointTable,
                       self->morphTable, 38);
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, thisx, &sOctorockColliderInit);
        CollisionCheck_SetInfo(&thisx->colChkInfo, &sDamageTable, &sColChkInfoInit);
        if ((self->numShots == 0xFF) || (self->numShots == 0)) {
            self->numShots = 1;
        }
        thisx->floorHeight =
            BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &thisx->floorPoly, &sp30, thisx, &thisx->world.pos);
        //! @bug calls WaterBox_GetSurfaceImpl directly
        if (!WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, thisx->world.pos.x, thisx->world.pos.z, &ySurface,
                                     &outWaterBox) ||
            (ySurface <= thisx->floorHeight)) {
            Actor_Kill(thisx);
        } else {
            thisx->home.pos.y = ySurface;
        }
        EnOkuta_SetupWaitToAppear(self);
    } else {
        ActorShape_Init(&thisx->shape, 1100.0f, ActorShadow_DrawCircle, 18.0f);
        thisx->flags &= ~1;
        thisx->flags |= 0x10;
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, thisx, &sProjectileColliderInit);
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, thisx, ACTORCAT_PROP);
        self->timer = 30;
        thisx->shape.rot.y = 0;
        self->actionFunc = EnOkuta_ProjectileFly;
        thisx->speedXZ = 10.0f;
    }
}

void EnOkuta_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnOkuta* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnOkuta_SpawnBubbles(EnOkuta* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i < 10; i++) {
        EffectSsBubble_Spawn(globalCtx, &self->actor.world.pos, -10.0f, 10.0f, 30.0f, 0.25f);
    }
}

void EnOkuta_SpawnDust(Vec3f* pos, Vec3f* velocity, s16 scaleStep, GlobalContext* globalCtx) {
    static Vec3f accel = { 0.0f, 0.0f, 0.0f };
    static Color_RGBA8 primColor = { 255, 255, 255, 255 };
    static Color_RGBA8 envColor = { 150, 150, 150, 255 };

    func_8002829C(globalCtx, pos, velocity, &accel, &primColor, &envColor, 0x190, scaleStep);
}

void EnOkuta_SpawnSplash(EnOkuta* self, GlobalContext* globalCtx) {
    EffectSsGSplash_Spawn(globalCtx, &self->actor.home.pos, NULL, NULL, 0, 1300);
}

void EnOkuta_SpawnRipple(EnOkuta* self, GlobalContext* globalCtx) {
    Vec3f pos;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.home.pos.y;
    pos.z = self->actor.world.pos.z;
    if ((globalCtx->gameplayFrames % 7) == 0 &&
        ((self->actionFunc != EnOkuta_Shoot) || ((self->actor.world.pos.y - self->actor.home.pos.y) < 50.0f))) {
        EffectSsGRipple_Spawn(globalCtx, &pos, 250, 650, 0);
    }
}

void EnOkuta_SetupWaitToAppear(EnOkuta* self) {
    self->actor.draw = NULL;
    self->actor.flags &= ~1;
    self->actionFunc = EnOkuta_WaitToAppear;
    self->actor.world.pos.y = self->actor.home.pos.y;
}

void EnOkuta_SetupAppear(EnOkuta* self, GlobalContext* globalCtx) {
    self->actor.draw = EnOkuta_Draw;
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    self->actor.flags |= 1;
    Animation_PlayOnce(&self->skelAnime, &gOctorokAppearAnim);
    EnOkuta_SpawnBubbles(self, globalCtx);
    self->actionFunc = EnOkuta_Appear;
}

void EnOkuta_SetupHide(EnOkuta* self) {
    Animation_PlayOnce(&self->skelAnime, &gOctorokHideAnim);
    self->actionFunc = EnOkuta_Hide;
}

void EnOkuta_SetupWaitToShoot(EnOkuta* self) {
    Animation_PlayLoop(&self->skelAnime, &gOctorokFloatAnim);
    self->timer = (self->actionFunc == EnOkuta_Shoot) ? 2 : 0;
    self->actionFunc = EnOkuta_WaitToShoot;
}

void EnOkuta_SetupShoot(EnOkuta* self, GlobalContext* globalCtx) {
    Animation_PlayOnce(&self->skelAnime, &gOctorokShootAnim);
    if (self->actionFunc != EnOkuta_Shoot) {
        self->timer = self->numShots;
    }
    self->jumpHeight = self->actor.yDistToPlayer + 20.0f;
    self->jumpHeight = CLAMP_MIN(self->jumpHeight, 10.0f);
    if (self->jumpHeight > 50.0f) {
        EnOkuta_SpawnSplash(self, globalCtx);
    }
    if (self->jumpHeight > 50.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_JUMP);
    }
    self->actionFunc = EnOkuta_Shoot;
}

void EnOkuta_SetupWaitToDie(EnOkuta* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gOctorokHitAnim, -5.0f);
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0xB);
    self->collider.base.acFlags &= ~AC_HIT;
    Actor_SetScale(&self->actor, 0.01f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_DEAD1);
    self->actionFunc = EnOkuta_WaitToDie;
}

void EnOkuta_SetupDie(EnOkuta* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gOctorokDieAnim, -3.0f);
    self->timer = 0;
    self->actionFunc = EnOkuta_Die;
}

void EnOkuta_SetupFreeze(EnOkuta* self) {
    self->timer = 80;
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
    self->actionFunc = EnOkuta_Freeze;
}

void EnOkuta_SpawnProjectile(EnOkuta* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Vec3f velocity;
    f32 sin = Math_SinS(self->actor.shape.rot.y);
    f32 cos = Math_CosS(self->actor.shape.rot.y);

    pos.x = self->actor.world.pos.x + (25.0f * sin);
    pos.y = self->actor.world.pos.y - 6.0f;
    pos.z = self->actor.world.pos.z + (25.0f * cos);
    if (Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_OKUTA, pos.x, pos.y, pos.z, self->actor.shape.rot.x,
                    self->actor.shape.rot.y, self->actor.shape.rot.z, 0x10) != NULL) {
        pos.x = self->actor.world.pos.x + (40.0f * sin);
        pos.z = self->actor.world.pos.z + (40.0f * cos);
        pos.y = self->actor.world.pos.y;
        velocity.x = 1.5f * sin;
        velocity.y = 0.0f;
        velocity.z = 1.5f * cos;
        EnOkuta_SpawnDust(&pos, &velocity, 20, globalCtx);
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_THROW);
}

void EnOkuta_WaitToAppear(EnOkuta* self, GlobalContext* globalCtx) {
    self->actor.world.pos.y = self->actor.home.pos.y;
    if ((self->actor.xzDistToPlayer < 480.0f) && (self->actor.xzDistToPlayer > 200.0f)) {
        EnOkuta_SetupAppear(self, globalCtx);
    }
}

void EnOkuta_Appear(EnOkuta* self, GlobalContext* globalCtx) {
    s32 pad;

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.xzDistToPlayer < 160.0f) {
            EnOkuta_SetupHide(self);
        } else {
            EnOkuta_SetupWaitToShoot(self);
        }
    } else if (self->skelAnime.curFrame <= 4.0f) {
        Actor_SetScale(&self->actor, self->skelAnime.curFrame * 0.25f * 0.01f);
    } else if (Animation_OnFrame(&self->skelAnime, 5.0f)) {
        Actor_SetScale(&self->actor, 0.01f);
    }
    if (Animation_OnFrame(&self->skelAnime, 2.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_JUMP);
    }
    if (Animation_OnFrame(&self->skelAnime, 12.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_LAND);
    }
    if (Animation_OnFrame(&self->skelAnime, 3.0f) || Animation_OnFrame(&self->skelAnime, 15.0f)) {
        EnOkuta_SpawnSplash(self, globalCtx);
    }
}

void EnOkuta_Hide(EnOkuta* self, GlobalContext* globalCtx) {
    s32 pad;

    Math_ApproachF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.5f, 30.0f);
    if (SkelAnime_Update(&self->skelAnime)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_BUBLE);
        EnOkuta_SpawnBubbles(self, globalCtx);
        EnOkuta_SetupWaitToAppear(self);
    } else if (self->skelAnime.curFrame >= 4.0f) {
        Actor_SetScale(&self->actor, (6.0f - self->skelAnime.curFrame) * 0.5f * 0.01f);
    }
    if (Animation_OnFrame(&self->skelAnime, 2.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_SINK);
    }
    if (Animation_OnFrame(&self->skelAnime, 4.0f)) {
        EnOkuta_SpawnSplash(self, globalCtx);
    }
}

void EnOkuta_WaitToShoot(EnOkuta* self, GlobalContext* globalCtx) {
    s16 temp_v0_2;
    s32 phi_v1;

    self->actor.world.pos.y = self->actor.home.pos.y;
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
        if (self->timer != 0) {
            self->timer--;
        }
    }
    if (Animation_OnFrame(&self->skelAnime, 0.5f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_FLOAT);
    }
    if (self->actor.xzDistToPlayer < 160.0f || self->actor.xzDistToPlayer > 560.0f) {
        EnOkuta_SetupHide(self);
    } else {
        temp_v0_2 = Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x71C, 0x38E);
        phi_v1 = ABS(temp_v0_2);
        if ((phi_v1 < 0x38E) && (self->timer == 0) && (self->actor.yDistToPlayer < 200.0f)) {
            EnOkuta_SetupShoot(self, globalCtx);
        }
    }
}

void EnOkuta_Shoot(EnOkuta* self, GlobalContext* globalCtx) {
    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x71C);
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->timer != 0) {
            self->timer--;
        }
        if (self->timer == 0) {
            EnOkuta_SetupWaitToShoot(self);
        } else {
            EnOkuta_SetupShoot(self, globalCtx);
        }
    } else {
        f32 curFrame = self->skelAnime.curFrame;

        if (curFrame < 13.0f) {
            self->actor.world.pos.y = (sinf((0.08333f * M_PI) * curFrame) * self->jumpHeight) + self->actor.home.pos.y;
        }
        if (Animation_OnFrame(&self->skelAnime, 6.0f)) {
            EnOkuta_SpawnProjectile(self, globalCtx);
        }
        if ((self->jumpHeight > 50.0f) && Animation_OnFrame(&self->skelAnime, 13.0f)) {
            EnOkuta_SpawnSplash(self, globalCtx);
        }
        if ((self->jumpHeight > 50.0f) && Animation_OnFrame(&self->skelAnime, 13.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_LAND);
        }
    }
    if (self->actor.xzDistToPlayer < 160.0f) {
        EnOkuta_SetupHide(self);
    }
}

void EnOkuta_WaitToDie(EnOkuta* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnOkuta_SetupDie(self);
    }
    Math_ApproachF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.5f, 5.0f);
}

void EnOkuta_Die(EnOkuta* self, GlobalContext* globalCtx) {
    static Vec3f accel = { 0.0f, -0.5f, 0.0f };
    static Color_RGBA8 primColor = { 255, 255, 255, 255 };
    static Color_RGBA8 envColor = { 150, 150, 150, 0 };
    Vec3f velocity;
    Vec3f pos;
    s32 i;

    if (SkelAnime_Update(&self->skelAnime)) {
        self->timer++;
    }
    Math_ApproachF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.5f, 5.0f);
    if (self->timer == 5) {
        pos.x = self->actor.world.pos.x;
        pos.y = self->actor.world.pos.y + 40.0f;
        pos.z = self->actor.world.pos.z;
        velocity.x = 0.0f;
        velocity.y = -0.5f;
        velocity.z = 0.0f;
        EnOkuta_SpawnDust(&pos, &velocity, -0x14, globalCtx);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_DEAD2);
    }
    if (Animation_OnFrame(&self->skelAnime, 15.0f)) {
        EnOkuta_SpawnSplash(self, globalCtx);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_LAND);
    }
    if (self->timer < 3) {
        Actor_SetScale(&self->actor, ((self->timer * 0.25f) + 1.0f) * 0.01f);
    } else if (self->timer < 6) {
        Actor_SetScale(&self->actor, (1.5f - ((self->timer - 2) * 0.2333f)) * 0.01f);
    } else if (self->timer < 11) {
        Actor_SetScale(&self->actor, (((self->timer - 5) * 0.04f) + 0.8f) * 0.01f);
    } else {
        if (Math_StepToF(&self->actor.scale.x, 0.0f, 0.0005f)) {
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 30, NA_SE_EN_OCTAROCK_BUBLE);
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x70);
            for (i = 0; i < 20; i++) {
                velocity.x = (Rand_ZeroOne() - 0.5f) * 7.0f;
                velocity.y = Rand_ZeroOne() * 7.0f;
                velocity.z = (Rand_ZeroOne() - 0.5f) * 7.0f;
                EffectSsDtBubble_SpawnCustomColor(globalCtx, &self->actor.world.pos, &velocity, &accel, &primColor,
                                                  &envColor, Rand_S16Offset(100, 50), 25, 0);
            }
            Actor_Kill(&self->actor);
        }
        self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;
    }
}

void EnOkuta_Freeze(EnOkuta* self, GlobalContext* globalCtx) {
    Vec3f pos;
    s16 temp_v1;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        EnOkuta_SetupDie(self);
    }
    if ((self->timer >= 64) && (self->timer & 1)) {
        temp_v1 = (self->timer - 64) >> 1;
        pos.y = (self->actor.world.pos.y - 32.0f) + (8.0f * (8 - temp_v1));
        pos.x = self->actor.world.pos.x + ((temp_v1 & 2) ? 10.0f : -10.0f);
        pos.z = self->actor.world.pos.z + ((temp_v1 & 1) ? 10.0f : -10.0f);
        EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &pos, 150, 150, 150, 250, 235, 245, 255,
                                       (Rand_ZeroOne() * 0.2f) + 1.9f);
    }
    Math_ApproachF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.5f, 5.0f);
}

void EnOkuta_ProjectileFly(EnOkuta* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Player* player = GET_PLAYER(globalCtx);
    Vec3s sp40;

    self->timer--;
    if (self->timer == 0) {
        self->actor.gravity = -1.0f;
    }
    self->actor.home.rot.z += 0x1554;
    if (self->actor.bgCheckFlags & 0x20) {
        self->actor.gravity = -1.0f;
        self->actor.speedXZ -= 0.1f;
        self->actor.speedXZ = CLAMP_MIN(self->actor.speedXZ, 1.0f);
    }
    if ((self->actor.bgCheckFlags & 8) || (self->actor.bgCheckFlags & 1) || (self->collider.base.atFlags & AT_HIT) ||
        self->collider.base.acFlags & AC_HIT || self->collider.base.ocFlags1 & OC1_HIT ||
        self->actor.floorHeight == BGCHECK_Y_MIN) {
        if ((player->currentShield == PLAYER_SHIELD_DEKU ||
             (player->currentShield == PLAYER_SHIELD_HYLIAN && LINK_IS_ADULT)) &&
            self->collider.base.atFlags & AT_HIT && self->collider.base.atFlags & AT_TYPE_ENEMY &&
            self->collider.base.atFlags & AT_BOUNCED) {
            self->collider.base.atFlags &= ~(AT_HIT | AT_BOUNCED | AT_TYPE_ENEMY);
            self->collider.base.atFlags |= AT_TYPE_PLAYER;
            self->collider.info.toucher.dmgFlags = 2;
            Matrix_MtxFToYXZRotS(&player->shieldMf, &sp40, 0);
            self->actor.world.rot.y = sp40.y + 0x8000;
            self->timer = 30;
        } else {
            pos.x = self->actor.world.pos.x;
            pos.y = self->actor.world.pos.y + 11.0f;
            pos.z = self->actor.world.pos.z;
            EffectSsHahen_SpawnBurst(globalCtx, &pos, 6.0f, 0, 1, 2, 15, 7, 10, gOctorokProjectileDL);
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EN_OCTAROCK_ROCK);
            Actor_Kill(&self->actor);
        }
    } else if (self->timer == -300) {
        Actor_Kill(&self->actor);
    }
}

void EnOkuta_UpdateHeadScale(EnOkuta* self) {
    f32 curFrame = self->skelAnime.curFrame;

    if (self->actionFunc == EnOkuta_Appear) {
        if (curFrame < 8.0f) {
            self->headScale.x = self->headScale.y = self->headScale.z = 1.0f;
        } else if (curFrame < 10.0f) {
            self->headScale.x = self->headScale.z = 1.0f;
            self->headScale.y = ((curFrame - 7.0f) * 0.4f) + 1.0f;
        } else if (curFrame < 14.0f) {
            self->headScale.x = self->headScale.z = ((curFrame - 9.0f) * 0.075f) + 1.0f;
            self->headScale.y = 1.8f - ((curFrame - 9.0f) * 0.25f);
        } else {
            self->headScale.x = self->headScale.z = 1.3f - ((curFrame - 13.0f) * 0.05f);
            self->headScale.y = ((curFrame - 13.0f) * 0.0333f) + 0.8f;
        }
    } else if (self->actionFunc == EnOkuta_Hide) {
        if (curFrame < 3.0f) {
            self->headScale.y = 1.0f;
        } else if (curFrame < 4.0f) {
            self->headScale.y = (curFrame - 2.0f) + 1.0f;
        } else {
            self->headScale.y = 2.0f - ((curFrame - 3.0f) * 0.333f);
        }
        self->headScale.x = self->headScale.z = 1.0f;
    } else if (self->actionFunc == EnOkuta_Shoot) {
        if (curFrame < 5.0f) {
            self->headScale.x = self->headScale.y = self->headScale.z = (curFrame * 0.125f) + 1.0f;
        } else if (curFrame < 7.0f) {
            self->headScale.x = self->headScale.y = self->headScale.z = 1.5f - ((curFrame - 4.0f) * 0.35f);
        } else if (curFrame < 17.0f) {
            self->headScale.x = self->headScale.z = ((curFrame - 6.0f) * 0.05f) + 0.8f;
            self->headScale.y = 0.8f;
        } else {
            self->headScale.x = self->headScale.z = 1.3f - ((curFrame - 16.0f) * 0.1f);
            self->headScale.y = ((curFrame - 16.0f) * 0.0666f) + 0.8f;
        }
    } else if (self->actionFunc == EnOkuta_WaitToShoot) {
        self->headScale.x = self->headScale.z = 1.0f;
        self->headScale.y = (sinf((M_PI / 16) * curFrame) * 0.2f) + 1.0f;
    } else {
        self->headScale.x = self->headScale.y = self->headScale.z = 1.0f;
    }
}

void EnOkuta_ColliderCheck(EnOkuta* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);
        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            Enemy_StartFinishingBlow(globalCtx, &self->actor);
            self->actor.colChkInfo.health = 0;
            self->actor.flags &= ~1;
            if (self->actor.colChkInfo.damageEffect == 3) {
                EnOkuta_SetupFreeze(self);
            } else {
                EnOkuta_SetupWaitToDie(self);
            }
        }
    }
}

void EnOkuta_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnOkuta* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    Player* player = GET_PLAYER(globalCtx);
    WaterBox* outWaterBox;
    f32 ySurface;
    Vec3f sp38;
    s32 sp34;

    if (!(player->stateFlags1 & 0x300000C0)) {
        if (self->actor.params == 0) {
            EnOkuta_ColliderCheck(self, globalCtx);
            if (!WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->actor.world.pos.x,
                                         self->actor.world.pos.z, &ySurface, &outWaterBox) ||
                (ySurface < self->actor.floorHeight)) {
                if (self->actor.colChkInfo.health != 0) {
                    Actor_Kill(&self->actor);
                    return;
                }
            } else {
                self->actor.home.pos.y = ySurface;
            }
        }
        self->actionFunc(self, globalCtx);
        if (self->actor.params == 0) {
            EnOkuta_UpdateHeadScale(self);
            self->collider.dim.height =
                (((sOctorockColliderInit.dim.height * self->headScale.y) - self->collider.dim.yShift) *
                 self->actor.scale.y * 100.0f);
        } else {
            sp34 = false;
            Actor_MoveForward(&self->actor);
            Math_Vec3f_Copy(&sp38, &self->actor.world.pos);
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 15.0f, 30.0f, 5);
            if ((self->actor.bgCheckFlags & 8) &&
                SurfaceType_IsIgnoredByProjectiles(&globalCtx->colCtx, self->actor.wallPoly, self->actor.wallBgId)) {
                sp34 = true;
                self->actor.bgCheckFlags &= ~8;
            }
            if ((self->actor.bgCheckFlags & 1) &&
                SurfaceType_IsIgnoredByProjectiles(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId)) {
                sp34 = true;
                self->actor.bgCheckFlags &= ~1;
            }
            if (sp34 && !(self->actor.bgCheckFlags & 9)) {
                Math_Vec3f_Copy(&self->actor.world.pos, &sp38);
            }
        }
        Collider_UpdateCylinder(&self->actor, &self->collider);
        if ((self->actionFunc == EnOkuta_Appear) || (self->actionFunc == EnOkuta_Hide)) {
            self->collider.dim.pos.y = self->actor.world.pos.y + (self->skelAnime.jointTable->y * self->actor.scale.y);
            self->collider.dim.radius = sOctorockColliderInit.dim.radius * self->actor.scale.x * 100.0f;
        }
        if (self->actor.params == 0x10) {
            self->actor.flags |= 0x1000000;
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        if (self->actionFunc != EnOkuta_WaitToAppear) {
            if ((self->actionFunc != EnOkuta_Die) && (self->actionFunc != EnOkuta_WaitToDie) &&
                (self->actionFunc != EnOkuta_Freeze)) {
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            }
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        Actor_SetFocus(&self->actor, 15.0f);
        if ((self->actor.params == 0) && (self->actor.draw != NULL)) {
            EnOkuta_SpawnRipple(self, globalCtx);
        }
    }
}

s32 EnOkuta_GetSnoutScale(EnOkuta* self, f32 curFrame, Vec3f* scale) {
    if (self->actionFunc == EnOkuta_WaitToShoot) {
        scale->x = scale->z = 1.0f;
        scale->y = (sinf((M_PI / 16) * curFrame) * 0.4f) + 1.0f;
    } else if (self->actionFunc == EnOkuta_Shoot) {
        if (curFrame < 5.0f) {
            scale->x = 1.0f;
            scale->y = scale->z = (curFrame * 0.25f) + 1.0f;
        } else if (curFrame < 7.0f) {
            scale->x = (curFrame - 4.0f) * 0.5f + 1.0f;
            scale->y = scale->z = 2.0f - (curFrame - 4.0f) * 0.5f;
        } else {
            scale->x = 2.0f - ((curFrame - 6.0f) * 0.0769f);
            scale->y = scale->z = 1.0f;
        }
    } else if (self->actionFunc == EnOkuta_Die) {
        if (curFrame >= 35.0f || curFrame < 25.0f) {
            return false;
        }
        if (curFrame < 27.0f) {
            scale->x = 1.0f;
            scale->y = scale->z = ((curFrame - 24.0f) * 0.5f) + 1.0f;
        } else if (curFrame < 30.0f) {
            scale->x = (curFrame - 26.0f) * 0.333f + 1.0f;
            scale->y = scale->z = 2.0f - (curFrame - 26.0f) * 0.333f;
        } else {
            scale->x = 2.0f - ((curFrame - 29.0f) * 0.2f);
            scale->y = scale->z = 1.0f;
        }
    } else {
        return false;
    }

    return true;
}

s32 EnOkuta_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                             void* thisx) {
    EnOkuta* self = THIS;
    f32 curFrame = self->skelAnime.curFrame;
    Vec3f scale;
    s32 doScale = false;

    if (self->actionFunc == EnOkuta_Die) {
        curFrame += self->timer;
    }
    if (limbIndex == 5) {
        if ((self->headScale.x != 1.0f) || (self->headScale.y != 1.0f) || (self->headScale.z != 1.0f)) {
            scale = self->headScale;
            doScale = true;
        }
    } else if (limbIndex == 8) {
        doScale = EnOkuta_GetSnoutScale(self, curFrame, &scale);
    }
    if (doScale) {
        Matrix_Scale(scale.x, scale.y, scale.z, MTXMODE_APPLY);
    }
    return false;
}

void EnOkuta_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnOkuta* self = THIS;
    s32 pad;

    func_80093D18(globalCtx->state.gfxCtx);

    if (self->actor.params == 0) {
        SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnOkuta_OverrideLimbDraw,
                          NULL, self);
    } else {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_okuta.c", 1653);

        Matrix_Mult(&globalCtx->mf_11DA0, MTXMODE_APPLY);
        Matrix_RotateZ(self->actor.home.rot.z * (M_PI / 0x8000), MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_okuta.c", 1657),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gOctorokProjectileDL);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_okuta.c", 1662);
    }
}
