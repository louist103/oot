/*
 * File: z_bg_haka_zou.c
 * Overlay: ovl_Bg_Haka_Zou
 * Description: Statue and Wall (Shadow Temple)
 */

#include "z_bg_haka_zou.h"
#include "objects/object_hakach_objects/object_hakach_objects.h"
#include "objects/object_haka_objects/object_haka_objects.h"

#define FLAGS 0x00000010

typedef enum {
    /* 0x0 */ STA_GIANT_BIRD_STATUE,
    /* 0x1 */ STA_BOMBABLE_SKULL_WALL,
    /* 0x2 */ STA_BOMBABLE_RUBBLE,
    /* 0x3 */ STA_UNKNOWN
} ShadowTempleAssetsType;

#define THIS ((BgHakaZou*)thisx)

void BgHakaZou_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaZou_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaZou_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaZou_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgHakaZou_Wait(BgHakaZou* self, GlobalContext* globalCtx);
void func_80882BDC(BgHakaZou* self, GlobalContext* globalCtx);
void func_80883000(BgHakaZou* self, GlobalContext* globalCtx);
void func_80883104(BgHakaZou* self, GlobalContext* globalCtx);
void func_80883144(BgHakaZou* self, GlobalContext* globalCtx);
void func_80883254(BgHakaZou* self, GlobalContext* globalCtx);
void func_80883328(BgHakaZou* self, GlobalContext* globalCtx);
void func_808834D8(BgHakaZou* self, GlobalContext* globalCtx);
void BgHakaZou_DoNothing(BgHakaZou* self, GlobalContext* globalCtx);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000008, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 5, 60, 0, { 0, 0, 0 } },
};

static Vec3f sZeroVec = { 0.0f, 0.0f, 0.0f };

const ActorInit Bg_Haka_Zou_InitVars = {
    ACTOR_BG_HAKA_ZOU,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHakaZou),
    (ActorFunc)BgHakaZou_Init,
    (ActorFunc)BgHakaZou_Destroy,
    (ActorFunc)BgHakaZou_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaZou_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgHakaZou* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);

    self->switchFlag = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;

    if (thisx->params == STA_UNKNOWN) {
        Actor_SetScale(thisx, (Rand_ZeroOne() * 0.005f) + 0.025f);

        thisx->speedXZ = Rand_ZeroOne();
        thisx->world.rot.y = thisx->shape.rot.y * ((Rand_ZeroOne() < 0.5f) ? -1 : 1) + Rand_CenteredFloat(0x1000);
        self->timer = 20;
        thisx->world.rot.x = Rand_S16Offset(0x100, 0x300) * ((Rand_ZeroOne() < 0.5f) ? -1 : 1);
        thisx->world.rot.z = Rand_S16Offset(0x400, 0x800) * ((Rand_ZeroOne() < 0.5f) ? -1 : 1);
    } else {
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, thisx, &sCylinderInit);
        Collider_UpdateCylinder(thisx, &self->collider);

        DynaPolyActor_Init(&self->dyna, 0);

        if (thisx->params == STA_GIANT_BIRD_STATUE) {
            thisx->uncullZoneForward = 2000.0f;
            thisx->uncullZoneScale = 3000.0f;
            thisx->uncullZoneDownward = 3000.0f;
        }
    }

    self->requiredObjBankIndex = (thisx->params == STA_BOMBABLE_RUBBLE)
                                     ? Object_GetIndex(&globalCtx->objectCtx, OBJECT_HAKACH_OBJECTS)
                                     : Object_GetIndex(&globalCtx->objectCtx, OBJECT_HAKA_OBJECTS);

    if (self->requiredObjBankIndex < 0) {
        Actor_Kill(thisx);
    } else if ((thisx->params != STA_UNKNOWN) && Flags_GetSwitch(globalCtx, self->switchFlag)) {
        if (thisx->params != STA_GIANT_BIRD_STATUE) {
            Actor_Kill(thisx);
        } else {
            thisx->shape.rot.x = -0x4000;
            thisx->world.pos.z -= 80.0f;
            thisx->world.pos.y -= 54.0f;
        }
    }

    self->actionFunc = BgHakaZou_Wait;
}

void BgHakaZou_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaZou* self = THIS;

    if (self->dyna.actor.params != STA_UNKNOWN) {
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void func_808828F4(BgHakaZou* self, GlobalContext* globalCtx) {
    Vec3f effectPos;
    Vec3f effectVelocity;
    f32 rand;
    s32 i;

    effectVelocity.x = 0.0f;
    effectVelocity.y = 1.0f;
    effectVelocity.z = 2.0f;

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            effectPos.x = self->dyna.actor.world.pos.x - (Rand_CenteredFloat(10.0f) + 112.0f);
        } else {
            effectPos.x = Rand_CenteredFloat(10.0f) + self->dyna.actor.world.pos.x;
        }

        rand = Rand_ZeroOne();
        effectPos.y = self->dyna.actor.world.pos.y + (60.0f * rand);
        effectPos.z = self->dyna.actor.world.pos.z + (112.0f * rand);

        func_800286CC(globalCtx, &effectPos, &effectVelocity, &sZeroVec, (Rand_ZeroOne() * 200.0f) + 1000.0f, 100);
    }
}

