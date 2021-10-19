#include "z_en_syateki_itm.h"
#include "vt.h"
#include "overlays/actors/ovl_En_Syateki_Man/z_en_syateki_man.h"
#include "overlays/actors/ovl_En_Ex_Ruppy/z_en_ex_ruppy.h"
#include "overlays/actors/ovl_En_G_Switch/z_en_g_switch.h"

#define FLAGS 0x00000010

#define THIS ((EnSyatekiItm*)thisx)

typedef enum {
    SYATEKI_ROUND_GREEN_APPEAR,
    SYATEKI_ROUND_BLUE_SEQUENTIAL,
    SYATEKI_ROUND_GREEN_THROW,
    SYATEKI_ROUND_BLUE_SIMUL,
    SYATEKI_ROUND_RED_LEFT,
    SYATEKI_ROUND_RED_RIGHT,
    SYATEKI_ROUND_MAX
} EnSyatekItemRound;

void EnSyatekiItm_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSyatekiItm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSyatekiItm_Update(Actor* thisx, GlobalContext* globalCtx);

void EnSyatekiItm_Idle(EnSyatekiItm* self, GlobalContext* globalCtx);
void EnSyatekiItm_StartRound(EnSyatekiItm* self, GlobalContext* globalCtx);
void EnSyatekiItm_SpawnTargets(EnSyatekiItm* self, GlobalContext* globalCtx);
void EnSyatekiItm_CheckTargets(EnSyatekiItm* self, GlobalContext* globalCtx);
void EnSyatekiItm_CleanupGame(EnSyatekiItm* self, GlobalContext* globalCtx);
void EnSyatekiItm_EndGame(EnSyatekiItm* self, GlobalContext* globalCtx);

const ActorInit En_Syateki_Itm_InitVars = {
    ACTOR_EN_SYATEKI_ITM,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnSyatekiItm),
    (ActorFunc)EnSyatekiItm_Init,
    (ActorFunc)EnSyatekiItm_Destroy,
    (ActorFunc)EnSyatekiItm_Update,
    NULL,
};

static Vec3f sGreenAppearHome = { 0.0f, -10.0f, -270.0f };
static Vec3f sBlueSeqHome1 = { -220.0f, 66.0f, -320.0f };
static Vec3f sBlueSeqHome2 = { 260.0f, 66.0f, -320.0f };
static Vec3f sGreenThrowHome = { 0.0f, -10.0f, -270.0f };
static Vec3f sBlueSimulHome1 = { -220.0f, 66.0f, -320.0f };
static Vec3f sBlueSimulHome2 = { 260.0f, 66.0f, -320.0f };
static Vec3f sRedLeftHome1 = { 260.0f, 100.0f, -320.0f };
static Vec3f sRedLeftHome2 = { 360.0f, 100.0f, -320.0f };
static Vec3f sRedRightHome1 = { -230.0f, 94.0f, -360.0f };
static Vec3f sRedRightHome2 = { -400.0f, 94.0f, -360.0f };
static Vec3f sGreenAppearFinal = { 0.0f, 53.0f, -270.0f };
static Vec3f sBlueSeqFinal1 = { -60.0f, 63.0f, -320.0f };
static Vec3f sBlueSeqFinal2 = { 60.0f, 63.0f, -320.0f };
static Vec3f sGreenThrowFinal = { 0.0f, 0.0f, 0.0f };
static Vec3f sBlueSimulFinal1 = { -60.0f, 63.0f, -320.0f };
static Vec3f sBlueSimulFinal2 = { 60.0f, 63.0f, -320.0f };
static Vec3f sRedLeftFinal1 = { -230.0f, 0.0f, 0.0f };
static Vec3f sRedLeftFinal2 = { -230.0f, 0.0f, 0.0f };
static Vec3f sRedRightFinal1 = { 260.0f, 0.0f, 0.0f };
static Vec3f sRedRightFinal2 = { 260.0f, 0.0f, 0.0f };
static s16 sTargetColors[] = { 0, 1, 0, 1, 2, 2 };
static s16 sRupeeTypes[] = { 0, 1, 1, 0, 1, 1, 4, 4, 4, 4 };
static Vec3f sRupeePos[] = {
    { -40.0f, 0.0f, -90.0f }, { -20.0f, 0.0f, -90.0f }, { 0.0f, 0.0f, -90.0f },   { 20.0f, 0.0f, -90.0f },
    { 40.0f, 0.0f, -90.0f },  { -40.0f, 0.0f, -60.0f }, { -20.0f, 0.0f, -60.0f }, { 0.0f, 0.0f, -60.0f },
    { 20.0f, 0.0f, -60.0f },  { 40.0f, 0.0f, -60.0f },
};

