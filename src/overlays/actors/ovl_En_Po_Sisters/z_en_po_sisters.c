/*
 * File: z_en_po_sisters.c
 * Overlay: ovl_En_Po_Sisters
 * Description: Forest Temple Four Poe Sisters
 */

#include "z_en_po_sisters.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_po_sisters/object_po_sisters.h"

#define FLAGS 0x00005215

#define THIS ((EnPoSisters*)thisx)

void EnPoSisters_Init(Actor* thisx, GlobalContext* globalCtx);
void EnPoSisters_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnPoSisters_Update(Actor* thisx, GlobalContext* globalCtx);
void EnPoSisters_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80ADA094(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADA4A8(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADA530(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADA6A0(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADA7F0(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADA8C0(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADA9E8(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADAAA4(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADAC70(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADAD54(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADAE6C(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADAFC0(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADB17C(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADB2B8(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADB338(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADB9F0(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADB4B0(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADB51C(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADB770(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADBB6C(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADBBF4(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADBC88(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADBD38(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADBD8C(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADBEE8(EnPoSisters* self, GlobalContext* globalCtx);
void func_80ADBF58(EnPoSisters* self, GlobalContext* globalCtx);

void func_80AD9AA8(EnPoSisters* self, GlobalContext* globalCtx);
void func_80AD9C24(EnPoSisters* self, GlobalContext* globalCtx);

void func_80AD9D44(EnPoSisters* self);

static Color_RGBA8 D_80ADD6F0[4] = {
    { 255, 170, 255, 255 },
    { 255, 200, 0, 255 },
    { 0, 170, 255, 255 },
    { 170, 255, 0, 255 },
};

static Color_RGBA8 D_80ADD700[4] = {
    { 100, 0, 255, 255 },
    { 255, 0, 0, 255 },
    { 0, 0, 255, 255 },
    { 0, 150, 0, 255 },
};

const ActorInit En_Po_Sisters_InitVars = {
    ACTOR_EN_PO_SISTERS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_PO_SISTERS,
    sizeof(EnPoSisters),
    (ActorFunc)EnPoSisters_Init,
    (ActorFunc)EnPoSisters_Destroy,
    (ActorFunc)EnPoSisters_Update,
    (ActorFunc)EnPoSisters_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0x4FC7FFEA, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 18, 60, 15, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 10, 25, 60, 40 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0xF),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(2, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xE),
    /* Master sword  */ DMG_ENTRY(2, 0xE),
    /* Giant's Knife */ DMG_ENTRY(4, 0xE),
    /* Fire arrow    */ DMG_ENTRY(2, 0x0),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xE),
    /* Giant spin    */ DMG_ENTRY(4, 0xE),
    /* Master spin   */ DMG_ENTRY(2, 0xE),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xE),
    /* Giant jump    */ DMG_ENTRY(8, 0xE),
    /* Master jump   */ DMG_ENTRY(4, 0xE),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static s32 D_80ADD784 = 0;

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 7, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 6000, ICHAIN_STOP),
};

static Vec3f sZeroVector = { 0.0f, 0.0f, 0.0f };

static s16 D_80ADD79C[4] = { 0xB000, 0xD000, 0x5000, 0x3000 };

static Vec3s D_80ADD7A4[4] = {
    { -22, 337, -1704 },
    { -431, 879, -3410 },
    { 549, 879, -3410 },
    { 1717, 515, -1340 },
};

static Vec3f D_80ADD7BC = { 120.0f, 250.0f, -1420.0f };

static Gfx* D_80ADD7C8[4] = {
    gPoeSistersMegBodyDL,
    gPoeSistersJoelleBodyDL,
    gPoeSistersBethBodyDL,
    gPoeSistersAmyBodyDL,
};

static Gfx* D_80ADD7D8[4] = {
    gPoeSistersMegFaceDL,
    gPoeSistersJoelleFaceDL,
    gPoeSistersBethFaceDL,
    gPoSistersAmyFaceDL,
};

static Color_RGBA8 D_80ADD7E8[4] = {
    { 80, 0, 100, 0 },
    { 80, 15, 0, 0 },
    { 0, 70, 50, 0 },
    { 70, 70, 0, 0 },
};

static Vec3f D_80ADD7F8 = { 1000.0f, -1700.0f, 0.0f };

void EnPoSisters_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnPoSisters* self = THIS;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 50.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gPoeSistersSkel, &gPoeSistersSwayAnim, self->jointTable,
                   self->morphTable, 12);
    self->unk_22E.r = 255;
    self->unk_22E.g = 255;
    self->unk_22E.b = 210;
    self->unk_22E.a = 255;
    self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);
    Lights_PointGlowSetInfo(&self->lightInfo, self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, 0,
                            0, 0, 0);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    self->unk_194 = (thisx->params >> 8) & 3;
    self->actor.naviEnemyId = self->unk_194 + 0x50;
    if (1) {}
    self->unk_195 = (thisx->params >> 0xA) & 3;
    self->unk_196 = 32;
    self->unk_197 = 20;
    self->unk_198 = 1;
    self->unk_199 = 32;
    self->unk_294 = 110.0f;
    self->actor.flags &= ~1;
    if (self->actor.params & 0x1000) {
        func_80ADA094(self, globalCtx);
    } else if (self->unk_194 == 0) {
        if (self->unk_195 == 0) {
            self->collider.base.ocFlags1 = OC1_ON | OC1_TYPE_PLAYER;
            func_80AD9AA8(self, globalCtx);
        } else {
            self->actor.flags &= ~0x00004200;
            self->collider.info.elemType = ELEMTYPE_UNK4;
            self->collider.info.bumper.dmgFlags |= 1;
            self->collider.base.ocFlags1 = OC1_NONE;
            func_80AD9C24(self, NULL);
        }
    } else {
        func_80AD9D44(self);
    }
    self->actor.params &= 0x3F;
}

void EnPoSisters_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnPoSisters* self = THIS;

    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
    if (self->unk_194 == 0 && self->unk_195 == 0) {
        func_800F5B58();
    }
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80AD9240(EnPoSisters* self, s32 arg1, Vec3f* arg2) {
    f32 temp_f20 = SQ(arg1) * 0.1f;
    Vec3f* vec;
    s32 i;

    for (i = 0; i < self->unk_198; i++) {
        vec = &self->unk_234[i];
        vec->x = arg2->x + Math_SinS((s16)(self->actor.shape.rot.y + (self->unk_19A * 0x800) + i * 0x2000)) * temp_f20;
        vec->z = arg2->z + Math_CosS((s16)(self->actor.shape.rot.y + (self->unk_19A * 0x800) + i * 0x2000)) * temp_f20;
        vec->y = arg2->y + arg1;
    }
}

void func_80AD9368(EnPoSisters* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeSistersSwayAnim, -3.0f);
    self->unk_19A = Rand_S16Offset(2, 3);
    self->actionFunc = func_80ADA4A8;
    self->actor.speedXZ = 0.0f;
}

void func_80AD93C4(EnPoSisters* self) {
    if (self->actionFunc != func_80ADA6A0) {
        Animation_MorphToLoop(&self->skelAnime, &gPoeSistersFloatAnim, -3.0f);
    }
    self->unk_19A = Rand_S16Offset(0xF, 3);
    self->unk_199 |= 7;
    self->actionFunc = func_80ADA530;
}

void func_80AD943C(EnPoSisters* self) {
    self->actionFunc = func_80ADA6A0;
}

void func_80AD944C(EnPoSisters* self) {
    if (self->unk_22E.a != 0) {
        self->collider.base.colType = COLTYPE_METAL;
        self->collider.base.acFlags |= AC_HARD;
    }
    Animation_MorphToLoop(&self->skelAnime, &gPoeSistersAttackAnim, -5.0f);
    self->actor.speedXZ = 0.0f;
    self->unk_19A = Animation_GetLastFrame(&gPoeSistersAttackAnim) * 3 + 3;
    self->unk_199 &= ~2;
    self->actionFunc = func_80ADA7F0;
}

void func_80AD94E0(EnPoSisters* self) {
    self->actor.speedXZ = 5.0f;
    if (self->unk_194 == 0) {
        self->collider.base.colType = COLTYPE_METAL;
        self->collider.base.acFlags |= AC_HARD;
        Animation_MorphToLoop(&self->skelAnime, &gPoeSistersAttackAnim, -5.0f);
    }
    self->unk_19A = 5;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->unk_199 |= 8;
    self->actionFunc = func_80ADA8C0;
}

void func_80AD9568(EnPoSisters* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeSistersFloatAnim, -3.0f);
    self->actor.world.rot.y = self->actor.yawTowardsPlayer + 0x8000;
    if (self->unk_194 != 0) {
        self->collider.base.colType = COLTYPE_HIT3;
        self->collider.base.acFlags &= ~AC_HARD;
    }
    self->actionFunc = func_80ADA9E8;
}

void func_80AD95D8(EnPoSisters* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gPoeSistersDamagedAnim, -3.0f);
    if (self->collider.base.ac != NULL) {
        self->actor.world.rot.y = (self->collider.info.acHitInfo->toucher.dmgFlags & 0x1F824)
                                      ? self->collider.base.ac->world.rot.y
                                      : Actor_WorldYawTowardActor(&self->actor, self->collider.base.ac) + 0x8000;
    }
    if (self->unk_194 != 0) {
        self->actor.speedXZ = 10.0f;
    }
    self->unk_199 &= ~0xB;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0x10);
    self->actionFunc = func_80ADAAA4;
}

void func_80AD96A4(EnPoSisters* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeSistersFleeAnim, -3.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
    self->unk_19A = 5;
    self->unk_199 |= 0xB;
    self->actor.speedXZ = 5.0f;
    self->actionFunc = func_80ADAC70;
}

void func_80AD9718(EnPoSisters* self) {
    Animation_Change(&self->skelAnime, &gPoeSistersAppearDisappearAnim, 1.5f, 0.0f,
                     Animation_GetLastFrame(&gPoeSistersAppearDisappearAnim), ANIMMODE_ONCE, -3.0f);
    self->actor.speedXZ = 0.0f;
    self->unk_19C = 100;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->unk_199 &= ~5;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DISAPPEAR);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH2);
    self->actionFunc = func_80ADAD54;
}

void func_80AD97C8(EnPoSisters* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 sp20;

    if (self->unk_195 == 0 || self->actionFunc != func_80ADAAA4) {
        if ((player->swordState == 0 || player->swordAnimation >= 24) &&
            player->actor.world.pos.y - player->actor.floorHeight < 1.0f) {
            Math_StepToF(&self->unk_294, 110.0f, 3.0f);
        } else {
            Math_StepToF(&self->unk_294, 170.0f, 10.0f);
        }
        sp20 = self->unk_294;
    } else if (self->unk_195 != 0) {
        sp20 = self->actor.parent->xzDistToPlayer;
    }
    self->actor.world.pos.x = (Math_SinS(self->actor.shape.rot.y + 0x8000) * sp20) + player->actor.world.pos.x;
    self->actor.world.pos.z = (Math_CosS(self->actor.shape.rot.y + 0x8000) * sp20) + player->actor.world.pos.z;
}

void func_80AD98F4(EnPoSisters* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &gPoeSistersAppearDisappearAnim, 1.5f, 0.0f,
                     Animation_GetLastFrame(&gPoeSistersAppearDisappearAnim), ANIMMODE_ONCE, -3.0f);
    if (self->unk_194 == 0) {
        self->unk_294 = 110.0f;
        func_80AD97C8(self, globalCtx);
        self->unk_22E.a = 0;
        self->actor.draw = EnPoSisters_Draw;
    } else {
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
    self->unk_19A = 15;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_APPEAR);
    self->unk_199 &= ~1;
    self->actionFunc = func_80ADAE6C;
}

void func_80AD99D4(EnPoSisters* self, GlobalContext* globalCtx) {
    self->unk_19A = 0;
    self->actor.speedXZ = 0.0f;
    self->actor.world.pos.y += 42.0f;
    self->actor.shape.yOffset = -6000.0f;
    self->actor.flags &= -2;
    self->unk_199 = 0;
    self->actionFunc = func_80ADAFC0;
    OnePointCutscene_Init(globalCtx, 3190, 999, &self->actor, MAIN_CAM);
}

void func_80AD9A54(EnPoSisters* self, GlobalContext* globalCtx) {
    self->unk_19A = 0;
    self->actor.world.pos.y = self->unk_234[0].y;
    Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x80);
    self->actionFunc = func_80ADB17C;
}

// Meg spawning fakes
void func_80AD9AA8(EnPoSisters* self, GlobalContext* globalCtx) {
    Actor* actor1 = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_PO_SISTERS, self->actor.world.pos.x,
                                self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0x400);
    Actor* actor2 = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_PO_SISTERS, self->actor.world.pos.x,
                                self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0x800);
    Actor* actor3 = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_PO_SISTERS, self->actor.world.pos.x,
                                self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0xC00);
    s32 pad;
    s32 pad1;

    if (actor1 == NULL || actor2 == NULL || actor3 == NULL) {
        if (actor1 != NULL) {
            Actor_Kill(actor1);
        }
        if (actor2 != NULL) {
            Actor_Kill(actor2);
        }
        if (actor3 != NULL) {
            Actor_Kill(actor3);
        }
        Actor_Kill(&self->actor);
    } else {
        actor3->parent = &self->actor;
        actor2->parent = &self->actor;
        actor1->parent = &self->actor;
        Animation_PlayLoop(&self->skelAnime, &gPoeSistersMegCryAnim);
        self->unk_198 = 0;
        self->unk_199 = 160;
        self->actionFunc = func_80ADB2B8;
    }
}

void func_80AD9C24(EnPoSisters* self, GlobalContext* globalCtx) {
    Vec3f vec;

    self->actor.draw = NULL;
    self->actor.flags &= -2;
    self->unk_19C = 100;
    self->unk_199 = 32;
    self->collider.base.colType = COLTYPE_HIT3;
    self->collider.base.acFlags &= ~AC_HARD;
    if (globalCtx != NULL) {
        vec.x = self->actor.world.pos.x;
        vec.y = self->actor.world.pos.y + 45.0f;
        vec.z = self->actor.world.pos.z;
        EffectSsDeadDb_Spawn(globalCtx, &vec, &sZeroVector, &sZeroVector, 150, 0, 255, 255, 255, 155, 150, 150, 150, 1,
                             9, 0);
    }
    Lights_PointSetColorAndRadius(&self->lightInfo, 0, 0, 0, 0);
    self->actionFunc = func_80ADB338;
}

void func_80AD9D44(EnPoSisters* self) {
    if (self->unk_194 == 3) {
        Animation_PlayOnce(&self->skelAnime, &gPoeSistersAppearDisappearAnim);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_APPEAR);
    } else {
        Animation_Change(&self->skelAnime, &gPoeSistersAppearDisappearAnim, 0.5f, 0.0f,
                         Animation_GetLastFrame(&gPoeSistersAppearDisappearAnim), ANIMMODE_ONCE_INTERP, 0.0f);
    }
    self->unk_22E.a = 0;
    self->unk_199 = 32;
    self->actionFunc = func_80ADB9F0;
}

void func_80AD9DF0(EnPoSisters* self, GlobalContext* globalCtx) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gPoeSistersAppearDisappearAnim, -5.0f);
    self->unk_198 = 1;
    self->unk_199 &= ~0x80;
    self->actionFunc = func_80ADB4B0;
    OnePointCutscene_Init(globalCtx, 3180, 156, &self->actor, MAIN_CAM);
}

void func_80AD9E60(EnPoSisters* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeSistersFloatAnim, -3.0f);
    self->unk_19A = Animation_GetLastFrame(&gPoeSistersFloatAnim) * 7 + 7;
    if (self->actor.parent != NULL) {
        self->actor.world.pos = self->actor.parent->world.pos;
        self->actor.shape.rot.y = self->actor.parent->shape.rot.y;
    } else {
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->unk_19A++;
    }
    if (self->unk_195 == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH2);
    }
    self->actionFunc = func_80ADB51C;
}

void func_80AD9F1C(EnPoSisters* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeSistersFloatAnim, -3.0f);
    self->unk_22E.a = 255;
    self->unk_19A = 300;
    self->unk_19C = 3;
    self->unk_199 |= 9;
    self->actor.flags |= 1;
    self->actionFunc = func_80ADB770;
}

