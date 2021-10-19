/*
 * File: z_en_sth.c
 * Overlay: ovl_En_Sth
 * Description: Uncursed House of Skulltula People
 */

#include "vt.h"
#include "z_en_sth.h"

#define FLAGS 0x00000019

#define THIS ((EnSth*)thisx)

void EnSth_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSth_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSth_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSth_Update2(Actor* thisx, GlobalContext* globalCtx);
void EnSth_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnSth_WaitForObjectLoaded(EnSth* self, GlobalContext* globalCtx);
void EnSth_ParentRewardObtainedWait(EnSth* self, GlobalContext* globalCtx);
void EnSth_RewardUnobtainedWait(EnSth* self, GlobalContext* globalCtx);
void EnSth_ChildRewardObtainedWait(EnSth* self, GlobalContext* globalCtx);

const ActorInit En_Sth_InitVars = {
    ACTOR_EN_STH,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnSth),
    (ActorFunc)EnSth_Init,
    (ActorFunc)EnSth_Destroy,
    (ActorFunc)EnSth_Update,
    NULL,
};

#include "z_en_sth_gfx.c"

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

static s16 sObjectIds[6] = {
    OBJECT_AHG, OBJECT_BOJ, OBJECT_BOJ, OBJECT_BOJ, OBJECT_BOJ, OBJECT_BOJ,
};

static FlexSkeletonHeader* sSkeletons[6] = {
    0x060000F0, 0x060000F0, 0x060000F0, 0x060000F0, 0x060000F0, 0x060000F0,
};

static AnimationHeader* sAnimations[6] = {
    &gParentDanceAnim, &gChildDanceAnim, &gChildDanceAnim, &gChildDanceAnim, &gChildDanceAnim, &gChildDanceAnim,
};

static EnSthActionFunc sRewardObtainedWaitActions[6] = {
    EnSth_ParentRewardObtainedWait, EnSth_ChildRewardObtainedWait, EnSth_ChildRewardObtainedWait,
    EnSth_ChildRewardObtainedWait,  EnSth_ChildRewardObtainedWait, EnSth_ChildRewardObtainedWait,
};

static u16 sEventFlags[6] = {
    0x0000, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000,
};

static s16 sGetItemIds[6] = {
    GI_RUPEE_GOLD, GI_WALLET_ADULT, GI_STONE_OF_AGONY, GI_WALLET_GIANT, GI_BOMBCHUS_10, GI_HEART_PIECE,
};

static Vec3f D_80B0B49C = { 700.0f, 400.0f, 0.0f };

static Color_RGB8 sTunicColors[6] = {
    { 190, 110, 0 }, { 0, 180, 110 }, { 0, 255, 80 }, { 255, 160, 60 }, { 190, 230, 250 }, { 240, 230, 120 },
};

void EnSth_SetupAction(EnSth* self, EnSthActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnSth_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSth* self = THIS;

    s16 objectId;
    s32 params = self->actor.params;
    s32 objectBankIdx;

    osSyncPrintf(VT_FGCOL(BLUE) "金スタル屋 no = %d\n" VT_RST, params); // "Gold Skulltula Shop"
    if (self->actor.params == 0) {
        if (gSaveContext.inventory.gsTokens < 100) {
            Actor_Kill(&self->actor);
            // "Gold Skulltula Shop I still can't be a human"
            osSyncPrintf("金スタル屋 まだ 人間に戻れない \n");
            return;
        }
    } else if (gSaveContext.inventory.gsTokens < (self->actor.params * 10)) {
        Actor_Kill(&self->actor);
        // "Gold Skulltula Shop I still can't be a human"
        osSyncPrintf(VT_FGCOL(BLUE) "金スタル屋 まだ 人間に戻れない \n" VT_RST);
        return;
    }

    objectId = sObjectIds[params];
    if (objectId != 1) {
        objectBankIdx = Object_GetIndex(&globalCtx->objectCtx, objectId);
    } else {
        objectBankIdx = 0;
    }

    osSyncPrintf("bank_ID = %d\n", objectBankIdx);
    if (objectBankIdx < 0) {
        ASSERT(0, "0", "../z_en_sth.c", 1564);
    }
    self->objectBankIdx = objectBankIdx;
    self->drawFunc = EnSth_Draw;
    Actor_SetScale(&self->actor, 0.01f);
    EnSth_SetupAction(self, EnSth_WaitForObjectLoaded);
    self->actor.draw = NULL;
    self->unk_2B2 = 0;
    self->actor.targetMode = 6;
}

