#include "z_en_skb.h"
#include "overlays/actors/ovl_En_Encount1/z_en_encount1.h"
#include "objects/object_skb/object_skb.h"

#define FLAGS 0x00000015

#define THIS ((EnSkb*)thisx)

void EnSkb_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSkb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSkb_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSkb_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AFCD60(EnSkb* self);
void func_80AFCDF8(EnSkb* self);
void func_80AFCE5C(EnSkb* self, GlobalContext* globalCtx);
void func_80AFCF48(EnSkb* self);
void func_80AFCFF0(EnSkb* self, GlobalContext* globalCtx);
void func_80AFD0A4(EnSkb* self);
void EnSkb_Advance(EnSkb* self, GlobalContext* globalCtx);
void func_80AFD33C(EnSkb* self);
void EnSkb_SetupAttack(EnSkb* self, GlobalContext* globalCtx);
void func_80AFD47C(EnSkb* self);
void func_80AFD508(EnSkb* self, GlobalContext* globalCtx);
void EnSkb_SetupStunned(EnSkb* self);
void func_80AFD59C(EnSkb* self, GlobalContext* globalCtx);
void func_80AFD6CC(EnSkb* self, GlobalContext* globalCtx);
void func_80AFD7B4(EnSkb* self, GlobalContext* globalCtx);
void func_80AFD880(EnSkb* self, GlobalContext* globalCtx);
void func_80AFD968(EnSkb* self, GlobalContext* globalCtx);

static ColliderJntSphElementInit sJntSphElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 15, { { 0, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 20 }, 100 },
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
    2,
    sJntSphElementsInit,
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x1),
    /* Deku stick    */ DMG_ENTRY(2, 0xF),
    /* Slingshot     */ DMG_ENTRY(1, 0xF),
    /* Explosive     */ DMG_ENTRY(2, 0xF),
    /* Boomerang     */ DMG_ENTRY(0, 0x1),
    /* Normal arrow  */ DMG_ENTRY(2, 0xF),
    /* Hammer swing  */ DMG_ENTRY(2, 0xF),
    /* Hookshot      */ DMG_ENTRY(0, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xE),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(4, 0x7),
    /* Ice arrow     */ DMG_ENTRY(2, 0xF),
    /* Light arrow   */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0x7),
    /* Ice magic     */ DMG_ENTRY(0, 0x6),
    /* Light magic   */ DMG_ENTRY(3, 0xD),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0xD),
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

const ActorInit En_Skb_InitVars = {
    ACTOR_EN_SKB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SKB,
    sizeof(EnSkb),
    (ActorFunc)EnSkb_Init,
    (ActorFunc)EnSkb_Destroy,
    (ActorFunc)EnSkb_Update,
    (ActorFunc)EnSkb_Draw,
};

void EnSkb_SetupAction(EnSkb* self, EnSkbActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnSkb_SpawnDebris(GlobalContext* globalCtx, EnSkb* self, Vec3f* spawnPos) {
    Vec3f pos;
    Vec3f vel = { 0.0f, 8.0f, 0.0f };
    Vec3f accel = { 0.0f, -1.5f, 0.0f };
    f32 spreadAngle;
    f32 scale;

    spreadAngle = (Rand_ZeroOne() - 0.5f) * 6.28f;
    pos.y = self->actor.floorHeight;
    pos.x = (Math_SinF(spreadAngle) * 15.0f) + spawnPos->x;
    pos.z = (Math_CosF(spreadAngle) * 15.0f) + spawnPos->z;
    accel.x = Rand_CenteredFloat(1.0f);
    accel.z = Rand_CenteredFloat(1.0f);
    vel.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    scale = (Rand_ZeroOne() * 5.0f) + 12.0f;
    EffectSsHahen_Spawn(globalCtx, &pos, &vel, &accel, 2, scale * 0.8f, -1, 10, 0);
    func_80033480(globalCtx, &pos, 10.0f, 1, 150, 0, 1);
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_STOP),
};

