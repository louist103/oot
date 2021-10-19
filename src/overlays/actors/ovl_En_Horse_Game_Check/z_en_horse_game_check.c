/*
 * File: z_en_horse_game_check.c
 * Overlay: ovl_En_Horse_Game_Check
 * Description: Horseback Minigames
 */

#include "z_en_horse_game_check.h"
#include "overlays/actors/ovl_En_Horse/z_en_horse.h"

#define FLAGS 0x00000010

#define THIS ((EnHorseGameCheckBase*)thisx)
#define AT_FINISH_LINE(actor)                                                                                     \
    (Math3D_PointInSquare2D(sFinishLine[0], sFinishLine[1], sFinishLine[2], sFinishLine[3], (actor)->world.pos.x, \
                            (actor)->world.pos.z))
#define AT_RANCH_EXIT(actor)                                                                                  \
    (Math3D_PointInSquare2D(sRanchExit[0], sRanchExit[1], sRanchExit[2], sRanchExit[3], (actor)->world.pos.x, \
                            (actor)->world.pos.z))

#define INGORACE_PLAYER_MOVE (1 << 0)
#define INGORACE_SET_TIMER (1 << 1)
#define INGORACE_INGO_MOVE (1 << 2)

typedef enum {
    /* 0 */ INGORACE_NO_RESULT,
    /* 1 */ INGORACE_PLAYER_WIN,
    /* 2 */ INGORACE_INGO_WIN,
    /* 3 */ INGORACE_TIME_UP
} HorseGameIngoRaceResult;

#define MALONRACE_PLAYER_MOVE (1 << 0)
#define MALONRACE_SET_TIMER (1 << 1)
#define MALONRACE_SECOND_LAP (1 << 2)
#define MALONRACE_BROKE_RULE (1 << 3)
#define MALONRACE_START_SFX (1 << 4)
#define MALONRACE_PLAYER_START (1 << 5)
#define MALONRACE_PLAYER_ON_MARK (1 << 6)

typedef enum {
    /* 0 */ MALONRACE_NO_RESULT,
    /* 1 */ MALONRACE_SUCCESS,
    /* 2 */ MALONRACE_TIME_UP,
    /* 3 */ MALONRACE_UNUSED,
    /* 4 */ MALONRACE_FAILURE
} HorseGameMalonRaceResult;

void EnHorseGameCheck_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHorseGameCheck_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHorseGameCheck_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHorseGameCheck_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Horse_Game_Check_InitVars = {
    ACTOR_EN_HORSE_GAME_CHECK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnHorseGameCheck),
    (ActorFunc)EnHorseGameCheck_Init,
    (ActorFunc)EnHorseGameCheck_Destroy,
    (ActorFunc)EnHorseGameCheck_Update,
    (ActorFunc)EnHorseGameCheck_Draw,
};

static Vec3f sIngoRaceCheckpoints[] = {
    { 1700.0f, 1.0f, -540.0f },
    { 117.0f, 1.0f, 488.0f },
    { -1700.0f, 1.0f, -540.0f },
};

static f32 sFinishLine[4] = { -200.0f, 80.0f, -2300.0f, -1470.0f };

static f32 sRanchExit[4] = { 800.0f, 1000.0f, -2900.0f, -2700.0f };

static Vec3f sUnusedZeroVec = { 0.0f, 0.0f, 0.0f };

static Vec3f sFencePos[] = {
    { 820.0f, -44.0f, -1655.0f }, { 1497.0f, -21.0f, -1198.0f },  { 1655.0f, -44.0f, -396.0f },
    { 1291.0f, -44.0f, 205.0f },  { 379.0f, -21.0f, 455.0f },     { -95.0f, -21.0f, 455.0f },
    { -939.0f, 1.0f, 455.0f },    { -1644.0f, -21.0f, -1035.0f },
};

