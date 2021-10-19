/*
 * File: z_obj_oshihiki.c
 * Overlay: ovl_Obj_Oshihiki
 * Description: Push Block
 */

#include "z_obj_oshihiki.h"
#include "overlays/actors/ovl_Obj_Switch/z_obj_switch.h"
#include "objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"

#define FLAGS 0x00000010

#define THIS ((ObjOshihiki*)thisx)

void ObjOshihiki_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjOshihiki_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjOshihiki_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjOshihiki_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjOshihiki_SetupOnScene(ObjOshihiki* self, GlobalContext* globalCtx);
void ObjOshihiki_OnScene(ObjOshihiki* self, GlobalContext* globalCtx);
void ObjOshihiki_SetupOnActor(ObjOshihiki* self, GlobalContext* globalCtx);
void ObjOshihiki_OnActor(ObjOshihiki* self, GlobalContext* globalCtx);
void ObjOshihiki_SetupPush(ObjOshihiki* self, GlobalContext* globalCtx);
void ObjOshihiki_Push(ObjOshihiki* self, GlobalContext* globalCtx);
void ObjOshihiki_SetupFall(ObjOshihiki* self, GlobalContext* globalCtx);
void ObjOshihiki_Fall(ObjOshihiki* self, GlobalContext* globalCtx);

const ActorInit Obj_Oshihiki_InitVars = {
    ACTOR_OBJ_OSHIHIKI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(ObjOshihiki),
    (ActorFunc)ObjOshihiki_Init,
    (ActorFunc)ObjOshihiki_Destroy,
    (ActorFunc)ObjOshihiki_Update,
    (ActorFunc)ObjOshihiki_Draw,
};

static f32 sScales[] = {
    (1 / 10.0f), (1 / 6.0f), (1 / 5.0f), (1 / 3.0f), (1 / 10.0f), (1 / 6.0f), (1 / 5.0f), (1 / 3.0f),
};

static Color_RGB8 sColors[][4] = {
    { { 110, 86, 40 }, { 110, 86, 40 }, { 110, 86, 40 }, { 110, 86, 40 } },         // deku tree
    { { 106, 120, 110 }, { 104, 80, 20 }, { 0, 0, 0 }, { 0, 0, 0 } },               // dodongos cavern
    { { 142, 99, 86 }, { 72, 118, 96 }, { 0, 0, 0 }, { 0, 0, 0 } },                 // forest temple
    { { 210, 150, 80 }, { 210, 170, 80 }, { 0, 0, 0 }, { 0, 0, 0 } },               // fire temple
    { { 102, 144, 182 }, { 176, 167, 100 }, { 100, 167, 100 }, { 117, 97, 96 } },   // water temple
    { { 232, 210, 176 }, { 232, 210, 176 }, { 232, 210, 176 }, { 232, 210, 176 } }, // spirit temple
    { { 135, 125, 95 }, { 135, 125, 95 }, { 135, 125, 95 }, { 135, 125, 95 } },     // shadow temple
    { { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 }, { 255, 255, 255 } }, // ganons castle
    { { 232, 210, 176 }, { 232, 210, 176 }, { 232, 210, 176 }, { 232, 210, 176 } }, // gerudo training grounds
};

static s16 sScenes[] = {
    SCENE_YDAN,      SCENE_DDAN,    SCENE_BMORI1, SCENE_HIDAN, SCENE_MIZUSIN,
    SCENE_JYASINZOU, SCENE_HAKADAN, SCENE_GANON,  SCENE_MEN,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_STOP),
};

// The vertices and center of the bottom face
static Vec3f sColCheckPoints[5] = {
    { 29.99f, 1.01f, -29.99f }, { -29.99f, 1.01f, -29.99f }, { -29.99f, 1.01f, 29.99f },
    { 29.99f, 1.01f, 29.99f },  { 0.0f, 1.01f, 0.0f },
};

static Vec2f sFaceVtx[] = {
    { -30.0f, 0.0f },
    { 30.0f, 0.0f },
    { -30.0f, 60.0f },
    { 30.0f, 60.0f },
};

static Vec2f sFaceDirection[] = {
    { 1.0f, 1.0f },
    { -1.0f, 1.0f },
    { 1.0f, -1.0f },
    { -1.0f, -1.0f },
};

