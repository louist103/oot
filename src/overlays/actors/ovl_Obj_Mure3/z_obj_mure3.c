/*
 * File: z_obj_mure3.c
 * Overlay: ovl_Obj_Mure3
 * Description: Tower of Rupees
 */

#include "z_obj_mure3.h"

#define FLAGS 0x00000000

#define THIS ((ObjMure3*)thisx)

void ObjMure3_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjMure3_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjMure3_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80B9AF24(ObjMure3* self);
void func_80B9AF34(ObjMure3* self, GlobalContext* globalCtx);
void func_80B9AF54(ObjMure3* self);
void func_80B9AF64(ObjMure3* self, GlobalContext* globalCtx);
void func_80B9AFEC(ObjMure3* self);
void func_80B9AFFC(ObjMure3* self, GlobalContext* globalCtx);

const ActorInit Obj_Mure3_InitVars = {
    ACTOR_OBJ_MURE3,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjMure3),
    (ActorFunc)ObjMure3_Init,
    (ActorFunc)ObjMure3_Destroy,
    (ActorFunc)ObjMure3_Update,
    NULL,
};

static s16 sRupeeCounts[] = { 5, 5, 7, 0 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 100, ICHAIN_STOP),
};

void func_80B9A9D0(ObjMure3* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3f spawnPos;

    Math_Vec3f_Copy(&spawnPos, &self->actor.world.pos);
    for (i = 0; i < 5; i++, spawnPos.y += 20.0f) {
        if (!((self->unk_16C >> i) & 1)) {
            self->unk_150[i] = Item_DropCollectible2(globalCtx, &spawnPos, 0x4000 | ITEM00_RUPEE_BLUE);
            if (self->unk_150[i] != NULL) {
                self->unk_150[i]->actor.room = self->actor.room;
            }
        }
    }
}

void func_80B9AA90(ObjMure3* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3f spawnPos;
    f32 sn = Math_SinS(self->actor.world.rot.y);
    f32 cos = Math_CosS(self->actor.world.rot.y);
    f32 radius;

    spawnPos.y = self->actor.world.pos.y;

    for (i = 0, radius = -40.0f; i < 5; i++, radius += 20.0f) {
        if (!((self->unk_16C >> i) & 1)) {
            spawnPos.x = self->actor.world.pos.x + (sn * radius);
            spawnPos.z = self->actor.world.pos.z + (cos * radius);
            self->unk_150[i] = Item_DropCollectible2(globalCtx, &spawnPos, 0x4000 | ITEM00_RUPEE_GREEN);
            if (self->unk_150[i] != NULL) {
                self->unk_150[i]->actor.room = self->actor.room;
            }
        }
    }
}

void func_80B9ABA0(ObjMure3* self, GlobalContext* globalCtx) {
    s32 i;
    Vec3f spawnPos;
    s16 yRot;

    spawnPos.y = self->actor.world.pos.y;
    yRot = self->actor.world.rot.y;
    for (i = 0; i < 6; i++) {
        if (!((self->unk_16C >> i) & 1)) {
            spawnPos.x = (Math_SinS(yRot) * 40.0f) + self->actor.world.pos.x;
            spawnPos.z = (Math_CosS(yRot) * 40.0f) + self->actor.world.pos.z;
            self->unk_150[i] = Item_DropCollectible2(globalCtx, &spawnPos, 0x4000 | ITEM00_RUPEE_GREEN);
            if (self->unk_150[i] != NULL) {
                self->unk_150[i]->actor.room = self->actor.room;
            }
        }
        yRot += 0x2AAA;
    }
    if (!((self->unk_16C >> 6) & 1)) {
        spawnPos.x = self->actor.world.pos.x;
        spawnPos.z = self->actor.world.pos.z;
        self->unk_150[6] = Item_DropCollectible2(globalCtx, &spawnPos, 0x4000 | ITEM00_RUPEE_RED);
        if (self->unk_150[6] != NULL) {
            self->unk_150[6]->actor.room = self->actor.room;
        }
    }
}

void func_80B9ACE4(ObjMure3* self, GlobalContext* globalCtx) {
    s16 count = sRupeeCounts[(self->actor.params >> 13) & 7];
    s32 i;

    for (i = 0; i < count; i++) {
        EnItem00** collectible = &self->unk_150[i];

        if (!((self->unk_16C >> i) & 1) && (*collectible != NULL)) {
            if (Actor_HasParent(&(*collectible)->actor, globalCtx) || ((*collectible)->actor.update == NULL)) {
                self->unk_16C |= (1 << i);
            } else {
                Actor_Kill(&(*collectible)->actor);
            }
        }
        *collectible = NULL;
    }
}

void func_80B9ADCC(ObjMure3* self, GlobalContext* globalCtx) {
    s16 count = sRupeeCounts[(self->actor.params >> 13) & 7];
    s32 i;

    for (i = 0; i < count; i++) {
        EnItem00** collectible = &self->unk_150[i];

        if ((*collectible != NULL) && !((self->unk_16C >> i) & 1)) {
            if (Actor_HasParent(&(*collectible)->actor, globalCtx)) {
                Flags_SetSwitch(globalCtx, self->actor.params & 0x3F);
            }
            if ((*collectible)->actor.update == NULL) {
                self->unk_16C |= (1 << i);
                self->unk_150[i] = NULL;
            }
        }
    }
}

void ObjMure3_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    ObjMure3* self = THIS;

    if (Flags_GetSwitch(globalCtx, self->actor.params & 0x3F)) {
        Actor_Kill(&self->actor);
        return;
    }
    Actor_ProcessInitChain(&self->actor, sInitChain);
    func_80B9AF24(self);
}

void ObjMure3_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void func_80B9AF24(ObjMure3* self) {
    self->actionFunc = func_80B9AF34;
}

void func_80B9AF34(ObjMure3* self, GlobalContext* globalCtx) {
    func_80B9AF54(self);
}

void func_80B9AF54(ObjMure3* self) {
    self->actionFunc = func_80B9AF64;
}

void func_80B9AF64(ObjMure3* self, GlobalContext* globalCtx) {
    static ObjMure3SpawnFunc spawnFuncs[] = { func_80B9A9D0, func_80B9AA90, func_80B9ABA0 };

    if (Math3D_Dist1DSq(self->actor.projectedPos.x, self->actor.projectedPos.z) < SQ(1150.0f)) {
        self->actor.flags |= 0x10;
        spawnFuncs[(self->actor.params >> 13) & 7](self, globalCtx);
        func_80B9AFEC(self);
    }
}

void func_80B9AFEC(ObjMure3* self) {
    self->actionFunc = func_80B9AFFC;
}

void func_80B9AFFC(ObjMure3* self, GlobalContext* globalCtx) {
    func_80B9ADCC(self, globalCtx);
    if (Math3D_Dist1DSq(self->actor.projectedPos.x, self->actor.projectedPos.z) >= SQ(1450.0f)) {
        self->actor.flags &= ~0x10;
        func_80B9ACE4(self, globalCtx);
        func_80B9AF54(self);
    }
}

void ObjMure3_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjMure3* self = THIS;

    self->actionFunc(self, globalCtx);
}
