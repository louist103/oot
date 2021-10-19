#include "z_boss_dodongo.h"
#include "objects/object_kingdodongo/object_kingdodongo.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "scenes/dungeons/ddan_boss/ddan_boss_room_1.h"

#define FLAGS 0x00000035

#define THIS ((BossDodongo*)thisx)

void BossDodongo_Init(Actor* thisx, GlobalContext* globalCtx);
void BossDodongo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BossDodongo_Update(Actor* thisx, GlobalContext* globalCtx);
void BossDodongo_Draw(Actor* thisx, GlobalContext* globalCtx);

void BossDodongo_SetupIntroCutscene(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_IntroCutscene(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_Walk(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_Inhale(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_BlowFire(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_Roll(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_SpawnFire(BossDodongo* self, GlobalContext* globalCtx, s16 arg2);
void BossDodongo_Explode(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_LayDown(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_Vulnerable(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_GetUp(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_SetupWalk(BossDodongo* self);
void BossDodongo_DeathCutscene(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_SetupDeathCutscene(BossDodongo* self);
void BossDodongo_Damaged(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_UpdateDamage(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_PlayerPosCheck(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_PlayerYawCheck(BossDodongo* self, GlobalContext* globalCtx);
f32 func_808C4F6C(BossDodongo* self, GlobalContext* globalCtx);
f32 func_808C50A8(BossDodongo* self, GlobalContext* globalCtx);
void BossDodongo_DrawEffects(GlobalContext* globalCtx);
void BossDodongo_UpdateEffects(GlobalContext* globalCtx);

const ActorInit Boss_Dodongo_InitVars = {
    ACTOR_EN_DODONGO,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_KINGDODONGO,
    sizeof(BossDodongo),
    (ActorFunc)BossDodongo_Init,
    (ActorFunc)BossDodongo_Destroy,
    (ActorFunc)BossDodongo_Update,
    (ActorFunc)BossDodongo_Draw,
};

#include "z_boss_dodongo_data.c"

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_S8(naviEnemyId, 0x0C, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -3000.0f, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 8200.0f, ICHAIN_STOP),
};

void func_808C1190(s16* arg0, u8* arg1, s16 arg2) {
    if (arg2[arg1] != 0) {
        arg0[arg2 / 2] = 0;
    }
}

void func_808C11D0(s16* arg0, u8* arg1, s16 arg2) {
    if (arg1[arg2] != 0) {
        arg0[arg2] = 0;
    }
}

void func_808C1200(s16* arg0, u8* arg1, s16 arg2) {
    if (arg1[arg2] != 0) {
        arg0[arg2] = 0;
    }
}

void func_808C1230(s16* arg0, u8* arg1, s16 arg2) {
    s16 index;

    if (arg1[arg2] != 0) {
        index = ((arg2 & 0xF) + ((arg2 & 0xF0) * 2));
        arg0[index + 16] = 0;
        arg0[index] = 0;
    }
}

void func_808C1278(s16* arg0, u8* arg1, s16 arg2) {
    s16 index;

    if (arg1[arg2] != 0) {
        index = ((arg2 & 0xF) * 2) + ((arg2 & 0xF0) * 2);
        arg0[index + 1] = 0;
        arg0[index] = 0;
    }
}

void func_808C12C4(u8* arg1, s16 arg2) {
    func_808C1190(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_015890), arg1, arg2);
    func_808C1200(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_017210), arg1, arg2);
    func_808C11D0(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_015D90), arg1, arg2);
    func_808C11D0(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_016390), arg1, arg2);
    func_808C11D0(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_016590), arg1, arg2);
    func_808C11D0(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_016790), arg1, arg2);
    func_808C1230(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_015990), arg1, arg2);
    func_808C1230(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_015F90), arg1, arg2);
    func_808C1278(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_016990), arg1, arg2);
    func_808C1278(SEGMENTED_TO_VIRTUAL(object_kingdodongo_Tex_016E10), arg1, arg2);
}

void func_808C1554(void* arg0, void* floorTex, s32 arg2, f32 arg3) {
    u16* temp_s3 = SEGMENTED_TO_VIRTUAL(arg0);
    u16* temp_s1 = SEGMENTED_TO_VIRTUAL(floorTex);
    s16 i;
    s16 i2;
    u16 sp54[2048];
    s16 temp;
    s16 temp2;

    for (i = 0; i < 2048; i += 32) {
        temp = sinf((((i / 32) + (s16)((arg2 * 50.0f) / 100.0f)) & 0x1F) * (M_PI / 16)) * arg3;
        for (i2 = 0; i2 < 32; i2++) {
            sp54[i + ((temp + i2) & 0x1F)] = temp_s1[i + i2];
        }
    }
    for (i = 0; i < 32; i++) {
        temp = sinf(((i + (s16)((arg2 * 80.0f) / 100.0f)) & 0x1F) * (M_PI / 16)) * arg3;
        temp *= 32;
        for (i2 = 0; i2 < 2048; i2 += 32) {
            temp2 = (temp + i2) & 0x7FF;
            temp_s3[i + temp2] = sp54[i + i2];
        }
    }
}

void func_808C17C8(GlobalContext* globalCtx, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, f32 arg4, s16 arg5) {
    s16 i;
    BossDodongoEffect* eff = (BossDodongoEffect*)globalCtx->specialEffects;

    for (i = 0; i < arg5; i++, eff++) {
        if (eff->unk_24 == 0) {
            eff->unk_24 = 1;
            eff->unk_00 = *arg1;
            eff->unk_0C = *arg2;
            eff->unk_18 = *arg3;
            eff->unk_2C = arg4 / 1000.0f;
            eff->alpha = 255;
            eff->unk_25 = (s16)Rand_ZeroFloat(10.0f);
            break;
        }
    }
}

s32 BossDodongo_AteExplosive(BossDodongo* self, GlobalContext* globalCtx) {
    f32 dx;
    f32 dy;
    f32 dz;
    Actor* currentExplosive = globalCtx->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].head;
    Actor* thisx = &self->actor;

    while (currentExplosive != NULL) {
        if (currentExplosive == thisx) {
            currentExplosive = currentExplosive->next;
            continue;
        }

        dx = currentExplosive->world.pos.x - self->mouthPos.x;
        dy = currentExplosive->world.pos.y - self->mouthPos.y;
        dz = currentExplosive->world.pos.z - self->mouthPos.z;

        if ((fabsf(dx) < 40.0f) && (fabsf(dy) < 40.0f) && (fabsf(dz) < 40.0f)) {
            Actor_Kill(currentExplosive);
            return true;
        }

        currentExplosive = currentExplosive->next;
    }

    return false;
}

void BossDodongo_Init(Actor* thisx, GlobalContext* globalCtx) {
    BossDodongo* self = THIS;
    s16 i;
    u16* temp_s1_3;
    u16* temp_s2;
    u32 temp_v0;

    globalCtx->specialEffects = &self->effects;
    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 9200.0f, ActorShadow_DrawCircle, 250.0f);
    Actor_SetScale(&self->actor, 0.01f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &object_kingdodongo_Skel_01B310, &object_kingdodongo_Anim_00F0D8, NULL,
                   NULL, 0);
    Animation_PlayLoop(&self->skelAnime, &object_kingdodongo_Anim_00F0D8);
    self->unk_1F8 = 1.0f;
    BossDodongo_SetupIntroCutscene(self, globalCtx);
    self->health = 12;
    self->colorFilterMin = 995.0f;
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->colorFilterMax = 1000.0f;
    self->unk_224 = 2.0f;
    self->unk_228 = 9200.0f;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sJntSphInit, self->items);

    if (Flags_GetClear(globalCtx, globalCtx->roomCtx.curRoom.num)) { // KD is dead
        temp_s1_3 = SEGMENTED_TO_VIRTUAL(gDodongosCavernBossLavaFloorTex);
        temp_s2 = SEGMENTED_TO_VIRTUAL(sLavaFloorRockTex);

        Actor_Kill(&self->actor);
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, -890.0f, -1523.76f,
                           -3304.0f, 0, 0, 0, WARP_DUNGEON_CHILD);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_BG_BREAKWALL, -890.0f, -1523.76f, -3304.0f, 0, 0, 0, 0x6000);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART, -690.0f, -1523.76f, -3304.0f, 0, 0, 0, 0);

        for (i = 0; i < 2048; i++) {
            temp_v0 = i;
            temp_s1_3[temp_v0] = temp_s2[temp_v0];
        }
    }

    self->actor.flags &= ~1;
}

void BossDodongo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BossDodongo* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void BossDodongo_SetupIntroCutscene(BossDodongo* self, GlobalContext* globalCtx) {
    s16 frames = Animation_GetLastFrame(&object_kingdodongo_Anim_00F0D8);

    Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_00F0D8, 1.0f, 0.0f, frames, ANIMMODE_LOOP, -10.0f);
    self->actionFunc = BossDodongo_IntroCutscene;
    self->csState = 0;
    self->unk_1BC = 1;
}

