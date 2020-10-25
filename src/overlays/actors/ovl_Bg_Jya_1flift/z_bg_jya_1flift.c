/*
 * File: z_bg_jya_1flift.c
 * Overlay: ovl_Bg_Jya_1flift
<<<<<<< HEAD
 * Description: Shortcut Elevator (Spirit Temple) Unused in Master Quest
 * !!!THIS FILE HAS BEEN MODIFIED! DO NOT USE FOR AN ACTUAL DECOMPILED SOURCE!!!
 */
=======
 * Description: Shortcut Elevator used in the vanilla version of the Spirit Temple
 */

#include "z_bg_jya_1flift.h"
>>>>>>> upstream/master

#include "z_bg_jya_1flift.h"
#include <alloca.h>
#define FLAGS 0x00000030

#define THIS ((BgJya1flift*)thisx)

void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx);

<<<<<<< HEAD
void BgJya1flift_Move(BgJya1flift* this, GlobalContext* globalCtx);

void printinfo(BgJya1flift* this, GlobalContext* globalCtx);
extern u8 D_808930E0 = 0;
=======
void func_80892DB0(BgJya1flift* this);
void func_80892DCC(BgJya1flift* this, GlobalContext* globalCtx);
void BgJya1flift_DoNothing(BgJya1flift* this, GlobalContext* globalCtx);
void BgJya1flift_ChangeDirection(BgJya1flift* this);
void BgJya1flift_Move(BgJya1flift* this, GlobalContext* globalCtx);
void func_80892E0C(BgJya1flift* this);
void BgJya1flift_ResetMoveDelay(BgJya1flift* this);
void BgJya1flift_DelayMove(BgJya1flift* this, GlobalContext* globalCtx);

u8 sHasSpawned = false;
>>>>>>> upstream/master

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
<<<<<<< HEAD

static f32 sSpeed = 5.0f;

f32 D_80893130[] = { 443.0f, -50.0f };
=======

static ColliderCylinderInit sCylinderInit = {
    { COLTYPE_UNK10, 0x00, 0x00, 0x39, 0x20, COLSHAPE_CYLINDER },
    { 0x00, { 0x00000000, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, 0x00, 0x00, 0x01 },
    { 70, 80, -82, { 0, 0, 0 } },
};

f32 finalPositions[] = { 443.0f, -50.0f };
>>>>>>> upstream/master

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_STOP),
};

extern UNK_TYPE D_060004A8;
extern Gfx D_060001F0[];

<<<<<<< HEAD
void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;
    s32 sp30;
    this->hasInitialized = 0;
    sp30 = 0;
    DynaPolyInfo_SetActorMove(&this->dyna, 0);
    DynaPolyInfo_Alloc(&D_060004A8, &sp30);
    this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp30);
    Actor_ProcessInitChain(thisx, sInitChain);
    thisx->room = -1;
    this->dyna.actor.posRot.pos.y= this->dyna.actor.initPosRot.pos.y;
    this->actionFunc = BgJya1flift_Move;
    this->hasInitialized = 1;
    thisx->speedXZ = 5.0f;
    thisx->gravity = 0.0f;

=======
void BgJya1flift_InitDynapoly(BgJya1flift* this, GlobalContext* globalCtx, UNK_PTR arg2, s32 moveFlag) {
    s32 pad;
    s32 localConst;
    s32 pad2;

    localConst = 0;
    DynaPolyInfo_SetActorMove(&this->dyna, moveFlag);
    DynaPolyInfo_Alloc(arg2, &localConst);
    this->dyna.dynaPolyId =
        DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, localConst);

    if (this->dyna.dynaPolyId == 0x32) {
        // Warning : move BG login failed
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_1flift.c", 0xB3,
                     this->dyna.actor.id, this->dyna.actor.params);
    }
}

void BgJya1flift_InitCollision(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;

    Collider_InitCylinder(globalCtx, &this->collider);
    Collider_SetCylinder(globalCtx, &this->collider, &this->dyna.actor, &sCylinderInit);
    this->dyna.actor.colChkInfo.mass = 0xFF;
}

