/*
 * File: z_bg_hidan_sima.c
 * Overlay: ovl_Bg_Hidan_Sima
 * Description: Stone platform (Fire Temple)
 */

#include "z_bg_hidan_sima.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgHidanSima*)thisx)

void BgHidanSima_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanSima_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanSima_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanSima_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8088E518(BgHidanSima* self, GlobalContext* globalCtx);
void func_8088E5D0(BgHidanSima* self, GlobalContext* globalCtx);
void func_8088E6D0(BgHidanSima* self, GlobalContext* globalCtx);
void func_8088E760(BgHidanSima* self, GlobalContext* globalCtx);
void func_8088E7A8(BgHidanSima* self, GlobalContext* globalCtx);
void func_8088E90C(BgHidanSima* self);

const ActorInit Bg_Hidan_Sima_InitVars = {
    ACTOR_BG_HIDAN_SIMA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanSima),
    (ActorFunc)BgHidanSima_Init,
    (ActorFunc)BgHidanSima_Destroy,
    (ActorFunc)BgHidanSima_Update,
    (ActorFunc)BgHidanSima_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x20000000, 0x01, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 1, { { 0, 40, 100 }, 22 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x20000000, 0x01, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NONE,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 1, { { 0, 40, 145 }, 30 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static void* sFireballsTexs[] = {
    gFireTempleFireball0Tex, gFireTempleFireball1Tex, gFireTempleFireball2Tex, gFireTempleFireball3Tex,
    gFireTempleFireball4Tex, gFireTempleFireball5Tex, gFireTempleFireball6Tex, gFireTempleFireball7Tex,
};

void BgHidanSima_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanSima* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 i;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    if (self->dyna.actor.params == 0) {
        CollisionHeader_GetVirtual(&gFireTempleStonePlatform1Col, &colHeader);
    } else {
        CollisionHeader_GetVirtual(&gFireTempleStonePlatform2Col, &colHeader);
    }
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->dyna.actor, &sJntSphInit, self->elements);
    for (i = 0; i < ARRAY_COUNT(sJntSphElementsInit); i++) {
        self->collider.elements[i].dim.worldSphere.radius = self->collider.elements[i].dim.modelSphere.radius;
    }
    if (self->dyna.actor.params == 0) {
        self->actionFunc = func_8088E518;
    } else {
        self->actionFunc = func_8088E760;
    }
}

void BgHidanSima_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanSima* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void func_8088E518(BgHidanSima* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 3.4f);
    if (func_8004356C(&self->dyna) && !(player->stateFlags1 & 0x6000)) {
        self->timer = 20;
        self->dyna.actor.world.rot.y = Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4000;
        if (self->dyna.actor.home.pos.y <= self->dyna.actor.world.pos.y) {
            self->actionFunc = func_8088E5D0;
        } else {
            self->actionFunc = func_8088E6D0;
        }
    }
}

void func_8088E5D0(BgHidanSima* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer != 0) {
        self->dyna.actor.world.pos.x =
            Math_SinS(self->dyna.actor.world.rot.y + (self->timer * 0x4000)) * 5.0f + self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z =
            Math_CosS(self->dyna.actor.world.rot.y + (self->timer * 0x4000)) * 5.0f + self->dyna.actor.home.pos.z;
    } else {
        self->actionFunc = func_8088E6D0;
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
    }
    if (!(self->timer % 4)) {
        func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 180, 10, 100);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_SHAKE);
    }
}

void func_8088E6D0(BgHidanSima* self, GlobalContext* globalCtx) {
    if (func_8004356C(&self->dyna)) {
        self->timer = 20;
    } else if (self->timer != 0) {
        self->timer--;
    }
    Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 100.0f, 1.7f);
    if (self->timer == 0) {
        self->actionFunc = func_8088E518;
    }
}

void func_8088E760(BgHidanSima* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->dyna.actor.world.rot.y += 0x8000;
        self->timer = 60;
        self->actionFunc = func_8088E7A8;
    }
}

void func_8088E7A8(BgHidanSima* self, GlobalContext* globalCtx) {
    f32 temp;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->dyna.actor.world.rot.y != self->dyna.actor.home.rot.y) {
        temp = (sinf(((60 - self->timer) * 0.01667 - 0.5) * M_PI) + 1) * 200;
    } else {
        temp = (sinf((self->timer * 0.01667 - 0.5) * M_PI) + 1) * -200;
    }
    self->dyna.actor.world.pos.x = Math_SinS(self->dyna.actor.world.rot.y) * temp + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = Math_CosS(self->dyna.actor.world.rot.y) * temp + self->dyna.actor.home.pos.z;
    if (self->timer == 0) {
        self->timer = 20;
        self->actionFunc = func_8088E760;
    }
    func_8002F974(&self->dyna.actor, NA_SE_EV_FIRE_PILLAR - SFX_FLAG);
}

