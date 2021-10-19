/*
 * File: z_en_rr.c
 * Overlay: ovl_En_Rr
 * Description: Like Like
 */

#include "z_en_rr.h"
#include "objects/object_rr/object_rr.h"
#include "vt.h"

#define FLAGS 0x00000435

#define THIS ((EnRr*)thisx)

#define RR_MESSAGE_SHIELD (1 << 0)
#define RR_MESSAGE_TUNIC (1 << 1)
#define RR_MOUTH 4
#define RR_BASE 0

typedef enum {
    /* 0 */ REACH_NONE,
    /* 1 */ REACH_EXTEND,
    /* 2 */ REACH_STOP,
    /* 3 */ REACH_OPEN,
    /* 4 */ REACH_GAPE,
    /* 5 */ REACH_CLOSE
} EnRrReachState;

typedef enum {
    /* 0x0 */ RR_DMG_NONE,
    /* 0x1 */ RR_DMG_STUN,
    /* 0x2 */ RR_DMG_FIRE,
    /* 0x3 */ RR_DMG_ICE,
    /* 0x4 */ RR_DMG_LIGHT_MAGIC,
    /* 0xB */ RR_DMG_LIGHT_ARROW = 11,
    /* 0xC */ RR_DMG_SHDW_ARROW,
    /* 0xD */ RR_DMG_WIND_ARROW,
    /* 0xE */ RR_DMG_SPRT_ARROW,
    /* 0xF */ RR_DMG_NORMAL
} EnRrDamageEffect;

typedef enum {
    /* 0 */ RR_DROP_RANDOM_RUPEE,
    /* 1 */ RR_DROP_MAGIC,
    /* 2 */ RR_DROP_ARROW,
    /* 3 */ RR_DROP_FLEXIBLE,
    /* 4 */ RR_DROP_RUPEE_PURPLE,
    /* 5 */ RR_DROP_RUPEE_RED
} EnRrDropType;

void EnRr_Init(Actor* thisx, GlobalContext* globalCtx);
void EnRr_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnRr_Update(Actor* thisx, GlobalContext* globalCtx);
void EnRr_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnRr_InitBodySegments(EnRr* self, GlobalContext* globalCtx);

void EnRr_SetupDamage(EnRr* self);
void EnRr_SetupDeath(EnRr* self);

void EnRr_Approach(EnRr* self, GlobalContext* globalCtx);
void EnRr_Reach(EnRr* self, GlobalContext* globalCtx);
void EnRr_GrabPlayer(EnRr* self, GlobalContext* globalCtx);
void EnRr_Damage(EnRr* self, GlobalContext* globalCtx);
void EnRr_Death(EnRr* self, GlobalContext* globalCtx);
void EnRr_Retreat(EnRr* self, GlobalContext* globalCtx);
void EnRr_Stunned(EnRr* self, GlobalContext* globalCtx);

const ActorInit En_Rr_InitVars = {
    ACTOR_EN_RR,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_RR,
    sizeof(EnRr),
    (ActorFunc)EnRr_Init,
    (ActorFunc)EnRr_Destroy,
    (ActorFunc)EnRr_Update,
    (ActorFunc)EnRr_Draw,
};

static char* sDropNames[] = {
    // "type 7", "small magic jar", "arrow", "fairy", "20 rupees", "50 rupees"
    "タイプ７  ", "魔法の壷小", "矢        ", "妖精      ", "20ルピー  ", "50ルピー  ",
};

static ColliderCylinderInitType1 sCylinderInit1 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 30, 55, 0, { 0, 0, 0 } },
};