void EnSyatekiItm_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnSyatekiItm* self = THIS;
    s32 i;

    self->man = (EnSyatekiMan*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_SYATEKI_MAN,
                                                  140.0f, 0.0f, 255.0f, 0, -0x4000, 0, 0);
    if (self->man == NULL) {
        // "Spawn error"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ エラー原 ☆☆☆☆ \n" VT_RST);
        Actor_Kill(&self->actor);
        return;
    }
    for (i = 0; i < 10; i++) {
        self->markers[i] =
            (EnExRuppy*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_EX_RUPPY,
                                           sRupeePos[i].x, sRupeePos[i].y, sRupeePos[i].z, 0, 0, 0, 4);
        if (self->markers[i] == NULL) {
            // "Second spawn error"
            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ エラー原セカンド ☆☆☆☆ \n" VT_RST);
            Actor_Kill(&self->actor);
            return;
        }
        self->markers[i]->colorIdx = sRupeeTypes[i];
    }
    self->actionFunc = EnSyatekiItm_Idle;
}

void EnSyatekiItm_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnSyatekiItm_Idle(EnSyatekiItm* self, GlobalContext* globalCtx) {
    s32 i;
    Player* player = GET_PLAYER(globalCtx);

    if (self->signal == ENSYATEKI_START) {
        player->actor.world.pos.x = -12.0f;
        player->actor.world.pos.y = 20.0f;
        player->actor.world.pos.z = 182.0f;
        player->currentYaw = player->actor.world.rot.y = player->actor.shape.rot.y = 0x7F03;
        player->actor.world.rot.x = player->actor.shape.rot.x = player->actor.world.rot.z = player->actor.shape.rot.z =
            0;
        func_8008EF44(globalCtx, 15);
        self->roundNum = self->hitCount = 0;
        for (i = 0; i < 6; i++) {
            self->roundFlags[i] = false;
        }
        for (i = 0; i < 10; i++) {
            self->markers[i]->galleryFlag = false;
        }
        self->actionFunc = EnSyatekiItm_StartRound;
    }
}

void EnSyatekiItm_StartRound(EnSyatekiItm* self, GlobalContext* globalCtx) {
    s32 i;
    s32 j;
    Player* player = GET_PLAYER(globalCtx);

    if (self->unkTimer == 0) {
        if (LINK_IS_ADULT) {
            for (i = 0, j = 0; i < SYATEKI_ROUND_MAX; i++) {
                if (self->roundFlags[i]) {
                    j++;
                }
            }
            if (j >= SYATEKI_ROUND_MAX) {
                player->actor.freezeTimer = 10;
                self->signal = ENSYATEKI_END;
                self->actionFunc = EnSyatekiItm_CleanupGame;
                return;
            }
            i = Rand_ZeroFloat(5.99f);
            while (self->roundFlags[i]) {
                i = Rand_ZeroFloat(5.99f);
                if (1) {}
            }
            self->roundNum = i + 1;
            self->roundFlags[i] = true;
        } else {
            self->roundNum++;
            if (self->roundNum > SYATEKI_ROUND_MAX) {
                player->actor.freezeTimer = 10;
                self->signal = ENSYATEKI_END;
                self->actionFunc = EnSyatekiItm_CleanupGame;
                return;
            }
        }

        self->timer = (self->roundNum == 1) ? 50 : 30;

        func_80078884(NA_SE_SY_FOUND);
        self->actionFunc = EnSyatekiItm_SpawnTargets;
    }
}

