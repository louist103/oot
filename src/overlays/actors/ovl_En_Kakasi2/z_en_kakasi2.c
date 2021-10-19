/*
 * File: z_en_kakasi2.c
 * Overlay: ovl_En_Kakasi2
 * Description: Pierre the Scarecrow Spawn
 */

#include "z_en_kakasi2.h"
#include "vt.h"
#include "objects/object_ka/object_ka.h"

#define FLAGS 0x0A000031

#define THIS ((EnKakasi2*)thisx)

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 70, 0, { 0, 0, 0 } },
};

void EnKakasi2_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKakasi2_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKakasi2_Update(Actor* thisx, GlobalContext* globalCtx);
void func_80A90948(Actor* thisx, GlobalContext* globalCtx);

void func_80A9062C(EnKakasi2* self, GlobalContext* globalCtx);
void func_80A90264(EnKakasi2* self, GlobalContext* globalCtx);
void func_80A904D8(EnKakasi2* self, GlobalContext* globalCtx);
void func_80A90578(EnKakasi2* self, GlobalContext* globalCtx);
void func_80A906C4(EnKakasi2* self, GlobalContext* globalCtx);

const ActorInit En_Kakasi2_InitVars = {
    ACTOR_EN_KAKASI2,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_KA,
    sizeof(EnKakasi2),
    (ActorFunc)EnKakasi2_Init,
    (ActorFunc)EnKakasi2_Destroy,
    (ActorFunc)EnKakasi2_Update,
    NULL,
};

void EnKakasi2_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi2* self = THIS;
    s32 pad;
    f32 spawnRangeY;
    f32 spawnRangeXZ;

    osSyncPrintf("\n\n");
    // "Visit Umeda"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 梅田参号見参！ ☆☆☆☆☆ \n" VT_RST);

    self->switchFlag = self->actor.params & 0x3F;
    spawnRangeY = (self->actor.params >> 6) & 0xFF;
    spawnRangeXZ = self->actor.world.rot.z;
    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    self->actor.targetMode = 4;
    self->maxSpawnDistance.x = (spawnRangeY * 40.0f) + 40.0f;
    self->maxSpawnDistance.y = (spawnRangeXZ * 40.0f) + 40.0f;

    // "Former? (Argument 0)"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 元？(引数０) ☆☆☆☆ %f\n" VT_RST, spawnRangeY);
    // "Former? (Z angle)"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 元？(Ｚアングル) ☆☆ %f\n" VT_RST, spawnRangeXZ);
    // "Correction coordinates X"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 補正座標Ｘ ☆☆☆☆☆ %f\n" VT_RST, self->maxSpawnDistance.x);
    // "Correction coordinates Y"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 補正座標Ｙ ☆☆☆☆☆ %f\n" VT_RST, self->maxSpawnDistance.y);
    // "Correction coordinates Z"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 補正座標Ｚ ☆☆☆☆☆ %f\n" VT_RST, self->maxSpawnDistance.z);
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ SAVE       ☆☆☆☆☆ %d\n" VT_RST, self->switchFlag);
    osSyncPrintf("\n\n");

    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->height = 60.0f;
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.flags |= 0x400;
    self->unk_198 = self->actor.shape.rot.y;

    if (self->switchFlag >= 0 && Flags_GetSwitch(globalCtx, self->switchFlag)) {
        self->actor.draw = func_80A90948;
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_ka_Skel_0065B0, &object_ka_Anim_000214, NULL, NULL, 0);
        self->actionFunc = func_80A9062C;
    } else {
        self->actionFunc = func_80A90264;
        self->actor.shape.yOffset = -8000.0f;
    }
}

void EnKakasi2_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi2* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
    //! @bug SkelAnime_Free is not called
}

