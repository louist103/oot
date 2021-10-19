/*
 * File: z_oceff_storm.c
 * Overlay: ovl_Oceff_Storm
 * Description: Song of Storms Effect
 */

#include "z_oceff_storm.h"

#define FLAGS 0x02000030

#define THIS ((OceffStorm*)thisx)

void OceffStorm_Init(Actor* thisx, GlobalContext* globalCtx);
void OceffStorm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void OceffStorm_Update(Actor* thisx, GlobalContext* globalCtx);
void OceffStorm_Draw(Actor* thisx, GlobalContext* globalCtx);

void OceffStorm_Draw2(Actor* thisx, GlobalContext* globalCtx);

void OceffStorm_DefaultAction(OceffStorm* self, GlobalContext* globalCtx);
void OceffStorm_UnkAction(OceffStorm* self, GlobalContext* globalCtx);

const ActorInit Oceff_Storm_InitVars = {
    ACTOR_OCEFF_STORM,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(OceffStorm),
    (ActorFunc)OceffStorm_Init,
    (ActorFunc)OceffStorm_Destroy,
    (ActorFunc)OceffStorm_Update,
    (ActorFunc)OceffStorm_Draw,
};

#include "z_oceff_storm_gfx.c"

void OceffStorm_SetupAction(OceffStorm* self, OceffStormActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void OceffStorm_Init(Actor* thisx, GlobalContext* globalCtx) {
    OceffStorm* self = THIS;
    OceffStorm_SetupAction(self, OceffStorm_DefaultAction);
    self->posYOffAdd = 0;
    self->counter = 0;
    self->primColorAlpha = 0;
    self->vtxAlpha = 0;
    self->actor.scale.y = 0.0f;
    self->actor.scale.z = 0.0f;
    self->actor.scale.x = 0.0f;
    self->posYOff = self->posYOffAdd;

    if (self->actor.params == 1) {
        OceffStorm_SetupAction(self, OceffStorm_UnkAction);
        self->actor.draw = OceffStorm_Draw2;
    } else {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_OKARINA_EFFECT, self->actor.world.pos.x,
                    self->actor.world.pos.y - 30.0f, self->actor.world.pos.z, 0, 0, 0, 1);
    }
}

void OceffStorm_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    OceffStorm* self = THIS;
    Player* player = GET_PLAYER(globalCtx);

    func_800876C8(globalCtx);
    if (gSaveContext.nayrusLoveTimer != 0) {
        player->stateFlags3 |= 0x40;
    }
}

void OceffStorm_DefaultAction(OceffStorm* self, GlobalContext* globalCtx) {
    if (self->counter < 20) {
        self->primColorAlpha = (s8)(self->counter * 5.0f);
    } else if (self->counter > 80) {
        self->primColorAlpha = (s8)((100 - self->counter) * 5.0f);
    } else {
        self->primColorAlpha = 100;
    }

    if (self->counter < 10 || self->counter >= 90) {
        self->vtxAlpha = 0;
    } else {
        if (self->counter <= 65) {
            if (self->vtxAlpha <= 200) {
                self->vtxAlpha += 10;
            }
            self->actor.scale.x = self->actor.scale.z = 0.4f;
            self->actor.scale.y = 0.3f;
        } else if (self->counter > 65) {
            self->vtxAlpha = (90 - self->counter) * 10;
        } else {
            self->vtxAlpha = 255;
            self->actor.scale.x = self->actor.scale.z = 0.4f;
        }
    }

    if (self->counter > 60) {
        self->actor.world.pos.y += self->posYOff * 0.01f;
        self->posYOff += self->posYOffAdd;
        self->posYOffAdd += 10;
    }

    if (self->counter < 100) {
        self->counter++;
    } else {
        Actor_Kill(&self->actor);
    }
}

void OceffStorm_UnkAction(OceffStorm* self, GlobalContext* globalCtx) {
    if (self->primColorAlpha < 100) {
        self->primColorAlpha += 5;
    }
    //! @bug Actor_Kill is never called so the actor will stay alive forever
}

void OceffStorm_Update(Actor* thisx, GlobalContext* globalCtx) {
    OceffStorm* self = THIS;
    Player* player = GET_PLAYER(globalCtx);

    self->actor.world.pos = player->actor.world.pos;
    self->actor.shape.rot.y = Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx));
    self->actionFunc(self, globalCtx);
}

void OceffStorm_Draw2(Actor* thisx, GlobalContext* globalCtx) {
    u32 scroll = globalCtx->state.frames & 0xFFF;
    OceffStorm* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_oceff_storm.c", 449);

    gDPPipeSync(POLY_XLU_DISP++);

    if (1) {}

    POLY_XLU_DISP = func_80093F34(POLY_XLU_DISP);
    gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_NOISE);
    gDPSetColorDither(POLY_XLU_DISP++, G_CD_NOISE);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 200, 200, 150, self->primColorAlpha);
    gSPDisplayList(POLY_XLU_DISP++, sTextureDL);
    gSPDisplayList(POLY_XLU_DISP++, Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, scroll * 8, scroll * 4, 64, 64, 1,
                                                     scroll * 4, scroll * 4, 64, 64));
    gSPTextureRectangle(POLY_XLU_DISP++, 0, 0, (SCREEN_WIDTH << 2), (SCREEN_HEIGHT << 2), G_TX_RENDERTILE, 0, 0, 140,
                        (1 << 15) | (31 << 10) | 884);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_oceff_storm.c", 477);
}

void OceffStorm_Draw(Actor* thisx, GlobalContext* globalCtx) {
    u32 scroll = globalCtx->state.frames & 0xFFF;
    OceffStorm* self = THIS;
    Vtx* vtxPtr = sCylinderVtx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_oceff_storm.c", 486);

    func_80093D84(globalCtx->state.gfxCtx);

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 200, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 150, 150, 0, 128);
    gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_NOISE);
    gDPSetColorDither(POLY_XLU_DISP++, G_CD_NOISE);

    vtxPtr[0].v.cn[3] = vtxPtr[6].v.cn[3] = vtxPtr[16].v.cn[3] = vtxPtr[25].v.cn[3] = self->vtxAlpha >> 1;
    vtxPtr[10].v.cn[3] = vtxPtr[22].v.cn[3] = self->vtxAlpha;

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_oceff_storm.c", 498),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_XLU_DISP++, sCylinderTexDL);
    gSPDisplayList(POLY_XLU_DISP++, Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, scroll * 4, (0 - scroll) * 8, 32, 32,
                                                     1, scroll * 8, (0 - scroll) * 12, 32, 32));
    gSPDisplayList(POLY_XLU_DISP++, sCylinderDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_oceff_storm.c", 512);

    OceffStorm_Draw2(&self->actor, globalCtx);
}
