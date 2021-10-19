/*
 * File: z_door_killer.c
 * Overlay: ovl_Door_Killer
 * Description: Fake doors which attack player
 */

#include "z_door_killer.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"
#include "objects/object_mizu_objects/object_mizu_objects.h"
#include "objects/object_haka_door/object_haka_door.h"
#include "objects/object_door_killer/object_door_killer.h"

#define FLAGS 0x00000010

#define THIS ((DoorKiller*)thisx)

typedef enum {
    /* 0 */ DOOR_KILLER_DOOR,
    /* 1 */ DOOR_KILLER_RUBBLE_PIECE_1,
    /* 2 */ DOOR_KILLER_RUBBLE_PIECE_2,
    /* 3 */ DOOR_KILLER_RUBBLE_PIECE_3,
    /* 4 */ DOOR_KILLER_RUBBLE_PIECE_4
} DoorKillerBehaviour;

void DoorKiller_Init(Actor* thisx, GlobalContext* globalCtx);
void DoorKiller_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DoorKiller_Update(Actor* thisx, GlobalContext* globalCtx);
void DoorKiller_Wait(DoorKiller* self, GlobalContext* globalCtx);
void DoorKiller_SetProperties(DoorKiller* self, GlobalContext* globalCtx);
void DoorKiller_DrawDoor(Actor* thisx, GlobalContext* globalCtx);
void DoorKiller_DrawRubble(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Door_Killer_InitVars = {
    ACTOR_DOOR_KILLER,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_DOOR_KILLER,
    sizeof(DoorKiller),
    (ActorFunc)DoorKiller_Init,
    (ActorFunc)DoorKiller_Destroy,
    (ActorFunc)DoorKiller_Update,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x10 },
        { 0x0001FFEE, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 20, 100, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphItemsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000008, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 100 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphItemsInit,
};

static DoorKillerTextureEntry sDoorTextures[4] = {
    { OBJECT_HIDAN_OBJECTS, gFireTempleDoorKillerTex },
    { OBJECT_MIZU_OBJECTS, object_mizu_objects_Tex_0035C0 },
    { OBJECT_HAKA_DOOR, object_haka_door_Tex_000000 },
    { OBJECT_GAMEPLAY_KEEP, gWoodenDoorTex },
};

void DoorKiller_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    f32 randF;
    DoorKiller* self = THIS;
    s32 bankIndex;
    s32 i;

    // Look in the object bank for one of the four objects containing door textures
    bankIndex = -1;
    for (i = 0; bankIndex < 0; i++) {
        bankIndex = Object_GetIndex(&globalCtx->objectCtx, sDoorTextures[i].objectId);
        self->textureEntryIndex = i;
    }
    osSyncPrintf("bank_ID = %d\n", bankIndex);
    osSyncPrintf("status = %d\n", self->textureEntryIndex);
    self->doorObjBankIndex = bankIndex;
    self->texture = sDoorTextures[self->textureEntryIndex].texture;

    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    Actor_SetScale(&self->actor, 0.01f);
    self->timer = 0;
    self->hasHitPlayerOrGround = 0;
    self->animStyle = 0;
    self->playerIsOpening = 0;

    switch ((u8)(self->actor.params & 0xFF)) {
        case DOOR_KILLER_DOOR:
            // `jointTable` is used for both the `jointTable` and `morphTable` args here. Because self actor doesn't
            // play any animations it does not cause problems, but it would need to be changed otherwise.
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_door_killer_Skel_001BC8, NULL, self->jointTable,
                               self->jointTable, 9);
            self->actionFunc = DoorKiller_SetProperties;
            DoorKiller_SetProperties(self, globalCtx);

            // manually set the overall rotation of the door
            self->jointTable[1].x = self->jointTable[1].z = 0x4000;

            // Set a cylinder collider to detect link attacks and larger sphere collider to detect explosions
            Collider_InitCylinder(globalCtx, &self->colliderCylinder);
            Collider_SetCylinder(globalCtx, &self->colliderCylinder, &self->actor, &sCylinderInit);
            Collider_InitJntSph(globalCtx, &self->colliderJntSph);
            Collider_SetJntSph(globalCtx, &self->colliderJntSph, &self->actor, &sJntSphInit, self->colliderJntSphItems);
            self->colliderJntSph.elements[0].dim.worldSphere.radius = 80;
            self->colliderJntSph.elements[0].dim.worldSphere.center.x = (s16)self->actor.world.pos.x;
            self->colliderJntSph.elements[0].dim.worldSphere.center.y = (s16)self->actor.world.pos.y + 50;
            self->colliderJntSph.elements[0].dim.worldSphere.center.z = (s16)self->actor.world.pos.z;

            // If tied to a switch flag and that switch flag is already set, kill the actor.
            if ((((self->actor.params >> 8) & 0x3F) != 0x3F) &&
                Flags_GetSwitch(globalCtx, ((self->actor.params >> 8) & 0x3F))) {
                Actor_Kill(&self->actor);
            }
            break;
        case DOOR_KILLER_RUBBLE_PIECE_1:
        case DOOR_KILLER_RUBBLE_PIECE_2:
        case DOOR_KILLER_RUBBLE_PIECE_3:
        case DOOR_KILLER_RUBBLE_PIECE_4:
            self->actionFunc = DoorKiller_SetProperties;
            DoorKiller_SetProperties(self, globalCtx);

            self->actor.gravity = -0.6f;
            self->actor.minVelocityY = -6.0f;

            // Random trajectories for rubble pieces
            randF = Rand_CenteredFloat(8.0f);
            self->actor.velocity.z = Rand_ZeroFloat(8.0f);
            self->actor.velocity.x = (Math_CosS(self->actor.world.rot.y) * randF) +
                                     (Math_SinS(self->actor.world.rot.y) * self->actor.velocity.z);
            self->actor.velocity.z = (-Math_SinS(self->actor.world.rot.y) * randF) +
                                     (Math_CosS(self->actor.world.rot.y) * self->actor.velocity.z);
            self->actor.velocity.y = Rand_ZeroFloat(4.0f) + 4.0f;

            // These are used as the x,y,z rotational velocities in DoorKiller_FallAsRubble
            self->actor.world.rot.x = Rand_CenteredFloat(0x1000);
            self->actor.world.rot.y = Rand_CenteredFloat(0x1000);
            self->actor.world.rot.z = Rand_CenteredFloat(0x1000);
            self->timer = 80;
            break;
    }
}

