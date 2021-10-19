/*
 * File: z_demo_ec.c
 * Overlay: ovl_Demo_Ec
 * Description: Credits revelers in Lon Lon
 */

#include "z_demo_ec.h"
#include "vt.h"
#include "objects/object_zo/object_zo.h"
#include "objects/object_ec/object_ec.h"
#include "objects/object_fish/object_fish.h"
#include "objects/object_oF1d_map/object_oF1d_map.h"
#include "objects/object_ma2/object_ma2.h"
#include "objects/object_in/object_in.h"
#include "objects/object_ta/object_ta.h"
#include "objects/object_fu/object_fu.h"
#include "objects/object_toryo/object_toryo.h"
#include "objects/object_daiku/object_daiku.h"
#include "objects/object_ge1/object_ge1.h"
#include "objects/object_kz/object_kz.h"
#include "objects/object_md/object_md.h"
#include "objects/object_niw/object_niw.h"
#include "objects/object_ds2/object_ds2.h"
#include "objects/object_os/object_os.h"
#include "objects/object_rs/object_rs.h"
#include "objects/object_gm/object_gm.h"
#include "objects/object_km1/object_km1.h"
#include "objects/object_kw1/object_kw1.h"
#include "objects/object_bji/object_bji.h"
#include "objects/object_ahg/object_ahg.h"
#include "objects/object_bob/object_bob.h"
#include "objects/object_bba/object_bba.h"
#include "objects/object_ane/object_ane.h"

#define FLAGS 0x00000010

#define THIS ((DemoEc*)thisx)

void DemoEc_Init(Actor* thisx, GlobalContext* globalCtx);
void DemoEc_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DemoEc_Update(Actor* thisx, GlobalContext* globalCtx);
void DemoEc_Draw(Actor* thisx, GlobalContext* globalCtx);

typedef enum {
    /* 00 */ EC_UPDATE_COMMON,
    /* 01 */ EC_UPDATE_INGO,
    /* 02 */ EC_UPDATE_TALON,
    /* 03 */ EC_UPDATE_WINDMILL_MAN,
    /* 04 */ EC_UPDATE_KOKIRI_BOY,
    /* 05 */ EC_UPDATE_KOKIRI_GIRL,
    /* 06 */ EC_UPDATE_OLD_MAN,
    /* 07 */ EC_UPDATE_BEARDED_MAN,
    /* 08 */ EC_UPDATE_WOMAN,
    /* 09 */ EC_UPDATE_OLD_WOMAN,
    /* 10 */ EC_UPDATE_BOSS_CARPENTER,
    /* 11 */ EC_UPDATE_CARPENTER,
    /* 12 */ EC_UPDATE_DANCING_KOKIRI_BOY,
    /* 13 */ EC_UPDATE_DANCING_KOKIRI_GIRL,
    /* 14 */ EC_UPDATE_GERUDO,
    /* 15 */ EC_UPDATE_DANCING_ZORA,
    /* 16 */ EC_UPDATE_KING_ZORA,
    /* 17 */ EC_UPDATE_17,
    /* 18 */ EC_UPDATE_18,
    /* 19 */ EC_UPDATE_MIDO,
    /* 20 */ EC_UPDATE_20,
    /* 21 */ EC_UPDATE_CUCCO,
    /* 22 */ EC_UPDATE_CUCCO_LADY,
    /* 23 */ EC_UPDATE_POTION_SHOP_OWNER,
    /* 24 */ EC_UPDATE_MASK_SHOP_OWNER,
    /* 25 */ EC_UPDATE_FISHING_MAN,
    /* 26 */ EC_UPDATE_BOMBCHU_SHOP_OWNER,
    /* 27 */ EC_UPDATE_GORON,
    /* 28 */ EC_UPDATE_MALON
} DemoEcUpdateMode;

typedef enum {
    /* 00 */ EC_DRAW_COMMON,
    /* 01 */ EC_DRAW_INGO,
    /* 02 */ EC_DRAW_TALON,
    /* 03 */ EC_DRAW_WINDMILL_MAN,
    /* 04 */ EC_DRAW_KOKIRI_BOY,
    /* 05 */ EC_DRAW_KOKIRI_GIRL,
    /* 06 */ EC_DRAW_OLD_MAN,
    /* 07 */ EC_DRAW_BEARDED_MAN,
    /* 08 */ EC_DRAW_WOMAN,
    /* 09 */ EC_DRAW_OLD_WOMAN,
    /* 10 */ EC_DRAW_BOSS_CARPENTER,
    /* 11 */ EC_DRAW_CARPENTER,
    /* 12 */ EC_DRAW_GERUDO,
    /* 13 */ EC_DRAW_DANCING_ZORA,
    /* 14 */ EC_DRAW_KING_ZORA,
    /* 15 */ EC_DRAW_MIDO,
    /* 16 */ EC_DRAW_CUCCO,
    /* 17 */ EC_DRAW_CUCCO_LADY,
    /* 18 */ EC_DRAW_POTION_SHOP_OWNER,
    /* 19 */ EC_DRAW_MASK_SHOP_OWNER,
    /* 20 */ EC_DRAW_FISHING_MAN,
    /* 21 */ EC_DRAW_BOMBCHU_SHOP_OWNER,
    /* 22 */ EC_DRAW_GORON,
    /* 23 */ EC_DRAW_MALON
} DemoEcDrawconfig;