void BossDodongo_IntroCutscene(BossDodongo* self, GlobalContext* globalCtx) {
    f32 phi_f0;
    Camera* camera;
    Player* player;
    Vec3f sp60;
    Vec3f sp54;
    Vec3f sp48;

    player = GET_PLAYER(globalCtx);
    camera = Gameplay_GetCamera(globalCtx, MAIN_CAM);

    if (self->unk_196 != 0) {
        self->unk_196--;
    }

    if (self->unk_198 != 0) {
        self->unk_198--;
    }

    if (self->unk_19A != 0) {
        self->unk_19A--;
    }

    switch (self->csState) {
        case 0:
            if (player->actor.world.pos.y < -1223.76f) {
                self->csState = 1;
                self->actor.world.pos.x = -1390.0f;
                self->actor.world.pos.z = -3374.0f;
                self->unk_1A0 = 1;
            }
            break;
        case 1:
            func_80064520(globalCtx, &globalCtx->csCtx);
            func_8002DF54(globalCtx, &self->actor, 1);
            Gameplay_ClearAllSubCameras(globalCtx);
            self->cutsceneCamera = Gameplay_CreateSubCamera(globalCtx);
            Gameplay_ChangeCameraStatus(globalCtx, 0, 1);
            Gameplay_ChangeCameraStatus(globalCtx, self->cutsceneCamera, 7);
            self->csState = 2;
            self->unk_196 = 0x3C;
            self->unk_198 = 160;
            player->actor.world.pos.y = -1023.76f;
            self->cameraEye.y = player->actor.world.pos.y - 480.0f + 50.0f;
        case 2:
            if (self->unk_198 >= 131) {
                player->actor.world.pos.x = -890.0f;
                player->actor.world.pos.z = -2804.0f;

                player->actor.speedXZ = 0.0f;
                player->actor.shape.rot.y = player->actor.world.rot.y = 0x3FFF;

                self->cameraEye.x = -890.0f;
                self->cameraEye.z = player->actor.world.pos.z - 100.0f;

                self->cameraAt.x = player->actor.world.pos.x;
                self->cameraAt.y = player->actor.world.pos.y + 20.0f;
                self->cameraAt.z = player->actor.world.pos.z;
            }

            if (self->unk_198 == 110) {
                func_8002DF54(globalCtx, &self->actor, 9);
            }

            if (self->unk_198 == 5) {
                func_8002DF54(globalCtx, &self->actor, 12);
            }

            if (self->unk_198 < 6) {
                player->actor.shape.rot.y = -0x4001;
            } else {
                player->actor.shape.rot.y = 0x3FFF;
            }

            if (self->unk_198 < 60) {
                self->unk_1BC = 1;
            } else {
                self->unk_1BC = 2;
            }

            BossDodongo_Walk(self, globalCtx);

            if (self->unk_196 == 1) {
                Audio_QueueSeqCmd(0x100100FF);
            }

            if (self->unk_196 == 0) {
                Math_SmoothStepToF(&self->cameraEye.x, self->vec.x + 30.0f, 0.2f, self->unk_204 * 20.0f, 0.0f);
                Math_SmoothStepToF(&self->cameraEye.y, self->vec.y, 0.2f, self->unk_204 * 20.0f, 0.0f);
                Math_SmoothStepToF(&self->cameraEye.z, self->vec.z + 10.0f, 0.2f, self->unk_204 * 20.0f, 0.0f);
                Math_SmoothStepToF(&self->unk_204, 1.0f, 1.0f, 0.02f, 0.0f);
            } else {
                self->cameraAt.x = player->actor.world.pos.x;
                self->cameraAt.y = player->actor.world.pos.y + 20.0f;
                self->cameraAt.z = player->actor.world.pos.z;
            }

            if (gSaveContext.eventChkInf[7] & 2) {
                if (self->unk_198 == 100) {
                    self->actor.world.pos.x = -1114.0f;
                    self->actor.world.pos.z = -2804.0f;
                    self->actor.world.rot.y = 0x3FFF;
                    self->unk_1A2 = 0;
                    self->unk_1A0 = 2;
                    self->csState = 4;
                    self->unk_196 = 30;
                    self->unk_198 = 150;
                    self->unk_204 = 0.0f;
                    Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_008EEC, 1.0f, 0.0f,
                                     Animation_GetLastFrame(&object_kingdodongo_Anim_008EEC), ANIMMODE_ONCE, 0.0f);
                    SkelAnime_Update(&self->skelAnime);
                }
            } else if (self->unk_198 == 0) {
                self->csState = 3;
                self->unk_19E = 0x14;
                self->unk_204 = 0.0f;
            }
            break;
        case 3:
            BossDodongo_Walk(self, globalCtx);
            Math_SmoothStepToF(&self->unk_20C, sinf(self->unk_19E * 0.05f) * 0.1f, 1.0f, 0.01f, 0.0f);
            Math_SmoothStepToF(&self->cameraEye.x, self->vec.x + 90.0f, 0.2f, self->unk_204 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraEye.y, self->vec.y + 50.0f, 0.2f, self->unk_204 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraEye.z, self->vec.z, 0.2f, self->unk_204 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraAt.y, self->vec.y - 10.0f, 0.2f, self->unk_204 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_204, 1.0f, 1.0f, 0.02f, 0.0f);
            if (fabsf(player->actor.world.pos.x - self->actor.world.pos.x) < 200.0f) {
                self->csState = 4;
                self->unk_196 = 0x1E;
                self->unk_198 = 0x96;
                self->unk_204 = 0.0f;
                Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_008EEC, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&object_kingdodongo_Anim_008EEC), ANIMMODE_ONCE, -5.0f);
            }
            break;
        case 4:
            Math_SmoothStepToF(&self->unk_20C, 0.0f, 1.0f, 0.01f, 0.0f);

            if (gSaveContext.eventChkInf[7] & 2) {
                phi_f0 = -50.0f;
            } else {
                phi_f0 = 0.0f;
            }

            Math_SmoothStepToF(&self->cameraEye.x, player->actor.world.pos.x + phi_f0 + 70.0f, 0.2f,
                               self->unk_204 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraEye.y, player->actor.world.pos.y + 10.0f, 0.2f, self->unk_204 * 20.0f,
                               0.0f);
            Math_SmoothStepToF(&self->cameraEye.z, player->actor.world.pos.z - 60.0f, 0.2f, self->unk_204 * 20.0f,
                               0.0f);

            Math_SmoothStepToF(&self->cameraAt.x, self->vec.x, 0.2f, self->unk_204 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraAt.y, self->vec.y, 0.2f, self->unk_204 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraAt.z, self->vec.z, 0.2f, self->unk_204 * 20.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_204, 1.0f, 1.0f, 0.02f, 0.0f);

            if (self->unk_196 == 0) {
                SkelAnime_Update(&self->skelAnime);
                Math_SmoothStepToF(&self->unk_208, 0.05f, 1.0f, 0.005f, 0.0f);
            }

            if (self->unk_198 == 0x64) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_OTAKEBI);
            }

            if (self->unk_198 == 0x5A) {
                if (!(gSaveContext.eventChkInf[7] & 2)) {
                    TitleCard_InitBossName(globalCtx, &globalCtx->actorCtx.titleCtx,
                                           SEGMENTED_TO_VIRTUAL(&object_kingdodongo_Blob_017410), 0xA0, 0xB4, 0x80,
                                           0x28);
                }
                Audio_QueueSeqCmd(0x6B);
            }

            if (self->unk_198 == 0) {
                camera->eye = self->cameraEye;
                camera->eyeNext = self->cameraEye;
                camera->at = self->cameraAt;
                func_800C08AC(globalCtx, self->cutsceneCamera, 0);
                self->cutsceneCamera = 0;
                func_80064534(globalCtx, &globalCtx->csCtx);
                func_8002DF54(globalCtx, &self->actor, 7);
                BossDodongo_SetupWalk(self);
                self->unk_1DA = 50;
                self->unk_1BC = 0;
                player->actor.shape.rot.y = -0x4002;
                gSaveContext.eventChkInf[7] |= 2;
            }
            break;
    }

    if (self->cutsceneCamera != 0) {
        if (self->unk_1B6 != 0) {
            self->unk_1B6--;
        }

        sp60.x = self->cameraEye.x;
        phi_f0 = sinf((self->unk_1B6 * 3.1415f * 90.0f) / 180.0f);
        sp60.y = (self->unk_1B6 * phi_f0 * 0.7f) + self->cameraEye.y;
        sp60.z = self->cameraEye.z;

        sp54.x = self->cameraAt.x;
        phi_f0 = sinf((self->unk_1B6 * 3.1415f * 90.0f) / 180.0f);
        sp54.y = (self->unk_1B6 * phi_f0 * 0.7f) + self->cameraAt.y;
        sp54.z = self->cameraAt.z;

        sp48.x = self->unk_20C;
        sp48.y = 1.0f;
        sp48.z = self->unk_20C;

        Gameplay_CameraSetAtEyeUp(globalCtx, self->cutsceneCamera, &sp54, &sp60, &sp48);
    }
}

void BossDodongo_SetupDamaged(BossDodongo* self) {
    if (self->actionFunc != BossDodongo_Damaged) {
        Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_001074, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_kingdodongo_Anim_001074), ANIMMODE_ONCE, -5.0f);
        self->actionFunc = BossDodongo_Damaged;
    }

    self->unk_1DA = 100;
}

