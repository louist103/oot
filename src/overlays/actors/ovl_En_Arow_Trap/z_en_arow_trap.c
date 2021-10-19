/*
 * File: z_en_arow_trap.c
 * Overlay: ovl_En_Arow_Trap
 * Description: An unused trap that reflects arrows.
 */

#include "z_en_arow_trap.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#define FLAGS 0x00000010

#define THIS ((EnArowTrap*)thisx)

void EnArowTrap_Init(Actor* thisx, GlobalContext* globalCtx);
void EnArowTrap_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnArowTrap_Update(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Arow_Trap_InitVars = {
    ACTOR_EN_AROW_TRAP,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnArowTrap),
    (ActorFunc)EnArowTrap_Init,
    (ActorFunc)EnArowTrap_Destroy,
    (ActorFunc)EnArowTrap_Update,
    NULL,
};

void EnArowTrap_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnArowTrap* self = THIS;

    Actor_SetScale(&self->actor, 0.01);
    self->unk_14C = 0;
    self->attackTimer = 80;
    self->actor.focus.pos = self->actor.world.pos;
}

void EnArowTrap_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnArowTrap_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnArowTrap* self = THIS;

    if (self->actor.xzDistToPlayer <= 400) {
        self->attackTimer--;

        if (self->attackTimer == 0) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ARROW, self->actor.world.pos.x,
                        self->actor.world.pos.y, self->actor.world.pos.z, self->actor.shape.rot.x,
                        self->actor.shape.rot.y, self->actor.shape.rot.z, ARROW_NORMAL_SILENT);
            self->attackTimer = 80;
        }
    }
}
