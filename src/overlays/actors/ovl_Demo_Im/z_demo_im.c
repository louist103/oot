/*
 * File: z_demo_im.c
 * Overlay: Demo_Im
 * Description: Impa
 */

#include "z_demo_im.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "scenes/indoors/nakaniwa/nakaniwa_scene.h"
#include "objects/object_im/object_im.h"
#include "vt.h"

#define FLAGS 0x00000011

#define THIS ((DemoIm*)thisx)

void DemoIm_Init(Actor* thisx, GlobalContext* globalCtx);
void DemoIm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DemoIm_Update(Actor* thisx, GlobalContext* globalCtx);
void DemoIm_Draw(Actor* thisx, GlobalContext* globalCtx);
void func_809856F8(DemoIm* self, GlobalContext* globalCtx);
void func_80985718(DemoIm* self, GlobalContext* globalCtx);
void func_80985738(DemoIm* self, GlobalContext* globalCtx);
void func_80985770(DemoIm* self, GlobalContext* globalCtx);
void func_809857B0(DemoIm* self, GlobalContext* globalCtx);
void func_809857F0(DemoIm* self, GlobalContext* globalCtx);
void func_80985830(DemoIm* self, GlobalContext* globalCtx);
void func_80985C10(DemoIm* self, GlobalContext* globalCtx);
void func_80985C40(DemoIm* self, GlobalContext* globalCtx);
void func_80985C94(DemoIm* self, GlobalContext* globalCtx);
void func_80985CE8(DemoIm* self, GlobalContext* globalCtx);
void func_809863BC(DemoIm* self, GlobalContext* globalCtx);
void func_809863DC(DemoIm* self, GlobalContext* globalCtx);
void func_80986430(DemoIm* self, GlobalContext* globalCtx);
void func_80986494(DemoIm* self, GlobalContext* globalCtx);
void func_809864D4(DemoIm* self, GlobalContext* globalCtx);
void func_809868E8(DemoIm* self, GlobalContext* globalCtx);
void func_80986908(DemoIm* self, GlobalContext* globalCtx);
void func_80986948(DemoIm* self, GlobalContext* globalCtx);
void func_80986D40(DemoIm* self, GlobalContext* globalCtx);
void func_80986DC8(DemoIm* self, GlobalContext* globalCtx);
void func_80986E20(DemoIm* self, GlobalContext* globalCtx);
void func_80986E40(DemoIm* self, GlobalContext* globalCtx);
void func_80986EAC(DemoIm* self, GlobalContext* globalCtx);
void func_80986F08(DemoIm* self, GlobalContext* globalCtx);
void func_80986F28(DemoIm* self, GlobalContext* globalCtx);
void func_80986F88(DemoIm* self, GlobalContext* globalCtx);
void func_80986FA8(DemoIm* self, GlobalContext* globalCtx);
void func_80987288(DemoIm* self, GlobalContext* globalCtx);
void func_809872A8(DemoIm* self, GlobalContext* globalCtx);
void func_809872F0(DemoIm* self, GlobalContext* globalCtx);
void func_80987330(DemoIm* self, GlobalContext* globalCtx);
void func_8098764C(DemoIm* self, GlobalContext* globalCtx);
void func_80987658(DemoIm* self, GlobalContext* globalCtx);

static void* D_80987830[] = {
    0x06007210,
    0x06007D50,
    0x06008150,
};

static u32 D_8098783C = 0;

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    { 0x00, { 0x00000000, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, 0x00, 0x00, 0x01 },
    { 25, 80, 0, { 0, 0, 0 } },
};

#include "z_demo_im_cutscene_data.c" EARLY

static DemoImActionFunc sActionFuncs[] = {
    func_809856F8, func_80985718, func_80985738, func_80985770, func_809857B0, func_809857F0, func_80985830,
    func_80985C10, func_80985C40, func_80985C94, func_809863BC, func_809863DC, func_80986430, func_80986494,
    func_809864D4, func_809868E8, func_80986908, func_80986948, func_80986D40, func_80986DC8, func_80986E20,
    func_80986E40, func_80986EAC, func_80986F08, func_80986F28, func_80986F88, func_80986FA8, func_80987288,
    func_809872A8, func_809872F0, func_80987330,
};

static Vec3f D_809887D8 = { 0.0f, 10.0f, 0.0f };

static DemoImDrawFunc sDrawFuncs[] = {
    func_8098764C,
    func_80987658,
    func_80985CE8,
};

const ActorInit Demo_Im_InitVars = {
    ACTOR_DEMO_IM,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_IM,
    sizeof(DemoIm),
    (ActorFunc)DemoIm_Init,
    (ActorFunc)DemoIm_Destroy,
    (ActorFunc)DemoIm_Update,
    (ActorFunc)DemoIm_Draw,
};

