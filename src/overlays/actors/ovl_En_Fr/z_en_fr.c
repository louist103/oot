#include "z_en_fr.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"
#include "vt.h"
#include "objects/object_fr/object_fr.h"

#define FLAGS 0x02000019

#define THIS ((EnFr*)thisx)

void EnFr_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFr_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFr_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFr_UpdateIdle(Actor* thisx, GlobalContext* globalCtx);
void EnFr_UpdateActive(Actor* thisx, GlobalContext* globalCtx);
void EnFr_Draw(Actor* thisx, GlobalContext* globalCtx);

// Animation Functions
void EnFr_SetupJumpingOutOfWater(EnFr* self, GlobalContext* globalCtx);
void EnFr_JumpingOutOfWater(EnFr* self, GlobalContext* globalCtx);
void EnFr_OrientOnLogSpot(EnFr* self, GlobalContext* globalCtx);
void EnFr_ChooseJumpFromLogSpot(EnFr* self, GlobalContext* globalCtx);
void EnFr_JumpingUp(EnFr* self, GlobalContext* globalCtx);
void EnFr_JumpingBackIntoWater(EnFr* self, GlobalContext* globalCtx);
void EnFr_DecrementBlinkTimerUpdate(EnFr* self);

// Activation
void EnFr_Idle(EnFr* self, GlobalContext* globalCtx);
void EnFr_Activate(EnFr* self, GlobalContext* globalCtx);
void EnFr_ActivateCheckFrogSong(EnFr* self, GlobalContext* globalCtx);

// Listening for Child Songs
void func_80A1BE98(EnFr* self, GlobalContext* globalCtx);
void EnFr_ListeningToOcarinaNotes(EnFr* self, GlobalContext* globalCtx);
void EnFr_ChildSong(EnFr* self, GlobalContext* globalCtx);
void EnFr_ChildSongFirstTime(EnFr* self, GlobalContext* globalCtx);

// Frog Song for HP Functions
void EnFr_TalkBeforeFrogSong(EnFr* self, GlobalContext* globalCtx);
void EnFr_SetupFrogSong(EnFr* self, GlobalContext* globalCtx);
void EnFr_ContinueFrogSong(EnFr* self, GlobalContext* globalCtx);
void EnFr_OcarinaMistake(EnFr* self, GlobalContext* globalCtx);

// Reward Functions
void EnFr_SetupReward(EnFr* self, GlobalContext* globalCtx, u8 unkCondition);
void EnFr_PrintTextBox(EnFr* self, GlobalContext* globalCtx);
void EnFr_TalkBeforeReward(EnFr* self, GlobalContext* globalCtx);
void EnFr_SetReward(EnFr* self, GlobalContext* globalCtx);

// Deactivate
void EnFr_Deactivate(EnFr* self, GlobalContext* globalCtx);
void EnFr_GiveReward(EnFr* self, GlobalContext* globalCtx);
void EnFr_SetIdle(EnFr* self, GlobalContext* globalCtx);

/*
Frogs params WIP docs

Represents 6 Actor Instances for frogs:
    - 1 Prop actor instance set to where Link plays Ocarina, manages 5 frogs
    - 5 NPC actor instances for the frogs themselves

0: Prop Actor Instance (located where link detects ocarina, interacts with Link)
1: Frog 0 (Yellow)
2: Frog 1 (Blue)
3: Frog 2 (Red)
4: Frog 3 (Purple)
5: Frog 4 (White)

Note that because of the Prop Actor, actor.params is 1 shifted from frogIndex
Therefore, frogIndex = actor.params - 1


sEnFrPointers.flags = 1
     - Activate frogs, frogs will jump out of the water

sEnFrPointers.flags = 1 to 11:
     - Counter: Frogs will sequentially jump out based on counter:
         - 1: Frog 1 (Blue)
         - 3: frog 3 (Purple)
         - 5: frog 0 (Yellow)
         - 7: frog 2 (Red)
         - 9: frog 4 (White)
     - Will proceed when counter reachers 11

sEnFrPointers.flags = 12
     - Deactivate frogs, frogs will jump back into the water
*/

typedef struct {
    u8 flags;
    EnFr* frogs[5];
} EnFrPointers;

typedef struct {
    f32 xzDist;
    f32 yaw;
    f32 yDist;
} LogSpotToFromWater;

static EnFrPointers sEnFrPointers = {
    0x00,
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};

// Flags for gSaveContext.eventChkInf[13]
static u16 sSongIndex[] = {
    0x0002, 0x0004, 0x0010, 0x0008, 0x0020, 0x0040, 0x0001, 0x0000,
};

// Frog to Index for Song Flag (sSongIndex) Mapping
static u8 sFrogToSongIndex[] = {
    FROG_SARIA, FROG_SUNS, FROG_SOT, FROG_ZL, FROG_EPONA,
};

// Song to Frog Index Mapping
static s32 sSongToFrog[] = {
    FROG_PURPLE, FROG_WHITE, FROG_YELLOW, FROG_BLUE, FROG_RED,
};

const ActorInit En_Fr_InitVars = {
    ACTOR_EN_FR,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_FR,
    sizeof(EnFr),
    (ActorFunc)EnFr_Init,
    (ActorFunc)EnFr_Destroy,
    (ActorFunc)EnFr_Update,
    NULL,
};

static Color_RGBA8 sEnFrColor[] = {
    { 200, 170, 0, 255 }, { 0, 170, 200, 255 }, { 210, 120, 100, 255 }, { 120, 130, 230, 255 }, { 190, 190, 190, 255 },
};

