/*
 * File: z_arrow_fire.c
 * Overlay: ovl_Arrow_Fire
 * Description: Fire Arrow. Spawned as a child of a normal arrow.
 */

#include "z_arrow_fire.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"

#define FLAGS 0x02000010

#define THIS ((ArrowFire*)thisx)

void ArrowFire_Init(Actor* thisx, GlobalContext* globalCtx);
void ArrowFire_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ArrowFire_Update(Actor* thisx, GlobalContext* globalCtx);
void ArrowFire_Draw(Actor* thisx, GlobalContext* globalCtx);

void ArrowFire_Charge(ArrowFire* self, GlobalContext* globalCtx);
void ArrowFire_Fly(ArrowFire* self, GlobalContext* globalCtx);
void ArrowFire_Hit(ArrowFire* self, GlobalContext* globalCtx);

#include "z_arrow_fire_gfx.c"

const ActorInit Arrow_Fire_InitVars = {
    ACTOR_ARROW_FIRE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ArrowFire),
    (ActorFunc)ArrowFire_Init,
    (ActorFunc)ArrowFire_Destroy,
    (ActorFunc)ArrowFire_Update,
    (ActorFunc)ArrowFire_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_STOP),
};

void ArrowFire_SetupAction(ArrowFire* self, ArrowFireActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void ArrowFire_Init(Actor* thisx, GlobalContext* globalCtx) {
    ArrowFire* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->radius = 0;
    self->unk_158 = 1.0f;
    ArrowFire_SetupAction(self, ArrowFire_Charge);
    Actor_SetScale(&self->actor, 0.01f);
    self->alpha = 160;
    self->timer = 0;
    self->unk_15C = 0.0f;
}

void ArrowFire_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    func_800876C8(globalCtx);
    LOG_STRING("消滅", "../z_arrow_fire.c", 421); // "Disappearance"
}

void ArrowFire_Charge(ArrowFire* self, GlobalContext* globalCtx) {
    EnArrow* arrow;

    arrow = (EnArrow*)self->actor.parent;
    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->radius < 10) {
        self->radius += 1;
    }
    // copy position and rotation from arrow
    self->actor.world.pos = arrow->actor.world.pos;
    self->actor.shape.rot = arrow->actor.shape.rot;

    func_8002F974(&self->actor, NA_SE_PL_ARROW_CHARGE_FIRE - SFX_FLAG);

    // if arrow has no parent, player has fired the arrow
    if (arrow->actor.parent == NULL) {
        self->unkPos = self->actor.world.pos;
        self->radius = 10;
        ArrowFire_SetupAction(self, ArrowFire_Fly);
        self->alpha = 255;
    }
}

void func_80865ECC(Vec3f* unkPos, Vec3f* firePos, f32 scale) {
    unkPos->x += ((firePos->x - unkPos->x) * scale);
    unkPos->y += ((firePos->y - unkPos->y) * scale);
    unkPos->z += ((firePos->z - unkPos->z) * scale);
}

void ArrowFire_Hit(ArrowFire* self, GlobalContext* globalCtx) {
    f32 scale;
    f32 offset;
    u16 timer;

    if (self->actor.projectedW < 50.0f) {
        scale = 10.0f;
    } else {
        if (950.0f < self->actor.projectedW) {
            scale = 310.0f;
        } else {
            scale = self->actor.projectedW;
            scale = ((scale - 50.0f) * (1.0f / 3.0f)) + 10.0f;
        }
    }

    timer = self->timer;
    if (timer != 0) {
        self->timer -= 1;

        if (self->timer >= 8) {
            offset = ((self->timer - 8) * (1.0f / 24.0f));
            offset = SQ(offset);
            self->radius = (((1.0f - offset) * scale) + 10.0f);
            self->unk_158 += ((2.0f - self->unk_158) * 0.1f);
            if (self->timer < 16) {
                if (1) {}
                self->alpha = ((self->timer * 0x23) - 0x118);
            }
        }
    }

    if (self->timer >= 9) {
        if (self->unk_15C < 1.0f) {
            self->unk_15C += 0.25f;
        }
    } else {
        if (self->unk_15C > 0.0f) {
            self->unk_15C -= 0.125f;
        }
    }

    if (self->timer < 8) {
        self->alpha = 0;
    }

    if (self->timer == 0) {
        self->timer = 255;
        Actor_Kill(&self->actor);
    }
}

