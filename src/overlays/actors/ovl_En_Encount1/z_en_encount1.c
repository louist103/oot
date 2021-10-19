#include "z_en_encount1.h"
#include "vt.h"
#include "overlays/actors/ovl_En_Tite/z_en_tite.h"

#define FLAGS 0x08000010

#define THIS ((EnEncount1*)thisx)

void EnEncount1_Init(Actor* thisx, GlobalContext* globalCtx);
void EnEncount1_Update(Actor* thisx, GlobalContext* globalCtx);

void EnEncount1_SpawnLeevers(EnEncount1* self, GlobalContext* globalCtx);
void EnEncount1_SpawnTektites(EnEncount1* self, GlobalContext* globalCtx);
void EnEncount1_SpawnStalchildOrWolfos(EnEncount1* self, GlobalContext* globalCtx);

static s16 sLeeverAngles[] = { 0x0000, 0x2710, 0x7148, 0x8EB8, 0xD8F0 };
static f32 sLeeverDists[] = { 200.0f, 170.0f, 120.0f, 120.0f, 170.0f };

const ActorInit En_Encount1_InitVars = {
    ACTOR_EN_ENCOUNT1,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnEncount1),
    (ActorFunc)EnEncount1_Init,
    NULL,
    (ActorFunc)EnEncount1_Update,
    NULL,
};

void EnEncount1_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnEncount1* self = THIS;
    f32 spawnRange;

    if (self->actor.params <= 0) {
        osSyncPrintf("\n\n");
        // "Input error death!"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 入力エラーデッス！ ☆☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 入力エラーデッス！ ☆☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 入力エラーデッス！ ☆☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 入力エラーデッス！ ☆☆☆☆☆ \n" VT_RST);
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 入力エラーデッス！ ☆☆☆☆☆ \n" VT_RST);
        osSyncPrintf("\n\n");
        Actor_Kill(&self->actor);
        return;
    }

    self->spawnType = (self->actor.params >> 0xB) & 0x1F;
    self->maxCurSpawns = (self->actor.params >> 6) & 0x1F;
    self->maxTotalSpawns = self->actor.params & 0x3F;
    self->curNumSpawn = self->totalNumSpawn = 0;
    spawnRange = 120.0f + (40.0f * self->actor.world.rot.z);
    self->spawnRange = spawnRange;

    osSyncPrintf("\n\n");
    // "It's an enemy spawner!"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 敵発生ゾーンでた！ ☆☆☆☆☆ %x\n" VT_RST, self->actor.params);
    // "Type"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 種類\t\t   ☆☆☆☆☆ %d\n" VT_RST, self->spawnType);
    // "Maximum number of simultaneous spawns"
    osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆☆ 最大同時発生数     ☆☆☆☆☆ %d\n" VT_RST, self->maxCurSpawns);
    // "Maximum number of spawns"
    osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 最大発生数  \t   ☆☆☆☆☆ %d\n" VT_RST, self->maxTotalSpawns);
    // "Spawn check range"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生チェック範囲   ☆☆☆☆☆ %f\n" VT_RST, self->spawnRange);
    osSyncPrintf("\n\n");

    self->actor.flags &= ~1;
    switch (self->spawnType) {
        case SPAWNER_LEEVER:
            self->timer = 30;
            self->maxCurSpawns = 5;
            if (globalCtx->sceneNum == SCENE_SPOT13) { // Haunted Wasteland
                self->reduceLeevers = true;
                self->maxCurSpawns = 3;
            }
            self->updateFunc = EnEncount1_SpawnLeevers;
            break;
        case SPAWNER_TEKTITE:
            self->maxCurSpawns = 2;
            self->updateFunc = EnEncount1_SpawnTektites;
            break;
        case SPAWNER_STALCHILDREN:
        case SPAWNER_WOLFOS:
            if (globalCtx->sceneNum == SCENE_SPOT00) { // Hyrule Field
                self->maxTotalSpawns = 10000;
            }
            self->updateFunc = EnEncount1_SpawnStalchildOrWolfos;
            break;
    }
}

