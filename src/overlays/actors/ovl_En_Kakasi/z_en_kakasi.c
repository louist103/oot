/*
 * File: z_en_kakasi.c
 * Overlay: ovl_En_Kakasi
 * Description: Pierre the Scarecrow
 */

#include "z_en_kakasi.h"
#include "vt.h"
#include "objects/object_ka/object_ka.h"

#define FLAGS 0x02000009

#define THIS ((EnKakasi*)thisx)

void EnKakasi_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKakasi_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKakasi_Update(Actor* thisx, GlobalContext* globalCtx);
void EnKakasi_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A8F660(EnKakasi* self, GlobalContext* globalCtx);
void func_80A8F75C(EnKakasi* self, GlobalContext* globalCtx);
void func_80A8F8D0(EnKakasi* self, GlobalContext* globalCtx);
void func_80A8F9C8(EnKakasi* self, GlobalContext* globalCtx);
void func_80A8FBB8(EnKakasi* self, GlobalContext* globalCtx);
void func_80A8FAA4(EnKakasi* self, GlobalContext* globalCtx);

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
        BUMP_NONE | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 70, 0, { 0, 0, 0 } },
};

const ActorInit En_Kakasi_InitVars = {
    ACTOR_EN_KAKASI,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_KA,
    sizeof(EnKakasi),
    (ActorFunc)EnKakasi_Init,
    (ActorFunc)EnKakasi_Destroy,
    (ActorFunc)EnKakasi_Update,
    (ActorFunc)EnKakasi_Draw,
};

void EnKakasi_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
    //! @bug SkelAnime_Free is not called
}

void EnKakasi_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi* self = THIS;

    osSyncPrintf("\n\n");
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ Ｌｅｔ’ｓ ＤＡＮＣＥ！ ☆☆☆☆☆ %f\n" VT_RST, self->actor.world.pos.y);

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.targetMode = 6;
    SkelAnime_InitFlex(globalCtx, &self->skelanime, &object_ka_Skel_0065B0, &object_ka_Anim_000214, NULL, NULL, 0);

    self->rot = self->actor.world.rot;
    self->actor.flags |= 0x400;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;

    Actor_SetScale(&self->actor, 0.01f);
    self->actionFunc = func_80A8F660;
}

void func_80A8F28C(EnKakasi* self) {
    self->unk_1A4 = 0;
    self->skelanime.playSpeed = 0.0f;
    self->unk_1A8 = self->unk_1AC = 0;

    Math_ApproachZeroF(&self->skelanime.curFrame, 0.5f, 1.0f);
    Math_SmoothStepToS(&self->actor.shape.rot.x, self->rot.x, 5, 0x2710, 0);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->rot.y, 5, 0x2710, 0);
    Math_SmoothStepToS(&self->actor.shape.rot.z, self->rot.z, 5, 0x2710, 0);
}

