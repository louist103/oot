/*
 * File: z_obj_timeblock.c
 * Overlay: ovl_Obj_Timeblock
 * Description: Song of Time Block
 */

#include "z_obj_timeblock.h"
#include "objects/object_timeblock/object_timeblock.h"

#define FLAGS 0x0A000011

#define THIS ((ObjTimeblock*)thisx)

void ObjTimeblock_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjTimeblock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjTimeblock_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjTimeblock_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjTimeblock_SetupNormal(ObjTimeblock* self);
void ObjTimeblock_SetupAltBehaviorVisible(ObjTimeblock* self);
void ObjTimeblock_SetupAltBehaviourNotVisible(ObjTimeblock* self);

s32 ObjTimeblock_WaitForOcarina(ObjTimeblock* self, GlobalContext* globalCtx);
s32 ObjTimeblock_WaitForSong(ObjTimeblock* self, GlobalContext* globalCtx);
void ObjTimeblock_DoNothing(ObjTimeblock* self, GlobalContext* globalCtx);
void ObjTimeblock_Normal(ObjTimeblock* self, GlobalContext* globalCtx);
void ObjTimeblock_AltBehaviorVisible(ObjTimeblock* self, GlobalContext* globalCtx);
void ObjTimeblock_AltBehaviourNotVisible(ObjTimeblock* self, GlobalContext* globalCtx);

const ActorInit Obj_Timeblock_InitVars = {
    ACTOR_OBJ_TIMEBLOCK,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_TIMEBLOCK,
    sizeof(ObjTimeblock),
    (ActorFunc)ObjTimeblock_Init,
    (ActorFunc)ObjTimeblock_Destroy,
    (ActorFunc)ObjTimeblock_Update,
    (ActorFunc)ObjTimeblock_Draw,
};

typedef struct {
    /* 0x00 */ f32 scale;
    /* 0x04 */ f32 height;
    /* 0x08 */ s16 demoEffectParams;
} ObjTimeblockSizeOptions; // size = 0x0C

static ObjTimeblockSizeOptions sSizeOptions[] = {
    { 1.0, 60.0, 0x0018 },
    { 0.60, 40.0, 0x0019 },
};

static f32 sRanges[] = { 60.0, 100.0, 140.0, 180.0, 220.0, 260.0, 300.0, 300.0 };

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_STOP),
};

static Color_RGB8 sPrimColors[] = {
    { 100, 120, 140 }, { 80, 140, 200 }, { 100, 150, 200 }, { 100, 200, 240 },
    { 80, 110, 140 },  { 70, 160, 225 }, { 80, 100, 130 },  { 100, 110, 190 },
};

u32 ObjTimeblock_CalculateIsVisible(ObjTimeblock* self) {
    if (!((self->dyna.actor.params >> 10) & 1)) {
        if (self->unk_177 == 0) {
            return self->unk_175;
        } else {
            u8 temp = ((self->dyna.actor.params >> 15) & 1) ? true : false;
            if (self->unk_177 == 1) {
                return self->unk_174 ^ temp;
            } else {
                u8 linkIsYoung = (LINK_AGE_IN_YEARS == YEARS_CHILD) ? true : false;
                return self->unk_174 ^ temp ^ linkIsYoung;
            }
        }
    } else {
        return (((self->dyna.actor.params >> 15) & 1) ? true : false) ^ self->unk_174;
    }
}

void ObjTimeblock_SpawnDemoEffect(ObjTimeblock* self, GlobalContext* globalCtx) {
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, self->dyna.actor.world.pos.x,
                self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z, 0, 0, 0,
                sSizeOptions[(self->dyna.actor.params >> 8) & 1].demoEffectParams);
}

void ObjTimeblock_ToggleSwitchFlag(GlobalContext* globalCtx, s32 flag) {
    if (Flags_GetSwitch(globalCtx, flag)) {
        Flags_UnsetSwitch(globalCtx, flag);
    } else {
        Flags_SetSwitch(globalCtx, flag);
    }
}

