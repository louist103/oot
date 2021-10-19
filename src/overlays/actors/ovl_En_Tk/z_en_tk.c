/*
 * File: z_en_tk.c
 * Overlay: ovl_En_Tk
 * Description: Dampe NPC from "Dampe's Heart-Pounding Gravedigging Tour"
 */

#include "z_en_tk.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_tk/object_tk.h"

#define FLAGS 0x00000009

#define THIS ((EnTk*)thisx)

void EnTk_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTk_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTk_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTk_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 EnTk_CheckNextSpot(EnTk* self, GlobalContext* globalCtx);
void EnTk_Rest(EnTk* self, GlobalContext* globalCtx);
void EnTk_Walk(EnTk* self, GlobalContext* globalCtx);
void EnTk_Dig(EnTk* self, GlobalContext* globalCtx);

const ActorInit En_Tk_InitVars = {
    ACTOR_EN_TK,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TK,
    sizeof(EnTk),
    (ActorFunc)EnTk_Init,
    (ActorFunc)EnTk_Destroy,
    (ActorFunc)EnTk_Update,
    (ActorFunc)EnTk_Draw,
};

void EnTkEff_Create(EnTk* self, Vec3f* pos, Vec3f* speed, Vec3f* accel, u8 duration, f32 size, f32 growth) {
    s16 i;
    EnTkEff* eff = self->eff;

    for (i = 0; i < ARRAY_COUNT(self->eff); i++) {
        if (eff->active != 1) {
            eff->size = size;
            eff->growth = growth;
            eff->timeTotal = eff->timeLeft = duration;
            eff->active = 1;
            eff->pos = *pos;
            eff->accel = *accel;
            eff->speed = *speed;
            break;
        }
        eff++;
    }
}

void EnTkEff_Update(EnTk* self) {
    s16 i;
    EnTkEff* eff;

    eff = self->eff;
    for (i = 0; i < ARRAY_COUNT(self->eff); i++) {
        if (eff->active != 0) {
            eff->timeLeft--;
            if (eff->timeLeft == 0) {
                eff->active = 0;
            }
            eff->accel.x = Rand_ZeroOne() * 0.4f - 0.2f;
            eff->accel.z = Rand_ZeroOne() * 0.4f - 0.2f;
            eff->pos.x += eff->speed.x;
            eff->pos.y += eff->speed.y;
            eff->pos.z += eff->speed.z;
            eff->speed.x += eff->accel.x;
            eff->speed.y += eff->accel.y;
            eff->speed.z += eff->accel.z;
            eff->size += eff->growth;
        }
        eff++;
    }
}

void EnTkEff_Draw(EnTk* self, GlobalContext* globalCtx) {
    static void* dustTextures[] = {
        &gDust8Tex, &gDust7Tex, &gDust6Tex, &gDust5Tex, &gDust4Tex, &gDust3Tex, &gDust2Tex, &gDust1Tex,
    };

    EnTkEff* eff = self->eff;
    s16 imageIdx;
    s16 gfxSetup;
    s16 alpha;
    s16 i;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tk_eff.c", 114);

    gfxSetup = 0;

    func_80093D84(globalCtx->state.gfxCtx);

    if (1) {}

    for (i = 0; i < ARRAY_COUNT(self->eff); i++) {
        if (eff->active != 0) {
            if (gfxSetup == 0) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);
                gSPDisplayList(POLY_XLU_DISP++, gDampeEff1DL);
                gDPSetEnvColor(POLY_XLU_DISP++, 100, 60, 20, 0);
                gfxSetup = 1;
            }

            alpha = eff->timeLeft * (255.0f / eff->timeTotal);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 170, 130, 90, alpha);

            gDPPipeSync(POLY_XLU_DISP++);
            Matrix_Translate(eff->pos.x, eff->pos.y, eff->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(eff->size, eff->size, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_tk_eff.c", 140),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            imageIdx = eff->timeLeft * ((f32)ARRAY_COUNT(dustTextures) / eff->timeTotal);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(dustTextures[imageIdx]));

            gSPDisplayList(POLY_XLU_DISP++, gDampeEff2DL);
        }
        eff++;
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tk_eff.c", 154);
}

s32 EnTkEff_CreateDflt(EnTk* self, Vec3f* pos, u8 duration, f32 size, f32 growth, f32 yAccelMax) {
    Vec3f speed = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };

    accel.y += Rand_ZeroOne() * yAccelMax;

    EnTkEff_Create(self, pos, &speed, &accel, duration, size, growth);

    return 0;
}

