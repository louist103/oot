#include "z_en_ny.h"

#define FLAGS 0x00000005

#define THIS ((EnNy*)thisx)

void EnNy_Init(Actor* thisx, GlobalContext* globalCtx);
void EnNy_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnNy_Update(Actor* thisx, GlobalContext* globalCtx);
void EnNy_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80ABDBF8(Actor* thisx, GlobalContext* globalCtx);
void func_80ABCF4C(EnNy* this, GlobalContext* globalCtx);
void func_80ABD9AC(EnNy* this, GlobalContext* globalCtx);
void func_80ABCD40(EnNy* this);
void func_80ABCDBC(EnNy* this);
void func_80ABD05C(EnNy* this, GlobalContext* globalCtx);
void func_80ABD11C(EnNy* this, GlobalContext* globalCtx);
void func_80ABCE50(EnNy* this, GlobalContext* globalCtx);
void func_80ABCE90(EnNy* this, GlobalContext* globalCtx);
void func_80ABCEEC(EnNy* this, GlobalContext* globalCtx);
void func_80ABDBB8(Actor* thisx, GlobalContext* GlobalContext);
void func_80ABD728(EnNy* this, GlobalContext* globalCtx);
void func_80ABE040(Actor* thisx, GlobalContext* GlobalContext);
void func_80ABD3B8(EnNy* this, f32, f32);

extern Gfx* D_06001DD0[];
extern Gfx* D_06001EA8[];
extern Gfx* D_06001FC8[];

const ActorInit En_Ny_InitVars = {
    ACTOR_EN_NY,
    ACTORTYPE_ENEMY,
    FLAGS,
    OBJECT_NY,
    sizeof(EnNy),
    (ActorFunc)EnNy_Init,
    (ActorFunc)EnNy_Destroy,
    (ActorFunc)EnNy_Update,
    (ActorFunc)EnNy_Draw,
};

static ColliderJntSphItemInit sJntSphItemsInit[1] = {
    {
        { 0x00, { 0xFFCFFFFF, 0x04, 0x08 }, { 0xFFCFFFFF, 0x00, 0x00 }, 0x01, 0x01, 0x01 },
        { 0, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sColliderInit = {
    { COLTYPE_UNK10, 0x11, 0x09, 0x39, 0x10, COLSHAPE_JNTSPH },
    1,
    sJntSphItemsInit,
};

static DamageTable sDamageTable = { {
    0x00, 0x00, 0x00, 0xF2, 0x00, 0xF2, 0xF2, 0x12, 0x00, 0xF2, 0xF4, 0x24, 0xF2, 0xF2, 0xE4, 0x00,
    0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF4, 0xF2, 0x00, 0xF8, 0xF4, 0x00, 0x00, 0x00, 0x00,
} };

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 40, ICHAIN_CONTINUE),
    ICHAIN_U8(unk_1F, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_4C, 30, ICHAIN_STOP),
};

void EnNy_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* this = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    thisx->colChkInfo.damageTable = &sDamageTable;
    thisx->colChkInfo.health = 2;
    Collider_InitJntSph(globalCtx, &this->collider);
    Collider_SetJntSph(globalCtx, &this->collider, thisx, &sColliderInit, &this->items);
    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawFunc_Circle, 20.0f);
    this->unk_1CA = 0;
    this->unk_1D0 = 0;
    Actor_SetScale(thisx, 0.01f);
    thisx->speedXZ = 0.0f;
    thisx->shape.rot.y = 0;
    thisx->gravity = -0.4f;
    this->unk_1CC = 0;
    this->unk_1CE = 2;
    thisx->velocity.y = 0.0f;
    this->unk_1D4 = 0xFF;
    this->unk_1D8 = 0;
    this->unk_1E8 = 0.0f;
    this->unk_1E0 = 0.25f;
    if (thisx->params == 0) {
        // New initials
        osSyncPrintf("ニュウ イニシャル[ %d ] ！！\n", thisx->params);
        thisx->colChkInfo.mass = 0;
        this->unk_1D4 = 0;
        this->unk_1D8 = 0xFF;
        this->unk_1E0 = 1.0f;
        func_80ABCDBC(this);
    } else {

        // Dummy new initials
        osSyncPrintf("ダミーニュウ イニシャル[ %d ] ！！\n", thisx->params);
        osSyncPrintf("En_Ny_actor_move2[ %x ] ！！\n", func_80ABDBF8);
        thisx->colChkInfo.mass = 0xFF;
        this->collider.base.type = COLTYPE_METAL_SHIELD;
        thisx->update = &func_80ABDBF8;
    }
}

