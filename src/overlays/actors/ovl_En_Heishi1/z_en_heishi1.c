/*
 * File: z_en_heishi1.c
 * Overlay: ovl_En_Heishi1
 * Description: Courtyard Guards
 */

#include "z_en_heishi1.h"
#include "objects/object_sd/object_sd.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((EnHeishi1*)thisx)

void EnHeishi1_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi1_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi1_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi1_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnHeishi1_SetupWait(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_SetupWalk(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_SetupMoveToLink(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_SetupTurnTowardLink(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_SetupKick(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_SetupWaitNight(EnHeishi1* self, GlobalContext* globalCtx);

void EnHeishi1_Wait(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_Walk(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_MoveToLink(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_TurnTowardLink(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_Kick(EnHeishi1* self, GlobalContext* globalCtx);
void EnHeishi1_WaitNight(EnHeishi1* self, GlobalContext* globalCtx);

static s32 sPlayerIsCaught = false;

const ActorInit En_Heishi1_InitVars = {
    0,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi1),
    (ActorFunc)EnHeishi1_Init,
    (ActorFunc)EnHeishi1_Destroy,
    (ActorFunc)EnHeishi1_Update,
    (ActorFunc)EnHeishi1_Draw,
};

static f32 sAnimParamsInit[][8] = {
    { 1.0f, -10.0f, 3.0f, 0.5f, 1000.0f, 200.0f, 0.3f, 1000.0f },
    { 3.0f, -3.0f, 6.0f, 0.8f, 2000.0f, 400.0f, 0.5f, 2000.0f },
    { 1.0f, -10.0f, 3.0f, 0.5f, 1000.0f, 200.0f, 0.3f, 1000.0f },
    { 3.0f, -3.0f, 6.0f, 0.8f, 2000.0f, 400.0f, 0.5f, 2000.0f },
};

static s16 sBaseHeadTimers[] = { 20, 10, 20, 10, 13, 0 };

static Vec3f sRupeePositions[] = {
    { 0.0f, 0.0f, 90.0f },  { -55.0f, 0.0f, 90.0f }, { -55.0f, 0.0f, 30.0f }, { -55.0f, 0.0f, -30.0f },
    { 0.0f, 0.0f, -30.0f }, { 55.0f, 0.0f, -30.0f }, { 55.0f, 0.0f, 30.0f },  { 55.0f, 0.0f, 90.0f },
};

static s32 sCamDataIdxs[] = {
    7, 7, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 4, 4, 5, 6,
};

static s16 sWaypoints[] = { 0, 4, 1, 5, 2, 6, 3, 7 };

void EnHeishi1_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHeishi1* self = THIS;
    Vec3f rupeePos;
    s32 i;

    Actor_SetScale(&self->actor, 0.01f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gEnHeishiSkel, &gEnHeishiIdleAnim, self->jointTable, self->morphTable,
                   17);

    self->type = (self->actor.params >> 8) & 0xFF;
    self->path = self->actor.params & 0xFF;

    for (i = 0; i < ARRAY_COUNT(sAnimParamsInit[0]); i++) {
        self->animParams[i] = sAnimParamsInit[self->type][i];
    }

    // "type"
    osSyncPrintf(VT_FGCOL(GREEN) " 種類☆☆☆☆☆☆☆☆☆☆☆☆☆ %d\n" VT_RST, self->type);
    // "path data"
    osSyncPrintf(VT_FGCOL(YELLOW) " れえるでぇたぁ☆☆☆☆☆☆☆☆ %d\n" VT_RST, self->path);
    osSyncPrintf(VT_FGCOL(PURPLE) " anime_frame_speed ☆☆☆☆☆☆ %f\n" VT_RST, self->animSpeed);
    // "interpolation frame"
    osSyncPrintf(VT_FGCOL(PURPLE) " 補間フレーム☆☆☆☆☆☆☆☆☆ %f\n" VT_RST, self->transitionRate);
    // "targeted movement speed value between points"
    osSyncPrintf(VT_FGCOL(PURPLE) " point間の移動スピード目標値 ☆ %f\n" VT_RST, self->moveSpeedTarget);
    // "maximum movement speed value between points"
    osSyncPrintf(VT_FGCOL(PURPLE) " point間の移動スピード最大 ☆☆ %f\n" VT_RST, self->moveSpeedMax);
    // "(body) targeted turning angle speed value"
    osSyncPrintf(VT_FGCOL(PURPLE) " (体)反転アングルスピード目標値 %f\n" VT_RST, self->bodyTurnSpeedTarget);
    // "(body) maximum turning angle speed"
    osSyncPrintf(VT_FGCOL(PURPLE) " (体)反転アングルスピード最大☆ %f\n" VT_RST, self->bodyTurnSpeedMax);
    // "(head) targeted turning angle speed value"
    osSyncPrintf(VT_FGCOL(PURPLE) " (頭)反転アングルスピード加算値 %f\n" VT_RST, self->headTurnSpeedScale);
    // "(head) maximum turning angle speed"
    osSyncPrintf(VT_FGCOL(PURPLE) " (頭)反転アングルスピード最大☆ %f\n" VT_RST, self->headTurnSpeedMax);
    osSyncPrintf(VT_FGCOL(GREEN) " 今時間 %d\n" VT_RST, ((void)0, gSaveContext.dayTime)); // "current time"
    osSyncPrintf(VT_FGCOL(YELLOW) " チェック時間 %d\n" VT_RST, 0xBAAA);                   // "check time"
    osSyncPrintf("\n\n");

    if (self->path == 3) {
        for (i = 0; i < ARRAY_COUNT(sRupeePositions); i++) {
            rupeePos = sRupeePositions[i];
            Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_EX_RUPPY, rupeePos.x, rupeePos.y,
                               rupeePos.z, 0, 0, 0, 3);
        }
    }

    if (self->type != 5) {
        if (((gSaveContext.dayTime < 0xB888) || IS_DAY) && !(gSaveContext.eventChkInf[8] & 1)) {
            self->actionFunc = EnHeishi1_SetupWalk;
        } else {
            Actor_Kill(&self->actor);
        }
    } else {
        if ((gSaveContext.dayTime >= 0xB889) || !IS_DAY || (gSaveContext.eventChkInf[8] & 1)) {
            self->actionFunc = EnHeishi1_SetupWaitNight;
        } else {
            Actor_Kill(&self->actor);
        }
    }
}

void EnHeishi1_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnHeishi1_SetupWalk(EnHeishi1* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiWalkAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiWalkAnim, self->animSpeed, 0.0f, (s16)frameCount, ANIMMODE_LOOP,
                     self->transitionRate);
    self->bodyTurnSpeed = 0.0f;
    self->moveSpeed = 0.0f;
    self->headDirection = Rand_ZeroFloat(1.99f);
    self->actionFunc = EnHeishi1_Walk;
}

void EnHeishi1_Walk(EnHeishi1* self, GlobalContext* globalCtx) {
    Path* path;
    Vec3s* pointPos;
    f32 pathDiffX;
    f32 pathDiffZ;
    s16 randOffset;

    SkelAnime_Update(&self->skelAnime);

    if (Animation_OnFrame(&self->skelAnime, 1.0f) || Animation_OnFrame(&self->skelAnime, 17.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_KNIGHT_WALK);
    }

    if (!sPlayerIsCaught) {
        path = &globalCtx->setupPathList[self->path];
        pointPos = SEGMENTED_TO_VIRTUAL(path->points);
        pointPos += self->waypoint;

        Math_ApproachF(&self->actor.world.pos.x, pointPos->x, 1.0f, self->moveSpeed);
        Math_ApproachF(&self->actor.world.pos.z, pointPos->z, 1.0f, self->moveSpeed);

        Math_ApproachF(&self->moveSpeed, self->moveSpeedTarget, 1.0f, self->moveSpeedMax);

        pathDiffX = pointPos->x - self->actor.world.pos.x;
        pathDiffZ = pointPos->z - self->actor.world.pos.z;
        Math_SmoothStepToS(&self->actor.shape.rot.y, (Math_FAtan2F(pathDiffX, pathDiffZ) * (0x8000 / M_PI)), 3,
                           self->bodyTurnSpeed, 0);

        Math_ApproachF(&self->bodyTurnSpeed, self->bodyTurnSpeedTarget, 1.0f, self->bodyTurnSpeedMax);

        if (self->headTimer == 0) {
            self->headDirection++;
            self->headAngleTarget = 0x2000;
            // if headDirection is odd, face 45 degrees left
            if ((self->headDirection & 1) != 0) {
                self->headAngleTarget *= -1;
            }
            randOffset = Rand_ZeroFloat(30.0f);
            self->headTimer = sBaseHeadTimers[self->type] + randOffset;
        }

        Math_ApproachF(&self->headAngle, self->headAngleTarget, self->headTurnSpeedScale, self->headTurnSpeedMax);

        if ((self->path == BREG(1)) && (BREG(0) != 0)) {
            osSyncPrintf(VT_FGCOL(RED) " 種類  %d\n" VT_RST, self->path);
            osSyncPrintf(VT_FGCOL(RED) " ぱす  %d\n" VT_RST, self->waypoint);
            osSyncPrintf(VT_FGCOL(RED) " 反転  %d\n" VT_RST, self->bodyTurnSpeed);
            osSyncPrintf(VT_FGCOL(RED) " 時間  %d\n" VT_RST, self->waypointTimer);
            osSyncPrintf(VT_FGCOL(RED) " 点座  %d\n" VT_RST, path->count);
            osSyncPrintf("\n\n");
        }

        // when 20 units away from a middle waypoint, decide whether or not to skip it
        if ((fabsf(pathDiffX) < 20.0f) && (fabsf(pathDiffZ) < 20.0f)) {
            if (self->waypointTimer == 0) {
                if (self->type >= 2) {
                    if ((self->waypoint >= 4) && (Rand_ZeroFloat(1.99f) > 1.0f)) {
                        if (self->waypoint == 7) {
                            self->waypoint = 0;
                        }
                        if (self->waypoint >= 4) {
                            self->waypoint -= 3;
                        }
                        self->waypointTimer = 5;
                        return;
                    }
                }
                self->actionFunc = EnHeishi1_SetupWait;
            }
        }
    }
}

void EnHeishi1_SetupMoveToLink(EnHeishi1* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiWalkAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiWalkAnim, 3.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -3.0f);
    self->bodyTurnSpeed = 0.0f;
    self->moveSpeed = 0.0f;
    func_8010B680(globalCtx, 0x702D, &self->actor);
    Interface_SetDoAction(globalCtx, 0x12);
    self->actionFunc = EnHeishi1_MoveToLink;
}

void EnHeishi1_MoveToLink(EnHeishi1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    Math_ApproachF(&self->actor.world.pos.x, player->actor.world.pos.x, 1.0f, self->moveSpeed);
    Math_ApproachF(&self->actor.world.pos.z, player->actor.world.pos.z, 1.0f, self->moveSpeed);
    Math_ApproachF(&self->moveSpeed, 6.0f, 1.0f, 0.4f);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, self->bodyTurnSpeed, 0);
    Math_ApproachF(&self->bodyTurnSpeed, 3000.0f, 1.0f, 300.0f);
    Math_ApproachZeroF(&self->headAngle, 0.5f, 2000.0f);

    if (self->actor.xzDistToPlayer < 70.0f) {
        self->actionFunc = EnHeishi1_SetupTurnTowardLink;
    }
}

void EnHeishi1_SetupWait(EnHeishi1* self, GlobalContext* globalCtx) {
    s16 rand;
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, self->animSpeed, 0.0f, (s16)frameCount, ANIMMODE_LOOP,
                     self->transitionRate);
    self->headBehaviorDecided = false;
    self->headDirection = Rand_ZeroFloat(1.99f);
    rand = Rand_ZeroFloat(50.0f);
    self->waitTimer = rand + 50;
    self->actionFunc = EnHeishi1_Wait;
}

void EnHeishi1_Wait(EnHeishi1* self, GlobalContext* globalCtx) {
    s16 randOffset;
    s32 i;

    SkelAnime_Update(&self->skelAnime);
    if (!sPlayerIsCaught) {
        switch (self->headBehaviorDecided) {
            case false:
                self->headDirection++;
                // if headDirection is odd, face 52 degrees left
                self->headAngleTarget = (self->headDirection & 1) ? 0x2500 : -0x2500;
                randOffset = Rand_ZeroFloat(30.0f);
                self->headTimer = sBaseHeadTimers[self->type] + randOffset;
                self->headBehaviorDecided = true;
                break;
            case true:
                if (self->headTimer == 0) {
                    if (self->waitTimer == 0) {
                        if ((self->type == 0) || (self->type == 1)) {
                            self->waypoint++;
                            if (self->waypoint >= 4) {
                                self->waypoint = 0;
                            }
                        } else {
                            // waypoints are defined with corners as 0-3 and middle points as 4-7
                            // to choose the next waypoint, the order "04152637" is hardcoded in an array
                            for (i = 0; i < ARRAY_COUNT(sWaypoints); i++) {
                                if (self->waypoint == sWaypoints[i]) {
                                    i++;
                                    if (i >= ARRAY_COUNT(sWaypoints)) {
                                        i = 0;
                                    }
                                    self->waypoint = sWaypoints[i];
                                    break;
                                }
                            }
                            self->waypointTimer = 5;
                        }
                        self->actionFunc = EnHeishi1_SetupWalk;
                    } else {
                        self->headBehaviorDecided = false;
                    }
                }
                break;
        }
        Math_ApproachF(&self->headAngle, self->headAngleTarget, self->headTurnSpeedScale,
                       self->headTurnSpeedMax + self->headTurnSpeedMax);

        if ((self->path == BREG(1)) && (BREG(0) != 0)) {
            osSyncPrintf(VT_FGCOL(GREEN) " 種類  %d\n" VT_RST, self->path);
            osSyncPrintf(VT_FGCOL(GREEN) " ぱす  %d\n" VT_RST, self->waypoint);
            osSyncPrintf(VT_FGCOL(GREEN) " 反転  %d\n" VT_RST, self->bodyTurnSpeed);
            osSyncPrintf(VT_FGCOL(GREEN) " 時間  %d\n" VT_RST, self->waypointTimer);
            osSyncPrintf("\n\n");
        }
    }
}

void EnHeishi1_SetupTurnTowardLink(EnHeishi1* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->kickTimer = 30;
    self->actionFunc = EnHeishi1_TurnTowardLink;
}

void EnHeishi1_TurnTowardLink(EnHeishi1* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->type != 5) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, self->bodyTurnSpeed, 0);
        Math_ApproachF(&self->bodyTurnSpeed, 3000.0f, 1.0f, 300.0f);
        Math_ApproachZeroF(&self->headAngle, 0.5f, 2000.0f);
    }

    if (self->kickTimer == 0) {
        self->actionFunc = EnHeishi1_SetupKick;
    }
}

