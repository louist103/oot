/*
 * File: z_bg_hidan_rock.c
 * Overlay: ovl_Bg_Hidan_Rock
 * Description: Stone blocks (Fire Temple)
 */

#include "z_bg_hidan_rock.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgHidanRock*)thisx)

void BgHidanRock_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanRock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanRock_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanRock_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8088B24C(BgHidanRock* self);

void func_8088B268(BgHidanRock* self, GlobalContext* globalCtx);
void func_8088B5F4(BgHidanRock* self, GlobalContext* globalCtx);
void func_8088B634(BgHidanRock* self, GlobalContext* globalCtx);
void func_8088B69C(BgHidanRock* self, GlobalContext* globalCtx);
void func_8088B79C(BgHidanRock* self, GlobalContext* globalCtx);
void func_8088B90C(BgHidanRock* self, GlobalContext* globalCtx);
void func_8088B954(BgHidanRock* self, GlobalContext* globalCtx);
void func_8088B990(BgHidanRock* self, GlobalContext* globalCtx);

void func_8088BC40(GlobalContext* globalCtx, BgHidanRock* self);

static Vec3f D_8088BF60 = { 3310.0f, 120.0f, 0.0f };

const ActorInit Bg_Hidan_Rock_InitVars = {
    ACTOR_BG_HIDAN_ROCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanRock),
    (ActorFunc)BgHidanRock_Init,
    (ActorFunc)BgHidanRock_Destroy,
    (ActorFunc)BgHidanRock_Update,
    (ActorFunc)BgHidanRock_Draw,
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
        { 0x20000000, 0x01, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { 45, 77, -40, { 3310, 120, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -600, ICHAIN_STOP),
};

void BgHidanRock_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanRock* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);

    self->type = thisx->params & 0xFF;
    self->unk_169 = 0;

    thisx->params = ((thisx->params) >> 8) & 0xFF;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, thisx, &sCylinderInit);

    if (self->type == 0) {
        if (Flags_GetSwitch(globalCtx, thisx->params)) {
            Math_Vec3f_Copy(&thisx->home.pos, &D_8088BF60);
            Math_Vec3f_Copy(&thisx->world.pos, &D_8088BF60);
            self->timer = 60;
            self->actionFunc = func_8088B5F4;
        } else {
            self->actionFunc = func_8088B268;
        }
        thisx->flags |= 0x30;
        CollisionHeader_GetVirtual(&gFireTempleStoneBlock1Col, &colHeader);
    } else {
        CollisionHeader_GetVirtual(&gFireTempleStoneBlock2Col, &colHeader);
        self->collider.dim.pos.x = thisx->home.pos.x;
        self->collider.dim.pos.y = thisx->home.pos.y;
        self->collider.dim.pos.z = thisx->home.pos.z;
        self->actionFunc = func_8088B634;
    }

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    self->timer = 0;
}

void BgHidanRock_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanRock* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(globalCtx, &self->collider);
    func_800F89E8(&self->unk_170);
}

void func_8088B24C(BgHidanRock* self) {
    self->dyna.actor.flags |= 0x30;
    self->actionFunc = func_8088B990;
}

