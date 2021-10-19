#include "z_en_ossan.h"
#include "vt.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_ossan/object_ossan.h"
#include "objects/object_oF1d_map/object_oF1d_map.h"
#include "objects/object_os/object_os.h"
#include "objects/object_zo/object_zo.h"
#include "objects/object_rs/object_rs.h"
#include "objects/object_ds2/object_ds2.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "objects/object_masterkokiri/object_masterkokiri.h"
#include "objects/object_km1/object_km1.h"
#include "objects/object_mastergolon/object_mastergolon.h"
#include "objects/object_masterzoora/object_masterzoora.h"
#include "objects/object_masterkokirihead/object_masterkokirihead.h"

#define FLAGS 0x00000019

#define THIS ((EnOssan*)thisx)

void EnOssan_Init(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_Update(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_DrawKokiriShopkeeper(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_DrawPotionShopkeeper(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_DrawBombchuShopkeeper(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_DrawBazaarShopkeeper(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_DrawZoraShopkeeper(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_DrawGoronShopkeeper(Actor* thisx, GlobalContext* globalCtx);
void EnOssan_DrawHappyMaskShopkeeper(Actor* thisx, GlobalContext* globalCtx);

void EnOssan_InitActionFunc(EnOssan* self, GlobalContext* globalCtx);
void EnOssan_MainActionFunc(EnOssan* self, GlobalContext* globalCtx);

void EnOssan_TalkDefaultShopkeeper(GlobalContext* globalCtx);
void EnOssan_TalkKokiriShopkeeper(GlobalContext* globalCtx);
void EnOssan_TalkKakarikoPotionShopkeeper(GlobalContext* globalCtx);
void EnOssan_TalkBombchuShopkeeper(GlobalContext* globalCtx);
void EnOssan_TalkMarketPotionShopkeeper(GlobalContext* globalCtx);
void EnOssan_TalkBazaarShopkeeper(GlobalContext* globalCtx);
void EnOssan_TalkZoraShopkeeper(GlobalContext* globalCtx);
void EnOssan_TalkGoronShopkeeper(GlobalContext* globalCtx);
void EnOssan_TalkHappyMaskShopkeeper(GlobalContext* globalCtx);

s16 ShopItemDisp_Default(s16 v);
s16 ShopItemDisp_SpookyMask(s16 v);
s16 ShopItemDisp_SkullMask(s16 v);
s16 ShopItemDisp_BunnyHood(s16 v);
s16 ShopItemDisp_ZoraMask(s16 v);
s16 ShopItemDisp_GoronMask(s16 v);
s16 ShopItemDisp_GerudoMask(s16 v);

void EnOssan_InitKokiriShopkeeper(EnOssan* self, GlobalContext* globalCtx);
void EnOssan_InitPotionShopkeeper(EnOssan* self, GlobalContext* globalCtx);
void EnOssan_InitBombchuShopkeeper(EnOssan* self, GlobalContext* globalCtx);
void EnOssan_InitBazaarShopkeeper(EnOssan* self, GlobalContext* globalCtx);
void EnOssan_InitZoraShopkeeper(EnOssan* self, GlobalContext* globalCtx);
void EnOssan_InitGoronShopkeeper(EnOssan* self, GlobalContext* globalCtx);
void EnOssan_InitHappyMaskShopkeeper(EnOssan* self, GlobalContext* globalCtx);

void EnOssan_State_Idle(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_StartConversation(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_FacingShopkeeper(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_TalkingToShopkeeper(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_LookToLeftShelf(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_LookToRightShelf(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_BrowseLeftShelf(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_BrowseRightShelf(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_LookFromShelfToShopkeeper(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_ItemSelected(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_SelectMilkBottle(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_SelectWeirdEgg(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_SelectUnimplementedItem(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_SelectBombs(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_CantGetItem(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_GiveItemWithFanfare(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_ItemPurchased(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_ContinueShoppingPrompt(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_GiveLonLonMilk(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_DisplayOnlyBombDialog(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_WaitForDisplayOnlyBombDialog(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_21(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_22(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_QuickBuyDialog(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_SelectMaskItem(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_LendMaskOfTruth(EnOssan* self, GlobalContext* globalCtx, Player* player);
void EnOssan_State_GiveDiscountDialog(EnOssan* self, GlobalContext* globalCtx, Player* player);

void EnOssan_Obj3ToSeg6(EnOssan* self, GlobalContext* globalCtx);

void EnOssan_StartShopping(GlobalContext* globalCtx, EnOssan* self);

void EnOssan_WaitForBlink(EnOssan* self);
void EnOssan_Blink(EnOssan* self);

u16 EnOssan_SetupHelloDialog(EnOssan* self);

s32 EnOssan_TakeItemOffShelf(EnOssan* self);
s32 EnOssan_ReturnItemToShelf(EnOssan* self);
void EnOssan_ResetItemPosition(EnOssan* self);
void EnOssan_SetStateGiveDiscountDialog(GlobalContext* globalCtx, EnOssan* self);

#define CURSOR_INVALID 0xFF

const ActorInit En_Ossan_InitVars = {
    ACTOR_EN_OSSAN,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnOssan),
    (ActorFunc)EnOssan_Init,
    (ActorFunc)EnOssan_Destroy,
    (ActorFunc)EnOssan_Update,
    NULL,
};

// Unused collider
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
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 30, 80, 0, { 0, 0, 0 } },
};

// Rupees to pay back to Happy Mask Shop
static s16 sMaskPaymentPrice[] = { 10, 30, 20, 50 };

// item yaw offsets
static s16 sItemShelfRot[] = { 0xEAAC, 0xEAAC, 0xEAAC, 0xEAAC, 0x1554, 0x1554, 0x1554, 0x1554 };

// unused values?
static s16 D_80AC8904[] = { 0x001E, 0x001F, 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025 };

static char* sShopkeeperPrintName[] = {
    "コキリの店  ", // "Kokiri Shop"
    "薬屋        ", // "Potion Shop"
    "夜の店      ", // "Night Shop"
    "路地裏の店  ", // "Back Alley Shop"
    "盾の店      ", // "Shield Shop"
    "大人の店    ", // "Adult Shop"
    "タロンの店  ", // "Talon Shop"
    "ゾーラの店  ", // "Zora Shop"
    "ゴロン夜の店", // "Goron Night Shop"
    "インゴーの店", // "Ingo Store"
    "お面屋      ", // "Mask Shop"
};

typedef struct {
    /* 0x00 */ s16 objId;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
} ShopkeeperObjInfo;

static s16 sShopkeeperObjectIds[][3] = {
    { OBJECT_KM1, OBJECT_MASTERKOKIRIHEAD, OBJECT_MASTERKOKIRI },
    { OBJECT_DS2, OBJECT_ID_MAX, OBJECT_ID_MAX },
    { OBJECT_RS, OBJECT_ID_MAX, OBJECT_ID_MAX },
    { OBJECT_DS2, OBJECT_ID_MAX, OBJECT_ID_MAX },
    { OBJECT_OSSAN, OBJECT_ID_MAX, OBJECT_ID_MAX },
    { OBJECT_OSSAN, OBJECT_ID_MAX, OBJECT_ID_MAX },
    { OBJECT_OSSAN, OBJECT_ID_MAX, OBJECT_ID_MAX },
    { OBJECT_ZO, OBJECT_ID_MAX, OBJECT_MASTERZOORA },
    { OBJECT_OF1D_MAP, OBJECT_ID_MAX, OBJECT_MASTERGOLON },
    { OBJECT_OSSAN, OBJECT_ID_MAX, OBJECT_ID_MAX },
    { OBJECT_OS, OBJECT_ID_MAX, OBJECT_ID_MAX },
};

static EnOssanTalkOwnerFunc sShopkeeperTalkOwner[] = {
    EnOssan_TalkKokiriShopkeeper,       EnOssan_TalkKakarikoPotionShopkeeper, EnOssan_TalkBombchuShopkeeper,
    EnOssan_TalkMarketPotionShopkeeper, EnOssan_TalkBazaarShopkeeper,         EnOssan_TalkDefaultShopkeeper,
    EnOssan_TalkDefaultShopkeeper,      EnOssan_TalkZoraShopkeeper,           EnOssan_TalkGoronShopkeeper,
    EnOssan_TalkDefaultShopkeeper,      EnOssan_TalkHappyMaskShopkeeper,
};

static f32 sShopkeeperScale[] = {
    0.01f, 0.011f, 0.0105f, 0.011f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f, 0.01f,
};

typedef struct {
    /* 0x00 */ s16 shopItemIndex;
    /* 0x02 */ s16 xOffset;
    /* 0x04 */ s16 yOffset;
    /* 0x06 */ s16 zOffset;
} ShopItem; // size 0x08

ShopItem sShopkeeperStores[][8] = {
    { { SI_DEKU_SHIELD, 50, 52, -20 },
      { SI_DEKU_NUTS_5, 50, 76, -20 },
      { SI_DEKU_NUTS_10, 80, 52, -3 },
      { SI_DEKU_STICK, 80, 76, -3 },
      { SI_DEKU_SEEDS_30, -50, 52, -20 },
      { SI_ARROWS_10, -50, 76, -20 },
      { SI_ARROWS_30, -80, 52, -3 },
      { SI_HEART, -80, 76, -3 } },

    { { SI_GREEN_POTION, 50, 52, -20 },
      { SI_BLUE_FIRE, 50, 76, -20 },
      { SI_RED_POTION_R30, 80, 52, -3 },
      { SI_FAIRY, 80, 76, -3 },
      { SI_DEKU_NUTS_5, -50, 52, -20 },
      { SI_BUGS, -50, 76, -20 },
      { SI_POE, -80, 52, -3 },
      { SI_FISH, -80, 76, -3 } },

    { { SI_BOMBCHU_10_2, 50, 52, -20 },
      { SI_BOMBCHU_10_4, 50, 76, -20 },
      { SI_BOMBCHU_10_3, 80, 52, -3 },
      { SI_BOMBCHU_10_1, 80, 76, -3 },
      { SI_BOMBCHU_20_3, -50, 52, -20 },
      { SI_BOMBCHU_20_1, -50, 76, -20 },
      { SI_BOMBCHU_20_4, -80, 52, -3 },
      { SI_BOMBCHU_20_2, -80, 76, -3 } },

    { { SI_GREEN_POTION, 50, 52, -20 },
      { SI_BLUE_FIRE, 50, 76, -20 },
      { SI_RED_POTION_R30, 80, 52, -3 },
      { SI_FAIRY, 80, 76, -3 },
      { SI_DEKU_NUTS_5, -50, 52, -20 },
      { SI_BUGS, -50, 76, -20 },
      { SI_POE, -80, 52, -3 },
      { SI_FISH, -80, 76, -3 } },

    { { SI_HYLIAN_SHIELD, 50, 52, -20 },
      { SI_BOMBS_5_R35, 50, 76, -20 },
      { SI_DEKU_NUTS_5, 80, 52, -3 },
      { SI_HEART, 80, 76, -3 },
      { SI_ARROWS_10, -50, 52, -20 },
      { SI_ARROWS_50, -50, 76, -20 },
      { SI_DEKU_STICK, -80, 52, -3 },
      { SI_ARROWS_30, -80, 76, -3 } },

    { { SI_HYLIAN_SHIELD, 50, 52, -20 },
      { SI_BOMBS_5_R25, 50, 76, -20 },
      { SI_DEKU_NUTS_5, 80, 52, -3 },
      { SI_HEART, 80, 76, -3 },
      { SI_ARROWS_10, -50, 52, -20 },
      { SI_ARROWS_50, -50, 76, -20 },
      { SI_DEKU_STICK, -80, 52, -3 },
      { SI_ARROWS_30, -80, 76, -3 } },

    { { SI_MILK_BOTTLE, 50, 52, -20 },
      { SI_DEKU_NUTS_5, 50, 76, -20 },
      { SI_DEKU_NUTS_10, 80, 52, -3 },
      { SI_HEART, 80, 76, -3 },
      { SI_WEIRD_EGG, -50, 52, -20 },
      { SI_DEKU_STICK, -50, 76, -20 },
      { SI_HEART, -80, 52, -3 },
      { SI_HEART, -80, 76, -3 } },

    { { SI_ZORA_TUNIC, 50, 52, -20 },
      { SI_ARROWS_10, 50, 76, -20 },
      { SI_HEART, 80, 52, -3 },
      { SI_ARROWS_30, 80, 76, -3 },
      { SI_DEKU_NUTS_5, -50, 52, -20 },
      { SI_ARROWS_50, -50, 76, -20 },
      { SI_FISH, -80, 52, -3 },
      { SI_RED_POTION_R50, -80, 76, -3 } },

    { { SI_BOMBS_5_R25, 50, 52, -20 },
      { SI_BOMBS_10, 50, 76, -20 },
      { SI_BOMBS_20, 80, 52, -3 },
      { SI_BOMBS_30, 80, 76, -3 },
      { SI_GORON_TUNIC, -50, 52, -20 },
      { SI_HEART, -50, 76, -20 },
      { SI_RED_POTION_R40, -80, 52, -3 },
      { SI_HEART, -80, 76, -3 } },

    { { SI_19, 50, 52, -20 },
      { SI_19, 50, 76, -20 },
      { SI_19, 80, 52, -3 },
      { SI_19, 80, 76, -3 },
      { SI_20, -50, 52, -20 },
      { SI_20, -50, 76, -20 },
      { SI_20, -80, 52, -3 },
      { SI_20, -80, 76, -3 } },

    { { SI_GERUDO_MASK, 50, 52, -20 },
      { SI_ZORA_MASK, 50, 76, -20 },
      { SI_MASK_OF_TRUTH, 80, 52, -3 },
      { SI_GORON_MASK, 80, 76, -3 },
      { SI_SKULL_MASK, -50, 52, -20 },
      { SI_KEATON_MASK, -50, 76, -20 },
      { SI_BUNNY_HOOD, -80, 52, -3 },
      { SI_SPOOKY_MASK, -80, 76, -3 } },
};
static EnOssanGetGirlAParamsFunc sShopItemReplaceFunc[] = {
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_SpookyMask,
    ShopItemDisp_SkullMask, ShopItemDisp_BunnyHood,  ShopItemDisp_Default, ShopItemDisp_ZoraMask,
    ShopItemDisp_GoronMask, ShopItemDisp_GerudoMask, ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,    ShopItemDisp_Default, ShopItemDisp_Default,
    ShopItemDisp_Default,   ShopItemDisp_Default,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 500, ICHAIN_STOP),
};

// When selecting an item to buy, self is the position the item moves to
static Vec3f sSelectedItemPosition[] = { { 17.0f, 58.0f, 30.0f }, { -17.0f, 58.0f, 30.0f } };

static EnOssanInitFunc sInitFuncs[] = {
    EnOssan_InitKokiriShopkeeper, EnOssan_InitPotionShopkeeper,    EnOssan_InitBombchuShopkeeper,
    EnOssan_InitPotionShopkeeper, EnOssan_InitBazaarShopkeeper,    EnOssan_InitBazaarShopkeeper,
    EnOssan_InitBazaarShopkeeper, EnOssan_InitZoraShopkeeper,      EnOssan_InitGoronShopkeeper,
    EnOssan_InitBazaarShopkeeper, EnOssan_InitHappyMaskShopkeeper,
};

static Vec3f sShopkeeperPositionOffsets[] = {
    { 0.0f, 0.0f, 33.0f }, { 0.0f, 0.0f, 31.0f }, { 0.0f, 0.0f, 31.0f }, { 0.0f, 0.0f, 31.0f },
    { 0.0f, 0.0f, 0.0f },  { 0.0f, 0.0f, 0.0f },  { 0.0f, 0.0f, 0.0f },  { 0.0f, 0.0f, 36.0f },
    { 0.0f, 0.0f, 15.0f }, { 0.0f, 0.0f, 0.0f },  { 0.0f, 0.0f, 26.0f },
};

static EnOssanStateFunc sStateFunc[] = {
    EnOssan_State_Idle,
    EnOssan_State_StartConversation,
    EnOssan_State_FacingShopkeeper,
    EnOssan_State_TalkingToShopkeeper,
    EnOssan_State_LookToLeftShelf,
    EnOssan_State_LookToRightShelf,
    EnOssan_State_BrowseLeftShelf,
    EnOssan_State_BrowseRightShelf,
    EnOssan_State_LookFromShelfToShopkeeper,
    EnOssan_State_ItemSelected,
    EnOssan_State_SelectMilkBottle,
    EnOssan_State_SelectWeirdEgg,
    EnOssan_State_SelectUnimplementedItem,
    EnOssan_State_SelectBombs,
    EnOssan_State_CantGetItem,
    EnOssan_State_GiveItemWithFanfare,
    EnOssan_State_ItemPurchased,
    EnOssan_State_ContinueShoppingPrompt,
    EnOssan_State_GiveLonLonMilk,
    EnOssan_State_DisplayOnlyBombDialog,
    EnOssan_State_WaitForDisplayOnlyBombDialog,
    EnOssan_State_21,
    EnOssan_State_22,
    EnOssan_State_QuickBuyDialog,
    EnOssan_State_SelectMaskItem,
    EnOssan_State_LendMaskOfTruth,
    EnOssan_State_GiveDiscountDialog,
};

void EnOssan_SetupAction(EnOssan* self, EnOssanActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

s16 ShopItemDisp_Default(s16 v) {
    return v;
}

s16 ShopItemDisp_SpookyMask(s16 v) {
    // Sold Skull Mask
    if (gSaveContext.itemGetInf[3] & 0x200) {
        return v;
    }
    return -1;
}

s16 ShopItemDisp_SkullMask(s16 v) {
    // Sold Keaton Mask
    if (gSaveContext.itemGetInf[3] & 0x100) {
        return v;
    }
    return -1;
}

s16 ShopItemDisp_BunnyHood(s16 v) {
    // Sold Spooky Mask
    if (gSaveContext.itemGetInf[3] & 0x400) {
        return v;
    }
    return -1;
}

s16 ShopItemDisp_ZoraMask(s16 v) {
    // Obtained Mask of Truth
    if (gSaveContext.itemGetInf[3] & 0x8000) {
        return v;
    }
    return -1;
}

s16 ShopItemDisp_GoronMask(s16 v) {
    // Obtained Mask of Truth
    if (gSaveContext.itemGetInf[3] & 0x8000) {
        return v;
    }
    return -1;
}

s16 ShopItemDisp_GerudoMask(s16 v) {
    // Obtained Mask of Truth
    if (gSaveContext.itemGetInf[3] & 0x8000) {
        return v;
    }
    return -1;
}

void EnOssan_SpawnItemsOnShelves(EnOssan* self, GlobalContext* globalCtx, ShopItem* shopItems) {
    EnTana* shelves;
    s16 itemParams;
    s32 i;

    for (i = 0; i < 8; i++, shopItems++) {
        if (shopItems->shopItemIndex < 0) {
            self->shelfSlots[i] = NULL;
        } else {
            itemParams = sShopItemReplaceFunc[shopItems->shopItemIndex](shopItems->shopItemIndex);

            if (itemParams < 0) {
                self->shelfSlots[i] = NULL;
            } else {
                shelves = self->shelves;
                self->shelfSlots[i] = (EnGirlA*)Actor_Spawn(
                    &globalCtx->actorCtx, globalCtx, ACTOR_EN_GIRLA, shelves->actor.world.pos.x + shopItems->xOffset,
                    shelves->actor.world.pos.y + shopItems->yOffset, shelves->actor.world.pos.z + shopItems->zOffset,
                    shelves->actor.shape.rot.x, shelves->actor.shape.rot.y + sItemShelfRot[i],
                    shelves->actor.shape.rot.z, itemParams);
            }
        }
    }
}

void EnOssan_UpdateShopOfferings(EnOssan* self, GlobalContext* globalCtx) {
    s32 i;
    ShopItem* storeItems;
    ShopItem* shopItem;

    if (self->actor.params == OSSAN_TYPE_MASK) {
        storeItems = sShopkeeperStores[self->actor.params];
        if (1) {}
        for (i = 0; i < 8; i++) {
            shopItem = &storeItems[i];
            if (shopItem->shopItemIndex >= 0 && self->shelfSlots[i] == NULL) {
                s16 params = sShopItemReplaceFunc[shopItem->shopItemIndex](shopItem->shopItemIndex);
                if (params >= 0) {
                    self->shelfSlots[i] = (EnGirlA*)Actor_Spawn(
                        &globalCtx->actorCtx, globalCtx, ACTOR_EN_GIRLA,
                        self->shelves->actor.world.pos.x + shopItem->xOffset,
                        self->shelves->actor.world.pos.y + shopItem->yOffset,
                        self->shelves->actor.world.pos.z + shopItem->zOffset, self->shelves->actor.shape.rot.x,
                        self->shelves->actor.shape.rot.y + sItemShelfRot[i], self->shelves->actor.shape.rot.z, params);
                }
            }
        }
    }
}

void EnOssan_TalkDefaultShopkeeper(GlobalContext* globalCtx) {
    func_8010B720(globalCtx, 0x9E);
}

void EnOssan_TalkKakarikoPotionShopkeeper(GlobalContext* globalCtx) {
    if (globalCtx->curSpawn == 0) {
        func_8010B720(globalCtx, 0x5046);
    } else {
        func_8010B720(globalCtx, 0x504E);
    }
}

void EnOssan_TalkMarketPotionShopkeeper(GlobalContext* globalCtx) {
    func_8010B720(globalCtx, 0x504E);
}

void EnOssan_TalkKokiriShopkeeper(GlobalContext* globalCtx) {
    func_8010B720(globalCtx, 0x10BA);
}

void EnOssan_TalkBazaarShopkeeper(GlobalContext* globalCtx) {
    if (globalCtx->curSpawn == 0) {
        func_8010B720(globalCtx, 0x9D);
    } else {
        func_8010B720(globalCtx, 0x9C);
    }
}

void EnOssan_TalkBombchuShopkeeper(GlobalContext* globalCtx) {
    func_8010B720(globalCtx, 0x7076);
}

void EnOssan_TalkZoraShopkeeper(GlobalContext* globalCtx) {
    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        func_8010B720(globalCtx, 0x403A);
    } else {
        func_8010B720(globalCtx, 0x403B);
    }
}

// Goron City, Goron
void EnOssan_TalkGoronShopkeeper(GlobalContext* globalCtx) {
    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        if (gSaveContext.eventChkInf[2] & 0x20) {
            func_8010B720(globalCtx, 0x3028);
        } else if (CUR_UPG_VALUE(UPG_STRENGTH) != 0) {
            func_8010B720(globalCtx, 0x302D);
        } else {
            func_8010B720(globalCtx, 0x300F);
        }
    } else if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE)) {
        func_8010B720(globalCtx, 0x3057);
    } else {
        func_8010B720(globalCtx, 0x305B);
    }
}

// Happy Mask Shop
void EnOssan_TalkHappyMaskShopkeeper(GlobalContext* globalCtx) {
    if ((gSaveContext.itemGetInf[3] & 0x100)       // Sold Keaton Mask
        && (gSaveContext.itemGetInf[3] & 0x200)    // Sold Skull Mask
        && (gSaveContext.itemGetInf[3] & 0x400)    // Sold Spooky Mask
        && (gSaveContext.itemGetInf[3] & 0x800)) { // Sold Bunny Hood
        func_8010B720(globalCtx, 0x70AE);
    } else {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 1:
                func_8010B720(globalCtx, 0x70A4);
                break;
            case 0:
                func_8010B720(globalCtx, 0x70A3);
                break;
        }
    }
}

void EnOssan_UpdateCameraDirection(EnOssan* self, GlobalContext* globalCtx, f32 cameraFaceAngle) {
    self->cameraFaceAngle = cameraFaceAngle;
    Camera_SetCameraData(GET_ACTIVE_CAM(globalCtx), 0xC, NULL, NULL, cameraFaceAngle, 0, 0);
}

s32 EnOssan_TryGetObjBankIndexes(EnOssan* self, GlobalContext* globalCtx, s16* objectIds) {
    if (objectIds[1] != OBJECT_ID_MAX) {
        self->objBankIndex2 = Object_GetIndex(&globalCtx->objectCtx, objectIds[1]);
        if (self->objBankIndex2 < 0) {
            return false;
        }
    } else {
        self->objBankIndex2 = -1;
    }
    if (objectIds[2] != OBJECT_ID_MAX) {
        self->objBankIndex3 = Object_GetIndex(&globalCtx->objectCtx, objectIds[2]);
        if (self->objBankIndex3 < 0) {
            return false;
        }
    } else {
        self->objBankIndex3 = -1;
    }
    return true;
}

void EnOssan_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnOssan* self = THIS;
    s32 pad;
    s16* objectIds;

    if (self->actor.params == OSSAN_TYPE_TALON && (LINK_AGE_IN_YEARS != YEARS_CHILD)) {
        self->actor.params = OSSAN_TYPE_INGO;
    }

    //! @bug This check will always evaluate to false, it should be || not &&
    if (self->actor.params > OSSAN_TYPE_MASK && self->actor.params < OSSAN_TYPE_KOKIRI) {
        Actor_Kill(&self->actor);
        osSyncPrintf(VT_COL(RED, WHITE));
        osSyncPrintf("引数がおかしいよ(arg_data=%d)！！\n", self->actor.params);
        osSyncPrintf(VT_RST);
        ASSERT(0, "0", "../z_en_oB1.c", 1246);
        return;
    }

    // If you've given Zelda's Letter to the Kakariko Guard
    if (self->actor.params == OSSAN_TYPE_MASK && !(gSaveContext.infTable[7] & 0x40)) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->actor.params == OSSAN_TYPE_KAKARIKO_POTION && (LINK_AGE_IN_YEARS == YEARS_CHILD)) {
        Actor_Kill(&self->actor);
        return;
    }

    // Completed Dodongo's Cavern
    if (self->actor.params == OSSAN_TYPE_BOMBCHUS && !(gSaveContext.eventChkInf[2] & 0x20)) {
        Actor_Kill(&self->actor);
        return;
    }

    objectIds = sShopkeeperObjectIds[self->actor.params];
    self->objBankIndex1 = Object_GetIndex(&globalCtx->objectCtx, objectIds[0]);

    if (self->objBankIndex1 < 0) {
        Actor_Kill(&self->actor);
        osSyncPrintf(VT_COL(RED, WHITE));
        osSyncPrintf("バンクが無いよ！！(%s)\n", sShopkeeperPrintName[self->actor.params]);
        osSyncPrintf(VT_RST);
        ASSERT(0, "0", "../z_en_oB1.c", 1284);
        return;
    }

    if (EnOssan_TryGetObjBankIndexes(self, globalCtx, objectIds) == 0) {
        Actor_Kill(&self->actor);
        osSyncPrintf(VT_COL(RED, WHITE));
        osSyncPrintf("予備バンクが無いよ！！(%s)\n", sShopkeeperPrintName[self->actor.params]);
        osSyncPrintf(VT_RST);
        ASSERT(0, "0", "../z_en_oB1.c", 1295);
        return;
    }

    Actor_ProcessInitChain(&self->actor, sInitChain);
    EnOssan_SetupAction(self, EnOssan_InitActionFunc);
}

void EnOssan_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnOssan* self = THIS;
    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnOssan_UpdateCursorPos(GlobalContext* globalCtx, EnOssan* self) {
    s16 x;
    s16 y;

    func_8002F374(globalCtx, &self->shelfSlots[self->cursorIndex]->actor, &x, &y);
    self->cursorX = x;
    self->cursorY = y;
}

void EnOssan_EndInteraction(GlobalContext* globalCtx, EnOssan* self) {
    Player* player = GET_PLAYER(globalCtx);

    // "End of conversation!"
    osSyncPrintf(VT_FGCOL(YELLOW) "%s[%d]:★★★ 会話終了！！ ★★★" VT_RST "\n", "../z_en_oB1.c", 1337);
    YREG(31) = 0;
    func_8002F194(&self->actor, globalCtx);
    globalCtx->msgCtx.msgMode = 0x36;
    globalCtx->msgCtx.unk_E3E7 = 4;
    player->stateFlags2 &= ~0x20000000;
    func_800BC490(globalCtx, 1);
    Interface_ChangeAlpha(50);
    self->drawCursor = 0;
    self->stickLeftPrompt.isEnabled = false;
    self->stickRightPrompt.isEnabled = false;
    EnOssan_UpdateCameraDirection(self, globalCtx, 0.0f);
    self->actor.textId = EnOssan_SetupHelloDialog(self);
    self->stateFlag = OSSAN_STATE_IDLE;
}

s32 EnOssan_TestEndInteraction(EnOssan* self, GlobalContext* globalCtx, Input* controller1) {
    if (CHECK_BTN_ALL(controller1->press.button, BTN_B)) {
        EnOssan_EndInteraction(globalCtx, self);
        return true;
    } else {
        return false;
    }
}

s32 EnOssan_TestCancelOption(EnOssan* self, GlobalContext* globalCtx, Input* controller1) {
    if (CHECK_BTN_ALL(controller1->press.button, BTN_B)) {
        self->stateFlag = self->tempStateFlag;
        func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
        return true;
    } else {
        return false;
    }
}

void EnOssan_SetStateStartShopping(GlobalContext* globalCtx, EnOssan* self, u8 skipHelloState) {
    YREG(31) = 1;
    self->headRot = self->headTargetRot = 0;
    Interface_SetDoAction(globalCtx, 0x10);
    EnOssan_UpdateCameraDirection(self, globalCtx, 0);

    if (!skipHelloState) {
        self->stateFlag = OSSAN_STATE_START_CONVERSATION;
    } else {
        EnOssan_StartShopping(globalCtx, self);
    }
}

void EnOssan_StartShopping(GlobalContext* globalCtx, EnOssan* self) {
    self->stateFlag = OSSAN_STATE_FACING_SHOPKEEPER;

    if (self->actor.params == OSSAN_TYPE_MASK) {
        // if all masks have been sold, give the option to ask about the mask of truth
        if ((gSaveContext.itemGetInf[3] & 0x100) && (gSaveContext.itemGetInf[3] & 0x200) &&
            (gSaveContext.itemGetInf[3] & 0x400) && (gSaveContext.itemGetInf[3] & 0x800)) {
            func_8010B720(globalCtx, 0x70AD);
        } else {
            func_8010B720(globalCtx, 0x70A2);
        }
    } else {
        func_8010B720(globalCtx, 0x83);
    }

    Interface_SetDoAction(globalCtx, 6);
    self->stickRightPrompt.isEnabled = true;
    self->stickLeftPrompt.isEnabled = true;
    EnOssan_UpdateCameraDirection(self, globalCtx, 0.0f);
}

void EnOssan_ChooseTalkToOwner(GlobalContext* globalCtx, EnOssan* self) {
    self->stateFlag = OSSAN_STATE_TALKING_TO_SHOPKEEPER;
    sShopkeeperTalkOwner[self->actor.params](globalCtx);
    Interface_SetDoAction(globalCtx, 6);
    self->stickLeftPrompt.isEnabled = false;
    self->stickRightPrompt.isEnabled = false;
}

void EnOssan_SetLookToShopkeeperFromShelf(GlobalContext* globalCtx, EnOssan* self) {
    func_80078884(NA_SE_SY_CURSOR);
    self->drawCursor = 0;
    self->stateFlag = OSSAN_STATE_LOOK_SHOPKEEPER;
}

void EnOssan_State_Idle(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    self->headTargetRot = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;

    if (func_8002F194(&self->actor, globalCtx)) {
        // "Start conversation!!"
        osSyncPrintf(VT_FGCOL(YELLOW) "★★★ 会話開始！！ ★★★" VT_RST "\n");
        player->stateFlags2 |= 0x20000000;
        func_800BC590(globalCtx);
        EnOssan_SetStateStartShopping(globalCtx, self, false);
    } else if (self->actor.xzDistToPlayer < 100.0f) {
        func_8002F2CC(&self->actor, globalCtx, 100);
    }
}

void EnOssan_UpdateJoystickInputState(GlobalContext* globalCtx, EnOssan* self) {
    Input* controller1 = &globalCtx->state.input[0];
    s8 stickX = controller1->rel.stick_x;
    s8 stickY = controller1->rel.stick_y;

    self->moveHorizontal = self->moveVertical = false;

    if (self->stickAccumX == 0) {
        if (stickX > 30 || stickX < -30) {
            self->stickAccumX = stickX;
            self->moveHorizontal = true;
        }
    } else if (stickX <= 30 && stickX >= -30) {
        self->stickAccumX = 0;
    } else if (self->stickAccumX * stickX < 0) { // Stick has swapped directions
        self->stickAccumX = stickX;
        self->moveHorizontal = true;
    } else {
        self->stickAccumX += stickX;

        if (self->stickAccumX > 2000) {
            self->stickAccumX = 2000;
        } else if (self->stickAccumX < -2000) {
            self->stickAccumX = -2000;
        }
    }

    if (self->stickAccumY == 0) {
        if (stickY > 30 || stickY < -30) {
            self->stickAccumY = stickY;
            self->moveVertical = true;
        }
    } else if (stickY <= 30 && stickY >= -30) {
        self->stickAccumY = 0;
    } else if (self->stickAccumY * stickY < 0) { // Stick has swapped directions
        self->stickAccumY = stickY;
        self->moveVertical = true;
    } else {
        self->stickAccumY += stickY;

        if (self->stickAccumY > 2000) {
            self->stickAccumY = 2000;
        } else if (self->stickAccumY < -2000) {
            self->stickAccumY = -2000;
        }
    }
}

u8 EnOssan_SetCursorIndexFromNeutral(EnOssan* self, u8 shelfOffset) {
    u8 i;

    // if cursor is on the top shelf
    if (self->cursorIndex & 1) {
        // scan top shelf for non-null item
        for (i = shelfOffset + 1; i < shelfOffset + 4; i += 2) {
            if (self->shelfSlots[i] != NULL) {
                return i;
            }
        }
        // scan bottom shelf for non-null item
        for (i = shelfOffset; i < shelfOffset + 4; i += 2) {
            if (self->shelfSlots[i] != NULL) {
                return i;
            }
        }
    } else {
        // scan bottom shelf for non-null item
        for (i = shelfOffset; i < shelfOffset + 4; i += 2) {
            if (self->shelfSlots[i] != NULL) {
                return i;
            }
        }
        // scan top shelf for non-null item
        for (i = shelfOffset + 1; i < shelfOffset + 4; i += 2) {
            if (self->shelfSlots[i] != NULL) {
                return i;
            }
        }
    }
    return CURSOR_INVALID;
}

u8 EnOssan_CursorRight(EnOssan* self, u8 cursorIndex, u8 shelfSlotMin) {
    u8 c = shelfSlotMin + 4;

    while (cursorIndex >= shelfSlotMin && cursorIndex < c) {
        cursorIndex -= 2;
        if (cursorIndex >= shelfSlotMin && cursorIndex < c) {
            if (self->shelfSlots[cursorIndex] != NULL) {
                return cursorIndex;
            }
        }
    }
    return CURSOR_INVALID;
}

u8 EnOssan_CursorLeft(EnOssan* self, u8 cursorIndex, u8 shelfSlotMax) {

    while (cursorIndex < shelfSlotMax) {
        cursorIndex += 2;
        if ((cursorIndex < shelfSlotMax) && self->shelfSlots[cursorIndex] != NULL) {
            return cursorIndex;
        }
    }
    return CURSOR_INVALID;
}

// pay salesman back
void EnOssan_TryPaybackMask(EnOssan* self, GlobalContext* globalCtx) {
    s16 price = sMaskPaymentPrice[self->happyMaskShopState];

    if (gSaveContext.rupees < price) {
        func_8010B720(globalCtx, 0x70A8);
        self->happyMaskShopkeeperEyeIdx = 1;
        self->happyMaskShopState = OSSAN_HAPPY_STATE_ANGRY;
    } else {
        Rupees_ChangeBy(-price);

        if (self->happyMaskShopState == OSSAN_HAPPY_STATE_REQUEST_PAYMENT_BUNNY_HOOD) {
            gSaveContext.eventChkInf[8] |= 0x8000;
            func_8010B720(globalCtx, 0x70A9);
            self->happyMaskShopState = OSSAN_HAPPY_STATE_ALL_MASKS_SOLD;
            return;
        }

        if (self->happyMaskShopState == OSSAN_HAPPY_STATE_REQUEST_PAYMENT_KEATON_MASK) {
            gSaveContext.eventChkInf[8] |= 0x1000;
        } else if (self->happyMaskShopState == OSSAN_HAPPY_STATE_REQUEST_PAYMENT_SPOOKY_MASK) {
            gSaveContext.eventChkInf[8] |= 0x4000;
        } else if (self->happyMaskShopState == OSSAN_HAPPY_STATE_REQUEST_PAYMENT_SKULL_MASK) {
            gSaveContext.eventChkInf[8] |= 0x2000;
        }

        func_8010B720(globalCtx, 0x70A7);
        self->happyMaskShopState = OSSAN_HAPPY_STATE_NONE;
    }
    self->stateFlag = OSSAN_STATE_START_CONVERSATION;
}

void EnOssan_State_StartConversation(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    u8 dialogState = func_8010BDBC(&globalCtx->msgCtx);

    if (self->actor.params == OSSAN_TYPE_MASK && dialogState == 4) {
        if (!EnOssan_TestEndInteraction(self, globalCtx, &globalCtx->state.input[0]) && func_80106BC8(globalCtx)) {
            switch (globalCtx->msgCtx.choiceIndex) {
                case 0:
                    EnOssan_StartShopping(globalCtx, self);
                    break;
                case 1:
                    EnOssan_EndInteraction(globalCtx, self);
                    break;
            }
        }
    } else if (dialogState == 5 && func_80106BC8(globalCtx)) {
        func_80078884(NA_SE_SY_MESSAGE_PASS);

        switch (self->happyMaskShopState) {
            case OSSAN_HAPPY_STATE_ALL_MASKS_SOLD:
                func_8010B720(globalCtx, 0x70AA);
                self->stateFlag = OSSAN_STATE_LEND_MASK_OF_TRUTH;
                return;
            case OSSAN_HAPPY_STATE_BORROWED_FIRST_MASK:
                EnOssan_EndInteraction(globalCtx, self);
                return;
            case OSSAN_HAPPY_STATE_REQUEST_PAYMENT_KEATON_MASK:
            case OSSAN_HAPPY_STATE_REQUEST_PAYMENT_SPOOKY_MASK:
            case OSSAN_HAPPY_STATE_REQUEST_PAYMENT_SKULL_MASK:
            case OSSAN_HAPPY_STATE_REQUEST_PAYMENT_BUNNY_HOOD:
                EnOssan_TryPaybackMask(self, globalCtx);
                return;
            case OSSAN_HAPPY_STATE_ANGRY:
                globalCtx->nextEntranceIndex = 0x1D1;
                globalCtx->sceneLoadFlag = 0x14;
                globalCtx->fadeTransition = 0x2E;
                return;
        }

        if (!EnOssan_TestEndInteraction(self, globalCtx, &globalCtx->state.input[0])) {
            // "Shop around by moving the stick left and right"
            osSyncPrintf("「スティック左右で品物みてくれ！」\n");
            EnOssan_StartShopping(globalCtx, self);
        }
    }

    if (1) {}
}

s32 EnOssan_FacingShopkeeperDialogResult(EnOssan* self, GlobalContext* globalCtx) {
    switch (globalCtx->msgCtx.choiceIndex) {
        case 0:
            EnOssan_ChooseTalkToOwner(globalCtx, self);
            return true;
        case 1:
            EnOssan_EndInteraction(globalCtx, self);
            return true;
        default:
            return false;
    }
}

void EnOssan_State_FacingShopkeeper(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    u8 nextIndex;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) &&
        !EnOssan_TestEndInteraction(self, globalCtx, &globalCtx->state.input[0])) {
        if (func_80106BC8(globalCtx) && EnOssan_FacingShopkeeperDialogResult(self, globalCtx)) {
            func_80078884(NA_SE_SY_DECIDE);
            return;
        }
        // Stick Left
        if (self->stickAccumX < 0) {
            nextIndex = EnOssan_SetCursorIndexFromNeutral(self, 4);
            if (nextIndex != CURSOR_INVALID) {
                self->cursorIndex = nextIndex;
                self->stateFlag = OSSAN_STATE_LOOK_SHELF_LEFT;
                Interface_SetDoAction(globalCtx, 6);
                self->stickLeftPrompt.isEnabled = false;
                func_80078884(NA_SE_SY_CURSOR);
            }
        } else if (self->stickAccumX > 0) {
            nextIndex = EnOssan_SetCursorIndexFromNeutral(self, 0);
            if (nextIndex != CURSOR_INVALID) {
                self->cursorIndex = nextIndex;
                self->stateFlag = OSSAN_STATE_LOOK_SHELF_RIGHT;
                Interface_SetDoAction(globalCtx, 6);
                self->stickRightPrompt.isEnabled = false;
                func_80078884(NA_SE_SY_CURSOR);
            }
        }
    }
}

void EnOssan_State_TalkingToShopkeeper(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        EnOssan_StartShopping(globalCtx, self);
    }
}

void EnOssan_State_LookToLeftShelf(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    Math_ApproachF(&self->cameraFaceAngle, 30.0f, 0.5f, 10.0f);

    if (self->cameraFaceAngle > 29.5f) {
        EnOssan_UpdateCameraDirection(self, globalCtx, 30.0f);
    }

    EnOssan_UpdateCameraDirection(self, globalCtx, self->cameraFaceAngle);

    if (self->cameraFaceAngle >= 30.0f) {
        EnOssan_UpdateCameraDirection(self, globalCtx, 30.0f);
        EnOssan_UpdateCursorPos(globalCtx, self);
        self->stateFlag = OSSAN_STATE_BROWSE_LEFT_SHELF;
        func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
    } else {
        self->stickAccumX = 0;
    }
}

void EnOssan_State_LookToRightShelf(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    Math_ApproachF(&self->cameraFaceAngle, -30.0f, 0.5f, 10.0f);

    if (self->cameraFaceAngle < -29.5f) {
        EnOssan_UpdateCameraDirection(self, globalCtx, -30.0f);
    }

    EnOssan_UpdateCameraDirection(self, globalCtx, self->cameraFaceAngle);

    if (self->cameraFaceAngle <= -30.0f) {
        EnOssan_UpdateCameraDirection(self, globalCtx, -30.0f);
        EnOssan_UpdateCursorPos(globalCtx, self);
        self->stateFlag = OSSAN_STATE_BROWSE_RIGHT_SHELF;
        func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
    } else {
        self->stickAccumX = 0;
    }
}

void EnOssan_CursorUpDown(EnOssan* self) {
    u8 curTemp = self->cursorIndex;
    u8 curScanTemp;

    if (self->stickAccumY < 0) {
        curTemp &= 0xFE;
        if (self->shelfSlots[curTemp] != NULL) {
            self->cursorIndex = curTemp;
            return;
        }
        // cursorIndex on right shelf
        if (curTemp < 4) {
            curScanTemp = curTemp + 2;
            if (curScanTemp >= 4) {
                curScanTemp = 0;
            }
            while (curScanTemp != curTemp) {
                if (self->shelfSlots[curScanTemp] != NULL) {
                    self->cursorIndex = curScanTemp;
                    return;
                }
                curScanTemp += 2;
                if (curScanTemp >= 4) {
                    curScanTemp = 0;
                }
            }
        } else {
            // cursorIndex on left shelf
            curScanTemp = curTemp + 2;
            if (curScanTemp >= 8) {
                curScanTemp = 4;
            }
            while (curScanTemp != curTemp) {
                if (self->shelfSlots[curScanTemp] != NULL) {
                    self->cursorIndex = curScanTemp;
                    return;
                }
                curScanTemp += 2;
                if (curScanTemp >= 8) {
                    curScanTemp = 4;
                }
            }
        }
    } else if (self->stickAccumY > 0) {
        curTemp |= 1;
        if (self->shelfSlots[curTemp] != NULL) {
            self->cursorIndex = curTemp;
            return;
        }
        // cursorIndex on right shelf
        if (curTemp < 4) {
            curScanTemp = curTemp + 2;
            if (curScanTemp >= 4) {
                curScanTemp = 1;
            }
            while (curScanTemp != curTemp) {
                if (self->shelfSlots[curScanTemp] != NULL) {
                    self->cursorIndex = curScanTemp;
                    return;
                }
                curScanTemp += 2;
                if (curScanTemp >= 4) {
                    curScanTemp = 1;
                }
            }
        } else {
            // cursorIndex on left shelf
            curScanTemp = curTemp + 2;
            if (curScanTemp >= 8) {
                curScanTemp = 5;
            }
            while (curScanTemp != curTemp) {
                if (self->shelfSlots[curScanTemp] != NULL) {
                    self->cursorIndex = curScanTemp;
                    return;
                }
                curScanTemp += 2;
                if (curScanTemp >= 8) {
                    curScanTemp = 5;
                }
            }
        }
    }
}

s32 EnOssan_HasPlayerSelectedItem(GlobalContext* globalCtx, EnOssan* self, Input* input) {
    EnGirlA* selectedItem = self->shelfSlots[self->cursorIndex];

    if (EnOssan_TestEndInteraction(self, globalCtx, input)) {
        return true;
    }
    if (func_80106BC8(globalCtx)) {
        if (selectedItem->actor.params != SI_SOLD_OUT && selectedItem->isInvisible == 0) {
            self->tempStateFlag = self->stateFlag;
            func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->itemBuyPromptTextId);
            self->stickLeftPrompt.isEnabled = false;
            self->stickRightPrompt.isEnabled = false;
            switch (selectedItem->actor.params) {
                case SI_KEATON_MASK:
                case SI_SPOOKY_MASK:
                case SI_SKULL_MASK:
                case SI_BUNNY_HOOD:
                case SI_MASK_OF_TRUTH:
                case SI_ZORA_MASK:
                case SI_GORON_MASK:
                case SI_GERUDO_MASK:
                    func_80078884(NA_SE_SY_DECIDE);
                    self->drawCursor = 0;
                    self->stateFlag = OSSAN_STATE_SELECT_ITEM_MASK;
                    return true;
                case SI_MILK_BOTTLE:
                    func_80078884(NA_SE_SY_DECIDE);
                    self->drawCursor = 0;
                    self->stateFlag = OSSAN_STATE_SELECT_ITEM_MILK_BOTTLE;
                    return true;
                case SI_WEIRD_EGG:
                    func_80078884(NA_SE_SY_DECIDE);
                    self->drawCursor = 0;
                    self->stateFlag = OSSAN_STATE_SELECT_ITEM_WEIRD_EGG;
                    return true;
                case SI_19:
                case SI_20:
                    func_80078884(NA_SE_SY_ERROR);
                    self->drawCursor = 0;
                    self->stateFlag = OSSAN_STATE_SELECT_ITEM_UNIMPLEMENTED;
                    return true;
                case SI_BOMBS_5_R25:
                case SI_BOMBS_10:
                case SI_BOMBS_20:
                case SI_BOMBS_30:
                case SI_BOMBS_5_R35:
                    func_80078884(NA_SE_SY_DECIDE);
                    self->drawCursor = 0;
                    self->stateFlag = OSSAN_STATE_SELECT_ITEM_BOMBS;
                    return true;
                default:
                    func_80078884(NA_SE_SY_DECIDE);
                    self->drawCursor = 0;
                    self->stateFlag = OSSAN_STATE_SELECT_ITEM;
                    return true;
            }
        }
        func_80078884(NA_SE_SY_ERROR);
        return true;
    }
    return false;
}

void EnOssan_State_BrowseLeftShelf(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    s32 a;
    s32 b;
    u8 prevIndex = self->cursorIndex;
    s32 c;
    s32 d;

    if (!EnOssan_ReturnItemToShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2152);
        self->delayTimer = 3;
        return;
    }
    if (self->delayTimer != 0) {
        self->delayTimer--;
        return;
    }
    self->drawCursor = 0xFF;
    self->stickRightPrompt.isEnabled = true;
    EnOssan_UpdateCursorPos(globalCtx, self);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) &&
        !EnOssan_HasPlayerSelectedItem(globalCtx, self, &globalCtx->state.input[0])) {
        if (self->moveHorizontal) {
            if (self->stickAccumX > 0) {
                a = EnOssan_CursorRight(self, self->cursorIndex, 4);
                if (a != CURSOR_INVALID) {
                    self->cursorIndex = a;
                } else {
                    EnOssan_SetLookToShopkeeperFromShelf(globalCtx, self);
                    return;
                }
            } else if (self->stickAccumX < 0) {
                b = EnOssan_CursorLeft(self, self->cursorIndex, 8);
                if (b != CURSOR_INVALID) {
                    self->cursorIndex = b;
                }
            }
        } else {
            if (self->stickAccumX > 0 && self->stickAccumX > 500) {
                c = EnOssan_CursorRight(self, self->cursorIndex, 4);
                if (c != CURSOR_INVALID) {
                    self->cursorIndex = c;
                } else {
                    EnOssan_SetLookToShopkeeperFromShelf(globalCtx, self);
                    return;
                }
            } else if (self->stickAccumX < 0 && self->stickAccumX < -500) {
                d = EnOssan_CursorLeft(self, self->cursorIndex, 8);
                if (d != CURSOR_INVALID) {
                    self->cursorIndex = d;
                }
            }
        }
        EnOssan_CursorUpDown(self);
        if (self->cursorIndex != prevIndex) {
            func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
            func_80078884(NA_SE_SY_CURSOR);
        }
    }
}

void EnOssan_State_BrowseRightShelf(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    s32 pad[2];
    u8 prevIndex;
    u8 nextIndex;

    prevIndex = self->cursorIndex;
    if (!EnOssan_ReturnItemToShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2244);
        self->delayTimer = 3;
        return;
    }
    if (self->delayTimer != 0) {
        self->delayTimer--;
        return;
    }
    self->drawCursor = 0xFF;
    self->stickLeftPrompt.isEnabled = true;
    EnOssan_UpdateCursorPos(globalCtx, self);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) &&
        !EnOssan_HasPlayerSelectedItem(globalCtx, self, &globalCtx->state.input[0])) {
        if (self->moveHorizontal) {
            if (self->stickAccumX < 0) {
                nextIndex = EnOssan_CursorRight(self, self->cursorIndex, 0);
                if (nextIndex != CURSOR_INVALID) {
                    self->cursorIndex = nextIndex;
                } else {
                    EnOssan_SetLookToShopkeeperFromShelf(globalCtx, self);
                    return;
                }
            } else if (self->stickAccumX > 0) {
                nextIndex = EnOssan_CursorLeft(self, self->cursorIndex, 4);
                if (nextIndex != CURSOR_INVALID) {
                    self->cursorIndex = nextIndex;
                }
            }
        } else {
            if (self->stickAccumX < 0 && self->stickAccumX < -500) {
                nextIndex = EnOssan_CursorRight(self, self->cursorIndex, 0);
                if (nextIndex != CURSOR_INVALID) {
                    self->cursorIndex = nextIndex;
                } else {
                    EnOssan_SetLookToShopkeeperFromShelf(globalCtx, self);
                    return;
                }
            } else if (self->stickAccumX > 0 && self->stickAccumX > 500) {
                nextIndex = EnOssan_CursorLeft(self, self->cursorIndex, 4);
                if (nextIndex != CURSOR_INVALID) {
                    self->cursorIndex = nextIndex;
                }
            }
        }
        EnOssan_CursorUpDown(self);
        if (self->cursorIndex != prevIndex) {
            func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
            func_80078884(NA_SE_SY_CURSOR);
        }
    }
}

void EnOssan_State_LookFromShelfToShopkeeper(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    Math_ApproachF(&self->cameraFaceAngle, 0.0f, 0.5f, 10.0f);
    if ((self->cameraFaceAngle < 0.5f) && (self->cameraFaceAngle > -0.5f)) {
        EnOssan_UpdateCameraDirection(self, globalCtx, 0.0f);
    }
    EnOssan_UpdateCameraDirection(self, globalCtx, self->cameraFaceAngle);
    if (self->cameraFaceAngle == 0.0f) {
        EnOssan_StartShopping(globalCtx, self);
    }
}

void EnOssan_State_DisplayOnlyBombDialog(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (!EnOssan_ReturnItemToShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2355);
        return;
    }
    Math_ApproachF(&self->cameraFaceAngle, 0.0f, 0.5f, 10.0f);
    if (self->cameraFaceAngle < 0.5f && self->cameraFaceAngle > -0.5f) {
        EnOssan_UpdateCameraDirection(self, globalCtx, 0.0f);
    }
    EnOssan_UpdateCameraDirection(self, globalCtx, self->cameraFaceAngle);
    if (self->cameraFaceAngle == 0.0f) {
        func_8010B720(globalCtx, 0x3010);
        self->stateFlag = OSSAN_STATE_WAIT_FOR_DISPLAY_ONLY_BOMB_DIALOG;
    }
}

void EnOssan_GiveItemWithFanfare(GlobalContext* globalCtx, EnOssan* self) {
    Player* player = GET_PLAYER(globalCtx);

    osSyncPrintf("\n" VT_FGCOL(YELLOW) "初めて手にいれた！！" VT_RST "\n\n");
    func_8002F434(&self->actor, globalCtx, self->shelfSlots[self->cursorIndex]->getItemId, 120.0f, 120.0f);
    globalCtx->msgCtx.msgMode = 0x36;
    globalCtx->msgCtx.unk_E3E7 = 4;
    player->stateFlags2 &= ~0x20000000;
    func_800BC490(globalCtx, 1);
    Interface_ChangeAlpha(50);
    self->drawCursor = 0;
    EnOssan_UpdateCameraDirection(self, globalCtx, 0.0f);
    self->stateFlag = OSSAN_STATE_GIVE_ITEM_FANFARE;
    osSyncPrintf(VT_FGCOL(YELLOW) "持ち上げ開始！！" VT_RST "\n\n");
}

void EnOssan_SetStateCantGetItem(GlobalContext* globalCtx, EnOssan* self, u16 textId) {
    func_8010B720(globalCtx, textId);
    self->stateFlag = OSSAN_STATE_CANT_GET_ITEM;
}

void EnOssan_SetStateQuickBuyDialog(GlobalContext* globalCtx, EnOssan* self, u16 textId) {
    func_8010B720(globalCtx, textId);
    self->stateFlag = OSSAN_STATE_QUICK_BUY;
}

void EnOssan_HandleCanBuyItem(GlobalContext* globalCtx, EnOssan* self) {
    EnGirlA* selectedItem = self->shelfSlots[self->cursorIndex];

    switch (selectedItem->canBuyFunc(globalCtx, selectedItem)) {
        case CANBUY_RESULT_SUCCESS_FANFARE:
            if (selectedItem->actor.params == SI_HYLIAN_SHIELD && gSaveContext.infTable[7] & 0x40) {
                EnOssan_SetStateGiveDiscountDialog(globalCtx, self);
            } else {
                EnOssan_GiveItemWithFanfare(globalCtx, self);
                self->drawCursor = 0;
                self->shopItemSelectedTween = 0.0f;
                selectedItem->setOutOfStockFunc(globalCtx, selectedItem);
            }
            break;
        case CANBUY_RESULT_SUCCESS:
            selectedItem->itemGiveFunc(globalCtx, selectedItem);
            EnOssan_SetStateQuickBuyDialog(globalCtx, self, 0x84);
            self->drawCursor = 0;
            self->shopItemSelectedTween = 0.0f;
            selectedItem->setOutOfStockFunc(globalCtx, selectedItem);
            break;
        case CANBUY_RESULT_CANT_GET_NOW:
            func_80078884(NA_SE_SY_ERROR);
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x86);
            break;
        case CANBUY_RESULT_NEED_BOTTLE:
            func_80078884(NA_SE_SY_ERROR);
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x96);
            break;
        case CANBUY_RESULT_NEED_RUPEES:
            func_80078884(NA_SE_SY_ERROR);
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x85);
            break;
        case CANBUY_RESULT_CANT_GET_NOW_5:
            func_80078884(NA_SE_SY_ERROR);
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x86);
            break;
    }
}

void EnOssan_HandleCanBuyLonLonMilk(GlobalContext* globalCtx, EnOssan* self) {
    EnGirlA* item = self->shelfSlots[self->cursorIndex];

    switch (item->canBuyFunc(globalCtx, item)) {
        case CANBUY_RESULT_SUCCESS_FANFARE:
            func_8010B720(globalCtx, 0x9C);
            self->stateFlag = OSSAN_STATE_GIVE_LON_LON_MILK;
            self->drawCursor = 0;
            break;
        case CANBUY_RESULT_SUCCESS:
            item->itemGiveFunc(globalCtx, item);
            EnOssan_SetStateQuickBuyDialog(globalCtx, self, 0x98);
            self->drawCursor = 0;
            self->shopItemSelectedTween = 0.0f;
            item->setOutOfStockFunc(globalCtx, item);
            break;
        case CANBUY_RESULT_NEED_BOTTLE:
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x96);
            break;
        case CANBUY_RESULT_NEED_RUPEES:
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x85);
            break;
    }
}

void EnOssan_HandleCanBuyWeirdEgg(GlobalContext* globalCtx, EnOssan* self) {
    EnGirlA* item = self->shelfSlots[self->cursorIndex];

    switch (item->canBuyFunc(globalCtx, item)) {
        case CANBUY_RESULT_SUCCESS_FANFARE:
            EnOssan_GiveItemWithFanfare(globalCtx, self);
            self->drawCursor = 0;
            self->shopItemSelectedTween = 0.0f;
            item->setOutOfStockFunc(globalCtx, item);
            break;
        case CANBUY_RESULT_SUCCESS:
            item->itemGiveFunc(globalCtx, item);
            EnOssan_SetStateQuickBuyDialog(globalCtx, self, 0x9A);
            self->drawCursor = 0;
            self->shopItemSelectedTween = 0.0f;
            item->setOutOfStockFunc(globalCtx, item);
            break;
        case CANBUY_RESULT_CANT_GET_NOW:
            func_80078884(NA_SE_SY_ERROR);
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x9D);
            break;
        case CANBUY_RESULT_NEED_RUPEES:
            func_80078884(NA_SE_SY_ERROR);
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x85);
            break;
    }
}

void EnOssan_HandleCanBuyBombs(GlobalContext* globalCtx, EnOssan* self) {
    EnGirlA* item = self->shelfSlots[self->cursorIndex];

    switch (item->canBuyFunc(globalCtx, item)) {
        case CANBUY_RESULT_SUCCESS_FANFARE:
        case CANBUY_RESULT_SUCCESS:
            item->itemGiveFunc(globalCtx, item);
            EnOssan_SetStateQuickBuyDialog(globalCtx, self, 0x84);
            self->drawCursor = 0;
            self->shopItemSelectedTween = 0.0f;
            item->setOutOfStockFunc(globalCtx, item);
            break;
        case CANBUY_RESULT_CANT_GET_NOW:
            func_80078884(NA_SE_SY_ERROR);
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x86);
            break;
        case CANBUY_RESULT_NEED_RUPEES:
            func_80078884(NA_SE_SY_ERROR);
            EnOssan_SetStateCantGetItem(globalCtx, self, 0x85);
            break;
    }
}

void EnOssan_BuyGoronCityBombs(GlobalContext* globalCtx, EnOssan* self) {
    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        if (!(gSaveContext.eventChkInf[2] & 0x20)) {
            if (gSaveContext.infTable[15] & 0x1000) {
                EnOssan_SetStateCantGetItem(globalCtx, self, 0x302E);
            } else {
                self->stickLeftPrompt.isEnabled = false;
                self->stickRightPrompt.isEnabled = false;
                self->drawCursor = 0;
                self->stateFlag = OSSAN_STATE_DISPLAY_ONLY_BOMB_DIALOG;
            }
        } else {
            EnOssan_HandleCanBuyBombs(globalCtx, self);
        }
    } else {
        EnOssan_HandleCanBuyBombs(globalCtx, self);
    }
}

void EnOssan_State_ItemSelected(EnOssan* self, GlobalContext* globalCtx2, Player* player) {
    GlobalContext* globalCtx = globalCtx2; // Necessary for OKs

    if (!EnOssan_TakeItemOffShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2654);
        return;
    }
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 &&
        !EnOssan_TestCancelOption(self, globalCtx, &globalCtx->state.input[0]) && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                EnOssan_HandleCanBuyItem(globalCtx, self);
                break;
            case 1:
                self->stateFlag = self->tempStateFlag;
                func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
                break;
        }
    }
}