void DoorKiller_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    DoorKiller* self = THIS;

    if ((thisx->params & 0xFF) == DOOR_KILLER_DOOR) {
        Collider_DestroyCylinder(globalCtx, &self->colliderCylinder);
        Collider_DestroyJntSph(globalCtx, &self->colliderJntSph);
    }
}

void DoorKiller_SpawnRubble(Actor* thisx, GlobalContext* globalCtx) {
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_KILLER, thisx->world.pos.x, thisx->world.pos.y + 9.0f,
                thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y, thisx->shape.rot.z,
                DOOR_KILLER_RUBBLE_PIECE_1);
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_KILLER, thisx->world.pos.x + 7.88f,
                thisx->world.pos.y + 39.8f, thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y,
                thisx->shape.rot.z, DOOR_KILLER_RUBBLE_PIECE_2);
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_KILLER, thisx->world.pos.x - 15.86f,
                thisx->world.pos.y + 61.98f, thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y,
                thisx->shape.rot.z, DOOR_KILLER_RUBBLE_PIECE_3);
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_KILLER, thisx->world.pos.x + 3.72f,
                thisx->world.pos.y + 85.1f, thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y,
                thisx->shape.rot.z, DOOR_KILLER_RUBBLE_PIECE_4);
}

/**
 * action function for the individual door pieces that spawn and fall down when the door is destroyed
 */
