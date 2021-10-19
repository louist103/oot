/*
 * File: z_boss_sst.c
 * Overlay: ovl_Boss_Sst
 * Description: Bongo Bongo
 */

#include "z_boss_sst.h"
#include "objects/object_sst/object_sst.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "overlays/actors/ovl_Bg_Sst_Floor/z_bg_sst_floor.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"

#define FLAGS 0x00000435

#define THIS ((BossSst*)thisx)

#define vParity actionVar
#define vVanish actionVar

#define LEFT 0
#define RIGHT 1
#define OTHER_HAND(hand) ((BossSst*)hand->actor.child)
#define HAND_STATE(hand) sHandState[hand->actor.params]

#define ROOM_CENTER_X -50.0f
#define ROOM_CENTER_Y 0.0f
#define ROOM_CENTER_Z 0.0f

typedef enum {
    /*  0 */ HAND_WAIT,
    /*  1 */ HAND_BEAT,
    /*  2 */ HAND_RETREAT,
    /*  3 */ HAND_SLAM,
    /*  4 */ HAND_SWEEP,
    /*  5 */ HAND_PUNCH,
    /*  6 */ HAND_CLAP,
    /*  7 */ HAND_GRAB,
    /*  8 */ HAND_DAMAGED,
    /*  9 */ HAND_FROZEN,
    /* 10 */ HAND_BREAK_ICE,
    /* 11 */ HAND_DEATH
} BossSstHandState;

typedef enum {
    /* 0 */ BONGO_NULL,
    /* 1 */ BONGO_ICE,
    /* 2 */ BONGO_SHOCKWAVE,
    /* 3 */ BONGO_SHADOW
} BossSstEffectMode;

void BossSst_Init(Actor* thisx, GlobalContext* globalCtx);
void BossSst_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BossSst_UpdateHand(Actor* thisx, GlobalContext* globalCtx);
void BossSst_UpdateHead(Actor* thisx, GlobalContext* globalCtx);
void BossSst_DrawHand(Actor* thisx, GlobalContext* globalCtx);
void BossSst_DrawHead(Actor* thisx, GlobalContext* globalCtx);
void BossSst_UpdateEffect(Actor* thisx, GlobalContext* globalCtx);
void BossSst_DrawEffect(Actor* thisx, GlobalContext* globalCtx);

void BossSst_HeadSfx(BossSst* self, u16 sfxId);

