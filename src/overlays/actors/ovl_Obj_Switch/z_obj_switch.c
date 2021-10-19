/*
 * File: z_obj_switch.c
 * Overlay: ovl_Obj_Switch
 * Description: Switches
 */

#include "z_obj_switch.h"
#include "objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((ObjSwitch*)thisx)

// type:        (self->dyna.actor.params & 7)
// subtype:     (self->dyna.actor.params >> 4 & 7)
// switch flag: (self->dyna.actor.params >> 8 & 0x3F)
// frozen:      self->dyna.actor.params >> 7 & 1

void ObjSwitch_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjSwitch_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjSwitch_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjSwitch_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjSwitch_FloorUpInit(ObjSwitch* self);
void ObjSwitch_FloorUp(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_FloorPressInit(ObjSwitch* self);
void ObjSwitch_FloorPress(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_FloorDownInit(ObjSwitch* self);
void ObjSwitch_FloorDown(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_FloorReleaseInit(ObjSwitch* self);
void ObjSwitch_FloorRelease(ObjSwitch* self, GlobalContext* globalCtx);

void ObjSwitch_EyeFrozenInit(ObjSwitch* self);
void ObjSwitch_EyeInit(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_EyeOpenInit(ObjSwitch* self);
void ObjSwitch_EyeOpen(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_EyeClosingInit(ObjSwitch* self);
void ObjSwitch_EyeClosing(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_EyeClosedInit(ObjSwitch* self);
void ObjSwitch_EyeClosed(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_EyeOpeningInit(ObjSwitch* self);
void ObjSwitch_EyeOpening(ObjSwitch* self, GlobalContext* globalCtx);

void ObjSwitch_CrystalOffInit(ObjSwitch* self);
void ObjSwitch_CrystalOff(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_CrystalTurnOnInit(ObjSwitch* self);
void ObjSwitch_CrystalTurnOn(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_CrystalOnInit(ObjSwitch* self);
void ObjSwitch_CrystalOn(ObjSwitch* self, GlobalContext* globalCtx);
void ObjSwitch_CrystalTurnOffInit(ObjSwitch* self);
void ObjSwitch_CrystalTurnOff(ObjSwitch* self, GlobalContext* globalCtx);

const ActorInit Obj_Switch_InitVars = {
    ACTOR_OBJ_SWITCH,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(ObjSwitch),
    (ActorFunc)ObjSwitch_Init,
    (ActorFunc)ObjSwitch_Destroy,
    (ActorFunc)ObjSwitch_Update,
    (ActorFunc)ObjSwitch_Draw,
};

static f32 sHeights[] = { 10, 10, 0, 30, 30 };

static ColliderTrisElementInit D_80B9EC34[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { -20.0f, 19.0f, -20.0f }, { -20.0f, 19.0f, 20.0f }, { 20.0f, 19.0f, 20.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x40000040, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 20.0f, 19.0f, 20.0f }, { 20.0f, 19.0f, -20.0f }, { -20.0f, 19.0f, -20.0f } } },
    },
};

static ColliderTrisInit sRustyFloorTrisInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    D_80B9EC34,
};

static ColliderTrisElementInit D_80B9ECBC[2] = {
    {
        {
            ELEMTYPE_UNK4,
            { 0x00000000, 0x00, 0x00 },
            { 0x0001F824, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 0.0f, 23.0f, 8.5f }, { -23.0f, 0.0f, 8.5f }, { 0.0f, -23.0f, 8.5f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x0001F824, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 0.0f, 23.0f, 8.5f }, { 0.0f, -23.0f, 8.5f }, { 23.0f, 0.0f, 8.5f } } },
    },
};

static ColliderTrisInit trisColliderEye = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_TRIS,
    },
    2,
    D_80B9ECBC,
};

static ColliderJntSphElementInit D_80B9ED44[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xEFC1FFFE, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 300, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sCyrstalJntSphereInit = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    1,
    D_80B9ED44,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 2000, ICHAIN_STOP),
};

void ObjSwitch_RotateY(Vec3f* dest, Vec3f* src, s16 angle) {
    f32 s = Math_SinS(angle);
    f32 c = Math_CosS(angle);

    dest->x = src->z * s + src->x * c;
    dest->y = src->y;
    dest->z = src->z * c - src->x * s;
}

void ObjSwitch_InitDynapoly(ObjSwitch* self, GlobalContext* globalCtx, CollisionHeader* collision, s32 moveFlag) {
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    DynaPolyActor_Init(&self->dyna, moveFlag);
    CollisionHeader_GetVirtual(collision, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    if (self->dyna.bgId == BG_ACTOR_MAX) {
        // "Warning : move BG registration failure"
        osSyncPrintf("Warning : move BG 登録失敗(%s %d)(name %d)(arg_data 0x%04x)\n", "../z_obj_switch.c", 531,
                     self->dyna.actor.id, self->dyna.actor.params);
    }
}

void ObjSwitch_InitJntSphCollider(ObjSwitch* self, GlobalContext* globalCtx, ColliderJntSphInit* colliderJntSphInit) {
    ColliderJntSph* colliderJntSph = &self->jntSph.col;

    Collider_InitJntSph(globalCtx, colliderJntSph);
    Collider_SetJntSph(globalCtx, colliderJntSph, &self->dyna.actor, colliderJntSphInit, self->jntSph.items);
    func_800D1694(self->dyna.actor.world.pos.x,
                  self->dyna.actor.world.pos.y + self->dyna.actor.shape.yOffset * self->dyna.actor.scale.y,
                  self->dyna.actor.world.pos.z, &self->dyna.actor.shape.rot);
    Matrix_Scale(self->dyna.actor.scale.x, self->dyna.actor.scale.y, self->dyna.actor.scale.z, MTXMODE_APPLY);
    Collider_UpdateSpheres(0, colliderJntSph);
}

void ObjSwitch_InitTrisCollider(ObjSwitch* self, GlobalContext* globalCtx, ColliderTrisInit* colliderTrisInit) {
    ColliderTris* colliderTris = &self->tris.col;
    s32 i;
    s32 j;
    Vec3f pos[3];

    Collider_InitTris(globalCtx, colliderTris);
    Collider_SetTris(globalCtx, colliderTris, &self->dyna.actor, colliderTrisInit, self->tris.items);

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            ObjSwitch_RotateY(&pos[j], &colliderTrisInit->elements[i].dim.vtx[j], self->dyna.actor.home.rot.y);
            Math_Vec3f_Sum(&pos[j], &self->dyna.actor.world.pos, &pos[j]);
        }

        Collider_SetTrisVertices(colliderTris, i, &pos[0], &pos[1], &pos[2]);
    }
}

