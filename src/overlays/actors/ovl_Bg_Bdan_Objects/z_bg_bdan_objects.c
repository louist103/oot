/*
 * File: z_bg_bdan_objects.c
 * Overlay: ovl_Bg_Bdan_Objects
 * Description: Lord Jabu-Jabu Objects
 */

#include "z_bg_bdan_objects.h"
#include "objects/object_bdan_objects/object_bdan_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgBdanObjects*)thisx)

void BgBdanObjects_Init(Actor* thisx, GlobalContext* globalCtx);
void BgBdanObjects_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgBdanObjects_Update(Actor* thisx, GlobalContext* globalCtx);
void BgBdanObjects_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8086C054(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C1A0(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C29C(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C55C(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C5BC(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C618(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C6EC(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C76C(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C7D0(BgBdanObjects* self, GlobalContext* globalCtx);
void BgBdanObjects_DoNothing(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C874(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C9A8(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086C9F0(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086CABC(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086CB10(BgBdanObjects* self, GlobalContext* globalCtx);
void func_8086CB8C(BgBdanObjects* self, GlobalContext* globalCtx);

const ActorInit Bg_Bdan_Objects_InitVars = {
    ACTOR_BG_BDAN_OBJECTS,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_BDAN_OBJECTS,
    sizeof(BgBdanObjects),
    (ActorFunc)BgBdanObjects_Init,
    (ActorFunc)BgBdanObjects_Destroy,
    (ActorFunc)BgBdanObjects_Update,
    (ActorFunc)BgBdanObjects_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x04 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_HARD,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { 0x00BB, 0x0050, 0x0000, { 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static Gfx* sDLists[] = {
    gJabuObjectsLargeRotatingSpikePlatformDL,
    gJabuElevatorPlatformDL,
    gJabuWaterDL,
    gJabuFallingPlatformDL,
};

s32 BgBdanObjects_GetContactRu1(BgBdanObjects* self, s32 arg1) {
    switch (arg1) {
        case 0:
            return self->cameraSetting == CAM_SET_NORMAL0;
        case 4:
            return gSaveContext.infTable[20] & 0x40;
        case 3:
            return self->cameraSetting == CAM_SET_DUNGEON1;
        default:
            osSyncPrintf("Bg_Bdan_Objects_Get_Contact_Ru1\nそんな受信モードは無い%d!!!!!!!!\n");
            return -1;
    }
}

void BgBdanObjects_SetContactRu1(BgBdanObjects* self, s32 arg1) {
    switch (arg1) {
        case 1:
            self->cameraSetting = CAM_SET_NORMAL1;
            break;
        case 2:
            self->cameraSetting = CAM_SET_DUNGEON0;
            break;
        case 4:
            gSaveContext.infTable[20] |= 0x40;
            break;
        default:
            osSyncPrintf("Bg_Bdan_Objects_Set_Contact_Ru1\nそんな送信モードは無い%d!!!!!!!!\n");
    }
}

void BgBdanObjects_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgBdanObjects* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    self->switchFlag = (thisx->params >> 8) & 0x3F;
    thisx->params &= 0xFF;
    if (thisx->params == 2) {
        thisx->flags |= 0x30;
        globalCtx->colCtx.colHeader->waterBoxes[7].ySurface = thisx->world.pos.y;
        self->actionFunc = func_8086C9A8;
        return;
    }
    if (thisx->params == 0) {
        CollisionHeader_GetVirtual(&gJabuBigOctoPlatformCol, &colHeader);
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->dyna.actor, &sCylinderInit);
        thisx->world.pos.y += -79.0f;
        if (Flags_GetClear(globalCtx, thisx->room)) {
            Flags_SetSwitch(globalCtx, self->switchFlag);
            self->actionFunc = func_8086C6EC;
        } else {
            if (BgBdanObjects_GetContactRu1(self, 4)) {
                if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_EN_BIGOKUTA,
                                       thisx->home.pos.x, thisx->home.pos.y, thisx->home.pos.z, 0,
                                       thisx->shape.rot.y + 0x8000, 0, 3) != NULL) {
                    thisx->child->world.pos.z = thisx->child->home.pos.z + 263.0f;
                }
                thisx->world.rot.y = 0;
                self->actionFunc = func_8086C618;
                thisx->world.pos.y = thisx->home.pos.y + -70.0f;
            } else {
                Flags_SetSwitch(globalCtx, self->switchFlag);
                self->timer = 0;
                self->actionFunc = func_8086C054;
            }
        }
    } else {
        if (thisx->params == 1) {
            CollisionHeader_GetVirtual(&gJabuElevatorCol, &colHeader);
            self->timer = 512;
            self->switchFlag = 0;
            self->actionFunc = func_8086C874;
        } else {
            CollisionHeader_GetVirtual(&gJabuLoweringPlatformCol, &colHeader);
            if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
                self->actionFunc = BgBdanObjects_DoNothing;
                thisx->world.pos.y = thisx->home.pos.y - 400.0f;
            } else {
                self->actionFunc = func_8086CB10;
            }
        }
    }
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
}

void BgBdanObjects_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgBdanObjects* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    if (thisx->params == 0) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void func_8086C054(BgBdanObjects* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (BgBdanObjects_GetContactRu1(self, 0)) {
        if (self->dyna.actor.xzDistToPlayer < 250.0f) {
            BgBdanObjects_SetContactRu1(self, 1);
            self->timer = 20;
            OnePointCutscene_Init(globalCtx, 3070, -99, &self->dyna.actor, MAIN_CAM);
            player->actor.world.pos.x = -1130.0f;
            player->actor.world.pos.y = -1025.0f;
            player->actor.world.pos.z = -3300.0f;
            func_800AA000(0.0f, 0xFF, 0x14, 0x96);
        }
    } else if (self->timer != 0) {
        if (self->timer != 0) {
            self->timer--;
        }
        if (self->timer == 0) {
            self->actionFunc = func_8086C1A0;
        }
    }

    if (!Gameplay_InCsMode(globalCtx) && !BgBdanObjects_GetContactRu1(self, 0)) {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + -79.0f;
    } else {
        self->dyna.actor.world.pos.y = (self->dyna.actor.home.pos.y + -79.0f) - 5.0f;
    }
}

void func_8086C1A0(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 500.0f, 0.5f, 7.5f, 1.0f) <
        0.1f) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BUYOSTAND_STOP_A);
        self->actionFunc = func_8086C29C;
        self->timer = 30;
        BgBdanObjects_SetContactRu1(self, 2);
        func_800AA000(0.0f, 0xFF, 0x14, 0x96);
    } else {
        if (self->timer != 0) {
            self->timer--;
        }
        if (self->timer == 0) {
            func_800AA000(0.0f, 0x78, 0x14, 0xA);
            self->timer = 11;
        }
        func_8002F974(&self->dyna.actor, NA_SE_EV_BUYOSTAND_RISING - SFX_FLAG);
    }
}

void func_8086C29C(BgBdanObjects* self, GlobalContext* globalCtx) {
    s32 temp;

    if (self->timer != 0) {
        self->timer--;
        if (self->timer == 0) {
            temp = Quake_Add(GET_ACTIVE_CAM(globalCtx), 1);
            Quake_SetSpeed(temp, 0x3A98);
            Quake_SetQuakeValues(temp, 0, 1, 0xFA, 1);
            Quake_SetCountdown(temp, 0xA);
        }
    }

    if (BgBdanObjects_GetContactRu1(self, 3)) {
        Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_EN_BIGOKUTA,
                           self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y + 140.0f,
                           self->dyna.actor.world.pos.z, 0, self->dyna.actor.shape.rot.y + 0x8000, 0, 0);
        BgBdanObjects_SetContactRu1(self, 4);
        self->timer = 10;
        self->actionFunc = func_8086C55C;
        func_8005B1A4(GET_ACTIVE_CAM(globalCtx));
    }
}

void func_8086C3D8(BgBdanObjects* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->dyna.actor.velocity.y += 0.5f;
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + -70.0f,
                     self->dyna.actor.velocity.y)) {
        self->dyna.actor.world.rot.y = 0;
        self->timer = 60;
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BUYOSTAND_STOP_U);
        self->dyna.actor.child->world.pos.y = self->dyna.actor.world.pos.y + 140.0f;
        self->actionFunc = func_8086C5BC;
        OnePointCutscene_Init(globalCtx, 3080, -99, self->dyna.actor.child, MAIN_CAM);
        player->actor.world.pos.x = -1130.0f;
        player->actor.world.pos.y = -1025.0f;
        player->actor.world.pos.z = -3500.0f;
        player->actor.shape.rot.y = 0x7530;
        player->actor.world.rot.y = player->actor.shape.rot.y;
        func_800AA000(0.0f, 0xFF, 0x1E, 0x96);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_BUYOSTAND_FALL - SFX_FLAG);
        if (self->timer != 0) {
            self->timer--;
        }
        if (self->timer == 0) {
            func_800AA000(0.0f, 0x78, 0x14, 0xA);
            self->timer = 11;
        }
        if (self->dyna.actor.child != NULL) {
            self->dyna.actor.child->world.pos.y = self->dyna.actor.world.pos.y + 140.0f;
        }
    }
}

void func_8086C55C(BgBdanObjects* self, GlobalContext* globalCtx) {
    self->timer--;

    if (self->timer == 0) {
        Flags_UnsetSwitch(globalCtx, self->switchFlag);
    } else if (self->timer == -40) {
        self->timer = 0;
        self->actionFunc = func_8086C3D8;
    }
}

void func_8086C5BC(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if ((self->timer == 0) && (self->dyna.actor.child != NULL)) {
        if (self->dyna.actor.child->params == 2) {
            self->actionFunc = func_8086C618;
        } else if (self->dyna.actor.child->params == 0) {
            self->dyna.actor.child->params = 1;
        }
    }
}

void func_8086C618(BgBdanObjects* self, GlobalContext* globalCtx) {
    Collider_UpdateCylinder(&self->dyna.actor, &self->collider);
    CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    if (Flags_GetClear(globalCtx, self->dyna.actor.room)) {
        Flags_SetSwitch(globalCtx, self->switchFlag);
        self->dyna.actor.home.rot.y = (s16)(self->dyna.actor.shape.rot.y + 0x2000) & 0xC000;
        self->actionFunc = func_8086C6EC;
    } else {
        self->dyna.actor.shape.rot.y += self->dyna.actor.world.rot.y;
        func_800F436C(&self->dyna.actor.projectedPos, 0x2063, ABS(self->dyna.actor.world.rot.y) / 512.0f);
    }
}

void func_8086C6EC(BgBdanObjects* self, GlobalContext* globalCtx) {
    s32 cond = Math_ScaledStepToS(&self->dyna.actor.shape.rot.y, self->dyna.actor.home.rot.y, 0x200);

    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + -125.0f, 3.0f)) {
        if (cond) {
            self->actionFunc = func_8086C76C;
        }
    }
}

