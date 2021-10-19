#include "z_en_niw_lady.h"
#include "objects/object_ane/object_ane.h"
#include "objects/object_os_anime/object_os_anime.h"
#include "overlays/actors/ovl_En_Niw/z_en_niw.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnNiwLady*)thisx)

void EnNiwLady_Init(Actor* thisx, GlobalContext* globalCtx);
void EnNiwLady_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnNiwLady_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80AB9F24(EnNiwLady* self, GlobalContext* globalCtx);
void EnNiwLady_Draw(Actor* thisx, GlobalContext* globalCtx);
void func_80ABA21C(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABAD38(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABA778(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABA878(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABA9B8(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABAB08(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABAC00(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABAA9C(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABAC84(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABA244(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABA654(EnNiwLady* self, GlobalContext* globalCtx);
void func_80ABAD7C(EnNiwLady* self, GlobalContext* globalCtx);

const ActorInit En_Niw_Lady_InitVars = {
    ACTOR_EN_NIW_LADY,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ANE,
    sizeof(EnNiwLady),
    (ActorFunc)EnNiwLady_Init,
    (ActorFunc)EnNiwLady_Destroy,
    (ActorFunc)EnNiwLady_Update,
    NULL,
};

static s16 sMissingCuccoTextIds[] = {
    0x5036, 0x5070, 0x5072, 0x5037, 0x5038, 0x5039, 0x503A, 0x503B, 0x503D, 0x503C,
};

static s16 D_80ABB3B4[] = {
    0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000,
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
        BUMP_ON,
        OCELEM_ON,
    },
    { 10, 10, 0, { 0, 0, 0 } },
};

void EnNiwLady_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnNiwLady* self = THIS;

    self->objectAneIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_ANE);
    self->objectOsAnimeIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_OS_ANIME);
    if ((self->objectOsAnimeIndex < 0) || (self->objectAneIndex < 0)) {
        Actor_Kill(thisx);
        return;
    }
    self->unk_278 = 0;
    if (globalCtx->sceneNum == SCENE_LABO) {
        self->unk_278 = 1;
    }
    if ((self->unk_278 != 0) && IS_DAY) {
        Actor_Kill(thisx);
        return;
    }
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ ねぇちゃんうっふん ☆☆☆☆☆ %d\n" VT_RST, self->unk_278);
    osSyncPrintf("\n\n");
    self->actionFunc = func_80AB9F24;
    thisx->uncullZoneForward = 600.0f;
}

void EnNiwLady_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnNiwLady* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnNiwLady_ChoseAnimation(EnNiwLady* self, GlobalContext* globalCtx, s32 arg2) {
    f32 frames;

    if (Text_GetFaceReaction(globalCtx, 8) != 0) {
        arg2 = 8;
    }
    if (arg2 != self->unk_270) {
        self->unk_275 = 0;
        self->unk_276 = 1;
        self->unk_270 = arg2;
        switch (arg2) {
            case 10:
                self->unk_275 = 1;
            case 9:
                frames = Animation_GetLastFrame(&gObjOsAnim_07D0);
                Animation_Change(&self->skelAnime, &gObjOsAnim_07D0, 1.0f, 0.0f, frames, ANIMMODE_LOOP, -10.0f);
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 8:
            case 21:
            case 22:
            case 24:
            case 29:
                frames = Animation_GetLastFrame(&gObjOsAnim_9F94);
                Animation_Change(&self->skelAnime, &gObjOsAnim_9F94, 1.0f, 0.0f, frames, ANIMMODE_LOOP, -10.0f);
                break;
            case 7:
            case 20:
            case 23:
            case 25:
            case 26:
            case 27:
            case 28:
                frames = Animation_GetLastFrame(&gObjOsAnim_0718);
                Animation_Change(&self->skelAnime, &gObjOsAnim_0718, 1.0f, 0.0f, frames, ANIMMODE_LOOP, -10.0f);
                break;
            case 100:
                frames = Animation_GetLastFrame(&gObjOsAnim_A630);
                Animation_Change(&self->skelAnime, &gObjOsAnim_A630, 1.0f, 0.0f, frames, ANIMMODE_LOOP, -10.0f);
                self->unk_276 = 0;
                break;
        }
        return;
    }
}

void func_80AB9F24(EnNiwLady* self, GlobalContext* globalCtx) {
    f32 frames;
    s32 pad;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->objectAneIndex) &&
        Object_IsLoaded(&globalCtx->objectCtx, self->objectOsAnimeIndex)) {
        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->objectAneIndex].segment);
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gCuccoLadySkel, NULL, self->jointTable, self->morphTable, 16);
        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->objectOsAnimeIndex].segment);
        self->unk_27E = 1;
        self->actor.gravity = -3.0f;
        Actor_SetScale(&self->actor, 0.01f);
        ActorShape_Init(&self->actor.shape, 0.0f, &ActorShadow_DrawCircle, 20.0f);
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        self->unk_272 = 0;
        self->actor.targetMode = 6;
        self->actor.draw = EnNiwLady_Draw;
        switch (self->unk_278) {
            case 0:
                if (!(gSaveContext.itemGetInf[0] & 0x1000) && !LINK_IS_ADULT) {
                    frames = Animation_GetLastFrame(&gObjOsAnim_A630);
                    Animation_Change(&self->skelAnime, &gObjOsAnim_A630, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, 0.0f);
                } else {
                    frames = Animation_GetLastFrame(&gObjOsAnim_07D0);
                    Animation_Change(&self->skelAnime, &gObjOsAnim_07D0, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, 0.0f);
                }
                if (LINK_IS_ADULT) {
                    self->actionFunc = func_80ABA778;
                } else {
                    self->actionFunc = func_80ABA21C;
                }
                return;
            case 1:
                frames = Animation_GetLastFrame(&gObjOsAnim_07D0);
                Animation_Change(&self->skelAnime, &gObjOsAnim_07D0, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, 0.0f);
                self->actionFunc = func_80ABAD38;
                return;
        }
    }
}

