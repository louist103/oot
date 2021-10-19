#include "z_door_warp1.h"
#include "objects/object_warp1/object_warp1.h"

#define FLAGS 0x00000000

#define THIS ((DoorWarp1*)thisx)

void DoorWarp1_Init(Actor* thisx, GlobalContext* globalCtx);
void DoorWarp1_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DoorWarp1_Update(Actor* thisx, GlobalContext* globalCtx);
void DoorWarp1_Draw(Actor* thisx, GlobalContext* globalCtx);

void DoorWarp1_WarpAppear(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_Destination(DoorWarp1* self, GlobalContext* globalCtx);
void func_8099B020(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_AwaitClearFlag(DoorWarp1* self, GlobalContext* globalCtx);
void func_8099A3A4(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_BlueCrystal(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_PurpleCrystal(DoorWarp1* self, GlobalContext* globalCtx);
void func_80999214(DoorWarp1* self, GlobalContext* globalCtx);
void func_80999348(DoorWarp1* self, GlobalContext* globalCtx);
void func_809995D4(DoorWarp1* self, GlobalContext* globalCtx);
void func_809998A4(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_ChildWarpIdle(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_RutoWarpIdle(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_ChildWarpOut(DoorWarp1* self, GlobalContext* globalCtx);
void func_80999EE0(DoorWarp1* self, GlobalContext* globalCtx);
void func_80999FE4(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_RutoWarpOut(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_AdultWarpIdle(DoorWarp1* self, GlobalContext* globalCtx);
void func_8099A508(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_AdultWarpOut(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_DoNothing(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_ChooseInitialAction(DoorWarp1* self, GlobalContext* globalCtx);
void DoorWarp1_FloatPlayer(DoorWarp1* self, GlobalContext* globalCtx);

const ActorInit Door_Warp1_InitVars = {
    ACTOR_DOOR_WARP1,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_WARP1,
    sizeof(DoorWarp1),
    (ActorFunc)DoorWarp1_Init,
    (ActorFunc)DoorWarp1_Destroy,
    (ActorFunc)DoorWarp1_Update,
    (ActorFunc)DoorWarp1_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 4000, ICHAIN_STOP),
};

static s16 sWarpTimerTarget;

void DoorWarp1_SetupAction(DoorWarp1* self, DoorWarp1ActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void DoorWarp1_Init(Actor* thisx, GlobalContext* globalCtx) {
    DoorWarp1* self = THIS;
    GlobalContext* globalCtx2 = globalCtx;

    self->unk_1B8 = 0;
    self->unk_1B4 = 0.0f;
    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);

    if (self->actor.params != WARP_SAGES && self->actor.params != WARP_BLUE_CRYSTAL &&
        self->actor.params != WARP_YELLOW && self->actor.params != WARP_DESTINATION) {
        Lights_PointNoGlowSetInfo(&self->upperLightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 0, 0, 0, 0);
        self->upperLight = LightContext_InsertLight(globalCtx2, &globalCtx2->lightCtx, &self->upperLightInfo);

        Lights_PointNoGlowSetInfo(&self->lowerLightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 0, 0, 0, 0);
        self->lowerLight = LightContext_InsertLight(globalCtx2, &globalCtx2->lightCtx, &self->lowerLightInfo);
    }
    osSyncPrintf("\nBOSSWARP arg_data=[%d]", self->actor.params);

    DoorWarp1_ChooseInitialAction(self, globalCtx2);
}

void DoorWarp1_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    u8 i;
    DoorWarp1* self = THIS;

    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->upperLight);
    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lowerLight);

    for (i = 0; i < 3; i++) {
        globalCtx->envCtx.adjAmbientColor[i] = globalCtx->envCtx.adjFogColor[i] = globalCtx->envCtx.adjLight1Color[i] =
            0;
    }
    //! @bug SkelAnime_Free is not called for crystal variants
}

void DoorWarp1_SetupWarp(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->scale = 0;
    self->unk_1AE = -140;
    self->unk_1B0 = -80;
    sWarpTimerTarget = 100;
    self->unk_1BC = 1.0f;
    self->lightRayAlpha = 0.0f;
    self->warpAlpha = 0.0f;
    self->crystalAlpha = 0.0f;

    switch (self->actor.params) {
        case WARP_YELLOW:
        case WARP_ORANGE:
        case WARP_GREEN:
        case WARP_RED:
            self->unk_194 = 0.23f;
            self->unk_198 = 0.6f;
            break;
        case WARP_DESTINATION:
            self->unk_194 = 0.0f;
            self->unk_198 = 0.0f;
            break;
        case WARP_UNK_7:
            self->scale = 100;
            self->unk_1AE = 120;
            self->unk_1B0 = 230;
            self->unk_194 = 0.3f;
            self->unk_198 = 0.3f;
            break;
        case WARP_BLUE_RUTO:
        default:
            self->unk_194 = 0.3f;
            self->unk_198 = 0.3f;
            break;
    }

    self->unk_19C = 0.0f;
    self->actor.shape.yOffset = 1.0f;
    self->warpTimer = 0;

    switch (self->actor.params) {
        case WARP_PURPLE_CRYSTAL:
        case WARP_BLUE_RUTO:
        case WARP_UNK_7:
        default:
            Lights_PointNoGlowSetInfo(&self->upperLightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                      self->actor.world.pos.z, 200, 255, 255, 255);
            Lights_PointNoGlowSetInfo(&self->lowerLightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                      self->actor.world.pos.z, 200, 255, 255, 255);
            break;
        case WARP_CLEAR_FLAG:
        case WARP_SAGES:
        case WARP_YELLOW:
        case WARP_DESTINATION:
        case WARP_ORANGE:
        case WARP_GREEN:
        case WARP_RED:
            break;
    }

    switch (self->actor.params) {
        case WARP_CLEAR_FLAG:
            DoorWarp1_SetupAction(self, DoorWarp1_AwaitClearFlag);
            break;
        case WARP_DESTINATION:
            if ((!(gSaveContext.entranceIndex == 0x608 ||  // sacred forest meadow
                   gSaveContext.entranceIndex == 0x564 ||  // death mountain crater
                   gSaveContext.entranceIndex == 0x60C ||  // lake hylia
                   gSaveContext.entranceIndex == 0x610 ||  // desert colossus
                   gSaveContext.entranceIndex == 0x580) && // graveyard
                 gSaveContext.sceneSetupIndex < 4) ||
                (GET_PLAYER(globalCtx)->actor.params & 0xF00) != 0x200) {
                Actor_Kill(&self->actor);
            }
            if (Actor_WorldDistXZToActor(&player->actor, &self->actor) > 100.0f) {
                Actor_Kill(&self->actor);
            }
            DoorWarp1_SetupAction(self, DoorWarp1_Destination);
            break;
        case WARP_UNK_7:
            DoorWarp1_SetupAction(self, func_8099B020);
            break;
        default:
            DoorWarp1_SetupAction(self, DoorWarp1_WarpAppear);
            break;
    }
}

void DoorWarp1_SetupAdultDungeonWarp(DoorWarp1* self, GlobalContext* globalCtx) {
    SkelAnime_Init(globalCtx, &self->skelAnime, &gWarpCrystalSkel, &gWarpCrystalAnim, NULL, NULL, 0);
    Animation_ChangeImpl(&self->skelAnime, &gWarpCrystalAnim, 1.0f, 1.0f, 1.0f, ANIMMODE_ONCE, 40.0f, 1);

    self->scale = 0;
    self->unk_1AE = -140;
    self->unk_1B0 = -80;
    sWarpTimerTarget = 160;
    self->actor.shape.yOffset = -400.0f;
    self->warpTimer = 0;
    self->unk_1BC = 1.0f;
    self->unk_194 = 0.3f;
    self->unk_198 = 0.3f;
    self->lightRayAlpha = 0.0f;
    self->warpAlpha = 0.0f;
    self->crystalAlpha = 0.0f;
    self->unk_19C = 0.0f;

    Lights_PointNoGlowSetInfo(&self->upperLightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, 200, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&self->lowerLightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, 200, 255, 255, 255);

    DoorWarp1_SetupAction(self, func_8099A3A4);
}

void DoorWarp1_SetupBlueCrystal(DoorWarp1* self, GlobalContext* globalCtx) {
    s16 i;

    SkelAnime_Init(globalCtx, &self->skelAnime, &gWarpCrystalSkel, &gWarpCrystalAnim, NULL, NULL, 0);
    Animation_ChangeImpl(&self->skelAnime, &gWarpCrystalAnim, 0, Animation_GetLastFrame(&gWarpCrystalAnim),
                         Animation_GetLastFrame(&gWarpCrystalAnim), ANIMMODE_ONCE, 0.0f, 1);

    self->skelAnime.curFrame = Animation_GetLastFrame(&gWarpCrystalAnim);
    self->scale = 10;
    self->unk_1AE = 120;
    self->unk_1B0 = 230;
    self->warpTimer = 0;
    self->unk_194 = 0.3f;
    self->unk_198 = 0.3f;
    self->lightRayAlpha = 0.0f;
    self->warpAlpha = 0.0f;
    self->crystalAlpha = 0.0f;
    self->unk_19C = 0.0f;
    self->unk_1BC = 1.0f;
    self->actor.shape.yOffset = 800.0f;

    for (i = 0; i < 3; i++) {
        globalCtx->envCtx.adjAmbientColor[i] = globalCtx->envCtx.adjFogColor[i] = globalCtx->envCtx.adjLight1Color[i] =
            -255;
    }

    globalCtx->envCtx.adjFogNear = -500;
    self->warpTimer = 30;
    self->unk_1B8 = 4000;
    DoorWarp1_SetupAction(self, DoorWarp1_BlueCrystal);
}

void DoorWarp1_SetupPurpleCrystal(DoorWarp1* self, GlobalContext* globalCtx) {
    SkelAnime_Init(globalCtx, &self->skelAnime, &gWarpCrystalSkel, &gWarpCrystalAnim, NULL, NULL, 0);
    Animation_ChangeImpl(&self->skelAnime, &gWarpCrystalAnim, 0, Animation_GetLastFrame(&gWarpCrystalAnim),
                         Animation_GetLastFrame(&gWarpCrystalAnim), ANIMMODE_ONCE, 0.0f, 1);

    self->skelAnime.curFrame = Animation_GetLastFrame(&gWarpCrystalAnim);
    self->unk_1AE = 120;
    self->unk_1B0 = 230;
    self->warpTimer = 200;
    self->unk_1B8 = 4000;
    self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = 1.0f;
    self->unk_194 = 0.3f;
    self->unk_198 = 0.3f;
    self->lightRayAlpha = 0.0f;
    self->warpAlpha = 0.0f;
    self->crystalAlpha = 0.0f;
    self->unk_19C = 0.0f;
    self->unk_1BC = 1.f;
    self->actor.shape.yOffset = 800.0f;

    if (gSaveContext.entranceIndex != 0x53) {
        self->actor.scale.x = 0.0499f;
        self->actor.scale.y = 0.077f;
        self->actor.scale.z = 0.09f;
        self->crystalAlpha = 255.0f;
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_SHUT_BY_CRYSTAL);
    }
    DoorWarp1_SetupAction(self, DoorWarp1_PurpleCrystal);
}

void DoorWarp1_SetPlayerPos(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    player->actor.velocity.y = 0.0f;
    player->actor.world.pos.x = self->actor.world.pos.x;
    player->actor.world.pos.y = self->actor.world.pos.y + 55.0f;
    player->actor.world.pos.z = self->actor.world.pos.z;
}

void DoorWarp1_BlueCrystal(DoorWarp1* self, GlobalContext* globalCtx) {
    if (self->warpTimer != 0) {
        self->warpTimer--;
    } else {
        DoorWarp1_SetupAction(self, func_80999214);
    }
    DoorWarp1_SetPlayerPos(self, globalCtx);
}

void func_80999214(DoorWarp1* self, GlobalContext* globalCtx) {
    s32 temp_f4;
    f32 darkness;
    s16 i;

    Math_SmoothStepToF(&self->crystalAlpha, 255.0f, 0.2f, 5.0f, 0.1f);

    darkness = (f32)(40 - self->warpTimer) / 40.0f;
    darkness = CLAMP_MIN(darkness, 0);

    for (i = 0; i < 3; i++) {
        globalCtx->envCtx.adjAmbientColor[i] = globalCtx->envCtx.adjFogColor[i] = globalCtx->envCtx.adjLight1Color[i] =
            -255 * darkness;
    }
    globalCtx->envCtx.adjFogNear = -500.0f * darkness;

    self->warpTimer++;
    if (darkness <= 0) {
        DoorWarp1_SetupAction(self, func_80999348);
    }
    self->actor.shape.rot.y += 0x320;
    DoorWarp1_SetPlayerPos(self, globalCtx);
}

void func_80999348(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    DoorWarp1_SetPlayerPos(self, globalCtx);

    if (self->warpTimer == 0) {
        Math_SmoothStepToF(&self->crystalAlpha, 0.0f, 0.1f, 4.0f, 1.0f);
        if (self->crystalAlpha <= 150.0f) {
            player->actor.gravity = -0.1f;
        }
        if (self->crystalAlpha <= 0.0f) {
            DoorWarp1_SetupAction(self, DoorWarp1_FloatPlayer);
        }
    } else {
        self->warpTimer--;
    }
    self->actor.shape.rot.y += 0x320;
}

void DoorWarp1_FloatPlayer(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    player->actor.gravity = -0.1f;
}

void DoorWarp1_PurpleCrystal(DoorWarp1* self, GlobalContext* globalCtx) {
    if (self->warpTimer != 0) {
        self->warpTimer--;
        Math_SmoothStepToF(&self->actor.scale.x, 0.0499f, 0.2f, 0.05f, 0.001f);
        Math_SmoothStepToF(&self->actor.scale.y, 0.077f, 0.2f, 0.05f, 0.001f);
        Math_SmoothStepToF(&self->actor.scale.z, 0.09f, 0.2f, 0.05f, 0.001f);
        Math_SmoothStepToF(&self->crystalAlpha, 255.0f, 0.2f, 5.0f, 0.1f);
    }
}

void DoorWarp1_ChooseInitialAction(DoorWarp1* self, GlobalContext* globalCtx) {
    switch (self->actor.params) {
        case WARP_DUNGEON_CHILD:
        case WARP_CLEAR_FLAG:
        case WARP_SAGES:
        case WARP_YELLOW:
        case WARP_BLUE_RUTO:
        case WARP_DESTINATION:
        case WARP_UNK_7:
        case WARP_ORANGE:
        case WARP_GREEN:
        case WARP_RED:
            DoorWarp1_SetupWarp(self, globalCtx);
            break;
        case WARP_DUNGEON_ADULT:
            DoorWarp1_SetupAdultDungeonWarp(self, globalCtx);
            break;
        case WARP_BLUE_CRYSTAL:
            DoorWarp1_SetupBlueCrystal(self, globalCtx);
            break;
        case WARP_PURPLE_CRYSTAL:
            DoorWarp1_SetupPurpleCrystal(self, globalCtx);
            break;
    }
}

void DoorWarp1_AwaitClearFlag(DoorWarp1* self, GlobalContext* globalCtx) {
    if (Flags_GetTempClear(globalCtx, self->actor.room)) {
        self->warpTimer = 200;
        Audio_QueueSeqCmd(0x21);
        DoorWarp1_SetupAction(self, func_809995D4);
    }
}

void func_809995D4(DoorWarp1* self, GlobalContext* globalCtx) {
    if (self->warpTimer == 0) {
        if (self->actor.xzDistToPlayer < 100.0f) {
            self->actor.world.pos.x = -98.0f;
            self->actor.world.pos.y = 827.0f;
            self->actor.world.pos.z = -3228.0f;
        }
        Lights_PointNoGlowSetInfo(&self->upperLightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 200, 255, 255, 255);
        Lights_PointNoGlowSetInfo(&self->lowerLightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 200, 255, 255, 255);
        DoorWarp1_SetupAction(self, DoorWarp1_WarpAppear);
    }
    self->warpTimer--;
}

void DoorWarp1_WarpAppear(DoorWarp1* self, GlobalContext* globalCtx) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    Math_SmoothStepToF(&self->lightRayAlpha, 255.0f, 0.4f, 10.0f, 0.01f);
    Math_SmoothStepToF(&self->warpAlpha, 255.0f, 0.4f, 10.0f, 0.01f);

    if (self->actor.params != WARP_YELLOW && self->actor.params != WARP_ORANGE && self->actor.params != WARP_GREEN &&
        self->actor.params != WARP_RED) {
        if (self->scale < 100) {
            self->scale += 2;
        }
        if (self->unk_1AE < 120) {
            self->unk_1AE += 4;
        }
        if (self->unk_1B0 < 230) {
            self->unk_1B0 += 4;
        } else if (self->actor.params == WARP_BLUE_RUTO) {
            DoorWarp1_SetupAction(self, DoorWarp1_RutoWarpIdle);
        } else if (self->actor.params != WARP_SAGES && self->actor.params != WARP_YELLOW) {
            DoorWarp1_SetupAction(self, DoorWarp1_ChildWarpIdle);
        } else {
            DoorWarp1_SetupAction(self, func_809998A4);
        }
    } else {
        if (self->unk_1AE < -50) {
            self->unk_1AE += 4;
        }
        if (self->unk_1B0 < 70) {
            self->unk_1B0 += 4;
        } else {
            DoorWarp1_SetupAction(self, func_809998A4);
        }
    }
}

void func_809998A4(DoorWarp1* self, GlobalContext* globalCtx) {
    if (self->lightRayAlpha != 0.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    }
    Math_SmoothStepToF(&self->lightRayAlpha, 0.0f, 0.1f, 2.0f, 0.01f);
    Math_SmoothStepToF(&self->warpAlpha, 0.0f, 0.1f, 2.0f, 0.01f);
}

s32 DoorWarp1_PlayerInRange(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 ret = false;

    if (fabsf(self->actor.xzDistToPlayer) < 60.0f) {
        if ((player->actor.world.pos.y - 20.0f) < self->actor.world.pos.y) {
            if (self->actor.world.pos.y < (player->actor.world.pos.y + 20.0f)) {
                ret = true;
            }
        }
    }
    return ret;
}

void DoorWarp1_ChildWarpIdle(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player;

    Audio_PlayActorSound2(&self->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);

    if (DoorWarp1_PlayerInRange(self, globalCtx)) {
        player = GET_PLAYER(globalCtx);

        Audio_PlaySoundGeneral(NA_SE_EV_LINK_WARP, &player->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        OnePointCutscene_Init(globalCtx, 0x25E7, 999, &self->actor, MAIN_CAM);
        func_8002DF54(globalCtx, &self->actor, 10);

        player->unk_450.x = self->actor.world.pos.x;
        player->unk_450.z = self->actor.world.pos.z;
        self->unk_1B2 = 1;
        DoorWarp1_SetupAction(self, DoorWarp1_ChildWarpOut);
    }
}

void DoorWarp1_ChildWarpOut(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->unk_1B2 >= 101) {
        if (player->actor.velocity.y < 10.0f) {
            player->actor.gravity = 0.1f;
        } else {
            player->actor.gravity = 0.0f;
        }
    } else {
        self->unk_1B2++;
    }

    Math_SmoothStepToF(&self->lightRayAlpha, 0.0f, 0.2f, 6.0f, 0.01f);
    self->warpTimer++;

    if (sWarpTimerTarget < self->warpTimer && gSaveContext.nextCutsceneIndex == 0xFFEF) {
        osSyncPrintf("\n\n\nじかんがきたからおーしまい fade_direction=[%d]", globalCtx->sceneLoadFlag, 0x14);

        if (globalCtx->sceneNum == SCENE_DDAN_BOSS) {
            if (!Flags_GetEventChkInf(0x25)) {
                Flags_SetEventChkInf(0x25);
                Item_Give(globalCtx, ITEM_GORON_RUBY);
                globalCtx->nextEntranceIndex = 0x13D;
                gSaveContext.nextCutsceneIndex = 0xFFF1;
            } else {
                globalCtx->nextEntranceIndex = 0x47A;
                gSaveContext.nextCutsceneIndex = 0;
            }
        } else if (globalCtx->sceneNum == SCENE_YDAN_BOSS) {
            if (!Flags_GetEventChkInf(7)) {
                Flags_SetEventChkInf(7);
                Flags_SetEventChkInf(9);
                Item_Give(globalCtx, ITEM_KOKIRI_EMERALD);
                globalCtx->nextEntranceIndex = 0xEE;
                gSaveContext.nextCutsceneIndex = 0xFFF1;
            } else {
                globalCtx->nextEntranceIndex = 0x457;
                gSaveContext.nextCutsceneIndex = 0;
            }
        } else if (globalCtx->sceneNum == SCENE_BDAN_BOSS) {
            globalCtx->nextEntranceIndex = 0x10E;
            gSaveContext.nextCutsceneIndex = 0;
        }
        osSyncPrintf("\n\n\nおわりおわり");
        globalCtx->sceneLoadFlag = 0x14;
        globalCtx->fadeTransition = 7;
        gSaveContext.nextTransition = 3;
    }

    Math_StepToF(&self->unk_194, 2.0f, 0.01f);
    Math_StepToF(&self->unk_198, 10.0f, 0.02f);
    Lights_PointNoGlowSetInfo(&self->upperLightInfo, (s16)player->actor.world.pos.x + 10.0f,
                              (s16)player->actor.world.pos.y + 10.0f, (s16)player->actor.world.pos.z + 10.0f, 235, 255,
                              255, 255);
    Lights_PointNoGlowSetInfo(&self->lowerLightInfo, (s16)player->actor.world.pos.x - 10.0f,
                              (s16)player->actor.world.pos.y - 10.0f, (s16)player->actor.world.pos.z - 10.0f, 235, 255,
                              255, 255);
    Math_SmoothStepToF(&self->actor.shape.yOffset, 0.0f, 0.5f, 2.0f, 0.1f);
}

void DoorWarp1_RutoWarpIdle(DoorWarp1* self, GlobalContext* globalCtx) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);

    if (self->rutoWarpState != WARP_BLUE_RUTO_STATE_INITIAL && DoorWarp1_PlayerInRange(self, globalCtx)) {
        self->rutoWarpState = WARP_BLUE_RUTO_STATE_ENTERED;
        func_8002DF54(globalCtx, &self->actor, 10);
        self->unk_1B2 = 1;
        DoorWarp1_SetupAction(self, func_80999EE0);
    }
}

static s16 sRutoWarpSubCamId;

void func_80999EE0(DoorWarp1* self, GlobalContext* globalCtx) {
    Vec3f at;
    Vec3f eye;
    Player* player = GET_PLAYER(globalCtx);

    if (self->rutoWarpState == WARP_BLUE_RUTO_STATE_3) {
        Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
        sRutoWarpSubCamId = Gameplay_CreateSubCamera(globalCtx);

        Gameplay_ChangeCameraStatus(globalCtx, sRutoWarpSubCamId, CAM_STAT_ACTIVE);
        at.x = self->actor.world.pos.x;
        at.y = 49.0f;
        at.z = self->actor.world.pos.z;
        eye.x = player->actor.world.pos.x;
        eye.y = 43.0f;
        eye.z = player->actor.world.pos.z;

        Gameplay_CameraSetAtEye(globalCtx, sRutoWarpSubCamId, &at, &eye);
        Gameplay_CameraSetFov(globalCtx, sRutoWarpSubCamId, 90.0f);
        self->rutoWarpState = WARP_BLUE_RUTO_STATE_TALKING;
        func_8010B680(globalCtx, 0x4022, NULL);
        DoorWarp1_SetupAction(self, func_80999FE4);
    }
}

void func_80999FE4(DoorWarp1* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 0) {
        Audio_PlaySoundGeneral(NA_SE_EV_LINK_WARP, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        OnePointCutscene_Init(globalCtx, 0x25E9, 999, &self->actor, MAIN_CAM);
        Gameplay_CopyCamera(globalCtx, -1, sRutoWarpSubCamId);
        Gameplay_ChangeCameraStatus(globalCtx, sRutoWarpSubCamId, CAM_STAT_WAIT);
        self->rutoWarpState = WARP_BLUE_RUTO_STATE_WARPING;
        DoorWarp1_SetupAction(self, DoorWarp1_RutoWarpOut);
    }
}

void DoorWarp1_RutoWarpOut(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->unk_1B2 >= 61) {
        if (player->actor.velocity.y < 10.f) {
            player->actor.gravity = 0.02f;
        } else {
            player->actor.gravity = 0.0f;
        }
    } else {
        self->unk_1B2++;
    }
    Math_SmoothStepToF(&self->lightRayAlpha, 0.0f, 0.2f, 6.0f, 0.01f);
    self->warpTimer++;

    if (self->warpTimer > sWarpTimerTarget && gSaveContext.nextCutsceneIndex == 0xFFEF) {
        gSaveContext.eventChkInf[3] |= 0x80;
        Item_Give(globalCtx, ITEM_ZORA_SAPPHIRE);
        globalCtx->nextEntranceIndex = 0x10E;
        gSaveContext.nextCutsceneIndex = 0xFFF0;
        globalCtx->sceneLoadFlag = 0x14;
        globalCtx->fadeTransition = 7;
    }

    Math_StepToF(&self->unk_194, 2.0f, 0.01f);
    Math_StepToF(&self->unk_198, 10.f, 0.02f);
    Lights_PointNoGlowSetInfo(&self->upperLightInfo, (s16)player->actor.world.pos.x + 10.0f,
                              (s16)player->actor.world.pos.y + 10.0f, (s16)player->actor.world.pos.z + 10.0f, 235, 255,
                              255, 255);
    Lights_PointNoGlowSetInfo(&self->lowerLightInfo, (s16)player->actor.world.pos.x - 10.0f,
                              (s16)player->actor.world.pos.y - 10.0f, (s16)player->actor.world.pos.z - 10.0f, 235, 255,
                              255, 255);
    Math_SmoothStepToF(&self->actor.shape.yOffset, 0.0f, 0.5f, 2.0f, 0.1f);
}

void func_8099A3A4(DoorWarp1* self, GlobalContext* globalCtx) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    Math_SmoothStepToF(&self->lightRayAlpha, 255.0f, 0.2f, 2.0f, 0.1f);
    Math_SmoothStepToF(&self->warpAlpha, 255.0f, 0.2f, 2.0f, 0.1f);

    if (self->scale < 10) {
        self->scale += 2;
    }
    if (self->unk_1AE < 120) {
        self->unk_1AE += 4;
    }
    if (self->unk_1B0 < 230) {
        self->unk_1B0 += 4;
    } else {
        DoorWarp1_SetupAction(self, DoorWarp1_AdultWarpIdle);
    }
}

void DoorWarp1_AdultWarpIdle(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player;

    Audio_PlayActorSound2(&self->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);

    if (DoorWarp1_PlayerInRange(self, globalCtx)) {
        player = GET_PLAYER(globalCtx);

        OnePointCutscene_Init(globalCtx, 0x25E8, 999, &self->actor, MAIN_CAM);
        func_8002DF54(globalCtx, &self->actor, 10);
        player->unk_450.x = self->actor.world.pos.x;
        player->unk_450.z = self->actor.world.pos.z;
        self->unk_1B2 = 20;
        DoorWarp1_SetupAction(self, func_8099A508);
    }
}

void func_8099A508(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->unk_1B2 != 0) {
        self->unk_1B2--;
        return;
    }
    Audio_PlaySoundGeneral(NA_SE_EV_LINK_WARP, &player->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    Animation_ChangeImpl(&self->skelAnime, &gWarpCrystalAnim, 1.0f, Animation_GetLastFrame(&gWarpCrystalAnim),
                         Animation_GetLastFrame(&gWarpCrystalAnim), ANIMMODE_ONCE, 40.0f, 1);

    self->unk_1B2 = 0x32;
    DoorWarp1_SetupAction(self, DoorWarp1_AdultWarpOut);
}

void DoorWarp1_AdultWarpOut(DoorWarp1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 temp_f0_2;

    if (self->unk_1B2 != 0) {
        self->unk_1B2--;
    }

    if (self->unk_1B2 < 31) {
        u32 phi_v0 = (LINK_IS_ADULT) ? 35 : 45;

        if ((player->actor.world.pos.y - self->actor.world.pos.y) <= phi_v0) {
            player->actor.gravity = 0.0139999995f;
        } else {
            player->actor.gravity = 0.0f;
            player->actor.velocity.y = 0.0f;
        }
        if (self->unk_1B2 <= 0) {
            if (self->unk_1B8 < 4000) {
                self->unk_1B8 += 40;
            }
            player->actor.world.rot.y -= self->unk_1B8;
            player->actor.shape.rot.y -= self->unk_1B8;
        }
        Math_SmoothStepToF(&player->actor.world.pos.x, self->actor.world.pos.x, 0.5f, 0.1f, 0.01f);
        Math_SmoothStepToF(&player->actor.world.pos.z, self->actor.world.pos.z, 0.5f, 0.1f, 0.01f);
    }
    self->warpTimer++;

    if (self->warpTimer > sWarpTimerTarget && gSaveContext.nextCutsceneIndex == 0xFFEF) {
        if (globalCtx->sceneNum == SCENE_MORIBOSSROOM) {
            if (!(gSaveContext.eventChkInf[4] & 0x100)) {
                gSaveContext.eventChkInf[4] |= 0x100;
                Item_Give(globalCtx, ITEM_MEDALLION_FOREST);
                globalCtx->nextEntranceIndex = 0x6B;
                gSaveContext.nextCutsceneIndex = 0;
                gSaveContext.chamberCutsceneNum = CHAMBER_CS_FOREST;
            } else {
                if (!LINK_IS_ADULT) {
                    globalCtx->nextEntranceIndex = 0x600;
                } else {
                    globalCtx->nextEntranceIndex = 0x608;
                }
                gSaveContext.nextCutsceneIndex = 0;
            }
        } else if (globalCtx->sceneNum == SCENE_FIRE_BS) {
            if (!(gSaveContext.eventChkInf[4] & 0x200)) {
                gSaveContext.eventChkInf[4] |= 0x200;
                Item_Give(globalCtx, ITEM_MEDALLION_FIRE);
                globalCtx->nextEntranceIndex = 0xDB;
                gSaveContext.nextCutsceneIndex = 0xFFF3;
            } else {
                if (!LINK_IS_ADULT) {
                    globalCtx->nextEntranceIndex = 0x4F6;
                } else {
                    globalCtx->nextEntranceIndex = 0x564;
                }
                gSaveContext.nextCutsceneIndex = 0;
            }
        } else if (globalCtx->sceneNum == SCENE_MIZUSIN_BS) {
            if (!(gSaveContext.eventChkInf[4] & 0x400)) {
                gSaveContext.eventChkInf[4] |= 0x400;
                Item_Give(globalCtx, ITEM_MEDALLION_WATER);
                globalCtx->nextEntranceIndex = 0x6B;
                gSaveContext.nextCutsceneIndex = 0;
                gSaveContext.chamberCutsceneNum = CHAMBER_CS_WATER;
            } else {
                if (!LINK_IS_ADULT) {
                    globalCtx->nextEntranceIndex = 0x604;
                } else {
                    globalCtx->nextEntranceIndex = 0x60C;
                }
                gSaveContext.nextCutsceneIndex = 0;
            }
        } else if (globalCtx->sceneNum == SCENE_JYASINBOSS) {
            if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_SPIRIT)) {
                Item_Give(globalCtx, ITEM_MEDALLION_SPIRIT);
                globalCtx->nextEntranceIndex = 0x6B;
                gSaveContext.nextCutsceneIndex = 0;
                gSaveContext.chamberCutsceneNum = CHAMBER_CS_SPIRIT;
            } else {
                if (!LINK_IS_ADULT) {
                    globalCtx->nextEntranceIndex = 0x1F1;
                } else {
                    globalCtx->nextEntranceIndex = 0x610;
                }
                gSaveContext.nextCutsceneIndex = 0;
            }
        } else if (globalCtx->sceneNum == SCENE_HAKADAN_BS) {
            if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_SHADOW)) {
                Item_Give(globalCtx, ITEM_MEDALLION_SHADOW);
                globalCtx->nextEntranceIndex = 0x6B;
                gSaveContext.nextCutsceneIndex = 0;
                gSaveContext.chamberCutsceneNum = CHAMBER_CS_SHADOW;
            } else {
                if (!LINK_IS_ADULT) {
                    globalCtx->nextEntranceIndex = 0x568;
                } else {
                    globalCtx->nextEntranceIndex = 0x580;
                }
                gSaveContext.nextCutsceneIndex = 0;
            }
        }
        globalCtx->sceneLoadFlag = 0x14;
        globalCtx->fadeTransition = 3;
        gSaveContext.nextTransition = 7;
    }
    if (self->warpTimer >= 141) {
        f32 screenFillAlpha;

        globalCtx->envCtx.fillScreen = true;
        screenFillAlpha = (f32)(self->warpTimer - 140) / 20.0f;

        if (screenFillAlpha > 1.0f) {
            screenFillAlpha = 1.0f;
        }
        globalCtx->envCtx.screenFillColor[0] = 160;
        globalCtx->envCtx.screenFillColor[1] = 160;
        globalCtx->envCtx.screenFillColor[2] = 160;
        globalCtx->envCtx.screenFillColor[3] = (u32)(255.0f * screenFillAlpha);

        osSyncPrintf("\nparcent=[%f]", screenFillAlpha);
    }
    Lights_PointNoGlowSetInfo(&self->upperLightInfo, (s16)player->actor.world.pos.x + 10.0f,
                              (s16)player->actor.world.pos.y + 10.0f, (s16)player->actor.world.pos.z + 10.0f, 235, 255,
                              255, 255);
    Lights_PointNoGlowSetInfo(&self->lowerLightInfo, (s16)player->actor.world.pos.x - 10.0f,
                              (s16)player->actor.world.pos.y - 10.0f, (s16)player->actor.world.pos.z - 10.0f, 235, 255,
                              255, 255);

    Math_SmoothStepToF(&self->actor.shape.yOffset, 800.0f, 0.5f, 15.0f, 0.1f);
    self->actor.shape.rot.y += 0x320;

    Math_SmoothStepToF(&self->unk_1BC, 1.13f, 0.2f, 0.1f, 0.01f);
    Math_StepToF(&self->unk_194, 2.0f, 0.003f);
    Math_StepToF(&self->unk_198, 10.0f, 0.006f);
    Math_SmoothStepToF(&self->lightRayAlpha, 0.0f, 0.2f, 3.0f, 0.01f);
    Math_SmoothStepToF(&self->warpAlpha, 0.0f, 0.2f, 2.0f, 0.01f);
    Math_SmoothStepToF(&self->crystalAlpha, 255.0f, 0.1f, 1.0f, 0.01f);

    temp_f0_2 = 1.0f - (f32)(sWarpTimerTarget - self->warpTimer) / (sWarpTimerTarget - (sWarpTimerTarget - 100));
    if (temp_f0_2 > 0.0f) {
        s16 i;

        for (i = 0; i < 3; i++) {
            globalCtx->envCtx.adjAmbientColor[i] = globalCtx->envCtx.adjFogColor[i] =
                globalCtx->envCtx.adjLight1Color[i] = -255.0f * temp_f0_2;
        }

        globalCtx->envCtx.adjFogNear = -500.0f * temp_f0_2;
        if (globalCtx->envCtx.adjFogNear < -300) {
            globalCtx->roomCtx.curRoom.segment = NULL;
        }
    }
}

