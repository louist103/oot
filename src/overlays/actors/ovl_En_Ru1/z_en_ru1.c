/*
 * File: z_en_ru1.c
 * Overlay: En_Ru1
 * Description: Ruto (child)
 */

#include "z_en_ru1.h"
#include "objects/object_ru1/object_ru1.h"
#include "vt.h"

#define FLAGS 0x04000011

#define THIS ((EnRu1*)thisx)

void EnRu1_Init(Actor* thisx, GlobalContext* globalCtx);
void EnRu1_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnRu1_Update(Actor* thisx, GlobalContext* globalCtx);
void EnRu1_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AEC0B4(EnRu1* self, GlobalContext* globalCtx);
void func_80AEC100(EnRu1* self, GlobalContext* globalCtx);
void func_80AEC130(EnRu1* self, GlobalContext* globalCtx);
void func_80AEC17C(EnRu1* self, GlobalContext* globalCtx);
void func_80AEC1D4(EnRu1* self, GlobalContext* globalCtx);
void func_80AEC244(EnRu1* self, GlobalContext* globalCtx);
void func_80AEC2C0(EnRu1* self, GlobalContext* globalCtx);
void func_80AECA94(EnRu1* self, GlobalContext* globalCtx);
void func_80AECAB4(EnRu1* self, GlobalContext* globalCtx);
void func_80AECAD4(EnRu1* self, GlobalContext* globalCtx);
void func_80AECB18(EnRu1* self, GlobalContext* globalCtx);
void func_80AECB60(EnRu1* self, GlobalContext* globalCtx);
void func_80AECBB8(EnRu1* self, GlobalContext* globalCtx);
void func_80AECC1C(EnRu1* self, GlobalContext* globalCtx);
void func_80AECC84(EnRu1* self, GlobalContext* globalCtx);
void func_80AED304(EnRu1* self, GlobalContext* globalCtx);
void func_80AED324(EnRu1* self, GlobalContext* globalCtx);
void func_80AED344(EnRu1* self, GlobalContext* globalCtx);
void func_80AED374(EnRu1* self, GlobalContext* globalCtx);
void func_80AED3A4(EnRu1* self, GlobalContext* globalCtx);
void func_80AED3E0(EnRu1* self, GlobalContext* globalCtx);
void func_80AED414(EnRu1* self, GlobalContext* globalCtx);
void func_80AEF29C(EnRu1* self, GlobalContext* globalCtx);
void func_80AEF2AC(EnRu1* self, GlobalContext* globalCtx);
void func_80AEF2D0(EnRu1* self, GlobalContext* globalCtx);
void func_80AEF354(EnRu1* self, GlobalContext* globalCtx);
void func_80AEF3A8(EnRu1* self, GlobalContext* globalCtx);
void func_80AEEBD4(EnRu1* self, GlobalContext* globalCtx);
void func_80AEEC5C(EnRu1* self, GlobalContext* globalCtx);
void func_80AEECF0(EnRu1* self, GlobalContext* globalCtx);
void func_80AEED58(EnRu1* self, GlobalContext* globalCtx);
void func_80AEEDCC(EnRu1* self, GlobalContext* globalCtx);
void func_80AEEE34(EnRu1* self, GlobalContext* globalCtx);
void func_80AEEE9C(EnRu1* self, GlobalContext* globalCtx);
void func_80AEEF08(EnRu1* self, GlobalContext* globalCtx);
void func_80AEEF5C(EnRu1* self, GlobalContext* globalCtx);
void func_80AEF9D8(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFA2C(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFAAC(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFB04(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFB68(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFCE8(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFBC8(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFC24(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFECC(EnRu1* self, GlobalContext* globalCtx);
void func_80AEFF40(EnRu1* self, GlobalContext* globalCtx);

void func_80AF0278(EnRu1* self, GlobalContext* globalCtx, s32 limbIndex, Vec3s* rot);

void EnRu1_DrawNothing(EnRu1* self, GlobalContext* globalCtx);
void EnRu1_DrawOpa(EnRu1* self, GlobalContext* globalCtx);
void EnRu1_DrawXlu(EnRu1* self, GlobalContext* globalCtx);

static ColliderCylinderInitType1 sCylinderInit1 = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    { 0x00, { 0x00000000, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, 0x00, 0x00, 0x01 },
    { 25, 80, 0, { 0 } },
};

static ColliderCylinderInitType1 sCylinderInit2 = {
    {
        COLTYPE_HIT0,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    { 0x00, { 0x00000101, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, 0x01, 0x00, 0x01 },
    { 20, 30, 0, { 0 } },
};

static void* sEyeTextures[] = {
    gRutoChildEyeOpenTex,     gRutoChildEyeHalfTex,  gRutoChildEyeClosedTex,
    gRutoChildEyeRollLeftTex, gRutoChildEyeHalf2Tex, gRutoChildEyeHalfWithBlushTex,
};

static void* sMouthTextures[] = {
    gRutoChildMouthClosedTex,
    gRutoChildMouthFrownTex,
    gRutoChildMouthOpenTex,
};

static s32 sUnused = 0;

#include "z_en_ru1_cutscene_data.c" EARLY

static u32 D_80AF1938 = 0;

static EnRu1ActionFunc sActionFuncs[] = {
    func_80AEC0B4, func_80AEC100, func_80AEC130, func_80AEC17C, func_80AEC1D4, func_80AEC244, func_80AEC2C0,
    func_80AECA94, func_80AECAB4, func_80AECAD4, func_80AECB18, func_80AECB60, func_80AECBB8, func_80AECC1C,
    func_80AECC84, func_80AED304, func_80AED324, func_80AED344, func_80AED374, func_80AED3A4, func_80AED3E0,
    func_80AED414, func_80AEF29C, func_80AEF2AC, func_80AEF2D0, func_80AEF354, func_80AEF3A8, func_80AEEBD4,
    func_80AEEC5C, func_80AEECF0, func_80AEED58, func_80AEEDCC, func_80AEEE34, func_80AEEE9C, func_80AEEF08,
    func_80AEEF5C, func_80AEF9D8, func_80AEFA2C, func_80AEFAAC, func_80AEFB04, func_80AEFB68, func_80AEFCE8,
    func_80AEFBC8, func_80AEFC24, func_80AEFECC, func_80AEFF40,
};

static EnRu1PreLimbDrawFunc sPreLimbDrawFuncs[] = {
    func_80AF0278,
};

static Vec3f sMultVec = { 0.0f, 10.0f, 0.0f };

static EnRu1DrawFunc sDrawFuncs[] = {
    EnRu1_DrawNothing,
    EnRu1_DrawOpa,
    EnRu1_DrawXlu,
};

const ActorInit En_Ru1_InitVars = {
    ACTOR_EN_RU1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RU1,
    sizeof(EnRu1),
    (ActorFunc)EnRu1_Init,
    (ActorFunc)EnRu1_Destroy,
    (ActorFunc)EnRu1_Update,
    (ActorFunc)EnRu1_Draw,
};

void func_80AEAC10(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[5];

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void func_80AEAC54(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[5];

    Collider_UpdateCylinder(&self->actor, &self->collider2);
    if (self->unk_34C != 0) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider2.base);
    } else if (self->actor.xzDistToPlayer > 32.0f) {
        self->unk_34C = 1;
    }
}

void func_80AEACDC(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[5];

    Collider_UpdateCylinder(&self->actor, &self->collider2);
    CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider2.base);
}

void func_80AEAD20(Actor* thisx, GlobalContext* globalCtx) {
    EnRu1* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit1);

    Collider_InitCylinder(globalCtx, &self->collider2);
    Collider_SetCylinderType1(globalCtx, &self->collider2, &self->actor, &sCylinderInit2);
}

void EnRu1_DestroyColliders(EnRu1* self, GlobalContext* globalCtx) {
    Collider_DestroyCylinder(globalCtx, &self->collider);
    Collider_DestroyCylinder(globalCtx, &self->collider2);
}

void func_80AEADD8(EnRu1* self) {
    self->unk_34C = 0;
}

u8 func_80AEADE0(EnRu1* self) {
    u8 params = self->actor.params >> 8;

    return params;
}

u8 func_80AEADF0(EnRu1* self) {
    s16 params = self->actor.params;

    return params;
}

void EnRu1_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnRu1* self = THIS;

    EnRu1_DestroyColliders(self, globalCtx);
}

void EnRu1_UpdateEyes(EnRu1* self) {
    s32 pad[3];
    s16* blinkTimer = &self->blinkTimer;
    s16* eyeIndex = &self->eyeIndex;

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset(60, 60);
    }

    *eyeIndex = *blinkTimer;
    if (*eyeIndex >= 3) {
        *eyeIndex = 0;
    }
}

void EnRu1_SetEyeIndex(EnRu1* self, s16 eyeIndex) {
    self->eyeIndex = eyeIndex;
}

void EnRu1_SetMouthIndex(EnRu1* self, s16 mouthIndex) {
    self->mouthIndex = mouthIndex;
}

void func_80AEAECC(EnRu1* self, GlobalContext* globalCtx) {
    f32* velocityY = &self->actor.velocity.y;
    f32 velocityYHeld = *velocityY;

    *velocityY = -4.0f;
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 19.0f, 25.0f, 30.0f, 7);
    *velocityY = velocityYHeld;
}

s32 EnRu1_IsCsStateIdle(GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        return true;
    }
    return false;
}

CsCmdActorAction* func_80AEAF58(GlobalContext* globalCtx, s32 npcActionIdx) {
    s32 pad[2];
    CsCmdActorAction* ret = NULL;

    if (!EnRu1_IsCsStateIdle(globalCtx)) {
        ret = globalCtx->csCtx.npcActions[npcActionIdx];
    }
    return ret;
}

s32 func_80AEAFA0(GlobalContext* globalCtx, u16 action, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction = func_80AEAF58(globalCtx, npcActionIdx);

    if ((csCmdNPCAction != NULL) && (csCmdNPCAction->action == action)) {
        return true;
    }
    return false;
}

s32 func_80AEAFE0(GlobalContext* globalCtx, u16 action, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction = func_80AEAF58(globalCtx, npcActionIdx);

    if ((csCmdNPCAction != NULL) && (csCmdNPCAction->action != action)) {
        return true;
    }
    return false;
}

s32 func_80AEB020(EnRu1* self, GlobalContext* globalCtx) {
    Actor* actorIt = globalCtx->actorCtx.actorLists[ACTORCAT_NPC].head;
    EnRu1* someEnRu1;

    while (actorIt != NULL) {
        if (actorIt->id == ACTOR_EN_RU1) {
            someEnRu1 = (EnRu1*)actorIt;
            if (someEnRu1 != self) {
                if ((someEnRu1->action == 31) || (someEnRu1->action == 32) || (someEnRu1->action == 24)) {
                    return true;
                }
            }
        }
        actorIt = actorIt->next;
    }
    return false;
}

BgBdanObjects* EnRu1_FindSwitch(GlobalContext* globalCtx) {
    Actor* actorIt = globalCtx->actorCtx.actorLists[ACTORCAT_BG].head;

    while (actorIt != NULL) {
        if (actorIt->id == ACTOR_BG_BDAN_OBJECTS && actorIt->params == 0) {
            return (BgBdanObjects*)actorIt;
        }
        actorIt = actorIt->next;
    }
    // "There is no stand"
    osSyncPrintf(VT_FGCOL(RED) "お立ち台が無い!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    return NULL;
}

void func_80AEB0EC(EnRu1* self, s32 cameraSetting) {
    if (self->unk_28C != NULL) {
        self->unk_28C->cameraSetting = cameraSetting;
    }
}

s32 func_80AEB104(EnRu1* self) {
    if (self->unk_28C != NULL) {
        return self->unk_28C->cameraSetting;
    } else {
        return 0;
    }
}

Actor* func_80AEB124(GlobalContext* globalCtx) {
    Actor* actorIt = globalCtx->actorCtx.actorLists[ACTORCAT_BOSS].head;

    while (actorIt != NULL) {
        if ((actorIt->id == ACTOR_DEMO_EFFECT) && ((actorIt->params & 0xFF) == 0x15)) {
            return actorIt;
        }
        actorIt = actorIt->next;
    }
    return NULL;
}

s32 func_80AEB174(GlobalContext* globalCtx) {
    return (func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx));
}

s32 func_80AEB1B4(GlobalContext* globalCtx) {
    return func_8010BDBC(&globalCtx->msgCtx) == 2;
}

void func_80AEB1D8(EnRu1* self) {
    self->action = 36;
    self->drawConfig = 0;
    self->actor.velocity.x = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.velocity.z = 0.0f;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = 0.0f;
    self->actor.minVelocityY = 0.0f;
    func_80AEB0EC(self, 0);
}

void func_80AEB220(EnRu1* self, GlobalContext* globalCtx) {
    if ((EnRu1_IsCsStateIdle(globalCtx)) && (self->actor.params == 0xA)) {
        func_80AEB1D8(self);
    }
}

void func_80AEB264(EnRu1* self, AnimationHeader* animation, u8 arg2, f32 transitionRate, s32 arg4) {
    s32 pad[2];
    AnimationHeader* animHeader = SEGMENTED_TO_VIRTUAL(animation);
    f32 frameCount = Animation_GetLastFrame(animHeader);
    f32 playbackSpeed;
    f32 unk0;
    f32 fc;

    if (arg4 == 0) {
        unk0 = 0.0f;
        fc = frameCount;
        playbackSpeed = 1.0f;
    } else {
        unk0 = frameCount;
        fc = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change(&self->skelAnime, animHeader, playbackSpeed, unk0, fc, arg2, transitionRate);
}

s32 EnRu1_FrameUpdateMatrix(EnRu1* self) {
    // why?
    if (self->action != 32) {
        return SkelAnime_Update(&self->skelAnime);
    } else {
        return SkelAnime_Update(&self->skelAnime);
    }
}

void func_80AEB364(EnRu1* self, GlobalContext* globalCtx) {
    self->skelAnime.moveFlags |= 1;
    AnimationContext_SetMoveActor(globalCtx, &self->actor, &self->skelAnime, 1.0f);
}

void func_80AEB3A4(EnRu1* self, GlobalContext* globalCtx) {
    self->skelAnime.moveFlags |= 1;
    func_80AEB364(self, globalCtx);
}

void func_80AEB3CC(EnRu1* self) {
    self->skelAnime.moveFlags &= ~0x1;
}

void func_80AEB3DC(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB264(self, &gRutoChildWaitHandsBehindBackAnim, 0, 0, 0);
    self->action = 0;
    self->drawConfig = 1;
    EnRu1_SetEyeIndex(self, 4);
    EnRu1_SetMouthIndex(self, 0);
}

CsCmdActorAction* func_80AEB438(GlobalContext* globalCtx) {
    return func_80AEAF58(globalCtx, 3);
}

s32 func_80AEB458(GlobalContext* globalCtx, u16 action) {
    return func_80AEAFA0(globalCtx, action, 3);
}

s32 func_80AEB480(GlobalContext* globalCtx, u16 action) {
    return func_80AEAFE0(globalCtx, action, 3);
}

void EnRu1_SpawnRipple(EnRu1* self, GlobalContext* globalCtx, s16 radiusMax, s16 life) {
    Vec3f pos;
    Actor* thisx = &self->actor;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + self->actor.yDistToWater;
    pos.z = self->actor.world.pos.z;
    EffectSsGRipple_Spawn(globalCtx, &pos, 100, radiusMax, life);
}

void func_80AEB50C(EnRu1* self, GlobalContext* globalCtx) {
    self->unk_270 += 1.0f;
    if (self->unk_270 >= kREG(3) + 10.0f) {
        EnRu1_SpawnRipple(self, globalCtx, kREG(1) + 500, 0);
        self->unk_270 = 0.0f;
    }
}

void func_80AEB59C(EnRu1* self, GlobalContext* globalCtx) {
    EnRu1_SpawnRipple(self, globalCtx, kREG(2) + 500, 0);
    EnRu1_SpawnRipple(self, globalCtx, kREG(2) + 500, kREG(3) + 10.0f);
    EnRu1_SpawnRipple(self, globalCtx, kREG(2) + 500, (kREG(3) + 10.0f) * 2.0f);
}

void EnRu1_SpawnSplash(EnRu1* self, GlobalContext* globalCtx) {
    Vec3f pos;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + self->actor.yDistToWater;
    pos.z = self->actor.world.pos.z;

    EffectSsGSplash_Spawn(globalCtx, &pos, 0, 0, 1, 0);
}

void func_80AEB6E0(EnRu1* self, GlobalContext* globalCtx) {
    SkelAnime* skelAnime = &self->skelAnime;

    if (skelAnime->baseTransl.y < skelAnime->jointTable[0].y) {
        skelAnime->moveFlags |= 3;
        AnimationContext_SetMoveActor(globalCtx, &self->actor, skelAnime, 1.0f);
    }
}

void func_80AEB738(EnRu1* self, GlobalContext* globalCtx) {
    SkelAnime* skelAnime = &self->skelAnime;

    skelAnime->baseTransl = skelAnime->jointTable[0];
    skelAnime->prevTransl = skelAnime->jointTable[0];
    if (skelAnime->baseTransl.y < skelAnime->jointTable[0].y) {
        skelAnime->moveFlags |= 3;
        AnimationContext_SetMoveActor(globalCtx, &self->actor, skelAnime, 1.0f);
    }
}

void func_80AEB7D0(EnRu1* self) {
    self->skelAnime.moveFlags &= ~0x3;
}

f32 func_80AEB7E0(CsCmdActorAction* csCmdNPCAction, GlobalContext* globalCtx) {
    s32 csCtxFrames = globalCtx->csCtx.frames;
    if ((csCtxFrames < csCmdNPCAction->endFrame) && (csCmdNPCAction->endFrame - csCmdNPCAction->startFrame > 0)) {
        return (Math_CosS(((csCtxFrames - csCmdNPCAction->startFrame) /
                           (f32)(csCmdNPCAction->endFrame - csCmdNPCAction->startFrame)) *
                          32768.0f) *
                -0.5f) +
               0.5f;
    }
    return 1.0f;
}

f32 func_80AEB87C(f32 arg0, s32 arg1, s32 arg2) {
    return (((f32)arg2 - arg1) * arg0) + arg1;
}

void func_80AEB89C(EnRu1* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = func_80AEB438(globalCtx);
    s16 npcActionRotY;

    if (npcAction != NULL) {
        npcActionRotY = npcAction->rot.y;
        self->actor.shape.rot.y = npcActionRotY;
        self->actor.world.rot.y = npcActionRotY;
        self->actor.world.pos.x = npcAction->startPos.x;
        self->actor.world.pos.y = npcAction->startPos.y;
        self->actor.world.pos.z = npcAction->startPos.z;
    }
}

void func_80AEB914(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB89C(self, globalCtx);
}

void func_80AEB934(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB89C(self, globalCtx);
}

void func_80AEB954(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB6E0(self, globalCtx);
}

void func_80AEB974(EnRu1* self, GlobalContext* globalCtx) {
    Vec3f* thisPos;
    f32 sp30;
    CsCmdActorAction* csCmdNPCAction = func_80AEB438(globalCtx);
    s32 pad;

    if (csCmdNPCAction != NULL) {
        sp30 = func_80AEB7E0(csCmdNPCAction, globalCtx);
        thisPos = &self->actor.world.pos;
        thisPos->x = func_80AEB87C(sp30, csCmdNPCAction->startPos.x, csCmdNPCAction->endPos.x);
        thisPos->y = func_80AEB87C(sp30, csCmdNPCAction->startPos.y, csCmdNPCAction->endPos.y);
        thisPos->z = func_80AEB87C(sp30, csCmdNPCAction->startPos.z, csCmdNPCAction->endPos.z);
    }
}

void func_80AEBA0C(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB6E0(self, globalCtx);
}

void func_80AEBA2C(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f* unk_364 = &self->unk_364;
    Vec3f* thisPos;
    f32 temp_ret_2;
    CsCmdActorAction* csCmdNPCAction = func_80AEB438(globalCtx);
    s32 pad2;
    if (csCmdNPCAction != NULL) {
        temp_ret_2 = func_80AEB7E0(csCmdNPCAction, globalCtx);
        thisPos = &self->actor.world.pos;
        thisPos->x = func_80AEB87C(temp_ret_2, unk_364->x, csCmdNPCAction->endPos.x);
        thisPos->y = func_80AEB87C(temp_ret_2, unk_364->y, csCmdNPCAction->endPos.y);
        thisPos->z = func_80AEB87C(temp_ret_2, unk_364->z, csCmdNPCAction->endPos.z);
    }
}

void func_80AEBAFC(EnRu1* self) {
    if (self->unk_298 == 0) {
        func_80078914(&self->actor.projectedPos, NA_SE_EV_DIVE_INTO_WATER);
        self->unk_298 = 1;
    }
}

void func_80AEBB3C(EnRu1* self) {
    if (Animation_OnFrame(&self->skelAnime, 5.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_FACE_UP);
    }
}

void func_80AEBB78(EnRu1* self) {
    SkelAnime* skelAnime = &self->skelAnime;

    if (Animation_OnFrame(skelAnime, 4.0f) || Animation_OnFrame(skelAnime, 13.0f) ||
        Animation_OnFrame(skelAnime, 22.0f) || Animation_OnFrame(skelAnime, 31.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_SWIM);
    }
}

void func_80AEBBF4(EnRu1* self) {
    if (Animation_OnFrame(&self->skelAnime, 8.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_SUBMERGE);
    }
}

void func_80AEBC30(GlobalContext* globalCtx) {
    Player* player;

    if (globalCtx->csCtx.frames == 0xCD) {
        player = GET_PLAYER(globalCtx);
        Audio_PlaySoundGeneral(NA_SE_EV_DIVE_INTO_WATER, &player->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

void func_80AEBC84(EnRu1* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.frames == 0x82) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_LAUGH_0);
    }
}

void func_80AEBCB8(EnRu1* self, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&self->skelAnime, &gRutoChildSwimOnBackAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildSwimOnBackAnim), ANIMMODE_LOOP, -8.0f);
    }
}

void func_80AEBD1C(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEB480(globalCtx, 2)) {
        self->action = 1;
        self->drawConfig = 0;
        func_80AEB914(self, globalCtx);
        func_80AEAECC(self, globalCtx);
        EnRu1_SpawnSplash(self, globalCtx);
        func_80AEB59C(self, globalCtx);
    }
}

