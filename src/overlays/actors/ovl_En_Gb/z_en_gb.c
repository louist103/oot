/*
 * File: z_en_gb.c
 * Overlay: ovl_En_Gb
 * Description: Poe Seller
 */

#include "z_en_gb.h"
#include "objects/object_ps/object_ps.h"

#define FLAGS 0x00000009

#define THIS ((EnGb*)thisx)

void EnGb_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGb_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGb_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A2F83C(EnGb* self, GlobalContext* globalCtx);
void func_80A2FC70(EnGb* self, GlobalContext* globalCtx);
void func_80A2FA50(EnGb* self, GlobalContext* globalCtx);
void func_80A2F9C0(EnGb* self, GlobalContext* globalCtx);
void func_80A2F94C(EnGb* self, GlobalContext* globalCtx);
void func_80A2FB40(EnGb* self, GlobalContext* globalCtx);
void func_80A2FBB0(EnGb* self, GlobalContext* globalCtx);
void func_80A2FC0C(EnGb* self, GlobalContext* globalCtx);

void EnGb_DrawCagedSouls(EnGb* self, GlobalContext* globalCtx);
void EnGb_UpdateCagedSouls(EnGb* self, GlobalContext* globalCtx);

const ActorInit En_Gb_InitVars = {
    ACTOR_EN_GB,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_PS,
    sizeof(EnGb),
    (ActorFunc)EnGb_Init,
    (ActorFunc)EnGb_Destroy,
    (ActorFunc)EnGb_Update,
    (ActorFunc)EnGb_Draw,
};

static EnGbCagedSoulInfo sCagedSoulInfo[] = {
    { { 255, 255, 170, 255 }, { 255, 200, 0, 255 }, gPoeSellerAngrySoulTex, -15 },
    { { 255, 255, 170, 255 }, { 0, 150, 0, 255 }, gPoeSellerHappySoulTex, -12 },
    { { 255, 170, 255, 255 }, { 100, 0, 150, 255 }, gPoeSellerSadSoulTex, -8 },
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
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
    { 40, 75, 0, { 0, 0, 0 } },
};

