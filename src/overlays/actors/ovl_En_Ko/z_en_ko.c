#include "z_en_ko.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnKo*)thisx)

void EnKo_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKo_Update(Actor* thisx, GlobalContext* globalCtx);
void EnKo_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A99048(EnKo* this, GlobalContext* globalCtx);
void func_80A995CC(EnKo* this, GlobalContext* globalCtx);
void func_80A99384(EnKo* this, GlobalContext* globalCtx);
void func_80A99438(EnKo* this, GlobalContext* globalCtx);
void func_80A99504(EnKo* this, GlobalContext* globalCtx);
void func_80A99560(EnKo* this, GlobalContext* globalCtx);
Gfx* func_80A99C94(GraphicsContext* gfxCtx, u8 r, u8 g, u8 b, u8 a);
s32 func_80A99864(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx);
u16 func_80A97610(GlobalContext*, Actor*);
s16 func_80A97738(GlobalContext*, Actor*);
f32 func_80A97BC0(EnKo* this);
s32 func_80A98ECC(EnKo* this, GlobalContext* globalCtx);

extern UNK_TYPE D_06006A60;
extern UNK_TYPE D_06007830;
extern UNK_TYPE D_06007D94;
extern UNK_TYPE D_0600879C;
extern UNK_TYPE D_06008F6C;

const ActorInit En_Ko_InitVars = {
    ACTOR_EN_KO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnKo),
    (ActorFunc)EnKo_Init,
    (ActorFunc)EnKo_Destroy,
    (ActorFunc)EnKo_Update,
    (ActorFunc)EnKo_Draw,
};

static ColliderCylinderInit D_80A9A100 = {
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
    { 20, 46, 0, { 0, 0, 0 } },
};

CollisionCheckInfoInit2 D_80A9A12C = { 0, 0, 0, 0, 0xFF };

void* D_80A9A138[] = { 0x06000240, 0x06000D40, 0x06001140, 0x00000000 };
void* D_80A9A148[] = { 0x06000F4C, 0x06001A0C, 0x06001E0C, 0x00000000 };

typedef struct {
    s16 objectId;
    Gfx* gfx;
    void** unk_8;
} struct_80A9A158;

// struct_80A9A158 D_80A9A158[] = { { 0x00FC, 0x06001890 } };

struct_80A9A158 D_80A9A158[] = { { 0x00FC, 0x06001890, NULL },
                                 { 0x00FD, 0x06002C10, D_80A9A148 },
                                 { 0x013D, 0x06002940, D_80A9A138 } };
// s32 D_80A9A160[] = { 0x00000000, 0x00FD0000, 0x06002C10, &D_80A9A148, 0x013D0000, 0x06002940, &D_80A9A138 };

typedef struct {
    s16 objectId;
    FlexSkeletonHeader* unk_4;
} struct_80A9A17C;

// Object IDS? OBJECT_KM1 / OBJECT_KW1
struct_80A9A17C D_80A9A17C[2] = { { OBJECT_KM1, 0x060000F0 }, { OBJECT_KW1, 0x060000F0 } };

// s32 D_80A9A17C[] = { 0x00FC0000 };
// s32 D_80A9A180[] = { 0x060000F0, 0x00FD0000, 0x060000F0 };

struct_80034EC0_Entry D_80A9A18C[] = {
    { 0x06008F6C, 1.0f, 2.0f, 14.0f, 4, 0.0f },   { 0x06008F6C, 0.0f, 1.0f, 1.0f, 4, 0.0f },
    { 0x06009B64, 0.0f, 0.0f, 0.0f, 2, 0.0f },    { 0x06009B64, 0.0f, 1.0f, 1.0f, 2, 0.0f },
    { 0x06009B64, 0.0f, 2.0f, 2.0f, 2, 0.0f },    { 0x060062DC, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x060062DC, 1.0f, 0.0f, -1.0f, 0, -10.0f }, { 0x06005808, 1.0f, 0.0f, -1.0f, 0, -10.0f },
    { 0x06007830, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x06008178, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x060065E0, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x0600879C, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x06007FFC, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x060080B4, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x060091AC, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x06006F9C, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x06007064, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x06007120, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x06007F38, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x06007D94, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x06006EE0, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x060098EC, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x060090EC, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x0600982C, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x06009274, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x060099A4, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x06009028, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x06007E64, 1.0f, 0.0f, -1.0f, 0, 0.0f },
    { 0x06007454, 1.0f, 0.0f, -1.0f, 0, 0.0f },   { 0x06008F6C, 0.0f, 1.0f, 1.0f, 4, -8.0f },
    { 0x06007D94, 1.0f, 0.0f, -1.0f, 0, -8.0f },  { 0x0600879C, 1.0f, 0.0f, -1.0f, 0, -8.0f },
    { 0x06006A60, 1.0f, 0.0f, -1.0f, 0, -8.0f },  { 0x06007830, 1.0f, 0.0f, -1.0f, 0, -8.0f }
};
/*typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
} struct_80A9A4BC;*/
u8 D_80A9A4BC[13][5] = { { 0x08, 0x09, 0x09, 0x0E, 0x0B }, { 0x02, 0x0C, 0x02, 0x0D, 0x0D },
                         { 0x0B, 0x0B, 0x0B, 0x0F, 0x09 }, { 0x00, 0x10, 0x10, 0x11, 0x12 },
                         { 0x13, 0x13, 0x14, 0x0A, 0x09 }, { 0x03, 0x03, 0x03, 0x03, 0x03 },
                         { 0x04, 0x16, 0x16, 0x04, 0x17 }, { 0x18, 0x10, 0x10, 0x19, 0x10 },
                         { 0x1A, 0x0F, 0x0F, 0x1A, 0x0F }, { 0x03, 0x03, 0x03, 0x1B, 0x1B },
                         { 0x02, 0x02, 0x02, 0x02, 0x16 }, { 0x0E, 0x0E, 0x0E, 0x0E, 0x0E },
                         { 0x05, 0x05, 0x05, 0x05, 0x05 } };
