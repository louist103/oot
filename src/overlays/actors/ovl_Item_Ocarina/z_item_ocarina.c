/*
 * File: z_item_ocarina.c
 * Overlay: ovl_Item_Ocarina
 * Description: Ocarina of Time
 */

#include "z_item_ocarina.h"
#include "scenes/overworld/spot00/spot00_scene.h"

#define FLAGS 0x00000010

#define THIS ((ItemOcarina*)thisx)

void ItemOcarina_Init(Actor* thisx, GlobalContext* globalCtx);
void ItemOcarina_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ItemOcarina_Update(Actor* thisx, GlobalContext* globalCtx);
void ItemOcarina_Draw(Actor* thisx, GlobalContext* globalCtx);

void ItemOcarina_GetThrown(ItemOcarina* self, GlobalContext* globalCtx);
void ItemOcarina_Fly(ItemOcarina* self, GlobalContext* globalCtx);
void ItemOcarina_WaitInWater(ItemOcarina* self, GlobalContext* globalCtx);
void ItemOcarina_StartSoTCutscene(ItemOcarina* self, GlobalContext* globalCtx);
void func_80B864EC(ItemOcarina* self, GlobalContext* globalCtx);
void func_80B865E0(ItemOcarina* self, GlobalContext* globalCtx);
void ItemOcarina_DoNothing(ItemOcarina* self, GlobalContext* globalCtx);

const ActorInit Item_Ocarina_InitVars = {
    ACTOR_ITEM_OCARINA,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GI_OCARINA,
    sizeof(ItemOcarina),
    (ActorFunc)ItemOcarina_Init,
    (ActorFunc)ItemOcarina_Destroy,
    (ActorFunc)ItemOcarina_Update,
    (ActorFunc)ItemOcarina_Draw,
};

void ItemOcarina_SetupAction(ItemOcarina* self, ItemOcarinaActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void ItemOcarina_Init(Actor* thisx, GlobalContext* globalCtx) {
    ItemOcarina* self = THIS;
    s32 params = thisx->params;

    ActorShape_Init(&self->actor.shape, 0, 0, 0);
    Actor_SetScale(&self->actor, 0.1f);

    switch (params) {
        case 0:
            ItemOcarina_SetupAction(self, ItemOcarina_GetThrown);
            break;
        case 1:
            ItemOcarina_SetupAction(self, func_80B865E0);
            break;
        case 2:
            ItemOcarina_SetupAction(self, ItemOcarina_DoNothing);
            break;
        case 3:
            ItemOcarina_SetupAction(self, ItemOcarina_WaitInWater);
            if (!(gSaveContext.eventChkInf[8] & 1) || (gSaveContext.eventChkInf[4] & 8)) {
                Actor_Kill(thisx);
                return;
            }
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ELF_MSG2, 299.0f, -140.0f, 884.0f, 0, 4, 1, 0x3800);
            Actor_SetScale(thisx, 0.2f);
            break;
        default:
            Actor_Kill(thisx);
            return;
    }

    LOG_NUM("no", params, "../z_item_ocarina.c", 210);
    self->spinRotOffset = 0x400;
}

void ItemOcarina_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void ItemOcarina_Fly(ItemOcarina* self, GlobalContext* globalCtx) {
    Vec3f ripplePos;

    func_8002D7EC(&self->actor);
    self->actor.shape.rot.x += self->spinRotOffset * 2;
    self->actor.shape.rot.y += self->spinRotOffset * 3;

    if (self->actor.minVelocityY < self->actor.velocity.y) {
        self->actor.velocity.y += self->actor.gravity;
        if (self->actor.velocity.y < self->actor.minVelocityY) {
            self->actor.velocity.y = self->actor.minVelocityY;
        }
    }

    if (globalCtx->csCtx.frames == 881) {
        self->actor.world.pos.x = 250.0f;
        self->actor.world.pos.y = 60.0f;
        self->actor.world.pos.z = 1075.0f;
        self->actor.velocity.x = 1.0f;
        self->actor.velocity.y = -5.0f;
        self->actor.velocity.z = -7.0f;
    }

    if (globalCtx->csCtx.frames == 897) {
        EffectSsGRipple_Spawn(globalCtx, &self->actor.world.pos, 100, 500, 0);
        EffectSsGSplash_Spawn(globalCtx, &self->actor.world.pos, 0, 0, 1, 0);
        self->actor.velocity.x = 0.0f;
        self->actor.velocity.y = 0.0f;
        self->actor.velocity.z = 0.0f;
        self->actor.gravity = -0.1f;
        self->actor.minVelocityY = -0.5f;
        self->spinRotOffset = 0;
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_BOMB_DROP_WATER);
    }

    // landed in water
    if (globalCtx->csCtx.frames == 906) {
        ripplePos.x = 274.0f;
        ripplePos.y = -60.0f;
        ripplePos.z = 907.0f;
        EffectSsGRipple_Spawn(globalCtx, &ripplePos, 100, 500, 0);
    }
}

