/*
 * File: z_en_md.c
 * Overlay: ovl_En_Md
 * Description: Mido
 */

#include "z_en_md.h"
#include "objects/object_md/object_md.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"

#define FLAGS 0x02000019

#define THIS ((EnMd*)thisx)

void EnMd_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMd_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMd_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AAB874(EnMd* self, GlobalContext* globalCtx);
void func_80AAB8F8(EnMd* self, GlobalContext* globalCtx);
void func_80AAB948(EnMd* self, GlobalContext* globalCtx);
void func_80AABC10(EnMd* self, GlobalContext* globalCtx);
void func_80AABD0C(EnMd* self, GlobalContext* globalCtx);

const ActorInit En_Md_InitVars = {
    ACTOR_EN_MD,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MD,
    sizeof(EnMd),
    (ActorFunc)EnMd_Init,
    (ActorFunc)EnMd_Destroy,
    (ActorFunc)EnMd_Update,
    (ActorFunc)EnMd_Draw,
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
    { 36, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static struct_80034EC0_Entry sAnimations[] = {
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -10.0f },
    { &gMidoRaiseHand1Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoHaltAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoPutHandDownAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoAnnoyedPointedHeadIdle1Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoAnnoyedPointedHeadIdle2Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoAnim_92B0, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoWalkingAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoHandsOnHipsTransitionAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    { &gMidoSlamAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoRaiseHand2Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoAngryHeadTurnAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
};

void func_80AAA250(EnMd* self) {
    f32 startFrame;

    startFrame = self->skelAnime.startFrame;
    self->skelAnime.startFrame = self->skelAnime.endFrame;
    self->skelAnime.curFrame = self->skelAnime.endFrame;
    self->skelAnime.endFrame = startFrame;
    self->skelAnime.playSpeed = -1.0f;
}

void func_80AAA274(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 2);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 3);
                self->unk_20A++;
            }
    }
}

void func_80AAA308(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 4);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 5);
                self->unk_20A++;
            }
    }
}

void func_80AAA39C(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 2);
            func_80AAA250(self);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 7);
                self->unk_20A++;
            } else {
                break;
            }
        case 2:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 8);
                self->unk_20A++;
            }
    }
}

void func_80AAA474(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 7);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 10);
                self->unk_20A++;
            }
    }
}

void func_80AAA508(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 2);
            func_80AAA250(self);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 10);
                self->unk_20A++;
            }
    }
}

void func_80AAA5A4(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 9);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 6);
                self->unk_20A++;
            }
    }
}

void func_80AAA638(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 9);
            func_80AAA250(self);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 10);
                self->unk_20A++;
            }
    }
}

void func_80AAA6D4(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 11);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 6);
                self->unk_20A++;
            }
    }
}

void func_80AAA768(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 12);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 3);
                self->unk_20A++;
            }
    }
}

void func_80AAA7FC(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 13);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 6);
                self->unk_20A++;
            }
    }
}

void func_80AAA890(EnMd* self) {
    switch (self->unk_20A) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 7);
            func_80AAA250(self);
            self->unk_20A++;
        case 1:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                func_80034EC0(&self->skelAnime, sAnimations, 10);
                self->unk_20A++;
            }
    }
}

void func_80AAA92C(EnMd* self, u8 arg1) {
    self->unk_20B = arg1;
    self->unk_20A = 0;
}

void func_80AAA93C(EnMd* self) {
    switch (self->unk_20B) {
        case 1:
            func_80AAA274(self);
            break;
        case 2:
            func_80AAA308(self);
            break;
        case 3:
            func_80AAA39C(self);
            break;
        case 4:
            func_80AAA474(self);
            break;
        case 5:
            func_80AAA508(self);
            break;
        case 6:
            func_80AAA5A4(self);
            break;
        case 7:
            func_80AAA638(self);
            break;
        case 8:
            func_80AAA6D4(self);
            break;
        case 9:
            func_80AAA768(self);
            break;
        case 10:
            func_80AAA7FC(self);
            break;
        case 11:
            func_80AAA890(self);
    }
}

