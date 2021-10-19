/*
 * File: z_bg_mizu_movebg.c
 * Overlay: ovl_Bg_Mizu_Movebg
 * Description: Kakariko Village Well Water
 */

#include "z_bg_mizu_movebg.h"
#include "overlays/actors/ovl_Bg_Mizu_Water/z_bg_mizu_water.h"
#include "objects/object_mizu_objects/object_mizu_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgMizuMovebg*)thisx)

#define MOVEBG_TYPE(params) (((u16)(params) >> 0xC) & 0xF)
#define MOVEBG_FLAGS(params) ((u16)(params)&0x3F)
#define MOVEBG_PATH_ID(params) (((u16)(params) >> 0x8) & 0xF)
#define MOVEBG_POINT_ID(params) ((u16)(params)&0xF)
#define MOVEBG_SPEED(params) (((u16)(params) >> 0x4) & 0xF)

void BgMizuMovebg_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMizuMovebg_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMizuMovebg_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMizuMovebg_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8089E318(BgMizuMovebg* self, GlobalContext* globalCtx);
void func_8089E650(BgMizuMovebg* self, GlobalContext* globalCtx);
s32 func_8089E108(Path* pathList, Vec3f* pos, s32 pathId, s32 pointId);

const ActorInit Bg_Mizu_Movebg_InitVars = {
    ACTOR_BG_MIZU_MOVEBG,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuMovebg),
    (ActorFunc)BgMizuMovebg_Init,
    (ActorFunc)BgMizuMovebg_Destroy,
    (ActorFunc)BgMizuMovebg_Update,
    (ActorFunc)BgMizuMovebg_Draw,
};

static f32 D_8089EB40[] = { -115.200005f, -115.200005f, -115.200005f, 0.0f };

static Gfx* D_8089EB50[] = {
    gObjectMizuObjectsMovebgDL_000190, gObjectMizuObjectsMovebgDL_000680, gObjectMizuObjectsMovebgDL_000C20,
    gObjectMizuObjectsMovebgDL_002E10, gObjectMizuObjectsMovebgDL_002E10, gObjectMizuObjectsMovebgDL_002E10,
    gObjectMizuObjectsMovebgDL_002E10, gObjectMizuObjectsMovebgDL_0011F0,
};

static CollisionHeader* D_8089EB70[] = {
    &gObjectMizuObjectsMovebgCol_0003F0, &gObjectMizuObjectsMovebgCol_000998, &gObjectMizuObjectsMovebgCol_000ED0,
    &gObjectMizuObjectsMovebgCol_003590, &gObjectMizuObjectsMovebgCol_003590, &gObjectMizuObjectsMovebgCol_003590,
    &gObjectMizuObjectsMovebgCol_003590, &gObjectMizuObjectsMovebgCol_0015F8,
};

