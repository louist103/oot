/*
 * File: z_en_tr.c
 * Overlay: ovl_En_Tr
 * Description: Koume and Kotake
 */

#include "z_en_tr.h"
#include "objects/object_tr/object_tr.h"

#define FLAGS 0x00000010

#define THIS ((EnTr*)thisx)

void EnTr_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTr_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTr_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTr_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnTr_DoNothing(EnTr* self, GlobalContext* globalCtx);
void EnTr_ShrinkVanish(EnTr* self, GlobalContext* globalCtx);
void EnTr_WaitToReappear(EnTr* self, GlobalContext* globalCtx);
void EnTr_ChooseAction1(EnTr* self, GlobalContext* globalCtx);

void EnTr_UpdateRotation(EnTr* self, GlobalContext* globalCtx, s32 actionIndex);
void func_80B24038(EnTr* self, GlobalContext* globalCtx, s32 actionIndex);
void EnTr_SetStartPosRot(EnTr* self, GlobalContext* globalCtx, s32 actionIndex);

const ActorInit En_Tr_InitVars = {
    ACTOR_EN_TR,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TR,
    sizeof(EnTr),
    (ActorFunc)EnTr_Init,
    (ActorFunc)EnTr_Destroy,
    (ActorFunc)EnTr_Update,
    (ActorFunc)EnTr_Draw,
};

// The first elements of these animation arrays are for Koume, the second for Kotake

static AnimationHeader* unused[] = {
    0x06003FC8,
    0x06001CDC,
};

static AnimationHeader* D_80B24368[] = {
    0x06002BC4, // Turn to look over right shoulder
    0x06000BFC, // Turn to look over right shoulder
};

static AnimationHeader* D_80B24370[] = {
    0x060035CC, // Looking over left shoulder
    0x060013CC, // Looking over right shoulder
};

static AnimationHeader* D_80B24378[] = {
    0x060049C8, // Floating on broom
    0x060049C8, // Floating on broom
};

static AnimationHeader* D_80B24380[] = {
    0x06012E1C, // Casting magic
    0x06012E1C, // Casting magic
};

static f32 D_80B24388[] = { 0.0f, 20.0f, -30.0f, 20.0f, -20.0f, -20.0f, 30.0f };

static f32 D_80B243A4[] = { 0.0f, 30.0f, 0.0f, -30.0f, 30.0f, -30.0f, 0.0f };

// Has to be 1-dimensional to match
static Color_RGBA8 D_80B243C0[4] = {
    { 255, 200, 0, 255 },
    { 255, 0, 0, 255 },
    { 255, 255, 255, 255 },
    { 0, 0, 255, 255 },
};

static void* sEyeTextures[] = {
    0x060086D8, // Open
    0x060094D8, // Half
    0x060098D8, // Closed
};

void EnTr_SetupAction(EnTr* self, EnTrActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnTr_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnTr* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    EnTr_SetupAction(self, EnTr_DoNothing);
    self->unk_2D4 = 0; // Set and not used
    self->actor.child = NULL;
    Actor_SetScale(&self->actor, 0.01f);

    switch (self->actor.params) {
        case TR_KOUME:
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_tr_Skel_011688, &object_tr_Anim_003FC8,
                               self->jointTable, self->morphTable, 27);
            Animation_PlayOnce(&self->skelAnime, &object_tr_Anim_003FC8);
            self->animation = NULL;
            EnTr_SetupAction(self, EnTr_ChooseAction1);
            self->actionIndex = 3;
            break;

        case TR_KOTAKE:
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_tr_Skel_00C530, &object_tr_Anim_001CDC,
                               self->jointTable, self->morphTable, 27);
            Animation_PlayOnce(&self->skelAnime, &object_tr_Anim_001CDC);
            self->animation = NULL;
            EnTr_SetupAction(self, EnTr_ChooseAction1);
            self->actionIndex = 2;
            break;

        default:
            ASSERT(0, "0", "../z_en_tr.c", 277);
            break;
    }
}

