/*
 * File: z_en_owl.c
 * Overlay: ovl_En_Owl
 * Description: Owl
 */

#include "z_en_owl.h"
#include "objects/object_owl/object_owl.h"
#include "scenes/overworld/spot06/spot06_scene.h"
#include "scenes/overworld/spot16/spot16_scene.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnOwl*)thisx)

void EnOwl_Init(Actor* thisx, GlobalContext* globalCtx);
void EnOwl_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnOwl_Update(Actor* thisx, GlobalContext* globalCtx);
void EnOwl_Draw(Actor* thisx, GlobalContext* globalCtx);
void EnOwl_ChangeMode(EnOwl* self, EnOwlActionFunc, OwlFunc, SkelAnime*, AnimationHeader*, f32);
void EnOwl_WaitDefault(EnOwl* self, GlobalContext* globalCtx);
void func_80ACC540(EnOwl* self);
void EnOwl_WaitOutsideKokiri(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitHyruleCastle(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitKakariko(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitGerudo(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitLakeHylia(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitZoraRiver(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitHyliaShortcut(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitDeathMountainShortcut(EnOwl* self, GlobalContext* globalCtx);
void func_80ACB3E0(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitLWPreSaria(EnOwl* self, GlobalContext* globalCtx);
void EnOwl_WaitLWPostSaria(EnOwl* self, GlobalContext* globalCtx);
void func_80ACD4D4(EnOwl* self, GlobalContext* globalCtx);
void func_80ACD130(EnOwl* self, GlobalContext* globalCtx, s32 arg2);
void func_80ACBAB8(EnOwl* self, GlobalContext* globalCtx);
void func_80ACD2CC(EnOwl* self, GlobalContext* globalCtx);
void func_80ACAA54(EnOwl* self, GlobalContext* globalCtx);
void func_80ACAC6C(EnOwl* self, GlobalContext* globalCtx);
void func_80ACADF0(EnOwl* self, GlobalContext* globalCtx);
void func_80ACAF74(EnOwl* self, GlobalContext* globalCtx);
void func_80ACC30C(EnOwl* self, GlobalContext* globalCtx);
void func_80ACB4FC(EnOwl* self, GlobalContext* globalCtx);
void func_80ACB680(EnOwl* self, GlobalContext* globalCtx);
void func_80ACC460(EnOwl* self);
void func_80ACBEA0(EnOwl*, GlobalContext*);

typedef enum {
    /* 0x00 */ OWL_DEFAULT,
    /* 0x01 */ OWL_OUTSIDE_KOKIRI,
    /* 0x02 */ OWL_HYRULE_CASTLE,
    /* 0x03 */ OWL_KAKARIKO,
    /* 0x04 */ OWL_HYLIA_GERUDO,
    /* 0x05 */ OWL_LAKE_HYLIA,
    /* 0x06 */ OWL_ZORA_RIVER,
    /* 0x07 */ OWL_HYLIA_SHORTCUT,
    /* 0x08 */ OWL_DEATH_MOUNTAIN,
    /* 0x09 */ OWL_DEATH_MOUNTAIN2,
    /* 0x0A */ OWL_DESSERT_COLOSSUS,
    /* 0x0B */ OWL_LOST_WOODS_PRESARIA,
    /* 0x0C */ OWL_LOST_WOODS_POSTSARIA
} EnOwlType;

typedef enum {
    /* 0x00 */ OWL_REPEAT,
    /* 0x01 */ OWL_OK
} EnOwlMessageChoice;

const ActorInit En_Owl_InitVars = {
    ACTOR_EN_OWL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OWL,
    sizeof(EnOwl),
    (ActorFunc)EnOwl_Init,
    (ActorFunc)EnOwl_Destroy,
    (ActorFunc)EnOwl_Update,
    (ActorFunc)EnOwl_Draw,
};

static ColliderCylinderInit sOwlCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 40, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 25, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2400, ICHAIN_STOP),
};

void EnOwl_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnOwl* self = THIS;
    ColliderCylinder* collider;
    s32 owlType;
    s32 switchFlag;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gOwlFlyingSkel, &gOwlFlyAnim, self->jointTable, self->morphTable,
                       21);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime2, &gOwlPerchingSkel, &gOwlPerchAnim, self->jointTable2,
                       self->morphTable2, 16);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sOwlCylinderInit);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->actor.minVelocityY = -10.0f;
    self->actor.targetArrowOffset = 500.0f;
    EnOwl_ChangeMode(self, EnOwl_WaitDefault, func_80ACC540, &self->skelAnime2, &gOwlPerchAnim, 0.0f);
    self->actionFlags = self->unk_406 = self->unk_409 = 0;
    self->unk_405 = 4;
    self->unk_404 = self->unk_407 = 0;
    self->unk_408 = 4;
    owlType = (self->actor.params & 0xFC0) >> 6;
    switchFlag = (self->actor.params & 0x3F);
    if (self->actor.params == 0xFFF) {
        owlType = OWL_OUTSIDE_KOKIRI;
        switchFlag = 0x20;
    }
    // "conversation owl %4x no = %d, sv = %d"
    osSyncPrintf(VT_FGCOL(CYAN) " 会話フクロウ %4x no = %d, sv = %d\n" VT_RST, self->actor.params, owlType, switchFlag);

    if ((owlType != OWL_DEFAULT) && (switchFlag < 0x20) && Flags_GetSwitch(globalCtx, switchFlag)) {
        osSyncPrintf("savebitでフクロウ退避\n"); // "Save owl with savebit"
        Actor_Kill(&self->actor);
        return;
    }

    self->unk_3EE = 0;
    self->unk_400 = self->actor.world.rot.y;

    switch (owlType) {
        case OWL_DEFAULT:
            self->actionFunc = EnOwl_WaitDefault;
            self->actor.uncullZoneForward = 4000.0f;
            self->unk_40A = 0;
            break;
        case OWL_OUTSIDE_KOKIRI:
            self->actionFunc = EnOwl_WaitOutsideKokiri;
            break;
        case OWL_HYRULE_CASTLE:
            self->actionFlags |= 2;
            self->unk_3EE = 0x20;
            self->actionFunc = EnOwl_WaitHyruleCastle;
            break;
        case OWL_KAKARIKO:
            if (gSaveContext.eventChkInf[4] & 1) {
                // has zelda's letter
                osSyncPrintf("フクロウ退避\n"); // "Owl evacuation"
                Actor_Kill(&self->actor);
                return;
            }

            self->actionFunc = EnOwl_WaitKakariko;
            break;
        case OWL_HYLIA_GERUDO:
            if (gSaveContext.eventChkInf[4] & 8) {
                // has ocarina of time
                osSyncPrintf("フクロウ退避\n"); // "Owl evacuation"
                Actor_Kill(&self->actor);
                return;
            }
            self->actionFunc = EnOwl_WaitGerudo;
            break;
        case OWL_LAKE_HYLIA:
            self->actionFunc = EnOwl_WaitLakeHylia;
            break;
        case OWL_ZORA_RIVER:
            if ((gSaveContext.eventChkInf[3] & 0x200) || !(gSaveContext.eventChkInf[4] & 1)) {
                // opened zora's domain or has zelda's letter
                osSyncPrintf("フクロウ退避\n"); // "Owl evacuation"
                Actor_Kill(&self->actor);
                return;
            }

            self->actionFunc = EnOwl_WaitZoraRiver;
            break;
        case OWL_HYLIA_SHORTCUT:
            self->actionFunc = EnOwl_WaitHyliaShortcut;
            Flags_UnsetSwitch(globalCtx, 0x23);
            return;
        case OWL_DEATH_MOUNTAIN:
            self->actionFunc = EnOwl_WaitDeathMountainShortcut;
            break;
        case OWL_DEATH_MOUNTAIN2:
            self->actionFunc = EnOwl_WaitDeathMountainShortcut;
            break;
        case OWL_DESSERT_COLOSSUS:
            self->actionFunc = func_80ACB3E0;
            break;
        case OWL_LOST_WOODS_PRESARIA:
            if (!CHECK_QUEST_ITEM(QUEST_SONG_LULLABY)) {
                osSyncPrintf("フクロウ退避\n"); // "Owl evacuation"
                Actor_Kill(&self->actor);
                return;
            }
            self->actionFunc = EnOwl_WaitLWPreSaria;
            break;
        case OWL_LOST_WOODS_POSTSARIA:
            if (!CHECK_QUEST_ITEM(QUEST_SONG_SARIA)) {
                osSyncPrintf("フクロウ退避\n"); // "Owl evacuation"
                Actor_Kill(&self->actor);
                return;
            }
            self->actionFunc = EnOwl_WaitLWPostSaria;
            break;
        default:
            // Outside kokiri forest
            osSyncPrintf(VT_FGCOL(CYAN));
            osSyncPrintf("no = %d  \n", owlType);
            // "Unfinished owl unfinished owl unfinished owl"
            osSyncPrintf("未完成のフクロウ未完成のフクロウ未完成のフクロウ\n");
            osSyncPrintf(VT_RST);
            self->actionFlags |= 2;
            self->unk_3EE = 0x20;
            self->actionFunc = EnOwl_WaitOutsideKokiri;
            break;
    }
}

void EnOwl_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnOwl* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

/**
 * Rotates self to the player instance
 */
void EnOwl_LookAtLink(EnOwl* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->actor.shape.rot.y = self->actor.world.rot.y =
        Math_Vec3f_Yaw(&self->actor.world.pos, &player->actor.world.pos);
}

/**
 * Checks if link is within `targetDist` units, initalize the camera for the owl.
 * returns 0 if the link is not within `targetDistance`, returns 1 once link is within
 * the distance, and the camera has been initalized.
 */
s32 EnOwl_CheckInitTalk(EnOwl* self, GlobalContext* globalCtx, u16 textId, f32 targetDist, u16 flags) {
    s32 timer;
    f32 distCheck;

    if (func_8002F194(&self->actor, globalCtx)) {
        if (self->actor.params == 0xFFF) {
            self->actionFlags |= 0x40;
            timer = -100;
        } else {
            if (Rand_ZeroOne() < 0.5f) {
                timer = (flags & 1) ? -97 : -99;
                self->actionFlags |= 0x40;
            } else {
                timer = (flags & 1) ? -96 : -98;
                self->actionFlags &= ~0x40;
            }
        }
        self->cameraIdx = OnePointCutscene_Init(globalCtx, 8700, timer, &self->actor, MAIN_CAM);
        return true;
    } else {
        self->actor.textId = textId;
        distCheck = (flags & 2) ? 200.0f : 1000.0f;
        if (self->actor.xzDistToPlayer < targetDist) {
            self->actor.flags |= 0x10000;
            func_8002F1C4(&self->actor, globalCtx, targetDist, distCheck, 0);
        }
        return false;
    }
}

s32 func_80ACA558(EnOwl* self, GlobalContext* globalCtx, u16 textId) {
    if (func_8002F194(&self->actor, globalCtx)) {
        return true;
    } else {
        self->actor.textId = textId;
        if (self->actor.xzDistToPlayer < 120.0f) {
            func_8002F1C4(&self->actor, globalCtx, 350.0f, 1000.0f, 0);
        }
        return false;
    }
}

void func_80ACA5C8(EnOwl* self) {
    EnOwl_ChangeMode(self, func_80ACBEA0, func_80ACC540, &self->skelAnime, &gOwlUnfoldWingsAnim, 0.0f);
    self->eyeTexIndex = 0;
    self->blinkTimer = Rand_S16Offset(60, 60);
}

void func_80ACA62C(EnOwl* self, GlobalContext* globalCtx) {
    s32 switchFlag = self->actor.params & 0x3F;

    if (switchFlag < 0x20) {
        Flags_SetSwitch(globalCtx, switchFlag);
        osSyncPrintf(VT_FGCOL(CYAN) " Actor_Environment_sw = %d\n" VT_RST, Flags_GetSwitch(globalCtx, switchFlag));
    }
    func_80ACA5C8(self);
}

void func_80ACA690(EnOwl* self, GlobalContext* globalCtx) {
    if ((self->unk_3EE & 0x3F) == 0) {
        func_80ACA62C(self, globalCtx);
    }
}

void func_80ACA6C0(EnOwl* self) {
    if (Rand_CenteredFloat(1.0f) < 0.0f) {
        self->actionFlags |= 0x20;
    } else {
        self->actionFlags &= ~0x20;
    }
}

void func_80ACA71C(EnOwl* self) {
    func_80ACA6C0(self);
    self->unk_3F2 = 0;
    self->actionFlags |= 0x10;
    self->unk_408 = 4;
    self->unk_404 = 0;
    self->unk_406 = 0;
    self->unk_405 = 4;
    self->unk_407 = self->unk_3F2;
}

void func_80ACA76C(EnOwl* self, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &self->actor, 8);

    if (func_8002F334(&self->actor, globalCtx)) {
        Audio_QueueSeqCmd(0x110000FF);
        func_80ACA62C(self, globalCtx);
        self->actor.flags &= ~0x10000;
    }
}

void func_80ACA7E0(EnOwl* self, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &self->actor, 8);

    if (func_8002F334(&self->actor, globalCtx)) {
        Audio_QueueSeqCmd(0x110000FF);
        if ((self->unk_3EE & 0x3F) == 0) {
            func_80ACA62C(self, globalCtx);
        } else {
            self->actionFlags &= ~2;
            func_80ACA71C(self);
            self->actionFunc = func_80ACA690;
        }
        self->actor.flags &= ~0x10000;
    }
}

