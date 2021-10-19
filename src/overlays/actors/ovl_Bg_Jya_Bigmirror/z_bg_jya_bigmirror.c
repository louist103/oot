/*
 * File: z_bg_jya_bigmirror.c
 * Overlay: ovl_Bg_Jya_Bigmirror
 * Description: Ceiling mirror and puzzle in Spirit Temple top rooms
 */

#include "z_bg_jya_bigmirror.h"
#include "objects/object_jya_obj/object_jya_obj.h"

#define FLAGS 0x00000030

#define THIS ((BgJyaBigmirror*)thisx)

void BgJyaBigmirror_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBigmirror_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBigmirror_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBigmirror_Draw(Actor* thisx, GlobalContext* globalCtx);

static u8 sIsSpawned = false;

const ActorInit Bg_Jya_Bigmirror_InitVars = {
    ACTOR_BG_JYA_BIGMIRROR,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaBigmirror),
    (ActorFunc)BgJyaBigmirror_Init,
    (ActorFunc)BgJyaBigmirror_Destroy,
    (ActorFunc)BgJyaBigmirror_Update,
    (ActorFunc)BgJyaBigmirror_Draw,
};

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 params;
    /* 0x0E */ s16 solvedRotY;
    /* 0x10 */ s16 initRotY;
} BigMirrorDataEntry; // size = 0x14

static BigMirrorDataEntry sCobraSpawnData[] = {
    { { -560.0f, 1743.0f, -310.0f }, 0xFF01, 0x4000, 0x8000 },
    { { 60.0f, 1743.0f, -310.0f }, 0xFF02, 0x8000, 0xA000 },
};

void BgJyaBigmirror_SetRoomFlag(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* self = THIS;

    self->puzzleFlags &=
        ~(BIGMIR_PUZZLE_IN_STATUE_ROOM | BIGMIR_PUZZLE_IN_1ST_TOP_ROOM | BIGMIR_PUZZLE_IN_2ND_TOP_ROOM);
    if (globalCtx->roomCtx.curRoom.num == 5) {
        self->puzzleFlags |= BIGMIR_PUZZLE_IN_STATUE_ROOM;
    } else if (globalCtx->roomCtx.curRoom.num == 0x19) {
        self->puzzleFlags |= BIGMIR_PUZZLE_IN_1ST_TOP_ROOM;
    } else if (globalCtx->roomCtx.curRoom.num == 0x1A) {
        self->puzzleFlags |= BIGMIR_PUZZLE_IN_2ND_TOP_ROOM;
    }
}

void BgJyaBigmirror_HandleCobra(Actor* thisx, GlobalContext* globalCtx) {
    static u8 cobraPuzzleFlags[] = { BIGMIR_PUZZLE_COBRA1_SOLVED, BIGMIR_PUZZLE_COBRA2_SOLVED };
    BgJyaBigmirror* self = THIS;
    BigMirrorDataEntry* curSpawnData;
    BigmirrorCobra* curCobraInfo;
    s32 i;

    if (self->puzzleFlags & (BIGMIR_PUZZLE_IN_1ST_TOP_ROOM | BIGMIR_PUZZLE_IN_2ND_TOP_ROOM)) {
        for (i = 0; i < 2; i++) {
            curSpawnData = &sCobraSpawnData[i];
            curCobraInfo = &self->cobraInfo[i];
            if (curCobraInfo->cobra != NULL) {
                curCobraInfo->rotY = curCobraInfo->cobra->dyna.actor.shape.rot.y;

                if (curCobraInfo->rotY == curSpawnData->solvedRotY) {
                    self->puzzleFlags |= cobraPuzzleFlags[i];
                } else {
                    self->puzzleFlags &= ~cobraPuzzleFlags[i];
                }

                if (curCobraInfo->cobra->dyna.actor.update == NULL) {
                    // "Cobra deleted"
                    osSyncPrintf("Error : コブラ削除された (%s %d)\n", "../z_bg_jya_bigmirror.c", 203);
                }
            } else {
                curCobraInfo->cobra = (BgJyaCobra*)Actor_SpawnAsChild(
                    &globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_BG_JYA_COBRA, curSpawnData->pos.x,
                    curSpawnData->pos.y, curSpawnData->pos.z, 0, curCobraInfo->rotY, 0, curSpawnData->params);
                self->actor.child = NULL;

                if (&curCobraInfo->cobra->dyna.actor == NULL) {
                    // "Cobra generation failed"
                    osSyncPrintf("Error : コブラ発生失敗 (%s %d)\n", "../z_bg_jya_bigmirror.c", 221);
                }
            }
        }
    } else {

        for (i = 0; i < 2; i++) {
            curCobraInfo = &self->cobraInfo[i];
            if (curCobraInfo->cobra != NULL) {
                if (curCobraInfo->cobra->dyna.actor.child != NULL) {
                    Actor_Kill(curCobraInfo->cobra->dyna.actor.child);
                    curCobraInfo->cobra->dyna.actor.child = NULL;
                }
                Actor_Kill(&curCobraInfo->cobra->dyna.actor);
                curCobraInfo->cobra = NULL;
            }
        }
    }
}

void BgJyaBigmirror_SetBombiwaFlag(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* self = THIS;

    if (Flags_GetSwitch(globalCtx, 0x29)) {
        self->puzzleFlags |= BIGMIR_PUZZLE_BOMBIWA_DESTROYED;
    } else {
        self->puzzleFlags &= ~(BIGMIR_PUZZLE_BOMBIWA_DESTROYED);
    }
}