void func_80ABA21C(EnNiwLady* self, GlobalContext* globalCtx) {
    self->actor.textId = sMissingCuccoTextIds[0];
    self->unk_262 = 6;
    self->actionFunc = func_80ABA244;
}

void func_80ABA244(EnNiwLady* self, GlobalContext* globalCtx) {
    EnNiw* currentCucco;
    s32 phi_s1;

    self->cuccosInPen = 0;
    currentCucco = (EnNiw*)globalCtx->actorCtx.actorLists[ACTORCAT_PROP].head;
    while (currentCucco != NULL) {
        if (currentCucco->actor.id == ACTOR_EN_NIW) {
            if ((fabsf(currentCucco->actor.world.pos.x - 330.0f) < 90.0f) &&
                (fabsf(currentCucco->actor.world.pos.z - 1610.0f) < 190.0f)) {
                if (self->unk_26C == 0) {
                    gSaveContext.infTable[25] |= D_80ABB3B4[currentCucco->unk_2AA];
                    if (BREG(1) != 0) {
                        // "GET inside the chicken fence!"
                        osSyncPrintf(VT_FGCOL(GREEN) "☆ 鶏柵内ＧＥＴ！☆ %x\n" VT_RST,
                                     D_80ABB3B4[currentCucco->unk_2AA]);
                    }
                }
                self->cuccosInPen++;
            } else if (self->unk_26C == 0) {
                gSaveContext.infTable[25] &= ~D_80ABB3B4[currentCucco->unk_2AA];
            }
        }
        currentCucco = (EnNiw*)currentCucco->actor.next;
    }
    if (BREG(7) != 0) {
        self->cuccosInPen = BREG(7) - 1;
    }
    phi_s1 = self->cuccosInPen;
    if ((func_8010BDBC(&globalCtx->msgCtx) == 0) || (func_8010BDBC(&globalCtx->msgCtx) == 6)) {
        self->unk_26E = 101;
    }
    if (self->cuccosInPen >= 7) {
        phi_s1 = 8;
        if ((self->unk_26C < 2) && (self->unk_26C == 0)) {
            phi_s1 = 7;
        }
    }
    if ((self->unk_26C != 0) && (phi_s1 < 7)) {
        phi_s1 = 9;
    }
    self->actor.textId = sMissingCuccoTextIds[phi_s1];
    if (Text_GetFaceReaction(globalCtx, 8) != 0) {
        self->actor.textId = Text_GetFaceReaction(globalCtx, 8);
        self->unk_262 = 6;
    }
    if ((self->unk_26C != 0) && (phi_s1 != 9)) {
        phi_s1 = 10;
        self->unk_26E = 11;
    }
    if (func_8002F194(&self->actor, globalCtx)) {
        osSyncPrintf("\n\n");
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ ねぇちゃん選択\t ☆☆☆☆ %d\n" VT_RST, phi_s1);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ ねぇちゃんハート     ☆☆☆☆ %d\n" VT_RST, self->unk_26C);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ ねぇちゃん保存       ☆☆☆☆ %d\n" VT_RST, self->unk_26A);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ ねぇちゃん今\t ☆☆☆☆ %d\n" VT_RST, self->cuccosInPen);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ self->actor.talk_message ☆☆ %x\n" VT_RST, self->actor.textId);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ self->message_end_code   ☆☆ %d\n" VT_RST, self->unk_262);
        osSyncPrintf("\n\n");
        if (Text_GetFaceReaction(globalCtx, 8) == 0) {
            if (self->actor.textId == 0x503C) {
                func_80078884(NA_SE_SY_ERROR);
                self->unk_26C = 2;
                self->unk_262 = 5;
                self->actionFunc = func_80ABA654;
                return;
            }
            self->unk_26E = phi_s1 + 1;
            if (phi_s1 == 7) {
                func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
                self->unk_26C = 1;
                self->unk_262 = 5;
                self->unk_26A = self->cuccosInPen;
                osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 柵内BIT変更前 ☆☆ %x\n" VT_RST, gSaveContext.infTable[25]);
                gSaveContext.infTable[25] &= 0x1FF;
                osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 柵内BIT変更後 ☆☆ %x\n" VT_RST, gSaveContext.infTable[25]);
                osSyncPrintf("\n\n");
                self->actionFunc = func_80ABA654;
                return;
            }
            if (self->unk_26A != self->cuccosInPen) {
                if (self->cuccosInPen < self->unk_26A) {
                    func_80078884(NA_SE_SY_ERROR);
                } else if (phi_s1 + 1 < 9) {
                    func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
                }
            }
            if (self->unk_26A < self->cuccosInPen) {
                self->unk_26A = self->cuccosInPen;
                return;
            }
        }
    } else {
        func_8002F2CC(&self->actor, globalCtx, 100.0f);
    }
}