void func_80AD9F90(EnPoSisters* self) {
    if (self->unk_194 == 1) {
        self->actor.home.pos.x = -632.0f;
        self->actor.home.pos.z = -3440.0f;
    } else {
        self->actor.home.pos.x = 752.0f;
        self->actor.home.pos.z = -3440.0f;
    }
    Animation_PlayLoop(&self->skelAnime, &gPoeSistersFloatAnim);
    self->unk_199 |= 0xA;
    self->actionFunc = func_80ADBB6C;
    self->actor.speedXZ = 5.0f;
}

void func_80ADA028(EnPoSisters* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeSistersSwayAnim, -3.0f);
    self->unk_22E.a = 255;
    self->unk_199 |= 0x15;
    self->actor.flags |= 1;
    self->actionFunc = func_80ADBBF4;
    self->actor.speedXZ = 0.0f;
}

void func_80ADA094(EnPoSisters* self, GlobalContext* globalCtx) {
    D_80ADD784 = 0;
    self->unk_22E.a = 0;
    self->unk_199 = 128;
    self->unk_19A = 50;
    self->unk_234[0] = self->actor.home.pos;
    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
    self->actionFunc = func_80ADBC88;
}

void func_80ADA10C(EnPoSisters* self) {
    s32 i;

    self->unk_198 = ARRAY_COUNT(self->unk_234);
    for (i = 0; i < ARRAY_COUNT(self->unk_234); i++) {
        self->unk_234[i] = self->unk_234[0];
    }
    self->actionFunc = func_80ADBD38;
}

