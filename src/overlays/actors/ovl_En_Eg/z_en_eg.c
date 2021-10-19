/*
 * File: z_en_eg.c
 * Overlay: ovl_En_Eg
 * Description: Triggers a void out (used in the tower collapse sequence?)
 */

#include "z_en_eg.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((EnEg*)thisx)

void EnEg_Init(Actor* thisx, GlobalContext* globalCtx);
void EnEg_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnEg_Update(Actor* thisx, GlobalContext* globalCtx);
void EnEg_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809FFDC8(EnEg* self, GlobalContext* globalCtx);

static s32 voided = false;

static EnEgActionFunc sActionFuncs[] = {
    func_809FFDC8,
};

const ActorInit En_Eg_InitVars = {
    ACTOR_EN_EG,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_ZL2,
    sizeof(EnEg),
    (ActorFunc)EnEg_Init,
    (ActorFunc)EnEg_Destroy,
    (ActorFunc)EnEg_Update,
    (ActorFunc)EnEg_Draw,
};

void EnEg_PlayVoidOutSFX() {
    func_800788CC(NA_SE_OC_ABYSS);
}

void EnEg_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnEg_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnEg* self = THIS;

    self->action = 0;
}

void func_809FFDC8(EnEg* self, GlobalContext* globalCtx) {
    if (!voided && (gSaveContext.timer2Value < 1) && Flags_GetSwitch(globalCtx, 0x36) && (kREG(0) == 0)) {
        // Void the player out
        Gameplay_TriggerRespawn(globalCtx);
        gSaveContext.respawnFlag = -2;
        Audio_QueueSeqCmd(NA_BGM_STOP);
        globalCtx->fadeTransition = 2;
        EnEg_PlayVoidOutSFX();
        voided = true;
    }
}

void EnEg_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnEg* self = THIS;
    s32 action = self->action;

    if (((action < 0) || (0 < action)) || (sActionFuncs[action] == NULL)) {
        // "Main Mode is wrong!!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sActionFuncs[action](self, globalCtx);
    }
}

void EnEg_Draw(Actor* thisx, GlobalContext* globalCtx) {
}