void ObjOshihiki_InitDynapoly(ObjOshihiki* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning : move BG registration failure"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_obj_oshihiki.c", 280,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void ObjOshihiki_RotateXZ(Vec3f* out, Vec3f* in, f32 sn, f32 cs) {
    out->x = (in->z * sn) + (in->x * cs);
    out->y = in->y;
    out->z = (in->z * cs) - (in->x * sn);
}

s32 ObjOshihiki_StrongEnough(ObjOshihiki* self) {
    s32 strength;

    if (self->cantMove) {
        return 0;
    }
    strength = Player_GetStrength();
    switch (self->dyna.actor.params & 0xF) {
        case PUSHBLOCK_SMALL_START_ON:
        case PUSHBLOCK_MEDIUM_START_ON:
        case PUSHBLOCK_SMALL_START_OFF:
        case PUSHBLOCK_MEDIUM_START_OFF:
            return 1;
            break;
        case PUSHBLOCK_LARGE_START_ON:
        case PUSHBLOCK_LARGE_START_OFF:
            return strength >= PLAYER_STR_BRACELET;
            break;
        case PUSHBLOCK_HUGE_START_ON:
        case PUSHBLOCK_HUGE_START_OFF:
            return strength >= PLAYER_STR_SILVER_G;
            break;
    }
    return 0;
}

void ObjOshihiki_ResetFloors(ObjOshihiki* self) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(self->floorBgIds); i++) {
        self->floorBgIds[i] = BGCHECK_SCENE;
    }
}

ObjOshihiki* ObjOshihiki_GetBlockUnder(ObjOshihiki* self, GlobalContext* globalCtx) {
    DynaPolyActor* dynaPolyActor;

    if ((self->floorBgIds[self->highestFloor] != BGCHECK_SCENE) &&
        (fabsf(self->dyna.actor.floorHeight - self->dyna.actor.world.pos.y) < 0.001f)) {
        dynaPolyActor = DynaPoly_GetActor(&globalCtx->colCtx, self->floorBgIds[self->highestFloor]);
        if ((dynaPolyActor != NULL) && (dynaPolyActor->actor.id == ACTOR_OBJ_OSHIHIKI)) {
            return (ObjOshihiki*)dynaPolyActor;
        }
    }
    return NULL;
}

void ObjOshihiki_UpdateInitPos(ObjOshihiki* self) {
    if (self->dyna.actor.home.pos.x < self->dyna.actor.world.pos.x) {
        while ((self->dyna.actor.world.pos.x - self->dyna.actor.home.pos.x) >= 20.0f) {
            self->dyna.actor.home.pos.x += 20.0f;
        }
    } else {
        while ((self->dyna.actor.home.pos.x - self->dyna.actor.world.pos.x) >= 20.0f) {
            self->dyna.actor.home.pos.x -= 20.0f;
        }
    }
    if (self->dyna.actor.home.pos.z < self->dyna.actor.world.pos.z) {
        while ((self->dyna.actor.world.pos.z - self->dyna.actor.home.pos.z) >= 20.0f) {
            self->dyna.actor.home.pos.z += 20.0f;
        }
    } else {
        while ((self->dyna.actor.home.pos.z - self->dyna.actor.world.pos.z) >= 20.0f) {
            self->dyna.actor.home.pos.z -= 20.0f;
        }
    }
}

s32 ObjOshihiki_NoSwitchPress(ObjOshihiki* self, DynaPolyActor* dyna, GlobalContext* globalCtx) {
    s16 dynaSwitchFlag;

    if (dyna == NULL) {
        return 1;
    } else if (dyna->actor.id == ACTOR_OBJ_SWITCH) {
        dynaSwitchFlag = (dyna->actor.params >> 8) & 0x3F;
        switch (dyna->actor.params & 0x33) {
            case 0x20: // Normal blue switch
                if ((dynaSwitchFlag == ((self->dyna.actor.params >> 8) & 0x3F)) &&
                    Flags_GetSwitch(globalCtx, dynaSwitchFlag)) {
                    return 0;
                }
                break;
            case 0x30: // Inverse blue switch
                if ((dynaSwitchFlag == ((self->dyna.actor.params >> 8) & 0x3F)) &&
                    !Flags_GetSwitch(globalCtx, dynaSwitchFlag)) {
                    return 0;
                }
                break;
        }
    }
    return 1;
}