void EnOwl_ConfirmKokiriMessage(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                func_8010B720(globalCtx, 0x2065);
                break;
            case OWL_OK:
                func_8010B720(globalCtx, 0x2067);
                self->actionFunc = func_80ACA76C;
                break;
        }
    }
}

void EnOwl_WaitOutsideKokiri(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (EnOwl_CheckInitTalk(self, globalCtx, 0x2064, 360.0f, 0)) {
        // Sets BGM
        func_800F5C64(0x5A);

        self->actionFunc = EnOwl_ConfirmKokiriMessage;
        // spoke to owl by lost woods
        gSaveContext.eventChkInf[6] |= 0x8000;
    }
}

void func_80ACA998(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                func_8010B720(globalCtx, 0x2069);
                self->actionFunc = func_80ACAA54;
                break;
            case OWL_OK:
                func_8010B720(globalCtx, 0x206B);
                self->actionFunc = func_80ACA7E0;
                break;
        }
        self->actionFlags &= ~2;
        func_80ACA71C(self);
    }
}

void func_80ACAA54(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0x206A);
        self->actionFunc = func_80ACA998;
        self->actionFlags |= 2;
        func_80ACA71C(self);
    }
}

void func_80ACAAC0(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0x2069);
        self->actionFunc = func_80ACAA54;
        self->actionFlags &= ~2;
        func_80ACA71C(self);
    }
}