void func_8088B268(BgHidanRock* self, GlobalContext* globalCtx) {
    static f32 D_8088BFC0 = 0.0f;
    f32 sp2C;
    s32 temp_v1;
    s32 frame;
    Player* player = GET_PLAYER(globalCtx);

    if (self->dyna.unk_150 != 0.0f) {
        if (self->timer == 0) {
            if (D_8088BFC0 == 0.0f) {
                if (self->dyna.unk_150 > 0.0f) {
                    D_8088BFC0 += 0.01f;
                } else {
                    D_8088BFC0 -= 0.01f;
                }
            }

            self->dyna.actor.speedXZ += 0.05f;
            self->dyna.actor.speedXZ = CLAMP_MAX(self->dyna.actor.speedXZ, 2.0f);

            if (D_8088BFC0 > 0.0f) {
                temp_v1 = Math_StepToF(&D_8088BFC0, 20.0f, self->dyna.actor.speedXZ);
            } else {
                temp_v1 = Math_StepToF(&D_8088BFC0, -20.0f, self->dyna.actor.speedXZ);
            }

            self->dyna.actor.world.pos.x = (Math_SinS(self->dyna.unk_158) * D_8088BFC0) + self->dyna.actor.home.pos.x;
            self->dyna.actor.world.pos.z = (Math_CosS(self->dyna.unk_158) * D_8088BFC0) + self->dyna.actor.home.pos.z;

            if (temp_v1) {
                player->stateFlags2 &= ~0x10;
                self->dyna.unk_150 = 0.0f;
                self->dyna.actor.home.pos.x = self->dyna.actor.world.pos.x;
                self->dyna.actor.home.pos.z = self->dyna.actor.world.pos.z;
                D_8088BFC0 = 0.0f;
                self->dyna.actor.speedXZ = 0.0f;
                self->timer = 5;
            }

            func_8002F974(&self->dyna.actor, NA_SE_EV_ROCK_SLIDE - SFX_FLAG);
        } else {
            player->stateFlags2 &= ~0x10;
            self->dyna.unk_150 = 0.0f;
            if (self->timer != 0) {
                self->timer--;
            }
        }
    } else {
        self->timer = 0;
    }

    sp2C = self->dyna.actor.world.pos.z - D_8088BF60.z;
    if (sp2C < 0.5f) {
        Flags_SetSwitch(globalCtx, self->dyna.actor.params);
        Math_Vec3f_Copy(&self->dyna.actor.home.pos, &D_8088BF60);
        self->dyna.actor.world.pos.x = D_8088BF60.x;
        self->dyna.actor.world.pos.z = D_8088BF60.z;
        self->dyna.actor.speedXZ = 0.0f;
        D_8088BFC0 = 0.0f;
        player->stateFlags2 &= ~0x10;
        self->actionFunc = func_8088B79C;
    }

    frame = globalCtx->gameplayFrames & 0xFF;
    if (globalCtx->gameplayFrames & 0x100) {
        self->unk_16C = 0.0f;
    } else if (frame < 128) {
        self->unk_16C = sinf(frame * (4 * 0.001f * M_PI)) * 19.625f;
    } else if (frame < 230) {
        self->unk_16C = 19.625f;
    } else {
        self->unk_16C -= 1.0f;
        self->unk_16C = CLAMP_MIN(self->unk_16C, 0.0f);
    }

    if (sp2C < 100.0f) {
        self->unk_16C = CLAMP_MAX(self->unk_16C, 6.125f);
    }
}

void func_8088B5F4(BgHidanRock* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        func_8088B24C(self);
    }
}

void func_8088B634(BgHidanRock* self, GlobalContext* globalCtx) {
    if (func_8004356C(&self->dyna)) {
        self->timer = 20;
        self->dyna.actor.world.rot.y = Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4000;
        self->actionFunc = func_8088B69C;
    }
}

void func_8088B69C(BgHidanRock* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer != 0) {
        self->dyna.actor.world.pos.x =
            self->dyna.actor.home.pos.x + 5.0f * Math_SinS(self->dyna.actor.world.rot.y + self->timer * 0x4000);
        self->dyna.actor.world.pos.z =
            self->dyna.actor.home.pos.z + 5.0f * Math_CosS(self->dyna.actor.world.rot.y + self->timer * 0x4000);
    } else {
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
        func_8088B24C(self);
    }

    if (!(self->timer % 4)) {
        func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 0xB4, 0x0A, 0x64);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_SHAKE);
    }
}

void func_8088B79C(BgHidanRock* self, GlobalContext* globalCtx) {
    self->timer--;
    if (self->dyna.actor.bgCheckFlags & 2) {
        if (self->type == 0) {
            self->timer = 60;
            self->actionFunc = func_8088B5F4;
        } else {
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - 15.0f;
            self->actionFunc = func_8088B90C;
            self->dyna.actor.flags &= ~0x30;
        }

        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        Audio_PlayActorSound2(
            &self->dyna.actor,
            SurfaceType_GetSfx(&globalCtx->colCtx, self->dyna.actor.floorPoly, self->dyna.actor.floorBgId) + 0x800);
    }

    self->unk_16C -= 0.5f;
    self->unk_16C = CLAMP_MIN(self->unk_16C, 0.0f);

    if (self->type == 0) {
        if (func_8004356C(&self->dyna)) {
            if (self->unk_169 == 0) {
                self->unk_169 = 3;
            }
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_HIDAN1);
        } else if (!func_8004356C(&self->dyna)) {
            if (self->unk_169 != 0) {
                Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON0);
            }
            self->unk_169 = 0;
        }
    }
}

