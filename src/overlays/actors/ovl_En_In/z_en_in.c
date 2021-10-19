#include "z_en_in.h"
#include "overlays/actors/ovl_En_Horse/z_en_horse.h"
#include "objects/object_in/object_in.h"

#define FLAGS 0x00000019

#define THIS ((EnIn*)thisx)

void EnIn_Init(Actor* thisx, GlobalContext* globalCtx);
void EnIn_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnIn_Update(Actor* thisx, GlobalContext* globalCtx);
void EnIn_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A79FB0(EnIn* self, GlobalContext* globalCtx);
void func_80A7A304(EnIn* self, GlobalContext* globalCtx);
void func_80A7A4C8(EnIn* self, GlobalContext* globalCtx);
void func_80A7A568(EnIn* self, GlobalContext* globalCtx);
void func_80A7A848(EnIn* self, GlobalContext* globalCtx);
void func_80A7ABD4(EnIn* self, GlobalContext* globalCtx);
void func_80A7AEF0(EnIn* self, GlobalContext* globalCtx);
void func_80A7B018(EnIn* self, GlobalContext* globalCtx);
void func_80A7B024(EnIn* self, GlobalContext* globalCtx);
void func_80A7AE84(EnIn* self, GlobalContext* globalCtx);
void func_80A7A770(EnIn* self, GlobalContext* globalCtx);
void func_80A7A940(EnIn* self, GlobalContext* globalCtx);
void func_80A7AA40(EnIn* self, GlobalContext* globalCtx);
void func_80A7A4BC(EnIn* self, GlobalContext* globalCtx);