typedef struct {
    u8 unk_0;
    u8 unk_1;
    Color_RGBA8 color1;
    u8 unk_6;
    Color_RGBA8 color2;
} struct_80A9A500;
struct_80A9A500 D_80A9A500[] = {
    { 0x00, 0x00, { 0, 130, 70, 255 }, 0x00, { 110, 170, 20, 255 } },
    { 0x01, 0x01, { 70, 190, 60, 255 }, 0x01, { 100, 30, 0, 255 } },
    { 0x00, 0x00, { 0, 130, 70, 255 }, 0x00, { 110, 170, 20, 255 } },
    { 0x00, 0x00, { 0, 130, 70, 255 }, 0x00, { 110, 170, 20, 255 } },
    { 0x00, 0x00, { 0, 130, 70, 255 }, 0x00, { 110, 170, 20, 255 } },
    { 0x01, 0x01, { 70, 190, 60, 255 }, 0x01, { 100, 30, 0, 255 } },
    { 0x01, 0x01, { 70, 190, 60, 255 }, 0x01, { 100, 30, 0, 255 } },
    { 0x00, 0x00, { 0, 130, 70, 255 }, 0x00, { 110, 170, 20, 255 } },
    { 0x00, 0x00, { 0, 130, 70, 255 }, 0x00, { 110, 170, 20, 255 } },
    { 0x01, 0x01, { 70, 190, 60, 255 }, 0x01, { 100, 30, 0, 255 } },
    { 0x01, 0x01, { 70, 190, 60, 255 }, 0x01, { 100, 30, 0, 255 } },
    { 0x00, 0x00, { 0, 130, 70, 255 }, 0x00, { 110, 170, 20, 255 } },
    { 0x02, 0x01, { 70, 190, 60, 255 }, 0x01, { 100, 30, 0, 255 } },
};

typedef struct {
    s8 unk_0;
    f32 unk_4;
    f32 unk_8;
} struct_80A9A590;

struct_80A9A590 D_80A9A590[] = { { 6, 30.0f, 180.0f }, { 6, 30.0f, 180.0f }, { 6, 30.0f, 180.0f }, { 6, 30.0f, 180.0f },
                                 { 6, 30.0f, 180.0f }, { 1, 30.0f, 240.0f }, { 6, 30.0f, 180.0f }, { 6, 30.0f, 180.0f },
                                 { 6, 30.0f, 180.0f }, { 6, 30.0f, 180.0f }, { 6, 30.0f, 180.0f }, { 6, 30.0f, 180.0f },
                                 { 6, 30.0f, 180.0f } };

s32 func_80A96DB0(EnKo* this, GlobalContext* globalCtx) {
    u8 temp_t0;
    u8 temp_t1;
    s16 temp_a1;

    temp_a1 = D_80A9A17C[D_80A9A500[this->actor.params & 0xFF].unk_6].objectId;
    temp_a1 = D_80A9A500[this->actor.params & 0xFF].unk_6;
    temp_t0 = D_80A9A500[this->actor.params & 0xFF].unk_0;
    temp_t1 = D_80A9A500[this->actor.params & 0xFF].unk_1;
    this->unk_196 = Object_GetIndex(&globalCtx->objectCtx, D_80A9A17C[temp_a1].objectId);
    if (this->unk_196 < 0) {
        return 0;
    }

    this->unk_195 = Object_GetIndex(&globalCtx->objectCtx, D_80A9A17C[temp_t1].objectId);
    if (this->unk_195 < 0) {
        return 0;
    }

    this->unk_194 = Object_GetIndex(&globalCtx->objectCtx, D_80A9A158[temp_t0].objectId);
    if (this->unk_194 < 0) {
        return 0;
    }

    return 1;
}

