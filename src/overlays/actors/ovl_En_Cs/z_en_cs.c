#include "z_en_cs.h"
#include "objects/object_cs/object_cs.h"
#include "objects/object_link_child/object_link_child.h"

#define FLAGS 0x00000009

#define THIS ((EnCs*)thisx)

void EnCs_Init(Actor* thisx, GlobalContext* globalCtx);
void EnCs_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnCs_Update(Actor* thisx, GlobalContext* globalCtx);
void EnCs_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnCs_Walk(EnCs* self, GlobalContext* globalCtx);
void EnCs_Talk(EnCs* self, GlobalContext* globalCtx);
void EnCs_Wait(EnCs* self, GlobalContext* globalCtx);
s32 EnCs_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx);
void EnCs_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx);

const ActorInit En_Cs_InitVars = {
    ACTOR_EN_CS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_CS,
    sizeof(EnCs),
    (ActorFunc)EnCs_Init,
    (ActorFunc)EnCs_Destroy,
    (ActorFunc)EnCs_Update,
    (ActorFunc)EnCs_Draw,
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

static CollisionCheckInfoInit2 sColChkInfoInit2 = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable[] = {
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

static struct_D_80AA1678 sAnimations[] = {
    { &gGraveyardKidWalkAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &gGraveyardKidSwingStickUpAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &gGraveyardKidGrabStickTwoHandsAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &gGraveyardKidIdleAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
};

void EnCs_SetAnimFromIndex(EnCs* self, s32 animIndex, s32* currentAnimIndex) {
    f32 morphFrames;

    if ((*currentAnimIndex < 0) || (animIndex == *currentAnimIndex)) {
        morphFrames = 0.0f;
    } else {
        morphFrames = sAnimations[animIndex].transitionRate;
    }

    if (sAnimations[animIndex].frameCount >= 0.0f) {
        Animation_Change(&self->skelAnime, sAnimations[animIndex].animation, sAnimations[animIndex].frameCount, 0.0f,
                         Animation_GetLastFrame(sAnimations[animIndex].animation), sAnimations[animIndex].mode,
                         morphFrames);
    } else {
        Animation_Change(&self->skelAnime, sAnimations[animIndex].animation, sAnimations[animIndex].frameCount,
                         Animation_GetLastFrame(sAnimations[animIndex].animation), 0.0f, sAnimations[animIndex].mode,
                         morphFrames);
    }

    *currentAnimIndex = animIndex;
}

void EnCs_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnCs* self = THIS;
    s32 pad;

    if (!IS_DAY) {
        Actor_Kill(&self->actor);
        return;
    }

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 19.0f);

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGraveyardKidSkel, NULL, self->jointTable, self->morphTable, 16);

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);

    CollisionCheck_SetInfo2(&self->actor.colChkInfo, sDamageTable, &sColChkInfoInit2);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    Animation_Change(&self->skelAnime, sAnimations[0].animation, 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimations[0].animation), sAnimations[0].mode,
                     sAnimations[0].transitionRate);

    self->actor.targetMode = 6;
    self->path = self->actor.params & 0xFF;
    self->unk_1EC = 0; // This variable is unused anywhere else
    self->talkState = 0;
    self->currentAnimIndex = -1;
    self->actor.gravity = -1.0f;

    EnCs_SetAnimFromIndex(self, 0, &self->currentAnimIndex);

    self->actionFunc = EnCs_Walk;
    self->walkSpeed = 1.0f;
}

void EnCs_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnCs* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 EnCs_GetTalkState(EnCs* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;
    s32 talkState = 1;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 4:
            if (func_80106BC8(globalCtx) != 0) {
                if (globalCtx->msgCtx.choiceIndex == 0) {
                    self->actor.textId = 0x2026;
                    EnCs_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
                    talkState = 2;
                } else {
                    self->actor.textId = 0x2024;
                    EnCs_SetAnimFromIndex(self, 1, &self->currentAnimIndex);
                    talkState = 2;
                }
            }
            break;
        case 6:
            if (func_80106BC8(globalCtx)) {
                if (self->actor.textId == 0x2026) {
                    Player_UnsetMask(globalCtx);
                    Item_Give(globalCtx, ITEM_SOLD_OUT);
                    gSaveContext.itemGetInf[3] |= 0x400;
                    Rupees_ChangeBy(30);
                    self->actor.textId = 0x2027;
                    talkState = 2;
                } else {
                    talkState = 0;
                }
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
            break;
    }

    return talkState;
}

s32 EnCs_GetTextID(EnCs* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 textId = Text_GetFaceReaction(globalCtx, 15);

    if (gSaveContext.itemGetInf[3] & 0x400) {
        if (textId == 0) {
            textId = 0x2028;
        }
    } else if (player->currentMask == PLAYER_MASK_SPOOKY) {
        textId = 0x2023;
    } else {
        if (textId == 0) {
            textId = 0x2022;
        }
    }

    return textId;
}