void ObjOshihiki_CheckType(ObjOshihiki* self, GlobalContext* globalCtx) {
    switch (self->dyna.actor.params & 0xF) {
        case PUSHBLOCK_SMALL_START_ON:
        case PUSHBLOCK_MEDIUM_START_ON:
        case PUSHBLOCK_LARGE_START_ON:
        case PUSHBLOCK_HUGE_START_ON:
        case PUSHBLOCK_SMALL_START_OFF:
        case PUSHBLOCK_MEDIUM_START_OFF:
        case PUSHBLOCK_LARGE_START_OFF:
        case PUSHBLOCK_HUGE_START_OFF:
            ObjOshihiki_InitDynapoly(self, globalCtx, &gPushBlockCol, 1);
            break;
        default:
            // "Error : type cannot be determined"
            osSyncPrintf("Error : タイプが判別できない(%s %d)(arg_data 0x%04x)\n", "../z_obj_oshihiki.c", 444,
                         self->dyna.actor.params);
            break;
    }
}

void ObjOshihiki_SetScale(ObjOshihiki* self, GlobalContext* globalCtx) {
    Actor_SetScale(&self->dyna.actor, sScales[self->dyna.actor.params & 0xF]);
}

void ObjOshihiki_SetTexture(ObjOshihiki* self, GlobalContext* globalCtx) {
    switch (self->dyna.actor.params & 0xF) {
        case PUSHBLOCK_SMALL_START_ON:
        case PUSHBLOCK_MEDIUM_START_ON:
        case PUSHBLOCK_SMALL_START_OFF:
        case PUSHBLOCK_MEDIUM_START_OFF:
            self->texture = gPushBlockSilverTex;
            break;
        case PUSHBLOCK_LARGE_START_ON:
        case PUSHBLOCK_LARGE_START_OFF:
            self->texture = gPushBlockBaseTex;
            break;
        case PUSHBLOCK_HUGE_START_ON:
        case PUSHBLOCK_HUGE_START_OFF:
            self->texture = gPushBlockGrayTex;
            break;
    }
}

void ObjOshihiki_SetColor(ObjOshihiki* self, GlobalContext* globalCtx) {
    Color_RGB8* src;
    Color_RGB8* color = &self->color;
    s16 paramsColorIdx;
    s32 i;

    paramsColorIdx = (self->dyna.actor.params >> 6) & 3;

    for (i = 0; i < ARRAY_COUNT(sScenes); i++) {
        if (sScenes[i] == globalCtx->sceneNum) {
            break;
        }
    }

    if (i >= ARRAY_COUNT(sColors)) {
        // "Error : scene_data_ID cannot be determined"
        osSyncPrintf("Error : scene_data_ID が判別できない。(%s %d)\n", "../z_obj_oshihiki.c", 579);
        color->r = color->g = color->b = 255;
    } else {
        src = &sColors[i][paramsColorIdx];
        color->r = src->r;
        color->g = src->g;
        color->b = src->b;
    }
}

void ObjOshihiki_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    ObjOshihiki* self = THIS;

    ObjOshihiki_CheckType(self, globalCtx);

    if ((((self->dyna.actor.params >> 8) & 0xFF) >= 0) && (((self->dyna.actor.params >> 8) & 0xFF) <= 0x3F)) {
        if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
            switch (self->dyna.actor.params & 0xF) {
                case PUSHBLOCK_SMALL_START_ON:
                case PUSHBLOCK_MEDIUM_START_ON:
                case PUSHBLOCK_LARGE_START_ON:
                case PUSHBLOCK_HUGE_START_ON:
                    Actor_Kill(&self->dyna.actor);
                    return;
            }
        } else {
            switch (self->dyna.actor.params & 0xF) {
                case PUSHBLOCK_SMALL_START_OFF:
                case PUSHBLOCK_MEDIUM_START_OFF:
                case PUSHBLOCK_LARGE_START_OFF:
                case PUSHBLOCK_HUGE_START_OFF:
                    Actor_Kill(&self->dyna.actor);
                    return;
            }
        }
    }

    ObjOshihiki_SetScale(self, globalCtx);
    ObjOshihiki_SetTexture(self, globalCtx);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;
    ObjOshihiki_SetColor(self, globalCtx);
    ObjOshihiki_ResetFloors(self);
    ObjOshihiki_SetupOnActor(self, globalCtx);
    // "(dungeon keep push-pull block)"
    osSyncPrintf("(dungeon keep 押し引きブロック)(arg_data 0x%04x)\n", self->dyna.actor.params);
}