s32 func_80A96EC4(EnKo* this, GlobalContext* globalCtx) {
    if (!Object_IsLoaded(&globalCtx->objectCtx, this->unk_196)) {
        return 0;
    }
    if (!Object_IsLoaded(&globalCtx->objectCtx, this->unk_195)) {
        return 0;
    }
    if (!Object_IsLoaded(&globalCtx->objectCtx, this->unk_194)) {
        return 0;
    }
    return 1;
}

s32 func_80A96F48(EnKo* this, GlobalContext* globalCtx) {
    this->objectIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_OS_ANIME);
    if (this->objectIndex < 0) {
        return 0;
    }
    return 1;
}

s32 func_80A96F94(EnKo* this, GlobalContext* globalCtx) {
    if (!Object_IsLoaded(&globalCtx->objectCtx, this->objectIndex)) {
        return 0;
    }
    return 1;
}

u16 func_80A96FD0(GlobalContext* globalCtx, Actor* thisx) {
    switch (thisx->params & 0xFF) {
        case 12:
            if (gSaveContext.eventChkInf[4] & 1) {
                return 0x10DA;
            }
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x10D9;
            }
            return ((gSaveContext.infTable[11] & 0x80) != 0) ? 0x10D8 : 0x10D7;
        case 0:
            if (gSaveContext.eventChkInf[4] & 1) {
                return 0x1025;
            }
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x1042;
            }
            return 0x1004;
        case 1:
            if (gSaveContext.eventChkInf[4] & 1) {
                return 0x1023;
            }
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x1043;
            }
            if (gSaveContext.infTable[1] & 0x4000) {
                return 0x1006;
            }
            return 0x1005;
        case 2:
            if (gSaveContext.eventChkInf[4] & 1) {
                return 0x1022;
            }
            return 0x1007;
        case 3:
            if (gSaveContext.eventChkInf[4] & 1) {
                return 0x1021;
            }
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x1044;
            }
            if (gSaveContext.infTable[2] & 4) {
                return 0x1009;
            }
            return 0x1008;
        case 4:
            if (gSaveContext.eventChkInf[4] & 1) {
                return 0x1097;
            }
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x1042;
            }
            if (gSaveContext.infTable[2] & 0x10) {
                return 0x100B;
            }
            return 0x100A;
        case 5:
            if (gSaveContext.eventChkInf[4] & 1) {
                return 0x10B0;
            }
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x1043;
            }
            if (gSaveContext.infTable[2] & 0x40) {
                return 0x100D;
            }
            return 0x100C;
        case 6:
            if (gSaveContext.eventChkInf[4] & 1) {
                return 0x10B5;
            }
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x1043;
            }
            if (gSaveContext.infTable[2] & 0x100) {
                return 0x1019;
            }
            return 0x100E;
        case 7:
            return 0x1035;
        case 8:
            return 0x1038;
        case 9:
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x104B;
            }
            return 0x103C;
        case 10:
            if ((gBitFlags[18]) & gSaveContext.inventory.questItems) {
                return 0x104C;
            }
            return 0x103D;
        case 11:
            return 0x103E;
    }
    return 0;
}

u16 func_80A97338(GlobalContext* globalCtx, Actor* thisx);
#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97338.s")

