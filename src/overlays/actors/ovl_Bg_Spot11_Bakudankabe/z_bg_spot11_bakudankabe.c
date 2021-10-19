/*
 * File: z_bg_spot11_bakudankabe.c
 * Overlay: ovl_Bg_Spot11_Bakudankabe
 * Description: Destructible Wall (Desert Colossus)
 */

#include "z_bg_spot11_bakudankabe.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/object_spot11_obj/object_spot11_obj.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

#define FLAGS 0x00000030

#define THIS ((BgSpot11Bakudankabe*)thisx)

void BgSpot11Bakudankabe_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot11Bakudankabe_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot11Bakudankabe_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot11Bakudankabe_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Bg_Spot11_Bakudankabe_InitVars = {
    ACTOR_BG_SPOT11_BAKUDANKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT11_OBJ,
    sizeof(BgSpot11Bakudankabe),
    (ActorFunc)BgSpot11Bakudankabe_Init,
    (ActorFunc)BgSpot11Bakudankabe_Destroy,
    (ActorFunc)BgSpot11Bakudankabe_Update,
    (ActorFunc)BgSpot11Bakudankabe_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000008, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 40, 80, 0, { 2259, 108, -1580 } },
};

static Vec3f D_808B272C = { 2259.0f, 108.0f, -1550.0f };
static Vec3f D_808B2738 = { 2259.0f, 108.0f, -1550.0f };

void func_808B2180(BgSpot11Bakudankabe* self, GlobalContext* globalCtx) {
    s32 pad;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->dyna.actor, &sCylinderInit);
    self->collider.dim.pos.x += (s16)self->dyna.actor.world.pos.x;
    self->collider.dim.pos.y += (s16)self->dyna.actor.world.pos.y;
    self->collider.dim.pos.z += (s16)self->dyna.actor.world.pos.z;
}

void func_808B2218(BgSpot11Bakudankabe* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;
    Vec3f burstDepthY;
    Vec3f burstDepthX;
    s32 i;

    burstDepthX.z = 0;
    burstDepthX.x = 0;

    for (i = 0; i < 20; i++) {
        s16 scale;
        s32 gravityInfluence;
        s32 rotationSpeed;

        Math_Vec3f_Sum(&thisx->world.pos, &D_808B272C, &burstDepthY);

        burstDepthY.x += (Rand_ZeroOne() - 0.5f) * 120.0f;
        burstDepthY.y += (30.0f + (i * 6.5f));
        burstDepthY.z += (Rand_ZeroOne() - 0.5f) * 20.0f;

        burstDepthX.y = (Rand_ZeroOne() - 0.2f) * 12.0f;
        scale = (Rand_ZeroOne() * 55.0f) + 8.0f;

        if (scale < 20) {
            gravityInfluence = -300;
        } else if (scale < 35) {
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
    Math_Vec3f_Sum(&thisx->world.pos, &D_808B272C, &burstDepthY);
    func_80033480(globalCtx, &burstDepthY, 70, 4, 110, 160, 1);
    burstDepthY.y += 40;
    func_80033480(globalCtx, &burstDepthY, 70, 5, 110, 160, 1);
    burstDepthY.y += 40;
    func_80033480(globalCtx, &burstDepthY, 70, 4, 110, 160, 1);
}

void BgSpot11Bakudankabe_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot11Bakudankabe* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params & 0x3F))) {
        Actor_Kill(&self->dyna.actor);
        return;
    }
    func_808B2180(self, globalCtx);
    CollisionHeader_GetVirtual(&gDesertColossusBombableWallCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    Actor_SetScale(&self->dyna.actor, 1.0f);
    osSyncPrintf("(spot11 爆弾壁)(arg_data 0x%04x)\n", self->dyna.actor.params);
}

void BgSpot11Bakudankabe_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot11Bakudankabe* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void BgSpot11Bakudankabe_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot11Bakudankabe* self = THIS;

    if (self->collider.base.acFlags & AC_HIT) {
        func_808B2218(self, globalCtx);
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params & 0x3F));
        Audio_PlaySoundAtPosition(globalCtx, &D_808B2738, 40, NA_SE_EV_WALL_BROKEN);
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        Actor_Kill(&self->dyna.actor);
        return;
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void BgSpot11Bakudankabe_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot11Bakudankabe* self = THIS;

    Gfx_DrawDListOpa(globalCtx, gDesertColossusBombableWallDL);
}
