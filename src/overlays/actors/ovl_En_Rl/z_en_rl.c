/*
 * File: z_en_rl.c
 * Overlay: En_Rl
 * Description: Rauru
 */

#include "z_en_rl.h"
#include "vt.h"
#include "objects/object_rl/object_rl.h"

#define FLAGS 0x00000010

#define THIS ((EnRl*)thisx)

void EnRl_Init(Actor* thisx, GlobalContext* globalCtx);
void EnRl_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnRl_Update(Actor* thisx, GlobalContext* globalCtx);
void EnRl_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AE7798(EnRl* self, GlobalContext* globalCtx);
void func_80AE77B8(EnRl* self, GlobalContext* globalCtx);
void func_80AE77F8(EnRl* self, GlobalContext* globalCtx);
void func_80AE7838(EnRl* self, GlobalContext* globalCtx);
void func_80AE7C64(EnRl* self, GlobalContext* globalCtx);
void func_80AE7C94(EnRl* self, GlobalContext* globalCtx);
void func_80AE7CE8(EnRl* self, GlobalContext* globalCtx);
void func_80AE7D40(EnRl* self, GlobalContext* globalCtx);
void func_80AE7FD0(EnRl* self, GlobalContext* globalCtx);
void func_80AE7FDC(EnRl* self, GlobalContext* globalCtx);
void func_80AE7D94(EnRl* self, GlobalContext* globalCtx);

static void* D_80AE81A0[] = { object_rl_Tex_003620, object_rl_Tex_003960, object_rl_Tex_003B60 };

void EnRl_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnRl* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
}

void func_80AE72D0(EnRl* self) {
    s32 pad[3];
    s16* timer = &self->timer;
    s16* eyeTextureIndex = &self->eyeTextureIndex;

    if (DECR(*timer) == 0) {
        *timer = Rand_S16Offset(60, 60);
    }

    *eyeTextureIndex = *timer;
    if (*eyeTextureIndex > 2) {
        *eyeTextureIndex = 0;
    }
}

void func_80AE7358(EnRl* self) {
    Animation_Change(&self->skelAnime, &object_rl_Anim_000A3C, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_rl_Anim_000A3C), ANIMMODE_LOOP, 0.0f);
    self->action = 4;
    self->drawConfig = 0;
    self->alpha = 0;
    self->lightBallSpawned = 0;
    self->actor.shape.shadowAlpha = 0;
    self->unk_19C = 0.0f;
}

void func_80AE73D8(EnRl* self, GlobalContext* globalCtx) {
    static s32 D_80AE81AC = 0;

    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        if (D_80AE81AC) {
            if (self->actor.params == 2) {
                func_80AE7358(self);
            }
            D_80AE81AC = 0;
        }
    } else if (!D_80AE81AC) {
        D_80AE81AC = 1;
    }
}

void func_80AE744C(EnRl* self, GlobalContext* globalCtx) {
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 75.0f, 30.0f, 30.0f, 5);
}

s32 func_80AE7494(EnRl* self) {
    return SkelAnime_Update(&self->skelAnime);
}

s32 func_80AE74B4(EnRl* self, GlobalContext* globalCtx, u16 arg2, s32 arg3) {
    CsCmdActorAction* csCmdActorAction;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        csCmdActorAction = globalCtx->csCtx.npcActions[arg3];
        if (csCmdActorAction != NULL && csCmdActorAction->action == arg2) {
            return 1;
        }
    }
    return 0;
}

s32 func_80AE74FC(EnRl* self, GlobalContext* globalCtx, u16 arg2, s32 arg3) {
    CsCmdActorAction* csCmdActorAction;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        csCmdActorAction = globalCtx->csCtx.npcActions[arg3];
        if (csCmdActorAction != NULL && csCmdActorAction->action != arg2) {
            return 1;
        }
    }
    return 0;
}

void func_80AE7544(EnRl* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_rl_Skel_007B38, &object_rl_Anim_000A3C, NULL, NULL, 0);
}

