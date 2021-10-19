/*
 * File: z_en_hintnuts.c
 * Overlay: ovl_En_Hintnuts
 * Description: Hint Deku Scrubs (Deku Tree)
 */

#include "z_en_hintnuts.h"
#include "objects/object_hintnuts/object_hintnuts.h"

#define FLAGS 0x00000005

#define THIS ((EnHintnuts*)thisx)

void EnHintnuts_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHintnuts_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHintnuts_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHintnuts_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnHintnuts_SetupWait(EnHintnuts* self);
void EnHintnuts_Wait(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_LookAround(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_Stand(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_ThrowNut(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_Burrow(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_BeginRun(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_BeginFreeze(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_Run(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_Talk(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_Leave(EnHintnuts* self, GlobalContext* globalCtx);
void EnHintnuts_Freeze(EnHintnuts* self, GlobalContext* globalCtx);

const ActorInit En_Hintnuts_InitVars = {
    ACTOR_EN_HINTNUTS,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_HINTNUTS,
    sizeof(EnHintnuts),
    (ActorFunc)EnHintnuts_Init,
    (ActorFunc)EnHintnuts_Destroy,
    (ActorFunc)EnHintnuts_Update,
    (ActorFunc)EnHintnuts_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT6,
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
    { 18, 32, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 1, 18, 32, MASS_HEAVY };

static s16 sPuzzleCounter = 0;

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(gravity, -1, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x0A, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2600, ICHAIN_STOP),
};

void EnHintnuts_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnHintnuts* self = THIS;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    if (self->actor.params == 0xA) {
        self->actor.flags &= ~5;
    } else {
        ActorShape_Init(&self->actor.shape, 0x0, ActorShadow_DrawCircle, 35.0f);
        SkelAnime_Init(globalCtx, &self->skelAnime, &gHintNutsSkel, &gHintNutsStandAnim, self->jointTable,
                       self->morphTable, 10);
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
        Actor_SetTextWithPrefix(globalCtx, &self->actor, (self->actor.params >> 8) & 0xFF);
        self->textIdCopy = self->actor.textId;
        self->actor.params &= 0xFF;
        sPuzzleCounter = 0;
        if (self->actor.textId == 0x109B) {
            if (Flags_GetClear(globalCtx, 0x9) != 0) {
                Actor_Kill(&self->actor);
                return;
            }
        }
        EnHintnuts_SetupWait(self);
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_HINTNUTS, self->actor.world.pos.x,
                           self->actor.world.pos.y, self->actor.world.pos.z, 0, self->actor.world.rot.y, 0, 0xA);
    }
}

void EnHintnuts_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnHintnuts* self = THIS;
    if (self->actor.params != 0xA) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void EnHintnuts_HitByScrubProjectile1(EnHintnuts* self, GlobalContext* globalCtx) {
    if (self->actor.textId != 0 && self->actor.category == ACTORCAT_ENEMY &&
        ((self->actor.params == 0) || (sPuzzleCounter == 2))) {
        self->actor.flags &= ~0x5;
        self->actor.flags |= 0x9;
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_BG);
    }
}

void EnHintnuts_SetupWait(EnHintnuts* self) {
    Animation_PlayOnceSetSpeed(&self->skelAnime, &gHintNutsUpAnim, 0.0f);
    self->animFlagAndTimer = Rand_S16Offset(100, 50);
    self->collider.dim.height = 5;
    self->actor.world.pos = self->actor.home.pos;
    self->collider.base.acFlags &= ~AC_ON;
    self->actionFunc = EnHintnuts_Wait;
}

void EnHintnuts_SetupLookAround(EnHintnuts* self) {
    Animation_PlayLoop(&self->skelAnime, &gHintNutsLookAroundAnim);
    self->animFlagAndTimer = 2;
    self->actionFunc = EnHintnuts_LookAround;
}

void EnHintnuts_SetupThrowScrubProjectile(EnHintnuts* self) {
    Animation_PlayOnce(&self->skelAnime, &gHintNutsSpitAnim);
    self->actionFunc = EnHintnuts_ThrowNut;
}

void EnHintnuts_SetupStand(EnHintnuts* self) {
    Animation_MorphToLoop(&self->skelAnime, &gHintNutsStandAnim, -3.0f);
    if (self->actionFunc == EnHintnuts_ThrowNut) {
        self->animFlagAndTimer = 2 | 0x1000; // sets timer and flag
    } else {
        self->animFlagAndTimer = 1;
    }
    self->actionFunc = EnHintnuts_Stand;
}

void EnHintnuts_SetupBurrow(EnHintnuts* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gHintNutsBurrowAnim, -5.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DOWN);
    self->actionFunc = EnHintnuts_Burrow;
}

void EnHintnuts_HitByScrubProjectile2(EnHintnuts* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gHintNutsUnburrowAnim, -3.0f);
    self->collider.dim.height = 37;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DAMAGE);
    self->collider.base.acFlags &= ~AC_ON;

    if (self->actor.params > 0 && self->actor.params < 4 && self->actor.category == ACTORCAT_ENEMY) {
        if (sPuzzleCounter == -4) {
            sPuzzleCounter = 0;
        }
        if (self->actor.params == sPuzzleCounter + 1) {
            sPuzzleCounter++;
        } else {
            if (sPuzzleCounter > 0) {
                sPuzzleCounter = -sPuzzleCounter;
            }
            sPuzzleCounter--;
        }
        self->actor.flags |= 0x10;
        self->actionFunc = EnHintnuts_BeginFreeze;
    } else {
        self->actionFunc = EnHintnuts_BeginRun;
    }
}

void EnHintnuts_SetupRun(EnHintnuts* self) {
    Animation_PlayLoop(&self->skelAnime, &gHintNutsRunAnim);
    self->animFlagAndTimer = 5;
    self->actionFunc = EnHintnuts_Run;
}

void EnHintnuts_SetupTalk(EnHintnuts* self) {
    Animation_MorphToLoop(&self->skelAnime, &gHintNutsTalkAnim, -5.0f);
    self->actionFunc = EnHintnuts_Talk;
    self->actor.speedXZ = 0.0f;
}

void EnHintnuts_SetupLeave(EnHintnuts* self, GlobalContext* globalCtx) {
    Animation_MorphToLoop(&self->skelAnime, &gHintNutsRunAnim, -5.0f);
    self->actor.speedXZ = 3.0f;
    self->animFlagAndTimer = 100;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->collider.base.ocFlags1 &= ~OC1_ON;
    self->actor.flags |= 0x10;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DAMAGE);
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ITEM00, self->actor.world.pos.x, self->actor.world.pos.y,
                self->actor.world.pos.z, 0x0, 0x0, 0x0, 0x3); // recovery heart
    self->actionFunc = EnHintnuts_Leave;
}

void EnHintnuts_SetupFreeze(EnHintnuts* self) {
    Animation_PlayLoop(&self->skelAnime, &gHintNutsFreezeAnim);
    self->actor.flags &= ~1;
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 100);
    self->actor.colorFilterTimer = 1;
    self->animFlagAndTimer = 0;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_FAINT);
    if (sPuzzleCounter == -3) {
        func_80078884(NA_SE_SY_ERROR);
        sPuzzleCounter = -4;
    }
    self->actionFunc = EnHintnuts_Freeze;
}

