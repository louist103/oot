/*
 * File: z_en_ge3.c
 * Overlay: ovl_En_Ge3
 * Description: Gerudo giving you membership card
 */

#include "z_en_ge3.h"
#include "objects/object_geldb/object_geldb.h"

#define FLAGS 0x00000019

#define THIS ((EnGe3*)thisx)

void EnGe3_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGe3_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGe3_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGe3_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnGe3_WaitLookAtPlayer(EnGe3* self, GlobalContext* globalCtx);
void EnGe3_ForceTalk(EnGe3* self, GlobalContext* globalCtx);
void EnGe3_UpdateWhenNotTalking(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Ge3_InitVars = {
    ACTOR_EN_GE3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GELDB,
    sizeof(EnGe3),
    (ActorFunc)EnGe3_Init,
    (ActorFunc)EnGe3_Destroy,
    (ActorFunc)EnGe3_Update,
    (ActorFunc)EnGe3_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000722, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 50, 0, { 0, 0, 0 } },
};

static EnGe3ActionFunc sActionFuncs[] = { EnGe3_WaitLookAtPlayer };
static AnimationHeader* sAnimations[] = { &gGerudoRedStandAnim }; // Idle with right hand on hip and left over mouth
static u8 sAnimationModes[] = { ANIMMODE_LOOP };

void EnGe3_ChangeAction(EnGe3* self, s32 i) {
    self->actionFunc = sActionFuncs[i];

    Animation_Change(&self->skelAnime, sAnimations[i], 1.0f, 0.0f, (f32)Animation_GetLastFrame(sAnimations[i]),
                     sAnimationModes[i], -8.0f);

    self->unk_30C &= ~2;
}

void EnGe3_Init(Actor* thisx, GlobalContext* globalCtx2) {
    EnGe3* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGerudoRedSkel, NULL, self->jointTable, self->morphTable,
                       GELDB_LIMB_MAX);
    Animation_PlayLoop(&self->skelAnime, &gGerudoRedStandAnim);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.world.rot.z = 0;
    self->actor.shape.rot.z = 0;
    EnGe3_ChangeAction(self, 0);
    self->actionFunc = EnGe3_ForceTalk;
    self->unk_30C = 0;
    self->actor.targetMode = 6;
    self->actor.minVelocityY = -4.0f;
    self->actor.gravity = -1.0f;
}

void EnGe3_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGe3* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnGe3_TurnToFacePlayer(EnGe3* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 angleDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(angleDiff) <= 0x4000) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 6, 4000, 100);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        func_80038290(globalCtx, &self->actor, &self->headRot, &self->unk_306, self->actor.focus.pos);
    } else {
        if (angleDiff < 0) {
            Math_SmoothStepToS(&self->headRot.y, -0x2000, 6, 6200, 0x100);
        } else {
            Math_SmoothStepToS(&self->headRot.y, 0x2000, 6, 6200, 0x100);
        }

        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 12, 1000, 100);
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
}

void EnGe3_LookAtPlayer(EnGe3* self, GlobalContext* globalCtx) {
    if ((ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) <= 0x2300) &&
        (self->actor.xzDistToPlayer < 100.0f)) {
        func_80038290(globalCtx, &self->actor, &self->headRot, &self->unk_306, self->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&self->headRot.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->headRot.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_306.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_306.y, 0, 6, 6200, 100);
    }
}

void EnGe3_Wait(EnGe3* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actionFunc = EnGe3_WaitLookAtPlayer;
        self->actor.update = EnGe3_UpdateWhenNotTalking;
        self->actor.flags &= ~0x10000;
    }
    EnGe3_TurnToFacePlayer(self, globalCtx);
}

void EnGe3_WaitLookAtPlayer(EnGe3* self, GlobalContext* globalCtx) {
    EnGe3_LookAtPlayer(self, globalCtx);
}

void EnGe3_WaitTillCardGiven(EnGe3* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = EnGe3_Wait;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_GERUDO_CARD, 10000.0f, 50.0f);
    }
}