static ColliderCylinderInitType1 sBottlesCylindersInit[] = {
    {
        {
            COLTYPE_NONE,
            AT_NONE,
            AC_NONE,
            OC1_ON | OC1_TYPE_ALL,
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
        { 4, 20, 0, { 0, 0, 0 } },
    },
    {
        {
            COLTYPE_NONE,
            AT_NONE,
            AC_NONE,
            OC1_ON | OC1_TYPE_ALL,
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
        { 4, 20, 0, { 0, 0, 0 } },
    },
    {
        {
            COLTYPE_NONE,
            AT_NONE,
            AC_NONE,
            OC1_ON | OC1_TYPE_ALL,
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
        { 10, 20, 0, { 0, 0, 0 } },
    },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 6, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 2200, ICHAIN_STOP),
};

// relative positions of poe souls
static Vec3f sCagedSoulPositions[] = {
    { -8.0f, 112.0f, -8.0f },
    { -3.0f, 112.0f, 29.0f },
    { 31.0f, 112.0f, 29.0f },
    { 31.0f, 112.0f, -8.0f },
};

// relative positions of bottles
static Vec3f sBottlesPositions[] = {
    { -48.0f, 0.0f, 34.0f },
    { -55.0f, 0.0f, 49.0f },
    { -48.0f, 0.0f, 60.0f },
};

void func_80A2F180(EnGb* self) {
    if (gSaveContext.infTable[0xB] & 0x40) {
        self->textId = 0x70F5;
    } else {
        self->textId = 0x70F4;
    }
}

void EnGb_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGb* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 i;
    f32 rand;
    Vec3f focusOffset;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&gPoeSellerCol, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gPoeSellerSkel, &gPoeSellerIdleAnim, self->jointTable,
                       self->morphTable, 12);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, &self->dyna.actor, &sCylinderInit);

    for (i = 0; i < ARRAY_COUNT(sBottlesCylindersInit); i++) {
        Collider_InitCylinder(globalCtx, &self->bottlesColliders[i]);
        Collider_SetCylinderType1(globalCtx, &self->bottlesColliders[i], &self->dyna.actor, &sBottlesCylindersInit[i]);
    }

    self->light = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y,
                              self->dyna.actor.home.pos.z, 255, 255, 255, 200);

    ActorShape_Init(&self->dyna.actor.shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
    Actor_SetScale(&self->dyna.actor, 0.01f);
    self->dyna.actor.colChkInfo.mass = 0xFF;
    self->dyna.actor.speedXZ = 0.0f;
    self->dyna.actor.velocity.y = 0.0f;
    self->dyna.actor.gravity = -1.0f;
    self->actionTimer = (s16)Rand_ZeroFloat(100.0f) + 100;

    for (i = 0; i < ARRAY_COUNT(sCagedSoulPositions); i++) {
        self->cagedSouls[i].infoIdx = (s32)Rand_ZeroFloat(30.0f) % 3;
        self->cagedSouls[i].unk_14.x = self->cagedSouls[i].translation.x =
            sCagedSoulPositions[i].x + self->dyna.actor.world.pos.x;
        self->cagedSouls[i].unk_14.y = self->cagedSouls[i].translation.y =
            sCagedSoulPositions[i].y + self->dyna.actor.world.pos.y;
        self->cagedSouls[i].unk_14.z = self->cagedSouls[i].translation.z =
            sCagedSoulPositions[i].z + self->dyna.actor.world.pos.z;
        self->cagedSouls[i].unk_1 = 1;
        self->cagedSouls[i].unk_3 = self->cagedSouls[i].unk_2 = 0;
        self->cagedSouls[i].unk_20 = self->cagedSouls[i].unk_24 = 0.0f;
        self->cagedSouls[i].unk_6 = Rand_ZeroFloat(40.0f);
        self->cagedSouls[i].rotate180 = self->cagedSouls[i].unk_6 & 1;
        self->cagedSouls[i].unk_28 = 0.2f;
    }

    rand = Rand_ZeroOne();
    self->lightColor.r = (s8)(rand * 30.0f) + 225;
    self->lightColor.g = (s8)(rand * 100.0f) + 155;
    self->lightColor.b = (s8)(rand * 160.0f) + 95;
    self->lightColor.a = 200;
    Matrix_Translate(self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z,
                     MTXMODE_NEW);
    Matrix_RotateRPY(self->dyna.actor.world.rot.x, self->dyna.actor.world.rot.y, self->dyna.actor.world.rot.z,
                     MTXMODE_APPLY);
    focusOffset.x = focusOffset.y = 0.0f;
    focusOffset.z = 44.0f;
    Matrix_MultVec3f(&focusOffset, &self->dyna.actor.focus.pos);
    self->dyna.actor.focus.pos.y += 62.5f;
    func_80A2F180(self);
    self->actionFunc = func_80A2F83C;
}

void EnGb_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGb* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->light);
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_80A2F608(EnGb* self) {
    s32 i;
    Vec3f sp48;
    Vec3f sp3C;

    Matrix_Translate(self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z,
                     MTXMODE_NEW);
    Matrix_RotateRPY(self->dyna.actor.world.rot.x, self->dyna.actor.world.rot.y, self->dyna.actor.world.rot.z,
                     MTXMODE_APPLY);
    sp48.x = sp48.y = 0.0f;
    sp48.z = 25.0f;
    Matrix_MultVec3f(&sp48, &sp3C);
    self->collider.dim.pos.x = sp3C.x;
    self->collider.dim.pos.y = sp3C.y;
    self->collider.dim.pos.z = sp3C.z;

    for (i = 0; i < ARRAY_COUNT(sBottlesPositions); i++) {
        Matrix_Translate(self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z,
                         MTXMODE_NEW);
        Matrix_RotateRPY(self->dyna.actor.world.rot.x, self->dyna.actor.world.rot.y, self->dyna.actor.world.rot.z,
                         MTXMODE_APPLY);
        Matrix_MultVec3f(&sBottlesPositions[i], &sp3C);
        self->bottlesColliders[i].dim.pos.x = sp3C.x;
        self->bottlesColliders[i].dim.pos.y = sp3C.y;
        self->bottlesColliders[i].dim.pos.z = sp3C.z;
    }
}