void EnOwl_WaitHyruleCastle(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (EnOwl_CheckInitTalk(self, globalCtx, 0x2068, 540.0f, 0)) {
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACAAC0;
    }
}

void func_80ACAB88(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                // obtained zelda's letter
                if (gSaveContext.eventChkInf[4] & 1) {
                    func_8010B720(globalCtx, 0x206D);
                } else {
                    func_8010B720(globalCtx, 0x206C);
                }
                self->actionFunc = func_80ACAC6C;
                break;
            case OWL_OK:
                func_8010B720(globalCtx, 0x206E);
                self->actionFunc = func_80ACA7E0;
                break;
        }

        self->actionFlags &= ~2;
        func_80ACA71C(self);
    }
}

void func_80ACAC6C(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0x206A);
        self->actionFunc = func_80ACAB88;
        self->actionFlags |= 2;
        func_80ACA71C(self);
    }
}

void EnOwl_WaitKakariko(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (EnOwl_CheckInitTalk(self, globalCtx, 0x206C, 480.0f, 0)) {
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACAC6C;
    }
}

void func_80ACAD34(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                func_8010B720(globalCtx, 0x206F);
                self->actionFunc = func_80ACADF0;
                break;
            case OWL_OK:
                func_8010B720(globalCtx, 0x2070);
                self->actionFunc = func_80ACA7E0;
                break;
        }

        self->actionFlags &= ~2;
        func_80ACA71C(self);
    }
}

void func_80ACADF0(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0x206A);
        self->actionFunc = func_80ACAD34;
        self->actionFlags |= 2;
        func_80ACA71C(self);
    }
}

void EnOwl_WaitGerudo(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (EnOwl_CheckInitTalk(self, globalCtx, 0x206F, 360.0f, 0)) {
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACADF0;
    }
}

void func_80ACAEB8(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                func_8010B720(globalCtx, 0x2071);
                self->actionFunc = func_80ACAF74;
                break;
            case OWL_OK:
                func_8010B720(globalCtx, 0x2072);
                self->actionFunc = func_80ACA7E0;
                break;
        }

        self->actionFlags &= ~2;
        func_80ACA71C(self);
    }
}

void func_80ACAF74(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0x206A);
        self->actionFunc = func_80ACAEB8;
        self->actionFlags |= 2;
        func_80ACA71C(self);
    }
}

void EnOwl_WaitLakeHylia(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (EnOwl_CheckInitTalk(self, globalCtx, 0x2071, 360.0f, 0)) {
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACAF74;
    }
}

