#include "z_en_heishi4.h"
#include "objects/object_sd/object_sd.h"
#include "vt.h"

#define FLAGS 0x00000009

#define THIS ((EnHeishi4*)thisx)

void EnHeishi4_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi4_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi4_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHeishi4_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A56544(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A5673C(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A56328(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A563BC(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A56B40(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A56614(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A56874(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A56900(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A56994(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A56A50(EnHeishi4* self, GlobalContext* globalCtx);
void func_80A56ACC(EnHeishi4* self, GlobalContext* globalCtx);

const ActorInit En_Heishi4_InitVars = {
    ACTOR_EN_HEISHI4,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_SD,
    sizeof(EnHeishi4),
    (ActorFunc)EnHeishi4_Init,
    (ActorFunc)EnHeishi4_Destroy,
    (ActorFunc)EnHeishi4_Update,
    (ActorFunc)EnHeishi4_Draw,
};

static u32 sFaceReactionSets[] = { 6, 7 };

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

void EnHeishi4_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi4* self = THIS;

    Actor_SetScale(thisx, 0.01f);
    self->type = thisx->params & 0xFF;
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    self->pos = thisx->world.pos;
    thisx->targetMode = 6;
    if (self->type == HEISHI4_AT_MARKET_DYING) {
        self->height = 30.0f;
        ActorShape_Init(&thisx->shape, 0.0f, NULL, 30.0f);
        SkelAnime_Init(globalCtx, &self->skelAnime, &gEnHeishiSkel, &gEnHeishiDyingGuardAnim_00C444, self->jointTable,
                       self->morphTable, 17);
    } else {
        self->height = 60.0f;
        ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
        SkelAnime_Init(globalCtx, &self->skelAnime, &gEnHeishiSkel, &gEnHeishiIdleAnim, self->jointTable,
                       self->morphTable, 17);
    }
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, thisx, &sCylinderInit);
    self->collider.dim.yShift = 0;
    self->collider.dim.radius = 15;
    self->collider.dim.height = 70;
    switch (self->type) {
        case HEISHI4_AT_KAKRIKO_ENTRANCE:
        case HEISHI4_AT_IMPAS_HOUSE:
            self->actionFunc = func_80A56328;
            break;
        case HEISHI4_AT_MARKET_DYING:
            self->collider.dim.radius = 28;
            self->collider.dim.height = 5;
            self->actionFunc = func_80A5673C;
            break;
        case HEISHI4_AT_MARKET_NIGHT:
            self->actionFunc = func_80A56544;
            break;
    }
    self->unk_27C = ((thisx->params >> 8) & 0xFF);
    osSyncPrintf("\n\n");
    osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 兵士２セット完了！ ☆☆☆☆☆ %d\n" VT_RST, thisx->params);
    osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ 識別完了！\t    ☆☆☆☆☆ %d\n" VT_RST, self->type);
    osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆☆☆☆ メッセージ完了！   ☆☆☆☆☆ %x\n\n" VT_RST, (thisx->params >> 8) & 0xF);
    osSyncPrintf("\n\n");
}

void EnHeishi4_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi4* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80A56328(EnHeishi4* self, GlobalContext* globalCtx) {
    f32 frames = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = func_80A563BC;
}

void func_80A563BC(EnHeishi4* self, GlobalContext* globalCtx) {
    s16 reactionOffset;

    self->unk_2B4 = 0;
    reactionOffset = self->type - 4;
    if (reactionOffset < 0) {
        reactionOffset = 0;
    }
    if (reactionOffset >= 3) {
        reactionOffset = 1;
    }
    if (Text_GetFaceReaction(globalCtx, sFaceReactionSets[reactionOffset]) != 0) {
        self->actor.textId = Text_GetFaceReaction(globalCtx, sFaceReactionSets[reactionOffset]);
        self->unk_2B4 = 1;
        self->actionFunc = func_80A56B40;
    } else {
        if (gSaveContext.eventChkInf[8] & 1) {
            self->actor.textId = 0x5065;
            self->actionFunc = func_80A56B40;
            return;
        }
        if (gSaveContext.eventChkInf[4] & 0x20) {
            self->actor.textId = 0x5068;
            self->actionFunc = func_80A56B40;
            return;
        }
        if (self->type == HEISHI4_AT_IMPAS_HOUSE) {
            if (self->unk_284 == 0) {
                self->actor.textId = 0x5079;
            } else {
                self->actor.textId = 0x507A;
            }
        } else if (IS_DAY) {
            if (self->unk_284 == 0) {
                self->actor.textId = 0x5063;
            } else {
                self->actor.textId = 0x5064;
            }
        } else if (self->unk_284 == 0) {
            self->actor.textId = 0x5066;
        } else {
            self->actor.textId = 0x5067;
        }

        self->actionFunc = func_80A56B40;
    }
}

void func_80A56544(EnHeishi4* self, GlobalContext* globalCtx) {
    f32 frames = Animation_GetLastFrame(&gEnHeishiIdleAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiIdleAnim, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, -10.0f);
    if (LINK_AGE_IN_YEARS != YEARS_CHILD) {
        osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ ぎゃぁ！オトナだー ☆☆☆☆☆ \n" VT_RST);
        Actor_Kill(&self->actor);
    } else {
        self->actionFunc = func_80A56614;
    }
}

void func_80A56614(EnHeishi4* self, GlobalContext* globalCtx) {
    s16 reactionOffset;

    reactionOffset = self->type - 4;
    self->unk_2B4 = 0;
    if (reactionOffset < 0) {
        reactionOffset = 0;
    }
    if (reactionOffset >= 3) {
        reactionOffset = 1;
    }
    if (Text_GetFaceReaction(globalCtx, sFaceReactionSets[reactionOffset]) != 0) {
        self->actor.textId = Text_GetFaceReaction(globalCtx, sFaceReactionSets[reactionOffset]);
        self->unk_2B4 = 1;
        self->actionFunc = func_80A56B40;
        return;
    }
    if (globalCtx->sceneNum == SCENE_MIHARIGOYA) {
        if (IS_DAY) {
            self->actor.textId = 0x7004;
        } else {
            self->actor.textId = 0x709A;
        }
    } else if (globalCtx->sceneNum != SCENE_MARKET_NIGHT) {
        if (IS_DAY) {
            self->actor.textId = 0x7002;
        } else {
            self->actor.textId = 0x7003;
        }
    } else {
        self->actor.textId = 0x7003;
    }

    self->actionFunc = func_80A56B40;
}

void func_80A5673C(EnHeishi4* self, GlobalContext* globalCtx) {
    if (gSaveContext.eventChkInf[4] & 0x20) {
        osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ マスターソード祝入手！ ☆☆☆☆☆ \n" VT_RST);
        Actor_Kill(&self->actor);
        return;
    }
    self->unk_284 = 0;
    if (gSaveContext.eventChkInf[8] & 1) {
        if (!(gSaveContext.infTable[6] & 0x1000)) {
            f32 frames = Animation_GetLastFrame(&gEnHeishiDyingGuardAnim_00C444);
            Animation_Change(&self->skelAnime, &gEnHeishiDyingGuardAnim_00C444, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP,
                             -10.0f);
            self->actor.textId = 0x7007;
            self->unk_282 = 5;
            self->unk_284 = 1;
            osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ デモ開始！ ☆☆☆☆☆ \n" VT_RST);
        } else {
            self->actor.textId = 0x7008;
            self->unk_282 = 6;
            osSyncPrintf(VT_FGCOL(BLUE) " ☆☆☆☆☆ 返事なし ☆☆☆☆☆ \n" VT_RST);
        }
        self->actionFunc = func_80A56874;
    } else {
        Actor_Kill(&self->actor);
    }
}

void func_80A56874(EnHeishi4* self, GlobalContext* globalCtx) {
    if (self->unk_284 != 0) {
        SkelAnime_Update(&self->skelAnime);
    }
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        if (self->unk_284 == 0) {
            self->actionFunc = func_80A5673C;

        } else {
            self->actionFunc = func_80A56900;
        }
    } else {
        func_8002F2CC(&self->actor, globalCtx, 100.0f);
    }
}

void func_80A56900(EnHeishi4* self, GlobalContext* globalCtx) {
    f32 frames = Animation_GetLastFrame(&gEnHeishiDyingGuardTalkAnim);

    Animation_Change(&self->skelAnime, &gEnHeishiDyingGuardTalkAnim, 1.0f, 0.0f, (s16)frames, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = func_80A56994;
}

void func_80A56994(EnHeishi4* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    func_80038290(globalCtx, &self->actor, &self->unk_260, &self->unk_266, self->actor.focus.pos);
    if (self->unk_282 == func_8010BDBC(&globalCtx->msgCtx)) {
        if (func_80106BC8(globalCtx) != 0) {
            func_80106CCC(globalCtx);
            gSaveContext.infTable[6] |= 0x1000;
            func_8002DF54(globalCtx, NULL, 8);
            self->actionFunc = func_80A56A50;
        }
    }
}

void func_80A56A50(EnHeishi4* self, GlobalContext* globalCtx) {
    f32 frames = Animation_GetLastFrame(&gEnHeishiDyingGuardDieAnim);
    self->unk_288 = frames;
    Animation_Change(&self->skelAnime, &gEnHeishiDyingGuardDieAnim, 1.0f, 0.0f, frames, ANIMMODE_ONCE, -10.0f);
    self->actionFunc = func_80A56ACC;
}

void func_80A56ACC(EnHeishi4* self, GlobalContext* globalCtx) {
    f32 currentFrame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if (self->unk_288 <= currentFrame) {
        func_8002DF54(globalCtx, NULL, 7);
        self->actionFunc = func_80A5673C;
    }
}

void func_80A56B40(EnHeishi4* self, GlobalContext* globalCtx) {
    s16 reactionOffset;

    SkelAnime_Update(&self->skelAnime);
    reactionOffset = (self->type - 4);
    if (reactionOffset < 0) {
        reactionOffset = 0;
    }
    if (reactionOffset >= 3) {
        reactionOffset = 1;
    }
    if (Text_GetFaceReaction(globalCtx, sFaceReactionSets[reactionOffset]) != 0) {
        if (self->unk_2B4 == 0) {
            if ((self->type == HEISHI4_AT_KAKRIKO_ENTRANCE) || (self->type == HEISHI4_AT_IMPAS_HOUSE)) {
                self->actionFunc = func_80A563BC;
                return;
            }
            if (self->type == HEISHI4_AT_MARKET_NIGHT) {
                self->actionFunc = func_80A56614;
                return;
            }
        }
    } else {
        if (self->unk_2B4 != 0) {
            if ((self->type == HEISHI4_AT_KAKRIKO_ENTRANCE) || (self->type == HEISHI4_AT_IMPAS_HOUSE)) {
                self->actionFunc = func_80A563BC;
                return;
            }
            if (self->type == HEISHI4_AT_MARKET_NIGHT) {
                self->actionFunc = func_80A56614;
                return;
            }
        }
    }
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        if ((self->type == HEISHI4_AT_KAKRIKO_ENTRANCE) || (self->type == HEISHI4_AT_IMPAS_HOUSE)) {
            self->unk_284 = 1;
            self->actionFunc = func_80A563BC;
            return;
        }
        if (self->type == HEISHI4_AT_MARKET_NIGHT) {
            self->actionFunc = func_80A56614;
            return;
        }
    }
    func_8002F2F4(&self->actor, globalCtx);
}

void EnHeishi4_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi4* self = THIS;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    thisx->world.pos.x = self->pos.x;
    thisx->world.pos.y = self->pos.y;
    thisx->world.pos.z = self->pos.z;
    Actor_SetFocus(thisx, self->height);
    if (self->type != HEISHI4_AT_MARKET_DYING) {
        self->unk_28C.unk_18 = player->actor.world.pos;
        if (!LINK_IS_ADULT) {
            self->unk_28C.unk_18.y = (player->actor.world.pos.y - 10.0f);
        }
        func_80034A14(thisx, &self->unk_28C, 2, 4);
        self->unk_260 = self->unk_28C.unk_08;
        self->unk_266 = self->unk_28C.unk_0E;
    }
    self->unk_27E += 1;
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(globalCtx, thisx, 10.0f, 10.0f, 30.0f, 0x1D);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

s32 EnHeishi_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                              void* thisx) {
    EnHeishi4* self = THIS;

    if (limbIndex == 9) {
        rot->x += self->unk_266.y;
    }
    if (limbIndex == 16) {
        rot->x += self->unk_260.y;
        rot->z += self->unk_260.z;
    }
    return false;
}

void EnHeishi4_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnHeishi4* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnHeishi_OverrideLimbDraw, NULL,
                      self);
}