Actor* ObjSwitch_SpawnIce(ObjSwitch* self, GlobalContext* globalCtx) {
    Actor* thisx = &self->dyna.actor;

    return Actor_SpawnAsChild(&globalCtx->actorCtx, thisx, globalCtx, ACTOR_OBJ_ICE_POLY, thisx->world.pos.x,
                              thisx->world.pos.y, thisx->world.pos.z, thisx->world.rot.x, thisx->world.rot.y,
                              thisx->world.rot.z, (self->dyna.actor.params >> 8 & 0x3F) << 8);
}

void ObjSwitch_SetOn(ObjSwitch* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 subType;

    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F))) {
        self->cooldownOn = false;
    } else {
        subType = (self->dyna.actor.params >> 4 & 7);
        Flags_SetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F));

        if (subType == 0 || subType == 4) {
            OnePointCutscene_AttentionSetSfx(globalCtx, &self->dyna.actor, NA_SE_SY_CORRECT_CHIME);
        } else {
            OnePointCutscene_AttentionSetSfx(globalCtx, &self->dyna.actor, NA_SE_SY_TRE_BOX_APPEAR);
        }

        self->cooldownOn = true;
    }
}

void ObjSwitch_SetOff(ObjSwitch* self, GlobalContext* globalCtx) {
    self->cooldownOn = false;

    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F))) {
        Flags_UnsetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F));

        if ((self->dyna.actor.params >> 4 & 7) == 1) {
            OnePointCutscene_AttentionSetSfx(globalCtx, &self->dyna.actor, NA_SE_SY_TRE_BOX_APPEAR);
            self->cooldownOn = true;
        }
    }
}

void ObjSwitch_UpdateTwoTexScrollXY(ObjSwitch* self) {
    self->x1TexScroll = (self->x1TexScroll - 1) & 0x7F;
    self->y1TexScroll = (self->y1TexScroll + 1) & 0x7F;
    self->x2TexScroll = (self->x2TexScroll + 1) & 0x7F;
    self->y2TexScroll = (self->y2TexScroll - 1) & 0x7F;
}

