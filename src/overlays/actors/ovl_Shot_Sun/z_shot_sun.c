/*
 * File: z_shot_sun.c
 * Overlay: ovl_Shot_Sun
 * Description: Lake Hylia Sun hitbox and Song of Storms Fairy spawner
 */

#include "z_shot_sun.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "scenes/overworld/spot06/spot06_scene.h"
#include "vt.h"

#define FLAGS 0x00000009

#define THIS ((ShotSun*)thisx)

void ShotSun_Init(Actor* thisx, GlobalContext* globalCtx);
void ShotSun_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ShotSun_Update(Actor* thisx, GlobalContext* globalCtx);

void ShotSun_SpawnFairy(ShotSun* self, GlobalContext* globalCtx);
void ShotSun_TriggerFairy(ShotSun* self, GlobalContext* globalCtx);
void func_80BADF0C(ShotSun* self, GlobalContext* globalCtx);
void ShotSun_UpdateHyliaSun(ShotSun* self, GlobalContext* globalCtx);

const ActorInit Shot_Sun_InitVars = {
    ACTOR_SHOT_SUN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ShotSun),
    (ActorFunc)ShotSun_Init,
    (ActorFunc)ShotSun_Destroy,
    (ActorFunc)ShotSun_Update,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000020, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 60, 0, { 0, 0, 0 } },
};

void ShotSun_Init(Actor* thisx, GlobalContext* globalCtx) {
    ShotSun* self = THIS;
    s32 params;

    // "Ocarina secret occurrence"
    osSyncPrintf("%d ---- オカリナの秘密発生!!!!!!!!!!!!!\n", self->actor.params);
    params = self->actor.params & 0xFF;
    if (params == 0x40 || params == 0x41) {
        self->unk_1A4 = 0;
        self->actor.flags |= 0x10;
        self->actor.flags |= 0x2000000;
        self->actionFunc = func_80BADF0C;
        self->actor.flags |= 0x8000000;
    } else {
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        self->actionFunc = ShotSun_UpdateHyliaSun;
        self->actor.flags &= ~1;
    }
}

void ShotSun_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ShotSun* self = THIS;
    s32 params = self->actor.params & 0xFF;

    if (params != 0x40 && params != 0x41) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void ShotSun_SpawnFairy(ShotSun* self, GlobalContext* globalCtx) {
    s32 params = self->actor.params & 0xFF;
    s32 fairyType;

    if (self->timer > 0) {
        self->timer--;
    } else {
        switch (params) {
            case 0x40:
                fairyType = FAIRY_HEAL_BIG;
                break;
            case 0x41:
                fairyType = FAIRY_HEAL_BIG;
                break;
        }

        //! @bug fairyType may be uninitialized
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ELF, self->actor.home.pos.x, self->actor.home.pos.y,
                    self->actor.home.pos.z, 0, 0, 0, fairyType);

        Actor_Kill(&self->actor);
    }
}

void ShotSun_TriggerFairy(ShotSun* self, GlobalContext* globalCtx) {
    if ((func_8005B198() == self->actor.category) || (self->timer != 0)) {
        self->actionFunc = ShotSun_SpawnFairy;
        self->timer = 50;

        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_KANKYO, self->actor.home.pos.x, self->actor.home.pos.y,
                    self->actor.home.pos.z, 0, 0, 0, 0x11);

        func_80078914(&self->actor.projectedPos, NA_SE_EV_TRE_BOX_APPEAR);
    }
}

void func_80BADF0C(ShotSun* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 pad;
    s32 params = self->actor.params & 0xFF;

    if (Math3D_Vec3fDistSq(&self->actor.world.pos, &player->actor.world.pos) > 22500.0f) {
        self->unk_1A4 = 0;
    } else {
        if (self->unk_1A4 == 0) {
            if (!(player->stateFlags2 & 0x1000000)) {
                player->stateFlags2 |= 0x800000;
                return;
            } else {
                self->unk_1A4 = 1;
            }
        }
        if (self->unk_1A4 == 1) {
            func_8010BD58(globalCtx, 1);
            self->unk_1A4 = 2;
        } else if (self->unk_1A4 == 2 && globalCtx->msgCtx.unk_E3EE == 4) {
            if ((params == 0x40 && globalCtx->msgCtx.unk_E3EC == 9) ||
                (params == 0x41 && globalCtx->msgCtx.unk_E3EC == 0xB)) {
                self->actionFunc = ShotSun_TriggerFairy;
                OnePointCutscene_Attention(globalCtx, &self->actor);
                self->timer = 0;
            } else {
                self->unk_1A4 = 0;
            }
            self->unk_1A4 = 0;
        }
    }
}

void ShotSun_UpdateHyliaSun(ShotSun* self, GlobalContext* globalCtx) {
    Vec3s cylinderPos;
    Player* player = GET_PLAYER(globalCtx);
    EnItem00* collectible;
    s32 pad;
    Vec3f spawnPos;

    if (self->collider.base.acFlags & AC_HIT) {
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        osSyncPrintf(VT_FGCOL(CYAN) "SHOT_SUN HIT!!!!!!!\n" VT_RST);
        if (INV_CONTENT(ITEM_ARROW_FIRE) == ITEM_NONE) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_ETCETERA, 700.0f, -800.0f, 7261.0f, 0, 0, 0, 7);
            globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gFireArrowsCS);
            if (1) {}
            gSaveContext.cutsceneTrigger = 1;
        } else {
            spawnPos.x = 700.0f;
            spawnPos.y = -800.0f;
            spawnPos.z = 7261.0f;

            collectible = Item_DropCollectible(globalCtx, &spawnPos, ITEM00_MAGIC_LARGE);
            if (collectible != NULL) {
                collectible->unk_15A = 6000;
                collectible->actor.speedXZ = 0.0f;
            }
        }
        Actor_Kill(&self->actor);
    } else {
        if (!(self->actor.xzDistToPlayer > 120.0f) && gSaveContext.dayTime >= 0x4555 && gSaveContext.dayTime < 0x5000) {
            cylinderPos.x = player->bodyPartsPos[7].x + globalCtx->envCtx.sunPos.x * (1.0f / 6.0f);
            cylinderPos.y = player->bodyPartsPos[7].y - 30.0f + globalCtx->envCtx.sunPos.y * (1.0f / 6.0f);
            cylinderPos.z = player->bodyPartsPos[7].z + globalCtx->envCtx.sunPos.z * (1.0f / 6.0f);

            self->hitboxPos = cylinderPos;

            Collider_SetCylinderPosition(&self->collider, &cylinderPos);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

void ShotSun_Update(Actor* thisx, GlobalContext* globalCtx) {
    ShotSun* self = THIS;

    self->actionFunc(self, globalCtx);
}
