#include "z_bg_jya_bombiwa.h"

#define FLAGS 0x00000000

#define THIS ((BgJyaBombiwa*)thisx)

void BgJyaBombiwa_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBombiwa_Draw(Actor* thisx, GlobalContext* globalCtx);

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
};
*/
static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_F4, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_F8, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_FC, 1000, ICHAIN_STOP),
};
extern UNK_TYPE D_0600E710;

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/func_808951A0.s")
void func_808951A0(BgJyaBombiwa* this, GlobalContext* globalCtx, void* arg2, s32 arg3) {
    s32 localConst;
    localConst = 0;

    DynaPolyInfo_SetActorMove(&this->dyna, arg3);
    DynaPolyInfo_Alloc(arg2, &localConst);
    this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, globalCtx + 0x810, &this->dyna.actor, localConst);
    if (this->dyna.dynaPolyId == 0x32) {
        osSyncPrintf( "Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_bombiwa.c", 0xAE, this->dyna.actor.id,  this->dyna.actor.params);
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/func_80895228.s")

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/BgJyaBombiwa_Init.s")
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

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/BgJyaBombiwa_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/func_8089539C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/BgJyaBombiwa_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/BgJyaBombiwa_Draw.s")