u16 func_80A97610(GlobalContext* globalCtx, Actor* thisx) {
    u16 faceReaction;

    if ((((((((thisx->params & 0xFF) == 0) || (thisx->params & 0xFF) == 2) || (thisx->params & 0xFF) == 3) ||
           (thisx->params & 0xFF) == 4) ||
          (thisx->params & 0xFF) == 7) ||
         (thisx->params & 0xFF) == 8) ||
        (thisx->params & 0xFF) == 0xB) {
        faceReaction = Text_GetFaceReaction(globalCtx, 0x13);
    }
    if ((((((thisx->params & 0xFF) == 1) || ((thisx->params & 0xFF) == 5)) || ((thisx->params & 0xFF) == 6)) ||
         ((thisx->params & 0xFF) == 9)) ||
        ((thisx->params & 0xFF) == 0xA)) {
        faceReaction = Text_GetFaceReaction(globalCtx, 0x14);
    }
    if ((thisx->params & 0xFF) == 0xC) {
        faceReaction = Text_GetFaceReaction(globalCtx, 0x12);
    }
    if (faceReaction != 0) {
        return faceReaction;
    }
    if (LINK_IS_ADULT) {
        return func_80A97338(globalCtx, thisx);
    }
    return func_80A96FD0(globalCtx, thisx);
}
//#define NON_EQUIVALENT
//#ifdef NON_EQUIVALENT // Is it actually non-equivalent? I havn't the slight idea
s16 func_80A97738(GlobalContext* globalCtx, Actor* thisx) {
    u16 phi_v1;

    // temp_v0 = func_8010BDBC(&globalCtx->msgCtx);

    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 2:
            // TODO the big one
            switch (thisx->textId) {
                case 0x1005:
                    gSaveContext.infTable[1] |= 0x4000;
                    break;
                case 0x1008:
                    gSaveContext.infTable[2] |= 0x4;
                    break;
                case 0x100A:
                    gSaveContext.infTable[2] |= 0x10;
                    break;
                case 0x100C:
                    gSaveContext.infTable[2] |= 0x40;
                    break;
                case 0x100E:
                    gSaveContext.infTable[2] |= 0x100;
                    break;
                case 0x104F:
                    gSaveContext.infTable[5] |= 0x200;
                    break;
                case 0x1053:
                    gSaveContext.infTable[6] |= 2;
                    break;
                case 0x1055:
                    gSaveContext.infTable[4] |= 2;
                    break;
                case 0x1058:
                    gSaveContext.infTable[5] |= 2;
                    break;
                case 0x105D:
                    gSaveContext.infTable[4] |= 0x80;
                    break;
                case 0x10BA:
                    break;
                case 0x10D7:
                    gSaveContext.infTable[11] |= 0x80;
                    break;
                default:
                    return 1;
            }
            return 0;

        case 3:
            if ((thisx->textId == 0x10B7) || (thisx->textId == 0x10B8)) {
                if (THIS->unk_210 == 0) {
                    Audio_PlaySoundGeneral((u16)0x4807U, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                    THIS->unk_210 = 1;
                }
            }
            return 1;

        case 4:
            if (func_80106BC8(globalCtx)) {
                switch (thisx->textId) {
                    case 0x1035:
                        thisx->textId = (globalCtx->msgCtx.choiceIndex == 0) ? 0x1036 : 0x1037;
                        func_8010B720(globalCtx, thisx->textId);
                        return 1;
                    case 0x1038:
                        switch (globalCtx->msgCtx.choiceIndex) {
                            case 0:
                                thisx->textId = 0x103A;
                                return 1;
                            case 1:
                                thisx->textId = 0x0103B;
                                return 1;
                            case 2:
                                thisx->textId = 0x1039;
                                return 1;
                        }
                        func_8010B720(globalCtx, thisx->textId);
                        return 1;
                    case 0x103E:
                        thisx->textId = (globalCtx->msgCtx.choiceIndex == 0) ? 0x103F : 0x1040;
                        func_8010B720(globalCtx, thisx->textId);
                        return 1;
                    case 0x10B7:
                        gSaveContext.infTable[11] |= 0x1000;
                    case 0x10B8:
                        thisx->textId = (globalCtx->msgCtx.choiceIndex == 0) ? 0x10BA : 0x10B9;
                        // if (globalCtx->msgCtx.choiceIndex != 0) {
                        //    return 1;
                        //}
                        // return 2;
                        return (globalCtx->msgCtx.choiceIndex == 0) ? 2 : 1;
                }
                return 1;
            }
        case 6:
            if (func_80106BC8(globalCtx) != 0) {
                return 3;
            }
    }
    return 1;
}
//#else
//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97738.s")
//#endif

s32 func_80A97B38(EnKo* this) {
    s32 rv;
    if (gSaveContext.linkAge != 0) {
        if ((gSaveContext.eventChkInf[4] & 1) != 0) {
            return 2;
        }
        if (((gBitFlags[18]) & gSaveContext.inventory.questItems) != 0) {
            return 1;
        }
        return 0;
    }

    if ((*gBitFlags & gSaveContext.inventory.questItems) != 0) {
        rv = 4;
    } else {
        rv = 3;
    }
    return rv;
}

f32 func_80A97BC0(EnKo* this) {
    f32 D_80A9A62C[13][5] = { { 0.0f, 0.0f, 0.0f, -30.0f, -20.0f },       { 0.0f, 0.0f, 0.0f, -20.0f, -10.0f },
                              { 0.0f, 0.0f, 0.0f, -30.0f, -20.0f },       { -10.0f, 10.0f, 10.0f, -10.0f, -30.0f },
                              { 0.0f, 0.0f, 0.0f, -10.0f, -20.0f },       { 0.0f, 0.0f, 0.0f, -20.0f, -20.0f },
                              { 0.0f, 0.0f, 0.0f, -10.0f, -20.0f },       { 10.0f, 10.0f, 10.0f, -60.0f, -20.0f },
                              { -10.0f, -10.0f, -20.0f, -30.0f, -30.0f }, { -10.0f, -10.0f, -10.0f, -40.0f, -40.0f },
                              { 0.0f, 0.0f, 0.0f, -10.0f, -20.0f },       { -10.0f, -10.0f, -20.0f, -30.0f, -30.0f },
                              { 0.0f, 0.0f, 0.0f, -20.0f, -20.0f } };
    s32 iVar3;

    if ((gSaveContext.linkAge == 0) && ((this->actor.params & 0xFF) == 0xC)) {
        return -20.0f;
    }
    // return D_80A9A62C[(this->actor.params & 0xFF) + func_80A97B38(this)] ;
    iVar3 = func_80A97B38(this);

    return D_80A9A62C[this->actor.params & 0xFF][iVar3];
}

