/*
 * File: z_en_wood02.c
 * Overlay: ovl_En_Wood02
 * Description: Trees, bushes, leaves
 */

#include "z_en_wood02.h"
#include "objects/object_wood02/object_wood02.h"

#define FLAGS 0x00000000

#define THIS ((EnWood02*)thisx)

void EnWood02_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWood02_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWood02_Update(Actor* thisx, GlobalContext* globalCtx);
void EnWood02_Draw(Actor* thisx, GlobalContext* globalCtx);

/**
 * WOOD_SPAWN_SPAWNER is also used by some individual trees: EnWood02_Update also checks for parent before running any
 * despawning code.
 *  */
typedef enum {
    /* 0 */ WOOD_SPAWN_NORMAL,
    /* 1 */ WOOD_SPAWN_SPAWNED,
    /* 2 */ WOOD_SPAWN_SPAWNER
} WoodSpawnType;

typedef enum {
    /* 0 */ WOOD_DRAW_TREE_CONICAL,
    /* 1 */ WOOD_DRAW_TREE_OVAL,
    /* 2 */ WOOD_DRAW_TREE_KAKARIKO_ADULT,
    /* 3 */ WOOD_DRAW_BUSH_GREEN,
    /* 4 */ WOOD_DRAW_4, // Used for black bushes and green leaves
    /* 5 */ WOOD_DRAW_LEAF_YELLOW
} WoodDrawType;

const ActorInit En_Wood02_InitVars = {
    ACTOR_EN_WOOD02,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_WOOD02,
    sizeof(EnWood02),
    (ActorFunc)EnWood02_Init,
    (ActorFunc)EnWood02_Destroy,
    (ActorFunc)EnWood02_Update,
    (ActorFunc)EnWood02_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_TREE,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK5,
        { 0x00000000, 0x00, 0x00 },
        { 0x0FC0074A, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 18, 60, 0, { 0, 0, 0 } },
};

static f32 sSpawnDistance[] = { 707.0f, 525.0f, 510.0f, 500.0f, 566.0f, 141.0f };

static s16 sSpawnAngle[] = { 0x1FFF, 0x4C9E, 0x77F5, 0xA5C9, 0xD6C3, 0xA000 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 5600, ICHAIN_STOP),
};

static Gfx* D_80B3BF54[] = {
    object_wood02_DL_0078D0, object_wood02_DL_007CA0, object_wood02_DL_0080D0, object_wood02_DL_000090,
    object_wood02_DL_000340, object_wood02_DL_000340, object_wood02_DL_000700,
};

static Gfx* D_80B3BF70[] = {
    object_wood02_DL_007968,
    object_wood02_DL_007D38,
    object_wood02_DL_0081A8,
    NULL,
    NULL,
    NULL,
    object_wood02_DL_007AD0,
    object_wood02_DL_007E20,
    object_wood02_DL_008350,
    object_wood02_DL_000160,
    object_wood02_DL_000440,
    object_wood02_DL_000700,
};

static f32 sSpawnCos;

static f32 sSpawnSin;

s32 EnWood02_SpawnZoneCheck(EnWood02* self, GlobalContext* globalCtx, Vec3f* pos) {
    f32 phi_f12;

    SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, pos, &self->actor.projectedPos, &self->actor.projectedW);

    phi_f12 = ((self->actor.projectedW == 0.0f) ? 1000.0f : fabsf(1.0f / self->actor.projectedW));

    if ((-self->actor.uncullZoneScale < self->actor.projectedPos.z) &&
        (self->actor.projectedPos.z < (self->actor.uncullZoneForward + self->actor.uncullZoneScale)) &&
        (((fabsf(self->actor.projectedPos.x) - self->actor.uncullZoneScale) * phi_f12) < 1.0f) &&
        (((self->actor.projectedPos.y + self->actor.uncullZoneDownward) * phi_f12) > -1.0f) &&
        (((self->actor.projectedPos.y - self->actor.uncullZoneScale) * phi_f12) < 1.0f)) {
        return true;
    }
    return false;
}

/** Spawns similar-looking trees or bushes only when the player is sufficiently close. Presumably done self way to keep
 * memory usage down in Hyrule Field. */
