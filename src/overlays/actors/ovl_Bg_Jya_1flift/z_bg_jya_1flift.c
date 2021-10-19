/*
 * File: z_bg_jya_1flift.c
 * Overlay: ovl_Bg_Jya_1flift
 * Description: Shortcut Elevator used in the vanilla version of the Spirit Temple
 */

#include "z_bg_jya_1flift.h"
#include "objects/object_jya_obj/object_jya_obj.h"

#define FLAGS 0x00000030

#define THIS ((BgJya1flift*)thisx)

void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgJya1flift_WaitForSwitch(BgJya1flift* this, GlobalContext* globalCtx);
void BgJya1flift_DoNothing(BgJya1flift* this, GlobalContext* globalCtx);
void BgJya1flift_ChangeDirection(BgJya1flift* this);
void BgJya1flift_Move(BgJya1flift* this, GlobalContext* globalCtx);
void BgJya1flift_ResetMoveDelay(BgJya1flift* this);
void BgJya1flift_DelayMove(BgJya1flift* this, GlobalContext* globalCtx);

static u8 sIsSpawned = false;

const ActorInit Bg_Jya_1flift_InitVars = {
    ACTOR_BG_JYA_1FLIFT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJya1flift),
    (ActorFunc)BgJya1flift_Init,
    (ActorFunc)BgJya1flift_Destroy,
    (ActorFunc)BgJya1flift_Update,
    (ActorFunc)BgJya1flift_Draw,
};

// typedef struct {
//    f32 initialPos[2];
//    f32 finalPos[2];
//} LiftPositions;

// static LiftPositions sPositions = { { 1480.0f, -183.0f }, { 1480.0f, -1000.0f } };

static f32 sFinalPositions[] = { -183.0f, -1000.0f };
// Move between         { 1480  -183}, and 1480, -1000

void BgJya1flift_InitDynapoly(BgJya1flift* this, GlobalContext* globalCtx, CollisionHeader* collision, s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&this->dyna, moveFlag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);

    if (this->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning : move BG login failed"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_jya_1flift.c", 179,
                     this->dyna.actor.id, this->dyna.actor.params);
    }
}

void BgJya1flift_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;
    // "1 F lift"
    osSyncPrintf("(１Ｆリフト)(flag %d)(room %d)\n", sIsSpawned, globalCtx->roomCtx.curRoom.num);
    if (sIsSpawned) {
        Actor_Kill(thisx);
        return;
    }
    this->dyna.actor.scale.x *= 10.0f;
    this->dyna.actor.scale.y *= 10.0f;
    this->dyna.actor.scale.z *= 10.0f;
    //
    BgJya1flift_InitDynapoly(this, globalCtx, &g1fliftCol, 0);
    thisx->room = -1;
    sIsSpawned = true;
}

void BgJya1flift_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJya1flift* this = THIS;

    if (sIsSpawned) {
        sIsSpawned = false;
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
    }
}


void BgJya1flift_Move(BgJya1flift* this, GlobalContext* globalCtx) {
    f32 tempVelocity;

    Math_StepToF(&this->dyna.actor.velocity.z, 6.0f, 0.4f);
    if (this->dyna.actor.velocity.z < 1.0f) {
        tempVelocity = 1.0f;
    } else {
        tempVelocity = this->dyna.actor.velocity.z;
    }
    if (fabsf(Math_SmoothStepToF(&this->dyna.actor.world.pos.z, (sFinalPositions[this->isMovingDown]), 0.5f,
                                 tempVelocity, 1.0f)) < 0.001f) {
        this->dyna.actor.world.pos.z = sFinalPositions[this->isMovingDown];
        this->isMovingDown ^= true;
        this->dyna.actor.velocity.z = 0.0f;
        Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_BLOCK_BOUND);
    } else {
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE3 - SFX_FLAG);
    }
}

void BgJya1flift_Update(Actor* thisx, GlobalContext* globalCtx2) {
    BgJya1flift* this = THIS;
    GlobalContext* globalCtx = globalCtx2;
    s32 tempIsRiding;

    if (this->actionFunc != NULL) {
        this->actionFunc(this, globalCtx);
    }
    tempIsRiding = func_8004356C(&this->dyna) ? true : false;
    if (tempIsRiding) {
        this->actionFunc = BgJya1flift_Move;
    }

    // this->isLinkRiding = tempIsRiding;
}

void BgJya1flift_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, g1fliftDL);
}
