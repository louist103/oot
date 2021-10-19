/*
 * File: z_bg_hidan_fwbig.c
 * Overlay: ovl_Bg_Hidan_Fwbig
 * Description: Large fire walls at Fire Temple (flame wall before bombable door and the one that chases the player in
 * the lava room)
 */

#include "z_bg_hidan_fwbig.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgHidanFwbig*)thisx)

typedef enum {
    /* 0 */ FWBIG_MOVE,
    /* 1 */ FWBIG_RESET,
    /* 2 */ FWBIG_KILL
} HidanFwbigMoveState;

void BgHidanFwbig_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFwbig_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFwbig_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFwbig_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgHidanFwbig_UpdatePosition(BgHidanFwbig* self);

void BgHidanFwbig_WaitForSwitch(BgHidanFwbig* self, GlobalContext* globalCtx);
void BgHidanFwbig_WaitForCs(BgHidanFwbig* self, GlobalContext* globalCtx);
void BgHidanFwbig_Lower(BgHidanFwbig* self, GlobalContext* globalCtx);
void BgHidanFwbig_WaitForTimer(BgHidanFwbig* self, GlobalContext* globalCtx);
void BgHidanFwbig_WaitForPlayer(BgHidanFwbig* self, GlobalContext* globalCtx);
void BgHidanFwbig_Move(BgHidanFwbig* self, GlobalContext* globalCtx);

const ActorInit Bg_Hidan_Fwbig_InitVars = {
    ACTOR_BG_HIDAN_FWBIG,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanFwbig),
    (ActorFunc)BgHidanFwbig_Init,
    (ActorFunc)BgHidanFwbig_Destroy,
    (ActorFunc)BgHidanFwbig_Update,
    (ActorFunc)BgHidanFwbig_Draw,
};

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
    { 30, 130, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_STOP),
};

void BgHidanFwbig_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgHidanFwbig* self = THIS;
    Player* player = GET_PLAYER(globalCtx);

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->direction = (u16)(thisx->params >> 8);
    thisx->params &= 0xFF;
    if (self->direction != 0) {
        self->actor.home.pos.x = 1560.0f;
        self->actor.home.pos.z = 0.0f;
        if (player->actor.world.pos.z > 300.0f) {
            self->direction = -1;
            self->actor.home.rot.y = self->actor.shape.rot.y = -0x4E38;
        } else if (player->actor.world.pos.z < -300.0f) {
            self->direction = 1;
            self->actor.home.rot.y = self->actor.shape.rot.y = -0x31C8;
        } else {
            Actor_Kill(&self->actor);
            return;
        }
        BgHidanFwbig_UpdatePosition(self);
        Actor_SetScale(&self->actor, 0.15f);
        self->collider.dim.height = 230;
        self->actor.flags |= 0x10;
        self->moveState = FWBIG_MOVE;
        self->actionFunc = BgHidanFwbig_WaitForPlayer;
        self->actor.world.pos.y = self->actor.home.pos.y - (2400.0f * self->actor.scale.y);
    } else {
        Actor_SetScale(&self->actor, 0.1f);
        self->actionFunc = BgHidanFwbig_WaitForSwitch;
    }
}

void BgHidanFwbig_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHidanFwbig* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void BgHidanFwbig_UpdatePosition(BgHidanFwbig* self) {
    s16 startAngle = self->actor.shape.rot.y + self->direction * -0x4000;

    self->actor.world.pos.x = (Math_SinS(startAngle) * 885.4f) + self->actor.home.pos.x;
    self->actor.world.pos.z = (Math_CosS(startAngle) * 885.4f) + self->actor.home.pos.z;
}

void BgHidanFwbig_WaitForSwitch(BgHidanFwbig* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->actor.params)) {
        self->actionFunc = BgHidanFwbig_WaitForCs;
        OnePointCutscene_Init(globalCtx, 3340, -99, &self->actor, MAIN_CAM);
        self->timer = 35;
    }
}

void BgHidanFwbig_WaitForCs(BgHidanFwbig* self, GlobalContext* globalCtx) {
    if (self->timer-- == 0) {
        self->actionFunc = BgHidanFwbig_Lower;
    }
}

void BgHidanFwbig_Rise(BgHidanFwbig* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 10.0f)) {
        if (self->direction == 0) {
            Flags_UnsetSwitch(globalCtx, self->actor.params);
            self->actionFunc = BgHidanFwbig_WaitForSwitch;
        } else {
            self->actionFunc = BgHidanFwbig_Move;
        }
    }
}

