/*
 * File: z_bg_mori_rakkatenjo.c
 * Overlay: ovl_Bg_Mori_Rakkatenjo
 * Description: Falling ceiling in Forest Temple
 */

#include "z_bg_mori_rakkatenjo.h"
#include "objects/object_mori_objects/object_mori_objects.h"

#define FLAGS 0x00000030

#define THIS ((BgMoriRakkatenjo*)thisx)

void BgMoriRakkatenjo_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMoriRakkatenjo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMoriRakkatenjo_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMoriRakkatenjo_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMoriRakkatenjo_SetupWaitForMoriTex(BgMoriRakkatenjo* self);
void BgMoriRakkatenjo_WaitForMoriTex(BgMoriRakkatenjo* self, GlobalContext* globalCtx);
void BgMoriRakkatenjo_SetupWait(BgMoriRakkatenjo* self);
void BgMoriRakkatenjo_Wait(BgMoriRakkatenjo* self, GlobalContext* globalCtx);
void BgMoriRakkatenjo_SetupFall(BgMoriRakkatenjo* self);
void BgMoriRakkatenjo_Fall(BgMoriRakkatenjo* self, GlobalContext* globalCtx);
void BgMoriRakkatenjo_SetupRest(BgMoriRakkatenjo* self);
void BgMoriRakkatenjo_Rest(BgMoriRakkatenjo* self, GlobalContext* globalCtx);
void BgMoriRakkatenjo_SetupRise(BgMoriRakkatenjo* self);
void BgMoriRakkatenjo_Rise(BgMoriRakkatenjo* self, GlobalContext* globalCtx);

static s16 sCamSetting = 0;

const ActorInit Bg_Mori_Rakkatenjo_InitVars = {
    ACTOR_BG_MORI_RAKKATENJO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriRakkatenjo),
    (ActorFunc)BgMoriRakkatenjo_Init,
    (ActorFunc)BgMoriRakkatenjo_Destroy,
    (ActorFunc)BgMoriRakkatenjo_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(gravity, -1, ICHAIN_CONTINUE),
    ICHAIN_F32(minVelocityY, -11, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgMoriRakkatenjo_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriRakkatenjo* self = THIS;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    // "Forest Temple obj. Falling Ceiling"
    osSyncPrintf("森の神殿 obj. 落下天井 (home posY %f)\n", self->dyna.actor.home.pos.y);
    if ((fabsf(1991.0f - self->dyna.actor.home.pos.x) > 0.001f) ||
        (fabsf(683.0f - self->dyna.actor.home.pos.y) > 0.001f) ||
        (fabsf(-2520.0f - self->dyna.actor.home.pos.z) > 0.001f)) {
        // "The set position has been changed. Let's fix the program."
        osSyncPrintf("Warning : セット位置が変更されています。プログラムを修正しましょう。\n");
    }
    if (self->dyna.actor.home.rot.y != 0x8000) {
        // "The set Angle has changed. Let's fix the program."
        osSyncPrintf("Warning : セット Angle が変更されています。プログラムを修正しましょう。\n");
    }
    self->moriTexObjIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjIndex < 0) {
        // "Forest Temple obj Falling Ceiling Bank Danger!"
        osSyncPrintf("Error : 森の神殿 obj 落下天井 バンク危険！(%s %d)\n", "../z_bg_mori_rakkatenjo.c", 205);
        Actor_Kill(&self->dyna.actor);
        return;
    }
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    CollisionHeader_GetVirtual(&gMoriRakkatenjoCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    BgMoriRakkatenjo_SetupWaitForMoriTex(self);
    sCamSetting = 0;
}

void BgMoriRakkatenjo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriRakkatenjo* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

s32 BgMoriRakkatenjo_IsLinkUnder(BgMoriRakkatenjo* self, GlobalContext* globalCtx) {
    Vec3f* pos = &GET_PLAYER(globalCtx)->actor.world.pos;

    return (-3300.0f < pos->z) && (pos->z < -1840.0f) && (1791.0f < pos->x) && (pos->x < 2191.0f);
}

s32 BgMoriRakkatenjo_IsLinkClose(BgMoriRakkatenjo* self, GlobalContext* globalCtx) {
    Vec3f* pos = &GET_PLAYER(globalCtx)->actor.world.pos;

    return (-3360.0f < pos->z) && (pos->z < -1840.0f) && (1791.0f < pos->x) && (pos->x < 2191.0f);
}

void BgMoriRakkatenjo_SetupWaitForMoriTex(BgMoriRakkatenjo* self) {
    self->actionFunc = BgMoriRakkatenjo_WaitForMoriTex;
}

void BgMoriRakkatenjo_WaitForMoriTex(BgMoriRakkatenjo* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->moriTexObjIndex)) {
        BgMoriRakkatenjo_SetupWait(self);
        self->dyna.actor.draw = BgMoriRakkatenjo_Draw;
    }
}

