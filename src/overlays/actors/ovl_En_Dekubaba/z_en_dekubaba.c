#include "z_en_dekubaba.h"
#include "objects/object_dekubaba/object_dekubaba.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"

#define FLAGS 0x00000005

#define THIS ((EnDekubaba*)thisx)

void EnDekubaba_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDekubaba_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDekubaba_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDekubaba_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDekubaba_SetupWait(EnDekubaba* self);
void EnDekubaba_SetupGrow(EnDekubaba* self);
void EnDekubaba_Wait(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_Grow(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_Retract(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_DecideLunge(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_Lunge(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_PrepareLunge(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_PullBack(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_Recover(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_Hit(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_StunnedVertical(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_Sway(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_PrunedSomersault(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_ShrinkDie(EnDekubaba* self, GlobalContext* globalCtx);
void EnDekubaba_DeadStickDrop(EnDekubaba* self, GlobalContext* globalCtx);

static Vec3f sZeroVec = { 0.0f, 0.0f, 0.0f };

const ActorInit En_Dekubaba_InitVars = {
    ACTOR_EN_DEKUBABA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DEKUBABA,
    sizeof(EnDekubaba),
    (ActorFunc)EnDekubaba_Init,
    (ActorFunc)EnDekubaba_Destroy,
    (ActorFunc)EnDekubaba_Update,
    (ActorFunc)EnDekubaba_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[7] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_HARD,
            BUMP_ON,
            OCELEM_ON,
        },
        { 1, { { 0, 100, 1000 }, 15 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 51, { { 0, 0, 1500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 52, { { 0, 0, 500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 53, { { 0, 0, 1500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 54, { { 0, 0, 500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 55, { { 0, 0, 1500 }, 8 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 56, { { 0, 0, 500 }, 8 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    7,
    sJntSphElementsInit,
};

static CollisionCheckInfoInit sColChkInfoInit = { 2, 25, 25, MASS_IMMOVABLE };

typedef enum {
    /* 0x0 */ DEKUBABA_DMGEFF_NONE,
    /* 0x1 */ DEKUBABA_DMGEFF_DEKUNUT,
    /* 0x2 */ DEKUBABA_DMGEFF_FIRE,
    /* 0xE */ DEKUBABA_DMGEFF_BOOMERANG = 14,
    /* 0xF */ DEKUBABA_DMGEFF_SWORD
} DekuBabaDamageEffect;

static DamageTable sDekuBabaDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, DEKUBABA_DMGEFF_DEKUNUT),
    /* Deku stick    */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(1, DEKUBABA_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(2, DEKUBABA_DMGEFF_BOOMERANG),
    /* Normal arrow  */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Hookshot      */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Kokiri sword  */ DMG_ENTRY(1, DEKUBABA_DMGEFF_SWORD),
    /* Master sword  */ DMG_ENTRY(2, DEKUBABA_DMGEFF_SWORD),
    /* Giant's Knife */ DMG_ENTRY(4, DEKUBABA_DMGEFF_SWORD),
    /* Fire arrow    */ DMG_ENTRY(4, DEKUBABA_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Light arrow   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(4, DEKUBABA_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Light magic   */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Shield        */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, DEKUBABA_DMGEFF_SWORD),
    /* Giant spin    */ DMG_ENTRY(4, DEKUBABA_DMGEFF_SWORD),
    /* Master spin   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_SWORD),
    /* Kokiri jump   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_SWORD),
    /* Giant jump    */ DMG_ENTRY(8, DEKUBABA_DMGEFF_SWORD),
    /* Master jump   */ DMG_ENTRY(4, DEKUBABA_DMGEFF_SWORD),
    /* Unknown 1     */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, DEKUBABA_DMGEFF_NONE),
    /* Unknown 2     */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
};

// The only difference is that for Big Deku Babas, Hookshot will act the same as Deku Nuts: i.e. it will stun, but
// cannot kill.
static DamageTable sBigDekuBabaDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, DEKUBABA_DMGEFF_DEKUNUT),
    /* Deku stick    */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Slingshot     */ DMG_ENTRY(1, DEKUBABA_DMGEFF_NONE),
    /* Explosive     */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Boomerang     */ DMG_ENTRY(2, DEKUBABA_DMGEFF_BOOMERANG),
    /* Normal arrow  */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Hammer swing  */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Hookshot      */ DMG_ENTRY(0, DEKUBABA_DMGEFF_DEKUNUT),
    /* Kokiri sword  */ DMG_ENTRY(1, DEKUBABA_DMGEFF_SWORD),
    /* Master sword  */ DMG_ENTRY(2, DEKUBABA_DMGEFF_SWORD),
    /* Giant's Knife */ DMG_ENTRY(4, DEKUBABA_DMGEFF_SWORD),
    /* Fire arrow    */ DMG_ENTRY(4, DEKUBABA_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Light arrow   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Unk arrow 1   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(4, DEKUBABA_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Light magic   */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Shield        */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, DEKUBABA_DMGEFF_SWORD),
    /* Giant spin    */ DMG_ENTRY(4, DEKUBABA_DMGEFF_SWORD),
    /* Master spin   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_SWORD),
    /* Kokiri jump   */ DMG_ENTRY(2, DEKUBABA_DMGEFF_SWORD),
    /* Giant jump    */ DMG_ENTRY(8, DEKUBABA_DMGEFF_SWORD),
    /* Master jump   */ DMG_ENTRY(4, DEKUBABA_DMGEFF_SWORD),
    /* Unknown 1     */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, DEKUBABA_DMGEFF_NONE),
    /* Unknown 2     */ DMG_ENTRY(0, DEKUBABA_DMGEFF_NONE),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 1500, ICHAIN_STOP),
};

void EnDekubaba_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDekubaba* self = THIS;
    s32 i;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 22.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gDekuBabaSkel, &gDekuBabaFastChompAnim, self->jointTable,
                   self->morphTable, 8);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderElements);

    if (self->actor.params == DEKUBABA_BIG) {
        self->size = 2.5f;

        for (i = 0; i < sJntSphInit.count; i++) {
            self->collider.elements[i].dim.worldSphere.radius = self->collider.elements[i].dim.modelSphere.radius =
                (sJntSphElementsInit[i].dim.modelSphere.radius * 2.50f);
        }

        // This and its counterpart below mean that a Deku Stick jumpslash will not trigger the Deku Stick drop route.
        // (Of course they reckoned without each age being able to use the other's items, so Stick and Master Sword
        // jumpslash can give the Stick drop as adult, and neither will as child.)
        if (!LINK_IS_ADULT) {
            sBigDekuBabaDamageTable.table[0x1B] = DMG_ENTRY(4, DEKUBABA_DMGEFF_NONE); // DMG_JUMP_MASTER
        }

        CollisionCheck_SetInfo(&self->actor.colChkInfo, &sBigDekuBabaDamageTable, &sColChkInfoInit);
        self->actor.colChkInfo.health = 4;
        self->actor.naviEnemyId = 0x08; // Big Deku Baba
        self->actor.targetMode = 2;
    } else {
        self->size = 1.0f;

        for (i = 0; i < sJntSphInit.count; i++) {
            self->collider.elements[i].dim.worldSphere.radius = self->collider.elements[i].dim.modelSphere.radius;
        }

        if (!LINK_IS_ADULT) {
            sDekuBabaDamageTable.table[0x1B] = DMG_ENTRY(4, DEKUBABA_DMGEFF_NONE); // DMG_JUMP_MASTER
        }

        CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDekuBabaDamageTable, &sColChkInfoInit);
        self->actor.naviEnemyId = 0x07; // Deku Baba
        self->actor.targetMode = 1;
    }

    EnDekubaba_SetupWait(self);
    self->timer = 0;
    self->boundFloor = NULL;
    self->bodyPartsPos[3] = self->actor.home.pos;
}

void EnDekubaba_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDekubaba* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void EnDekubaba_DisableHitboxes(EnDekubaba* self) {
    s32 i;

    for (i = 1; i < ARRAY_COUNT(self->colliderElements); i++) {
        self->collider.elements[i].info.bumperFlags &= ~BUMP_ON;
    }
}

void EnDekubaba_SetupWait(EnDekubaba* self) {
    s32 i;
    ColliderJntSphElement* element;

    self->actor.shape.rot.x = -0x4000;
    self->stemSectionAngle[0] = self->stemSectionAngle[1] = self->stemSectionAngle[2] = self->actor.shape.rot.x;

    self->actor.world.pos.x = self->actor.home.pos.x;
    self->actor.world.pos.z = self->actor.home.pos.z;
    self->actor.world.pos.y = self->actor.home.pos.y + 14.0f * self->size;

    Actor_SetScale(&self->actor, self->size * 0.01f * 0.5f);

    self->collider.base.colType = COLTYPE_HARD;
    self->collider.base.acFlags |= AC_HARD;
    self->timer = 45;

    for (i = 1; i < ARRAY_COUNT(self->colliderElements); i++) {
        element = &self->collider.elements[i];
        element->dim.worldSphere.center.x = self->actor.world.pos.x;
        element->dim.worldSphere.center.y = (s16)self->actor.world.pos.y - 7;
        element->dim.worldSphere.center.z = self->actor.world.pos.z;
    }

    self->actionFunc = EnDekubaba_Wait;
}

void EnDekubaba_SetupGrow(EnDekubaba* self) {
    s32 i;

    Animation_Change(&self->skelAnime, &gDekuBabaFastChompAnim,
                     Animation_GetLastFrame(&gDekuBabaFastChompAnim) * (1.0f / 15), 0.0f,
                     Animation_GetLastFrame(&gDekuBabaFastChompAnim), ANIMMODE_ONCE, 0.0f);

    self->timer = 15;

    for (i = 2; i < ARRAY_COUNT(self->colliderElements); i++) {
        self->collider.elements[i].info.ocElemFlags |= OCELEM_ON;
    }

    self->collider.base.colType = COLTYPE_HIT6;
    self->collider.base.acFlags &= ~AC_HARD;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DUMMY482);
    self->actionFunc = EnDekubaba_Grow;
}

void EnDekubaba_SetupRetract(EnDekubaba* self) {
    s32 i;

    Animation_Change(&self->skelAnime, &gDekuBabaFastChompAnim, -1.5f, Animation_GetLastFrame(&gDekuBabaFastChompAnim),
                     0.0f, ANIMMODE_ONCE, -3.0f);

    self->timer = 15;

    for (i = 2; i < ARRAY_COUNT(self->colliderElements); i++) {
        self->collider.elements[i].info.ocElemFlags &= ~OCELEM_ON;
    }

    self->actionFunc = EnDekubaba_Retract;
}

void EnDekubaba_SetupDecideLunge(EnDekubaba* self) {
    self->timer = Animation_GetLastFrame(&gDekuBabaFastChompAnim) * 2;
    Animation_MorphToLoop(&self->skelAnime, &gDekuBabaFastChompAnim, -3.0f);
    self->actionFunc = EnDekubaba_DecideLunge;
}

void EnDekubaba_SetupPrepareLunge(EnDekubaba* self) {
    self->timer = 8;
    self->skelAnime.playSpeed = 0.0f;
    self->actionFunc = EnDekubaba_PrepareLunge;
}

void EnDekubaba_SetupLunge(EnDekubaba* self) {
    Animation_PlayOnce(&self->skelAnime, &gDekuBabaPauseChompAnim);
    self->timer = 0;
    self->actionFunc = EnDekubaba_Lunge;
}

void EnDekubaba_SetupPullBack(EnDekubaba* self) {
    Animation_Change(&self->skelAnime, &gDekuBabaPauseChompAnim, 1.0f, 15.0f,
                     Animation_GetLastFrame(&gDekuBabaPauseChompAnim), ANIMMODE_ONCE, -3.0f);
    self->timer = 0;
    self->actionFunc = EnDekubaba_PullBack;
}

void EnDekubaba_SetupRecover(EnDekubaba* self) {
    self->timer = 9;
    self->collider.base.acFlags |= AC_ON;
    self->skelAnime.playSpeed = -1.0f;
    self->actionFunc = EnDekubaba_Recover;
}

void EnDekubaba_SetupHit(EnDekubaba* self, s32 arg1) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gDekuBabaPauseChompAnim, -5.0f);
    self->timer = arg1;
    self->collider.base.acFlags &= ~AC_ON;
    Actor_SetScale(&self->actor, self->size * 0.01f);

    if (arg1 == 2) {
        Actor_SetColorFilter(&self->actor, 0, 155, 0, 62);
    } else {
        Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 42);
    }

    self->actionFunc = EnDekubaba_Hit;
}

void EnDekubaba_SetupPrunedSomersault(EnDekubaba* self) {
    self->timer = 0;
    self->skelAnime.playSpeed = 0.0f;
    self->actor.gravity = -0.8f;
    self->actor.velocity.y = 4.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
    self->collider.base.acFlags &= ~AC_ON;
    self->actor.speedXZ = self->size * 3.0f;
    self->actor.flags |= 0x30;
    self->actionFunc = EnDekubaba_PrunedSomersault;
}

void EnDekubaba_SetupShrinkDie(EnDekubaba* self) {
    Animation_Change(&self->skelAnime, &gDekuBabaFastChompAnim, -1.5f, Animation_GetLastFrame(&gDekuBabaFastChompAnim),
                     0.0f, ANIMMODE_ONCE, -3.0f);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnDekubaba_ShrinkDie;
}

void EnDekubaba_SetupStunnedVertical(EnDekubaba* self) {
    s32 i;

    for (i = 1; i < ARRAY_COUNT(self->colliderElements); i++) {
        self->collider.elements[i].info.bumperFlags |= BUMP_ON;
    }

    if (self->timer == 1) {
        Animation_Change(&self->skelAnime, &gDekuBabaFastChompAnim, 4.0f, 0.0f,
                         Animation_GetLastFrame(&gDekuBabaFastChompAnim), ANIMMODE_LOOP, -3.0f);
        self->timer = 40;
    } else {
        Animation_Change(&self->skelAnime, &gDekuBabaFastChompAnim, 0.0f, 0.0f,
                         Animation_GetLastFrame(&gDekuBabaFastChompAnim), ANIMMODE_LOOP, -3.0f);
        self->timer = 60;
    }

    self->actor.world.pos.x = self->actor.home.pos.x;
    self->actor.world.pos.y = self->actor.home.pos.y + (60.0f * self->size);
    self->actor.world.pos.z = self->actor.home.pos.z;
    self->actionFunc = EnDekubaba_StunnedVertical;
}

void EnDekubaba_SetupSway(EnDekubaba* self) {
    self->targetSwayAngle = -0x6000;
    self->stemSectionAngle[2] = -0x5000;
    self->stemSectionAngle[1] = -0x4800;

    EnDekubaba_DisableHitboxes(self);
    Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 35);
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnDekubaba_Sway;
}

void EnDekubaba_SetupDeadStickDrop(EnDekubaba* self, GlobalContext* globalCtx) {
    Actor_SetScale(&self->actor, 0.03f);
    self->actor.shape.rot.x -= 0x4000;
    self->actor.shape.yOffset = 1000.0f;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.shape.shadowScale = 3.0f;
    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_MISC);
    self->actor.flags &= ~0x20;
    self->timer = 200;
    self->actionFunc = EnDekubaba_DeadStickDrop;
}

// Action functions

void EnDekubaba_Wait(EnDekubaba* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.world.pos.x = self->actor.home.pos.x;
    self->actor.world.pos.z = self->actor.home.pos.z;
    self->actor.world.pos.y = self->actor.home.pos.y + 14.0f * self->size;

    if ((self->timer == 0) && (self->actor.xzDistToPlayer < 200.0f * self->size) &&
        (fabsf(self->actor.yDistToPlayer) < 30.0f * self->size)) {
        EnDekubaba_SetupGrow(self);
    }
}

void EnDekubaba_Grow(EnDekubaba* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 headDistHorizontal;
    f32 headDistVertical;
    f32 headShiftX;
    f32 headShiftZ;

    if (self->timer != 0) {
        self->timer--;
    }

    SkelAnime_Update(&self->skelAnime);

    self->actor.scale.x = self->actor.scale.y = self->actor.scale.z =
        self->size * 0.01f * (0.5f + (15 - self->timer) * 0.5f / 15.0f);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0x1800, 0x800);

    headDistVertical = sinf(CLAMP_MAX((15 - self->timer) * (1.0f / 15), 0.7f) * M_PI) * 32.0f + 14.0f;

    if (self->actor.shape.rot.x < -0x38E3) {
        headDistHorizontal = 0.0f;
    } else if (self->actor.shape.rot.x < -0x238E) {
        Math_ScaledStepToS(&self->stemSectionAngle[0], -0x5555, 0x38E);
        headDistHorizontal = Math_CosS(self->stemSectionAngle[0]) * 20.0f;
    } else if (self->actor.shape.rot.x < -0xE38) {
        Math_ScaledStepToS(&self->stemSectionAngle[0], -0xAAA, 0x38E);
        Math_ScaledStepToS(&self->stemSectionAngle[1], -0x5555, 0x38E);
        Math_ScaledStepToS(&self->stemSectionAngle[2], -0x5555, 0x222);

        headDistHorizontal = 20.0f * (Math_CosS(self->stemSectionAngle[0]) + Math_CosS(self->stemSectionAngle[1])) +
                             (headDistVertical -
                              20.0f * (-Math_SinS(self->stemSectionAngle[0]) - Math_SinS(self->stemSectionAngle[1]))) *
                                 Math_CosS(self->stemSectionAngle[2]) / -Math_SinS(self->stemSectionAngle[2]);
    } else {
        Math_ScaledStepToS(&self->stemSectionAngle[0], -0xAAA, 0x38E);
        Math_ScaledStepToS(&self->stemSectionAngle[1], -0x31C7, 0x222);
        Math_ScaledStepToS(&self->stemSectionAngle[2], -0x5555, 0x222);

        headDistHorizontal = 20.0f * (Math_CosS(self->stemSectionAngle[0]) + Math_CosS(self->stemSectionAngle[1])) +
                             (headDistVertical -
                              20.0f * (-Math_SinS(self->stemSectionAngle[0]) - Math_SinS(self->stemSectionAngle[1]))) *
                                 Math_CosS(self->stemSectionAngle[2]) / -Math_SinS(self->stemSectionAngle[2]);
    }

    if (self->timer < 10) {
        Math_ApproachS(&self->actor.shape.rot.y, Math_Vec3f_Yaw(&self->actor.home.pos, &player->actor.world.pos), 2,
                       0xE38);
        if (headShiftZ) {} // One way of fake-matching
    }

    self->actor.world.pos.y = self->actor.home.pos.y + (headDistVertical * self->size);
    headShiftX = headDistHorizontal * self->size * Math_SinS(self->actor.shape.rot.y);
    headShiftZ = headDistHorizontal * self->size * Math_CosS(self->actor.shape.rot.y);
    self->actor.world.pos.x = self->actor.home.pos.x + headShiftX;
    self->actor.world.pos.z = self->actor.home.pos.z + headShiftZ;

    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.home.pos, self->size * 3.0f, 0, self->size * 12.0f,
                             self->size * 5.0f, 1, HAHEN_OBJECT_DEFAULT, 10, NULL);

    if (self->timer == 0) {
        if (Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) < 240.0f * self->size) {
            EnDekubaba_SetupPrepareLunge(self);
        } else {
            EnDekubaba_SetupRetract(self);
        }
    }
}

void EnDekubaba_Retract(EnDekubaba* self, GlobalContext* globalCtx) {
    f32 headDistHorizontal;
    f32 headDistVertical;
    f32 xShift;
    f32 zShift;

    if (self->timer != 0) {
        self->timer--;
    }

    SkelAnime_Update(&self->skelAnime);

    self->actor.scale.x = self->actor.scale.y = self->actor.scale.z =
        self->size * 0.01f * (0.5f + self->timer * (1.0f / 30));
    Math_ScaledStepToS(&self->actor.shape.rot.x, -0x4000, 0x300);

    headDistVertical = (sinf(CLAMP_MAX(self->timer * 0.033f, 0.7f) * M_PI) * 32.0f) + 14.0f;

    if (self->actor.shape.rot.x < -0x38E3) {
        headDistHorizontal = 0.0f;
    } else if (self->actor.shape.rot.x < -0x238E) {
        Math_ScaledStepToS(&self->stemSectionAngle[0], -0x4000, 0x555);
        headDistHorizontal = Math_CosS(self->stemSectionAngle[0]) * 20.0f;
    } else if (self->actor.shape.rot.x < -0xE38) {
        Math_ScaledStepToS(&self->stemSectionAngle[0], -0x5555, 0x555);
        Math_ScaledStepToS(&self->stemSectionAngle[1], -0x4000, 0x555);
        Math_ScaledStepToS(&self->stemSectionAngle[2], -0x4000, 0x333);

        headDistHorizontal = 20.0f * (Math_CosS(self->stemSectionAngle[0]) + Math_CosS(self->stemSectionAngle[1])) +
                             (headDistVertical -
                              20.0f * (-Math_SinS(self->stemSectionAngle[0]) - Math_SinS(self->stemSectionAngle[1]))) *
                                 Math_CosS(self->stemSectionAngle[2]) / -Math_SinS(self->stemSectionAngle[2]);
    } else {
        Math_ScaledStepToS(&self->stemSectionAngle[0], -0x5555, 0x555);
        Math_ScaledStepToS(&self->stemSectionAngle[1], -0x5555, 0x333);
        Math_ScaledStepToS(&self->stemSectionAngle[2], -0x4000, 0x333);

        headDistHorizontal = 20.0f * (Math_CosS(self->stemSectionAngle[0]) + Math_CosS(self->stemSectionAngle[1])) +
                             (headDistVertical -
                              20.0f * (-Math_SinS(self->stemSectionAngle[0]) - Math_SinS(self->stemSectionAngle[1]))) *
                                 Math_CosS(self->stemSectionAngle[2]) / -Math_SinS(self->stemSectionAngle[2]);
    }

    self->actor.world.pos.y = self->actor.home.pos.y + (headDistVertical * self->size);
    xShift = headDistHorizontal * self->size * Math_SinS(self->actor.shape.rot.y);
    zShift = headDistHorizontal * self->size * Math_CosS(self->actor.shape.rot.y);
    self->actor.world.pos.x = self->actor.home.pos.x + xShift;
    self->actor.world.pos.z = self->actor.home.pos.z + zShift;

    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.home.pos, self->size * 3.0f, 0, self->size * 12.0f,
                             self->size * 5.0f, 1, HAHEN_OBJECT_DEFAULT, 0xA, NULL);

    if (self->timer == 0) {
        EnDekubaba_SetupWait(self);
    }
}

void EnDekubaba_UpdateHeadPosition(EnDekubaba* self) {
    f32 horizontalHeadShift = (Math_CosS(self->stemSectionAngle[0]) + Math_CosS(self->stemSectionAngle[1]) +
                               Math_CosS(self->stemSectionAngle[2])) *
                              20.0f;

    self->actor.world.pos.x =
        self->actor.home.pos.x + Math_SinS(self->actor.shape.rot.y) * (horizontalHeadShift * self->size);
    self->actor.world.pos.y =
        self->actor.home.pos.y - (Math_SinS(self->stemSectionAngle[0]) + Math_SinS(self->stemSectionAngle[1]) +
                                  Math_SinS(self->stemSectionAngle[2])) *
                                     20.0f * self->size;
    self->actor.world.pos.z =
        self->actor.home.pos.z + Math_CosS(self->actor.shape.rot.y) * (horizontalHeadShift * self->size);
}

void EnDekubaba_DecideLunge(EnDekubaba* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 12.0f)) {
        if (self->actor.params == DEKUBABA_BIG) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_MOUTH);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_JR_MOUTH);
        }
    }

    if (self->timer != 0) {
        self->timer--;
    }

    Math_ApproachS(&self->actor.shape.rot.y, Math_Vec3f_Yaw(&self->actor.home.pos, &player->actor.world.pos), 2,
                   (self->timer % 5) * 0x222);

    if (self->timer < 10) {
        self->stemSectionAngle[0] += 0x16C;
        self->stemSectionAngle[1] += 0x16C;
        self->stemSectionAngle[2] += 0xB6;
        self->actor.shape.rot.x += 0x222;
    } else if (self->timer < 20) {
        self->stemSectionAngle[0] -= 0x16C;
        self->stemSectionAngle[1] += 0x111;
        self->actor.shape.rot.x += 0x16C;
    } else if (self->timer < 30) {
        self->stemSectionAngle[1] -= 0x111;
        self->actor.shape.rot.x -= 0xB6;
    } else {
        self->stemSectionAngle[1] -= 0xB6;
        self->stemSectionAngle[2] += 0xB6;
        self->actor.shape.rot.x -= 0x16C;
    }

    EnDekubaba_UpdateHeadPosition(self);

    if (240.0f * self->size < Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos)) {
        EnDekubaba_SetupRetract(self);
    } else if ((self->timer == 0) || (self->actor.xzDistToPlayer < 80.0f * self->size)) {
        EnDekubaba_SetupPrepareLunge(self);
    }
}

