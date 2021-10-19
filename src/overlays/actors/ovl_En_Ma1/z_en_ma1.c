/*
 * File: z_en_ma1.c
 * Overlay: En_Ma1
 * Description: Child Malon
 */

#include "z_en_ma1.h"
#include "objects/object_ma1/object_ma1.h"

#define FLAGS 0x02000039

#define THIS ((EnMa1*)thisx)

void EnMa1_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMa1_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMa1_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMa1_Draw(Actor* thisx, GlobalContext* globalCtx);

u16 EnMa1_GetText(GlobalContext* globalCtx, Actor* self);
s16 func_80AA0778(GlobalContext* globalCtx, Actor* self);

void func_80AA0D88(EnMa1* self, GlobalContext* globalCtx);
void func_80AA0EA0(EnMa1* self, GlobalContext* globalCtx);
void func_80AA0EFC(EnMa1* self, GlobalContext* globalCtx);
void func_80AA0F44(EnMa1* self, GlobalContext* globalCtx);
void func_80AA106C(EnMa1* self, GlobalContext* globalCtx);
void func_80AA10EC(EnMa1* self, GlobalContext* globalCtx);
void func_80AA1150(EnMa1* self, GlobalContext* globalCtx);
void EnMa1_DoNothing(EnMa1* self, GlobalContext* globalCtx);

const ActorInit En_Ma1_InitVars = {
    ACTOR_EN_MA1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MA1,
    sizeof(EnMa1),
    (ActorFunc)EnMa1_Init,
    (ActorFunc)EnMa1_Destroy,
    (ActorFunc)EnMa1_Update,
    (ActorFunc)EnMa1_Draw,
};

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
    { 18, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static struct_D_80AA1678 sAnimationInfo[] = {
    { &gMalonChildIdleAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gMalonChildIdleAnim, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &gMalonChildSingAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gMalonChildSingAnim, 1.0f, ANIMMODE_LOOP, -10.0f },
};

static Vec3f D_80AA16B8 = { 800.0f, 0.0f, 0.0f };

static void* sMouthTextures[] = {
    gMalonChildNeutralMouthTex,
    gMalonChildSmilingMouthTex,
    gMalonChildTalkingMouthTex,
};

static void* sEyeTextures[] = {
    gMalonChildEyeOpenTex,
    gMalonChildEyeHalfTex,
    gMalonChildEyeClosedTex,
};

u16 EnMa1_GetText(GlobalContext* globalCtx, Actor* thisx) {
    u16 faceReaction = Text_GetFaceReaction(globalCtx, 0x17);
    if (faceReaction != 0) {
        return faceReaction;
    }
    if (CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
        return 0x204A;
    }
    if (gSaveContext.eventChkInf[1] & 0x40) {
        return 0x2049;
    }
    if (gSaveContext.eventChkInf[1] & 0x20) {
        if ((gSaveContext.infTable[8] & 0x20)) {
            return 0x2049;
        } else {
            return 0x2048;
        }
    }
    if (gSaveContext.eventChkInf[1] & 0x10) {
        return 0x2047;
    }
    if (gSaveContext.eventChkInf[1] & 4) {
        return 0x2044;
    }
    if (gSaveContext.infTable[8] & 0x10) {
        if (gSaveContext.infTable[8] & 0x800) {
            return 0x2043;
        } else {
            return 0x2042;
        }
    }
    return 0x2041;
}

s16 func_80AA0778(GlobalContext* globalCtx, Actor* thisx) {
    s16 ret = 1;
    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 2:
            switch (thisx->textId) {
                case 0x2041:
                    gSaveContext.infTable[8] |= 0x10;
                    gSaveContext.eventChkInf[1] |= 1;
                    ret = 0;
                    break;
                case 0x2043:
                    ret = 1;
                    break;
                case 0x2047:
                    gSaveContext.eventChkInf[1] |= 0x20;
                    ret = 0;
                    break;
                case 0x2048:
                    gSaveContext.infTable[8] |= 0x20;
                    ret = 0;
                    break;
                case 0x2049:
                    gSaveContext.eventChkInf[1] |= 0x40;
                    ret = 0;
                    break;
                case 0x2061:
                    ret = 2;
                    break;
                default:
                    ret = 0;
                    break;
            }
            break;
        case 4:
        case 5:
            if (func_80106BC8(globalCtx) != 0) {
                ret = 2;
            }
            break;
        case 6:
            if (func_80106BC8(globalCtx) != 0) {
                ret = 3;
            }
            break;
        case 0:
        case 1:
        case 3:
        case 7:
        case 8:
        case 9:
            ret = 1;
            break;
    }
    return ret;
}