void EnHeishi1_SetupKick(EnHeishi1* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = EnHeishi1_Kick;
}

void EnHeishi1_Kick(EnHeishi1* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (!self->loadStarted) {
        // if dialog state is 5 and textbox has been advanced, kick player out
        if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx))) {
            func_80106CCC(globalCtx);
            if (!self->loadStarted) {
                gSaveContext.eventChkInf[4] |= 0x4000;
                globalCtx->nextEntranceIndex = 0x4FA;
                globalCtx->sceneLoadFlag = 0x14;
                self->loadStarted = true;
                sPlayerIsCaught = false;
                globalCtx->fadeTransition = 0x2E;
                gSaveContext.nextTransition = 0x2E;
            }
        }
    }
}

void EnHeishi1_SetupWaitNight(EnHeishi1* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = EnHeishi1_WaitNight;
}

void EnHeishi1_WaitNight(EnHeishi1* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->actor.xzDistToPlayer < 100.0f) {
        func_8010B680(globalCtx, 0x702D, &self->actor);
        func_80078884(NA_SE_SY_FOUND);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発見！ ☆☆☆☆☆ \n" VT_RST); // "Discovered!"
        func_8002DF54(globalCtx, &self->actor, 1);
        self->actionFunc = EnHeishi1_SetupKick;
    }
}