void EnEncount1_SpawnLeevers(EnEncount1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 floorType;
    f32 spawnDist;
    s32 spawnParams;
    s16 spawnLimit;
    s16 spawnAngle;
    Vec3f spawnPos;
    CollisionPoly* floorPoly;
    s32 bgId;
    f32 floorY;
    EnReeba* leever;

    self->outOfRangeTimer = 0;
    spawnPos = self->actor.world.pos;

    if ((self->timer == 0) && (globalCtx->csCtx.state == CS_STATE_IDLE) && (self->curNumSpawn <= self->maxCurSpawns) &&
        (self->curNumSpawn < 5)) {
        floorType = func_80041D4C(&globalCtx->colCtx, player->actor.floorPoly, player->actor.floorBgId);
        if ((floorType != 4) && (floorType != 7) && (floorType != 12)) {
            self->numLeeverSpawns = 0;
        } else if (!(self->reduceLeevers && (self->actor.xzDistToPlayer > 1300.0f))) {
            spawnLimit = 5;
            if (self->reduceLeevers) {
                spawnLimit = 3;
            }
            while ((self->curNumSpawn < self->maxCurSpawns) && (self->curNumSpawn < spawnLimit) && (self->timer == 0)) {
                spawnDist = sLeeverDists[self->leeverIndex];
                spawnAngle = sLeeverAngles[self->leeverIndex] + player->actor.shape.rot.y;
                spawnParams = LEEVER_SMALL;

                if ((self->killCount >= 10) && (self->bigLeever == NULL)) {
                    self->killCount = self->numLeeverSpawns = 0;
                    spawnAngle = sLeeverAngles[0];
                    spawnDist = sLeeverDists[2];
                    spawnParams = LEEVER_BIG;
                }

                spawnPos.x = player->actor.world.pos.x + Math_SinS(spawnAngle) * spawnDist;
                spawnPos.y = player->actor.floorHeight + 120.0f;
                spawnPos.z = player->actor.world.pos.z + Math_CosS(spawnAngle) * spawnDist;

                floorY = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &floorPoly, &bgId, &self->actor, &spawnPos);
                if (floorY <= BGCHECK_Y_MIN) {
                    break;
                }
                spawnPos.y = floorY;

                leever = (EnReeba*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_REEBA,
                                                      spawnPos.x, spawnPos.y, spawnPos.z, 0, 0, 0, spawnParams);

                if (1) {}
                if (1) {}
                if (leever != NULL) {
                    self->curNumSpawn++;
                    leever->unk_280 = self->leeverIndex++;
                    if (self->leeverIndex >= 5) {
                        self->leeverIndex = 0;
                    }
                    self->numLeeverSpawns++;
                    if (self->numLeeverSpawns >= 12) {
                        self->timer = 150;
                        self->numLeeverSpawns = 0;
                    }
                    if (spawnParams != LEEVER_SMALL) {
                        self->timer = 300;
                        self->bigLeever = leever;
                    }
                    if (!self->reduceLeevers) {
                        self->maxCurSpawns = (s16)Rand_ZeroFloat(3.99f) + 2;
                    } else {
                        self->maxCurSpawns = (s16)Rand_ZeroFloat(2.99f) + 1;
                    }
                } else {
                    // "Cannot spawn!"
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                    break;
                }
            }
        }
    }
}

void EnEncount1_SpawnTektites(EnEncount1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 bgId;
    CollisionPoly* floorPoly;
    Vec3f spawnPos;
    f32 floorY;

    if (self->timer == 0) {
        self->timer = 10;
        if ((fabsf(player->actor.world.pos.y - self->actor.world.pos.y) > 100.0f) ||
            (self->actor.xzDistToPlayer > self->spawnRange)) {
            self->outOfRangeTimer++;
        } else {
            self->outOfRangeTimer = 0;
            if ((self->curNumSpawn < self->maxCurSpawns) && (self->totalNumSpawn < self->maxTotalSpawns)) {
                spawnPos.x = self->actor.world.pos.x + Rand_CenteredFloat(50.0f);
                spawnPos.y = self->actor.world.pos.y + 120.0f;
                spawnPos.z = self->actor.world.pos.z + Rand_CenteredFloat(50.0f);
                floorY = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &floorPoly, &bgId, &self->actor, &spawnPos);
                if (floorY <= BGCHECK_Y_MIN) {
                    return;
                }
                spawnPos.y = floorY;
                if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_TITE, spawnPos.x,
                                       spawnPos.y, spawnPos.z, 0, 0, 0, TEKTITE_RED) != NULL) { // Red tektite
                    self->curNumSpawn++;
                    self->totalNumSpawn++;
                } else {
                    // "Cannot spawn!"
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                }
            }
        }
    }
}

