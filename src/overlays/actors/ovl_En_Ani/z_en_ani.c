/*
 * File: z_en_ani.c
 * Overlay: ovl_En_Ani
 * Description: Kakariko Roof Guy
 */

#include "z_en_ani.h"
#include "objects/object_ani/object_ani.h"

#define FLAGS 0x00000009

#define THIS ((EnAni*)thisx)

void EnAni_Init(Actor* thisx, GlobalContext* globalCtx);
void EnAni_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnAni_Update(Actor* thisx, GlobalContext* globalCtx);
void EnAni_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 EnAni_SetText(EnAni* self, GlobalContext* globalCtx, u16 textId);
void func_809B04F0(EnAni* self, GlobalContext* globalCtx);
void func_809B0524(EnAni* self, GlobalContext* globalCtx);
void func_809B0558(EnAni* self, GlobalContext* globalCtx);
void func_809B05F0(EnAni* self, GlobalContext* globalCtx);
void func_809B064C(EnAni* self, GlobalContext* globalCtx);
void func_809B07F8(EnAni* self, GlobalContext* globalCtx);
void func_809B0988(EnAni* self, GlobalContext* globalCtx);
void func_809B0994(EnAni* self, GlobalContext* globalCtx);
void func_809B0A28(EnAni* self, GlobalContext* globalCtx);
void func_809B0A6C(EnAni* self, GlobalContext* globalCtx);

const ActorInit En_Ani_InitVars = {
    ACTOR_EN_ANI,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ANI,
    sizeof(EnAni),
    (ActorFunc)EnAni_Init,
    (ActorFunc)EnAni_Destroy,
    (ActorFunc)EnAni_Update,
    (ActorFunc)EnAni_Draw,
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
    { 30, 40, 0, { 0 } },
};

void EnAni_SetupAction(EnAni* self, EnAniActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 850, ICHAIN_STOP),
};

void EnAni_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnAni* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, -2800.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gRoofManSkel, &gRoofManIdleAnim, self->jointTable,
                       self->morphTable, 0x10);
    Animation_PlayOnce(&self->skelAnime, &gRoofManIdleAnim);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    if (!LINK_IS_ADULT) {
        EnAni_SetupAction(self, func_809B064C);
    } else {
        EnAni_SetupAction(self, func_809B07F8);
    }
    self->unk_2AA = 0;
    self->unk_2A8 = 0;
    self->actor.minVelocityY = -1.0f;
    self->actor.velocity.y = -1.0f;
}

void EnAni_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnAni* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 EnAni_SetText(EnAni* self, GlobalContext* globalCtx, u16 textId) {
    self->actor.textId = textId;
    self->unk_2A8 |= 1;
    func_8002F2CC(&self->actor, globalCtx, 100.0f);
    return 0;
}

void func_809B04F0(EnAni* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        EnAni_SetupAction(self, func_809B064C);
    }
}

void func_809B0524(EnAni* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        EnAni_SetupAction(self, func_809B07F8);
    }
}

void func_809B0558(EnAni* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        if (!LINK_IS_ADULT) {
            EnAni_SetupAction(self, func_809B04F0);
        } else {
            EnAni_SetupAction(self, func_809B0524);
        }
        gSaveContext.itemGetInf[1] |= 0x20;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_HEART_PIECE, 10000.0f, 200.0f);
    }
}

void func_809B05F0(EnAni* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        EnAni_SetupAction(self, func_809B0558);
    }
    func_8002F434(&self->actor, globalCtx, GI_HEART_PIECE, 10000.0f, 200.0f);
}

void func_809B064C(EnAni* self, GlobalContext* globalCtx) {
    u16 textId;
    s16 yawDiff;
    u16 textId2;

    textId2 = Text_GetFaceReaction(globalCtx, 0xA);
    textId = textId2 & 0xFFFF;

    if (!textId) {}

    if (textId2 == 0) {
        textId = !IS_DAY ? 0x5051 : 0x5050;
    }

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (func_8002F194(&self->actor, globalCtx)) {
        if (self->actor.textId == 0x5056) {
            EnAni_SetupAction(self, func_809B04F0);
        } else if (self->actor.textId == 0x5055) {
            EnAni_SetupAction(self, func_809B05F0);
        } else {
            EnAni_SetupAction(self, func_809B04F0);
        }
    } else if (yawDiff >= -0x36AF && yawDiff < 0 && self->actor.xzDistToPlayer < 150.0f &&
               -80.0f < self->actor.yDistToPlayer) {
        if (gSaveContext.itemGetInf[1] & 0x20) {
            EnAni_SetText(self, globalCtx, 0x5056);
        } else {
            EnAni_SetText(self, globalCtx, 0x5055);
        }
    } else if (yawDiff >= -0x3E7 && yawDiff < 0x36B0 && self->actor.xzDistToPlayer < 350.0f) {
        EnAni_SetText(self, globalCtx, textId);
    }
}