void func_80ADA1B8(EnPoSisters* self) {
    Animation_Change(&self->skelAnime, &gPoeSistersAppearDisappearAnim, 0.833f, 0.0f,
                     Animation_GetLastFrame(&gPoeSistersAppearDisappearAnim), ANIMMODE_ONCE_INTERP, 0.0f);
    if (self->unk_194 == 0 || self->unk_194 == 1) {
        self->unk_19A = 40;
    } else {
        self->unk_19A = 76;
    }
    self->unk_198 = 0;
    D_80ADD784 = 0;
    self->actionFunc = func_80ADBD8C;
}

void func_80ADA25C(EnPoSisters* self) {
    Animation_PlayLoop(&self->skelAnime, &gPoeSistersSwayAnim);
    self->unk_198 = 8;
    self->unk_19A = 32;
    func_80AD9240(self, self->unk_19A, &self->actor.home.pos);
    self->actionFunc = func_80ADBEE8;
}

void func_80ADA2BC(EnPoSisters* self, GlobalContext* globalCtx) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeSistersFloatAnim, -3.0f);
    self->unk_198 = 0;
    self->unk_199 = 40;
    self->unk_19A = 90;
    self->unk_196 = 32;
    self->actor.world.rot.y = D_80ADD79C[self->unk_194];
    self->actor.home.pos.y = self->actor.world.pos.y;
    if (self->unk_194 == 0) {
        Flags_SetSwitch(globalCtx, 0x1B);
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_FLAME_IGNITION);
    self->actionFunc = func_80ADBF58;
}

