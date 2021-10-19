/*
 * File: z_en_ice_hono.c
 * Overlay: ovl_En_Ice_Hono
 * Description: The various types of Blue Fire
 */

#include "z_en_ice_hono.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000000

#define THIS ((EnIceHono*)thisx)

void EnIceHono_Init(Actor* thisx, GlobalContext* globalCtx);
void EnIceHono_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnIceHono_Update(Actor* thisx, GlobalContext* globalCtx);
void EnIceHono_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnIceHono_CapturableFlame(EnIceHono* self, GlobalContext* globalCtx);
void EnIceHono_DropFlame(EnIceHono* self, GlobalContext* globalCtx);
void EnIceHono_SpreadFlames(EnIceHono* self, GlobalContext* globalCtx);
void EnIceHono_SmallFlameMove(EnIceHono* self, GlobalContext* globalCtx);

void EnIceHono_SetupActionCapturableFlame(EnIceHono* self);
void EnIceHono_SetupActionDroppedFlame(EnIceHono* self);
void EnIceHono_SetupActionSpreadFlames(EnIceHono* self);
void EnIceHono_SetupActionSmallFlame(EnIceHono* self);

const ActorInit En_Ice_Hono_InitVars = {
    ACTOR_EN_ICE_HONO,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnIceHono),
    (ActorFunc)EnIceHono_Init,
    (ActorFunc)EnIceHono_Destroy,
    (ActorFunc)EnIceHono_Update,
    (ActorFunc)EnIceHono_Draw,
};

static ColliderCylinderInit sCylinderInitCapturableFlame = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 25, 80, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInitDroppedFlame = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_OTHER,
        AC_NONE,
        OC1_ON | OC1_TYPE_2,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 12, 60, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChainCapturableFlame[] = {
    ICHAIN_U8(targetMode, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 60, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

static InitChainEntry sInitChainDroppedFlame[] = {
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

static InitChainEntry sInitChainSmallFlame[] = {
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

f32 EnIceHono_XZDistanceSquared(Vec3f* v1, Vec3f* v2) {
    return SQ(v1->x - v2->x) + SQ(v1->z - v2->z);
}

void EnIceHono_InitCapturableFlame(Actor* thisx, GlobalContext* globalCtx) {
    EnIceHono* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChainCapturableFlame);
    Actor_SetScale(&self->actor, 0.0074f);
    self->actor.flags |= 1;
    Actor_SetFocus(&self->actor, 10.0f);

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInitCapturableFlame);
    Collider_UpdateCylinder(&self->actor, &self->collider);

    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    EnIceHono_SetupActionCapturableFlame(self);
}

void EnIceHono_InitDroppedFlame(Actor* thisx, GlobalContext* globalCtx) {
    EnIceHono* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChainDroppedFlame);
    self->actor.scale.x = self->actor.scale.z = self->actor.scale.y = 0.00002f;
    self->actor.gravity = -0.3f;
    self->actor.minVelocityY = -4.0f;
    self->actor.shape.yOffset = 0.0f;
    self->actor.shape.rot.x = self->actor.shape.rot.y = self->actor.shape.rot.z = self->actor.world.rot.x =
        self->actor.world.rot.y = self->actor.world.rot.z = 0;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInitDroppedFlame);
    Collider_UpdateCylinder(&self->actor, &self->collider);

    self->collider.dim.radius = self->actor.scale.x * 4000.4f;
    self->collider.dim.height = self->actor.scale.y * 8000.2f;
    self->actor.colChkInfo.mass = 253;
    EnIceHono_SetupActionDroppedFlame(self);
}

void EnIceHono_InitSmallFlame(Actor* thisx, GlobalContext* globalCtx) {
    EnIceHono* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChainSmallFlame);
    self->actor.scale.x = self->actor.scale.z = self->actor.scale.y = 0.0008f;
    self->actor.gravity = -0.3f;
    self->actor.minVelocityY = -4.0f;
    self->actor.shape.yOffset = 0.0f;

    EnIceHono_SetupActionSmallFlame(self);
}

