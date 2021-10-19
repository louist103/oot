/*
 * File: z_en_horse_zelda.c
 * Overlay: ovl_En_Horse_Zelda
 * Description: Zelda's Horse
 */

#include "z_en_horse_zelda.h"
#include "objects/object_horse_zelda/object_horse_zelda.h"

#define FLAGS 0x00000010

#define THIS ((EnHorseZelda*)thisx)

void EnHorseZelda_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHorseZelda_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHorseZelda_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHorseZelda_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A6DCCC(EnHorseZelda* self, GlobalContext* globalCtx);
void func_80A6DDFC(EnHorseZelda* self, GlobalContext* globalCtx);
void func_80A6DC7C(EnHorseZelda* self);

const ActorInit En_Horse_Zelda_InitVars = {
    ACTOR_EN_HORSE_ZELDA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HORSE_ZELDA,
    sizeof(EnHorseZelda),
    (ActorFunc)EnHorseZelda_Init,
    (ActorFunc)EnHorseZelda_Destroy,
    (ActorFunc)EnHorseZelda_Update,
    (ActorFunc)EnHorseZelda_Draw,
};

static AnimationHeader* sAnimationHeaders[] = { &gHorseZeldaGallopingAnim };

static f32 splaySpeeds[] = { 2.0f / 3.0f };

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 40, 100, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 13, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1 | OC2_UNK1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static CollisionCheckInfoInit sColChkInfoInit = { 10, 35, 100, MASS_HEAVY };

typedef struct {
    /* 0x0 */ Vec3s unk_0;
    /* 0x6 */ u8 unk_6;
} unknownStruct; // size = 0x8

static unknownStruct D_80A6E240[] = {
    { -1682, -500, 12578, 0x07 }, { -3288, -500, 13013, 0x07 }, { -5142, -417, 11630, 0x07 },
    { -5794, -473, 9573, 0x07 },  { -6765, -500, 8364, 0x07 },  { -6619, -393, 6919, 0x07 },
    { -5193, 124, 5433, 0x07 },   { -2970, 2, 4537, 0x07 },     { -2949, -35, 4527, 0x07 },
    { -1907, -47, 2978, 0x07 },   { 2488, 294, 3628, 0x07 },    { 3089, 378, 4713, 0x07 },
    { 1614, -261, 7596, 0x07 },   { 754, -187, 9295, 0x07 },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_STOP),
};

static EnHorseZeldaActionFunc sActionFuncs[] = {
    func_80A6DCCC,
    func_80A6DDFC,
};

void func_80A6D8D0(unknownStruct* data, s32 index, Vec3f* vec) {
    vec->x = data[index].unk_0.x;
    vec->y = data[index].unk_0.y;
    vec->z = data[index].unk_0.z;
}

void func_80A6D918(EnHorseZelda* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f sp28;
    s16 yawDiff;

    func_80A6D8D0(D_80A6E240, self->unk_1EC, &sp28);
    if (Math3D_Vec3f_DistXYZ(&sp28, &self->actor.world.pos) <= 400.0f) {
        self->unk_1EC++;
        if (self->unk_1EC >= 14) {
            self->unk_1EC = 0;
            func_80A6D8D0(D_80A6E240, 0, &sp28);
        }
    }
    yawDiff = Math_Vec3f_Yaw(&self->actor.world.pos, &sp28) - self->actor.world.rot.y;
    if (yawDiff >= 0x12D) {
        self->actor.world.rot.y += 0x12C;
    } else if (yawDiff < -0x12C) {
        self->actor.world.rot.y -= 0x12C;
    } else {
        self->actor.world.rot.y += yawDiff;
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(globalCtx)->actor) <= 300.0f) {
        if (self->actor.speedXZ < 12.0f) {
            self->actor.speedXZ += 1.0f;
        } else {
            self->actor.speedXZ -= 1.0f;
        }
    } else if (self->actor.speedXZ < D_80A6E240[self->unk_1EC].unk_6) {
        self->actor.speedXZ += 0.5f;
    } else {
        self->actor.speedXZ -= 0.5f;
    }
}

