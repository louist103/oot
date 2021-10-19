/*
 * File: z_en_wonder_talk.c
 * Overlay: ovl_En_Wonder_Talk
 * Description: Checkable spot (Green Navi)
 */

#include "z_en_wonder_talk.h"
#include "vt.h"

#define FLAGS 0x08000009

#define THIS ((EnWonderTalk*)thisx)

void EnWonderTalk_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWonderTalk_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWonderTalk_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80B391CC(EnWonderTalk* self, GlobalContext* globalCtx);
void func_80B395F0(EnWonderTalk* self, GlobalContext* globalCtx);
void func_80B3943C(EnWonderTalk* self, GlobalContext* globalCtx);

const ActorInit En_Wonder_Talk_InitVars = {
    ACTOR_EN_WONDER_TALK,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWonderTalk),
    (ActorFunc)EnWonderTalk_Init,
    (ActorFunc)EnWonderTalk_Destroy,
    (ActorFunc)EnWonderTalk_Update,
    NULL,
};

void EnWonderTalk_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnWonderTalk_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnWonderTalk* self = THIS;

    osSyncPrintf("\n\n");
    // "Special conversation"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 特殊会話くん ☆☆☆☆☆ %x\n" VT_RST, self->actor.params);

    self->unk_150 = (self->actor.params >> 0xB) & 0x1F;
    self->unk_152 = (self->actor.params >> 6) & 0x1F;
    self->switchFlag = self->actor.params & 0x3F;
    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    self->actor.targetMode = 1;
    if (self->switchFlag >= 0) {
        if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ Ｙｏｕ ａｒｅ Ｓｈｏｃｋ！  ☆☆☆☆☆ %d\n" VT_RST, self->switchFlag);
            Actor_Kill(&self->actor);
            return;
        }
    }
    self->actionFunc = func_80B391CC;
    self->unk_15C = 40.0f;
}

void func_80B391CC(EnWonderTalk* self, GlobalContext* globalCtx) {
    if (self->switchFlag < 0 || !Flags_GetSwitch(globalCtx, self->switchFlag)) {
        switch (self->unk_150) {
            case 1:
                // "Slate GO!"
                osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 石板ＧＯ！ ☆☆☆☆☆ \n" VT_RST);
                self->height = 0.0f;
                self->unk_15C = 80.0f;
                // "Attention coordinates"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 注目座標\t       \t☆☆☆☆☆ %f\n" VT_RST, 0.0f);
                if (!LINK_IS_ADULT) {
                    self->actor.textId = 0x7040;
                    // "Children"
                    osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ こども ☆☆☆☆☆ \n" VT_RST);
                } else {
                    // "Adult"
                    osSyncPrintf(VT_FGCOL(CYAN) " ☆☆☆☆☆ おとな ☆☆☆☆☆ \n" VT_RST);
                    self->actor.textId = 0x7088;
                }

                self->unk_156 = 5;
                osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ self->actor.talk_message    ☆☆☆☆☆ %x\n" VT_RST, self->actor.textId);
                break;
            case 2:
                // "Diary start!"
                osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 日記帳スタート！ ☆☆☆☆☆ \n" VT_RST);
                self->actor.textId = 0x5002;
                self->unk_156 = 4;
                self->height = 30.0f;
                self->unk_15C = 40.0f;
                // "Attention coordinates"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 注目座標\t       \t☆☆☆☆☆ %f\n" VT_RST, 30.0f);
                break;
            case 3:
                self->actor.textId = 0x501E;
                self->unk_156 = 5;
                self->height = 0.0f;
                self->unk_15C = 110.0f;
                // "Attention coordinates"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 注目座標\t       \t☆☆☆☆☆ %f\n" VT_RST, 0.0f);
                break;
            case 4:
                self->actor.textId = 0x5020;
                self->unk_156 = 6;
                self->height = 0.0f;
                // "Attention coordinates"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 注目座標\t       \t☆☆☆☆☆ %f\n" VT_RST, 0.0f);
                self->unk_15C = 120.0f;
                if (gSaveContext.eventChkInf[1] & 0x2000) {
                    Actor_Kill(&self->actor);
                }
                break;
            case 5:
                self->actor.textId = 0x501F;
                self->unk_156 = 5;
                self->height = 0.0f;
                self->unk_15C = 110.0f;
                // "Attention coordinates"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 注目座標\t       \t☆☆☆☆☆ %f\n" VT_RST, 0.0f);
                break;
            default:
                self->actor.textId = 0x7072;
                self->unk_156 = 5;
                break;
        }

        osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ self->actor.talk_message    ☆☆☆☆☆ %x\n" VT_RST, self->actor.textId);
        self->actionFunc = func_80B3943C;
    }
}