static InitChainEntry D_8089EB90[] = {
    ICHAIN_F32(uncullZoneScale, 1500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static Vec3f D_8089EBA0 = { 0.0f, 80.0f, 23.0f };
static Vec3f D_8089EBAC = { 0.0f, 80.0f, 23.0f };

static u8 D_8089EE40;

s32 func_8089DC30(GlobalContext* globalCtx) {
    s32 result;

    if (Flags_GetSwitch(globalCtx, WATER_TEMPLE_WATER_F1_FLAG)) {
        result = 1;
    } else if (Flags_GetSwitch(globalCtx, WATER_TEMPLE_WATER_F2_FLAG)) {
        result = 2;
    } else if (Flags_GetSwitch(globalCtx, WATER_TEMPLE_WATER_F3_FLAG)) {
        result = 3;
    } else {
        result = 1;
    }
    return result;
}

#ifdef NON_MATCHING
// Codegen OK, Stack Issues
void BgMizuMovebg_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgMizuMovebg* self = THIS;
    WaterBox* waterBoxes = globalCtx->colCtx.colHeader->waterBoxes;
    CollisionHeader* colHeader = NULL;
    Vec3f sp48;

    Actor_ProcessInitChain(&self->dyna.actor, D_8089EB90);
    self->homeY = self->dyna.actor.world.pos.y;
    self->dlist = D_8089EB50[MOVEBG_TYPE(self->dyna.actor.params)];
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(D_8089EB70[MOVEBG_TYPE(self->dyna.actor.params)], &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    switch (MOVEBG_TYPE(self->dyna.actor.params)) {
        case 0:
            if (waterBoxes[2].ySurface + 15.0f < self->homeY - 700.0f) {
                self->dyna.actor.world.pos.y = self->homeY - 700.0f;
            } else {
                self->dyna.actor.world.pos.y = waterBoxes[2].ySurface + 15.0f;
            }
            self->actionFunc = func_8089E318;
            break;
        case 1:
            if (waterBoxes[2].ySurface + 15.0f < self->homeY - 710.0f) {
                self->dyna.actor.world.pos.y = self->homeY - 710.0f;
            } else {
                self->dyna.actor.world.pos.y = waterBoxes[2].ySurface + 15.0f;
            }
            self->actionFunc = func_8089E318;
            break;
        case 2:
            if (waterBoxes[2].ySurface + 15.0f < self->homeY - 700.0f) {
                self->dyna.actor.world.pos.y = self->homeY - 700.0f;
            } else {
                self->dyna.actor.world.pos.y = waterBoxes[2].ySurface + 15.0f;
            }
            self->actionFunc = func_8089E318;
            break;
        case 3:
            self->dyna.actor.world.pos.y = self->homeY + D_8089EB40[func_8089DC30(globalCtx)];
            self->actionFunc = func_8089E318;
            break;
        case 4:
        case 5:
        case 6:
            if (Flags_GetSwitch(globalCtx, MOVEBG_FLAGS(self->dyna.actor.params))) {
                self->dyna.actor.world.pos.y = self->homeY + 115.2;
            } else {
                self->dyna.actor.world.pos.y = self->homeY;
            }
            self->actionFunc = func_8089E318;
            break;
        case 7:
            self->scrollAlpha1 = 160;
            self->scrollAlpha2 = 160;
            self->scrollAlpha3 = 160;
            self->scrollAlpha4 = 160;
            self->waypointId = MOVEBG_POINT_ID(self->dyna.actor.params);
            func_8089E108(globalCtx->setupPathList, &self->dyna.actor.world.pos,
                          MOVEBG_PATH_ID(self->dyna.actor.params), self->waypointId);
            self->actionFunc = func_8089E650;
            break;
    }

    switch (MOVEBG_TYPE(self->dyna.actor.params)) {
        case 3:
        case 4:
        case 5:
        case 6:
            Matrix_RotateY(self->dyna.actor.world.rot.y * (M_PI / 32768), MTXMODE_NEW);
            Matrix_MultVec3f(&D_8089EBA0, &sp48);

            if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_OBJ_HSBLOCK,
                                   self->dyna.actor.world.pos.x + sp48.x, self->dyna.actor.world.pos.y + sp48.y,
                                   self->dyna.actor.world.pos.z + sp48.z, self->dyna.actor.world.rot.x,
                                   self->dyna.actor.world.rot.y, self->dyna.actor.world.rot.z, 2) == NULL) {
                Actor_Kill(&self->dyna.actor);
            }
            break;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Mizu_Movebg/BgMizuMovebg_Init.s")
#endif

void BgMizuMovebg_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgMizuMovebg* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    switch (MOVEBG_TYPE(thisx->params)) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (self->sfxFlags & 2) {
                D_8089EE40 &= ~2;
            }
            break;
        case 7:
            if (self->sfxFlags & 1) {
                D_8089EE40 &= ~1;
            }
            break;
    }
}

s32 func_8089E108(Path* pathList, Vec3f* pos, s32 pathId, s32 pointId) {
    Path* path = pathList;
    Vec3s* point;

    path += pathId;
    point = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[pointId];

    pos->x = point->x;
    pos->y = point->y;
    pos->z = point->z;

    return 0;
}