static ColliderCylinderInitType1 sCylinderInit2 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_NO_PUSH | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 20, -10, { 0, 0, 0 } },
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, RR_DMG_NONE),
    /* Deku stick    */ DMG_ENTRY(2, RR_DMG_NORMAL),
    /* Slingshot     */ DMG_ENTRY(1, RR_DMG_NORMAL),
    /* Explosive     */ DMG_ENTRY(2, RR_DMG_NORMAL),
    /* Boomerang     */ DMG_ENTRY(0, RR_DMG_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, RR_DMG_NORMAL),
    /* Hammer swing  */ DMG_ENTRY(2, RR_DMG_NORMAL),
    /* Hookshot      */ DMG_ENTRY(0, RR_DMG_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, RR_DMG_NORMAL),
    /* Master sword  */ DMG_ENTRY(2, RR_DMG_NORMAL),
    /* Giant's Knife */ DMG_ENTRY(4, RR_DMG_NORMAL),
    /* Fire arrow    */ DMG_ENTRY(4, RR_DMG_FIRE),
    /* Ice arrow     */ DMG_ENTRY(4, RR_DMG_ICE),
    /* Light arrow   */ DMG_ENTRY(15, RR_DMG_LIGHT_ARROW),
    /* Unk arrow 1   */ DMG_ENTRY(4, RR_DMG_WIND_ARROW),
    /* Unk arrow 2   */ DMG_ENTRY(15, RR_DMG_SHDW_ARROW),
    /* Unk arrow 3   */ DMG_ENTRY(15, RR_DMG_SPRT_ARROW),
    /* Fire magic    */ DMG_ENTRY(4, RR_DMG_FIRE),
    /* Ice magic     */ DMG_ENTRY(3, RR_DMG_ICE),
    /* Light magic   */ DMG_ENTRY(10, RR_DMG_LIGHT_MAGIC),
    /* Shield        */ DMG_ENTRY(0, RR_DMG_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, RR_DMG_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, RR_DMG_NORMAL),
    /* Giant spin    */ DMG_ENTRY(4, RR_DMG_NORMAL),
    /* Master spin   */ DMG_ENTRY(2, RR_DMG_NORMAL),
    /* Kokiri jump   */ DMG_ENTRY(2, RR_DMG_NORMAL),
    /* Giant jump    */ DMG_ENTRY(8, RR_DMG_NORMAL),
    /* Master jump   */ DMG_ENTRY(4, RR_DMG_NORMAL),
    /* Unknown 1     */ DMG_ENTRY(10, RR_DMG_SPRT_ARROW),
    /* Unblockable   */ DMG_ENTRY(0, RR_DMG_NONE),
    /* Hammer jump   */ DMG_ENTRY(0, RR_DMG_NONE),
    /* Unknown 2     */ DMG_ENTRY(0, RR_DMG_NONE),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x37, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

void EnRr_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnRr* self = THIS;
    s32 i;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.colChkInfo.damageTable = &sDamageTable;
    self->actor.colChkInfo.health = 4;
    Collider_InitCylinder(globalCtx, &self->collider1);
    Collider_SetCylinderType1(globalCtx, &self->collider1, &self->actor, &sCylinderInit1);
    Collider_InitCylinder(globalCtx, &self->collider2);
    Collider_SetCylinderType1(globalCtx, &self->collider2, &self->actor, &sCylinderInit2);
    Actor_SetFocus(&self->actor, 30.0f);
    self->actor.scale.y = 0.013f;
    self->actor.scale.x = self->actor.scale.z = 0.014f;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.velocity.y = self->actor.speedXZ = 0.0f;
    self->actor.gravity = -0.4f;
    self->actionTimer = 0;
    self->eatenShield = 0;
    self->eatenTunic = 0;
    self->retreat = false;
    self->grabTimer = 0;
    self->invincibilityTimer = 0;
    self->effectTimer = 0;
    self->hasPlayer = false;
    self->stopScroll = false;
    self->ocTimer = 0;
    self->reachState = self->isDead = false;
    self->actionFunc = EnRr_Approach;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].height = self->bodySegs[i].heightTarget = self->bodySegs[i].scaleMod.x =
            self->bodySegs[i].scaleMod.y = self->bodySegs[i].scaleMod.z = 0.0f;
    }
    EnRr_InitBodySegments(self, globalCtx);
}

void EnRr_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnRr* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider1);
    Collider_DestroyCylinder(globalCtx, &self->collider2);
}

void EnRr_SetSpeed(EnRr* self, f32 speed) {
    self->actor.speedXZ = speed;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIKE_WALK);
}

void EnRr_SetupReach(EnRr* self) {
    static f32 segmentHeights[] = { 0.0f, 500.0f, 750.0f, 1000.0f, 1000.0f };
    s32 i;

    self->reachState = 1;
    self->actionTimer = 20;
    self->segPhaseVelTarget = 2500.0f;
    self->segMoveRate = 0.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].heightTarget = segmentHeights[i];
        self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z = 0.8f;
        self->bodySegs[i].rotTarget.x = 6000.0f;
        self->bodySegs[i].rotTarget.z = 0.0f;
    }
    self->actionFunc = EnRr_Reach;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIKE_UNARI);
}

void EnRr_SetupNeutral(EnRr* self) {
    s32 i;

    self->reachState = 0;
    self->segMoveRate = 0.0f;
    self->segPhaseVelTarget = 2500.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].heightTarget = 0.0f;
        self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
        self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z = 1.0f;
    }
    if (self->retreat) {
        self->actionTimer = 100;
        self->actionFunc = EnRr_Retreat;
    } else {
        self->actionTimer = 60;
        self->actionFunc = EnRr_Approach;
    }
}

