#include "z_en_fire_rock.h"
#include "overlays/actors/ovl_En_Encount2/z_en_encount2.h"
#include "vt.h"
#include "objects/object_efc_star_field/object_efc_star_field.h"

#define FLAGS 0x00000030

#define THIS ((EnFireRock*)thisx)

void EnFireRock_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFireRock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFireRock_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFireRock_Draw(Actor* thisx, GlobalContext* globalCtx);

void FireRock_WaitSpawnRocksFromCeiling(EnFireRock* self, GlobalContext* globalCtx);
void FireRock_WaitOnFloor(EnFireRock* self, GlobalContext* globalCtx);
void EnFireRock_Fall(EnFireRock* self, GlobalContext* globalCtx);
void EnFireRock_SpawnMoreBrokenPieces(EnFireRock* self, GlobalContext* globalCtx);

const ActorInit En_Fire_Rock_InitVars = {
    ACTOR_EN_FIRE_ROCK,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_EFC_STAR_FIELD,
    sizeof(EnFireRock),
    (ActorFunc)EnFireRock_Init,
    (ActorFunc)EnFireRock_Destroy,
    (ActorFunc)EnFireRock_Update,
    (ActorFunc)EnFireRock_Draw,
};

static ColliderCylinderInit D_80A12CA0 = {
    {
        COLTYPE_HARD,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x09, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 30, 30, -10, { 0, 0, 0 } },
};

static ColliderCylinderInit D_80A12CCC = {
    {
        COLTYPE_HARD,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x01, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 30, 30, -10, { 0, 0, 0 } },
};

void EnFireRock_Init(Actor* thisx, GlobalContext* globalCtx) {
    GlobalContext* globalCtx2 = globalCtx;
    Player* player = GET_PLAYER(globalCtx);
    EnFireRock* self = THIS;
    s16 temp;

    self->type = self->actor.params;
    if (self->type != FIRE_ROCK_CEILING_SPOT_SPAWNER) {
        ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 15.0f);
        if (self->type != FIRE_ROCK_ON_FLOOR) {
            self->angularVelocity.x = (Rand_ZeroFloat(10.0f) + 15.0f);
            self->angularVelocity.y = (Rand_ZeroFloat(10.0f) + 15.0f);
            self->angularVelocity.z = (Rand_ZeroFloat(10.0f) + 15.0f);
        }
    }
    switch (self->type) {
        case FIRE_ROCK_CEILING_SPOT_SPAWNER:
            self->actor.draw = NULL;
            // "☆☆☆☆☆ ceiling waiting rock ☆☆☆☆☆"
            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 天井待ち岩 ☆☆☆☆☆ \n" VT_RST);
            self->actionFunc = FireRock_WaitSpawnRocksFromCeiling;
            break;
        case FIRE_ROCK_ON_FLOOR:
            Actor_SetScale(&self->actor, 0.03f);
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &D_80A12CCC);
            // "☆☆☆☆☆ floor rock ☆☆☆☆☆"
            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 床岩 ☆☆☆☆☆ \n" VT_RST);
            self->collider.dim.radius = 23;
            self->collider.dim.height = 37;
            self->collider.dim.yShift = -10;
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
            self->actor.colChkInfo.mass = MASS_IMMOVABLE;
            self->actionFunc = FireRock_WaitOnFloor;
            break;
        case FIRE_ROCK_SPAWNED_FALLING1: // spawned by encount2
            // sets unused vars?
            self->unk17C.x = (f32)(Rand_CenteredFloat(50.0f) + player->actor.world.pos.x);
            self->unk17C.z = (f32)(Rand_CenteredFloat(50.0f) + player->actor.world.pos.z);
        case FIRE_ROCK_SPAWNED_FALLING2: // spawned by encount2 and by the ceilling spawner
            self->scale = (Rand_ZeroFloat(2.0f) / 100.0f) + 0.02f;
            Actor_SetScale(&self->actor, self->scale);
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &D_80A12CA0);
            self->actor.world.rot.y = self->actor.shape.rot.y = Rand_CenteredFloat(65535.0f);
            self->actionFunc = EnFireRock_Fall;
            self->actor.shape.shadowScale = 15.0f;
            break;
        case FIRE_ROCK_BROKEN_PIECE1:
            self->actor.velocity.y = Rand_ZeroFloat(3.0f) + 4.0f;
            self->actor.speedXZ = Rand_ZeroFloat(3.0f) + 3.0f;
            self->scale = (Rand_ZeroFloat(1.0f) / 100.0f) + 0.02f;
            Actor_SetScale(&self->actor, self->scale);
            self->actor.gravity = -1.5f;
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &D_80A12CA0);
            self->actor.shape.shadowScale = 10.0f;
            self->actor.world.rot.y = self->actor.shape.rot.y = Rand_CenteredFloat(65535.0f);
            self->actionFunc = EnFireRock_Fall;
            break;
        case FIRE_ROCK_BROKEN_PIECE2:
            self->actor.velocity.y = Rand_ZeroFloat(3.0f) + 4.0f;
            self->actor.speedXZ = Rand_ZeroFloat(3.0f) + 2.0f;
            self->scale = (Rand_ZeroFloat(1.0f) / 500.0f) + 0.01f;
            Actor_SetScale(&self->actor, self->scale);
            self->actor.gravity = -1.2f;
            self->actor.shape.shadowScale = 5.0f;
            self->actor.world.rot.y = self->actor.shape.rot.y = Rand_CenteredFloat(65535.0f);
            self->actionFunc = EnFireRock_Fall;
            break;
        default:
            // "☆☆☆☆☆ No such rock! ERR !!!!!! ☆☆☆☆☆"
            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ そんな岩はねぇ！ERR!!!!!! ☆☆☆☆☆ \n" VT_RST);
            Actor_Kill(&self->actor);
            break;
    }
}

