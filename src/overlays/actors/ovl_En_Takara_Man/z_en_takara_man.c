/*
 * File: z_en_takara_man.c
 * Overlay: ovl_En_Takara_Man
 * Description: Treasure Chest Game Man
 */

#include "z_en_takara_man.h"
#include "vt.h"
#include "objects/object_ts/object_ts.h"

#define FLAGS 0x08000039

#define THIS ((EnTakaraMan*)thisx)

void EnTakaraMan_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTakaraMan_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTakaraMan_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTakaraMan_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B176E0(EnTakaraMan* self, GlobalContext* globalCtx);
void func_80B1778C(EnTakaraMan* self, GlobalContext* globalCtx);
void func_80B17B14(EnTakaraMan* self, GlobalContext* globalCtx);
void func_80B17934(EnTakaraMan* self, GlobalContext* globalCtx);
void func_80B17A6C(EnTakaraMan* self, GlobalContext* globalCtx);
void func_80B17AC4(EnTakaraMan* self, GlobalContext* globalCtx);

const ActorInit En_Takara_Man_InitVars = {
    ACTOR_EN_TAKARA_MAN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TS,
    sizeof(EnTakaraMan),
    (ActorFunc)EnTakaraMan_Init,
    (ActorFunc)EnTakaraMan_Destroy,
    (ActorFunc)EnTakaraMan_Update,
    (ActorFunc)EnTakaraMan_Draw,
};

static u8 sTakaraIsInitialized = false;

void EnTakaraMan_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnTakaraMan_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnTakaraMan* self = THIS;

    if (sTakaraIsInitialized) {
        Actor_Kill(&self->actor);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ もういてる原 ☆☆☆☆☆ \n" VT_RST); // "Already initialized"
        return;
    }

    sTakaraIsInitialized = true;
    osSyncPrintf("\n\n");
    // "Bun! %x" (needs a better translation)
    osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ ばぅん！ ☆☆☆☆☆ %x\n" VT_RST, globalCtx->actorCtx.flags.chest);
    globalCtx->actorCtx.flags.chest = 0;
    gSaveContext.inventory.dungeonKeys[gSaveContext.mapIndex] = -1;
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_ts_Skel_004FE0, &object_ts_Anim_000498, self->jointTable,
                       self->morphTable, 10);
    thisx->focus.pos = thisx->world.pos;
    self->pos = thisx->world.pos;
    thisx->world.pos.x = 133.0f;
    thisx->world.pos.y = -12.0f;
    thisx->world.pos.z = 102.0f;
    Actor_SetScale(&self->actor, 0.013f);
    self->height = 90.0f;
    self->originalRoomNum = thisx->room;
    thisx->room = -1;
    thisx->world.rot.y = thisx->shape.rot.y = -0x4E20;
    thisx->targetMode = 1;
    self->actionFunc = func_80B176E0;
}

void func_80B176E0(EnTakaraMan* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&object_ts_Anim_000498);

    Animation_Change(&self->skelAnime, &object_ts_Anim_000498, 1.0f, 0.0f, (s16)frameCount, ANIMMODE_LOOP, -10.0f);
    if (!self->unk_214) {
        self->actor.textId = 0x6D;
        self->dialogState = 4;
    }
    self->actionFunc = func_80B1778C;
}

void func_80B1778C(EnTakaraMan* self, GlobalContext* globalCtx) {
    s16 absYawDiff;
    s16 yawDiff;

    SkelAnime_Update(&self->skelAnime);
    if (func_8002F194(&self->actor, globalCtx) && self->dialogState != 6) {
        if (!self->unk_214) {
            self->actionFunc = func_80B17934;
        } else {
            self->actionFunc = func_80B17B14;
        }
    } else {
        yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
        if (globalCtx->roomCtx.curRoom.num == 6 && !self->unk_21A) {
            self->actor.textId = 0x6E;
            self->unk_21A = 1;
            self->dialogState = 6;
        }

        if (!self->unk_21A && self->unk_214) {
            if (Flags_GetSwitch(globalCtx, 0x32)) {
                self->actor.textId = 0x84;
                self->dialogState = 5;
            } else {
                self->actor.textId = 0x704C;
                self->dialogState = 6;
            }
        }

        absYawDiff = ABS(yawDiff);
        if (absYawDiff < 0x4300) {
            if (globalCtx->roomCtx.curRoom.num != self->originalRoomNum) {
                self->actor.flags &= ~1;
                self->unk_218 = 0;
            } else {
                if (!self->unk_218) {
                    self->actor.flags |= 1;
                    self->unk_218 = 1;
                }
                func_8002F2CC(&self->actor, globalCtx, 100.0f);
            }
        }
    }
}

void func_80B17934(EnTakaraMan* self, GlobalContext* globalCtx) {
    if (self->dialogState == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // Yes
                if (gSaveContext.rupees >= 10) {
                    func_80106CCC(globalCtx);
                    Rupees_ChangeBy(-10);
                    self->unk_214 = 1;
                    self->actor.parent = NULL;
                    func_8002F434(&self->actor, globalCtx, GI_DOOR_KEY, 2000.0f, 1000.0f);
                    self->actionFunc = func_80B17A6C;
                } else {
                    func_80106CCC(globalCtx);
                    self->actor.textId = 0x85;
                    func_8010B720(globalCtx, self->actor.textId);
                    self->dialogState = 5;
                    self->actionFunc = func_80B17B14;
                }
                break;
            case 1: // No
                func_80106CCC(globalCtx);
                self->actor.textId = 0x2D;
                func_8010B720(globalCtx, self->actor.textId);
                self->dialogState = 5;
                self->actionFunc = func_80B17B14;
                break;
        }
    }
}

void func_80B17A6C(EnTakaraMan* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actionFunc = func_80B17AC4;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_DOOR_KEY, 2000.0f, 1000.0f);
    }
}

void func_80B17AC4(EnTakaraMan* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        self->actionFunc = func_80B176E0;
    }
}

void func_80B17B14(EnTakaraMan* self, GlobalContext* globalCtx) {
    if (self->dialogState == func_8010BDBC(&globalCtx->msgCtx) && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);
        self->actionFunc = func_80B176E0;
    }
}

void EnTakaraMan_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnTakaraMan* self = THIS;

    if (self->eyeTimer != 0) {
        self->eyeTimer--;
    }

    Actor_SetFocus(&self->actor, self->height);
    func_80038290(globalCtx, &self->actor, &self->unk_22C, &self->unk_232, self->actor.focus.pos);
    if (self->eyeTimer == 0) {
        self->eyeTextureIdx++;
        if (self->eyeTextureIdx >= 2) {
            self->eyeTextureIdx = 0;
            self->eyeTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }
    self->unk_212++;
    self->actionFunc(self, globalCtx);
}

s32 EnTakaraMan_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                 void* thisx) {
    EnTakaraMan* self = THIS;

    if (limbIndex == 1) {
        rot->x += self->unk_232.y;
    }
    if (limbIndex == 8) {
        rot->x += self->unk_22C.y;
        rot->z += self->unk_22C.z;
    }
    return false;
}

void EnTakaraMan_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = {
        0x06000970,
        0x06000D70,
    };
    EnTakaraMan* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_takara_man.c", 528);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTextureIdx]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnTakaraMan_OverrideLimbDraw, NULL, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_takara_man.c", 544);
}
