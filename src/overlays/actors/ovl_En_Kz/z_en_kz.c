/*
 * File: z_en_kz.c
 * Overlay: ovl_En_Kz
 * Description: King Zora
 */

#include "z_en_kz.h"
#include "objects/object_kz/object_kz.h"

#define FLAGS 0x00000009

#define THIS ((EnKz*)thisx)

void EnKz_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKz_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKz_Update(Actor* thisx, GlobalContext* globalCtx);
void EnKz_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnKz_PreMweepWait(EnKz* self, GlobalContext* globalCtx);
void EnKz_SetupMweep(EnKz* self, GlobalContext* globalCtx);
void EnKz_Mweep(EnKz* self, GlobalContext* globalCtx);
void EnKz_StopMweep(EnKz* self, GlobalContext* globalCtx);
void EnKz_Wait(EnKz* self, GlobalContext* globalCtx);
void EnKz_SetupGetItem(EnKz* self, GlobalContext* globalCtx);
void EnKz_StartTimer(EnKz* self, GlobalContext* globalCtx);

const ActorInit En_Kz_InitVars = {
    ACTOR_EN_KZ,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_KZ,
    sizeof(EnKz),
    (ActorFunc)EnKz_Init,
    (ActorFunc)EnKz_Destroy,
    (ActorFunc)EnKz_Update,
    (ActorFunc)EnKz_Draw,
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
    { 80, 120, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static struct_80034EC0_Entry sAnimations[] = {
    { &object_kz_Anim_00075C, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    { &object_kz_Anim_00075C, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -10.0f },
    { &object_kz_Anim_00046C, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -10.0f },
};

u16 EnKz_GetTextNoMaskChild(GlobalContext* globalCtx, EnKz* self) {
    Player* player = GET_PLAYER(globalCtx);

    if (CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
        return 0x402B;
    } else if (gSaveContext.eventChkInf[3] & 8) {
        return 0x401C;
    } else {
        player->exchangeItemId = EXCH_ITEM_LETTER_RUTO;
        return 0x401A;
    }
}

u16 EnKz_GetTextNoMaskAdult(GlobalContext* globalCtx, EnKz* self) {
    Player* player = GET_PLAYER(globalCtx);

    if (INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_FROG) {
        if (!(gSaveContext.infTable[19] & 0x200)) {
            if (CHECK_OWNED_EQUIP(EQUIP_TUNIC, 2)) {
                return 0x401F;
            } else {
                return 0x4012;
            }
        } else {
            return CHECK_QUEST_ITEM(QUEST_SONG_SERENADE) ? 0x4045 : 0x401A;
        }
    } else {
        player->exchangeItemId = EXCH_ITEM_PRESCRIPTION;
        return 0x4012;
    }
}

u16 EnKz_GetText(GlobalContext* globalCtx, Actor* thisx) {
    EnKz* self = THIS;
    u16 reactionText = Text_GetFaceReaction(globalCtx, 0x1E);

    if (reactionText != 0) {
        return reactionText;
    }

    if (LINK_IS_ADULT) {
        return EnKz_GetTextNoMaskAdult(globalCtx, self);
    } else {
        return EnKz_GetTextNoMaskChild(globalCtx, self);
    }
}

s16 func_80A9C6C0(GlobalContext* globalCtx, Actor* thisx) {
    EnKz* self = THIS;
    s16 ret = 1;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 6:
            ret = 0;
            switch (self->actor.textId) {
                case 0x4012:
                    gSaveContext.infTable[19] |= 0x200;
                    ret = 2;
                    break;
                case 0x401B:
                    ret = func_80106BC8(globalCtx) == 0 ? 1 : 2;
                    break;
                case 0x401F:
                    gSaveContext.infTable[19] |= 0x200;
                    break;
            }
            break;
        case 3:
            if (self->actor.textId != 0x4014) {
                if (self->actor.textId == 0x401B && !self->sfxPlayed) {
                    Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0,
                                           &D_801333E8);
                    self->sfxPlayed = true;
                }
            } else if (!self->sfxPlayed) {
                Audio_PlaySoundGeneral(NA_SE_SY_TRE_BOX_APPEAR, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                self->sfxPlayed = true;
            }
            break;
        case 4:
            if (func_80106BC8(globalCtx) == 0) {
                break;
            }
            if (self->actor.textId == 0x4014) {
                if (globalCtx->msgCtx.choiceIndex == 0) {
                    EnKz_SetupGetItem(self, globalCtx);
                    ret = 2;
                } else {
                    self->actor.textId = 0x4016;
                    func_8010B720(globalCtx, self->actor.textId);
                }
            }
            break;
        case 5:
            if (func_80106BC8(globalCtx) != 0) {
                ret = 2;
            }
            break;
        case 0:
        case 1:
        case 2:
        case 7:
        case 8:
        case 9:
            break;
    }
    return ret;
}

void EnKz_UpdateEyes(EnKz* self) {
    if (DECR(self->blinkTimer) == 0) {
        self->eyeIdx += 1;
        if (self->eyeIdx >= 3) {
            self->blinkTimer = Rand_S16Offset(30, 30);
            self->eyeIdx = 0;
        }
    }
}

s32 func_80A9C95C(GlobalContext* globalCtx, EnKz* self, s16* arg2, f32 unkf, callback1_800343CC callback1,
                  callback2_800343CC callback2) {
    Player* player = GET_PLAYER(globalCtx);
    s16 sp32;
    s16 sp30;
    f32 xzDistToPlayer;
    f32 yaw;

    if (func_8002F194(&self->actor, globalCtx) != 0) {
        *arg2 = 1;
        return 1;
    }

    if (*arg2 != 0) {
        *arg2 = callback2(globalCtx, &self->actor);
        return 0;
    }

    yaw = Math_Vec3f_Yaw(&self->actor.home.pos, &player->actor.world.pos);
    yaw -= self->actor.shape.rot.y;
    if ((fabsf(yaw) > 1638.0f) || (self->actor.xzDistToPlayer < 265.0f)) {
        self->actor.flags &= ~1;
        return 0;
    }

    self->actor.flags |= 1;

    func_8002F374(globalCtx, &self->actor, &sp32, &sp30);
    if (!((sp32 >= -30) && (sp32 < 361) && (sp30 >= -10) && (sp30 < 241))) {
        return 0;
    }

    xzDistToPlayer = self->actor.xzDistToPlayer;
    self->actor.xzDistToPlayer = Math_Vec3f_DistXZ(&self->actor.home.pos, &player->actor.world.pos);
    if (func_8002F2CC(&self->actor, globalCtx, unkf) == 0) {
        self->actor.xzDistToPlayer = xzDistToPlayer;
        return 0;
    }
    self->actor.xzDistToPlayer = xzDistToPlayer;
    self->actor.textId = callback1(globalCtx, &self->actor);

    return 0;
}

void func_80A9CB18(EnKz* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (func_80A9C95C(globalCtx, self, &self->unk_1E0.unk_00, 340.0f, EnKz_GetText, func_80A9C6C0)) {
        if ((self->actor.textId == 0x401A) && !(gSaveContext.eventChkInf[3] & 8)) {
            if (func_8002F368(globalCtx) == EXCH_ITEM_LETTER_RUTO) {
                self->actor.textId = 0x401B;
                self->sfxPlayed = false;
            } else {
                self->actor.textId = 0x401A;
            }
            player->actor.textId = self->actor.textId;
            return;
        }

        if (LINK_IS_ADULT) {
            if ((INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_PRESCRIPTION) &&
                (func_8002F368(globalCtx) == EXCH_ITEM_PRESCRIPTION)) {
                self->actor.textId = 0x4014;
                self->sfxPlayed = false;
                player->actor.textId = self->actor.textId;
                self->isTrading = true;
                return;
            }

            self->isTrading = false;
            if (gSaveContext.infTable[19] & 0x200) {
                self->actor.textId = CHECK_QUEST_ITEM(QUEST_SONG_SERENADE) ? 0x4045 : 0x401A;
                player->actor.textId = self->actor.textId;
            } else {
                self->actor.textId = CHECK_OWNED_EQUIP(EQUIP_TUNIC, 2) ? 0x401F : 0x4012;
                player->actor.textId = self->actor.textId;
            }
        }
    }
}

s32 EnKz_FollowPath(EnKz* self, GlobalContext* globalCtx) {
    Path* path;
    Vec3s* pointPos;
    f32 pathDiffX;
    f32 pathDiffZ;

    if ((self->actor.params & 0xFF00) == 0xFF00) {
        return 0;
    }

    path = &globalCtx->setupPathList[(self->actor.params & 0xFF00) >> 8];
    pointPos = SEGMENTED_TO_VIRTUAL(path->points);
    pointPos += self->waypoint;

    pathDiffX = pointPos->x - self->actor.world.pos.x;
    pathDiffZ = pointPos->z - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.world.rot.y, (Math_FAtan2F(pathDiffX, pathDiffZ) * (0x8000 / M_PI)), 0xA, 0x3E8, 1);

    if ((SQ(pathDiffX) + SQ(pathDiffZ)) < 10.0f) {
        self->waypoint++;
        if (self->waypoint >= path->count) {
            self->waypoint = 0;
        }
        return 1;
    }
    return 0;
}

s32 EnKz_SetMovedPos(EnKz* self, GlobalContext* globalCtx) {
    Path* path;
    Vec3s* lastPointPos;

    if ((self->actor.params & 0xFF00) == 0xFF00) {
        return 0;
    }

    path = &globalCtx->setupPathList[(self->actor.params & 0xFF00) >> 8];
    lastPointPos = SEGMENTED_TO_VIRTUAL(path->points);
    lastPointPos += path->count - 1;

    self->actor.world.pos.x = lastPointPos->x;
    self->actor.world.pos.y = lastPointPos->y;
    self->actor.world.pos.z = lastPointPos->z;

    return 1;
}

void EnKz_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnKz* self = THIS;
    s32 pad;

    SkelAnime_InitFlex(globalCtx, &self->skelanime, &object_kz_Skel_0086D0, NULL, self->jointTable, self->morphTable,
                       12);
    ActorShape_Init(&self->actor.shape, 0.0, NULL, 0.0);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    Actor_SetScale(&self->actor, 0.01);
    self->actor.targetMode = 3;
    self->unk_1E0.unk_00 = 0;
    func_80034EC0(&self->skelanime, sAnimations, 0);

    if (gSaveContext.eventChkInf[3] & 8) {
        EnKz_SetMovedPos(self, globalCtx);
    }

    if (LINK_IS_ADULT) {
        if (!(gSaveContext.infTable[19] & 0x100)) {
            Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_BG_ICE_SHELTER,
                               self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0,
                               0x04FF);
        }
        self->actionFunc = EnKz_Wait;
    } else {
        self->actionFunc = EnKz_PreMweepWait;
    }
}