static s16 sDrawObjects[] = {
    /*  0 */ OBJECT_IN,
    /*  1 */ OBJECT_TA,
    /*  2 */ OBJECT_FU,
    /*  3 */ OBJECT_KM1,
    /*  4 */ OBJECT_KW1,
    /*  5 */ OBJECT_BJI,
    /*  6 */ OBJECT_AHG,
    /*  7 */ OBJECT_BOB,
    /*  8 */ OBJECT_BBA,
    /*  9 */ OBJECT_TORYO,
    /* 10 */ OBJECT_DAIKU,
    /* 11 */ OBJECT_DAIKU,
    /* 12 */ OBJECT_DAIKU,
    /* 13 */ OBJECT_DAIKU,
    /* 14 */ OBJECT_KM1,
    /* 15 */ OBJECT_KW1,
    /* 16 */ OBJECT_GE1,
    /* 17 */ OBJECT_GE1,
    /* 18 */ OBJECT_GE1,
    /* 19 */ OBJECT_ZO,
    /* 20 */ OBJECT_KZ,
    /* 21 */ OBJECT_MD,
    /* 22 */ OBJECT_NIW,
    /* 23 */ OBJECT_NIW,
    /* 24 */ OBJECT_NIW,
    /* 25 */ OBJECT_ANE,
    /* 26 */ OBJECT_DS2,
    /* 27 */ OBJECT_OS,
    /* 28 */ OBJECT_FISH,
    /* 29 */ OBJECT_RS,
    /* 30 */ OBJECT_OF1D_MAP,
    /* 31 */ OBJECT_OF1D_MAP,
    /* 32 */ OBJECT_OF1D_MAP,
    /* 33 */ OBJECT_OF1D_MAP,
    /* 34 */ OBJECT_MA2,
};

static s16 sAnimationObjects[] = {
    OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC,  OBJECT_EC,
    OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC,  OBJECT_EC,
    OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC,  OBJECT_EC,
    OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_EC, OBJECT_GM, OBJECT_MA2,
};

extern FlexSkeletonHeader object_bji_Skel_0000F0;
extern FlexSkeletonHeader object_ahg_Skel_0000F0;

void DemoEc_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    DemoEc* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
}

void DemoEc_Init(Actor* thisx, GlobalContext* globalCtx) {
    DemoEc* self = THIS;

    if ((self->actor.params < 0) || (self->actor.params > 34)) {
        osSyncPrintf(VT_FGCOL(RED) "Demo_Ec_Actor_ct:arg_dataがおかしい!!!!!!!!!!!!\n" VT_RST);
        Actor_Kill(&self->actor);
    } else {
        self->updateMode = EC_UPDATE_COMMON;
        self->drawConfig = EC_DRAW_COMMON;
    }
}

s32 DemoEc_UpdateSkelAnime(DemoEc* self) {
    return SkelAnime_Update(&self->skelAnime);
}

void DemoEc_UpdateBgFlags(DemoEc* self, GlobalContext* globalCtx) {
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 30.0f, 25.0f, 30.0f, 7);
}

void func_8096D594(DemoEc* self, GlobalContext* globalCtx) {
    self->skelAnime.moveFlags |= 3;
    AnimationContext_SetMoveActor(globalCtx, &self->actor, &self->skelAnime, 1.0f);
}

void func_8096D5D4(DemoEc* self, GlobalContext* globalCtx) {
    self->skelAnime.baseTransl = self->skelAnime.jointTable[0];
    self->skelAnime.prevTransl = self->skelAnime.jointTable[0];
    self->skelAnime.moveFlags |= 3;
    AnimationContext_SetMoveActor(globalCtx, &self->actor, &self->skelAnime, 1.0f);
}

void func_8096D64C(DemoEc* self, GlobalContext* globalCtx) {
    self->skelAnime.moveFlags |= 3;
    AnimationContext_SetMoveActor(globalCtx, &self->actor, &self->skelAnime, 1.0f);
}

void DemoEc_UpdateEyes(DemoEc* self) {
    s32 pad[3];
    s16* blinkTimer = &self->blinkTimer;
    s16* eyeTexIndex = &self->eyeTexIndex;

    if (DECR(*blinkTimer) == 0) {
        *blinkTimer = Rand_S16Offset(60, 60);
    }

    *eyeTexIndex = *blinkTimer;

    if (*eyeTexIndex >= 3) {
        *eyeTexIndex = 0;
    }
}

void DemoEc_SetEyeTexIndex(DemoEc* self, s16 texIndex) {
    self->eyeTexIndex = texIndex;
}

void DemoEc_InitSkelAnime(DemoEc* self, GlobalContext* globalCtx, FlexSkeletonHeader* skeletonHeader) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, SEGMENTED_TO_VIRTUAL(skeletonHeader), NULL, NULL, NULL, 0);
}