void EnFireRock_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFireRock* self = THIS;

    if ((self->actor.parent != NULL) && (self->actor.parent == &self->spawner->actor)) {
        EnEncount2* spawner = (EnEncount2*)self->actor.parent;
        if ((spawner->actor.update != NULL) && (spawner->numSpawnedRocks > 0)) {
            spawner->numSpawnedRocks--;
            osSyncPrintf("\n\n");
            // "☆☆☆☆☆ Number of spawned instances recovery ☆☆☆☆☆%d"
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生数回復 ☆☆☆☆☆%d\n" VT_RST, spawner->numSpawnedRocks);
            osSyncPrintf("\n\n");
        }
    }
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnFireRock_Fall(EnFireRock* self, GlobalContext* globalCtx) {
    Player* player;
    Vec3f flamePos;
    s32 i;

    player = GET_PLAYER(globalCtx);
    if ((self->actor.floorHeight == -10000.0f) || (self->actor.world.pos.y < (player->actor.world.pos.y - 200.0f))) {
        Actor_Kill(&self->actor);
        return;
    }
    switch (self->type) {
        case FIRE_ROCK_SPAWNED_FALLING1:
            if (player->actor.world.pos.y < self->actor.world.pos.y) {
                if ((player->actor.world.pos.x > -700.0f) || (player->actor.world.pos.x < 100.0f) ||
                    (player->actor.world.pos.z > -1290.0f) || (player->actor.world.pos.z < -3880.0f)) {
                    Math_ApproachF(&self->actor.world.pos.x, player->actor.world.pos.x, 1.0f, 10.0f);
                    Math_ApproachF(&self->actor.world.pos.z, player->actor.world.pos.z, 1.0f, 10.0f);
                }
            }
        case FIRE_ROCK_SPAWNED_FALLING2:
            flamePos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
            flamePos.y = Rand_CenteredFloat(20.0f) + self->actor.world.pos.y;
            flamePos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
            EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &flamePos, 100, 0, 0, -1);
            break;
        case FIRE_ROCK_BROKEN_PIECE1:
            if ((globalCtx->gameplayFrames & 3) == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_VALVAISA_ROCK);
            }
            break;
    }
    if ((self->actor.bgCheckFlags & 1) && (self->timer == 0)) {
        switch (self->type) {
            case FIRE_ROCK_SPAWNED_FALLING1:
            case FIRE_ROCK_SPAWNED_FALLING2:
                func_80033E88(&self->actor, globalCtx, 5, 2);
            case FIRE_ROCK_BROKEN_PIECE1:
                Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, self->actor.shape.shadowScale,
                                         1, 8.0f, 500, 10, 0);
                for (i = 0; i < 5; i++) {
                    flamePos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
                    flamePos.y = self->actor.floorHeight;
                    flamePos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
                    EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &flamePos, 300, 0, 0, -1);
                }
                self->actionFunc = EnFireRock_SpawnMoreBrokenPieces;
                break;
            default:
                Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, self->actor.shape.shadowScale,
                                         3, 8.0f, 200, 10, 0);
                Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 40, NA_SE_EV_EXPLOSION);
                Actor_Kill(&self->actor);
                break;
        }
    }
}