void DoorKiller_FallAsRubble(DoorKiller* self, GlobalContext* globalCtx) {
    self->actor.velocity.y += self->actor.gravity;
    if (self->actor.velocity.y < self->actor.minVelocityY) {
        self->actor.velocity.y = self->actor.minVelocityY;
    }

    self->actor.velocity.x *= 0.98f;
    self->actor.velocity.z *= 0.98f;

    // world.rot is repurposed to be the rotation velocity for the rubble pieces
    self->actor.shape.rot.x += self->actor.world.rot.x;
    self->actor.shape.rot.y += self->actor.world.rot.y;
    self->actor.shape.rot.z += self->actor.world.rot.z;

    if (self->timer != 0) {
        self->timer--;
    } else {
        Actor_Kill(&self->actor);
    }
    func_8002D7EC(&self->actor);
}

s32 DoorKiller_IsHit(Actor* thisx, GlobalContext* globalCtx) {
    DoorKiller* self = THIS;
    if ((self->colliderCylinder.base.acFlags & 2) && (self->colliderCylinder.info.acHitInfo != NULL)) {
        return true;
    }
    return false;
}

void DoorKiller_SetAC(DoorKiller* self, GlobalContext* globalCtx) {
    Collider_UpdateCylinder(&self->actor, &self->colliderCylinder);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderCylinder.base);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
}

void DoorKiller_Die(DoorKiller* self, GlobalContext* globalCtx) {
    s32 switchFlag = (self->actor.params >> 8) & 0x3F;

    // Can set a switch flag on death based on params
    if (switchFlag != 0x3F) {
        Flags_SetSwitch(globalCtx, switchFlag);
    }
    Actor_Kill(&self->actor);
}

/**
 * After slamming on the floor, rise back upright
 */
void DoorKiller_RiseBackUp(DoorKiller* self, GlobalContext* globalCtx) {
    s32 i;
    s16 rotation;

    if (self->timer > 0) {
        self->timer--;
    } else {
        self->actionFunc = DoorKiller_Wait;
        self->timer = 16;
        DoorKiller_SetAC(self, globalCtx);
        return;
    }

    self->actor.shape.rot.x = (self->timer >= 8) ? (self->timer * 0x800) - 0x4000 : 0;

    if (self->timer >= 12) {
        rotation = (-self->timer * -500) - 8000;
    } else if (self->timer >= 8) {
        rotation = -2000;
    } else if (self->timer >= 5) {
        rotation = (self->timer * -500) + 2000;
    } else {
        rotation = 0;
    }

    for (i = 2; i < 9; i++) {
        self->jointTable[i].z = -rotation;
    }

    if (self->timer < 8) {
        rotation = Math_SinS(self->timer * 0x2000) * self->timer * 100.0f;
        for (i = 2; i < 9; i++) {
            self->jointTable[i].y = rotation;
        }
    }
}

/**
 * After wobbling, fall over and slam onto the floor, damaging the player if they are in the way. Uses manual distance
 * check for damaging player, not AT system.
 */
