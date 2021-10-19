/*
 * File: z_en_kakasi3.c
 * Overlay: ovl_En_Kakasi3
 * Description: Bonooru the Scarecrow
 */

#include "z_en_kakasi3.h"
#include "vt.h"
#include "objects/object_ka/object_ka.h"

#define FLAGS 0x02000009

#define THIS ((EnKakasi3*)thisx)

void EnKakasi3_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKakasi3_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKakasi3_Update(Actor* thisx, GlobalContext* globalCtx);
void EnKakasi3_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A911F0(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A91284(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A91348(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A915B8(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A91620(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A91760(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A917FC(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A9187C(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A918E4(EnKakasi3* self, GlobalContext* globalCtx);
void func_80A91A90(EnKakasi3* self, GlobalContext* globalCtx);

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

const ActorInit En_Kakasi3_InitVars = {
    ACTOR_EN_KAKASI3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_KA,
    sizeof(EnKakasi3),
    (ActorFunc)EnKakasi3_Init,
    (ActorFunc)EnKakasi3_Destroy,
    (ActorFunc)EnKakasi3_Update,
    (ActorFunc)EnKakasi3_Draw,
};

void EnKakasi3_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi3* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
    //! @bug SkelAnime_Free is not called
}

void EnKakasi3_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi3* self = THIS;

    osSyncPrintf("\n\n");
    // "Obonur" -- Related to the name of the scarecrow (Bonooru)
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ おーボヌール ☆☆☆☆☆ \n" VT_RST);
    self->actor.targetMode = 6;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_ka_Skel_0065B0, &object_ka_Anim_000214, NULL, NULL, 0);
    self->actor.flags |= 0x400;
    self->rot = self->actor.world.rot;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&self->actor, 0.01f);
    self->actionFunc = func_80A911F0;
}

void func_80A90E28(EnKakasi3* self) {
    self->unk_1A4 = 0;
    self->skelAnime.playSpeed = 0.0f;
    self->unk_1AA = self->unk_1AE = 0x0;

    Math_ApproachZeroF(&self->skelAnime.curFrame, 0.5f, 1.0f);
    Math_SmoothStepToS(&self->actor.shape.rot.x, self->rot.x, 5, 0x2710, 0);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->rot.y, 5, 0x2710, 0);
    Math_SmoothStepToS(&self->actor.shape.rot.z, self->rot.z, 5, 0x2710, 0);
}

void func_80A90EBC(EnKakasi3* self, GlobalContext* globalCtx, s32 arg) {
    s16 currentFrame;
    s16 phi_v0;

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
            if (self->unk_1AE == 0x0) {
                self->unk_1AE = 0x1388;
            }
            break;
        case 3:
            self->unk_19A++;
            if (self->unk_1AA == 0x0) {
                self->unk_1AA = 0x1388;
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
        Math_ApproachF(&self->skelAnime.playSpeed, self->unk_1B8, 0.1f, 0.2f);
        Math_SmoothStepToS(&self->actor.shape.rot.x, self->unk_1AA, 0x5, 0x3E8, 0);
        Math_SmoothStepToS(&self->actor.shape.rot.z, self->unk_1AE, 0x5, 0x3E8, 0);

        if (self->unk_1AA != 0x0 && fabsf(self->actor.shape.rot.x - self->unk_1AA) < 50.0f) {
            self->unk_1AA *= -1.0f;
        }
        if (self->unk_1AE != 0x0 && fabsf(self->actor.shape.rot.z - self->unk_1AE) < 50.0f) {
            self->unk_1AE *= -1.0f;
        }

        if (self->unk_1A4 != 0) {
            self->actor.shape.rot.y += 0x1000;
            if (self->actor.shape.rot.y == 0) {
                self->unk_1A4 = 0;
            }
        }
        currentFrame = self->skelAnime.curFrame;
        if (currentFrame == 11 || currentFrame == 17) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_KAKASHI_SWING);
        }
        SkelAnime_Update(&self->skelAnime);
    }
}

void func_80A911F0(EnKakasi3* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&object_ka_Anim_000214);

    Animation_Change(&self->skelAnime, &object_ka_Anim_000214, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = func_80A91284;
}

void func_80A91284(EnKakasi3* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    self->actor.textId = 0x40A1;
    self->dialogState = 6;
    self->unk_19A = 0;

    if (!LINK_IS_ADULT) {
        self->unk_194 = false;
        if (gSaveContext.scarecrowSpawnSongSet) {
            self->actor.textId = 0x40A0;
            self->dialogState = 5;
            self->unk_1A8 = 1;
        }
    } else {
        self->unk_194 = true;
        if (gSaveContext.scarecrowSpawnSongSet) {
            if (self->unk_195) {
                self->actor.textId = 0x40A2;
            } else {
                self->actor.textId = 0x40A3;
            }
        }
    }
    self->actionFunc = func_80A91348;
}

void func_80A91348(EnKakasi3* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    func_80A90E28(self);
    SkelAnime_Update(&self->skelAnime);
    self->camId = SUBCAM_NONE;
    if (func_8002F194(&self->actor, globalCtx)) {
        if (!self->unk_194) {
            if (self->unk_1A8 == 0) {
                self->actionFunc = func_80A91284;
            } else {
                self->actionFunc = func_80A91760;
            }
        } else {
            self->actionFunc = func_80A91284;
        }
    } else {
        s16 angleTowardsLink = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if (!(self->actor.xzDistToPlayer > 120.0f)) {
            s16 absAngleTowardsLink = ABS(angleTowardsLink);

            if (absAngleTowardsLink < 0x4300) {
                if (!self->unk_194) {

                    if (player->stateFlags2 & 0x1000000) {
                        self->camId = OnePointCutscene_Init(globalCtx, 2260, -99, &self->actor, MAIN_CAM);
                        globalCtx->msgCtx.msgMode = 0x37;
                        self->dialogState = 5;
                        self->unk_1B8 = 0.0f;
                        func_8010B680(globalCtx, 0x40A4, NULL);
                        player->stateFlags2 |= 0x800000;
                        self->actionFunc = func_80A915B8;
                        return;
                    }
                    if (self->actor.xzDistToPlayer < 80.0f) {
                        player->stateFlags2 |= 0x800000;
                    }
                } else if (gSaveContext.scarecrowSpawnSongSet && !self->unk_195) {

                    if (player->stateFlags2 & 0x1000000) {
                        self->camId = OnePointCutscene_Init(globalCtx, 2260, -99, &self->actor, MAIN_CAM);
                        globalCtx->msgCtx.msgMode = 0x37;
                        self->dialogState = 5;
                        self->unk_1B8 = 0.0f;
                        func_8010B680(globalCtx, 0x40A8, NULL);
                        player->stateFlags2 |= 0x800000;
                        self->actionFunc = func_80A9187C;
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

void func_80A915B8(EnKakasi3* self, GlobalContext* globalCtx) {

    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);
        func_8010BD58(globalCtx, 0x2C);
        self->actionFunc = func_80A91620;
    }
}

void func_80A91620(EnKakasi3* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((globalCtx->msgCtx.unk_E3EE == 4 || (globalCtx->msgCtx.unk_E3EE >= 5 && globalCtx->msgCtx.unk_E3EE < 11)) &&
        (globalCtx->msgCtx.msgMode == 0)) {

        OnePointCutscene_EndCutscene(globalCtx, self->camId);
        if (globalCtx->cameraPtrs[self->camId] == NULL) {
            self->camId = SUBCAM_NONE;
        }
        if (self->camId != SUBCAM_NONE) {
            func_8005B1A4(globalCtx->cameraPtrs[self->camId]);
        }
        self->actionFunc = func_80A911F0;
        return;
    }

    if (globalCtx->msgCtx.unk_E3EE == 3 && globalCtx->msgCtx.msgMode == 0) {
        self->dialogState = 5;
        func_8010B680(globalCtx, 0x40A5, NULL);
        func_8002DF54(globalCtx, NULL, 8);
        self->actionFunc = func_80A91A90;
        return;
    }

    if (globalCtx->msgCtx.unk_E3EE == 1) {
        func_80A90EBC(self, globalCtx, 0);
        player->stateFlags2 |= 0x800000;
    }
}

void func_80A91760(EnKakasi3* self, GlobalContext* globalCtx) {

    func_80A90E28(self);
    SkelAnime_Update(&self->skelAnime);
    if (self->dialogState == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx)) {
        globalCtx->msgCtx.msgMode = 0x37;
        func_8010BD58(globalCtx, 0x2D);
        self->actionFunc = func_80A917FC;
        self->camId = OnePointCutscene_Init(globalCtx, 2280, -99, &self->actor, MAIN_CAM);
    }
}

void func_80A917FC(EnKakasi3* self, GlobalContext* globalCtx) {

    if (globalCtx->msgCtx.unk_E3EE != 15) {
        func_80A90EBC(self, globalCtx, 1);
    } else {
        globalCtx->msgCtx.unk_E3EE = 4;
        func_80106CCC(globalCtx);
        OnePointCutscene_EndCutscene(globalCtx, self->camId);
        self->actionFunc = func_80A911F0;
    }
}

void func_80A9187C(EnKakasi3* self, GlobalContext* globalCtx) {

    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);
        func_8010BD58(globalCtx, 0x28);
        self->actionFunc = func_80A918E4;
    }
}

void func_80A918E4(EnKakasi3* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (BREG(3) != 0) {
        // "No way!"
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ まさか！ ☆☆☆☆☆ %d\n" VT_RST, globalCtx->msgCtx.unk_E3EE);
    }
    if ((globalCtx->msgCtx.unk_E3EE == 4 || (globalCtx->msgCtx.unk_E3EE >= 5 && globalCtx->msgCtx.unk_E3EE < 11)) &&
        globalCtx->msgCtx.msgMode == 0) {

        func_8010B680(globalCtx, 0x40A6, NULL);
        self->dialogState = 5;
        OnePointCutscene_EndCutscene(globalCtx, self->camId);
        self->camId = SUBCAM_NONE;
        func_8002DF54(globalCtx, NULL, 8);
        self->actionFunc = func_80A91A90;
        return;
    }

    if (globalCtx->msgCtx.unk_E3EE == 3 && globalCtx->msgCtx.msgMode == 0) {
        globalCtx->msgCtx.unk_E3EE = 4;
        if (BREG(3) != 0) {
            osSyncPrintf("\n\n");
            // "With self, other guys are OK! That's it!"
            osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ これで、他の奴もＯＫ！だ！ ☆☆☆☆☆ %d\n" VT_RST,
                         globalCtx->msgCtx.unk_E3EE);
        }
        self->unk_195 = true;
        func_8010B680(globalCtx, 0x40A7, NULL);
        self->dialogState = 5;
        func_8002DF54(globalCtx, NULL, 8);
        self->actionFunc = func_80A91A90;
        return;
    }

    if (globalCtx->msgCtx.unk_E3EE == 1) {
        func_80A90EBC(self, globalCtx, 0);
        player->stateFlags2 |= 0x800000;
    }
}

void func_80A91A90(EnKakasi3* self, GlobalContext* globalCtx) {
    func_80A90E28(self);
    SkelAnime_Update(&self->skelAnime);
    func_8002DF54(globalCtx, NULL, 8);

    if (self->dialogState == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx)) {
        if (self->unk_195) {
            if (!(gSaveContext.eventChkInf[9] & 0x1000)) {
                gSaveContext.eventChkInf[9] |= 0x1000;
            }
        }
        if (globalCtx->cameraPtrs[self->camId] == NULL) {
            self->camId = SUBCAM_NONE;
        }
        if (self->camId != SUBCAM_NONE) {
            func_8005B1A4(globalCtx->cameraPtrs[self->camId]);
        }
        func_80106CCC(globalCtx);
        globalCtx->msgCtx.unk_E3EE = 4;
        func_8002DF54(globalCtx, NULL, 7);
        self->actionFunc = func_80A911F0;
    }
}

void EnKakasi3_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi3* self = THIS;
    s32 pad;
    s32 i;

    if (BREG(2) != 0) {
        osSyncPrintf("\n\n");
        // "flag!"
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ フラグ！ ☆☆☆☆☆ %d\n" VT_RST, gSaveContext.scarecrowSpawnSongSet);
    }

    self->unk_198++;
    self->actor.world.rot = self->actor.shape.rot;
    for (i = 0; i < ARRAY_COUNT(self->unk_19C); i++) {
        if (self->unk_19C[i] != 0) {
            self->unk_19C[i]--;
        }
    }

    Actor_SetFocus(&self->actor, 60.0f);
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 50.0f, 50.0f, 100.0f, 28);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnKakasi3_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnKakasi3* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);
}