// Jumping back into water frog animation
//      sLogSpotToFromWater[frog].xzDist is magnitude of xz distance frog travels
//      sLogSpotToFromWater[frog].yaw is rot around y-axis of jumping back into water
//      sLogSpotToFromWater[frog].yDist is change in y distance frog travels
static LogSpotToFromWater sLogSpotToFromWater[] = {
    { 0.0f, 0.0f, 0.0f },              // Prop   (Where link pulls ocarina)
    { 80.0f, -0.375f * M_PI, -80.0f }, // FROG_YELLOW
    { 80.0f, -0.5f * M_PI, -80.0f },   // FROG_BLUE
    { 80.0f, -0.25f * M_PI, -80.0f },  // FROG_RED
    { 80.0f, 0.875f * M_PI, -80.0f },  // FROG_PURPLE
    { 80.0f, 0.5f * M_PI, -80.0f },    // FROG_WHITE
};

// Timer values for the frog choir song
static s16 sTimerFrogSong[] = {
    40, 20, 15, 12, 12,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

// Counter to Coordinate Frog jumping out of water one at a time
// Used as part of sEnFrPointers.flags
static u8 sTimerJumpingOutOfWater[] = {
    5, 1, 7, 3, 9,
};

// targetScale (default = 150.0) Actor scale target for Math_ApproachF
// Used as a frog grows from hearing a new child song
static f32 sGrowingScale[] = {
    180.0f,
    210.0f,
    240.0f,
    270.0f,
};

static u8 sSmallFrogNotes[] = {
    5,  // C-Down Ocarina
    2,  // A Button Ocarina
    9,  // C-Right Ocarina
    11, // C-Left Ocarina
    14, // C Up Ocarina
};

static s8 sLargeFrogNotes[] = {
    -7,  // C-Down Ocarina
    -10, // A Button Ocarina
    -3,  // C-Right Ocarina
    -1,  // C-Left Ocarina
    2,   // C Up Ocarina
};

static u8 sJumpOrder[] = {
    FROG_BLUE, FROG_YELLOW, FROG_RED, FROG_PURPLE, FROG_WHITE, FROG_BLUE, FROG_YELLOW, FROG_RED,
};

// probably can be replaced with a message context enum at a later time
// A, CDOWN, CRIGHT, CLEFT, CUP
static u8 sOcarinaNotes[] = {
    0, 1, 2, 3, 4,
};

void EnFr_OrientUnderwater(EnFr* self) {
    Vec3f vec1;
    Vec3f vec2;

    vec1.x = vec1.y = 0.0f;
    vec1.z = self->xzDistToLogSpot = sLogSpotToFromWater[self->actor.params].xzDist;
    Matrix_RotateY(sLogSpotToFromWater[self->actor.params].yaw, MTXMODE_NEW);
    Matrix_MultVec3f(&vec1, &vec2);
    self->actor.world.pos.x = self->posLogSpot.x + vec2.x;
    self->actor.world.pos.z = self->posLogSpot.z + vec2.z;
    self->actor.world.pos.y = sLogSpotToFromWater[self->actor.params].yDist + self->posLogSpot.y;
    self->actor.world.rot.y = self->actor.shape.rot.y =
        (s16)(sLogSpotToFromWater[self->actor.params].yaw * ((f32)0x8000 / M_PI)) + 0x8000;
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = 0.0f;
}

void EnFr_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFr* self = THIS;

    if (self->actor.params == 0) {
        self->actor.destroy = NULL;
        self->actor.draw = NULL;
        self->actor.update = EnFr_UpdateIdle;
        self->actor.flags &= ~0x11;
        self->actor.flags &= ~0;
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_PROP);
        self->actor.textId = 0x40AC;
        self->actionFunc = EnFr_Idle;
    } else {
        if ((self->actor.params >= 6) || (self->actor.params < 0)) {
            osSyncPrintf(VT_COL(RED, WHITE));
            // "The argument is wrong!!"
            osSyncPrintf("%s[%d] : 引数が間違っている！！(%d)\n", "../z_en_fr.c", 370, self->actor.params);
            osSyncPrintf(VT_RST);
            ASSERT(0, "0", "../z_en_fr.c", 372);
        }

        self->objBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_GAMEPLAY_FIELD_KEEP);
        if (self->objBankIndex < 0) {
            Actor_Kill(&self->actor);
            osSyncPrintf(VT_COL(RED, WHITE));
            // "There is no bank!!"
            osSyncPrintf("%s[%d] : バンクが無いよ！！\n", "../z_en_fr.c", 380);
            osSyncPrintf(VT_RST);
            ASSERT(0, "0", "../z_en_fr.c", 382);
        }
    }
}

// Draw only the purple frog when ocarina is not pulled out on the log spot
void EnFr_DrawIdle(EnFr* self) {
    self->actor.draw = (self->actor.params - 1) != FROG_PURPLE ? NULL : EnFr_Draw;
}

void EnFr_DrawActive(EnFr* self) {
    self->actor.draw = EnFr_Draw;
}