void func_80ACB03C(EnOwl* self, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &self->actor, 8);

    if (func_8002F334(&self->actor, globalCtx)) {
        Audio_QueueSeqCmd(0x110000FF);
        func_80ACA62C(self, globalCtx);
        self->actor.flags &= ~0x10000;
    }
}

void EnOwl_WaitZoraRiver(EnOwl* self, GlobalContext* globalCtx) {
    u16 textId;

    EnOwl_LookAtLink(self, globalCtx);

    if (CHECK_QUEST_ITEM(QUEST_SONG_SARIA)) {
        if (CHECK_QUEST_ITEM(QUEST_SONG_LULLABY)) {
            textId = 0x4031;
        } else {
            textId = 0x4017;
        }
    } else {
        textId = 0x4002;
    }

    if (EnOwl_CheckInitTalk(self, globalCtx, textId, 360.0f, 0)) {
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACB03C;
    }
}

void func_80ACB148(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        Audio_QueueSeqCmd(0x110000FF);
        func_80ACA5C8(self);
        self->actionFunc = func_80ACC30C;
        Flags_SetSwitch(globalCtx, 0x23);
    }
}

void EnOwl_WaitHyliaShortcut(EnOwl* self, GlobalContext* globalCtx) {
    u16 textId = (gSaveContext.infTable[25] & 0x20) ? 0x4004 : 0x4003;
    // Spoke to Owl in Lake Hylia
    EnOwl_LookAtLink(self, globalCtx);
    if (func_80ACA558(self, globalCtx, textId)) {
        gSaveContext.infTable[25] |= 0x20;
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACB148;
    }
}

void func_80ACB22C(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        Audio_QueueSeqCmd(0x110000FF);
        func_80ACA5C8(self);
        self->actionFunc = func_80ACC30C;
    }
}

void func_80ACB274(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        Audio_QueueSeqCmd(0x110000FF);
        self->actionFunc = EnOwl_WaitDeathMountainShortcut;
    }
}

void EnOwl_WaitDeathMountainShortcut(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (!gSaveContext.magicAcquired) {
        if (func_80ACA558(self, globalCtx, 0x3062)) {
            func_800F5C64(0x5A);
            self->actionFunc = func_80ACB274;
            return;
        }
    } else {
        if (func_80ACA558(self, globalCtx, 0x3063)) {
            func_800F5C64(0x5A);
            self->actionFunc = func_80ACB22C;
        }
    }
}

void func_80ACB344(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                func_8010B720(globalCtx, 0x607A);
                break;
            case OWL_OK:
                func_8010B720(globalCtx, 0x607C);
                self->actionFunc = func_80ACA7E0;
                break;
        }
    }
}

void func_80ACB3E0(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (EnOwl_CheckInitTalk(self, globalCtx, 0x6079, 360.0f, 2)) {
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACB344;
    }
}

void func_80ACB440(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                func_8010B720(globalCtx, 0x10C1);
                self->actionFunc = func_80ACB4FC;
                break;
            case OWL_OK:
                func_8010B720(globalCtx, 0x10C3);
                self->actionFunc = func_80ACA7E0;
        }

        self->actionFlags &= ~2;
        func_80ACA71C(self);
    }
}

void func_80ACB4FC(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0x10C2);
        self->actionFunc = func_80ACB440;
        self->actionFlags |= 2;
        func_80ACA71C(self);
    }
}

void EnOwl_WaitLWPreSaria(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (EnOwl_CheckInitTalk(self, globalCtx, 0x10C0, 190.0f, 0)) {
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACB4FC;
    }
}

void func_80ACB5C4(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case OWL_REPEAT:
                func_8010B720(globalCtx, 0x10C5);
                self->actionFunc = func_80ACB680;
                break;
            case OWL_OK:
                func_8010B720(globalCtx, 0x10C7);
                self->actionFunc = func_80ACA7E0;
                break;
        }

        self->actionFlags &= ~2;
        func_80ACA71C(self);
    }
}

void func_80ACB680(EnOwl* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0x10C6);
        self->actionFunc = func_80ACB5C4;
        self->actionFlags |= 2;
        func_80ACA71C(self);
    }
}

void EnOwl_WaitLWPostSaria(EnOwl* self, GlobalContext* globalCtx) {
    EnOwl_LookAtLink(self, globalCtx);

    if (EnOwl_CheckInitTalk(self, globalCtx, 0x10C4, 360.0f, 0)) {
        func_800F5C64(0x5A);
        self->actionFunc = func_80ACB680;
    }
}

void func_80ACB748(EnOwl* self, GlobalContext* globalCtx) {
    static Vec3f D_80ACD62C = { 0.0f, 0.0f, 0.0f };
    f32 dist;
    f32 weight;
    s32 owlType = (self->actor.params & 0xFC0) >> 6;

    dist = Math3D_Vec3f_DistXYZ(&self->eye, &globalCtx->view.eye) / 45.0f;
    self->eye.x = globalCtx->view.eye.x;
    self->eye.y = globalCtx->view.eye.y;
    self->eye.z = globalCtx->view.eye.z;

    weight = dist;
    if (weight > 1.0f) {
        weight = 1.0f;
    }

    switch (owlType) {
        case 7:
            func_800F436C(&D_80ACD62C, NA_SE_EV_FLYING_AIR - SFX_FLAG, weight * 2.0f);
            if ((globalCtx->csCtx.frames > 324) ||
                ((globalCtx->csCtx.frames >= 142 && (globalCtx->csCtx.frames <= 266)))) {
                func_800F4414(&D_80ACD62C, NA_SE_EN_OWL_FLUTTER, weight * 2.0f);
            }
            if (globalCtx->csCtx.frames == 85) {
                func_800F436C(&D_80ACD62C, NA_SE_EV_PASS_AIR, weight * 2.0f);
            }
            break;
        case 8:
        case 9:
            func_800F436C(&D_80ACD62C, NA_SE_EV_FLYING_AIR - SFX_FLAG, weight * 2.0f);
            if ((globalCtx->csCtx.frames >= 420) ||
                ((0xC1 < globalCtx->csCtx.frames && (globalCtx->csCtx.frames <= 280)))) {
                func_800F4414(&D_80ACD62C, NA_SE_EN_OWL_FLUTTER, weight * 2.0f);
            }
            if (globalCtx->csCtx.frames == 217) {
                func_800F436C(&D_80ACD62C, NA_SE_EV_PASS_AIR, weight * 2.0f);
            }
            break;
    }
}

