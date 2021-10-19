/*
 * File: z_bg_mori_hashigo.c
 * Overlay: ovl_Bg_Mori_Hashigo
 * Description: Falling ladder and clasp that holds it. Unused.
 */

#include "z_bg_mori_hashigo.h"
#include "objects/object_mori_objects/object_mori_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgMoriHashigo*)thisx)

void BgMoriHashigo_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHashigo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHashigo_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMoriHashigo_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMoriHashigo_SetupWaitForMoriTex(BgMoriHashigo* self);
void BgMoriHashigo_WaitForMoriTex(BgMoriHashigo* self, GlobalContext* globalCtx);
void BgMoriHashigo_SetupClasp(BgMoriHashigo* self);
void BgMoriHashigo_Clasp(BgMoriHashigo* self, GlobalContext* globalCtx);
void BgMoriHashigo_SetupLadderWait(BgMoriHashigo* self);
void BgMoriHashigo_LadderWait(BgMoriHashigo* self, GlobalContext* globalCtx);
void BgMoriHashigo_SetupLadderFall(BgMoriHashigo* self);
void BgMoriHashigo_LadderFall(BgMoriHashigo* self, GlobalContext* globalCtx);
void BgMoriHashigo_SetupLadderRest(BgMoriHashigo* self);

const ActorInit Bg_Mori_Hashigo_InitVars = {
    ACTOR_BG_MORI_HASHIGO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriHashigo),
    (ActorFunc)BgMoriHashigo_Init,
    (ActorFunc)BgMoriHashigo_Destroy,
    (ActorFunc)BgMoriHashigo_Update,
    NULL,
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK4,
            { 0x00000000, 0x00, 0x00 },
            { 0x0001F820, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 25 }, 100 },
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

static InitChainEntry sInitChainClasp[] = {
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),  ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_CONTINUE), ICHAIN_U8(targetMode, 3, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 40, ICHAIN_CONTINUE),    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

static InitChainEntry sInitChainLadder[] = {
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgMoriHashigo_InitDynapoly(BgMoriHashigo* self, GlobalContext* globalCtx, CollisionHeader* collision,
                                s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader;
    s32 pad2;

    colHeader = NULL;
    DynaPolyActor_Init(&self->dyna, moveFlag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning : move BG login failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_mori_hashigo.c", 164,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgMoriHashigo_InitCollider(BgMoriHashigo* self, GlobalContext* globalCtx) {
    s32 pad;

    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->dyna.actor, &sJntSphInit, self->colliderItems);

    self->collider.elements[0].dim.worldSphere.center.x = (s16)self->dyna.actor.world.pos.x;
    self->collider.elements[0].dim.worldSphere.center.y = (s16)self->dyna.actor.world.pos.y + 21;
    self->collider.elements[0].dim.worldSphere.center.z = (s16)self->dyna.actor.world.pos.z;
    self->collider.elements[0].dim.worldSphere.radius = 19;
}

s32 BgMoriHashigo_SpawnLadder(BgMoriHashigo* self, GlobalContext* globalCtx) {
    f32 sn;
    f32 cs;
    Vec3f pos;
    Actor* ladder;

    cs = Math_CosS(self->dyna.actor.shape.rot.y);
    sn = Math_SinS(self->dyna.actor.shape.rot.y);

    pos.x = 6.0f * sn + self->dyna.actor.world.pos.x;
    pos.y = -210.0f + self->dyna.actor.world.pos.y;
    pos.z = 6.0f * cs + self->dyna.actor.world.pos.z;

    ladder = Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_BG_MORI_HASHIGO, pos.x, pos.y,
                                pos.z, self->dyna.actor.world.rot.x, self->dyna.actor.world.rot.y,
                                self->dyna.actor.world.rot.z, 0);
    if (ladder != NULL) {
        return true;
    } else {
        // "Ladder failure"
        osSyncPrintf("Error : 梯子の発生失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mori_hashigo.c", 220,
                     self->dyna.actor.params);
        return false;
    }
}

s32 BgMoriHashigo_InitClasp(BgMoriHashigo* self, GlobalContext* globalCtx) {
    Actor_ProcessInitChain(&self->dyna.actor, sInitChainClasp);
    self->dyna.actor.flags |= 1;
    Actor_SetFocus(&self->dyna.actor, 55.0f);
    BgMoriHashigo_InitCollider(self, globalCtx);
    if ((self->dyna.actor.params == HASHIGO_CLASP) && !BgMoriHashigo_SpawnLadder(self, globalCtx)) {
        return false;
    } else {
        return true;
    }
}

s32 BgMoriHashigo_InitLadder(BgMoriHashigo* self, GlobalContext* globalCtx) {
    BgMoriHashigo_InitDynapoly(self, globalCtx, &gMoriHashigoCol, DPM_UNK);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChainLadder);
    return true;
}

