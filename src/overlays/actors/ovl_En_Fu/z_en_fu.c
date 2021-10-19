/*
 * File: z_en_fu.c
 * Overlay: ovl_En_Fu
 * Description: Windmill Man
 */

#include "z_en_fu.h"
#include "objects/object_fu/object_fu.h"
#include "scenes/indoors/hakasitarelay/hakasitarelay_scene.h"

#define FLAGS 0x02000019

#define THIS ((EnFu*)thisx)

#define FU_RESET_LOOK_ANGLE (1 << 0)
#define FU_WAIT (1 << 1)

void EnFu_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFu_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFu_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFu_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnFu_WaitChild(EnFu* self, GlobalContext* globalCtx);
void func_80A1DA04(EnFu* self, GlobalContext* globalCtx);

void EnFu_WaitAdult(EnFu* self, GlobalContext* globalCtx);
void EnFu_TeachSong(EnFu* self, GlobalContext* globalCtx);
void EnFu_WaitForPlayback(EnFu* self, GlobalContext* globalCtx);
void func_80A1DBA0(EnFu* self, GlobalContext* globalCtx);
void func_80A1DBD4(EnFu* self, GlobalContext* globalCtx);
void func_80A1DB60(EnFu* self, GlobalContext* globalCtx);

const ActorInit En_Fu_InitVars = {
    ACTOR_EN_FU,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_FU,
    sizeof(EnFu),
    (ActorFunc)EnFu_Init,
    (ActorFunc)EnFu_Destroy,
    (ActorFunc)EnFu_Update,
    (ActorFunc)EnFu_Draw,
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
    { 30, 40, 0, { 0, 0, 0 } },
};

static Vec3f sMtxSrc = {
    700.0f,
    700.0f,
    0.0f,
};

typedef enum {
    /* 0x00 */ FU_FACE_CALM,
    /* 0x01 */ FU_FACE_MAD
} EnFuFace;

void EnFu_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnFu* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelanime, &gWindmillManSkel, &gWindmillManPlayStillAnim, self->jointTable,
                       self->morphTable, FU_LIMB_MAX);
    Animation_PlayLoop(&self->skelanime, &gWindmillManPlayStillAnim);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&self->actor, 0.01f);
    if (!LINK_IS_ADULT) {
        self->actionFunc = EnFu_WaitChild;
        self->facialExpression = FU_FACE_CALM;
    } else {
        self->actionFunc = EnFu_WaitAdult;
        self->facialExpression = FU_FACE_MAD;
        self->skelanime.playSpeed = 2.0f;
    }
    self->behaviorFlags = 0;
    self->actor.targetMode = 6;
}

void EnFu_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFu* self = THIS;
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 func_80A1D94C(EnFu* self, GlobalContext* globalCtx, u16 textID, EnFuActionFunc actionFunc) {
    s16 yawDiff;

    // func_8002F194 returns 1 if actor flags & 0x100 is set and unsets it
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = actionFunc;
        return true;
    }
    self->actor.textId = textID;
    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if ((ABS(yawDiff) < 0x2301) && (self->actor.xzDistToPlayer < 100.0f)) {
        func_8002F2CC(&self->actor, globalCtx, 100.0f);
    } else {
        self->behaviorFlags |= FU_RESET_LOOK_ANGLE;
    }
    return false;
}

void func_80A1DA04(EnFu* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx) != 0) {
        self->behaviorFlags &= ~FU_WAIT;
        self->actionFunc = EnFu_WaitChild;

        if (self->skelanime.animation == &gWindmillManPlayAndMoveHeadAnim) {
            Animation_Change(&self->skelanime, &gWindmillManPlayStillAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gWindmillManPlayStillAnim), ANIMMODE_ONCE, -4.0f);
        }
    }
}

void EnFu_WaitChild(EnFu* self, GlobalContext* globalCtx) {
    u16 textID = Text_GetFaceReaction(globalCtx, 0xB);

    if (textID == 0) {
        textID = (gSaveContext.eventChkInf[6] & 0x80) ? 0x5033 : 0x5032;
    }

    // if actor flags & 0x100 is set and textID is 0x5033, change animation
    // if func_80A1D94C returns 1, actionFunc is set to func_80A1DA04
    if (func_80A1D94C(self, globalCtx, textID, func_80A1DA04)) {
        if (textID == 0x5033) {
            Animation_Change(&self->skelanime, &gWindmillManPlayAndMoveHeadAnim, 1.0f, 0.0f,
                             Animation_GetLastFrame(&gWindmillManPlayAndMoveHeadAnim), ANIMMODE_ONCE, -4.0f);
        }
    }
}

void func_80A1DB60(EnFu* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        self->actionFunc = EnFu_WaitAdult;
        gSaveContext.eventChkInf[5] |= 0x800;
        globalCtx->msgCtx.unk_E3EE = 4;
    }
}

void func_80A1DBA0(EnFu* self, GlobalContext* globalCtx) {
    // if dialog state is 2 set action to WaitAdult
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actionFunc = EnFu_WaitAdult;
    }
}