void EnOssan_State_SelectMilkBottle(EnOssan* self, GlobalContext* globalCtx2, Player* player) {
    GlobalContext* globalCtx = globalCtx2; // Need for OK

    if (!EnOssan_TakeItemOffShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2693);
        return;
    }
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 &&
        !EnOssan_TestCancelOption(self, globalCtx, &globalCtx->state.input[0]) && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                EnOssan_HandleCanBuyLonLonMilk(globalCtx, self);
                break;
            case 1:
                self->stateFlag = self->tempStateFlag;
                func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
                break;
        }
    }
}

void EnOssan_State_SelectWeirdEgg(EnOssan* self, GlobalContext* globalCtx2, Player* player) {
    GlobalContext* globalCtx = globalCtx2; // Needed for OK

    if (!EnOssan_TakeItemOffShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2732);
        return;
    }
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 &&
        !EnOssan_TestCancelOption(self, globalCtx, &globalCtx->state.input[0]) && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                EnOssan_HandleCanBuyWeirdEgg(globalCtx, self);
                break;
            case 1:
                self->stateFlag = self->tempStateFlag;
                func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
                break;
        }
    }
}

void EnOssan_State_SelectUnimplementedItem(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (!EnOssan_TakeItemOffShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2771);
        return;
    }
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        self->stateFlag = self->tempStateFlag;
        func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
    }
}

