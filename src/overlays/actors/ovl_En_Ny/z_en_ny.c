#include "z_en_ny.h"
#include "objects/object_ny/object_ny.h"

#define FLAGS 0x00000005

#define THIS ((EnNy*)thisx)

void EnNy_Init(Actor* thisx, GlobalContext* globalCtx);
void EnNy_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnNy_Update(Actor* thisx, GlobalContext* globalCtx);
void EnNy_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnNy_UpdateUnused(Actor* thisx, GlobalContext* globalCtx);
void EnNy_Move(EnNy* self, GlobalContext* globalCtx);
void EnNy_Die(EnNy* self, GlobalContext* globalCtx);
void func_80ABCD40(EnNy* self);
void func_80ABCDBC(EnNy* self);
void EnNy_TurnToStone(EnNy* self, GlobalContext* globalCtx);
void func_80ABD11C(EnNy* self, GlobalContext* globalCtx);
void func_80ABCE50(EnNy* self, GlobalContext* globalCtx);
void func_80ABCE90(EnNy* self, GlobalContext* globalCtx);
void func_80ABCEEC(EnNy* self, GlobalContext* globalCtx);
void EnNy_UpdateDeath(Actor* thisx, GlobalContext* GlobalContext);
void EnNy_SetupDie(EnNy* self, GlobalContext* globalCtx);
void EnNy_DrawDeathEffect(Actor* thisx, GlobalContext* GlobalContext);
void func_80ABD3B8(EnNy* self, f32, f32);

const ActorInit En_Ny_InitVars = {
    ACTOR_EN_NY,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_NY,
    sizeof(EnNy),
    (ActorFunc)EnNy_Init,
    (ActorFunc)EnNy_Destroy,
    (ActorFunc)EnNy_Update,
    (ActorFunc)EnNy_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x04, 0x08 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sColliderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0xF),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(2, 0xF),
    /* Hammer swing  */ DMG_ENTRY(2, 0xF),
    /* Hookshot      */ DMG_ENTRY(2, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(0, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
    /* Fire arrow    */ DMG_ENTRY(4, 0x2),
    /* Ice arrow     */ DMG_ENTRY(2, 0xF),
    /* Light arrow   */ DMG_ENTRY(2, 0xF),
    /* Unk arrow 1   */ DMG_ENTRY(4, 0xE),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(4, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(0, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x28, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

void EnNy_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.colChkInfo.damageTable = &sDamageTable;
    self->actor.colChkInfo.health = 2;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sColliderInit, self->elements);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    self->unk_1CA = 0;
    self->unk_1D0 = 0;
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.speedXZ = 0.0f;
    self->actor.shape.rot.y = 0;
    self->actor.gravity = -0.4f;
    self->hitPlayer = 0;
    self->unk_1CE = 2;
    self->actor.velocity.y = 0.0f;
    self->unk_1D4 = 0xFF;
    self->unk_1D8 = 0;
    self->unk_1E8 = 0.0f;
    self->unk_1E0 = 0.25f;
    if (self->actor.params == 0) {
        // "New initials"
        osSyncPrintf("ニュウ イニシャル[ %d ] ！！\n", self->actor.params);
        self->actor.colChkInfo.mass = 0;
        self->unk_1D4 = 0;
        self->unk_1D8 = 0xFF;
        self->unk_1E0 = 1.0f;
        func_80ABCDBC(self);
    } else {
        // This mode is unused in the final game
        // "Dummy new initials"
        osSyncPrintf("ダミーニュウ イニシャル[ %d ] ！！\n", self->actor.params);
        osSyncPrintf("En_Ny_actor_move2[ %x ] ！！\n", EnNy_UpdateUnused);
        self->actor.colChkInfo.mass = 0xFF;
        self->collider.base.colType = COLTYPE_METAL;
        self->actor.update = EnNy_UpdateUnused;
    }
}

void EnNy_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* self = THIS;
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void func_80ABCD40(EnNy* self) {
    f32 temp;

    temp = (self->actor.yDistToWater > 0.0f) ? 0.7f : 1.0f;
    self->unk_1E8 = 2.8f * temp;
}

void func_80ABCD84(EnNy* self) {
    self->actionFunc = func_80ABCE50;
}

void func_80ABCD94(EnNy* self) {
    self->stoneTimer = 0x14;
    self->actionFunc = func_80ABCE90;
}

void func_80ABCDAC(EnNy* self) {
    self->actionFunc = func_80ABCEEC;
}

void func_80ABCDBC(EnNy* self) {
    self->unk_1F4 = 0.0f;
    func_80ABCD40(self);
    self->stoneTimer = 180;
    self->actionFunc = EnNy_Move;
}

void EnNy_SetupTurnToStone(EnNy* self) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NYU_HIT_STOP);
    self->actionFunc = EnNy_TurnToStone;
    self->unk_1E8 = 0.0f;
}