void BossDodongo_SetupExplode(BossDodongo* self) {
    Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_00E848, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_kingdodongo_Anim_00E848), ANIMMODE_ONCE, -5.0f);
    self->actionFunc = BossDodongo_Explode;
    self->unk_1B0 = 10;
    self->unk_1C0 = 2;
    self->unk_1DA = 35;
    self->unk_1FC = 50.0f;
    self->unk_200 = 300.0f;
}

void BossDodongo_SetupWalk(BossDodongo* self) {
    Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_01D934, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_kingdodongo_Anim_01D934), ANIMMODE_ONCE, -10.0f);
    self->unk_1AA = 0;
    self->actionFunc = BossDodongo_Walk;
    self->unk_1DA = 0;
    self->actor.flags |= 1;
    self->unk_1E4 = 0.0f;
}

void BossDodongo_SetupRoll(BossDodongo* self) {
    Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_00DF38, 1.0f, 0.0f, 59.0f, ANIMMODE_ONCE, -5.0f);
    self->actionFunc = BossDodongo_Roll;
    self->numWallCollisions = 0;
    self->unk_1DA = 27;
}

void BossDodongo_SetupBlowFire(BossDodongo* self) {
    self->actor.speedXZ = 0.0f;
    self->unk_1E4 = 0.0f;
    Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_0061D4, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_kingdodongo_Anim_0061D4), ANIMMODE_ONCE, 0.0f);
    self->actionFunc = BossDodongo_BlowFire;
    self->unk_1DA = 50;
    self->unk_1AE = 0;
}

void BossDodongo_SetupInhale(BossDodongo* self) {
    self->actor.speedXZ = 0.0f;
    Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_008EEC, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_kingdodongo_Anim_008EEC), ANIMMODE_ONCE, -5.0f);
    self->actionFunc = BossDodongo_Inhale;
    self->unk_1DA = 100;
    self->unk_1AC = 0;
    self->unk_1E2 = 1;
}

void BossDodongo_Damaged(BossDodongo* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToF(&self->unk_1F8, 1.0f, 0.5f, 0.02f, 0.001f);
    Math_SmoothStepToF(&self->unk_208, 0.05f, 1.0f, 0.005f, 0.0f);

    if (Animation_OnFrame(&self->skelAnime, Animation_GetLastFrame(&object_kingdodongo_Anim_001074))) {
        BossDodongo_SetupRoll(self);
    }
}

void BossDodongo_Explode(BossDodongo* self, GlobalContext* globalCtx) {
    static Color_RGBA8 dustPrimColor = { 255, 255, 0, 255 };
    static Color_RGBA8 dustEnvColor = { 255, 10, 0, 255 };
    s16 pad;
    Vec3f dustVel;
    Vec3f dustAcell;
    Vec3f dustPos;
    s16 i;

    Math_SmoothStepToF(&self->unk_208, 0.05f, 1.0f, 0.005f, 0.0f);
    SkelAnime_Update(&self->skelAnime);

    if (self->unk_1DA == 0) {
        for (i = 0; i < 30; i++) {
            dustVel.x = Rand_CenteredFloat(20.0f);
            dustVel.y = Rand_CenteredFloat(20.0f);
            dustVel.z = Rand_CenteredFloat(20.0f);

            dustAcell.x = dustVel.x * -0.1f;
            dustAcell.y = dustVel.y * -0.1f;
            dustAcell.z = dustVel.z * -0.1f;

            dustPos.x = self->actor.world.pos.x + (dustVel.x * 3.0f);
            dustPos.y = self->actor.world.pos.y + 90.0f + (dustVel.y * 3.0f);
            dustPos.z = self->actor.world.pos.z + (dustVel.z * 3.0f);

            func_8002836C(globalCtx, &dustPos, &dustVel, &dustAcell, &dustPrimColor, &dustEnvColor, 500, 10, 10);
        }

        Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_004E0C, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_kingdodongo_Anim_004E0C), ANIMMODE_ONCE, -5.0f);
        self->actionFunc = BossDodongo_LayDown;
        Audio_PlayActorSound2(&self->actor, NA_SE_IT_BOMB_EXPLOSION);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_DAMAGE);
        func_80033E88(&self->actor, globalCtx, 4, 10);
        self->health -= 2;

        // make sure not to die from the bomb explosion
        if (self->health <= 0) {
            self->health = 1;
        }
    }
}

void BossDodongo_LayDown(BossDodongo* self, GlobalContext* globalCtx) {
    self->unk_1BE = 10;
    Math_SmoothStepToF(&self->unk_1F8, 1.3f, 1.0f, 0.1f, 0.001f);
    SkelAnime_Update(&self->skelAnime);

    if (Animation_OnFrame(&self->skelAnime, Animation_GetLastFrame(&object_kingdodongo_Anim_004E0C))) {
        Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_0042A8, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_kingdodongo_Anim_0042A8), ANIMMODE_LOOP, -5.0f);
        self->actionFunc = BossDodongo_Vulnerable;
        self->unk_1DA = 100;
    }
}

void BossDodongo_Vulnerable(BossDodongo* self, GlobalContext* globalCtx) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_DOWN - SFX_FLAG);
    self->unk_1BE = 10;
    Math_SmoothStepToF(&self->unk_1F8, 1.0f, 0.5f, 0.02f, 0.001f);
    Math_SmoothStepToF(&self->unk_208, 0.05f, 1.0f, 0.005f, 0.0f);
    SkelAnime_Update(&self->skelAnime);

    if (self->unk_1DA == 0) {
        Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_009D10, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_kingdodongo_Anim_009D10), ANIMMODE_ONCE, -5.0f);
        self->actionFunc = BossDodongo_GetUp;
    }
}

void BossDodongo_GetUp(BossDodongo* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (Animation_OnFrame(&self->skelAnime, Animation_GetLastFrame(&object_kingdodongo_Anim_009D10))) {
        BossDodongo_SetupRoll(self);
    }
}

void BossDodongo_BlowFire(BossDodongo* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f unusedZeroVec1 = { 0.0f, 0.0f, 0.0f };
    Vec3f unusedZeroVec2 = { 0.0f, 0.0f, 0.0f };

    SkelAnime_Update(&self->skelAnime);

    if (Animation_OnFrame(&self->skelAnime, 12.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_CRY);
    }

    if (Animation_OnFrame(&self->skelAnime, 17.0f)) {
        self->unk_1C8 = 28;
    }

    if ((self->skelAnime.curFrame > 17.0f) && (self->skelAnime.curFrame < 35.0f)) {
        BossDodongo_SpawnFire(self, globalCtx, self->unk_1AE);
        self->unk_1AE++;
        Math_SmoothStepToF(&self->unk_244, 0.0f, 1.0f, 8.0f, 0.0f);
    }

    if (self->unk_1DA == 0) {
        BossDodongo_SetupRoll(self);
    }
}

void BossDodongo_Inhale(BossDodongo* self, GlobalContext* GlobalContext) {
    self->unk_1E2 = 1;

    if (self->unk_1AC > 20) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_BREATH - SFX_FLAG);
    }

    Math_SmoothStepToF(&self->unk_208, 0.05f, 1.0f, 0.005f, 0.0f);
    SkelAnime_Update(&self->skelAnime);

    if (self->unk_1DA == 0) {
        BossDodongo_SetupBlowFire(self);
    } else {
        self->unk_1AC++;

        if ((self->unk_1AC > 20) && (self->unk_1AC < 82) && BossDodongo_AteExplosive(self, GlobalContext)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_DRINK);
            BossDodongo_SetupExplode(self);
        }
    }
}

static Vec3f sCornerPositions[] = {
    { -1390.0f, 0.0f, -3804.0f },
    { -1390.0f, 0.0f, -2804.0f },
    { -390.0f, 0.0f, -2804.0f },
    { -390.0f, 0.0f, -3804.0f },
};

