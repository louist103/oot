#include "z_en_bom_bowl_man.h"
#include "vt.h"
#include "overlays/actors/ovl_En_Syateki_Niw/z_en_syateki_niw.h"
#include "overlays/actors/ovl_En_Ex_Item/z_en_ex_item.h"
#include "objects/object_bg/object_bg.h"

#define FLAGS 0x08000039

#define THIS ((EnBomBowlMan*)thisx)

typedef enum {
    /* 0 */ CHU_GIRL_EYES_ASLEEP,
    /* 1 */ CHU_GIRL_EYES_OPEN_SLOWLY,
    /* 2 */ CHU_GIRL_EYES_BLINK_RAPIDLY,
    /* 3 */ CHU_GIRL_EYES_AWAKE
} BombchuGirlEyeMode;

void EnBomBowlMan_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlMan_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlMan_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlMan_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnBomBowMan_SetupWaitAsleep(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_WaitAsleep(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_TalkAsleep(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_WakeUp(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_BlinkAwake(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_CheckBeatenDC(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_WaitNotBeatenDC(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_TalkNotBeatenDC(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_SetupRunGame(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_RunGame(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowlMan_HandlePlayChoice(EnBomBowlMan* self, GlobalContext* globalCtx);
void func_809C41FC(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_SetupChooseShowPrize(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowMan_ChooseShowPrize(EnBomBowlMan* self, GlobalContext* globalCtx);
void EnBomBowlMan_BeginPlayGame(EnBomBowlMan* self, GlobalContext* globalCtx);

const ActorInit En_Bom_Bowl_Man_InitVars = {
    ACTOR_EN_BOM_BOWL_MAN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_BG,
    sizeof(EnBomBowlMan),
    (ActorFunc)EnBomBowlMan_Init,
    (ActorFunc)EnBomBowlMan_Destroy,
    (ActorFunc)EnBomBowlMan_Update,
    (ActorFunc)EnBomBowlMan_Draw,
};

void EnBomBowlMan_Init(Actor* thisx, GlobalContext* globalCtx2) {
    static f32 cuccoColliderDims[][3] = { { 16.0f, 46.0f, 0.0f }, { 36.0f, 56.0f, 0.0f } };
    static Vec3f cuccoSpawnPos[] = { { 60, -60, -430 }, { 0, -120, -620 } };
    static f32 cuccoScales[] = { 0.01f, 0.03f };
    EnBomBowlMan* self = THIS;
    EnSyatekiNiw* cucco;
    s32 i;
    GlobalContext* globalCtx = globalCtx2;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gChuGirlSkel, &gChuGirlNoddingOffAnim, self->jointTable,
                       self->morphTable, 11);
    // "☆ Man, my shoulders hurt~ ☆"
    osSyncPrintf(VT_FGCOL(GREEN) "☆ もー 肩こっちゃうよねぇ〜 \t\t ☆ \n" VT_RST);
    // "☆ Isn't there some sort of job that will pay better and be more relaxing? ☆ %d"
    osSyncPrintf(VT_FGCOL(GREEN) "☆ もっとラクしてもうかるバイトないかしら？ ☆ %d\n" VT_RST,
                 globalCtx->bombchuBowlingStatus);
    self->posCopy = self->actor.world.pos;
    self->actor.shape.yOffset = -60.0f;
    Actor_SetScale(&self->actor, 0.013f);

    for (i = 0; i < 2; i++) {
        cucco = (EnSyatekiNiw*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_SYATEKI_NIW, cuccoSpawnPos[i].x,
                                           cuccoSpawnPos[i].y, cuccoSpawnPos[i].z, 0, 0, 0, 1);

        if (cucco != NULL) {
            cucco->unk_2F4 = cuccoScales[i];
            cucco->collider.dim.radius = (s16)cuccoColliderDims[i][0];
            cucco->collider.dim.height = (s16)cuccoColliderDims[i][1];
        }
    }

    self->prizeSelect = (s16)Rand_ZeroFloat(4.99f);
    self->actor.targetMode = 1;
    self->actionFunc = EnBomBowMan_SetupWaitAsleep;
}

void EnBomBowlMan_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnBomBowMan_SetupWaitAsleep(EnBomBowlMan* self, GlobalContext* globalCtx) {
    self->frameCount = (f32)Animation_GetLastFrame(&gChuGirlNoddingOffAnim);
    Animation_Change(&self->skelAnime, &gChuGirlNoddingOffAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_LOOP, -10.0f);
    self->actor.textId = 0xC0;
    self->dialogState = 5;
    self->actionFunc = EnBomBowMan_WaitAsleep;
}

void EnBomBowMan_WaitAsleep(EnBomBowlMan* self, GlobalContext* globalCtx) {
    s16 yawDiff;

    SkelAnime_Update(&self->skelAnime);

    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnBomBowMan_TalkAsleep;
    } else {
        yawDiff = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y));

        if (!(self->actor.xzDistToPlayer > 120.0f) && (yawDiff < 0x4300)) {
            func_8002F2CC(&self->actor, globalCtx, 120.0f);
        }
    }
}

void EnBomBowMan_TalkAsleep(EnBomBowlMan* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if ((func_8010BDBC(&globalCtx->msgCtx) == self->dialogState) && (func_80106BC8(globalCtx) != 0)) {
        globalCtx->msgCtx.msgMode = 0x37;
        self->actionFunc = EnBomBowMan_WakeUp;
    }
}

void EnBomBowMan_WakeUp(EnBomBowlMan* self, GlobalContext* globalCtx) {
    self->frameCount = (f32)Animation_GetLastFrame(&gChuGirlWakeUpAnim);
    Animation_Change(&self->skelAnime, &gChuGirlWakeUpAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_ONCE, -10.0f);
    self->eyeMode = CHU_GIRL_EYES_OPEN_SLOWLY;
    self->actionFunc = EnBomBowMan_BlinkAwake;
}

void EnBomBowMan_BlinkAwake(EnBomBowlMan* self, GlobalContext* globalCtx) {
    f32 frameCount = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if (frameCount == 30.0f) {
        self->dialogState = 5;

        // Check for beaten Dodongo's Cavern
        if ((gSaveContext.eventChkInf[2] & 0x20) || BREG(2)) {
            self->actor.textId = 0xBF;
        } else {
            self->actor.textId = 0x7058;
        }
    }
    func_8010B720(globalCtx, self->actor.textId);

    if ((self->eyeTextureIndex == 0) && (self->eyeMode == CHU_GIRL_EYES_BLINK_RAPIDLY) && (self->blinkTimer == 0)) {
        // Blink twice, then move on
        self->eyeTextureIndex = 2;
        self->blinkCount++;
        if (self->blinkCount >= 3) {
            self->actionFunc = EnBomBowMan_CheckBeatenDC;
        }
    }
}

void EnBomBowMan_CheckBeatenDC(EnBomBowlMan* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if ((func_8010BDBC(&globalCtx->msgCtx) == self->dialogState) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->frameCount = (f32)Animation_GetLastFrame(&gChuGirlLeanOverCounterAnim);
        Animation_Change(&self->skelAnime, &gChuGirlLeanOverCounterAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_LOOP,
                         -10.0f);
        self->eyeMode = CHU_GIRL_EYES_AWAKE;
        self->blinkTimer = (s16)Rand_ZeroFloat(60.0f) + 20;

        // Check for beaten Dodongo's Cavern
        if (!((gSaveContext.eventChkInf[2] & 0x20) || BREG(2))) {
            self->actionFunc = EnBomBowMan_WaitNotBeatenDC;
        } else {
            self->actor.textId = 0x18;
            self->dialogState = 4;
            func_8010B720(globalCtx, self->actor.textId);
            self->actionFunc = EnBomBowlMan_HandlePlayChoice;
        }
    }
}

void EnBomBowMan_WaitNotBeatenDC(EnBomBowlMan* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnBomBowMan_TalkNotBeatenDC;
    } else {
        func_8002F2CC(&self->actor, globalCtx, 120.0f);
    }
}

void EnBomBowMan_TalkNotBeatenDC(EnBomBowlMan* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if ((func_8010BDBC(&globalCtx->msgCtx) == self->dialogState) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->actionFunc = EnBomBowMan_WaitNotBeatenDC;
    }
}

void EnBomBowMan_SetupRunGame(EnBomBowlMan* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (self->minigamePlayStatus == 0) {
        if (!self->startedPlaying) {
            self->actor.textId = 0x18;
        } else {
            self->actor.textId = 0x1A;
        }

        self->dialogState = 4;
    } else {
        self->actor.textId = 0x19;
        self->dialogState = 5;
    }

    self->actionFunc = EnBomBowMan_RunGame;
}

void EnBomBowMan_RunGame(EnBomBowlMan* self, GlobalContext* globalCtx) {
    s16 yawDiff;

    SkelAnime_Update(&self->skelAnime);

    if (BREG(3)) {
        osSyncPrintf(VT_FGCOL(RED) "☆ game_play->bomchu_game_flag ☆ %d\n" VT_RST, globalCtx->bombchuBowlingStatus);
        // "HOW'S THE FIRST WALL DOING?"
        osSyncPrintf(VT_FGCOL(RED) "☆ 壁１の状態どう？ ☆ %d\n" VT_RST, self->wallStatus[0]);
        // "HOW'S THE SECOND WALL DOING?"
        osSyncPrintf(VT_FGCOL(RED) "☆ 壁２の状態どう？ ☆ %d\n" VT_RST, self->wallStatus[1]);
        // "HOLE INFORMATION"
        osSyncPrintf(VT_FGCOL(RED) "☆ 穴情報\t     ☆ %d\n" VT_RST, self->bowlPit->status);
        osSyncPrintf("\n\n");
    }

    self->gameResult = 0;

    if (self->bowlPit != NULL) {
        if ((self->wallStatus[0] != 1) && (self->wallStatus[1] != 1) && (self->bowlPit->status == 2)) {
            self->gameResult = 1; // Won
            self->bowlPit->status = 0;
            // "Center HIT!"
            osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 中央ＨＩＴ！！！！ ☆☆☆☆☆ \n" VT_RST);
        }

        if ((globalCtx->bombchuBowlingStatus == -1) &&
            (globalCtx->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].length == 0) && (self->bowlPit->status == 0) &&
            (self->wallStatus[0] != 1) && (self->wallStatus[1] != 1)) {
            self->gameResult = 2; // Lost
            // "Bombchu lost"
            osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ ボムチュウ消化 ☆☆☆☆☆ \n" VT_RST);
        }
    }

    if (self->gameResult != 0) { // won or lost
        self->actor.textId = 0x1A;
        self->dialogState = 4;
        self->minigamePlayStatus = 0;

        if ((self->exItem != NULL) && (self->exItem->actor.update != NULL)) {
            self->exItem->killItem = true;
            self->exItem = NULL;
        }

        globalCtx->bombchuBowlingStatus = 0;
        self->playingAgain = true;
        func_8010B680(globalCtx, self->actor.textId, NULL);

        if (self->gameResult == 2) {
            func_8002DF54(globalCtx, NULL, 8);
        }
        self->actionFunc = EnBomBowlMan_HandlePlayChoice;
    } else {
        if (func_8002F194(&self->actor, globalCtx)) {
            if (self->minigamePlayStatus == 0) {
                self->actionFunc = EnBomBowlMan_HandlePlayChoice;
            } else {
                self->actionFunc = func_809C41FC;
            }
        } else {
            yawDiff = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y));

            if (!(self->actor.xzDistToPlayer > 120.0f) && (yawDiff < 0x4300)) {
                func_8002F2CC(&self->actor, globalCtx, 120.0f);
            }
        }
    }
}

void EnBomBowlMan_HandlePlayChoice(EnBomBowlMan* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if ((func_8010BDBC(&globalCtx->msgCtx) == self->dialogState) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);

        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // Yes
                if (gSaveContext.rupees >= 30) {
                    Rupees_ChangeBy(-30);
                    self->minigamePlayStatus = 1;
                    self->wallStatus[0] = self->wallStatus[1] = 0;
                    globalCtx->bombchuBowlingStatus = 10;
                    Flags_SetSwitch(globalCtx, 0x38);

                    if (!self->startedPlaying && !self->playingAgain) {
                        self->actor.textId = 0x19;
                        func_8010B720(globalCtx, self->actor.textId);
                        self->dialogState = 5;
                        self->actionFunc = func_809C41FC;
                    } else {
                        self->actor.textId = 0x1B;
                        func_8010B720(globalCtx, self->actor.textId);
                        self->dialogState = 5;
                        OnePointCutscene_Init(globalCtx, 8010, -99, NULL, MAIN_CAM);
                        func_8002DF54(globalCtx, NULL, 8);
                        self->actionFunc = EnBomBowMan_SetupChooseShowPrize;
                    }
                } else {
                    self->playingAgain = false;
                    self->actor.textId = 0x85;
                    func_8010B720(globalCtx, self->actor.textId);
                    self->dialogState = 5;
                    self->actionFunc = func_809C41FC;
                }
                break;

            case 1: // No
                self->playingAgain = false;
                self->actor.textId = 0x2D;
                func_8010B720(globalCtx, self->actor.textId);
                self->dialogState = 5;
                self->actionFunc = func_809C41FC;
        }
    }
}

void func_809C41FC(EnBomBowlMan* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((func_8010BDBC(&globalCtx->msgCtx) == self->dialogState) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);

        if (((self->actor.textId == 0x2D) || (self->actor.textId == 0x85)) && Flags_GetSwitch(globalCtx, 0x38)) {
            Flags_UnsetSwitch(globalCtx, 0x38);
        }

        if (self->minigamePlayStatus == 1) {
            self->actor.textId = 0x1B;
            func_8010B720(globalCtx, self->actor.textId);
            self->dialogState = 5;
            OnePointCutscene_Init(globalCtx, 8010, -99, NULL, MAIN_CAM);
            func_8002DF54(globalCtx, NULL, 8);
            self->actionFunc = EnBomBowMan_SetupChooseShowPrize;
        } else {
            if (self->gameResult == 2) {
                func_8002DF54(globalCtx, NULL, 7);
            }
            self->actionFunc = EnBomBowMan_SetupRunGame;
        }
    }
}

void EnBomBowMan_SetupChooseShowPrize(EnBomBowlMan* self, GlobalContext* globalCtx) {
    Vec3f accel = { 0.0f, 0.1f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;

    SkelAnime_Update(&self->skelAnime);

    if ((func_8010BDBC(&globalCtx->msgCtx) == self->dialogState) && (func_80106BC8(globalCtx) != 0)) {
        pos.x = 148.0f;
        pos.y = 40.0f;
        pos.z = 300.0f;
        EffectSsBomb2_SpawnLayered(globalCtx, &pos, &velocity, &accel, 50, 15);
        Audio_PlayActorSound2(&self->actor, NA_SE_IT_GOODS_APPEAR);
        self->prizeRevealTimer = 10;
        self->actionFunc = EnBomBowMan_ChooseShowPrize;
    }
}

static Vec3f sPrizePosOffset[] = {
    { 0.0f, 22.0f, 0.0f }, { 0.0f, 22.0f, 0.0f }, { 0.0f, 8.0f, 0.0f }, { 0.0f, 9.0f, 0.0f }, { 0.0f, -2.0f, 0.0f },
};

static s16 sPrizeRot[] = { 0x4268, 0x4268, -0x03E8, 0x0000, 0x4268, 0x0000 };

void EnBomBowMan_ChooseShowPrize(EnBomBowlMan* self, GlobalContext* globalCtx) {
    s16 prizeTemp;
    s32 pad;

    SkelAnime_Update(&self->skelAnime);

    if (self->prizeRevealTimer == 0) {
        switch (self->prizeSelect) {
            case 0:
                prizeTemp = EXITEM_BOMB_BAG_BOWLING;
                if (gSaveContext.itemGetInf[1] & 2) {
                    prizeTemp = EXITEM_PURPLE_RUPEE_BOWLING;
                }
                break;
            case 1:
                prizeTemp = EXITEM_PURPLE_RUPEE_BOWLING;
                break;
            case 2:
                prizeTemp = EXITEM_BOMBCHUS_BOWLING;
                break;
            case 3:
                prizeTemp = EXITEM_HEART_PIECE_BOWLING;
                if (gSaveContext.itemGetInf[1] & 4) {
                    prizeTemp = EXITEM_PURPLE_RUPEE_BOWLING;
                }
                break;
            case 4:
                prizeTemp = EXITEM_BOMBS_BOWLING;
                break;
        }

        self->prizeIndex = prizeTemp;

        if (BREG(7)) {
            self->prizeIndex = BREG(7) - 1;
        }

        self->exItem = (EnExItem*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_EX_ITEM,
                                                     sPrizePosOffset[self->prizeIndex].x + 148.0f,
                                                     sPrizePosOffset[self->prizeIndex].y + 40.0f,
                                                     sPrizePosOffset[self->prizeIndex].z + 300.0f, 0,
                                                     sPrizeRot[self->prizeIndex], 0, self->prizeIndex + EXITEM_COUNTER);

        if (!self->startedPlaying) {
            self->bowlPit = (EnBomBowlPit*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx,
                                                              ACTOR_EN_BOM_BOWL_PIT, 0.0f, 90.0f, -860.0f, 0, 0, 0, 0);
            if (self->bowlPit != NULL) {
                self->bowlPit->prizeIndex = self->prizeIndex;
            }
        } else {
            self->bowlPit->prizeIndex = self->prizeIndex;
        }

        self->bowlPit->start = 1;
        self->minigamePlayStatus = 2;
        self->actor.textId = 0x405A;
        func_8010B720(globalCtx, self->actor.textId);
        self->dialogState = 5;

        // Cycle through prizes in order
        self->prizeSelect++;
        if (self->prizeSelect >= 5) {
            self->prizeSelect = 0;
        }
        self->actionFunc = EnBomBowlMan_BeginPlayGame;
    }
}

void EnBomBowlMan_BeginPlayGame(EnBomBowlMan* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if ((func_8010BDBC(&globalCtx->msgCtx) == self->dialogState) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        func_8005B1A4(GET_ACTIVE_CAM(globalCtx));
        self->startedPlaying = true;

        if (BREG(2)) {
            BREG(2) = 0;
        }

        // "Wow"
        osSyncPrintf(VT_FGCOL(YELLOW) "☆ わー ☆ %d\n" VT_RST, globalCtx->bombchuBowlingStatus);
        func_8002DF54(globalCtx, NULL, 7);
        self->actionFunc = EnBomBowMan_SetupRunGame;
    }
}

void EnBomBowlMan_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlMan* self = THIS;

    self->timer++;
    self->actor.focus.pos.y = 60.0f;
    Actor_SetFocus(&self->actor, 60.0f);

    switch (self->eyeMode) {
        case CHU_GIRL_EYES_ASLEEP:
            self->eyeTextureIndex = 2;
            break;
        case CHU_GIRL_EYES_OPEN_SLOWLY:
            if (self->eyeTextureIndex > 0) {
                self->eyeTextureIndex--;
            } else {
                self->blinkTimer = 30;
                self->eyeMode = CHU_GIRL_EYES_BLINK_RAPIDLY;
            }
            break;
        case CHU_GIRL_EYES_BLINK_RAPIDLY:
            if ((self->blinkTimer == 0) && (self->eyeTextureIndex > 0)) {
                self->eyeTextureIndex--;
            }
            break;
        default:
            if (self->blinkTimer == 0) {
                self->eyeTextureIndex++;
                if (self->eyeTextureIndex >= 3) {
                    self->eyeTextureIndex = 0;
                    self->blinkTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
                }
            }

            func_80038290(globalCtx, &self->actor, &self->unk_218, &self->unk_224, self->actor.focus.pos);
            break;
    }

    DECR(self->prizeRevealTimer);
    DECR(self->blinkTimer);

    self->actionFunc(self, globalCtx);
}

s32 EnBomBowlMan_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                  void* thisx) {
    EnBomBowlMan* self = THIS;

    if (limbIndex == 4) { // head
        rot->x += self->unk_218.y;
        rot->z += self->unk_218.z;
    }

    return false;
}

void EnBomBowlMan_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { gChuGirlEyeOpenTex, gChuGirlEyeHalfTex, gChuGirlEyeClosedTex };
    EnBomBowlMan* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bom_bowl_man.c", 907);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTextureIndex]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnBomBowlMan_OverrideLimbDraw, NULL, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bom_bowl_man.c", 923);
}