void func_8088E90C(BgHidanSima* self) {
    ColliderJntSphElement* elem;
    s32 i;
    f32 cos = Math_CosS(self->dyna.actor.world.rot.y + 0x8000);
    f32 sin = Math_SinS(self->dyna.actor.world.rot.y + 0x8000);

    for (i = 0; i < 2; i++) {
        elem = &self->collider.elements[i];
        elem->dim.worldSphere.center.x = self->dyna.actor.world.pos.x + sin * elem->dim.modelSphere.center.z;
        elem->dim.worldSphere.center.y = (s16)self->dyna.actor.world.pos.y + elem->dim.modelSphere.center.y;
        elem->dim.worldSphere.center.z = self->dyna.actor.world.pos.z + cos * elem->dim.modelSphere.center.z;
    }
}

void BgHidanSima_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanSima* self = THIS;
    s32 pad;

    self->actionFunc(self, globalCtx);
    if (self->dyna.actor.params != 0) {
        s32 temp = (self->dyna.actor.world.rot.y == self->dyna.actor.shape.rot.y) ? self->timer : (self->timer + 80);
        if (self->actionFunc == func_8088E7A8) {
            temp += 20;
        }
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - ((1.0f - cosf(temp * (M_PI / 20))) * 5.0f);
        if (self->actionFunc == func_8088E7A8) {
            func_8088E90C(self);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

Gfx* func_8088EB54(GlobalContext* globalCtx, BgHidanSima* self, Gfx* gfx) {
    MtxF mtxF;
    s32 phi_s5;
    s32 s3;
    s32 v0;
    f32 cos;
    f32 sin;
    s32 pad[2];

    Matrix_MtxFCopy(&mtxF, &gMtxFClear);
    cos = Math_CosS(self->dyna.actor.world.rot.y + 0x8000);
    sin = Math_SinS(self->dyna.actor.world.rot.y + 0x8000);

    phi_s5 = (60 - self->timer) >> 1;
    phi_s5 = CLAMP_MAX(phi_s5, 3);

    v0 = 3 - (self->timer >> 1);
    v0 = CLAMP_MIN(v0, 0);

    mtxF.xw = self->dyna.actor.world.pos.x + ((79 - ((self->timer % 6) * 4)) + v0 * 25) * sin;
    mtxF.zw = self->dyna.actor.world.pos.z + ((79 - ((self->timer % 6) * 4)) + v0 * 25) * cos;
    mtxF.yw = self->dyna.actor.world.pos.y + 40.0f;
    mtxF.zz = v0 * 0.4f + 1.0f;
    mtxF.yy = v0 * 0.4f + 1.0f;
    mtxF.xx = v0 * 0.4f + 1.0f;

    for (s3 = v0; s3 < phi_s5; s3++) {
        mtxF.xw += 25.0f * sin;
        mtxF.zw += 25.0f * cos;
        mtxF.xx += 0.4f;
        mtxF.yy += 0.4f;
        mtxF.zz += 0.4f;

        gSPSegment(gfx++, 0x09, SEGMENTED_TO_VIRTUAL(sFireballsTexs[(self->timer + s3) % 7]));
        gSPMatrix(gfx++,
                  Matrix_MtxFToMtx(Matrix_CheckFloats(&mtxF, "../z_bg_hidan_sima.c", 611),
                                   Graph_Alloc(globalCtx->state.gfxCtx, sizeof(Mtx))),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gfx++, gFireTempleFireballDL);
    }
    mtxF.xw = self->dyna.actor.world.pos.x + (phi_s5 * 25 + 80) * sin;
    mtxF.zw = self->dyna.actor.world.pos.z + (phi_s5 * 25 + 80) * cos;
    gSPSegment(gfx++, 0x09, SEGMENTED_TO_VIRTUAL(sFireballsTexs[(self->timer + s3) % 7]));
    gSPMatrix(gfx++,
              Matrix_MtxFToMtx(Matrix_CheckFloats(&mtxF, "../z_bg_hidan_sima.c", 624),
                               Graph_Alloc(globalCtx->state.gfxCtx, sizeof(Mtx))),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gfx++, gFireTempleFireballDL);
    return gfx;
}

void BgHidanSima_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanSima* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_sima.c", 641);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_hidan_sima.c", 645),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (self->dyna.actor.params == 0) {
        gSPDisplayList(POLY_OPA_DISP++, gFireTempleStonePlatform1DL);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, gFireTempleStonePlatform2DL);
        if (self->actionFunc == func_8088E7A8) {
            POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0x14);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 1, 255, 255, 0, 150);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 255);
            POLY_XLU_DISP = func_8088EB54(globalCtx, self, POLY_XLU_DISP);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_sima.c", 668);
}