s32 func_80AA08C4(EnMa1* self, GlobalContext* globalCtx) {
    if ((self->actor.shape.rot.z == 3) && (gSaveContext.sceneSetupIndex == 5)) {
        return 1;
    }
    if (!LINK_IS_CHILD) {
        return 0;
    }
    if (((globalCtx->sceneNum == SCENE_MARKET_NIGHT) || (globalCtx->sceneNum == SCENE_MARKET_DAY)) &&
        !(gSaveContext.eventChkInf[1] & 0x10) && !(gSaveContext.infTable[8] & 0x800)) {
        return 1;
    }
    if ((globalCtx->sceneNum == SCENE_SPOT15) && !(gSaveContext.eventChkInf[1] & 0x10)) {
        if (gSaveContext.infTable[8] & 0x800) {
            return 1;
        } else {
            gSaveContext.infTable[8] |= 0x800;
            return 0;
        }
    }
    if ((globalCtx->sceneNum == SCENE_SOUKO) && IS_NIGHT && (gSaveContext.eventChkInf[1] & 0x10)) {
        return 1;
    }
    if (globalCtx->sceneNum != SCENE_SPOT20) {
        return 0;
    }
    if ((self->actor.shape.rot.z == 3) && IS_DAY && (gSaveContext.eventChkInf[1] & 0x10)) {
        return 1;
    }
    return 0;
}

void EnMa1_UpdateEyes(EnMa1* self) {
    if (DECR(self->blinkTimer) == 0) {
        self->eyeIndex += 1;
        if (self->eyeIndex >= 3) {
            self->blinkTimer = Rand_S16Offset(30, 30);
            self->eyeIndex = 0;
        }
    }
}

void EnMa1_ChangeAnimation(EnMa1* self, UNK_TYPE idx) {
    f32 frameCount = Animation_GetLastFrame(sAnimationInfo[idx].animation);

    Animation_Change(&self->skelAnime, sAnimationInfo[idx].animation, 1.0f, 0.0f, frameCount, sAnimationInfo[idx].mode,
                     sAnimationInfo[idx].transitionRate);
}

void func_80AA0AF4(EnMa1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 phi_a3;

    if ((self->unk_1E8.unk_00 == 0) && (self->skelAnime.animation == &gMalonChildSingAnim)) {
        phi_a3 = 1;
    } else {
        phi_a3 = 0;
    }

    self->unk_1E8.unk_18 = player->actor.world.pos;
    self->unk_1E8.unk_18.y -= -10.0f;

    func_80034A14(&self->actor, &self->unk_1E8, 0, phi_a3);
}

void func_80AA0B74(EnMa1* self) {
    if (self->skelAnime.animation == &gMalonChildSingAnim) {
        if (self->unk_1E8.unk_00 == 0) {
            if (self->unk_1E0 != 0) {
                self->unk_1E0 = 0;
                func_800F6584(0);
            }
        } else {
            if (self->unk_1E0 == 0) {
                self->unk_1E0 = 1;
                func_800F6584(1);
            }
        }
    }
}

void EnMa1_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnMa1* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gMalonChildSkel, NULL, NULL, NULL, 0);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(22), &sColChkInfoInit);

    if (!func_80AA08C4(self, globalCtx)) {
        Actor_Kill(&self->actor);
        return;
    }

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.targetMode = 6;
    self->unk_1E8.unk_00 = 0;

    if (!(gSaveContext.eventChkInf[1] & 0x10) || CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
        self->actionFunc = func_80AA0D88;
        EnMa1_ChangeAnimation(self, 2);
    } else {
        self->actionFunc = func_80AA0F44;
        EnMa1_ChangeAnimation(self, 2);
    }
}

void EnMa1_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnMa1* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80AA0D88(EnMa1* self, GlobalContext* globalCtx) {
    if (self->unk_1E8.unk_00 != 0) {
        if (self->skelAnime.animation != &gMalonChildIdleAnim) {
            EnMa1_ChangeAnimation(self, 1);
        }
    } else {
        if (self->skelAnime.animation != &gMalonChildSingAnim) {
            EnMa1_ChangeAnimation(self, 3);
        }
    }

    if ((globalCtx->sceneNum == SCENE_SPOT15) && (gSaveContext.eventChkInf[1] & 0x10)) {
        Actor_Kill(&self->actor);
    } else if (!(gSaveContext.eventChkInf[1] & 0x10) || CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
        if (self->unk_1E8.unk_00 == 2) {
            self->actionFunc = func_80AA0EA0;
            globalCtx->msgCtx.unk_E3E7 = 4;
            globalCtx->msgCtx.msgMode = 0x36;
        }
    }
}

void func_80AA0EA0(EnMa1* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = func_80AA0EFC;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_WEIRD_EGG, 120.0f, 10.0f);
    }
}