void DemoEc_ChangeAnimation(DemoEc* self, AnimationHeader* animation, u8 mode, f32 transitionRate, s32 reverse) {
    f32 frameCount;
    f32 startFrame;
    AnimationHeader* anim;
    f32 playbackSpeed;
    s16 frameCountS;

    anim = SEGMENTED_TO_VIRTUAL(animation);
    frameCountS = Animation_GetLastFrame(anim);

    if (!reverse) {
        startFrame = 0.0f;
        frameCount = frameCountS;
        playbackSpeed = 1.0f;
    } else {
        frameCount = 0.0f;
        startFrame = frameCountS;
        playbackSpeed = -1.0f;
    }

    Animation_Change(&self->skelAnime, anim, playbackSpeed, startFrame, frameCount, mode, transitionRate);
}

Gfx* DemoEc_AllocColorDList(GraphicsContext* gfxCtx, u8* color) {
    Gfx* dList;

    dList = Graph_Alloc(gfxCtx, sizeof(Gfx) * 2);
    gDPSetEnvColor(dList, color[0], color[1], color[2], color[3]);
    gSPEndDisplayList(dList + 1);

    return dList;
}

void DemoEc_DrawSkeleton(DemoEc* self, GlobalContext* globalCtx, void* eyeTexture, void* arg3,
                         OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    SkelAnime* skelAnime = &self->skelAnime;
    s32 pad;

    OPEN_DISPS(gfxCtx, "../z_demo_ec.c", 565);

    func_80093D18(gfxCtx);

    if (eyeTexture != 0) {
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTexture));
    }

    if (arg3 != 0) {
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(arg3));
    }

    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);
    POLY_OPA_DISP = SkelAnime_DrawFlex(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       overrideLimbDraw, postLimbDraw, &self->actor, POLY_OPA_DISP);
    CLOSE_DISPS(gfxCtx, "../z_demo_ec.c", 595);
}

void DemoEc_DrawSkeletonCustomColor(DemoEc* self, GlobalContext* globalCtx, Gfx* arg2, Gfx* arg3, u8* color1,
                                    u8* color2, OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw) {
    s32 pad;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(gfxCtx, "../z_demo_ec.c", 609);

    func_80093D18(gfxCtx);

    if (arg2 != 0) {
        gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(arg2));
    }

    if (arg3 != 0) {
        gSPSegment(POLY_OPA_DISP++, 0x0B, SEGMENTED_TO_VIRTUAL(arg3));
    }

    if (color1 != NULL) {
        //! @bug DemoEc_AllocColorDList is called twice in SEGMENTED_TO_VIRTUAL, allocating two display lists
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(DemoEc_AllocColorDList(gfxCtx, color1)));
    }

    if (color2 != NULL) {
        //! @bug DemoEc_AllocColorDList is called twice in SEGMENTED_TO_VIRTUAL, allocating two display lists
        //! @bug meant to pass color2 instead of color1?
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(DemoEc_AllocColorDList(gfxCtx, color1)));
    }

    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);
    POLY_OPA_DISP = SkelAnime_DrawFlex(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       overrideLimbDraw, postLimbDraw, &self->actor, POLY_OPA_DISP);

    CLOSE_DISPS(gfxCtx, "../z_demo_ec.c", 646);
}

void DemoEc_UseDrawObject(DemoEc* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s32 drawObjBankIndex = self->drawObjBankIndex;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;

    OPEN_DISPS(gfxCtx, "../z_demo_ec.c", 662);

    gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[drawObjBankIndex].segment);
    gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[drawObjBankIndex].segment);
    if (!globalCtx) {}

    CLOSE_DISPS(gfxCtx, "../z_demo_ec.c", 670);
}

void DemoEc_UseAnimationObject(DemoEc* self, GlobalContext* globalCtx) {
    s32 animObjBankIndex = self->animObjBankIndex;

    gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[animObjBankIndex].segment);
}

CsCmdActorAction* DemoEc_GetNpcAction(GlobalContext* globalCtx, s32 actionIndex) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        return globalCtx->csCtx.npcActions[actionIndex];
    } else {
        return NULL;
    }
}

void DemoEc_SetNpcActionPosRot(DemoEc* self, GlobalContext* globalCtx, s32 actionIndex) {
    CsCmdActorAction* npcAction = DemoEc_GetNpcAction(globalCtx, actionIndex);

    if (npcAction != NULL) {
        self->actor.world.pos.x = npcAction->startPos.x;
        self->actor.world.pos.y = npcAction->startPos.y;
        self->actor.world.pos.z = npcAction->startPos.z;

        self->actor.world.rot.y = self->actor.shape.rot.y = npcAction->rot.y;
    }
}

void DemoEc_InitIngo(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_in_Skel_013B88);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcIngoAnim, 0, 0.0f, false);
    func_8096D64C(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_INGO;
    self->drawConfig = EC_DRAW_INGO;
}

void DemoEc_UpdateIngo(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawIngo(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_DrawSkeleton(self, globalCtx, &object_in_Tex_004390, &object_in_Tex_004350, 0, 0);
}

void DemoEc_InitTalon(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_ta_Skel_00B7B8);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcTalonAnim, 0, 0.0f, false);
    func_8096D64C(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_TALON;
    self->drawConfig = EC_DRAW_TALON;
}

