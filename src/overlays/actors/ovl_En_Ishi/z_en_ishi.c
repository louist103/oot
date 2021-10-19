/*
 * File: z_en_ishi.c
 * Overlay: ovl_En_Ishi
 * Description: Small and large gray rocks
 */

#include "z_en_ishi.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

#include "vt.h"

#define FLAGS 0x00800000

#define THIS ((EnIshi*)thisx)

void EnIshi_Init(Actor* thisx, GlobalContext* globalCtx);
void EnIshi_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnIshi_Update(Actor* thisx, GlobalContext* globalCtx);
void EnIshi_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnIshi_SetupWait(EnIshi* self);
void EnIshi_Wait(EnIshi* self, GlobalContext* globalCtx);
void EnIshi_SetupLiftedUp(EnIshi* self);
void EnIshi_LiftedUp(EnIshi* self, GlobalContext* globalCtx);
void EnIshi_SetupFly(EnIshi* self);
void EnIshi_Fly(EnIshi* self, GlobalContext* globalCtx);
void EnIshi_SpawnFragmentsSmall(EnIshi* self, GlobalContext* globalCtx);
void EnIshi_SpawnFragmentsLarge(EnIshi* self, GlobalContext* globalCtx);
void EnIshi_SpawnDustSmall(EnIshi* self, GlobalContext* globalCtx);
void EnIshi_SpawnDustLarge(EnIshi* self, GlobalContext* globalCtx);

static s16 sRotSpeedX = 0;
static s16 sRotSpeedY = 0;

const ActorInit En_Ishi_InitVars = {
    ACTOR_EN_ISHI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(EnIshi),
    (ActorFunc)EnIshi_Init,
    (ActorFunc)EnIshi_Destroy,
    (ActorFunc)EnIshi_Update,
    (ActorFunc)EnIshi_Draw,
};

static f32 sRockScales[] = { 0.1f, 0.4f };
static f32 D_80A7FA20[] = { 58.0f, 80.0f };
static f32 D_80A7FA28[] = { 0.0f, 0.005f };

// the sizes of these arrays are very large and take up way more space than it needs to.
// coincidentally the sizes are the same as the ID for NA_SE_EV_ROCK_BROKEN, which may explain a mistake that could
// have been made here
static u16 sBreakSounds[0x2852] = { NA_SE_EV_ROCK_BROKEN, NA_SE_EV_WALL_BROKEN };

static u8 sBreakSoundDurations[0x2852] = { 20, 40 };

static EnIshiEffectSpawnFunc sFragmentSpawnFuncs[] = { EnIshi_SpawnFragmentsSmall, EnIshi_SpawnFragmentsLarge };

static EnIshiEffectSpawnFunc sDustSpawnFuncs[] = { EnIshi_SpawnDustSmall, EnIshi_SpawnDustLarge };

static ColliderCylinderInit sCylinderInits[] = {
    {
        {
            COLTYPE_HARD,
            AT_NONE,
            AC_ON | AC_HARD | AC_TYPE_PLAYER,
            OC1_ON | OC1_TYPE_ALL,
            OC2_TYPE_2,
            COLSHAPE_CYLINDER,
        },
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x4FC1FFFE, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 10, 18, -2, { 0, 0, 0 } },
    },
    {
        {
            COLTYPE_HARD,
            AT_NONE,
            AC_ON | AC_HARD | AC_TYPE_PLAYER,
            OC1_ON | OC1_TYPE_ALL,
            OC2_TYPE_2,
            COLSHAPE_CYLINDER,
        },
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x4FC1FFF6, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 55, 70, 0, { 0, 0, 0 } },
    },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE };

void EnIshi_InitCollider(Actor* thisx, GlobalContext* globalCtx) {
    EnIshi* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInits[self->actor.params & 1]);
    Collider_UpdateCylinder(&self->actor, &self->collider);
}