void func_80AA0EFC(EnMa1* self, GlobalContext* globalCtx) {
    if (self->unk_1E8.unk_00 == 3) {
        self->unk_1E8.unk_00 = 0;
        self->actionFunc = func_80AA0D88;
        gSaveContext.eventChkInf[1] |= 4;
        globalCtx->msgCtx.msgMode = 0x36;
    }
}

void func_80AA0F44(EnMa1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->unk_1E8.unk_00 != 0) {
        if (self->skelAnime.animation != &gMalonChildIdleAnim) {
            EnMa1_ChangeAnimation(self, 1);
        }
    } else {
        if (self->skelAnime.animation != &gMalonChildSingAnim) {
            EnMa1_ChangeAnimation(self, 3);
        }
    }

    if (gSaveContext.eventChkInf[1] & 0x40) {
        if (player->stateFlags2 & 0x1000000) {
            player->stateFlags2 |= 0x2000000;
            player->unk_6A8 = &self->actor;
            self->actor.textId = 0x2061;
            func_8010B680(globalCtx, self->actor.textId, NULL);
            self->unk_1E8.unk_00 = 1;
            self->actor.flags |= 0x10000;
            self->actionFunc = func_80AA106C;
        } else if (self->actor.xzDistToPlayer < 30.0f + (f32)self->collider.dim.radius) {
            player->stateFlags2 |= 0x800000;
        }
    }
}

void func_80AA106C(EnMa1* self, GlobalContext* globalCtx) {
    GET_PLAYER(globalCtx)->stateFlags2 |= 0x800000;
    if (self->unk_1E8.unk_00 == 2) {
        func_800ED858(2);
        func_8010BD58(globalCtx, 9);
        self->actor.flags &= ~0x10000;
        self->actionFunc = func_80AA10EC;
    }
}

void func_80AA10EC(EnMa1* self, GlobalContext* globalCtx) {
    GET_PLAYER(globalCtx)->stateFlags2 |= 0x800000;
    if (func_8010BDBC(&globalCtx->msgCtx) == 7) {
        func_8010BD58(globalCtx, 0x16);
        self->actionFunc = func_80AA1150;
    }
}

void func_80AA1150(EnMa1* self, GlobalContext* globalCtx) {
    GET_PLAYER(globalCtx)->stateFlags2 |= 0x800000;
    if (globalCtx->msgCtx.unk_E3EE == 3) {
        globalCtx->nextEntranceIndex = 0x157;
        gSaveContext.nextCutsceneIndex = 0xFFF1;
        globalCtx->fadeTransition = 42;
        globalCtx->sceneLoadFlag = 0x14;
        self->actionFunc = EnMa1_DoNothing;
    }
}

void EnMa1_DoNothing(EnMa1* self, GlobalContext* globalCtx) {
}

void EnMa1_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnMa1* self = THIS;
    s32 pad;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    SkelAnime_Update(&self->skelAnime);
    EnMa1_UpdateEyes(self);
    self->actionFunc(self, globalCtx);
    if (self->actionFunc != EnMa1_DoNothing) {
        func_800343CC(globalCtx, &self->actor, &self->unk_1E8.unk_00, (f32)self->collider.dim.radius + 30.0f,
                      EnMa1_GetText, func_80AA0778);
    }
    func_80AA0B74(self);
    func_80AA0AF4(self, globalCtx);
}

s32 EnMa1_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnMa1* self = THIS;
    Vec3s vec;

    if ((limbIndex == 2) || (limbIndex == 5)) {
        *dList = NULL;
    }
    if (limbIndex == 15) {
        Matrix_Translate(1400.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        vec = self->unk_1E8.unk_08;
        Matrix_RotateX((vec.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((vec.x / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_Translate(-1400.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }
    if (limbIndex == 8) {
        vec = self->unk_1E8.unk_0E;
        Matrix_RotateX((-vec.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((-vec.x / 32768.0f) * M_PI, MTXMODE_APPLY);
    }
    return false;
}

void EnMa1_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnMa1* self = THIS;
    Vec3f vec = D_80AA16B8;

    if (limbIndex == 15) {
        Matrix_MultVec3f(&vec, &self->actor.focus.pos);
    }
}

void EnMa1_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnMa1* self = THIS;
    Camera* camera;
    f32 distFromCamera;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ma1.c", 1226);

    camera = GET_ACTIVE_CAM(globalCtx);
    distFromCamera = Math_Vec3f_DistXZ(&self->actor.world.pos, &camera->eye);
    func_800F6268(distFromCamera, 0x2F);
    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sMouthTextures[self->mouthIndex]));
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeIndex]));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnMa1_OverrideLimbDraw, EnMa1_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ma1.c", 1261);
}
