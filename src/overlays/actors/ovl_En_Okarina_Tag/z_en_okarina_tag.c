/*
 * File: z_en_okarina_tag.c
 * Overlay: ovl_En_Okarina_Tag
 * Description: Music Staff (Ocarina) spot
 */

#include "z_en_okarina_tag.h"
#include "scenes/misc/hakaana_ouke/hakaana_ouke_scene.h"
#include "scenes/overworld/spot02/spot02_scene.h"
#include "vt.h"

#define FLAGS 0x02000010

#define THIS ((EnOkarinaTag*)thisx)

void EnOkarinaTag_Init(Actor* thisx, GlobalContext* globalCtx);
void EnOkarinaTag_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnOkarinaTag_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80ABEF2C(EnOkarinaTag* self, GlobalContext* globalCtx);
void func_80ABF708(EnOkarinaTag* self, GlobalContext* globalCtx);
void func_80ABF28C(EnOkarinaTag* self, GlobalContext* globalCtx);
void func_80ABF0CC(EnOkarinaTag* self, GlobalContext* globalCtx);
void func_80ABF4C8(EnOkarinaTag* self, GlobalContext* globalCtx);
void func_80ABF7CC(EnOkarinaTag* self, GlobalContext* globalCtx);

const ActorInit En_Okarina_Tag_InitVars = {
    ACTOR_EN_OKARINA_TAG,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnOkarinaTag),
    (ActorFunc)EnOkarinaTag_Init,
    (ActorFunc)EnOkarinaTag_Destroy,
    (ActorFunc)EnOkarinaTag_Update,
    NULL,
};

extern CutsceneData D_80ABF9D0[];
extern CutsceneData D_80ABFB40[];

void EnOkarinaTag_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnOkarinaTag_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnOkarinaTag* self = THIS;

    osSyncPrintf("\n\n");
    // "Ocarina tag outbreak"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ オカリナタグ発生 ☆☆☆☆☆ %x\n" VT_RST, self->actor.params);
    self->actor.flags &= ~1;
    self->unk_150 = (self->actor.params >> 0xA) & 0x3F;
    self->unk_152 = (self->actor.params >> 6) & 0xF;
    self->switchFlag = self->actor.params & 0x3F;
    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    if (self->unk_152 == 0xF) {
        self->unk_152 = 0;
        self->unk_158 = 1;
    }
    self->actor.targetMode = 1;
    if (self->actor.world.rot.z > 0) {
        self->unk_15C = self->actor.world.rot.z * 40.0f;
    }

    // "Save information"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ セーブ情報\t ☆☆☆☆☆ %d\n" VT_RST, self->switchFlag);
    // "Type index"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 種類インデックス ☆☆☆☆☆ %d\n" VT_RST, self->unk_150);
    // "Correct answer information"
    osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 正解情報\t ☆☆☆☆☆ %d\n" VT_RST, self->unk_152);
    // "Range information"
    osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 範囲情報\t ☆☆☆☆☆ %d\n" VT_RST, self->actor.world.rot.z);
    // "Processing range information"
    osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 処理範囲情報\t ☆☆☆☆☆ %f\n" VT_RST, self->unk_15C);
    // "Hit?"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 当り？\t\t ☆☆☆☆☆ %d\n" VT_RST, self->unk_158);
    osSyncPrintf("\n\n");

    if ((self->switchFlag >= 0) && (Flags_GetSwitch(globalCtx, self->switchFlag))) {
        Actor_Kill(&self->actor);
    } else {
        switch (self->unk_150) {
            case 7:
                self->actionFunc = func_80ABEF2C;
                break;
            case 2:
                if (LINK_IS_ADULT) {
                    Actor_Kill(&self->actor);
                    break;
                }
            case 1:
            case 4:
            case 6:
                self->actionFunc = func_80ABF28C;
                break;
            case 5:
                self->actor.textId = 0x5021;
                self->actionFunc = func_80ABF708;
                break;
            default:
                Actor_Kill(&self->actor);
                break;
        }
    }
}

