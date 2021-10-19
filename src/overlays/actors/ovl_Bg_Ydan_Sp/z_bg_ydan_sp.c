/*
 * File: z_bg_ydan_sp.c
 * Overlay: ovl_Bg_Ydan_Sp
 * Description: Webs
 */

#include "z_bg_ydan_sp.h"
#include "objects/object_ydan_objects/object_ydan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgYdanSp*)thisx)

void BgYdanSp_Init(Actor* thisx, GlobalContext* globalCtx);
void BgYdanSp_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgYdanSp_Update(Actor* thisx, GlobalContext* globalCtx);
void BgYdanSp_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgYdanSp_BurnFloorWeb(BgYdanSp* self, GlobalContext* globalCtx);
void BgYdanSp_FloorWebIdle(BgYdanSp* self, GlobalContext* globalCtx);
void BgYdanSp_BurnWallWeb(BgYdanSp* self, GlobalContext* globalCtx);
void BgYdanSp_WallWebIdle(BgYdanSp* self, GlobalContext* globalCtx);

extern CollisionHeader gDTWebWallCol;

typedef enum {
    /* 0 */ WEB_FLOOR,
    /* 1 */ WEB_WALL
} BgYdanSpType;

const ActorInit Bg_Ydan_Sp_InitVars = {
    ACTOR_BG_YDAN_SP,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_YDAN_OBJECTS,
    sizeof(BgYdanSp),
    (ActorFunc)BgYdanSp_Init,
    (ActorFunc)BgYdanSp_Destroy,
    (ActorFunc)BgYdanSp_Update,
    (ActorFunc)BgYdanSp_Draw,
};

static ColliderTrisElementInit sTrisItemsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0x00020800, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 75.0f, -8.0f, 75.0f }, { -75.0f, -8.0f, 75.0f }, { -75.0f, -8.0f, -75.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x00 },
            { 0x00020800, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 140.0f, 288.8f, 0.0f }, { -140.0f, 288.0f, 0.0f }, { -140.0f, 0.0f, 0.0f } } },
    },
};

static ColliderTrisInit sTrisInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_TRIS,
    },
    2,
    sTrisItemsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgYdanSp_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanSp* self = THIS;
    ColliderTrisElementInit* ti0 = &sTrisItemsInit[0];
    Vec3f tri[3];
    s32 i;
    CollisionHeader* colHeader = NULL;
    ColliderTrisElementInit* ti1 = &sTrisItemsInit[1];
    f32 cossY;
    f32 sinsY;
    f32 cossX;
    f32 nSinsX;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    self->isDestroyedSwitchFlag = thisx->params & 0x3F;
    self->burnSwitchFlag = (thisx->params >> 6) & 0x3F;
    self->dyna.actor.params = (thisx->params >> 0xC) & 0xF;
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    Collider_InitTris(globalCtx, &self->trisCollider);
    Collider_SetTris(globalCtx, &self->trisCollider, &self->dyna.actor, &sTrisInit, self->trisColliderItems);
    if (self->dyna.actor.params == WEB_FLOOR) {
        CollisionHeader_GetVirtual(&gDTWebFloorCol, &colHeader);
        self->actionFunc = BgYdanSp_FloorWebIdle;

        for (i = 0; i < 3; i++) {
            tri[i].x = ti0->dim.vtx[i].x + self->dyna.actor.world.pos.x;
            tri[i].y = ti0->dim.vtx[i].y + self->dyna.actor.world.pos.y;
            tri[i].z = ti0->dim.vtx[i].z + self->dyna.actor.world.pos.z;
        }

        Collider_SetTrisVertices(&self->trisCollider, 0, &tri[0], &tri[1], &tri[2]);
        tri[1].x = tri[0].x;
        tri[1].z = tri[2].z;
        Collider_SetTrisVertices(&self->trisCollider, 1, &tri[0], &tri[2], &tri[1]);
        self->unk16C = 0.0f;
    } else {
        CollisionHeader_GetVirtual(&gDTWebWallCol, &colHeader);
        self->actionFunc = BgYdanSp_WallWebIdle;
        Actor_SetFocus(&self->dyna.actor, 30.0f);
        sinsY = Math_SinS(self->dyna.actor.shape.rot.y);
        cossY = Math_CosS(self->dyna.actor.shape.rot.y);
        nSinsX = -Math_SinS(self->dyna.actor.shape.rot.x);
        cossX = Math_CosS(self->dyna.actor.shape.rot.x);

        for (i = 0; i < 3; i++) {
            tri[i].x =
                self->dyna.actor.world.pos.x + (cossY * ti1->dim.vtx[i].x) - (sinsY * ti1->dim.vtx[i].y * nSinsX);
            tri[i].y = self->dyna.actor.world.pos.y + (ti1->dim.vtx[i].y * cossX);
            tri[i].z =
                self->dyna.actor.world.pos.z - (sinsY * ti1->dim.vtx[i].x) + (ti1->dim.vtx[i].y * cossY * nSinsX);
        }

        Collider_SetTrisVertices(&self->trisCollider, 0, &tri[0], &tri[1], &tri[2]);

        tri[1].x = self->dyna.actor.world.pos.x + (cossY * ti1->dim.vtx[0].x) - (ti1->dim.vtx[2].y * sinsY * nSinsX);
        tri[1].y = self->dyna.actor.world.pos.y + (ti1->dim.vtx[2].y * cossX);
        tri[1].z = self->dyna.actor.world.pos.z - (sinsY * ti1->dim.vtx[0].x) + (ti1->dim.vtx[2].y * cossY * nSinsX);
        Collider_SetTrisVertices(&self->trisCollider, 1, &tri[0], &tri[2], &tri[1]);
    }
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
    self->timer = 0;
    if (Flags_GetSwitch(globalCtx, self->isDestroyedSwitchFlag)) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgYdanSp_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanSp* self = THIS;
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyTris(globalCtx, &self->trisCollider);
}

