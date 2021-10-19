#include "z_en_go2.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_oF1d_map/object_oF1d_map.h"

#define FLAGS 0x00000039

#define THIS ((EnGo2*)thisx)

/*
FLAGS

gSaveContext.eventChkInf[2] & 0x8 - DC entrance boulder blown up as child

InfTable

gSaveContext.infTable[11] & 0x10
gSaveContext.infTable[14] & 0x1 - Talked to DMT Goron at DC entrance (Before DC is opened as child)
gSaveContext.infTable[14] & 0x8 - Talked to GC Goron in bottom level stairwell
gSaveContext.infTable[14] & 0x40 - Talked to GC Goron at LW entrance (Before LW shortcut is opened)
gSaveContext.infTable[14] & 0x800 - Talked to DMT Goron at Bomb Flower with goron bracelet
gSaveContext.infTable[15] & 0x1 - Talked to Goron at GC Entrance (Before goron ruby is obtained)
gSaveContext.infTable[15] & 0x10 - Talked to Goron at GC Island (Before goron ruby is obtained)
gSaveContext.infTable[15] & 0x100 - (not on cloud modding) Talked to GC Goron outside Darunias door (after opening door,
before getting goron bracelet) gSaveContext.infTable[16] & 0x200 - Obtained Fire Tunic from Goron Link
gSaveContext.infTable[16] & 0x400 - (not on cloud modding)
gSaveContext.infTable[16] & 0x800 - Spoke to Goron Link About Volvagia
gSaveContext.infTable[16] & 0x1000 - Stopped Goron Link's Rolling
gSaveContext.infTable[16] & 0x2000 - EnGo Exclusive
gSaveContext.infTable[16] & 0x4000 - Spoke to Goron Link
gSaveContext.infTable[16] & 0x8000 - (not on cloud modding)

gSaveContext.infTable[17] & 0x4000 - Bomb bag upgrade obtained from rolling Goron

EnGo
pathIndex: self->actor.params & 0xF
Goron: self->actor.params & 0xF0

EnGo2
(self->actor.params & 0x3E0) >> 5
(self->actor.params & 0xFC00) >> 0xA - Gorons in Fire Temple
self->actor.params & 0x1F

Gorons only move when self->unk_194.unk_00 == 0
*/

void EnGo2_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGo2_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGo2_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGo2_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnGo2_StopRolling(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_CurledUp(EnGo2* self, GlobalContext* globalCtx);

void func_80A46B40(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_GoronDmtBombFlowerAnimation(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_GoronRollingBigContinueRolling(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_ContinueRolling(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_SlowRolling(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_GroundRolling(EnGo2* self, GlobalContext* globalCtx);

void EnGo2_ReverseRolling(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_SetupGetItem(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_SetGetItem(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_BiggoronEyedrops(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_GoronLinkStopRolling(EnGo2* self, GlobalContext* globalCtx);
void EnGo2_GoronFireGenericAction(EnGo2* self, GlobalContext* globalCtx);

static void* sDustTex[] = { gDust8Tex, gDust7Tex, gDust6Tex, gDust5Tex, gDust4Tex, gDust3Tex, gDust2Tex, gDust1Tex };

static Vec3f sPos = { 0.0f, 0.0f, 0.0f };
static Vec3f sVelocity = { 0.0f, 0.0f, 0.0f };
static Vec3f sAccel = { 0.0f, 0.3f, 0.0f };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000008, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 40, 65, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = {
    0, 0, 0, 0, MASS_IMMOVABLE,
};

const ActorInit En_Go2_InitVars = {
    ACTOR_EN_GO2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OF1D_MAP,
    sizeof(EnGo2),
    (ActorFunc)EnGo2_Init,
    (ActorFunc)EnGo2_Destroy,
    (ActorFunc)EnGo2_Update,
    (ActorFunc)EnGo2_Draw,
};

static EnGo2DataStruct1 D_80A4816C[14] = {
    { 0, 0, 0, 68, 148 }, { 0, 0, 0, 24, 52 }, { 0, 320, 380, 400, 120 }, { 0, 0, 0, 30, 68 }, { 0, 0, 0, 46, 90 },
    { 0, 0, 0, 30, 68 },  { 0, 0, 0, 30, 68 }, { 0, 0, 0, 30, 68 },       { 0, 0, 0, 30, 68 }, { 0, 0, 0, 30, 68 },
    { 0, 0, 0, 30, 68 },  { 0, 0, 0, 30, 68 }, { 0, 0, 0, 30, 68 },       { 0, 0, 0, 30, 68 },
};

static EnGo2DataStruct2 D_80A481F8[14] = {
    { 30.0f, 0.026f, 6, 60.0f }, { 24.0f, 0.008f, 6, 30.0f }, { 28.0f, 0.16f, 5, 380.0f }, { 28.0f, 0.01f, 7, 40.0f },
    { 30.0f, 0.015f, 6, 30.0f }, { 28.0f, 0.01f, 6, 30.0f },  { 28.0f, 0.01f, 6, 30.0f },  { 28.0f, 0.01f, 6, 30.0f },
    { 28.0f, 0.01f, 6, 30.0f },  { 28.0f, 0.01f, 6, 30.0f },  { 28.0f, 0.01f, 6, 30.0f },  { 28.0f, 0.01f, 6, 30.0f },
    { 28.0f, 0.01f, 6, 30.0f },  { 28.0f, 0.01f, 6, 30.0f },
};

static f32 D_80A482D8[14][2] = {
    { 80.0f, 80.0f }, { -10.0f, -10.0f }, { 800.0f, 800.0f }, { 0.0f, 0.0f },   { 20.0f, 40.0f },
    { 20.0f, 20.0f }, { 20.0f, 20.0f },   { 20.0f, 20.0f },   { 20.0f, 20.0f }, { 20.0f, 20.0f },
    { 20.0f, 20.0f }, { 20.0f, 20.0f },   { 20.0f, 20.0f },   { 20.0f, 20.0f },
};

static struct_80034EC0_Entry sAnimations[] = {
    { &gGoronAnim_004930, 0.0f, 0.0f, -1.0f, 0x00, 0.0f },  { &gGoronAnim_004930, 0.0f, 0.0f, -1.0f, 0x00, -8.0f },
    { &gGoronAnim_0029A8, 1.0f, 0.0f, -1.0f, 0x00, -8.0f }, { &gGoronAnim_010590, 1.0f, 0.0f, -1.0f, 0x00, -8.0f },
    { &gGoronAnim_003768, 1.0f, 0.0f, -1.0f, 0x00, -8.0f }, { &gGoronAnim_0038E4, 1.0f, 0.0f, -1.0f, 0x02, -8.0f },
    { &gGoronAnim_002D80, 1.0f, 0.0f, -1.0f, 0x02, -8.0f }, { &gGoronAnim_00161C, 1.0f, 0.0f, -1.0f, 0x00, -8.0f },
    { &gGoronAnim_001A00, 1.0f, 0.0f, -1.0f, 0x00, -8.0f }, { &gGoronAnim_0021D0, 1.0f, 0.0f, -1.0f, 0x00, -8.0f },
    { &gGoronAnim_004930, 0.0f, 0.0f, -1.0f, 0x01, -8.0f }, { &gGoronAnim_000750, 1.0f, 0.0f, -1.0f, 0x00, -8.0f },
    { &gGoronAnim_000D5C, 1.0f, 0.0f, -1.0f, 0x00, -8.0f },
};

static EnGo2DustEffectData sDustEffectData[2][4] = {
    {
        { 12, 0.2f, 0.2f, 1, 18.0f, 0.0f },
        { 12, 0.1f, 0.2f, 12, 26.0f, 0.0f },
        { 12, 0.1f, 0.3f, 4, 10.0f, 0.0f },
        { 12, 0.2f, 0.2f, 1, 18.0f, 0.0f },
    },
    {
        { 12, 0.5f, 0.4f, 3, 42.0f, 0.0f },
        { 12, 0.5f, 0.4f, 3, 42.0f, 0.0f },
        { 12, 0.5f, 0.4f, 3, 42.0f, 0.0f },
        { 12, 0.5f, 0.4f, 3, 42.0f, 0.0f },
    },
};

static Vec3f sZeroVec = { 0.0f, 0.0f, 0.0f };

void EnGo2_AddDust(EnGo2* self, Vec3f* pos, Vec3f* velocity, Vec3f* accel, u8 initialTimer, f32 scale, f32 scaleStep) {
    EnGoEffect* dustEffect = self->dustEffects;
    s16 i;
    s16 timer;

    for (i = 0; i < ARRAY_COUNT(self->dustEffects); i++, dustEffect++) {
        if (dustEffect->type != 1) {
            dustEffect->scale = scale;
            dustEffect->scaleStep = scaleStep;
            if (1) {}
            timer = initialTimer;
            dustEffect->timer = timer;
            dustEffect->type = 1;
            dustEffect->initialTimer = initialTimer;
            dustEffect->pos = *pos;
            dustEffect->accel = *accel;
            dustEffect->velocity = *velocity;
            return;
        }
    }
}

void EnGo2_UpdateDust(EnGo2* self) {
    EnGoEffect* dustEffect = self->dustEffects;
    f32 randomNumber;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(self->dustEffects); i++, dustEffect++) {
        if (dustEffect->type) {
            dustEffect->timer--;
            if (dustEffect->timer == 0) {
                dustEffect->type = 0;
            }
            dustEffect->accel.x = (Rand_ZeroOne() * 0.4f) - 0.2f;
            randomNumber = Rand_ZeroOne() * 0.4f;
            dustEffect->accel.z = randomNumber - 0.2f;
            dustEffect->pos.x += dustEffect->velocity.x;
            dustEffect->pos.y += dustEffect->velocity.y;
            dustEffect->pos.z += dustEffect->velocity.z;
            dustEffect->velocity.x += dustEffect->accel.x;
            dustEffect->velocity.y += dustEffect->accel.y;
            dustEffect->velocity.z += randomNumber - 0.2f;
            dustEffect->scale += dustEffect->scaleStep;
        }
    }
}

void EnGo2_DrawDust(EnGo2* self, GlobalContext* globalCtx) {
    EnGoEffect* dustEffect = self->dustEffects;
    s16 alpha;
    s16 firstDone;
    s16 index;
    s16 i;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_go2_eff.c", 111);

    firstDone = false;
    func_80093D84(globalCtx->state.gfxCtx);
    if (1) {}

    for (i = 0; i < ARRAY_COUNT(self->dustEffects); i++, dustEffect++) {
        if (dustEffect->type) {
            if (!firstDone) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);
                gSPDisplayList(POLY_XLU_DISP++, gGoronDL_00FD40);
                gDPSetEnvColor(POLY_XLU_DISP++, 100, 60, 20, 0);
                firstDone = true;
            }

            alpha = dustEffect->timer * (255.0f / dustEffect->initialTimer);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 170, 130, 90, alpha);
            gDPPipeSync(POLY_XLU_DISP++);
            Matrix_Translate(dustEffect->pos.x, dustEffect->pos.y, dustEffect->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(dustEffect->scale, dustEffect->scale, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_go2_eff.c", 137),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            index = dustEffect->timer * (8.0f / dustEffect->initialTimer);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sDustTex[index]));
            gSPDisplayList(POLY_XLU_DISP++, gGoronDL_00FD50);
        }
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_go2_eff.c", 151);
}

s32 EnGo2_SpawnDust(EnGo2* self, u8 initialTimer, f32 scale, f32 scaleStep, s32 numDustEffects, f32 radius,
                    f32 yAccel) {
    Vec3f pos = sPos;
    Vec3f velocity = sVelocity;
    Vec3f accel = sAccel;
    s32 i;
    s16 angle;

    pos = self->actor.world.pos; // overwrites sPos data
    pos.y = self->actor.floorHeight;
    angle = (Rand_ZeroOne() - 0.5f) * 0x10000;
    i = numDustEffects;
    while (i >= 0) {
        accel.y += Rand_ZeroOne() * yAccel;
        pos.x = (Math_SinS(angle) * radius) + self->actor.world.pos.x;
        pos.z = (Math_CosS(angle) * radius) + self->actor.world.pos.z;
        EnGo2_AddDust(self, &pos, &velocity, &accel, initialTimer, scale, scaleStep);
        angle += (s16)(0x10000 / numDustEffects);
        i--;
    }
    return 0;
}

void EnGo2_GetItem(EnGo2* self, GlobalContext* globalCtx, s32 getItemId) {
    self->getItemId = getItemId;
    func_8002F434(&self->actor, globalCtx, getItemId, self->actor.xzDistToPlayer + 1.0f,
                  fabsf(self->actor.yDistToPlayer) + 1.0f);
}

s32 EnGo2_GetDialogState(EnGo2* self, GlobalContext* globalCtx) {
    s16 dialogState = func_8010BDBC(&globalCtx->msgCtx);

    if ((self->dialogState == 10) || (self->dialogState == 5) || (self->dialogState == 2) || (self->dialogState == 1)) {
        if (dialogState != self->dialogState) {
            self->unk_20C++;
        }
    }

    self->dialogState = dialogState;
    return dialogState;
}

u16 EnGo2_GoronFireGenericGetTextId(EnGo2* self) {
    switch ((self->actor.params & 0xFC00) >> 0xA) {
        case 3:
            return 0x3069;
        case 5:
            return 0x306A;
        case 4:
            return 0x306B;
        case 2:
            return 0x306C;
        case 10:
            return 0x306D;
        case 8:
            return 0x306E;
        case 11:
            return 0x306F;
        case 1:
            return 0x3070;
        default:
            return 0x3052;
    }
}

u16 EnGo2_GetTextIdGoronCityRollingBig(GlobalContext* globalCtx, EnGo2* self) {
    if (gSaveContext.infTable[17] & 0x4000) {
        return 0x3013;
    } else if (CUR_CAPACITY(UPG_BOMB_BAG) >= 20 && self->waypoint > 7 && self->waypoint < 12) {
        return 0x3012;
    } else {
        return 0x3011;
    }
}

s16 EnGo2_GetStateGoronCityRollingBig(GlobalContext* globalCtx, EnGo2* self) {
    s32 bombBagUpgrade;

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 2:
            return 2;
        case 5:
            if (func_80106BC8(globalCtx)) {
                if (self->actor.textId == 0x3012) {
                    self->actionFunc = EnGo2_SetupGetItem;
                    bombBagUpgrade = CUR_CAPACITY(UPG_BOMB_BAG) == 30 ? GI_BOMB_BAG_40 : GI_BOMB_BAG_30;
                    EnGo2_GetItem(self, globalCtx, bombBagUpgrade);
                    func_80106CCC(globalCtx);
                    gSaveContext.infTable[17] |= 0x4000;
                    return 2;
                } else {
                    return 2;
                }
            }
        default:
            return 1;
    }
}

u16 EnGo2_GetTextIdGoronDmtBombFlower(GlobalContext* globalCtx, EnGo2* self) {
    return CHECK_QUEST_ITEM(QUEST_GORON_RUBY) ? 0x3027 : 0x300A;
}

// DMT Goron by Bomb Flower Choice
s16 EnGo2_GetStateGoronDmtBombFlower(GlobalContext* globalCtx, EnGo2* self) {
    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 2:
            if ((self->actor.textId == 0x300B) && (gSaveContext.infTable[14] & 0x800) == 0) {
                gSaveContext.infTable[14] |= 0x800;
                return 2;
            } else {
                return 0;
            }
        case 4:
            if (func_80106BC8(globalCtx)) {
                // Ask question to DMT Goron by bomb flower
                if (self->actor.textId == 0x300A) {
                    if (globalCtx->msgCtx.choiceIndex == 0) {
                        self->actor.textId = CUR_UPG_VALUE(UPG_STRENGTH) != 0 ? 0x300B : 0x300C;
                    } else {
                        self->actor.textId = 0x300D;
                    }
                    func_8010B720(globalCtx, self->actor.textId);
                }
                return 1;
            }
        default:
            return 1;
    }
}

u16 EnGo2_GetTextIdGoronDmtRollingSmall(GlobalContext* globalCtx, EnGo2* self) {
    if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
        return 0x3027;
    } else {
        return (gSaveContext.eventChkInf[2] & 0x8) ? 0x3026 : 0x3009;
    }
}

s16 EnGo2_GetStateGoronDmtRollingSmall(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        return 0;
    } else {
        return 1;
    }
}

u16 EnGo2_GetTextIdGoronDmtDcEntrance(GlobalContext* globalCtx, EnGo2* self) {
    if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && LINK_IS_ADULT) {
        return 0x3043;
    } else if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
        return 0x3027;
    } else {
        return gSaveContext.eventChkInf[2] & 0x8 ? 0x3021 : gSaveContext.infTable[14] & 0x1 ? 0x302A : 0x3008;
    }
}

s16 EnGo2_GetStateGoronDmtDcEntrance(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        if (self->actor.textId == 0x3008) {
            gSaveContext.infTable[14] |= 0x1;
        }
        return 0;
    } else {
        return 1;
    }
}

u16 EnGo2_GetTextIdGoronCityEntrance(GlobalContext* globalCtx, EnGo2* self) {
    if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && LINK_IS_ADULT) {
        return 0x3043;
    } else if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
        return 0x3027;
    } else {
        return gSaveContext.infTable[15] & 0x1 ? 0x3015 : 0x3014;
    }
}

