/*
 * File: z_en_bb.c
 * Overlay: ovl_En_Bb
 * Description: Bubble (Flying Skull Enemy)
 */

#include "z_en_bb.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_Bb/object_Bb.h"

#define FLAGS 0x01000015

#define THIS ((EnBb*)thisx)

#define vBombHopPhase actionVar1
#define vTrailIdx actionVar1
#define vTrailMaxAlpha actionVar2
#define vMoveAngleY actionVar2
#define vFlameTimer actionVar2

typedef enum {
    /* 0 */ BB_DAMAGE,
    /* 1 */ BB_KILL,
    /* 2 */ BB_FLAME_TRAIL,
    /* 3 */ BB_DOWN,
    /* 4 */ BB_STUNNED,
    /* 5 */ BB_UNUSED,
    /* 6 */ BB_BLUE,
    /* 7 */ BB_RED,
    /* 8 */ BB_WHITE,
    /* 9 */ BB_GREEN
} EnBbAction;

typedef enum {
    /* 0 */ BBMOVE_NORMAL,
    /* 1 */ BBMOVE_NOCLIP,
    /* 2 */ BBMOVE_HIDDEN
} EnBbMoveMode;

typedef enum {
    /* 0 */ BBBLUE_NORMAL,
    /* 1 */ BBBLUE_AGGRO
} EnBbBlueActionState;

typedef enum {
    /* 0 */ BBRED_WAIT,
    /* 1 */ BBRED_ATTACK,
    /* 2 */ BBRED_HIDE
} EnBbRedActionState;

typedef enum {
    /* 0 */ BBGREEN_FLAME_ON,
    /* 1 */ BBGREEN_FLAME_OFF
} EnBbGreenActionState;

// Main functions

void EnBb_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBb_Update(Actor* self, GlobalContext* globalCtx);
void EnBb_Draw(Actor* self, GlobalContext* globalCtx);

// Helper functions

void EnBb_FaceWaypoint(EnBb* self);
void EnBb_SetWaypoint(EnBb* self, GlobalContext* globalCtx);

// Action functions

void EnBb_SetupFlameTrail(EnBb* self);
void EnBb_FlameTrail(EnBb* self, GlobalContext* globalCtx);

void EnBb_SetupDeath(EnBb* self, GlobalContext* globalCtx);
void EnBb_Death(EnBb* self, GlobalContext* globalCtx);

void EnBb_Damage(EnBb* self, GlobalContext* globalCtx);

void EnBb_SetupBlue(EnBb* self);
void EnBb_Blue(EnBb* self, GlobalContext* globalCtx);

void EnBb_SetupDown(EnBb* self);
void EnBb_Down(EnBb* self, GlobalContext* globalCtx);

void EnBb_SetupRed(GlobalContext* globalCtx, EnBb* self);
void EnBb_Red(EnBb* self, GlobalContext* globalCtx);

void EnBb_SetupWhite(GlobalContext* globalCtx, EnBb* self);
void EnBb_White(EnBb* self, GlobalContext* globalCtx);

void EnBb_InitGreen(EnBb* self, GlobalContext* globalCtx);
void EnBb_Green(EnBb* self, GlobalContext* globalCtx);

void EnBb_Stunned(EnBb* self, GlobalContext* globalCtx);

static DamageTable sDamageTableBlueGreen = {
    /* Deku nut      */ DMG_ENTRY(0, 0xF),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0xA),
    /* Boomerang     */ DMG_ENTRY(0, 0xF),
    /* Normal arrow  */ DMG_ENTRY(2, 0xE),
    /* Hammer swing  */ DMG_ENTRY(2, 0xA),
    /* Hookshot      */ DMG_ENTRY(0, 0xF),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(2, 0xE),
    /* Ice arrow     */ DMG_ENTRY(4, 0xC),
    /* Light arrow   */ DMG_ENTRY(4, 0xB),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x6),
    /* Ice magic     */ DMG_ENTRY(3, 0x9),
    /* Light magic   */ DMG_ENTRY(3, 0x8),
    /* Shield        */ DMG_ENTRY(0, 0xA),
    /* Mirror Ray    */ DMG_ENTRY(0, 0xA),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0x0),
    /* Master spin   */ DMG_ENTRY(2, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0x0),
    /* Master jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x6),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0xA),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static DamageTable sDamageTableRed = {
    /* Deku nut      */ DMG_ENTRY(0, 0xD),
    /* Deku stick    */ DMG_ENTRY(0, 0xD),
    /* Slingshot     */ DMG_ENTRY(0, 0xD),
    /* Explosive     */ DMG_ENTRY(2, 0xA),
    /* Boomerang     */ DMG_ENTRY(0, 0xD),
    /* Normal arrow  */ DMG_ENTRY(2, 0xE),
    /* Hammer swing  */ DMG_ENTRY(2, 0xA),
    /* Hookshot      */ DMG_ENTRY(0, 0xD),
    /* Kokiri sword  */ DMG_ENTRY(0, 0xD),
    /* Master sword  */ DMG_ENTRY(2, 0xE),
    /* Giant's Knife */ DMG_ENTRY(4, 0xE),
    /* Fire arrow    */ DMG_ENTRY(2, 0xE),
    /* Ice arrow     */ DMG_ENTRY(4, 0x9),
    /* Light arrow   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 1   */ DMG_ENTRY(4, 0xE),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0xE),
    /* Fire magic    */ DMG_ENTRY(0, 0x6),
    /* Ice magic     */ DMG_ENTRY(3, 0x9),
    /* Light magic   */ DMG_ENTRY(0, 0x6),
    /* Shield        */ DMG_ENTRY(0, 0xA),
    /* Mirror Ray    */ DMG_ENTRY(0, 0xA),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0xE),
    /* Master spin   */ DMG_ENTRY(2, 0xE),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0xE),
    /* Master jump   */ DMG_ENTRY(4, 0xE),
    /* Unknown 1     */ DMG_ENTRY(0, 0x6),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0xA),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static DamageTable sDamageTableWhite = {
    /* Deku nut      */ DMG_ENTRY(0, 0xF),
    /* Deku stick    */ DMG_ENTRY(2, 0xE),
    /* Slingshot     */ DMG_ENTRY(1, 0xE),
    /* Explosive     */ DMG_ENTRY(2, 0xA),
    /* Boomerang     */ DMG_ENTRY(0, 0xF),
    /* Normal arrow  */ DMG_ENTRY(2, 0xE),
    /* Hammer swing  */ DMG_ENTRY(2, 0xA),
    /* Hookshot      */ DMG_ENTRY(0, 0xF),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xE),
    /* Master sword  */ DMG_ENTRY(2, 0xE),
    /* Giant's Knife */ DMG_ENTRY(4, 0xE),
    /* Fire arrow    */ DMG_ENTRY(4, 0x5),
    /* Ice arrow     */ DMG_ENTRY(2, 0xE),
    /* Light arrow   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 1   */ DMG_ENTRY(4, 0xE),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0xE),
    /* Fire magic    */ DMG_ENTRY(4, 0x7),
    /* Ice magic     */ DMG_ENTRY(0, 0x6),
    /* Light magic   */ DMG_ENTRY(0, 0x6),
    /* Shield        */ DMG_ENTRY(0, 0xA),
    /* Mirror Ray    */ DMG_ENTRY(0, 0xA),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xE),
    /* Giant spin    */ DMG_ENTRY(4, 0xE),
    /* Master spin   */ DMG_ENTRY(2, 0xE),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xE),
    /* Giant jump    */ DMG_ENTRY(8, 0xE),
    /* Master jump   */ DMG_ENTRY(4, 0xE),
    /* Unknown 1     */ DMG_ENTRY(0, 0x6),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0xA),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

