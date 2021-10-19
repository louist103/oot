#include "z_en_peehat.h"
#include "objects/object_peehat/object_peehat.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"

#define FLAGS 0x01000015

#define THIS ((EnPeehat*)thisx)

#define GROUND_HOVER_HEIGHT 75.0f
#define MAX_LARVA 3

void EnPeehat_Init(Actor* thisx, GlobalContext* globalCtx);
void EnPeehat_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnPeehat_Update(Actor* thisx, GlobalContext* globalCtx);
void EnPeehat_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnPeehat_Ground_SetStateGround(EnPeehat* self);
void EnPeehat_Flying_SetStateGround(EnPeehat* self);
void EnPeehat_Larva_SetStateSeekPlayer(EnPeehat* self);
void EnPeehat_Ground_StateGround(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Ground_SetStateRise(EnPeehat* self);
void EnPeehat_Flying_StateGrounded(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Flying_SetStateRise(EnPeehat* self);
void EnPeehat_Flying_StateFly(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Flying_SetStateLanding(EnPeehat* self);
void EnPeehat_Ground_StateRise(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Ground_SetStateHover(EnPeehat* self);
void EnPeehat_Flying_StateRise(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Ground_StateSeekPlayer(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Ground_SetStateReturnHome(EnPeehat* self);
void EnPeehat_Ground_SetStateLanding(EnPeehat* self);
void EnPeehat_Larva_StateSeekPlayer(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_SetStateAttackRecoil(EnPeehat* self);
void EnPeehat_Ground_StateLanding(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Flying_StateLanding(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Ground_StateHover(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Ground_StateReturnHome(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_StateAttackRecoil(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_StateBoomerangStunned(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_Adult_StateDie(EnPeehat* self, GlobalContext* globalCtx);
void EnPeehat_SetStateExplode(EnPeehat* self);
void EnPeehat_StateExplode(EnPeehat* self, GlobalContext* globalCtx);

const ActorInit En_Peehat_InitVars = {
    ACTOR_EN_PEEHAT,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_PEEHAT,
    sizeof(EnPeehat),
    (ActorFunc)EnPeehat_Init,
    (ActorFunc)EnPeehat_Destroy,
    (ActorFunc)EnPeehat_Update,
    (ActorFunc)EnPeehat_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_WOOD,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 50, 160, -70, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElemInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT6,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElemInit,
};

static ColliderQuadInit sQuadInit = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x10 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

typedef enum {
    /* 00 */ PEAHAT_DMG_EFF_ATTACK = 0,
    /* 06 */ PEAHAT_DMG_EFF_LIGHT_ICE_ARROW = 6,
    /* 12 */ PEAHAT_DMG_EFF_FIRE = 12,
    /* 13 */ PEAHAT_DMG_EFF_HOOKSHOT = 13,
    /* 14 */ PEAHAT_DMG_EFF_BOOMERANG = 14,
    /* 15 */ PEAHAT_DMG_EFF_NUT = 15
} DamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, PEAHAT_DMG_EFF_NUT),
    /* Deku stick    */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Slingshot     */ DMG_ENTRY(1, PEAHAT_DMG_EFF_ATTACK),
    /* Explosive     */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Boomerang     */ DMG_ENTRY(0, PEAHAT_DMG_EFF_BOOMERANG),
    /* Normal arrow  */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Hammer swing  */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Hookshot      */ DMG_ENTRY(2, PEAHAT_DMG_EFF_HOOKSHOT),
    /* Kokiri sword  */ DMG_ENTRY(1, PEAHAT_DMG_EFF_ATTACK),
    /* Master sword  */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Giant's Knife */ DMG_ENTRY(4, PEAHAT_DMG_EFF_ATTACK),
    /* Fire arrow    */ DMG_ENTRY(4, PEAHAT_DMG_EFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Light arrow   */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Unk arrow 1   */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Unk arrow 2   */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Unk arrow 3   */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Fire magic    */ DMG_ENTRY(3, PEAHAT_DMG_EFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(0, PEAHAT_DMG_EFF_LIGHT_ICE_ARROW),
    /* Light magic   */ DMG_ENTRY(0, PEAHAT_DMG_EFF_LIGHT_ICE_ARROW),
    /* Shield        */ DMG_ENTRY(0, PEAHAT_DMG_EFF_ATTACK),
    /* Mirror Ray    */ DMG_ENTRY(0, PEAHAT_DMG_EFF_ATTACK),
    /* Kokiri spin   */ DMG_ENTRY(1, PEAHAT_DMG_EFF_ATTACK),
    /* Giant spin    */ DMG_ENTRY(4, PEAHAT_DMG_EFF_ATTACK),
    /* Master spin   */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Kokiri jump   */ DMG_ENTRY(2, PEAHAT_DMG_EFF_ATTACK),
    /* Giant jump    */ DMG_ENTRY(8, PEAHAT_DMG_EFF_ATTACK),
    /* Master jump   */ DMG_ENTRY(4, PEAHAT_DMG_EFF_ATTACK),
    /* Unknown 1     */ DMG_ENTRY(0, PEAHAT_DMG_EFF_ATTACK),
    /* Unblockable   */ DMG_ENTRY(0, PEAHAT_DMG_EFF_ATTACK),
    /* Hammer jump   */ DMG_ENTRY(4, PEAHAT_DMG_EFF_ATTACK),
    /* Unknown 2     */ DMG_ENTRY(0, PEAHAT_DMG_EFF_ATTACK),
};

typedef enum {
    /* 00 */ PEAHAT_STATE_DYING,
    /* 01 */ PEAHAT_STATE_EXPLODE,
    /* 03 */ PEAHAT_STATE_3 = 3,
    /* 04 */ PEAHAT_STATE_4,
    /* 05 */ PEAHAT_STATE_FLY,
    /* 07 */ PEAHAT_STATE_ATTACK_RECOIL = 7,
    /* 08 */ PEAHAT_STATE_8,
    /* 09 */ PEAHAT_STATE_9,
    /* 10 */ PEAHAT_STATE_LANDING,
    /* 12 */ PEAHAT_STATE_RETURN_HOME = 12,
    /* 13 */ PEAHAT_STATE_STUNNED,
    /* 14 */ PEAHAT_STATE_SEEK_PLAYER,
    /* 15 */ PEAHAT_STATE_15
} PeahatState;

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 700, ICHAIN_STOP),
};

void EnPeehat_SetupAction(EnPeehat* self, EnPeehatActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnPeehat_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnPeehat* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Actor_SetScale(&self->actor, 36.0f * 0.001f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gPeehatSkel, &gPeehatRisingAnim, self->jointTable, self->morphTable,
                   24);
    ActorShape_Init(&self->actor.shape, 100.0f, ActorShadow_DrawCircle, 27.0f);
    self->actor.focus.pos = self->actor.world.pos;
    self->unk2D4 = 0;
    self->actor.world.rot.y = 0;
    self->actor.colChkInfo.mass = MASS_HEAVY;
    self->actor.colChkInfo.health = 6;
    self->actor.colChkInfo.damageTable = &sDamageTable;
    self->actor.floorHeight = self->actor.world.pos.y;
    Collider_InitCylinder(globalCtx, &self->colCylinder);
    Collider_SetCylinder(globalCtx, &self->colCylinder, &self->actor, &sCylinderInit);
    Collider_InitQuad(globalCtx, &self->colQuad);
    Collider_SetQuad(globalCtx, &self->colQuad, &self->actor, &sQuadInit);
    Collider_InitJntSph(globalCtx, &self->colJntSph);
    Collider_SetJntSph(globalCtx, &self->colJntSph, &self->actor, &sJntSphInit, self->colJntSphItemList);

    self->actor.naviEnemyId = 0x48;
    self->xzDistToRise = 740.0f;
    self->xzDistMax = 1200.0f;
    self->actor.uncullZoneForward = 4000.0f;
    self->actor.uncullZoneScale = 800.0f;
    self->actor.uncullZoneDownward = 1800.0f;
    switch (self->actor.params) {
        case PEAHAT_TYPE_GROUNDED:
            EnPeehat_Ground_SetStateGround(self);
            break;
        case PEAHAT_TYPE_FLYING:
            self->actor.uncullZoneForward = 4200.0f;
            self->xzDistToRise = 2800.0f;
            self->xzDistMax = 1400.0f;
            EnPeehat_Flying_SetStateGround(self);
            self->actor.flags &= ~1;
            break;
        case PEAHAT_TYPE_LARVA:
            self->actor.scale.x = self->actor.scale.z = 0.006f;
            self->actor.scale.y = 0.003f;
            self->colCylinder.dim.radius = 25;
            self->colCylinder.dim.height = 15;
            self->colCylinder.dim.yShift = -5;
            self->colCylinder.info.bumper.dmgFlags = 0x1F824;
            self->colQuad.base.atFlags = AT_ON | AT_TYPE_ENEMY;
            self->colQuad.base.acFlags = AC_ON | AC_TYPE_PLAYER;
            self->actor.naviEnemyId = 0x49; // Larva
            EnPeehat_Larva_SetStateSeekPlayer(self);
            break;
    }
}

void EnPeehat_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnPeehat* self = THIS;
    EnPeehat* parent;

    Collider_DestroyCylinder(globalCtx, &self->colCylinder);
    Collider_DestroyJntSph(globalCtx, &self->colJntSph);

    // If PEAHAT_TYPE_LARVA, decrement total larva spawned
    if (self->actor.params > 0) {
        parent = (EnPeehat*)self->actor.parent;
        if (parent != NULL && parent->actor.update != NULL) {
            parent->unk2FA--;
        }
    }
}

void EnPeehat_SpawnDust(GlobalContext* globalCtx, EnPeehat* self, Vec3f* pos, f32 arg3, s32 arg4, f32 arg5, f32 arg6) {
    Vec3f dustPos;
    Vec3f dustVel = { 0.0f, 8.0f, 0.0f };
    Vec3f dustAccel = { 0.0f, -1.5f, 0.0f };
    f32 rot; // radians
    s32 pScale;

    rot = (Rand_ZeroOne() - 0.5f) * 6.28f;
    dustPos.y = self->actor.floorHeight;
    dustPos.x = Math_SinF(rot) * arg3 + pos->x;
    dustPos.z = Math_CosF(rot) * arg3 + pos->z;
    dustAccel.x = (Rand_ZeroOne() - 0.5f) * arg5;
    dustAccel.z = (Rand_ZeroOne() - 0.5f) * arg5;
    dustVel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    pScale = (Rand_ZeroOne() * 5 + 12) * arg6;
    EffectSsHahen_Spawn(globalCtx, &dustPos, &dustVel, &dustAccel, arg4, pScale, HAHEN_OBJECT_DEFAULT, 10, NULL);
}

/**
 * Handles being hit when on the ground
 */
void EnPeehat_HitWhenGrounded(EnPeehat* self, GlobalContext* globalCtx) {
    self->colCylinder.base.acFlags &= ~AC_HIT;
    if ((globalCtx->gameplayFrames & 0xF) == 0) {
        Vec3f itemDropPos = self->actor.world.pos;
        itemDropPos.y += 70.0f;
        Item_DropCollectibleRandom(globalCtx, &self->actor, &itemDropPos, 0x40);
        Item_DropCollectibleRandom(globalCtx, &self->actor, &itemDropPos, 0x40);
        Item_DropCollectibleRandom(globalCtx, &self->actor, &itemDropPos, 0x40);
        self->unk2D4 = 240;
    } else {
        s32 i;
        self->colCylinder.base.acFlags &= ~AC_HIT;
        for (i = MAX_LARVA - self->unk2FA; i > 0; i--) {
            Actor* larva =
                Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_PEEHAT,
                                   Rand_CenteredFloat(25.0f) + self->actor.world.pos.x,
                                   Rand_CenteredFloat(25.0f) + (self->actor.world.pos.y + 50.0f),
                                   Rand_CenteredFloat(25.0f) + self->actor.world.pos.z, 0, 0, 0, PEAHAT_TYPE_LARVA);
            if (larva != NULL) {
                larva->velocity.y = 6.0f;
                larva->shape.rot.y = larva->world.rot.y = Rand_CenteredFloat(0xFFFF);
                self->unk2FA++;
            }
        }
        self->unk2D4 = 8;
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_DAMAGE);
}

void EnPeehat_Ground_SetStateGround(EnPeehat* self) {
    Animation_Change(&self->skelAnime, &gPeehatRisingAnim, 0.0f, 3.0f, Animation_GetLastFrame(&gPeehatRisingAnim),
                     ANIMMODE_ONCE, 0.0f);
    self->seekPlayerTimer = 600;
    self->unk2D4 = 0;
    self->unk2FA = 0;
    self->state = PEAHAT_STATE_3;
    self->colCylinder.base.acFlags &= ~AC_HIT;
    EnPeehat_SetupAction(self, EnPeehat_Ground_StateGround);
}

void EnPeehat_Ground_StateGround(EnPeehat* self, GlobalContext* globalCtx) {
    if (IS_DAY) {
        self->actor.flags |= 1;
        if (self->riseDelayTimer == 0) {
            if (self->actor.xzDistToPlayer < self->xzDistToRise) {
                EnPeehat_Ground_SetStateRise(self);
            }
        } else {
            Math_SmoothStepToF(&self->actor.shape.yOffset, -1000.0f, 1.0f, 10.0f, 0.0f);
            self->riseDelayTimer--;
        }
    } else {
        self->actor.flags &= ~1;
        Math_SmoothStepToF(&self->actor.shape.yOffset, -1000.0f, 1.0f, 50.0f, 0.0f);
        if (self->unk2D4 != 0) {
            self->unk2D4--;
            if (self->unk2D4 & 4) {
                Math_SmoothStepToF(&self->scaleShift, 0.205f, 1.0f, 0.235f, 0.0f);
            } else {
                Math_SmoothStepToF(&self->scaleShift, 0.0f, 1.0f, 0.005f, 0.0f);
            }
        } else if (self->colCylinder.base.acFlags & AC_HIT) {
            EnPeehat_HitWhenGrounded(self, globalCtx);
        }
    }
}

void EnPeehat_Flying_SetStateGround(EnPeehat* self) {
    Animation_Change(&self->skelAnime, &gPeehatRisingAnim, 0.0f, 3.0f, Animation_GetLastFrame(&gPeehatRisingAnim),
                     ANIMMODE_ONCE, 0.0f);
    self->seekPlayerTimer = 400;
    self->unk2D4 = 0;
    self->unk2FA = 0; //! @bug: overwrites number of child larva spawned, allowing for more than MAX_LARVA spawns
    self->state = PEAHAT_STATE_4;
    EnPeehat_SetupAction(self, EnPeehat_Flying_StateGrounded);
}

void EnPeehat_Flying_StateGrounded(EnPeehat* self, GlobalContext* globalCtx) {
    if (IS_DAY) {
        if (self->actor.xzDistToPlayer < self->xzDistToRise) {
            EnPeehat_Flying_SetStateRise(self);
        }
    } else {
        Math_SmoothStepToF(&self->actor.shape.yOffset, -1000.0f, 1.0f, 50.0f, 0.0f);
        if (self->unk2D4 != 0) {
            self->unk2D4--;
            if (self->unk2D4 & 4) {
                Math_SmoothStepToF(&self->scaleShift, 0.205f, 1.0f, 0.235f, 0.0f);
            } else {
                Math_SmoothStepToF(&self->scaleShift, 0.0f, 1.0f, 0.005f, 0.0f);
            }
        } else if (self->colCylinder.base.acFlags & AC_HIT) {
            EnPeehat_HitWhenGrounded(self, globalCtx);
        }
    }
}

void EnPeehat_Flying_SetStateFly(EnPeehat* self) {
    Animation_PlayLoop(&self->skelAnime, &gPeehatFlyingAnim);
    self->state = PEAHAT_STATE_FLY;
    EnPeehat_SetupAction(self, EnPeehat_Flying_StateFly);
}

void EnPeehat_Flying_StateFly(EnPeehat* self, GlobalContext* globalCtx) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
    SkelAnime_Update(&self->skelAnime);
    if (!IS_DAY || self->xzDistToRise < self->actor.xzDistToPlayer) {
        EnPeehat_Flying_SetStateLanding(self);
    } else if (self->actor.xzDistToPlayer < self->xzDistMax) {
        if (self->unk2FA < MAX_LARVA && (globalCtx->gameplayFrames & 7) == 0) {
            Actor* larva = Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_PEEHAT,
                                              Rand_CenteredFloat(25.0f) + self->actor.world.pos.x,
                                              Rand_CenteredFloat(5.0f) + self->actor.world.pos.y,
                                              Rand_CenteredFloat(25.0f) + self->actor.world.pos.z, 0, 0, 0, 1);
            if (larva != NULL) {
                larva->shape.rot.y = larva->world.rot.y = Rand_CenteredFloat(0xFFFF);
                self->unk2FA++;
            }
        }
    }
    self->bladeRot += self->bladeRotVel;
}

void EnPeehat_Ground_SetStateRise(EnPeehat* self) {
    f32 lastFrame = Animation_GetLastFrame(&gPeehatRisingAnim);

    if (self->state != PEAHAT_STATE_STUNNED) {
        Animation_Change(&self->skelAnime, &gPeehatRisingAnim, 0.0f, 3.0f, lastFrame, ANIMMODE_ONCE, 0.0f);
    }
    self->state = PEAHAT_STATE_8;
    self->animTimer = lastFrame;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_UP);
    EnPeehat_SetupAction(self, EnPeehat_Ground_StateRise);
}

void EnPeehat_Ground_StateRise(EnPeehat* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.shape.yOffset, 0.0f, 1.0f, 50.0f, 0.0f);
    if (Math_SmoothStepToS(&self->bladeRotVel, 4000, 1, 800, 0) == 0) {
        if (self->animTimer != 0) {
            self->animTimer--;
            if (self->skelAnime.playSpeed == 0.0f) {
                if (self->animTimer == 0) {
                    self->animTimer = 40;
                    self->skelAnime.playSpeed = 1.0f;
                }
            }
        }
        if (SkelAnime_Update(&self->skelAnime) || self->animTimer == 0) {
            EnPeehat_Ground_SetStateHover(self);
        } else {
            self->actor.world.pos.y += 6.5f;
        }
        if (self->actor.world.pos.y - self->actor.floorHeight < 80.0f) {
            Vec3f pos = self->actor.world.pos;
            pos.y = self->actor.floorHeight;
            func_80033480(globalCtx, &pos, 90.0f, 1, 0x96, 100, 1);
        }
    }
    EnPeehat_SpawnDust(globalCtx, self, &self->actor.world.pos, 75.0f, 2, 1.05f, 2.0f);
    Math_SmoothStepToF(&self->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    self->bladeRot += self->bladeRotVel;
}

void EnPeehat_Flying_SetStateRise(EnPeehat* self) {
    f32 lastFrame;

    lastFrame = Animation_GetLastFrame(&gPeehatRisingAnim);
    if (self->state != PEAHAT_STATE_STUNNED) {
        Animation_Change(&self->skelAnime, &gPeehatRisingAnim, 0.0f, 3.0f, lastFrame, ANIMMODE_ONCE, 0.0f);
    }
    self->state = PEAHAT_STATE_9;
    self->animTimer = lastFrame;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_UP);
    EnPeehat_SetupAction(self, EnPeehat_Flying_StateRise);
}

void EnPeehat_Flying_StateRise(EnPeehat* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.shape.yOffset, 0.0f, 1.0f, 50.0f, 0.0f);
    if (Math_SmoothStepToS(&self->bladeRotVel, 4000, 1, 800, 0) == 0) {
        if (self->animTimer != 0) {
            self->animTimer--;
            if (self->skelAnime.playSpeed == 0.0f) {
                if (self->animTimer == 0) {
                    self->animTimer = 40;
                    self->skelAnime.playSpeed = 1.0f;
                }
            }
        }
        if (SkelAnime_Update(&self->skelAnime) || self->animTimer == 0) {
            //! @bug: overwrites number of child larva spawned, allowing for more than MAX_LARVA spawns
            self->unk2FA = 0;
            EnPeehat_Flying_SetStateFly(self);
        } else {
            self->actor.world.pos.y += 18.0f;
        }
        if (self->actor.world.pos.y - self->actor.floorHeight < 80.0f) {
            Vec3f pos = self->actor.world.pos;
            pos.y = self->actor.floorHeight;
            func_80033480(globalCtx, &pos, 90.0f, 1, 0x96, 100, 1);
        }
    }
    EnPeehat_SpawnDust(globalCtx, self, &self->actor.world.pos, 75.0f, 2, 1.05f, 2.0f);
    Math_SmoothStepToF(&self->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    self->bladeRot += self->bladeRotVel;
}

void EnPeehat_Ground_SetStateSeekPlayer(EnPeehat* self) {
    Animation_PlayLoop(&self->skelAnime, &gPeehatFlyingAnim);
    self->state = PEAHAT_STATE_SEEK_PLAYER;
    self->unk2E0 = 0.0f;
    EnPeehat_SetupAction(self, EnPeehat_Ground_StateSeekPlayer);
}

void EnPeehat_Ground_StateSeekPlayer(EnPeehat* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToF(&self->actor.speedXZ, 3.0f, 1.0f, 0.25f, 0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.floorHeight + 80.0f, 1.0f, 3.0f, 0.0f);
    if (self->seekPlayerTimer <= 0) {
        EnPeehat_Ground_SetStateLanding(self);
        self->riseDelayTimer = 40;
    } else {
        self->seekPlayerTimer--;
    }
    if (IS_DAY && (Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < self->xzDistMax)) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 1000, 0);
        if (self->unk2FA != 0) {
            self->actor.shape.rot.y += 0x1C2;
        } else {
            self->actor.shape.rot.y -= 0x1C2;
        }
    } else {
        EnPeehat_Ground_SetStateReturnHome(self);
    }
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->bladeRotVel, 4000, 1, 500, 0);
    self->bladeRot += self->bladeRotVel;
    Math_SmoothStepToF(&self->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
}

void EnPeehat_Larva_SetStateSeekPlayer(EnPeehat* self) {
    Animation_PlayLoop(&self->skelAnime, &gPeehatFlyingAnim);
    self->state = PEAHAT_STATE_SEEK_PLAYER;
    self->unk2D4 = 0;
    EnPeehat_SetupAction(self, EnPeehat_Larva_StateSeekPlayer);
}

void EnPeehat_Larva_StateSeekPlayer(EnPeehat* self, GlobalContext* globalCtx) {
    f32 speedXZ = 5.3f;

    if (self->actor.xzDistToPlayer <= 5.3f) {
        speedXZ = self->actor.xzDistToPlayer + 0.0005f;
    }
    if (self->actor.parent != NULL && self->actor.parent->update == NULL) {
        self->actor.parent = NULL;
    }
    self->actor.speedXZ = speedXZ;
    if (self->actor.world.pos.y - self->actor.floorHeight >= 70.0f) {
        Math_SmoothStepToF(&self->actor.velocity.y, -1.3f, 1.0f, 0.5f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->actor.velocity.y, -0.135f, 1.0f, 0.05f, 0.0f);
    }
    if (self->unk2D4 == 0) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 830, 0);
    } else {
        self->unk2D4--;
    }
    self->actor.shape.rot.y += 0x15E;
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->bladeRotVel, 4000, 1, 500, 0);
    self->bladeRot += self->bladeRotVel;
    Math_SmoothStepToF(&self->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_SM_FLY - SFX_FLAG);
    if (self->colQuad.base.atFlags & AT_BOUNCED) {
        self->actor.colChkInfo.health = 0;
        self->colQuad.base.acFlags = self->colQuad.base.acFlags & ~AC_BOUNCED;
        EnPeehat_SetStateAttackRecoil(self);
    } else if ((self->colQuad.base.atFlags & AT_HIT) || (self->colCylinder.base.acFlags & AC_HIT) ||
               (self->actor.bgCheckFlags & 1)) {
        Player* player = GET_PLAYER(globalCtx);
        self->colQuad.base.atFlags &= ~AT_HIT;
        if (!(self->colCylinder.base.acFlags & AC_HIT) && &player->actor == self->colQuad.base.at) {
            if (Rand_ZeroOne() > 0.5f) {
                self->actor.world.rot.y += 0x2000;
            } else {
                self->actor.world.rot.y -= 0x2000;
            }
            self->unk2D4 = 40;
        } else if (self->colCylinder.base.acFlags & AC_HIT || self->actor.bgCheckFlags & 1) {
            Vec3f zeroVec = { 0, 0, 0 };
            s32 i;
            for (i = 4; i >= 0; i--) {
                Vec3f pos;
                pos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
                pos.y = Rand_CenteredFloat(10.0f) + self->actor.world.pos.y;
                pos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
                EffectSsDeadDb_Spawn(globalCtx, &pos, &zeroVec, &zeroVec, 40, 7, 255, 255, 255, 255, 255, 0, 0, 1, 9,
                                     1);
            }
        }
        if (&player->actor != self->colQuad.base.at || self->colCylinder.base.acFlags & AC_HIT) {
            if (!(self->actor.bgCheckFlags & 1)) {
                EffectSsDeadSound_SpawnStationary(globalCtx, &self->actor.projectedPos, NA_SE_EN_PIHAT_SM_DEAD, 1, 1,
                                                  40);
            }
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x20);
            Actor_Kill(&self->actor);
        }
    }
}

