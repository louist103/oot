/*
 * File: z_obj_mure.c
 * Overlay: ovl_Obj_Mure
 * Description: Spawns Fish, Bug, Butterfly
 */

#include "z_obj_mure.h"

#define FLAGS 0x00000000

#define THIS ((ObjMure*)thisx)

void ObjMure_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjMure_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjMure_Update(Actor* thisx, GlobalContext* globalCtx);

void ObjMure_InitialAction(ObjMure* self, GlobalContext* globalCtx);
void ObjMure_CulledState(ObjMure* self, GlobalContext* globalCtx);
void ObjMure_ActiveState(ObjMure* self, GlobalContext* globalCtx);

s32 ObjMure_GetMaxChildSpawns(ObjMure* self);

const ActorInit Obj_Mure_InitVars = {
    ACTOR_OBJ_MURE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjMure),
    (ActorFunc)ObjMure_Init,
    (ActorFunc)ObjMure_Destroy,
    (ActorFunc)ObjMure_Update,
    NULL,
};

typedef enum {
    /* 0 */ OBJMURE_TYPE_GRASS,
    /* 1 */ OBJMURE_TYPE_UNDEFINED,
    /* 2 */ OBJMURE_TYPE_FISH,
    /* 3 */ OBJMURE_TYPE_BUGS,
    /* 4 */ OBJMURE_TYPE_BUTTERFLY
} ObjMureType;

typedef enum {
    /* 0 */ OBJMURE_CHILD_STATE_0,
    /* 1 */ OBJMURE_CHILD_STATE_1, // Dead
    /* 2 */ OBJMURE_CHILD_STATE_2
} ObjMureChildState;

static f32 sZClip[] = { 1600.0f, 1600.0f, 1000.0f, 1000.0f, 1000.0f };

static s32 sMaxChildSpawns[] = { 12, 9, 8, 0 };

static s16 sSpawnActorIds[] = { ACTOR_EN_KUSA, 0, ACTOR_EN_FISH, ACTOR_EN_INSECT, ACTOR_EN_BUTTE };

static s16 sSpawnParams[] = { 0, 2, -1, 0, -1 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_STOP),
};

s32 ObjMure_SetCullingImpl(Actor* thisx, GlobalContext* globalCtx) {
    ObjMure* self = THIS;
    s32 result;

    switch (self->type) {
        case OBJMURE_TYPE_FISH:
        case OBJMURE_TYPE_BUGS:
        case OBJMURE_TYPE_BUTTERFLY:
            Actor_ProcessInitChain(&self->actor, sInitChain);
            result = true;
            break;
        default:
            // "Error : Culling is not set.(%s %d)(arg_data 0x%04x)"
            osSyncPrintf("Error : カリングの設定がされていません。(%s %d)(arg_data 0x%04x)\n", "../z_obj_mure.c", 204,
                         self->actor.params);
            return false;
    }
    return result;
}

s32 ObjMure_SetCulling(Actor* thisx, GlobalContext* globalCtx) {
    if (!ObjMure_SetCullingImpl(thisx, globalCtx)) {
        return false;
    }
    return true;
}

void ObjMure_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjMure* self = THIS;

    self->chNum = (thisx->params >> 0xC) & 0x0F;
    self->ptn = (thisx->params >> 8) & 0x07;
    self->svNum = (thisx->params >> 5) & 0x03;
    self->type = thisx->params & 0x1F;

    if (self->ptn >= 4) {
        osSyncPrintf("Error 群れな敵 (%s %d)(arg_data 0x%04x)\n", "../z_obj_mure.c", 237, thisx->params);
        Actor_Kill(&self->actor);
        return;
    } else if (self->type >= 5) {
        osSyncPrintf("Error 群れな敵 (%s %d)(arg_data 0x%04x)\n", "../z_obj_mure.c", 245, thisx->params);
        Actor_Kill(&self->actor);
        return;
    } else if (!ObjMure_SetCulling(thisx, globalCtx)) {
        Actor_Kill(&self->actor);
        return;
    }
    self->actionFunc = ObjMure_InitialAction;
    osSyncPrintf("群れな敵 (arg_data 0x%04x)(chNum(%d) ptn(%d) svNum(%d) type(%d))\n", thisx->params, self->chNum,
                 self->ptn, self->svNum, self->type);
    if (ObjMure_GetMaxChildSpawns(self) <= 0) {
        osSyncPrintf("Warning : 個体数が設定されていません(%s %d)(arg_data 0x%04x)\n", "../z_obj_mure.c", 268,
                     thisx->params);
    }
}

