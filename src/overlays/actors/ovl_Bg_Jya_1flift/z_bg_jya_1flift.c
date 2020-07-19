/*
 * File: z_bg_jya_1flift.c
 * Overlay: ovl_Bg_Jya_1flift
 * Description: Shortcut Elevator (Spirit Temple) Unused in Master Quest
 */

#include "z_bg_jya_1flift.h"
#include <alloca.h>
#define FLAGS 0x00000010

#define THIS ((BgJya1flift*)thisx)

void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgJya1flift_InitDynapoly(BgJya1flift* this, GlobalContext* globalCtx, void* arg2, s32 arg3);
void BgJya1flift_ChangeDirection(BgJya1flift* this);
void BgJya1flift_Move(BgJya1flift* this, GlobalContext* globalCtx);

void printinfo(Player* player, BgJya1flift* this, GlobalContext* globalCtx);
extern u8 D_808930E0 = 0;

const ActorInit Bg_Jya_1flift_InitVars = {
    ACTOR_BG_JYA_1FLIFT,
    ACTORTYPE_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJya1flift),
    (ActorFunc)BgJya1flift_Init,
    (ActorFunc)BgJya1flift_Destroy,
    (ActorFunc)BgJya1flift_Update,
    (ActorFunc)BgJya1flift_Draw,
};

f32 D_80893130[] = { 443.0f, -50.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_STOP),
};

extern UNK_TYPE D_060004A8;
extern Gfx D_060001F0[];

void BgJya1flift_InitDynapoly(BgJya1flift* this, GlobalContext* globalCtx, void* arg2, s32 moveFlag) {
    s32 sp30;
    sp30 = 0;
    DynaPolyInfo_SetActorMove(&this->dyna, moveFlag);
    DynaPolyInfo_Alloc(arg2, (void*)&sp30);
    this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp30);
    
}

void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;
    this->hasInitialized = 0;
    if (D_808930E0 != 0) {
        Actor_Kill(thisx);
        return;
    }
    BgJya1flift_InitDynapoly(this, globalCtx, &D_060004A8, 0);
    Actor_ProcessInitChain(thisx, sInitChain);
    thisx->room = -1;
    D_808930E0 = 1;
    this->dyna.actor.posRot.pos.y= this->dyna.actor.initPosRot.pos.y;
    this->actionFunc = BgJya1flift_Move;
    this->hasInitialized = 1;
}

void BgJya1flift_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;

    if (this->hasInitialized != 0) {
        D_808930E0 = 0;
        DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
    }
}


void BgJya1flift_Move(BgJya1flift* this, GlobalContext* globalCtx) {
    Input* input;
    Player* player = PLAYER;

    input = globalCtx->state.input;
    if (func_8004356C(&this->dyna)){
        if (CHECK_PAD(input[0].cur, A_BUTTON)) {
            this->dyna.actor.posRot.pos.y += 5.0f;
            player->actor.posRot.pos.y = this->dyna.actor.posRot.pos.y;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        if (CHECK_PAD(input[0].cur, B_BUTTON)) {
            this->dyna.actor.posRot.pos.y -= 5.0f;
            player->actor.posRot.pos.y = this->dyna.actor.posRot.pos.y;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        if (CHECK_PAD(input[0].cur, L_JPAD)) {
            this->dyna.actor.posRot.pos.x -= 5.0f;
            player->actor.posRot.pos.x = this->dyna.actor.posRot.pos.x;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        if (CHECK_PAD(input[0].cur, R_JPAD)) {
            this->dyna.actor.posRot.pos.x += 5.0f;
            player->actor.posRot.pos.x = this->dyna.actor.posRot.pos.x;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        if (CHECK_PAD(input[0].cur, D_JPAD)) {
            this->dyna.actor.posRot.pos.z -= 5.0f;
            player->actor.posRot.pos.z = this->dyna.actor.posRot.pos.z;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        if (CHECK_PAD(input[0].cur, U_JPAD)) {
            this->dyna.actor.posRot.pos.z += 5.0f;
            player->actor.posRot.pos.z = this->dyna.actor.posRot.pos.z;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        } 
    }
}


void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;
    Player* player = PLAYER;
    this->actionFunc(this, globalCtx);
    printinfo(player, this, globalCtx);
    return;
}
void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, D_060001F0);
}

void printinfo(Player* player, BgJya1flift* this, GlobalContext* globalCtx) {
    GfxPrint* printer;
    GraphicsContext* gfxCtx;
    gfxCtx = globalCtx->state.gfxCtx;
    printer = alloca(sizeof(GfxPrint));
    GfxPrint_Init(printer);
    GfxPrint_Open(printer, gfxCtx->polyOpa.p);
    GfxPrint_SetPos(printer, 1, 8);
    GfxPrint_Printf(printer, "LINK POS Y: %.0f", player->actor.posRot.pos.y);
    GfxPrint_SetPos(printer, 1, 9);
    GfxPrint_Printf(printer, "LINK POS X: %.0f", player->actor.posRot.pos.x);
    GfxPrint_SetPos(printer, 1, 10);
    GfxPrint_Printf(printer, "LINK POS Z: %.0f", player->actor.posRot.pos.z);
    GfxPrint_SetColor(printer, 255, 255, 255, 255);
    GfxPrint_SetPos(printer, 1, 11);
    GfxPrint_Printf(printer, "LIFT POS Y: %.0f", this->dyna.actor.posRot.pos.y);
    GfxPrint_SetPos(printer, 1, 12);
    GfxPrint_Printf(printer, "LIFT POS X: %.0f", this->dyna.actor.posRot.pos.x);
    GfxPrint_SetPos(printer, 1, 13);
    GfxPrint_Printf(printer, "LIFT POS Z: %.0f", this->dyna.actor.posRot.pos.z);
    gfxCtx->polyOpa.p = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);
}