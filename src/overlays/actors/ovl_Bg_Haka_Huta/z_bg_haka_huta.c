/*
 * File: z_bg_haka_huta.c
 * Overlay: ovl_Bg_Haka_Huta
 * Description: Coffin Lid
 */

#include "z_bg_haka_huta.h"
#include "objects/object_hakach_objects/object_hakach_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgHakaHuta*)thisx)

void BgHakaHuta_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaHuta_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaHuta_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaHuta_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgHakaHuta_SpawnDust(BgHakaHuta* self, GlobalContext* globalCtx);
void BgHakaHuta_PlaySound(BgHakaHuta* self, GlobalContext* globalCtx, u16 sfx);
void BgHakaHuta_SpawnEnemies(BgHakaHuta* self, GlobalContext* globalCtx);
void BgHakaHuta_Open(BgHakaHuta* self, GlobalContext* globalCtx);
void BgHakaHuta_SlideOpen(BgHakaHuta* self, GlobalContext* globalCtx);
void func_8087D720(BgHakaHuta* self, GlobalContext* globalCtx);
void BgHakaHuta_DoNothing(BgHakaHuta* self, GlobalContext* globalCtx);

const ActorInit Bg_Haka_Huta_InitVars = {
    ACTOR_BG_HAKA_HUTA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKACH_OBJECTS,
    sizeof(BgHakaHuta),
    (ActorFunc)BgHakaHuta_Init,
    (ActorFunc)BgHakaHuta_Destroy,
    (ActorFunc)BgHakaHuta_Update,
    (ActorFunc)BgHakaHuta_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaHuta_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaHuta* self = THIS;
    s16 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&gBotwCoffinLidCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    self->unk_16A = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    if (Flags_GetSwitch(globalCtx, thisx->params)) {
        self->counter = -1;
        self->actionFunc = func_8087D720;
    } else {
        self->actionFunc = BgHakaHuta_SpawnEnemies;
    }
}

void BgHakaHuta_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaHuta* self = THIS;
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgHakaHuta_SpawnDust(BgHakaHuta* self, GlobalContext* globalCtx) {
    static Vec3f sEffectAccel[] = { 0.0f, 0.0f, 0.0f };
    static Color_RGBA8 primColor = { 30, 20, 50, 255 };
    static Color_RGBA8 envColor = { 0, 0, 0, 255 };
    f32 scale;
    f32 phi_f20;
    Vec3f effectPos;
    Vec3f effectVel;
    s32 i;
    f32 new_Xpos;
    f32 xPosOffset;

    phi_f20 = (self->dyna.actor.world.rot.y == 0) ? 1.0f : -1.0f;
    effectVel.y = 0.0f;
    effectVel.z = 0.0f;
    effectVel.x = -0.5f * phi_f20;
    effectPos.y = self->dyna.actor.world.pos.y;
    effectPos.z = self->dyna.actor.world.pos.z;
    new_Xpos = 50 - ((self->dyna.actor.world.pos.x - self->dyna.actor.home.pos.x) * phi_f20);
    xPosOffset = new_Xpos * phi_f20;

    for (i = 0; i < 4; i++) {
        if (i == 2) {
            effectPos.z += 120.0f * phi_f20;
        }
        effectPos.x = self->dyna.actor.home.pos.x - (Rand_ZeroOne() * xPosOffset);
        scale = ((Rand_ZeroOne() * 10.0f) + 50.0f);
        func_8002829C(globalCtx, &effectPos, &effectVel, sEffectAccel, &primColor, &envColor, scale, 0xA);
    }
}

void BgHakaHuta_PlaySound(BgHakaHuta* self, GlobalContext* globalCtx, u16 sfx) {
    Vec3f pos;

    pos.z = (self->dyna.actor.shape.rot.y == 0) ? self->dyna.actor.world.pos.z + 120.0f
                                                : self->dyna.actor.world.pos.z - 120.0f;
    pos.x = self->dyna.actor.world.pos.x;
    pos.y = self->dyna.actor.world.pos.y;
    Audio_PlaySoundAtPosition(globalCtx, &pos, 30, sfx);
}

