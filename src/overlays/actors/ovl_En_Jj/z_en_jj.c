/*
 * File: z_en_jj.c
 * Overlay: ovl_En_Jj
 * Description: Lord Jabu-Jabu
 */

#include "z_en_jj.h"
#include "objects/object_jj/object_jj.h"
#include "overlays/actors/ovl_Eff_Dust/z_eff_dust.h"

#define FLAGS 0x00000030

#define THIS ((EnJj*)thisx)

typedef enum {
    /* 0 */ JABUJABU_EYE_OPEN,
    /* 1 */ JABUJABU_EYE_HALF,
    /* 2 */ JABUJABU_EYE_CLOSED,
    /* 3 */ JABUJABU_EYE_MAX
} EnJjEyeState;

void EnJj_Init(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Update(Actor* thisx, GlobalContext* globalCtx);
void EnJj_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnJj_UpdateStaticCollision(Actor* thisx, GlobalContext* globalCtx);
void EnJj_WaitToOpenMouth(EnJj* self, GlobalContext* globalCtx);
void EnJj_WaitForFish(EnJj* self, GlobalContext* globalCtx);
void EnJj_BeginCutscene(EnJj* self, GlobalContext* globalCtx);
void EnJj_RemoveDust(EnJj* self, GlobalContext* globalCtx);

const ActorInit En_Jj_InitVars = {
    ACTOR_EN_JJ,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_JJ,
    sizeof(EnJj),
    (ActorFunc)EnJj_Init,
    (ActorFunc)EnJj_Destroy,
    (ActorFunc)EnJj_Update,
    (ActorFunc)EnJj_Draw,
};

static s32 sUnused = 0;

#include "z_en_jj_cutscene_data.c" EARLY

static s32 sUnused2[] = { 0, 0 };

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000004, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 170, 150, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 87, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 3300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1100, ICHAIN_STOP),
};

void EnJj_SetupAction(EnJj* self, EnJjActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnJj_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnJj* self = THIS;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    ActorShape_Init(&self->dyna.actor.shape, 0.0f, NULL, 0.0f);

    switch (self->dyna.actor.params) {
        case JABUJABU_MAIN:
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gJabuJabuSkel, &gJabuJabuAnim, self->jointTable,
                               self->morphTable, 22);
            Animation_PlayLoop(&self->skelAnime, &gJabuJabuAnim);
            self->unk_30A = 0;
            self->eyeIndex = 0;
            self->blinkTimer = 0;
            self->extraBlinkCounter = 0;
            self->extraBlinkTotal = 0;

            if (gSaveContext.eventChkInf[3] & 0x400) { // Fish given
                EnJj_SetupAction(self, EnJj_WaitToOpenMouth);
            } else {
                EnJj_SetupAction(self, EnJj_WaitForFish);
            }

            self->bodyCollisionActor = (DynaPolyActor*)Actor_SpawnAsChild(
                &globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_EN_JJ, self->dyna.actor.world.pos.x - 10.0f,
                self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z, 0, self->dyna.actor.world.rot.y, 0,
                JABUJABU_COLLISION);
            DynaPolyActor_Init(&self->dyna, 0);
            CollisionHeader_GetVirtual(&gJabuJabuHeadCol, &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, &self->dyna.actor, &sCylinderInit);
            self->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;
            break;

        case JABUJABU_COLLISION:
            DynaPolyActor_Init(&self->dyna, 0);
            CollisionHeader_GetVirtual(&gJabuJabuBodyCol, &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
            func_8003ECA8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            self->dyna.actor.update = EnJj_UpdateStaticCollision;
            self->dyna.actor.draw = NULL;
            Actor_SetScale(&self->dyna.actor, 0.087f);
            break;

        case JABUJABU_UNUSED_COLLISION:
            DynaPolyActor_Init(&self->dyna, 0);
            CollisionHeader_GetVirtual(&gJabuJabuUnusedCol, &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
            self->dyna.actor.update = EnJj_UpdateStaticCollision;
            self->dyna.actor.draw = NULL;
            Actor_SetScale(&self->dyna.actor, 0.087f);
            break;
    }
}

void EnJj_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnJj* self = THIS;

    switch (self->dyna.actor.params) {
        case JABUJABU_MAIN:
            DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            Collider_DestroyCylinder(globalCtx, &self->collider);
            break;

        case JABUJABU_COLLISION:
        case JABUJABU_UNUSED_COLLISION:
            DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
            break;
    }
}

/**
 * Blink routine. Blinks at the end of each randomised blinkTimer cycle. If extraBlinkCounter is not zero, blink that
 * many more times before resuming random blinkTimer cycles. extraBlinkTotal can be set to a positive number to blink
 * that many extra times at the end of every blinkTimer cycle, but the actor always sets it to zero, so only one
 * multiblink happens when extraBlinkCounter is nonzero.
 */
void EnJj_Blink(EnJj* self) {
    if (self->blinkTimer > 0) {
        self->blinkTimer--;
    } else {
        self->eyeIndex++;
        if (self->eyeIndex >= JABUJABU_EYE_MAX) {
            self->eyeIndex = JABUJABU_EYE_OPEN;
            if (self->extraBlinkCounter > 0) {
                self->extraBlinkCounter--;
            } else {
                self->blinkTimer = Rand_S16Offset(20, 20);
                self->extraBlinkCounter = self->extraBlinkTotal;
            }
        }
    }
}

