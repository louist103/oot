/*
 * File: z_bg_ddan_kd.c
 * Overlay: ovl_Bg_Ddan_Kd
 * Description: Falling stairs in Dodongo's Cavern
 */

#include "z_bg_ddan_kd.h"
#include "objects/object_ddan_objects/object_ddan_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgDdanKd*)thisx)

void BgDdanKd_Init(Actor* thisx, GlobalContext* globalCtx);
void BgDdanKd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgDdanKd_Update(Actor* thisx, GlobalContext* globalCtx);
void BgDdanKd_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgDdanKd_CheckForExplosions(BgDdanKd* self, GlobalContext* globalCtx);
void BgDdanKd_LowerStairs(BgDdanKd* self, GlobalContext* globalCtx);
void func_80871838(BgDdanKd* self, GlobalContext* globalCtx);

const ActorInit Bg_Ddan_Kd_InitVars = {
    ACTOR_BG_DDAN_KD,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DDAN_OBJECTS,
    sizeof(BgDdanKd),
    (ActorFunc)BgDdanKd_Init,
    (ActorFunc)BgDdanKd_Destroy,
    (ActorFunc)BgDdanKd_Update,
    (ActorFunc)BgDdanKd_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ALL,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 245, 180, -400, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 32767, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 32767, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 32767, ICHAIN_STOP),
};

void BgDdanKd_SetupAction(BgDdanKd* self, BgDdanKdActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgDdanKd_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanKd* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    self->prevExplosive = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->dyna.actor, &sCylinderInit);
    CollisionHeader_GetVirtual(&gDodongoFallingStairsCol, &colHeader);

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params) == 0) {
        BgDdanKd_SetupAction(self, BgDdanKd_CheckForExplosions);
    } else {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - 200.0f - 20.0f;
        BgDdanKd_SetupAction(self, func_80871838);
    }
}

void BgDdanKd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanKd* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void BgDdanKd_CheckForExplosions(BgDdanKd* self, GlobalContext* globalCtx) {
    Actor* explosive;

    explosive = Actor_GetCollidedExplosive(globalCtx, &self->collider.base);
    if (explosive != NULL) {
        osSyncPrintf("dam    %d\n", self->dyna.actor.colChkInfo.damage);
        explosive->params = 2;
    }
    if ((explosive != NULL) && (self->prevExplosive != NULL) && (explosive != self->prevExplosive) &&
        (Math_Vec3f_DistXZ(&self->prevExplosivePos, &explosive->world.pos) > 80.0f)) {
        BgDdanKd_SetupAction(self, BgDdanKd_LowerStairs);
        OnePointCutscene_Init(globalCtx, 3050, 999, &self->dyna.actor, MAIN_CAM);
    } else {
        if (self->timer != 0) {
            self->timer -= 1;
        } else {
            self->prevExplosive = explosive;
            if (explosive != NULL) {
                self->timer = 13;
                self->prevExplosivePos = explosive->world.pos;
            }
        }
        Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void BgDdanKd_LowerStairs(BgDdanKd* self, GlobalContext* globalCtx) {
    static Vec3f D_808718FC = { 0.0f, 5.0f, 0.0f };
    static Vec3f D_80871908 = { 0.0f, -0.45f, 0.0f };
    Vec3f sp5C;
    Vec3f sp50;
    f32 sp4C;

    Math_SmoothStepToF(&self->dyna.actor.speedXZ, 4.0f, 0.5f, 0.025f, 0.0f);
    func_800AA000(500.0f, 0x78, 0x14, 0xA);

    if (Math_SmoothStepToF(&self->dyna.actor.world.pos.y, (self->dyna.actor.home.pos.y - 200.0f) - 20.0f, 0.075f,
                           self->dyna.actor.speedXZ, 0.0075f) == 0.0f) {
        Flags_SetSwitch(globalCtx, self->dyna.actor.params);
        BgDdanKd_SetupAction(self, func_80871838);
    } else {
        sp4C = (self->dyna.actor.prevPos.y - self->dyna.actor.world.pos.y) + (self->dyna.actor.speedXZ * 0.25f);

        if (globalCtx->state.frames & 1) {
            sp5C = sp50 = self->dyna.actor.world.pos;

            if (globalCtx->state.frames & 2) {
                sp5C.z += 210.0f + Rand_ZeroOne() * 230.0f;
                sp50.z += 210.0f + Rand_ZeroOne() * 230.0f;
            } else {
                sp5C.z += 330.0f + Rand_ZeroOne() * 240.0f;
                sp50.z += 330.0f + Rand_ZeroOne() * 240.0f;
            }
            sp5C.x += 80.0f + Rand_ZeroOne() * 10.0f;
            sp50.x -= 80.0f + Rand_ZeroOne() * 10.0f;
            sp5C.y = self->dyna.actor.floorHeight + 20.0f + Rand_ZeroOne();
            sp50.y = self->dyna.actor.floorHeight + 20.0f + Rand_ZeroOne();

            func_80033480(globalCtx, &sp5C, 20.0f, 1, sp4C * 135.0f, 60, 1);
            func_80033480(globalCtx, &sp50, 20.0f, 1, sp4C * 135.0f, 60, 1);

            D_808718FC.x = Rand_CenteredFloat(3.0f);
            D_808718FC.z = Rand_CenteredFloat(3.0f);

            func_8003555C(globalCtx, &sp5C, &D_808718FC, &D_80871908);
            func_8003555C(globalCtx, &sp50, &D_808718FC, &D_80871908);

            sp5C = self->dyna.actor.world.pos;
            sp5C.z += 560.0f + Rand_ZeroOne() * 5.0f;
            sp5C.x += (Rand_ZeroOne() - 0.5f) * 160.0f;
            sp5C.y = Rand_ZeroOne() * 3.0f + (self->dyna.actor.floorHeight + 20.0f);

            func_80033480(globalCtx, &sp5C, 20.0f, 1, sp4C * 135.0f, 60, 1);
            func_8003555C(globalCtx, &sp5C, &D_808718FC, &D_80871908);
        }
        Camera_AddQuake(&globalCtx->mainCamera, 0, sp4C * 0.6f, 3);
        Audio_PlaySoundGeneral(NA_SE_EV_PILLAR_SINK - SFX_FLAG, &self->dyna.actor.projectedPos, 4, &D_801333E0,
                               &D_801333E0, &D_801333E8);
    }
}

void func_80871838(BgDdanKd* self, GlobalContext* globalCtx) {
}

void BgDdanKd_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanKd* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgDdanKd_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gDodongoFallingStairsDL);
}