s32 func_80A2F760(EnGb* self) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(self->cagedSouls); i++) {
        if (self->cagedSouls[i].unk_3) {
            return 1;
        }
    }
    return 0;
}

void func_80A2F7C0(EnGb* self) {
    Animation_Change(&self->skelAnime, &gPoeSellerSwingStickAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gPoeSellerSwingStickAnim), ANIMMODE_ONCE, 0.0f);
    Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_NALE_MAGIC);
    self->actionFunc = func_80A2FC70;
}

void func_80A2F83C(EnGb* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (!func_80A2F760(self)) {
        if (self->actionTimer != 0) {
            self->actionTimer--;
        } else {
            func_80A2F7C0(self);
            return;
        }
    }
    if (func_8002F194(&self->dyna.actor, globalCtx)) {
        switch (func_8002F368(globalCtx)) {
            case EXCH_ITEM_NONE:
                func_80A2F180(self);
                self->actionFunc = func_80A2F94C;
                break;
            case EXCH_ITEM_POE:
                player->actor.textId = 0x70F6;
                self->actionFunc = func_80A2F9C0;
                break;
            case EXCH_ITEM_BIG_POE:
                player->actor.textId = 0x70F7;
                self->actionFunc = func_80A2FA50;
                break;
        }
        return;
    }
    if (self->dyna.actor.xzDistToPlayer < 100.0f) {
        func_8002F298(&self->dyna.actor, globalCtx, 100.0f, EXCH_ITEM_POE);
    }
}

void func_80A2F94C(EnGb* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        if (!(gSaveContext.infTable[0xB] & 0x40)) {
            gSaveContext.infTable[0xB] |= 0x40;
        }
        func_80A2F180(self);
        self->actionFunc = func_80A2F83C;
    }
}

void func_80A2F9C0(EnGb* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        if (!(gSaveContext.infTable[0xB] & 0x40)) {
            gSaveContext.infTable[0xB] |= 0x40;
        }
        func_80A2F180(self);
        Player_UpdateBottleHeld(globalCtx, GET_PLAYER(globalCtx), ITEM_BOTTLE, PLAYER_AP_BOTTLE);
        Rupees_ChangeBy(10);
        self->actionFunc = func_80A2F83C;
    }
}

void func_80A2FA50(EnGb* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        if (!(gSaveContext.infTable[0xB] & 0x40)) {
            gSaveContext.infTable[0xB] |= 0x40;
        }
        func_80A2F180(self);
        Player_UpdateBottleHeld(globalCtx, GET_PLAYER(globalCtx), ITEM_BOTTLE, PLAYER_AP_BOTTLE);
        Rupees_ChangeBy(50);
        HIGH_SCORE(HS_POE_POINTS) += 100;
        if (HIGH_SCORE(HS_POE_POINTS) != 1000) {
            if (HIGH_SCORE(HS_POE_POINTS) > 1100) {
                HIGH_SCORE(HS_POE_POINTS) = 1100;
            }
            self->actionFunc = func_80A2F83C;
        } else {
            Player* player = GET_PLAYER(globalCtx);

            player->exchangeItemId = EXCH_ITEM_NONE;
            self->textId = 0x70F8;
            func_8010B720(globalCtx, self->textId);
            self->actionFunc = func_80A2FB40;
        }
    }
}

void func_80A2FB40(EnGb* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        func_8002F434(&self->dyna.actor, globalCtx, GI_BOTTLE, 100.0f, 10.0f);
        self->actionFunc = func_80A2FBB0;
    }
}

void func_80A2FBB0(EnGb* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->dyna.actor, globalCtx)) {
        self->dyna.actor.parent = NULL;
        self->actionFunc = func_80A2FC0C;
    } else {
        func_8002F434(&self->dyna.actor, globalCtx, GI_BOTTLE, 100.0f, 10.0f);
    }
}

void func_80A2FC0C(EnGb* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        func_8002F194(&self->dyna.actor, globalCtx);
        func_80A2F180(self);
        self->actionFunc = func_80A2F83C;
    }
}

