/*
 * File: z_obj_warp2block.c
 * Overlay: ovl_Obj_Warp2Block
 * Description: Navi Infospot (Green, Time Block)
 */

#include "z_obj_warp2block.h"
#include "objects/object_timeblock/object_timeblock.h"
#include "vt.h"

#define FLAGS 0x0A000011

#define THIS ((ObjWarp2block*)thisx)

void ObjWarp2block_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjWarp2block_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjWarp2block_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjWarp2block_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjWarp2block_Spawn(ObjWarp2block* self, GlobalContext* globalCtx);
s32 func_80BA1ECC(ObjWarp2block* self, GlobalContext* globalCtx);
void ObjWarp2block_SwapWithChild(ObjWarp2block* self, GlobalContext* globalCtx);
s32 func_80BA2218(ObjWarp2block* self, GlobalContext* globalCtx);
s32 func_80BA228C(ObjWarp2block* self, GlobalContext* globalCtx);
s32 func_80BA2304(ObjWarp2block* self, GlobalContext* globalCtx);
void ObjWarp2block_SetInactive(ObjWarp2block* self);
void ObjWarp2block_DoNothing(ObjWarp2block* self, GlobalContext* globalCtx);
void func_80BA24E8(ObjWarp2block* self);
void func_80BA24F8(ObjWarp2block* self, GlobalContext* globalCtx);
void func_80BA2600(ObjWarp2block* self);
void func_80BA2610(ObjWarp2block* self, GlobalContext* globalCtx);

const ActorInit Obj_Warp2block_InitVars = {
    ACTOR_OBJ_WARP2BLOCK,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_TIMEBLOCK,
    sizeof(ObjWarp2block),
    (ActorFunc)ObjWarp2block_Init,
    (ActorFunc)ObjWarp2block_Destroy,
    (ActorFunc)ObjWarp2block_Update,
    (ActorFunc)ObjWarp2block_Draw,
};

typedef struct {
    /* 0x00 */ f32 scale;
    /* 0x04 */ f32 focus;
    /* 0x08 */ s16 params;
} Warp2BlockSpawnData; // size = 0x0C

static Warp2BlockSpawnData sSpawnData[] = {
    { 1.0f, 60.0f, 0x0018 },
    { 0.6f, 40.0f, 0x0019 },
};

static f32 sDistances[] = { 60.0f, 100.0f, 140.0f, 180.0f, 220.0f, 260.0f, 300.0f, 300.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_STOP),
};

static Color_RGB8 sColors[] = {
    { 100, 120, 140 }, { 80, 140, 200 }, { 100, 150, 200 }, { 100, 200, 240 },
    { 80, 110, 140 },  { 70, 160, 225 }, { 80, 100, 130 },  { 100, 110, 190 },
};

void ObjWarp2block_Spawn(ObjWarp2block* self, GlobalContext* globalCtx) {
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, self->dyna.actor.world.pos.x,
                self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z, 0, 0, 0,
                sSpawnData[(self->dyna.actor.params >> 8) & 1].params);

    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, self->dyna.actor.child->world.pos.x,
                self->dyna.actor.child->world.pos.y, self->dyna.actor.child->world.pos.z, 0, 0, 0,
                sSpawnData[(self->dyna.actor.child->params >> 8) & 1].params);
}

s32 func_80BA1ECC(ObjWarp2block* self, GlobalContext* globalCtx) {
    s32 pad;
    Actor* temp_a3;
    Player* player;
    Vec3f sp20;
    f32 temp_f2;

    if (func_80043590(&self->dyna)) {
        return 0;
    }

    temp_a3 = self->dyna.actor.child;
    player = GET_PLAYER(globalCtx);
    if ((self->dyna.actor.xzDistToPlayer <= sDistances[(((self->dyna.actor.params >> 0xB) & 7))]) ||
        (temp_a3->xzDistToPlayer <= sDistances[(((temp_a3->params >> 0xB) & 7))])) {

        func_8002DBD0(&self->dyna.actor, &sp20, &player->actor.world.pos);
        temp_f2 = (self->dyna.actor.scale.x * 50.0f) + 6.0f;

        if (!(temp_f2 < fabsf(sp20.x)) && !(temp_f2 < fabsf(sp20.z))) {
            return 0;
        }

        func_8002DBD0(temp_a3, &sp20, &player->actor.world.pos);
        temp_f2 = (temp_a3->scale.x * 50.0f) + 6.0f;

        if (!(temp_f2 < fabsf(sp20.x)) && !(temp_f2 < fabsf(sp20.z))) {
            return 0;
        }
    } else {
        return 0;
    }

    return 1;
}

