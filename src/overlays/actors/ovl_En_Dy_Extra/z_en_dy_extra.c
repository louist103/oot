/*
 * File: z_en_dy_extra.c
 * Overlay: ovl_En_Dy_Extra
 * Description: Spiral Beams (Great Fairy Fountains)
 */

#include "z_en_dy_extra.h"
#include "objects/object_dy_obj/object_dy_obj.h"
#include "vt.h"

#define FLAGS 0x00000030

#define THIS ((EnDyExtra*)thisx)

void EnDyExtra_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDyExtra_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDyExtra_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDyExtra_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDyExtra_WaitForTrigger(EnDyExtra* self, GlobalContext* globalCtx);
void EnDyExtra_FallAndKill(EnDyExtra* self, GlobalContext* globalCtx);

const ActorInit En_Dy_Extra_InitVars = {
    ACTOR_EN_DY_EXTRA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DY_OBJ,
    sizeof(EnDyExtra),
    (ActorFunc)EnDyExtra_Init,
    (ActorFunc)EnDyExtra_Destroy,
    (ActorFunc)EnDyExtra_Update,
    (ActorFunc)EnDyExtra_Draw,
};

void EnDyExtra_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnDyExtra_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDyExtra* self = THIS;

    osSyncPrintf("\n\n");
    // "Big fairy effect"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 大妖精効果 ☆☆☆☆☆ %d\n" VT_RST, self->actor.params);
    self->type = self->actor.params;
    self->scale.x = 0.025f;
    self->scale.y = 0.039f;
    self->scale.z = 0.025f;
    self->unk_168 = self->actor.world.pos;
    self->actor.gravity = -0.2f;
    self->unk_158 = 1.0f;
    self->timer = 60;
    self->actionFunc = EnDyExtra_WaitForTrigger;
}

void EnDyExtra_WaitForTrigger(EnDyExtra* self, GlobalContext* globalCtx) {
    Math_ApproachF(&self->actor.gravity, 0.0f, 0.1f, 0.005f);
    if (self->actor.world.pos.y < -55.0f) {
        self->actor.velocity.y = 0.0f;
    }
    if (self->timer == 0 && self->trigger != 0) {
        self->timer = 200;
        self->actionFunc = EnDyExtra_FallAndKill;
    }
}

void EnDyExtra_FallAndKill(EnDyExtra* self, GlobalContext* globalCtx) {
    Math_ApproachF(&self->actor.gravity, 0.0f, 0.1f, 0.005f);
    if (self->timer == 0 || self->unk_158 < 0.02f) {
        Actor_Kill(&self->actor);
        return;
    }
    Math_ApproachZeroF(&self->unk_158, 0.03f, 0.05f);
    if (self->actor.world.pos.y < -55.0f) {
        self->actor.velocity.y = 0.0f;
    }
}

void EnDyExtra_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDyExtra* self = THIS;

    if (self->timer != 0) {
        self->timer--;
    }
    self->actor.scale.x = self->scale.x;
    self->actor.scale.y = self->scale.y;
    self->actor.scale.z = self->scale.z;
    Audio_PlayActorSound2(&self->actor, NA_SE_PL_SPIRAL_HEAL_BEAM - SFX_FLAG);
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
}

void EnDyExtra_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Color_RGBA8 primColors[] = { { 255, 255, 170, 255 }, { 255, 255, 170, 255 } };
    static Color_RGBA8 envColors[] = { { 255, 100, 255, 255 }, { 100, 255, 255, 255 } };
    static u8 D_809FFC50[] = { 0x02, 0x01, 0x01, 0x02, 0x00, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02,
                               0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x01, 0x02 };
    EnDyExtra* self = THIS;
    s32 pad;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    Vtx* vertices = SEGMENTED_TO_VIRTUAL(gGreatFairySpiralBeamVtx);
    s32 i;
    u8 unk[3];

    unk[0] = 0.0f;
    unk[1] = (s8)(self->unk_158 * 240.0f);
    unk[2] = (s8)(self->unk_158 * 255.0f);

    for (i = 0; i < 27; i++) {
        if (D_809FFC50[i]) {
            vertices[i].v.cn[3] = unk[D_809FFC50[i]];
        }
    }

    OPEN_DISPS(gfxCtx, "../z_en_dy_extra.c", 294);

    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, globalCtx->state.frames * 2, 0, 0x20, 0x40, 1,
                                globalCtx->state.frames, globalCtx->state.frames * -8, 0x10, 0x10));
    gDPPipeSync(POLY_XLU_DISP++);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dy_extra.c", 307),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, primColors[self->type].r, primColors[self->type].g,
                    primColors[self->type].b, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, envColors[self->type].r, envColors[self->type].g, envColors[self->type].b, 128);
    gSPDisplayList(POLY_XLU_DISP++, gGreatFairySpiralBeamDL);

    CLOSE_DISPS(gfxCtx, "../z_en_dy_extra.c", 325);
}