void BgJyaBigmirror_HandleMirRay(Actor* thisx, GlobalContext* globalCtx) {
    static s16 sMirRayParamss[] = { 0x0005, 0x0007, 0x0008 };
    static Vec3f sMirRayPoss[] = {
        { 60.0f, 1802.0f, -1102.0f },
        { -560.0f, 1800.0f, -310.0f },
        { 60.0f, 1800.0f, -310.0f },
    };
    BgJyaBigmirror* self = THIS;
    s32 puzzleSolved;
    s32 lightBeamToggles[3];
    s32 i;
    s32 objBankIndex;

    objBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MIR_RAY);

    if ((objBankIndex < 0) || (objBankIndex != self->mirRayObjIndex)) {
        self->lightBeams[2] = NULL;
        self->lightBeams[1] = NULL;
        self->lightBeams[0] = NULL;
    } else {
        puzzleSolved = !!(self->puzzleFlags & (BIGMIR_PUZZLE_IN_STATUE_ROOM | BIGMIR_PUZZLE_IN_1ST_TOP_ROOM));

        if (puzzleSolved) {
            puzzleSolved = !!(self->puzzleFlags & BIGMIR_PUZZLE_COBRA2_SOLVED);

            if (puzzleSolved) {
                puzzleSolved = !!(self->puzzleFlags & BIGMIR_PUZZLE_COBRA1_SOLVED);
            }
        }
        lightBeamToggles[0] = puzzleSolved; // Only spawn if puzzle solved
        if (1) {}
        lightBeamToggles[1] = lightBeamToggles[2] =
            self->puzzleFlags & (BIGMIR_PUZZLE_IN_1ST_TOP_ROOM | BIGMIR_PUZZLE_IN_2ND_TOP_ROOM);

        for (i = 0; i < 3; i++) {
            if (lightBeamToggles[i]) {
                if ((self->lightBeams[i] == NULL) && Object_IsLoaded(&globalCtx->objectCtx, objBankIndex)) {
                    self->lightBeams[i] = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_MIR_RAY, sMirRayPoss[i].x,
                                                      sMirRayPoss[i].y, sMirRayPoss[i].z, 0, 0, 0, sMirRayParamss[i]);

                    if (self->lightBeams[i] == NULL) {
                        // "Mir Ray generation failed"
                        osSyncPrintf("Error : Mir Ray 発生失敗 (%s %d)\n", "../z_bg_jya_bigmirror.c", 310);
                    }
                }
            } else {
                if (self->lightBeams[i] != NULL) {
                    Actor_Kill(self->lightBeams[i]);
                    self->lightBeams[i] = NULL;
                }
            }
        }
    }
    self->mirRayObjIndex = objBankIndex;
}

void BgJyaBigmirror_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* self = THIS;

    if (sIsSpawned) {
        Actor_Kill(&self->actor);
        return;
    }

    Actor_SetScale(&self->actor, 0.1f);
    self->cobraInfo[0].rotY = sCobraSpawnData[0].initRotY;
    self->cobraInfo[1].rotY = sCobraSpawnData[1].initRotY;
    self->actor.room = -1;
    sIsSpawned = true;
    self->spawned = true;
    self->mirRayObjIndex = -1;

    // "jya Bigmirror"
    osSyncPrintf("(jya 大鏡)(arg_data 0x%04x)\n", self->actor.params);
}

void BgJyaBigmirror_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* self = THIS;

    if (self->spawned) {
        sIsSpawned = false;
    }
}

void BgJyaBigmirror_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror_SetRoomFlag(thisx, globalCtx);
    BgJyaBigmirror_HandleCobra(thisx, globalCtx);
    BgJyaBigmirror_SetBombiwaFlag(thisx, globalCtx);
    BgJyaBigmirror_HandleMirRay(thisx, globalCtx);
}

void BgJyaBigmirror_DrawLightBeam(Actor* thisx, GlobalContext* globalCtx) {
    static Vec3s D_80893F4C = { 0, 0, 0 };
    BgJyaBigmirror* self = THIS;
    Actor* lift;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_jya_bigmirror.c", 435);
    func_80093D84(globalCtx->state.gfxCtx);
    lift = Actor_Find(&globalCtx->actorCtx, ACTOR_BG_JYA_LIFT, ACTORCAT_BG);
    if (lift != NULL) {
        self->liftHeight = lift->world.pos.y;
    }
    func_800D1694(self->actor.world.pos.x, self->actor.world.pos.y + 40.0f, self->actor.world.pos.z,
                  &self->actor.shape.rot);
    Matrix_Scale(0.1f, (self->liftHeight * -(1.0f / 1280.0f)) + (1779.4f / 1280.0f), 0.1f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_jya_bigmirror.c", 457),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gBigMirror1DL);

    if (lift != NULL) {
        if (1) {}
        func_800D1694(lift->world.pos.x, lift->world.pos.y, lift->world.pos.z, &D_80893F4C);
        Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_jya_bigmirror.c", 467),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gBigMirror2DL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_jya_bigmirror.c", 476);
}

void BgJyaBigmirror_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* self = THIS;

    if (self->puzzleFlags & BIGMIR_PUZZLE_IN_1ST_TOP_ROOM) {
        Gfx_DrawDListOpa(globalCtx, gBigMirror3DL);
        Gfx_DrawDListXlu(globalCtx, gBigMirror4DL);
    }

    if ((self->puzzleFlags &
         (BIGMIR_PUZZLE_IN_STATUE_ROOM | BIGMIR_PUZZLE_IN_1ST_TOP_ROOM | BIGMIR_PUZZLE_IN_2ND_TOP_ROOM)) &&
        (self->puzzleFlags & BIGMIR_PUZZLE_COBRA2_SOLVED) && (self->puzzleFlags & BIGMIR_PUZZLE_COBRA1_SOLVED)) {
        BgJyaBigmirror_DrawLightBeam(&self->actor, globalCtx);
    }
}
