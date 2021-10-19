/*
 * File: z_obj_comb.c
 * Overlay: ovl_Obj_Comb
 * Description: Beehive
 */

#include "z_obj_comb.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

#define FLAGS 0x00000000

#define THIS ((ObjComb*)thisx)

void ObjComb_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjComb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjComb_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjComb_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjComb_Break(ObjComb* self, GlobalContext* globalCtx);
void ObjComb_ChooseItemDrop(ObjComb* self, GlobalContext* globalCtx);
void ObjComb_SetupWait(ObjComb* self);
void ObjComb_Wait(ObjComb* self, GlobalContext* globalCtx);

const ActorInit Obj_Comb_InitVars = {
    ACTOR_OBJ_COMB,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(ObjComb),
    (ActorFunc)ObjComb_Init,
    (ActorFunc)ObjComb_Destroy,
    (ActorFunc)ObjComb_Update,
    (ActorFunc)ObjComb_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x4001FFFE, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 900, ICHAIN_STOP),
};

void ObjComb_Break(ObjComb* self, GlobalContext* globalCtx) {
    Vec3f pos1;
    Vec3f pos;
    Vec3f velocity;
    Gfx* dlist = gFieldBeehiveFragmentDL;
    s16 scale;
    s16 angle = 0;
    s16 gravity;
    u8 arg5;
    u8 arg6;
    f32 rand1;
    f32 rand2;
    s32 i;

    for (i = 0; i < 31; i++) {
        angle += 0x4E20;
        rand1 = Rand_ZeroOne() * 10.0f;

        pos1.x = Math_SinS(angle) * rand1;
        pos1.y = (i - 15) * 0.7f;
        pos1.z = Math_CosS(angle) * rand1;

        Math_Vec3f_Sum(&pos1, &self->actor.world.pos, &pos);

        velocity.x = (Rand_ZeroOne() - 0.5f) + pos1.x * 0.5f;
        velocity.y = (Rand_ZeroOne() - 0.5f) + pos1.y * 0.6f;
        velocity.z = (Rand_ZeroOne() - 0.5f) + pos1.z * 0.5f;

        scale = Rand_ZeroOne() * 72.0f + 25.0f;

        if (scale < 40) {
            gravity = -200;
            arg6 = 40;
        } else if (scale < 70) {
            gravity = -280;
            arg6 = 30;
        } else {
            gravity = -340;
            arg6 = 20;
        }

        rand2 = Rand_ZeroOne();

        if (rand2 < 0.1f) {
            arg5 = 96;
        } else if (rand2 < 0.8f) {
            arg5 = 64;
        } else {
            arg5 = 32;
        }

        EffectSsKakera_Spawn(globalCtx, &pos, &velocity, &pos, gravity, arg5, arg6, 4, 0, scale, 0, 0, 80,
                             KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_FIELD_KEEP, dlist);
    }

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y - 10.0f;
    pos.z = self->actor.world.pos.z;
    func_80033480(globalCtx, &pos, 40.0f, 6, 70, 60, 1);
}

void ObjComb_ChooseItemDrop(ObjComb* self, GlobalContext* globalCtx) {
    s16 params = self->actor.params & 0x1F;

    if ((params > 0) || (params < 0x1A)) {
        if (params == 6) {
            if (Flags_GetCollectible(globalCtx, (self->actor.params >> 8) & 0x3F)) {
                params = -1;
            } else {
                params = (params | (((self->actor.params >> 8) & 0x3F) << 8));
            }
        } else if (Rand_ZeroOne() < 0.5f) {
            params = -1;
        }
        if (params >= 0) {
            Item_DropCollectible(globalCtx, &self->actor.world.pos, params);
        }
    }
}

void ObjComb_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjComb* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItems);
    ObjComb_SetupWait(self);
}

void ObjComb_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    ObjComb* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void ObjComb_SetupWait(ObjComb* self) {
    self->actionFunc = ObjComb_Wait;
}

void ObjComb_Wait(ObjComb* self, GlobalContext* globalCtx) {
    s32 dmgFlags;

    self->unk_1B0 -= 50;
    if (self->unk_1B0 < 0) {
        self->unk_1B0 = 0;
    }

    if ((self->collider.base.acFlags & AC_HIT) != 0) {
        self->collider.base.acFlags &= ~AC_HIT;
        dmgFlags = self->collider.elements[0].info.acHitInfo->toucher.dmgFlags;
        if (dmgFlags & 0x4001F866) {
            self->unk_1B0 = 1500;
        } else {
            ObjComb_Break(self, globalCtx);
            ObjComb_ChooseItemDrop(self, globalCtx);
            Actor_Kill(&self->actor);
        }
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if (self->actor.update != NULL) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void ObjComb_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjComb* self = THIS;

    self->unk_1B2 += 0x2EE0;
    self->actionFunc(self, globalCtx);
    self->actor.shape.rot.x = Math_SinS(self->unk_1B2) * self->unk_1B0 + self->actor.home.rot.x;
}

void ObjComb_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ObjComb* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_comb.c", 369);

    func_80093D18(globalCtx->state.gfxCtx);

    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y + (118.0f * self->actor.scale.y),
                     self->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateY(self->actor.shape.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_RotateX(self->actor.shape.rot.x * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_RotateZ(self->actor.shape.rot.z * (M_PI / 0x8000), MTXMODE_APPLY);
    Matrix_Translate(0, -(self->actor.scale.y * 118.0f), 0, MTXMODE_APPLY);
    Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_comb.c", 394),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, gFieldBeehiveDL);

    Collider_UpdateSpheres(0, &self->collider);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_comb.c", 402);
}