void func_80ACB904(EnOwl* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE && (globalCtx->csCtx.npcActions[7] != NULL)) {
        if (self->unk_40A != globalCtx->csCtx.npcActions[7]->action) {
            func_80ACD130(self, globalCtx, 7);
            func_80ACBAB8(self, globalCtx);
        }
        func_80ACD2CC(self, globalCtx);
    }

    if (self->actionFlags & 0x80) {
        func_80ACB748(self, globalCtx);
    }
}

void func_80ACB994(EnOwl* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE && (globalCtx->csCtx.npcActions[7] != NULL)) {
        if (self->unk_40A != globalCtx->csCtx.npcActions[7]->action) {
            func_80ACD130(self, globalCtx, 7);
            func_80ACBAB8(self, globalCtx);
        }
        func_80ACD4D4(self, globalCtx);
    }

    if (self->actionFlags & 0x80) {
        func_80ACB748(self, globalCtx);
    }
}

void EnOwl_WaitDefault(EnOwl* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE && (globalCtx->csCtx.npcActions[7] != NULL)) {
        if (self->unk_40A != globalCtx->csCtx.npcActions[7]->action) {
            self->actionFlags |= 4;
            func_80ACD130(self, globalCtx, 7);
            func_80ACBAB8(self, globalCtx);
        } else {
            self->actor.world.rot.z = globalCtx->csCtx.npcActions[7]->urot.y;
        }
    }

    if (self->actionFlags & 0x80) {
        func_80ACB748(self, globalCtx);
    }
}

void func_80ACBAB8(EnOwl* self, GlobalContext* globalCtx) {
    switch (globalCtx->csCtx.npcActions[7]->action - 1) {
        case 0:
            EnOwl_ChangeMode(self, func_80ACB904, func_80ACC540, &self->skelAnime, &gOwlFlyAnim, 0.0f);
            break;
        case 1:
            self->actor.draw = EnOwl_Draw;
            EnOwl_ChangeMode(self, EnOwl_WaitDefault, func_80ACC540, &self->skelAnime, &gOwlPerchAnim, 0.0f);
            break;
        case 2:
            self->actor.draw = EnOwl_Draw;
            EnOwl_ChangeMode(self, func_80ACB994, func_80ACC540, &self->skelAnime, &gOwlFlyAnim, 0.0f);
            break;
        case 3:
            self->actor.draw = NULL;
            self->actionFunc = EnOwl_WaitDefault;
            break;
        case 4:
            Actor_Kill(&self->actor);
            break;
    }

    self->unk_40A = globalCtx->csCtx.npcActions[7]->action;
}

void func_80ACBC0C(EnOwl* self, GlobalContext* globalCtx) {
    self->actor.flags |= 0x20;

    if (self->actor.xzDistToPlayer > 6000.0f && !(self->actionFlags & 0x80)) {
        Actor_Kill(&self->actor);
    }

    Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_400, 2, 0x80, 0x40);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->actor.speedXZ < 16.0f) {
        self->actor.speedXZ += 0.5f;
    }

    if ((self->unk_3F8 + 1000.0f) < self->actor.world.pos.y) {
        if (self->actor.velocity.y > 0.0f) {
            self->actor.velocity.y -= 0.4f;
        }
    } else if (self->actor.velocity.y < 4.0f) {
        self->actor.velocity.y += 0.2f;
    }

    self->actionFlags |= 8;
}

void func_80ACBD4C(EnOwl* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame > 10.0f) {
        Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_400, 2, 0x400, 0x40);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    if (self->skelAnime.curFrame > 45.0f) {
        self->actor.velocity.y = 2.0f;
        self->actor.gravity = 0.0f;
        self->actor.speedXZ = 8.0f;
    } else if (self->skelAnime.curFrame > 17.0f) {
        self->actor.velocity.y = 6.0f;
        self->actor.gravity = 0.0f;
        self->actor.speedXZ = 4.0f;
    }

    if (self->actionFlags & 1) {
        EnOwl_ChangeMode(self, func_80ACBC0C, func_80ACC460, &self->skelAnime, &gOwlFlyAnim, 0.0f);
        self->unk_3FE = 6;
        if (self->actionFlags & 0x40) {
            self->unk_400 += 0x2000;
        } else {
            self->unk_400 -= 0x2000;
        }
    }

    self->actionFlags |= 8;
}

void func_80ACBEA0(EnOwl* self, GlobalContext* GlobalContext) {
    if (self->actionFlags & 1) {
        self->unk_3FE = 3;
        EnOwl_ChangeMode(self, func_80ACBD4C, func_80ACC540, &self->skelAnime, &gOwlTakeoffAnim, 0.0f);
        self->unk_3F8 = self->actor.world.pos.y;
        self->actor.velocity.y = 2.0f;
        if (self->actionFlags & 0x40) {
            self->unk_400 = self->actor.world.rot.y + 0x4000;
        } else {
            self->unk_400 = self->actor.world.rot.y - 0x4000;
        }
    }

    self->actionFlags |= 8;
}

void func_80ACBF50(EnOwl* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_400, 2, 0x384, 0x258);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->actionFlags & 1) {
        EnOwl_ChangeMode(self, func_80ACBC0C, func_80ACC460, &self->skelAnime, &gOwlFlyAnim, 0.0f);
        self->unk_3FE = 6;
        self->actor.velocity.y = 2.0f;
        self->actor.gravity = 0.0f;
        self->actor.speedXZ = 4.0f;
    }
    self->actionFlags |= 8;
}

