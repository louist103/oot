/*
 * File: z_obj_tsubo.c
 * Overlay: ovl_Obj_Tsubo
 * Description: Breakable pot
 */

#include "z_obj_tsubo.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"

#define FLAGS 0x00800010

#define THIS ((ObjTsubo*)thisx)

void ObjTsubo_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjTsubo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjTsubo_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjTsubo_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjTsubo_SpawnCollectible(ObjTsubo* self, GlobalContext* globalCtx);
void ObjTsubo_ApplyGravity(ObjTsubo* self);
s32 ObjTsubo_SnapToFloor(ObjTsubo* self, GlobalContext* globalCtx);
void ObjTsubo_InitCollider(Actor* thisx, GlobalContext* globalCtx);
void ObjTsubo_AirBreak(ObjTsubo* self, GlobalContext* globalCtx);
void ObjTsubo_WaterBreak(ObjTsubo* self, GlobalContext* globalCtx);
void ObjTsubo_SetupWaitForObject(ObjTsubo* self);
void ObjTsubo_WaitForObject(ObjTsubo* self, GlobalContext* globalCtx);
void ObjTsubo_SetupIdle(ObjTsubo* self);
void ObjTsubo_Idle(ObjTsubo* self, GlobalContext* globalCtx);
void ObjTsubo_SetupLiftedUp(ObjTsubo* self);
void ObjTsubo_LiftedUp(ObjTsubo* self, GlobalContext* globalCtx);
void ObjTsubo_SetupThrown(ObjTsubo* self);
void ObjTsubo_Thrown(ObjTsubo* self, GlobalContext* globalCtx);

static s16 D_80BA1B50 = 0;
static s16 D_80BA1B54 = 0;
static s16 D_80BA1B58 = 0;
static s16 D_80BA1B5C = 0;

const ActorInit Obj_Tsubo_InitVars = {
    ACTOR_OBJ_TSUBO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjTsubo),
    (ActorFunc)ObjTsubo_Init,
    (ActorFunc)ObjTsubo_Destroy,
    (ActorFunc)ObjTsubo_Update,
    NULL,
};

static s16 sObjectIds[] = { OBJECT_GAMEPLAY_DANGEON_KEEP, OBJECT_TSUBO };

static Gfx* D_80BA1B84[] = { gPotDL, 0x060017C0 };

static Gfx* D_80BA1B8C[] = { gPotFragmentDL, 0x06001960 };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HARD,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000002, 0x00, 0x01 },
        { 0x4FC1FFFE, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 9, 26, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit[] = { 0, 12, 60, MASS_IMMOVABLE };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -1200, ICHAIN_CONTINUE), ICHAIN_F32_DIV1000(minVelocityY, -20000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 150, ICHAIN_CONTINUE),   ICHAIN_F32(uncullZoneForward, 900, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),   ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

void ObjTsubo_SpawnCollectible(ObjTsubo* self, GlobalContext* globalCtx) {
    s16 dropParams = self->actor.params & 0x1F;

    if ((dropParams >= ITEM00_RUPEE_GREEN) && (dropParams <= ITEM00_BOMBS_SPECIAL)) {
        Item_DropCollectible(globalCtx, &self->actor.world.pos,
                             (dropParams | (((self->actor.params >> 9) & 0x3F) << 8)));
    }
}

void ObjTsubo_ApplyGravity(ObjTsubo* self) {
    self->actor.velocity.y += self->actor.gravity;
    if (self->actor.velocity.y < self->actor.minVelocityY) {
        self->actor.velocity.y = self->actor.minVelocityY;
    }
}

s32 ObjTsubo_SnapToFloor(ObjTsubo* self, GlobalContext* globalCtx) {
    CollisionPoly* floorPoly;
    Vec3f pos;
    s32 bgID;
    f32 floorY;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + 20.0f;
    pos.z = self->actor.world.pos.z;
    floorY = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &floorPoly, &bgID, &self->actor, &pos);
    if (floorY > BGCHECK_Y_MIN) {
        self->actor.world.pos.y = floorY;
        Math_Vec3f_Copy(&self->actor.home.pos, &self->actor.world.pos);
        return true;
    } else {
        osSyncPrintf("地面に付着失敗\n");
        return false;
    }
}