void BossSst_HeadSetupLurk(BossSst* self);
void BossSst_HeadLurk(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupIntro(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadIntro(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupNeutral(BossSst* self);
void BossSst_HeadNeutral(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadWait(BossSst* self, GlobalContext* globalCtx);

void BossSst_HeadSetupDamagedHand(BossSst* self, s32 bothHands);
void BossSst_HeadDamagedHand(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupReadyCharge(BossSst* self);
void BossSst_HeadReadyCharge(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupCharge(BossSst* self);
void BossSst_HeadCharge(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupEndCharge(BossSst* self);
void BossSst_HeadEndCharge(BossSst* self, GlobalContext* globalCtx);

void BossSst_HeadSetupFrozenHand(BossSst* self);
void BossSst_HeadFrozenHand(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupUnfreezeHand(BossSst* self);
void BossSst_HeadUnfreezeHand(BossSst* self, GlobalContext* globalCtx);

void BossSst_HeadStunned(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupVulnerable(BossSst* self);
void BossSst_HeadVulnerable(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadDamage(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupRecover(BossSst* self);
void BossSst_HeadRecover(BossSst* self, GlobalContext* globalCtx);

void BossSst_HeadDeath(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupThrash(BossSst* self);
void BossSst_HeadThrash(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupDarken(BossSst* self);
void BossSst_HeadDarken(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupFall(BossSst* self);
void BossSst_HeadFall(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupMelt(BossSst* self);
void BossSst_HeadMelt(BossSst* self, GlobalContext* globalCtx);
void BossSst_HeadSetupFinish(BossSst* self);
void BossSst_HeadFinish(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandGrabPlayer(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandReleasePlayer(BossSst* self, GlobalContext* globalCtx, s32 dropPlayer);
void BossSst_HandSelectAttack(BossSst* self);
void BossSst_HandSetDamage(BossSst* self, s32 damage);
void BossSst_HandSetInvulnerable(BossSst* self, s32 isInv);

void BossSst_HandSetupWait(BossSst* self);
void BossSst_HandWait(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupDownbeat(BossSst* self);
void BossSst_HandDownbeat(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupOffbeat(BossSst* self);
void BossSst_HandOffbeat(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupDownbeatEnd(BossSst* self);
void BossSst_HandDownbeatEnd(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupOffbeatEnd(BossSst* self);
void BossSst_HandOffbeatEnd(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandReadySlam(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupSlam(BossSst* self);
void BossSst_HandSlam(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandEndSlam(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandReadySweep(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupSweep(BossSst* self);
void BossSst_HandSweep(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandReadyPunch(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupPunch(BossSst* self);
void BossSst_HandPunch(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandReadyClap(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupClap(BossSst* self);
void BossSst_HandClap(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupEndClap(BossSst* self);
void BossSst_HandEndClap(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandReadyGrab(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupGrab(BossSst* self);
void BossSst_HandGrab(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupCrush(BossSst* self);
void BossSst_HandCrush(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupEndCrush(BossSst* self);
void BossSst_HandEndCrush(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupSwing(BossSst* self);
void BossSst_HandSwing(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandSetupRetreat(BossSst* self);
void BossSst_HandRetreat(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandSetupReel(BossSst* self);
void BossSst_HandReel(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupReadyShake(BossSst* self);
void BossSst_HandReadyShake(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupShake(BossSst* self);
void BossSst_HandShake(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupReadyCharge(BossSst* self);
void BossSst_HandReadyCharge(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandSetupFrozen(BossSst* self);
void BossSst_HandFrozen(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupReadyBreakIce(BossSst* self);
void BossSst_HandReadyBreakIce(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupBreakIce(BossSst* self);
void BossSst_HandBreakIce(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandStunned(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandDamage(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupRecover(BossSst* self);
void BossSst_HandRecover(BossSst* self, GlobalContext* globalCtx);

void BossSst_HandSetupThrash(BossSst* self);
void BossSst_HandThrash(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupDarken(BossSst* self);
void BossSst_HandDarken(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupFall(BossSst* self);
void BossSst_HandFall(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupMelt(BossSst* self);
void BossSst_HandMelt(BossSst* self, GlobalContext* globalCtx);
void BossSst_HandSetupFinish(BossSst* self);
void BossSst_HandFinish(BossSst* self, GlobalContext* globalCtx);

void BossSst_SpawnHeadShadow(BossSst* self);
void BossSst_SpawnHandShadow(BossSst* self);
void BossSst_SpawnShockwave(BossSst* self);
void BossSst_SpawnIceCrystal(BossSst* self, s32 index);
void BossSst_SpawnIceShard(BossSst* self);
void BossSst_IceShatter(BossSst* self);

#include "overlays/ovl_Boss_Sst/ovl_Boss_Sst.c"

static BossSst* sHead;
static BossSst* sHands[2];
static BgSstFloor* sFloor;

static Vec3f sRoomCenter = { ROOM_CENTER_X, ROOM_CENTER_Y, ROOM_CENTER_Z };
static Vec3f sHandOffsets[2];
static s16 sHandYawOffsets[2];

static s16 sCutsceneCamera;
static Vec3f sCameraAt = { ROOM_CENTER_X + 50.0f, ROOM_CENTER_Y + 0.0f, ROOM_CENTER_Z + 0.0f };
static Vec3f sCameraEye = { ROOM_CENTER_X + 150.0f, ROOM_CENTER_Y + 100.0f, ROOM_CENTER_Z + 0.0f };
static Vec3f sCameraAtVel = { 0.0f, 0.0f, 0.0f };
static Vec3f sCameraEyeVel = { 0.0f, 0.0f, 0.0f };

static Vec3f sCameraAtPoints[] = {
    { ROOM_CENTER_X - 50.0f, ROOM_CENTER_Y + 300.0f, ROOM_CENTER_Z + 0.0f },
    { ROOM_CENTER_X + 150.0f, ROOM_CENTER_Y + 300.0f, ROOM_CENTER_Z + 100.0f },
    { ROOM_CENTER_X + 0.0f, ROOM_CENTER_Y + 600.0f, ROOM_CENTER_Z + 100.0f },
    { ROOM_CENTER_X + 50.0f, ROOM_CENTER_Y + 400.0f, ROOM_CENTER_Z + 200.0f },
    { ROOM_CENTER_X + 50.0f, ROOM_CENTER_Y + 200.0f, ROOM_CENTER_Z + 200.0f },
    { ROOM_CENTER_X - 50.0f, ROOM_CENTER_Y + 0.0f, ROOM_CENTER_Z + 200.0f },
    { ROOM_CENTER_X - 150.0f, ROOM_CENTER_Y + 0.0f, ROOM_CENTER_Z + 100.0f },
    { ROOM_CENTER_X - 60.0f, ROOM_CENTER_Y + 180.0f, ROOM_CENTER_Z + 730.0f },
};

static Vec3f sCameraEyePoints[] = {
    { ROOM_CENTER_X + 250.0f, ROOM_CENTER_Y + 800.0f, ROOM_CENTER_Z + 800.0f },
    { ROOM_CENTER_X - 150.0f, ROOM_CENTER_Y + 700.0f, ROOM_CENTER_Z + 1400.0f },
    { ROOM_CENTER_X + 250.0f, ROOM_CENTER_Y + 100.0f, ROOM_CENTER_Z + 750.0f },
    { ROOM_CENTER_X + 50.0f, ROOM_CENTER_Y + 200.0f, ROOM_CENTER_Z + 900.0f },
    { ROOM_CENTER_X + 50.0f, ROOM_CENTER_Y + 200.0f, ROOM_CENTER_Z + 900.0f },
    { ROOM_CENTER_X + 350.0f, ROOM_CENTER_Y + 400.0f, ROOM_CENTER_Z + 1200.0f },
    { ROOM_CENTER_X - 50.0f, ROOM_CENTER_Y + 200.0f, ROOM_CENTER_Z + 800.0f },
    { ROOM_CENTER_X - 50.0f, ROOM_CENTER_Y + 200.0f, ROOM_CENTER_Z + 800.0f },
};

static Vec3f sZeroVec = { 0.0f, 0.0f, 0.0f };
static u32 sBodyStatic = false;

// Unreferenced. Maybe two zero vectors?
static u32 sUnkValues[] = { 0, 0, 0, 0, 0, 0 };

static Color_RGBA8 sBodyColor = { 255, 255, 255, 255 };
static Color_RGBA8 sStaticColor = { 0, 0, 0, 255 };
static s32 sHandState[] = { HAND_WAIT, HAND_WAIT };

const ActorInit Boss_Sst_InitVars = {
    ACTOR_BOSS_SST,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_SST,
    sizeof(BossSst),
    (ActorFunc)BossSst_Init,
    (ActorFunc)BossSst_Destroy,
    (ActorFunc)BossSst_UpdateHand,
    (ActorFunc)BossSst_DrawHand,
};

#include "z_boss_sst_colchk.c"

static AnimationHeader* sHandIdleAnims[] = { &gBongoLeftHandIdleAnim, &gBongoRightHandIdleAnim };
static AnimationHeader* sHandFlatPoses[] = { &gBongoLeftHandFlatPoseAnim, &gBongoRightHandFlatPoseAnim };
static AnimationHeader* sHandOpenPoses[] = { &gBongoLeftHandOpenPoseAnim, &gBongoRightHandOpenPoseAnim };
static AnimationHeader* sHandFistPoses[] = { &gBongoLeftHandFistPoseAnim, &gBongoRightHandFistPoseAnim };
static AnimationHeader* sHandClenchAnims[] = { &gBongoLeftHandClenchAnim, &gBongoRightHandClenchAnim };
static AnimationHeader* sHandDamagePoses[] = { &gBongoLeftHandDamagePoseAnim, &gBongoRightHandDamagePoseAnim };
static AnimationHeader* sHandPushoffPoses[] = { &gBongoLeftHandPushoffPoseAnim, &gBongoRightHandPushoffPoseAnim };
static AnimationHeader* sHandHangPoses[] = { &gBongoLeftHandHangPoseAnim, &gBongoRightHandHangPoseAnim };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x29, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 20, ICHAIN_STOP),
};

void BossSst_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BossSst* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Collider_InitCylinder(globalCtx, &self->colliderCyl);
    Collider_InitJntSph(globalCtx, &self->colliderJntSph);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    Flags_SetSwitch(globalCtx, 0x14);
    if (self->actor.params == BONGO_HEAD) {
        sFloor = (BgSstFloor*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_BG_SST_FLOOR, sRoomCenter.x,
                                          sRoomCenter.y, sRoomCenter.z, 0, 0, 0, BONGOFLOOR_REST);
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gBongoHeadSkel, &gBongoHeadEyeOpenIdleAnim, self->jointTable,
                           self->morphTable, 45);
        ActorShape_Init(&self->actor.shape, 70000.0f, ActorShadow_DrawCircle, 95.0f);
        Collider_SetJntSph(globalCtx, &self->colliderJntSph, &self->actor, &sJntSphInitHead, self->colliderItems);
        Collider_SetCylinder(globalCtx, &self->colliderCyl, &self->actor, &sCylinderInitHead);
        sHead = self;
        self->actor.world.pos.x = ROOM_CENTER_X + 50.0f;
        self->actor.world.pos.y = ROOM_CENTER_Y + 0.0f;
        self->actor.world.pos.z = ROOM_CENTER_Z - 650.0f;
        self->actor.home.pos = self->actor.world.pos;
        self->actor.shape.rot.y = 0;
        if (Flags_GetClear(globalCtx, globalCtx->roomCtx.curRoom.num)) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_WARP1, ROOM_CENTER_X, ROOM_CENTER_Y,
                        ROOM_CENTER_Z + 400.0f, 0, 0, 0, WARP_DUNGEON_ADULT);
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, ROOM_CENTER_X, ROOM_CENTER_Y,
                        ROOM_CENTER_Z - 200.0f, 0, 0, 0, 0);
            Actor_Kill(&self->actor);
        } else {
            sHands[LEFT] =
                (BossSst*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_BOSS_SST, self->actor.world.pos.x + 200.0f,
                                      self->actor.world.pos.y, self->actor.world.pos.z + 400.0f, 0,
                                      self->actor.shape.rot.y, 0, BONGO_LEFT_HAND);
            sHands[RIGHT] = (BossSst*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_BOSS_SST,
                                                  self->actor.world.pos.x + (-200.0f), self->actor.world.pos.y,
                                                  self->actor.world.pos.z + 400.0f, 0, self->actor.shape.rot.y, 0,
                                                  BONGO_RIGHT_HAND);
            sHands[LEFT]->actor.child = &sHands[RIGHT]->actor;
            sHands[RIGHT]->actor.child = &sHands[LEFT]->actor;

            self->actor.flags &= ~1;
            self->actor.update = BossSst_UpdateHead;
            self->actor.draw = BossSst_DrawHead;
            self->radius = -650.0f;
            self->actor.targetArrowOffset = 4000.0f;
            BossSst_HeadSetupLurk(self);
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_BOSS);
        }
    } else {
        Collider_SetJntSph(globalCtx, &self->colliderJntSph, &self->actor, &sJntSphInitHand, self->colliderItems);
        Collider_SetCylinder(globalCtx, &self->colliderCyl, &self->actor, &sCylinderInitHand);
        if (self->actor.params == BONGO_LEFT_HAND) {
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gBongoLeftHandSkel, &gBongoLeftHandIdleAnim,
                               self->jointTable, self->morphTable, 27);
            self->vParity = -1;
            self->colliderJntSph.elements[0].dim.modelSphere.center.z *= -1;
        } else {
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gBongoRightHandSkel, &gBongoRightHandIdleAnim,
                               self->jointTable, self->morphTable, 27);
            self->vParity = 1;
        }

        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 95.0f);
        self->handZPosMod = -3500;
        self->actor.targetArrowOffset = 5000.0f;
        self->actor.flags &= ~1;
        BossSst_HandSetupWait(self);
    }
}

void BossSst_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossSst* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->colliderJntSph);
    Collider_DestroyCylinder(globalCtx, &self->colliderCyl);
    func_800F89E8(&self->center);
}

void BossSst_HeadSetupLurk(BossSst* self) {
    self->actor.draw = NULL;
    sHands[LEFT]->actor.draw = NULL;
    sHands[RIGHT]->actor.draw = NULL;
    self->vVanish = false;
    self->actionFunc = BossSst_HeadLurk;
}

void BossSst_HeadLurk(BossSst* self, GlobalContext* globalCtx) {
    if (self->actor.yDistToPlayer < 1000.0f) {
        BossSst_HeadSetupIntro(self, globalCtx);
    }
}

void BossSst_HeadSetupIntro(BossSst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->timer = 611;
    self->ready = false;
    player->actor.world.pos.x = sRoomCenter.x;
    player->actor.world.pos.y = ROOM_CENTER_Y + 1000.0f;
    player->actor.world.pos.z = sRoomCenter.z;
    player->linearVelocity = player->actor.velocity.y = 0.0f;
    player->actor.shape.rot.y = -0x8000;
    player->targetYaw = -0x8000;
    player->currentYaw = -0x8000;
    player->fallStartHeight = 0;
    player->stateFlags1 |= 0x20;

    func_80064520(globalCtx, &globalCtx->csCtx);
    func_8002DF54(globalCtx, &self->actor, 8);
    sCutsceneCamera = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, sCutsceneCamera, CAM_STAT_ACTIVE);
    Math_Vec3f_Copy(&sCameraAt, &player->actor.world.pos);
    if (gSaveContext.eventChkInf[7] & 0x80) {
        sCameraEye.z = ROOM_CENTER_Z - 100.0f;
    }

    Gameplay_CameraSetAtEye(globalCtx, sCutsceneCamera, &sCameraAt, &sCameraEye);
    Audio_QueueSeqCmd(0x100100FF);
    self->actionFunc = BossSst_HeadIntro;
}

void BossSst_HeadIntro(BossSst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 tempo;
    s32 introStateTimer;
    s32 revealStateTimer;

    if (self->timer != 0) {
        self->timer--;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        Animation_MorphToLoop(&self->skelAnime, &gBongoHeadEyeCloseIdleAnim, -3.0f);
    }

    if (self->timer == 0) {
        sHands[RIGHT]->actor.flags |= 1;
        sHands[LEFT]->actor.flags |= 1;
        player->stateFlags1 &= ~0x20;
        func_80064534(globalCtx, &globalCtx->csCtx);
        func_8002DF54(globalCtx, &self->actor, 7);
        sCameraAt.y += 30.0f;
        sCameraAt.z += 300.0f;
        Gameplay_CameraSetAtEye(globalCtx, sCutsceneCamera, &sCameraAt, &sCameraEye);
        Gameplay_CopyCamera(globalCtx, MAIN_CAM, sCutsceneCamera);
        Gameplay_ChangeCameraStatus(globalCtx, sCutsceneCamera, CAM_STAT_WAIT);
        Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
        Gameplay_ClearCamera(globalCtx, sCutsceneCamera);
        gSaveContext.eventChkInf[7] |= 0x80;
        BossSst_HeadSetupNeutral(self);
        self->colliderJntSph.base.ocFlags1 |= OC1_ON;
        sHands[LEFT]->colliderJntSph.base.ocFlags1 |= OC1_ON;
        sHands[RIGHT]->colliderJntSph.base.ocFlags1 |= OC1_ON;
        self->timer = 112;
    } else if (self->timer >= 546) {
        if (player->actor.world.pos.y > 100.0f) {
            player->actor.world.pos.x = sRoomCenter.x;
            player->actor.world.pos.z = sRoomCenter.z;
            player->linearVelocity = 0;
            player->actor.shape.rot.y = -0x8000;
            player->targetYaw = -0x8000;
            player->currentYaw = -0x8000;
        }

        Math_Vec3f_Copy(&sCameraAt, &player->actor.world.pos);
        if (player->actor.bgCheckFlags & 2) {
            if (!self->ready) {
                sFloor->dyna.actor.params = BONGOFLOOR_HIT;
                self->ready = true;
                func_800AA000(self->actor.xyzDistToPlayerSq, 0xFF, 0x14, 0x96);
                Audio_PlayActorSound2(&sFloor->dyna.actor, NA_SE_EN_SHADEST_TAIKO_HIGH);
            } else if (gSaveContext.eventChkInf[7] & 0x80) {
                sHands[RIGHT]->actor.draw = BossSst_DrawHand;
                sHands[LEFT]->actor.draw = BossSst_DrawHand;
                self->actor.draw = BossSst_DrawHead;
                self->timer = 178;
                sCameraAt.x = ROOM_CENTER_X - 23.0f;
                sCameraAt.y = ROOM_CENTER_Y + 0.0f;
                sCameraAt.z = ROOM_CENTER_Z + 0.0f;
            } else {
                self->timer = 546;
            }
        }
    } else if (self->timer >= 478) {
        sCameraEye.x += 10.0f;
        sCameraEye.y += 10.0f;
        sCameraEye.z -= 10.0f;
    } else if (self->timer >= 448) {
        if (self->timer == 460) {
            sHands[RIGHT]->actor.draw = BossSst_DrawHand;
            sHands[LEFT]->actor.draw = BossSst_DrawHand;
            self->actor.draw = BossSst_DrawHead;
            player->actor.world.pos.x = sRoomCenter.x;
            player->actor.world.pos.z = sRoomCenter.z;
            BossSst_HandSetupDownbeat(sHands[RIGHT]);
        }
        if (self->timer > 460) {
            sCameraEye.x -= 40.0f;
            sCameraEye.y -= 40.0f;
            sCameraEye.z += 20.0f;
        } else if (self->timer == 460) {
            sCameraAt.x = sHands[RIGHT]->actor.home.pos.x + 0.0f;
            sCameraAt.y = sHands[RIGHT]->actor.home.pos.y - 20.0f;
            sCameraAt.z = sHands[RIGHT]->actor.home.pos.z + 10.0f;
            sCameraEye.x = sHands[RIGHT]->actor.home.pos.x + 150.0f;
            sCameraEye.y = sHands[RIGHT]->actor.home.pos.y + 100.0f;
            sCameraEye.z = sHands[RIGHT]->actor.home.pos.z + 80.0f;
        }
    } else {
        if (self->timer >= 372) {
            introStateTimer = self->timer - 372;
            tempo = 6;
            if (self->timer == 447) {
                sCameraAt = player->actor.world.pos;
                sCameraEye.x = ROOM_CENTER_X - 200.0f;
                sCameraEye.y = ROOM_CENTER_Y + 160.0f;
                sCameraEye.z = ROOM_CENTER_Z - 190.0f;
            } else if (introStateTimer == 11) {
                sCameraAt.x = sHands[RIGHT]->actor.home.pos.x + 30.0f;
                sCameraAt.y = sHands[RIGHT]->actor.home.pos.y + 0.0f;
                sCameraAt.z = sHands[RIGHT]->actor.home.pos.z + 20.0f;
                sCameraEye.x = sHands[RIGHT]->actor.home.pos.x + 100.0f;
                sCameraEye.y = sHands[RIGHT]->actor.home.pos.y + 10.0f;
                sCameraEye.z = sHands[RIGHT]->actor.home.pos.z - 210.0f;
            } else if (introStateTimer == 62) {
                sCameraAt.x = sHands[LEFT]->actor.home.pos.x + 0.0f;
                sCameraAt.y = sHands[LEFT]->actor.home.pos.y + 50.0f;
                sCameraAt.z = sHands[LEFT]->actor.home.pos.z + 100.0f;
                sCameraEye.x = sHands[LEFT]->actor.home.pos.x + 110.0f;
                sCameraEye.y = sHands[LEFT]->actor.home.pos.y + 180.0f;
                sCameraEye.z = sHands[LEFT]->actor.home.pos.z - 70.0f;
            }
        } else if (self->timer >= 304) {
            introStateTimer = self->timer - 304;
            tempo = 5;
            if (introStateTimer == 11) {
                sCameraAt.x = sHands[RIGHT]->actor.home.pos.x + 40.0f;
                sCameraAt.y = sHands[RIGHT]->actor.home.pos.y - 90.0f;
                sCameraAt.z = sHands[RIGHT]->actor.home.pos.z - 40.0f;
                sCameraEye.x = sHands[RIGHT]->actor.home.pos.x - 20.0f;
                sCameraEye.y = sHands[RIGHT]->actor.home.pos.y + 210.0f;
                sCameraEye.z = sHands[RIGHT]->actor.home.pos.z + 170.0f;
            } else if (self->timer == 368) {
                sCameraAt.x = sHands[LEFT]->actor.home.pos.x - 20.0f;
                sCameraAt.y = sHands[LEFT]->actor.home.pos.y + 0.0f;
                sCameraAt.z = sHands[LEFT]->actor.home.pos.z + 0.0f;
                sCameraEye.x = sHands[LEFT]->actor.home.pos.x - 70.0f;
                sCameraEye.y = sHands[LEFT]->actor.home.pos.y + 170.0f;
                sCameraEye.z = sHands[LEFT]->actor.home.pos.z + 150.0f;
            }
        } else if (self->timer >= 244) {
            introStateTimer = self->timer - 244;
            tempo = 4;
            if (introStateTimer == 11) {
                sCameraAt.x = sHands[RIGHT]->actor.home.pos.x + 30.0f;
                sCameraAt.y = sHands[RIGHT]->actor.home.pos.y + 70.0f;
                sCameraAt.z = sHands[RIGHT]->actor.home.pos.z + 40.0f;
                sCameraEye.x = sHands[RIGHT]->actor.home.pos.x + 110.0f;
                sCameraEye.y = sHands[RIGHT]->actor.home.pos.y - 140.0f;
                sCameraEye.z = sHands[RIGHT]->actor.home.pos.z - 10.0f;
            } else if (self->timer == 300) {
                sCameraAt.x = sHands[LEFT]->actor.home.pos.x - 20.0f;
                sCameraAt.y = sHands[LEFT]->actor.home.pos.y - 80.0f;
                sCameraAt.z = sHands[LEFT]->actor.home.pos.z + 320.0f;
                sCameraEye.x = sHands[LEFT]->actor.home.pos.x - 130.0f;
                sCameraEye.y = sHands[LEFT]->actor.home.pos.y + 130.0f;
                sCameraEye.z = sHands[LEFT]->actor.home.pos.z - 150.0f;
            }
        } else if (self->timer >= 192) {
            introStateTimer = self->timer - 192;
            tempo = 3;
            if (self->timer == 240) {
                sCameraAt.x = sHands[LEFT]->actor.home.pos.x - 190.0f;
                sCameraAt.y = sHands[LEFT]->actor.home.pos.y - 110.0f;
                sCameraAt.z = sHands[LEFT]->actor.home.pos.z + 40.0f;
                sCameraEye.x = sHands[LEFT]->actor.home.pos.x + 120.0f;
                sCameraEye.y = sHands[LEFT]->actor.home.pos.y + 130.0f;
                sCameraEye.z = sHands[LEFT]->actor.home.pos.z + 50.0f;
            } else if (introStateTimer == 12) {
                sCameraAt.x = sRoomCenter.x + 50.0f;
                sCameraAt.y = sRoomCenter.y - 90.0f;
                sCameraAt.z = sRoomCenter.z - 200.0f;
                sCameraEye.x = sRoomCenter.x + 50.0f;
                sCameraEye.y = sRoomCenter.y + 350.0f;
                sCameraEye.z = sRoomCenter.z + 150.0f;
            }
        } else if (self->timer >= 148) {
            introStateTimer = self->timer - 148;
            tempo = 2;
        } else if (self->timer >= 112) {
            introStateTimer = self->timer - 112;
            tempo = 1;
        } else {
            introStateTimer = self->timer % 28;
            tempo = 0;
        }
        if (self->timer <= 198) {
            revealStateTimer = 198 - self->timer;
            if ((gSaveContext.eventChkInf[7] & 0x80) && (revealStateTimer <= 44)) {
                sCameraAt.x += 492.0f * 0.01f;
                sCameraAt.y += 200.0f * 0.01f;
                sCameraEye.x -= 80.0f * 0.01f;
                sCameraEye.y -= 360.0f * 0.01f;
                sCameraEye.z += 1000.0f * 0.01f;
            } else if (self->timer <= 20) {
                sCameraAt.y -= 700.0f * 0.01f;
                sCameraAt.z += 900.0f * 0.01f;
                sCameraEye.x += 650.0f * 0.01f;
                sCameraEye.y += 400.0f * 0.01f;
                sCameraEye.z += 1550.0f * 0.01f;
                self->vVanish = true;
                self->actor.flags |= 0x80;
            } else if (revealStateTimer < 40) {
                sCameraAt.x += 125.0f * 0.01f;
                sCameraAt.y += 350.0f * 0.01f;
                sCameraAt.z += 500.0f * 0.01f;
                sCameraEye.x += 200.0f * 0.01f;
                sCameraEye.y -= 850.0f * 0.01f;
            } else if (revealStateTimer >= 45) {
                if (revealStateTimer < 85) {
                    sCameraAt.x -= 250.0f * 0.01f;
                    sCameraAt.y += 425.0f * 0.01f;
                    sCameraAt.z -= 1200.0f * 0.01f;
                    sCameraEye.x -= 650.0f * 0.01f;
                    sCameraEye.y += 125.0f * 0.01f;
                    sCameraEye.z -= 350.0f * 0.01f;
                } else if (revealStateTimer == 85) {
                    if (!(gSaveContext.eventChkInf[7] & 0x80)) {
                        TitleCard_InitBossName(globalCtx, &globalCtx->actorCtx.titleCtx,
                                               SEGMENTED_TO_VIRTUAL(&gBongoTitleCardTex), 160, 180, 128, 40);
                    }
                    Audio_QueueSeqCmd(0x1B);
                    Animation_MorphToPlayOnce(&self->skelAnime, &gBongoHeadEyeCloseAnim, -5.0f);
                    BossSst_HeadSfx(self, NA_SE_EN_SHADEST_DISAPPEAR);
                }
            }
        }
        if (introStateTimer == 12) {
            BossSst_HandSetupDownbeat(sHands[RIGHT]);
        }
        if ((introStateTimer != 5) && ((introStateTimer % ((tempo * 2) + 7)) == 5)) {
            BossSst_HandSetupOffbeat(sHands[LEFT]);
        }
    }

    if (self->actionFunc != BossSst_HeadNeutral) {
        Gameplay_CameraSetAtEye(globalCtx, sCutsceneCamera, &sCameraAt, &sCameraEye);
    }
}

void BossSst_HeadSetupWait(BossSst* self) {
    if (self->skelAnime.animation != &gBongoHeadEyeCloseIdleAnim) {
        Animation_MorphToLoop(&self->skelAnime, &gBongoHeadEyeCloseIdleAnim, -5.0f);
    }
    self->actionFunc = BossSst_HeadWait;
}

void BossSst_HeadWait(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((HAND_STATE(sHands[LEFT]) == HAND_WAIT) && (HAND_STATE(sHands[RIGHT]) == HAND_WAIT)) {
        BossSst_HeadSetupNeutral(self);
    }
}

void BossSst_HeadSetupNeutral(BossSst* self) {
    self->timer = 127;
    self->ready = false;
    self->actionFunc = BossSst_HeadNeutral;
}

void BossSst_HeadNeutral(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (!self->ready && ((HAND_STATE(sHands[LEFT]) == HAND_BEAT) || (HAND_STATE(sHands[LEFT]) == HAND_WAIT)) &&
        ((HAND_STATE(sHands[RIGHT]) == HAND_BEAT) || (HAND_STATE(sHands[RIGHT]) == HAND_WAIT))) {
        self->ready = true;
    }

    if (self->ready) {
        if (self->timer != 0) {
            self->timer--;
        }
    }

    if (self->timer == 0) {
        if ((GET_PLAYER(globalCtx)->actor.world.pos.y > -50.0f) && !(GET_PLAYER(globalCtx)->stateFlags1 & 0x6080)) {
            sHands[Rand_ZeroOne() <= 0.5f]->ready = true;
            BossSst_HeadSetupWait(self);
        } else {
            self->timer = 28;
        }
    } else {
        Math_ApproachS(&self->actor.shape.rot.y,
                       Actor_WorldYawTowardPoint(&GET_PLAYER(globalCtx)->actor, &sRoomCenter) + 0x8000, 4, 0x400);
        if ((self->timer == 28) || (self->timer == 84)) {
            BossSst_HeadSfx(self, NA_SE_EN_SHADEST_PRAY);
        }
    }
}

void BossSst_HeadSetupDamagedHand(BossSst* self, s32 bothHands) {
    if (bothHands) {
        Animation_MorphToPlayOnce(&self->skelAnime, &gBongoHeadEyeOpenAnim, -5.0f);
    } else {
        Animation_MorphToPlayOnce(&self->skelAnime, &gBongoHeadDamagedHandAnim, -5.0f);
    }
    self->actionFunc = BossSst_HeadDamagedHand;
}

void BossSst_HeadDamagedHand(BossSst* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        if ((HAND_STATE(sHands[LEFT]) == HAND_DAMAGED) && (HAND_STATE(sHands[RIGHT]) == HAND_DAMAGED)) {
            BossSst_HeadSetupReadyCharge(self);
        } else if ((HAND_STATE(sHands[LEFT]) == HAND_FROZEN) || (HAND_STATE(sHands[RIGHT]) == HAND_FROZEN)) {
            BossSst_HeadSetupFrozenHand(self);
        } else if (self->skelAnime.animation == &gBongoHeadEyeOpenAnim) {
            BossSst_HeadSetupUnfreezeHand(self);
        } else {
            BossSst_HeadSetupWait(self);
        }
    }
}

void BossSst_HeadSetupReadyCharge(BossSst* self) {
    Animation_MorphToLoop(&self->skelAnime, &gBongoHeadEyeOpenIdleAnim, -5.0f);
    self->actor.speedXZ = 0.0f;
    self->colliderCyl.base.acFlags |= AC_ON;
    self->actionFunc = BossSst_HeadReadyCharge;
}

void BossSst_HeadReadyCharge(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (sHands[LEFT]->ready && (sHands[LEFT]->actionFunc == BossSst_HandReadyCharge) && sHands[RIGHT]->ready &&
        (sHands[RIGHT]->actionFunc == BossSst_HandReadyCharge)) {
        BossSst_HeadSetupCharge(self);
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 4, 0x800, 0x400);
    }
}

void BossSst_HeadSetupCharge(BossSst* self) {
    Animation_Change(&self->skelAnime, &gBongoHeadChargeAnim, 0.5f, 0.0f, Animation_GetLastFrame(&gBongoHeadChargeAnim),
                     ANIMMODE_ONCE_INTERP, -5.0f);
    BossSst_HandSetDamage(sHands[LEFT], 0x20);
    BossSst_HandSetDamage(sHands[RIGHT], 0x20);
    self->colliderJntSph.base.atFlags |= AT_ON;
    self->actor.speedXZ = 3.0f;
    self->radius = -650.0f;
    self->ready = false;
    self->actionFunc = BossSst_HeadCharge;
}

void BossSst_HeadCharge(BossSst* self, GlobalContext* globalCtx) {
    f32 chargeDist;
    s32 animFinish = SkelAnime_Update(&self->skelAnime);

    if (!self->ready && Animation_OnFrame(&self->skelAnime, 6.0f)) {
        self->ready = true;
        self->actor.speedXZ = 0.25f;
        self->skelAnime.playSpeed = 0.2f;
    }

    self->actor.speedXZ *= 1.25f;
    self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 45.0f);

    if (self->ready) {
        if (Math_SmoothStepToF(&self->radius, 650.0f, 0.4f, self->actor.speedXZ, 1.0f) < 10.0f) {
            self->radius = 650.0f;
            BossSst_HeadSetupEndCharge(self);
        } else {
            chargeDist = (650.0f - self->radius) * 3.0f;
            if (chargeDist > 180.0f) {
                chargeDist = 180.0f;
            }

            self->actor.world.pos.y = self->actor.home.pos.y - chargeDist;
        }

        if (!animFinish) {
            sHandOffsets[LEFT].z += 5.0f;
            sHandOffsets[RIGHT].z += 5.0f;
        }
    } else {
        Math_ApproachF(&self->radius, -700.0f, 0.4f, self->actor.speedXZ);
        Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - 180.0f, 20.0f);
        sHandOffsets[LEFT].y += 5.0f;
        sHandOffsets[RIGHT].y += 5.0f;
    }

    if (self->colliderJntSph.base.atFlags & AT_HIT) {
        self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        sHands[LEFT]->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        sHands[RIGHT]->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        func_8002F71C(globalCtx, &self->actor, 10.0f, self->actor.shape.rot.y, 5.0f);
        func_8002F7DC(&GET_PLAYER(globalCtx)->actor, NA_SE_PL_BODY_HIT);
    }
}

void BossSst_HeadSetupEndCharge(BossSst* self) {
    Animation_MorphToLoop(&self->skelAnime, &gBongoHeadEyeCloseIdleAnim, -20.0f);
    self->targetYaw = Actor_WorldYawTowardPoint(&self->actor, &sRoomCenter);
    self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
    self->colliderCyl.base.acFlags &= ~AC_ON;
    self->radius *= -1.0f;
    self->actionFunc = BossSst_HeadEndCharge;
}

void BossSst_HeadEndCharge(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Math_SmoothStepToS(&self->actor.shape.rot.y, self->targetYaw, 4, 0x800, 0x100) == 0) {
        BossSst_HandSetupRetreat(sHands[LEFT]);
        BossSst_HandSetupRetreat(sHands[RIGHT]);
        BossSst_HeadSetupNeutral(self);
    }
}

void BossSst_HeadSetupFrozenHand(BossSst* self) {
    Animation_MorphToLoop(&self->skelAnime, &gBongoHeadEyeOpenIdleAnim, -5.0f);
    self->ready = false;
    self->colliderCyl.base.acFlags |= AC_ON;
    self->actionFunc = BossSst_HeadFrozenHand;
}

void BossSst_HeadFrozenHand(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->ready) {
        BossSst_HeadSetupUnfreezeHand(self);
    }
}

void BossSst_HeadSetupUnfreezeHand(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBongoHeadEyeCloseAnim, -5.0f);
    self->colliderCyl.base.acFlags &= ~AC_ON;
    self->actionFunc = BossSst_HeadUnfreezeHand;
}

void BossSst_HeadUnfreezeHand(BossSst* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        BossSst_HeadSetupWait(self);
    }
}

void BossSst_HeadSetupStunned(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBongoHeadKnockoutAnim, -5.0f);
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, Animation_GetLastFrame(&gBongoHeadKnockoutAnim));
    self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
    self->colliderCyl.base.acFlags &= ~AC_ON;
    self->vVanish = false;
    self->actor.flags &= ~0x80;
    BossSst_HeadSfx(self, NA_SE_EN_SHADEST_FREEZE);
    self->actionFunc = BossSst_HeadStunned;
}

void BossSst_HeadStunned(BossSst* self, GlobalContext* globalCtx) {
    f32 bounce;
    s32 animFinish;
    f32 currentFrame;

    Math_StepToF(&sHandOffsets[LEFT].z, 600.0f, 20.0f);
    Math_StepToF(&sHandOffsets[RIGHT].z, 600.0f, 20.0f);
    Math_StepToF(&sHandOffsets[LEFT].x, 200.0f, 20.0f);
    Math_StepToF(&sHandOffsets[RIGHT].x, -200.0f, 20.0f);
    self->actor.velocity.y += self->actor.gravity;
    animFinish = SkelAnime_Update(&self->skelAnime);
    currentFrame = self->skelAnime.curFrame;
    if (currentFrame <= 6.0f) {
        bounce = (sinf((M_PI / 11) * currentFrame) * 100.0f) + (self->actor.home.pos.y - 180.0f);
        if (self->actor.world.pos.y < bounce) {
            self->actor.world.pos.y = bounce;
        }
    } else if (currentFrame <= 11.0f) {
        self->actor.world.pos.y = (sinf((M_PI / 11) * currentFrame) * 170.0f) + (self->actor.home.pos.y - 250.0f);
    } else {
        self->actor.world.pos.y =
            (sinf((currentFrame - 11.0f) * (M_PI / 5)) * 50.0f) + (self->actor.home.pos.y - 250.0f);
    }

    if ((animFinish) || Animation_OnFrame(&self->skelAnime, 11.0f)) {
        BossSst_HeadSfx(self, NA_SE_EN_SHADEST_LAND);
    }

    if (self->radius < -500.0f) {
        Math_SmoothStepToF(&self->radius, -500.0f, 1.0f, 50.0f, 5.0f);
    } else {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.5f, 15.0f, 3.0f);
        self->radius += self->actor.speedXZ;
    }

    self->radius = CLAMP_MAX(self->radius, 400.0f);

    self->actor.world.pos.y += self->actor.velocity.y;
    if (animFinish) {
        BossSst_HeadSetupVulnerable(self);
    }
}

void BossSst_HeadSetupVulnerable(BossSst* self) {
    Animation_MorphToLoop(&self->skelAnime, &gBongoHeadStunnedAnim, -5.0f);
    self->colliderCyl.base.acFlags |= AC_ON;
    self->colliderCyl.info.bumper.dmgFlags = 0x0FC00702; // Sword-type damage
    self->actor.speedXZ = 0.0f;
    self->colliderJntSph.elements[10].info.bumperFlags |= (BUMP_ON | BUMP_HOOKABLE);
    self->colliderJntSph.elements[0].info.bumperFlags &= ~BUMP_ON;
    if (self->actionFunc != BossSst_HeadDamage) {
        self->timer = 50;
    }

    self->actionFunc = BossSst_HeadVulnerable;
}

void BossSst_HeadVulnerable(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&sHandOffsets[LEFT].z, 600.0f, 20.0f);
    Math_StepToF(&sHandOffsets[RIGHT].z, 600.0f, 20.0f);
    Math_StepToF(&sHandOffsets[LEFT].x, 200.0f, 20.0f);
    Math_StepToF(&sHandOffsets[RIGHT].x, -200.0f, 20.0f);
    if ((self->actor.flags & 0x2000) == 0x2000) {
        self->timer += 2;
        self->timer = CLAMP_MAX(self->timer, 50);
    } else {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            BossSst_HandSetupRecover(sHands[LEFT]);
            BossSst_HandSetupRecover(sHands[RIGHT]);
            BossSst_HeadSetupRecover(self);
        }
    }
}

void BossSst_HeadSetupDamage(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBongoHeadDamageAnim, -3.0f);
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, Animation_GetLastFrame(&gBongoHeadDamageAnim));
    Actor_SetColorFilter(&sHands[LEFT]->actor, 0x4000, 0xFF, 0, Animation_GetLastFrame(&gBongoHeadDamageAnim));
    Actor_SetColorFilter(&sHands[RIGHT]->actor, 0x4000, 0xFF, 0, Animation_GetLastFrame(&gBongoHeadDamageAnim));
    self->colliderCyl.base.acFlags &= ~AC_ON;
    BossSst_HeadSfx(self, NA_SE_EN_SHADEST_DAMAGE);
    self->actionFunc = BossSst_HeadDamage;
}

void BossSst_HeadDamage(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        BossSst_HeadSetupVulnerable(self);
    }
}

void BossSst_HeadSetupRecover(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBongoHeadRecoverAnim, -5.0f);
    self->colliderCyl.base.acFlags &= ~AC_ON;
    self->colliderCyl.info.bumper.dmgFlags = 0xFFCFFFFF;
    self->colliderJntSph.elements[10].info.bumperFlags &= ~(BUMP_ON | BUMP_HOOKABLE);
    self->colliderJntSph.elements[0].info.bumperFlags |= BUMP_ON;
    self->vVanish = true;
    self->actor.speedXZ = 5.0f;
    self->actionFunc = BossSst_HeadRecover;
}

void BossSst_HeadRecover(BossSst* self, GlobalContext* globalCtx) {
    s32 animFinish;
    f32 currentFrame;
    f32 diff;

    animFinish = SkelAnime_Update(&self->skelAnime);
    currentFrame = self->skelAnime.curFrame;
    if (currentFrame < 10.0f) {
        self->actor.world.pos.y += 10.0f;
        sHandOffsets[LEFT].y -= 10.0f;
        sHandOffsets[RIGHT].y -= 10.0f;
        Math_SmoothStepToF(&self->radius, -750.0f, 1.0f, self->actor.speedXZ, 2.0f);
    } else {
        self->actor.speedXZ *= 1.25f;
        self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 50.0f);
        diff = Math_SmoothStepToF(&self->radius, -650.0f, 1.0f, self->actor.speedXZ, 2.0f);
        diff += Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.5f, 30.0f, 3.0f);
    }
    if (animFinish && (diff < 10.0f)) {
        self->actor.world.pos.y = self->actor.home.pos.y;
        self->radius = -650.0f;
        BossSst_HandSetupRetreat(sHands[LEFT]);
        BossSst_HandSetupRetreat(sHands[RIGHT]);
        BossSst_HeadSetupNeutral(self);
    }
}

void BossSst_SetCameraTargets(f32 cameraSpeedMod, s32 targetIndex) {
    Vec3f* nextAt = &sCameraAtPoints[targetIndex];
    Vec3f* nextEye = &sCameraEyePoints[targetIndex];

    if (targetIndex != 0) {
        Math_Vec3f_Copy(&sCameraAt, &sCameraAtPoints[targetIndex - 1]);
        Math_Vec3f_Copy(&sCameraEye, &sCameraEyePoints[targetIndex - 1]);
    }

    sCameraAtVel.x = (nextAt->x - sCameraAt.x) * cameraSpeedMod;
    sCameraAtVel.y = (nextAt->y - sCameraAt.y) * cameraSpeedMod;
    sCameraAtVel.z = (nextAt->z - sCameraAt.z) * cameraSpeedMod;

    sCameraEyeVel.x = (nextEye->x - sCameraEye.x) * cameraSpeedMod;
    sCameraEyeVel.y = (nextEye->y - sCameraEye.y) * cameraSpeedMod;
    sCameraEyeVel.z = (nextEye->z - sCameraEye.z) * cameraSpeedMod;
}

void BossSst_UpdateDeathCamera(BossSst* self, GlobalContext* globalCtx) {
    Vec3f cameraAt;
    Vec3f cameraEye;
    f32 sn;
    f32 cs;

    sCameraAt.x += sCameraAtVel.x;
    sCameraAt.y += sCameraAtVel.y;
    sCameraAt.z += sCameraAtVel.z;
    sCameraEye.x += sCameraEyeVel.x;
    sCameraEye.y += sCameraEyeVel.y;
    sCameraEye.z += sCameraEyeVel.z;

    sn = Math_SinS(self->actor.shape.rot.y);
    cs = Math_CosS(self->actor.shape.rot.y);
    cameraAt.x = self->actor.world.pos.x + (sCameraAt.z * sn) + (sCameraAt.x * cs);
    cameraAt.y = self->actor.home.pos.y - 140.0f + sCameraAt.y;
    cameraAt.z = self->actor.world.pos.z + (sCameraAt.z * cs) - (sCameraAt.x * sn);
    cameraEye.x = self->actor.world.pos.x + (sCameraEye.z * sn) + (sCameraEye.x * cs);
    cameraEye.y = self->actor.home.pos.y - 140.0f + sCameraEye.y;
    cameraEye.z = self->actor.world.pos.z + (sCameraEye.z * cs) - (sCameraEye.x * sn);
    Gameplay_CameraSetAtEye(globalCtx, sCutsceneCamera, &cameraAt, &cameraEye);
}

void BossSst_HeadSetupDeath(BossSst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Animation_MorphToLoop(&self->skelAnime, &gBongoHeadEyeOpenIdleAnim, -5.0f);
    BossSst_HeadSfx(self, NA_SE_EN_SHADEST_DEAD);
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 60);
    Actor_SetColorFilter(&sHands[LEFT]->actor, 0x4000, 0xFF, 0, 60);
    Actor_SetColorFilter(&sHands[RIGHT]->actor, 0x4000, 0xFF, 0, 60);
    self->timer = 60;
    self->colliderCyl.base.acFlags &= ~AC_ON;
    self->colliderJntSph.base.ocFlags1 &= ~OC1_ON;
    sHands[LEFT]->colliderJntSph.base.ocFlags1 &= ~OC1_ON;
    sHands[RIGHT]->colliderJntSph.base.ocFlags1 &= ~OC1_ON;
    Audio_QueueSeqCmd(0x100100FF);
    sCutsceneCamera = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, sCutsceneCamera, CAM_STAT_ACTIVE);
    Gameplay_CopyCamera(globalCtx, sCutsceneCamera, MAIN_CAM);
    func_8002DF54(globalCtx, &player->actor, 8);
    func_80064520(globalCtx, &globalCtx->csCtx);
    Math_Vec3f_Copy(&sCameraEye, &GET_ACTIVE_CAM(globalCtx)->eye);
    self->actionFunc = BossSst_HeadDeath;
}

void BossSst_HeadDeath(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - 140.0f, 20.0f);
    if (self->timer == 0) {
        BossSst_HandSetupThrash(sHands[LEFT]);
        BossSst_HandSetupThrash(sHands[RIGHT]);
        BossSst_HeadSetupThrash(self);
    } else if (self->timer > 48) {
        Gameplay_CameraSetAtEye(globalCtx, sCutsceneCamera, &self->actor.focus.pos, &sCameraEye);
        Math_StepToF(&self->radius, -350.0f, 10.0f);
    } else if (self->timer == 48) {
        Player* player = GET_PLAYER(globalCtx);

        player->actor.world.pos.x = sRoomCenter.x + (400.0f * Math_SinS(self->actor.shape.rot.y)) +
                                    (Math_CosS(self->actor.shape.rot.y) * -120.0f);
        player->actor.world.pos.z = sRoomCenter.z + (400.0f * Math_CosS(self->actor.shape.rot.y)) -
                                    (Math_SinS(self->actor.shape.rot.y) * -120.0f);
        player->actor.shape.rot.y = Actor_WorldYawTowardPoint(&player->actor, &sRoomCenter);
        func_8002DBD0(&self->actor, &sCameraEye, &GET_ACTIVE_CAM(globalCtx)->eye);
        func_8002DBD0(&self->actor, &sCameraAt, &GET_ACTIVE_CAM(globalCtx)->at);
        self->radius = -350.0f;
        self->actor.world.pos.x = sRoomCenter.x - (Math_SinS(self->actor.shape.rot.y) * 350.0f);
        self->actor.world.pos.z = sRoomCenter.z - (Math_CosS(self->actor.shape.rot.y) * 350.0f);
        BossSst_SetCameraTargets(1.0 / 48, 0);
        BossSst_UpdateDeathCamera(self, globalCtx);
    } else {
        BossSst_UpdateDeathCamera(self, globalCtx);
    }
}

void BossSst_HeadSetupThrash(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gBongoHeadEyeOpenIdleAnim, -5.0f);
    self->timer = 160;
    self->targetYaw = self->actor.shape.rot.y;
    BossSst_SetCameraTargets(1.0 / 80, 1);
    self->actionFunc = BossSst_HeadThrash;
}

void BossSst_HeadThrash(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    if ((self->timer == 0) && (self->actor.shape.rot.y == self->targetYaw)) {
        BossSst_HeadSetupDarken(self);
    } else if (self->timer >= 80) {
        BossSst_UpdateDeathCamera(self, globalCtx);
    }
}

void BossSst_HeadSetupDarken(BossSst* self) {
    self->timer = 160;
    BossSst_SetCameraTargets(1.0 / 80, 2);
    self->actionFunc = BossSst_HeadDarken;
}

void BossSst_HeadDarken(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (1) {}

    if (self->timer >= 80) {
        if (self->timer == 80) {
            sBodyStatic = true;
        }
        BossSst_UpdateDeathCamera(self, globalCtx);
        sBodyColor.r = sBodyColor.g = sBodyColor.b = (self->timer * 3) - 240;
        if (self->timer == 80) {
            BossSst_SetCameraTargets(1.0 / 80, 3);
        }
    } else {
        sBodyColor.b = (80 - self->timer) / 1.0f;
        sBodyColor.r = sBodyColor.g = sStaticColor.r = sStaticColor.g = sStaticColor.b = (80 - self->timer) / 8.0f;
        BossSst_UpdateDeathCamera(self, globalCtx);
        if (self->timer == 0) {
            BossSst_HeadSetupFall(self);
        }
    }
}

void BossSst_HeadSetupFall(BossSst* self) {
    self->actor.speedXZ = 1.0f;
    Math_Vec3f_Copy(&sCameraAt, &sCameraAtPoints[3]);
    Math_Vec3f_Copy(&sCameraEye, &sCameraEyePoints[3]);
    sCameraAtVel.x = 0.0f;
    sCameraAtVel.z = 0.0f;
    sCameraAtVel.y = -50.0f;
    Math_Vec3f_Copy(&sCameraEyeVel, &sZeroVec);
    self->actionFunc = BossSst_HeadFall;
}

void BossSst_HeadFall(BossSst* self, GlobalContext* globalCtx) {
    self->actor.speedXZ *= 1.5f;
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - 230.0f, self->actor.speedXZ)) {
        BossSst_HeadSetupMelt(self);
    }

    if (sCameraAt.y > 200.0f) {
        BossSst_UpdateDeathCamera(self, globalCtx);
    }
}

void BossSst_HeadSetupMelt(BossSst* self) {
    BossSst_SpawnHeadShadow(self);
    self->timer = 80;
    BossSst_SetCameraTargets(1.0 / 60, 5);
    self->actionFunc = BossSst_HeadMelt;
}

void BossSst_HeadMelt(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.scale.y -= 0.00025f;
    self->actor.scale.x += 0.000075f;
    self->actor.scale.z += 0.000075f;
    self->actor.world.pos.y = self->actor.home.pos.y - 11500.0f * self->actor.scale.y;
    if (self->timer == 0) {
        BossSst_HeadSetupFinish(self);
    } else if (self->timer >= 20.0f) {
        BossSst_UpdateDeathCamera(self, globalCtx);
    }
}

void BossSst_HeadSetupFinish(BossSst* self) {
    self->actor.draw = BossSst_DrawEffect;
    self->timer = 40;
    Audio_QueueSeqCmd(0x21);
    BossSst_SetCameraTargets(1.0 / 40, 6);
    self->actionFunc = BossSst_HeadFinish;
}

void BossSst_HeadFinish(BossSst* self, GlobalContext* globalCtx) {
    static Color_RGBA8 colorIndigo = { 80, 80, 150, 255 };
    static Color_RGBA8 colorDarkIndigo = { 40, 40, 80, 255 };
    static Color_RGBA8 colorUnused[2] = {
        { 0, 0, 0, 255 },
        { 100, 100, 100, 0 },
    };
    Vec3f spawnPos;
    s32 i;

    self->timer--;
    if (self->effectMode == BONGO_NULL) {
        if (self->timer < -170) {
            BossSst_UpdateDeathCamera(self, globalCtx);
            Gameplay_CopyCamera(globalCtx, MAIN_CAM, sCutsceneCamera);
            Gameplay_ChangeCameraStatus(globalCtx, sCutsceneCamera, CAM_STAT_WAIT);
            Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
            Gameplay_ClearCamera(globalCtx, sCutsceneCamera);
            func_8002DF54(globalCtx, &GET_PLAYER(globalCtx)->actor, 7);
            func_80064534(globalCtx, &globalCtx->csCtx);
            Actor_Kill(&self->actor);
            Actor_Kill(&sHands[LEFT]->actor);
            Actor_Kill(&sHands[RIGHT]->actor);
            Flags_SetClear(globalCtx, globalCtx->roomCtx.curRoom.num);
        }
    } else if (self->effects[0].alpha == 0) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_WARP1, ROOM_CENTER_X, ROOM_CENTER_Y, ROOM_CENTER_Z, 0,
                    0, 0, WARP_DUNGEON_ADULT);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART,
                    (Math_SinS(self->actor.shape.rot.y) * 200.0f) + ROOM_CENTER_X, ROOM_CENTER_Y,
                    Math_CosS(self->actor.shape.rot.y) * 200.0f + ROOM_CENTER_Z, 0, 0, 0, 0);
        BossSst_SetCameraTargets(1.0f, 7);
        self->effectMode = BONGO_NULL;
    } else if (self->timer == 0) {
        self->effects[0].status = 0;
        self->effects[1].status = -1;
        self->effects[2].status = -1;
    } else if (self->timer > 0) {
        self->effects[0].status += 5;
        BossSst_UpdateDeathCamera(self, globalCtx);
    }

    colorIndigo.a = self->effects[0].alpha;
    colorDarkIndigo.a = self->effects[0].alpha;

    for (i = 0; i < 5; i++) {
        spawnPos.x = sRoomCenter.x + 0.0f + Rand_CenteredFloat(800.0f);
        spawnPos.y = sRoomCenter.y + (-28.0f) + (Rand_ZeroOne() * 5.0f);
        spawnPos.z = sRoomCenter.z + 0.0f + Rand_CenteredFloat(800.0f);
        EffectSsGSplash_Spawn(globalCtx, &spawnPos, &colorIndigo, &colorDarkIndigo, 0, 0x3E8);
    }
}

void BossSst_HandSetupWait(BossSst* self) {
    HAND_STATE(self) = HAND_WAIT;
    self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
    Animation_MorphToLoop(&self->skelAnime, sHandIdleAnims[self->actor.params], 5.0f);
    self->ready = false;
    self->timer = 20;
    self->actionFunc = BossSst_HandWait;
}

void BossSst_HandWait(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, 20.0f);
    Math_StepToF(&self->actor.world.pos.x, self->actor.home.pos.x, 1.0f);
    Math_StepToF(&self->actor.world.pos.z, self->actor.home.pos.z, 1.0f);
    if (HAND_STATE(OTHER_HAND(self)) == HAND_DAMAGED) {
        Player* player = GET_PLAYER(globalCtx);

        if (self->timer != 0) {
            self->timer--;
        }

        if ((self->timer == 0) && (player->actor.world.pos.y > -50.0f) && !(player->stateFlags1 & 0x6080)) {
            BossSst_HandSelectAttack(self);
        }
    } else if (sHead->actionFunc == BossSst_HeadNeutral) {
        if ((self->actor.params == BONGO_RIGHT_HAND) && ((sHead->timer % 28) == 12)) {
            BossSst_HandSetupDownbeat(self);
        } else if ((self->actor.params == BONGO_LEFT_HAND) && ((sHead->timer % 7) == 5) && (sHead->timer < 112)) {
            BossSst_HandSetupOffbeat(self);
        }
    }
}

void BossSst_HandSetupDownbeat(BossSst* self) {
    HAND_STATE(self) = HAND_BEAT;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 5.0f);
    self->actor.shape.rot.x = 0;
    self->timer = 12;
    self->actionFunc = BossSst_HandDownbeat;
}

void BossSst_HandDownbeat(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (HAND_STATE(OTHER_HAND(self)) == HAND_DAMAGED) {
        BossSst_HandSetupWait(self);
    } else {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer >= 3) {
            self->actor.shape.rot.x -= 0x100;
            Math_StepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 180.0f, 20.0f);
        } else {
            self->actor.shape.rot.x += 0x300;
            Math_StepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 0.0f, 60.0f);
        }

        if (self->timer == 0) {
            sFloor->dyna.actor.params = BONGOFLOOR_HIT;
            if (sHead->actionFunc == BossSst_HeadWait) {
                if (self->ready) {
                    BossSst_HandSelectAttack(self);
                } else {
                    BossSst_HandSetupWait(self);
                }
            } else {
                BossSst_HandSetupDownbeatEnd(self);
            }
            func_800AA000(self->actor.xyzDistToPlayerSq, 0xFF, 0x14, 0x96);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_TAIKO_HIGH);
        }
    }
}

void BossSst_HandSetupDownbeatEnd(BossSst* self) {
    sFloor->dyna.actor.params = BONGOFLOOR_HIT;
    Animation_PlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params]);
    self->actionFunc = BossSst_HandDownbeatEnd;
}

