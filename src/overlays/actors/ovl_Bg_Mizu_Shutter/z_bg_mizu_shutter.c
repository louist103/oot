#include "z_bg_mizu_shutter.h"
#include "objects/object_mizu_objects/object_mizu_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgMizuShutter*)thisx)

#define SIZE_PARAM (((u16)self->dyna.actor.params >> 0xC) & 0xF)
#define TIMER_PARAM (((u16)self->dyna.actor.params >> 6) & 0x3F)

void BgMizuShutter_Init(BgMizuShutter* self, GlobalContext* globalCtx);
void BgMizuShutter_Destroy(BgMizuShutter* self, GlobalContext* globalCtx);
void BgMizuShutter_Update(BgMizuShutter* self, GlobalContext* globalCtx);
void BgMizuShutter_Draw(BgMizuShutter* self, GlobalContext* globalCtx);

void BgMizuShutter_WaitForTimer(BgMizuShutter* self, GlobalContext* globalCtx);
void BgMizuShutter_WaitForSwitch(BgMizuShutter* self, GlobalContext* globalCtx);
void BgMizuShutter_Move(BgMizuShutter* self, GlobalContext* globalCtx);
void BgMizuShutter_WaitForCutscene(BgMizuShutter* self, GlobalContext* globalCtx);

const ActorInit Bg_Mizu_Shutter_InitVars = {
    ACTOR_BG_MIZU_SHUTTER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuShutter),
    (ActorFunc)BgMizuShutter_Init,
    (ActorFunc)BgMizuShutter_Destroy,
    (ActorFunc)BgMizuShutter_Update,
    (ActorFunc)BgMizuShutter_Draw,
};

static Gfx* sDisplayLists[] = { gObjectMizuObjectsShutterDL_007130, gObjectMizuObjectsShutterDL_0072D0 };

static CollisionHeader* sCollisionHeaders[] = {
    &gObjectMizuObjectsShutterCol_007250,
    &gObjectMizuObjectsShutterCol_0073F0,
};

static Vec3f sDisplacements[] = {
    { 0.0f, 100.0f, 0.0f },
    { 0.0f, 140.0f, 0.0f },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgMizuShutter_Init(BgMizuShutter* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuShutter* self = THIS;
    s32 pad2;
    CollisionHeader* sp30 = NULL;
    s32 pad3;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->displayList = sDisplayLists[SIZE_PARAM];
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(sCollisionHeaders[SIZE_PARAM], &sp30);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, sp30);
    if ((SIZE_PARAM == BGMIZUSHUTTER_SMALL) || (SIZE_PARAM == BGMIZUSHUTTER_LARGE)) {
        self->closedPos = self->dyna.actor.world.pos;
        self->timer = 0;
        self->timerMax = TIMER_PARAM * 20;
        Matrix_RotateY(self->dyna.actor.world.rot.y * (M_PI / 0x8000), MTXMODE_NEW);
        Matrix_RotateX(self->dyna.actor.world.rot.x * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_RotateZ(self->dyna.actor.world.rot.z * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_MultVec3f(&sDisplacements[SIZE_PARAM], &self->openPos);
        self->openPos.x += self->dyna.actor.world.pos.x;
        self->openPos.y += self->dyna.actor.world.pos.y;
        self->openPos.z += self->dyna.actor.world.pos.z;
        if (self->timerMax != 0x3F * 20) {
            Flags_UnsetSwitch(globalCtx, (u16)self->dyna.actor.params & 0x3F);
            self->dyna.actor.world.pos = self->closedPos;
        }
        if (Flags_GetSwitch(globalCtx, (u16)self->dyna.actor.params & 0x3F)) {
            self->dyna.actor.world.pos = self->openPos;
            self->actionFunc = BgMizuShutter_WaitForTimer;
        } else {
            self->actionFunc = BgMizuShutter_WaitForSwitch;
        }
    }
}

void BgMizuShutter_Destroy(BgMizuShutter* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuShutter* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgMizuShutter_WaitForSwitch(BgMizuShutter* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (u16)self->dyna.actor.params & 0x3F)) {
        if (ABS(self->dyna.actor.world.rot.x) > 0x2C60) {
            OnePointCutscene_Init(globalCtx, 4510, -99, &self->dyna.actor, MAIN_CAM);
        } else {
            OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        }
        self->actionFunc = BgMizuShutter_WaitForCutscene;
        self->timer = 30;
    }
}

void BgMizuShutter_WaitForCutscene(BgMizuShutter* self, GlobalContext* globalCtx) {
    if (self->timer-- == 0) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_METALDOOR_OPEN);
        self->actionFunc = BgMizuShutter_Move;
    }
}

void BgMizuShutter_Move(BgMizuShutter* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (u16)self->dyna.actor.params & 0x3F)) {
        Math_SmoothStepToF(&self->dyna.actor.world.pos.x, self->openPos.x, 1.0f, 4.0f, 0.1f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->openPos.y, 1.0f, 4.0f, 0.1f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.z, self->openPos.z, 1.0f, 4.0f, 0.1f);
        if ((self->dyna.actor.world.pos.x == self->openPos.x) && (self->dyna.actor.world.pos.y == self->openPos.y) &&
            (self->dyna.actor.world.pos.z == self->openPos.z)) {
            self->timer = self->timerMax;
            self->actionFunc = BgMizuShutter_WaitForTimer;
        }
    } else {
        Math_SmoothStepToF(&self->maxSpeed, 20.0f, 1.0f, 3.0f, 0.1f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.x, self->closedPos.x, 1.0f, self->maxSpeed, 0.1f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->closedPos.y, 1.0f, self->maxSpeed, 0.1f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.z, self->closedPos.z, 1.0f, self->maxSpeed, 0.1f);
        if ((self->dyna.actor.world.pos.x == self->closedPos.x) &&
            (self->dyna.actor.world.pos.y == self->closedPos.y) &&
            (self->dyna.actor.world.pos.z == self->closedPos.z)) {
            func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 0x78, 0x14, 0xA);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_STONE_BOUND);
            self->actionFunc = BgMizuShutter_WaitForSwitch;
        }
    }
}

void BgMizuShutter_WaitForTimer(BgMizuShutter* self, GlobalContext* globalCtx) {
    if (self->timerMax != 0x3F * 20) {
        self->timer--;
        func_8002F994(&self->dyna.actor, self->timer);
        if (self->timer == 0) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_METALDOOR_CLOSE);
            Flags_UnsetSwitch(globalCtx, (u16)self->dyna.actor.params & 0x3F);
            self->actionFunc = BgMizuShutter_Move;
        }
    }
}

void BgMizuShutter_Update(BgMizuShutter* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuShutter* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgMizuShutter_Draw(BgMizuShutter* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuShutter* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_shutter.c", 410);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mizu_shutter.c", 415),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (self->displayList != NULL) {
        gSPDisplayList(POLY_OPA_DISP++, self->displayList);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_shutter.c", 422);
}