void func_80AEBD94(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[2];
    f32 frameCount;

    if (func_80AEB480(globalCtx, 3)) {
        frameCount = Animation_GetLastFrame(&gRutoChildAnim_009060);
        func_80AEB934(self, globalCtx);
        func_80AEB738(self, globalCtx);
        Animation_Change(&self->skelAnime, &gRutoChildAnim_009060, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, 0.0f);
        self->action = 2;
        self->drawConfig = 1;
    }
}

void func_80AEBE3C(EnRu1* self, GlobalContext* globalCtx, s32 arg2) {
    s32 pad[2];
    f32 frameCount;

    if (arg2 != 0) {
        frameCount = Animation_GetLastFrame(&gRutoChildTreadWaterAnim);
        func_80AEB7D0(self);
        Animation_Change(&self->skelAnime, &gRutoChildTreadWaterAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
        self->action = 3;
    } else {
        func_80AEB954(self, globalCtx);
    }
}

void func_80AEBEC8(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[2];
    f32 frameCount;

    if (func_80AEB458(globalCtx, 6)) {
        frameCount = Animation_GetLastFrame(&gRutoChildTransitionToSwimOnBackAnim);
        func_80AEB738(self, globalCtx);
        Animation_Change(&self->skelAnime, &gRutoChildTransitionToSwimOnBackAnim, 1.0f, 0, frameCount, ANIMMODE_ONCE,
                         -8.0f);
        self->action = 4;
    }
}

void func_80AEBF60(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEB480(globalCtx, 6)) {
        func_80AEB7D0(self);
        self->action = 5;
        self->unk_364 = self->actor.world.pos;
    } else {
        func_80AEBA0C(self, globalCtx);
    }
}