void DemoEc_UpdateTalon(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawTalon(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_DrawSkeleton(self, globalCtx, &object_ta_Tex_0076C0, &object_ta_Tex_007AC0, NULL, NULL);
}

void DemoEc_InitWindmillMan(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gWindmillManSkel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcWindmillManAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_WINDMILL_MAN;
    self->drawConfig = EC_DRAW_WINDMILL_MAN;
}

void DemoEc_UpdateWindmillMan(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawWindmillMan(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_DrawSkeleton(self, globalCtx, &gWindmillManEyeClosedTex, &gWindmillManMouthAngryTex, NULL, NULL);
}

void DemoEc_InitKokiriBoy(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gKm1Skel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcKokiriAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_KOKIRI_BOY;
    self->drawConfig = EC_DRAW_KOKIRI_BOY;
}

void DemoEc_InitDancingKokiriBoy(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gKm1Skel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcDancingKokiriAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_DANCING_KOKIRI_BOY;
    self->drawConfig = EC_DRAW_KOKIRI_BOY;
}

void DemoEc_UpdateKokiriBoy(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_UpdateDancingKokiriBoy(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateKokiriBoy(self, globalCtx);
}

void DemoEc_DrawKokiriBoy(DemoEc* self, GlobalContext* globalCtx) {
    static u8 color1[] = { 0, 130, 70, 255 };
    static u8 color2[] = { 110, 170, 20, 255 };

    DemoEc_DrawSkeletonCustomColor(self, globalCtx, NULL, NULL, color1, color2, NULL, NULL);
}

void DemoEc_InitKokiriGirl(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gKw1Skel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcKokiriAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_KOKIRI_GIRL;
    self->drawConfig = EC_DRAW_KOKIRI_GIRL;
}

void DemoEc_InitDancingKokiriGirl(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gKw1Skel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcDancingKokiriAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_DANCING_KOKIRI_GIRL;
    self->drawConfig = EC_DRAW_KOKIRI_GIRL;
}

void DemoEc_UpdateKokiriGirl(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_UpdateDancingKokiriGirl(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateKokiriGirl(self, globalCtx);
}

void DemoEc_DrawKokiriGirl(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x06000F4C, 0x06001A0C, 0x06001E0C };
    static u8 color1[] = { 70, 190, 60, 255 };
    static u8 color2[] = { 100, 30, 0, 255 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeletonCustomColor(self, globalCtx, eyeTexture, NULL, color1, color2, NULL, NULL);
}
void DemoEc_InitOldMan(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_bji_Skel_0000F0);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcOldManAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_OLD_MAN;
    self->drawConfig = EC_DRAW_OLD_MAN;
}

void DemoEc_UpdateOldMan(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawOldMan(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x060005FC, 0x060009FC, 0x06000DFC };
    static u8 color1[] = { 0, 50, 100, 255 };
    static u8 color2[] = { 0, 50, 160, 255 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeletonCustomColor(self, globalCtx, eyeTexture, NULL, color1, color2, NULL, NULL);
}

void DemoEc_InitBeardedMan(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_ahg_Skel_0000F0);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcOldManAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_BEARDED_MAN;
    self->drawConfig = EC_DRAW_BEARDED_MAN;
}

void DemoEc_UpdateBeardedMan(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawBeardedMan(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x0600057C, 0x0600067C, 0x0600077C };
    static u8 color1[] = { 255, 255, 255, 255 };
    static u8 color2[] = { 255, 255, 255, 255 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeletonCustomColor(self, globalCtx, eyeTexture, NULL, color1, color2, NULL, NULL);
}

void DemoEc_InitWoman(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_bob_Skel_0000F0);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcOldManAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_WOMAN;
    self->drawConfig = EC_DRAW_WOMAN;
}

void DemoEc_UpdateWoman(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawWoman(DemoEc* self, GlobalContext* globalCtx) {
    static Gfx* eyeTextures[] = { 0x060007C8, 0x06000FC8, 0x060017C8 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, NULL);
}

void DemoEc_InitOldWoman(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_bba_Skel_0000F0);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcOldManAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_OLD_WOMAN;
    self->drawConfig = EC_DRAW_OLD_WOMAN;
}

void DemoEc_UpdateOldWoman(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawOldWoman(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_DrawSkeleton(self, globalCtx, &object_bba_Tex_0004C8, NULL, NULL, NULL);
}

void DemoEc_InitBossCarpenter(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_toryo_Skel_007150);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcCarpenterAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_BOSS_CARPENTER;
    self->drawConfig = EC_DRAW_BOSS_CARPENTER;
}

void DemoEc_UpdateBossCarpenter(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawBossCarpenter(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_DrawSkeleton(self, globalCtx, NULL, NULL, NULL, NULL);
}

void DemoEc_InitCarpenter(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_daiku_Skel_007958);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcCarpenterAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_CARPENTER;
    self->drawConfig = EC_DRAW_CARPENTER;
}

void DemoEc_UpdateCarpenter(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

s32 DemoEc_CarpenterOverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                     void* thisx, Gfx** gfx) {
    DemoEc* self = THIS;

    if (limbIndex == 1) {
        gDPPipeSync((*gfx)++);
        switch (self->actor.params) {
            case 10:
                gDPSetEnvColor((*gfx)++, 170, 10, 70, 255);
                break;
            case 11:
                gDPSetEnvColor((*gfx)++, 170, 200, 255, 255);
                break;
            case 12:
                gDPSetEnvColor((*gfx)++, 0, 230, 70, 255);
                break;
            case 13:
                gDPSetEnvColor((*gfx)++, 200, 0, 150, 255);
                break;
        }
    }

    return false;
}

Gfx* DemoEc_GetCarpenterPostLimbDList(DemoEc* self) {
    switch (self->actor.params) {
        case 10:
            return object_daiku_DL_005BD0;
        case 11:
            return object_daiku_DL_005AC0;
        case 12:
            return object_daiku_DL_005990;
        case 13:
            return object_daiku_DL_005880;
        default:
            osSyncPrintf(VT_FGCOL(RED) "かつらが無い!!!!!!!!!!!!!!!!\n" VT_RST);
            return 0;
    }
}

void DemoEc_CarpenterPostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx,
                                  Gfx** gfx) {
    DemoEc* self = THIS;
    Gfx* postLimbDList;

    if (limbIndex == 15) {
        postLimbDList = DemoEc_GetCarpenterPostLimbDList(self);
        gSPDisplayList((*gfx)++, SEGMENTED_TO_VIRTUAL(postLimbDList));
    }
}

void DemoEc_DrawCarpenter(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_DrawSkeleton(self, globalCtx, NULL, 0, DemoEc_CarpenterOverrideLimbDraw, DemoEc_CarpenterPostLimbDraw);
}

void DemoEc_InitGerudo(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gGerudoWhiteSkel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcGerudoAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_GERUDO;
    self->drawConfig = EC_DRAW_GERUDO;
}

void DemoEc_UpdateGerudo(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

Gfx* DemoEc_GetGerudoPostLimbDList(DemoEc* self) {
    switch (self->actor.params) {
        case 16:
            return gGerudoWhiteHairstyleBobDL;
        case 17:
            return gGerudoWhiteHairstyleStraightFringeDL;
        case 18:
            return gGerudoWhiteHairstyleSpikyDL;
        default:
            osSyncPrintf(VT_FGCOL(RED) "かつらが無い!!!!!!!!!!!!!!!!\n" VT_RST);
            return NULL;
    }
}

void DemoEc_GerudoPostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx,
                               Gfx** gfx) {
    DemoEc* self = THIS;
    Gfx* postLimbDList;

    if (limbIndex == 15) {
        postLimbDList = DemoEc_GetGerudoPostLimbDList(self);
        gSPDisplayList((*gfx)++, SEGMENTED_TO_VIRTUAL(postLimbDList));
    }
}

void DemoEc_DrawGerudo(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x06000708, 0x06000F08, 0x06001708 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, DemoEc_GerudoPostLimbDraw);
}

void DemoEc_InitDancingZora(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gZoraSkel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcDancingZoraAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_DANCING_ZORA;
    self->drawConfig = EC_DRAW_DANCING_ZORA;
}

void DemoEc_UpdateDancingZora(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawDancingZora(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { gZoraEyeOpenTex, gZoraEyeHalfTex, gZoraEyeClosedTex };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, NULL);
}

void DemoEc_InitKingZora(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_kz_Skel_0086D0);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcKingZoraAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_KING_ZORA;
    self->drawConfig = EC_DRAW_KING_ZORA;
    DemoEc_SetEyeTexIndex(self, 3);
}

