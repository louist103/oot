/*
 * File: z_en_dns.c
 * Overlay: En_Dns
 * Description: Deku Salesman
 */

#include "z_en_dns.h"
#include "objects/object_shopnuts/object_shopnuts.h"
#include "vt.h"

#define FLAGS 0x00000009

#define THIS ((EnDns*)thisx)

void EnDns_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDns_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDns_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDns_Draw(Actor* thisx, GlobalContext* globalCtx);

u32 func_809EF5A4(EnDns* self);
u32 func_809EF658(EnDns* self);
u32 func_809EF70C(EnDns* self);
u32 func_809EF73C(EnDns* self);
u32 func_809EF800(EnDns* self);
u32 func_809EF854(EnDns* self);
u32 func_809EF8F4(EnDns* self);
u32 func_809EF9A4(EnDns* self);

void func_809EF9F8(EnDns* self);
void func_809EFA28(EnDns* self);
void func_809EFA58(EnDns* self);
void func_809EFA9C(EnDns* self);
void func_809EFACC(EnDns* self);
void func_809EFAFC(EnDns* self);
void func_809EFB40(EnDns* self);

void EnDns_SetupWait(EnDns* self, GlobalContext* globalCtx);
void EnDns_Wait(EnDns* self, GlobalContext* globalCtx);
void EnDns_Talk(EnDns* self, GlobalContext* globalCtx);
void func_809EFDD0(EnDns* self, GlobalContext* globalCtx);
void func_809EFEE8(EnDns* self, GlobalContext* globalCtx);
void func_809EFF50(EnDns* self, GlobalContext* globalCtx);
void func_809EFF98(EnDns* self, GlobalContext* globalCtx);
void func_809F008C(EnDns* self, GlobalContext* globalCtx);
void EnDns_SetupBurrow(EnDns* self, GlobalContext* globalCtx);
void EnDns_Burrow(EnDns* self, GlobalContext* globalCtx);

const ActorInit En_Dns_InitVars = {
    ACTOR_EN_DNS,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SHOPNUTS,
    sizeof(EnDns),
    (ActorFunc)EnDns_Init,
    (ActorFunc)EnDns_Destroy,
    (ActorFunc)EnDns_Update,
    (ActorFunc)EnDns_Draw,
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 18, 32, 0, { 0, 0, 0 } },
};

static u16 D_809F040C[] = {
    0x10A0, 0x10A1, 0x10A2, 0x10CA, 0x10CB, 0x10CC, 0x10CD, 0x10CE, 0x10CF, 0x10DC, 0x10DD,
};

// Debug text: "sells"  { "Deku Nuts",    "Deku Sticks",        "Piece of Heart",  "Deku Seeds",
//                        "Deku Shield",  "Bombs",              "Arrows",          "Red Potion",
//                        "Green Potion", "Deku Stick Upgrade", "Deku Nut Upgrade" }
static char* D_809F0424[] = {
    "デクの実売り            ", "デクの棒売り            ", "ハートの欠片売り        ", "デクの種売り            ",
    "デクの盾売り            ", "バクダン売り            ", "矢売り                  ", "赤のくすり売り          ",
    "緑のくすり売り          ", "デクの棒持てる数を増やす", "デクの実持てる数を増やす",
};

static DnsItemEntry D_809F0450 = { 20, 5, GI_NUTS_5_2, func_809EF5A4, func_809EFA28 };

static DnsItemEntry D_809F0460 = { 15, 1, GI_STICKS_1, func_809EF658, func_809EF9F8 };

static DnsItemEntry D_809F0470 = { 10, 1, GI_HEART_PIECE, func_809EF70C, func_809EFA58 };

static DnsItemEntry D_809F0480 = { 40, 30, GI_SEEDS_30, func_809EF73C, func_809EF9F8 };

static DnsItemEntry D_809F0490 = { 50, 1, GI_SHIELD_DEKU, func_809EF800, func_809EF9F8 };

static DnsItemEntry D_809F04A0 = { 40, 5, GI_BOMBS_5, func_809EF854, func_809EFA9C };

static DnsItemEntry D_809F04B0 = { 70, 20, GI_ARROWS_LARGE, func_809EF8F4, func_809EFACC };