void EnPeehat_Ground_SetStateLanding(EnPeehat* self) {
    self->state = PEAHAT_STATE_LANDING;
    Animation_PlayOnce(&self->skelAnime, &gPeehatLandingAnim);
    EnPeehat_SetupAction(self, EnPeehat_Ground_StateLanding);
}

void EnPeehat_Ground_StateLanding(EnPeehat* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.shape.yOffset, -1000.0f, 1.0f, 50.0f, 0.0f);
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);
    Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 50, 0);
    if (SkelAnime_Update(&self->skelAnime)) {
        EnPeehat_Ground_SetStateGround(self);
        self->actor.world.pos.y = self->actor.floorHeight;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_LAND);
    } else if (self->actor.floorHeight < self->actor.world.pos.y) {
        Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.floorHeight, 0.3f, 3.5f, 0.25f);
        if (self->actor.world.pos.y - self->actor.floorHeight < 60.0f) {
            Vec3f pos = self->actor.world.pos;
            pos.y = self->actor.floorHeight;
            func_80033480(globalCtx, &pos, 80.0f, 1, 150, 100, 1);
            EnPeehat_SpawnDust(globalCtx, self, &pos, 75.0f, 2, 1.05f, 2.0f);
        }
    }
    Math_SmoothStepToS(&self->bladeRotVel, 0, 1, 100, 0);
    self->bladeRot += self->bladeRotVel;
}