s16 EnGo2_GetStateGoronCityEntrance(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        if (self->actor.textId == 0x3014) {
            gSaveContext.infTable[15] |= 0x1;
        }
        return 0;
    } else {
        return 1;
    }
}

u16 EnGo2_GetTextIdGoronCityIsland(GlobalContext* globalCtx, EnGo2* self) {
    if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && LINK_IS_ADULT) {
        return 0x3043;
    } else if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
        return 0x3067;
    } else {
        return gSaveContext.infTable[15] & 0x10 ? 0x3017 : 0x3016;
    }
}

s16 EnGo2_GetStateGoronCityIsland(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        if (self->actor.textId == 0x3016) {
            gSaveContext.infTable[15] |= 0x10;
        }
        return 0;
    } else {
        return 1;
    }
}

u16 EnGo2_GetTextIdGoronCityLowestFloor(GlobalContext* globalCtx, EnGo2* self) {
    if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && LINK_IS_ADULT) {
        return 0x3043;
    } else if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
        return 0x3027;
    } else {
        return CUR_UPG_VALUE(UPG_STRENGTH) != 0
                   ? 0x302C
                   : !Flags_GetSwitch(globalCtx, 0x1B) ? 0x3017 : gSaveContext.infTable[15] & 0x100 ? 0x3019 : 0x3018;
    }
}

s16 EnGo2_GetStateGoronCityLowestFloor(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        if (self->actor.textId == 0x3018) {
            gSaveContext.infTable[15] |= 0x100;
        }
        return 0;
    } else {
        return 1;
    }
}

u16 EnGo2_GetTextIdGoronCityLink(GlobalContext* globalCtx, EnGo2* self) {
    if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE)) {
        return gSaveContext.infTable[16] & 0x8000 ? 0x3042 : 0x3041;
    } else if (CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1)) {
        return gSaveContext.infTable[16] & 0x4000 ? 0x3038 : 0x3037;
    } else if (gSaveContext.infTable[16] & 0x1000) {
        self->unk_20C = 0;
        self->dialogState = 0;
        return gSaveContext.infTable[16] & 0x400 ? 0x3033 : 0x3032;
    } else {
        return 0x3030;
    }
}