void EnRr_SetupGrabPlayer(EnRr* self, Player* player) {
    s32 i;

    self->grabTimer = 100;
    self->actor.flags &= ~1;
    self->ocTimer = 8;
    self->hasPlayer = true;
    self->reachState = 0;
    self->segMoveRate = self->swallowOffset = self->actor.speedXZ = 0.0f;
    self->pulseSizeTarget = 0.15f;
    self->segPhaseVelTarget = 5000.0f;
    self->wobbleSizeTarget = 512.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].heightTarget = 0.0f;
        self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
        self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z = 1.0f;
    }
    self->actionFunc = EnRr_GrabPlayer;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIKE_DRINK);
}

u8 EnRr_GetMessage(u8 shield, u8 tunic) {
    u8 messageIndex = 0;

    if ((shield == 1 /* Deku shield */) || (shield == 2 /* Hylian shield */)) {
        messageIndex = RR_MESSAGE_SHIELD;
    }
    if ((tunic == 2 /* Goron tunic */) || (tunic == 3 /* Zora tunic */)) {
        messageIndex |= RR_MESSAGE_TUNIC;
    }

    return messageIndex;
}

void EnRr_SetupReleasePlayer(EnRr* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    u8 shield;
    u8 tunic;

    self->actor.flags |= 1;
    self->hasPlayer = false;
    self->ocTimer = 110;
    self->segMoveRate = 0.0f;
    self->segPhaseVelTarget = 2500.0f;
    self->wobbleSizeTarget = 2048.0f;
    tunic = 0;
    shield = 0;
    if (CUR_EQUIP_VALUE(EQUIP_SHIELD) != 3 /* Mirror shield */) {
        shield = Inventory_DeleteEquipment(globalCtx, EQUIP_SHIELD);
        if (shield != 0) {
            self->eatenShield = shield;
            self->retreat = true;
        }
    }
    if (CUR_EQUIP_VALUE(EQUIP_TUNIC) != 1 /* Kokiri tunic */) {
        tunic = Inventory_DeleteEquipment(globalCtx, EQUIP_TUNIC);
        if (tunic != 0) {
            self->eatenTunic = tunic;
            self->retreat = true;
        }
    }
    player->actor.parent = NULL;
    switch (EnRr_GetMessage(shield, tunic)) {
        case RR_MESSAGE_SHIELD:
            func_8010B680(globalCtx, 0x305F, NULL);
            break;
        case RR_MESSAGE_TUNIC:
            func_8010B680(globalCtx, 0x3060, NULL);
            break;
        case RR_MESSAGE_TUNIC | RR_MESSAGE_SHIELD:
            func_8010B680(globalCtx, 0x3061, NULL);
            break;
    }
    osSyncPrintf(VT_FGCOL(YELLOW) "%s[%d] : Rr_Catch_Cancel" VT_RST "\n", "../z_en_rr.c", 650);
    func_8002F6D4(globalCtx, &self->actor, 4.0f, self->actor.shape.rot.y, 12.0f, 8);
    if (self->actor.colorFilterTimer == 0) {
        self->actionFunc = EnRr_Approach;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIKE_THROW);
    } else if (self->actor.colChkInfo.health != 0) {
        EnRr_SetupDamage(self);
    } else {
        EnRr_SetupDeath(self);
    }
}

void EnRr_SetupDamage(EnRr* self) {
    s32 i;

    self->reachState = 0;
    self->actionTimer = 20;
    self->segMoveRate = 0.0f;
    self->segPhaseVelTarget = 2500.0f;
    self->pulseSizeTarget = 0.0f;
    self->wobbleSizeTarget = 0.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].heightTarget = 0.0f;
        self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
        self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z = 1.0f;
    }
    self->actionFunc = EnRr_Damage;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIKE_DAMAGE);
}

void EnRr_SetupApproach(EnRr* self) {
    s32 i;

    self->segMoveRate = 0.0f;
    self->pulseSizeTarget = 0.15f;
    self->segPhaseVelTarget = 2500.0f;
    self->wobbleSizeTarget = 2048.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].heightTarget = 0.0f;
        self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
        self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z = 1.0f;
    }
    self->actionFunc = EnRr_Approach;
}

void EnRr_SetupDeath(EnRr* self) {
    s32 i;

    self->isDead = true;
    self->frameCount = 0;
    self->shrinkRate = 0.0f;
    self->segMoveRate = 0.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].heightTarget = 0.0f;
        self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
    }
    self->actionFunc = EnRr_Death;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIKE_DEAD);
    self->actor.flags &= ~1;
}