void EnPeehat_Flying_SetStateLanding(EnPeehat* self) {
    Animation_PlayOnce(&self->skelAnime, &gPeehatLandingAnim);
    self->state = PEAHAT_STATE_LANDING;
    EnPeehat_SetupAction(self, EnPeehat_Flying_StateLanding);
}

void EnPeehat_Flying_StateLanding(EnPeehat* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.shape.yOffset, -1000.0f, 1.0f, 50.0f, 0.0f);
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);
    Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 50, 0);
    if (SkelAnime_Update(&self->skelAnime)) {
        EnPeehat_Flying_SetStateGround(self);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_LAND);
        self->actor.world.pos.y = self->actor.floorHeight;
    } else if (self->actor.floorHeight < self->actor.world.pos.y) {
        Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.floorHeight, 0.3f, 13.5f, 0.25f);
        if (self->actor.world.pos.y - self->actor.floorHeight < 60.0f) {
            Vec3f pos = self->actor.world.pos;
            pos.y = self->actor.floorHeight;
            func_80033480(globalCtx, &pos, 80.0f, 1, 150, 100, 1);
            EnPeehat_SpawnDust(globalCtx, self, &pos, 75.0f, 2, 1.05f, 2.0f);
        }
    }
    Math_SmoothStepToS(&self->bladeRotVel, 0, 1, 100, 0);
    self->bladeRot += self->bladeRotVel;
}

