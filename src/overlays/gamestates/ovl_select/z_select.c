/*
 * File: z_select.c
 * Overlay: ovl_select
 * Description: Debug Scene Select Menu
 */

#include "ultra64.h"
#include "global.h"
#include "vt.h"
#include "alloca.h"

void Select_LoadTitle(SelectContext* self) {
    self->state.running = false;
    SET_NEXT_GAMESTATE(&self->state, Title_Init, TitleContext);
}

void Select_LoadGame(SelectContext* self, s32 entranceIndex) {
    osSyncPrintf(VT_FGCOL(BLUE));
    osSyncPrintf("\n\n\nＦＩＬＥ＿ＮＯ＝%x\n\n\n", gSaveContext.fileNum);
    osSyncPrintf(VT_RST);
    if (gSaveContext.fileNum == 0xFF) {
        Sram_InitDebugSave();
        gSaveContext.unk_13F6 = gSaveContext.magic;
        gSaveContext.magic = 0;
        gSaveContext.unk_13F4 = 0;
        gSaveContext.magicLevel = gSaveContext.magic;
    }
    gSaveContext.buttonStatus[4] = BTN_ENABLED;
    gSaveContext.buttonStatus[3] = BTN_ENABLED;
    gSaveContext.buttonStatus[2] = BTN_ENABLED;
    gSaveContext.buttonStatus[1] = BTN_ENABLED;
    gSaveContext.buttonStatus[0] = BTN_ENABLED;
    gSaveContext.unk_13E7 = gSaveContext.unk_13E8 = gSaveContext.unk_13EA = gSaveContext.unk_13EC = 0;
    Audio_QueueSeqCmd(NA_BGM_STOP);
    gSaveContext.entranceIndex = entranceIndex;
    gSaveContext.respawnFlag = 0;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entranceIndex = -1;
    gSaveContext.seqIndex = 0xFF;
    gSaveContext.nightSeqIndex = 0xFF;
    gSaveContext.showTitleCard = true;
    gWeatherMode = 0;
    self->state.running = false;
    SET_NEXT_GAMESTATE(&self->state, Gameplay_Init, GlobalContext);
}

