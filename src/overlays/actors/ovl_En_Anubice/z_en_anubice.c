/*
 * File: z_en_anubice.c
 * Overlay: ovl_En_Anubice
 * Description: Anubis Body
 */

#include "z_en_anubice.h"
#include "objects/object_anubice/object_anubice.h"
#include "overlays/actors/ovl_En_Anubice_Tag/z_en_anubice_tag.h"
#include "overlays/actors/ovl_Bg_Hidan_Curtain/z_bg_hidan_curtain.h"
#include "vt.h"

#define FLAGS 0x00000015

#define THIS ((EnAnubice*)thisx)

void EnAnubice_Init(Actor* thisx, GlobalContext* globalCtx);
void EnAnubice_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnAnubice_Update(Actor* thisx, GlobalContext* globalCtx);
void EnAnubice_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnAnubice_FindFlameCircles(EnAnubice* self, GlobalContext* globalCtx);
void EnAnubice_SetupIdle(EnAnubice* self, GlobalContext* globalCtx);
void EnAnubice_Idle(EnAnubice* self, GlobalContext* globalCtx);
void EnAnubice_GoToHome(EnAnubice* self, GlobalContext* globalCtx);
void EnAnubis_SetupShootFireball(EnAnubice* self, GlobalContext* globalCtx);
void EnAnubis_ShootFireball(EnAnubice* self, GlobalContext* globalCtx);
void EnAnubice_Die(EnAnubice* self, GlobalContext* globalCtx);

const ActorInit En_Anubice_InitVars = {
    ACTOR_EN_ANUBICE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ANUBICE,
    sizeof(EnAnubice),
    (ActorFunc)EnAnubice_Init,
    (ActorFunc)EnAnubice_Destroy,
    (ActorFunc)EnAnubice_Update,
    (ActorFunc)EnAnubice_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 29, 103, 0, { 0, 0, 0 } },
};

static DamageTable sDamageTable[] = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0xF),
    /* Slingshot     */ DMG_ENTRY(0, 0xF),
    /* Explosive     */ DMG_ENTRY(0, 0xF),
    /* Boomerang     */ DMG_ENTRY(0, 0xF),
    /* Normal arrow  */ DMG_ENTRY(0, 0xF),
    /* Hammer swing  */ DMG_ENTRY(1, 0xF),
    /* Hookshot      */ DMG_ENTRY(2, 0xF),
    /* Kokiri sword  */ DMG_ENTRY(0, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(6, 0xF),
    /* Fire arrow    */ DMG_ENTRY(2, 0x2),
    /* Ice arrow     */ DMG_ENTRY(0, 0xF),
    /* Light arrow   */ DMG_ENTRY(0, 0xF),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0xF),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0xF),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0xF),
    /* Fire magic    */ DMG_ENTRY(3, 0x2),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0xF),
    /* Giant spin    */ DMG_ENTRY(6, 0xF),
    /* Master spin   */ DMG_ENTRY(2, 0xF),
    /* Kokiri jump   */ DMG_ENTRY(0, 0xF),
    /* Giant jump    */ DMG_ENTRY(12, 0xF),
    /* Master jump   */ DMG_ENTRY(4, 0xF),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

void EnAnubice_Hover(EnAnubice* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->hoverVelocityTimer += 1500.0f;
    self->targetHeight = player->actor.world.pos.y + self->playerHeightOffset;
    Math_ApproachF(&self->actor.world.pos.y, self->targetHeight, 0.1f, 10.0f);
    Math_ApproachF(&self->playerHeightOffset, 10.0f, 0.1f, 0.5f);
    self->actor.velocity.y = Math_SinS(self->hoverVelocityTimer);
}

void EnAnubice_SetFireballRot(EnAnubice* self, GlobalContext* globalCtx) {
    f32 xzdist;
    f32 x;
    f32 y;
    f32 z;
    Player* player = GET_PLAYER(globalCtx);

    x = player->actor.world.pos.x - self->fireballPos.x;
    y = player->actor.world.pos.y + 10.0f - self->fireballPos.y;
    z = player->actor.world.pos.z - self->fireballPos.z;
    xzdist = sqrtf(SQ(x) + SQ(z));

    self->fireballRot.x = -RADF_TO_BINANG(Math_FAtan2F(y, xzdist));
    self->fireballRot.y = RADF_TO_BINANG(Math_FAtan2F(x, z));
}

