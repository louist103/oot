/*
 * File: z_en_tp.c
 * Overlay: ovl_En_Tp
 * Description: Electric Tailpasaran
 */

#include "z_en_tp.h"
#include "objects/object_tp/object_tp.h"

#define FLAGS 0x00000000

#define THIS ((EnTp*)thisx)

void EnTp_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnTp_Tail_SetupFollowHead(EnTp* self);
void EnTp_Tail_FollowHead(EnTp* self, GlobalContext* globalCtx);
void EnTp_Head_SetupApproachPlayer(EnTp* self);
void EnTp_Head_ApproachPlayer(EnTp* self, GlobalContext* globalCtx);
void EnTp_SetupDie(EnTp* self);
void EnTp_Die(EnTp* self, GlobalContext* globalCtx);
void EnTp_Fragment_SetupFade(EnTp* self);
void EnTp_Fragment_Fade(EnTp* self, GlobalContext* globalCtx);
void EnTp_Head_SetupTakeOff(EnTp* self);
void EnTp_Head_TakeOff(EnTp* self, GlobalContext* globalCtx);
void EnTp_Head_SetupWait(EnTp* self);
void EnTp_Head_Wait(EnTp* self, GlobalContext* globalCtx);
void EnTp_Head_SetupBurrowReturnHome(EnTp* self);
void EnTp_Head_BurrowReturnHome(EnTp* self, GlobalContext* globalCtx);

typedef enum {
    /* 0 */ TAILPASARAN_ACTION_FRAGMENT_FADE,
    /* 1 */ TAILPASARAN_ACTION_DIE,
    /* 2 */ TAILPASARAN_ACTION_TAIL_FOLLOWHEAD,
    /* 4 */ TAILPASARAN_ACTION_HEAD_WAIT = 4,
    /* 7 */ TAILPASARAN_ACTION_HEAD_APPROACHPLAYER = 7,
    /* 8 */ TAILPASARAN_ACTION_HEAD_TAKEOFF,
    /* 9 */ TAILPASARAN_ACTION_HEAD_BURROWRETURNHOME
} TailpasaranAction;

const ActorInit En_Tp_InitVars = {
    ACTOR_EN_TP,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_TP,
    sizeof(EnTp),
    (ActorFunc)EnTp_Init,
    (ActorFunc)EnTp_Destroy,
    (ActorFunc)EnTp_Update,
    (ActorFunc)EnTp_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x03, 0x08 },
            { 0xFFCFFFFF, 0x01, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 4 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT1,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

typedef enum {
    /* 00 */ TAILPASARAN_DMGEFF_NONE,
    /* 01 */ TAILPASARAN_DMGEFF_DEKUNUT,
    /* 14 */ TAILPASARAN_DMGEFF_SHOCKING = 14, // Kills the Tailpasaran but shocks Player
    /* 15 */ TAILPASARAN_DMGEFF_INSULATING     // Kills the Tailpasaran and does not shock Player
} TailpasaranDamageEffect;

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_DEKUNUT),
    /* Deku stick    */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_INSULATING),
    /* Slingshot     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(1, TAILPASARAN_DMGEFF_INSULATING),
    /* Normal arrow  */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_SHOCKING),
    /* Hookshot      */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Kokiri sword  */ DMG_ENTRY(1, TAILPASARAN_DMGEFF_SHOCKING),
    /* Master sword  */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_SHOCKING),
    /* Giant's Knife */ DMG_ENTRY(4, TAILPASARAN_DMGEFF_SHOCKING),
    /* Fire arrow    */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Ice arrow     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Light arrow   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Ice magic     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Light magic   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Shield        */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, TAILPASARAN_DMGEFF_SHOCKING),
    /* Giant spin    */ DMG_ENTRY(4, TAILPASARAN_DMGEFF_SHOCKING),
    /* Master spin   */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_SHOCKING),
    /* Kokiri jump   */ DMG_ENTRY(2, TAILPASARAN_DMGEFF_SHOCKING),
    /* Giant jump    */ DMG_ENTRY(8, TAILPASARAN_DMGEFF_SHOCKING),
    /* Master jump   */ DMG_ENTRY(4, TAILPASARAN_DMGEFF_SHOCKING),
    /* Unknown 1     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, TAILPASARAN_DMGEFF_SHOCKING),
    /* Unknown 2     */ DMG_ENTRY(0, TAILPASARAN_DMGEFF_NONE),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 10, ICHAIN_STOP),
};

