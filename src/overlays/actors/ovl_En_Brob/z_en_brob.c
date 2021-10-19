/*
 * File: z_en_brob.c
 * Overlay: ovl_En_Brob
 * Description: Flobbery Muscle Block (Jabu-Jabu's Belly)
 */

#include "z_en_brob.h"
#include "objects/object_brob/object_brob.h"

#define FLAGS 0x00000005

#define THIS ((EnBrob*)thisx)

void EnBrob_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBrob_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBrob_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBrob_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809CADDC(EnBrob* self, GlobalContext* globalCtx);
void func_809CB054(EnBrob* self, GlobalContext* globalCtx);
void func_809CB114(EnBrob* self, GlobalContext* globalCtx);
void func_809CB218(EnBrob* self, GlobalContext* globalCtx);
void func_809CB2B8(EnBrob* self, GlobalContext* globalCtx);
void func_809CB354(EnBrob* self, GlobalContext* globalCtx);
void func_809CB458(EnBrob* self, GlobalContext* globalCtx);

const ActorInit En_Brob_InitVars = {
    ACTOR_EN_BROB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BROB,
    sizeof(EnBrob),
    (ActorFunc)EnBrob_Init,
    (ActorFunc)EnBrob_Destroy,
    (ActorFunc)EnBrob_Update,
    (ActorFunc)EnBrob_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0xFFCFFFFF, 0x03, 0x08 },
        { 0xFFCFFFFF, 0x01, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 8000, 11000, -5000, { 0, 0, 0 } },
};

static CollisionCheckInfoInit sColChkInfoInit = { 0, 60, 120, MASS_IMMOVABLE };

void EnBrob_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnBrob* self = THIS;
    CollisionHeader* colHeader = NULL;

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_brob_Skel_0015D8, &object_brob_Anim_001750,
                       self->jointTable, self->morphTable, 10);
    DynaPolyActor_Init(&self->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(&object_brob_Col_001A70, &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
    Collider_InitCylinder(globalCtx, &self->colliders[0]);
    Collider_SetCylinder(globalCtx, &self->colliders[0], &self->dyna.actor, &sCylinderInit);
    Collider_InitCylinder(globalCtx, &self->colliders[1]);
    Collider_SetCylinder(globalCtx, &self->colliders[1], &self->dyna.actor, &sCylinderInit);
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &sColChkInfoInit);
    if (((thisx->params >> 8) & 0xFF) == 0) {
        Actor_SetScale(&self->dyna.actor, 0.01f);
        thisx->params &= 0xFF;
        if (thisx->params != 0xFF) {
            thisx->scale.y *= (thisx->params & 0xFF) * (1.0f / 30.0f);
        }
    } else {
        Actor_SetScale(&self->dyna.actor, 0.005f);
        thisx->params &= 0xFF;
        if (thisx->params != 0xFF) {
            thisx->scale.y *= (thisx->params & 0xFF) * (2.0f / 30.0f);
        }
    }
    self->colliders[0].dim.radius *= thisx->scale.x;
    self->colliders[0].dim.height = thisx->scale.y * 12000.0f;
    self->colliders[0].dim.yShift = 0;
    self->colliders[1].dim.radius *= thisx->scale.x;
    self->colliders[1].dim.height *= thisx->scale.y;
    self->colliders[1].dim.yShift *= thisx->scale.y;
    self->actionFunc = NULL;
    thisx->flags &= ~1;
    func_809CADDC(self, globalCtx);
}

void EnBrob_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBrob* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(globalCtx, &self->colliders[0]);
    Collider_DestroyCylinder(globalCtx, &self->colliders[1]);
}

void func_809CADDC(EnBrob* self, GlobalContext* globalCtx) {
    func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    self->timer = self->actionFunc == func_809CB2B8 ? 200 : 0;
    self->unk_1AE = 0;
    self->actionFunc = func_809CB054;
}

void func_809CAE44(EnBrob* self, GlobalContext* globalCtx) {
    Animation_PlayOnce(&self->skelAnime, &object_brob_Anim_001750);
    func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    self->unk_1AE = 1000;
    self->actionFunc = func_809CB114;
}