void EnHintnuts_Wait(EnHintnuts* self, GlobalContext* globalCtx) {
    s32 hasSlowPlaybackSpeed = false;

    if (self->skelAnime.playSpeed < 0.5f) {
        hasSlowPlaybackSpeed = true;
    }
    if (hasSlowPlaybackSpeed && (self->animFlagAndTimer != 0)) {
        self->animFlagAndTimer--;
    }
    if (Animation_OnFrame(&self->skelAnime, 9.0f)) {
        self->collider.base.acFlags |= AC_ON;
    } else if (Animation_OnFrame(&self->skelAnime, 8.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
    }

    self->collider.dim.height = 5.0f + ((CLAMP(self->skelAnime.curFrame, 9.0f, 12.0f) - 9.0f) * 9.0f);
    if (!hasSlowPlaybackSpeed && (self->actor.xzDistToPlayer < 120.0f)) {
        EnHintnuts_SetupBurrow(self);
    } else if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.xzDistToPlayer < 120.0f) {
            EnHintnuts_SetupBurrow(self);
        } else if ((self->animFlagAndTimer == 0) && (self->actor.xzDistToPlayer > 320.0f)) {
            EnHintnuts_SetupLookAround(self);
        } else {
            EnHintnuts_SetupStand(self);
        }
    }
    if (hasSlowPlaybackSpeed && 160.0f < self->actor.xzDistToPlayer && fabsf(self->actor.yDistToPlayer) < 120.0f &&
        ((self->animFlagAndTimer == 0) || (self->actor.xzDistToPlayer < 480.0f))) {
        self->skelAnime.playSpeed = 1.0f;
    }
}