void EnIceHono_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnIceHono* self = THIS;
    s16 params = self->actor.params;

    switch (self->actor.params) {
        case -1:
            EnIceHono_InitCapturableFlame(&self->actor, globalCtx);
            break;
        case 0:
            EnIceHono_InitDroppedFlame(&self->actor, globalCtx);
            break;
        case 1:
        case 2:
            EnIceHono_InitSmallFlame(&self->actor, globalCtx);
            break;
    }

    if ((self->actor.params == -1) || (self->actor.params == 0)) {
        Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, (s16)self->actor.world.pos.y + 10,
                                  self->actor.world.pos.z, 155, 210, 255, 0);
        self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);
        self->unk_154 = Rand_ZeroOne() * (0x1FFFF / 2.0f);
        self->unk_156 = Rand_ZeroOne() * (0x1FFFF / 2.0f);
        osSyncPrintf("(ice 炎)(arg_data 0x%04x)\n", self->actor.params); // "(ice flame)"
    }
}

void EnIceHono_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnIceHono* self = THIS;

    if ((self->actor.params == -1) || (self->actor.params == 0)) {
        LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

u32 EnIceHono_InBottleRange(EnIceHono* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->actor.xzDistToPlayer < 60.0f) {
        Vec3f tempPos;
        tempPos.x = Math_SinS(self->actor.yawTowardsPlayer + 0x8000) * 40.0f + player->actor.world.pos.x;
        tempPos.y = player->actor.world.pos.y;
        tempPos.z = Math_CosS(self->actor.yawTowardsPlayer + 0x8000) * 40.0f + player->actor.world.pos.z;

        //! @bug: self check is superfluous: it is automatically satisfied if the coarse check is satisfied. It may have
        //! been intended to check the actor is in front of Player, but yawTowardsPlayer does not depend on Player's
        //! world rotation.
        if (EnIceHono_XZDistanceSquared(&tempPos, &self->actor.world.pos) <= SQ(40.0f)) {
            return true;
        }
    }
    return false;
}

void EnIceHono_SetupActionCapturableFlame(EnIceHono* self) {
    self->actionFunc = EnIceHono_CapturableFlame;
    self->alpha = 255;
    self->actor.shape.yOffset = -1000.0f;
}

void EnIceHono_CapturableFlame(EnIceHono* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
    } else if (EnIceHono_InBottleRange(self, globalCtx)) {
        // GI_MAX in self case allows the player to catch the actor in a bottle
        func_8002F434(&self->actor, globalCtx, GI_MAX, 60.0f, 100.0f);
    }

    if (self->actor.xzDistToPlayer < 200.0f) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    func_8002F8F0(&self->actor, NA_SE_EV_FIRE_PILLAR_S - SFX_FLAG);
}

void EnIceHono_SetupActionDroppedFlame(EnIceHono* self) {
    self->actionFunc = EnIceHono_DropFlame;
    self->timer = 200;
    self->alpha = 255;
}

void EnIceHono_DropFlame(EnIceHono* self, GlobalContext* globalCtx) {
    u32 bgFlag = self->actor.bgCheckFlags & 1;

    Math_StepToF(&self->actor.scale.x, 0.0017f, 0.00008f);
    self->actor.scale.z = self->actor.scale.x;
    Math_StepToF(&self->actor.scale.y, 0.0017f, 0.00008f);

    if (bgFlag != 0) {
        s32 i;
        for (i = 0; i < 8; i++) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ICE_HONO, self->actor.world.pos.x,
                        self->actor.world.pos.y, self->actor.world.pos.z, 0,
                        ((s32)(Rand_ZeroOne() * 1000.0f) + i * 0x2000) - 0x1F4, 0, 1);
        }
        EnIceHono_SetupActionSpreadFlames(self);
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, self->actor.scale.x * 3500.0f, 0.0f, 5);

    Collider_UpdateCylinder(&self->actor, &self->collider);
    self->collider.dim.radius = self->actor.scale.x * 4000.0f;
    self->collider.dim.height = self->actor.scale.y * 8000.0f;
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
    }
}

void EnIceHono_SetupActionSpreadFlames(EnIceHono* self) {
    self->actionFunc = EnIceHono_SpreadFlames;
    self->timer = 60;
    self->alpha = 255;
}