void BgYdanSp_UpdateFloorWebCollision(BgYdanSp* self) {
    s16 newY;
    CollisionHeader* colHeader;

    colHeader = SEGMENTED_TO_VIRTUAL(&gDTWebFloorCol);
    colHeader->vtxList = SEGMENTED_TO_VIRTUAL(colHeader->vtxList);
    newY = (self->dyna.actor.home.pos.y - self->dyna.actor.world.pos.y) * 10;
    colHeader->vtxList[14].y = newY;
    colHeader->vtxList[12].y = newY;
    colHeader->vtxList[10].y = newY;
    colHeader->vtxList[9].y = newY;
    colHeader->vtxList[6].y = newY;
    colHeader->vtxList[5].y = newY;
    colHeader->vtxList[1].y = newY;
    colHeader->vtxList[0].y = newY;
#ifdef AVOID_UB
    return colHeader; // not used, surpress warning
#endif
}

void BgYdanSp_BurnWeb(BgYdanSp* self, GlobalContext* globalCtx) {
    self->timer = 30;
    self = self;
    func_80078884(NA_SE_SY_CORRECT_CHIME);
    Flags_SetSwitch(globalCtx, self->isDestroyedSwitchFlag);
    if (self->dyna.actor.params == WEB_FLOOR) {
        self->actionFunc = BgYdanSp_BurnFloorWeb;
    } else {
        self->actionFunc = BgYdanSp_BurnWallWeb;
    }
}

void BgYdanSp_BurnFloorWeb(BgYdanSp* self, GlobalContext* globalCtx) {
    static Vec3f accel = { 0 };
    Vec3f velocity;
    Vec3f pos2;
    f32 distXZ;
    f32 sins;
    f32 coss;
    s16 rot;
    s16 rot2;
    s32 i;

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        Actor_Kill(&self->dyna.actor);
        return;
    }
    if ((self->timer % 3) == 0) {
        rot2 = Rand_ZeroOne() * 0x2AAA;
        velocity.y = 0.0f;
        pos2.y = self->dyna.actor.world.pos.y;

        for (i = 0; i < 6; i++) {
            rot = Rand_CenteredFloat(0x2800) + rot2;
            sins = Math_SinS(rot);
            coss = Math_CosS(rot);
            pos2.x = self->dyna.actor.world.pos.x + (120.0f * sins);
            pos2.z = self->dyna.actor.world.pos.z + (120.0f * coss);
            distXZ = Math_Vec3f_DistXZ(&self->dyna.actor.home.pos, &pos2) * (1.0f / 120.0f);
            if (distXZ < 0.7f) {
                sins = Math_SinS(rot + 0x8000);
                coss = Math_CosS(rot + 0x8000);
                pos2.x = self->dyna.actor.world.pos.x + (120.0f * sins);
                pos2.z = self->dyna.actor.world.pos.z + (120.0f * coss);
                distXZ = Math_Vec3f_DistXZ(&self->dyna.actor.home.pos, &pos2) * (1.0f / 120.0f);
            }
            velocity.x = (7.0f * sins) * distXZ;
            velocity.y = 0.0f;
            velocity.z = (7.0f * coss) * distXZ;
            EffectSsDeadDb_Spawn(globalCtx, &self->dyna.actor.home.pos, &velocity, &accel, 60, 6, 255, 255, 150, 170,
                                 255, 0, 0, 1, 0xE, 1);
            rot2 += 0x2AAA;
        }
    }
}