void EnOssan_State_SelectBombs(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (!EnOssan_TakeItemOffShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2798);
        return;
    }
    osSyncPrintf("店主の依頼 ( %d )\n", gSaveContext.infTable[15] & 0x1000);
    if (self->actor.params != OSSAN_TYPE_GORON) {
        EnOssan_State_ItemSelected(self, globalCtx, player);
        return;
    }
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 &&
        !EnOssan_TestCancelOption(self, globalCtx, &globalCtx->state.input[0]) && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                EnOssan_BuyGoronCityBombs(globalCtx, self);
                break;
            case 1:
                self->stateFlag = self->tempStateFlag;
                func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
                break;
        }
    }
}

void EnOssan_State_SelectMaskItem(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    u8 talkState = func_8010BDBC(&globalCtx->msgCtx);
    EnGirlA* item = self->shelfSlots[self->cursorIndex];

    if (!EnOssan_TakeItemOffShelf(self)) {
        osSyncPrintf("%s[%d]:" VT_FGCOL(GREEN) "ズーム中！！" VT_RST "\n", "../z_en_oB1.c", 2845);
        return;
    }
    if (talkState == 5) {
        if (func_80106BC8(globalCtx)) {
            self->stateFlag = self->tempStateFlag;
            func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
        }
    } else if (talkState == 4 && !EnOssan_TestCancelOption(self, globalCtx, &globalCtx->state.input[0]) &&
               func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                switch (item->actor.params) {
                    case SI_KEATON_MASK:
                        gSaveContext.itemGetInf[2] |= 0x08;
                        break;
                    case SI_SPOOKY_MASK:
                        gSaveContext.itemGetInf[2] |= 0x20;
                        break;
                    case SI_SKULL_MASK:
                        gSaveContext.itemGetInf[2] |= 0x10;
                        break;
                    case SI_BUNNY_HOOD:
                        gSaveContext.itemGetInf[2] |= 0x40;
                        break;
                    case SI_MASK_OF_TRUTH:
                    case SI_ZORA_MASK:
                    case SI_GORON_MASK:
                    case SI_GERUDO_MASK:
                        break;
                }
                EnOssan_GiveItemWithFanfare(globalCtx, self);
                self->drawCursor = 0;
                self->shopItemSelectedTween = 0.0f;
                item->setOutOfStockFunc(globalCtx, item);
                break;
            case 1:
                self->stateFlag = self->tempStateFlag;
                func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
                break;
        }
    }
}