void EnSkb_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSkb* self = THIS;
    s16 paramOffsetBody;
    s16 paramOffsetArm;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.colChkInfo.damageTable = &sDamageTable;
    ActorShape_Init(&self->actor.shape, 0.0f, &ActorShadow_DrawCircle, 0.0f);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.colChkInfo.mass = 0xFE;
    self->actor.colChkInfo.health = 2;
    self->actor.shape.yOffset = -8000.0f;
    SkelAnime_Init(globalCtx, &self->skelAnime, &gStalchildSkel, &gStalchildUncurlingAnim, self->jointTable,
                   self->morphTable, 20);
    self->actor.naviEnemyId = 0x55;

    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItem);
    Actor_SetScale(&self->actor, ((self->actor.params * 0.1f) + 1.0f) * 0.01f);

    paramOffsetBody = self->actor.params + 0xA;
    self->collider.elements[0].dim.worldSphere.radius = paramOffsetBody;
    self->collider.elements[0].dim.modelSphere.radius = paramOffsetBody;
    if (1) {};
    paramOffsetArm = (self->actor.params * 2) + 0x14;
    self->collider.elements[1].dim.worldSphere.radius = paramOffsetArm;
    self->collider.elements[1].dim.modelSphere.radius = paramOffsetArm;
    self->actor.home.pos = self->actor.world.pos;
    self->actor.floorHeight = self->actor.world.pos.y;
    func_80AFCDF8(self);
}

void EnSkb_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnSkb* self = THIS;
    if (self->actor.parent != NULL) {
        EnEncount1* spawner = (EnEncount1*)self->actor.parent;

        if (spawner->actor.update != NULL) {
            if (spawner->curNumSpawn > 0) {
                spawner->curNumSpawn--;
            }
        }
    }
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void func_80AFCD60(EnSkb* self) {
    if (IS_DAY) {
        func_80AFCF48(self);
    } else if (Actor_IsFacingPlayer(&self->actor, 0x11C7) &&
               (self->actor.xzDistToPlayer < (60.0f + (self->actor.params * 6.0f)))) {
        func_80AFD33C(self);
    } else {
        func_80AFD0A4(self);
    }
}

void func_80AFCDF8(EnSkb* self) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, &gStalchildUncurlingAnim, 1.0f);
    self->unk_280 = 0;
    self->actor.flags &= ~1;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIVA_APPEAR);
    EnSkb_SetupAction(self, func_80AFCE5C);
}

void func_80AFCE5C(EnSkb* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame < 4.0f) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    } else {
        self->actor.flags |= 1;
    }
    Math_SmoothStepToF(&self->actor.shape.yOffset, 0.0f, 1.0f, 800.0f, 0.0f);
    Math_SmoothStepToF(&self->actor.shape.shadowScale, 25.0f, 1.0f, 2.5f, 0.0f);
    if ((globalCtx->gameplayFrames & 1) != 0) {
        EnSkb_SpawnDebris(globalCtx, self, &self->actor.world.pos);
    }
    if ((SkelAnime_Update(&self->skelAnime) != 0) && (0.0f == self->actor.shape.yOffset)) {
        func_80AFCD60(self);
    }
}

void func_80AFCF48(EnSkb* self) {
    Animation_Change(&self->skelAnime, &gStalchildUncurlingAnim, -1.0f,
                     Animation_GetLastFrame(&gStalchildUncurlingAnim), 0.0f, ANIMMODE_ONCE, -4.0f);
    self->unk_280 = 0;
    self->unk_281 = 0;
    self->actor.flags &= ~1;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_AKINDONUTS_HIDE);
    EnSkb_SetupAction(self, func_80AFCFF0);
}