void BossDodongo_Walk(BossDodongo* self, GlobalContext* globalCtx) {
    Vec3f* sp4C;
    f32 sp48;
    f32 sp44;

    if (self->unk_1AA == 0) {
        if (Animation_OnFrame(&self->skelAnime, 14.0f)) {
            Animation_PlayLoop(&self->skelAnime, &object_kingdodongo_Anim_01CAE0);
            self->unk_1AA = 1;
        }
    } else if (self->unk_1BC != 2) {
        if (((s32)self->skelAnime.curFrame == 1) || ((s32)self->skelAnime.curFrame == 31)) {
            if ((s32)self->skelAnime.curFrame == 1) {
                Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->unk_410, 25.0f, 0xA, 8.0f, 0x1F4, 0xA, 0);
            } else {
                Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->unk_404, 25.0f, 0xA, 8.0f, 0x1F4, 0xA, 0);
            }

            if (self->unk_1BC != 0) {
                func_80078884(NA_SE_EN_DODO_K_WALK);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_WALK);
            }

            if (self->cutsceneCamera == 0) {
                func_80033E88(&self->actor, globalCtx, 4, 10);
            } else {
                self->unk_1B6 = 10;
                func_800A9F6C(0.0f, 180, 20, 100);
            }
        }
    }

    SkelAnime_Update(&self->skelAnime);
    sp4C = &sCornerPositions[self->unk_1A0];
    self->unk_1EC = 0.7f;
    Math_SmoothStepToF(&self->unk_1E4, self->unk_1EC * 4.0f, 1.0f, self->unk_1EC * 0.25f, 0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.x, sp4C->x, 0.3f, self->unk_1E4, 0.0f);
    Math_SmoothStepToF(&self->actor.world.pos.z, sp4C->z, 0.3f, self->unk_1E4, 0.0f);
    sp48 = sp4C->x - self->actor.world.pos.x;
    sp44 = sp4C->z - self->actor.world.pos.z;
    Math_SmoothStepToF(&self->unk_1E8, 2000.0f, 1.0f, self->unk_1EC * 80.0f, 0.0f);
    Math_SmoothStepToS(&self->actor.world.rot.y, Math_FAtan2F(sp48, sp44) * 10430.378f, 5,
                       (self->unk_1EC * self->unk_1E8), 5);
    Math_SmoothStepToS(&self->unk_1C4, 0, 2, 2000, 0);

    if ((fabsf(sp48) <= 5.0f) && (fabsf(sp44) <= 5.0f)) {
        self->unk_1E8 = 0.0f;
        self->unk_1E4 = 0.0f;
        if (self->unk_1A2 == 0) {
            self->unk_1A0++;
            if (self->unk_1A0 >= 4) {
                self->unk_1A0 = 0;
            }
        } else {
            self->unk_1A0--;
            if (self->unk_1A0 < 0) {
                self->unk_1A0 = 3;
            }
        }
    }

    if ((self->unk_1DA == 0) && (self->unk_1BC == 0)) {
        if ((self->actor.xzDistToPlayer < 500.0f) && (self->unk_1A4 != 0) && !self->playerPosInRange) {
            BossDodongo_SetupInhale(self);
            BossDodongo_SpawnFire(self, globalCtx, -1);
        }

        if (!self->playerPosInRange && !self->playerYawInRange) {
            BossDodongo_SetupRoll(self);
        }
    }
}

void BossDodongo_Roll(BossDodongo* self, GlobalContext* globalCtx) {
    Vec3f* sp5C;
    Vec3f sp50;
    f32 sp4C;
    f32 sp48;

    self->actor.flags |= 0x1000000;
    SkelAnime_Update(&self->skelAnime);

    if (self->unk_1DA == 10) {
        self->actor.velocity.y = 15.0f;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_CRY);
    }

    if (self->unk_1DA == 1) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_COLI2);
    }

    sp5C = &sCornerPositions[self->unk_1A0];
    self->unk_1EC = 3.0f;

    if (self->unk_1DA == 0) {
        Math_SmoothStepToF(&self->unk_1E4, self->unk_1EC * 5.0f, 1.0f, self->unk_1EC * 0.25f, 0.0f);
        Math_SmoothStepToF(&self->actor.world.pos.x, sp5C->x, 1.0f, self->unk_1E4, 0.0f);
        Math_SmoothStepToF(&self->actor.world.pos.z, sp5C->z, 1.0f, self->unk_1E4, 0.0f);
        self->unk_1C4 += 2000;

        if (self->actor.bgCheckFlags & 1) {
            self->unk_228 = 7700.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_ROLL - SFX_FLAG);

            if ((self->unk_19E & 7) == 0) {
                Camera_AddQuake(&globalCtx->mainCamera, 2, 1, 8);
            }

            if (!(self->unk_19E & 1)) {
                Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 40.0f, 3, 8.0f, 0x1F4, 0xA,
                                         0);
            }
        }
    }

    sp4C = sp5C->x - self->actor.world.pos.x;
    sp48 = sp5C->z - self->actor.world.pos.z;
    Math_SmoothStepToF(&self->unk_1E8, 2000.0f, 1.0f, self->unk_1EC * 100.0f, 0.0f);
    Math_SmoothStepToS(&self->actor.world.rot.y, Math_FAtan2F(sp4C, sp48) * 10430.378f, 5,
                       self->unk_1EC * self->unk_1E8, 0);

    if (fabsf(sp4C) <= 15.0f && fabsf(sp48) <= 15.0f) {
        self->numWallCollisions++;

        if (self->numWallCollisions >= 2) {
            if (self->unk_1A6 != 0) {
                self->unk_1A2 = 1 - self->unk_1A2;
            }

            self->unk_1E8 = 0.0f;
            self->unk_1E4 = 0.0f;
            BossDodongo_SetupWalk(self);
            self->unk_228 = 9200.0f;
            self->actor.velocity.y = 20.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_COLI);
            Camera_AddQuake(&globalCtx->mainCamera, 2, 6, 8);
            sp50.x = self->actor.world.pos.x;
            sp50.y = self->actor.world.pos.y + 60.0f;
            sp50.z = self->actor.world.pos.z;
            func_80033480(globalCtx, &sp50, 250.0f, 40, 800, 10, 0);
            func_80033E88(&self->actor, globalCtx, 6, 15);
        } else {
            self->actor.velocity.y = 15.0f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_COLI2);
        }

        if (self->unk_1A2 == 0) {
            self->unk_1A0++;
            if (self->unk_1A0 >= 4) {
                self->unk_1A0 = 0;
            }
        } else {
            self->unk_1A0--;
            if (self->unk_1A0 < 0) {
                self->unk_1A0 = 3;
            }
        }
    }
}

