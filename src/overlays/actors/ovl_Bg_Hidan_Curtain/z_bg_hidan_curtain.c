/*
 * File: z_bg_hidan_curtain.c
 * Overlay: ovl_Bg_Hidan_Curtain
 * Description: Flame circle
 */

#include "z_bg_hidan_curtain.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000010

#define THIS ((BgHidanCurtain*)thisx)

void BgHidanCurtain_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanCurtain_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanCurtain_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanCurtain_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgHidanCurtain_WaitForSwitchOn(BgHidanCurtain* self, GlobalContext* globalCtx);
void BgHidanCurtain_WaitForCutscene(BgHidanCurtain* self, GlobalContext* globalCtx);
void BgHidanCurtain_WaitForClear(BgHidanCurtain* self, GlobalContext* globalCtx);
void BgHidanCurtain_TurnOn(BgHidanCurtain* self, GlobalContext* globalCtx);
void BgHidanCurtain_TurnOff(BgHidanCurtain* self, GlobalContext* globalCtx);
void BgHidanCurtain_WaitForTimer(BgHidanCurtain* self, GlobalContext* globalCtx);

typedef struct {
    /* 0x00 */ s16 radius;
    /* 0x02 */ s16 height;
    /* 0x04 */ f32 scale;
    /* 0x08 */ f32 riseDist;
    /* 0x0C */ f32 riseSpeed;
} BgHidanCurtainParams; // size = 0x10

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x20000000, 0x01, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 81, 144, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sCcInfoInit = { 1, 80, 100, MASS_IMMOVABLE };

static BgHidanCurtainParams sHCParams[] = { { 81, 144, 0.090f, 144.0f, 5.0f }, { 46, 88, 0.055f, 88.0f, 3.0f } };

const ActorInit Bg_Hidan_Curtain_InitVars = {
    ACTOR_BG_HIDAN_CURTAIN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHidanCurtain),
    (ActorFunc)BgHidanCurtain_Init,
    (ActorFunc)BgHidanCurtain_Destroy,
    (ActorFunc)BgHidanCurtain_Update,
    (ActorFunc)BgHidanCurtain_Draw,
};

void BgHidanCurtain_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHidanCurtain* self = THIS;
    BgHidanCurtainParams* hcParams;

    osSyncPrintf("Curtain (arg_data 0x%04x)\n", self->actor.params);
    Actor_SetFocus(&self->actor, 20.0f);
    self->type = (thisx->params >> 0xC) & 0xF;
    if (self->type > 6) {
        // "Type is not set"
        osSyncPrintf("Error : object のタイプが設定されていない(%s %d)(arg_data 0x%04x)\n", "../z_bg_hidan_curtain.c",
                     352, self->actor.params);
        Actor_Kill(&self->actor);
        return;
    }

    self->size = ((self->type == 2) || (self->type == 4)) ? 1 : 0;
    hcParams = &sHCParams[self->size];
    self->treasureFlag = (thisx->params >> 6) & 0x3F;
    thisx->params &= 0x3F;

    if ((self->actor.params < 0) || (self->actor.params > 0x3F)) {
        // "Save bit is not set"
        osSyncPrintf("Warning : object のセーブビットが設定されていない(%s %d)(arg_data 0x%04x)\n",
                     "../z_bg_hidan_curtain.c", 373, self->actor.params);
    }
    Actor_SetScale(&self->actor, hcParams->scale);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->collider.dim.pos.x = self->actor.world.pos.x;
    self->collider.dim.pos.y = self->actor.world.pos.y;
    self->collider.dim.pos.z = self->actor.world.pos.z;
    self->collider.dim.radius = hcParams->radius;
    self->collider.dim.height = hcParams->height;
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &sCcInfoInit);
    if (self->type == 0) {
        self->actionFunc = BgHidanCurtain_WaitForClear;
    } else {
        self->actionFunc = BgHidanCurtain_WaitForSwitchOn;
        if ((self->type == 4) || (self->type == 5)) {
            self->actor.world.pos.y = self->actor.home.pos.y - hcParams->riseDist;
        }
    }
    if (((self->type == 1) && Flags_GetTreasure(globalCtx, self->treasureFlag)) ||
        (((self->type == 0) || (self->type == 6)) && Flags_GetClear(globalCtx, self->actor.room))) {
        Actor_Kill(&self->actor);
    }
    self->texScroll = Rand_ZeroOne() * 15.0f;
}

void BgHidanCurtain_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHidanCurtain* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void BgHidanCurtain_WaitForSwitchOn(BgHidanCurtain* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->actor.params)) {
        if (self->type == 1) {
            self->actionFunc = BgHidanCurtain_WaitForCutscene;
            OnePointCutscene_Init(globalCtx, 3350, -99, &self->actor, MAIN_CAM);
            self->timer = 50;
        } else if (self->type == 3) {
            self->actionFunc = BgHidanCurtain_WaitForCutscene;
            OnePointCutscene_Init(globalCtx, 3360, 60, &self->actor, MAIN_CAM);
            self->timer = 30;
        } else {
            self->actionFunc = BgHidanCurtain_TurnOff;
        }
    }
}