void EnTp_SetupAction(EnTp* self, EnTpActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnTp_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnTp* self = THIS;
    EnTp* now;
    EnTp* next;
    s32 i;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.targetMode = 3;
    self->actor.colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.14f);
    self->unk_150 = 0;
    self->actor.colChkInfo.health = 1;
    now = self;
    self->alpha = 255;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItems);

    if (self->actor.params <= TAILPASARAN_HEAD) {
        self->actor.naviEnemyId = 0x06;
        self->timer = 0;
        self->collider.base.acFlags |= AC_HARD;
        self->collider.elements->dim.modelSphere.radius = self->collider.elements->dim.worldSphere.radius = 8;
        EnTp_Head_SetupWait(self);
        self->actor.focus.pos = self->actor.world.pos;
        self->actor.flags |= 0x15;
        Actor_SetScale(&self->actor, 1.5f);

        for (i = 0; i <= 6; i++) {
            next = (EnTp*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_TP, self->actor.world.pos.x,
                                      self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0 * i);

            if (0 * i) {} // Very fake, but needed to get the s registers right
            if (next != NULL) {
                now->actor.child = &next->actor;
                next->actor.parent = &now->actor;
                next->kiraSpawnTimer = i + 1;
                next->head = self;
                Actor_SetScale(&next->actor, 0.3f);

                if (i == 2) {
                    next->actor.flags |= 0x15;
                    next->unk_150 = 1; // Why?
                }

                next->timer = next->unk_15C = i * -5;
                next->horizontalVariation = 6.0f - (i * 0.75f);
                now = next;
                if (0 * i) {}
            }
        }
    } else if (self->actor.params == TAILPASARAN_TAIL) {
        EnTp_Tail_SetupFollowHead(self);
    } else {
        EnTp_Fragment_SetupFade(self);
    }
}

void EnTp_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnTp* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnTp_Tail_SetupFollowHead(EnTp* self) {
    self->actionIndex = TAILPASARAN_ACTION_TAIL_FOLLOWHEAD;
    EnTp_SetupAction(self, EnTp_Tail_FollowHead);
}

void EnTp_Tail_FollowHead(EnTp* self, GlobalContext* globalCtx) {
    s16 angle;
    s16 phase;

    if (self->actor.params == TAILPASARAN_TAIL_DYING) {
        self->actionIndex = TAILPASARAN_ACTION_DIE;

        if (self->actor.parent == NULL) {
            EnTp_SetupDie(self);
        }
    } else {
        if (self->unk_150 != 0) {
            self->actor.flags |= 1;
        }

        if (self->head->unk_150 != 0) {
            self->actor.speedXZ = self->red = self->actor.velocity.y = self->heightPhase = 0.0f;
            if (self->actor.world.pos.y < self->head->actor.home.pos.y) {
                self->actor.flags &= ~1;
            }

            self->actor.world.pos = self->actor.parent->prevPos;
        } else {
            Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.parent->world.pos.y - 4.0f, 1.0f, 1.0f, 0.0f);
            angle = self->head->actor.shape.rot.y + 0x4000;
            phase = 2000 * (self->head->unk_15C + self->timer);
            self->actor.world.pos.x =
                self->actor.home.pos.x + Math_SinS(phase) * (Math_SinS(angle) * self->horizontalVariation);
            self->actor.world.pos.z =
                self->actor.home.pos.z + Math_SinS(phase) * (Math_CosS(angle) * self->horizontalVariation);
        }
    }
}

