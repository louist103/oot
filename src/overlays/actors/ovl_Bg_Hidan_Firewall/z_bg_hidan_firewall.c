/*
 * File: z_bg_hidan_firewall.c
 * Overlay: ovl_Bg_Hidan_Firewall
 * Description: Proximity Triggered Flame Wall
 */

#include "z_bg_hidan_firewall.h"
#include "objects/object_hidan_objects/object_hidan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgHidanFirewall*)thisx)

void BgHidanFirewall_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFirewall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFirewall_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHidanFirewall_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 BgHidanFirewall_CheckProximity(BgHidanFirewall* self, GlobalContext* globalCtx);
void BgHidanFirewall_Wait(BgHidanFirewall* self, GlobalContext* globalCtx);
void BgHidanFirewall_Countdown(BgHidanFirewall* self, GlobalContext* globalCtx);
void BgHidanFirewall_Erupt(BgHidanFirewall* self, GlobalContext* globalCtx);
void BgHidanFirewall_Collide(BgHidanFirewall* self, GlobalContext* globalCtx);
void BgHidanFirewall_ColliderFollowPlayer(BgHidanFirewall* self, GlobalContext* globalCtx);

const ActorInit Bg_Hidan_Firewall_InitVars = {
    ACTOR_BG_HIDAN_FIREWALL,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanFirewall),
    (ActorFunc)BgHidanFirewall_Init,
    (ActorFunc)BgHidanFirewall_Destroy,
    (ActorFunc)BgHidanFirewall_Update,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x20000000, 0x01, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 30, 83, 0, { 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 1, 80, 100, MASS_IMMOVABLE };

void BgHidanFirewall_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanFirewall* self = THIS;

    self->actor.scale.x = 0.12f;
    self->actor.scale.z = 0.12f;
    self->actor.scale.y = 0.01f;

    self->unk_150 = 0;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);

    self->collider.dim.pos.y = self->actor.world.pos.y;

    CollisionCheck_SetInfo(&self->actor.colChkInfo, NULL, &sColChkInfoInit);

    self->actionFunc = BgHidanFirewall_Wait;
}

void BgHidanFirewall_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanFirewall* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 BgHidanFirewall_CheckProximity(BgHidanFirewall* self, GlobalContext* globalCtx) {
    Player* player;
    Vec3f distance;

    player = GET_PLAYER(globalCtx);
    func_8002DBD0(&self->actor, &distance, &player->actor.world.pos);

    if (fabsf(distance.x) < 100.0f && fabsf(distance.z) < 120.0f) {
        return 1;
    }
    return 0;
}

void BgHidanFirewall_Wait(BgHidanFirewall* self, GlobalContext* globalCtx) {
    if (BgHidanFirewall_CheckProximity(self, globalCtx) != 0) {
        self->actor.draw = BgHidanFirewall_Draw;
        self->actor.params = 5;
        self->actionFunc = BgHidanFirewall_Countdown;
    }
}

void BgHidanFirewall_Countdown(BgHidanFirewall* self, GlobalContext* globalCtx) {

    if (self->actor.params != 0) {
        self->actor.params--;
    }
    if (self->actor.params == 0) {
        self->actionFunc = BgHidanFirewall_Erupt;
    }
}

void BgHidanFirewall_Erupt(BgHidanFirewall* self, GlobalContext* globalCtx) {
    if (BgHidanFirewall_CheckProximity(self, globalCtx) != 0) {
        Math_StepToF(&self->actor.scale.y, 0.1f, 0.01f / 0.4f);
    } else {
        if (Math_StepToF(&self->actor.scale.y, 0.01f, 0.01f) != 0) {
            self->actor.draw = NULL;
            self->actionFunc = BgHidanFirewall_Wait;
        } else {
            self->actor.params = 0;
        }
    }
}

void BgHidanFirewall_Collide(BgHidanFirewall* self, GlobalContext* globalCtx) {
    s16 phi_a3;

    if (Actor_IsFacingPlayer(&self->actor, 0x4000)) {
        phi_a3 = self->actor.shape.rot.y;
    } else {
        phi_a3 = self->actor.shape.rot.y + 0x8000;
    }

    func_8002F71C(globalCtx, &self->actor, 5.0f, phi_a3, 1.0f);
}

void BgHidanFirewall_ColliderFollowPlayer(BgHidanFirewall* self, GlobalContext* globalCtx) {
    Player* player;
    Vec3f sp30;
    f32 temp_ret;
    f32 sp28;
    f32 phi_f0;

    player = GET_PLAYER(globalCtx);

    func_8002DBD0(&self->actor, &sp30, &player->actor.world.pos);
    if (sp30.x < -70.0f) {
        sp30.x = -70.0f;
    } else {
        if (70.0f < sp30.x) {
            phi_f0 = 70.0f;
        } else {
            phi_f0 = sp30.x;
        }
        sp30.x = phi_f0;
    }
    if (self->actor.params == 0) {
        if (0.0f < sp30.z) {
            sp30.z = -25.0f;
            self->actor.params = -1;
        } else {
            sp30.z = 25.0f;
            self->actor.params = 1;
        }
    } else {
        sp30.z = self->actor.params * 25.0f;
    }
    sp28 = Math_SinS(self->actor.shape.rot.y);
    temp_ret = Math_CosS(self->actor.shape.rot.y);
    self->collider.dim.pos.x = self->actor.world.pos.x + sp30.x * temp_ret + sp30.z * sp28;
    self->collider.dim.pos.z = self->actor.world.pos.z - sp30.x * sp28 + sp30.z * temp_ret;
}

void BgHidanFirewall_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanFirewall* self = THIS;
    s32 pad;

    self->unk_150 = (self->unk_150 + 1) % 8;

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        BgHidanFirewall_Collide(self, globalCtx);
    }

    self->actionFunc(self, globalCtx);
    if (self->actionFunc == BgHidanFirewall_Erupt) {
        BgHidanFirewall_ColliderFollowPlayer(self, globalCtx);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        func_8002F974(&self->actor, NA_SE_EV_FIRE_PLATE - SFX_FLAG);
    }
}

static void* sFireballTexs[] = {
    gFireTempleFireball0Tex, gFireTempleFireball1Tex, gFireTempleFireball2Tex, gFireTempleFireball3Tex,
    gFireTempleFireball4Tex, gFireTempleFireball5Tex, gFireTempleFireball6Tex, gFireTempleFireball7Tex,
};

void BgHidanFirewall_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHidanFirewall* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_firewall.c", 448);

    POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0x14);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sFireballTexs[self->unk_150]));
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x01, 255, 255, 0, 150);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 255);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_hidan_firewall.c", 458),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gFireTempleFireballUpperHalfDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_hidan_firewall.c", 463);
}
