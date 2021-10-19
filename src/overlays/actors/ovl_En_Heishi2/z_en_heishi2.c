/*
 * File: z_en_heishi2.c
 * Overlay: ovl_En_Heishi2
 * Description: Hyrulian Guards
 */

#include "vt.h"
#include "z_en_heishi2.h"
#include "objects/object_sd/object_sd.h"
#include "objects/object_link_child/object_link_child.h"
#include "overlays/actors/ovl_Bg_Gate_Shutter/z_bg_gate_shutter.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "overlays/actors/ovl_Bg_Spot15_Saku/z_bg_spot15_saku.h"

#define FLAGS 0x00000009

#define THIS ((EnHeishi2*)thisx)

void EnHeishi2_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi2_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi2_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi2_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnHeishi2_DrawKingGuard(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi2_DoNothing1(EnHeishi2* self, GlobalContext* globalCtx);
void EnHeishi_DoNothing2(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A531E4(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53278(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A5344C(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A54954(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53538(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A535BC(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A5399C(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53638(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A5372C(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A5475C(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53AD4(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53C0C(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53C90(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53D0C(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A544AC(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53908(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53F30(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A54038(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A5427C(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A549E8(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53850(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A54320(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A543A0(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A5455C(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A546DC(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A541FC(EnHeishi2* self, GlobalContext* globalCtx);
void func_80A53DF8(EnHeishi2* self, GlobalContext* globalCtx);

const ActorInit En_Heishi2_InitVars = {
    ACTOR_EN_HEISHI2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi2),
    (ActorFunc)EnHeishi2_Init,
    (ActorFunc)EnHeishi2_Destroy,
    (ActorFunc)EnHeishi2_Update,
    (ActorFunc)EnHeishi2_Draw,
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
    { 33, 40, 0, { 0, 0, 0 } },
};

void EnHeishi2_Init(Actor* thisx, GlobalContext* globalCtx) {
    ColliderCylinder* collider;
    EnHeishi2* self = THIS;

    Actor_SetScale(&self->actor, 0.01f);
    self->type = self->actor.params & 0xFF;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;

    if ((self->type == 6) || (self->type == 9)) {
        self->actor.draw = EnHeishi2_DrawKingGuard;
        self->actor.flags &= -2;
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, 6);
        if (self->type == 6) {
            self->actionFunc = EnHeishi2_DoNothing1;

        } else {
            osSyncPrintf("\n\n");
            // "No, I'm completely disappointed" (message for when shooting guard window in courtyard)
            osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆☆☆☆ いやー ついうっかり ☆☆☆☆☆ \n" VT_RST);

            Actor_SetScale(&self->actor, 0.02f);

            self->unk_274 = self->actor.world.pos;
            self->actor.world.rot.y = 0x7918;
            self->actor.world.pos.x += 90.0f;
            self->actor.world.pos.y -= 60.0f;
            self->actor.world.pos.z += 90.0f;
            self->actor.shape.rot.y = self->actor.world.rot.y;
            Collider_DestroyCylinder(globalCtx, &self->collider);
            func_8002DF54(globalCtx, 0, 8);
            self->actor.flags |= 0x11;
            self->actionFunc = func_80A544AC;
        }
    } else {
        self->unk_2E0 = 60.0f;
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
        SkelAnime_Init(globalCtx, &self->skelAnime, &gEnHeishiSkel, &gEnHeishiIdleAnim, self->jointTable,
                       self->morphTable, 17);
        collider = &self->collider;
        Collider_InitCylinder(globalCtx, collider);
        Collider_SetCylinder(globalCtx, collider, &self->actor, &sCylinderInit);
        self->collider.dim.yShift = 0;
        self->collider.dim.radius = 15;
        self->collider.dim.height = 70;
        self->actor.targetMode = 6;

        switch (self->type) {

            case 2:
                self->actionFunc = func_80A531E4;
                self->actor.gravity = -1.0f;
                break;
            case 5:
                self->actionFunc = func_80A53908;
                self->actor.gravity = -1.0f;
                break;
            case 6:
                osSyncPrintf("\n\n");
                // "Peep hole soldier!"
                osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 覗き穴奥兵士ふぃ〜 ☆☆☆☆☆ \n" VT_RST);
                Collider_DestroyCylinder(globalCtx, collider);
                self->actor.flags &= -0xA;
                self->actionFunc = EnHeishi_DoNothing2;
                break;
        }

        self->unk_2F0 = (self->actor.params >> 8) & 0xFF;
        osSyncPrintf("\n\n");
        // "Soldier Set 2 Completed!"
        osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 兵士２セット完了！ ☆☆☆☆☆ %d\n" VT_RST, self->actor.params);
        // "Identification Completed!"
        osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ 識別完了！         ☆☆☆☆☆ %d\n" VT_RST, self->type);
        // "Message completed!"
        osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆☆☆☆ メッセージ完了！   ☆☆☆☆☆ %x\n\n" VT_RST, (self->actor.params >> 8) & 0xF);
    }
}

void EnHeishi2_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi2* self = THIS;

    if ((self->collider.dim.radius != 0) || (self->collider.dim.height != 0)) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void EnHeishi2_DoNothing1(EnHeishi2* self, GlobalContext* globalCtx) {
}

void EnHeishi_DoNothing2(EnHeishi2* self, GlobalContext* globalCtx) {
}

void func_80A531E4(EnHeishi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = func_80A53278;
}

void func_80A53278(EnHeishi2* self, GlobalContext* globalCtx) {
    self->unk_30B = 0;
    self->unk_30E = 0;
    if (Text_GetFaceReaction(globalCtx, 5) != 0) {
        self->actor.textId = Text_GetFaceReaction(globalCtx, 5);
        self->unk_30B = 1;
        self->unk_300 = 6;
        self->actionFunc = func_80A5475C;
    } else if ((gSaveContext.eventChkInf[0] & 0x200) && (gSaveContext.eventChkInf[2] & 0x20) &&
               (gSaveContext.eventChkInf[3] & 0x80)) {
        // "Get all spiritual stones!"
        osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 全部の精霊石GET！ ☆☆☆☆☆ \n" VT_RST);
        self->unk_300 = 6;
        self->actor.textId = 0x7006;
        self->actionFunc = func_80A5475C;
    } else if (!IS_DAY) {
        // "Sleep early for children!"
        osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ 子供ははやくネロ！ ☆☆☆☆☆ \n" VT_RST);
        self->unk_300 = 6;
        self->actor.textId = 0x7002;
        self->actionFunc = func_80A5475C;

    } else if (self->unk_30C != 0) {
        // "Anything passes"
        osSyncPrintf(VT_FGCOL(BLUE) " ☆☆☆☆☆ なんでも通るよ ☆☆☆☆☆ \n" VT_RST);
        self->unk_300 = 6;
        self->actor.textId = 0x7099;
        self->actionFunc = func_80A5475C;
    } else if (gSaveContext.eventChkInf[1] & 4) {
        if (self->unk_30E == 0) {
            // "Start under the first sleeve!"
            osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆☆☆☆ １回目袖の下開始！ ☆☆☆☆☆ \n" VT_RST);
            self->actor.textId = 0x7071;
            self->unk_30E = 1;
        } else {
            // "Start under the second sleeve!"
            osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆☆☆☆ ２回目袖の下開始！ ☆☆☆☆☆ \n" VT_RST);
            self->actor.textId = 0x7072;
        }
        self->unk_300 = 4;
        self->actionFunc = func_80A5475C;

    } else {
        // "That's okay"
        osSyncPrintf(VT_FGCOL(CYAN) " ☆☆☆☆☆ それはとおらんよぉ ☆☆☆☆☆ \n" VT_RST);
        self->unk_300 = 6;
        self->actor.textId = 0x7029;
        self->actionFunc = func_80A5475C;
    }
}

void func_80A5344C(EnHeishi2* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx) != 0)) {
        self->unk_300 = 5;
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                if (gSaveContext.rupees >= 10) {
                    Rupees_ChangeBy(-10);
                    self->actor.textId = 0x7098;
                    self->actionFunc = func_80A53538;
                    break;
                } else {
                    self->actor.textId = 0x7097;
                    self->actionFunc = func_80A54954;
                    break;
                }
            case 1:
                self->actor.textId = 0x7096;
                self->actionFunc = func_80A54954;
                break;

            default:
                break;
        }
        func_8010B720(globalCtx, self->actor.textId);
    }
}

void func_80A53538(EnHeishi2* self, GlobalContext* globalCtx) {

    SkelAnime_Update(&self->skelAnime);
    if (self->unk_300 == func_8010BDBC(&globalCtx->msgCtx)) {
        if (func_80106BC8(globalCtx) != 0) {
            func_8002DF54(globalCtx, NULL, 8);
            globalCtx->msgCtx.msgMode = 0x37;
            self->actionFunc = func_80A535BC;
        }
    }
}

void func_80A535BC(EnHeishi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiSlamSpearAnim);

    self->unk_2EC = frameCount;
    Animation_Change(&self->skelAnime, &gEnHeishiSlamSpearAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, -10.0f);
    self->actionFunc = func_80A53638;
}

void func_80A53638(EnHeishi2* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 frameCount = self->skelAnime.curFrame;

    BgSpot15Saku* actor = (BgSpot15Saku*)globalCtx->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;

    SkelAnime_Update(&self->skelAnime);
    if ((frameCount >= 12.0f) && (!self->audioFlag)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_SPEAR_HIT);
        self->audioFlag = 1;
    }
    if (self->unk_2EC <= frameCount) {
        while (actor != NULL) {
            if (actor->dyna.actor.id != ACTOR_BG_SPOT15_SAKU) {
                actor = (BgSpot15Saku*)(actor->dyna.actor.next);
            } else {
                self->gate = &actor->dyna.actor;
                actor->unk_168 = 1;
                break;
            }
        }
        // "I've come!"
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆ きたきたきたぁ！ ☆☆☆ %x\n" VT_RST, actor->dyna.actor.next);
        self->actionFunc = func_80A5372C;
    }
}

void func_80A5372C(EnHeishi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->unk_2F2[0] = 200;
    self->cameraId = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, self->cameraId, CAM_STAT_ACTIVE);
    self->unk_280.x = 947.0f;
    self->unk_280.y = 1195.0f;
    self->unk_280.z = 2682.0f;

    self->unk_28C.x = 1164.0f;
    self->unk_28C.y = 1145.0f;
    self->unk_28C.z = 3014.0f;

    Gameplay_CameraSetAtEye(globalCtx, self->cameraId, &self->unk_280, &self->unk_28C);
    self->actionFunc = func_80A53850;
}

void func_80A53850(EnHeishi2* self, GlobalContext* globalCtx) {
    BgSpot15Saku* gate;

    SkelAnime_Update(&self->skelAnime);
    Gameplay_CameraSetAtEye(globalCtx, self->cameraId, &self->unk_280, &self->unk_28C);
    gate = (BgSpot15Saku*)self->gate;
    if ((self->unk_2F2[0] == 0) || (gate->unk_168 == 0)) {
        Gameplay_ClearCamera(globalCtx, self->cameraId);
        Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
        func_80106CCC(globalCtx);
        self->unk_30C = 1;
        func_8002DF54(globalCtx, NULL, 7);
        self->actionFunc = func_80A531E4;
    }
}

void func_80A53908(EnHeishi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = func_80A5399C;
}

void func_80A5399C(EnHeishi2* self, GlobalContext* globalCtx) {
    s16 var;

    self->unk_30B = 0;
    var = 0;
    if (gSaveContext.infTable[7] & 0x40) {
        if (!(gSaveContext.infTable[7] & 0x80)) {
            if (Player_GetMask(globalCtx) == PLAYER_MASK_KEATON) {
                if (self->unk_309 == 0) {
                    self->actor.textId = 0x200A;
                } else {
                    self->actor.textId = 0x200B;
                }
                self->unk_300 = 4;
                self->unk_30B = 1;
                var = 1;
            } else {
                self->actor.textId = 0x2016;
                self->unk_300 = 6;
                var = 1;
            }
        } else {
            self->actor.textId = 0x2020;
            self->unk_300 = 5;
            self->unk_30E = 0;
        }
        if (Text_GetFaceReaction(globalCtx, 5) != 0) {
            if (var == 0) {
                self->actor.textId = Text_GetFaceReaction(globalCtx, 5);
                self->unk_30B = 1;
                self->unk_300 = 6;
                self->unk_30E = 0;
            }
        }
        self->actionFunc = func_80A5475C;
    } else {
        // "I don't know"
        osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆☆☆☆ とおしゃしねぇちゅーの ☆☆☆☆☆ \n" VT_RST);
        self->actionFunc = func_80A53AD4;
    }
}

void func_80A53AD4(EnHeishi2* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 exchangeItemId;
    s16 yawDiffTemp;
    s16 yawDiff;

    SkelAnime_Update(&self->skelAnime);
    if (Text_GetFaceReaction(globalCtx, 5) != 0) {
        self->actor.textId = Text_GetFaceReaction(globalCtx, 5);
    } else {
        self->actor.textId = 0x200E;
    }
    self->unk_300 = 6;
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        exchangeItemId = func_8002F368(globalCtx);
        if (exchangeItemId == EXCH_ITEM_LETTER_ZELDA) {
            func_80078884(NA_SE_SY_CORRECT_CHIME);
            player->actor.textId = 0x2010;
            self->unk_300 = 5;
            self->actionFunc = func_80A53C0C;
        } else if (exchangeItemId != EXCH_ITEM_NONE) {
            player->actor.textId = 0x200F;
        }
    } else {
        yawDiffTemp = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        yawDiff = ABS(yawDiffTemp);
        if (!(120.0f < self->actor.xzDistToPlayer) && (yawDiff < 0x4300)) {
            func_8002F298(&self->actor, globalCtx, 100.0f, EXCH_ITEM_LETTER_ZELDA);
        }
    }
}

void func_80A53C0C(EnHeishi2* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_300 == func_8010BDBC(&globalCtx->msgCtx)) {
        if (func_80106BC8(globalCtx) != 0) {
            func_8002DF54(globalCtx, 0, 8);
            globalCtx->msgCtx.msgMode = 0x37;
            self->actionFunc = func_80A53C90;
        }
    }
}

void func_80A53C90(EnHeishi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiSlamSpearAnim);

    self->unk_2EC = frameCount;
    Animation_Change(&self->skelAnime, &gEnHeishiSlamSpearAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, -10.0f);
    self->actionFunc = func_80A53D0C;
}

void func_80A53D0C(EnHeishi2* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 frameCount;
    BgGateShutter* gate;

    frameCount = self->skelAnime.curFrame;
    gate = (BgGateShutter*)globalCtx->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;
    SkelAnime_Update(&self->skelAnime);
    if (12.0f <= frameCount) {
        if (self->audioFlag == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_SPEAR_HIT);
            self->audioFlag = 1;
        }
    }
    if (self->unk_2EC <= frameCount) {
        while (gate != NULL) {
            if (gate->dyna.actor.id != ACTOR_BG_GATE_SHUTTER) {
                gate = (BgGateShutter*)gate->dyna.actor.next;
            } else {
                self->gate = &gate->dyna.actor;
                gate->openingState = 1;
                break;
            }
        }
        // "I've come!"
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆ きたきたきたぁ！ ☆☆☆ %x\n" VT_RST, gate->dyna.actor.next);
        self->actionFunc = func_80A53DF8;
    }
}

void func_80A53DF8(EnHeishi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->unk_2F2[0] = 200;
    self->cameraId = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, self->cameraId, CAM_STAT_ACTIVE);
    self->unk_2BC.x = -71.0f;
    self->unk_280.x = -71.0f;
    self->unk_2BC.y = 571.0f;
    self->unk_280.y = 571.0f;
    self->unk_2BC.z = -1487.0f;
    self->unk_280.z = -1487.0f;
    self->unk_298.x = 181.0f;
    self->unk_28C.x = 181.0f;
    self->unk_298.y = 417.0f;
    self->unk_28C.y = 417.0f;
    self->unk_298.z = -1079.0f;
    self->unk_28C.z = -1079.0f;
    Gameplay_CameraSetAtEye(globalCtx, self->cameraId, &self->unk_280, &self->unk_28C);
    self->actionFunc = func_80A53F30;
}

void func_80A53F30(EnHeishi2* self, GlobalContext* globalCtx) {
    BgGateShutter* gate;

    SkelAnime_Update(&self->skelAnime);
    Gameplay_CameraSetAtEye(globalCtx, self->cameraId, &self->unk_280, &self->unk_28C);
    gate = (BgGateShutter*)self->gate;
    if ((self->unk_2F2[0] == 0) || (gate->openingState == 0)) {
        Gameplay_ClearCamera(globalCtx, self->cameraId);
        Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
        if ((self->unk_30A != 2)) {
            if (self->unk_30A == 0) {
                self->actor.textId = 0x2015;
                func_8010B720(globalCtx, self->actor.textId);
                self->actionFunc = func_80A54038;
            } else {
                func_80106CCC(globalCtx);
                func_8002DF54(globalCtx, NULL, 7);
                self->actionFunc = func_80A53908;
            }
        } else {
            self->unk_30E = 0;
            self->actor.textId = 0x2021;
            Rupees_ChangeBy(15);
            func_8010B720(globalCtx, self->actor.textId);
            self->actionFunc = func_80A5427C;
        }
    }
}

void func_80A54038(EnHeishi2* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (func_8010BDBC(&globalCtx->msgCtx) == 5) {
        if (func_80106BC8(globalCtx) != 0) {
            gSaveContext.infTable[7] |= 0x40;
            func_80106CCC(globalCtx);
            func_8002DF54(globalCtx, 0, 7);
            self->actionFunc = func_80A53908;
        }
    }
}

void func_80A540C0(EnHeishi2* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx) != 0)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                self->actor.textId = 0x2020;
                func_8010B720(globalCtx, self->actor.textId);
                Player_UnsetMask(globalCtx);
                gSaveContext.infTable[7] |= 0x80;
                gSaveContext.itemGetInf[3] |= 0x100;
                Item_Give(globalCtx, ITEM_SOLD_OUT);
                if (self->unk_30A != 0) {
                    self->unk_30A = 2;
                    self->unk_30E = 1;
                    self->actionFunc = func_80A5427C;
                } else {
                    self->unk_30E = 0;
                    self->actionFunc = func_80A541FC;
                }
                break;
            case 1:
                self->unk_30E = 1;
                self->actor.textId = 0x200C;
                func_8010B720(globalCtx, self->actor.textId);
                self->unk_300 = 5;
                if (self->unk_30A == 0) {
                    self->actionFunc = func_80A5427C;
                } else {
                    self->actionFunc = func_80A54954;
                }
        }
    }
}

void func_80A541FC(EnHeishi2* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        self->actor.textId = 0x2021;
        Rupees_ChangeBy(15);
        func_8010B720(globalCtx, self->actor.textId);
        self->actionFunc = func_80A5427C;
    }
}

void func_80A5427C(EnHeishi2* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        if (self->unk_30E == 0) {
            self->unk_30E = 0;
            self->unk_30A = self->unk_30E;
            func_80106CCC(globalCtx);
            self->actionFunc = func_80A53908;
        } else {
            globalCtx->msgCtx.msgMode = 0x37;
            self->actionFunc = func_80A54320;
        }
    }
}

void func_80A54320(EnHeishi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiSlamSpearAnim);

    self->unk_2EC = frameCount;
    Animation_Change(&self->skelAnime, &gEnHeishiSlamSpearAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, -10.0f);
    self->audioFlag = 0;
    self->actionFunc = func_80A543A0;
}

void func_80A543A0(EnHeishi2* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 frameCount = self->skelAnime.curFrame;
    BgGateShutter* gate = (BgGateShutter*)(globalCtx->actorCtx.actorLists[ACTORCAT_ITEMACTION].head);
    SkelAnime_Update(&self->skelAnime);

    if ((frameCount >= 12.0f) && (!self->audioFlag)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_SPEAR_HIT);
        self->audioFlag = 1;
    }

    if (self->unk_2EC <= frameCount) {
        while (gate != NULL) {
            if (ACTOR_BG_GATE_SHUTTER != gate->dyna.actor.id) {
                gate = (BgGateShutter*)(gate->dyna.actor.next);
            } else {
                self->gate = &gate->dyna.actor;
                if (self->unk_30A != 2) {
                    gate->openingState = -1;
                    break;
                } else {
                    gate->openingState = 2;
                    break;
                }
            }
        }
        if (self->unk_30A == 0) {
            self->unk_30A = 1;
        }
        self->actionFunc = func_80A53DF8;
    }
}

void func_80A544AC(EnHeishi2* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.z, -6100, 5, self->unk_2E4, 0);
    Math_ApproachF(&self->unk_2E4, 3000.0f, 1.0f, 500.0f);
    self->actor.world.rot.z = self->actor.shape.rot.z;
    if (self->actor.shape.rot.z < -6000) {
        func_8010B680(globalCtx, 0x708F, NULL);
        self->actor.flags |= 0x10000;
        self->actionFunc = func_80A5455C;
        self->unk_2E4 = 0.0f;
    }
}

void func_80A5455C(EnHeishi2* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f pos;
    f32 rotY;
    EnBom* bomb;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_8002DF54(globalCtx, NULL, 7);
        func_80106CCC(globalCtx);

        pos.x = Rand_CenteredFloat(20.0f) + self->unk_274.x;
        pos.y = Rand_CenteredFloat(20.0f) + (self->unk_274.y - 40.0f);
        pos.z = Rand_CenteredFloat(20.0f) + (self->unk_274.z - 20.0f);
        rotY = Rand_CenteredFloat(7000.0f) + self->actor.yawTowardsPlayer;
        bomb = (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, pos.x, pos.y, pos.z, 0, rotY, 0, 0);
        if (bomb != NULL) {
            bomb->actor.speedXZ = Rand_CenteredFloat(5.0f) + 10.0f;
            bomb->actor.velocity.y = Rand_CenteredFloat(5.0f) + 10.0f;
        }

        // "This is down!"
        osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ これでダウンだ！ ☆☆☆☆☆ \n" VT_RST);
        self->actionFunc = func_80A546DC;
    }
}

void func_80A546DC(EnHeishi2* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.z, 200, 5, self->unk_2E4, 0);
    Math_ApproachF(&self->unk_2E4, 3000.0f, 1.0f, 500.0f);
    self->actor.world.rot.z = self->actor.shape.rot.z;
    if (self->actor.shape.rot.z > 0) {
        Actor_Kill(&self->actor);
    }
}

void func_80A5475C(EnHeishi2* self, GlobalContext* globalCtx) {
    s16 yawDiff;

    SkelAnime_Update(&self->skelAnime);

    if (Text_GetFaceReaction(globalCtx, 5) != 0) {
        if (self->unk_30B == 0) {
            if (self->type == 2) {
                self->actionFunc = func_80A53278;
                return;
            }
            if (self->type == 5) {
                self->actionFunc = func_80A5399C;
                return;
            }
        }
    } else if (self->unk_30B != 0) {
        if (self->type == 2) {
            self->actionFunc = func_80A53278;
            return;
        }
        if (self->type == 5) {
            self->actionFunc = func_80A5399C;
            return;
        }
    }

    if (func_8002F194(&self->actor, globalCtx)) {
        if (self->type == 2) {
            if (self->unk_30E == 1) {
                self->actionFunc = func_80A5344C;
                return;
            } else {
                self->actionFunc = func_80A53278;
                return;
            }
        } else if (self->type == 5) {
            if (self->unk_300 == 6) {
                self->actionFunc = func_80A5399C;
            }

            if (self->unk_300 == 5) {
                self->actionFunc = func_80A54954;
            }

            if (self->unk_300 == 4) {
                self->unk_309 = 1;
                func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
                self->actionFunc = func_80A540C0;
            }
            return;
        }
    }

    if (((self->type != 2) && (self->type != 5)) ||
        ((yawDiff = ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)),
          !(self->actor.xzDistToPlayer > 120.0f)) &&
         (yawDiff < 0x4300))) {
        func_8002F2F4(&self->actor, globalCtx);
    }
}