void func_80ABA654(EnNiwLady* self, GlobalContext* globalCtx) {
    if (self->unk_262 == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx) != 0) {
        func_80106CCC(globalCtx);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ ハート ☆☆☆☆☆ %d\n" VT_RST, self->unk_26C);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 爆弾   ☆☆☆☆☆ %d\n" VT_RST, self->unk_272);
        osSyncPrintf("\n\n");
        self->unk_26E = 0xB;
        if (!(gSaveContext.itemGetInf[0] & 0x1000)) {
            self->actor.parent = NULL;
            self->getItemId = GI_BOTTLE;
            func_8002F434(&self->actor, globalCtx, GI_BOTTLE, 100.0f, 50.0f);
            self->actionFunc = func_80ABAC00;
            return;
        }
        if (self->unk_26C == 1) {
            self->getItemId = GI_RUPEE_PURPLE;
            func_8002F434(&self->actor, globalCtx, GI_RUPEE_PURPLE, 100.0f, 50.0f);
            self->actionFunc = func_80ABAC00;
        }
        self->actionFunc = func_80ABA244;
    }
}

static s16 sTradeItemTextIds[] = { 0x503E, 0x503F, 0x5047, 0x5040, 0x5042, 0x5043,
                                   0x5044, 0x00CF, 0x5045, 0x5042, 0x5027 };

void func_80ABA778(EnNiwLady* self, GlobalContext* globalCtx) {
    // "☆☆☆☆☆ Adult message check ☆☆☆☆☆"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ アダルトメッセージチェック ☆☆☆☆☆ \n" VT_RST);
    self->unk_262 = 6;
    self->unk_273 = 0;
    if (!(gSaveContext.itemGetInf[2] & 0x1000)) {
        if (self->unk_274 != 0) {
            self->unk_27A = 1;
        } else {
            self->unk_27A = 0;
        }
        self->unk_273 = 1;
        self->unk_262 = 4;
    } else {
        self->unk_27A = 2;
        if (!(gSaveContext.itemGetInf[2] & 0x4000)) {
            self->unk_27A = 3;
            if (gSaveContext.eventChkInf[6] & 0x400) {
                self->unk_27A = 9;
                if (self->unk_277 != 0) {
                    self->unk_27A = 10;
                }
            } else {
                self->unk_27A = 4;
            }
        }
    }
    self->actor.textId = sTradeItemTextIds[self->unk_27A];
    self->actionFunc = func_80ABA878;
}