void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;
    // 1 F lift
    osSyncPrintf("(１Ｆリフト)(flag %d)(room %d)\n", sHasSpawned, globalCtx->roomCtx.curRoom.num);
    this->hasInitialized = false;
    if (sHasSpawned) {
        Actor_Kill(thisx);
        return;
    }
    BgJya1flift_InitDynapoly(this, globalCtx, &D_060004A8, 0);
    Actor_ProcessInitChain(thisx, sInitChain);
    BgJya1flift_InitCollision(thisx, globalCtx);
    if (Flags_GetSwitch(globalCtx, (thisx->params & 0x3F))) {
        LINK_AGE_IN_YEARS == YEARS_ADULT ? BgJya1flift_ChangeDirection(this) : func_80892E0C(this);
    } else {
        func_80892DB0(this);
    }
    thisx->room = -1;
    sHasSpawned = true;
    this->hasInitialized = true;
>>>>>>> upstream/master
}

void BgJya1flift_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;

<<<<<<< HEAD
    if (this->hasInitialized != 0) {
        D_808930E0 = 0;
=======
    if (this->hasInitialized) {
        sHasSpawned = false;
        Collider_DestroyCylinder(globalCtx, &this->collider);
>>>>>>> upstream/master
        DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
    }
}

<<<<<<< HEAD

void BgJya1flift_Move(BgJya1flift* this, GlobalContext* globalCtx) {
    Input* input;
    Player* player = PLAYER;

    input = globalCtx->state.input;
    if(CHECK_PAD(input[0].cur, U_CBUTTONS)){
        this->dyna.actor.posRot.pos = player->actor.posRot.pos;
    }

    if (func_8004356C(&this->dyna)){
        if(CHECK_PAD(input[0].cur, A_BUTTON)){
            this->dyna.actor.speedXZ += 5.0f;
        }
        if(CHECK_PAD(input[0].cur, D_CBUTTONS)){
            if(this->dyna.actor.speedXZ != 5.0f)
                this->dyna.actor.speedXZ -= 5.0f;            
        }
        if (CHECK_PAD(input[0].cur, L_CBUTTONS)) {
            this->dyna.actor.posRot.pos.y -= 5.0f;
            player->actor.posRot.pos.x = this->dyna.actor.posRot.pos.x;
            player->actor.posRot.pos.z = this->dyna.actor.posRot.pos.z;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        if (CHECK_PAD(input[0].cur, R_CBUTTONS)) {
            this->dyna.actor.posRot.pos.y += 5.0f;
            player->actor.posRot.pos = this->dyna.actor.posRot.pos;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        if(CHECK_PAD(input[0].cur, U_JPAD)){
            Actor_MoveForward(&this->dyna.actor);
            player->actor.posRot.pos.x = this->dyna.actor.posRot.pos.x;
            player->actor.posRot.pos.z = this->dyna.actor.posRot.pos.z;
        }
        if(CHECK_PAD(input[0].cur, L_JPAD)){
            this->dyna.actor.posRot.rot.y += 500;
            this->dyna.actor.shape.rot.y += 500;
            player->actor.shape.rot.y = this->dyna.actor.shape.rot.y;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        if(CHECK_PAD(input[0].cur, R_JPAD)){
            this->dyna.actor.posRot.rot.y -= 500;
            this->dyna.actor.shape.rot.y -= 500;
            player->actor.shape.rot.y = this->dyna.actor.shape.rot.y;
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
    }
}


void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;
    this->actionFunc(this, globalCtx);
    printinfo(this, globalCtx);
    gSaveContext.dayTime = 0x5000;
}
void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, D_060001F0);
}

void printinfo(BgJya1flift* this, GlobalContext* globalCtx) {
    GfxPrint* printer;
    GraphicsContext* gfxCtx;
    Player* player = PLAYER;

    gfxCtx = globalCtx->state.gfxCtx;
    printer = alloca(sizeof(GfxPrint));
    GfxPrint_Init(printer);
    GfxPrint_Open(printer, gfxCtx->polyOpa.p);

    GfxPrint_SetPos(printer, 0, 8);
    GfxPrint_Printf(printer, "LINK POS Y: %.0f", player->actor.posRot.pos.y);
    GfxPrint_SetPos(printer, 0, 9);
    GfxPrint_Printf(printer, "LINK POS X: %.0f", player->actor.posRot.pos.x);
    GfxPrint_SetPos(printer, 0, 10);
    GfxPrint_Printf(printer, "LINK POS Z: %.0f", player->actor.posRot.pos.z);
    GfxPrint_SetColor(printer, 255, 255, 255, 255);
    GfxPrint_SetPos(printer, 0, 12);
    GfxPrint_Printf(printer, "LIFT POS Y: %.0f", this->dyna.actor.posRot.pos.y);
    GfxPrint_SetPos(printer, 0, 13);
    GfxPrint_Printf(printer, "LIFT POS X: %.0f", this->dyna.actor.posRot.pos.x);
    GfxPrint_SetPos(printer, 0, 14);
    GfxPrint_Printf(printer, "LIFT POS Z: %.0f", this->dyna.actor.posRot.pos.z);
    GfxPrint_SetPos(printer, 0, 15);
    GfxPrint_Printf(printer, "LIFT ROT Y: %d", this->dyna.actor.posRot.rot.y);
    GfxPrint_SetPos(printer, 0,0);
    GfxPrint_Printf(printer, "github.com/louist103/oot");
    gfxCtx->polyOpa.p = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);
}
=======
void func_80892DB0(BgJya1flift* this) {
    this->actionFunc = func_80892DCC;
    this->dyna.actor.posRot.pos.y = finalPositions[0];
}