void BossDodongo_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BossDodongo* self = THIS;
    f32 temp_f0;
    s16 i;
    Player* player = GET_PLAYER(globalCtx);
    Player* player2 = GET_PLAYER(globalCtx);
    s32 pad;

    self->unk_1E2 = 0;
    self->unk_19E++;

    if (self->unk_1DA != 0) {
        self->unk_1DA--;
    }

    if (self->unk_1DC != 0) {
        self->unk_1DC--;
    }

    if (self->unk_1DE != 0) {
        self->unk_1DE--;
    }

    if (self->unk_1C0 != 0) {
        self->unk_1C0--;
    }

    if (self->unk_1C8 != 0) {
        self->unk_1C8--;
    }

    temp_f0 = func_808C4F6C(self, globalCtx);

    if (temp_f0 > 0.0f) {
        self->unk_1A4 = temp_f0;
    } else {
        self->unk_1A4 = 0;
    }

    temp_f0 = func_808C50A8(self, globalCtx);

    if (temp_f0 > 0.0f) {
        self->unk_1A6 = temp_f0;
    } else {
        self->unk_1A6 = 0;
    }

    BossDodongo_PlayerYawCheck(self, globalCtx);
    BossDodongo_PlayerPosCheck(self, globalCtx);

    self->actionFunc(self, globalCtx);

    thisx->shape.rot.y = thisx->world.rot.y;

    Math_SmoothStepToF(&thisx->shape.yOffset, self->unk_228, 1.0f, 100.0f, 0.0f);
    Actor_MoveForward(thisx);
    BossDodongo_UpdateDamage(self, globalCtx);
    Actor_UpdateBgCheckInfo(globalCtx, thisx, 10.0f, 10.0f, 20.0f, 4);
    Math_SmoothStepToF(&self->unk_208, 0, 1, 0.001f, 0.0);
    Math_SmoothStepToF(&self->unk_20C, 0, 1, 0.001f, 0.0);

    if ((self->unk_19E % 128) == 0) {
        for (i = 0; i < 50; i++) {
            self->unk_324[i] = (Rand_ZeroOne() * 0.25f) + 0.5f;
        }
    }

    for (i = 0; i < 50; i++) {
        self->unk_25C[i] += self->unk_324[i];
    }

    if (self->unk_1C8 != 0) {
        if (self->unk_1C8 >= 11) {
            Math_SmoothStepToF(&self->unk_240, (self->unk_1C8 & 1) ? (40.0f) : (60.0f), 1.0f, 50.0f, 0.0f);
        } else {
            Math_SmoothStepToF(&self->unk_240, 0.0f, 1, 10.0f, 0.0);
        }

        if ((globalCtx->envCtx.adjLight1Color[2] == 0) && (globalCtx->envCtx.adjAmbientColor[2] == 0)) {
            globalCtx->envCtx.adjLight1Color[0] = (u8)self->unk_240;
            globalCtx->envCtx.adjLight1Color[1] = (u8)(self->unk_240 * 0.1f);
            globalCtx->envCtx.adjAmbientColor[0] = (u8)self->unk_240;
            globalCtx->envCtx.adjAmbientColor[1] = (u8)(self->unk_240 * 0.1f);
        }
    }

    if (self->unk_1BE != 0) {
        if (self->unk_1BE >= 1000) {
            Math_SmoothStepToF(&self->colorFilterR, 30.0f, 1, 20.0f, 0.0);
            Math_SmoothStepToF(&self->colorFilterG, 10.0f, 1, 20.0f, 0.0);
        } else {
            self->unk_1BE--;
            Math_SmoothStepToF(&self->colorFilterR, 255.0f, 1, 20.0f, 0.0);
            Math_SmoothStepToF(&self->colorFilterG, 0.0f, 1, 20.0f, 0.0);
        }

        Math_SmoothStepToF(&self->colorFilterB, 0.0f, 1, 20.0f, 0.0);
        Math_SmoothStepToF(&self->colorFilterMin, 900.0f, 1, 10.0f, 0.0);
        Math_SmoothStepToF(&self->colorFilterMax, 1099.0f, 1, 10.0f, 0.0);
    } else {
        Math_SmoothStepToF(&self->colorFilterR, globalCtx->lightCtx.fogColor[0], 1, 5.0f, 0.0);
        Math_SmoothStepToF(&self->colorFilterG, globalCtx->lightCtx.fogColor[1], 1.0f, 5.0f, 0.0);
        Math_SmoothStepToF(&self->colorFilterB, globalCtx->lightCtx.fogColor[2], 1.0f, 5.0f, 0.0);
        Math_SmoothStepToF(&self->colorFilterMin, globalCtx->lightCtx.fogNear, 1.0, 5.0f, 0.0);
        Math_SmoothStepToF(&self->colorFilterMax, 1000.0f, 1, 5.0f, 0.0);
    }

    if (player->actor.world.pos.y < -1000.0f) {
        s16 phi_s0_3;
        s16 sp90;
        s16 magma2DrawMode;
        s16 magmaScale = 0;

        if (self->unk_224 > 1.9f) {
            phi_s0_3 = 1;
            magma2DrawMode = 0;
            sp90 = 0;
        } else if (self->unk_224 > 1.7f) {
            phi_s0_3 = 3;
            sp90 = 1;
            if (globalCtx) {}
            magma2DrawMode = 0;
        } else if (self->unk_224 > 1.4f) {
            phi_s0_3 = 7;
            sp90 = 3;
            magma2DrawMode = Rand_ZeroOne() * 1.9f;
        } else if (self->unk_224 > 1.1f) {
            phi_s0_3 = 7;
            sp90 = 4095;
            magma2DrawMode = Rand_ZeroOne() * 1.9f;
        } else {
            phi_s0_3 = 1;
            sp90 = -1;
            magma2DrawMode = 1;
            magmaScale = ((s16)(Rand_ZeroOne() * 50)) - 50;
        }

        if (player2->csMode >= 10) {
            phi_s0_3 = -1;
        }

        if ((self->unk_19E & phi_s0_3) == 0) {
            static Color_RGBA8 magmaPrimColor[] = { { 255, 255, 0, 255 }, { 0, 0, 0, 150 } };
            static Color_RGBA8 magmaEnvColor[] = { { 255, 0, 0, 255 }, { 0, 0, 0, 0 } };
            Vec3f sp84;
            f32 temp_f12;
            f32 temp_f10;

            temp_f12 = Rand_ZeroOne() * 330.0f;
            temp_f10 = Rand_ZeroOne() * 6.28f;
            sp84.x = (sinf(temp_f10) * temp_f12) + (-890.0f);
            sp84.y = -1523.76f;
            sp84.z = (cosf(temp_f10) * temp_f12) + (-3304.0f);
            EffectSsGMagma2_Spawn(globalCtx, &sp84, &magmaPrimColor[magma2DrawMode], &magmaEnvColor[magma2DrawMode],
                                  10 - (magma2DrawMode * 5), magma2DrawMode, magmaScale + 100);
        }

        if ((self->unk_19E & sp90) == 0) {
            Vec3f sp6C = { 0.0f, 0.0f, 0.0f };
            Vec3f sp60 = { 0.0f, 0.0f, 0.0f };
            Vec3f sp54;
            f32 sp50 = Rand_ZeroOne() * 330.0f;
            f32 sp4C = Rand_ZeroOne() * 6.28f;

            sp54.x = sinf(sp4C) * sp50 + (-890.0f);
            sp54.y = -1523.76f;
            sp54.z = cosf(sp4C) * sp50 + (-3304.0f);
            EffectSsGMagma_Spawn(globalCtx, &sp54);
            for (i = 0; i < 4; i++) {
                sp60.y = 0.4f;
                sp60.x = Rand_CenteredFloat(0.5f);
                sp60.z = Rand_CenteredFloat(0.5f);
                sp50 = Rand_ZeroOne() * 330.0f;
                sp4C = Rand_ZeroOne() * 6.28f;
                sp54.x = sinf(sp4C) * sp50 + (-890.0f);
                sp54.y = -1513.76f;
                sp54.z = cosf(sp4C) * sp50 + (-3304.0f);
                func_808C17C8(globalCtx, &sp54, &sp6C, &sp60, ((s16)Rand_ZeroFloat(2.0f)) + 6, 0x50);
            }
        }

        func_808C1554(gDodongosCavernBossLavaFloorTex, sLavaFloorLavaTex, self->unk_19E, self->unk_224);
    }

    if (self->unk_1C6 != 0) {
        u16* ptr1 = SEGMENTED_TO_VIRTUAL(sLavaFloorLavaTex);
        u16* ptr2 = SEGMENTED_TO_VIRTUAL(sLavaFloorRockTex);
        s16 i2;

        for (i2 = 0; i2 < 20; i2++) {
            s16 new_var = self->unk_1C2 & 0x7FF;

            ptr1[new_var] = ptr2[new_var];
            self->unk_1C2 += 37;
        }
        Math_SmoothStepToF(&self->unk_224, 0.0f, 1.0f, 0.01f, 0.0f);
    }

    if (self->unk_1BC == 0) {
        if (self->actionFunc != BossDodongo_DeathCutscene) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }

        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

        if (self->actionFunc == BossDodongo_Roll) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }

    self->collider.elements[0].dim.scale = (self->actionFunc == BossDodongo_Inhale) ? 0.0f : 1.0f;

    for (i = 6; i < 19; i++) {
        if (i != 12) {
            self->collider.elements[i].dim.scale = (self->actionFunc == BossDodongo_Roll) ? 0.0f : 1.0f;
        }
    }

    if (self->unk_244 != 0) {
        MREG(64) = 1;
        MREG(65) = 255;
        MREG(66) = 80;
        MREG(67) = 0;
        MREG(68) = (u8)self->unk_244;
    } else {
        MREG(64) = 0;
    }

    Math_SmoothStepToF(&self->unk_244, 0.0f, 1.0f, 2.0f, 0.0f);
    BossDodongo_UpdateEffects(globalCtx);
}

s32 BossDodongo_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                 void* thisx) {
    f32 mtxScaleY;
    f32 mtxScaleZ;
    BossDodongo* self = THIS;
    // required for matching
    if ((limbIndex == 6) || (limbIndex == 7)) {
        if (self->unk_25C) {}
        goto block_1;
    }
block_1:
    Matrix_JointPosition(pos, rot);

    if (*dList != NULL) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_dodongo.c", 3787);

        mtxScaleZ = 1.0f;
        mtxScaleY = 1.0f;

        if ((limbIndex == 33) || (limbIndex == 48)) {
            mtxScaleY = mtxScaleZ = self->unk_1F8;
        }

        Matrix_Push();
        Matrix_Scale(1.0f, mtxScaleY, mtxScaleZ, MTXMODE_APPLY);

        if ((limbIndex != 6) && (limbIndex != 7)) {
            Matrix_RotateX(self->unk_25C[limbIndex] * 0.115f, MTXMODE_APPLY);
            Matrix_RotateY(self->unk_25C[limbIndex] * 0.13f, MTXMODE_APPLY);
            Matrix_RotateZ(self->unk_25C[limbIndex] * 0.1f, MTXMODE_APPLY);
            Matrix_Scale(1.0f - self->unk_208, self->unk_208 + 1.0f, 1.0f - self->unk_208, MTXMODE_APPLY);
            Matrix_RotateZ(-(self->unk_25C[limbIndex] * 0.1f), MTXMODE_APPLY);
            Matrix_RotateY(-(self->unk_25C[limbIndex] * 0.13f), MTXMODE_APPLY);
            Matrix_RotateX(-(self->unk_25C[limbIndex] * 0.115f), MTXMODE_APPLY);
        }

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_boss_dodongo.c", 3822),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, *dList);
        Matrix_Pop();

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_dodongo.c", 3826);
    }
    { s32 pad; } // Required to match
    return 1;
}

void BossDodongo_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Vec3f D_808CA450 = { 5000.0f, -2500.0f, 0.0f };
    static Vec3f D_808CA45C = { 0.0f, 0.0f, 0.0f };
    static Vec3f D_808CA468 = { 11500.0f, -3000.0f, 0.0f };
    static Vec3f D_808CA474 = { 5000.0f, -2000.0f, 0.0f };
    static Vec3f D_808CA480 = { 8000.0f, 0.0f, 0.0f };
    static Vec3f D_808CA48C = { 8000.0f, 0.0f, 0.0f };
    BossDodongo* self = THIS;

    if (limbIndex == 6) {
        Matrix_MultVec3f(&D_808CA45C, &self->vec);
        Matrix_MultVec3f(&D_808CA450, &self->actor.focus.pos);
        Matrix_MultVec3f(&D_808CA468, &self->firePos);
        Matrix_MultVec3f(&D_808CA474, &self->mouthPos);
    } else if (limbIndex == 39) {
        Matrix_MultVec3f(&D_808CA480, &self->unk_410);
    } else if (limbIndex == 46) {
        Matrix_MultVec3f(&D_808CA48C, &self->unk_404);
    }
    Collider_UpdateSpheres(limbIndex, &self->collider);
}