const ActorInit En_In_InitVars = {
    ACTOR_EN_IN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_IN,
    sizeof(EnIn),
    (ActorFunc)EnIn_Init,
    (ActorFunc)EnIn_Destroy,
    (ActorFunc)EnIn_Update,
    (ActorFunc)EnIn_Draw,
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
    { 18, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = {
    0, 0, 0, 0, MASS_IMMOVABLE,
};

static struct_D_80AA1678 sAnimationInfo[] = {
    { &object_in_Anim_001CC0, 1.0f, ANIMMODE_LOOP, 0.0f }, { &object_in_Anim_001CC0, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &object_in_Anim_013C6C, 1.0f, ANIMMODE_LOOP, 0.0f }, { &object_in_Anim_013C6C, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &object_in_Anim_000CB0, 1.0f, ANIMMODE_LOOP, 0.0f }, { &object_in_Anim_0003B4, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &object_in_Anim_001BE0, 1.0f, ANIMMODE_LOOP, 0.0f }, { &object_in_Anim_013D60, 1.0f, ANIMMODE_LOOP, 0.0f },
    { &object_in_Anim_01431C, 1.0f, ANIMMODE_LOOP, 0.0f }, { &object_in_Anim_014CA8, 1.0f, ANIMMODE_LOOP, 0.0f },
};

static AnimationHeader* D_80A7B918[] = {
    &object_in_Anim_0151C8, &object_in_Anim_015DF0, &object_in_Anim_016B3C, &object_in_Anim_015814,
    &object_in_Anim_01646C, &object_in_Anim_0175D0, &object_in_Anim_017B58, &object_in_Anim_018C38,
};

static Gfx* sAlternateOutfitDLs[] = {
    NULL,
    NULL,
    object_in_DL_013688,
    object_in_DL_0137C0,
    object_in_DL_013910,
    object_in_DL_0132B8,
    object_in_DL_0133F0,
    object_in_DL_013540,
    object_in_DL_013100,
    object_in_DL_011758,
    object_in_DL_012AC0,
    object_in_DL_012BF0,
    object_in_DL_012D20,
    object_in_DL_012480,
    object_in_DL_0125B0,
    object_in_DL_0126E0,
    object_in_DL_011AD8,
    object_in_DL_012350,
    object_in_DL_012220,
    object_in_DL_012120,
};

u16 func_80A78FB0(GlobalContext* globalCtx) {
    if (gSaveContext.eventChkInf[1] & 0x10) {
        if (gSaveContext.infTable[9] & 0x80) {
            return 0x2046;
        } else {
            return 0x2045;
        }
    }
    if (gSaveContext.infTable[9] & 0x10) {
        return 0x2040;
    } else {
        return 0x203F;
    }
}

u16 func_80A79010(GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    u16 temp_v0 = Text_GetFaceReaction(globalCtx, 25);

    if (temp_v0 != 0) {
        return temp_v0;
    }
    if (gSaveContext.eventChkInf[1] & 0x100) {
        if (IS_DAY) {
            return 0x205F;
        } else {
            return 0x2057;
        }
    }
    if (IS_NIGHT) {
        return 0x204E;
    }
    switch (gSaveContext.eventInf[0] & 0xF) {
        case 1:
            if (!(player->stateFlags1 & 0x800000)) {
                return 0x2036;
            } else if (gSaveContext.eventChkInf[1] & 0x800) {
                if (gSaveContext.infTable[10] & 4) {
                    return 0x2036;
                } else {
                    return 0x2038;
                }
            } else {
                return 0x2037;
            }
        case 3:
            if ((gSaveContext.eventInf[0] & 0x40) || (gSaveContext.eventInf[0] & 0x20)) {
                return 0x203E;
            } else {
                return 0x203D;
            }
        case 4:
            return 0x203A;
        case 5:
        case 6:
            return 0x203C;
        case 7:
            return 0x205B;
        case 2:
        default:
            if (gSaveContext.infTable[0x9] & 0x400) {
                return 0x2031;
            } else {
                return 0x2030;
            }
    }
}

u16 func_80A79168(GlobalContext* globalCtx, Actor* thisx) {
    u16 temp_v0 = Text_GetFaceReaction(globalCtx, 25);

    if (temp_v0 != 0) {
        return temp_v0;
    }
    if (!LINK_IS_ADULT) {
        return func_80A78FB0(globalCtx);
    } else {
        return func_80A79010(globalCtx);
    }
}

s16 func_80A791CC(GlobalContext* globalCtx, Actor* thisx) {
    s32 temp_var = 0;
    switch (thisx->textId) {
        case 0x2045:
            gSaveContext.infTable[9] |= 0x80;
            break;
        case 0x203E:
            temp_var = 2;
            break;
        case 0x203F:
            gSaveContext.eventChkInf[1] |= 2;
            gSaveContext.infTable[9] |= 0x10;
            break;
    }
    return temp_var;
}

s16 func_80A7924C(GlobalContext* globalCtx, Actor* thisx) {
    EnIn* self = THIS;
    s32 sp18 = 1;

    switch (self->actor.textId) {
        case 0x2030:
        case 0x2031:
            if (globalCtx->msgCtx.choiceIndex == 1) {
                self->actor.textId = 0x2032;
            } else if (gSaveContext.rupees < 10) {
                self->actor.textId = 0x2033;
            } else {
                self->actor.textId = 0x2034;
            }
            func_8010B720(globalCtx, self->actor.textId);
            gSaveContext.infTable[9] |= 0x400;
            break;
        case 0x2034:
            if (globalCtx->msgCtx.choiceIndex == 1) {
                Rupees_ChangeBy(-10);
                self->actor.textId = 0x205C;
            } else {
                self->actor.textId = 0x2035;
            }
            func_8010B720(globalCtx, self->actor.textId);
            break;
        case 0x2036:
        case 0x2037:
            if (globalCtx->msgCtx.choiceIndex == 1) {
                sp18 = 2;
            } else {
                self->actor.textId = 0x201F;
                func_8010B720(globalCtx, self->actor.textId);
            }
            break;
        case 0x2038:
            if (globalCtx->msgCtx.choiceIndex == 0 && gSaveContext.rupees >= 50) {
                sp18 = 2;
            } else {
                self->actor.textId = 0x2039;
                func_8010B720(globalCtx, self->actor.textId);
                gSaveContext.infTable[10] |= 4;
            }
            break;
        case 0x205B:
            if (globalCtx->msgCtx.choiceIndex == 0 && gSaveContext.rupees >= 50) {
                sp18 = 2;
            } else {
                func_8010B720(globalCtx, self->actor.textId = 0x2039);
                gSaveContext.eventInf[0] &= ~0xF;
                gSaveContext.eventInf[0] &= ~0x20;
                gSaveContext.eventInf[0] &= ~0x40;
                self->actionFunc = func_80A7A4C8;
            }
            break;
    }
    if (!gSaveContext.rupees) {}

    return sp18;
}

s16 func_80A7949C(GlobalContext* globalCtx, Actor* thisx) {
    s32 phi_v1 = 1;
    if (thisx->textId == 0x2035) {
        Rupees_ChangeBy(-10);
        thisx->textId = 0x205C;
        func_8010B720(globalCtx, thisx->textId);
    } else {
        phi_v1 = 2;
    }
    return phi_v1;
}

s16 func_80A79500(GlobalContext* globalCtx, Actor* thisx) {
    s16 sp1E = 1;

    osSyncPrintf("message_check->(%d[%x])\n", func_8010BDBC(&globalCtx->msgCtx), thisx->textId);
    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 0:
        case 1:
            break;
        case 2:
            sp1E = func_80A791CC(globalCtx, thisx);
            break;
        case 3:
            break;
        case 4:
            if (func_80106BC8(globalCtx) != 0) {
                sp1E = func_80A7924C(globalCtx, thisx);
            }
            break;
        case 5:
            if (func_80106BC8(globalCtx) != 0) {
                sp1E = func_80A7949C(globalCtx, thisx);
            }
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            break;
    }
    return sp1E;
}

void func_80A795C8(EnIn* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 phi_a3;

    if (self->skelAnime.animation == &object_in_Anim_0003B4 || self->skelAnime.animation == &object_in_Anim_001BE0 ||
        self->skelAnime.animation == &object_in_Anim_013D60) {
        phi_a3 = 1;
    } else {
        phi_a3 = 0;
    }
    if (self->actionFunc == func_80A7A568) {
        phi_a3 = 4;
    }
    if (self->actionFunc == func_80A7B024) {
        self->unk_308.unk_18 = globalCtx->view.eye;
        self->unk_308.unk_14 = 60.0f;
    } else {
        self->unk_308.unk_18 = player->actor.world.pos;
        self->unk_308.unk_14 = 16.0f;
    }
    func_80034A14(&self->actor, &self->unk_308, 1, phi_a3);
}

void func_80A79690(SkelAnime* skelAnime, EnIn* self, GlobalContext* globalCtx) {
    if (skelAnime->baseTransl.y < skelAnime->jointTable[0].y) {
        skelAnime->moveFlags |= 3;
        AnimationContext_SetMoveActor(globalCtx, &self->actor, skelAnime, 1.0f);
    }
}

void func_80A796EC(EnIn* self, s32 arg1) {
    Animation_Change(&self->skelAnime, sAnimationInfo[arg1].animation, 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationInfo[arg1].animation), sAnimationInfo[arg1].mode,
                     sAnimationInfo[arg1].transitionRate);
}