void func_8096F1D4(DemoEc* self) {
    f32 currentFrame = self->skelAnime.curFrame;

    if (currentFrame <= 32.0f) {
        DemoEc_SetEyeTexIndex(self, 3);
    } else {
        DemoEc_UpdateEyes(self);
    }
}

void func_8096F224(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcAnim_006930, 2, -8.0f, false);
    self->updateMode = EC_UPDATE_17;
}

void func_8096F26C(DemoEc* self, s32 arg1) {
    if (arg1 != 0) {
        DemoEc_ChangeAnimation(self, &gDemoEcAnim_006220, 0, 0.0f, false);
        self->updateMode = EC_UPDATE_18;
    }
}

void func_8096F2B0(DemoEc* self, GlobalContext* globalCtx, s32 arg2) {
    CsCmdActorAction* npcAction;
    s32 sp18;

    npcAction = DemoEc_GetNpcAction(globalCtx, arg2);

    if (npcAction != NULL) {
        sp18 = npcAction->action;
        if ((sp18 != self->npcAction)) {
            if (self->npcAction) {}
            if (sp18 == 2) {
                func_8096F224(self, globalCtx);
            }
            self->npcAction = sp18;
        }
    }
}

void DemoEc_UpdateKingZora(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_SetNpcActionPosRot(self, globalCtx, 6);
    DemoEc_UpdateBgFlags(self, globalCtx);
    func_8096F2B0(self, globalCtx, 6);
}