void func_80A1DBD4(EnFu* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (globalCtx->msgCtx.unk_E3EE >= 4) {
        self->actionFunc = EnFu_WaitAdult;
        globalCtx->msgCtx.unk_E3EE = 4;
        self->actor.flags &= ~0x10000;
    } else if (globalCtx->msgCtx.unk_E3EE == 3) {
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        self->actionFunc = func_80A1DB60;
        self->actor.flags &= ~0x10000;
        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gSongOfStormsCs);
        gSaveContext.cutsceneTrigger = 1;
        Item_Give(globalCtx, ITEM_SONG_STORMS);
        globalCtx->msgCtx.unk_E3EE = 0;
        gSaveContext.eventChkInf[6] |= 0x20;
    } else if (globalCtx->msgCtx.unk_E3EE == 2) {
        player->stateFlags2 &= ~0x1000000;
        self->actionFunc = EnFu_WaitAdult;
    } else if (globalCtx->msgCtx.unk_E3EE == 1) {
        player->stateFlags2 |= 0x800000;
    }
}

void EnFu_WaitForPlayback(EnFu* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    player->stateFlags2 |= 0x800000;
    // if dialog state is 7, player has played back the song
    if (func_8010BDBC(&globalCtx->msgCtx) == 7) {
        func_8010BD58(globalCtx, 0x1A);
        self->actionFunc = func_80A1DBD4;
    }
}

void EnFu_TeachSong(EnFu* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    player->stateFlags2 |= 0x800000;
    // if dialog state is 2, start song demonstration
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        self->behaviorFlags &= ~FU_WAIT;
        func_800ED858(4);              // seems to be related to setting instrument type
        func_8010BD58(globalCtx, 0xD); // play song demonstration, song 0xD = SoS
        self->actionFunc = EnFu_WaitForPlayback;
    }
}

void EnFu_WaitAdult(EnFu* self, GlobalContext* globalCtx) {
    static s16 yawDiff;
    Player* player = GET_PLAYER(globalCtx);

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    if ((gSaveContext.eventChkInf[5] & 0x800)) {
        func_80A1D94C(self, globalCtx, 0x508E, func_80A1DBA0);
    } else if (player->stateFlags2 & 0x1000000) {
        self->actor.textId = 0x5035;
        func_8010B680(globalCtx, self->actor.textId, NULL);
        self->actionFunc = EnFu_TeachSong;
        self->behaviorFlags |= FU_WAIT;
    } else if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = func_80A1DBA0;
    } else if (ABS(yawDiff) < 0x2301) {
        if (self->actor.xzDistToPlayer < 100.0f) {
            self->actor.textId = 0x5034;
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
            player->stateFlags2 |= 0x800000;
        }
    }
}

void EnFu_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnFu* self = THIS;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    if ((!(self->behaviorFlags & FU_WAIT)) && (SkelAnime_Update(&self->skelanime) != 0)) {
        Animation_Change(&self->skelanime, self->skelanime.animation, 1.0f, 0.0f,
                         Animation_GetLastFrame(self->skelanime.animation), ANIMMODE_ONCE, 0.0f);
    }
    self->actionFunc(self, globalCtx);
    if ((self->behaviorFlags & FU_RESET_LOOK_ANGLE)) {
        Math_SmoothStepToS(&self->lookAngleOffset.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->lookAngleOffset.y, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_2A2.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->unk_2A2.y, 0, 6, 6200, 100);
        self->behaviorFlags &= ~FU_RESET_LOOK_ANGLE;
    } else {
        func_80038290(globalCtx, &self->actor, &self->lookAngleOffset, &self->unk_2A2, self->actor.focus.pos);
    }
}

s32 EnFu_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnFu* self = THIS;
    s32 pad;

    if (limbIndex == FU_LIMB_UNK) {
        return false;
    }
    switch (limbIndex) {
        case FU_LIMB_HEAD:
            rot->x += self->lookAngleOffset.y;
            rot->z += self->lookAngleOffset.x;
            break;
        case FU_LIMB_CHEST_MUSIC_BOX:
            break;
    }

    if (!(self->behaviorFlags & FU_WAIT)) {
        return false;
    }

    if (limbIndex == FU_LIMB_CHEST_MUSIC_BOX) {
        rot->y += (Math_SinS((globalCtx->state.frames * (limbIndex * 50 + 0x814))) * 200.0f);
        rot->z += (Math_CosS((globalCtx->state.frames * (limbIndex * 50 + 0x940))) * 200.0f);
    }
    return false;
}

void EnFu_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnFu* self = THIS;

    if (limbIndex == FU_LIMB_HEAD) {
        Matrix_MultVec3f(&sMtxSrc, &self->actor.focus.pos);
    }
}

void EnFu_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* sEyesSegments[] = { gWindmillManEyeClosedTex, gWindmillManEyeAngryTex };
    static void* sMouthSegments[] = { gWindmillManMouthOpenTex, gWindmillManMouthAngryTex };
    s32 pad;
    EnFu* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fu.c", 773);

    func_800943C8(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyesSegments[self->facialExpression]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sMouthSegments[self->facialExpression]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelanime.skeleton, self->skelanime.jointTable, self->skelanime.dListCount,
                          EnFu_OverrideLimbDraw, EnFu_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fu.c", 791);
}