static SceneSelectEntry sScenes[] = {
    { " 1:Hyrule Field", Select_LoadGame, 0x00CD },
    { " 2:Kakariko Village", Select_LoadGame, 0x00DB },
    { " 3:Graveyard", Select_LoadGame, 0x00E4 },
    { " 4:Zora's River", Select_LoadGame, 0x00EA },
    { " 5:Kokiri Forest", Select_LoadGame, 0x00EE },
    { " 6:Sacred Forest Meadow", Select_LoadGame, 0x00FC },
    { " 7:Lake Hylia", Select_LoadGame, 0x0102 },
    { " 8:Zora's Domain", Select_LoadGame, 0x0108 },
    { " 9:Zora's Fountain", Select_LoadGame, 0x010E },
    { "10:Gerudo Valley", Select_LoadGame, 0x0117 },
    { "11:Lost Woods", Select_LoadGame, 0x011E },
    { "12:Desert Colossus", Select_LoadGame, 0x0123 },
    { "13:Gerudo's Fortress", Select_LoadGame, 0x0129 },
    { "14:Haunted Wasteland", Select_LoadGame, 0x0130 },
    { "15:Hyrule Castle", Select_LoadGame, 0x0138 },
    { "16:Death Mountain Trail", Select_LoadGame, 0x013D },
    { "17:Death Mountain Crater", Select_LoadGame, 0x0147 },
    { "18:Goron City", Select_LoadGame, 0x014D },
    { "19:Lon Lon Ranch", Select_LoadGame, 0x0157 },
    { "20:Temple Of Time", Select_LoadGame, 0x0053 },
    { "21:Chamber of Sages", Select_LoadGame, 0x006B },
    { "22:Shooting Gallery", Select_LoadGame, 0x003B },
    { "23:Castle Courtyard Game", Select_LoadGame, 0x007A },
    { "24:Grave 1", Select_LoadGame, 0x031C },
    { "25:Grave 2", Select_LoadGame, 0x004B },
    { "26:Royal Family's Tomb", Select_LoadGame, 0x002D },
    { "27:Great Fairy's Fountain (Din)", Select_LoadGame, 0x0315 },
    { "28:Great Fairy's Fountain (Farore)", Select_LoadGame, 0x036D },
    { "29:Great Fairy's Fountain (Nayru)", Select_LoadGame, 0x0371 },
    { "30:Ganon's Tower - Collapsing", Select_LoadGame, 0x043F },
    { "31:Castle Courtyard", Select_LoadGame, 0x0400 },
    { "32:Fishing Pond", Select_LoadGame, 0x045F },
    { "33:Bombchu Bowling Alley", Select_LoadGame, 0x0507 },
    { "34:Lon Lon Ranch House", Select_LoadGame, 0x004F },
    { "35:Lon Lon Ranch Silo", Select_LoadGame, 0x05D0 },
    { "36:Guard House", Select_LoadGame, 0x007E },
    { "37:Potion Shop", Select_LoadGame, 0x0072 },
    { "38:Treasure Chest Game", Select_LoadGame, 0x0063 },
    { "39:House Of Skulltula", Select_LoadGame, 0x0550 },
    { "40:Entrance to Market", Select_LoadGame, 0x0033 },
    { "41:Market", Select_LoadGame, 0x00B1 },
    { "42:Back Alley", Select_LoadGame, 0x00AD },
    { "43:Temple of Time Exterior", Select_LoadGame, 0x0171 },
    { "44:Link's House", Select_LoadGame, 0x00BB },
    { "45:Kakariko House 1", Select_LoadGame, 0x02FD },
    { "46:Back Alley House 1", Select_LoadGame, 0x043B },
    { "47:House of the Know-it-All Brothers", Select_LoadGame, 0x00C9 },
    { "48:House of Twins", Select_LoadGame, 0x009C },
    { "49:Mido's House", Select_LoadGame, 0x0433 },
    { "50:Saria's House", Select_LoadGame, 0x0437 },
    { "51:Stable", Select_LoadGame, 0x02F9 },
    { "52:Grave Keeper's Hut", Select_LoadGame, 0x030D },
    { "53:Dog Lady's House", Select_LoadGame, 0x0398 },
    { "54:Impa's House", Select_LoadGame, 0x039C },
    { "55:Lakeside Laboratory", Select_LoadGame, 0x0043 },
    { "56:Running Man's Tent", Select_LoadGame, 0x03A0 },
    { "57:Bazaar", Select_LoadGame, 0x00B7 },
    { "58:Kokiri Shop", Select_LoadGame, 0x00C1 },
    { "59:Goron Shop", Select_LoadGame, 0x037C },
    { "60:Zora Shop", Select_LoadGame, 0x0380 },
    { "61:Closed Shop", Select_LoadGame, 0x0384 },
    { "62:Potion Shop", Select_LoadGame, 0x0388 },
    { "63:Bombchu Shop ", Select_LoadGame, 0x0390 },
    { "64:Happy Mask Shop", Select_LoadGame, 0x0530 },
    { "65:Gerudo Training Ground", Select_LoadGame, 0x0008 },
    { "66:Inside the Deku Tree", Select_LoadGame, 0x0000 },
    { "67:Gohma's Lair", Select_LoadGame, 0x040F },
    { "68:Dodongo's Cavern", Select_LoadGame, 0x0004 },
    { "69:King Dodongo's Lair", Select_LoadGame, 0x040B },
    { "70:Inside Jabu-Jabu's Belly", Select_LoadGame, 0x0028 },
    { "71:Barinade's Lair", Select_LoadGame, 0x0301 },
    { "72:Forest Temple", Select_LoadGame, 0x0169 },
    { "73:Phantom Ganon's Lair", Select_LoadGame, 0x000C },
    { "74:Bottom of the Well", Select_LoadGame, 0x0098 },
    { "75:Shadow Temple", Select_LoadGame, 0x0037 },
    { "76:Bongo Bongo's Lair", Select_LoadGame, 0x0413 },
    { "77:Fire Temple", Select_LoadGame, 0x0165 },
    { "78:Volvagia's Lair", Select_LoadGame, 0x0305 },
    { "79:Water Temple", Select_LoadGame, 0x0010 },
    { "80:Morpha's Lair", Select_LoadGame, 0x0417 },
    { "81:Spirit Temple", Select_LoadGame, 0x0082 },
    { "82:Iron Knuckle's Lair", Select_LoadGame, 0x008D },
    { "83:Twinrova's Lair", Select_LoadGame, 0x05EC },
    { "84:Stairs to Ganondorf's Lair", Select_LoadGame, 0x041B },
    { "85:Ganondorf's Lair", Select_LoadGame, 0x041F },
    { "86:Ice Cavern", Select_LoadGame, 0x0088 },
    { "87:Dampé Grave Relay Game", Select_LoadGame, 0x044F },
    { "88:Inside Ganon's Castle", Select_LoadGame, 0x0467 },
    { "89:Ganon's Lair", Select_LoadGame, 0x0517 },
    { "90:Escaping Ganon's Castle 1", Select_LoadGame, 0x0179 },
    { "91:Escaping Ganon's Castle 2", Select_LoadGame, 0x01B5 },
    { "92:Escaping Ganon's Castle 3", Select_LoadGame, 0x03DC },
    { "93:Escaping Ganon's Castle 4", Select_LoadGame, 0x03E4 },
    { "94:Escaping Ganon's Castle 5", Select_LoadGame, 0x056C },
    { "95:Thieves' Hideout 1-2", Select_LoadGame, 0x0486 },
    { "96:Thieves' Hideout 3-4 9-10", Select_LoadGame, 0x048E },
    { "97:Thieves' Hideout 5-6", Select_LoadGame, 0x0496 },
    { "98:Thieves' Hideout 7-8", Select_LoadGame, 0x049E },
    { "99:Thieves' Hideout 11-12", Select_LoadGame, 0x04AE },
    { "100:Thieves' Hideout 13", Select_LoadGame, 0x0570 },
    { "101:Grotto 0", Select_LoadGame, 0x003F },
    { "102:Grotto 1", Select_LoadGame, 0x0598 },
    { "103:Grotto 2", Select_LoadGame, 0x059C },
    { "104:Grotto 3", Select_LoadGame, 0x05A0 },
    { "105:Grotto 4", Select_LoadGame, 0x05A4 },
    { "106:Grotto 5", Select_LoadGame, 0x05A8 },
    { "107:Grotto 6", Select_LoadGame, 0x05AC },
    { "108:Grotto 7", Select_LoadGame, 0x05B0 },
    { "109:Grotto 8", Select_LoadGame, 0x05B4 },
    { "110:Grotto 9", Select_LoadGame, 0x05B8 },
    { "111:Grotto 10", Select_LoadGame, 0x05BC },
    { "112:Grotto 11", Select_LoadGame, 0x05C0 },
    { "113:Grotto 12", Select_LoadGame, 0x05C4 },
    { "114:Grotto 13", Select_LoadGame, 0x05FC },
    { "115:Goddess Cutscene Environment", Select_LoadGame, 0x00A0 },
    { "116:Test Room", Select_LoadGame, 0x0520 },
    { "117:SRD Map", Select_LoadGame, 0x0018 },
    { "118:Test Map", Select_LoadGame, 0x0094 },
    { "119:Treasure Chest Warp", Select_LoadGame, 0x0024 },
    { "120:Stalfos Miniboss Room", Select_LoadGame, 0x001C },
    { "121:Stalfos Boss Room", Select_LoadGame, 0x0020 },
    { "122:Dark Link Room", Select_LoadGame, 0x0047 },
    { "123:Shooting Gallery Duplicate", Select_LoadGame, 0x02EA },
    { "124:depth test", Select_LoadGame, 0x00B6 },
    { "125:Hyrule Garden Game (Broken)", Select_LoadGame, 0x0076 },
    { "title", Select_LoadTitle, 0x0000 },
};