void ObjMure_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

s32 ObjMure_GetMaxChildSpawns(ObjMure* self) {
    if (self->chNum == 0) {
        return sMaxChildSpawns[self->ptn];
    }
    return self->chNum;
}

void ObjMure_GetSpawnPos(Vec3f* outPos, Vec3f* inPos, s32 ptn, s32 idx) {
    if (ptn >= 4) {
        osSyncPrintf("おかしなの (%s %d)\n", "../z_obj_mure.c", 307);
    }
    *outPos = *inPos;
}

void ObjMure_SpawnActors0(ObjMure* self, GlobalContext* globalCtx) {
    ActorContext* ac;
    s32 i;
    Vec3f pos;
    s32 pad;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);

    for (i = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            // "Error: I already have a child(%s %d)(arg_data 0x%04x)"
            osSyncPrintf("Error : 既に子供がいる(%s %d)(arg_data 0x%04x)\n", "../z_obj_mure.c", 333,
                         self->actor.params);
        }
        switch (self->childrenStates[i]) {
            case OBJMURE_CHILD_STATE_1:
                break;
            case OBJMURE_CHILD_STATE_2:
                ac = &globalCtx->actorCtx;
                ObjMure_GetSpawnPos(&pos, &self->actor.world.pos, self->ptn, i);
                self->children[i] =
                    Actor_Spawn(ac, globalCtx, sSpawnActorIds[self->type], pos.x, pos.y, pos.z, self->actor.world.rot.x,
                                self->actor.world.rot.y, self->actor.world.rot.z, sSpawnParams[self->type]);
                if (self->children[i] != NULL) {
                    self->children[i]->flags |= 0x800;
                    self->children[i]->room = self->actor.room;
                } else {
                    osSyncPrintf("warning 発生失敗 (%s %d)\n", "../z_obj_mure.c", 359);
                }
                break;
            default:
                ac = &globalCtx->actorCtx;
                ObjMure_GetSpawnPos(&pos, &self->actor.world.pos, self->ptn, i);
                self->children[i] =
                    Actor_Spawn(ac, globalCtx, sSpawnActorIds[self->type], pos.x, pos.y, pos.z, self->actor.world.rot.x,
                                self->actor.world.rot.y, self->actor.world.rot.z, sSpawnParams[self->type]);
                if (self->children[i] != NULL) {
                    self->children[i]->room = self->actor.room;
                } else {
                    osSyncPrintf("warning 発生失敗 (%s %d)\n", "../z_obj_mure.c", 382);
                }
                break;
        }
    }
}

void ObjMure_SpawnActors1(ObjMure* self, GlobalContext* globalCtx) {
    ActorContext* ac = (ActorContext*)globalCtx; // fake match
    Actor* actor = &self->actor;
    Vec3f spawnPos;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            osSyncPrintf("Error : 既に子供がいる(%s %d)(arg_data 0x%04x)\n", "../z_obj_mure.c", 407, actor->params);
        }
        ac = &globalCtx->actorCtx;
        ObjMure_GetSpawnPos(&spawnPos, &actor->world.pos, self->ptn, i);
        self->children[i] = Actor_Spawn(ac, globalCtx, sSpawnActorIds[self->type], spawnPos.x, spawnPos.y, spawnPos.z,
                                        actor->world.rot.x, actor->world.rot.y, actor->world.rot.z,
                                        (self->type == 4 && i == 0) ? 1 : sSpawnParams[self->type]);
        if (self->children[i] != NULL) {
            self->childrenStates[i] = OBJMURE_CHILD_STATE_0;
            self->children[i]->room = actor->room;
        } else {
            self->childrenStates[i] = OBJMURE_CHILD_STATE_1;
            osSyncPrintf("warning 発生失敗 (%s %d)\n", "../z_obj_mure.c", 438);
        }
    }
}

