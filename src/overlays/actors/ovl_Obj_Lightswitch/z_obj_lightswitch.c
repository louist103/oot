/*
 * File: z_obj_lightswitch.c
 * Overlay: ovl_Obj_Lightswitch
 * Description: Sun Emblem Trigger (Spirit Temple)
 */

#include "z_obj_lightswitch.h"
#include "vt.h"
#include "overlays/actors/ovl_Obj_Oshihiki/z_obj_oshihiki.h"
#include "objects/object_lightswitch/object_lightswitch.h"

#define FLAGS 0x00000010

#define THIS ((ObjLightswitch*)thisx)

typedef enum {
    /* 0x00 */ FACE_EYES_CLOSED,
    /* 0x01 */ FACE_EYES_OPEN,
    /* 0x02 */ FACE_EYES_OPEN_SMILING
} FaceTextureIndex;

void ObjLightswitch_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjLightswitch_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjLightswitch_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjLightswitch_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjLightswitch_SetupOff(ObjLightswitch* self);
void ObjLightswitch_Off(ObjLightswitch* self, GlobalContext* globalCtx);
void ObjLightswitch_SetupTurnOn(ObjLightswitch* self);
void ObjLightswitch_TurnOn(ObjLightswitch* self, GlobalContext* globalCtx);
void ObjLightswitch_SetupOn(ObjLightswitch* self);
void ObjLightswitch_On(ObjLightswitch* self, GlobalContext* globalCtx);
void ObjLightswitch_SetupTurnOff(ObjLightswitch* self);
void ObjLightswitch_TurnOff(ObjLightswitch* self, GlobalContext* globalCtx);
void ObjLightswitch_SetupDisappearDelay(ObjLightswitch* self);
void ObjLightswitch_DisappearDelay(ObjLightswitch* self, GlobalContext* globalCtx);
void ObjLightswitch_SetupDisappear(ObjLightswitch* self);
void ObjLightswitch_Disappear(ObjLightswitch* self, GlobalContext* globalCtx);

const ActorInit Obj_Lightswitch_InitVars = {
    ACTOR_OBJ_LIGHTSWITCH,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_LIGHTSWITCH,
    sizeof(ObjLightswitch),
    (ActorFunc)ObjLightswitch_Init,
    (ActorFunc)ObjLightswitch_Destroy,
    (ActorFunc)ObjLightswitch_Update,
    (ActorFunc)ObjLightswitch_Draw,
};

static ColliderJntSphElementInit sColliderJntSphElementInit[] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00200000, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 19 }, 100 },
    },
};
static ColliderJntSphInit sColliderJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    1,
    sColliderJntSphElementInit,
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 12, 60, MASS_IMMOVABLE };

static void* sFaceTextures[] = { object_lightswitch_Tex_000C20, object_lightswitch_Tex_000420,
                                 object_lightswitch_Tex_001420 };

static Vec3f D_80B97F68 = { -1707.0f, 843.0f, -180.0f };
static Vec3f D_80B97F74 = { 0.0f, 0.0f, 0.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void ObjLightswitch_InitCollider(ObjLightswitch* self, GlobalContext* globalCtx) {
    s32 pad;

    Collider_InitJntSph(globalCtx, &self->collider);
    Collider_SetJntSph(globalCtx, &self->collider, &self->actor, &sColliderJntSphInit, self->colliderItems);
    func_800D1694(self->actor.world.pos.x, self->actor.world.pos.y + (self->actor.shape.yOffset * self->actor.scale.y),
                  self->actor.world.pos.z, &self->actor.shape.rot);
    Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
    Collider_UpdateSpheres(0, &self->collider);
}

void ObjLightswitch_SetSwitchFlag(ObjLightswitch* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->actor; // required
    s32 type;

    if (!Flags_GetSwitch(globalCtx, self->actor.params >> 8 & 0x3F)) {
        type = self->actor.params >> 4 & 3;

        Flags_SetSwitch(globalCtx, self->actor.params >> 8 & 0x3F);

        if (type == OBJLIGHTSWITCH_TYPE_1) {
            OnePointCutscene_AttentionSetSfx(globalCtx, thisx, NA_SE_SY_TRE_BOX_APPEAR);
        } else if (type == OBJLIGHTSWITCH_TYPE_BURN) {
            OnePointCutscene_AttentionSetSfx(globalCtx, thisx, NA_SE_SY_ERROR);
        } else {
            OnePointCutscene_AttentionSetSfx(globalCtx, thisx, NA_SE_SY_CORRECT_CHIME);
        }
    }
}

void ObjLightswitch_ClearSwitchFlag(ObjLightswitch* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, self->actor.params >> 8 & 0x3F)) {
        Flags_UnsetSwitch(globalCtx, self->actor.params >> 8 & 0x3F);

        if ((self->actor.params >> 4 & 3) == OBJLIGHTSWITCH_TYPE_1) {
            OnePointCutscene_AttentionSetSfx(globalCtx, &self->actor, NA_SE_SY_TRE_BOX_APPEAR);
        }
    }
}

