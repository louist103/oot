#include "z_bg_ydan_maruta.h"

#define FLAGS 0x00000000

#define THIS ((BgYdanMaruta*)thisx)

void BgYdanMaruta_Init(Actor* thisx, GlobalContext* globalCtx);
void BgYdanMaruta_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgYdanMaruta_Update(Actor* thisx, GlobalContext* globalCtx);
void BgYdanMaruta_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808BEFF4(BgYdanMaruta* this, GlobalContext* globalCtx);
void func_808BF25C(BgYdanMaruta* this, GlobalContext* globalCtx);
void func_808BF078(BgYdanMaruta* this, GlobalContext* globalCtx);

const ActorInit Bg_Ydan_Maruta_InitVars = {
    ACTOR_BG_YDAN_MARUTA,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_YDAN_OBJECTS,
    sizeof(BgYdanMaruta),
    (ActorFunc)BgYdanMaruta_Init,
    (ActorFunc)BgYdanMaruta_Destroy,
    (ActorFunc)BgYdanMaruta_Update,
    (ActorFunc)BgYdanMaruta_Draw,
};

//s32 D_808BF300[] = {0x00000000, 0x20000000, 0x00040000, 0x00000004, 0x00000000, 0x11010000, 0x435C0000, 0xC1200000, 0x00000000, 0x435C0000, 0x41200000, 0x00000000, 0xC35C0000, 0x41200000, 0x00000000};
static ColliderTrisItemInit sTrisItemsInit[1] = {
    {
        { 0x00, { 0x20000000, 0x00, 0x04 }, { 0x00000004, 0x00, 0x00 }, 0x11, 0x01, 0x00 },
        { { { 220.0f, -10.0f, 0.0f }, { 220.0f, 10.0f, 0.0f }, { -220.0f, 10.0f, 0.0f } } },
    },
};
s32 D_808BF33C[] = {0x00000000, 0x20000000, 0x00040000, 0x00000004, 0x00000000, 0x11010000, 0x41800000, 0x00000000, 0x00000000, 0x41800000, 0x43070000, 0x00000000, 0xC1800000, 0x43070000, 0x00000000};

//s32 D_808BF378[] = {0x0A110900, 0x20020000, 0x00000002, 0x808BF300};
static ColliderTrisInit sTrisInit =
{
    { COLTYPE_UNK10, 0x11, 0x09, 0x00, 0x20, COLSHAPE_TRIS },
    2, sTrisItemsInit,
};

//s32 D_808BF388[] = {0x48500064, 0x00000000};
static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

