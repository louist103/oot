/*
 * File: z_bg_ddan_jd.c
 * Overlay: ovl_Bg_Ddan_Jd
 * Description: Rising stone platform (Dodongo's Cavern)
 */

#include "z_bg_ddan_jd.h"
#include "objects/object_ddan_objects/object_ddan_objects.h"

#define FLAGS 0x00000030

#define THIS ((BgDdanJd*)thisx)

void BgDdanJd_Init(Actor* thisx, GlobalContext* globalCtx);
void BgDdanJd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgDdanJd_Update(Actor* thisx, GlobalContext* globalCtx);
void BgDdanJd_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgDdanJd_Idle(BgDdanJd* self, GlobalContext* globalCtx);
void BgDdanJd_Move(BgDdanJd* self, GlobalContext* globalCtx);

const ActorInit Bg_Ddan_Jd_InitVars = {
    ACTOR_BG_DDAN_JD,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DDAN_OBJECTS,
    sizeof(BgDdanJd),
    (ActorFunc)BgDdanJd_Init,
    (ActorFunc)BgDdanJd_Destroy,
    (ActorFunc)BgDdanJd_Update,
    (ActorFunc)BgDdanJd_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

typedef enum {
    /* 0 */ STATE_GO_BOTTOM,
    /* 1 */ STATE_GO_MIDDLE_FROM_BOTTOM,
    /* 2 */ STATE_GO_MIDDLE_FROM_TOP,
    /* 3 */ STATE_GO_TOP
} BgDdanJdState;

#define MOVE_HEIGHT_MIDDLE 140.0f
#define MOVE_HEIGHT_TOP 700.0f

#define IDLE_FRAMES 100

// Since ySpeed is used to determine if the platform should rise to the top of the dungeon, these must be assigned
// different values in order for the shortcut to work correctly
#define DEFAULT_Y_SPEED 1
#define SHORTCUT_Y_SPEED 5

void BgDdanJd_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgDdanJd* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gDodongoRisingPlatformCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    self->idleTimer = IDLE_FRAMES;
    self->state = STATE_GO_BOTTOM;

    // Missing check for actor.params < 0x40. This will cause inconsistent behavior if params >= 0x40 and the bound
    // switch state is turned on while in the same room, as the shortcut behavior won't become enabled until the actor
    // is reloaded.
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params)) {
        self->ySpeed = SHORTCUT_Y_SPEED;
    } else {
        self->ySpeed = DEFAULT_Y_SPEED;
    }
    self->actionFunc = BgDdanJd_Idle;
}

void BgDdanJd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanJd* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgDdanJd_Idle(BgDdanJd* self, GlobalContext* globalCtx) {
    if (self->idleTimer != 0) {
        self->idleTimer--;
    }

    // if self is the platform that rises all the way to the top, and the switch state has just changed to on
    if (self->ySpeed == DEFAULT_Y_SPEED && self->dyna.actor.params < 0x40 &&
        Flags_GetSwitch(globalCtx, self->dyna.actor.params)) {
        self->ySpeed = SHORTCUT_Y_SPEED;
        self->state = STATE_GO_MIDDLE_FROM_BOTTOM;
        self->idleTimer = 0;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + MOVE_HEIGHT_MIDDLE;
        OnePointCutscene_Init(globalCtx, 3060, -99, &self->dyna.actor, MAIN_CAM);
    }
    if (self->idleTimer == 0) {
        self->idleTimer = IDLE_FRAMES;
        if (self->state == STATE_GO_BOTTOM) {
            self->state = STATE_GO_MIDDLE_FROM_BOTTOM;
            self->targetY = self->dyna.actor.home.pos.y + MOVE_HEIGHT_MIDDLE;
        } else if (self->state == STATE_GO_MIDDLE_FROM_BOTTOM) {
            // If the platform has been activated as a shortcut
            if (self->ySpeed != DEFAULT_Y_SPEED) {
                self->state = STATE_GO_TOP;
                self->targetY = self->dyna.actor.home.pos.y + MOVE_HEIGHT_TOP;
            } else {
                self->state = STATE_GO_BOTTOM;
                self->targetY = self->dyna.actor.home.pos.y;
            }
        } else if (self->state == STATE_GO_MIDDLE_FROM_TOP) {
            // If the platform has been activated as a shortcut
            if (self->ySpeed != DEFAULT_Y_SPEED) {
                self->state = STATE_GO_TOP;
                self->targetY = self->dyna.actor.home.pos.y + MOVE_HEIGHT_TOP;
            } else {
                self->state = STATE_GO_BOTTOM;
                self->targetY = self->dyna.actor.home.pos.y;
            }
        } else if (self->state == STATE_GO_TOP) {
            self->state = STATE_GO_MIDDLE_FROM_TOP;
            self->targetY = self->dyna.actor.home.pos.y + MOVE_HEIGHT_MIDDLE;
        }
        self->actionFunc = BgDdanJd_Move;
    }
}

// Handles dust particles and sfx when moving
void BgDdanJd_MoveEffects(BgDdanJd* self, GlobalContext* globalCtx) {
    Vec3f dustPos;

    // Generate random dust particles at the platform's base.
    dustPos.y = self->dyna.actor.home.pos.y;
    if (globalCtx->gameplayFrames & 1) {
        dustPos.x = self->dyna.actor.world.pos.x + 65.0f;
        dustPos.z = Rand_CenteredFloat(110.0f) + self->dyna.actor.world.pos.z;
        func_80033480(globalCtx, &dustPos, 5.0f, 1, 20, 60, 1);
        dustPos.x = self->dyna.actor.world.pos.x - 65.0f;
        dustPos.z = Rand_CenteredFloat(110.0f) + self->dyna.actor.world.pos.z;
        func_80033480(globalCtx, &dustPos, 5.0f, 1, 20, 60, 1);
    } else {
        dustPos.x = Rand_CenteredFloat(110.0f) + self->dyna.actor.world.pos.x;
        dustPos.z = self->dyna.actor.world.pos.z + 65.0f;
        func_80033480(globalCtx, &dustPos, 5.0f, 1, 20, 60, 1);
        dustPos.x = Rand_CenteredFloat(110.0f) + self->dyna.actor.world.pos.x;
        dustPos.z = self->dyna.actor.world.pos.z - 65.0f;
        func_80033480(globalCtx, &dustPos, 5.0f, 1, 20, 60, 1);
    }
    if (self->ySpeed == SHORTCUT_Y_SPEED) {
        func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
    }
}

// Implements the platform's movement state
void BgDdanJd_Move(BgDdanJd* self, GlobalContext* globalCtx) {
    // if self is the platform that rises all the way to the top, and the switch state has just changed to on
    if (self->ySpeed == DEFAULT_Y_SPEED && self->dyna.actor.params < 0x40 &&
        Flags_GetSwitch(globalCtx, self->dyna.actor.params)) {
        self->ySpeed = SHORTCUT_Y_SPEED;
        self->state = STATE_GO_MIDDLE_FROM_BOTTOM;
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + MOVE_HEIGHT_MIDDLE;
        self->idleTimer = 0;
        self->actionFunc = BgDdanJd_Idle;
        OnePointCutscene_Init(globalCtx, 3060, -99, &self->dyna.actor, MAIN_CAM);
    } else if (Math_StepToF(&self->dyna.actor.world.pos.y, self->targetY, self->ySpeed)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_PILLAR_MOVE_STOP);
        self->actionFunc = BgDdanJd_Idle;
    }
    BgDdanJd_MoveEffects(self, globalCtx);
}

void BgDdanJd_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanJd* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgDdanJd_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gDodongoRisingPlatformDL);
}