/** z_en_tk_eff.c ends here probably **/

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 30, 52, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

void EnTk_RestAnim(EnTk* self, GlobalContext* globalCtx) {
    AnimationHeader* anim = &gDampeRestAnim;

    Animation_Change(&self->skelAnime, anim, 1.0f, 0.0f, Animation_GetLastFrame(&gDampeRestAnim), ANIMMODE_LOOP,
                     -10.0f);

    self->actionCountdown = Rand_S16Offset(60, 60);
    self->actor.speedXZ = 0.0f;
}

void EnTk_WalkAnim(EnTk* self, GlobalContext* globalCtx) {
    AnimationHeader* anim = &gDampeWalkAnim;

    Animation_Change(&self->skelAnime, anim, 1.0f, 0.0f, Animation_GetLastFrame(&gDampeRestAnim), ANIMMODE_LOOP,
                     -10.0f);

    self->actionCountdown = Rand_S16Offset(240, 240);
}

void EnTk_DigAnim(EnTk* self, GlobalContext* globalCtx) {
    AnimationHeader* anim = &gDampeDigAnim;

    Animation_Change(&self->skelAnime, anim, 1.0f, 0.0f, Animation_GetLastFrame(&gDampeDigAnim), ANIMMODE_LOOP, -10.0f);

    if (EnTk_CheckNextSpot(self, globalCtx) >= 0) {
        self->validDigHere = 1;
    }
}

void EnTk_UpdateEyes(EnTk* self) {
    if (DECR(self->blinkCountdown) == 0) {
        self->eyeTextureIdx++;
        if (self->eyeTextureIdx > 2) {
            self->blinkCycles--;
            if (self->blinkCycles < 0) {
                self->blinkCountdown = Rand_S16Offset(30, 30);
                self->blinkCycles = 2;
                if (Rand_ZeroOne() > 0.5f) {
                    self->blinkCycles++;
                }
            }
            self->eyeTextureIdx = 0;
        }
    }
}

s32 EnTk_CheckFacingPlayer(EnTk* self) {
    s16 v0;
    s16 v1;

    if (self->actor.xyzDistToPlayerSq > 10000.0f) {
        return 0;
    }

    v0 = self->actor.shape.rot.y;
    v0 -= self->h_21E;
    v0 -= self->headRot;

    v1 = self->actor.yawTowardsPlayer - v0;
    if (ABS(v1) < 0x1554) {
        return 1;
    } else {
        return 0;
    }
}

s32 EnTk_CheckNextSpot(EnTk* self, GlobalContext* globalCtx) {
    Actor* prop;
    f32 dxz;
    f32 dy;

    prop = globalCtx->actorCtx.actorLists[ACTORCAT_PROP].head;

    while (prop != NULL) {
        if (prop->id != ACTOR_EN_IT) {
            prop = prop->next;
            continue;
        }

        if (prop == self->currentSpot) {
            prop = prop->next;
            continue;
        }

        dy = prop->world.pos.y - self->actor.floorHeight;
        dxz = Actor_WorldDistXZToActor(&self->actor, prop);
        if (dxz > 40.0f || dy > 10.0f) {
            prop = prop->next;
            continue;
        }

        self->currentSpot = prop;
        return prop->params;
    }

    return -1;
}

void EnTk_CheckCurrentSpot(EnTk* self) {
    f32 dxz;
    f32 dy;

    if (self->currentSpot != NULL) {
        dy = self->currentSpot->world.pos.y - self->actor.floorHeight;
        dxz = Actor_WorldDistXZToActor(&self->actor, self->currentSpot);
        if (dxz > 40.0f || dy > 10.0f) {
            self->currentSpot = NULL;
        }
    }
}

f32 EnTk_Step(EnTk* self, GlobalContext* globalCtx) {
    f32 stepFrames[] = { 36.0f, 10.0f };
    f32 a1_;
    s32 i;

    if (self->skelAnime.curFrame == 0.0f || self->skelAnime.curFrame == 25.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_WALK);
    }

    if (self->skelAnime.animation != &gDampeWalkAnim) {
        return 0.0f;
    }

    a1_ = self->skelAnime.curFrame;
    for (i = 0; i < ARRAY_COUNT(stepFrames); i++) {
        if (a1_ < stepFrames[i] + 12.0f && a1_ >= stepFrames[i]) {
            break;
        }
    }
    if (i >= ARRAY_COUNT(stepFrames)) {
        return 0.0f;
    } else {
        a1_ = (0x8000 / 12.0f) * (a1_ - stepFrames[i]);
        return Math_SinS(a1_) * 2.0f;
    }
}