void func_80B3943C(EnWonderTalk* self, GlobalContext* globalCtx) {
    s16 yawDiff;
    s16 yawDiffTemp;

    self->unk_15A++;
    if (self->unk_150 == 4 && (gSaveContext.eventChkInf[1] & 0x2000)) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->switchFlag < 0 || !Flags_GetSwitch(globalCtx, self->switchFlag)) {
        if ((func_8002F194(&self->actor, globalCtx))) {
            if (self->unk_156 != 6) {
                self->actionFunc = func_80B395F0;
            } else {
                if (self->switchFlag >= 0) {
                    self->actor.flags &= ~1;
                    Flags_SetSwitch(globalCtx, self->switchFlag);
                }
                self->actionFunc = func_80B391CC;
            }
        } else if (!(self->unk_15C < self->actor.xzDistToPlayer)) {
            yawDiffTemp = (self->actor.yawTowardsPlayer - self->actor.world.rot.y);
            yawDiff = ABS(yawDiffTemp);

            if (yawDiff < 0x4000) {
                if (self->unk_15A >= 2) {
                    osSyncPrintf("\n\n");
                    // "Save information"
                    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ セーブ情報\t\t☆☆☆☆☆ %d\n" VT_RST, self->switchFlag);
                    // "Type index"
                    osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 種類インデックス\t☆☆☆☆☆ %d\n" VT_RST, self->unk_150);
                    // "Actual message type"
                    osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 実質メッセージ種類     %x\n" VT_RST, self->actor.textId);
                    // "Specified range"
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 指定範囲               %d\n" VT_RST, self->actor.world.rot.z);
                    osSyncPrintf("\n\n");
                }
                self->unk_15A = 0;
                func_8002F2CC(&self->actor, globalCtx, self->unk_15C);
            }
        }
    }
}

void func_80B395F0(EnWonderTalk* self, GlobalContext* globalCtx) {
    if (self->unk_156 == func_8010BDBC(&globalCtx->msgCtx)) {
        if (func_80106BC8(globalCtx)) {
            if (self->switchFlag >= 0) {
                self->actor.flags &= -2;
                Flags_SetSwitch(globalCtx, self->switchFlag);
            }
            switch (self->unk_150) {
                case 1:
                    func_80106CCC(globalCtx);
                    self->actionFunc = func_80B391CC;
                    break;
                case 2:
                    switch (globalCtx->msgCtx.choiceIndex) {
                        case 0:
                            if (!LINK_IS_ADULT) {
                                // "I'm still a child!"
                                osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ まだコドモなの！ ☆☆☆☆☆ \n" VT_RST);
                                self->actor.textId = 0x5001;
                            } else {
                                // "I'm an adult. .. .."
                                osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ アダルトなの。。。 ☆☆☆☆☆ \n" VT_RST);
                                self->actor.textId = 0x5003;
                            }
                            break;
                        case 1:
                            // "Out!"
                            osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆☆☆☆ はずれ！ ☆☆☆☆☆ \n" VT_RST);
                            self->actor.textId = 0x5004;
                            break;
                    }

                    self->unk_156 = 6;
                    func_8010B720(globalCtx, self->actor.textId);
                    self->actionFunc = func_80B391CC;
                    break;
                case 3:
                    func_80106CCC(globalCtx);
                    if (self->unk_164 == 0) {
                        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_POH, self->actor.world.pos.x,
                                    self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 2);
                        self->unk_164 = 1;
                    }

                    self->actionFunc = func_80B391CC;
                    break;
                case 5:
                    func_80106CCC(globalCtx);
                    if (self->unk_164 == 0) {
                        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_POH, self->actor.world.pos.x,
                                    self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 3);
                        self->unk_164 = 1;
                    }
                    self->actionFunc = func_80B391CC;
                    break;
            }
        }
    }
}

void EnWonderTalk_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnWonderTalk* self = THIS;

    if (self->unk_158 != 0) {
        self->unk_158--;
    }
    self->actionFunc(self, globalCtx);
    Actor_SetFocus(&self->actor, self->height);

    if (BREG(0) != 0) {
        if (self->unk_15A != 0) {
            if ((self->unk_15A & 1) == 0) {
                DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                       self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 10, 10, 10, 255, 4, globalCtx->state.gfxCtx);
            }
        } else {
            DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                   self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                   1.0f, 1.0f, 0, 255, 0, 255, 4, globalCtx->state.gfxCtx);
        }
    }
}