void EnDekubaba_Lunge(EnDekubaba* self, GlobalContext* globalCtx) {
    static Color_RGBA8 primColor = { 105, 255, 105, 255 };
    static Color_RGBA8 envColor = { 150, 250, 150, 0 };
    s32 allStepsDone;
    s16 curFrame10;
    Vec3f velocity;

    SkelAnime_Update(&self->skelAnime);

    if (self->timer == 0) {
        if (Animation_OnFrame(&self->skelAnime, 1.0f)) {
            if (self->actor.params == DEKUBABA_BIG) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_ATTACK);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_JR_ATTACK);
            }
        }

        Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x222);

        curFrame10 = self->skelAnime.curFrame * 10.0f;

        allStepsDone = true;
        allStepsDone &= Math_ScaledStepToS(&self->stemSectionAngle[0], -0xE38, curFrame10 + 0x38E);
        allStepsDone &= Math_ScaledStepToS(&self->stemSectionAngle[1], -0xE38, curFrame10 + 0x71C);
        allStepsDone &= Math_ScaledStepToS(&self->stemSectionAngle[2], -0xE38, curFrame10 + 0xE38);

        if (allStepsDone) {
            Animation_PlayLoopSetSpeed(&self->skelAnime, &gDekuBabaFastChompAnim, 4.0f);
            velocity.x = Math_SinS(self->actor.shape.rot.y) * 5.0f;
            velocity.y = 0.0f;
            velocity.z = Math_CosS(self->actor.shape.rot.y) * 5.0f;

            func_8002829C(globalCtx, &self->actor.world.pos, &velocity, &sZeroVec, &primColor, &envColor, 1,
                          self->size * 100.0f);
            self->timer = 1;
            self->collider.base.acFlags |= AC_ON;
        }
    } else if (self->timer > 10) {
        EnDekubaba_SetupPullBack(self);
    } else {
        self->timer++;

        if ((self->timer >= 4) && !Actor_IsFacingPlayer(&self->actor, 0x16C)) {
            Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xF, 0x71C);
        }

        if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 12.0f)) {
            if (self->actor.params == DEKUBABA_BIG) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_MOUTH);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_JR_MOUTH);
            }
        }
    }

    EnDekubaba_UpdateHeadPosition(self);
}

