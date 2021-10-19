/*
 * File: z_bg_jya_bombiwa.c
 * Overlay: ovl_Bg_Jya_Bombiwa
 * Description: Spirit Temple top room bombable wall
 */

#include "z_bg_jya_bombiwa.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/object_jya_obj/object_jya_obj.h"
#include "vt.h"

#define FLAGS 0x00000000

#define THIS ((BgJyaBombiwa*)thisx)

void BgJyaBombiwa_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Bg_Jya_Bombiwa_InitVars = {
    ACTOR_BG_JYA_BOMBIWA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaBombiwa),
    (ActorFunc)BgJyaBombiwa_Init,
    (ActorFunc)BgJyaBombiwa_Destroy,
    (ActorFunc)BgJyaBombiwa_Update,
    (ActorFunc)BgJyaBombiwa_Draw,
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
        { 0, { { 0, 0, 0 }, 50 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgJyaBombiwa_SetupDynaPoly(BgJyaBombiwa* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 flag) {
    s16 pad1;
    CollisionHeader* colHeader = NULL;
    s16 pad2;

    DynaPolyActor_Init(&self->dyna, flag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {

        // "Warning: move BG registration failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_bombiwa.c", 174,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgJyaBombiwa_InitCollider(BgJyaBombiwa* self, GlobalContext* globalCtx) {
    s32 pad;

    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->dyna.actor, &sJntSphInit, self->colliderItems);
}

void BgJyaBombiwa_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBombiwa* self = THIS;

    if ((self->dyna.actor.params & 0x3F) != 0x29) {
        osSyncPrintf(VT_COL(YELLOW, BLACK));

        // "Warning: Switch Number changed (%s %d)(SW %d)"
        osSyncPrintf("Ｗａｒｎｉｎｇ : Switch Number が変更された(%s %d)(SW %d)\n", "../z_bg_jya_bombiwa.c", 218,
                     self->dyna.actor.params & 0x3F);
        osSyncPrintf(VT_RST);
    }
    BgJyaBombiwa_SetupDynaPoly(self, globalCtx, &gBombiwaCol, DPM_UNK);
    BgJyaBombiwa_InitCollider(self, globalCtx);
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
        Actor_Kill(&self->dyna.actor);
    } else {
        Actor_ProcessInitChain(&self->dyna.actor, sInitChain);

        // "Rock destroyed by jya bomb"
        osSyncPrintf("(jya 爆弾で破壊岩)(arg_data 0x%04x)\n", self->dyna.actor.params);
    }
}

void BgJyaBombiwa_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBombiwa* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void BgJyaBombiwa_Break(BgJyaBombiwa* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Vec3f velocity;
    s16 arg5;
    s8 arg6;
    s8 arg7;
    s32 i;
    s16 scale;

    for (i = 0; i < 16; i++) {
        pos.x = ((Rand_ZeroOne() * 80.0f) + self->dyna.actor.world.pos.x) - 40.0f;
        pos.y = (Rand_ZeroOne() * 140.0f) + self->dyna.actor.world.pos.y;
        pos.z = ((Rand_ZeroOne() * 80.0f) + self->dyna.actor.world.pos.z) - 40.0f;
        velocity.x = (Rand_ZeroOne() - 0.5f) * 10.0f;
        velocity.y = Rand_ZeroOne() * 12.0f;
        velocity.z = (Rand_ZeroOne() - 0.5f) * 10.0f;
        scale = (s32)(i * 1.8f) + 3;
        if (scale > 15) {
            arg5 = 5;
        } else {
            arg5 = 1;
        }
        if (Rand_ZeroOne() < 0.4f) {
            arg5 |= 0x40;
            arg6 = 0xC;
            arg7 = 8;
        } else {
            arg5 |= 0x20;
            arg6 = 0xC;
            arg7 = 8;
            if (scale < 10) {
                arg6 = 0x50;
                arg7 = 80;
            }
        }
        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &pos, -400, arg5, arg6, arg7, 0, scale, 1, 20, 80,
                             KAKERA_COLOR_NONE, OBJECT_JYA_OBJ, gBombiwaEffectDL);
    }
    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y + 70.0f;
    pos.z = self->dyna.actor.world.pos.z;
    func_80033480(globalCtx, &pos, 100.0f, 0xA, 0x64, 0xA0, 1);
}

void BgJyaBombiwa_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBombiwa* self = THIS;

    if (self->collider.base.acFlags & AC_HIT) {
        BgJyaBombiwa_Break(self, globalCtx);
        Flags_SetSwitch(globalCtx, self->dyna.actor.params & 0x3F);
        Audio_PlaySoundAtPosition(globalCtx, &self->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
        Actor_Kill(&self->dyna.actor);
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void BgJyaBombiwa_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBombiwa* self = THIS;

    Gfx_DrawDListOpa(globalCtx, gBombiwaDL);
    Collider_UpdateSpheres(0, &self->collider);
}
