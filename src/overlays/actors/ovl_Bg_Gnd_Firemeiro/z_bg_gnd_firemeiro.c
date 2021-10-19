/*
 * File: z_bg_gnd_firemeiro.c
 * Overlay: ovl_Bg_Gnd_Firemeiro
 * Description: Sinking lava platform (Ganon's Castle)
 */

#include "z_bg_gnd_firemeiro.h"
#include "objects/object_demo_kekkai/object_demo_kekkai.h"

#define FLAGS 0x00000030

#define THIS ((BgGndFiremeiro*)thisx)

void BgGndFiremeiro_Init(Actor* thisx, GlobalContext* globalCtx);
void BgGndFiremeiro_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgGndFiremeiro_Update(Actor* thisx, GlobalContext* globalCtx);
void BgGndFiremeiro_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgGndFiremeiro_Sink(BgGndFiremeiro* self, GlobalContext* globalCtx);
void BgGndFiremeiro_Shake(BgGndFiremeiro* self, GlobalContext* globalCtx);
void BgGndFiremeiro_Rise(BgGndFiremeiro* self, GlobalContext* globalCtx);

const ActorInit Bg_Gnd_Firemeiro_InitVars = {
    ACTOR_BG_GND_FIREMEIRO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DEMO_KEKKAI,
    sizeof(BgGndFiremeiro),
    (ActorFunc)BgGndFiremeiro_Init,
    (ActorFunc)BgGndFiremeiro_Destroy,
    (ActorFunc)BgGndFiremeiro_Update,
    (ActorFunc)BgGndFiremeiro_Draw,
};

void BgGndFiremeiro_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgGndFiremeiro* self = THIS;
    CollisionHeader* colHeader = NULL;

    ActorShape_Init(&self->dyna.actor.shape, 0.0f, NULL, 0.0f);
    Actor_SetScale(&self->dyna.actor, 0.1f);
    self->initPos = self->dyna.actor.world.pos;

    if (self->dyna.actor.params == 0) {
        DynaPolyActor_Init(&self->dyna, DPM_UNK);
        CollisionHeader_GetVirtual(&gFireTrialPlatformCol, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);
        self->actionFunc = BgGndFiremeiro_Rise;
    }
}

void BgGndFiremeiro_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgGndFiremeiro* self = THIS;

    if (self->dyna.actor.params == 0) {
        if (1) {}
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
}

void BgGndFiremeiro_Sink(BgGndFiremeiro* self, GlobalContext* globalCtx) {
    f32 sunkHeight = self->initPos.y - 150.0f;

    if (func_8004356C(&self->dyna)) {
        self->timer = 10;
    }

    if (sunkHeight < self->dyna.actor.world.pos.y) {
        self->dyna.actor.world.pos.y -= 0.5f;

        if (self->dyna.actor.world.pos.y < sunkHeight) {
            self->dyna.actor.world.pos.y = sunkHeight;
        }

        func_8002F948(&self->dyna.actor, NA_SE_EV_ROLL_STAND_2 - SFX_FLAG);
    }

    if (self->timer > 0) {
        self->timer--;
    } else {
        self->actionFunc = BgGndFiremeiro_Rise;
    }
}

void BgGndFiremeiro_Shake(BgGndFiremeiro* self, GlobalContext* globalCtx) {
    s32 pad;
    f32 randSign;

    if (func_8004356C(&self->dyna)) { // Player standing on it
        if (self->timer > 0) {
            self->timer--;

            randSign = ((self->timer & 1) ? 2.0f : -2.0f);

            self->dyna.actor.world.pos = self->initPos;
            self->dyna.actor.world.pos.x += randSign * Math_SinS(self->timer * 0x2FFF);
            self->dyna.actor.world.pos.z += randSign * Math_CosS(self->timer * 0x2FFF);
            self->dyna.actor.world.pos.y += Math_CosS(self->timer * 0x7FFF);

            if (!(self->timer % 4)) {
                Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_BLOCK_SHAKE);
            }
        } else {
            self->timer = 10;
            self->dyna.actor.world.pos = self->initPos;
            self->actionFunc = BgGndFiremeiro_Sink;
        }
    } else {
        self->dyna.actor.world.pos = self->initPos;
        self->actionFunc = BgGndFiremeiro_Rise;
    }
}

void BgGndFiremeiro_Rise(BgGndFiremeiro* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* thisx = &self->dyna.actor;

    if ((player->currentBoots != PLAYER_BOOTS_HOVER) && func_8004356C(&self->dyna)) { // Player standing on it
        if (thisx->world.pos.y < self->initPos.y) {
            self->actionFunc = BgGndFiremeiro_Sink;
            self->timer = 20;
        } else {
            self->actionFunc = BgGndFiremeiro_Shake;
            self->timer = 20;
        }
    } else {
        if (thisx->world.pos.y < self->initPos.y) {
            thisx->world.pos.y += 2.0f;
            if (self->initPos.y < thisx->world.pos.y) {
                thisx->world.pos.y = self->initPos.y;
            }
        }
    }
}

void BgGndFiremeiro_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgGndFiremeiro* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgGndFiremeiro_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_gnd_firemeiro.c", 280);
    func_800943C8(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_gnd_firemeiro.c", 282),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gFireTrialPlatformDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_gnd_firemeiro.c", 285);
}
