/*
 * File: z_en_anubice_tag.c
 * Overlay: ovl_En_Anubice_Tag
 * Description: Spawns and manages the Anubis enemy
 */

#include "z_en_anubice_tag.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((EnAnubiceTag*)thisx)

void EnAnubiceTag_Init(Actor* thisx, GlobalContext* globalCtx);
void EnAnubiceTag_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnAnubiceTag_Update(Actor* thisx, GlobalContext* globalCtx);
void EnAnubiceTag_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnAnubiceTag_SpawnAnubis(EnAnubiceTag* self, GlobalContext* globalCtx);
void EnAnubiceTag_ManageAnubis(EnAnubiceTag* self, GlobalContext* globalCtx);

const ActorInit En_Anubice_Tag_InitVars = {
    ACTOR_EN_ANUBICE_TAG,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnAnubiceTag),
    (ActorFunc)EnAnubiceTag_Init,
    (ActorFunc)EnAnubiceTag_Destroy,
    (ActorFunc)EnAnubiceTag_Update,
    (ActorFunc)EnAnubiceTag_Draw,
};

void EnAnubiceTag_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubiceTag* self = THIS;

    osSyncPrintf("\n\n");
    // "Anubis control tag generated"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ アヌビス制御タグ発生 ☆☆☆☆☆ %d\n" VT_RST, self->actor.params);

    if (self->actor.params < -1) {
        self->actor.params = 0;
    }
    if (self->actor.params != 0) {
        self->triggerRange = self->actor.params * 40.0f;
    }
    self->actionFunc = EnAnubiceTag_SpawnAnubis;
}

void EnAnubiceTag_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnAnubiceTag_SpawnAnubis(EnAnubiceTag* self, GlobalContext* globalCtx) {
    self->anubis = (EnAnubice*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_ANUBICE,
                                                  self->actor.world.pos.x, self->actor.world.pos.y,
                                                  self->actor.world.pos.z, 0, self->actor.yawTowardsPlayer, 0, 0);

    if (self->anubis != NULL) {
        self->actionFunc = EnAnubiceTag_ManageAnubis;
    }
}

void EnAnubiceTag_ManageAnubis(EnAnubiceTag* self, GlobalContext* globalCtx) {
    EnAnubice* anubis;
    Vec3f offset;

    if (self->anubis != NULL) {
        anubis = self->anubis;
        if (anubis->actor.update == NULL) {
            return;
        }
    } else {
        Actor_Kill(&self->actor);
        return;
    }

    if (anubis->deathTimer != 0) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->actor.xzDistToPlayer < (200.0f + self->triggerRange)) {
        if (!anubis->isLinkOutOfRange) {
            if (!anubis->isKnockedback) {
                anubis->isMirroringLink = true;
                offset.x = -Math_SinS(self->actor.yawTowardsPlayer) * self->actor.xzDistToPlayer;
                offset.z = -Math_CosS(self->actor.yawTowardsPlayer) * self->actor.xzDistToPlayer;
                Math_ApproachF(&anubis->actor.world.pos.x, (self->actor.world.pos.x + offset.x), 0.3f, 10.0f);
                Math_ApproachF(&anubis->actor.world.pos.z, (self->actor.world.pos.z + offset.z), 0.3f, 10.0f);
                return;
            }
        }
    } else {
        if (anubis->isMirroringLink) {
            anubis->isLinkOutOfRange = true;
        }
    }
}

void EnAnubiceTag_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubiceTag* self = THIS;

    self->actionFunc(self, globalCtx);
}

void EnAnubiceTag_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnAnubiceTag* self = THIS;

    if (BREG(0) != 0) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, 0xFF, 0, 0, 0xFF, 4, globalCtx->state.gfxCtx);
    }
}