void EnOssan_State_CantGetItem(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        self->stateFlag = self->tempStateFlag;
        func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
    }
}

void EnOssan_State_QuickBuyDialog(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    EnGirlA* item;

    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        self->shopItemSelectedTween = 0.0f;
        EnOssan_ResetItemPosition(self);
        item = self->shelfSlots[self->cursorIndex];
        item->updateStockedItemFunc(globalCtx, item);
        self->stateFlag = self->tempStateFlag;
        func_8010B720(globalCtx, self->shelfSlots[self->cursorIndex]->actor.textId);
    }
}

void EnOssan_State_GiveItemWithFanfare(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    // The player sets itself as the parent actor to signal that it has obtained the give item request
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->stateFlag = OSSAN_STATE_ITEM_PURCHASED;
        return;
    }
    func_8002F434(&self->actor, globalCtx, self->shelfSlots[self->cursorIndex]->getItemId, 120.0f, 120.0f);
}

void EnOssan_State_ItemPurchased(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    EnGirlA* item;
    EnGirlA* itemTemp;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && func_80106BC8(globalCtx)) {
        if (self->actor.params == OSSAN_TYPE_MASK) {
            itemTemp = self->shelfSlots[self->cursorIndex];
            EnOssan_ResetItemPosition(self);
            item = self->shelfSlots[self->cursorIndex];
            item->updateStockedItemFunc(globalCtx, item);
            if (itemTemp->actor.params == SI_MASK_OF_TRUTH && !(gSaveContext.itemGetInf[3] & 0x8000)) {
                gSaveContext.itemGetInf[3] |= 0x8000;
                func_8010B720(globalCtx, 0x70AB);
                self->happyMaskShopState = OSSAN_HAPPY_STATE_BORROWED_FIRST_MASK;
                EnOssan_UpdateShopOfferings(self, globalCtx);
                self->stateFlag = OSSAN_STATE_START_CONVERSATION;
                return;
            } else {
                EnOssan_EndInteraction(globalCtx, self);
                return;
            }
        }
        item = self->shelfSlots[self->cursorIndex];
        item->buyEventFunc(globalCtx, item);
        self->stateFlag = OSSAN_STATE_CONTINUE_SHOPPING_PROMPT;
        func_8010B720(globalCtx, 0x6B);
    }
}

