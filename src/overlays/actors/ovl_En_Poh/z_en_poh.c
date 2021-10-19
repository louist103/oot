/*
 * File: z_en_poh.c
 * Overlay: ovl_En_Poh
 * Description: Graveyard Poe
 */

#include "z_en_poh.h"
#include "objects/object_poh/object_poh.h"
#include "objects/object_po_composer/object_po_composer.h"

#define FLAGS 0x00001015

#define THIS ((EnPoh*)thisx)

void EnPoh_Init(Actor* thisx, GlobalContext* globalCtx);
void EnPoh_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnPoh_Update(Actor* thisx, GlobalContext* globalCtx);

void EnPoh_UpdateLiving(Actor* thisx, GlobalContext* globalCtx);
void EnPoh_UpdateDead(Actor* thisx, GlobalContext* globalCtx);
void EnPoh_DrawRegular(Actor* thisx, GlobalContext* globalCtx);
void EnPoh_DrawComposer(Actor* thisx, GlobalContext* globalCtx);
void EnPoh_DrawSoul(Actor* thisx, GlobalContext* globalCtx);

void func_80ADEAC4(EnPoh* self, GlobalContext* globalCtx);
void EnPoh_Idle(EnPoh* self, GlobalContext* globalCtx);
void func_80ADEC9C(EnPoh* self, GlobalContext* globalCtx);
void EnPoh_Attack(EnPoh* self, GlobalContext* globalCtx);
void func_80ADEECC(EnPoh* self, GlobalContext* globalCtx);
void func_80ADF894(EnPoh* self, GlobalContext* globalCtx);
void EnPoh_ComposerAppear(EnPoh* self, GlobalContext* globalCtx);
void func_80ADEF38(EnPoh* self, GlobalContext* globalCtx);
void func_80ADF15C(EnPoh* self, GlobalContext* globalCtx);
void func_80ADF574(EnPoh* self, GlobalContext* globalCtx);
void func_80ADF5E0(EnPoh* self, GlobalContext* globalCtx);
void EnPoh_Disappear(EnPoh* self, GlobalContext* globalCtx);
void EnPoh_Appear(EnPoh* self, GlobalContext* globalCtx);
void EnPoh_Death(EnPoh* self, GlobalContext* globalCtx);
void func_80ADFE28(EnPoh* self, GlobalContext* globalCtx);
void func_80ADFE80(EnPoh* self, GlobalContext* globalCtx);
void func_80AE009C(EnPoh* self, GlobalContext* globalCtx);
void EnPoh_TalkRegular(EnPoh* self, GlobalContext* globalCtx);
void EnPoh_TalkComposer(EnPoh* self, GlobalContext* globalCtx);

static s16 D_80AE1A50 = 0;

const ActorInit En_Poh_InitVars = {
    ACTOR_EN_POH,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnPoh),
    (ActorFunc)EnPoh_Init,
    (ActorFunc)EnPoh_Destroy,
    (ActorFunc)EnPoh_Update,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT3,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 40, 20, { 0, 0, 0 } },
};

static ColliderJntSphElementInit D_80AE1AA0[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 18, { { 0, 1400, 0 }, 10 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    D_80AE1AA0,
};

static CollisionCheckInfoInit sColChkInfoInit = { 4, 25, 50, 40 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(1, 0x1),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(2, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
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

static EnPohInfo sPoeInfo[2] = {
    {
        { 255, 170, 255 },
        { 100, 0, 150 },
        18,
        5,
        248,
        &gPoeDisappearAnim,
        &gPoeFloatAnim,
        &gPoeDamagedAnim,
        &gPoeFleeAnim,
        gPoeLanternDL,
        gPoeBurnDL,
        gPoeSoulDL,
    },
    {
        { 255, 255, 170 },
        { 0, 150, 0 },
        9,
        1,
        244,
        &gPoeComposerDisappearAnim,
        &gPoeComposerFloatAnim,
        &gPoeComposerDamagedAnim,
        &gPoeComposerFleeAnim,
        gPoeComposerLanternDL,
        gPoeComposerBurnDL,
        gPoeComposerSoulDL,
    },
};

static Color_RGBA8 D_80AE1B4C = { 75, 20, 25, 255 };
static Color_RGBA8 D_80AE1B50 = { 80, 110, 90, 255 };
static Color_RGBA8 D_80AE1B54 = { 90, 85, 50, 255 };
static Color_RGBA8 D_80AE1B58 = { 100, 90, 100, 255 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 3200, ICHAIN_STOP),
};

static Vec3f D_80AE1B60 = { 0.0f, 3.0f, 0.0f };
static Vec3f D_80AE1B6C = { 0.0f, 0.0f, 0.0f };

void EnPoh_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnItem00* collectible;
    EnPoh* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    Collider_InitJntSph(globalCtx, &self->colliderSph);
    Collider_SetJntSph(globalCtx, &self->colliderSph, &self->actor, &sJntSphInit, &self->colliderSphItem);
    self->colliderSph.elements[0].dim.worldSphere.radius = 0;
    self->colliderSph.elements[0].dim.worldSphere.center.x = self->actor.world.pos.x;
    self->colliderSph.elements[0].dim.worldSphere.center.y = self->actor.world.pos.y;
    self->colliderSph.elements[0].dim.worldSphere.center.z = self->actor.world.pos.z;
    Collider_InitCylinder(globalCtx, &self->colliderCyl);
    Collider_SetCylinder(globalCtx, &self->colliderCyl, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    self->unk_194 = 0;
    self->unk_195 = 32;
    self->visibilityTimer = Rand_S16Offset(700, 300);
    self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);
    Lights_PointGlowSetInfo(&self->lightInfo, self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z,
                            255, 255, 255, 0);
    if (self->actor.params >= 4) {
        self->actor.params = EN_POH_NORMAL;
    }
    if (self->actor.params == EN_POH_RUPEE) {
        D_80AE1A50++;
        if (D_80AE1A50 >= 3) {
            Actor_Kill(&self->actor);
        } else {
            collectible = Item_DropCollectible(globalCtx, &self->actor.world.pos, 0x4000 | ITEM00_RUPEE_BLUE);
            if (collectible != NULL) {
                collectible->actor.speedXZ = 0.0f;
            }
        }
    } else if (self->actor.params == EN_POH_FLAT) {
        if (Flags_GetSwitch(globalCtx, 0x28) || Flags_GetSwitch(globalCtx, 0x9)) {
            Actor_Kill(&self->actor);
        } else {
            Flags_SetSwitch(globalCtx, 0x28);
        }
    } else if (self->actor.params == EN_POH_SHARP) {
        if (Flags_GetSwitch(globalCtx, 0x29) || Flags_GetSwitch(globalCtx, 0x9)) {
            Actor_Kill(&self->actor);
        } else {
            Flags_SetSwitch(globalCtx, 0x29);
        }
    }
    if (self->actor.params < EN_POH_SHARP) {
        self->objectIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_POH);
        self->infoIdx = EN_POH_INFO_NORMAL;
        self->actor.naviEnemyId = 0x44;
    } else {
        self->objectIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_PO_COMPOSER);
        self->infoIdx = EN_POH_INFO_COMPOSER;
        self->actor.naviEnemyId = 0x43;
    }
    self->info = &sPoeInfo[self->infoIdx];
    if (self->objectIdx < 0) {
        Actor_Kill(&self->actor);
    }
}