s16 EnGo2_GetStateGoronCityLink(GlobalContext* globalCtx, EnGo2* self) {
    switch (EnGo2_GetDialogState(self, globalCtx)) {
        case 2:
            switch (self->actor.textId) {
                case 0x3036:
                    EnGo2_GetItem(self, globalCtx, GI_TUNIC_GORON);
                    self->actionFunc = EnGo2_SetupGetItem;
                    return 2;
                case 0x3037:
                    gSaveContext.infTable[16] |= 0x4000;
                default:
                    return 0;
            }
        case 4:
            if (func_80106BC8(globalCtx)) {
                if (self->actor.textId == 0x3034) {
                    if (globalCtx->msgCtx.choiceIndex == 0) {
                        self->actor.textId = gSaveContext.infTable[16] & 0x800 ? 0x3033 : 0x3035;
                        if (self->actor.textId == 0x3035) {
                            Audio_StopSfx(0x39EB);
                        }
                    } else {
                        self->actor.textId = gSaveContext.infTable[16] & 0x800 ? 0x3036 : 0x3033;
                        if (self->actor.textId == 0x3036) {
                            Audio_StopSfx(0x39EB);
                        }
                    }
                    func_8010B720(globalCtx, self->actor.textId);
                    self->unk_20C = 0;
                }
            } else {
                break;
            }
            return 1;
        case 5:
            if (func_80106BC8(globalCtx)) {
                switch (self->actor.textId) {
                    case 0x3035:
                        gSaveContext.infTable[16] |= 0x800;
                    case 0x3032:
                    case 0x3033:
                        self->actor.textId = 0x3034;
                        func_8010B720(globalCtx, self->actor.textId);
                        return 1;
                    default:
                        return 2;
                }
            }
    }
    return 1;
}

u16 EnGo2_GetTextIdGoronDmtBiggoron(GlobalContext* globalCtx, EnGo2* self) {
    Player* player = GET_PLAYER(globalCtx);

    if (gSaveContext.bgsFlag) {
        player->exchangeItemId = EXCH_ITEM_CLAIM_CHECK;
        return 0x305E;
    } else if (INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_CLAIM_CHECK) {
        player->exchangeItemId = EXCH_ITEM_CLAIM_CHECK;
        return 0x305E;
    } else if (INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_PRESCRIPTION) {
        player->exchangeItemId = EXCH_ITEM_EYEDROPS;
        return 0x3058;
    } else {
        player->exchangeItemId = EXCH_ITEM_SWORD_BROKEN;
        return 0x3053;
    }
}

s16 EnGo2_GetStateGoronDmtBiggoron(GlobalContext* globalCtx, EnGo2* self) {
    s32 unusedPad;
    u8 dialogState = self->dialogState;

    switch (EnGo2_GetDialogState(self, globalCtx)) {
        case 6:
            if (self->actor.textId == 0x305E) {
                if (!gSaveContext.bgsFlag) {
                    EnGo2_GetItem(self, globalCtx, GI_SWORD_BGS);
                    self->actionFunc = EnGo2_SetupGetItem;
                    return 2;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        case 3:
            switch (self->actor.textId) {
                case 0x305E:
                    if (func_8002F368(globalCtx) != EXCH_ITEM_CLAIM_CHECK) {
                        break;
                    }
                case 0x3059:
                    if (dialogState == 0) {
                        func_800F4524(&D_801333D4, NA_SE_EN_GOLON_WAKE_UP, 60);
                    }
                case 0x3054:
                    if (dialogState == 0) {
                        Audio_PlaySoundGeneral(NA_SE_SY_TRE_BOX_APPEAR, &D_801333D4, 4, &D_801333E0, &D_801333E0,
                                               &D_801333E8);
                    }
            }
            return 1;
        case 4:
            if (func_80106BC8(globalCtx)) {
                if ((self->actor.textId == 0x3054) || (self->actor.textId == 0x3055)) {
                    if (globalCtx->msgCtx.choiceIndex == 0) {
                        EnGo2_GetItem(self, globalCtx, GI_PRESCRIPTION);
                        self->actionFunc = EnGo2_SetupGetItem;
                        return 2;
                    }
                    self->actor.textId = 0x3056;
                    func_8010B720(globalCtx, self->actor.textId);
                }
                return 1;
            }
            break;
        case 5:
            if (func_80106BC8(globalCtx)) {
                if (self->actor.textId == 0x3059) {
                    globalCtx->msgCtx.msgMode = 0x37;
                    self->actionFunc = EnGo2_BiggoronEyedrops;
                }
                return 2;
            }
    }
    return 1;
}

u16 EnGo2_GetTextIdGoronFireGeneric(GlobalContext* globalCtx, EnGo2* self) {
    if (Flags_GetSwitch(globalCtx, (self->actor.params & 0xFC00) >> 0xA)) {
        return 0x3071;
    } else {
        return 0x3051;
    }
}

s16 EnGo2_GetStateGoronFireGeneric(GlobalContext* globalCtx, EnGo2* self) {
    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 2:
            return 0;
        case 5:
            if (func_80106BC8(globalCtx)) {
                if (self->actor.textId == 0x3071) {
                    self->actor.textId = EnGo2_GoronFireGenericGetTextId(self);
                    func_8010B720(globalCtx, self->actor.textId);
                }
                return 1;
            }
        default:
            return 1;
    }
}

u16 EnGo2_GetTextIdGoronCityStairwell(GlobalContext* globalCtx, EnGo2* self) {
    return !LINK_IS_ADULT ? gSaveContext.infTable[14] & 0x8 ? 0x3022 : 0x300E : 0x3043;
}

s16 EnGo2_GetStateGoronCityStairwell(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        if (self->actor.textId == 0x300E) {
            gSaveContext.infTable[14] |= 0x8;
        }
        return 0;
    } else {
        return 1;
    }
}

// Goron in child market bazaar after obtaining Goron Ruby
u16 EnGo2_GetTextIdGoronMarketBazaar(GlobalContext* globalCtx, EnGo2* self) {
    return 0x7122;
}

s16 EnGo2_GetStateGoronMarketBazaar(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        return 0;
    } else {
        return 1;
    }
}

u16 EnGo2_GetTextIdGoronCityLostWoods(GlobalContext* globalCtx, EnGo2* self) {
    if (!LINK_IS_ADULT) {
        if (Flags_GetSwitch(globalCtx, 0x1C)) {
            return 0x302F;
        } else {
            return gSaveContext.infTable[14] & 0x40 ? 0x3025 : 0x3024;
        }
    } else {
        return 0x3043;
    }
}

s16 EnGo2_GetStateGoronCityLostWoods(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        if (self->actor.textId == 0x3024) {
            gSaveContext.infTable[14] |= 0x40;
        }
        return 0;
    } else {
        return 1;
    }
}

// Goron at base of DMT summit
u16 EnGo2_GetTextIdGoronDmtFairyHint(GlobalContext* globalCtx, EnGo2* self) {
    if (!LINK_IS_ADULT) {
        return CHECK_QUEST_ITEM(QUEST_GORON_RUBY) ? 0x3065 : 0x3064;
    } else {
        return 0x3043;
    }
}

s16 EnGo2_GetStateGoronDmtFairyHint(GlobalContext* globalCtx, EnGo2* self) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        return 0;
    } else {
        return 1;
    }
}

u16 EnGo2_GetTextId(GlobalContext* globalCtx, Actor* thisx) {
    EnGo2* self = THIS;
    u16 faceReaction = Text_GetFaceReaction(globalCtx, 0x20);

    if (faceReaction) {
        return faceReaction;
    } else {
        switch (self->actor.params & 0x1F) {
            case GORON_CITY_ROLLING_BIG:
                return EnGo2_GetTextIdGoronCityRollingBig(globalCtx, self);
            case GORON_CITY_LINK:
                return EnGo2_GetTextIdGoronCityLink(globalCtx, self);
            case GORON_DMT_BIGGORON:
                return EnGo2_GetTextIdGoronDmtBiggoron(globalCtx, self);
            case GORON_FIRE_GENERIC:
                return EnGo2_GetTextIdGoronFireGeneric(globalCtx, self);
            case GORON_DMT_BOMB_FLOWER:
                return EnGo2_GetTextIdGoronDmtBombFlower(globalCtx, self);
            case GORON_DMT_ROLLING_SMALL:
                return EnGo2_GetTextIdGoronDmtRollingSmall(globalCtx, self);
            case GORON_DMT_DC_ENTRANCE:
                return EnGo2_GetTextIdGoronDmtDcEntrance(globalCtx, self);
            case GORON_CITY_ENTRANCE:
                return EnGo2_GetTextIdGoronCityEntrance(globalCtx, self);
            case GORON_CITY_ISLAND:
                return EnGo2_GetTextIdGoronCityIsland(globalCtx, self);
            case GORON_CITY_LOWEST_FLOOR:
                return EnGo2_GetTextIdGoronCityLowestFloor(globalCtx, self);
            case GORON_CITY_STAIRWELL:
                return EnGo2_GetTextIdGoronCityStairwell(globalCtx, self);
            case GORON_CITY_LOST_WOODS:
                return EnGo2_GetTextIdGoronCityLostWoods(globalCtx, self);
            case GORON_DMT_FAIRY_HINT:
                return EnGo2_GetTextIdGoronDmtFairyHint(globalCtx, self);
            case GORON_MARKET_BAZAAR:
                return EnGo2_GetTextIdGoronMarketBazaar(globalCtx, self);
        }
    }
#ifdef AVOID_UB
    return 0; // surpress warning
#endif
}