void EnDekubaba_PrepareLunge(EnDekubaba* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->timer != 0) {
        self->timer--;
    }

    Math_SmoothStepToS(&self->actor.shape.rot.x, 0x1800, 2, 0xE38, 0x71C);
    Math_ApproachS(&self->actor.shape.rot.y, Math_Vec3f_Yaw(&self->actor.home.pos, &player->actor.world.pos), 2, 0xE38);
    Math_ScaledStepToS(&self->stemSectionAngle[0], 0xAAA, 0x444);
    Math_ScaledStepToS(&self->stemSectionAngle[1], -0x4718, 0x888);
    Math_ScaledStepToS(&self->stemSectionAngle[2], -0x6AA4, 0x888);

    if (self->timer == 0) {
        EnDekubaba_SetupLunge(self);
    }

    EnDekubaba_UpdateHeadPosition(self);
}

void EnDekubaba_PullBack(EnDekubaba* self, GlobalContext* globalCtx) {
    Vec3f dustPos;
    f32 xIncr;
    f32 zIncr;
    s32 i;

    SkelAnime_Update(&self->skelAnime);

    if (self->timer == 0) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x93E, 0x38E);
        Math_ScaledStepToS(&self->stemSectionAngle[0], -0x888, 0x16C);
        Math_ScaledStepToS(&self->stemSectionAngle[1], -0x888, 0x16C);
        if (Math_ScaledStepToS(&self->stemSectionAngle[2], -0x888, 0x16C)) {
            xIncr = Math_SinS(self->actor.shape.rot.y) * 30.0f * self->size;
            zIncr = Math_CosS(self->actor.shape.rot.y) * 30.0f * self->size;
            dustPos = self->actor.home.pos;

            for (i = 0; i < 3; i++) {
                func_800286CC(globalCtx, &dustPos, &sZeroVec, &sZeroVec, self->size * 500.0f, self->size * 50.0f);
                dustPos.x += xIncr;
                dustPos.z += zIncr;
            }

            self->timer = 1;
        }
    } else if (self->timer == 11) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x93E, 0x200);
        Math_ScaledStepToS(&self->stemSectionAngle[0], -0xAAA, 0x200);
        Math_ScaledStepToS(&self->stemSectionAngle[2], -0x5C71, 0x200);

        if (Math_ScaledStepToS(&self->stemSectionAngle[1], 0x238C, 0x200)) {
            self->timer = 12;
        }
    } else if (self->timer == 18) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x2AA8, 0xAAA);

        if (Math_ScaledStepToS(&self->stemSectionAngle[0], 0x1554, 0x5B0)) {
            self->timer = 25;
        }

        Math_ScaledStepToS(&self->stemSectionAngle[1], -0x38E3, 0xAAA);
        Math_ScaledStepToS(&self->stemSectionAngle[2], -0x5C71, 0x2D8);
    } else if (self->timer == 25) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x5550, 0xAAA);

        if (Math_ScaledStepToS(&self->stemSectionAngle[0], -0x6388, 0x93E)) {
            self->timer = 26;
        }

        Math_ScaledStepToS(&self->stemSectionAngle[1], -0x3FFC, 0x4FA);
        Math_ScaledStepToS(&self->stemSectionAngle[2], -0x238C, 0x444);
    } else if (self->timer == 26) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x1800, 0x93E);

        if (Math_ScaledStepToS(&self->stemSectionAngle[0], -0x1555, 0x71C)) {
            self->timer = 27;
        }

        Math_ScaledStepToS(&self->stemSectionAngle[1], -0x38E3, 0x2D8);
        Math_ScaledStepToS(&self->stemSectionAngle[2], -0x5C71, 0x5B0);
    } else if (self->timer >= 27) {
        self->timer++;

        if (self->timer > 30) {
            if (self->actor.xzDistToPlayer < 80.0f * self->size) {
                EnDekubaba_SetupPrepareLunge(self);
            } else {
                EnDekubaba_SetupDecideLunge(self);
            }
        }
    } else {
        self->timer++;

        if (self->timer == 10) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_SCRAPE);
        }

        if (self->timer >= 12) {
            Math_ScaledStepToS(&self->stemSectionAngle[2], -0x5C71, 0x88);
        }
    }

    EnDekubaba_UpdateHeadPosition(self);
}