void EnSth_SetupShapeColliderUpdate2AndDraw(EnSth* self, GlobalContext* globalCtx) {
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.update = EnSth_Update2;
    self->actor.draw = self->drawFunc;
}

void EnSth_SetupAfterObjectLoaded(EnSth* self, GlobalContext* globalCtx) {
    s32 pad;
    s16* params;

    EnSth_SetupShapeColliderUpdate2AndDraw(self, globalCtx);
    gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[self->objectBankIdx].segment);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, sSkeletons[self->actor.params], NULL, self->jointTable,
                       self->morphTable, 16);
    Animation_PlayLoop(&self->skelAnime, sAnimations[self->actor.params]);

    self->eventFlag = sEventFlags[self->actor.params];
    params = &self->actor.params;
    if (gSaveContext.eventChkInf[13] & self->eventFlag) {
        EnSth_SetupAction(self, sRewardObtainedWaitActions[*params]);
    } else {
        EnSth_SetupAction(self, EnSth_RewardUnobtainedWait);
    }
}

void EnSth_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnSth* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnSth_WaitForObjectLoaded(EnSth* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->objectBankIdx)) {
        self->actor.objBankIndex = self->objectBankIdx;
        self->actionFunc = EnSth_SetupAfterObjectLoaded;
    }
}

void EnSth_FacePlayer(EnSth* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 diffRot = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (ABS(diffRot) <= 0x4000) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 6, 0xFA0, 0x64);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        func_80038290(globalCtx, &self->actor, &self->headRot, &self->unk_2AC, self->actor.focus.pos);
    } else {
        if (diffRot < 0) {
            Math_SmoothStepToS(&self->headRot.y, -0x2000, 6, 0x1838, 0x100);
        } else {
            Math_SmoothStepToS(&self->headRot.y, 0x2000, 6, 0x1838, 0x100);
        }
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xC, 0x3E8, 0x64);
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
}

void EnSth_LookAtPlayer(EnSth* self, GlobalContext* globalCtx) {
    s16 diffRot = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if ((ABS(diffRot) <= 0x4300) && (self->actor.xzDistToPlayer < 100.0f)) {
        func_80038290(globalCtx, &self->actor, &self->headRot, &self->unk_2AC, self->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&self->headRot.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&self->headRot.y, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&self->unk_2AC.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&self->unk_2AC.y, 0, 6, 0x1838, 0x64);
    }
}

void EnSth_RewardObtainedTalk(EnSth* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        if (self->actor.params == 0) {
            EnSth_SetupAction(self, EnSth_ParentRewardObtainedWait);
        } else {
            EnSth_SetupAction(self, EnSth_ChildRewardObtainedWait);
        }
    }
    EnSth_FacePlayer(self, globalCtx);
}

void EnSth_ParentRewardObtainedWait(EnSth* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        EnSth_SetupAction(self, EnSth_RewardObtainedTalk);
    } else {
        self->actor.textId = 0x23;
        if (self->actor.xzDistToPlayer < 100.0f) {
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
        }
    }
    EnSth_LookAtPlayer(self, globalCtx);
}

void EnSth_GivePlayerItem(EnSth* self, GlobalContext* globalCtx) {
    u16 getItemId = sGetItemIds[self->actor.params];

    switch (self->actor.params) {
        case 1:
        case 3:
            switch (CUR_UPG_VALUE(UPG_WALLET)) {
                case 0:
                    getItemId = GI_WALLET_ADULT;
                    break;

                case 1:
                    getItemId = GI_WALLET_GIANT;
                    break;
            }
            break;
    }

    func_8002F434(&self->actor, globalCtx, getItemId, 10000.0f, 50.0f);
}

void EnSth_GiveReward(EnSth* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        EnSth_SetupAction(self, EnSth_RewardObtainedTalk);
        gSaveContext.eventChkInf[13] |= self->eventFlag;
    } else {
        EnSth_GivePlayerItem(self, globalCtx);
    }
    EnSth_FacePlayer(self, globalCtx);
}

