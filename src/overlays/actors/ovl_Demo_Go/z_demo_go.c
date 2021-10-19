/*
 * File: z_demo_go.c
 * Overlay: Demo_Go
 * Description: Gorons (Cutscene)
 */

#include "z_demo_go.h"
#include "objects/object_oF1d_map/object_oF1d_map.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((DemoGo*)thisx)

void DemoGo_Init(Actor* thisx, GlobalContext* globalCtx);
void DemoGo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DemoGo_Update(Actor* thisx, GlobalContext* globalCtx);
void DemoGo_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8097CE10(DemoGo* self, GlobalContext* globalCtx);
void func_8097CFDC(DemoGo* self, GlobalContext* globalCtx);
void func_8097CFFC(DemoGo* self, GlobalContext* globalCtx);
void func_8097D01C(DemoGo* self, GlobalContext* globalCtx);
void func_8097D058(DemoGo* self, GlobalContext* globalCtx);
void func_8097D088(DemoGo* self, GlobalContext* globalCtx);
void func_8097D0D0(DemoGo* self, GlobalContext* globalCtx);
void func_8097D130(DemoGo* self, GlobalContext* globalCtx);
void func_8097D290(DemoGo* self, GlobalContext* globalCtx);
void func_8097D29C(DemoGo* self, GlobalContext* globalCtx);

static void* sEyeTextures[] = { gGoronCsEyeOpenTex, gGoronCsEyeHalfTex, gGoronCsEyeClosedTex };

static DemoGoActionFunc D_8097D44C[] = {
    func_8097CFDC, func_8097CFFC, func_8097D01C, func_8097D058, func_8097D088, func_8097D0D0, func_8097D130,
};

static DemoGoDrawFunc D_8097D468[] = {
    func_8097D290,
    func_8097D29C,
};

const ActorInit Demo_Go_InitVars = {
    ACTOR_DEMO_GO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OF1D_MAP,
    sizeof(DemoGo),
    (ActorFunc)DemoGo_Init,
    (ActorFunc)DemoGo_Destroy,
    (ActorFunc)DemoGo_Update,
    (ActorFunc)DemoGo_Draw,
};

s32 func_8097C870(DemoGo* self) {
    s32 ret;

    switch (self->actor.params) {
        case 0:
            ret = 3;
            break;
        case 1:
            ret = 4;
            break;
        default:
            if (1) {
                ret = 5;
            }
            break;
    }
    return ret;
}

void func_8097C8A8(DemoGo* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->actor;
    Vec3f sp20;
    f32 sp1C;

    if ((thisx->params == 0) || (thisx->params == 1)) {
        SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, &thisx->world.pos, &sp20, &sp1C);
        Audio_PlaySoundAtPosition(globalCtx, &sp20, 20, NA_SE_EV_OBJECT_FALL);
    }
}

void DemoGo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    DemoGo* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
}

void func_8097C930(DemoGo* self) {
    s16* something = &self->unk_192;
    s16* other = &self->unk_190;
    s32 pad[3];

    if (DECR(*something) == 0) {
        *something = Rand_S16Offset(60, 60);
    }
    *other = *something;
    if (*other >= 3) {
        *other = 0;
    }
}

void func_8097C9B8(DemoGo* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_EN_DODO_M_GND);
}

void func_8097C9DC(DemoGo* self) {
    s32 pad[2];
    if (Animation_OnFrame(&self->skelAnime, 12.0f) || Animation_OnFrame(&self->skelAnime, 25.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_EN_MORIBLIN_WALK);
    }
}

void func_8097CA30(DemoGo* self, GlobalContext* globalCtx) {
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 75.0f, 30.0f, 30.0f, 5);
}

void func_8097CA78(DemoGo* self, GlobalContext* globalCtx) {
    s16 pad;
    Vec3f vec = self->actor.world.pos;
    func_80033480(globalCtx, &vec, kREG(11) + 100.0f, kREG(12) + 0xA, kREG(13) + 0x12C, kREG(14), 0);
    func_8097C9B8(self);
}

