/*
 * File: z_en_si.c
 * Overlay: En_Si
 * Description: Gold Skulltula token
 */

#include "z_en_si.h"

#define FLAGS 0x00000201

#define THIS ((EnSi*)thisx)

void EnSi_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSi_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSi_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSi_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 func_80AFB748(EnSi* self, GlobalContext* globalCtx);
void func_80AFB768(EnSi* self, GlobalContext* globalCtx);
void func_80AFB89C(EnSi* self, GlobalContext* globalCtx);
void func_80AFB950(EnSi* self, GlobalContext* globalCtx);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_NO_PUSH | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000090, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 18, 2, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 D_80AFBADC = { 0, 0, 0, 0, MASS_IMMOVABLE };

const ActorInit En_Si_InitVars = {
    ACTOR_EN_SI,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_ST,
    sizeof(EnSi),
    (ActorFunc)EnSi_Init,
    (ActorFunc)EnSi_Destroy,
    (ActorFunc)EnSi_Update,
    (ActorFunc)EnSi_Draw,
};

void EnSi_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSi* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &D_80AFBADC);
    Actor_SetScale(&self->actor, 0.025f);
    self->unk_19C = 0;
    self->actionFunc = func_80AFB768;
    self->actor.shape.yOffset = 42.0f;
}

void EnSi_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnSi* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 func_80AFB748(EnSi* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
    }
    return 0;
}

void func_80AFB768(EnSi* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((self->actor.flags & 0x2000) == 0x2000) {
        self->actionFunc = func_80AFB89C;
    } else {
        Math_SmoothStepToF(&self->actor.scale.x, 0.25f, 0.4f, 1.0f, 0.0f);
        Actor_SetScale(&self->actor, self->actor.scale.x);
        self->actor.shape.rot.y += 0x400;

        if (!Player_InCsMode(globalCtx)) {
            func_80AFB748(self, globalCtx);

            if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
                self->collider.base.ocFlags2 &= ~OC2_HIT_PLAYER;
                Item_Give(globalCtx, ITEM_SKULL_TOKEN);
                player->actor.freezeTimer = 10;
                func_8010B680(globalCtx, 0xB4, 0);
                func_800F5C64(0x39);
                self->actionFunc = func_80AFB950;
            } else {
                Collider_UpdateCylinder(&self->actor, &self->collider);
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
                CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            }
        }
    }
}

void func_80AFB89C(EnSi* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToF(&self->actor.scale.x, 0.25f, 0.4f, 1.0f, 0.0f);
    Actor_SetScale(&self->actor, self->actor.scale.x);
    self->actor.shape.rot.y += 0x400;

    if ((self->actor.flags & 0x2000) != 0x2000) {
        Item_Give(globalCtx, ITEM_SKULL_TOKEN);
        player->actor.freezeTimer = 10;
        func_8010B680(globalCtx, 0xB4, 0);
        func_800F5C64(0x39);
        self->actionFunc = func_80AFB950;
    }
}

void func_80AFB950(EnSi* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (func_8010BDBC(&globalCtx->msgCtx) != 2) {
        player->actor.freezeTimer = 10;
    } else {
        SET_GS_FLAGS((self->actor.params & 0x1F00) >> 8, self->actor.params & 0xFF);
        Actor_Kill(&self->actor);
    }
}

void EnSi_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnSi* self = THIS;

    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    self->actionFunc(self, globalCtx);
    Actor_SetFocus(&self->actor, 16.0f);
}

void EnSi_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnSi* self = THIS;

    if (self->actionFunc != func_80AFB950) {
        func_8002ED80(&self->actor, globalCtx, 0);
        func_8002EBCC(&self->actor, globalCtx, 0);
        GetItem_Draw(globalCtx, GID_SKULL_TOKEN_2);
    }
}