void func_80AFCFF0(EnSkb* self, GlobalContext* globalCtx) {
    if ((Math_SmoothStepToF(&self->actor.shape.yOffset, -8000.0f, 1.0f, 500.0f, 0.0f) != 0.0f) &&
        (globalCtx->gameplayFrames & 1)) {
        EnSkb_SpawnDebris(globalCtx, self, &self->actor.world.pos);
    }
    Math_SmoothStepToF(&self->actor.shape.shadowScale, 0.0f, 1.0f, 2.5f, 0.0f);
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        Actor_Kill(&self->actor);
    }
}

void func_80AFD0A4(EnSkb* self) {
    Animation_Change(&self->skelAnime, &gStalchildWalkingAnim, 0.96000004f, 0.0f,
                     Animation_GetLastFrame(&gStalchildWalkingAnim), ANIMMODE_LOOP, -4.0f);
    self->unk_280 = 4;
    self->unk_288 = 0;
    self->actor.speedXZ = self->actor.scale.y * 160.0f;
    EnSkb_SetupAction(self, EnSkb_Advance);
}

void EnSkb_Advance(EnSkb* self, GlobalContext* globalCtx) {
    s32 thisKeyFrame;
    s32 prevKeyFrame;
    f32 playSpeed;
    Player* player = GET_PLAYER(globalCtx);

    if ((self->unk_283 != 0) && ((globalCtx->gameplayFrames & 0xF) == 0)) {
        self->unk_288 = Rand_CenteredFloat(50000.0f);
    }
    Math_SmoothStepToS(&self->actor.shape.rot.y, (self->actor.yawTowardsPlayer + self->unk_288), 1, 0x2EE, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    thisKeyFrame = self->skelAnime.curFrame;
    SkelAnime_Update(&self->skelAnime);
    if (self->skelAnime.playSpeed >= 0.0f) {
        playSpeed = self->skelAnime.playSpeed;
    } else {
        playSpeed = -self->skelAnime.playSpeed;
    }
    prevKeyFrame = (self->skelAnime.curFrame - playSpeed);
    if (self->skelAnime.playSpeed >= 0.0f) {
        playSpeed = self->skelAnime.playSpeed;
    } else {
        playSpeed = -self->skelAnime.playSpeed;
    }
    if (thisKeyFrame != (s32)self->skelAnime.curFrame) {
        if (((prevKeyFrame < 9) && (((s32)playSpeed + thisKeyFrame) >= 8)) ||
            !((prevKeyFrame >= 16) || (((s32)playSpeed + thisKeyFrame) < 15))) {

            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALKID_WALK);
        }
    }
    if (Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos) > 800.0f || IS_DAY) {
        func_80AFCF48(self);
    } else if (Actor_IsFacingPlayer(&self->actor, 0x11C7) &&
               (self->actor.xzDistToPlayer < (60.0f + (self->actor.params * 6.0f)))) {
        func_80AFD33C(self);
    }
}

void func_80AFD33C(EnSkb* self) {
    Animation_Change(&self->skelAnime, &gStalchildAttackingAnim, 0.6f, 0.0f,
                     Animation_GetLastFrame(&gStalchildAttackingAnim), ANIMMODE_ONCE_INTERP, 4.0f);
    self->collider.base.atFlags &= ~4;
    self->unk_280 = 3;
    self->actor.speedXZ = 0.0f;
    EnSkb_SetupAction(self, EnSkb_SetupAttack);
}

void EnSkb_SetupAttack(EnSkb* self, GlobalContext* globalCtx) {
    s32 frameData;

    frameData = self->skelAnime.curFrame;
    if (frameData == 3) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALKID_ATTACK);
        self->unk_281 = 1;
    } else if (frameData == 6) {
        self->unk_281 = 0;
    }
    if (self->collider.base.atFlags & 4) {
        self->collider.base.atFlags &= ~6;
        func_80AFD47C(self);
    } else if (SkelAnime_Update(&self->skelAnime) != 0) {
        func_80AFCD60(self);
    }
}

