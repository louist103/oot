/*
 * File: z_en_kusa.c
 * Overlay: ovl_en_kusa
 * Description: Bush
 */

#include "z_en_kusa.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"
#include "vt.h"

#define FLAGS 0x00800010

#define THIS ((EnKusa*)thisx)

void EnKusa_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKusa_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKusa_Update(Actor* thisx, GlobalContext* globalCtx);
void EnKusa_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnKusa_SetupLiftedUp(EnKusa* self);
void func_80A9B7EC(EnKusa* self);
void func_80A9B89C(EnKusa* self);
void func_80A9BBB0(EnKusa* self);
void func_80A9BEAC(EnKusa* self);
void func_80A9BF3C(EnKusa* self);
void func_80A9C00C(EnKusa* self);

void func_80A9BC1C(EnKusa* self, GlobalContext* globalCtx);
void func_80A9B810(EnKusa* self, GlobalContext* globalCtx);
void func_80A9B8D8(EnKusa* self, GlobalContext* globalCtx);
void EnKusa_LiftedUp(EnKusa* self, GlobalContext* globalCtx);
void func_80A9BEFC(EnKusa* self, GlobalContext* globalCtx);
void func_80A9BF30(EnKusa* self, GlobalContext* globalCtx);
void func_80A9BFA8(EnKusa* self, GlobalContext* globalCtx);
void func_80A9C068(EnKusa* self, GlobalContext* globalCtx);

static s16 D_80A9C1D0 = 0;
static s16 D_80A9C1D4 = 0;
static s16 D_80A9C1D8 = 0;
static s16 D_80A9C1DC = 0;

const ActorInit En_Kusa_InitVars = {
    ACTOR_EN_KUSA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnKusa),
    (ActorFunc)EnKusa_Init,
    (ActorFunc)EnKusa_Destroy,
    (ActorFunc)EnKusa_Update,
    NULL,
};

static s16 sObjectIds[] = { OBJECT_GAMEPLAY_FIELD_KEEP, OBJECT_KUSA, OBJECT_KUSA };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER | OC1_TYPE_2,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x4FC00758, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 12, 44, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 30, MASS_IMMOVABLE };

static Vec3f D_80A9C23C[] = {
    { 0.0f, 0.7071f, 0.7071f },
    { 0.7071f, 0.7071f, 0.0f },
    { 0.0f, 0.7071f, -0.7071f },
    { -0.7071f, 0.7071f, 0.0f },
};

s16 D_80A9C26C[] = { 108, 102, 96, 84, 66, 55, 42, 38 };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 400, ICHAIN_CONTINUE),         ICHAIN_F32_DIV1000(gravity, -3200, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(minVelocityY, -17000, ICHAIN_CONTINUE), ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),         ICHAIN_F32(uncullZoneDownward, 120, ICHAIN_STOP),
};

extern Gfx D_060002E0[];

void EnKusa_SetupAction(EnKusa* self, EnKusaActionFunc actionFunc) {
    self->timer = 0;
    self->actionFunc = actionFunc;
}

s32 EnKusa_SnapToFloor(EnKusa* self, GlobalContext* globalCtx, f32 yOffset) {
    s32 pad;
    CollisionPoly* poly;
    Vec3f pos;
    s32 bgId;
    f32 floorY;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + 30.0f;
    pos.z = self->actor.world.pos.z;

    floorY = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &poly, &bgId, &self->actor, &pos);

    if (floorY > BGCHECK_Y_MIN) {
        self->actor.world.pos.y = floorY + yOffset;
        Math_Vec3f_Copy(&self->actor.home.pos, &self->actor.world.pos);
        return true;
    } else {
        osSyncPrintf(VT_COL(YELLOW, BLACK));
        // "Failure attaching to ground"
        osSyncPrintf("地面に付着失敗(%s %d)\n", "../z_en_kusa.c", 323);
        osSyncPrintf(VT_RST);
        return false;
    }
}