static DnsItemEntry D_809F04C0 = { 40, 1, GI_POTION_RED, func_809EF9A4, func_809EF9F8 };

static DnsItemEntry D_809F04D0 = { 40, 1, GI_POTION_GREEN, func_809EF9A4, func_809EF9F8 };

static DnsItemEntry D_809F04E0 = { 40, 1, GI_STICK_UPGRADE_20, func_809EF70C, func_809EFAFC };

static DnsItemEntry D_809F04F0 = { 40, 1, GI_NUT_UPGRADE_30, func_809EF70C, func_809EFB40 };

static DnsItemEntry* sItemEntries[] = {
    &D_809F0450, &D_809F0460, &D_809F0470, &D_809F0480, &D_809F0490, &D_809F04A0,
    &D_809F04B0, &D_809F04C0, &D_809F04D0, &D_809F04E0, &D_809F04F0,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x4E, ICHAIN_CONTINUE),
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

typedef struct {
    /* 0x00 */ AnimationHeader* anim;
    /* 0x04 */ u8 mode;
    /* 0x08 */ f32 transitionRate;
} DnsAnimInfo; // size = 0xC

static DnsAnimInfo sAnimInfo[] = {
    { &gBusinessScrubNervousIdleAnim, ANIMMODE_LOOP, 0.0f },
    { &gBusinessScrubAnim_4404, ANIMMODE_ONCE, 0.0f },
    { &gBusinessScrubNervousTransitionAnim, ANIMMODE_ONCE, 0.0f },
};

void EnDns_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDns* self = THIS;

    if (self->actor.params < 0) {
        // "Function Error (Deku Salesman)"
        osSyncPrintf(VT_FGCOL(RED) "引数エラー（売りナッツ）[ arg_data = %d ]" VT_RST "\n", self->actor.params);
        Actor_Kill(&self->actor);
        return;
    }
    // Sell Seeds instead of Arrows if Link is child
    if ((self->actor.params == 0x0006) && (LINK_AGE_IN_YEARS == YEARS_CHILD)) {
        self->actor.params = 0x0003;
    }
    // "Deku Salesman"
    osSyncPrintf(VT_FGCOL(GREEN) "◆◆◆ 売りナッツ『%s』 ◆◆◆" VT_RST "\n", D_809F0424[self->actor.params],
                 self->actor.params);
    Actor_ProcessInitChain(&self->actor, sInitChain);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gBusinessScrubSkel, &gBusinessScrubNervousTransitionAnim,
                       self->jointTable, self->morphTable, 18);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    ActorShape_Init(&self->actor.shape, 0.0f, &ActorShadow_DrawCircle, 35.0f);
    self->actor.textId = D_809F040C[self->actor.params];
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.colChkInfo.mass = MASS_IMMOVABLE;
    self->maintainCollider = 1;
    self->standOnGround = 1;
    self->dropCollectible = 0;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = -1.0f;
    self->dnsItemEntry = sItemEntries[self->actor.params];
    self->actionFunc = EnDns_SetupWait;
}

void EnDns_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDns* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnDns_Change(EnDns* self, u8 arg1) {
    s16 frameCount;

    frameCount = Animation_GetLastFrame(sAnimInfo[arg1].anim);
    self->unk_2BA = arg1; // Not used anywhere else?
    Animation_Change(&self->skelAnime, sAnimInfo[arg1].anim, 1.0f, 0.0f, (f32)frameCount, sAnimInfo[arg1].mode,
                     sAnimInfo[arg1].transitionRate);
}

/* Item give checking functions */

u32 func_809EF5A4(EnDns* self) {
    if ((CUR_CAPACITY(UPG_NUTS) != 0) && (AMMO(ITEM_NUT) >= CUR_CAPACITY(UPG_NUTS))) {
        return 1;
    }
    if (gSaveContext.rupees < self->dnsItemEntry->itemPrice) {
        return 0;
    }
    if (Item_CheckObtainability(ITEM_NUT) == ITEM_NONE) {
        return 2;
    }
    return 4;
}

u32 func_809EF658(EnDns* self) {
    if ((CUR_CAPACITY(UPG_STICKS) != 0) && (AMMO(ITEM_STICK) >= CUR_CAPACITY(UPG_STICKS))) {
        return 1;
    }
    if (gSaveContext.rupees < self->dnsItemEntry->itemPrice) {
        return 0;
    }
    if (Item_CheckObtainability(ITEM_STICK) == ITEM_NONE) {
        return 2;
    }
    return 4;
}

