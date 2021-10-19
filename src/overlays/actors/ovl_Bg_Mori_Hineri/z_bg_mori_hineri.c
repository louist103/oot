/*
 * File: z_bg_mori_hineri.c
 * Overlay: ovl_Bg_Mori_Hineri
 * Description: Twisting hallway in Forest Temple
 */

#include "z_bg_mori_hineri.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_box/object_box.h"
#include "objects/object_mori_hineri1/object_mori_hineri1.h"
#include "objects/object_mori_hineri1a/object_mori_hineri1a.h"
#include "objects/object_mori_hineri2/object_mori_hineri2.h"
#include "objects/object_mori_hineri2a/object_mori_hineri2a.h"
#include "objects/object_mori_tex/object_mori_tex.h"

#define FLAGS 0x00000030

#define THIS ((BgMoriHineri*)thisx)

void BgMoriHineri_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHineri_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHineri_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHineri_DrawHallAndRoom(Actor* thisx, GlobalContext* globalCtx);

void func_808A39FC(BgMoriHineri* self, GlobalContext* globalCtx);
void func_808A3E54(BgMoriHineri* self, GlobalContext* globalCtx);
void func_808A3C8C(BgMoriHineri* self, GlobalContext* globalCtx);
void BgMoriHineri_SpawnBossKeyChest(BgMoriHineri* self, GlobalContext* globalCtx);
void BgMoriHineri_DoNothing(BgMoriHineri* self, GlobalContext* globalCtx);
void func_808A3D58(BgMoriHineri* self, GlobalContext* globalCtx);

static s16 sNextCamIdx = SUBCAM_NONE;

const ActorInit Bg_Mori_Hineri_InitVars = {
    ACTOR_BG_MORI_HINERI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgMoriHineri),
    (ActorFunc)BgMoriHineri_Init,
    (ActorFunc)BgMoriHineri_Destroy,
    (ActorFunc)BgMoriHineri_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

static Gfx* sDLists[] = {
    0x060024E0,
    0x06001980,
    0x060020F0,
    0x06002B70,
};

void BgMoriHineri_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgMoriHineri* self = THIS;
    s8 moriHineriObjIdx;
    u32 switchFlagParam;
    s32 t6;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);

    switchFlagParam = self->dyna.actor.params & 0x3F;
    t6 = self->dyna.actor.params & 0x4000;

    if (t6 != 0) {
        self->switchFlag = switchFlagParam;
    } else {
        self->switchFlag = ((self->dyna.actor.params >> 8) & 0x3F);
        self->switchFlag = (Flags_GetSwitch(globalCtx, self->switchFlag)) ? 1 : 0;
    }
    self->dyna.actor.params = ((self->dyna.actor.params & 0x8000) >> 0xE);
    if (Flags_GetSwitch(globalCtx, switchFlagParam)) {
        if (self->dyna.actor.params == 0) {
            self->dyna.actor.params = 1;
        } else if (self->dyna.actor.params == 2) {
            self->dyna.actor.params = 3;
        }
    }
    self->boxObjIdx = -1;
    if (self->dyna.actor.params == 0) {
        self->moriHineriObjIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_HINERI1);
        if (t6 == 0) {
            self->boxObjIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_BOX);
        }
    } else {
        if (self->dyna.actor.params == 1) {
            moriHineriObjIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_HINERI1A);
        } else {
            moriHineriObjIdx = (self->dyna.actor.params == 2)
                                   ? Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_HINERI2)
                                   : Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_HINERI2A);
        }
        self->moriHineriObjIdx = moriHineriObjIdx;
    }
    self->moriTexObjIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_TEX);
    if (t6 != 0) {
        self->dyna.actor.params += 4;
    }
    if ((self->moriHineriObjIdx < 0) || (self->moriTexObjIdx < 0)) {
        Actor_Kill(&self->dyna.actor);
    } else {
        self->actionFunc = func_808A39FC;
    }
}

