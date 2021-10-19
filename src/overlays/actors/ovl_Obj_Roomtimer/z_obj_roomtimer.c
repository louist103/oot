/*
 * File: z_obj_roomtimer.c
 * Overlay: ovl_Obj_Roomtimer
 * Description: Starts Timer 1 with a value specified in params
 */

#include "z_obj_roomtimer.h"

#define FLAGS 0x00000010

#define THIS ((ObjRoomtimer*)thisx)

void ObjRoomtimer_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjRoomtimer_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjRoomtimer_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80B9D054(ObjRoomtimer* self, GlobalContext* globalCtx);
void func_80B9D0B0(ObjRoomtimer* self, GlobalContext* globalCtx);

const ActorInit Obj_Roomtimer_InitVars = {
    ACTOR_OBJ_ROOMTIMER,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjRoomtimer),
    (ActorFunc)ObjRoomtimer_Init,
    (ActorFunc)ObjRoomtimer_Destroy,
    (ActorFunc)ObjRoomtimer_Update,
    (ActorFunc)NULL,
};

void ObjRoomtimer_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjRoomtimer* self = THIS;
    s16 params = self->actor.params;

    self->switchFlag = (params >> 10) & 0x3F;
    self->actor.params = params & 0x3FF;
    params = self->actor.params;

    if (params != 0x3FF) {
        if (params > 600) {
            self->actor.params = 600;
        } else {
            self->actor.params = params;
        }
    }

    self->actionFunc = func_80B9D054;
}

void ObjRoomtimer_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ObjRoomtimer* self = THIS;

    if ((self->actor.params != 0x3FF) && (gSaveContext.timer1Value > 0)) {
        gSaveContext.timer1State = 10;
    }
}

void func_80B9D054(ObjRoomtimer* self, GlobalContext* globalCtx) {
    if (self->actor.params != 0x3FF) {
        func_80088B34(self->actor.params);
    }

    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
    self->actionFunc = func_80B9D0B0;
}

void func_80B9D0B0(ObjRoomtimer* self, GlobalContext* globalCtx) {
    if (Flags_GetTempClear(globalCtx, self->actor.room)) {
        if (self->actor.params != 0x3FF) {
            gSaveContext.timer1State = 10;
        }
        Flags_SetClear(globalCtx, self->actor.room);
        Flags_SetSwitch(globalCtx, self->switchFlag);
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        Actor_Kill(&self->actor);
    } else {
        if ((self->actor.params != 0x3FF) && (gSaveContext.timer1Value == 0)) {
            Audio_PlaySoundGeneral(NA_SE_OC_ABYSS, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            Gameplay_TriggerVoidOut(globalCtx);
            Actor_Kill(&self->actor);
        }
    }
}

void ObjRoomtimer_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjRoomtimer* self = THIS;

    self->actionFunc(self, globalCtx);
}