void ObjTsubo_InitCollider(Actor* thisx, GlobalContext* globalCtx) {
    ObjTsubo* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);
}

void ObjTsubo_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjTsubo* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ObjTsubo_InitCollider(&self->actor, globalCtx);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, sColChkInfoInit);
    if (!ObjTsubo_SnapToFloor(self, globalCtx)) {
        Actor_Kill(&self->actor);
        return;
    }
    self->objTsuboBankIndex = Object_GetIndex(&globalCtx->objectCtx, sObjectIds[(self->actor.params >> 8) & 1]);
    if (self->objTsuboBankIndex < 0) {
        osSyncPrintf("Error : バンク危険！ (arg_data 0x%04x)(%s %d)\n", self->actor.params, "../z_obj_tsubo.c", 410);
        Actor_Kill(&self->actor);
    } else {
        ObjTsubo_SetupWaitForObject(self);
        osSyncPrintf("(dungeon keep 壷)(arg_data 0x%04x)\n", self->actor.params);
    }
}

void ObjTsubo_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    ObjTsubo* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void ObjTsubo_AirBreak(ObjTsubo* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 rand;
    s16 angle;
    Vec3f pos;
    Vec3f velocity;
    f32 sins;
    f32 coss;
    s32 arg5;
    s32 i;

    for (i = 0, angle = 0; i < 15; i++, angle += 0x4E20) {
        sins = Math_SinS(angle);
        coss = Math_CosS(angle);
        pos.x = sins * 8.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = coss * 8.0f;
        velocity.x = pos.x * 0.23f;
        velocity.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        velocity.z = pos.z * 0.23f;
        Math_Vec3f_Sum(&pos, &self->actor.world.pos, &pos);
        rand = Rand_ZeroOne();
        if (rand < 0.2f) {
            arg5 = 96;
        } else if (rand < 0.6f) {
            arg5 = 64;
        } else {
            arg5 = 32;
        }
        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &self->actor.world.pos, -240, arg5, 10, 10, 0,
                             (Rand_ZeroOne() * 95.0f) + 15.0f, 0, 32, 60, KAKERA_COLOR_NONE,
                             sObjectIds[(self->actor.params >> 8) & 1], D_80BA1B8C[(self->actor.params >> 8) & 1]);
    }
    func_80033480(globalCtx, &self->actor.world.pos, 30.0f, 4, 20, 50, 1);
}

void ObjTsubo_WaterBreak(ObjTsubo* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s16 angle;
    Vec3f pos = self->actor.world.pos;
    Vec3f velocity;
    s32 phi_s0;
    s32 i;

    pos.y += self->actor.yDistToWater;
    EffectSsGSplash_Spawn(globalCtx, &pos, NULL, NULL, 0, 400);
    for (i = 0, angle = 0; i < 15; i++, angle += 0x4E20) {
        f32 sins = Math_SinS(angle);
        f32 coss = Math_CosS(angle);

        pos.x = sins * 8.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = coss * 8.0f;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        Math_Vec3f_Sum(&pos, &self->actor.world.pos, &pos);
        phi_s0 = (Rand_ZeroOne() < .2f) ? 64 : 32;
        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &self->actor.world.pos, -180, phi_s0, 30, 30, 0,
                             (Rand_ZeroOne() * 95.0f) + 15.0f, 0, 32, 70, KAKERA_COLOR_NONE,
                             sObjectIds[(self->actor.params >> 8) & 1], D_80BA1B8C[(self->actor.params >> 8) & 1]);
    }
}

void ObjTsubo_SetupWaitForObject(ObjTsubo* self) {
    self->actionFunc = ObjTsubo_WaitForObject;
}

void ObjTsubo_WaitForObject(ObjTsubo* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->objTsuboBankIndex)) {
        self->actor.draw = ObjTsubo_Draw;
        self->actor.objBankIndex = self->objTsuboBankIndex;
        ObjTsubo_SetupIdle(self);
        self->actor.flags &= ~0x10;
    }
}

void ObjTsubo_SetupIdle(ObjTsubo* self) {
    self->actionFunc = ObjTsubo_Idle;
}