void ObjMure_SpawnActors(ObjMure* self, GlobalContext* globalCtx) {
    switch (self->svNum) {
        case 0:
            ObjMure_SpawnActors0(self, globalCtx);
            break;
        case 1:
            ObjMure_SpawnActors1(self, globalCtx);
            break;
    }
}

void ObjMure_KillActorsImpl(ObjMure* self, GlobalContext* globalCtx) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        switch (self->childrenStates[i]) {
            case OBJMURE_CHILD_STATE_1:
                self->children[i] = NULL;
                break;
            case OBJMURE_CHILD_STATE_2:
                if (self->children[i] != NULL) {
                    Actor_Kill(self->children[i]);
                    self->children[i] = NULL;
                }
                break;
            default:
                if (self->children[i] != NULL) {
                    if (Actor_HasParent(self->children[i], globalCtx)) {
                        self->children[i] = NULL;
                    } else {
                        Actor_Kill(self->children[i]);
                        self->children[i] = NULL;
                    }
                }
                break;
        }
    }
}

void ObjMure_KillActors(ObjMure* self, GlobalContext* globalCtx) {
    ObjMure_KillActorsImpl(self, globalCtx);
}

void ObjMure_CheckChildren(ObjMure* self, GlobalContext* globalCtx) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            if (self->childrenStates[i] == OBJMURE_CHILD_STATE_0) {
                if (self->children[i]->update != NULL) {
                    if (self->children[i]->flags & 0x800) {
                        self->childrenStates[i] = OBJMURE_CHILD_STATE_2;
                    }
                } else {
                    self->childrenStates[i] = OBJMURE_CHILD_STATE_1;
                    self->children[i] = NULL;
                }
            } else if (self->childrenStates[i] == OBJMURE_CHILD_STATE_2 && self->children[i]->update == NULL) {
                self->childrenStates[i] = OBJMURE_CHILD_STATE_1;
                self->children[i] = NULL;
            }
        }
    }
}

void ObjMure_InitialAction(ObjMure* self, GlobalContext* globalCtx) {
    self->actionFunc = ObjMure_CulledState;
}

void ObjMure_CulledState(ObjMure* self, GlobalContext* globalCtx) {
    if (fabsf(self->actor.projectedPos.z) < sZClip[self->type]) {
        self->actionFunc = ObjMure_ActiveState;
        self->actor.flags |= 0x10;
        ObjMure_SpawnActors(self, globalCtx);
    }
}

void ObjMure_SetFollowTargets(ObjMure* self, f32 randMax) {
    s32 index;
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;

    for (i = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            self->children[i]->child = NULL;
            if (Rand_ZeroOne() <= randMax) {
                index = Rand_ZeroOne() * (maxChildren - 0.5f);
                if (i != index) {
                    self->children[i]->child = self->children[index];
                }
            }
        }
    }
}

/**
 * Selects a child that will follow after the player
 * `idx1` is the index + 1 of the child that will follow the player. If `idx1` is zero, no actor will follow the player
 */
void ObjMure_SetChildToFollowPlayer(ObjMure* self, s32 idx1) {
    s32 maxChildren = ObjMure_GetMaxChildSpawns(self);
    s32 i;
    s32 i2;
    s32 j;

    for (i = 0, i2 = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            if (i2 < idx1) {
                i2++;
                self->children[i]->child = self->children[i];
                for (j = 0; j < maxChildren; j++) {
                    if (i != j && self->children[j]->child == self->children[i]) {
                        self->children[j]->child = NULL;
                    }
                }
            } else if (self->children[i]->child == self->children[i]) {
                self->children[i]->child = NULL;
            }
        }
    }
}

