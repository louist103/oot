/*
 * File: z_door_ana.c
 * Overlay: ovl_Door_Ana
 * Description: Grottos Entrances/Exits
 */

#include "z_door_ana.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"

#define FLAGS 0x02000000

#define THIS ((DoorAna*)thisx)

void DoorAna_Init(Actor* thisx, GlobalContext* globalCtx);
void DoorAna_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DoorAna_Update(Actor* thisx, GlobalContext* globalCtx);
void DoorAna_Draw(Actor* thisx, GlobalContext* globalCtx);

void DoorAna_WaitClosed(DoorAna* self, GlobalContext* globalCtx);
void DoorAna_WaitOpen(DoorAna* self, GlobalContext* globalCtx);
void DoorAna_GrabPlayer(DoorAna* self, GlobalContext* globalCtx);

const ActorInit Door_Ana_InitVars = {
    ACTOR_DOOR_ANA,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(DoorAna),
    (ActorFunc)DoorAna_Init,
    (ActorFunc)DoorAna_Destroy,
    (ActorFunc)DoorAna_Update,
    (ActorFunc)DoorAna_Draw,
};

static ColliderCylinderInit sCylinderInit = {
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
        { 0x00000048, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 50, 10, 0, { 0 } },
};

// array of entrance table entries to grotto destinations
static s16 entrances[] = {
    0x036D, 0x003F, 0x0598, 0x059C, 0x05A0, 0x05A4, 0x05A8, 0x05AC,
    0x05B0, 0x05B4, 0x05B8, 0x05BC, 0x05C0, 0x05C4, 0x05FC,
};

void DoorAna_SetupAction(DoorAna* self, DoorAnaActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void DoorAna_Init(Actor* thisx, GlobalContext* globalCtx) {
    DoorAna* self = THIS;

    self->actor.shape.rot.z = 0;
    self->actor.shape.rot.y = self->actor.shape.rot.z;
    // init block for grottos that are initially "hidden" (require explosives/hammer/song of storms to open)
    if ((self->actor.params & 0x300) != 0) {
        // only allocate collider for grottos that need bombing/hammering open
        if ((self->actor.params & 0x200) != 0) {
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        } else {
            self->actor.flags |= 0x10;
        }
        Actor_SetScale(&self->actor, 0);
        DoorAna_SetupAction(self, DoorAna_WaitClosed);
    } else {
        DoorAna_SetupAction(self, DoorAna_WaitOpen);
    }
    self->actor.targetMode = 0;
}

void DoorAna_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    DoorAna* self = THIS;

    // free collider if it has one
    if ((self->actor.params & 0x200) != 0) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

// update routine for grottos that are currently "hidden"/unopened
void DoorAna_WaitClosed(DoorAna* self, GlobalContext* globalCtx) {
    u32 openGrotto = false;
    if (!(self->actor.params & 0x200)) {
        // opening with song of storms
        if (self->actor.xyzDistToPlayerSq < 40000.0f && Flags_GetEnv(globalCtx, 5)) {
            openGrotto = true;
            self->actor.flags &= ~0x10;
        }
    } else {
        // bombing/hammering open a grotto
        if (self->collider.base.acFlags & AC_HIT) {
            openGrotto = true;
            Collider_DestroyCylinder(globalCtx, &self->collider);
        } else {
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
    // open the grotto
    if (openGrotto) {
        self->actor.params &= ~0x0300;
        DoorAna_SetupAction(self, DoorAna_WaitOpen);
        Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
    func_8002F5F0(&self->actor, globalCtx);
}

// update routine for grottos that are open
void DoorAna_WaitOpen(DoorAna* self, GlobalContext* globalCtx) {
    Player* player;
    s32 destinationIdx;

    player = GET_PLAYER(globalCtx);
    if (Math_StepToF(&self->actor.scale.x, 0.01f, 0.001f)) {
        if ((self->actor.targetMode != 0) && (globalCtx->sceneLoadFlag == 0) && (player->stateFlags1 & 0x80000000) &&
            (player->unk_84F == 0)) {
            destinationIdx = ((self->actor.params >> 0xC) & 7) - 1;
            Gameplay_SetupRespawnPoint(globalCtx, RESPAWN_MODE_RETURN, 0x4FF);
            gSaveContext.respawn[RESPAWN_MODE_RETURN].pos.y = self->actor.world.pos.y;
            gSaveContext.respawn[RESPAWN_MODE_RETURN].yaw = self->actor.home.rot.y;
            gSaveContext.respawn[RESPAWN_MODE_RETURN].data = self->actor.params & 0xFFFF;
            if (destinationIdx < 0) {
                destinationIdx = self->actor.home.rot.z + 1;
            }
            globalCtx->nextEntranceIndex = entrances[destinationIdx];
            DoorAna_SetupAction(self, DoorAna_GrabPlayer);
        } else {
            if (!Player_InCsMode(globalCtx) && !(player->stateFlags1 & 0x8800000) &&
                self->actor.xzDistToPlayer <= 15.0f && -50.0f <= self->actor.yDistToPlayer &&
                self->actor.yDistToPlayer <= 15.0f) {
                player->stateFlags1 |= 0x80000000;
                self->actor.targetMode = 1;
            } else {
                self->actor.targetMode = 0;
            }
        }
    }
    Actor_SetScale(&self->actor, self->actor.scale.x);
}

// update function for after the player has triggered the grotto
void DoorAna_GrabPlayer(DoorAna* self, GlobalContext* globalCtx) {
    Player* player;

    if (self->actor.yDistToPlayer <= 0.0f && 15.0f < self->actor.xzDistToPlayer) {
        player = GET_PLAYER(globalCtx);
        player->actor.world.pos.x = Math_SinS(self->actor.yawTowardsPlayer) * 15.0f + self->actor.world.pos.x;
        player->actor.world.pos.z = Math_CosS(self->actor.yawTowardsPlayer) * 15.0f + self->actor.world.pos.z;
    }
}

void DoorAna_Update(Actor* thisx, GlobalContext* globalCtx) {
    DoorAna* self = THIS;

    self->actionFunc(self, globalCtx);
    // changes the grottos facing angle based on camera angle
    self->actor.shape.rot.y = Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000;
}

void DoorAna_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_door_ana.c", 440);

    func_80093D84(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_door_ana.c", 446),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gGrottoDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_door_ana.c", 449);
}