void EnPoh_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnPoh* self = THIS;

    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
    Collider_DestroyJntSph(globalCtx, &self->colliderSph);
    Collider_DestroyCylinder(globalCtx, &self->colliderCyl);
    if (self->actor.params == EN_POH_RUPEE) {
        D_80AE1A50--;
    }
}

void func_80ADE114(EnPoh* self) {
    Animation_PlayLoop(&self->skelAnime, self->info->idleAnim);
    self->unk_198 = Rand_S16Offset(2, 3);
    self->actionFunc = func_80ADEAC4;
    self->actor.speedXZ = 0.0f;
}

void EnPoh_SetupIdle(EnPoh* self) {
    Animation_PlayLoop(&self->skelAnime, self->info->idleAnim2);
    self->unk_198 = Rand_S16Offset(15, 3);
    self->actionFunc = EnPoh_Idle;
}

void func_80ADE1BC(EnPoh* self) {
    Animation_PlayLoop(&self->skelAnime, self->info->idleAnim2);
    self->actionFunc = func_80ADEC9C;
    self->unk_198 = 0;
    self->actor.speedXZ = 2.0f;
}

void EnPoh_SetupAttack(EnPoh* self) {
    if (self->infoIdx == EN_POH_INFO_NORMAL) {
        Animation_MorphToLoop(&self->skelAnime, &gPoeAttackAnim, -6.0f);
    } else {
        Animation_PlayLoop(&self->skelAnime, &gPoeComposerAttackAnim);
    }
    self->unk_198 = 12;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
    self->actionFunc = EnPoh_Attack;
}

void func_80ADE28C(EnPoh* self) {
    if (self->infoIdx == EN_POH_INFO_NORMAL) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gPoeDamagedAnim, -6.0f);
    } else {
        Animation_PlayOnce(&self->skelAnime, &gPoeComposerDamagedAnim);
    }
    if (self->colliderCyl.info.acHitInfo->toucher.dmgFlags & 0x0001F824) {
        self->actor.world.rot.y = self->colliderCyl.base.ac->world.rot.y;
    } else {
        self->actor.world.rot.y = Actor_WorldYawTowardActor(&self->actor, self->colliderCyl.base.ac) + 0x8000;
    }
    self->colliderCyl.base.acFlags &= ~AC_ON;
    self->actor.speedXZ = 5.0f;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0x10);
    self->actionFunc = func_80ADEECC;
}

void func_80ADE368(EnPoh* self) {
    Animation_MorphToLoop(&self->skelAnime, self->info->fleeAnim, -5.0f);
    self->actor.speedXZ = 5.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
    self->colliderCyl.base.acFlags |= AC_ON;
    self->unk_198 = 200;
    self->actionFunc = func_80ADF894;
}

void EnPoh_SetupInitialAction(EnPoh* self) {
    self->lightColor.a = 0;
    self->actor.flags &= ~1;
    if (self->infoIdx == EN_POH_INFO_NORMAL) {
        Animation_PlayOnceSetSpeed(&self->skelAnime, &gPoeAppearAnim, 0.0f);
        self->actionFunc = func_80ADEF38;
    } else {
        Animation_PlayOnceSetSpeed(&self->skelAnime, &gPoeComposerAppearAnim, 1.0f);
        self->actor.world.pos.y = self->actor.home.pos.y + 20.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_APPEAR);
        self->actionFunc = EnPoh_ComposerAppear;
    }
}

void func_80ADE48C(EnPoh* self) {
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->unk_198 = 0;
    self->actor.naviEnemyId = 0xFF;
    self->actor.flags &= ~1;
    self->actionFunc = func_80ADF15C;
}

void func_80ADE4C8(EnPoh* self) {
    Animation_PlayOnce(&self->skelAnime, self->info->idleAnim2);
    self->actionFunc = func_80ADF574;
    self->actor.speedXZ = -5.0f;
}

void func_80ADE514(EnPoh* self) {
    Animation_PlayLoop(&self->skelAnime, self->info->idleAnim);
    self->unk_19C = self->actor.world.rot.y + 0x8000;
    self->actionFunc = func_80ADF5E0;
    self->actor.speedXZ = 0.0f;
}

void EnPoh_SetupDisappear(EnPoh* self) {
    self->unk_194 = 32;
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DISAPPEAR);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
    self->actionFunc = EnPoh_Disappear;
}