void func_80ADA35C(EnPoSisters* self, GlobalContext* globalCtx) {
    f32 targetY;
    Player* player = GET_PLAYER(globalCtx);

    if (self->actionFunc == func_80ADBF58) {
        targetY = self->actor.home.pos.y;
    } else if (self->unk_194 == 0 || self->unk_194 == 3) {
        targetY = player->actor.world.pos.y + 5.0f;
    } else {
        targetY = 832.0f;
    }
    Math_ApproachF(&self->actor.world.pos.y, targetY, 0.5f, 3.0f);
    if (!self->unk_196) {
        self->unk_196 = 32;
    }
    if (self->unk_196 != 0) {
        self->unk_196--;
    }
    self->actor.world.pos.y += (2.0f + 0.5f * Rand_ZeroOne()) * Math_SinS(self->unk_196 * 0x800);
    if (self->unk_22E.a == 255 && self->actionFunc != func_80ADA8C0 && self->actionFunc != func_80ADA7F0) {
        if (self->actionFunc == func_80ADAC70) {
            func_8002F974(&self->actor, NA_SE_EN_PO_AWAY - SFX_FLAG);
        } else {
            func_8002F974(&self->actor, NA_SE_EN_PO_FLY - SFX_FLAG);
        }
    }
}

void func_80ADA4A8(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && self->unk_19A != 0) {
        self->unk_19A--;
    }
    if (self->unk_19A == 0 || self->actor.xzDistToPlayer < 200.0f) {
        func_80AD93C4(self);
    }
}

void func_80ADA530(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&self->actor.speedXZ, 1.0f, 0.2f);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && self->unk_19A != 0) {
        self->unk_19A--;
    }
    if (self->actor.xzDistToPlayer < 200.0f && fabsf(self->actor.yDistToPlayer + 5.0f) < 30.0f) {
        func_80AD943C(self);
    } else if (self->unk_19A == 0 && Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f) != 0) {
        func_80AD9368(self);
    }
    if (self->actor.bgCheckFlags & 8) {
        Math_ScaledStepToS(&self->actor.world.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos),
                           0x71C);
    } else if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) > 300.0f) {
        Math_ScaledStepToS(&self->actor.world.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos),
                           0x71C);
    }
}

void func_80ADA6A0(EnPoSisters* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 temp_v0;

    SkelAnime_Update(&self->skelAnime);
    temp_v0 = self->actor.yawTowardsPlayer - player->actor.shape.rot.y;
    Math_StepToF(&self->actor.speedXZ, 2.0f, 0.2f);
    if (temp_v0 > 0x3000) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x3000, 0x71C);
    } else if (temp_v0 < -0x3000) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer - 0x3000, 0x71C);
    } else {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0x71C);
    }
    if (self->actor.xzDistToPlayer < 160.0f && fabsf(self->actor.yDistToPlayer + 5.0f) < 30.0f) {
        func_80AD944C(self);
    } else if (self->actor.xzDistToPlayer > 240.0f) {
        func_80AD93C4(self);
    }
}

void func_80ADA7F0(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_19A != 0) {
        self->unk_19A--;
    }
    self->actor.shape.rot.y += 384.0f * ((self->skelAnime.endFrame + 1.0f) * 3.0f - self->unk_19A);
    if (self->unk_19A == 18 || self->unk_19A == 7) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_ROLL);
    }
    if (self->unk_19A == 0) {
        func_80AD94E0(self);
    }
}

void func_80ADA8C0(EnPoSisters* self, GlobalContext* globalCtx) {
    s32 pad;

    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && self->unk_19A != 0) {
        self->unk_19A--;
    }
    self->actor.shape.rot.y += (384.0f * self->skelAnime.endFrame) * 3.0f;
    if (self->unk_19A == 0 && ABS((s16)(self->actor.shape.rot.y - self->actor.world.rot.y)) < 0x1000) {
        if (self->unk_194 != 0) {
            self->collider.base.colType = COLTYPE_HIT3;
            self->collider.base.acFlags &= ~AC_HARD;
            func_80AD93C4(self);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH2);
            func_80AD9C24(self, globalCtx);
        }
    }
    if (Animation_OnFrame(&self->skelAnime, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_ROLL);
    }
}

void func_80ADA9E8(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    self->actor.shape.rot.y -= (self->actor.speedXZ * 10.0f) * 128.0f;
    if (Math_StepToF(&self->actor.speedXZ, 0.0f, 0.1f) != 0) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (self->unk_194 != 0) {
            func_80AD93C4(self);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH2);
            func_80AD9C24(self, globalCtx);
        }
    }
}

void func_80ADAAA4(EnPoSisters* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) && !(self->actor.flags & 0x8000)) {
        if (self->actor.colChkInfo.health != 0) {
            if (self->unk_194 != 0) {
                func_80AD96A4(self);
            } else if (self->unk_195 != 0) {
                func_80AD9C24(self, NULL);
            } else {
                func_80AD9C24(self, globalCtx);
            }
        } else {
            func_80AD99D4(self, globalCtx);
        }
    }
    if (self->unk_195 != 0) {
        Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.parent->shape.rot.y,
                           (self->unk_195 == 2) ? 0x800 : 0x400);
        self->unk_22E.a = ((self->skelAnime.endFrame - self->skelAnime.curFrame) * 255.0f) / self->skelAnime.endFrame;
        self->actor.world.pos.y = self->actor.parent->world.pos.y;
        func_80AD97C8(self, globalCtx);
    } else if (self->unk_194 != 0) {
        Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    }
}

void func_80ADAC70(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x8000, 1820);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && self->unk_19A != 0) {
        self->unk_19A--;
    }
    if (self->actor.bgCheckFlags & 8) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        self->unk_199 |= 2;
        func_80AD9718(self);
    } else if (self->unk_19A == 0 && 240.0f < self->actor.xzDistToPlayer) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        func_80AD93C4(self);
    }
}

void func_80ADAD54(EnPoSisters* self, GlobalContext* globalCtx) {
    s32 endFrame;

    if (SkelAnime_Update(&self->skelAnime)) {
        self->unk_22E.a = 0;
        self->collider.info.bumper.dmgFlags = 0x00060001;
        func_80AD93C4(self);
    } else {
        endFrame = self->skelAnime.endFrame;
        self->unk_22E.a = ((endFrame - self->skelAnime.curFrame) * 255.0f) / endFrame;
    }
}