void Select_UpdateMenu(SelectContext* self) {
    Input* controller1;
    s32 pad;
    SceneSelectEntry* selectedScene;

    controller1 = &self->state.input[0];

    if (self->unk_21C == 0) {

        if (CHECK_BTN_ALL(controller1->press.button, BTN_A) || CHECK_BTN_ALL(controller1->press.button, BTN_START)) {
            selectedScene = &self->scenes[self->currentScene];
            if (selectedScene->loadFunc != NULL) {
                selectedScene->loadFunc(self, selectedScene->entranceIndex);
            }
        }

        if (CHECK_BTN_ALL(controller1->press.button, BTN_B)) {
            if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
                gSaveContext.linkAge = 1;
            } else {
                gSaveContext.linkAge = 0;
            }
        }

        if (CHECK_BTN_ALL(controller1->press.button, BTN_Z)) {
            if (gSaveContext.cutsceneIndex == 0x8000) {
                gSaveContext.cutsceneIndex = 0;
            } else if (gSaveContext.cutsceneIndex == 0) {
                gSaveContext.cutsceneIndex = 0xFFF0;
            } else if (gSaveContext.cutsceneIndex == 0xFFF0) {
                gSaveContext.cutsceneIndex = 0xFFF1;
            } else if (gSaveContext.cutsceneIndex == 0xFFF1) {
                gSaveContext.cutsceneIndex = 0xFFF2;
            } else if (gSaveContext.cutsceneIndex == 0xFFF2) {
                gSaveContext.cutsceneIndex = 0xFFF3;
            } else if (gSaveContext.cutsceneIndex == 0xFFF3) {
                gSaveContext.cutsceneIndex = 0xFFF4;
            } else if (gSaveContext.cutsceneIndex == 0xFFF4) {
                gSaveContext.cutsceneIndex = 0xFFF5;
            } else if (gSaveContext.cutsceneIndex == 0xFFF5) {
                gSaveContext.cutsceneIndex = 0xFFF6;
            } else if (gSaveContext.cutsceneIndex == 0xFFF6) {
                gSaveContext.cutsceneIndex = 0xFFF7;
            } else if (gSaveContext.cutsceneIndex == 0xFFF7) {
                gSaveContext.cutsceneIndex = 0xFFF8;
            } else if (gSaveContext.cutsceneIndex == 0xFFF8) {
                gSaveContext.cutsceneIndex = 0xFFF9;
            } else if (gSaveContext.cutsceneIndex == 0xFFF9) {
                gSaveContext.cutsceneIndex = 0xFFFA;
            } else if (gSaveContext.cutsceneIndex == 0xFFFA) {
                gSaveContext.cutsceneIndex = 0x8000;
            }
        } else if (CHECK_BTN_ALL(controller1->press.button, BTN_R)) {
            if (gSaveContext.cutsceneIndex == 0x8000) {
                gSaveContext.cutsceneIndex = 0xFFFA;
            } else if (gSaveContext.cutsceneIndex == 0) {
                gSaveContext.cutsceneIndex = 0x8000;
            } else if (gSaveContext.cutsceneIndex == 0xFFF0) {
                gSaveContext.cutsceneIndex = 0;
            } else if (gSaveContext.cutsceneIndex == 0xFFF1) {
                gSaveContext.cutsceneIndex = 0xFFF0;
            } else if (gSaveContext.cutsceneIndex == 0xFFF2) {
                gSaveContext.cutsceneIndex = 0xFFF1;
            } else if (gSaveContext.cutsceneIndex == 0xFFF3) {
                gSaveContext.cutsceneIndex = 0xFFF2;
            } else if (gSaveContext.cutsceneIndex == 0xFFF4) {
                gSaveContext.cutsceneIndex = 0xFFF3;
            } else if (gSaveContext.cutsceneIndex == 0xFFF5) {
                gSaveContext.cutsceneIndex = 0xFFF4;
            } else if (gSaveContext.cutsceneIndex == 0xFFF6) {
                gSaveContext.cutsceneIndex = 0xFFF5;
            } else if (gSaveContext.cutsceneIndex == 0xFFF7) {
                gSaveContext.cutsceneIndex = 0xFFF6;
            } else if (gSaveContext.cutsceneIndex == 0xFFF8) {
                gSaveContext.cutsceneIndex = 0xFFF7;
            } else if (gSaveContext.cutsceneIndex == 0xFFF9) {
                gSaveContext.cutsceneIndex = 0xFFF8;
            } else if (gSaveContext.cutsceneIndex == 0xFFFA) {
                gSaveContext.cutsceneIndex = 0xFFF9;
            }
        }

        gSaveContext.nightFlag = 0;
        if (gSaveContext.cutsceneIndex == 0) {
            gSaveContext.nightFlag = 1;
        }

        // user can change "opt", but it doesn't do anything
        if (CHECK_BTN_ALL(controller1->press.button, BTN_CUP)) {
            self->opt--;
        }
        if (CHECK_BTN_ALL(controller1->press.button, BTN_CDOWN)) {
            self->opt++;
        }

        if (CHECK_BTN_ALL(controller1->press.button, BTN_DUP)) {
            if (self->unk_22C == 1) {
                self->unk_224 = 0;
            }
            if (self->unk_224 == 0) {
                self->unk_224 = 0x14;
                self->unk_22C = 1;
                Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                self->unk_220 = R_UPDATE_RATE;
            }
        }

        if (CHECK_BTN_ALL(controller1->cur.button, BTN_DUP) && self->unk_224 == 0) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            self->unk_220 = R_UPDATE_RATE * 3;
        }

        if (CHECK_BTN_ALL(controller1->press.button, BTN_DDOWN)) {
            if (self->unk_230 == 1) {
                self->unk_228 = 0;
            }
            if (self->unk_228 == 0) {
                self->unk_228 = 0x14;
                self->unk_230 = 1;
                Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                self->unk_220 = -R_UPDATE_RATE;
            }
        }

        if (CHECK_BTN_ALL(controller1->cur.button, BTN_DDOWN) && (self->unk_228 == 0)) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            self->unk_220 = -R_UPDATE_RATE * 3;
        }

        if (CHECK_BTN_ALL(controller1->press.button, BTN_DLEFT) || CHECK_BTN_ALL(controller1->cur.button, BTN_DLEFT)) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            self->unk_220 = R_UPDATE_RATE;
        }

        if (CHECK_BTN_ALL(controller1->press.button, BTN_DRIGHT) ||
            CHECK_BTN_ALL(controller1->cur.button, BTN_DRIGHT)) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            self->unk_220 = -R_UPDATE_RATE;
        }
    }

    if (CHECK_BTN_ALL(controller1->press.button, BTN_L)) {
        self->unk_1DC++;
        self->unk_1DC = (self->unk_1DC + 7) % 7;
        self->currentScene = self->unk_20C = self->unk_1E0[self->unk_1DC];
    }

    self->unk_21C += self->unk_220;

    if (self->unk_21C < -7) {
        self->unk_220 = 0;
        self->unk_21C = 0;

        self->currentScene++;
        self->currentScene = (self->currentScene + self->count) % self->count;

        if (self->currentScene == ((self->unk_20C + self->count + 0x13) % self->count)) {
            self->unk_20C++;
            self->unk_20C = (self->unk_20C + self->count) % self->count;
        }
    }

    if (self->unk_21C >= 8) {
        self->unk_220 = 0;
        self->unk_21C = 0;

        if (self->currentScene == self->unk_20C) {
            self->unk_20C -= 2;
            self->unk_20C = (self->unk_20C + self->count) % self->count;
        }

        self->currentScene--;
        self->currentScene = (self->currentScene + self->count) % self->count;

        if (self->currentScene == ((self->unk_20C + self->count) % self->count)) {
            self->unk_20C--;
            self->unk_20C = (self->unk_20C + self->count) % self->count;
        }
    }

    self->currentScene = (self->currentScene + self->count) % self->count;
    self->unk_20C = (self->unk_20C + self->count) % self->count;

    dREG(80) = self->currentScene;
    dREG(81) = self->unk_20C;
    dREG(82) = self->unk_1DC;

    if (self->unk_224 != 0) {
        self->unk_224--;
    }

    if (self->unk_224 == 0) {
        self->unk_22C = 0;
    }

    if (self->unk_228 != 0) {
        self->unk_228--;
    }

    if (self->unk_228 == 0) {
        self->unk_230 = 0;
    }
}