void EnKz_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnKz* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnKz_PreMweepWait(EnKz* self, GlobalContext* globalCtx) {
    if (self->unk_1E0.unk_00 == 2) {
        func_80034EC0(&self->skelanime, sAnimations, 2);
        self->unk_1E0.unk_00 = 0;
        self->actionFunc = EnKz_SetupMweep;
    } else {
        func_80034F54(globalCtx, self->unk_2A6, self->unk_2BE, 12);
    }
}

void EnKz_SetupMweep(EnKz* self, GlobalContext* globalCtx) {
    Vec3f unused = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;
    Vec3f initPos;

    self->cutsceneCamera = Gameplay_CreateSubCamera(globalCtx);
    self->gameplayCamera = globalCtx->activeCamera;
    Gameplay_ChangeCameraStatus(globalCtx, self->gameplayCamera, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, self->cutsceneCamera, CAM_STAT_ACTIVE);
    pos = self->actor.world.pos;
    initPos = self->actor.home.pos;
    pos.y += 60.0f;
    initPos.y += -100.0f;
    initPos.z += 260.0f;
    Gameplay_CameraSetAtEye(globalCtx, self->cutsceneCamera, &pos, &initPos);
    func_8002DF54(globalCtx, &self->actor, 8);
    self->actor.speedXZ = 0.1f;
    self->actionFunc = EnKz_Mweep;
}