void BgYdanSp_FloorWebBroken(BgYdanSp* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgYdanSp_FloorWebBreaking(BgYdanSp* self, GlobalContext* globalCtx) {
    static Color_RGBA8 primColor = { 250, 250, 250, 255 };
    static Color_RGBA8 envColor = { 180, 180, 180, 255 };
    static Vec3f zeroVec = { 0 };
    s32 i;
    Vec3f pos;
    s16 rot;

    if (self->timer != 0) {
        self->timer--;
    }

    self->dyna.actor.world.pos.y = (sinf((f32)self->timer * (M_PI / 20)) * self->unk16C) + self->dyna.actor.home.pos.y;
    if (self->dyna.actor.home.pos.y - self->dyna.actor.world.pos.y > 190.0f) {
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        self->timer = 40;
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        Flags_SetSwitch(globalCtx, self->isDestroyedSwitchFlag);
        self->actionFunc = BgYdanSp_FloorWebBroken;
        pos.y = self->dyna.actor.world.pos.y - 60.0f;
        rot = 0;
        for (i = 0; i < 6; i++) {
            pos.x = Math_SinS(rot) * 60.0f + self->dyna.actor.world.pos.x;
            pos.z = Math_CosS(rot) * 60.0f + self->dyna.actor.world.pos.z;
            func_8002829C(globalCtx, &pos, &zeroVec, &zeroVec, &primColor, &envColor, 1000, 10);

            rot += 0x2AAA;
        }
    }
    BgYdanSp_UpdateFloorWebCollision(self);
}

void BgYdanSp_FloorWebIdle(BgYdanSp* self, GlobalContext* globalCtx) {
    Player* player;
    Vec3f webPos;
    f32 sqrtFallDistance;
    f32 unk;

    player = GET_PLAYER(globalCtx);
    webPos.x = self->dyna.actor.world.pos.x;
    webPos.y = self->dyna.actor.world.pos.y - 50.0f;
    webPos.z = self->dyna.actor.world.pos.z;
    if (Player_IsBurningStickInRange(globalCtx, &webPos, 70.0f, 50.0f) != 0) {
        self->dyna.actor.home.pos.x = player->swordInfo[0].tip.x;
        self->dyna.actor.home.pos.z = player->swordInfo[0].tip.z;
        BgYdanSp_BurnWeb(self, globalCtx);
        return;
    }
    if ((self->trisCollider.base.acFlags & 2) != 0) {
        BgYdanSp_BurnWeb(self, globalCtx);
        return;
    }
    if (func_8004356C(&self->dyna)) {
        sqrtFallDistance = sqrtf(CLAMP_MIN(player->fallDistance, 0.0f));
        if (player->fallDistance > 750.0f) {
            if (self->dyna.actor.xzDistToPlayer < 80.0f) {
                self->unk16C = 200.0f;
                self->dyna.actor.room = -1;
                self->dyna.actor.flags |= 0x10;
                self->timer = 40;
                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_WEB_BROKEN);
                self->actionFunc = BgYdanSp_FloorWebBreaking;
                return;
            }
        }
        unk = sqrtFallDistance + sqrtFallDistance;
        if (self->unk16C < unk) {
            if (unk > 2.0f) {
                self->unk16C = unk;
                self->timer = 14;
            }
        }
        if (player->actor.speedXZ != 0.0f) {
            if (self->unk16C < 0.1f) {
                self->timer = 14;
            }
            if (self->unk16C < 2.0f) {
                self->unk16C = 2.0f;
            } else {
                self->unk16C = self->unk16C;
            }
        }
    }
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->timer = 14;
    }
    self->dyna.actor.world.pos.y = sinf((f32)self->timer * (M_PI / 7)) * self->unk16C + self->dyna.actor.home.pos.y;
    Math_ApproachZeroF(&self->unk16C, 1.0f, 0.8f);
    if (self->timer == 13) {
        if (self->unk16C > 3.0f) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_WEB_VIBRATION);
        } else {
            Audio_StopSfx(NA_SE_EV_WEB_VIBRATION);
        }
    }
    BgYdanSp_UpdateFloorWebCollision(self);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->trisCollider.base);
}

