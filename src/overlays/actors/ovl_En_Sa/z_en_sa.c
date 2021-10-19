#include "z_en_sa.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "objects/object_sa/object_sa.h"
#include "scenes/overworld/spot04/spot04_scene.h"
#include "scenes/overworld/spot05/spot05_scene.h"

#define FLAGS 0x02000019

#define THIS ((EnSa*)thisx)

void EnSa_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSa_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSa_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSa_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AF6448(EnSa* self, GlobalContext* globalCtx);
void func_80AF67D0(EnSa* self, GlobalContext* globalCtx);
void func_80AF683C(EnSa* self, GlobalContext* globalCtx);
void func_80AF68E4(EnSa* self, GlobalContext* globalCtx);
void func_80AF6B20(EnSa* self, GlobalContext* globalCtx);

typedef enum {
    /* 0 */ SARIA_EYE_OPEN,
    /* 1 */ SARIA_EYE_HALF,
    /* 2 */ SARIA_EYE_CLOSED,
    /* 3 */ SARIA_EYE_SUPRISED,
    /* 4 */ SARIA_EYE_SAD
} SariaEyeState;

typedef enum {
    /* 0 */ SARIA_MOUTH_CLOSED2,
    /* 1 */ SARIA_MOUTH_SUPRISED,
    /* 2 */ SARIA_MOUTH_CLOSED,
    /* 3 */ SARIA_MOUTH_SMILING_OPEN,
    /* 4 */ SARIA_MOUTH_FROWNING
} SariaMouthState;

const ActorInit En_Sa_InitVars = {
    ACTOR_EN_SA,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SA,
    sizeof(EnSa),
    (ActorFunc)EnSa_Init,
    (ActorFunc)EnSa_Destroy,
    (ActorFunc)EnSa_Update,
    (ActorFunc)EnSa_Draw,
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
    { 20, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = {
    0, 0, 0, 0, MASS_IMMOVABLE,
};

static struct_D_80AA1678 sAnimationInfo[] = {
    { &gSariaWaitArmsToSideAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &gSariaLookUpArmExtendedAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &gSariaWaveAnim, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &gSariaRunAnim, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &gSariaWaitArmsToSideAnim, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &gSariaLookOverShoulderAnim, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &gSariaPlayingOcarinaAnim, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &gSariaStopPlayingOcarinaAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &gSariaOcarinaToMouthAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &gSariaLinkLearnedSariasSongAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &gSariaReturnToOcarinaAnim, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &gSariaPlayingOcarinaAnim, 1.0f, ANIMMODE_LOOP, 0.0f },
};

static struct_80034EC0_Entry sAnimations[] = {
    { &gSariaTransitionHandsSideToChestToSideAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gSariaTransitionHandsSideToBackAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -4.0f },
    { &gSariaRightArmExtendedWaitAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gSariaHandsOutAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gSariaStandHandsOnHipsAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gSariaExtendRightArmAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gSariaTransitionHandsSideToHipsAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gSariaHandsBehindBackWaitAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gSariaHandsOnFaceAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gSariaWaitArmsToSideAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
};

s16 func_80AF5560(EnSa* self, GlobalContext* globalCtx) {
    s16 textState = func_8010BDBC(&globalCtx->msgCtx);

    if (self->unk_209 == 10 || self->unk_209 == 5 || self->unk_209 == 2 || self->unk_209 == 1) {
        if (textState != self->unk_209) {
            self->unk_208++;
        }
    }
    self->unk_209 = textState;
    return textState;
}

u16 func_80AF55E0(GlobalContext* globalCtx, Actor* thisx) {
    EnSa* self = THIS;
    u16 reaction = Text_GetFaceReaction(globalCtx, 0x10);

    if (reaction != 0) {
        return reaction;
    }
    if (CHECK_QUEST_ITEM(QUEST_SONG_SARIA)) {
        return 0x10AD;
    }
    if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) {
        self->unk_208 = 0;
        self->unk_209 = 0;
        if (gSaveContext.infTable[0] & 0x20) {
            return 0x1048;
        } else {
            return 0x1047;
        }
    }
    if (gSaveContext.eventChkInf[0] & 4) {
        self->unk_208 = 0;
        self->unk_209 = 0;
        if (gSaveContext.infTable[0] & 8) {
            return 0x1032;
        } else {
            return 0x1031;
        }
    }
    if (gSaveContext.infTable[0] & 1) {
        self->unk_208 = 0;
        self->unk_209 = 0;
        if (gSaveContext.infTable[0] & 2) {
            return 0x1003;
        } else {
            return 0x1002;
        }
    }
    return 0x1001;
}

s16 func_80AF56F4(GlobalContext* globalCtx, Actor* thisx) {
    s16 ret = 1;
    EnSa* self = THIS;

    switch (func_80AF5560(self, globalCtx)) {
        case 2:
            switch (self->actor.textId) {
                case 0x1002:
                    gSaveContext.infTable[0] |= 2;
                    ret = 0;
                    break;
                case 0x1031:
                    gSaveContext.eventChkInf[0] |= 8;
                    gSaveContext.infTable[0] |= 8;
                    ret = 0;
                    break;
                case 0x1047:
                    gSaveContext.infTable[0] |= 0x20;
                    ret = 0;
                    break;
                default:
                    ret = 0;
                    break;
            }
            break;
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
            break;
    }
    return ret;
}

void func_80AF57D8(EnSa* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum != SCENE_SPOT05 ||
        ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) < 0x1555 || self->unk_1E0.unk_00 != 0) {
        func_800343CC(globalCtx, &self->actor, &self->unk_1E0.unk_00, self->collider.dim.radius + 30.0f, func_80AF55E0,
                      func_80AF56F4);
    }
}