void func_80AEBFD8(EnRu1* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction = func_80AEB438(globalCtx);
    f32 frameCount;
    u16 csCtxFrames;
    u16 endFrame;

    if (csCmdNPCAction != NULL) {
        csCtxFrames = globalCtx->csCtx.frames;
        endFrame = csCmdNPCAction->endFrame;
        if (csCtxFrames >= endFrame - 2) {
            frameCount = Animation_GetLastFrame(&gRutoChildTransitionFromSwimOnBackAnim);
            Animation_Change(&self->skelAnime, &gRutoChildTransitionFromSwimOnBackAnim, 1.0, 0, frameCount,
                             ANIMMODE_ONCE, -8.0f);
            self->action = 6;
        }
    }
}

void func_80AEC070(EnRu1* self, GlobalContext* globalCtx, UNK_TYPE arg2) {
    if ((func_80AEB458(globalCtx, 8)) && (arg2 != 0)) {
        Actor_Kill(&self->actor);
    }
}

void func_80AEC0B4(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB89C(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    func_80AEBC84(self, globalCtx);
    func_80AEBC30(globalCtx);
    func_80AEBD1C(self, globalCtx);
}

void func_80AEC100(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEBAFC(self);
    func_80AEBD94(self, globalCtx);
}

void func_80AEC130(EnRu1* self, GlobalContext* globalCtx) {
    s32 something = EnRu1_FrameUpdateMatrix(self);

    func_80AEAECC(self, globalCtx);
    func_80AEBB3C(self);
    func_80AEBE3C(self, globalCtx, something);
}

void func_80AEC17C(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB974(self, globalCtx);
    func_80AEAECC(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    func_80AEB50C(self, globalCtx);
    func_80AEBEC8(self, globalCtx);
}

void func_80AEC1D4(EnRu1* self, GlobalContext* globalCtx) {
    s32 something;

    something = EnRu1_FrameUpdateMatrix(self);
    func_80AEAECC(self, globalCtx);
    EnRu1_UpdateEyes(self);
    func_80AEB50C(self, globalCtx);
    func_80AEBCB8(self, something);
    func_80AEBBF4(self);
    func_80AEBF60(self, globalCtx);
}

void func_80AEC244(EnRu1* self, GlobalContext* globalCtx) {
    s32 something;

    something = EnRu1_FrameUpdateMatrix(self);
    func_80AEBA2C(self, globalCtx);
    func_80AEAECC(self, globalCtx);
    EnRu1_UpdateEyes(self);
    func_80AEB50C(self, globalCtx);
    func_80AEBCB8(self, something);
    func_80AEBB78(self);
    func_80AEBFD8(self, globalCtx);
}

void func_80AEC2C0(EnRu1* self, GlobalContext* globalCtx) {
    s32 something;

    something = EnRu1_FrameUpdateMatrix(self);
    func_80AEAECC(self, globalCtx);
    EnRu1_UpdateEyes(self);
    func_80AEB50C(self, globalCtx);
    func_80AEC070(self, globalCtx, something);
}

void func_80AEC320(EnRu1* self, GlobalContext* globalCtx) {
    s8 actorRoom;

    if (!(gSaveContext.infTable[20] & 2)) {
        func_80AEB264(self, &gRutoChildWait2Anim, 0, 0, 0);
        self->action = 7;
        EnRu1_SetMouthIndex(self, 1);
    } else if ((gSaveContext.infTable[20] & 0x80) && !(gSaveContext.infTable[20] & 1) &&
               !(gSaveContext.infTable[20] & 0x20)) {
        if (!func_80AEB020(self, globalCtx)) {
            func_80AEB264(self, &gRutoChildWait2Anim, 0, 0, 0);
            actorRoom = self->actor.room;
            self->action = 22;
            self->actor.room = -1;
            self->drawConfig = 0;
            self->roomNum1 = actorRoom;
            self->roomNum3 = actorRoom;
            self->roomNum2 = actorRoom;
        } else {
            Actor_Kill(&self->actor);
        }
    } else {
        Actor_Kill(&self->actor);
    }
}

void func_80AEC40C(EnRu1* self) {
    f32 unk_26C = self->unk_26C;

    if (unk_26C < 8.0f) {
        self->actor.speedXZ = (((kREG(3) * 0.01f) + 2.7f) / 8.0f) * unk_26C;
    } else {
        self->actor.speedXZ = (kREG(3) * 0.01f) + 2.7f;
    }
    self->actor.velocity.y = -1.0f;
    Actor_MoveForward(&self->actor);
}

void func_80AEC4CC(EnRu1* self) {
    self->actor.velocity.y = -1.0f;
    Actor_MoveForward(&self->actor);
}

void func_80AEC4F4(EnRu1* self) {
    f32* speedXZ = &self->actor.speedXZ;
    f32* unk_26C = &self->unk_26C;

    if (self->unk_26C < 8.0f) {
        *unk_26C += 1.0f;
        *speedXZ *= (8.0f - *unk_26C) / 8.0f;
        self->actor.velocity.y = -*unk_26C * (((kREG(4) * 0.01f) + 13.0f) / 8.0f);
    } else {
        *speedXZ = 0.0f;
        self->actor.velocity.y = -((kREG(4) * 0.01f) + 13.0f);
    }
    Actor_MoveForward(&self->actor);
}

s32 func_80AEC5FC(EnRu1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 thisPosZ = self->actor.world.pos.z;
    f32 playerPosZ = player->actor.world.pos.z;

    if ((playerPosZ - thisPosZ <= 265.0f) && (player->actor.world.pos.y >= self->actor.world.pos.y)) {
        return true;
    }
    return false;
}

void func_80AEC650(EnRu1* self) {
    s32 pad[2];

    if (self->unk_280 == 0) {
        if (Animation_OnFrame(&self->skelAnime, 2.0f) || Animation_OnFrame(&self->skelAnime, 7.0f)) {
            func_80078914(&self->actor.projectedPos, NA_SE_PL_WALK_DIRT);
        }
    }
}

void func_80AEC6B0(EnRu1* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_EV_FALL_DOWN_DIRT);
    func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_FALL);
}

void func_80AEC6E4(EnRu1* self, GlobalContext* globalCtx) {
    if ((func_80AEAFA0(globalCtx, 4, 3)) && (self->unk_280 == 0)) {
        Animation_Change(&self->skelAnime, &gRutoChildBringArmsUpAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildBringArmsUpAnim), ANIMMODE_ONCE, -8.0f);
        self->unk_280 = 1;
        func_80AEC6B0(self);
    }
}

