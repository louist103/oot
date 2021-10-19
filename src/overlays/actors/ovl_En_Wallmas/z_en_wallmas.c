/*
 * File: z_en_wallmas
 * Overlay: En_Wallmas
 * Description: Wallmaster (Ceiling monster)
 */

#include "z_en_wallmas.h"
#include "objects/object_wallmaster/object_wallmaster.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000015

#define THIS ((EnWallmas*)thisx)

#define TIMER_SCALE ((f32)OS_CLOCK_RATE / 10000000000)
#define DEGREE_60_RAD (60.0f * M_PI / 180.0f)
#define DEGREE_15_RAD (15.0f * M_PI / 180.0f)

#define DAMAGE_EFFECT_BURN 2
#define DAMAGE_EFFECT_STUN_WHITE 4
#define DAMAGE_EFFECT_STUN_BLUE 1

void EnWallmas_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWallmas_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWallmas_Update(Actor* thisx, GlobalContext* globalCtx);
void EnWallmas_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnWallmas_TimerInit(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_ProximityOrSwitchInit(EnWallmas* self);
void EnWallmas_WaitToDrop(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_Drop(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_Land(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_Stand(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_JumpToCeiling(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_ReturnToCeiling(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_TakeDamage(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_Cooldown(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_Die(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_TakePlayer(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_WaitForProximity(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_WaitForSwitchFlag(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_Stun(EnWallmas* self, GlobalContext* globalCtx);
void EnWallmas_Walk(EnWallmas* self, GlobalContext* globalCtx);

const ActorInit En_Wallmas_InitVars = {
    ACTOR_EN_WALLMAS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_WALLMASTER,
    sizeof(EnWallmas),
    (ActorFunc)EnWallmas_Init,
    (ActorFunc)EnWallmas_Destroy,
    (ActorFunc)EnWallmas_Update,
    (ActorFunc)EnWallmas_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
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
    { 30, 40, 0, { 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 4, 30, 40, 150 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x1),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(4, 0x2),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(4, 0x4),
    /* Unk arrow 1   */ DMG_ENTRY(4, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(4, 0x4),
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
    ICHAIN_S8(naviEnemyId, 0x30, 1),
    ICHAIN_F32(targetArrowOffset, 5500, 1),
    ICHAIN_F32_DIV1000(gravity, -1500, 0),
};

void EnWallmas_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnWallmas* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    ActorShape_Init(&thisx->shape, 0, NULL, 0.5f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gWallmasterSkel, &gWallmasterWaitAnim, self->jointTable,
                       self->morphTable, 25);

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, thisx, &sCylinderInit);
    CollisionCheck_SetInfo(&thisx->colChkInfo, &sDamageTable, &sColChkInfoInit);
    self->switchFlag = (u8)(thisx->params >> 0x8);
    thisx->params = thisx->params & 0xFF;

    if (thisx->params == WMT_FLAG) {
        if (Flags_GetSwitch(globalCtx, self->switchFlag) != 0) {
            Actor_Kill(thisx);
            return;
        }

        EnWallmas_ProximityOrSwitchInit(self);
    } else if (thisx->params == WMT_PROXIMITY) {
        EnWallmas_ProximityOrSwitchInit(self);
    } else {
        EnWallmas_TimerInit(self, globalCtx);
    }
}

void EnWallmas_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnWallmas* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnWallmas_TimerInit(EnWallmas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->actor.flags &= ~1;
    self->actor.flags |= 0x20;
    self->timer = 0x82;
    self->actor.velocity.y = 0.0f;
    self->actor.world.pos.y = player->actor.world.pos.y;
    self->actor.floorHeight = player->actor.floorHeight;
    self->actor.draw = EnWallmas_Draw;
    self->actionFunc = EnWallmas_WaitToDrop;
}

void EnWallmas_SetupDrop(EnWallmas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    AnimationHeader* objSegChangee = &gWallmasterLungeAnim;

    Animation_Change(&self->skelAnime, objSegChangee, 0.0f, 20.0f, Animation_GetLastFrame(&gWallmasterLungeAnim),
                     ANIMMODE_ONCE, 0.0f);

    self->yTarget = player->actor.world.pos.y;
    self->actor.world.pos.y = player->actor.world.pos.y + 300.0f;
    self->actor.world.rot.y = player->actor.shape.rot.y + 0x8000;
    self->actor.floorHeight = player->actor.floorHeight;
    self->actor.flags |= 1;
    self->actor.flags &= ~0x20;
    self->actionFunc = EnWallmas_Drop;
}

void EnWallmas_SetupLand(EnWallmas* self, GlobalContext* globalCtx) {
    AnimationHeader* objSegFrameCount = &gWallmasterJumpAnim;
    AnimationHeader* objSegChangee = &gWallmasterJumpAnim;

    Animation_Change(&self->skelAnime, objSegChangee, 1.0f, 41.0f, Animation_GetLastFrame(objSegFrameCount),
                     ANIMMODE_ONCE, -3.0f);

    Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 15.0f, 6, 20.0f, 0x12C, 0x64, 1);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_LAND);
    self->actionFunc = EnWallmas_Land;
}

void EnWallmas_SetupStand(EnWallmas* self) {
    Animation_PlayOnce(&self->skelAnime, &gWallmasterStandUpAnim);
    self->actionFunc = EnWallmas_Stand;
}

void EnWallmas_SetupWalk(EnWallmas* self) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, &gWallmasterWalkAnim, 3.0f);
    self->actionFunc = EnWallmas_Walk;
    self->actor.speedXZ = 3.0f;
}

void EnWallmas_SetupJumpToCeiling(EnWallmas* self) {
    Animation_PlayOnce(&self->skelAnime, &gWallmasterStopWalkAnim);
    self->actionFunc = EnWallmas_JumpToCeiling;
    self->actor.speedXZ = 0.0f;
}
void EnWallmas_SetupReturnToCeiling(EnWallmas* self) {
    AnimationHeader* objSegFrameCount = &gWallmasterJumpAnim;
    AnimationHeader* objSegChangee = &gWallmasterJumpAnim;

    self->timer = 0;
    self->actor.speedXZ = 0.0f;

    Animation_Change(&self->skelAnime, objSegChangee, 3.0f, 0.0f, Animation_GetLastFrame(objSegFrameCount),
                     ANIMMODE_ONCE, -3.0f);

    self->actionFunc = EnWallmas_ReturnToCeiling;
}

void EnWallmas_SetupTakeDamage(EnWallmas* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gWallmasterDamageAnim, -3.0f);
    if (self->collider.info.acHitInfo->toucher.dmgFlags & 0x0001F824) {
        self->actor.world.rot.y = self->collider.base.ac->world.rot.y;
    } else {
        self->actor.world.rot.y = Actor_WorldYawTowardActor(&self->actor, self->collider.base.ac) + 0x8000;
    }

    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0x14);
    self->actionFunc = EnWallmas_TakeDamage;
    self->actor.speedXZ = 5.0f;
    self->actor.velocity.y = 10.0f;
}

void EnWallmas_SetupCooldown(EnWallmas* self) {
    Animation_PlayOnce(&self->skelAnime, &gWallmasterRecoverFromDamageAnim);
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actionFunc = EnWallmas_Cooldown;
}

void EnWallmas_SetupDie(EnWallmas* self, GlobalContext* globalCtx) {
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;

    EffectSsDeadDb_Spawn(globalCtx, &self->actor.world.pos, &zeroVec, &zeroVec, 250, -10, 255, 255, 255, 255, 0, 0, 255,
                         1, 9, true);

    Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xC0);
    self->actionFunc = EnWallmas_Die;
}

void EnWallmas_SetupTakePlayer(EnWallmas* self, GlobalContext* globalCtx) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gWallmasterHoverAnim, -5.0f);
    self->timer = -0x1E;
    self->actionFunc = EnWallmas_TakePlayer;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;

    self->yTarget = self->actor.yDistToPlayer;
    func_8002DF38(globalCtx, &self->actor, 0x25);
    OnePointCutscene_Init(globalCtx, 9500, 9999, &self->actor, MAIN_CAM);
}

void EnWallmas_ProximityOrSwitchInit(EnWallmas* self) {
    self->timer = 0;
    self->actor.draw = NULL;
    self->actor.flags &= ~1;
    if (self->actor.params == WMT_PROXIMITY) {
        self->actionFunc = EnWallmas_WaitForProximity;
    } else {
        self->actionFunc = EnWallmas_WaitForSwitchFlag;
    }
}

void EnWallmas_SetupStun(EnWallmas* self) {
    Animation_Change(&self->skelAnime, &gWallmasterJumpAnim, 1.5f, 0, 20.0f, ANIMMODE_ONCE, -3.0f);

    self->actor.speedXZ = 0.0f;
    if (self->actor.colChkInfo.damageEffect == 4) {
        Actor_SetColorFilter(&self->actor, -0x8000, 0xFF, 0, 0x50);
    } else {
        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    }

    self->timer = 0x50;
    self->actionFunc = EnWallmas_Stun;
}

void EnWallmas_WaitToDrop(EnWallmas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f* playerPos = &player->actor.world.pos;

    self->actor.world.pos = *playerPos;
    self->actor.floorHeight = player->actor.floorHeight;
    self->actor.floorPoly = player->actor.floorPoly;

    if (self->timer != 0) {
        self->timer--;
    }

    if ((player->stateFlags1 & 0x100000) || (player->stateFlags1 & 0x8000000) || !(player->actor.bgCheckFlags & 1) ||
        ((self->actor.params == 1) && (320.0f < Math_Vec3f_DistXZ(&self->actor.home.pos, playerPos)))) {
        Audio_StopSfx(NA_SE_EN_FALL_AIM);
        self->timer = 0x82;
    }

    if (self->timer == 0x50) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_AIM);
    }

    if (self->timer == 0) {
        EnWallmas_SetupDrop(self, globalCtx);
    }
}

void EnWallmas_Drop(EnWallmas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (!Player_InCsMode(globalCtx) && !(player->stateFlags2 & 0x10) && (player->invincibilityTimer >= 0) &&
        (self->actor.xzDistToPlayer < 30.0f) && (self->actor.yDistToPlayer < -5.0f) &&
        (-(f32)(player->cylinder.dim.height + 10) < self->actor.yDistToPlayer)) {
        EnWallmas_SetupTakePlayer(self, globalCtx);
    }
}

void EnWallmas_Land(EnWallmas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        EnWallmas_SetupStand(self);
    }
}

void EnWallmas_Stand(EnWallmas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        EnWallmas_SetupWalk(self);
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x8000, 0xB6);
}