s32 EnTk_Orient(EnTk* self, GlobalContext* globalCtx) {
    Path* path;
    Vec3s* point;
    f32 dx;
    f32 dz;

    if (self->actor.params < 0) {
        return 1;
    }

    path = &globalCtx->setupPathList[0];
    point = SEGMENTED_TO_VIRTUAL(path->points);
    point += self->currentWaypoint;

    dx = point->x - self->actor.world.pos.x;
    dz = point->z - self->actor.world.pos.z;

    Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(dx, dz) * (0x8000 / M_PI), 10, 1000, 1);
    self->actor.world.rot = self->actor.shape.rot;

    if (SQ(dx) + SQ(dz) < 10.0f) {
        self->currentWaypoint++;
        if (self->currentWaypoint >= path->count) {
            self->currentWaypoint = 0;
        }

        return 0;
    } else {
        return 1;
    }
}

u16 func_80B1C54C(GlobalContext* globalCtx, Actor* thisx) {
    u16 ret;

    ret = Text_GetFaceReaction(globalCtx, 14);
    if (ret != 0) {
        return ret;
    }

    if (gSaveContext.infTable[13] & 0x0200) {
        /* "Do you want me to dig here? ..." */
        return 0x5019;
    } else {
        /* "Hey kid! ..." */
        return 0x5018;
    }
}

s16 func_80B1C5A0(GlobalContext* globalCtx, Actor* thisx) {
    s32 ret = 1;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 0:
        case 1:
            break;
        case 2:
            /* "I am the boss of the carpenters ..." (wtf?) */
            if (thisx->textId == 0x5028) {
                gSaveContext.infTable[13] |= 0x0100;
            }
            ret = 0;
            break;
        case 3:
            break;
        case 4:
            if (func_80106BC8(globalCtx) && (thisx->textId == 0x5018 || thisx->textId == 0x5019)) {
                if (globalCtx->msgCtx.choiceIndex == 1) {
                    /* "Thanks a lot!" */
                    thisx->textId = 0x0084;
                } else if (gSaveContext.rupees < 10) {
                    /* "You don't have enough Rupees!" */
                    thisx->textId = 0x0085;
                } else {
                    globalCtx->msgCtx.msgMode = 0x37;
                    Rupees_ChangeBy(-10);
                    gSaveContext.infTable[13] |= 0x0200;
                    return 2;
                }
                func_8010B720(globalCtx, thisx->textId);
                gSaveContext.infTable[13] |= 0x0200;
            }
            break;
        case 5:
            if (func_80106BC8(globalCtx) && (thisx->textId == 0x0084 || thisx->textId == 0x0085)) {
                func_80106CCC(globalCtx);
                ret = 0;
            }
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            break;
    }

    return ret;
}

s32 EnTk_ChooseReward(EnTk* self) {
    f32 luck;
    s32 reward;

    luck = Rand_ZeroOne();

    if (luck < 0.4f) {
        reward = 0;
    } else if (luck < 0.7) {
        reward = 1;
    } else if (luck < 0.9) {
        reward = 2;
    } else {
        reward = 3;
    }

    switch (reward) {
        case 0:
            if (self->rewardCount[0] < 8) {
                self->rewardCount[0] += 1;
                return reward;
            }
            break;
        case 1:
            if (self->rewardCount[1] < 4) {
                self->rewardCount[1] += 1;
                return reward;
            }
            break;
        case 2:
            if (self->rewardCount[2] < 2) {
                self->rewardCount[2] += 1;
                return reward;
            }
            break;
        case 3:
            if (self->rewardCount[3] < 1) {
                self->rewardCount[3] += 1;
                return reward;
            }
            break;
    }

    if (self->rewardCount[0] < 8) {
        self->rewardCount[0] += 1;
        reward = 0;
    } else if (self->rewardCount[1] < 4) {
        self->rewardCount[1] += 1;
        reward = 1;
    } else if (self->rewardCount[2] < 2) {
        self->rewardCount[2] += 1;
        reward = 2;
    } else if (self->rewardCount[3] < 1) {
        self->rewardCount[3] += 1;
        reward = 3;
    } else {
        reward = 0;
        self->rewardCount[0] = 1;
        self->rewardCount[1] = 0;
        self->rewardCount[2] = 0;
        self->rewardCount[3] = 0;
    }

    return reward;
}