void ObjTimeblock_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjTimeblock* self = THIS;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    self->dyna.actor.world.rot.z = self->dyna.actor.shape.rot.z = 0;

    CollisionHeader_GetVirtual(&gSongOfTimeBlockCol, &colHeader);

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    Actor_SetScale(&self->dyna.actor, sSizeOptions[(self->dyna.actor.params >> 8) & 1].scale);

    if ((self->dyna.actor.params >> 6) & 1) {
        self->unk_177 = 0;
    } else {
        self->unk_177 = ((self->dyna.actor.params & 0x3F) < 0x38) ? 2 : 1;
    }

    self->songObserverFunc = ObjTimeblock_WaitForOcarina;

    Actor_SetFocus(&self->dyna.actor, sSizeOptions[(self->dyna.actor.params >> 8) & 1].height);

    self->unk_174 = (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) ? true : false;
    self->unk_175 = ((self->dyna.actor.params >> 15) & 1) ? true : false;
    self->isVisible = ObjTimeblock_CalculateIsVisible(self);

    if (!((self->dyna.actor.params >> 10) & 1)) {
        ObjTimeblock_SetupNormal(self);
    } else if (self->isVisible) {
        ObjTimeblock_SetupAltBehaviorVisible(self);
    } else {
        ObjTimeblock_SetupAltBehaviourNotVisible(self);
    }

    // "Block of time"
    osSyncPrintf("時のブロック (<arg> %04xH <type> save:%d color:%d range:%d move:%d)\n", (u16)self->dyna.actor.params,
                 self->unk_177, self->dyna.actor.home.rot.z & 7, (self->dyna.actor.params >> 11) & 7,
                 (self->dyna.actor.params >> 10) & 1);
}

void ObjTimeblock_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    ObjTimeblock* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

u8 ObjTimeblock_PlayerIsInRange(ObjTimeblock* self, GlobalContext* globalCtx) {
    if (self->isVisible && func_80043590(&self->dyna)) {
        return false;
    }

    if (self->dyna.actor.xzDistToPlayer <= sRanges[(self->dyna.actor.params >> 11) & 7]) {
        Vec3f distance;
        f32 blockSize;

        func_8002DBD0(&self->dyna.actor, &distance, &GET_PLAYER(globalCtx)->actor.world.pos);
        blockSize = self->dyna.actor.scale.x * 50.0f + 6.0f;
        // Return true if player's xz position is not inside the block
        if (blockSize < fabsf(distance.x) || blockSize < fabsf(distance.z)) {
            return true;
        }
    }

    return false;
}

s32 ObjTimeblock_WaitForOcarina(ObjTimeblock* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (ObjTimeblock_PlayerIsInRange(self, globalCtx)) {
        if (player->stateFlags2 & 0x1000000) {
            func_8010BD58(globalCtx, 1);
            self->songObserverFunc = ObjTimeblock_WaitForSong;
        } else {
            player->stateFlags2 |= 0x800000;
        }
    }
    return false;
}

s32 ObjTimeblock_WaitForSong(ObjTimeblock* self, GlobalContext* globalCtx) {
    if (globalCtx->msgCtx.unk_E3EE == 4) {
        self->songObserverFunc = ObjTimeblock_WaitForOcarina;
    }
    if (globalCtx->msgCtx.unk_E3EC == 10) {
        if (self->unk_172 == 254) {
            self->songEndTimer = 110;
        } else {
            self->songEndTimer--;
            if (self->songEndTimer == 0) {
                return true;
            }
        }
    }
    return false;
}

void ObjTimeblock_SetupDoNothing(ObjTimeblock* self) {
    self->actionFunc = ObjTimeblock_DoNothing;
}

void ObjTimeblock_DoNothing(ObjTimeblock* self, GlobalContext* globalCtx) {
}

void ObjTimeblock_SetupNormal(ObjTimeblock* self) {
    self->actionFunc = ObjTimeblock_Normal;
}

void ObjTimeblock_Normal(ObjTimeblock* self, GlobalContext* globalCtx) {
    u32 newIsVisible;

    if (self->songObserverFunc(self, globalCtx) && self->demoEffectTimer <= 0) {
        ObjTimeblock_SpawnDemoEffect(self, globalCtx);
        self->demoEffectTimer = 160;

        // Possibly points the camera to self actor
        OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        // "◯◯◯◯ Time Block Attention Camera (frame counter  %d)\n"
        osSyncPrintf("◯◯◯◯ Time Block 注目カメラ (frame counter  %d)\n", globalCtx->state.frames);

        self->demoEffectFirstPartTimer = 12;

        if (self->unk_177 == 0) {
            self->dyna.actor.params ^= 0x8000;
        } else {
            ObjTimeblock_ToggleSwitchFlag(globalCtx, self->dyna.actor.params & 0x3F);
        }
    }

    self->unk_172 = globalCtx->msgCtx.unk_E3EC;
    if (self->demoEffectFirstPartTimer > 0) {
        self->demoEffectFirstPartTimer--;
        if (self->demoEffectFirstPartTimer == 0) {
            if (self->unk_177 == 0) {
                self->unk_175 = ((self->dyna.actor.params >> 15) & 1) ? true : false;
            } else {
                self->unk_174 = (Flags_GetSwitch(globalCtx, self->dyna.actor.params & 0x3F)) ? true : false;
            }
        }
    }

    newIsVisible = ObjTimeblock_CalculateIsVisible(self);
    if (self->unk_177 == 1 && newIsVisible != self->isVisible) {
        ObjTimeblock_SetupDoNothing(self);
    }
    self->isVisible = newIsVisible;

    if (self->demoEffectTimer == 50) {
        func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
    }
}