void EnWood02_SpawnOffspring(EnWood02* self, GlobalContext* globalCtx) {
    EnWood02* childWood;
    s16* childSpawnAngle;
    Vec3f childPos;
    s16 extraRot;
    s16 childParams;
    s32 i;

    for (i = 4; i >= 0; i--) {
        if ((self->unk_14E[i] & 0x7F) == 0) {
            extraRot = 0;
            if (self->actor.params == WOOD_BUSH_GREEN_LARGE_SPAWNER) {
                extraRot = 0x4000;
            }
            childSpawnAngle = &sSpawnAngle[i];
            sSpawnCos = Math_CosS(*childSpawnAngle + self->actor.world.rot.y + extraRot);
            sSpawnSin = Math_SinS(*childSpawnAngle + self->actor.world.rot.y + extraRot);
            childPos.x = (sSpawnDistance[i] * sSpawnSin) + self->actor.home.pos.x;
            childPos.y = self->actor.home.pos.y;
            childPos.z = (sSpawnDistance[i] * sSpawnCos) + self->actor.home.pos.z;
            if (EnWood02_SpawnZoneCheck(self, globalCtx, &childPos)) {
                if ((self->unk_14E[i] & 0x80) != 0) {
                    childParams = (0xFF00 | (self->actor.params + 1));
                } else {
                    childParams = (((self->drawType & 0xF0) << 4) | (self->actor.params + 1));
                }
                childWood = (EnWood02*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx,
                                                          ACTOR_EN_WOOD02, childPos.x, childPos.y, childPos.z,
                                                          self->actor.world.rot.x, *childSpawnAngle, 0, childParams);
                if (childWood != NULL) {
                    childWood->unk_14E[0] = i;
                    self->unk_14E[i] |= 1;
                    childWood->actor.projectedPos = self->actor.projectedPos;
                } else {
                    self->unk_14E[i] &= 0x80;
                }
            }
        }
    }
}

void EnWood02_Init(Actor* thisx, GlobalContext* globalCtx2) {
    s16 spawnType;
    f32 actorScale;
    GlobalContext* globalCtx = globalCtx2;
    EnWood02* self = THIS;
    CollisionPoly* outPoly;
    s32 bgId;
    f32 floorY;
    s16 extraRot;

    spawnType = WOOD_SPAWN_NORMAL;
    actorScale = 1.0f;
    self->unk_14C = (self->actor.params >> 8) & 0xFF;

    if (self->actor.home.rot.z != 0) {
        self->actor.home.rot.z = (self->actor.home.rot.z << 8) | self->unk_14C;
        self->unk_14C = -1;
        self->actor.world.rot.z = self->actor.shape.rot.z = 0;
    } else if (self->unk_14C & 0x80) {
        self->unk_14C = -1;
    }

    self->actor.params &= 0xFF;
    Actor_ProcessInitChain(&self->actor, sInitChain);

    if (self->actor.params <= WOOD_TREE_KAKARIKO_ADULT) {
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    }

    switch (self->actor.params) {
        case WOOD_BUSH_GREEN_LARGE_SPAWNER:
        case WOOD_BUSH_BLACK_LARGE_SPAWNER:
            spawnType = 1;
        case WOOD_BUSH_GREEN_LARGE_SPAWNED:
        case WOOD_BUSH_BLACK_LARGE_SPAWNED:
            spawnType++;
        case WOOD_TREE_CONICAL_LARGE:
        case WOOD_BUSH_GREEN_LARGE:
        case WOOD_BUSH_BLACK_LARGE:
            actorScale = 1.5f;
            self->actor.uncullZoneForward = 4000.0f;
            self->actor.uncullZoneScale = 2000.0f;
            self->actor.uncullZoneDownward = 2400.0f;
            break;
        case WOOD_TREE_CONICAL_SPAWNER:
        case WOOD_TREE_OVAL_YELLOW_SPAWNER:
        case WOOD_TREE_OVAL_GREEN_SPAWNER:
        case WOOD_BUSH_GREEN_SMALL_SPAWNER:
        case WOOD_BUSH_BLACK_SMALL_SPAWNER:
            spawnType = 1;
        case WOOD_TREE_CONICAL_SPAWNED:
        case WOOD_TREE_OVAL_YELLOW_SPAWNED:
        case WOOD_TREE_OVAL_GREEN_SPAWNED:
        case WOOD_BUSH_GREEN_SMALL_SPAWNED:
        case WOOD_BUSH_BLACK_SMALL_SPAWNED:
            spawnType++;
        case WOOD_TREE_CONICAL_MEDIUM:
        case WOOD_TREE_OVAL_GREEN:
        case WOOD_TREE_KAKARIKO_ADULT:
        case WOOD_BUSH_GREEN_SMALL:
        case WOOD_BUSH_BLACK_SMALL:
            self->actor.uncullZoneForward = 4000.0f;
            self->actor.uncullZoneScale = 800.0f;
            self->actor.uncullZoneDownward = 1800.0f;
            break;
        case WOOD_TREE_CONICAL_SMALL:
            actorScale = 0.6f;
            self->actor.uncullZoneForward = 4000.0f;
            self->actor.uncullZoneScale = 400.0f;
            self->actor.uncullZoneDownward = 1000.0f;
            break;
        case WOOD_LEAF_GREEN:
        case WOOD_LEAF_YELLOW:
            self->unk_14E[0] = 0x4B;
            actorScale = 0.02f;
            self->actor.velocity.x = Rand_CenteredFloat(6.0f);
            self->actor.velocity.z = Rand_CenteredFloat(6.0f);
            self->actor.velocity.y = (Rand_ZeroOne() * 1.25f) + -3.1f;
    }

    if (self->actor.params <= WOOD_TREE_CONICAL_SPAWNED) {
        self->drawType = WOOD_DRAW_TREE_CONICAL;
    } else if (self->actor.params <= WOOD_TREE_OVAL_GREEN_SPAWNED) {
        self->drawType = WOOD_DRAW_TREE_OVAL;
    } else if (self->actor.params <= WOOD_TREE_KAKARIKO_ADULT) {
        self->drawType = WOOD_DRAW_TREE_KAKARIKO_ADULT;
    } else if (self->actor.params <= WOOD_BUSH_GREEN_LARGE_SPAWNED) {
        self->drawType = WOOD_DRAW_BUSH_GREEN;
    } else if (self->actor.params <= WOOD_LEAF_GREEN) { // Black bushes and green leaves
        self->drawType = WOOD_DRAW_4;
    } else {
        self->drawType = WOOD_DRAW_LEAF_YELLOW;
    }

    Actor_SetScale(&self->actor, actorScale);
    self->spawnType = spawnType;

    if (spawnType != WOOD_SPAWN_NORMAL) {
        extraRot = 0;

        if (self->actor.params == WOOD_BUSH_GREEN_LARGE_SPAWNER) {
            extraRot = 0x4000;
        }

        if (spawnType == WOOD_SPAWN_SPAWNER) {
            self->drawType |= self->unk_14C << 4;
            EnWood02_SpawnOffspring(self, globalCtx);
            sSpawnCos = Math_CosS(sSpawnAngle[5] + self->actor.world.rot.y + extraRot);
            sSpawnSin = Math_SinS(sSpawnAngle[5] + self->actor.world.rot.y + extraRot);
            self->actor.world.pos.x += (sSpawnSin * sSpawnDistance[5]);
            self->actor.world.pos.z += (sSpawnCos * sSpawnDistance[5]);
        } else {
            self->actor.flags |= 0x10;
        }

        // Snap to floor, or remove if over void
        self->actor.world.pos.y += 200.0f;
        floorY = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &outPoly, &bgId, &self->actor, &self->actor.world.pos);

        if (floorY > BGCHECK_Y_MIN) {
            self->actor.world.pos.y = floorY;
        } else {
            Actor_Kill(&self->actor);
            return;
        }
    }
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    self->actor.home.rot.y = 0;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
}