void EnPeehat_Ground_SetStateHover(EnPeehat* self) {
    Animation_PlayLoop(&self->skelAnime, &gPeehatFlyingAnim);
    self->actor.speedXZ = Rand_ZeroOne() * 0.5f + 2.5f;
    self->unk2D4 = Rand_ZeroOne() * 10 + 10;
    self->state = PEAHAT_STATE_15;
    EnPeehat_SetupAction(self, EnPeehat_Ground_StateHover);
}

void EnPeehat_Ground_StateHover(EnPeehat* self, GlobalContext* globalCtx) {
    f32 cos;
    Player* player = GET_PLAYER(globalCtx);

    // hover but don't gain altitude
    if (self->actor.world.pos.y - self->actor.floorHeight > 75.0f) {
        self->actor.world.pos.y -= 1.0f;
    }
    self->actor.world.pos.y += Math_CosF(self->unk2E0) * 1.4f;
    cos = Math_CosF(self->unk2E0) * 0.18f;
    self->unk2E0 += ((0.0f <= cos) ? cos : -cos) + 0.07f;
    self->unk2D4--;
    if (self->unk2D4 <= 0) {
        self->actor.speedXZ = Rand_ZeroOne() * 0.5f + 2.5f;
        self->unk2D4 = Rand_ZeroOne() * 10.0f + 10.0f;
        self->unk2F4 = (Rand_ZeroOne() - 0.5f) * 1000.0f;
    }
    SkelAnime_Update(&self->skelAnime);
    self->actor.world.rot.y += self->unk2F4;
    if (self->seekPlayerTimer <= 0) {
        EnPeehat_Ground_SetStateLanding(self);
        self->riseDelayTimer = 40;
    } else {
        self->seekPlayerTimer--;
    }
    self->actor.shape.rot.y += 0x15E;
    // if daytime, and the player is close to the initial spawn position
    if (IS_DAY && Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < self->xzDistMax) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        EnPeehat_Ground_SetStateSeekPlayer(self);
        self->unk2FA = globalCtx->gameplayFrames & 1;
    } else {
        EnPeehat_Ground_SetStateReturnHome(self);
    }
    Math_SmoothStepToS(&self->bladeRotVel, 4000, 1, 500, 0);
    self->bladeRot += self->bladeRotVel;
    Math_SmoothStepToF(&self->scaleShift, 0.075f, 1.0f, 0.005f, 0.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
}