void func_80984BE0(DemoIm* self) {
    s32 pad[3];
    s16* unk_25E = &self->unk_25E;
    s16* unk_25C = &self->unk_25C;

    if (DECR(*unk_25E) == 0) {
        *unk_25E = Rand_S16Offset(0x3C, 0x3C);
    }

    *unk_25C = *unk_25E;
    if (*unk_25C >= 3) {
        *unk_25C = 0;
    }
}

void func_80984C68(DemoIm* self) {
    self->action = 7;
    self->drawConfig = 0;
    self->unk_26C = 0;
    self->unk_270 = 0;
    self->actor.shape.shadowAlpha = 0;
    self->unk_268 = 0.0f;
}

void func_80984C8C(DemoIm* self, GlobalContext* globalCtx) {
    u32* something = &D_8098783C;

    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        if (*something != 0) {
            if (self->actor.params == 2) {
                func_80984C68(self);
            }
            *something = 0;
        }
    } else {
        if (*something == 0) {
            *something = 1;
        }
    }
}

void func_80984D00(Actor* thisx, GlobalContext* globalCtx) {
    DemoIm* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit);
}

void func_80984D4C(Actor* thisx, GlobalContext* globalCtx) {
    DemoIm* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80984D74(DemoIm* self, GlobalContext* globalCtx) {
    s32 pad[5];

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void func_80984DB8(DemoIm* self) {
    s32 pad[2];
    Vec3s* vec1 = &self->unk_2D4.unk_08;
    Vec3s* vec2 = &self->unk_2D4.unk_0E;

    Math_SmoothStepToS(&vec1->x, 0, 20, 6200, 100);
    Math_SmoothStepToS(&vec1->y, 0, 20, 6200, 100);

    Math_SmoothStepToS(&vec2->x, 0, 20, 6200, 100);
    Math_SmoothStepToS(&vec2->y, 0, 20, 6200, 100);
}

void func_80984E58(DemoIm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 yawDiff;
    s16 phi_a3;

    self->unk_2D4.unk_18 = player->actor.world.pos;
    self->unk_2D4.unk_14 = kREG(16) + 4.0f;

    yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    phi_a3 = (ABS(yawDiff) < 0x18E3) ? 2 : 1;
    func_80034A14(&self->actor, &self->unk_2D4, kREG(17) + 0xC, phi_a3);
}

void func_80984F10(DemoIm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_2D4.unk_18 = player->actor.world.pos;
    self->unk_2D4.unk_14 = kREG(16) + 12.0f;

    func_80034A14(&self->actor, &self->unk_2D4, kREG(17) + 0xC, 2);
}

void func_80984F94(DemoIm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_2D4.unk_18 = player->actor.world.pos;
    self->unk_2D4.unk_14 = kREG(16) + 4.0f;
    func_80034A14(&self->actor, &self->unk_2D4, kREG(17) + 0xC, 4);
}

void func_80985018(DemoIm* self, GlobalContext* globalCtx) {
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 75.0f, 30.0f, 30.0f, 5);
}

s32 func_80985060(DemoIm* self) {
    return SkelAnime_Update(&self->skelAnime);
}

s32 func_80985080(GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        return 1;
    }
    return 0;
}

CsCmdActorAction* func_809850A0(GlobalContext* globalCtx, s32 actionIdx) {
    s32 pad[2];
    CsCmdActorAction* ret = NULL;

    if (!func_80985080(globalCtx)) {
        ret = globalCtx->csCtx.npcActions[actionIdx];
    }
    return ret;
}

s32 func_809850E8(DemoIm* self, GlobalContext* globalCtx, u16 action, s32 actionIdx) {
    CsCmdActorAction* npcAction = func_809850A0(globalCtx, actionIdx);

    if (npcAction != NULL) {
        if (npcAction->action == action) {
            return 1;
        }
    }
    return 0;
}

s32 func_80985134(DemoIm* self, GlobalContext* globalCtx, u16 action, s32 actionIdx) {
    CsCmdActorAction* npcAction = func_809850A0(globalCtx, actionIdx);

    if (npcAction != NULL) {
        if (npcAction->action != action) {
            return 1;
        }
    }
    return 0;
}

void func_80985180(DemoIm* self, GlobalContext* globalCtx, s32 actionIdx) {
    CsCmdActorAction* npcAction = func_809850A0(globalCtx, actionIdx);

    if (npcAction != NULL) {
        self->actor.world.pos.x = npcAction->startPos.x;
        self->actor.world.pos.y = npcAction->startPos.y;
        self->actor.world.pos.z = npcAction->startPos.z;
        self->actor.world.rot.y = self->actor.shape.rot.y = npcAction->rot.y;
    }
}