s32 EnIshi_SnapToFloor(EnIshi* self, GlobalContext* globalCtx, f32 arg2) {
    CollisionPoly* poly;
    Vec3f pos;
    s32 bgId;
    f32 floorY;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + 30.0f;
    pos.z = self->actor.world.pos.z;
    floorY = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &poly, &bgId, &self->actor, &pos);
    if (floorY > BGCHECK_Y_MIN) {
        self->actor.world.pos.y = floorY + arg2;
        Math_Vec3f_Copy(&self->actor.home.pos, &self->actor.world.pos);
        return true;
    } else {
        osSyncPrintf(VT_COL(YELLOW, BLACK));
        // "Failure attaching to ground"
        osSyncPrintf("地面に付着失敗(%s %d)\n", "../z_en_ishi.c", 388);
        osSyncPrintf(VT_RST);
        return false;
    }
}

void EnIshi_SpawnFragmentsSmall(EnIshi* self, GlobalContext* globalCtx) {
    static s16 scales[] = { 16, 13, 11, 9, 7, 5 };
    s32 pad;
    Vec3f velocity;
    Vec3f pos;
    s16 phi_v0;
    s32 i;

    for (i = 0; i < ARRAY_COUNT(scales); i++) {
        pos.x = self->actor.world.pos.x + (Rand_ZeroOne() - 0.5f) * 8.0f;
        pos.y = self->actor.world.pos.y + (Rand_ZeroOne() * 5.0f) + 5.0f;
        pos.z = self->actor.world.pos.z + (Rand_ZeroOne() - 0.5f) * 8.0f;
        Math_Vec3f_Copy(&velocity, &self->actor.velocity);
        if (self->actor.bgCheckFlags & 1) {
            velocity.x *= 0.8f;
            velocity.y *= -0.8f;
            velocity.z *= 0.8f;
        } else if (self->actor.bgCheckFlags & 8) {
            velocity.x *= -0.8f;
            velocity.y *= 0.8f;
            velocity.z *= -0.8f;
        }
        velocity.x += (Rand_ZeroOne() - 0.5f) * 11.0f;
        velocity.y += Rand_ZeroOne() * 6.0f;
        velocity.z += (Rand_ZeroOne() - 0.5f) * 11.0f;
        if (Rand_ZeroOne() < 0.5f) {
            phi_v0 = 65;
        } else {
            phi_v0 = 33;
        }
        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &pos, -420, phi_v0, 30, 5, 0, scales[i], 3, 10, 40,
                             KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_FIELD_KEEP, gFieldKakeraDL);
    }
}

void EnIshi_SpawnFragmentsLarge(EnIshi* self, GlobalContext* globalCtx) {
    static s16 scales[] = { 145, 135, 120, 100, 70, 50, 45, 40, 35 };
    Actor* thisx = &self->actor;
    Vec3f velocity;
    Vec3f pos;
    s16 angle = 0x1000;
    s32 i;
    f32 rand;
    s16 phi_v0;
    s16 phi_v1;

    for (i = 0; i < ARRAY_COUNT(scales); i++) {
        angle += 0x4E20;
        rand = Rand_ZeroOne() * 10.0f;
        pos.x = self->actor.world.pos.x + (Math_SinS(angle) * rand);
        pos.y = self->actor.world.pos.y + (Rand_ZeroOne() * 40.0f) + 5.0f;
        pos.z = self->actor.world.pos.z + (Math_CosS(angle) * rand);
        Math_Vec3f_Copy(&velocity, &thisx->velocity);
        if (thisx->bgCheckFlags & 1) {
            velocity.x *= 0.9f;
            velocity.y *= -0.8f;
            velocity.z *= 0.9f;
        } else if (thisx->bgCheckFlags & 8) {
            velocity.x *= -0.9f;
            velocity.y *= 0.8f;
            velocity.z *= -0.9f;
        }
        rand = Rand_ZeroOne() * 10.0f;
        velocity.x += rand * Math_SinS(angle);
        velocity.y += (Rand_ZeroOne() * 4.0f) + ((Rand_ZeroOne() * i) * 0.7f);
        velocity.z += rand * Math_CosS(angle);
        if (i == 0) {
            phi_v0 = 41;
            phi_v1 = -450;
        } else if (i < 4) {
            phi_v0 = 37;
            phi_v1 = -380;
        } else {
            phi_v0 = 69;
            phi_v1 = -320;
        }
        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &self->actor.world.pos, phi_v1, phi_v0, 30, 5, 0, scales[i], 5,
                             2, 70, KAKERA_COLOR_WHITE, OBJECT_GAMEPLAY_FIELD_KEEP, gSilverRockFragmentsDL);
    }
}