void func_80AAAA24(EnMd* self) {
    if (self->unk_1E0.unk_00 != 0) {
        switch (self->actor.textId) {
            case 0x102F:
                if ((self->unk_208 == 0) && (self->unk_20B != 1)) {
                    func_80AAA92C(self, 1);
                }
                if ((self->unk_208 == 2) && (self->unk_20B != 2)) {
                    func_80AAA92C(self, 2);
                }
                if ((self->unk_208 == 5) && (self->unk_20B != 8)) {
                    func_80AAA92C(self, 8);
                }
                if ((self->unk_208 == 11) && (self->unk_20B != 9)) {
                    func_80AAA92C(self, 9);
                }
                break;
            case 0x1033:
                if ((self->unk_208 == 0) && (self->unk_20B != 1)) {
                    func_80AAA92C(self, 1);
                }
                if ((self->unk_208 == 1) && (self->unk_20B != 2)) {
                    func_80AAA92C(self, 2);
                }
                if ((self->unk_208 == 5) && (self->unk_20B != 10)) {
                    func_80AAA92C(self, 10);
                }
                if ((self->unk_208 == 7) && (self->unk_20B != 9)) {
                    func_80AAA92C(self, 9);
                }
                break;
            case 0x1030:
            case 0x1034:
            case 0x1045:
                if ((self->unk_208 == 0) && (self->unk_20B != 1)) {
                    func_80AAA92C(self, 1);
                }
                break;
            case 0x1046:
                if ((self->unk_208 == 0) && (self->unk_20B != 6)) {
                    func_80AAA92C(self, 6);
                }
                break;
        }
    } else if (self->skelAnime.animation != &gMidoHandsOnHipsIdleAnim) {
        func_80034EC0(&self->skelAnime, sAnimations, 10);
        func_80AAA92C(self, 0);
    }

    func_80AAA93C(self);
}

s16 func_80AAAC78(EnMd* self, GlobalContext* globalCtx) {
    s16 dialogState = func_8010BDBC(&globalCtx->msgCtx);

    if ((self->unk_209 == 10) || (self->unk_209 == 5) || (self->unk_209 == 2) || (self->unk_209 == 1)) {
        if (self->unk_209 != dialogState) {
            self->unk_208++;
        }
    }

    self->unk_209 = dialogState;
    return dialogState;
}

u16 EnMd_GetTextKokiriForest(GlobalContext* globalCtx, EnMd* self) {
    u16 reactionText = Text_GetFaceReaction(globalCtx, 0x11);

    if (reactionText != 0) {
        return reactionText;
    }

    self->unk_208 = 0;
    self->unk_209 = 0;

    if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) {
        return 0x1045;
    }

    if (gSaveContext.eventChkInf[0] & 0x10) {
        return 0x1034;
    }

    if ((CUR_EQUIP_VALUE(EQUIP_SHIELD) == 1) && (CUR_EQUIP_VALUE(EQUIP_SWORD) == 1)) {
        return 0x1033;
    }

    if (gSaveContext.infTable[0] & 0x1000) {
        return 0x1030;
    }

    return 0x102F;
}

u16 EnMd_GetTextKokiriHome(GlobalContext* globalCtx, EnMd* self) {
    self->unk_208 = 0;
    self->unk_209 = 0;

    if (gSaveContext.eventChkInf[4] & 1) {
        return 0x1028;
    }

    return 0x1046;
}

u16 EnMd_GetTextLostWoods(GlobalContext* globalCtx, EnMd* self) {
    self->unk_208 = 0;
    self->unk_209 = 0;

    if (gSaveContext.eventChkInf[4] & 0x100) {
        if (gSaveContext.infTable[1] & 0x200) {
            return 0x1071;
        }
        return 0x1070;
    }

    if (gSaveContext.eventChkInf[0] & 0x400) {
        return 0x1068;
    }

    if (gSaveContext.infTable[1] & 0x20) {
        return 0x1061;
    }

    return 0x1060;
}

u16 EnMd_GetText(GlobalContext* globalCtx, Actor* thisx) {
    EnMd* self = THIS;

    switch (globalCtx->sceneNum) {
        case SCENE_SPOT04:
            return EnMd_GetTextKokiriForest(globalCtx, self);
        case SCENE_KOKIRI_HOME4:
            return EnMd_GetTextKokiriHome(globalCtx, self);
        case SCENE_SPOT10:
            return EnMd_GetTextLostWoods(globalCtx, self);
        default:
            return 0;
    }
}