void DoorWarp1_Destination(DoorWarp1* self, GlobalContext* globalCtx) {
    f32 alphaFrac;

    self->warpTimer++;
    self->unk_194 = 5.0f;

    alphaFrac = 1.0f;
    if (self->warpTimer < 20) {
        alphaFrac = self->warpTimer / 20.f;
    } else if (self->warpTimer >= 60) {
        alphaFrac = 1.0f - ((self->warpTimer - 60.0f) / 20.f);
    }
    self->warpAlpha = 255.0f * alphaFrac;
    self->lightRayAlpha = 0.0f;

    if (self->warpTimer >= 80.0f) {
        self->warpAlpha = 0.0f;
        DoorWarp1_SetupAction(self, DoorWarp1_DoNothing);
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
}

void DoorWarp1_DoNothing(DoorWarp1* self, GlobalContext* globalCtx) {
}

void func_8099B020(DoorWarp1* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->lightRayAlpha, 128.0f, 0.2f, 2.0f, 0.1f);
    Math_SmoothStepToF(&self->warpAlpha, 128.0f, 0.2f, 2.0f, 0.1f);

    if (self->lightRayAlpha >= 128.0f) {
        Math_StepToF(&self->unk_194, 2.0f, 0.01f);
        Math_StepToF(&self->unk_198, 10.0f, 0.02f);
    }
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
}

