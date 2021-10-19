/*
 * File: z_en_elf.c
 * Overlay: ovl_En_Elf
 * Description: Fairy
 */

#include "z_en_elf.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x02000030

#define THIS ((EnElf*)thisx)

#define FAIRY_FLAG_TIMED (1 << 8)
#define FAIRY_FLAG_BIG (1 << 9)

void EnElf_Init(Actor* thisx, GlobalContext* globalCtx);
void EnElf_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnElf_Update(Actor* thisx, GlobalContext* globalCtx);
void EnElf_Draw(Actor* thisx, GlobalContext* globalCtx);
void func_80A053F0(Actor* thisx, GlobalContext* globalCtx);
void func_80A052F4(Actor* thisx, GlobalContext* globalCtx);
void func_80A05208(Actor* thisx, GlobalContext* globalCtx);
void func_80A05188(Actor* thisx, GlobalContext* globalCtx);
void func_80A05114(Actor* thisx, GlobalContext* globalCtx);
void func_80A05040(Actor* thisx, GlobalContext* globalCtx);

// Navi
void func_80A03CF8(EnElf* self, GlobalContext* globalCtx);

// Healing Fairies
void func_80A0329C(EnElf* self, GlobalContext* globalCtx);
void func_80A03610(EnElf* self, GlobalContext* globalCtx);

// Healing Fairies Revive From Death
void func_80A03990(EnElf* self, GlobalContext* globalCtx);
void func_80A03814(EnElf* self, GlobalContext* globalCtx);

// Kokiri Fairies
void func_80A0353C(EnElf* self, GlobalContext* globalCtx);

// Fairy Spawner
void func_80A03604(EnElf* self, GlobalContext* globalCtx);

// Move(?) functions
void func_80A0214C(EnElf* self, GlobalContext* globalCtx);
void func_80A02AA4(EnElf* self, GlobalContext* globalCtx);
void func_80A02A20(EnElf* self, GlobalContext* globalCtx);
void func_80A02B38(EnElf* self, GlobalContext* globalCtx);
void func_80A020A4(EnElf* self, GlobalContext* globalCtx);
void func_80A01FE0(EnElf* self, GlobalContext* globalCtx);

// misc
void func_80A04414(EnElf* self, GlobalContext* globalCtx);
void func_80A0461C(EnElf* self, GlobalContext* globalCtx);
void EnElf_SpawnSparkles(EnElf* self, GlobalContext* globalCtx, s32 sparkleLife);
void EnElf_GetCutsceneNextPos(Vec3f* vec, GlobalContext* globalCtx, s32 action);

const ActorInit En_Elf_InitVars = {
    ACTOR_EN_ELF,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnElf),
    (ActorFunc)EnElf_Init,
    (ActorFunc)EnElf_Destroy,
    (ActorFunc)EnElf_Update,
    (ActorFunc)EnElf_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 8, ICHAIN_STOP),
};

static Color_RGBAf sInnerColors[] = {
    { 255.0f, 255.0f, 255.0f, 255.0f },
    { 255.0f, 220.0f, 220.0f, 255.0f },
};

static Color_RGBAf sOuterColors[] = {
    { 255.0f, 255.0f, 255.0f, 255.0f },
    { 255.0f, 50.0f, 100.0f, 255.0f },
};

typedef struct {
    u8 r, g, b;
} FairyColorFlags;

static FairyColorFlags sColorFlags[] = {
    { 0, 0, 0 }, { 1, 0, 0 }, { 1, 2, 0 }, { 1, 0, 2 }, { 0, 1, 0 }, { 2, 1, 0 }, { 0, 1, 2 },
    { 0, 0, 1 }, { 2, 0, 1 }, { 0, 2, 1 }, { 1, 1, 0 }, { 1, 0, 1 }, { 0, 1, 1 },
};

