/*
 * File: z_en_bdfire.c
 * Overlay: ovl_En_Bdfire
 * Description: King Dodongo's Fire Breath
 */

#include "z_en_bdfire.h"
#include "objects/object_kingdodongo/object_kingdodongo.h"

#define FLAGS 0x00000030

#define THIS ((EnBdfire*)thisx)

void EnBdfire_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBdfire_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBdfire_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBdfire_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnBdfire_DrawFire(EnBdfire* self, GlobalContext* globalCtx);
void func_809BC2A4(EnBdfire* self, GlobalContext* globalCtx);
void func_809BC598(EnBdfire* self, GlobalContext* globalCtx);

const ActorInit En_Bdfire_InitVars = {
    0,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_KINGDODONGO,
    sizeof(EnBdfire),
    (ActorFunc)EnBdfire_Init,
    (ActorFunc)EnBdfire_Destroy,
    (ActorFunc)EnBdfire_Update,
    (ActorFunc)EnBdfire_Draw,
};

void EnBdfire_SetupAction(EnBdfire* self, EnBdfireActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnbdFire_SetupDraw(EnBdfire* self, EnBdfireDrawFunc drawFunc) {
    self->drawFunc = drawFunc;
}

void EnBdfire_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBdfire* self = THIS;
    s32 pad;

    Actor_SetScale(&self->actor, 0.6f);
    EnbdFire_SetupDraw(self, EnBdfire_DrawFire);
    if (self->actor.params < 0) {
        EnBdfire_SetupAction(self, func_809BC2A4);
        self->actor.scale.x = 2.8f;
        self->unk_154 = 90;
        Lights_PointNoGlowSetInfo(&self->lightInfoNoGlow, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 255, 255, 255, 300);
        self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfoNoGlow);
    } else {
        EnBdfire_SetupAction(self, func_809BC598);
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
        self->actor.speedXZ = 30.0f;
        self->unk_154 = (25 - (s32)(self->actor.params * 0.8f));
        if (self->unk_154 < 0) {
            self->unk_154 = 0;
        }
        self->unk_188 = 4.2000003f - (self->actor.params * 0.25f * 0.6f);

        if (self->unk_188 < 0.90000004f) {
            self->unk_188 = 0.90000004f;
        }
        self->unk_18C = 255.0f - (self->actor.params * 10.0f);
        if (self->unk_18C < 20.0f) {
            self->unk_18C = 20.0f;
        }
        self->unk_156 = (Rand_ZeroOne() * 8.0f);
    }
}

void EnBdfire_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBdfire* self = THIS;

    if (self->actor.params < 0) {
        LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
    }
}

void func_809BC2A4(EnBdfire* self, GlobalContext* globalCtx) {
    BossDodongo* kingDodongo;
    s32 temp;

    kingDodongo = (BossDodongo*)self->actor.parent;
    self->actor.world.pos.x = kingDodongo->firePos.x;
    self->actor.world.pos.y = kingDodongo->firePos.y;
    self->actor.world.pos.z = kingDodongo->firePos.z;
    if (kingDodongo->unk_1E2 == 0) {
        Math_SmoothStepToF(&self->actor.scale.x, 0.0f, 1.0f, 0.6f, 0.0f);
        if (Math_SmoothStepToF(&self->unk_18C, 0.0f, 1.0f, 20.0f, 0.0f) == 0.0f) {
            Actor_Kill(&self->actor);
        }
    } else {
        if (self->unk_154 < 70) {
            Math_SmoothStepToF(&self->unk_18C, 128.0f, 0.1f, 1.5f, 0.0f);
            Math_SmoothStepToF(&self->unk_190, 255.0f, 1.0f, 3.8249998f, 0.0f);
            Math_SmoothStepToF(&self->unk_194, 100.0f, 1.0f, 1.5f, 0.0f);
        }
        if (self->unk_154 == 0) {
            temp = 0;
        } else {
            self->unk_154--;
            temp = self->unk_154;
        }
        if (temp == 0) {
            Math_SmoothStepToF(&self->actor.scale.x, 0.0f, 1.0f, 0.3f, 0.0f);
            Math_SmoothStepToF(&self->unk_190, 0.0f, 1.0f, 25.5f, 0.0f);
            Math_SmoothStepToF(&self->unk_194, 0.0f, 1.0f, 10.0f, 0.0f);
            if (Math_SmoothStepToF(&self->unk_18C, 0.0f, 1.0f, 10.0f, 0.0f) == 0.0f) {
                Actor_Kill(&self->actor);
            }
        }
        Actor_SetScale(&self->actor, self->actor.scale.x);
        Lights_PointSetColorAndRadius(&self->lightInfoNoGlow, self->unk_190, self->unk_194, 0, 300);
    }
}

