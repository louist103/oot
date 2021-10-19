#include "z_en_dh.h"
#include "objects/object_dh/object_dh.h"

#define FLAGS 0x00000415

#define THIS ((EnDh*)thisx)

typedef enum {
    /* 0 */ DH_WAIT,
    /* 1 */ DH_RETREAT,
    /* 2 */ DH_BURROW,
    /* 3 */ DH_WALK,
    /* 4 */ DH_ATTACK,
    /* 5 */ DH_DEATH,
    /* 6 */ DH_DAMAGE
} EnDhAction;

void EnDh_Init(Actor* self, GlobalContext* globalCtx);
void EnDh_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDh_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDh_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDh_SetupWait(EnDh* self);
void EnDh_SetupWalk(EnDh* self);
void EnDh_SetupAttack(EnDh* self);
void EnDh_SetupBurrow(EnDh* self);

void EnDh_Wait(EnDh* self, GlobalContext* globalCtx);
void EnDh_Walk(EnDh* self, GlobalContext* globalCtx);
void EnDh_Retreat(EnDh* self, GlobalContext* globalCtx);
void EnDh_Attack(EnDh* self, GlobalContext* globalCtx);
void EnDh_Burrow(EnDh* self, GlobalContext* globalCtx);
void EnDh_Damage(EnDh* self, GlobalContext* globalCtx);
void EnDh_Death(EnDh* self, GlobalContext* globalCtx);

const ActorInit En_Dh_InitVars = {
    ACTOR_EN_DH,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DH,
    sizeof(EnDh),
    (ActorFunc)EnDh_Init,
    (ActorFunc)EnDh_Destroy,
    (ActorFunc)EnDh_Update,
    (ActorFunc)EnDh_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000008, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 35, 70, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON | OCELEM_UNK3,
        },
        { 1, { { 0, 0, 0 }, 20 }, 100 },
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
    sJntSphElementsInit,
};

static DamageTable D_809EC620 = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(2, 0xF),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(2, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
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
    /* Kokiri spin   */ DMG_ENTRY(2, 0xF),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(4, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x2F, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3500, ICHAIN_STOP),
};

void EnDh_SetupAction(EnDh* self, EnDhActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnDh_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDh* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.colChkInfo.damageTable = &D_809EC620;
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_dh_Skel_007E88, &object_dh_Anim_005880, self->jointTable,
                       self->limbRotTable, 16);
    ActorShape_Init(&self->actor.shape, 0.0f, &ActorShadow_DrawCircle, 64.0f);
    self->actor.params = ENDH_WAIT_UNDERGROUND;
    self->actor.colChkInfo.mass = MASS_HEAVY;
    self->actor.colChkInfo.health = LINK_IS_ADULT ? 14 : 20;
    self->alpha = self->unk_258 = 255;
    self->actor.flags &= ~1;
    Collider_InitCylinder(globalCtx, &self->collider1);
    Collider_SetCylinder(globalCtx, &self->collider1, &self->actor, &sCylinderInit);
    Collider_InitJntSph(globalCtx, &self->collider2);
    Collider_SetJntSph(globalCtx, &self->collider2, &self->actor, &sJntSphInit, self->elements);
    EnDh_SetupWait(self);
}

void EnDh_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDh* self = THIS;

    func_800F5B58();
    Collider_DestroyCylinder(globalCtx, &self->collider1);
    Collider_DestroyJntSph(globalCtx, &self->collider2);
}

void EnDh_SpawnDebris(GlobalContext* globalCtx, EnDh* self, Vec3f* spawnPos, f32 spread, s32 arg4, f32 accelXZ,
                      f32 scale) {
    Vec3f pos;
    Vec3f vel = { 0.0f, 8.0f, 0.0f };
    Vec3f accel = { 0.0f, -1.5f, 0.0f };
    f32 spreadAngle;
    f32 scaleMod;

    spreadAngle = (Rand_ZeroOne() - 0.5f) * 6.28f;
    pos.y = self->actor.floorHeight;
    pos.x = (Math_SinF(spreadAngle) * spread) + spawnPos->x;
    pos.z = (Math_CosF(spreadAngle) * spread) + spawnPos->z;
    accel.x = (Rand_ZeroOne() - 0.5f) * accelXZ;
    accel.z = (Rand_ZeroOne() - 0.5f) * accelXZ;
    vel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    scaleMod = (Rand_ZeroOne() * 5.0f) + 12.0f;
    EffectSsHahen_Spawn(globalCtx, &pos, &vel, &accel, arg4, scaleMod * scale, -1, 10, NULL);
}

