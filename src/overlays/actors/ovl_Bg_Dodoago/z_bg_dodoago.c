/*
 * File: z_bg_dodoago.c
 * Overlay: ovl_Bg_Dodoago
 * Description: Dodongo Head Statue in Dodongo's Cavern
 */

#include "z_bg_dodoago.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "objects/object_ddan_objects/object_ddan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgDodoago*)thisx)

void BgDodoago_Init(Actor* thisx, GlobalContext* globalCtx);
void BgDodoago_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgDodoago_Update(Actor* thisx, GlobalContext* globalCtx);
void BgDodoago_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80871CF4(BgDodoago* self, GlobalContext* globalCtx);
void func_80871FB8(BgDodoago* self, GlobalContext* globalCtx);
void BgDodoago_DoNothing(BgDodoago* self, GlobalContext* globalCtx);
void func_80872288(BgDodoago* self, GlobalContext* globalCtx);

const ActorInit Bg_Dodoago_InitVars = {
    ACTOR_BG_DODOAGO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DDAN_OBJECTS,
    sizeof(BgDodoago),
    (ActorFunc)BgDodoago_Init,
    (ActorFunc)BgDodoago_Destroy,
    (ActorFunc)BgDodoago_Update,
    (ActorFunc)BgDodoago_Draw,
};

static ColliderCylinderInit sColCylinderInit0 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ALL,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 80, 30, 80, { 0, 0, 0 } },
};

static ColliderCylinderInit sColCylinderInit1 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_NO_PUSH | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 50, 60, 280, { 0, 0, 0 } },
};

static s16 sHasParent = false;

void BgDodoago_SetupAction(BgDodoago* self, BgDodoagoActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgDodoago_SpawnSparkles(Vec3f* vec, GlobalContext* globalCtx) {
    Vec3f pos;
    Color_RGBA8 primColor = { 100, 100, 100, 0 };
    Color_RGBA8 envColor = { 40, 40, 40, 0 };
    static Vec3f velocity = { 0.0f, -1.5f, 0.0f };
    static Vec3f acceleration = { 0.0f, -0.2f, 0.0f };
    s32 i;

    for (i = 4; i > 0; i--) {
        pos.x = Rand_CenteredFloat(20.0f) + vec->x;
        pos.y = Rand_CenteredFloat(10.0f) + vec->y;
        pos.z = Rand_CenteredFloat(20.0f) + vec->z;
        EffectSsKiraKira_SpawnSmall(globalCtx, &pos, &velocity, &acceleration, &primColor, &envColor);
    }
}

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 5000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

static u8 D_808727C0[100];

static s32 D_80872824;

void BgDodoago_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgDodoago* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gDodongoLowerJawCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    ActorShape_Init(&self->dyna.actor.shape, 0.0f, NULL, 0.0f);

    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params & 0x3F))) {
        BgDodoago_SetupAction(self, BgDodoago_DoNothing);
        self->dyna.actor.shape.rot.x = 0x1333;
        globalCtx->roomCtx.unk_74[0] = globalCtx->roomCtx.unk_74[1] = 0xFF;
        return;
    }

    Collider_InitCylinder(globalCtx, &self->colliders[0]);
    Collider_InitCylinder(globalCtx, &self->colliders[1]);
    Collider_InitCylinder(globalCtx, &self->colliders[2]);
    Collider_SetCylinder(globalCtx, &self->colliders[0], &self->dyna.actor, &sColCylinderInit0);
    Collider_SetCylinder(globalCtx, &self->colliders[1], &self->dyna.actor, &sColCylinderInit1);
    Collider_SetCylinder(globalCtx, &self->colliders[2], &self->dyna.actor, &sColCylinderInit1);
    BgDodoago_SetupAction(self, func_80871CF4);
    D_808727C0[0] = 0;
}

void BgDodoago_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgDodoago* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(globalCtx, &self->colliders[0]);
    Collider_DestroyCylinder(globalCtx, &self->colliders[1]);
    Collider_DestroyCylinder(globalCtx, &self->colliders[2]);
}

void func_80871CF4(BgDodoago* self, GlobalContext* globalCtx) {
    Actor* explosive = Actor_GetCollidedExplosive(globalCtx, &self->colliders[0].base);

    if (explosive != NULL) {
        self->unk_164 =
            (Math_Vec3f_Yaw(&self->dyna.actor.world.pos, &explosive->world.pos) >= self->dyna.actor.shape.rot.y) ? 1
                                                                                                                 : 0;

        if (((globalCtx->roomCtx.unk_74[0] == 0xFF) && (self->unk_164 == 1)) ||
            ((globalCtx->roomCtx.unk_74[1] == 0xFF) && (self->unk_164 == 0))) {
            Flags_SetSwitch(globalCtx, (self->dyna.actor.params & 0x3F));
            self->unk_164 = 0;
            Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            BgDodoago_SetupAction(self, func_80871FB8);
            OnePointCutscene_Init(globalCtx, 3380, 160, &self->dyna.actor, MAIN_CAM);
        } else if (globalCtx->roomCtx.unk_74[self->unk_164] == 0) {
            OnePointCutscene_Init(globalCtx, 3065, 40, &self->dyna.actor, MAIN_CAM);
            BgDodoago_SetupAction(self, func_80872288);
            Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        } else {
            OnePointCutscene_Init(globalCtx, 3065, 20, &self->dyna.actor, MAIN_CAM);
            Audio_PlaySoundGeneral(NA_SE_SY_ERROR, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            D_80872824 += 0x1E;
            return;
        }

        if (!sHasParent) {
            self->dyna.actor.parent = explosive;
            sHasParent = true;
            D_80872824 = 0x32;
        }
    } else if (Flags_GetEventChkInf(0xB0)) {
        Collider_UpdateCylinder(&self->dyna.actor, &self->colliders[0]);
        Collider_UpdateCylinder(&self->dyna.actor, &self->colliders[1]);
        Collider_UpdateCylinder(&self->dyna.actor, &self->colliders[2]);
        self->colliders[0].dim.pos.z += 200;
        self->colliders[1].dim.pos.z += 215;
        self->colliders[1].dim.pos.x += 90;
        self->colliders[2].dim.pos.z += 215;
        self->colliders[2].dim.pos.x -= 90;

        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliders[0].base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliders[1].base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliders[2].base);
    }
}