void BgHakaZou_Wait(BgHakaZou* self, GlobalContext* globalCtx) {
    CollisionHeader* colHeader;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->requiredObjBankIndex)) {
        self->dyna.actor.objBankIndex = self->requiredObjBankIndex;
        self->dyna.actor.draw = BgHakaZou_Draw;

        if (self->dyna.actor.params == STA_UNKNOWN) {
            self->actionFunc = func_80882BDC;
        } else {
            Actor_SetObjectDependency(globalCtx, &self->dyna.actor);

            colHeader = NULL;

            if (self->dyna.actor.params == STA_GIANT_BIRD_STATUE) {
                CollisionHeader_GetVirtual(&object_haka_objects_Col_006F70, &colHeader);
                self->collider.dim.radius = 80;
                self->collider.dim.height = 100;
                self->collider.dim.yShift = -30;
                self->collider.dim.pos.x -= 56;
                self->collider.dim.pos.z += 56;
                self->dyna.actor.uncullZoneScale = 1500.0f;
            } else if (self->dyna.actor.params == STA_BOMBABLE_SKULL_WALL) {
                CollisionHeader_GetVirtual(&object_haka_objects_Col_005E30, &colHeader);
                self->collider.dim.yShift = -50;
            } else {
                CollisionHeader_GetVirtual(&gBotwBombSpotCol, &colHeader);
                self->collider.dim.radius = 55;
                self->collider.dim.height = 20;
            }

            self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

            if ((self->dyna.actor.params == STA_GIANT_BIRD_STATUE) && Flags_GetSwitch(globalCtx, self->switchFlag)) {
                self->actionFunc = BgHakaZou_DoNothing;
            } else {
                self->actionFunc = func_80883000;
            }
        }
    }
}
void func_80882BDC(BgHakaZou* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    self->dyna.actor.shape.rot.x += self->dyna.actor.world.rot.x;
    self->dyna.actor.shape.rot.z += self->dyna.actor.world.rot.z;

    if (self->dyna.actor.bgCheckFlags & 2) {
        if (self->dyna.actor.velocity.y < -8.0f) {
            self->dyna.actor.velocity.y *= -0.6f;
            self->dyna.actor.velocity.y = CLAMP_MAX(self->dyna.actor.velocity.y, 10.0f);
            self->dyna.actor.bgCheckFlags &= ~3;
            self->dyna.actor.speedXZ = 2.0f;
        } else {
            Actor_Kill(&self->dyna.actor);
        }
    }

    if (self->timer == 0) {
        Actor_Kill(&self->dyna.actor);
    }
}

void func_80882CC4(BgHakaZou* self, GlobalContext* globalCtx) {
    s32 i;
    s32 j;
    Vec3f actorSpawnPos;
    f32 sin;
    f32 cos;
    s32 pad;

    sin = Math_SinS(self->dyna.actor.shape.rot.y - 0x4000) * 40.0f;
    cos = Math_CosS(self->dyna.actor.shape.rot.y - 0x4000) * 40.0f;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            actorSpawnPos.x = self->dyna.actor.world.pos.x + (j - 1) * sin;
            actorSpawnPos.z = self->dyna.actor.world.pos.z + (j - 1) * cos;
            actorSpawnPos.y = self->dyna.actor.world.pos.y + (i - 1) * 55;

            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_BG_HAKA_ZOU, actorSpawnPos.x, actorSpawnPos.y,
                        actorSpawnPos.z, 0, self->dyna.actor.shape.rot.y, 0, self->dyna.actor.params + 2);
            func_800286CC(globalCtx, &actorSpawnPos, &sZeroVec, &sZeroVec, 1000, 50);
        }
    }
}

void func_80882E54(BgHakaZou* self, GlobalContext* globalCtx) {
    Vec3f fragmentPos;
    s32 i;
    s32 j;
    s32 num = 25;

    fragmentPos.x = self->collider.dim.pos.x;
    fragmentPos.y = self->collider.dim.pos.y;
    fragmentPos.z = self->collider.dim.pos.z;

    EffectSsHahen_SpawnBurst(globalCtx, &fragmentPos, 10.0f, 0, 10, 10, 4, 141, 40, gBotwBombSpotDL);

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            fragmentPos.x = self->collider.dim.pos.x + (((j * 2) - 1) * num);
            fragmentPos.z = self->collider.dim.pos.z + (((i * 2) - 1) * num);
            EffectSsHahen_SpawnBurst(globalCtx, &fragmentPos, 10.0f, 0, 10, 10, 4, 141, 40, gBotwBombSpotDL);
            func_800286CC(globalCtx, &fragmentPos, &sZeroVec, &sZeroVec, 1000, 50);
        }
    }
}