void ObjTsubo_Idle(ObjTsubo* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 temp_v0;
    s32 phi_v1;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        ObjTsubo_SetupLiftedUp(self);
    } else if ((self->actor.bgCheckFlags & 0x20) && (self->actor.yDistToWater > 15.0f)) {
        ObjTsubo_WaterBreak(self, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        ObjTsubo_SpawnCollectible(self, globalCtx);
        Actor_Kill(&self->actor);
    } else if ((self->collider.base.acFlags & AC_HIT) &&
               (self->collider.info.acHitInfo->toucher.dmgFlags & 0x4FC1FFFC)) {
        ObjTsubo_AirBreak(self, globalCtx);
        ObjTsubo_SpawnCollectible(self, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&self->actor);
    } else {
        if (self->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&self->actor, &self->collider);
            self->collider.base.acFlags &= ~AC_HIT;
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            if (self->actor.xzDistToPlayer < 150.0f) {
                CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            }
        }
        if (self->actor.xzDistToPlayer < 100.0f) {
            temp_v0 = self->actor.yawTowardsPlayer - GET_PLAYER(globalCtx)->actor.world.rot.y;
            phi_v1 = ABS(temp_v0);
            if (phi_v1 >= 0x5556) {
                // GI_NONE in self case allows the player to lift the actor
                func_8002F434(&self->actor, globalCtx, GI_NONE, 30.0f, 30.0f);
            }
        }
    }
}

void ObjTsubo_SetupLiftedUp(ObjTsubo* self) {
    self->actionFunc = ObjTsubo_LiftedUp;
    self->actor.room = -1;
    func_8002F7DC(&self->actor, NA_SE_PL_PULL_UP_POT);
    self->actor.flags |= 0x10;
}

void ObjTsubo_LiftedUp(ObjTsubo* self, GlobalContext* globalCtx) {
    if (Actor_HasNoParent(&self->actor, globalCtx)) {
        self->actor.room = globalCtx->roomCtx.curRoom.num;
        ObjTsubo_SetupThrown(self);
        ObjTsubo_ApplyGravity(self);
        func_8002D7EC(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 5.0f, 15.0f, 0.0f, 0x85);
    }
}

void ObjTsubo_SetupThrown(ObjTsubo* self) {
    self->actor.velocity.x = Math_SinS(self->actor.world.rot.y) * self->actor.speedXZ;
    self->actor.velocity.z = Math_CosS(self->actor.world.rot.y) * self->actor.speedXZ;
    self->actor.colChkInfo.mass = 240;
    D_80BA1B50 = (Rand_ZeroOne() - 0.7f) * 2800.0f;
    D_80BA1B58 = (Rand_ZeroOne() - 0.5f) * 2000.0f;
    D_80BA1B54 = 0;
    D_80BA1B5C = 0;
    self->actionFunc = ObjTsubo_Thrown;
}

void ObjTsubo_Thrown(ObjTsubo* self, GlobalContext* globalCtx) {
    s32 pad[2];

    if ((self->actor.bgCheckFlags & 0xB) || (self->collider.base.atFlags & AT_HIT)) {
        ObjTsubo_AirBreak(self, globalCtx);
        ObjTsubo_SpawnCollectible(self, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&self->actor);
    } else if (self->actor.bgCheckFlags & 0x40) {
        ObjTsubo_WaterBreak(self, globalCtx);
        ObjTsubo_SpawnCollectible(self, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&self->actor);
    } else {
        ObjTsubo_ApplyGravity(self);
        func_8002D7EC(&self->actor);
        Math_StepToS(&D_80BA1B54, D_80BA1B50, 0x64);
        Math_StepToS(&D_80BA1B5C, D_80BA1B58, 0x64);
        self->actor.shape.rot.x += D_80BA1B54;
        self->actor.shape.rot.y += D_80BA1B5C;
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 5.0f, 15.0f, 0.0f, 0x85);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void ObjTsubo_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjTsubo* self = THIS;

    self->actionFunc(self, globalCtx);
}

void ObjTsubo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, D_80BA1B84[(thisx->params >> 8) & 1]);
}
