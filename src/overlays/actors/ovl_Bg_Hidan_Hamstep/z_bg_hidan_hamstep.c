/*
 * File: z_bg_hidan_hamstep.c
 * Overlay: ovl_Bg_Hidan_Hamstep
 * Description: Stone Steps and Platforms (Fire Temple)
 */

#include "z_bg_hidan_hamstep.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgHidanHamstep*)thisx)

void BgHidanHamstep_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanHamstep_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanHamstep_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanHamstep_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808887C4(BgHidanHamstep* self, GlobalContext* globalCtx);
void func_80888860(BgHidanHamstep* self, GlobalContext* globalCtx);
void func_808889B8(BgHidanHamstep* self, GlobalContext* globalCtx);
void func_80888A58(BgHidanHamstep* self, GlobalContext* globalCtx);
void BgHidanHamstep_DoNothing(BgHidanHamstep* self, GlobalContext* globalCtx);

static f32 sYPosOffsets[] = {
    -20.0f, -120.0f, -220.0f, -320.0f, -420.0f,
};

static ColliderTrisElementInit sTrisElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { -20.0f, 3.0f, -20.0f }, { -20.0f, 3.0f, 20.0f }, { 20.0f, 3.0f, 20.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 20.0f, 3.0f, 20.0f }, { 20.0f, 3.0f, -20.0f }, { -20.0f, 3.0f, -20.0f } } },
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
    2,
    sTrisElementsInit,
};

const ActorInit Bg_Hidan_Hamstep_InitVars = {
    ACTOR_BG_HIDAN_HAMSTEP,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanHamstep),
    (ActorFunc)BgHidanHamstep_Init,
    (ActorFunc)BgHidanHamstep_Destroy,
    (ActorFunc)BgHidanHamstep_Update,
    (ActorFunc)BgHidanHamstep_Draw,
};

static BgHidanHamstepActionFunc sActionFuncs[] = {
    func_808887C4, func_80888860, func_808889B8, func_80888A58, BgHidanHamstep_DoNothing,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static f32 sEffectPositions[][2] = {
    { -100.0f, 40.0f },  { 100.0f, 40.0f }, { -100.0f, 0.0f },   { 100.0f, 0.0f },
    { -100.0f, -40.0f }, { 100.0f, 40.0f }, { -100.0f, -80.0f }, { -50.0f, -80.0f },
    { 0.0f, -80.0f },    { 50.0f, -80.0f }, { 100.0f, -80.0f },
};

void BgHidanHamstep_SetupAction(BgHidanHamstep* self, s32 action) {
    self->action = action;
    self->actionFunc = sActionFuncs[action];
}

s32 BgHidanHamstep_SpawnChildren(BgHidanHamstep* self, GlobalContext* globalCtx2) {
    BgHidanHamstep* step = self;
    s32 i;
    Vec3f pos;
    f32 sin;
    f32 cos;
    s16 params;
    GlobalContext* globalCtx = globalCtx2;

    pos = pos; // Required to match
    pos.y = self->dyna.actor.home.pos.y - 100.0f;
    sin = Math_SinS(self->dyna.actor.shape.rot.y + 0x8000);
    cos = Math_CosS(self->dyna.actor.shape.rot.y + 0x8000);

    for (i = 0; i < 5; i++) {
        pos.x = (((i * 160.0f) + 60.0f) * sin) + self->dyna.actor.home.pos.x;
        pos.z = (((i * 160.0f) + 60.0f) * cos) + self->dyna.actor.home.pos.z;

        params = (i + 1) & 0xFF;
        params |= (self->dyna.actor.params & 0xFF00);

        step = (BgHidanHamstep*)Actor_SpawnAsChild(
            &globalCtx->actorCtx, &step->dyna.actor, globalCtx, ACTOR_BG_HIDAN_HAMSTEP, pos.x, pos.y, pos.z,
            self->dyna.actor.world.rot.x, self->dyna.actor.world.rot.y, self->dyna.actor.world.rot.z, params);

        if (step == NULL) {
            return 0;
        }
    }
    return 1;
}

void BgHidanHamstep_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanHamstep* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;
    Vec3f sp48[3];
    s32 i;
    s32 i2;
    BgHidanHamstep* step;

    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);

    if ((self->dyna.actor.params & 0xFF) == 0) {
        Collider_InitTris(globalCtx, &self->collider);
        Collider_SetTris(globalCtx, &self->collider, &self->dyna.actor, &sTrisInit, self->colliderItems);

        for (i = 0; i < 2; i++) {
            for (i2 = 0; i2 < 3; i2++) {
                sp48[i2].x = sTrisInit.elements[i].dim.vtx[i2].x + self->dyna.actor.home.pos.x;
                sp48[i2].y = sTrisInit.elements[i].dim.vtx[i2].y + self->dyna.actor.home.pos.y;
                sp48[i2].z = sTrisInit.elements[i].dim.vtx[i2].z + self->dyna.actor.home.pos.z;
            }
            Collider_SetTrisVertices(&self->collider, i, &sp48[0], &sp48[1], &sp48[2]);
        }
    }

    if ((self->dyna.actor.params & 0xFF) == 0) {
        CollisionHeader_GetVirtual(&gFireTempleStoneStep1Col, &colHeader);
    } else {
        CollisionHeader_GetVirtual(&gFireTempleStoneStep2Col, &colHeader);
    }

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0xFF)) {
        if ((self->dyna.actor.params & 0xFF) == 0) {
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + (-20.0f);
            BgHidanHamstep_SetupAction(self, 4);
        } else {
            self->dyna.actor.world.pos.y =
                sYPosOffsets[(self->dyna.actor.params & 0xFF) - 1] + self->dyna.actor.home.pos.y;
            BgHidanHamstep_SetupAction(self, 4);
        }
    } else if ((self->dyna.actor.params & 0xFF) == 0) {
        BgHidanHamstep_SetupAction(self, 0);
    } else {
        BgHidanHamstep_SetupAction(self, 2);
    }

    self->dyna.actor.gravity = -1.2f;
    self->dyna.actor.minVelocityY = -12.0f;

    if ((self->dyna.actor.params & 0xFF) == 0) {
        // "Fire Temple Object [Hammer Step] appears"
        osSyncPrintf("◯◯◯炎の神殿オブジェクト【ハンマーステップ】出現\n");
        if (BgHidanHamstep_SpawnChildren(self, globalCtx) == 0) {
            step = self;

            // "[Hammer Step] I can't create a step!"
            osSyncPrintf("【ハンマーステップ】 足場産れない！！\n");
            osSyncPrintf("%s %d\n", "../z_bg_hidan_hamstep.c", 425);

            while (step != NULL) {
                Actor_Kill(&step->dyna.actor);
                step = (BgHidanHamstep*)step->dyna.actor.child;
            }
        }
    }
}