void EnElf_SetupAction(EnElf* self, EnElfActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void func_80A01C38(EnElf* self, s32 arg1) {
    self->unk_2A8 = arg1;

    switch (self->unk_2A8) {
        case 0:
            self->unk_2AE = 0x400;
            self->unk_2B0 = 0x200;
            self->func_2C8 = func_80A02A20;
            self->unk_2C0 = 100;
            self->unk_2B4 = 5.0f;
            self->unk_2B8 = 20.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
        case 12:
            self->unk_2AE = 0x400;
            self->unk_2B0 = 0x200;
            self->func_2C8 = func_80A02A20;
            self->unk_2C0 = 100;
            self->unk_2B4 = 1.0f;
            self->unk_2B8 = 5.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
        case 10:
            self->unk_2AE = 0x400;
            self->unk_2B0 = 0;
            self->func_2C8 = func_80A02A20;
            self->unk_2B8 = 0.0f;
            self->unk_2B4 = 5.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
        case 9:
            self->unk_2AE = 0x1000;
            self->unk_2B0 = 0x200;
            self->func_2C8 = func_80A02A20;
            self->unk_2B4 = 3.0f;
            self->unk_2B8 = 10.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
        case 7:
            self->func_2C8 = func_80A02A20;
            self->unk_2AE = 0x1E;
            self->unk_2C0 = 1;
            self->unk_2B4 = 0.0f;
            self->unk_2B8 = 0.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
        case 8:
            self->unk_2AE = 0x1000;
            self->unk_2B0 = 0x200;
            self->func_2C8 = func_80A02A20;
            self->unk_2B4 = 0.0f;
            self->unk_2B8 = 0.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
        case 1:
            self->unk_2AE = 0x1000;
            self->unk_2B0 = 0x800;
            self->func_2C8 = func_80A02A20;
            self->unk_2B4 = 5.0f;
            self->unk_2B8 = 7.5f;
            self->skelAnime.playSpeed = 2.0f;
            break;
        case 2:
            self->unk_2AE = 0x400;
            self->unk_2B0 = 0x1000;
            self->func_2C8 = func_80A02AA4;
            self->unk_2B4 = 10.0f;
            self->unk_2B8 = 20.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
        case 3:
            self->unk_2B0 = 0x600;
            self->func_2C8 = func_80A02B38;
            self->unk_2B8 = 1.0f;
            self->unk_2B4 = 1.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
        case 4:
            self->unk_2B0 = 0x800;
            self->func_2C8 = func_80A02B38;
            self->unk_2B4 = 20.0f;
            self->unk_2B8 = 10.0;
            self->skelAnime.playSpeed = 2.0f;
            break;
        case 5:
            self->unk_2B0 = 0x200;
            self->func_2C8 = func_80A02B38;
            self->unk_2B4 = 10.0f;
            self->unk_2B8 = 10.0f;
            self->skelAnime.playSpeed = 0.5f;
            break;
        case 6:
            self->unk_2AE = 0x1000;
            self->unk_2B0 = 0x800;
            self->func_2C8 = func_80A02A20;
            self->unk_2B4 = 60.0f;
            self->unk_2B8 = 20.0f;
            self->skelAnime.playSpeed = 2.0f;
            break;
        case 11:
            self->unk_2AE = 0x400;
            self->unk_2B0 = 0x2000;
            self->func_2C8 = func_80A02A20;
            self->unk_2C0 = 42;
            self->unk_2B4 = 5.0f;
            self->unk_2B8 = 1.0f;
            self->skelAnime.playSpeed = 1.0f;
            break;
    }
}

s32 func_80A01F90(Vec3f* self, Vec3f* arg1, f32 arg2) {
    return SQ(arg2) < (SQ(arg1->x - self->x) + SQ(arg1->z - self->z));
}

void func_80A01FE0(EnElf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (!func_80A01F90(&self->actor.world.pos, &player->actor.world.pos, 30.0f)) {
        self->unk_2B8 = 0.5f;
    } else {
        self->unk_2B8 = 2.0f;
    }

    if (self->unk_2C0 > 0) {
        self->unk_2C0--;
    } else {
        self->unk_2A8 = 1;
        self->unk_2AC = 0x80;
        self->unk_2B8 = Rand_ZeroFloat(1.0f) + 0.5f;
        self->unk_2B0 = Rand_CenteredFloat(32767.0f);
        self->func_2C8 = func_80A0214C;
    }
}

void func_80A020A4(EnElf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (func_80A01F90(&self->actor.world.pos, &player->actor.world.pos, 50.0f)) {
        if (self->unk_2C0 > 0) {
            self->unk_2C0--;
        } else {
            self->unk_2A8 = 1;
            self->unk_2AC = 0x80;
            self->unk_2B8 = Rand_ZeroFloat(1.0f) + 0.5f;
            self->unk_2B0 = Rand_CenteredFloat(32767.0f);
            self->func_2C8 = func_80A0214C;
        }
    }
}

void func_80A0214C(EnElf* self, GlobalContext* globalCtx) {
    f32 xzDistToPlayer;

    if (self->unk_2C0 > 0) {
        self->unk_2C0--;
    } else {
        xzDistToPlayer = self->actor.xzDistToPlayer;
        if (xzDistToPlayer < 50.0f) {
            if (Rand_ZeroOne() < 0.2f) {
                self->unk_2A8 = 2;
                self->unk_2AC = 0x400;
                self->unk_2B8 = 2.0f;
                self->func_2C8 = func_80A020A4;
                self->actor.speedXZ = 1.5f;
                self->unk_2C0 = (s16)Rand_ZeroFloat(8.0f) + 4;
            } else {
                self->unk_2C0 = 10;
            }
        } else {
            if (xzDistToPlayer > 150.0f) {
                xzDistToPlayer = 150.0f;
            }

            xzDistToPlayer = ((xzDistToPlayer - 50.0f) * 0.95f) + 0.05f;

            if (Rand_ZeroOne() < xzDistToPlayer) {
                self->unk_2A8 = 3;
                self->unk_2AC = 0x200;
                self->unk_2B8 = (xzDistToPlayer * 2.0f) + 1.0f;
                self->func_2C8 = func_80A01FE0;
                self->unk_2C0 = (s16)Rand_ZeroFloat(16.0f) + 0x10;
            } else {
                self->unk_2C0 = 10;
            }
        }
    }

    if (Rand_ZeroOne() < 0.1f) {
        self->unk_2A8 = 1;
        self->unk_2AC = 0x80;
        self->unk_2B8 = Rand_ZeroFloat(0.5f) + 0.5f;
        self->unk_2B0 = Rand_CenteredFloat(32767.0f);
    }
}

void func_80A0232C(EnElf* self, GlobalContext* globalCtx) {
    if (func_80A01F90(&self->unk_28C, &self->actor.world.pos, 100.0f)) {
        self->unk_2A8 = 0;
        self->unk_2AC = 0x200;
        self->func_2C8 = func_80A0214C;
        self->unk_2B8 = 1.5f;
    } else {
        self->func_2C8(self, globalCtx);
    }
}

f32 EnElf_GetColorValue(s32 colorFlag) {
    switch (colorFlag) {
        case 1:
            return Rand_ZeroFloat(55.0f) + 200.0f;
        case 2:
            return Rand_ZeroFloat(255.0f);
        default:
            return 0.0f;
    }
}

void EnElf_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnElf* self = THIS;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 colorConfig;
    s32 i;

    Actor_ProcessInitChain(thisx, sInitChain);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gFairySkel, &gFairyAnim, self->jointTable, self->morphTable, 15);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 15.0f);
    thisx->shape.shadowAlpha = 0xFF;

    Lights_PointGlowSetInfo(&self->lightInfoGlow, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 255, 255,
                            255, 0);
    self->lightNodeGlow = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfoGlow);

    Lights_PointNoGlowSetInfo(&self->lightInfoNoGlow, thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, 255,
                              255, 255, 0);
    self->lightNodeNoGlow = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfoNoGlow);

    self->fairyFlags = 0;
    self->disappearTimer = 600;
    self->unk_2A4 = 0.0f;
    colorConfig = 0;

    switch (thisx->params) {
        case FAIRY_NAVI:
            thisx->room = -1;
            EnElf_SetupAction(self, func_80A03CF8);
            func_80A01C38(self, 0);
            self->fairyFlags |= 4;
            thisx->update = func_80A053F0;
            self->elfMsg = NULL;
            self->unk_2C7 = 0x14;

            if ((gSaveContext.naviTimer >= 25800) || (gSaveContext.naviTimer < 3000)) {
                gSaveContext.naviTimer = 0;
            }
            break;
        case FAIRY_REVIVE_BOTTLE:
            colorConfig = -1;
            EnElf_SetupAction(self, func_80A03610);
            self->unk_2B8 = Math_Vec3f_DistXZ(&thisx->world.pos, &player->actor.world.pos);
            self->unk_2AC = player->actor.shape.rot.y;
            self->unk_2B0 = -0x1000;
            self->unk_28C.y = thisx->world.pos.y - player->actor.world.pos.y;
            self->unk_2AA = 0;
            self->unk_2B4 = 0.0f;
            break;
        case FAIRY_REVIVE_DEATH:
            colorConfig = -1;
            EnElf_SetupAction(self, func_80A03990);
            self->unk_2B8 = 0.0f;
            self->unk_2AC = player->actor.shape.rot.y;
            self->unk_2B0 = 0;
            self->unk_28C.y = thisx->world.pos.y - player->actor.world.pos.y;
            self->unk_2AA = 0;
            self->unk_2B4 = 7.0f;
            break;
        case FAIRY_HEAL_BIG:
            self->fairyFlags |= FAIRY_FLAG_BIG;
            thisx->shape.shadowDraw = ActorShadow_DrawWhiteCircle;
        case FAIRY_HEAL_TIMED:
            self->fairyFlags |= FAIRY_FLAG_TIMED;
        case FAIRY_HEAL:
            colorConfig = -1;
            EnElf_SetupAction(self, func_80A0329C);
            self->unk_2B4 = Rand_ZeroFloat(10.0f) + 10.0f;
            self->unk_2AA = 0;
            self->unk_2AE = (s16)(Rand_ZeroFloat(1048.0f)) + 0x200;
            self->unk_28C = thisx->world.pos;
            self->unk_2BC = Rand_CenteredFloat(32767.0f);
            self->func_2C8 = func_80A0214C;
            func_80A0232C(self, globalCtx);
            self->unk_2C0 = 0;
            self->disappearTimer = 240;
            break;
        case FAIRY_KOKIRI:
            colorConfig = Rand_ZeroFloat(11.99f) + 1.0f;
            EnElf_SetupAction(self, func_80A0353C);
            func_80A01C38(self, 0);
            break;
        case FAIRY_SPAWNER:
            EnElf_SetupAction(self, func_80A03604);
            func_80A01C38(self, 8);

            for (i = 0; i < 8; i++) {
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ELF, thisx->world.pos.x,
                            thisx->world.pos.y - 30.0f, thisx->world.pos.z, 0, 0, 0, FAIRY_HEAL);
            }
            break;
        default:
            ASSERT(0, "0", "../z_en_elf.c", 1103);
            break;
    }

    self->unk_2A0 = 3.0f;
    self->innerColor = sInnerColors[0];

    if (colorConfig > 0) {
        self->outerColor.r = EnElf_GetColorValue(sColorFlags[colorConfig].r);
        self->outerColor.g = EnElf_GetColorValue(sColorFlags[colorConfig].g);
        self->outerColor.b = EnElf_GetColorValue(sColorFlags[colorConfig].b);
        self->outerColor.a = 0.0f;
    } else {
        self->innerColor = sInnerColors[-colorConfig];
        self->outerColor = sOuterColors[-colorConfig];
    }
}

