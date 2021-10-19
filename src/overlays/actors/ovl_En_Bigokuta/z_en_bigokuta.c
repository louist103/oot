#include "z_en_bigokuta.h"
#include "objects/object_bigokuta/object_bigokuta.h"

#define FLAGS 0x00000035

#define THIS ((EnBigokuta*)thisx)

void EnBigokuta_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBigokuta_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBigokuta_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBigokuta_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809BD318(EnBigokuta* self);
void func_809BD3E0(EnBigokuta* self);
void func_809BDF34(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BD84C(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BD8DC(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BDAE8(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BDB90(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BDC08(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BE3E4(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BE4A4(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BE518(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BCF68(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BDFC8(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BE26C(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BE180(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BE058(EnBigokuta* self, GlobalContext* globalCtx);
void func_809BD1C8(EnBigokuta* self, GlobalContext* globalCtx);

static Color_RGBA8 sEffectPrimColor = { 255, 255, 255, 255 };
static Color_RGBA8 sEffectEnvColor = { 100, 255, 255, 255 };
static Vec3f sEffectPosAccel = { 0.0f, 0.0f, 0.0f };

const ActorInit En_Bigokuta_InitVars = {
    ACTOR_EN_BIGOKUTA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BIGOKUTA,
    sizeof(EnBigokuta),
    (ActorFunc)EnBigokuta_Init,
    (ActorFunc)EnBigokuta_Destroy,
    (ActorFunc)EnBigokuta_Update,
    (ActorFunc)EnBigokuta_Draw,
};

static ColliderJntSphElementInit sJntSphElementInit[1] = {
    {
        {
            ELEMTYPE_UNK1,
            { 0x20000000, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_HARD,
            BUMP_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 45, -30 }, 75 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT0,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementInit),
    sJntSphElementInit,
};

static ColliderCylinderInit sCylinderInit[] = {
    { {
          COLTYPE_HARD,
          AT_ON | AT_TYPE_ENEMY,
          AC_ON | AC_HARD | AC_TYPE_PLAYER,
          OC1_ON | OC1_TYPE_ALL,
          OC2_TYPE_1,
          COLSHAPE_CYLINDER,
      },
      {
          ELEMTYPE_UNK1,
          { 0x20000000, 0x00, 0x08 },
          { 0xFFCFFFE7, 0x00, 0x00 },
          TOUCH_ON | TOUCH_SFX_HARD,
          BUMP_ON,
          OCELEM_ON,
      },
      { 50, 100, 0, { 30, 0, 12 } } },
    { {
          COLTYPE_HARD,
          AT_ON | AT_TYPE_ENEMY,
          AC_ON | AC_HARD | AC_TYPE_PLAYER,
          OC1_ON | OC1_TYPE_ALL,
          OC2_TYPE_1,
          COLSHAPE_CYLINDER,
      },
      {
          ELEMTYPE_UNK1,
          { 0x20000000, 0x00, 0x08 },
          { 0xFFCFFFE7, 0x00, 0x00 },
          TOUCH_ON | TOUCH_SFX_HARD,
          BUMP_ON,
          OCELEM_ON,
      },
      { 50, 100, 0, { -30, 0, 12 } } },
};

static CollisionCheckInfoInit sColChkInfoInit[] = { 4, 130, 120, 200 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0xF),
    /* Boomerang     */ DMG_ENTRY(0, 0x1),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(0, 0x0),
    /* Giant's Knife */ DMG_ENTRY(0, 0x0),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(0, 0x0),
    /* Master spin   */ DMG_ENTRY(0, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(0, 0x0),
    /* Master jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -1, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x59, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 33, ICHAIN_STOP),
};

// possibly color data
static s32 sUnused[] = { 0xFFFFFFFF, 0x969696FF };

void EnBigokuta_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBigokuta* self = THIS;
    s32 i;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_bigokuta_Skel_006BC0, &object_bigokuta_Anim_0014B8,
                       self->jointTable, self->morphTable, 20);

    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, &self->element);

    self->collider.elements->dim.worldSphere.radius = self->collider.elements->dim.modelSphere.radius;

    for (i = 0; i < ARRAY_COUNT(sCylinderInit); i++) {
        Collider_InitCylinder(globalCtx, &self->cylinder[i]);
        Collider_SetCylinder(globalCtx, &self->cylinder[i], &self->actor, &sCylinderInit[i]);
    }

    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, sColChkInfoInit);

    self->unk_194 = 1;

    if (self->actor.params == 0) {
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
        func_809BD318(self);
    } else {
        func_809BD3E0(self);
        self->unk_19A = 0;
        self->unk_196 = 1;
        self->actor.home.pos.y = -1025.0f;
    }
}

void EnBigokuta_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBigokuta* self = THIS;
    s32 i;

    Collider_DestroyJntSph(globalCtx, &self->collider);
    for (i = 0; i < ARRAY_COUNT(self->cylinder); i++) {
        Collider_DestroyCylinder(globalCtx, &self->cylinder[i]);
    }
}

void func_809BCE3C(EnBigokuta* self) {
    self->actor.world.rot.y = self->actor.shape.rot.y + self->unk_194 * -0x4000;
    self->actor.world.pos.x = Math_SinS(self->actor.world.rot.y) * 263.0f + self->actor.home.pos.x;
    self->actor.world.pos.z = Math_CosS(self->actor.world.rot.y) * 263.0f + self->actor.home.pos.z;
}

void func_809BCEBC(EnBigokuta* self, GlobalContext* globalCtx) {
    Vec3f pos;
    f32 yDistFromHome = self->actor.world.pos.y - self->actor.home.pos.y;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.home.pos.y + 3.0f;
    pos.z = self->actor.world.pos.z;

    if (((globalCtx->gameplayFrames % 7) == 0) && (yDistFromHome <= 0.0f) && (yDistFromHome > -100.0f)) {
        EffectSsGRipple_Spawn(globalCtx, &pos, 800, 1300, 0);
    }
}

void func_809BCF68(EnBigokuta* self, GlobalContext* globalCtx) {
    Vec3f effectPos;
    s16 rot;

    if (globalCtx->gameplayFrames & 1) {
        rot = Rand_S16Offset(0x1200, 0xC00) + self->actor.shape.rot.y - self->unk_194 * 0xA00;
    } else {
        rot = self->actor.shape.rot.y - self->unk_194 * 0xA00 - Rand_S16Offset(0x1200, 0xC00);
    }
    if (self->actionFunc != func_809BE4A4) {
        if (self->actionFunc == func_809BE3E4 || (globalCtx->gameplayFrames & 2)) {
            effectPos.x = self->actor.world.pos.x - Math_SinS(rot) * 80.0f;
            effectPos.z = self->actor.world.pos.z - Math_CosS(rot) * 80.0f;
            effectPos.y = self->actor.home.pos.y + 1.0f;
            EffectSsGRipple_Spawn(globalCtx, &effectPos, 100, 500, 0);
        } else {
            effectPos.x = self->actor.world.pos.x - Math_SinS(rot) * 120.0f;
            effectPos.z = self->actor.world.pos.z - Math_CosS(rot) * 120.0f;
            effectPos.y = self->actor.home.pos.y + 5.0f;
        }
    } else {
        effectPos.x = self->actor.world.pos.x - Math_SinS(rot) * 50.0f;
        effectPos.z = self->actor.world.pos.z - Math_CosS(rot) * 50.0f;
        effectPos.y = self->actor.home.pos.y + 1.0f;
        EffectSsGRipple_Spawn(globalCtx, &effectPos, 100, 500, 0);
    }
    EffectSsGSplash_Spawn(globalCtx, &effectPos, NULL, NULL, 1, 800);
    if (self->actionFunc != func_809BE4A4) {
        func_8002F974(&self->actor, NA_SE_EN_DAIOCTA_SPLASH - SFX_FLAG);
    }
}

void func_809BD1C8(EnBigokuta* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3f effectPos;

    effectPos.y = self->actor.world.pos.y;

    for (i = 0; i < 4; i++) {
        effectPos.x = ((i >= 2) ? 1 : -1) * 60.0f + self->actor.world.pos.x;
        effectPos.z = ((i & 1) ? 1 : -1) * 60.0f + self->actor.world.pos.z;
        EffectSsGSplash_Spawn(globalCtx, &effectPos, NULL, NULL, 1, 2000);
    }

    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_LAND_WATER);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOLON_LAND_BIG);
    func_80033E88(&self->actor, globalCtx, 0xA, 8);
}

void func_809BD2E4(EnBigokuta* self) {
    Actor* parent = self->actor.parent;

    if (parent != NULL) {
        Math_ScaledStepToS(&parent->world.rot.y, self->unk_19A, 0x10);
    }
}

void func_809BD318(EnBigokuta* self) {
    Animation_PlayLoop(&self->skelAnime, &object_bigokuta_Anim_0014B8);
    self->unk_19A = 0;
    self->actor.home.pos.y = -1025.0f;
    self->unk_196 = 121;
    self->actionFunc = func_809BD84C;
}

void func_809BD370(EnBigokuta* self) {
    self->unk_196 = 21;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STAL_JUMP);
    self->actionFunc = func_809BD8DC;
}

void func_809BD3AC(EnBigokuta* self) {
    self->actor.world.pos.x = self->actor.home.pos.x + 263.0f;
    self->unk_196 = 10;
    self->actionFunc = func_809BDAE8;
    self->actor.world.pos.y = self->actor.home.pos.y;
}

void func_809BD3E0(EnBigokuta* self) {
    self->unk_196 = 40;
    self->actionFunc = func_809BDB90;
}

void func_809BD3F8(EnBigokuta* self) {
    Animation_MorphToLoop(&self->skelAnime, &object_bigokuta_Anim_001CA4, -5.0f);
    self->unk_196 = 350;
    self->unk_198 = 80;
    self->unk_19A = self->unk_194 * -0x200;
    func_809BCE3C(self);
    self->cylinder[0].base.atFlags |= AT_ON;
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = func_809BDC08;
}

void func_809BD47C(EnBigokuta* self) {
    self->unk_196 = 16;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.colorFilterTimer = 0;
    self->actionFunc = func_809BDF34;
}

void func_809BD4A4(EnBigokuta* self) {
    Animation_MorphToLoop(&self->skelAnime, &object_bigokuta_Anim_0014B8, -5.0f);
    self->unk_195 = true;
    self->actor.world.rot.x = self->actor.shape.rot.y + 0x8000;
    self->unk_19A = self->unk_194 * 0x200;
    self->collider.base.acFlags &= ~AC_ON;
    self->cylinder[0].base.atFlags |= AT_ON;
    self->actionFunc = func_809BDFC8;
}

void func_809BD524(EnBigokuta* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_bigokuta_Anim_000D1C, -5.0f);
    self->unk_196 = 80;
    self->unk_19A = 0;
    self->cylinder[0].base.atFlags |= AT_ON;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_MAHI);
    if (self->collider.elements->info.acHitInfo->toucher.dmgFlags & 1) {
        self->unk_195 = true;
        self->unk_196 = 20;
    } else {
        self->unk_195 = false;
        self->unk_196 = 80;
    }
    Actor_SetColorFilter(&self->actor, 0, 255, 0, self->unk_196);
    self->actionFunc = func_809BE058;
}

void func_809BD5E0(EnBigokuta* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_bigokuta_Anim_000444, -5.0f);
    self->unk_196 = 24;
    self->unk_19A = 0;
    self->cylinder[0].base.atFlags &= ~AT_ON;
    Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 24);
    self->actionFunc = func_809BE180;
}
void func_809BD658(EnBigokuta* self) {

    Animation_MorphToPlayOnce(&self->skelAnime, &object_bigokuta_Anim_000A74, -5.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_DEAD2);
    self->unk_196 = 38;
    self->unk_198 = 10;
    self->actionFunc = func_809BE26C;
}

void func_809BD6B8(EnBigokuta* self) {
    if (!self->unk_195) {
        if (Rand_ZeroOne() < 0.5f) {
            self->unk_196 = 24;
        } else {
            self->unk_196 = 28;
        }
    } else {
        if (ABS(self->actor.shape.rot.y - self->actor.yawTowardsPlayer) >= 0x4000) {
            self->unk_196 = 4;
        } else {
            self->unk_196 = 0;
        }
    }
    self->unk_19A = 0;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = func_809BE3E4;
}

void func_809BD768(EnBigokuta* self) {
    self->unk_194 = Rand_ZeroOne() < 0.5f ? -1 : 1;
    self->unk_19A = 0;
    self->actor.flags &= ~1;
    self->cylinder[0].base.atFlags &= ~AT_ON;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_SINK);
    self->actionFunc = func_809BE4A4;
}

void func_809BD7F0(EnBigokuta* self, GlobalContext* globalCtx) {
    self->actor.world.rot.y = Actor_WorldYawTowardPoint(&GET_PLAYER(globalCtx)->actor, &self->actor.home.pos);
    self->actor.shape.rot.y = self->actor.world.rot.y + (self->unk_194 * 0x4000);
    func_809BCE3C(self);
    self->actionFunc = func_809BE518;
}

void func_809BD84C(EnBigokuta* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    self->unk_196--;

    if (self->unk_196 == 13 || self->unk_196 == -20) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_VOICE);
    }
    if (self->unk_196 == 1) {
        func_800F5ACC(0x38);
    }
    if (self->actor.params == 1) {
        func_809BD370(self);
    }
}

void func_809BD8DC(EnBigokuta* self, GlobalContext* globalCtx) {
    Vec3f effectPos;

    self->unk_196--;

    if (self->unk_196 >= 0) {
        Math_StepToF(&self->actor.world.pos.x, self->actor.home.pos.x + 263.0f, 263.0f / 21);

        if (self->unk_196 < 14) {
            self->actor.world.pos.y = sinf(self->unk_196 * (M_PI / 28)) * 200.0f + self->actor.home.pos.y;
        } else {
            self->actor.world.pos.y =
                sinf((self->unk_196 - 7) * (M_PI / 14)) * 130.0f + (self->actor.home.pos.y + 70.0f);
        }
        if (self->unk_196 == 0) {
            effectPos.x = self->actor.world.pos.x + 40.0f;
            effectPos.y = self->actor.world.pos.y;
            effectPos.z = self->actor.world.pos.z - 70.0f;
            EffectSsGSplash_Spawn(globalCtx, &effectPos, NULL, NULL, 1, 2000);
            effectPos.x = self->actor.world.pos.x - 40.0f;
            EffectSsGSplash_Spawn(globalCtx, &effectPos, NULL, NULL, 1, 2000);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_LAND_WATER);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOLON_LAND_BIG);
            func_800AA000(0.0f, 0xFF, 0x14, 0x96);
        }
    } else if (self->unk_196 < -1) {
        self->actor.world.pos.y = self->actor.home.pos.y - (sinf((self->unk_196 + 1) * (M_PI / 10)) * 20.0f);
        if (self->unk_196 == -10) {
            func_809BD3AC(self);
        }
    }
}

void func_809BDAE8(EnBigokuta* self, GlobalContext* globalCtx) {
    if (Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y + 0x4000, 0x400)) {
        if (self->unk_196 != 0) {
            self->unk_196--;
        }
        if (self->unk_196 == 0) {
            func_809BCE3C(self);
            self->actor.home.pos.y = self->actor.world.pos.y;
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_ENEMY);
            self->actor.params = 2;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_VOICE);
            func_809BD3E0(self);
        }
    }
}

void func_809BDB90(EnBigokuta* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_196 != 0) {
        self->unk_196--;
    }
    if (self->unk_196 == 0) {
        if (self->actor.params == 3) {
            func_800F5ACC(0x38);
        }
        func_809BD3F8(self);
    }
}

void func_809BDC08(EnBigokuta* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 phi_v0;
    s16 pad;
    s16 phi_v1;
    Vec3f sp28;

    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_BUBLE);
    }

    if (self->unk_196 < 0) {
        self->actor.shape.rot.y += self->unk_194 * 0x200;
        func_809BCE3C(self);
        self->unk_196++;
        if (self->unk_196 == 0) {
            self->unk_196 = 350;
        }
        func_809BCF68(self, globalCtx);
        return;
    }

    phi_v1 = (Actor_WorldDistXZToPoint(&player->actor, &self->actor.home.pos) - 180.0f) * (8.0f / 15);
    func_8002DBD0(&self->actor, &sp28, &player->actor.world.pos);
    if (fabsf(sp28.x) > 263.0f || ((sp28.z > 0.0f) && !Actor_IsFacingPlayer(&self->actor, 0x1B00) &&
                                   !Player_IsFacingActor(&self->actor, 0x2000, globalCtx))) {
        phi_v1 -= 0x80;
        if (self->unk_196 != 0) {
            self->unk_196--;
        }
    }

    if ((self->actor.xzDistToPlayer < 250.0f) && !Actor_IsFacingPlayer(&self->actor, 0x6000)) {
        if (self->unk_198 != 0) {
            self->unk_198--;
        }
        if (self->actor.xzDistToPlayer < 180.0f) {
            phi_v1 += 0x20;
        }
    } else {
        self->unk_198 = 80;
    }
    if (self->actor.colChkInfo.health == 1) {
        phi_v1 = (phi_v1 + 0x130) * 1.1f;
    } else {
        phi_v1 += 0x130;
    }
    self->actor.shape.rot.y += phi_v1 * self->unk_194;
    func_809BCE3C(self);
    func_809BCF68(self, globalCtx);
    if (self->unk_198 == 0) {
        func_809BD768(self);
    } else if (self->unk_196 == 0) {
        func_809BD4A4(self);
    } else if (self->unk_195) {
        phi_v0 = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        if (phi_v0 < 0) {
            phi_v0 = -phi_v0;
        }
        if (phi_v0 < 0x4100 && phi_v0 > 0x3F00) {
            if (Rand_ZeroOne() < 0.6f) {
                self->unk_196 = 0;
                func_809BD4A4(self);
            }
            self->unk_195 = false;
        }
    }
}

void func_809BDF34(EnBigokuta* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_196 != 0) {
        self->unk_196--;
    }
    self->actor.world.pos.y = (sinf(self->unk_196 * (M_PI / 16)) * 100.0f) + self->actor.home.pos.y;
    if (self->unk_196 == 0) {
        func_809BD1C8(self, globalCtx);
        func_809BD3F8(self);
    }
}

void func_809BDFC8(EnBigokuta* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_196 != 0) {
        self->unk_196--;
    }
    if (self->unk_196 == 20) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_VOICE);
    }
    if ((self->unk_196 == 0) && Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.world.rot.x, 0x800)) {
        self->unk_194 = -self->unk_194;
        func_809BD3F8(self);
    }
}

void func_809BE058(EnBigokuta* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 speedXZ;

    if (self->unk_196 != 0) {
        self->unk_196--;
    }

    SkelAnime_Update(&self->skelAnime);

    if ((self->collider.base.ocFlags1 & OC1_HIT) || (self->cylinder[0].base.ocFlags1 & OC1_HIT) ||
        (self->cylinder[1].base.ocFlags1 & OC1_HIT)) {
        speedXZ = CLAMP_MIN(player->actor.speedXZ, 1.0f);
        if (!(self->collider.base.ocFlags1 & OC1_HIT)) {
            self->cylinder[0].base.ocFlags1 &= ~OC1_HIT;
            self->cylinder[1].base.ocFlags1 &= ~OC1_HIT;
            speedXZ *= -1.0f;
        }
        player->actor.world.pos.x -= speedXZ * Math_SinS(self->actor.shape.rot.y);
        player->actor.world.pos.z -= speedXZ * Math_CosS(self->actor.shape.rot.y);
    }
    if (self->unk_196 == 0) {
        func_809BD6B8(self);
    }
}

void func_809BE180(EnBigokuta* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_196 != 0) {
        self->unk_196--;
    }
    if (self->unk_196 == 0) {
        if (self->actor.colChkInfo.health != 0) {
            func_809BD4A4(self);
        } else {
            func_809BD658(self);
        }
    } else if (self->unk_196 >= 8) {
        self->actor.shape.rot.y += self->unk_194 * 0x200;
        self->actor.world.pos.y = sinf((self->unk_196 - 8) * (M_PI / 16)) * 100.0f + self->actor.home.pos.y;
        func_809BCE3C(self);
        if (self->unk_196 == 8) {
            func_809BD1C8(self, globalCtx);
        }
    }
}
void func_809BE26C(EnBigokuta* self, GlobalContext* globalCtx) {
    Vec3f effectPos;

    if (self->unk_196 != 0) {
        if (self->unk_196 != 0) {
            self->unk_196--;
        }
        if (self->unk_196 >= 10) {
            self->actor.shape.rot.y += 0x2000;
        }
    } else if (SkelAnime_Update(&self->skelAnime)) {
        if (self->unk_198 != 0) {
            self->unk_198--;
        }
        if (self->unk_198 == 6) {
            effectPos.x = self->actor.world.pos.x;
            effectPos.y = self->actor.world.pos.y + 150.0f;
            effectPos.z = self->actor.world.pos.z;
            func_8002829C(globalCtx, &effectPos, &sEffectPosAccel, &sEffectPosAccel, &sEffectPrimColor,
                          &sEffectEnvColor, 1200, 20);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_DEAD2);
        }
        if (self->unk_198 == 0 && Math_StepToF(&self->actor.scale.y, 0.0f, 0.001f)) {
            Flags_SetClear(globalCtx, self->actor.room);
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON0);
            func_8005ACFC(globalCtx->cameraPtrs[MAIN_CAM], 4);
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 50, NA_SE_EN_OCTAROCK_BUBLE);
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xB0);
            Actor_Kill(&self->actor);
        }
    }
}

void func_809BE3E4(EnBigokuta* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->unk_196 != 0) {
        if (self->unk_196 != 0) {
            self->unk_196--;
        }
        self->actor.shape.rot.y += 0x2000;
    }
    if (self->unk_196 == 0) {
        if ((s16)(self->actor.shape.rot.y - self->actor.world.rot.y) > 0) {
            self->unk_194 = 1;
        } else {
            self->unk_194 = -1;
        }

        func_809BD3F8(self);
        if (!self->unk_195) {
            self->unk_196 = -40;
        }
    }
    func_809BCF68(self, globalCtx);
}

void func_809BE4A4(EnBigokuta* self, GlobalContext* globalCtx) {
    self->actor.world.pos.y -= 10.0f;
    self->actor.shape.rot.y += 0x2000;
    if (self->actor.world.pos.y < (self->actor.home.pos.y + -200.0f)) {
        func_809BD7F0(self, globalCtx);
    }
    func_809BCF68(self, globalCtx);
}

void func_809BE518(EnBigokuta* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 10.0f)) {
        self->actor.flags |= 1;
        func_809BD3F8(self);
    }
}

void func_809BE568(EnBigokuta* self) {
    s32 i;
    f32 sin = Math_SinS(self->actor.shape.rot.y);
    f32 cos = Math_CosS(self->actor.shape.rot.y);

    self->collider.elements->dim.worldSphere.center.x =
        (self->collider.elements->dim.modelSphere.center.z * sin) +
        (self->actor.world.pos.x + (self->collider.elements->dim.modelSphere.center.x * cos));
    self->collider.elements->dim.worldSphere.center.z =
        (self->actor.world.pos.z + (self->collider.elements->dim.modelSphere.center.z * cos)) -
        (self->collider.elements->dim.modelSphere.center.x * sin);
    self->collider.elements->dim.worldSphere.center.y =
        self->collider.elements->dim.modelSphere.center.y + self->actor.world.pos.y;

    for (i = 0; i < ARRAY_COUNT(self->cylinder); i++) {
        self->cylinder[i].dim.pos.x =
            self->actor.world.pos.x + sCylinderInit[i].dim.pos.z * sin + sCylinderInit[i].dim.pos.x * cos;
        self->cylinder[i].dim.pos.z =
            self->actor.world.pos.z + sCylinderInit[i].dim.pos.z * cos - sCylinderInit[i].dim.pos.x * sin;
        self->cylinder[i].dim.pos.y = self->actor.world.pos.y;
    }
}

void func_809BE798(EnBigokuta* self, GlobalContext* globalCtx) {
    s16 effectRot;
    s16 yawDiff;

    if ((self->cylinder[0].base.atFlags & AT_HIT) || (self->cylinder[1].base.atFlags & AT_HIT) ||
        (self->collider.base.atFlags & AT_HIT)) {
        self->cylinder[0].base.atFlags &= ~AT_HIT;
        self->cylinder[1].base.atFlags &= ~AT_HIT;
        self->collider.base.atFlags &= ~AT_HIT;
        yawDiff = self->actor.yawTowardsPlayer - self->actor.world.rot.y;
        if (yawDiff > 0x4000) {
            effectRot = 0x4000;
        } else if (yawDiff > 0) {
            effectRot = 0x6000;
        } else if (yawDiff < -0x4000) {
            effectRot = -0x4000;
        } else {
            effectRot = -0x6000;
        }
        func_8002F71C(globalCtx, &self->actor, 10.0f, self->actor.world.rot.y + effectRot, 5.0f);
        if (self->actionFunc == func_809BDC08) {
            func_809BD4A4(self);
            self->unk_196 = 40;
        } else if (self->actionFunc == func_809BE3E4) {
            if ((effectRot * self->unk_194) > 0) {
                self->unk_194 = 0 - self->unk_194;
                self->unk_196 += 4;
            }
        }
    }
}

void EnBigokuta_UpdateDamage(EnBigokuta* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        if (self->actor.colChkInfo.damageEffect != 0 || self->actor.colChkInfo.damage != 0) {
            if (self->actor.colChkInfo.damageEffect == 1) {
                if (self->actionFunc != func_809BE058) {
                    func_809BD524(self);
                }
            } else if (self->actor.colChkInfo.damageEffect == 0xF) {
                func_809BD47C(self);
            } else if (!Actor_IsFacingPlayer(&self->actor, 0x4000)) {
                if (Actor_ApplyDamage(&self->actor) == 0) { // Dead
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_DEAD);
                    Enemy_StartFinishingBlow(globalCtx, &self->actor);
                } else {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DAIOCTA_DAMAGE);
                }
                func_809BD5E0(self);
            }
        }
    }
}

void EnBigokuta_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnBigokuta* self = THIS;
    s32 i;
    GlobalContext* globalCtx = globalCtx2;

    func_809BE798(self, globalCtx);
    EnBigokuta_UpdateDamage(self, globalCtx);
    self->actionFunc(self, globalCtx);
    func_809BD2E4(self);
    func_809BE568(self);
    Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_TAKO);
    func_8005AD1C(globalCtx->cameraPtrs[MAIN_CAM], 4);

    if (self->cylinder[0].base.atFlags & AT_ON) {
        if (self->actionFunc != func_809BE058) {
            for (i = 0; i < ARRAY_COUNT(self->cylinder); i++) {
                CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->cylinder[i].base);
            }
            self->actor.flags |= 0x1000000;
        } else {
            for (i = 0; i < ARRAY_COUNT(self->cylinder); i++) {
                CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->cylinder[i].base);
            }
        }
        for (i = 0; i < ARRAY_COUNT(self->cylinder); i++) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->cylinder[i].base);
        }
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
    if (self->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    } else {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    Actor_SetFocus(&self->actor, self->actor.scale.y * 25.0f * 100.0f);
    func_809BCEBC(self, globalCtx);
}

s32 EnBigokuta_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx) {
    EnBigokuta* self = THIS;
    u8 intensity;
    f32 temp_f0;
    s32 temp_hi;

    if (limbIndex == 15) {
        if (self->actionFunc == func_809BE058 || self->actionFunc == func_809BE180) {
            OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bigokuta.c", 1914);

            if (self->actionFunc == func_809BE058) {
                temp_hi = self->unk_196 % 12;
                if (temp_hi >= 8) {
                    temp_f0 = (12 - temp_hi) * (M_PI / 8);
                } else {
                    temp_f0 = temp_hi * (M_PI / 16);
                }
            } else {
                temp_hi = self->unk_196 % 6;
                if (temp_hi >= 4) {
                    temp_f0 = (6 - temp_hi) * (M_PI / 4);
                } else {
                    temp_f0 = temp_hi * (M_PI / 8);
                }
            }

            temp_f0 = sinf(temp_f0) * 125.0f + 130.0f;
            gDPPipeSync(POLY_OPA_DISP++);

            gDPSetEnvColor(POLY_OPA_DISP++, temp_f0, temp_f0, temp_f0, 255);
            CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bigokuta.c", 1945);
        }
    } else if (limbIndex == 10) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bigokuta.c", 1950);
        if (self->actionFunc == func_809BE26C) {
            intensity = self->unk_196 * (255.0f / 38);
        } else {
            intensity = 255;
        }
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, intensity, intensity, intensity, intensity);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bigokuta.c", 1972);
    } else if (limbIndex == 17 && self->actionFunc == func_809BE26C) {
        if (self->unk_198 < 5) {
            Matrix_Scale((self->unk_198 * 0.2f * 0.25f) + 1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
        } else if (self->unk_198 < 8) {
            temp_f0 = (self->unk_198 - 5) * (1.0f / 12);
            Matrix_Scale(1.25f - temp_f0, 1.0f + temp_f0, 1.0f + temp_f0, MTXMODE_APPLY);
        } else {
            temp_f0 = ((self->unk_198 - 8) * 0.125f);
            Matrix_Scale(1.0f, 1.25f - temp_f0, 1.25f - temp_f0, MTXMODE_APPLY);
        }
    }
    return false;
}

void EnBigokuta_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBigokuta* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bigokuta.c", 2017);

    if ((self->actionFunc != func_809BE26C) || (self->unk_196 != 0) || (self->unk_198 != 0)) {
        func_80093D18(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
        if (self->unk_196 & 1) {
            if ((self->actionFunc == func_809BE180 && self->unk_196 >= 8) ||
                (self->actionFunc == func_809BE26C && self->unk_196 >= 10)) {
                f32 rotX = Rand_ZeroOne() * (M_PI * 2.0f);
                f32 rotY = Rand_ZeroOne() * (M_PI * 2.0f);

                Matrix_RotateY(rotY, MTXMODE_APPLY);
                Matrix_RotateX(rotX, MTXMODE_APPLY);
                Matrix_Scale(0.78999996f, 1.3f, 0.78999996f, MTXMODE_APPLY);
                Matrix_RotateX(-rotX, MTXMODE_APPLY);
                Matrix_RotateY(-rotY, MTXMODE_APPLY);
            }
        }
        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnBigokuta_OverrideLimbDraw, NULL, self);
    } else {
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x0C, D_80116280);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, (self->actor.scale.y * 7727.273f));
        POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           self->skelAnime.dListCount, NULL, NULL, NULL, POLY_XLU_DISP);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bigokuta.c", 2076);
}