void BossSst_HandDownbeatEnd(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (HAND_STATE(OTHER_HAND(self)) == HAND_DAMAGED) {
        BossSst_HandSetupWait(self);
    } else {
        Math_SmoothStepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 40.0f, 0.5f, 20.0f, 3.0f);
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x800, 0x100);
        Math_StepToF(&self->actor.world.pos.x, self->actor.home.pos.x, 1.0f);
        Math_StepToF(&self->actor.world.pos.z, self->actor.home.pos.z, 1.0f);
        if ((sHead->actionFunc != BossSst_HeadIntro) && ((sHead->timer % 28) == 12)) {
            BossSst_HandSetupDownbeat(self);
        }
    }
}

void BossSst_HandSetupOffbeat(BossSst* self) {
    HAND_STATE(self) = HAND_BEAT;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 5.0f);
    self->actor.shape.rot.x = 0;
    self->timer = 5;
    self->actionFunc = BossSst_HandOffbeat;
}

void BossSst_HandOffbeat(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (HAND_STATE(OTHER_HAND(self)) == HAND_DAMAGED) {
        BossSst_HandSetupWait(self);
    } else {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer != 0) {
            self->actor.shape.rot.x -= 0x140;
            Math_StepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 60.0f, 15.0f);
        } else {
            self->actor.shape.rot.x += 0x500;
            Math_StepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 0.0f, 60.0f);
        }

        if (self->timer == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_TAIKO_LOW);
            BossSst_HandSetupOffbeatEnd(self);
        }
    }
}