void func_80AEC780(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    if ((func_80AEC5FC(self, globalCtx)) && (!Gameplay_InCsMode(globalCtx)) && (!(player->stateFlags1 & 0x206000)) &&
        (player->actor.bgCheckFlags & 1)) {

        globalCtx->csCtx.segment = &D_80AF0880;
        gSaveContext.cutsceneTrigger = 1;
        player->linearVelocity = 0.0f;
        self->action = 8;
    }
}

void func_80AEC81C(EnRu1* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction;
    s16 newRotY;

    if (func_80AEAFE0(globalCtx, 1, 3)) {
        csCmdNPCAction = globalCtx->csCtx.npcActions[3];
        self->actor.world.pos.x = csCmdNPCAction->startPos.x;
        self->actor.world.pos.y = csCmdNPCAction->startPos.y;
        self->actor.world.pos.z = csCmdNPCAction->startPos.z;
        newRotY = csCmdNPCAction->rot.y;
        self->actor.shape.rot.y = newRotY;
        self->actor.world.rot.y = newRotY;
        self->action = 9;
        self->drawConfig = 1;
    }
}

void func_80AEC8B8(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEAFA0(globalCtx, 3, 3)) {
        Animation_Change(&self->skelAnime, &gRutoChildTurnAroundAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildTurnAroundAnim), ANIMMODE_ONCE, -8.0f);
        self->action = 10;
    }
}

void func_80AEC93C(EnRu1* self, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&self->skelAnime, &gRutoChildWalkAnim, 1.0f, 0, Animation_GetLastFrame(&gRutoChildWalkAnim),
                         ANIMMODE_LOOP, -8.0f);
        self->actor.world.rot.y += 0x8000;
        self->action = 0xB;
        self->unk_26C = 0.0f;
    }
}

void func_80AEC9C4(EnRu1* self) {
    self->unk_26C += 1.0f;
    if (self->unk_26C >= 8.0f) {
        self->action = 12;
        self->unk_26C = 0.0f;
        self->actor.velocity.y = -1.0f;
    }
}

void func_80AECA18(EnRu1* self) {
    if (!(self->actor.bgCheckFlags & 1)) {
        self->action = 13;
        self->unk_26C = 0.0f;
        self->actor.velocity.y = 0.0f;
    }
}

void func_80AECA44(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEAFA0(globalCtx, 5, 3)) {
        gSaveContext.infTable[20] |= 2;
        self->action = 14;
    }
}

void func_80AECA94(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEC780(self, globalCtx);
}

void func_80AECAB4(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEC81C(self, globalCtx);
}

void func_80AECAD4(EnRu1* self, GlobalContext* globalCtx) {
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAECC(self, globalCtx);
    func_80AEC8B8(self, globalCtx);
}

void func_80AECB18(EnRu1* self, GlobalContext* globalCtx) {
    s32 something;

    something = EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAECC(self, globalCtx);
    func_80AEC93C(self, something);
}

void func_80AECB60(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEC40C(self);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAECC(self, globalCtx);
    func_80AEC650(self);
    func_80AEC9C4(self);
}

void func_80AECBB8(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEC4CC(self);
    func_80AEC6E4(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAECC(self, globalCtx);
    func_80AEC650(self);
    func_80AECA18(self);
}

void func_80AECC1C(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEC4F4(self);
    func_80AEC6E4(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAECC(self, globalCtx);
    func_80AEC650(self);
    func_80AECA44(self, globalCtx);
}

void func_80AECC84(EnRu1* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        Actor_Kill(&self->actor);
    }
}

void func_80AECCB0(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f* pos;
    s16 yawTowardsPlayer;
    f32 spawnX;
    f32 spawnY;
    f32 spawnZ;
    s32 pad2[2];

    yawTowardsPlayer = self->actor.yawTowardsPlayer;
    pos = &self->actor.world.pos;
    spawnX = ((kREG(1) + 12.0f) * Math_SinS(yawTowardsPlayer)) + pos->x;
    spawnY = pos->y;
    spawnZ = ((kREG(1) + 12.0f) * Math_CosS(yawTowardsPlayer)) + pos->z;
    self->blueWarp = (DoorWarp1*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1,
                                                    spawnX, spawnY, spawnZ, 0, yawTowardsPlayer, 0, WARP_BLUE_RUTO);
}

void func_80AECDA0(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB264(self, &gRutoChildWaitHandsOnHipsAnim, 0, 0, 0);
    self->action = 15;
    self->actor.shape.yOffset = -10000.0f;
    EnRu1_SetEyeIndex(self, 5);
    EnRu1_SetMouthIndex(self, 2);
}

void func_80AECE04(EnRu1* self, GlobalContext* globalCtx) {
    self->actor.shape.yOffset += (250.0f / 3.0f);
}

void func_80AECE20(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad2;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f* playerPos = &player->actor.world.pos;
    s16 shapeRotY = player->actor.shape.rot.y;
    s32 pad;
    f32 unk_27C = self->unk_27C;
    Vec3f* pos = &self->actor.world.pos;

    pos->x = (Math_SinS(shapeRotY) * unk_27C) + playerPos->x;
    pos->y = playerPos->y;
    pos->z = (Math_CosS(shapeRotY) * unk_27C) + playerPos->z;
}

void func_80AECEB4(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f* player_unk_450 = &player->unk_450;
    Vec3f* pos = &self->actor.world.pos;
    s16 shapeRotY = self->actor.shape.rot.y;

    player_unk_450->x = ((kREG(2) + 30.0f) * Math_SinS(shapeRotY)) + pos->x;
    player_unk_450->z = ((kREG(2) + 30.0f) * Math_CosS(shapeRotY)) + pos->z;
}

s32 func_80AECF6C(EnRu1* self, GlobalContext* globalCtx) {
    s16* shapeRotY;
    Player* player = GET_PLAYER(globalCtx);
    Player* otherPlayer;
    s16 temp_f16;
    f32 temp1;
    f32 temp2;
    s32 pad2[5];

    self->unk_26C += 1.0f;
    if ((player->actor.speedXZ == 0.0f) && (self->unk_26C >= 3.0f)) {
        otherPlayer = GET_PLAYER(globalCtx);
        player->actor.world.pos.x = otherPlayer->unk_450.x;
        player->actor.world.pos.y = otherPlayer->unk_450.y;
        player->actor.world.pos.z = otherPlayer->unk_450.z;
        shapeRotY = &player->actor.shape.rot.y;
        temp1 = self->actor.world.pos.x - player->actor.world.pos.x;
        temp2 = self->actor.world.pos.z - player->actor.world.pos.z;
        temp_f16 = Math_FAtan2F(temp1, temp2) * (0x8000 / M_PI);
        if (*shapeRotY != temp_f16) {
            Math_SmoothStepToS(shapeRotY, temp_f16, 0x14, 0x1838, 0x64);
            player->actor.world.rot.y = *shapeRotY;
        } else {
            return true;
        }
    }
    return false;
}

s32 func_80AED084(EnRu1* self, s32 state) {
    if (self->blueWarp != NULL && self->blueWarp->rutoWarpState == state) {
        return true;
    }
    return false;
}

void func_80AED0B0(EnRu1* self, s32 state) {
    if (self->blueWarp != NULL) {
        self->blueWarp->rutoWarpState = state;
    }
}

void func_80AED0C8(EnRu1* self, GlobalContext* globalCtx) {
    self->action = 16;
}

void func_80AED0D8(EnRu1* self, GlobalContext* globalCtx) {
    self->action = 17;
    self->drawConfig = 1;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    func_80AECCB0(self, globalCtx);
}

void func_80AED110(EnRu1* self) {
    if (self->actor.shape.yOffset >= 0.0f) {
        self->action = 18;
        self->actor.shape.yOffset = 0.0f;
        func_80AED0B0(self, WARP_BLUE_RUTO_STATE_READY);
    }
}

void func_80AED154(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AED084(self, WARP_BLUE_RUTO_STATE_ENTERED)) {
        self->action = 0x13;
        self->unk_26C = 0.0f;
        func_80AECEB4(self, globalCtx);
    }
}

void func_80AED19C(EnRu1* self, s32 cond) {
    if (cond) {
        Animation_Change(&self->skelAnime, &gRutoChildTransitionHandsOnHipToCrossArmsAndLegsAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildTransitionHandsOnHipToCrossArmsAndLegsAnim), ANIMMODE_ONCE,
                         -8.0f);
        self->action = 20;
        func_80AED0B0(self, WARP_BLUE_RUTO_STATE_3);
    }
}

void func_80AED218(EnRu1* self, UNK_TYPE arg1) {
    if (func_80AED084(self, WARP_BLUE_RUTO_STATE_TALKING)) {
        if (arg1 != 0) {
            Animation_Change(&self->skelAnime, &gRutoChildWaitSittingAnim, 1.0f, 0,
                             Animation_GetLastFrame(&gRutoChildWaitSittingAnim), ANIMMODE_LOOP, -8.0f);
        }
    } else if (func_80AED084(self, WARP_BLUE_RUTO_STATE_WARPING)) {
        Animation_Change(&self->skelAnime, &gRutoChildWaitInBlueWarpAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildWaitInBlueWarpAnim), ANIMMODE_ONCE, -8.0f);
        self->action = 21;
        self->unk_27C = self->actor.xzDistToPlayer;
    }
}

void func_80AED304(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED0C8(self, globalCtx);
}

void func_80AED324(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED0D8(self, globalCtx);
}

