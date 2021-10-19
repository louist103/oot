#include "z_en_rd.h"
#include "objects/object_rd/object_rd.h"

#define FLAGS 0x00000415

#define THIS ((EnRd*)thisx)

void EnRd_Init(Actor* thisx, GlobalContext* globalCtx);
void EnRd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnRd_Update(Actor* thisx, GlobalContext* globalCtx);
void EnRd_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AE269C(EnRd* self);
void func_80AE2744(EnRd* self, GlobalContext* globalCtx);
void func_80AE2970(EnRd* self);
void func_80AE2A10(EnRd* self, GlobalContext* globalCtx);
void func_80AE2C1C(EnRd* self, GlobalContext* globalCtx);
void func_80AE2F50(EnRd* self, GlobalContext* globalCtx);
void func_80AE2FD0(EnRd* self, GlobalContext* globalCtx);
void func_80AE31DC(EnRd* self);
void func_80AE3260(EnRd* self, GlobalContext* globalCtx);
void func_80AE33F0(EnRd* self);
void func_80AE392C(EnRd* self);
void func_80AE39D4(EnRd* self);
void func_80AE3454(EnRd* self, GlobalContext* globalCtx);
void func_80AE37BC(EnRd* self);
void func_80AE3834(EnRd* self, GlobalContext* globalCtx);
void func_80AE3978(EnRd* self, GlobalContext* globalCtx);
void func_80AE3A54(EnRd* self, GlobalContext* globalCtx);
void func_80AE3B18(EnRd* self, GlobalContext* globalCtx);
void func_80AE3C98(EnRd* self, GlobalContext* globalCtx);
void func_80AE3ECC(EnRd* self, GlobalContext* globalCtx);

const ActorInit En_Rd_InitVars = {
    ACTOR_EN_RD,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_RD,
    sizeof(EnRd),
    (ActorFunc)EnRd_Init,
    (ActorFunc)EnRd_Destroy,
    (ActorFunc)EnRd_Update,
    (ActorFunc)EnRd_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 70, 0, { 0, 0, 0 } },
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(2, 0xF),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0xF),
    /* Hookshot      */ DMG_ENTRY(0, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0xE),
    /* Ice magic     */ DMG_ENTRY(0, 0x6),
    /* Light magic   */ DMG_ENTRY(3, 0xD),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xF),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xF),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3500, ICHAIN_STOP),
};

static Vec3f D_80AE4918 = { 0.0f, 0.0f, 0.0f };

// I'm guessing these are primitive and environment colors that go unused
static Color_RGBA8 D_80AE4924 = { 200, 200, 255, 255 };
static Color_RGBA8 D_80AE4928 = { 0, 0, 255, 0 };

static Vec3f D_80AE492C = { 0.0f, 0.0f, 0.0f };
static Color_RGBA8 D_80AE4938 = { 200, 200, 255, 255 };
static Color_RGBA8 D_80AE493C = { 0, 0, 255, 0 };

static Vec3f D_80AE4940 = { 300.0f, 0.0f, 0.0f };
static Vec3f D_80AE494C = { 300.0f, 0.0f, 0.0f };
static Vec3f D_80AE4958 = { 0.25f, 0.25f, 0.25f };