void EnFr_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnFr* self = THIS;
    s32 pad;
    s32 frogIndex;
    s32 pad2;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->objBankIndex)) {
        self->actor.flags &= ~0x10;
        frogIndex = self->actor.params - 1;
        sEnFrPointers.frogs[frogIndex] = self;
        Actor_ProcessInitChain(&self->actor, sInitChain);
        // frog
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_fr_Skel_00B498, &object_fr_Anim_001534,
                           self->jointTable, self->morphTable, 24);
        // butterfly
        SkelAnime_Init(globalCtx, &self->skelAnimeButterfly, &gButterflySkel, &gButterflyAnim,
                       self->jointTableButterfly, self->morphTableButterfly, 8);
        // When playing the song for the HP, the frog with the next note and the butterfly turns on its lightsource
        self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);
        Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.home.pos.x, self->actor.home.pos.y,
                                  self->actor.home.pos.z, 255, 255, 255, -1);
        // Check to see if the song for a particular frog has been played.
        // If it has, the frog is larger. If not, the frog is smaller
        self->scale = gSaveContext.eventChkInf[13] & sSongIndex[sFrogToSongIndex[frogIndex]] ? 270.0f : 150.0f;
        // When the frogs are not active (link doesn't have his ocarina out),
        // Then shrink the frogs down by a factor of 10,000
        Actor_SetScale(&self->actor, self->scale * 0.0001f);
        self->actor.minVelocityY = -9999.0f;
        Actor_SetFocus(&self->actor, 10.0f);
        self->eyeTexIndex = 1;
        self->blinkTimer = (s16)(Rand_ZeroFloat(60.0f) + 20.0f);
        self->blinkFunc = EnFr_DecrementBlinkTimerUpdate;
        self->isBelowWaterSurfacePrevious = self->isBelowWaterSurfaceCurrent = false;
        self->isJumpingUp = false;
        self->posLogSpot = self->actor.world.pos;
        self->actionFunc = EnFr_SetupJumpingOutOfWater;
        self->isDeactivating = false;
        self->growingScaleIndex = 0;
        self->isActive = false;
        self->isJumpingToFrogSong = false;
        self->songIndex = FROG_NO_SONG;
        self->unusedButterflyActor = NULL;
        EnFr_OrientUnderwater(self);
        EnFr_DrawIdle(self);
        self->actor.update = EnFr_UpdateActive;
        self->isButterflyDrawn = false;
        self->xyAngleButterfly = 0x1000 * (s16)Rand_ZeroFloat(255.0f);
        self->posButterflyLight.x = self->posButterfly.x = self->posLogSpot.x;
        self->posButterflyLight.y = self->posButterfly.y = self->posLogSpot.y + 50.0f;
        self->posButterflyLight.z = self->posButterfly.z = self->posLogSpot.z;
        self->actor.flags &= ~1;
    }
}

void EnFr_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFr* self = THIS;

    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
}

void EnFr_IsDivingIntoWater(EnFr* self, GlobalContext* globalCtx) {
    WaterBox* waterBox;
    f32 waterSurface;

    if (WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->actor.world.pos.x, self->actor.world.pos.z,
                                &waterSurface, &waterBox)) {
        self->isBelowWaterSurfacePrevious = self->isBelowWaterSurfaceCurrent;
        self->isBelowWaterSurfaceCurrent = self->actor.world.pos.y <= waterSurface ? true : false;
    }
}

void EnFr_DivingIntoWater(EnFr* self, GlobalContext* globalCtx) {
    Vec3f vec;

    // Jumping into or out of water
    if (self->isBelowWaterSurfaceCurrent != self->isBelowWaterSurfacePrevious) {
        vec.x = self->actor.world.pos.x;
        vec.y = self->actor.world.pos.y - 10.0f;
        vec.z = self->actor.world.pos.z;
        EffectSsGSplash_Spawn(globalCtx, &vec, NULL, NULL, 1, 1);

        if (self->isBelowWaterSurfaceCurrent == false) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_DIVE_INTO_WATER_L);
        } else {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_BOMB_DROP_WATER);
        }
    }
}

s32 EnFr_IsBelowLogSpot(EnFr* self, f32* yDistToLogSpot) {
    yDistToLogSpot[0] = self->actor.world.pos.y - self->posLogSpot.y;
    if ((self->actor.velocity.y < 0.0f) && (yDistToLogSpot[0] <= 0.0f)) {
        self->actor.velocity.y = 0.0f;
        self->actor.world.pos.y = self->posLogSpot.y;
        return true;
    } else {
        return false;
    }
}

s32 EnFr_IsAboveAndWithin30DistXZ(Player* player, EnFr* self) {
    f32 xDistToPlayer = player->actor.world.pos.x - self->actor.world.pos.x;
    f32 zDistToPlayer = player->actor.world.pos.z - self->actor.world.pos.z;
    f32 yDistToPlayer = player->actor.world.pos.y - self->actor.world.pos.y;

    return ((SQ(xDistToPlayer) + SQ(zDistToPlayer)) <= SQ(30.0f)) && (yDistToPlayer >= 0.0f);
}

void EnFr_DecrementBlinkTimer(EnFr* self) {
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    } else {
        self->blinkFunc = EnFr_DecrementBlinkTimerUpdate;
    }
}

void EnFr_DecrementBlinkTimerUpdate(EnFr* self) {
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    } else if (self->eyeTexIndex) {
        self->eyeTexIndex = 0;
        self->blinkTimer = (s16)(Rand_ZeroFloat(60.0f) + 20.0f);
        self->blinkFunc = EnFr_DecrementBlinkTimer;
    } else {
        self->eyeTexIndex = 1;
        self->blinkTimer = 1;
    }
}

void EnFr_SetupJumpingOutOfWater(EnFr* self, GlobalContext* globalCtx) {
    if (sEnFrPointers.flags == sTimerJumpingOutOfWater[self->actor.params - 1]) {
        Animation_Change(&self->skelAnime, &object_fr_Anim_0007BC, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_fr_Anim_0007BC), ANIMMODE_ONCE, 0.0f);
        EnFr_DrawActive(self);
        self->actionFunc = EnFr_JumpingOutOfWater;
    }
}