void func_80A54954(EnHeishi2* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = func_80A549E8;
}

void func_80A549E8(EnHeishi2* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->unk_300 == func_8010BDBC(&globalCtx->msgCtx)) {
        if (func_80106BC8(globalCtx) != 0) {
            func_80106CCC(globalCtx);
            if (self->type == 2) {
                self->actionFunc = func_80A531E4;
            }
            if (self->type == 5) {
                self->actionFunc = func_80A53908;
            }
        }
    }
}

void EnHeishi2_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnHeishi2* self = THIS;
    s32 i;

    Actor_SetFocus(&self->actor, self->unk_2E0);
    if ((self->type == 2) || (self->type == 5)) {
        self->actor.focus.pos.y = 70.0f;
        Actor_SetFocus(&self->actor, 70.0f);
        func_80038290(globalCtx, &self->actor, &self->unk_260, &self->unk_26C, self->actor.focus.pos);
    }

    self->unk_2FC++;

    for (i = 0; i != 5; i++) {
        if (self->unk_2F2[i] != 0) {
            self->unk_2F2[i]--;
        }
    }
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    switch (self->type) {
        case 6:
            break;
        case 9:
            break;
        default:
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 30.0f, 0x1D);
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            break;
    }
}

s32 EnHeishi2_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                               void* thisx) {
    EnHeishi2* self = THIS;

    switch (self->type) {
        case 1:
            break;
        case 7:
            break;
        default:
            if (limbIndex == 9) {
                rot->x = rot->x + self->unk_26C.y;
            }
            if (limbIndex == 16) {
                rot->x = rot->x + self->unk_260.y;
                rot->z = rot->z + self->unk_260.z;
            }
    }

    return false;
}