void EnRr_SetupStunned(EnRr* self) {
    s32 i;

    self->stopScroll = true;
    self->segMovePhase = 0;
    self->segPhaseVel = 0.0f;
    self->segPhaseVelTarget = 2500.0f;
    self->segPulsePhaseDiff = 0.0f;
    self->segWobblePhaseDiffX = 0.0f;
    self->segWobbleXTarget = 3.0f;
    self->segWobblePhaseDiffZ = 0.0f;
    self->segWobbleZTarget = 1.0f;
    self->pulseSize = 0.0f;
    self->pulseSizeTarget = 0.15f;
    self->wobbleSize = 0.0f;
    self->wobbleSizeTarget = 2048.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].scaleMod.y = 0.0f;
        self->bodySegs[i].rotTarget.x = 0.0f;
        self->bodySegs[i].rotTarget.y = 0.0f;
        self->bodySegs[i].rotTarget.z = 0.0f;
        self->bodySegs[i].scale.x = self->bodySegs[i].scale.y = self->bodySegs[i].scale.z =
            self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.y = self->bodySegs[i].scaleTarget.z = 1.0f;
    }
    self->actionFunc = EnRr_Stunned;
}

void EnRr_CollisionCheck(EnRr* self, GlobalContext* globalCtx) {
    Vec3f hitPos;
    Player* player = GET_PLAYER(globalCtx);

    if (self->collider2.base.acFlags & AC_HIT) {
        self->collider2.base.acFlags &= ~AC_HIT;
        // "Kakin" (not sure what self means)
        osSyncPrintf(VT_FGCOL(GREEN) "カキン(%d)！！" VT_RST "\n", self->frameCount);
        hitPos.x = self->collider2.info.bumper.hitPos.x;
        hitPos.y = self->collider2.info.bumper.hitPos.y;
        hitPos.z = self->collider2.info.bumper.hitPos.z;
        CollisionCheck_SpawnShieldParticlesMetal2(globalCtx, &hitPos);
    } else {
        if (self->collider1.base.acFlags & AC_HIT) {
            u8 dropType = RR_DROP_RANDOM_RUPEE;

            self->collider1.base.acFlags &= ~AC_HIT;
            if (self->actor.colChkInfo.damageEffect != 0) {
                hitPos.x = self->collider1.info.bumper.hitPos.x;
                hitPos.y = self->collider1.info.bumper.hitPos.y;
                hitPos.z = self->collider1.info.bumper.hitPos.z;
                CollisionCheck_BlueBlood(globalCtx, NULL, &hitPos);
            }
            switch (self->actor.colChkInfo.damageEffect) {
                case RR_DMG_LIGHT_ARROW:
                    dropType++; // purple rupee
                case RR_DMG_SHDW_ARROW:
                    dropType++; // flexible
                case RR_DMG_WIND_ARROW:
                    dropType++; // arrow
                case RR_DMG_SPRT_ARROW:
                    dropType++; // magic jar
                case RR_DMG_NORMAL:
                    // "ouch"
                    osSyncPrintf(VT_FGCOL(RED) "いてっ( %d : LIFE %d : DAMAGE %d : %x )！！" VT_RST "\n",
                                 self->frameCount, self->actor.colChkInfo.health, self->actor.colChkInfo.damage,
                                 self->actor.colChkInfo.damageEffect);
                    self->stopScroll = false;
                    Actor_ApplyDamage(&self->actor);
                    self->invincibilityTimer = 40;
                    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, self->invincibilityTimer);
                    if (self->hasPlayer) {
                        EnRr_SetupReleasePlayer(self, globalCtx);
                    } else if (self->actor.colChkInfo.health != 0) {
                        EnRr_SetupDamage(self);
                    } else {
                        self->dropType = dropType;
                        EnRr_SetupDeath(self);
                    }
                    return;
                case RR_DMG_FIRE: // Fire Arrow and Din's Fire
                    Actor_ApplyDamage(&self->actor);
                    if (self->actor.colChkInfo.health == 0) {
                        self->dropType = RR_DROP_RANDOM_RUPEE;
                    }
                    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 0x50);
                    self->effectTimer = 20;
                    EnRr_SetupStunned(self);
                    return;
                case RR_DMG_ICE: // Ice Arrow and unused ice magic
                    Actor_ApplyDamage(&self->actor);
                    if (self->actor.colChkInfo.health == 0) {
                        self->dropType = RR_DROP_RANDOM_RUPEE;
                    }
                    if (self->actor.colorFilterTimer == 0) {
                        self->effectTimer = 20;
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0x2000, 0x50);
                    }
                    EnRr_SetupStunned(self);
                    return;
                case RR_DMG_LIGHT_MAGIC: // Unused light magic
                    Actor_ApplyDamage(&self->actor);
                    if (self->actor.colChkInfo.health == 0) {
                        self->dropType = RR_DROP_RUPEE_RED;
                    }
                    Actor_SetColorFilter(&self->actor, -0x8000, 0xFF, 0x2000, 0x50);
                    EnRr_SetupStunned(self);
                    return;
                case RR_DMG_STUN: // Boomerang and Hookshot
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
                    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0x2000, 0x50);
                    EnRr_SetupStunned(self);
                    return;
            }
        }
        if ((self->ocTimer == 0) && (self->actor.colorFilterTimer == 0) && (player->invincibilityTimer == 0) &&
            !(player->stateFlags2 & 0x80) &&
            ((self->collider1.base.ocFlags1 & OC1_HIT) || (self->collider2.base.ocFlags1 & OC1_HIT))) {
            self->collider1.base.ocFlags1 &= ~OC1_HIT;
            self->collider2.base.ocFlags1 &= ~OC1_HIT;
            // "catch"
            osSyncPrintf(VT_FGCOL(GREEN) "キャッチ(%d)！！" VT_RST "\n", self->frameCount);
            if (globalCtx->grabPlayer(globalCtx, player)) {
                player->actor.parent = &self->actor;
                self->stopScroll = false;
                EnRr_SetupGrabPlayer(self, player);
            }
        }
    }
}