f32 func_80AF5894(EnSa* self) {
    f32 endFrame = self->skelAnime.endFrame;
    f32 startFrame = self->skelAnime.startFrame;

    self->skelAnime.startFrame = endFrame;
    self->skelAnime.curFrame = endFrame;
    self->skelAnime.endFrame = startFrame;
    self->skelAnime.playSpeed = -1.0f;
    return startFrame;
}

void func_80AF58B8(EnSa* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 3);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 2);
                self->unk_20A++;
            }
            break;
    }
}

void func_80AF594C(EnSa* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 8);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 9);
                self->unk_20A++;
            }
            break;
    }
}

void func_80AF59E0(EnSa* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 1);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 7);
                self->unk_20A++;
            }
            break;
    }
}

void func_80AF5A74(EnSa* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 1);
            func_80AF5894(self);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 9);
                self->unk_20A++;
            }
            break;
    }
}

void func_80AF5B10(EnSa* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 6);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 4);
                self->unk_20A++;
            }
            break;
    }
}

void func_80AF5BA4(EnSa* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 6);
            func_80AF5894(self);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 9);
                self->unk_20A++;
            }
            break;
    }
}

void func_80AF5C40(EnSa* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 5);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 0);
                self->unk_20A++;
            }
            break;
    }
}

void func_80AF5CD4(EnSa* self, u8 arg1) {
    self->unk_20B = arg1;
    self->unk_20A = 0;
}

void func_80AF5CE4(EnSa* self) {
    switch (self->unk_20B) {
        case 1:
            func_80AF58B8(self);
            break;
        case 2:
            func_80AF594C(self);
            break;
        case 3:
            func_80AF59E0(self);
            break;
        case 4:
            func_80AF5A74(self);
            break;
        case 5:
            func_80AF5B10(self);
            break;
        case 6:
            func_80AF5BA4(self);
            break;
        case 7:
            func_80AF5C40(self);
            break;
    }
}

void EnSa_ChangeAnim(EnSa* self, s32 action) {
    Animation_Change(&self->skelAnime, sAnimationInfo[action].animation, 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationInfo[action].animation), sAnimationInfo[action].mode,
                     sAnimationInfo[action].transitionRate);
}