void func_80ABEF2C(EnOkarinaTag* self, GlobalContext* globalCtx) {
    Player* player;
    u16 unk_152;

    player = GET_PLAYER(globalCtx);
    self->unk_15A++;
    if ((self->switchFlag >= 0) && (Flags_GetSwitch(globalCtx, self->switchFlag))) {
        self->actor.flags &= ~1;
    } else {
        if ((self->unk_152 != 6) || (gSaveContext.scarecrowSpawnSongSet)) {
            if (player->stateFlags2 & 0x1000000) {
                // "North! ! ! ! !"
                osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ 北！！！！！ ☆☆☆☆☆ %f\n" VT_RST, self->actor.xzDistToPlayer);
            }
            if ((self->actor.xzDistToPlayer < (90.0f + self->unk_15C)) &&
                (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 80.0f)) {
                if (player->stateFlags2 & 0x2000000) {
                    unk_152 = self->unk_152;
                    if (unk_152 == 6) {
                        unk_152 = 0xA;
                    }
                    player->stateFlags2 |= 0x800000;
                    func_8010BD58(globalCtx, unk_152 + 0x22);
                    self->actionFunc = func_80ABF0CC;
                } else if ((self->actor.xzDistToPlayer < (50.0f + self->unk_15C) &&
                            ((fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 40.0f)))) {
                    self->unk_15A = 0;
                    player->unk_6A8 = &self->actor;
                }
            }
        }
    }
}

void func_80ABF0CC(EnOkarinaTag* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (globalCtx->msgCtx.unk_E3EE == 4) {
        self->actionFunc = func_80ABEF2C;
    } else {
        if (globalCtx->msgCtx.unk_E3EE == 3) {
            if (self->switchFlag >= 0) {
                Flags_SetSwitch(globalCtx, self->switchFlag);
            }
            if (globalCtx->sceneNum == SCENE_MIZUSIN) {
                globalCtx->msgCtx.msgMode = 0x37;
            }
            if ((globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) && (globalCtx->sceneNum != SCENE_YOUSEI_IZUMI_YOKO)) {
                globalCtx->msgCtx.unk_E3EE = 4;
            }
            func_80078884(NA_SE_SY_CORRECT_CHIME);
            self->actionFunc = func_80ABEF2C;
            return;
        }
        if (self->unk_158 != 0) {
            if ((globalCtx->msgCtx.unk_E3EE == 5) || (globalCtx->msgCtx.unk_E3EE == 6) ||
                (globalCtx->msgCtx.unk_E3EE == 7) || (globalCtx->msgCtx.unk_E3EE == 8) ||
                (globalCtx->msgCtx.unk_E3EE == 9) || (globalCtx->msgCtx.unk_E3EE == 10) ||
                (globalCtx->msgCtx.unk_E3EE == 13)) {
                if (self->switchFlag >= 0) {
                    Flags_SetSwitch(globalCtx, self->switchFlag);
                }
                globalCtx->msgCtx.unk_E3EE = 4;
                func_80078884(NA_SE_SY_CORRECT_CHIME);
                self->actionFunc = func_80ABEF2C;
                return;
            }
        }
        if ((globalCtx->msgCtx.unk_E3EE >= 5) && (globalCtx->msgCtx.unk_E3EE < 0xE)) {
            globalCtx->msgCtx.unk_E3EE = 4;
            self->actionFunc = func_80ABEF2C;
        } else if (globalCtx->msgCtx.unk_E3EE == 1) {
            player->stateFlags2 |= 0x800000;
        }
    }
}

void func_80ABF28C(EnOkarinaTag* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_15A++;
    if ((self->unk_152 != 6) || (gSaveContext.scarecrowSpawnSongSet)) {
        if ((self->switchFlag >= 0) && Flags_GetSwitch(globalCtx, self->switchFlag)) {
            self->actor.flags &= ~1;
        } else if (((self->unk_150 != 4) || !(gSaveContext.eventChkInf[4] & 0x800)) &&
                   ((self->unk_150 != 6) || !(gSaveContext.eventChkInf[1] & 0x2000)) &&
                   (self->actor.xzDistToPlayer < (90.0f + self->unk_15C)) &&
                   (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 80.0f)) {
            if (player->stateFlags2 & 0x1000000) {
                switch (self->unk_150) {
                    case 1:
                        func_8010BD58(globalCtx, 0x24);
                        break;
                    case 2:
                        func_8010BD58(globalCtx, 0x27);
                        break;
                    case 4:
                        func_8010BD58(globalCtx, 0x26);
                        break;
                    case 6:
                        func_8010BD58(globalCtx, 0x24);
                        break;
                    default:
                        // "Ocarina Invisible-kun demo start check error source"
                        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ オカリナ透明君デモ開始チェックエラー原 ☆☆☆☆☆ %d\n" VT_RST,
                                     self->unk_150);
                        Actor_Kill(&self->actor);
                        break;
                }
                player->stateFlags2 |= 0x800000;
                self->actionFunc = func_80ABF4C8;
            } else if ((self->actor.xzDistToPlayer < (50.0f + self->unk_15C)) &&
                       (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 40.0f)) {
                self->unk_15A = 0;
                player->stateFlags2 |= 0x800000;
            }
        }
    }
}

