#include "z_en_mb.h"
#include <alloca.h>
#define NON_MATCHING
#define FLAGS 0x00000015

#define THIS ((EnMb*)thisx)

void EnMb_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMb_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMb_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Mb_InitVars = {
    ACTOR_EN_MB,
    ACTORTYPE_ENEMY,
    FLAGS,
    OBJECT_MB,
    sizeof(EnMb),
    (ActorFunc)EnMb_Init,
    (ActorFunc)EnMb_Destroy,
    (ActorFunc)EnMb_Update,
    (ActorFunc)EnMb_Draw,
};

extern SkeletonHeader D_06008F38;
extern AnimationHeader D_060028E0;
extern SkeletonHeader D_06014190;
extern AnimationHeader D_0600EBE4;
extern AnimationHeader D_060041A8;
extern AnimationHeader D_06009FC0;
extern AnimationHeader D_06002C10;
extern AnimationHeader D_06002F10;
extern AnimationHeader D_06009280;
extern AnimationHeader D_06001950;
extern AnimationHeader D_0600BE58;
extern AnimationHeader D_0600D5D4;
extern AnimationHeader D_0600E18C;
extern AnimationHeader D_0600B4BC;
extern AnimationHeader D_0600ABE0;
extern AnimationHeader D_0600095C;
extern AnimationHeader D_060016B4;
extern AnimationHeader D_060041A8;
extern AnimationHeader D_0600C44C;

void printInfo(EnMb* this, GlobalContext* globalCtx);
void EnMb_DrawAnim(EnMb* this);

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 74, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, 64536, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_4C, 5300, ICHAIN_STOP),
};

void EnMb_Init(Actor* thisx, GlobalContext* globalCtx) {

    Actor_ProcessInitChain(&THIS->actor, sInitChain);
    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawFunc_Circle, 46.0f);

    SkelAnime_InitSV(globalCtx, &THIS->skelAnime, &D_06008F38, &D_060028E0, &THIS->limbDrawTable,
                     &THIS->transitionDrawTable, 28);
    Actor_SetScale(thisx, 0.014f);
}

void EnMb_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

Input* input;
s8 index = 0;
s8 pressedUP = 0;
s8 pressedDown = 0;

void EnMb_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnMb* this;
    input = globalCtx->state.input;
    this = THIS;
    printInfo(this, globalCtx);
    if ((CHECK_BTN_ANY(input[0].press.button, BTN_DUP) && pressedUP == 0)) {
        pressedUP = 1;
        if (index < 12)
            index++;
    }
    if (!CHECK_BTN_ANY(input[0].press.button, BTN_DUP)) {
        pressedUP = 0;
    }
    if ((CHECK_BTN_ANY(input[0].press.button, BTN_DDOWN) && pressedDown == 0)) {
        pressedDown = 1;
        if (index > 0)
            index--;
    }
    if (!CHECK_BTN_ANY(input[0].press.button, BTN_DDOWN)) {
        pressedDown = 0;
    }
    EnMb_DrawAnim(this);
}

void EnMb_Draw(Actor* thisx, GlobalContext* globalCtx) {

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawSV(globalCtx, THIS->skelAnime.skeleton, THIS->skelAnime.limbDrawTbl, THIS->skelAnime.dListCount, 0, 0,
                     thisx);
}

char* list[] = {
    "&D_0600E18C", "&D_0600E18C", "&D_06002F10", "&D_06002C10", "&D_06002C10",
    "&D_0600D5D4", "&D_0600EBE4", "&D_0600B4BC", "&D_06009FC0", "&D_06009FC0",
};

void printInfo(EnMb* this, GlobalContext* globalCtx) {
    GfxPrint* printer;
    GraphicsContext* gfxCtx;
    Player* player = PLAYER;

    gfxCtx = globalCtx->state.gfxCtx;
    printer = alloca(sizeof(GfxPrint));
    GfxPrint_Init(printer);
    GfxPrint_Open(printer, gfxCtx->polyOpa.p);
    GfxPrint_SetColor(printer, 255, 255, 255, 255);
    GfxPrint_SetPos(printer, 0, 8);

    GfxPrint_Printf(printer, "INDEX %d", index);
    GfxPrint_SetPos(printer, 9, 0);
    GfxPrint_Printf(printer, list[index]);

    gfxCtx->polyOpa.p = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);
}
s8 currentPlaying = 0;
void EnMb_DrawAnim(EnMb* this) {
    switch (index) {
        case 0:
            if (currentPlaying != 0) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_0600E18C, 3.0f, 0.0f, SkelAnime_GetFrameCount(&D_0600E18C), 3,
                                     0.0f);
                currentPlaying = 0;
            }
            break;
        case 1:
            if (currentPlaying != 1) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_0600E18C, 3.0f, 0.0f, SkelAnime_GetFrameCount(&D_0600E18C), 3,
                                     0.0f);
                currentPlaying = 1;
            }
            break;
        case 2:
            if (currentPlaying != 2) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_06002F10, 0.5f, 0.0f, SkelAnime_GetFrameCount(&D_06002F10), 1,
                                     0.0f);
                currentPlaying = 2;
            }
            break;
        case 3:
            if (currentPlaying != 3) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_06002C10, -1.0f, SkelAnime_GetFrameCount(&D_06002C10), 0.0f,
                                     2, 0.0f);
                currentPlaying = 3;
            }
            break;
        case 4:
            if (currentPlaying != 4) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_0600D5D4, 1.0f, 4.0f, SkelAnime_GetFrameCount(&D_06002C10), 3,
                                     0.0f);
                currentPlaying = 4;
            }
            break;
        case 5:
            if (currentPlaying != 5) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_0600EBE4, 5.0f, 0.0f, SkelAnime_GetFrameCount(&D_0600EBE4), 3,
                                     0.0f);
                currentPlaying = 5;
            }
            break;
        case 6:
            if (currentPlaying != 6) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_0600B4BC, 3.0f, 0.0f, SkelAnime_GetFrameCount(&D_0600B4BC), 3,
                                     0.0f);
                currentPlaying = 6;
            }
            break;
        case 7:
            if (currentPlaying != 7) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_06009FC0, 0.0f, 0.0f, SkelAnime_GetFrameCount(&D_06009FC0), 1,
                                     -4.0f);
                currentPlaying = 7;
            }
            break;
        case 8:
            if (currentPlaying != 8) {
                SkelAnime_ChangeAnim(&this->skelAnime, &D_06009FC0, 0.0f, 0.0f, SkelAnime_GetFrameCount(&D_06009FC0), 0,
                                     -4.0f);
                currentPlaying = 8;
            }
            break;
        case 9:
            if (currentPlaying != 9) {
                SkelAnime_ChangeAnimTransitionRepeat(&this->skelAnime, &D_060041A8, -4.0f);
                currentPlaying = 9;
            }
            break;
        case 10:
            if (currentPlaying != 10) {
                SkelAnime_ChangeAnimDefaultRepeat(&this->skelAnime, &D_0600EBE4);
                currentPlaying = 10;
            }
            break;
        case 11:
            if (currentPlaying != 11) {
                SkelAnime_ChangeAnimTransitionRepeat(&this->skelAnime, &D_060041A8, -4.0f);
                currentPlaying = 11;
            }
            break;
    }
}

#undef NON_MATCHING