void func_80ACC00C(EnOwl* self, GlobalContext* globalCtx) {
    s32 owlType;
    s32 temp_v0;
    s32 temp_v0_2;

    Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_400, 2, 0x384, 0x258);
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->actor.xzDistToPlayer < 50.0f) {
        if (!Gameplay_InCsMode(globalCtx)) {
            owlType = (self->actor.params & 0xFC0) >> 6;
            osSyncPrintf(VT_FGCOL(CYAN));
            osSyncPrintf("%dのフクロウ\n", owlType); // "%d owl"
            osSyncPrintf(VT_RST);
            switch (owlType) {
                case 7:
                    osSyncPrintf(VT_FGCOL(CYAN));
                    osSyncPrintf("SPOT 06 の デモがはしった\n"); // "Demo of SPOT 06 has been completed"
                    osSyncPrintf(VT_RST);
                    globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gLakeHyliaOwlCs);
                    self->actor.draw = NULL;
                    break;
                case 8:
                case 9:
                    globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gDmtOwlCs);
                    self->actor.draw = NULL;
                    break;
                default:
                    ASSERT(0, "0", "../z_en_owl.c", 1693);
                    break;
            }

            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
            gSaveContext.cutsceneTrigger = 1;
            func_800F44EC(0x14, 0xA);
            self->actionFunc = EnOwl_WaitDefault;
            self->unk_40A = 0;
            self->actionFlags |= 0x80;
            gTimeIncrement = 0;
        }
    }

    if (self->skelAnime.curFrame >= 37.0f) {
        if (self->unk_3FE > 0) {
            self->skelAnime.curFrame = 21.0f;
            self->unk_3FE--;
        } else {
            self->actionFunc = func_80ACBF50;
        }
    }

    self->actionFlags |= 8;
}

void func_80ACC23C(EnOwl* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame < 20.0f) {
        self->actor.speedXZ = 1.5f;
    } else {
        self->actor.speedXZ = 0.0f;
        Math_SmoothStepToS(&self->actor.world.rot.y, self->unk_400, 2, 0x384, 0x258);
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    if (self->skelAnime.curFrame >= 37.0f) {
        self->skelAnime.curFrame = 21.0f;
        self->actionFunc = func_80ACC00C;
        self->unk_3FE = 5;
        self->actor.velocity.y = 0.0f;
        self->actor.gravity = 0.0f;
        self->actor.speedXZ = 0.0f;
    }

    self->actionFlags |= 8;
}

void func_80ACC30C(EnOwl* self, GlobalContext* globalCtx) {
    if (self->actionFlags & 1) {
        self->unk_3FE = 3;
        EnOwl_ChangeMode(self, func_80ACC23C, func_80ACC540, &self->skelAnime, &gOwlTakeoffAnim, 0.0f);
        self->unk_3F8 = self->actor.world.pos.y;
        self->actor.velocity.y = 0.2f;
    }

    self->actionFlags |= 8;
}

void func_80ACC390(EnOwl* self) {
    SkelAnime_Update(self->curSkelAnime);

    if (self->unk_3FE > 0) {
        self->unk_3FE--;
        self->actor.shape.rot.z = Math_SinS(self->unk_3FE * 0x333) * 1000.0f;
    } else {
        self->unk_410 = func_80ACC460;
        self->unk_3FE = 6;
        Animation_Change(self->curSkelAnime, &gOwlFlyAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gOwlFlyAnim), 2, 5.0f);
    }
}

void func_80ACC460(EnOwl* self) {
    if (SkelAnime_Update(self->curSkelAnime)) {
        if (self->unk_3FE > 0) {
            self->unk_3FE--;
            Animation_Change(self->curSkelAnime, self->curSkelAnime->animation, 1.0f, 0.0f,
                             Animation_GetLastFrame(self->curSkelAnime->animation), ANIMMODE_ONCE, 0.0f);
        } else {
            self->unk_3FE = 0xA0;
            self->unk_410 = func_80ACC390;
            Animation_Change(self->curSkelAnime, &gOwlGlideAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gOwlGlideAnim), 0,
                             5.0f);
        }
    }
}

void func_80ACC540(EnOwl* self) {
    if (SkelAnime_Update(self->curSkelAnime)) {
        Animation_Change(self->curSkelAnime, self->curSkelAnime->animation, 1.0f, 0.0f,
                         Animation_GetLastFrame(self->curSkelAnime->animation), ANIMMODE_ONCE, 0.0f);
        self->actionFlags |= 1;
    } else {
        self->actionFlags &= ~1;
    }
}

s32 func_80ACC5CC(EnOwl* self) {
    s32 phi_v1 = (self->actionFlags & 2) ? 0x20 : 0;

    if (phi_v1 == (self->unk_3EE & 0x3F)) {
        return true;
    } else {
        if (self->actionFlags & 0x20) {
            self->unk_3EE += 4;
        } else {
            self->unk_3EE -= 4;
        }

        return false;
    }
}

s32 func_80ACC624(EnOwl* self, GlobalContext* globalCtx) {
    s32 switchFlag = (self->actor.params & 0xFC0) >> 6;

    if (globalCtx->sceneNum != SCENE_SPOT11) {
        return true;
    } else if (switchFlag == 0xA) {
        return true;
    } else if (globalCtx->csCtx.frames >= 300 && globalCtx->csCtx.frames <= 430) {
        return true;
    } else if (globalCtx->csCtx.frames >= 1080 && globalCtx->csCtx.frames <= 1170) {
        return true;
    } else {
        return false;
    }
}