s32 D_80A9A730[] = { 0x01010100, 0x01010101, 0x01010101, 0x01000101, 0x01010001, 0x01010100,
                     0x01000000, 0x00000101, 0x01010101, 0x01010001, 0x00000000, 0x00000000,
                     0x00000101, 0x01010100, 0x00000000, 0x01010101, 0x01000000 };

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97C7C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97D00.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97D68.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97E18.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97EB0.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97F20.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A97F70.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98034.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98124.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98174.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98254.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A9835C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98464.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A9856C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98674.s")

void func_80A9877C(EnKo* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;

    if ((globalCtx->csCtx.state != 0) || (gDbgCamEnabled != 0)) {
        this->unk_1E8.unk_18 = globalCtx->view.eye;
        this->unk_1E8.unk_14 = 40.0f;
        if ((this->actor.params & 0xFF) != 0) {
            func_80034A14((Actor*)this, &this->unk_1E8, (u16)2, (u16)2);
        }
    } else {
        this->unk_1E8.unk_18 = player->actor.world.pos;
        this->unk_1E8.unk_14 = func_80A97BC0(this);
        if ((func_80A98ECC(this, globalCtx) == 0) && (this->unk_1E8.unk_00 == 0)) {
            return;
        }
    }
    if ((func_800343CC(globalCtx, &this->actor, &this->unk_1E8.unk_00, this->unk_21C, func_80A97610, func_80A97738) !=
         0) &&
        ((this->actor.params & 0xFF) == 0xC) && (globalCtx->sceneNum == 0x5B)) {
        this->actor.textId = gSaveContext.inventory.items[gItemSlots[0x2D]] >= 0x32 ? 0x10B9 : 0x10DF;

        if (func_8002F368(globalCtx) == 9) {
            this->actor.textId = (gSaveContext.infTable[11] & 0x1000) ? 0x10B8 : 0x10B7;
            this->unk_210 = 0;
        }
        player->actor.textId = this->actor.textId;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98934.s")

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98C18.s")
void func_80A98C18(EnKo* this) {
    void** temp_v1;
    s32 phi_v1;

    if (DECR(this->unk_214) == 0) {
        phi_v1 = D_80A9A500[this->actor.params & 0xFF].unk_0;
        this->unk_216++;
        temp_v1 = D_80A9A158[phi_v1].unk_8;
        if ((temp_v1 != NULL) && (temp_v1[this->unk_216] == NULL)) {
            this->unk_214 = Rand_S16Offset(30, 30);
            this->unk_216 = 0;
        }
    }
}

// Seems fake
s32 func_80A98CD8(EnKo* this) {
    s32 type;
    struct_80A9A590* temp_v1;

    temp_v1 = &D_80A9A590[this->actor.params & 0xFF];
    this->actor.targetMode = temp_v1->unk_0;
    type = this->actor.params & 0xFF;
    if (1) {}
    this->unk_21C = temp_v1->unk_4;
    this->unk_21C = this->unk_21C + this->collider.dim.radius;
    this->unk_218 = temp_v1->unk_8;
    return type;
}

s32 func_80A98D2C(EnKo* this) {
    if (LINK_IS_ADULT) {
        return (gBitFlags[0] & gSaveContext.inventory.questItems) ? 4 : 3;
    }
    if (gBitFlags[18] & gSaveContext.inventory.questItems) {
        return (gSaveContext.eventChkInf[4] & 1) ? 2 : 1;
    }
    return 0;
}

void func_80A98DB4(EnKo* this, GlobalContext* globalCtx) {
    f32 dist;
    f32 phi_f0;

    // temp_v0 = globalCtx->sceneNum;
    if ((globalCtx->sceneNum != SCENE_SPOT10) && (globalCtx->sceneNum != SCENE_SPOT04)) {
        this->unk_220 = 255.0f;
        return;
    }
    if ((globalCtx->csCtx.state != 0) || (gDbgCamEnabled != 0)) {
        dist = Math_Vec3f_DistXYZ(&this->actor.world.pos, &globalCtx->view.eye) * 0.25f;
    } else {
        dist = this->actor.xzDistToPlayer;
    }

    Math_SmoothStepToF(&this->unk_220, (this->unk_218 < dist) ? 0.0f : 255.0f, 0.3f, 40.0f, 1.0f);
    if (this->unk_220 < 10.0f) {
        this->actor.flags = this->actor.flags & ~1;
        return;
    }
    this->actor.flags = this->actor.flags | 1;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A98ECC.s")

void EnKo_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnKo* this = THIS;

    if (((thisx->params & 0xFF) >= 0xD) || (func_80A96F48(this, globalCtx) == 0) ||
        (func_80A96DB0(this, globalCtx) == 0)) {
        Actor_Kill(thisx);
    }
    if (func_80A98934(thisx, globalCtx) == 0) {
        Actor_Kill(thisx);
    }
    this->actionFunc = func_80A99048;
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/EnKo_Destroy.s")

void func_80A99048(EnKo* this, GlobalContext* globalCtx) {
    if ((func_80A96F94(this, globalCtx) != 0) && (func_80A96EC4(this, globalCtx) != 0)) {
        this->actor.flags &= ~0x10;
        this->actor.objBankIndex = this->unk_196;
        gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[this->actor.objBankIndex].segment);
        SkelAnime_InitFlex(globalCtx, &this->skelAnime, D_80A9A17C[D_80A9A500[this->actor.params & 0xFF].unk_6].unk_4,
                           NULL, this->limbDrawTable, this->transitionDrawTable, 16);
        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 18.0f);
        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[this->objectIndex].segment);
        Collider_InitCylinder(globalCtx, &this->collider);
        Collider_SetCylinder(globalCtx, &this->collider, &this->actor, &D_80A9A100);
        CollisionCheck_SetInfo2(&this->actor.colChkInfo, NULL, &D_80A9A12C);
        if ((this->actor.params & 0xFF) == 7) {
            // Angle Z
            osSyncPrintf(VT_BGCOL(BLUE) "  アングルＺ->(%d)\n" VT_RST, this->actor.shape.rot.z);
            if ((LINK_IS_ADULT) && ((gBitFlags[0] & gSaveContext.inventory.questItems) == 0)) {
                if (this->actor.shape.rot.z != 1) {
                    Actor_Kill(&this->actor);
                    return;
                }
            } else if (this->actor.shape.rot.z != 0) {
                Actor_Kill(&this->actor);
                return;
            }
        }
        if ((this->actor.params & 0xFF) == 5) {
            this->collider.base.ocFlags1 |= 0x40;
        }
        this->unk_212 = func_80A98D2C(this);
        func_80034EC0(&this->skelAnime, D_80A9A18C, D_80A9A4BC[this->actor.params & 0xFF][this->unk_212]);
        Actor_SetScale(&this->actor, 0.01f);
        func_80A98CD8(this);
        this->unk_220 = 0.0f;
        this->path = Path_GetByIndex(globalCtx, (this->actor.params & 0xFF00) >> 8, 0xFF);
        Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_ELF, this->actor.world.pos.x,
                           this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 3);
        if ((this->actor.params & 0xFF) == 3) {
            if ((gBitFlags[18] & gSaveContext.inventory.questItems) == 0) {
                this->collider.dim.height = this->collider.dim.height + 0xC8;
                this->actionFunc = func_80A995CC;
                return;
            }
            Path_CopyLastPoint(this->path, &this->actor.world.pos);
        }
        this->actionFunc = func_80A99384;
    }
}