void func_80ADAE6C(EnPoSisters* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->unk_22E.a = 255;
        if (self->unk_194 != 0) {
            self->unk_199 |= 1;
            self->collider.info.bumper.dmgFlags = 0x4FC7FFEA;
            if (self->unk_19A != 0) {
                self->unk_19A--;
            }
            if (self->unk_19A == 0) {
                self->unk_197 = 20;
                func_80AD93C4(self);
            }
        } else {
            func_80AD9F1C(self);
        }
    } else {
        self->unk_22E.a = (self->skelAnime.curFrame * 255.0f) / self->skelAnime.endFrame;
        if (self->unk_194 == 0) {
            func_80AD97C8(self, globalCtx);
        }
    }
}

void func_80ADAFC0(EnPoSisters* self, GlobalContext* globalCtx) {
    s32 i;

    self->unk_19A++;
    self->unk_198 = CLAMP_MAX(self->unk_198 + 1, 8);
    for (i = self->unk_198 - 1; i > 0; i--) {
        self->unk_234[i] = self->unk_234[i - 1];
    }
    self->unk_234[0].x =
        (Math_SinS((self->actor.shape.rot.y + self->unk_19A * 0x3000) - 0x4000) * (3000.0f * self->actor.scale.x)) +
        self->actor.world.pos.x;
    self->unk_234[0].z =
        (Math_CosS((self->actor.shape.rot.y + self->unk_19A * 0x3000) - 0x4000) * (3000.0f * self->actor.scale.x)) +
        self->actor.world.pos.z;
    if (self->unk_19A < 8) {
        self->unk_234[0].y = self->unk_234[1].y - 9.0f;
    } else {
        self->unk_234[0].y = self->unk_234[1].y + 2.0f;
        if (self->unk_19A >= 16) {
            if (Math_StepToF(&self->actor.scale.x, 0.0f, 0.001f) != 0) {
                func_80AD9A54(self, globalCtx);
            }
            self->actor.scale.z = self->actor.scale.x;
            self->actor.scale.y = self->actor.scale.x;
        }
    }
    if (self->unk_19A == 16) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DEAD2);
    }
}

void func_80ADB17C(EnPoSisters* self, GlobalContext* globalCtx) {
    self->unk_19A++;
    if (self->unk_19A == 64) {
        Flags_SetSwitch(globalCtx, self->actor.params);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 30, NA_SE_EV_FLAME_IGNITION);
        if (self->unk_194 == 0) {
            Flags_UnsetSwitch(globalCtx, 0x1B);
        }
        globalCtx->envCtx.unk_BF = 0xFF;
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        Actor_Kill(&self->actor);
    } else if (self->unk_19A < 32) {
        func_80AD9240(self, self->unk_19A, &self->actor.world.pos);
    } else {
        func_80AD9240(self, 64 - self->unk_19A, &self->actor.world.pos);
    }
    if (self->unk_19A == 32) {
        self->actor.world.pos.x = D_80ADD7A4[self->unk_194].x;
        self->actor.world.pos.y = D_80ADD7A4[self->unk_194].y;
        self->actor.world.pos.z = D_80ADD7A4[self->unk_194].z;
    }
}

void func_80ADB2B8(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->actor.xzDistToPlayer < 130.0f) {
        func_80AD9DF0(self, globalCtx);
    }
    if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_CRY);
    }
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
}

void func_80ADB338(EnPoSisters* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    EnPoSisters* realMeg = (EnPoSisters*)self->actor.parent;

    if (self->unk_195 == 0) {
        if (Actor_WorldDistXZToPoint(&player->actor, &self->actor.home.pos) < 600.0f) {
            if (self->unk_19C != 0) {
                self->unk_19C--;
            }
        } else {
            self->unk_19C = 100;
        }
        if (self->unk_19C == 0) {
            self->actor.shape.rot.y = (s32)(4.0f * Rand_ZeroOne()) * 0x4000 + self->actor.yawTowardsPlayer;
            self->actor.world.pos.y = player->actor.world.pos.y + 5.0f;
            func_80AD98F4(self, globalCtx);
        }
    } else {
        if (realMeg->actionFunc == func_80ADB51C) {
            self->actor.draw = EnPoSisters_Draw;
            func_80AD9E60(self);
        } else if (realMeg->actionFunc == func_80ADAE6C) {
            self->actor.shape.rot.y = self->actor.parent->shape.rot.y + self->unk_195 * 0x4000;
            self->actor.world.pos.y = player->actor.world.pos.y + 5.0f;
            func_80AD98F4(self, globalCtx);
        } else if (realMeg->actionFunc == func_80ADAFC0) {
            Actor_Kill(&self->actor);
        }
    }
}

void func_80ADB4B0(EnPoSisters* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        func_80AD9E60(self);
    }
    func_80AD97C8(self, globalCtx);
    self->actor.world.pos.y += 1.0f;
    Actor_SetFocus(&self->actor, 40.0f);
}

void func_80ADB51C(EnPoSisters* self, GlobalContext* globalCtx) {
    f32 temp_f2;
    s16 phi_v0;
    s16 phi_a2;
    u8 temp;

    SkelAnime_Update(&self->skelAnime);
    temp_f2 = self->skelAnime.endFrame * 0.5f;
    self->unk_22E.a = (fabsf(temp_f2 - self->skelAnime.curFrame) * 255.0f) / temp_f2;
    if (self->unk_19A != 0) {
        self->unk_19A -= 1;
    }
    if (self->unk_19A == 0) {
        self->actor.world.rot.y = self->actor.shape.rot.y += 0x4000 * (s32)(Rand_ZeroOne() * 4.0f);
        if (self->unk_195 == 0) {
            func_800F5ACC(0x38);
        }
        func_80AD9F1C(self);
    } else {
        self->actor.world.pos.y += 0.1f;
        temp = self->unk_195;
        if (temp != 0) {
            if (self->unk_19A > 90) {
                phi_v0 = 1;
                phi_a2 = 64;
            } else if (self->unk_19A > 70) {
                phi_v0 = 0;
                phi_a2 = 64;
            } else if (self->unk_19A > 55) {
                phi_v0 = 1;
                phi_a2 = 96;
            } else if (self->unk_19A > 40) {
                phi_v0 = 0;
                phi_a2 = 96;
            } else {
                phi_v0 = 1;
                phi_a2 = 256;
            }
            if (self->unk_195 == 2) {
                phi_a2 *= 2;
            }
            Math_ScaledStepToS(&self->actor.shape.rot.y,
                               self->actor.parent->shape.rot.y + (self->unk_195 * 0x4000) * phi_v0, phi_a2);
        } else if (self->unk_19A == 70 || self->unk_19A == 40) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH2);
        }
    }
    func_80AD97C8(self, globalCtx);
    Actor_SetFocus(&self->actor, 40.0f);
}