void BgHakaHuta_SpawnEnemies(BgHakaHuta* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->dyna.actor.params) && !Player_InCsMode(globalCtx)) {
        self->counter = 25;
        self->actionFunc = BgHakaHuta_Open;
        OnePointCutscene_Init(globalCtx, 6001, 999, &self->dyna.actor, MAIN_CAM);
        if (self->unk_16A == 2) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_FIREFLY,
                        (self->dyna.actor.world.pos.x + (-25.0f) * Math_CosS(self->dyna.actor.shape.rot.y) +
                         40.0f * Math_SinS(self->dyna.actor.shape.rot.y)),
                        self->dyna.actor.world.pos.y - 10.0f,
                        (self->dyna.actor.world.pos.z - (-25.0f) * Math_SinS(self->dyna.actor.shape.rot.y) +
                         Math_CosS(self->dyna.actor.shape.rot.y) * 40.0f),
                        0, self->dyna.actor.shape.rot.y + 0x8000, 0, 2);

            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_FIREFLY,
                        (self->dyna.actor.world.pos.x + (-25.0f) * (Math_CosS(self->dyna.actor.shape.rot.y)) +
                         Math_SinS(self->dyna.actor.shape.rot.y) * 80.0f),
                        self->dyna.actor.world.pos.y - 10.0f,
                        (self->dyna.actor.world.pos.z - (-25.0f) * (Math_SinS(self->dyna.actor.shape.rot.y)) +
                         Math_CosS(self->dyna.actor.shape.rot.y) * 80.0f),
                        0, self->dyna.actor.shape.rot.y, 0, 2);

        } else if (self->unk_16A == 1) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_RD,
                        (self->dyna.actor.home.pos.x + (-25.0f) * (Math_CosS(self->dyna.actor.shape.rot.y)) +
                         Math_SinS(self->dyna.actor.shape.rot.y) * 100.0f),
                        self->dyna.actor.home.pos.y - 40.0f,
                        (self->dyna.actor.home.pos.z - (-25.0f) * (Math_SinS(self->dyna.actor.shape.rot.y)) +
                         Math_CosS(self->dyna.actor.shape.rot.y) * 100.0f),
                        0, self->dyna.actor.shape.rot.y, 0, 0xFD);
        }
    }
}

void BgHakaHuta_Open(BgHakaHuta* self, GlobalContext* globalCtx) {
    f32 posOffset;

    if (self->counter != 0) {
        self->counter--;
    }
    posOffset = (self->dyna.actor.world.rot.y == 0) ? 4.0f : -4.0f;
    Math_StepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x + posOffset, 2.0f);
    if (self->counter == 0) {
        self->counter = 37;
        BgHakaHuta_PlaySound(self, globalCtx, NA_SE_EV_COFFIN_CAP_OPEN);
        self->actionFunc = BgHakaHuta_SlideOpen;
    }
}

void BgHakaHuta_SlideOpen(BgHakaHuta* self, GlobalContext* globalCtx) {
    f32 posOffset;

    if (self->counter != 0) {
        self->counter--;
    }
    posOffset = (self->dyna.actor.world.rot.y == 0) ? 24.0f : -24.0f;
    if (!Math_StepToF(&self->dyna.actor.world.pos.x, self->dyna.actor.home.pos.x + posOffset, 0.5f)) {
        BgHakaHuta_SpawnDust(self, globalCtx);
    }
    if (self->counter == 0) {
        BgHakaHuta_PlaySound(self, globalCtx, NA_SE_EV_COFFIN_CAP_BOUND);
        self->actionFunc = func_8087D720;
    }
}

void func_8087D720(BgHakaHuta* self, GlobalContext* globalCtx) {
    static Vec3f D_8087D958 = { 30.0f, 0.0f, 0.0f };
    static Vec3f D_8087D964 = { 0.03258f, 0.3258f, -0.9449f };
    MtxF mtx;
    Vec3f vec;
    s32 quakeIndex;

    self->counter++;
    if (self->counter == 6) {
        self->actionFunc = BgHakaHuta_DoNothing;
        quakeIndex = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
        Quake_SetSpeed(quakeIndex, 0x7530);
        Quake_SetQuakeValues(quakeIndex, 4, 0, 0, 0);
        Quake_SetCountdown(quakeIndex, 2);
    } else if (self->counter == 0) {
        self->counter = 6;
        self->actionFunc = BgHakaHuta_DoNothing;
    }

    D_8087D958.x = self->counter + 24.0f;
    if (D_8087D958.x > 30.0f) {
        D_8087D958.x = 30.0f;
    }
    Matrix_RotateY(self->dyna.actor.world.rot.y * (M_PI / 0x8000), MTXMODE_NEW);
    Matrix_RotateAxis(self->counter * (191 * M_PI / 3750), &D_8087D964, MTXMODE_APPLY);
    Matrix_MultVec3f(&D_8087D958, &vec);
    self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x + vec.x;
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + vec.y;
    self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z + vec.z;
    Matrix_Get(&mtx);
    Matrix_MtxFToYXZRotS(&mtx, &self->dyna.actor.shape.rot, 0);
}

void BgHakaHuta_DoNothing(BgHakaHuta* self, GlobalContext* globalCtx) {
}

void BgHakaHuta_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaHuta* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgHakaHuta_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gBotwCoffinLidDL);
}