void EnNy_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* this = THIS;
    Collider_DestroyJntSph(globalCtx, &this->collider);
}

void func_80ABCD40(EnNy* this) {
    f32 temp;

    temp = (this->actor.waterY > 0.0f) ? 0.7f : 1.0f;
    this->unk_1E8 = 2.8f * temp;
}

void func_80ABCD84(EnNy* this) {
    this->actionFunc = func_80ABCE50;
}

void func_80ABCD94(EnNy* this) {
    this->unk_1DC = 0x14;
    this->actionFunc = func_80ABCE90;
}

void func_80ABCDAC(EnNy* this) {
    this->actionFunc = func_80ABCEEC;
}

void func_80ABCDBC(EnNy* this) {
    this->unk_1F4 = 0.0f;
    func_80ABCD40(this);
    this->unk_1DC = 0xB4;
    this->actionFunc = func_80ABCF4C;
}

void func_80ABCDFC(EnNy* this) {
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_NYU_HIT_STOP);
    this->actionFunc = func_80ABD05C;
    this->unk_1E8 = 0.0f;
}

void func_80ABCE38(EnNy* this) {
    this->unk_1DC = 0x3C;
    this->actionFunc = func_80ABD11C;
}

void func_80ABCE50(EnNy* this, GlobalContext* globalCtx) {
    if (this->actor.xyzDistFromLinkSq <= 25600.0f) {
        func_80ABCD94(this);
    }
}

void func_80ABCE90(EnNy* this, GlobalContext* globalCtx) {
    s32 phi_v1;
    s32 phi_v0;

    phi_v1 = this->unk_1D4 - 0x40;
    phi_v0 = this->unk_1D8 + 0x40;
    if (phi_v0 >= 0xFF) {
        phi_v1 = 0;
        phi_v0 = 0xFF;
        func_80ABCDAC(this);
    }
    this->unk_1D4 = phi_v1;
    this->unk_1D8 = phi_v0;
}

void func_80ABCEEC(EnNy* this, GlobalContext* globalCtx) {
    f32 phi_f0;

    phi_f0 = this->unk_1E0;
    phi_f0 += 2.0f;
    if (phi_f0 >= 1.0f) {
        phi_f0 = 1.0f;
        func_80ABCDBC(this);
    }
    this->unk_1E0 = phi_f0;
}

void func_80ABCF4C(EnNy* this, GlobalContext* globalCtx) {
    f32 sp2C;
    s32 temp;

    if (!(this->unk_1F0 < this->actor.waterY)) {
        func_8002F974(&this->actor, NA_SE_EN_NYU_MOVE - SFX_FLAG);
    }
    func_80ABCD40(this);
    temp = this->unk_1DC;
    this->unk_1DC--;
    if ((temp <= 0) || (this->unk_1CC != 0)) {
        func_80ABCDFC(this);
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsLink, 0xA, this->unk_1F4, 0);
        Math_ApproachF(&this->unk_1F4, 2000.0f, 1.0f, 100.0f);
        this->actor.posRot.rot.y = this->actor.shape.rot.y;
        sp2C = Math_FAtan2F(this->actor.yDistFromLink, this->actor.xzDistFromLink);
        this->actor.speedXZ = fabsf(cosf(sp2C) * this->unk_1E8);
        if (this->unk_1F0 < this->actor.waterY) {
            this->unk_1EC = sinf(sp2C) * this->unk_1E8;
        }
    }
}