s32 func_80A7975C(EnIn* self, GlobalContext* globalCtx) {
    if (self->actor.params != 1 || self->actor.shape.rot.z != 1 || !LINK_IS_ADULT) {
        return 0;
    }
    self->animationIdx = 1;
    self->collider.base.ocFlags1 &= ~OC1_ON;
    Animation_Change(&self->skelAnime, D_80A7B918[self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(D_80A7B918[self->animationIdx]), 2, 0.0f);
    self->actionFunc = func_80A7A304;
    return 1;
}

s32 func_80A79830(EnIn* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_SPOT20 && LINK_IS_CHILD && IS_DAY && self->actor.shape.rot.z == 1 &&
        !(gSaveContext.eventChkInf[1] & 0x10)) {
        return 1;
    }
    if (globalCtx->sceneNum == SCENE_MALON_STABLE && LINK_IS_CHILD && IS_DAY && self->actor.shape.rot.z == 3 &&
        (gSaveContext.eventChkInf[1] & 0x10)) {
        return 1;
    }
    if (globalCtx->sceneNum == SCENE_MALON_STABLE && LINK_IS_CHILD && IS_NIGHT) {
        if ((self->actor.shape.rot.z == 2) && !(gSaveContext.eventChkInf[1] & 0x10)) {
            return 1;
        }
        if ((self->actor.shape.rot.z == 4) && (gSaveContext.eventChkInf[1] & 0x10)) {
            return 1;
        }
    }
    if (globalCtx->sceneNum == SCENE_SPOT20 && LINK_IS_ADULT && IS_DAY) {
        if ((self->actor.shape.rot.z == 5) && !(gSaveContext.eventChkInf[1] & 0x100)) {
            return 2;
        }
        if ((self->actor.shape.rot.z == 7) && (gSaveContext.eventChkInf[1] & 0x100)) {
            return 4;
        }
    }
    if (globalCtx->sceneNum == SCENE_SOUKO && LINK_IS_ADULT && IS_NIGHT) {
        if (self->actor.shape.rot.z == 6 && !(gSaveContext.eventChkInf[1] & 0x100)) {
            return 3;
        }
        if (self->actor.shape.rot.z == 8 && (gSaveContext.eventChkInf[1] & 0x100)) {
            return 3;
        }
    }
    return 0;
}

void func_80A79A2C(EnIn* self) {
    s16 phi_v1;

    if (self->unk_1EE != 3) {
        if (self->unk_1EA == 0) {
            phi_v1 = 0;
        } else {
            phi_v1 = --self->unk_1EA;
        }
        if (phi_v1 == 0) {
            self->unk_1EE++;
            if (self->unk_1EE >= 3) {
                self->unk_1EA = Rand_S16Offset(30, 30);
                self->unk_1EE = 0;
            }
        }
    }
}

