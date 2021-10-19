/*
 * File: z_demo_ext.c
 * Overlay: Demo_Ext
 * Description: Magic Vortex in Silver Gauntlets Cutscene
 */

#include "z_demo_ext.h"
#include "vt.h"
#include "objects/object_fhg/object_fhg.h"

#define FLAGS 0x00000010

#define THIS ((DemoExt*)thisx)

typedef enum {
    /* 0x00 */ EXT_WAIT,
    /* 0x01 */ EXT_MAINTAIN,
    /* 0x02 */ EXT_DISPELL
} DemoExtAction;

typedef enum {
    /* 0x00 */ EXT_DRAW_NOTHING,
    /* 0x01 */ EXT_DRAW_VORTEX
} DemoExtDrawMode;

void DemoExt_Init(Actor* thisx, GlobalContext* globalCtx);
void DemoExt_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DemoExt_Update(Actor* thisx, GlobalContext* globalCtx);
void DemoExt_Draw(Actor* thisx, GlobalContext* globalCtx);

void DemoExt_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void DemoExt_Init(Actor* thisx, GlobalContext* globalCtx) {
    DemoExt* self = THIS;

    self->scrollIncr[0] = 25;
    self->scrollIncr[1] = 40;
    self->scrollIncr[2] = 5;
    self->scrollIncr[3] = 30;
    self->primAlpha = kREG(28) + 255;
    self->envAlpha = kREG(32) + 255;
    self->scale.x = kREG(19) + 400.0f;
    self->scale.y = kREG(20) + 100.0f;
    self->scale.z = kREG(21) + 400.0f;
}

void DemoExt_PlayVortexSFX(DemoExt* self) {
    if (self->alphaTimer <= (kREG(35) + 40.0f) - 15.0f) {
        Audio_PlaySoundGeneral(NA_SE_EV_FANTOM_WARP_L - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0,
                               &D_801333E0, &D_801333E8);
    }
}

CsCmdActorAction* DemoExt_GetNpcAction(GlobalContext* globalCtx, s32 npcActionIndex) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        return globalCtx->csCtx.npcActions[npcActionIndex];
    }
    return NULL;
}

void DemoExt_SetupWait(DemoExt* self) {
    self->action = EXT_WAIT;
    self->drawMode = EXT_DRAW_NOTHING;
}

void DemoExt_SetupMaintainVortex(DemoExt* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = DemoExt_GetNpcAction(globalCtx, 5);

    if (npcAction != NULL) {
        self->actor.world.pos.x = npcAction->startPos.x;
        self->actor.world.pos.y = npcAction->startPos.y;
        self->actor.world.pos.z = npcAction->startPos.z;
        self->actor.world.rot.y = self->actor.shape.rot.y = npcAction->rot.y;
    }
    self->action = EXT_MAINTAIN;
    self->drawMode = EXT_DRAW_VORTEX;
}

void DemoExt_SetupDispellVortex(DemoExt* self) {
    self->action = EXT_DISPELL;
    self->drawMode = EXT_DRAW_VORTEX;
}

void DemoExt_FinishClosing(DemoExt* self) {
    self->alphaTimer += 1.0f;
    if ((kREG(35) + 40.0f) <= self->alphaTimer) {
        Actor_Kill(&self->actor);
    }
}