void func_80A99384(EnKo* this, GlobalContext* globalCtx) {
    if (((this->actor.params & 0xFF) == 0xC) && (this->unk_1E8.unk_00 != 0) && (this->actor.textId == 0x10B9)) {
        func_80034EC0(&this->skelAnime, D_80A9A18C, 7);
        this->actionFunc = func_80A99438;
    } else if (((this->actor.params & 0xFF) == 0xC) && (this->unk_1E8.unk_00 == 2)) {
        this->actionFunc = func_80A99504;
        globalCtx->msgCtx.unk_E3E7 = 4;
        globalCtx->msgCtx.msgMode = 0x36;
    }
}

void func_80A99438(EnKo* this, GlobalContext* globalCtx) {
    if (((this->actor.params & 0xFF) == 0xC) && (this->unk_1E8.unk_00 == 2)) {
        func_80034EC0(&this->skelAnime, D_80A9A18C, 6);
        this->actionFunc = func_80A99504;
        globalCtx->msgCtx.unk_E3E7 = 4;
        globalCtx->msgCtx.msgMode = 0x36;
    } else if ((this->unk_1E8.unk_00 == 0) || (this->actor.textId != 0x10B9)) {
        func_80034EC0(&this->skelAnime, D_80A9A18C, 6);
        this->actionFunc = func_80A99384;
    }
}

void func_80A99504(EnKo* this, GlobalContext* globalCtx) {
    if (Actor_HasParent(&this->actor, globalCtx)) {
        this->actor.parent = NULL;
        this->actionFunc = func_80A99560;
    } else {
        func_8002F434(&this->actor, globalCtx, GI_SAW, 120.0f, 10.0f);
    }
}