void EnCs_HandleTalking(EnCs* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 sp2A;
    s16 sp28;

    if (self->talkState == 2) {
        func_8010B720(globalCtx, self->actor.textId);
        self->talkState = 1;
    } else if (self->talkState == 1) {
        self->talkState = EnCs_GetTalkState(self, globalCtx);
    } else if (func_8002F194(&self->actor, globalCtx)) {
        if ((self->actor.textId == 0x2022) || ((self->actor.textId != 0x2022) && (self->actor.textId != 0x2028))) {
            EnCs_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
        }

        if ((self->actor.textId == 0x2023) || (self->actor.textId == 0x2028)) {
            EnCs_SetAnimFromIndex(self, 1, &self->currentAnimIndex);
        }

        if (self->actor.textId == 0x2023) {
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
        }

        self->talkState = 1;
    } else {
        func_8002F374(globalCtx, &self->actor, &sp2A, &sp28);

        if ((sp2A >= 0) && (sp2A <= 320) && (sp28 >= 0) && (sp28 <= 240) &&
            (func_8002F2CC(&self->actor, globalCtx, 100.0f))) {
            self->actor.textId = EnCs_GetTextID(self, globalCtx);
        }
    }
}

s32 EnCs_GetwaypointCount(Path* pathList, s32 pathIndex) {
    Path* path = &pathList[pathIndex];

    return path->count;
}

s32 EnCs_GetPathPoint(Path* pathList, Vec3f* dest, s32 pathIndex, s32 waypoint) {
    Path* path = pathList;
    Vec3s* pathPos;

    path += pathIndex;
    pathPos = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[waypoint];

    dest->x = pathPos->x;
    dest->y = pathPos->y;
    dest->z = pathPos->z;

    return 0;
}

s32 EnCs_HandleWalking(EnCs* self, GlobalContext* globalCtx) {
    f32 xDiff;
    f32 zDiff;
    Vec3f pathPos;
    s32 waypointCount;
    s16 walkAngle1;
    s16 walkAngle2;

    EnCs_GetPathPoint(globalCtx->setupPathList, &pathPos, self->path, self->waypoint);
    xDiff = pathPos.x - self->actor.world.pos.x;
    zDiff = pathPos.z - self->actor.world.pos.z;
    walkAngle1 = Math_FAtan2F(xDiff, zDiff) * (32768.0f / M_PI);
    self->walkAngle = walkAngle1;
    self->walkDist = sqrtf((xDiff * xDiff) + (zDiff * zDiff));

    while (self->walkDist <= 10.44f) {
        self->waypoint++;
        waypointCount = EnCs_GetwaypointCount(globalCtx->setupPathList, self->path);

        if ((self->waypoint < 0) || (!(self->waypoint < waypointCount))) {
            self->waypoint = 0;
        }

        EnCs_GetPathPoint(globalCtx->setupPathList, &pathPos, self->path, self->waypoint);
        xDiff = pathPos.x - self->actor.world.pos.x;
        zDiff = pathPos.z - self->actor.world.pos.z;
        walkAngle2 = Math_FAtan2F(xDiff, zDiff) * (32768.0f / M_PI);
        self->walkAngle = walkAngle2;
        self->walkDist = sqrtf((xDiff * xDiff) + (zDiff * zDiff));
    }

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->walkAngle, 1, 2500, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.speedXZ = self->walkSpeed;
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    return 0;
}

void EnCs_Walk(EnCs* self, GlobalContext* globalCtx) {
    s32 rnd;
    s32 animIndex;
    s32 curAnimFrame;

    if (self->talkState != 0) {
        self->actionFunc = EnCs_Talk;
        return;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        animIndex = self->currentAnimIndex;

        if (self->talkState == 0) {
            if (gSaveContext.itemGetInf[3] & 0x400) {
                rnd = Rand_ZeroOne() * 10.0f;
            } else {
                rnd = Rand_ZeroOne() * 5.0f;
            }

            if (rnd == 0) {
                if (gSaveContext.itemGetInf[3] & 0x400) {
                    animIndex = 2.0f * Rand_ZeroOne();
                    animIndex = (animIndex == 0) ? 2 : 1;
                } else {
                    animIndex = 2;
                }

                self->actionFunc = EnCs_Wait;
            } else {
                animIndex = 0;
            }
        }

        EnCs_SetAnimFromIndex(self, animIndex, &self->currentAnimIndex);
    }

    if (self->talkState == 0) {
        curAnimFrame = self->skelAnime.curFrame;

        if (((curAnimFrame >= 8) && (curAnimFrame < 16)) || ((curAnimFrame >= 23) && (curAnimFrame < 30)) ||
            (curAnimFrame == 0)) {
            self->walkSpeed = 0.0f;
        } else {
            self->walkSpeed = 1.0f;
        }

        EnCs_HandleWalking(self, globalCtx);
    }
}