void EnWallmas_Walk(EnWallmas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        EnWallmas_SetupJumpToCeiling(self);
    }

    Math_ScaledStepToS(&self->actor.world.rot.y, (s16)((s32)self->actor.yawTowardsPlayer + 0x8000), 0xB6);

    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 12.0f) ||
        Animation_OnFrame(&self->skelAnime, 24.0f) || Animation_OnFrame(&self->skelAnime, 36.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_WALK);
    }
}

void EnWallmas_JumpToCeiling(EnWallmas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        EnWallmas_SetupReturnToCeiling(self);
    }
}

void EnWallmas_ReturnToCeiling(EnWallmas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    SkelAnime_Update(&self->skelAnime);
    if (self->skelAnime.curFrame > 20.0f) {
        self->timer += 9;
        self->actor.world.pos.y = self->actor.world.pos.y + 30.0f;
    }

    if (Animation_OnFrame(&self->skelAnime, 20.0f) != 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_UP);
    }

    if (self->actor.yDistToPlayer < -900.0f) {
        if (self->actor.params == WMT_FLAG) {
            Actor_Kill(&self->actor);
            return;
        }

        if (self->actor.params == WMT_TIMER ||
            Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < 200.0f) {
            EnWallmas_TimerInit(self, globalCtx);
        } else {
            EnWallmas_ProximityOrSwitchInit(self);
        }
    }
}