void ItemOcarina_GetThrown(ItemOcarina* self, GlobalContext* globalCtx) {
    self->actor.gravity = -0.3f;
    self->actor.minVelocityY = -5.0f;
    self->actor.velocity.x = 0.0f;
    self->actor.velocity.y = 6.0f;
    self->actor.velocity.z = 0.0f;
    ItemOcarina_SetupAction(self, ItemOcarina_Fly);
}

void func_80B864EC(ItemOcarina* self, GlobalContext* globalCtx) {
    func_8002D7EC(&self->actor);
    self->actor.shape.rot.x += self->spinRotOffset * 2;
    self->actor.shape.rot.y += self->spinRotOffset * 3;

    if (self->actor.minVelocityY < self->actor.velocity.y) {
        self->actor.velocity.y += self->actor.gravity;

        if (self->actor.velocity.y < self->actor.minVelocityY) {
            self->actor.velocity.y = self->actor.minVelocityY;
        }
    }

    if (globalCtx->csCtx.frames == 220) {
        self->actor.world.pos.x = 144.0f;
        self->actor.world.pos.y = 80.0f;
        self->actor.world.pos.z = 1686.0f;
        self->actor.velocity.x = 1.0f;
        self->actor.velocity.y = 2.0f;
        self->actor.velocity.z = -7.0f;
        self->actor.gravity = -0.15f;
        self->actor.minVelocityY = -5.0f;
    }
}

void func_80B865E0(ItemOcarina* self, GlobalContext* globalCtx) {
    self->actor.gravity = -0.3f;
    self->actor.minVelocityY = -5.0f;
    self->actor.velocity.x = 0.0f;
    self->actor.velocity.y = 4.0f;
    self->actor.velocity.z = 6.0f;
    ItemOcarina_SetupAction(self, func_80B864EC);
}

void ItemOcarina_DoNothing(ItemOcarina* self, GlobalContext* globalCtx) {
}

void ItemOcarina_StartSoTCutscene(ItemOcarina* self, GlobalContext* globalCtx) {
    if (func_8002F334(&self->actor, globalCtx)) {
        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gZeldaSongOfTimeCs);
        gSaveContext.cutsceneTrigger = 1;
    }
}

void ItemOcarina_WaitInWater(ItemOcarina* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        gSaveContext.eventChkInf[4] |= 8;
        Flags_SetSwitch(globalCtx, 3);
        self->actionFunc = ItemOcarina_StartSoTCutscene;
        self->actor.draw = NULL;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_OCARINA_OOT, 30.0f, 50.0f);

        if ((globalCtx->gameplayFrames & 13) == 0) {
            EffectSsBubble_Spawn(globalCtx, &self->actor.world.pos, 0.0f, 0.0f, 10.0f, 0.13f);
        }
    }
}

void ItemOcarina_Update(Actor* thisx, GlobalContext* globalCtx) {
    ItemOcarina* self = THIS;

    self->actionFunc(self, globalCtx);
}

void ItemOcarina_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ItemOcarina* self = THIS;

    func_8002EBCC(thisx, globalCtx, 0);
    func_8002ED80(thisx, globalCtx, 0);
    GetItem_Draw(globalCtx, GID_OCARINA_TIME);
}
