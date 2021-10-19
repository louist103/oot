/*
 * File: z_en_mm.c
 * Overlay: ovl_En_Mm
 * Description: Running Man (child)
 */

#include "z_en_mm.h"
#include "objects/object_mm/object_mm.h"
#include "objects/object_link_child/object_link_child.h"

#define FLAGS 0x00000019

#define THIS ((EnMm*)thisx)

typedef enum {
    /* 0 */ RM_ANIM_RUN,
    /* 1 */ RM_ANIM_SIT,
    /* 2 */ RM_ANIM_SIT_WAIT,
    /* 3 */ RM_ANIM_STAND,
    /* 4 */ RM_ANIM_SPRINT,
    /* 5 */ RM_ANIM_EXCITED, // plays when talking to him with bunny hood on
    /* 6 */ RM_ANIM_HAPPY    // plays when you sell him the bunny hood
} RunningManAnimIndex;

typedef enum {
    /* 0 */ RM_MOUTH_CLOSED,
    /* 1 */ RM_MOUTH_OPEN
} RunningManMouthTex;

void EnMm_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMm_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMm_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AAE598(EnMm* self, GlobalContext* globalCtx);
void func_80AAE294(EnMm* self, GlobalContext* globalCtx);
void func_80AAE50C(EnMm* self, GlobalContext* globalCtx);
void func_80AAE224(EnMm* self, GlobalContext* globalCtx);
s32 func_80AADA70(void);

s32 EnMm_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx);
void EnMm_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void*);

const ActorInit En_Mm_InitVars = {
    ACTOR_EN_MM,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MM,
    sizeof(EnMm),
    (ActorFunc)EnMm_Init,
    (ActorFunc)EnMm_Destroy,
    (ActorFunc)EnMm_Update,
    (ActorFunc)EnMm_Draw,
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
    { 18, 63, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit[] = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(0, 0x0),
    /* Master sword  */ DMG_ENTRY(0, 0x0),
    /* Giant's Knife */ DMG_ENTRY(0, 0x0),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0x0),
    /* Giant spin    */ DMG_ENTRY(0, 0x0),
    /* Master spin   */ DMG_ENTRY(0, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(0, 0x0),
    /* Giant jump    */ DMG_ENTRY(0, 0x0),
    /* Master jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

typedef struct {
    /* 0x00 */ AnimationHeader* animation;
    /* 0x04 */ f32 playSpeed;
    /* 0x08 */ u8 mode;
    /* 0x0C */ f32 morphFrames;
} EnMmAnimEntry; // size = 0x10

static EnMmAnimEntry sAnimationEntries[] = {
    { &gRunningManRunAnim, 1.0f, ANIMMODE_LOOP, -7.0f },     { &gRunningManSitStandAnim, -1.0f, ANIMMODE_ONCE, -7.0f },
    { &gRunningManSitWaitAnim, 1.0f, ANIMMODE_LOOP, -7.0f }, { &gRunningManSitStandAnim, 1.0f, ANIMMODE_ONCE, -7.0f },
    { &gRunningManSprintAnim, 1.0f, ANIMMODE_LOOP, -7.0f },  { &gRunningManExcitedAnim, 1.0f, ANIMMODE_LOOP, -12.0f },
    { &gRunningManHappyAnim, 1.0f, ANIMMODE_LOOP, -12.0f },
};

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} EnMmPathInfo;

static EnMmPathInfo sPathInfo[] = {
    { 0, 1, 0, 0 },
    { 1, 1, 0, 1 },
    { 1, 3, 2, 1 },
    { -1, 0, 2, 0 },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_STOP),
};