void EnPeehat_Ground_SetStateReturnHome(EnPeehat* self) {
    self->state = PEAHAT_STATE_RETURN_HOME;
    self->actor.speedXZ = 2.5f;
    EnPeehat_SetupAction(self, EnPeehat_Ground_StateReturnHome);
}

void EnPeehat_Ground_StateReturnHome(EnPeehat* self, GlobalContext* globalCtx) {
    f32 cos;
    s16 yRot;
    Player* player;

    player = GET_PLAYER(globalCtx);
    if (self->actor.world.pos.y - self->actor.floorHeight > 75.0f) {
        self->actor.world.pos.y -= 1.0f;
    } else {
        self->actor.world.pos.y += 1.0f;
    }
    self->actor.world.pos.y += Math_CosF(self->unk2E0) * 1.4f;
    cos = Math_CosF(self->unk2E0) * 0.18f;
    self->unk2E0 += ((0.0f <= cos) ? cos : -cos) + 0.07f;
    yRot = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
    Math_SmoothStepToS(&self->actor.world.rot.y, yRot, 1, 600, 0);
    Math_SmoothStepToS(&self->actor.shape.rot.x, 4500, 1, 600, 0);
    self->actor.shape.rot.y += 0x15E;
    self->bladeRot += self->bladeRotVel;
    if (Math_Vec3f_DistXZ(&self->actor.world.pos, &self->actor.home.pos) < 2.0f) {
        EnPeehat_Ground_SetStateLanding(self);
        self->riseDelayTimer = 60;
    }
    if (IS_DAY && Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < self->xzDistMax) {
        self->seekPlayerTimer = 400;
        EnPeehat_Ground_SetStateSeekPlayer(self);
        self->unk2FA = (globalCtx->gameplayFrames & 1);
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
}

void EnPeehat_SetStateAttackRecoil(EnPeehat* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gPeehatRecoilAnim, -4.0f);
    self->state = PEAHAT_STATE_ATTACK_RECOIL;
    self->actor.speedXZ = -9.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    EnPeehat_SetupAction(self, EnPeehat_StateAttackRecoil);
}