s32 func_80AF5DFC(EnSa* self, GlobalContext* globalCtx) {
    if (gSaveContext.cutsceneIndex >= 0xFFF0 && gSaveContext.cutsceneIndex != 0xFFFD) {
        if (globalCtx->sceneNum == SCENE_SPOT04) {
            return 4;
        }
        if (globalCtx->sceneNum == SCENE_SPOT05) {
            return 5;
        }
    }
    if (globalCtx->sceneNum == SCENE_KOKIRI_HOME5 && !LINK_IS_ADULT &&
        INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_FAIRY && !(gSaveContext.eventChkInf[4] & 1)) {
        return 1;
    }
    if (globalCtx->sceneNum == SCENE_SPOT05 && (gSaveContext.eventChkInf[4] & 1)) {
        return CHECK_QUEST_ITEM(QUEST_SONG_SARIA) ? 2 : 5;
    }
    if (globalCtx->sceneNum == SCENE_SPOT04 && !CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) {
        if (gSaveContext.infTable[0] & 1) {
            return 1;
        }
        return 4;
    }
    return 0;
}

void func_80AF5F34(EnSa* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 phi_a3 = 0;

    if (globalCtx->sceneNum == SCENE_SPOT04) {
        phi_a3 = (self->actionFunc == func_80AF68E4) ? 1 : 4;
    }
    if (globalCtx->sceneNum == SCENE_SPOT05) {
        phi_a3 = (self->skelAnime.animation == &gSariaPlayingOcarinaAnim) ? 1 : 3;
    }
    if (globalCtx->sceneNum == SCENE_SPOT05 && self->actionFunc == func_80AF6448 &&
        self->skelAnime.animation == &gSariaStopPlayingOcarinaAnim) {
        phi_a3 = 1;
    }
    if (globalCtx->sceneNum == SCENE_SPOT05 && self->actionFunc == func_80AF68E4 &&
        self->skelAnime.animation == &gSariaOcarinaToMouthAnim) {
        phi_a3 = 1;
    }
    self->unk_1E0.unk_18 = player->actor.world.pos;
    self->unk_1E0.unk_14 = 4.0f;
    func_80034A14(&self->actor, &self->unk_1E0, 2, phi_a3);
}

s32 func_80AF603C(EnSa* self) {
    if (self->skelAnime.animation != &gSariaPlayingOcarinaAnim &&
        self->skelAnime.animation != &gSariaOcarinaToMouthAnim) {
        return 0;
    }
    if (self->unk_1E0.unk_00 != 0) {
        return 0;
    }
    self->unk_20E = 0;
    if (self->rightEyeIndex != SARIA_EYE_CLOSED) {
        return 0;
    }
    return 1;
}

void func_80AF609C(EnSa* self) {
    s16 phi_v1;

    if (func_80AF603C(self) == 0) {
        if (self->unk_20E == 0) {
            phi_v1 = 0;
        } else {
            self->unk_20E--;
            phi_v1 = self->unk_20E;
        }
        if (phi_v1 == 0) {
            self->rightEyeIndex++;
            if (self->rightEyeIndex < SARIA_EYE_SUPRISED) {
                self->leftEyeIndex = self->rightEyeIndex;
            } else {
                self->unk_20E = Rand_S16Offset(30, 30);
                self->leftEyeIndex = SARIA_EYE_OPEN;
                self->rightEyeIndex = self->leftEyeIndex;
            }
        }
    }
}

void func_80AF6130(CsCmdActorAction* csAction, Vec3f* dst) {
    dst->x = csAction->startPos.x;
    dst->y = csAction->startPos.y;
    dst->z = csAction->startPos.z;
}

void func_80AF6170(CsCmdActorAction* csAction, Vec3f* dst) {
    dst->x = csAction->endPos.x;
    dst->y = csAction->endPos.y;
    dst->z = csAction->endPos.z;
}