void func_80A0299C(EnElf* self, s32 arg1) {
}

void func_80A029A8(EnElf* self, s16 increment) {
    if (self->disappearTimer < 600) {
        self->disappearTimer += increment;
    }
}

void EnElf_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnElf* self = THIS;

    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNodeGlow);
    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNodeNoGlow);
}

void func_80A02A20(EnElf* self, GlobalContext* globalCtx) {
    self->unk_28C.x = Math_SinS(self->unk_2AC) * self->unk_2B8;
    self->unk_28C.y = Math_SinS(self->unk_2AA) * self->unk_2B4;
    self->unk_28C.z = Math_CosS(self->unk_2AC) * self->unk_2B8;
    self->unk_2AC += self->unk_2B0;
    self->unk_2AA += self->unk_2AE;
}

void func_80A02AA4(EnElf* self, GlobalContext* globalCtx) {
    f32 xzScale;

    xzScale = (Math_CosS(self->unk_2AA) * self->unk_2B4) + self->unk_2B8;

    self->unk_28C.x = Math_SinS(self->unk_2AC) * xzScale;
    self->unk_28C.y = 0.0f;
    self->unk_28C.z = Math_CosS(self->unk_2AC) * xzScale;

    self->unk_2AC += self->unk_2B0;
    self->unk_2AA += self->unk_2AE;
}

void func_80A02B38(EnElf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_2AA = (self->unk_2AC * 2) & 0xFFFF;
    self->unk_28C.x = Math_SinS(self->unk_2AC) * self->unk_2B8;
    self->unk_28C.y = Math_SinS(self->unk_2AA) * self->unk_2B4;
    self->unk_28C.z = -Math_SinS(player->actor.shape.rot.y) * self->unk_28C.x;
    self->unk_28C.x = Math_CosS(player->actor.shape.rot.y) * self->unk_28C.x;
    self->unk_2AC += self->unk_2B0;
}

void func_80A02BD8(EnElf* self, Vec3f* targetPos, f32 arg2) {
    f32 yVelTarget;
    f32 yVelDirection;

    yVelTarget = ((targetPos->y + self->unk_28C.y) - self->actor.world.pos.y) * arg2;
    yVelDirection = (yVelTarget >= 0.0f) ? 1.0f : -1.0f;
    yVelTarget = fabsf(yVelTarget);
    yVelTarget = CLAMP(yVelTarget, 0.0f, 20.0f) * yVelDirection;
    Math_StepToF(&self->actor.velocity.y, yVelTarget, 32.0f);
}

void func_80A02C98(EnElf* self, Vec3f* targetPos, f32 arg2) {
    f32 xVelTarget;
    f32 zVelTarget;
    f32 xVelDirection;
    f32 zVelDirection;

    xVelTarget = ((targetPos->x + self->unk_28C.x) - self->actor.world.pos.x) * arg2;
    zVelTarget = ((targetPos->z + self->unk_28C.z) - self->actor.world.pos.z) * arg2;

    xVelDirection = (xVelTarget >= 0.0f) ? 1.0f : -1.0f;
    zVelDirection = (zVelTarget >= 0.0f) ? 1.0f : -1.0f;

    xVelTarget = fabsf(xVelTarget);
    zVelTarget = fabsf(zVelTarget);

    xVelTarget = CLAMP(xVelTarget, 0.0f, 20.0f) * xVelDirection;
    zVelTarget = CLAMP(zVelTarget, 0.0f, 20.0f) * zVelDirection;

    func_80A02BD8(self, targetPos, arg2);
    Math_StepToF(&self->actor.velocity.x, xVelTarget, 1.5f);
    Math_StepToF(&self->actor.velocity.z, zVelTarget, 1.5f);
    func_8002D7EC(&self->actor);
}

void func_80A02E30(EnElf* self, Vec3f* targetPos) {
    func_80A02BD8(self, targetPos, 0.2f);
    self->actor.velocity.x = (targetPos->x + self->unk_28C.x) - self->actor.world.pos.x;
    self->actor.velocity.z = (targetPos->z + self->unk_28C.z) - self->actor.world.pos.z;
    func_8002D7EC(&self->actor);
    self->actor.world.pos.x = targetPos->x + self->unk_28C.x;
    self->actor.world.pos.z = targetPos->z + self->unk_28C.z;
}

void func_80A02EC0(EnElf* self, Vec3f* targetPos) {
    func_80A02BD8(self, targetPos, 0.2f);
    self->actor.velocity.x = self->actor.velocity.z = 0.0f;
    func_8002D7EC(&self->actor);
    self->actor.world.pos.x = targetPos->x + self->unk_28C.x;
    self->actor.world.pos.z = targetPos->z + self->unk_28C.z;
}

void func_80A02F2C(EnElf* self, Vec3f* targetPos) {
    f32 yVelTarget;
    f32 yVelDirection;

    yVelTarget = (((Math_SinS(self->unk_2AA) * self->unk_2B4) + targetPos->y) - self->actor.world.pos.y) * 0.2f;
    yVelDirection = (yVelTarget >= 0.0f) ? 1.0f : -1.0f;
    self->unk_2AA += self->unk_2AE;
    yVelTarget = fabsf(yVelTarget);
    yVelTarget = CLAMP(yVelTarget, 0.0f, 20.0f) * yVelDirection;
    Math_StepToF(&self->actor.velocity.y, yVelTarget, 1.5f);
}