void EnWallmas_TakeDamage(EnWallmas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        if (self->actor.colChkInfo.health == 0) {
            EnWallmas_SetupDie(self, globalCtx);
        } else {
            EnWallmas_SetupCooldown(self);
        }
    }
    if (Animation_OnFrame(&self->skelAnime, 13.0f) != 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
    }

    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.2f);
}

void EnWallmas_Cooldown(EnWallmas* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        EnWallmas_SetupReturnToCeiling(self);
    }
}

void EnWallmas_Die(EnWallmas* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->actor.scale.x, 0.0f, 0.0015) != 0) {
        Actor_SetScale(&self->actor, 0.01f);
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xC0);
        Actor_Kill(&self->actor);
    }
    self->actor.scale.z = self->actor.scale.x;
    self->actor.scale.y = self->actor.scale.x;
}

void EnWallmas_TakePlayer(EnWallmas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (Animation_OnFrame(&self->skelAnime, 1.0f) != 0) {
        if (!LINK_IS_ADULT) {
            func_8002F7DC(&self->actor, NA_SE_VO_LI_DAMAGE_S_KID);
        } else {
            func_8002F7DC(&self->actor, NA_SE_VO_LI_DAMAGE_S);
        }

        Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_CATCH);
    }
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        player->actor.world.pos.x = self->actor.world.pos.x;
        player->actor.world.pos.z = self->actor.world.pos.z;

        if (self->timer < 0) {
            self->actor.world.pos.y = self->actor.world.pos.y + 2.0f;
        } else {
            self->actor.world.pos.y = self->actor.world.pos.y + 10.0f;
        }

        if (!LINK_IS_ADULT) {
            player->actor.world.pos.y = self->actor.world.pos.y - 30.0f;
        } else {
            player->actor.world.pos.y = self->actor.world.pos.y - 50.0f;
        }

        if (self->timer == -0x1E) {
            if (!LINK_IS_ADULT) {
                func_8002F7DC(&self->actor, NA_SE_VO_LI_TAKEN_AWAY_KID);
            } else {
                func_8002F7DC(&self->actor, NA_SE_VO_LI_TAKEN_AWAY);
            }
        }
        if (self->timer == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_UP);
        }

        self->timer = self->timer + 2;
    } else {
        Math_StepToF(&self->actor.world.pos.y, player->actor.world.pos.y + (!LINK_IS_ADULT ? 30.0f : 50.0f), 5.0f);
    }

    Math_StepToF(&self->actor.world.pos.x, player->actor.world.pos.x, 3.0f);
    Math_StepToF(&self->actor.world.pos.z, player->actor.world.pos.z, 3.0f);

    if (self->timer == 0x1E) {
        func_80078884(NA_SE_OC_ABYSS);
        Gameplay_TriggerRespawn(globalCtx);
    }
}

