/*
 * File: z_en_siofuki.c
 * Overlay: ovl_En_Siofuki
 * Description: Water Spout
 */

#include "z_en_siofuki.h"
#include "objects/object_siofuki/object_siofuki.h"

#define FLAGS 0x00000030

#define THIS ((EnSiofuki*)thisx)

void EnSiofuki_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSiofuki_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSiofuki_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSiofuki_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AFC34C(EnSiofuki* self, GlobalContext* globalCtx);
void func_80AFC544(EnSiofuki* self, GlobalContext* globalCtx);
void func_80AFC478(EnSiofuki* self, GlobalContext* globalCtx);

const ActorInit En_Siofuki_InitVars = {
    ACTOR_EN_SIOFUKI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SIOFUKI,
    sizeof(EnSiofuki),
    (ActorFunc)EnSiofuki_Init,
    (ActorFunc)EnSiofuki_Destroy,
    (ActorFunc)EnSiofuki_Update,
    (ActorFunc)EnSiofuki_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void EnSiofuki_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSiofuki* self = THIS;
    s32 type;
    CollisionHeader* colHeader = NULL;
    s32 pad;

    if ((thisx->room == 10) && Flags_GetSwitch(globalCtx, 0x1E)) {
        Actor_Kill(thisx);
        return;
    }

    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&object_siofuki_Col_000D78, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    self->sfxFlags |= 1;

    type = ((u16)thisx->params >> 0xC) & 0xF;
    if (!((type == 0) || (type == 1))) {
        Actor_Kill(thisx);
        return;
    }

    self->initPosY = thisx->world.pos.y;
    self->unk_174 = 35.0f;
    self->unk_170 = -6058.0f + self->unk_174;

    if (thisx->shape.rot.x != 0) {
        self->maxHeight = thisx->shape.rot.x * 40.0f;
        self->currentHeight = self->maxHeight;
    }
    self->activeTime = 0;
    if (thisx->shape.rot.y != 0) {
        self->activeTime = thisx->shape.rot.y;
    }
    if (thisx->shape.rot.z != 0) {
        thisx->scale.x = thisx->shape.rot.z * (1.0f / 1.73f) * 0.1f;
        thisx->scale.z = thisx->shape.rot.z * 0.5f * 0.1f;
    }

    thisx->world.rot.x = 0;
    thisx->world.rot.y = 0;
    thisx->world.rot.z = 0;
    thisx->shape.rot.x = 0;
    thisx->shape.rot.y = 0;
    thisx->shape.rot.z = 0;

    type = ((u16)thisx->params >> 0xC) & 0xF;
    if (type == EN_SIOFUKI_RAISING) {
        self->currentHeight = 10.0f;
        self->targetHeight = 10.0f;
        self->actionFunc = func_80AFC34C;
    } else if (type == EN_SIOFUKI_LOWERING) {
        if (Flags_GetTreasure(globalCtx, (u16)thisx->params & 0x3F)) {
            self->currentHeight = -45.0f;
            self->targetHeight = -45.0f;
            self->actionFunc = func_80AFC544;
        } else {
            self->targetHeight = self->currentHeight;
            self->actionFunc = func_80AFC478;
        }
    }
}

void EnSiofuki_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnSiofuki* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
}

void func_80AFBDC8(EnSiofuki* self, GlobalContext* globalCtx) {
    self->oscillation = sinf((globalCtx->gameplayFrames & 0x1F) / 32.0f * M_PI * 2.0f) * 4.0f;
    self->unk_170 = self->unk_174 * 10.0f + -6058.0f - self->oscillation * 10.0f;
    self->unk_174 = 35.0f;
    self->dyna.actor.world.pos.y = self->initPosY + self->currentHeight + self->oscillation;
}