void func_80AED344(EnRu1* self, GlobalContext* globalCtx) {
    func_80AECE04(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    func_80AED110(self);
}

void func_80AED374(EnRu1* self, GlobalContext* globalCtx) {
    EnRu1_FrameUpdateMatrix(self);
    func_80AED154(self, globalCtx);
}

void func_80AED3A4(EnRu1* self, GlobalContext* globalCtx) {
    EnRu1_FrameUpdateMatrix(self);
    func_80AED19C(self, func_80AECF6C(self, globalCtx));
}

void func_80AED3E0(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEAECC(self, globalCtx);
    func_80AED218(self, EnRu1_FrameUpdateMatrix(self));
}

void func_80AED414(EnRu1* self, GlobalContext* globalCtx) {
    func_80AECE20(self, globalCtx);
    func_80AEAECC(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
}

void func_80AED44C(EnRu1* self, GlobalContext* globalCtx) {
    s8 actorRoom;

    if ((gSaveContext.infTable[20] & 2) && !(gSaveContext.infTable[20] & 0x20) && !(gSaveContext.infTable[20] & 1) &&
        !(gSaveContext.infTable[20] & 0x80)) {
        if (!func_80AEB020(self, globalCtx)) {
            func_80AEB264(self, &gRutoChildWait2Anim, 0, 0, 0);
            actorRoom = self->actor.room;
            self->action = 22;
            self->actor.room = -1;
            self->roomNum1 = actorRoom;
            self->roomNum3 = actorRoom;
            self->roomNum2 = actorRoom;
        } else {
            Actor_Kill(&self->actor);
        }
    } else {
        Actor_Kill(&self->actor);
    }
}

void func_80AED4FC(EnRu1* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_EV_LAND_DIRT);
}

void func_80AED520(EnRu1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Audio_PlaySoundGeneral(NA_SE_PL_PULL_UP_RUTO, &player->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                           &D_801333E8);
    func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_LIFT);
}

void func_80AED57C(EnRu1* self) {
    if (self->actor.speedXZ != 0.0f) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_THROW);
    }
}

void func_80AED5B8(EnRu1* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_CRASH);
}

void func_80AED5DC(EnRu1* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_UNBALLANCE);
}

void func_80AED600(EnRu1* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_DISCOVER);
}

s32 func_80AED624(EnRu1* self, GlobalContext* globalCtx) {
    s8 curRoomNum = globalCtx->roomCtx.curRoom.num;

    if (self->roomNum2 != curRoomNum) {
        Actor_Kill(&self->actor);
        return false;
    } else if (((self->roomNum1 != curRoomNum) || (self->roomNum2 != curRoomNum)) &&
               (self->actor.yDistToWater > kREG(16) + 50.0f) && (self->action != 33)) {
        self->action = 33;
        self->drawConfig = 2;
        self->alpha = 0xFF;
        self->unk_2A4 = 0.0f;
    }
    return true;
}

void func_80AED6DC(EnRu1* self, GlobalContext* globalCtx) {
    s8 curRoomNum = globalCtx->roomCtx.curRoom.num;

    self->roomNum2 = curRoomNum;
    self->unk_288 = 0.0f;
}

void func_80AED6F8(GlobalContext* globalCtx) {
    s8 curRoomNum;

    if ((!(gSaveContext.infTable[20] & 0x80))) {
        curRoomNum = globalCtx->roomCtx.curRoom.num;
        if (curRoomNum == 2) {
            gSaveContext.infTable[20] |= 0x80;
        }
    }
}

void func_80AED738(EnRu1* self, GlobalContext* globalCtx) {
    u32 temp_v0;

    if (func_80AED624(self, globalCtx)) {
        self->unk_2A4 += 1.0f;
        if (self->unk_2A4 < 20.0f) {
            temp_v0 = ((20.0f - self->unk_2A4) * 255.0f) / 20.0f;
            self->alpha = temp_v0;
            self->actor.shape.shadowAlpha = temp_v0;
        } else {
            Actor_Kill(&self->actor);
        }
    }
}

void func_80AED83C(EnRu1* self) {
    s32 pad[2];
    Vec3s* tempPtr;
    Vec3s* tempPtr2;

    tempPtr = &self->unk_374.unk_08;
    Math_SmoothStepToS(&tempPtr->x, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&tempPtr->y, 0, 0x14, 0x1838, 0x64);
    tempPtr2 = &self->unk_374.unk_0E;
    Math_SmoothStepToS(&tempPtr2->x, 0, 0x14, 0x1838, 0x64);
    Math_SmoothStepToS(&tempPtr2->y, 0, 0x14, 0x1838, 0x64);
}

void func_80AED8DC(EnRu1* self) {
    s32 temp_hi;
    s16* unk_2AC = &self->unk_2AC;
    s16* someY = &self->unk_374.unk_08.y;
    s16* unk_29E = &self->unk_29E;
    s32 pad[2];

    if (DECR(*unk_2AC) == 0) {
        *unk_2AC = Rand_S16Offset(0xA, 0x19);
        temp_hi = *unk_2AC % 5;
        if (temp_hi == 0) {
            self->unk_2B0 = 1;
        } else if (temp_hi == 1) {
            self->unk_2B0 = 2;
        } else {
            self->unk_2B0 = 0;
        }
        *unk_29E = 0;
    }

    if (self->unk_2B0 == 0) {
        Math_SmoothStepToS(unk_29E, 0 - *someY, 1, 0x190, 0x190);
        Math_SmoothStepToS(someY, 0, 3, ABS(*unk_29E), 0x64);
    } else if (self->unk_2B0 == 1) {
        Math_SmoothStepToS(unk_29E, -0x2AAA - *someY, 1, 0x190, 0x190);
        Math_SmoothStepToS(someY, -0x2AAA, 3, ABS(*unk_29E), 0x64);
    } else {
        Math_SmoothStepToS(unk_29E, 0x2AAA - *someY, 1, 0x190, 0x190);
        Math_SmoothStepToS(someY, 0x2AAA, 3, ABS(*unk_29E), 0x64);
    }
}

void func_80AEDAE0(EnRu1* self, GlobalContext* globalCtx) {
    DynaPolyActor* dynaPolyActor = DynaPoly_GetActor(&globalCtx->colCtx, self->actor.floorBgId);

    if (dynaPolyActor == NULL || dynaPolyActor->actor.id == ACTOR_EN_BOX) {
        self->actor.bgCheckFlags &= ~0x19;
    }
}

void func_80AEDB30(EnRu1* self, GlobalContext* globalCtx) {
    DynaPolyActor* dynaPolyActor;
    f32* velocityY;
    f32* speedXZ;
    f32* gravity;
    s16 wallYaw;
    s16 rotY;
    s32 temp_a1_2;
    s32 temp_a0;
    s32 phi_v1;

    if (self->actor.bgCheckFlags & 1) {
        velocityY = &self->actor.velocity.y;
        dynaPolyActor = DynaPoly_GetActor(&globalCtx->colCtx, self->actor.floorBgId);
        if (*velocityY <= 0.0f) {
            speedXZ = &self->actor.speedXZ;
            if (dynaPolyActor != NULL) {
                if (dynaPolyActor->actor.id != ACTOR_EN_BOX) {
                    *speedXZ = 0.0f;
                }
            } else {
                if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
                    *speedXZ *= (kREG(19) * 0.01f) + 0.8f;
                } else {
                    *speedXZ = 0.0f;
                }
            }
            gravity = &self->actor.gravity;
            if (dynaPolyActor != NULL) {
                if (dynaPolyActor->actor.id != ACTOR_EN_BOX) {
                    *velocityY = 0.0f;
                    self->actor.minVelocityY = 0.0f;
                    *gravity = 0.0f;
                } else {
                    *velocityY *= -1.0f;
                }
            } else {
                *velocityY *= -((kREG(20) * 0.01f) + 0.6f);
                if (*velocityY <= -*gravity * ((kREG(20) * 0.01f) + 0.6f)) {
                    *velocityY = 0.0f;
                    self->actor.minVelocityY = 0.0f;
                    *gravity = 0.0f;
                }
            }
            func_80AED4FC(self);
        }
    }
    if (self->actor.bgCheckFlags & 0x10) {
        speedXZ = &self->actor.speedXZ;
        velocityY = &self->actor.velocity.y;
        if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
            *speedXZ *= (kREG(19) * 0.01f) + 0.8f;
        } else {
            *speedXZ = 0.0f;
        }
        if (*velocityY >= 0.0f) {
            *velocityY *= -((kREG(20) * 0.01f) + 0.6f);
            func_80AED4FC(self);
        }
    }
    if (self->actor.bgCheckFlags & 8) {
        speedXZ = &self->actor.speedXZ;
        if (*speedXZ != 0.0f) {
            rotY = self->actor.world.rot.y;
            wallYaw = self->actor.wallYaw;
            temp_a0 = (wallYaw * 2) - rotY;
            temp_a1_2 = temp_a0 + 0x8000;
            if ((s16)((temp_a0 - wallYaw) + 0x8000) >= 0) {
                phi_v1 = (s16)(temp_a1_2 - wallYaw);
            } else {
                phi_v1 = -(s16)(temp_a1_2 - wallYaw);
            }
            if (phi_v1 < 0x4001) {
                if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
                    *speedXZ *= (kREG(21) * 0.01f) + 0.6f;
                } else {
                    *speedXZ = 0.0f;
                }
                self->actor.world.rot.y = temp_a1_2;
                func_80AED4FC(self);
                func_80AED5B8(self);
            }
        }
    }
}

void func_80AEDEF4(EnRu1* self, GlobalContext* globalCtx) {
    f32* speedXZ = &self->actor.speedXZ;
    DynaPolyActor* dynaPolyActor = DynaPoly_GetActor(&globalCtx->colCtx, self->actor.floorBgId);

    if (dynaPolyActor != NULL && dynaPolyActor->actor.id == ACTOR_EN_BOX) {
        if (*speedXZ != 0.0f) {
            *speedXZ *= 1.1f;
        } else {
            *speedXZ = 1.0f;
        }
    }
    if (*speedXZ >= (kREG(27) * 0.01f) + 3.0f) {
        *speedXZ *= (kREG(22) * 0.01f) + 0.98f;
    } else {
        *speedXZ = 0.0f;
    }
}

void func_80AEDFF4(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEDB30(self, globalCtx);
    func_80AEDEF4(self, globalCtx);
    Actor_MoveForward(&self->actor);
}

void func_80AEE02C(EnRu1* self) {
    self->actor.velocity.x = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.velocity.z = 0.0f;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = 0.0f;
    self->actor.minVelocityY = 0.0f;
}

