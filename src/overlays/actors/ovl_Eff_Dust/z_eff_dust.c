/**
 * File z_eff_dust.c
 * Overlay: ovl_Eff_Dust
 * Description: Dust effects
 */

#include "z_eff_dust.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000030

#define THIS ((EffDust*)thisx)

void EffDust_Init(Actor* thisx, GlobalContext* globalCtx);
void EffDust_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EffDust_Update(Actor* thisx, GlobalContext* globalCtx);
void EffDust_Draw(Actor* thisx, GlobalContext* globalCtx);

void EffDust_InitPosAndDistance(EffDust* self);

void EffDust_UpdateFunc_8099DB28(EffDust* self, GlobalContext* globalCtx);
void EffDust_UpdateFunc_8099DD74(EffDust* self, GlobalContext* globalCtx);
void EffDust_UpdateFunc_8099DFC0(EffDust* self, GlobalContext* globalCtx);
void EffDust_DrawFunc_8099E4F4(Actor* thisx, GlobalContext* globalCtx);
void EffDust_DrawFunc_8099E784(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Eff_Dust_InitVars = {
    ACTOR_EFF_DUST,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EffDust),
    (ActorFunc)EffDust_Init,
    (ActorFunc)EffDust_Destroy,
    (ActorFunc)EffDust_Update,
    (ActorFunc)EffDust_Draw,
};

static Gfx D_8099EB60[] = {
    gsSPEndDisplayList(),
};

