#include "z_en_ex_ruppy.h"
#include "vt.h"
#include "../ovl_En_Diving_Game/z_en_diving_game.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000010

#define THIS ((EnExRuppy*)thisx)

void EnExRuppy_Init(Actor* thisx, GlobalContext* globalCtx);
void EnExRuppy_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnExRuppy_Update(Actor* thisx, GlobalContext* globalCtx);
void EnExRuppy_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnExRuppy_DropIntoWater(EnExRuppy* self, GlobalContext* globalCtx);
void EnExRuppy_WaitToBlowUp(EnExRuppy* self, GlobalContext* globalCtx);
void EnExRuppy_WaitAsCollectible(EnExRuppy* self, GlobalContext* globalCtx);
void EnExRuppy_GalleryTarget(EnExRuppy* self, GlobalContext* globalCtx);
void EnExRuppy_EnterWater(EnExRuppy* self, GlobalContext* globalCtx);
void EnExRuppy_Sink(EnExRuppy* self, GlobalContext* globalCtx);
void EnExRuppy_WaitInGame(EnExRuppy* self, GlobalContext* globalCtx);
void EnExRuppy_Kill(EnExRuppy* self, GlobalContext* globalCtx);

static s16 sEnExRuppyCollectibleTypes[] = {
    ITEM00_RUPEE_GREEN, ITEM00_RUPEE_BLUE, ITEM00_RUPEE_RED, ITEM00_RUPEE_ORANGE, ITEM00_RUPEE_PURPLE,
};

// Unused, as the function sets these directly
static s16 sRupeeValues[] = {
    1, 5, 20, 500, 50,
};

const ActorInit En_Ex_Ruppy_InitVars = {
    ACTOR_EN_EX_RUPPY,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnExRuppy),
    (ActorFunc)EnExRuppy_Init,
    (ActorFunc)EnExRuppy_Destroy,
    (ActorFunc)EnExRuppy_Update,
    (ActorFunc)EnExRuppy_Draw,
};