void Select_PrintMenu(SelectContext* self, GfxPrint* printer) {
    s32 scene;
    s32 i;
    char* name;

    GfxPrint_SetColor(printer, 255, 155, 150, 255);
    GfxPrint_SetPos(printer, 12, 2);
    GfxPrint_Printf(printer, "ZELDA MAP SELECT");
    GfxPrint_SetColor(printer, 255, 255, 255, 255);

    for (i = 0; i < 20; i++) {
        GfxPrint_SetPos(printer, 5, i + 4);

        scene = ((self->unk_20C + i) + self->count) % self->count;
        if (scene == self->currentScene) {
            GfxPrint_SetColor(printer, 255, 20, 20, 255);
        } else {
            GfxPrint_SetColor(printer, 200, 200, 55, 255);
        }

        name = self->scenes[scene].name;
        if (name == NULL) {
            name = "**Null**";
        }

        GfxPrint_Printf(printer, "%s", name);
    };

    GfxPrint_SetColor(printer, 155, 55, 150, 255);
    GfxPrint_SetPos(printer, 20, 26);
    GfxPrint_Printf(printer, "OPT=%d", self->opt);
}

static char* sLoadingMessages[] = {
    "Please wait a minute",
    "Hold on a sec",
    "Wait a moment",
    "Loading",
    "Now working",
    "Now creating",
    "It's not broken",
    "Coffee Break",
    "Please set B side",
    "Be patient, now",
    "Please wait just a minute",
    "Don't worry, don't worry. Take a break, take a break",
};

