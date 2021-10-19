/*
 * File: z_en_ex_item.c
 * Overlay: ovl_En_Ex_Item
 * Description: Minigame prize items
 */

#include "z_en_ex_item.h"
#include "overlays/actors/ovl_En_Bom_Bowl_Pit/z_en_bom_bowl_pit.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "vt.h"

#define FLAGS 0x00000030

#define THIS ((EnExItem*)thisx)

void EnExItem_Init(Actor* thisx, GlobalContext* globalCtx);
void EnExItem_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnExItem_Update(Actor* thisx, GlobalContext* globalCtx);
void EnExItem_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnExItem_DrawItems(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_DrawHeartPiece(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_DrawRupee(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_DrawKey(EnExItem* self, GlobalContext* globalCtx, s32 index);
void EnExItem_DrawMagic(EnExItem* self, GlobalContext* globalCtx, s16 magicIndex);

void EnExItem_WaitForObject(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_BowlPrize(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_SetupBowlCounter(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_BowlCounter(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_ExitChest(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_FairyMagic(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_TargetPrizeApproach(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_TargetPrizeGive(EnExItem* self, GlobalContext* globalCtx);
void EnExItem_TargetPrizeFinish(EnExItem* self, GlobalContext* globalCtx);

const ActorInit En_Ex_Item_InitVars = {
    ACTOR_EN_EX_ITEM,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnExItem),
    (ActorFunc)EnExItem_Init,
    (ActorFunc)EnExItem_Destroy,
    (ActorFunc)EnExItem_Update,
    (ActorFunc)EnExItem_Draw,
};

void EnExItem_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnExItem_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnExItem* self = THIS;

    self->actor.flags &= ~1;
    self->type = self->actor.params & 0xFF;
    self->unusedParam = (self->actor.params >> 8) & 0xFF;
    osSyncPrintf("\n\n");
    // "What will come out?"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ なにがでるかな？ ☆☆☆☆☆ %d\n" VT_RST, self->type);
    // "What will come out?"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ なにがでるかな？ ☆☆☆☆☆ %d\n" VT_RST, self->unusedParam);
    self->initPos = self->actor.world.pos;
    self->getItemObjId = -1;
    switch (self->type) {
        case EXITEM_BOMB_BAG_BOWLING:
        case EXITEM_BOMB_BAG_COUNTER:
            self->getItemObjId = OBJECT_GI_BOMBPOUCH;
            break;
        case EXITEM_HEART_PIECE_BOWLING:
        case EXITEM_HEART_PIECE_COUNTER:
            self->getItemObjId = OBJECT_GI_HEARTS;
            break;
        case EXITEM_BOMBCHUS_BOWLING:
        case EXITEM_BOMBCHUS_COUNTER:
            self->getItemObjId = OBJECT_GI_BOMB_2;
            break;
        case EXITEM_BOMBS_BOWLING:
        case EXITEM_BOMBS_COUNTER:
            self->getItemObjId = OBJECT_GI_BOMB_1;
            break;
        case EXITEM_PURPLE_RUPEE_BOWLING:
        case EXITEM_PURPLE_RUPEE_COUNTER:
        case EXITEM_GREEN_RUPEE_CHEST:
        case EXITEM_BLUE_RUPEE_CHEST:
        case EXITEM_RED_RUPEE_CHEST:
        case EXITEM_13:
        case EXITEM_14:
            self->getItemObjId = OBJECT_GI_RUPY;
            break;
        case EXITEM_SMALL_KEY_CHEST:
            self->scale = 0.05f;
            self->actor.velocity.y = 10.0f;
            self->timer = 7;
            self->actionFunc = EnExItem_ExitChest;
            break;
        case EXITEM_MAGIC_FIRE:
        case EXITEM_MAGIC_WIND:
        case EXITEM_MAGIC_DARK:
            self->getItemObjId = OBJECT_GI_GODDESS;
            break;
        case EXITEM_BULLET_BAG:
            self->getItemObjId = OBJECT_GI_DEKUPOUCH;
    }

    if (self->getItemObjId >= 0) {
        self->objectIdx = Object_GetIndex(&globalCtx->objectCtx, self->getItemObjId);
        self->actor.draw = NULL;
        if (self->objectIdx < 0) {
            Actor_Kill(&self->actor);
            // "What?"
            osSyncPrintf("なにみの？ %d\n", self->actor.params);
            // "bank is funny"
            osSyncPrintf(VT_FGCOL(PURPLE) " バンクおかしいしぞ！%d\n" VT_RST "\n", self->actor.params);
            return;
        }
        self->actionFunc = EnExItem_WaitForObject;
    }
}

void EnExItem_WaitForObject(EnExItem* self, GlobalContext* globalCtx) {
    s32 onCounter;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->objectIdx)) {
        // "End of transfer"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 転送終了 ☆☆☆☆☆ %d\n" VT_RST, self->actor.params, self);
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 転送終了 ☆☆☆☆☆ %d\n" VT_RST, self->actor.params, self);
        osSyncPrintf(VT_FGCOL(BLUE) "☆☆☆☆☆ 転送終了 ☆☆☆☆☆ %d\n" VT_RST, self->actor.params, self);
        osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 転送終了 ☆☆☆☆☆ %d\n" VT_RST, self->actor.params, self);
        osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 転送終了 ☆☆☆☆☆ %d\n\n" VT_RST, self->actor.params, self);
        self->actor.objBankIndex = self->objectIdx;
        self->actor.draw = EnExItem_Draw;
        self->stopRotate = false;
        onCounter = false;
        switch (self->type) {
            case EXITEM_BOMB_BAG_COUNTER:
                onCounter = true;
            case EXITEM_BOMB_BAG_BOWLING:
                self->unk_17C = func_8002EBCC;
                self->giDrawId = GID_BOMB_BAG_30;
                self->timer = 65;
                self->prizeRotateTimer = 35;
                self->scale = 0.5f;
                if (onCounter == 0) {
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actionFunc = EnExItem_SetupBowlCounter;
                    self->actor.shape.yOffset = -18.0f;
                }
                break;
            case EXITEM_HEART_PIECE_COUNTER:
                onCounter = true;
            case EXITEM_HEART_PIECE_BOWLING:
                self->unk_17C = func_8002ED80;
                self->timer = 65;
                self->prizeRotateTimer = 35;
                self->scale = 0.5f;
                if (!onCounter) {
                    func_80078884(NA_SE_SY_PIECE_OF_HEART);
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actionFunc = EnExItem_SetupBowlCounter;
                    self->actor.shape.yOffset = -10.0f;
                }
                break;
            case EXITEM_BOMBCHUS_COUNTER:
                onCounter = true;
            case EXITEM_BOMBCHUS_BOWLING:
                self->unk_17C = func_8002EBCC;
                self->giDrawId = GID_BOMBCHU;
                self->timer = 65;
                self->prizeRotateTimer = 35;
                self->scale = 0.5f;
                if (!onCounter) {
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actionFunc = EnExItem_SetupBowlCounter;
                }
                break;
            case EXITEM_BOMBS_BOWLING:
            case EXITEM_BOMBS_COUNTER:
                self->unk_17C = func_8002EBCC;
                self->giDrawId = GID_BOMB;
                self->timer = 65;
                self->prizeRotateTimer = 35;
                self->scale = 0.5f;
                self->unkFloat = 0.5f;
                if (self->type == EXITEM_BOMBS_BOWLING) {
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actionFunc = EnExItem_SetupBowlCounter;
                    self->actor.shape.yOffset = 10.0f;
                }
                break;
            case EXITEM_PURPLE_RUPEE_BOWLING:
            case EXITEM_PURPLE_RUPEE_COUNTER:
                self->unk_17C = func_8002EBCC;
                self->unk_180 = func_8002ED80;
                self->giDrawId = GID_RUPEE_PURPLE;
                self->timer = 65;
                self->prizeRotateTimer = 35;
                self->scale = 0.5f;
                self->unkFloat = 0.5f;
                if (self->type == EXITEM_PURPLE_RUPEE_BOWLING) {
                    self->actionFunc = EnExItem_BowlPrize;
                } else {
                    self->actionFunc = EnExItem_SetupBowlCounter;
                    self->actor.shape.yOffset = 40.0f;
                }
                break;
            case EXITEM_GREEN_RUPEE_CHEST:
            case EXITEM_BLUE_RUPEE_CHEST:
            case EXITEM_RED_RUPEE_CHEST:
            case EXITEM_13:
            case EXITEM_14:
                self->unk_17C = func_8002EBCC;
                self->unk_180 = func_8002ED80;
                self->timer = 7;
                self->scale = 0.5f;
                self->unkFloat = 0.5f;
                self->actor.velocity.y = 10.0f;
                switch (self->type) {
                    case EXITEM_GREEN_RUPEE_CHEST:
                        self->giDrawId = GID_RUPEE_GREEN;
                        break;
                    case EXITEM_BLUE_RUPEE_CHEST:
                        self->giDrawId = GID_RUPEE_BLUE;
                        break;
                    case EXITEM_RED_RUPEE_CHEST:
                        self->giDrawId = GID_RUPEE_RED;
                        break;
                    case EXITEM_14:
                        self->giDrawId = GID_RUPEE_PURPLE;
                        break;
                }
                self->actionFunc = EnExItem_ExitChest;
                break;
            case EXITEM_MAGIC_FIRE:
            case EXITEM_MAGIC_WIND:
            case EXITEM_MAGIC_DARK:
                self->scale = 0.35f;
                self->actionFunc = EnExItem_FairyMagic;
                break;
            case EXITEM_BULLET_BAG:
                self->unk_17C = func_8002EBCC;
                self->giDrawId = GID_BULLET_BAG;
                self->scale = 0.1f;
                self->timer = 80;
                self->prizeRotateTimer = 35;
                self->actionFunc = EnExItem_TargetPrizeApproach;
                break;
            case EXITEM_SMALL_KEY_CHEST:
                break;
        }
    }
}

void EnExItem_BowlPrize(EnExItem* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 tmpf1;
    f32 tmpf2;
    f32 tmpf3;
    f32 tmpf4;
    f32 tmpf5;
    f32 tmpf6;
    f32 tmpf7;
    f32 sp3C;

    if (!self->stopRotate) {
        self->actor.shape.rot.y += 0x1000;
        if ((self->prizeRotateTimer == 0) && ((self->actor.shape.rot.y & 0xFFFF) == 0x9000)) {
            self->stopRotate++;
        }
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, 0, 5, 0x1000, 0);
    }
    if (self->timer != 0) {
        if (self->prizeRotateTimer != 0) {
            sp3C = 250.0f;
            if (self->type == EXITEM_BOMBCHUS_BOWLING) {
                sp3C = 220.0f;
            }
            tmpf1 = globalCtx->view.lookAt.x - globalCtx->view.eye.x;
            tmpf2 = globalCtx->view.lookAt.y - globalCtx->view.eye.y;
            tmpf3 = globalCtx->view.lookAt.z + sp3C - globalCtx->view.eye.z;
            tmpf4 = sqrtf(SQ(tmpf1) + SQ(tmpf2) + SQ(tmpf3));

            tmpf5 = (tmpf1 / tmpf4) * 5.0f;
            tmpf6 = (tmpf2 / tmpf4) * 5.0f;
            tmpf7 = (tmpf3 / tmpf4) * 5.0f;

            tmpf1 = globalCtx->view.eye.x + tmpf5 - self->actor.world.pos.x;
            tmpf2 = globalCtx->view.eye.y + tmpf6 - self->actor.world.pos.y;
            tmpf3 = globalCtx->view.eye.z + tmpf7 - self->actor.world.pos.z;

            self->actor.world.pos.x += (tmpf1 / tmpf4) * 5.0f;
            self->actor.world.pos.y += (tmpf2 / tmpf4) * 5.0f;
            self->actor.world.pos.z += (tmpf3 / tmpf4) * 5.0f;
        }
    } else {
        // "parent"
        osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 母親ー？     ☆☆☆☆☆ %x\n" VT_RST, self->actor.parent);
        // "Can it move?"
        osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 動いてねー？ ☆☆☆☆☆ %x\n" VT_RST, self->actor.parent->update);
        if ((self->actor.parent != NULL) && (self->actor.parent->update != NULL)) {
            ((EnBomBowlPit*)self->actor.parent)->exItemDone = 1;
            // "It can't move!"
            osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ さぁきえるぞ！ ☆☆☆☆☆ \n" VT_RST);
        }
        Actor_Kill(&self->actor);
    }
}

void EnExItem_SetupBowlCounter(EnExItem* self, GlobalContext* globalCtx) {
    self->actor.world.rot.y = self->actor.shape.rot.y = 0x4268;
    self->actionFunc = EnExItem_BowlCounter;
}

void EnExItem_BowlCounter(EnExItem* self, GlobalContext* globalCtx) {
    self->actor.shape.rot.y += 0x800;
    if (self->killItem) {
        Actor_Kill(&self->actor);
    }
}

void EnExItem_ExitChest(EnExItem* self, GlobalContext* globalCtx) {
    self->actor.shape.rot.y += 0x1000;
    if (self->timer != 0) {
        if (self->timer == 1) {
            self->chestKillTimer = 20;
        }
    } else {
        self->actor.velocity.y = 0.0f;
        if (self->chestKillTimer == 0) {
            Actor_Kill(&self->actor);
        }
    }
    Actor_MoveForward(&self->actor);
}

void EnExItem_FairyMagic(EnExItem* self, GlobalContext* globalCtx) {
    self->actor.shape.rot.y += 0x800;
}

void EnExItem_TargetPrizeApproach(EnExItem* self, GlobalContext* globalCtx) {
    f32 tmpf1;
    f32 tmpf2;
    f32 tmpf3;
    f32 tmpf4;
    f32 tmpf5;
    f32 tmpf6;
    f32 tmpf7;

    Math_ApproachF(&self->scale, 0.8f, 0.1f, 0.02f);
    if (!self->stopRotate) {
        self->actor.shape.rot.y += 0x1000;
        if ((self->prizeRotateTimer == 0) && ((self->actor.shape.rot.y & 0xFFFF) == 0x9000)) {
            self->stopRotate++;
        }
    } else {
        Math_SmoothStepToS(&self->actor.shape.rot.y, -0x4000, 5, 0x1000, 0);
    }

    if (self->timer != 0) {
        if (self->prizeRotateTimer != 0) {
            tmpf1 = globalCtx->view.lookAt.x - globalCtx->view.eye.x;
            tmpf2 = globalCtx->view.lookAt.y - 10.0f - globalCtx->view.eye.y;
            tmpf3 = globalCtx->view.lookAt.z + 10.0f - globalCtx->view.eye.z;
            tmpf4 = sqrtf(SQ(tmpf1) + SQ(tmpf2) + SQ(tmpf3));

            tmpf5 = (tmpf1 / tmpf4) * 5.0f;
            tmpf6 = (tmpf2 / tmpf4) * 5.0f;
            tmpf7 = (tmpf3 / tmpf4) * 5.0f;

            tmpf1 = globalCtx->view.eye.x + tmpf5 - self->actor.world.pos.x;
            tmpf2 = globalCtx->view.eye.y - 10.0f + tmpf6 - self->actor.world.pos.y;
            tmpf3 = globalCtx->view.eye.z + 10.0f + tmpf7 - self->actor.world.pos.z;

            self->actor.world.pos.x += (tmpf1 / tmpf4) * 5.0f;
            self->actor.world.pos.y += (tmpf2 / tmpf4) * 5.0f;
            self->actor.world.pos.z += (tmpf3 / tmpf4) * 5.0f;
        }
    } else {
        s32 getItemId;

        self->actor.draw = NULL;
        func_8002DF54(globalCtx, NULL, 7);
        self->actor.parent = NULL;
        if (CUR_UPG_VALUE(UPG_BULLET_BAG) == 1) {
            getItemId = GI_BULLET_BAG_40;
        } else {
            getItemId = GI_BULLET_BAG_50;
        }
        func_8002F434(&self->actor, globalCtx, getItemId, 2000.0f, 1000.0f);
        self->actionFunc = EnExItem_TargetPrizeGive;
    }
}

void EnExItem_TargetPrizeGive(EnExItem* self, GlobalContext* globalCtx) {
    s32 getItemId;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actionFunc = EnExItem_TargetPrizeFinish;
    } else {
        getItemId = (CUR_UPG_VALUE(UPG_BULLET_BAG) == 2) ? GI_BULLET_BAG_50 : GI_BULLET_BAG_40;

        func_8002F434(&self->actor, globalCtx, getItemId, 2000.0f, 1000.0f);
    }
}

void EnExItem_TargetPrizeFinish(EnExItem* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && func_80106BC8(globalCtx)) {
        // "Successful completion"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 正常終了 ☆☆☆☆☆ \n" VT_RST);
        gSaveContext.itemGetInf[1] |= 0x2000;
        Actor_Kill(&self->actor);
    }
}

void EnExItem_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnExItem* self = THIS;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->chestKillTimer != 0) {
        self->chestKillTimer--;
    }
    if (self->prizeRotateTimer != 0) {
        self->prizeRotateTimer--;
    }
    self->actionFunc(self, globalCtx);
}

void EnExItem_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnExItem* self = THIS;
    s32 magicType;

    Actor_SetScale(&self->actor, self->scale);
    switch (self->type) {
        case EXITEM_BOMB_BAG_BOWLING:
        case EXITEM_BOMBCHUS_BOWLING:
        case EXITEM_BOMBS_BOWLING:
        case EXITEM_BOMB_BAG_COUNTER:
        case EXITEM_BOMBCHUS_COUNTER:
        case EXITEM_BOMBS_COUNTER:
        case EXITEM_BULLET_BAG:
            EnExItem_DrawItems(self, globalCtx);
            break;
        case EXITEM_HEART_PIECE_BOWLING:
        case EXITEM_HEART_PIECE_COUNTER:
            EnExItem_DrawHeartPiece(self, globalCtx);
            break;
        case EXITEM_PURPLE_RUPEE_BOWLING:
        case EXITEM_PURPLE_RUPEE_COUNTER:
        case EXITEM_GREEN_RUPEE_CHEST:
        case EXITEM_BLUE_RUPEE_CHEST:
        case EXITEM_RED_RUPEE_CHEST:
        case EXITEM_13:
        case EXITEM_14:
            EnExItem_DrawRupee(self, globalCtx);
            break;
        case EXITEM_SMALL_KEY_CHEST:
            EnExItem_DrawKey(self, globalCtx, 0);
            break;
        case EXITEM_MAGIC_FIRE:
        case EXITEM_MAGIC_WIND:
        case EXITEM_MAGIC_DARK:
            magicType = self->type - EXITEM_MAGIC_FIRE;
            EnExItem_DrawMagic(self, globalCtx, magicType);
            break;
    }
}