void BossSst_HandSetupOffbeatEnd(BossSst* self) {
    Animation_PlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params]);
    self->actionFunc = BossSst_HandOffbeatEnd;
}

void BossSst_HandOffbeatEnd(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (HAND_STATE(OTHER_HAND(self)) == HAND_DAMAGED) {
        BossSst_HandSetupWait(self);
    } else {
        Math_SmoothStepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 40.0f, 0.5f, 20.0f, 3.0f);
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x400, 0xA0);
        Math_StepToF(&self->actor.world.pos.x, self->actor.home.pos.x, 1.0f);
        Math_StepToF(&self->actor.world.pos.z, self->actor.home.pos.z, 1.0f);
        if (sHead->actionFunc == BossSst_HeadWait) {
            if (self->ready) {
                BossSst_HandSelectAttack(self);
            } else {
                BossSst_HandSetupWait(self);
            }
        } else if ((sHead->actionFunc != BossSst_HeadIntro) && ((sHead->timer % 7) == 5) &&
                   ((sHead->timer % 28) != 5)) {
            BossSst_HandSetupOffbeat(self);
        }
    }
}

void BossSst_HandSetupEndSlam(BossSst* self) {
    HAND_STATE(self) = HAND_RETREAT;
    self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
    Animation_MorphToPlayOnce(&self->skelAnime, sHandPushoffPoses[self->actor.params], 6.0f);
    self->actionFunc = BossSst_HandEndSlam;
}

void BossSst_HandEndSlam(BossSst* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        BossSst_HandSetupRetreat(self);
    }
}

void BossSst_HandSetupRetreat(BossSst* self) {
    HAND_STATE(self) = HAND_RETREAT;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandHangPoses[self->actor.params], 10.0f);
    self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
    self->colliderJntSph.base.acFlags |= AC_ON;
    self->actor.flags |= 1;
    BossSst_HandSetInvulnerable(self, false);
    self->timer = 0;
    self->actionFunc = BossSst_HandRetreat;
    self->actor.speedXZ = 3.0f;
}

void BossSst_HandRetreat(BossSst* self, GlobalContext* globalCtx) {
    f32 diff;
    s32 inPosition;

    SkelAnime_Update(&self->skelAnime);
    self->actor.speedXZ = self->actor.speedXZ * 1.2f;
    self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 50.0f);

    diff = Math_SmoothStepToF(&self->actor.world.pos.x, self->actor.home.pos.x, 0.3f, self->actor.speedXZ, 1.0f);
    diff += Math_SmoothStepToF(&self->actor.world.pos.z, self->actor.home.pos.z, 0.3f, self->actor.speedXZ, 1.0f);
    if (self->timer != 0) {
        if (self->timer != 0) {
            self->timer--;
        }

        self->actor.world.pos.y = (sinf((self->timer * M_PI) / 16.0f) * 250.0f) + self->actor.home.pos.y;
        if (self->timer == 0) {
            BossSst_HandSetupWait(self);
        } else if (self->timer == 4) {
            Animation_MorphToLoop(&self->skelAnime, sHandIdleAnims[self->actor.params], 4.0f);
        }
    } else {
        inPosition = Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, 0x200);
        inPosition &= Math_ScaledStepToS(&self->actor.shape.rot.z, self->actor.home.rot.z, 0x200);
        inPosition &= Math_ScaledStepToS(&self->handYRotMod, 0, 0x800);
        func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
        if ((Math_SmoothStepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 250.0f, 0.5f, 70.0f, 5.0f) < 1.0f) &&
            inPosition && (diff < 10.0f)) {
            self->timer = 8;
        }
    }
}

void BossSst_HandSetupReadySlam(BossSst* self) {
    HAND_STATE(self) = HAND_SLAM;
    self->timer = 0;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 10.0f);
    self->actionFunc = BossSst_HandReadySlam;
}

void BossSst_HandReadySlam(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            BossSst_HandSetupSlam(self);
        }
    } else {
        Player* player = GET_PLAYER(globalCtx);

        if (Math_StepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 300.0f, 30.0f) &&
            (self->actor.xzDistToPlayer < 140.0f)) {
            self->timer = 20;
        }
        Math_ScaledStepToS(&self->actor.shape.rot.x, -0x1000, 0x100);
        Math_ApproachF(&self->actor.world.pos.x, player->actor.world.pos.x, 0.5f, 40.0f);
        Math_ApproachF(&self->actor.world.pos.z, player->actor.world.pos.z, 0.5f, 40.0f);
        func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
    }
}

void BossSst_HandSetupSlam(BossSst* self) {
    HAND_STATE(self) = HAND_SLAM;
    self->actor.velocity.y = 1.0f;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 10.0f);
    BossSst_HandSetDamage(self, 0x20);
    self->ready = false;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_FLY_ATTACK);
    self->actionFunc = BossSst_HandSlam;
}

void BossSst_HandSlam(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToS(&self->handZPosMod, -0xDAC, 0x1F4);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x1000);
    Math_ScaledStepToS(&self->handYRotMod, 0, 0x1000);
    if (self->timer != 0) {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            if (self->colliderJntSph.base.acFlags & AC_ON) {
                BossSst_HandSetupEndSlam(self);
            } else {
                self->colliderJntSph.base.acFlags |= AC_ON;
                BossSst_HandSetupWait(self);
            }
        }
    } else {
        if (self->ready) {
            self->timer = 30;
            self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        } else {
            self->actor.velocity.y *= 1.5f;
            if (Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, self->actor.velocity.y)) {
                self->ready = true;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_TAIKO_LOW);
                BossSst_SpawnShockwave(self);
                self->colliderCyl.base.atFlags |= AT_ON;
                Collider_UpdateCylinder(&self->actor, &self->colliderCyl);
                self->colliderCyl.dim.radius = sCylinderInitHand.dim.radius;
            }
        }

        if (self->colliderJntSph.base.atFlags & AT_HIT) {
            Player* player = GET_PLAYER(globalCtx);

            player->actor.world.pos.x = (Math_SinS(self->actor.yawTowardsPlayer) * 100.0f) + self->actor.world.pos.x;
            player->actor.world.pos.z = (Math_CosS(self->actor.yawTowardsPlayer) * 100.0f) + self->actor.world.pos.z;

            self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
            func_8002F71C(globalCtx, &self->actor, 5.0f, self->actor.yawTowardsPlayer, 0.0f);
        }

        Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x200);
    }
}