void EnOwl_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnOwl* self = THIS;
    s16 phi_a1;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 10.0f, 5);
    self->unk_410(self);
    self->actionFlags &= ~8;
    self->actionFunc(self, globalCtx);
    if (self->actor.update == NULL) {
        // "Owl disappears"
        osSyncPrintf("フクロウ消滅!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return;
    }

    if (!(self->actionFlags & 0x80) && func_80ACC624(self, globalCtx)) {
        if ((self->skelAnime.animation == &gOwlTakeoffAnim &&
             (self->skelAnime.curFrame == 2.0f || self->skelAnime.curFrame == 9.0f ||
              self->skelAnime.curFrame == 23.0f || self->skelAnime.curFrame == 40.0f ||
              self->skelAnime.curFrame == 58.0f)) ||
            (self->skelAnime.animation == &gOwlFlyAnim && self->skelAnime.curFrame == 4.0f)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_OWL_FLUTTER);
        }
    }

    if (self->actor.draw != NULL) {
        Actor_MoveForward(&self->actor);
    }

    if (self->actionFlags & 2) {
        self->eyeTexIndex = 2;
    } else {
        if (DECR(self->blinkTimer) == 0) {
            self->blinkTimer = Rand_S16Offset(60, 60);
        }

        self->eyeTexIndex = self->blinkTimer;

        if (self->eyeTexIndex >= 3) {
            self->eyeTexIndex = 0;
        }
    }

    if (!(self->actionFlags & 8)) {
        phi_a1 = 0;
        if (self->actionFlags & 0x10) {
            switch (self->unk_404) {
                case 0:
                    self->unk_404 = 1;
                    self->unk_405 = 6;
                    break;
                case 1:
                    self->unk_405--;

                    if (self->unk_405 != 0) {
                        phi_a1 = Math_CosS(self->unk_405 * 8192) * 4096.0f;
                    } else {
                        if (self->actionFlags & 2) {
                            self->unk_3EE = 0;
                        } else {
                            self->unk_3EE = 0x20;
                        }

                        if (self->actionFlags & 0x20) {
                            self->unk_3EE -= 4;
                        } else {
                            self->unk_3EE += 4;
                        }
                        self->unk_404++;
                    }

                    if (self->actionFlags & 0x20) {
                        phi_a1 = -phi_a1;
                    }
                    break;
                case 2:
                    if (func_80ACC5CC(self)) {
                        self->actionFlags &= ~0x10;
                        self->unk_406 = (s32)Rand_ZeroFloat(20.0f) + 0x3C;
                        self->unk_404 = 0;
                        func_80ACA6C0(self);
                    }
                    break;
                default:
                    break;
            }
        } else {
            if (self->unk_406 > 0) {
                self->unk_406--;
            } else {
                if (self->unk_404 == 0) {
                    if (Rand_ZeroOne() < 0.3f) {
                        self->unk_404 = 4;
                        self->unk_405 = 0xC;
                    } else {
                        self->unk_404 = 1;
                        self->unk_405 = 4;
                    }
                }

                self->unk_405--;

                switch (self->unk_404) {
                    case 1:
                        phi_a1 = Math_SinS((-self->unk_405 * 4096) + 0x4000) * 5000.0f;
                        if (self->unk_405 <= 0) {
                            self->unk_405 = (s32)(Rand_ZeroFloat(15.0f) + 5.0f);
                            self->unk_404 = 2;
                        }
                        break;
                    case 2:
                        phi_a1 = 0x1388;
                        if (self->unk_405 <= 0) {
                            self->unk_404 = 3;
                            self->unk_405 = 4;
                        }
                        break;
                    case 3:
                        phi_a1 = Math_SinS(self->unk_405 * 4096) * 5000.0f;
                        if (self->unk_405 <= 0) {
                            self->unk_406 = (s32)Rand_ZeroFloat(20.0f) + 0x3C;
                            self->unk_404 = 0;
                            func_80ACA6C0(self);
                        }
                        break;
                    case 4:
                        phi_a1 = Math_SinS(self->unk_405 * 8192) * 5000.0f;
                        if (self->unk_405 <= 0) {
                            self->unk_406 = (s32)Rand_ZeroFloat(20.0f) + 0x3C;
                            self->unk_404 = 0;
                            func_80ACA6C0(self);
                        }
                        break;
                    default:
                        break;
                }

                if (self->actionFlags & 0x20) {
                    phi_a1 = -phi_a1;
                }
            }

            if (self->unk_409 > 0) {
                self->unk_409--;
            } else {
                self->unk_408--;
                switch (self->unk_407) {
                    case 0:
                        self->unk_3F2 = (-self->unk_408 * 0x5DC) + 0x1770;
                        if (self->unk_408 <= 0) {
                            self->unk_407 = 1;
                            self->unk_408 = (s32)(Rand_ZeroFloat(15.0f) + 5.0f);
                        }
                        break;
                    case 1:
                        self->unk_3F2 = 0x1770;
                        if (self->unk_408 <= 0) {
                            self->unk_407 = 2;
                            self->unk_408 = 4;
                        }
                        break;
                    case 2:
                        self->unk_3F2 = self->unk_408 * 0x5DC;
                        if (self->unk_408 <= 0) {
                            self->unk_407 = 0;
                            self->unk_408 = 4;
                            self->unk_409 = (s32)Rand_ZeroFloat(40.0f) + 0xA0;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        if (phi_a1) {}
        self->unk_3F0 = (u16)((self->unk_3EE << 2) << 8) + phi_a1;
        self->unk_3EC = ABS(self->unk_3F0) >> 3;
    } else {
        self->unk_3F2 = 0;
        if (self->actionFlags & 2) {
            self->unk_3F0 = -0x8000;
        } else {
            self->unk_3F0 = 0;
        }

        self->unk_3EC = ABS(self->unk_3F0) >> 3;
    }
}

s32 EnOwl_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** gfx, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnOwl* self = THIS;

    switch (limbIndex) {
        case 3:
            rot->x += self->unk_3F0;
            rot->z += self->unk_3EC;
            rot->z -= self->unk_3F2;
            break;
        case 2:
            rot->z += self->unk_3F2;
            break;
        case 4:
            if (!(self->actionFlags & 8)) {
                rot->y -= (s16)(self->unk_3EC * 1.5f);
            }
            break;
        case 5:
            if (!(self->actionFlags & 8)) {
                rot->y += (s16)(self->unk_3EC * 1.5f);
            }
            break;
        default:
            break;
    }
    return false;
}

void EnOwl_PostLimbUpdate(GlobalContext* globalCtx, s32 limbIndex, Gfx** gfx, Vec3s* rot, void* thisx) {
    EnOwl* self = THIS;
    Vec3f vec;

    vec.z = 0.0f;
    if (self->actionFlags & 2) {
        vec.x = 700.0f;
        vec.y = 400.0f;
    } else {
        vec.y = 0.0f;
        vec.x = 1400.0f;
    }
    if (limbIndex == 3) {
        Matrix_MultVec3f(&vec, &self->actor.focus.pos);
    }
}

void EnOwl_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { gObjOwlEyeOpenTex, gObjOwlEyeHalfTex, gObjOwlEyeClosedTex };
    EnOwl* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_owl.c", 2247);

    func_800943C8(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 8, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTexIndex]));
    SkelAnime_DrawFlexOpa(globalCtx, self->curSkelAnime->skeleton, self->curSkelAnime->jointTable,
                          self->curSkelAnime->dListCount, EnOwl_OverrideLimbDraw, EnOwl_PostLimbUpdate, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_owl.c", 2264);
}