void EnIshi_SpawnDustSmall(EnIshi* self, GlobalContext* globalCtx) {
    Vec3f pos;

    Math_Vec3f_Copy(&pos, &self->actor.world.pos);
    if (self->actor.bgCheckFlags & 1) {
        pos.x += 2.0f * self->actor.velocity.x;
        pos.y -= 2.0f * self->actor.velocity.y;
        pos.z += 2.0f * self->actor.velocity.z;
    } else if (self->actor.bgCheckFlags & 8) {
        pos.x -= 2.0f * self->actor.velocity.x;
        pos.y += 2.0f * self->actor.velocity.y;
        pos.z -= 2.0f * self->actor.velocity.z;
    }
    func_80033480(globalCtx, &pos, 60.0f, 3, 0x50, 0x3C, 1);
}

void EnIshi_SpawnDustLarge(EnIshi* self, GlobalContext* globalCtx) {
    Vec3f pos;

    Math_Vec3f_Copy(&pos, &self->actor.world.pos);
    if (self->actor.bgCheckFlags & 1) {
        pos.x += 2.0f * self->actor.velocity.x;
        pos.y -= 2.0f * self->actor.velocity.y;
        pos.z += 2.0f * self->actor.velocity.z;
    } else if (self->actor.bgCheckFlags & 8) {
        pos.x -= 2.0f * self->actor.velocity.x;
        pos.y += 2.0f * self->actor.velocity.y;
        pos.z -= 2.0f * self->actor.velocity.z;
    }
    func_80033480(globalCtx, &pos, 140.0f, 0xA, 0xB4, 0x5A, 1);
}

void EnIshi_DropCollectible(EnIshi* self, GlobalContext* globalCtx) {
    s16 dropParams;

    if ((self->actor.params & 1) == ROCK_SMALL) {
        dropParams = (self->actor.params >> 8) & 0xF;

        if (dropParams >= 0xD) {
            dropParams = 0;
        }

        Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, dropParams << 4);
    }
}

void EnIshi_Fall(EnIshi* self) {
    self->actor.velocity.y += self->actor.gravity;

    if (self->actor.velocity.y < self->actor.minVelocityY) {
        self->actor.velocity.y = self->actor.minVelocityY;
    }
}

void func_80A7ED94(Vec3f* arg0, f32 arg1) {
    arg1 += ((Rand_ZeroOne() * 0.2f) - 0.1f) * arg1;
    arg0->x -= arg0->x * arg1;
    arg0->y -= arg0->y * arg1;
    arg0->z -= arg0->z * arg1;
}

void EnIshi_SpawnBugs(EnIshi* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i < 3; i++) {
        Actor* bug = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_INSECT, self->actor.world.pos.x,
                                 self->actor.world.pos.y, self->actor.world.pos.z, 0, Rand_ZeroOne() * 0xFFFF, 0, 1);

        if (bug == NULL) {
            break;
        }
    }
}

static InitChainEntry sInitChains[][5] = {
    {
        ICHAIN_F32_DIV1000(gravity, -1200, ICHAIN_CONTINUE),
        ICHAIN_F32_DIV1000(minVelocityY, -20000, ICHAIN_CONTINUE),
        ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
        ICHAIN_F32(uncullZoneScale, 150, ICHAIN_CONTINUE),
        ICHAIN_F32(uncullZoneDownward, 400, ICHAIN_STOP),
    },
    {
        ICHAIN_F32_DIV1000(gravity, -2500, ICHAIN_CONTINUE),
        ICHAIN_F32_DIV1000(minVelocityY, -20000, ICHAIN_CONTINUE),
        ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
        ICHAIN_F32(uncullZoneScale, 250, ICHAIN_CONTINUE),
        ICHAIN_F32(uncullZoneDownward, 500, ICHAIN_STOP),
    },
};