void func_80A79AB4(EnIn* self, GlobalContext* globalCtx) {
    s32 i;
    u32 f = 0;

    if (self->skelAnime.animation != &object_in_Anim_014CA8) {
        f = globalCtx->gameplayFrames;
    }
    for (i = 0; i < ARRAY_COUNT(self->unk_330); i++) {
        self->unk_330[i].y = (2068 + 50 * i) * f;
        self->unk_330[i].z = (2368 + 50 * i) * f;
    }
}

void func_80A79BAC(EnIn* self, GlobalContext* globalCtx, s32 index, u32 arg3) {
    s16 entrances[] = { 0x0558, 0x04CA, 0x0157 };

    globalCtx->nextEntranceIndex = entrances[index];
    if (index == 2) {
        gSaveContext.nextCutsceneIndex = 0xFFF0;
    }
    globalCtx->fadeTransition = arg3;
    globalCtx->sceneLoadFlag = 0x14;
    func_8002DF54(globalCtx, &self->actor, 8);
    Interface_ChangeAlpha(1);
    if (index == 0) {
        AREG(6) = 0;
    }
    gSaveContext.timer1State = 0;
}

void func_80A79C78(EnIn* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f sp48;
    Vec3f sp3C;
    Vec3s zeroVec = { 0, 0, 0 };

    self->camId = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, self->camId, CAM_STAT_ACTIVE);
    sp48.x = self->actor.world.pos.x;
    sp48.y = self->actor.world.pos.y + 60.0f;
    sp48.z = self->actor.world.pos.z;
    sp3C.x = sp48.x;
    sp3C.y = sp48.y - 22.0f;
    sp3C.z = sp48.z + 40.0f;
    Gameplay_CameraSetAtEye(globalCtx, self->camId, &sp48, &sp3C);
    self->actor.shape.rot.y = Math_Vec3f_Yaw(&self->actor.world.pos, &sp3C);
    self->unk_308.unk_08 = zeroVec;
    self->unk_308.unk_0E = zeroVec;
    func_8010B680(globalCtx, 0x2025, NULL);
    self->unk_308.unk_00 = 1;
    player->actor.world.pos = self->actor.world.pos;
    player->actor.world.pos.x += 100.0f * Math_SinS(self->actor.shape.rot.y);
    player->actor.world.pos.z += 100.0f * Math_CosS(self->actor.shape.rot.y);
    if (player->rideActor != NULL) {
        player->rideActor->world.pos = player->actor.world.pos;
        player->rideActor->freezeTimer = 10;
    }
    player->actor.freezeTimer = 10;
    self->actor.flags &= ~1;
    ShrinkWindow_SetVal(0x20);
    Interface_ChangeAlpha(2);
}

static s32 D_80A7B998 = 0;

void EnIn_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnIn* self = THIS;
    RespawnData* respawn = &gSaveContext.respawn[RESPAWN_MODE_DOWN];
    Vec3f respawnPos;

    self->ingoObjBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_IN);
    if (self->ingoObjBankIndex < 0 && self->actor.params > 0) {
        self->actionFunc = NULL;
        Actor_Kill(&self->actor);
        return;
    }
    respawnPos = respawn->pos;
    // hardcoded coords for lon lon entrance
    if (D_80A7B998 == 0 && respawnPos.x == 1107.0f && respawnPos.y == 0.0f && respawnPos.z == -3740.0f) {
        gSaveContext.eventInf[0] = 0;
        D_80A7B998 = 1;
    }
    self->actionFunc = func_80A79FB0;
}