void EnWood02_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnWood02* self = THIS;

    if (self->actor.params <= WOOD_TREE_KAKARIKO_ADULT) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void EnWood02_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnWood02* self = THIS;
    f32 wobbleAmplitude;
    u8 new_var;
    u8 phi_v0;
    s32 pad;
    Vec3f dropsSpawnPt;
    s32 i;
    s32 leavesParams;

    // Despawn extra trees in a group if out of range
    if ((self->spawnType == WOOD_SPAWN_SPAWNED) && (self->actor.parent != NULL)) {
        if (!(self->actor.flags & 0x40)) {
            new_var = self->unk_14E[0];
            phi_v0 = 0;

            if (self->unk_14C < 0) {
                phi_v0 = 0x80;
            }

            ((EnWood02*)self->actor.parent)->unk_14E[new_var] = phi_v0;
            Actor_Kill(&self->actor);
            return;
        }
    } else if (self->spawnType == WOOD_SPAWN_SPAWNER) {
        EnWood02_SpawnOffspring(self, globalCtx);
    }

    if (self->actor.params <= WOOD_TREE_KAKARIKO_ADULT) {
        if (self->collider.base.acFlags & AC_HIT) {
            self->collider.base.acFlags &= ~AC_HIT;
            Audio_PlayActorSound2(&self->actor, NA_SE_IT_REFLECTION_WOOD);
        }

        if (self->actor.home.rot.y != 0) {
            dropsSpawnPt = self->actor.world.pos;
            dropsSpawnPt.y += 200.0f;

            if ((self->unk_14C >= 0) && (self->unk_14C < 0x64)) {
                Item_DropCollectibleRandom(globalCtx, &self->actor, &dropsSpawnPt, self->unk_14C << 4);
            } else {
                if (self->actor.home.rot.z != 0) {
                    self->actor.home.rot.z &= 0x1FFF;
                    self->actor.home.rot.z |= 0xE000;
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_SW, dropsSpawnPt.x, dropsSpawnPt.y,
                                dropsSpawnPt.z, 0, self->actor.world.rot.y, 0, self->actor.home.rot.z);
                    self->actor.home.rot.z = 0;
                }
            }

            // Spawn falling leaves
            if (self->unk_14C >= -1) {
                leavesParams = WOOD_LEAF_GREEN;

                if ((self->actor.params == WOOD_TREE_OVAL_YELLOW_SPAWNER) ||
                    (self->actor.params == WOOD_TREE_OVAL_YELLOW_SPAWNED)) {
                    leavesParams = WOOD_LEAF_YELLOW;
                }
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_TREE_SWING);

                for (i = 3; i >= 0; i--) {
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_WOOD02, dropsSpawnPt.x, dropsSpawnPt.y,
                                dropsSpawnPt.z, 0, Rand_CenteredFloat(65535.0f), 0, leavesParams);
                }
            }
            self->unk_14C = -0x15;
            self->actor.home.rot.y = 0;
        }

        if (self->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    } else if (self->actor.params < 0x17) { // Bush
        Player* player = GET_PLAYER(globalCtx);

        if (self->unk_14C >= -1) {
            if (((player->rideActor == NULL) && (sqrt(self->actor.xyzDistToPlayerSq) < 20.0) &&
                 (player->linearVelocity != 0.0f)) ||
                ((player->rideActor != NULL) && (sqrt(self->actor.xyzDistToPlayerSq) < 60.0) &&
                 (player->rideActor->speedXZ != 0.0f))) {
                if ((self->unk_14C >= 0) && (self->unk_14C < 0x64)) {
                    Item_DropCollectibleRandom(globalCtx, &self->actor, &self->actor.world.pos,
                                               ((self->unk_14C << 4) | 0x8000));
                }
                self->unk_14C = -0x15;
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_TREE_SWING);
            }
        }
    } else { // Leaves
        self->unk_14C++;
        Math_ApproachF(&self->actor.velocity.x, 0.0f, 1.0f, 5 * 0.01f);
        Math_ApproachF(&self->actor.velocity.z, 0.0f, 1.0f, 5 * 0.01f);
        func_8002D7EC(&self->actor);
        self->actor.shape.rot.z = Math_SinS(3000 * self->unk_14C) * 0x4000;
        self->unk_14E[0]--;

        if (self->unk_14E[0] == 0) {
            Actor_Kill(&self->actor);
        }
    }

    // Wobble from impact
    if (self->unk_14C < -1) {
        self->unk_14C++;
        wobbleAmplitude = Math_SinS((self->unk_14C ^ 0xFFFF) * 0x3332) * 250.0f;
        self->actor.shape.rot.x = (Math_CosS(self->actor.yawTowardsPlayer - self->actor.shape.rot.y) * wobbleAmplitude);
        self->actor.shape.rot.z = (Math_SinS(self->actor.yawTowardsPlayer - self->actor.shape.rot.y) * wobbleAmplitude);
    }
}