void ObjOshihiki_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    ObjOshihiki* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void ObjOshihiki_SetFloors(ObjOshihiki* self, GlobalContext* globalCtx) {
    s32 i;

    for (i = 0; i < 5; i++) {
        Vec3f colCheckPoint;
        Vec3f colCheckOffset;
        CollisionPoly** floorPoly;
        s32* floorBgId;

        colCheckOffset.x = sColCheckPoints[i].x * (self->dyna.actor.scale.x * 10.0f);
        colCheckOffset.y = sColCheckPoints[i].y * (self->dyna.actor.scale.y * 10.0f);
        colCheckOffset.z = sColCheckPoints[i].z * (self->dyna.actor.scale.z * 10.0f);
        ObjOshihiki_RotateXZ(&colCheckPoint, &colCheckOffset, self->yawSin, self->yawCos);
        colCheckPoint.x += self->dyna.actor.world.pos.x;
        colCheckPoint.y += self->dyna.actor.prevPos.y;
        colCheckPoint.z += self->dyna.actor.world.pos.z;

        floorPoly = &self->floorPolys[i];
        floorBgId = &self->floorBgIds[i];
        self->floorHeights[i] = BgCheck_EntityRaycastFloor6(&globalCtx->colCtx, floorPoly, floorBgId, &self->dyna.actor,
                                                            &colCheckPoint, 0.0f);
    }
}

s16 ObjOshihiki_GetHighestFloor(ObjOshihiki* self) {
    s16 highestFloor = 0;
    s16 temp = 1;
    f32 phi_f0 = self->floorHeights[temp];

    if (phi_f0 > self->floorHeights[highestFloor]) {
        highestFloor = temp;
    } else if ((self->floorBgIds[temp] == BGCHECK_SCENE) && ((phi_f0 - self->floorHeights[highestFloor]) > -0.001f)) {
        highestFloor = temp;
    }
    if (self->floorHeights[temp + 1] > self->floorHeights[highestFloor]) {
        highestFloor = temp + 1;
    } else if ((self->floorBgIds[temp + 1] == BGCHECK_SCENE) &&
               ((self->floorHeights[temp + 1] - self->floorHeights[highestFloor]) > -0.001f)) {
        highestFloor = temp + 1;
    }
    if (self->floorHeights[temp + 2] > self->floorHeights[highestFloor]) {
        highestFloor = temp + 2;
    } else if ((self->floorBgIds[temp + 2] == BGCHECK_SCENE) &&
               ((self->floorHeights[temp + 2] - self->floorHeights[highestFloor]) > -0.001f)) {
        highestFloor = temp + 2;
    }
    if (self->floorHeights[temp + 3] > self->floorHeights[highestFloor]) {
        highestFloor = temp + 3;
    } else if ((self->floorBgIds[temp + 3] == BGCHECK_SCENE) &&
               ((self->floorHeights[temp + 3] - self->floorHeights[highestFloor]) > -0.001f)) {
        highestFloor = temp + 3;
    }
    return highestFloor;
}

void ObjOshihiki_SetGround(ObjOshihiki* self, GlobalContext* globalCtx) {
    ObjOshihiki_ResetFloors(self);
    ObjOshihiki_SetFloors(self, globalCtx);
    self->highestFloor = ObjOshihiki_GetHighestFloor(self);
    self->dyna.actor.floorHeight = self->floorHeights[self->highestFloor];
}

s32 ObjOshihiki_CheckFloor(ObjOshihiki* self, GlobalContext* globalCtx) {
    ObjOshihiki_SetGround(self, globalCtx);

    if ((self->dyna.actor.floorHeight - self->dyna.actor.world.pos.y) >= -0.001f) {
        self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        return 1;
    }

    return 0;
}

s32 ObjOshihiki_CheckGround(ObjOshihiki* self, GlobalContext* globalCtx) {
    if (self->dyna.actor.world.pos.y <= BGCHECK_Y_MIN + 10.0f) {
        // "Warning : Push-pull block fell too much"
        osSyncPrintf("Warning : 押し引きブロック落ちすぎた(%s %d)(arg_data 0x%04x)\n", "../z_obj_oshihiki.c", 809,
                     self->dyna.actor.params);
        Actor_Kill(&self->dyna.actor);
        return 0;
    }
    if ((self->dyna.actor.floorHeight - self->dyna.actor.world.pos.y) >= -0.001f) {
        self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
        return 1;
    }
    return 0;
}