void EnRd_SetupAction(EnRd* self, EnRdActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnRd_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnRd* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    thisx->targetMode = 0;
    thisx->colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    self->unk_310 = self->unk_30E = 0;
    thisx->focus.pos = thisx->world.pos;
    thisx->focus.pos.y += 50.0f;
    thisx->colChkInfo.mass = MASS_HEAVY;
    thisx->colChkInfo.health = 8;
    self->unk_314 = self->unk_31D = 0xFF;
    self->unk_312 = (thisx->params & 0xFF00) >> 8;

    if (thisx->params & 0x80) {
        thisx->params |= 0xFF00;
    } else {
        thisx->params &= 0xFF;
    }

    if (thisx->params >= -1) {
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_rd_Skel_00E778, &object_rd_Anim_0087D0,
                           self->jointTable, self->morphTable, 26);
        thisx->naviEnemyId = 0x2A;
    } else {
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_rd_Skel_003DD8, &object_rd_Anim_0087D0,
                           self->jointTable, self->morphTable, 26);
        thisx->naviEnemyId = 0x2D;
    }

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, thisx, &sCylinderInit);

    if (thisx->params >= -2) {
        func_80AE269C(self);
    } else {
        func_80AE2970(self);
    }

    SkelAnime_Update(&self->skelAnime);

    if (thisx->params == 3) {
        thisx->flags |= 0x80;
    }
}

void EnRd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnRd* self = THIS;

    if (gSaveContext.sunsSongState != SUNSSONG_INACTIVE) {
        gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
    }
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80AE2630(GlobalContext* globalCtx, Actor* thisx, s32 arg2) {
    Actor* enemyIt = globalCtx->actorCtx.actorLists[ACTORCAT_ENEMY].head;

    while (enemyIt != NULL) {
        if ((enemyIt->id != ACTOR_EN_RD) || (enemyIt == thisx) || (enemyIt->params < 0)) {
            enemyIt = enemyIt->next;
            continue;
        }

        if (arg2 != 0) {
            enemyIt->parent = thisx;
        } else if (enemyIt->parent == thisx) {
            enemyIt->parent = NULL;
        }
        enemyIt = enemyIt->next;
    }
}

void func_80AE269C(EnRd* self) {
    if (self->actor.params != 2) {
        Animation_MorphToLoop(&self->skelAnime, &object_rd_Anim_0087D0, -6.0f);
    } else {
        Animation_PlayLoop(&self->skelAnime, &object_rd_Anim_005D98);
    }

    self->unk_31B = 0;
    self->unk_30C = (Rand_ZeroOne() * 10.0f) + 5.0f;
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    EnRd_SetupAction(self, func_80AE2744);
}

void func_80AE2744(EnRd* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->unk_30E, 0, 1, 0x64, 0);
    Math_SmoothStepToS(&self->unk_310, 0, 1, 0x64, 0);

    if ((self->actor.params == 2) && (0.0f == self->skelAnime.curFrame)) {
        if (Rand_ZeroOne() >= 0.5f) {
            Animation_PlayLoop(&self->skelAnime, &object_rd_Anim_005D98);
        } else {
            Animation_PlayLoop(&self->skelAnime, &object_rd_Anim_0057AC);
        }
    } else {
        self->unk_30C--;
        if (self->unk_30C == 0) {
            self->unk_30C = (Rand_ZeroOne() * 10.0f) + 10.0f;
            self->skelAnime.curFrame = 0.0f;
        }
    }

    if (self->actor.parent != NULL) {
        if (self->unk_305 == 0) {
            if (self->actor.params != 2) {
                func_80AE31DC(self);
            } else {
                func_80AE392C(self);
            }
        }
    } else {
        if (self->unk_305 != 0) {
            if (self->actor.params != 2) {
                func_80AE37BC(self);
            } else {
                func_80AE392C(self);
            }
        }

        self->unk_305 = 0;
        if ((self->actor.xzDistToPlayer <= 150.0f) && func_8002DDE4(globalCtx)) {
            if ((self->actor.params != 2) && (self->unk_305 == 0)) {
                func_80AE37BC(self);
            } else {
                func_80AE392C(self);
            }
        }
    }

    if ((globalCtx->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_CRY);
    }
}

void func_80AE2970(EnRd* self) {
    Animation_Change(&self->skelAnime, &object_rd_Anim_0087D0, 0, 0, Animation_GetLastFrame(&object_rd_Anim_0087D0),
                     ANIMMODE_LOOP, -6.0f);
    self->unk_31B = 11;
    self->unk_30C = 6;
    self->actor.shape.rot.x = -0x4000;
    self->actor.gravity = 0.0f;
    self->actor.shape.yOffset = 0.0f;
    self->actor.speedXZ = 0.0f;
    EnRd_SetupAction(self, func_80AE2A10);
}