void BossSst_HandSetupReadySweep(BossSst* self) {
    HAND_STATE(self) = HAND_SWEEP;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 10.0f);
    self->radius = Actor_WorldDistXZToPoint(&self->actor, &sHead->actor.world.pos);
    self->actor.world.rot.y = Actor_WorldYawTowardPoint(&sHead->actor, &self->actor.world.pos);
    self->targetYaw = self->actor.home.rot.y + (self->vParity * 0x2000);
    self->actionFunc = BossSst_HandReadySweep;
}

void BossSst_HandReadySweep(BossSst* self, GlobalContext* globalCtx) {
    s32 inPosition;

    SkelAnime_Update(&self->skelAnime);
    inPosition = Math_StepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 50.0f, 4.0f);
    inPosition &= Math_ScaledStepToS(&self->actor.shape.rot.y, self->targetYaw, 0x200);
    inPosition &= Math_ScaledStepToS(&self->actor.world.rot.y, self->targetYaw, 0x400);
    inPosition &= (Math_SmoothStepToF(&self->radius, sHead->actor.xzDistToPlayer, 0.5f, 60.0f, 1.0f) < 10.0f);

    self->actor.world.pos.x = (Math_SinS(self->actor.world.rot.y) * self->radius) + sHead->actor.world.pos.x;
    self->actor.world.pos.z = (Math_CosS(self->actor.world.rot.y) * self->radius) + sHead->actor.world.pos.z;
    if (inPosition) {
        BossSst_HandSetupSweep(self);
    } else {
        func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
    }
}

void BossSst_HandSetupSweep(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 5.0f);
    BossSst_HandSetDamage(self, 0x10);
    self->targetYaw = self->actor.home.rot.y - (self->vParity * 0x2000);
    self->handMaxSpeed = 0x300;
    self->handAngSpeed = 0;
    self->ready = false;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_FLY_ATTACK);
    self->actionFunc = BossSst_HandSweep;
}

void BossSst_HandSweep(BossSst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 newTargetYaw;

    SkelAnime_Update(&self->skelAnime);
    self->handAngSpeed += 0x60;
    self->handAngSpeed = CLAMP_MAX(self->handAngSpeed, self->handMaxSpeed);

    if (!Math_SmoothStepToS(&self->actor.shape.rot.y, self->targetYaw, 4, self->handAngSpeed, 0x10)) {
        self->colliderJntSph.base.ocFlags1 &= ~OC1_NO_PUSH;
        BossSst_HandSetupRetreat(self);
    } else if (self->colliderJntSph.base.atFlags & AT_HIT) {
        self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        self->ready = true;
        func_8002F71C(globalCtx, &self->actor, 5.0f, self->actor.shape.rot.y - (self->vParity * 0x3800), 0.0f);
        func_8002F7DC(&player->actor, NA_SE_PL_BODY_HIT);
        newTargetYaw = self->actor.shape.rot.y - (self->vParity * 0x1400);
        if (((s16)(newTargetYaw - self->targetYaw) * self->vParity) > 0) {
            self->targetYaw = newTargetYaw;
        }
    }

    if (!self->ready && ((player->cylinder.dim.height > 40.0f) || (player->actor.world.pos.y > 1.0f))) {
        self->colliderJntSph.base.atFlags |= AT_ON;
        self->colliderJntSph.base.ocFlags1 &= ~OC1_NO_PUSH;
    } else {
        self->colliderJntSph.base.atFlags &= ~AT_ON;
        self->colliderJntSph.base.ocFlags1 |= OC1_NO_PUSH;
    }

    self->actor.world.pos.x = (Math_SinS(self->actor.shape.rot.y) * self->radius) + sHead->actor.world.pos.x;
    self->actor.world.pos.z = (Math_CosS(self->actor.shape.rot.y) * self->radius) + sHead->actor.world.pos.z;
}

void BossSst_HandSetupReadyPunch(BossSst* self) {
    HAND_STATE(self) = HAND_PUNCH;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandPushoffPoses[self->actor.params], 10.0f);
    self->actionFunc = BossSst_HandReadyPunch;
}

void BossSst_HandReadyPunch(BossSst* self, GlobalContext* globalCtx) {
    s32 inPosition = Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0x400);

    if (SkelAnime_Update(&self->skelAnime) && inPosition) {
        BossSst_HandSetupPunch(self);
    }
}

void BossSst_HandSetupPunch(BossSst* self) {
    self->actor.speedXZ = 0.5f;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFistPoses[self->actor.params], 5.0f);
    BossSst_HandSetInvulnerable(self, true);
    self->targetRoll = self->vParity * 0x3F00;
    BossSst_HandSetDamage(self, 0x10);
    self->actionFunc = BossSst_HandPunch;
}

void BossSst_HandPunch(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_StepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 80.0f, 20.0f);
    if (Math_ScaledStepToS(&self->actor.shape.rot.z, self->targetRoll, 0x400)) {
        self->targetRoll *= -1;
    }

    self->actor.speedXZ *= 1.25f;
    self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 50.0f);

    self->actor.world.pos.x += self->actor.speedXZ * Math_SinS(self->actor.shape.rot.y);
    self->actor.world.pos.z += self->actor.speedXZ * Math_CosS(self->actor.shape.rot.y);
    if (self->actor.bgCheckFlags & 8) {
        BossSst_HandSetupRetreat(self);
    } else if (self->colliderJntSph.base.atFlags & AT_HIT) {
        func_8002F7DC(&GET_PLAYER(globalCtx)->actor, NA_SE_PL_BODY_HIT);
        func_8002F71C(globalCtx, &self->actor, 10.0f, self->actor.shape.rot.y, 5.0f);
        BossSst_HandSetupRetreat(self);
    }

    func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
}

void BossSst_HandSetupReadyClap(BossSst* self) {
    HAND_STATE(self) = HAND_CLAP;
    if (HAND_STATE(OTHER_HAND(self)) != HAND_CLAP) {
        BossSst_HandSetupReadyClap(OTHER_HAND(self));
    }

    Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 10.0f);
    self->radius = Actor_WorldDistXZToPoint(&self->actor, &sHead->actor.world.pos);
    self->actor.world.rot.y = Actor_WorldYawTowardPoint(&sHead->actor, &self->actor.world.pos);
    self->targetYaw = self->actor.home.rot.y - (self->vParity * 0x1800);
    self->targetRoll = self->vParity * 0x4000;
    self->timer = 0;
    self->ready = false;
    OTHER_HAND(self)->ready = false;
    self->actionFunc = BossSst_HandReadyClap;
}

void BossSst_HandReadyClap(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            BossSst_HandSetupClap(self);
            BossSst_HandSetupClap(OTHER_HAND(self));
            OTHER_HAND(self)->radius = self->radius;
        }
    } else if (!self->ready) {
        self->ready = SkelAnime_Update(&self->skelAnime);
        self->ready &= Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x600);
        self->ready &= Math_ScaledStepToS(&self->actor.shape.rot.z, self->targetRoll, 0x600);
        self->ready &= Math_ScaledStepToS(&self->actor.shape.rot.y, self->targetYaw, 0x200);
        self->ready &= Math_ScaledStepToS(&self->actor.world.rot.y, self->targetYaw, 0x400);
        self->ready &= Math_SmoothStepToF(&self->radius, sHead->actor.xzDistToPlayer, 0.5f, 50.0f, 1.0f) < 10.0f;
        self->ready &= Math_SmoothStepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 95.0f, 0.5f, 30.0f, 1.0f) < 1.0f;

        self->actor.world.pos.x = Math_SinS(self->actor.world.rot.y) * self->radius + sHead->actor.world.pos.x;
        self->actor.world.pos.z = Math_CosS(self->actor.world.rot.y) * self->radius + sHead->actor.world.pos.z;
    } else if (OTHER_HAND(self)->ready) {
        self->timer = 20;
    }
}

void BossSst_HandSetupClap(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 3.0f);
    self->timer = 0;
    self->handMaxSpeed = 0x240;
    self->handAngSpeed = 0;
    self->ready = false;
    BossSst_HandSetDamage(self, 0x20);
    self->actionFunc = BossSst_HandClap;
}

void BossSst_HandClap(BossSst* self, GlobalContext* globalCtx) {
    static s32 dropFlag = false;
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            if (dropFlag) {
                Item_DropCollectible(globalCtx, &self->actor.world.pos,
                                     (Rand_ZeroOne() < 0.5f) ? ITEM00_ARROWS_SMALL : ITEM00_MAGIC_SMALL);
                dropFlag = false;
            }

            BossSst_HandReleasePlayer(self, globalCtx, true);
            BossSst_HandSetupEndClap(self);
        }
    } else {
        if (self->colliderJntSph.base.atFlags & AT_HIT) {
            self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
            OTHER_HAND(self)->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
            BossSst_HandGrabPlayer(self, globalCtx);
        }

        if (self->ready) {
            self->timer = 30;
            self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
            if (!(player->stateFlags2 & 0x80)) {
                dropFlag = true;
            }
        } else {
            self->handAngSpeed += 0x40;
            self->handAngSpeed = CLAMP_MAX(self->handAngSpeed, self->handMaxSpeed);

            if (Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, self->handAngSpeed)) {
                if (self->actor.params == BONGO_LEFT_HAND) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_CLAP);
                }
                self->ready = true;
            } else {
                func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
            }

            self->actor.world.pos.x = (Math_SinS(self->actor.shape.rot.y) * self->radius) + sHead->actor.world.pos.x;
            self->actor.world.pos.z = (Math_CosS(self->actor.shape.rot.y) * self->radius) + sHead->actor.world.pos.z;
        }
    }

    if (player->actor.parent == &self->actor) {
        player->unk_850 = 0;
        player->actor.world.pos = self->actor.world.pos;
    }
}

void BossSst_HandSetupEndClap(BossSst* self) {
    self->targetYaw = self->actor.home.rot.y - (self->vParity * 0x1000);
    Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 10.0f);
    self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
    self->actionFunc = BossSst_HandEndClap;
}

void BossSst_HandEndClap(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.z, 0, 0x200);
    if (Math_ScaledStepToS(&self->actor.shape.rot.y, self->targetYaw, 0x100)) {
        BossSst_HandSetupRetreat(self);
    }
    self->actor.world.pos.x = (Math_SinS(self->actor.shape.rot.y) * self->radius) + sHead->actor.world.pos.x;
    self->actor.world.pos.z = (Math_CosS(self->actor.shape.rot.y) * self->radius) + sHead->actor.world.pos.z;
}

void BossSst_HandSetupReadyGrab(BossSst* self) {
    HAND_STATE(self) = HAND_GRAB;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 10.0f);
    self->targetYaw = self->vParity * -0x5000;
    self->targetRoll = self->vParity * 0x4000;
    self->actionFunc = BossSst_HandReadyGrab;
}

void BossSst_HandReadyGrab(BossSst* self, GlobalContext* globalCtx) {
    s32 inPosition;

    SkelAnime_Update(&self->skelAnime);
    inPosition = Math_SmoothStepToS(&self->actor.shape.rot.z, self->targetRoll, 4, 0x800, 0x100) == 0;
    inPosition &= Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer + self->targetYaw, 0xA00);
    Math_ApproachF(&self->actor.world.pos.y, ROOM_CENTER_Y + 95.0f, 0.5f, 20.0f);
    if (inPosition) {
        BossSst_HandSetupGrab(self);
    }
}

void BossSst_HandSetupGrab(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFistPoses[self->actor.params], 5.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y + (self->vParity * 0x4000);
    self->targetYaw = self->actor.world.rot.y;
    self->timer = 30;
    self->actor.speedXZ = 0.5f;
    BossSst_HandSetDamage(self, 0x20);
    self->actionFunc = BossSst_HandGrab;
}

void BossSst_HandGrab(BossSst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.world.rot.y =
        ((1.0f - sinf(self->timer * (M_PI / 60.0f))) * (self->vParity * 0x2000)) + self->targetYaw;
    self->actor.shape.rot.y = self->actor.world.rot.y - (self->vParity * 0x4000);
    if (self->timer < 5) {
        Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.5f, 25.0f, 5.0f);
        if (SkelAnime_Update(&self->skelAnime)) {
            self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
            self->actor.speedXZ = 0.0f;
            if (player->stateFlags2 & 0x80) {
                if (Rand_ZeroOne() < 0.5f) {
                    BossSst_HandSetupCrush(self);
                } else {
                    BossSst_HandSetupSwing(self);
                }
            } else {
                Item_DropCollectible(globalCtx, &self->actor.world.pos,
                                     (Rand_ZeroOne() < 0.5f) ? ITEM00_ARROWS_SMALL : ITEM00_MAGIC_SMALL);
                BossSst_HandSetupRetreat(self);
            }
        }
    } else {
        self->actor.speedXZ *= 1.26f;
        self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 70.0f);
        func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
    }

    if (self->colliderJntSph.base.atFlags & AT_HIT) {
        self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_CATCH);
        BossSst_HandGrabPlayer(self, globalCtx);
        self->timer = CLAMP_MAX(self->timer, 5);
    }

    self->actor.world.pos.x += self->actor.speedXZ * Math_SinS(self->actor.world.rot.y);
    self->actor.world.pos.z += self->actor.speedXZ * Math_CosS(self->actor.world.rot.y);
    if (player->stateFlags2 & 0x80) {
        player->unk_850 = 0;
        player->actor.world.pos = self->actor.world.pos;
        player->actor.shape.rot.y = self->actor.shape.rot.y;
    }
}

void BossSst_HandSetupCrush(BossSst* self) {
    Animation_MorphToLoop(&self->skelAnime, sHandClenchAnims[self->actor.params], -10.0f);
    self->timer = 20;
    self->actionFunc = BossSst_HandCrush;
}

void BossSst_HandCrush(BossSst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    if (!(player->stateFlags2 & 0x80)) {
        BossSst_HandReleasePlayer(self, globalCtx, true);
        BossSst_HandSetupEndCrush(self);
    } else {
        player->actor.world.pos = self->actor.world.pos;
        if (self->timer == 0) {
            self->timer = 20;
            if (!LINK_IS_ADULT) {
                func_8002F7DC(&player->actor, NA_SE_VO_LI_DAMAGE_S_KID);
            } else {
                func_8002F7DC(&player->actor, NA_SE_VO_LI_DAMAGE_S);
            }

            globalCtx->damagePlayer(globalCtx, -8);
        }
        if (Animation_OnFrame(&self->skelAnime, 0.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_CATCH);
        }
    }
}

void BossSst_HandSetupEndCrush(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 10.0f);
    self->actionFunc = BossSst_HandEndCrush;
}

void BossSst_HandEndCrush(BossSst* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        BossSst_HandSetupRetreat(self);
    }
}

void BossSst_HandSetupSwing(BossSst* self) {
    self->amplitude = -0x4000;
    self->timer = 1;
    self->center.x = self->actor.world.pos.x - (Math_SinS(self->actor.shape.rot.y) * 200.0f);
    self->center.y = self->actor.world.pos.y;
    self->center.z = self->actor.world.pos.z - (Math_CosS(self->actor.shape.rot.y) * 200.0f);
    self->actionFunc = BossSst_HandSwing;
}

void BossSst_HandSwing(BossSst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 offXZ;

    if (Math_ScaledStepToS(&self->actor.shape.rot.x, self->amplitude, self->timer * 0xE4 + 0x1C8)) {
        if (self->amplitude != 0) {
            self->amplitude = 0;
            if (self->timer == 4) {
                Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 4.0f);
            }
        } else {
            if (self->timer == 4) {
                player->actor.shape.rot.x = 0;
                player->actor.shape.rot.z = 0;
                BossSst_HandSetupRetreat(self);
                return;
            }
            self->amplitude = (self->timer == 3) ? -0x6000 : -0x4000;
            self->timer++;
        }
    }

    self->actor.world.pos.y = (Math_CosS(self->actor.shape.rot.x + 0x4000) * 200.0f) + self->center.y;
    offXZ = Math_SinS(self->actor.shape.rot.x + 0x4000) * 200.0f;
    self->actor.world.pos.x = (Math_SinS(self->actor.shape.rot.y) * offXZ) + self->center.x;
    self->actor.world.pos.z = (Math_CosS(self->actor.shape.rot.y) * offXZ) + self->center.z;
    if (self->timer != 4) {
        self->actor.shape.rot.z = (self->actor.shape.rot.x + 0x4000) * self->vParity;
    } else {
        Math_ScaledStepToS(&self->actor.shape.rot.z, 0, 0x800);
    }

    if (player->stateFlags2 & 0x80) {
        player->unk_850 = 0;
        Math_Vec3f_Copy(&player->actor.world.pos, &self->actor.world.pos);
        player->actor.shape.rot.x = self->actor.shape.rot.x;
        player->actor.shape.rot.z = (self->vParity * -0x4000) + self->actor.shape.rot.z;
    } else {
        Math_ScaledStepToS(&player->actor.shape.rot.x, 0, 0x600);
        Math_ScaledStepToS(&player->actor.shape.rot.z, 0, 0x600);
        player->actor.world.pos.x += 20.0f * Math_SinS(self->actor.shape.rot.y);
        player->actor.world.pos.z += 20.0f * Math_CosS(self->actor.shape.rot.y);
    }

    if ((self->timer == 4) && (self->amplitude == 0) && SkelAnime_Update(&self->skelAnime) &&
        (player->stateFlags2 & 0x80)) {
        BossSst_HandReleasePlayer(self, globalCtx, false);
        player->actor.world.pos.x += 70.0f * Math_SinS(self->actor.shape.rot.y);
        player->actor.world.pos.z += 70.0f * Math_CosS(self->actor.shape.rot.y);
        func_8002F71C(globalCtx, &self->actor, 15.0f, self->actor.shape.rot.y, 2.0f);
        func_8002F7DC(&player->actor, NA_SE_PL_BODY_HIT);
    }

    func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
}