void EnIn_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnIn* self = THIS;

    if (self->actionFunc != NULL && self->actionFunc != func_80A79FB0) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void func_80A79FB0(EnIn* self, GlobalContext* globalCtx) {
    s32 sp3C = 0;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->ingoObjBankIndex) || self->actor.params <= 0) {
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_in_Skel_013B88, NULL, self->jointTable,
                           self->morphTable, 20);
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
        if (func_80A7975C(self, globalCtx)) {
            gSaveContext.eventInf[0] &= ~0x8000;
            return;
        }
        Actor_SetScale(&self->actor, 0.01f);
        self->actor.targetMode = 6;
        self->unk_308.unk_00 = 0;
        self->actionFunc = func_80A7A4BC;

        switch (func_80A79830(self, globalCtx)) {
            case 1:
                func_80A796EC(self, 9);
                self->actionFunc = func_80A7A4BC;
                break;
            case 3:
                func_80A796EC(self, 7);
                self->actionFunc = func_80A7A4BC;
                if (!(gSaveContext.eventChkInf[1] & 0x100)) {
                    self->actor.params = 5;
                }
                break;
            case 4:
                func_80A796EC(self, 8);
                self->unk_1EE = 3;
                self->actionFunc = func_80A7A4BC;
                break;
            case 0:
                Actor_Kill(&self->actor);
                break;
            default:
                switch (gSaveContext.eventInf[0] & 0xF) {
                    case 0:
                    case 2:
                    case 3:
                    case 4:
                    case 7:
                        if (self->actor.params == 2) {
                            sp3C = 1;
                        }
                        break;
                    case 1:
                        if (self->actor.params == 3) {
                            sp3C = 1;
                        }
                        break;
                    case 5:
                    case 6:
                        if (self->actor.params == 4) {
                            sp3C = 1;
                        }
                        break;
                }
                if (sp3C != 1) {
                    Actor_Kill(&self->actor);
                    return;
                }
                switch (gSaveContext.eventInf[0] & 0xF) {
                    case 0:
                    case 2:
                        func_80A796EC(self, 2);
                        self->actionFunc = func_80A7A4C8;
                        gSaveContext.eventInf[0] = 0;
                        break;
                    case 1:
                        self->actor.targetMode = 3;
                        func_80A796EC(self, 2);
                        self->actionFunc = func_80A7A568;
                        func_80088B34(0x3C);
                        break;
                    case 3:
                        func_80A796EC(self, 4);
                        self->actionFunc = func_80A7A770;
                        break;
                    case 4:
                        func_80A796EC(self, 6);
                        self->unk_1EC = 8;
                        self->actionFunc = func_80A7A940;
                        break;
                    case 5:
                    case 6:
                        self->actor.targetMode = 3;
                        func_80A796EC(self, 6);
                        self->unk_1EC = 8;
                        self->actionFunc = func_80A7AA40;
                        break;
                    case 7:
                        func_80A796EC(self, 2);
                        self->actionFunc = func_80A7A848;
                        break;
                }
        }
    }
}

void func_80A7A304(EnIn* self, GlobalContext* globalCtx) {
    if (self->skelAnime.animation == &object_in_Anim_015814 || self->skelAnime.animation == &object_in_Anim_01646C) {
        if (self->skelAnime.curFrame == 8.0f) {
            Audio_PlaySoundRandom(&self->actor.projectedPos, NA_SE_VO_IN_LASH_0,
                                  NA_SE_VO_IN_LASH_1 - NA_SE_VO_IN_LASH_0 + 1);
        }
    }
    if (self->skelAnime.animation == &object_in_Anim_018C38 && self->skelAnime.curFrame == 20.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_VO_IN_CRY_0);
    }
    if (SkelAnime_Update(&self->skelAnime) != 0) {
        self->animationIdx %= 8;
        self->unk_1E8 = self->animationIdx;
        if (self->animationIdx == 3 || self->animationIdx == 4) {
            Audio_PlaySoundGeneral(NA_SE_IT_LASH, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            if (Rand_ZeroOne() < 0.3f) {
                Audio_PlaySoundGeneral(NA_SE_IT_INGO_HORSE_NEIGH, &self->actor.projectedPos, 4, &D_801333E0,
                                       &D_801333E0, &D_801333E8);
            }
        }
        Animation_Change(&self->skelAnime, D_80A7B918[self->animationIdx], 1.0f, 0.0f,
                         Animation_GetLastFrame(D_80A7B918[self->animationIdx]), 2, -10.0f);
    }
}

void func_80A7A4BC(EnIn* self, GlobalContext* globalCtx) {
}

void func_80A7A4C8(EnIn* self, GlobalContext* globalCtx) {
    if (self->unk_308.unk_00 == 2) {
        func_80A79BAC(self, globalCtx, 1, 0x20);
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x000F) | 0x0001;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x8000) | 0x8000;
        gSaveContext.infTable[10] &= ~4;
        func_800775F0(0x40);
        globalCtx->msgCtx.unk_E3E7 = 0;
        globalCtx->msgCtx.msgMode = 0x36;
        self->unk_308.unk_00 = 0;
    }
}