const ActorInit En_Bb_InitVars = {
    ACTOR_EN_BB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BB,
    sizeof(EnBb),
    (ActorFunc)EnBb_Init,
    (ActorFunc)EnBb_Destroy,
    (ActorFunc)EnBb_Update,
    (ActorFunc)EnBb_Draw,
};

static ColliderJntSphElementInit sJntSphElementInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, -120, 0 }, 4 }, 300 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 10, ICHAIN_STOP),
};

void EnBb_SetupAction(EnBb* self, EnBbActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

Actor* EnBb_FindExplosive(GlobalContext* globalCtx, EnBb* self, f32 range) {
    Actor* explosive = globalCtx->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].head;
    f32 dist;

    while (explosive != NULL) {
        if (explosive->params != 0) {
            explosive = explosive->next;
            continue;
        }
        dist = Actor_WorldDistXYZToActor(&self->actor, explosive);
        if ((explosive->params == 0) && (dist <= range)) {
            return explosive;
        }
        explosive = explosive->next;
    }
    return NULL;
}

void EnBb_SpawnFlameTrail(GlobalContext* globalCtx, EnBb* self, s16 startAtZero) {
    EnBb* now = self;
    EnBb* next;
    s32 i;

    for (i = 0; i < 5; i++) {
        next = (EnBb*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BB, self->actor.world.pos.x,
                                  self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
        if (next != NULL) {
            now->actor.child = &next->actor;
            next->actor.parent = &now->actor;
            next->targetActor = &self->actor;
            next->vTrailIdx = i + 1;
            next->actor.scale.x = 1.0f;
            next->vTrailMaxAlpha = next->flamePrimAlpha = 255 - (i * 40);
            next->flameScaleY = next->actor.scale.y = 0.8f - (i * 0.075f);
            next->flameScaleX = next->actor.scale.z = 1.0f - (i * 0.094f);
            if (startAtZero) {
                next->flamePrimAlpha = 0;
                next->flameScaleY = next->flameScaleX = 0.0f;
            }
            next->flameScrollMod = i + 1;
            next->timer = 2 * i + 2;
            next->flameEnvColor.r = 255;
            now = next;
        }
    }
}

void EnBb_KillFlameTrail(EnBb* self) {
    Actor* actor = &self->actor;

    while (actor->child != NULL) {
        Actor* nextActor = actor->child;

        if (nextActor->id == ACTOR_EN_BB) {
            nextActor->parent = NULL;
            actor->child = NULL;
            nextActor->params = ENBB_KILL_TRAIL;
        }
        actor = nextActor;
    }
    self->actor.child = NULL;
}

void EnBb_Init(Actor* thisx, GlobalContext* globalCtx) {
    EffectBlureInit1 blureInit;
    s32 pad;
    EnBb* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    SkelAnime_Init(globalCtx, &self->skelAnime, &object_Bb_Skel_001A30, &object_Bb_Anim_000444, self->jointTable,
                   self->morphTable, 16);
    self->unk_254 = 0;
    thisx->colChkInfo.health = 4;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, thisx, &sJntSphInit, self->elements);

    self->actionState = thisx->params >> 8;

    if (thisx->params & 0x80) {
        thisx->params |= 0xFF00;
    }
    if (thisx->params <= ENBB_BLUE) {
        ActorShape_Init(&thisx->shape, 200.0f, ActorShadow_DrawCircle, 35.0f);
    }
    if (thisx->params & 0xFF00) {
        self->timer = 0;
        self->flameScaleY = 80.0f;
        self->flameScaleX = 100.0f;
        self->collider.elements[0].info.toucherFlags = TOUCH_ON | TOUCH_SFX_HARD;
        self->collider.elements[0].info.toucher.dmgFlags = 0xFFCFFFFF;
        self->collider.elements[0].info.toucher.damage = 8;
        self->bobSize = self->actionState * 20.0f;
        self->flamePrimAlpha = 255;
        self->moveMode = BBMOVE_NORMAL;
        Actor_SetScale(thisx, 0.01f);
        switch (thisx->params) {
            case ENBB_BLUE:
                thisx->naviEnemyId = 0x1C;
                thisx->colChkInfo.damageTable = &sDamageTableBlueGreen;
                self->flamePrimBlue = self->flameEnvColor.b = 255;
                thisx->world.pos.y += 50.0f;
                EnBb_SetupBlue(self);
                thisx->flags |= 0x4000;
                break;
            case ENBB_RED:
                thisx->naviEnemyId = 0x24;
                thisx->colChkInfo.damageTable = &sDamageTableRed;
                self->flameEnvColor.r = 255;
                self->collider.elements[0].info.toucher.effect = 1;
                EnBb_SetupRed(globalCtx, self);
                break;
            case ENBB_WHITE:
                thisx->naviEnemyId = 0x1D;
                thisx->colChkInfo.damageTable = &sDamageTableWhite;
                self->path = self->actionState;
                blureInit.p1StartColor[0] = blureInit.p1StartColor[1] = blureInit.p1StartColor[2] =
                    blureInit.p1StartColor[3] = blureInit.p2StartColor[0] = blureInit.p2StartColor[1] =
                        blureInit.p2StartColor[2] = blureInit.p2StartColor[3] = blureInit.p1EndColor[0] =
                            blureInit.p1EndColor[1] = blureInit.p1EndColor[2] = blureInit.p2EndColor[0] =
                                blureInit.p2EndColor[1] = blureInit.p2EndColor[2] = 255;

                blureInit.p1EndColor[3] = 0;
                blureInit.p2EndColor[3] = 0;
                blureInit.elemDuration = 16;
                blureInit.unkFlag = 0;
                blureInit.calcMode = 2;

                Effect_Add(globalCtx, &self->blureIdx, EFFECT_BLURE1, 0, 0, &blureInit);
                EnBb_SetupWhite(globalCtx, self);
                EnBb_SetWaypoint(self, globalCtx);
                EnBb_FaceWaypoint(self);
                thisx->flags |= 0x4000;
                break;
            case ENBB_GREEN_BIG:
                self->path = self->actionState >> 4;
                self->collider.elements[0].dim.modelSphere.radius = 0x16;
                Actor_SetScale(thisx, 0.03f);
            case ENBB_GREEN:
                thisx->naviEnemyId = 0x1E;
                self->bobSize = (self->actionState & 0xF) * 20.0f;
                thisx->colChkInfo.damageTable = &sDamageTableBlueGreen;
                self->flameEnvColor.g = 255;
                thisx->colChkInfo.health = 1;

                EnBb_InitGreen(self, globalCtx);
                break;
        }
        thisx->focus.pos = thisx->world.pos;
    } else {
        EnBb_SetupFlameTrail(self);
    }
    self->collider.elements[0].dim.worldSphere.radius =
        self->collider.elements[0].dim.modelSphere.radius * self->collider.elements[0].dim.scale;
}