void EnKusa_DropCollectible(EnKusa* self, GlobalContext* globalCtx) {
    s16 dropParams;

    switch (self->actor.params & 3) {
        case 0:
        case 2:
            dropParams = (self->actor.params >> 8) & 0xF;

            if (dropParams >= 0xD) {
                dropParams = 0;
            }
            Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, dropParams << 4);
            break;
        case 1:
            if (Rand_ZeroOne() < 0.5f) {
                Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_SEEDS);
            } else {
                Item_DropCollectible(globalCtx, &self->actor.world.pos, ITEM00_HEART);
            }
            break;
    }
}

void EnKusa_Fall(EnKusa* self) {
    self->actor.velocity.y += self->actor.gravity;

    if (self->actor.velocity.y < self->actor.minVelocityY) {
        self->actor.velocity.y = self->actor.minVelocityY;
    }
}

void func_80A9B174(Vec3f* vec, f32 arg1) {
    arg1 += ((Rand_ZeroOne() * 0.2f) - 0.1f) * arg1;
    vec->x -= vec->x * arg1;
    vec->y -= vec->y * arg1;
    vec->z -= vec->z * arg1;
}

void EnKusa_SetScale(EnKusa* self) {
    self->actor.scale.y = 0.16000001f;
    self->actor.scale.x = 0.120000005f;
    self->actor.scale.z = 0.120000005f;
}

void EnKusa_SpawnFragments(EnKusa* self, GlobalContext* globalCtx) {
    Vec3f velocity;
    Vec3f pos;
    s32 i;
    s32 index;
    Vec3f* scale;
    s32 pad;

    for (i = 0; i < ARRAY_COUNT(D_80A9C23C); i++) {
        scale = &D_80A9C23C[i];

        pos.x = self->actor.world.pos.x + (scale->x * self->actor.scale.x * 20.0f);
        pos.y = self->actor.world.pos.y + (scale->y * self->actor.scale.y * 20.0f) + 10.0f;
        pos.z = self->actor.world.pos.z + (scale->z * self->actor.scale.z * 20.0f);

        velocity.x = (Rand_ZeroOne() - 0.5f) * 8.0f;
        velocity.y = Rand_ZeroOne() * 10.0f;
        velocity.z = (Rand_ZeroOne() - 0.5f) * 8.0f;

        index = (s32)(Rand_ZeroOne() * 111.1f) & 7;

        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &pos, -100, 64, 40, 3, 0, D_80A9C26C[index], 0, 0, 80,
                             KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_KEEP, gCuttableShrubStalkDL);

        pos.x = self->actor.world.pos.x + (scale->x * self->actor.scale.x * 40.0f);
        pos.y = self->actor.world.pos.y + (scale->y * self->actor.scale.y * 40.0f) + 10.0f;
        pos.z = self->actor.world.pos.z + (scale->z * self->actor.scale.z * 40.0f);

        velocity.x = (Rand_ZeroOne() - 0.5f) * 6.0f;
        velocity.y = Rand_ZeroOne() * 10.0f;
        velocity.z = (Rand_ZeroOne() - 0.5f) * 6.0f;

        index = (s32)(Rand_ZeroOne() * 111.1f) % 7;

        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &pos, -100, 64, 40, 3, 0, D_80A9C26C[index], 0, 0, 80,
                             KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_KEEP, gCuttableShrubTipDL);
    }
}

void EnKusa_SpawnBugs(EnKusa* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i < 3; i++) {
        Actor* bug = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_INSECT, self->actor.world.pos.x,
                                 self->actor.world.pos.y, self->actor.world.pos.z, 0, Rand_ZeroOne() * 0xFFFF, 0, 1);

        if (bug == NULL) {
            break;
        }
    }
}

void EnKusa_InitCollider(Actor* thisx, GlobalContext* globalCtx) {
    EnKusa* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    Collider_UpdateCylinder(&self->actor, &self->collider);
}

void EnKusa_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnKusa* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        self->actor.uncullZoneForward += 1000.0f;
    }

    EnKusa_InitCollider(thisx, globalCtx);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);

    if (self->actor.shape.rot.y == 0) {
        s16 rand = Rand_ZeroFloat(0x10000);

        self->actor.world.rot.y = rand;
        self->actor.home.rot.y = rand;
        self->actor.shape.rot.y = rand;
    }

    if (!EnKusa_SnapToFloor(self, globalCtx, 0.0f)) {
        Actor_Kill(&self->actor);
        return;
    }

    self->kusaTexObjIndex = Object_GetIndex(&globalCtx->objectCtx, sObjectIds[thisx->params & 3]);

    if (self->kusaTexObjIndex < 0) {
        // "Bank danger!"
        osSyncPrintf("Error : バンク危険！ (arg_data 0x%04x)(%s %d)\n", thisx->params, "../z_en_kusa.c", 561);
        Actor_Kill(&self->actor);
        return;
    }

    func_80A9B7EC(self);
}