void func_80A7A568(EnIn* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 phi_a2;
    s32 phi_a3;

    if (!(gSaveContext.eventChkInf[1] & 0x800) && (player->stateFlags1 & 0x800000)) {
        gSaveContext.infTable[10] |= 0x800;
    }
    if (gSaveContext.timer1State == 10) {
        Audio_PlaySoundGeneral(NA_SE_SY_FOUND, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        func_80A79C78(self, globalCtx);
        self->actionFunc = func_80A7B024;
        gSaveContext.timer1State = 0;
    } else if (self->unk_308.unk_00 == 2) {
        if (globalCtx->msgCtx.choiceIndex == 0) {
            if (gSaveContext.rupees < 50) {
                globalCtx->msgCtx.unk_E3E7 = 4;
                globalCtx->msgCtx.msgMode = 0x36;
                self->unk_308.unk_00 = 0;
                return;
            }
            gSaveContext.eventInf[0] =
                (gSaveContext.eventInf[0] & ~0x10) | (((EnHorse*)GET_PLAYER(globalCtx)->rideActor)->type << 4);
            gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0xF) | 2;
            phi_a2 = 2;
            phi_a3 = 2;
        } else {
            Audio_PlaySoundGeneral(NA_SE_SY_FOUND, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            if (!(gSaveContext.eventChkInf[1] & 0x800)) {
                if (gSaveContext.infTable[10] & 0x800) {
                    gSaveContext.eventChkInf[1] |= 0x800;
                    gSaveContext.infTable[10] |= 0x800;
                }
            }
            gSaveContext.eventInf[0] &= ~0xF;
            phi_a2 = 0;
            phi_a3 = 0x20;
        }
        func_80A79BAC(self, globalCtx, phi_a2, phi_a3);
        globalCtx->msgCtx.unk_E3E7 = 0;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x8000) | 0x8000;
        globalCtx->msgCtx.msgMode = 0x36;
        self->unk_308.unk_00 = 0;
    }
}

void func_80A7A770(EnIn* self, GlobalContext* globalCtx) {
    if (self->unk_308.unk_00 == 0) {
        self->actor.flags |= 0x10000;
    } else if (self->unk_308.unk_00 == 2) {
        Rupees_ChangeBy(-50);
        self->actor.flags &= ~0x10000;
        func_80A796EC(self, 3);
        self->actionFunc = func_80A7A848;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x0F) | 7;
        self->unk_308.unk_00 = 0;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & 0xFFFF) | 0x20;
        if (!(gSaveContext.eventInf[0] & 0x40)) {
            globalCtx->msgCtx.unk_E3E7 = 4;
            globalCtx->msgCtx.msgMode = 0x36;
        }
    }
}

void func_80A7A848(EnIn* self, GlobalContext* globalCtx) {
    if (self->unk_308.unk_00 == 2) {
        if ((globalCtx->msgCtx.choiceIndex == 0 && gSaveContext.rupees < 50) || globalCtx->msgCtx.choiceIndex == 1) {
            gSaveContext.eventInf[0] &= ~0xF;
            self->actionFunc = func_80A7A4C8;
        } else {
            func_80A79BAC(self, globalCtx, 2, 0x26);
            gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0xF) | 2;
            gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x8000) | 0x8000;
            globalCtx->msgCtx.unk_E3E7 = 0;
            globalCtx->msgCtx.msgMode = 0x36;
        }
        self->unk_308.unk_00 = 0;
        gSaveContext.eventInf[0] &= ~0x20;
        gSaveContext.eventInf[0] &= ~0x40;
    }
}

void func_80A7A940(EnIn* self, GlobalContext* globalCtx) {
    if (self->unk_308.unk_00 == 0) {
        self->actor.flags |= 0x10000;
        return;
    }
    if (self->unk_1EC != 0) {
        self->unk_1EC--;
        if (self->unk_1EC == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_VO_IN_LOST);
        }
    }
    if (self->unk_308.unk_00 == 2) {
        self->actor.flags &= ~0x10000;
        func_80A79BAC(self, globalCtx, 2, 0x26);
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x000F) | 0x0002;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x8000) | 0x8000;
        globalCtx->msgCtx.unk_E3E7 = 0;
        globalCtx->msgCtx.msgMode = 0x36;
        self->unk_308.unk_00 = 0;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & 0xFFFF) | 0x40;
    }
}

void func_80A7AA40(EnIn* self, GlobalContext* globalCtx) {
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    Vec3f sp30;
    Vec3f sp24;

    self->camId = Gameplay_CreateSubCamera(globalCtx);
    self->activeCamId = globalCtx->activeCamera;
    Gameplay_ChangeCameraStatus(globalCtx, self->activeCamId, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, self->camId, CAM_STAT_ACTIVE);

    self->unk_2F0 = 0.0f;
    self->unk_2F4 = 50.0f;
    self->unk_2F8 = 0.0f;
    self->unk_2FC = 0.0f;
    self->unk_300 = 50.0f;
    self->unk_304 = 50.0f;

    sp30 = self->actor.world.pos;
    sp24 = self->actor.world.pos;

    sp30.x += self->unk_2F0;
    sp30.y += self->unk_2F4;
    sp30.z += self->unk_2F8;

    sp24.x += self->unk_2FC;
    sp24.y += self->unk_300;
    sp24.z += self->unk_304;

    Gameplay_CameraSetAtEye(globalCtx, self->camId, &sp30, &sp24);
    self->actor.textId = 0x203B;
    func_8010B680(globalCtx, self->actor.textId, NULL);
    self->unk_308.unk_00 = 1;
    self->unk_1FC = 0;
    globalCtx->csCtx.frames = 0;
    ShrinkWindow_SetVal(0x20);
    Interface_ChangeAlpha(2);
    self->actionFunc = func_80A7ABD4;
}