void EnHeishi2_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnHeishi2* self = THIS;

    if (limbIndex == 16) {
        Matrix_Get(&self->mtxf_330);
    }
}

void EnHeishi2_DrawKingGuard(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_heishi2.c", 1772);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_heishi2.c", 1774),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, &gHeishiKingGuardDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_heishi2.c", 1777);
}

void EnHeishi2_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi2* self = THIS;
    Mtx* mtx;
    s32 linkObjBankIndex;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_heishi2.c", 1792);

    func_80093D18(globalCtx->state.gfxCtx);

    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnHeishi2_OverrideLimbDraw,
                      EnHeishi2_PostLimbDraw, self);
    if ((self->type == 5) && (gSaveContext.infTable[7] & 0x80)) {
        linkObjBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_LINK_CHILD);
        if (linkObjBankIndex >= 0) {
            Matrix_Put(&self->mtxf_330);
            Matrix_Translate(-570.0f, 0.0f, 0.0f, MTXMODE_APPLY);
            Matrix_RotateZ(DEGTORAD(70.0), MTXMODE_APPLY);
            mtx = Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_heishi2.c", 1820) - 7;

            gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[linkObjBankIndex].segment);
            gSPSegment(POLY_OPA_DISP++, 0x0D, mtx);
            gSPDisplayList(POLY_OPA_DISP++, gLinkChildKeatonMaskDL);
            gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->actor.objBankIndex].segment);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_heishi2.c", 1834);
}