void EnExItem_DrawItems(EnExItem* self, GlobalContext* globalCtx) {
    if (self->unk_17C != NULL) {
        self->unk_17C(&self->actor, globalCtx, 0);
    }
    if (self) {}
    func_8002ED80(&self->actor, globalCtx, 0);
    GetItem_Draw(globalCtx, self->giDrawId);
}

void EnExItem_DrawHeartPiece(EnExItem* self, GlobalContext* globalCtx) {
    func_8002ED80(&self->actor, globalCtx, 0);
    GetItem_Draw(globalCtx, GID_HEART_PIECE);
}

void EnExItem_DrawMagic(EnExItem* self, GlobalContext* globalCtx, s16 magicIndex) {
    static s16 sgiDrawIds[] = { GID_DINS_FIRE, GID_FARORES_WIND, GID_NAYRUS_LOVE };

    func_8002ED80(&self->actor, globalCtx, 0);
    GetItem_Draw(globalCtx, sgiDrawIds[magicIndex]);
}

void EnExItem_DrawKey(EnExItem* self, GlobalContext* globalCtx, s32 index) {
    static s32 keySegments[] = { 0x0403F140 };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ex_item.c", 880);

    func_8009460C(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ex_item.c", 887),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(keySegments[index]));
    gSPDisplayList(POLY_OPA_DISP++, gItemDropDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ex_item.c", 893);
}

void EnExItem_DrawRupee(EnExItem* self, GlobalContext* globalCtx) {
    if (self->unk_17C != NULL) {
        self->unk_17C(&self->actor, globalCtx, 0);
    }
    if (self->unk_180 != NULL) {
        self->unk_180(&self->actor, globalCtx, 0);
    }
    GetItem_Draw(globalCtx, self->giDrawId);
}
