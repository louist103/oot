/*
 * File: z_en_dnt_demo.c
 * Overlay: ovl_En_Dnt_Demo
 * Description: Forest Stage minigame
 */

#include "z_en_dnt_demo.h"

#include "overlays/actors/ovl_En_Dnt_Jiji/z_en_dnt_jiji.h"
#include "overlays/actors/ovl_En_Dnt_Nomal/z_en_dnt_nomal.h"
#include "vt.h"

#define FLAGS 0x00000000

#define THIS ((EnDntDemo*)thisx)

typedef enum {
    /* 0 */ DNT_LIKE,
    /* 1 */ DNT_HATE,
    /* 2 */ DNT_LOVE
} EnDntDemoResults;

void EnDntDemo_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDntDemo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDntDemo_Update(Actor* self, GlobalContext* globalCtx);

void EnDntDemo_Judge(EnDntDemo* self, GlobalContext* globalCtx);
void EnDntDemo_Results(EnDntDemo* self, GlobalContext* globalCtx);
void EnDntDemo_Prize(EnDntDemo* self, GlobalContext* globalCtx);

const ActorInit En_Dnt_Demo_InitVars = {
    ACTOR_EN_DNT_DEMO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnDntDemo),
    (ActorFunc)EnDntDemo_Init,
    (ActorFunc)EnDntDemo_Destroy,
    (ActorFunc)EnDntDemo_Update,
    NULL,
};

//! @bug
//! This table is missing a column for the Mask of Truth, so it reads the first value of the next row. In the last row,
//! it reads the first entry of sResultValues (4), which is an invalid result. The scrubs have no reaction in self case.
static s16 sResultTable[8][7] = {
    /* Keaton    Skull     Spooky    Bunny     Goron      Zora     Gerudo         Truth   */
    { DNT_LIKE, DNT_HATE, DNT_LIKE, DNT_HATE, DNT_LOVE, DNT_LIKE, DNT_HATE }, /* DNT_HATE */
    { DNT_HATE, DNT_LIKE, DNT_HATE, DNT_LIKE, DNT_HATE, DNT_HATE, DNT_LOVE }, /* DNT_LOVE */
    { DNT_LOVE, DNT_HATE, DNT_HATE, DNT_HATE, DNT_LIKE, DNT_LIKE, DNT_LIKE }, /* DNT_HATE */
    { DNT_HATE, DNT_LOVE, DNT_HATE, DNT_HATE, DNT_HATE, DNT_HATE, DNT_LIKE }, /* DNT_LIKE */
    { DNT_LIKE, DNT_LIKE, DNT_LOVE, DNT_LIKE, DNT_LIKE, DNT_LIKE, DNT_HATE }, /* DNT_LIKE */
    { DNT_LIKE, DNT_LIKE, DNT_LIKE, DNT_LOVE, DNT_HATE, DNT_LOVE, DNT_LIKE }, /* DNT_HATE */
    { DNT_HATE, DNT_HATE, DNT_HATE, DNT_HATE, DNT_HATE, DNT_HATE, DNT_HATE }, /* DNT_LOVE */
    { DNT_LOVE, DNT_LOVE, DNT_LOVE, DNT_LOVE, DNT_LOVE, DNT_LOVE, DNT_LOVE }, /* INVALID  */
};

static s16 sResultValues[3][2] = {
    /* DNT_LIKE */ { DNT_SIGNAL_HIDE, DNT_ACTION_LOW_RUPEES },
    /* DNT_HATE */ { DNT_SIGNAL_HIDE, DNT_ACTION_ATTACK },
    /* DNT_LOVE */ { DNT_SIGNAL_DANCE, DNT_ACTION_DANCE },
};

static Vec3f sScrubPos[] = {
    { 3810.0f, -20.0f, 1010.0f }, { 3890.0f, -20.0f, 990.0f }, { 3730.0f, -20.0f, 950.0f },
    { 3840.0f, -20.0f, 930.0f },  { 3910.0f, -20.0f, 870.0f }, { 3780.0f, -20.0f, 860.0f },
    { 3710.0f, -20.0f, 840.0f },  { 3860.0f, -20.0f, 790.0f }, { 3750.0f, -20.0f, 750.0f },
};