void func_80A2FC70(EnGb* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame == Animation_GetLastFrame(&gPoeSellerSwingStickAnim)) {
        Animation_Change(&self->skelAnime, &gPoeSellerIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gPoeSellerIdleAnim),
                         ANIMMODE_LOOP, 0.0f);
        self->actionFunc = func_80A2F83C;
    } else if (self->skelAnime.curFrame == 18.0f) {
        self->cagedSouls[1].unk_1 = 3;
        self->cagedSouls[1].unk_3 = 1;
        self->cagedSouls[2].unk_1 = 3;
        self->cagedSouls[2].unk_3 = 1;
        self->cagedSouls[3].unk_1 = 3;
        self->cagedSouls[3].unk_3 = 1;
        if (self->actionFunc) {} // these ifs cannot just contain a constant
        self->cagedSouls[0].unk_1 = 3;
        self->cagedSouls[0].unk_3 = 1;
        if (self->actionFunc) {}
        self->actionTimer = (s16)Rand_ZeroFloat(600.0f) + 600;
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_WOOD_HIT);
    }
}

void EnGb_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnGb* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    s32 i;
    f32 rand;

    self->frameTimer++;
    SkelAnime_Update(&self->skelAnime);
    self->actionFunc(self, globalCtx);
    self->dyna.actor.textId = self->textId;
    func_80A2F608(self);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

    for (i = 0; i < ARRAY_COUNT(self->bottlesColliders); i++) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->bottlesColliders[i].base);
    }

    rand = Rand_ZeroOne();
    self->lightColor.r = (s8)(rand * 30.0f) + 225;
    self->lightColor.g = (s8)(rand * 100.0f) + 155;
    self->lightColor.b = (s8)(rand * 160.0f) + 95;
    self->lightColor.a = 200;
    EnGb_UpdateCagedSouls(self, globalCtx);
}

void EnGb_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnGb* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_gb.c", 763);

    func_80093D18(globalCtx->state.gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, self->lightColor.r, self->lightColor.g, self->lightColor.b, 255);

    Lights_PointNoGlowSetInfo(&self->lightInfo, self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y,
                              self->dyna.actor.world.pos.z, self->lightColor.r, self->lightColor.g, self->lightColor.b,
                              self->lightColor.a);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, &self->dyna.actor);
    EnGb_DrawCagedSouls(self, globalCtx);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_gb.c", 796);
}