void EnDh_SetupWait(EnDh* self) {
    Animation_PlayLoop(&self->skelAnime, &object_dh_Anim_003A8C);
    self->curAction = DH_WAIT;
    self->actor.world.pos.x = Rand_CenteredFloat(600.0f) + self->actor.home.pos.x;
    self->actor.world.pos.z = Rand_CenteredFloat(600.0f) + self->actor.home.pos.z;
    self->actor.shape.yOffset = -15000.0f;
    self->dirtWaveSpread = self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.flags |= 0x80;
    self->dirtWavePhase = self->actionState = self->actor.params = ENDH_WAIT_UNDERGROUND;
    EnDh_SetupAction(self, EnDh_Wait);
}

void EnDh_Wait(EnDh* self, GlobalContext* globalCtx) {
    if ((s32)self->skelAnime.curFrame == 5) {
        func_800F5ACC(0x38);
    }
    if (Actor_GetCollidedExplosive(globalCtx, &self->collider1.base)) {
        self->actor.params = ENDH_START_ATTACK_BOMB;
    }
    if ((self->actor.params >= ENDH_START_ATTACK_GRAB) || (self->actor.params <= ENDH_HANDS_KILLED_4)) {
        switch (self->actionState) {
            case 0:
                self->actor.flags |= 1;
                self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
                self->actor.flags &= ~0x80;
                self->actionState++;
                self->drawDirtWave++;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_HIDE);
            case 1:
                self->dirtWavePhase += 0x3A7;
                Math_SmoothStepToF(&self->dirtWaveSpread, 300.0f, 1.0f, 5.0f, 0.0f);
                self->dirtWaveHeight = Math_SinS(self->dirtWavePhase) * 55.0f;
                self->dirtWaveAlpha = (s16)(Math_SinS(self->dirtWavePhase) * 255.0f);
                EnDh_SpawnDebris(globalCtx, self, &self->actor.world.pos, self->dirtWaveSpread, 4, 2.05f, 1.2f);
                if (self->actor.shape.yOffset == 0.0f) {
                    self->drawDirtWave = false;
                    self->actionState++;
                } else if (self->dirtWavePhase > 0x12C0) {
                    self->actor.shape.yOffset += 500.0f;
                }
                break;
            case 2:
                EnDh_SetupWalk(self);
                break;
        }
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x7D0, 0);
        SkelAnime_Update(&self->skelAnime);
        if (self->actor.params != ENDH_START_ATTACK_BOMB) {
            func_8008EEAC(globalCtx, &self->actor);
        }
    }
}

void EnDh_SetupWalk(EnDh* self) {
    Animation_Change(&self->skelAnime, &object_dh_Anim_003A8C, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_dh_Anim_003A8C) - 3.0f, ANIMMODE_LOOP, -6.0f);
    self->curAction = DH_WALK;
    self->timer = 300;
    self->actor.speedXZ = 1.0f;
    EnDh_SetupAction(self, EnDh_Walk);
}

void EnDh_Walk(EnDh* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    SkelAnime_Update(&self->skelAnime);
    if (((s32)self->skelAnime.curFrame % 8) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_WALK);
    }
    if ((globalCtx->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_LAUGH);
    }
    if (self->actor.xzDistToPlayer <= 100.0f) {
        self->actor.speedXZ = 0.0f;
        if (Actor_IsFacingPlayer(&self->actor, 60 * 0x10000 / 360)) {
            EnDh_SetupAttack(self);
        }
    } else if (--self->timer == 0) {
        EnDh_SetupBurrow(self);
    }
}

void EnDh_SetupRetreat(EnDh* self, GlobalContext* globalCtx) {
    Animation_MorphToLoop(&self->skelAnime, &object_dh_Anim_005880, -4.0f);
    self->curAction = DH_RETREAT;
    self->timer = 70;
    self->actor.speedXZ = 1.0f;
    EnDh_SetupAction(self, EnDh_Retreat);
}

