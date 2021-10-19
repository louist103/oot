#include "global.h"
#include "vt.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000010

#define THIS ((EnAObj*)thisx)

void EnAObj_Init(Actor* thisx, GlobalContext* globalCtx);
void EnAObj_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnAObj_Update(Actor* thisx, GlobalContext* globalCtx);
void EnAObj_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8001D204(EnAObj* self, GlobalContext* globalCtx);
void func_8001D25C(EnAObj* self, GlobalContext* globalCtx);
void func_8001D360(EnAObj* self, GlobalContext* globalCtx);
void func_8001D4A8(EnAObj* self, GlobalContext* globalCtx);
void func_8001D608(EnAObj* self, GlobalContext* globalCtx);

void func_8001D234(EnAObj* self, s16 params);
void func_8001D310(EnAObj* self, s16 params);
void func_8001D480(EnAObj* self, s16 params);
void func_8001D5C8(EnAObj* self, s16 params);

const ActorInit En_A_Obj_InitVars = {
    ACTOR_EN_A_OBJ,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnAObj),
    (ActorFunc)EnAObj_Init,
    (ActorFunc)EnAObj_Destroy,
    (ActorFunc)EnAObj_Update,
    (ActorFunc)EnAObj_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ALL,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 25, 60, 0, { 0, 0, 0 } },
};

extern CollisionHeader D_06000730;

static CollisionHeader* D_8011546C[] = {
    &gUnknown1Col, &gUnknown1Col, &gUnknown4Col, &gUnknown5Col, &gUnknown6Col, &D_06000730,
};

static Gfx* D_80115484[] = {
    gUnusedRockRectangularPrism2DL,
    gUnusedRockRectangularPrism2DL,
    gUnusedRockRectangularPrism2DL,
    gUnusedRockRectangularPrism4DL,
    gUnusedRockRectangularPrism4DL,
    gUnusedRockRectangularPrism5DL,
    0x06000210,
    gUnusedGrassBladesDL,
    gUnusedTreeStumpDL,
    gSignRectangularDL,
    gSignDirectionalDL,
    gBoulderFragmentsDL,
};

void EnAObj_SetupAction(EnAObj* self, EnAObjActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnAObj_Init(Actor* thisx, GlobalContext* globalCtx) {
    CollisionHeader* colHeader = NULL;
    s32 pad;
    EnAObj* self = THIS;
    f32 sp28 = 6.0f;

    self->textId = (thisx->params >> 8) & 0xFF;
    thisx->params &= 0xFF;

    switch (thisx->params) {
        case A_OBJ_BLOCK_SMALL:
            Actor_SetScale(thisx, 0.025f);
            break;
        case A_OBJ_BLOCK_LARGE:
            Actor_SetScale(thisx, 0.05f);
            break;
        case A_OBJ_BLOCK_HUGE:
        case A_OBJ_CUBE_SMALL:
        case A_OBJ_UNKNOWN_6:
            Actor_SetScale(thisx, 0.1f);
            break;
        case A_OBJ_BLOCK_SMALL_ROT:
            Actor_SetScale(thisx, 0.005f);
            break;
        case A_OBJ_BLOCK_LARGE_ROT:
        default:
            Actor_SetScale(thisx, 0.01f);
            break;
    }

    if (thisx->params >= 9) {
        sp28 = 12.0f;
    }

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, sp28);

    thisx->focus.pos = thisx->world.pos;
    self->dyna.bgId = BGACTOR_NEG_ONE;
    self->dyna.unk_160 = 0;
    self->dyna.unk_15C = DPM_UNK;
    thisx->uncullZoneDownward = 1200.0f;
    thisx->uncullZoneScale = 200.0f;

    switch (thisx->params) {
        case A_OBJ_BLOCK_LARGE:
        case A_OBJ_BLOCK_HUGE:
            self->dyna.bgId = 1;
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, thisx, ACTORCAT_BG);
            func_8001D5C8(self, thisx->params);
            break;
        case A_OBJ_BLOCK_SMALL_ROT:
        case A_OBJ_BLOCK_LARGE_ROT:
            self->dyna.bgId = 3;
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, thisx, ACTORCAT_BG);
            func_8001D310(self, thisx->params);
            break;
        case A_OBJ_UNKNOWN_6:
            // clang-format off
            thisx->flags |= 0x1; self->dyna.bgId = 5; self->unk_178 = 10.0f;
            // clang-format on
            thisx->gravity = -2.0f;
            func_8001D234(self, thisx->params);
            break;
        case A_OBJ_GRASS_CLUMP:
        case A_OBJ_TREE_STUMP:
            self->dyna.bgId = 0;
            func_8001D234(self, thisx->params);
            break;
        case A_OBJ_SIGNPOST_OBLONG:
        case A_OBJ_SIGNPOST_ARROW:
            thisx->textId = (self->textId & 0xFF) | 0x300;
            // clang-format off
            thisx->flags |= 0x1 | 0x8; thisx->targetArrowOffset = 500.0f;
            // clang-format on
            self->unk_178 = 45.0f;
            func_8001D234(self, thisx->params);
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, thisx, &sCylinderInit);
            thisx->colChkInfo.mass = MASS_IMMOVABLE;
            thisx->targetMode = 0;
            break;
        case A_OBJ_KNOB:
            thisx->gravity = -1.5f;
            func_8001D480(self, thisx->params);
            break;
        default:
            thisx->gravity = -2.0f;
            func_8001D234(self, thisx->params);
            break;
    }

    if (thisx->params < 5) {
        thisx->colChkInfo.mass = MASS_IMMOVABLE;
    }

    if (self->dyna.bgId != BGACTOR_NEG_ONE) {
        CollisionHeader_GetVirtual(D_8011546C[self->dyna.bgId], &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    }
}