void func_80ABA878(EnNiwLady* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s8 playerExchangeItemId;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 0) || (func_8010BDBC(&globalCtx->msgCtx) == 6)) {
        self->unk_26E = 11;
    }
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        playerExchangeItemId = func_8002F368(globalCtx);
        if ((playerExchangeItemId == 6) && (gSaveContext.eventChkInf[6] & 0x400)) {
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
            player->actor.textId = sTradeItemTextIds[5];
            self->unk_26E = self->unk_27A + 21;
            self->unk_262 = 4;
            self->actionFunc = func_80ABAB08;
        } else if (playerExchangeItemId != 0) {
            player->actor.textId = sTradeItemTextIds[7];
            self->unk_26E = self->unk_27A + 21;
        } else {
            self->unk_274 = 1;
            self->unk_26E = self->unk_27A + 21;
            self->actionFunc = !self->unk_273 ? func_80ABA778 : func_80ABA9B8;
        }
    } else {
        func_8002F298(&self->actor, globalCtx, 50.0f, 6);
    }
}

void func_80ABA9B8(EnNiwLady* self, GlobalContext* globalCtx) {
    if ((self->unk_262 == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                func_80106CCC(globalCtx);
                self->actor.parent = NULL;
                func_8002F434(&self->actor, globalCtx, GI_POCKET_EGG, 200.0f, 100.0f);
                self->actionFunc = func_80ABAC00;
                break;
            case 1:
                self->actor.textId = sTradeItemTextIds[3];
                self->unk_26E = self->unk_27A + 21;
                func_8010B720(globalCtx, self->actor.textId);
                self->unk_262 = 5;
                self->actionFunc = func_80ABAA9C;
                break;
        }
    }
}

void func_80ABAA9C(EnNiwLady* self, GlobalContext* globalCtx) {
    self->unk_26E = 11;
    if ((self->unk_262 == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->actionFunc = func_80ABA778;
    }
}

void func_80ABAB08(EnNiwLady* self, GlobalContext* globalCtx) {
    if ((self->unk_262 == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                func_80106CCC(globalCtx);
                self->actor.parent = NULL;
                func_8002F434(&self->actor, globalCtx, GI_COJIRO, 200.0f, 100.0f);
                self->actionFunc = func_80ABAC00;
                break;
            case 1:
                func_80106CCC(globalCtx);
                self->unk_277 = 1;
                self->actor.textId = sTradeItemTextIds[8];
                self->unk_26E = self->unk_27A + 21;
                func_8010B720(globalCtx, self->actor.textId);
                self->unk_262 = 5;
                self->actionFunc = func_80ABAA9C;
                break;
        }
    }
}

void func_80ABAC00(EnNiwLady* self, GlobalContext* globalCtx) {
    s32 getItemId;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actionFunc = func_80ABAC84;
    } else {
        getItemId = self->getItemId;
        if (LINK_IS_ADULT) {
            getItemId = !(gSaveContext.itemGetInf[2] & 0x1000) ? GI_POCKET_EGG : GI_COJIRO;
        }
        func_8002F434(&self->actor, globalCtx, getItemId, 200.0f, 100.0f);
    }
}

void func_80ABAC84(EnNiwLady* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) != 6) || (func_80106BC8(globalCtx) == 0)) {
        return;
    }
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 正常終了 ☆☆☆☆☆ \n" VT_RST);
    if (LINK_IS_ADULT) {
        if (!(gSaveContext.itemGetInf[2] & 0x1000)) {
            gSaveContext.itemGetInf[2] |= 0x1000;
        } else {
            gSaveContext.itemGetInf[2] |= 0x4000;
        }
        self->actionFunc = func_80ABA778;
    } else {
        gSaveContext.itemGetInf[0] |= 0x1000;
        self->unk_262 = 6;
        self->actionFunc = func_80ABA244;
    }
}

void func_80ABAD38(EnNiwLady* self, GlobalContext* globalCtx) {
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 通常メッセージチェック ☆☆☆☆☆ \n" VT_RST);
    self->unk_262 = 6;
    self->actionFunc = func_80ABAD7C;
}

