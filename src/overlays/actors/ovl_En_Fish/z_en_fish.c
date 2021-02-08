#include "z_en_fish.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

#define FLAGS 0x00000000

#define THIS ((EnFish*)thisx)

void EnFish_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFish_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFish_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFish_Draw(Actor* thisx, GlobalContext* globalCtx);

// extern FlexSkeletonHeader D_04018FE0;
// extern AnimationHeader D_0401909C;
// extern AnimationHeader D_040185FC;

/*
// D_80A1701C
static ColliderJntSphItemInit sJntSphItemsInit[1] = {
    {
        { 0x00, { 0x00000000, 0x00, 0x00 }, { 0xFFCFFFFF, 0x00, 0x00 }, 0x00, 0x00, 0x01 },
        { 0, { { 0, 0, 0 }, 5 }, 100 },
    },
};
// D_80A17040
static ColliderJntSphInit sJntSphInit =
{
    { COLTYPE_UNK10, 0x00, 0x00, 0x39, 0x10, COLSHAPE_JNTSPH },
    1, sJntSphItemsInit,
};
*/

static ColliderJntSphElementInit sJntSphItemsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 5 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphItemsInit,
};

const ActorInit En_Fish_InitVars = {
    ACTOR_EN_FISH,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnFish),
    (ActorFunc)EnFish_Init,
    (ActorFunc)EnFish_Destroy,
    (ActorFunc)EnFish_Update,
    (ActorFunc)EnFish_Draw,
};

// D_80A17070

void EnFish_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFish* this = THIS;
    // this->actor.colChkInfo.mass = 0x32U;
    thisx->gravity = -1.0f;
    thisx->shape.rot.x = 0;
    thisx->shape.rot.z = 0x4000;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 8.0f);
    SkelAnime_Init(globalCtx, &this->skelAnime, &gFieldUnusedFishSkel, &gFieldUnusedFishAnim, this->limbDrawTable,
                   this->unk_21A, 6);
}
u8 hitFloor;
void EnFish_Update(Actor* thisx, GlobalContext* globalCtx) {
    if ((thisx->floorHeight != thisx->world.pos.y)&&(hitFloor==false)) {
        Actor_MoveForward(thisx);
    } else if(hitFloor == false) {
        hitFloor = true;
        thisx->world.pos.y += 2.0f;
    }
    Actor_UpdateBgCheckInfo(globalCtx, thisx, 75.0f, 30.0f, 30.0f, 5);
    SkelAnime_Update(&THIS->skelAnime);
}

void EnFish_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}
// GfxPrint printer;
void EnFish_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnFish* this = THIS;
    // GfxPrint_Init(&printer);
    // GfxPrint_Open(&printer,globalCtx->state.gfxCtx->polyOpa.p);
    // GfxPrint_SetPos(&printer,0,0);
    // GfxPrint_SetColor(&printer,255,255,255,255);
    // GfxPrint_Printf(&printer, "Fish ROT %x",(s16)this->actor.shape.rot.z);
    // GfxPrint_SetPos(&printer,0,01);
    // GfxPrint_Printf(&printer,"Fish rot* %x",&thisx->shape.rot);
    // globalCtx->state.gfxCtx->polyOpa.p = GfxPrint_Close(&printer);
    // GfxPrint_Destroy(&printer);
    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable, 0, 0, thisx);
}