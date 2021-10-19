/*
 * File: z_bg_haka_ship.c
 * Overlay: ovl_Bg_Haka_Ship
 * Description: Shadow Temple Ship
 */

#include "z_bg_haka_ship.h"
#include "objects/object_haka_objects/object_haka_objects.h"

#define FLAGS 0x00000030

#define THIS ((BgHakaShip*)thisx)

void BgHakaShip_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaShip_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaShip_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaShip_Draw(Actor* thisx, GlobalContext* globalCtx);
void BgHakaShip_ChildUpdatePosition(BgHakaShip* self, GlobalContext* globalCtx);
void BgHakaShip_WaitForSong(BgHakaShip* self, GlobalContext* globalCtx);
void BgHakaShip_CutsceneStationary(BgHakaShip* self, GlobalContext* globalCtx);
void BgHakaShip_Move(BgHakaShip* self, GlobalContext* globalCtx);
void BgHakaShip_SetupCrash(BgHakaShip* self, GlobalContext* globalCtx);
void BgHakaShip_CrashShake(BgHakaShip* self, GlobalContext* globalCtx);
void BgHakaShip_CrashFall(BgHakaShip* self, GlobalContext* globalCtx);

const ActorInit Bg_Haka_Ship_InitVars = {
    ACTOR_BG_HAKA_SHIP,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKA_OBJECTS,
    sizeof(BgHakaShip),
    (ActorFunc)BgHakaShip_Init,
    (ActorFunc)BgHakaShip_Destroy,
    (ActorFunc)BgHakaShip_Update,
    (ActorFunc)BgHakaShip_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaShip_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaShip* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    self->switchFlag = (thisx->params >> 8) & 0xFF;
    self->dyna.actor.params &= 0xFF;

    if (self->dyna.actor.params == 0) {
        CollisionHeader_GetVirtual(&object_haka_objects_Col_00E408, &colHeader);
        self->counter = 8;
        self->actionFunc = BgHakaShip_WaitForSong;
    } else {
        CollisionHeader_GetVirtual(&object_haka_objects_Col_00ED7C, &colHeader);
        self->actionFunc = BgHakaShip_ChildUpdatePosition;
    }
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    self->dyna.actor.world.rot.y = self->dyna.actor.shape.rot.y - 0x4000;
    self->yOffset = 0;
    if (self->dyna.actor.params == 0 &&
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_BG_HAKA_SHIP,
                           self->dyna.actor.world.pos.x + -10.0f, self->dyna.actor.world.pos.y + 82.0f,
                           self->dyna.actor.world.pos.z, 0, 0, 0, 1) == NULL) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgHakaShip_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaShip* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    func_800F89E8(&self->bellSoundPos);
}

void BgHakaShip_ChildUpdatePosition(BgHakaShip* self, GlobalContext* globalCtx) {
    Actor* parent = self->dyna.actor.parent;

    if (parent != NULL && parent->update != NULL) {
        self->dyna.actor.world.pos.x = parent->world.pos.x + -10.0f;
        self->dyna.actor.world.pos.y = parent->world.pos.y + 82.0f;
        self->dyna.actor.world.pos.z = parent->world.pos.z;
    } else {
        self->dyna.actor.parent = NULL;
    }
}

void BgHakaShip_WaitForSong(BgHakaShip* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
        if (self->counter) {
            self->counter--;
        }
        if (self->counter == 0) {
            self->counter = 130;
            self->actionFunc = BgHakaShip_CutsceneStationary;
            osSyncPrintf("シーン 外輪船 ...  アァクション！！\n");
            OnePointCutscene_Init(globalCtx, 3390, 999, &self->dyna.actor, MAIN_CAM);
        }
    }
}

void BgHakaShip_CutsceneStationary(BgHakaShip* self, GlobalContext* globalCtx) {
    if (self->counter) {
        self->counter--;
    }
    self->yOffset = sinf(self->counter * (M_PI / 25)) * 6144.0f;
    if (self->counter == 0) {
        self->counter = 50;
        self->actionFunc = BgHakaShip_Move;
    }
}