void func_809CAEA0(EnBrob* self) {
    Animation_MorphToLoop(&self->skelAnime, &object_brob_Anim_001958, -5.0f);
    self->unk_1AE = 8000;
    self->timer = 1200;
    self->actionFunc = func_809CB218;
}

void func_809CAEF4(EnBrob* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &object_brob_Anim_000290, -5.0f);
    self->unk_1AE -= 125.0f;
    Actor_SetColorFilter(&self->dyna.actor, 0, 0xFF, 0, 0x50);
    Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_GOMA_JR_FREEZE);
    self->actionFunc = func_809CB2B8;
}

void func_809CAF88(EnBrob* self) {
    Animation_Change(&self->skelAnime, &object_brob_Anim_001750, -1.0f,
                     Animation_GetLastFrame(&object_brob_Anim_001750), 0.0f, ANIMMODE_ONCE, -5.0f);
    self->unk_1AE = 8250;
    self->actionFunc = func_809CB354;
}

void func_809CB008(EnBrob* self) {
    Animation_MorphToLoop(&self->skelAnime, &object_brob_Anim_001678, -5.0f);
    self->timer = 10;
    self->actionFunc = func_809CB458;
}

void func_809CB054(EnBrob* self, GlobalContext* globalCtx) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        if (func_8004356C(&self->dyna) != 0) {
            func_8002F71C(globalCtx, &self->dyna.actor, 5.0f, self->dyna.actor.yawTowardsPlayer, 1.0f);
            func_809CAE44(self, globalCtx);
        } else if (self->dyna.actor.xzDistToPlayer < 300.0f) {
            func_809CAE44(self, globalCtx);
        }
    } else if (self->timer >= 81) {
        self->dyna.actor.colorFilterTimer = 80;
    }
}

void func_809CB114(EnBrob* self, GlobalContext* globalCtx) {
    f32 curFrame;

    if (SkelAnime_Update(&self->skelAnime)) {
        func_809CAEA0(self);
    } else {
        curFrame = self->skelAnime.curFrame;
        if (curFrame < 8.0f) {
            self->unk_1AE += 1000.0f;
        } else if (curFrame < 12.0f) {
            self->unk_1AE += 250.0f;
        } else {
            self->unk_1AE -= 250.0f;
        }
    }
}

void func_809CB218(EnBrob* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 6.0f) || Animation_OnFrame(&self->skelAnime, 15.0f)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EN_BROB_WAVE);
    }
    if (self->timer != 0) {
        self->timer--;
    }
    if ((self->timer == 0) && (self->dyna.actor.xzDistToPlayer > 500.0f)) {
        func_809CAF88(self);
    }
}

void func_809CB2B8(EnBrob* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        func_809CADDC(self, globalCtx);
    } else if (self->skelAnime.curFrame < 8.0f) {
        self->unk_1AE -= 1250.0f;
    }
    self->dyna.actor.colorFilterTimer = 0x50;
}

void func_809CB354(EnBrob* self, GlobalContext* globalCtx) {
    f32 curFrame;

    if (SkelAnime_Update(&self->skelAnime)) {
        func_809CADDC(self, globalCtx);
    } else {
        curFrame = self->skelAnime.curFrame;
        if (curFrame < 8.0f) {
            self->unk_1AE -= 1000.0f;
        } else if (curFrame < 12.0f) {
            self->unk_1AE -= 250.0f;
        } else {
            self->unk_1AE += 250.0f;
        }
    }
}

