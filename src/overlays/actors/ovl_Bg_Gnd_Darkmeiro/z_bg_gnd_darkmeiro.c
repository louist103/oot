/*
 * File: z_bg_gnd_darkmeiro.c
 * Overlay: ovl_Bg_Gnd_Darkmeiro
 * Description: Shadow trial actors (invisible path, clear block, and timer)
 */

#include "z_bg_gnd_darkmeiro.h"
#include "objects/object_demo_kekkai/object_demo_kekkai.h"

#define FLAGS 0x00000030

#define THIS ((BgGndDarkmeiro*)thisx)

void BgGndDarkmeiro_Init(Actor* thisx, GlobalContext* globalCtx);
void BgGndDarkmeiro_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgGndDarkmeiro_Update(Actor* thisx, GlobalContext* globalCtx);
void BgGndDarkmeiro_DrawInvisiblePath(Actor* thisx, GlobalContext* globalCtx);
void BgGndDarkmeiro_DrawSwitchBlock(Actor* thisx, GlobalContext* globalCtx);
void BgGndDarkmeiro_DrawStaticBlock(Actor* thisx, GlobalContext* globalCtx);

void BgGndDarkmeiro_Noop(BgGndDarkmeiro* self, GlobalContext* globalCtx);
void BgGndDarkmeiro_UpdateBlockTimer(BgGndDarkmeiro* self, GlobalContext* globalCtx);
void BgGndDarkmeiro_UpdateStaticBlock(BgGndDarkmeiro* self, GlobalContext* globalCtx);
void BgGndDarkmeiro_UpdateSwitchBlock(BgGndDarkmeiro* self, GlobalContext* globalCtx);

const ActorInit Bg_Gnd_Darkmeiro_InitVars = {
    ACTOR_BG_GND_DARKMEIRO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DEMO_KEKKAI,
    sizeof(BgGndDarkmeiro),
    (ActorFunc)BgGndDarkmeiro_Init,
    (ActorFunc)BgGndDarkmeiro_Destroy,
    (ActorFunc)BgGndDarkmeiro_Update,
    NULL,
};

void BgGndDarkmeiro_ToggleBlock(BgGndDarkmeiro* self, GlobalContext* globalCtx) {
    if (self->actionFlags & 2) {
        if (self->timer1 == 0) {
            func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            self->actionFlags &= ~2;
        }
    } else if (self->timer1 != 0) {
        func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        self->actionFlags |= 2;
    }
}

void BgGndDarkmeiro_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    CollisionHeader* colHeader = NULL;
    BgGndDarkmeiro* self = THIS;

    self->updateFunc = BgGndDarkmeiro_Noop;
    Actor_SetScale(&self->dyna.actor, 0.1f);
    switch (self->dyna.actor.params & 0xFF) {
        case DARKMEIRO_INVISIBLE_PATH:
            self->dyna.actor.draw = BgGndDarkmeiro_DrawInvisiblePath;
            self->dyna.actor.flags |= 0x80;
            break;
        case DARKMEIRO_CLEAR_BLOCK:
            CollisionHeader_GetVirtual(&gClearBlockCol, &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
            if (((self->dyna.actor.params >> 8) & 0x3F) == 0x3F) {
                self->updateFunc = BgGndDarkmeiro_UpdateStaticBlock;
                self->dyna.actor.draw = BgGndDarkmeiro_DrawStaticBlock;
            } else {
                self->actionFlags = self->timer1 = self->timer2 = 0;
                thisx->draw = BgGndDarkmeiro_DrawSwitchBlock;
                self->updateFunc = BgGndDarkmeiro_UpdateSwitchBlock;
                if (!Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
                    func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                } else {
                    self->timer1 = 64;
                    self->actionFlags |= 2;
                }
            }
            break;
        case DARKMEIRO_BLOCK_TIMER:
            self->actionFlags = self->timer1 = self->timer2 = 0;
            self->updateFunc = BgGndDarkmeiro_UpdateBlockTimer;
            thisx->draw = NULL;
            if (Flags_GetSwitch(globalCtx, ((self->dyna.actor.params >> 8) & 0x3F) + 1)) {
                self->timer1 = 64;
                self->actionFlags |= 4;
            }
            if (Flags_GetSwitch(globalCtx, ((self->dyna.actor.params >> 8) & 0x3F) + 2)) {
                self->timer2 = 64;
                self->actionFlags |= 8;
            }
            if ((self->timer1 != 0) || (self->timer2 != 0)) {
                Flags_SetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);
            } else {
                Flags_UnsetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);
            }
            break;
    }
}