void func_8086C76C(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (func_8004356C(&self->dyna)) {
        if (self->dyna.actor.xzDistToPlayer < 120.0f) {
            self->actionFunc = func_8086C7D0;
            OnePointCutscene_Init(globalCtx, 3090, -99, &self->dyna.actor, MAIN_CAM);
        }
    }
}

void func_8086C7D0(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 965.0f, 0.5f, 15.0f, 0.2f) <
        0.01f) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BUYOSTAND_STOP_A);
        self->actionFunc = BgBdanObjects_DoNothing;
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_BUYOSTAND_RISING - SFX_FLAG);
    }
}

void BgBdanObjects_DoNothing(BgBdanObjects* self, GlobalContext* globalCtx) {
}

void func_8086C874(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->switchFlag == 0) {
        if (func_8004356C(&self->dyna)) {
            self->cameraSetting = globalCtx->cameraPtrs[MAIN_CAM]->setting;
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_NORMAL2);
            func_8005AD1C(globalCtx->cameraPtrs[MAIN_CAM], 4);
            self->switchFlag = 10;
        }
    } else {
        Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_NORMAL2);
        if (!func_8004356C(&self->dyna)) {
            if (self->switchFlag != 0) {
                self->switchFlag--;
            }
        }
        if (self->switchFlag == 0) {
            if (1) {}
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], self->cameraSetting);
            func_8005ACFC(globalCtx->cameraPtrs[MAIN_CAM], 4);
        }
    }
    self->dyna.actor.world.pos.y =
        self->dyna.actor.home.pos.y - (sinf(self->timer * (M_PI / 256.0f)) * 471.24f); // pi * 150
    if (self->timer == 0) {
        self->timer = 512;
    }
}