void EnRr_InitBodySegments(EnRr* self, GlobalContext* globalCtx) {
    s32 i;

    self->segMovePhase = 0;
    self->segPhaseVel = 0.0f;
    self->segPhaseVelTarget = 2500.0f;
    self->segPulsePhaseDiff = 0.0f;
    self->segWobblePhaseDiffX = 0.0f;
    self->segWobbleXTarget = 3.0f;
    self->segWobblePhaseDiffZ = 0.0f;
    self->segWobbleZTarget = 1.0f;
    self->pulseSize = 0.0f;
    self->pulseSizeTarget = 0.15f;
    self->wobbleSize = 0.0f;
    self->wobbleSizeTarget = 2048.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].scaleMod.y = 0.0f;
        self->bodySegs[i].rotTarget.x = 0.0f;
        self->bodySegs[i].rotTarget.y = 0.0f;
        self->bodySegs[i].rotTarget.z = 0.0f;
        self->bodySegs[i].scale.x = self->bodySegs[i].scale.y = self->bodySegs[i].scale.z =
            self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.y = self->bodySegs[i].scaleTarget.z = 1.0f;
    }
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].scaleMod.x = self->bodySegs[i].scaleMod.z =
            Math_CosS(i * (u32)(s16)self->segPulsePhaseDiff * 0x1000) * self->pulseSize;
    }
    for (i = 1; i < 5; i++) {
        self->bodySegs[i].rotTarget.x = Math_CosS(i * (u32)(s16)self->segWobblePhaseDiffX * 0x1000) * self->wobbleSize;
        self->bodySegs[i].rotTarget.z = Math_SinS(i * (u32)(s16)self->segWobblePhaseDiffZ * 0x1000) * self->wobbleSize;
    }
}

void EnRr_UpdateBodySegments(EnRr* self, GlobalContext* globalCtx) {
    s32 i;
    s16 phase = self->segMovePhase;

    if (!self->isDead) {
        for (i = 0; i < 5; i++) {
            self->bodySegs[i].scaleMod.x = self->bodySegs[i].scaleMod.z =
                Math_CosS(phase + i * (s16)self->segPulsePhaseDiff * 0x1000) * self->pulseSize;
        }
        phase = self->segMovePhase;
        if (!self->isDead && (self->reachState == 0)) {
            for (i = 1; i < 5; i++) {
                self->bodySegs[i].rotTarget.x =
                    Math_CosS(phase + i * (s16)self->segWobblePhaseDiffX * 0x1000) * self->wobbleSize;
                self->bodySegs[i].rotTarget.z =
                    Math_SinS(phase + i * (s16)self->segWobblePhaseDiffZ * 0x1000) * self->wobbleSize;
            }
        }
    }
    if (!self->stopScroll) {
        self->segMovePhase += (s16)self->segPhaseVel;
    }
}