void EnTr_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnTr_CrySpellcast(EnTr* self, GlobalContext* globalCtx) {
    if (self->timer == 11) {
        // Both cry in the title screen cutscene, but only Kotake in the in-game cutscene
        if ((self->actor.params != TR_KOUME) || (gSaveContext.sceneSetupIndex == 6)) {
            Audio_PlaySoundGeneral(NA_SE_EN_TWINROBA_SHOOT_VOICE, &D_801333D4, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
    }

    if (self->timer > 0) {
        self->timer--;
    } else if (self->actor.child != NULL) {
        self->actor.child = NULL;
    }
    func_8002F974(&self->actor, NA_SE_EN_TWINROBA_FLY_DEMO - SFX_FLAG);
}

void EnTr_DoNothing(EnTr* self, GlobalContext* globalCtx) {
}

void EnTr_ChooseAction2(EnTr* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if (globalCtx->csCtx.npcActions[self->actionIndex] != NULL) {
            switch (globalCtx->csCtx.npcActions[self->actionIndex]->action) {

                case 4:
                    Actor_SetScale(&self->actor, 0.01f);
                    EnTr_SetupAction(self, EnTr_ShrinkVanish);
                    self->timer = 24;
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DEAD2);
                    break;

                case 6:
                    Animation_Change(&self->skelAnime, D_80B24380[self->actor.params], 1.0f, 0.0f,
                                     Animation_GetLastFrame(D_80B24380[self->actor.params]), ANIMMODE_ONCE, -5.0f);
                    EnTr_SetupAction(self, EnTr_CrySpellcast);
                    self->animation = D_80B24378[self->actor.params];
                    self->timer = 39;
                    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_DEMO_6K,
                                       self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, 0, 0,
                                       0, self->actor.params + 9);
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_FANTOM_MASIC1);
                    break;

                default:
                    func_80B24038(self, globalCtx, self->actionIndex);
                    EnTr_UpdateRotation(self, globalCtx, self->actionIndex);
                    break;
            }
            func_8002F974(&self->actor, NA_SE_EN_TWINROBA_FLY_DEMO - SFX_FLAG);
        }
    }
}

void EnTr_FlyKidnapCutscene(EnTr* self, GlobalContext* globalCtx) {
    Vec3f originalPos = self->actor.world.pos;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if (globalCtx->csCtx.npcActions[self->actionIndex] != NULL) {
            if (globalCtx->csCtx.npcActions[self->actionIndex]->action == 8) {
                func_80B24038(self, globalCtx, self->actionIndex);
                self->actor.world.rot.y = Math_Atan2S(self->actor.velocity.z, self->actor.velocity.x);
                Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.world.rot.y, 10, 0x400, 0x100);
                self->actor.world.rot.y = self->actor.shape.rot.y;
            } else {
                EnTr_SetStartPosRot(self, globalCtx, self->actionIndex);
                self->actor.world.pos.x += Math_SinS(self->timer) * 150.0f;
                self->actor.world.pos.y += -100.0f;
                self->actor.world.pos.z += Math_CosS(self->timer) * 150.0f;

                self->actor.shape.rot.y = (s16)(self->timer) + 0x4000;
                self->timer += 0x400;

                self->actor.velocity.x = self->actor.world.pos.x - originalPos.x;
                self->actor.velocity.y = self->actor.world.pos.y - originalPos.y;
                self->actor.velocity.z = self->actor.world.pos.z - originalPos.z;
            }

            if (globalCtx->csCtx.frames < 670) {
                func_8002F974(&self->actor, NA_SE_EN_TWINROBA_FLY_DEMO - SFX_FLAG);
            }
        }
    }
}