void EnPoh_SetupAppear(EnPoh* self) {
    self->unk_194 = 0;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_APPEAR);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
    self->actionFunc = EnPoh_Appear;
}

void EnPoh_SetupDeath(EnPoh* self, GlobalContext* globalCtx) {
    self->actor.update = EnPoh_UpdateDead;
    self->actor.draw = EnPoh_DrawSoul;
    self->actor.shape.shadowDraw = NULL;
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.flags |= 0x10;
    self->actor.gravity = -1.0f;
    self->actor.shape.yOffset = 1500.0f;
    self->actor.world.pos.y -= 15.0f;
    if (self->infoIdx != EN_POH_INFO_COMPOSER) {
        self->actor.shape.rot.x = -0x8000;
    }
    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, 8);
    self->unk_198 = 60;
    self->actionFunc = EnPoh_Death;
}

void func_80ADE6D4(EnPoh* self) {
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, 0, 0, 0, 0);
    self->visibilityTimer = 0;
    self->actor.shape.rot.y = 0;
    self->lightColor.r = 0;
    self->lightColor.a = 0;
    self->actor.shape.yOffset = 0.0f;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    if (self->actor.params >= EN_POH_SHARP) {
        self->lightColor.g = 200;
        self->lightColor.b = 0;
    } else {
        self->lightColor.g = 0;
        self->lightColor.b = 200;
    }
    self->actor.scale.x = 0.0f;
    self->actor.scale.y = 0.0f;
    self->actor.shape.rot.x = 0;
    self->actor.home.pos.y = self->actor.world.pos.y;
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_METAL_BOX_BOUND);
    self->actionFunc = func_80ADFE28;
}

void EnPoh_Talk(EnPoh* self, GlobalContext* globalCtx) {
    self->actor.home.pos.y = self->actor.world.pos.y;
    Actor_SetFocus(&self->actor, -10.0f);
    self->colliderCyl.dim.radius = 13;
    self->colliderCyl.dim.height = 30;
    self->colliderCyl.dim.yShift = 0;
    self->colliderCyl.dim.pos.x = self->actor.world.pos.x;
    self->colliderCyl.dim.pos.y = self->actor.world.pos.y - 20.0f;
    self->colliderCyl.dim.pos.z = self->actor.world.pos.z;
    self->colliderCyl.base.ocFlags1 = OC1_ON | OC1_TYPE_PLAYER;
    if (self->actor.params == EN_POH_FLAT || self->actor.params == EN_POH_SHARP) {
        if (CHECK_QUEST_ITEM(QUEST_SONG_SUN)) {
            self->actor.textId = 0x5000;
        } else if (!Flags_GetSwitch(globalCtx, 0xA) && !Flags_GetSwitch(globalCtx, 0xB)) {
            self->actor.textId = 0x500F;
        } else if ((self->actor.params == EN_POH_FLAT && Flags_GetSwitch(globalCtx, 0xA)) ||
                   (self->actor.params == EN_POH_SHARP && Flags_GetSwitch(globalCtx, 0xB))) {
            self->actor.textId = 0x5013;
        } else {
            self->actor.textId = 0x5012;
        }
    } else {
        self->actor.textId = 0x5005;
    }
    self->unk_198 = 200;
    self->unk_195 = 32;
    self->actor.flags |= 1;
    self->actionFunc = func_80ADFE80;
}

void func_80ADE950(EnPoh* self, s32 arg1) {
    if (arg1) {
        func_800F8A44(&self->actor.projectedPos, NA_SE_EN_PO_BIG_CRY - SFX_FLAG);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
    }
    self->actionFunc = func_80AE009C;
}

void func_80ADE998(EnPoh* self) {
    self->actionFunc = EnPoh_TalkRegular;
    self->actor.home.pos.y = self->actor.world.pos.y - 15.0f;
}

void func_80ADE9BC(EnPoh* self) {
    self->actionFunc = EnPoh_TalkComposer;
}

void EnPoh_MoveTowardsPlayerHeight(EnPoh* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_StepToF(&self->actor.world.pos.y, player->actor.world.pos.y, 1.0f);
    self->actor.world.pos.y += 2.5f * Math_SinS(self->unk_195 * 0x800);
    if (self->unk_195 != 0) {
        self->unk_195 -= 1;
    }
    if (self->unk_195 == 0) {
        self->unk_195 = 32;
    }
}

void func_80ADEA5C(EnPoh* self) {
    if (Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) > 400.0f) {
        self->unk_19C = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
    }
    Math_ScaledStepToS(&self->actor.world.rot.y, self->unk_19C, 0x71C);
}

void func_80ADEAC4(EnPoh* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && self->unk_198 != 0) {
        self->unk_198--;
    }
    EnPoh_MoveTowardsPlayerHeight(self, globalCtx);
    if (self->actor.xzDistToPlayer < 200.0f) {
        func_80ADE1BC(self);
    } else if (self->unk_198 == 0) {
        EnPoh_SetupIdle(self);
    }
    if (self->lightColor.a == 255) {
        func_8002F974(&self->actor, NA_SE_EN_PO_FLY - SFX_FLAG);
    }
}

void EnPoh_Idle(EnPoh* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&self->actor.speedXZ, 1.0f, 0.2f);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && self->unk_198 != 0) {
        self->unk_198--;
    }
    func_80ADEA5C(self);
    EnPoh_MoveTowardsPlayerHeight(self, globalCtx);
    if (self->actor.xzDistToPlayer < 200.0f && self->unk_198 < 19) {
        func_80ADE1BC(self);
    } else if (self->unk_198 == 0) {
        if (Rand_ZeroOne() < 0.1f) {
            func_80ADE514(self);
        } else {
            func_80ADE114(self);
        }
    }
    if (self->lightColor.a == 255) {
        func_8002F974(&self->actor, NA_SE_EN_PO_FLY - SFX_FLAG);
    }
}

