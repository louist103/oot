/*
 * File: z_en_dnt_jiji.c
 * Overlay: ovl_En_Dnt_Jiji
 * Description: Forest Stage scrub leader
 */

#include "z_en_dnt_jiji.h"
#include "objects/object_dns/object_dns.h"
#include "overlays/actors/ovl_En_Dnt_Demo/z_en_dnt_demo.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnDntJiji*)thisx)

void EnDntJiji_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDntJiji_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDntJiji_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDntJiji_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDntJiji_SetFlower(EnDntJiji* self, GlobalContext* globalCtx);

void EnDntJiji_SetupWait(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_SetupUnburrow(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_SetupWalk(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_SetupCower(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_SetupGivePrize(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_SetupHide(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_SetupReturn(EnDntJiji* self, GlobalContext* globalCtx);

void EnDntJiji_Wait(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_Up(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_Unburrow(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_Walk(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_Burrow(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_Cower(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_SetupTalk(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_Talk(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_GivePrize(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_Hide(EnDntJiji* self, GlobalContext* globalCtx);
void EnDntJiji_Return(EnDntJiji* self, GlobalContext* globalCtx);

const ActorInit En_Dnt_Jiji_InitVars = {
    ACTOR_EN_DNT_JIJI,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DNS,
    sizeof(EnDntJiji),
    (ActorFunc)EnDntJiji_Init,
    (ActorFunc)EnDntJiji_Destroy,
    (ActorFunc)EnDntJiji_Update,
    (ActorFunc)EnDntJiji_Draw,
};

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
    { 30, 80, 0, { 0, 0, 0 } },
};

void EnDntJiji_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDntJiji* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gDntJijiSkel, &gDntJijiBurrowAnim, self->jointTable, self->morphTable,
                   13);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->stage = (EnDntDemo*)self->actor.parent;
    osSyncPrintf("\n\n");
    // "Deku Scrub mask show elder"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ デグナッツお面品評会長老 ☆☆☆☆☆ %x\n" VT_RST, self->stage);
    self->actor.flags &= ~1;
    self->actor.colChkInfo.mass = 0xFF;
    self->actor.targetMode = 6;
    self->actionFunc = EnDntJiji_SetFlower;
    self->actor.gravity = -2.0f;
}

void EnDntJiji_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDntJiji* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnDntJiji_SetFlower(EnDntJiji* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 1) {
        self->flowerPos = self->actor.world.pos;
        self->actionFunc = EnDntJiji_SetupWait;
    }
}

void EnDntJiji_SetupWait(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiBurrowAnim);
    Animation_Change(&self->skelAnime, &gDntJijiBurrowAnim, 0.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->skelAnime.curFrame = 8.0f;
    self->isSolid = self->action = DNT_LEADER_ACTION_NONE;
    self->actionFunc = EnDntJiji_Wait;
}

void EnDntJiji_Wait(EnDntJiji* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    if ((self->timer == 1) && (self->actor.xzDistToPlayer < 150.0f) && !Gameplay_InCsMode(globalCtx) &&
        !(player->stateFlags1 & 0x800)) {
        OnePointCutscene_Init(globalCtx, 2230, -99, &self->actor, MAIN_CAM);
        self->timer = 0;
        func_8002DF54(globalCtx, NULL, 8);
        self->actionFunc = EnDntJiji_SetupUnburrow;
    }
}

void EnDntJiji_SetupUp(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiUpAnim);
    Animation_Change(&self->skelAnime, &gDntJijiUpAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 6.0f, 0, 15, 5, 20, HAHEN_OBJECT_DEFAULT, 10, NULL);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
    self->actionFunc = EnDntJiji_Up;
}

void EnDntJiji_Up(EnDntJiji* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    if (self->actor.xzDistToPlayer < 150.0f) {
        self->actionFunc = EnDntJiji_SetupCower;
    }
}

void EnDntJiji_SetupUnburrow(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiUnburrowAnim);
    Animation_Change(&self->skelAnime, &gDntJijiUnburrowAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 6.0f, 0, 15, 5, 20, HAHEN_OBJECT_DEFAULT, 10, NULL);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
    self->actionFunc = EnDntJiji_Unburrow;
}

void EnDntJiji_Unburrow(EnDntJiji* self, GlobalContext* globalCtx) {
    f32 frame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if (self->endFrame <= frame) {
        if (self->action != DNT_LEADER_ACTION_ATTACK) {
            self->actionFunc = EnDntJiji_SetupWalk;
        } else {
            self->actionFunc = EnDntJiji_SetupReturn;
        }
    }
}

void EnDntJiji_SetupWalk(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiWalkAnim);
    Animation_Change(&self->skelAnime, &gDntJijiWalkAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->actor.speedXZ = 1.0f;
    self->isSolid = true;
    self->unburrow = true;
    self->actionFunc = EnDntJiji_Walk;
}

void EnDntJiji_Walk(EnDntJiji* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, 0x3E8, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    Math_ApproachF(&self->actor.speedXZ, 1.0f, 0.2f, 0.4f);
    if (self->sfxTimer == 0) {
        self->sfxTimer = 5;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }
    if ((self->actor.bgCheckFlags & 8) && (self->actor.bgCheckFlags & 1)) {
        self->actor.velocity.y = 9.0f;
        self->actor.speedXZ = 3.0f;
    }
    if (self->actor.xzDistToPlayer < 100.0f) {
        if (CUR_UPG_VALUE(UPG_STICKS) == 1) {
            self->getItemId = GI_STICK_UPGRADE_20;
        } else {
            self->getItemId = GI_STICK_UPGRADE_30;
        }
        self->actor.textId = 0x104D;
        func_8010B680(globalCtx, self->actor.textId, NULL);
        self->actor.speedXZ = 0.0f;
        self->unused = 5;
        self->actionFunc = EnDntJiji_Talk;
    }
}

void EnDntJiji_SetupBurrow(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiBurrowAnim);
    Animation_Change(&self->skelAnime, &gDntJijiBurrowAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 6.0f, 0, 15, 5, 20, HAHEN_OBJECT_DEFAULT, 10, NULL);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DOWN);
    self->actionFunc = EnDntJiji_Burrow;
}

void EnDntJiji_Burrow(EnDntJiji* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
}

void EnDntJiji_SetupCower(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiCowerAnim);
    Animation_Change(&self->skelAnime, &gDntJijiCowerAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
    EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 3.0f, 0, 9, 3, 10, HAHEN_OBJECT_DEFAULT, 10, NULL);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);

    if ((CUR_UPG_VALUE(UPG_NUTS) == 1) || (CUR_UPG_VALUE(UPG_NUTS) == 0)) {
        self->getItemId = GI_NUT_UPGRADE_30;
    } else {
        self->getItemId = GI_NUT_UPGRADE_40;
    }
    self->actor.flags |= 1;
    self->actor.textId = 0x10DB;
    self->unused = 5;
    self->actionFunc = EnDntJiji_Cower;
}

void EnDntJiji_Cower(EnDntJiji* self, GlobalContext* globalCtx) {
    f32 frame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    if (frame >= self->endFrame) {
        if (func_8002F194(&self->actor, globalCtx)) {
            self->actionFunc = EnDntJiji_SetupTalk;
        } else {
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
        }
    }
}

void EnDntJiji_SetupTalk(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiTalkAnim);
    Animation_Change(&self->skelAnime, &gDntJijiTalkAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = EnDntJiji_Talk;
}

void EnDntJiji_Talk(EnDntJiji* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        func_8005B1A4(GET_ACTIVE_CAM(globalCtx));
        func_80106CCC(globalCtx);
        func_8002DF54(globalCtx, NULL, 7);
        self->actor.parent = NULL;
        func_8002F434(&self->actor, globalCtx, self->getItemId, 400.0f, 200.0f);
        self->actionFunc = EnDntJiji_SetupGivePrize;
    }
}

void EnDntJiji_SetupGivePrize(EnDntJiji* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actionFunc = EnDntJiji_GivePrize;
    } else {
        func_8002F434(&self->actor, globalCtx, self->getItemId, 400.0f, 200.0f);
    }
}

void EnDntJiji_GivePrize(EnDntJiji* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && func_80106BC8(globalCtx)) {
        if ((self->getItemId == GI_NUT_UPGRADE_30) || (self->getItemId == GI_NUT_UPGRADE_40)) {
            // "nut"
            osSyncPrintf("実 \n");
            osSyncPrintf("実 \n");
            osSyncPrintf("実 \n");
            osSyncPrintf("実 \n");
            osSyncPrintf("実 \n");
            osSyncPrintf("実 \n");
            osSyncPrintf("実 \n");
            osSyncPrintf("実 \n");
            gSaveContext.itemGetInf[1] |= 0x8000;
        } else {
            // "stick"
            osSyncPrintf("棒 \n");
            osSyncPrintf("棒 \n");
            osSyncPrintf("棒 \n");
            osSyncPrintf("棒 \n");
            osSyncPrintf("棒 \n");
            osSyncPrintf("棒 \n");
            gSaveContext.itemGetInf[1] |= 0x4000;
        }
        self->actor.textId = 0;
        if ((self->stage != NULL) && (self->stage->actor.update != NULL)) {
            self->stage->action = DNT_ACTION_NONE;
            if (!self->unburrow) {
                self->stage->leaderSignal = DNT_SIGNAL_HIDE;
            } else {
                self->stage->leaderSignal = DNT_SIGNAL_RETURN;
            }
        }
        self->actor.flags &= ~1;
        if (!self->unburrow) {
            self->actionFunc = EnDntJiji_SetupHide;
        } else {
            self->actionFunc = EnDntJiji_SetupReturn;
        }
    }
}

void EnDntJiji_SetupHide(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiHideAnim);
    Animation_Change(&self->skelAnime, &gDntJijiHideAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
    self->actionFunc = EnDntJiji_Hide;
}

void EnDntJiji_Hide(EnDntJiji* self, GlobalContext* globalCtx) {
    f32 frame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if (self->endFrame <= frame) {
        self->actionFunc = EnDntJiji_SetupWait;
    }
}

void EnDntJiji_SetupReturn(EnDntJiji* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntJijiWalkAnim);
    Animation_Change(&self->skelAnime, &gDntJijiWalkAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->actor.speedXZ = 2.0f;
    self->isSolid = self->unburrow = true;
    self->actionFunc = EnDntJiji_Return;
}

void EnDntJiji_Return(EnDntJiji* self, GlobalContext* globalCtx) {
    f32 dx;
    f32 dz;

    SkelAnime_Update(&self->skelAnime);
    dx = self->flowerPos.x - self->actor.world.pos.x;
    dz = self->flowerPos.z - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(dx, dz) * (0x8000 / M_PI), 1, 0xBB8, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if ((self->actor.bgCheckFlags & 8) && (self->actor.bgCheckFlags & 1)) {
        self->actor.velocity.y = 9.0f;
        self->actor.speedXZ = 3.0f;
    }
    if (self->sfxTimer == 0) {
        self->sfxTimer = 3;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }
    if ((fabsf(dx) < 5.0f) && (fabsf(dz) < 5.0f)) {
        self->actor.world.pos.x = self->flowerPos.x;
        self->actor.world.pos.z = self->flowerPos.z;
        if (self->attackFlag) {
            if ((self->stage->actor.update != NULL) && (self->stage->leaderSignal == DNT_SIGNAL_NONE)) {
                self->stage->leaderSignal = DNT_SIGNAL_HIDE;
                self->stage->action = DNT_ACTION_ATTACK;
                Audio_QueueSeqCmd(0x81A);
            }
        }
        self->actor.speedXZ = 0.0f;
        self->isSolid = 0;
        self->actionFunc = EnDntJiji_SetupBurrow;
    }
}

void EnDntJiji_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDntJiji* self = THIS;

    Actor_SetScale(&self->actor, 0.015f);
    self->unkTimer++;
    if (BREG(0)) {
        // "time"
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 時間 ☆☆☆☆☆ %d\n" VT_RST, self->timer);
    }
    if ((self->timer > 1) && (self->timer != 0)) {
        self->timer--;
    }
    if (self->sfxTimer != 0) {
        self->sfxTimer--;
    }
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    }
    switch (self->stageSignal) {
        case DNT_LEADER_SIGNAL_UP:
            self->isSolid = true;
            self->action = DNT_LEADER_ACTION_UP;
            self->actionFunc = EnDntJiji_SetupUp;
            break;
        case DNT_LEADER_SIGNAL_BURROW:
            self->isSolid = false;
            self->action = DNT_LEADER_ACTION_NONE;
            self->actionFunc = EnDntJiji_SetupBurrow;
            break;
        case DNT_LEADER_SIGNAL_RETURN:
            self->actionFunc = EnDntJiji_SetupReturn;
            break;
        case DNT_LEADER_SIGNAL_NONE:
            break;
    }
    if (self->actor.textId != 0) {
        Actor_SetFocus(&self->actor, 30.0f);
    }
    if (self->stageSignal != DNT_LEADER_SIGNAL_NONE) {
        self->stageSignal = DNT_LEADER_SIGNAL_NONE;
    }
    if (self->blinkTimer == 0) {
        self->eyeState++;
        if (self->eyeState > 2) {
            self->eyeState = 0;
            self->blinkTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 60.0f, 0x1D);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    if (self->isSolid != 0) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void EnDntJiji_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* blinkTex[] = { &gDntJijiEyeOpenTex, &gDntJijiEyeHalfTex, &gDntJijiEyeShutTex };
    EnDntJiji* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dnt_jiji.c", 1019);
    func_80093D18(globalCtx->state.gfxCtx);
    Matrix_Push();
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(blinkTex[self->eyeState]));
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, NULL, self);
    Matrix_Pop();
    Matrix_Translate(self->flowerPos.x, self->flowerPos.y, self->flowerPos.z, MTXMODE_NEW);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dnt_jiji.c", 1040),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gDntJijiFlowerDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dnt_jiji.c", 1043);
}