void EnTp_Head_SetupApproachPlayer(EnTp* self) {
    self->actionIndex = TAILPASARAN_ACTION_HEAD_APPROACHPLAYER;
    self->timer = 200;
    EnTp_SetupAction(self, EnTp_Head_ApproachPlayer);
}

void EnTp_Head_ApproachPlayer(EnTp* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToF(&self->actor.world.pos.y, player->actor.world.pos.y + 30.0f, 1.0f, 0.5f, 0.0f);
    Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        if (&player->actor == self->collider.base.at) {
            self->timer = 1;
        }
    }

    if (self->red < 255) {
        self->red += 15;
    }

    if (Math_CosF(self->heightPhase) == 0.0f) {
        self->extraHeightVariation = 2.0f * Rand_ZeroOne();
    }

    self->actor.world.pos.y += Math_CosF(self->heightPhase) * (2.0f + self->extraHeightVariation);
    self->heightPhase += 0.2f;
    Math_SmoothStepToF(&self->actor.speedXZ, 2.5f, 0.1f, 0.2f, 0.0f);
    self->timer--;

    if (self->timer != 0) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 750, 0);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    } else {
        EnTp_Head_SetupBurrowReturnHome(self);
    }
}

void EnTp_SetupDie(EnTp* self) {
    Actor* now;

    self->timer = 2;

    if (self->actor.params <= TAILPASARAN_HEAD) {
        for (now = self->actor.child; now != NULL; now = now->child) {
            now->params = TAILPASARAN_TAIL_DYING;
            now->colChkInfo.health = 0;
        }

        self->timer = 13;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_TAIL_DEAD);
    }
    self->actionIndex = TAILPASARAN_ACTION_DIE;
    EnTp_SetupAction(self, EnTp_Die);
}

/**
 * Spawns effects and smaller tail segment-like fragments
 */
void EnTp_Die(EnTp* self, GlobalContext* globalCtx) {
    EnTp* now;
    s16 i;
    s32 pad;
    Vec3f effectVelAccel = { 0.0f, 0.5f, 0.0f };
    Vec3f effectPos = { 0.0f, 0.0f, 0.0f };

    self->timer--;

    if (self->timer <= 0) {
        if (self->actor.params == TAILPASARAN_HEAD_DYING) {
            effectPos.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + self->actor.world.pos.x;
            effectPos.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + self->actor.world.pos.z;
            effectPos.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + self->actor.world.pos.y;
            EffectSsDeadDb_Spawn(globalCtx, &effectPos, &effectVelAccel, &effectVelAccel, 100, 0, 255, 255, 255, 255, 0,
                                 0, 255, 1, 9, 1);

            effectPos.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + self->actor.world.pos.x;
            effectPos.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + self->actor.world.pos.z;
            effectPos.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + self->actor.world.pos.y;
            EffectSsDeadDb_Spawn(globalCtx, &effectPos, &effectVelAccel, &effectVelAccel, 100, 0, 255, 255, 255, 255, 0,
                                 0, 255, 1, 9, 1);
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x50);
        } else {
            for (i = 0; i < 1; i++) {
                now =
                    (EnTp*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_TP, self->actor.world.pos.x,
                                       self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, TAILPASARAN_FRAGMENT);

                if (now != NULL) {
                    Actor_SetScale(&now->actor, self->actor.scale.z * 0.5f);
                    now->red = self->red;
                }
            }
        }

        if (self->actor.child != NULL) {
            self->actor.child->parent = NULL;
            self->actor.child->params = TAILPASARAN_TAIL_DYING;
            self->actor.child->colChkInfo.health = 0;
        }

        self->unk_150 = 2;
        Actor_Kill(&self->actor);
    }
}