void EnDntDemo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnDntDemo_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnDntDemo* self = THIS;
    s32 i;
    s32 pad;

    osSyncPrintf("\n\n");
    // "Deku Scrub mask show start"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ デグナッツお面品評会開始 ☆☆☆☆☆ \n" VT_RST);
    for (i = 0; i < 9; i++) {
        self->scrubPos[i] = sScrubPos[i];
        self->scrubs[i] = (EnDntNomal*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx,
                                                          ACTOR_EN_DNT_NOMAL, self->scrubPos[i].x, self->scrubPos[i].y,
                                                          self->scrubPos[i].z, 0, 0, 0, i + ENDNTNOMAL_STAGE);
        if (self->scrubs[i] != NULL) {
            // "zako zako" [small fries]
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ ザコザコ ☆☆☆☆☆ %x\n" VT_RST, self->scrubs[i]);
        }
    }

    self->leaderPos.x = 4050.0f;
    self->leaderPos.y = -20.0f;
    self->leaderPos.z = 1000.0f;
    self->leader = (EnDntJiji*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_DNT_JIJI,
                                                  self->leaderPos.x, self->leaderPos.y, self->leaderPos.z, 0, 0, 0, 0);
    if (self->leader != NULL) {
        // "jiji jiji jiji jiji jiji" [onomatopoeia for the scrub sound?]
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ じじじじじじじじじじい ☆☆☆☆☆ %x\n" VT_RST, self->leader);
    }
    self->subCamera = 0;
    self->actor.flags &= ~1;
    self->actionFunc = EnDntDemo_Judge;
}