void func_8096F378(DemoEc* self, GlobalContext* globalCtx) {
    s32 animDone = DemoEc_UpdateSkelAnime(self);

    func_8096D594(self, globalCtx);
    func_8096F1D4(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
    func_8096F26C(self, animDone);
}

void func_8096F3D4(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawKingZora(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x06001470, 0x06001870, 0x06001C70, 0x06002070 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, NULL);
}

void DemoEc_InitMido(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gMidoSkel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcMidoAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_MIDO;
    self->drawConfig = EC_DRAW_MIDO;
    DemoEc_SetEyeTexIndex(self, 3);
}

void func_8096F4FC(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcAnim_008D1C, 2, -8.0f, false);
    self->updateMode = EC_UPDATE_20;
}

void func_8096F544(DemoEc* self, s32 changeAnim) {
    if (changeAnim) {
        DemoEc_ChangeAnimation(self, &gDemoEcAnim_009234, 0, 0.0f, false);
    }
}

void func_8096F578(DemoEc* self, GlobalContext* globalCtx, s32 arg2) {
    CsCmdActorAction* npcAction;
    s32 sp18;
    npcAction = DemoEc_GetNpcAction(globalCtx, arg2);
    if (npcAction != NULL) {
        sp18 = npcAction->action;
        if ((sp18 != self->npcAction)) {
            if (self->npcAction) {}
            if (sp18 == 2) {
                func_8096F4FC(self, globalCtx);
            }
            self->npcAction = sp18;
        }
    }
}

void DemoEc_UpdateMido(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_SetNpcActionPosRot(self, globalCtx, 7);
    DemoEc_UpdateBgFlags(self, globalCtx);
    func_8096F578(self, globalCtx, 7);
}

void func_8096F640(DemoEc* self, GlobalContext* globalCtx) {
    s32 animDone = DemoEc_UpdateSkelAnime(self);

    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
    func_8096F544(self, animDone);
}

void DemoEc_DrawMido(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x06004FF0, 0x06005930, 0x06005D30, 0x06006130 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, NULL);
}

void DemoEc_InitCucco(DemoEc* self, GlobalContext* globalCtx) {
    AnimationHeader* animation;

    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gCuccoSkel);
    DemoEc_UseAnimationObject(self, globalCtx);

    if (self->actor.params == 22) {
        animation = &gDemoEcJumpingCuccoAnim;
    } else if (self->actor.params == 23) {
        animation = &gDemoEcJumpingCucco2Anim;
    } else {
        animation = &gDemoEcWalkingCuccoAnim;
    }

    DemoEc_ChangeAnimation(self, animation, 0, 0.0f, false);
    func_8096D64C(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_CUCCO;
    self->drawConfig = EC_DRAW_CUCCO;
}

void DemoEc_UpdateCucco(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawCucco(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_DrawSkeleton(self, globalCtx, NULL, NULL, NULL, NULL);
}

void DemoEc_InitCuccoLady(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gCuccoLadySkel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcCuccoLadyAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_CUCCO_LADY;
    self->drawConfig = EC_DRAW_CUCCO_LADY;
}

void DemoEc_UpdateCuccoLady(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawCuccoLady(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x060008C8, 0x060010C8, 0x060018C8 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, NULL);
}

void DemoEc_InitPotionShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_ds2_Skel_004258);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcPotionShopOwnerAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_POTION_SHOP_OWNER;
    self->drawConfig = EC_DRAW_POTION_SHOP_OWNER;
}

void DemoEc_UpdatePotionShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawPotionShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x060030D8, 0x060034D8, 0x060038D8 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, NULL);
}

void DemoEc_InitMaskShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_os_Skel_004658);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcPotionShopOwnerAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_MASK_SHOP_OWNER;
    self->drawConfig = EC_DRAW_MASK_SHOP_OWNER;
}

void DemoEc_UpdateMaskShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawMaskShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_DrawSkeleton(self, globalCtx, &gOsEyeClosedTex, NULL, NULL, NULL);
}

void DemoEc_InitFishingOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gFishingOwnerSkel);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcPotionShopOwnerAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_FISHING_MAN;
    self->drawConfig = EC_DRAW_FISHING_MAN;
}

void DemoEc_UpdateFishingOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_FishingOwnerPostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx,
                                     Gfx** gfx) {
    DemoEc* self = THIS;

    if ((limbIndex == 8) && !(HIGH_SCORE(HS_FISHING) & 0x1000)) {
        gSPDisplayList((*gfx)++, SEGMENTED_TO_VIRTUAL(gFishingOwnerHatDL));
    }
}

void DemoEc_DrawFishingOwner(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = {
        gFishingOwnerEyeOpenTex,
        gFishingOwnerEyeHalfTex,
        gFishingOwnerEyeClosedTex,
    };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, DemoEc_FishingOwnerPostLimbDraw);
}

void DemoEc_InitBombchuShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_rs_Skel_004868);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &gDemoEcPotionShopOwnerAnim, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_BOMBCHU_SHOP_OWNER;
    self->drawConfig = EC_DRAW_BOMBCHU_SHOP_OWNER;
}

void DempEc_UpdateBombchuShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawBombchuShopOwner(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { gBombchuShopkeeperEyeOpenTex, gBombchuShopkeeperEyeHalfTex,
                                   gBombchuShopkeeperEyeClosedTex };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, NULL, NULL, NULL);
}

