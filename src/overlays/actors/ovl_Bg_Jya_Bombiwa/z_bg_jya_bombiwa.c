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
};
*/
static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_F4, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_F8, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_FC, 1000, ICHAIN_STOP),
};
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


extern UNK_TYPE D_0600E710;
extern Gfx D_0600E490[];
extern u32 D_0600EDC0;

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
    //f32 spC4;
   // f32 spC0;
    //f32 spBC;
    f32 spB8;
    f32 spB4;
    f32 spB0;

   // s32 temp_s0;
    //s32 temp_s1;
    //s32 temp_s2;
    s32 phi_s2;
    s32 phi_s0;
    s32 phi_s3;
    s32 phi_s4;
    s32 phi_s0_2;

    phi_s2 = 0;
    do { //loop1
        this->dyna.actor.posRot.pos.x = ((Math_Rand_ZeroOne() * 80.0f) + this->dyna.actor.posRot.pos.x) - 40.0f; // spBC
        this->dyna.actor.posRot.pos.y = (Math_Rand_ZeroOne() * 140.0f) + this->dyna.actor.posRot.pos.y;          // spC0
        this->dyna.actor.posRot.pos.z = ((Math_Rand_ZeroOne() * 80.0f) + this->dyna.actor.posRot.pos.z) - 40.0f; // spC4
        spB0 = (Math_Rand_ZeroOne() - 0.5f) * 10.0f;
        spB4 = Math_Rand_ZeroOne() * 12.0f;
        spB8 = (Math_Rand_ZeroOne() - 0.5f) * 10.0f;
        //temp_s1 =(((s32)((f32)phi_s2 * 1.8f) + 3) << 0x10) >> 0x10;
        phi_s0_2 = 1;
        if ((((s32)((f32)phi_s2 * 1.8f) + 3) << 0x10) >> 0x10 >= 0x10) {
            phi_s0_2 = 5;
        }
        if (Math_Rand_ZeroOne() < 0.4f) {
            phi_s0 = (s32)((phi_s0_2 | 0x40) << 0x10) >> 0x10;
            phi_s3 = 0xC;
            phi_s4 = 8;
        } else {
            //temp_s0 = (s32)((phi_s0_2 | 0x20) << 0x10) >> 0x10;
            //phi_s0 = (s32)((phi_s0_2 | 0x20) << 0x10) >> 0x10;
            phi_s3 = 0xC;
            phi_s4 = 8;
            if ((((s32)((f32)phi_s2 * 1.8f) + 3) << 0x10) >> 0x10 < 0xA) {
               // phi_s0 = temp_s0;
                phi_s3 = 0x50;
                phi_s4 = 0x50;
            }
        }
        func_80029E8C(globalCtx, (Vec3f*)&this->dyna.actor.posRot.pos.z, (Vec3f*)&spB0, (Vec3f*)&this->dyna.actor.posRot.pos.z, -0x190, phi_s0,
                      phi_s3, phi_s4, 0,
                      (((s32)((f32)phi_s2 * 1.8f) + 3) << 0x10) >> 0x10, 1, 0x14, 0x50, -1, 0xF1, D_0600EDC0);
        //temp_s2 = phi_s2 + 1;
        phi_s2 ++;
    } while (phi_s2 != 0x10);
    //if (temp_s2 != 0x10) {
    //    goto loop_1;
    //}
    //spBC = this->dyna.actor.posRot.pos.x;
   // spC0 = this->dyna.actor.posRot.pos.y + 70.0f;
   // spC4 = this->dyna.actor.posRot.pos.z;
    func_80033480(globalCtx, (Vec3f*)&this->dyna.actor.posRot.pos.z, 100.0f, 0xA, 0x64, 0xA0, 1);
}
#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Jya_Bombiwa/BgJyaBombiwa_Update.s")


void BgJyaBombiwa_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBombiwa* this = THIS;

    Gfx_DrawDListOpa(globalCtx, D_0600E490);
    func_800628A4(0, &this->collider);
}