u32 func_809EF70C(EnDns* self) {
    if (gSaveContext.rupees < self->dnsItemEntry->itemPrice) {
        return 0;
    }
    return 4;
}

u32 func_809EF73C(EnDns* self) {
    if (INV_CONTENT(ITEM_SLINGSHOT) == ITEM_NONE) {
        return 3;
    }
    if (AMMO(ITEM_SLINGSHOT) >= CUR_CAPACITY(UPG_BULLET_BAG)) {
        return 1;
    }
    if (gSaveContext.rupees < self->dnsItemEntry->itemPrice) {
        return 0;
    }
    if (Item_CheckObtainability(ITEM_SEEDS) == ITEM_NONE) {
        return 2;
    }
    return 4;
}

u32 func_809EF800(EnDns* self) {
    if (gBitFlags[4] & gSaveContext.inventory.equipment) {
        return 1;
    }
    if (gSaveContext.rupees < self->dnsItemEntry->itemPrice) {
        return 0;
    }
    return 4;
}

u32 func_809EF854(EnDns* self) {
    if (!CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
        return 3;
    }
    if (AMMO(ITEM_BOMB) >= CUR_CAPACITY(UPG_BOMB_BAG)) {
        return 1;
    }
    if (gSaveContext.rupees < self->dnsItemEntry->itemPrice) {
        return 0;
    }
    return 4;
}

u32 func_809EF8F4(EnDns* self) {
    if (Item_CheckObtainability(ITEM_BOW) == ITEM_NONE) {
        return 3;
    }
    if (AMMO(ITEM_BOW) >= CUR_CAPACITY(UPG_QUIVER)) {
        return 1;
    }
    if (gSaveContext.rupees < self->dnsItemEntry->itemPrice) {
        return 0;
    }
    return 4;
}

u32 func_809EF9A4(EnDns* self) {
    if (!Inventory_HasEmptyBottle()) {
        return 1;
    }
    if (gSaveContext.rupees < self->dnsItemEntry->itemPrice) {
        return 0;
    }
    return 4;
}

/* Paying and flagging functions */

void func_809EF9F8(EnDns* self) {
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void func_809EFA28(EnDns* self) {
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void func_809EFA58(EnDns* self) {
    gSaveContext.itemGetInf[0] |= 0x800;
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void func_809EFA9C(EnDns* self) {
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void func_809EFACC(EnDns* self) {
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void func_809EFAFC(EnDns* self) {
    gSaveContext.infTable[25] |= 0x4;
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void func_809EFB40(EnDns* self) {
    gSaveContext.infTable[25] |= 0x8;
    Rupees_ChangeBy(-self->dnsItemEntry->itemPrice);
}

void EnDns_SetupWait(EnDns* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame == self->skelAnime.endFrame) {
        self->actionFunc = EnDns_Wait;
        EnDns_Change(self, 0);
    }
}

void EnDns_Wait(EnDns* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 2000, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = EnDns_Talk;
    } else {
        if ((self->collider.base.ocFlags1 & OC1_HIT) || self->actor.isTargeted) {
            self->actor.flags |= 0x10000;
        } else {
            self->actor.flags &= ~0x10000;
        }
        if (self->actor.xzDistToPlayer < 130.0f) {
            func_8002F2F4(&self->actor, globalCtx);
        }
    }
}

void EnDns_Talk(EnDns* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx))) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // OK
                switch (self->dnsItemEntry->purchaseableCheck(self)) {
                    case 0:
                        func_8010B720(globalCtx, 0x10A5);
                        self->actionFunc = func_809F008C;
                        break;
                    case 1:
                        func_8010B720(globalCtx, 0x10A6);
                        self->actionFunc = func_809F008C;
                        break;
                    case 3:
                        func_8010B720(globalCtx, 0x10DE);
                        self->actionFunc = func_809F008C;
                        break;
                    case 2:
                    case 4:
                        func_8010B720(globalCtx, 0x10A7);
                        self->actionFunc = func_809EFEE8;
                        break;
                }
                break;
            case 1: // No way
                func_8010B720(globalCtx, 0x10A4);
                self->actionFunc = func_809F008C;
        }
    }
}