void EnRr_Approach(EnRr* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, 0x1F4, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if ((self->actionTimer == 0) && (self->actor.xzDistToPlayer < 160.0f)) {
        EnRr_SetupReach(self);
    } else if ((self->actor.xzDistToPlayer < 400.0f) && (self->actor.speedXZ == 0.0f)) {
        EnRr_SetSpeed(self, 2.0f);
    }
}

void EnRr_Reach(EnRr* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, 0x1F4, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    switch (self->reachState) {
        case REACH_EXTEND:
            if (self->actionTimer == 0) {
                self->reachState = REACH_STOP;
            }
            break;
        case REACH_STOP:
            if (self->actionTimer == 0) {
                self->actionTimer = 5;
                self->bodySegs[RR_MOUTH].scaleTarget.x = self->bodySegs[RR_MOUTH].scaleTarget.z = 1.5f;
                self->reachState = REACH_OPEN;
            }
            break;
        case REACH_OPEN:
            if (self->actionTimer == 0) {
                self->actionTimer = 2;
                self->bodySegs[RR_MOUTH].heightTarget = 2000.0f;
                self->reachState = REACH_GAPE;
            }
            break;
        case REACH_GAPE:
            if (self->actionTimer == 0) {
                self->actionTimer = 20;
                self->bodySegs[RR_MOUTH].scaleTarget.x = self->bodySegs[RR_MOUTH].scaleTarget.z = 0.8f;
                self->reachState = REACH_CLOSE;
            }
            break;
        case REACH_CLOSE:
            if (self->actionTimer == 0) {
                EnRr_SetupNeutral(self);
            }
            break;
    }
}

void EnRr_GrabPlayer(EnRr* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    func_800AA000(self->actor.xyzDistToPlayerSq, 120, 2, 120);
    if ((self->frameCount % 8) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIKE_EAT);
    }
    self->ocTimer = 8;
    if ((self->grabTimer == 0) || !(player->stateFlags2 & 0x80)) {
        EnRr_SetupReleasePlayer(self, globalCtx);
    } else {
        Math_ApproachF(&player->actor.world.pos.x, self->mouthPos.x, 1.0f, 30.0f);
        Math_ApproachF(&player->actor.world.pos.y, self->mouthPos.y + self->swallowOffset, 1.0f, 30.0f);
        Math_ApproachF(&player->actor.world.pos.z, self->mouthPos.z, 1.0f, 30.0f);
        Math_ApproachF(&self->swallowOffset, -55.0f, 1.0f, 5.0f);
    }
}

void EnRr_Damage(EnRr* self, GlobalContext* globalCtx) {
    s32 i;

    if (self->actor.colorFilterTimer == 0) {
        EnRr_SetupApproach(self);
    } else if ((self->actor.colorFilterTimer & 8) != 0) {
        for (i = 1; i < 5; i++) {
            self->bodySegs[i].rotTarget.z = 5000.0f;
        }
    } else {
        for (i = 1; i < 5; i++) {
            self->bodySegs[i].rotTarget.z = -5000.0f;
        }
    }
}

void EnRr_Death(EnRr* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 i;

    if (self->frameCount < 40) {
        for (i = 0; i < 5; i++) {
            Math_ApproachF(&self->bodySegs[i].heightTarget, i + 59 - (self->frameCount * 25.0f), 1.0f, 50.0f);
            self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z =
                (SQ(4 - i) * (f32)self->frameCount * 0.003f) + 1.0f;
        }
    } else if (self->frameCount >= 95) {
        Vec3f dropPos;

        dropPos.x = self->actor.world.pos.x;
        dropPos.y = self->actor.world.pos.y;
        dropPos.z = self->actor.world.pos.z;
        switch (self->eatenShield) {
            case 1:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_SHIELD_DEKU);
                break;
            case 2:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_SHIELD_HYLIAN);
                break;
        }
        switch (self->eatenTunic) {
            case 2:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_TUNIC_GORON);
                break;
            case 3:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_TUNIC_ZORA);
                break;
        }
        // "dropped"
        osSyncPrintf(VT_FGCOL(GREEN) "「%s」が出た！！" VT_RST "\n", sDropNames[self->dropType]);
        switch (self->dropType) {
            case RR_DROP_MAGIC:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_MAGIC_SMALL);
                break;
            case RR_DROP_ARROW:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_ARROWS_SINGLE);
                break;
            case RR_DROP_FLEXIBLE:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_FLEXIBLE);
                break;
            case RR_DROP_RUPEE_PURPLE:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_RUPEE_PURPLE);
                break;
            case RR_DROP_RUPEE_RED:
                Item_DropCollectible(globalCtx, &dropPos, ITEM00_RUPEE_RED);
                break;
            case RR_DROP_RANDOM_RUPEE:
            default:
                Item_DropCollectibleRandom(globalCtx, &self->actor, &dropPos, 12 << 4);
                break;
        }
        Actor_Kill(&self->actor);
    } else if (self->frameCount == 88) {
        Vec3f pos;
        Vec3f vel;
        Vec3f accel;

        pos.x = self->actor.world.pos.x;
        pos.y = self->actor.world.pos.y + 20.0f;
        pos.z = self->actor.world.pos.z;
        vel.x = 0.0f;
        vel.y = 0.0f;
        vel.z = 0.0f;
        accel.x = 0.0f;
        accel.y = 0.0f;
        accel.z = 0.0f;

        EffectSsDeadDb_Spawn(globalCtx, &pos, &vel, &accel, 100, 0, 255, 255, 255, 255, 255, 0, 0, 1, 9, true);
    } else {
        Math_ApproachF(&self->actor.scale.x, 0.0f, 1.0f, self->shrinkRate);
        Math_ApproachF(&self->shrinkRate, 0.001f, 1.0f, 0.00001f);
        self->actor.scale.z = self->actor.scale.x;
    }
}