void EnIshi_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnIshi* self = THIS;
    s16 type = self->actor.params & 1;

    Actor_ProcessInitChain(&self->actor, sInitChains[type]);
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        self->actor.uncullZoneForward += 1000.0f;
    }
    if (self->actor.shape.rot.y == 0) {
        self->actor.shape.rot.y = self->actor.world.rot.y = Rand_ZeroFloat(0x10000);
    }
    Actor_SetScale(&self->actor, sRockScales[type]);
    EnIshi_InitCollider(&self->actor, globalCtx);
    if ((type == ROCK_LARGE) &&
        Flags_GetSwitch(globalCtx, ((self->actor.params >> 0xA) & 0x3C) | ((self->actor.params >> 6) & 3))) {
        Actor_Kill(&self->actor);
        return;
    }
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    self->actor.shape.yOffset = D_80A7FA20[type];
    if (!((self->actor.params >> 5) & 1) && !EnIshi_SnapToFloor(self, globalCtx, 0.0f)) {
        Actor_Kill(&self->actor);
        return;
    }
    EnIshi_SetupWait(self);
}

void EnIshi_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnIshi* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnIshi_SetupWait(EnIshi* self) {
    self->actionFunc = EnIshi_Wait;
}

void EnIshi_Wait(EnIshi* self, GlobalContext* globalCtx) {
    static u16 liftSounds[] = { NA_SE_PL_PULL_UP_ROCK, NA_SE_PL_PULL_UP_BIGROCK };
    s32 pad;
    s16 type = self->actor.params & 1;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        EnIshi_SetupLiftedUp(self);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, liftSounds[type]);
        if ((self->actor.params >> 4) & 1) {
            EnIshi_SpawnBugs(self, globalCtx);
        }
    } else if ((self->collider.base.acFlags & AC_HIT) && (type == ROCK_SMALL) &&
               self->collider.info.acHitInfo->toucher.dmgFlags & 0x40000048) {
        EnIshi_DropCollectible(self, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, sBreakSoundDurations[type], sBreakSounds[type]);
        sFragmentSpawnFuncs[type](self, globalCtx);
        sDustSpawnFuncs[type](self, globalCtx);
        Actor_Kill(&self->actor);
    } else if (self->actor.xzDistToPlayer < 600.0f) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        self->collider.base.acFlags &= ~AC_HIT;
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        if (self->actor.xzDistToPlayer < 400.0f) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            if (self->actor.xzDistToPlayer < 90.0f) {
                // GI_NONE in these cases allows the player to lift the actor
                if (type == ROCK_LARGE) {
                    func_8002F434(&self->actor, globalCtx, GI_NONE, 80.0f, 20.0f);
                } else {
                    func_8002F434(&self->actor, globalCtx, GI_NONE, 50.0f, 10.0f);
                }
            }
        }
    }
}

void EnIshi_SetupLiftedUp(EnIshi* self) {
    self->actionFunc = EnIshi_LiftedUp;
    self->actor.room = -1;
    self->actor.flags |= 0x10;
}

void EnIshi_LiftedUp(EnIshi* self, GlobalContext* globalCtx) {
    if (Actor_HasNoParent(&self->actor, globalCtx)) {
        self->actor.room = globalCtx->roomCtx.curRoom.num;
        if ((self->actor.params & 1) == ROCK_LARGE) {
            Flags_SetSwitch(globalCtx, ((self->actor.params >> 0xA) & 0x3C) | ((self->actor.params >> 6) & 3));
        }
        EnIshi_SetupFly(self);
        EnIshi_Fall(self);
        func_80A7ED94(&self->actor.velocity, D_80A7FA28[self->actor.params & 1]);
        func_8002D7EC(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 7.5f, 35.0f, 0.0f, 0xC5);
    }
}