void DoorWarp1_Update(Actor* thisx, GlobalContext* globalCtx) {
    DoorWarp1* self = THIS;

    self->actionFunc(self, globalCtx);

    if (self->actor.params != WARP_PURPLE_CRYSTAL) {
        Actor_SetScale(&self->actor, self->scale / 100.0f);
    }
}

void DoorWarp1_DrawBlueCrystal(DoorWarp1* self, GlobalContext* globalCtx) {
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2078);

    func_80093D84(globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0xFF, 0xFF, 200, 255, 255, (u8)self->crystalAlpha);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, (u8)self->crystalAlpha);

    POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, NULL,
                                   &self->actor, POLY_XLU_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2098);

    SkelAnime_Update(&self->skelAnime);
}

void DoorWarp1_DrawPurpleCrystal(DoorWarp1* self, GlobalContext* globalCtx) {
    s32 pad[2];
    Vec3f eye;

    eye.x = -(Math_SinS(globalCtx->state.frames * 200) * 120.0f) * 80.0f;
    eye.y = (Math_CosS(globalCtx->state.frames * 200) * 120.0f) * 80.0f;
    eye.z = (Math_CosS(globalCtx->state.frames * 200) * 120.0f) * 80.0f;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2122);

    func_80093D84(globalCtx->state.gfxCtx);
    func_8002EB44(&self->actor.world.pos, &eye, &eye, globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (u8)self->crystalAlpha);
    gDPSetEnvColor(POLY_XLU_DISP++, 150, 0, 100, (u8)self->crystalAlpha);

    POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, NULL,
                                   &self->actor, POLY_XLU_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2152);

    SkelAnime_Update(&self->skelAnime);
}