void EnRr_Retreat(EnRr* self, GlobalContext* globalCtx) {
    if (self->actionTimer == 0) {
        self->retreat = false;
        self->actionFunc = EnRr_Approach;
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer + 0x8000, 0xA, 0x3E8, 0);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (self->actor.speedXZ == 0.0f) {
            EnRr_SetSpeed(self, 2.0f);
        }
    }
}

void EnRr_Stunned(EnRr* self, GlobalContext* globalCtx) {
    if (self->actor.colorFilterTimer == 0) {
        self->stopScroll = false;
        if (self->hasPlayer) {
            EnRr_SetupReleasePlayer(self, globalCtx);
        } else if (self->actor.colChkInfo.health != 0) {
            self->actionFunc = EnRr_Approach;
        } else {
            EnRr_SetupDeath(self);
        }
    }
}

void EnRr_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnRr* self = THIS;
    s32 i;

    self->frameCount++;
    if (!self->stopScroll) {
        self->scrollTimer++;
    }
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (self->grabTimer != 0) {
        self->grabTimer--;
    }
    if (self->ocTimer != 0) {
        self->ocTimer--;
    }
    if (self->invincibilityTimer != 0) {
        self->invincibilityTimer--;
    }
    if (self->effectTimer != 0) {
        self->effectTimer--;
    }

    Actor_SetFocus(&self->actor, 30.0f);
    EnRr_UpdateBodySegments(self, globalCtx);
    if (!self->isDead && ((self->actor.colorFilterTimer == 0) || !(self->actor.colorFilterParams & 0x4000))) {
        EnRr_CollisionCheck(self, globalCtx);
    }

    self->actionFunc(self, globalCtx);
    if (self->hasPlayer == 0x3F80) { // checks if 1.0f has been stored to hasPlayer's address
        ASSERT(0, "0", "../z_en_rr.c", 1355);
    }

    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.1f);
    Actor_MoveForward(&self->actor);
    Collider_UpdateCylinder(&self->actor, &self->collider1);
    self->collider2.dim.pos.x = self->mouthPos.x;
    self->collider2.dim.pos.y = self->mouthPos.y;
    self->collider2.dim.pos.z = self->mouthPos.z;
    if (!self->isDead && (self->invincibilityTimer == 0)) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider1.base);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider2.base);
        if (self->ocTimer == 0) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider1.base);
        }
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider2.base);
    } else {
        self->collider2.base.ocFlags1 &= ~OC1_HIT;
        self->collider2.base.acFlags &= ~AC_HIT;
        self->collider1.base.ocFlags1 &= ~OC1_HIT;
        self->collider1.base.acFlags &= ~AC_HIT;
    }
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 30.0f, 20.0f, 7);
    if (!self->stopScroll) {
        Math_ApproachF(&self->segPhaseVel, self->segPhaseVelTarget, 1.0f, 50.0f);
        Math_ApproachF(&self->segPulsePhaseDiff, 4.0f, 1.0f, 5.0f);
        Math_ApproachF(&self->segWobblePhaseDiffX, self->segWobbleXTarget, 1.0f, 0.04f);
        Math_ApproachF(&self->segWobblePhaseDiffZ, self->segWobbleZTarget, 1.0f, 0.01f);
        Math_ApproachF(&self->pulseSize, self->pulseSizeTarget, 1.0f, 0.0015f);
        Math_ApproachF(&self->wobbleSize, self->wobbleSizeTarget, 1.0f, 20.0f);
        for (i = 0; i < 5; i++) {
            Math_SmoothStepToS(&self->bodySegs[i].rot.x, self->bodySegs[i].rotTarget.x, 5, self->segMoveRate * 1000.0f,
                               0);
            Math_SmoothStepToS(&self->bodySegs[i].rot.z, self->bodySegs[i].rotTarget.z, 5, self->segMoveRate * 1000.0f,
                               0);
            Math_ApproachF(&self->bodySegs[i].scale.x, self->bodySegs[i].scaleTarget.x, 1.0f, self->segMoveRate * 0.2f);
            self->bodySegs[i].scale.z = self->bodySegs[i].scale.x;
            Math_ApproachF(&self->bodySegs[i].height, self->bodySegs[i].heightTarget, 1.0f, self->segMoveRate * 300.0f);
        }
        Math_ApproachF(&self->segMoveRate, 1.0f, 1.0f, 0.2f);
    }
}