void func_80A99560(EnKo* this, GlobalContext* globalCtx) {
    if (this->unk_1E8.unk_00 == 3) {
        this->actor.textId = 0x10B9;
        func_8010B720(globalCtx, this->actor.textId);
        this->unk_1E8.unk_00 = 1;
        gSaveContext.itemGetInf[3] |= 2;
        this->actionFunc = func_80A99384;
    }
}

#ifdef NON_MATCHING
void func_80A995CC(EnKo* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    f32 temp_f10;
    f32 temp_f2;
    s16 yawToPlayer;
    f32 phi_f0;

    yawToPlayer = Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos);
    this->actor.world.pos.x = this->actor.home.pos.x;
    temp_f10 = 80.0f * Math_SinS(yawToPlayer);
    this->actor.world.pos.x = this->actor.world.pos.x + temp_f10;
    this->actor.world.pos.z = this->actor.home.pos.z;
    // yawToLink = this->actor.yawTowardsLink;
    this->actor.world.pos.z += 80.0f * Math_CosS(yawToPlayer);
    this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    // this->actor.world.pos.z +=  temp_f10;
    if ((this->unk_1E8.unk_00 == 0) || (this->actor.isTargeted == 0)) {

        temp_f2 = fabsf((f32)this->actor.yawTowardsPlayer - yawToPlayer) * 0.001f * 3.0f;
        if (temp_f2 > 1.0f) {

            phi_f0 = CLAMP_MAX(temp_f2, 3.0f);
            this->skelAnime.playSpeed = phi_f0;
            return;
        }
    }
    this->skelAnime.playSpeed = 1.0f;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A995CC.s")
#endif

void EnKo_Update(Actor* thisx, GlobalContext* globalCtx) {
    ColliderCylinder* collider;
    EnKo* this = THIS;

    if (this->actionFunc != func_80A99048) {
        if ((s32)this->unk_220 != 0) {
            gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[this->objectIndex].segment);
            SkelAnime_Update(&this->skelAnime);
            func_80A98DB4(this, globalCtx);
            func_80A98C18(this);
        } else {
            func_80A98DB4(this, globalCtx);
        }
    }
    if (this->unk_1E8.unk_00 == 0) {
        Actor_MoveForward(&this->actor);
    }
    if (func_80A97C7C(this) != 0) {
        Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 0.0f, 0.0f, 0.0f, 4);
        this->actor.gravity = -1.0f;
    } else {
        this->actor.gravity = 0.0f;
    }
    this->actionFunc(this, globalCtx);
    func_80A9877C(this, globalCtx);
    collider = &this->collider;
    { s32 pad; } // Probably fake but works
    Collider_UpdateCylinder(&this->actor, collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, collider);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/func_80A99864.s")
/*s32 func_80A99864(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                  void* arg6) {
    EnKo* this = THIS;
    s16 sp40;
    void* temp_a1;
    s32 temp_a2;
    struct_80A9A158* temp_v0;
    void* temp_v1;
    void* temp_v1_2;
    void* temp_v1_3;

    if (limbIndex == 0xF) {
        // temp_v1 = *arg6;
        //*arg6 = (void *) (temp_v1 + 8);
        // temp_v1->unk0 = 0xDB060018;
        // temp_v1->unk4 = (void *) globalCtx->objectCtx.status[this->unk194].segment;
        gSPSegment(arg6++, 0x06, globalCtx->objectCtx.status[this->unk_194].segment);
        gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[this->unk_194].segment);
        temp_v0 = &D_80A9A158[D_80A9A500[this->actor.params & 0xFF].unk_0];
        *dList = temp_v0->gfx;
        temp_a1 = temp_v0->unk_8;
        if (temp_a1 != 0) {
            temp_a2 = temp_a1[this->unk_216];
            gSPSegment(arg6++, 0x0A, SEGMENTED_TO_VIRTUAL(temp_a2));
        }
        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[this->unk_196].segment);
    }
    if (limbIndex == 8) {
        sp40.unk0 = (?32) (unaligned s32) this->unk1F6;
        sp40.unk4 = (u16)this->unk1FA;
        Matrix_RotateX(((f32)(0 - sp42) / 32768.0f) * 3.1415927f, (u8)1U);
        Matrix_RotateZ(((f32)sp40 / 32768.0f) * 3.1415927f, (u8)1U);
    }
    if (limbIndex == 0xF) {
        Matrix_Translate(1200.0f, 0.0f, 0.0f, (u8)1U);
        sp40.unk0 = (?32) (unaligned s32) this->unk1F0;
        (&sp40)[2] = (s16)this->unk1F4;
        Matrix_RotateX(((f32)sp42 / 32768.0f) * 3.1415927f, (u8)1U);
        Matrix_RotateZ(((f32)sp40 / 32768.0f) * 3.1415927f, (u8)1U);
        Matrix_Translate(-1200.0f, 0.0f, 0.0f, (u8)1U);
    }
    if ((limbIndex == 8) || (limbIndex == 9) || (limbIndex == 0xC)) {
        temp_v1_3 = this + (limbIndex * 2);
        rot->y = (s16)(s32)((f32)rot->y + (Math_SinS(temp_v1_3->unk2E4) * 200.0f));
        rot->z = (s16)(s32)((f32)rot->z + (Math_CosS(temp_v1_3->unk304) * 200.0f));
    }
    return 0;
}*/