void func_80985200(DemoIm* self, GlobalContext* globalCtx, s32 actionIdx) {
    CsCmdActorAction* npcAction = func_809850A0(globalCtx, actionIdx);

    if (npcAction != NULL) {
        self->actor.world.pos.x = npcAction->startPos.x;
        self->actor.world.pos.y = npcAction->startPos.y;
        self->actor.world.pos.z = npcAction->startPos.z;
        self->actor.world.rot.y = self->actor.shape.rot.y = npcAction->rot.y;
    }
}

void func_80985280(DemoIm* self, AnimationHeader* animHeaderSeg, u8 arg2, f32 transitionRate, s32 arg4) {
    f32 frameCount = Animation_GetLastFrame(animHeaderSeg);
    f32 playbackSpeed;
    f32 unk0;
    f32 fc;

    if (arg4 == 0) {
        unk0 = 0.0f;
        fc = frameCount;
        playbackSpeed = 1.0f;
    } else {
        fc = 0.0f;
        unk0 = frameCount;
        playbackSpeed = -1.0f;
    }

    Animation_Change(&self->skelAnime, animHeaderSeg, playbackSpeed, unk0, fc, arg2, transitionRate);
}

void func_80985310(DemoIm* self, GlobalContext* globalCtx) {
    func_80985280(self, &object_im_Anim_001868, 0, 0.0f, 0);
    self->actor.shape.yOffset = -10000.0f;
}

void func_80985358(DemoIm* self, GlobalContext* globalCtx) {
    f32 posX = self->actor.world.pos.x;
    f32 posY = self->actor.world.pos.y;
    f32 posZ = self->actor.world.pos.z;

    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, posX, posY, posZ, 0, 0, 0,
                       WARP_SAGES);
}

void func_809853B4(DemoIm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 playerX = player->actor.world.pos.x;
    f32 playerY = player->actor.world.pos.y + 80.0f;
    f32 playerZ = player->actor.world.pos.z;

    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_EFFECT, playerX, playerY, playerZ, 0,
                       0, 0, 0xD);
    Item_Give(globalCtx, ITEM_MEDALLION_SHADOW);
}

void func_80985430(DemoIm* self, GlobalContext* globalCtx) {
    self->actor.shape.yOffset += 250.0f / 3.0f;
}

void func_8098544C(DemoIm* self, GlobalContext* globalCtx) {
    s32 pad[2];

    if ((gSaveContext.chamberCutsceneNum == 4) && (gSaveContext.sceneSetupIndex < 4)) {
        Player* player = GET_PLAYER(globalCtx);

        self->action = 1;
        globalCtx->csCtx.segment = D_8098786C;
        gSaveContext.cutsceneTrigger = 2;
        Item_Give(globalCtx, ITEM_MEDALLION_SHADOW);
        player->actor.world.rot.y = player->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
    }
}

void func_809854DC(DemoIm* self, GlobalContext* globalCtx) {
    if ((globalCtx->csCtx.state != CS_STATE_IDLE) && (globalCtx->csCtx.npcActions[5] != NULL) &&
        (globalCtx->csCtx.npcActions[5]->action == 2)) {
        Animation_Change(&self->skelAnime, &object_im_Anim_001868, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_im_Anim_001868), ANIMMODE_LOOP, 0.0f);
        self->action = 2;
        self->drawConfig = 1;
        func_80985358(self, globalCtx);
    }
}

void func_8098557C(DemoIm* self) {
    if (self->actor.shape.yOffset >= 0.0f) {
        self->action = 3;
        self->actor.shape.yOffset = 0.0f;
    }
}

void func_809855A8(DemoIm* self, GlobalContext* globalCtx) {
    if ((globalCtx->csCtx.state != CS_STATE_IDLE) && (globalCtx->csCtx.npcActions[5] != NULL) &&
        (globalCtx->csCtx.npcActions[5]->action == 3)) {
        Animation_Change(&self->skelAnime, &object_im_Anim_000710, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_im_Anim_000710), ANIMMODE_ONCE, 4.0f);
        self->action = 4;
    }
}

void func_80985640(DemoIm* self, s32 arg1) {
    if (arg1 != 0) {
        Animation_Change(&self->skelAnime, &object_im_Anim_000AFC, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_im_Anim_000AFC), ANIMMODE_LOOP, 0.0f);
        self->action = 5;
    }
}