s32 ObjOshihiki_CheckWall(GlobalContext* globalCtx, s16 angle, f32 direction, ObjOshihiki* self) {
    f32 maxDist = ((direction >= 0.0f) ? 1.0f : -1.0f) * (300.0f * self->dyna.actor.scale.x + 20.0f - 0.5f);
    f32 sn = Math_SinS(angle);
    f32 cs = Math_CosS(angle);
    s32 i;

    for (i = 0; i < 4; i++) {
        Vec3f faceVtx;
        Vec3f faceVtxNext;
        Vec3f posResult;
        Vec3f faceVtxOffset;
        s32 bgId;
        CollisionPoly* outPoly;

        faceVtxOffset.x = (sFaceVtx[i].x * self->dyna.actor.scale.x * 10.0f) + sFaceDirection[i].x;
        faceVtxOffset.y = (sFaceVtx[i].y * self->dyna.actor.scale.y * 10.0f) + sFaceDirection[i].y;
        faceVtxOffset.z = 0.0f;
        ObjOshihiki_RotateXZ(&faceVtx, &faceVtxOffset, sn, cs);
        faceVtx.x += self->dyna.actor.world.pos.x;
        faceVtx.y += self->dyna.actor.world.pos.y;
        faceVtx.z += self->dyna.actor.world.pos.z;
        faceVtxNext.x = faceVtx.x + maxDist * sn;
        faceVtxNext.y = faceVtx.y;
        faceVtxNext.z = faceVtx.z + maxDist * cs;
        if (BgCheck_EntityLineTest3(&globalCtx->colCtx, &faceVtx, &faceVtxNext, &posResult, &outPoly, true, false,
                                    false, true, &bgId, &self->dyna.actor, 0.0f)) {
            return true;
        }
    }
    return false;
}

s32 ObjOshihiki_MoveWithBlockUnder(ObjOshihiki* self, GlobalContext* globalCtx) {
    s32 pad;
    ObjOshihiki* blockUnder = ObjOshihiki_GetBlockUnder(self, globalCtx);

    if ((blockUnder != NULL) && (blockUnder->stateFlags & PUSHBLOCK_SETUP_PUSH) &&
        !ObjOshihiki_CheckWall(globalCtx, blockUnder->dyna.unk_158, blockUnder->direction, self)) {
        self->blockUnder = blockUnder;
    }

    if ((self->stateFlags & PUSHBLOCK_MOVE_UNDER) && (self->blockUnder != NULL)) {
        if (self->blockUnder->stateFlags & PUSHBLOCK_PUSH) {
            self->underDistX = self->blockUnder->dyna.actor.world.pos.x - self->blockUnder->dyna.actor.prevPos.x;
            self->underDistZ = self->blockUnder->dyna.actor.world.pos.z - self->blockUnder->dyna.actor.prevPos.z;
            self->dyna.actor.world.pos.x += self->underDistX;
            self->dyna.actor.world.pos.z += self->underDistZ;
            ObjOshihiki_UpdateInitPos(self);
            return true;
        } else if (!(self->blockUnder->stateFlags & PUSHBLOCK_SETUP_PUSH)) {
            self->blockUnder = NULL;
        }
    }
    return false;
}

void ObjOshihiki_SetupOnScene(ObjOshihiki* self, GlobalContext* globalCtx) {
    self->stateFlags |= PUSHBLOCK_SETUP_ON_SCENE;
    self->actionFunc = ObjOshihiki_OnScene;
    self->dyna.actor.gravity = 0.0f;
    self->dyna.actor.velocity.x = self->dyna.actor.velocity.y = self->dyna.actor.velocity.z = 0.0f;
}

void ObjOshihiki_OnScene(ObjOshihiki* self, GlobalContext* globalCtx) {
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    self->stateFlags |= PUSHBLOCK_ON_SCENE;
    if ((self->timer <= 0) && (fabsf(self->dyna.unk_150) > 0.001f)) {
        if (ObjOshihiki_StrongEnough(self) &&
            !ObjOshihiki_CheckWall(globalCtx, self->dyna.unk_158, self->dyna.unk_150, self)) {
            self->direction = self->dyna.unk_150;
            ObjOshihiki_SetupPush(self, globalCtx);
        } else {
            player->stateFlags2 &= ~0x10;
            self->dyna.unk_150 = 0.0f;
        }
    } else {
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_150 = 0.0f;
    }
}