// Rising out of coffin
void func_80AE2A10(EnRd* self, GlobalContext* globalCtx) {
    if (self->actor.shape.rot.x != -0x4000) {
        Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 0x7D0, 0);
        if (Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.3f, 2.0f, 0.3f) == 0.0f) {
            self->actor.gravity = -3.5f;
            func_80AE269C(self);
        }
    } else {
        if (self->actor.world.pos.y == self->actor.home.pos.y) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_CRY);
        }
        if (Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 50.0f, 0.3f, 2.0f, 0.3f) == 0.0f) {
            if (self->unk_30C != 0) {
                self->unk_30C--;
                Math_SmoothStepToF(&self->actor.speedXZ, 6.0f, 0.3f, 1.0f, 0.3f);
            } else if (Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.3f, 1.0f, 0.3f) == 0.0f) {
                Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 0x7D0, 0);
            }
        }
    }
}

void func_80AE2B90(EnRd* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &object_rd_Anim_00EFDC, 1.0f, 4.0f,
                     Animation_GetLastFrame(&object_rd_Anim_00EFDC), ANIMMODE_LOOP_INTERP, -4.0f);
    self->actor.speedXZ = 0.4f;
    self->unk_31B = 4;
    EnRd_SetupAction(self, func_80AE2C1C);
}

void func_80AE2C1C(EnRd* self, GlobalContext* globalCtx) {
    Vec3f sp44 = D_80AE4918;
    Color_RGBA8 sp40 = D_80AE4924;
    Color_RGBA8 sp3C = D_80AE4928;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    s16 sp32 = self->actor.yawTowardsPlayer - self->actor.shape.rot.y - self->unk_30E - self->unk_310;

    self->skelAnime.playSpeed = self->actor.speedXZ;
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0xFA, 0);
    Math_SmoothStepToS(&self->unk_30E, 0, 1, 0x64, 0);
    Math_SmoothStepToS(&self->unk_310, 0, 1, 0x64, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    SkelAnime_Update(&self->skelAnime);

    if (Actor_WorldDistXYZToPoint(&player->actor, &self->actor.home.pos) >= 150.0f) {
        func_80AE2F50(self, globalCtx);
    }

    if ((ABS(sp32) < 0x1554) && (Actor_WorldDistXYZToActor(&self->actor, &player->actor) <= 150.0f)) {
        if (!(player->stateFlags1 & 0x2C6080) && !(player->stateFlags2 & 0x80)) {
            if (self->unk_306 == 0) {
                if (!(self->unk_312 & 0x80)) {
                    player->actor.freezeTimer = 40;
                    func_8008EEAC(globalCtx, &self->actor);
                    GET_PLAYER(globalCtx)->unk_684 = &self->actor;
                    func_800AA000(self->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
                }
                self->unk_306 = 0x3C;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_AIM);
            }
        } else {
            func_80AE2F50(self, globalCtx);
        }
    }

    if (self->unk_307 != 0) {
        self->unk_307--;
    }

    if (!self->unk_307 && (Actor_WorldDistXYZToActor(&self->actor, &player->actor) <= 45.0f) &&
        Actor_IsFacingPlayer(&self->actor, 0x38E3)) {
        player->actor.freezeTimer = 0;
        if (globalCtx->grabPlayer(globalCtx, player)) {
            self->actor.flags &= ~1;
            func_80AE33F0(self);
        }
    } else if (self->actor.params > 0) {
        if (self->actor.parent != NULL) {
            func_80AE31DC(self);
        } else {
            self->unk_305 = 0;
        }
    }

    if ((self->skelAnime.curFrame == 10.0f) || (self->skelAnime.curFrame == 22.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_WALK);
    } else if ((globalCtx->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_CRY);
    }
}

