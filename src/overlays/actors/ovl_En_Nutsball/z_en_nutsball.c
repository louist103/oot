/*
 * File: z_en_nutsball.c
 * Overlay: ovl_En_Nutsball
 * Description: Projectile fired by deku scrubs
 */

#include "z_en_nutsball.h"
#include "objects/object_dns/object_dns.h"
#include "objects/object_hintnuts/object_hintnuts.h"
#include "objects/object_dnk/object_dnk.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"

#define FLAGS 0x00000010

#define THIS ((EnNutsball*)thisx)

void EnNutsball_Init(Actor* thisx, GlobalContext* globalCtx);
void EnNutsball_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnNutsball_Update(Actor* thisx, GlobalContext* globalCtx);
void EnNutsball_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80ABBB34(EnNutsball* self, GlobalContext* globalCtx);
void func_80ABBBA8(EnNutsball* self, GlobalContext* globalCtx);

const ActorInit En_Nutsball_InitVars = {
    ACTOR_EN_NUTSBALL,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnNutsball),
    (ActorFunc)EnNutsball_Init,
    (ActorFunc)EnNutsball_Destroy,
    (ActorFunc)EnNutsball_Update,
    (ActorFunc)NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_WOOD,
        BUMP_ON,
        OCELEM_ON,
    },
    { 13, 13, 0, { 0 } },
};

static s16 sObjectIDs[] = { OBJECT_DEKUNUTS, OBJECT_HINTNUTS, OBJECT_SHOPNUTS, OBJECT_DNS, OBJECT_DNK };

static Gfx* sDLists[] = { 0x06002028, gHintNutsNutDL, 0x06004008, gDntJijiNutDL, gDntStageNutDL };

void EnNutsball_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnNutsball* self = THIS;
    s32 pad;

    ActorShape_Init(&self->actor.shape, 400.0f, ActorShadow_DrawCircle, 13.0f);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->objBankIndex = Object_GetIndex(&globalCtx->objectCtx, sObjectIDs[self->actor.params]);

    if (self->objBankIndex < 0) {
        Actor_Kill(&self->actor);
    } else {
        self->actionFunc = func_80ABBB34;
    }
}

void EnNutsball_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnNutsball* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80ABBB34(EnNutsball* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->objBankIndex)) {
        self->actor.objBankIndex = self->objBankIndex;
        self->actor.draw = EnNutsball_Draw;
        self->actor.shape.rot.y = 0;
        self->timer = 30;
        self->actionFunc = func_80ABBBA8;
        self->actor.speedXZ = 10.0f;
    }
}

void func_80ABBBA8(EnNutsball* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3s sp4C;
    Vec3f sp40;

    self->timer--;

    if (self->timer == 0) {
        self->actor.gravity = -1.0f;
    }

    self->actor.home.rot.z += 0x2AA8;

    if ((self->actor.bgCheckFlags & 8) || (self->actor.bgCheckFlags & 1) || (self->collider.base.atFlags & AT_HIT) ||
        (self->collider.base.acFlags & AC_HIT) || (self->collider.base.ocFlags1 & OC1_HIT)) {
        // Checking if the player is using a shield that reflects projectiles
        // And if so, reflects the projectile on impact
        if ((player->currentShield == PLAYER_SHIELD_DEKU) ||
            ((player->currentShield == PLAYER_SHIELD_HYLIAN) && LINK_IS_ADULT)) {
            if ((self->collider.base.atFlags & AT_HIT) && (self->collider.base.atFlags & AT_TYPE_ENEMY) &&
                (self->collider.base.atFlags & AT_BOUNCED)) {
                self->collider.base.atFlags &= ~AT_TYPE_ENEMY & ~AT_BOUNCED & ~AT_HIT;
                self->collider.base.atFlags |= AT_TYPE_PLAYER;

                self->collider.info.toucher.dmgFlags = 2;
                Matrix_MtxFToYXZRotS(&player->shieldMf, &sp4C, 0);
                self->actor.world.rot.y = sp4C.y + 0x8000;
                self->timer = 30;
                return;
            }
        }

        sp40.x = self->actor.world.pos.x;
        sp40.y = self->actor.world.pos.y + 4;
        sp40.z = self->actor.world.pos.z;

        EffectSsHahen_SpawnBurst(globalCtx, &sp40, 6.0f, 0, 7, 3, 15, HAHEN_OBJECT_DEFAULT, 10, NULL);
        Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, NA_SE_EN_OCTAROCK_ROCK);
        Actor_Kill(&self->actor);
    } else {
        if (self->timer == -300) {
            Actor_Kill(&self->actor);
        }
    }
}

void EnNutsball_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnNutsball* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;

    if (!(player->stateFlags1 & 0x300000C0) || (self->actionFunc == func_80ABBB34)) {
        self->actionFunc(self, globalCtx);

        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10, sCylinderInit.dim.radius, sCylinderInit.dim.height, 5);
        Collider_UpdateCylinder(&self->actor, &self->collider);

        self->actor.flags |= 0x1000000;

        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void EnNutsball_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_nutsball.c", 327);

    func_80093D18(globalCtx->state.gfxCtx);
    Matrix_Mult(&globalCtx->mf_11DA0, MTXMODE_APPLY);
    Matrix_RotateZ(thisx->home.rot.z * 9.58738e-05f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_nutsball.c", 333),
              G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(POLY_OPA_DISP++, sDLists[thisx->params]);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_nutsball.c", 337);
}