void EnFr_JumpingOutOfWater(EnFr* self, GlobalContext* globalCtx) {
    Vec3f vec1;
    Vec3f vec2;

    if (self->skelAnime.curFrame == 6.0f) {
        sEnFrPointers.flags++;
        self->skelAnime.playSpeed = 0.0f;
    } else if (self->skelAnime.curFrame == 3.0f) {
        self->actor.gravity = -10.0f;
        self->actor.speedXZ = 0.0f;
        self->actor.velocity.y = 47.0f;
    }

    vec1.x = vec1.y = 0.0f;
    vec1.z = self->xzDistToLogSpot;
    Matrix_RotateY(((self->actor.world.rot.y + 0x8000) / (f32)0x8000) * M_PI, MTXMODE_NEW);
    Matrix_MultVec3f(&vec1, &vec2);
    self->actor.world.pos.x = self->posLogSpot.x + vec2.x;
    self->actor.world.pos.z = self->posLogSpot.z + vec2.z;
    if (self->skelAnime.curFrame >= 3.0f) {
        Math_ApproachF(&self->xzDistToLogSpot, 0.0f, 1.0f, 10.0f);
    }

    if (EnFr_IsBelowLogSpot(self, &vec2.y)) {
        self->actor.gravity = 0.0f;
        self->actionFunc = EnFr_OrientOnLogSpot;
        self->unusedFloat = 0.0f;
    }

    if ((self->actor.velocity.y <= 0.0f) && (vec2.y < 40.0f)) {
        self->skelAnime.playSpeed = 1.0f;
    }
}

void EnFr_OrientOnLogSpot(EnFr* self, GlobalContext* globalCtx) {
    s16 rotYRemaining = Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 2, 10000, 100);

    self->actor.world.rot.y = self->actor.shape.rot.y;

    if ((rotYRemaining == 0) && (self->skelAnime.curFrame == self->skelAnime.endFrame)) {
        sEnFrPointers.flags++;
        self->actionFunc = EnFr_ChooseJumpFromLogSpot;
        Animation_Change(&self->skelAnime, &object_fr_Anim_001534, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_fr_Anim_001534), ANIMMODE_LOOP, 0.0f);
    }
}

void EnFr_ChooseJumpFromLogSpot(EnFr* self, GlobalContext* globalCtx) {
    if (sEnFrPointers.flags == 12) {
        self->actor.world.rot.y = ((f32)0x8000 / M_PI) * sLogSpotToFromWater[self->actor.params].yaw;
        Animation_Change(&self->skelAnime, &object_fr_Anim_0007BC, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_fr_Anim_0007BC), ANIMMODE_ONCE, 0.0f);
        self->actionFunc = EnFr_JumpingBackIntoWater;
    } else if (self->isJumpingUp) {
        Animation_Change(&self->skelAnime, &object_fr_Anim_0007BC, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_fr_Anim_0007BC), ANIMMODE_ONCE, 0.0f);
        self->actionFunc = EnFr_JumpingUp;
    }
}

void EnFr_JumpingUp(EnFr* self, GlobalContext* globalCtx) {
    f32 yDistToLogSpot;

    if (self->skelAnime.curFrame == 6.0f) {
        self->skelAnime.playSpeed = 0.0f;
    } else if (self->skelAnime.curFrame == 3.0f) {
        self->actor.gravity = -10.0f;
        self->actor.velocity.y = 25.0f;
        if (self->isJumpingToFrogSong) {
            self->isJumpingToFrogSong = false;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_EAT);
        }
    }

    if (EnFr_IsBelowLogSpot(self, &yDistToLogSpot)) {
        self->isJumpingUp = false;
        self->actor.gravity = 0.0f;
        Animation_Change(&self->skelAnime, &object_fr_Anim_0011C0, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_fr_Anim_0011C0), ANIMMODE_LOOP, 0.0f);
        self->actionFunc = EnFr_ChooseJumpFromLogSpot;
    } else if ((self->actor.velocity.y <= 0.0f) && (yDistToLogSpot < 40.0f)) {
        self->skelAnime.playSpeed = 1.0f;
    }
}

void EnFr_JumpingBackIntoWater(EnFr* self, GlobalContext* globalCtx) {
    f32 yUnderwater = sLogSpotToFromWater[self->actor.params].yDist + self->posLogSpot.y;

    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.world.rot.y, 2, 10000, 100);
    if (self->skelAnime.curFrame == 6.0f) {
        self->skelAnime.playSpeed = 0.0f;
    } else if (self->skelAnime.curFrame == 3.0f) {
        self->actor.speedXZ = 6.0f;
        self->actor.gravity = -10.0f;
        self->actor.velocity.y = 25.0f;
    }

    // Final Spot Reached
    if ((self->actor.velocity.y < 0.0f) && (self->actor.world.pos.y < yUnderwater)) {
        Animation_Change(&self->skelAnime, &object_fr_Anim_001534, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_fr_Anim_001534), ANIMMODE_LOOP, 0.0f);
        self->actionFunc = EnFr_SetupJumpingOutOfWater;
        EnFr_DrawIdle(self);
        self->isDeactivating = true;
        EnFr_OrientUnderwater(self);
    }
}