void func_80ABD05C(EnNy* this, GlobalContext* globalCtx) {
    f32 phi_f0;

    phi_f0 = this->unk_1E0;
    phi_f0 -= 2.0f;
    if (phi_f0 <= 0.25f) {
        phi_f0 = 0.25f;
        if (this->actor.bgCheckFlags & 2) {
            if (!(this->unk_1F0 < this->actor.waterY)) {
                Audio_PlayActorSound2(&this->actor, NA_SE_EN_DODO_M_GND);
            }
            this->actor.bgCheckFlags &= ~2;
            this->actor.speedXZ = 0.0f;
            this->actor.posRot.rot.y = this->actor.shape.rot.y;
            func_80ABCE38(this);
        }
    }
    this->unk_1E0 = phi_f0;
}

void func_80ABD11C(EnNy* this, GlobalContext* globalCtx) {
    s32 phi_v0;
    s32 phi_v1;

    phi_v0 = this->unk_1D4;
    phi_v0 += 0x40;
    phi_v1 = this->unk_1D8;
    phi_v1 -= 0x40;
    if (phi_v0 >= 0xFF) {
        phi_v0 = 0xFF;
        phi_v1 = 0;
        if (this->unk_1DC != 0) {
            this->unk_1DC--;
        } else {
            func_80ABCD84(this);
        }
    }
    this->unk_1D4 = phi_v0;
    this->unk_1D8 = phi_v1;
}

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ny/func_80ABD190.s")
s32 func_80ABD190(EnNy* this, GlobalContext* globalCtx) { // Collision Check
    u8 sp3F;
    Vec3f effectPos;

    sp3F = 0;
    this->unk_1CC = 0;
    if ((this->collider.base.atFlags & 4) != 0) {
        this->collider.base.atFlags = this->collider.base.atFlags & 0xFFFB;
        this->unk_1CC = 1;
        this->actor.posRot.rot.y = this->actor.yawTowardsLink;
        this->actor.speedXZ = -4.0f;
        return 0;
    }
    if ((this->collider.base.atFlags & 2) != 0) {
        this->collider.base.atFlags = this->collider.base.atFlags & 0xFFFD;
        this->unk_1CC = 1;
        return 0;
    } else {
        if ((this->collider.base.acFlags & 2) != 0) {
            this->collider.base.acFlags = this->collider.base.acFlags & 0xFFFD;
            effectPos.x = this->collider.list[0].body.bumper.unk_06.x;
            effectPos.y = this->collider.list[0].body.bumper.unk_06.y;
            effectPos.z = this->collider.list[0].body.bumper.unk_06.z;
            if ((this->unk_1E0 == 0.25f) && (this->unk_1D4 == 0xFF)) {
                switch (this->actor.colChkInfo.damageEffect) {
                    case 0xE:
                        sp3F = 1;
                    case 0xF:
                        Actor_ApplyDamage(&this->actor);
                        func_8003426C(&this->actor, 0x4000, 0xFF, 0x2000, 0x50);
                        break;
                    case 1:
                        Actor_ApplyDamage(&this->actor);
                        func_8003426C(&this->actor, 0x4000, 0xFF, 0x2000, 0x50);
                        break;
                    case 2:
                        this->unk_1CA = 4;
                        Actor_ApplyDamage(&this->actor);
                        func_8003426C(&this->actor, 0x4000, 0xFF, 0x2000, 0x50);
                        break;
                }
            }
            this->unk_1DC = 0;
            if (this->actor.colChkInfo.health == 0) {
                this->actor.shape.unk_14 = 0;
                this->actor.flags = this->actor.flags & ~1;
                this->unk_1D0 = sp3F;
                func_80032C7C(globalCtx, &this->actor);
                return 1;
            }
            EffectSsHitMark_SpawnFixedScale(globalCtx, 0, &effectPos);
            return 0;
        }
    }
    return 0;
}

void func_80ABD3B8(EnNy* this, f32 arg1, f32 arg2) {
    if (this->unk_1E8 == 0.0f) {
        this->actor.gravity = -0.4f;
    } else if (!(arg1 < this->actor.waterY)) {
        this->actor.gravity = -0.4f;
    } else if (arg2 < this->actor.waterY) {
        this->actor.gravity = 0.0;
        if (this->unk_1EC < this->actor.velocity.y) {
            this->actor.velocity.y -= 0.4f;
            if (this->actor.velocity.y < this->unk_1EC) {
                this->actor.velocity.y = this->unk_1EC;
            }
        } else if (this->actor.velocity.y < this->unk_1EC) {
            this->actor.velocity.y += 0.4f;
            if (this->unk_1EC < this->actor.velocity.y) {
                this->actor.velocity.y = this->unk_1EC;
            }
        }
    }
}