void func_8089E198(BgMizuMovebg* self, GlobalContext* globalCtx) {
    f32 waterLevel = globalCtx->colCtx.colHeader->waterBoxes[2].ySurface;

    if (waterLevel < WATER_TEMPLE_WATER_F1_Y) {
        self->scrollAlpha1 = 255;
    } else if (waterLevel < WATER_TEMPLE_WATER_F2_Y) {
        self->scrollAlpha1 = 255 - (s32)((waterLevel - WATER_TEMPLE_WATER_F1_Y) /
                                         (WATER_TEMPLE_WATER_F2_Y - WATER_TEMPLE_WATER_F1_Y) * (255 - 160));
    } else {
        self->scrollAlpha1 = 160;
    }

    if (waterLevel < WATER_TEMPLE_WATER_F2_Y) {
        self->scrollAlpha2 = 255;
    } else if (waterLevel < WATER_TEMPLE_WATER_F3_Y) {
        self->scrollAlpha2 = 255 - (s32)((waterLevel - WATER_TEMPLE_WATER_F2_Y) /
                                         (WATER_TEMPLE_WATER_F3_Y - WATER_TEMPLE_WATER_F2_Y) * (255 - 160));
    } else {
        self->scrollAlpha2 = 160;
    }

    if (waterLevel < WATER_TEMPLE_WATER_B1_Y) {
        self->scrollAlpha3 = 255;
    } else if (waterLevel < WATER_TEMPLE_WATER_F1_Y) {
        self->scrollAlpha3 = 255 - (s32)((waterLevel - WATER_TEMPLE_WATER_B1_Y) /
                                         (WATER_TEMPLE_WATER_F1_Y - WATER_TEMPLE_WATER_B1_Y) * (255 - 160));
    } else {
        self->scrollAlpha3 = 160;
    }

    self->scrollAlpha4 = self->scrollAlpha3;
}