void BgHidanCurtain_WaitForCutscene(BgHidanCurtain* self, GlobalContext* globalCtx) {
    if (self->timer-- == 0) {
        self->actionFunc = BgHidanCurtain_TurnOff;
    }
}

void BgHidanCurtain_WaitForClear(BgHidanCurtain* self, GlobalContext* globalCtx) {
    if (Flags_GetClear(globalCtx, self->actor.room)) {
        self->actionFunc = BgHidanCurtain_TurnOff;
    }
}

void BgHidanCurtain_WaitForSwitchOff(BgHidanCurtain* self, GlobalContext* globalCtx) {
    if (!Flags_GetSwitch(globalCtx, self->actor.params)) {
        self->actionFunc = BgHidanCurtain_TurnOn;
    }
}

void BgHidanCurtain_TurnOn(BgHidanCurtain* self, GlobalContext* globalCtx) {
    f32 riseSpeed = sHCParams[self->size].riseSpeed;

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, riseSpeed)) {
        Flags_UnsetSwitch(globalCtx, self->actor.params);
        self->actionFunc = BgHidanCurtain_WaitForSwitchOn;
    }
}

void BgHidanCurtain_TurnOff(BgHidanCurtain* self, GlobalContext* globalCtx) {
    BgHidanCurtainParams* hcParams = &sHCParams[self->size];

    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - hcParams->riseDist, hcParams->riseSpeed)) {
        if ((self->type == 0) || (self->type == 6)) {
            Actor_Kill(&self->actor);
        } else if (self->type == 5) {
            self->actionFunc = BgHidanCurtain_WaitForSwitchOff;
        } else {
            if (self->type == 2) {
                self->timer = 400;
            } else if (self->type == 4) {
                self->timer = 200;
            } else if (self->type == 3) {
                self->timer = 160;
            } else { // self->type == 1
                self->timer = 300;
            }
            self->actionFunc = BgHidanCurtain_WaitForTimer;
        }
    }
}

void BgHidanCurtain_WaitForTimer(BgHidanCurtain* self, GlobalContext* globalCtx) {
    DECR(self->timer);
    if (self->timer == 0) {
        self->actionFunc = BgHidanCurtain_TurnOn;
    }
    if ((self->type == 1) || (self->type == 3)) {
        func_8002F994(&self->actor, self->timer);
    }
}

void BgHidanCurtain_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgHidanCurtain* self = THIS;
    BgHidanCurtainParams* hcParams = &sHCParams[self->size];
    f32 riseProgress;

    if ((globalCtx->cameraPtrs[MAIN_CAM]->setting == CAM_SET_ITEM0) ||
        (globalCtx->cameraPtrs[MAIN_CAM]->setting == CAM_SET_ITEM2)) {
        self->collider.base.atFlags &= ~AT_HIT;
    } else {
        if (self->collider.base.atFlags & AT_HIT) {
            self->collider.base.atFlags &= ~AT_HIT;
            func_8002F71C(globalCtx, &self->actor, 5.0f, self->actor.yawTowardsPlayer, 1.0f);
        }
        if ((self->type == 4) || (self->type == 5)) {
            self->actor.world.pos.y = (2.0f * self->actor.home.pos.y) - hcParams->riseDist - self->actor.world.pos.y;
        }

        self->actionFunc(self, globalCtx);

        if ((self->type == 4) || (self->type == 5)) {
            self->actor.world.pos.y = (2.0f * self->actor.home.pos.y) - hcParams->riseDist - self->actor.world.pos.y;
        }
        riseProgress = (hcParams->riseDist - (self->actor.home.pos.y - self->actor.world.pos.y)) / hcParams->riseDist;
        self->alpha = 255.0f * riseProgress;
        if (self->alpha > 50) {
            self->collider.dim.height = hcParams->height * riseProgress;
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            if (gSaveContext.sceneSetupIndex <= 3) {
                func_8002F974(&self->actor, NA_SE_EV_FIRE_PILLAR_S - SFX_FLAG);
            }
        } else if ((self->type == 1) && Flags_GetTreasure(globalCtx, self->treasureFlag)) {
            Actor_Kill(&self->actor);
        }
        self->texScroll++;
    }
}

void BgHidanCurtain_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanCurtain* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_curtain.c", 685);
    func_80093D84(globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 220, 0, self->alpha);

    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, self->texScroll & 0x7F, 0, 0x20, 0x40, 1, 0,
                                (self->texScroll * -0xF) & 0xFF, 0x20, 0x40));

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_hidan_curtain.c", 698),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_XLU_DISP++, gEffFireCircleDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_curtain.c", 702);
}
