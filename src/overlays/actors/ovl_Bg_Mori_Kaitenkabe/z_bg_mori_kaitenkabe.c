/*
 * File: z_bg_mori_kaitenkabe.c
 * Overlay: ovl_Bg_Mori_Kaitenkabe
 * Description: Rotating wall in Forest Temple basement
 */

#include "z_bg_mori_kaitenkabe.h"
#include "objects/object_mori_objects/object_mori_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgMoriKaitenkabe*)thisx)

void BgMoriKaitenkabe_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMoriKaitenkabe_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMoriKaitenkabe_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMoriKaitenkabe_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMoriKaitenkabe_WaitForMoriTex(BgMoriKaitenkabe* self, GlobalContext* globalCtx);
void BgMoriKaitenkabe_SetupWait(BgMoriKaitenkabe* self);
void BgMoriKaitenkabe_Wait(BgMoriKaitenkabe* self, GlobalContext* globalCtx);
void BgMoriKaitenkabe_SetupRotate(BgMoriKaitenkabe* self);
void BgMoriKaitenkabe_Rotate(BgMoriKaitenkabe* self, GlobalContext* globalCtx);

const ActorInit Bg_Mori_Kaitenkabe_InitVars = {
    ACTOR_BG_MORI_KAITENKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriKaitenkabe),
    (ActorFunc)BgMoriKaitenkabe_Init,
    (ActorFunc)BgMoriKaitenkabe_Destroy,
    (ActorFunc)BgMoriKaitenkabe_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgMoriKaitenkabe_CrossProduct(Vec3f* dest, Vec3f* v1, Vec3f* v2) {
    dest->x = (v1->y * v2->z) - (v1->z * v2->y);
    dest->y = (v1->z * v2->x) - (v1->x * v2->z);
    dest->z = (v1->x * v2->y) - (v1->y * v2->x);
}

void BgMoriKaitenkabe_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriKaitenkabe* self = THIS;
    CollisionHeader* colHeader = NULL;

    // "Forest Temple object 【Rotating Wall (arg_data: 0x% 04x)】 appears"
    osSyncPrintf("◯◯◯森の神殿オブジェクト【回転壁(arg_data : 0x%04x)】出現 \n", self->dyna.actor.params);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gMoriKaitenkabeCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    self->moriTexObjIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjIndex < 0) {
        Actor_Kill(&self->dyna.actor);
        // "【Rotating wall】 Bank danger!"
        osSyncPrintf("【回転壁】 バンク危険！(%s %d)\n", "../z_bg_mori_kaitenkabe.c", 176);
    } else {
        self->actionFunc = BgMoriKaitenkabe_WaitForMoriTex;
    }
}

void BgMoriKaitenkabe_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriKaitenkabe* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgMoriKaitenkabe_WaitForMoriTex(BgMoriKaitenkabe* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->moriTexObjIndex)) {
        BgMoriKaitenkabe_SetupWait(self);
        self->dyna.actor.draw = BgMoriKaitenkabe_Draw;
    }
}

void BgMoriKaitenkabe_SetupWait(BgMoriKaitenkabe* self) {
    self->actionFunc = BgMoriKaitenkabe_Wait;
    self->timer = 0;
}

void BgMoriKaitenkabe_Wait(BgMoriKaitenkabe* self, GlobalContext* globalCtx) {
    Vec3f push;
    Vec3f leverArm;
    Vec3f torque;
    Player* player = GET_PLAYER(globalCtx);

    if (self->dyna.unk_150 > 0.001f) {
        self->timer++;
        if ((self->timer > 28) && !Player_InCsMode(globalCtx)) {
            BgMoriKaitenkabe_SetupRotate(self);
            func_8002DF54(globalCtx, &self->dyna.actor, 8);
            Math_Vec3f_Copy(&self->lockedPlayerPos, &player->actor.world.pos);
            push.x = Math_SinS(self->dyna.unk_158);
            push.y = 0.0f;
            push.z = Math_CosS(self->dyna.unk_158);
            leverArm.x = self->dyna.actor.world.pos.x - player->actor.world.pos.x;
            leverArm.y = 0.0f;
            leverArm.z = self->dyna.actor.world.pos.z - player->actor.world.pos.z;
            BgMoriKaitenkabe_CrossProduct(&torque, &push, &leverArm);
            self->rotDirection = (torque.y > 0.0f) ? 1.0f : -1.0f;
        }
    } else {
        self->timer = 0;
    }
    if (fabsf(self->dyna.unk_150) > 0.001f) {
        self->dyna.unk_150 = 0.0f;
        player->stateFlags2 &= ~0x10;
    }
}

void BgMoriKaitenkabe_SetupRotate(BgMoriKaitenkabe* self) {
    self->actionFunc = BgMoriKaitenkabe_Rotate;
    self->rotSpeed = 0.0f;
    self->rotYdeg = 0.0f;
}

void BgMoriKaitenkabe_Rotate(BgMoriKaitenkabe* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* thisx = &self->dyna.actor;
    s16 rotY;

    Math_StepToF(&self->rotSpeed, 0.6f, 0.02f);
    if (Math_StepToF(&self->rotYdeg, self->rotDirection * 45.0f, self->rotSpeed)) {
        BgMoriKaitenkabe_SetupWait(self);
        func_8002DF54(globalCtx, thisx, 7);
        if (self->rotDirection > 0.0f) {
            thisx->home.rot.y += 0x2000;
        } else {
            thisx->home.rot.y -= 0x2000;
        }
        thisx->world.rot.y = thisx->shape.rot.y = thisx->home.rot.y;
        func_800788CC(NA_SE_EV_STONEDOOR_STOP);
    } else {
        rotY = self->rotYdeg * (0x10000 / 360.0f);
        thisx->world.rot.y = thisx->shape.rot.y = thisx->home.rot.y + rotY;
        func_800788CC(NA_SE_EV_WALL_SLIDE - SFX_FLAG);
    }
    if (fabsf(self->dyna.unk_150) > 0.001f) {
        self->dyna.unk_150 = 0.0f;
        player->stateFlags2 &= ~0x10;
    }
    Math_Vec3f_Copy(&player->actor.world.pos, &self->lockedPlayerPos);
}

void BgMoriKaitenkabe_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriKaitenkabe* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgMoriKaitenkabe_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriKaitenkabe* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_kaitenkabe.c", 347);
    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, globalCtx->objectCtx.status[self->moriTexObjIndex].segment);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_kaitenkabe.c", 352),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, gMoriKaitenkabeDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_kaitenkabe.c", 356);
}