void func_80AFD47C(EnSkb* self) {
    Animation_Change(&self->skelAnime, &gStalchildAttackingAnim, -0.4f, self->skelAnime.curFrame - 1.0f, 0.0f,
                     ANIMMODE_ONCE_INTERP, 0.0f);
    self->collider.base.atFlags &= ~4;
    self->unk_280 = 5;
    self->unk_281 = 0;
    EnSkb_SetupAction(self, func_80AFD508);
}

void func_80AFD508(EnSkb* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        func_80AFCD60(self);
    }
}

void EnSkb_SetupStunned(EnSkb* self) {
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = 0.0f;
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->unk_281 = 0;
    self->unk_280 = 6;
    EnSkb_SetupAction(self, func_80AFD59C);
}

void func_80AFD59C(EnSkb* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 2) {
        self->actor.speedXZ = 0.0f;
    }
    if (self->actor.bgCheckFlags & 1) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 0.05f;
        }
    }
    if ((self->actor.colorFilterTimer == 0) && (self->actor.bgCheckFlags & 1)) {
        if (self->actor.colChkInfo.health == 0) {
            func_80AFD7B4(self, globalCtx);
        } else {
            func_80AFCD60(self);
        }
    }
}

void func_80AFD644(EnSkb* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gStalchildDamagedAnim, -4.0f);
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = -4.0f;
    }
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALKID_DAMAGE);
    self->unk_280 = 2;
    EnSkb_SetupAction(self, func_80AFD6CC);
}

void func_80AFD6CC(EnSkb* self, GlobalContext* globalCtx) {
    // self cast is likely not real, but allows for a match
    u8* new_var;
    new_var = &self->unk_283;
    if ((self->unk_283 != 1) || BodyBreak_SpawnParts(&self->actor, &self->bodyBreak, globalCtx, 1)) {
        if ((*new_var) != 0) {
            self->unk_283 = (*new_var) | 2;
        }
        if (self->actor.bgCheckFlags & 2) {
            self->actor.speedXZ = 0;
        }
        if (self->actor.bgCheckFlags & 1) {
            if (self->actor.speedXZ < 0.0f) {
                self->actor.speedXZ += 0.05f;
            }
        }

        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 1, 0x1194, 0);
        if (SkelAnime_Update(&self->skelAnime) && (self->actor.bgCheckFlags & 1)) {
            func_80AFCD60(self);
        }
    }
}

void func_80AFD7B4(EnSkb* self, GlobalContext* globalCtx) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gStalchildDyingAnim, -4.0f);
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    if (self->actor.bgCheckFlags & 1) {
        self->actor.speedXZ = -6.0f;
    }
    self->unk_280 = 1;
    self->actor.flags &= ~1;
    BodyBreak_Alloc(&self->bodyBreak, 18, globalCtx);
    self->unk_283 |= 4;
    EffectSsDeadSound_SpawnStationary(globalCtx, &self->actor.projectedPos, NA_SE_EN_STALKID_DEAD, 1, 1, 0x28);
    EnSkb_SetupAction(self, func_80AFD880);
}

void func_80AFD880(EnSkb* self, GlobalContext* globalCtx) {
    if (BodyBreak_SpawnParts(&self->actor, &self->bodyBreak, globalCtx, 1)) {
        if (self->actor.scale.x == 0.01f) {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0x10);
        } else if (self->actor.scale.x <= 0.015f) {
            Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_RUPEE_BLUE);
        } else {
            Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_RUPEE_RED);
            Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_RUPEE_RED);
            Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_RUPEE_RED);
        }

        self->unk_283 |= 8;
        Actor_Kill(&self->actor);
    }
}