void EnDekubaba_Recover(EnDekubaba* self, GlobalContext* globalCtx) {
    s32 anyStepsDone;

    SkelAnime_Update(&self->skelAnime);

    if (self->timer > 8) {
        anyStepsDone = Math_SmoothStepToS(&self->actor.shape.rot.x, 0x1800, 1, 0x11C6, 0x71C);
        anyStepsDone |= Math_SmoothStepToS(&self->stemSectionAngle[0], -0x1555, 1, 0xAAA, 0x71C);
        anyStepsDone |= Math_SmoothStepToS(&self->stemSectionAngle[1], -0x38E3, 1, 0xE38, 0x71C);
        anyStepsDone |= Math_SmoothStepToS(&self->stemSectionAngle[2], -0x5C71, 1, 0x11C6, 0x71C);

        if (!anyStepsDone) {
            self->timer = 8;
        }
    } else {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            EnDekubaba_SetupDecideLunge(self);
        }
    }

    EnDekubaba_UpdateHeadPosition(self);
}

/**
 * Hit by a weapon or hit something when lunging.
 */
void EnDekubaba_Hit(EnDekubaba* self, GlobalContext* globalCtx) {
    s32 allStepsDone;

    SkelAnime_Update(&self->skelAnime);

    allStepsDone = true;
    allStepsDone &= Math_ScaledStepToS(&self->actor.shape.rot.x, -0x4000, 0xE38);
    allStepsDone &= Math_ScaledStepToS(&self->stemSectionAngle[0], -0x4000, 0xE38);
    allStepsDone &= Math_ScaledStepToS(&self->stemSectionAngle[1], -0x4000, 0xE38);
    allStepsDone &= Math_ScaledStepToS(&self->stemSectionAngle[2], -0x4000, 0xE38);

    if (allStepsDone) {
        if (self->actor.colChkInfo.health == 0) {
            EnDekubaba_SetupShrinkDie(self);
        } else {
            self->collider.base.acFlags |= AC_ON;
            if (self->timer == 0) {
                if (self->actor.xzDistToPlayer < 80.0f * self->size) {
                    EnDekubaba_SetupPrepareLunge(self);
                } else {
                    EnDekubaba_SetupRecover(self);
                }
            } else {
                EnDekubaba_SetupStunnedVertical(self);
            }
        }
    }

    EnDekubaba_UpdateHeadPosition(self);
}