void BgHidanHamstep_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanHamstep* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);

    if ((self->dyna.actor.params & 0xFF) == 0) {
        Collider_DestroyTris(globalCtx, &self->collider);
    }
}

void func_808884C8(BgHidanHamstep* step, GlobalContext* globalCtx) {
    Vec3f pos = step->dyna.actor.world.pos;
    s32 i;
    f32 sin;
    f32 cos;

    pos.y -= 20.0f;

    func_80033480(globalCtx, &pos, 0.0f, 0, 600, 300, 0);

    sin = Math_SinS(step->dyna.actor.shape.rot.y + 0x8000);
    cos = Math_CosS(step->dyna.actor.shape.rot.y + 0x8000);

    pos.y = step->dyna.actor.world.pos.y;

    for (i = 0; i < ARRAY_COUNT(sEffectPositions); i++) {
        pos.x = (sEffectPositions[i][1] * sin) + (sEffectPositions[i][0] * cos) + step->dyna.actor.world.pos.x;
        pos.z = ((sEffectPositions[i][1] * cos) - (sEffectPositions[i][0] * sin)) + step->dyna.actor.world.pos.z;
        func_80033480(globalCtx, &pos, 0.0f, 0, 150, 150, 0);
    }
}

void func_80888638(BgHidanHamstep* self, GlobalContext* globalCtx) {
    BgHidanHamstep* child = (BgHidanHamstep*)self->dyna.actor.child;

    while (child != NULL) {
        if ((child->dyna.actor.params & 0xFF) != 0) {
            func_808884C8(child, globalCtx);
        }
        child = (BgHidanHamstep*)child->dyna.actor.child;
    }
}

void func_80888694(BgHidanHamstep* self, BgHidanHamstep* parent) {
    BgHidanHamstep* child;

    if ((self->dyna.actor.params & 0xFF) >= 2) {
        if (parent->dyna.actor.world.pos.y < self->dyna.actor.world.pos.y) {
            self->dyna.actor.world.pos.y = parent->dyna.actor.world.pos.y;
        } else if ((self->dyna.actor.world.pos.y - parent->dyna.actor.world.pos.y) < -100.0f) {
            self->dyna.actor.world.pos.y = parent->dyna.actor.world.pos.y - 100.0f;
        }
    }

    child = (BgHidanHamstep*)self->dyna.actor.child;

    while (child != NULL) {
        if (self->dyna.actor.world.pos.y < child->dyna.actor.world.pos.y) {
            child->dyna.actor.world.pos.y = self->dyna.actor.world.pos.y;
        }
        child = (BgHidanHamstep*)child->dyna.actor.child;
    }
}