void EnFr_SetScaleActive(EnFr* self, GlobalContext* globalCtx) {
    switch (self->isGrowing) {
        case false:
            Math_ApproachF(&self->scale, sGrowingScale[self->growingScaleIndex], 2.0f, 25.0f);
            if (self->scale >= sGrowingScale[self->growingScaleIndex]) {
                self->scale = sGrowingScale[self->growingScaleIndex];
                if (self->growingScaleIndex < 3) {
                    self->isGrowing = true;
                } else {
                    self->isJumpingUp = false;
                    self->isActive = false;
                }
            }
            break;
        case true:
            Math_ApproachF(&self->scale, 150.0f, 2.0f, 25.0f);
            if (self->scale <= 150.0f) {
                self->scale = 150.0f;
                self->growingScaleIndex++;
                if (self->growingScaleIndex >= 4) {
                    self->growingScaleIndex = 3;
                }
                self->isGrowing = false;
            }
            break;
    }
}

void EnFr_ButterflyPath(EnFr* self, GlobalContext* globalCtx) {
    s16 rotY = self->actor.shape.rot.y;
    f32 sin;
    Vec3f vec1;
    Vec3f vec2;

    self->xyAngleButterfly += 0x1000;
    Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, MTXMODE_APPLY);
    vec1.x = vec1.y = 0.0f;
    vec1.z = 25.0f;
    Matrix_MultVec3f(&vec1, &vec2);
    sin = Math_SinS(self->xyAngleButterfly * 2) * 5.0f;
    self->posButterfly.x = (Math_SinS(rotY) * sin) + vec2.x;
    self->posButterfly.y = (2.0f * Math_CosS(self->xyAngleButterfly)) + (self->posLogSpot.y + 50.0f);
    self->posButterfly.z = (Math_CosS(rotY) * sin) + vec2.z;
    Matrix_Translate(self->posButterfly.x, self->posButterfly.y, self->posButterfly.z, MTXMODE_NEW);
    Matrix_RotateRPY(self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, MTXMODE_APPLY);
    vec1.x = 0.0f;
    vec1.y = -15.0f;
    vec1.z = 20.0f;
    Matrix_MultVec3f(&vec1, &self->posButterflyLight);
}

void EnFr_UpdateActive(Actor* thisx, GlobalContext* globalCtx) {
    EnFr* self = THIS;

    self->jumpCounter++;
    Actor_SetScale(&self->actor, self->scale * 0.0001f);

    if (self->isActive) {
        EnFr_SetScaleActive(self, globalCtx);
    } else {
        Actor_SetFocus(&self->actor, 10.0f);
        self->blinkFunc(self);
        self->actionFunc(self, globalCtx);
        EnFr_IsDivingIntoWater(self, globalCtx);
        EnFr_DivingIntoWater(self, globalCtx);
        SkelAnime_Update(&self->skelAnime);
        SkelAnime_Update(&self->skelAnimeButterfly);
        EnFr_ButterflyPath(self, globalCtx);
        Actor_MoveForward(&self->actor);
    }
}

s32 EnFr_SetupJumpingUp(EnFr* self, s32 frogIndex) {
    EnFr* frog = sEnFrPointers.frogs[frogIndex];
    u8 semitone;

    if (frog != NULL && frog->isJumpingUp == false) {
        semitone = frog->growingScaleIndex == 3 ? sLargeFrogNotes[frogIndex] : sSmallFrogNotes[frogIndex];
        if (self->songIndex == FROG_CHOIR_SONG) {
            frog->isJumpingToFrogSong = true;
        }
        frog->isJumpingUp = true;
        Audio_PlaySoundTransposed(&frog->actor.projectedPos, NA_SE_EV_FROG_JUMP, semitone);
        return true;
    } else {
        return false;
    }
}

void EnFr_Idle(EnFr* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (player->stateFlags2 & 0x2000000) {
        if (globalCtx->msgCtx.unk_E3EE == 4) {
            globalCtx->msgCtx.unk_E3EE = 0;
        }

        OnePointCutscene_Init(globalCtx, 4110, ~0x62, &self->actor, MAIN_CAM);
        globalCtx->msgCtx.msgMode = 0x37;
        player->actor.world.pos.x = self->actor.world.pos.x; // x = 990.0f
        player->actor.world.pos.y = self->actor.world.pos.y; // y = 205.0f
        player->actor.world.pos.z = self->actor.world.pos.z; // z = -1220.0f
        player->currentYaw = player->actor.world.rot.y = player->actor.shape.rot.y = self->actor.world.rot.y;
        self->reward = GI_NONE;
        self->actionFunc = EnFr_Activate;
    } else if (EnFr_IsAboveAndWithin30DistXZ(player, self)) {
        player->unk_6A8 = &self->actor;
    }
}

void EnFr_Activate(EnFr* self, GlobalContext* globalCtx) {
    if (globalCtx->msgCtx.msgMode == 0xC) {
        globalCtx->msgCtx.msgMode = 0x37;
        sEnFrPointers.flags = 1;
        self->actionFunc = EnFr_ActivateCheckFrogSong;
    } else if (globalCtx->msgCtx.msgMode == 0x37) { // Goes to Frogs 2 Song
        sEnFrPointers.flags = 1;
        self->actionFunc = EnFr_ActivateCheckFrogSong;
    }
}

void EnFr_ActivateCheckFrogSong(EnFr* self, GlobalContext* globalCtx) {
    if (sEnFrPointers.flags == 11) {
        // Check if all 6 child songs have been played for the frogs
        if ((gSaveContext.eventChkInf[13] & 0x2)        // ZL
            && (gSaveContext.eventChkInf[13] & 0x4)     // Epona
            && (gSaveContext.eventChkInf[13] & 0x10)    // Saria
            && (gSaveContext.eventChkInf[13] & 0x8)     // Suns
            && (gSaveContext.eventChkInf[13] & 0x20)    // SoT
            && (gSaveContext.eventChkInf[13] & 0x40)) { // SoS
            self->actionFunc = EnFr_TalkBeforeFrogSong;
            self->songIndex = FROG_CHOIR_SONG;
            func_8010B680(globalCtx, 0x40AB, &self->actor);
        } else {
            self->songIndex = FROG_ZL;
            self->actionFunc = func_80A1BE98;
        }
    }
}