void func_80BA06AC(ObjTimeblock* self, GlobalContext* globalCtx) {
    s32 switchFlag = self->dyna.actor.params & 0x3F;
    self->unk_172 = globalCtx->msgCtx.unk_E3EC;

    if (self->demoEffectFirstPartTimer > 0 && --self->demoEffectFirstPartTimer == 0) {
        self->unk_174 = (Flags_GetSwitch(globalCtx, switchFlag)) ? true : false;
    }

    self->isVisible = ObjTimeblock_CalculateIsVisible(self);
    self->unk_176 = (Flags_GetSwitch(globalCtx, switchFlag)) ? true : false;
}

void ObjTimeblock_SetupAltBehaviorVisible(ObjTimeblock* self) {
    self->actionFunc = ObjTimeblock_AltBehaviorVisible;
}

void ObjTimeblock_AltBehaviorVisible(ObjTimeblock* self, GlobalContext* globalCtx) {
    if (self->songObserverFunc(self, globalCtx) && self->demoEffectTimer <= 0) {
        self->demoEffectFirstPartTimer = 12;
        ObjTimeblock_SpawnDemoEffect(self, globalCtx);
        self->demoEffectTimer = 160;
        OnePointCutscene_Attention(globalCtx, &self->dyna.actor);
        // "Time Block Attention Camera (frame counter)"
        osSyncPrintf("◯◯◯◯ Time Block 注目カメラ (frame counter  %d)\n", globalCtx->state.frames);
        ObjTimeblock_ToggleSwitchFlag(globalCtx, self->dyna.actor.params & 0x3F);
    }

    func_80BA06AC(self, globalCtx);

    if (self->demoEffectTimer == 50) {
        func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
    }

    if (!self->isVisible && self->demoEffectTimer <= 0) {
        ObjTimeblock_SetupAltBehaviourNotVisible(self);
    }
}

void ObjTimeblock_SetupAltBehaviourNotVisible(ObjTimeblock* self) {
    self->actionFunc = ObjTimeblock_AltBehaviourNotVisible;
}

void ObjTimeblock_AltBehaviourNotVisible(ObjTimeblock* self, GlobalContext* globalCtx) {
    s32 switchFlag = self->dyna.actor.params & 0x3F;
    s8 switchFlagIsSet = (Flags_GetSwitch(globalCtx, switchFlag)) ? true : false;

    if (self->unk_176 ^ switchFlagIsSet && switchFlagIsSet ^ (((self->dyna.actor.params >> 15) & 1) ? true : false)) {
        if (self->demoEffectTimer <= 0) {
            ObjTimeblock_SpawnDemoEffect(self, globalCtx);
            self->demoEffectTimer = 160;
        }
        self->demoEffectFirstPartTimer = 12;
    }

    func_80BA06AC(self, globalCtx);

    if (self->isVisible && self->demoEffectTimer <= 0) {
        ObjTimeblock_SetupAltBehaviorVisible(self);
    }
}

void ObjTimeblock_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjTimeblock* self = THIS;

    self->actionFunc(self, globalCtx);

    if (self->demoEffectTimer > 0) {
        self->demoEffectTimer--;
    }

    if (self->isVisible) {
        func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    } else {
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
}

void ObjTimeblock_Draw(Actor* thisx, GlobalContext* globalCtx) {
    if (((ObjTimeblock*)thisx)->isVisible) {
        Color_RGB8* primColor = &sPrimColors[thisx->home.rot.z & 7];

        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_timeblock.c", 762);

        func_80093D18(globalCtx->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_timeblock.c", 766),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, primColor->r, primColor->g, primColor->b, 255);
        gSPDisplayList(POLY_OPA_DISP++, gSongOfTimeBlockDL);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_timeblock.c", 772);
    }
}