void EnBb_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnBb* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnBb_SetupFlameTrail(EnBb* self) {
    self->action = BB_FLAME_TRAIL;
    self->moveMode = BBMOVE_NOCLIP;
    self->actor.flags &= ~1;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = 0.0f;
    self->actor.speedXZ = 0.0f;
    EnBb_SetupAction(self, EnBb_FlameTrail);
}

void EnBb_FlameTrail(EnBb* self, GlobalContext* globalCtx) {
    if (self->actor.params == ENBB_KILL_TRAIL) {
        if (self->actor.parent == NULL) {
            EnBb_SetupDeath(self, globalCtx);
        }
    } else {
        if (self->timer == 0) {
            if (((EnBb*)self->targetActor)->flameScaleY != 0.0f) {
                Math_SmoothStepToF(&self->flameScaleY, self->actor.scale.y, 1.0f, self->actor.scale.y * 0.1f, 0.0f);
                Math_SmoothStepToF(&self->flameScaleX, self->actor.scale.z, 1.0f, self->actor.scale.z * 0.1f, 0.0f);
                if (self->flamePrimAlpha != self->vTrailMaxAlpha) {
                    self->flamePrimAlpha += 10;
                    if (self->vTrailMaxAlpha < self->flamePrimAlpha) {
                        self->flamePrimAlpha = self->vTrailMaxAlpha;
                    }
                }
            } else {
                if (!self->flamePrimAlpha) {
                    Actor_Kill(&self->actor);
                    return;
                } else if (self->flamePrimAlpha <= 20) {
                    self->flamePrimAlpha = 0;
                } else {
                    self->flamePrimAlpha -= 20;
                }
            }
            self->actor.world.pos = self->actor.parent->prevPos;
        } else {
            self->timer--;
            self->actor.shape.rot.y = self->actor.world.rot.y = self->actor.parent->world.rot.y;
        }
    }
    if (self->actor.parent != NULL) {
        self->actor.velocity.y = self->actor.parent->velocity.y;
    }
}

void EnBb_SetupDeath(EnBb* self, GlobalContext* globalCtx) {
    if (self->actor.params <= ENBB_BLUE) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.speedXZ = -7.0f;
        self->timer = 5;
        self->actor.shape.rot.x += 0x4E20;
        EffectSsDeadSound_SpawnStationary(globalCtx, &self->actor.projectedPos, NA_SE_EN_BUBLE_DEAD, 1, 1, 0x28);
    }
    self->action = BB_KILL;
    EnBb_SetupAction(self, EnBb_Death);
}

void EnBb_Death(EnBb* self, GlobalContext* globalCtx) {
    s16 enpartType = 3;
    Vec3f sp40 = { 0.0f, 0.5f, 0.0f };
    Vec3f sp34 = { 0.0f, 0.0f, 0.0f };

    if (self->actor.params <= ENBB_BLUE) {
        Math_SmoothStepToF(&self->flameScaleY, 0.0f, 1.0f, 30.0f, 0.0f);
        Math_SmoothStepToF(&self->flameScaleX, 0.0f, 1.0f, 30.0f, 0.0f);
        if (self->timer != 0) {
            self->timer--;
            self->actor.shape.rot.x -= 0x4E20;
            return;
        }

        if (self->bodyBreak.val == BODYBREAK_STATUS_FINISHED) {
            BodyBreak_Alloc(&self->bodyBreak, 12, globalCtx);
        }

        if ((self->dmgEffect == 7) || (self->dmgEffect == 5)) {
            enpartType = 11;
        }

        if (!BodyBreak_SpawnParts(&self->actor, &self->bodyBreak, globalCtx, enpartType)) {
            return;
        }
        Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xD0);
    } else {
        if (self->flamePrimAlpha) {
            if (self->flamePrimAlpha <= 20) {
                self->flamePrimAlpha = 0;
            } else {
                self->flamePrimAlpha -= 20;
            }
            return;
        }
    }
    Actor_Kill(&self->actor);
}

void EnBb_SetupDamage(EnBb* self) {
    self->action = BB_DAMAGE;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_DAMAGE);
    if (self->actor.params > ENBB_GREEN) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        if ((self->actor.bgCheckFlags & 8) == 0) {
            self->actor.speedXZ = -7.0f;
        }
        self->actor.shape.yOffset = 1500.0f;
    }
    if (self->actor.params == ENBB_RED) {
        EnBb_KillFlameTrail(self);
    }
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0xC);
    self->timer = 5;
    EnBb_SetupAction(self, EnBb_Damage);
}

void EnBb_Damage(EnBb* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f);
    if (self->actor.speedXZ == 0.0f) {
        self->actor.shape.yOffset = 200.0f;
        EnBb_SetupDown(self);
    }
}

void EnBb_SetupBlue(EnBb* self) {
    Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000444);
    self->actor.speedXZ = (Rand_ZeroOne() * 0.5f) + 0.5f;
    self->timer = (Rand_ZeroOne() * 20.0f) + 40.0f;
    self->unk_264 = (Rand_ZeroOne() * 30.0f) + 180.0f;
    self->targetActor = NULL;
    self->action = BB_BLUE;
    EnBb_SetupAction(self, EnBb_Blue);
}