void ObjLightswitch_SpawnDisappearEffects(ObjLightswitch* self, GlobalContext* globalCtx) {
    Vec3f pos;
    f32 s = Math_SinS(self->actor.shape.rot.y);
    f32 c = Math_CosS(self->actor.shape.rot.y);
    f32 x;
    f32 y;
    f32 z;
    s32 pad;

    if (self->alpha >= (100 << 6)) {
        x = (CLAMP_MAX((1.0f - 1.0f / (255 << 6) * self->alpha) * 400.0f, 60.0f) - 30.0f + 30.0f) * Rand_ZeroOne();
        y = x - 30.0f;
        if (x > 30.0f) {
            x = 30.0f;
        } else {
            x = 900.0f - (y * y);
            if (x < 100.0f) {
                x = 100.0f;
            }
            x = sqrtf(x);
        }
        x = 2.0f * (x * (Rand_ZeroOne() - 0.5f));
        z = (30.0f - fabsf(x)) * 0.5f + 10.0f * Rand_ZeroOne();
        pos.x = self->actor.world.pos.x + ((z * s) + (x * c));
        pos.y = self->actor.world.pos.y + y + 10.0f;
        pos.z = self->actor.world.pos.z + ((z * c) - (x * s));
        EffectSsDeadDb_Spawn(globalCtx, &pos, &D_80B97F74, &D_80B97F74, 100, 0, 255, 255, 160, 160, 255, 0, 0, 1, 9,
                             true);
    }
}

void ObjLightswitch_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjLightswitch* self = THIS;
    s32 switchFlagSet = Flags_GetSwitch(globalCtx, self->actor.params >> 8 & 0x3F);
    s32 removeSelf = false;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    Actor_SetFocus(&self->actor, 0.0f);
    if (switchFlagSet) {
        if ((self->actor.params >> 4 & 3) == OBJLIGHTSWITCH_TYPE_BURN) {
            removeSelf = true;
        } else {
            ObjLightswitch_SetupOn(self);
        }
    } else {
        ObjLightswitch_SetupOff(self);
    }
    if ((self->actor.params & 1) == 1) {
        if (switchFlagSet) {
            Math_Vec3f_Copy(&self->actor.world.pos, &D_80B97F68);
            Math_Vec3f_Copy(&self->actor.home.pos, &D_80B97F68);
        }
        self->actor.shape.rot.x = -0x4000;
        self->actor.shape.rot.z = 0;
        self->actor.world.rot.x = self->actor.home.rot.x = self->actor.shape.rot.x;
        self->actor.world.rot.z = self->actor.home.rot.z = self->actor.shape.rot.z;
        self->actor.flags |= 0x20;
        if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_OBJ_OSHIHIKI,
                               self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z, 0,
                               self->actor.home.rot.y, 0, (0xFF << 8) | PUSHBLOCK_SMALL_START_ON) == NULL) {
            osSyncPrintf(VT_COL(RED, WHITE));
            // "Push-pull block occurrence failure"
            osSyncPrintf("押引ブロック発生失敗(%s %d)(arg_data 0x%04x)\n", "../z_obj_lightswitch.c", 452,
                         self->actor.params);
            osSyncPrintf(VT_RST);
            removeSelf = true;
        }
    }
    ObjLightswitch_InitCollider(self, globalCtx);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);
    if (removeSelf) {
        Actor_Kill(&self->actor);
    }
    // "Light switch"
    osSyncPrintf("(光スイッチ)(arg_data 0x%04x)\n", self->actor.params);
}

void ObjLightswitch_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    ObjLightswitch* self = THIS;

    Collider_DestroyJntSph(globalCtx, &self->collider);
}

void ObjLightswitch_SetupOff(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_Off;
    self->faceTextureIndex = FACE_EYES_CLOSED;
    self->color[0] = 155 << 6;
    self->color[1] = 125 << 6;
    self->color[2] = 255 << 6;
    self->alpha = 255 << 6;
}

void ObjLightswitch_Off(ObjLightswitch* self, GlobalContext* globalCtx) {
    switch (self->actor.params >> 4 & 3) {
        case OBJLIGHTSWITCH_TYPE_STAY_ON:
        case OBJLIGHTSWITCH_TYPE_2:
            if (self->collider.base.acFlags & AC_HIT) {
                ObjLightswitch_SetupTurnOn(self);
                ObjLightswitch_SetSwitchFlag(self, globalCtx);
            }
            break;
        case OBJLIGHTSWITCH_TYPE_1:
            if ((self->collider.base.acFlags & AC_HIT) && !(self->prevFrameACflags & AC_HIT)) {
                ObjLightswitch_SetupTurnOn(self);
                ObjLightswitch_SetSwitchFlag(self, globalCtx);
            }
            break;
        case OBJLIGHTSWITCH_TYPE_BURN:
            if (self->collider.base.acFlags & AC_HIT) {
                ObjLightswitch_SetupDisappearDelay(self);
                ObjLightswitch_SetSwitchFlag(self, globalCtx);
            }
            break;
    }
}