void EnDekubaba_StunnedVertical(EnDekubaba* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        EnDekubaba_DisableHitboxes(self);

        if (self->actor.xzDistToPlayer < 80.0f * self->size) {
            EnDekubaba_SetupPrepareLunge(self);
        } else {
            EnDekubaba_SetupRecover(self);
        }
    }
}

/**
 * Sway back and forth with decaying amplitude until close enough to vertical.
 */
void EnDekubaba_Sway(EnDekubaba* self, GlobalContext* globalCtx) {
    s16 angleToVertical;

    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, self->stemSectionAngle[0], 0x71C);
    Math_ScaledStepToS(&self->stemSectionAngle[0], self->stemSectionAngle[1], 0x71C);
    Math_ScaledStepToS(&self->stemSectionAngle[1], self->stemSectionAngle[2], 0x71C);

    if (Math_ScaledStepToS(&self->stemSectionAngle[2], self->targetSwayAngle, 0x71C)) {
        self->targetSwayAngle = -0x4000 - (self->targetSwayAngle + 0x4000) * 0.8f;
    }
    angleToVertical = self->targetSwayAngle + 0x4000;

    if (ABS(angleToVertical) < 0x100) {
        self->collider.base.acFlags |= AC_ON;
        if (self->actor.xzDistToPlayer < 80.0f * self->size) {
            EnDekubaba_SetupPrepareLunge(self);
        } else {
            EnDekubaba_SetupRecover(self);
        }
    }

    EnDekubaba_UpdateHeadPosition(self);
}