void func_80ADB770(EnPoSisters* self, GlobalContext* globalCtx) {
    s32 temp_v0;
    s32 phi_a0;

    if (self->unk_19A != 0) {
        self->unk_19A--;
    }
    if (self->unk_19C > 0) {
        if (self->unk_19A >= 16) {
            SkelAnime_Update(&self->skelAnime);
            if (self->unk_195 == 0) {
                if (ABS((s16)(16 - self->unk_196)) < 14) {
                    self->actor.shape.rot.y +=
                        (0x580 - (self->unk_19C * 0x180)) * fabsf(Math_SinS(self->unk_196 * 0x800));
                }
                if (self->unk_19A >= 284 || self->unk_19A < 31) {
                    self->unk_199 |= 0x40;
                } else {
                    self->unk_199 &= ~0x40;
                }
            } else {
                self->actor.shape.rot.y = (s16)(self->actor.parent->shape.rot.y + (self->unk_195 * 0x4000));
            }
        }
    }
    if (self->unk_195 == 0) {
        if (self->unk_19A >= 284 || (self->unk_19A < 31 && self->unk_19A >= 16)) {
            self->unk_199 |= 0x40;
        } else {
            self->unk_199 &= ~0x40;
        }
    }
    if (Actor_WorldDistXZToPoint(&GET_PLAYER(globalCtx)->actor, &self->actor.home.pos) > 600.0f) {
        self->unk_199 &= ~0x40;
        func_80AD9C24(self, globalCtx);
    } else if (self->unk_19A == 0) {
        if (self->unk_195 == 0) {
            func_80AD94E0(self);
        } else {
            func_80AD9C24(self, globalCtx);
        }
    } else if (self->unk_195 != 0) {
        EnPoSisters* realMeg = (EnPoSisters*)self->actor.parent;
        if (realMeg->actionFunc == func_80ADAAA4) {
            func_80AD95D8(self);
        }
    } else if (self->unk_19C == 0) {
        self->unk_19C = -15;
    } else if (self->unk_19C < 0) {
        self->unk_19C++;
        if (self->unk_19C == 0) {
            func_80AD94E0(self);
        }
    }
    func_80AD97C8(self, globalCtx);
}

void func_80ADB9F0(EnPoSisters* self, GlobalContext* globalCtx) {
    f32 div;

    if (SkelAnime_Update(&self->skelAnime)) {
        self->unk_22E.a = 255;
        if (self->unk_194 == 3) {
            self->actor.flags |= 1;
            self->actor.home.pos.x = 1992.0f;
            self->actor.home.pos.z = -1440.0f;
            self->unk_199 |= 0x18;
            func_80AD93C4(self);
        } else {
            func_80AD9F90(self);
        }
    } else {
        div = self->skelAnime.curFrame / self->skelAnime.endFrame;
        self->unk_22E.a = 255.0f * div;
    }
    if (self->unk_194 != 3 && Animation_OnFrame(&self->skelAnime, 1.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_APPEAR);
    }
    Actor_SetFocus(&self->actor, 40.0f);
}

void func_80ADBB6C(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) < 10.0f) {
        func_80ADA028(self);
    } else {
        Math_ScaledStepToS(&self->actor.world.rot.y, Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos),
                           1820);
    }
}

void func_80ADBBF4(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1820);
    if (self->actor.xzDistToPlayer < 240.0f && fabsf(self->actor.yDistToPlayer + 5.0f) < 30.0f) {
        func_80AD93C4(self);
    }
}

void func_80ADBC88(EnPoSisters* self, GlobalContext* globalCtx) {
    if (D_80ADD784 != 0 || !Player_InCsMode(globalCtx)) {
        if (self->unk_19A != 0) {
            self->unk_19A--;
        }
        if (self->unk_19A == 30) {
            if (self->unk_194 == 0) {
                OnePointCutscene_Init(globalCtx, 3140, 999, NULL, MAIN_CAM);
            }
            D_80ADD784 = 1;
        }
        if (self->unk_19A == 0) {
            func_80ADA10C(self);
        }
    }
    func_8002F974(&self->actor, NA_SE_EV_TORCH - SFX_FLAG);
}

void func_80ADBD38(EnPoSisters* self, GlobalContext* globalCtx) {
    self->unk_19A++;
    func_80AD9240(self, self->unk_19A, &self->actor.home.pos);
    if (self->unk_19A == 32) {
        func_80ADA1B8(self);
    }
}

void func_80ADBD8C(EnPoSisters* self, GlobalContext* globalCtx) {
    self->unk_19A--;
    if (self->unk_19A == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_APPEAR);
        self->unk_199 &= ~0x80;
    }
    if (self->unk_19A <= 0) {
        if (SkelAnime_Update(&self->skelAnime)) {
            self->unk_22E.a = 255;
            D_80ADD784 |= (1 << self->unk_194);
        } else {
            self->unk_22E.a = (self->skelAnime.curFrame * 255.0f) / self->skelAnime.endFrame;
        }
    }
    if (D_80ADD784 == 15) {
        func_80ADA25C(self);
    }
}

void func_80ADBEE8(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_19A != 0) {
        self->unk_19A--;
    }
    func_80AD9240(self, self->unk_19A, &self->actor.home.pos);
    if (self->unk_19A == 0) {
        func_80ADA2BC(self, globalCtx);
    }
}

void func_80ADBF58(EnPoSisters* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    self->unk_19A--;
    Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.world.rot.y, 0x500);
    if (self->unk_19A == 0 && self->unk_194 == 0) {
        globalCtx->envCtx.unk_BF = 4;
    }
    if (self->unk_19A < 0) {
        Math_StepToF(&self->actor.speedXZ, 5.0f, 0.2f);
    }
    if (self->unk_19A == -70 && self->unk_194 == 1) {
        Audio_PlaySoundAtPosition(globalCtx, &D_80ADD7BC, 40, NA_SE_EN_PO_LAUGH);
    }
    if (self->unk_19A < -120) {
        Actor_Kill(&self->actor);
    }
}