void EnWallmas_WaitForProximity(EnWallmas* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    if (Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < 200.0f) {
        EnWallmas_TimerInit(self, globalCtx);
    }
}

void EnWallmas_WaitForSwitchFlag(EnWallmas* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag) != 0) {
        EnWallmas_TimerInit(self, globalCtx);
        self->timer = 0x51;
    }
}

void EnWallmas_Stun(EnWallmas* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        if (self->actor.colChkInfo.health == 0) {
            EnWallmas_SetupDie(self, globalCtx);
        } else {
            EnWallmas_SetupReturnToCeiling(self);
        }
    }
}

void EnWallmas_ColUpdate(EnWallmas* self, GlobalContext* globalCtx) {
    if ((self->collider.base.acFlags & AC_HIT) != 0) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);
        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_DEAD);
                self->actor.flags &= ~1;
            } else {
                if (self->actor.colChkInfo.damage != 0) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FALL_DAMAGE);
                }
            }

            if ((self->actor.colChkInfo.damageEffect == DAMAGE_EFFECT_STUN_WHITE) ||
                (self->actor.colChkInfo.damageEffect == DAMAGE_EFFECT_STUN_BLUE)) {
                if (self->actionFunc != EnWallmas_Stun) {
                    EnWallmas_SetupStun(self);
                }
            } else {
                if (self->actor.colChkInfo.damageEffect == DAMAGE_EFFECT_BURN) {
                    EffectSsFCircle_Spawn(globalCtx, &self->actor, &self->actor.world.pos, 40, 40);
                }

                EnWallmas_SetupTakeDamage(self);
            }
        }
    }
}