s32 EnHorseGameCheck_InitIngoRace(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    EnHorseGameCheckIngoRace* self = (EnHorseGameCheckIngoRace*)base;
    s32 i;

    self->base.type = HORSEGAME_INGO_RACE;
    self->startFlags = 0;
    for (i = 0; i < 3; i++) {
        self->playerCheck[i] = 0;
    }
    self->ingoHorse =
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_HORSE, -250.0f, 1.0f, -1650.0f, 0, 0x4000, 0, 0x8003);

    if (self->ingoHorse == NULL) {
        LogUtils_HungupThread("../z_en_horse_game_check.c", 385);
    }
    self->startTimer = 0;
    self->finishTimer = 0;
    self->result = INGORACE_NO_RESULT;
    self->playerFinish = 0;
    self->ingoFinish = 0;

    return true;
}

s32 EnHorseGameCheck_DestroyIngoRace(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    return true;
}

void EnHorseGameCheck_FinishIngoRace(EnHorseGameCheckIngoRace* self, GlobalContext* globalCtx) {
    gSaveContext.cutsceneIndex = 0;
    if (self->result == INGORACE_PLAYER_WIN) {
        globalCtx->nextEntranceIndex = 0x4CE;
        if (gSaveContext.eventInf[0] & 0x40) {
            gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0xF) | 6;
            gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x8000) | 0x8000;
            globalCtx->fadeTransition = 3;
            func_800775F0(0x42);
        } else {
            gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0xF) | 4;
            gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x8000) | 0x8000;
            func_800775F0(0x42);
            globalCtx->fadeTransition = 0x2E;
        }
    } else {
        globalCtx->nextEntranceIndex = 0x558;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0xF) | 3;
        globalCtx->fadeTransition = 0x20;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x8000) | 0x8000;
    }
    DREG(25) = 0;
    globalCtx->sceneLoadFlag = 0x14;
    gSaveContext.timer1State = 0;
}

