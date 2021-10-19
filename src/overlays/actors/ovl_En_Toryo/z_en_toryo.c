/*
 * File: z_en_tory.c
 * Overlay: ovl_En_Toryo
 * Description: Boss Carpenter
 */

#include "z_en_toryo.h"
#include "objects/object_toryo/object_toryo.h"

#define FLAGS 0x00000009

#define THIS ((EnToryo*)thisx)

void EnToryo_Init(Actor* thisx, GlobalContext* globalCtx);
void EnToryo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnToryo_Update(Actor* thisx, GlobalContext* globalCtx);
void EnToryo_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B20914(EnToryo* self, GlobalContext* globalCtx);
s32 EnToryo_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx);
void EnToryo_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx);

const ActorInit En_Toryo_InitVars = {
    ACTOR_EN_TORYO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TORYO,
    sizeof(EnToryo),
    (ActorFunc)EnToryo_Init,
    (ActorFunc)EnToryo_Destroy,
    (ActorFunc)EnToryo_Update,
    (ActorFunc)EnToryo_Draw,
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
    { 18, 63, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(0, 0x0),
    /* Master sword  */ DMG_ENTRY(0, 0x0),
    /* Giant's Knife */ DMG_ENTRY(0, 0x0),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0x0),
    /* Giant spin    */ DMG_ENTRY(0, 0x0),
    /* Master spin   */ DMG_ENTRY(0, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(0, 0x0),
    /* Giant jump    */ DMG_ENTRY(0, 0x0),
    /* Master jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

typedef struct {
    AnimationHeader* anim;
    f32 unk_4;
    u8 mode;
    f32 transitionRate;
} EnToryoAnimation;

static EnToryoAnimation sEnToryoAnimation = { &object_toryo_Anim_000E50, 1.0f, 0, 0 };

static Vec3f sMultVec = { 800.0f, 1000.0f, 0.0f };

void EnToryo_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnToryo* self = THIS;
    s32 pad;

    switch (globalCtx->sceneNum) {
        case SCENE_SPOT09:
            if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
                self->stateFlags |= 1;
            }
            break;
        case SCENE_SPOT01:
            if ((LINK_AGE_IN_YEARS == YEARS_CHILD) && IS_DAY) {
                self->stateFlags |= 2;
            }
            break;
        case SCENE_KAKARIKO:
            if ((LINK_AGE_IN_YEARS == YEARS_CHILD) && IS_NIGHT) {
                self->stateFlags |= 4;
            }
            break;
    }

    if ((self->stateFlags & 7) == 0) {
        Actor_Kill(&self->actor);
    }

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 42.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_toryo_Skel_007150, NULL, self->jointTable, self->morphTable,
                       17);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    Animation_Change(&self->skelAnime, sEnToryoAnimation.anim, 1.0f, 0.0f,
                     Animation_GetLastFrame(sEnToryoAnimation.anim), sEnToryoAnimation.mode,
                     sEnToryoAnimation.transitionRate);
    self->stateFlags |= 8;
    self->actor.targetMode = 6;
    self->actionFunc = func_80B20914;
}

void EnToryo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnToryo* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 func_80B203D8(EnToryo* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 ret = 1;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
            ret = 1;
            break;
        case 4:
            if (func_80106BC8(globalCtx)) {
                if (globalCtx->msgCtx.choiceIndex == 0) {
                    func_80106CCC(globalCtx);
                    self->actor.parent = NULL;
                    player->exchangeItemId = EXCH_ITEM_NONE;
                    globalCtx->msgCtx.msgMode = 0x37;
                    self->actor.textId = 0x601B;
                    ret = 3;
                } else {
                    self->actor.textId = 0x606F;
                    ret = 2;
                }
            }
            break;
        case 6:
            switch (self->actor.textId) {
                case 0x5028:
                    ret = 1;
                    if (func_80106BC8(globalCtx) != 0) {
                        gSaveContext.infTable[23] |= 4;
                        ret = 0;
                    }
                    break;
                case 0x601B:
                    ret = 1;
                    if (func_80106BC8(globalCtx) != 0) {
                        ret = 4;
                    }
                    break;
                case 0x606F:
                    ret = 1;
                    if (func_80106BC8(globalCtx) != 0) {
                        gSaveContext.infTable[23] |= 2;
                        ret = 0;
                    }
                    break;
                case 0x606A:
                    ret = 1;
                    if (func_80106BC8(globalCtx) != 0) {
                        gSaveContext.infTable[23] |= 1;
                        ret = 0;
                    }
                    break;
                case 0x606B:
                case 0x606C:
                case 0x606D:
                case 0x606E:
                default:
                    ret = 1;
                    if (func_80106BC8(globalCtx) != 0) {
                        ret = 0;
                    }
                    break;
            }
            break;
    }
    return ret;
}

s32 func_80B205CC(EnToryo* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 ret = 5;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            ret = 5;
            break;
        case 6:
            if (func_80106BC8(globalCtx) != 0) {
                ret = 0;
            }
            break;
    }
    return ret;
}

u32 func_80B20634(EnToryo* self, GlobalContext* globalCtx) {
    u32 ret;

    if (self->unk_1E0 != 0) {
        if (self->unk_1E0 == 10) {
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
            if (gSaveContext.infTable[23] & 2) {
                ret = 0x606E;
            } else {
                ret = 0x606D;
            }
        } else {
            ret = 0x200F;
        }
    }
    //! @bug return value may be unitialized
    return ret;
}

s32 func_80B206A0(EnToryo* self, GlobalContext* globalCtx) {
    s32 textId = Text_GetFaceReaction(globalCtx, 0);
    s32 ret = textId;

    if (textId == 0) {
        if ((self->stateFlags & 1)) {
            if ((gSaveContext.eventChkInf[9] & 0xF) == 0xF) {
                ret = 0x606C;
            } else if ((gSaveContext.infTable[23] & 1)) {
                ret = 0x606B;
            } else {
                ret = 0x606A;
            }
        } else if ((self->stateFlags & 2)) {
            if ((gSaveContext.infTable[23] & 4)) {
                ret = 0x5029;
            } else {
                ret = 0x5028;
            }
        } else {
            ret = textId;
            if ((self->stateFlags & 4)) {
                ret = 0x506C;
            }
        }
    }
    return ret;
}

void func_80B20768(EnToryo* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 sp32;
    s16 sp30;

    if (self->unk_1E4 == 3) {
        func_8002F194(&self->actor, globalCtx);
        func_8010B720(globalCtx, self->actor.textId);
        self->unk_1E4 = 1;
    }

    if (self->unk_1E4 == 1) {
        self->unk_1E4 = func_80B203D8(self, globalCtx);
    }

    if (self->unk_1E4 == 5) {
        self->unk_1E4 = func_80B205CC(self, globalCtx);
        return;
    }

    if (self->unk_1E4 == 2) {
        func_8010B720(globalCtx, self->actor.textId);
        self->unk_1E4 = 1;
    }

    if (self->unk_1E4 == 4) {
        if (Actor_HasParent(&self->actor, globalCtx)) {
            self->actor.parent = NULL;
            self->unk_1E4 = 5;
        } else {
            func_8002F434(&self->actor, globalCtx, GI_SWORD_BROKEN, 100.0f, 10.0f);
        }
        return;
    }

    if (self->unk_1E4 == 0) {
        if (func_8002F194(&self->actor, globalCtx) != 0) {
            self->unk_1E0 = func_8002F368(globalCtx);
            if (self->unk_1E0 != 0) {
                player->actor.textId = func_80B20634(self, globalCtx);
                self->actor.textId = player->actor.textId;
            }
            self->unk_1E4 = 1;
            return;
        }

        func_8002F374(globalCtx, &self->actor, &sp32, &sp30);
        if ((sp32 >= 0) && (sp32 < 0x141) && (sp30 >= 0) && (sp30 < 0xF1)) {
            self->actor.textId = func_80B206A0(self, globalCtx);
            func_8002F298(&self->actor, globalCtx, 100.0f, 10);
        }
    }
}

void func_80B20914(EnToryo* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    func_80B20768(self, globalCtx);
    if (self->unk_1E4 != 0) {
        self->stateFlags |= 0x10;
    } else {
        self->stateFlags &= ~0x10;
    }
}

void EnToryo_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnToryo* self = THIS;
    ColliderCylinder* collider = &self->collider;
    Player* player = GET_PLAYER(globalCtx);
    f32 rot;

    Collider_UpdateCylinder(thisx, collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, (Collider*)collider);

    self->actionFunc(self, globalCtx);

    if ((self->stateFlags & 8)) {
        self->unk_1EC.unk_18.x = player->actor.focus.pos.x;
        self->unk_1EC.unk_18.y = player->actor.focus.pos.y;
        self->unk_1EC.unk_18.z = player->actor.focus.pos.z;

        if ((self->stateFlags & 0x10)) {
            func_80034A14(thisx, &self->unk_1EC, 0, 4);
            return;
        }

        rot = thisx->yawTowardsPlayer - thisx->shape.rot.y;
        if ((rot < 14563.0f) && (rot > -14563.0f)) {
            func_80034A14(thisx, &self->unk_1EC, 0, 2);
        } else {
            func_80034A14(thisx, &self->unk_1EC, 0, 1);
        }
    }
}

void EnToryo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnToryo* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnToryo_OverrideLimbDraw, EnToryo_PostLimbDraw, self);
}

s32 EnToryo_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                             void* thisx) {
    EnToryo* self = THIS;

    if ((self->stateFlags & 8)) {
        switch (limbIndex) {
            case 8:
                rot->x += self->unk_1EC.unk_0E.y;
                rot->y -= self->unk_1EC.unk_0E.x;
                break;
            case 15:
                rot->x += self->unk_1EC.unk_08.y;
                rot->z += self->unk_1EC.unk_08.x;
                break;
        }
    }
    return 0;
}

void EnToryo_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnToryo* self = THIS;

    switch (limbIndex) {
        case 15:
            Matrix_MultVec3f(&sMultVec, &self->actor.focus.pos);
            break;
    }
}