void EnMm_ChangeAnimation(EnMm* self, s32 newAnimIndex, s32* curAnimIndex) {
    f32 morphFrames;

    if ((*curAnimIndex < 0) || (newAnimIndex == *curAnimIndex)) {
        morphFrames = 0.0f;
    } else {
        morphFrames = sAnimationEntries[newAnimIndex].morphFrames;
    }

    if (sAnimationEntries[newAnimIndex].playSpeed >= 0.0f) {
        Animation_Change(&self->skelAnime, sAnimationEntries[newAnimIndex].animation,
                         sAnimationEntries[newAnimIndex].playSpeed, 0.0f,
                         Animation_GetLastFrame(sAnimationEntries[newAnimIndex].animation),
                         sAnimationEntries[newAnimIndex].mode, morphFrames);
    } else {
        Animation_Change(&self->skelAnime, sAnimationEntries[newAnimIndex].animation,
                         sAnimationEntries[newAnimIndex].playSpeed,
                         Animation_GetLastFrame(sAnimationEntries[newAnimIndex].animation), 0.0f,
                         sAnimationEntries[newAnimIndex].mode, morphFrames);
    }

    *curAnimIndex = newAnimIndex;
}

void EnMm_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnMm* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 21.0f);

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gRunningManSkel, NULL, self->jointTable, self->morphTable, 16);

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &sDamageTable, sColChkInfoInit);

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    Animation_Change(&self->skelAnime, sAnimationEntries[RM_ANIM_RUN].animation, 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationEntries[RM_ANIM_RUN].animation),
                     sAnimationEntries[RM_ANIM_RUN].mode, sAnimationEntries[RM_ANIM_RUN].morphFrames);

    self->path = self->actor.params & 0xFF;
    self->unk_1F0 = 2;
    self->unk_1E8 = 0;
    self->actor.targetMode = 2;
    self->actor.gravity = -1.0f;
    self->speedXZ = 3.0f;
    self->unk_204 = self->actor.objBankIndex;

    if (func_80AADA70() == 1) {
        self->mouthTexIndex = RM_MOUTH_OPEN;
        EnMm_ChangeAnimation(self, RM_ANIM_RUN, &self->curAnimIndex);
        self->actionFunc = func_80AAE598;
    } else {
        self->mouthTexIndex = RM_MOUTH_CLOSED;
        EnMm_ChangeAnimation(self, RM_ANIM_SIT_WAIT, &self->curAnimIndex);
        self->actionFunc = func_80AAE294;
    }
}

void EnMm_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnMm* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 func_80AADA70(void) {
    s32 isDay = false;

    if ((gSaveContext.dayTime > 0x3555) && (gSaveContext.dayTime <= 0xD556)) {
        isDay = true;
    }

    return isDay;
}

s32 func_80AADAA0(EnMm* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 sp1C = 1;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            if (func_80106BC8(globalCtx)) {
                if (globalCtx->msgCtx.choiceIndex == 0) {
                    player->actor.textId = 0x202D;
                    self->unk_254 &= ~1;
                    EnMm_ChangeAnimation(self, RM_ANIM_HAPPY, &self->curAnimIndex);
                } else {
                    player->actor.textId = 0x202C;
                    gSaveContext.infTable[23] |= 0x1000;
                }
                sp1C = 2;
            }
            break;
        case 5:
            if (func_80106BC8(globalCtx)) {
                Player_UnsetMask(globalCtx);
                Item_Give(globalCtx, ITEM_SOLD_OUT);
                gSaveContext.itemGetInf[3] |= 0x800;
                Rupees_ChangeBy(500);
                player->actor.textId = 0x202E;
                sp1C = 2;
            }
            break;
        case 6:
            if (func_80106BC8(globalCtx)) {
                if ((player->actor.textId == 0x202E) || (player->actor.textId == 0x202C)) {
                    self->unk_254 |= 1;
                    EnMm_ChangeAnimation(self, RM_ANIM_SIT_WAIT, &self->curAnimIndex);
                }
                sp1C = 0;
            }
            break;
    }

    return sp1C;
}