void EnKusa_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnKusa* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80A9B7EC(EnKusa* self) {
    EnKusa_SetupAction(self, func_80A9B810);
}

void func_80A9B810(EnKusa* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->kusaTexObjIndex)) {
        if (self->actor.flags & 0x800) {
            func_80A9BEAC(self);
        } else {
            func_80A9B89C(self);
        }

        self->actor.draw = EnKusa_Draw;
        self->actor.objBankIndex = self->kusaTexObjIndex;
        self->actor.flags &= ~0x10;
    }
}

void func_80A9B89C(EnKusa* self) {
    EnKusa_SetupAction(self, func_80A9B8D8);
    self->actor.flags &= ~0x10;
}

void func_80A9B8D8(EnKusa* self, GlobalContext* globalCtx) {
    s32 pad;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        EnKusa_SetupLiftedUp(self);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_PL_PULL_UP_PLANT);
    } else if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        EnKusa_SpawnFragments(self, globalCtx);
        EnKusa_DropCollectible(self, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EV_PLANT_BROKEN);

        if ((self->actor.params >> 4) & 1) {
            EnKusa_SpawnBugs(self, globalCtx);
        }

        if ((self->actor.params & 3) == 0) {
            Actor_Kill(&self->actor);
            return;
        }

        func_80A9BEAC(self);
        self->actor.flags |= 0x800;
    } else {
        if (!(self->collider.base.ocFlags1 & OC1_TYPE_PLAYER) && (self->actor.xzDistToPlayer > 12.0f)) {
            self->collider.base.ocFlags1 |= OC1_TYPE_PLAYER;
        }

        if (self->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

            if (self->actor.xzDistToPlayer < 400.0f) {
                CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
                if (self->actor.xzDistToPlayer < 100.0f) {
                    func_8002F580(&self->actor, globalCtx);
                }
            }
        }
    }
}

void EnKusa_SetupLiftedUp(EnKusa* self) {
    EnKusa_SetupAction(self, EnKusa_LiftedUp);
    self->actor.room = -1;
    self->actor.flags |= 0x10;
}

void EnKusa_LiftedUp(EnKusa* self, GlobalContext* globalCtx) {
    if (Actor_HasNoParent(&self->actor, globalCtx)) {
        self->actor.room = globalCtx->roomCtx.curRoom.num;
        func_80A9BBB0(self);
        self->actor.velocity.x = self->actor.speedXZ * Math_SinS(self->actor.world.rot.y);
        self->actor.velocity.z = self->actor.speedXZ * Math_CosS(self->actor.world.rot.y);
        self->actor.colChkInfo.mass = 240;
        self->actor.gravity = -0.1f;
        EnKusa_Fall(self);
        func_80A9B174(&self->actor.velocity, 0.005f);
        func_8002D7EC(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 7.5f, 35.0f, 0.0f, 0xC5);
        self->actor.gravity = -3.2f;
    }
}

void func_80A9BBB0(EnKusa* self) {
    EnKusa_SetupAction(self, func_80A9BC1C);
    D_80A9C1D0 = -0xBB8;
    D_80A9C1D8 = ((Rand_ZeroOne() - 0.5f) * 1600.0f);
    D_80A9C1D4 = 0;
    D_80A9C1DC = 0;
}

