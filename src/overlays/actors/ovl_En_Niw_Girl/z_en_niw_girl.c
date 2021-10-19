/*
 * File: z_en_niw_girl.c
 * Overlay: ovl_En_Niw_Girl
 * Description: Girl that chases a cuckoo around in Hyrule Market and Kakariko Village
 */

#include "z_en_niw_girl.h"
#include "objects/object_gr/object_gr.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnNiwGirl*)thisx)

void EnNiwGirl_Init(Actor* thisx, GlobalContext* globalCtx);
void EnNiwGirl_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnNiwGirl_Update(Actor* thisx, GlobalContext* globalCtx);
void EnNiwGirl_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnNiwGirl_Talk(EnNiwGirl* self, GlobalContext* globalCtx);
void func_80AB94D0(EnNiwGirl* self, GlobalContext* globalCtx);
void func_80AB9210(EnNiwGirl* self, GlobalContext* globalCtx);

const ActorInit En_Niw_Girl_InitVars = {
    ACTOR_EN_NIW_GIRL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GR,
    sizeof(EnNiwGirl),
    (ActorFunc)EnNiwGirl_Init,
    (ActorFunc)EnNiwGirl_Destroy,
    (ActorFunc)EnNiwGirl_Update,
    (ActorFunc)EnNiwGirl_Draw,
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
    { 10, 30, 0, { 0, 0, 0 } },
};

void EnNiwGirl_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnNiwGirl* self = THIS;
    s32 pad;
    Vec3f vec1;
    Vec3f vec2;
    s32 pad2;

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gNiwGirlSkel, &gNiwGirlRunAnim, self->jointTable, self->morphTable,
                       17);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.targetMode = 6;
    if (self->actor.params < 0) {
        self->actor.params = 0;
    }
    self->path = ((self->actor.params >> 8) & 0xFF);
    self->actor.gravity = -3.0f;
    Matrix_RotateY((self->actor.shape.rot.y / 32768.0f) * M_PI, MTXMODE_NEW);
    vec2.x = vec2.y = vec2.z = 0.0f;
    vec1.x = vec1.y = 0.0f;
    vec1.z = 50.0;
    Matrix_MultVec3f(&vec1, &vec2);
    self->chasedEnNiw = (EnNiw*)Actor_SpawnAsChild(
        &globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_NIW, self->actor.world.pos.x + vec2.x,
        self->actor.world.pos.y + vec2.y, self->actor.world.pos.z + vec2.z, 0, self->actor.world.rot.y, 0, 0xA);
    if (self->chasedEnNiw != NULL) {
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ シツレイしちゃうわね！プンプン ☆☆☆☆☆ %d\n" VT_RST, self->actor.params);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ きゃははははは、まてー ☆☆☆☆☆ %d\n" VT_RST, self->path);
        osSyncPrintf("\n\n");
        self->actor.colChkInfo.mass = MASS_IMMOVABLE;
        self->actionFunc = EnNiwGirl_Talk;
    } else {
        osSyncPrintf("\n\n");
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ なぜか、セットできむぅあせん ☆☆☆☆☆ %d\n" VT_RST, self->actor.params);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ んんがくく ☆☆☆☆☆ %d\n" VT_RST, self->path);
        osSyncPrintf("\n\n");
        Actor_Kill(&self->actor);
    }
}

void EnNiwGirl_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnNiwGirl_Jump(EnNiwGirl* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gNiwGirlRunAnim);
    Animation_Change(&self->skelAnime, &gNiwGirlRunAnim, 1.0f, 0.0f, frameCount, 0, -10.0f);
    self->actor.flags &= ~1;
    self->actionFunc = func_80AB9210;
}

void func_80AB9210(EnNiwGirl* self, GlobalContext* globalCtx) {
    Path* path = &globalCtx->setupPathList[self->path];
    f32 xDistBetween;
    f32 zDistBetween;

    SkelAnime_Update(&self->skelAnime);
    Math_ApproachF(&self->actor.speedXZ, 3.0f, 0.2f, 0.4f);

    // Find the X and Z distance between the girl and the cuckoo she is chasing
    xDistBetween = self->chasedEnNiw->actor.world.pos.x - self->actor.world.pos.x;
    zDistBetween = self->chasedEnNiw->actor.world.pos.z - self->actor.world.pos.z;
    if (func_8010BDBC(&globalCtx->msgCtx) != 0) {
        self->chasedEnNiw->path = 0;
    }
    if (sqrtf(SQ(xDistBetween) + SQ(zDistBetween)) < 70.0f) {
        self->chasedEnNiw->path = (self->path + 1);
        self->chasedEnNiw->unk_2EC = path->count;
    } else if (sqrtf(SQ(xDistBetween) + SQ(zDistBetween)) > 150.0f) {
        self->chasedEnNiw->path = 0;
    }

    // Change her angle so that she is always facing the cuckoo
    Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(xDistBetween, zDistBetween) * (0x8000 / M_PI), 3,
                       self->unk_27C, 0);
    Math_ApproachF(&self->unk_27C, 5000.0f, 30.0f, 150.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y;

    // Only allow Link to talk to her when she is playing the jumping animation
    if ((self->jumpTimer == 0) || (Player_GetMask(globalCtx) != PLAYER_MASK_NONE)) {
        self->jumpTimer = 60;
        self->actionFunc = EnNiwGirl_Talk;
    }
}