void func_80B23254(EnTr* self, GlobalContext* globalCtx, s32 arg2, f32 arg3, f32 scale) {
    Vec3f pos;
    Vec3f velocity;
    Vec3f accel;
    Vec3f sp58;
    Color_RGBA8* primColor;
    Color_RGBA8* envColor;
    Vec3f cameraEye = GET_ACTIVE_CAM(globalCtx)->eye;
    s16 yaw = Math_Vec3f_Yaw(&cameraEye, &self->actor.world.pos);
    s16 reversePitch = -Math_Vec3f_Pitch(&cameraEye, &self->actor.world.pos);
    f32 sp3C;

    accel.x = accel.z = 0.0f;
    sp3C = Math_SinS(yaw);
    velocity.x = Math_CosS(reversePitch) * (arg3 * sp3C);
    velocity.y = Math_SinS(reversePitch) * arg3;
    sp3C = Math_CosS(yaw);
    velocity.z = Math_CosS(reversePitch) * (arg3 * sp3C);
    accel.y = 0.5f;

    primColor = &D_80B243C0[2 * self->actor.params];
    envColor = &D_80B243C0[2 * self->actor.params + 1];

    sp58 = self->actor.world.pos;
    sp58.x -= velocity.x * 10.0f;
    sp58.y -= velocity.y * 10.0f;
    sp58.z -= velocity.z * 10.0f;

    pos.x = sp58.x + ((D_80B24388[arg2] * scale) * Math_CosS(yaw));
    pos.y = sp58.y + (D_80B243A4[arg2] * scale);
    pos.z = sp58.z - ((D_80B24388[arg2] * scale) * Math_SinS(yaw));
    func_8002829C(globalCtx, &pos, &velocity, &accel, primColor, envColor, (s32)(800.0f * scale), (s32)(80.0f * scale));
}

void EnTr_ShrinkVanish(EnTr* self, GlobalContext* globalCtx) {
    if (self->timer >= 17) {
        self->actor.shape.rot.y = (self->actor.shape.rot.y - (self->timer * 0x28F)) + 0x3D68;
    } else {
        if (self->timer >= 5) {
            Actor_SetScale(&self->actor, self->actor.scale.x * 0.9f);
            self->actor.shape.rot.y = (self->actor.shape.rot.y - (self->timer * 0x28F)) + 0x3D68;
        } else if (self->timer > 0) {
            s32 temp_hi = (self->timer * 2) % 7;

            func_80B23254(self, globalCtx, temp_hi, 5.0f, 0.2f);
            func_80B23254(self, globalCtx, (temp_hi + 1) % 7, 5.0f, 0.2f);
            Actor_SetScale(&self->actor, self->actor.scale.x * 0.9f);
            self->actor.shape.rot.y = (self->actor.shape.rot.y - (self->timer * 0x28F)) + 0x3D68;
        } else {
            EnTr_SetupAction(self, EnTr_WaitToReappear);
            self->actor.draw = NULL;
        }
    }

    if (self->timer == 4) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_BUBLE_DOWN);
    }

    if (self->timer > 0) {
        self->timer--;
    }
}

void EnTr_Reappear(EnTr* self, GlobalContext* globalCtx) {
    if (self->timer >= 31) {
        s32 temp_hi = (self->timer * 2) % 7;

        func_80B23254(self, globalCtx, temp_hi, 5.0f, 1.0f);
        func_80B23254(self, globalCtx, (temp_hi + 1) % 7, 5.0f, 1.0f);
    } else if (self->timer == 30) {
        self->actor.draw = EnTr_Draw;
        self->actor.shape.rot.y += self->timer * 0x1A6;
    } else if (self->timer > 0) {
        self->actor.shape.rot.y += self->timer * 0x1A6;
        Actor_SetScale(&self->actor, (self->actor.scale.x * 0.8f) + 0.002f);
    } else {
        EnTr_SetupAction(self, EnTr_ChooseAction2);
        Actor_SetScale(&self->actor, 0.01f);
    }

    if (self->timer > 0) {
        self->timer--;
    }
    func_8002F974(&self->actor, NA_SE_EN_TWINROBA_FLY_DEMO - SFX_FLAG);
}

void EnTr_WaitToReappear(EnTr* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if ((globalCtx->csCtx.npcActions[self->actionIndex] != NULL) &&
            ((globalCtx->csCtx.npcActions[self->actionIndex]->action == 3) ||
             (globalCtx->csCtx.npcActions[self->actionIndex]->action == 5))) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_TWINROBA_TRANSFORM);
            self->timer = 34;
            EnTr_SetStartPosRot(self, globalCtx, self->actionIndex);
            EnTr_SetupAction(self, EnTr_Reappear);
            Animation_PlayLoop(&self->skelAnime, &object_tr_Anim_0049C8);
            self->animation = NULL;
            Actor_SetScale(&self->actor, 0.003f);
        }
    }
}

void EnTr_TakeOff(EnTr* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(D_80B24378[self->actor.params]);

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if ((globalCtx->csCtx.npcActions[self->actionIndex] != NULL) &&
            (globalCtx->csCtx.npcActions[self->actionIndex]->action == 3)) {
            Animation_Change(&self->skelAnime, D_80B24378[self->actor.params], 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP,
                             -10.0f);
            self->animation = NULL;
            EnTr_SetupAction(self, EnTr_ChooseAction2);
        }
    }
}