void EnBb_Blue(EnBb* self, GlobalContext* globalCtx) {
    Actor* explosive;
    s16 moveYawToWall;
    s16 thisYawToWall;
    s16 afterHitAngle;

    Math_SmoothStepToF(&self->flameScaleY, 80.0f, 1.0f, 10.0f, 0.0f);
    Math_SmoothStepToF(&self->flameScaleX, 100.0f, 1.0f, 10.0f, 0.0f);
    if (self->actor.floorHeight > BGCHECK_Y_MIN) {
        Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.floorHeight + 50.0f + self->flyHeightMod, 1.0f, 0.5f,
                           0.0f);
    }
    SkelAnime_Update(&self->skelAnime);
    if (Math_CosF(self->bobPhase) == 0.0f) {
        if (self->charge) {
            self->bobSpeedMod = Rand_ZeroOne() * 2.0f;
        } else {
            self->bobSpeedMod = Rand_ZeroOne() * 4.0f;
        }
    }
    self->actor.world.pos.y += Math_CosF(self->bobPhase) * (1.0f + self->bobSpeedMod);
    self->bobPhase += 0.2f;
    Math_SmoothStepToF(&self->actor.speedXZ, self->maxSpeed, 1.0f, 0.5f, 0.0f);

    if (Math_Vec3f_DistXZ(&self->actor.world.pos, &self->actor.home.pos) > 300.0f) {
        self->vMoveAngleY = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
        Math_SmoothStepToS(&self->actor.world.rot.y, self->vMoveAngleY, 1, 0x7D0, 0);
    } else {
        self->timer--;
        if (self->timer <= 0) {
            self->charge ^= true;
            self->flyHeightMod = (s16)(Math_CosF(self->bobPhase) * 10.0f);
            self->actor.speedXZ = 0.0f;
            if (self->charge && (self->targetActor == NULL)) {
                self->vMoveAngleY = self->actor.world.rot.y;
                if (self->actor.xzDistToPlayer < 200.0f) {
                    Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000184);
                    self->vMoveAngleY = self->actor.yawTowardsPlayer;
                }
                self->maxSpeed = (Rand_ZeroOne() * 1.5f) + 6.0f;
                self->timer = (Rand_ZeroOne() * 5.0f) + 20.0f;
                self->actionState = BBBLUE_NORMAL;
            } else {
                Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000444);
                self->maxSpeed = (Rand_ZeroOne() * 1.5f) + 1.0f;
                self->timer = (Rand_ZeroOne() * 20.0f) + 40.0f;
                self->vMoveAngleY = Math_SinF(self->bobPhase) * 65535.0f;
            }
        }
        if ((self->actor.xzDistToPlayer < 150.0f) && (self->actionState != BBBLUE_NORMAL)) {
            if (!self->charge) {
                Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000184);
                self->maxSpeed = (Rand_ZeroOne() * 1.5f) + 6.0f;
                self->timer = (Rand_ZeroOne() * 5.0f) + 20.0f;
                self->vMoveAngleY = self->actor.yawTowardsPlayer;
                self->actionState = self->charge = true; // Sets actionState to BBBLUE_AGGRO
            }
        } else if (self->actor.xzDistToPlayer < 200.0f) {
            self->vMoveAngleY = self->actor.yawTowardsPlayer;
        }
        if (self->targetActor == NULL) {
            explosive = EnBb_FindExplosive(globalCtx, self, 300.0f);
        } else if (self->targetActor->params == 0) {
            explosive = self->targetActor;
        } else {
            explosive = NULL;
        }
        if (explosive != NULL) {
            self->vMoveAngleY = Actor_WorldYawTowardActor(&self->actor, explosive);
            if ((self->vBombHopPhase == 0) && (explosive != self->targetActor)) {
                self->vBombHopPhase = -0x8000;
                self->targetActor = explosive;
                self->actor.speedXZ *= 0.5f;
            }
            Math_SmoothStepToS(&self->actor.world.rot.y, self->vMoveAngleY, 1, 0x1388, 0);
            Math_SmoothStepToF(&self->actor.world.pos.x, explosive->world.pos.x, 1.0f, 1.5f, 0.0f);
            Math_SmoothStepToF(&self->actor.world.pos.y, explosive->world.pos.y + 40.0f, 1.0f, 1.5f, 0.0f);
            Math_SmoothStepToF(&self->actor.world.pos.z, explosive->world.pos.z, 1.0f, 1.5f, 0.0f);
        } else {
            self->targetActor = NULL;
        }
        if (self->vBombHopPhase != 0) {
            self->actor.world.pos.y += -Math_CosS(self->vBombHopPhase) * 10.0f;
            self->vBombHopPhase += 0x1000;
            Math_SmoothStepToS(&self->actor.world.rot.y, self->vMoveAngleY, 1, 0x7D0, 0);
        }
        thisYawToWall = self->actor.wallYaw - self->actor.world.rot.y;
        moveYawToWall = self->actor.wallYaw - self->vMoveAngleY;
        if ((self->targetActor == NULL) && (self->actor.bgCheckFlags & 8) &&
            (ABS(thisYawToWall) > 0x4000 || ABS(moveYawToWall) > 0x4000)) {
            self->vMoveAngleY = self->actor.wallYaw + self->actor.wallYaw - self->actor.world.rot.y - 0x8000;
            Math_SmoothStepToS(&self->actor.world.rot.y, self->vMoveAngleY, 1, 0xBB8, 0);
        }
    }
    Math_SmoothStepToS(&self->actor.world.rot.y, self->vMoveAngleY, 1, 0x3E8, 0);
    if ((self->collider.base.acFlags & AC_HIT) || (self->collider.base.atFlags & AT_HIT)) {
        self->vMoveAngleY = self->actor.yawTowardsPlayer + 0x8000;
        if (self->collider.base.acFlags & AC_HIT) {
            afterHitAngle = -0x8000;
        } else {
            afterHitAngle = 0x4000;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_BITE);
            if (globalCtx->gameplayFrames & 1) {
                afterHitAngle = -0x4000;
            }
        }
        self->actor.world.rot.y = self->actor.yawTowardsPlayer + afterHitAngle;
        self->collider.base.acFlags &= ~AC_HIT;
        self->collider.base.atFlags &= ~AT_HIT;
    }

    if (self->maxSpeed >= 6.0f) {
        if ((s32)self->skelAnime.curFrame == 0 || (s32)self->skelAnime.curFrame == 5) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_MOUTH);
        } else if ((s32)self->skelAnime.curFrame == 2 || (s32)self->skelAnime.curFrame == 7) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_WING);
        }
    } else {
        if ((s32)self->skelAnime.curFrame == 5) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_WING);
        }
    }
    if (((s32)self->skelAnime.curFrame == 0) && (Rand_ZeroOne() < 0.1f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_LAUGH);
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnBb_SetupDown(EnBb* self) {
    Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000444);
    self->action = BB_DOWN;
    self->timer = 200;
    self->actor.colorFilterTimer = 0;
    self->actor.bgCheckFlags &= ~1;
    self->actor.speedXZ = 3.0f;
    self->flameScaleX = 0.0f;
    self->flameScaleY = 0.0f;
    self->actor.gravity = -2.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_DOWN);
    EnBb_SetupAction(self, EnBb_Down);
}

void EnBb_Down(EnBb* self, GlobalContext* globalCtx) {
    s16 yawDiff = self->actor.world.rot.y - self->actor.wallYaw;

    SkelAnime_Update(&self->skelAnime);
    if (self->actor.bgCheckFlags & 8) {
        if (ABS(yawDiff) > 0x4000) {
            self->actor.world.rot.y = self->actor.wallYaw + self->actor.wallYaw - self->actor.world.rot.y - 0x8000;
        }
        self->actor.bgCheckFlags &= ~8;
    }
    if (self->actor.bgCheckFlags & 3) {
        if (self->actor.params == ENBB_RED) {
            s32 floorType = func_80041D4C(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);

            if ((floorType == 2) || (floorType == 3) || (floorType == 9)) {
                self->moveMode = BBMOVE_HIDDEN;
                self->timer = 10;
                self->actionState++;
                self->actor.flags &= ~1;
                self->action = BB_RED;
                EnBb_SetupAction(self, EnBb_Red);
                return;
            }
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        if (self->actor.velocity.y < -14.0f) {
            self->actor.velocity.y *= -0.7f;
        } else {
            self->actor.velocity.y = 10.0f;
        }
        self->actor.bgCheckFlags &= ~1;
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 7.0f, 2, 2.0f, 0, 0, 0);
        Math_SmoothStepToS(&self->actor.world.rot.y, -self->actor.yawTowardsPlayer, 1, 0xBB8, 0);
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
    if ((s32)self->skelAnime.curFrame == 5) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_WING);
    }
    if (self->timer == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_UP);
        switch (self->actor.params) {
            case ENBB_BLUE:
                self->actor.velocity.y = 0.0f;
                self->actor.gravity = 0.0f;
                EnBb_SetupBlue(self);
                break;
            case ENBB_RED:
                if (self->actor.velocity.y == 10.0f) {
                    EnBb_SetupRed(globalCtx, self);
                    EnBb_SpawnFlameTrail(globalCtx, self, true);
                }
                break;
            case ENBB_WHITE:
                self->actor.velocity.y = 0.0f;
                self->actor.gravity = 0.0f;
                EnBb_SetupWhite(globalCtx, self);
                self->actor.world.pos.y -= 60.0f;
                break;
        }
    } else {
        self->timer--;
    }
}

