/*
 * File: z_en_js.c
 * Overlay: ovl_En_Js
 * Description: Magic Carpet Salesman
 */

#include "z_en_js.h"
#include "objects/object_js/object_js.h"

#define FLAGS 0x00000009

#define THIS ((EnJs*)thisx)

void EnJs_Init(Actor* thisx, GlobalContext* globalCtx);
void EnJs_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnJs_Update(Actor* thisx, GlobalContext* globalCtx);
void EnJs_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A89304(EnJs* self, GlobalContext* globalCtx);

const ActorInit En_Js_InitVars = {
    ACTOR_EN_JS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_JS,
    sizeof(EnJs),
    (ActorFunc)EnJs_Init,
    (ActorFunc)EnJs_Destroy,
    (ActorFunc)EnJs_Update,
    (ActorFunc)EnJs_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 40, 0, { 0, 0, 0 } },
};

void En_Js_SetupAction(EnJs* self, EnJsActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnJs_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnJs* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 36.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gCarpetMerchantSkel, &gCarpetMerchantSlappingKneeAnim,
                       self->jointTable, self->morphTable, 13);
    Animation_PlayOnce(&self->skelAnime, &gCarpetMerchantSlappingKneeAnim);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&self->actor, 0.01f);
    En_Js_SetupAction(self, func_80A89304);
    self->unk_284 = 0;
    self->actor.gravity = -1.0f;
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_JSJUTAN, self->actor.world.pos.x,
                       self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
}

void EnJs_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnJs* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

u8 func_80A88F64(EnJs* self, GlobalContext* globalCtx, u16 textId) {
    s16 yawDiff;

    if (func_8002F194(&self->actor, globalCtx)) {
        return 1;
    } else {
        self->actor.textId = textId;
        yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if (ABS(yawDiff) <= 0x1800 && self->actor.xzDistToPlayer < 100.0f) {
            self->unk_284 |= 1;
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
        }
        return 0;
    }
}

void func_80A89008(EnJs* self) {
    En_Js_SetupAction(self, func_80A89304);
    Animation_Change(&self->skelAnime, &gCarpetMerchantSlappingKneeAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gCarpetMerchantSlappingKneeAnim), ANIMMODE_ONCE, -4.0f);
}

void func_80A89078(EnJs* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        func_80A89008(self);
        self->actor.flags &= ~0x10000;
    }
}

void func_80A890C0(EnJs* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        En_Js_SetupAction(self, func_80A89078);
    } else {
        func_8002F2CC(&self->actor, globalCtx, 1000.0f);
    }
}

void func_80A8910C(EnJs* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actor.textId = 0x6078;
        En_Js_SetupAction(self, func_80A890C0);
        self->actor.flags |= 0x10000;
    }
}

void func_80A89160(EnJs* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        En_Js_SetupAction(self, func_80A8910C);
    } else {
        func_8002F434(&self->actor, globalCtx, GI_BOMBCHUS_10, 10000.0f, 50.0f);
    }
}

void func_80A891C4(EnJs* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // yes
                if (gSaveContext.rupees < 200) {
                    func_8010B720(globalCtx, 0x6075);
                    func_80A89008(self);
                } else {
                    Rupees_ChangeBy(-200);
                    En_Js_SetupAction(self, func_80A89160);
                }
                break;
            case 1: // no
                func_8010B720(globalCtx, 0x6074);
                func_80A89008(self);
        }
    }
}

void func_80A89294(EnJs* self) {
    En_Js_SetupAction(self, func_80A891C4);
    Animation_Change(&self->skelAnime, &gCarpetMerchantIdleAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gCarpetMerchantIdleAnim), ANIMMODE_ONCE, -4.0f);
}

void func_80A89304(EnJs* self, GlobalContext* globalCtx) {
    if (func_80A88F64(self, globalCtx, 0x6077)) {
        func_80A89294(self);
    }
}

void EnJs_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnJs* self = THIS;
    s32 pad;
    s32 pad2;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    if (self->actor.bgCheckFlags & 1) {
        if (SurfaceType_GetSfx(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId) == 1) {
            Math_ApproachF(&self->actor.shape.yOffset, sREG(80) + -2000.0f, 1.0f, (sREG(81) / 10.0f) + 50.0f);
        }
    } else {
        Math_ApproachZeroF(&self->actor.shape.yOffset, 1.0f, (sREG(81) / 10.0f) + 50.0f);
    }
    if (SkelAnime_Update(&self->skelAnime)) {
        self->skelAnime.curFrame = 0.0f;
    }
    self->actionFunc(self, globalCtx);
    if (self->unk_284 & 1) {
        func_80038290(globalCtx, &self->actor, &self->unk_278, &self->unk_27E, self->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&self->unk_278.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&self->unk_278.y, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&self->unk_27E.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&self->unk_27E.y, 0, 6, 0x1838, 0x64);
    }
    self->unk_284 &= ~0x1;

    if (DECR(self->unk_288) == 0) {
        self->unk_288 = Rand_S16Offset(0x3C, 0x3C);
    }

    self->unk_286 = self->unk_288;

    if (self->unk_286 >= 3) {
        self->unk_286 = 0;
    }
}

s32 EnJs_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnJs* self = THIS;

    if (limbIndex == 12) {
        rot->y -= self->unk_278.y;
    }
    return false;
}

void EnJs_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f D_80A896DC = { 0.0f, 0.0f, 0.0f };
    EnJs* self = THIS;

    if (limbIndex == 12) {
        Matrix_MultVec3f(&D_80A896DC, &self->actor.focus.pos);
    }
}
void EnJs_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnJs* self = THIS;

    func_800943C8(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnJs_OverrideLimbDraw, EnJs_PostLimbDraw, self);
}