void EnWallmas_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnWallmas* self = THIS;
    char pad[4];

    EnWallmas_ColUpdate(self, globalCtx);
    self->actionFunc(self, globalCtx);

    if ((self->actionFunc == EnWallmas_WaitToDrop) || (self->actionFunc == EnWallmas_WaitForProximity) ||
        (self->actionFunc == EnWallmas_TakePlayer) || (self->actionFunc == EnWallmas_WaitForSwitchFlag)) {
        return;
    }

    if ((self->actionFunc != EnWallmas_ReturnToCeiling) && (self->actionFunc != EnWallmas_TakePlayer)) {
        Actor_MoveForward(&self->actor);
    }

    if (self->actionFunc != EnWallmas_Drop) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 25.0f, 0.0f, 0x1D);
    } else if (self->actor.world.pos.y <= self->yTarget) {
        self->actor.world.pos.y = self->yTarget;
        self->actor.velocity.y = 0.0f;
        EnWallmas_SetupLand(self, globalCtx);
    }

    if ((self->actionFunc != EnWallmas_Die) && (self->actionFunc != EnWallmas_Drop)) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

        if ((self->actionFunc != EnWallmas_TakeDamage) && (self->actor.bgCheckFlags & 1) != 0 &&
            (self->actor.freezeTimer == 0)) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }

    Actor_SetFocus(&self->actor, 25.0f);

    if (self->actionFunc == EnWallmas_TakeDamage) {
        return;
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnWallmas_DrawXlu(EnWallmas* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 xzScale;
    MtxF mf;

    if ((self->actor.floorPoly == NULL) || ((self->timer >= 0x51) && (self->actionFunc != EnWallmas_Stun))) {
        return;
    }

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_wallmas.c", 1386);

    func_80094044(globalCtx->state.gfxCtx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, 255);

    func_80038A28(self->actor.floorPoly, self->actor.world.pos.x, self->actor.floorHeight, self->actor.world.pos.z,
                  &mf);
    Matrix_Mult(&mf, MTXMODE_NEW);

    if ((self->actionFunc != EnWallmas_WaitToDrop) && (self->actionFunc != EnWallmas_ReturnToCeiling) &&
        (self->actionFunc != EnWallmas_TakePlayer) && (self->actionFunc != EnWallmas_WaitForSwitchFlag)) {
        xzScale = self->actor.scale.x * 50.0f;
    } else {
        xzScale = ((0x50 - self->timer) >= 0x51 ? 0x50 : (0x50 - self->timer)) * TIMER_SCALE;
    }

    Matrix_Scale(xzScale, 1.0f, xzScale, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_wallmas.c", 1421), G_MTX_LOAD);
    gSPDisplayList(POLY_XLU_DISP++, &gCircleShadowDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_wallmas.c", 1426);
}

s32 EnWallMas_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                               void* thisx) {
    EnWallmas* self = THIS;

    if (limbIndex == 1) {
        if (self->actionFunc != EnWallmas_TakePlayer) {
            pos->z -= 1600.0f;
        } else {
            pos->z -= ((1600.0f * (self->skelAnime.endFrame - self->skelAnime.curFrame)) / self->skelAnime.endFrame);
        }
    }
    return false;
}

void EnWallMas_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    if (limbIndex == 2) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_wallmas.c", 1478);

        Matrix_Push();
        Matrix_Translate(1600.0f, -700.0f, -1700.0f, MTXMODE_APPLY);
        Matrix_RotateY(DEGREE_60_RAD, MTXMODE_APPLY);
        Matrix_RotateZ(DEGREE_15_RAD, MTXMODE_APPLY);
        Matrix_Scale(2.0f, 2.0f, 2.0f, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_wallmas.c", 1489), G_MTX_LOAD);
        gSPDisplayList(POLY_OPA_DISP++, gWallmasterFingerDL);

        Matrix_Pop();

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_wallmas.c", 1495);
    }
}

void EnWallmas_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnWallmas* self = THIS;

    if (self->actionFunc != EnWallmas_WaitToDrop) {
        func_80093D18(globalCtx->state.gfxCtx);
        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnWallMas_OverrideLimbDraw, EnWallMas_PostLimbDraw, self);
    }

    EnWallmas_DrawXlu(self, globalCtx);
}
