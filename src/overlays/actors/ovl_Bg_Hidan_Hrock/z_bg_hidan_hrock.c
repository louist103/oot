/*
 * File: z_bg_hidan_hrock.c
 * Overlay: ovl_Bg_Hidan_Hrock
 * Description: Huge stone spike platform (Fire Temple)
 */

#include "z_bg_hidan_hrock.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgHidanHrock*)thisx)

void BgHidanHrock_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanHrock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanHrock_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanHrock_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8088960C(BgHidanHrock* self, GlobalContext* globalCtx);
void func_808896B8(BgHidanHrock* self, GlobalContext* globalCtx);
void func_808894A4(BgHidanHrock* self, GlobalContext* globalCtx);

const ActorInit Bg_Hidan_Hrock_InitVars = {
    ACTOR_BG_HIDAN_HROCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanHrock),
    (ActorFunc)BgHidanHrock_Init,
    (ActorFunc)BgHidanHrock_Destroy,
    (ActorFunc)BgHidanHrock_Update,
    (ActorFunc)BgHidanHrock_Draw,
};

static ColliderTrisElementInit sTrisElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
            OCELEM_NONE,
        },
        { { { -40.0f, 3.0f, -40.0f }, { -40.0f, 3.0f, 40.0f }, { 40.0f, 3.0f, 40.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
            OCELEM_NONE,
        },
        { { { 40.0f, 3.0f, 40.0f }, { 40.0f, 3.0f, -40.0f }, { -40.0f, 3.0f, -40.0f } } },
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
    2,
    sTrisElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -1, ICHAIN_STOP),
};

void BgHidanHrock_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanHrock* self = THIS;
    ColliderTrisElementInit* colliderElementInit;
    Vec3f vertices[3];
    f32 cosRotY;
    f32 sinRotY;
    s32 i;
    s32 j;
    CollisionHeader* collisionHeader = NULL;

    Actor_ProcessInitChain(thisx, sInitChain);
    self->unk_16A = thisx->params & 0x3F;
    thisx->params = (thisx->params >> 8) & 0xFF;
    Collider_InitTris(globalCtx, &self->collider);
    Collider_SetTris(globalCtx, &self->collider, thisx, &sTrisInit, self->colliderItems);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);

    sinRotY = Math_SinS(thisx->shape.rot.y);
    cosRotY = Math_CosS(thisx->shape.rot.y);

    if (thisx->params == 0) {
        sinRotY *= 1.5f;
        cosRotY *= 1.5f;
    }

    for (i = 0; i < 2; i++) {
        colliderElementInit = &sTrisInit.elements[i];

        if (1) {
            for (j = 0; j < 3; j++) {
                Vec3f* vtx = &colliderElementInit->dim.vtx[j];

                vertices[j].x = vtx->z * sinRotY + (thisx->home.pos.x + vtx->x * cosRotY);
                vertices[j].y = vtx->y + thisx->home.pos.y;
                vertices[j].z = vtx->z * cosRotY + (thisx->home.pos.z - vtx->x * sinRotY);
            }
        }
        Collider_SetTrisVertices(&self->collider, i, &vertices[0], &vertices[1], &vertices[2]);
    }

    if (Flags_GetSwitch(globalCtx, self->unk_16A)) {
        self->actionFunc = func_808894A4;
        if (thisx->params == 0) {
            thisx->world.pos.y -= 2800.0f;
            thisx->uncullZoneForward = 3000.0f;
        } else if (thisx->params == 1) {
            thisx->world.pos.y -= 800.0f;
        } else if (thisx->params == 2) {
            thisx->world.pos.y -= 240.0f;
        }
    } else {
        if (thisx->params == 0) {
            thisx->flags |= 0x30;
            thisx->uncullZoneForward = 3000.0f;
        }
        self->actionFunc = func_808896B8;
    }

    if (thisx->params == 0) {
        CollisionHeader_GetVirtual(&gFireTempleTallestPillarAboveRoomBeforeBossCol, &collisionHeader);
    } else {
        CollisionHeader_GetVirtual(&gFireTemplePillarInsertedInGroundCol, &collisionHeader);
    }

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, collisionHeader);
}

void BgHidanHrock_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanHrock* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyTris(globalCtx, &self->collider);
}

void func_808894A4(BgHidanHrock* self, GlobalContext* globalCtx) {
}

void func_808894B0(BgHidanHrock* self, GlobalContext* globalCtx) {
    if (self->unk_168 != 0) {
        self->unk_168--;
    }

    self->dyna.actor.world.pos.x =
        (Math_SinS(self->dyna.actor.world.rot.y + (self->unk_168 << 0xE)) * 5.0f) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z =
        (Math_CosS(self->dyna.actor.world.rot.y + (self->unk_168 << 0xE)) * 5.0f) + self->dyna.actor.home.pos.z;

    if (!(self->unk_168 % 4)) {
        func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 180, 10, 100);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_SHAKE);
    }

    if (self->unk_168 == 0) {
        if (self->dyna.actor.params == 0) {
            self->dyna.actor.home.pos.y -= 2800.0f;
        } else if (self->dyna.actor.params == 1) {
            self->dyna.actor.home.pos.y -= 800.0f;
        } else {
            self->dyna.actor.home.pos.y -= 240.0f;
        }

        self->actionFunc = func_8088960C;
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
    }
}

void func_8088960C(BgHidanHrock* self, GlobalContext* globalCtx) {
    self->dyna.actor.velocity.y++;

    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, self->dyna.actor.velocity.y)) {
        self->dyna.actor.flags &= ~0x30;
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);

        if (self->dyna.actor.params == 0) {
            if (globalCtx->roomCtx.curRoom.num == 10) {
                self->dyna.actor.room = 10;
            } else {
                Actor_Kill(&self->dyna.actor);
            }
        }

        self->actionFunc = func_808894A4;
    }
}

void func_808896B8(BgHidanHrock* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & 2) {
        self->collider.base.acFlags &= ~2;
        self->actionFunc = func_808894B0;
        self->dyna.actor.flags |= 0x10;

        if (self->dyna.actor.params == 0) {
            self->dyna.actor.room = -1;
        }

        self->unk_168 = 20;
        Flags_SetSwitch(globalCtx, self->unk_16A);
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    if (func_8004356C(&self->dyna)) {
        Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 5.0f, 1.0f);
    } else {
        Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 1.0f);
    }
}

void BgHidanHrock_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanHrock* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgHidanHrock_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* dlists[] = {
        gFireTempleTallestPillarAboveRoomBeforeBossDL,
        gFireTemplePillarInsertedInGroundDL,
        gFireTemplePillarInsertedInGroundDL,
    };

    Gfx_DrawDListOpa(globalCtx, dlists[thisx->params]);
}
