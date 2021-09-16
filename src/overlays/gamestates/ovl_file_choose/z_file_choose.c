#include "ultra64.h"
#include "global.h"

GfxPrint gPrinter;

#define THIS ((FileChooseContext*)thisx)
#define INPUT this->state.input[0].press
typedef enum {
    FILE_CHOOSE_MAIN,
    FILE_CHOOSE_COPY,
    FILE_CHOOSE_ERASE,
    FILE_CHOOSE_OPTIONS,
    FILE_CHOOSE_PAGE_MAX,
} Page;

typedef enum {
    FILE_CHOOSE_OPTION_AUDIO,
    FILE_CHOOSE_OPTION_TARGER,
} Option;

typedef enum {
    FILE_CHOOSE_AUDIO_STEREO,
    FILE_CHOOSE_AUDIO_MONO,
    FILE_CHOOSE_AUDIO_SURROUND,
    FILE_CHOOSE_AUDIO_HEADPHONE
} AudioOption;

void FileChoose_Destroy(GameState* thisx) {
    GfxPrint_Destroy(&gPrinter);
}

void FileChoose_UpdateMenu(FileChooseContext* this) {
    switch (this->pageIndex) {
        case FILE_CHOOSE_MAIN:
            if ((s8)INPUT.stick_y < -0x30) { /*Pressed down*/
                if (this->cursorPos[FILE_CHOOSE_MAIN] == 0) {
                    this->cursorPos[FILE_CHOOSE_MAIN] = 6; /* Options button */
                } else {
                    this->cursorPos[FILE_CHOOSE_MAIN]--;
                }
            } else if ((s8)INPUT.stick_y > 0x30) {
                if (this->cursorPos[FILE_CHOOSE_MAIN] == 6) {
                    this->cursorPos[FILE_CHOOSE_MAIN] = 0; /* File 1 */
                } else {
                    this->cursorPos[FILE_CHOOSE_MAIN]++;
                }
            }
    }
    osSyncPrintf("this->cursorPos%d", this->cursorPos[FILE_CHOOSE_MAIN]);
}

__inline void FileChoose_DrawMenu(FileChooseContext* this) {
    u8 i;
    u8 j;
    OPEN_DISPS(this->state.gfxCtx, __FILE__, __LINE__);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(this->state.gfxCtx, 0, 0, 0);
    SET_FULLSCREEN_VIEWPORT(&this->view);
    func_800AAA50(&this->view, 0xF);
    func_80094140(this->state.gfxCtx);
    GfxPrint_Open(&gPrinter, OVERLAY_DISP);
    GfxPrint_SetColor(&gPrinter, 255, 255, 255, 255);
    GfxPrint_SetPos(&gPrinter, 7, 2);
    // GfxPrint_Printf(&gPrinter, "Ctx:%X\n", this);
    // GfxPrint_Printf(&gPrinter, "OFFSET OF FILENAMES:%X\n", this->fileNames);
    switch (this->pageIndex) {
        case FILE_CHOOSE_MAIN:
            GfxPrint_Printf(&gPrinter, "Choose a file to load:");
            for (i = 0; i < 3; i++) {
                GfxPrint_SetPos(&gPrinter, 4, i + 5);
                GfxPrint_Printf(&gPrinter, "File %d   ", i);
                // if (this->cursorPos[FILE_CHOOSE_MAIN] == i) {
                //    GfxPrint_SetColor(&gPrinter, 0, 0, 255, 255);
                //} else {
                //    GfxPrint_SetColor(&gPrinter, 255, 255, 255, 255);
                //}
                for (j = 0; j < 8; j++) {
                    if (this->fileNames[i][j] <= 0xF) {
                        GfxPrint_Printf(&gPrinter, "%X ", (this->fileNames[i][j]));
                    } else if (this->fileNames[i][j] > 0xF && this->fileNames[i][j] <= 25) {
                        GfxPrint_Printf(&gPrinter, "%c ", (this->fileNames[i][j] + 0x37));
                    } else if (this->fileNames[i][j] > 26 && this->fileNames[i][j] < 62) {
                        GfxPrint_Printf(&gPrinter, "%c ", (this->fileNames[i][j] + 0x3D));
                    } else {
                        GfxPrint_Printf(&gPrinter, " ");
                    }
                }
                // GfxPrint_PrintChar(&gPrinter, '\n');
            }

            GfxPrint_SetPos(&gPrinter, 4, 10);
            GfxPrint_Printf(&gPrinter, "Copy");
            GfxPrint_SetPos(&gPrinter, 4, 11);
            GfxPrint_Printf(&gPrinter, "Delete");
            GfxPrint_SetPos(&gPrinter, 4, 13);
            GfxPrint_Printf(&gPrinter, "Options");
            break;
        case FILE_CHOOSE_COPY:
            break;
        case FILE_CHOOSE_ERASE:
            break;
        case FILE_CHOOSE_OPTIONS:
            GfxPrint_Printf(&gPrinter, "Audio setting:");
            GfxPrint_SetPos(&gPrinter, 5, 5);
            GfxPrint_Printf(&gPrinter, "Stereo");
            GfxPrint_Printf(&gPrinter, "\tMono");
            GfxPrint_Printf(&gPrinter, "\tSurround");
            GfxPrint_Printf(&gPrinter, "\tHeadphones");
            break;
    }
    OVERLAY_DISP = GfxPrint_Close(&gPrinter);

    CLOSE_DISPS(this->state.gfxCtx, __FILE__, __LINE__);
}

void FileChoose_Main(GameState* thisx) {
    FileChooseContext* this = THIS;

    FileChoose_UpdateMenu(this);
    FileChoose_DrawMenu(this);
}

void FileChoose_Init(GameState* thisx) {
    FileChooseContext* this = (FileChooseContext*)thisx;
    
    this->state.destroy = FileChoose_Destroy;
    this->state.main = FileChoose_Main;
    this->pageIndex = FILE_CHOOSE_MAIN;
    this->cursorPos[FILE_CHOOSE_MAIN] = 0;
    this->cursorPos[FILE_CHOOSE_COPY] = 0;
    this->cursorPos[FILE_CHOOSE_ERASE] = 0;
    this->cursorPos[FILE_CHOOSE_OPTIONS] = 0;
    View_Init(&this->view, this->state.gfxCtx);
    Sram_Alloc(&this->state, &this->sramCtx);
    Sram_VerifyAndLoadAllSaves(this, &this->sramCtx);


    GfxPrint_Init(&gPrinter);

    R_UPDATE_RATE = 1;
}