void func_80883000(BgHakaZou* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        Flags_SetSwitch(globalCtx, self->switchFlag);

        if (self->dyna.actor.params == STA_GIANT_BIRD_STATUE) {
            self->timer = 20;
            self->actionFunc = func_80883144;
            OnePointCutscene_Init(globalCtx, 3400, 999, &self->dyna.actor, MAIN_CAM);
        } else if (self->dyna.actor.params == 2) {
            func_80882E54(self, globalCtx);
            self->dyna.actor.draw = NULL;
            self->timer = 1;
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_EXPLOSION);
            self->actionFunc = func_80883104;
        } else {
            func_80882CC4(self, globalCtx);
            self->timer = 1;
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_WALL_BROKEN);
            self->actionFunc = func_80883104;
        }
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void func_80883104(BgHakaZou* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        Actor_Kill(&self->dyna.actor);
    }
}

void func_80883144(BgHakaZou* self, GlobalContext* globalCtx) {
    Vec3f explosionPos;

    if (self->timer != 0) {
        self->timer--;
    }

    if (!(self->timer % 4)) {
        explosionPos.x = Rand_CenteredFloat(200.0f) + (self->dyna.actor.world.pos.x - 56.0f);
        explosionPos.y = (Rand_ZeroOne() * 80.0f) + self->dyna.actor.world.pos.y;
        explosionPos.z = Rand_CenteredFloat(200.0f) + (self->dyna.actor.world.pos.z + 56.0f);

        EffectSsBomb2_SpawnLayered(globalCtx, &explosionPos, &sZeroVec, &sZeroVec, 150, 70);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_IT_BOMB_EXPLOSION);
    }

    if (self->timer == 0) {
        self->timer = 20;
        self->actionFunc = func_80883254;
    }
}

void func_80883254(BgHakaZou* self, GlobalContext* globalCtx) {
    f32 moveDist = (Rand_ZeroOne() * 0.5f) + 0.5f;

    Math_StepToF(&self->dyna.actor.world.pos.z, self->dyna.actor.home.pos.z - 80.0f, 2.0f * moveDist);

    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y - 40.0f, moveDist)) {
        if (self->timer != 0) {
            self->timer--;
        }

        if (self->timer == 0) {
            self->timer = 60;
            self->dyna.actor.world.rot.x = 8;
            self->actionFunc = func_80883328;
        }
    } else {
        func_808828F4(self, globalCtx);
    }
}

void func_80883328(BgHakaZou* self, GlobalContext* globalCtx) {
    Vec3f effectPos;
    s32 i;
    s32 j;

    self->dyna.actor.world.rot.x += self->dyna.actor.world.rot.x / 8.0f;

    if (Math_ScaledStepToS(&self->dyna.actor.shape.rot.x, -0x4000, self->dyna.actor.world.rot.x)) {
        effectPos.x = self->dyna.actor.world.pos.x;
        effectPos.y = self->dyna.actor.world.pos.y;

        for (j = 0; j < 2; j++) {
            effectPos.z = self->dyna.actor.world.pos.z;

            for (i = 0; i < 4; i++) {
                effectPos.z -= (i == 2) ? 550.0f : 50.0f;
                func_800286CC(globalCtx, &effectPos, &sZeroVec, &sZeroVec, (Rand_ZeroOne() * 200.0f) + 1000.0f, 200);
            }

            effectPos.x -= 112.0f;
        }

        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_STONE_BOUND);
        self->timer = 25;
        self->actionFunc = func_808834D8;
    }
}

void func_808834D8(BgHakaZou* self, GlobalContext* globalCtx) {
    f32 moveDist;

    if (self->timer != 0) {
        self->timer--;
    }

    moveDist = (self->timer % 2) ? 15.0f : -15.0f;
    self->dyna.actor.world.pos.y += ((self->timer & 0xFE) * 0.04f * moveDist);

    if (self->timer == 0) {
        self->actionFunc = BgHakaZou_DoNothing;
    }
}

void BgHakaZou_DoNothing(BgHakaZou* self, GlobalContext* globalCtx) {
}

void BgHakaZou_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaZou* self = THIS;

    self->actionFunc(self, globalCtx);

    if (self->dyna.actor.params == 3) {
        Actor_MoveForward(&self->dyna.actor);
    }
}

void BgHakaZou_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* dLists[] = { 0x060064E0, 0x06005CE0, gBotwBombSpotDL, 0x06005CE0 };

    Gfx_DrawDListOpa(globalCtx, dLists[thisx->params]);
}