void EnIshi_SetupFly(EnIshi* self) {
    self->actor.velocity.x = Math_SinS(self->actor.world.rot.y) * self->actor.speedXZ;
    self->actor.velocity.z = Math_CosS(self->actor.world.rot.y) * self->actor.speedXZ;
    if ((self->actor.params & 1) == ROCK_SMALL) {
        sRotSpeedX = (Rand_ZeroOne() - 0.5f) * 16000.0f;
        sRotSpeedY = (Rand_ZeroOne() - 0.5f) * 2400.0f;
    } else {
        sRotSpeedX = (Rand_ZeroOne() - 0.5f) * 8000.0f;
        sRotSpeedY = (Rand_ZeroOne() - 0.5f) * 1600.0f;
    }
    self->actor.colChkInfo.mass = 240;
    self->actionFunc = EnIshi_Fly;
}

void EnIshi_Fly(EnIshi* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 type = self->actor.params & 1;
    s32 pad2;
    s32 quakeIdx;
    Vec3f contactPos;

    if (self->actor.bgCheckFlags & 9) {
        EnIshi_DropCollectible(self, globalCtx);
        sFragmentSpawnFuncs[type](self, globalCtx);
        if (!(self->actor.bgCheckFlags & 0x20)) {
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, sBreakSoundDurations[type],
                                      sBreakSounds[type]);
            sDustSpawnFuncs[type](self, globalCtx);
        }
        if (type == ROCK_LARGE) {
            quakeIdx = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
            Quake_SetSpeed(quakeIdx, -0x3CB0);
            Quake_SetQuakeValues(quakeIdx, 3, 0, 0, 0);
            Quake_SetCountdown(quakeIdx, 7);
            func_800AA000(self->actor.xyzDistToPlayerSq, 0xFF, 0x14, 0x96);
        }
        Actor_Kill(&self->actor);
        return;
    }
    if (self->actor.bgCheckFlags & 0x40) {
        contactPos.x = self->actor.world.pos.x;
        contactPos.y = self->actor.world.pos.y + self->actor.yDistToWater;
        contactPos.z = self->actor.world.pos.z;
        EffectSsGSplash_Spawn(globalCtx, &contactPos, 0, 0, 0, 350);
        if (type == ROCK_SMALL) {
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 150, 650, 0);
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 400, 800, 4);
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 500, 1100, 8);
        } else {
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 300, 700, 0);
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 500, 900, 4);
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 500, 1300, 8);
        }
        self->actor.minVelocityY = -6.0f;
        sRotSpeedX >>= 2;
        sRotSpeedY >>= 2;
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 40, NA_SE_EV_DIVE_INTO_WATER_L);
        self->actor.bgCheckFlags &= ~0x40;
    }
    Math_StepToF(&self->actor.shape.yOffset, 0.0f, 2.0f);
    EnIshi_Fall(self);
    func_80A7ED94(&self->actor.velocity, D_80A7FA28[type]);
    func_8002D7EC(&self->actor);
    self->actor.shape.rot.x += sRotSpeedX;
    self->actor.shape.rot.y += sRotSpeedY;
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 7.5f, 35.0f, 0.0f, 0xC5);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnIshi_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnIshi* self = THIS;

    self->actionFunc(self, globalCtx);
}

void EnIshi_DrawSmall(EnIshi* self, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gFieldKakeraDL);
}

void EnIshi_DrawLarge(EnIshi* self, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ishi.c", 1050);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ishi.c", 1055),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    gSPDisplayList(POLY_OPA_DISP++, gSilverRockDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ishi.c", 1062);
}

static EnIshiDrawFunc sDrawFuncs[] = { EnIshi_DrawSmall, EnIshi_DrawLarge };

void EnIshi_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnIshi* self = THIS;

    sDrawFuncs[self->actor.params & 1](self, globalCtx);
}