void EnCs_Wait(EnCs* self, GlobalContext* globalCtx) {
    s32 animIndex;

    if (self->talkState != 0) {
        self->actionFunc = EnCs_Talk;
        return;
    }

    if (SkelAnime_Update(&self->skelAnime)) {
        animIndex = self->currentAnimIndex;

        if (self->talkState == 0) {
            if (self->animLoopCount > 0) {
                self->animLoopCount--;
                animIndex = self->currentAnimIndex;
            } else {
                animIndex = 0;
                self->actionFunc = EnCs_Walk;
            }
        }

        EnCs_SetAnimFromIndex(self, animIndex, &self->currentAnimIndex);
    }
}

void EnCs_Talk(EnCs* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (SkelAnime_Update(&self->skelAnime) != 0) {
        EnCs_SetAnimFromIndex(self, self->currentAnimIndex, &self->currentAnimIndex);
    }

    self->flag |= 1;
    self->npcInfo.unk_18.x = player->actor.focus.pos.x;
    self->npcInfo.unk_18.y = player->actor.focus.pos.y;
    self->npcInfo.unk_18.z = player->actor.focus.pos.z;
    func_80034A14(&self->actor, &self->npcInfo, 0, 4);

    if (self->talkState == 0) {
        EnCs_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
        self->actionFunc = EnCs_Walk;
        self->flag &= ~1;
    }
}

void EnCs_Update(Actor* thisx, GlobalContext* globalCtx) {
    static s32 eyeBlinkFrames[] = { 70, 1, 1 };
    EnCs* self = THIS;
    s32 pad;

    if (self->currentAnimIndex == 0) {
        if (((s32)self->skelAnime.curFrame == 9) || ((s32)self->skelAnime.curFrame == 23)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_CHIBI_WALK);
        }
    } else if (self->currentAnimIndex == 1) {
        if (((s32)self->skelAnime.curFrame == 10) || ((s32)self->skelAnime.curFrame == 25)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_CHIBI_WALK);
        }
    } else if ((self->currentAnimIndex == 2) && ((s32)self->skelAnime.curFrame == 20)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_CHIBI_WALK);
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

    self->actionFunc(self, globalCtx);

    EnCs_HandleTalking(self, globalCtx);

    self->eyeBlinkTimer--;

    if (self->eyeBlinkTimer < 0) {
        self->eyeIndex++;

        if (self->eyeIndex >= 3) {
            self->eyeIndex = 0;
        }

        self->eyeBlinkTimer = eyeBlinkFrames[self->eyeIndex];
    }
}

void EnCs_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = {
        gGraveyardKidEyesOpenTex,
        gGraveyardKidEyesHalfTex,
        gGraveyardKidEyesClosedTex,
    };
    EnCs* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_cs.c", 968);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeIndex]));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnCs_OverrideLimbDraw, EnCs_PostLimbDraw, &self->actor);

    if (gSaveContext.itemGetInf[3] & 0x400) {
        s32 childLinkObjectIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_LINK_CHILD);

        // Handle attaching the Spooky Mask to the boy's face
        if (childLinkObjectIndex >= 0) {
            Mtx* mtx;

            Matrix_Put(&self->spookyMaskMtx);
            mtx = Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_cs.c", 1000);
            gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[childLinkObjectIndex].segment);
            gSPSegment(POLY_OPA_DISP++, 0x0D, mtx - 7);
            gSPDisplayList(POLY_OPA_DISP++, gLinkChildSpookyMaskDL);
            gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->actor.objBankIndex].segment);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_cs.c", 1015);
}

s32 EnCs_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnCs* self = THIS;

    if (self->flag & 1) {
        switch (limbIndex) {
            case 8:
                rot->x += self->npcInfo.unk_0E.y;
                rot->y -= self->npcInfo.unk_0E.x;
                break;
            case 15:
                rot->x += self->npcInfo.unk_08.y;
                rot->z += self->npcInfo.unk_08.x;
                break;
        }
    }

    return 0;
}

void EnCs_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f D_809E2970 = { 500.0f, 800.0f, 0.0f };
    EnCs* self = THIS;

    if (limbIndex == 15) {
        Matrix_MultVec3f(&D_809E2970, &self->actor.focus.pos);
        Matrix_Translate(0.0f, -200.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateY(0.0f, MTXMODE_APPLY);
        Matrix_RotateX(0.0f, MTXMODE_APPLY);
        Matrix_RotateZ(1.7453293f, MTXMODE_APPLY); // close to 5 * M_PI / 9
        Matrix_Get(&self->spookyMaskMtx);
    }
}
