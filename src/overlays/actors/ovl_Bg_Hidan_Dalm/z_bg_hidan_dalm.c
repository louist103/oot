/*
 * File: z_bg_hidan_dalm.c
 * Overlay: ovl_Bg_Hidan_Dalm
 * Description: Hammerable Totem Pieces (Fire Temple)
 */

#include "z_bg_hidan_dalm.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgHidanDalm*)thisx)

void BgHidanDalm_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanDalm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanDalm_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanDalm_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgHidanDalm_Wait(BgHidanDalm* self, GlobalContext* globalCtx);
void BgHidanDalm_Shrink(BgHidanDalm* self, GlobalContext* globalCtx);

const ActorInit Bg_Hidan_Dalm_InitVars = {
    ACTOR_BG_HIDAN_DALM,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanDalm),
    (ActorFunc)BgHidanDalm_Init,
    (ActorFunc)BgHidanDalm_Destroy,
    (ActorFunc)BgHidanDalm_Update,
    (ActorFunc)BgHidanDalm_Draw,
};

static ColliderTrisElementInit sTrisElementInit[4] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
            OCELEM_NONE,
        },
        { { { 305.0f, 0.0f, -300.0f }, { 305.0f, 600.0f, -300.0f }, { 305.0f, 600.0f, 300.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
            OCELEM_NONE,
        },
        { { { 305.0f, 0.0f, -300.0f }, { 305.0f, 600.0f, 300.0f }, { 305.0f, 0.0f, 300.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
            OCELEM_NONE,
        },
        { { { -305.0f, 0.0f, -300.0f }, { -305.0f, 600.0f, 300.0f }, { -305.0f, 600.0f, -300.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
            OCELEM_NONE,
        },
        { { { -305.0f, 0.0f, -300.0f }, { -305.0f, 0.0f, 300.0f }, { -305.0f, 600.0f, 300.0f } } },
    },
};

static ColliderTrisInit sTrisInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_TRIS,
    },
    4,
    sTrisElementInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -200, ICHAIN_STOP),
};

void BgHidanDalm_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanDalm* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gFireTempleHammerableTotemCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    Collider_InitTris(globalCtx, &self->collider);
    Collider_SetTris(globalCtx, &self->collider, thisx, &sTrisInit, self->colliderItems);

    self->switchFlag = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
        Actor_Kill(thisx);
    } else {
        self->actionFunc = BgHidanDalm_Wait;
    }
}

void BgHidanDalm_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanDalm* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyTris(globalCtx, &self->collider);
}

void BgHidanDalm_Wait(BgHidanDalm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((self->collider.base.acFlags & AC_HIT) && !Player_InCsMode(globalCtx) &&
        (player->swordAnimation == 22 || player->swordAnimation == 23)) {
        self->collider.base.acFlags &= ~AC_HIT;
        if ((self->collider.elements[0].info.bumperFlags & BUMP_HIT) ||
            (self->collider.elements[1].info.bumperFlags & BUMP_HIT)) {
            self->dyna.actor.world.rot.y -= 0x4000;
        } else {
            self->dyna.actor.world.rot.y += 0x4000;
        }
        self->dyna.actor.world.pos.x += 32.5f * Math_SinS(self->dyna.actor.world.rot.y);
        self->dyna.actor.world.pos.z += 32.5f * Math_CosS(self->dyna.actor.world.rot.y);

        func_8002DF54(globalCtx, &self->dyna.actor, 8);
        self->dyna.actor.flags |= 0x10;
        self->actionFunc = BgHidanDalm_Shrink;
        self->dyna.actor.bgCheckFlags &= ~2;
        self->dyna.actor.bgCheckFlags &= ~8;
        self->dyna.actor.speedXZ = 10.0f;
        Flags_SetSwitch(globalCtx, self->switchFlag);
        func_8002F7DC(&GET_PLAYER(globalCtx)->actor, NA_SE_IT_HAMMER_HIT);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_DARUMA_VANISH);
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void BgHidanDalm_Shrink(BgHidanDalm* self, GlobalContext* globalCtx) {
    static Vec3f accel = { 0, 0, 0 };
    s32 i;
    Vec3f velocity;
    Vec3f pos;

    if (Math_StepToF(&self->dyna.actor.scale.x, 0.0f, 0.004f)) {
        func_8002DF54(globalCtx, &self->dyna.actor, 7);
        Actor_Kill(&self->dyna.actor);
    }

    self->dyna.actor.scale.y = self->dyna.actor.scale.z = self->dyna.actor.scale.x;

    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y + self->dyna.actor.scale.x * 160.0f;
    pos.z = self->dyna.actor.world.pos.z;

    for (i = 0; i < 4; i++) {
        velocity.x = 5.0f * Math_SinS(self->dyna.actor.world.rot.y + 0x8000) + (Rand_ZeroOne() - 0.5f) * 5.0f;
        velocity.z = 5.0f * Math_CosS(self->dyna.actor.world.rot.y + 0x8000) + (Rand_ZeroOne() - 0.5f) * 5.0f;
        velocity.y = (Rand_ZeroOne() - 0.5f) * 1.5f;
        EffectSsKiraKira_SpawnSmallYellow(globalCtx, &pos, &velocity, &accel);
    }
}

void BgHidanDalm_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanDalm* self = THIS;

    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->dyna.actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->dyna.actor, 10.0f, 15.0f, 32.0f, 5);
}

/**
 * Update vertices of collider tris based on the current matrix
 */
void BgHidanDalm_UpdateCollider(BgHidanDalm* self) {
    Vec3f pos2;
    Vec3f pos1;
    Vec3f pos0;

    Matrix_MultVec3f(&sTrisElementInit[0].dim.vtx[0], &pos0);
    Matrix_MultVec3f(&sTrisElementInit[0].dim.vtx[1], &pos1);
    Matrix_MultVec3f(&sTrisElementInit[0].dim.vtx[2], &pos2);
    Collider_SetTrisVertices(&self->collider, 0, &pos0, &pos1, &pos2);
    Matrix_MultVec3f(&sTrisElementInit[1].dim.vtx[2], &pos1);
    Collider_SetTrisVertices(&self->collider, 1, &pos0, &pos2, &pos1);

    Matrix_MultVec3f(&sTrisElementInit[2].dim.vtx[0], &pos0);
    Matrix_MultVec3f(&sTrisElementInit[2].dim.vtx[1], &pos1);
    Matrix_MultVec3f(&sTrisElementInit[2].dim.vtx[2], &pos2);
    Collider_SetTrisVertices(&self->collider, 2, &pos0, &pos1, &pos2);
    Matrix_MultVec3f(&sTrisElementInit[3].dim.vtx[1], &pos2);
    Collider_SetTrisVertices(&self->collider, 3, &pos0, &pos2, &pos1);
}

void BgHidanDalm_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanDalm* self = THIS;

    if (self->dyna.actor.params == 0) {
        Gfx_DrawDListOpa(globalCtx, gFireTempleHammerableTotemBodyDL);
    } else {
        Gfx_DrawDListOpa(globalCtx, gFireTempleHammerableTotemHeadDL);
    }

    if (self->actionFunc == BgHidanDalm_Wait) {
        BgHidanDalm_UpdateCollider(self);
    }
}