s16 EnGo2_GetState(GlobalContext* globalCtx, Actor* thisx) {
    EnGo2* self = THIS;
    switch (self->actor.params & 0x1F) {
        case GORON_CITY_ROLLING_BIG:
            return EnGo2_GetStateGoronCityRollingBig(globalCtx, self);
        case GORON_CITY_LINK:
            return EnGo2_GetStateGoronCityLink(globalCtx, self);
        case GORON_DMT_BIGGORON:
            return EnGo2_GetStateGoronDmtBiggoron(globalCtx, self);
        case GORON_FIRE_GENERIC:
            return EnGo2_GetStateGoronFireGeneric(globalCtx, self);
        case GORON_DMT_BOMB_FLOWER:
            return EnGo2_GetStateGoronDmtBombFlower(globalCtx, self);
        case GORON_DMT_ROLLING_SMALL:
            return EnGo2_GetStateGoronDmtRollingSmall(globalCtx, self);
        case GORON_DMT_DC_ENTRANCE:
            return EnGo2_GetStateGoronDmtDcEntrance(globalCtx, self);
        case GORON_CITY_ENTRANCE:
            return EnGo2_GetStateGoronCityEntrance(globalCtx, self);
        case GORON_CITY_ISLAND:
            return EnGo2_GetStateGoronCityIsland(globalCtx, self);
        case GORON_CITY_LOWEST_FLOOR:
            return EnGo2_GetStateGoronCityLowestFloor(globalCtx, self);
        case GORON_CITY_STAIRWELL:
            return EnGo2_GetStateGoronCityStairwell(globalCtx, self);
        case GORON_CITY_LOST_WOODS:
            return EnGo2_GetStateGoronCityLostWoods(globalCtx, self);
        case GORON_DMT_FAIRY_HINT:
            return EnGo2_GetStateGoronDmtFairyHint(globalCtx, self);
        case GORON_MARKET_BAZAAR:
            return EnGo2_GetStateGoronMarketBazaar(globalCtx, self);
    }
#ifdef AVOID_UB
    return 0; // surpress warning
#endif
}

s32 func_80A44790(EnGo2* self, GlobalContext* globalCtx) {
    if ((self->actor.params & 0x1F) != GORON_DMT_BIGGORON && (self->actor.params & 0x1F) != GORON_CITY_ROLLING_BIG) {
        return func_800343CC(globalCtx, &self->actor, &self->unk_194.unk_00, self->unk_218, EnGo2_GetTextId,
                             EnGo2_GetState);
    } else if (((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) && ((self->collider.base.ocFlags2 & 1) == 0)) {
        return false;
    } else {
        if (func_8002F194(&self->actor, globalCtx)) {
            self->unk_194.unk_00 = 1;
            return true;
        } else if (self->unk_194.unk_00 != 0) {
            self->unk_194.unk_00 = EnGo2_GetState(globalCtx, &self->actor);
            return false;
        } else if (func_8002F2CC(&self->actor, globalCtx, self->unk_218)) {
            self->actor.textId = EnGo2_GetTextId(globalCtx, &self->actor);
        }
        return false;
    }
}

void EnGo2_SetColliderDim(EnGo2* self) {
    u8 index = self->actor.params & 0x1F;

    self->collider.dim.radius = D_80A4816C[index].radius;
    self->collider.dim.height = D_80A4816C[index].height;
}

void EnGo2_SetShape(EnGo2* self) {
    u8 index = self->actor.params & 0x1F;

    self->actor.shape.shadowScale = D_80A481F8[index].shape_unk_10;
    Actor_SetScale(&self->actor, D_80A481F8[index].scale);
    self->actor.targetMode = D_80A481F8[index].actor_unk_1F;
    self->unk_218 = D_80A481F8[index].unk_218;
    self->unk_218 += self->collider.dim.radius;
}

void EnGo2_CheckCollision(EnGo2* self, GlobalContext* globalCtx) {
    Vec3s pos;
    f32 xzDist;

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y;
    pos.z = self->actor.world.pos.z;
    xzDist = D_80A4816C[self->actor.params & 0x1F].xzDist;
    pos.x += (s16)(xzDist * Math_SinS(self->actor.shape.rot.y));
    pos.z += (s16)(xzDist * Math_CosS(self->actor.shape.rot.y));
    pos.y += D_80A4816C[self->actor.params & 0x1F].yDist;
    self->collider.dim.pos = pos;
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnGo2_SwapInitialFrameAnimFrameCount(EnGo2* self) {
    f32 initialFrame;

    initialFrame = self->skelAnime.startFrame;
    self->skelAnime.startFrame = self->skelAnime.endFrame;
    self->skelAnime.endFrame = initialFrame;
}

s32 func_80A44AB0(EnGo2* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 arg2;

    if ((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) {
        return false;
    } else {
        if ((self->actionFunc != EnGo2_SlowRolling) && (self->actionFunc != EnGo2_ReverseRolling) &&
            (self->actionFunc != EnGo2_ContinueRolling)) {
            return false;
        } else {
            if (self->collider.base.acFlags & 2) {
                Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                self->actor.flags &= ~0x1000000;
                self->collider.base.acFlags &= ~0x2;
                EnGo2_StopRolling(self, globalCtx);
                return true;
            }
            if (player->invincibilityTimer <= 0) {
                self->collider.base.ocFlags1 |= 8;
            } else {
                return false;
            }
            if (self->collider.base.ocFlags2 & 1) {
                self->collider.base.ocFlags2 &= ~1;

                arg2 = self->actionFunc == EnGo2_ContinueRolling ? 1.5f : self->actor.speedXZ * 1.5f;

                globalCtx->damagePlayer(globalCtx, -4);
                func_8002F71C(globalCtx, &self->actor, arg2, self->actor.yawTowardsPlayer, 6.0f);
                Audio_PlayActorSound2(&player->actor, NA_SE_PL_BODY_HIT);
                self->collider.base.ocFlags1 &= ~0x8;
            }
        }
    }
    return false;
}

s32 EnGo2_UpdateWaypoint(EnGo2* self, GlobalContext* globalCtx) {
    s32 change;

    if (self->path == NULL) {
        return 0;
    }

    change = (u8)(self->path->count - 1);
    if (self->reverse) {
        self->waypoint--;
        if (self->waypoint < 0) {
            self->waypoint = change - 1;
        }
    } else {
        self->waypoint++;
        if (self->waypoint >= change) {
            self->waypoint = 0;
        }
    }

    return 1;
}

s32 EnGo2_Orient(EnGo2* self, GlobalContext* globalCtx) {
    s16 targetYaw;
    f32 waypointDistSq = Path_OrientAndGetDistSq(&self->actor, self->path, self->waypoint, &targetYaw);

    Math_SmoothStepToS(&self->actor.world.rot.y, targetYaw, 6, 4000, 1);
    if (waypointDistSq > 0.0f && waypointDistSq < SQ(30.0f)) {
        return EnGo2_UpdateWaypoint(self, globalCtx);
    } else {
        return 0;
    }
}

s32 func_80A44D84(EnGo2* self) {
    s16 targetYaw;

    Path_OrientAndGetDistSq(&self->actor, self->path, self->waypoint, &targetYaw);
    self->actor.world.rot.y = targetYaw;
    return 1;
}

s32 EnGo2_IsWakingUp(EnGo2* self) {
    s16 yawDiff;
    f32 xyzDist = (self->actor.params & 0x1F) == GORON_DMT_BIGGORON ? 800.0f : 200.0f;
    f32 yDist = (self->actor.params & 0x1F) == GORON_DMT_BIGGORON ? 400.0f : 60.0f;
    s16 yawDiffAbs;

    if ((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) {
        if ((self->collider.base.ocFlags2 & 1) == 0) {
            self->actor.flags &= ~1;
            return false;
        } else {
            self->actor.flags |= 1;
            return true;
        }
    }

    xyzDist = SQ(xyzDist);
    yawDiff = (f32)self->actor.yawTowardsPlayer - (f32)self->actor.shape.rot.y;
    yawDiffAbs = ABS(yawDiff);
    if (self->actor.xyzDistToPlayerSq <= xyzDist && fabsf(self->actor.yDistToPlayer) < yDist && yawDiffAbs < 0x2AA8) {
        return true;
    } else {
        return false;
    }
}

s32 EnGo2_IsRollingOnGround(EnGo2* self, s16 arg1, f32 arg2, s16 arg3) {
    if ((self->actor.bgCheckFlags & 1) == 0 || self->actor.velocity.y > 0.0f) {
        return false;
    }

    if (DECR(self->unk_590)) {
        if (!arg3) {
            return true;
        } else {
            self->actor.world.pos.y =
                (self->unk_590 & 1) ? self->actor.world.pos.y + 1.5f : self->actor.world.pos.y - 1.5f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_BIGBALL_ROLL - SFX_FLAG);
            return true;
        }
    }

    if (self->unk_59C >= 2) {
        Audio_PlayActorSound2(&self->actor, (self->actor.params & 0x1F) == GORON_CITY_ROLLING_BIG
                                                ? NA_SE_EN_GOLON_LAND_BIG
                                                : NA_SE_EN_DODO_M_GND);
    }

    self->unk_59C--;
    if (self->unk_59C <= 0) {
        if (self->unk_59C == 0) {
            self->unk_590 = Rand_S16Offset(60, 30);
            self->unk_59C = 0;
            self->actor.velocity.y = 0.0f;
            return true;
        } else {
            self->unk_59C = arg1;
        }
    }

    self->actor.velocity.y = ((f32)self->unk_59C / (f32)arg1) * arg2;
    return true;
}

void EnGo2_BiggoronSetTextId(EnGo2* self, GlobalContext* globalCtx, Player* player) {
    u16 textId;

    if ((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) {
        if (gSaveContext.bgsFlag) {
            if (func_8002F368(globalCtx) == EXCH_ITEM_CLAIM_CHECK) {
                self->actor.textId = 0x3003;
            } else {
                self->actor.textId = 0x305E;
            }
            player->actor.textId = self->actor.textId;

        } else if (!gSaveContext.bgsFlag && (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_CLAIM_CHECK)) {
            if (func_8002F368(globalCtx) == EXCH_ITEM_CLAIM_CHECK) {
                if (Environment_GetBgsDayCount() >= 3) {
                    textId = 0x305E;
                } else {
                    textId = 0x305D;
                }
                self->actor.textId = textId;
            } else {
                if (Environment_GetBgsDayCount() >= 3) {
                    textId = 0x3002;
                } else {
                    textId = 0x305D;
                }
                self->actor.textId = textId;
            }
            player->actor.textId = self->actor.textId;

        } else if ((INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_PRESCRIPTION) &&
                   (INV_CONTENT(ITEM_TRADE_ADULT) <= ITEM_CLAIM_CHECK)) {
            if (func_8002F368(globalCtx) == EXCH_ITEM_EYEDROPS) {
                self->actor.textId = 0x3059;
            } else {
                self->actor.textId = 0x3058;
            }
            if (self->actor.textId == 0x3059) {
                gSaveContext.timer2State = 0;
            }
            player->actor.textId = self->actor.textId;

        } else if (INV_CONTENT(ITEM_TRADE_ADULT) <= ITEM_SWORD_BROKEN) {
            if (func_8002F368(globalCtx) == EXCH_ITEM_SWORD_BROKEN) {
                if (gSaveContext.infTable[11] & 0x10) {
                    textId = 0x3055;
                } else {
                    textId = 0x3054;
                }
                self->actor.textId = textId;
            } else {
                self->actor.textId = 0x3053;
            }
            player->actor.textId = self->actor.textId;
        } else {
            self->actor.textId = 0x3053;
            player->actor.textId = self->actor.textId;
        }
    }
}

void func_80A45288(EnGo2* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 linkAge;

    if (self->actionFunc != EnGo2_GoronFireGenericAction) {
        self->unk_194.unk_18 = player->actor.world.pos;
        linkAge = gSaveContext.linkAge;
        self->unk_194.unk_14 = D_80A482D8[self->actor.params & 0x1F][linkAge];
        func_80034A14(&self->actor, &self->unk_194, 4, self->unk_26E);
    }
    if ((self->actionFunc != EnGo2_SetGetItem) && (self->isAwake == true)) {
        if (func_80A44790(self, globalCtx)) {
            EnGo2_BiggoronSetTextId(self, globalCtx, player);
        }
    }
}

void func_80A45360(EnGo2* self, f32* alpha) {
    f32 alphaTarget =
        (self->skelAnime.animation == &gGoronAnim_004930) && (self->skelAnime.curFrame <= 32.0f) ? 0.0f : 255.0f;

    Math_ApproachF(alpha, alphaTarget, 0.4f, 100.0f);
    self->actor.shape.shadowAlpha = (u8)(u32)*alpha;
}

void EnGo2_RollForward(EnGo2* self) {
    f32 speedXZ = self->actor.speedXZ;

    if (self->unk_194.unk_00 != 0) {
        self->actor.speedXZ = 0.0f;
    }

    if (self->actionFunc != EnGo2_ContinueRolling) {
        Actor_MoveForward(&self->actor);
    }

    self->actor.speedXZ = speedXZ;
}

void func_80A454CC(EnGo2* self) {
    switch (self->actor.params & 0x1F) {
        case GORON_CITY_ROLLING_BIG:
        case GORON_DMT_DC_ENTRANCE:
        case GORON_CITY_ENTRANCE:
        case GORON_CITY_STAIRWELL:
        case GORON_DMT_FAIRY_HINT:
            func_80034EC0(&self->skelAnime, sAnimations, 9);
            break;
        case GORON_DMT_BIGGORON:
            if (INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_SWORD_BROKEN && INV_CONTENT(ITEM_TRADE_ADULT) <= ITEM_EYEDROPS) {
                func_80034EC0(&self->skelAnime, sAnimations, 4);
                break;
            }
        default:
            self->skelAnime.playSpeed = 0.0f;
            break;
    }
}

f32 EnGo2_GetTargetXZSpeed(EnGo2* self) {
    f32 yDist = (self->actor.params & 0x1F) == GORON_DMT_BIGGORON ? 400.0f : 60.0f;
    s32 index = self->actor.params & 0x1F;

    if (index == GORON_CITY_LINK && (fabsf(self->actor.yDistToPlayer) < yDist) &&
        (self->actor.xzDistToPlayer < 400.0f)) {
        return 9.0f;
    } else {
        return index == GORON_CITY_ROLLING_BIG ? 3.6000001f : 6.0f;
    }
}

s32 EnGo2_IsCameraModified(EnGo2* self, GlobalContext* globalCtx) {
    Camera* camera = globalCtx->cameraPtrs[MAIN_CAM];

    if ((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) {
        if (EnGo2_IsWakingUp(self)) {
            Camera_ChangeSetting(camera, CAM_SET_TEPPEN);
            func_8005AD1C(camera, 4);
        } else if (!EnGo2_IsWakingUp(self) && (camera->setting == CAM_SET_TEPPEN)) {
            Camera_ChangeSetting(camera, CAM_SET_DUNGEON1);
            func_8005ACFC(camera, 4);
        }
    }

    if ((self->actor.params & 0x1F) == GORON_FIRE_GENERIC || (self->actor.params & 0x1F) == GORON_CITY_ROLLING_BIG ||
        (self->actor.params & 0x1F) == GORON_CITY_STAIRWELL || (self->actor.params & 0x1F) == GORON_DMT_BIGGORON ||
        (self->actor.params & 0x1F) == GORON_MARKET_BAZAAR) {
        return true;
    } else if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1)) {
        return true;
    } else {
        return false;
    }
}

void EnGo2_DefaultWakingUp(EnGo2* self) {
    if (EnGo2_IsWakingUp(self)) {
        self->unk_26E = 2;
    } else {
        self->unk_26E = 1;
    }

    if (self->unk_194.unk_00 != 0) {
        self->unk_26E = 4;
    }

    self->isAwake = true;
}

void EnGo2_WakingUp(EnGo2* self) {
    f32 xyzDist = (self->actor.params & 0x1F) == GORON_DMT_BIGGORON ? 800.0f : 200.0f;
    s32 isTrue = true;

    xyzDist = SQ(xyzDist);
    self->unk_26E = 1;
    if ((self->actor.xyzDistToPlayerSq <= xyzDist) || (self->unk_194.unk_00 != 0)) {
        self->unk_26E = 4;
    }

    self->isAwake = isTrue;
}

void EnGo2_BiggoronWakingUp(EnGo2* self) {
    if (EnGo2_IsWakingUp(self) || self->unk_194.unk_00 != 0) {
        self->unk_26E = 2;
        self->isAwake = true;
    } else {
        self->unk_26E = 1;
        self->isAwake = false;
    }
}

void EnGo2_SelectGoronWakingUp(EnGo2* self) {
    switch (self->actor.params & 0x1F) {
        case GORON_DMT_BOMB_FLOWER:
            self->isAwake = true;
            self->unk_26E = EnGo2_IsWakingUp(self) ? 2 : 1;
            break;
        case GORON_FIRE_GENERIC:
            EnGo2_WakingUp(self);
            break;
        case GORON_DMT_BIGGORON:
            EnGo2_BiggoronWakingUp(self);
            break;
        case GORON_CITY_LINK:
            if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1)) {
                EnGo2_WakingUp(self);
                break;
            }
        default:
            EnGo2_DefaultWakingUp(self);
    }
}

void EnGo2_EyeMouthTexState(EnGo2* self) {
    switch (self->eyeMouthTexState) {
        case 1:
            self->blinkTimer = 0;
            self->eyeTexIndex = 0;
            self->mouthTexIndex = 0;
            break;
        case 2:
            self->blinkTimer = 0;
            self->eyeTexIndex = 1;
            self->mouthTexIndex = 0;
            break;
        // case 3 only when biggoron is given eyedrops. Biggoron smiles. (only use of second mouth texture)
        case 3:
            self->blinkTimer = 0;
            self->eyeTexIndex = 0;
            self->mouthTexIndex = 1;
            break;
        default:
            if (DECR(self->blinkTimer) == 0) {
                self->eyeTexIndex++;
                if (self->eyeTexIndex >= 4) {
                    self->blinkTimer = Rand_S16Offset(30, 30);
                    self->eyeTexIndex = 1;
                }
            }
    }
}

void EnGo2_SitDownAnimation(EnGo2* self) {
    if ((self->skelAnime.playSpeed != 0.0f) && (self->skelAnime.animation == &gGoronAnim_004930)) {
        if (self->skelAnime.playSpeed > 0.0f && self->skelAnime.curFrame == 14.0f) {
            if ((self->actor.params & 0x1F) != GORON_DMT_BIGGORON) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOLON_SIT_DOWN);
            } else {
                func_800F4524(&D_801333D4, NA_SE_EN_GOLON_SIT_DOWN, 60);
            }
        }
        if (self->skelAnime.playSpeed < 0.0f) {
            if (self->skelAnime.curFrame == 1.0f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
            }
            if (self->skelAnime.curFrame == 40.0f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOLON_SIT_DOWN);
            }
        }
    }
}