void ArrowFire_Fly(ArrowFire* self, GlobalContext* globalCtx) {
    EnArrow* arrow;
    f32 distanceScaled;
    s32 pad;

    arrow = (EnArrow*)self->actor.parent;
    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        Actor_Kill(&self->actor);
        return;
    }
    // copy position and rotation from arrow
    self->actor.world.pos = arrow->actor.world.pos;
    self->actor.shape.rot = arrow->actor.shape.rot;
    distanceScaled = Math_Vec3f_DistXYZ(&self->unkPos, &self->actor.world.pos) * (1.0f / 24.0f);
    self->unk_158 = distanceScaled;
    if (distanceScaled < 1.0f) {
        self->unk_158 = 1.0f;
    }
    func_80865ECC(&self->unkPos, &self->actor.world.pos, 0.05f);

    if (arrow->hitFlags & 1) {
        Audio_PlayActorSound2(&self->actor, NA_SE_IT_EXPLOSION_FRAME);
        ArrowFire_SetupAction(self, ArrowFire_Hit);
        self->timer = 32;
        self->alpha = 255;
    } else if (arrow->timer < 34) {
        if (self->alpha < 35) {
            Actor_Kill(&self->actor);
        } else {
            self->alpha -= 0x19;
        }
    }
}

void ArrowFire_Update(Actor* thisx, GlobalContext* globalCtx) {
    ArrowFire* self = THIS;

    if (globalCtx->msgCtx.msgMode == 0xD || globalCtx->msgCtx.msgMode == 0x11) {
        Actor_Kill(&self->actor);
    } else {
        self->actionFunc(self, globalCtx);
    }
}

void ArrowFire_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ArrowFire* self = THIS;
    s32 pad;
    u32 stateFrames;
    EnArrow* arrow;
    Actor* tranform;

    stateFrames = globalCtx->state.frames;
    arrow = (EnArrow*)self->actor.parent;
    if (1) {}

    if ((arrow != NULL) && (arrow->actor.update != NULL) && (self->timer < 255)) {
        if (1) {}
        tranform = (arrow->hitFlags & 2) ? &self->actor : &arrow->actor;

        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_arrow_fire.c", 618);

        Matrix_Translate(tranform->world.pos.x, tranform->world.pos.y, tranform->world.pos.z, MTXMODE_NEW);
        Matrix_RotateY(tranform->shape.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_RotateX(tranform->shape.rot.x * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_RotateZ(tranform->shape.rot.z * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

        // Draw red effect over the screen when arrow hits
        if (self->unk_15C > 0) {
            POLY_XLU_DISP = func_800937C0(POLY_XLU_DISP);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, (s32)(40.0f * self->unk_15C) & 0xFF, 0, 0,
                            (s32)(150.0f * self->unk_15C) & 0xFF);
            gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_DISABLE);
            gDPSetColorDither(POLY_XLU_DISP++, G_CD_DISABLE);
            gDPFillRectangle(POLY_XLU_DISP++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        }

        // Draw fire on the arrow
        func_80093D84(globalCtx->state.gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 200, 0, self->alpha);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 128);
        Matrix_RotateRPY(0x4000, 0x0, 0x0, MTXMODE_APPLY);
        if (self->timer != 0) {
            Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        } else {
            Matrix_Translate(0.0f, 1500.0f, 0.0f, MTXMODE_APPLY);
        }
        Matrix_Scale(self->radius * 0.2f, self->unk_158 * 4.0f, self->radius * 0.2f, MTXMODE_APPLY);
        Matrix_Translate(0.0f, -700.0f, 0.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_arrow_fire.c", 666),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, sTextureDL);
        gSPDisplayList(POLY_XLU_DISP++,
                       Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 255 - (stateFrames * 2) % 256, 0, 64, 32, 1,
                                        255 - stateFrames % 256, 511 - (stateFrames * 10) % 512, 64, 64));
        gSPDisplayList(POLY_XLU_DISP++, sVertexDL);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_arrow_fire.c", 682);
    }
}