void ObjOshihiki_SetupOnActor(ObjOshihiki* self, GlobalContext* globalCtx) {
    self->stateFlags |= PUSHBLOCK_SETUP_ON_ACTOR;
    self->actionFunc = ObjOshihiki_OnActor;
    self->dyna.actor.velocity.x = self->dyna.actor.velocity.y = self->dyna.actor.velocity.z = 0.0f;
    self->dyna.actor.gravity = -1.0f;
}

void ObjOshihiki_OnActor(ObjOshihiki* self, GlobalContext* globalCtx) {
    s32 bgId;
    Player* player = GET_PLAYER(globalCtx);
    DynaPolyActor* dynaPolyActor;

    self->stateFlags |= PUSHBLOCK_ON_ACTOR;
    Actor_MoveForward(&self->dyna.actor);

    if (ObjOshihiki_CheckFloor(self, globalCtx)) {
        bgId = self->floorBgIds[self->highestFloor];
        if (bgId == BGCHECK_SCENE) {
            ObjOshihiki_SetupOnScene(self, globalCtx);
        } else {
            dynaPolyActor = DynaPoly_GetActor(&globalCtx->colCtx, bgId);
            if (dynaPolyActor != NULL) {
                func_800434A8(dynaPolyActor);
                func_80043538(dynaPolyActor);

                if ((self->timer <= 0) && (fabsf(self->dyna.unk_150) > 0.001f)) {
                    if (ObjOshihiki_StrongEnough(self) && ObjOshihiki_NoSwitchPress(self, dynaPolyActor, globalCtx) &&
                        !ObjOshihiki_CheckWall(globalCtx, self->dyna.unk_158, self->dyna.unk_150, self)) {

                        self->direction = self->dyna.unk_150;
                        ObjOshihiki_SetupPush(self, globalCtx);
                    } else {
                        player->stateFlags2 &= ~0x10;
                        self->dyna.unk_150 = 0.0f;
                    }
                } else {
                    player->stateFlags2 &= ~0x10;
                    self->dyna.unk_150 = 0.0f;
                }
            } else {
                ObjOshihiki_SetupOnScene(self, globalCtx);
            }
        }
    } else {
        bgId = self->floorBgIds[self->highestFloor];
        if (bgId == BGCHECK_SCENE) {
            ObjOshihiki_SetupFall(self, globalCtx);
        } else {
            dynaPolyActor = DynaPoly_GetActor(&globalCtx->colCtx, bgId);

            if ((dynaPolyActor != NULL) && (dynaPolyActor->unk_15C & 1)) {
                func_800434A8(dynaPolyActor);
                func_80043538(dynaPolyActor);
                self->dyna.actor.world.pos.y = self->dyna.actor.floorHeight;
            } else {
                ObjOshihiki_SetupFall(self, globalCtx);
            }
        }
    }
}

void ObjOshihiki_SetupPush(ObjOshihiki* self, GlobalContext* globalCtx) {
    self->stateFlags |= PUSHBLOCK_SETUP_PUSH;
    self->actionFunc = ObjOshihiki_Push;
    self->dyna.actor.gravity = 0.0f;
}

void ObjOshihiki_Push(ObjOshihiki* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;
    Player* player = GET_PLAYER(globalCtx);
    f32 pushDistSigned;
    s32 stopFlag;

    self->pushSpeed += 0.5f;
    self->stateFlags |= PUSHBLOCK_PUSH;
    self->pushSpeed = CLAMP_MAX(self->pushSpeed, 2.0f);
    stopFlag = Math_StepToF(&self->pushDist, 20.0f, self->pushSpeed);
    pushDistSigned = ((self->direction >= 0.0f) ? 1.0f : -1.0f) * self->pushDist;
    thisx->world.pos.x = thisx->home.pos.x + (pushDistSigned * self->yawSin);
    thisx->world.pos.z = thisx->home.pos.z + (pushDistSigned * self->yawCos);

    if (!ObjOshihiki_CheckFloor(self, globalCtx)) {
        thisx->home.pos.x = thisx->world.pos.x;
        thisx->home.pos.z = thisx->world.pos.z;
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_150 = 0.0f;
        self->pushDist = 0.0f;
        self->pushSpeed = 0.0f;
        ObjOshihiki_SetupFall(self, globalCtx);
    } else if (stopFlag) {
        player = GET_PLAYER(globalCtx);
        if (ObjOshihiki_CheckWall(globalCtx, self->dyna.unk_158, self->dyna.unk_150, self)) {
            Audio_PlayActorSound2(thisx, NA_SE_EV_BLOCK_BOUND);
        }

        thisx->home.pos.x = thisx->world.pos.x;
        thisx->home.pos.z = thisx->world.pos.z;
        player->stateFlags2 &= ~0x10;
        self->dyna.unk_150 = 0.0f;
        self->pushDist = 0.0f;
        self->pushSpeed = 0.0f;
        self->timer = 10;
        if (self->floorBgIds[self->highestFloor] == BGCHECK_SCENE) {
            ObjOshihiki_SetupOnScene(self, globalCtx);
        } else {
            ObjOshihiki_SetupOnActor(self, globalCtx);
        }
    }
    Audio_PlayActorSound2(thisx, NA_SE_EV_ROCK_SLIDE - SFX_FLAG);
}

