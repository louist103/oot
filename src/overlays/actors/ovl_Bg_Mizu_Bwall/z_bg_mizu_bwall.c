/*
 * File: z_bg_mizu_bwall.c
 * Overlay: ovl_Bg_Mizu_Bwall
 * Description: Water Temple bombable walls
 */

#include "z_bg_mizu_bwall.h"
#include "overlays/actors/ovl_Bg_Mizu_Water/z_bg_mizu_water.h"
#include "objects/object_mizu_objects/object_mizu_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgMizuBwall*)thisx)

void BgMizuBwall_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMizuBwall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMizuBwall_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMizuBwall_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMizuBwall_Idle(BgMizuBwall* self, GlobalContext* globalCtx);
void BgMizuBwall_Break(BgMizuBwall* self, GlobalContext* globalCtx);
void BgMizuBwall_DoNothing(BgMizuBwall* self, GlobalContext* globalCtx);

const ActorInit Bg_Mizu_Bwall_InitVars = {
    ACTOR_BG_MIZU_BWALL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuBwall),
    (ActorFunc)BgMizuBwall_Init,
    (ActorFunc)BgMizuBwall_Destroy,
    (ActorFunc)BgMizuBwall_Update,
    (ActorFunc)BgMizuBwall_Draw,
};

static ColliderTrisElementInit sTrisElementInitFloor[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000008, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { -40.0f, 0.0f, -40.0f }, { -40.0f, 0.0f, 40.0f }, { 40.0f, 0.0f, 40.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000008, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { -40.0f, 0.0f, -40.0f }, { 40.0f, 0.0f, 40.0f }, { 40.0f, 0.0f, -40.0f } } },
    },
};

static ColliderTrisInit sTrisInitFloor = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    sTrisElementInitFloor,
};

static ColliderTrisElementInit sTrisElementInitRutoWall[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000008, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 0.0f, 116.0f, 0.0f }, { 0.0f, 0.0f, 70.0f }, { 0.0f, 0.0f, -70.0f } } },
    },
};

static ColliderTrisInit sTrisInitRutoWall = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    1,
    sTrisElementInitRutoWall,
};

static ColliderTrisElementInit sTrisElementInitWall[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000008, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 0.0f, 120.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 60.0f, 0.0f, 0.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000008, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 0.0f, 120.0f, 0.0f }, { 60.0f, 0.0f, 0.0f }, { 60.0f, 120.0f, 0.0f } } },
    },
};

static ColliderTrisInit sTrisInitUnusedWall = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    sTrisElementInitWall,
};

static ColliderTrisInit sTrisInitStingerWall = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    sTrisElementInitWall,
};

static Gfx* sDLists[] = {
    gObjectMizuObjectsBwallDL_001A30, gObjectMizuObjectsBwallDL_002390, gObjectMizuObjectsBwallDL_001CD0,
    gObjectMizuObjectsBwallDL_002090, gObjectMizuObjectsBwallDL_001770,
};
static CollisionHeader* sColHeaders[] = {
    &gObjectMizuObjectsBwallCol_001C58, &gObjectMizuObjectsBwallCol_0025A4, &gObjectMizuObjectsBwallCol_001DE8,
    &gObjectMizuObjectsBwallCol_001DE8, &gObjectMizuObjectsBwallCol_001DE8,
};