s16 func_80AAAF04(GlobalContext* globalCtx, Actor* thisx) {
    EnMd* self = THIS;
    switch (func_80AAAC78(self, globalCtx)) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
            return 1;
        case 2:
            switch (self->actor.textId) {
                case 0x1028:
                    gSaveContext.eventChkInf[0] |= 0x8000;
                    break;
                case 0x102F:
                    gSaveContext.eventChkInf[0] |= 4;
                    gSaveContext.infTable[0] |= 0x1000;
                    break;
                case 0x1060:
                    gSaveContext.infTable[1] |= 0x20;
                    break;
                case 0x1070:
                    gSaveContext.infTable[1] |= 0x200;
                    break;
                case 0x1033:
                case 0x1067:
                    return 2;
            }
            return 0;
        case 5:
            if (func_80106BC8(globalCtx) != 0) {
                return 2;
            }
        default:
            return 1;
    }
}

u8 EnMd_ShouldSpawn(EnMd* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_SPOT04) {
        if (!(gSaveContext.eventChkInf[1] & 0x1000) && !(gSaveContext.eventChkInf[4] & 1)) {
            return 1;
        }
    }

    if (globalCtx->sceneNum == SCENE_KOKIRI_HOME4) {
        if (((gSaveContext.eventChkInf[1] & 0x1000) != 0) || ((gSaveContext.eventChkInf[4] & 1) != 0)) {
            if (!LINK_IS_ADULT) {
                return 1;
            }
        }
    }

    if (globalCtx->sceneNum == SCENE_SPOT10) {
        return 1;
    }

    return 0;
}

void EnMd_UpdateEyes(EnMd* self) {
    if (DECR(self->blinkTimer) == 0) {
        self->eyeIdx++;
        if (self->eyeIdx > 2) {
            self->blinkTimer = Rand_S16Offset(30, 30);
            self->eyeIdx = 0;
        }
    }
}

void func_80AAB158(EnMd* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 absYawDiff;
    s16 temp;
    s16 temp2;
    s16 yawDiff;

    if (self->actor.xzDistToPlayer < 170.0f) {
        yawDiff = (f32)self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        absYawDiff = ABS(yawDiff);

        temp = (absYawDiff <= func_800347E8(2)) ? 2 : 1;
        temp2 = 1;
    } else {
        temp = 1;
        temp2 = 0;
    }

    if (self->unk_1E0.unk_00 != 0) {
        temp = 4;
    }

    if (self->actionFunc == func_80AABD0C) {
        temp = 1;
        temp2 = 0;
    }
    if (self->actionFunc == func_80AAB8F8) {
        temp = 4;
        temp2 = 1;
    }

    if ((globalCtx->csCtx.state != CS_STATE_IDLE) || gDbgCamEnabled) {
        self->unk_1E0.unk_18 = globalCtx->view.eye;
        self->unk_1E0.unk_14 = 40.0f;
        temp = 2;
    } else {
        self->unk_1E0.unk_18 = player->actor.world.pos;
        self->unk_1E0.unk_14 = (gSaveContext.linkAge > 0) ? 0.0f : -18.0f;
    }

    func_80034A14(&self->actor, &self->unk_1E0, 2, temp);
    if (self->actionFunc != func_80AABC10) {
        if (temp2) {
            func_800343CC(globalCtx, &self->actor, &self->unk_1E0.unk_00, self->collider.dim.radius + 30.0f,
                          EnMd_GetText, func_80AAAF04);
        }
    }
}

u8 EnMd_FollowPath(EnMd* self, GlobalContext* globalCtx) {
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
    Math_SmoothStepToS(&self->actor.world.rot.y, Math_FAtan2F(pathDiffX, pathDiffZ) * (65536.0f / (2 * M_PI)), 4, 4000,
                       1);

    if ((SQ(pathDiffX) + SQ(pathDiffZ)) < 100.0f) {
        self->waypoint++;
        if (self->waypoint >= path->count) {
            self->waypoint = 0;
        }
        return 1;
    }
    return 0;
}