void EnPeehat_StateAttackRecoil(EnPeehat* self, GlobalContext* globalCtx) {
    self->bladeRot += self->bladeRotVel;
    SkelAnime_Update(&self->skelAnime);
    self->actor.speedXZ += 0.5f;
    if (self->actor.speedXZ == 0.0f) {
        // Is PEAHAT_TYPE_LARVA
        if (self->actor.params > 0) {
            Vec3f zeroVec = { 0, 0, 0 };
            s32 i;
            for (i = 4; i >= 0; i--) {
                Vec3f pos;
                pos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
                pos.y = Rand_CenteredFloat(10.0f) + self->actor.world.pos.y;
                pos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
                EffectSsDeadDb_Spawn(globalCtx, &pos, &zeroVec, &zeroVec, 40, 7, 255, 255, 255, 255, 255, 0, 0, 1, 9,
                                     1);
            }
            Actor_Kill(&self->actor);
        } else {
            EnPeehat_Ground_SetStateSeekPlayer(self);
            // Is PEAHAT_TYPE_GROUNDED
            if (self->actor.params < 0) {
                self->unk2FA = (self->unk2FA != 0) ? 0 : 1;
            }
        }
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_FLY - SFX_FLAG);
}

void EnPeehat_SetStateBoomerangStunned(EnPeehat* self) {
    self->state = PEAHAT_STATE_STUNNED;
    if (self->actor.floorHeight < self->actor.world.pos.y) {
        self->actor.speedXZ = -9.0f;
    }
    self->bladeRotVel = 0;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Actor_SetColorFilter(&self->actor, 0, 200, 0, 80);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    EnPeehat_SetupAction(self, EnPeehat_StateBoomerangStunned);
}

void EnPeehat_StateBoomerangStunned(EnPeehat* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.floorHeight, 1.0f, 8.0f, 0.0f);
    if (self->actor.colorFilterTimer == 0) {
        EnPeehat_Ground_SetStateRise(self);
    }
}

void EnPeehat_Adult_SetStateDie(EnPeehat* self) {
    self->bladeRotVel = 0;
    self->isStateDieFirstUpdate = 1;
    self->actor.speedXZ = 0.0f;
    Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 8);
    self->state = PEAHAT_STATE_DYING;
    self->scaleShift = 0.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    EnPeehat_SetupAction(self, EnPeehat_Adult_StateDie);
}