void EnHeishi1_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi1* self = THIS;
    s16 path;
    u8 i;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad2;
    Camera* activeCam;

    self->activeTimer++;

    for (i = 0; i < ARRAY_COUNT(self->timers); i++) {
        if (self->timers[i] != 0) {
            self->timers[i]--;
        }
    }

    if (self->waypointTimer != 0) {
        self->waypointTimer--;
    }

    activeCam = GET_ACTIVE_CAM(globalCtx);

    if (player->actor.freezeTimer == 0) {

        self->actionFunc(self, globalCtx);

        self->actor.uncullZoneForward = 550.0f;
        self->actor.uncullZoneScale = 350.0f;
        self->actor.uncullZoneDownward = 700.0f;

        if (self->type != 5) {
            path = self->path * 2;
            if ((sCamDataIdxs[path] == activeCam->camDataIdx) || (sCamDataIdxs[path + 1] == activeCam->camDataIdx)) {
                if (!sPlayerIsCaught) {
                    if ((self->actionFunc == EnHeishi1_Walk) || (self->actionFunc == EnHeishi1_Wait)) {
                        Vec3f searchBallVel;
                        Vec3f searchBallAccel = { 0.0f, 0.0f, 0.0f };
                        Vec3f searchBallMult = { 0.0f, 0.0f, 20.0f };
                        Vec3f searchBallPos;

                        searchBallPos.x = self->actor.world.pos.x;
                        searchBallPos.y = self->actor.world.pos.y + 60.0f;
                        searchBallPos.z = self->actor.world.pos.z;

                        Matrix_Push();
                        Matrix_RotateY(((self->actor.shape.rot.y + self->headAngle) / 32768.0f) * M_PI, MTXMODE_NEW);
                        searchBallMult.z = 30.0f;
                        Matrix_MultVec3f(&searchBallMult, &searchBallVel);
                        Matrix_Pop();

                        EffectSsSolderSrchBall_Spawn(globalCtx, &searchBallPos, &searchBallVel, &searchBallAccel, 2,
                                                     &self->linkDetected);

                        if (self->actor.xzDistToPlayer < 60.0f) {
                            self->linkDetected = true;
                        } else if (self->actor.xzDistToPlayer < 70.0f) {
                            // self case probably exists to detect link making a jump sound
                            // from slightly further away than the previous 60 unit check
                            if (player->actor.velocity.y > -4.0f) {
                                self->linkDetected = true;
                            }
                        }

                        if (self->linkDetected) {
                            //! @bug This appears to be a check to make sure that link is standing on the ground
                            // before getting caught. However self is an issue for two reasons:
                            // 1: When doing a backflip or falling from the upper path, links y velocity will reach
                            // less than -4.0 before even touching the ground.
                            // 2: There is one frame when landing from a sidehop where you can sidehop again without
                            // letting y velocity reach -4.0 or less. This enables the player to do frame perfect
                            // sidehops onto the next screen and prevent getting caught.
                            if (!(player->actor.velocity.y > -3.9f)) {
                                self->linkDetected = false;
                                // self 60 unit height check is so the player doesnt get caught when on the upper path
                                if (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 60.0f) {
                                    func_80078884(NA_SE_SY_FOUND);
                                    // "Discovered!"
                                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発見！ ☆☆☆☆☆ \n" VT_RST);
                                    func_8002DF54(globalCtx, &self->actor, 1);
                                    sPlayerIsCaught = true;
                                    self->actionFunc = EnHeishi1_SetupMoveToLink;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

s32 EnHeishi1_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                               void* thisx) {
    EnHeishi1* self = THIS;

    // turn the guards head to match the direction he is looking
    if (limbIndex == 16) {
        rot->x += (s16)self->headAngle;
    }

    return false;
}

void EnHeishi1_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHeishi1* self = THIS;
    Vec3f matrixScale = { 0.3f, 0.3f, 0.3f };

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnHeishi1_OverrideLimbDraw, NULL,
                      self);
    func_80033C30(&self->actor.world.pos, &matrixScale, 0xFF, globalCtx);

    if ((self->path == BREG(1)) && (BREG(0) != 0)) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y + 100.0f, self->actor.world.pos.z,
                               17000, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f, 1.0f, 255, 0, 0,
                               255, 4, globalCtx->state.gfxCtx);
    }
}