void ObjSwitch_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjSwitch* self = THIS;
    s32 switchFlagSet;
    s32 type;

    switchFlagSet = Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F));
    type = (self->dyna.actor.params & 7);

    if (type == OBJSWITCH_TYPE_FLOOR || type == OBJSWITCH_TYPE_FLOOR_RUSTY) {
        ObjSwitch_InitDynapoly(self, globalCtx, &gFloorSwitchCol, DPM_PLAYER);
    }

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);

    if (type == OBJSWITCH_TYPE_FLOOR || type == OBJSWITCH_TYPE_FLOOR_RUSTY) {
        self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y + 1.0f;
    }

    Actor_SetFocus(&self->dyna.actor, sHeights[type]);

    if (type == OBJSWITCH_TYPE_FLOOR_RUSTY) {
        ObjSwitch_InitTrisCollider(self, globalCtx, &sRustyFloorTrisInit);
    } else if (type == OBJSWITCH_TYPE_EYE) {
        ObjSwitch_InitTrisCollider(self, globalCtx, &trisColliderEye);
    } else if (type == OBJSWITCH_TYPE_CRYSTAL || type == OBJSWITCH_TYPE_CRYSTAL_TARGETABLE) {
        ObjSwitch_InitJntSphCollider(self, globalCtx, &sCyrstalJntSphereInit);
    }

    if (type == OBJSWITCH_TYPE_CRYSTAL_TARGETABLE) {
        self->dyna.actor.flags |= 1;
        self->dyna.actor.targetMode = 4;
    }

    self->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;

    if ((self->dyna.actor.params >> 7 & 1) && (ObjSwitch_SpawnIce(self, globalCtx) == NULL)) {
        osSyncPrintf(VT_FGCOL(RED));
        osSyncPrintf("Error : 氷発生失敗 (%s %d)\n", "../z_obj_switch.c", 732);
        osSyncPrintf(VT_RST);
        self->dyna.actor.params &= ~0x80;
    }

    if (self->dyna.actor.params >> 7 & 1) {
        ObjSwitch_EyeFrozenInit(self);
    } else if (type == OBJSWITCH_TYPE_FLOOR || type == OBJSWITCH_TYPE_FLOOR_RUSTY) {
        if (switchFlagSet) {
            ObjSwitch_FloorDownInit(self);
        } else {
            ObjSwitch_FloorUpInit(self);
        }
    } else if (type == OBJSWITCH_TYPE_EYE) {
        if (switchFlagSet) {
            ObjSwitch_EyeClosedInit(self);
        } else {
            ObjSwitch_EyeOpenInit(self);
        }
    } else if (type == OBJSWITCH_TYPE_CRYSTAL || type == OBJSWITCH_TYPE_CRYSTAL_TARGETABLE) {
        if (switchFlagSet) {
            ObjSwitch_CrystalOnInit(self);
        } else {
            ObjSwitch_CrystalOffInit(self);
        }
    }

    osSyncPrintf("(Dungeon switch)(arg_data 0x%04x)\n", self->dyna.actor.params);
}

void ObjSwitch_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ObjSwitch* self = THIS;

    switch ((self->dyna.actor.params & 7)) {
        case OBJSWITCH_TYPE_FLOOR:
        case OBJSWITCH_TYPE_FLOOR_RUSTY:
            DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            break;
    }

    switch ((self->dyna.actor.params & 7)) {
        case OBJSWITCH_TYPE_FLOOR_RUSTY:
        case OBJSWITCH_TYPE_EYE:
            Collider_DestroyTris(globalCtx, &self->tris.col);
            break;
        case OBJSWITCH_TYPE_CRYSTAL:
        case OBJSWITCH_TYPE_CRYSTAL_TARGETABLE:
            Collider_DestroyJntSph(globalCtx, &self->jntSph.col);
            break;
    }
}

void ObjSwitch_FloorUpInit(ObjSwitch* self) {
    self->dyna.actor.scale.y = 33.0f / 200.0f;
    self->actionFunc = ObjSwitch_FloorUp;
}