void DemoEc_InitGorons(DemoEc* self, GlobalContext* globalCtx) {
    s32 pad[2];
    AnimationHeader* animation;
    f32 goronScale;
    Vec3f* scale = &self->actor.scale;

    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &gGoronSkel);
    DemoEc_UseAnimationObject(self, globalCtx);

    if (self->actor.params == 30) {
        animation = &gDemoEcGoronAnim;
        goronScale = 1.0f;
    } else if (self->actor.params == 31) {
        animation = &gDemoEcGoron2Anim;
        goronScale = 1.0f;
    } else if (self->actor.params == 32) {
        animation = &gDemoEcGoronAnim;
        goronScale = 15.0f;
    } else {
        goronScale = 5.0f;
        animation = &object_gm_Anim_0002B8;
    }

    DemoEc_ChangeAnimation(self, animation, 0, 0.0f, false);

    scale->x *= goronScale;
    scale->y *= goronScale;
    scale->z *= goronScale;

    func_8096D64C(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_GORON;
    self->drawConfig = EC_DRAW_GORON;
}

void DemoEc_UpdateGorons(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawGorons(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { gGoronCsEyeOpenTex, gGoronCsEyeHalfTex, gGoronCsEyeClosedTex };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, &gGoronCsMouthNeutralTex, NULL, NULL);
}

void DemoEc_InitMalon(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UseDrawObject(self, globalCtx);
    DemoEc_InitSkelAnime(self, globalCtx, &object_ma2_Skel_008D90);
    DemoEc_UseAnimationObject(self, globalCtx);
    DemoEc_ChangeAnimation(self, &object_ma2_Anim_009EE0, 0, 0.0f, false);
    func_8096D5D4(self, globalCtx);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    self->updateMode = EC_UPDATE_MALON;
    self->drawConfig = EC_DRAW_MALON;
}

void DemoEc_UpdateMalon(DemoEc* self, GlobalContext* globalCtx) {
    DemoEc_UpdateSkelAnime(self);
    func_8096D594(self, globalCtx);
    DemoEc_UpdateEyes(self);
    DemoEc_UpdateBgFlags(self, globalCtx);
}

void DemoEc_DrawMalon(DemoEc* self, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { object_ma2_Tex_002570, object_ma2_Tex_002C70, object_ma2_Tex_003070 };
    s32 eyeTexIndex = self->eyeTexIndex;
    void* eyeTexture = eyeTextures[eyeTexIndex];

    DemoEc_DrawSkeleton(self, globalCtx, eyeTexture, &object_ma2_Tex_003770, NULL, NULL);
}

static DemoEcInitFunc sInitFuncs[] = {
    /*  0 */ DemoEc_InitIngo,
    /*  1 */ DemoEc_InitTalon,
    /*  2 */ DemoEc_InitWindmillMan,
    /*  3 */ DemoEc_InitKokiriBoy,
    /*  4 */ DemoEc_InitKokiriGirl,
    /*  5 */ DemoEc_InitOldMan,
    /*  6 */ DemoEc_InitBeardedMan,
    /*  7 */ DemoEc_InitWoman,
    /*  8 */ DemoEc_InitOldWoman,
    /*  9 */ DemoEc_InitBossCarpenter,
    /* 10 */ DemoEc_InitCarpenter,
    /* 11 */ DemoEc_InitCarpenter,
    /* 12 */ DemoEc_InitCarpenter,
    /* 13 */ DemoEc_InitCarpenter,
    /* 14 */ DemoEc_InitDancingKokiriBoy,
    /* 15 */ DemoEc_InitDancingKokiriGirl,
    /* 16 */ DemoEc_InitGerudo,
    /* 17 */ DemoEc_InitGerudo,
    /* 18 */ DemoEc_InitGerudo,
    /* 19 */ DemoEc_InitDancingZora,
    /* 20 */ DemoEc_InitKingZora,
    /* 21 */ DemoEc_InitMido,
    /* 22 */ DemoEc_InitCucco,
    /* 23 */ DemoEc_InitCucco,
    /* 24 */ DemoEc_InitCucco,
    /* 25 */ DemoEc_InitCuccoLady,
    /* 26 */ DemoEc_InitPotionShopOwner,
    /* 27 */ DemoEc_InitMaskShopOwner,
    /* 28 */ DemoEc_InitFishingOwner,
    /* 29 */ DemoEc_InitBombchuShopOwner,
    /* 30 */ DemoEc_InitGorons,
    /* 31 */ DemoEc_InitGorons,
    /* 32 */ DemoEc_InitGorons,
    /* 33 */ DemoEc_InitGorons,
    /* 34 */ DemoEc_InitMalon,
};

void DemoEc_InitNpc(DemoEc* self, GlobalContext* globalCtx) {
    s16 type = self->actor.params;

    if (sInitFuncs[type] == NULL) {
        // "Demo_Ec_main_init: Initialization process is wrong arg_data"
        osSyncPrintf(VT_FGCOL(RED) " Demo_Ec_main_init:初期化処理がおかしいarg_data = %d!\n" VT_RST, type);
        Actor_Kill(&self->actor);
        return;
    }

    sInitFuncs[type](self, globalCtx);
}