void func_80A8F320(EnKakasi* self, GlobalContext* globalCtx, s16 arg) {
    s16 phi_v0;
    s16 currentFrame;

    phi_v0 = globalCtx->msgCtx.unk_E410;
    if (arg != 0) {
        if (self->unk_19C[3] == 0) {
            self->unk_19C[3] = (s16)Rand_ZeroFloat(10.99f) + 30;
            self->unk_1A6 = (s16)Rand_ZeroFloat(4.99f);
        }

        self->unk_19A = (s16)Rand_ZeroFloat(2.99f) + 5;
        phi_v0 = self->unk_1A6;
    }
    switch (phi_v0) {
        case 0:
            self->unk_19A++;
            if (self->unk_1A4 == 0) {
                self->unk_1A4 = 1;
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_KAKASHI_ROLL);
            }
            break;
        case 1:
            self->unk_19A++;
            self->unk_1B8 = 1.0f;
            break;
        case 2:
            self->unk_19A++;
            if (self->unk_1AC == 0) {
                self->unk_1AC = 0x1388;
            }
            break;
        case 3:
            self->unk_19A++;
            if (self->unk_1A8 == 0) {
                self->unk_1A8 = 0x1388;
            }
            break;
        case 4:
            self->unk_19A++;
            self->unk_1B8 = 2.0f;
            break;
    }

    if (self->unk_19A > 8) {
        self->unk_19A = 8;
    }

    if (self->unk_19A != 0) {
        self->actor.gravity = -1.0f;
        if (self->unk_19A == 8 && (self->actor.bgCheckFlags & 1)) {
            self->actor.velocity.y = 3.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_IT_KAKASHI_JUMP);
        }
        Math_ApproachF(&self->skelanime.playSpeed, self->unk_1B8, 0.1f, 0.2f);
        Math_SmoothStepToS(&self->actor.shape.rot.x, self->unk_1A8, 5, 0x3E8, 0);
        Math_SmoothStepToS(&self->actor.shape.rot.z, self->unk_1AC, 5, 0x3E8, 0);

        if (self->unk_1A8 != 0 && fabsf(self->actor.shape.rot.x - self->unk_1A8) < 50.0f) {
            self->unk_1A8 *= -1.0f;
        }
        if (self->unk_1AC != 0 && fabsf(self->actor.shape.rot.z - self->unk_1AC) < 50.0f) {
            self->unk_1AC *= -1.0f;
        }

        if (self->unk_1A4 != 0) {
            self->actor.shape.rot.y += 0x1000;
            if (self->actor.shape.rot.y == 0) {
                self->unk_1A4 = 0;
            }
        }
        currentFrame = self->skelanime.curFrame;
        if (currentFrame == 11 || currentFrame == 17) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_KAKASHI_SWING);
        }
        SkelAnime_Update(&self->skelanime);
    }
}

void func_80A8F660(EnKakasi* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&object_ka_Anim_000214);

    Animation_Change(&self->skelanime, &object_ka_Anim_000214, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);

    self->actor.textId = 0x4076;
    self->unk_196 = 6;
    if (!LINK_IS_ADULT) {
        self->unk_194 = false;
        if (gSaveContext.scarecrowCustomSongSet) {
            self->actor.textId = 0x407A;
            self->unk_196 = 5;
        }
    } else {
        self->unk_194 = true;
        if (gSaveContext.scarecrowCustomSongSet) {
            self->actor.textId = 0x4079;
            self->unk_196 = 5;
        }
    }
    self->actionFunc = func_80A8F75C;
}

void func_80A8F75C(EnKakasi* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    func_80A8F28C(self);
    SkelAnime_Update(&self->skelanime);
    self->camId = SUBCAM_NONE;
    if (func_8002F194(&self->actor, globalCtx)) {
        if (self->unk_196 == 5) {
            self->actionFunc = func_80A8F9C8;
        } else {
            self->actionFunc = func_80A8F660;
        }
    } else {
        s16 yawTowardsPlayer = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if (!(self->actor.xzDistToPlayer > 120.0f)) {
            s16 absyawTowardsPlayer = ABS(yawTowardsPlayer);

            if (absyawTowardsPlayer < 0x4300) {
                if (!self->unk_194) {
                    if (player->stateFlags2 & 0x1000000) {
                        self->camId = OnePointCutscene_Init(globalCtx, 2260, -99, &self->actor, MAIN_CAM);

                        func_8010BD58(globalCtx, 0x2A);
                        self->unk_19A = 0;
                        self->unk_1B8 = 0.0;
                        player->stateFlags2 |= 0x800000;
                        self->actionFunc = func_80A8F8D0;
                        return;
                    }
                    if (self->actor.xzDistToPlayer < 80.0f) {
                        player->stateFlags2 |= 0x800000;
                    }
                }
                func_8002F2CC(&self->actor, globalCtx, 100.0f);
            }
        }
    }
}

