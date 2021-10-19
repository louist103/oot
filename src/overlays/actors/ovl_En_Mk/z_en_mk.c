/*
 * File: z_en_mk.c
 * Overlay: ovl_En_Mk
 * Description: Lakeside Professor
 */

#include "z_en_mk.h"
#include "objects/object_mk/object_mk.h"

#define FLAGS 0x00000019

#define THIS ((EnMk*)thisx)

void EnMk_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMk_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMk_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMk_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnMk_Wait(EnMk* self, GlobalContext* globalCtx);

const ActorInit En_Mk_InitVars = {
    ACTOR_EN_MK,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_MK,
    sizeof(EnMk),
    (ActorFunc)EnMk_Init,
    (ActorFunc)EnMk_Destroy,
    (ActorFunc)EnMk_Update,
    (ActorFunc)EnMk_Draw,
};

static ColliderCylinderInit sCylinderInit = {
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

void EnMk_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnMk* self = THIS;
    s32 swimFlag;

    self->actor.minVelocityY = -4.0f;
    self->actor.gravity = -1.0f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_mk_Skel_005DF0, &object_mk_Anim_000D88, self->jointTable,
                       self->morphTable, 13);
    Animation_PlayLoop(&self->skelAnime, &object_mk_Anim_000D88);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->actor.colChkInfo.mass = 0xFF;
    Actor_SetScale(&self->actor, 0.01f);

    self->actionFunc = EnMk_Wait;
    self->flags = 0;
    self->swimFlag = 0;
    self->actor.targetMode = 6;

    if (gSaveContext.itemGetInf[1] & 1) {
        self->flags |= 4;
    }
}

void EnMk_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnMk* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80AACA40(EnMk* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actor.flags &= 0xFFFEFFFF;
        self->actionFunc = EnMk_Wait;
    }

    self->flags |= 1;
}

void func_80AACA94(EnMk* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx) != 0) {
        self->actor.parent = NULL;
        self->actionFunc = func_80AACA40;
        func_80088AA0(240);
        gSaveContext.eventInf[1] &= ~1;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_EYEDROPS, 10000.0f, 50.0f);
    }
}

void func_80AACB14(EnMk* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        self->actionFunc = func_80AACA94;
        func_8002F434(&self->actor, globalCtx, GI_EYEDROPS, 10000.0f, 50.0f);
    }
}

void func_80AACB6C(EnMk* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = func_80AACB14;
    }

    self->flags |= 1;
}

void func_80AACBAC(EnMk* self, GlobalContext* globalCtx) {
    if (self->timer > 0) {
        self->timer--;
        self->actor.shape.rot.y -= 0x800;
    } else {
        self->actionFunc = func_80AACB6C;
        func_8010B720(globalCtx, 0x4030);
    }
}

void func_80AACC04(EnMk* self, GlobalContext* globalCtx) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        self->timer = 16;
        self->actionFunc = func_80AACBAC;
        Animation_Change(&self->skelAnime, &object_mk_Anim_000D88, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_mk_Anim_000D88), ANIMMODE_LOOP, -4.0f);
        self->flags &= ~2;
    }
}

void func_80AACCA0(EnMk* self, GlobalContext* globalCtx) {
    if (self->timer > 0) {
        self->timer--;
        self->actor.shape.rot.y += 0x800;
    } else {
        self->timer = 120;
        self->actionFunc = func_80AACC04;
        Animation_Change(&self->skelAnime, &object_mk_Anim_000724, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_mk_Anim_000724), ANIMMODE_LOOP, -4.0f);
        self->flags &= ~2;
    }
}

void func_80AACD48(EnMk* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->actionFunc = func_80AACCA0;
        globalCtx->msgCtx.msgMode = 0x37;
        player->exchangeItemId = EXCH_ITEM_NONE;
        self->timer = 16;
        Animation_Change(&self->skelAnime, &object_mk_Anim_000D88, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_mk_Anim_000D88), ANIMMODE_LOOP, -4.0f);
        self->flags &= ~2;
    }

    self->flags |= 1;
}

void func_80AACE2C(EnMk* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_8010B720(globalCtx, 0x4001);
        Animation_Change(&self->skelAnime, &object_mk_Anim_000AC0, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_mk_Anim_000AC0), ANIMMODE_ONCE, -4.0f);
        self->flags &= ~2;
        self->actionFunc = func_80AACD48;
    }

    self->flags |= 1;
}

void func_80AACEE8(EnMk* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_8010B720(globalCtx, 0x4000);
        Animation_Change(&self->skelAnime, &object_mk_Anim_000AC0, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_mk_Anim_000AC0), ANIMMODE_LOOP, -4.0f);
        self->flags &= ~2;
        self->actionFunc = func_80AACE2C;
    }

    self->flags |= 1;
}

void func_80AACFA0(EnMk* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = func_80AACA40;
        gSaveContext.itemGetInf[1] |= 1;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_HEART_PIECE, 10000.0f, 50.0f);
    }
}