void Select_PrintLoadingMessage(SelectContext* self, GfxPrint* printer) {
    s32 randomMsg;

    GfxPrint_SetPos(printer, 0xA, 0xF);
    GfxPrint_SetColor(printer, 255, 255, 255, 255);
    randomMsg = Rand_ZeroOne() * ARRAY_COUNT(sLoadingMessages);
    GfxPrint_Printf(printer, "%s", sLoadingMessages[randomMsg]);
}

static char* sAgeLabels[] = {
    "Adult", // "17(young)"
    "Child", // "5(very young)"
};

void Select_PrintAgeSetting(SelectContext* self, GfxPrint* printer, s32 age) {
    GfxPrint_SetPos(printer, 4, 26);
    GfxPrint_SetColor(printer, 255, 255, 55, 255);
    GfxPrint_Printf(printer, "Age:%s", sAgeLabels[age]);
}

void Select_PrintCutsceneSetting(SelectContext* self, GfxPrint* printer, u16 csIndex) {
    char* label;

    GfxPrint_SetPos(printer, 4, 25);
    GfxPrint_SetColor(printer, 255, 255, 55, 255);

    switch (csIndex) {
        case 0:
            label = "\x8D ﾖﾙ \x8Cｺﾞﾛﾝ";
            gSaveContext.dayTime = 0;
            break;
        case 0x8000:
            // clang-format off
            gSaveContext.dayTime = 0x8000; label = "\x8Dｵﾋﾙ \x8Cｼﾞｬﾗ";
            // clang-format on
            break;
        case 0xFFF0:
            // clang-format off
            gSaveContext.dayTime = 0x8000; label = "ﾃﾞﾓ00";
            // clang-format on
            break;
        case 0xFFF1:
            label = "ﾃﾞﾓ01";
            break;
        case 0xFFF2:
            label = "ﾃﾞﾓ02";
            break;
        case 0xFFF3:
            label = "ﾃﾞﾓ03";
            break;
        case 0xFFF4:
            label = "ﾃﾞﾓ04";
            break;
        case 0xFFF5:
            label = "ﾃﾞﾓ05";
            break;
        case 0xFFF6:
            label = "ﾃﾞﾓ06";
            break;
        case 0xFFF7:
            label = "ﾃﾞﾓ07";
            break;
        case 0xFFF8:
            label = "ﾃﾞﾓ08";
            break;
        case 0xFFF9:
            label = "ﾃﾞﾓ09";
            break;
        case 0xFFFA:
            label = "ﾃﾞﾓ0A";
            break;
    };

    gSaveContext.skyboxTime = gSaveContext.dayTime;
    GfxPrint_Printf(printer, "Stage:\x8C%s", label);
}