void EnExRuppy_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnExRuppy* self = THIS;
    EnDivingGame* divingGame;
    f32 temp1;
    f32 temp2;
    s16 temp3;

    self->type = self->actor.params;
    // "Index"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ インデックス ☆☆☆☆☆ %x\n" VT_RST, self->type);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);

    switch (self->type) {
        case 0:
            self->unk_160 = 0.01f;
            Actor_SetScale(&self->actor, self->unk_160);
            self->actor.room = -1;
            self->actor.gravity = 0.0f;

            // If you haven't won the diving game before, you will always get 5 rupees
            if (!(gSaveContext.eventChkInf[3] & 0x100)) {
                self->rupeeValue = 5;
                self->colorIdx = 1;
            } else {
                temp1 = 200.99f;
                if (self->actor.parent != NULL) {
                    divingGame = (EnDivingGame*)self->actor.parent;
                    if (divingGame->actor.update != NULL) {
                        temp2 = divingGame->extraWinCount * 10.0f;
                        temp1 += temp2;
                    }
                }

                temp3 = Rand_ZeroFloat(temp1);
                if ((temp3 >= 0) && (temp3 < 40)) {
                    self->rupeeValue = 1;
                    self->colorIdx = 0;
                } else if ((temp3 >= 40) && (temp3 < 170)) {
                    self->rupeeValue = 5;
                    self->colorIdx = 1;
                } else if ((temp3 >= 170) && (temp3 < 190)) {
                    self->rupeeValue = 20;
                    self->colorIdx = 2;
                } else if ((temp3 >= 190) && (temp3 < 200)) {
                    self->rupeeValue = 50;
                    self->colorIdx = 4;
                } else {
                    self->unk_160 = 0.02f;
                    Actor_SetScale(&self->actor, self->unk_160);
                    self->rupeeValue = 500;
                    self->colorIdx = 3;
                    if (self->actor.parent != NULL) {
                        divingGame = (EnDivingGame*)self->actor.parent;
                        if (divingGame->actor.update != NULL) {
                            divingGame->extraWinCount = 0;
                        }
                    }
                }
            }

            self->actor.shape.shadowScale = 7.0f;
            self->actor.shape.yOffset = 700.0f;
            self->unk_15A = self->actor.world.rot.z;
            self->actor.world.rot.z = 0;
            self->timer = 30;
            self->actor.flags &= ~1;
            self->actionFunc = EnExRuppy_DropIntoWater;
            break;

        case 1:
        case 2: // Giant pink ruppe that explodes when you touch it
            if (self->type == 1) {
                Actor_SetScale(&self->actor, 0.1f);
                self->colorIdx = 4;
            } else {
                Actor_SetScale(thisx, 0.02f);
                self->colorIdx = (s16)Rand_ZeroFloat(3.99f) + 1;
            }
            self->actor.gravity = -3.0f;
            // "Wow Coin"
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ わーなーコイン ☆☆☆☆☆ \n" VT_RST);
            self->actor.shape.shadowScale = 6.0f;
            self->actor.shape.yOffset = 700.0f;
            self->actor.flags &= ~1;
            self->actionFunc = EnExRuppy_WaitToBlowUp;
            break;

        case 3: // Spawned by the guard in Hyrule courtyard
            Actor_SetScale(&self->actor, 0.02f);
            self->colorIdx = 0;
            switch ((s16)Rand_ZeroFloat(30.99f)) {
                case 0:
                    self->colorIdx = 2;
                    break;
                case 10:
                case 20:
                case 30:
                    self->colorIdx = 1;
                    break;
            }
            self->actor.gravity = -3.0f;
            // "Normal rupee"
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ ノーマルルピー ☆☆☆☆☆ \n" VT_RST);
            self->actor.shape.shadowScale = 6.0f;
            self->actor.shape.yOffset = 700.0f;
            self->actor.flags &= ~1;
            self->actionFunc = EnExRuppy_WaitAsCollectible;
            break;

        case 4: // Progress markers in the shooting gallery
            self->actor.gravity = -3.0f;
            self->actor.flags &= ~1;
            Actor_SetScale(&self->actor, 0.01f);
            self->actor.shape.shadowScale = 6.0f;
            self->actor.shape.yOffset = -700.0f;
            self->actionFunc = EnExRuppy_GalleryTarget;
            break;
    }
}