void func_80ADEC9C(EnPoh* self, GlobalContext* globalCtx) {
    Player* player;
    s16 facingDiff;

    player = GET_PLAYER(globalCtx);
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_198 != 0) {
        self->unk_198--;
    }
    facingDiff = self->actor.yawTowardsPlayer - player->actor.shape.rot.y;
    if (facingDiff >= 0x3001) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x3000, 0x71C);
    } else if (facingDiff < -0x3000) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer - 0x3000, 0x71C);
    } else {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0x71C);
    }
    EnPoh_MoveTowardsPlayerHeight(self, globalCtx);
    if (self->actor.xzDistToPlayer > 280.0f) {
        EnPoh_SetupIdle(self);
    } else if (self->unk_198 == 0 && self->actor.xzDistToPlayer < 140.0f &&
               !Player_IsFacingActor(&self->actor, 0x2AAA, globalCtx)) {
        EnPoh_SetupAttack(self);
    }
    if (self->lightColor.a == 255) {
        func_8002F974(&self->actor, NA_SE_EN_PO_FLY - SFX_FLAG);
    }
}

void EnPoh_Attack(EnPoh* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_KANTERA);
        if (self->unk_198 != 0) {
            self->unk_198--;
        }
    }
    EnPoh_MoveTowardsPlayerHeight(self, globalCtx);
    if (self->unk_198 >= 10) {
        Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0xE38);
    } else if (self->unk_198 == 9) {
        self->actor.speedXZ = 5.0f;
        self->skelAnime.playSpeed = 2.0f;
    } else if (self->unk_198 == 0) {
        EnPoh_SetupIdle(self);
        self->unk_198 = 23;
    }
}

void func_80ADEECC(EnPoh* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.colChkInfo.health != 0) {
            func_80ADE368(self);
        } else {
            func_80ADE48C(self);
        }
    }
}

void func_80ADEF38(EnPoh* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->lightColor.a = 255;
        self->visibilityTimer = Rand_S16Offset(700, 300);
        self->actor.flags |= 1;
        EnPoh_SetupIdle(self);
    } else if (self->skelAnime.curFrame > 10.0f) {
        self->lightColor.a = ((self->skelAnime.curFrame - 10.0f) * 0.05f) * 255.0f;
    }
    if (self->skelAnime.playSpeed < 0.5f && self->actor.xzDistToPlayer < 280.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_APPEAR);
        self->skelAnime.playSpeed = 1.0f;
    }
}

void EnPoh_ComposerAppear(EnPoh* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->lightColor.a = 255;
        self->visibilityTimer = Rand_S16Offset(700, 300);
        self->actor.flags |= 1;
        EnPoh_SetupIdle(self);
    } else {
        self->lightColor.a = CLAMP_MAX((s32)(self->skelAnime.curFrame * 25.5f), 255);
    }
}

void func_80ADF15C(EnPoh* self, GlobalContext* globalCtx) {
    Vec3f vec;
    f32 multiplier;
    f32 newScale;
    s32 pad;
    s32 pad1;

    self->unk_198++;
    if (self->unk_198 < 8) {
        if (self->unk_198 < 5) {
            vec.y = Math_SinS((self->unk_198 * 0x1000) - 0x4000) * 23.0f + (self->actor.world.pos.y + 40.0f);
            multiplier = Math_CosS((self->unk_198 * 0x1000) - 0x4000) * 23.0f;
            vec.x = Math_SinS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4800) * multiplier +
                    self->actor.world.pos.x;
            vec.z = Math_CosS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4800) * multiplier +
                    self->actor.world.pos.z;
        } else {
            vec.y = (self->actor.world.pos.y + 40.0f) + (15.0f * (self->unk_198 - 5));
            vec.x =
                Math_SinS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4800) * 23.0f + self->actor.world.pos.x;
            vec.z =
                Math_CosS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4800) * 23.0f + self->actor.world.pos.z;
        }
        EffectSsDeadDb_Spawn(globalCtx, &vec, &D_80AE1B60, &D_80AE1B6C, self->unk_198 * 10 + 80, 0, 255, 255, 255, 255,
                             0, 0, 255, 1, 9, 1);
        vec.x = (self->actor.world.pos.x + self->actor.world.pos.x) - vec.x;
        vec.z = (self->actor.world.pos.z + self->actor.world.pos.z) - vec.z;
        EffectSsDeadDb_Spawn(globalCtx, &vec, &D_80AE1B60, &D_80AE1B6C, self->unk_198 * 10 + 80, 0, 255, 255, 255, 255,
                             0, 0, 255, 1, 9, 1);
        vec.x = self->actor.world.pos.x;
        vec.z = self->actor.world.pos.z;
        EffectSsDeadDb_Spawn(globalCtx, &vec, &D_80AE1B60, &D_80AE1B6C, self->unk_198 * 10 + 80, 0, 255, 255, 255, 255,
                             0, 0, 255, 1, 9, 1);
        if (self->unk_198 == 1) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_EXTINCT);
        }
    } else if (self->unk_198 == 28) {
        EnPoh_SetupDeath(self, globalCtx);
    } else if (self->unk_198 >= 19) {
        newScale = (28 - self->unk_198) * 0.001f;
        self->actor.world.pos.y += 5.0f;
        self->actor.scale.z = newScale;
        self->actor.scale.y = newScale;
        self->actor.scale.x = newScale;
    }
    if (self->unk_198 == 18) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DEAD2);
    }
}

void func_80ADF574(EnPoh* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        EnPoh_SetupIdle(self);
        self->unk_198 = 23;
    } else {
        Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
        self->actor.shape.rot.y += 0x1000;
    }
}

