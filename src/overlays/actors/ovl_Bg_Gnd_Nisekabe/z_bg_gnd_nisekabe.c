/*
 * File: z_bg_gnd_nisekabe.c
 * Overlay: ovl_Bg_Gnd_Nisekabe
 * Description: Ganon's Castle Fake Wall
 */

#include "z_bg_gnd_nisekabe.h"
#include "objects/object_demo_kekkai/object_demo_kekkai.h"

#define FLAGS 0x00000010

#define THIS ((BgGndNisekabe*)thisx)

void BgGndNisekabe_Init(Actor* thisx, GlobalContext* globalCtx);
void BgGndNisekabe_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgGndNisekabe_Update(Actor* thisx, GlobalContext* globalCtx);
void BgGndNisekabe_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit Bg_Gnd_Nisekabe_InitVars = {
    ACTOR_BG_GND_NISEKABE,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DEMO_KEKKAI,
    sizeof(BgGndNisekabe),
    (ActorFunc)BgGndNisekabe_Init,
    (ActorFunc)BgGndNisekabe_Destroy,
    (ActorFunc)BgGndNisekabe_Update,
    (ActorFunc)BgGndNisekabe_Draw,
};

void BgGndNisekabe_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgGndNisekabe* self = THIS;

    Actor_SetScale(&self->actor, 0.1);
    self->actor.uncullZoneForward = 3000.0;
}

void BgGndNisekabe_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void BgGndNisekabe_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgGndNisekabe* self = THIS;

    if (globalCtx->actorCtx.unk_03 != 0) {
        self->actor.flags |= 0x80;
    } else {
        self->actor.flags &= ~0x80;
    }
}

void BgGndNisekabe_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static Gfx* dLists[] = {
        gLightTrialFakeWallDL,
        gGanonsCastleUnusedFakeWallDL,
        gGanonsCastleScrubsFakeWallDL,
    };
    BgGndNisekabe* self = THIS;
    u32 index = self->actor.params & 0xFF;

    if ((self->actor.flags & 0x80) == 0x80) {
        Gfx_DrawDListXlu(globalCtx, dLists[index]);
    } else {
        Gfx_DrawDListOpa(globalCtx, dLists[index]);
    }
}