void func_8097CB0C(DemoGo* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->actor;
    PosRot* world = &thisx->world;
    CutsceneContext* csCtx = &globalCtx->csCtx;
    CsCmdActorAction* npcAction;
    f32 temp_ret;
    s32 pad;
    Vec3f startPos;
    Vec3f endPos;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        npcAction = csCtx->npcActions[func_8097C870(self)];
        if (npcAction != NULL) {
            temp_ret = Environment_LerpWeight(npcAction->endFrame, npcAction->startFrame, csCtx->frames);
            startPos.x = npcAction->startPos.x;
            startPos.y = npcAction->startPos.y;
            startPos.z = npcAction->startPos.z;
            endPos.x = npcAction->endPos.x;
            endPos.y = npcAction->endPos.y;
            endPos.z = npcAction->endPos.z;
            world->pos.x = (endPos.x - startPos.x) * temp_ret + startPos.x;
            world->pos.y = (endPos.y - startPos.y) * temp_ret + startPos.y;
            world->pos.z = (endPos.z - startPos.z) * temp_ret + startPos.z;
            world->rot.y = thisx->shape.rot.y = npcAction->rot.y;
        }
    }
}

void func_8097CC08(DemoGo* self) {
    f32 something = self->unk_19C;

    if (something < 8.0f) {
        self->actor.speedXZ = (((kREG(15) * 0.01f) + 1.2f) / 8.0f) * something;
    } else {
        self->actor.speedXZ = (kREG(15) * 0.01f) + 1.2f;
    }
    Actor_MoveForward(&self->actor);
}

void func_8097CCC0(DemoGo* self) {
    Actor_MoveForward(&self->actor);
}

void func_8097CCE0(DemoGo* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction;
    Actor* thisx = &self->actor;
    s32 rotYDelta;
    s32 newRotY;
    s32 thisRotY;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        npcAction = globalCtx->csCtx.npcActions[func_8097C870(self)];
        if (npcAction != NULL) {
            thisRotY = thisx->world.rot.y;
            rotYDelta = npcAction->rot.y - thisRotY;
            if ((rotYDelta > -(kREG(16) + 0x96)) && (rotYDelta < kREG(16) + 0x96)) {
                newRotY = npcAction->rot.y;
            } else if (rotYDelta > 0) {
                newRotY = (thisRotY + kREG(16)) + 0x96;
            } else {
                newRotY = (thisRotY - kREG(16)) - 0x96;
            }
            thisx->shape.rot.y = newRotY;
            thisx->world.rot.y = newRotY;
        }
    }
}

s32 DemoGo_FrameUpdateMatrix(DemoGo* self) {
    return SkelAnime_Update(&self->skelAnime);
}

s32 func_8097CDB0(DemoGo* self, GlobalContext* globalCtx, u16 npcAction) {
    CutsceneContext* csCtx = &globalCtx->csCtx;
    s32 actionIdx = func_8097C870(self);

    if ((csCtx->state != CS_STATE_IDLE) && (csCtx->npcActions[actionIdx] != NULL) &&
        (csCtx->npcActions[actionIdx]->action == npcAction)) {
        return 1;
    }
    return 0;
}

void func_8097CE10(DemoGo* self, GlobalContext* globalCtx) {
    self->action = 1;
}

void func_8097CE20(DemoGo* self, GlobalContext* globalCtx) {
    if (func_8097CDB0(self, globalCtx, 2)) {
        self->action = 2;
        self->drawConfig = 1;
        func_8097CB0C(self, globalCtx);
        func_8097C8A8(self, globalCtx);
    }
}

