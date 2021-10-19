/*
 * File: z_bg_mori_bigst.c
 * Overlay: ovl_Bg_Mori_Bigst
 * Description: Forest Temple falling platform and Stalfos fight
 */

#include "z_bg_mori_bigst.h"
#include "objects/object_mori_objects/object_mori_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgMoriBigst*)thisx)

void BgMoriBigst_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMoriBigst_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMoriBigst_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMoriBigst_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMoriBigst_SetupWaitForMoriTex(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_WaitForMoriTex(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_SetupNoop(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_SetupStalfosFight(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_StalfosFight(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_SetupFall(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_Fall(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_SetupLanding(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_Landing(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_SetupStalfosPairFight(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_StalfosPairFight(BgMoriBigst* self, GlobalContext* globalCtx);
void BgMoriBigst_SetupDone(BgMoriBigst* self, GlobalContext* globalCtx);

const ActorInit Bg_Mori_Bigst_InitVars = {
    ACTOR_BG_MORI_BIGST,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MORI_OBJECTS,
    sizeof(BgMoriBigst),
    (ActorFunc)BgMoriBigst_Init,
    (ActorFunc)BgMoriBigst_Destroy,
    (ActorFunc)BgMoriBigst_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),      ICHAIN_F32(uncullZoneScale, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 3000, ICHAIN_CONTINUE),     ICHAIN_F32_DIV1000(gravity, -500, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(minVelocityY, -12000, ICHAIN_CONTINUE), ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgMoriBigst_SetupAction(BgMoriBigst* self, BgMoriBigstActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgMoriBigst_InitDynapoly(BgMoriBigst* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning : move BG login failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_mori_bigst.c", 190,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgMoriBigst_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriBigst* self = THIS;

    // "mori (bigST.keyceiling)"
    osSyncPrintf("mori (bigST.鍵型天井)(arg : %04x)(sw %d)(noE %d)(roomC %d)(playerPosY %f)\n", self->dyna.actor.params,
                 Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F),
                 Flags_GetTempClear(globalCtx, self->dyna.actor.room), Flags_GetClear(globalCtx, self->dyna.actor.room),
                 GET_PLAYER(globalCtx)->actor.world.pos.y);
    BgMoriBigst_InitDynapoly(self, globalCtx, &gMoriBigstCol, DPM_UNK);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->moriTexObjIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MORI_TEX);
    if (self->moriTexObjIndex < 0) {
        // "【Big Stalfos key ceiling】 bank danger!"
        osSyncPrintf("【ビッグスタルフォス鍵型天井】 バンク危険！\n");
        osSyncPrintf("%s %d\n", "../z_bg_mori_bigst.c", 234);
        Actor_Kill(&self->dyna.actor);
        return;
    }
    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;
    } else {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + 270.0f;
    }
    Actor_SetFocus(&self->dyna.actor, 50.0f);
    BgMoriBigst_SetupWaitForMoriTex(self, globalCtx);
}

void BgMoriBigst_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriBigst* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void BgMoriBigst_SetupWaitForMoriTex(BgMoriBigst* self, GlobalContext* globalCtx) {
    BgMoriBigst_SetupAction(self, BgMoriBigst_WaitForMoriTex);
}

void BgMoriBigst_WaitForMoriTex(BgMoriBigst* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->moriTexObjIndex)) {
        thisx->draw = BgMoriBigst_Draw;
        if (Flags_GetClear(globalCtx, thisx->room) && (GET_PLAYER(globalCtx)->actor.world.pos.y > 700.0f)) {
            if (Flags_GetSwitch(globalCtx, (thisx->params >> 8) & 0x3F)) {
                BgMoriBigst_SetupDone(self, globalCtx);
            } else {
                BgMoriBigst_SetupStalfosFight(self, globalCtx);
            }
        } else {
            BgMoriBigst_SetupNoop(self, globalCtx);
        }
    }
}

void BgMoriBigst_SetupNoop(BgMoriBigst* self, GlobalContext* globalCtx) {
    BgMoriBigst_SetupAction(self, NULL);
}

void BgMoriBigst_SetupStalfosFight(BgMoriBigst* self, GlobalContext* globalCtx) {
    Actor* stalfos;

    BgMoriBigst_SetupAction(self, BgMoriBigst_StalfosFight);
    Flags_UnsetClear(globalCtx, self->dyna.actor.room);
    stalfos = Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_EN_TEST, 209.0f, 827.0f,
                                 -3320.0f, 0, 0, 0, 1);
    if (stalfos != NULL) {
        self->dyna.actor.child = NULL;
        self->dyna.actor.home.rot.z++;
    } else {
        // "Second Stalfos failure"
        osSyncPrintf("Warning : 第２スタルフォス発生失敗\n");
    }
    Flags_SetClear(globalCtx, self->dyna.actor.room);
}

void BgMoriBigst_StalfosFight(BgMoriBigst* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((self->dyna.actor.home.rot.z == 0) &&
        ((self->dyna.actor.home.pos.y - 5.0f) <= GET_PLAYER(globalCtx)->actor.world.pos.y)) {
        BgMoriBigst_SetupFall(self, globalCtx);
        OnePointCutscene_Init(globalCtx, 3220, 72, &self->dyna.actor, MAIN_CAM);
    }
}

void BgMoriBigst_SetupFall(BgMoriBigst* self, GlobalContext* globalCtx) {
    BgMoriBigst_SetupAction(self, BgMoriBigst_Fall);
}

void BgMoriBigst_Fall(BgMoriBigst* self, GlobalContext* globalCtx) {
    Actor_MoveForward(&self->dyna.actor);
    if (self->dyna.actor.world.pos.y <= self->dyna.actor.home.pos.y) {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;
        BgMoriBigst_SetupLanding(self, globalCtx);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_STONE_BOUND);
        OnePointCutscene_Init(globalCtx, 1020, 8, &self->dyna.actor, MAIN_CAM);
        func_8002DF38(globalCtx, NULL, 0x3C);
    }
}

void BgMoriBigst_SetupLanding(BgMoriBigst* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 quake;

    BgMoriBigst_SetupAction(self, BgMoriBigst_Landing);
    self->waitTimer = 18;
    quake = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
    Quake_SetSpeed(quake, 25000);
    Quake_SetQuakeValues(quake, 5, 0, 0, 0);
    Quake_SetCountdown(quake, 16);
}

void BgMoriBigst_Landing(BgMoriBigst* self, GlobalContext* globalCtx) {
    if (self->waitTimer <= 0) {
        BgMoriBigst_SetupStalfosPairFight(self, globalCtx);
    }
}

void BgMoriBigst_SetupStalfosPairFight(BgMoriBigst* self, GlobalContext* globalCtx) {
    Actor* stalfos1;
    Actor* stalfos2;

    BgMoriBigst_SetupAction(self, BgMoriBigst_StalfosPairFight);
    Flags_UnsetClear(globalCtx, self->dyna.actor.room);
    stalfos1 = Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_EN_TEST, 70.0f, 827.0f,
                                  -3383.0f, 0, 0, 0, 5);
    if (stalfos1 != NULL) {
        self->dyna.actor.child = NULL;
        self->dyna.actor.home.rot.z++;
    } else {
        // "Warning: 3-1 Stalfos failure"
        osSyncPrintf("Warning : 第３-1スタルフォス発生失敗\n");
    }
    stalfos2 = Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_EN_TEST, 170.0f, 827.0f,
                                  -3260.0f, 0, 0, 0, 5);
    if (stalfos2 != NULL) {
        self->dyna.actor.child = NULL;
        self->dyna.actor.home.rot.z++;
    } else {
        // "Warning: 3-2 Stalfos failure"
        osSyncPrintf("Warning : 第３-2スタルフォス発生失敗\n");
    }
    Flags_SetClear(globalCtx, self->dyna.actor.room);
}

void BgMoriBigst_StalfosPairFight(BgMoriBigst* self, GlobalContext* globalCtx) {
    if ((self->dyna.actor.home.rot.z == 0) && !Player_InCsMode(globalCtx)) {
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);
        BgMoriBigst_SetupDone(self, globalCtx);
    }
}

void BgMoriBigst_SetupDone(BgMoriBigst* self, GlobalContext* globalCtx) {
    BgMoriBigst_SetupAction(self, NULL);
}

void BgMoriBigst_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriBigst* self = THIS;

    Actor_SetFocus(&self->dyna.actor, 50.0f);
    if (self->waitTimer > 0) {
        self->waitTimer--;
    }
    if (func_80043590(&self->dyna)) {
        func_80074CE8(globalCtx, 6);
    }
    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
}

void BgMoriBigst_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMoriBigst* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_bigst.c", 541);
    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, globalCtx->objectCtx.status[self->moriTexObjIndex].segment);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mori_bigst.c", 548),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, gMoriBigstDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mori_bigst.c", 553);
}