void func_80AEE050(EnRu1* self) {
    s32 pad;
    f32 sp28;
    f32 sp24;
    f32 temp_f10;
    EnRu1* thisx = self; // necessary to match

    if (self->unk_350 == 0) {
        if ((self->actor.minVelocityY == 0.0f) && (self->actor.speedXZ == 0.0f)) {
            self->unk_350 = 1;
            func_80AEE02C(self);
            self->unk_35C = 0;
            self->unk_358 = (self->actor.yDistToWater - 10.0f) * 0.5f;
            self->unk_354 = self->actor.world.pos.y + thisx->unk_358; // thisx only used here
        } else {
            self->actor.gravity = 0.0f;
            self->actor.minVelocityY *= 0.2f;
            self->actor.velocity.y *= 0.2f;
            if (self->actor.minVelocityY >= -0.1f) {
                self->actor.minVelocityY = 0.0f;
                self->actor.velocity.y = 0.0f;
            }
            self->actor.speedXZ *= 0.5f;
            if (self->actor.speedXZ <= 0.1f) {
                self->actor.speedXZ = 0.0f;
            }
            self->actor.velocity.x = Math_SinS(self->actor.world.rot.y) * self->actor.speedXZ;
            self->actor.velocity.z = Math_CosS(self->actor.world.rot.y) * self->actor.speedXZ;
            func_8002D7EC(&self->actor);
        }
    } else {
        if (self->unk_350 == 1) {
            if (self->unk_358 <= 1.0f) {
                func_80AEE02C(self);
                self->unk_350 = 2;
                self->unk_360 = 0.0f;
            } else {
                sp28 = self->unk_358;
                sp24 = self->unk_354;
                temp_f10 = Math_CosS(self->unk_35C) * -sp28;
                self->actor.world.pos.y = temp_f10 + sp24;
                self->unk_35C += 0x3E8;
                self->unk_358 *= 0.95f;
            }
        } else {
            self->unk_360 += 1.0f;
            if (self->unk_360 > 0.0f) {
                self->unk_350 = 3;
            }
        }
    }
}

s32 func_80AEE264(EnRu1* self, GlobalContext* globalCtx) {
    if (!func_8002F194(&self->actor, globalCtx)) {
        self->actor.flags |= 9;
        if ((gSaveContext.infTable[20] & 8)) {
            self->actor.textId = 0x404E;
            func_8002F2F4(&self->actor, globalCtx);
        } else if (gSaveContext.infTable[20] & 4) {
            self->actor.textId = 0x404D;
            func_8002F2F4(&self->actor, globalCtx);
        } else {
            self->actor.textId = 0x404C;
            func_8002F2F4(&self->actor, globalCtx);
        }
        return false;
    }
    return true;
}

void func_80AEE2F8(EnRu1* self, GlobalContext* globalCtx) {
    DynaPolyActor* dynaPolyActor;
    s32 floorBgId;
    if ((self->actor.bgCheckFlags & 1) && (self->actor.floorBgId != BGCHECK_SCENE)) {
        floorBgId = self->actor.floorBgId;
        dynaPolyActor = DynaPoly_GetActor(&globalCtx->colCtx, floorBgId);
        if ((dynaPolyActor != NULL) && (dynaPolyActor->actor.id == ACTOR_BG_BDAN_SWITCH)) {
            if (((dynaPolyActor->actor.params >> 8) & 0x3F) == 0x38) {
                gSaveContext.infTable[20] |= 1;
                return;
            }
        }
    }
    gSaveContext.infTable[20] &= ~0x1;
}

s32 func_80AEE394(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[2];
    CollisionContext* colCtx;
    DynaPolyActor* dynaPolyActor;
    s32 floorBgId;

    if ((self->actor.bgCheckFlags & 1) && self->actor.floorBgId != BGCHECK_SCENE) {
        colCtx = &globalCtx->colCtx;
        floorBgId = self->actor.floorBgId; // necessary match, can't move self out of self block unfortunately
        dynaPolyActor = DynaPoly_GetActor(colCtx, floorBgId);
        if (dynaPolyActor != NULL && dynaPolyActor->actor.id == ACTOR_BG_BDAN_OBJECTS &&
            dynaPolyActor->actor.params == 0 && !Player_InCsMode(globalCtx) && globalCtx->msgCtx.unk_E300 == 0) {
            func_80AEE02C(self);
            globalCtx->csCtx.segment = &D_80AF10A4;
            gSaveContext.cutsceneTrigger = 1;
            self->action = 36;
            self->drawConfig = 0;
            self->unk_28C = (BgBdanObjects*)dynaPolyActor;
            self->actor.shape.shadowAlpha = 0;
            return true;
        }
    }
    return false;
}

void func_80AEE488(EnRu1* self, GlobalContext* globalCtx) {
    s8 curRoomNum;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        curRoomNum = globalCtx->roomCtx.curRoom.num;
        self->roomNum3 = curRoomNum;
        self->action = 31;
        func_80AED520(self, globalCtx);
    } else if ((!func_80AEE394(self, globalCtx)) && (!(self->actor.bgCheckFlags & 1))) {
        self->actor.minVelocityY = -((kREG(24) * 0.01f) + 6.8f);
        self->actor.gravity = -((kREG(23) * 0.01f) + 1.3f);
        self->action = 28;
    }
}

void func_80AEE568(EnRu1* self, GlobalContext* globalCtx) {
    if (!func_80AEE394(self, globalCtx)) {
        if ((self->actor.bgCheckFlags & 1) && (self->actor.speedXZ == 0.0f) && (self->actor.minVelocityY == 0.0f)) {
            func_80AEE02C(self);
            func_8002F580(&self->actor, globalCtx);
            self->action = 27;
            func_80AEADD8(self);
        } else if (self->actor.yDistToWater > 0.0f) {
            self->action = 29;
            self->unk_350 = 0;
        }
    }
}

void func_80AEE628(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s8 curRoomNum = globalCtx->roomCtx.curRoom.num;

    if (EnRu1_IsCsStateIdle(globalCtx)) {
        Animation_Change(&self->skelAnime, &gRutoChildSittingAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildSittingAnim), ANIMMODE_LOOP, -8.0f);
        gSaveContext.infTable[20] |= 0x10;
        self->action = 31;
    }
    self->roomNum3 = curRoomNum;
}

s32 func_80AEE6D0(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad;
    s8 curRoomNum = globalCtx->roomCtx.curRoom.num;

    if (!(gSaveContext.infTable[20] & 0x10) && (func_80AEB124(globalCtx) != 0)) {
        if (!Player_InCsMode(globalCtx)) {
            Animation_Change(&self->skelAnime, &gRutoChildSeesSapphireAnim, 1.0f, 0,
                             Animation_GetLastFrame(&gRutoChildSquirmAnim), ANIMMODE_LOOP, -8.0f);
            func_80AED600(self);
            self->action = 34;
            self->unk_26C = 0.0f;
            globalCtx->csCtx.segment = &D_80AF1728;
            gSaveContext.cutsceneTrigger = 1;
        }
        self->roomNum3 = curRoomNum;
        return true;
    }
    self->roomNum3 = curRoomNum;
    return false;
}

void func_80AEE7C4(EnRu1* self, GlobalContext* globalCtx) {
    f32 frameCount;
    s32 pad[13];
    Player* player;
    f32* unk_370 = &self->unk_370;

    if (Actor_HasNoParent(&self->actor, globalCtx)) {
        frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);
        Animation_Change(&self->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
        func_80AED6DC(self, globalCtx);
        self->actor.speedXZ *= (kREG(25) * 0.01f) + 1.0f;
        self->actor.velocity.y *= (kREG(26) * 0.01f) + 1.0f;
        self->actor.minVelocityY = -((kREG(24) * 0.01f) + 6.8f);
        self->actor.gravity = -((kREG(23) * 0.01f) + 1.3f);
        func_80AED57C(self);
        self->action = 28;
        *unk_370 = 0.0f;
        return;
    }

    if (func_80AEE6D0(self, globalCtx)) {
        *unk_370 = 0.0f;
        return;
    }

    player = GET_PLAYER(globalCtx);
    if (player->stateFlags2 & 0x10000000) {
        self->unk_370 += 1.0f;
        if (self->action != 32) {
            if (*unk_370 > 30.0f) {
                if (Rand_S16Offset(0, 3) == 0) {
                    frameCount = Animation_GetLastFrame(&gRutoChildSquirmAnim);
                    Animation_Change(&self->skelAnime, &gRutoChildSquirmAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP,
                                     -8.0f);
                    func_80AED5DC(self);
                    self->action = 32;
                }
                *unk_370 = 0.0f;
            }
        } else {
            if (*unk_370 > 50.0f) {
                frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);
                Animation_Change(&self->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
                self->action = 31;
                *unk_370 = 0.0f;
            }
        }
    } else {
        frameCount = Animation_GetLastFrame(&gRutoChildSittingAnim);
        Animation_Change(&self->skelAnime, &gRutoChildSittingAnim, 1.0f, 0, frameCount, ANIMMODE_LOOP, -8.0f);
        *unk_370 = 0.0f;
    }
}

s32 func_80AEEAC8(EnRu1* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 1) {
        func_80AEE02C(self);
        func_8002F580(&self->actor, globalCtx);
        self->action = 27;
        func_80AEADD8(self);
        return true;
    }
    return false;
}

void func_80AEEB24(EnRu1* self, GlobalContext* globalCtx) {
    if ((func_80AEEAC8(self, globalCtx) == 0) && (self->unk_350 == 3)) {
        self->action = 30;
        func_80AEE02C(self);
        self->actor.gravity = -0.1f;
        self->actor.minVelocityY = -((kREG(18) * 0.1f) + 0.7f);
    }
}

void func_80AEEBB4(EnRu1* self, GlobalContext* globalCtx) {
    func_8002F580(&self->actor, globalCtx);
}