void EnAnubice_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubice* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gAnubiceSkel, &gAnubiceIdleAnim, self->jointTable, self->morphTable,
                   16);

    osSyncPrintf("\n\n");
    // "☆☆☆☆☆ Anubis occurence ☆☆☆☆☆"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ アヌビス発生 ☆☆☆☆☆ \n" VT_RST);

    self->actor.naviEnemyId = 0x3A;

    Collider_InitCylinder(globalCtx, &self->col);
    Collider_SetCylinder(globalCtx, &self->col, &self->actor, &sCylinderInit);

    Actor_SetScale(&self->actor, 0.015f);

    self->actor.colChkInfo.damageTable = sDamageTable;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.shape.yOffset = -4230.0f;
    self->focusHeightOffset = 0.0f;
    self->actor.flags &= ~1;
    self->home = self->actor.world.pos;
    self->actor.targetMode = 3;
    self->actionFunc = EnAnubice_FindFlameCircles;
}

void EnAnubice_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubice* self = THIS;
    EnAnubiceTag* temp_v1;

    Collider_DestroyCylinder(globalCtx, &self->col);

    if (self->actor.params != 0) {
        if (self->actor.parent) {}

        temp_v1 = (EnAnubiceTag*)self->actor.parent;
        if (temp_v1 != NULL && temp_v1->actor.update != NULL) {
            temp_v1->anubis = NULL;
        }
    }
}

void EnAnubice_FindFlameCircles(EnAnubice* self, GlobalContext* globalCtx) {
    Actor* currentProp;
    s32 flameCirclesFound;

    if (self->isMirroringLink) {
        if (!self->hasSearchedForFlameCircles) {
            flameCirclesFound = 0;
            currentProp = globalCtx->actorCtx.actorLists[ACTORCAT_PROP].head;
            while (currentProp != NULL) {
                if (currentProp->id != ACTOR_BG_HIDAN_CURTAIN) {
                    currentProp = currentProp->next;
                } else {
                    self->flameCircles[flameCirclesFound] = (BgHidanCurtain*)currentProp;
                    // "☆☆☆☆☆ How many fires? ☆☆☆☆☆"
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 火は幾つ？ ☆☆☆☆☆ %d\n" VT_RST, flameCirclesFound);
                    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 火は幾つ？ ☆☆☆☆☆ %x\n" VT_RST,
                                 self->flameCircles[flameCirclesFound]);
                    if (flameCirclesFound < 4) {
                        flameCirclesFound++;
                    }
                    currentProp = currentProp->next;
                }
            }
            self->hasSearchedForFlameCircles = true;
        }
        self->actor.flags |= 1;
        self->actionFunc = EnAnubice_SetupIdle;
    }
}

void EnAnubice_SetupIdle(EnAnubice* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(&gAnubiceIdleAnim);

    Animation_Change(&self->skelAnime, &gAnubiceIdleAnim, 1.0f, 0.0f, (s16)lastFrame, ANIMMODE_LOOP, -10.0f);

    self->actionFunc = EnAnubice_Idle;
    self->actor.velocity.x = self->actor.velocity.z = self->actor.gravity = 0.0f;
}

void EnAnubice_Idle(EnAnubice* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    Math_ApproachZeroF(&self->actor.shape.yOffset, 0.5f, 300.0f);
    Math_ApproachF(&self->focusHeightOffset, 70.0f, 0.5f, 5.0f);

    if (!self->isKnockedback) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, 3000, 0);
    }

    if (self->actor.shape.yOffset > -2.0f) {
        self->actor.shape.yOffset = 0.0f;

        if (player->swordState != 0) {
            self->actionFunc = EnAnubis_SetupShootFireball;
        } else if (self->isLinkOutOfRange) {
            self->actor.velocity.y = 0.0f;
            self->actor.gravity = -1.0f;
            self->actionFunc = EnAnubice_GoToHome;
        }
    }
}

void EnAnubice_GoToHome(EnAnubice* self, GlobalContext* globalCtx) {
    f32 xzdist;
    f32 xRatio;
    f32 zRatio;
    f32 x;
    f32 z;

    SkelAnime_Update(&self->skelAnime);
    Math_ApproachF(&self->actor.shape.yOffset, -4230.0f, 0.5f, 300.0f);
    Math_ApproachZeroF(&self->focusHeightOffset, 0.5f, 5.0f);

    if (!self->isKnockedback) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, 3000, 0);
    }

    if (fabsf(self->home.x - self->actor.world.pos.x) > 3.0f && fabsf(self->home.z - self->actor.world.pos.z) > 3.0f) {
        x = self->home.x - self->actor.world.pos.x;
        z = self->home.z - self->actor.world.pos.z;
        xzdist = sqrtf(SQ(x) + SQ(z));
        xRatio = ((x) / xzdist);
        zRatio = ((z) / xzdist);
        self->actor.world.pos.x += (xRatio * 8);
        self->actor.world.pos.z += (zRatio * 8.0f);
    } else if (self->actor.shape.yOffset < -4220.0f) {
        self->actor.shape.yOffset = -4230.0f;
        self->isMirroringLink = self->isLinkOutOfRange = false;
        self->actionFunc = EnAnubice_FindFlameCircles;
        self->actor.gravity = 0.0f;
    }
}