void EnDekubaba_PrunedSomersault(EnDekubaba* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3f dustPos;
    f32 deltaX;
    f32 deltaZ;
    f32 deltaY;

    Math_StepToF(&self->actor.speedXZ, 0.0f, self->size * 0.1f);

    if (self->timer == 0) {
        Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4800, 0x71C);
        Math_ScaledStepToS(&self->stemSectionAngle[0], 0x4800, 0x71C);
        Math_ScaledStepToS(&self->stemSectionAngle[1], 0x4800, 0x71C);

        EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, self->size * 3.0f, 0, self->size * 12.0f,
                                 self->size * 5.0f, 1, HAHEN_OBJECT_DEFAULT, 10, NULL);

        if ((self->actor.scale.x > 0.005f) && ((self->actor.bgCheckFlags & 2) || (self->actor.bgCheckFlags & 8))) {
            self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = 0.0f;
            self->actor.speedXZ = 0.0f;
            self->actor.flags &= -6;
            EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, self->size * 3.0f, 0, self->size * 12.0f,
                                     self->size * 5.0f, 15, HAHEN_OBJECT_DEFAULT, 10, NULL);
        }

        if (self->actor.bgCheckFlags & 2) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
            self->timer = 1;
        }
    } else if (self->timer == 1) {
        dustPos = self->actor.world.pos;

        deltaY = 20.0f * Math_SinS(self->actor.shape.rot.x);
        deltaX = -20.0f * Math_CosS(self->actor.shape.rot.x) * Math_SinS(self->actor.shape.rot.y);
        deltaZ = -20.0f * Math_CosS(self->actor.shape.rot.x) * Math_CosS(self->actor.shape.rot.y);

        for (i = 0; i < 4; i++) {
            func_800286CC(globalCtx, &dustPos, &sZeroVec, &sZeroVec, 500, 50);
            dustPos.x += deltaX;
            dustPos.y += deltaY;
            dustPos.z += deltaZ;
        }

        func_800286CC(globalCtx, &self->actor.home.pos, &sZeroVec, &sZeroVec, self->size * 500.0f, self->size * 100.0f);
        EnDekubaba_SetupDeadStickDrop(self, globalCtx);
    }
}