void EnOssan_State_ContinueShoppingPrompt(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    EnGirlA* selectedItem;
    u8 talkState = func_8010BDBC(&globalCtx->msgCtx);

    if (talkState == 4) {
        if (func_80106BC8(globalCtx)) {
            EnOssan_ResetItemPosition(self);
            selectedItem = self->shelfSlots[self->cursorIndex];
            selectedItem->updateStockedItemFunc(globalCtx, selectedItem);
            if (!EnOssan_TestEndInteraction(self, globalCtx, &globalCtx->state.input[0])) {
                switch (globalCtx->msgCtx.choiceIndex) {
                    case 0:
                        osSyncPrintf(VT_FGCOL(YELLOW) "★★★ 続けるよ！！ ★★★" VT_RST "\n");
                        player->actor.shape.rot.y += 0x8000;
                        player->stateFlags2 |= 0x20000000;
                        func_800BC490(globalCtx, 2);
                        func_8010B680(globalCtx, self->actor.textId, &self->actor);
                        EnOssan_SetStateStartShopping(globalCtx, self, true);
                        func_8002F298(&self->actor, globalCtx, 100.0f, -1);
                        break;
                    case 1:
                    default:
                        osSyncPrintf(VT_FGCOL(YELLOW) "★★★ やめるよ！！ ★★★" VT_RST "\n");
                        EnOssan_EndInteraction(globalCtx, self);
                        break;
                }
            }
        }
    } else if (talkState == 5 && func_80106BC8(globalCtx)) {
        EnOssan_ResetItemPosition(self);
        selectedItem = self->shelfSlots[self->cursorIndex];
        selectedItem->updateStockedItemFunc(globalCtx, selectedItem);
        player->actor.shape.rot.y += 0x8000;
        player->stateFlags2 |= 0x20000000;
        func_800BC490(globalCtx, 2);
        func_8010B680(globalCtx, self->actor.textId, &self->actor);
        EnOssan_SetStateStartShopping(globalCtx, self, true);
        func_8002F298(&self->actor, globalCtx, 100.0f, -1);
    }
}