void EnGe3_GiveCard(EnGe3* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->actor.flags &= ~0x10000;
        self->actionFunc = EnGe3_WaitTillCardGiven;
        func_8002F434(&self->actor, globalCtx, GI_GERUDO_CARD, 10000.0f, 50.0f);
    }
}

void EnGe3_ForceTalk(EnGe3* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnGe3_GiveCard;
    } else {
        if (!(self->unk_30C & 4)) {
            func_8002DF54(globalCtx, &self->actor, 7);
            self->unk_30C |= 4;
        }
        self->actor.textId = 0x6004;
        self->actor.flags |= 0x10000;
        func_8002F1C4(&self->actor, globalCtx, 300.0f, 300.0f, 0);
    }
    EnGe3_LookAtPlayer(self, globalCtx);
}

void EnGe3_UpdateCollision(EnGe3* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 40.0f, 25.0f, 40.0f, 5);

    if (!(self->unk_30C & 2) && SkelAnime_Update(&self->skelAnime)) {
        self->unk_30C |= 2;
    }
}

void EnGe3_MoveAndBlink(EnGe3* self, GlobalContext* globalCtx) {

    Actor_MoveForward(&self->actor);

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset(60, 60);
    }

    self->eyeIndex = self->blinkTimer;

    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}

void EnGe3_UpdateWhenNotTalking(Actor* thisx, GlobalContext* globalCtx) {
    EnGe3* self = THIS;

    EnGe3_UpdateCollision(self, globalCtx);
    self->actionFunc(self, globalCtx);

    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnGe3_Wait;
        self->actor.update = EnGe3_Update;
    } else {
        self->actor.textId = 0x6005;
        if (self->actor.xzDistToPlayer < 100.0f) {
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
        }
    }

    EnGe3_MoveAndBlink(self, globalCtx);
}

void EnGe3_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGe3* self = THIS;

    EnGe3_UpdateCollision(self, globalCtx);
    self->actionFunc(self, globalCtx);
    EnGe3_MoveAndBlink(self, globalCtx);
}

s32 EnGe3_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnGe3* self = THIS;

    switch (limbIndex) {
        // Hide swords and veil from object_geldb
        case GELDB_LIMB_VEIL:
        case GELDB_LIMB_R_SWORD:
        case GELDB_LIMB_L_SWORD:
            *dList = NULL;
            return false;
        // Turn head
        case GELDB_LIMB_HEAD:
            rot->x += self->headRot.y;

        // This is a hack to fix the color-changing clothes self Gerudo has on N64 versions
        default:
            OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge3.c", 547);
            switch (limbIndex) {
                case GELDB_LIMB_NECK:
                    break;
                case GELDB_LIMB_HEAD:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetEnvColor(POLY_OPA_DISP++, 80, 60, 10, 255);
                    break;
                case GELDB_LIMB_R_SWORD:
                case GELDB_LIMB_L_SWORD:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetEnvColor(POLY_OPA_DISP++, 140, 170, 230, 255);
                    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
                    break;
                default:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetEnvColor(POLY_OPA_DISP++, 140, 0, 0, 255);
                    break;
            }
            CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge3.c", 566);
            break;
    }
    return false;
}

void EnGe3_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnGe3* self = THIS;
    Vec3f D_80A351C8 = { 600.0f, 700.0f, 0.0f };

    if (limbIndex == GELDB_LIMB_HEAD) {
        Matrix_MultVec3f(&D_80A351C8, &self->actor.focus.pos);
    }
}

void EnGe3_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    static void* eyeTextures[] = {
        0x06005FE8, // Half-open
        0x060065A8, // Quarter-open
        0x06006D28, // Closed
    };
    EnGe3* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge3.c", 614);

    func_800943C8(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeIndex]));
    func_8002EBCC(&self->actor, globalCtx, 0);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnGe3_OverrideLimbDraw, EnGe3_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge3.c", 631);
}