void func_809B07F8(EnAni* self, GlobalContext* globalCtx) {
    s16 pad;
    s16 yawDiff;
    u16 textId;

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if (func_8002F194(&self->actor, globalCtx)) {
        if (self->actor.textId == 0x5056) {
            EnAni_SetupAction(self, func_809B0524);
        } else if (self->actor.textId == 0x5055) {
            EnAni_SetupAction(self, func_809B05F0);
        } else {
            EnAni_SetupAction(self, func_809B0524);
        }
    } else if (yawDiff > -0x36B0 && yawDiff < 0 && self->actor.xzDistToPlayer < 150.0f &&
               -80.0f < self->actor.yDistToPlayer) {
        if (gSaveContext.itemGetInf[1] & 0x20) {
            EnAni_SetText(self, globalCtx, 0x5056);
        } else {
            EnAni_SetText(self, globalCtx, 0x5055);
        }
    } else if (yawDiff > -0x3E8 && yawDiff < 0x36B0 && self->actor.xzDistToPlayer < 350.0f) {
        if (!(gSaveContext.eventChkInf[2] & 0x8000)) {
            textId = 0x5052;
        } else {
            textId = (gSaveContext.itemGetInf[1] & 0x20) ? 0x5054 : 0x5053;
        }
        EnAni_SetText(self, globalCtx, textId);
    }
}

void func_809B0988(EnAni* self, GlobalContext* globalCtx) {
}

void func_809B0994(EnAni* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.npcActions[0]->action == 4) {
        Animation_Change(&self->skelAnime, &gRoofManGettingUpAfterKnockbackAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gRoofManGettingUpAfterKnockbackAnim), ANIMMODE_ONCE, -4.0f);
        self->unk_2AA++;
        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
    }
}

void func_809B0A28(EnAni* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->unk_2AA++;
    }
}

void func_809B0A6C(EnAni* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->skelAnime.curFrame = 0.0f;
    }
    if (globalCtx->csCtx.npcActions[0]->action == 2) {
        Animation_Change(&self->skelAnime, &gRoofManKnockbackAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gRoofManKnockbackAnim), ANIMMODE_ONCE, 0.0f);
        self->actor.shape.shadowDraw = NULL;
        self->unk_2AA++;
    }
}

void EnAni_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnAni* self = THIS;
    s32 pad[2];

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    if ((globalCtx->csCtx.state != CS_STATE_IDLE) && (globalCtx->csCtx.npcActions[0] != NULL)) {
        switch (self->unk_2AA) {
            case 0:
                func_809B0A6C(self, globalCtx);
                break;
            case 1:
                func_809B0A28(self, globalCtx);
                break;
            case 2:
                func_809B0994(self, globalCtx);
                break;
            case 3:
                func_809B0A28(self, globalCtx);
                break;
            case 4:
                func_809B0988(self, globalCtx);
                break;
        }

        if (globalCtx->csCtx.frames == 100) {
            func_800788CC(NA_SE_IT_EARTHQUAKE);
        }
    } else {
        if (SkelAnime_Update(&self->skelAnime) != 0) {
            self->skelAnime.curFrame = 0.0f;
        }
        self->actionFunc(self, globalCtx);
    }

    if (self->unk_2A8 & 1) {
        func_80038290(globalCtx, &self->actor, &self->unk_29C, &self->unk_2A2, self->actor.focus.pos);
        self->unk_2A2.z = 0;
        self->unk_2A2.y = self->unk_2A2.z;
        self->unk_2A2.x = self->unk_2A2.z;
    } else {
        Math_SmoothStepToS(&self->unk_29C.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_29C.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_2A2.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_2A2.y, 0, 6, 6200, 100);
    }

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset(60, 60);
    }
    self->eyeIndex = self->blinkTimer;
    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}

s32 EnAni_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnAni* self = THIS;

    if (limbIndex == 15) {
        rot->x += self->unk_29C.y;
        rot->z += self->unk_29C.x;
    }
    return false;
}

void EnAni_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f sMultVec = { 800.0f, 500.0f, 0.0f };
    EnAni* self = THIS;

    if (limbIndex == 15) {
        Matrix_MultVec3f(&sMultVec, &self->actor.focus.pos);
    }
}

void EnAni_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = {
        gRoofManEyeOpenTex,
        gRoofManEyeHalfTex,
        gRoofManEyeClosedTex,
    };
    EnAni* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ani.c", 719);

    func_800943C8(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeIndex]));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnAni_OverrideLimbDraw, EnAni_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ani.c", 736);
}