void Select_DrawMenu(SelectContext* self) {
    GraphicsContext* gfxCtx = self->state.gfxCtx;
    GfxPrint* printer;

    OPEN_DISPS(gfxCtx, "../z_select.c", 930);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(gfxCtx, 0, 0, 0);
    SET_FULLSCREEN_VIEWPORT(&self->view);
    func_800AAA50(&self->view, 0xF);
    func_80094140(gfxCtx);
    printer = alloca(sizeof(GfxPrint));
    GfxPrint_Init(printer);
    GfxPrint_Open(printer, POLY_OPA_DISP);
    Select_PrintMenu(self, printer);
    Select_PrintAgeSetting(self, printer, ((void)0, gSaveContext.linkAge));
    Select_PrintCutsceneSetting(self, printer, ((void)0, gSaveContext.cutsceneIndex));
    POLY_OPA_DISP = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);

    CLOSE_DISPS(gfxCtx, "../z_select.c", 966);
}

void Select_DrawLoadingScreen(SelectContext* self) {
    GraphicsContext* gfxCtx = self->state.gfxCtx;
    GfxPrint* printer;

    OPEN_DISPS(gfxCtx, "../z_select.c", 977);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(gfxCtx, 0, 0, 0);
    SET_FULLSCREEN_VIEWPORT(&self->view);
    func_800AAA50(&self->view, 0xF);
    func_80094140(gfxCtx);
    printer = alloca(sizeof(GfxPrint));
    GfxPrint_Init(printer);
    GfxPrint_Open(printer, POLY_OPA_DISP);
    Select_PrintLoadingMessage(self, printer);
    POLY_OPA_DISP = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);

    CLOSE_DISPS(gfxCtx, "../z_select.c", 1006);
}