void func_80ABAD7C(EnNiwLady* self, GlobalContext* globalCtx) {
    self->actor.textId = 0x503D;
    if (Text_GetFaceReaction(globalCtx, 8) != 0) {
        self->actor.textId = Text_GetFaceReaction(globalCtx, 8);
    }
    if ((func_8010BDBC(&globalCtx->msgCtx) == 0) || (func_8010BDBC(&globalCtx->msgCtx) == 6)) {
        self->unk_26E = 8;
    }
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        self->unk_274 = 1;
        self->unk_26E = self->unk_27A + 9;
        self->actionFunc = func_80ABAD38;
    } else {
        func_8002F2CC(&self->actor, globalCtx, 100.0f);
    }
}

void EnNiwLady_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnNiwLady* self = THIS;
    Player* player = GET_PLAYER(globalCtx);

    Actor_SetFocus(thisx, 60.0f);
    self->unk_288.unk_18 = player->actor.world.pos;
    if (!LINK_IS_ADULT) {
        self->unk_288.unk_18.y = player->actor.world.pos.y - 10.0f;
    }
    func_80034A14(thisx, &self->unk_288, 2, 4);
    self->unk_254 = self->unk_288.unk_08;
    self->unk_25A = self->unk_288.unk_0E;
    if (self->unk_276 == 0) {
        Math_SmoothStepToS(&self->unk_254.y, 0, 5, 3000, 0);
    }
    gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->objectOsAnimeIndex].segment);
    if (self->objectOsAnimeIndex >= 0) {
        if (self->unk_27E != 0) {
            if (self->unk_26E != 0) {
                self->unk_26E--;
                EnNiwLady_ChoseAnimation(self, globalCtx, self->unk_26E);
                self->unk_26E = 0;
            }
            SkelAnime_Update(&self->skelAnime);
        }
        self->objectAneIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_ANE);
        if (self->objectAneIndex >= 0) {
            self->actionFunc(self, globalCtx);
            if (self->unusedTimer2 != 0) {
                self->unusedTimer2--;
            }
            if (self->unusedRandomTimer != 0) {
                self->unusedRandomTimer--;
            }
            self->unusedTimer++;
            if (self->unusedRandomTimer == 0) {
                self->faceState++;
                if (self->faceState >= 3) {
                    self->faceState = 0;
                    self->unusedRandomTimer = ((s16)Rand_ZeroFloat(60.0f) + 0x14);
                }
            }
            Actor_UpdateBgCheckInfo(globalCtx, thisx, 20.0f, 20.0f, 60.0f, 0x1D);
            Collider_UpdateCylinder(thisx, &self->collider);
            if (1) {}
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

Gfx* func_80ABB0A0(GraphicsContext* gfxCtx) {
    Gfx* dList;

    dList = Graph_Alloc(gfxCtx, sizeof(Gfx));
    gSPEndDisplayList(dList);
    return dList;
}

s32 EnNiwLady_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                               void* thisx) {
    EnNiwLady* self = THIS;
    s32 pad;

    if (limbIndex == 15) {
        rot->x += self->unk_254.y;
        rot->z += self->unk_254.x;
    }
    if (limbIndex == 8) {
        rot->x += self->unk_25A.y;
    }
    if (self->unk_275 != 0) {
        if ((limbIndex == 8) || (limbIndex == 10) || (limbIndex == 13)) {
            rot->y += (Math_SinS((globalCtx->state.frames * ((limbIndex * 0x32) + 0x814))) * 200.0f);
            rot->z += (Math_CosS((globalCtx->state.frames * ((limbIndex * 0x32) + 0x940))) * 200.0f);
        }
    }
    return false;
}

void EnNiwLady_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* sEyeTextures[] = { gCuccoLadyEyeOpenTex, gCuccoLadyEyeHalfTex, gCuccoLadyEyeClosedTex };
    EnNiwLady* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_niw_lady.c", 1347);
    if (self->unk_27E != 0) {
        func_80093D18(globalCtx->state.gfxCtx);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->faceState]));
        gSPSegment(POLY_OPA_DISP++, 0x0C, func_80ABB0A0(globalCtx->state.gfxCtx));
        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnNiwLady_OverrideLimbDraw, NULL, self);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_niw_lady.c", 1370);
}