void DoorKiller_FallOver(DoorKiller* self, GlobalContext* globalCtx) {
    s32 i;
    s16 rotation;

    if (self->timer > 0) {
        self->timer--;
    } else {
        self->actionFunc = DoorKiller_RiseBackUp;
        self->timer = 16;
        return;
    }

    self->actor.shape.rot.x = (self->timer >= 4) ? 0x8000 + (-self->timer * 0x1000) : 0x4000;

    if (self->timer >= 6) {
        rotation = (-self->timer * -500) - 4000;
    } else if (self->timer >= 4) {
        rotation = -1000;
    } else if (self->timer >= 3) {
        rotation = (self->timer * -500) + 1000;
    } else {
        rotation = 0;
    }

    for (i = 2; i < 9; i++) {
        self->jointTable[i].z = rotation;
    }

    if (self->timer == 4) {
        // spawn 20 random dust particles just before slamming down
        Vec3f velocity = { 0.0f, 0.0f, 0.0f };
        Vec3f accel = { 0.0f, 1.0f, 0.0f };
        Vec3f pos;
        s32 j;
        f32 randF;

        for (j = 0; j != 20; j++) {
            pos.y = 0.0f;
            randF = Rand_CenteredFloat(40.0f);
            pos.z = Rand_ZeroFloat(100.0f);
            pos.x = (Math_CosS(self->actor.world.rot.y) * randF) + (Math_SinS(self->actor.world.rot.y) * pos.z);
            pos.z = (-Math_SinS(self->actor.world.rot.y) * randF) + (Math_CosS(self->actor.world.rot.y) * pos.z);
            velocity.x = pos.x * 0.2f;
            velocity.z = pos.z * 0.2f;
            accel.x = -(velocity.x) * 0.1f;
            accel.z = -(velocity.z) * 0.1f;
            pos.x += self->actor.world.pos.x;
            pos.y += self->actor.world.pos.y;
            pos.z += self->actor.world.pos.z;
            func_8002865C(globalCtx, &pos, &velocity, &accel, 300, 30);
        }
    }
    if (!(self->hasHitPlayerOrGround & 1)) {
        Vec3f playerPosRelToDoor;
        Player* player = GET_PLAYER(globalCtx);
        func_8002DBD0(&self->actor, &playerPosRelToDoor, &player->actor.world.pos);
        if ((fabsf(playerPosRelToDoor.y) < 20.0f) && (fabsf(playerPosRelToDoor.x) < 20.0f) &&
            (playerPosRelToDoor.z < 100.0f) && (playerPosRelToDoor.z > 0.0f)) {
            self->hasHitPlayerOrGround |= 1;
            func_8002F6D4(globalCtx, &self->actor, 6.0f, self->actor.yawTowardsPlayer, 6.0f, 16);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_KDOOR_HIT);
            func_8002F7DC(&player->actor, NA_SE_PL_BODY_HIT);
        }
    }
    if (!(self->hasHitPlayerOrGround & 1) && (self->timer == 2)) {
        self->hasHitPlayerOrGround |= 1;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_KDOOR_HIT_GND);
    }
}

/**
 * Wobble around, signifying the door is about to fall over. Does not set AC and so cannot be destroyed during self.
 */
void DoorKiller_Wobble(DoorKiller* self, GlobalContext* globalCtx) {
    s16 rotation;
    s32 i;

    if ((self->timer == 16) || (self->timer == 8)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_KDOOR_WAVE);
    }

    if (self->timer > 0) {
        self->timer--;
    } else {
        self->actionFunc = DoorKiller_FallOver;
        self->timer = 8;
        self->hasHitPlayerOrGround &= ~1;
        return;
    }

    rotation = Math_SinS(self->timer * 0x2000) * self->timer * 100.0f;
    for (i = 2; i < 9; i++) {
        self->jointTable[i].y = rotation;
    }
    rotation = (u16)(s32)(-Math_CosS(self->timer * 0x1000) * 1000.0f) + 1000;
    for (i = 2; i < 9; i++) {
        self->jointTable[i].z = rotation;
    }
}

/**
 * Idle while the player attempts to open the door and then begin to wobble
 */
void DoorKiller_WaitBeforeWobble(DoorKiller* self, GlobalContext* globalCtx) {
    if (self->timer > 0) {
        self->timer--;
    } else {
        self->timer = 16;
        self->actionFunc = DoorKiller_Wobble;
    }
}