void func_80A1BE98(EnFr* self, GlobalContext* globalCtx) {
    EnFr* frog;
    s32 frogIndex;

    for (frogIndex = 0; frogIndex < ARRAY_COUNT(sEnFrPointers.frogs); frogIndex++) {
        frog = sEnFrPointers.frogs[frogIndex];
        if (frog != NULL && frog->actionFunc == EnFr_ChooseJumpFromLogSpot) {
            continue;
        } else {
            return;
        }
    }

    func_8010BD58(globalCtx, 0x30);
    self->actionFunc = EnFr_ListeningToOcarinaNotes;
}

void EnFr_ListeningToOcarinaNotes(EnFr* self, GlobalContext* globalCtx) {
    self->songIndex = FROG_NO_SONG;
    switch (globalCtx->msgCtx.unk_E3EE) { // Ocarina Song played
        case 7:
            self->songIndex = FROG_ZL;
            break;
        case 6:
            self->songIndex = FROG_EPONA;
            break;
        case 5:
            self->songIndex = FROG_SARIA;
            break;
        case 8:
            self->songIndex = FROG_SUNS;
            break;
        case 9:
            self->songIndex = FROG_SOT;
            break;
        case 10:
            self->songIndex = FROG_STORMS;
            break;
        case 4:
            EnFr_OcarinaMistake(self, globalCtx);
            break;
        case 1:                                   // Ocarina note played, but no song played
            switch (globalCtx->msgCtx.unk_E410) { // Jumping frogs in open ocarina based on ocarina note played
                case 0:
                    EnFr_SetupJumpingUp(self, FROG_BLUE);
                    break;
                case 1:
                    EnFr_SetupJumpingUp(self, FROG_YELLOW);
                    break;
                case 2:
                    EnFr_SetupJumpingUp(self, FROG_RED);
                    break;
                case 3:
                    EnFr_SetupJumpingUp(self, FROG_PURPLE);
                    break;
                case 4:
                    EnFr_SetupJumpingUp(self, FROG_WHITE);
                    break;
            }
    }
    if (self->songIndex != FROG_NO_SONG) {
        self->jumpCounter = 0;
        self->actionFunc = EnFr_ChildSong;
    }
}

void EnFr_ChildSong(EnFr* self, GlobalContext* globalCtx) {
    EnFr* frog;
    u8 songIndex;

    if (self->jumpCounter < 48) {
        if (self->jumpCounter % 4 == 0) {
            EnFr_SetupJumpingUp(self, sJumpOrder[(self->jumpCounter >> 2) & 7]);
        }
    } else {
        songIndex = self->songIndex;
        if (songIndex == FROG_STORMS) {
            self->actor.textId = 0x40AA;
            EnFr_SetupReward(self, globalCtx, false);
        } else if (!(gSaveContext.eventChkInf[13] & sSongIndex[songIndex])) {
            frog = sEnFrPointers.frogs[sSongToFrog[songIndex]];
            func_80078884(NA_SE_SY_CORRECT_CHIME);
            if (frog->actionFunc == EnFr_ChooseJumpFromLogSpot) {
                frog->isJumpingUp = true;
                frog->isActive = true;
                Audio_PlayActorSound2(&frog->actor, NA_SE_EV_FROG_GROW_UP);
                self->actionFunc = EnFr_ChildSongFirstTime;
            } else {
                self->jumpCounter = 48;
            }
        } else {
            self->actor.textId = 0x40A9;
            EnFr_SetupReward(self, globalCtx, true);
        }
    }
}

void EnFr_ChildSongFirstTime(EnFr* self, GlobalContext* globalCtx) {
    EnFr* frog = sEnFrPointers.frogs[sSongToFrog[self->songIndex]];
    if (frog->isActive == false) {
        self->actor.textId = 0x40A9;
        EnFr_SetupReward(self, globalCtx, true);
    }
}

void EnFr_TalkBeforeFrogSong(EnFr* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx))) {
        func_80106CCC(globalCtx);
        self->frogSongTimer = 2;
        self->actionFunc = EnFr_SetupFrogSong;
    }
}

void EnFr_CheckOcarinaInputFrogSong(u8 ocarinaNote) {
    EnFr* frog;
    s32 frogIndexButterfly;
    s32 frogIndex;

    switch (ocarinaNote) {
        case 0:
            frogIndexButterfly = FROG_BLUE;
            break;
        case 1:
            frogIndexButterfly = FROG_YELLOW;
            break;
        case 2:
            frogIndexButterfly = FROG_RED;
            break;
        case 3:
            frogIndexButterfly = FROG_PURPLE;
            break;
        case 4:
            frogIndexButterfly = FROG_WHITE;
    }
    // Turn on or off butterfly above frog
    for (frogIndex = 0; frogIndex < ARRAY_COUNT(sEnFrPointers.frogs); frogIndex++) {
        frog = sEnFrPointers.frogs[frogIndex];
        frog->isButterflyDrawn = frogIndex == frogIndexButterfly ? true : false;
    }
}