void func_8086C9A8(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
        self->timer = 100;
        self->actionFunc = func_8086C9F0;
    }
}

void func_8086C9F0(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (self->timer == 0) {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 0.5f)) {
            Flags_UnsetSwitch(globalCtx, self->switchFlag);
            self->actionFunc = func_8086C9A8;
        }
        func_8002F948(&self->dyna.actor, NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG);
    } else {
        if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 75.0f, 0.5f)) {
            self->actionFunc = func_8086CABC;
        }
        func_8002F948(&self->dyna.actor, NA_SE_EV_WATER_LEVEL_DOWN - SFX_FLAG);
    }
    globalCtx->colCtx.colHeader->waterBoxes[7].ySurface = self->dyna.actor.world.pos.y;
}

void func_8086CABC(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    func_8002F994(&self->dyna.actor, self->timer);
    if (self->timer == 0) {
        self->actionFunc = func_8086C9F0;
    }
}

void func_8086CB10(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (func_8004356C(&self->dyna)) {
        Flags_SetSwitch(globalCtx, self->switchFlag);
        self->timer = 50;
        self->actionFunc = func_8086CB8C;
        self->dyna.actor.home.pos.y -= 200.0f;
        OnePointCutscene_Init(globalCtx, 3100, 51, &self->dyna.actor, MAIN_CAM);
    }
}