void func_80AFD968(EnSkb* self, GlobalContext* globalCtx) {
    s16 pad;
    s32 i;
    Vec3f flamePos;
    s16 scale;
    s16 phi_v1;
    Player* player;

    if ((self->unk_280 != 1) && (self->actor.bgCheckFlags & 0x60) && (self->actor.yDistToWater >= 40.0f)) {
        self->actor.colChkInfo.health = 0;
        self->unk_281 = 0;
        func_80AFD7B4(self, globalCtx);
    } else if (self->unk_280 >= 3) {
        if ((self->collider.base.acFlags & 2) != 0) {
            self->collider.base.acFlags &= ~2;
            if (self->actor.colChkInfo.damageEffect != 6) {
                self->unk_282 = self->actor.colChkInfo.damageEffect;
                Actor_SetDropFlag(&self->actor, &self->collider.elements[1].info, 1);
                self->unk_281 = 0;
                if (self->actor.colChkInfo.damageEffect == 1) {
                    if (self->unk_280 != 6) {
                        Actor_SetColorFilter(&self->actor, 0, 0x78, 0, 0x50);
                        Actor_ApplyDamage(&self->actor);
                        EnSkb_SetupStunned(self);
                    }
                } else {
                    phi_v1 = 8;
                    if (self->actor.colChkInfo.damageEffect == 7) {
                        scale = self->actor.scale.y * 7500.0f;
                        for (i = 4; i >= 0; i--) {
                            flamePos = self->actor.world.pos;
                            flamePos.x += Rand_CenteredFloat(20.0f);
                            flamePos.z += Rand_CenteredFloat(20.0f);
                            flamePos.y += (Rand_ZeroOne() * 25.0f);
                            EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &flamePos, scale, 0, 0, -1);
                        }
                        phi_v1 = 25;
                    }
                    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, phi_v1);
                    if (!Actor_ApplyDamage(&self->actor)) {
                        func_80AFD7B4(self, globalCtx);
                        return;
                    }
                    player = GET_PLAYER(globalCtx);
                    if (self->unk_283 == 0) {
                        if ((self->actor.colChkInfo.damageEffect == 0xD) ||
                            ((self->actor.colChkInfo.damageEffect == 0xE) &&
                             ((player->swordAnimation >= 4 && player->swordAnimation <= 11) ||
                              (player->swordAnimation == 20 || player->swordAnimation == 21)))) {
                            BodyBreak_Alloc(&self->bodyBreak, 2, globalCtx);
                            self->unk_283 = 1;
                        }
                    }
                    func_80AFD644(self);
                }
            }
        }
    }
}

void EnSkb_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnSkb* self = THIS;
    s32 pad;

    func_80AFD968(self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 15.0f, 30.0f, 60.0f, 0x1D);
    self->actionFunc(self, globalCtx);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += (3000.0f * self->actor.scale.y);
    if (self->unk_281 != 0) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if (self->unk_280 >= 3) {
        if ((self->actor.colorFilterTimer == 0) || ((self->actor.colorFilterParams & 0x4000) == 0)) {

            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

s32 EnSkb_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnSkb* self = THIS;
    s16 color;
    s16 pad[2];

    if (limbIndex == 11) {
        if ((self->unk_283 & 2) == 0) {
            OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_skb.c", 972);
            color = ABS((s16)(Math_SinS((globalCtx->gameplayFrames * 0x1770)) * 95.0f)) + 160;
            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, color, color, color, 255);
            CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_skb.c", 978);
        } else {
            *dList = NULL;
        }
    } else if ((limbIndex == 12) && ((self->unk_283 & 2) != 0)) {
        *dList = NULL;
    }
    return 0;
}

void EnSkb_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnSkb* self = THIS;

    Collider_UpdateSpheres(limbIndex, &self->collider);

    if ((self->unk_283 ^ 1) == 0) {
        BodyBreak_SetInfo(&self->bodyBreak, limbIndex, 11, 12, 18, dList, BODYBREAK_OBJECT_DEFAULT);
    } else if ((self->unk_283 ^ (self->unk_283 | 4)) == 0) {
        BodyBreak_SetInfo(&self->bodyBreak, limbIndex, 0, 18, 18, dList, BODYBREAK_OBJECT_DEFAULT);
    }
}

void EnSkb_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnSkb* self = THIS;
    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnSkb_OverrideLimbDraw,
                      EnSkb_PostLimbDraw, &self->actor);
}