void func_80871FB8(BgDodoago* self, GlobalContext* globalCtx) {
    Vec3f currentPos;
    Vec3f D_808725CC[] = {
        { 0.0f, -200.0f, 430.0f },   { 20.0f, -200.0f, 420.0f }, { -20.0f, -200.0f, 420.0f },
        { 40.0, -200.0f, 380.0f },   { -40.0, -200.0f, 380.0f }, { 50.0, -200.0f, 350.0f },
        { -50.0f, -200.0f, 350.0f }, { 60.0f, -200.0f, 320.0f }, { -60.0f, -200.0f, 320.0f },
        { 70.0f, -200.0f, 290.0f },  { -70.0, -200.0f, 290.0f },
    };
    s32 i;

    if (globalCtx->roomCtx.unk_74[0] < 0xFF) {
        globalCtx->roomCtx.unk_74[0] += 5;
    }
    if (globalCtx->roomCtx.unk_74[1] < 0xFF) {
        globalCtx->roomCtx.unk_74[1] += 5;
    }

    if (globalCtx->roomCtx.unk_74[0] != 0xFF || globalCtx->roomCtx.unk_74[1] != 0xFF) {
        D_80872824--;
        return;
    }

    if (D_80872824 == 0x6C) {
        for (i = 10; i >= 0; i--) {
            currentPos.x = D_808725CC[i].x + self->dyna.actor.world.pos.x;
            currentPos.y = D_808725CC[i].y + self->dyna.actor.world.pos.y;
            currentPos.z = D_808725CC[i].z + self->dyna.actor.world.pos.z;
            func_80033480(globalCtx, &currentPos, 2.0f, 3, 0xC8, 0x4B, 1);
        }
    }

    currentPos.x = self->dyna.actor.world.pos.x + 200.0f;
    currentPos.y = self->dyna.actor.world.pos.y - 20.0f;
    currentPos.z = self->dyna.actor.world.pos.z + 100.0f;

    BgDodoago_SpawnSparkles(&currentPos, globalCtx);

    currentPos.x = self->dyna.actor.world.pos.x - 200.0f;
    currentPos.y = self->dyna.actor.world.pos.y - 20.0f;
    currentPos.z = self->dyna.actor.world.pos.z + 100.0f;

    BgDodoago_SpawnSparkles(&currentPos, globalCtx);
    Math_StepToS(&self->unk_164, 0x64, 3);
    func_800AA000(500.0f, 0x78, 0x14, 0xA);

    if (Math_SmoothStepToS(&self->dyna.actor.shape.rot.x, 0x1333, 0x6E - self->unk_164, 0x3E8, 0x32) == 0) {
        BgDodoago_SetupAction(self, BgDodoago_DoNothing);
        Audio_PlaySoundGeneral(NA_SE_EV_STONE_BOUND, &self->dyna.actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else {
        Audio_PlaySoundGeneral(NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG, &self->dyna.actor.projectedPos, 4, &D_801333E0,
                               &D_801333E0, &D_801333E8);
    }
}

void BgDodoago_DoNothing(BgDodoago* self, GlobalContext* globalCtx) {
}

void func_80872288(BgDodoago* self, GlobalContext* globalCtx) {
    globalCtx->roomCtx.unk_74[self->unk_164] += 5;

    if (globalCtx->roomCtx.unk_74[self->unk_164] == 0xFF) {
        BgDodoago_SetupAction(self, func_80871CF4);
    }
}

void BgDodoago_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgDodoago* self = THIS;
    EnBom* bomb;

    if (self->dyna.actor.parent == NULL) {
        if ((s32)(self->colliders[1].base.ocFlags1 & OC1_HIT) || (self->colliders[2].base.ocFlags1 & OC1_HIT)) {

            if ((s32)(self->colliders[1].base.ocFlags1 & OC1_HIT)) {
                bomb = (EnBom*)self->colliders[1].base.oc;
            } else {
                bomb = (EnBom*)self->colliders[2].base.oc;
            }
            self->colliders[1].base.ocFlags1 &= ~OC1_HIT;
            self->colliders[2].base.ocFlags1 &= ~OC1_HIT;
            if (bomb->actor.category == ACTORCAT_EXPLOSIVE && bomb->actor.id == ACTOR_EN_BOM &&
                bomb->actor.params == 0) {
                self->dyna.actor.parent = &bomb->actor;
                bomb->timer = 50;
                bomb->actor.speedXZ = 0.0f;
                D_80872824 = 0;
            }
        }
    } else {
        D_80872824++;
        Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F);
        if (D_808727C0[0] == 0 && D_80872824 >= 0x8D) {
            if (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) {
                D_808727C0[0]++;
            } else {
                self->dyna.actor.parent = NULL;
            }
        }
    }
    self->actionFunc(self, globalCtx);
}

void BgDodoago_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_dodoago.c", 672);

    if (Flags_GetEventChkInf(0xB0)) {
        func_80093D18(globalCtx->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_dodoago.c", 677),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gDodongoLowerJarDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_dodoago.c", 681);
}