void EnFr_DeactivateButterfly() {
    s32 frogIndex;
    EnFr* frog;

    for (frogIndex = 0; frogIndex < ARRAY_COUNT(sEnFrPointers.frogs); frogIndex++) {
        frog = sEnFrPointers.frogs[frogIndex];
        frog->isButterflyDrawn = false;
    }
}

u8 EnFr_GetNextNoteFrogSong(u8 ocarinaNoteIndex) {
    if (!(gSaveContext.eventChkInf[13] & 1)) {
        return gFrogsSongPtr[ocarinaNoteIndex];
    } else {
        return sOcarinaNotes[(s32)Rand_ZeroFloat(60.0f) % 5];
    }
}

void EnFr_SetupFrogSong(EnFr* self, GlobalContext* globalCtx) {
    if (self->frogSongTimer != 0) {
        self->frogSongTimer--;
    } else {
        self->frogSongTimer = 40;
        self->ocarinaNoteIndex = 0;
        func_8010BD58(globalCtx, 0x2F);
        self->ocarinaNote = EnFr_GetNextNoteFrogSong(self->ocarinaNoteIndex);
        EnFr_CheckOcarinaInputFrogSong(self->ocarinaNote);
        self->actionFunc = EnFr_ContinueFrogSong;
    }
}

s32 EnFr_IsFrogSongComplete(EnFr* self, GlobalContext* globalCtx) {
    u8 index;
    u8 ocarinaNote;
    MessageContext* msgCtx = &globalCtx->msgCtx;
    u8 ocarinaNoteIndex;

    if (self->ocarinaNote == (*msgCtx).unk_E410) { // required to match, possibly an array?
        self->ocarinaNoteIndex++;
        ocarinaNoteIndex = self->ocarinaNoteIndex;
        if (1) {}
        if (ocarinaNoteIndex >= 14) { // Frog Song is completed
            self->ocarinaNoteIndex = 13;
            return true;
        }
        // The first four notes have more frames to receive an input
        index = ocarinaNoteIndex < 4 ? (s32)ocarinaNoteIndex : 4;
        ocarinaNote = EnFr_GetNextNoteFrogSong(ocarinaNoteIndex);
        self->ocarinaNote = ocarinaNote;
        EnFr_CheckOcarinaInputFrogSong(ocarinaNote);
        self->frogSongTimer = sTimerFrogSong[index];
    }
    return false;
}

void EnFr_OcarinaMistake(EnFr* self, GlobalContext* globalCtx) {
    func_80106CCC(globalCtx);
    self->reward = GI_NONE;
    func_80078884(NA_SE_SY_OCARINA_ERROR);
    func_800ED858(0);
    sEnFrPointers.flags = 12;
    EnFr_DeactivateButterfly();
    self->actionFunc = EnFr_Deactivate;
}

void EnFr_ContinueFrogSong(EnFr* self, GlobalContext* globalCtx) {
    s32 counter;
    EnFr* frog;
    s32 i;

    if (self->frogSongTimer == 0) {
        EnFr_OcarinaMistake(self, globalCtx);
    } else {
        self->frogSongTimer--;
        if (globalCtx->msgCtx.msgMode == 0x32) {
            counter = 0;
            for (i = 0; i < ARRAY_COUNT(sEnFrPointers.frogs); i++) {
                frog = sEnFrPointers.frogs[i];
                if (frog != NULL && frog->actionFunc == EnFr_ChooseJumpFromLogSpot) {
                    continue;
                } else {
                    counter++;
                }
            }
            if (counter == 0 && CHECK_BTN_ALL(globalCtx->state.input[0].press.button, BTN_B)) {
                EnFr_OcarinaMistake(self, globalCtx);
                return;
            }
        }

        if (globalCtx->msgCtx.msgMode == 0x33) {
            globalCtx->msgCtx.msgMode = 0x31;
            switch (globalCtx->msgCtx.unk_E410) {
                case 0:
                    EnFr_SetupJumpingUp(self, FROG_BLUE);
                    break;
                case 1:
                    EnFr_SetupJumpingUp(self, FROG_YELLOW);
                    break;
                case 2:
                    EnFr_SetupJumpingUp(self, FROG_RED);
                    break;
                case 3:
                    EnFr_SetupJumpingUp(self, FROG_PURPLE);
                    break;
                case 4:
                    EnFr_SetupJumpingUp(self, FROG_WHITE);
            }
            if (EnFr_IsFrogSongComplete(self, globalCtx)) {
                self->actor.textId = 0x40AC;
                EnFr_SetupReward(self, globalCtx, false);
            }
        }
    }
}

void EnFr_SetupReward(EnFr* self, GlobalContext* globalCtx, u8 unkCondition) {
    EnFr_DeactivateButterfly();
    if (unkCondition) {
        func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
    } else {
        func_80078884(NA_SE_SY_CORRECT_CHIME);
    }

    func_800ED858(0);
    globalCtx->msgCtx.msgMode = 0x37;
    self->actionFunc = EnFr_PrintTextBox;
}

void EnFr_PrintTextBox(EnFr* self, GlobalContext* globalCtx) {
    func_8010B680(globalCtx, self->actor.textId, &self->actor);
    self->actionFunc = EnFr_TalkBeforeReward;
}

void EnFr_TalkBeforeReward(EnFr* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        self->frogSongTimer = 100;
        func_80106CCC(globalCtx);
        self->actionFunc = EnFr_SetReward;
    }
}