extern UNK_TYPE D_060066A8;

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/BgYdanMaruta_Init.s")
void BgYdanMaruta_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanMaruta* this = THIS;
    ? sp70;
    Vec3f sp64;
    Vec3f sp60;
    //f32 sp60;
    //f32 sp5C;
    //f32 sp58;
    Vec3f sp4C;
    f32 sinRotY;
  //  f32 sp40;
    s32 localConst;
    //ColliderTris *sp2C;
   // ? *temp_v0;
    Vec3f temp_v0_2;
    //ColliderTris *temp_a1;
    //f32 *temp_t7;
    f32 cosRotY;
    f32 temp_f16;
    f32 temp_f16_2;
   // s16 temp_t8;
   // s32 *temp_v1;
    s32 *temp_v1_2;
    void *temp_v1_3;
    f32 phi_f16;
    Vec3f phi_v0;
    s32 *phi_v1;
    f32 phi_f16_2;
    Vec3f phi_v0_2;
   // s32 *phi_v1_2;
    s32 *phi_s1;

    localConst = 0;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    //temp_a1 = &this->collider;
    //sp2C = temp_a1;
    Collider_InitTris(globalCtx, &this->collider);
    Collider_SetTris(globalCtx, &this->collider, (Actor *) &this->dyna.actor, &sTrisInit, &this->items);
   // temp_t8 = this->dyna.actor.params;
    this->unk_168 = this->dyna.actor.params;
    this->dyna.actor.params = (s16) (((s32) this->dyna.actor.params >> 8) & 0xFF);
    if (this->dyna.actor.params == 0) {
        this->actionFunc = func_808BEFF4;
        phi_s1 = &sTrisItemsInit;
    } else {
        DynaPolyInfo_SetActorMove(&this->dyna, 0);
        DynaPolyInfo_Alloc(D_060066A8, (void *) &localConst);
        this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna,&this->dyna.actor, localConst);
        this->dyna.actor.initPosRot.pos.y = (f32) (this->dyna.actor.initPosRot.pos.y + -280.0f);
        if (Flags_GetSwitch(globalCtx, (s32) this->unk_168) != 0) {
            this->actionFunc = func_808BF25C;
            this->dyna.actor.posRot.pos.y = this->dyna.actor.initPosRot.pos.y;
            phi_s1 = D_808BF33C;
        } else {
            this->actionFunc = func_808BF078;
            phi_s1 = D_808BF33C;
        }
    }
    sinRotY = Math_Sins(this->dyna.actor.shape.rot.y);
    cosRotY = Math_Coss(this->dyna.actor.shape.rot.y);
   // temp_v1 = phi_s1;
   // temp_v0 = &sp4C + 0xC;
    temp_f16 = phi_s1->unk18 * cosRotY;
    phi_f16 = temp_f16;
    phi_v0 = &sp4C + 0xC;;
    phi_v1 = phi_s1;
    phi_f16_2 = phi_s1;
    phi_v0_2 = &sp4C + 0xC;;
  //  phi_v1_2 = phi_s1;
    if ((&sp4C + 0xC) != &sp70) {
loop_6:
        temp_v0_2 = phi_v0 + 0xC;
        temp_v1_2 = phi_v1 + 0xC;
        temp_v0_2.x = (phi_f16 + this->dyna.actor.posRot.pos.x);
        temp_v0_2.y = (temp_v1_2->unk10 + this->dyna.actor.posRot.pos.y);
        temp_v0_2.z = (this->dyna.actor.posRot.pos.z - (temp_v1_2->unkC * sinRotY));
        temp_f16_2 = temp_v1_2->unk18 * cosRotY;
        phi_f16 = temp_f16_2;
        phi_v0 = temp_v0_2;
        phi_v1 = temp_v1_2;
        phi_f16_2 = temp_f16_2;
        phi_v0_2 = temp_v0_2;
       // phi_v1_2 = temp_v1_2;
        if (temp_v0_2 != &sp70) {
            goto loop_6;
        }
    }
    //temp_v1_3 = phi_v1_2 + 0xC;
    temp_v1_3 = temp_v1_2 + 0xC;
    phi_v0_2.x = (phi_f16_2 + this->dyna.actor.posRot.pos.x);
    phi_v0_2.y = (temp_v1_3->unk10 + this->dyna.actor.posRot.pos.y);
    phi_v0_2.z = (this->dyna.actor.posRot.pos.z - (temp_v1_3->unkC * sinRotY));
   // sinRotY = sinRotY;
  //  sp40 = cosRotY;
    func_800627A0(&this->collider, 0, (Vec3f *) &sp4C, &sp60, &sp64);
    //temp_t7 = sp60.x;
    sp60.x = (phi_s1->unk30 * cosRotY) + this->dyna.actor.posRot.pos.x;
    sp60.y = phi_s1->unk1C + this->dyna.actor.posRot.pos.y;
    sp60.z = this->dyna.actor.posRot.pos.z - (phi_s1->unk30 * sinRotY);
    func_800627A0(&this->collider, 1, (Vec3f *) &sp4C, &sp64, &sp60);
}




#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/BgYdanMaruta_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/func_808BEFF4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/func_808BF078.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/func_808BF108.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/func_808BF1EC.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/func_808BF25C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/BgYdanMaruta_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_Bg_Ydan_Maruta/BgYdanMaruta_Draw.s")