// Post limb draw
void func_80A99BC4(GlobalContext* globalCtx2, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    GlobalContext* globalCtx = globalCtx2;
    EnKo* this = THIS;
    Vec3f D_80A9A774 = { 0.0f, 0.0f, 0.0f };

    if (limbIndex == 7) {
        gSPSegment((*gfx)++, 0x06, globalCtx->objectCtx.status[this->unk_195].segment);
        gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[this->unk_195].segment);
    }
    if (limbIndex == 15) {
        Matrix_MultVec3f(&D_80A9A774, &this->actor.focus.pos);
    }
}

// AllocColorDList?
Gfx* func_80A99C94(GraphicsContext* gfxCtx, u8 r, u8 g, u8 b, u8 a) {
    Gfx* dList;

    dList = Graph_Alloc(gfxCtx, sizeof(Gfx) * 2);
    // dListHead = dList;
    gDPSetEnvColor(dList, r, g, b, a);
    gSPEndDisplayList(dList + 1);
    return dList;
}

// Type Conversion issues
#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ko/EnKo_Draw.s")
/*void EnKo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnKo* this = THIS;
    Color_RGBA8 color1;
    Color_RGBA8 color2;
    //GraphicsContext* gfxCtx;

    color1 = D_80A9A500[thisx->params & 0xFF].color1;
    color2 = D_80A9A500[thisx->params & 0xFF].color2;
    thisx->shape.unk_14 = this->unk_220;
    //gfxCtx = globalCtx->state.gfxCtx;
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ko.c", 0x82F);
    //temp_f0 = this->unk_220;
    //temp_v0 = (s16)(s32)temp_f0;
    if ((s16)this->unk_220 == 0xFF) {
        // temp_v0_2 = temp_s1->polyOpa.p;
        // temp_s1->polyOpa.p = temp_v0_2 + 8;
        // temp_v0_2->words.w0 = 0xDB060020;
        // sp44 = temp_v0_2;
        // sp44->words.w1 = func_80A99C94(globalCtx->state.gfxCtx, sp60, sp61, sp62, 0xFF);
        gSPSegment(POLY_OPA_DISP++, 0x08, func_80A99C94(globalCtx->state.gfxCtx, color1.r, color1.g, color1.b, 0xFF));
        gSPSegment(POLY_OPA_DISP++, 0x09, func_80A99C94(globalCtx->state.gfxCtx, color2.r, color2.g, color2.b, 0xFF));
        // temp_v0_3 = temp_s1->polyOpa.p;
        // temp_s1->polyOpa.p = temp_v0_3 + 8;
        // temp_v0_3->words.w0 = 0xDB060024;
        // sp40 = temp_v0_3;
        // sp40->words.w1 = func_80A99C94(globalCtx->state.gfxCtx, sp5C, sp5D, sp5E, 0xFF);
        // func_80034BA0(globalCtx, (SkelAnime*)(thisx + 0x14C), &func_80A99864, &func_80A99BC4, thisx,
        //              (s32)thisx->unk220);
        func_80034BA0(globalCtx, &this->skelAnime, &func_80A99864, &func_80A99BC4, thisx, this->unk_220);
    } else if ((s16)this->unk_220 != 0) {
        //sp63 = (u8)(u32)temp_f0;
        //sp5F = (u8)(u32)this->unk_220;
        gSPSegment(POLY_OPA_DISP++, 0x08, func_80A99C94(globalCtx->state.gfxCtx, color1.r, color1.g, color1.b,
(u8)(u32)this->unk_220));
        // temp_v0_4 = temp_s1->polyXlu.p;
        // temp_s1->polyXlu.p = temp_v0_4 + 8;
        // temp_v0_4->words.w0 = 0xDB060020;
        // sp3C = temp_v0_4;
        // sp3C->words.w1 = func_80A99C94(globalCtx->state.gfxCtx, sp60, sp61, sp62, (?32) sp63);
        gSPSegment(POLY_OPA_DISP++, 0x09, func_80A99C94(globalCtx->state.gfxCtx, color2.r, color2.g, color2.b,
(u8)(u32)this->unk_220)); func_80034CC4(globalCtx, &this->skelAnime, &func_80A99864, &func_80A99BC4, thisx,
(s32)this->unk_220);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ko.c", 0x858);
}
*/