void EnHintnuts_LookAround(EnHintnuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && self->animFlagAndTimer != 0) {
        self->animFlagAndTimer--;
    }
    if ((self->actor.xzDistToPlayer < 120.0f) || (self->animFlagAndTimer == 0)) {
        EnHintnuts_SetupBurrow(self);
    }
}

void EnHintnuts_Stand(EnHintnuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) && self->animFlagAndTimer != 0) {
        self->animFlagAndTimer--;
    }
    if (!(self->animFlagAndTimer & 0x1000)) {
        Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    }
    if (self->actor.xzDistToPlayer < 120.0f || self->animFlagAndTimer == 0x1000) {
        EnHintnuts_SetupBurrow(self);
    } else if (self->animFlagAndTimer == 0) {
        EnHintnuts_SetupThrowScrubProjectile(self);
    }
}

void EnHintnuts_ThrowNut(EnHintnuts* self, GlobalContext* globalCtx) {
    Vec3f nutPos;

    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
    if (self->actor.xzDistToPlayer < 120.0f) {
        EnHintnuts_SetupBurrow(self);
    } else if (SkelAnime_Update(&self->skelAnime)) {
        EnHintnuts_SetupStand(self);
    } else if (Animation_OnFrame(&self->skelAnime, 6.0f)) {
        nutPos.x = self->actor.world.pos.x + (Math_SinS(self->actor.shape.rot.y) * 23.0f);
        nutPos.y = self->actor.world.pos.y + 12.0f;
        nutPos.z = self->actor.world.pos.z + (Math_CosS(self->actor.shape.rot.y) * 23.0f);
        if (Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_NUTSBALL, nutPos.x, nutPos.y, nutPos.z,
                        self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z, 1) != NULL) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_THROW);
        }
    }
}

void EnHintnuts_Burrow(EnHintnuts* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnHintnuts_SetupWait(self);
    } else {
        self->collider.dim.height = 5.0f + ((3.0f - CLAMP(self->skelAnime.curFrame, 1.0f, 3.0f)) * 12.0f);
    }
    if (Animation_OnFrame(&self->skelAnime, 4.0f)) {
        self->collider.base.acFlags &= ~AC_ON;
    }

    Math_ApproachF(&self->actor.world.pos.x, self->actor.home.pos.x, 0.5f, 3.0f);
    Math_ApproachF(&self->actor.world.pos.z, self->actor.home.pos.z, 0.5f, 3.0f);
}

void EnHintnuts_BeginRun(EnHintnuts* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->unk_196 = self->actor.yawTowardsPlayer + 0x8000;
        EnHintnuts_SetupRun(self);
    }
    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 0xE38);
}

void EnHintnuts_BeginFreeze(EnHintnuts* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnHintnuts_SetupFreeze(self);
    }
}