/**
 * After the rock has already hit the ground and started rolling, spawn two more, giving the illusion of breaking into
 * two pieces.
 */
void EnFireRock_SpawnMoreBrokenPieces(EnFireRock* self, GlobalContext* globalCtx) {
    EnFireRock* spawnedFireRock;
    s32 nextRockType;
    s32 i;
    s32 temp;

    nextRockType = FIRE_ROCK_SPAWNED_FALLING1;
    switch (self->type) {
        case FIRE_ROCK_SPAWNED_FALLING1:
        case FIRE_ROCK_SPAWNED_FALLING2:
            nextRockType = FIRE_ROCK_BROKEN_PIECE1;
            break;
        case FIRE_ROCK_BROKEN_PIECE1:
            nextRockType = FIRE_ROCK_BROKEN_PIECE2;
    }

    if (nextRockType != FIRE_ROCK_SPAWNED_FALLING1) {
        for (i = 0; i < 2; i++) {
            spawnedFireRock = (EnFireRock*)Actor_Spawn(
                &globalCtx->actorCtx, globalCtx, ACTOR_EN_FIRE_ROCK, Rand_CenteredFloat(3.0f) + self->actor.world.pos.x,
                Rand_CenteredFloat(3.0f) + (self->actor.world.pos.y + 10.0f),
                Rand_CenteredFloat(3.0f) + self->actor.world.pos.z, 0, 0, 0, nextRockType);
            if (spawnedFireRock != NULL) {
                spawnedFireRock->actor.world.rot.y = self->actor.world.rot.y;
                if (i == 0) {
                    spawnedFireRock->actor.shape.rot.y = self->actor.shape.rot.y;
                }
                spawnedFireRock->scale = self->scale - 0.01f;
            } else {
                osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ イッパイデッス ☆☆☆☆☆ \n" VT_RST);
            }
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_VALVAISA_ROCK);
    }
    Actor_Kill(&self->actor);
}

void FireRock_WaitSpawnRocksFromCeiling(EnFireRock* self, GlobalContext* globalCtx) {
    EnFireRock* spawnedFireRock;

    if (self->actor.xzDistToPlayer < 200.0f) {
        if ((self->playerNearby == 0) && (self->timer2 == 0)) {
            self->timer2 = 30;
            spawnedFireRock = (EnFireRock*)Actor_Spawn(
                &globalCtx->actorCtx, globalCtx, ACTOR_EN_FIRE_ROCK, Rand_CenteredFloat(3.0f) + self->actor.world.pos.x,
                self->actor.world.pos.y + 10.0f, Rand_CenteredFloat(3.0f) + self->actor.world.pos.z, 0, 0, 0,
                FIRE_ROCK_SPAWNED_FALLING2);
            if (spawnedFireRock != NULL) {
                spawnedFireRock->timer = 10;
            } else {
                osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ イッパイデッス ☆☆☆☆☆ \n" VT_RST);
            }
        }
        self->playerNearby = 1;
    } else {
        self->playerNearby = 0;
    }
    if (BREG(0) != 0) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, 0, 255, 0, 255, 4, globalCtx->state.gfxCtx);
    }
}

void FireRock_WaitOnFloor(EnFireRock* self, GlobalContext* globalCtx) {
    Vec3f flamePos;
    s16 scale;

    if (self->timer2 == 0) {
        flamePos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
        flamePos.y = Rand_CenteredFloat(20.0f) + self->actor.world.pos.y;
        flamePos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
        scale = 130 + (s16)Rand_CenteredFloat(60.0f);
        self->timer2 = 3 + (s16)Rand_ZeroFloat(3.0f);
        EffectSsEnFire_SpawnVec3f(globalCtx, &self->actor, &flamePos, scale, 0, 0, -1);
    }
}