void func_80ADC034(EnPoSisters* self, GlobalContext* globalCtx) {
    if (self->actor.isTargeted && self->unk_22E.a == 255) {
        if (self->unk_197 != 0) {
            self->unk_197--;
        }
    } else {
        self->unk_197 = 20;
    }
    if (self->unk_22E.a == 0) {
        if (self->unk_19C != 0) {
            self->unk_19C--;
        }
    }
    if (self->actionFunc != func_80ADA7F0 && self->actionFunc != func_80ADA8C0 && self->actionFunc != func_80ADAAA4) {
        if (self->unk_197 == 0) {
            func_80AD9718(self);
        } else if (self->unk_19C == 0 && self->unk_22E.a == 0) {
            func_80AD98F4(self, globalCtx);
        }
    }
}

void func_80ADC10C(EnPoSisters* self, GlobalContext* globalCtx) {
    Vec3f sp24;

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);
        if (self->unk_195 != 0) {
            ((EnPoSisters*)self->actor.parent)->unk_19C--;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH2);
            func_80AD9C24(self, globalCtx);
            if (Rand_ZeroOne() < 0.2f) {
                sp24.x = self->actor.world.pos.x;
                sp24.y = self->actor.world.pos.y;
                sp24.z = self->actor.world.pos.z;
                Item_DropCollectible(globalCtx, &sp24, ITEM00_ARROWS_SMALL);
            }
        } else if (self->collider.base.colType == 9 ||
                   (self->actor.colChkInfo.damageEffect == 0 && self->actor.colChkInfo.damage == 0)) {
            if (self->unk_194 == 0) {
                self->actor.freezeTimer = 0;
            }
        } else if (self->actor.colChkInfo.damageEffect == 0xF) {
            self->actor.world.rot.y = self->actor.shape.rot.y;
            self->unk_199 |= 2;
            func_80AD98F4(self, globalCtx);
        } else if (self->unk_194 == 0 && self->actor.colChkInfo.damageEffect == 0xE &&
                   self->actionFunc == func_80ADB770) {
            if (self->unk_19C == 0) {
                self->unk_19C = -45;
            }
        } else {
            if (Actor_ApplyDamage(&self->actor) != 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DAMAGE);
            } else {
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_SISTER_DEAD);
            }
            func_80AD95D8(self);
        }
    }
}

void EnPoSisters_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnPoSisters* self = THIS;
    s16 temp;

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        func_80AD9568(self);
    }
    func_80ADC10C(self, globalCtx);
    if (self->unk_199 & 4) {
        func_80ADC034(self, globalCtx);
    }
    self->actionFunc(self, globalCtx);
    if (self->unk_199 & 0x1F) {
        if (self->unk_199 & 8) {
            func_80ADA35C(self, globalCtx);
        }
        Actor_MoveForward(&self->actor);

        if (self->unk_199 & 0x10) {
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 0.0f, 5);
        } else {
            Vec3f vec;
            UNK_TYPE sp34;

            vec.x = self->actor.world.pos.x;
            vec.y = self->actor.world.pos.y + 10.0f;
            vec.z = self->actor.world.pos.z;
            self->actor.floorHeight =
                BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->actor.floorPoly, &sp34, &self->actor, &vec);
        }

        Collider_UpdateCylinder(&self->actor, &self->collider);
        if (self->actionFunc == func_80ADA8C0 || self->actionFunc == func_80ADA7F0) {
            self->unk_198++;
            self->unk_198 = CLAMP_MAX(self->unk_198, 8);
        } else if (self->actionFunc != func_80ADAFC0) {
            temp = self->unk_198 - 1;
            self->unk_198 = CLAMP_MIN(temp, 1);
        }
        if (self->actionFunc == func_80ADA8C0) {
            self->actor.flags |= 0x01000000;
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        if (self->unk_199 & 1) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        if (self->actionFunc != func_80ADB338) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        Actor_SetFocus(&self->actor, 40.0f);
        if (self->actionFunc == func_80ADAC70) {
            self->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
        } else if (self->unk_199 & 2) {
            self->actor.shape.rot.y = self->actor.world.rot.y;
        }
    }
}

void func_80ADC55C(EnPoSisters* self) {
    s16 temp_var;

    if (self->skelAnime.animation == &gPoeSistersAttackAnim) {
        self->unk_22E.r = CLAMP_MAX((s16)(self->unk_22E.r + 5), 255);
        self->unk_22E.g = CLAMP_MIN((s16)(self->unk_22E.g - 5), 50);
        temp_var = self->unk_22E.b - 5;
        self->unk_22E.b = CLAMP_MIN(temp_var, 0);
    } else if (self->skelAnime.animation == &gPoeSistersFleeAnim) {
        self->unk_22E.r = CLAMP_MAX((s16)(self->unk_22E.r + 5), 80);
        self->unk_22E.g = CLAMP_MAX((s16)(self->unk_22E.g + 5), 255);
        temp_var = self->unk_22E.b + 5;
        self->unk_22E.b = CLAMP_MAX(temp_var, 225);
    } else if (self->skelAnime.animation == &gPoeSistersDamagedAnim) {
        if (self->actor.colorFilterTimer & 2) {
            self->unk_22E.r = 0;
            self->unk_22E.g = 0;
            self->unk_22E.b = 0;
        } else {
            self->unk_22E.r = 80;
            self->unk_22E.g = 255;
            self->unk_22E.b = 225;
        }
    } else {
        self->unk_22E.r = CLAMP_MAX((s16)(self->unk_22E.r + 5), 255);
        self->unk_22E.g = CLAMP_MAX((s16)(self->unk_22E.g + 5), 255);
        if (self->unk_22E.b > 210) {
            temp_var = self->unk_22E.b - 5;
            self->unk_22E.b = CLAMP_MIN(temp_var, 210);
        } else {
            temp_var = self->unk_22E.b + 5;
            self->unk_22E.b = CLAMP_MAX(temp_var, 210);
        }
    }
}