void func_80AE2F50(EnRd* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &object_rd_Anim_00EFDC, 0.5f, 0, Animation_GetLastFrame(&object_rd_Anim_00EFDC),
                     ANIMMODE_LOOP_INTERP, -4.0f);
    self->unk_31B = 2;
    EnRd_SetupAction(self, func_80AE2FD0);
}

void func_80AE2FD0(EnRd* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    s16 targetY = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);

    if (Actor_WorldDistXYZToPoint(&self->actor, &self->actor.home.pos) >= 5.0f) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, targetY, 1, 0x1C2, 0);
    } else {
        self->actor.speedXZ = 0.0f;
        if (Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, 1, 0x1C2, 0) == 0) {
            if (self->actor.params != 2) {
                func_80AE269C(self);
            } else {
                func_80AE39D4(self);
            }
        }
    }

    Math_SmoothStepToS(&self->unk_30E, 0, 1, 0x64, 0);
    Math_SmoothStepToS(&self->unk_310, 0, 1, 0x64, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    SkelAnime_Update(&self->skelAnime);

    if (!(player->stateFlags1 & 0x2C6080) && !(player->stateFlags2 & 0x80) &&
        (Actor_WorldDistXYZToPoint(&player->actor, &self->actor.home.pos) < 150.0f)) {
        self->actor.targetMode = 0;
        func_80AE2B90(self, globalCtx);
    } else if (self->actor.params > 0) {
        if (self->actor.parent != NULL) {
            func_80AE31DC(self);
        } else {
            self->unk_305 = 0;
        }
    }

    if (self->skelAnime.curFrame == 10.0f || self->skelAnime.curFrame == 22.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_WALK);
    } else if ((globalCtx->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_CRY);
    }
}

void func_80AE31DC(EnRd* self) {
    Animation_Change(&self->skelAnime, &object_rd_Anim_00EFDC, 0.5f, 0, Animation_GetLastFrame(&object_rd_Anim_00EFDC),
                     ANIMMODE_LOOP_INTERP, -4.0f);
    self->unk_31B = 3;
    self->unk_305 = 1;
    EnRd_SetupAction(self, func_80AE3260);
}

void func_80AE3260(EnRd* self, GlobalContext* globalCtx) {
    if (self->actor.parent != NULL) {
        s32 pad;
        s16 targetY;
        Vec3f thisPos = self->actor.parent->world.pos;

        targetY = Actor_WorldYawTowardPoint(&self->actor, &thisPos);

        Math_SmoothStepToS(&self->actor.shape.rot.y, targetY, 1, 0xFA, 0);

        if (Actor_WorldDistXYZToPoint(&self->actor, &thisPos) >= 45.0f) {
            self->actor.speedXZ = 0.4f;
        } else {
            self->actor.speedXZ = 0.0f;

            if (self->actor.params != 2) {
                func_80AE269C(self);
            } else {
                func_80AE39D4(self);
            }
        }

        Math_SmoothStepToS(&self->unk_30E, 0, 1, 0x64, 0);
        Math_SmoothStepToS(&self->unk_310, 0, 1, 0x64, 0);
    } else {
        func_80AE2B90(self, globalCtx);
    }

    self->actor.world.rot.y = self->actor.shape.rot.y;
    SkelAnime_Update(&self->skelAnime);

    if (self->skelAnime.curFrame == 10.0f || self->skelAnime.curFrame == 22.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_WALK);
    } else if ((globalCtx->gameplayFrames & 0x5F) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_CRY);
    }
}

void func_80AE33F0(EnRd* self) {
    Animation_PlayOnce(&self->skelAnime, &object_rd_Anim_004ADC);
    self->unk_30C = self->unk_304 = 0;
    self->unk_319 = 200;
    self->unk_31B = 8;
    self->actor.speedXZ = 0.0f;
    EnRd_SetupAction(self, func_80AE3454);
}