s32 EnMm_GetTextId(EnMm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 textId;

    textId = Text_GetFaceReaction(globalCtx, 0x1C);

    if (gSaveContext.itemGetInf[3] & 0x800) {
        if (textId == 0) {
            textId = 0x204D;
        }
    } else if (player->currentMask == PLAYER_MASK_BUNNY) {
        textId = (gSaveContext.infTable[23] & 0x1000) ? 0x202B : 0x202A;
    } else if (textId == 0) {
        textId = 0x2029;
    }

    return textId;
}

void func_80AADCD0(EnMm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 yawDiff;
    s16 sp26;
    s16 sp24;

    if (self->unk_1E0 == 2) {
        func_8010B720(globalCtx, player->actor.textId);
        self->unk_1E0 = 1;
    } else if (self->unk_1E0 == 1) {
        self->unk_1E0 = func_80AADAA0(self, globalCtx);
    } else {
        if (func_8002F194(&self->actor, globalCtx)) {
            self->unk_1E0 = 1;

            if (self->curAnimIndex != 5) {
                if ((self->actor.textId == 0x202A) || (self->actor.textId == 0x202B)) {
                    EnMm_ChangeAnimation(self, RM_ANIM_EXCITED, &self->curAnimIndex);
                    func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
                }
            }
        } else {
            func_8002F374(globalCtx, &self->actor, &sp26, &sp24);
            yawDiff = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y));

            if ((sp26 >= 0) && (sp26 <= 0x140) && (sp24 >= 0) && (sp24 <= 0xF0) && (yawDiff <= 17152.0f) &&
                (self->unk_1E0 != 3) && func_8002F2CC(&self->actor, globalCtx, 100.0f)) {
                self->actor.textId = EnMm_GetTextId(self, globalCtx);
            }
        }
    }
}

s32 EnMm_GetPointCount(Path* pathList, s32 pathNum) {
    return (pathList + pathNum)->count;
}

s32 func_80AADE60(Path* pathList, Vec3f* pos, s32 pathNum, s32 waypoint) {
    Vec3s* pointPos;
    pointPos = &((Vec3s*)SEGMENTED_TO_VIRTUAL((pathList + pathNum)->points))[waypoint];

    pos->x = pointPos->x;
    pos->y = pointPos->y;
    pos->z = pointPos->z;

    return 0;
}

s32 func_80AADEF0(EnMm* self, GlobalContext* globalCtx) {
    f32 xDiff;
    f32 zDiff;
    Vec3f waypointPos;
    s32 phi_a2;
    s32 phi_v1;

    func_80AADE60(globalCtx->setupPathList, &waypointPos, self->path, self->waypoint);

    xDiff = waypointPos.x - self->actor.world.pos.x;
    zDiff = waypointPos.z - self->actor.world.pos.z;

    self->yawToWaypoint = (s32)(Math_FAtan2F(xDiff, zDiff) * 10430.378f);
    self->distToWaypoint = sqrtf(SQ(xDiff) + SQ(zDiff));

    while ((self->distToWaypoint <= 10.44f) && (self->unk_1E8 != 0)) {
        self->waypoint += sPathInfo[self->unk_1E8].unk_00;

        phi_a2 = sPathInfo[self->unk_1E8].unk_08;

        switch (phi_a2) {
            case 0:
                phi_a2 = 0;
                break;
            case 1:
                phi_a2 = EnMm_GetPointCount(globalCtx->setupPathList, self->path) - 1;
                break;
            case 2:
                phi_a2 = self->unk_1F0;
                break;
        }

        phi_v1 = sPathInfo[self->unk_1E8].unk_0C;

        switch (phi_v1) {
            case 0:
                phi_v1 = 0;
                break;
            case 1:
                phi_v1 = EnMm_GetPointCount(globalCtx->setupPathList, self->path) - 1;
                break;
            case 2:
                phi_v1 = self->unk_1F0;
                break;
        }

        if ((sPathInfo[self->unk_1E8].unk_00 >= 0 && (self->waypoint < phi_a2 || phi_v1 < self->waypoint)) ||
            (sPathInfo[self->unk_1E8].unk_00 < 0 && (phi_a2 < self->waypoint || self->waypoint < phi_v1))) {
            self->unk_1E8 = sPathInfo[self->unk_1E8].unk_04;
            self->waypoint = sPathInfo[self->unk_1E8].unk_08;
        }

        func_80AADE60(globalCtx->setupPathList, &waypointPos, self->path, self->waypoint);

        xDiff = waypointPos.x - self->actor.world.pos.x;
        zDiff = waypointPos.z - self->actor.world.pos.z;

        self->yawToWaypoint = (s32)(Math_FAtan2F(xDiff, zDiff) * 10430.378f);
        self->distToWaypoint = sqrtf(SQ(xDiff) + SQ(zDiff));
    }

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->yawToWaypoint, 1, 2500, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    Math_SmoothStepToF(&self->actor.speedXZ, self->speedXZ, 0.6f, self->distToWaypoint, 0.0f);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    return 0;
}