void EnEncount1_SpawnStalchildOrWolfos(EnEncount1* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 spawnDist;
    s16 spawnAngle;
    s16 spawnId;
    s16 spawnParams;
    s16 kcOver10;
    s16 tempmod;
    Vec3f spawnPos;
    CollisionPoly* floorPoly;
    s32 bgId;
    f32 floorY;

    if (globalCtx->sceneNum != SCENE_SPOT00) {
        if ((fabsf(player->actor.world.pos.y - self->actor.world.pos.y) > 100.0f) ||
            (self->actor.xzDistToPlayer > self->spawnRange)) {
            self->outOfRangeTimer++;
            return;
        }
    } else if (IS_DAY || (Player_GetMask(globalCtx) == PLAYER_MASK_BUNNY)) {
        self->killCount = 0;
        return;
    }

    self->outOfRangeTimer = 0;
    spawnPos = self->actor.world.pos;
    if ((self->curNumSpawn < self->maxCurSpawns) && (self->totalNumSpawn < self->maxTotalSpawns)) {
        while ((self->curNumSpawn < self->maxCurSpawns) && (self->totalNumSpawn < self->maxTotalSpawns)) {
            if (globalCtx->sceneNum == SCENE_SPOT00) {
                if ((player->unk_89E == 0) || (player->actor.floorBgId != BGCHECK_SCENE) ||
                    !(player->actor.bgCheckFlags & 1) || (player->stateFlags1 & 0x08000000)) {

                    self->fieldSpawnTimer = 60;
                    break;
                }
                if (self->fieldSpawnTimer == 60) {
                    self->maxCurSpawns = 2;
                }
                if (self->fieldSpawnTimer != 0) {
                    self->fieldSpawnTimer--;
                    break;
                }

                spawnDist = Rand_CenteredFloat(40.0f) + 200.0f;
                spawnAngle = player->actor.shape.rot.y;
                if (self->curNumSpawn != 0) {
                    spawnAngle = -spawnAngle;
                    spawnDist = Rand_CenteredFloat(40.0f) + 100.0f;
                }
                spawnPos.x =
                    player->actor.world.pos.x + (Math_SinS(spawnAngle) * spawnDist) + Rand_CenteredFloat(40.0f);
                spawnPos.y = player->actor.floorHeight + 120.0f;
                spawnPos.z =
                    player->actor.world.pos.z + (Math_CosS(spawnAngle) * spawnDist) + Rand_CenteredFloat(40.0f);
                floorY = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &floorPoly, &bgId, &self->actor, &spawnPos);
                if (floorY <= BGCHECK_Y_MIN) {
                    break;
                }
                if ((player->actor.yDistToWater != BGCHECK_Y_MIN) &&
                    (floorY < (player->actor.world.pos.y - player->actor.yDistToWater))) {
                    break;
                }
                spawnPos.y = floorY;
            }
            if (self->spawnType == SPAWNER_WOLFOS) {
                spawnId = ACTOR_EN_WF;
                spawnParams = (0xFF << 8) | 0x00;
            } else {
                spawnId = ACTOR_EN_SKB;
                spawnParams = 0;

                kcOver10 = self->killCount / 10;
                if (kcOver10 > 0) {
                    tempmod = self->killCount % 10;
                    if (tempmod == 0) {
                        spawnParams = kcOver10 * 5;
                    }
                }
                self->killCount++;
            }
            if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, spawnId, spawnPos.x, spawnPos.y,
                                   spawnPos.z, 0, 0, 0, spawnParams) != NULL) {
                self->curNumSpawn++;
                if (self->curNumSpawn >= self->maxCurSpawns) {
                    self->fieldSpawnTimer = 100;
                }
                if (globalCtx->sceneNum != SCENE_SPOT00) {
                    self->totalNumSpawn++;
                }
            } else {
                // "Cannot spawn!"
                osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 発生できません！ ☆☆☆☆☆\n" VT_RST);
                break;
            }
        }
    }
}

void EnEncount1_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnEncount1* self = THIS;

    if (self->timer != 0) {
        self->timer--;
    }

    self->updateFunc(self, globalCtx);

    if (BREG(0) != 0) {
        if (self->outOfRangeTimer != 0) {
            if ((self->outOfRangeTimer & 1) == 0) {
                DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                       self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 120, 120, 120, 255, 4, globalCtx->state.gfxCtx);
            }
        } else {
            DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                                   self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                   1.0f, 1.0f, 255, 0, 255, 255, 4, globalCtx->state.gfxCtx);
        }
    }
}
