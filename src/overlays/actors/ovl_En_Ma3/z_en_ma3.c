/*
 * File: z_en_ma3.c
 * Overlay: En_Ma3
 * Description: Adult Malon (Ranch)
 */

#include "z_en_ma3.h"
#include "objects/object_ma2/object_ma2.h"

#define FLAGS 0x00000039

#define THIS ((EnMa3*)thisx)

void EnMa3_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMa3_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMa3_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMa3_Draw(Actor* thisx, GlobalContext* globalCtx);

u16 func_80AA2AA0(GlobalContext* globalCtx, Actor* self);
s16 func_80AA2BD4(GlobalContext* globalCtx, Actor* self);

void func_80AA2E54(EnMa3* self, GlobalContext* globalCtx);
s32 func_80AA2EC8(EnMa3* self, GlobalContext* globalCtx);
s32 func_80AA2F28(EnMa3* self);
void func_80AA2F80(EnMa3* self);
void func_80AA3004(EnMa3* self, s32 arg1);
void func_80AA3200(EnMa3* self, GlobalContext* globalCtx);

const ActorInit En_Ma3_InitVars = {
    ACTOR_EN_MA3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MA2,
    sizeof(EnMa3),
    (ActorFunc)EnMa3_Init,
    (ActorFunc)EnMa3_Destroy,
    (ActorFunc)EnMa3_Update,
    (ActorFunc)EnMa3_Draw,
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

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static struct_D_80AA1678 D_80AA3848[] = {
    { 0x060007D4, 1.0f, ANIMMODE_LOOP, 0.0f },   { 0x060007D4, 1.0f, ANIMMODE_LOOP, -10.0f },
    { 0x060093BC, 1.0f, ANIMMODE_LOOP, 0.0f },   { 0x06009EE0, 1.0f, ANIMMODE_LOOP, 0.0f },
    { 0x06009EE0, 1.0f, ANIMMODE_LOOP, -10.0f },
};

u16 func_80AA2AA0(GlobalContext* globalCtx, Actor* thisx) {
    Player* player = GET_PLAYER(globalCtx);
    s16* timer1ValuePtr; // weirdness with self necessary to match

    if (!(gSaveContext.infTable[11] & 0x100)) {
        return 0x2000;
    }
    timer1ValuePtr = &gSaveContext.timer1Value;
    if (gSaveContext.eventInf[0] & 0x400) {
        gSaveContext.timer1Value = gSaveContext.timer1Value;
        thisx->flags |= 0x10000;
        if (gSaveContext.timer1Value >= 0xD3) {
            return 0x208E;
        }
        if ((HIGH_SCORE(HS_HORSE_RACE) == 0) || (HIGH_SCORE(HS_HORSE_RACE) >= 0xB4)) {
            HIGH_SCORE(HS_HORSE_RACE) = 0xB4;
            gSaveContext.timer1Value = *timer1ValuePtr;
        }
        if (!(gSaveContext.eventChkInf[1] & 0x4000) && (gSaveContext.timer1Value < 0x32)) {
            return 0x208F;
        } else if (gSaveContext.timer1Value < HIGH_SCORE(HS_HORSE_RACE)) {
            return 0x2012;
        } else {
            return 0x2004;
        }
    }
    if ((!(player->stateFlags1 & 0x800000)) &&
        (Actor_FindNearby(globalCtx, thisx, ACTOR_EN_HORSE, 1, 1200.0f) == NULL)) {
        return 0x2001;
    }
    if (!(gSaveContext.infTable[11] & 0x200)) {
        return 0x2002;
    } else {
        return 0x2003;
    }
}

s16 func_80AA2BD4(GlobalContext* globalCtx, Actor* thisx) {
    s16 ret = 1;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 5:
            if (func_80106BC8(globalCtx) != 0) {
                globalCtx->nextEntranceIndex = 0x157;
                gSaveContext.nextCutsceneIndex = 0xFFF0;
                globalCtx->fadeTransition = 0x26;
                globalCtx->sceneLoadFlag = 0x14;
                gSaveContext.eventInf[0] |= 0x400;
                gSaveContext.timer1State = 0xF;
            }
            break;
        case 4:
            if (func_80106BC8(globalCtx) != 0) {
                gSaveContext.infTable[11] |= 0x200;
                if (globalCtx->msgCtx.choiceIndex == 0) {
                    if (gSaveContext.eventChkInf[1] & 0x4000) {
                        func_8010B720(globalCtx, 0x2091);
                    } else if (HIGH_SCORE(HS_HORSE_RACE) == 0) {
                        func_8010B720(globalCtx, 0x2092);
                    } else {
                        func_8010B720(globalCtx, 0x2090);
                    }
                }
            }
            break;
        case 2:
            switch (thisx->textId) {
                case 0x2000:
                    gSaveContext.infTable[11] |= 0x100;
                    ret = 0;
                    break;
                case 0x208F:
                    gSaveContext.eventChkInf[1] |= 0x4000;
                case 0x2004:
                case 0x2012:
                    if (HIGH_SCORE(HS_HORSE_RACE) > gSaveContext.timer1Value) {
                        HIGH_SCORE(HS_HORSE_RACE) = gSaveContext.timer1Value;
                    }
                case 0x208E:
                    gSaveContext.eventInf[0] &= ~0x400;
                    thisx->flags &= ~0x10000;
                    ret = 0;
                    gSaveContext.timer1State = 0xA;
                    break;
                case 0x2002:
                    gSaveContext.infTable[11] |= 0x200;
                case 0x2003:
                    if (!(gSaveContext.eventInf[0] & 0x400)) {
                        ret = 0;
                    }
                    break;
                default:
                    ret = 0;
            }
            break;
        case 0:
        case 1:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
            break;
    }
    return ret;
}