void EnExRuppy_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnExRuppy_SpawnSparkles(EnExRuppy* self, GlobalContext* globalCtx, s16 numSparkles, s32 movementType) {
    static Vec3f velocities[] = { { 0.0f, 0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    static Vec3f accelerations[] = { { 0.0f, 0.01f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;
    s32 i;
    s16 scale;
    s16 life;

    if (numSparkles < 1) {
        numSparkles = 1;
    }

    primColor.r = 255;
    primColor.g = 255;
    primColor.b = 0;
    envColor.r = 255;
    envColor.g = 255;
    envColor.b = 255;
    velocity = velocities[movementType];
    accel = accelerations[movementType];
    scale = 3000;
    life = 16;

    for (i = 0; i < numSparkles; i++) {
        if (movementType == 1) {
            accel.x = Rand_CenteredFloat(20.0f);
            accel.z = Rand_CenteredFloat(20.0f);
            scale = 5000;
            life = 20;
        }
        pos.x = (Rand_ZeroOne() - 0.5f) * 10.0f + self->actor.world.pos.x;
        pos.y = (Rand_ZeroOne() - 0.5f) * 10.0f + (self->actor.world.pos.y + self->unk_160 * 600.0f);
        pos.z = (Rand_ZeroOne() - 0.5f) * 10.0f + self->actor.world.pos.z;
        EffectSsKiraKira_SpawnDispersed(globalCtx, &pos, &velocity, &accel, &primColor, &envColor, scale, life);
    }
}

void EnExRuppy_DropIntoWater(EnExRuppy* self, GlobalContext* globalCtx) {
    EnDivingGame* divingGame;

    self->actor.shape.rot.y += 0x7A8;
    Math_ApproachF(&self->actor.gravity, -2.0f, 0.3f, 1.0f);
    EnExRuppy_SpawnSparkles(self, globalCtx, 2, 0);
    func_80078884(NA_SE_EV_RAINBOW_SHOWER - SFX_FLAG);
    divingGame = (EnDivingGame*)self->actor.parent;
    if ((divingGame != NULL) && (divingGame->actor.update != NULL) &&
        ((divingGame->unk_296 == 0) || (self->actor.bgCheckFlags & 0x20) || (self->timer == 0))) {
        self->invisible = true;
        self->actor.speedXZ = 0.0f;
        self->actor.velocity.x = self->actor.velocity.y = self->actor.velocity.z = 0.0f;
        self->actor.gravity = 0.0f;
        func_80078914(&self->actor.projectedPos, NA_SE_EV_BOMB_DROP_WATER);
        self->actionFunc = EnExRuppy_EnterWater;
    }
}

void EnExRuppy_EnterWater(EnExRuppy* self, GlobalContext* globalCtx) {
    EnDivingGame* divingGame = (EnDivingGame*)self->actor.parent;
    f32 temp_f2;

    if ((divingGame != NULL) && (divingGame->actor.update != NULL) && (divingGame->unk_2A2 == 2)) {
        self->invisible = false;
        self->actor.world.pos.x = ((Rand_ZeroOne() - 0.5f) * 300.0f) + -260.0f;
        self->actor.world.pos.y = ((Rand_ZeroOne() - 0.5f) * 200.0f) + 370.0f;
        temp_f2 = self->unk_15A * -50.0f;
        if (!(gSaveContext.eventChkInf[3] & 0x100)) {
            temp_f2 += -500.0f;
            self->actor.world.pos.z = ((Rand_ZeroOne() - 0.5f) * 80.0f) + temp_f2;
        } else {
            temp_f2 += -300.0f;
            self->actor.world.pos.z = ((Rand_ZeroOne() - 0.5f) * 60.0f) + temp_f2;
        }
        self->actionFunc = EnExRuppy_Sink;
        self->actor.gravity = -1.0f;
    }
}

void EnExRuppy_Sink(EnExRuppy* self, GlobalContext* globalCtx) {
    EnDivingGame* divingGame;
    Vec3f pos;
    s32 pad;

    if ((self->actor.bgCheckFlags & 0x20) && (self->actor.yDistToWater > 15.0f)) {
        pos = self->actor.world.pos;
        pos.y += self->actor.yDistToWater;
        self->actor.velocity.y = -1.0f;
        self->actor.gravity = -0.2f;
        EffectSsGSplash_Spawn(globalCtx, &pos, 0, 0, 0, 800);
        func_80078914(&self->actor.projectedPos, NA_SE_EV_BOMB_DROP_WATER);
        self->actionFunc = EnExRuppy_WaitInGame;
    }
    divingGame = (EnDivingGame*)self->actor.parent;
    if ((divingGame != NULL) && (divingGame->actor.update != NULL) && (divingGame->phase == ENDIVINGGAME_PHASE_ENDED)) {
        self->timer = 20;
        self->actionFunc = EnExRuppy_Kill;
    }
}

void EnExRuppy_WaitInGame(EnExRuppy* self, GlobalContext* globalCtx) {
    EnDivingGame* divingGame;
    Vec3f D_80A0B388 = { 0.0f, 0.1f, 0.0f };
    Vec3f D_80A0B394 = { 0.0f, 0.0f, 0.0f };
    f32 localConst = 30.0f;

    if (self->timer == 0) {
        self->timer = 10;
        EffectSsBubble_Spawn(globalCtx, &self->actor.world.pos, 0.0f, 5.0f, 5.0f, Rand_ZeroFloat(0.03f) + 0.07f);
    }
    if (self->actor.parent != NULL) {
        divingGame = (EnDivingGame*)self->actor.parent;
        if (divingGame->actor.update != NULL) {
            if (divingGame->phase == ENDIVINGGAME_PHASE_ENDED) {
                self->timer = 20;
                self->actionFunc = EnExRuppy_Kill;
                if (1) {}
            } else if (self->actor.xyzDistToPlayerSq < SQ(localConst)) {
                Rupees_ChangeBy(self->rupeeValue);
                func_80078884(NA_SE_SY_GET_RUPY);
                divingGame->grabbedRupeesCounter++;
                Actor_Kill(&self->actor);
            }
        } else {
            Actor_Kill(&self->actor);
        }
    }
}

void EnExRuppy_Kill(EnExRuppy* self, GlobalContext* globalCtx) {
    self->invisible += 1;
    self->invisible &= 1; // Net effect is self->invisible = !self->invisible;
    if (self->timer == 0) {
        Actor_Kill(&self->actor);
    }
}

typedef struct {
    /* 0x000 */ Actor actor;
    /* 0x14C */ char unk_14C[0x11A];
    /* 0x226 */ s16 unk_226;
} EnExRuppyParentActor; // Unclear what actor was intended to spawn self.

void EnExRuppy_WaitToBlowUp(EnExRuppy* self, GlobalContext* globalCtx) {
    EnExRuppyParentActor* parent;
    Vec3f accel = { 0.0f, 0.1f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    f32 distToBlowUp = 50.0f;
    s16 explosionScale;
    s16 explosionScaleStep;

    if (self->type == 2) {
        distToBlowUp = 30.0f;
    }
    if (self->actor.xyzDistToPlayerSq < SQ(distToBlowUp)) {
        parent = (EnExRuppyParentActor*)self->actor.parent;
        if (parent != NULL) {
            if (parent->actor.update != NULL) {
                parent->unk_226 = 1;
            }
        } else {
            // "That idiot! error"
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ そ、そんなばかな！エラー！！！！！ ☆☆☆☆☆ \n" VT_RST);
        }
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ バカめ！ ☆☆☆☆☆ \n" VT_RST); // "Stupid!"
        explosionScale = 100;
        explosionScaleStep = 30;
        if (self->type == 2) {
            explosionScale = 20;
            explosionScaleStep = 6;
        }
        EffectSsBomb2_SpawnLayered(globalCtx, &self->actor.world.pos, &velocity, &accel, explosionScale,
                                   explosionScaleStep);
        func_8002F71C(globalCtx, &self->actor, 2.0f, self->actor.yawTowardsPlayer, 0.0f);
        Audio_PlayActorSound2(&self->actor, NA_SE_IT_BOMB_EXPLOSION);
        Actor_Kill(&self->actor);
    }
}

void EnExRuppy_WaitAsCollectible(EnExRuppy* self, GlobalContext* globalCtx) {
    f32 localConst = 30.0f;

    if (self->actor.xyzDistToPlayerSq < SQ(localConst)) {
        func_80078884(NA_SE_SY_GET_RUPY);
        Item_DropCollectible(globalCtx, &self->actor.world.pos, (sEnExRuppyCollectibleTypes[self->colorIdx] | 0x8000));
        Actor_Kill(&self->actor);
    }
}

void EnExRuppy_GalleryTarget(EnExRuppy* self, GlobalContext* globalCtx) {
    if (self->galleryFlag) {
        Math_ApproachF(&self->actor.shape.yOffset, 700.0f, 0.5f, 200.0f);
    } else {
        Math_ApproachF(&self->actor.shape.yOffset, -700.0f, 0.5f, 200.0f);
    }
}

void EnExRuppy_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnExRuppy* self = THIS;

    self->actor.shape.rot.y += 1960;
    self->actionFunc(self, globalCtx);
    if (self->timer != 0) {
        self->timer--;
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 50.0f, 0x1C);
}

void EnExRuppy_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* rupeeTextures[] = {
        gRupeeGreenTex, gRupeeBlueTex, gRupeeRedTex, gRupeePinkTex, gRupeeOrangeTex,
    };
    s32 pad;
    EnExRuppy* self = THIS;

    if (!self->invisible) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ex_ruppy.c", 774);

        func_80093D18(globalCtx->state.gfxCtx);
        func_8002EBCC(thisx, globalCtx, 0);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ex_ruppy.c", 780),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(rupeeTextures[self->colorIdx]));
        gSPDisplayList(POLY_OPA_DISP++, gRupeeDL);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ex_ruppy.c", 784);
    }
}