void EnGo2_GetDustData(EnGo2* self, s32 index2) {
    s32 index1 = (self->actor.params & 0x1F) == GORON_CITY_ROLLING_BIG ? 1 : 0;
    EnGo2DustEffectData* dustEffectData = &sDustEffectData[index1][index2];

    EnGo2_SpawnDust(self, dustEffectData->initialTimer, dustEffectData->scale, dustEffectData->scaleStep,
                    dustEffectData->numDustEffects, dustEffectData->radius, dustEffectData->yAccel);
}

void EnGo2_RollingAnimation(EnGo2* self, GlobalContext* globalCtx) {
    if ((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) {
        self->actor.flags &= ~1;
        func_80034EC0(&self->skelAnime, sAnimations, 10);
        self->skelAnime.playSpeed = -0.5f;
    } else {
        func_80034EC0(&self->skelAnime, sAnimations, 1);
        self->skelAnime.playSpeed = -1.0f;
    }
    EnGo2_SwapInitialFrameAnimFrameCount(self);
    self->unk_26E = 1;
    self->unk_211 = false;
    self->isAwake = false;
    self->actionFunc = EnGo2_CurledUp;
}

void EnGo2_WakeUp(EnGo2* self, GlobalContext* globalCtx) {
    if (self->skelAnime.playSpeed == 0.0f) {
        if ((self->actor.params & 0x1F) != GORON_DMT_BIGGORON) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOLON_WAKE_UP);
        } else {
            func_800F4524(&D_801333D4, NA_SE_EN_GOLON_WAKE_UP, 60);
        }
    }
    if ((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) {
        OnePointCutscene_Init(globalCtx, 4200, -99, &self->actor, MAIN_CAM);
        func_80034EC0(&self->skelAnime, sAnimations, 10);
        self->skelAnime.playSpeed = 0.5f;
    } else {
        func_80034EC0(&self->skelAnime, sAnimations, 1);
        self->skelAnime.playSpeed = 1.0f;
    }
    self->actionFunc = func_80A46B40;
}