void EnNiwGirl_Talk(EnNiwGirl* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &gNiwGirlJumpAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gNiwGirlJumpAnim), 0,
                     -10.0f);
    self->actor.flags |= 1;
    self->actor.textId = 0x7000;
    if ((gSaveContext.eventChkInf[8] & 1) && (self->unk_27A == 0)) {
        self->actor.textId = 0x70EA;
    }
    switch (Player_GetMask(globalCtx)) {
        case PLAYER_MASK_KEATON:
            self->actor.textId = 0x7118;
            break;
        case PLAYER_MASK_SPOOKY:
            self->actor.textId = 0x7119;
            break;
        case PLAYER_MASK_BUNNY:
        case PLAYER_MASK_ZORA:
        case PLAYER_MASK_GERUDO:
            self->actor.textId = 0x711A;
            break;
        case PLAYER_MASK_SKULL:
        case PLAYER_MASK_GORON:
        case PLAYER_MASK_TRUTH:
            self->actor.textId = 0x711B;
            break;
    }
    self->unk_270 = 6;
    self->actionFunc = func_80AB94D0;
}

void func_80AB94D0(EnNiwGirl* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (func_8010BDBC(&globalCtx->msgCtx)) {
        self->chasedEnNiw->path = 0;
    }
    Math_ApproachZeroF(&self->actor.speedXZ, 0.8f, 0.2f);
    if (func_8002F194(&self->actor, globalCtx)) {
        if (self->actor.textId == 0x70EA) {
            self->unk_27A = 1;
        }
    } else {
        if ((self->jumpTimer == 0) && !func_8010BDBC(&globalCtx->msgCtx)) {
            self->jumpTimer = Rand_ZeroFloat(100.0f) + 250.0f;
            self->actionFunc = EnNiwGirl_Jump;
        } else {
            func_8002F2CC(&self->actor, globalCtx, 100.0f);
        }
    }
}

void EnNiwGirl_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnNiwGirl* self = THIS;
    EnNiwGirlActionFunc tempActionFunc;
    Player* player = GET_PLAYER(globalCtx);

    Actor_SetScale(&self->actor, 0.013f);
    self->unkUpTimer++;
    tempActionFunc = func_80AB94D0;
    if (self->blinkTimer == 0) {
        self->eyeIndex++;
        if (self->eyeIndex >= 3) {
            self->eyeIndex = 0;
            self->blinkTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }
    self->unk_280 = 30.0f;
    Actor_SetFocus(&self->actor, 30.0f);
    if (tempActionFunc == self->actionFunc) {
        self->unk_2D4.unk_18 = player->actor.world.pos;
        if (!LINK_IS_ADULT) {
            self->unk_2D4.unk_18.y = player->actor.world.pos.y - 10.0f;
        }
        func_80034A14(&self->actor, &self->unk_2D4, 2, 4);
        self->unk_260 = self->unk_2D4.unk_08;
        self->unk_266 = self->unk_2D4.unk_0E;
    } else {
        Math_SmoothStepToS(&self->unk_266.y, 0, 5, 3000, 0);
        Math_SmoothStepToS(&self->unk_260.y, 0, 5, 3000, 0);
        Math_SmoothStepToS(&self->unk_260.z, 0, 5, 3000, 0);
    }
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    }
    if (self->jumpTimer != 0) {
        self->jumpTimer--;
    }
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 100.0f, 100.0f, 200.0f, 0x1C);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

s32 EnNiwGirlOverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                              void* thisx) {
    EnNiwGirl* self = THIS;

    if (limbIndex == 3) {
        rot->x += self->unk_266.y;
    }
    if (limbIndex == 4) {
        rot->x += self->unk_260.y;
        rot->z += self->unk_260.z;
    }
    return false;
}

static Vec3f sConstVec3f = { 0.2f, 0.2f, 0.2f };

void EnNiwGirl_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { gNiwGirlEyeOpenTex, gNiwGirlEyeHalfTex, gNiwGirlEyeClosedTex };
    EnNiwGirl* self = THIS;
    s32 pad;
    Vec3f sp4C = sConstVec3f;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_niw_girl.c", 573);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeIndex]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnNiwGirlOverrideLimbDraw, NULL, self);
    func_80033C30(&self->actor.world.pos, &sp4C, 255, globalCtx);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_niw_girl.c", 592);
}
