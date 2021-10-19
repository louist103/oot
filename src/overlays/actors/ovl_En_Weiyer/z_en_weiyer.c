/*
 * File: z_en_weiyer.c
 * Overlay: ovl_En_Weiyer
 * Description: Stinger (Water)
 */

#include "z_en_weiyer.h"
#include "objects/object_ei/object_ei.h"

#define FLAGS 0x00000005

#define THIS ((EnWeiyer*)thisx)

void EnWeiyer_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWeiyer_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWeiyer_Update(Actor* thisx, GlobalContext* globalCtx);
void EnWeiyer_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B32804(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B328E8(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B32C2C(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B32D30(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B32E34(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B33018(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B331CC(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B333B8(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B332B4(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B33338(EnWeiyer* self, GlobalContext* globalCtx);
void func_80B3349C(EnWeiyer* self, GlobalContext* globalCtx);

const ActorInit En_Weiyer_InitVars = {
    ACTOR_EN_WEIYER,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_EI,
    sizeof(EnWeiyer),
    (ActorFunc)EnWeiyer_Init,
    (ActorFunc)EnWeiyer_Destroy,
    (ActorFunc)EnWeiyer_Update,
    (ActorFunc)EnWeiyer_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
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
    { 16, 10, -6, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 2, 45, 15, 100 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x1),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(2, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(2, 0x0),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
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
    ICHAIN_S8(naviEnemyId, 0x19, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 3, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2500, ICHAIN_STOP),
};

void EnWeiyer_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnWeiyer* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    ActorShape_Init(&self->actor.shape, 1000.0f, ActorShadow_DrawCircle, 65.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gStingerSkel, &gStingerIdleAnim, self->jointTable, self->morphTable,
                   19);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    self->actionFunc = func_80B32804;
}

void EnWeiyer_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnWeiyer* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80B32384(EnWeiyer* self) {
    self->unk_196 = self->actor.shape.rot.y;
    self->unk_27C = (cosf(-M_PI / 8) * 3.0f) + self->actor.world.pos.y;
    Animation_MorphToLoop(&self->skelAnime, &gStingerHitAnim, -5.0f);
    self->unk_194 = 30;
    self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 2.5f);
    self->collider.base.atFlags &= ~AT_ON;
    self->unk_280 = self->actor.floorHeight;
    self->actionFunc = func_80B328E8;
}

void func_80B32434(EnWeiyer* self) {
    Animation_MorphToLoop(&self->skelAnime, &gStingerHitAnim, -5.0f);
    self->collider.base.atFlags |= AT_ON;
    self->unk_194 = 0;
    self->actor.speedXZ = 5.0f;
    self->actionFunc = func_80B32C2C;
}

void func_80B32494(EnWeiyer* self) {
    Animation_Change(&self->skelAnime, &gStingerPopOutAnim, 2.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f);
    self->unk_194 = 40;
    self->collider.base.atFlags |= AT_ON;
    self->actionFunc = func_80B32D30;
}

void func_80B32508(EnWeiyer* self) {
    self->unk_194 = 200;
    self->collider.base.atFlags |= AT_ON;
    self->skelAnime.playSpeed = 3.0f;
    self->actionFunc = func_80B32E34;
}

void func_80B32538(EnWeiyer* self) {
    self->unk_194 = 200;
    self->unk_196 = self->actor.yawTowardsPlayer + 0x8000;
    self->unk_27C = self->actor.world.pos.y;
    self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 4.0f);
    self->collider.base.atFlags &= ~AT_ON;
    self->skelAnime.playSpeed = 1.0f;
    self->actionFunc = func_80B33018;
}

void func_80B325A0(EnWeiyer* self) {
    Animation_Change(&self->skelAnime, &gStingerHitAnim, 2.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -3.0f);
    self->unk_194 = 40;
    self->collider.base.atFlags &= ~AT_ON;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.speedXZ = 3.0f;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xC8, 0, 0x28);
    self->collider.dim.height = sCylinderInit.dim.height;
    self->actionFunc = func_80B331CC;
}

void func_80B32660(EnWeiyer* self) {
    Animation_Change(&self->skelAnime, &gStingerPopOutAnim, 2.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f);
    self->unk_194 = 80;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = -1.0f;
    self->collider.dim.height = sCylinderInit.dim.height + 15;
    Actor_SetColorFilter(&self->actor, 0, 0xC8, 0, 0x50);
    self->collider.base.atFlags &= ~AT_ON;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->actionFunc = func_80B333B8;
}

void func_80B32724(EnWeiyer* self) {
    Animation_MorphToLoop(&self->skelAnime, &gStingerHitAnim, -5.0f);
    self->unk_194 = 20;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xC8, 0, 0x28);
    self->collider.base.atFlags &= ~AT_ON;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.speedXZ = 3.0f;
    self->actionFunc = func_80B332B4;
}

void func_80B327B0(EnWeiyer* self) {
    self->actor.colorFilterParams |= 0x2000;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actionFunc = func_80B33338;
}

void func_80B327D8(EnWeiyer* self) {
    self->actor.shape.rot.x = -0x2000;
    self->unk_194 = -1;
    self->actor.speedXZ = 5.0f;
    self->actionFunc = func_80B3349C;
}

void func_80B32804(EnWeiyer* self, GlobalContext* globalCtx) {
    WaterBox* waterBox;
    s32 bgId;

    self->actor.world.pos.y += 0.5f;
    self->actor.floorHeight = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->actor.floorPoly, &bgId,
                                                          &self->actor, &self->actor.world.pos);

    if (!WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->actor.world.pos.x, self->actor.world.pos.z,
                                 &self->actor.home.pos.y, &waterBox) ||
        ((self->actor.home.pos.y - 5.0f) <= self->actor.floorHeight)) {
        Actor_Kill(&self->actor);
    } else {
        self->actor.home.pos.y -= 5.0f;
        self->actor.world.pos.y = (self->actor.home.pos.y + self->actor.floorHeight) / 2.0f;
        func_80B32384(self);
    }
}

void func_80B328E8(EnWeiyer* self, GlobalContext* globalCtx) {
    s32 sp34;
    f32 curFrame;

    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x800);
    sp34 = Animation_OnFrame(&self->skelAnime, 0.0f);
    curFrame = self->skelAnime.curFrame;
    Math_StepToF(&self->unk_27C, self->unk_280, 0.5f);
    self->actor.world.pos.y = self->unk_27C - cosf((curFrame - 5.0f) * (M_PI / 40)) * 3.0f;

    if (curFrame <= 45.0f) {
        Math_StepToF(&self->actor.speedXZ, 1.0f, 0.03f);
    } else {
        Math_StepToF(&self->actor.speedXZ, 1.3f, 0.03f);
    }

    if (self->actor.bgCheckFlags & 8) {
        self->unk_196 = self->actor.wallYaw;
        self->unk_194 = 30;
    }

    if (Math_ScaledStepToS(&self->actor.shape.rot.y, self->unk_196, 182)) {
        if (self->unk_194 != 0) {
            self->unk_194--;
        }

        if (self->unk_194 == 0) {
            self->unk_196 =
                Rand_S16Offset(0x2000, 0x2000) * ((Rand_ZeroOne() < 0.5f) ? -1 : 1) + self->actor.shape.rot.y;
            self->unk_194 = 30;

            if (Rand_ZeroOne() < 0.3333f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_CRY);
            }
        }
    }

    if (self->actor.home.pos.y < self->actor.world.pos.y) {
        if (self->actor.home.pos.y < self->actor.floorHeight) {
            func_80B32434(self);
        } else {
            self->actor.world.pos.y = self->actor.home.pos.y;
            self->unk_280 =
                Rand_ZeroOne() * ((self->actor.home.pos.y - self->actor.floorHeight) / 2.0f) + self->actor.floorHeight;
        }
    } else {
        Player* player = GET_PLAYER(globalCtx);

        if (self->actor.bgCheckFlags & 1) {
            self->unk_280 =
                self->actor.home.pos.y - Rand_ZeroOne() * ((self->actor.home.pos.y - self->actor.floorHeight) / 2.0f);
        } else if (sp34 && (Rand_ZeroOne() < 0.1f)) {
            self->unk_280 =
                Rand_ZeroOne() * (self->actor.home.pos.y - self->actor.floorHeight) + self->actor.floorHeight;
        }

        if ((self->actor.xzDistToPlayer < 400.0f) && (fabsf(self->actor.yDistToPlayer) < 250.0f) &&
            (player->actor.world.pos.y < (self->actor.home.pos.y + 20.0f))) {
            func_80B32508(self);
        }
    }
}

void func_80B32C2C(EnWeiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->unk_194 == 0) {
        if (Math_ScaledStepToS(&self->actor.shape.rot.x, -0x4000, 0x800)) {
            self->actor.shape.rot.z = 0;
            self->actor.shape.rot.y += 0x8000;
            self->unk_194 = 1;
        } else {
            self->actor.shape.rot.z = self->actor.shape.rot.x * 2;
        }
    } else {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x1800, 0x800);

        if (self->actor.world.pos.y < self->actor.home.pos.y) {
            if (self->actor.shape.rot.x > 0) {
                EffectSsGSplash_Spawn(globalCtx, &self->actor.world.pos, NULL, NULL, 1, 400);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_SINK);
            }

            func_80B32538(self);
        } else if (self->actor.bgCheckFlags & 1) {
            func_80B32494(self);
        }
    }
}