void EnTp_Fragment_SetupFade(EnTp* self) {
    self->actionIndex = TAILPASARAN_ACTION_FRAGMENT_FADE;
    self->actor.world.pos.x += ((Rand_ZeroOne() - 0.5f) * 5.0f);
    self->actor.world.pos.y += ((Rand_ZeroOne() - 0.5f) * 5.0f);
    self->actor.world.pos.z += ((Rand_ZeroOne() - 0.5f) * 5.0f);
    self->actor.velocity.x = (Rand_ZeroOne() - 0.5f) * 1.5f;
    self->actor.velocity.y = (Rand_ZeroOne() - 0.5f) * 1.5f;
    self->actor.velocity.z = (Rand_ZeroOne() - 0.5f) * 1.5f;
    self->actor.flags &= ~1;
    EnTp_SetupAction(self, EnTp_Fragment_Fade);
}

void EnTp_Fragment_Fade(EnTp* self, GlobalContext* globalCtx) {
    func_8002D7EC(&self->actor);
    self->alpha -= 20;

    if (self->alpha < 20) {
        self->alpha = 0;
        Actor_Kill(&self->actor);
    }
}

void EnTp_Head_SetupTakeOff(EnTp* self) {
    self->timer = (Rand_ZeroOne() * 15.0f) + 40.0f;
    self->actionIndex = TAILPASARAN_ACTION_HEAD_TAKEOFF;
    EnTp_SetupAction(self, EnTp_Head_TakeOff);
}

/**
 * Flies up and loops around until it makes for Player
 */
