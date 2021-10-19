/*
 * File: z_obj_hsblock.c
 * Overlay: ovl_Obj_Hsblock
 * Description: Stone Hookshot Target
 */

#include "z_obj_hsblock.h"
#include "objects/object_d_hsblock/object_d_hsblock.h"

#define FLAGS 0x00000000

#define THIS ((ObjHsblock*)thisx)

void ObjHsblock_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjHsblock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjHsblock_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjHsblock_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B93DF4(ObjHsblock* self, GlobalContext* globalCtx);
void func_80B93E5C(ObjHsblock* self, GlobalContext* globalCtx);

void func_80B93D90(ObjHsblock* self);
void func_80B93DB0(ObjHsblock* self);
void func_80B93E38(ObjHsblock* self);

const ActorInit Obj_Hsblock_InitVars = {
    ACTOR_OBJ_HSBLOCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_D_HSBLOCK,
    sizeof(ObjHsblock),
    (ActorFunc)ObjHsblock_Init,
    (ActorFunc)ObjHsblock_Destroy,
    (ActorFunc)ObjHsblock_Update,
    (ActorFunc)ObjHsblock_Draw,
};

static f32 D_80B940C0[] = { 85.0f, 85.0f, 0.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2000, ICHAIN_STOP),
};

static CollisionHeader* sCollisionHeaders[] = { &gHookshotTargetCol, &gHookshotTargetCol, &gHookshotPostCol };

static Color_RGB8 sFireTempleColor = { 165, 125, 55 };

static Gfx* sDLists[] = { gHookshotPostDL, gHookshotPostDL, gHookshotTargetDL };

void ObjHsblock_SetupAction(ObjHsblock* self, ObjHsblockActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void func_80B93B68(ObjHsblock* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 moveFlags) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2[2];

    DynaPolyActor_Init(&self->dyna, moveFlags);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_obj_hsblock.c", 163,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void func_80B93BF0(ObjHsblock* self, GlobalContext* globalCtx) {
    if ((self->dyna.actor.params >> 5) & 1) {
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_OBJ_ICE_POLY,
                           self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z,
                           self->dyna.actor.world.rot.x, self->dyna.actor.world.rot.y, self->dyna.actor.world.rot.z, 1);
    }
}

void ObjHsblock_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjHsblock* self = THIS;

    func_80B93B68(self, globalCtx, sCollisionHeaders[thisx->params & 3], DPM_UNK);
    Actor_ProcessInitChain(thisx, sInitChain);
    func_80B93BF0(self, globalCtx);

    switch (thisx->params & 3) {
        case 0:
        case 2:
            func_80B93D90(self);
            break;
        case 1:
            if (Flags_GetSwitch(globalCtx, (thisx->params >> 8) & 0x3F)) {
                func_80B93D90(self);
            } else {
                func_80B93DB0(self);
            }
    }

    mREG(13) = 255;
    mREG(14) = 255;
    mREG(15) = 255;
}

void ObjHsblock_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ObjHsblock* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_80B93D90(ObjHsblock* self) {
    ObjHsblock_SetupAction(self, NULL);
}

void func_80B93DB0(ObjHsblock* self) {
    self->dyna.actor.flags |= 0x10;
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - 105.0f;
    ObjHsblock_SetupAction(self, func_80B93DF4);
}

void func_80B93DF4(ObjHsblock* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
        func_80B93E38(self);
    }
}

void func_80B93E38(ObjHsblock* self) {
    ObjHsblock_SetupAction(self, func_80B93E5C);
}

void func_80B93E5C(ObjHsblock* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->dyna.actor.velocity.y, 16.0f, 0.1f, 0.8f, 0.0f);
    if (fabsf(Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 0.3f,
                                 self->dyna.actor.velocity.y, 0.3f)) < 0.001f) {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;
        func_80B93D90(self);
        self->dyna.actor.flags &= ~0x10;
    }
}

void ObjHsblock_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjHsblock* self = THIS;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
    Actor_SetFocus(thisx, D_80B940C0[thisx->params & 3]);
}

void ObjHsblock_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Color_RGB8* color;
    Color_RGB8 defaultColor;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_hsblock.c", 365);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_hsblock.c", 369),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (globalCtx->sceneNum == SCENE_HIDAN) {
        color = &sFireTempleColor;
    } else {
        defaultColor.r = mREG(13);
        defaultColor.g = mREG(14);
        defaultColor.b = mREG(15);
        color = &defaultColor;
    }

    gDPSetEnvColor(POLY_OPA_DISP++, color->r, color->g, color->b, 255);
    gSPDisplayList(POLY_OPA_DISP++, sDLists[thisx->params & 3]);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_hsblock.c", 399);
}