void func_8088B90C(BgHidanRock* self, GlobalContext* globalCtx) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 1.0f)) {
        self->actionFunc = func_8088B634;
    }
}

void func_8088B954(BgHidanRock* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        self->actionFunc = func_8088B79C;
        self->dyna.actor.velocity.y = 0.0f;
    }
}

void func_8088B990(BgHidanRock* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->timer++;
    if (self->dyna.unk_150 != 0.0f) {
        self->dyna.actor.speedXZ = 0.0f;
        player->stateFlags2 &= ~0x10;
    }

    if ((self->type == 0 && (Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 1820.0f,
                                                0.25f, 20.0f, 0.5f) < 0.1f)) ||
        ((self->type != 0) && (Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 480.0,
                                                  0.25f, 20.0f, 0.5f) < 0.1f))) {
        if (self->type == 0) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_BOUND);
        }
        self->timer = 20;
        self->actionFunc = func_8088B954;
    }

    self->unk_16C = (self->dyna.actor.world.pos.y + 50.0f - self->dyna.actor.home.pos.y + 40.0f) / 80.0f;
    if (self->type == 0) {
        if (func_8004356C(&self->dyna)) {
            if (self->unk_169 == 0) {
                self->unk_169 = 3;
            }
            Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_HIDAN1);
        } else if (!func_8004356C(&self->dyna)) {
            if (self->unk_169 != 0) {
                Camera_ChangeSetting(globalCtx->cameraPtrs[MAIN_CAM], CAM_SET_DUNGEON0);
            }
            self->unk_169 = 0;
        }
    }
}

void BgHidanRock_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanRock* self = THIS;

    self->actionFunc(self, globalCtx);
    if (self->actionFunc == func_8088B79C) {
        Actor_MoveForward(&self->dyna.actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->dyna.actor, 0.0f, 0.0f, 0.0f, 4);
    }

    if (self->unk_16C > 0.0f) {
        self->collider.dim.height = sCylinderInit.dim.height * self->unk_16C;
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

static void* sVerticalFlamesTexs[] = {
    gFireTempleBigVerticalFlame0Tex, gFireTempleBigVerticalFlame1Tex, gFireTempleBigVerticalFlame2Tex,
    gFireTempleBigVerticalFlame3Tex, gFireTempleBigVerticalFlame4Tex, gFireTempleBigVerticalFlame5Tex,
    gFireTempleBigVerticalFlame6Tex, gFireTempleBigVerticalFlame7Tex,
};

void func_8088BC40(GlobalContext* globalCtx, BgHidanRock* self) {
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_rock.c", 808);

    POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0x14);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x01, 255, 255, 0, 150);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 255);

    if (self->type == 0) {
        Matrix_Translate(D_8088BF60.x, D_8088BF60.y - 40.0f, D_8088BF60.z, MTXMODE_NEW);
    } else {
        Matrix_Translate(self->dyna.actor.home.pos.x, self->dyna.actor.home.pos.y - 40.0f, self->dyna.actor.home.pos.z,
                         MTXMODE_NEW);
    }

    Matrix_RotateRPY(0, Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000, 0, MTXMODE_APPLY);
    Matrix_Translate(-10.5f, 0.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(6.0f, self->unk_16C, 6.0f, MTXMODE_APPLY);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sVerticalFlamesTexs[globalCtx->gameplayFrames & 7]));
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_hidan_rock.c", 853),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gFireTempleBigVerticalFlameDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_rock.c", 857);
}

void BgHidanRock_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanRock* self = THIS;
    s32 pad;

    if (self->type == 0) {
        Gfx_DrawDListOpa(globalCtx, gFireTempleStoneBlock1DL);
    } else {
        Gfx_DrawDListOpa(globalCtx, gFireTempleStoneBlock2DL);
    }

    if (self->unk_16C > 0.0f) {
        if (self->type == 0) {
            SkinMatrix_Vec3fMtxFMultXYZ(&globalCtx->mf_11D60, &D_8088BF60, &self->unk_170);
        } else {
            SkinMatrix_Vec3fMtxFMultXYZ(&globalCtx->mf_11D60, &self->dyna.actor.home.pos, &self->unk_170);
        }

        func_80078914(&self->unk_170, NA_SE_EV_FIRE_PILLAR - SFX_FLAG);
        func_8088BC40(globalCtx, self);
    }
}