void DemoEc_InitCommon(DemoEc* self, GlobalContext* globalCtx) {
    s32 pad;
    s16 primary;
    s32 type;
    s16 pad2;
    s16 sp28;
    s32 primaryBankIndex;
    s32 secondaryBankIndex;

    type = self->actor.params;
    primary = sDrawObjects[type];
    sp28 = sAnimationObjects[type];
    primaryBankIndex = Object_GetIndex(&globalCtx->objectCtx, primary);
    secondaryBankIndex = Object_GetIndex(&globalCtx->objectCtx, sp28);

    if ((secondaryBankIndex < 0) || (primaryBankIndex < 0)) {
        // "Demo_Ec_main_bank: Bank unreadable arg_data = %d!"
        osSyncPrintf(VT_FGCOL(RED) "Demo_Ec_main_bank:バンクを読めない arg_data = %d!\n" VT_RST, type);
        Actor_Kill(&self->actor);
        return;
    }

    if (Object_IsLoaded(&globalCtx->objectCtx, primaryBankIndex) &&
        Object_IsLoaded(&globalCtx->objectCtx, secondaryBankIndex)) {

        self->drawObjBankIndex = primaryBankIndex;
        self->animObjBankIndex = secondaryBankIndex;

        DemoEc_InitNpc(self, globalCtx);
    }
}

static DemoEcUpdateFunc sUpdateFuncs[] = {
    DemoEc_InitCommon,
    DemoEc_UpdateIngo,
    DemoEc_UpdateTalon,
    DemoEc_UpdateWindmillMan,
    DemoEc_UpdateKokiriBoy,
    DemoEc_UpdateKokiriGirl,
    DemoEc_UpdateOldMan,
    DemoEc_UpdateBeardedMan,
    DemoEc_UpdateWoman,
    DemoEc_UpdateOldWoman,
    DemoEc_UpdateBossCarpenter,
    DemoEc_UpdateCarpenter,
    DemoEc_UpdateDancingKokiriBoy,
    DemoEc_UpdateDancingKokiriGirl,
    DemoEc_UpdateGerudo,
    DemoEc_UpdateDancingZora,
    DemoEc_UpdateKingZora,
    func_8096F378,
    func_8096F3D4,
    DemoEc_UpdateMido,
    func_8096F640,
    DemoEc_UpdateCucco,
    DemoEc_UpdateCuccoLady,
    DemoEc_UpdatePotionShopOwner,
    DemoEc_UpdateMaskShopOwner,
    DemoEc_UpdateFishingOwner,
    DempEc_UpdateBombchuShopOwner,
    DemoEc_UpdateGorons,
    DemoEc_UpdateMalon,
};

void DemoEc_Update(Actor* thisx, GlobalContext* globalCtx) {
    DemoEc* self = THIS;
    s32 updateMode = self->updateMode;

    if ((updateMode < 0) || (updateMode >= ARRAY_COUNT(sUpdateFuncs)) || sUpdateFuncs[updateMode] == NULL) {
        // "The main mode is strange !!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        if (updateMode != EC_UPDATE_COMMON) {
            DemoEc_UseAnimationObject(self, globalCtx);
        }
        sUpdateFuncs[updateMode](self, globalCtx);
    }
}

void DemoEc_DrawCommon(DemoEc* self, GlobalContext* globalCtx) {
}

static DemoEcDrawFunc sDrawFuncs[] = {
    DemoEc_DrawCommon,          DemoEc_DrawIngo,
    DemoEc_DrawTalon,           DemoEc_DrawWindmillMan,
    DemoEc_DrawKokiriBoy,       DemoEc_DrawKokiriGirl,
    DemoEc_DrawOldMan,          DemoEc_DrawBeardedMan,
    DemoEc_DrawWoman,           DemoEc_DrawOldWoman,
    DemoEc_DrawBossCarpenter,   DemoEc_DrawCarpenter,
    DemoEc_DrawGerudo,          DemoEc_DrawDancingZora,
    DemoEc_DrawKingZora,        DemoEc_DrawMido,
    DemoEc_DrawCucco,           DemoEc_DrawCuccoLady,
    DemoEc_DrawPotionShopOwner, DemoEc_DrawMaskShopOwner,
    DemoEc_DrawFishingOwner,    DemoEc_DrawBombchuShopOwner,
    DemoEc_DrawGorons,          DemoEc_DrawMalon,
};

void DemoEc_Draw(Actor* thisx, GlobalContext* globalCtx) {
    DemoEc* self = THIS;
    s32 drawConfig = self->drawConfig;

    if ((drawConfig < 0) || (drawConfig >= ARRAY_COUNT(sDrawFuncs)) || sDrawFuncs[drawConfig] == NULL) {
        // "The main mode is strange !!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        if (drawConfig != EC_DRAW_COMMON) {
            DemoEc_UseDrawObject(self, globalCtx);
        }
        sDrawFuncs[drawConfig](self, globalCtx);
    }
}

const ActorInit Demo_Ec_InitVars = {
    ACTOR_DEMO_EC,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_EC,
    sizeof(DemoEc),
    (ActorFunc)DemoEc_Init,
    (ActorFunc)DemoEc_Destroy,
    (ActorFunc)DemoEc_Update,
    (ActorFunc)DemoEc_Draw,
};