void func_80AAE224(EnMm* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->actionFunc = func_80AAE598;
        self->unk_1E8 = 0;
        self->mouthTexIndex = RM_MOUTH_CLOSED;
        self->unk_254 |= 1;
        self->unk_1E0 = 0;
        self->actor.speedXZ = 0.0f;
        EnMm_ChangeAnimation(self, RM_ANIM_SIT_WAIT, &self->curAnimIndex);
    }
}

void func_80AAE294(EnMm* self, GlobalContext* globalCtx) {
    f32 floorYNorm;
    Vec3f dustPos;

    if (!Player_InCsMode(globalCtx)) {
        SkelAnime_Update(&self->skelAnime);

        if (self->curAnimIndex == 0) {
            if (((s32)self->skelAnime.curFrame == 1) || ((s32)self->skelAnime.curFrame == 6)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_PL_WALK_GROUND);
            }
        }

        if (self->curAnimIndex == 4) {
            if (((self->skelAnime.curFrame - self->skelAnime.playSpeed < 9.0f) && (self->skelAnime.curFrame >= 9.0f)) ||
                ((self->skelAnime.curFrame - self->skelAnime.playSpeed < 19.0f) &&
                 (self->skelAnime.curFrame >= 19.0f))) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_WALK);
            }
        }

        if (gSaveContext.itemGetInf[3] & 0x800) {
            self->speedXZ = 10.0f;
            self->skelAnime.playSpeed = 2.0f;
        } else {
            self->speedXZ = 3.0f;
            self->skelAnime.playSpeed = 1.0f;
        }

        func_80AADEF0(self, globalCtx);

        if (func_80AADA70() == 0) {
            if (self->actor.floorPoly != NULL) {
                floorYNorm = self->actor.floorPoly->normal.y * 0.00003051851f;

                if ((floorYNorm > 0.9848f) || (floorYNorm < -0.9848f)) {
                    if (self->sitTimer > 30) {
                        EnMm_ChangeAnimation(self, RM_ANIM_SIT, &self->curAnimIndex);
                        self->actionFunc = func_80AAE224;
                    } else {
                        self->sitTimer++;
                    }
                } else {
                    self->sitTimer = 0;
                }
            }
        }

        if (gSaveContext.itemGetInf[3] & 0x800) {
            dustPos.x = self->actor.world.pos.x;
            dustPos.y = self->actor.world.pos.y;
            dustPos.z = self->actor.world.pos.z;

            if (gSaveContext.gameMode != 3) {
                func_80033480(globalCtx, &dustPos, 50.0f, 2, 350, 20, 0);
            }

            if (self->collider.base.ocFlags2 & OC2_HIT_PLAYER) {
                func_8002F71C(globalCtx, &self->actor, 3.0f, self->actor.yawTowardsPlayer, 4.0f);
            }
        }
    }
}