void func_80B32D30(EnWeiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_FLUTTER);
    }

    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x800);
    Math_StepToF(&self->actor.speedXZ, 0.0f, 1.0f);

    if (self->unk_194 != 0) {
        self->unk_194--;
    }

    if (self->unk_194 == 0) {
        func_80B32434(self);
    } else if (self->actor.world.pos.y < self->actor.home.pos.y) {
        func_80B32384(self);
    }
}

s16 func_80B32DEC(EnWeiyer* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f vec;

    vec.x = player->actor.world.pos.x;
    vec.y = player->actor.world.pos.y + 20.0f;
    vec.z = player->actor.world.pos.z;

    return Actor_WorldPitchTowardPoint(&self->actor, &vec);
}

void func_80B32E34(EnWeiyer* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);

    if (self->unk_194 != 0) {
        self->unk_194--;
    }

    if ((self->unk_194 == 0) || ((self->actor.home.pos.y + 20.0f) <= player->actor.world.pos.y) ||
        (self->collider.base.atFlags & AT_HIT)) {
        func_80B32538(self);
    } else {
        if (Actor_IsFacingPlayer(&self->actor, 0x2800)) {
            Math_StepToF(&self->actor.speedXZ, 4.0f, 0.2f);
        } else {
            Math_StepToF(&self->actor.speedXZ, 1.3f, 0.2f);
        }

        if (self->actor.home.pos.y < self->actor.world.pos.y) {
            if (self->actor.home.pos.y < self->actor.floorHeight) {
                self->actor.shape.rot.x = 0;
                func_80B32434(self);
                return;
            }

            self->actor.world.pos.y = self->actor.home.pos.y;
            Math_SmoothStepToS(&self->actor.shape.rot.x, 0x1000, 2, 0x100, 0x40);
        } else {
            Math_SmoothStepToS(&self->actor.shape.rot.x, func_80B32DEC(self, globalCtx), 2, 0x100, 0x40);
        }

        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0x200, 0x80);

        if ((player->actor.yDistToWater < 50.0f) && (self->actor.yDistToWater < 20.0f) &&
            Actor_IsFacingPlayer(&self->actor, 0x2000)) {
            func_80B327D8(self);
        }
    }
}