void DoorKiller_Wait(DoorKiller* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f playerPosRelToDoor;
    s16 angleToFacingPlayer;

    func_8002DBD0(&self->actor, &playerPosRelToDoor, &player->actor.world.pos);

    // playerIsOpening is set by player
    if (self->playerIsOpening) {
        self->actionFunc = DoorKiller_WaitBeforeWobble;
        self->timer = 10;
        self->playerIsOpening = 0;
        return;
    }

    if (DoorKiller_IsHit(&self->actor, globalCtx)) {
        // AC cylinder: wobble if hit by most weapons, die if hit by explosives or hammer
        if ((self->colliderCylinder.info.acHitInfo->toucher.dmgFlags & 0x1FFA6) != 0) {
            self->timer = 16;
            self->actionFunc = DoorKiller_Wobble;
        } else if ((self->colliderCylinder.info.acHitInfo->toucher.dmgFlags & 0x48) != 0) {
            DoorKiller_SpawnRubble(&self->actor, globalCtx);
            self->actionFunc = DoorKiller_Die;
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EN_KDOOR_BREAK);
        }
    } else if (Actor_GetCollidedExplosive(globalCtx, &self->colliderJntSph.base) != NULL) {
        // AC sphere: die if hit by explosive
        DoorKiller_SpawnRubble(&self->actor, globalCtx);
        self->actionFunc = DoorKiller_Die;
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EN_KDOOR_BREAK);
    } else if (!Player_InCsMode(globalCtx) && (fabsf(playerPosRelToDoor.y) < 20.0f) &&
               (fabsf(playerPosRelToDoor.x) < 20.0f) && (playerPosRelToDoor.z < 50.0f) &&
               (playerPosRelToDoor.z > 0.0f)) {
        // Set player properties to make the door openable if within range
        angleToFacingPlayer = player->actor.shape.rot.y - self->actor.shape.rot.y;
        if (playerPosRelToDoor.z > 0.0f) {
            angleToFacingPlayer = 0x8000 - angleToFacingPlayer;
        }
        if (ABS(angleToFacingPlayer) < 0x3000) {
            player->doorType = PLAYER_DOORTYPE_FAKE;
            player->doorDirection = (playerPosRelToDoor.z >= 0.0f) ? 1.0f : -1.0f;
            player->doorActor = &self->actor;
        }
    }

    DoorKiller_SetAC(self, globalCtx);
}

/**
 * Grabs the virtual address of the texture from the relevant door object
 */
void DoorKiller_UpdateTexture(Actor* thisx, GlobalContext* globalCtx) {
    DoorKiller* self = THIS;

    gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->doorObjBankIndex].segment);
    self->texture = SEGMENTED_TO_VIRTUAL(self->texture);
    gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[thisx->objBankIndex].segment);
}

/**
 * Gets the correct door texture, defines the appropriate draw fucntion and action function based on type behaviour
 * (door or rubble).
 */
void DoorKiller_SetProperties(DoorKiller* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->doorObjBankIndex)) {
        DoorKiller_UpdateTexture(&self->actor, globalCtx);
        switch (self->actor.params & 0xFF) {
            case DOOR_KILLER_DOOR:
                self->actionFunc = DoorKiller_Wait;
                self->actor.draw = DoorKiller_DrawDoor;
                break;
            case DOOR_KILLER_RUBBLE_PIECE_1:
            case DOOR_KILLER_RUBBLE_PIECE_2:
            case DOOR_KILLER_RUBBLE_PIECE_3:
            case DOOR_KILLER_RUBBLE_PIECE_4:
                self->actionFunc = DoorKiller_FallAsRubble;
                self->actor.draw = DoorKiller_DrawRubble;
                break;
        }
    }
}

void DoorKiller_Update(Actor* thisx, GlobalContext* globalCtx) {
    DoorKiller* self = THIS;

    self->actionFunc(self, globalCtx);
}

void DoorKiller_SetTexture(Actor* thisx, GlobalContext* globalCtx) {
    DoorKiller* self = THIS;
    void* doorTexture = self->texture;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_door_killer.c", 883);
    gSPSegment(POLY_OPA_DISP++, 0x08, doorTexture);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_door_killer.c", 885);
}

void DoorKiller_DrawDoor(Actor* thisx, GlobalContext* globalCtx) {
    DoorKiller* self = THIS;

    func_800943C8(globalCtx->state.gfxCtx);
    DoorKiller_SetTexture(&self->actor, globalCtx);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, NULL);
}

void DoorKiller_DrawRubble(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* dLists[] = { object_door_killer_DL_001250, object_door_killer_DL_001550, object_door_killer_DL_0017B8,
                             object_door_killer_DL_001A58 };
    s32 rubblePieceIndex = (thisx->params & 0xFF) - 1;
    DoorKiller* self = THIS;

    if ((self->timer >= 20) || ((self->timer & 1) == 0)) {
        DoorKiller_SetTexture(thisx, globalCtx);
        Gfx_DrawDListOpa(globalCtx, dLists[rubblePieceIndex]);
    }
}