void func_80AE3454(EnRd* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    if (SkelAnime_Update(&self->skelAnime)) {
        self->unk_304++;
    }

    switch (self->unk_304) {
        case 1:
            Animation_PlayLoop(&self->skelAnime, &object_rd_Anim_004268);
            self->unk_304++;
            globalCtx->damagePlayer(globalCtx, -8);
            func_800AA000(self->actor.xzDistToPlayer, 0xFF, 1, 0xC);
            self->unk_319 = 20;
        case 0:
            Math_SmoothStepToS(&self->unk_30E, 0, 1, 0x5DC, 0);
            Math_SmoothStepToS(&self->unk_310, 0, 1, 0x5DC, 0);
        case 2:
            if (!(player->stateFlags2 & 0x80)) {
                Animation_Change(&self->skelAnime, &object_rd_Anim_0046F8, 0.5f, 0.0f,
                                 Animation_GetLastFrame(&object_rd_Anim_0046F8), ANIMMODE_ONCE_INTERP, 0.0f);
                self->unk_304++;
                self->unk_31B = 4;
                return;
            }

            if (!LINK_IS_ADULT) {
                Math_SmoothStepToF(&self->actor.shape.yOffset, -1500.0f, 1.0f, 150.0f, 0.0f);
            }

            Math_SmoothStepToF(&self->actor.world.pos.x,
                               (Math_SinS(player->actor.shape.rot.y) * -25.0f) + player->actor.world.pos.x, 1.0f, 10.0f,
                               0.0f);
            Math_SmoothStepToF(&self->actor.world.pos.y, player->actor.world.pos.y, 1.0f, 10.0f, 0.0f);
            Math_SmoothStepToF(&self->actor.world.pos.z,
                               (Math_CosS(player->actor.shape.rot.y) * -25.0f) + player->actor.world.pos.z, 1.0f, 10.0f,
                               0.0f);
            Math_SmoothStepToS(&self->actor.shape.rot.y, player->actor.shape.rot.y, 1, 0x1770, 0);

            if (self->skelAnime.curFrame == 0.0f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_ATTACK);
            }
            self->unk_319--;

            if (self->unk_319 == 0) {
                globalCtx->damagePlayer(globalCtx, -8);
                func_800AA000(self->actor.xzDistToPlayer, 0xF0, 1, 0xC);
                self->unk_319 = 20;
                func_8002F7DC(&player->actor, NA_SE_VO_LI_DAMAGE_S + player->ageProperties->unk_92);
            }
            break;
        case 3:
            if (!LINK_IS_ADULT) {
                Math_SmoothStepToF(&self->actor.shape.yOffset, 0, 1.0f, 400.0f, 0.0f);
            }
            break;
        case 4:
            if (!LINK_IS_ADULT) {
                Math_SmoothStepToF(&self->actor.shape.yOffset, 0, 1.0f, 400.0f, 0.0f);
            }
            self->actor.targetMode = 0;
            self->actor.flags |= 1;
            self->unk_306 = 0xA;
            self->unk_307 = 0xF;
            func_80AE2B90(self, globalCtx);
            break;
    }
}

void func_80AE37BC(EnRd* self) {
    Animation_Change(&self->skelAnime, &object_rd_Anim_004F94, 0.0f, 0.0f,
                     Animation_GetLastFrame(&object_rd_Anim_004F94), ANIMMODE_ONCE, 0.0f);
    self->unk_31B = 7;
    EnRd_SetupAction(self, func_80AE3834);
}