void EnOssan_State_WaitForDisplayOnlyBombDialog(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        gSaveContext.infTable[15] |= 0x1000;
        EnOssan_StartShopping(globalCtx, self);
    }
}

// Unreachable
void EnOssan_State_21(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 1 && func_80106BC8(globalCtx)) {
        self->stateFlag = OSSAN_STATE_22;
        func_8010B720(globalCtx, 0x3012);
        gSaveContext.infTable[15] |= 0x1000;
    }
}

// Unreachable
void EnOssan_State_22(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        EnOssan_StartShopping(globalCtx, self);
    }
}

void EnOssan_State_GiveLonLonMilk(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        EnOssan_GiveItemWithFanfare(globalCtx, self);
    }
}

// For giving Mask of Truth when you first sell all masks
void EnOssan_State_LendMaskOfTruth(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 5 && func_80106BC8(globalCtx)) {
        gSaveContext.itemGetInf[2] |= 0x400;
        self->cursorIndex = 2;
        EnOssan_GiveItemWithFanfare(globalCtx, self);
    }
}

// Hylian Shield discount dialog
void EnOssan_SetStateGiveDiscountDialog(GlobalContext* globalCtx, EnOssan* self) {
    func_8010B720(globalCtx, 0x71B2);
    self->stateFlag = OSSAN_STATE_DISCOUNT_DIALOG;
}

void EnOssan_State_GiveDiscountDialog(EnOssan* self, GlobalContext* globalCtx, Player* player) {
    EnGirlA* selectedItem;

    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        selectedItem = self->shelfSlots[self->cursorIndex];
        EnOssan_GiveItemWithFanfare(globalCtx, self);
        self->drawCursor = 0;
        self->shopItemSelectedTween = 0.0f;
        selectedItem->setOutOfStockFunc(globalCtx, selectedItem);
    }
}

void EnOssan_PositionSelectedItem(EnOssan* self) {
    EnGirlA* item;
    u8 i;
    u8 i2;
    ShopItem* shopItem;
    f32 tx;
    f32 ty;
    f32 tz;

    i = self->cursorIndex;
    shopItem = &sShopkeeperStores[self->actor.params][i];
    item = self->shelfSlots[i];

    i2 = i >> 2;
    tx = (sSelectedItemPosition[i2].x - shopItem->xOffset) * self->shopItemSelectedTween + shopItem->xOffset;
    ty = (sSelectedItemPosition[i2].y - shopItem->yOffset) * self->shopItemSelectedTween + shopItem->yOffset;
    tz = (sSelectedItemPosition[i2].z - shopItem->zOffset) * self->shopItemSelectedTween + shopItem->zOffset;

    item->actor.world.pos.x = self->shelves->actor.world.pos.x + tx;
    item->actor.world.pos.y = self->shelves->actor.world.pos.y + ty;
    item->actor.world.pos.z = self->shelves->actor.world.pos.z + tz;
}

void EnOssan_ResetItemPosition(EnOssan* self) {
    self->shopItemSelectedTween = 0.0f;
    EnOssan_PositionSelectedItem(self);
}

// returns true if animation has completed
s32 EnOssan_TakeItemOffShelf(EnOssan* self) {
    Math_ApproachF(&self->shopItemSelectedTween, 1.0f, 1.0f, 0.15f);
    if (self->shopItemSelectedTween >= 0.85f) {
        self->shopItemSelectedTween = 1.0f;
    }
    EnOssan_PositionSelectedItem(self);
    if (self->shopItemSelectedTween == 1.0f) {
        return true;
    } else {
        return false;
    }
}

// returns true if animation has completed
s32 EnOssan_ReturnItemToShelf(EnOssan* self) {
    Math_ApproachF(&self->shopItemSelectedTween, 0.0f, 1.0f, 0.15f);
    if (self->shopItemSelectedTween <= 0.15f) {
        self->shopItemSelectedTween = 0.0f;
    }
    EnOssan_PositionSelectedItem(self);
    if (self->shopItemSelectedTween == 0.0f) {
        return true;
    } else {
        return false;
    }
}

void EnOssan_UpdateItemSelectedProperty(EnOssan* self) {
    EnGirlA** temp_a1 = self->shelfSlots;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (temp_a1[0] != NULL) {
            if (self->stateFlag != OSSAN_STATE_SELECT_ITEM && self->stateFlag != OSSAN_STATE_SELECT_ITEM_MILK_BOTTLE &&
                self->stateFlag != OSSAN_STATE_SELECT_ITEM_WEIRD_EGG &&
                self->stateFlag != OSSAN_STATE_SELECT_ITEM_UNIMPLEMENTED &&
                self->stateFlag != OSSAN_STATE_SELECT_ITEM_BOMBS && self->stateFlag != OSSAN_STATE_SELECT_ITEM_MASK &&
                self->stateFlag != OSSAN_STATE_CANT_GET_ITEM && self->drawCursor == 0) {
                temp_a1[0]->isSelected = false;
            } else {
                if (self->cursorIndex == i) {
                    temp_a1[0]->isSelected = true;
                } else {
                    temp_a1[0]->isSelected = false;
                }
            }
        }
        temp_a1++;
    }
}

void EnOssan_UpdateCursorAnim(EnOssan* self) {
    f32 t;

    t = self->cursorAnimTween;
    if (self->cursorAnimState == 0) {
        t += 0.05f;
        if (t >= 1.0f) {
            t = 1.0f;
            self->cursorAnimState = 1;
        }
    } else {
        t -= 0.05f;
        if (t <= 0.0f) {
            t = 0.0f;
            self->cursorAnimState = 0;
        }
    }
    self->cursorColorR = ColChanMix(0, 0.0f, t);
    self->cursorColorG = ColChanMix(255, 80.0f, t);
    self->cursorColorB = ColChanMix(80, 0.0f, t);
    self->cursorColorA = ColChanMix(255, 0.0f, t);
    self->cursorAnimTween = t;
}