void EnGo2_GetItemAnimation(EnGo2* self, GlobalContext* globalCtx) {
    func_80034EC0(&self->skelAnime, sAnimations, 1);
    self->unk_211 = true;
    self->actionFunc = func_80A46B40;
    self->skelAnime.playSpeed = 0.0f;
    self->actor.speedXZ = 0.0f;
    self->skelAnime.curFrame = self->skelAnime.endFrame;
}

void EnGo2_SetupRolling(EnGo2* self, GlobalContext* globalCtx) {
    if ((self->actor.params & 0x1F) == GORON_CITY_ROLLING_BIG || (self->actor.params & 0x1F) == GORON_CITY_LINK) {
        self->collider.info.bumperFlags = 1;
        self->actor.speedXZ = gSaveContext.infTable[17] & 0x4000 ? 6.0f : 3.6000001f;
    } else {
        self->actor.speedXZ = 6.0f;
    }
    self->actor.flags |= 0x1000000;
    self->animTimer = 10;
    self->actor.shape.yOffset = 1800.0f;
    self->actor.speedXZ += self->actor.speedXZ; // Speeding up
    self->actionFunc = EnGo2_ContinueRolling;
}

void EnGo2_StopRolling(EnGo2* self, GlobalContext* globalCtx) {
    EnBom* bomb;

    if (((self->actor.params & 0x1F) != GORON_CITY_ROLLING_BIG) && ((self->actor.params & 0x1F) != GORON_CITY_LINK)) {
        if ((self->actor.params & 0x1F) == GORON_DMT_ROLLING_SMALL) {
            bomb = (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, self->actor.world.pos.x,
                                       self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
            if (bomb != NULL) {
                bomb->timer = 0;
            }
        }
    } else {
        self->collider.info.bumperFlags = 0;
    }

    self->actor.shape.rot = self->actor.world.rot;
    self->unk_59C = 0;
    self->unk_590 = 0;
    self->actionFunc = EnGo2_GroundRolling;
    self->actor.shape.yOffset = 0.0f;
    self->actor.speedXZ = 0.0f;
}

s32 EnGo2_IsFreeingGoronInFire(EnGo2* self, GlobalContext* globalCtx) {
    if ((self->actor.params & 0x1F) != GORON_FIRE_GENERIC) {
        return false;
    }

    // shaking curled up
    self->actor.world.pos.x += (globalCtx->state.frames & 1) ? 1.0f : -1.0f;
    if (Flags_GetSwitch(globalCtx, (self->actor.params & 0xFC00) >> 0xA)) {
        return true;
    }
    return false;
}

s32 EnGo2_IsGoronDmtBombFlower(EnGo2* self) {
    if ((self->actor.params & 0x1F) != GORON_DMT_BOMB_FLOWER || self->unk_194.unk_00 != 2) {
        return false;
    }

    func_80034EC0(&self->skelAnime, sAnimations, 3);
    self->unk_194.unk_00 = 0;
    self->isAwake = false;
    self->unk_26E = 1;
    self->actionFunc = EnGo2_GoronDmtBombFlowerAnimation;
    return true;
}

s32 EnGo2_IsGoronRollingBig(EnGo2* self, GlobalContext* globalCtx) {
    if ((self->actor.params & 0x1F) != GORON_CITY_ROLLING_BIG || (self->unk_194.unk_00 != 2)) {
        return false;
    }
    self->unk_194.unk_00 = 0;
    EnGo2_RollingAnimation(self, globalCtx);
    self->actionFunc = EnGo2_GoronRollingBigContinueRolling;
    return true;
}

s32 EnGo2_IsGoronFireGeneric(EnGo2* self) {
    if ((self->actor.params & 0x1F) != GORON_FIRE_GENERIC || self->unk_194.unk_00 == 0) {
        return false;
    }
    self->actionFunc = EnGo2_GoronFireGenericAction;
    return true;
}

s32 EnGo2_IsGoronLinkReversing(EnGo2* self) {
    if ((self->actor.params & 0x1F) != GORON_CITY_LINK || (self->waypoint >= self->unk_216) ||
        !EnGo2_IsWakingUp(self)) {
        return false;
    }
    return true;
}

s32 EnGo2_IsRolling(EnGo2* self) {
    if (self->unk_194.unk_00 == 0 || self->actor.speedXZ < 1.0f) {
        return false;
    }
    if (EnGo2_IsRollingOnGround(self, 2, 20.0 / 3.0f, 0)) {
        if ((self->unk_590 >= 9) && (self->unk_59C == 0)) {
            self->unk_590 = 8;
        }
        EnGo2_GetDustData(self, 0);
    }
    return true;
}

void EnGo2_GoronLinkAnimation(EnGo2* self, GlobalContext* globalCtx) {
    s32 animation = 13;

    if ((self->actor.params & 0x1F) == GORON_CITY_LINK) {
        if ((self->actor.textId == 0x3035 && self->unk_20C == 0) ||
            (self->actor.textId == 0x3036 && self->unk_20C == 0)) {
            if (self->skelAnime.animation != &gGoronAnim_000D5C) {
                animation = 12;
                self->eyeMouthTexState = 0;
            }
        }

        if ((self->actor.textId == 0x3032 && self->unk_20C == 12) || (self->actor.textId == 0x3033) ||
            (self->actor.textId == 0x3035 && self->unk_20C == 6)) {
            if (self->skelAnime.animation != &gGoronAnim_000750) {
                animation = 11;
                self->eyeMouthTexState = 1;
            }
        }

        if (self->skelAnime.animation == &gGoronAnim_000750) {
            if (self->skelAnime.curFrame == 20.0f) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOLON_CRY);
            }
        }

        if (animation != 13) {
            func_80034EC0(&self->skelAnime, sAnimations, animation);
        }
    }
}

void EnGo2_GoronFireCamera(EnGo2* self, GlobalContext* globalCtx) {
    s16 yaw;

    self->camId = Gameplay_CreateSubCamera(globalCtx);
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
    Gameplay_ChangeCameraStatus(globalCtx, self->camId, CAM_STAT_ACTIVE);
    Path_CopyLastPoint(self->path, &self->at);
    yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->at) + 0xE38;
    self->eye.x = Math_SinS(yaw) * 100.0f + self->actor.world.pos.x;
    self->eye.z = Math_CosS(yaw) * 100.0f + self->actor.world.pos.z;
    self->eye.y = self->actor.world.pos.y + 20.0f;
    self->at.x = self->actor.world.pos.x;
    self->at.y = self->actor.world.pos.y + 40.0f;
    self->at.z = self->actor.world.pos.z;
    Gameplay_CameraSetAtEye(globalCtx, self->camId, &self->at, &self->eye);
}

void EnGo2_GoronFireClearCamera(EnGo2* self, GlobalContext* globalCtx) {
    Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
    Gameplay_ClearCamera(globalCtx, self->camId);
}

void EnGo2_BiggoronAnimation(EnGo2* self) {
    if (INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_SWORD_BROKEN && INV_CONTENT(ITEM_TRADE_ADULT) <= ITEM_EYEDROPS &&
        (self->actor.params & 0x1F) == GORON_DMT_BIGGORON && self->unk_194.unk_00 == 0) {
        if (DECR(self->animTimer) == 0) {
            self->animTimer = Rand_S16Offset(30, 30);
            func_800F4524(&D_801333D4, NA_SE_EN_GOLON_EYE_BIG, 60);
        }
    }
}

void EnGo2_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGo2* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 28.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGoronSkel, NULL, self->jointTable, self->morphTable, 18);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInfoInit);

    // Not GORON_CITY_ROLLING_BIG, GORON_CITY_LINK, GORON_DMT_BIGGORON
    switch (self->actor.params & 0x1F) {
        case GORON_FIRE_GENERIC:
        case GORON_DMT_BOMB_FLOWER:
        case GORON_DMT_ROLLING_SMALL:
        case GORON_DMT_DC_ENTRANCE:
        case GORON_CITY_ENTRANCE:
        case GORON_CITY_ISLAND:
        case GORON_CITY_LOWEST_FLOOR:
        case GORON_CITY_STAIRWELL:
        case GORON_CITY_LOST_WOODS:
        case GORON_DMT_FAIRY_HINT:
        case GORON_MARKET_BAZAAR:
            self->actor.flags &= ~0x10;
            self->actor.flags &= ~0x20;
    }

    EnGo2_SetColliderDim(self);
    EnGo2_SetShape(self);
    func_80034EC0(&self->skelAnime, sAnimations, 0);
    self->actor.gravity = -1.0f;
    self->alpha = self->actor.shape.shadowAlpha = 0;
    self->reverse = 0;
    self->isAwake = false;
    self->unk_211 = false;
    self->goronState = 0;
    self->waypoint = 0;
    self->unk_216 = self->actor.shape.rot.z;
    self->unk_26E = 1;
    self->path = Path_GetByIndex(globalCtx, (self->actor.params & 0x3E0) >> 5, 0x1F);
    switch (self->actor.params & 0x1F) {
        case GORON_CITY_ENTRANCE:
        case GORON_CITY_ISLAND:
        case GORON_CITY_LOWEST_FLOOR:
        case GORON_CITY_STAIRWELL:
        case GORON_CITY_LOST_WOODS:
            if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && LINK_IS_ADULT) {
                Actor_Kill(&self->actor);
            }
            self->actionFunc = EnGo2_CurledUp;
            break;
        case GORON_MARKET_BAZAAR:
            if ((LINK_IS_ADULT) || !CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
                Actor_Kill(&self->actor);
            }
            EnGo2_GetItemAnimation(self, globalCtx);
            break;
        case GORON_CITY_LINK:
            if ((gSaveContext.infTable[16] & 0x200)) {
                Path_CopyLastPoint(self->path, &self->actor.world.pos);
                self->actor.home.pos = self->actor.world.pos;
                if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE) && CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1)) {
                    EnGo2_GetItemAnimation(self, globalCtx);
                } else {
                    self->actionFunc = EnGo2_CurledUp;
                }
            } else {
                gSaveContext.infTable[16] &= ~0x1000;
                self->collider.dim.height = (D_80A4816C[self->actor.params & 0x1F].height * 0.6f);
                EnGo2_SetupRolling(self, globalCtx);
                self->isAwake = true;
            }
            break;
        case GORON_CITY_ROLLING_BIG:
        case GORON_DMT_ROLLING_SMALL:
            self->collider.dim.height = (D_80A4816C[self->actor.params & 0x1F].height * 0.6f);
            EnGo2_SetupRolling(self, globalCtx);
            break;
        case GORON_FIRE_GENERIC:
            if (Flags_GetSwitch(globalCtx, (self->actor.params & 0xFC00) >> 0xA)) {
                Actor_Kill(&self->actor);
            } else {
                self->isAwake = true;
                self->actionFunc = EnGo2_CurledUp;
            }
            break;
        case GORON_DMT_BIGGORON:
            self->actor.shape.shadowDraw = NULL;
            self->actor.flags &= ~1;
            if ((INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_SWORD_BROKEN) &&
                (INV_CONTENT(ITEM_TRADE_ADULT) <= ITEM_EYEDROPS)) {
                self->eyeMouthTexState = 1;
            }
            self->collider.base.acFlags = 0;
            self->collider.base.ocFlags1 = 0xD; // OC_PLAYER | OC_NO_PUSH | OC_ON
            self->actionFunc = EnGo2_CurledUp;
            break;
        case GORON_DMT_BOMB_FLOWER:
            if (gSaveContext.infTable[14] & 0x800) {
                Path_CopyLastPoint(self->path, &self->actor.world.pos);
                self->actor.home.pos = self->actor.world.pos;
            }
        case GORON_DMT_DC_ENTRANCE:
        case GORON_DMT_FAIRY_HINT:
        default:
            self->actionFunc = EnGo2_CurledUp;
    }
}

