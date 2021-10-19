/*
 * File: z_bg_menkuri_eye.c
 * Overlay: ovl_Bg_Menkuri_Eye
 * Description: Eye platform eye switches
 */

#include "z_bg_menkuri_eye.h"
#include "objects/object_menkuri_objects/object_menkuri_objects.h"

#define FLAGS 0x00000020

#define THIS ((BgMenkuriEye*)thisx)

void BgMenkuriEye_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMenkuriEye_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMenkuriEye_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMenkuriEye_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Bg_Menkuri_Eye_InitVars = {
    ACTOR_BG_MENKURI_EYE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MENKURI_OBJECTS,
    sizeof(BgMenkuriEye),
    (ActorFunc)BgMenkuriEye_Init,
    (ActorFunc)BgMenkuriEye_Destroy,
    (ActorFunc)BgMenkuriEye_Update,
    (ActorFunc)BgMenkuriEye_Draw,
};

static s32 D_8089C1A0;

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
        { 1, { { 0, 0, 0 }, 14 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgMenkuriEye_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgMenkuriEye* self = THIS;
    ColliderJntSphElement* colliderList;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->colliderItems);
    self->collider.elements[0].dim.worldSphere.center.x = self->actor.world.pos.x;
    self->collider.elements[0].dim.worldSphere.center.y = self->actor.world.pos.y;
    self->collider.elements[0].dim.worldSphere.center.z = self->actor.world.pos.z;
    colliderList = self->collider.elements;
    colliderList->dim.worldSphere.radius = colliderList->dim.modelSphere.radius;
    if (!Flags_GetSwitch(globalCtx, self->actor.params)) {
        D_8089C1A0 = 0;
    }
    self->framesUntilDisable = -1;
}

void BgMenkuriEye_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgMenkuriEye* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void BgMenkuriEye_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMenkuriEye* self = THIS;

    if (!Flags_GetSwitch(globalCtx, self->actor.params)) {
        if (self->framesUntilDisable != -1) {
            if (self->framesUntilDisable != 0) {
                self->framesUntilDisable -= 1;
            }
            if (self->framesUntilDisable == 0) {
                self->framesUntilDisable = -1;
                D_8089C1A0 -= 1;
            }
        }
    }
    if ((self->collider.base.acFlags & AC_HIT) &&
        (ABS((s16)(self->collider.base.ac->world.rot.y - self->actor.shape.rot.y)) > 0x5000)) {
        self->collider.base.acFlags &= ~AC_HIT;
        if (self->framesUntilDisable == -1) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_AMOS_DAMAGE);
            D_8089C1A0 += 1;
            D_8089C1A0 = CLAMP_MAX(D_8089C1A0, 4);
        }
        self->framesUntilDisable = 416;
        if (D_8089C1A0 == 4) {
            Flags_SetSwitch(globalCtx, self->actor.params);
            func_80078884(NA_SE_SY_CORRECT_CHIME);
        }
    }
    if (self->framesUntilDisable == -1) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    Actor_SetFocus(&self->actor, 0.0f);
}

void BgMenkuriEye_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgMenkuriEye* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_menkuri_eye.c", 292);
    func_80093D84(globalCtx->state.gfxCtx);
    if (Flags_GetSwitch(globalCtx, self->actor.params)) {
        gDPSetEnvColor(POLY_XLU_DISP++, 200, 0, 0, 255);
    } else if (self->framesUntilDisable == -1) {
        gDPSetEnvColor(POLY_XLU_DISP++, 200, 0, 0, 0);
    } else {
        gDPSetEnvColor(POLY_XLU_DISP++, 200, 0, 0, 255);
    }
    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, MTXMODE_APPLY);
    Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_menkuri_eye.c", 331),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_XLU_DISP++, gGTGEyeStatueEyeDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_menkuri_eye.c", 335);
}