void EnOssan_UpdateStickDirectionPromptAnim(EnOssan* self) {
    f32 arrowAnimTween;
    f32 new_var3;       // likely fake temp
    s32 new_var2 = 255; // likely fake temp
    f32 stickAnimTween;

    arrowAnimTween = self->arrowAnimTween;
    stickAnimTween = self->stickAnimTween;
    if (self->arrowAnimState == 0) {
        arrowAnimTween += 0.05f;
        if (arrowAnimTween > 1.0f) {
            arrowAnimTween = 1.0f;
            self->arrowAnimState = 1;
        }

    } else {
        arrowAnimTween -= 0.05f;
        if (arrowAnimTween < 0.0f) {
            arrowAnimTween = 0.0f;
            self->arrowAnimState = 0;
        }
    }

    self->arrowAnimTween = arrowAnimTween;
    if (self->stickAnimState == 0) {
        stickAnimTween += 0.1f;
        if (stickAnimTween > 1.0f) {
            stickAnimTween = 1.0f;
            self->stickAnimState = 1;
        }

    } else {
        stickAnimTween = 0.0f;
        self->stickAnimState = 0;
    }

    self->stickAnimTween = stickAnimTween;
    self->stickLeftPrompt.arrowColorR = (u8)(255 - ((s32)(155.0f * arrowAnimTween)));
    self->stickLeftPrompt.arrowColorG = (u8)(new_var2 - (s32)(155.0f * arrowAnimTween));
    new_var3 = (155.0f * arrowAnimTween);
    self->stickLeftPrompt.arrowColorB = (u8)(0 - ((s32)((-100.0f) * arrowAnimTween)));
    self->stickLeftPrompt.arrowColorA = (u8)(200 - ((s32)(50.0f * arrowAnimTween)));
    self->stickRightPrompt.arrowColorR = (u8)(new_var2 - (s32)new_var3);
    self->stickRightPrompt.arrowColorG = (u8)(255 - (s32)new_var3);
    self->stickRightPrompt.arrowColorB = (u8)(0 - ((s32)((-100.0f) * arrowAnimTween)));
    self->stickRightPrompt.arrowColorA = (u8)(200 - ((s32)(50.0f * arrowAnimTween)));
    self->stickRightPrompt.arrowTexX = 290.0f;
    self->stickLeftPrompt.arrowTexX = 33.0f;
    self->stickRightPrompt.stickTexX = 274.0f;
    self->stickLeftPrompt.stickTexX = 49.0f;
    self->stickRightPrompt.stickTexX += (8.0f * stickAnimTween);
    self->stickLeftPrompt.stickTexX -= (8.0f * stickAnimTween);
    self->stickLeftPrompt.arrowTexY = self->stickRightPrompt.arrowTexY = 91.0f;
    self->stickLeftPrompt.stickTexY = self->stickRightPrompt.stickTexY = 95.0f;
}

void EnOssan_WaitForBlink(EnOssan* self) {
    s16 decr = self->blinkTimer - 1;

    if (decr != 0) {
        self->blinkTimer = decr;
    } else {
        self->blinkFunc = EnOssan_Blink;
    }
}

void EnOssan_Blink(EnOssan* self) {
    s16 decr;
    s16 eyeTextureIdxTemp;

    decr = self->blinkTimer - 1;
    if (decr != 0) {
        self->blinkTimer = decr;
        return;
    }
    eyeTextureIdxTemp = self->eyeTextureIdx + 1;
    if (eyeTextureIdxTemp > 2) {
        self->eyeTextureIdx = 0;
        self->blinkTimer = (s32)(Rand_ZeroOne() * 60.0f) + 20;
        self->blinkFunc = EnOssan_WaitForBlink;
    } else {
        self->eyeTextureIdx = eyeTextureIdxTemp;
        self->blinkTimer = 1;
    }
}

s32 EnOssan_AreShopkeeperObjectsLoaded(EnOssan* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->objBankIndex1)) {
        if (self->objBankIndex2 >= 0 && !Object_IsLoaded(&globalCtx->objectCtx, self->objBankIndex2)) {
            return false;
        }
        if (self->objBankIndex3 >= 0 && !Object_IsLoaded(&globalCtx->objectCtx, self->objBankIndex3)) {
            return false;
        }
        return true;
    }
    return false;
}

void EnOssan_InitBazaarShopkeeper(EnOssan* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gObjectOssanSkel, &gObjectOssanAnim_000338, NULL, NULL, 0);
    self->actor.draw = EnOssan_DrawBazaarShopkeeper;
    self->obj3ToSeg6Func = NULL;
}

void EnOssan_InitKokiriShopkeeper(EnOssan* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gKm1Skel, NULL, NULL, NULL, 0);
    gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[self->objBankIndex3].segment);
    Animation_Change(&self->skelAnime, &object_masterkokiri_Anim_0004A8, 1.0f, 0.0f,
                     Animation_GetLastFrame(&object_masterkokiri_Anim_0004A8), 0, 0.0f);
    self->actor.draw = EnOssan_DrawKokiriShopkeeper;
    self->obj3ToSeg6Func = EnOssan_Obj3ToSeg6;
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_ELF, self->actor.world.pos.x,
                       self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, FAIRY_KOKIRI);
}

void EnOssan_InitGoronShopkeeper(EnOssan* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGoronSkel, NULL, NULL, NULL, 0);
    gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[self->objBankIndex3].segment);
    Animation_Change(&self->skelAnime, &gGoronShopkeeperAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gGoronShopkeeperAnim),
                     0, 0.0f);
    self->actor.draw = EnOssan_DrawGoronShopkeeper;
    self->obj3ToSeg6Func = EnOssan_Obj3ToSeg6;
}

void EnOssan_InitZoraShopkeeper(EnOssan* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gZoraSkel, NULL, NULL, NULL, 0);
    gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[self->objBankIndex3].segment);
    Animation_Change(&self->skelAnime, &gZoraShopkeeperAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gZoraShopkeeperAnim),
                     0, 0.0f);
    self->actor.draw = EnOssan_DrawZoraShopkeeper;
    self->obj3ToSeg6Func = EnOssan_Obj3ToSeg6;
}

void EnOssan_InitPotionShopkeeper(EnOssan* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_ds2_Skel_004258, &object_ds2_Anim_0002E4, 0, 0, 0);
    self->actor.draw = EnOssan_DrawPotionShopkeeper;
    self->obj3ToSeg6Func = NULL;
}

void EnOssan_InitHappyMaskShopkeeper(EnOssan* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_os_Skel_004658, &object_os_Anim_0002E4, NULL, NULL, 0);
    self->actor.draw = EnOssan_DrawHappyMaskShopkeeper;
    self->obj3ToSeg6Func = NULL;
}

void EnOssan_InitBombchuShopkeeper(EnOssan* self, GlobalContext* globalCtx) {
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_rs_Skel_004868, &object_rs_Anim_00065C, 0, 0, 0);
    self->actor.draw = EnOssan_DrawBombchuShopkeeper;
    self->obj3ToSeg6Func = NULL;
}

u16 EnOssan_SetupHelloDialog(EnOssan* self) {
    self->happyMaskShopState = OSSAN_HAPPY_STATE_NONE;
    // mask shop messages
    if (self->actor.params == OSSAN_TYPE_MASK) {
        if (INV_CONTENT(ITEM_TRADE_CHILD) == ITEM_SOLD_OUT) {
            if (gSaveContext.itemGetInf[3] & 0x800) {
                if (!(gSaveContext.eventChkInf[8] & 0x8000)) {
                    // Pay back Bunny Hood
                    self->happyMaskShopState = OSSAN_HAPPY_STATE_REQUEST_PAYMENT_BUNNY_HOOD;
                    return 0x70C6;
                } else {
                    return 0x70AC;
                }
            }
            if (gSaveContext.itemGetInf[3] & 0x400) {
                if (!(gSaveContext.eventChkInf[8] & 0x4000)) {
                    // Pay back Spooky Mask
                    self->happyMaskShopState = OSSAN_HAPPY_STATE_REQUEST_PAYMENT_SPOOKY_MASK;
                    return 0x70C5;
                } else {
                    return 0x70AC;
                }
            }
            if (gSaveContext.itemGetInf[3] & 0x200) {
                if (!(gSaveContext.eventChkInf[8] & 0x2000)) {
                    // Pay back Skull Mask
                    self->happyMaskShopState = OSSAN_HAPPY_STATE_REQUEST_PAYMENT_SKULL_MASK;
                    return 0x70C4;
                } else {
                    return 0x70AC;
                }
            }
            if (gSaveContext.itemGetInf[3] & 0x100) {
                if (!(gSaveContext.eventChkInf[8] & 0x1000)) {
                    // Pay back Keaton Mask
                    self->happyMaskShopState = OSSAN_HAPPY_STATE_REQUEST_PAYMENT_KEATON_MASK;
                    return 0x70A5;
                } else {
                    return 0x70AC;
                }
            }
        } else {
            if (gSaveContext.itemGetInf[3] & 0x800) {
                return 0x70AC;
            } else if (!(gSaveContext.itemGetInf[3] & 0x400) && !(gSaveContext.itemGetInf[2] & 0x10) &&
                       !(gSaveContext.itemGetInf[3] & 0x100)) {
                // Haven't borrowed the Keaton Mask
                if (!(gSaveContext.itemGetInf[2] & 0x8)) {
                    return 0x70A1;
                } else {
                    // Haven't sold the Keaton Mask
                    self->happyMaskShopState = OSSAN_HAPPY_STATE_BORROWED_FIRST_MASK;
                    return 0x70A6;
                }
            } else {
                return 0x70C7;
            }
        }
    }

    return 0x9E;
}

void EnOssan_InitActionFunc(EnOssan* self, GlobalContext* globalCtx) {
    ShopItem* items;

    if (EnOssan_AreShopkeeperObjectsLoaded(self, globalCtx)) {
        self->actor.flags &= ~0x10;
        self->actor.objBankIndex = self->objBankIndex1;
        Actor_SetObjectDependency(globalCtx, &self->actor);

        self->shelves = (EnTana*)Actor_Find(&globalCtx->actorCtx, ACTOR_EN_TANA, ACTORCAT_PROP);

        if (self->shelves == NULL) {
            osSyncPrintf(VT_COL(RED, WHITE));
            // "Warning!! There are no shelves!!"
            osSyncPrintf("★★★ 警告！！ 棚がないよ！！ ★★★\n");
            osSyncPrintf(VT_RST);
            return;
        }

        // "Shopkeeper (params) init"
        osSyncPrintf(VT_FGCOL(YELLOW) "◇◇◇ 店のおやじ( %d ) 初期設定 ◇◇◇" VT_RST "\n", self->actor.params);

        self->actor.world.pos.x += sShopkeeperPositionOffsets[self->actor.params].x;
        self->actor.world.pos.y += sShopkeeperPositionOffsets[self->actor.params].y;
        self->actor.world.pos.z += sShopkeeperPositionOffsets[self->actor.params].z;

        items = sShopkeeperStores[self->actor.params];

        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
        sInitFuncs[self->actor.params](self, globalCtx);
        self->actor.textId = EnOssan_SetupHelloDialog(self);
        self->cursorY = self->cursorX = 100.0f;
        self->actor.colChkInfo.mass = MASS_IMMOVABLE;
        self->actor.colChkInfo.cylRadius = 50;
        self->stateFlag = OSSAN_STATE_IDLE;
        self->stickAccumX = self->stickAccumY = 0;

        self->cursorIndex = 0;
        self->cursorZ = 1.5f;
        self->cursorColorR = 0;
        self->cursorColorG = 255;
        self->cursorColorB = 80;
        self->cursorColorA = 255;
        self->cursorAnimTween = 0;

        self->cursorAnimState = 0;
        self->drawCursor = 0;
        self->happyMaskShopkeeperEyeIdx = 0;

        self->stickLeftPrompt.stickColorR = 200;
        self->stickLeftPrompt.stickColorG = 200;
        self->stickLeftPrompt.stickColorB = 200;
        self->stickLeftPrompt.stickColorA = 180;
        self->stickLeftPrompt.stickTexX = 49;
        self->stickLeftPrompt.stickTexY = 95;
        self->stickLeftPrompt.arrowColorR = 255;
        self->stickLeftPrompt.arrowColorG = 255;
        self->stickLeftPrompt.arrowColorB = 0;
        self->stickLeftPrompt.arrowColorA = 200;
        self->stickLeftPrompt.arrowTexX = 33;
        self->stickLeftPrompt.arrowTexY = 91;
        self->stickLeftPrompt.z = 1;
        self->stickLeftPrompt.isEnabled = false;

        self->stickRightPrompt.stickColorR = 200;
        self->stickRightPrompt.stickColorG = 200;
        self->stickRightPrompt.stickColorB = 200;
        self->stickRightPrompt.stickColorA = 180;
        self->stickRightPrompt.stickTexX = 274;
        self->stickRightPrompt.stickTexY = 95;
        self->stickRightPrompt.arrowColorR = 255;
        self->stickRightPrompt.arrowColorG = 255;
        self->stickRightPrompt.arrowColorB = 0;
        self->stickRightPrompt.arrowColorA = 200;
        self->stickRightPrompt.arrowTexX = 290;
        self->stickRightPrompt.arrowTexY = 91;
        self->stickRightPrompt.z = 1;
        self->stickRightPrompt.isEnabled = false;

        self->arrowAnimState = 0;
        self->stickAnimState = 0;
        self->arrowAnimTween = 0;
        self->stickAnimTween = 0;
        self->shopItemSelectedTween = 0;
        Actor_SetScale(&self->actor, sShopkeeperScale[self->actor.params]);
        EnOssan_SpawnItemsOnShelves(self, globalCtx, items);
        self->headRot = self->headTargetRot = 0;
        self->blinkTimer = 20;
        self->eyeTextureIdx = 0;
        self->blinkFunc = EnOssan_WaitForBlink;
        self->actor.flags &= ~1;
        EnOssan_SetupAction(self, EnOssan_MainActionFunc);
    }
}