void BgYdanSp_BurnWallWeb(BgYdanSp* self, GlobalContext* globalCtx) {
    static Vec3f accel = { 0 };
    Vec3f velocity;
    Vec3f spC8;
    f32 distXYZ;
    f32 sins;
    f32 coss;
    f32 coss2;
    s16 rot;
    s16 rot2;
    s32 i;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        Actor_Kill(&self->dyna.actor);
        return;
    }
    if ((self->timer % 3) == 0) {
        rot2 = Rand_ZeroOne() * 0x2AAA;

        for (i = 0; i < 6; i++) {
            rot = Rand_CenteredFloat(0x2800) + rot2;
            sins = Math_SinS(rot);
            coss = Math_CosS(rot);
            coss2 = Math_CosS(self->dyna.actor.shape.rot.y) * sins;
            sins *= Math_SinS(self->dyna.actor.shape.rot.y);

            spC8.x = self->dyna.actor.world.pos.x + (140.0f * coss2);
            spC8.y = self->dyna.actor.world.pos.y + (140.0f * (1.0f + coss));
            spC8.z = self->dyna.actor.world.pos.z - (140.0f * sins);
            distXYZ = Math_Vec3f_DistXYZ(&self->dyna.actor.home.pos, &spC8) * (1.0f / 140.0f);
            if (distXYZ < 0.65f) {
                sins = Math_SinS(rot + 0x8000);
                coss = Math_CosS(rot + 0x8000);
                coss2 = Math_CosS(self->dyna.actor.shape.rot.y) * sins;
                sins *= Math_SinS(self->dyna.actor.shape.rot.y);
                spC8.x = self->dyna.actor.world.pos.x + (140.0f * coss2);
                spC8.y = self->dyna.actor.world.pos.y + (140.0f * (1.0f + coss));
                spC8.z = self->dyna.actor.world.pos.z - (140.0f * sins);
                distXYZ = Math_Vec3f_DistXYZ(&self->dyna.actor.home.pos, &spC8) * (1.0f / 140.0f);
            }
            velocity.x = 6.5f * coss2 * distXYZ;
            velocity.y = 6.5f * coss * distXYZ;
            velocity.z = -6.5f * sins * distXYZ;
            EffectSsDeadDb_Spawn(globalCtx, &self->dyna.actor.home.pos, &velocity, &accel, 80, 6, 255, 255, 150, 170,
                                 255, 0, 0, 1, 0xE, 1);
            rot2 += 0x2AAA;
        }
    }
}

void BgYdanSp_WallWebIdle(BgYdanSp* self, GlobalContext* globalCtx) {
    Player* player;
    Vec3f sp30;

    player = GET_PLAYER(globalCtx);
    if (Flags_GetSwitch(globalCtx, self->burnSwitchFlag) || (self->trisCollider.base.acFlags & 2)) {
        self->dyna.actor.home.pos.y = self->dyna.actor.world.pos.y + 80.0f;
        BgYdanSp_BurnWeb(self, globalCtx);
    } else if (player->heldItemActionParam == PLAYER_AP_STICK && player->unk_860 != 0) {
        func_8002DBD0(&self->dyna.actor, &sp30, &player->swordInfo[0].tip);
        if (fabsf(sp30.x) < 100.0f && sp30.z < 1.0f && sp30.y < 200.0f) {
            OnePointCutscene_Init(globalCtx, 3020, 40, &self->dyna.actor, MAIN_CAM);
            Math_Vec3f_Copy(&self->dyna.actor.home.pos, &player->swordInfo[0].tip);
            BgYdanSp_BurnWeb(self, globalCtx);
        }
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->trisCollider.base);
}

void BgYdanSp_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanSp* self = THIS;
    self->actionFunc(self, globalCtx);
}

void BgYdanSp_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanSp* self = THIS;
    s32 i;
    MtxF mtxF;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_ydan_sp.c", 781);
    func_80093D84(globalCtx->state.gfxCtx);
    if (thisx->params == WEB_WALL) {
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_ydan_sp.c", 787),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gDTWebWallDL);
    } else if (self->actionFunc == BgYdanSp_FloorWebBroken) {
        Matrix_Get(&mtxF);
        if (self->timer == 40) {
            Matrix_Translate(0.0f, (thisx->home.pos.y - thisx->world.pos.y) * 10.0f, 0.0f, MTXMODE_APPLY);
            Matrix_Scale(1.0f, ((thisx->home.pos.y - thisx->world.pos.y) + 10.0f) * 0.1f, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_ydan_sp.c", 808),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gDTWebFloorDL);
        }
        for (i = 0; i < 8; i++) {
            Matrix_Put(&mtxF);
            Matrix_RotateRPY(-0x5A0, i * 0x2000, 0, MTXMODE_APPLY);
            Matrix_Translate(0.0f, 700.0f, -900.0f, MTXMODE_APPLY);
            Matrix_Scale(3.5f, 5.0f, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_ydan_sp.c", 830),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gDTUnknownWebDL);
        }
    } else {
        Matrix_Translate(0.0f, (thisx->home.pos.y - thisx->world.pos.y) * 10.0f, 0.0f, MTXMODE_APPLY);
        Matrix_Scale(1.0f, ((thisx->home.pos.y - thisx->world.pos.y) + 10.0f) * 0.1f, 1.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_ydan_sp.c", 849),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gDTWebFloorDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_ydan_sp.c", 856);
}