void EnSa_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSa* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 12.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gSariaSkel, NULL, self->jointTable, self->morphTable, 17);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit);

    switch (func_80AF5DFC(self, globalCtx)) {
        case 2:
            EnSa_ChangeAnim(self, 0xB);
            self->actionFunc = func_80AF6448;
            break;
        case 5:
            EnSa_ChangeAnim(self, 0xB);
            self->actionFunc = func_80AF683C;
            break;
        case 1:
            self->actor.gravity = -1.0f;
            EnSa_ChangeAnim(self, 0);
            self->actionFunc = func_80AF6448;
            break;
        case 4:
            self->unk_210 = 0;
            self->actor.gravity = -1.0f;
            globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gSpot04Cs_10E20);
            gSaveContext.cutsceneTrigger = 1;
            EnSa_ChangeAnim(self, 4);
            self->actionFunc = func_80AF68E4;
            break;
        case 3:
            self->unk_210 = 0;
            self->actor.gravity = -1.0f;
            EnSa_ChangeAnim(self, 0);
            self->actionFunc = func_80AF68E4;
            break;
        case 0:
            Actor_Kill(&self->actor);
            return;
    }

    Actor_SetScale(&self->actor, 0.01f);

    self->actor.targetMode = 6;
    self->unk_1E0.unk_00 = 0;
    self->alpha = 255;
    self->unk_21A = self->actor.shape.rot;

    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_ELF, self->actor.world.pos.x,
                       self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, FAIRY_KOKIRI);
}

void EnSa_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnSa* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80AF6448(EnSa* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_SPOT04) {
        if (self->unk_1E0.unk_00 != 0) {
            switch (self->actor.textId) {
                case 0x1002:
                    if (self->unk_208 == 0 && self->unk_20B != 1) {
                        func_80AF5CD4(self, 1);
                        self->mouthIndex = 1;
                    }
                    if (self->unk_208 == 2 && self->unk_20B != 2) {
                        func_80AF5CD4(self, 2);
                        self->mouthIndex = 1;
                    }
                    if (self->unk_208 == 5) {
                        self->mouthIndex = 0;
                    }
                    break;
                case 0x1003:
                    if (self->unk_208 == 0 && self->unk_20B != 4) {
                        func_80AF5CD4(self, 4);
                    }
                    break;
                case 0x1031:
                    if (self->unk_208 == 0 && self->unk_20B != 4 &&
                        self->skelAnime.animation == &gSariaHandsBehindBackWaitAnim) {
                        func_80AF5CD4(self, 4);
                        self->mouthIndex = 3;
                    }
                    if (self->unk_208 == 2 && self->unk_20B != 5) {
                        func_80AF5CD4(self, 5);
                        self->mouthIndex = 2;
                    }
                    if (self->unk_208 == 4 && self->unk_20B != 6) {
                        func_80AF5CD4(self, 6);
                        self->mouthIndex = 0;
                    }
                    break;
                case 0x1032:
                    if (self->unk_208 == 0 && self->unk_20B != 4 &&
                        self->skelAnime.animation == &gSariaHandsBehindBackWaitAnim) {
                        func_80AF5CD4(self, 4);
                    }
                    break;
                case 0x1047:
                    if (self->unk_208 == 1 && self->unk_20B != 7) {
                        func_80AF5CD4(self, 7);
                    }
                    break;
                case 0x1048:
                    if (self->unk_208 == 0 && self->unk_20B != 7) {
                        func_80AF5CD4(self, 7);
                    }
                    break;
            }
        } else if (!CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) &&
                   ((gSaveContext.infTable[0] & 2) || (gSaveContext.infTable[0] & 8))) {
            if (self->unk_20B != 3) {
                func_80AF5CD4(self, 3);
            }
        } else {
            func_80AF5CD4(self, 0);
        }
        func_80AF5CE4(self);
    }
    if (self->skelAnime.animation == &gSariaStopPlayingOcarinaAnim) {
        self->skelAnime.playSpeed = -1.0f;
        if ((s32)self->skelAnime.curFrame == 0) {
            EnSa_ChangeAnim(self, 6);
        }
    }
    if (self->unk_1E0.unk_00 != 0 && globalCtx->sceneNum == SCENE_SPOT05) {
        Animation_Change(&self->skelAnime, &gSariaStopPlayingOcarinaAnim, 1.0f, 0.0f, 10.0f, ANIMMODE_ONCE, -10.0f);
        self->actionFunc = func_80AF67D0;
    }
}

