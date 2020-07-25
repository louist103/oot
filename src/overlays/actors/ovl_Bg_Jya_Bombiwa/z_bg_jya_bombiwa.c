#include "z_bg_jya_bombiwa.h"

#define FLAGS 0x00000000

#define THIS ((BgJyaBombiwa*)thisx)

void BgJyaBombiwa_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808951A0(BgJyaBombiwa* this, GlobalContext* globalCtx, void* arg2, DynaPolyMoveFlag flag);
void func_80895228(BgJyaBombiwa* this, GlobalContext* globalCtx);
/*
const ActorInit Bg_Jya_Bombiwa_InitVars = {
    ACTOR_BG_JYA_BOMBIWA,
    ACTORTYPE_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaBombiwa),
    (ActorFunc)BgJyaBombiwa_Init,
    (ActorFunc)BgJyaBombiwa_Destroy,
    (ActorFunc)BgJyaBombiwa_Update,
    (ActorFunc)BgJyaBombiwa_Draw,
};*/
static ColliderJntSphItemInit sJntSphItemsInit[1] = {
    {
        { 0x00, { 0x00000000, 0x00, 0x00 }, { 0x00000008, 0x00, 0x00 }, 0x00, 0x01, 0x00 },
        { 0, { { 0, 0, 0 }, 50 }, 100 },
    },
};
static ColliderJntSphInit sJntSphInit = {
    { COLTYPE_UNK10, 0x00, 0x09, 0x00, 0x00, COLSHAPE_JNTSPH },
    1,
    &sJntSphItemsInit,
};
static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_F4, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_F8, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_FC, 1000, ICHAIN_STOP),
};





extern UNK_TYPE D_0600E710;
extern Gfx D_0600E490[];
extern Gfx D_0600EDC0[];
extern UNK_TYPE D_80895854 = 0x3FE66666;
extern f32 D_80895858 = 0.4f;

    //#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/func_808951A0.s")
void func_808951A0(BgJyaBombiwa* this, GlobalContext* globalCtx, void* arg2, DynaPolyMoveFlag flag) {//Name to BgJyaBombiwa_InitDynapoly
    s16 pad1;
    s32 localConst = 0;
    s16 pad2;

    DynaPolyInfo_SetActorMove(&this->dyna, flag);
    DynaPolyInfo_Alloc(arg2, &localConst);
    this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, localConst);
    if (this->dyna.dynaPolyId == 0x32) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_bombiwa.c", 174,
                     this->dyna.actor.id, this->dyna.actor.params);
    }
    
}

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/func_80895228.s")
void func_80895228(BgJyaBombiwa* this, GlobalContext* globalCtx) {//Name to BgJyaBombiwa_InitCollision
    Actor* actor = &this->dyna.actor;

    Collider_InitJntSph(globalCtx, &this->collider);
    Collider_SetJntSph(globalCtx, &this->collider, actor, &sJntSphInit, &this->colliderItems);
}


void BgJyaBombiwa_Init(Actor* thisx, GlobalContext* globalCtx) {
    if ((thisx->params & 0x3F) != 0x29) {
        osSyncPrintf("\x1b[43;30m");
        osSyncPrintf("Ｗａｒｎｉｎｇ : Switch Number が変更された(%s %d)(SW %d)\n",
                     "../z_bg_jya_bombiwa.c", 0xDA, thisx->params & 0x3F);
        osSyncPrintf("\x1b[m");
    }
    func_808951A0(thisx, globalCtx, &D_0600E710, 0);
    func_80895228(thisx, globalCtx);
    if (Flags_GetSwitch(globalCtx, (s32)(thisx->params & 0x3F)) != 0) {
        Actor_Kill(thisx);
        return;
    }
    Actor_ProcessInitChain(thisx, sInitChain);
    osSyncPrintf((const char*)"(jya 爆弾で破壊岩)(arg_data 0x%04x)\n", thisx->params);
}

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/BgJyaBombiwa_Destroy.s")
void BgJyaBombiwa_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBombiwa* this = THIS;

    DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
    Collider_DestroyJntSph(globalCtx, &this->collider);
}

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/func_8089539C.s")
void func_8089539C(BgJyaBombiwa* this, GlobalContext* globalCtx) {
    Vec3f temp;
    Vec3f temp2;
    s16 tempS;
    s8 rotationSpeed;
    s8 burstVelocity;
    s32 i;
    s16 tScale;

    for (i = 0; i != 0x10; i++) {
        temp.x = ((Math_Rand_ZeroOne() * 80.0f) + this->dyna.actor.posRot.pos.x) - 40.0f;
        temp.y = (Math_Rand_ZeroOne() * 140.0f) + this->dyna.actor.posRot.pos.y;
        temp.z = ((Math_Rand_ZeroOne() * 80.0f) + this->dyna.actor.posRot.pos.z) - 40.0f;
        temp2.x = (Math_Rand_ZeroOne() - 0.5f) * 10.0f;
        temp2.y = Math_Rand_ZeroOne() * 12.0f;
        temp2.z = (Math_Rand_ZeroOne() - 0.5f) * 10.0f;

        tScale = (s32)(i * 1.8f) + 3;
        if (tScale > 15) {
            tempS = 5;
        } else {
            tempS = 1;
        }
        if (Math_Rand_ZeroOne() < 0.4f) {
            tempS |= 0x40;
            rotationSpeed = 0xC;
            burstVelocity = 8;
        } else {
            tempS |= 0x20;

            rotationSpeed = 0xC;
            burstVelocity = 8;
            if (tScale < 10) {
                rotationSpeed = 0x50;
                burstVelocity = 80;
            }
        } //                                          Gravity influence
        func_80029E8C(globalCtx, &temp, &temp2, &temp, -400, tempS, rotationSpeed, burstVelocity, 0, tScale, 1, 20, 80,
                      -1, OBJECT_JYA_OBJ, D_0600EDC0);
    }
    temp.x = this->dyna.actor.posRot.pos.x;
    temp.y = this->dyna.actor.posRot.pos.y + 70.0f;
    temp.z = this->dyna.actor.posRot.pos.z;
    func_80033480(globalCtx, &temp, 100.0f, 0xA, 0x64, 0xA0, 1);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/BgJyaBombiwa_Update.s")


void BgJyaBombiwa_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBombiwa* this = THIS;

    Gfx_DrawDListOpa(globalCtx, D_0600E490);
    func_800628A4(0, &this->collider);
}