void func_80ADF5E0(EnPoh* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Math_ScaledStepToS(&self->actor.world.rot.y, self->unk_19C, 1820) != 0) {
        EnPoh_SetupIdle(self);
    }
    if (self->actor.xzDistToPlayer < 200.0f) {
        func_80ADE1BC(self);
    }
    EnPoh_MoveTowardsPlayerHeight(self, globalCtx);
}

void EnPoh_Disappear(EnPoh* self, GlobalContext* globalCtx) {
    if (self->unk_194 != 0) {
        self->unk_194--;
    }
    self->actor.world.rot.y += 0x1000;
    EnPoh_MoveTowardsPlayerHeight(self, globalCtx);
    self->lightColor.a = self->unk_194 * 7.96875f;
    if (self->unk_194 == 0) {
        self->visibilityTimer = Rand_S16Offset(100, 50);
        EnPoh_SetupIdle(self);
    }
}

void EnPoh_Appear(EnPoh* self, GlobalContext* globalCtx) {
    self->unk_194++;
    self->actor.world.rot.y -= 0x1000;
    EnPoh_MoveTowardsPlayerHeight(self, globalCtx);
    self->lightColor.a = self->unk_194 * 7.96875f;
    if (self->unk_194 == 32) {
        self->visibilityTimer = Rand_S16Offset(700, 300);
        self->unk_194 = 0;
        EnPoh_SetupIdle(self);
    }
}

void func_80ADF894(EnPoh* self, GlobalContext* globalCtx) {
    f32 multiplier;

    SkelAnime_Update(&self->skelAnime);
    multiplier = Math_SinS(self->unk_195 * 0x800) * 3.0f;
    self->actor.world.pos.x -= multiplier * Math_CosS(self->actor.shape.rot.y);
    self->actor.world.pos.z += multiplier * Math_SinS(self->actor.shape.rot.y);
    Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x8000, 0x71C);
    EnPoh_MoveTowardsPlayerHeight(self, globalCtx);
    if (self->unk_198 == 0 || self->actor.xzDistToPlayer > 250.0f) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        EnPoh_SetupIdle(self);
    }
    func_8002F974(&self->actor, NA_SE_EN_PO_AWAY - SFX_FLAG);
}

void EnPoh_Death(EnPoh* self, GlobalContext* globalCtx) {
    s32 objId;

    if (self->unk_198 != 0) {
        self->unk_198--;
    }
    if (self->actor.bgCheckFlags & 1) {
        objId = (self->infoIdx == EN_POH_INFO_COMPOSER) ? OBJECT_PO_COMPOSER : OBJECT_POH;
        EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 6.0f, 0, 1, 1, 15, objId, 10,
                                 self->info->lanternDisplayList);
        func_80ADE6D4(self);
    } else if (self->unk_198 == 0) {
        Actor_Kill(&self->actor);
        return;
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 10.0f, 4);
}

void func_80ADFA90(EnPoh* self, s32 arg1) {
    f32 multiplier;

    self->lightColor.a = CLAMP(self->lightColor.a + arg1, 0, 255);
    if (arg1 < 0) {
        multiplier = self->lightColor.a * (1.0f / 255);
        self->actor.scale.x = self->actor.scale.z = 0.0056000002f * multiplier + 0.0014000001f;
        self->actor.scale.y = (0.007f - 0.007f * multiplier) + 0.007f;
    } else {
        multiplier = 1.0f;
        self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = self->lightColor.a * (0.007f / 0xFF);
        self->actor.world.pos.y = self->actor.home.pos.y + (1.0f / 17.0f) * self->lightColor.a;
    }
    self->lightColor.r = self->info->lightColor.r * multiplier;
    self->lightColor.g = self->info->lightColor.g * multiplier;
    self->lightColor.b = self->info->lightColor.b * multiplier;
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, self->info->lightColor.r, self->info->lightColor.g,
                              self->info->lightColor.b, self->lightColor.a * (200.0f / 255));
}

void func_80ADFE28(EnPoh* self, GlobalContext* globalCtx) {
    self->actor.home.pos.y += 2.0f;
    func_80ADFA90(self, 20);
    if (self->lightColor.a == 255) {
        EnPoh_Talk(self, globalCtx);
    }
}

void func_80ADFE80(EnPoh* self, GlobalContext* globalCtx) {
    if (self->unk_198 != 0) {
        self->unk_198--;
    }
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        if (self->actor.params >= EN_POH_SHARP) {
            func_80ADE9BC(self);
        } else {
            func_80ADE998(self);
        }
        return;
    }
    if (self->unk_198 == 0) {
        func_80ADE950(self, 1);
        self->actor.flags &= ~0x10000;
        return;
    }
    if (self->colliderCyl.base.ocFlags1 & OC1_HIT) {
        self->actor.flags |= 0x10000;
        func_8002F2F4(&self->actor, globalCtx);
    } else {
        self->actor.flags &= ~0x10000;
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderCyl.base);
    }
    self->actor.world.pos.y = Math_SinS(self->unk_195 * 0x800) * 5.0f + self->actor.home.pos.y;
    if (self->unk_195 != 0) {
        self->unk_195 -= 1;
    }
    if (self->unk_195 == 0) {
        self->unk_195 = 32;
    }
    self->colliderCyl.dim.pos.y = self->actor.world.pos.y - 20.0f;
    Actor_SetFocus(&self->actor, -10.0f);
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, self->info->lightColor.r, self->info->lightColor.g,
                              self->info->lightColor.b, self->lightColor.a * (200.0f / 255));
}

void func_80AE009C(EnPoh* self, GlobalContext* globalCtx) {
    func_80ADFA90(self, -13);
    if (self->lightColor.a == 0) {
        Actor_Kill(&self->actor);
    }
}

