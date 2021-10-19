/*
 * File: z_en_ds.c
 * Overlay: ovl_En_Ds
 * Description: Potion Shop Granny
 */

#include "z_en_ds.h"
#include "objects/object_ds/object_ds.h"

#define FLAGS 0x00000009

#define THIS ((EnDs*)thisx)

void EnDs_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDs_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDs_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDs_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDs_Wait(EnDs* self, GlobalContext* globalCtx);

const ActorInit En_Ds_InitVars = {
    ACTOR_EN_DS,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DS,
    sizeof(EnDs),
    (ActorFunc)EnDs_Init,
    (ActorFunc)EnDs_Destroy,
    (ActorFunc)EnDs_Update,
    (ActorFunc)EnDs_Draw,
};

void EnDs_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDs* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gPotionShopLadySkel, &gPotionShopLadyAnim, self->jointTable,
                       self->morphTable, 6);
    Animation_PlayOnce(&self->skelAnime, &gPotionShopLadyAnim);

    self->actor.colChkInfo.mass = MASS_IMMOVABLE;

    Actor_SetScale(&self->actor, 0.013f);

    self->actionFunc = EnDs_Wait;
    self->actor.targetMode = 1;
    self->unk_1E8 = 0;
    self->actor.flags &= ~0x1;
    self->unk_1E4 = 0.0f;
}

void EnDs_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnDs_Talk(EnDs* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx) != 0) {
        self->actionFunc = EnDs_Wait;
        self->actor.flags &= ~0x10000;
    }
    self->unk_1E8 |= 1;
}

void EnDs_TalkNoEmptyBottle(EnDs* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->actionFunc = EnDs_Wait;
    }
    self->unk_1E8 |= 1;
}

void EnDs_TalkAfterGiveOddPotion(EnDs* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        self->actionFunc = EnDs_Talk;
    } else {
        self->actor.flags |= 0x10000;
        func_8002F2CC(&self->actor, globalCtx, 1000.0f);
    }
}

void EnDs_DisplayOddPotionText(EnDs* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx) != 0) {
        self->actor.textId = 0x504F;
        self->actionFunc = EnDs_TalkAfterGiveOddPotion;
        self->actor.flags &= ~0x100;
        gSaveContext.itemGetInf[3] |= 1;
    }
}

void EnDs_GiveOddPotion(EnDs* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = EnDs_DisplayOddPotionText;
        gSaveContext.timer2State = 0;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_ODD_POTION, 10000.0f, 50.0f);
    }
}

void EnDs_TalkAfterBrewOddPotion(EnDs* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->actionFunc = EnDs_GiveOddPotion;
        func_8002F434(&self->actor, globalCtx, GI_ODD_POTION, 10000.0f, 50.0f);
    }
}

void EnDs_BrewOddPotion3(EnDs* self, GlobalContext* globalCtx) {
    if (self->brewTimer > 0) {
        self->brewTimer -= 1;
    } else {
        self->actionFunc = EnDs_TalkAfterBrewOddPotion;
        func_8010B720(globalCtx, 0x504D);
    }

    Math_StepToF(&self->unk_1E4, 0, 0.03f);
    Environment_AdjustLights(globalCtx, self->unk_1E4 * (2.0f - self->unk_1E4), 0.0f, 0.1f, 1.0f);
}

void EnDs_BrewOddPotion2(EnDs* self, GlobalContext* globalCtx) {
    if (self->brewTimer > 0) {
        self->brewTimer -= 1;
    } else {
        self->actionFunc = EnDs_BrewOddPotion3;
        self->brewTimer = 60;
        Flags_UnsetSwitch(globalCtx, 0x3F);
    }
}

void EnDs_BrewOddPotion1(EnDs* self, GlobalContext* globalCtx) {
    if (self->brewTimer > 0) {
        self->brewTimer -= 1;
    } else {
        self->actionFunc = EnDs_BrewOddPotion2;
        self->brewTimer = 20;
    }

    Math_StepToF(&self->unk_1E4, 1.0f, 0.01f);
    Environment_AdjustLights(globalCtx, self->unk_1E4 * (2.0f - self->unk_1E4), 0.0f, 0.1f, 1.0f);
}