void func_80ABCE38(EnNy* self) {
    self->stoneTimer = 0x3C;
    self->actionFunc = func_80ABD11C;
}

void func_80ABCE50(EnNy* self, GlobalContext* globalCtx) {
    if (self->actor.xyzDistToPlayerSq <= 25600.0f) {
        func_80ABCD94(self);
    }
}

void func_80ABCE90(EnNy* self, GlobalContext* globalCtx) {
    s32 phi_v1;
    s32 phi_v0;

    phi_v1 = self->unk_1D4 - 0x40;
    phi_v0 = self->unk_1D8 + 0x40;
    if (phi_v0 >= 0xFF) {
        phi_v1 = 0;
        phi_v0 = 0xFF;
        func_80ABCDAC(self);
    }
    self->unk_1D4 = phi_v1;
    self->unk_1D8 = phi_v0;
}

void func_80ABCEEC(EnNy* self, GlobalContext* globalCtx) {
    f32 phi_f0;

    phi_f0 = self->unk_1E0;
    phi_f0 += 2.0f;
    if (phi_f0 >= 1.0f) {
        phi_f0 = 1.0f;
        func_80ABCDBC(self);
    }
    self->unk_1E0 = phi_f0;
}

void EnNy_Move(EnNy* self, GlobalContext* globalCtx) {
    f32 yawDiff;
    s32 stoneTimer;

    if (!(self->unk_1F0 < self->actor.yDistToWater)) {
        func_8002F974(&self->actor, NA_SE_EN_NYU_MOVE - SFX_FLAG);
    }
    func_80ABCD40(self);
    stoneTimer = self->stoneTimer;
    self->stoneTimer--;
    if ((stoneTimer <= 0) || (self->hitPlayer != false)) {
        EnNy_SetupTurnToStone(self);
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, self->unk_1F4, 0);
        Math_ApproachF(&self->unk_1F4, 2000.0f, 1.0f, 100.0f);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        yawDiff = Math_FAtan2F(self->actor.yDistToPlayer, self->actor.xzDistToPlayer);
        self->actor.speedXZ = fabsf(cosf(yawDiff) * self->unk_1E8);
        if (self->unk_1F0 < self->actor.yDistToWater) {
            self->unk_1EC = sinf(yawDiff) * self->unk_1E8;
        }
    }
}

void EnNy_TurnToStone(EnNy* self, GlobalContext* globalCtx) {
    f32 phi_f0;

    phi_f0 = self->unk_1E0;
    phi_f0 -= 2.0f;
    if (phi_f0 <= 0.25f) {
        phi_f0 = 0.25f;
        if (self->actor.bgCheckFlags & 2) {
            if (!(self->unk_1F0 < self->actor.yDistToWater)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
            }
            self->actor.bgCheckFlags &= ~2;
            self->actor.speedXZ = 0.0f;
            self->actor.world.rot.y = self->actor.shape.rot.y;
            func_80ABCE38(self);
        }
    }
    self->unk_1E0 = phi_f0;
}