void EnKz_Mweep(EnKz* self, GlobalContext* globalCtx) {
    Vec3f unused = { 0.0f, 0.0f, 0.0f };
    Vec3f pos;
    Vec3f initPos;

    pos = self->actor.world.pos;
    initPos = self->actor.home.pos;
    pos.y += 60.0f;
    initPos.y += -100.0f;
    initPos.z += 260.0f;
    Gameplay_CameraSetAtEye(globalCtx, self->cutsceneCamera, &pos, &initPos);
    if ((EnKz_FollowPath(self, globalCtx) == 1) && (self->waypoint == 0)) {
        func_80034EC0(&self->skelanime, sAnimations, 1);
        Inventory_ReplaceItem(globalCtx, ITEM_LETTER_RUTO, ITEM_BOTTLE);
        EnKz_SetMovedPos(self, globalCtx);
        gSaveContext.eventChkInf[3] |= 8;
        self->actor.speedXZ = 0.0;
        self->actionFunc = EnKz_StopMweep;
    }
    if (self->skelanime.curFrame == 13.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_VO_KZ_MOVE);
    }
}

void EnKz_StopMweep(EnKz* self, GlobalContext* globalCtx) {
    Gameplay_ChangeCameraStatus(globalCtx, self->gameplayCamera, CAM_STAT_ACTIVE);
    Gameplay_ClearCamera(globalCtx, self->cutsceneCamera);
    func_8002DF54(globalCtx, &self->actor, 7);
    self->actionFunc = EnKz_Wait;
}

