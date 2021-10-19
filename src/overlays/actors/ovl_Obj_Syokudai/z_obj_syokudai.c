/*
 * File: z_obj_syokudai.c
 * Overlay: ovl_Obj_Syokudai
 * Description: Torch
 */

#include "z_obj_syokudai.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_syokudai/object_syokudai.h"

#define FLAGS 0x00000410

#define THIS ((ObjSyokudai*)thisx)

void ObjSyokudai_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjSyokudai_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjSyokudai_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjSyokudai_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Obj_Syokudai_InitVars = {
    ACTOR_OBJ_SYOKUDAI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SYOKUDAI,
    sizeof(ObjSyokudai),
    (ActorFunc)ObjSyokudai_Init,
    (ActorFunc)ObjSyokudai_Destroy,
    (ActorFunc)ObjSyokudai_Update,
    (ActorFunc)ObjSyokudai_Draw,
};

static ColliderCylinderInit sCylInitStand = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00100000, 0x00, 0x00 },
        { 0xEE01FFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 12, 45, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylInitFlame = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
        { 0x00020820, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 15, 45, 45, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

static s32 sLitTorchCount;

void ObjSyokudai_Init(Actor* thisx, GlobalContext* globalCtx) {
    static u8 sColTypesStand[] = { 0x09, 0x0B, 0x0B };
    s32 pad;
    ObjSyokudai* self = THIS;
    s32 torchType = self->actor.params & 0xF000;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);

    Collider_InitCylinder(globalCtx, &self->colliderStand);
    Collider_SetCylinder(globalCtx, &self->colliderStand, &self->actor, &sCylInitStand);
    self->colliderStand.base.colType = sColTypesStand[self->actor.params >> 0xC];

    Collider_InitCylinder(globalCtx, &self->colliderFlame);
    Collider_SetCylinder(globalCtx, &self->colliderFlame, &self->actor, &sCylInitFlame);

    self->actor.colChkInfo.mass = MASS_IMMOVABLE;

    Lights_PointGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y + 70.0f,
                            self->actor.world.pos.z, 255, 255, 180, -1);
    self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);

    if ((self->actor.params & 0x400) || ((torchType != 2) && Flags_GetSwitch(globalCtx, self->actor.params & 0x3F))) {
        self->litTimer = -1;
    }

    self->flameTexScroll = (s32)(Rand_ZeroOne() * 20.0f);
    sLitTorchCount = 0;
    Actor_SetFocus(&self->actor, 60.0f);
}

void ObjSyokudai_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    ObjSyokudai* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->colliderStand);
    Collider_DestroyCylinder(globalCtx, &self->colliderFlame);
    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
}