void func_80AE7590(EnRl* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player;
    Vec3f pos;
    s16 sceneNum = globalCtx->sceneNum;

    if (gSaveContext.sceneSetupIndex == 4 && sceneNum == SCENE_KENJYANOMA && globalCtx->csCtx.state != CS_STATE_IDLE &&
        globalCtx->csCtx.npcActions[6] != NULL && globalCtx->csCtx.npcActions[6]->action == 2 &&
        !self->lightMedallionGiven) {
        player = GET_PLAYER(globalCtx);
        pos.x = player->actor.world.pos.x;
        pos.y = player->actor.world.pos.y + 80.0f;
        pos.z = player->actor.world.pos.z;
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, pos.x, pos.y, pos.z, 0, 0, 0, 0xE);
        Item_Give(globalCtx, ITEM_MEDALLION_LIGHT);
        self->lightMedallionGiven = 1;
    }
}

void func_80AE7668(EnRl* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->drawConfig = 1;
    self->action = 1;
    player->actor.world.rot.y = player->actor.shape.rot.y = self->actor.world.rot.y + 0x8000;
}

void func_80AE7698(EnRl* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdActorAction;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        csCmdActorAction = globalCtx->csCtx.npcActions[0];
        if (csCmdActorAction != NULL && csCmdActorAction->action == 3) {
            Animation_Change(&self->skelAnime, &object_rl_Anim_00040C, 1.0f, 0.0f,
                             Animation_GetLastFrame(&object_rl_Anim_00040C), ANIMMODE_ONCE, 0.0f);
            self->action = 2;
        }
    }
}

void func_80AE772C(EnRl* self, s32 arg1) {
    if (arg1) {
        Animation_Change(&self->skelAnime, &object_rl_Anim_000830, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_rl_Anim_000830), ANIMMODE_LOOP, 0.0f);
        self->action = 3;
    }
}

void func_80AE7798(EnRl* self, GlobalContext* globalCtx) {
    func_80AE7668(self, globalCtx);
}

void func_80AE77B8(EnRl* self, GlobalContext* globalCtx) {
    func_80AE744C(self, globalCtx);
    func_80AE7494(self);
    func_80AE72D0(self);
    func_80AE7698(self, globalCtx);
}

void func_80AE77F8(EnRl* self, GlobalContext* globalCtx) {
    s32 temp;

    func_80AE744C(self, globalCtx);
    temp = func_80AE7494(self);
    func_80AE72D0(self);
    func_80AE772C(self, temp);
}

void func_80AE7838(EnRl* self, GlobalContext* globalCtx) {
    func_80AE744C(self, globalCtx);
    func_80AE7494(self);
    func_80AE72D0(self);
    func_80AE7590(self, globalCtx);
}

void func_80AE7878(EnRl* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_rl_Skel_007B38, &object_rl_Anim_000A3C, NULL, NULL, 0);
    self->action = 4;
    self->actor.shape.shadowAlpha = 0;
}

void func_80AE78D4(EnRl* self, GlobalContext* globalCtx) {
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_6K, self->actor.world.pos.x,
                       kREG(18) + 22.0f + self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 5);
}

void func_80AE7954(EnRl* self, GlobalContext* globalCtx) {
    if (func_80AE74B4(self, globalCtx, 4, 0)) {
        self->action = 5;
        self->drawConfig = 2;
        self->alpha = 0;
        self->actor.shape.shadowAlpha = 0;
        self->unk_19C = 0.0f;
    }
}

void func_80AE79A4(EnRl* self, GlobalContext* globalCtx) {
    f32* unk_19C = &self->unk_19C;
    s32 alpha = 255;

    if (func_80AE74B4(self, globalCtx, 4, 0)) {
        *unk_19C += 1.0f;
        if (*unk_19C >= kREG(5) + 10.0f) {
            self->action = 7;
            self->drawConfig = 1;
            *unk_19C = kREG(5) + 10.0f;
            self->alpha = alpha;
            self->actor.shape.shadowAlpha = alpha;
            return;
        }
    } else {
        *unk_19C -= 1.0f;
        if (*unk_19C <= 0.0f) {
            self->action = 4;
            self->drawConfig = 0;
            *unk_19C = 0.0f;
            self->alpha = 0;
            self->actor.shape.shadowAlpha = 0;
            return;
        }
    }
    alpha = (*unk_19C / (kREG(5) + 10.0f)) * 255.0f;
    self->alpha = alpha;
    self->actor.shape.shadowAlpha = alpha;
}