void ObjSwitch_FloorUp(ObjSwitch* self, GlobalContext* globalCtx) {
    if ((self->dyna.actor.params & 7) == OBJSWITCH_TYPE_FLOOR_RUSTY) {
        if (self->tris.col.base.acFlags & AC_HIT) {
            ObjSwitch_FloorPressInit(self);
            ObjSwitch_SetOn(self, globalCtx);
            self->tris.col.base.acFlags &= ~AC_HIT;
        } else {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->tris.col.base);
        }
    } else {
        switch ((self->dyna.actor.params >> 4 & 7)) {
            case OBJSWITCH_SUBTYPE_FLOOR_0:
                if (func_8004356C(&self->dyna)) {
                    ObjSwitch_FloorPressInit(self);
                    ObjSwitch_SetOn(self, globalCtx);
                }
                break;
            case OBJSWITCH_SUBTYPE_FLOOR_1:
                if ((self->dyna.unk_160 & 2) && !(self->unk_17F & 2)) {
                    ObjSwitch_FloorPressInit(self);
                    ObjSwitch_SetOn(self, globalCtx);
                }
                break;
            case OBJSWITCH_SUBTYPE_FLOOR_2:
                if (func_800435B4(&self->dyna)) {
                    ObjSwitch_FloorPressInit(self);
                    ObjSwitch_SetOn(self, globalCtx);
                }
                break;
            case OBJSWITCH_SUBTYPE_FLOOR_3:
                if (func_800435B4(&self->dyna)) {
                    ObjSwitch_FloorPressInit(self);
                    ObjSwitch_SetOff(self, globalCtx);
                }
                break;
        }
    }
}

void ObjSwitch_FloorPressInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_FloorPress;
    self->cooldownTimer = 100;
}

void ObjSwitch_FloorPress(ObjSwitch* self, GlobalContext* globalCtx) {
    if ((self->dyna.actor.params >> 4 & 7) == OBJSWITCH_SUBTYPE_FLOOR_3 || !self->cooldownOn ||
        func_8005B198() == self->dyna.actor.category || self->cooldownTimer <= 0) {
        self->dyna.actor.scale.y -= 99.0f / 2000.0f;
        if (self->dyna.actor.scale.y <= 33.0f / 2000.0f) {
            ObjSwitch_FloorDownInit(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
            func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 120, 20, 10);
        }
    }
}

void ObjSwitch_FloorDownInit(ObjSwitch* self) {
    self->dyna.actor.scale.y = 33.0f / 2000.0f;
    self->releaseTimer = 6;
    self->actionFunc = ObjSwitch_FloorDown;
}

void ObjSwitch_FloorDown(ObjSwitch* self, GlobalContext* globalCtx) {
    switch ((self->dyna.actor.params >> 4 & 7)) {
        case OBJSWITCH_SUBTYPE_FLOOR_0:
            if (!Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F))) {
                ObjSwitch_FloorReleaseInit(self);
            }
            break;
        case OBJSWITCH_SUBTYPE_FLOOR_1:
            if ((self->dyna.unk_160 & 2) && !(self->unk_17F & 2)) {
                ObjSwitch_FloorReleaseInit(self);
                ObjSwitch_SetOff(self, globalCtx);
            }
            break;
        case OBJSWITCH_SUBTYPE_FLOOR_2:
        case OBJSWITCH_SUBTYPE_FLOOR_3:
            if (!func_800435B4(&self->dyna) && !Player_InCsMode(globalCtx)) {
                if (self->releaseTimer <= 0) {
                    ObjSwitch_FloorReleaseInit(self);
                    if ((self->dyna.actor.params >> 4 & 7) == OBJSWITCH_SUBTYPE_FLOOR_2) {
                        ObjSwitch_SetOff(self, globalCtx);
                    } else {
                        ObjSwitch_SetOn(self, globalCtx);
                    }
                }
            } else {
                self->releaseTimer = 6;
            }
            break;
    }
}

void ObjSwitch_FloorReleaseInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_FloorRelease;
    self->cooldownTimer = 100;
}