void func_80AF67D0(EnSa* self, GlobalContext* globalCtx) {
    if (self->unk_1E0.unk_00 == 0) {
        Animation_Change(&self->skelAnime, &gSariaStopPlayingOcarinaAnim, 0.0f, 10.0f, 0.0f, ANIMMODE_ONCE, -10.0f);
        self->actionFunc = func_80AF6448;
    }
}

void func_80AF683C(EnSa* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (!(player->actor.world.pos.z >= -2220.0f) && !Gameplay_InCsMode(globalCtx)) {
        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(spot05_sceneCutsceneData0x005730);
        gSaveContext.cutsceneTrigger = 1;
        self->actionFunc = func_80AF68E4;
    }
}

void func_80AF68E4(EnSa* self, GlobalContext* globalCtx) {
    s16 phi_v0;
    Vec3f startPos;
    Vec3f endPos;
    Vec3f D_80AF7448 = { 0.0f, 0.0f, 0.0f };
    CsCmdActorAction* csAction;
    f32 temp_f0;
    f32 gravity;

    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        self->actionFunc = func_80AF6B20;
        return;
    }
    csAction = globalCtx->csCtx.npcActions[1];
    if (csAction != NULL) {
        func_80AF6130(csAction, &startPos);
        func_80AF6170(csAction, &endPos);

        if (self->unk_210 == 0) {
            self->actor.world.pos = startPos;
        }
        if (self->unk_210 != csAction->action) {
            switch (csAction->action) {
                case 2:
                    self->mouthIndex = 1;
                    break;
                case 9:
                    self->mouthIndex = 1;
                    break;
                default:
                    self->mouthIndex = 0;
                    break;
            }
            EnSa_ChangeAnim(self, csAction->action);
            self->unk_210 = csAction->action;
        }
        if (phi_v0) {}
        if (csAction->action == 3) {
            if (self->unk_20C == 0) {
                phi_v0 = 0;
            } else {
                self->unk_20C--;
                phi_v0 = self->unk_20C;
            }
            if (phi_v0 == 0) {
                Audio_PlaySoundGeneral(NA_SE_PL_WALK_GROUND, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                       &D_801333E8);
                self->unk_20C = 8;
            }
        }
        self->actor.shape.rot.x = csAction->urot.x;
        self->actor.shape.rot.y = csAction->urot.y;
        self->actor.shape.rot.z = csAction->urot.z;
        self->actor.velocity = D_80AF7448;

        if (globalCtx->csCtx.frames < csAction->endFrame) {
            temp_f0 = csAction->endFrame - csAction->startFrame;
            self->actor.velocity.x = (endPos.x - startPos.x) / temp_f0;
            self->actor.velocity.y = (endPos.y - startPos.y) / temp_f0;
            gravity = self->actor.gravity;
            if (globalCtx->sceneNum == SCENE_SPOT05) {
                gravity = 0.0f;
            }
            self->actor.velocity.y += gravity;
            if (self->actor.velocity.y < self->actor.minVelocityY) {
                self->actor.velocity.y = self->actor.minVelocityY;
            }
            self->actor.velocity.z = (endPos.z - startPos.z) / temp_f0;
        }
    }
}

void func_80AF6B20(EnSa* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_SPOT05) {
        Item_Give(globalCtx, ITEM_SONG_SARIA);
        EnSa_ChangeAnim(self, 6);
    }

    if (globalCtx->sceneNum == SCENE_SPOT04) {
        EnSa_ChangeAnim(self, 4);
        self->actor.world.pos = self->actor.home.pos;
        self->actor.world.rot = self->unk_21A;
        self->mouthIndex = 0;
        gSaveContext.infTable[0] |= 1;
    }

    self->actionFunc = func_80AF6448;
}