void func_80AAD014(EnMk* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx) != 0) {
        self->actionFunc = func_80AACFA0;
        func_8002F434(&self->actor, globalCtx, GI_HEART_PIECE, 10000.0f, 50.0f);
    }

    self->flags |= 1;
}

void EnMk_Wait(EnMk* self, GlobalContext* globalCtx) {
    s16 angle;
    s32 swimFlag;
    Player* player = GET_PLAYER(globalCtx);
    s32 playerExchangeItem;

    if (func_8002F194(&self->actor, globalCtx) != 0) {
        playerExchangeItem = func_8002F368(globalCtx);

        if (self->actor.textId != 0x4018) {
            player->actor.textId = self->actor.textId;
            self->actionFunc = func_80AACA40;
        } else {
            if (INV_CONTENT(ITEM_ODD_MUSHROOM) == ITEM_EYEDROPS) {
                player->actor.textId = 0x4032;
                self->actionFunc = func_80AACA40;
            } else {
                switch (playerExchangeItem) {
                    case EXCH_ITEM_NONE:
                        if (self->swimFlag >= 8) {
                            if (gSaveContext.itemGetInf[1] & 1) {
                                player->actor.textId = 0x4075;
                                self->actionFunc = func_80AACA40;
                            } else {
                                player->actor.textId = 0x4074;
                                self->actionFunc = func_80AAD014;
                                self->swimFlag = 0;
                            }
                        } else {
                            if (self->swimFlag == 0) {
                                player->actor.textId = 0x4018;
                                self->actionFunc = func_80AACA40;
                            } else {
                                player->actor.textId = 0x406C + self->swimFlag;
                                self->actionFunc = func_80AACA40;
                            }
                        }
                        break;
                    case EXCH_ITEM_FROG:
                        player->actor.textId = 0x4019;
                        self->actionFunc = func_80AACEE8;
                        Animation_Change(&self->skelAnime, &object_mk_Anim_000368, 1.0f, 0.0f,
                                         Animation_GetLastFrame(&object_mk_Anim_000368), ANIMMODE_ONCE, -4.0f);
                        self->flags &= ~2;
                        gSaveContext.timer2State = 0;
                        func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
                        break;
                    default:
                        player->actor.textId = 0x4018;
                        self->actionFunc = func_80AACA40;
                        break;
                }
            }
        }
    } else {
        self->actor.textId = Text_GetFaceReaction(globalCtx, 0x1A);

        if (self->actor.textId == 0) {
            self->actor.textId = 0x4018;
        }

        angle = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

        if ((ABS(angle) < 0x2151) && (self->actor.xzDistToPlayer < 100.0f)) {
            func_8002F298(&self->actor, globalCtx, 100.0f, EXCH_ITEM_FROG);
            self->flags |= 1;
        }
    }
}

void EnMk_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnMk* self = THIS;
    s32 pad;
    Vec3s vec;
    Player* player;
    s16 swimFlag;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    if ((!(self->flags & 2)) && (SkelAnime_Update(&self->skelAnime))) {
        self->flags |= 2;
    }

    self->actionFunc(self, globalCtx);

    if (self->flags & 1) {
        func_80038290(globalCtx, &self->actor, &self->headRotation, &vec, self->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&self->headRotation.x, 0, 6, 6200, 100);
        Math_SmoothStepToS(&self->headRotation.y, 0, 6, 6200, 100);
    }

    player = GET_PLAYER(globalCtx);

    if (self->flags & 8) {
        if (!(player->stateFlags2 & 0x400)) {
            self->flags &= ~8;
        }
    } else {
        if (player->currentBoots == PLAYER_BOOTS_IRON) {
            self->flags |= 8;
        } else if (player->stateFlags2 & 0x400) {
            swimFlag = player->actor.yDistToWater;

            if (swimFlag > 0) {
                if (swimFlag >= 320) {
                    if (swimFlag >= 355) {
                        swimFlag = 8;
                    } else {
                        swimFlag = 7;
                    }
                } else if (swimFlag < 80) {
                    swimFlag = 1;
                } else {
                    swimFlag *= 0.025f;
                }

                if (self->swimFlag < swimFlag) {
                    self->swimFlag = swimFlag;

                    if ((!(self->flags & 4)) && (self->swimFlag >= 8)) {
                        self->flags |= 4;
                        func_80078884(NA_SE_SY_CORRECT_CHIME);
                    }
                }
            }
        }
    }
}

s32 EnMk_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnMk* self = THIS;

    if (limbIndex == 11) {
        rot->y -= self->headRotation.y;
        rot->z += self->headRotation.x;
    }

    return false;
}

void EnMk_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f D_80AAD64C = { 1000.0f, -100.0f, 0.0f };
    EnMk* self = THIS;

    if (limbIndex == 11) {
        Matrix_MultVec3f(&D_80AAD64C, &self->actor.focus.pos);
    }
}

void EnMk_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnMk* self = THIS;

    func_800943C8(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnMk_OverrideLimbDraw, EnMk_PostLimbDraw, &self->actor);
}
