/*
 * File: z_bg_toki_swd.c
 * Overlay: ovl_Bg_Toki_Swd
 * Description: Master Sword (Contains Cutscenes)
 */

#include "z_bg_toki_swd.h"
#include "objects/object_toki_objects/object_toki_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgTokiSwd*)thisx)

void BgTokiSwd_Init(Actor* thisx, GlobalContext* globalCtx);
void BgTokiSwd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgTokiSwd_Update(Actor* thisx, GlobalContext* globalCtx);
void BgTokiSwd_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808BAF40(BgTokiSwd* self, GlobalContext* globalCtx);
void func_808BB0AC(BgTokiSwd* self, GlobalContext* globalCtx);
void func_808BB128(BgTokiSwd* self, GlobalContext* globalCtx);

extern CutsceneData D_808BB2F0[];
extern CutsceneData D_808BB7A0[];
extern CutsceneData D_808BBD90[];

const ActorInit Bg_Toki_Swd_InitVars = {
    ACTOR_BG_TOKI_SWD,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_TOKI_OBJECTS,
    sizeof(BgTokiSwd),
    (ActorFunc)BgTokiSwd_Init,
    (ActorFunc)BgTokiSwd_Destroy,
    (ActorFunc)BgTokiSwd_Update,
    (ActorFunc)BgTokiSwd_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1 | OC2_UNK1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 10, 70, 0, { 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 10, 35, 100, MASS_IMMOVABLE };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 25, ICHAIN_STOP),
};

void BgTokiSwd_SetupAction(BgTokiSwd* self, BgTokiSwdActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgTokiSwd_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgTokiSwd* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->actor.shape.yOffset = 800.0f;
    BgTokiSwd_SetupAction(self, func_808BAF40);

    if (LINK_IS_ADULT) {
        self->actor.draw = NULL;
    }

    if (gSaveContext.sceneSetupIndex == 5) {
        globalCtx->roomCtx.unk_74[0] = 0xFF;
    }

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, thisx, &sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
}

void BgTokiSwd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgTokiSwd* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_808BAF40(BgTokiSwd* self, GlobalContext* globalCtx) {
    if (((gSaveContext.eventChkInf[4] & 0x8000) == 0) && (gSaveContext.sceneSetupIndex < 4) &&
        Actor_IsFacingAndNearPlayer(&self->actor, 800.0f, 0x7530) && !Gameplay_InCsMode(globalCtx)) {
        gSaveContext.eventChkInf[4] |= 0x8000;
        globalCtx->csCtx.segment = D_808BBD90;
        gSaveContext.cutsceneTrigger = 1;
    }
    if (!LINK_IS_ADULT || ((gSaveContext.eventChkInf[5] & 0x20))) {
        if (Actor_HasParent(&self->actor, globalCtx)) {
            if (!LINK_IS_ADULT) {
                Item_Give(globalCtx, ITEM_SWORD_MASTER);
                globalCtx->csCtx.segment = D_808BB2F0;
            } else {
                globalCtx->csCtx.segment = D_808BB7A0;
            }
            Audio_QueueSeqCmd(NA_BGM_STOP);
            Audio_QueueSeqCmd(0x53);
            gSaveContext.cutsceneTrigger = 1;
            self->actor.parent = NULL;
            BgTokiSwd_SetupAction(self, func_808BB0AC);
        } else {
            if (Actor_IsFacingPlayer(&self->actor, 0x2000)) {
                func_8002F580(&self->actor, globalCtx);
            }
        }
    }
    if (gSaveContext.sceneSetupIndex == 5) {
        if (globalCtx->roomCtx.unk_74[0] > 0) {
            globalCtx->roomCtx.unk_74[0]--;
        } else {
            globalCtx->roomCtx.unk_74[0] = 0;
        }
    }
}

void func_808BB0AC(BgTokiSwd* self, GlobalContext* globalCtx) {
    Player* player;

    // if sword has a parent it has been pulled/placed from the pedestal
    if (Actor_HasParent(&self->actor, globalCtx)) {
        if (!LINK_IS_ADULT) {
            Audio_PlayActorSound2(&self->actor, NA_SE_IT_SWORD_PUTAWAY_STN);
            self->actor.draw = NULL; // sword has been pulled, dont draw sword
        } else {
            self->actor.draw = BgTokiSwd_Draw; // sword has been placed, draw the master sword
        }
        BgTokiSwd_SetupAction(self, func_808BB128);
    } else {
        player = GET_PLAYER(globalCtx);
        player->interactRangeActor = &self->actor;
    }
}

void func_808BB128(BgTokiSwd* self, GlobalContext* globalCtx) {
    if (Flags_GetEnv(globalCtx, 1) && (globalCtx->roomCtx.unk_74[0] < 0xFF)) {
        globalCtx->roomCtx.unk_74[0] += 5;
    }
}

void BgTokiSwd_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgTokiSwd* self = THIS;

    self->actionFunc(self, globalCtx);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void BgTokiSwd_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgTokiSwd* self = THIS;
    s32 pad[3];

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_toki_swd.c", 727);

    func_80093D18(globalCtx->state.gfxCtx);

    func_8002EBCC(&self->actor, globalCtx, 0);

    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TexScroll(globalCtx->state.gfxCtx, 0, -(globalCtx->gameplayFrames % 0x80), 32, 32));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_toki_swd.c", 742),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_toki_objects_DL_001BD0);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_toki_swd.c", 776);
}
