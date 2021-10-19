/*
 * File: z_bg_bombwall.c
 * Overlay: ovl_Bg_Bombwall
 * Description: Bombable Wall
 */

#include "z_bg_bombwall.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

#define FLAGS 0x00400000

#define THIS ((BgBombwall*)thisx)

void BgBombwall_Init(Actor* thisx, GlobalContext* globalCtx);
void BgBombwall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgBombwall_Update(Actor* thisx, GlobalContext* globalCtx);
void BgBombwall_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8086ED50(BgBombwall* self, GlobalContext* globalCtx);
void func_8086ED70(BgBombwall* self, GlobalContext* globalCtx);
void func_8086EDFC(BgBombwall* self, GlobalContext* globalCtx);
void func_8086EE40(BgBombwall* self, GlobalContext* globalCtx);
void func_8086EE94(BgBombwall* self, GlobalContext* globalCtx);

static ColliderTrisElementInit sTrisElementsInit[3] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000048, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { -70.0f, 176.0f, 0.0f }, { -70.0f, -4.0f, 0.0f }, { 0.0f, -4.0f, 30.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000048, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 70.0f, 176.0f, 0.0f }, { -70.0f, 176.0f, 0.0f }, { 0.0f, -4.0f, 30.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000048, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 70.0f, -4.0f, 0.0f }, { 70.0f, 176.0f, 0.0f }, { 0.0f, -4.0f, 30.0f } } },
    },
};

static ColliderTrisInit sTrisInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    3,
    sTrisElementsInit,
};

const ActorInit Bg_Bombwall_InitVars = {
    ACTOR_BG_BOMBWALL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(BgBombwall),
    (ActorFunc)BgBombwall_Init,
    (ActorFunc)BgBombwall_Destroy,
    (ActorFunc)BgBombwall_Update,
    (ActorFunc)BgBombwall_Draw,
};

void BgBombwall_InitDynapoly(BgBombwall* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gBgBombwallCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning : move BG login failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_bombwall.c", 243,
                     self->dyna.actor.params);
    }
}

void BgBombwall_RotateVec(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3) {
    arg0->x = (arg1->z * arg2) + (arg1->x * arg3);
    arg0->y = arg1->y;
    arg0->z = (arg1->z * arg3) - (arg1->x * arg2);
}

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgBombwall_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 i;
    s32 j;
    Vec3f vecs[3];
    Vec3f sp80;
    s32 pad;
    BgBombwall* self = THIS;
    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    Actor_SetScale(&self->dyna.actor, 0.1f);

    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
        func_8086EE94(self, globalCtx);
    } else {
        BgBombwall_InitDynapoly(self, globalCtx);
        self->unk_2A2 |= 2;
        Collider_InitTris(globalCtx, &self->collider);
        Collider_SetTris(globalCtx, &self->collider, &self->dyna.actor, &sTrisInit, self->colliderItems);

        for (i = 0; i <= 2; i++) {
            for (j = 0; j <= 2; j++) {
                sp80.x = sTrisInit.elements[i].dim.vtx[j].x;
                sp80.y = sTrisInit.elements[i].dim.vtx[j].y;
                sp80.z = sTrisInit.elements[i].dim.vtx[j].z + 2.0f;

                BgBombwall_RotateVec(&vecs[j], &sp80, sin, cos);

                vecs[j].x += self->dyna.actor.world.pos.x;
                vecs[j].y += self->dyna.actor.world.pos.y;
                vecs[j].z += self->dyna.actor.world.pos.z;
            }
            Collider_SetTrisVertices(&self->collider, i, &vecs[0], &vecs[1], &vecs[2]);
        }

        self->unk_2A2 |= 1;
        func_8086ED50(self, globalCtx);
    }

    osSyncPrintf("(field keep 汎用爆弾壁)(arg_data 0x%04x)(angY %d)\n", self->dyna.actor.params,
                 self->dyna.actor.shape.rot.y);
}

void BgBombwall_DestroyCollision(BgBombwall* self, GlobalContext* globalCtx) {
    if (self->unk_2A2 & 2) {
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        self->unk_2A2 &= ~2;
    }

    if (self->unk_2A2 & 1) {
        Collider_DestroyTris(globalCtx, &self->collider);
        self->unk_2A2 &= ~1;
    }
}

void BgBombwall_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgBombwall* self = THIS;

    BgBombwall_DestroyCollision(self, globalCtx);
}

static Vec3s D_8086F010[] = {
    { 40, 85, 21 }, { -43, 107, 14 }, { -1, 142, 14 }, { -27, 44, 27 }, { 28, 24, 20 }, { -39, 54, 21 }, { 49, 50, 20 },
};

void func_8086EB5C(BgBombwall* self, GlobalContext* globalCtx) {
    s16 rand;
    s16 rand2;
    Vec3f sp88;
    s32 i;
    f32 sin = Math_SinS(self->dyna.actor.shape.rot.y);
    f32 cos = Math_CosS(self->dyna.actor.shape.rot.y);
    Vec3f* pos = &self->dyna.actor.world.pos;
    f32 temp;
    f32 new_var;

    for (i = 0; i < 7; i++) {
        new_var = D_8086F010[i].x;
        temp = new_var * cos;
        sp88.x = ((sin * D_8086F010[i].z) + ((f32)temp)) + pos->x;
        sp88.y = pos->y + D_8086F010[i].y;
        sp88.z = ((D_8086F010[i].z * cos) - (sin * D_8086F010[i].x)) + pos->z;
        rand = ((s16)(Rand_ZeroOne() * 120.0f)) + 0x14;
        rand2 = ((s16)(Rand_ZeroOne() * 240.0f)) + 0x14;
        func_80033480(globalCtx, &sp88, 50.0f, 2, rand, rand2, 1);
    }

    sp88.x = pos->x;
    new_var = pos->y + 90.0f;
    sp88.y = pos->y + 90.0f;
    sp88.z = pos->z + 15.0f;
    func_80033480(globalCtx, &sp88, 40.0f, 4, 0xA, 0x32, 1);
}

void func_8086ED50(BgBombwall* self, GlobalContext* globalCtx) {
    self->dList = gBgBombwallNormalDL;
    self->actionFunc = func_8086ED70;
}

void func_8086ED70(BgBombwall* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        func_8086EDFC(self, globalCtx);
        Flags_SetSwitch(globalCtx, self->dyna.actor.params & 0x3F);
    } else if (self->dyna.actor.xzDistToPlayer < 600.0f) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void func_8086EDFC(BgBombwall* self, GlobalContext* globalCtx) {
    self->dList = gBgBombwallNormalDL;
    self->unk_2A0 = 1;
    func_8086EB5C(self, globalCtx);
    self->actionFunc = func_8086EE40;
}

void func_8086EE40(BgBombwall* self, GlobalContext* globalCtx) {
    if (self->unk_2A0 > 0) {
        self->unk_2A0--;
    } else {
        func_8086EE94(self, globalCtx);

        if (((self->dyna.actor.params >> 0xF) & 1) != 0) {
            func_80078884(NA_SE_SY_CORRECT_CHIME);
        }
    }
}

void func_8086EE94(BgBombwall* self, GlobalContext* globalCtx) {
    self->dList = gBgBombwallBrokenDL;
    BgBombwall_DestroyCollision(self, globalCtx);
    self->actionFunc = NULL;
}

void BgBombwall_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgBombwall* self = THIS;

    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
}

void BgBombwall_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgBombwall* self = THIS;

    Gfx_DrawDListOpa(globalCtx, self->dList);
}
