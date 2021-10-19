/*
 * File: z_en_wonder_talk2.c
 * Overlay: ovl_En_Wonder_Talk2
 * Description: Dialog spot
 */

#include "z_en_wonder_talk2.h"
#include "vt.h"

#define FLAGS 0x08000009

#define THIS ((EnWonderTalk2*)thisx)

void EnWonderTalk2_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWonderTalk2_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWonderTalk2_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80B3A10C(EnWonderTalk2* self, GlobalContext* globalCtx);
void func_80B3A4F8(EnWonderTalk2* self, GlobalContext* globalCtx);
void func_80B3A15C(EnWonderTalk2* self, GlobalContext* globalCtx);
void func_80B3A3D4(EnWonderTalk2* self, GlobalContext* globalCtx);
void EnWonderTalk2_DoNothing(EnWonderTalk2* self, GlobalContext* globalCtx);

const ActorInit En_Wonder_Talk2_InitVars = {
    ACTOR_EN_WONDER_TALK2,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWonderTalk2),
    (ActorFunc)EnWonderTalk2_Init,
    (ActorFunc)EnWonderTalk2_Destroy,
    (ActorFunc)EnWonderTalk2_Update,
    NULL,
};

static s16 D_80B3A8E0[] = { 6, 0, 1, 2, 3, 4, 5 };

void EnWonderTalk2_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnWonderTalk2_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnWonderTalk2* self = THIS;

    osSyncPrintf("\n\n");
    // "Transparent message"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 透明メッセージ君 ☆☆☆☆☆ %x\n" VT_RST, self->actor.params);
    self->baseMsgId = (self->actor.params >> 6) & 0xFF;
    if (self->actor.world.rot.z > 0) {
        s32 rangeIndex = 0;
        s16 rotZmod10 = self->actor.world.rot.z;

        while (rotZmod10 > 10) {
            rotZmod10 -= 10;
            rangeIndex++;
        }
        // rangeIndex = rot.z/10 here
        self->triggerRange = rotZmod10 * 40.0f;
        if (rangeIndex > 6) {
            rangeIndex = 0;
        }

        self->actor.targetMode = D_80B3A8E0[rangeIndex];

        osSyncPrintf("\n\n");
        // "originally?"
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 元は？       ☆☆☆☆☆ %d\n" VT_RST, self->actor.world.rot.z);
        // "The range is?"
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ レンジは？   ☆☆☆☆☆ %d\n" VT_RST, self->actor.targetMode);
        // "Is the range?"
        osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ は、範囲わ？ ☆☆☆☆☆ %f\n" VT_RST, self->triggerRange);
        osSyncPrintf("\n\n");
        osSyncPrintf("\n\n");
        osSyncPrintf("\n\n");
    }

    self->initPos = self->actor.world.pos;
    self->switchFlag = (self->actor.params & 0x3F);
    self->talkMode = ((self->actor.params >> 0xE) & 3);

    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    if (self->switchFlag >= 0 && Flags_GetSwitch(globalCtx, self->switchFlag)) {
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ Ｙｏｕ ａｒｅ Ｓｈｏｃｋ！  ☆☆☆☆☆ %d\n" VT_RST, self->switchFlag);
        Actor_Kill(&self->actor);
        return;
    }
    if ((self->talkMode == 1) && (globalCtx->sceneNum == SCENE_MEN) && (self->switchFlag != 0x08) &&
        (self->switchFlag != 0x16) && (self->switchFlag != 0x2F)) {

        self->unk_15A = false;
        self->talkMode = 4;
    }
    if (self->talkMode == 3) {
        self->actor.flags &= ~0x08000000;
        self->actionFunc = EnWonderTalk2_DoNothing;
    } else {
        self->actionFunc = func_80B3A10C;
    }
}

void func_80B3A10C(EnWonderTalk2* self, GlobalContext* globalCtx) {
    self->actor.textId = 0x200;
    self->actor.textId |= self->baseMsgId;
    if (self->talkMode == 1 || self->talkMode == 4) {
        self->actionFunc = func_80B3A4F8;
    } else {
        self->actionFunc = func_80B3A15C;
    }
}

void func_80B3A15C(EnWonderTalk2* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_158++;
    if ((self->switchFlag >= 0) && Flags_GetSwitch(globalCtx, self->switchFlag)) {
        if (!self->unk_15A) {
            self->actor.flags &= ~1;
            self->unk_15A = true;
        }
    } else if (func_8002F194(&self->actor, globalCtx)) {
        if ((self->switchFlag >= 0) && (self->talkMode != 2)) {
            Flags_SetSwitch(globalCtx, self->switchFlag);
            // "I saved it! All of it!"
            osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ セーブしたよ！おもいっきり！ %x\n" VT_RST, self->switchFlag);
        }

        self->actionFunc = func_80B3A10C;
    } else {
        s16 yawDiff = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.world.rot.y));

        if (!((self->actor.xzDistToPlayer > 40.0f + self->triggerRange) ||
              (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) > 100.0f) || (yawDiff >= 0x4000))) {
            if (self->unk_158 >= 2) {
                osSyncPrintf("\n\n");
                // "Transparent Message Kimi Set"
                osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 透明メッセージ君せっと %x\n" VT_RST, self->actor.params);
                // "Save Information"
                osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ セーブ情報 \t           %x\n" VT_RST, self->switchFlag);
                // "Specified message type"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 指定メッセージ種類     %x\n" VT_RST, self->baseMsgId);
                // "Actual message type"
                osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 実質メッセージ種類     %x\n" VT_RST, self->actor.textId);
                // "Specified range"
                osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 指定範囲               %d\n" VT_RST, self->actor.world.rot.z);
                // "Processing range"
                osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 処理範囲               %f\n" VT_RST, self->triggerRange);
                switch (self->talkMode) {
                    case 0:
                        // "Normal"
                        osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆ 通常 ☆☆ \n" VT_RST);
                        break;
                    case 2:
                        // "Check only"
                        osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆ チェックのみ ☆☆ \n" VT_RST);
                        break;
                    case 3:
                        // "Lock only"
                        osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆ ロックのみ ☆☆ \n" VT_RST);
                        break;
                }
            }

            self->unk_158 = 0;
            func_8002F1C4(&self->actor, globalCtx, self->triggerRange + 50.0f, 100.0f, EXCH_ITEM_NONE);
        }
    }
}