void func_80ABD11C(EnNy* self, GlobalContext* globalCtx) {
    s32 phi_v0;
    s32 phi_v1;

    phi_v0 = self->unk_1D4;
    phi_v0 += 0x40;
    phi_v1 = self->unk_1D8;
    phi_v1 -= 0x40;
    if (phi_v0 >= 0xFF) {
        phi_v0 = 0xFF;
        phi_v1 = 0;
        if (self->stoneTimer != 0) {
            self->stoneTimer--;
        } else {
            func_80ABCD84(self);
        }
    }
    self->unk_1D4 = phi_v0;
    self->unk_1D8 = phi_v1;
}

s32 EnNy_CollisionCheck(EnNy* self, GlobalContext* globalCtx) {
    u8 sp3F;
    Vec3f effectPos;

    sp3F = 0;
    self->hitPlayer = 0;
    if (self->collider.base.atFlags & 4) {
        self->collider.base.atFlags &= ~4;
        self->hitPlayer = 1;
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.speedXZ = -4.0f;
        return 0;
    }
    if (self->collider.base.atFlags & 2) {
        self->collider.base.atFlags &= ~2;
        self->hitPlayer = 1;
        return 0;
    } else {
        if (self->collider.base.acFlags & 2) {
            self->collider.base.acFlags &= ~2;
            effectPos.x = self->collider.elements[0].info.bumper.hitPos.x;
            effectPos.y = self->collider.elements[0].info.bumper.hitPos.y;
            effectPos.z = self->collider.elements[0].info.bumper.hitPos.z;
            if ((self->unk_1E0 == 0.25f) && (self->unk_1D4 == 0xFF)) {
                switch (self->actor.colChkInfo.damageEffect) {
                    case 0xE:
                        sp3F = 1;
                    case 0xF:
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 0x50);
                        break;
                    case 1:
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 0x50);
                        break;
                    case 2:
                        self->unk_1CA = 4;
                        Actor_ApplyDamage(&self->actor);
                        Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0x2000, 0x50);
                        break;
                }
            }
            self->stoneTimer = 0;
            if (self->actor.colChkInfo.health == 0) {
                self->actor.shape.shadowAlpha = 0;
                self->actor.flags &= ~1;
                self->unk_1D0 = sp3F;
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                return 1;
            }
            EffectSsHitMark_SpawnFixedScale(globalCtx, 0, &effectPos);
            return 0;
        }
    }
    return 0;
}

void func_80ABD3B8(EnNy* self, f32 arg1, f32 arg2) {
    if (self->unk_1E8 == 0.0f) {
        self->actor.gravity = -0.4f;
    } else if (!(arg1 < self->actor.yDistToWater)) {
        self->actor.gravity = -0.4f;
    } else if (arg2 < self->actor.yDistToWater) {
        self->actor.gravity = 0.0;
        if (self->unk_1EC < self->actor.velocity.y) {
            self->actor.velocity.y -= 0.4f;
            if (self->actor.velocity.y < self->unk_1EC) {
                self->actor.velocity.y = self->unk_1EC;
            }
        } else if (self->actor.velocity.y < self->unk_1EC) {
            self->actor.velocity.y += 0.4f;
            if (self->unk_1EC < self->actor.velocity.y) {
                self->actor.velocity.y = self->unk_1EC;
            }
        }
    }
}