void EnPeehat_Adult_StateDie(EnPeehat* self, GlobalContext* globalCtx) {
    if (self->isStateDieFirstUpdate) {
        self->unk2D4--;
        if (self->unk2D4 <= 0 || self->actor.colChkInfo.health == 0) {
            Animation_MorphToPlayOnce(&self->skelAnime, &gPeehatRecoilAnim, -4.0f);
            self->bladeRotVel = 4000;
            self->unk2D4 = 14;
            self->actor.speedXZ = 0;
            self->actor.velocity.y = 6;
            self->isStateDieFirstUpdate = 0;
            self->actor.shape.rot.z = self->actor.shape.rot.x = 0;
        } else if (self->actor.colorFilterTimer & 4) {
            Math_SmoothStepToF(&self->scaleShift, 0.205f, 1.0f, 0.235f, 0);
        } else {
            Math_SmoothStepToF(&self->scaleShift, 0, 1.0f, 0.005f, 0);
        }
    } else {
        SkelAnime_Update(&self->skelAnime);
        self->bladeRot += self->bladeRotVel;
        Math_SmoothStepToS(&self->bladeRotVel, 4000, 1, 250, 0);
        if (self->actor.colChkInfo.health == 0) {
            self->actor.scale.x -= 0.0015f;
            Actor_SetScale(&self->actor, self->actor.scale.x);
        }
        if (Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.floorHeight + 88.5f, 1.0f, 3.0f, 0.0f) == 0.0f &&
            self->actor.world.pos.y - self->actor.floorHeight < 59.0f) {
            Vec3f pos = self->actor.world.pos;
            pos.y = self->actor.floorHeight;
            func_80033480(globalCtx, &pos, 80.0f, 1, 150, 100, 1);
            EnPeehat_SpawnDust(globalCtx, self, &pos, 75.0f, 2, 1.05f, 2.0f);
        }
        if (self->actor.speedXZ < 0) {
            self->actor.speedXZ += 0.25f;
        }
        self->unk2D4--;
        if (self->unk2D4 <= 0) {
            if (self->actor.colChkInfo.health == 0) {
                EnPeehat_SetStateExplode(self);
                // if PEAHAT_TYPE_GROUNDED
            } else if (self->actor.params < 0) {
                EnPeehat_Ground_SetStateHover(self);
                self->riseDelayTimer = 60;
            } else {
                EnPeehat_Flying_SetStateFly(self);
            }
        }
    }
}

void EnPeehat_SetStateExplode(EnPeehat* self) {
    Animation_PlayLoop(&self->skelAnime, &gPeehatFlyingAnim);
    self->state = PEAHAT_STATE_EXPLODE;
    self->animTimer = 5;
    self->unk2E0 = 0.0f;
    EnPeehat_SetupAction(self, &EnPeehat_StateExplode);
}

void EnPeehat_StateExplode(EnPeehat* self, GlobalContext* globalCtx) {
    EnBom* bomb;
    s32 pad[2];

    if (self->animTimer == 5) {
        bomb = (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, self->actor.world.pos.x,
                                   self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0x602, 0);
        if (bomb != NULL) {
            bomb->timer = 0;
        }
    }
    self->animTimer--;
    if (self->animTimer == 0) {
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x40);
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x40);
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x40);
        Actor_Kill(&self->actor);
    }
}

void EnPeehat_Adult_CollisionCheck(EnPeehat* self, GlobalContext* globalCtx) {
    if ((self->colCylinder.base.acFlags & AC_BOUNCED) || (self->colQuad.base.acFlags & AC_BOUNCED)) {
        self->colQuad.base.acFlags &= ~AC_BOUNCED;
        self->colCylinder.base.acFlags &= ~AC_BOUNCED;
        self->colJntSph.base.acFlags &= ~AC_HIT;
    } else if (self->colJntSph.base.acFlags & AC_HIT) {
        self->colJntSph.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlagJntSph(&self->actor, &self->colJntSph, 1);
        if (self->actor.colChkInfo.damageEffect == PEAHAT_DMG_EFF_NUT ||
            self->actor.colChkInfo.damageEffect == PEAHAT_DMG_EFF_LIGHT_ICE_ARROW) {
            return;
        }
        if (self->actor.colChkInfo.damageEffect == PEAHAT_DMG_EFF_HOOKSHOT) {
            self->actor.colChkInfo.health = 0;
        } else if (self->actor.colChkInfo.damageEffect == PEAHAT_DMG_EFF_BOOMERANG) {
            if (self->state != PEAHAT_STATE_STUNNED) {
                EnPeehat_SetStateBoomerangStunned(self);
            }
            return;
        } else {
            Actor_ApplyDamage(&self->actor);
            Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 8);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_PIHAT_DAMAGE);
        }

        if (self->actor.colChkInfo.damageEffect == PEAHAT_DMG_EFF_FIRE) {
            Vec3f pos;
            s32 i;
            for (i = 4; i >= 0; i--) {
                pos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
                pos.y = Rand_ZeroOne() * 25.0f + self->actor.world.pos.y;
                pos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
                EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &pos, 70, 0, 0, -1);
            }
            Actor_SetColorFilter(&self->actor, 0x4000, 200, 0, 100);
        }
        if (self->actor.colChkInfo.health == 0) {
            EnPeehat_Adult_SetStateDie(self);
        }
    }
}