void EnSa_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnSa* self = THIS;
    s32 pad;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    SkelAnime_Update(&self->skelAnime);

    if (self->skelAnime.animation == &gSariaOcarinaToMouthAnim &&
        self->skelAnime.curFrame >= Animation_GetLastFrame(&gSariaOcarinaToMouthAnim)) {
        EnSa_ChangeAnim(self, 6);
    }

    if (self->actionFunc != func_80AF68E4) {
        self->alpha = func_80034DD4(&self->actor, globalCtx, self->alpha, 400.0f);
    } else {
        self->alpha = 255;
    }

    self->actor.shape.shadowAlpha = self->alpha;

    if (self->actionFunc == func_80AF68E4) {
        self->actor.world.pos.x += self->actor.velocity.x;
        self->actor.world.pos.y += self->actor.velocity.y;
        self->actor.world.pos.z += self->actor.velocity.z;
    } else {
        func_8002D7EC(&self->actor);
    }

    if (globalCtx->sceneNum != SCENE_SPOT05) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    }

    func_80AF609C(self);
    self->actionFunc(self, globalCtx);
    func_80AF57D8(self, globalCtx);
    func_80AF5F34(self, globalCtx);
}

s32 EnSa_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                          Gfx** gfx) {
    EnSa* self = THIS;
    s32 pad;
    Vec3s sp18;

    if (limbIndex == 16) {
        Matrix_Translate(900.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        sp18 = self->unk_1E0.unk_08;
        Matrix_RotateX(BINANG_TO_RAD(sp18.y), MTXMODE_APPLY);
        Matrix_RotateZ(BINANG_TO_RAD(sp18.x), MTXMODE_APPLY);
        Matrix_Translate(-900.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }

    if (limbIndex == 9) {
        sp18 = self->unk_1E0.unk_0E;
        Matrix_RotateY(BINANG_TO_RAD(sp18.y), MTXMODE_APPLY);
        Matrix_RotateX(BINANG_TO_RAD(sp18.x), MTXMODE_APPLY);
    }

    if (globalCtx->sceneNum == SCENE_SPOT05 && limbIndex == 15) {
        *dList = gSariaRightHandAndOcarinaDL;
    }

    return 0;
}

void EnSa_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnSa* self = THIS;
    Vec3f D_80AF7454 = { 400.0, 0.0f, 0.0f };

    if (limbIndex == 16) {
        Matrix_MultVec3f(&D_80AF7454, &self->actor.focus.pos);
    }
}

void EnSa_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* mouthTextures[] = {
        gSariaMouthClosed2Tex,  gSariaMouthSmilingOpenTex, gSariaMouthFrowningTex,
        gSariaMouthSuprisedTex, gSariaMouthClosedTex,
    };
    static void* eyeTextures[] = {
        gSariaEyeOpenTex, gSariaEyeHalfTex, gSariaEyeClosedTex, gSariaEyeSuprisedTex, gSariaEyeSadTex,
    };
    EnSa* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_sa.c", 1444);

    if (self->alpha == 255) {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->rightEyeIndex]));
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTextures[self->leftEyeIndex]));
        gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(mouthTextures[self->mouthIndex]));
        func_80034BA0(globalCtx, &self->skelAnime, EnSa_OverrideLimbDraw, EnSa_PostLimbDraw, &self->actor, self->alpha);
    } else if (self->alpha != 0) {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->rightEyeIndex]));
        gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTextures[self->leftEyeIndex]));
        gSPSegment(POLY_XLU_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(mouthTextures[self->mouthIndex]));
        func_80034CC4(globalCtx, &self->skelAnime, EnSa_OverrideLimbDraw, EnSa_PostLimbDraw, &self->actor, self->alpha);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_sa.c", 1497);
}