void EnDh_Retreat(EnDh* self, GlobalContext* globalCtx) {
    self->timer--;
    if (self->timer == 0) {
        self->retreat = false;
        EnDh_SetupBurrow(self);
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, (s16)(self->actor.yawTowardsPlayer + 0x8000), 1, 0xBB8, 0);
    }
    self->actor.world.rot.y = self->actor.shape.rot.y;
    SkelAnime_Update(&self->skelAnime);
}

void EnDh_SetupAttack(EnDh* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_dh_Anim_004658, -6.0f);
    self->timer = self->actionState = 0;
    self->curAction = DH_ATTACK;
    self->actor.speedXZ = 0.0f;
    EnDh_SetupAction(self, EnDh_Attack);
}

void EnDh_Attack(EnDh* self, GlobalContext* globalCtx) {
    s32 pad;

    if (SkelAnime_Update(&self->skelAnime)) {
        self->actionState++;
    } else if ((self->actor.xzDistToPlayer > 100.0f) || !Actor_IsFacingPlayer(&self->actor, 60 * 0x10000 / 360)) {
        Animation_Change(&self->skelAnime, &object_dh_Anim_004658, -1.0f, self->skelAnime.curFrame, 0.0f, ANIMMODE_ONCE,
                         -4.0f);
        self->actionState = 4;
        self->collider2.base.atFlags = self->collider2.elements[0].info.toucherFlags = AT_NONE; // also TOUCH_NONE
        self->collider2.elements[0].info.toucher.dmgFlags = self->collider2.elements[0].info.toucher.damage = 0;
    }
    switch (self->actionState) {
        case 1:
            Animation_PlayOnce(&self->skelAnime, &object_dh_Anim_001A3C);
            self->actionState++;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_BITE);
        case 0:
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x5DC, 0);
            break;
        case 2:
            if (self->skelAnime.curFrame >= 4.0f) {
                self->collider2.base.atFlags = self->collider2.elements[0].info.toucherFlags =
                    AT_ON | AT_TYPE_ENEMY; // also TOUCH_ON | TOUCH_SFX_WOOD
                self->collider2.elements[0].info.toucher.dmgFlags = 0xFFCFFFFF;
                self->collider2.elements[0].info.toucher.damage = 8;
            }
            if (self->collider2.base.atFlags & AT_BOUNCED) {
                self->collider2.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
                self->collider2.base.atFlags = self->collider2.elements[0].info.toucherFlags =
                    AT_NONE; // also TOUCH_NONE
                self->collider2.elements[0].info.toucher.dmgFlags = self->collider2.elements[0].info.toucher.damage = 0;
                self->actionState++;
            } else if (self->collider2.base.atFlags & AT_HIT) {
                self->collider2.base.atFlags &= ~AT_HIT;
                func_8002F71C(globalCtx, &self->actor, 8.0f, self->actor.shape.rot.y, 8.0f);
            }
            break;
        case 3:
            if ((self->actor.xzDistToPlayer <= 100.0f) && (Actor_IsFacingPlayer(&self->actor, 60 * 0x10000 / 360))) {
                Animation_Change(&self->skelAnime, &object_dh_Anim_004658, 1.0f, 20.0f,
                                 Animation_GetLastFrame(&object_dh_Anim_004658), ANIMMODE_ONCE, -6.0f);
                self->actionState = 0;
            } else {
                Animation_Change(&self->skelAnime, &object_dh_Anim_004658, -1.0f,
                                 Animation_GetLastFrame(&object_dh_Anim_004658), 0.0f, ANIMMODE_ONCE, -4.0f);
                self->actionState++;
                self->collider2.base.atFlags = self->collider2.elements[0].info.toucherFlags =
                    AT_NONE; // also TOUCH_NONE
                self->collider2.elements[0].info.toucher.dmgFlags = self->collider2.elements[0].info.toucher.damage = 0;
            }
            break;
        case 5:
            EnDh_SetupWalk(self);
            break;
        case 4:
            break;
    }
    self->actor.world.rot.y = self->actor.shape.rot.y;
}