void EnTp_Head_TakeOff(EnTp* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToF(&self->actor.speedXZ, 2.5f, 0.1f, 0.2f, 0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.y, player->actor.world.pos.y + 85.0f + self->horizontalVariation, 1.0f,
                       self->actor.speedXZ * 0.25f, 0.0f);
    Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        if (&player->actor == self->collider.base.at) {
            self->unk_15C = 1;
        }
    }

    if (self->red != 0) {
        self->red -= 15;
    }

    if (Math_CosF(self->heightPhase) == 0.0f) {
        self->extraHeightVariation = Rand_ZeroOne() * 4.0f;
    }

    self->actor.world.pos.y +=
        Math_CosF(self->heightPhase) * ((self->actor.speedXZ * 0.25f) + self->extraHeightVariation);
    self->actor.world.rot.y += self->unk_164;
    self->heightPhase += 0.2f;

    if (self->timer != 0) {
        self->timer--;
    }

    Math_SmoothStepToS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos), 1, 750,
                       0);

    if (self->timer == 0) {
        EnTp_Head_SetupApproachPlayer(self);
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnTp_Head_SetupWait(EnTp* self) {
    self->actionIndex = TAILPASARAN_ACTION_HEAD_WAIT;
    self->unk_150 = 0;
    self->actor.shape.rot.x = -0x4000;
    self->timer = 60;
    self->unk_15C = 0;
    self->actor.speedXZ = 0.0f;
    EnTp_SetupAction(self, EnTp_Head_Wait);
}

/**
 * Awaken and rise from the ground when Player is closer than 200
 */
void EnTp_Head_Wait(EnTp* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 yaw;

    self->unk_15C--;

    if (self->actor.xzDistToPlayer < 200.0f) {
        if (self->collider.base.atFlags & AT_HIT) {
            self->collider.base.atFlags &= ~AT_HIT;
            if (&player->actor == self->collider.base.at) {
                self->timer = 0;
            }
        }

        if (self->timer != 0) {
            self->timer--;

            Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 500, 0);
            Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 1, 1500, 0);

            yaw = Math_Vec3f_Yaw(&self->actor.home.pos, &player->actor.world.pos) + 0x4000;
            Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 30.0f, 0.3f, 1.0f, 0.3f);
            self->actor.world.pos.x = self->actor.home.pos.x +
                                      (Math_SinS(2000 * self->unk_15C) * (Math_SinS(yaw) * self->horizontalVariation));
            self->actor.world.pos.z = self->actor.home.pos.z +
                                      (Math_SinS(2000 * self->unk_15C) * (Math_CosS(yaw) * self->horizontalVariation));
        } else {
            self->actor.shape.rot.x = 0;
            self->unk_150 = 1;
            EnTp_Head_SetupTakeOff(self);
        }
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.x, -0x4000, 1, 500, 0);

        if (Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.3f, 1.5f, 0.3f) == 0.0f) {
            self->timer = 60;
        } else {
            yaw = Math_Vec3f_Yaw(&self->actor.home.pos, &player->actor.world.pos);
            self->actor.world.pos.x =
                self->actor.home.pos.x + (Math_SinS(2000 * self->unk_15C) * (Math_SinS(yaw) * 6.0f));
            self->actor.world.pos.z =
                self->actor.home.pos.z + (Math_SinS(2000 * self->unk_15C) * (Math_CosS(yaw) * 6.0f));
        }
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->actor.world.pos.y != self->actor.home.pos.y) {
        Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

void EnTp_Head_SetupBurrowReturnHome(EnTp* self) {
    self->actionIndex = TAILPASARAN_ACTION_HEAD_BURROWRETURNHOME;
    self->timer = 0;
    EnTp_SetupAction(self, EnTp_Head_BurrowReturnHome);
}

void EnTp_Head_BurrowReturnHome(EnTp* self, GlobalContext* globalCtx) {
    static Vec3f bubbleAccel = { 0.0f, -0.5f, 0.0f };
    static Color_RGBA8 bubblePrimColor = { 255, 255, 255, 255 };
    static Color_RGBA8 bubbleEnvColor = { 150, 150, 150, 0 };
    Vec3f bubbleVelocity;
    Vec3f bubblePos;
    s32 closeToFloor;
    EnTp* now;
    s16 temp_v0; // Required to match, usage can maybe be improved

    closeToFloor = false;
    temp_v0 = self->timer;
    self->unk_15C--;

    if ((temp_v0 != 0) || ((self->actor.home.pos.y - self->actor.world.pos.y) > 60.0f)) {
        self->timer = temp_v0 - 1;
        temp_v0 = self->timer;

        if (temp_v0 == 0) {
            EnTp_Head_SetupWait(self);

            for (now = (EnTp*)self->actor.child; now != NULL; now = (EnTp*)now->actor.child) {
                now->unk_15C = now->timer;
            }
        } else {
            if (self->actor.shape.rot.x != -0x4000) {
                self->timer = 80;
                self->actor.velocity.y = 0.0f;
                self->actor.speedXZ = 0.0f;
                self->actor.world.pos = self->actor.home.pos;
                self->actor.shape.rot.x = -0x4000;

                for (now = (EnTp*)self->actor.child; now != NULL; now = (EnTp*)now->actor.child) {
                    now->actor.velocity.y = 0.0f;
                    now->actor.speedXZ = 0.0f;
                    now->actor.world.pos = self->actor.home.pos;
                    now->actor.world.pos.y = self->actor.home.pos.y - 80.0f;
                }
            }

            self->actor.world.pos.y = self->actor.home.pos.y - self->timer;
        }
    } else {
        if (self->actor.shape.rot.x != 0x4000) {
            self->actor.shape.rot.x -= 0x400;
        }

        if (self->red != 0) {
            self->red -= 15;
        }

        self->actor.speedXZ = 2.0f * Math_CosS(self->actor.shape.rot.x);
        self->actor.velocity.y = Math_SinS(self->actor.shape.rot.x) * -2.0f;

        if ((self->actor.world.pos.y - self->actor.floorHeight) < 20.0f) {
            closeToFloor = true;
        }

        if (self->actor.world.pos.y != self->actor.home.pos.y) {
            Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }

        if (closeToFloor && ((globalCtx->gameplayFrames & 1) != 0)) {
            bubblePos = self->actor.world.pos;
            bubblePos.y = self->actor.floorHeight;

            bubbleVelocity.x = Rand_CenteredFloat(5.0f);
            bubbleVelocity.y = (Rand_ZeroOne() * 3.5f) + 1.5f;
            bubbleVelocity.z = Rand_CenteredFloat(5.0f);

            EffectSsDtBubble_SpawnCustomColor(globalCtx, &bubblePos, &bubbleVelocity, &bubbleAccel, &bubblePrimColor,
                                              &bubbleEnvColor, Rand_S16Offset(100, 50), 20, 0);
        }
    }
}

void EnTp_UpdateDamage(EnTp* self, GlobalContext* globalCtx) {
    s32 phi_s2;
    s32 phi_s4;
    EnTp* head; // Can eliminate self and just use now, but they're used differently
    EnTp* now;

    if ((self->collider.base.acFlags & AC_HIT) && (self->actionIndex >= TAILPASARAN_ACTION_TAIL_FOLLOWHEAD)) {
        phi_s4 = phi_s2 = 0;

        if (self->actor.params <= TAILPASARAN_HEAD) {
            phi_s2 = 1;
        }

        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlagJntSph(&self->actor, &self->collider, 1);
        self->damageEffect = self->actor.colChkInfo.damageEffect;

        if (self->actor.colChkInfo.damageEffect != TAILPASARAN_DMGEFF_NONE) {
            if (self->actor.colChkInfo.damageEffect == TAILPASARAN_DMGEFF_DEKUNUT) {
                phi_s4 = 1;
            }

            // Head is invincible
            if (phi_s2 == 0) {
                Actor_ApplyDamage(&self->actor);
            }

            if (self->actor.colChkInfo.health == 0) {
                self->actor.flags &= ~1;
                head = self->head;

                if (head->actor.params <= TAILPASARAN_HEAD) {
                    EnTp_SetupDie(head);
                    head->damageEffect = self->actor.colChkInfo.damageEffect;
                    head->actor.params = TAILPASARAN_HEAD_DYING;
                }
            } else {
                if (phi_s4 != 0) {
                    self->actor.freezeTimer = 80;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
                    if (phi_s2 != 0) {
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
                    } else {
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0x2000, 0x50);
                    }
                }

                for (now = (EnTp*)self->actor.parent; now != NULL; now = (EnTp*)now->actor.parent) {
                    now->collider.base.acFlags &= ~AC_HIT;

                    if (phi_s4 != 0) {
                        now->actor.freezeTimer = 80;
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);

                        if (phi_s2 != 0) {
                            Actor_SetColorFilter(&now->actor, 0, 0xFF, 0, 0x50);
                        } else {
                            Actor_SetColorFilter(&now->actor, 0, 0xFF, 0x2000, 0x50);
                        }
                    }
                }

                for (now = (EnTp*)self->actor.child; now != NULL; now = (EnTp*)now->actor.child) {
                    now->collider.base.acFlags &= ~AC_HIT;
                    if (phi_s4 != 0) {
                        now->actor.freezeTimer = 80;

                        if (phi_s2 != 0) {
                            Actor_SetColorFilter(&now->actor, 0, 0xFF, 0, 0x50);
                        } else {
                            Actor_SetColorFilter(&now->actor, 0, 0xFF, 0x2000, 0x50);
                        }
                    }
                }
            }
        }
    }
}