void func_809BC598(EnBdfire* self, GlobalContext* globalCtx) {
    s16 phi_v1_2;
    Player* player = GET_PLAYER(globalCtx);
    f32 distToBurn;
    BossDodongo* bossDodongo;
    s16 i;
    s16 phi_v1;
    s32 temp;

    bossDodongo = ((BossDodongo*)self->actor.parent);
    self->unk_158 = bossDodongo->unk_1A2;
    phi_v1_2 = 0;
    if (self->actor.params == 0) {
        Audio_PlaySoundGeneral(NA_SE_EN_DODO_K_FIRE - SFX_FLAG, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
    Math_SmoothStepToF(&self->actor.scale.x, self->unk_188, 0.3f, 0.5f, 0.0f);
    Actor_SetScale(&self->actor, self->actor.scale.x);
    if (self->actor.world.pos.x < -1390.0f) {
        if (self->actor.velocity.x < -10.0f) {
            self->actor.world.pos.x = -1390.0f;
            phi_v1_2 = 1;
        }
    }
    if ((self->actor.world.pos.x > -390.0f) && (self->actor.velocity.x > 10.0f)) {
        self->actor.world.pos.x = -390.0f;
        phi_v1_2 = 1;
    }
    if ((self->actor.world.pos.z > -2804.0f) && (self->actor.velocity.z > 10.0f)) {
        self->actor.world.pos.z = -2804.0f;
        phi_v1_2 = 1;
    }
    if ((self->actor.world.pos.z < -3804.0f) && (self->actor.velocity.z < -10.0f)) {
        self->actor.world.pos.z = -3804.0f;
        phi_v1_2 = 1;
    }
    if (phi_v1_2 != 0) {
        if (self->unk_158 == 0) {
            self->actor.world.rot.y += 0x4000;
        } else {
            self->actor.world.rot.y -= 0x4000;
        }
    }
    if (self->unk_154 == 0) {
        temp = 0;
    } else {
        self->unk_154--;
        temp = self->unk_154;
    }
    if (temp == 0) {
        Math_SmoothStepToF(&self->unk_18C, 0.0f, 1.0f, 10.0f, 0.0f);
        if (self->unk_18C < 10.0f) {
            Actor_Kill(&self->actor);
            return;
        }
    } else if (!player->isBurning) {
        distToBurn = (self->actor.scale.x * 130.0f) / 4.2000003f;
        if (self->actor.xyzDistToPlayerSq < SQ(distToBurn)) {
            for (i = 0; i < 18; i++) {
                player->flameTimers[i] = Rand_S16Offset(0, 200);
            }
            player->isBurning = true;
            func_8002F6D4(globalCtx, &self->actor, 20.0f, self->actor.world.rot.y, 0.0f, 8);
            osSyncPrintf("POWER\n");
        }
    }
}

void EnBdfire_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBdfire* self = THIS;

    self->unk_156++;
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
}

void EnBdfire_DrawFire(EnBdfire* self, GlobalContext* globalCtx) {
    static void* D_809BCB10[] = {
        0x060264E0, 0x060274E0, 0x060284E0, 0x060294E0, 0x0602A4E0, 0x0602B4E0, 0x0602C4E0, 0x0602D4E0,
    };
    s16 temp;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bdfire.c", 612);
    temp = self->unk_156 & 7;
    func_800D1FD4(&globalCtx->mf_11DA0);
    func_80094BC4(globalCtx->state.gfxCtx);
    POLY_XLU_DISP = func_80094968(POLY_XLU_DISP);
    gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0,
                      ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0,
                      ENVIRONMENT);
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 100, (s8)self->unk_18C);
    gDPSetEnvColor(POLY_XLU_DISP++, 200, 0, 0, 0);
    gSPSegment(POLY_XLU_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_809BCB10[temp]));
    Matrix_Translate(0.0f, 11.0f, 0.0f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_bdfire.c", 647),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, object_kingdodongo_DL_01D950);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bdfire.c", 651);
}

void EnBdfire_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBdfire* self = THIS;

    self->drawFunc(self, globalCtx);
}