static InitChainEntry D_8089D854[] = {
    ICHAIN_F32(uncullZoneScale, 1500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgMizuBwall_RotateVec3f(Vec3f* out, Vec3f* in, f32 sin, f32 cos) {
    out->x = (in->z * sin) + (in->x * cos);
    out->y = in->y;
    out->z = (in->z * cos) - (in->x * sin);
}

void BgMizuBwall_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuBwall* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, D_8089D854);
    self->yRot = self->dyna.actor.world.pos.y;
    self->dList = sDLists[(u16)self->dyna.actor.params & 0xF];
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(sColHeaders[(u16)self->dyna.actor.params & 0xF], &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    switch ((u16)self->dyna.actor.params & 0xF) {
        case MIZUBWALL_FLOOR:
            if (Flags_GetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                self->dList = NULL;
                self->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &self->collider);
                if (!Collider_SetTris(globalCtx, &self->collider, &self->dyna.actor, &sTrisInitFloor, self->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 484, self->dyna.actor.params);
                    Actor_Kill(&self->dyna.actor);
                } else {
                    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitFloor); i++) {
                        for (j = 0; j < 3; j++) {
                            offset.x = sTrisInitFloor.elements[i].dim.vtx[j].x;
                            offset.y = sTrisInitFloor.elements[i].dim.vtx[j].y;
                            offset.z = sTrisInitFloor.elements[i].dim.vtx[j].z + 2.0f;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += self->dyna.actor.world.pos.x;
                            vtx[j].y += self->dyna.actor.world.pos.y;
                            vtx[j].z += self->dyna.actor.world.pos.z;
                        }
                        Collider_SetTrisVertices(&self->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    self->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
        case MIZUBWALL_RUTO_ROOM:
            if (Flags_GetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                self->dList = NULL;
                self->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &self->collider);
                if (!Collider_SetTris(globalCtx, &self->collider, &self->dyna.actor, &sTrisInitRutoWall,
                                      self->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 558, self->dyna.actor.params);
                    Actor_Kill(&self->dyna.actor);
                } else {
                    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitRutoWall); i++) {
                        for (j = 0; j < 3; j++) {
                            offset.x = sTrisInitRutoWall.elements[i].dim.vtx[j].x;
                            offset.y = sTrisInitRutoWall.elements[i].dim.vtx[j].y;
                            offset.z = sTrisInitRutoWall.elements[i].dim.vtx[j].z + 2.0f;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += self->dyna.actor.world.pos.x;
                            vtx[j].y += self->dyna.actor.world.pos.y;
                            vtx[j].z += self->dyna.actor.world.pos.z;
                        }
                        Collider_SetTrisVertices(&self->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    self->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
        case MIZUBWALL_UNUSED:
            if (Flags_GetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                self->dList = NULL;
                self->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &self->collider);
                if (!Collider_SetTris(globalCtx, &self->collider, &self->dyna.actor, &sTrisInitUnusedWall,
                                      self->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 638, self->dyna.actor.params);
                    Actor_Kill(&self->dyna.actor);
                } else {
                    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitFloor); i++) {
                        for (j = 0; j < 3; j++) {
                            //! @bug This uses the wrong set of collision triangles, causing the collider to be
                            //!      flat to the ground instead of vertical. It should use sTrisInitUnusedWall.
                            offset.x = sTrisInitFloor.elements[i].dim.vtx[j].x;
                            offset.y = sTrisInitFloor.elements[i].dim.vtx[j].y;
                            offset.z = sTrisInitFloor.elements[i].dim.vtx[j].z;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += self->dyna.actor.world.pos.x;
                            vtx[j].y += self->dyna.actor.world.pos.y;
                            vtx[j].z += self->dyna.actor.world.pos.z;
                        }
                        Collider_SetTrisVertices(&self->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    self->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
        case MIZUBWALL_STINGER_ROOM_1:
            if (Flags_GetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                self->dList = NULL;
                self->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &self->collider);
                if (!Collider_SetTris(globalCtx, &self->collider, &self->dyna.actor, &sTrisInitStingerWall,
                                      self->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 724, self->dyna.actor.params);
                    Actor_Kill(&self->dyna.actor);
                } else {
                    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitFloor); i++) {
                        for (j = 0; j < 3; j++) {
                            //! @bug This uses the wrong set of collision triangles, causing the collider to be
                            //!      flat to the ground instead of vertical. It should use sTrisInitStingerWall.
                            offset.x = sTrisInitFloor.elements[i].dim.vtx[j].x;
                            offset.y = sTrisInitFloor.elements[i].dim.vtx[j].y;
                            offset.z = sTrisInitFloor.elements[i].dim.vtx[j].z + 2.0f;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += self->dyna.actor.world.pos.x;
                            vtx[j].y += self->dyna.actor.world.pos.y;
                            vtx[j].z += self->dyna.actor.world.pos.z;
                        }
                        Collider_SetTrisVertices(&self->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    self->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
        case MIZUBWALL_STINGER_ROOM_2:
            if (Flags_GetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                self->dList = NULL;
                self->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &self->collider);
                if (!Collider_SetTris(globalCtx, &self->collider, &self->dyna.actor, &sTrisInitStingerWall,
                                      self->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 798, self->dyna.actor.params);
                    Actor_Kill(&self->dyna.actor);
                } else {
                    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitFloor); i++) {
                        for (j = 0; j < 3; j++) {
                            //! @bug This uses the wrong set of collision triangles, causing the collider to be
                            //!      flat to the ground instead of vertical. It should use sTrisInitStingerWall.
                            offset.x = sTrisInitFloor.elements[i].dim.vtx[j].x;
                            offset.y = sTrisInitFloor.elements[i].dim.vtx[j].y;
                            offset.z = sTrisInitFloor.elements[i].dim.vtx[j].z + 2.0f;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += self->dyna.actor.world.pos.x;
                            vtx[j].y += self->dyna.actor.world.pos.y;
                            vtx[j].z += self->dyna.actor.world.pos.z;
                        }
                        Collider_SetTrisVertices(&self->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    self->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
    }
}

void BgMizuBwall_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuBwall* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyTris(globalCtx, &self->collider);
}

void BgMizuBwall_SetAlpha(BgMizuBwall* self, GlobalContext* globalCtx) {
    f32 waterLevel = globalCtx->colCtx.colHeader->waterBoxes[2].ySurface;

    if (globalCtx->colCtx.colHeader->waterBoxes) {}

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

void BgMizuBwall_SpawnDebris(BgMizuBwall* self, GlobalContext* globalCtx) {
    s32 i;
    s32 pad;
    s16 rand1;
    s16 rand2;
    Vec3f* thisPos = &self->dyna.actor.world.pos;
    Vec3f debrisPos;
    f32 tempx;
    f32 tempz;
    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);
    Vec3f debrisOffsets[15];

    for (i = 0; i < ARRAY_COUNT(debrisOffsets); i++) {
        switch ((u16)self->dyna.actor.params & 0xF) {
            case MIZUBWALL_FLOOR:
                debrisOffsets[i].x = (Rand_ZeroOne() * 80.0f) - 40.0f;
                debrisOffsets[i].y = Rand_ZeroOne() * 0;
                debrisOffsets[i].z = (Rand_ZeroOne() * 80.0f) - 40.0f;
                break;
            case MIZUBWALL_RUTO_ROOM:
                debrisOffsets[i].x = Rand_ZeroOne() * 0;
                debrisOffsets[i].y = Rand_ZeroOne() * 100.0f;
                debrisOffsets[i].z = (Rand_ZeroOne() * 80.0f) - 40.0f;
                break;
            case MIZUBWALL_UNUSED:
            case MIZUBWALL_STINGER_ROOM_1:
            default:
                debrisOffsets[i].x = (Rand_ZeroOne() * 120) - 60.0f;
                debrisOffsets[i].y = Rand_ZeroOne() * 120;
                debrisOffsets[i].z = Rand_ZeroOne() * 0;
                break;
        }
    }

    for (i = 0; i < ARRAY_COUNT(debrisOffsets); i++) {
        tempx = debrisOffsets[i].x;
        tempz = debrisOffsets[i].z;

        debrisPos.x = thisPos->x + tempz * sin + tempx * cos;
        debrisPos.y = thisPos->y + debrisOffsets[i].y;
        debrisPos.z = thisPos->z + tempz * cos - tempx * sin;

        rand1 = (s16)(Rand_ZeroOne() * 120.0f) + 20;
        rand2 = (s16)(Rand_ZeroOne() * 240.0f) + 20;
        func_80033480(globalCtx, &debrisPos, 50.0f, 2, rand1, rand2, 0);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_A_OBJ, debrisPos.x, debrisPos.y, debrisPos.z, 0, 0, 0,
                    0xB);
    }
}

void BgMizuBwall_Idle(BgMizuBwall* self, GlobalContext* globalCtx) {
    BgMizuBwall_SetAlpha(self, globalCtx);
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        Flags_SetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 8) & 0x3F);
        self->breakTimer = 1;
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        self->dList = NULL;
        BgMizuBwall_SpawnDebris(self, globalCtx);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_WALL_BROKEN);
        Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        self->actionFunc = BgMizuBwall_Break;
    } else if (self->dyna.actor.xzDistToPlayer < 600.0f) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void BgMizuBwall_Break(BgMizuBwall* self, GlobalContext* globalCtx) {
    if (self->breakTimer > 0) {
        self->breakTimer--;
    } else {
        self->actionFunc = BgMizuBwall_DoNothing;
    }
}

void BgMizuBwall_DoNothing(BgMizuBwall* self, GlobalContext* globalCtx) {
}

void BgMizuBwall_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuBwall* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgMizuBwall_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    BgMizuBwall* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    u32 frames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_bwall.c", 1095);
    if (1) {}
    frames = globalCtx->gameplayFrames;

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, 1 * frames, 0, 0x20, 0x20, 1, 0, 0, 0x20, 0x20, 0,
                                        0, 0, self->scrollAlpha1));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, 1 * frames, 0, 0x20, 0x20, 1, 0, 0, 0x20, 0x20, 0,
                                        0, 0, self->scrollAlpha2));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, 1 * frames, 0, 0x20, 0x20, 1, 0, 0, 0x20, 0x20, 0,
                                        0, 0, self->scrollAlpha3));
    gSPSegment(POLY_OPA_DISP++, 0x0B,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, 3 * frames, 0, 0x20, 0x20, 1, 0, 0, 0x20, 0x20, 0,
                                        0, 0, self->scrollAlpha4));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mizu_bwall.c", 1129), 2);

    if (self->dList != NULL) {
        gSPDisplayList(POLY_OPA_DISP++, self->dList);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_bwall.c", 1136);
}