void EnOwl_ChangeMode(EnOwl* self, EnOwlActionFunc actionFunc, OwlFunc arg2, SkelAnime* skelAnime,
                      AnimationHeader* animation, f32 morphFrames) {
    self->curSkelAnime = skelAnime;
    Animation_Change(self->curSkelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_ONCE,
                     morphFrames);
    self->actionFunc = actionFunc;
    self->unk_410 = arg2;
}

void func_80ACD130(EnOwl* self, GlobalContext* globalCtx, s32 idx) {
    Vec3f startPos;

    startPos.x = globalCtx->csCtx.npcActions[idx]->startPos.x;
    startPos.y = globalCtx->csCtx.npcActions[idx]->startPos.y;
    startPos.z = globalCtx->csCtx.npcActions[idx]->startPos.z;
    self->actor.world.pos = startPos;
    self->actor.world.rot.y = self->actor.shape.rot.y = globalCtx->csCtx.npcActions[idx]->rot.y;
    self->actor.shape.rot.z = globalCtx->csCtx.npcActions[idx]->urot.z;
}

f32 func_80ACD1C4(GlobalContext* globalCtx, s32 idx) {
    f32 ret = Environment_LerpWeight(globalCtx->csCtx.npcActions[idx]->endFrame,
                                     globalCtx->csCtx.npcActions[idx]->startFrame, globalCtx->csCtx.frames);

    ret = CLAMP_MAX(ret, 1.0f);
    return ret;
}

void func_80ACD220(EnOwl* self, Vec3f* arg1, f32 arg2) {
    Vec3f rpy;

    rpy.x = (arg1->x - self->actor.world.pos.x) * arg2;
    rpy.y = (arg1->y - self->actor.world.pos.y) * arg2;
    rpy.z = (arg1->z - self->actor.world.pos.z) * arg2;

    Math_StepToF(&self->actor.velocity.y, rpy.y, 1.0f);
    self->actor.speedXZ = sqrtf(SQ(rpy.x) + SQ(rpy.z));
    self->actor.world.rot.y = Math_Vec3f_Yaw(&self->actor.world.pos, arg1);
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void func_80ACD2CC(EnOwl* self, GlobalContext* globalCtx) {
    Vec3f pos;
    s32 angle;
    f32 t = func_80ACD1C4(globalCtx, 7);

    pos.x = globalCtx->csCtx.npcActions[7]->startPos.x;
    pos.y = globalCtx->csCtx.npcActions[7]->startPos.y;
    pos.z = globalCtx->csCtx.npcActions[7]->startPos.z;
    angle = (s16)globalCtx->csCtx.npcActions[7]->rot.y - self->actor.world.rot.z;
    if (angle < 0) {
        angle += 0x10000;
    }
    angle = (s16)((t * angle) + self->actor.world.rot.z);
    angle = (u16)angle;
    if (self->actionFlags & 4) {
        f32 phi_f2 = globalCtx->csCtx.npcActions[7]->urot.x;

        phi_f2 *= 10.0f * (360.0f / 0x10000);
        if (phi_f2 < 0.0f) {
            phi_f2 += 360.0f;
        }
        pos.x -= Math_SinS(angle) * phi_f2;
        pos.z += Math_CosS(angle) * phi_f2;
        self->unk_3F8 = phi_f2;
        self->actor.world.pos = pos;
        self->actor.draw = &EnOwl_Draw;
        self->actionFlags &= ~4;
        self->actor.speedXZ = 0.0f;
    } else {
        pos.x -= Math_SinS(angle) * self->unk_3F8;
        pos.z += Math_CosS(angle) * self->unk_3F8;
        func_80ACD220(self, &pos, 1.0f);
    }
}

void func_80ACD4D4(EnOwl* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Vec3f endPosf;
    f32 temp_ret = func_80ACD1C4(globalCtx, 7);

    pos.x = globalCtx->csCtx.npcActions[7]->startPos.x;
    pos.y = globalCtx->csCtx.npcActions[7]->startPos.y;
    pos.z = globalCtx->csCtx.npcActions[7]->startPos.z;
    endPosf.x = globalCtx->csCtx.npcActions[7]->endPos.x;
    endPosf.y = globalCtx->csCtx.npcActions[7]->endPos.y;
    endPosf.z = globalCtx->csCtx.npcActions[7]->endPos.z;
    pos.x = (endPosf.x - pos.x) * temp_ret + pos.x;
    pos.y = (endPosf.y - pos.y) * temp_ret + pos.y;
    pos.z = (endPosf.z - pos.z) * temp_ret + pos.z;
    func_80ACD220(self, &pos, 1.0f);
}