void EnGo2_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnGo2_CurledUp(EnGo2* self, GlobalContext* globalCtx) {
    u8 index = self->actor.params & 0x1F;
    s16 height;
    s32 quake;

    if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
        if ((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) {
            quake = Quake_Add(GET_ACTIVE_CAM(globalCtx), 3);
            Quake_SetSpeed(quake, -0x3CB0);
            Quake_SetQuakeValues(quake, 8, 0, 0, 0);
            Quake_SetCountdown(quake, 16);
        } else {
            EnGo2_GetDustData(self, 1);
        }
        self->skelAnime.playSpeed = 0.0f;
    }

    if ((s32)self->skelAnime.curFrame == 0) {
        self->collider.dim.height = (D_80A4816C[index].height * 0.6f);
    } else {
        height = D_80A4816C[index].height;
        self->collider.dim.height =
            ((D_80A4816C[index].height * 0.4f * (self->skelAnime.curFrame / self->skelAnime.startFrame)) +
             (height * 0.6f));
    }
    if (EnGo2_IsFreeingGoronInFire(self, globalCtx)) {
        self->isAwake = false;
        EnGo2_WakeUp(self, globalCtx);
    }
    if (((self->actor.params & 0x1F) != GORON_FIRE_GENERIC) && EnGo2_IsWakingUp(self)) {
        EnGo2_WakeUp(self, globalCtx);
    }
}

void func_80A46B40(EnGo2* self, GlobalContext* globalCtx) {
    u8 index = (self->actor.params & 0x1F);
    f32 height;

    if (self->unk_211 == true) {
        EnGo2_BiggoronAnimation(self);
        EnGo2_GoronLinkAnimation(self, globalCtx);
        EnGo2_SelectGoronWakingUp(self);

        if (!EnGo2_IsGoronRollingBig(self, globalCtx) && !EnGo2_IsGoronFireGeneric(self)) {
            if (EnGo2_IsGoronDmtBombFlower(self)) {
                return;
            }
        } else {
            return;
        }
    } else {
        if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
            if ((self->actor.params & 0x1F) == GORON_DMT_BIGGORON) {
                self->actor.flags |= 1;
            }
            func_80A454CC(self);
            self->unk_211 = true;
            self->collider.dim.height = D_80A4816C[index].height;
        } else {
            height = D_80A4816C[index].height;
            self->collider.dim.height =
                (s16)((height * 0.4f * (self->skelAnime.curFrame / self->skelAnime.endFrame)) + (height * 0.6f));
        }
    }
    if ((!EnGo2_IsCameraModified(self, globalCtx)) && (!EnGo2_IsWakingUp(self))) {
        EnGo2_RollingAnimation(self, globalCtx);
    }
}

void EnGo2_GoronDmtBombFlowerAnimation(EnGo2* self, GlobalContext* globalCtx) {
    f32 float1 = self->skelAnime.endFrame;
    f32 float2 = self->skelAnime.curFrame * ((f32)0x8000 / float1);

    self->actor.speedXZ = Math_SinS(float2);
    if ((EnGo2_Orient(self, globalCtx)) && (self->waypoint == 0)) {
        EnGo2_GetItemAnimation(self, globalCtx);
    }
}

void EnGo2_GoronRollingBigContinueRolling(EnGo2* self, GlobalContext* globalCtx) {
    if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
        EnGo2_GetDustData(self, 1);
        self->skelAnime.playSpeed = 0.0f;
        EnGo2_SetupRolling(self, globalCtx);
    }
}

void EnGo2_ContinueRolling(EnGo2* self, GlobalContext* globalCtx) {
    f32 float1 = 1000.0f;

    if (((self->actor.params & 0x1F) != GORON_DMT_ROLLING_SMALL || !(self->actor.xyzDistToPlayerSq > SQ(float1))) &&
        DECR(self->animTimer) == 0) {
        self->actionFunc = EnGo2_SlowRolling;
        self->actor.speedXZ *= 0.5f; // slowdown
    }
    EnGo2_GetDustData(self, 2);
}

void EnGo2_SlowRolling(EnGo2* self, GlobalContext* globalCtx) {
    s32 orientation;
    s32 index;

    if (!EnGo2_IsRolling(self)) {
        if (EnGo2_IsRollingOnGround(self, 4, 8.0f, 1) == true) {
            if (EnGo2_IsGoronLinkReversing(self)) {
                self->actionFunc = EnGo2_ReverseRolling;
                return;
            }
            EnGo2_GetDustData(self, 3);
        }
        orientation = EnGo2_Orient(self, globalCtx);
        index = self->actor.params & 0x1F;
        if (index != GORON_CITY_LINK) {
            if ((index == GORON_DMT_ROLLING_SMALL) && (orientation == 1) && (self->waypoint == 0)) {
                EnGo2_StopRolling(self, globalCtx);
                return;
            }
        } else if ((orientation == 2) && (self->waypoint == 1)) {
            EnGo2_StopRolling(self, globalCtx);
            return;
        }
        Math_ApproachF(&self->actor.speedXZ, EnGo2_GetTargetXZSpeed(self), 0.4f, 0.6f);
        self->actor.shape.rot = self->actor.world.rot;
    }
}

void EnGo2_GroundRolling(EnGo2* self, GlobalContext* globalCtx) {
    if (EnGo2_IsRollingOnGround(self, 4, 8.0f, 0)) {
        EnGo2_GetDustData(self, 0);
        if (self->unk_59C == 0) {
            switch (self->actor.params & 0x1F) {
                case GORON_CITY_LINK:
                    self->goronState = 0;
                    self->actionFunc = EnGo2_GoronLinkStopRolling;
                    break;
                case GORON_CITY_ROLLING_BIG:
                    EnGo2_WakeUp(self, globalCtx);
                    break;
                default:
                    self->actionFunc = EnGo2_CurledUp;
            }
        }
    }
}

void EnGo2_ReverseRolling(EnGo2* self, GlobalContext* globalCtx) {
    if (!EnGo2_IsRolling(self)) {
        Math_ApproachF(&self->actor.speedXZ, 0.0f, 0.6f, 0.8f);
        if (self->actor.speedXZ >= 1.0f) {
            EnGo2_GetDustData(self, 3);
        }
        if ((s32)self->actor.speedXZ == 0) {
            self->actor.world.rot.y ^= 0x8000;
            self->actor.shape.rot.y = self->actor.world.rot.y;
            self->reverse ^= 1;
            EnGo2_UpdateWaypoint(self, globalCtx);
            EnGo2_SetupRolling(self, globalCtx);
        }
    }
}

void EnGo2_SetupGetItem(EnGo2* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = EnGo2_SetGetItem;
    } else {
        func_8002F434(&self->actor, globalCtx, self->getItemId, self->actor.xzDistToPlayer + 1.0f,
                      fabsf(self->actor.yDistToPlayer) + 1.0f);
    }
}

void EnGo2_SetGetItem(EnGo2* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && func_80106BC8(globalCtx)) {
        self->unk_194.unk_00 = 0;
        switch (self->getItemId) {
            case GI_CLAIM_CHECK:
                Environment_ClearBgsDayCount();
                EnGo2_GetItemAnimation(self, globalCtx);
                return;
            case GI_TUNIC_GORON:
                gSaveContext.infTable[16] |= 0x200;
                EnGo2_GetItemAnimation(self, globalCtx);
                return;
            case GI_SWORD_BGS:
                gSaveContext.bgsFlag = true;
                break;
            case GI_BOMB_BAG_30:
            case GI_BOMB_BAG_40:
                EnGo2_RollingAnimation(self, globalCtx);
                self->actionFunc = EnGo2_GoronRollingBigContinueRolling;
                return;
        }
        self->actionFunc = func_80A46B40;
    }
}