/**
 * Die and drop Deku Nuts (Stick drop is handled elsewhere)
 */
void EnDekubaba_ShrinkDie(EnDekubaba* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, self->size * 5.0f);

    if (Math_StepToF(&self->actor.scale.x, self->size * 0.1f * 0.01f, self->size * 0.1f * 0.01f)) {
        func_800286CC(globalCtx, &self->actor.home.pos, &sZeroVec, &sZeroVec, self->size * 500.0f, self->size * 100.0f);
        if (self->actor.dropFlag == 0) {
            Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_NUTS);

            if (self->actor.params == DEKUBABA_BIG) {
                Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_NUTS);
                Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_NUTS);
            }
        } else {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x30);
        }
        Actor_Kill(&self->actor);
    }

    self->actor.scale.y = self->actor.scale.z = self->actor.scale.x;
    self->actor.shape.rot.z += 0x1C70;
    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.home.pos, self->size * 3.0f, 0, self->size * 12.0f,
                             self->size * 5.0f, 1, HAHEN_OBJECT_DEFAULT, 10, NULL);
}

void EnDekubaba_DeadStickDrop(EnDekubaba* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (Actor_HasParent(&self->actor, globalCtx) || (self->timer == 0)) {
        Actor_Kill(&self->actor);
        return;
    }

    func_8002F554(&self->actor, globalCtx, GI_STICKS_1);
}

// Update and associated functions

void EnDekubaba_UpdateDamage(EnDekubaba* self, GlobalContext* globalCtx) {
    Vec3f* firePos;
    f32 fireScale;
    s32 phi_s0; // Used for both health and iterator

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlagJntSph(&self->actor, &self->collider, 1);

        if ((self->collider.base.colType != COLTYPE_HARD) &&
            ((self->actor.colChkInfo.damageEffect != DEKUBABA_DMGEFF_NONE) || (self->actor.colChkInfo.damage != 0))) {

            phi_s0 = self->actor.colChkInfo.health - self->actor.colChkInfo.damage;

            if (self->actionFunc != EnDekubaba_StunnedVertical) {
                if ((self->actor.colChkInfo.damageEffect == DEKUBABA_DMGEFF_BOOMERANG) ||
                    (self->actor.colChkInfo.damageEffect == DEKUBABA_DMGEFF_DEKUNUT)) {
                    if (self->actor.colChkInfo.damageEffect == DEKUBABA_DMGEFF_BOOMERANG) {
                        phi_s0 = self->actor.colChkInfo.health;
                    }

                    EnDekubaba_SetupHit(self, 2);
                } else if (self->actionFunc == EnDekubaba_PullBack) {
                    if (phi_s0 <= 0) {
                        phi_s0 = 1;
                    }

                    EnDekubaba_SetupHit(self, 1);
                } else {
                    EnDekubaba_SetupHit(self, 0);
                }
            } else if ((self->actor.colChkInfo.damageEffect == DEKUBABA_DMGEFF_BOOMERANG) ||
                       (self->actor.colChkInfo.damageEffect == DEKUBABA_DMGEFF_SWORD)) {
                if (phi_s0 > 0) {
                    EnDekubaba_SetupSway(self);
                } else {
                    EnDekubaba_SetupPrunedSomersault(self);
                }
            } else if (self->actor.colChkInfo.damageEffect != DEKUBABA_DMGEFF_DEKUNUT) {
                EnDekubaba_SetupHit(self, 0);
            } else {
                return;
            }

            self->actor.colChkInfo.health = CLAMP_MIN(phi_s0, 0);

            if (self->actor.colChkInfo.damageEffect == DEKUBABA_DMGEFF_FIRE) {
                firePos = &self->actor.world.pos;
                fireScale = (self->size * 70.0f);

                for (phi_s0 = 0; phi_s0 < 4; phi_s0++) {
                    EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, firePos, fireScale, 0, 0, phi_s0);
                }
            }
        } else {
            return;
        }
    } else if ((globalCtx->actorCtx.unk_02 != 0) && (self->collider.base.colType != COLTYPE_HARD) &&
               (self->actionFunc != EnDekubaba_StunnedVertical) && (self->actionFunc != EnDekubaba_Hit) &&
               (self->actor.colChkInfo.health != 0)) {
        self->actor.colChkInfo.health--;
        self->actor.dropFlag = 0x00;
        EnDekubaba_SetupHit(self, 1);
    } else {
        return;
    }

    if (self->actor.colChkInfo.health != 0) {
        if (self->timer == 2) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_DAMAGE);
        }
    } else {
        Enemy_StartFinishingBlow(globalCtx, &self->actor);
        if (self->actor.params == DEKUBABA_BIG) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_DEAD);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DEKU_JR_DEAD);
        }
    }
}