void func_809CB458(EnBrob* self, GlobalContext* globalCtx) {
    Vec3f pos;
    f32 dist1;
    f32 dist2;
    s32 i;

    SkelAnime_Update(&self->skelAnime);
    if (Animation_OnFrame(&self->skelAnime, 0) && (self->timer != 0)) {
        self->timer--;
    }

    dist1 = globalCtx->gameplayFrames % 2 ? 0.0f : self->dyna.actor.scale.x * 5500.0f;
    dist2 = self->dyna.actor.scale.x * 5500.0f;

    for (i = 0; i < 4; i++) {
        static Color_RGBA8 primColor = { 255, 255, 255, 255 };
        static Color_RGBA8 envColor = { 200, 255, 255, 255 };

        if (i % 2) {
            pos.x = self->dyna.actor.world.pos.x + dist1;
            pos.z = self->dyna.actor.world.pos.z + dist2;
        } else {
            pos.x = self->dyna.actor.world.pos.x + dist2;
            pos.z = self->dyna.actor.world.pos.z + dist1;
            dist1 = -dist1;
            dist2 = -dist2;
        }
        pos.y = (((Rand_ZeroOne() * 15000.0f) + 1000.0f) * self->dyna.actor.scale.y) + self->dyna.actor.world.pos.y;
        EffectSsLightning_Spawn(globalCtx, &pos, &primColor, &envColor, self->dyna.actor.scale.y * 8000.0f,
                                Rand_ZeroOne() * 65536.0f, 4, 1);
    }

    if (self->timer == 0) {
        func_809CAEA0(self);
    }
}

void EnBrob_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnBrob* self = THIS;
    s32 i;
    s32 acHits[2];

    acHits[0] = (self->colliders[0].base.acFlags & AC_HIT) != 0;
    acHits[1] = (self->colliders[1].base.acFlags & AC_HIT) != 0;
    if ((acHits[0] && (self->colliders[0].info.acHitInfo->toucher.dmgFlags & 0x10)) ||
        (acHits[1] && (self->colliders[1].info.acHitInfo->toucher.dmgFlags & 0x10))) {

        for (i = 0; i < 2; i++) {
            self->colliders[i].base.atFlags &= ~(AT_HIT | AT_BOUNCED);
            self->colliders[i].base.acFlags &= ~AC_HIT;
        }

        func_809CAEF4(self);
    } else if ((self->colliders[0].base.atFlags & AT_HIT) || (self->colliders[1].base.atFlags & AT_HIT) ||
               (acHits[0] && (self->colliders[0].info.acHitInfo->toucher.dmgFlags & 0x100)) ||
               (acHits[1] && (self->colliders[1].info.acHitInfo->toucher.dmgFlags & 0x100))) {

        if (self->actionFunc == func_809CB114 && !(self->colliders[0].base.atFlags & AT_BOUNCED) &&
            !(self->colliders[1].base.atFlags & AT_BOUNCED)) {
            func_8002F71C(globalCtx, &self->dyna.actor, 5.0f, self->dyna.actor.yawTowardsPlayer, 1.0f);
        } else if (self->actionFunc != func_809CB114) {
            func_809CB008(self);
        }

        for (i = 0; i < 2; i++) {
            self->colliders[i].base.atFlags &= ~(AT_HIT | AT_BOUNCED);
            self->colliders[i].base.acFlags &= ~AC_HIT;
        }
    }
    self->actionFunc(self, globalCtx);
    if (self->actionFunc != func_809CB054 && self->actionFunc != func_809CB354) {
        if (self->actionFunc != func_809CB2B8) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliders[0].base);
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliders[1].base);
            if (self->actionFunc != func_809CB114) {
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliders[0].base);
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliders[1].base);
            }
        }
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliders[0].base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliders[1].base);
    }
}

void EnBrob_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnBrob* self = THIS;
    MtxF mtx;

    Matrix_Get(&mtx);
    if (limbIndex == 3) {
        self->colliders[0].dim.pos.x = mtx.xw;
        self->colliders[0].dim.pos.y = mtx.yw;
        self->colliders[0].dim.pos.z = mtx.zw;
    } else if (limbIndex == 8) {
        self->colliders[1].dim.pos.x = mtx.xw;
        self->colliders[1].dim.pos.y = (mtx.yw + 7.0f);
        self->colliders[1].dim.pos.z = mtx.zw;
    }
}

void EnBrob_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBrob* self = THIS;

    func_80093D18(globalCtx->state.gfxCtx);
    Matrix_Translate(0.0f, self->unk_1AE, 0.0f, MTXMODE_APPLY);
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, EnBrob_PostLimbDraw, self);
}