void EnPoh_TalkRegular(EnPoh* self, GlobalContext* globalCtx) {
    if (self->actor.textId != 0x5005) {
        func_80ADFA90(self, -13);
    } else {
        func_8002F974(&self->actor, NA_SE_EN_PO_BIG_CRY - SFX_FLAG);
    }
    if (func_8010BDBC(&globalCtx->msgCtx) == 4) {
        if (func_80106BC8(globalCtx) != 0) {
            func_800F8A44(&self->actor.projectedPos, NA_SE_EN_PO_BIG_CRY - SFX_FLAG);
            if (globalCtx->msgCtx.choiceIndex == 0) {
                if (Inventory_HasEmptyBottle()) {
                    self->actor.textId = 0x5008;
                    Item_Give(globalCtx, ITEM_POE);
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_BIG_GET);
                } else {
                    self->actor.textId = 0x5006;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
                }
            } else {
                self->actor.textId = 0x5007;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
            }
            func_8010B720(globalCtx, self->actor.textId);
        }
    } else if (func_8002F334(&self->actor, globalCtx) != 0) {
        func_80ADE950(self, 0);
    }
}

void EnPoh_TalkComposer(EnPoh* self, GlobalContext* globalCtx) {
    func_8002F974(&self->actor, NA_SE_EN_PO_BIG_CRY - SFX_FLAG);
    if (func_8010BDBC(&globalCtx->msgCtx) == 4) {
        if (func_80106BC8(globalCtx) != 0) {
            if (globalCtx->msgCtx.choiceIndex == 0) {
                if (!Flags_GetSwitch(globalCtx, 0xB) && !Flags_GetSwitch(globalCtx, 0xA)) {
                    self->actor.textId = 0x5010;
                } else {
                    self->actor.textId = 0x5014;
                }
                func_8010B720(globalCtx, self->actor.textId);
            } else {
                if (self->actor.params == EN_POH_SHARP) {
                    Flags_SetSwitch(globalCtx, 0xB);
                } else {
                    Flags_SetSwitch(globalCtx, 0xA);
                }
                func_80ADE950(self, 1);
            }
        }
    } else if (func_8002F334(&self->actor, globalCtx) != 0) {
        if (self->actor.textId == 0x5000) {
            Flags_SetSwitch(globalCtx, 9);
        }
        func_80ADE950(self, 1);
    }
}

void func_80AE032C(EnPoh* self, GlobalContext* globalCtx) {
    if (self->colliderCyl.base.acFlags & AC_HIT) {
        self->colliderCyl.base.acFlags &= ~AC_HIT;
        if (self->actor.colChkInfo.damageEffect != 0 || self->actor.colChkInfo.damage != 0) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DEAD);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DAMAGE);
            }
            func_80ADE28C(self);
        }
    }
}

void EnPoh_UpdateVisibility(EnPoh* self) {
    if (self->actionFunc != EnPoh_Appear && self->actionFunc != EnPoh_Disappear && self->actionFunc != func_80ADEF38 &&
        self->actionFunc != EnPoh_ComposerAppear) {
        if (self->visibilityTimer != 0) {
            self->visibilityTimer--;
        }
        if (self->lightColor.a == 255) {
            if (self->actor.isTargeted) {
                self->unk_194++;
                self->unk_194 = CLAMP_MAX(self->unk_194, 20);
            } else {
                self->unk_194 = 0;
            }
            if ((self->unk_194 == 20 || self->visibilityTimer == 0) &&
                (self->actionFunc == func_80ADEAC4 || self->actionFunc == EnPoh_Idle ||
                 self->actionFunc == func_80ADEC9C || self->actionFunc == func_80ADF894 ||
                 self->actionFunc == func_80ADF5E0)) {
                EnPoh_SetupDisappear(self);
            }
        } else if (self->lightColor.a == 0 && self->visibilityTimer == 0 &&
                   (self->actionFunc == func_80ADEAC4 || self->actionFunc == EnPoh_Idle ||
                    self->actionFunc == func_80ADEC9C || self->actionFunc == func_80ADF5E0)) {
            EnPoh_SetupAppear(self);
        }
    }
}

void EnPoh_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnPoh* self = THIS;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->objectIdx) != 0) {
        self->actor.objBankIndex = self->objectIdx;
        self->actor.update = EnPoh_UpdateLiving;
        Actor_SetObjectDependency(globalCtx, &self->actor);
        if (self->infoIdx == EN_POH_INFO_NORMAL) {
            SkelAnime_Init(globalCtx, &self->skelAnime, &gPoeSkel, &gPoeFloatAnim, self->jointTable, self->morphTable,
                           21);
            self->actor.draw = EnPoh_DrawRegular;
        } else {
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gPoeComposerSkel, &gPoeComposerFloatAnim, self->jointTable,
                               self->morphTable, 12);
            self->actor.draw = EnPoh_DrawComposer;
            self->colliderSph.elements[0].dim.limb = 9;
            self->colliderSph.elements[0].dim.modelSphere.center.y *= -1;
            self->actor.shape.rot.y = self->actor.world.rot.y = -0x4000;
            self->colliderCyl.dim.radius = 20;
            self->colliderCyl.dim.height = 55;
            self->colliderCyl.dim.yShift = 15;
        }
        self->actor.flags &= ~0x10;
        EnPoh_SetupInitialAction(self);
    }
}