void EnBb_SetupRed(GlobalContext* globalCtx, EnBb* self) {
    Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000184);
    if (self->action == BB_DOWN) {
        self->actor.speedXZ = 5.0f;
        self->actor.gravity = -1.0f;
        self->actor.velocity.y = 16.0f;
        self->actionState = BBRED_ATTACK;
        self->timer = 0;
        self->moveMode = BBMOVE_NORMAL;
        self->actor.bgCheckFlags &= ~1;
    } else {
        self->actor.colChkInfo.health = 4;
        self->timer = 0;
        self->actionState = BBRED_WAIT;
        self->moveMode = BBMOVE_HIDDEN;
        self->actor.world.pos.y -= 80.0f;
        self->actor.home.pos = self->actor.world.pos;
        self->actor.velocity.y = self->actor.gravity = self->actor.speedXZ = 0.0f;
        self->actor.bgCheckFlags &= ~1;
        self->actor.flags &= ~1;
    }
    self->action = BB_RED;
    EnBb_SetupAction(self, EnBb_Red);
}

void EnBb_Red(EnBb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 floorType;
    s16 yawDiff;

    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    switch (self->actionState) {
        case BBRED_WAIT:
            if ((Actor_WorldDistXYZToActor(&self->actor, &player->actor) <= 250.0f) && (ABS(yawDiff) <= 0x4000) &&
                (self->timer == 0)) {
                self->actor.speedXZ = 5.0f;
                self->actor.gravity = -1.0f;
                self->actor.velocity.y = 18.0f;
                self->moveMode = BBMOVE_NOCLIP;
                self->timer = 7;
                self->actor.bgCheckFlags &= ~1;
                self->actionState++;
                EnBb_SpawnFlameTrail(globalCtx, self, false);
            }
            break;
        case BBRED_ATTACK:
            if (self->timer == 0) {
                self->moveMode = BBMOVE_NORMAL;
                self->actor.flags |= 1;
            }
            self->bobPhase += Rand_ZeroOne();
            Math_SmoothStepToF(&self->flameScaleY, 80.0f, 1.0f, 10.0f, 0.0f);
            Math_SmoothStepToF(&self->flameScaleX, 100.0f, 1.0f, 10.0f, 0.0f);
            if (self->actor.bgCheckFlags & 8) {
                yawDiff = self->actor.world.rot.y - self->actor.wallYaw;
                if (ABS(yawDiff) > 0x4000) {
                    self->actor.world.rot.y =
                        self->actor.wallYaw + self->actor.wallYaw - self->actor.world.rot.y - 0x8000;
                }
                self->actor.bgCheckFlags &= ~8;
            }
            if (self->actor.bgCheckFlags & 1) {
                floorType = func_80041D4C(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);
                if ((floorType == 2) || (floorType == 3) || (floorType == 9)) {
                    self->moveMode = BBMOVE_HIDDEN;
                    self->timer = 10;
                    self->actionState++;
                    self->actor.flags &= ~1;
                } else {
                    self->actor.velocity.y *= -1.06f;
                    if (self->actor.velocity.y > 13.0f) {
                        self->actor.velocity.y = 13.0f;
                    }
                    self->actor.world.rot.y = Math_SinF(self->bobPhase) * 65535.0f;
                }
                self->actor.bgCheckFlags &= ~1;
            }
            self->actor.shape.rot.y = self->actor.world.rot.y;
            if (Actor_GetCollidedExplosive(globalCtx, &self->collider.base) != NULL) {
                EnBb_SetupDown(self);
            }
            break;
        case BBRED_HIDE:
            if (self->timer == 0) {
                self->actor.speedXZ = 0.0f;
                self->actor.gravity = 0.0f;
                self->actor.velocity.y = 0.0f;
                self->actionState = BBRED_WAIT;
                self->timer = 120;
                self->actor.world.pos = self->actor.home.pos;
                self->actor.shape.rot = self->actor.world.rot = self->actor.home.rot;
                EnBb_KillFlameTrail(self);
            }
            break;
    }
    if (self->actionState != BBRED_WAIT) {
        if (((s32)self->skelAnime.curFrame == 0) || ((s32)self->skelAnime.curFrame == 5)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_MOUTH);
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLEFALL_FIRE - SFX_FLAG);
    }
}

void EnBb_FaceWaypoint(EnBb* self) {
    self->actor.world.rot.y = self->actor.shape.rot.y = Math_Vec3f_Yaw(&self->actor.world.pos, &self->waypointPos);
}

void EnBb_SetWaypoint(EnBb* self, GlobalContext* globalCtx) {
    Path* path = &globalCtx->setupPathList[self->path];
    Vec3s* point;

    if (self->waypoint == (s16)(path->count - 1)) {
        self->waypoint = 0;
    } else {
        self->waypoint++;
    }
    point = (Vec3s*)SEGMENTED_TO_VIRTUAL(path->points) + self->waypoint;
    self->waypointPos.x = point->x;
    self->waypointPos.y = point->y;
    self->waypointPos.z = point->z;
}

void EnBb_SetupWhite(GlobalContext* globalCtx, EnBb* self) {
    Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000444);
    self->actor.speedXZ = 0.0f;
    self->actor.world.pos.y += 60.0f;
    self->flameScaleX = 100.0f;
    self->action = BB_WHITE;
    self->waypoint = 0;
    self->timer = (Rand_ZeroOne() * 30.0f) + 40.0f;
    self->maxSpeed = 7.0f;
    EnBb_SetupAction(self, EnBb_White);
}