void EnAObj_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnAObj* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);

    switch (self->dyna.actor.params) {
        case A_OBJ_SIGNPOST_OBLONG:
        case A_OBJ_SIGNPOST_ARROW:
            Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void func_8001D204(EnAObj* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->dyna.actor, globalCtx)) {
        func_8001D234(self, self->dyna.actor.params);
    }
}

void func_8001D234(EnAObj* self, s16 params) {
    EnAObj_SetupAction(self, func_8001D25C);
}

void func_8001D25C(EnAObj* self, GlobalContext* globalCtx) {
    s16 var;

    if (self->dyna.actor.textId != 0) {
        var = self->dyna.actor.yawTowardsPlayer - self->dyna.actor.shape.rot.y;
        if ((ABS(var) < 0x2800) || ((self->dyna.actor.params == 0xA) && (ABS(var) > 0x5800))) {
            if (func_8002F194(&self->dyna.actor, globalCtx)) {
                EnAObj_SetupAction(self, func_8001D204);
            } else {
                func_8002F2F4(&self->dyna.actor, globalCtx);
            }
        }
    }
}

void func_8001D310(EnAObj* self, s16 params) {
    self->unk_16E = 0;
    self->unk_168 = 10;
    self->dyna.actor.world.rot.y = 0;
    self->dyna.actor.shape.rot = self->dyna.actor.world.rot;
    EnAObj_SetupAction(self, func_8001D360);
}

void func_8001D360(EnAObj* self, GlobalContext* globalCtx) {
    if (self->unk_16E == 0) {
        if (self->dyna.unk_160 != 0) {
            self->unk_16E++;
            self->unk_170 = 20;

            if ((s16)(self->dyna.actor.yawTowardsPlayer + 0x4000) < 0) {
                self->unk_174 = -1000;
            } else {
                self->unk_174 = 1000;
            }

            if (self->dyna.actor.yawTowardsPlayer < 0) {
                self->unk_172 = -self->unk_174;
            } else {
                self->unk_172 = self->unk_174;
            }
        }
    } else {
        if (self->unk_168 != 0) {
            self->unk_168--;
        } else {
            self->dyna.actor.shape.rot.y += self->unk_172;
            self->dyna.actor.shape.rot.x += self->unk_174;
            self->unk_170--;
            self->dyna.actor.gravity = -1.0f;

            if (self->unk_170 == 0) {
                self->dyna.actor.world.pos = self->dyna.actor.home.pos;
                self->unk_16E = 0;
                self->unk_168 = 10;
                self->dyna.actor.velocity.y = 0.0f;
                self->dyna.actor.gravity = 0.0f;
                self->dyna.actor.shape.rot = self->dyna.actor.world.rot;
            }
        }
    }
}