void EnDs_OfferOddPotion(EnDs* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx) != 0)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // yes
                self->actionFunc = EnDs_BrewOddPotion1;
                self->brewTimer = 60;
                Flags_SetSwitch(globalCtx, 0x3F);
                globalCtx->msgCtx.msgMode = 0x37;
                player->exchangeItemId = EXCH_ITEM_NONE;
                break;
            case 1: // no
                func_8010B720(globalCtx, 0x504C);
                self->actionFunc = EnDs_Talk;
        }
    }
}

s32 EnDs_CheckRupeesAndBottle() {
    if (gSaveContext.rupees < 100) {
        return 0;
    } else if (Inventory_HasEmptyBottle() == 0) {
        return 1;
    } else {
        return 2;
    }
}

void EnDs_GiveBluePotion(EnDs* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = EnDs_Talk;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_POTION_BLUE, 10000.0f, 50.0f);
    }
}

void EnDs_OfferBluePotion(EnDs* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx) != 0)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // yes
                switch (EnDs_CheckRupeesAndBottle()) {
                    case 0: // have less than 100 rupees
                        func_8010B720(globalCtx, 0x500E);
                        break;
                    case 1: // have 100 rupees but no empty bottle
                        func_8010B720(globalCtx, 0x96);
                        self->actionFunc = EnDs_TalkNoEmptyBottle;
                        return;
                    case 2: // have 100 rupees and empty bottle
                        Rupees_ChangeBy(-100);
                        self->actor.flags &= ~0x10000;
                        func_8002F434(&self->actor, globalCtx, GI_POTION_BLUE, 10000.0f, 50.0f);
                        self->actionFunc = EnDs_GiveBluePotion;
                        return;
                }
                break;
            case 1: // no
                func_8010B720(globalCtx, 0x500D);
        }
        self->actionFunc = EnDs_Talk;
    }
}

void EnDs_Wait(EnDs* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 yawDiff;

    if (func_8002F194(&self->actor, globalCtx) != 0) {
        if (func_8002F368(globalCtx) == EXCH_ITEM_ODD_MUSHROOM) {
            Audio_PlaySoundGeneral(NA_SE_SY_TRE_BOX_APPEAR, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            player->actor.textId = 0x504A;
            self->actionFunc = EnDs_OfferOddPotion;
        } else if (gSaveContext.itemGetInf[3] & 1) {
            player->actor.textId = 0x500C;
            self->actionFunc = EnDs_OfferBluePotion;
        } else {
            if (INV_CONTENT(ITEM_ODD_MUSHROOM) == ITEM_ODD_MUSHROOM) {
                player->actor.textId = 0x5049;
            } else {
                player->actor.textId = 0x5048;
            }
            self->actionFunc = EnDs_Talk;
        }
    } else {
        yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        self->actor.textId = 0x5048;

        if ((ABS(yawDiff) < 0x2151) && (self->actor.xzDistToPlayer < 200.0f)) {
            func_8002F298(&self->actor, globalCtx, 100.0f, EXCH_ITEM_ODD_MUSHROOM);
            self->unk_1E8 |= 1;
        }
    }
}

void EnDs_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDs* self = THIS;

    if (SkelAnime_Update(&self->skelAnime) != 0) {
        self->skelAnime.curFrame = 0.0f;
    }

    self->actionFunc(self, globalCtx);

    if (self->unk_1E8 & 1) {
        func_80038290(globalCtx, &self->actor, &self->unk_1D8, &self->unk_1DE, self->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&self->unk_1D8.x, 0, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->unk_1D8.y, 0, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->unk_1DE.x, 0, 6, 0x1838, 100);
        Math_SmoothStepToS(&self->unk_1DE.y, 0, 6, 0x1838, 100);
    }
}

s32 EnDs_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnDs* self = THIS;

    if (limbIndex == 5) {
        rot->x += self->unk_1D8.y;
        rot->z += self->unk_1D8.x;
    }
    return false;
}

void EnDs_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f sMultVec = { 1100.0f, 500.0f, 0.0f };
    EnDs* self = THIS;

    if (limbIndex == 5) {
        Matrix_MultVec3f(&sMultVec, &self->actor.focus.pos);
    }
}

void EnDs_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDs* self = THIS;

    func_800943C8(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnDs_OverrideLimbDraw, EnDs_PostLimbDraw, self);
}