void func_80AE3834(EnRd* self, GlobalContext* globalCtx) {
    Vec3f sp34 = D_80AE492C;
    Color_RGBA8 sp30 = D_80AE4938;
    Color_RGBA8 sp2C = D_80AE493C;
    Player* player = GET_PLAYER(globalCtx);
    s16 temp_v0 = self->actor.yawTowardsPlayer - self->actor.shape.rot.y - self->unk_30E - self->unk_310;

    if (ABS(temp_v0) < 0x2008) {
        if (!(self->unk_312 & 0x80)) {
            player->actor.freezeTimer = 60;
            func_800AA000(self->actor.xzDistToPlayer, 0xFF, 0x14, 0x96);
            func_8008EEAC(globalCtx, &self->actor);
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_AIM);
        func_80AE2B90(self, globalCtx);
    }
}

void func_80AE392C(EnRd* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_rd_Anim_008040, -4.0f);
    self->unk_31B = 5;
    EnRd_SetupAction(self, func_80AE3978);
}

void func_80AE3978(EnRd* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.parent != NULL) {
            func_80AE31DC(self);
        } else {
            func_80AE37BC(self);
        }
    }
}

void func_80AE39D4(EnRd* self) {
    Animation_Change(&self->skelAnime, &object_rd_Anim_008040, -1.0f, Animation_GetLastFrame(&object_rd_Anim_008040),
                     0.0f, ANIMMODE_ONCE, -4.0f);
    self->unk_31B = 6;
    EnRd_SetupAction(self, func_80AE3A54);
}

void func_80AE3A54(EnRd* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        func_80AE269C(self);
    }
}

void func_80AE3A8C(EnRd* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_rd_Anim_0074F0, -6.0f);

    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = -2.0f;
    }

    self->actor.flags |= 1;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_DAMAGE);
    self->unk_31B = 9;
    EnRd_SetupAction(self, func_80AE3B18);
}

void func_80AE3B18(EnRd* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->actor.speedXZ < 0.0f) {
        self->actor.speedXZ += 0.15f;
    }

    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Math_SmoothStepToS(&self->unk_30E, 0, 1, 0x12C, 0);
    Math_SmoothStepToS(&self->unk_310, 0, 1, 0x12C, 0);
    if (SkelAnime_Update(&self->skelAnime)) {
        self->actor.world.rot.y = self->actor.shape.rot.y;

        if (self->actor.parent != NULL) {
            func_80AE31DC(self);
        } else if (Actor_WorldDistXYZToPoint(&player->actor, &self->actor.home.pos) >= 150.0f) {
            func_80AE2F50(self, globalCtx);
        } else {
            func_80AE2B90(self, globalCtx);
        }

        self->unk_31D = 0xFF;
    }
}

void func_80AE3C20(EnRd* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_rd_Anim_006E88, -1.0f);
    self->unk_31B = 10;
    self->unk_30C = 300;
    self->actor.flags &= ~1;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_REDEAD_DEAD);
    EnRd_SetupAction(self, func_80AE3C98);
}

void func_80AE3C98(EnRd* self, GlobalContext* globalCtx) {
    if (self->actor.category != ACTORCAT_PROP) {
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
    }

    Math_SmoothStepToS(&self->unk_30E, 0, 1, 0x7D0, 0);
    Math_SmoothStepToS(&self->unk_310, 0, 1, 0x7D0, 0);

    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->unk_30C == 0) {
            if (!Flags_GetSwitch(globalCtx, self->unk_312 & 0x7F)) {
                Flags_SetSwitch(globalCtx, self->unk_312 & 0x7F);
            }
            if (self->unk_314 != 0) {
                if (self->unk_314 == 0xB4) {
                    func_80AE2630(globalCtx, &self->actor, 0);
                }
                self->actor.scale.y -= 0.000075f;
                self->unk_314 -= 5;
            } else {
                Actor_Kill(&self->actor);
            }
        } else {
            self->unk_30C--;
        }
    } else if (((s32)self->skelAnime.curFrame == 33) || ((s32)self->skelAnime.curFrame == 40)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIZA_DOWN);
    }
}