// Fish, Bugs
void ObjMure_GroupBehavior0(ObjMure* self, GlobalContext* globalCtx) {
    if (self->unk_1A4 <= 0) {
        if (self->unk_1A6) {
            self->unk_1A6 = false;
            ObjMure_SetFollowTargets(self, (Rand_ZeroOne() * 0.5f) + 0.1f);
            if (self->actor.xzDistToPlayer < 60.0f) {
                self->unk_1A4 = (s16)(Rand_ZeroOne() * 5.5f) + 4;
            } else {
                self->unk_1A4 = (s16)(Rand_ZeroOne() * 40.5f) + 4;
            }
        } else {
            self->unk_1A6 = true;
            if (self->actor.xzDistToPlayer < 60.0f) {
                self->unk_1A4 = (s16)(Rand_ZeroOne() * 10.5f) + 4;
                ObjMure_SetFollowTargets(self, (Rand_ZeroOne() * 0.2f) + 0.8f);
            } else {
                self->unk_1A4 = (s16)(Rand_ZeroOne() * 10.5f) + 4;
                ObjMure_SetFollowTargets(self, (Rand_ZeroOne() * 0.2f) + 0.6f);
            }
        }
    }
    if (self->actor.xzDistToPlayer < 120.0f) {
        self->unk_1A8++;
    } else {
        self->unk_1A8 = 0;
    }
    if (self->unk_1A8 >= 80) {
        ObjMure_SetChildToFollowPlayer(self, 1);
    } else {
        ObjMure_SetChildToFollowPlayer(self, 0);
    }
}

// Butterflies
void ObjMure_GroupBehavior1(ObjMure* self, GlobalContext* globalCtx) {
    s32 maxChildren;
    s32 i;

    if (self->unk_1A4 <= 0) {
        if (self->unk_1A6) {
            self->unk_1A6 = false;
            ObjMure_SetFollowTargets(self, Rand_ZeroOne() * 0.2f);
            if (self->actor.xzDistToPlayer < 60.0f) {
                self->unk_1A4 = (s16)(Rand_ZeroOne() * 5.5f) + 4;
            } else {
                self->unk_1A4 = (s16)(Rand_ZeroOne() * 40.5f) + 4;
            }
        } else {
            self->unk_1A6 = true;
            ObjMure_SetFollowTargets(self, Rand_ZeroOne() * 0.7f);
            self->unk_1A4 = (s16)(Rand_ZeroOne() * 10.5f) + 4;
        }
    }

    maxChildren = ObjMure_GetMaxChildSpawns(self);
    for (i = 0; i < maxChildren; i++) {
        if (self->children[i] != NULL) {
            if (self->children[i]->child != NULL && self->children[i]->child->update == NULL) {
                self->children[i]->child = NULL;
            }
        }
    }
}

static ObjMureActionFunc sTypeGroupBehaviorFunc[] = {
    NULL, NULL, ObjMure_GroupBehavior0, ObjMure_GroupBehavior0, ObjMure_GroupBehavior1,
};

void ObjMure_ActiveState(ObjMure* self, GlobalContext* globalCtx) {
    ObjMure_CheckChildren(self, globalCtx);
    if (sZClip[self->type] + 40.0f <= fabsf(self->actor.projectedPos.z)) {
        self->actionFunc = ObjMure_CulledState;
        self->actor.flags &= ~0x10;
        ObjMure_KillActors(self, globalCtx);
    } else if (sTypeGroupBehaviorFunc[self->type] != NULL) {
        sTypeGroupBehaviorFunc[self->type](self, globalCtx);
    }
}

void ObjMure_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjMure* self = THIS;

    if (self->unk_1A4 > 0) {
        self->unk_1A4--;
    }
    self->actionFunc(self, globalCtx);
}