void func_80A03018(EnElf* self, GlobalContext* globalCtx) {
    s32 pad[2];
    Player* player = GET_PLAYER(globalCtx);
    s16 targetYaw;
    Vec3f* unk_28C = &self->unk_28C;

    Math_SmoothStepToF(&self->actor.speedXZ, self->unk_2B8, 0.2f, 0.5f, 0.01f);

    switch (self->unk_2A8) {
        case 0:
            targetYaw = Math_Atan2S(-(self->actor.world.pos.z - unk_28C->z), -(self->actor.world.pos.x - unk_28C->x));
            break;

        case 3:
            targetYaw = Math_Atan2S(-(self->actor.world.pos.z - player->actor.world.pos.z),
                                    -(self->actor.world.pos.x - player->actor.world.pos.x));
            break;

        case 2:
            targetYaw = Math_Atan2S(self->actor.world.pos.z - player->actor.world.pos.z,
                                    self->actor.world.pos.x - player->actor.world.pos.x);
            break;

        default:
            targetYaw = self->unk_2B0;
            break;
    }

    Math_SmoothStepToS(&self->unk_2BC, targetYaw, 10, self->unk_2AC, 0x20);
    self->actor.world.rot.y = self->unk_2BC;
    Actor_MoveForward(&self->actor);
}

void func_80A03148(EnElf* self, Vec3f* arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 xVelTarget;
    f32 zVelTarget;
    f32 xzVelocity;
    f32 clampedXZ;

    xVelTarget = ((arg1->x + self->unk_28C.x) - self->actor.world.pos.x) * arg4;
    zVelTarget = ((arg1->z + self->unk_28C.z) - self->actor.world.pos.z) * arg4;
    arg4 += 0.3f;
    arg3 += 30.0f;

    func_80A02BD8(self, arg1, arg4);

    xzVelocity = sqrtf(SQ(xVelTarget) + SQ(zVelTarget));

    self->actor.speedXZ = clampedXZ = CLAMP(xzVelocity, arg2, arg3);

    if ((xzVelocity != clampedXZ) && (xzVelocity != 0.0f)) {
        xzVelocity = clampedXZ / xzVelocity;
        xVelTarget *= xzVelocity;
        zVelTarget *= xzVelocity;
    }

    Math_StepToF(&self->actor.velocity.x, xVelTarget, 5.0f);
    Math_StepToF(&self->actor.velocity.z, zVelTarget, 5.0f);
    func_8002D7EC(&self->actor);
}

void func_80A0329C(EnElf* self, GlobalContext* globalCtx) {
    Player* refActor = GET_PLAYER(globalCtx);
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    f32 heightDiff;

    SkelAnime_Update(&self->skelAnime);

    if (Rand_ZeroOne() < 0.05f) {
        self->unk_2B4 = Rand_ZeroFloat(10.0f) + 10.0f;
        self->unk_2AE = (s16)(Rand_ZeroFloat(1024.0f)) + 0x200;
    }

    func_80A0232C(self, globalCtx);
    self->unk_28C.y = player->bodyPartsPos[0].y;
    func_80A02F2C(self, &self->unk_28C);
    func_80A03018(self, globalCtx);

    if ((self->unk_2A8 == 2) || (self->unk_2A8 == 3)) {
        EnElf_SpawnSparkles(self, globalCtx, 16);
    }

    if (Actor_HasParent(&self->actor, globalCtx)) {
        Actor_Kill(&self->actor);
        return;
    }

    if (!Player_InCsMode(globalCtx)) {
        heightDiff = self->actor.world.pos.y - refActor->actor.world.pos.y;

        if ((heightDiff > 0.0f) && (heightDiff < 60.0f)) {
            if (!func_80A01F90(&self->actor.world.pos, &refActor->actor.world.pos, 10.0f)) {
                Health_ChangeBy(globalCtx, 128);
                if (self->fairyFlags & FAIRY_FLAG_BIG) {
                    Magic_Fill(globalCtx);
                }
                self->unk_2B8 = 50.0f;
                self->unk_2AC = refActor->actor.shape.rot.y;
                self->unk_2B0 = -0x1000;
                self->unk_28C.y = 30.0f;
                self->unk_2B4 = 0.0f;
                self->unk_2AA = 0;
                EnElf_SetupAction(self, func_80A03610);
                return;
            }
        }

        if (self->fairyFlags & FAIRY_FLAG_TIMED) {
            if (self->disappearTimer > 0) {
                self->disappearTimer--;
            } else {
                self->disappearTimer--;

                if (self->disappearTimer > -10) {
                    Actor_SetScale(&self->actor, ((self->disappearTimer + 10) * 0.008f) * 0.1f);
                } else {
                    Actor_Kill(&self->actor);
                    return;
                }
            }
        }

        if (!(self->fairyFlags & FAIRY_FLAG_BIG)) {
            // GI_MAX in self case allows the player to catch the actor in a bottle
            func_8002F434(&self->actor, globalCtx, GI_MAX, 80.0f, 60.0f);
        }
    }
}

void func_80A0353C(EnElf* self, GlobalContext* globalCtx) {
    Vec3f parentPos;
    Actor* parent;

    SkelAnime_Update(&self->skelAnime);
    func_80A02A20(self, globalCtx);
    parent = self->actor.parent;

    if ((parent != NULL) && (parent->update != NULL)) {
        parentPos = self->actor.parent->world.pos;
        parentPos.y += ((1500.0f * self->actor.scale.y) + 40.0f);
        func_80A02C98(self, &parentPos, 0.2f);
    } else {
        Actor_Kill(&self->actor);
    }

    self->unk_2BC = Math_Atan2S(self->actor.velocity.z, self->actor.velocity.x);
}

void func_80A03604(EnElf* self, GlobalContext* globalCtx) {
}

void func_80A03610(EnElf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToF(&self->unk_2B8, 30.0f, 0.1f, 4.0f, 1.0f);

    self->unk_28C.x = Math_CosS(self->unk_2AC) * self->unk_2B8;
    self->unk_28C.y = self->unk_28C.y + self->unk_2B4;

    switch (self->unk_2AA) {
        case 0:
            if (self->unk_2B4 < 2.0f) {
                self->unk_2B4 += 0.1f;
            } else {
                self->unk_2AA++;
            }
            break;
        case 1:
            if (self->unk_2B4 > -1.0f) {
                self->unk_2B4 -= 0.2f;
            }
    }

    self->unk_28C.z = Math_SinS(self->unk_2AC) * -self->unk_2B8;
    self->unk_2AC += self->unk_2B0;
    func_80A02C98(self, &player->actor.world.pos, 0.2f);

    if (self->unk_2B4 < 0.0f) {
        if ((self->unk_28C.y < 20.0f) && (self->unk_28C.y > 0.0f)) {
            Actor_SetScale(&self->actor, (self->unk_28C.y * 0.008f) * 0.05f);
        }
    }

    if (self->unk_28C.y < -10.0f) {
        Actor_Kill(&self->actor);
        return;
    }

    self->unk_2BC = Math_Atan2S(self->actor.velocity.z, self->actor.velocity.x);
    EnElf_SpawnSparkles(self, globalCtx, 32);
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
}

