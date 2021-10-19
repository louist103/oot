#include "z_en_bom_bowl_pit.h"
#include "vt.h"
#include "overlays/actors/ovl_En_Bom_Chu/z_en_bom_chu.h"
#include "overlays/actors/ovl_En_Ex_Item/z_en_ex_item.h"

#define FLAGS 0x00000010

#define THIS ((EnBomBowlPit*)thisx)

void EnBomBowlPit_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlPit_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlPit_Update(Actor* thisx, GlobalContext* globalCtx);

void EnBomBowlPit_SetupDetectHit(EnBomBowlPit* self, GlobalContext* globalCtx);
void EnBomBowlPit_DetectHit(EnBomBowlPit* self, GlobalContext* globalCtx);
void EnBomBowlPit_CameraDollyIn(EnBomBowlPit* self, GlobalContext* globalCtx);
void EnBomBowlPit_SpawnPrize(EnBomBowlPit* self, GlobalContext* globalCtx);
void EnBomBowlPit_SetupGivePrize(EnBomBowlPit* self, GlobalContext* globalCtx);
void EnBomBowlPit_GivePrize(EnBomBowlPit* self, GlobalContext* globalCtx);
void EnBomBowlPit_WaitTillPrizeGiven(EnBomBowlPit* self, GlobalContext* globalCtx);
void EnBomBowlPit_Reset(EnBomBowlPit* self, GlobalContext* globalCtx);

static s32 sGetItemIds[] = { GI_BOMB_BAG_30, GI_HEART_PIECE, GI_BOMBCHUS_10, GI_BOMBS_1, GI_RUPEE_PURPLE };

const ActorInit En_Bom_Bowl_Pit_InitVars = {
    ACTOR_EN_BOM_BOWL_PIT,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBomBowlPit),
    (ActorFunc)EnBomBowlPit_Init,
    (ActorFunc)EnBomBowlPit_Destroy,
    (ActorFunc)EnBomBowlPit_Update,
    NULL,
};

void EnBomBowlPit_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlPit* self = THIS;

    self->actionFunc = EnBomBowlPit_SetupDetectHit;
}

void EnBomBowlPit_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnBomBowlPit_SetupDetectHit(EnBomBowlPit* self, GlobalContext* globalCtx) {
    if (self->start != 0) {
        self->start = self->status = 0;
        self->actionFunc = EnBomBowlPit_DetectHit;
    }
}

void EnBomBowlPit_DetectHit(EnBomBowlPit* self, GlobalContext* globalCtx) {
    EnBomChu* chu;
    Vec3f chuPosDiff;

    if (globalCtx->cameraPtrs[MAIN_CAM]->setting == CAM_SET_FIXED1) {
        chu = (EnBomChu*)globalCtx->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].head;

        while (chu != NULL) {
            if ((&chu->actor == &self->actor) || (chu->actor.id != ACTOR_EN_BOM_CHU)) {
                chu = (EnBomChu*)chu->actor.next;
                continue;
            }

            chuPosDiff.x = chu->actor.world.pos.x - self->actor.world.pos.x;
            chuPosDiff.y = chu->actor.world.pos.y - self->actor.world.pos.y;
            chuPosDiff.z = chu->actor.world.pos.z - self->actor.world.pos.z;

            if (((fabsf(chuPosDiff.x) < 40.0f) || (BREG(2))) && ((fabsf(chuPosDiff.y) < 40.0f) || (BREG(2))) &&
                ((fabsf(chuPosDiff.z) < 40.0f) || (BREG(2)))) {
                func_8002DF54(globalCtx, NULL, 8);
                chu->timer = 1;

                self->camId = Gameplay_CreateSubCamera(globalCtx);
                Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
                Gameplay_ChangeCameraStatus(globalCtx, self->camId, CAM_STAT_ACTIVE);

                self->unk_1C8.x = self->unk_1C8.y = self->unk_1C8.z = 0.1f;
                self->unk_1A4.x = self->unk_1A4.y = self->unk_1A4.z = 0.1f;

                self->unk_180.x = self->unk_168.x = globalCtx->view.lookAt.x;
                self->unk_180.y = self->unk_168.y = globalCtx->view.lookAt.y;
                self->unk_180.z = self->unk_168.z = globalCtx->view.lookAt.z;

                self->unk_18C.x = self->unk_174.x = globalCtx->view.eye.x;
                self->unk_18C.y = self->unk_174.y = globalCtx->view.eye.y;
                self->unk_18C.z = self->unk_174.z = globalCtx->view.eye.z;

                self->unk_1BC.x = 20.0f;
                self->unk_1BC.y = 100.0f;
                self->unk_1BC.z = -800.0f;

                self->unk_198.x = 20.0f;
                self->unk_198.y = 50.0f;
                self->unk_198.z = -485.0f;

                self->unk_1B0.x = fabsf(self->unk_18C.x - self->unk_198.x) * 0.02f;
                self->unk_1B0.y = fabsf(self->unk_18C.y - self->unk_198.y) * 0.02f;
                self->unk_1B0.z = fabsf(self->unk_18C.z - self->unk_198.z) * 0.02f;

                self->unk_1D4.x = fabsf(self->unk_180.x - self->unk_1BC.x) * 0.02f;
                self->unk_1D4.y = fabsf(self->unk_180.y - self->unk_1BC.y) * 0.02f;
                self->unk_1D4.z = fabsf(self->unk_180.z - self->unk_1BC.z) * 0.02f;

                Gameplay_CameraSetAtEye(globalCtx, self->camId, &self->unk_180, &self->unk_18C);
                self->actor.textId = 0xF;
                func_8010B680(globalCtx, self->actor.textId, NULL);
                self->unk_154 = 5;
                func_80078884(NA_SE_EV_HIT_SOUND);
                func_8002DF54(globalCtx, NULL, 8);
                self->status = 1;
                self->actionFunc = EnBomBowlPit_CameraDollyIn;
                break;
            } else {
                chu = (EnBomChu*)chu->actor.next;
            }
        }
    }
}