void EnPeehat_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnPeehat* self = THIS;
    s32 i;
    Player* player = GET_PLAYER(globalCtx);

    // If Adult Peahat
    if (thisx->params <= 0) {
        EnPeehat_Adult_CollisionCheck(self, globalCtx);
    }
    if (thisx->colChkInfo.damageEffect != PEAHAT_DMG_EFF_LIGHT_ICE_ARROW) {
        if (thisx->speedXZ != 0.0f || thisx->velocity.y != 0.0f) {
            Actor_MoveForward(thisx);
            Actor_UpdateBgCheckInfo(globalCtx, thisx, 25.0f, 30.0f, 30.0f, 5);
        }

        self->actionFunc(self, globalCtx);
        if ((globalCtx->gameplayFrames & 0x7F) == 0) {
            self->jiggleRotInc = (Rand_ZeroOne() * 0.25f) + 0.5f;
        }
        self->jiggleRot += self->jiggleRotInc;
    }
    // if PEAHAT_TYPE_GROUNDED
    if (thisx->params < 0) {
        // Set the Z-Target point on the Peahat's weak point
        thisx->focus.pos.x = self->colJntSph.elements[0].dim.worldSphere.center.x;
        thisx->focus.pos.y = self->colJntSph.elements[0].dim.worldSphere.center.y;
        thisx->focus.pos.z = self->colJntSph.elements[0].dim.worldSphere.center.z;
        if (self->state == PEAHAT_STATE_SEEK_PLAYER) {
            Math_SmoothStepToS(&thisx->shape.rot.x, 6000, 1, 300, 0);
        } else {
            Math_SmoothStepToS(&thisx->shape.rot.x, 0, 1, 300, 0);
        }
    } else {
        thisx->focus.pos = thisx->world.pos;
    }
    Collider_UpdateCylinder(thisx, &self->colCylinder);
    if (thisx->colChkInfo.health > 0) {
        // If Adult Peahat
        if (thisx->params <= 0) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder.base);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colJntSph.base);
            if (thisx->colorFilterTimer == 0 || !(thisx->colorFilterParams & 0x4000)) {
                if (self->state != PEAHAT_STATE_EXPLODE) {
                    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colJntSph.base);
                }
            }
        }
        if (thisx->params != PEAHAT_TYPE_FLYING && self->colQuad.base.atFlags & AT_HIT) {
            self->colQuad.base.atFlags &= ~AT_HIT;
            if (&player->actor == self->colQuad.base.at) {
                EnPeehat_SetStateAttackRecoil(self);
            }
        }
    }
    if (self->state == PEAHAT_STATE_15 || self->state == PEAHAT_STATE_SEEK_PLAYER || self->state == PEAHAT_STATE_FLY ||
        self->state == PEAHAT_STATE_RETURN_HOME || self->state == PEAHAT_STATE_EXPLODE) {
        if (thisx->params != PEAHAT_TYPE_FLYING) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colQuad.base);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colQuad.base);
        }
        // if PEAHAT_TYPE_GROUNDED
        if (thisx->params < 0 && (thisx->flags & 0x40)) {
            for (i = 1; i >= 0; i--) {
                Vec3f posResult;
                CollisionPoly* poly = NULL;
                s32 bgId;
                Vec3f* posB = &self->bladeTip[i];

                if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &thisx->world.pos, posB, &posResult, &poly, true, true,
                                            false, true, &bgId) == true) {
                    func_80033480(globalCtx, &posResult, 0.0f, 1, 300, 150, 1);
                    EnPeehat_SpawnDust(globalCtx, self, &posResult, 0.0f, 3, 1.05f, 1.5f);
                }
            }
        } else if (thisx->params != PEAHAT_TYPE_FLYING) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder.base);
        }
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colCylinder.base);
    }
    Math_SmoothStepToF(&self->scaleShift, 0.0f, 1.0f, 0.001f, 0.0f);
}

s32 EnPeehat_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                              void* thisx) {
    EnPeehat* self = THIS;

    if (limbIndex == 4) {
        rot->x = -self->bladeRot;
    }
    if (limbIndex == 3 || (limbIndex == 23 && (self->state == PEAHAT_STATE_DYING || self->state == PEAHAT_STATE_3 ||
                                               self->state == PEAHAT_STATE_4))) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_peehat.c", 1946);
        Matrix_Push();
        Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
        Matrix_RotateX(self->jiggleRot * 0.115f, MTXMODE_APPLY);
        Matrix_RotateY(self->jiggleRot * 0.13f, MTXMODE_APPLY);
        Matrix_RotateZ(self->jiggleRot * 0.1f, MTXMODE_APPLY);
        Matrix_Scale(1.0f - self->scaleShift, self->scaleShift + 1.0f, 1.0f - self->scaleShift, MTXMODE_APPLY);
        Matrix_RotateZ(-(self->jiggleRot * 0.1f), MTXMODE_APPLY);
        Matrix_RotateY(-(self->jiggleRot * 0.13f), MTXMODE_APPLY);
        Matrix_RotateX(-(self->jiggleRot * 0.115f), MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_peehat.c", 1959),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, *dList);
        Matrix_Pop();
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_peehat.c", 1963);
        return true;
    }
    return false;
}

void EnPeehat_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f peahatBladeTip[] = { { 0.0f, 0.0f, 5500.0f }, { 0.0f, 0.0f, -5500.0f } };

    EnPeehat* self = THIS;
    f32 damageYRot;

    if (limbIndex == 4) {
        Matrix_MultVec3f(&peahatBladeTip[0], &self->bladeTip[0]);
        Matrix_MultVec3f(&peahatBladeTip[1], &self->bladeTip[1]);
        return;
    }
    // is Adult Peahat
    if (limbIndex == 3 && self->actor.params <= 0) {
        damageYRot = 0.0f;
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_peehat.c", 1981);
        Matrix_Push();
        Matrix_Translate(-1000.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        Collider_UpdateSpheres(0, &self->colJntSph);
        Matrix_Translate(500.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        if (self->actor.colorFilterTimer != 0 && (self->actor.colorFilterParams & 0x4000)) {
            damageYRot = Math_SinS(self->actor.colorFilterTimer * 0x4E20) * 0.35f;
        }
        Matrix_RotateY(3.2f + damageYRot, MTXMODE_APPLY);
        Matrix_Scale(0.3f, 0.2f, 0.2f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_peehat.c", 1990),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, *dList);
        Matrix_Pop();
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_peehat.c", 1994);
    }
}

void EnPeehat_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Vec3f D_80AD285C[] = {
        { 0.0f, 0.0f, -4500.0f }, { -4500.0f, 0.0f, 0.0f }, { 4500.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 4500.0f }
    };
    EnPeehat* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnPeehat_OverrideLimbDraw,
                      EnPeehat_PostLimbDraw, self);
    if (self->actor.speedXZ != 0.0f || self->actor.velocity.y != 0.0f) {
        Matrix_MultVec3f(&D_80AD285C[0], &self->colQuad.dim.quad[1]);
        Matrix_MultVec3f(&D_80AD285C[1], &self->colQuad.dim.quad[0]);
        Matrix_MultVec3f(&D_80AD285C[2], &self->colQuad.dim.quad[3]);
        Matrix_MultVec3f(&D_80AD285C[3], &self->colQuad.dim.quad[2]);
        Collider_SetQuadVertices(&self->colQuad, &self->colQuad.dim.quad[0], &self->colQuad.dim.quad[1],
                                 &self->colQuad.dim.quad[2], &self->colQuad.dim.quad[3]);
    }
}