void ObjSwitch_FloorRelease(ObjSwitch* self, GlobalContext* globalCtx) {
    s16 subType = (self->dyna.actor.params >> 4 & 7);

    if (((subType != OBJSWITCH_SUBTYPE_FLOOR_1) && (subType != OBJSWITCH_SUBTYPE_FLOOR_3)) || !self->cooldownOn ||
        func_8005B198() == self->dyna.actor.category || self->cooldownTimer <= 0) {
        self->dyna.actor.scale.y += 99.0f / 2000.0f;
        if (self->dyna.actor.scale.y >= 33.0f / 200.0f) {
            ObjSwitch_FloorUpInit(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
            if (subType == OBJSWITCH_SUBTYPE_FLOOR_1) {
                func_800AA000(self->dyna.actor.xyzDistToPlayerSq, 120, 20, 10);
            }
        }
    }
}

s32 ObjSwitch_EyeIsHit(ObjSwitch* self) {
    Actor* collidingActor;
    s16 yawDiff;

    if ((self->tris.col.base.acFlags & AC_HIT) && !(self->unk_17F & 2)) {
        collidingActor = self->tris.col.base.ac;
        if (collidingActor != NULL) {
            yawDiff = collidingActor->world.rot.y - self->dyna.actor.shape.rot.y;
            if (ABS(yawDiff) > 0x5000) {
                return 1;
            }
        }
    }
    return 0;
}

void ObjSwitch_EyeFrozenInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_EyeInit;
}

void ObjSwitch_EyeInit(ObjSwitch* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F))) {
        ObjSwitch_EyeClosedInit(self);
    } else {
        ObjSwitch_EyeOpenInit(self);
    }
}

void ObjSwitch_EyeOpenInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_EyeOpen;
    self->eyeTexIndex = 0;
}

void ObjSwitch_EyeOpen(ObjSwitch* self, GlobalContext* globalCtx) {
    if (ObjSwitch_EyeIsHit(self) || (self->dyna.actor.params >> 7 & 1)) {
        ObjSwitch_EyeClosingInit(self);
        ObjSwitch_SetOn(self, globalCtx);
        self->dyna.actor.params &= ~0x80;
    }
}

void ObjSwitch_EyeClosingInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_EyeClosing;
    self->cooldownTimer = 100;
}

void ObjSwitch_EyeClosing(ObjSwitch* self, GlobalContext* globalCtx) {
    if (!self->cooldownOn || func_8005B198() == self->dyna.actor.category || self->cooldownTimer <= 0) {
        self->eyeTexIndex++;
        if (self->eyeTexIndex >= 3) {
            ObjSwitch_EyeClosedInit(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
        }
    }
}

void ObjSwitch_EyeClosedInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_EyeClosed;
    self->eyeTexIndex = 3;
}

void ObjSwitch_EyeClosed(ObjSwitch* self, GlobalContext* globalCtx) {
    switch ((self->dyna.actor.params >> 4 & 7)) {
        case OBJSWITCH_SUBTYPE_EYE_0:
            if (!Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F))) {
                ObjSwitch_EyeOpeningInit(self);
                self->dyna.actor.params &= ~0x80;
            }
            break;
        case OBJSWITCH_SUBTYPE_EYE_1:
            if (ObjSwitch_EyeIsHit(self) || (self->dyna.actor.params >> 7 & 1)) {
                ObjSwitch_EyeOpeningInit(self);
                ObjSwitch_SetOff(self, globalCtx);
                self->dyna.actor.params &= ~0x80;
            }
            break;
    }
}

void ObjSwitch_EyeOpeningInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_EyeOpening;
    self->cooldownTimer = 100;
}

void ObjSwitch_EyeOpening(ObjSwitch* self, GlobalContext* globalCtx) {
    if ((self->dyna.actor.params >> 4 & 7) != OBJSWITCH_SUBTYPE_EYE_1 || !self->cooldownOn ||
        func_8005B198() == self->dyna.actor.category || self->cooldownTimer <= 0) {
        self->eyeTexIndex--;
        if (self->eyeTexIndex <= 0) {
            ObjSwitch_EyeOpenInit(self);
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_FOOT_SWITCH);
        }
    }
}

void ObjSwitch_CrystalOffInit(ObjSwitch* self) {
    self->crystalColor.r = 0;
    self->crystalColor.g = 0;
    self->crystalColor.b = 0;
    self->crystalSubtype1texture = gCrstalSwitchRedTex;
    self->actionFunc = ObjSwitch_CrystalOff;
}

