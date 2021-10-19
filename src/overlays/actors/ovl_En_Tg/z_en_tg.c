/*
 * File: z_en_tg.c
 * Overlay: ovl_En_Tg
 * Description: Dancing Couple
 */

#include "z_en_tg.h"
#include "objects/object_mu/object_mu.h"

#define FLAGS 0x00000009

#define THIS ((EnTg*)thisx)

void EnTg_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTg_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTg_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTg_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnTg_SpinIfNotTalking(EnTg* self, GlobalContext* globalCtx);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
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
    { 20, 64, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

const ActorInit En_Tg_InitVars = {
    ACTOR_EN_TG,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MU,
    sizeof(EnTg),
    (ActorFunc)EnTg_Init,
    (ActorFunc)EnTg_Destroy,
    (ActorFunc)EnTg_Update,
    (ActorFunc)EnTg_Draw,
};

u16 EnTg_GetTextId(GlobalContext* globalCtx, Actor* thisx) {
    EnTg* self = THIS;
    u16 temp;
    u32 phi;

    // If the player is wearing a mask, return a special reaction text
    temp = Text_GetFaceReaction(globalCtx, 0x24);
    if (temp != 0) {
        return temp;
    }
    // Use a different set of dialogue in Kakariko Village (Adult)
    if (globalCtx->sceneNum == SCENE_SPOT01) {
        if (self->nextDialogue % 2 != 0) {
            phi = 0x5089;
        } else {
            phi = 0x508A;
        }
        return phi;
    } else {
        if (self->nextDialogue % 2 != 0) {
            phi = 0x7025;
        } else {
            phi = 0x7026;
        }
        return phi;
    }
}

s16 EnTg_OnTextComplete(GlobalContext* globalCtx, Actor* thisx) {
    EnTg* self = THIS;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            return 1;
        case 2:
            switch (self->actor.textId) {
                case 0x5089:
                case 0x508A:
                    self->nextDialogue++;
                    break;
                case 0x7025:
                case 0x7026:
                    self->actor.params ^= 1;
                    self->nextDialogue++;
                    break;
            }
            return 0;
        default:
            return 1;
    }
}

void EnTg_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnTg* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 28.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gDancingCoupleSkel, &gDancingCoupleAnim, NULL, NULL, 0);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    self->actor.targetMode = 6;
    Actor_SetScale(&self->actor, 0.01f);
    self->nextDialogue = globalCtx->state.frames % 2;
    self->actionFunc = EnTg_SpinIfNotTalking;
}

void EnTg_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnTg* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnTg_SpinIfNotTalking(EnTg* self, GlobalContext* globalCtx) {
    if (!self->isTalking) {
        self->actor.shape.rot.y += 0x800;
    }
}

void EnTg_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnTg* self = THIS;
    s32 pad;
    f32 temp;
    Vec3s sp2C;

    sp2C.x = self->actor.world.pos.x;
    sp2C.y = self->actor.world.pos.y;
    sp2C.z = (s16)self->actor.world.pos.z + 3;
    self->collider.dim.pos = sp2C;
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    SkelAnime_Update(&self->skelAnime);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    self->actionFunc(self, globalCtx);
    temp = self->collider.dim.radius + 30.0f;
    func_800343CC(globalCtx, &self->actor, &self->isTalking, temp, EnTg_GetTextId, EnTg_OnTextComplete);
}

s32 EnTg_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    return false;
}

void EnTg_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnTg* self = THIS;
    Vec3f targetOffset = { 0.0f, 800.0f, 0.0f };

    if (limbIndex == 9) {
        // Place the target point at the guy's head instead of the center of the actor
        Matrix_MultVec3f(&targetOffset, &self->actor.focus.pos);
    }
}

Gfx* EnTg_SetColor(GraphicsContext* gfxCtx, u8 r, u8 g, u8 b, u8 a) {
    Gfx* displayList = Graph_Alloc(gfxCtx, 2 * sizeof(Gfx));

    gDPSetEnvColor(displayList, r, g, b, a);
    gSPEndDisplayList(displayList + 1);
    return displayList;
}

void EnTg_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnTg* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tg.c", 462);
    Matrix_Translate(0.0f, 0.0f, -560.0f, MTXMODE_APPLY);

    // Set the guy's shoes and shirt to royal blue
    gSPSegment(POLY_OPA_DISP++, 0x08, EnTg_SetColor(globalCtx->state.gfxCtx, 0, 50, 160, 0));

    // Set the girl's shirt to white
    gSPSegment(POLY_OPA_DISP++, 0x09, EnTg_SetColor(globalCtx->state.gfxCtx, 255, 255, 255, 0));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnTg_OverrideLimbDraw, EnTg_PostLimbDraw, self);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tg.c", 480);
}