void BossDodongo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BossDodongo* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_boss_dodongo.c", 3922);
    func_80093D18(globalCtx->state.gfxCtx);

    if ((self->unk_1C0 >= 2) && (self->unk_1C0 & 1)) {
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, 255, 255, 255, 0, 900, 1099);
    } else {
        POLY_OPA_DISP = Gfx_SetFog(POLY_OPA_DISP, (u32)self->colorFilterR, (u32)self->colorFilterG,
                                   (u32)self->colorFilterB, 0, self->colorFilterMin, self->colorFilterMax);
    }

    Matrix_RotateZ(self->unk_23C, MTXMODE_APPLY);
    Matrix_RotateX((self->unk_1C4 / 32768.0f) * 3.14159f, MTXMODE_APPLY);

    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, BossDodongo_OverrideLimbDraw,
                      BossDodongo_PostLimbDraw, self);

    POLY_OPA_DISP = Gameplay_SetFog(globalCtx, POLY_OPA_DISP);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_boss_dodongo.c", 3981);

    BossDodongo_DrawEffects(globalCtx);
}

f32 func_808C4F6C(BossDodongo* self, GlobalContext* globalCtx) {
    f32 xDiff;
    f32 zDiff;
    f32 sp2C;
    s32 pad;
    f32 temp_f2;
    f32 rotation;
    Player* player = GET_PLAYER(globalCtx);

    xDiff = player->actor.world.pos.x - self->actor.world.pos.x;
    zDiff = player->actor.world.pos.z - self->actor.world.pos.z;

    rotation = Math_CosS(-self->actor.world.rot.y);
    sp2C = (Math_SinS(-self->actor.world.rot.y) * zDiff) + (rotation * xDiff);
    rotation = Math_SinS(-self->actor.world.rot.y);
    temp_f2 = (Math_CosS(-self->actor.world.rot.y) * zDiff) + (-rotation * xDiff);

    if ((fabsf(sp2C) < 150.0f) && (temp_f2 >= 100.0f) && (temp_f2 <= 2000.0f)) {
        return temp_f2;
    }
    return -1.0f;
}

f32 func_808C50A8(BossDodongo* self, GlobalContext* globalCtx) {
    f32 xDiff;
    f32 zDiff;
    f32 sp2C;
    s32 pad;
    f32 temp_f2;
    f32 rotation;
    Player* player = GET_PLAYER(globalCtx);

    xDiff = player->actor.world.pos.x - self->actor.world.pos.x;
    zDiff = player->actor.world.pos.z - self->actor.world.pos.z;

    rotation = Math_CosS(-0x8000 - self->actor.world.rot.y);
    sp2C = (Math_SinS(-0x8000 - self->actor.world.rot.y) * zDiff) + (rotation * xDiff);
    rotation = Math_SinS(-0x8000 - self->actor.world.rot.y);
    temp_f2 = (Math_CosS(-0x8000 - self->actor.world.rot.y) * zDiff) + (-rotation * xDiff);

    if ((fabsf(sp2C) < 150.0f) && (100.0f <= temp_f2) && (temp_f2 <= 2000.0f)) {
        return temp_f2;
    }

    return -1.0f;
}

void BossDodongo_PlayerYawCheck(BossDodongo* self, GlobalContext* globalCtx) {
    s16 yawDiff = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.world.rot.y;

    if ((yawDiff < 0x38E3) && (-0x38E3 < yawDiff)) {
        self->playerYawInRange = true;
    } else {
        self->playerYawInRange = false;
    }
}

void BossDodongo_PlayerPosCheck(BossDodongo* self, GlobalContext* globalCtx) {
    Vec3f* temp_v1;
    s16 i;

    self->playerPosInRange = false;

    for (i = 0; i < 4; i++) {
        temp_v1 = &sCornerPositions[i];

        if ((fabsf(self->actor.world.pos.x - temp_v1->x) < 200.0f) &&
            (fabsf(self->actor.world.pos.z - temp_v1->z) < 200.0f)) {
            self->playerPosInRange = true;
            break;
        }
    }
}

void BossDodongo_SpawnFire(BossDodongo* self, GlobalContext* globalCtx, s16 params) {
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_BDFIRE, self->vec.x, self->vec.y - 20.0f,
                       self->vec.z, 0, self->actor.shape.rot.y, 0, params);
}

void BossDodongo_UpdateDamage(BossDodongo* self, GlobalContext* globalCtx) {
    s32 pad;
    ColliderInfo* item1;
    u8 swordDamage;
    s32 damage;
    ColliderInfo* item2;
    s16 i;

    if ((self->health <= 0) && (self->actionFunc != BossDodongo_DeathCutscene)) {
        BossDodongo_SetupDeathCutscene(self);
        Enemy_StartFinishingBlow(globalCtx, &self->actor);
        return;
    }

    if (self->unk_1C0 == 0) {
        if (self->actionFunc == BossDodongo_Inhale) {
            for (i = 0; i < 19; i++) {
                if (self->collider.elements[i].info.bumperFlags & 2) {
                    item1 = self->collider.elements[i].info.acHitInfo;
                    item2 = item1;

                    if ((item2->toucher.dmgFlags & 0x10) || (item2->toucher.dmgFlags & 4)) {
                        self->collider.elements[i].info.bumperFlags &= ~2;
                        self->unk_1C0 = 2;
                        BossDodongo_SetupWalk(self);
                        self->unk_1DA = 0x32;
                        return;
                    }
                }
            }
        }

        if (self->collider.elements->info.bumperFlags & 2) {
            self->collider.elements->info.bumperFlags &= ~2;
            item1 = self->collider.elements[0].info.acHitInfo;
            if ((self->actionFunc == BossDodongo_Vulnerable) || (self->actionFunc == BossDodongo_LayDown)) {
                swordDamage = damage = CollisionCheck_GetSwordDamage(item1->toucher.dmgFlags);

                if (damage != 0) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_DAMAGE);
                    BossDodongo_SetupDamaged(self);
                    self->unk_1C0 = 5;
                    self->health -= swordDamage;
                }
            }
        }
    }
}

void BossDodongo_SetupDeathCutscene(BossDodongo* self) {
    self->actor.speedXZ = 0.0f;
    self->unk_1E4 = 0.0f;
    Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_002D0C, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_kingdodongo_Anim_002D0C), ANIMMODE_ONCE, -5.0f);
    self->actionFunc = BossDodongo_DeathCutscene;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_DEAD);
    self->unk_1DA = 0;
    self->csState = 0;
    self->actor.flags &= ~5;
    self->unk_1BC = 1;
    Audio_QueueSeqCmd(0x100100FF);
}

