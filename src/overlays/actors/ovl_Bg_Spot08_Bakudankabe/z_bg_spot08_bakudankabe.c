/*
 * File: z_bg_spot08_bakudankabe
 * Overlay: ovl_Bg_Spot08_Bakudankabe
 * Description: Destructible Wall (Zora's Fountain)
 */

#include "z_bg_spot08_bakudankabe.h"
#include "objects/object_spot08_obj/object_spot08_obj.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

#define FLAGS 0x00400000

#define THIS ((BgSpot08Bakudankabe*)thisx)

void BgSpot08Bakudankabe_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Bakudankabe_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Bakudankabe_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot08Bakudankabe_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808B02D0(BgSpot08Bakudankabe* self, GlobalContext* globalCtx);
void func_808B0324(BgSpot08Bakudankabe* self, GlobalContext* globalCtx);

const ActorInit Bg_Spot08_Bakudankabe_InitVars = {
    ACTOR_BG_SPOT08_BAKUDANKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT08_OBJ,
    sizeof(BgSpot08Bakudankabe),
    (ActorFunc)BgSpot08Bakudankabe_Init,
    (ActorFunc)BgSpot08Bakudankabe_Destroy,
    (ActorFunc)BgSpot08Bakudankabe_Update,
    (ActorFunc)BgSpot08Bakudankabe_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000008, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 0, { { 0, 50, 50 }, 70 }, 100 },
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
        { 0, { { -100, 50, 50 }, 70 }, 100 },
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
        { 0, { { 100, 50, 50 }, 70 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    3,
    sJntSphElementsInit,
};

static Vec3f D_808B08AC[] = {
    { 0.0f, 116.65f, 50.0f },
    { 115.0f, 95.0f, 10.0f },
    { -115.0f, 95.0f, 10.0f },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F(scale, 1, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 3200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void func_808B02D0(BgSpot08Bakudankabe* self, GlobalContext* globalCtx) {
    s32 pad;

    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->dyna.actor, &sJntSphInit, self->colliderItems);
}

void func_808B0324(BgSpot08Bakudankabe* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s32 i;
    Vec3f burstDepthY;
    Vec3f burstDepthX;
    f32 sinY;
    f32 cosY;

    sinY = Math_SinS(self->dyna.actor.shape.rot.y);
    cosY = Math_CosS(self->dyna.actor.shape.rot.y);

    burstDepthX.z = 0.0f;
    burstDepthX.x = 0.0f;

    for (i = 0; i < 24; i++) {
        s16 gravityInfluence;
        s16 scale;
        f32 temp1;
        f32 temp2;
        s32 rotationSpeed;

        temp1 = (Rand_ZeroOne() - 0.5f) * 440.0f;
        temp2 = (Rand_ZeroOne() - 0.5f) * 20.0f;
        burstDepthY.x = self->dyna.actor.world.pos.x + temp2 * sinY + (temp1 * cosY);
        burstDepthY.y = (self->dyna.actor.world.pos.y + 20.0f) + (i * (65.0f / 12.0f));
        burstDepthY.z = self->dyna.actor.world.pos.z + temp2 * cosY - (temp1 * sinY);

        burstDepthX.y = (Rand_ZeroOne() - 0.2f) * 12.0f;
        scale = Rand_ZeroOne() * 75.0f + 10.0f;

        if (scale < 25) {
            gravityInfluence = -300;
        } else if (scale < 50) {
            gravityInfluence = -360;
        } else {
            gravityInfluence = -420;
        }

        if (Rand_ZeroOne() < 0.4f) {
            rotationSpeed = 65;
        } else {
            rotationSpeed = 33;
        }

        EffectSsKakera_Spawn(globalCtx, &burstDepthY, &burstDepthX, &burstDepthY, gravityInfluence, rotationSpeed, 0x1E,
                             4, 0, scale, 1, 3, 80, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_FIELD_KEEP, gFieldKakeraDL);
    }

    for (i = 0; i < ARRAY_COUNT(D_808B08AC); i++) {
        burstDepthY.x = self->dyna.actor.world.pos.x + D_808B08AC[i].z * sinY + D_808B08AC[i].x * cosY;
        burstDepthY.y = self->dyna.actor.world.pos.y + D_808B08AC[i].y;
        burstDepthY.z = self->dyna.actor.world.pos.z + D_808B08AC[i].z * cosY - (D_808B08AC[i].x * sinY);
        func_80033480(globalCtx, &burstDepthY, 120.0f, 4, 0x78, 0xA0, 1);
    }
}

void BgSpot08Bakudankabe_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Bakudankabe* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params & 0x3F))) {
        Actor_Kill(&self->dyna.actor);
        return;
    }
    func_808B02D0(self, globalCtx);
    CollisionHeader_GetVirtual(&gZorasFountainBombableWallCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
}

void BgSpot08Bakudankabe_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Bakudankabe* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void BgSpot08Bakudankabe_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Bakudankabe* self = THIS;

    if (self->collider.base.acFlags & AC_HIT) {
        func_808B0324(self, globalCtx);
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params & 0x3F));
        Audio_PlaySoundAtPosition(globalCtx, &self->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        Actor_Kill(&self->dyna.actor);
    } else if (self->dyna.actor.xzDistToPlayer < 800.0f) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void BgSpot08Bakudankabe_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot08Bakudankabe* self = THIS;

    Collider_UpdateSpheres(0, &self->collider);
    Collider_UpdateSpheres(1, &self->collider);
    Collider_UpdateSpheres(2, &self->collider);
    Gfx_DrawDListOpa(globalCtx, gZorasFountainBombableWallDL);
}