void func_80AEEBD4(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEAC54(self, globalCtx);
    func_80AEAECC(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEEBB4(self, globalCtx);
    func_80AEE488(self, globalCtx);
    func_80AED624(self, globalCtx);
    func_80AEDAE0(self, globalCtx);
}

void func_80AEEC5C(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEACDC(self, globalCtx);
    func_80AEAECC(self, globalCtx);
    func_80AEE2F8(self, globalCtx);
    func_80AEDFF4(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEE568(self, globalCtx);
    func_80AED624(self, globalCtx);
    func_80AEDAE0(self, globalCtx);
}

void func_80AEECF0(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEAECC(self, globalCtx);
    func_80AEE050(self);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEEB24(self, globalCtx);
    func_80AED624(self, globalCtx);
}

void func_80AEED58(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEAECC(self, globalCtx);
    Actor_MoveForward(&self->actor);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEEAC8(self, globalCtx);
    func_80AED624(self, globalCtx);
    func_80AEDAE0(self, globalCtx);
}

void func_80AEEDCC(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED8DC(self);
    EnRu1_FrameUpdateMatrix(self);
    func_80AEAECC(self, globalCtx);
    func_80AEE2F8(self, globalCtx);
    EnRu1_UpdateEyes(self);
    func_80AED6F8(globalCtx);
    func_80AEE7C4(self, globalCtx);
}

void func_80AEEE34(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    EnRu1_FrameUpdateMatrix(self);
    func_80AEAECC(self, globalCtx);
    func_80AEE2F8(self, globalCtx);
    EnRu1_UpdateEyes(self);
    func_80AED6F8(globalCtx);
    func_80AEE7C4(self, globalCtx);
}

void func_80AEEE9C(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEAECC(self, globalCtx);
    func_80AEDFF4(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AED738(self, globalCtx);
    func_80AED624(self, globalCtx);
}

void func_80AEEF08(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    EnRu1_FrameUpdateMatrix(self);
    func_80AEAECC(self, globalCtx);
    EnRu1_UpdateEyes(self);
    func_80AEE628(self, globalCtx);
}

void func_80AEEF5C(EnRu1* self, GlobalContext* globalCtx) {
}

void func_80AEEF68(EnRu1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 something;

    self->unk_374.unk_18 = player->actor.world.pos;
    self->unk_374.unk_14 = kREG(16) - 3.0f;
    something = kREG(17) + 0xC;
    func_80034A14(&self->actor, &self->unk_374, something, 2);
}

void func_80AEEFEC(EnRu1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 something;

    self->unk_374.unk_18 = player->actor.world.pos;
    self->unk_374.unk_14 = kREG(16) - 3.0f;
    something = kREG(17) + 0xC;
    func_80034A14(&self->actor, &self->unk_374, something, 4);
    self->actor.world.rot.y = self->actor.shape.rot.y;
}

void func_80AEF080(EnRu1* self) {
    if (Animation_OnFrame(&self->skelAnime, 11.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_EV_LAND_DIRT);
    }
}

s32 func_80AEF0BC(EnRu1* self, GlobalContext* globalCtx) {
    s32 frameCount;

    if (gSaveContext.infTable[20] & 4) {
        frameCount = Animation_GetLastFrame(&gRutoChildSitAnim);
        Animation_Change(&self->skelAnime, &gRutoChildSitAnim, 1.0f, 0, frameCount, ANIMMODE_ONCE, -8.0f);
        globalCtx->msgCtx.msgMode = 0x37;
        self->action = 26;
        self->actor.flags &= ~0x9;
        return true;
    }
    return false;
}

void func_80AEF170(EnRu1* self, GlobalContext* globalCtx, s32 cond) {
    if (cond) {
        self->action = 25;
    }
}

void func_80AEF188(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEB174(globalCtx) && !func_80AEF0BC(self, globalCtx)) {
        func_80106CCC(globalCtx);
        gSaveContext.infTable[20] |= 4;
        self->action = 24;
    }
}

void func_80AEF1F0(EnRu1* self, GlobalContext* globalCtx, UNK_TYPE arg2) {
    if (arg2 != 0) {
        Animation_Change(&self->skelAnime, &gRutoChildSittingAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gRutoChildSittingAnim), ANIMMODE_LOOP, 0.0f);
        func_80106CCC(globalCtx);
        gSaveContext.infTable[20] |= 8;
        func_80AED6DC(self, globalCtx);
        func_8002F580(&self->actor, globalCtx);
        self->action = 27;
        func_80AEADD8(self);
    }
}

void func_80AEF29C(EnRu1* self, GlobalContext* globalCtx) {
    self->action = 23;
}

void func_80AEF2AC(EnRu1* self, GlobalContext* globalCtx) {
    self->action = 24;
    self->drawConfig = 1;
    self->actor.flags |= 9;
}

void func_80AEF2D0(EnRu1* self, GlobalContext* globalCtx) {
    s32 cond;

    func_80AEEF68(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAC10(self, globalCtx);
    func_80AEAECC(self, globalCtx);
    cond = func_80AEE264(self, globalCtx);
    func_80AED624(self, globalCtx);
    func_80AEF170(self, globalCtx, cond);
}

void func_80AEF354(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEEFEC(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAECC(self, globalCtx);
    func_80AEF188(self, globalCtx);
}

void func_80AEF3A8(EnRu1* self, GlobalContext* globalCtx) {
    s32 something;

    func_80AED83C(self);
    something = EnRu1_FrameUpdateMatrix(self);
    func_80AEF080(self);
    EnRu1_UpdateEyes(self);
    func_80AEAECC(self, globalCtx);
    func_80AEF1F0(self, globalCtx, something);
}

void func_80AEF40C(EnRu1* self) {
    SkelAnime* skelAnime = &self->skelAnime;

    if (Animation_OnFrame(skelAnime, 2.0f) || Animation_OnFrame(skelAnime, 7.0f) ||
        Animation_OnFrame(skelAnime, 12.0f) || Animation_OnFrame(skelAnime, 18.0f) ||
        Animation_OnFrame(skelAnime, 25.0f) || Animation_OnFrame(skelAnime, 33.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_WALK_DIRT);
    }
}

void func_80AEF4A8(EnRu1* self, GlobalContext* globalCtx) {
    Audio_PlaySoundAtPosition(globalCtx, &self->actor.projectedPos, 20, NA_SE_VO_RT_FALL);
}

void func_80AEF4E0(EnRu1* self) {
    if (Animation_OnFrame(&self->skelAnime, 5.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_LAUGH_0);
    }
}

void func_80AEF51C(EnRu1* self) {
    func_80078914(&self->actor.projectedPos, NA_SE_VO_RT_THROW);
}

void func_80AEF540(EnRu1* self) {
    if (func_80AEB104(self) == 2) {
        EnRu1_SetEyeIndex(self, 3);
        EnRu1_SetMouthIndex(self, 2);
        if (self->skelAnime.mode != 2) {
            func_80AEB264(self, &gRutoChildShutterAnim, 2, -8.0f, 0);
            func_80AEF51C(self);
        }
    }
}

void func_80AEF5B8(EnRu1* self) {
    f32 curFrame;

    if (D_80AF1938 == 0) {
        curFrame = self->skelAnime.curFrame;
        if (curFrame >= 60.0f) {
            EnRu1_SetEyeIndex(self, 3);
            EnRu1_SetMouthIndex(self, 0);
            func_80AED57C(self);
            D_80AF1938 = 1;
        }
    }
}

void func_80AEF624(EnRu1* self, GlobalContext* globalCtx) {
    f32 frameCount;
    CsCmdActorAction* csCmdNPCAction;
    CsCmdActorAction* csCmdNPCAction2;
    s16 newRotTmp;

    if (func_80AEAFE0(globalCtx, 1, 3)) {
        frameCount = Animation_GetLastFrame(&gRutoChildWalkToAndHoldUpSapphireAnim);
        // self weird part with the redundant variable is necessary to match for some reason
        csCmdNPCAction2 = globalCtx->csCtx.npcActions[3];
        csCmdNPCAction = csCmdNPCAction2;
        self->actor.world.pos.x = csCmdNPCAction->startPos.x;
        self->actor.world.pos.y = csCmdNPCAction->startPos.y;
        self->actor.world.pos.z = csCmdNPCAction->startPos.z;
        newRotTmp = csCmdNPCAction->rot.x;
        self->actor.shape.rot.x = newRotTmp;
        self->actor.world.rot.x = newRotTmp;
        newRotTmp = csCmdNPCAction->rot.y;
        self->actor.shape.rot.y = newRotTmp;
        self->actor.world.rot.y = newRotTmp;
        newRotTmp = csCmdNPCAction->rot.z;
        self->actor.shape.rot.z = newRotTmp;
        self->actor.world.rot.z = newRotTmp;
        Animation_Change(&self->skelAnime, &gRutoChildWalkToAndHoldUpSapphireAnim, 1.0f, 0.0f, frameCount,
                         ANIMMODE_ONCE, 0.0f);
        func_80AEB3A4(self, globalCtx);
        self->action = 37;
        self->drawConfig = 1;
        self->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80AEF728(EnRu1* self, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&self->skelAnime, &gRutoChildHoldArmsUpAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gRutoChildHoldArmsUpAnim), ANIMMODE_LOOP, 0.0f);
        func_80AEB3CC(self);
        self->action = 38;
    }
}

void func_80AEF79C(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEAFE0(globalCtx, 2, 3)) {
        Animation_Change(&self->skelAnime, &gRutoChildBringHandsDownAnim, 1.0f, 0,
                         Animation_GetLastFrame(&gRutoChildBringHandsDownAnim), ANIMMODE_ONCE, -8.0f);
        self->action = 39;
    }
}

void func_80AEF820(EnRu1* self, UNK_TYPE arg1) {
    if (arg1 != 0) {
        Animation_Change(&self->skelAnime, &gRutoChildWait2Anim, 1.0f, 0, Animation_GetLastFrame(&gRutoChildWait2Anim),
                         ANIMMODE_LOOP, -8.0f);
        self->action = 40;
    }
}

void func_80AEF890(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s8 curRoomNum;

    if ((gSaveContext.sceneSetupIndex < 4) && (EnRu1_IsCsStateIdle(globalCtx))) {
        curRoomNum = globalCtx->roomCtx.curRoom.num;
        gSaveContext.infTable[20] |= 0x20;
        Flags_SetSwitch(globalCtx, func_80AEADE0(self));
        func_80AEB0EC(self, 1);
        self->action = 42;
        self->actor.room = curRoomNum;
    }
}

void func_80AEF930(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEB104(self) == 3) {
        self->actor.flags |= 9;
        self->actor.textId = 0x4048;
        func_8010B720(globalCtx, self->actor.textId);
        func_80AEF4A8(self, globalCtx);
        self->action = 43;
        self->drawConfig = 0;
    }
}