void func_80A03814(EnElf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);

    if (self->unk_28C.y > 200.0f) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->unk_2AE >= 0x7E) {
        self->unk_2B8 += 0.1f;
        self->unk_2B4 += 0.5f;
        self->unk_28C.y += self->unk_2B4;
    } else {
        self->unk_2AE++;

        if (self->unk_2B8 < 30.0f) {
            self->unk_2B8 += 0.5f;
        }

        if (self->unk_28C.y > 0.0f) {
            self->unk_28C.y -= 0.7f;
        }
    }

    self->unk_28C.x = Math_CosS(self->unk_2AC) * self->unk_2B8;
    self->unk_28C.z = Math_SinS(self->unk_2AC) * -self->unk_2B8;
    self->unk_2AC += self->unk_2B0;
    func_80A02E30(self, &player->bodyPartsPos[0]);
    self->unk_2BC = Math_Atan2S(self->actor.velocity.z, self->actor.velocity.x);
    EnElf_SpawnSparkles(self, globalCtx, 32);
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
}

void func_80A03990(EnElf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    SkelAnime_Update(&self->skelAnime);

    self->unk_28C.z = 0.0f;
    self->unk_28C.x = 0.0f;
    self->unk_28C.y += self->unk_2B4;
    self->unk_2B4 -= 0.35f;

    if (self->unk_2B4 <= 0.0f) {
        EnElf_SetupAction(self, func_80A03814);
        self->unk_2B0 = 0x800;
        self->unk_2AE = 0;
        self->unk_2B4 = 0.0f;
        self->unk_2B8 = 1.0f;
    }

    func_80A02E30(self, &player->bodyPartsPos[0]);
    Actor_SetScale(&self->actor, (1.0f - (SQ(self->unk_2B4) * SQ(1.0f / 9.0f))) * 0.008f);
    self->unk_2BC = Math_Atan2S(self->actor.velocity.z, self->actor.velocity.x);
    EnElf_SpawnSparkles(self, globalCtx, 32);
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_FIATY_HEAL - SFX_FLAG);
}

void func_80A03AB0(EnElf* self, GlobalContext* globalCtx) {
    if (self->fairyFlags & 4) {
        func_80A04414(self, globalCtx);
    }

    SkelAnime_Update(&self->skelAnime);

    if (self->func_2C8 == NULL) {
        ASSERT(0, "0", "../z_en_elf.c", 1725);
    }

    self->func_2C8(self, globalCtx);
}

void EnElf_UpdateLights(EnElf* self, GlobalContext* globalCtx) {
    s16 glowLightRadius;
    Player* player;

    glowLightRadius = 100;

    if (self->unk_2A8 == 8) {
        glowLightRadius = 0;
    }

    if (self->fairyFlags & 0x20) {
        player = GET_PLAYER(globalCtx);
        Lights_PointNoGlowSetInfo(&self->lightInfoNoGlow, player->actor.world.pos.x,
                                  (s16)(player->actor.world.pos.y) + 60.0f, player->actor.world.pos.z, 255, 255, 255,
                                  200);
    } else {
        Lights_PointNoGlowSetInfo(&self->lightInfoNoGlow, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 255, 255, 255, -1);
    }

    Lights_PointGlowSetInfo(&self->lightInfoGlow, self->actor.world.pos.x, self->actor.world.pos.y,
                            self->actor.world.pos.z, 255, 255, 255, glowLightRadius);

    self->unk_2BC = Math_Atan2S(self->actor.velocity.z, self->actor.velocity.x);

    Actor_SetScale(&self->actor, self->actor.scale.x);
}

void func_80A03CF8(EnElf* self, GlobalContext* globalCtx) {
    Vec3f nextPos;
    Vec3f prevPos;
    Player* player = GET_PLAYER(globalCtx);
    Actor* arrowPointedActor;
    f32 xScale;
    f32 distFromLinksHead;

    func_80A0461C(self, globalCtx);
    func_80A03AB0(self, globalCtx);

    xScale = 0.0f;

    if ((globalCtx->csCtx.state != CS_STATE_IDLE) && (globalCtx->csCtx.npcActions[8] != NULL)) {
        EnElf_GetCutsceneNextPos(&nextPos, globalCtx, 8);

        if (globalCtx->csCtx.npcActions[8]->action == 5) {
            if (1) {}
            EnElf_SpawnSparkles(self, globalCtx, 16);
        }

        prevPos = self->actor.world.pos;

        if (self->unk_2A8 == 0xA) {
            func_80A02EC0(self, &nextPos);
        } else {
            func_80A02C98(self, &nextPos, 0.2f);
        }

        if ((globalCtx->sceneNum == SCENE_LINK_HOME) && (gSaveContext.sceneSetupIndex == 4)) {
            // play dash sound as Navi enters Links house in the intro
            if (1) {}
            if (globalCtx->csCtx.frames == 55) {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_FAIRY_DASH);
            }

            // play dash sound in intervals as Navi is waking up Link in the intro
            if (self->unk_2A8 == 6) {
                if (self->fairyFlags & 0x40) {
                    if (prevPos.y < self->actor.world.pos.y) {
                        self->fairyFlags &= ~0x40;
                    }
                } else {
                    if (self->actor.world.pos.y < prevPos.y) {
                        self->fairyFlags |= 0x40;
                        Audio_PlayActorSound2(&self->actor, NA_SE_EV_FAIRY_DASH);
                    }
                }
            }
        }
    } else {
        distFromLinksHead = Math_Vec3f_DistXYZ(&player->bodyPartsPos[8], &self->actor.world.pos);

        switch (self->unk_2A8) {
            case 7:
                func_80A02C98(self, &player->bodyPartsPos[8], 1.0f - self->unk_2AE * (1.0f / 30.0f));
                xScale = Math_Vec3f_DistXYZ(&player->bodyPartsPos[8], &self->actor.world.pos);

                if (distFromLinksHead < 7.0f) {
                    self->unk_2C0 = 0;
                    xScale = 0.0f;
                } else if (distFromLinksHead < 25.0f) {
                    xScale = (xScale - 5.0f) * 0.05f;
                    xScale = 1.0f - xScale;
                    xScale = (1.0f - SQ(xScale)) * 0.008f;
                } else {
                    xScale = 0.008f;
                }
                EnElf_SpawnSparkles(self, globalCtx, 16);
                break;
            case 8:
                func_80A02C98(self, &player->bodyPartsPos[8], 0.2f);
                self->actor.world.pos = player->bodyPartsPos[8];
                func_80A029A8(self, 1);
                break;
            case 11:
                nextPos = player->bodyPartsPos[8];
                nextPos.y += 1500.0f * self->actor.scale.y;
                func_80A02E30(self, &nextPos);
                EnElf_SpawnSparkles(self, globalCtx, 16);

                if (self->unk_2B8 <= 19.0f) {
                    self->unk_2B8 += 1.0f;
                }

                if (self->unk_2B8 >= 21.0f) {
                    self->unk_2B8 -= 1.0f;
                }

                if (self->unk_2C0 < 0x20) {
                    self->unk_2B0 = (self->unk_2C0 * 0xF0) + 0x200;
                    func_80A0299C(self, 1);
                }
                break;
            case 12:
                nextPos = GET_ACTIVE_CAM(globalCtx)->eye;
                nextPos.y += (-2000.0f * self->actor.scale.y);
                func_80A03148(self, &nextPos, 0.0f, 20.0f, 0.2f);
                break;
            default:
                func_80A029A8(self, 1);
                nextPos = globalCtx->actorCtx.targetCtx.naviRefPos;
                nextPos.y += (1500.0f * self->actor.scale.y);
                arrowPointedActor = globalCtx->actorCtx.targetCtx.arrowPointedActor;

                if (arrowPointedActor != NULL) {
                    func_80A03148(self, &nextPos, 0.0f, 20.0f, 0.2f);

                    if (self->actor.speedXZ >= 5.0f) {
                        EnElf_SpawnSparkles(self, globalCtx, 16);
                    }
                } else {
                    if ((self->timer % 32) == 0) {
                        self->unk_2A0 = Rand_ZeroFloat(7.0f) + 3.0f;
                    }

                    if (self->fairyFlags & 2) {
                        if (distFromLinksHead < 30.0f) {
                            self->fairyFlags ^= 2;
                        }

                        func_80A03148(self, &nextPos, 0.0f, 20.0f, 0.2f);
                        EnElf_SpawnSparkles(self, globalCtx, 16);
                    } else {
                        if (distFromLinksHead > 100.0f) {
                            self->fairyFlags |= 2;

                            if (self->unk_2C7 == 0) {
                                Audio_PlayActorSound2(&self->actor, NA_SE_EV_FAIRY_DASH);
                            }

                            self->unk_2C0 = 0x64;
                        }
                        func_80A03148(self, &nextPos, 0.0f, self->unk_2A0, 0.2f);
                    }
                }
                break;
        }
    }

    if (self->unk_2A8 == 7) {
        self->actor.scale.x = xScale;
    } else if (self->unk_2A8 == 8) {
        self->actor.scale.x = 0.0f;
    } else {
        Math_SmoothStepToF(&self->actor.scale.x, 0.008f, 0.3f, 0.00080000004f, 0.000080000005f);
    }

    EnElf_UpdateLights(self, globalCtx);
}