void EnSyatekiItm_SpawnTargets(EnSyatekiItm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    s32 i;
    s32 roundIdx;

    if (globalCtx->shootingGalleryStatus == -1) {
        player->actor.freezeTimer = 10;
        self->signal = ENSYATEKI_END;
        self->actionFunc = EnSyatekiItm_CleanupGame;
        return;
    }
    if (self->timer == 0) {
        for (i = 0; i < 2; i++) {
            Math_Vec3f_Copy(&self->targetHome[i], &zeroVec);
            Math_Vec3f_Copy(&self->targetFinal[i], &zeroVec);
            self->targets[i] = NULL;
        }
        self->numTargets = 2;
        self->curMarkers[0] = self->curMarkers[1] = NULL;
        roundIdx = self->roundNum - 1;

        switch (roundIdx) {
            case SYATEKI_ROUND_GREEN_APPEAR:
                Math_Vec3f_Copy(&self->targetHome[0], &sGreenAppearHome);
                Math_Vec3f_Copy(&self->targetFinal[0], &sGreenAppearFinal);
                self->curMarkers[0] = self->markers[0];
                self->numTargets = 1;
                break;
            case SYATEKI_ROUND_BLUE_SEQUENTIAL:
                Math_Vec3f_Copy(&self->targetHome[0], &sBlueSeqHome1);
                Math_Vec3f_Copy(&self->targetHome[1], &sBlueSeqHome2);
                Math_Vec3f_Copy(&self->targetFinal[0], &sBlueSeqFinal1);
                Math_Vec3f_Copy(&self->targetFinal[1], &sBlueSeqFinal2);
                self->curMarkers[0] = self->markers[1];
                self->curMarkers[1] = self->markers[2];
                break;
            case SYATEKI_ROUND_GREEN_THROW:
                Math_Vec3f_Copy(&self->targetHome[0], &sGreenThrowHome);
                Math_Vec3f_Copy(&self->targetFinal[0], &sGreenThrowFinal);
                self->curMarkers[0] = self->markers[3];
                self->numTargets = 1;
                break;
            case SYATEKI_ROUND_BLUE_SIMUL:
                Math_Vec3f_Copy(&self->targetHome[0], &sBlueSimulHome1);
                Math_Vec3f_Copy(&self->targetHome[1], &sBlueSimulHome2);
                Math_Vec3f_Copy(&self->targetFinal[0], &sBlueSimulFinal1);
                Math_Vec3f_Copy(&self->targetFinal[1], &sBlueSimulFinal2);
                self->curMarkers[0] = self->markers[4];
                self->curMarkers[1] = self->markers[5];
                break;
            case SYATEKI_ROUND_RED_LEFT:
                Math_Vec3f_Copy(&self->targetHome[0], &sRedLeftHome1);
                Math_Vec3f_Copy(&self->targetHome[1], &sRedLeftHome2);
                Math_Vec3f_Copy(&self->targetFinal[0], &sRedLeftFinal1);
                Math_Vec3f_Copy(&self->targetFinal[1], &sRedLeftFinal2);
                self->curMarkers[0] = self->markers[6];
                self->curMarkers[1] = self->markers[7];
                break;
            case SYATEKI_ROUND_RED_RIGHT:
                Math_Vec3f_Copy(&self->targetHome[0], &sRedRightHome1);
                Math_Vec3f_Copy(&self->targetHome[1], &sRedRightHome2);
                Math_Vec3f_Copy(&self->targetFinal[0], &sRedRightFinal1);
                Math_Vec3f_Copy(&self->targetFinal[1], &sRedRightFinal2);
                self->curMarkers[0] = self->markers[8];
                self->curMarkers[1] = self->markers[9];
                break;
        }

        for (i = 0; i < self->numTargets; i++) {
            self->targets[i] = (EnGSwitch*)Actor_SpawnAsChild(
                &globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_G_SWITCH, self->targetHome[i].x,
                self->targetHome[i].y, self->targetHome[i].z, 0, 0, 0, (ENGSWITCH_TARGET_RUPEE << 0xC) | 0x3F);
            if (self->targets[i] == NULL) {
                // "Rupee spawn error"
                osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ ルピーでエラー原 ☆☆☆☆ \n" VT_RST);
                Actor_Kill(&self->actor);
                return;
            }
            self->targets[i]->index = i;
            self->targets[i]->colorIdx = sTargetColors[roundIdx];
            Math_Vec3f_Copy(&self->targets[i]->targetPos, &self->targetFinal[i]);
            switch (roundIdx) {
                case SYATEKI_ROUND_BLUE_SEQUENTIAL:
                    if (i == 1) {
                        self->targets[i]->delayTimer = 60;
                    }
                    break;
                case SYATEKI_ROUND_GREEN_THROW:
                    self->targets[i]->actor.velocity.y = 15.0f;
                    self->targets[i]->actor.gravity = -1.0f;
                    self->targets[i]->moveMode = GSWITCH_THROW;
                    break;
                case SYATEKI_ROUND_RED_LEFT:
                    self->targets[i]->actor.velocity.x = -5.0f;
                    self->targets[i]->moveMode = GSWITCH_LEFT;
                    break;
                case SYATEKI_ROUND_RED_RIGHT:
                    self->targets[i]->actor.velocity.x = 7.0f;
                    self->targets[i]->moveMode = GSWITCH_RIGHT;
                    break;
            }
        }
        self->targetState[0] = self->targetState[1] = ENSYATEKIHIT_NONE;
        self->actionFunc = EnSyatekiItm_CheckTargets;
    }
}