void EnFr_SetReward(EnFr* self, GlobalContext* globalCtx) {
    u16 songIndex;

    sEnFrPointers.flags = 12;
    songIndex = self->songIndex;
    self->actionFunc = EnFr_Deactivate;
    self->reward = GI_NONE;
    if ((songIndex >= FROG_ZL) && (songIndex <= FROG_SOT)) {
        if (!(gSaveContext.eventChkInf[13] & sSongIndex[songIndex])) {
            gSaveContext.eventChkInf[13] |= sSongIndex[songIndex];
            self->reward = GI_RUPEE_PURPLE;
        } else {
            self->reward = GI_RUPEE_BLUE;
        }
    } else if (songIndex == FROG_STORMS) {
        if (!(gSaveContext.eventChkInf[13] & sSongIndex[songIndex])) {
            gSaveContext.eventChkInf[13] |= sSongIndex[songIndex];
            self->reward = GI_HEART_PIECE;
        } else {
            self->reward = GI_RUPEE_BLUE;
        }
    } else if (songIndex == FROG_CHOIR_SONG) {
        if (!(gSaveContext.eventChkInf[13] & sSongIndex[songIndex])) {
            gSaveContext.eventChkInf[13] |= sSongIndex[songIndex];
            self->reward = GI_HEART_PIECE;
        } else {
            self->reward = GI_RUPEE_PURPLE;
        }
    }
}

void EnFr_Deactivate(EnFr* self, GlobalContext* globalCtx) {
    EnFr* frogLoop1;
    EnFr* frogLoop2;
    s32 frogIndex;

    // Originally was going to have separate butterfly actor
    // Changed to include butterfly as part of frog actor
    // This unused code would have frozen the butterfly actor above frog
    if (self->unusedButterflyActor != NULL) {
        self->unusedButterflyActor->freezeTimer = 10;
    }

    for (frogIndex = 0; frogIndex < ARRAY_COUNT(sEnFrPointers.frogs); frogIndex++) {
        frogLoop1 = sEnFrPointers.frogs[frogIndex];
        if (frogLoop1 == NULL) {
            osSyncPrintf(VT_COL(RED, WHITE));
            // "There are no frogs!?"
            osSyncPrintf("%s[%d]カエルがいない！？\n", "../z_en_fr.c", 1604);
            osSyncPrintf(VT_RST);
            return;
        } else if (frogLoop1->isDeactivating != true) {
            return;
        }
    }

    for (frogIndex = 0; frogIndex < ARRAY_COUNT(sEnFrPointers.frogs); frogIndex++) {
        frogLoop2 = sEnFrPointers.frogs[frogIndex];
        if (frogLoop2 == NULL) {
            osSyncPrintf(VT_COL(RED, WHITE));
            // "There are no frogs!?"
            osSyncPrintf("%s[%d]カエルがいない！？\n", "../z_en_fr.c", 1618);
            osSyncPrintf(VT_RST);
            return;
        }
        frogLoop2->isDeactivating = false;
    }

    globalCtx->msgCtx.unk_E3EE = 4;
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_FROG_CRY_0);
    if (self->reward == GI_NONE) {
        self->actionFunc = EnFr_Idle;
    } else {
        self->actionFunc = EnFr_GiveReward;
        func_8002F434(&self->actor, globalCtx, self->reward, 30.0f, 100.0f);
    }
}

void EnFr_GiveReward(EnFr* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = EnFr_SetIdle;
    } else {
        func_8002F434(&self->actor, globalCtx, self->reward, 30.0f, 100.0f);
    }
}

void EnFr_SetIdle(EnFr* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx))) {
        self->actionFunc = EnFr_Idle;
    }
}

void EnFr_UpdateIdle(Actor* thisx, GlobalContext* globalCtx) {
    EnFr* self = THIS;

    if (BREG(0)) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, 255, 0, 0, 255, 4, globalCtx->state.gfxCtx);
    }
    self->jumpCounter++;
    self->actionFunc(self, globalCtx);
}

s32 EnFr_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    if ((limbIndex == 7) || (limbIndex == 8)) {
        *dList = NULL;
    }
    return 0;
}

void EnFr_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnFr* self = THIS;

    if ((limbIndex == 7) || (limbIndex == 8)) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fr.c", 1735);
        Matrix_Push();
        func_800D1FD4(&globalCtx->mf_11DA0);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_fr.c", 1738),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, *dList);
        Matrix_Pop();
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fr.c", 1741);
    }
}

void EnFr_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { 0x060059A0, 0x06005BA0 };
    s16 lightRadius;
    EnFr* self = THIS;
    s16 frogIndex = self->actor.params - 1;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_fr.c", 1754);
    func_80093D18(globalCtx->state.gfxCtx);
    // For the frogs 2 HP, the frog with the next note and the butterfly lights up
    lightRadius = self->isButterflyDrawn ? 95 : -1;
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->posButterflyLight.x, self->posButterflyLight.y,
                              self->posButterflyLight.z, 255, 255, 255, lightRadius);
    gDPSetEnvColor(POLY_OPA_DISP++, sEnFrColor[frogIndex].r, sEnFrColor[frogIndex].g, sEnFrColor[frogIndex].b, 255);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTexIndex]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTexIndex]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnFr_OverrideLimbDraw, EnFr_PostLimbDraw, self);
    if (self->isButterflyDrawn) {
        Matrix_Translate(self->posButterfly.x, self->posButterfly.y, self->posButterfly.z, MTXMODE_NEW);
        Matrix_Scale(0.015f, 0.015f, 0.015f, MTXMODE_APPLY);
        Matrix_RotateRPY(self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z, MTXMODE_APPLY);
        SkelAnime_DrawOpa(globalCtx, self->skelAnimeButterfly.skeleton, self->skelAnimeButterfly.jointTable, NULL, NULL,
                          NULL);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_fr.c", 1816);
}