void EnAnubis_SetupShootFireball(EnAnubice* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(&gAnubiceAttackingAnim);

    self->animLastFrame = lastFrame;
    Animation_Change(&self->skelAnime, &gAnubiceAttackingAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -10.0f);
    self->actionFunc = EnAnubis_ShootFireball;
    self->actor.velocity.x = self->actor.velocity.z = 0.0f;
}

void EnAnubis_ShootFireball(EnAnubice* self, GlobalContext* globalCtx) {
    f32 curFrame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);

    if (!self->isKnockedback) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, 3000, 0);
    }

    EnAnubice_SetFireballRot(self, globalCtx);

    if (curFrame == 12.0f) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ANUBICE_FIRE, self->fireballPos.x,
                    self->fireballPos.y + 15.0f, self->fireballPos.z, self->fireballRot.x, self->fireballRot.y, 0, 0);
    }

    if (self->animLastFrame <= curFrame) {
        self->actionFunc = EnAnubice_SetupIdle;
    }
}

void EnAnubice_SetupDie(EnAnubice* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(&gAnubiceFallDownAnim);

    self->animLastFrame = lastFrame;
    Animation_Change(&self->skelAnime, &gAnubiceFallDownAnim, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -20.0f);

    self->unk_256 = false;
    self->unk_258 = 0;
    self->deathTimer = 20;
    self->actor.velocity.x = self->actor.velocity.z = 0.0f;
    self->actor.gravity = -1.0f;

    if (BgCheck_SphVsFirstPoly(&globalCtx->colCtx, &self->fireballPos, 70.0f)) {
        self->unk_256 = true;
        self->unk_258 = self->actor.shape.rot.x - 0x7F00;
    }

    self->actionFunc = EnAnubice_Die;
}

void EnAnubice_Die(EnAnubice* self, GlobalContext* globalCtx) {
    f32 curFrame;
    f32 phi_f2;
    Vec3f sp4C = { 0.0f, 0.0f, 0.0f };
    Vec3f fireEffectPos = { 0.0f, 0.0f, 0.0f };
    s32 pad;

    SkelAnime_Update(&self->skelAnime);
    Math_ApproachZeroF(&self->actor.shape.shadowScale, 0.4f, 0.25f);

    if (self->unk_256) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->unk_258, 1, 10000, 0);
        if (fabsf(self->actor.shape.rot.y - self->unk_258) < 100.0f) {
            self->unk_256 = false;
        }
    }

    curFrame = self->skelAnime.curFrame;
    phi_f2 = curFrame * -3000.0f;
    phi_f2 = CLAMP_MIN(phi_f2, -11000.0f);

    Matrix_RotateY(BINANG_TO_RAD(self->actor.shape.rot.y), MTXMODE_NEW);
    Matrix_RotateX(BINANG_TO_RAD(phi_f2), MTXMODE_APPLY);
    sp4C.y = Rand_CenteredFloat(10.0f) + 30.0f;
    Matrix_MultVec3f(&sp4C, &fireEffectPos);
    fireEffectPos.x += self->actor.world.pos.x + Rand_CenteredFloat(40.0f);
    fireEffectPos.y += self->actor.world.pos.y + Rand_CenteredFloat(40.0f);
    fireEffectPos.z += self->actor.world.pos.z + Rand_CenteredFloat(30.0f);
    Actor_SetColorFilter(&self->actor, 0x4000, 0x80, 0, 8);
    EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &fireEffectPos, 100, 0, 0, -1);

    if (self->animLastFrame <= curFrame && (self->actor.bgCheckFlags & 1)) {
        Math_ApproachF(&self->actor.shape.yOffset, -4230.0f, 0.5f, 300.0f);
        if (self->actor.shape.yOffset < -2000.0f) {
            Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos, 0xC0);
            Actor_Kill(&self->actor);
        }
    }
}