void func_80AE3DE4(EnRd* self) {
    self->unk_31B = 1;
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (gSaveContext.sunsSongState != SUNSSONG_INACTIVE) {
        self->unk_318 = 1;
        self->unk_316 = 0x258;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIGHT_ARROW_HIT);
        Actor_SetColorFilter(&self->actor, -0x8000, -0x7F38, 0, 0xFF);
    } else if (self->unk_31C == 1) {
        Actor_SetColorFilter(&self->actor, 0, 0xC8, 0, 0x50);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_LIGHT_ARROW_HIT);
        Actor_SetColorFilter(&self->actor, -0x8000, 0xC8, 0, 0x50);
    }
    EnRd_SetupAction(self, func_80AE3ECC);
}

void func_80AE3ECC(EnRd* self, GlobalContext* globalCtx) {
    if ((self->unk_318 != 0) && (self->unk_316 != 0)) {
        self->unk_316--;
        if (self->unk_316 >= 0xFF) {
            Actor_SetColorFilter(&self->actor, -0x8000, 0xC8, 0, 0xFF);
        }
        if (self->unk_316 == 0) {
            self->unk_318 = 0;
            gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
        }
    }

    if (self->actor.colorFilterTimer == 0) {
        if (self->actor.colChkInfo.health == 0) {
            func_80AE2630(globalCtx, &self->actor, 1);
            func_80AE3C20(self);
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x90);
        } else {
            func_80AE3A8C(self);
        }
    }
}

void func_80AE3F9C(EnRd* self, GlobalContext* globalCtx) {
    s16 temp1;
    s16 temp2;
    s16 temp3;

    temp1 = self->actor.yawTowardsPlayer - (s16)(self->actor.shape.rot.y + self->unk_310);
    temp2 = CLAMP(temp1, -500, 500);

    temp1 -= self->unk_30E;
    temp3 = CLAMP(temp1, -500, 500);

    if ((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y) >= 0) {
        self->unk_310 += ABS(temp2);
        self->unk_30E += ABS(temp3);
    } else {
        self->unk_310 -= ABS(temp2);
        self->unk_30E -= ABS(temp3);
    }

    self->unk_310 = CLAMP(self->unk_310, -18783, 18783);
    self->unk_30E = CLAMP(self->unk_30E, -9583, 9583);
}

void func_80AE4114(EnRd* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    if ((gSaveContext.sunsSongState != SUNSSONG_INACTIVE) && (self->actor.shape.rot.x == 0) && (self->unk_318 == 0) &&
        (self->unk_31B != 9) && (self->unk_31B != 10) && (self->unk_31B != 1)) {
        func_80AE3DE4(self);
        return;
    }

    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        self->unk_31C = self->actor.colChkInfo.damageEffect;

        if (self->unk_31B != 11) {
            Actor_SetDropFlag(&self->actor, &self->collider.info, 1);
            if (player->unk_844 != 0) {
                self->unk_31D = player->unk_845;
            }

            if ((self->unk_31C != 0) && (self->unk_31C != 6)) {
                if (((self->unk_31C == 1) || (self->unk_31C == 13)) && (self->unk_31B != 1)) {
                    Actor_ApplyDamage(&self->actor);
                    func_80AE3DE4(self);
                    return;
                }

                self->unk_318 = 0;
                self->unk_316 = 0;

                if (self->unk_31C == 0xE) {
                    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 0x50);
                    self->unk_31A = 0x28;
                } else {
                    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
                }

                Actor_ApplyDamage(&self->actor);
                if (self->actor.colChkInfo.health == 0) {
                    func_80AE2630(globalCtx, &self->actor, 1);
                    func_80AE3C20(self);
                    Item_DropCollectibleRandom(globalCtx, 0, &self->actor.world.pos, 0x90);
                } else {
                    func_80AE3A8C(self);
                }
            }
        }
    }
}