void EnNy_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* this = THIS;
    f32 temp_f20;
    f32 temp_f22;
    s32 i;

    this->unk_1C8++;
    temp_f20 = this->unk_1E0 - 0.25f;
    if (this->unk_1CA != 0) {
        this->unk_1CA--;
    }
    Actor_SetHeight(thisx, 0.0f);
    Actor_SetScale(thisx, 0.01f);
    this->collider.list->dim.scale = (1.33f * temp_f20) + 1.0f;
    temp_f22 = (24.0f * temp_f20) + 12.0f;
    thisx->shape.rot.x = thisx->shape.rot.x + (s16)(this->unk_1E8 * 1000.0f);
    func_80ABD3B8(this, temp_f22 + 10.0f, temp_f22 - 10.0f);
    Actor_MoveForward(thisx);
    Math_StepToF(&this->unk_1E4, this->unk_1E8, 0.1f);
    this->actionFunc(thisx, globalCtx);
    thisx->pos4.y -= temp_f22;
    thisx->posRot.pos.y -= temp_f22;
    func_8002E4B4(globalCtx, thisx, 20.0f, 20.0f, 60.0f, 7);
    this->unk_1F0 = temp_f22;
    thisx->posRot.pos.y = thisx->posRot.pos.y + temp_f22;
    if (func_80ABD190(this, globalCtx) != 0) {
        for (i = 0; i < 8; i++) {
            this->unk_1F8[i].x = (Rand_CenteredFloat(20.0f) + thisx->posRot.pos.x);
            this->unk_1F8[i].y = (Rand_CenteredFloat(20.0f) + thisx->posRot.pos.y);
            this->unk_1F8[i].z = (Rand_CenteredFloat(20.0f) + thisx->posRot.pos.z);
        }
        this->unk_1C8 = 0;
        thisx->update = func_80ABDBB8;
        thisx->draw = func_80ABE040;
        this->actionFunc = func_80ABD728;
        return;
    }
    if (this->unk_1E0 > 0.25f) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->collider);
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->collider);
}

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ny/func_80ABD728.s")
void func_80ABD728(EnNy* this, GlobalContext* globalCtx) {
    s32 temp;
    s32 i;
    Vec3f effectPos;
    Vec3f effectVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effectAccel = { 0.0f, 0.1f, 0.0f };

    if (this->unk_1C8 >= 2) {
        if (this->actor.waterY > 0.0f) {
            for (i = 0; i < 10; i++) {
                effectPos.x = Rand_CenteredFloat(30.0f) + this->actor.posRot.pos.x;
                effectPos.y = Rand_CenteredFloat(30.0f) + this->actor.posRot.pos.y;
                effectPos.z = Rand_CenteredFloat(30.0f) + this->actor.posRot.pos.z;
                temp = Rand_S16Offset(0x50, 0x64);
                EffectSsDtBubble_SpawnColorProfile(globalCtx, &effectPos, &effectVelocity, &effectAccel,
                                                   temp, 0x19, 0, 1);
            }
            for (i = 0; i < 0x14; i++) {
                effectPos.x = Rand_CenteredFloat(30.0f) + this->actor.posRot.pos.x;
                effectPos.y = Rand_CenteredFloat(30.0f) + this->actor.posRot.pos.y;
                effectPos.z = Rand_CenteredFloat(30.0f) + this->actor.posRot.pos.z;
                EffectSsBubble_Spawn(globalCtx, &effectPos, 10.0f, 10.0f, 30.0f, 0.25f);
            }
        }
        for (i = 0; i < 8; i++) {
            this->unk_1F8[i + 8].x = Rand_CenteredFloat(10.0f);
            this->unk_1F8[i + 8].z = Rand_CenteredFloat(10.0f);
            this->unk_1F8[i + 8].y = Rand_ZeroFloat(4.0f) + 4.0f;
        }
        this->unk_1C8 = 0;
        if (this->unk_1D0 == 0) {
            Item_DropCollectibleRandom(globalCtx, &this->actor, &this->actor.posRot.pos, 0xA0);
        } else {
            Item_DropCollectible(globalCtx, &this->actor.posRot.pos, 8);
        }
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_NYU_DEAD);
        this->actionFunc = &func_80ABD9AC;
    }
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ny/func_80ABD9AC.s")