void EnDh_SetupBurrow(EnDh* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_dh_Anim_002148, -6.0f);
    self->curAction = DH_BURROW;
    self->dirtWaveSpread = self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->dirtWavePhase = 0;
    self->actionState = 0;
    self->actor.flags &= ~1;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_HIDE);
    EnDh_SetupAction(self, EnDh_Burrow);
}

void EnDh_Burrow(EnDh* self, GlobalContext* globalCtx) {
    switch (self->actionState) {
        case 0:
            self->actionState++;
            self->drawDirtWave++;
            self->collider1.base.atFlags = self->collider1.info.toucherFlags =
                AT_ON | AT_TYPE_ENEMY; // also TOUCH_ON | TOUCH_SFX_WOOD
            self->collider1.info.toucher.dmgFlags = 0xFFCFFFFF;
            self->collider1.info.toucher.damage = 4;
        case 1:
            self->dirtWavePhase += 0x47E;
            Math_SmoothStepToF(&self->dirtWaveSpread, 300.0f, 1.0f, 8.0f, 0.0f);
            self->dirtWaveHeight = Math_SinS(self->dirtWavePhase) * 55.0f;
            self->dirtWaveAlpha = (s16)(Math_SinS(self->dirtWavePhase) * 255.0f);
            EnDh_SpawnDebris(globalCtx, self, &self->actor.world.pos, self->dirtWaveSpread, 4, 2.05f, 1.2f);
            self->collider1.dim.radius = self->dirtWaveSpread * 0.6f;
            if (SkelAnime_Update(&self->skelAnime)) {
                self->actionState++;
            }
            break;
        case 2:
            self->drawDirtWave = false;
            self->collider1.dim.radius = 35;
            self->collider1.base.atFlags = self->collider1.info.toucherFlags = AT_NONE; // Also TOUCH_NONE
            self->collider1.info.toucher.dmgFlags = self->collider1.info.toucher.damage = 0;
            EnDh_SetupWait(self);
            break;
    }
}

void EnDh_SetupDamage(EnDh* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_dh_Anim_003D6C, -6.0f);
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = -1.0f;
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_DAMAGE);
    self->curAction = DH_DAMAGE;
    EnDh_SetupAction(self, EnDh_Damage);
}

void EnDh_Damage(EnDh* self, GlobalContext* globalCtx) {
    if (self->actor.speedXZ < 0.0f) {
        self->actor.speedXZ += 0.15f;
    }
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    if (SkelAnime_Update(&self->skelAnime)) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
        if (self->retreat) {
            EnDh_SetupRetreat(self, globalCtx);
        } else if ((self->actor.xzDistToPlayer <= 105.0f) && Actor_IsFacingPlayer(&self->actor, 60 * 0x10000 / 360)) {
            f32 frames = Animation_GetLastFrame(&object_dh_Anim_004658);

            EnDh_SetupAttack(self);
            Animation_Change(&self->skelAnime, &object_dh_Anim_004658, 1.0f, 20.0f, frames, ANIMMODE_ONCE, -6.0f);
        } else {
            EnDh_SetupWalk(self);
        }
        self->unk_258 = 255;
    }
}

void EnDh_SetupDeath(EnDh* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_dh_Anim_0032BC, -1.0f);
    self->curAction = DH_DEATH;
    self->timer = 300;
    self->actor.flags &= ~1;
    self->actor.speedXZ = 0.0f;
    func_800F5B58();
    self->actor.params = ENDH_DEATH;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEADHAND_DEAD);
    EnDh_SetupAction(self, EnDh_Death);
}

void EnDh_Death(EnDh* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) || (self->timer != 300)) {
        if (self->timer == 300) {
            Animation_PlayLoop(&self->skelAnime, &object_dh_Anim_00375C);
        }
        self->timer--;
        if (self->timer < 150) {
            if (self->alpha != 0) {
                self->actor.scale.y -= 0.000075f;
                self->actor.shape.shadowAlpha = self->alpha -= 5;
            } else {
                Actor_Kill(&self->actor);
                return;
            }
        }
    } else {
        if (((s32)self->skelAnime.curFrame == 53) || ((s32)self->skelAnime.curFrame == 56) ||
            ((s32)self->skelAnime.curFrame == 61)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_DOWN);
        }
        if ((s32)self->skelAnime.curFrame == 61) {
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
        }
    }
}

