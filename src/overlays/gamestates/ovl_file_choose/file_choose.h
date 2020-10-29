#include "ultra64.h"
#include "global.h"
#include "vt.h"

#ifndef _FILE_CHOOSE_H_
#define _FILE_CHOOSE_H_

#define NEWF_GET_Z(sramCtx, slotNum) (sramCtx->readBuff[gSramSlotOffsets[0][slotNum] + 0x1C])
#define NEWF_GET_E(sramCtx, slotNum) (sramCtx->readBuff[gSramSlotOffsets[0][slotNum] + 0x1D])
#define NEWF_GET_L(sramCtx, slotNum) (sramCtx->readBuff[gSramSlotOffsets[0][slotNum] + 0x1E])
#define NEWF_GET_D(sramCtx, slotNum) (sramCtx->readBuff[gSramSlotOffsets[0][slotNum] + 0x1F])
#define NEWF_GET_A(sramCtx, slotNum) (sramCtx->readBuff[gSramSlotOffsets[0][slotNum] + 0x20])
#define NEWF_GET_Z2(sramCtx, slotNum) (sramCtx->readBuff[gSramSlotOffsets[0][slotNum] + 0x21])

#define SLOT_OCCUPIED(sramCtx, slotNum) \
    ((NEWF_GET_Z(sramCtx, slotNum) == 'Z') || \
     (NEWF_GET_E(sramCtx, slotNum) == 'E') || \
     (NEWF_GET_L(sramCtx, slotNum) == 'L') || \
     (NEWF_GET_D(sramCtx, slotNum) == 'D') || \
     (NEWF_GET_A(sramCtx, slotNum) == 'A') || \
     (NEWF_GET_Z2(sramCtx, slotNum) == 'Z'))

typedef enum {
    /* 0 */ MENU_MODE_INIT,
    /* 1 */ MENU_MODE_CONFIG,
    /* 2 */ MENU_MODE_SELECT
} MenuMode;

typedef enum {
    /* 0 */ TITLE_SELECT_FILE,   // "Please select a file."
    /* 1 */ TITLE_OPEN_FILE,     // "Open this file?"
    /* 2 */ TITLE_COPY_FROM,     // "Copy which file?"
    /* 3 */ TITLE_COPY_TO,       // "Copy to which file?"
    /* 4 */ TITLE_COPY_CONFIRM,  // "Are you sure?"
    /* 5 */ TITLE_COPY_COMPLETE, // "File copied."
    /* 6 */ TITLE_ERASE_FILE,    // "Erase which file?"
    /* 7 */ TITLE_ERASE_CONFIRM, // "Are you sure?"
    /* 8 */ TITLE_ERASE_COMPLETE // "File erased."
} TitleLabel;

typedef enum {
    /* -1 */ WARNING_NONE = -1,
    /*  0 */ WARNING_NO_FILE_COPY,   // "No file to copy."
    /*  1 */ WARNING_NO_FILE_ERASE,  // "No file to erase."
    /*  2 */ WARNING_NO_EMPTY_FILES, // "There is no empty file."
    /*  3 */ WARNING_FILE_EMPTY,     // "This is an empty file."
    /*  4 */ WARNING_FILE_IN_USE     // "This file is in use."
} WarningLabel;

typedef enum {
    /* 0 */ BTN_MAIN_FILE_1,
    /* 1 */ BTN_MAIN_FILE_2,
    /* 2 */ BTN_MAIN_FILE_3,
    /* 3 */ BTN_MAIN_COPY,
    /* 4 */ BTN_MAIN_ERASE,
    /* 5 */ BTN_MAIN_OPTIONS
} MainMenuButtonIndex;

typedef enum {
    /* 0 */ BTN_COPY_FILE_1,
    /* 1 */ BTN_COPY_FILE_2,
    /* 2 */ BTN_COPY_FILE_3,
    /* 3 */ BTN_COPY_QUIT
} CopyMenuButtonIndex;

typedef enum {
    /* 0 */ BTN_ERASE_FILE_1,
    /* 1 */ BTN_ERASE_FILE_2,
    /* 2 */ BTN_ERASE_FILE_3,
    /* 3 */ BTN_ERASE_QUIT
} EraseMenuButtonIndex;

typedef enum {
    /* 0 */ BTN_CONFIRM_YES,
    /* 1 */ BTN_CONFIRM_QUIT
} ConfirmButtonIndex;

typedef enum {
    /* 0 */ SETTING_AUDIO,
    /* 1 */ SETTING_TARGET
} SettingIndex;

typedef enum {
    /* 0 */ AUDIO_STEREO,
    /* 1 */ AUDIO_MONO,
    /* 2 */ AUDIO_HEADSET,
    /* 3 */ AUDIO_SURROUND
} AudioOption;

typedef enum {
    /* 0 */ CHAR_PAGE_HIRA,
    /* 1 */ CHAR_PAGE_KATA,
    /* 2 */ CHAR_PAGE_ENG,
} CharPage;