void EnIceHono_SpreadFlames(EnIceHono* self, GlobalContext* globalCtx) {
    if (self->timer > 20) {
        Math_StepToF(&self->actor.scale.x, 0.011f, 0.00014f);
        Math_StepToF(&self->actor.scale.y, 0.006f, 0.00012f);
    } else {
        Math_StepToF(&self->actor.scale.x, 0.0001f, 0.00015f);
        Math_StepToF(&self->actor.scale.y, 0.0001f, 0.00015f);
    }
    self->actor.scale.z = self->actor.scale.x;
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, self->actor.scale.x * 3500.0f, 0.0f, 4);
    if (self->timer < 25) {
        self->alpha -= 10;
        self->alpha = CLAMP(self->alpha, 0, 255);
    }

    if ((self->alpha > 100) && (self->timer < 40)) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        self->collider.dim.radius = self->actor.scale.x * 6000.0f;
        self->collider.dim.height = self->actor.scale.y * 8000.0f;
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    if (self->timer == 46) {
        s32 i;
        for (i = 0; i < 10; i++) {
            s32 rot = i * 0x1999;
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ICE_HONO, self->actor.world.pos.x,
                        self->actor.world.pos.y, self->actor.world.pos.z, 0,
                        ((s32)(Rand_ZeroOne() * 1000.0f) + rot) - 0x1F4, 0, 2);
        }
    }

    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
    }
}

void EnIceHono_SetupActionSmallFlame(EnIceHono* self) {
    self->actionFunc = EnIceHono_SmallFlameMove;
    self->timer = 44;
    self->alpha = 255;
    if (self->actor.params == 1) {
        self->smallFlameTargetYScale = (Rand_ZeroOne() * 0.005f) + 0.004f;
        self->actor.speedXZ = (Rand_ZeroOne() * 1.6f) + 0.5f;
    } else {
        self->smallFlameTargetYScale = (Rand_ZeroOne() * 0.005f) + 0.003f;
        self->actor.speedXZ = (Rand_ZeroOne() * 2.0f) + 0.5f;
    }
}

void EnIceHono_SmallFlameMove(EnIceHono* self, GlobalContext* globalCtx) {
    if (self->timer > 20) {
        Math_StepToF(&self->actor.scale.x, 0.006f, 0.00016f);
        Math_StepToF(&self->actor.scale.y, self->smallFlameTargetYScale * 0.667f, 0.00014f);
    } else {
        Math_StepToF(&self->actor.scale.x, 0.0001f, 0.00015f);
        Math_StepToF(&self->actor.scale.y, 0.0001f, 0.00015f);
    }
    self->actor.scale.z = self->actor.scale.x;
    Math_StepToF(&self->actor.speedXZ, 0, 0.06f);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 0.0f, 5);

    if (self->timer < 25) {
        self->alpha -= 10;
        self->alpha = CLAMP(self->alpha, 0, 255);
    }
    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
    }
}

void EnIceHono_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnIceHono* self = THIS;
    s32 pad1;
    f32 intensity;
    s32 pad2;
    f32 sin154;
    f32 sin156;

    if (self->timer > 0) {
        self->timer--;
    }
    if (self->actor.params == 0) {
        func_8002F8F0(&self->actor, NA_SE_IT_FLAME - SFX_FLAG);
    }
    if ((self->actor.params == -1) || (self->actor.params == 0)) {
        self->unk_154 += 0x1111;
        self->unk_156 += 0x4000;
        sin156 = Math_SinS(self->unk_156);
        sin154 = Math_SinS(self->unk_154);
        intensity = (Rand_ZeroOne() * 0.05f) + ((sin154 * 0.125f) + (sin156 * 0.1f)) + 0.425f;
        if ((intensity > 0.7f) || (intensity < 0.2f)) {
            osSyncPrintf("ありえない値(ratio = %f)\n", intensity); // "impossible value(ratio = %f)"
        }
        Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, (s16)self->actor.world.pos.y + 10,
                                  self->actor.world.pos.z, (s32)(155.0f * intensity), (s32)(210.0f * intensity),
                                  (s32)(255.0f * intensity), 1400);
    }

    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
}

void EnIceHono_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnIceHono* self = THIS;
    u32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ice_hono.c", 695);
    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0, (globalCtx->state.frames * -20) % 512,
                                32, 128));

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 170, 255, 255, self->alpha);

    gDPSetEnvColor(POLY_XLU_DISP++, 0, 150, 255, 0);

    Matrix_RotateY((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) - self->actor.shape.rot.y + 0x8000) *
                       (M_PI / 0x8000),
                   MTXMODE_APPLY);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ice_hono.c", 718),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ice_hono.c", 722);
}