void EnNy_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* self = THIS;
    f32 temp_f20;
    f32 temp_f22;
    s32 i;

    self->timer++;
    temp_f20 = self->unk_1E0 - 0.25f;
    if (self->unk_1CA != 0) {
        self->unk_1CA--;
    }
    Actor_SetFocus(&self->actor, 0.0f);
    Actor_SetScale(&self->actor, 0.01f);
    self->collider.elements[0].dim.scale = 1.33f * temp_f20 + 1.0f;
    temp_f22 = (24.0f * temp_f20) + 12.0f;
    self->actor.shape.rot.x += (s16)(self->unk_1E8 * 1000.0f);
    func_80ABD3B8(self, temp_f22 + 10.0f, temp_f22 - 10.0f);
    Actor_MoveForward(&self->actor);
    Math_StepToF(&self->unk_1E4, self->unk_1E8, 0.1f);
    self->actionFunc(self, globalCtx);
    self->actor.prevPos.y -= temp_f22;
    self->actor.world.pos.y -= temp_f22;
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 60.0f, 7);
    self->unk_1F0 = temp_f22;
    self->actor.world.pos.y += temp_f22;
    if (EnNy_CollisionCheck(self, globalCtx) != 0) {
        for (i = 0; i < 8; i++) {
            self->unk_1F8[i].x = (Rand_CenteredFloat(20.0f) + self->actor.world.pos.x);
            self->unk_1F8[i].y = (Rand_CenteredFloat(20.0f) + self->actor.world.pos.y);
            self->unk_1F8[i].z = (Rand_CenteredFloat(20.0f) + self->actor.world.pos.z);
        }
        self->timer = 0;
        self->actor.update = EnNy_UpdateDeath;
        self->actor.draw = EnNy_DrawDeathEffect;
        self->actionFunc = EnNy_SetupDie;
        return;
    }
    if (self->unk_1E0 > 0.25f) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnNy_SetupDie(EnNy* self, GlobalContext* globalCtx) {
    s32 effectScale;
    s32 i;
    Vec3f effectPos;
    Vec3f effectVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effectAccel = { 0.0f, 0.1f, 0.0f };

    if (self->timer >= 2) {
        if (self->actor.yDistToWater > 0.0f) {
            for (i = 0; i < 10; i++) {
                effectPos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
                effectPos.y = Rand_CenteredFloat(30.0f) + self->actor.world.pos.y;
                effectPos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;
                effectScale = Rand_S16Offset(0x50, 0x64);
                EffectSsDtBubble_SpawnColorProfile(globalCtx, &effectPos, &effectVelocity, &effectAccel, effectScale,
                                                   25, 0, 1);
            }
            for (i = 0; i < 0x14; i++) {
                effectPos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
                effectPos.y = Rand_CenteredFloat(30.0f) + self->actor.world.pos.y;
                effectPos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;
                EffectSsBubble_Spawn(globalCtx, &effectPos, 10.0f, 10.0f, 30.0f, 0.25f);
            }
        }
        for (i = 0; i < 8; i++) {
            self->unk_1F8[i + 8].x = Rand_CenteredFloat(10.0f);
            self->unk_1F8[i + 8].z = Rand_CenteredFloat(10.0f);
            self->unk_1F8[i + 8].y = Rand_ZeroFloat(4.0f) + 4.0f;
        }
        self->timer = 0;
        if (self->unk_1D0 == 0) {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xA0);
        } else {
            Item_DropCollectible(globalCtx, &self->actor.world.pos, 8);
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NYU_DEAD);
        self->actionFunc = EnNy_Die;
    }
}

void EnNy_Die(EnNy* self, GlobalContext* globalCtx) {
    s32 i;

    if (self->actor.yDistToWater > 0.0f) {
        for (i = 0; i < 8; i += 1) {
            self->unk_1F8[i].x += self->unk_1F8[i + 8].x;
            self->unk_1F8[i].y += self->unk_1F8[i + 8].y;
            self->unk_1F8[i].z += self->unk_1F8[i + 8].z;
            Math_StepToF(&self->unk_1F8[i + 8].x, 0.0f, 0.1f);
            Math_StepToF(&self->unk_1F8[i + 8].y, -1.0f, 0.4f);
            Math_StepToF(&self->unk_1F8[i + 8].z, 0.0f, 0.1f);
        }
        if (self->timer >= 0x1F) {
            Actor_Kill(&self->actor);
            return;
        }
    } else {
        for (i = 0; i < 8; i += 1) {
            self->unk_1F8[i].x += self->unk_1F8[i + 8].x;
            self->unk_1F8[i].y += self->unk_1F8[i + 8].y;
            self->unk_1F8[i].z += self->unk_1F8[i + 8].z;
            Math_StepToF(&self->unk_1F8[i + 8].x, 0.0f, 0.15f);
            Math_StepToF(&self->unk_1F8[i + 8].y, -1.0f, 0.6f);
            Math_StepToF(&self->unk_1F8[i + 8].z, 0.0f, 0.15f);
        }
        if (self->timer >= 0x10) {
            Actor_Kill(&self->actor);
            return;
        }
    }
}