void BossDodongo_DeathCutscene(BossDodongo* self, GlobalContext* globalCtx) {
    Vec3f* cornerPos;
    Vec3f sp198;
    Vec3f sp184;
    f32 tempSin;
    f32 tempCos;
    f32 sp178;
    s16 i;
    Vec3f effectPos;
    Camera* camera;
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);

    switch (self->csState) {
        case 0:
            self->csState = 5;
            func_80064520(globalCtx, &globalCtx->csCtx);
            func_8002DF54(globalCtx, &self->actor, 1);
            self->cutsceneCamera = Gameplay_CreateSubCamera(globalCtx);
            Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_UNK3);
            Gameplay_ChangeCameraStatus(globalCtx, self->cutsceneCamera, CAM_STAT_ACTIVE);
            camera = Gameplay_GetCamera(globalCtx, MAIN_CAM);
            self->cameraEye.x = camera->eye.x;
            self->cameraEye.y = camera->eye.y;
            self->cameraEye.z = camera->eye.z;
            self->cameraAt.x = camera->at.x;
            self->cameraAt.y = camera->at.y;
            self->cameraAt.z = camera->at.z;
            break;
        case 5:
            tempSin = Math_SinS(self->actor.shape.rot.y - 0x1388) * 150.0f;
            tempCos = Math_CosS(self->actor.shape.rot.y - 0x1388) * 150.0f;
            Math_SmoothStepToF(&player->actor.world.pos.x, self->actor.world.pos.x + tempSin, 0.5f, 5.0f, 0.0f);
            Math_SmoothStepToF(&player->actor.world.pos.z, self->actor.world.pos.z + tempCos, 0.5f, 5.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_208, 0.07f, 1.0f, 0.005f, 0.0f);
            tempSin = Math_SinS(self->actor.world.rot.y) * 230.0f;
            tempCos = Math_CosS(self->actor.world.rot.y) * 230.0f;
            Math_SmoothStepToF(&self->cameraEye.x, self->actor.world.pos.x + tempSin, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->cameraEye.y, self->actor.world.pos.y + 20.0f, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->cameraEye.z, self->actor.world.pos.z + tempCos, 0.2f, 50.0f, 0.1f);
            Math_SmoothStepToF(&self->cameraAt.x, self->actor.world.pos.x, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->cameraAt.y, self->actor.focus.pos.y - 70.0f, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->cameraAt.z, self->actor.world.pos.z, 0.2f, 30.0f, 0.1f);
            if (Animation_OnFrame(&self->skelAnime, Animation_GetLastFrame(&object_kingdodongo_Anim_002D0C))) {
                Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_003CF8, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&object_kingdodongo_Anim_003CF8), ANIMMODE_ONCE, -1.0f);
                self->csState = 6;
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_BG_BREAKWALL, -890.0f, -1523.76f, -3304.0f, 0, 0, 0,
                            0x6000);
            }
            break;
        case 6:
            Math_SmoothStepToF(&self->cameraAt.x, self->actor.world.pos.x, 0.2f, 30.0f, 0.1f);
            Math_SmoothStepToF(&self->cameraAt.y, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.1f);
            Math_SmoothStepToF(&self->cameraAt.z, self->actor.world.pos.z, 0.2f, 30.0f, 0.1f);

            if (Animation_OnFrame(&self->skelAnime, Animation_GetLastFrame(&object_kingdodongo_Anim_003CF8))) {
                Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_00DF38, 1.0f, 30.0f, 59.0f, ANIMMODE_ONCE,
                                 -1.0f);
                self->csState = 7;
                self->unk_228 = 7700.0f;
                self->unk_204 = 0.0f;
                self->unk_1E4 = 0.0f;
                self->numWallCollisions = 0;
                self->unk_19E = 0;
            }
            break;
        case 7:
            self->unk_1C4 += 0x7D0;
            Math_SmoothStepToF(&self->cameraAt.x, self->actor.world.pos.x, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraAt.y, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraAt.z, self->actor.world.pos.z, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraEye.x, -890.0f, 0.1f, self->unk_204 * 5.0f, 0.1f);
            Math_SmoothStepToF(&self->cameraEye.z, -3304.0f, 0.1f, self->unk_204 * 5.0f, 0.1f);
            Math_SmoothStepToF(&self->unk_204, 1.0f, 1.0f, 0.1f, 0.0f);
            if (self->unk_1DA == 1) {
                self->csState = 8;
                self->actor.speedXZ = self->unk_1E4 / 1.5f;
                if (self->unk_1A2 == 0) {
                    self->unk_238 = 250.0f;
                } else {
                    self->unk_238 = -250.0f;
                }
                self->unk_1DA = 1000;
                self->unk_234 = 2000.0f;
            } else {
                cornerPos = &sCornerPositions[self->unk_1A0];
                self->unk_1EC = 3.0f;
                Math_SmoothStepToF(&self->unk_1E4, self->unk_1EC * 5.0f, 1.0f, self->unk_1EC * 0.25f, 0.0f);
                tempSin = cornerPos->x - self->actor.world.pos.x;
                tempCos = cornerPos->z - self->actor.world.pos.z;
                sp178 = sqrtf(SQ(tempSin) + SQ(tempCos)) - 200.0f;
                if ((sqrtf(SQ(tempSin) + SQ(tempCos)) < 200.0f) || (self->unk_1DA != 0)) {
                    sp178 = 0.0f;
                }
                sp178 = CLAMP_MAX(sp178, 70.0f);
                self->unk_23C = (Math_SinS(self->unk_19E * 1000) * -50.0f) / 100.0f;

                sp198.x = Math_SinS(self->unk_19E * 1000) * sp178;
                sp198.y = sp198.z = 0.0f;

                Matrix_RotateY(self->actor.shape.rot.y * (M_PI / 0x8000), MTXMODE_NEW);
                Matrix_MultVec3f(&sp198, &sp184);

                Math_SmoothStepToF(&self->actor.world.pos.x, cornerPos->x + sp184.x, 1.0f, self->unk_1E4, 0.0f);
                Math_SmoothStepToF(&self->actor.world.pos.z, cornerPos->z + sp184.z, 1.0f, self->unk_1E4, 0.0f);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_ROLL - SFX_FLAG);
                if ((self->unk_19E & 7) == 0) {
                    Camera_AddQuake(&globalCtx->mainCamera, 2, 1, 8);
                }
                if (!(self->unk_19E & 1)) {
                    Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 40.0f, 3, 8.0f, 0x1F4,
                                             0xA, 0);
                }
                tempSin = cornerPos->x - self->actor.world.pos.x;
                tempCos = cornerPos->z - self->actor.world.pos.z;
                Math_SmoothStepToF(&self->unk_1E8, 1500.0f, 1.0f, self->unk_1EC * 100.0f, 0.0f);
                Math_SmoothStepToS(&self->actor.world.rot.y, (Math_FAtan2F(tempSin, tempCos) * 10430.378f), 5,
                                   (self->unk_1EC * self->unk_1E8), 0);

                if ((fabsf(tempSin) <= 15.0f) && (fabsf(tempCos) <= 15.0f)) {
                    Vec3f dustPos;

                    self->actor.velocity.y = 15.0f;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_COLI2);
                    if (self->unk_1A2 == 0) {
                        self->unk_1A0 = self->unk_1A0 + 1;
                        if (self->unk_1A0 >= 4) {
                            self->unk_1A0 = 0;
                        }
                    } else {
                        self->unk_1A0--;
                        if (self->unk_1A0 < 0) {
                            self->unk_1A0 = 3;
                        }
                    }
                    self->unk_1DA = 0xA;
                    dustPos.x = self->actor.world.pos.x;
                    dustPos.y = self->actor.world.pos.y + 60.0f;
                    dustPos.z = self->actor.world.pos.z;
                    func_80033480(globalCtx, &dustPos, 250.0f, 0x28, 0x320, 0xA, 0);
                }
            }
            break;
        case 8:
        case 9:
            if (self->unk_1DA == 884) {
                Animation_Change(&self->skelAnime, &object_kingdodongo_Anim_0042A8, 1.0f, 0.0f,
                                 (f32)Animation_GetLastFrame(&object_kingdodongo_Anim_0042A8), ANIMMODE_LOOP, -20.0f);
                tempSin = self->cameraEye.x - self->actor.world.pos.x;
                tempCos = self->cameraEye.z - self->actor.world.pos.z;
                self->unk_22C = sqrtf(SQ(tempSin) + SQ(tempCos));
                self->unk_230 = Math_FAtan2F(tempSin, tempCos);
                self->unk_1DC = 350;
                self->csState = 9;
            }
            if (self->unk_1DA < 854) {
                for (i = 0; i < 2; i++) {
                    func_808C12C4(D_808C7000, self->unk_1CC);
                    if (self->unk_1CC < 256) {
                        self->unk_1CC++;
                    }
                }
            }
            if (self->unk_1DA < 984) {
                Math_SmoothStepToS(&self->unk_1C4, -0x4000, 0xA, 0x12C, 0);
            }
            if (self->unk_1DA == 904) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_END);
            }
            if (self->unk_1DA < 854) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_LAST - SFX_FLAG);
            }
            if (self->unk_1DA == 960) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_LAVA);
            }
            if (self->unk_1DA < 960) {
                Math_SmoothStepToF(&self->actor.shape.shadowScale, 0.0f, 1.0f, 10.0f, 0.0f);
                if (self->unk_1DA >= 710) {

                    if (self->unk_1DA == 710) {
                        Vec3f sp124[] = {
                            { -440.0f, 0.0f, -3304.0f },
                            { -890.0f, 0.0f, -3754.0f },
                            { -1340.0f, 0.0f, -3304.0f },
                            { -890.0f, 0.0f, -2854.0f },
                        };
                        Vec3f spF4[] = {
                            { -890.0f, 0.0f, -2854.0f },
                            { -440.0f, 0.0f, -3304.0f },
                            { -890.0f, 0.0f, -3754.0f },
                            { -1340.0f, 0.0f, -3304.0f },
                        };
                        Vec3f* phi_v0_2;

                        self->unk_1C6 = 1;
                        if (self->unk_1A2 == 0) {
                            phi_v0_2 = &sp124[self->unk_1A0];
                        } else {
                            phi_v0_2 = &spF4[self->unk_1A0];
                        }
                        player->actor.world.pos.x = phi_v0_2->x;
                        player->actor.world.pos.z = phi_v0_2->z;
                        self->unk_204 = 0.0f;
                    }
                    if (self->unk_1DA >= 885) {
                        Math_SmoothStepToF(&self->unk_228, 200.0, 0.2f, 100.0f, 0.0f);
                    } else {
                        Math_SmoothStepToF(&self->unk_228, -6600.0f, 0.2f, 30.0f, 0.0f);
                    }
                    {
                        static Vec3f dustVel = { 0.0f, 0.0f, 0.0f };
                        static Vec3f dustAcell = { 0.0f, 1.0f, 0.0f };
                        static Color_RGBA8 dustPrimColor = { 255, 255, 100, 255 };
                        static Color_RGBA8 dustEnvColor = { 255, 100, 0, 255 };
                        s16 colorIndex;
                        Color_RGBA8 magmaPrimColor2[] = { { 255, 255, 0, 255 }, { 0, 0, 0, 100 } };
                        Color_RGBA8 magmaEnvColor2[] = { { 255, 0, 0, 255 }, { 0, 0, 0, 0 } };

                        effectPos.x = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.x;
                        effectPos.y = Rand_ZeroFloat(50.0f) + self->actor.world.pos.y;
                        effectPos.z = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.z;
                        func_8002836C(globalCtx, &effectPos, &dustVel, &dustAcell, &dustPrimColor, &dustEnvColor, 0x1F4,
                                      0xA, 0xA);
                        effectPos.x = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.x;
                        effectPos.y = -1498.76f;
                        effectPos.z = Rand_CenteredFloat(120.0f) + self->actor.focus.pos.z;
                        colorIndex = (Rand_ZeroOne() * 1.9f);
                        EffectSsGMagma2_Spawn(globalCtx, &effectPos, &magmaPrimColor2[colorIndex],
                                              &magmaEnvColor2[colorIndex], 10 - (colorIndex * 5), colorIndex,
                                              (s16)(Rand_ZeroOne() * 100.0f) + 100);
                    }
                }
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_ROLL - SFX_FLAG);
                if (!(self->unk_19E & 1)) {
                    Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, 40.0f, 3, 8.0f, 0x1F4,
                                             0xA, 0);
                }
            }
            Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.2f, 0.1f, 0.0f);
            self->actor.world.rot.y += (s16)self->unk_238;
            self->unk_1C4 += (s16)self->unk_234;
            if (self->unk_1DA >= 0x367) {
                if (self->unk_1A2 == 0) {
                    if (self->unk_238 < 450.0f) {
                        self->unk_238 += 10.0f;
                    }
                } else if (-450.0f < self->unk_238) {
                    self->unk_238 -= 10.0f;
                }
            } else {
                Math_SmoothStepToF(&self->unk_238, 0.0f, 0.05f, 40.0f, 0.0f);
            }
            Math_SmoothStepToF(&self->unk_234, 0.0f, 0.2f, 17.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraAt.x, self->actor.world.pos.x, 0.2f, 30.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraAt.y, (self->actor.world.pos.y - 70.0f) + 130.0f, 0.2f, 20.0f, 0.0f);
            Math_SmoothStepToF(&self->cameraAt.z, self->actor.world.pos.z, 0.2f, 30.0f, 0.0f);
            if (self->csState == 9) {
                if (self->unk_1DA < 0x2C6) {
                    Vec3f spAC[] = { { -390.0f, 0.0f, -3304.0f },
                                     { -890.0f, 0.0f, -3804.0f },
                                     { -1390.0f, 0.0f, -3304.0f },
                                     { -890.0f, 0.0f, -2804.0f } };

                    Vec3f sp7C[] = { { -890.0f, 0.0f, -2804.0f },
                                     { -390.0f, 0.0f, -3304.0f },
                                     { -890.0f, 0.0f, -3804.0f },
                                     { -1390.0f, 0.0f, -3304.0f } };
                    Vec3f* sp78;
                    s32 pad74;

                    if (self->unk_1A2 == 0) {
                        sp78 = &spAC[self->unk_1A0];
                    } else {
                        sp78 = &sp7C[self->unk_1A0];
                    }

                    Math_SmoothStepToF(&self->cameraEye.x, sp78->x, 0.2f, self->unk_204 * 20.0f, 0.0f);
                    Math_SmoothStepToF(&self->cameraEye.y, player->actor.world.pos.y + 30.0f, 0.1f,
                                       self->unk_204 * 20.0f, 0.0f);
                    Math_SmoothStepToF(&self->cameraEye.z, sp78->z, 0.1f, self->unk_204 * 20.0f, 0.0f);
                    Math_SmoothStepToF(&self->unk_204, 1.0f, 1.0f, 0.02f, 0.0f);
                } else {
                    if (self->unk_1A2 == 0) {
                        self->unk_230 += 0.01f;
                    } else {
                        self->unk_230 -= 0.01f;
                    }
                    Math_SmoothStepToF(&self->unk_22C, 220.0f, 0.1f, 5.0f, 0.1f);
                    tempSin = sinf(self->unk_230) * (*self).unk_22C;
                    tempCos = cosf(self->unk_230) * (*self).unk_22C;
                    Math_SmoothStepToF(&self->cameraEye.x, self->actor.world.pos.x + tempSin, 0.2f, 50.0f, 0.0f);
                    Math_SmoothStepToF(&self->cameraEye.y, self->actor.world.pos.y + 20.0f, 0.2f, 50.0f, 0.0f);
                    Math_SmoothStepToF(&self->cameraEye.z, self->actor.world.pos.z + tempCos, 0.2f, 50.0f, 0.0f);
                    Math_SmoothStepToF(&self->unk_23C, 0.0f, 0.2f, 0.01f, 0.0f);
                }
            } else {

                if (self->unk_1A2 == 0) {
                    Math_SmoothStepToF(&self->unk_23C, -0.5f, 0.2f, 0.05f, 0.0f);
                } else {
                    Math_SmoothStepToF(&self->unk_23C, 0.5f, 0.2f, 0.05f, 0.0f);
                }

                Math_SmoothStepToF(&self->cameraEye.x, -890.0f, 0.1f, self->unk_204 * 5.0f, 0.1f);
                Math_SmoothStepToF(&self->cameraEye.z, -3304.0f, 0.1f, self->unk_204 * 5.0f, 0.1f);
                Math_SmoothStepToF(&self->unk_204, 1.0f, 1.0f, 0.05f, 0.0f);
            }

            if (self->unk_1DA == 820) {
                Audio_QueueSeqCmd(0x21);
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_B_HEART,
                            Math_SinS(self->actor.shape.rot.y) * -50.0f + self->actor.world.pos.x,
                            self->actor.world.pos.y,
                            Math_CosS(self->actor.shape.rot.y) * -50.0f + self->actor.world.pos.z, 0, 0, 0, 0);
            }
            if (self->unk_1DA == 600) {
                camera = Gameplay_GetCamera(globalCtx, MAIN_CAM);
                camera->eye = self->cameraEye;
                camera->eyeNext = self->cameraEye;
                camera->at = self->cameraAt;
                func_800C08AC(globalCtx, self->cutsceneCamera, 0);
                self->unk_1BC = 0;
                self->cutsceneCamera = MAIN_CAM;
                self->csState = 100;
                Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
                func_80064534(globalCtx, &globalCtx->csCtx);
                func_8002DF54(globalCtx, &self->actor, 7);
                Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DOOR_WARP1, -890.0f, -1523.76f,
                                   -3304.0f, 0, 0, 0, WARP_DUNGEON_CHILD);
                self->skelAnime.playSpeed = 0.0f;
                Flags_SetClear(globalCtx, globalCtx->roomCtx.curRoom.num);
            }
        case 100:
            if ((self->unk_1DA < 0x2C6) && (Rand_ZeroOne() < 0.5f)) {
                Vec3f sp68;
                Color_RGBA8 D_808CA568 = { 0, 0, 0, 100 };
                Color_RGBA8 D_808CA56C = { 0, 0, 0, 0 };

                sp68.x = Rand_CenteredFloat(60.0f) + self->actor.focus.pos.x;
                sp68.y = (Rand_ZeroOne() * 50.0f) + -1498.76f;
                sp68.z = Rand_CenteredFloat(60.0f) + self->actor.focus.pos.z;
                EffectSsGMagma2_Spawn(globalCtx, &sp68, &D_808CA568, &D_808CA56C, 5, 1,
                                      (s16)(Rand_ZeroOne() * 50.0f) + 50);
            }
            break;
    }
    if (self->cutsceneCamera != MAIN_CAM) {
        Gameplay_CameraSetAtEye(globalCtx, self->cutsceneCamera, &self->cameraAt, &self->cameraEye);
    }
}