void ObjSwitch_CrystalOff(ObjSwitch* self, GlobalContext* globalCtx) {
    switch ((self->dyna.actor.params >> 4 & 7)) {
        case OBJSWITCH_SUBTYPE_CRYSTAL_0:
            if ((self->jntSph.col.base.acFlags & AC_HIT) && self->disableAcTimer <= 0) {
                self->disableAcTimer = 10;
                ObjSwitch_SetOn(self, globalCtx);
                ObjSwitch_CrystalTurnOnInit(self);
            }
            break;
        case OBJSWITCH_SUBTYPE_CRYSTAL_4:
            if (((self->jntSph.col.base.acFlags & AC_HIT) && self->disableAcTimer <= 0) ||
                Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F))) {
                self->disableAcTimer = 10;
                ObjSwitch_SetOn(self, globalCtx);
                ObjSwitch_CrystalTurnOnInit(self);
            }
            break;
        case OBJSWITCH_SUBTYPE_CRYSTAL_1:
            if ((self->jntSph.col.base.acFlags & AC_HIT) && !(self->unk_17F & 2) && self->disableAcTimer <= 0) {
                self->disableAcTimer = 10;
                ObjSwitch_SetOn(self, globalCtx);
                ObjSwitch_CrystalTurnOnInit(self);
            }
            ObjSwitch_UpdateTwoTexScrollXY(self);
            break;
    }
}

void ObjSwitch_CrystalTurnOnInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_CrystalTurnOn;
    self->cooldownTimer = 100;
}

void ObjSwitch_CrystalTurnOn(ObjSwitch* self, GlobalContext* globalCtx) {
    if (!self->cooldownOn || func_8005B198() == self->dyna.actor.category || self->cooldownTimer <= 0) {
        ObjSwitch_CrystalOnInit(self);
        if ((self->dyna.actor.params >> 4 & 7) == OBJSWITCH_SUBTYPE_CRYSTAL_1) {
            ObjSwitch_UpdateTwoTexScrollXY(self);
        }
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_DIAMOND_SWITCH);
    }
}

void ObjSwitch_CrystalOnInit(ObjSwitch* self) {
    self->crystalColor.r = 255;
    self->crystalColor.g = 255;
    self->crystalColor.b = 255;
    self->crystalSubtype1texture = gCrstalSwitchBlueTex;
    self->actionFunc = ObjSwitch_CrystalOn;
}

void ObjSwitch_CrystalOn(ObjSwitch* self, GlobalContext* globalCtx) {
    switch ((self->dyna.actor.params >> 4 & 7)) {
        case OBJSWITCH_SUBTYPE_CRYSTAL_0:
        case OBJSWITCH_SUBTYPE_CRYSTAL_4:
            if (!Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8 & 0x3F))) {
                ObjSwitch_CrystalTurnOffInit(self);
            }
            break;
        case OBJSWITCH_SUBTYPE_CRYSTAL_1:
            if ((self->jntSph.col.base.acFlags & AC_HIT) && !(self->unk_17F & 2) && self->disableAcTimer <= 0) {
                self->disableAcTimer = 10;
                globalCtx = globalCtx;
                ObjSwitch_CrystalTurnOffInit(self);
                ObjSwitch_SetOff(self, globalCtx);
            }
            break;
    }
    ObjSwitch_UpdateTwoTexScrollXY(self);
}

void ObjSwitch_CrystalTurnOffInit(ObjSwitch* self) {
    self->actionFunc = ObjSwitch_CrystalTurnOff;
    self->cooldownTimer = 100;
}

void ObjSwitch_CrystalTurnOff(ObjSwitch* self, GlobalContext* globalCtx) {
    if ((self->dyna.actor.params >> 4 & 7) != OBJSWITCH_SUBTYPE_CRYSTAL_1 || !self->cooldownOn ||
        func_8005B198() == self->dyna.actor.category || self->cooldownTimer <= 0) {
        ObjSwitch_CrystalOffInit(self);
        ObjSwitch_UpdateTwoTexScrollXY(self);
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_DIAMOND_SWITCH);
    }
}