void ObjOshihiki_SetupFall(ObjOshihiki* self, GlobalContext* globalCtx) {
    self->stateFlags |= PUSHBLOCK_SETUP_FALL;
    self->dyna.actor.velocity.x = self->dyna.actor.velocity.y = self->dyna.actor.velocity.z = 0.0f;
    self->dyna.actor.gravity = -1.0f;
    ObjOshihiki_SetGround(self, globalCtx);
    self->actionFunc = ObjOshihiki_Fall;
}

void ObjOshihiki_Fall(ObjOshihiki* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->stateFlags |= PUSHBLOCK_FALL;
    if (fabsf(self->dyna.unk_150) > 0.001f) {
        self->dyna.unk_150 = 0.0f;
        player->stateFlags2 &= ~0x10;
    }
    Actor_MoveForward(&self->dyna.actor);
    if (ObjOshihiki_CheckGround(self, globalCtx)) {
        if (self->floorBgIds[self->highestFloor] == BGCHECK_SCENE) {
            ObjOshihiki_SetupOnScene(self, globalCtx);
        } else {
            ObjOshihiki_SetupOnActor(self, globalCtx);
        }
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        Audio_PlayActorSound2(&self->dyna.actor,
                              SurfaceType_GetSfx(&globalCtx->colCtx, self->floorPolys[self->highestFloor],
                                                 self->floorBgIds[self->highestFloor]) +
                                  SFX_FLAG);
    }
}

void ObjOshihiki_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    ObjOshihiki* self = THIS;

    self->stateFlags &=
        ~(PUSHBLOCK_SETUP_FALL | PUSHBLOCK_FALL | PUSHBLOCK_SETUP_PUSH | PUSHBLOCK_PUSH | PUSHBLOCK_SETUP_ON_ACTOR |
          PUSHBLOCK_ON_ACTOR | PUSHBLOCK_SETUP_ON_SCENE | PUSHBLOCK_ON_SCENE);
    self->stateFlags |= PUSHBLOCK_MOVE_UNDER;

    if (self->timer > 0) {
        self->timer--;
    }

    self->dyna.actor.world.rot.y = self->dyna.unk_158;

    self->yawSin = Math_SinS(self->dyna.actor.world.rot.y);
    self->yawCos = Math_CosS(self->dyna.actor.world.rot.y);

    if (self->actionFunc != NULL) {
        self->actionFunc(self, globalCtx);
    }
}

void ObjOshihiki_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    ObjOshihiki* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_oshihiki.c", 1289);
    if (ObjOshihiki_MoveWithBlockUnder(self, globalCtx)) {
        Matrix_Translate(self->underDistX * 10.0f, 0.0f, self->underDistZ * 10.0f, MTXMODE_APPLY);
    }
    self->stateFlags &= ~PUSHBLOCK_MOVE_UNDER;
    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(self->texture));

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_oshihiki.c", 1308),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    switch (globalCtx->sceneNum) {
        case SCENE_YDAN:
        case SCENE_DDAN:
        case SCENE_BMORI1:
        case SCENE_HIDAN:
        case SCENE_MIZUSIN:
        case SCENE_JYASINZOU:
        case SCENE_HAKADAN:
        case SCENE_MEN:
            gDPSetEnvColor(POLY_OPA_DISP++, self->color.r, self->color.g, self->color.b, 255);
            break;
        default:
            gDPSetEnvColor(POLY_OPA_DISP++, mREG(13), mREG(14), mREG(15), 255);
            break;
    }

    gSPDisplayList(POLY_OPA_DISP++, &gPushBlockDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_oshihiki.c", 1334);
}