void func_809856AC(DemoIm* self, GlobalContext* globalCtx) {
    if ((globalCtx->csCtx.state != CS_STATE_IDLE) && (globalCtx->csCtx.npcActions[6] != NULL) &&
        (globalCtx->csCtx.npcActions[6]->action == 2)) {
        self->action = 6;
        func_809853B4(self, globalCtx);
    }
}

void func_809856F8(DemoIm* self, GlobalContext* globalCtx) {
    func_8098544C(self, globalCtx);
}

void func_80985718(DemoIm* self, GlobalContext* globalCtx) {
    func_809854DC(self, globalCtx);
}

void func_80985738(DemoIm* self, GlobalContext* globalCtx) {
    func_80985430(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_8098557C(self);
}

void func_80985770(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_809855A8(self, globalCtx);
}

void func_809857B0(DemoIm* self, GlobalContext* globalCtx) {
    s32 sp1C;

    func_80985018(self, globalCtx);
    sp1C = func_80985060(self);
    func_80984BE0(self);
    func_80985640(self, sp1C);
}

void func_809857F0(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_809856AC(self, globalCtx);
}

void func_80985830(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
}

void func_80985860(DemoIm* self, GlobalContext* globalCtx) {
    func_80985280(self, &object_im_Anim_001868, 0, 0.0f, 0);
    self->action = 7;
    self->actor.shape.shadowAlpha = 0;
}

void func_809858A8(void) {
    func_800788CC(NA_SE_SY_WHITE_OUT_T);
}

void func_809858C8(DemoIm* self, GlobalContext* globalCtx) {
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_6K, self->actor.world.pos.x,
                       (kREG(17) + 24.0f) + self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 6);
}

void func_80985948(DemoIm* self, GlobalContext* globalCtx) {
    if (func_809850E8(self, globalCtx, 4, 5)) {
        Animation_Change(&self->skelAnime, &object_im_Anim_011C08, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_im_Anim_011C08), ANIMMODE_ONCE, 0.0f);
        self->action = 8;
        self->drawConfig = 2;
        self->unk_26C = 0;
        self->actor.shape.shadowAlpha = 0;
        self->unk_268 = 0.0f;
        func_809858A8();
    }
}

void func_809859E0(DemoIm* self, GlobalContext* globalCtx) {
    f32* unk_268 = &self->unk_268;
    s32 alpha = 255;

    if (func_809850E8(self, globalCtx, 4, 5)) {
        *unk_268 += 1.0f;
        if (*unk_268 >= kREG(5) + 10.0f) {
            self->action = 9;
            self->drawConfig = 1;
            *unk_268 = kREG(5) + 10.0f;
            self->unk_26C = self->actor.shape.shadowAlpha = alpha;
            return;
        }
    } else {
        *unk_268 -= 1.0f;
        if (*unk_268 <= 0.0f) {
            self->action = 7;
            self->drawConfig = 0;
            *unk_268 = 0.0f;
            self->unk_26C = 0;
            self->actor.shape.shadowAlpha = 0;
            return;
        }
    }
    self->actor.shape.shadowAlpha = self->unk_26C = (*unk_268 / (kREG(5) + 10.0f)) * 255.0f;
}

void func_80985B34(DemoIm* self, GlobalContext* globalCtx) {
    if (func_80985134(self, globalCtx, 4, 5)) {
        Animation_Change(&self->skelAnime, &object_im_Anim_012218, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_im_Anim_012218), ANIMMODE_ONCE, -8.0f);
        self->action = 8;
        self->drawConfig = 2;
        self->unk_268 = kREG(5) + 10.0f;
        self->unk_26C = 255;
        if (self->unk_270 == 0) {
            func_809858C8(self, globalCtx);
            self->unk_270 = 1;
        }
        self->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80985C10(DemoIm* self, GlobalContext* globalCtx) {
    func_80985948(self, globalCtx);
    func_80984C8C(self, globalCtx);
}

void func_80985C40(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_809859E0(self, globalCtx);
    func_80984C8C(self, globalCtx);
}

void func_80985C94(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_80985B34(self, globalCtx);
    func_80984C8C(self, globalCtx);
}

void func_80985CE8(DemoIm* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s16 unk_25C = self->unk_25C;
    void* sp68 = D_80987830[unk_25C];
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_im_inKenjyanomaDemo02.c", 281);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sp68));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sp68));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->unk_26C);
    gSPSegment(POLY_XLU_DISP++, 0x0C, &D_80116280[0]);

    POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       NULL, NULL, NULL, POLY_XLU_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_im_inKenjyanomaDemo02.c", 308);
}

void func_80985E60(DemoIm* self, GlobalContext* globalCtx) {
    func_80985280(self, &object_im_Anim_001868, 0, 0.0f, 0);
    self->action = 10;
    self->unk_280 = 1;
}

void func_80985EAC(DemoIm* self, GlobalContext* globalCtx) {
    if ((globalCtx->csCtx.frames >= 80) && (globalCtx->csCtx.frames < 243)) {
        func_80984F10(self, globalCtx);
    } else {
        func_80984DB8(self);
    }
}

void func_80985EF4(DemoIm* self) {
    if (!Animation_OnFrame(&self->skelAnime, Animation_GetLastFrame(&object_im_Anim_01182C) - 1.0f)) {
        func_80985060(self);
    }
}

void func_80985F54(DemoIm* self) {
    self->action = 10;
    self->drawConfig = 0;
}

void func_80985F64(DemoIm* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &object_im_Anim_001868, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_im_Anim_001868), ANIMMODE_LOOP, 0.0f);
    func_80985180(self, globalCtx, 5);
    self->action = 11;
    self->drawConfig = 1;
}