void ObjSwitch_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjSwitch* self = THIS;

    if (self->releaseTimer > 0) {
        self->releaseTimer--;
    }
    if (self->cooldownTimer > 0) {
        self->cooldownTimer--;
    }

    self->actionFunc(self, globalCtx);

    switch ((self->dyna.actor.params & 7)) {
        case OBJSWITCH_TYPE_FLOOR:
        case OBJSWITCH_TYPE_FLOOR_RUSTY:
            self->unk_17F = self->dyna.unk_160;
            break;
        case OBJSWITCH_TYPE_EYE:
            self->unk_17F = self->tris.col.base.acFlags;
            self->tris.col.base.acFlags &= ~AC_HIT;
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->tris.col.base);
            break;
        case OBJSWITCH_TYPE_CRYSTAL:
        case OBJSWITCH_TYPE_CRYSTAL_TARGETABLE:
            if (!Player_InCsMode(globalCtx) && self->disableAcTimer > 0) {
                self->disableAcTimer--;
            }
            self->unk_17F = self->jntSph.col.base.acFlags;
            self->jntSph.col.base.acFlags &= ~AC_HIT;
            if (self->disableAcTimer <= 0) {
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->jntSph.col.base);
            }
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->jntSph.col.base);
            break;
    }
}

void ObjSwitch_DrawFloor(ObjSwitch* self, GlobalContext* globalCtx) {
    static Gfx* floorSwitchDLists[] = { gFloorSwitch1DL, gFloorSwitch3DL, gFloorSwitch2DL, gFloorSwitch2DL };

    Gfx_DrawDListOpa(globalCtx, floorSwitchDLists[(self->dyna.actor.params >> 4 & 7)]);
}

void ObjSwitch_DrawFloorRusty(ObjSwitch* self, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, gRustyFloorSwitchDL);
}

void ObjSwitch_DrawEye(ObjSwitch* self, GlobalContext* globalCtx) {
    static void* eyeTextures[][4] = {
        { gEyeSwitchGoldOpenTex, gEyeSwitchGoldOpeningTex, gEyeSwitchGoldClosingTex, gEyeSwitchGoldClosedTex },
        { gEyeSwitchSilverOpenTex, gEyeSwitchSilverHalfTex, gEyeSwitchSilverClosedTex, gEyeSwitchSilverClosedTex },
    };
    static Gfx* eyeDlists[] = { gEyeSwitch1DL, gEyeSwitch2DL };
    s32 pad;
    s32 subType = (self->dyna.actor.params >> 4 & 7);

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_switch.c", 1459);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_switch.c", 1462),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[subType][self->eyeTexIndex]));
    gSPDisplayList(POLY_OPA_DISP++, eyeDlists[subType]);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_switch.c", 1471);
}

void ObjSwitch_DrawCrystal(ObjSwitch* self, GlobalContext* globalCtx) {
    static Gfx* xluDLists[] = { gCrystalSwitchCoreXluDL, gCrystalSwitchDiamondXluDL, NULL, NULL,
                                gCrystalSwitchCoreXluDL };
    static Gfx* opaDLists[] = { gCrystalSwitchCoreOpaDL, gCrystalSwitchDiamondOpaDL, NULL, NULL,
                                gCrystalSwitchCoreOpaDL };
    s32 pad1;
    s32 pad2;
    s32 subType;

    subType = (self->dyna.actor.params >> 4 & 7);
    func_8002ED80(&self->dyna.actor, globalCtx, 0);

    if (1) {}

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_switch.c", 1494);

    func_80093D84(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_switch.c", 0x5D9),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, xluDLists[subType]);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_switch.c", 1502);

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_switch.c", 1507);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_switch.c", 1511),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (subType == OBJSWITCH_SUBTYPE_CRYSTAL_1) {
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(self->crystalSubtype1texture));
    }

    gDPSetEnvColor(POLY_OPA_DISP++, self->crystalColor.r, self->crystalColor.g, self->crystalColor.b, 128);
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, self->x1TexScroll, self->y1TexScroll, 0x20, 0x20, 1,
                                self->x2TexScroll, self->y2TexScroll, 0x20, 0x20));
    gSPDisplayList(POLY_OPA_DISP++, opaDLists[subType]);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_switch.c", 1533);
}

static ObjSwitchActionFunc sDrawFuncs[] = {
    ObjSwitch_DrawFloor, ObjSwitch_DrawFloorRusty, ObjSwitch_DrawEye, ObjSwitch_DrawCrystal, ObjSwitch_DrawCrystal,
};

void ObjSwitch_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ObjSwitch* self = THIS;

    sDrawFuncs[(self->dyna.actor.params & 7)](self, globalCtx);
}