void BgMoriHineri_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgMoriHineri* self = THIS;
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_808A39FC(BgMoriHineri* self, GlobalContext* globalCtx) {
    CollisionHeader* colHeader;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->moriHineriObjIdx) &&
        Object_IsLoaded(&globalCtx->objectCtx, self->moriTexObjIdx) &&
        ((self->boxObjIdx < 0) || Object_IsLoaded(&globalCtx->objectCtx, self->boxObjIdx))) {
        self->dyna.actor.objBankIndex = self->moriHineriObjIdx;
        if (self->dyna.actor.params >= 4) {
            self->dyna.actor.params -= 4;
            if (self->dyna.actor.params == 0) {
                self->moriHineriObjIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_HINERI1A);
            } else if (self->dyna.actor.params == 1) {
                self->moriHineriObjIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_HINERI1);
            } else {
                self->moriHineriObjIdx = (self->dyna.actor.params == 2)
                                             ? Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_HINERI2A)
                                             : Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_HINERI2);
            }
            if (self->moriHineriObjIdx < 0) {
                Actor_Kill(&self->dyna.actor);
            } else {
                self->actionFunc = func_808A3D58;
            }
        } else {
            Actor_SetObjectDependency(globalCtx, &self->dyna.actor);
            colHeader = NULL;
            self->dyna.actor.draw = BgMoriHineri_DrawHallAndRoom;
            if (self->dyna.actor.params == 0) {
                self->actionFunc = func_808A3C8C;
                CollisionHeader_GetVirtual(&object_mori_hineri1_Col_0054B8, &colHeader);
            } else if (self->dyna.actor.params == 1) {
                self->actionFunc = BgMoriHineri_SpawnBossKeyChest;
                CollisionHeader_GetVirtual(&object_mori_hineri1a_Col_003490, &colHeader);
            } else if (self->dyna.actor.params == 2) {
                self->actionFunc = BgMoriHineri_DoNothing;
                CollisionHeader_GetVirtual(&object_mori_hineri2_Col_0043D0, &colHeader);
            } else {
                self->actionFunc = func_808A3C8C;
                CollisionHeader_GetVirtual(&object_mori_hineri2a_Col_006078, &colHeader);
            }
            self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
        }
    }
}

void BgMoriHineri_DoNothing(BgMoriHineri* self, GlobalContext* globalCtx) {
}

void BgMoriHineri_SpawnBossKeyChest(BgMoriHineri* self, GlobalContext* globalCtx) {
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOX, self->dyna.actor.world.pos.x + 147.0f,
                self->dyna.actor.world.pos.y + -245.0f, self->dyna.actor.world.pos.z + -453.0f, 0, 0x4000, 0, 0x27EE);
    self->actionFunc = BgMoriHineri_DoNothing;
}

void func_808A3C8C(BgMoriHineri* self, GlobalContext* globalCtx) {
    f32 f0;
    Player* player = GET_PLAYER(globalCtx);

    f0 = 1100.0f - (player->actor.world.pos.z - self->dyna.actor.world.pos.z);
    self->dyna.actor.shape.rot.z = CLAMP(f0, 0.0f, 1000.0f) * 16.384f;
    Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON1);
    if (self->dyna.actor.params != 0) {
        self->dyna.actor.shape.rot.z = -self->dyna.actor.shape.rot.z;
    }
}

void func_808A3D58(BgMoriHineri* self, GlobalContext* globalCtx) {
    s16 mainCamChildIdx;

    if ((Flags_GetSwitch(globalCtx, self->switchFlag) &&
         (self->dyna.actor.params == 0 || self->dyna.actor.params == 2)) ||
        (!Flags_GetSwitch(globalCtx, self->switchFlag) &&
         (self->dyna.actor.params == 1 || self->dyna.actor.params == 3))) {
        self->dyna.actor.draw = BgMoriHineri_DrawHallAndRoom;
        self->actionFunc = func_808A3E54;

        mainCamChildIdx = globalCtx->cameraPtrs[MAIN_CAM]->childCamIdx;
        if ((mainCamChildIdx != SUBCAM_FREE) && (globalCtx->cameraPtrs[mainCamChildIdx]->setting == CAM_SET_DEMO1)) {
            OnePointCutscene_EndCutscene(globalCtx, mainCamChildIdx);
        }
        OnePointCutscene_Init(globalCtx, 3260, 40, &self->dyna.actor, MAIN_CAM);
        sNextCamIdx = OnePointCutscene_Init(globalCtx, 3261, 40, &self->dyna.actor, MAIN_CAM);
    }
}