void ObjLightswitch_SetupTurnOn(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_TurnOn;
    self->toggleDelay = 100;
    self->timer = 0;
    self->faceTextureIndex = FACE_EYES_CLOSED;
}

void ObjLightswitch_TurnOn(ObjLightswitch* self, GlobalContext* globalCtx) {
    if (func_8005B198() == self->actor.category || self->toggleDelay <= 0) {
        if (self->timer == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_TRIFORCE_FLASH);
        }

        self->timer++;

        Math_StepToS(&self->flameRingRotSpeed, -0xAA, 0xA);
        self->flameRingRot += self->flameRingRotSpeed;

        self->color[0] = self->timer * (((255 - 155) << 6) / 20) + (155 << 6);
        self->color[1] = self->timer * (((255 - 125) << 6) / 20) + (125 << 6);

        if (self->timer >= 20) {
            ObjLightswitch_SetupOn(self);
        } else if (self->timer == 15) {
            self->faceTextureIndex = FACE_EYES_OPEN;
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_FOOT_SWITCH);
        }
    }
}

void ObjLightswitch_SetupOn(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_On;
    self->faceTextureIndex = FACE_EYES_OPEN_SMILING;

    self->color[0] = 255 << 6;
    self->color[1] = 255 << 6;
    self->color[2] = 255 << 6;
    self->alpha = 255 << 6;

    self->flameRingRotSpeed = -0xAA;
    self->timer = 0;
}

void ObjLightswitch_On(ObjLightswitch* self, GlobalContext* globalCtx) {
    switch (self->actor.params >> 4 & 3) {
        case OBJLIGHTSWITCH_TYPE_STAY_ON:
            if (!Flags_GetSwitch(globalCtx, self->actor.params >> 8 & 0x3F)) {
                ObjLightswitch_SetupTurnOff(self);
            }
            break;
        case OBJLIGHTSWITCH_TYPE_1:
            if (self->collider.base.acFlags & AC_HIT && !(self->prevFrameACflags & AC_HIT)) {
                ObjLightswitch_SetupTurnOff(self);
                ObjLightswitch_ClearSwitchFlag(self, globalCtx);
            }
            break;
        case OBJLIGHTSWITCH_TYPE_2:
            if (!(self->collider.base.acFlags & AC_HIT)) {
                if (self->timer >= 7) {
                    ObjLightswitch_SetupTurnOff(self);
                    ObjLightswitch_ClearSwitchFlag(self, globalCtx);
                } else {
                    self->timer++;
                }
            } else {
                self->timer = 0;
            }
            break;
    }
    self->flameRingRot += self->flameRingRotSpeed;
}

void ObjLightswitch_SetupTurnOff(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_TurnOff;
    self->toggleDelay = 100;
    self->timer = 20;
    self->faceTextureIndex = FACE_EYES_OPEN;
}

void ObjLightswitch_TurnOff(ObjLightswitch* self, GlobalContext* globalCtx) {
    if ((self->actor.params >> 4 & 3) != OBJLIGHTSWITCH_TYPE_1 || func_8005B198() == self->actor.category ||
        self->toggleDelay <= 0) {
        self->timer--;

        Math_StepToS(&self->flameRingRotSpeed, 0, 0xA);
        self->flameRingRot += self->flameRingRotSpeed;

        self->color[0] = self->timer * (((255 - 155) << 6) / 20) + (155 << 6);
        self->color[1] = self->timer * (((255 - 125) << 6) / 20) + (125 << 6);

        if (self->timer <= 0) {
            ObjLightswitch_SetupOff(self);
        } else if (self->timer == 15) {
            self->faceTextureIndex = FACE_EYES_CLOSED;
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_FOOT_SWITCH);
        }
    }
}

void ObjLightswitch_SetupDisappearDelay(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_DisappearDelay;
    self->toggleDelay = 100;
}

void ObjLightswitch_DisappearDelay(ObjLightswitch* self, GlobalContext* globalCtx) {
    if (func_8005B198() == self->actor.category || self->toggleDelay <= 0) {
        ObjLightswitch_SetupDisappear(self);
    }
}

void ObjLightswitch_SetupDisappear(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_Disappear;
    self->alpha = 255 << 6;
}