void EnWood02_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnWood02* self = THIS;
    s16 type;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    u8 red;
    u8 green;
    u8 blue;

    OPEN_DISPS(gfxCtx, "../z_en_wood02.c", 775);
    type = self->actor.params;

    if ((type == WOOD_TREE_OVAL_GREEN_SPAWNER) || (type == WOOD_TREE_OVAL_GREEN_SPAWNED) ||
        (type == WOOD_TREE_OVAL_GREEN) || (type == WOOD_LEAF_GREEN)) {
        red = 50;
        green = 170;
        blue = 70;
    } else if ((type == WOOD_TREE_OVAL_YELLOW_SPAWNER) || (type == WOOD_TREE_OVAL_YELLOW_SPAWNED) ||
               (type == WOOD_LEAF_YELLOW)) {
        red = 180;
        green = 155;
        blue = 0;
    } else {
        red = green = blue = 255;
    }

    func_80093D84(gfxCtx);

    if ((self->actor.params == WOOD_LEAF_GREEN) || (self->actor.params == WOOD_LEAF_YELLOW)) {
        func_80093D18(gfxCtx);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, red, green, blue, 127);
        Gfx_DrawDListOpa(globalCtx, object_wood02_DL_000700);
    } else if (D_80B3BF70[self->drawType & 0xF] != NULL) {
        Gfx_DrawDListOpa(globalCtx, D_80B3BF54[self->drawType & 0xF]);
        gDPSetEnvColor(POLY_XLU_DISP++, red, green, blue, 0);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_en_wood02.c", 808),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, D_80B3BF70[self->drawType & 0xF]);
    } else {
        func_80093D84(gfxCtx);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_en_wood02.c", 814),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, D_80B3BF54[self->drawType & 0xF]);
    }

    CLOSE_DISPS(gfxCtx, "../z_en_wood02.c", 840);
}