s32 EnHorseGameCheck_UpdateIngoRace(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    EnHorseGameCheckIngoRace* self = (EnHorseGameCheckIngoRace*)base;
    Player* player = GET_PLAYER(globalCtx);
    s32 i;
    EnHorse* ingoHorse;
    EnHorse* horse;

    if ((self->startTimer > 50) && !(self->startFlags & INGORACE_SET_TIMER)) {
        self->startFlags |= INGORACE_SET_TIMER;
        func_80088B34(0);
    } else if ((self->startTimer > 80) && (player->rideActor != NULL) && !(self->startFlags & INGORACE_PLAYER_MOVE)) {
        self->startFlags |= INGORACE_PLAYER_MOVE;
        horse = (EnHorse*)player->rideActor;
        horse->inRace = 1;
    } else if ((self->startTimer > 81) && !(self->startFlags & INGORACE_INGO_MOVE)) {
        ingoHorse = (EnHorse*)self->ingoHorse;

        ingoHorse->inRace = 1;
        self->startFlags |= INGORACE_INGO_MOVE;
        Audio_PlaySoundGeneral(NA_SE_SY_START_SHOT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }

    self->startTimer++;

    for (i = 0; i < 3; i++) {
        if ((player->rideActor != NULL) &&
            (Math3D_Vec3f_DistXYZ(&sIngoRaceCheckpoints[i], &player->rideActor->world.pos) < 400.0f)) {
            if ((i > 0) && (self->playerCheck[i - 1] == 1)) {
                self->playerCheck[i] = 1;
            } else if (i == 0) {
                self->playerCheck[i] = 1;
            }
        }
        if (Math3D_Vec3f_DistXYZ(&sIngoRaceCheckpoints[i], &self->ingoHorse->world.pos) < 400.0f) {
            if ((i > 0) && (self->ingoCheck[i - 1] == 1)) {
                self->ingoCheck[i] = 1;
            } else if (i == 0) {
                self->ingoCheck[i] = 1;
            }
        }
    }

    if (self->result == INGORACE_NO_RESULT) {
        Player* player2 = player;

        if ((player2->rideActor != NULL) && (self->playerCheck[2] == 1) && AT_FINISH_LINE(player2->rideActor)) {
            self->playerFinish++;
            if (self->playerFinish > 0) {
                self->result = INGORACE_PLAYER_WIN;
                self->finishTimer = 55;
                Audio_QueueSeqCmd(0x41);
                Audio_PlaySoundGeneral(NA_SE_SY_START_SHOT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            }
            for (i = 0; i < 3; i++) {
                self->playerCheck[i] = 0;
            }
        }
        if ((self->ingoHorse != NULL) && (self->ingoCheck[2] == 1) && AT_FINISH_LINE(self->ingoHorse)) {
            self->ingoFinish++;
            if (self->ingoFinish > 0) {
                ingoHorse = (EnHorse*)self->ingoHorse;

                self->result = INGORACE_INGO_WIN;
                self->finishTimer = 70;
                ingoHorse->stateFlags |= ENHORSE_INGO_WON;
                Audio_QueueSeqCmd(0x41);
                Audio_PlaySoundGeneral(NA_SE_SY_START_SHOT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            }
            for (i = 0; i < 3; i++) {
                self->ingoCheck[i] = 0;
            }
        }
        if (((player2->rideActor != NULL) && AT_RANCH_EXIT(player2->rideActor)) || AT_RANCH_EXIT(&player2->actor)) {
            Audio_QueueSeqCmd(0x41);
            self->result = INGORACE_INGO_WIN;
            self->finishTimer = 20;
        }
        if ((gSaveContext.timer1Value >= 180) && (self->startFlags & 2)) {
            Audio_QueueSeqCmd(0x41);
            self->result = INGORACE_TIME_UP;
            self->finishTimer = 20;
        }
    } else {
        if (self->finishTimer > 0) {
            self->finishTimer--;
        } else {
            EnHorseGameCheck_FinishIngoRace(self, globalCtx);
        }
    }
    return true;
}

s32 EnHorseGameCheck_InitGerudoArchery(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    EnHorseGameCheckGerudoArchery* self = (EnHorseGameCheckGerudoArchery*)base;

    self->base.type = HORSEGAME_GERUDO_ARCHERY;
    self->unk_150 = 0;
    self->startTimer = 0;
    return true;
}

s32 EnHorseGameCheck_DestroyGerudoArchery(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    return true;
}

s32 EnHorseGameCheck_UpdateGerudoArchery(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    EnHorseGameCheckGerudoArchery* self = (EnHorseGameCheckGerudoArchery*)base;
    Player* player = GET_PLAYER(globalCtx);
    EnHorse* horse = (EnHorse*)player->rideActor;

    if (horse == NULL) {
        return true;
    }

    if (self->startTimer > 90) {
        if (globalCtx) {}
        horse->hbaStarted = 1;
    }
    self->startTimer++;
    return true;
}

s32 EnHorseGameCheck_InitType3(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    EnHorseGameCheck3* self = (EnHorseGameCheck3*)base;

    self->base.type = HORSEGAME_TYPE3;
    self->unk_150 = 0;
    return true;
}

s32 EnHorseGameCheck_DestroyType3(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    return true;
}

s32 EnHorseGameCheck_UpdateType3(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    return true;
}

s32 EnHorseGameCheck_InitMalonRace(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    EnHorseGameCheckMalonRace* self = (EnHorseGameCheckMalonRace*)base;
    s32 i;

    self->base.type = HORSEGAME_MALON_RACE;
    self->raceFlags = 0;
    self->finishTimer = 0;
    self->result = MALONRACE_NO_RESULT;
    for (i = 0; i < 16; i++) {
        self->fenceCheck[i] = 0;
    }
    self->lapCount = 0;
    return true;
}

s32 EnHorseGameCheck_DestroyMalonRace(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    return true;
}

void EnHorseGameCheck_FinishMalonRace(EnHorseGameCheckMalonRace* self, GlobalContext* globalCtx) {
    if ((self->result == MALONRACE_SUCCESS) || (self->result == MALONRACE_TIME_UP)) {
        gSaveContext.cutsceneIndex = 0;
        globalCtx->nextEntranceIndex = 0x4CE;
        globalCtx->fadeTransition = 0x2E;
        globalCtx->sceneLoadFlag = 0x14;
    } else if (self->result == MALONRACE_FAILURE) {
        gSaveContext.timer1Value = 240;
        gSaveContext.timer1State = 0xF;
        gSaveContext.cutsceneIndex = 0;
        globalCtx->nextEntranceIndex = 0x4CE;
        globalCtx->fadeTransition = 0x2E;
        globalCtx->sceneLoadFlag = 0x14;
    } else {
        // "not supported"
        osSyncPrintf("En_HGC_Spot20_Ta_end():対応せず\n");
        gSaveContext.cutsceneIndex = 0;
        globalCtx->nextEntranceIndex = 0x157;
        globalCtx->fadeTransition = 0x2E;
        globalCtx->sceneLoadFlag = 0x14;
    }
}

s32 EnHorseGameCheck_UpdateMalonRace(EnHorseGameCheckBase* base, GlobalContext* globalCtx) {
    EnHorseGameCheckMalonRace* self = (EnHorseGameCheckMalonRace*)base;
    s32 i;
    Player* player = GET_PLAYER(globalCtx);
    EnHorse* horse;

    if (!(self->raceFlags & MALONRACE_PLAYER_ON_MARK) && AT_FINISH_LINE(player->rideActor)) {
        self->raceFlags |= MALONRACE_PLAYER_ON_MARK;
    } else if ((self->raceFlags & MALONRACE_PLAYER_ON_MARK) && !(self->raceFlags & MALONRACE_PLAYER_START) &&
               !AT_FINISH_LINE(player->rideActor)) {
        self->raceFlags |= MALONRACE_PLAYER_START;
    }
    if ((self->startTimer > 50) && !(self->raceFlags & MALONRACE_SET_TIMER)) {
        self->raceFlags |= MALONRACE_SET_TIMER;
        func_80088B34(0);
    } else if ((self->startTimer > 80) && (player->rideActor != NULL) && !(self->raceFlags & MALONRACE_PLAYER_MOVE)) {
        self->raceFlags |= MALONRACE_PLAYER_MOVE;
        horse = (EnHorse*)player->rideActor;

        horse->inRace = 1;
    } else if ((self->startTimer > 81) && !(self->raceFlags & MALONRACE_START_SFX)) {
        self->raceFlags |= MALONRACE_START_SFX;
        Audio_PlaySoundGeneral(NA_SE_SY_START_SHOT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }

    self->startTimer++;
    if (self->result == MALONRACE_NO_RESULT) {
        Player* player2 = player;
        f32 dist;

        for (i = 0; i < 16; i++) {
            if ((self->lapCount == 0) && (i >= 8)) {
                break;
            }
            dist = Math_Vec3f_DistXZ(&sFencePos[i % 8], &player2->rideActor->world.pos);
            if ((player->rideActor != NULL) && (dist < 250.0f)) {
                horse = (EnHorse*)player2->rideActor;

                if (horse->stateFlags & ENHORSE_JUMPING) {
                    if ((i > 0) && (self->fenceCheck[i - 1] == 1)) {
                        self->fenceCheck[i] = 1;
                    } else if (i == 0) {
                        self->fenceCheck[i] = 1;
                    }

                    if ((self->fenceCheck[i - 1] == 0) && !(self->raceFlags & MALONRACE_BROKE_RULE)) {
                        self->raceFlags |= MALONRACE_BROKE_RULE;
                        func_8010B680(globalCtx, 0x208C, NULL);
                        self->result = 4;
                        self->finishTimer = 30;
                    }
                }
            }
        }
        if ((player2->rideActor != NULL) && (self->raceFlags & MALONRACE_PLAYER_START) &&
            AT_FINISH_LINE(player2->rideActor)) {
            if ((self->lapCount == 1) && (self->fenceCheck[15] == 0) && (player2->rideActor->prevPos.x < -200.0f)) {
                self->raceFlags |= MALONRACE_BROKE_RULE;
                func_8010B680(globalCtx, 0x208C, NULL);
                self->result = MALONRACE_FAILURE;
                self->finishTimer = 30;
            } else if (self->fenceCheck[15] == 1) {
                self->lapCount = 2;
                Audio_QueueSeqCmd(0x41);
                Audio_PlaySoundGeneral(NA_SE_SY_START_SHOT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                self->result = MALONRACE_SUCCESS;
                self->finishTimer = 70;
                gSaveContext.timer1State = 0xF;
            } else if ((self->fenceCheck[7] == 1) && !(self->raceFlags & MALONRACE_SECOND_LAP)) {
                self->lapCount = 1;
                self->raceFlags |= MALONRACE_SECOND_LAP;
                func_8010B680(globalCtx, 0x208D, NULL);
            } else if (self->fenceCheck[7] == 0) {
                self->raceFlags |= MALONRACE_BROKE_RULE;
                func_8010B680(globalCtx, 0x208C, NULL);
                self->result = MALONRACE_FAILURE;
                self->finishTimer = 30;
            } else if (player2->rideActor->prevPos.x > 80.0f) {
                self->raceFlags |= MALONRACE_BROKE_RULE;
                func_8010B680(globalCtx, 0x208C, NULL);
                self->result = MALONRACE_FAILURE;
                self->finishTimer = 30;
            }
        }
        if ((gSaveContext.timer1Value >= 180) && (self->raceFlags & MALONRACE_SET_TIMER)) {
            gSaveContext.timer1Value = 240;
            self->result = MALONRACE_TIME_UP;
            self->finishTimer = 30;
            gSaveContext.timer1State = 0;
        }
    } else {
        if (self->finishTimer > 0) {
            self->finishTimer--;
        } else {
            EnHorseGameCheck_FinishMalonRace(self, globalCtx);
        }
    }
    return true;
}

static EnHorseGameCheckFunc sInitFuncs[] = {
    NULL,
    EnHorseGameCheck_InitIngoRace,
    EnHorseGameCheck_InitGerudoArchery,
    EnHorseGameCheck_InitType3,
    EnHorseGameCheck_InitMalonRace,
};

static EnHorseGameCheckFunc sDestroyFuncs[] = {
    NULL,
    EnHorseGameCheck_DestroyIngoRace,
    EnHorseGameCheck_DestroyGerudoArchery,
    EnHorseGameCheck_DestroyType3,
    EnHorseGameCheck_DestroyMalonRace,
};

static EnHorseGameCheckFunc sUpdateFuncs[] = {
    NULL,
    EnHorseGameCheck_UpdateIngoRace,
    EnHorseGameCheck_UpdateGerudoArchery,
    EnHorseGameCheck_UpdateType3,
    EnHorseGameCheck_UpdateMalonRace,
};

void EnHorseGameCheck_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHorseGameCheckBase* self = THIS;

    if ((globalCtx->sceneNum == SCENE_SPOT20) && (Flags_GetEventChkInf(0x18) || DREG(1))) {
        self->actor.params = HORSEGAME_MALON_RACE;
    }
    if (sInitFuncs[self->actor.params] != NULL) {
        sInitFuncs[self->actor.params](self, globalCtx);
    }
}

void EnHorseGameCheck_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHorseGameCheckBase* self = THIS;

    if (sDestroyFuncs[self->actor.params] != NULL) {
        sDestroyFuncs[self->actor.params](self, globalCtx);
    }
}

void EnHorseGameCheck_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHorseGameCheckBase* self = THIS;

    if (sUpdateFuncs[self->type] != NULL) {
        sUpdateFuncs[self->type](self, globalCtx);
    }
}

void EnHorseGameCheck_Draw(Actor* thisx, GlobalContext* globalCtx) {
}