//#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ny/func_80ABDBB8.s")
void func_80ABDBB8(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* this = THIS;

    this->unk_1C8++;
    if (this->unk_1CA != 0) {
        this->unk_1CA--;
    }
    this->actionFunc(this, globalCtx);
}

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Ny/func_80ABDBF8.s")

Vec3f sOffset[] = {
    { 5.0f, 0.0f, 0.0f },
    { -5.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 5.0f },
    { 0.0f, 0.0f, -5.0f },
};
void EnNy_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnNy* this = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ny.c", 837);
    func_800628A4(0, &this->collider);
    func_8002ED80(&this->actor, globalCtx, 1);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ny.c", 845),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_PASS, G_RM_AA_ZB_XLU_SURF2);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->unk_1D8);
    gSPDisplayList(POLY_XLU_DISP++, D_06001EA8);
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetRenderMode(POLY_XLU_DISP++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->unk_1D4);
    gSPDisplayList(POLY_XLU_DISP++, D_06001DD0);
    if (this->unk_1E0 > 0.25f) {
        Matrix_Scale(this->unk_1E0, this->unk_1E0, this->unk_1E0, MTXMODE_APPLY);
        func_8002EBCC(&this->actor, globalCtx, 1);
        func_80093D18(globalCtx->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ny.c", 868),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, D_06001FC8);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ny.c", 872);
    if (this->unk_1CA != 0) {
        Vec3f tempVec;
        Vec3f* offset;
        s16 temp;
        temp = this->unk_1CA - 1;
        this->actor.dmgEffectTimer++;
        if (temp == 0) {
            offset = &sOffset[temp & 3];
            tempVec.x = Rand_CenteredFloat(5.0f) + (this->actor.posRot.pos.x + offset->x);
            tempVec.y = Rand_CenteredFloat(5.0f) + (this->actor.posRot.pos.y + offset->y);
            tempVec.z = Rand_CenteredFloat(5.0f) + (this->actor.posRot.pos.z + offset->z);
            EffectSsEnFire_SpawnVec3f(globalCtx, &this->actor, &tempVec, 100, 0, 0, -1);
        }
    }
}
// Draw Effect?

void func_80ABE040(Actor* thisx, GlobalContext* globalCtx) {
    EnNy* this = THIS;
    Vec3f* temp;
    f32 temp_f12;
    s32 i;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ny.c", 900);
    func_80093D18(globalCtx->state.gfxCtx);
    gDPSetEnvColor(POLY_OPA_DISP++, 0x00, 0x00, 0x00, 0xFF);
    gDPSetRenderMode(POLY_OPA_DISP++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(POLY_OPA_DISP++);
    for (i = 0; i < 8; i++) {
        if (this->unk_1C8 < (i + 0x16)) {
            temp = &this->unk_1F8[i];
            Matrix_Translate(temp->x, temp->y, temp->z, MTXMODE_NEW);
            temp_f12 = thisx->scale.x * 0.4f * (1.0f + (i * 0.04f));
            Matrix_Scale(temp_f12, temp_f12, temp_f12, MTXMODE_APPLY);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_ny.c", 912),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, D_06001DD0);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ny.c", 919);
    if (this->unk_1CA != 0) {
        Vec3f tempVec;
        Vec3f* offset;
        s16 temp;
        temp = this->unk_1CA - 1;
        thisx->dmgEffectTimer++;
        if ((temp & 1) == 0) {
            offset = &sOffset[temp & 3];
            tempVec.x = Rand_CenteredFloat(5.0f) + (thisx->posRot.pos.x + offset->x);
            tempVec.y = Rand_CenteredFloat(5.0f) + (thisx->posRot.pos.y + offset->y);
            tempVec.z = Rand_CenteredFloat(5.0f) + (thisx->posRot.pos.z + offset->z);
            EffectSsEnFire_SpawnVec3f(globalCtx, thisx, &tempVec, 100, 0, 0, -1);
        }
    }
}
