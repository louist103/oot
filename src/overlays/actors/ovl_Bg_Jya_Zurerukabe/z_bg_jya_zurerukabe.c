/*
 * File: z_bg_jya_zurerukabe.c
 * Overlay: ovl_Bg_Jya_Zurerukabe
 * Description: Sliding, Climbable Brick Wall
 */

#include "z_bg_jya_zurerukabe.h"
#include "objects/object_jya_obj/object_jya_obj.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((BgJyaZurerukabe*)thisx)

void BgJyaZurerukabe_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaZurerukabe_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaZurerukabe_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaZurerukabe_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8089B4C8(BgJyaZurerukabe* self, GlobalContext* globalCtx);
void func_8089B7B4(BgJyaZurerukabe* self);
void func_8089B7C4(BgJyaZurerukabe* self, GlobalContext* globalCtx);
void func_8089B80C(BgJyaZurerukabe* self);
void func_8089B870(BgJyaZurerukabe* self, GlobalContext* globalCtx);

static f32 D_8089B9C0[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

const ActorInit Bg_Jya_Zurerukabe_InitVars = {
    ACTOR_BG_JYA_ZURERUKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaZurerukabe),
    (ActorFunc)BgJyaZurerukabe_Init,
    (ActorFunc)BgJyaZurerukabe_Destroy,
    (ActorFunc)BgJyaZurerukabe_Update,
    (ActorFunc)BgJyaZurerukabe_Draw,
};

static s16 D_8089B9F0[4] = { 943, 1043, 1243, 1343 };

static s16 D_8089B9F8[4] = { -1, 1, -1, 1 };

static s16 D_8089BA00[4] = { 48, 48, 36, 36 };

static f32 D_8089BA08[4] = { 8.0f, 8.0f, 10.0f, 10.0f };

static s16 D_8089BA18[6][2] = {
    { 0x0388, 0x0395 }, { 0x03EA, 0x03FF }, { 0x0454, 0x0467 },
    { 0x04B4, 0x04C1 }, { 0x0518, 0x0528 }, { 0x0581, 0x0590 },
};

static s16 D_8089BA30[6] = {
    0, 0, 1, 2, 2, 3,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgJyaZurerukabe_InitDynaPoly(BgJyaZurerukabe* self, GlobalContext* globalCtx, CollisionHeader* collision,
                                  s32 flag) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, flag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_zurerukabe.c", 194,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void func_8089B4C8(BgJyaZurerukabe* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((player->stateFlags1 == 0x200000) && (player->actor.wallPoly != NULL)) {
        s32 i;

        for (i = 0; i < ARRAY_COUNT(D_8089BA18); i++) {
            f32 posY = player->actor.world.pos.y;

            if ((posY >= D_8089BA18[i][0]) && (posY <= D_8089BA18[i][1])) {
                break;
            }
        }

        switch (i) {
            case 0:
            case 2:
            case 3:
            case 5:
                if (fabsf(D_8089B9C0[D_8089BA30[i]]) > 1.0f) {
                    func_8002F6D4(globalCtx, &self->dyna.actor, 1.5f, self->dyna.actor.shape.rot.y, 0.0f, 0);
                }
                break;
            case 1:
            case 4:
                if (fabsf(D_8089B9C0[D_8089BA30[i]] - D_8089B9C0[D_8089BA30[i + 1]]) > 1.0f) {
                    func_8002F6D4(globalCtx, &self->dyna.actor, 1.5f, self->dyna.actor.shape.rot.y, 0.0f, 0);
                }
                break;
        }
    }
}

void BgJyaZurerukabe_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaZurerukabe* self = THIS;
    s32 i;

    BgJyaZurerukabe_InitDynaPoly(self, globalCtx, &gZurerukabeCol, DPM_UNK);
    Actor_ProcessInitChain(thisx, sInitChain);

    for (i = 0; i < ARRAY_COUNT(D_8089B9F0); i++) {
        if (fabsf(D_8089B9F0[i] - self->dyna.actor.home.pos.y) < 1.0f) {
            self->unk_168 = i;
            break;
        }
    }

    if (i == ARRAY_COUNT(D_8089B9F0)) {
        osSyncPrintf(VT_COL(RED, WHITE));
        osSyncPrintf("home pos が変更されたみたい(%s %d)(arg_data 0x%04x)\n", "../z_bg_jya_zurerukabe.c", 299,
                     self->dyna.actor.params);
        osSyncPrintf(VT_RST);
    }

    self->unk_16E = D_8089B9F8[self->unk_168];
    func_8089B7B4(self);
    osSyncPrintf("(jya ずれる壁)(arg_data 0x%04x)\n", self->dyna.actor.params);
}

void BgJyaZurerukabe_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaZurerukabe* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    D_8089B9C0[self->unk_168] = 0.0f;
}

void func_8089B7B4(BgJyaZurerukabe* self) {
    self->actionFunc = func_8089B7C4;
}

void func_8089B7C4(BgJyaZurerukabe* self, GlobalContext* globalCtx) {
    if (self->unk_16A <= 0) {
        func_8089B80C(self);
    }
    D_8089B9C0[self->unk_168] = 0.0f;
}

void func_8089B80C(BgJyaZurerukabe* self) {
    self->actionFunc = func_8089B870;
    self->unk_16A = D_8089BA00[self->unk_168];
    if (ABS(self->unk_16C) == 4) {
        self->unk_16E = -self->unk_16E;
    }
    self->unk_16C += self->unk_16E;
}

void func_8089B870(BgJyaZurerukabe* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x + (self->unk_16C * 75),
                     D_8089BA08[self->unk_168])) {
        func_8089B7B4(self);
    }

    D_8089B9C0[self->unk_168] = D_8089BA08[self->unk_168] * self->unk_16E;
    func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
}

void BgJyaZurerukabe_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaZurerukabe* self = THIS;

    if (self->unk_16A > 0) {
        self->unk_16A--;
    }

    self->actionFunc(self, globalCtx);

    if (self->unk_168 == 0) {
        func_8089B4C8(self, globalCtx);
    }
}

void BgJyaZurerukabe_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gZurerukabeDL);
}