void EnSth_RewardUnobtainedTalk(EnSth* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);
        EnSth_SetupAction(self, EnSth_GiveReward);
        EnSth_GivePlayerItem(self, globalCtx);
    }
    EnSth_FacePlayer(self, globalCtx);
}

void EnSth_RewardUnobtainedWait(EnSth* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        EnSth_SetupAction(self, EnSth_RewardUnobtainedTalk);
    } else {
        if (self->actor.params == 0) {
            self->actor.textId = 0x28;
        } else {
            self->actor.textId = 0x21;
        }
        if (self->actor.xzDistToPlayer < 100.0f) {
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
        }
    }
    EnSth_LookAtPlayer(self, globalCtx);
}

void EnSth_ChildRewardObtainedWait(EnSth* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        EnSth_SetupAction(self, EnSth_RewardObtainedTalk);
    } else {
        if (gSaveContext.inventory.gsTokens < 50) {
            self->actor.textId = 0x20;
        } else {
            self->actor.textId = 0x1F;
        }
        if (self->actor.xzDistToPlayer < 100.0f) {
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
        }
    }
    EnSth_LookAtPlayer(self, globalCtx);
}

void EnSth_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnSth* self = THIS;

    self->actionFunc(self, globalCtx);
}

void EnSth_Update2(Actor* thisx, GlobalContext* globalCtx) {
    EnSth* self = THIS;
    s32 pad;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    if (SkelAnime_Update(&self->skelAnime)) {
        self->skelAnime.curFrame = 0.0f;
    }
    self->actionFunc(self, globalCtx);

    if (DECR(self->unk_2B6) == 0) {
        self->unk_2B6 = Rand_S16Offset(0x3C, 0x3C);
    }
    self->unk_2B4 = self->unk_2B6;
    if (self->unk_2B4 >= 3) {
        self->unk_2B4 = 0;
    }
}

s32 EnSth_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnSth* self = THIS;

    s32 temp_v1;

    if (limbIndex == 15) {
        rot->x += self->headRot.y;
        rot->z += self->headRot.x;
        *dList = D_80B0A050;
    }

    if (self->unk_2B2 & 2) {
        self->unk_2B2 &= ~2;
        return 0;
    }

    if ((limbIndex == 8) || (limbIndex == 10) || (limbIndex == 13)) {
        temp_v1 = limbIndex * 0x32;
        rot->y += (Math_SinS(globalCtx->state.frames * (temp_v1 + 0x814)) * 200.0f);
        rot->z += (Math_CosS(globalCtx->state.frames * (temp_v1 + 0x940)) * 200.0f);
    }
    return 0;
}

void EnSth_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnSth* self = THIS;

    if (limbIndex == 15) {
        Matrix_MultVec3f(&D_80B0B49C, &self->actor.focus.pos);
        if (self->actor.params != 0) {
            OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_sth.c", 2079);

            gSPDisplayList(POLY_OPA_DISP++, D_80B0A3C0);

            CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_sth.c", 2081);
        }
    }
}

Gfx* EnSth_AllocColorDList(GraphicsContext* globalCtx, u8 envR, u8 envG, u8 envB, u8 envA) {
    Gfx* dList;

    dList = Graph_Alloc(globalCtx, 2 * sizeof(Gfx));
    gDPSetEnvColor(dList, envR, envG, envB, envA);
    gSPEndDisplayList(dList + 1);

    return dList;
}

void EnSth_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnSth* self = THIS;
    Color_RGB8* envColor1;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_sth.c", 2133);

    gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[self->objectBankIdx].segment);
    func_800943C8(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08,
               EnSth_AllocColorDList(globalCtx->state.gfxCtx, sTunicColors[self->actor.params].r,
                                     sTunicColors[self->actor.params].g, sTunicColors[self->actor.params].b, 255));

    if (self->actor.params == 0) {
        gSPSegment(POLY_OPA_DISP++, 0x09, EnSth_AllocColorDList(globalCtx->state.gfxCtx, 190, 110, 0, 255));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x09, EnSth_AllocColorDList(globalCtx->state.gfxCtx, 90, 110, 130, 255));
    }
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnSth_OverrideLimbDraw, EnSth_PostLimbDraw, &self->actor);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_sth.c", 2176);
}