static Vec3f sEffectOffsets[] = {
    { 25.0f, 0.0f, 0.0f },
    { -25.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 25.0f },
    { 0.0f, 0.0f, -25.0f },
};

void EnRr_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f zeroVec;
    EnRr* self = THIS;
    s32 i;
    Mtx* segMtx = Graph_Alloc(globalCtx->state.gfxCtx, 4 * sizeof(Mtx));

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_rr.c", 1478);
    if (1) {}
    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x0C, segMtx);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (self->scrollTimer * 0) & 0x7F,
                                (self->scrollTimer * 0) & 0x3F, 32, 16, 1, (self->scrollTimer * 0) & 0x3F,
                                (self->scrollTimer * -6) & 0x7F, 32, 16));
    Matrix_Push();

    Matrix_Scale((1.0f + self->bodySegs[RR_BASE].scaleMod.x) * self->bodySegs[RR_BASE].scale.x,
                 (1.0f + self->bodySegs[RR_BASE].scaleMod.y) * self->bodySegs[RR_BASE].scale.y,
                 (1.0f + self->bodySegs[RR_BASE].scaleMod.z) * self->bodySegs[RR_BASE].scale.z, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_rr.c", 1501),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Matrix_Pop();
    zeroVec.x = 0.0f;
    zeroVec.y = 0.0f;
    zeroVec.z = 0.0f;
    for (i = 1; i < 5; i++) {
        Matrix_Translate(0.0f, self->bodySegs[i].height + 1000.0f, 0.0f, MTXMODE_APPLY);

        Matrix_RotateRPY(self->bodySegs[i].rot.x, self->bodySegs[i].rot.y, self->bodySegs[i].rot.z, MTXMODE_APPLY);
        Matrix_Push();
        Matrix_Scale((1.0f + self->bodySegs[i].scaleMod.x) * self->bodySegs[i].scale.x,
                     (1.0f + self->bodySegs[i].scaleMod.y) * self->bodySegs[i].scale.y,
                     (1.0f + self->bodySegs[i].scaleMod.z) * self->bodySegs[i].scale.z, MTXMODE_APPLY);
        Matrix_ToMtx(segMtx, "../z_en_rr.c", 1527);
        Matrix_Pop();
        segMtx++;
        Matrix_MultVec3f(&zeroVec, &self->effectPos[i]);
    }
    self->effectPos[0] = self->actor.world.pos;
    Matrix_MultVec3f(&zeroVec, &self->mouthPos);
    gSPDisplayList(POLY_XLU_DISP++, gLikeLikeDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_rr.c", 1551);
    if (self->effectTimer != 0) {
        Vec3f effectPos;
        s16 effectTimer = self->effectTimer - 1;

        self->actor.colorFilterTimer++;
        if ((effectTimer & 1) == 0) {
            s32 segIndex = 4 - (effectTimer >> 2);
            s32 offIndex = (effectTimer >> 1) & 3;

            effectPos.x = self->effectPos[segIndex].x + sEffectOffsets[offIndex].x + Rand_CenteredFloat(10.0f);
            effectPos.y = self->effectPos[segIndex].y + sEffectOffsets[offIndex].y + Rand_CenteredFloat(10.0f);
            effectPos.z = self->effectPos[segIndex].z + sEffectOffsets[offIndex].z + Rand_CenteredFloat(10.0f);
            if (self->actor.colorFilterParams & 0x4000) {
                EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &effectPos, 100, 0, 0, -1);
            } else {
                EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &effectPos, 150, 150, 150, 250, 235, 245, 255,
                                               3.0f);
            }
        }
    }
}