void func_8097CE78(DemoGo* self, GlobalContext* globalCtx) {
    CutsceneContext* csCtx = &globalCtx->csCtx;
    CsCmdActorAction* npcAction;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        npcAction = csCtx->npcActions[func_8097C870(self)];
        if (npcAction != NULL && csCtx->frames >= npcAction->endFrame) {
            func_8097CA78(self, globalCtx);
            self->action = 3;
        }
    }
}

void func_8097CEEC(DemoGo* self, GlobalContext* globalCtx) {
    if (func_8097CDB0(self, globalCtx, 3)) {
        self->action = 4;
    }
}

void func_8097CF20(DemoGo* self, GlobalContext* globalCtx, s32 arg2) {
    AnimationHeader* animation = &gGoronAnim_0029A8;
    if (arg2 != 0) {
        Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_LOOP,
                         -8.0f);
        self->action = 5;
        self->unk_19C = 0.0f;
    }
}

void func_8097CF9C(DemoGo* self) {
    self->unk_19C += 1.0f;
    if (self->unk_19C >= 8.0f) {
        self->action = 6;
    }
}

void func_8097CFDC(DemoGo* self, GlobalContext* globalCtx) {
    func_8097CE10(self, globalCtx);
}

void func_8097CFFC(DemoGo* self, GlobalContext* globalCtx) {
    func_8097CE20(self, globalCtx);
}

void func_8097D01C(DemoGo* self, GlobalContext* globalCtx) {
    func_8097CB0C(self, globalCtx);
    func_8097CA30(self, globalCtx);
    func_8097CE78(self, globalCtx);
}

void func_8097D058(DemoGo* self, GlobalContext* globalCtx) {
    func_8097CA30(self, globalCtx);
    func_8097CEEC(self, globalCtx);
}

void func_8097D088(DemoGo* self, GlobalContext* globalCtx) {
    s32 something;

    func_8097CA30(self, globalCtx);
    something = DemoGo_FrameUpdateMatrix(self);
    func_8097C930(self);
    func_8097CF20(self, globalCtx, something);
}

void func_8097D0D0(DemoGo* self, GlobalContext* globalCtx) {
    func_8097CCE0(self, globalCtx);
    func_8097CCC0(self);
    func_8097CA30(self, globalCtx);
    DemoGo_FrameUpdateMatrix(self);
    func_8097C930(self);
    func_8097C9DC(self);
    func_8097CF9C(self);
}
void func_8097D130(DemoGo* self, GlobalContext* globalCtx) {
    func_8097CCE0(self, globalCtx);
    func_8097CC08(self);
    func_8097CA30(self, globalCtx);
    DemoGo_FrameUpdateMatrix(self);
    func_8097C930(self);
    func_8097C9DC(self);
}

void DemoGo_Update(Actor* thisx, GlobalContext* globalCtx) {
    DemoGo* self = THIS;

    if (self->action < 0 || self->action >= 7 || D_8097D44C[self->action] == 0) {
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    D_8097D44C[self->action](self, globalCtx);
}

void DemoGo_Init(Actor* thisx, GlobalContext* globalCtx) {
    DemoGo* self = THIS;
    AnimationHeader* animation = &gGoronAnim_004930;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGoronSkel, NULL, NULL, NULL, 0);
    Animation_Change(&self->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_ONCE, 0.0f);
    self->action = 0;
}

void func_8097D290(DemoGo* self, GlobalContext* globalCtx) {
}

void func_8097D29C(DemoGo* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 eyeTexIdx = self->unk_190;
    SkelAnime* skelAnime = &self->skelAnime;
    void* eyeTexture = sEyeTextures[eyeTexIdx];
    void* mouthTexture = &gGoronCsMouthSmileTex;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_go.c", 732);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTexture));

    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL, NULL,
                          self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_go.c", 746);
}

void DemoGo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    DemoGo* self = THIS;

    if (self->drawConfig < 0 || self->drawConfig >= 2 || D_8097D468[self->drawConfig] == NULL) {
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    D_8097D468[self->drawConfig](self, globalCtx);
}