void BgHidanFwbig_Lower(BgHidanFwbig* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y - (2400.0f * self->actor.scale.y), 10.0f)) {
        if (self->direction == 0) {
            self->actionFunc = BgHidanFwbig_WaitForTimer;
            self->timer = 150;
        } else if (self->moveState == FWBIG_KILL) {
            Actor_Kill(&self->actor);
        } else {
            if (self->moveState == FWBIG_MOVE) {
                self->actor.shape.rot.y -= (self->direction * 0x1800);
            } else {
                self->moveState = FWBIG_MOVE;
                self->actor.shape.rot.y = self->actor.home.rot.y;
            }
            BgHidanFwbig_UpdatePosition(self);
            self->actionFunc = BgHidanFwbig_Rise;
        }
    }
}

void BgHidanFwbig_WaitForTimer(BgHidanFwbig* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->actionFunc = BgHidanFwbig_Rise;
    }
    func_8002F994(&self->actor, self->timer);
}

void BgHidanFwbig_WaitForPlayer(BgHidanFwbig* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (player->actor.world.pos.x < 1150.0f) {
        self->actionFunc = BgHidanFwbig_Rise;
        OnePointCutscene_Init(globalCtx, 3290, -99, &self->actor, MAIN_CAM);
    }
}

void BgHidanFwbig_Move(BgHidanFwbig* self, GlobalContext* globalCtx) {
    if (!Player_InCsMode(globalCtx)) {
        if (Math_ScaledStepToS(&self->actor.shape.rot.y, self->actor.home.rot.y + (self->direction * 0x6390), 0x20)) {
            self->moveState = FWBIG_RESET;
            self->actionFunc = BgHidanFwbig_Lower;
        } else {
            BgHidanFwbig_UpdatePosition(self);
        }
    }
}

void BgHidanFwbig_MoveCollider(BgHidanFwbig* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f projPos;
    f32 cs;
    f32 sn;

    func_8002DBD0(&self->actor, &projPos, &player->actor.world.pos);
    projPos.z = ((projPos.z >= 0.0f) ? 1.0f : -1.0f) * 25.0f * -1.0f;
    if (self->direction == 0) {
        projPos.x = CLAMP(projPos.x, -360.0f, 360.0f);
    } else {
        projPos.x = CLAMP(projPos.x, -500.0f, 500.0f);
    }

    sn = Math_SinS(self->actor.shape.rot.y);
    cs = Math_CosS(self->actor.shape.rot.y);
    self->collider.dim.pos.x = self->actor.world.pos.x + (projPos.x * cs) + (projPos.z * sn);
    self->collider.dim.pos.z = self->actor.world.pos.z - (projPos.x * sn) + (projPos.z * cs);
    self->collider.dim.pos.y = self->actor.world.pos.y;

    self->actor.world.rot.y = (projPos.z < 0.0f) ? self->actor.shape.rot.y : self->actor.shape.rot.y + 0x8000;
}

void BgHidanFwbig_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHidanFwbig* self = THIS;

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        func_8002F71C(globalCtx, &self->actor, 5.0f, self->actor.world.rot.y, 1.0f);
        if (self->direction != 0) {
            self->actionFunc = BgHidanFwbig_Lower;
        }
    }
    if ((self->direction != 0) && (globalCtx->roomCtx.prevRoom.num == self->actor.room)) {
        self->moveState = FWBIG_KILL;
        self->actionFunc = BgHidanFwbig_Lower;
    }

    self->actionFunc(self, globalCtx);

    if ((self->actor.home.pos.y - 200.0f) < self->actor.world.pos.y) {
        if (gSaveContext.sceneSetupIndex < 4) {
            func_8002F974(&self->actor, NA_SE_EV_BURNING - SFX_FLAG);
        } else if ((s16)self->actor.world.pos.x == -513) {
            func_8002F974(&self->actor, NA_SE_EV_FLAME_OF_FIRE - SFX_FLAG);
        }
        BgHidanFwbig_MoveCollider(self, globalCtx);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void BgHidanFwbig_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    f32 height;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_fwbig.c", 630);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(gEffUnknown4Tex));

    gSPSegment(POLY_XLU_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(gEffUnknown5Tex));

    height = thisx->scale.y * 2400.0f;
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 220, 0,
                    ((height - (thisx->home.pos.y - thisx->world.pos.y)) * 255.0f) / height);

    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, globalCtx->gameplayFrames % 0x80, 0, 0x20, 0x40, 1, 0,
                                (u8)(globalCtx->gameplayFrames * -15), 0x20, 0x40));

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_hidan_fwbig.c", 660),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_XLU_DISP++, gFireTempleBigFireWallDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_fwbig.c", 664);
}