void EnTp_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnTp* self = THIS;
    Vec3f kiraVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f kiraAccel = { 0.0f, -0.6f, 0.0f };
    Vec3f kiraPos;
    Color_RGBA8 kiraPrimColor = { 0, 0, 255, 255 };
    Color_RGBA8 kiraEnvColor = { 0, 0, 0, 0 };
    Player* player = GET_PLAYER(globalCtx);
    s16 yawToWall;

    if (player->stateFlags1 & 0x4000000) { // Shielding
        self->damageEffect = TAILPASARAN_DMGEFF_NONE;
    }

    if (self->actor.colChkInfo.health != 0) {
        EnTp_UpdateDamage(self, globalCtx);
    }

    self->actionFunc(self, globalCtx);

    if (self->actor.params <= TAILPASARAN_HEAD) {
        Actor_MoveForward(&self->actor);

        if (self->actionIndex != TAILPASARAN_ACTION_HEAD_BURROWRETURNHOME) {
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 15.0f, 10.0f, 5);
        }

        // Turn away from wall
        if ((self->actor.speedXZ != 0.0f) && (self->actor.bgCheckFlags & 8)) {
            yawToWall = self->actor.wallYaw - self->actor.world.rot.y;

            if (ABS(yawToWall) > 0x4000) {
                if (yawToWall >= 0) {
                    self->actor.world.rot.y -= 500;
                } else {
                    self->actor.world.rot.y += 500;
                }

                self->actor.shape.rot.y = self->actor.world.rot.y;
            }
        }

        self->actor.shape.rot.z += 0x800;

        if (self->actor.shape.rot.z == 0) {
            Audio_PlaySoundGeneral(NA_SE_EN_TAIL_CRY, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }

        if (self->actionIndex >= TAILPASARAN_ACTION_TAIL_FOLLOWHEAD) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }

    if (self->actor.params != TAILPASARAN_TAIL_DYING) {
        self->kiraSpawnTimer--;
        self->kiraSpawnTimer &= 7;
    }

    self->actor.focus.pos = self->actor.world.pos;

    if (self->damageEffect == TAILPASARAN_DMGEFF_SHOCKING) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if ((self->kiraSpawnTimer & 7) == 0) {
        kiraPrimColor.r = self->red;
        kiraAccel.x = -self->actor.velocity.x * 0.25f;
        kiraAccel.y = -self->actor.velocity.y * 0.25f;
        kiraAccel.z = -self->actor.velocity.z * 0.25f;
        kiraPos.x = ((Rand_ZeroOne() - 0.5f) * 25.0f) + self->actor.world.pos.x;
        kiraPos.y = ((Rand_ZeroOne() - 0.5f) * 20.0f) + self->actor.world.pos.y;
        kiraPos.z = ((Rand_ZeroOne() - 0.5f) * 25.0f) + self->actor.world.pos.z;
        EffectSsKiraKira_SpawnSmall(globalCtx, &kiraPos, &kiraVelocity, &kiraAccel, &kiraPrimColor, &kiraEnvColor);
    }

    if ((self->actionIndex >= TAILPASARAN_ACTION_TAIL_FOLLOWHEAD) && (self->actor.colChkInfo.health != 0)) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void EnTp_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnTp* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tp.c", 1451);

    if (self->unk_150 != 2) {
        if ((thisx->params <= TAILPASARAN_HEAD) || (thisx->params == TAILPASARAN_HEAD_DYING)) {
            func_80093D18(globalCtx->state.gfxCtx);

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_tp.c", 1459),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gTailpasaranHeadDL);

            Matrix_Translate(0.0f, 0.0f, 8.0f, MTXMODE_APPLY);
        } else {
            func_80093D84(globalCtx->state.gfxCtx);
            func_800D1FD4(&globalCtx->mf_11DA0);

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, self->red, 0, 255, self->alpha);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT,
                              TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT,
                              TEXEL0, ENVIRONMENT);
            gDPPipeSync(POLY_XLU_DISP++);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gTailpasaranTailSegmentTex));
            gDPPipeSync(POLY_XLU_DISP++);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_tp.c", 1480),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gTailpasaranTailSegmentDL);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tp.c", 1495);

    if ((thisx->params <= TAILPASARAN_TAIL) || (thisx->params == TAILPASARAN_TAIL_DYING)) {
        Collider_UpdateSpheres(0, &self->collider);
    }
}