void func_8089E318(BgMizuMovebg* self, GlobalContext* globalCtx) {
    WaterBox* waterBoxes = globalCtx->colCtx.colHeader->waterBoxes;
    f32 phi_f0;
    s32 type;
    Vec3f sp28;

    func_8089E198(self, globalCtx);

    type = MOVEBG_TYPE(self->dyna.actor.params);
    switch (type) {
        case 0:
        case 2:
            phi_f0 = waterBoxes[2].ySurface + 15.0f;
            if (phi_f0 < self->homeY - 700.0f) {
                self->dyna.actor.world.pos.y = self->homeY - 700.0f;
            } else {
                self->dyna.actor.world.pos.y = phi_f0;
            }
            break;
        case 1:
            phi_f0 = waterBoxes[2].ySurface + 15.0f;
            if (phi_f0 < self->homeY - 710.0f) {
                self->dyna.actor.world.pos.y = self->homeY - 710.0f;
            } else {
                self->dyna.actor.world.pos.y = phi_f0;
            }
            break;
        case 3:
            phi_f0 = self->homeY + D_8089EB40[func_8089DC30(globalCtx)];
            if (!Math_StepToF(&self->dyna.actor.world.pos.y, phi_f0, 1.0f)) {
                if (!(D_8089EE40 & 2) && MOVEBG_SPEED(self->dyna.actor.params) != 0) {
                    D_8089EE40 |= 2;
                    self->sfxFlags |= 2;
                }
                if (self->sfxFlags & 2) {
                    if (self->dyna.actor.room == 0) {
                        func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
                    } else {
                        func_8002F948(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
                    }
                }
            }
            break;
        case 4:
        case 5:
        case 6:
            if (Flags_GetSwitch(globalCtx, MOVEBG_FLAGS(self->dyna.actor.params))) {
                phi_f0 = self->homeY + 115.200005f;
            } else {
                phi_f0 = self->homeY;
            }
            if (!Math_StepToF(&self->dyna.actor.world.pos.y, phi_f0, 1.0f)) {
                if (!(D_8089EE40 & 2) && MOVEBG_SPEED(self->dyna.actor.params) != 0) {
                    D_8089EE40 |= 2;
                    self->sfxFlags |= 2;
                }
                if (self->sfxFlags & 2) {
                    func_8002F948(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
                }
            }
            break;
    }

    type = MOVEBG_TYPE(self->dyna.actor.params);
    switch (type) {
        case 3:
        case 4:
        case 5:
        case 6:
            if (globalCtx->roomCtx.curRoom.num == self->dyna.actor.room) {
                Matrix_RotateY(self->dyna.actor.world.rot.y * (M_PI / 32768), MTXMODE_NEW);
                Matrix_MultVec3f(&D_8089EBAC, &sp28);
                self->dyna.actor.child->world.pos.x = self->dyna.actor.world.pos.x + sp28.x;
                self->dyna.actor.child->world.pos.y = self->dyna.actor.world.pos.y + sp28.y;
                self->dyna.actor.child->world.pos.z = self->dyna.actor.world.pos.z + sp28.z;
                self->dyna.actor.child->flags &= ~1;
            }
            break;
    }
}

void func_8089E650(BgMizuMovebg* self, GlobalContext* globalCtx) {
    Vec3f waypoint;
    f32 dist;
    f32 dx;
    f32 dy;
    f32 dz;

    self->dyna.actor.speedXZ = MOVEBG_SPEED(self->dyna.actor.params) * 0.1f;
    func_8089E108(globalCtx->setupPathList, &waypoint, MOVEBG_PATH_ID(self->dyna.actor.params), self->waypointId);
    dist = Actor_WorldDistXYZToPoint(&self->dyna.actor, &waypoint);
    if (dist < self->dyna.actor.speedXZ) {
        self->dyna.actor.speedXZ = dist;
    }
    func_80035844(&self->dyna.actor.world.pos, &waypoint, &self->dyna.actor.world.rot, 1);
    func_8002D97C(&self->dyna.actor);
    dx = waypoint.x - self->dyna.actor.world.pos.x;
    dy = waypoint.y - self->dyna.actor.world.pos.y;
    dz = waypoint.z - self->dyna.actor.world.pos.z;
    if (fabsf(dx) < 2.0f && fabsf(dy) < 2.0f && fabsf(dz) < 2.0f) {
        self->waypointId++;
        if (self->waypointId >= globalCtx->setupPathList[MOVEBG_PATH_ID(self->dyna.actor.params)].count) {
            self->waypointId = 0;
            func_8089E108(globalCtx->setupPathList, &self->dyna.actor.world.pos,
                          MOVEBG_PATH_ID(self->dyna.actor.params), 0);
        }
    }
    if (!(D_8089EE40 & 1) && MOVEBG_SPEED(self->dyna.actor.params) != 0) {
        D_8089EE40 |= 1;
        self->sfxFlags |= 1;
    }
    if (self->sfxFlags & 1) {
        func_8002F948(&self->dyna.actor, NA_SE_EV_ROLL_STAND_2 - SFX_FLAG);
    }
}

void BgMizuMovebg_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMizuMovebg* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgMizuMovebg_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    BgMizuMovebg* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    u32 frames;

    if (1) {}

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_movebg.c", 754);

    frames = globalCtx->gameplayFrames;
    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, frames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0, 0,
                                        self->scrollAlpha1));

    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, frames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0, 0,
                                        self->scrollAlpha2));

    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, frames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0, 0,
                                        self->scrollAlpha3));

    gSPSegment(POLY_OPA_DISP++, 0x0B,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, frames * 3, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0, 0,
                                        self->scrollAlpha4));

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mizu_movebg.c", 788),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (self->dlist != NULL) {
        gSPDisplayList(POLY_OPA_DISP++, self->dlist);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_movebg.c", 795);
}