void func_80AE067C(EnPoh* self) {
    s16 temp_var;

    if (self->actionFunc == EnPoh_Attack) {
        self->lightColor.r = CLAMP_MAX((s16)(self->lightColor.r + 5), 255);
        self->lightColor.g = CLAMP_MIN((s16)(self->lightColor.g - 5), 50);
        temp_var = self->lightColor.b - 5;
        self->lightColor.b = CLAMP_MIN(temp_var, 0);
    } else if (self->actionFunc == func_80ADF894) {
        self->lightColor.r = CLAMP_MAX((s16)(self->lightColor.r + 5), 80);
        self->lightColor.g = CLAMP_MAX((s16)(self->lightColor.g + 5), 255);
        temp_var = self->lightColor.b + 5;
        self->lightColor.b = CLAMP_MAX(temp_var, 225);
    } else if (self->actionFunc == func_80ADEECC) {
        if (self->actor.colorFilterTimer & 2) {
            self->lightColor.r = 0;
            self->lightColor.g = 0;
            self->lightColor.b = 0;
        } else {
            self->lightColor.r = 80;
            self->lightColor.g = 255;
            self->lightColor.b = 225;
        }
    } else {
        self->lightColor.r = CLAMP_MAX((s16)(self->lightColor.r + 5), 255);
        self->lightColor.g = CLAMP_MAX((s16)(self->lightColor.g + 5), 255);
        if (self->lightColor.b >= 211) {
            temp_var = self->lightColor.b - 5;
            self->lightColor.b = CLAMP_MIN(temp_var, 210);
        } else {
            temp_var = self->lightColor.b + 5;
            self->lightColor.b = CLAMP_MAX(temp_var, 210);
        }
    }
}

void func_80AE089C(EnPoh* self) {
    f32 rand;

    if ((self->actionFunc == func_80ADEF38 || self->actionFunc == EnPoh_ComposerAppear) &&
        self->skelAnime.curFrame < 12.0f) {
        self->envColor.r = self->envColor.g = self->envColor.b = (s16)(self->skelAnime.curFrame * 16.66f) + 55;
        self->envColor.a = self->skelAnime.curFrame * (100.0f / 6.0f);
    } else {
        rand = Rand_ZeroOne();
        self->envColor.r = (s16)(rand * 30.0f) + 225;
        self->envColor.g = (s16)(rand * 100.0f) + 155;
        self->envColor.b = (s16)(rand * 160.0f) + 95;
        self->envColor.a = 200;
    }
}

void EnPoh_UpdateLiving(Actor* thisx, GlobalContext* globalCtx) {
    EnPoh* self = THIS;
    s32 pad;
    Vec3f vec;
    UNK_TYPE sp38;

    if (self->colliderSph.base.atFlags & AT_HIT) {
        self->colliderSph.base.atFlags &= ~AT_HIT;
        func_80ADE4C8(self);
    }
    func_80AE032C(self, globalCtx);
    EnPoh_UpdateVisibility(self);
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    if (self->actionFunc == EnPoh_Attack && self->unk_198 < 10) {
        self->actor.flags |= 0x1000000;
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderSph.base);
    }
    Collider_UpdateCylinder(&self->actor, &self->colliderCyl);
    if ((self->colliderCyl.base.acFlags & AC_ON) && self->lightColor.a == 255) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderCyl.base);
    }
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderCyl.base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderSph.base);
    Actor_SetFocus(&self->actor, 42.0f);
    if (self->actionFunc != func_80ADEECC && self->actionFunc != func_80ADF574) {
        if (self->actionFunc == func_80ADF894) {
            self->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
        } else {
            self->actor.shape.rot.y = self->actor.world.rot.y;
        }
    }
    vec.x = self->actor.world.pos.x;
    vec.y = self->actor.world.pos.y + 20.0f;
    vec.z = self->actor.world.pos.z;
    self->actor.floorHeight =
        BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->actor.floorPoly, &sp38, &self->actor, &vec);
    func_80AE089C(self);
    self->actor.shape.shadowAlpha = self->lightColor.a;
}

s32 EnPoh_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                           Gfx** gfxP) {
    EnPoh* self = THIS;

    if ((self->lightColor.a == 0 || limbIndex == self->info->unk_6) ||
        (self->actionFunc == func_80ADF15C && self->unk_198 >= 2)) {
        *dList = NULL;
    } else if (self->actor.params == EN_POH_FLAT && limbIndex == 0xA) {
        // Replace Sharp's head with Flat's
        *dList = gPoeComposerFlatHeadDL;
    }
    if (limbIndex == 0x13 && self->infoIdx == EN_POH_INFO_NORMAL) {
        gDPPipeSync((*gfxP)++);
        gDPSetEnvColor((*gfxP)++, self->lightColor.r, self->lightColor.g, self->lightColor.b, self->lightColor.a);
    }
    return false;
}

void EnPoh_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfxP) {
    EnPoh* self = THIS;

    Collider_UpdateSpheres(limbIndex, &self->colliderSph);
    if (self->actionFunc == func_80ADF15C && self->unk_198 >= 2 && limbIndex == self->info->unk_7) {
        gSPMatrix((*gfxP)++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_poh.c", 2460),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList((*gfxP)++, self->info->burnDisplayList);
    }
    if (limbIndex == self->info->unk_6) {
        if (self->actionFunc == func_80ADF15C && self->unk_198 >= 19 && 0.0f != self->actor.scale.x) {
            f32 mtxScale = 0.01f / self->actor.scale.x;
            Matrix_Scale(mtxScale, mtxScale, mtxScale, MTXMODE_APPLY);
        }
        Matrix_Get(&self->unk_368);
        if (self->actionFunc == func_80ADF15C && self->unk_198 == 27) {
            self->actor.world.pos.x = self->unk_368.xw;
            self->actor.world.pos.y = self->unk_368.yw;
            self->actor.world.pos.z = self->unk_368.zw;
        }
        Lights_PointGlowSetInfo(&self->lightInfo, self->colliderSph.elements[0].dim.worldSphere.center.x,
                                self->colliderSph.elements[0].dim.worldSphere.center.y,
                                self->colliderSph.elements[0].dim.worldSphere.center.z, self->envColor.r,
                                self->envColor.g, self->envColor.b, self->envColor.a * (200.0f / 255));
    }
}

void EnPoh_DrawRegular(Actor* thisx, GlobalContext* globalCtx) {
    EnPoh* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_poh.c", 2629);
    func_80AE067C(self);
    func_80093D18(globalCtx->state.gfxCtx);
    func_80093D84(globalCtx->state.gfxCtx);
    if (self->lightColor.a == 255 || self->lightColor.a == 0) {
        gDPSetEnvColor(POLY_OPA_DISP++, self->lightColor.r, self->lightColor.g, self->lightColor.b, self->lightColor.a);
        gSPSegment(POLY_OPA_DISP++, 0x08, D_80116280 + 2);
        POLY_OPA_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                       EnPoh_OverrideLimbDraw, EnPoh_PostLimbDraw, &self->actor, POLY_OPA_DISP);
    } else {
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, self->lightColor.a);
        gSPSegment(POLY_XLU_DISP++, 0x08, D_80116280);
        POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                       EnPoh_OverrideLimbDraw, EnPoh_PostLimbDraw, &self->actor, POLY_XLU_DISP);
    }
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, self->envColor.r, self->envColor.g, self->envColor.b, 255);
    Matrix_Put(&self->unk_368);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_poh.c", 2676),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, self->info->lanternDisplayList);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_poh.c", 2681);
}