void func_80892DCC(BgJya1flift* this, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (this->dyna.actor.params & 0x3F))) {
        BgJya1flift_ChangeDirection(this);
    }
}

void func_80892E0C(BgJya1flift* this) {
    this->actionFunc = BgJya1flift_DoNothing;
    this->dyna.actor.posRot.pos.y = finalPositions[0];
}

void BgJya1flift_DoNothing(BgJya1flift* this, GlobalContext* globalCtx) {
}

void BgJya1flift_ChangeDirection(BgJya1flift* this) {
    this->actionFunc = BgJya1flift_Move;
    this->isMovingDown ^= true;
    this->dyna.actor.velocity.y = 0.0f;
}

void BgJya1flift_Move(BgJya1flift* this, GlobalContext* globalCtx) {
    f32 tempVelocity;

    Math_ApproxF(&this->dyna.actor.velocity.y, 6.0f, 0.4f);
    if (this->dyna.actor.velocity.y < 1.0f) {
        tempVelocity = 1.0f;
    } else {
        tempVelocity = this->dyna.actor.velocity.y;
    }
    if (fabsf(Math_SmoothScaleMaxMinF(&this->dyna.actor.posRot.pos.y, (finalPositions[this->isMovingDown]), 0.5f,
                                      tempVelocity, 1.0f)) < 0.001f) {
        this->dyna.actor.posRot.pos.y = finalPositions[this->isMovingDown];
        BgJya1flift_ResetMoveDelay(this);
        Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
    } else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
}

void BgJya1flift_ResetMoveDelay(BgJya1flift* this) {
    this->actionFunc = BgJya1flift_DelayMove;
    this->moveDelay = 0;
}

void BgJya1flift_DelayMove(BgJya1flift* this, GlobalContext* globalCtx) {
    this->moveDelay++;
    if (this->moveDelay >= 21) {
        BgJya1flift_ChangeDirection(this);
    }
}

void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;
    s32 pad;
    s32 tempIsRiding;

    // Room 0 is the first room and 6 is the room that the lift starts on
    if (globalCtx->roomCtx.curRoom.num == 6 || globalCtx->roomCtx.curRoom.num == 0) {
        this->actionFunc(this, globalCtx);
        if (globalCtx) {}
        tempIsRiding = func_8004356C(&this->dyna) ? true : false;
        if ((this->actionFunc == BgJya1flift_Move) || (this->actionFunc == BgJya1flift_DelayMove)) {
            if (tempIsRiding) {
                func_8005A77C(globalCtx->cameraPtrs[0], 0x30);
            } else if (!tempIsRiding && this->isLinkRiding) {
                func_8005A77C(globalCtx->cameraPtrs[0], 3);
            }
        }
        this->isLinkRiding = tempIsRiding;
        Collider_CylinderUpdate(thisx, &this->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->collider);
    } else {
        Actor_Kill(thisx);
    }
}

void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, D_060001F0);
}
>>>>>>> upstream/master