void func_80AE7AF8(EnRl* self, GlobalContext* globalCtx) {
    if (func_80AE74B4(self, globalCtx, 3, 0)) {
        Animation_Change(&self->skelAnime, &object_rl_Anim_00040C, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_rl_Anim_00040C), ANIMMODE_ONCE, -8.0f);
        self->action = 6;
    } else if (func_80AE74FC(self, globalCtx, 4, 0)) {
        self->action = 5;
        self->drawConfig = 2;
        self->unk_19C = kREG(5) + 10.0f;
        self->alpha = 255;
        if (!self->lightBallSpawned) {
            func_80AE78D4(self, globalCtx);
            self->lightBallSpawned = 1;
        }
        self->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80AE7BF8(EnRl* self, s32 arg1) {
    if (arg1 != 0) {
        Animation_Change(&self->skelAnime, &object_rl_Anim_000830, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_rl_Anim_000830), ANIMMODE_LOOP, 0.0f);
        self->action = 7;
    }
}

void func_80AE7C64(EnRl* self, GlobalContext* globalCtx) {
    func_80AE7954(self, globalCtx);
    func_80AE73D8(self, globalCtx);
}

void func_80AE7C94(EnRl* self, GlobalContext* globalCtx) {
    func_80AE744C(self, globalCtx);
    func_80AE7494(self);
    func_80AE72D0(self);
    func_80AE79A4(self, globalCtx);
    func_80AE73D8(self, globalCtx);
}

void func_80AE7CE8(EnRl* self, GlobalContext* globalCtx) {
    s32 temp;

    func_80AE744C(self, globalCtx);
    temp = func_80AE7494(self);
    func_80AE72D0(self);
    func_80AE7BF8(self, temp);
    func_80AE73D8(self, globalCtx);
}

void func_80AE7D40(EnRl* self, GlobalContext* globalCtx) {
    func_80AE744C(self, globalCtx);
    func_80AE7494(self);
    func_80AE72D0(self);
    func_80AE7AF8(self, globalCtx);
    func_80AE73D8(self, globalCtx);
}

void func_80AE7D94(EnRl* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s16 temp = self->eyeTextureIndex;
    void* tex = D_80AE81A0[temp];
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_rl_inKenjyanomaDemo02.c", 304);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(tex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(tex));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, self->alpha);
    gSPSegment(POLY_XLU_DISP++, 0x0C, D_80116280);

    POLY_XLU_DISP = SkelAnime_DrawFlex(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                                       NULL, NULL, NULL, POLY_XLU_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_rl_inKenjyanomaDemo02.c", 331);
}

static EnRlActionFunc sActionFuncs[] = {
    func_80AE7798, func_80AE77B8, func_80AE77F8, func_80AE7838,
    func_80AE7C64, func_80AE7C94, func_80AE7CE8, func_80AE7D40,
};

void EnRl_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnRl* self = THIS;

    if ((self->action < 0) || (self->action > 7) || (sActionFuncs[self->action] == NULL)) {
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sActionFuncs[self->action](self, globalCtx);
}

void EnRl_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnRl* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 50.0f);
    if (self->actor.params == 2) {
        func_80AE7878(self, globalCtx);
    } else {
        func_80AE7544(self, globalCtx);
    }
}
void func_80AE7FD0(EnRl* self, GlobalContext* globalCtx) {
}

void func_80AE7FDC(EnRl* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s16 temp = self->eyeTextureIndex;
    void* tex = D_80AE81A0[temp];
    SkelAnime* skelAnime = &self->skelAnime;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_rl.c", 416);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(tex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(tex));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);

    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL, NULL,
                          &self->actor);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_rl.c", 437);
}

static EnRlDrawFunc sDrawFuncs[] = {
    func_80AE7FD0,
    func_80AE7FDC,
    func_80AE7D94,
};

void EnRl_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnRl* self = THIS;

    if (self->drawConfig < 0 || self->drawConfig >= 3 || sDrawFuncs[self->drawConfig] == NULL) {
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sDrawFuncs[self->drawConfig](self, globalCtx);
}

const ActorInit En_Rl_InitVars = {
    ACTOR_EN_RL,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RL,
    sizeof(EnRl),
    (ActorFunc)EnRl_Init,
    (ActorFunc)EnRl_Destroy,
    (ActorFunc)EnRl_Update,
    (ActorFunc)EnRl_Draw,
};