void EffDust_SetupAction(EffDust* self, EffDustActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EffDust_SetupDraw(EffDust* self, EffDustDrawFunc drawFunc) {
    self->drawFunc = drawFunc;
}

void EffDust_InitPosAndDistance(EffDust* self) {
    s32 i;

    for (i = 0; i < 64; i++) {
        self->initialPositions[i].z = 0.0f;
        self->initialPositions[i].y = 0.0f;
        self->initialPositions[i].x = 0.0f;

        self->distanceTraveled[i] = 1.0f;
    }
    self->index = 0;
}

void EffDust_Init(Actor* thisx, GlobalContext* globalCtx) {
    EffDust* self = THIS;
    EffDustType dustEffect = self->actor.params;

    EffDust_InitPosAndDistance(self);

    switch (dustEffect) {
        case EFF_DUST_TYPE_0:
            EffDust_SetupAction(self, EffDust_UpdateFunc_8099DB28);
            EffDust_SetupDraw(self, EffDust_DrawFunc_8099E4F4);
            self->dy = 0.8f;
            self->dz = 0.8f;
            self->dx = 1.0f;
            self->scalingFactor = 0.1f;
            break;
        case EFF_DUST_TYPE_1:
            EffDust_SetupAction(self, EffDust_UpdateFunc_8099DD74);
            EffDust_SetupDraw(self, EffDust_DrawFunc_8099E4F4);
            self->dx = 0.8f;
            self->dz = 0.8f;
            self->dy = 1.0f;
            self->scalingFactor = 0.5f;
            break;
        case EFF_DUST_TYPE_2:
            EffDust_SetupAction(self, EffDust_UpdateFunc_8099DFC0);
            EffDust_SetupDraw(self, EffDust_DrawFunc_8099E784);
            self->dx = 0.5f;
            self->scalingFactor = 15.0f;
            break;
        case EFF_DUST_TYPE_3:
            EffDust_SetupAction(self, EffDust_UpdateFunc_8099DFC0);
            EffDust_SetupDraw(self, EffDust_DrawFunc_8099E784);
            self->dx = 0.5f;
            self->scalingFactor = 10.0f;
            break;
        case EFF_DUST_TYPE_4:
            EffDust_SetupAction(self, EffDust_UpdateFunc_8099DFC0);
            EffDust_SetupDraw(self, EffDust_DrawFunc_8099E784);
            self->actor.room = -1;
            self->dx = 0.5f;
            self->scalingFactor = 20.0f;
            break;
        default:
            SystemArena_FreeDebug(self, "../z_eff_dust.c", 202);
            break;
    }

    self->life = 10;
}

void EffDust_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EffDust_UpdateFunc_8099DB28(EffDust* self, GlobalContext* globalCtx) {
    s16 theta;
    s16 fi;
    f32* distanceTraveled = self->distanceTraveled;
    s32 i;
    s32 j;

    for (i = 0; i < 64; i++) {
        if ((*distanceTraveled) < 1.0f) {
            *distanceTraveled += 0.05f;
        }
        distanceTraveled++;
    }

    for (j = 0; j < 3; j++) {
        i = self->index & 0x3F;
        if (self->distanceTraveled[i] >= 1.0f) {
            // Spherical coordinate system.
            fi = Rand_CenteredFloat(8192.0f);
            theta = Rand_CenteredFloat(4096.0f);
            self->initialPositions[i].x = -800.0f * Math_CosS(fi) * Math_CosS(theta);
            self->initialPositions[i].y = -800.0f * Math_SinS(theta);
            self->initialPositions[i].z = -800.0f * Math_SinS(fi) * Math_CosS(theta);
            self->distanceTraveled[i] = 0.0f;
            self->index += 1;
        }
    }
}

void EffDust_UpdateFunc_8099DD74(EffDust* self, GlobalContext* globalCtx) {
    s16 theta;
    s16 fi;
    f32* distanceTraveled = self->distanceTraveled;
    s32 i;
    s32 j;

    for (i = 0; i < 64; i++) {
        if ((*distanceTraveled) < 1.0f) {
            *distanceTraveled += 0.03f;
        }
        distanceTraveled++;
    }

    for (j = 0; j < 2; j++) {
        i = self->index & 0x3F;
        if (self->distanceTraveled[i] >= 1.0f) {
            // Spherical coordinate system.
            fi = Rand_CenteredFloat(65536.0f);
            theta = Rand_ZeroFloat(8192.0f);
            self->initialPositions[i].x = 400.0f * Math_CosS(fi) * Math_CosS(theta);
            self->initialPositions[i].y = 400.0f * Math_SinS(theta);
            self->initialPositions[i].z = 400.0f * Math_SinS(fi) * Math_CosS(theta);
            self->distanceTraveled[i] = 0.0f;
            self->index += 1;
        }
    }
}

void EffDust_UpdateFunc_8099DFC0(EffDust* self, GlobalContext* globalCtx) {
    s16 theta;
    Player* player = GET_PLAYER(globalCtx);
    Actor* parent = self->actor.parent;
    f32* distanceTraveled = self->distanceTraveled;
    s32 i;
    s32 j;

    if (parent == NULL || parent->update == NULL || !(player->stateFlags1 & 0x1000)) {
        if (self->life != 0) {
            self->life -= 1;
        } else {
            Actor_Kill(&self->actor);
        }

        for (i = 0; i < 64; i++) {
            if ((*distanceTraveled) < 1.0f) {
                *distanceTraveled += 0.2f;
            }
            distanceTraveled++;
        }

        return;
    }

    for (i = 0; i < 64; i++) {
        if ((*distanceTraveled) < 1.0f) {
            *distanceTraveled += 0.1f;
        }
        distanceTraveled++;
    }

    self->actor.world.pos = parent->world.pos;

    for (j = 0; j < 3; j++) {
        i = self->index & 0x3F;
        if (self->distanceTraveled[i] >= 1.0f) {

            theta = Rand_CenteredFloat(65536.0f);
            switch (self->actor.params) {
                case EFF_DUST_TYPE_2:
                    self->initialPositions[i].x = (Rand_ZeroOne() * 4500.0f) + 700.0f;
                    if (self->initialPositions[i].x > 3000.0f) {
                        self->initialPositions[i].y = (3000.0f * Rand_ZeroOne()) * Math_SinS(theta);
                        self->initialPositions[i].z = (3000.0f * Rand_ZeroOne()) * Math_CosS(theta);
                    } else {
                        self->initialPositions[i].y = 3000.0f * Math_SinS(theta);
                        self->initialPositions[i].z = 3000.0f * Math_CosS(theta);
                    }
                    break;

                case EFF_DUST_TYPE_3:
                    self->initialPositions[i].x = (Rand_ZeroOne() * 2500.0f) + 700.0f;
                    if (self->initialPositions[i].x > 2000.0f) {
                        self->initialPositions[i].y = (2000.0f * Rand_ZeroOne()) * Math_SinS(theta);
                        self->initialPositions[i].z = (2000.0f * Rand_ZeroOne()) * Math_CosS(theta);
                    } else {
                        self->initialPositions[i].y = 2000.0f * Math_SinS(theta);
                        self->initialPositions[i].z = 2000.0f * Math_CosS(theta);
                    }
                    break;

                case EFF_DUST_TYPE_4:
                    self->initialPositions[i].x = (Rand_ZeroOne() * 8500.0f) + 1700.0f;
                    if (self->initialPositions[i].x > 5000.0f) {
                        self->initialPositions[i].y = (4000.0f * Rand_ZeroOne()) * Math_SinS(theta);
                        self->initialPositions[i].z = (4000.0f * Rand_ZeroOne()) * Math_CosS(theta);
                    } else {
                        self->initialPositions[i].y = 4000.0f * Math_SinS(theta);
                        self->initialPositions[i].z = 4000.0f * Math_CosS(theta);
                    }

                    break;
                default:
                    break;
            }

            self->distanceTraveled[i] = 0.0f;
            self->index += 1;
        }
    }
}

void EffDust_Update(Actor* thisx, GlobalContext* globalCtx) {
    EffDust* self = THIS;

    self->actionFunc(self, globalCtx);
}

void EffDust_DrawFunc_8099E4F4(Actor* thisx, GlobalContext* globalCtx2) {
    EffDust* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    Vec3f* initialPositions;
    f32* distanceTraveled;
    s32 i;
    f32 aux;

    OPEN_DISPS(gfxCtx, "../z_eff_dust.c", 425);

    func_80093D18(gfxCtx);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 128, 128, 128, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 128, 128, 128, 0);

    initialPositions = self->initialPositions;
    distanceTraveled = self->distanceTraveled;

    gSPSegment(POLY_XLU_DISP++, 0x08, D_8099EB60);

    for (i = 0; i < 64; i++) {
        if (*distanceTraveled < 1.0f) {
            aux = 1.0f - (*distanceTraveled * *distanceTraveled);
            Matrix_Translate(self->actor.world.pos.x + (initialPositions->x * ((self->dx * aux) + (1.0f - self->dx))),
                             self->actor.world.pos.y + (initialPositions->y * ((self->dy * aux) + (1.0f - self->dy))),
                             self->actor.world.pos.z + (initialPositions->z * ((self->dz * aux) + (1.0f - self->dz))),
                             MTXMODE_NEW);

            Matrix_Scale(self->scalingFactor, self->scalingFactor, self->scalingFactor, MTXMODE_APPLY);
            Matrix_Mult(&globalCtx->mf_11DA0, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_dust.c", 449),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gEffSparklesDL));
        }

        initialPositions++;
        distanceTraveled++;
        // Needed for matching.
        if (0) {}
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_dust.c", 458);
}

void EffDust_DrawFunc_8099E784(Actor* thisx, GlobalContext* globalCtx2) {
    EffDust* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    f32* distanceTraveled;
    Vec3f* initialPositions;
    s32 i;
    f32 aux;
    Player* player = GET_PLAYER(globalCtx);

    OPEN_DISPS(gfxCtx, "../z_eff_dust.c", 472);

    func_80093D18(gfxCtx);

    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
    if (player->unk_858 >= 0.85f) {
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
    } else {
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 0);
    }

    initialPositions = self->initialPositions;
    distanceTraveled = self->distanceTraveled;

    gSPSegment(POLY_XLU_DISP++, 0x08, D_8099EB60);

    for (i = 0; i < 64; i++) {
        if (*distanceTraveled < 1.0f) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, *distanceTraveled * 255);

            // Needed to match.
            if (!self) {}

            aux = 1.0f - (*distanceTraveled * *distanceTraveled);

            Matrix_Mult(&player->mf_9E0, MTXMODE_NEW);

            Matrix_Translate(initialPositions->x * ((self->dx * aux) + (1.0f - self->dx)),
                             initialPositions->y * (1.0f - *distanceTraveled) + 320.0f,
                             initialPositions->z * (1.0f - *distanceTraveled) + -20.0f, MTXMODE_APPLY);

            Matrix_Scale(*distanceTraveled * self->scalingFactor, *distanceTraveled * self->scalingFactor,
                         *distanceTraveled * self->scalingFactor, MTXMODE_APPLY);

            func_800D1FD4(&globalCtx->mf_11DA0);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_dust.c", 506),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gEffSparklesDL));
        }

        initialPositions++;
        distanceTraveled++;
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_dust.c", 515);
}

void EffDust_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EffDust* self = THIS;

    self->drawFunc(thisx, globalCtx);
}