void EnJj_OpenMouth(EnJj* self, GlobalContext* globalCtx) {
    DynaPolyActor* bodyCollisionActor = self->bodyCollisionActor;

    if (self->mouthOpenAngle >= -5200) {
        self->mouthOpenAngle -= 102;

        if (self->mouthOpenAngle < -2600) {
            func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, bodyCollisionActor->bgId);
        }
    }
}

void EnJj_WaitToOpenMouth(EnJj* self, GlobalContext* globalCtx) {
    if (self->dyna.actor.xzDistToPlayer < 300.0f) {
        EnJj_SetupAction(self, EnJj_OpenMouth);
    }
}

void EnJj_WaitForFish(EnJj* self, GlobalContext* globalCtx) {
    static Vec3f feedingSpot = { -1589.0f, 53.0f, -43.0f };
    Player* player = GET_PLAYER(globalCtx);

    if ((Math_Vec3f_DistXZ(&feedingSpot, &player->actor.world.pos) < 300.0f) &&
        globalCtx->isPlayerDroppingFish(globalCtx)) {
        self->cutsceneCountdownTimer = 100;
        EnJj_SetupAction(self, EnJj_BeginCutscene);
    }

    self->collider.dim.pos.x = -1245;
    self->collider.dim.pos.y = 20;
    self->collider.dim.pos.z = -48;
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnJj_BeginCutscene(EnJj* self, GlobalContext* globalCtx) {
    DynaPolyActor* bodyCollisionActor = self->bodyCollisionActor;

    if (self->cutsceneCountdownTimer > 0) {
        self->cutsceneCountdownTimer--;
    } else {
        EnJj_SetupAction(self, EnJj_RemoveDust);
        globalCtx->csCtx.segment = &D_80A88164;
        gSaveContext.cutsceneTrigger = 1;
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, bodyCollisionActor->bgId);
        func_8005B1A4(GET_ACTIVE_CAM(globalCtx));
        gSaveContext.eventChkInf[3] |= 0x400;
        func_80078884(NA_SE_SY_CORRECT_CHIME);
    }
}

void EnJj_CutsceneUpdate(EnJj* self, GlobalContext* globalCtx) {
    switch (globalCtx->csCtx.npcActions[2]->action) {
        case 1:
            if (self->unk_30A & 2) {
                self->eyeIndex = 0;
                self->blinkTimer = Rand_S16Offset(20, 20);
                self->extraBlinkCounter = 0;
                self->extraBlinkTotal = 0;
                self->unk_30A ^= 2;
            }
            break;

        case 2:
            self->unk_30A |= 1;

            if (!(self->unk_30A & 8)) {
                self->dust = Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_EFF_DUST,
                                                -1100.0f, 105.0f, -27.0f, 0, 0, 0, EFF_DUST_TYPE_0);
                self->unk_30A |= 8;
            }
            break;

        case 3:
            if (!(self->unk_30A & 2)) {
                self->eyeIndex = 0;
                self->blinkTimer = 0;
                self->extraBlinkCounter = 1;
                self->extraBlinkTotal = 0;
                self->unk_30A |= 2;
            }
            break;
    }

    if (self->unk_30A & 1) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_JABJAB_BREATHE - SFX_FLAG);

        if (self->mouthOpenAngle >= -5200) {
            self->mouthOpenAngle -= 102;
        }
    }
}

void EnJj_RemoveDust(EnJj* self, GlobalContext* globalCtx) {
    Actor* dust;

    if (!(self->unk_30A & 4)) {
        self->unk_30A |= 4;
        dust = self->dust;

        if (dust != NULL) {
            Actor_Kill(dust);
            self->dyna.actor.child = NULL;
        }
    }
}

void EnJj_UpdateStaticCollision(Actor* thisx, GlobalContext* globalCtx) {
}

void EnJj_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnJj* self = THIS;

    if ((globalCtx->csCtx.state != CS_STATE_IDLE) && (globalCtx->csCtx.npcActions[2] != NULL)) {
        EnJj_CutsceneUpdate(self, globalCtx);
    } else {
        self->actionFunc(self, globalCtx);

        if (self->skelAnime.curFrame == 41.0f) {
            Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_JABJAB_GROAN);
        }
    }

    EnJj_Blink(self);
    SkelAnime_Update(&self->skelAnime);
    Actor_SetScale(&self->dyna.actor, 0.087f);

    // Head
    self->skelAnime.jointTable[10].z = self->mouthOpenAngle;
}

void EnJj_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    static void* eyeTextures[] = { gJabuJabuEyeOpenTex, gJabuJabuEyeHalfTex, gJabuJabuEyeClosedTex };
    GlobalContext* globalCtx = globalCtx2;
    EnJj* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_jj.c", 879);

    func_800943C8(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, (cosf(self->skelAnime.curFrame * (M_PI / 41.0f)) * 10.0f) - 10.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(10.0f, 10.0f, 10.0f, MTXMODE_APPLY);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeIndex]));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_jj.c", 898);
}