void EnKz_Wait(EnKz* self, GlobalContext* globalCtx) {
    if (self->unk_1E0.unk_00 == 2) {
        self->actionFunc = EnKz_SetupGetItem;
        EnKz_SetupGetItem(self, globalCtx);
    } else {
        func_80034F54(globalCtx, self->unk_2A6, self->unk_2BE, 12);
    }
}

void EnKz_SetupGetItem(EnKz* self, GlobalContext* globalCtx) {
    s32 getItemId;
    f32 xzRange;
    f32 yRange;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->unk_1E0.unk_00 = 1;
        self->actionFunc = EnKz_StartTimer;
    } else {
        getItemId = self->isTrading == true ? GI_FROG : GI_TUNIC_ZORA;
        yRange = fabsf(self->actor.yDistToPlayer) + 1.0f;
        xzRange = self->actor.xzDistToPlayer + 1.0f;
        func_8002F434(&self->actor, globalCtx, getItemId, xzRange, yRange);
    }
}

void EnKz_StartTimer(EnKz* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx))) {
        if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_FROG) {
            func_80088AA0(180); // start timer2 with 3 minutes
            gSaveContext.eventInf[1] &= ~1;
        }
        self->unk_1E0.unk_00 = 0;
        self->actionFunc = EnKz_Wait;
    }
}

void EnKz_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnKz* self = THIS;
    s32 pad;

    if (LINK_IS_ADULT && !(gSaveContext.infTable[19] & 0x100)) {
        gSaveContext.infTable[19] |= 0x100;
    }
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    SkelAnime_Update(&self->skelanime);
    EnKz_UpdateEyes(self);
    Actor_MoveForward(&self->actor);
    if (self->actionFunc != EnKz_StartTimer) {
        func_80A9CB18(self, globalCtx);
    }
    self->actionFunc(self, globalCtx);
}

s32 EnKz_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnKz* self = THIS;

    if (limbIndex == 8 || limbIndex == 9 || limbIndex == 10) {
        rot->y += Math_SinS(self->unk_2A6[limbIndex]) * 200.0f;
        rot->z += Math_CosS(self->unk_2BE[limbIndex]) * 200.0f;
    }
    if (limbIndex) {}
    return false;
}

void EnKz_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnKz* self = THIS;
    Vec3f mult = { 2600.0f, 0.0f, 0.0f };

    if (limbIndex == 11) {
        Matrix_MultVec3f(&mult, &self->actor.focus.pos);
    }
}

void EnKz_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* sEyeSegments[] = {
        object_kz_Tex_001470,
        object_kz_Tex_001870,
        object_kz_Tex_001C70,
    };
    EnKz* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_kz.c", 1259);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeSegments[self->eyeIdx]));
    func_800943C8(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelanime.skeleton, self->skelanime.jointTable, self->skelanime.dListCount,
                          EnKz_OverrideLimbDraw, EnKz_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_kz.c", 1281);
}