void func_80AA2E54(EnMa3* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 phi_a3;

    if ((self->unk_1E0.unk_00 == 0) && (self->skelAnime.animation == &object_ma2_Anim_009EE0)) {
        phi_a3 = 1;
    } else {
        phi_a3 = 0;
    }

    self->unk_1E0.unk_18 = player->actor.world.pos;
    self->unk_1E0.unk_14 = 0.0f;
    func_80034A14(&self->actor, &self->unk_1E0, 0, phi_a3);
}

s32 func_80AA2EC8(EnMa3* self, GlobalContext* globalCtx) {
    if (LINK_IS_CHILD) {
        return 2;
    }
    if (!(gSaveContext.eventChkInf[1] & 0x100)) {
        return 2;
    }
    if (gSaveContext.eventInf[0] & 0x400) {
        return 1;
    }
    return 0;
}

s32 func_80AA2F28(EnMa3* self) {
    if (self->skelAnime.animation != &object_ma2_Anim_009EE0) {
        return 0;
    }
    if (self->unk_1E0.unk_00 != 0) {
        return 0;
    }
    self->unk_20C = 0;
    if (self->unk_20E != 2) {
        return 0;
    }
    self->unk_210 = 2;
    return 1;
}

void func_80AA2F80(EnMa3* self) {
    if ((!func_80AA2F28(self)) && (DECR(self->unk_20C) == 0)) {
        self->unk_20E += 1;
        if (self->unk_20E >= 3) {
            self->unk_20C = Rand_S16Offset(0x1E, 0x1E);
            self->unk_20E = 0;
        }
    }
}

void func_80AA3004(EnMa3* self, s32 idx) {
    f32 frameCount = Animation_GetLastFrame(D_80AA3848[idx].animation);

    Animation_Change(&self->skelAnime, D_80AA3848[idx].animation, 1.0f, 0.0f, frameCount, D_80AA3848[idx].mode,
                     D_80AA3848[idx].transitionRate);
}

void EnMa3_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnMa3* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_ma2_Skel_008D90, NULL, NULL, NULL, 0);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(22), &sColChkInfoInit);

    switch (func_80AA2EC8(self, globalCtx)) {
        case 0:
            func_80AA3004(self, 0);
            self->actionFunc = func_80AA3200;
            break;
        case 1:
            func_80AA3004(self, 0);
            self->actionFunc = func_80AA3200;
            break;
        case 2:
            Actor_Kill(&self->actor);
            return;
    }

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    Actor_SetScale(&self->actor, 0.01f);
    self->unk_1E0.unk_00 = (u16)0;
}

void EnMa3_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnMa3* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80AA3200(EnMa3* self, GlobalContext* globalCtx) {
    if (self->unk_1E0.unk_00 == 2) {
        self->actor.flags &= ~0x10000;
        self->unk_1E0.unk_00 = 0;
    }
}

void EnMa3_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnMa3* self = THIS;
    s32 pad;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    SkelAnime_Update(&self->skelAnime);
    func_80AA2F80(self);
    self->actionFunc(self, globalCtx);
    func_80AA2E54(self, globalCtx);
    func_800343CC(globalCtx, &self->actor, &self->unk_1E0.unk_00, (f32)self->collider.dim.radius + 150.0f,
                  func_80AA2AA0, func_80AA2BD4);
    if (self->unk_1E0.unk_00 == 0) {
        if (self->unk_20A != 0) {
            func_800F6584(0);
            self->unk_20A = 0;
        }
    } else if (self->unk_20A == 0) {
        func_800F6584(1);
        self->unk_20A = 1;
    }
}

s32 EnMa3_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnMa3* self = THIS;
    Vec3s vec;

    if ((limbIndex == 3) || (limbIndex == 6)) {
        *dList = NULL;
    }
    if (limbIndex == 18) {
        Matrix_Translate(1400.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        vec = self->unk_1E0.unk_08;
        Matrix_RotateX((vec.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((vec.x / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_Translate(-1400.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }
    if (limbIndex == 11) {
        vec = self->unk_1E0.unk_0E;
        Matrix_RotateY((-vec.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateX((-vec.x / 32768.0f) * M_PI, MTXMODE_APPLY);
    }
    if ((limbIndex == 11) || (limbIndex == 12) || (limbIndex == 15)) {
        rot->y += Math_SinS(self->unk_212[limbIndex].y) * 200.0f;
        rot->z += Math_CosS(self->unk_212[limbIndex].z) * 200.0f;
    }
    return false;
}

void EnMa3_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnMa3* self = THIS;
    Vec3f vec = { 900.0f, 0.0f, 0.0f };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ma3.c", 927);

    if (limbIndex == 18) {
        Matrix_MultVec3f(&vec, &self->actor.focus.pos);
    }
    if ((limbIndex == 14) && (self->skelAnime.animation == &object_ma2_Anim_0093BC)) {
        gSPDisplayList(POLY_OPA_DISP++, &object_ma2_DL_005420);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ma3.c", 950);
}

void EnMa3_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* D_80AA38A4[] = {
        0x06002970,
        0x06003570,
        0x06003770,
    };
    static void* D_80AA38B0[] = {
        0x06002570,
        0x06002C70,
        0x06003070,
    };
    EnMa3* self = THIS;
    Camera* camera;
    f32 someFloat;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ma3.c", 978);

    camera = GET_ACTIVE_CAM(globalCtx);
    someFloat = Math_Vec3f_DistXZ(&self->actor.world.pos, &camera->eye);
    func_800F6268(someFloat, 0x2F);
    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(D_80AA38A4[self->unk_210]));
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_80AA38B0[self->unk_20E]));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnMa3_OverrideLimbDraw, EnMa3_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ma3.c", 1013);
}