s32 EnPoSisters_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                 void* thisx, Gfx** gfxP) {
    EnPoSisters* self = THIS;
    Color_RGBA8* color;

    if (limbIndex == 1 && (self->unk_199 & 0x40)) {
        if (self->unk_19A >= 284) {
            rot->x += (self->unk_19A * 0x1000) + 0xFFEE4000;
        } else {
            rot->x += (self->unk_19A * 0x1000) + 0xFFFF1000;
        }
    }
    if (self->unk_22E.a == 0 || limbIndex == 8 || (self->actionFunc == func_80ADAFC0 && self->unk_19A >= 8)) {
        *dList = NULL;
    } else if (limbIndex == 9) {
        *dList = D_80ADD7C8[self->unk_194];
    } else if (limbIndex == 10) {
        *dList = D_80ADD7D8[self->unk_194];
        gDPPipeSync((*gfxP)++);
        gDPSetEnvColor((*gfxP)++, self->unk_22E.r, self->unk_22E.g, self->unk_22E.b, self->unk_22E.a);
    } else if (limbIndex == 11) {
        color = &D_80ADD7E8[self->unk_194];
        gDPPipeSync((*gfxP)++);
        gDPSetEnvColor((*gfxP)++, color->r, color->g, color->b, self->unk_22E.a);
    }
    return false;
}

void EnPoSisters_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx,
                              Gfx** gfxP) {
    EnPoSisters* self = THIS;
    s32 i;
    s32 pad;

    if (self->actionFunc == func_80ADAFC0 && self->unk_19A >= 8 && limbIndex == 9) {
        gSPMatrix((*gfxP)++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_sisters.c", 2876),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList((*gfxP)++, gPoSistersBurnDL);
    }
    if (limbIndex == 8 && self->actionFunc != func_80ADB2B8) {
        if (self->unk_199 & 0x20) {
            for (i = self->unk_198 - 1; i > 0; i--) {
                self->unk_234[i] = self->unk_234[i - 1];
            }
            Matrix_MultVec3f(&D_80ADD7F8, &self->unk_234[0]);
        } else if (self->actionFunc == func_80ADBD8C) {
            Matrix_MultVec3f(&D_80ADD7F8, &self->actor.home.pos);
        }
        if (self->unk_198 > 0) {
            Color_RGBA8* color = &D_80ADD6F0[self->unk_194];
            f32 temp_f2 = Rand_ZeroOne() * 0.3f + 0.7f;

            if (self->actionFunc == func_80ADB17C || self->actionFunc == func_80ADBD38 ||
                self->actionFunc == func_80ADBEE8) {
                Lights_PointNoGlowSetInfo(&self->lightInfo, self->unk_234[0].x, self->unk_234[0].y + 15.0f,
                                          self->unk_234[0].z, color->r * temp_f2, color->g * temp_f2,
                                          color->b * temp_f2, 200);
            } else {
                Lights_PointGlowSetInfo(&self->lightInfo, self->unk_234[0].x, self->unk_234[0].y + 15.0f,
                                        self->unk_234[0].z, color->r * temp_f2, color->g * temp_f2, color->b * temp_f2,
                                        200);
            }
        } else {
            Lights_PointSetColorAndRadius(&self->lightInfo, 0, 0, 0, 0);
        }
        if (!(self->unk_199 & 0x80)) {
            Matrix_Get(&self->unk_2F8);
        }
    }
}

void EnPoSisters_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnPoSisters* self = THIS;
    u8 phi_s5;
    f32 phi_f20;
    s32 i;
    u8 spE7;
    Color_RGBA8* temp_s1 = &D_80ADD700[self->unk_194];
    Color_RGBA8* temp_s7 = &D_80ADD6F0[self->unk_194];
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_po_sisters.c", 2989);
    func_80ADC55C(self);
    func_80093D18(globalCtx->state.gfxCtx);
    func_80093D84(globalCtx->state.gfxCtx);
    if (self->unk_22E.a == 255 || self->unk_22E.a == 0) {
        gDPSetEnvColor(POLY_OPA_DISP++, self->unk_22E.r, self->unk_22E.g, self->unk_22E.b, self->unk_22E.a);
        gSPSegment(POLY_OPA_DISP++, 0x09, D_80116280 + 2);
        POLY_OPA_DISP =
            SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                           EnPoSisters_OverrideLimbDraw, EnPoSisters_PostLimbDraw, &self->actor, POLY_OPA_DISP);
    } else {
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, self->unk_22E.a);
        gSPSegment(POLY_XLU_DISP++, 0x09, D_80116280);
        POLY_XLU_DISP =
            SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                           EnPoSisters_OverrideLimbDraw, EnPoSisters_PostLimbDraw, &self->actor, POLY_XLU_DISP);
    }
    if (!(self->unk_199 & 0x80)) {
        Matrix_Put(&self->unk_2F8);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_sisters.c", 3034),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gPoSistersTorchDL);
    }
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0,
                                (globalCtx->gameplayFrames * -20) % 512, 0x20, 0x80));
    gDPSetEnvColor(POLY_XLU_DISP++, temp_s1->r, temp_s1->g, temp_s1->b, temp_s1->a);
    if (self->actionFunc == func_80ADB17C) {
        if (self->unk_19A < 32) {
            phi_s5 = ((32 - self->unk_19A) * 255) / 32;
            phi_f20 = 0.0056000003f;
        } else {
            phi_s5 = (self->unk_19A * 255 - 8160) / 32;
            phi_f20 = 0.0027f;
        }
    } else if (self->actionFunc == func_80ADBD38) {
        phi_s5 = ((32 - self->unk_19A) * 255) / 32;
        phi_f20 = 0.0027f;
    } else if (self->actionFunc == func_80ADBEE8) {
        phi_s5 = ((32 - self->unk_19A) * 255) / 32;
        phi_f20 = 0.0035f;
    } else if (self->actionFunc == func_80ADBC88) {
        //! @bug uninitialised spE7
        phi_s5 = spE7;
        phi_f20 = 0.0027f;
    } else {
        phi_s5 = spE7;
        phi_f20 = self->actor.scale.x * 0.5f;
    }
    for (i = 0; i < self->unk_198; i++) {
        if (self->actionFunc != func_80ADB17C && self->actionFunc != func_80ADBD38 &&
            self->actionFunc != func_80ADBEE8) {
            phi_s5 = -i * 31 + 248;
        }
        gDPPipeSync(POLY_XLU_DISP++);
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, temp_s7->r, temp_s7->g, temp_s7->b, phi_s5);
        Matrix_Translate(self->unk_234[i].x, self->unk_234[i].y, self->unk_234[i].z, MTXMODE_NEW);
        Matrix_RotateRPY(0, (s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000), 0, MTXMODE_APPLY);
        if (self->actionFunc == func_80ADAFC0) {
            phi_f20 = (self->unk_19A - i) * 0.025f + 0.5f;
            phi_f20 = CLAMP(phi_f20, 0.5f, 0.8f) * 0.007f;
        }
        Matrix_Scale(phi_f20, phi_f20, phi_f20, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_sisters.c", 3132),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_po_sisters.c", 3139);
}