void ObjWarp2block_SwapWithChild(ObjWarp2block* self, GlobalContext* globalCtx) {
    Vec3f tempVec;
    Vec3s tempRot;
    s32 temp;

    Math_Vec3f_Copy(&tempVec, &self->dyna.actor.world.pos);
    Math_Vec3f_Copy(&self->dyna.actor.world.pos, &self->dyna.actor.child->world.pos);
    Math_Vec3f_Copy(&self->dyna.actor.child->world.pos, &tempVec);

    temp = self->dyna.actor.world.rot.y;
    self->dyna.actor.world.rot.y = self->dyna.actor.child->world.rot.y;
    self->dyna.actor.child->world.rot.y = temp;

    temp = self->dyna.actor.shape.rot.y;
    self->dyna.actor.shape.rot.y = self->dyna.actor.child->shape.rot.y;
    self->dyna.actor.child->shape.rot.y = temp;

    temp = self->dyna.actor.home.rot.z;
    self->dyna.actor.home.rot.z = self->dyna.actor.child->home.rot.z;
    self->dyna.actor.child->home.rot.z = temp;

    Math_Vec3f_Copy(&tempVec, &self->dyna.actor.scale);
    Math_Vec3f_Copy(&self->dyna.actor.scale, &self->dyna.actor.child->scale);
    Math_Vec3f_Copy(&self->dyna.actor.child->scale, &tempVec);

    Math_Vec3f_Copy(&tempVec, &self->dyna.actor.focus.pos);
    Math_Vec3f_Copy(&self->dyna.actor.focus.pos, &self->dyna.actor.child->focus.pos);
    Math_Vec3f_Copy(&self->dyna.actor.child->focus.pos, &tempVec);

    tempRot = self->dyna.actor.focus.rot;
    self->dyna.actor.focus.rot = self->dyna.actor.child->focus.rot;
    self->dyna.actor.child->focus.rot = tempRot;

    temp = self->dyna.actor.params & 0x7FFF;
    self->dyna.actor.params = (self->dyna.actor.params & 0x8000) | (self->dyna.actor.child->params & 0x7FFF);
    self->dyna.actor.child->params = (self->dyna.actor.child->params & 0x8000) | (temp & 0x7FFF);

    if (Math3D_Vec3fDistSq(&self->dyna.actor.world.pos, &self->dyna.actor.home.pos) < 0.01f) {
        Flags_UnsetSwitch(globalCtx, self->dyna.actor.params & 0x3F);
    } else {
        Flags_SetSwitch(globalCtx, self->dyna.actor.params & 0x3F);
    }
}

s32 func_80BA2218(ObjWarp2block* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (func_80BA1ECC(self, globalCtx)) {
        if (player->stateFlags2 & 0x1000000) {
            func_8010BD58(globalCtx, 1);
            self->func_168 = func_80BA228C;
        } else {
            player->stateFlags2 |= 0x800000;
        }
    }

    return 0;
}