void BgHakaShip_Move(BgHakaShip* self, GlobalContext* globalCtx) {
    f32 distanceFromHome;
    Actor* child;

    if (self->counter) {
        self->counter--;
    }
    if (self->counter == 0) {
        self->counter = 50;
    }
    self->dyna.actor.world.pos.y = (sinf(self->counter * (M_PI / 25)) * 50.0f) + self->dyna.actor.home.pos.y;

    distanceFromHome = self->dyna.actor.home.pos.x - self->dyna.actor.world.pos.x;
    if (distanceFromHome > 7650.0f) {
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x - 7650.0f;
        self->dyna.actor.speedXZ = 0.0f;
    }
    if (distanceFromHome > 7600.0f && !Gameplay_InCsMode(globalCtx)) {
        self->counter = 40;
        self->dyna.actor.speedXZ = 0.0f;
        func_8010B680(globalCtx, 0x5071, NULL);
        self->actionFunc = BgHakaShip_SetupCrash;
    } else {
        Math_StepToF(&self->dyna.actor.speedXZ, 4.0f, 0.2f);
    }
    child = self->dyna.actor.child;
    if (child != NULL && child->update != NULL) {
        child->shape.rot.z += ((655.0f / 13.0f) * self->dyna.actor.speedXZ);
    } else {
        self->dyna.actor.child = NULL;
    }
    self->yOffset = sinf(self->counter * (M_PI / 25)) * 6144.0f;
}

void BgHakaShip_SetupCrash(BgHakaShip* self, GlobalContext* globalCtx) {
    if (self->counter) {
        self->counter--;
    }
    if (self->counter == 0) {
        self->counter = 40;
        self->actionFunc = BgHakaShip_CrashShake;
    }
    Math_ScaledStepToS(&self->yOffset, 0, 128);
}

void BgHakaShip_CrashShake(BgHakaShip* self, GlobalContext* globalCtx) {
    if (self->counter != 0) {
        self->counter--;
    }
    self->dyna.actor.world.pos.y = self->counter % 4 * 3 - 6 + self->dyna.actor.home.pos.y;
    if (!self->counter) {
        self->dyna.actor.gravity = -1.0f;
        self->actionFunc = BgHakaShip_CrashFall;
    }
    func_8002F974(&self->dyna.actor, NA_SE_EV_BLOCKSINK - SFX_FLAG);
}

void BgHakaShip_CrashFall(BgHakaShip* self, GlobalContext* globalCtx) {
    Actor* child;

    if (self->dyna.actor.home.pos.y - self->dyna.actor.world.pos.y > 2000.0f) {
        Actor_Kill(&self->dyna.actor);
        child = self->dyna.actor.child;
        if (child != NULL && child->update != NULL) {
            Actor_Kill(child);
        }
    } else {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCKSINK - SFX_FLAG);
        if ((self->dyna.actor.home.pos.y - self->dyna.actor.world.pos.y > 500.0f) && func_8004356C(&self->dyna)) {
            Gameplay_TriggerVoidOut(globalCtx);
        }
    }
}

void BgHakaShip_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaShip* self = THIS;

    self->actionFunc(self, globalCtx);
    if (self->dyna.actor.params == 0) {
        Actor_MoveForward(&self->dyna.actor);
    }
}

void BgHakaShip_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaShip* self = THIS;
    f32 angleTemp;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_haka_ship.c", 528);
    func_80093D18(globalCtx->state.gfxCtx);
    if (self->dyna.actor.params == 0) {
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_ship.c", 534),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, object_haka_objects_DL_00D330);
        angleTemp = self->yOffset * (M_PI / 0x8000);
        Matrix_Translate(-3670.0f, 620.0f, 1150.0f, MTXMODE_APPLY);
        Matrix_RotateZ(angleTemp, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_ship.c", 547),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, object_haka_objects_DL_005A70);
        Matrix_Translate(0.0f, 0.0f, -2300.0f, MTXMODE_APPLY);
        Matrix_RotateZ(-(2.0f * angleTemp), MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_ship.c", 556),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, object_haka_objects_DL_005A70);
    } else {
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_haka_ship.c", 562),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, object_haka_objects_DL_00E910);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_haka_ship.c", 568);
    if (self->actionFunc == BgHakaShip_CutsceneStationary || self->actionFunc == BgHakaShip_Move) {
        s32 pad;
        Vec3f sp2C;

        sp2C.x = self->dyna.actor.world.pos.x + -367.0f;
        sp2C.y = self->dyna.actor.world.pos.y + 62.0f;
        sp2C.z = self->dyna.actor.world.pos.z;

        SkinMatrix_Vec3fMtxFMultXYZ(&globalCtx->mf_11D60, &sp2C, &self->bellSoundPos);
        func_80078914(&self->bellSoundPos, NA_SE_EV_SHIP_BELL - SFX_FLAG);
    }
}