void EnElf_ChangeColor(Color_RGBAf* dest, Color_RGBAf* newColor, Color_RGBAf* curColor, f32 rate) {
    Color_RGBAf rgbaDiff;

    rgbaDiff.r = (newColor->r - curColor->r);
    rgbaDiff.g = (newColor->g - curColor->g);
    rgbaDiff.b = (newColor->b - curColor->b);
    rgbaDiff.a = (newColor->a - curColor->a);

    dest->r += (rgbaDiff.r * rate);
    dest->g += (rgbaDiff.g * rate);
    dest->b += (rgbaDiff.b * rate);
    dest->a += (rgbaDiff.a * rate);
}

void func_80A04414(EnElf* self, GlobalContext* globalCtx) {
    Actor* arrowPointedActor = globalCtx->actorCtx.targetCtx.arrowPointedActor;
    Player* player = GET_PLAYER(globalCtx);
    f32 transitionRate;
    u16 targetSound;

    if (globalCtx->actorCtx.targetCtx.unk_40 != 0.0f) {
        self->unk_2C6 = 0;
        self->unk_29C = 1.0f;

        if (self->unk_2C7 == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_FAIRY_DASH);
        }

    } else {
        if (self->unk_2C6 == 0) {
            if ((arrowPointedActor == NULL) ||
                (Math_Vec3f_DistXYZ(&self->actor.world.pos, &globalCtx->actorCtx.targetCtx.naviRefPos) < 50.0f)) {
                self->unk_2C6 = 1;
            }
        } else if (self->unk_29C != 0.0f) {
            if (Math_StepToF(&self->unk_29C, 0.0f, 0.25f) != 0) {
                self->innerColor = globalCtx->actorCtx.targetCtx.naviInner;
                self->outerColor = globalCtx->actorCtx.targetCtx.naviOuter;
            } else {
                transitionRate = 0.25f / self->unk_29C;
                EnElf_ChangeColor(&self->innerColor, &globalCtx->actorCtx.targetCtx.naviInner, &self->innerColor,
                                  transitionRate);
                EnElf_ChangeColor(&self->outerColor, &globalCtx->actorCtx.targetCtx.naviOuter, &self->outerColor,
                                  transitionRate);
            }
        }
    }

    if (self->fairyFlags & 1) {
        if ((arrowPointedActor == NULL) || (player->unk_664 == NULL)) {
            self->fairyFlags ^= 1;
        }
    } else {
        if ((arrowPointedActor != NULL) && (player->unk_664 != NULL)) {
            if (arrowPointedActor->category == ACTORCAT_NPC) {
                targetSound = NA_SE_VO_NAVY_HELLO;
            } else {
                targetSound =
                    (arrowPointedActor->category == ACTORCAT_ENEMY) ? NA_SE_VO_NAVY_ENEMY : NA_SE_VO_NAVY_HEAR;
            }

            if (self->unk_2C7 == 0) {
                Audio_PlayActorSound2(&self->actor, targetSound);
            }

            self->fairyFlags |= 1;
        }
    }
}

void func_80A0461C(EnElf* self, GlobalContext* globalCtx) {
    s32 temp;
    Actor* arrowPointedActor;
    Player* player = GET_PLAYER(globalCtx);

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if (globalCtx->csCtx.npcActions[8] != NULL) {
            switch (globalCtx->csCtx.npcActions[8]->action) {
                case 4:
                    temp = 9;
                    break;
                case 3:
                    temp = 6;
                    break;
                case 1:
                    temp = 10;
                    break;
                default:
                    temp = 0;
                    break;
            }
        } else {
            temp = 0;
            self->unk_2C0 = 100;
        }

    } else {
        arrowPointedActor = globalCtx->actorCtx.targetCtx.arrowPointedActor;

        if ((player->stateFlags1 & 0x400) || ((YREG(15) & 0x10) && func_800BC56C(globalCtx, 2))) {
            temp = 12;
            self->unk_2C0 = 100;
        } else if (arrowPointedActor == NULL || arrowPointedActor->category == ACTORCAT_NPC) {
            if (arrowPointedActor != NULL) {
                self->unk_2C0 = 100;
                player->stateFlags2 |= 0x100000;
                temp = 0;
            } else {
                switch (self->unk_2A8) {
                    case 0:
                        if (self->unk_2C0 != 0) {
                            self->unk_2C0--;
                            temp = 0;
                        } else {
                            if (self->unk_2C7 == 0) {
                                Audio_PlayActorSound2(&self->actor, NA_SE_EV_NAVY_VANISH);
                            }
                            temp = 7;
                        }
                        break;
                    case 7:
                        if (self->unk_2C0 != 0) {
                            if (self->unk_2AE > 0) {
                                self->unk_2AE--;
                                temp = 7;
                            } else {
                                player->stateFlags2 |= 0x100000;
                                temp = 0;
                            }
                        } else {
                            temp = 8;
                            func_80A029A8(self, 10);
                        }
                        break;
                    case 8:
                        temp = 8;
                        break;
                    case 11:
                        temp = self->unk_2A8;
                        if (self->unk_2C0 > 0) {
                            self->unk_2C0--;
                        } else {
                            temp = 0;
                        }
                        break;
                    default:
                        temp = 0;
                        break;
                }
            }
        } else {
            temp = 1;
        }

        switch (temp) {
            case 0:
                if (!(player->stateFlags2 & 0x100000)) {
                    temp = 7;
                    if (self->unk_2C7 == 0) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_EV_NAVY_VANISH);
                    }
                }
                break;
            case 8:
                if (player->stateFlags2 & 0x100000) {
                    func_80A0299C(self, 0x32);
                    self->unk_2C0 = 42;
                    temp = 11;
                    if (self->unk_2C7 == 0) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_EV_FAIRY_DASH);
                    }
                }
                break;
            case 7:
                player->stateFlags2 &= ~0x100000;
                break;
            default:
                player->stateFlags2 |= 0x100000;
                break;
        }
    }

    if (temp != self->unk_2A8) {
        func_80A01C38(self, temp);

        if (temp == 11) {
            self->unk_2B8 = Math_Vec3f_DistXZ(&player->bodyPartsPos[8], &self->actor.world.pos);
            self->unk_2AC = Math_Vec3f_Yaw(&self->actor.world.pos, &player->bodyPartsPos[8]);
        }
    }
}