void EnAnubice_Update(Actor* thisx, GlobalContext* globalCtx) {
    f32 zero;
    BgHidanCurtain* flameCircle;
    s32 i;
    Vec3f sp48;
    Vec3f sp3C;
    EnAnubice* self = THIS;

    if (self->actionFunc != EnAnubice_SetupDie && self->actionFunc != EnAnubice_Die &&
        self->actor.shape.yOffset == 0.0f) {
        EnAnubice_Hover(self, globalCtx);
        for (i = 0; i < 5; i++) {
            flameCircle = self->flameCircles[i];

            if (flameCircle != NULL && fabsf(flameCircle->actor.world.pos.x - self->actor.world.pos.x) < 60.0f &&
                fabsf(self->flameCircles[i]->actor.world.pos.z - self->actor.world.pos.z) < 60.0f &&
                flameCircle->timer != 0) {
                Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
                self->actor.flags &= ~1;
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_ANUBIS_DEAD);
                self->actionFunc = EnAnubice_SetupDie;
                return;
            }
        }

        if (self->col.base.acFlags & 2) {
            self->col.base.acFlags &= ~2;
            if (self->actor.colChkInfo.damageEffect == 2) {
                Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
                self->actor.flags &= ~1;
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_ANUBIS_DEAD);
                self->actionFunc = EnAnubice_SetupDie;
                return;
            }

            if (!self->isKnockedback) {
                self->knockbackTimer = 10;
                self->isKnockedback = true;

                sp48.x = 0.0f;
                sp48.y = 0.0f;
                sp48.z = -10.0f;
                sp3C.x = 0.0f;
                sp3C.y = 0.0f;
                sp3C.z = 0.0f;

                Matrix_RotateY(BINANG_TO_RAD(self->actor.shape.rot.y), MTXMODE_NEW);
                Matrix_MultVec3f(&sp48, &sp3C);

                self->actor.velocity.x = sp3C.x;
                self->actor.velocity.z = sp3C.z;
                self->knockbackRecoveryVelocity.x = -sp3C.x;
                self->knockbackRecoveryVelocity.z = -sp3C.z;

                Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_CUTBODY);
            }
        }

        if (self->isKnockedback) {
            self->actor.shape.rot.y += 6500;
            Math_ApproachF(&self->actor.velocity.x, self->knockbackRecoveryVelocity.x, 0.3f, 1.0f);
            Math_ApproachF(&self->actor.velocity.z, self->knockbackRecoveryVelocity.z, 0.3f, 1.0f);

            zero = 0.0f;
            if (zero) {}

            if (self->knockbackTimer == 0) {
                self->actor.velocity.x = self->actor.velocity.z = 0.0f;
                self->knockbackRecoveryVelocity.x = self->knockbackRecoveryVelocity.z = 0.0f;
                self->isKnockedback = false;
            }
        }
    }

    self->timeAlive++;

    if (self->knockbackTimer != 0) {
        self->knockbackTimer--;
    }

    if (self->deathTimer != 0) {
        self->deathTimer--;
    }

    self->actionFunc(self, globalCtx);

    self->actor.velocity.y += self->actor.gravity;
    func_8002D7EC(&self->actor);

    if (!self->isLinkOutOfRange) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 5.0f, 5.0f, 10.0f, 0x1D);
    } else {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 5.0f, 5.0f, 10.0f, 0x1C);
    }

    if (self->actionFunc != EnAnubice_SetupDie && self->actionFunc != EnAnubice_Die) {
        Actor_SetFocus(&self->actor, self->focusHeightOffset);
        Collider_UpdateCylinder(&self->actor, &self->col);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->col.base);

        if (!self->isKnockedback && self->actor.shape.yOffset == 0.0f) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->col.base);
        }
    }
}

s32 EnAnubis_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                              void* thisx) {
    EnAnubice* self = THIS;

    if (limbIndex == 13) {
        rot->z += self->unk_278;
    }

    return false;
}

void EnAnubis_PostLimbDraw(struct GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnAnubice* self = THIS;
    Vec3f pos = { 0.0f, 0.0f, 0.0f };

    if (limbIndex == 13) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_anubice.c", 853);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_anubice.c", 856),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gAnubiceEyesDL);
        Matrix_MultVec3f(&pos, &self->fireballPos);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_anubice.c", 868);
    }
}

void EnAnubice_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubice* self = THIS;

    func_80093D84(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnAnubis_OverrideLimbDraw,
                      EnAnubis_PostLimbDraw, self);
}