void func_80AEF99C(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEB1B4(globalCtx) != 0) {
        func_80AEB0EC(self, 4);
        Actor_Kill(&self->actor);
    }
}

void func_80AEF9D8(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEF624(self, globalCtx);
    func_80AEB220(self, globalCtx);
}

void func_80AEFA2C(EnRu1* self, GlobalContext* globalCtx) {
    s32 something;

    func_80AED83C(self);
    func_80AEB364(self, globalCtx);
    func_80AEAECC(self, globalCtx);
    something = EnRu1_FrameUpdateMatrix(self);
    func_80AEF4E0(self);
    func_80AEF5B8(self);
    func_80AEF40C(self);
    func_80AEF728(self, something);
    func_80AEB220(self, globalCtx);
}

void func_80AEFAAC(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEAECC(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    func_80AEF79C(self, globalCtx);
    func_80AEB220(self, globalCtx);
}

void func_80AEFB04(EnRu1* self, GlobalContext* globalCtx) {
    s32 something;

    func_80AED83C(self);
    func_80AEAECC(self, globalCtx);
    something = EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEF820(self, something);
    func_80AEB220(self, globalCtx);
}

void func_80AEFB68(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEAECC(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEF890(self, globalCtx);
    func_80AEB220(self, globalCtx);
}

void func_80AEFBC8(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEAECC(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEF540(self);
    func_80AEF930(self, globalCtx);
}

void func_80AEFC24(EnRu1* self, GlobalContext* globalCtx) {
    func_80AED83C(self);
    func_80AEF99C(self, globalCtx);
}

void func_80AEFC54(EnRu1* self, GlobalContext* globalCtx) {
    if ((gSaveContext.infTable[20] & 0x20) && !(gSaveContext.infTable[20] & 0x40)) {
        func_80AEB264(self, &gRutoChildWait2Anim, 0, 0, 0);
        self->action = 41;
        self->unk_28C = EnRu1_FindSwitch(globalCtx);
        func_80AEB0EC(self, 1);
        self->actor.flags &= ~0x9;
    } else {
        Actor_Kill(&self->actor);
    }
}

void func_80AEFCE8(EnRu1* self, GlobalContext* globalCtx) {
    self->unk_28C = EnRu1_FindSwitch(globalCtx);
    if (self->unk_28C != NULL) {
        self->action = 42;
        self->drawConfig = 1;
        func_80AEB0EC(self, 1);
    }
}

void func_80AEFD38(EnRu1* self, GlobalContext* globalCtx) {
    if ((gSaveContext.eventChkInf[3] & 0x80) && LINK_IS_CHILD) {
        func_80AEB264(self, &gRutoChildWait2Anim, 0, 0, 0);
        self->actor.flags &= ~0x10;
        self->action = 44;
        self->drawConfig = 1;
    } else {
        Actor_Kill(&self->actor);
    }
}

s32 func_80AEFDC0(EnRu1* self, GlobalContext* globalCtx) {
    if (!func_8002F194(&self->actor, globalCtx)) {
        self->actor.flags |= 9;
        self->actor.textId = Text_GetFaceReaction(globalCtx, 0x1F);
        if (self->actor.textId == 0) {
            self->actor.textId = 0x402C;
        }
        func_8002F2F4(&self->actor, globalCtx);
        return false;
    }
    return true;
}

s32 func_80AEFE38(EnRu1* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        self->actor.flags &= ~0x9;
        return true;
    }
    return false;
}

void func_80AEFE84(EnRu1* self, GlobalContext* globalCtx, s32 cond) {
    if (cond) {
        self->action = 45;
    }
}

void func_80AEFE9C(EnRu1* self, GlobalContext* globalCtx) {
    if (func_80AEFE38(self, globalCtx)) {
        self->action = 44;
    }
}

void func_80AEFECC(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEEF68(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAC10(self, globalCtx);
    func_80AEAECC(self, globalCtx);
    func_80AEFE84(self, globalCtx, func_80AEFDC0(self, globalCtx));
}

void func_80AEFF40(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEEFEC(self, globalCtx);
    EnRu1_FrameUpdateMatrix(self);
    EnRu1_UpdateEyes(self);
    func_80AEAECC(self, globalCtx);
    func_80AEFE9C(self, globalCtx);
}

void func_80AEFF94(EnRu1* self, GlobalContext* globalCtx) {
    s8 actorRoom;

    if ((gSaveContext.infTable[20] & 2) && (gSaveContext.infTable[20] & 1) && !(gSaveContext.infTable[20] & 0x20) &&
        (!(func_80AEB020(self, globalCtx)))) {
        func_80AEB264(self, &gRutoChildWait2Anim, 0, 0, 0);
        actorRoom = self->actor.room;
        self->action = 22;
        self->actor.room = -1;
        self->drawConfig = 0;
        self->roomNum1 = actorRoom;
        self->roomNum3 = actorRoom;
        self->roomNum2 = actorRoom;
        // "Ruto switch set"
        osSyncPrintf("スイッチルトセット!!!!!!!!!!!!!!!!!!!!!!\n");
    } else {
        // "Ruto switch not set"
        osSyncPrintf("スイッチルトセットしない!!!!!!!!!!!!!!!!!!!!!!\n");
        Actor_Kill(&self->actor);
    }
}

void func_80AF0050(EnRu1* self, GlobalContext* globalCtx) {
    func_80AEB264(self, &gRutoChildWait2Anim, 0, 0, 0);
    self->action = 36;
    self->roomNum1 = self->actor.room;
    self->unk_28C = EnRu1_FindSwitch(globalCtx);
    self->actor.room = -1;
}

void EnRu1_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnRu1* self = THIS;

    if (self->action < 0 || self->action >= ARRAY_COUNT(sActionFuncs) || sActionFuncs[self->action] == NULL) {
        // "Main mode is improper!"
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }

    sActionFuncs[self->action](self, globalCtx);
}

void EnRu1_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnRu1* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gRutoChildSkel, NULL, self->jointTable, self->morphTable, 17);
    func_80AEAD20(&self->actor, globalCtx);
    switch (func_80AEADF0(self)) {
        case 0:
            func_80AECDA0(self, globalCtx);
            break;
        case 1:
            func_80AEB3DC(self, globalCtx);
            break;
        case 2:
            func_80AEC320(self, globalCtx);
            break;
        case 3:
            func_80AED44C(self, globalCtx);
            break;
        case 4:
            func_80AEFC54(self, globalCtx);
            break;
        case 5:
            func_80AEFD38(self, globalCtx);
            break;
        case 6:
            func_80AEFF94(self, globalCtx);
            break;
        case 10:
            func_80AF0050(self, globalCtx);
            break;
        default:
            Actor_Kill(&self->actor);
            // "Relevant arge_data = %d unacceptable"
            osSyncPrintf("該当 arge_data = %d 無し\n", func_80AEADF0(self));
            break;
    }
}

void func_80AF0278(EnRu1* self, GlobalContext* globalCtx, s32 limbIndex, Vec3s* rot) {
    Vec3s* vec1 = &self->unk_374.unk_0E;
    Vec3s* vec2 = &self->unk_374.unk_08;

    switch (limbIndex) {
        case RUTO_CHILD_LEFT_UPPER_ARM:
            rot->x += vec1->y;
            rot->y -= vec1->x;
            break;
        case RUTO_CHILD_TORSO:
            rot->x += vec2->y;
            rot->z += vec2->x;
            break;
    }
}

s32 EnRu1_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                           Gfx** gfx) {
    EnRu1* self = THIS;

    if ((self->unk_290 < 0) || (self->unk_290 > 0) || (*sPreLimbDrawFuncs[self->unk_290] == NULL)) {
        // "Neck rotation mode is improper!"
        osSyncPrintf(VT_FGCOL(RED) "首回しモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sPreLimbDrawFuncs[self->unk_290](self, globalCtx, limbIndex, rot);
    }
    return false;
}

void EnRu1_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnRu1* self = THIS;
    Vec3f vec1;
    Vec3f vec2;

    if (limbIndex == RUTO_CHILD_TORSO) {
        vec1 = sMultVec;
        Matrix_MultVec3f(&vec1, &vec2);
        self->actor.focus.pos.x = vec2.x;
        self->actor.focus.pos.y = vec2.y;
        self->actor.focus.pos.z = vec2.z;
        self->actor.focus.rot.x = self->actor.world.rot.x;
        self->actor.focus.rot.y = self->actor.world.rot.y;
        self->actor.focus.rot.z = self->actor.world.rot.z;
    }
}

void EnRu1_DrawNothing(EnRu1* self, GlobalContext* globalCtx) {
}

void EnRu1_DrawOpa(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s16 eyeIndex = self->eyeIndex;
    void* eyeTex = sEyeTextures[eyeIndex];
    s16 mouthIndex = self->mouthIndex;
    SkelAnime* skelAnime = &self->skelAnime;
    void* mouthTex = sMouthTextures[mouthIndex];
    s32 pad1;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ru1.c", 1282);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTex));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);

    POLY_OPA_DISP = SkelAnime_DrawFlex(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       EnRu1_OverrideLimbDraw, EnRu1_PostLimbDraw, self, POLY_OPA_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ru1.c", 1309);
}

void EnRu1_DrawXlu(EnRu1* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s16 eyeIndex = self->eyeIndex;
    void* eyeTex = sEyeTextures[eyeIndex];
    s16 mouthIndex = self->mouthIndex;
    SkelAnime* skelAnime = &self->skelAnime;
    void* mouthTex = sMouthTextures[mouthIndex];
    s32 pad1;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ru1.c", 1324);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTex));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->alpha);
    gSPSegment(POLY_XLU_DISP++, 0x0C, &D_80116280[0]);

    POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       EnRu1_OverrideLimbDraw, NULL, self, POLY_XLU_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ru1.c", 1353);
}

void EnRu1_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnRu1* self = THIS;

    if (self->drawConfig < 0 || self->drawConfig >= ARRAY_COUNT(sDrawFuncs) || sDrawFuncs[self->drawConfig] == 0) {
        // "Draw mode is improper!"
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sDrawFuncs[self->drawConfig](self, globalCtx);
}