void ObjLightswitch_Disappear(ObjLightswitch* self, GlobalContext* globalCtx) {
    self->alpha -= 200;
    ObjLightswitch_SpawnDisappearEffects(self, globalCtx);
    if (self->alpha < 0) {
        Actor_Kill(&self->actor);
    }
}

void ObjLightswitch_Update(Actor* thisx, GlobalContext* globalCtx2) {
    ObjLightswitch* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    if (self->toggleDelay > 0) {
        self->toggleDelay--;
    }

    self->actionFunc(self, globalCtx);

    if (self->actor.update != NULL) {
        if ((self->actor.params & 1) == 1) {
            self->actor.world.pos.x = self->actor.child->world.pos.x;
            self->actor.world.pos.y = self->actor.child->world.pos.y + 60.0f;
            self->actor.world.pos.z = self->actor.child->world.pos.z;
            Actor_SetFocus(&self->actor, 0.0f);
        }

        self->prevFrameACflags = self->collider.base.acFlags;
        self->collider.base.acFlags &= ~AC_HIT;
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void ObjLightswitch_DrawOpa(ObjLightswitch* self, GlobalContext* globalCtx) {
    Actor* child;
    Vec3f pos;
    Vec3s rot;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 809);
    func_80093D18(globalCtx->state.gfxCtx);

    gDPSetEnvColor(POLY_OPA_DISP++, (u8)(self->color[0] >> 6), (u8)(self->color[1] >> 6), (u8)(self->color[2] >> 6),
                   (u8)(self->alpha >> 6));
    gSPSegment(POLY_OPA_DISP++, 0x09, &D_80116280[2]);

    if ((self->actor.params & 1) == 1) {
        child = self->actor.child;
        self->actor.world.pos.x = child->world.pos.x;
        self->actor.world.pos.y = child->world.pos.y + 60.0f;
        self->actor.world.pos.z = child->world.pos.z;
        Math_Vec3f_Copy(&pos, &self->actor.world.pos);
        func_800D1694(pos.x, pos.y, pos.z, &self->actor.shape.rot);
        Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
    } else {
        pos.x = self->actor.world.pos.x;
        pos.y = self->actor.world.pos.y + self->actor.shape.yOffset * self->actor.scale.y;
        pos.z = self->actor.world.pos.z;
    }

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 841),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sFaceTextures[self->faceTextureIndex]));
    gSPDisplayList(POLY_OPA_DISP++, object_lightswitch_DL_000260);

    rot.x = self->actor.shape.rot.x;
    rot.y = self->actor.shape.rot.y;
    rot.z = self->actor.shape.rot.z + self->flameRingRot;
    func_800D1694(pos.x, pos.y, pos.z, &rot);
    Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 859),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_lightswitch_DL_000398);

    rot.z = self->actor.shape.rot.z - self->flameRingRot;
    func_800D1694(pos.x, pos.y, pos.z, &rot);
    Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 873),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_lightswitch_DL_000408);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 878);
}

void ObjLightswitch_DrawXlu(ObjLightswitch* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f sp68;
    Vec3s sp60;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 890);
    func_80093D84(globalCtx->state.gfxCtx);

    gDPSetEnvColor(POLY_XLU_DISP++, (u8)(self->color[0] >> 6), (u8)(self->color[1] >> 6), (u8)(self->color[2] >> 6),
                   (u8)(self->alpha >> 6));
    gSPSegment(POLY_XLU_DISP++, 0x09, D_80116280);

    sp68.x = self->actor.world.pos.x;
    sp68.y = self->actor.world.pos.y + (self->actor.shape.yOffset * self->actor.scale.y);
    sp68.z = self->actor.world.pos.z;

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 912),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sFaceTextures[self->faceTextureIndex]));
    gSPDisplayList(POLY_XLU_DISP++, object_lightswitch_DL_000260);

    sp60.x = self->actor.shape.rot.x;
    sp60.y = self->actor.shape.rot.y;
    sp60.z = self->actor.shape.rot.z + self->flameRingRot;

    func_800D1694(sp68.x, sp68.y, sp68.z, &sp60);
    Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 930),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, object_lightswitch_DL_000398);

    sp60.z = self->actor.shape.rot.z - self->flameRingRot;
    func_800D1694(sp68.x, sp68.y, sp68.z, &sp60);
    Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 944),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, object_lightswitch_DL_000408);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_lightswitch.c", 949);
}

void ObjLightswitch_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ObjLightswitch* self = THIS;
    s32 alpha = self->alpha >> 6 & 0xFF;

    if ((self->actor.params & 1) == 1) {
        Collider_UpdateSpheres(0, &self->collider);
    }

    if ((self->actor.params >> 4 & 3) == OBJLIGHTSWITCH_TYPE_BURN && (alpha > 0 || alpha < 255)) {
        ObjLightswitch_DrawXlu(self, globalCtx);
    } else {
        ObjLightswitch_DrawOpa(self, globalCtx);
    }
}