void EnBb_White(EnBb* self, GlobalContext* globalCtx) {
    if (self->actor.speedXZ == 0.0f) {
        f32 distL1;
        f32 vx;
        f32 vz;
        s16 pitch = Math_Vec3f_Pitch(&self->actor.world.pos, &self->waypointPos);
        f32 vy = Math_SinS(pitch) * self->maxSpeed;
        f32 vxz = Math_CosS(pitch) * self->maxSpeed;

        vx = Math_SinS(self->actor.shape.rot.y) * vxz;
        vz = Math_CosS(self->actor.shape.rot.y) * vxz;
        distL1 = Math_SmoothStepToF(&self->actor.world.pos.x, self->waypointPos.x, 1.0f, ABS(vx), 0.0f);
        distL1 += Math_SmoothStepToF(&self->actor.world.pos.y, self->waypointPos.y, 1.0f, ABS(vy), 0.0f);
        distL1 += Math_SmoothStepToF(&self->actor.world.pos.z, self->waypointPos.z, 1.0f, ABS(vz), 0.0f);
        self->bobPhase += (0.05f + (Rand_ZeroOne() * 0.01f));
        if (distL1 == 0.0f) {
            self->timer--;
            if (self->timer == 0) {
                EnBb_SetWaypoint(self, globalCtx);
                EnBb_FaceWaypoint(self);
                Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000184);
                self->timer = Rand_ZeroOne() * 30.0f + 40.0f;
            } else {
                if (self->moveMode != BBMOVE_NORMAL) {
                    Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000444);
                }
                self->actor.world.rot.y += 0x1F40;
            }
            self->moveMode = BBMOVE_NORMAL;
            self->maxSpeed = 0.0f;
        } else {
            self->moveMode = BBMOVE_NOCLIP;
            self->maxSpeed = 10.0f;
        }
        if (self->collider.base.atFlags & AT_HIT) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_BITE);
            self->collider.base.atFlags &= ~AT_HIT;
        }
        self->actor.shape.rot.y = self->actor.world.rot.y;
    } else if (Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f) == 0.0f) {
        EnBb_FaceWaypoint(self);
    }
    SkelAnime_Update(&self->skelAnime);
    if (((s32)self->skelAnime.curFrame == 0) && (Rand_ZeroOne() <= 0.1f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_LAUGH);
    }

    if ((self->maxSpeed != 0.0f) && (((s32)self->skelAnime.curFrame == 0) || ((s32)self->skelAnime.curFrame == 5))) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_MOUTH);
    } else if (((s32)self->skelAnime.curFrame == 2) || ((s32)self->skelAnime.curFrame == 7)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_WING);
    }
}

void EnBb_InitGreen(EnBb* self, GlobalContext* globalCtx) {
    Vec3f bobOffset = { 0.0f, 0.0f, 0.0f };

    Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000444);
    self->moveMode = BBMOVE_NOCLIP;
    self->actionState = BBGREEN_FLAME_ON;
    self->bobPhase = Rand_ZeroOne();
    self->actor.shape.rot.x = self->actor.shape.rot.z = 0;
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    if (self->actor.params == ENBB_GREEN_BIG) {
        EnBb_SetWaypoint(self, globalCtx);
        EnBb_FaceWaypoint(self);
    }
    Matrix_Translate(self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->actor.world.rot.x, self->actor.world.rot.y, 0, MTXMODE_APPLY);
    Matrix_RotateZ(self->bobPhase, MTXMODE_APPLY);
    bobOffset.y = self->bobSize;
    Matrix_MultVec3f(&bobOffset, &self->actor.world.pos);
    self->targetActor = NULL;
    self->action = BB_GREEN;
    self->actor.speedXZ = 0.0f;
    self->vFlameTimer = (Rand_ZeroOne() * 30.0f) + 180.0f;
    EnBb_SetupAction(self, EnBb_Green);
}

void EnBb_SetupGreen(EnBb* self) {
    Animation_PlayLoop(&self->skelAnime, &object_Bb_Anim_000444);
    self->moveMode = BBMOVE_NOCLIP;
    self->actionState = BBGREEN_FLAME_ON;
    self->targetActor = NULL;
    self->action = BB_GREEN;
    self->actor.speedXZ = 0.0f;
    self->vFlameTimer = (Rand_ZeroOne() * 30.0f) + 180.0f;
    self->actor.shape.rot.z = 0;
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    EnBb_SetupAction(self, EnBb_Green);
}

void EnBb_Green(EnBb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f bobOffset = { 0.0f, 0.0f, 0.0f };
    Vec3f nextPos = player->actor.world.pos;

    nextPos.y += 30.0f;
    if (self->actor.params == ENBB_GREEN_BIG) {
        if (self->actor.speedXZ == 0.0f) {
            s16 pitch = Math_Vec3f_Pitch(&self->actor.home.pos, &self->waypointPos);
            s16 yaw = Math_Vec3f_Yaw(&self->actor.home.pos, &self->waypointPos);
            f32 vy = Math_SinS(pitch) * self->maxSpeed;
            f32 vxz = Math_CosS(pitch) * self->maxSpeed;
            f32 vz;
            f32 vx;
            f32 distL1;

            Math_SmoothStepToS(&self->actor.world.rot.y, yaw, 1, 0x3E8, 0);
            vx = Math_SinS(self->actor.world.rot.y) * vxz;
            distL1 = Math_CosS(self->actor.world.rot.y) * vxz;
            vz = Math_SmoothStepToF(&self->actor.home.pos.x, self->waypointPos.x, 1.0f, ABS(vx), 0.0f);
            vz += Math_SmoothStepToF(&self->actor.home.pos.y, self->waypointPos.y, 1.0f, ABS(vy), 0.0f);
            vz += Math_SmoothStepToF(&self->actor.home.pos.z, self->waypointPos.z, 1.0f, ABS(distL1), 0.0f);
            self->bobPhase += (0.05f + (Rand_ZeroOne() * 0.01f));
            if (vz == 0.0f) {
                EnBb_SetWaypoint(self, globalCtx);
            }
            self->moveMode = BBMOVE_NOCLIP;
            self->maxSpeed = 10.0f;
            if (self->collider.base.atFlags & AT_HIT) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_BITE);
                self->collider.base.atFlags &= ~AT_HIT;
            }
            if (Math_CosF(self->bobPhase) == 0.0f) {
                if (self->charge) {
                    self->bobSpeedMod = Rand_ZeroOne();
                } else {
                    self->bobSpeedMod = Rand_ZeroOne() * 3.0f;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_LAUGH);
                }
            }
            self->actor.shape.rot.y = self->actor.world.rot.y;
        } else if (Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 1.0f, 0.5f, 0.0f) == 0.0f) {
            EnBb_FaceWaypoint(self);
        }
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA0, 0);
        Math_SmoothStepToS(&self->actor.shape.rot.x, Math_Vec3f_Pitch(&self->actor.world.pos, &nextPos), 1, 0xFA0, 0);
    }
    SkelAnime_Update(&self->skelAnime);
    if (Math_CosF(self->bobPhase) <= 0.002f) {
        self->bobSpeedMod = Rand_ZeroOne() * 0.05f;
    }
    Matrix_Translate(self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->actor.world.rot.x, self->actor.world.rot.y, 0, MTXMODE_APPLY);
    Matrix_RotateZ(self->bobPhase, MTXMODE_APPLY);
    bobOffset.y = self->bobSize;
    Matrix_MultVec3f(&bobOffset, &nextPos);
    Math_SmoothStepToF(&self->actor.world.pos.x, nextPos.x, 1.0f, self->bobPhase * 0.75f, 0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.y, nextPos.y, 1.0f, self->bobPhase * 0.75f, 0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.z, nextPos.z, 1.0f, self->bobPhase * 0.75f, 0.0f);
    self->bobPhase += 0.1f + self->bobSpeedMod;
    if (Actor_GetCollidedExplosive(globalCtx, &self->collider.base) || (--self->vFlameTimer == 0)) {
        self->actionState++;
        self->timer = (Rand_ZeroOne() * 30.0f) + 60.0f;
        if (self->vFlameTimer != 0) {
            self->collider.base.acFlags &= ~AC_HIT;
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_DOWN);
    }
    if (self->actionState != BBGREEN_FLAME_ON) {
        self->timer--;
        if (self->timer == 0) {
            self->actionState = BBGREEN_FLAME_ON;
            self->vFlameTimer = (Rand_ZeroOne() * 30.0f) + 180.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_UP);
        }
        Math_SmoothStepToF(&self->flameScaleY, 0.0f, 1.0f, 10.0f, 0.0f);
        Math_SmoothStepToF(&self->flameScaleX, 0.0f, 1.0f, 10.0f, 0.0f);
    } else {
        Math_SmoothStepToF(&self->flameScaleY, 80.0f, 1.0f, 10.0f, 0.0f);
        Math_SmoothStepToF(&self->flameScaleX, 100.0f, 1.0f, 10.0f, 0.0f);
    }
    if ((s32)self->skelAnime.curFrame == 5) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_WING);
    }
    if (((s32)self->skelAnime.curFrame == 0) && (Rand_ZeroOne() < 0.1f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_LAUGH);
    }
}