void BgGndDarkmeiro_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgGndDarkmeiro* self = THIS;

    if ((self->dyna.actor.params & 0xFF) == 1) {
        if (1) {}
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
}

void BgGndDarkmeiro_Noop(BgGndDarkmeiro* self, GlobalContext* globalCtx) {
}

void BgGndDarkmeiro_UpdateBlockTimer(BgGndDarkmeiro* self, GlobalContext* globalCtx) {
    s16 timeLeft;

    if (Flags_GetSwitch(globalCtx, ((self->dyna.actor.params >> 8) & 0x3F) + 1)) {
        if (self->actionFlags & 4) {
            if (self->timer1 > 0) {
                self->timer1--;
            } else {
                Flags_UnsetSwitch(globalCtx, ((self->dyna.actor.params >> 8) & 0x3F) + 1);
                self->actionFlags &= ~4;
            }
        } else {
            self->actionFlags |= 4;
            self->timer1 = 304;
            Audio_PlaySoundGeneral(NA_SE_EV_RED_EYE, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        }
    }

    if (Flags_GetSwitch(globalCtx, ((self->dyna.actor.params >> 8) & 0x3F) + 2)) {
        if (self->actionFlags & 8) {
            if (self->timer2 > 0) {
                self->timer2--;
            } else {
                Flags_UnsetSwitch(globalCtx, ((self->dyna.actor.params >> 8) & 0x3F) + 2);
                self->actionFlags &= ~8;
            }
        } else {
            self->actionFlags |= 8;
            self->timer2 = 304;
            Audio_PlaySoundGeneral(NA_SE_EV_RED_EYE, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        }
    }

    timeLeft = CLAMP_MIN(self->timer1, self->timer2);
    if (timeLeft > 0) {
        func_8002F994(&self->dyna.actor, timeLeft);
    }
    if ((self->timer1 >= 64) || (self->timer2 >= 64)) {
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);
    } else {
        Flags_UnsetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);
    }
}

void BgGndDarkmeiro_UpdateStaticBlock(BgGndDarkmeiro* self, GlobalContext* globalCtx) {
}

void BgGndDarkmeiro_UpdateSwitchBlock(BgGndDarkmeiro* self, GlobalContext* globalCtx) {
    if (self->timer1 > 0) {
        self->timer1--;
    }

    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
        self->timer1 = 64;
    }

    BgGndDarkmeiro_ToggleBlock(self, globalCtx);
}

void BgGndDarkmeiro_Update(Actor* thisx, GlobalContext* globalCtx2) {
    BgGndDarkmeiro* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    self->updateFunc(self, globalCtx2);
}

void BgGndDarkmeiro_DrawInvisiblePath(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListXlu(globalCtx, gShadowTrialPathDL);
}

void BgGndDarkmeiro_DrawSwitchBlock(Actor* thisx, GlobalContext* globalCtx) {
    BgGndDarkmeiro* self = THIS;
    s16 vanishTimer;

    vanishTimer = self->timer1;
    if (vanishTimer != 0) {
        if (vanishTimer > 64) {
            self->timer2 = (self->timer2 < 120) ? self->timer2 + 8 : 127;
        } else if (vanishTimer > 16) {
            self->timer2 = (Math_CosS((u16)self->timer1 * 0x1000) * 64.0f) + 127.0f;
            if (self->timer2 > 127) {
                self->timer2 = 127;
            }
        } else {
            self->timer2 = vanishTimer * 8;
        }

        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_gnd_darkmeiro.c", 378);
        //! @bug Due to a bug in the display list, the transparency data is not used.
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 198, 202, 208, self->timer2);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_gnd_darkmeiro.c", 380);

        Gfx_DrawDListXlu(globalCtx, gClearBlockDL);
    }
}

void BgGndDarkmeiro_DrawStaticBlock(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_gnd_darkmeiro.c", 391);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 198, 202, 208, 255);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_gnd_darkmeiro.c", 393);

    Gfx_DrawDListXlu(globalCtx, gClearBlockDL);
}