void EnNy_UpdateDeath(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* self = THIS;

    self->timer++;
    if (self->unk_1CA != 0) {
        self->unk_1CA--;
    }
    self->actionFunc(self, globalCtx);
}

void EnNy_UpdateUnused(Actor* thisx, GlobalContext* globalCtx2) {
    EnNy* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    f32 sp3C;
    f32 temp_f0;

    sp3C = self->unk_1E0 - 0.25f;
    self->timer++;
    Actor_SetFocus(&self->actor, 0.0f);
    Actor_SetScale(&self->actor, 0.01f);
    temp_f0 = (24.0f * sp3C) + 12.0f;
    self->actor.prevPos.y -= temp_f0;
    self->actor.world.pos.y -= temp_f0;

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 60.0f, 7);
    self->unk_1F0 = temp_f0;
    self->actor.world.pos.y += temp_f0;

    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Math_StepToF(&self->unk_1E4, self->unk_1E8, 0.1f);
}
static Vec3f sFireOffsets[] = {
    { 5.0f, 0.0f, 0.0f },
    { -5.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 5.0f },
    { 0.0f, 0.0f, -5.0f },
};

void EnNy_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnNy* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ny.c", 837);
    Collider_UpdateSpheres(0, &self->collider);
    func_8002ED80(&self->actor, globalCtx, 1);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ny.c", 845),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_AA_ZB_XLU_SURF2);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->unk_1D8);
    gSPDisplayList(POLY_XLU_DISP++, gEnNyMetalBodyDL);
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->unk_1D4);
    gSPDisplayList(POLY_XLU_DISP++, gEnNyRockBodyDL);
    if (self->unk_1E0 > 0.25f) {
        Matrix_Scale(self->unk_1E0, self->unk_1E0, self->unk_1E0, MTXMODE_APPLY);
        func_8002EBCC(&self->actor, globalCtx, 1);
        func_80093D18(globalCtx->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ny.c", 868),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gEnNySpikeDL);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ny.c", 872);
    if (self->unk_1CA != 0) {
        Vec3f tempVec;
        Vec3f* fireOffset;
        s16 temp;
        temp = self->unk_1CA - 1;
        self->actor.colorFilterTimer++;
        if (temp == 0) {
            fireOffset = &sFireOffsets[temp & 3];
            tempVec.x = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.x + fireOffset->x);
            tempVec.y = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.y + fireOffset->y);
            tempVec.z = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.z + fireOffset->z);
            EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &tempVec, 100, 0, 0, -1);
        }
    }
}

void EnNy_DrawDeathEffect(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* self = THIS;
    Vec3f* temp;
    f32 scale;
    s32 i;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ny.c", 900);
    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, 0x00, 0x00, 0x00, 0xFF);
    gDPSetRenderMode(POLY_OPA_DISP++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(POLY_OPA_DISP++);
    for (i = 0; i < 8; i++) {
        if (self->timer < (i + 22)) {
            temp = &self->unk_1F8[i];
            Matrix_Translate(temp->x, temp->y, temp->z, MTXMODE_NEW);
            scale = self->actor.scale.x * 0.4f * (1.0f + (i * 0.04f));
            Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ny.c", 912),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, gEnNyRockBodyDL);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ny.c", 919);
    if (self->unk_1CA != 0) {
        Vec3f tempVec;
        Vec3f* fireOffset;
        s16 fireOffsetIndex;

        fireOffsetIndex = self->unk_1CA - 1;
        self->actor.colorFilterTimer++;
        if ((fireOffsetIndex & 1) == 0) {
            fireOffset = &sFireOffsets[fireOffsetIndex & 3];
            tempVec.x = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.x + fireOffset->x);
            tempVec.y = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.y + fireOffset->y);
            tempVec.z = Rand_CenteredFloat(5.0f) + (self->actor.world.pos.z + fireOffset->z);
            EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &tempVec, 100, 0, 0, -1);
        }
    }
}