void func_80888734(BgHidanHamstep* self) {
    BgHidanHamstep* parent = (BgHidanHamstep*)self->dyna.actor.parent;
    f32 frameDivisor = R_UPDATE_RATE * 0.5f;

    if (parent != NULL) {
        self->dyna.actor.velocity.y = parent->dyna.actor.velocity.y;

        if ((self->dyna.actor.params & 0xFF) == 1) {
            self->dyna.actor.world.pos.y = parent->dyna.actor.world.pos.y - 100.0f;
        } else {
            self->dyna.actor.world.pos.y += (self->dyna.actor.velocity.y * frameDivisor);
        }

        func_80888694(self, parent);
    }
}

void func_808887C4(BgHidanHamstep* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        OnePointCutscene_Init(globalCtx, 3310, 100, &self->dyna.actor, MAIN_CAM);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_HAMMER_SWITCH);
        self->collider.base.acFlags = AC_NONE;
        BgHidanHamstep_SetupAction(self, 1);
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0xFF);
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void func_80888860(BgHidanHamstep* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;
    s32 quakeIndex;

    Actor_MoveForward(&self->dyna.actor);

    if (((self->dyna.actor.world.pos.y - self->dyna.actor.home.pos.y) < (-20.0f - self->dyna.actor.minVelocityY)) &&
        (self->dyna.actor.velocity.y <= 0.0f)) {
        self->unk_244++;

        if (self->unk_244 >= 7) {
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + -20.0f;
            BgHidanHamstep_SetupAction(self, 4);
        } else {
            self->dyna.actor.velocity.y *= -0.24f;

            if (1) {}

            if (self->unk_244 == 1) {
                quakeIndex = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
                Quake_SetSpeed(quakeIndex, -15536);
                Quake_SetQuakeValues(quakeIndex, 0, 0, 500, 0);
                Quake_SetCountdown(quakeIndex, 20);
                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
                func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 255, 20, 150);
                func_80888638(self, globalCtx);
                osSyncPrintf("A(%d)\n", self->dyna.actor.params);
            }
        }
    }
}

void func_808889B8(BgHidanHamstep* self, GlobalContext* globalCtx) {
    s32 pad;
    BgHidanHamstep* parent = (BgHidanHamstep*)self->dyna.actor.parent;

    func_80888734(self);

    if ((parent->action == 4) || ((parent->action == 3) && (parent->unk_244 >= 5))) {
        if ((self->dyna.actor.params & 0xFF) == 1) {
            self->dyna.actor.world.pos.y =
                sYPosOffsets[(self->dyna.actor.params & 0xFF) - 1] + self->dyna.actor.home.pos.y;
            BgHidanHamstep_SetupAction(self, 4);
        } else {
            BgHidanHamstep_SetupAction(self, 3);
        }
    }
}

void func_80888A58(BgHidanHamstep* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;
    s32 quakeIndex;

    Actor_MoveForward(&self->dyna.actor);
    func_80888694(self, (BgHidanHamstep*)self->dyna.actor.parent);

    if (((self->dyna.actor.params & 0xFF) <= 0) || ((self->dyna.actor.params & 0xFF) >= 6)) {
        // "[Hammer Step] arg_data strange (arg_data = %d)"
        osSyncPrintf("【ハンマーステップ】 arg_data おかしい (arg_data = %d)", self->dyna.actor.params);
        osSyncPrintf("%s %d\n", "../z_bg_hidan_hamstep.c", 696);
    }

    if (((self->dyna.actor.world.pos.y - self->dyna.actor.home.pos.y) <=
         sYPosOffsets[(self->dyna.actor.params & 0xFF) - 1]) &&
        (self->dyna.actor.velocity.y <= 0.0f)) {
        self->unk_244++;

        if (self->unk_244 >= 7) {
            self->dyna.actor.world.pos.y =
                sYPosOffsets[(self->dyna.actor.params & 0xFF) - 1] + self->dyna.actor.home.pos.y;
            BgHidanHamstep_SetupAction(self, 3);
        } else {
            self->dyna.actor.velocity.y *= -0.24f;

            if (1) {}

            if (self->unk_244 == 1) {
                quakeIndex = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
                Quake_SetSpeed(quakeIndex, -15536);
                Quake_SetQuakeValues(quakeIndex, 20, 1, 0, 0);
                Quake_SetCountdown(quakeIndex, 7);

                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
                func_800AA000(10000.0f, 255, 20, 150);
                func_808884C8(self, globalCtx);

                if ((self->dyna.actor.params & 0xFF) == 5) {
                    func_80078884(NA_SE_SY_CORRECT_CHIME);
                }

                osSyncPrintf("B(%d)\n", self->dyna.actor.params);
            }
        }
    }
}

void BgHidanHamstep_DoNothing(BgHidanHamstep* self, GlobalContext* globalCtx) {
}

void BgHidanHamstep_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanHamstep* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgHidanHamstep_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_hamstep.c", 782);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_hidan_hamstep.c", 787),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if ((thisx->params & 0xFF) == 0) {
        gSPDisplayList(POLY_OPA_DISP++, gFireTempleStoneStep1DL);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, gFireTempleStoneStep2DL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_hamstep.c", 796);
}