void EnOssan_Obj3ToSeg6(EnOssan* self, GlobalContext* globalCtx) {
    gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->objBankIndex3].segment);
}

void EnOssan_MainActionFunc(EnOssan* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->blinkFunc(self);
    EnOssan_UpdateJoystickInputState(globalCtx, self);
    EnOssan_UpdateItemSelectedProperty(self);
    EnOssan_UpdateStickDirectionPromptAnim(self);
    EnOssan_UpdateCursorAnim(self);
    Math_StepToS(&self->headRot, self->headTargetRot, 0x190);

    if (player != NULL) {
        sStateFunc[self->stateFlag](self, globalCtx, player);
    }

    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 26.0f, 10.0f, 0.0f, 5);
    Actor_SetFocus(&self->actor, 90.0f);
    Actor_SetScale(&self->actor, sShopkeeperScale[self->actor.params]);

    // use animation object if needed
    if (self->obj3ToSeg6Func != NULL) {
        self->obj3ToSeg6Func(self, globalCtx);
    }

    SkelAnime_Update(&self->skelAnime);
}

void EnOssan_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnOssan* self = THIS;

    self->timer++;
    self->actionFunc(self, globalCtx);
}

s32 EnOssan_OverrideLimbDrawDefaultShopkeeper(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos,
                                              Vec3s* rot, void* thisx) {
    EnOssan* self = THIS;

    if (limbIndex == 8) {
        rot->x += self->headRot;
    }
    return 0;
}

void EnOssan_DrawCursor(GlobalContext* globalCtx, EnOssan* self, f32 x, f32 y, f32 z, u8 drawCursor) {
    s32 ulx, uly, lrx, lry;
    f32 w;
    s32 dsdx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4192);
    if (drawCursor != 0) {
        func_80094520(globalCtx->state.gfxCtx);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, self->cursorColorR, self->cursorColorG, self->cursorColorB,
                        self->cursorColorA);
        gDPLoadTextureBlock_4b(OVERLAY_DISP++, gSelectionCursorTex, G_IM_FMT_IA, 16, 16, 0, G_TX_MIRROR | G_TX_WRAP,
                               G_TX_MIRROR | G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD);
        w = 16.0f * z;
        ulx = (x - w) * 4.0f;
        uly = (y - w) * 4.0f;
        lrx = (x + w) * 4.0f;
        lry = (y + w) * 4.0f;
        dsdx = (1.0f / z) * 1024.0f;
        gSPTextureRectangle(OVERLAY_DISP++, ulx, uly, lrx, lry, G_TX_RENDERTILE, 0, 0, dsdx, dsdx);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4215);
}

void EnOssan_DrawTextRec(GlobalContext* globalCtx, s32 r, s32 g, s32 b, s32 a, f32 x, f32 y, f32 z, s32 s, s32 t,
                         f32 dx, f32 dy) {
    f32 unk;
    s32 ulx, uly, lrx, lry;
    f32 w, h;
    s32 dsdx, dtdy;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4228);
    gDPPipeSync(OVERLAY_DISP++);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, r, g, b, a);

    w = 8.0f * z;
    h = 12.0f * z;
    unk = (1.0f / z) * 1024;
    dsdx = unk * dx;
    dtdy = dy * unk;

    ulx = (x - w) * 4.0f;
    uly = (y - h) * 4.0f;
    lrx = (x + w) * 4.0f;
    lry = (y + h) * 4.0f;
    gSPTextureRectangle(OVERLAY_DISP++, ulx, uly, lrx, lry, G_TX_RENDERTILE, s, t, dsdx, dtdy);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4242);
}

void EnOssan_DrawStickDirectionPrompts(GlobalContext* globalCtx, EnOssan* self) {
    s32 drawStickLeftPrompt = self->stickLeftPrompt.isEnabled;
    s32 drawStickRightPrompt = self->stickRightPrompt.isEnabled;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4252);
    if (drawStickLeftPrompt || drawStickRightPrompt) {
        func_80094520(globalCtx->state.gfxCtx);
        gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPLoadTextureBlock(OVERLAY_DISP++, gArrowCursorTex, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 24, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        if (drawStickLeftPrompt) {
            EnOssan_DrawTextRec(globalCtx, self->stickLeftPrompt.arrowColorR, self->stickLeftPrompt.arrowColorG,
                                self->stickLeftPrompt.arrowColorB, self->stickLeftPrompt.arrowColorA,
                                self->stickLeftPrompt.arrowTexX, self->stickLeftPrompt.arrowTexY,
                                self->stickLeftPrompt.z, 0, 0, -1.0f, 1.0f);
        }
        if (drawStickRightPrompt) {
            EnOssan_DrawTextRec(globalCtx, self->stickRightPrompt.arrowColorR, self->stickRightPrompt.arrowColorG,
                                self->stickRightPrompt.arrowColorB, self->stickRightPrompt.arrowColorA,
                                self->stickRightPrompt.arrowTexX, self->stickRightPrompt.arrowTexY,
                                self->stickRightPrompt.z, 0, 0, 1.0f, 1.0f);
        }
        gDPLoadTextureBlock(OVERLAY_DISP++, gControlStickTex, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 16, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 4, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        if (drawStickLeftPrompt) {
            EnOssan_DrawTextRec(globalCtx, self->stickLeftPrompt.stickColorR, self->stickLeftPrompt.stickColorG,
                                self->stickLeftPrompt.stickColorB, self->stickLeftPrompt.stickColorA,
                                self->stickLeftPrompt.stickTexX, self->stickLeftPrompt.stickTexY,
                                self->stickLeftPrompt.z, 0, 0, -1.0f, 1.0f);
        }
        if (drawStickRightPrompt) {
            EnOssan_DrawTextRec(globalCtx, self->stickRightPrompt.stickColorR, self->stickRightPrompt.stickColorG,
                                self->stickRightPrompt.stickColorB, self->stickRightPrompt.stickColorA,
                                self->stickRightPrompt.stickTexX, self->stickRightPrompt.stickTexY,
                                self->stickRightPrompt.z, 0, 0, 1.0f, 1.0f);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4300);
}

void EnOssan_DrawBazaarShopkeeper(Actor* thisx, GlobalContext* globalCtx) {
    static void* sBazaarShopkeeperEyeTextures[] = { gOssanEyeOpenTex, gOssanEyeHalfTex, gOssanEyeClosedTex };
    EnOssan* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4320);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sBazaarShopkeeperEyeTextures[self->eyeTextureIdx]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnOssan_OverrideLimbDrawDefaultShopkeeper, NULL, self);
    EnOssan_DrawCursor(globalCtx, self, self->cursorX, self->cursorY, self->cursorZ, self->drawCursor);
    EnOssan_DrawStickDirectionPrompts(globalCtx, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4340);
}

s32 EnOssan_OverrideLimbDrawKokiriShopkeeper(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos,
                                             Vec3s* rot, void* thisx) {
    static void* sKokiriShopkeeperEyeTextures[] = { 0x06001570, 0x060001F0, 0x06000B30 };
    EnOssan* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4354);

    if (limbIndex == 15) {
        gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->objBankIndex2].segment);
        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->objBankIndex2].segment);
        *dList = gKokiriShopkeeperHeadDL;
        gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(sKokiriShopkeeperEyeTextures[self->eyeTextureIdx]));
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4374);

    return 0;
}

Gfx* EnOssan_EndDList(GraphicsContext* gfxCtx) {
    Gfx* disp = Graph_Alloc(gfxCtx, sizeof(Gfx));

    gSPEndDisplayList(disp);
    return disp;
}

Gfx* EnOssan_SetEnvColor(GraphicsContext* gfxCtx, u8 r, u8 g, u8 b, u8 a) {
    Gfx* disp = Graph_Alloc(gfxCtx, sizeof(Gfx) * 2);

    gDPSetEnvColor(disp, r, g, b, a);
    gSPEndDisplayList(disp + 1);
    return disp;
}

void EnOssan_DrawKokiriShopkeeper(Actor* thisx, GlobalContext* globalCtx) {
    EnOssan* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4409);

    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x08, EnOssan_SetEnvColor(globalCtx->state.gfxCtx, 0, 130, 70, 255));
    gSPSegment(POLY_OPA_DISP++, 0x09, EnOssan_SetEnvColor(globalCtx->state.gfxCtx, 110, 170, 20, 255));
    gSPSegment(POLY_OPA_DISP++, 0x0C, EnOssan_EndDList(globalCtx->state.gfxCtx));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnOssan_OverrideLimbDrawKokiriShopkeeper, NULL, self);
    EnOssan_DrawCursor(globalCtx, self, self->cursorX, self->cursorY, self->cursorZ, self->drawCursor);
    EnOssan_DrawStickDirectionPrompts(globalCtx, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4434);
}

void EnOssan_DrawGoronShopkeeper(Actor* thisx, GlobalContext* globalCtx) {
    static void* sGoronShopkeeperEyeTextures[] = { gGoronCsEyeOpenTex, gGoronCsEyeHalfTex, gGoronCsEyeClosedTex };
    EnOssan* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4455);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sGoronShopkeeperEyeTextures[self->eyeTextureIdx]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(gGoronCsMouthNeutralTex));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);
    EnOssan_DrawCursor(globalCtx, self, self->cursorX, self->cursorY, self->cursorZ, self->drawCursor);
    EnOssan_DrawStickDirectionPrompts(globalCtx, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4476);
}

s32 EnOssan_OverrideLimbDrawZoraShopkeeper(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                           void* thisx) {
    EnOssan* self = THIS;

    if (limbIndex == 15) {
        rot->x += self->headRot;
    }
    return 0;
}

void EnOssan_DrawZoraShopkeeper(Actor* thisx, GlobalContext* globalCtx) {
    static void* sZoraShopkeeperEyeTextures[] = { gZoraEyeOpenTex, gZoraEyeHalfTex, gZoraEyeClosedTex };
    EnOssan* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4506);

    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, EnOssan_EndDList(globalCtx->state.gfxCtx));
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sZoraShopkeeperEyeTextures[self->eyeTextureIdx]));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnOssan_OverrideLimbDrawZoraShopkeeper, NULL, self);
    EnOssan_DrawCursor(globalCtx, self, self->cursorX, self->cursorY, self->cursorZ, self->drawCursor);
    EnOssan_DrawStickDirectionPrompts(globalCtx, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4531);
}

void EnOssan_DrawPotionShopkeeper(Actor* thisx, GlobalContext* globalCtx) {
    static void* sPotionShopkeeperEyeTextures[] = { gPotionShopkeeperEyeOpenTex, gPotionShopkeeperEyeHalfTex,
                                                    gPotionShopkeeperEyeClosedTex };
    EnOssan* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4544);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sPotionShopkeeperEyeTextures[self->eyeTextureIdx]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);
    EnOssan_DrawCursor(globalCtx, self, self->cursorX, self->cursorY, self->cursorZ, self->drawCursor);
    EnOssan_DrawStickDirectionPrompts(globalCtx, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4564);
}

void EnOssan_DrawHappyMaskShopkeeper(Actor* thisx, GlobalContext* globalCtx) {
    static void* sHappyMaskShopkeeperEyeTextures[] = { gOsEyeClosedTex, gOsEyeOpenTex };
    EnOssan* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4578);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08,
               SEGMENTED_TO_VIRTUAL(sHappyMaskShopkeeperEyeTextures[self->happyMaskShopkeeperEyeIdx]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);
    EnOssan_DrawCursor(globalCtx, self, self->cursorX, self->cursorY, self->cursorZ, self->drawCursor);
    EnOssan_DrawStickDirectionPrompts(globalCtx, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4598);
}

void EnOssan_DrawBombchuShopkeeper(Actor* thisx, GlobalContext* globalCtx) {
    static void* sBombchuShopkeeperEyeTextures[] = { gBombchuShopkeeperEyeOpenTex, gBombchuShopkeeperEyeHalfTex,
                                                     gBombchuShopkeeperEyeClosedTex };
    EnOssan* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4611);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sBombchuShopkeeperEyeTextures[self->eyeTextureIdx]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);
    EnOssan_DrawCursor(globalCtx, self, self->cursorX, self->cursorY, self->cursorZ, self->drawCursor);
    EnOssan_DrawStickDirectionPrompts(globalCtx, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_oB1.c", 4631);
}