void func_80B33018(EnWeiyer* self, GlobalContext* globalCtx) {
    f32 curFrame;

    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x800);
    curFrame = self->skelAnime.curFrame;
    Math_StepToF(&self->unk_27C, (self->actor.home.pos.y - self->actor.floorHeight) / 4.0f + self->actor.floorHeight,
                 1.0f);
    self->actor.world.pos.y = self->unk_27C - cosf((curFrame - 5.0f) * (M_PI / 40)) * 3.0f;

    if (curFrame <= 45.0f) {
        Math_StepToF(&self->actor.speedXZ, 1.0f, 0.03f);
    } else {
        Math_StepToF(&self->actor.speedXZ, 1.3f, 0.03f);
    }

    if (self->unk_194 != 0) {
        self->unk_194--;
    }

    if (self->actor.bgCheckFlags & 8) {
        self->unk_196 = self->actor.wallYaw;
    }

    if (Math_SmoothStepToS(&self->actor.shape.rot.y, self->unk_196, 2, 0x200, 0x80) == 0) {
        self->unk_196 = self->actor.yawTowardsPlayer + 0x8000;
    }

    if (self->actor.home.pos.y < self->actor.world.pos.y) {
        if (self->actor.home.pos.y < self->actor.floorHeight) {
            func_80B32434(self);
        } else {
            self->actor.world.pos.y = self->actor.home.pos.y;
        }
    }

    if (self->unk_194 == 0) {
        func_80B32384(self);
    }
}

void func_80B331CC(EnWeiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->unk_194 != 0) {
        self->unk_194--;
    }

    if (self->actor.bgCheckFlags & 8) {
        self->unk_196 = self->actor.wallYaw;
    } else {
        self->unk_196 = self->actor.yawTowardsPlayer + 0x8000;
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, self->unk_196, 0x38E);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x200);
    self->actor.shape.rot.z = sinf(self->unk_194 * (M_PI / 5)) * 5120.0f;

    if (self->unk_194 == 0) {
        self->actor.shape.rot.z = 0;
        self->collider.base.acFlags |= AC_ON;
        func_80B32384(self);
    }
}

void func_80B332B4(EnWeiyer* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, -0x4000, 0x400);
    self->actor.shape.rot.z += 0x1000;

    if (self->unk_194 != 0) {
        self->unk_194--;
    }

    if ((self->unk_194 == 0) || (self->actor.bgCheckFlags & 0x10)) {
        func_80B327B0(self);
    }
}