void EnTk_DigEff(EnTk* self) {
    Vec3f pos = { 0.0f, 0.0f, 0.0f };
    Vec3f speed = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };

    if (self->skelAnime.curFrame >= 32.0f && self->skelAnime.curFrame < 40.0f) {
        pos.x = (Rand_ZeroOne() - 0.5f) * 12.0f + self->v3f_304.x;
        pos.y = (Rand_ZeroOne() - 0.5f) * 8.0f + self->v3f_304.y;
        pos.z = (Rand_ZeroOne() - 0.5f) * 12.0f + self->v3f_304.z;
        EnTkEff_CreateDflt(self, &pos, 12, 0.2f, 0.1f, 0.0f);
    }
}

void EnTk_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnTk* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0, ActorShadow_DrawCircle, 24.0f);

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gDampeSkel, NULL, self->jointTable, self->morphTable, 18);
    Animation_Change(&self->skelAnime, &gDampeRestAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gDampeRestAnim),
                     ANIMMODE_LOOP, 0.0f);

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);

    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit);

    if (gSaveContext.dayTime <= 0xC000 || gSaveContext.dayTime >= 0xE000 || !!LINK_IS_ADULT ||
        globalCtx->sceneNum != SCENE_SPOT02) {
        Actor_Kill(&self->actor);
        return;
    }

    Actor_SetScale(&self->actor, 0.01f);

    self->actor.targetMode = 6;
    self->actor.gravity = -0.1f;
    self->currentReward = -1;
    self->currentSpot = NULL;
    self->actionFunc = EnTk_Rest;
}

void EnTk_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnTk* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnTk_Rest(EnTk* self, GlobalContext* globalCtx) {
    s16 v1;
    s16 a1_;

    if (self->h_1E0 != 0) {
        v1 = self->actor.shape.rot.y;
        v1 -= self->h_21E;
        v1 = self->actor.yawTowardsPlayer - v1;

        if (self->h_1E0 == 2) {
            EnTk_DigAnim(self, globalCtx);
            self->h_1E0 = 0;
            self->actionFunc = EnTk_Dig;
            return;
        }

        func_800343CC(globalCtx, &self->actor, &self->h_1E0, self->collider.dim.radius + 30.0f, func_80B1C54C,
                      func_80B1C5A0);
    } else if (EnTk_CheckFacingPlayer(self)) {
        v1 = self->actor.shape.rot.y;
        v1 -= self->h_21E;
        v1 = self->actor.yawTowardsPlayer - v1;

        self->actionCountdown = 0;
        func_800343CC(globalCtx, &self->actor, &self->h_1E0, self->collider.dim.radius + 30.0f, func_80B1C54C,
                      func_80B1C5A0);
    } else if (func_8002F194(&self->actor, globalCtx)) {
        v1 = self->actor.shape.rot.y;
        v1 -= self->h_21E;
        v1 = self->actor.yawTowardsPlayer - v1;

        self->actionCountdown = 0;
        self->h_1E0 = 1;
    } else if (DECR(self->actionCountdown) == 0) {
        EnTk_WalkAnim(self, globalCtx);
        self->actionFunc = EnTk_Walk;

        /*! @bug v1 is uninitialized past self branch */
    } else {
        v1 = 0;
    }

    a1_ = CLAMP(-v1, 1270, 10730);
    Math_SmoothStepToS(&self->headRot, a1_, 6, 1000, 1);
}

void EnTk_Walk(EnTk* self, GlobalContext* globalCtx) {
    if (self->h_1E0 == 2) {
        EnTk_DigAnim(self, globalCtx);
        self->h_1E0 = 0;
        self->actionFunc = EnTk_Dig;
    } else {
        self->actor.speedXZ = EnTk_Step(self, globalCtx);
        EnTk_Orient(self, globalCtx);
        Math_SmoothStepToS(&self->headRot, 0, 6, 1000, 1);
        EnTk_CheckCurrentSpot(self);

        DECR(self->actionCountdown);
        if (EnTk_CheckFacingPlayer(self) || self->actionCountdown == 0) {
            EnTk_RestAnim(self, globalCtx);
            self->actionFunc = EnTk_Rest;
        }
    }
}