void func_8001D480(EnAObj* self, s16 params) {
    EnAObj_SetupAction(self, func_8001D4A8);
}

void func_8001D4A8(EnAObj* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->dyna.actor.speedXZ, 1.0f, 1.0f, 0.5f, 0.0f);
    self->dyna.actor.shape.rot.x = self->dyna.actor.shape.rot.x + (self->dyna.actor.world.rot.x >> 1);
    self->dyna.actor.shape.rot.z = self->dyna.actor.shape.rot.z + (self->dyna.actor.world.rot.z >> 1);

    if ((self->dyna.actor.speedXZ != 0.0f) && (self->dyna.actor.bgCheckFlags & 0x8)) {
        self->dyna.actor.world.rot.y =
            ((self->dyna.actor.wallYaw - self->dyna.actor.world.rot.y) + self->dyna.actor.wallYaw) - 0x8000;
        if (1) {} // Necessary to match
        self->dyna.actor.bgCheckFlags &= ~0x8;
    }

    if (self->dyna.actor.bgCheckFlags & 0x2) {
        if (self->dyna.actor.velocity.y < -8.0f) {
            self->dyna.actor.velocity.y *= -0.6f;
            self->dyna.actor.speedXZ *= 0.6f;
            self->dyna.actor.bgCheckFlags &= ~0x3;
        } else {
            Actor_Kill(&self->dyna.actor);
        }
    }
}

void func_8001D5C8(EnAObj* self, s16 params) {
    self->dyna.actor.uncullZoneDownward = 1200.0f;
    self->dyna.actor.uncullZoneScale = 720.0f;
    EnAObj_SetupAction(self, func_8001D608);
}

void func_8001D608(EnAObj* self, GlobalContext* globalCtx) {
    self->dyna.actor.speedXZ += self->dyna.unk_150;
    self->dyna.actor.world.rot.y = self->dyna.unk_158;
    self->dyna.actor.speedXZ = CLAMP(self->dyna.actor.speedXZ, -2.5f, 2.5f);

    Math_SmoothStepToF(&self->dyna.actor.speedXZ, 0.0f, 1.0f, 1.0f, 0.0f);

    if (self->dyna.actor.speedXZ != 0.0f) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_ROCK_SLIDE - SFX_FLAG);
    }

    self->dyna.unk_154 = 0.0f;
    self->dyna.unk_150 = 0.0f;
}

void EnAObj_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnAObj* self = THIS;

    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->dyna.actor);

    if (self->dyna.actor.gravity != 0.0f) {
        if (self->dyna.actor.params != A_OBJ_KNOB) {
            Actor_UpdateBgCheckInfo(globalCtx, &self->dyna.actor, 5.0f, 40.0f, 0.0f, 0x1D);
        } else {
            Actor_UpdateBgCheckInfo(globalCtx, &self->dyna.actor, 5.0f, 20.0f, 0.0f, 0x1D);
        }
    }

    self->dyna.actor.focus.pos = self->dyna.actor.world.pos;
    self->dyna.actor.focus.pos.y += self->unk_178;

    switch (self->dyna.actor.params) {
        case A_OBJ_SIGNPOST_OBLONG:
        case A_OBJ_SIGNPOST_ARROW:
            Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void EnAObj_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 type = thisx->params;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_a_keep.c", 701);

    func_80093D18(globalCtx->state.gfxCtx);

    if (type > A_OBJ_KNOB) {
        type = A_OBJ_KNOB;
    }

    if (thisx->params == A_OBJ_KNOB) {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 1, 60, 60, 60, 50);
    }

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_a_keep.c", 712),
              G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(POLY_OPA_DISP++, D_80115484[type]);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_a_keep.c", 715);
}