void func_80B33338(EnWeiyer* self, GlobalContext* globalCtx) {
    self->actor.shape.shadowAlpha = CLAMP_MIN((s16)(self->actor.shape.shadowAlpha - 5), 0);
    self->actor.world.pos.y -= 2.0f;

    if (self->actor.shape.shadowAlpha == 0) {
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xE0);
        Actor_Kill(&self->actor);
    }
}

void func_80B333B8(EnWeiyer* self, GlobalContext* globalCtx) {
    if (self->unk_194 != 0) {
        self->unk_194--;
    }

    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x200);
    Math_ScaledStepToS(&self->actor.shape.rot.z, 0, 0x200);
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.home.pos.y < self->actor.floorHeight) {
        if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_FLUTTER);
        }

        if (self->actor.bgCheckFlags & 2) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        }
    }

    if (self->unk_194 == 0) {
        self->actor.gravity = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->collider.dim.height = sCylinderInit.dim.height;
        func_80B32384(self);
    }
}

void func_80B3349C(EnWeiyer* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 phi_a1;
    s32 phi_a0;

    SkelAnime_Update(&self->skelAnime);

    phi_a0 = ((self->actor.home.pos.y + 20.0f) <= player->actor.world.pos.y);

    if (self->unk_194 == -1) {
        if (phi_a0 || (self->collider.base.atFlags & AT_HIT)) {
            func_80B32538(self);
        } else if (self->actor.yDistToWater < 0.0f) {
            self->unk_194 = 10;
            EffectSsGSplash_Spawn(globalCtx, &self->actor.world.pos, NULL, NULL, 1, 400);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_JUMP);
        }
    } else {
        if (phi_a0 || (self->collider.base.atFlags & AT_HIT)) {
            self->unk_194 = 0;
        } else if (self->unk_194 != 0) {
            self->unk_194--;
        }

        if (self->unk_194 == 0) {
            phi_a1 = 0x1800;
        } else {
            phi_a1 = func_80B32DEC(self, globalCtx);
            phi_a1 = CLAMP_MIN(phi_a1, 0);
        }

        if (self->actor.shape.rot.x < phi_a1) {
            Math_ScaledStepToS(&self->actor.shape.rot.x, phi_a1, 0x400);
        }

        if (self->actor.bgCheckFlags & 1) {
            func_80B32434(self);
        } else if ((self->actor.bgCheckFlags & 0x20) && (self->actor.shape.rot.x > 0)) {
            EffectSsGSplash_Spawn(globalCtx, &self->actor.world.pos, NULL, NULL, 1, 400);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_OCTAROCK_SINK);
            func_80B32538(self);
        } else {
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 8, 0x100, 0x80);
        }
    }
}

void func_80B3368C(EnWeiyer* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);

        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (self->actor.colChkInfo.damageEffect == 1) {
                if (self->actionFunc != func_80B333B8) {
                    func_80B32660(self);
                }
            } else if (Actor_ApplyDamage(&self->actor) == 0) {
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_DEAD);
                self->actor.flags &= ~1;
                func_80B32724(self);
            } else {
                func_80B325A0(self);
            }
        }
    }
}

void EnWeiyer_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnWeiyer* self = THIS;
    s32 pad;

    self->actor.home.pos.y = self->actor.yDistToWater + self->actor.world.pos.y - 5.0f;
    func_80B3368C(self, globalCtx);
    self->actionFunc(self, globalCtx);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.world.rot.x = -self->actor.shape.rot.x;

    if ((self->actor.world.rot.x == 0) || (self->actionFunc == func_80B333B8)) {
        Actor_MoveForward(&self->actor);
    } else {
        func_8002D97C(&self->actor);
    }

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 30.0f, 45.0f, 7);
    Actor_SetFocus(&self->actor, 0.0f);

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~(AT_ON | AT_HIT);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_EIER_ATTACK);
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);

    if (self->collider.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if (self->collider.base.acFlags & AT_ON) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

s32 EnWeiyer_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                              Gfx** gfx) {
    if (limbIndex == 1) {
        pos->z += 2000.0f;
    }

    return 0;
}

void EnWeiyer_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnWeiyer* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_weiyer.c", 1193);

    if (self->actionFunc != func_80B33338) {
        func_80093D18(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 0x08, &D_80116280[2]);
        gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
        POLY_OPA_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                       EnWeiyer_OverrideLimbDraw, NULL, &self->actor, POLY_OPA_DISP);
    } else {
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08, &D_80116280[0]);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, self->actor.shape.shadowAlpha);
        POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                       EnWeiyer_OverrideLimbDraw, NULL, &self->actor, POLY_XLU_DISP);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_weiyer.c", 1240);
}