void func_80985FE8(DemoIm* self, s32 arg1) {
    if (arg1 != 0) {
        Animation_Change(&self->skelAnime, &object_im_Anim_01182C, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_im_Anim_01182C), ANIMMODE_LOOP, -8.0f);
    }
}

void func_8098604C(DemoIm* self) {
    f32 frameCount = Animation_GetLastFrame(&object_im_Anim_010EE0);

    Animation_Change(&self->skelAnime, &object_im_Anim_010EE0, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, -8.0f);
    self->action = 12;
    self->drawConfig = 1;
    self->unk_2D0 = 1;
}

void func_809860C8(DemoIm* self) {
    self->action = 13;
    self->drawConfig = 1;
}

void func_809860DC(DemoIm* self, s32 arg1) {
    if (arg1 != 0) {
        Animation_Change(&self->skelAnime, &object_im_Anim_001868, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_im_Anim_001868), ANIMMODE_LOOP, -8.0f);
        self->unk_2D0 = 0;
    }
}

void func_80986148(DemoIm* self) {
    Animation_Change(&self->skelAnime, &object_im_Anim_010EE0, -1.0f, Animation_GetLastFrame(&object_im_Anim_010EE0),
                     0.0f, ANIMMODE_ONCE, -8.0f);
    self->action = 14;
    self->drawConfig = 1;
}