typedef enum {
    /* 00 */ CM_FADE_IN_START,
    /* 01 */ CM_FADE_IN_END,
    /* 02 */ CM_MAIN_MENU,
    /* 03 */ CM_03,
    /* 04 */ CM_04,
    /* 05 */ CM_05,
    /* 06 */ CM_06,
    /* 07 */ CM_07,
    /* 08 */ CM_08,
    /* 09 */ CM_09,
    /* 10 */ CM_10,
    /* 11 */ CM_11,
    /* 12 */ CM_12,
    /* 13 */ CM_13,
    /* 14 */ CM_14,
    /* 15 */ CM_15,
    /* 16 */ CM_16,
    /* 17 */ CM_17,
    /* 18 */ CM_18,
    /* 19 */ CM_19,
    /* 20 */ CM_20,
    /* 21 */ CM_21,
    /* 22 */ CM_22,
    /* 23 */ CM_23,
    /* 24 */ CM_24,
    /* 25 */ CM_25,
    /* 26 */ CM_26,
    /* 27 */ CM_27,
    /* 28 */ CM_28,
    /* 29 */ CM_29,
    /* 30 */ CM_30,
    /* 31 */ CM_31,
    /* 32 */ CM_ROT_TO_NAME_ENTRY,
    /* 33 */ CM_KEYBOARD_CURSOR,
    /* 34 */ CM_START_NAME_ENTRY,
    /* 35 */ CM_35,
    /* 36 */ CM_ROT_TO_OPTIONS,
    /* 37 */ CM_OPTIONS_MENU,
    /* 38 */ CM_START_OPTIONS,
    /* 39 */ CM_ROT_FROM_OPTIONS,
    /* 40 */ CM_40
} ConfigMode;

// update funcs for menuMode 1 (gConfigModeUpdateFuncs)
void FileChoose_StartFadeIn(FileChooseContext* this); // 00
void FileChoose_FinishFadeIn(FileChooseContext* this); // 01
void FileChoose_UpdateMainMenu(FileChooseContext* this); // 02
void func_80803D40(FileChooseContext* this); // 03
void func_80803ED8(FileChooseContext* this); // 04
void func_80804248(FileChooseContext* this); // 05
void func_808043D8(FileChooseContext* this); // 06
void func_808044A0(FileChooseContext* this); // 07
void func_80804858(FileChooseContext* this); // 08
void func_80804924(FileChooseContext* this); // 09
void func_80804A50(FileChooseContext* this); // 10
void func_80804C74(FileChooseContext* this); // 11
void func_80804CD0(FileChooseContext* this); // 12
void func_80804ED8(FileChooseContext* this); // 13
void func_8080510C(FileChooseContext* this); // 14
void func_808051C8(FileChooseContext* this); // 15
void func_80805318(FileChooseContext* this); // 16
void func_80805434(FileChooseContext* this); // 17
void func_80805524(FileChooseContext* this); // 18
void func_80805824(FileChooseContext* this); // 19
void func_8080595C(FileChooseContext* this); // 20
void func_80805B2C(FileChooseContext* this); // 21
void func_80805EB8(FileChooseContext* this); // 22
void func_80806180(FileChooseContext* this); // 23
void func_8080625C(FileChooseContext* this); // 24
void func_80806444(FileChooseContext* this); // 25
void func_808064F4(FileChooseContext* this); // 26
void func_80806710(FileChooseContext* this); // 27
void func_808068F0(FileChooseContext* this); // 28
void func_808069B4(FileChooseContext* this); // 29
void func_80806C20(FileChooseContext* this); // 30
void func_8080BE28(FileChooseContext* this); // 31
void FileChoose_RotateToNameEntry(FileChooseContext* this); // 32
void FileChoose_UpdateKeyboardCursor(FileChooseContext* this); // 33
void FileChoose_StartNameEntry(FileChooseContext* this); // 34
void FileChoose_RotateFromOptions(FileChooseContext* this); // 35
void FileChoose_RotateToOptions(FileChooseContext* this); // 36
void FileChoose_UpdateOptionsMenu(FileChooseContext* this); // 37
void FileChoose_StartOptions(FileChooseContext* this); // 38
void FileChoose_RotateFromOptions(FileChooseContext* this); // 39
void func_8080BE30(FileChooseContext* this); // 40

// update funcs for menuMode 2 (gSelectModeUpdateFuncs)
void func_8080FE2C(FileChooseContext* this);
void func_8080FF98(FileChooseContext* this);
void func_8081009C(FileChooseContext* this);
void func_8081017C(FileChooseContext* this);
void func_80810354(FileChooseContext* this);
void func_80810440(FileChooseContext* this);
void func_80810698(FileChooseContext* this);
void func_808106F4(FileChooseContext* this);

// draw funcs for each menu mode (gFileSelectDrawFuncs)
void FileChoose_InitModeDraw(GameState* thisx);
void FileChoose_ConfigModeDraw(GameState* thisx);
void FileChoose_SelectModeDraw(GameState* thisx);

// update funcs for each menu mode (gFileSelectUpdateFuncs)
void FileChoose_InitModeUpdate(FileChooseContext* this);
void FileChoose_ConfigModeUpdate(FileChooseContext* this);
void FileChoose_SelectModeUpdate(FileChooseContext* this);

#endif