void BgMoriRakkatenjo_SetupWait(BgMoriRakkatenjo* self) {
    self->timer = (self->fallCount > 0) ? 100 : 21;
    self->dyna.actor.world.pos.y = 683.0f;
    self->actionFunc = BgMoriRakkatenjo_Wait;
}

void BgMoriRakkatenjo_Wait(BgMoriRakkatenjo* self, GlobalContext* globalCtx) {
    if (self->fallCount == 0) {
        if (BgMoriRakkatenjo_IsLinkClose(self, globalCtx) || (self->timer < 20)) {
            if (self->timer <= 0) {
                BgMoriRakkatenjo_SetupFall(self);
            }
        } else {
            self->timer = 21;
        }
    } else {
        if (BgMoriRakkatenjo_IsLinkUnder(self, globalCtx) || (self->timer < 20)) {
            if (self->timer <= 0) {
                BgMoriRakkatenjo_SetupFall(self);
            }
        } else {
            self->timer = 100;
        }
    }
    if (self->timer < 20) {
        func_800788CC(NA_SE_EV_BLOCKSINK - SFX_FLAG);
    }
}

void BgMoriRakkatenjo_SetupFall(BgMoriRakkatenjo* self) {
    self->actionFunc = BgMoriRakkatenjo_Fall;
    self->bounceCount = 0;
    self->dyna.actor.velocity.y = 0.0f;
}

void BgMoriRakkatenjo_Fall(BgMoriRakkatenjo* self, GlobalContext* globalCtx) {
    static f32 bounceVel[] = { 4.0f, 1.5f, 0.4f, 0.1f };
    s32 pad;
    Actor* thisx = &self->dyna.actor;
    s32 quake;

    Actor_MoveForward(thisx);
    if ((thisx->velocity.y < 0.0f) && (thisx->world.pos.y <= 403.0f)) {
        if (self->bounceCount >= ARRAY_COUNT(bounceVel)) {
            BgMoriRakkatenjo_SetupRest(self);
        } else {
            if (self->bounceCount == 0) {
                self->fallCount++;
                func_800788CC(NA_SE_EV_STONE_BOUND);
                func_800AA000(SQ(thisx->yDistToPlayer), 0xFF, 0x14, 0x96);
            }
            thisx->world.pos.y =
                403.0f - (thisx->world.pos.y - 403.0f) * bounceVel[self->bounceCount] / fabsf(thisx->velocity.y);
            thisx->velocity.y = bounceVel[self->bounceCount];
            self->bounceCount++;
            quake = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
            Quake_SetSpeed(quake, 50000);
            Quake_SetQuakeValues(quake, 5, 0, 0, 0);
            Quake_SetCountdown(quake, 5);
        }
    }
}

void BgMoriRakkatenjo_SetupRest(BgMoriRakkatenjo* self) {
    self->actionFunc = BgMoriRakkatenjo_Rest;
    self->dyna.actor.world.pos.y = 403.0f;
    self->timer = 20;
}

void BgMoriRakkatenjo_Rest(BgMoriRakkatenjo* self, GlobalContext* globalCtx) {
    if (self->timer <= 0) {
        BgMoriRakkatenjo_SetupRise(self);
    }
}

void BgMoriRakkatenjo_SetupRise(BgMoriRakkatenjo* self) {
    self->actionFunc = BgMoriRakkatenjo_Rise;
    self->dyna.actor.velocity.y = -0.1f;
}

void BgMoriRakkatenjo_Rise(BgMoriRakkatenjo* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->dyna.actor.velocity.y, 5.0f, 0.06f, 0.1f, 0.0f);
    self->dyna.actor.world.pos.y += self->dyna.actor.velocity.y;
    if (self->dyna.actor.world.pos.y >= 683.0f) {
        BgMoriRakkatenjo_SetupWait(self);
    }
}

void BgMoriRakkatenjo_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriRakkatenjo* self = THIS;

    if (self->timer > 0) {
        self->timer--;
    }
    self->actionFunc(self, globalCtx);
    if (BgMoriRakkatenjo_IsLinkUnder(self, globalCtx)) {
        if (sCamSetting == CAM_SET_NONE) {
            osSyncPrintf("camera changed (mori rakka tenjyo) ... \n");
            sCamSetting = globalCtx->cameraPtrs[MAIN_CAM]->setting;
            Camera_SetCameraData(globalCtx->cameraPtrs[MAIN_CAM], 1, &self->dyna.actor, NULL, 0, 0, 0);
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_MORI1);
        }
    } else if (sCamSetting != CAM_SET_NONE) {
        osSyncPrintf("camera changed (previous) ... \n");
        Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON1);
        sCamSetting = 0;
    }
}

void BgMoriRakkatenjo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriRakkatenjo* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_rakkatenjo.c", 497);
    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, globalCtx->objectCtx.status[self->moriTexObjIndex].segment);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_rakkatenjo.c", 502),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, gMoriRakkatenjoDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_rakkatenjo.c", 506);
}