void Select_Draw(SelectContext* self) {
    GraphicsContext* gfxCtx = self->state.gfxCtx;

    OPEN_DISPS(gfxCtx, "../z_select.c", 1013);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(gfxCtx, 0, 0, 0);
    SET_FULLSCREEN_VIEWPORT(&self->view);
    func_800AAA50(&self->view, 0xF);

    if (!self->state.running) {
        Select_DrawLoadingScreen(self);
    } else {
        Select_DrawMenu(self);
    }

    CLOSE_DISPS(gfxCtx, "../z_select.c", 1037);
}

void Select_Main(GameState* thisx) {
    SelectContext* self = (SelectContext*)thisx;

    Select_UpdateMenu(self);
    Select_Draw(self);
}

void Select_Destroy(GameState* thisx) {
    osSyncPrintf("%c", 7);
    // "view_cleanup will hang, so it won't be called"
    osSyncPrintf("*** view_cleanupはハングアップするので、呼ばない ***\n");
}

void Select_Init(GameState* thisx) {
    SelectContext* self = (SelectContext*)thisx;
    u32 size;
    s32 pad;

    self->state.main = Select_Main;
    self->state.destroy = Select_Destroy;
    self->scenes = sScenes;
    self->unk_20C = 0;
    self->currentScene = 0;
    self->unk_1E0[0] = 0;
    self->unk_1E0[1] = 0x13;
    self->unk_1E0[2] = 0x25;
    self->unk_1E0[3] = 0x33;
    self->unk_1E0[4] = 0x3B;
    self->unk_1E0[5] = 0x49;
    self->unk_1E0[6] = 0x5B;
    self->unk_1DC = 0;
    self->opt = 0;
    self->count = 126;
    View_Init(&self->view, self->state.gfxCtx);
    self->view.flags = 0xA;
    self->unk_21C = 0;
    self->unk_220 = 0;
    self->unk_224 = 0;
    self->unk_228 = 0;
    self->unk_22C = 0;
    self->unk_230 = 0;
    self->unk_234 = 0;

    size = (u32)_z_select_staticSegmentRomEnd - (u32)_z_select_staticSegmentRomStart;

    if ((dREG(80) >= 0) && (dREG(80) < self->count)) {
        self->currentScene = dREG(80);
        self->unk_20C = dREG(81);
        self->unk_1DC = dREG(82);
    }
    R_UPDATE_RATE = 1;

    self->staticSegment = GameState_Alloc(&self->state, size, "../z_select.c", 1114);
    DmaMgr_SendRequest1(self->staticSegment, _z_select_staticSegmentRomStart, size, "../z_select.c", 1115);
    gSaveContext.cutsceneIndex = 0x8000;
    gSaveContext.linkAge = 1;
}