u8 EnMd_SetMovedPos(EnMd* self, GlobalContext* globalCtx) {
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

void func_80AAB5A4(EnMd* self, GlobalContext* globalCtx) {
    f32 temp;

    if (globalCtx->sceneNum != SCENE_KOKIRI_HOME4) {
        temp = (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !(gSaveContext.eventChkInf[1] & 0x1000) &&
                (globalCtx->sceneNum == SCENE_SPOT04))
                   ? 100.0f
                   : 400.0f;
        self->alpha = func_80034DD4(&self->actor, globalCtx, self->alpha, temp);
        self->actor.shape.shadowAlpha = self->alpha;
    } else {
        self->alpha = 255;
        self->actor.shape.shadowAlpha = self->alpha;
    }
}

void EnMd_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnMd* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gMidoSkel, NULL, self->jointTable, self->morphTable, 17);

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    if (!EnMd_ShouldSpawn(self, globalCtx)) {
        Actor_Kill(&self->actor);
        return;
    }

    func_80034EC0(&self->skelAnime, sAnimations, 0);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.targetMode = 6;
    self->alpha = 255;
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_ELF, self->actor.world.pos.x,
                       self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, FAIRY_KOKIRI);

    if (((globalCtx->sceneNum == SCENE_SPOT04) && !(gSaveContext.eventChkInf[0] & 0x10)) ||
        ((globalCtx->sceneNum == SCENE_SPOT04) && (gSaveContext.eventChkInf[0] & 0x10) &&
         CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) ||
        ((globalCtx->sceneNum == SCENE_SPOT10) && !(gSaveContext.eventChkInf[0] & 0x400))) {
        self->actor.home.pos = self->actor.world.pos;
        self->actionFunc = func_80AAB948;
        return;
    }

    if (globalCtx->sceneNum != SCENE_KOKIRI_HOME4) {
        EnMd_SetMovedPos(self, globalCtx);
    }

    self->actionFunc = func_80AAB874;
}

void EnMd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnMd* self = THIS;
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80AAB874(EnMd* self, GlobalContext* globalCtx) {
    if (self->skelAnime.animation == &gMidoHandsOnHipsIdleAnim) {
        func_80034F54(globalCtx, self->unk_214, self->unk_236, 17);
    } else if ((self->unk_1E0.unk_00 == 0) && (self->unk_20B != 7)) {
        func_80AAA92C(self, 7);
    }

    func_80AAAA24(self);
}

void func_80AAB8F8(EnMd* self, GlobalContext* globalCtx) {
    if (self->skelAnime.animation == &gMidoHandsOnHipsIdleAnim) {
        func_80034F54(globalCtx, self->unk_214, self->unk_236, 17);
    }
    func_80AAA93C(self);
}

void func_80AAB948(EnMd* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 temp;
    Actor* actorToBlock = &GET_PLAYER(globalCtx)->actor;
    s16 yaw;

    func_80AAAA24(self);

    if (self->unk_1E0.unk_00 == 0) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer;
        self->actor.shape.rot.y = self->actor.yawTowardsPlayer;

        yaw = Math_Vec3f_Yaw(&self->actor.home.pos, &actorToBlock->world.pos);

        self->actor.world.pos.x = self->actor.home.pos.x;
        self->actor.world.pos.x += 60.0f * Math_SinS(yaw);

        self->actor.world.pos.z = self->actor.home.pos.z;
        self->actor.world.pos.z += 60.0f * Math_CosS(yaw);

        temp = fabsf((f32)self->actor.yawTowardsPlayer - yaw) * 0.001f * 3.0f;
        self->skelAnime.playSpeed = CLAMP(temp, 1.0f, 3.0f);
    }

    if (self->unk_1E0.unk_00 == 2) {
        if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !(gSaveContext.eventChkInf[1] & 0x1000) &&
            (globalCtx->sceneNum == SCENE_SPOT04)) {
            globalCtx->msgCtx.msgMode = 0x37;
        }

        if (globalCtx->sceneNum == SCENE_SPOT04) {
            gSaveContext.eventChkInf[0] |= 0x10;
        }
        if (globalCtx->sceneNum == SCENE_SPOT10) {
            gSaveContext.eventChkInf[0] |= 0x400;
        }

        func_80AAA92C(self, 3);
        func_80AAA93C(self);
        self->waypoint = 1;
        self->unk_1E0.unk_00 = 0;
        self->actionFunc = func_80AABD0C;
        self->actor.speedXZ = 1.5f;
        return;
    }

    if (self->skelAnime.animation == &gMidoHandsOnHipsIdleAnim) {
        func_80034F54(globalCtx, self->unk_214, self->unk_236, 17);
    }

    if ((self->unk_1E0.unk_00 == 0) && (globalCtx->sceneNum == SCENE_SPOT10)) {
        if (player->stateFlags2 & 0x1000000) {
            player->stateFlags2 |= 0x2000000;
            player->unk_6A8 = &self->actor;
            func_8010BD58(globalCtx, 0x22);
            self->actionFunc = func_80AABC10;
            return;
        }

        if (self->actor.xzDistToPlayer < (30.0f + self->collider.dim.radius)) {
            player->stateFlags2 |= 0x800000;
        }
    }
}