void EnElf_SpawnSparkles(EnElf* self, GlobalContext* globalCtx, s32 sparkleLife) {
    static Vec3f sparkleVelocity = { 0.0f, -0.05f, 0.0f };
    static Vec3f sparkleAccel = { 0.0f, -0.025f, 0.0f };
    s32 pad;
    Vec3f sparklePos;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;

    sparklePos.x = Rand_CenteredFloat(6.0f) + self->actor.world.pos.x;
    sparklePos.y = (Rand_ZeroOne() * 6.0f) + self->actor.world.pos.y;
    sparklePos.z = Rand_CenteredFloat(6.0f) + self->actor.world.pos.z;

    primColor.r = self->innerColor.r;
    primColor.g = self->innerColor.g;
    primColor.b = self->innerColor.b;

    envColor.r = self->outerColor.r;
    envColor.g = self->outerColor.g;
    envColor.b = self->outerColor.b;

    EffectSsKiraKira_SpawnDispersed(globalCtx, &sparklePos, &sparkleVelocity, &sparkleAccel, &primColor, &envColor,
                                    1000, sparkleLife);
}

void func_80A04D90(EnElf* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 bgId;

    self->actor.floorHeight = BgCheck_EntityRaycastFloor5(globalCtx, &globalCtx->colCtx, &self->actor.floorPoly, &bgId,
                                                          &self->actor, &self->actor.world.pos);
    self->actor.shape.shadowAlpha = 0x32;
}

// move to talk to player
void func_80A04DE4(EnElf* self, GlobalContext* globalCtx) {
    Vec3f headCopy;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f naviRefPos;

    if (self->fairyFlags & 0x10) {
        naviRefPos = globalCtx->actorCtx.targetCtx.naviRefPos;

        if ((player->unk_664 == NULL) || (&player->actor == player->unk_664) || (&self->actor == player->unk_664)) {
            naviRefPos.x = player->bodyPartsPos[7].x + (Math_SinS(player->actor.shape.rot.y) * 20.0f);
            naviRefPos.y = player->bodyPartsPos[7].y + 5.0f;
            naviRefPos.z = player->bodyPartsPos[7].z + (Math_CosS(player->actor.shape.rot.y) * 20.0f);
        }

        self->actor.focus.pos = naviRefPos;
        self->fairyFlags &= ~0x10;
    }

    func_80A03AB0(self, globalCtx);
    headCopy = self->actor.focus.pos;

    func_80A03148(self, &headCopy, 0, 20.0f, 0.2f);

    if (self->actor.speedXZ >= 5.0f) {
        EnElf_SpawnSparkles(self, globalCtx, 16);
    }

    Math_SmoothStepToF(&self->actor.scale.x, 0.008f, 0.3f, 0.00080000004f, 0.000080000005f);
    EnElf_UpdateLights(self, globalCtx);
}

// move after talking to player
void func_80A04F94(EnElf* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->unk_2BC, 5, 0x1000, 0x400);
    self->timer++;
    Math_StepToF(&self->unk_2A4, 1.0f, 0.05f);
    Environment_AdjustLights(globalCtx, SQ(self->unk_2A4), player->actor.projectedPos.z + 780.0f, 0.2f, 0.5f);
}

// ask to talk to saria again
void func_80A05040(Actor* thisx, GlobalContext* globalCtx) {
    EnElf* self = THIS;

    func_80A04DE4(self, globalCtx);

    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // yes
                func_8010B720(globalCtx, ElfMessage_GetSariaText(globalCtx));
                self->actor.update = func_80A05114;
                break;
            case 1: // no
                func_80106CCC(globalCtx);
                self->actor.update = func_80A053F0;
                func_80A01C38(self, 0);
                self->fairyFlags &= ~0x20;
                break;
        }
    }

    func_80A04F94(self, globalCtx);
}

void func_80A05114(Actor* thisx, GlobalContext* globalCtx) {
    EnElf* self = THIS;

    func_80A04DE4(self, globalCtx);

    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, 0xE3);
        self->actor.update = func_80A05040;
    }

    func_80A04F94(self, globalCtx);
}

void func_80A05188(Actor* thisx, GlobalContext* globalCtx) {
    EnElf* self = THIS;

    func_80A04DE4(self, globalCtx);

    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        func_8010B720(globalCtx, ElfMessage_GetSariaText(globalCtx));
        self->actor.update = func_80A05114;
    }

    func_80A04F94(self, globalCtx);
}

// ask to talk to navi
void func_80A05208(Actor* thisx, GlobalContext* globalCtx) {
    s32 naviCUpText;
    EnElf* self = THIS;

    func_80A04DE4(self, globalCtx);

    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // yes
                naviCUpText = ElfMessage_GetCUpText(globalCtx);

                if (naviCUpText != 0) {
                    func_8010B720(globalCtx, naviCUpText);
                } else {
                    func_8010B720(globalCtx, 0x15F);
                }

                self->actor.update = func_80A052F4;
                break;
            case 1: // no
                func_80106CCC(globalCtx);
                self->actor.update = func_80A053F0;
                func_80A01C38(self, 0);
                self->fairyFlags &= ~0x20;
                break;
        }
    }

    func_80A04F94(self, globalCtx);
}

// ask to talk to saria
void func_80A052F4(Actor* thisx, GlobalContext* globalCtx) {
    EnElf* self = THIS;

    func_80A04DE4(self, globalCtx);

    if (func_8010BDBC(&globalCtx->msgCtx) == 4) {
        if (func_80106BC8(globalCtx)) {
            globalCtx->msgCtx.unk_E3F2 = 0xFF;

            switch (globalCtx->msgCtx.choiceIndex) {
                case 0: // yes
                    self->actor.update = func_80A05188;
                    func_8010B720(globalCtx, 0xE2);
                    break;
                case 1: // no
                    self->actor.update = func_80A05208;
                    func_8010B720(globalCtx, 0xE1);
                    break;
            }
        }
    } else if (func_8002F334(thisx, globalCtx)) {
        self->actor.update = func_80A053F0;
        func_80A01C38(self, 0);
        self->fairyFlags &= ~0x20;
    }

    func_80A04F94(self, globalCtx);
}