void EnRd_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnRd* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad2;

    func_80AE4114(self, globalCtx);

    if (gSaveContext.sunsSongState != SUNSSONG_INACTIVE && self->unk_318 == 0) {
        gSaveContext.sunsSongState = SUNSSONG_INACTIVE;
    }

    if (self->unk_31C != 6 && ((self->unk_31B != 11) || (self->unk_31C != 14))) {
        if (self->unk_306 != 0) {
            self->unk_306--;
        }

        self->actionFunc(self, globalCtx);
        if (self->unk_31B != 8 && self->actor.speedXZ != 0.0f) {
            Actor_MoveForward(&self->actor);
        }

        if ((self->actor.shape.rot.x == 0) && (self->unk_31B != 8) && (self->actor.speedXZ != 0.0f)) {
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 30.0f, 20.0f, 35.0f, 0x1D);
        }

        if (self->unk_31B == 7) {
            func_80AE3F9C(self, globalCtx);
        }
    }

    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 50.0f;

    if ((self->actor.colChkInfo.health > 0) && (self->unk_31B != 8)) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        if ((self->unk_31B != 9) || ((player->unk_844 != 0) && (player->unk_845 != self->unk_31D))) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

s32 EnRd_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                          Gfx** gfx) {
    EnRd* self = THIS;

    if (limbIndex == 23) {
        rot->y += self->unk_30E;
    } else if (limbIndex == 12) {
        rot->y += self->unk_310;
    }
    return false;
}

void EnRd_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    Vec3f sp2C = D_80AE4940;
    EnRd* self = THIS;
    s32 idx = -1;
    Vec3f destPos;

    if ((self->unk_31A != 0) || ((self->actor.colorFilterTimer != 0) && (self->actor.colorFilterParams & 0x4000))) {
        switch (limbIndex - 1) {
            case 23:
                idx = 0;
                break;
            case 0:
                idx = 1;
                break;
            case 21:
                idx = 2;
                break;
            case 17:
                idx = 3;
                break;
            case 13:
                idx = 4;
                break;
            case 24:
                idx = 5;
                break;
            case 8:
                idx = 6;
                break;
            case 3:
                idx = 7;
                break;
            case 10:
                idx = 8;
                break;
            case 5:
                idx = 9;
                break;
        }

        if (idx >= 0) {
            Matrix_MultVec3f(&sp2C, &destPos);
            self->firePos[idx].x = destPos.x;
            self->firePos[idx].y = destPos.y;
            self->firePos[idx].z = destPos.z;
        }
    }
}

void EnRd_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnRd* self = THIS;
    Vec3f thisPos = thisx->world.pos;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_rd.c", 1679);

    if (self->unk_314 == 0xFF) {
        func_80093D18(globalCtx->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, self->unk_314);
        gSPSegment(POLY_OPA_DISP++, 8, &D_80116280[2]);
        POLY_OPA_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           self->skelAnime.dListCount, EnRd_OverrideLimbDraw, EnRd_PostLimbDraw, self,
                                           POLY_OPA_DISP);
        func_80033C30(&thisPos, &D_80AE4958, 255, globalCtx);
        if (self->unk_31A != 0) {
            thisx->colorFilterTimer++;
            self->unk_31A--;
            if (self->unk_31A % 4 == 0) {
                EffectSsEnFire_SpawnVec3s(globalCtx, thisx, &self->firePos[self->unk_31A >> 2], 0x4B, 0, 0,
                                          (self->unk_31A >> 2));
            }
        }
    } else {
        func_80093D84(globalCtx->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->unk_314);
        gSPSegment(POLY_XLU_DISP++, 8, &D_80116280[0]);
        POLY_XLU_DISP =
            SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                               self->skelAnime.dListCount, EnRd_OverrideLimbDraw, NULL, self, POLY_XLU_DISP);

        func_80033C30(&thisPos, &D_80AE4958, self->unk_314, globalCtx);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_rd.c", 1735);
}