void func_80A90264(EnKakasi2* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_194++;

    if ((BREG(1) != 0) && (self->actor.xzDistToPlayer < self->maxSpawnDistance.x) &&
        (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < self->maxSpawnDistance.y)) {

        self->actor.draw = func_80A90948;
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_ka_Skel_0065B0, &object_ka_Anim_000214, NULL, NULL, 0);
        OnePointCutscene_Attention(globalCtx, &self->actor);
        self->actor.flags |= 0x8000001;

        func_80078884(NA_SE_SY_CORRECT_CHIME);
        if (self->switchFlag >= 0) {
            Flags_SetSwitch(globalCtx, self->switchFlag);
        }

        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ SAVE 終了 ☆☆☆☆☆ %d\n" VT_RST, self->switchFlag);
        self->actionFunc = func_80A904D8;
    } else if ((self->actor.xzDistToPlayer < self->maxSpawnDistance.x) &&
               (fabsf(player->actor.world.pos.y - self->actor.world.pos.y) < self->maxSpawnDistance.y) &&
               (gSaveContext.eventChkInf[9] & 0x1000)) {

        self->unk_194 = 0;
        if (globalCtx->msgCtx.unk_E3EE == 11) {
            if (self->switchFlag >= 0) {
                Flags_SetSwitch(globalCtx, self->switchFlag);
            }
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ SAVE 終了 ☆☆☆☆☆ %d\n" VT_RST, self->switchFlag);
            globalCtx->msgCtx.unk_E3EE = 4;
            self->actor.draw = func_80A90948;
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_ka_Skel_0065B0, &object_ka_Anim_000214, NULL, NULL,
                               0);
            OnePointCutscene_Attention(globalCtx, &self->actor);
            func_80078884(NA_SE_SY_CORRECT_CHIME);

            self->actor.flags |= 0x8000001;
            self->actionFunc = func_80A904D8;
        }
    }
}

void func_80A904D8(EnKakasi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&object_ka_Anim_000214);

    Animation_Change(&self->skelAnime, &object_ka_Anim_000214, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_COME_UP_DEKU_JR);
    self->actionFunc = func_80A90578;
}

void func_80A90578(EnKakasi2* self, GlobalContext* globalCtx) {
    s16 currentFrame;

    SkelAnime_Update(&self->skelAnime);

    currentFrame = self->skelAnime.curFrame;
    if (currentFrame == 11 || currentFrame == 17) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_KAKASHI_SWING);
    }

    self->actor.shape.rot.y += 0x800;
    Math_ApproachZeroF(&self->actor.shape.yOffset, 0.5f, 500.0f);

    if (self->actor.shape.yOffset > -100.0f) {
        self->actionFunc = func_80A9062C;
        self->actor.shape.yOffset = 0.0f;
    }
}

void func_80A9062C(EnKakasi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&object_ka_Anim_000214);

    Animation_Change(&self->skelAnime, &object_ka_Anim_000214, 0.0f, 0.0f, (s16)frameCount, ANIMMODE_ONCE, -10.0f);
    self->actionFunc = func_80A906C4;
}

void func_80A906C4(EnKakasi2* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame != 0) {
        Math_ApproachZeroF(&self->skelAnime.curFrame, 0.5f, 1.0f);
    }
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->unk_198, 5, 0xBB8, 0);
    SkelAnime_Update(&self->skelAnime);
}

void EnKakasi2_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnKakasi2* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    self->actor.world.rot = self->actor.shape.rot;
    Actor_SetFocus(&self->actor, self->height);
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);

    if (self->actor.shape.yOffset == 0.0f) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    if (BREG(0) != 0) {
        if (BREG(5) != 0) {
            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ self->actor.player_distance ☆☆☆☆☆ %f\n" VT_RST,
                         self->actor.xzDistToPlayer);
            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ self->hosei.x ☆☆☆☆☆ %f\n" VT_RST, self->maxSpawnDistance.x);
            osSyncPrintf("\n\n");
        }
        if (self->actor.draw == NULL) {
            if (self->unk_194 != 0) {
                if ((self->unk_194 % 2) == 0) {
                    DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                           self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z,
                                           1.0f, 1.0f, 1.0f, 70, 70, 70, 255, 4, globalCtx->state.gfxCtx);
                }
            } else {
                DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                       self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 0, 255, 255, 255, 4, globalCtx->state.gfxCtx);
            }
        }
    }
}

void func_80A90948(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi2* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);
}