void BossDodongo_UpdateEffects(GlobalContext* globalCtx) {
    BossDodongoEffect* eff = (BossDodongoEffect*)globalCtx->specialEffects;
    Color_RGB8 effectColors[] = { { 255, 128, 0 }, { 255, 0, 0 }, { 255, 255, 0 }, { 255, 0, 0 } };
    s16 colorIndex;
    s16 i;

    for (i = 0; i < 80; i++, eff++) {
        if (eff->unk_24 != 0) {
            eff->unk_00.x += eff->unk_0C.x;
            eff->unk_00.y += eff->unk_0C.y;
            eff->unk_00.z += eff->unk_0C.z;
            eff->unk_25++;
            eff->unk_0C.x += eff->unk_18.x;
            eff->unk_0C.y += eff->unk_18.y;
            eff->unk_0C.z += eff->unk_18.z;
            if (eff->unk_24 == 1) {
                colorIndex = eff->unk_25 % 4;
                eff->color.r = effectColors[colorIndex].r;
                eff->color.g = effectColors[colorIndex].g;
                eff->color.b = effectColors[colorIndex].b;
                eff->alpha -= 20;
                if (eff->alpha <= 0) {
                    eff->alpha = 0;
                    eff->unk_24 = 0;
                }
            }
        }
    }
}

void BossDodongo_DrawEffects(GlobalContext* globalCtx) {
    MtxF* unkMtx;
    s16 i;
    u8 phi_s3 = 0;
    BossDodongoEffect* eff;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;

    eff = (BossDodongoEffect*)globalCtx->specialEffects;

    OPEN_DISPS(gfxCtx, "../z_boss_dodongo.c", 5228);

    func_80093D84(globalCtx->state.gfxCtx);
    unkMtx = &globalCtx->mf_11DA0;

    for (i = 0; i < 80; i++, eff++) {
        if (eff->unk_24 == 1) {
            gDPPipeSync(POLY_XLU_DISP++);

            if (phi_s3 == 0) {
                gSPDisplayList(POLY_XLU_DISP++, object_kingdodongo_DL_009D50);
                phi_s3++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, eff->color.r, eff->color.g, eff->color.b, eff->alpha);
            Matrix_Translate(eff->unk_00.x, eff->unk_00.y, eff->unk_00.z, MTXMODE_NEW);
            func_800D1FD4(unkMtx);
            Matrix_Scale(eff->unk_2C, eff->unk_2C, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_boss_dodongo.c", 5253),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_kingdodongo_DL_009DD0);
        }
    }

    CLOSE_DISPS(gfxCtx, "../z_boss_dodongo.c", 5258);
}