void EnBb_SetupStunned(EnBb* self) {
    self->action = BB_STUNNED;
    if (self->actor.params != ENBB_WHITE) {
        if (self->actor.params != ENBB_RED) {
            if (self->actor.params > ENBB_GREEN) {
                self->actor.gravity = -2.0f;
                self->actor.shape.yOffset = 1500.0f;
            }
            self->actor.speedXZ = 0.0f;
            self->flameScaleX = 0.0f;
            self->flameScaleY = 0.0f;
        } else {
            EnBb_KillFlameTrail(self);
        }
    }
    switch (self->dmgEffect) {
        case 8:
            Actor_SetColorFilter(&self->actor, -0x8000, 0xC8, 0, 0x50);
            break;
        case 9:
            self->fireIceTimer = 0x30;
        case 15:
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
            Actor_SetColorFilter(&self->actor, 0, 0xB4, 0, 0x50);
            break;
    }
    self->actor.bgCheckFlags &= ~1;
    EnBb_SetupAction(self, EnBb_Stunned);
}

void EnBb_Stunned(EnBb* self, GlobalContext* globalCtx) {
    s16 yawDiff = self->actor.world.rot.y - self->actor.wallYaw;

    if (self->actor.bgCheckFlags & 8) {
        if (ABS(yawDiff) > 0x4000) {
            self->actor.world.rot.y = self->actor.wallYaw + self->actor.wallYaw - self->actor.world.rot.y - 0x8000;
        }
        self->actor.bgCheckFlags &= ~8;
    }
    if (self->actor.bgCheckFlags & 2) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        if (self->actor.velocity.y < -14.0f) {
            self->actor.velocity.y *= -0.4f;
        } else {
            self->actor.velocity.y = 0.0f;
        }
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 7.0f, 2, 2.0f, 0, 0, 0);
    }
    if (self->actor.colorFilterTimer == 0) {
        self->actor.shape.yOffset = 200.0f;
        if (self->actor.colChkInfo.health != 0) {
            if ((self->actor.params == ENBB_GREEN) || (self->actor.params == ENBB_GREEN_BIG)) {
                EnBb_SetupGreen(self);
            } else if (self->actor.params == ENBB_WHITE) {
                self->action = BB_WHITE;
                EnBb_SetupAction(self, EnBb_White);
            } else {
                EnBb_SetupDown(self);
            }
        } else {
            self->actor.flags &= ~1;
            EnBb_SetupDeath(self, globalCtx);
        }
    }
}

void EnBb_CollisionCheck(EnBb* self, GlobalContext* globalCtx) {
    if (self->collider.base.atFlags & AT_BOUNCED) {
        self->collider.base.atFlags &= ~AT_BOUNCED;
        if (self->action != BB_DOWN) {
            if (self->actor.params >= ENBB_RED) {
                self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer + 0x8000;
                if (self->actor.params == ENBB_RED) {
                    EnBb_KillFlameTrail(self);
                }
                EnBb_SetupDown(self);
                return;
            }
            self->actionVar2 = 1;
        }
    }
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        self->dmgEffect = self->actor.colChkInfo.damageEffect;
        Actor_SetDropFlag(&self->actor, &self->collider.elements[0].info, 0);
        switch (self->dmgEffect) {
            case 7:
                self->actor.freezeTimer = self->collider.elements[0].info.acHitInfo->toucher.damage;
            case 5:
                self->fireIceTimer = 0x30;
                //! @bug
                //! Setting fireIceTimer here without calling Actor_SetColorFilter causes a crash if the bubble is
                //! killed in a single hit by an attack with damage effect 5 or 7 while actor updating is halted. Using
                //! Din's Fire on a white bubble will do just that. The mechanism is complex and described below.
                goto block_15;
            case 6:
                self->actor.freezeTimer = self->collider.elements[0].info.acHitInfo->toucher.damage;
                break;
            case 8:
            case 9:
            case 15:
                if (self->action != BB_STUNNED) {
                    Actor_ApplyDamage(&self->actor);
                    EnBb_SetupStunned(self);
                }
                break;
            default:
            block_15:
                if ((self->dmgEffect == 14) || (self->dmgEffect == 12) || (self->dmgEffect == 11) ||
                    (self->dmgEffect == 10) || (self->dmgEffect == 7) || (self->dmgEffect == 5)) {
                    if ((self->action != BB_DOWN) || (self->timer < 190)) {
                        Actor_ApplyDamage(&self->actor);
                    }
                    if ((self->action != BB_DOWN) && (self->actor.params != ENBB_WHITE)) {
                        EnBb_SetupDown(self);
                    }
                } else {
                    if (((self->action == BB_DOWN) && (self->timer < 190)) ||
                        ((self->actor.params != ENBB_WHITE) && (self->flameScaleX < 20.0f))) {
                        Actor_ApplyDamage(&self->actor);
                    } else {
                        self->collider.base.acFlags |= AC_HIT;
                    }
                }
                if (self->actor.colChkInfo.health == 0) {
                    self->actor.flags &= ~1;
                    if (self->actor.params == ENBB_RED) {
                        EnBb_KillFlameTrail(self);
                    }
                    EnBb_SetupDeath(self, globalCtx);
                    //! @bug
                    //! Because Din's Fire kills the bubble in a single hit, Actor_SetColorFilter is never called and
                    //! colorFilterParams is never set. And because Din's Fire halts updating during its cutscene,
                    //! EnBb_Death doesn't kill the bubble on the next frame like it should. This combines with
                    //! the bug in EnBb_Draw below to crash the game.
                } else if ((self->actor.params == ENBB_WHITE) &&
                           ((self->action == BB_WHITE) || (self->action == BB_STUNNED))) {
                    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0xC);
                    self->actor.speedXZ = -8.0f;
                    self->maxSpeed = 0.0f;
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_DAMAGE);
                } else if (((self->action == BB_DOWN) && (self->timer < 190)) ||
                           ((self->actor.params != ENBB_WHITE) && (self->flameScaleX < 20.0f))) {
                    EnBb_SetupDamage(self);
                }
            case 13:
                break;
        }
    }
}