void func_80A8F8D0(EnKakasi* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (globalCtx->msgCtx.unk_E3EE == 4 && globalCtx->msgCtx.msgMode == 0) {
        // "end?"
        osSyncPrintf(VT_FGCOL(BLUE) "☆☆☆☆☆ 終り？ ☆☆☆☆☆ \n" VT_RST);

        if (self->unk_19A != 0) {
            func_80106CCC(globalCtx);
            self->actor.textId = 0x4077;
            self->unk_196 = 5;
            func_8010B680(globalCtx, self->actor.textId, NULL);
            self->actionFunc = func_80A8F9C8;
        } else {
            OnePointCutscene_EndCutscene(globalCtx, self->camId);
            self->camId = SUBCAM_NONE;
            self->actionFunc = func_80A8F660;
        }
    } else if (globalCtx->msgCtx.unk_E3EE == 1) {
        func_80A8F320(self, globalCtx, 0);
        player->stateFlags2 |= 0x800000;
    }
}

void func_80A8F9C8(EnKakasi* self, GlobalContext* globalCtx) {
    func_80A8F28C(self);
    SkelAnime_Update(&self->skelanime);
    func_8002DF54(globalCtx, NULL, 8);

    if (self->unk_196 == func_8010BDBC(&globalCtx->msgCtx) && (func_80106BC8(globalCtx) != 0)) {

        if (self->camId != SUBCAM_NONE) {
            func_8005B1A4(globalCtx->cameraPtrs[self->camId]);
        }
        self->camId = OnePointCutscene_Init(globalCtx, 2270, -99, &self->actor, MAIN_CAM);
        globalCtx->msgCtx.msgMode = 0x37;
        func_8002DF54(globalCtx, NULL, 8);
        func_8010BD58(globalCtx, 0x2B);
        self->actionFunc = func_80A8FAA4;
    }
}

void func_80A8FAA4(EnKakasi* self, GlobalContext* globalCtx) {
    if (globalCtx->msgCtx.unk_E3EE != 0xF) {
        func_80A8F320(self, globalCtx, 1);
        return;
    }

    osSyncPrintf("game_play->message.msg_mode=%d\n", globalCtx->msgCtx.msgMode);

    if (globalCtx->msgCtx.msgMode == 0) {
        if (self->unk_194) {
            self->actor.textId = 0x4077;
            self->unk_196 = 5;
            func_8010B680(globalCtx, self->actor.textId, NULL);
        } else {
            self->actor.textId = 0x4078;
            self->unk_196 = 5;
            func_8010B680(globalCtx, self->actor.textId, NULL);
        }
        self->actionFunc = func_80A8FBB8;
        OnePointCutscene_EndCutscene(globalCtx, self->camId);
        self->camId = SUBCAM_NONE;
        self->camId = OnePointCutscene_Init(globalCtx, 2260, -99, &self->actor, MAIN_CAM);
        func_8005B1A4(globalCtx->cameraPtrs[self->camId]);
    }
}

void func_80A8FBB8(EnKakasi* self, GlobalContext* globalCtx) {
    func_80A8F28C(self);
    SkelAnime_Update(&self->skelanime);

    if (self->unk_196 == func_8010BDBC(&globalCtx->msgCtx)) {
        if (func_80106BC8(globalCtx) != 0) {
            func_8005B1A4(globalCtx->cameraPtrs[self->camId]);
            func_80106CCC(globalCtx);
            func_8002DF54(globalCtx, NULL, 7);
            self->actionFunc = func_80A8F660;
        }
    }
}

void EnKakasi_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi* self = THIS;
    s32 pad;
    s32 i;

    self->unk_198++;
    self->actor.world.rot = self->actor.shape.rot;
    for (i = 0; i < ARRAY_COUNT(self->unk_19C); i++) {
        if (self->unk_19C[i] != 0) {
            self->unk_19C[i]--;
        }
    }

    self->height = 60.0f;
    Actor_SetFocus(&self->actor, self->height);
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 50.0f, 100.0f, 28);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnKakasi_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi* self = THIS;

    if (BREG(3) != 0) {
        osSyncPrintf("\n\n");
        // "flag!"
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ フラグ！ ☆☆☆☆☆ %d\n" VT_RST, gSaveContext.scarecrowCustomSongSet);
    }
    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelanime.skeleton, self->skelanime.jointTable, self->skelanime.dListCount,
                          NULL, NULL, self);
}