void func_80AAE50C(EnMm* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->sitTimer = 0;
        self->actionFunc = func_80AAE294;

        if (gSaveContext.itemGetInf[3] & 0x800) {
            EnMm_ChangeAnimation(self, RM_ANIM_SPRINT, &self->curAnimIndex);
            self->mouthTexIndex = RM_MOUTH_CLOSED;
        } else {
            EnMm_ChangeAnimation(self, RM_ANIM_RUN, &self->curAnimIndex);
            self->mouthTexIndex = RM_MOUTH_OPEN;
        }

        self->unk_1E8 = 1;
    }
}

void func_80AAE598(EnMm* self, GlobalContext* globalCtx) {
    func_80038290(globalCtx, &self->actor, &self->unk_248, &self->unk_24E, self->actor.focus.pos);
    SkelAnime_Update(&self->skelAnime);

    if ((func_80AADA70() != 0) && (self->unk_1E0 == 0)) {
        self->unk_1E0 = 3;
        self->actionFunc = func_80AAE50C;
        self->unk_254 &= ~1;
        EnMm_ChangeAnimation(self, RM_ANIM_STAND, &self->curAnimIndex);
    }
}

void EnMm_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnMm* self = THIS;

    self->actionFunc(self, globalCtx);
    func_80AADCD0(self, globalCtx);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnMm_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* mouthTextures[] = { gRunningManMouthOpenTex, gRunningManMouthClosedTex };
    s32 pad;
    EnMm* self = THIS;

    if (0) {}

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_mm.c", 1065);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(mouthTextures[self->mouthTexIndex]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnMm_OverrideLimbDraw, EnMm_PostLimbDraw, self);

    if (gSaveContext.itemGetInf[3] & 0x800) {
        s32 linkChildObjBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_LINK_CHILD);

        if (linkChildObjBankIndex >= 0) {
            Mtx* mtx;
            Vec3s sp50;
            Mtx* mtx2;

            mtx = Graph_Alloc(globalCtx->state.gfxCtx, sizeof(Mtx) * 2);

            Matrix_Put(&self->unk_208);
            mtx2 = Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_mm.c", 1111);

            gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[linkChildObjBankIndex].segment);
            gSPSegment(POLY_OPA_DISP++, 0x0B, mtx);
            gSPSegment(POLY_OPA_DISP++, 0x0D, mtx2 - 7);

            sp50.x = 994;
            sp50.y = 3518;
            sp50.z = -13450;

            func_800D1694(97.0f, -1203.0f, -240.0f, &sp50);
            Matrix_ToMtx(mtx++, "../z_en_mm.c", 1124);

            sp50.x = -994;
            sp50.y = -3518;
            sp50.z = -13450;

            func_800D1694(97.0f, -1203.0f, 240.0f, &sp50);
            Matrix_ToMtx(mtx, "../z_en_mm.c", 1131);

            gSPDisplayList(POLY_OPA_DISP++, gLinkChildBunnyHoodDL);
            gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->actor.objBankIndex].segment);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_mm.c", 1141);
}

s32 EnMm_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnMm* self = THIS;

    if (self->unk_254 & 1) {
        switch (limbIndex) {
            case 8:
                rot->x += self->unk_24E.y;
                rot->y -= self->unk_24E.x;
                break;
            case 15:
                rot->x += self->unk_248.y;
                rot->z += (self->unk_248.x + 0xFA0);
                break;
            default:
                break;
        }
    }

    return 0;
}

void EnMm_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f headOffset = { 200.0f, 800.0f, 0.0f };
    EnMm* self = THIS;

    if (limbIndex == 15) {
        Matrix_MultVec3f(&headOffset, &self->actor.focus.pos);
        Matrix_Translate(260.0f, 20.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateY(0.0f, MTXMODE_APPLY);
        Matrix_RotateX(0.0f, MTXMODE_APPLY);
        Matrix_RotateZ(2.5132742f, MTXMODE_APPLY);
        Matrix_Translate(-260.0f, 58.0f, 10.0f, MTXMODE_APPLY);
        Matrix_Get(&self->unk_208);
    }
}