void EnBomBowlPit_CameraDollyIn(EnBomBowlPit* self, GlobalContext* globalCtx) {
    if (self->camId != SUBCAM_FREE) {
        Math_ApproachF(&self->unk_180.x, self->unk_1BC.x, self->unk_1C8.x, self->unk_1D4.x);
        Math_ApproachF(&self->unk_180.y, self->unk_1BC.y, self->unk_1C8.y, self->unk_1D4.y);
        Math_ApproachF(&self->unk_180.z, self->unk_1BC.z, self->unk_1C8.z, self->unk_1D4.z);
        Math_ApproachF(&self->unk_18C.x, self->unk_198.x, self->unk_1A4.x, self->unk_1B0.x);
        Math_ApproachF(&self->unk_18C.y, self->unk_198.y, self->unk_1A4.y, self->unk_1B0.y);
        Math_ApproachF(&self->unk_18C.z, self->unk_198.z, self->unk_1A4.z, self->unk_1B0.z);
    }

    Gameplay_CameraSetAtEye(globalCtx, self->camId, &self->unk_180, &self->unk_18C);

    if ((self->unk_154 == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
    }

    if ((fabsf(self->unk_18C.x - self->unk_198.x) < 5.0f) && (fabsf(self->unk_18C.y - self->unk_198.y) < 5.0f) &&
        (fabsf(self->unk_18C.z - self->unk_198.z) < 5.0f) && (fabsf(self->unk_180.x - self->unk_1BC.x) < 5.0f) &&
        (fabsf(self->unk_180.y - self->unk_1BC.y) < 5.0f) && (fabsf(self->unk_180.z - self->unk_1BC.z) < 5.0f)) {
        func_80106CCC(globalCtx);
        self->timer = 30;
        self->actionFunc = EnBomBowlPit_SpawnPrize;
    }
}

void EnBomBowlPit_SpawnPrize(EnBomBowlPit* self, GlobalContext* globalCtx) {
    if (self->timer == 0) {
        self->exItem = (EnExItem*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_EX_ITEM,
                                                     self->actor.world.pos.x, self->actor.world.pos.y,
                                                     self->actor.world.pos.z - 70.0f, 0, 0, 0, self->prizeIndex);
        if (self->exItem != NULL) {
            self->actionFunc = EnBomBowlPit_SetupGivePrize;
        }
    }
}

void EnBomBowlPit_SetupGivePrize(EnBomBowlPit* self, GlobalContext* globalCtx) {
    if (self->exItemDone != 0) {
        switch (self->prizeIndex) {
            case EXITEM_BOMB_BAG_BOWLING:
                gSaveContext.itemGetInf[1] |= 2;
                break;
            case EXITEM_HEART_PIECE_BOWLING:
                gSaveContext.itemGetInf[1] |= 4;
                break;
        }

        Gameplay_ClearCamera(globalCtx, self->camId);
        Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
        func_8002DF54(globalCtx, NULL, 8);
        self->actionFunc = EnBomBowlPit_GivePrize;
    }
}

void EnBomBowlPit_GivePrize(EnBomBowlPit* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    func_8002DF54(globalCtx, NULL, 7);
    self->getItemId = sGetItemIds[self->prizeIndex];

    if ((self->getItemId == GI_BOMB_BAG_30) && (CUR_CAPACITY(UPG_BOMB_BAG) == 30)) {
        self->getItemId = GI_BOMB_BAG_40;
    }

    player->stateFlags1 &= ~0x20000000;
    self->actor.parent = NULL;
    func_8002F434(&self->actor, globalCtx, self->getItemId, 2000.0f, 1000.0f);
    player->stateFlags1 |= 0x20000000;
    self->actionFunc = EnBomBowlPit_WaitTillPrizeGiven;
}

void EnBomBowlPit_WaitTillPrizeGiven(EnBomBowlPit* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actionFunc = EnBomBowlPit_Reset;
    } else {
        func_8002F434(&self->actor, globalCtx, self->getItemId, 2000.0f, 1000.0f);
    }
}

void EnBomBowlPit_Reset(EnBomBowlPit* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
        // "Normal termination"/"completion"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 正常終了 ☆☆☆☆☆ \n" VT_RST);
        if (self->getItemId == GI_HEART_PIECE) {
            gSaveContext.healthAccumulator = 0x140;
            // "Ah recovery!" (?)
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ あぁ回復！ ☆☆☆☆☆ \n" VT_RST);
        }
        self->exItemDone = 0;
        self->status = 2;
        self->actionFunc = EnBomBowlPit_SetupDetectHit;
    }
}

void EnBomBowlPit_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlPit* self = THIS;

    self->actionFunc(self, globalCtx);

    if (self->timer != 0) {
        self->timer--;
    }
}