void EnDh_CollisionCheck(EnDh* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 lastHealth;

    if ((self->collider2.base.acFlags & AC_HIT) && !self->retreat) {
        self->collider2.base.acFlags &= ~AC_HIT;
        if ((self->actor.colChkInfo.damageEffect != 0) && (self->actor.colChkInfo.damageEffect != 6)) {
            self->collider2.base.atFlags = self->collider2.elements[0].info.toucherFlags = AT_NONE; // also TOUCH_NONE
            self->collider2.elements[0].info.toucher.dmgFlags = self->collider2.elements[0].info.toucher.damage = 0;
            if (player->unk_844 != 0) {
                self->unk_258 = player->unk_845;
            }
            Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
            lastHealth = self->actor.colChkInfo.health;
            if (Actor_ApplyDamage(&self->actor) == 0) {
                EnDh_SetupDeath(self);
                Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x90);
            } else {
                if (((lastHealth >= 15) && (self->actor.colChkInfo.health < 15)) ||
                    ((lastHealth >= 9) && (self->actor.colChkInfo.health < 9)) ||
                    ((lastHealth >= 3) && (self->actor.colChkInfo.health < 3))) {

                    self->retreat++;
                }
                EnDh_SetupDamage(self);
            }
        }
    }
}

void EnDh_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDh* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad40;

    EnDh_CollisionCheck(self, globalCtx);
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 45.0f, 45.0f, 0x1D);
    self->actor.focus.pos = self->headPos;
    Collider_UpdateCylinder(&self->actor, &self->collider1);
    if (self->actor.colChkInfo.health > 0) {
        if (self->curAction == DH_WAIT) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider1.base);
        } else {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider1.base);
        }
        if (((self->curAction != DH_DAMAGE) && (self->actor.shape.yOffset == 0.0f)) ||
            ((player->unk_844 != 0) && (player->unk_845 != self->unk_258))) {

            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider2.base);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider2.base);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider1.base);
        }
    } else {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider1.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider2.base);
    }
}

void EnDh_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    Vec3f headOffset = { 2000.0f, 1000.0f, 0.0f };
    EnDh* self = THIS;

    if (limbIndex == 13) {
        Matrix_MultVec3f(&headOffset, &self->headPos);
        Matrix_Push();
        Matrix_Translate(headOffset.x, headOffset.y, headOffset.z, MTXMODE_APPLY);
        Collider_UpdateSpheres(1, &self->collider2);
        Matrix_Pop();
    }
}

void EnDh_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDh* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dh.c", 1099);
    if (self->alpha == 255) {
        func_80093D18(globalCtx->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, self->alpha);
        gSPSegment(POLY_OPA_DISP++, 0x08, &D_80116280[2]);
        POLY_OPA_DISP =
            SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                               self->skelAnime.dListCount, NULL, EnDh_PostLimbDraw, &self->actor, POLY_OPA_DISP);
    } else {
        func_80093D84(globalCtx->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->alpha);
        gSPSegment(POLY_XLU_DISP++, 0x08, &D_80116280[0]);
        POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           self->skelAnime.dListCount, NULL, NULL, &self->actor, POLY_XLU_DISP);
    }
    if (self->drawDirtWave) {
        func_80093D84(globalCtx->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 85, 55, 0, 130);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (globalCtx->state.frames * -3) % 0x80, 0, 0x20, 0x40, 1,
                                    (globalCtx->state.frames * -10) % 0x80, (globalCtx->state.frames * -20) % 0x100,
                                    0x20, 0x40));
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 0, 0, 0, self->dirtWaveAlpha);

        Matrix_Translate(0.0f, -self->actor.shape.yOffset, 0.0f, MTXMODE_APPLY);
        Matrix_Scale(self->dirtWaveSpread * 0.01f, self->dirtWaveHeight * 0.01f, self->dirtWaveSpread * 0.01f,
                     MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dh.c", 1160),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, object_dh_DL_007FC0);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dh.c", 1166);
}