void EnDntDemo_Judge(EnDntDemo* self, GlobalContext* globalCtx) {
    s16 delay;
    s16 reaction;
    s16 rand9;
    s16 maskIdx;
    s16 resultIdx;
    u8 ignore;
    s32 i;

    if (self->leaderSignal != DNT_SIGNAL_NONE) {
        for (i = 0; i < 9; i++) {
            self->scrubs[i]->stageSignal = self->leaderSignal;
            self->scrubs[i]->action = self->action;
            self->scrubs[i]->stagePrize = DNT_PRIZE_NONE;
        }
        if (self->leader->isSolid) {
            self->leader->stageSignal = DNT_LEADER_SIGNAL_BURROW;
        }
        self->leaderSignal = DNT_SIGNAL_NONE;
        self->actionFunc = EnDntDemo_Results;
    } else if ((self->actor.xzDistToPlayer > 30.0f) || (Player_GetMask(globalCtx) == 0)) {
        self->debugArrowTimer++;
        if (self->subCamera != SUBCAM_FREE) {
            self->subCamera = SUBCAM_FREE;
        }
        if (self->judgeTimer != 0) {
            for (i = 0; i < 9; i++) {
                self->scrubs[i]->stageSignal = DNT_SIGNAL_HIDE;
            }
            self->judgeTimer = 0;
        }
    } else {
        if ((Player_GetMask(globalCtx) != 0) && (self->subCamera == SUBCAM_FREE)) {
            self->subCamera = OnePointCutscene_Init(globalCtx, 2220, -99, &self->scrubs[3]->actor, MAIN_CAM);
        }
        self->debugArrowTimer = 0;
        if (self->judgeTimer == 40) {
            for (i = 0; i < 9; i++) {
                self->scrubs[i]->stageSignal = DNT_SIGNAL_LOOK;
            }
        }
        if (self->judgeTimer > 40) {
            // "gera gera" [onomatopoeia for loud giggling]
            osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ げらげら ☆☆☆☆☆ \n" VT_RST);
            func_800F436C(&self->actor.projectedPos, NA_SE_EV_CROWD - SFX_FLAG, 2.0f);
        }
        if (self->judgeTimer < 120) {
            self->judgeTimer++;
        } else {
            ignore = false;
            reaction = DNT_SIGNAL_NONE;
            delay = 0;
            switch (Player_GetMask(globalCtx)) {
                case PLAYER_MASK_SKULL:
                    if (!(gSaveContext.itemGetInf[1] & 0x4000)) {
                        reaction = DNT_SIGNAL_CELEBRATE;
                        self->prize = DNT_PRIZE_STICK;
                        Audio_QueueSeqCmd(0x3E);
                        break;
                    }
                case PLAYER_MASK_TRUTH:
                    if (!(gSaveContext.itemGetInf[1] & 0x8000) && (Player_GetMask(globalCtx) != PLAYER_MASK_SKULL)) {
                        Audio_PlaySoundGeneral(NA_SE_SY_TRE_BOX_APPEAR, &D_801333D4, 4, &D_801333E0, &D_801333E0,
                                               &D_801333E8);
                        self->prize = DNT_PRIZE_NUTS;
                        self->leader->stageSignal = DNT_LEADER_SIGNAL_UP;
                        reaction = DNT_SIGNAL_LOOK;
                        if (self->subCamera != SUBCAM_FREE) {
                            self->subCamera = SUBCAM_FREE;
                            reaction = DNT_SIGNAL_LOOK;
                            OnePointCutscene_Init(globalCtx, 2340, -99, &self->leader->actor, MAIN_CAM);
                        }
                        break;
                    }
                case PLAYER_MASK_KEATON:
                case PLAYER_MASK_SPOOKY:
                case PLAYER_MASK_BUNNY:
                case PLAYER_MASK_GORON:
                case PLAYER_MASK_ZORA:
                case PLAYER_MASK_GERUDO:
                    rand9 = Rand_ZeroFloat(8.99f);
                    maskIdx = Player_GetMask(globalCtx);
                    maskIdx--;
                    if (rand9 == 8) {
                        ignore = true;
                        delay = 8;
                        reaction = DNT_SIGNAL_HIDE;
                        // "Special!"
                        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 特別！ ☆☆☆☆☆ \n" VT_RST);
                    } else {
                        if (maskIdx >= PLAYER_MASK_MAX - 1) {
                            // "This is dangerous!"
                            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ ヤバいよこれ！ ☆☆☆☆☆ \n" VT_RST);
                            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ ヤバいよこれ！ ☆☆☆☆☆ \n" VT_RST);
                            osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ ヤバいよこれ！ ☆☆☆☆☆ \n" VT_RST);
                            osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ ヤバいよこれ！ ☆☆☆☆☆ \n" VT_RST);
                            maskIdx = Rand_ZeroFloat(7.99f);
                        }

                        resultIdx = sResultTable[rand9][maskIdx];
                        reaction = sResultValues[resultIdx][0];
                        self->action = sResultValues[resultIdx][1];
                        switch (self->action) {
                            case DNT_ACTION_LOW_RUPEES:
                                Audio_QueueSeqCmd(0x2D);
                                break;
                            case DNT_ACTION_ATTACK:
                                if (self->subCamera != SUBCAM_FREE) {
                                    self->subCamera = SUBCAM_FREE;
                                    OnePointCutscene_Init(globalCtx, 2350, -99, &self->scrubs[3]->actor, MAIN_CAM);
                                }
                                Audio_QueueSeqCmd(0x81A);
                                break;
                            case DNT_ACTION_DANCE:
                                Audio_QueueSeqCmd(0x55);
                                break;
                        }
                        osSyncPrintf("\n\n");
                        // "Each index 1"
                        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 各インデックス１ ☆☆☆☆☆ %d\n" VT_RST, rand9);
                        // "Each index 2"
                        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 各インデックス２ ☆☆☆☆☆ %d\n" VT_RST, maskIdx);
                        // "Each index 3"
                        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 各インデックス３ ☆☆☆☆☆ %d\n" VT_RST, resultIdx);
                        osSyncPrintf("\n");
                        // "What kind of evaluation?"
                        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ どういう評価？  ☆☆☆☆☆☆ %d\n" VT_RST, reaction);
                        // "What kind of action?"
                        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ どういうアクション？  ☆☆☆ %d\n" VT_RST, self->action);
                        osSyncPrintf("\n\n");
                        break;
                    }
            }
            if (reaction != DNT_SIGNAL_NONE) {
                for (i = 0; i < 9; i++) {
                    if (delay != 0) {
                        self->scrubs[i]->timer3 = delay * i;
                    }
                    self->scrubs[i]->action = self->action;
                    self->scrubs[i]->stageSignal = reaction;
                    self->scrubs[i]->ignore = ignore;
                    if (self->prize != DNT_PRIZE_NONE) {
                        self->scrubs[i]->timer1 = 300;
                        self->scrubs[i]->stagePrize = self->prize;
                        self->scrubs[i]->targetPos = self->leader->actor.world.pos;
                        if (self->prize == DNT_PRIZE_NUTS) {
                            self->leader->stageSignal = DNT_LEADER_SIGNAL_UP;
                        }
                        if (self->prize == DNT_PRIZE_STICK) {
                            self->leader->timer = 300;
                        }
                    }
                }
                self->actionFunc = EnDntDemo_Results;
            }
        }
    }
}