void func_80A9BC1C(EnKusa* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f contactPos;

    if (self->actor.bgCheckFlags & 11) {
        if (!(self->actor.bgCheckFlags & 32)) {
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EV_PLANT_BROKEN);
        }
        EnKusa_SpawnFragments(self, globalCtx);
        EnKusa_DropCollectible(self, globalCtx);
        switch (self->actor.params & 3) {
            case 0:
            case 2:
                Actor_Kill(&self->actor);
                break;

            case 1:
                func_80A9BF3C(self);
                break;
        }
    } else {
        if (self->actor.bgCheckFlags & 0x40) {
            contactPos.x = self->actor.world.pos.x;
            contactPos.y = self->actor.world.pos.y + self->actor.yDistToWater;
            contactPos.z = self->actor.world.pos.z;
            EffectSsGSplash_Spawn(globalCtx, &contactPos, NULL, NULL, 0, 400);
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 150, 650, 0);
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 400, 800, 4);
            EffectSsGRipple_Spawn(globalCtx, &contactPos, 500, 1100, 8);
            self->actor.minVelocityY = -3.0f;
            D_80A9C1D4 >>= 1;
            D_80A9C1D0 >>= 1;
            D_80A9C1DC >>= 1;
            D_80A9C1D8 >>= 1;
            self->actor.bgCheckFlags &= ~0x40;
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 40, NA_SE_EV_DIVE_INTO_WATER_L);
        }
        EnKusa_Fall(self);
        Math_StepToS(&D_80A9C1D4, D_80A9C1D0, 0x1F4);
        Math_StepToS(&D_80A9C1DC, D_80A9C1D8, 0xAA);
        self->actor.shape.rot.x += D_80A9C1D4;
        self->actor.shape.rot.y += D_80A9C1DC;
        func_80A9B174(&self->actor.velocity, 0.05f);
        func_8002D7EC(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 7.5f, 35.0f, 0.0f, 0xC5);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void func_80A9BEAC(EnKusa* self) {

    if ((self->actor.params & 3) != 1) {
        if ((self->actor.params & 3) == 2) {
            EnKusa_SetupAction(self, func_80A9BF30);
        }
    } else {
        EnKusa_SetupAction(self, func_80A9BEFC);
    }
}

void func_80A9BEFC(EnKusa* self, GlobalContext* globalCtx) {
    if (self->timer >= 120) {
        func_80A9C00C(self);
    }
}

void func_80A9BF30(EnKusa* self, GlobalContext* globalCtx) {
}

void func_80A9BF3C(EnKusa* self) {
    self->actor.world.pos.x = self->actor.home.pos.x;
    self->actor.world.pos.y = self->actor.home.pos.y - 9.0f;
    self->actor.world.pos.z = self->actor.home.pos.z;
    EnKusa_SetScale(self);
    self->actor.shape.rot = self->actor.home.rot;
    EnKusa_SetupAction(self, func_80A9BFA8);
}

void func_80A9BFA8(EnKusa* self, GlobalContext* globalCtx) {
    if (self->timer > 120) {
        if ((Math_StepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.6f)) && (self->timer >= 170)) {
            func_80A9C00C(self);
        }
    }
}

void func_80A9C00C(EnKusa* self) {
    EnKusa_SetupAction(self, func_80A9C068);
    EnKusa_SetScale(self);
    self->actor.shape.rot = self->actor.home.rot;
    self->actor.flags &= ~0x800;
}

void func_80A9C068(EnKusa* self, GlobalContext* globalCtx) {
    s32 sp24 = true;

    sp24 &= Math_StepToF(&self->actor.scale.y, 0.4f, 0.014f);
    sp24 &= Math_StepToF(&self->actor.scale.x, 0.4f, 0.011f);
    self->actor.scale.z = self->actor.scale.x;

    if (sp24) {
        Actor_SetScale(&self->actor, 0.4f);
        func_80A9B89C(self);
        self->collider.base.ocFlags1 &= ~OC1_TYPE_PLAYER;
    }
}

void EnKusa_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnKusa* self = THIS;

    self->timer++;

    self->actionFunc(self, globalCtx);

    if (self->actor.flags & 0x800) {
        self->actor.shape.yOffset = -6.25f;
    } else {
        self->actor.shape.yOffset = 0.0f;
    }
}

void EnKusa_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* dLists[] = { gFieldBushDL, 0x06000140, 0x06000140 };
    EnKusa* self = THIS;

    if (self->actor.flags & 0x800) {
        Gfx_DrawDListOpa(globalCtx, D_060002E0);
    } else {
        Gfx_DrawDListOpa(globalCtx, dLists[thisx->params & 3]);
    }
}