void EnFireRock_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnFireRock* self = THIS;
    s16 setCollision;
    Player* player = GET_PLAYER(globalCtx);
    Actor* playerActor = &GET_PLAYER(globalCtx)->actor;

    if (self->timer2 != 0) {
        self->timer2--;
    }
    if (self->timer != 0) {
        self->timer--;
    }
    self->actionFunc(self, globalCtx);

    if (self->type != FIRE_ROCK_CEILING_SPOT_SPAWNER) {
        f32 temp;
        self->rockRotation.x += self->angularVelocity.x;
        self->rockRotation.y += self->angularVelocity.y;
        self->rockRotation.z += self->angularVelocity.z;
        self->relativePos.y = 3.0f;

        temp = 10.0f + (self->scale * 300.0f);
        thisx->shape.shadowScale = temp;
        if (thisx->shape.shadowScale < 10.0f) {
            thisx->shape.shadowScale = 10.0f;
        }
        if (thisx->shape.shadowScale > 20.0f) {
            thisx->shape.shadowScale = 20.0f;
        }

        if ((self->type == FIRE_ROCK_SPAWNED_FALLING1) || (self->type == FIRE_ROCK_SPAWNED_FALLING2)) {
            thisx->gravity = -0.3f - (self->scale * 7.0f);
        }
        if (self->type != FIRE_ROCK_ON_FLOOR) {
            Actor_MoveForward(thisx);
            Actor_UpdateBgCheckInfo(globalCtx, thisx, 50.0f, 50.0f, 100.0f, 0x1C);
        }

        setCollision = false;
        if (self->actionFunc != EnFireRock_SpawnMoreBrokenPieces) {
            if ((self->type == FIRE_ROCK_SPAWNED_FALLING1) || (self->type == FIRE_ROCK_SPAWNED_FALLING2) ||
                (self->type == FIRE_ROCK_BROKEN_PIECE1)) {
                if (self->collider.base.atFlags & 4) {
                    self->collider.base.atFlags &= ~4;
                    Audio_PlayActorSound2(thisx, NA_SE_EV_BRIDGE_OPEN_STOP);
                    thisx->velocity.y = 0.0f;
                    thisx->speedXZ = 0.0f;
                    self->actionFunc = EnFireRock_SpawnMoreBrokenPieces;
                    // "☆☆☆☆☆ Shield Defense Lv1 ☆☆☆☆☆"
                    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ シールド防御 Lv１ ☆☆☆☆☆ \n" VT_RST);
                    return;
                }
                setCollision = true;
            }
        }

        if (self->type == FIRE_ROCK_ON_FLOOR) {
            if (self->collider.base.atFlags & 2) {
                self->collider.base.atFlags &= ~2;
                if (self->collider.base.at == playerActor) {
                    if (!(player->stateFlags1 & 0x04000000)) {
                        func_8002F758(globalCtx, thisx, 2.0f, -player->actor.world.rot.y, 3.0f, 4);
                    }
                    return;
                }
            }
            setCollision = true;
        }
        if (setCollision) {
            Collider_UpdateCylinder(thisx, &self->collider);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

void EnFireRock_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnFireRock* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fire_rock.c", 747);
    Matrix_Translate(thisx->world.pos.x + self->relativePos.x, thisx->world.pos.y + self->relativePos.y,
                     thisx->world.pos.z + self->relativePos.z, MTXMODE_NEW);
    Matrix_RotateX(DEG_TO_RAD(self->rockRotation.x), MTXMODE_APPLY);
    Matrix_RotateY(DEG_TO_RAD(self->rockRotation.y), MTXMODE_APPLY);
    Matrix_RotateZ(DEG_TO_RAD(self->rockRotation.z), MTXMODE_APPLY);
    Matrix_Scale(thisx->scale.x, thisx->scale.y, thisx->scale.z, MTXMODE_APPLY);
    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 155, 55, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, 155, 255, 55, 255);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_fire_rock.c", 768),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_efc_star_field_DL_000DE0);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fire_rock.c", 773);
}