void EnTr_TurnLookOverShoulder(EnTr* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(D_80B24368[self->actor.params]);

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if ((globalCtx->csCtx.npcActions[self->actionIndex] != NULL) &&
            (globalCtx->csCtx.npcActions[self->actionIndex]->action == 2)) {
            Animation_Change(&self->skelAnime, D_80B24368[self->actor.params], 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE,
                             -4.0f);
            self->animation = D_80B24370[self->actor.params];
            EnTr_SetupAction(self, EnTr_TakeOff);
        }
    }
}

void EnTr_ChooseAction1(EnTr* self, GlobalContext* globalCtx) {
    u32 frames = globalCtx->gameplayFrames;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if (globalCtx->csCtx.npcActions[self->actionIndex] != NULL) {
            switch (globalCtx->csCtx.npcActions[self->actionIndex]->action) {
                case 1:
                    EnTr_SetStartPosRot(self, globalCtx, self->actionIndex);
                    EnTr_SetupAction(self, EnTr_TurnLookOverShoulder);
                    break;

                case 3:
                    EnTr_SetStartPosRot(self, globalCtx, self->actionIndex);
                    EnTr_SetupAction(self, EnTr_ChooseAction2);
                    Animation_PlayLoop(&self->skelAnime, &object_tr_Anim_0049C8);
                    self->animation = NULL;
                    break;

                case 4:
                    EnTr_SetupAction(self, EnTr_WaitToReappear);
                    self->actor.draw = NULL;
                    break;

                case 7:
                    EnTr_SetupAction(self, EnTr_FlyKidnapCutscene);
                    Animation_PlayLoop(&self->skelAnime, &object_tr_Anim_0049C8);
                    self->animation = NULL;
                    self->timer =
                        ((self->actor.params != TR_KOUME) ? ((u8)frames * 0x400) + 0x8000 : (u8)frames * 0x400);
                    break;
            }
        }
    }
}

void EnTr_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnTr* self = THIS;

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 5);
    self->actionFunc(self, globalCtx);

    if (SkelAnime_Update(&self->skelAnime) != 0) {
        if (self->animation != NULL) {
            if ((self->animation == &object_tr_Anim_0035CC) || (self->animation == &object_tr_Anim_0013CC)) {
                if (self->actor.params != TR_KOUME) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_TWINROBA_LAUGH2);
                } else {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_TWINROBA_LAUGH);
                }
                Animation_PlayLoop(&self->skelAnime, self->animation);
            } else if (self->animation == &object_tr_Anim_0049C8) {
                EnTr_SetupAction(self, EnTr_ChooseAction2);
                Animation_Change(&self->skelAnime, &object_tr_Anim_0049C8, 1.0f, 0.0f,
                                 Animation_GetLastFrame(&object_tr_Anim_0049C8), ANIMMODE_LOOP, -5.0f);
            } else {
                Animation_PlayLoop(&self->skelAnime, self->animation);
            }
            self->animation = NULL;
        } else {
            self->skelAnime.curFrame = 0.0f;
        }
    }
    Actor_SetFocus(&self->actor, 0.0f);

    if (DECR(self->blinkTimer) == 0) {
        self->blinkTimer = Rand_S16Offset(60, 60);
    }
    self->eyeIndex = self->blinkTimer;
    if (self->eyeIndex >= 3) {
        self->eyeIndex = 0;
    }
}

s32 EnTr_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    Vec3f src = { 2300.0f, 0.0f, -600.0f };
    Vec3f dest = { 0.0f, 0.0f, 0.0f };
    EnTr* self = THIS;
    Actor* child = self->actor.child;

    if ((child != NULL) && (limbIndex == 19)) {
        Matrix_MultVec3f(&src, &dest);
        dest.x -= (10.0f * Math_SinS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx))));
        dest.z -= (10.0f * Math_CosS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx))));
        child->world.pos = dest;
    }
    return 0;
}