void EnGo2_BiggoronEyedrops(EnGo2* self, GlobalContext* globalCtx) {
    switch (self->goronState) {
        case 0:
            func_80034EC0(&self->skelAnime, sAnimations, 5);
            self->actor.flags &= ~1;
            self->actor.shape.rot.y += 0x5B0;
            self->unk_26E = 1;
            self->animTimer = self->skelAnime.endFrame + 60.0f + 60.0f; // eyeDrops animation timer
            self->eyeMouthTexState = 2;
            self->unk_20C = 0;
            self->goronState++;
            func_800F483C(0x28, 5);
            OnePointCutscene_Init(globalCtx, 4190, -99, &self->actor, MAIN_CAM);
            break;
        case 1:
            if (DECR(self->animTimer)) {
                if (self->animTimer == 60 || self->animTimer == 120) {
                    func_8005B1A4(GET_ACTIVE_CAM(globalCtx));
                    func_800F4524(&D_801333D4, NA_SE_EV_GORON_WATER_DROP, 60);
                }
            } else {
                func_800F4524(&D_801333D4, NA_SE_EN_GOLON_GOOD_BIG, 60);
                func_80034EC0(&self->skelAnime, sAnimations, 6);
                func_8010B720(globalCtx, 0x305A);
                self->eyeMouthTexState = 3;
                self->goronState++;
                func_800F483C(0x7F, 5);
            }
            break;
        case 2:
            if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
                self->eyeMouthTexState = 0;
            }
            if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
                func_80034EC0(&self->skelAnime, sAnimations, 1);
                self->actor.flags |= 1;
                self->unk_26E = 2;
                self->skelAnime.playSpeed = 0.0f;
                self->skelAnime.curFrame = self->skelAnime.endFrame;
                EnGo2_GetItem(self, globalCtx, GI_CLAIM_CHECK);
                self->actionFunc = EnGo2_SetupGetItem;
                self->goronState = 0;
            }
            break;
    }
}

void EnGo2_GoronLinkStopRolling(EnGo2* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    switch (self->goronState) {
        case 0:
            if (func_8010BDBC(&globalCtx->msgCtx)) {
                return;
            } else {
                func_8010B680(globalCtx, 0x3031, NULL);
                player->actor.freezeTimer = 10;
                self->goronState++;
            }
        case 1:
            break;
        default:
            return;
    }

    if (func_8010BDBC(&globalCtx->msgCtx) != 2) {
        player->actor.freezeTimer = 10;
    } else {
        gSaveContext.infTable[16] |= 0x1000;
        self->unk_26E = 1;
        self->unk_211 = false;
        self->isAwake = false;
        self->actionFunc = EnGo2_CurledUp;
    }
}

void EnGo2_GoronFireGenericAction(EnGo2* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3s D_80A4854C = { 0x00, 0x00, 0x00 };

    switch (self->goronState) {
        case 0: // Wake up
            if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
                EnGo2_GoronFireCamera(self, globalCtx);
                globalCtx->msgCtx.msgMode = 0x37;
                func_80034EC0(&self->skelAnime, sAnimations, 2);
                self->waypoint = 1;
                self->skelAnime.playSpeed = 2.0f;
                func_80A44D84(self);
                self->actor.shape.rot = self->actor.world.rot;
                self->animTimer = 60;
                self->actor.gravity = 0.0f;
                self->actor.speedXZ = 2.0f;
                self->unk_194.unk_08 = D_80A4854C;
                self->unk_194.unk_0E = D_80A4854C;
                self->goronState++;
                self->goronState++;
                player->actor.world.rot.y = self->actor.world.rot.y;
                player->actor.shape.rot.y = self->actor.world.rot.y;
                player->actor.world.pos.x =
                    (f32)((Math_SinS(self->actor.world.rot.y) * -30.0f) + self->actor.world.pos.x);
                player->actor.world.pos.z =
                    (f32)((Math_CosS(self->actor.world.rot.y) * -30.0f) + self->actor.world.pos.z);
                func_8002DF54(globalCtx, &self->actor, 8);
                func_800F5C64(0x51);
            }
            break;
        case 2: // Walking away
            if (DECR(self->animTimer)) {
                if (!(self->animTimer % 8)) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_WALK);
                }
                Actor_MoveForward(&self->actor);
            } else {
                self->animTimer = 0;
                self->actor.speedXZ = 0.0f;
                if ((((self->actor.params & 0xFC00) >> 0xA) != 1) && (((self->actor.params & 0xFC00) >> 0xA) != 2) &&
                    (((self->actor.params & 0xFC00) >> 0xA) != 4) && (((self->actor.params & 0xFC00) >> 0xA) != 5) &&
                    (((self->actor.params & 0xFC00) >> 0xA) != 9) && (((self->actor.params & 0xFC00) >> 0xA) != 11)) {
                    self->goronState++;
                }
                self->goronState++;
            }
            break;
        case 3: // Walking away
            self->animTimer++;
            if (!(self->animTimer % 8) && (self->animTimer < 10)) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_WALK);
            }
            if (self->animTimer == 10) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_IRON_DOOR_OPEN);
            }
            if (self->animTimer > 44) {
                Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EV_IRON_DOOR_CLOSE);
            } else {
                break;
            }
        case 4: // Finalize walking away
            func_80106CCC(globalCtx);
            EnGo2_GoronFireClearCamera(self, globalCtx);
            func_8002DF54(globalCtx, &self->actor, 7);
            Actor_Kill(&self->actor);
            break;
        case 1:
            break;
    }
}

void EnGo2_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGo2* self = THIS;

    func_80A45360(self, &self->alpha);
    EnGo2_SitDownAnimation(self);
    SkelAnime_Update(&self->skelAnime);
    EnGo2_RollForward(self);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, (f32)self->collider.dim.height * 0.5f,
                            (f32)self->collider.dim.radius * 0.6f, 0.0f, 5);
    if (self->unk_194.unk_00 == 0) {
        func_80A44AB0(self, globalCtx);
    }
    self->actionFunc(self, globalCtx);
    if (self->unk_211 == true) {
        func_80034F54(globalCtx, self->unk_226, self->unk_24A, 18);
    }
    func_80A45288(self, globalCtx);
    EnGo2_EyeMouthTexState(self);
    EnGo2_CheckCollision(self, globalCtx);
}

s32 EnGo2_DrawCurledUp(EnGo2* self, GlobalContext* globalCtx) {
    Vec3f D_80A48554 = { 0.0f, 0.0f, 0.0f };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_go2.c", 2881);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_go2.c", 2884),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gGoronDL_00BD80);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_go2.c", 2889);
    Matrix_MultVec3f(&D_80A48554, &self->actor.focus.pos);

    return 1;
}

s32 EnGo2_DrawRolling(EnGo2* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f D_80A48560 = { 0.0f, 0.0f, 0.0f };
    f32 speedXZ;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_go2.c", 2914);
    func_80093D18(globalCtx->state.gfxCtx);
    speedXZ = self->actionFunc == EnGo2_ReverseRolling ? 0.0f : self->actor.speedXZ;
    Matrix_RotateRPY((globalCtx->state.frames * ((s16)speedXZ * 1400)), 0, self->actor.shape.rot.z, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_go2.c", 2926),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gGoronDL_00C140);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_go2.c", 2930);
    Matrix_MultVec3f(&D_80A48560, &self->actor.focus.pos);
    return 1;
}

s32 EnGo2_OverrideLimbDraw(GlobalContext* globalCtx, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnGo2* self = THIS;
    Vec3s vec1;
    f32 float1;

    if (limb == 17) {
        Matrix_Translate(2800.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        vec1 = self->unk_194.unk_08;
        float1 = (vec1.y / (f32)0x8000) * M_PI;
        Matrix_RotateX(float1, MTXMODE_APPLY);
        float1 = (vec1.x / (f32)0x8000) * M_PI;
        Matrix_RotateZ(float1, MTXMODE_APPLY);
        Matrix_Translate(-2800.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }
    if (limb == 10) {
        vec1 = self->unk_194.unk_0E;
        float1 = (vec1.y / (f32)0x8000) * M_PI;
        Matrix_RotateY(float1, MTXMODE_APPLY);
        float1 = (vec1.x / (f32)0x8000) * M_PI;
        Matrix_RotateX(float1, MTXMODE_APPLY);
    }
    if ((limb == 10) || (limb == 11) || (limb == 14)) {
        float1 = Math_SinS(self->unk_226[limb]);
        rot->y += float1 * 200.0f;
        float1 = Math_CosS(self->unk_24A[limb]);
        rot->z += float1 * 200.0f;
    }
    return 0;
}

void EnGo2_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnGo2* self = THIS;
    Vec3f D_80A4856C = { 600.0f, 0.0f, 0.0f };

    if (limbIndex == 17) {
        Matrix_MultVec3f(&D_80A4856C, &self->actor.focus.pos);
    }
}

void EnGo2_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnGo2* self = THIS;
    void* eyeTextures[] = { gGoronCsEyeClosed2Tex, gGoronCsEyeOpenTex, gGoronCsEyeHalfTex, gGoronCsEyeClosedTex };
    void* mouthTextures[] = { gGoronCsMouthNeutralTex, gGoronCsMouthSmileTex };

    EnGo2_UpdateDust(self);
    Matrix_Push();
    EnGo2_DrawDust(self, globalCtx);
    Matrix_Pop();

    if ((self->actionFunc == EnGo2_CurledUp) && (self->skelAnime.playSpeed == 0.0f) &&
        (self->skelAnime.curFrame == 0.0f)) {
        if (1) {}
        EnGo2_DrawCurledUp(self, globalCtx);
    } else if (self->actionFunc == EnGo2_SlowRolling || self->actionFunc == EnGo2_ReverseRolling ||
               self->actionFunc == EnGo2_ContinueRolling) {
        EnGo2_DrawRolling(self, globalCtx);
    } else {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_go2.c", 3063);
        func_80093D18(globalCtx->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTexIndex]));
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(mouthTextures[self->mouthTexIndex]));

        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnGo2_OverrideLimbDraw, EnGo2_PostLimbDraw, self);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_go2.c", 3081);
    }
}