void EnPoh_DrawComposer(Actor* thisx, GlobalContext* globalCtx) {
    EnPoh* self = THIS;
    Color_RGBA8* sp90;
    Color_RGBA8* phi_t0;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_poh.c", 2694);
    func_80AE067C(self);
    if (self->actor.params == EN_POH_SHARP) {
        sp90 = &D_80AE1B4C;
        phi_t0 = &D_80AE1B54;
    } else {
        sp90 = &D_80AE1B50;
        phi_t0 = &D_80AE1B58;
    }
    if (self->lightColor.a == 255 || self->lightColor.a == 0) {
        func_80093D18(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 0x08,
                   Gfx_EnvColor(globalCtx->state.gfxCtx, self->lightColor.r, self->lightColor.g, self->lightColor.b,
                                self->lightColor.a));
        gSPSegment(POLY_OPA_DISP++, 0x0A,
                   Gfx_EnvColor(globalCtx->state.gfxCtx, sp90->r, sp90->g, sp90->b, self->lightColor.a));
        gSPSegment(POLY_OPA_DISP++, 0x0B,
                   Gfx_EnvColor(globalCtx->state.gfxCtx, phi_t0->r, phi_t0->g, phi_t0->b, self->lightColor.a));
        gSPSegment(POLY_OPA_DISP++, 0x0C, D_80116280 + 2);
        POLY_OPA_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           self->skelAnime.dListCount, EnPoh_OverrideLimbDraw, EnPoh_PostLimbDraw,
                                           &self->actor, POLY_OPA_DISP);
    } else {
        func_80093D18(globalCtx->state.gfxCtx);
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_EnvColor(globalCtx->state.gfxCtx, self->lightColor.r, self->lightColor.g, self->lightColor.b,
                                self->lightColor.a));
        gSPSegment(POLY_XLU_DISP++, 0x0A,
                   Gfx_EnvColor(globalCtx->state.gfxCtx, sp90->r, sp90->g, sp90->b, self->lightColor.a));
        gSPSegment(POLY_XLU_DISP++, 0x0B,
                   Gfx_EnvColor(globalCtx->state.gfxCtx, phi_t0->r, phi_t0->g, phi_t0->b, self->lightColor.a));
        gSPSegment(POLY_XLU_DISP++, 0x0C, D_80116280);
        POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           self->skelAnime.dListCount, EnPoh_OverrideLimbDraw, EnPoh_PostLimbDraw,
                                           &self->actor, POLY_XLU_DISP);
    }
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, self->envColor.r, self->envColor.g, self->envColor.b, 255);
    Matrix_Put(&self->unk_368);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_poh.c", 2787),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, self->info->lanternDisplayList);
    gSPDisplayList(POLY_OPA_DISP++, gPoeComposerLanternBottomDL);
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, sp90->r, sp90->g, sp90->b, 255);
    gSPDisplayList(POLY_OPA_DISP++, gPoeComposerLanternTopDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_poh.c", 2802);
}

void EnPoh_UpdateDead(Actor* thisx, GlobalContext* globalCtx) {
    EnPoh* self = THIS;

    self->actionFunc(self, globalCtx);
    if (self->actionFunc != EnPoh_Death) {
        self->visibilityTimer++;
    }
    func_80AE089C(self);
}

void EnPoh_DrawSoul(Actor* thisx, GlobalContext* globalCtx) {
    EnPoh* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_poh.c", 2833);

    if (self->actionFunc == EnPoh_Death) {
        func_80093D18(globalCtx->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, self->envColor.r, self->envColor.g, self->envColor.b, 255);
        Lights_PointGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                self->actor.world.pos.z, self->envColor.r, self->envColor.g, self->envColor.b, 200);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_poh.c", 2854),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, self->info->lanternDisplayList);
        if (self->infoIdx == EN_POH_INFO_COMPOSER) {
            Color_RGBA8* envColor = (self->actor.params == EN_POH_SHARP) ? &D_80AE1B4C : &D_80AE1B50;
            s32 pad;
            gSPDisplayList(POLY_OPA_DISP++, gPoeComposerLanternBottomDL);
            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, envColor->r, envColor->g, envColor->b, 255);
            gSPDisplayList(POLY_OPA_DISP++, gPoeComposerLanternTopDL);
        }
    } else {
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0,
                                    (self->visibilityTimer * self->info->unk_8) % 512U, 0x20, 0x80));
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, self->info->primColor.r, self->info->primColor.g,
                        self->info->primColor.b, self->lightColor.a);
        gDPSetEnvColor(POLY_XLU_DISP++, self->lightColor.r, self->lightColor.g, self->lightColor.b, 255);
        Matrix_RotateY((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000) * 9.58738e-05f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_poh.c", 2910),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, self->info->soulDisplayList);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_poh.c", 2916);
}