void DoorWarp1_DrawWarp(DoorWarp1* self, GlobalContext* globalCtx) {
    s32 pad;
    u32 pad1;
    u32 spEC = globalCtx->state.frames * 10;
    f32 spE8 = (self->unk_194 >= 1.0f) ? 0.0f : 1.0f - self->unk_194;
    f32 spE4 = (self->unk_198 >= 1.0f) ? 0.0f : 1.0f - self->unk_198;
    f32 xzScale;
    f32 temp_f0;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2173);

    temp_f0 = 1.0f - (2.0f - self->unk_194) / 1.7f;
    if (self->actor.params != WARP_YELLOW && self->actor.params != WARP_DESTINATION &&
        self->actor.params != WARP_ORANGE && self->actor.params != WARP_GREEN && self->actor.params != WARP_RED) {
        self->unk_19C += (s16)(temp_f0 * 15.0f);
    }
    if (self->actor.params == WARP_DESTINATION) {
        self->unk_19C -= (s16)(temp_f0 * 2.0f);
    }
    func_80093D84(globalCtx->state.gfxCtx);

    switch (self->actor.params) {
        case WARP_YELLOW:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, (u8)self->warpAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 200, 255, 0, 255);
            break;
        case WARP_ORANGE:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, (u8)self->warpAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 150, 0, 255);
            break;
        case WARP_GREEN:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, (u8)self->warpAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 200, 0, 255);
            break;
        case WARP_RED:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, (u8)self->warpAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 50, 0, 255);
            break;
        default:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255 * temp_f0, 255, 255, (u8)self->warpAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 255 * temp_f0, 255, 255);
            break;
    }
    gDPSetColorDither(POLY_XLU_DISP++, G_CD_DISABLE);
    gDPSetColorDither(POLY_XLU_DISP++, G_AD_NOTPATTERN | G_CD_MAGICSQ);

    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y + 1.0f, self->actor.world.pos.z, MTXMODE_NEW);
    gSPSegment(POLY_XLU_DISP++, 0x0A, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2247));
    Matrix_Push();

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, spEC & 0xFF, -((s16)(self->unk_19C + self->unk_19C) & 511),
                                0x100, 0x100, 1, spEC & 0xFF, -((s16)(self->unk_19C + self->unk_19C) & 511), 0x100,
                                0x100));

    Matrix_Translate(0.0f, self->unk_194 * 230.0f, 0.0f, MTXMODE_APPLY);
    xzScale = (((f32)self->unk_1AE * spE8) / 100.0f) + 1.0f;
    Matrix_Scale(xzScale, 1.0f, xzScale, MTXMODE_APPLY);
    gSPSegment(POLY_XLU_DISP++, 0x09, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2267));
    gSPDisplayList(POLY_XLU_DISP++, gWarpPortalDL);
    Matrix_Pop();

    if (self->lightRayAlpha > 0.0f) {
        switch (self->actor.params) {
            case WARP_YELLOW:
                gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, (u8)self->warpAlpha);
                gDPSetEnvColor(POLY_XLU_DISP++, 200, 255, 0, 255);
                break;
            case WARP_ORANGE:
                gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, (u8)self->warpAlpha);
                gDPSetEnvColor(POLY_XLU_DISP++, 255, 150, 0, 255);
                break;
            case WARP_GREEN:
                gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, (u8)self->warpAlpha);
                gDPSetEnvColor(POLY_XLU_DISP++, 0, 200, 0, 255);
                break;
            case WARP_RED:
                gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 255, (u8)self->warpAlpha);
                gDPSetEnvColor(POLY_XLU_DISP++, 255, 50, 0, 255);
                break;
            default:
                gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255 * temp_f0, 255, 255, (u8)self->lightRayAlpha);
                gDPSetEnvColor(POLY_XLU_DISP++, 0, 255 * temp_f0, 255, 255);
                break;
        }
        spEC *= 2;

        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, spEC & 0xFF, -((s16)self->unk_19C & 511), 0x100, 0x100,
                                    1, spEC & 0xFF, -((s16)self->unk_19C & 511), 0x100, 0x100));

        Matrix_Translate(0.0f, self->unk_198 * 60.0f, 0.0f, MTXMODE_APPLY);

        xzScale = (((f32)self->unk_1B0 * spE4) / 100.0f) + 1.0f;
        Matrix_Scale(xzScale, 1.0f, xzScale, MTXMODE_APPLY);

        gSPSegment(POLY_XLU_DISP++, 0x09, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2336));
        gSPDisplayList(POLY_XLU_DISP++, gWarpPortalDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_door_warp1.c", 2340);
}

void DoorWarp1_Draw(Actor* thisx, GlobalContext* globalCtx) {
    DoorWarp1* self = THIS;

    switch (self->actor.params) {
        case WARP_DUNGEON_ADULT:
            DoorWarp1_DrawBlueCrystal(self, globalCtx);
            DoorWarp1_DrawWarp(self, globalCtx);
            break;
        case WARP_DUNGEON_CHILD:
        case WARP_CLEAR_FLAG:
        case WARP_SAGES:
        case WARP_YELLOW:
        case WARP_BLUE_RUTO:
        case WARP_DESTINATION:
        case WARP_UNK_7:
        case WARP_ORANGE:
        case WARP_GREEN:
        case WARP_RED:
            DoorWarp1_DrawWarp(self, globalCtx);
            break;
        case WARP_BLUE_CRYSTAL:
            DoorWarp1_DrawBlueCrystal(self, globalCtx);
            break;
        case WARP_PURPLE_CRYSTAL:
            DoorWarp1_DrawPurpleCrystal(self, globalCtx);
            break;
    }
}