void EnGb_UpdateCagedSouls(EnGb* self, GlobalContext* globalCtx) {
    f32 temp_f20;
    s16 rot;
    s32 i;

    for (i = 0; i < 4; i++) {
        switch (self->cagedSouls[i].unk_1) {
            case 0:
                Math_ApproachF(&self->cagedSouls[i].unk_20, 1.0f, 0.02f, self->cagedSouls[i].unk_24);
                Math_ApproachF(&self->cagedSouls[i].unk_24, 1.0f, 0.001f, 1.0f);
                if ((self->cagedSouls[i].unk_28 - .01f) <= self->cagedSouls[i].unk_20) {
                    self->cagedSouls[i].unk_20 = self->cagedSouls[i].unk_28;
                    self->cagedSouls[i].unk_24 = 0.0f;
                    self->cagedSouls[i].unk_1 = 1;
                    self->cagedSouls[i].unk_2 = 2;
                    self->cagedSouls[i].unk_6 = 0;
                }
                break;
            case 1:
                if (self->cagedSouls[i].unk_6 != 0) {
                    self->cagedSouls[i].unk_6--;
                } else {
                    self->cagedSouls[i].unk_3 = 0;
                    self->cagedSouls[i].unk_24 = 0.0f;
                    self->cagedSouls[i].unk_1 = self->cagedSouls[i].unk_2;
                }
                break;
            case 2:
                Math_ApproachF(&self->cagedSouls[i].unk_20, 0.0f, 0.02f, self->cagedSouls[i].unk_24);
                Math_ApproachF(&self->cagedSouls[i].unk_24, 1.0f, 0.001f, 1.0f);
                if (self->cagedSouls[i].unk_20 <= 0.01f) {
                    self->cagedSouls[i].unk_28 = self->cagedSouls[i].unk_28 + 0.2f;
                    if (self->cagedSouls[i].unk_28 > 1.0f) {
                        self->cagedSouls[i].unk_28 = 1.0f;
                    }
                    self->cagedSouls[i].unk_20 = 0.0f;
                    self->cagedSouls[i].unk_24 = 0.0f;
                    self->cagedSouls[i].unk_1 = 1;
                    self->cagedSouls[i].unk_2 = 0;
                    self->cagedSouls[i].unk_6 = 0;
                }
                break;
            case 3:
                Math_ApproachF(&self->cagedSouls[i].unk_20, 0.0f, 0.5f, 1.0f);
                if (self->cagedSouls[i].unk_20 <= 0.01f) {
                    self->cagedSouls[i].unk_28 = 0.2f;
                    self->cagedSouls[i].unk_20 = 0.0f;
                    self->cagedSouls[i].unk_24 = 0.0f;
                    self->cagedSouls[i].unk_1 = 1;
                    self->cagedSouls[i].unk_2 = 0;
                    self->cagedSouls[i].unk_6 = (s16)Rand_ZeroFloat(60.0f) + 60;
                }
                break;
        }

        temp_f20 = self->cagedSouls[i].unk_20 * 60.0f;
        if ((i == 0) || (i == 3)) {
            self->cagedSouls[i].translation.x = self->cagedSouls[i].unk_14.x;
            self->cagedSouls[i].translation.y = self->cagedSouls[i].unk_14.y + temp_f20;
            self->cagedSouls[i].translation.z = self->cagedSouls[i].unk_14.z;
        } else if (i == 1) {
            rot = self->dyna.actor.world.rot.y - 0x4000;
            self->cagedSouls[i].translation.x = self->cagedSouls[i].unk_14.x + Math_SinS(rot) * temp_f20;
            self->cagedSouls[i].translation.z = self->cagedSouls[i].unk_14.z + Math_CosS(rot) * temp_f20;
            self->cagedSouls[i].translation.y = self->cagedSouls[i].unk_14.y;
        } else {
            rot = self->dyna.actor.world.rot.y + 0x4000;
            self->cagedSouls[i].translation.x = self->cagedSouls[i].unk_14.x + Math_SinS(rot) * temp_f20;
            self->cagedSouls[i].translation.z = self->cagedSouls[i].unk_14.z + Math_CosS(rot) * temp_f20;
            self->cagedSouls[i].translation.y = self->cagedSouls[i].unk_14.y;
        }
    }
}

void EnGb_DrawCagedSouls(EnGb* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 i;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_gb.c", 914);

    func_80093D84(globalCtx->state.gfxCtx);

    for (i = 0; i < 4; i++) {
        s32 idx = self->cagedSouls[i].infoIdx;

        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0,
                                    (u32)(sCagedSoulInfo[idx].timerMultiplier * self->frameTimer) % 512, 32, 128));
        gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sCagedSoulInfo[idx].texture));
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, sCagedSoulInfo[idx].prim.r, sCagedSoulInfo[idx].prim.g,
                        sCagedSoulInfo[idx].prim.b, sCagedSoulInfo[idx].prim.a);
        gDPSetEnvColor(POLY_XLU_DISP++, sCagedSoulInfo[idx].env.r, sCagedSoulInfo[idx].env.g, sCagedSoulInfo[idx].env.b,
                       sCagedSoulInfo[idx].env.a);

        Matrix_Push();
        Matrix_Translate(self->cagedSouls[i].translation.x, self->cagedSouls[i].translation.y,
                         self->cagedSouls[i].translation.z, MTXMODE_NEW);
        func_800D1FD4(&globalCtx->mf_11DA0);

        if (self->cagedSouls[i].rotate180) {
            Matrix_RotateRPY(0, -0x8000, 0, MTXMODE_APPLY);
        }
        Matrix_Scale(0.007f, 0.007f, 1.0f, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_gb.c", 955),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gPoeSellerCagedSoulDL);

        Matrix_Pop();
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_gb.c", 962);
}