void EnSyatekiItm_CheckTargets(EnSyatekiItm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 i;
    s16 j;

    if (globalCtx->shootingGalleryStatus == -1) {
        player->actor.freezeTimer = 10;
        self->signal = ENSYATEKI_END;
        self->actionFunc = EnSyatekiItm_CleanupGame;
    } else {
        for (i = 0, j = 0; i < 2; i++) {
            if (self->targetState[i] != ENSYATEKIHIT_NONE) {
                if (self->targetState[i] == ENSYATEKIHIT_HIT) {
                    self->curMarkers[i]->galleryFlag = true;
                }
                j++;
            }
        }
        if (j == self->numTargets) {
            self->actionFunc = EnSyatekiItm_StartRound;
        }
    }
}

void EnSyatekiItm_CleanupGame(EnSyatekiItm* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if ((self->targetState[i] == ENSYATEKIHIT_NONE) && (self->targets[i] != NULL)) {
            Actor_Kill(&self->targets[i]->actor);
        }
    }
    self->actionFunc = EnSyatekiItm_EndGame;
}

void EnSyatekiItm_EndGame(EnSyatekiItm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    player->actor.freezeTimer = 10;
    if (self->signal == ENSYATEKI_RESULTS) {
        self->signal = ENSYATEKI_NONE;
        self->actionFunc = EnSyatekiItm_Idle;
    }
    if (self->signal == ENSYATEKI_START) {
        // "1 frame attack and defense!"
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ １フレームの攻防！ ☆☆☆☆ \n" VT_RST);
        self->signal = ENSYATEKI_NONE;
        self->actionFunc = EnSyatekiItm_Idle;
    }
}

void EnSyatekiItm_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnSyatekiItm* self = THIS;

    self->actionFunc(self, globalCtx);

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->unkTimer != 0) {
        self->unkTimer--;
    }
    if (BREG(0)) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, 255, 0, 0, 255, 4, globalCtx->state.gfxCtx);
    }
}