void func_80A7ABD4(EnIn* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f sp48;
    Vec3f sp3C;

    if (player->rideActor != NULL) {
        player->rideActor->freezeTimer = 10;
    }
    player->actor.freezeTimer = 10;
    if (self->actor.textId == 0x203B) {
        if (self->unk_1EC != 0) {
            self->unk_1EC--;
            if (self->unk_1EC == 0) {
                Audio_PlayActorSound2(&self->actor, NA_SE_VO_IN_LOST);
            }
        }
    }
    if (self->unk_308.unk_00 != 0) {
        if (self->unk_308.unk_00 == 2) {
            if (self->actor.textId == 0x203B) {
                self->actor.textId = 0x203C;
                func_8010B680(globalCtx, self->actor.textId, NULL);
                self->unk_308.unk_00 = 1;
                func_80A796EC(self, 3);
            } else {
                globalCtx->msgCtx.msgMode = 0x36;
                self->unk_308.unk_00 = 0;
            }
        }
    } else {
        if (globalCtx->csCtx.frames++ >= 50) {
            self->actionFunc = func_80A7AE84;
            return;
        }
        if (globalCtx->csCtx.frames == 44) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_RONRON_DOOR_CLOSE);
        }
        Math_SmoothStepToF(&self->unk_2F0, 0.0f, 0.06f, 10000.0f, 0.0f);
        Math_SmoothStepToF(&self->unk_2F4, 50.0f, 0.06f, 10000.0f, 0.0f);
        Math_SmoothStepToF(&self->unk_2F8, 0.0f, 0.06f, 10000.0f, 0.0f);
        Math_SmoothStepToF(&self->unk_2FC, 0.0f, 0.06f, 10000.0f, 0.0f);
        Math_SmoothStepToF(&self->unk_300, 150.0f, 0.06f, 10000.0f, 0.0f);
        Math_SmoothStepToF(&self->unk_304, 300.0f, 0.06f, 10000.0f, 0.0f);

        sp48 = self->actor.world.pos;
        sp3C = self->actor.world.pos;

        sp48.x += self->unk_2F0;
        sp48.y += self->unk_2F4;
        sp48.z += self->unk_2F8;
        sp3C.x += self->unk_2FC;
        sp3C.y += self->unk_300;
        sp3C.z += self->unk_304;
        Gameplay_CameraSetAtEye(globalCtx, self->camId, &sp48, &sp3C);
    }
}

void func_80A7AE84(EnIn* self, GlobalContext* globalCtx) {
    Gameplay_ChangeCameraStatus(globalCtx, self->activeCamId, CAM_STAT_ACTIVE);
    Gameplay_ClearCamera(globalCtx, self->camId);
    func_8002DF54(globalCtx, &self->actor, 7);
    Interface_ChangeAlpha(0x32);
    self->actionFunc = func_80A7AEF0;
}

void func_80A7AEF0(EnIn* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 yaw;
    Vec3f pos = self->actor.world.pos;

    pos.x += 90.0f * Math_SinS(self->actor.shape.rot.y);
    pos.z += 90.0f * Math_CosS(self->actor.shape.rot.y);
    yaw = Math_Vec3f_Yaw(&pos, &player->actor.world.pos);
    if (ABS(yaw) > 0x4000) {
        globalCtx->nextEntranceIndex = 0x0476;
        globalCtx->sceneLoadFlag = 0x14;
        globalCtx->fadeTransition = 5;
        self->actionFunc = func_80A7B018;
    } else if (self->unk_308.unk_00 == 2) {
        globalCtx->msgCtx.unk_E3E7 = 4;
        globalCtx->msgCtx.msgMode = 0x36;
        self->unk_308.unk_00 = 0;
    }
}

void func_80A7B018(EnIn* self, GlobalContext* globalCtx) {
}