void BgMoriHashigo_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriHashigo* self = THIS;

    if (self->dyna.actor.params == HASHIGO_CLASP) {
        if (!BgMoriHashigo_InitClasp(self, globalCtx)) {
            Actor_Kill(&self->dyna.actor);
            return;
        }
    } else if (self->dyna.actor.params == HASHIGO_LADDER) {
        if (!BgMoriHashigo_InitLadder(self, globalCtx)) {
            Actor_Kill(&self->dyna.actor);
            return;
        }
    }
    self->moriTexObjIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjIndex < 0) {
        // "Bank danger!"
        osSyncPrintf("Error : バンク危険！(arg_data 0x%04x)(%s %d)\n", self->dyna.actor.params,
                     "../z_bg_mori_hashigo.c", 312);
        Actor_Kill(&self->dyna.actor);
    } else {
        BgMoriHashigo_SetupWaitForMoriTex(self);
        // "(Forest Temple Ladder and its clasp)"
        osSyncPrintf("(森の神殿 梯子とその留め金)(arg_data 0x%04x)\n", self->dyna.actor.params);
    }
}

void BgMoriHashigo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriHashigo* self = THIS;

    if (self->dyna.actor.params == HASHIGO_LADDER) {
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
    if (self->dyna.actor.params == HASHIGO_CLASP) {
        Collider_DestroyJntSph(globalCtx, &self->collider);
    }
}

void BgMoriHashigo_SetupWaitForMoriTex(BgMoriHashigo* self) {
    self->actionFunc = BgMoriHashigo_WaitForMoriTex;
}

void BgMoriHashigo_WaitForMoriTex(BgMoriHashigo* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->moriTexObjIndex)) {
        if (self->dyna.actor.params == HASHIGO_CLASP) {
            BgMoriHashigo_SetupClasp(self);
        } else if (self->dyna.actor.params == HASHIGO_LADDER) {
            BgMoriHashigo_SetupLadderWait(self);
        }
        self->dyna.actor.draw = BgMoriHashigo_Draw;
    }
}

void BgMoriHashigo_SetupClasp(BgMoriHashigo* self) {
    self->actionFunc = BgMoriHashigo_Clasp;
}

void BgMoriHashigo_Clasp(BgMoriHashigo* self, GlobalContext* globalCtx) {
    if (self->hitTimer <= 0) {
        if (self->collider.base.acFlags & AC_HIT) {
            self->collider.base.acFlags &= ~AC_HIT;
            self->hitTimer = 10;
        } else {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

void BgMoriHashigo_SetupLadderWait(BgMoriHashigo* self) {
    self->actionFunc = BgMoriHashigo_LadderWait;
}

void BgMoriHashigo_LadderWait(BgMoriHashigo* self, GlobalContext* globalCtx) {
    BgMoriHashigo* clasp = (BgMoriHashigo*)self->dyna.actor.parent;

    if (clasp->hitTimer > 0) {
        BgMoriHashigo_SetupLadderFall(self);
    }
}

void BgMoriHashigo_SetupLadderFall(BgMoriHashigo* self) {
    self->bounceCounter = 0;
    self->actionFunc = BgMoriHashigo_LadderFall;
    self->dyna.actor.gravity = -1.0f;
    self->dyna.actor.minVelocityY = -10.0f;
    self->dyna.actor.velocity.y = 2.0f;
}

void BgMoriHashigo_LadderFall(BgMoriHashigo* self, GlobalContext* globalCtx) {
    static f32 bounceSpeed[3] = { 4.0f, 2.7f, 1.7f };
    Actor* thisx = &self->dyna.actor;

    Actor_MoveForward(thisx);
    if ((thisx->bgCheckFlags & 1) && (thisx->velocity.y < 0.0f)) {
        if (self->bounceCounter >= ARRAY_COUNT(bounceSpeed)) {
            BgMoriHashigo_SetupLadderRest(self);
        } else {
            Actor_UpdateBgCheckInfo(globalCtx, thisx, 0.0f, 0.0f, 0.0f, 0x1C);
            thisx->velocity.y = bounceSpeed[self->bounceCounter];
            self->bounceCounter++;
        }
    } else {
        Actor_UpdateBgCheckInfo(globalCtx, thisx, 0.0f, 0.0f, 0.0f, 0x1C);
    }
}

void BgMoriHashigo_SetupLadderRest(BgMoriHashigo* self) {
    self->actionFunc = NULL;
    self->dyna.actor.gravity = 0.0f;
    self->dyna.actor.velocity.y = 0.0f;
    self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
}

void BgMoriHashigo_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriHashigo* self = THIS;

    if (self->hitTimer > 0) {
        self->hitTimer--;
    }
    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
}

void BgMoriHashigo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriHashigo* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_hashigo.c", 516);
    func_80093D18(globalCtx->state.gfxCtx);
    if (1) {}
    gSPSegment(POLY_OPA_DISP++, 0x08, globalCtx->objectCtx.status[self->moriTexObjIndex].segment);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_hashigo.c", 521),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    switch (self->dyna.actor.params) {
        case HASHIGO_CLASP:
            gSPDisplayList(POLY_OPA_DISP++, gMoriHashigoClaspDL);
            break;
        case HASHIGO_LADDER:
            gSPDisplayList(POLY_OPA_DISP++, gMoriHashigoLadderDL);
            break;
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_hashigo.c", 531);
}