void EnDntDemo_Results(EnDntDemo* self, GlobalContext* globalCtx) {
    s32 i;

    if (self->leaderSignal != DNT_SIGNAL_NONE) {
        for (i = 0; i < 9; i++) {
            self->scrubs[i]->action = self->action;
            self->scrubs[i]->stageSignal = self->leaderSignal;
            self->scrubs[i]->stagePrize = DNT_PRIZE_NONE;
        }
        if (self->leader->action == DNT_LEADER_ACTION_UP) {
            self->leader->stageSignal = DNT_LEADER_SIGNAL_BURROW;
        } else if (self->leader->unburrow) {
            self->leader->stageSignal = DNT_LEADER_SIGNAL_RETURN;
        } else {
            self->leader->action = DNT_LEADER_ACTION_ATTACK;
        }
        self->leader->timer = 0;
        self->leaderSignal = self->action = DNT_SIGNAL_NONE;
        self->actionFunc = EnDntDemo_Prize;
    } else if (self->prize == DNT_PRIZE_STICK) {
        for (i = 0; i < 9; i++) {
            s16 offsetAngle = -self->leader->actor.shape.rot.y;
            Vec3f leaderPos = self->leader->actor.world.pos;
            f32 offsetDist;

            if (!(i & 1)) {
                offsetAngle -= 0x59D8;
            }
            offsetDist = ((i + 1) * 20.0f) + 20.0f;
            self->scrubs[i]->timer2 = 10;
            self->scrubs[i]->targetPos.x = leaderPos.x + Math_SinS(offsetAngle) * offsetDist;
            self->scrubs[i]->targetPos.y = leaderPos.y;
            self->scrubs[i]->targetPos.z = leaderPos.z + Math_CosS(offsetAngle) * offsetDist;
        }
    }
}

void EnDntDemo_Prize(EnDntDemo* self, GlobalContext* globalCtx) {
    s32 i;

    if (self->leaderSignal != DNT_SIGNAL_NONE) {
        for (i = 0; i < 9; i++) {
            self->scrubs[i]->action = self->action;
            self->scrubs[i]->stageSignal = self->leaderSignal;
            self->scrubs[i]->stagePrize = DNT_PRIZE_NONE;
        }
        self->leaderSignal = self->action = DNT_SIGNAL_NONE;
    }
}

void EnDntDemo_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDntDemo* self = THIS;

    if (self->unkTimer2 != 0) {
        self->unkTimer2--;
    }
    if (self->unkTimer1 != 0) {
        self->unkTimer1--;
    }
    self->actionFunc(self, globalCtx);
    if (BREG(0)) {
        if (self->debugArrowTimer != 0) {
            if (!(self->debugArrowTimer & 1)) {
                DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                       self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 120, 120, 0, 255, 4, globalCtx->state.gfxCtx);
            }
        } else {
            DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                   self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                   1.0f, 1.0f, 255, 255, 255, 255, 4, globalCtx->state.gfxCtx);
        }
    }
}