void func_80A7B024(EnIn* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (player->rideActor != NULL) {
        player->rideActor->freezeTimer = 10;
    }
    player->actor.freezeTimer = 10;
    if (self->unk_308.unk_00 == 2) {
        if (1) {}
        if (!(gSaveContext.eventChkInf[1] & 0x800) && (gSaveContext.infTable[10] & 0x800)) {
            gSaveContext.eventChkInf[1] |= 0x800;
            gSaveContext.infTable[10] |= 0x800;
        }
        func_80A79BAC(self, globalCtx, 0, 0x26);
        gSaveContext.eventInf[0] = gSaveContext.eventInf[0] & ~0xF;
        gSaveContext.eventInf[0] = (gSaveContext.eventInf[0] & ~0x8000) | 0x8000;
        globalCtx->msgCtx.unk_E3E7 = 4;
        globalCtx->msgCtx.msgMode = 0x36;
        self->unk_308.unk_00 = 0;
    }
}

void EnIn_Update(Actor* thisx, GlobalContext* globalCtx) {
    ColliderCylinder* collider;
    EnIn* self = THIS;

    if (self->actionFunc == func_80A79FB0) {
        self->actionFunc(self, globalCtx);
        return;
    }
    collider = &self->collider;
    Collider_UpdateCylinder(&self->actor, collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &collider->base);
    if (self->actionFunc != func_80A7A304) {
        SkelAnime_Update(&self->skelAnime);
        if (self->skelAnime.animation == &object_in_Anim_001BE0 && ((gSaveContext.eventInf[0] & 0xF) != 6)) {
            func_80A79690(&self->skelAnime, self, globalCtx);
        }
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    }
    func_80A79A2C(self);
    self->actionFunc(self, globalCtx);
    if (self->actionFunc != func_80A7A304) {
        func_80A79AB4(self, globalCtx);
        if (gSaveContext.timer2Value < 6 && gSaveContext.timer2State != 0 && self->unk_308.unk_00 == 0) {
            if (func_8002F194(&self->actor, globalCtx)) {}
        } else {
            func_800343CC(globalCtx, &self->actor, &self->unk_308.unk_00,
                          ((self->actor.targetMode == 6) ? 80.0f : 320.0f) + self->collider.dim.radius, func_80A79168,
                          func_80A79500);
            if (self->unk_308.unk_00 != 0) {
                self->unk_1FA = self->unk_1F8;
                self->unk_1F8 = func_8010BDBC(&globalCtx->msgCtx);
            }
        }
        func_80A795C8(self, globalCtx);
    }
}

s32 EnIn_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnIn* self = THIS;
    Vec3s sp2C;

    if (self->actor.params > 0 && limbIndex != 16) {
        if (sAlternateOutfitDLs[limbIndex] != NULL) {
            *dList = sAlternateOutfitDLs[limbIndex];
        }
    }
    if (limbIndex == 16) {
        Matrix_Translate(1500.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        sp2C = self->unk_308.unk_08;
        Matrix_RotateZ(BINANG_TO_RAD(sp2C.x), MTXMODE_APPLY);
        Matrix_RotateX(BINANG_TO_RAD(sp2C.y), MTXMODE_APPLY);
        Matrix_Translate(-1500.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }
    if (limbIndex == 9) {
        sp2C = self->unk_308.unk_0E;
        Matrix_RotateX(BINANG_TO_RAD(sp2C.x), MTXMODE_APPLY);
        Matrix_RotateY(BINANG_TO_RAD(sp2C.y), MTXMODE_APPLY);
    }
    if (limbIndex == 9 || limbIndex == 10 || limbIndex == 13) {
        rot->y += Math_SinS(self->unk_330[limbIndex].y) * 200.0f;
        rot->z += Math_CosS(self->unk_330[limbIndex].z) * 200.0f;
    }
    return 0;
}

void EnIn_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnIn* self = THIS;
    Vec3f D_80A7B9A8 = { 1600.0, 0.0f, 0.0f };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_in.c", 2335);
    if (limbIndex == 16) {
        Matrix_MultVec3f(&D_80A7B9A8, &self->actor.focus.pos);
        self->actor.focus.rot = self->actor.world.rot;
    }
    if (limbIndex == 12 && self->skelAnime.animation == &object_in_Anim_014CA8) {
        gSPDisplayList(POLY_OPA_DISP++, object_in_DL_007A20);
    }
    if (limbIndex == 15 && self->skelAnime.animation == &object_in_Anim_014CA8) {
        gSPDisplayList(POLY_OPA_DISP++, object_in_DL_007BF8);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_in.c", 2365);
}

void EnIn_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* D_80A7B9B4[] = { object_in_Tex_003590, object_in_Tex_0047D0, object_in_Tex_004BD0,
                                  object_in_Tex_004390 };
    EnIn* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_in.c", 2384);
    if (self->actionFunc != func_80A79FB0) {
        func_80093D18(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80A7B9B4[self->unk_1EE]));
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(&object_in_Tex_0034D0));
        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnIn_OverrideLimbDraw, EnIn_PostLimbDraw, &self->actor);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_in.c", 2416);
}