void func_80A053F0(Actor* thisx, GlobalContext* globalCtx) {
    u8 unk2C7;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    EnElf* self = THIS;

    if (player->naviTextId == 0) {
        if (player->unk_664 == NULL) {
            if (((gSaveContext.naviTimer >= 600) && (gSaveContext.naviTimer <= 3000)) || (nREG(89) != 0)) {
                player->naviTextId = ElfMessage_GetCUpText(globalCtx);

                if (player->naviTextId == 0x15F) {
                    player->naviTextId = 0;
                }
            }
        }
    } else if (player->naviTextId < 0) {
        // trigger dialog instantly for negative message IDs
        thisx->flags |= 0x10000;
    }

    if (func_8002F194(thisx, globalCtx)) {
        func_800F4524(&D_801333D4, NA_SE_VO_SK_LAUGH, 0x20);
        thisx->focus.pos = thisx->world.pos;

        if (thisx->textId == ElfMessage_GetCUpText(globalCtx)) {
            self->fairyFlags |= 0x80;
            gSaveContext.naviTimer = 3001;
        }

        self->fairyFlags |= 0x10;
        self->fairyFlags |= 0x20;
        thisx->update = func_80A052F4;
        func_80A01C38(self, 3);

        if (self->elfMsg != NULL) {
            self->elfMsg->actor.flags |= 0x100;
        }

        thisx->flags &= ~0x10000;
    } else {
        self->actionFunc(self, globalCtx);
        thisx->shape.rot.y = self->unk_2BC;
        nREG(80) = gSaveContext.sceneFlags[127].chest;

        if (nREG(81) != 0) {
            if (gSaveContext.sceneFlags[127].chest) {
                LOG_NUM("z_common_data.memory.information.room_inf[127][ 0 ]", gSaveContext.sceneFlags[127].chest,
                        "../z_en_elf.c", 2595);
            }
        }

        if (!Gameplay_InCsMode(globalCtx)) {
            if (gSaveContext.naviTimer < 25800) {
                gSaveContext.naviTimer++;
            } else if (!(self->fairyFlags & 0x80)) {
                gSaveContext.naviTimer = 0;
            }
        }
    }

    self->elfMsg = NULL;
    self->timer++;

    if (self->unk_2A4 > 0.0f) {
        Math_StepToF(&self->unk_2A4, 0.0f, 0.05f);
        Environment_AdjustLights(globalCtx, SQ(self->unk_2A4) * self->unk_2A4, player->actor.projectedPos.z + 780.0f,
                                 0.2f, 0.5f);
    }

    // temp probably fake match
    unk2C7 = self->unk_2C7;
    if (unk2C7 > 0) {
        self->unk_2C7--;
    }

    if ((self->unk_2C7 == 0) && (globalCtx->csCtx.state != CS_STATE_IDLE)) {
        self->unk_2C7 = 1;
    }

    func_80A04D90(self, globalCtx);
}

void EnElf_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnElf* self = THIS;

    self->actionFunc(self, globalCtx);
    self->actor.shape.rot.y = self->unk_2BC;
    self->timer++;

    if (self->fairyFlags & FAIRY_FLAG_BIG) {
        func_80A04D90(self, globalCtx);
    }
}

s32 EnElf_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                           Gfx** gfx) {
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    s32 pad;
    f32 scale;
    Vec3f mtxMult;
    EnElf* self = THIS;

    if (limbIndex == 8) {
        scale = ((Math_SinS(self->timer * 4096) * 0.1f) + 1.0f) * 0.012f;

        if (self->fairyFlags & FAIRY_FLAG_BIG) {
            scale *= 2.0f;
        }

        scale *= (self->actor.scale.x * 124.99999f);
        Matrix_MultVec3f(&zeroVec, &mtxMult);
        Matrix_Translate(mtxMult.x, mtxMult.y, mtxMult.z, MTXMODE_NEW);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    }

    // do not draw wings for big fairies
    if (self->fairyFlags & FAIRY_FLAG_BIG) {
        if (limbIndex == 4 || limbIndex == 7 || limbIndex == 11 || limbIndex == 14) {
            *dList = NULL;
        }
    }

    return false;
}

void EnElf_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    f32 alphaScale;
    s32 envAlpha;
    EnElf* self = THIS;
    s32 pad1;
    Gfx* dListHead;
    Player* player = GET_PLAYER(globalCtx);

    if ((self->unk_2A8 != 8) && !(self->fairyFlags & 8)) {
        if (!(player->stateFlags1 & 0x100000) || (kREG(90) < self->actor.projectedPos.z)) {
            dListHead = Graph_Alloc(globalCtx->state.gfxCtx, sizeof(Gfx) * 4);

            OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_elf.c", 2730);

            func_80094B58(globalCtx->state.gfxCtx);

            envAlpha = (self->timer * 50) & 0x1FF;
            envAlpha = (envAlpha > 255) ? 511 - envAlpha : envAlpha;

            alphaScale = self->disappearTimer < 0 ? (self->disappearTimer * (7.0f / 6000.0f)) + 1.0f : 1.0f;

            gSPSegment(POLY_XLU_DISP++, 0x08, dListHead);
            gDPPipeSync(dListHead++);
            gDPSetPrimColor(dListHead++, 0, 0x01, (u8)self->innerColor.r, (u8)self->innerColor.g,
                            (u8)self->innerColor.b, (u8)(self->innerColor.a * alphaScale));

            if (self->fairyFlags & 4) {
                gDPSetRenderMode(dListHead++, G_RM_PASS, G_RM_CLD_SURF2);
            } else {
                gDPSetRenderMode(dListHead++, G_RM_PASS, G_RM_ZB_CLD_SURF2);
            }

            gSPEndDisplayList(dListHead++);
            gDPSetEnvColor(POLY_XLU_DISP++, (u8)self->outerColor.r, (u8)self->outerColor.g, (u8)self->outerColor.b,
                           (u8)(envAlpha * alphaScale));
            POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                           EnElf_OverrideLimbDraw, NULL, self, POLY_XLU_DISP);

            CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_elf.c", 2793);
        }
    }
}

void EnElf_GetCutsceneNextPos(Vec3f* vec, GlobalContext* globalCtx, s32 action) {
    Vec3f startPos;
    Vec3f endPos;
    CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[action];
    f32 lerp;

    startPos.x = npcAction->startPos.x;
    startPos.y = npcAction->startPos.y;
    startPos.z = npcAction->startPos.z;

    endPos.x = npcAction->endPos.x;
    endPos.y = npcAction->endPos.y;
    endPos.z = npcAction->endPos.z;

    lerp = Environment_LerpWeight(npcAction->endFrame, npcAction->startFrame, globalCtx->csCtx.frames);

    vec->x = ((endPos.x - startPos.x) * lerp) + startPos.x;
    vec->y = ((endPos.y - startPos.y) * lerp) + startPos.y;
    vec->z = ((endPos.z - startPos.z) * lerp) + startPos.z;
}