void EnHorseZelda_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseZelda* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Actor_SetScale(&self->actor, 0.0115f);
    self->actor.gravity = -3.5f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawHorse, 20.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.focus.pos = self->actor.world.pos;
    self->action = 0;
    self->actor.focus.pos.y += 70.0f;
    func_800A663C(globalCtx, &self->skin, &gHorseZeldaSkel, &gHorseZeldaGallopingAnim);
    self->animationIndex = 0;
    Animation_PlayOnce(&self->skin.skelAnime, sAnimationHeaders[0]);
    Collider_InitCylinder(globalCtx, &self->colliderCylinder);
    Collider_SetCylinderType1(globalCtx, &self->colliderCylinder, &self->actor, &sCylinderInit);
    Collider_InitJntSph(globalCtx, &self->colliderSphere);
    Collider_SetJntSph(globalCtx, &self->colliderSphere, &self->actor, &sJntSphInit, &self->colliderSphereItem);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    self->animationIndex = 0;
    func_80A6DC7C(self);
}

void EnHorseZelda_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseZelda* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->colliderCylinder);
    Collider_DestroyJntSph(globalCtx, &self->colliderSphere);
    func_800A6888(globalCtx, &self->skin);
}

void func_80A6DC7C(EnHorseZelda* self) {
    self->action = 0;
    self->animationIndex++;
    if (self->animationIndex > 0) {
        self->animationIndex = 0;
    }
    Animation_PlayOnce(&self->skin.skelAnime, sAnimationHeaders[self->animationIndex]);
}

void func_80A6DCCC(EnHorseZelda* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 0.0f;
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        func_80A6DC7C(self);
    }
}

void func_80A6DD14(EnHorseZelda* self) {
    f32 sp34;

    self->action = 1;
    self->animationIndex = 0;
    sp34 = self->actor.speedXZ / 6.0f;
    Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->animationIndex],
                     splaySpeeds[self->animationIndex] * sp34 * 1.5f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->animationIndex]), ANIMMODE_ONCE, 0.0f);
}

void func_80A6DDFC(EnHorseZelda* self, GlobalContext* globalCtx) {
    func_80A6D918(self, globalCtx);
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        func_80A6DD14(self);
    }
}

void func_80A6DE38(EnHorseZelda* self, GlobalContext* globalCtx) {
    s32 pad;
    CollisionPoly* poly;
    s32 pad2;
    Vec3f pos;
    s32 bgId;

    pos.x = (Math_SinS(self->actor.shape.rot.y) * 30.0f) + self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + 60.0f;
    pos.z = (Math_CosS(self->actor.shape.rot.y) * 30.0f) + self->actor.world.pos.z;
    self->unk_1F4 = BgCheck_EntityRaycastFloor3(&globalCtx->colCtx, &poly, &bgId, &pos);
    self->actor.shape.rot.x = Math_FAtan2F(self->actor.world.pos.y - self->unk_1F4, 30.0f) * (0x8000 / M_PI);
}

void EnHorseZelda_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseZelda* self = THIS;
    s32 pad;

    sActionFuncs[self->action](self, globalCtx);
    self->actor.speedXZ = 0.0f;
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 55.0f, 100.0f, 0x1D);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    Collider_UpdateCylinder(&self->actor, &self->colliderCylinder);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderCylinder.base);
}

void func_80A6DFD4(Actor* thisx, GlobalContext* globalCtx, PSkinAwb* skin) {
    Vec3f sp4C;
    Vec3f sp40;
    EnHorseZelda* self = THIS;
    s32 i;

    for (i = 0; i < self->colliderSphere.count; i++) {
        sp4C.x = self->colliderSphere.elements[i].dim.modelSphere.center.x;
        sp4C.y = self->colliderSphere.elements[i].dim.modelSphere.center.y;
        sp4C.z = self->colliderSphere.elements[i].dim.modelSphere.center.z;

        func_800A6408(skin, self->colliderSphere.elements[i].dim.limb, &sp4C, &sp40);

        self->colliderSphere.elements[i].dim.worldSphere.center.x = sp40.x;
        self->colliderSphere.elements[i].dim.worldSphere.center.y = sp40.y;
        self->colliderSphere.elements[i].dim.worldSphere.center.z = sp40.z;

        self->colliderSphere.elements[i].dim.worldSphere.radius =
            self->colliderSphere.elements[i].dim.modelSphere.radius * self->colliderSphere.elements[i].dim.scale;
    }

    //! @bug see relevant comment in `EnHorse_SkinCallback1`
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderSphere.base);
}

void EnHorseZelda_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnHorseZelda* self = THIS;

    func_80A6DE38(self, globalCtx);
    func_80093D18(globalCtx->state.gfxCtx);
    func_800A6330(&self->actor, globalCtx, &self->skin, func_80A6DFD4, 1);
}