void EnHintnuts_CheckProximity(EnHintnuts* self, GlobalContext* globalCtx) {
    if (self->actor.category != ACTORCAT_ENEMY) {
        if ((self->collider.base.ocFlags1 & OC1_HIT) || self->actor.isTargeted) {
            self->actor.flags |= 0x10000;
        } else {
            self->actor.flags &= ~0x10000;
        }
        if (self->actor.xzDistToPlayer < 130.0f) {
            self->actor.textId = self->textIdCopy;
            func_8002F2F4(&self->actor, globalCtx);
        }
    }
}

void EnHintnuts_Run(EnHintnuts* self, GlobalContext* globalCtx) {
    s32 temp_ret;
    s16 diffRotInit;
    s16 diffRot;
    f32 phi_f0;

    SkelAnime_Update(&self->skelAnime);
    temp_ret = Animation_OnFrame(&self->skelAnime, 0.0f);
    if (temp_ret != 0 && self->animFlagAndTimer != 0) {
        self->animFlagAndTimer--;
    }
    if ((temp_ret != 0) || (Animation_OnFrame(&self->skelAnime, 6.0f))) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }

    Math_StepToF(&self->actor.speedXZ, 7.5f, 1.0f);
    if (Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_196, 1, 0xE38, 0xB6) == 0) {
        if (self->actor.bgCheckFlags & 0x20) {
            self->unk_196 = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
        } else if (self->actor.bgCheckFlags & 8) {
            self->unk_196 = self->actor.wallYaw;
        } else if (self->animFlagAndTimer == 0) {
            diffRotInit = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
            diffRot = diffRotInit - self->actor.yawTowardsPlayer;
            if (ABS(diffRot) >= 0x2001) {
                self->unk_196 = diffRotInit;
            } else {
                phi_f0 = (0.0f <= (f32)diffRot) ? 1.0f : -1.0f;
                self->unk_196 = (s16)((phi_f0 * -8192.0f) + (f32)self->actor.yawTowardsPlayer);
            }
        } else {
            self->unk_196 = (s16)(self->actor.yawTowardsPlayer + 0x8000);
        }
    }

    self->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        EnHintnuts_SetupTalk(self);
    } else if (self->animFlagAndTimer == 0 && Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos) < 20.0f &&
               fabsf(self->actor.world.pos.y - self->actor.home.pos.y) < 2.0f) {
        self->actor.speedXZ = 0.0f;
        if (self->actor.category == ACTORCAT_BG) {
            self->actor.flags &= ~0x00010009;
            self->actor.flags |= 0x5;
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_ENEMY);
        }
        EnHintnuts_SetupBurrow(self);
    } else {
        EnHintnuts_CheckProximity(self, globalCtx);
    }
}

void EnHintnuts_Talk(EnHintnuts* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0x3, 0x400, 0x100);
    if (func_8010BDBC(&globalCtx->msgCtx) == 5) {
        EnHintnuts_SetupLeave(self, globalCtx);
    }
}

void EnHintnuts_Leave(EnHintnuts* self, GlobalContext* globalCtx) {
    s16 temp_a1;

    SkelAnime_Update(&self->skelAnime);
    if (self->animFlagAndTimer != 0) {
        self->animFlagAndTimer--;
    }
    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 6.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }
    if (self->actor.bgCheckFlags & 8) {
        temp_a1 = self->actor.wallYaw;
    } else {
        temp_a1 = self->actor.yawTowardsPlayer - Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) - 0x8000;
        if (ABS(temp_a1) >= 0x4001) {
            temp_a1 = Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000;
        } else {
            temp_a1 = Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) - (temp_a1 >> 1) + 0x8000;
        }
    }
    Math_ScaledStepToS(&self->actor.shape.rot.y, temp_a1, 0x800);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if ((self->animFlagAndTimer == 0) || (self->actor.projectedPos.z < 0.0f)) {
        func_80106CCC(globalCtx);
        if (self->actor.params == 3) {
            Flags_SetClear(globalCtx, self->actor.room);
            sPuzzleCounter = 3;
        }
        if (self->actor.child != NULL) {
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, self->actor.child, ACTORCAT_PROP);
        }
        Actor_Kill(&self->actor);
    }
}