void EnBb_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnBb* self = THIS;
    Vec3f sp4C = { 0.0f, 0.0f, 0.0f };
    Vec3f sp40 = { 0.0f, -0.6f, 0.0f };
    Color_RGBA8 sp3C = { 0, 0, 255, 255 };
    Color_RGBA8 sp38 = { 0, 0, 0, 0 };
    f32 sp34 = -15.0f;

    if (self->actor.params <= ENBB_BLUE) {
        EnBb_CollisionCheck(self, globalCtx);
    }
    if (self->actor.colChkInfo.damageEffect != 0xD) {
        self->actionFunc(self, globalCtx);
        if ((self->actor.params <= ENBB_BLUE) && (self->actor.speedXZ >= -6.0f) &&
            ((self->actor.flags & 0x8000) == 0)) {
            Actor_MoveForward(&self->actor);
        }
        if (self->moveMode == BBMOVE_NORMAL) {
            if ((self->actor.world.pos.y - 20.0f) <= self->actor.floorHeight) {
                sp34 = 20.0f;
            }
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, sp34, 25.0f, 20.0f, 5);
        }
        self->actor.focus.pos = self->actor.world.pos;
        self->collider.elements->dim.worldSphere.center.x = self->actor.world.pos.x;
        self->collider.elements->dim.worldSphere.center.y =
            self->actor.world.pos.y + (self->actor.shape.yOffset * self->actor.scale.y);
        self->collider.elements->dim.worldSphere.center.z = self->actor.world.pos.z;

        if ((self->action > BB_KILL) && ((self->actor.speedXZ != 0.0f) || (self->action == BB_GREEN))) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        if ((self->action > BB_FLAME_TRAIL) &&
            ((self->actor.colorFilterTimer == 0) || !(self->actor.colorFilterParams & 0x4000)) &&
            (self->moveMode != BBMOVE_HIDDEN)) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

void EnBb_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnBb* self = THIS;

    BodyBreak_SetInfo(&self->bodyBreak, limbIndex, 4, 15, 15, dList, BODYBREAK_OBJECT_DEFAULT);
}

static Vec3f sFireIceOffsets[] = {
    { 13.0f, 10.0f, 0.0f }, { 5.0f, 25.0f, 5.0f },   { -5.0f, 25.0f, 5.0f },  { -13.0f, 10.0f, 0.0f },
    { 5.0f, 25.0f, -5.0f }, { -5.0f, 25.0f, -5.0f }, { 0.0f, 10.0f, -13.0f }, { 5.0f, 0.0f, 5.0f },
    { 5.0f, 0.0f, -5.0f },  { 0.0f, 10.0f, 13.0f },  { -5.0f, 0.0f, 5.0f },   { -5.0f, 0.0f, -5.0f },
};

void EnBb_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnBb* self = THIS;
    Vec3f blureBase1 = { 0.0f, 5000.0f, 0.0f };
    Vec3f blureBase2 = { 0.0f, 2000.0f, 0.0f };
    Vec3f blureVtx1;
    Vec3f blureVtx2;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bb.c", 2044);

    blureBase1.z = self->maxSpeed * 80.0f;
    blureBase2.z = self->maxSpeed * 80.0f;
    if (self->moveMode != BBMOVE_HIDDEN) {
        if (self->actor.params <= ENBB_BLUE) {
            func_80093D18(globalCtx->state.gfxCtx);
            SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, EnBb_PostLimbDraw,
                              self);

            if (self->fireIceTimer != 0) {
                self->actor.colorFilterTimer++;
                //! @bug:
                //! The purpose of self is to counteract Actor_UpdateAll decrementing colorFilterTimer. However,
                //! the above bugs mean unk_2A8 can be nonzero without damage effects ever having been set.
                //! This routine will then increment colorFilterTimer, and on the next frame Actor_Draw will try
                //! to draw the unset colorFilterParams. This causes a divide-by-zero error, crashing the game.
                if (1) {}
                self->fireIceTimer--;
                if ((self->fireIceTimer % 4) == 0) {
                    Vec3f sp70;
                    s32 index = self->fireIceTimer >> 2;

                    sp70.x = self->actor.world.pos.x + sFireIceOffsets[index].x;
                    sp70.y = self->actor.world.pos.y + sFireIceOffsets[index].y;
                    sp70.z = self->actor.world.pos.z + sFireIceOffsets[index].z;

                    if ((self->dmgEffect != 7) && (self->dmgEffect != 5)) {
                        EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &sp70, 0x96, 0x96, 0x96, 0xFA, 0xEB,
                                                       0xF5, 0xFF, 0.8f);
                    } else {
                        sp70.y -= 17.0f;
                        EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &sp70, 0x28, 1, 0, -1);
                    }
                }
            }
            Matrix_Translate(0.0f, self->flameScaleX * -40.0f, 0.0f, MTXMODE_APPLY);
        } else {
            Matrix_Translate(0.0f, -40.0f, 0.0f, MTXMODE_APPLY);
        }
        if (self->actor.params != ENBB_WHITE) {
            func_80093D84(globalCtx->state.gfxCtx);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0,
                                        ((globalCtx->gameplayFrames + (self->flameScrollMod * 10)) *
                                         (-20 - (self->flameScrollMod * -2))) %
                                            0x200,
                                        0x20, 0x80));
            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, self->flamePrimBlue, self->flamePrimAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, self->flameEnvColor.r, self->flameEnvColor.g, self->flameEnvColor.b, 0);
            Matrix_RotateY(((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) - self->actor.shape.rot.y + 0x8000)) *
                               (M_PI / 0x8000),
                           MTXMODE_APPLY);
            Matrix_Scale(self->flameScaleX * 0.01f, self->flameScaleY * 0.01f, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_bb.c", 2106),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
        } else {
            Matrix_MultVec3f(&blureBase1, &blureVtx1);
            Matrix_MultVec3f(&blureBase2, &blureVtx2);
            if ((self->maxSpeed != 0.0f) && (self->action == BB_WHITE) && !(globalCtx->gameplayFrames & 1) &&
                (self->actor.colChkInfo.health != 0)) {
                EffectBlure_AddVertex(Effect_GetByIndex(self->blureIdx), &blureVtx1, &blureVtx2);
            } else if (self->action != BB_WHITE) {
                EffectBlure_AddSpace(Effect_GetByIndex(self->blureIdx));
            }
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bb.c", 2127);
}