void EnDekubaba_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDekubaba* self = THIS;

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        EnDekubaba_SetupRecover(self);
    }

    EnDekubaba_UpdateDamage(self, globalCtx);
    self->actionFunc(self, globalCtx);

    if (self->actionFunc == EnDekubaba_PrunedSomersault) {
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, self->size * 15.0f, 10.0f, 5);
    } else if (self->actionFunc != EnDekubaba_DeadStickDrop) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        if (self->boundFloor == NULL) {
            self->boundFloor = self->actor.floorPoly;
        }
    }
    if (self->actionFunc == EnDekubaba_Lunge) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        self->actor.flags |= 0x1000000;
    }

    if (self->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if (self->actionFunc != EnDekubaba_DeadStickDrop) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

// Draw functions

void EnDekubaba_DrawStemRetracted(EnDekubaba* self, GlobalContext* globalCtx) {
    f32 horizontalScale;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2445);

    horizontalScale = self->size * 0.01f;

    Matrix_Translate(self->actor.home.pos.x, self->actor.home.pos.y + (-6.0f * self->size), self->actor.home.pos.z,
                     MTXMODE_NEW);
    Matrix_RotateRPY(self->stemSectionAngle[0], self->actor.shape.rot.y, 0, MTXMODE_APPLY);
    Matrix_Scale(horizontalScale, horizontalScale, horizontalScale, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2461),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gDekuBabaStemTopDL);

    Actor_SetFocus(&self->actor, 0.0f);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2468);
}

void EnDekubaba_DrawStemExtended(EnDekubaba* self, GlobalContext* globalCtx) {
    static Gfx* stemDLists[] = { gDekuBabaStemTopDL, gDekuBabaStemMiddleDL, gDekuBabaStemBaseDL };
    MtxF mtx;
    s32 i;
    f32 horizontalStepSize;
    f32 spA4;
    f32 scale;
    s32 stemSections;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2487);

    if (self->actionFunc == EnDekubaba_PrunedSomersault) {
        stemSections = 2;
    } else {
        stemSections = 3;
    }

    scale = self->size * 0.01f;
    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    Matrix_Get(&mtx);
    if (self->actor.colorFilterTimer != 0) {
        spA4 = self->size * 20.0f;
        self->bodyPartsPos[2].x = self->actor.world.pos.x;
        self->bodyPartsPos[2].y = self->actor.world.pos.y - spA4;
        self->bodyPartsPos[2].z = self->actor.world.pos.z;
    }

    for (i = 0; i < stemSections; i++) {
        mtx.yw += 20.0f * Math_SinS(self->stemSectionAngle[i]) * self->size;
        horizontalStepSize = 20.0f * Math_CosS(self->stemSectionAngle[i]) * self->size;
        mtx.xw -= horizontalStepSize * Math_SinS(self->actor.shape.rot.y);
        mtx.zw -= horizontalStepSize * Math_CosS(self->actor.shape.rot.y);

        Matrix_Put(&mtx);
        Matrix_RotateRPY(self->stemSectionAngle[i], self->actor.shape.rot.y, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2533),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(POLY_OPA_DISP++, stemDLists[i]);

        Collider_UpdateSpheres(51 + 2 * i, &self->collider);
        Collider_UpdateSpheres(52 + 2 * i, &self->collider);

        if (i == 0) {
            if (self->actionFunc != EnDekubaba_Sway) {
                self->actor.focus.pos.x = mtx.xw;
                self->actor.focus.pos.y = mtx.yw;
                self->actor.focus.pos.z = mtx.zw;
            } else {
                self->actor.focus.pos.x = self->actor.home.pos.x;
                self->actor.focus.pos.y = self->actor.home.pos.y + (40.0f * self->size);
                self->actor.focus.pos.z = self->actor.home.pos.z;
            }
        }

        if ((i < 2) && (self->actor.colorFilterTimer != 0)) {
            // checking colorFilterTimer ensures that spA4 has been initialized earlier, so not a bug
            self->bodyPartsPos[i].x = mtx.xw;
            self->bodyPartsPos[i].y = mtx.yw - spA4;
            self->bodyPartsPos[i].z = mtx.zw;
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2569);
}

void EnDekubaba_DrawStemBasePruned(EnDekubaba* self, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2579);

    Matrix_RotateRPY(self->stemSectionAngle[2], self->actor.shape.rot.y, 0, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2586),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gDekuBabaStemBaseDL);

    Collider_UpdateSpheres(55, &self->collider);
    Collider_UpdateSpheres(56, &self->collider);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2596);
}

void EnDekubaba_DrawBaseShadow(EnDekubaba* self, GlobalContext* globalCtx) {
    MtxF mtx;
    f32 horizontalScale;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2692);
    func_80094044(globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 0, 0, 255);

    func_80038A28(self->boundFloor, self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, &mtx);
    Matrix_Mult(&mtx, MTXMODE_NEW);

    horizontalScale = self->size * 0.15f;
    Matrix_Scale(horizontalScale, 1.0f, horizontalScale, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2710),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gCircleShadowDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2715);
}

void EnDekubaba_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnDekubaba* self = THIS;

    if (limbIndex == 1) {
        Collider_UpdateSpheres(limbIndex, &self->collider);
    }
}

void EnDekubaba_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDekubaba* self = THIS;
    f32 scale;

    if (1) {}
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2752);
    func_80093D18(globalCtx->state.gfxCtx);

    if (self->actionFunc != EnDekubaba_DeadStickDrop) {
        SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL,
                          EnDekubaba_PostLimbDraw, self);

        if (self->actionFunc == EnDekubaba_Wait) {
            EnDekubaba_DrawStemRetracted(self, globalCtx);
        } else {
            EnDekubaba_DrawStemExtended(self, globalCtx);
        }

        scale = self->size * 0.01f;
        Matrix_Translate(self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, MTXMODE_NEW);
        Matrix_RotateY(self->actor.home.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2780),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gDekuBabaBaseLeavesDL);

        if (self->actionFunc == EnDekubaba_PrunedSomersault) {
            EnDekubaba_DrawStemBasePruned(self, globalCtx);
        }

        if (self->boundFloor != NULL) {
            EnDekubaba_DrawBaseShadow(self, globalCtx);
        }

        // Display solid until 40 frames left, then blink until killed.
    } else if ((self->timer > 40) || ((self->timer % 2) != 0)) {
        Matrix_Translate(0.0f, 0.0f, 200.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2797),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gDekuBabaStickDropDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dekubaba.c", 2804);
}