void func_809EFDD0(EnDns* self, GlobalContext* globalCtx) {
    if (self->actor.params == 0x9) {
        if (CUR_UPG_VALUE(UPG_STICKS) < 2) {
            func_8002F434(&self->actor, globalCtx, GI_STICK_UPGRADE_20, 130.0f, 100.0f);
        } else {
            func_8002F434(&self->actor, globalCtx, GI_STICK_UPGRADE_30, 130.0f, 100.0f);
        }
    } else if (self->actor.params == 0xA) {
        if (CUR_UPG_VALUE(UPG_NUTS) < 2) {
            func_8002F434(&self->actor, globalCtx, GI_NUT_UPGRADE_30, 130.0f, 100.0f);
        } else {
            func_8002F434(&self->actor, globalCtx, GI_NUT_UPGRADE_40, 130.0f, 100.0f);
        }
    } else {
        func_8002F434(&self->actor, globalCtx, self->dnsItemEntry->getItemId, 130.0f, 100.0f);
    }
}

void func_809EFEE8(EnDns* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        func_809EFDD0(self, globalCtx);
        self->actionFunc = func_809EFF50;
    }
}

void func_809EFF50(EnDns* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = func_809EFF98;
    } else {
        func_809EFDD0(self, globalCtx);
    }
}

void func_809EFF98(EnDns* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (player->stateFlags1 & 0x400) {
        if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
            self->dnsItemEntry->setRupeesAndFlags(self);
            self->dropCollectible = 1;
            self->maintainCollider = 0;
            self->actor.flags &= ~1;
            EnDns_Change(self, 1);
            self->actionFunc = EnDns_SetupBurrow;
        }
    } else {
        self->dnsItemEntry->setRupeesAndFlags(self);
        self->dropCollectible = 1;
        self->maintainCollider = 0;
        self->actor.flags &= ~1;
        EnDns_Change(self, 1);
        self->actionFunc = EnDns_SetupBurrow;
    }
}

void func_809F008C(EnDns* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
        self->maintainCollider = 0;
        self->actor.flags &= ~1;
        EnDns_Change(self, 1);
        self->actionFunc = EnDns_SetupBurrow;
    }
}

void EnDns_SetupBurrow(EnDns* self, GlobalContext* globalCtx) {
    f32 frameCount = Animation_GetLastFrame(&gBusinessScrubAnim_4404);

    if (self->skelAnime.curFrame == frameCount) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_AKINDONUTS_HIDE);
        self->actionFunc = EnDns_Burrow;
        self->standOnGround = 0;
        self->yInitPos = self->actor.world.pos.y;
    }
}

void EnDns_Burrow(EnDns* self, GlobalContext* globalCtx) {
    f32 depth;
    Vec3f initPos;
    s32 i;

    depth = self->yInitPos - self->actor.world.pos.y;
    if ((self->dustTimer & 3) == 0) {
        initPos.x = self->actor.world.pos.x;
        initPos.y = self->yInitPos;
        initPos.z = self->actor.world.pos.z;
        func_80028990(globalCtx, 20.0f, &initPos);
    }
    self->actor.shape.rot.y += 0x2000;
    // Drops only if you bought its item
    if (depth > 400.0f) {
        if (self->dropCollectible) {
            initPos.x = self->actor.world.pos.x;
            initPos.y = self->yInitPos;
            initPos.z = self->actor.world.pos.z;
            for (i = 0; i < 3; i++) {
                Item_DropCollectible(globalCtx, &initPos, ITEM00_HEART);
            }
        }
        Actor_Kill(&self->actor);
    }
}

void EnDns_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDns* self = THIS;
    s16 pad;

    self->dustTimer++;
    self->actor.textId = D_809F040C[self->actor.params];
    Actor_SetFocus(&self->actor, 60.0f);
    Actor_SetScale(&self->actor, 0.01f);
    SkelAnime_Update(&self->skelAnime);
    Actor_MoveForward(&self->actor);
    self->actionFunc(self, globalCtx);
    if (self->standOnGround) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 20.0f, 4);
    }
    if (self->maintainCollider) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void EnDns_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDns* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, &self->actor);
}