void ObjSyokudai_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    ObjSyokudai* self = THIS;
    s32 torchCount = (self->actor.params >> 6) & 0xF;
    s32 switchFlag = self->actor.params & 0x3F;
    s32 torchType = self->actor.params & 0xF000;
    s32 litTimeScale;
    WaterBox* dummy;
    f32 waterSurface;
    s32 lightRadius = -1;
    u8 brightness = 0;
    Player* player;
    EnArrow* arrow;
    s32 interactionType;
    u32 dmgFlags;
    Vec3f tipToFlame;
    s32 pad;
    s32 pad2;

    litTimeScale = torchCount;
    if (torchCount == 10) {
        torchCount = 24;
    }
    if (WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->actor.world.pos.x, self->actor.world.pos.z,
                                &waterSurface, &dummy) &&
        ((waterSurface - self->actor.world.pos.y) > 52.0f)) {
        self->litTimer = 0;
        if (torchType == 1) {
            Flags_UnsetSwitch(globalCtx, switchFlag);
            if (torchCount != 0) {
                self->litTimer = 1;
            }
        }
    } else {
        player = GET_PLAYER(globalCtx);
        interactionType = 0;
        if (self->actor.params & 0x400) {
            self->litTimer = -1;
        }
        if (torchCount != 0) {
            if (Flags_GetSwitch(globalCtx, switchFlag)) {
                if (self->litTimer == 0) {
                    self->litTimer = -1;
                    if (torchType == 0) {
                        OnePointCutscene_Attention(globalCtx, &self->actor);
                    }
                } else if (self->litTimer > 0) {
                    self->litTimer = -1;
                }
            } else if (self->litTimer < 0) {
                self->litTimer = 20;
            }
        }
        if (self->colliderFlame.base.acFlags & AC_HIT) {
            dmgFlags = self->colliderFlame.info.acHitInfo->toucher.dmgFlags;
            if (dmgFlags & 0x20820) {
                interactionType = 1;
            }
        } else if (player->heldItemActionParam == PLAYER_AP_STICK) {
            Math_Vec3f_Diff(&player->swordInfo[0].tip, &self->actor.world.pos, &tipToFlame);
            tipToFlame.y -= 67.0f;
            if ((SQ(tipToFlame.x) + SQ(tipToFlame.y) + SQ(tipToFlame.z)) < SQ(20.0f)) {
                interactionType = -1;
            }
        }
        if (interactionType != 0) {
            if (self->litTimer != 0) {
                if (interactionType < 0) {
                    if (player->unk_860 == 0) {
                        player->unk_860 = 210;
                        Audio_PlaySoundGeneral(NA_SE_EV_FLAME_IGNITION, &self->actor.projectedPos, 4, &D_801333E0,
                                               &D_801333E0, &D_801333E8);
                    } else if (player->unk_860 < 200) {
                        player->unk_860 = 200;
                    }
                } else if (dmgFlags & 0x20) {
                    arrow = (EnArrow*)self->colliderFlame.base.ac;
                    if ((arrow->actor.update != NULL) && (arrow->actor.id == ACTOR_EN_ARROW)) {
                        arrow->actor.params = 0;
                        arrow->collider.info.toucher.dmgFlags = 0x800;
                    }
                }
                if ((0 <= self->litTimer) && (self->litTimer < (50 * litTimeScale + 100)) && (torchType != 0)) {
                    self->litTimer = 50 * litTimeScale + 100;
                }
            } else if ((torchType != 0) && (((interactionType > 0) && (dmgFlags & 0x20800)) ||
                                            ((interactionType < 0) && (player->unk_860 != 0)))) {

                if ((interactionType < 0) && (player->unk_860 < 200)) {
                    player->unk_860 = 200;
                }
                if (torchCount == 0) {
                    self->litTimer = -1;
                    if (torchType != 2) {
                        Flags_SetSwitch(globalCtx, switchFlag);
                        OnePointCutscene_Attention(globalCtx, &self->actor);
                    }
                } else {
                    sLitTorchCount++;
                    if (sLitTorchCount >= torchCount) {
                        Flags_SetSwitch(globalCtx, switchFlag);
                        OnePointCutscene_Attention(globalCtx, &self->actor);
                        self->litTimer = -1;
                    } else {
                        self->litTimer = (litTimeScale * 50) + 110;
                    }
                }
                Audio_PlaySoundGeneral(NA_SE_EV_FLAME_IGNITION, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                       &D_801333E8);
            }
        }
    }

    Collider_UpdateCylinder(&self->actor, &self->colliderStand);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderStand.base);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderStand.base);

    Collider_UpdateCylinder(&self->actor, &self->colliderFlame);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderFlame.base);

    if (self->litTimer > 0) {
        self->litTimer--;
        if ((self->litTimer == 0) && (torchType != 0)) {
            sLitTorchCount--;
        }
    }
    if (self->litTimer != 0) {
        if ((self->litTimer < 0) || (self->litTimer >= 20)) {
            lightRadius = 200;
        } else {
            lightRadius = (self->litTimer * 200.0f) / 20.0f;
        }
        brightness = (u8)(Rand_ZeroOne() * 127.0f) + 128;
        func_8002F974(&self->actor, NA_SE_EV_TORCH - SFX_FLAG);
    }
    Lights_PointSetColorAndRadius(&self->lightInfo, brightness, brightness, 0, lightRadius);
    self->flameTexScroll++;
}

void ObjSyokudai_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* displayLists[] = { gGoldenTorchDL, gTimedTorchDL, gWoodenTorchDL };
    s32 pad;
    ObjSyokudai* self = THIS;
    s32 timerMax;

    timerMax = (((self->actor.params >> 6) & 0xF) * 50) + 100;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_syokudai.c", 707);
    func_80093D18(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_syokudai.c", 714),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, displayLists[(u16)self->actor.params >> 0xC]);

    if (self->litTimer != 0) {
        f32 flameScale = 1.0f;

        if (self->litTimer > timerMax) {
            flameScale = (timerMax - self->litTimer + 10) / 10.0f;
        } else if ((self->litTimer > 0) && (self->litTimer < 20)) {
            flameScale = self->litTimer / 20.0f;
        }
        flameScale *= 0.0027f;

        func_80093D84(globalCtx->state.gfxCtx);

        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0,
                                    (self->flameTexScroll * -20) & 0x1FF, 0x20, 0x80));

        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 0, 255);

        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);

        Matrix_Translate(0.0f, 52.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateY((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) - self->actor.shape.rot.y + 0x8000) *
                           (M_PI / 0x8000),
                       MTXMODE_APPLY);
        Matrix_Scale(flameScale, flameScale, flameScale, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_syokudai.c", 745),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_syokudai.c", 749);
}