void func_80B3A3D4(EnWonderTalk2* self, GlobalContext* globalCtx) {
    if (BREG(2) != 0) {
        // "Oh"
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ わー %d\n" VT_RST, func_8010BDBC(&globalCtx->msgCtx));
    }

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 5:
        case 6:
            if (func_80106BC8(globalCtx)) {
                if (func_8010BDBC(&globalCtx->msgCtx) == 5) {
                    func_80106CCC(globalCtx);
                }
            } else {
                break;
            }
        case 0:
            if ((self->switchFlag >= 0) && (self->talkMode != 4)) {
                Flags_SetSwitch(globalCtx, self->switchFlag);
                // "(Forced) I saved it! All of it!"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ (強制)セーブしたよ！おもいっきり！ %x\n" VT_RST, self->switchFlag);
            }

            if (self->talkMode == 4) {
                self->unk_15A = true;
            }
            self->actor.flags &= ~0x11;
            func_8002DF54(globalCtx, NULL, 7);
            self->unk_156 = true;
            self->actionFunc = func_80B3A4F8;
            break;
    }
}

void func_80B3A4F8(EnWonderTalk2* self, GlobalContext* globalCtx) {
    Player* player;

    player = GET_PLAYER(globalCtx);
    self->unk_158++;
    if (self->switchFlag >= 0 && Flags_GetSwitch(globalCtx, self->switchFlag)) {
        if (!self->unk_15A) {
            self->actor.flags &= ~1;
            self->unk_15A = true;
        }
    } else if ((self->talkMode != 4) || !self->unk_15A) {
        if (BREG(2) != 0) {
            // "distance"
            osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ きょり %f\n" VT_RST, self->actor.xzDistToPlayer);
        }
        if (((self->actor.xzDistToPlayer < (40.0f + self->triggerRange)) &&
             (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < 100.0f)) &&
            !Gameplay_InCsMode(globalCtx)) {
            if (self->unk_158 >= 2) {
                osSyncPrintf("\n\n");
                // "Transparent Message Kimi Seto"
                osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 透明メッセージ君せっと %x\n" VT_RST, self->actor.params);
                // "Save Information"
                osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ セーブ情報 \t           %x\n" VT_RST, self->switchFlag);
                // "Specified message type"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 指定メッセージ種類     %x\n" VT_RST, self->baseMsgId);
                // "Real message type"
                osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 実質メッセージ種類     %x\n" VT_RST, self->actor.textId);
                // "Specified range"
                osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 指定範囲               %d\n" VT_RST, self->actor.world.rot.z);
                // "Processing range"
                osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 処理範囲               %f\n" VT_RST, self->triggerRange);
                // "What is your range?"
                osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ レンジは？ \t\t   %d\n" VT_RST, self->actor.targetMode);
                osSyncPrintf("\n\n");
                osSyncPrintf("\n\n");
                switch (self->talkMode) {
                    case 1:
                        // "Compulsion"
                        osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆ 強制 ☆☆ \n" VT_RST);
                        break;
                    case 4:
                        // "Gerudo Training Grounds Forced Check Only"
                        osSyncPrintf(VT_FGCOL(RED) " ☆☆ ゲルドの修練場強制チェックのみ ☆☆ \n" VT_RST);
                        break;
                }

                osSyncPrintf("\n\n");
            }
            self->unk_158 = 0;
            if (!self->unk_156) {
                func_8010B680(globalCtx, self->actor.textId, NULL);
                func_8002DF54(globalCtx, NULL, 8);
                self->actor.flags |= 0x11;
                self->actionFunc = func_80B3A3D4;
            }

        } else {
            self->unk_156 = false;
        }
    }
}

void EnWonderTalk2_DoNothing(EnWonderTalk2* self, GlobalContext* globalCtx) {
}

void EnWonderTalk2_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnWonderTalk2* self = THIS;

    self->actionFunc(self, globalCtx);
    self->actor.world.pos.y = self->initPos.y;

    Actor_SetFocus(&self->actor, self->height);

    if (BREG(0) != 0) {
        if (self->unk_158 != 0) {
            if ((self->unk_158 & 1) == 0) {
                DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                       self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 70, 70, 70, 255, 4, globalCtx->state.gfxCtx);
            }
        } else {
            DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                   self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                   1.0f, 1.0f, 0, 0, 255, 255, 4, globalCtx->state.gfxCtx);
        }
    }
}