void EnTk_Dig(EnTk* self, GlobalContext* globalCtx) {
    Vec3f rewardOrigin;
    Vec3f rewardPos;
    s32 rewardParams[] = {
        ITEM00_RUPEE_GREEN, ITEM00_RUPEE_BLUE, ITEM00_RUPEE_RED, ITEM00_RUPEE_PURPLE, ITEM00_HEART_PIECE,
    };

    EnTk_DigEff(self);

    if (self->skelAnime.curFrame == 32.0f) {
        /* What's gonna come out? */
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_DIG_UP);

        self->rewardTimer = 0;

        if (self->validDigHere == 1) {
            rewardOrigin.x = 0.0f;
            rewardOrigin.y = 0.0f;
            rewardOrigin.z = -40.0f;

            Matrix_RotateY(self->actor.shape.rot.y, MTXMODE_NEW);
            Matrix_MultVec3f(&rewardOrigin, &rewardPos);

            rewardPos.x += self->actor.world.pos.x;
            rewardPos.y += self->actor.world.pos.y;
            rewardPos.z += self->actor.world.pos.z;

            self->currentReward = EnTk_ChooseReward(self);
            if (self->currentReward == 3) {
                /*
                 * Upgrade the purple rupee reward to the heart piece if self
                 * is the first grand prize dig.
                 */
                if (!(gSaveContext.itemGetInf[1] & 0x1000)) {
                    gSaveContext.itemGetInf[1] |= 0x1000;
                    self->currentReward = 4;
                }
            }

            Item_DropCollectible(globalCtx, &rewardPos, rewardParams[self->currentReward]);
        }
    }

    if (self->skelAnime.curFrame >= 32.0f && self->rewardTimer == 10) {
        /* Play a reward sound shortly after digging */
        if (self->validDigHere == 0) {
            /* Bad dig spot */
            Audio_PlayActorSound2(&self->actor, NA_SE_SY_ERROR);
        } else if (self->currentReward == 4) {
            /* Heart piece */
            Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        } else {
            /* Rupee */
            Audio_PlayActorSound2(&self->actor, NA_SE_SY_TRE_BOX_APPEAR);
        }
    }
    self->rewardTimer++;

    if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
        if (self->currentReward < 0) {
            /* "Nope, nothing here!" */
            func_8010B680(globalCtx, 0x501A, NULL);
        } else {
            func_80106CCC(globalCtx);
        }

        EnTk_RestAnim(self, globalCtx);

        self->currentReward = -1;
        self->validDigHere = 0;
        self->actionFunc = EnTk_Rest;
    }
}

void EnTk_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnTk* self = THIS;
    s32 pad;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

    SkelAnime_Update(&self->skelAnime);

    Actor_MoveForward(&self->actor);

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 40.0f, 10.0f, 0.0f, 5);

    self->actionFunc(self, globalCtx);

    EnTkEff_Update(self);

    EnTk_UpdateEyes(self);
}

void func_80B1D200(GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tk.c", 1188);

    gSPDisplayList(POLY_OPA_DISP++, gDampeShovelDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tk.c", 1190);
}

s32 EnTk_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnTk* self = THIS;

    switch (limbIndex) {
        /* Limb 15 - Head */
        case 15:
            self->h_21E = rot->y;
            break;
        /* Limb 16 - Jaw */
        case 16:
            self->h_21E += rot->y;
            rot->y += self->headRot;
            break;
    }

    return false;
}

void EnTk_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnTk* self = THIS;
    Vec3f sp28 = { 0.0f, 0.0f, 4600.0f };
    Vec3f sp1C = { 0.0f, 0.0f, 0.0f };

    /* Limb 16 - Jaw */
    if (limbIndex == 16) {
        Matrix_MultVec3f(&sp1C, &self->actor.focus.pos);
    }

    /* Limb 14 - Neck */
    if (limbIndex == 14) {
        Matrix_MultVec3f(&sp28, &self->v3f_304);
        func_80B1D200(globalCtx);
    }
}

void EnTk_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* sEyesSegments[] = {
        gDampeEyeOpenTex,
        gDampeEyeHalfTex,
        gDampeEyeClosedTex,
    };
    EnTk* self = THIS;

    Matrix_Push();
    EnTkEff_Draw(self, globalCtx);
    Matrix_Pop();

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tk.c", 1294);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyesSegments[self->eyeTextureIdx]));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnTk_OverrideLimbDraw, EnTk_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tk.c", 1312);
}