void func_80ABF4C8(EnOkarinaTag* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (globalCtx->msgCtx.unk_E3EE == 4) {
        self->actionFunc = func_80ABF28C;
    } else if (globalCtx->msgCtx.unk_E3EE == 3) {
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        if (self->switchFlag >= 0) {
            Flags_SetSwitch(globalCtx, self->switchFlag);
        }
        switch (self->unk_150) {
            case 1:
                Flags_SetSwitch(globalCtx, self->switchFlag);
                gSaveContext.eventChkInf[3] |= 0x200;
                break;
            case 2:
                globalCtx->csCtx.segment = D_80ABF9D0;
                gSaveContext.cutsceneTrigger = 1;
                func_800F574C(1.18921f, 0x5A);
                break;
            case 4:
                globalCtx->csCtx.segment = D_80ABFB40;
                gSaveContext.cutsceneTrigger = 1;
                break;
            case 6:
                globalCtx->csCtx.segment = LINK_IS_ADULT ? SEGMENTED_TO_VIRTUAL(&spot02_sceneCutsceneData0x003C80)
                                                         : SEGMENTED_TO_VIRTUAL(&spot02_scene_Cs_005020);
                gSaveContext.cutsceneTrigger = 1;
                gSaveContext.eventChkInf[1] |= 0x2000;
                func_80078884(NA_SE_SY_CORRECT_CHIME);
                break;
            default:
                break;
        }
        globalCtx->msgCtx.unk_E3EE = 4;
        self->actionFunc = func_80ABF28C;
    } else {
        if (globalCtx->msgCtx.unk_E3EE >= 5) {
            if (globalCtx->msgCtx.unk_E3EE < 0xE) {
                globalCtx->msgCtx.unk_E3EE = 4;
                self->actionFunc = func_80ABF28C;
                return;
            }
        }
        if (globalCtx->msgCtx.unk_E3EE == 1) {
            player->stateFlags2 |= 0x800000;
        }
    }
}

void func_80ABF708(EnOkarinaTag* self, GlobalContext* globalCtx) {
    s16 yawDiff;
    s16 yawDiffNew;

    if (func_8002F194(&self->actor, globalCtx) != 0) {
        self->actionFunc = func_80ABF7CC;
    } else {
        yawDiff = self->actor.yawTowardsPlayer - self->actor.world.rot.y;
        self->unk_15A++;
        if (!(self->actor.xzDistToPlayer > 120.0f)) {
            if (CHECK_QUEST_ITEM(QUEST_SONG_SUN)) {
                self->actor.textId = 0x5021;
            }
            yawDiffNew = ABS(yawDiff);
            if (yawDiffNew < 0x4300) {
                self->unk_15A = 0;
                func_8002F2CC(&self->actor, globalCtx, 70.0f);
            }
        }
    }
}

void func_80ABF7CC(EnOkarinaTag* self, GlobalContext* globalCtx) {
    // "Open sesame sesame!"
    osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 開けゴマゴマゴマ！ ☆☆☆☆☆ %d\n" VT_RST, func_8010BDBC(&globalCtx->msgCtx));

    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        if (!CHECK_QUEST_ITEM(QUEST_SONG_SUN)) {
            globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(&gSunSongTeachCs);
            gSaveContext.cutsceneTrigger = 1;
        }
        self->actionFunc = func_80ABF708;
    }
}

void EnOkarinaTag_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnOkarinaTag* self = THIS;

    self->actionFunc(self, globalCtx);
    if (BREG(0) != 0) {
        if (self->unk_15A != 0) {
            if (!(self->unk_15A & 1)) {
                DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                       self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 120, 120, 120, 255, 4, globalCtx->state.gfxCtx);
            }
        } else {
            DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                   self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                   1.0f, 1.0f, 255, 0, 0, 255, 4, globalCtx->state.gfxCtx);
        }
    }
}
