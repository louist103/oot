/*
 * File: z_en_heishi3.c
 * Overlay: ovl_En_Heishi3
 * Description: Hyrule Castle Guard
 */

#include "z_en_heishi3.h"
#include "objects/object_sd/object_sd.h"
#include "vt.h"

#define FLAGS 0x00000000

#define THIS ((EnHeishi3*)thisx)

void EnHeishi3_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi3_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi3_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi3_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnHeishi3_SetupGuardType(EnHeishi3* self, GlobalContext* globalCtx);
void EnHeishi3_StandSentinelInGrounds(EnHeishi3* self, GlobalContext* globalCtx);
void EnHeishi3_StandSentinelInCastle(EnHeishi3* self, GlobalContext* globalCtx);
void EnHeishi3_CatchStart(EnHeishi3* self, GlobalContext* globalCtx);
void EnHeishi3_ResetAnimationToIdle(EnHeishi3* self, GlobalContext* globalCtx);
void func_80A55D00(EnHeishi3* self, GlobalContext* globalCtx);
void func_80A55BD4(EnHeishi3* self, GlobalContext* globalCtx);

static s16 sPlayerCaught = 0;

const ActorInit En_Heishi3_InitVars = {
    ACTOR_EN_HEISHI3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi3),
    (ActorFunc)EnHeishi3_Init,
    (ActorFunc)EnHeishi3_Destroy,
    (ActorFunc)EnHeishi3_Update,
    (ActorFunc)EnHeishi3_Draw,
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
    { 15, 70, 0, { 0, 0, 0 } },
};

void EnHeishi3_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi3* self = THIS;

    sPlayerCaught = 0;
    if (self->actor.params <= 0) {
        self->unk_278 = 0;
    } else {
        self->unk_278 = 1;
        if (self->actor.world.pos.x < -290.0f) {
            self->unk_278 = 2;
        }
    }
    Actor_SetScale(&self->actor, 0.01f);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gEnHeishiSkel, &gEnHeishiIdleAnim, self->jointTable, self->morphTable,
                   17);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.targetMode = 6;
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    // "Castle Gate Soldier - Power Up"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 城門兵パワーアップ ☆☆☆☆☆ \n" VT_RST);

    self->actor.gravity = -3.0f;
    self->actor.focus.pos = self->actor.world.pos;
    self->actionFunc = EnHeishi3_SetupGuardType;
}

void EnHeishi3_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi3* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnHeishi3_SetupGuardType(EnHeishi3* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    if (self->unk_278 == 0) {
        self->actionFunc = EnHeishi3_StandSentinelInGrounds;
    } else {
        self->actionFunc = EnHeishi3_StandSentinelInCastle;
    }
}

/**
 * Handles the guards standing on Hyrule Castle Grounds.
 **/
void EnHeishi3_StandSentinelInGrounds(EnHeishi3* self, GlobalContext* globalCtx) {
    Player* player;
    s16 yawDiff;
    s16 yawDiffNew;
    f32 sightRange;

    player = GET_PLAYER(globalCtx);
    SkelAnime_Update(&self->skelAnime);
    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    yawDiffNew = ABS(yawDiff);
    if (yawDiffNew < 0x4300) {
        if (IS_DAY) {
            sightRange = 250.0f;
        } else {
            sightRange = 200.0f;
        }
    } else {
        if (IS_DAY) {
            sightRange = 150.0f;
        } else {
            sightRange = 100.0f;
        }
    }
    if ((self->actor.xzDistToPlayer < sightRange) &&
        (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 100.0f) && (sPlayerCaught == 0)) {
        sPlayerCaught = 1;
        func_8010B680(globalCtx, 0x702D, &self->actor);
        func_80078884(NA_SE_SY_FOUND);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発見！ ☆☆☆☆☆ \n" VT_RST); // "Discovered!"
        func_8002DF54(globalCtx, &self->actor, 1);
        self->actionFunc = EnHeishi3_CatchStart;
    }
}

/**
 * Handles the guards standing in front of Hyrule Castle.
 **/
void EnHeishi3_StandSentinelInCastle(EnHeishi3* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    if ((player->actor.world.pos.x < -190.0f) && (player->actor.world.pos.x > -380.0f) &&
        (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 100.0f) &&
        (player->actor.world.pos.z < 1020.0f) && (player->actor.world.pos.z > 700.0f) && (sPlayerCaught == 0)) {
        if (self->unk_278 == 1) {
            if ((player->actor.world.pos.x < -290.0f)) {
                return;
            }
        } else {
            if (player->actor.world.pos.x > -290.0f) {
                return;
            }
        }
        sPlayerCaught = 1;
        func_8010B680(globalCtx, 0x702D, &self->actor);
        func_80078884(NA_SE_SY_FOUND);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発見！ ☆☆☆☆☆ \n" VT_RST); // "Discovered!"
        func_8002DF54(globalCtx, &self->actor, 1);
        self->actionFunc = EnHeishi3_CatchStart;
    }
}

void EnHeishi3_CatchStart(EnHeishi3* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiWalkAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiWalkAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->caughtTimer = 20;
    self->actionFunc = func_80A55BD4;
    self->actor.speedXZ = 2.5f;
}

void func_80A55BD4(EnHeishi3* self, GlobalContext* globalCtx) {

    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 1.0f) || Animation_OnFrame(&self->skelAnime, 17.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_KNIGHT_WALK);
    }
    if (self->caughtTimer == 0) {
        self->actionFunc = EnHeishi3_ResetAnimationToIdle;
        self->actor.speedXZ = 0.0f;
    } else {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 5, 3000, 0);
    }
}

void EnHeishi3_ResetAnimationToIdle(EnHeishi3* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = func_80A55D00;
}

// This function initiates the respawn after the player gets caught.
void func_80A55D00(EnHeishi3* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0) && (self->respawnFlag == 0)) {
        gSaveContext.eventChkInf[4] |= 0x4000;
        globalCtx->nextEntranceIndex = 0x47E; // Hyrule Castle from Guard Capture (outside)
        globalCtx->sceneLoadFlag = 0x14;
        self->respawnFlag = 1;
        globalCtx->fadeTransition = 0x2E;
        gSaveContext.nextTransition = 0x2E;
    }
}

void EnHeishi3_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi3* self = THIS;
    s32 pad;

    Actor_SetFocus(&self->actor, 60.0f);
    self->unk_274 += 1;
    if (self->caughtTimer != 0) {
        self->caughtTimer -= 1;
    }
    self->actionFunc(self, globalCtx);
    self->actor.shape.rot = self->actor.world.rot;
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 50.0f, 0x1C);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

s32 EnHeishi3_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                               void* thisx) {
    EnHeishi3* self = THIS;

    if (limbIndex == 9) {
        rot->x += self->unk_26E;
    }

    if (limbIndex == 16) {
        rot->x += self->unk_262;
        rot->z += self->unk_264;
    }

    return false;
}

void EnHeishi3_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi3* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnHeishi3_OverrideLimbDraw, NULL,
                      self);
}