s32 func_80BA228C(ObjWarp2block* self, GlobalContext* globalCtx) {
    if (globalCtx->msgCtx.unk_E3EE == 4) {
        self->func_168 = func_80BA2218;
    }

    if (globalCtx->msgCtx.unk_E3EC == 0xA) {
        if (self->unk_172 == 0xFE) {
            self->unk_16E = 0x6E;
        } else {
            self->unk_16E--;
            if (self->unk_16E == 0) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80BA2304(ObjWarp2block* self, GlobalContext* globalCtx) {
    s32 ret = self->func_168(self, globalCtx);
    self->unk_172 = globalCtx->msgCtx.unk_E3EC;

    return ret;
}

void ObjWarp2block_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    ObjWarp2block* self = THIS;
    CollisionHeader* collisionHeader;

    collisionHeader = NULL;
    self->dyna.actor.world.rot.z = self->dyna.actor.shape.rot.z = 0;
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);

    Actor_SetScale(&self->dyna.actor, sSpawnData[(self->dyna.actor.params >> 8) & 1].scale);
    self->func_168 = func_80BA2218;
    Actor_SetFocus(&self->dyna.actor, sSpawnData[(self->dyna.actor.params >> 8) & 1].focus);

    if ((self->dyna.actor.params >> 0xF) & 1) {
        func_80BA24E8(self);
        if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
            self->dyna.actor.draw = NULL;
        }
        DynaPolyActor_Init(&self->dyna, 0);
        CollisionHeader_GetVirtual(&gSongOfTimeBlockCol, &collisionHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, collisionHeader);
    } else {
        ObjWarp2block_SetInactive(self);
    }

    osSyncPrintf("時のブロック(ワープ２) (<arg> %04xH <type> color:%d range:%d)\n", self->dyna.actor.params & 0xFFFF,
                 self->dyna.actor.home.rot.z & 7, (self->dyna.actor.params >> 0xB) & 7);
}

void ObjWarp2block_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ObjWarp2block* self = THIS;
    if ((self->dyna.actor.params >> 0xF) & 1) {
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
}

void ObjWarp2block_SetInactive(ObjWarp2block* self) {
    self->actionFunc = ObjWarp2block_DoNothing;
    self->dyna.actor.draw = NULL;
}

void ObjWarp2block_DoNothing(ObjWarp2block* self, GlobalContext* globalCtx) {
}

void func_80BA24E8(ObjWarp2block* self) {
    self->actionFunc = func_80BA24F8;
}

void func_80BA24F8(ObjWarp2block* self, GlobalContext* globalCtx) {
    Actor* current = globalCtx->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;

    while (current != NULL) {
        if (current->id == ACTOR_OBJ_WARP2BLOCK && !((current->params >> 0xF) & 1) &&
            ((self->dyna.actor.params & 0x3F) == (current->params & 0x3F))) {
            self->dyna.actor.child = current;
            if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
                ObjWarp2block_SwapWithChild(self, globalCtx);
                self->dyna.actor.draw = ObjWarp2block_Draw;
            }
            func_80BA2600(self);
            return;
        }

        current = current->next;
    }

    self->unk_174++;
    if (self->unk_174 > 60) {
        osSyncPrintf(VT_COL(RED, WHITE));
        osSyncPrintf("Error : 時のブロック(ワープ２)が対でセットされていません(%s %d)\n", "../z_obj_warp2block.c", 505);
        osSyncPrintf(VT_RST);
        Actor_Kill(&self->dyna.actor);
    }
}

void func_80BA2600(ObjWarp2block* self) {
    self->actionFunc = func_80BA2610;
}

void func_80BA2610(ObjWarp2block* self, GlobalContext* globalCtx) {
    if ((func_80BA2304(self, globalCtx) != 0) && (self->unk_16C <= 0)) {
        ObjWarp2block_Spawn(self, globalCtx);
        self->unk_16C = 0xA0;
        OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        self->unk_170 = 0xC;
    }

    if (self->unk_170 > 0) {
        self->unk_170--;
        if (self->unk_170 == 0) {
            ObjWarp2block_SwapWithChild(self, globalCtx);
        }
    }
    if (self->unk_16C == 0x32) {
        func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
    }
}

void ObjWarp2block_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjWarp2block* self = THIS;

    self->actionFunc(self, globalCtx);
    if (self->unk_16C > 0) {
        self->unk_16C--;
    }
}

void ObjWarp2block_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Color_RGB8* sp44;

    sp44 = &sColors[thisx->home.rot.z & 7];

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_warp2block.c", 584);
    func_80093D18(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_warp2block.c", 588),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, sp44->r, sp44->g, sp44->b, 255);
    gSPDisplayList(POLY_OPA_DISP++, gSongOfTimeBlockDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_warp2block.c", 594);
}