void DemoExt_CheckCsMode(DemoExt* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction = DemoExt_GetNpcAction(globalCtx, 5);
    s32 csAction;
    s32 previousCsAction;

    if (csCmdNPCAction != NULL) {
        csAction = csCmdNPCAction->action;
        previousCsAction = self->previousCsAction;

        if (csAction != previousCsAction) {
            switch (csAction) {
                case 1:
                    DemoExt_SetupWait(self);
                    break;
                case 2:
                    DemoExt_SetupMaintainVortex(self, globalCtx);
                    break;
                case 3:
                    DemoExt_SetupDispellVortex(self);
                    break;
                default:
                    // "Demo_Ext_Check_DemoMode: there is no such action!"
                    osSyncPrintf("Demo_Ext_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
                    break;
            }
            self->previousCsAction = csAction;
        }
    }
}

void DemoExt_SetScrollAndRotation(DemoExt* self) {
    s16* scrollIncr = self->scrollIncr;
    s16* curScroll = self->curScroll;
    s32 i;

    for (i = 3; i != 0; i--) {
        curScroll[i] += scrollIncr[i];
    }
    self->rotationPitch += (s16)(kREG(34) + 1000);
}

void DemoExt_SetColorsAndScales(DemoExt* self) {
    Vec3f* scale = &self->scale;
    f32 shrinkFactor;

    shrinkFactor = ((kREG(35) + 40.0f) - self->alphaTimer) / (kREG(35) + 40.0f);
    if (shrinkFactor < 0.0f) {
        shrinkFactor = 0.0f;
    }

    self->primAlpha = (u32)(kREG(28) + 255) * shrinkFactor;
    self->envAlpha = (u32)(kREG(32) + 255) * shrinkFactor;
    scale->x = (kREG(19) + 400.0f) * shrinkFactor;
    scale->y = (kREG(20) + 100.0f) * shrinkFactor;
    scale->z = (kREG(21) + 400.0f) * shrinkFactor;
}

void DemoExt_Wait(DemoExt* self, GlobalContext* globalCtx) {
    DemoExt_CheckCsMode(self, globalCtx);
}

void DemoExt_MaintainVortex(DemoExt* self, GlobalContext* globalCtx) {
    DemoExt_PlayVortexSFX(self);
    DemoExt_SetScrollAndRotation(self);
    DemoExt_CheckCsMode(self, globalCtx);
}

void DemoExt_DispellVortex(DemoExt* self, GlobalContext* globalCtx) {
    DemoExt_PlayVortexSFX(self);
    DemoExt_SetScrollAndRotation(self);
    DemoExt_SetColorsAndScales(self);
    DemoExt_FinishClosing(self);
}

static DemoExtActionFunc sActionFuncs[] = {
    DemoExt_Wait,
    DemoExt_MaintainVortex,
    DemoExt_DispellVortex,
};

void DemoExt_Update(Actor* thisx, GlobalContext* globalCtx) {
    DemoExt* self = THIS;

    if ((self->action < EXT_WAIT) || (self->action > EXT_DISPELL) || sActionFuncs[self->action] == NULL) {
        // "Main mode is abnormal!"
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sActionFuncs[self->action](self, globalCtx);
    }
}

void DemoExt_DrawNothing(Actor* thisx, GlobalContext* globalCtx) {
}

void DemoExt_DrawVortex(Actor* thisx, GlobalContext* globalCtx) {
    DemoExt* self = THIS;
    Mtx* mtx;
    GraphicsContext* gfxCtx;
    s16* curScroll;
    Vec3f* scale;

    scale = &self->scale;
    gfxCtx = globalCtx->state.gfxCtx;
    mtx = Graph_Alloc(gfxCtx, sizeof(Mtx));

    OPEN_DISPS(gfxCtx, "../z_demo_ext.c", 460);
    Matrix_Push();
    Matrix_Scale(scale->x, scale->y, scale->z, MTXMODE_APPLY);
    Matrix_RotateRPY((s16)(kREG(16) + 0x4000), self->rotationPitch, kREG(18), MTXMODE_APPLY);
    Matrix_Translate(kREG(22), kREG(23), kREG(24), MTXMODE_APPLY);
    Matrix_ToMtx(mtx, "../z_demo_ext.c", 476);
    Matrix_Pop();
    func_80093D84(gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, kREG(33) + 128, kREG(25) + 140, kREG(26) + 80, kREG(27) + 140, self->primAlpha);
    gDPSetEnvColor(POLY_XLU_DISP++, kREG(29) + 90, kREG(30) + 50, kREG(31) + 95, self->envAlpha);

    curScroll = self->curScroll;
    gSPSegment(
        POLY_XLU_DISP++, 0x08,
        Gfx_TwoTexScroll(gfxCtx, 0, curScroll[0], curScroll[1], 0x40, 0x40, 1, curScroll[2], curScroll[3], 0x40, 0x40));

    gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gPhantomWarpDL);
    gSPPopMatrix(POLY_XLU_DISP++, G_MTX_MODELVIEW);

    CLOSE_DISPS(gfxCtx, "../z_demo_ext.c", 512);
}

static DemoExtDrawFunc sDrawFuncs[] = {
    DemoExt_DrawNothing,
    DemoExt_DrawVortex,
};

void DemoExt_Draw(Actor* thisx, GlobalContext* globalCtx) {
    DemoExt* self = THIS;

    if ((self->drawMode < EXT_DRAW_NOTHING) || (self->drawMode > EXT_DRAW_VORTEX) ||
        sDrawFuncs[self->drawMode] == NULL) {
        // "Draw mode is abnormal!"
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sDrawFuncs[self->drawMode](thisx, globalCtx);
    }
}

const ActorInit Demo_Ext_InitVars = {
    ACTOR_DEMO_EXT,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_FHG,
    sizeof(DemoExt),
    (ActorFunc)DemoExt_Init,
    (ActorFunc)DemoExt_Destroy,
    (ActorFunc)DemoExt_Update,
    (ActorFunc)DemoExt_Draw,
};
