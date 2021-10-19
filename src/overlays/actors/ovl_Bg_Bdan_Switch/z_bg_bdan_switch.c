/*
 * File: z_bg_bdan_switch.c
 * Overlay: ovl_Bg_Bdan_Switch
 * Description: Switches Inside Lord Jabu-Jabu
 */

#include "z_bg_bdan_switch.h"
#include "objects/object_bdan_objects/object_bdan_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgBdanSwitch*)thisx)

void BgBdanSwitch_Init(Actor* thisx, GlobalContext* globalCtx);
void BgBdanSwitch_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgBdanSwitch_Update(Actor* thisx, GlobalContext* globalCtx);
void BgBdanSwitch_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8086D5C4(BgBdanSwitch* self);
void func_8086D5E0(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086D67C(BgBdanSwitch* self);
void func_8086D694(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086D730(BgBdanSwitch* self);
void func_8086D754(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086D7FC(BgBdanSwitch* self);
void func_8086D80C(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086D86C(BgBdanSwitch* self);
void func_8086D888(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086D8BC(BgBdanSwitch* self);
void func_8086D8CC(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086D95C(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086D9F8(BgBdanSwitch* self);
void func_8086DA1C(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086DAB4(BgBdanSwitch* self);
void func_8086DAC4(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086DB24(BgBdanSwitch* self);
void func_8086DB40(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086DB4C(BgBdanSwitch* self);
void func_8086DB68(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086DC30(BgBdanSwitch* self);
void func_8086DC48(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086DCCC(BgBdanSwitch* self);
void func_8086DCE8(BgBdanSwitch* self, GlobalContext* globalCtx);
void func_8086DDA8(BgBdanSwitch* self);
void func_8086DDC0(BgBdanSwitch* self, GlobalContext* globalCtx);

const ActorInit Bg_Bdan_Switch_InitVars = {
    ACTOR_BG_BDAN_SWITCH,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_BDAN_OBJECTS,
    sizeof(BgBdanSwitch),
    (ActorFunc)BgBdanSwitch_Init,
    (ActorFunc)BgBdanSwitch_Destroy,
    (ActorFunc)BgBdanSwitch_Update,
    (ActorFunc)BgBdanSwitch_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xEFC1FFFE, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 120, 0 }, 370 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 1400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1200, ICHAIN_STOP),
};

static Vec3f D_8086E0E0 = { 0.0f, 140.0f, 0.0f };

void BgBdanSwitch_InitDynaPoly(BgBdanSwitch* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 flag) {
    s16 pad1;
    CollisionHeader* colHeader = NULL;
    s16 pad2;

    DynaPolyActor_Init(&self->dyna, flag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    if (self->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_bg_bdan_switch.c", 325,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void BgBdanSwitch_InitCollision(BgBdanSwitch* self, GlobalContext* globalCtx) {
    Actor* actor = &self->dyna.actor;
    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, actor, &sJntSphInit, self->colliderItems);
}

void func_8086D0EC(BgBdanSwitch* self) {
    if (self->unk_1CC > 0) {
        self->unk_1CC += 0x5DC;
    } else {
        self->unk_1CC += 0xFA0;
    }

    switch (self->dyna.actor.params & 0xFF) {
        case BLUE:
        case YELLOW_HEAVY:
        case YELLOW:
            self->unk_1D4 = ((Math_CosS(self->unk_1CC) * 0.5f) + (53.000004f / 6.0f)) * 0.012f;
            self->unk_1D0 = ((Math_CosS(self->unk_1CC) * 0.5f) + 20.5f) * (self->unk_1C8 * 0.0050000004f);
            self->dyna.actor.scale.y = self->unk_1C8 * 0.1f;
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            self->unk_1D4 = ((Math_CosS(self->unk_1CC) * 0.5f) + (43.0f / 6.0f)) * 0.0075000003f;
            self->unk_1D0 = ((Math_CosS(self->unk_1CC) * 0.5f) + 20.5f) * (self->unk_1C8 * 0.0050000004f);
            self->dyna.actor.scale.y = self->unk_1C8 * 0.1f;
    }
    self->dyna.actor.shape.yOffset = 1.2f / self->unk_1D0;
}

void BgBdanSwitch_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgBdanSwitch* self = THIS;
    s32 pad;
    s16 type;
    s32 flag;

    type = self->dyna.actor.params & 0xFF;
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    if (type == YELLOW_TALL_1 || type == YELLOW_TALL_2) {
        self->dyna.actor.scale.z = 0.05f;
        self->dyna.actor.scale.x = 0.05f;
    } else {
        self->dyna.actor.scale.z = 0.1f;
        self->dyna.actor.scale.x = 0.1f;
    }
    self->dyna.actor.scale.y = 0.0f;
    Actor_SetFocus(&self->dyna.actor, 10.0f);

    switch (type) {
        case BLUE:
        case YELLOW_HEAVY:
        case YELLOW:
            BgBdanSwitch_InitDynaPoly(self, globalCtx, &gJabuFloorSwitchCol, DPM_PLAYER);
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            BgBdanSwitch_InitCollision(self, globalCtx);
            self->dyna.actor.flags |= 1;
            self->dyna.actor.targetMode = 4;
            break;
    }

    flag = Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);

    switch (type) {
        case BLUE:
        case YELLOW:
            if (flag) {
                func_8086D730(self);
            } else {
                func_8086D5C4(self);
            }
            break;
        case YELLOW_HEAVY:
            if (flag) {
                func_8086DB24(self);
            } else {
                func_8086D86C(self);
            }
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            if (flag) {
                func_8086DCCC(self);
            } else {
                func_8086DB4C(self);
            }
            break;
        default:
            osSyncPrintf("不正な ARG_DATA(arg_data 0x%04x)(%s %d)\n", self->dyna.actor.params, "../z_bg_bdan_switch.c",
                         454);
            Actor_Kill(&self->dyna.actor);
            return;
    }
    osSyncPrintf("(巨大魚ダンジョン 専用スイッチ)(arg_data 0x%04x)\n", self->dyna.actor.params);
}

void BgBdanSwitch_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgBdanSwitch* self = THIS;

    switch (self->dyna.actor.params & 0xFF) {
        case BLUE:
        case YELLOW_HEAVY:
        case YELLOW:
            DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            Collider_DestroyJntSph(globalCtx, &self->collider);
            break;
    }
}

void func_8086D4B4(BgBdanSwitch* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 type;

    if (!Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
        type = self->dyna.actor.params & 0xFF;
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);
        if (type == BLUE || type == YELLOW_TALL_2) {
            OnePointCutscene_AttentionSetSfx(globalCtx, &self->dyna.actor, NA_SE_SY_TRE_BOX_APPEAR);
        } else {
            OnePointCutscene_AttentionSetSfx(globalCtx, &self->dyna.actor, NA_SE_SY_CORRECT_CHIME);
        }
    }
}

void func_8086D548(BgBdanSwitch* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
        Flags_UnsetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F);
        if ((self->dyna.actor.params & 0xFF) == YELLOW_TALL_2) {
            OnePointCutscene_AttentionSetSfx(globalCtx, &self->dyna.actor, NA_SE_SY_TRE_BOX_APPEAR);
        }
    }
}

void func_8086D5C4(BgBdanSwitch* self) {
    self->actionFunc = func_8086D5E0;
    self->unk_1C8 = 1.0f;
}

void func_8086D5E0(BgBdanSwitch* self, GlobalContext* globalCtx) {
    switch (self->dyna.actor.params & 0xFF) {
        case BLUE:
            if (func_800435B4(&self->dyna)) {
                func_8086D67C(self);
                func_8086D4B4(self, globalCtx);
            }
            break;
        case YELLOW:
            if (func_8004356C(&self->dyna)) {
                func_8086D67C(self);
                func_8086D4B4(self, globalCtx);
            }
            break;
    }
}

void func_8086D67C(BgBdanSwitch* self) {
    self->actionFunc = func_8086D694;
    self->unk_1DA = 0x64;
}

void func_8086D694(BgBdanSwitch* self, GlobalContext* globalCtx) {
    if ((func_8005B198() == self->dyna.actor.category) || (self->unk_1DA <= 0)) {
        self->unk_1C8 -= 0.2f;
        if (self->unk_1C8 <= 0.1f) {
            func_8086D730(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
            func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 0x78, 0x14, 0xA);
        }
    }
}

void func_8086D730(BgBdanSwitch* self) {
    self->unk_1C8 = 0.1f;
    self->actionFunc = func_8086D754;
    self->unk_1D8 = 6;
}

void func_8086D754(BgBdanSwitch* self, GlobalContext* globalCtx) {
    switch (self->dyna.actor.params & 0xFF) {
        case BLUE:
            if (!func_800435B4(&self->dyna)) {
                if (self->unk_1D8 <= 0) {
                    func_8086D7FC(self);
                    func_8086D548(self, globalCtx);
                }
            } else {
                self->unk_1D8 = 6;
            }
            break;
        case YELLOW:
            if (!Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
                func_8086D7FC(self);
            }
            break;
    }
}

void func_8086D7FC(BgBdanSwitch* self) {
    self->actionFunc = func_8086D80C;
}

void func_8086D80C(BgBdanSwitch* self, GlobalContext* globalCtx) {
    self->unk_1C8 += 0.2f;
    if (self->unk_1C8 >= 1.0f) {
        func_8086D5C4(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
    }
}

void func_8086D86C(BgBdanSwitch* self) {
    self->actionFunc = func_8086D888;
    self->unk_1C8 = 1.0f;
}

void func_8086D888(BgBdanSwitch* self, GlobalContext* globalCtx) {
    if (func_8004356C(&self->dyna)) {
        func_8086D8BC(self);
    }
}

void func_8086D8BC(BgBdanSwitch* self) {
    self->actionFunc = func_8086D8CC;
}

void func_8086D8CC(BgBdanSwitch* self, GlobalContext* globalCtx) {
    self->unk_1C8 -= 0.2f;
    if (self->unk_1C8 <= 0.6f) {
        func_8086D9F8(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
        func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 0x78, 0x14, 0xA);
    }
}

void func_8086D944(BgBdanSwitch* self) {
    self->actionFunc = func_8086D95C;
    self->unk_1DA = 0x64;
}

void func_8086D95C(BgBdanSwitch* self, GlobalContext* globalCtx) {
    if ((func_8005B198() == self->dyna.actor.category) || (self->unk_1DA <= 0)) {
        self->unk_1C8 -= 0.2f;
        if (self->unk_1C8 <= 0.1f) {
            func_8086DB24(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
            func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 0x78, 0x14, 0xA);
        }
    }
}

void func_8086D9F8(BgBdanSwitch* self) {
    self->unk_1C8 = 0.6f;
    self->actionFunc = func_8086DA1C;
    self->unk_1D8 = 6;
}

void func_8086DA1C(BgBdanSwitch* self, GlobalContext* globalCtx) {
    Actor* heldActor = GET_PLAYER(globalCtx)->heldActor;

    if (func_8004356C(&self->dyna)) {
        if (heldActor != NULL && heldActor->id == ACTOR_EN_RU1) {
            if (self->unk_1D8 <= 0) {
                func_8086D944(self);
                func_8086D4B4(self, globalCtx);
            }
        } else {
            self->unk_1D8 = 6;
        }
    } else {
        if (self->unk_1D8 <= 0) {
            func_8086DAB4(self);
        }
    }
}

void func_8086DAB4(BgBdanSwitch* self) {
    self->actionFunc = func_8086DAC4;
}

void func_8086DAC4(BgBdanSwitch* self, GlobalContext* globalCtx) {
    self->unk_1C8 += 0.2f;
    if (self->unk_1C8 >= 1.0f) {
        func_8086D86C(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
    }
}

void func_8086DB24(BgBdanSwitch* self) {
    self->unk_1C8 = 0.1f;
    self->actionFunc = func_8086DB40;
}

void func_8086DB40(BgBdanSwitch* self, GlobalContext* globalCtx) {
}

void func_8086DB4C(BgBdanSwitch* self) {
    self->actionFunc = func_8086DB68;
    self->unk_1C8 = 2.0f;
}

void func_8086DB68(BgBdanSwitch* self, GlobalContext* globalCtx) {
    switch (self->dyna.actor.params & 0xFF) {
        default:
            return;
        case YELLOW_TALL_1:
            if (((self->collider.base.acFlags & AC_HIT) != 0) && self->unk_1D8 <= 0) {
                self->unk_1D8 = 0xA;
                func_8086DC30(self);
                func_8086D4B4(self, globalCtx);
            }
            break;
        case YELLOW_TALL_2:
            if (((self->collider.base.acFlags & AC_HIT) != 0) && ((self->unk_1DC & 2) == 0) && self->unk_1D8 <= 0) {
                self->unk_1D8 = 0xA;
                func_8086DC30(self);
                func_8086D4B4(self, globalCtx);
            }
            break;
    }
}

void func_8086DC30(BgBdanSwitch* self) {
    self->actionFunc = func_8086DC48;
    self->unk_1DA = 0x64;
}

void func_8086DC48(BgBdanSwitch* self, GlobalContext* globalCtx) {
    if ((func_8005B198() == self->dyna.actor.category) || (self->unk_1DA <= 0)) {
        self->unk_1C8 -= 0.3f;
        if (self->unk_1C8 <= 1.0f) {
            func_8086DCCC(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
        }
    }
}

void func_8086DCCC(BgBdanSwitch* self) {
    self->actionFunc = func_8086DCE8;
    self->unk_1C8 = 1.0f;
}

void func_8086DCE8(BgBdanSwitch* self, GlobalContext* globalCtx) {
    switch (self->dyna.actor.params & 0xFF) {
        case YELLOW_TALL_1:
            if (!Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
                func_8086DDA8(self);
            }
            break;
        case YELLOW_TALL_2:
            if (((self->collider.base.acFlags & AC_HIT) != 0) && ((self->unk_1DC & 2) == 0) && (self->unk_1D8 <= 0)) {
                self->unk_1D8 = 0xA;
                func_8086DDA8(self);
                func_8086D548(self, globalCtx);
            }
            break;
    }
}

void func_8086DDA8(BgBdanSwitch* self) {
    self->actionFunc = func_8086DDC0;
    self->unk_1DA = 0x64;
}

void func_8086DDC0(BgBdanSwitch* self, GlobalContext* globalCtx) {
    if ((((self->dyna.actor.params & 0xFF) != YELLOW_TALL_2) || (func_8005B198() == self->dyna.actor.category)) ||
        (self->unk_1DA <= 0)) {
        self->unk_1C8 += 0.3f;
        if (self->unk_1C8 >= 2.0f) {
            func_8086DB4C(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
        }
    }
}

void BgBdanSwitch_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgBdanSwitch* self = THIS;
    s32 type;

    if (self->unk_1DA > 0) {
        self->unk_1DA--;
    }
    self->actionFunc(self, globalCtx);
    func_8086D0EC(self);
    type = self->dyna.actor.params & 0xFF;
    if (type != 3 && type != 4) {
        self->unk_1D8--;
    } else {
        if (!Player_InCsMode(globalCtx) && self->unk_1D8 > 0) {
            self->unk_1D8--;
        }
        self->unk_1DC = self->collider.base.acFlags;
        self->collider.base.acFlags &= ~AC_HIT;
        self->collider.elements[0].dim.modelSphere.radius = self->unk_1D4 * 370.0f;
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void func_8086DF58(BgBdanSwitch* self, GlobalContext* globalCtx, Gfx* dlist) {
    func_800D1694(self->dyna.actor.world.pos.x,
                  self->dyna.actor.world.pos.y + (self->dyna.actor.shape.yOffset * self->unk_1D0),
                  self->dyna.actor.world.pos.z, &self->dyna.actor.shape.rot);
    Matrix_Scale(self->unk_1D4, self->unk_1D0, self->unk_1D4, MTXMODE_APPLY);
    Gfx_DrawDListOpa(globalCtx, dlist);
}

void BgBdanSwitch_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgBdanSwitch* self = THIS;

    switch (self->dyna.actor.params & 0xFF) {
        case YELLOW_HEAVY:
        case YELLOW:
            func_8086DF58(self, globalCtx, gJabuYellowFloorSwitchDL);
            break;
        case YELLOW_TALL_1:
        case YELLOW_TALL_2:
            func_8086DF58(self, globalCtx, gJabuYellowFloorSwitchDL);
            Collider_UpdateSpheres(0, &self->collider);
            Matrix_MultVec3f(&D_8086E0E0, &self->dyna.actor.focus.pos);
            break;
        case BLUE:
            func_8086DF58(self, globalCtx, gJabuBlueFloorSwitchDL);
            break;
    }
}