void BossSst_HandSetupReel(BossSst* self) {
    HAND_STATE(self) = HAND_DAMAGED;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 4.0f);
    self->timer = 36;
    Math_Vec3f_Copy(&self->center, &self->actor.world.pos);
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 200);
    self->actionFunc = BossSst_HandReel;
}

void BossSst_HandReel(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    if (!(self->timer % 4)) {
        if (self->timer % 8) {
            Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 4.0f);
        } else {
            Animation_MorphToPlayOnce(&self->skelAnime, sHandFistPoses[self->actor.params], 6.0f);
        }
    }

    self->actor.colorFilterTimer = 200;
    self->actor.world.pos.x += Rand_CenteredFloat(20.0f);
    self->actor.world.pos.y += Rand_CenteredFloat(20.0f);
    self->actor.world.pos.z += Rand_CenteredFloat(20.0f);

    if (self->actor.world.pos.y < (self->actor.floorHeight + 100.0f)) {
        Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight + 100.0f, 20.0f);
    }

    if (self->timer == 0) {
        BossSst_HandSetupReadyShake(self);
    }
}

void BossSst_HandSetupReadyShake(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandDamagePoses[self->actor.params], 8.0f);
    self->actionFunc = BossSst_HandReadyShake;
}

void BossSst_HandReadyShake(BossSst* self, GlobalContext* globalCtx) {
    f32 diff;
    s32 inPosition;

    diff = Math_SmoothStepToF(&self->actor.world.pos.x, self->actor.home.pos.x, 0.5f, 25.0f, 1.0f);
    diff += Math_SmoothStepToF(&self->actor.world.pos.z, self->actor.home.pos.z, 0.5f, 25.0f, 1.0f);
    diff += Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 200.0f, 0.2f, 30.0f, 1.0f);
    inPosition = Math_ScaledStepToS(&self->actor.shape.rot.x, 0x4000, 0x400);
    inPosition &= Math_ScaledStepToS(&self->actor.shape.rot.z, 0, 0x1000);
    inPosition &= Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, 0x800);
    inPosition &= Math_StepToS(&self->handZPosMod, -0x5DC, 0x1F4);
    inPosition &= Math_ScaledStepToS(&self->handYRotMod, self->vParity * -0x2000, 0x800);
    self->actor.colorFilterTimer = 200;
    if ((diff < 30.0f) && inPosition) {
        BossSst_HandSetupShake(self);
    } else {
        func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
    }
}

void BossSst_HandSetupShake(BossSst* self) {
    self->timer = 200;
    self->actionFunc = BossSst_HandShake;
}

void BossSst_HandShake(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.shape.rot.x = 0x4000 + (sinf(self->timer * (M_PI / 5)) * 0x2000);
    self->handYRotMod = (self->vParity * -0x2000) + (sinf(self->timer * (M_PI / 4)) * 0x2800);

    if (!(self->timer % 8)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_SHAKEHAND);
    }

    if (HAND_STATE(OTHER_HAND(self)) == HAND_DAMAGED) {
        if ((OTHER_HAND(self)->actionFunc == BossSst_HandShake) ||
            (OTHER_HAND(self)->actionFunc == BossSst_HandReadyCharge)) {
            BossSst_HandSetupReadyCharge(self);
        } else if (self->timer == 0) {
            self->timer = 80;
        }
    } else if (self->timer == 0) {
        self->actor.flags |= 1;
        BossSst_HandSetupSlam(self);
    }
}

void BossSst_HandSetupReadyCharge(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFistPoses[self->actor.params], 10.0f);
    self->ready = false;
    self->actionFunc = BossSst_HandReadyCharge;
}

void BossSst_HandReadyCharge(BossSst* self, GlobalContext* globalCtx) {
    if (!self->ready) {
        self->ready = SkelAnime_Update(&self->skelAnime);
        self->ready &= Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x800);
        self->ready &=
            Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y + (self->vParity * 0x1000), 0x800);
        self->ready &= Math_ScaledStepToS(&self->handYRotMod, 0, 0x800);
        self->ready &= Math_ScaledStepToS(&self->actor.shape.rot.z, self->vParity * 0x2800, 0x800);
        self->ready &= Math_StepToS(&self->handZPosMod, -0xDAC, 0x1F4);
        if (self->ready) {
            self->actor.colorFilterTimer = 0;
        }
    } else if (self->colliderJntSph.base.atFlags & AT_HIT) {
        self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        OTHER_HAND(self)->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        sHead->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
        func_8002F71C(globalCtx, &self->actor, 10.0f, self->actor.shape.rot.y, 5.0f);
        func_8002F7DC(&GET_PLAYER(globalCtx)->actor, NA_SE_PL_BODY_HIT);
    }
}

void BossSst_HandSetupStunned(BossSst* hand) {
    Animation_MorphToPlayOnce(&hand->skelAnime, sHandIdleAnims[hand->actor.params], 10.0f);
    if (hand->actionFunc != BossSst_HandDamage) {
        hand->ready = false;
    }

    hand->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
    hand->colliderJntSph.base.acFlags |= AC_ON;
    BossSst_HandSetInvulnerable(hand, true);
    Actor_SetColorFilter(&hand->actor, 0, 0xFF, 0, Animation_GetLastFrame(&gBongoHeadKnockoutAnim));
    hand->actionFunc = BossSst_HandStunned;
}

void BossSst_HandStunned(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ApproachF(&self->actor.world.pos.z, (Math_CosS(sHead->actor.shape.rot.y) * 200.0f) + self->actor.home.pos.z,
                   0.5f, 25.0f);
    Math_ApproachF(&self->actor.world.pos.x, (Math_SinS(sHead->actor.shape.rot.y) * 200.0f) + self->actor.home.pos.x,
                   0.5f, 25.0f);
    if (!self->ready) {
        Math_ScaledStepToS(&self->handYRotMod, 0, 0x800);
        Math_StepToS(&self->handZPosMod, -0xDAC, 0x1F4);
        Math_ScaledStepToS(&self->actor.shape.rot.x, self->actor.home.rot.x, 0x800);
        Math_ScaledStepToS(&self->actor.shape.rot.z, self->actor.home.rot.z, 0x800);
        Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y, 0x800);
        if (sHead->actionFunc == BossSst_HeadVulnerable) {
            self->ready = true;
            Animation_MorphToPlayOnce(&self->skelAnime, sHandDamagePoses[self->actor.params], 10.0f);
        }
    } else {
        Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, 30.0f);
    }
}

void BossSst_HandSetupDamage(BossSst* hand) {
    hand->actor.shape.rot.x = 0;
    Animation_MorphToPlayOnce(&hand->skelAnime, sHandOpenPoses[hand->actor.params], 3.0f);
    hand->timer = 6;
    hand->actionFunc = BossSst_HandDamage;
}

void BossSst_HandDamage(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    SkelAnime_Update(&self->skelAnime);
    if (self->timer >= 2) {
        self->actor.shape.rot.x -= 0x200;
        Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight + 200.0f, 50.0f);
    } else {
        self->actor.shape.rot.x += 0x400;
        Math_StepToF(&self->actor.world.pos.y, self->actor.floorHeight, 100.0f);
    }

    if (self->timer == 0) {
        if (self->actor.floorHeight >= 0.0f) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_TAIKO_HIGH);
        }
        BossSst_HandSetupStunned(self);
    }
}

void BossSst_HandSetupThrash(BossSst* self) {
    HAND_STATE(self) = HAND_DEATH;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 2.0f);
    self->actor.shape.rot.x = 0;
    self->timer = 160;
    if (self->actor.params == BONGO_LEFT_HAND) {
        self->amplitude = -0x800;
    } else {
        self->amplitude = 0;
        self->actor.shape.rot.x = -0x800;
    }

    self->handAngSpeed = 0x180;
    self->actionFunc = BossSst_HandThrash;
}

void BossSst_HandThrash(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    SkelAnime_Update(&self->skelAnime);
    Math_ApproachF(&self->actor.world.pos.z, (Math_CosS(sHead->actor.shape.rot.y) * 200.0f) + self->actor.home.pos.z,
                   0.5f, 25.0f);
    Math_ApproachF(&self->actor.world.pos.x, (Math_SinS(sHead->actor.shape.rot.y) * 200.0f) + self->actor.home.pos.x,
                   0.5f, 25.0f);
    if (Math_ScaledStepToS(&self->actor.shape.rot.x, self->amplitude, self->handAngSpeed)) {
        if (self->amplitude != 0) {
            self->amplitude = 0;
            Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 5.0f);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_TAIKO_HIGH);
            self->amplitude = -0x800;
            Animation_MorphToPlayOnce(&self->skelAnime, sHandOpenPoses[self->actor.params], 5.0f);
        }

        if (self->timer < 80.0f) {
            self->handAngSpeed -= 0x40;
            self->handAngSpeed = CLAMP_MIN(self->handAngSpeed, 0x40);
        }
    }

    self->actor.world.pos.y =
        (((self->handAngSpeed / 256.0f) + 0.5f) * 150.0f) * (-1.0f / 0x800) * self->actor.shape.rot.x;
    if (self->timer == 0) {
        BossSst_HandSetupDarken(self);
    }
}

void BossSst_HandSetupDarken(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 5.0f);
    self->actionFunc = BossSst_HandDarken;
}

void BossSst_HandDarken(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, -0x800, self->handAngSpeed);
    Math_StepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 90.0f, 5.0f);
    if (sHead->actionFunc == BossSst_HeadFall) {
        BossSst_HandSetupFall(self);
    }
}

void BossSst_HandSetupFall(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFlatPoses[self->actor.params], 3.0f);
    self->actionFunc = BossSst_HandFall;
}

void BossSst_HandFall(BossSst* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_ScaledStepToS(&self->actor.shape.rot.x, 0, 0x400);
    self->actor.world.pos.y = sHead->actor.world.pos.y + 230.0f;
    if (sHead->actionFunc == BossSst_HeadMelt) {
        BossSst_HandSetupMelt(self);
    }
}

void BossSst_HandSetupMelt(BossSst* self) {
    BossSst_SpawnHandShadow(self);
    self->actor.shape.shadowDraw = NULL;
    self->timer = 80;
    self->actionFunc = BossSst_HandMelt;
}

void BossSst_HandMelt(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.scale.y -= 0.00025f;
    self->actor.scale.x += 0.000025f;
    self->actor.scale.z += 0.000025f;
    self->actor.world.pos.y = ROOM_CENTER_Y + 0.0f;
    if (self->timer == 0) {
        BossSst_HandSetupFinish(self);
    }
}

void BossSst_HandSetupFinish(BossSst* self) {
    self->actor.draw = BossSst_DrawEffect;
    self->timer = 20;
    self->effects[0].status = 0;
    self->actionFunc = BossSst_HandFinish;
}

void BossSst_HandFinish(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        self->effectMode = BONGO_NULL;
    }
}

void BossSst_HandSetupRecover(BossSst* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, sHandPushoffPoses[self->actor.params], 10.0f);
    self->ready = false;
    self->actionFunc = BossSst_HandRecover;
}

void BossSst_HandRecover(BossSst* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->actor.world.pos.y, ROOM_CENTER_Y + 250.0f, 0.5f, 70.0f, 5.0f);
    if (SkelAnime_Update(&self->skelAnime)) {
        if (!self->ready) {
            Animation_MorphToPlayOnce(&self->skelAnime, sHandHangPoses[self->actor.params], 10.0f);
            self->ready = true;
        }
    }
    func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
}

void BossSst_HandSetupFrozen(BossSst* self) {
    s32 i;

    HAND_STATE(self) = HAND_FROZEN;
    Math_Vec3f_Copy(&self->center, &self->actor.world.pos);
    BossSst_HandSetupReadyBreakIce(OTHER_HAND(self));
    self->ready = false;
    self->effectMode = BONGO_ICE;
    self->timer = 35;
    for (i = 0; i < 18; i++) {
        self->effects[i].move = false;
    }

    BossSst_SpawnIceCrystal(self, 0);
    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0xA);
    self->handAngSpeed = 0;
    self->actionFunc = BossSst_HandFrozen;
}

void BossSst_HandFrozen(BossSst* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if ((self->timer % 2) != 0) {
        BossSst_SpawnIceCrystal(self, (self->timer >> 1) + 1);
    }

    if (self->ready) {
        BossSst_IceShatter(self);
        BossSst_HandSetupRetreat(self);
        sHead->ready = true;
    } else {
        self->actor.colorFilterTimer = 10;
        if (self->handAngSpeed != 0) {
            f32 offY = Math_SinS(OTHER_HAND(self)->actor.shape.rot.x) * 5.0f;
            f32 offXZ = Math_CosS(OTHER_HAND(self)->actor.shape.rot.x) * 5.0f;

            if ((self->handAngSpeed % 2) != 0) {
                offY *= -1.0f;
                offXZ *= -1.0f;
            }

            self->actor.world.pos.x = self->center.x + (Math_CosS(OTHER_HAND(self)->actor.shape.rot.y) * offXZ);
            self->actor.world.pos.y = self->center.y + offY;
            self->actor.world.pos.z = self->center.z + (Math_SinS(OTHER_HAND(self)->actor.shape.rot.y) * offXZ);
            self->handAngSpeed--;
        }
    }
}

void BossSst_HandSetupReadyBreakIce(BossSst* self) {
    HAND_STATE(self) = HAND_BREAK_ICE;
    Animation_MorphToPlayOnce(&self->skelAnime, sHandFistPoses[self->actor.params], 5.0f);
    self->ready = false;
    self->actor.colorFilterTimer = 0;
    if (self->effectMode == BONGO_ICE) {
        self->effectMode = BONGO_NULL;
    }

    self->radius = Actor_WorldDistXZToPoint(&self->actor, &OTHER_HAND(self)->center);
    self->targetYaw = Actor_WorldYawTowardPoint(&self->actor, &OTHER_HAND(self)->center);
    BossSst_HandSetInvulnerable(self, true);
    self->actionFunc = BossSst_HandReadyBreakIce;
}

void BossSst_HandReadyBreakIce(BossSst* self, GlobalContext* globalCtx) {
    s32 inPosition;

    inPosition = Math_ScaledStepToS(&self->actor.shape.rot.y, self->targetYaw, 0x400);
    inPosition &= Math_ScaledStepToS(&self->actor.shape.rot.x, 0x1000, 0x400);
    inPosition &= Math_ScaledStepToS(&self->actor.shape.rot.z, 0, 0x800);
    inPosition &= Math_ScaledStepToS(&self->handYRotMod, 0, 0x400);
    inPosition &= Math_StepToF(&self->actor.world.pos.y, OTHER_HAND(self)->center.y + 200.0f, 50.0f);
    inPosition &= Math_StepToF(&self->radius, 400.0f, 60.0f);
    self->actor.world.pos.x = OTHER_HAND(self)->center.x - (Math_SinS(self->targetYaw) * self->radius);
    self->actor.world.pos.z = OTHER_HAND(self)->center.z - (Math_CosS(self->targetYaw) * self->radius);
    if (SkelAnime_Update(&self->skelAnime) && inPosition) {
        BossSst_HandSetupBreakIce(self);
    }
}

void BossSst_HandSetupBreakIce(BossSst* self) {
    self->timer = 9;
    self->actionFunc = BossSst_HandBreakIce;
    self->actor.speedXZ = 0.5f;
}

