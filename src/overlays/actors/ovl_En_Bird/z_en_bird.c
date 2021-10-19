/*
 * File: z_en_bird.c
 * Overlay: ovl_En_Bird
 * Description: An unused brown bird
 */

#include "z_en_bird.h"
#include "objects/object_bird/object_bird.h"

#define FLAGS 0x00000000

#define THIS ((EnBird*)thisx)

void EnBird_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809C1E00(EnBird* self, s16 params);
void func_809C1E40(EnBird* self, GlobalContext* globalCtx);
void func_809C1D60(EnBird* self, GlobalContext* globalCtx);
void func_809C1CAC(EnBird* self, s16 params);

const ActorInit En_Bird_InitVars = {
    ACTOR_EN_BIRD,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BIRD,
    sizeof(EnBird),
    (ActorFunc)EnBird_Init,
    (ActorFunc)EnBird_Destroy,
    (ActorFunc)EnBird_Update,
    (ActorFunc)EnBird_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 5600, ICHAIN_STOP),
};

void EnBird_SetupAction(EnBird* self, EnBirdActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnBird_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBird* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Actor_SetScale(&self->actor, 0.01);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gBirdSkel, &gBirdFlyAnim, NULL, NULL, 0);
    ActorShape_Init(&self->actor.shape, 5500, ActorShadow_DrawCircle, 4);
    self->unk_194 = 0;
    self->unk_198 = 0;
    self->unk_1C0 = 0x9C4;
    self->actor.colChkInfo.mass = 0;
    self->unk_1A8 = 1.5f;
    self->unk_1AC = 0.5f;
    self->unk_1A0 = 0.0f;
    self->unk_1A4 = 0.0f;
    self->unk_1B8 = 0.0f;
    self->unk_1B0 = 40.0f;
    self->unk_1BC = 70.0f;
    func_809C1CAC(self, self->actor.params);
}

void EnBird_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void func_809C1CAC(EnBird* self, s16 params) {
    f32 frameCount = Animation_GetLastFrame(&gBirdFlyAnim);
    f32 playbackSpeed = self->unk_19C ? 0.0f : 1.0f;
    AnimationHeader* anim = &gBirdFlyAnim;

    self->unk_198 = Rand_S16Offset(5, 0x23);
    Animation_Change(&self->skelAnime, anim, playbackSpeed, 0.0f, frameCount, ANIMMODE_LOOP, 0.0f);
    EnBird_SetupAction(self, func_809C1D60);
}

void func_809C1D60(EnBird* self, GlobalContext* globalCtx) {
    f32 fVar2 = sinf(self->unk_1B4);

    self->actor.shape.yOffset = self->actor.shape.yOffset + fVar2 * self->unk_1A0;
    Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.1f, 0.5f, 0.0f);

    if (self->unk_19C != 0) {
        self->skelAnime.playSpeed = self->actor.speedXZ + self->actor.speedXZ;
    }

    SkelAnime_Update(&self->skelAnime);
    self->unk_198 -= 1;

    if (self->unk_198 <= 0) {
        func_809C1E00(self, self->actor.params);
    }
}

void func_809C1E00(EnBird* self, s16 params) {
    self->unk_198 = Rand_S16Offset(0x14, 0x2D);
    EnBird_SetupAction(self, func_809C1E40);
}

void func_809C1E40(EnBird* self, GlobalContext* globalCtx) {
    f32 fVar4 = sinf(self->unk_1B4);
    self->actor.shape.yOffset += fVar4 * self->unk_1A0;
    Math_SmoothStepToF(&self->actor.speedXZ, self->unk_1A8, 0.1f, self->unk_1AC, 0.0f);

    if (self->unk_1B0 < Math_Vec3f_DistXZ(&self->actor.world.pos, &self->actor.home.pos) || self->unk_198 < 4) {
        Math_StepToAngleS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos),
                          self->unk_1C0);
    } else {
        fVar4 = sinf(self->unk_1B4);
        self->actor.world.rot.y += (s16)(fVar4 * self->unk_1A4);
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
    SkelAnime_Update(&self->skelAnime);
    self->unk_198 -= 1;
    if (self->unk_198 < 0) {
        func_809C1CAC(self, self->actor.params);
    }
}

void EnBird_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBird* self = THIS;

    self->unk_1B4 += self->unk_1B8;
    self->actionFunc(self, globalCtx);
}

void EnBird_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBird* self = THIS;

    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, NULL, NULL);
}