void EnHintnuts_Freeze(EnHintnuts* self, GlobalContext* globalCtx) {
    self->actor.colorFilterTimer = 1;
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_FAINT);
    }
    if (self->animFlagAndTimer == 0) {
        if (sPuzzleCounter == 3) {
            if (self->actor.child != NULL) {
                Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, self->actor.child, ACTORCAT_PROP);
            }
            self->animFlagAndTimer = 1;
        } else if (sPuzzleCounter == -4) {
            self->animFlagAndTimer = 2;
        }
    } else if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - 35.0f, 7.0f) != 0) {
        if (self->animFlagAndTimer == 1) {
            Actor_Kill(&self->actor);
        } else {
            self->actor.flags |= 1;
            self->actor.flags &= ~0x10;
            self->actor.colChkInfo.health = sColChkInfoInit.health;
            self->actor.colorFilterTimer = 0;
            EnHintnuts_SetupWait(self);
        }
    }
}

void EnHintnuts_ColliderCheck(EnHintnuts* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&self->actor, &self->collider.info, 1);
        if (self->collider.base.ac->id != ACTOR_EN_NUTSBALL) {
            EnHintnuts_SetupBurrow(self);
        } else {
            EnHintnuts_HitByScrubProjectile1(self, globalCtx);
            EnHintnuts_HitByScrubProjectile2(self);
        }
    } else if (globalCtx->actorCtx.unk_02 != 0) {
        EnHintnuts_HitByScrubProjectile1(self, globalCtx);
        EnHintnuts_HitByScrubProjectile2(self);
    }
}

void EnHintnuts_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnHintnuts* self = THIS;
    s32 pad;

    if (self->actor.params != 0xA) {
        EnHintnuts_ColliderCheck(self, globalCtx);
        self->actionFunc(self, globalCtx);
        if (self->actionFunc != EnHintnuts_Freeze && self->actionFunc != EnHintnuts_BeginFreeze) {
            Actor_MoveForward(&self->actor);
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, self->collider.dim.radius,
                                    self->collider.dim.height, 0x1D);
        }
        Collider_UpdateCylinder(&self->actor, &self->collider);
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        if (self->actionFunc == EnHintnuts_Wait) {
            Actor_SetFocus(&self->actor, self->skelAnime.curFrame);
        } else if (self->actionFunc == EnHintnuts_Burrow) {
            Actor_SetFocus(&self->actor,
                           20.0f - ((self->skelAnime.curFrame * 20.0f) / Animation_GetLastFrame(&gHintNutsBurrowAnim)));
        } else {
            Actor_SetFocus(&self->actor, 20.0f);
        }
    }
}

s32 EnHintnuts_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx) {
    Vec3f vec;
    f32 curFrame;
    EnHintnuts* self = THIS;

    if (limbIndex == 5 && self->actionFunc == EnHintnuts_ThrowNut) {
        curFrame = self->skelAnime.curFrame;
        if (curFrame <= 6.0f) {
            vec.y = 1.0f - (curFrame * 0.0833f);
            vec.z = 1.0f + (curFrame * 0.1167f);
            vec.x = 1.0f + (curFrame * 0.1167f);
        } else if (curFrame <= 7.0f) {
            curFrame -= 6.0f;
            vec.y = 0.5f + curFrame;
            vec.z = 1.7f - (curFrame * 0.7f);
            vec.x = 1.7f - (curFrame * 0.7f);
        } else if (curFrame <= 10.0f) {
            vec.y = 1.5f - ((curFrame - 7.0f) * 0.1667f);
            vec.z = 1.0f;
            vec.x = 1.0f;
        } else {
            return false;
        }
        Matrix_Scale(vec.x, vec.y, vec.z, MTXMODE_APPLY);
    }
    return false;
}

void EnHintnuts_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnHintnuts* self = THIS;

    if (self->actor.params == 0xA) {
        Gfx_DrawDListOpa(globalCtx, gHintNutsFlowerDL);
    } else {
        SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnHintnuts_OverrideLimbDraw,
                          NULL, self);
    }
}