void func_809861C4(DemoIm* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = func_809850A0(globalCtx, 5);

    if (npcAction != NULL) {
        u32 action = npcAction->action;
        u32 unk_274 = self->unk_274;

        if (action != unk_274) {
            switch (action) {
                case 9:
                    func_80986148(self);
                    break;
                case 7:
                    Animation_Change(&self->skelAnime, &object_im_Anim_01182C, 1.0f, 0.0f,
                                     Animation_GetLastFrame(&object_im_Anim_01182C), ANIMMODE_LOOP, -8.0f);
                    self->action = 12;
                    break;
                default:
                    osSyncPrintf("Demo_Im_Ocarina_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
            }
            self->unk_274 = action;
        }
    }
}

void func_8098629C(DemoIm* self, GlobalContext* globalCtx) {
    if (func_80985080(globalCtx)) {
        self->action = 21;
        self->drawConfig = 1;
        self->unk_280 = 1;
    }
}

void func_809862E0(DemoIm* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = func_809850A0(globalCtx, 5);

    if (npcAction != NULL) {
        u32 action = npcAction->action;
        u32 unk_274 = self->unk_274;
        if (action != unk_274) {
            switch (action) {
                case 1:
                    func_80985F54(self);
                    break;
                case 2:
                    func_80985F64(self, globalCtx);
                    break;
                case 7:
                    func_8098604C(self);
                    break;
                case 8:
                    func_809860C8(self);
                    break;
                case 9:
                    func_80986148(self);
                    break;
                default:
                    osSyncPrintf("Demo_Im_Ocarina_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
            }
            self->unk_274 = action;
        }
    }
}

void func_809863BC(DemoIm* self, GlobalContext* globalCtx) {
    func_809862E0(self, globalCtx);
}

void func_809863DC(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80985EAC(self, globalCtx);
    func_80984BE0(self);
    func_809862E0(self, globalCtx);
}

void func_80986430(DemoIm* self, GlobalContext* globalCtx) {
    s32 sp24;

    func_80985018(self, globalCtx);
    sp24 = func_80985060(self);
    func_80985EAC(self, globalCtx);
    func_80984BE0(self);
    func_80985FE8(self, sp24);
    func_809862E0(self, globalCtx);
}

void func_80986494(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985EF4(self);
    func_80984BE0(self);
    func_809861C4(self, globalCtx);
}

void func_809864D4(DemoIm* self, GlobalContext* globalCtx) {
    s32 sp24;

    func_80985018(self, globalCtx);
    sp24 = func_80985060(self);
    func_80984BE0(self);
    func_809860DC(self, sp24);
    func_8098629C(self, globalCtx);
}

void func_8098652C(DemoIm* self, GlobalContext* globalCtx) {
    func_80985280(self, &object_im_Anim_001868, 0, 0.0f, 0);
    self->action = 15;
}

void func_80986570(DemoIm* self, GlobalContext* globalCtx) {
    if (Animation_OnFrame(&self->skelAnime, 7.0f) && (self->actor.bgCheckFlags & 1)) {
        u32 sfxId = SFX_FLAG;

        sfxId += SurfaceType_GetSfx(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);
        Audio_PlaySoundGeneral(sfxId, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
}

void func_809865F8(DemoIm* self, GlobalContext* globalCtx, s32 arg2) {
    s32 pad[2];

    if (arg2 != 0) {
        f32* unk_278 = &self->unk_278;

        if (*unk_278 >= 0.0f) {
            if (self->unk_27C == 0) {
                Vec3f* thisPos = &self->actor.world.pos;
                s16 shapeRotY = self->actor.shape.rot.y;
                f32 spawnPosX = thisPos->x + (Math_SinS(shapeRotY) * 30.0f);
                f32 spawnPosY = thisPos->y;
                f32 spawnPosZ = thisPos->z + (Math_CosS(shapeRotY) * 30.0f);

                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ARROW, spawnPosX, spawnPosY, spawnPosZ, 0xFA0,
                            self->actor.shape.rot.y, 0, ARROW_CS_NUT);
                self->unk_27C = 1;
            }
        } else {
            *unk_278 += 1.0f;
        }
    }
}

void func_80986700(DemoIm* self) {
    self->action = 15;
    self->drawConfig = 0;
}

void func_80986710(DemoIm* self, GlobalContext* globalCtx) {
    Animation_Change(&self->skelAnime, &object_im_Anim_001868, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_im_Anim_001868), ANIMMODE_LOOP, 0.0f);
    func_80985180(self, globalCtx, 5);
    self->action = 16;
    self->drawConfig = 1;
}

void func_80986794(DemoIm* self) {
    Animation_Change(&self->skelAnime, &object_im_Anim_0014E4, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_im_Anim_0014E4), ANIMMODE_ONCE, -8.0f);
    self->action = 17;
    self->drawConfig = 1;
}

void func_8098680C(DemoIm* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = func_809850A0(globalCtx, 5);

    if (npcAction != NULL) {
        u32 action = npcAction->action;
        u32 unk_274 = self->unk_274;

        if (action != unk_274) {
            switch (action) {
                case 1:
                    func_80986700(self);
                    break;
                case 2:
                    func_80986710(self, globalCtx);
                    break;
                case 10:
                    func_80986794(self);
                    break;
                case 11:
                    Actor_Kill(&self->actor);
                    break;
                default:
                    osSyncPrintf("Demo_Im_Spot00_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
            }
            self->unk_274 = action;
        }
    }
}

void func_809868E8(DemoIm* self, GlobalContext* globalCtx) {
    func_8098680C(self, globalCtx);
}

void func_80986908(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_8098680C(self, globalCtx);
}

void func_80986948(DemoIm* self, GlobalContext* globalCtx) {
    s32 sp24;

    func_80985018(self, globalCtx);
    sp24 = func_80985060(self);
    func_80986570(self, globalCtx);
    func_80984BE0(self);
    func_809865F8(self, globalCtx, sp24);
    func_8098680C(self, globalCtx);
}

void func_809869B0(DemoIm* self, GlobalContext* globalCtx) {
    func_80985280(self, &object_im_Anim_001868, 0, 0.0f, 0);
    self->action = 18;
    self->actor.shape.shadowAlpha = 0;
}

s32 func_809869F8(DemoIm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 playerPosX = player->actor.world.pos.x;
    f32 thisPosX = self->actor.world.pos.x;

    if ((thisPosX - (kREG(16) + 30.0f) > playerPosX) && (!(self->actor.flags & 0x40))) {
        return 1;
    }
    return 0;
}

s32 func_80986A5C(DemoIm* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 playerPosX = player->actor.world.pos.x;
    f32 thisPosX = self->actor.world.pos.x;

    if ((thisPosX - (kREG(17) + 130.0f) < playerPosX) && (!Gameplay_InCsMode(globalCtx))) {
        return 1;
    }
    return 0;
}

s32 func_80986AD0(DemoIm* self, GlobalContext* globalCtx) {
    self->actor.flags |= 9;
    if (!func_8002F194(&self->actor, globalCtx)) {
        self->actor.textId = 0x708E;
        func_8002F2F4(&self->actor, globalCtx);
    } else {
        return 1;
    }
    return 0;
}

void func_80986B2C(GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        Player* player = GET_PLAYER(globalCtx);

        globalCtx->nextEntranceIndex = 0xCD;
        globalCtx->fadeTransition = 38;
        globalCtx->sceneLoadFlag = 0x14;
        func_8002DF54(globalCtx, &player->actor, 8);
    }
}

void func_80986BA0(DemoIm* self, GlobalContext* globalCtx) {
    if (func_809869F8(self, globalCtx)) {
        self->action = 21;
        self->drawConfig = 1;
        self->unk_280 = 1;
        self->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80986BE4(DemoIm* self, s32 arg1) {
    if (arg1 != 0) {
        self->action = 22;
    }
}

void func_80986BF8(DemoIm* self, GlobalContext* globalCtx) {
    if (gSaveContext.eventChkInf[4] & 1) {
        self->action = 24;
        self->drawConfig = 1;
        self->unk_280 = 1;
        self->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80986C30(DemoIm* self, GlobalContext* globalCtx) {
    if (func_80986A5C(self, globalCtx)) {
        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gZeldasCourtyardLullabyCs);
        gSaveContext.cutsceneTrigger = 1;
        gSaveContext.eventChkInf[5] |= 0x200;
        Item_Give(globalCtx, ITEM_SONG_LULLABY);
        func_80985F54(self);
    }
}

void func_80986CC8(DemoIm* self) {
    if (gSaveContext.eventChkInf[4] & 1) {
        self->action = 26;
        self->drawConfig = 1;
        self->unk_280 = 1;
        self->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80986CFC(DemoIm* self, GlobalContext* globalCtx) {
    if (func_80986A5C(self, globalCtx)) {
        gSaveContext.eventChkInf[4] |= 0x1000;
        self->action = 19;
    }
}

void func_80986D40(DemoIm* self, GlobalContext* globalCtx) {
    if (gSaveContext.sceneSetupIndex == 6) {
        self->action = 19;
        self->drawConfig = 1;
    } else if (gSaveContext.eventChkInf[8] & 1) {
        Actor_Kill(&self->actor);
    } else if (!(gSaveContext.eventChkInf[5] & 0x200)) {
        self->action = 23;
    } else {
        self->action = 20;
    }
}

void func_80986DC8(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_80984E58(self, globalCtx);
    self->actor.flags &= ~0x9;
}

void func_80986E20(DemoIm* self, GlobalContext* globalCtx) {
    func_80986BA0(self, globalCtx);
}

void func_80986E40(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_80984E58(self, globalCtx);
    func_80984D74(self, globalCtx);
    func_80986BE4(self, func_80986AD0(self, globalCtx));
}

void func_80986EAC(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_80984F94(self, globalCtx);
    func_80984D74(self, globalCtx);
    func_80986B2C(globalCtx);
}

void func_80986F08(DemoIm* self, GlobalContext* globalCtx) {
    func_80986BF8(self, globalCtx);
}

void func_80986F28(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_80984E58(self, globalCtx);
    func_80984D74(self, globalCtx);
    func_80986C30(self, globalCtx);
}

void func_80986F88(DemoIm* self, GlobalContext* globalCtx) {
    func_80986CC8(self);
}

void func_80986FA8(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_80984E58(self, globalCtx);
    self->actor.flags &= ~0x9;
    func_80984D74(self, globalCtx);
    func_80986CFC(self, globalCtx);
}

void func_80987018(DemoIm* self, GlobalContext* globalCtx) {
    func_80985280(self, &object_im_Anim_001868, 0, 0.0f, 0);
    self->action = 27;
    self->drawConfig = 0;
    self->actor.shape.shadowAlpha = 0;
}

void func_80987064(DemoIm* self) {
    f32* unk_268 = &self->unk_268;
    f32 temp;
    s32 alpha = 255;

    *unk_268 += 1.0f;
    temp = kREG(17) + 10.0f;

    if (*unk_268 >= temp) {
        self->actor.shape.shadowAlpha = self->unk_26C = alpha;
    } else {
        self->actor.shape.shadowAlpha = self->unk_26C = (*unk_268 / temp) * 255.0f;
    }
}

void func_809870F0(DemoIm* self, GlobalContext* globalCtx) {
    func_80985200(self, globalCtx, 5);
    self->action = 28;
    self->drawConfig = 2;
}

void func_80987128(DemoIm* self) {
    if (self->unk_268 >= kREG(17) + 10.0f) {
        self->action = 29;
        self->drawConfig = 1;
    }
}

void func_80987174(DemoIm* self) {
    func_80985280(self, &object_im_Anim_0101C8, 2, -8.0f, 0);
    self->action = 30;
}

void func_809871B4(DemoIm* self, s32 arg1) {
    if (arg1 != 0) {
        func_80985280(self, &object_im_Anim_00FB10, 0, 0.0f, 0);
    }
}

void func_809871E8(DemoIm* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = func_809850A0(globalCtx, 5);

    if (npcAction != NULL) {
        u32 action = npcAction->action;
        u32 unk_274 = self->unk_274;

        if (action != unk_274) {
            switch (action) {
                case 12:
                    func_809870F0(self, globalCtx);
                    break;
                case 13:
                    func_80987174(self);
                    break;
                default:
                    osSyncPrintf("Demo_Im_inEnding_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
            }
            self->unk_274 = action;
        }
    }
}

void func_80987288(DemoIm* self, GlobalContext* globalCtx) {
    func_809871E8(self, globalCtx);
}

void func_809872A8(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_80987064(self);
    func_80987128(self);
}

void func_809872F0(DemoIm* self, GlobalContext* globalCtx) {
    func_80985018(self, globalCtx);
    func_80985060(self);
    func_80984BE0(self);
    func_809871E8(self, globalCtx);
}

void func_80987330(DemoIm* self, GlobalContext* globalCtx) {
    s32 sp1C;

    func_80985018(self, globalCtx);
    sp1C = func_80985060(self);
    func_80984BE0(self);
    func_809871B4(self, sp1C);
}

void DemoIm_Update(Actor* thisx, GlobalContext* globalCtx) {
    DemoIm* self = THIS;

    if ((self->action < 0) || (self->action >= 31) || (sActionFuncs[self->action] == NULL)) {
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sActionFuncs[self->action](self, globalCtx);
}

void DemoIm_Init(Actor* thisx, GlobalContext* globalCtx) {
    DemoIm* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    func_80984D00(thisx, globalCtx);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_im_Skel_00F788, NULL, self->jointTable, self->morphTable,
                       17);
    thisx->flags &= ~1;

    switch (self->actor.params) {
        case 2:
            func_80985860(self, globalCtx);
            break;
        case 3:
            func_80985E60(self, globalCtx);
            break;
        case 4:
            func_8098652C(self, globalCtx);
            break;
        case 5:
            func_809869B0(self, globalCtx);
            break;
        case 6:
            func_80987018(self, globalCtx);
            break;
        default:
            func_80985310(self, globalCtx);
    }
}

void DemoIm_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    func_80984D4C(thisx, globalCtx);
}

s32 DemoIm_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    DemoIm* self = THIS;
    s32* unk_2D0 = &self->unk_2D0;

    if (self->unk_280 != 0) {
        Vec3s* unk_2D4_unk_0E = &self->unk_2D4.unk_0E;
        Vec3s* unk_2D4_unk_08 = &self->unk_2D4.unk_08;

        switch (limbIndex) {
            case 9:
                rot->x += unk_2D4_unk_0E->y;
                rot->y -= unk_2D4_unk_0E->x;
                break;
            case 16:
                rot->x += unk_2D4_unk_08->y;
                rot->z += unk_2D4_unk_08->x;
                break;
        }
    }

    if ((*unk_2D0 != 0) && (limbIndex == 15)) {
        *dList = object_im_DL_00EDE8;
    }

    return false;
}

void DemoIm_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    DemoIm* self = THIS;

    if (limbIndex == 16) {
        Vec3f sp28 = D_809887D8;
        Vec3f dest;

        Matrix_MultVec3f(&sp28, &dest);
        self->actor.focus.pos.x = dest.x;
        self->actor.focus.pos.y = dest.y;
        self->actor.focus.pos.z = dest.z;
        self->actor.focus.rot.x = self->actor.world.rot.x;
        self->actor.focus.rot.y = self->actor.world.rot.y;
        self->actor.focus.rot.z = self->actor.world.rot.z;
    }
}

void func_8098764C(DemoIm* self, GlobalContext* globalCtx) {
}

void func_80987658(DemoIm* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s16 unk_25C = self->unk_25C;
    void* sp68 = D_80987830[unk_25C];
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_demo_im.c", 904);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sp68));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sp68));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);

    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                          DemoIm_OverrideLimbDraw, DemoIm_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_demo_im.c", 925);
}

void DemoIm_Draw(Actor* thisx, GlobalContext* globalCtx) {
    DemoIm* self = THIS;

    if ((self->drawConfig < 0) || (self->drawConfig >= 3) || (sDrawFuncs[self->drawConfig] == NULL)) {
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sDrawFuncs[self->drawConfig](self, globalCtx);
}