void BossSst_HandBreakIce(BossSst* self, GlobalContext* globalCtx) {
    if ((self->timer % 2) != 0) {
        self->actor.speedXZ *= 1.5f;
        self->actor.speedXZ = CLAMP_MAX(self->actor.speedXZ, 60.0f);

        if (Math_StepToF(&self->radius, 100.0f, self->actor.speedXZ)) {
            BossSst_SpawnIceShard(self);
            if (self->timer != 0) {
                self->timer--;
            }

            if (self->timer != 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_ICE_BROKEN);
            }

            OTHER_HAND(self)->handAngSpeed = 5;
        }
    } else {
        self->actor.speedXZ *= 0.8f;
        Math_StepToF(&self->radius, 500.0f, self->actor.speedXZ);
        if (self->actor.speedXZ < 2.0f) {
            if (self->timer != 0) {
                self->timer--;
            }
        }
    }

    self->actor.world.pos.x = OTHER_HAND(self)->center.x - (Math_SinS(self->targetYaw) * self->radius);
    self->actor.world.pos.z = OTHER_HAND(self)->center.z - (Math_CosS(self->targetYaw) * self->radius);
    self->actor.world.pos.y = OTHER_HAND(self)->center.y + (self->radius * 0.4f);
    if (self->timer == 0) {
        OTHER_HAND(self)->ready = true;
        BossSst_HandSetupRetreat(self);
    }

    func_8002F974(&self->actor, NA_SE_EN_SHADEST_HAND_FLY - SFX_FLAG);
}

void BossSst_HandGrabPlayer(BossSst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (globalCtx->grabPlayer(globalCtx, player)) {
        player->actor.parent = &self->actor;
        if (player->actor.colChkInfo.health > 0) {
            self->colliderJntSph.base.ocFlags1 &= ~OC1_ON;
            if (HAND_STATE(self) == HAND_CLAP) {
                OTHER_HAND(self)->colliderJntSph.base.ocFlags1 &= ~OC1_ON;
            }
        }
    }
}

void BossSst_HandReleasePlayer(BossSst* self, GlobalContext* globalCtx, s32 dropPlayer) {
    Player* player = GET_PLAYER(globalCtx);

    if (player->actor.parent == &self->actor) {
        player->actor.parent = NULL;
        player->unk_850 = 100;
        self->colliderJntSph.base.ocFlags1 |= OC1_ON;
        OTHER_HAND(self)->colliderJntSph.base.ocFlags1 |= OC1_ON;
        if (dropPlayer) {
            func_8002F71C(globalCtx, &self->actor, 0.0f, self->actor.shape.rot.y, 0.0f);
        }
    }
}

void BossSst_MoveAround(BossSst* self) {
    BossSst* hand;
    Vec3f* vec;
    f32 sn;
    f32 cs;
    s32 i;

    sn = Math_SinS(self->actor.shape.rot.y);
    cs = Math_CosS(self->actor.shape.rot.y);
    if (self->actionFunc != BossSst_HeadEndCharge) {
        self->actor.world.pos.x = sRoomCenter.x + (self->radius * sn);
        self->actor.world.pos.z = sRoomCenter.z + (self->radius * cs);
    }

    for (i = 0; i < 2; i++) {
        hand = sHands[i];
        vec = &sHandOffsets[i];

        hand->actor.world.pos.x = self->actor.world.pos.x + (vec->z * sn) + (vec->x * cs);
        hand->actor.world.pos.y = self->actor.world.pos.y + vec->y;
        hand->actor.world.pos.z = self->actor.world.pos.z + (vec->z * cs) - (vec->x * sn);

        hand->actor.home.pos.x = self->actor.world.pos.x + (400.0f * sn) + (-200.0f * hand->vParity * cs);
        hand->actor.home.pos.y = self->actor.world.pos.y;
        hand->actor.home.pos.z = self->actor.world.pos.z + (400.0f * cs) - (-200.0f * hand->vParity * sn);

        hand->actor.home.rot.y = self->actor.shape.rot.y;
        hand->actor.shape.rot.y = sHandYawOffsets[i] + self->actor.shape.rot.y;

        if (hand->actor.world.pos.y < hand->actor.floorHeight) {
            hand->actor.world.pos.y = hand->actor.floorHeight;
        }
    }
}

void BossSst_HandSelectAttack(BossSst* self) {
    f32 rand = Rand_ZeroOne() * 6.0f;
    s32 randInt;

    if (HAND_STATE(OTHER_HAND(self)) == HAND_DAMAGED) {
        rand *= 5.0f / 6;
        if (rand > 4.0f) {
            rand = 4.0f;
        }
    }

    randInt = rand;
    if (randInt == 0) {
        BossSst_HandSetupReadySlam(self);
    } else if (randInt == 1) {
        BossSst_HandSetupReadySweep(self);
    } else if (randInt == 2) {
        BossSst_HandSetupReadyPunch(self);
    } else if (randInt == 5) {
        BossSst_HandSetupReadyClap(self);
    } else { // randInt == 3 || randInt == 4
        BossSst_HandSetupReadyGrab(self);
    }
}

void BossSst_HandSetDamage(BossSst* self, s32 damage) {
    s32 i;

    self->colliderJntSph.base.atFlags |= AT_ON;
    for (i = 0; i < 11; i++) {
        self->colliderJntSph.elements[i].info.toucher.damage = damage;
    }
}

void BossSst_HandSetInvulnerable(BossSst* self, s32 isInv) {
    self->colliderJntSph.base.acFlags &= ~AC_HIT;
    if (isInv) {
        self->colliderJntSph.base.colType = COLTYPE_HARD;
        self->colliderJntSph.base.acFlags |= AC_HARD;
    } else {
        self->colliderJntSph.base.colType = COLTYPE_HIT0;
        self->colliderJntSph.base.acFlags &= ~AC_HARD;
    }
}

void BossSst_HeadSfx(BossSst* self, u16 sfxId) {
    func_80078914(&self->center, sfxId);
}

void BossSst_HandCollisionCheck(BossSst* self, GlobalContext* globalCtx) {
    if ((self->colliderJntSph.base.acFlags & AC_HIT) && (self->colliderJntSph.base.colType != COLTYPE_HARD)) {
        s32 bothHands = true;

        self->colliderJntSph.base.acFlags &= ~AC_HIT;
        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            self->colliderJntSph.base.atFlags &= ~(AT_ON | AT_HIT);
            self->colliderJntSph.base.acFlags &= ~AC_ON;
            self->colliderJntSph.base.ocFlags1 &= ~OC1_NO_PUSH;
            BossSst_HandReleasePlayer(self, globalCtx, true);
            if (HAND_STATE(OTHER_HAND(self)) == HAND_CLAP) {
                BossSst_HandReleasePlayer(OTHER_HAND(self), globalCtx, true);
                BossSst_HandSetupRetreat(OTHER_HAND(self));
            }

            self->actor.flags &= ~1;
            if (self->actor.colChkInfo.damageEffect == 3) {
                BossSst_HandSetupFrozen(self);
            } else {
                BossSst_HandSetupReel(self);
                if (HAND_STATE(OTHER_HAND(self)) != HAND_DAMAGED) {
                    bothHands = false;
                }
            }

            BossSst_HeadSetupDamagedHand(sHead, bothHands);
            Item_DropCollectible(globalCtx, &self->actor.world.pos,
                                 (Rand_ZeroOne() < 0.5f) ? ITEM00_ARROWS_SMALL : ITEM00_MAGIC_SMALL);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_DAMAGE_HAND);
        }
    }
}

void BossSst_HeadCollisionCheck(BossSst* self, GlobalContext* globalCtx) {
    if (self->colliderCyl.base.acFlags & AC_HIT) {
        self->colliderCyl.base.acFlags &= ~AC_HIT;
        if ((self->actor.colChkInfo.damageEffect != 0) || (self->actor.colChkInfo.damage != 0)) {
            if (self->actionFunc == BossSst_HeadVulnerable) {
                if (Actor_ApplyDamage(&self->actor) == 0) {
                    Enemy_StartFinishingBlow(globalCtx, &self->actor);
                    BossSst_HeadSetupDeath(self, globalCtx);
                } else {
                    BossSst_HeadSetupDamage(self);
                }

                BossSst_HandSetupDamage(sHands[LEFT]);
                BossSst_HandSetupDamage(sHands[RIGHT]);
            } else {
                BossSst_HeadSetupStunned(self);
                if (HAND_STATE(sHands[RIGHT]) == HAND_FROZEN) {
                    BossSst_IceShatter(sHands[RIGHT]);
                } else if (HAND_STATE(sHands[LEFT]) == HAND_FROZEN) {
                    BossSst_IceShatter(sHands[LEFT]);
                }

                BossSst_HandSetupStunned(sHands[RIGHT]);
                BossSst_HandSetupStunned(sHands[LEFT]);
            }
        }
    }
}

void BossSst_UpdateHand(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossSst* self = THIS;
    BossSstHandTrail* trail;

    if (self->colliderCyl.base.atFlags & AT_ON) {
        if ((self->effects[0].move < 5) ||
            (self->actor.xzDistToPlayer < ((self->effects[2].scale * 0.01f) * sCylinderInitHand.dim.radius)) ||
            (self->colliderCyl.base.atFlags & AT_HIT)) {
            self->colliderCyl.base.atFlags &= ~(AT_ON | AT_HIT);
        } else {
            self->colliderCyl.dim.radius = (self->effects[0].scale * 0.01f) * sCylinderInitHand.dim.radius;
        }
    }

    BossSst_HandCollisionCheck(self, globalCtx);
    self->actionFunc(self, globalCtx);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 130.0f, 0.0f, 5);
    Actor_SetFocus(&self->actor, 0.0f);
    if (self->colliderJntSph.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
    }

    if ((sHead->actionFunc != BossSst_HeadLurk) && (sHead->actionFunc != BossSst_HeadIntro) &&
        (self->colliderJntSph.base.acFlags & AC_ON)) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
    }

    if (self->colliderJntSph.base.ocFlags1 & OC1_ON) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
    }

    if (self->colliderCyl.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderCyl.base);
    }

    if ((HAND_STATE(self) != HAND_DEATH) && (HAND_STATE(self) != HAND_WAIT) && (HAND_STATE(self) != HAND_BEAT) &&
        (HAND_STATE(self) != HAND_FROZEN)) {
        self->trailCount++;
        self->trailCount = CLAMP_MAX(self->trailCount, 7);
    } else {
        self->trailCount--;
        self->trailCount = CLAMP_MIN(self->trailCount, 0);
    }

    trail = &self->handTrails[self->trailIndex];
    Math_Vec3f_Copy(&trail->world.pos, &self->actor.world.pos);
    trail->world.rot = self->actor.shape.rot;
    trail->zPosMod = self->handZPosMod;
    trail->yRotMod = self->handYRotMod;

    self->trailIndex = (self->trailIndex + 1) % 7;
    BossSst_UpdateEffect(&self->actor, globalCtx);
}

void BossSst_UpdateHead(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossSst* self = THIS;

    func_8002DBD0(&self->actor, &sHandOffsets[RIGHT], &sHands[RIGHT]->actor.world.pos);
    func_8002DBD0(&self->actor, &sHandOffsets[LEFT], &sHands[LEFT]->actor.world.pos);

    sHandYawOffsets[LEFT] = sHands[LEFT]->actor.shape.rot.y - thisx->shape.rot.y;
    sHandYawOffsets[RIGHT] = sHands[RIGHT]->actor.shape.rot.y - thisx->shape.rot.y;

    BossSst_HeadCollisionCheck(self, globalCtx);
    self->actionFunc(self, globalCtx);
    if (self->vVanish) {
        if ((globalCtx->actorCtx.unk_03 == 0) || (thisx->colorFilterTimer != 0)) {
            self->actor.flags &= ~0x80;
        } else {
            self->actor.flags |= 0x80;
        }
    }

    if (self->colliderJntSph.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
    }

    if ((self->actionFunc != BossSst_HeadLurk) && (self->actionFunc != BossSst_HeadIntro)) {
        if (self->colliderCyl.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderCyl.base);
        }
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
    }

    if (self->colliderJntSph.base.ocFlags1 & OC1_ON) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
    }

    BossSst_MoveAround(self);
    if ((!self->vVanish || ((self->actor.flags & 0x80) == 0x80)) &&
        ((self->actionFunc == BossSst_HeadReadyCharge) || (self->actionFunc == BossSst_HeadCharge) ||
         (self->actionFunc == BossSst_HeadFrozenHand) || (self->actionFunc == BossSst_HeadStunned) ||
         (self->actionFunc == BossSst_HeadVulnerable) || (self->actionFunc == BossSst_HeadDamage))) {
        self->actor.flags |= 1;
    } else {
        self->actor.flags &= ~1;
    }

    if (self->actionFunc == BossSst_HeadCharge) {
        BossSst_HeadSfx(self, NA_SE_EN_SHADEST_MOVE - SFX_FLAG);
    }

    BossSst_UpdateEffect(&self->actor, globalCtx);
}

s32 BossSst_OverrideHandDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                             void* thisx) {
    BossSst* self = THIS;

    if (limbIndex == 1) {
        pos->z += self->handZPosMod;
        rot->y += self->handYRotMod;
    }
    return false;
}

void BossSst_PostHandDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    BossSst* self = THIS;

    Collider_UpdateSpheres(limbIndex, &self->colliderJntSph);
}

s32 BossSst_OverrideHandTrailDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                  void* data, Gfx** gfx) {
    BossSstHandTrail* trail = (BossSstHandTrail*)data;

    if (limbIndex == 1) {
        pos->z += trail->zPosMod;
        rot->y += trail->yRotMod;
    }
    return false;
}

void BossSst_DrawHand(Actor* thisx, GlobalContext* globalCtx) {
    BossSst* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_sst.c", 6563);

    func_80093D18(globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, sBodyColor.r, sBodyColor.g, sBodyColor.b, 255);

    if (!sBodyStatic) {
        gSPSegment(POLY_OPA_DISP++, 0x08, &D_80116280[2]);
    } else {
        gDPSetEnvColor(POLY_OPA_DISP++, sStaticColor.r, sStaticColor.g, sStaticColor.b, 0);
        gSPSegment(POLY_OPA_DISP++, 0x08, sBodyStaticDList);
    }

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          BossSst_OverrideHandDraw, BossSst_PostHandDraw, self);
    if (self->trailCount >= 2) {
        BossSstHandTrail* trail;
        BossSstHandTrail* trail2;
        s32 i;
        s32 idx;
        s32 end;
        s32 pad;

        func_80093D84(globalCtx->state.gfxCtx);

        end = self->trailCount >> 1;
        idx = (self->trailIndex + 4) % 7;
        trail = &self->handTrails[idx];
        trail2 = &self->handTrails[(idx + 2) % 7];

        for (i = 0; i < end; i++) {
            if (Math3D_Vec3fDistSq(&trail2->world.pos, &trail->world.pos) > 900.0f) {
                func_800D1694(trail->world.pos.x, trail->world.pos.y, trail->world.pos.z, &trail->world.rot);
                Matrix_Scale(0.02f, 0.02f, 0.02f, MTXMODE_APPLY);

                gSPSegment(POLY_XLU_DISP++, 0x08, sHandTrailDList);
                gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, ((3 - i) * 10) + 20, 0, ((3 - i) * 20) + 50,
                                ((3 - i) * 30) + 70);

                POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                                   self->skelAnime.dListCount, BossSst_OverrideHandTrailDraw, NULL,
                                                   trail, POLY_XLU_DISP);
            }
            idx = (idx + 5) % 7;
            trail2 = trail;
            trail = &self->handTrails[idx];
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_sst.c", 6654);

    BossSst_DrawEffect(&self->actor, globalCtx);
}