void func_80AABC10(EnMd* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (globalCtx->msgCtx.unk_E3EE >= 4) {
        self->actionFunc = func_80AAB948;
        globalCtx->msgCtx.unk_E3EE = 4;
    } else if (globalCtx->msgCtx.unk_E3EE == 3) {
        Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        self->actor.textId = 0x1067;
        func_8002F2CC(&self->actor, globalCtx, self->collider.dim.radius + 30.0f);

        self->actionFunc = func_80AAB948;
        globalCtx->msgCtx.unk_E3EE = 4;
    } else {
        player->stateFlags2 |= 0x800000;
    }
}

void func_80AABD0C(EnMd* self, GlobalContext* globalCtx) {
    func_80034F54(globalCtx, self->unk_214, self->unk_236, 17);
    func_80AAA93C(self);

    if (!(EnMd_FollowPath(self, globalCtx)) || (self->waypoint != 0)) {
        self->actor.shape.rot = self->actor.world.rot;
        return;
    }

    if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !(gSaveContext.eventChkInf[1] & 0x1000) &&
        (globalCtx->sceneNum == SCENE_SPOT04)) {
        func_80106CCC(globalCtx);
        gSaveContext.eventChkInf[1] |= 0x1000;
        Actor_Kill(&self->actor);
        return;
    }

    func_80AAA92C(self, 11);

    self->skelAnime.playSpeed = 0.0f;
    self->actor.speedXZ = 0.0f;
    self->actor.home.pos = self->actor.world.pos;
    self->actionFunc = func_80AAB8F8;
}

void EnMd_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnMd* self = THIS;
    s32 pad;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    SkelAnime_Update(&self->skelAnime);
    EnMd_UpdateEyes(self);
    func_80AAB5A4(self, globalCtx);
    Actor_MoveForward(&self->actor);
    func_80AAB158(self, globalCtx);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    self->actionFunc(self, globalCtx);
}

s32 EnMd_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                          Gfx** gfx) {
    EnMd* self = THIS;
    Vec3s vec;

    if (limbIndex == 16) {
        Matrix_Translate(1200.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        vec = self->unk_1E0.unk_08;
        Matrix_RotateX((vec.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((vec.x / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_Translate(-1200.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }
    if (limbIndex == 9) {
        vec = self->unk_1E0.unk_0E;
        Matrix_RotateX((vec.x / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateY((vec.y / 32768.0f) * M_PI, MTXMODE_APPLY);
    }

    if (((limbIndex == 9) || (limbIndex == 10)) || (limbIndex == 13)) {
        rot->y += Math_SinS(self->unk_214[limbIndex]) * 200.0f;
        rot->z += Math_CosS(self->unk_236[limbIndex]) * 200.0f;
    }

    return false;
}

void EnMd_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnMd* self = THIS;
    Vec3f vec = { 400.0f, 0.0f, 0.0f };

    if (limbIndex == 16) {
        Matrix_MultVec3f(&vec, &self->actor.focus.pos);
    }
}

void EnMd_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* sEyeTextures[] = {
        &gMidoEyeOpenTex,
        &gMidoEyeHalfTex,
        &gMidoEyeClosedTex,
    };
    EnMd* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_md.c", 1280);

    if (self->alpha == 255) {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeIdx]));
        func_80034BA0(globalCtx, &self->skelAnime, EnMd_OverrideLimbDraw, EnMd_PostLimbDraw, &self->actor, self->alpha);
    } else if (self->alpha != 0) {
        gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeIdx]));
        func_80034CC4(globalCtx, &self->skelAnime, EnMd_OverrideLimbDraw, EnMd_PostLimbDraw, &self->actor, self->alpha);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_md.c", 1317);
}