void func_80AFBE8C(EnSiofuki* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 dX;
    f32 dY;
    f32 dZ;
    s16 angle;
    s16 dAngle;
    f32 dist2d;
    f32 speedScale;

    dX = player->actor.world.pos.x - self->dyna.actor.world.pos.x;
    dY = player->actor.world.pos.y - self->dyna.actor.world.pos.y;
    dZ = player->actor.world.pos.z - self->dyna.actor.world.pos.z;

    if ((dX > (self->dyna.actor.scale.x * -346.0f)) && (dX < (self->dyna.actor.scale.x * 346.0f)) &&
        (dZ > (self->dyna.actor.scale.z * -400.0f)) && (dZ < (self->dyna.actor.scale.z * 400.0f)) && (dY < 0.0f)) {
        if (func_8004356C(&self->dyna)) {
            if (self->splashTimer <= 0) {
                EffectSsGSplash_Spawn(globalCtx, &player->actor.world.pos, NULL, NULL, 1, 1);
                self->splashTimer = 10;
            } else {
                self->splashTimer--;
            }

            self->applySpeed = false;
            self->appliedSpeed = 0.0f;
            self->targetAppliedSpeed = 0.0f;
        } else {
            dist2d = sqrtf(SQ(dX) + SQ(dZ));
            self->applySpeed = true;
            self->splashTimer = 0;
            angle = Math_FAtan2F(dX, dZ) * (0x8000 / M_PI);
            dAngle = (player->actor.world.rot.y ^ 0x8000) - angle;
            player->actor.gravity = 0.0f;
            player->actor.velocity.y = 0.0f;
            Math_SmoothStepToF(&player->actor.world.pos.y, self->dyna.actor.world.pos.y, 0.5f, 4.0f, 1.0f);

            if ((dAngle < 0x4000) && (dAngle > -0x4000)) {
                self->appliedYaw = player->actor.world.rot.y ^ 0x8000;
                speedScale = dist2d / (self->dyna.actor.scale.x * 40.0f * 10.0f);
                speedScale = CLAMP_MIN(speedScale, 0.0f);
                speedScale = CLAMP_MAX(speedScale, 1.0f);
                player->linearVelocity *= speedScale;
                Math_ApproachF(&self->targetAppliedSpeed, 3.0f, 1.0f, 1.0f);
                Math_ApproachF(&self->appliedSpeed, self->targetAppliedSpeed, 1.0f, 0.3f * speedScale);
            } else {
                self->appliedYaw = player->actor.world.rot.y;
                player->linearVelocity /= 2.0f;
                Math_ApproachF(&self->targetAppliedSpeed, 3.0f, 1.0f, 1.0f);
                Math_ApproachF(&self->appliedSpeed, self->targetAppliedSpeed, 1.0f, 0.1f);
            }

            player->windDirection = self->appliedYaw;
            player->windSpeed = self->appliedSpeed;
        }
    } else {
        if (self->applySpeed) {
            player->linearVelocity = self->appliedSpeed + player->linearVelocity;
            player->currentYaw = self->appliedYaw;
        }

        self->targetAppliedSpeed = 0.0f;
        self->appliedSpeed = 0.0f;
        self->applySpeed = false;
    }
}

void func_80AFC1D0(EnSiofuki* self, GlobalContext* globalCtx) {
    Math_SmoothStepToF(&self->currentHeight, self->targetHeight, 0.8f, 3.0f, 0.01f);
}

void func_80AFC218(EnSiofuki* self, GlobalContext* globalCtx) {
    func_80AFBDC8(self, globalCtx);
    func_80AFBE8C(self, globalCtx);
    func_80AFC1D0(self, globalCtx);

    self->timer--;
    if (self->timer < 0) {
        Flags_UnsetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 6) & 0x3F);
        switch (((u16)self->dyna.actor.params >> 0xC) & 0xF) {
            case EN_SIOFUKI_RAISING:
                self->targetHeight = 10.0f;
                self->actionFunc = func_80AFC34C;
                break;
            case EN_SIOFUKI_LOWERING:
                self->targetHeight = self->maxHeight;
                self->actionFunc = func_80AFC478;
                break;
        }
    } else {
        func_8002F994(&self->dyna.actor, self->timer);
    }

    if (((((u16)self->dyna.actor.params >> 0xC) & 0xF) == EN_SIOFUKI_LOWERING) &&
        Flags_GetTreasure(globalCtx, (u16)self->dyna.actor.params & 0x3F)) {
        self->currentHeight = -45.0f;
        self->targetHeight = -45.0f;
        Flags_UnsetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 6) & 0x3F);
        self->actionFunc = func_80AFC544;
    }
}