void EnTr_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnTr* self = THIS;

    if (1) {}

    if ((globalCtx->csCtx.state == CS_STATE_IDLE) || (globalCtx->csCtx.npcActions[self->actionIndex] == 0)) {
        self->actor.shape.shadowDraw = NULL;
    } else {
        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;

        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tr.c", 840);
        func_800943C8(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeIndex]));
        func_8002EBCC(&self->actor, globalCtx, 0);
        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnTr_OverrideLimbDraw, NULL, self);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tr.c", 854);
    }
}

f32 func_80B23FDC(GlobalContext* globalCtx, s32 actionIndex) {
    f32 phi_f2 = Environment_LerpWeight(globalCtx->csCtx.npcActions[actionIndex]->endFrame,
                                        globalCtx->csCtx.npcActions[actionIndex]->startFrame, globalCtx->csCtx.frames);
    phi_f2 = CLAMP_MAX(phi_f2, 1.0f);
    return phi_f2;
}

void func_80B24038(EnTr* self, GlobalContext* globalCtx, s32 actionIndex) {
    Vec3f startPos;
    Vec3f endPos;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 phi_f12;

    startPos.x = globalCtx->csCtx.npcActions[actionIndex]->startPos.x;
    startPos.y = globalCtx->csCtx.npcActions[actionIndex]->startPos.y;
    startPos.z = globalCtx->csCtx.npcActions[actionIndex]->startPos.z;

    endPos.x = globalCtx->csCtx.npcActions[actionIndex]->endPos.x;
    endPos.y = globalCtx->csCtx.npcActions[actionIndex]->endPos.y;
    endPos.z = globalCtx->csCtx.npcActions[actionIndex]->endPos.z;

    temp_f0 = func_80B23FDC(globalCtx, actionIndex);

    startPos.x = ((endPos.x - startPos.x) * temp_f0) + startPos.x;
    startPos.y = ((endPos.y - startPos.y) * temp_f0) + startPos.y;
    startPos.z = ((endPos.z - startPos.z) * temp_f0) + startPos.z;

    endPos.x = (startPos.x - self->actor.world.pos.x) * 0.1f;
    endPos.y = (startPos.y - self->actor.world.pos.y) * 0.1f;
    endPos.z = (startPos.z - self->actor.world.pos.z) * 0.1f;

    temp_f0_2 = sqrtf(SQ(endPos.x) + SQ(endPos.y) + SQ(endPos.z));
    phi_f12 = CLAMP(temp_f0_2, 0.0f, 20.0f);

    if ((temp_f0_2 != phi_f12) && (temp_f0_2 != 0.0f)) {
        endPos.x *= phi_f12 / temp_f0_2;
        endPos.y *= phi_f12 / temp_f0_2;
        endPos.z *= phi_f12 / temp_f0_2;
    }

    Math_StepToF(&self->actor.velocity.x, endPos.x, 1.0f);
    Math_StepToF(&self->actor.velocity.y, endPos.y, 1.0f);
    Math_StepToF(&self->actor.velocity.z, endPos.z, 1.0f);
    func_8002D7EC(&self->actor);
}

void EnTr_UpdateRotation(EnTr* self, GlobalContext* globalCtx, s32 actionIndex) {
    s16 rotY = globalCtx->csCtx.npcActions[actionIndex]->rot.y;
    s32 rotDiff = self->actor.world.rot.y - rotY;
    s32 rotSign;

    if (rotDiff < 0) {
        rotDiff = -rotDiff;
        rotSign = 1;
    } else {
        rotSign = -1;
    }

    if (rotDiff >= 0x8000) {
        rotSign = -rotSign;
        rotDiff = 0x10000 - rotDiff;
    }

    rotDiff *= 0.1f;

    self->actor.world.rot.y += rotDiff * rotSign;
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnTr_SetStartPosRot(EnTr* self, GlobalContext* globalCtx, s32 actionIndex) {
    Vec3f startPos;

    startPos.x = globalCtx->csCtx.npcActions[actionIndex]->startPos.x;
    startPos.y = globalCtx->csCtx.npcActions[actionIndex]->startPos.y;
    startPos.z = globalCtx->csCtx.npcActions[actionIndex]->startPos.z;

    self->actor.world.pos = startPos;
    self->actor.world.rot.y = self->actor.shape.rot.y = globalCtx->csCtx.npcActions[actionIndex]->rot.y;
}