s32 BossSst_OverrideHeadDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                             Gfx** gfx) {
    BossSst* self = THIS;
    s32 shakeAmp;
    s32 pad;
    s32 timer12;
    f32 shakeMod;

    if (((self->actor.flags & 0x80) != 0x80) && self->vVanish) {
        *dList = NULL;
    } else if (self->actionFunc == BossSst_HeadThrash) { // Animation modifications for death cutscene
        shakeAmp = (self->timer / 10) + 1;
        if ((limbIndex == 3) || (limbIndex == 39) || (limbIndex == 42)) {

            shakeMod = sinf(self->timer * (M_PI / 5));
            rot->x += ((0x500 * Rand_ZeroOne() + 0xA00) / 0x10) * shakeAmp * shakeMod;

            shakeMod = sinf((self->timer % 5) * (M_PI / 5));
            rot->z -= ((0x800 * Rand_ZeroOne() + 0x1000) / 0x10) * shakeAmp * shakeMod + 0x1000;

            if (limbIndex == 3) {

                shakeMod = sinf(self->timer * (M_PI / 5));
                rot->y += ((0x500 * Rand_ZeroOne() + 0xA00) / 0x10) * shakeAmp * shakeMod;
            }
        } else if ((limbIndex == 5) || (limbIndex == 6)) {

            shakeMod = sinf((self->timer % 5) * (M_PI / 5));
            rot->z -= ((0x280 * Rand_ZeroOne() + 0x500) / 0x10) * shakeAmp * shakeMod + 0x500;

            if (limbIndex == 5) {

                shakeMod = sinf(self->timer * (M_PI / 5));
                rot->x += ((0x500 * Rand_ZeroOne() + 0xA00) / 0x10) * shakeAmp * shakeMod;

                shakeMod = sinf(self->timer * (M_PI / 5));
                rot->y += ((0x500 * Rand_ZeroOne() + 0xA00) / 0x10) * shakeAmp * shakeMod;
            }
        } else if (limbIndex == 2) {
            shakeMod = sinf(self->timer * (M_PI / 5));
            rot->x += ((0x200 * Rand_ZeroOne() + 0x400) / 0x10) * shakeAmp * shakeMod;

            shakeMod = sinf(self->timer * (M_PI / 5));
            rot->y += ((0x200 * Rand_ZeroOne() + 0x400) / 0x10) * shakeAmp * shakeMod;

            shakeMod = sinf((self->timer % 5) * (M_PI / 5));
            rot->z -= ((0x100 * Rand_ZeroOne() + 0x200) / 0x10) * shakeAmp * shakeMod + 0x200;
        }
    } else if (self->actionFunc == BossSst_HeadDeath) {
        if (self->timer > 48) {
            timer12 = self->timer - 36;
        } else {
            pad = ((self->timer > 6) ? 6 : self->timer);
            timer12 = pad * 2;
        }

        if ((limbIndex == 3) || (limbIndex == 39) || (limbIndex == 42)) {
            rot->z -= 0x2000 * sinf(timer12 * (M_PI / 24));
        } else if ((limbIndex == 5) || (limbIndex == 6)) {
            rot->z -= 0xA00 * sinf(timer12 * (M_PI / 24));
        } else if (limbIndex == 2) {
            rot->z -= 0x400 * sinf(timer12 * (M_PI / 24));
        }
    } else if ((self->actionFunc == BossSst_HeadDarken) || (self->actionFunc == BossSst_HeadFall) ||
               (self->actionFunc == BossSst_HeadMelt)) {
        if ((limbIndex == 3) || (limbIndex == 39) || (limbIndex == 42)) {
            rot->z -= 0x1000;
        } else if ((limbIndex == 5) || (limbIndex == 6)) {
            rot->z -= 0x500;
        } else if (limbIndex == 2) {
            rot->z -= 0x200;
        }
    }
    return false;
}

void BossSst_PostHeadDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    static Vec3f headVec = { 1000.0f, 0.0f, 0.0f };
    BossSst* self = THIS;
    Vec3f headPos;

    if (limbIndex == 8) {
        Matrix_MultVec3f(&zeroVec, &self->actor.focus.pos);
        Matrix_MultVec3f(&headVec, &headPos);
        self->colliderCyl.dim.pos.x = headPos.x;
        self->colliderCyl.dim.pos.y = headPos.y;
        self->colliderCyl.dim.pos.z = headPos.z;
    }

    Collider_UpdateSpheres(limbIndex, &self->colliderJntSph);
}

void BossSst_DrawHead(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossSst* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_sst.c", 6810);

    if ((self->actor.flags & 0x80) != 0x80) {
        func_80093D18(globalCtx->state.gfxCtx);
        gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x80, sBodyColor.r, sBodyColor.g, sBodyColor.b, 255);
        if (!sBodyStatic) {
            gSPSegment(POLY_OPA_DISP++, 0x08, &D_80116280[2]);
        } else {
            gDPSetEnvColor(POLY_OPA_DISP++, sStaticColor.r, sStaticColor.g, sStaticColor.b, 0);
            gSPSegment(POLY_OPA_DISP++, 0x08, sBodyStaticDList);
        }
    } else {
        func_80093D84(globalCtx->state.gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, 255);
        gSPSegment(POLY_XLU_DISP++, 0x08, &D_80116280[2]);
    }

    if (self->actionFunc == BossSst_HeadThrash) {
        f32 randPitch = Rand_ZeroOne() * (2 * M_PI);
        f32 randYaw = Rand_ZeroOne() * (2 * M_PI);

        Matrix_RotateY(randYaw, MTXMODE_APPLY);
        Matrix_RotateX(randPitch, MTXMODE_APPLY);
        Matrix_Scale((self->timer * 0.000375f) + 1.0f, 1.0f - (self->timer * 0.00075f),
                     (self->timer * 0.000375f) + 1.0f, MTXMODE_APPLY);
        Matrix_RotateX(-randPitch, MTXMODE_APPLY);
        Matrix_RotateY(-randYaw, MTXMODE_APPLY);
    }

    if ((self->actor.flags & 0x80) != 0x80) {
        POLY_OPA_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           self->skelAnime.dListCount, BossSst_OverrideHeadDraw, BossSst_PostHeadDraw,
                                           self, POLY_OPA_DISP);
    } else {
        POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           self->skelAnime.dListCount, BossSst_OverrideHeadDraw, BossSst_PostHeadDraw,
                                           self, POLY_XLU_DISP);
    }

    if ((self->actionFunc == BossSst_HeadIntro) && (113 >= self->timer) && (self->timer > 20)) {
        s32 yOffset;
        Vec3f vanishMaskPos;
        Vec3f vanishMaskOffset;

        func_80093D84(globalCtx->state.gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, 0, 0, 18, 255);

        yOffset = 113 * 8 - self->timer * 8;
        vanishMaskPos.x = ROOM_CENTER_X + 85.0f;
        vanishMaskPos.y = ROOM_CENTER_Y - 250.0f + yOffset;
        vanishMaskPos.z = ROOM_CENTER_Z + 190.0f;
        if (vanishMaskPos.y > 450.0f) {
            vanishMaskPos.y = 450.0f;
        }

        Matrix_MultVec3fExt(&vanishMaskPos, &vanishMaskOffset, &globalCtx->mf_11DA0);
        Matrix_Translate(self->actor.world.pos.x + vanishMaskOffset.x, self->actor.world.pos.y + vanishMaskOffset.y,
                         self->actor.world.pos.z + vanishMaskOffset.z, MTXMODE_NEW);
        Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_sst.c", 6934),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, sIntroVanishDList);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_sst.c", 6941);

    SkinMatrix_Vec3fMtxFMultXYZ(&globalCtx->mf_11D60, &self->actor.focus.pos, &self->center);
    BossSst_DrawEffect(&self->actor, globalCtx);
}

void BossSst_SpawnHeadShadow(BossSst* self) {
    static Vec3f shadowOffset[] = {
        { 0.0f, 0.0f, 340.0f },
        { -160.0f, 0.0f, 250.0f },
        { 160.0f, 0.0f, 250.0f },
    };
    s32 pad;
    s32 i;
    f32 sn;
    f32 cs;

    self->effectMode = BONGO_SHADOW;
    sn = Math_SinS(self->actor.shape.rot.y);
    cs = Math_CosS(self->actor.shape.rot.y);

    for (i = 0; i < 3; i++) {
        BossSstEffect* shadow = &self->effects[i];
        Vec3f* offset = &shadowOffset[i];

        shadow->pos.x = self->actor.world.pos.x + (sn * offset->z) + (cs * offset->x);
        shadow->pos.y = 0.0f;
        shadow->pos.z = self->actor.world.pos.z + (cs * offset->z) - (sn * offset->x);

        shadow->scale = 1450;
        shadow->alpha = 254;
        shadow->status = 65;
    }

    self->effects[3].status = -1;
}

void BossSst_SpawnHandShadow(BossSst* self) {
    self->effectMode = BONGO_SHADOW;
    self->effects[0].pos.x = self->actor.world.pos.x + (Math_CosS(self->actor.shape.rot.y) * 30.0f * self->vParity);
    self->effects[0].pos.z = self->actor.world.pos.z - (Math_SinS(self->actor.shape.rot.y) * 30.0f * self->vParity);
    self->effects[0].pos.y = self->actor.world.pos.y;
    self->effects[0].scale = 2300;
    self->effects[0].alpha = 254;
    self->effects[0].status = 5;
    self->effects[1].status = -1;
}

void BossSst_SpawnShockwave(BossSst* self) {
    s32 i;
    s32 scale = 120;
    s32 alpha = 250;

    Audio_PlayActorSound2(&self->actor, NA_SE_EN_SHADEST_HAND_WAVE);
    self->effectMode = BONGO_SHOCKWAVE;

    for (i = 0; i < 3; i++) {
        BossSstEffect* shockwave = &self->effects[i];

        Math_Vec3f_Copy(&shockwave->pos, &self->actor.world.pos);
        shockwave->move = (i + 9) * 2;
        shockwave->scale = scale;
        shockwave->alpha = alpha / shockwave->move;
        scale -= 50;
        alpha -= 25;
    }
}

void BossSst_SpawnIceCrystal(BossSst* self, s32 index) {
    BossSstEffect* ice = &self->effects[index];
    Sphere16* sphere;

    if (index < 11) {
        sphere = &self->colliderJntSph.elements[index].dim.worldSphere;

        ice->pos.x = sphere->center.x;
        ice->pos.y = sphere->center.y;
        ice->pos.z = sphere->center.z;
        if (index == 0) {
            ice->pos.x -= 25.0f;
            ice->pos.y -= 25.0f;
            ice->pos.z -= 25.0f;
        }
    } else {
        sphere = &self->colliderJntSph.elements[0].dim.worldSphere;

        ice->pos.x = ((((index - 11) & 1) ? 1 : -1) * 25.0f) + sphere->center.x;
        ice->pos.y = ((((index - 11) & 2) ? 1 : -1) * 25.0f) + sphere->center.y;
        ice->pos.z = ((((index - 11) & 4) ? 1 : -1) * 25.0f) + sphere->center.z;
    }

    ice->pos.x -= self->actor.world.pos.x;
    ice->pos.y -= self->actor.world.pos.y;
    ice->pos.z -= self->actor.world.pos.z;

    ice->status = 0;

    ice->rot.x = Rand_ZeroOne() * 0x10000;
    ice->rot.y = Rand_ZeroOne() * 0x10000;
    ice->rot.z = Rand_ZeroOne() * 0x10000;

    ice->alpha = 120;
    ice->move = true;

    ice->vel.x = (Rand_ZeroOne() * 0.06f + 0.12f) * ice->pos.x;
    ice->vel.y = (Rand_ZeroOne() * 15.0f + 5.0f);
    ice->vel.z = (Rand_ZeroOne() * 0.06f + 0.12f) * ice->pos.z;
    ice->scale = 4000;

    if ((index % 2) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_PL_FREEZE_S);
    }
}

void BossSst_SpawnIceShard(BossSst* self) {
    s32 i;
    Vec3f spawnPos;
    f32 offXZ;

    self->effectMode = BONGO_ICE;
    offXZ = Math_CosS(self->actor.shape.rot.x) * 50.0f;
    spawnPos.x = Math_CosS(self->actor.shape.rot.y) * offXZ + self->actor.world.pos.x;
    spawnPos.y = Math_SinS(self->actor.shape.rot.x) * 50.0f + self->actor.world.pos.y - 10.0f;
    spawnPos.z = Math_SinS(self->actor.shape.rot.y) * offXZ + self->actor.world.pos.z;

    for (i = 0; i < 18; i++) {
        BossSstEffect* ice = &self->effects[i];

        Math_Vec3f_Copy(&ice->pos, &spawnPos);
        ice->status = 1;
        ice->rot.x = Rand_ZeroOne() * 0x10000;
        ice->rot.y = Rand_ZeroOne() * 0x10000;
        ice->rot.z = Rand_ZeroOne() * 0x10000;

        ice->alpha = 120;
        ice->move = true;

        ice->vel.x = Rand_CenteredFloat(20.0f);
        ice->vel.y = Rand_ZeroOne() * 10.0f + 3.0f;
        ice->vel.z = Rand_CenteredFloat(20.0f);

        ice->scale = Rand_ZeroOne() * 200.0f + 400.0f;
    }
}

void BossSst_IceShatter(BossSst* self) {
    s32 i;

    self->effects[0].status = 1;
    Audio_PlayActorSound2(&self->actor, NA_SE_PL_ICE_BROKEN);

    for (i = 0; i < 18; i++) {
        BossSstEffect* ice = &self->effects[i];

        if (ice->move) {
            ice->pos.x += self->actor.world.pos.x;
            ice->pos.y += self->actor.world.pos.y;
            ice->pos.z += self->actor.world.pos.z;
        }
    }
}

void BossSst_UpdateEffect(Actor* thisx, GlobalContext* globalCtx) {
    BossSst* self = THIS;
    BossSstEffect* effect;
    s32 i;

    if (self->effectMode != BONGO_NULL) {
        if (self->effectMode == BONGO_ICE) {
            if (self->effects[0].status) {
                for (i = 0; i < 18; i++) {
                    effect = &self->effects[i];

                    if (effect->move) {
                        effect->pos.x += effect->vel.x;
                        effect->pos.y += effect->vel.y;
                        effect->pos.z += effect->vel.z;
                        effect->alpha -= 3;
                        effect->vel.y -= 1.0f;
                        effect->rot.x += 0xD00;
                        effect->rot.y += 0x1100;
                        effect->rot.z += 0x1500;
                    }
                }
            }
            if (self->effects[0].alpha == 0) {
                self->effectMode = BONGO_NULL;
            }
        } else if (self->effectMode == BONGO_SHOCKWAVE) {
            for (i = 0; i < 3; i++) {
                BossSstEffect* effect2 = &self->effects[i];
                s32 scale = effect2->move * 2;

                effect2->scale += CLAMP_MAX(scale, 20) + i;
                if (effect2->move != 0) {
                    effect2->move--;
                }
            }

            if (self->effects[0].move == 0) {
                self->effectMode = BONGO_NULL;
            }
        } else if (self->effectMode == BONGO_SHADOW) {
            effect = &self->effects[0];

            if (self->actor.params == BONGO_HEAD) {
                SkinMatrix_Vec3fMtxFMultXYZ(&globalCtx->mf_11D60, &self->actor.focus.pos, &self->center);
                BossSst_HeadSfx(self, NA_SE_EN_SHADEST_LAST - SFX_FLAG);
            }
            while (effect->status != -1) {
                if (effect->status == 0) {
                    effect->alpha -= 2;
                } else {
                    effect->scale += effect->status;
                }

                effect->scale = CLAMP_MAX(effect->scale, 10000);
                effect++;
            }
        }
    }
}

void BossSst_DrawEffect(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BossSst* self = THIS;
    s32 i;
    BossSstEffect* effect;

    if (self->effectMode != BONGO_NULL) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_sst.c", 7302);

        func_80093D84(globalCtx->state.gfxCtx);
        if (self->effectMode == BONGO_ICE) {
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, globalCtx->gameplayFrames % 256, 0x20, 0x10, 1,
                                        0, (globalCtx->gameplayFrames * 2) % 256, 0x40, 0x20));
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 50, 100, self->effects[0].alpha);
            gSPDisplayList(POLY_XLU_DISP++, gBongoIceCrystalDL);

            for (i = 0; i < 18; i++) {
                effect = &self->effects[i];
                if (effect->move) {
                    func_8003435C(&effect->pos, globalCtx);
                    if (self->effects[0].status != 0) {
                        Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
                    } else {
                        Matrix_Translate(effect->pos.x + self->actor.world.pos.x,
                                         effect->pos.y + self->actor.world.pos.y,
                                         effect->pos.z + self->actor.world.pos.z, MTXMODE_NEW);
                    }

                    Matrix_RotateRPY(effect->rot.x, effect->rot.y, effect->rot.z, MTXMODE_APPLY);
                    Matrix_Scale(effect->scale * 0.001f, effect->scale * 0.001f, effect->scale * 0.001f, MTXMODE_APPLY);

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_sst.c", 7350),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(POLY_XLU_DISP++, gBongoIceShardDL);
                }
            }
        } else if (self->effectMode == BONGO_SHOCKWAVE) {
            f32 scaleY = 0.005f;

            gDPPipeSync(POLY_XLU_DISP++);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, globalCtx->gameplayFrames % 128, 0, 0x20, 0x40, 1,
                                        0, (globalCtx->gameplayFrames * -15) % 256, 0x20, 0x40));

            for (i = 0; i < 3; i++, scaleY -= 0.001f) {
                effect = &self->effects[i];

                if (effect->move != 0) {
                    Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
                    Matrix_Scale(effect->scale * 0.001f, scaleY, effect->scale * 0.001f, MTXMODE_APPLY);

                    gDPPipeSync(POLY_XLU_DISP++);
                    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 30, 0, 30, effect->alpha * effect->move);
                    gDPSetEnvColor(POLY_XLU_DISP++, 30, 0, 30, 0);
                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_sst.c", 7396),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(POLY_XLU_DISP++, gEffFireCircleDL);
                }
            }
        } else if (self->effectMode == BONGO_SHADOW) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 10, 10, 80, 0);
            gDPSetEnvColor(POLY_XLU_DISP++, 10, 10, 10, self->effects[0].alpha);

            effect = &self->effects[0];
            while (effect->status != -1) {
                Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
                Matrix_Scale(effect->scale * 0.001f, 1.0f, effect->scale * 0.001f, MTXMODE_APPLY);

                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_sst.c", 7423),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++, sShadowDList);
                effect++;
            }
        }

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_sst.c", 7433);
    }
}