void func_808A3E54(BgMoriHineri* self, GlobalContext* globalCtx) {
    s8 objBankIndex;

    if (globalCtx->activeCamera == sNextCamIdx) {
        if (sNextCamIdx != MAIN_CAM) {
            objBankIndex = self->dyna.actor.objBankIndex;
            self->dyna.actor.objBankIndex = self->moriHineriObjIdx;
            self->moriHineriObjIdx = objBankIndex;
            self->dyna.actor.params ^= 1;
            sNextCamIdx = MAIN_CAM;
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
        } else {
            self->dyna.actor.draw = NULL;
            self->actionFunc = func_808A3D58;
            sNextCamIdx = SUBCAM_NONE;
        }
    }
    if ((sNextCamIdx >= SUBCAM_FIRST) &&
        ((GET_ACTIVE_CAM(globalCtx)->eye.z - self->dyna.actor.world.pos.z) < 1100.0f)) {
        func_8002F948(&self->dyna.actor, NA_SE_EV_FLOOR_ROLLING - SFX_FLAG);
    }
}

void BgMoriHineri_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMoriHineri* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgMoriHineri_DrawHallAndRoom(Actor* thisx, GlobalContext* globalCtx) {
    BgMoriHineri* self = THIS;
    s8 objIndex;
    MtxF mtx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_hineri.c", 611);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, globalCtx->objectCtx.status[self->moriTexObjIdx].segment);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_hineri.c", 618),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sDLists[self->dyna.actor.params]);
    if (self->boxObjIdx > 0) {
        Matrix_Get(&mtx);
    }
    if ((self->actionFunc == func_808A3C8C) && (self->dyna.actor.shape.rot.z != 0)) {
        func_80093D18(globalCtx->state.gfxCtx);
        if (self->dyna.actor.params == 0) {
            Matrix_Translate(-1761.0f, 1278.0f, -1821.0f, MTXMODE_NEW);
        } else {
            Matrix_Translate(1999.0f, 1278.0f, -1821.0f, MTXMODE_NEW);
        }
        Matrix_RotateRPY(0, -0x8000, self->dyna.actor.shape.rot.z, MTXMODE_APPLY);
        Matrix_Translate(0.0f, -50.0f, 0.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_hineri.c", 652),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gDungeonDoorDL);
    }
    if ((self->boxObjIdx > 0) && ((self->boxObjIdx = Object_GetIndex(&globalCtx->objectCtx, OBJECT_BOX)) > 0) &&
        (Object_IsLoaded(&globalCtx->objectCtx, self->boxObjIdx))) {
        gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->boxObjIdx].segment);
        gSPSegment(POLY_OPA_DISP++, 0x08, &D_80116280[2]);
        Matrix_Put(&mtx);
        Matrix_Translate(147.0f, -245.0f, -453.0f, MTXMODE_APPLY);
        Matrix_RotateY(M_PI / 2, MTXMODE_APPLY);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_hineri.c", 689),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gTreasureChestBossKeyChestFrontDL);
        Matrix_Put(&mtx);
        Matrix_Translate(167.0f, -218.0f, -453.0f, MTXMODE_APPLY);
        if (Flags_GetTreasure(globalCtx, 0xE)) {
            Matrix_RotateZ(0x3500 * (M_PI / 0x8000), MTXMODE_APPLY);
        } else {
            Matrix_RotateZ(M_PI, MTXMODE_APPLY);
        }
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_hineri.c", 703),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gTreasureChestBossKeyChestSideAndTopDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_hineri.c", 709);
}