void func_8086CB8C(BgBdanObjects* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - (cosf(self->timer * (M_PI / 50.0f)) * 200.0f);

    if (self->timer == 0) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BUYOSTAND_STOP_U);
        self->actionFunc = BgBdanObjects_DoNothing;
        Gameplay_CopyCamera(globalCtx, MAIN_CAM, SUBCAM_ACTIVE);
    } else {
        func_8002F974(&self->dyna.actor, NA_SE_EV_BUYOSTAND_FALL - SFX_FLAG);
    }
}

void BgBdanObjects_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgBdanObjects* self = THIS;

    Actor_SetFocus(thisx, 50.0f);
    self->actionFunc(self, globalCtx);
}

void BgBdanObjects_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgBdanObjects* self = THIS;

    if (thisx->params == 0) {
        if (self->actionFunc == func_8086C054) {
            if (((thisx->home.pos.y + -79.0f) - 5.0f) < thisx->world.pos.y) {
                Matrix_Translate(0.0f, -50.0f, 0.0f, MTXMODE_APPLY);
            }
        }
    }

    if (thisx->params == 2) {
        Gfx_DrawDListXlu(globalCtx, gJabuWaterDL);
    } else {
        Gfx_DrawDListOpa(globalCtx, sDLists[thisx->params]);
    }
}