void func_80AFC34C(EnSiofuki* self, GlobalContext* globalCtx) {
    func_80AFBDC8(self, globalCtx);
    func_80AFBE8C(self, globalCtx);
    func_80AFC1D0(self, globalCtx);

    if (Flags_GetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 6) & 0x3F)) {
        self->targetHeight = 400.0f;
        self->timer = 300;
        self->actionFunc = func_80AFC218;
    }
}

void func_80AFC3C8(EnSiofuki* self, GlobalContext* globalCtx) {
    func_80AFBDC8(self, globalCtx);
    func_80AFBE8C(self, globalCtx);
    func_80AFC1D0(self, globalCtx);

    self->timer--;
    if (self->timer < 0) {
        self->timer = self->activeTime * 20;
        self->targetHeight = -45.0f;
        self->actionFunc = func_80AFC218;
    }

    if (Flags_GetTreasure(globalCtx, (u16)self->dyna.actor.params & 0x3F)) {
        self->currentHeight = -45.0f;
        self->targetHeight = -45.0f;
        self->actionFunc = func_80AFC544;
    }
}

void func_80AFC478(EnSiofuki* self, GlobalContext* globalCtx) {
    func_80AFBDC8(self, globalCtx);
    func_80AFBE8C(self, globalCtx);
    func_80AFC1D0(self, globalCtx);

    if (((u16)self->dyna.actor.params >> 0xC & 0xF) == EN_SIOFUKI_LOWERING) {
        if (Flags_GetSwitch(globalCtx, ((u16)self->dyna.actor.params >> 6) & 0x3F)) {
            self->timer = 20;
            self->actionFunc = func_80AFC3C8;
            OnePointCutscene_Init(globalCtx, 5010, 40, &self->dyna.actor, MAIN_CAM);
        }

        if (Flags_GetTreasure(globalCtx, (u16)self->dyna.actor.params & 0x3F)) {
            self->currentHeight = -45.0f;
            self->targetHeight = -45.0f;
            self->actionFunc = func_80AFC544;
        }
    }
}

void func_80AFC544(EnSiofuki* self, GlobalContext* globalCtx) {
    func_80AFBDC8(self, globalCtx);
    func_80AFC1D0(self, globalCtx);
}

void EnSiofuki_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnSiofuki* self = THIS;

    self->actionFunc(self, globalCtx);
}

void EnSiofuki_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnSiofuki* self = THIS;
    u32 x;
    u32 y;
    u32 gameplayFrames = globalCtx->gameplayFrames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_siofuki.c", 654);
    func_80093D84(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, self->unk_170, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_siofuki.c", 662),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    x = gameplayFrames * 15;
    y = gameplayFrames * -15;
    gSPSegment(POLY_XLU_DISP++, 0x08, Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, x, y, 64, 64, 1, x, y, 64, 64));
    gSPDisplayList(POLY_XLU_DISP++, object_siofuki_DL_000B70);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_siofuki.c", 674);

    if (self->sfxFlags & 1) {
        f32 heightRatio;
        switch (((u16)thisx->params >> 0xC) & 0xF) {
            case EN_SIOFUKI_RAISING:
                heightRatio = (self->currentHeight - 10.0f) / (400.0f - 10.0f);
                func_800F436C(&thisx->projectedPos, NA_SE_EV_FOUNTAIN - SFX_FLAG, 1.0f + heightRatio);
                break;
            case EN_SIOFUKI_LOWERING:
                if (self->currentHeight > -35.0f) {
                    heightRatio = (self->currentHeight - -35.0f) / (self->maxHeight - -35.0f);
                    func_800F436C(&thisx->projectedPos, NA_SE_EV_FOUNTAIN - SFX_FLAG, 1.0f + heightRatio);
                }
                break;
        }
    }
}
