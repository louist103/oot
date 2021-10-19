/*
 * File: z_eff_ss_kakera.c
 * Overlay: ovl_Effect_Ss_Kakera
 * Description: Fragments. Appearance is determined by the supplied display list.
 */

#include "z_eff_ss_kakera.h"

#define rReg0 regs[0]
#define rGravity regs[1]
#define rPitch regs[2]
#define rYaw regs[3]
#define rReg4 regs[4]
#define rReg5 regs[5]
#define rReg6 regs[6]
#define rScale regs[7]
#define rReg8 regs[8]
#define rReg9 regs[9]
#define rObjId regs[10]
#define rObjBankIdx regs[11]
#define rColorIdx regs[12]

u32 EffectSsKakera_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx);
void EffectSsKakera_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self);
void EffectSsKakera_Update(GlobalContext* globalCtx, u32 index, EffectSs* self);

void func_809A9BA8(EffectSs* self, GlobalContext* globalCtx);

EffectSsInit Effect_Ss_Kakera_InitVars = {
    EFFECT_SS_KAKERA,
    EffectSsKakera_Init,
};

u32 EffectSsKakera_Init(GlobalContext* globalCtx, u32 index, EffectSs* self, void* initParamsx) {
    EffectSsKakeraInitParams* initParams = (EffectSsKakeraInitParams*)initParamsx;
    s32 objId;

    self->pos = initParams->pos;
    self->velocity = initParams->velocity;
    self->life = initParams->life;
    self->priority = 101;

    if (initParams->dList != NULL) {
        self->gfx = initParams->dList;
        objId = initParams->objId;

        if (objId == OBJECT_GAMEPLAY_KEEP || objId == OBJECT_GAMEPLAY_FIELD_KEEP ||
            objId == OBJECT_GAMEPLAY_DANGEON_KEEP) {
            self->rObjId = KAKERA_OBJECT_DEFAULT;
        } else {
            self->rObjId = initParams->objId;
            func_809A9BA8(self, globalCtx);
        }

    } else {
        osSyncPrintf("shape_modelがNULL\n");
        LogUtils_HungupThread("../z_eff_kakera.c", 178);
    }

    self->draw = EffectSsKakera_Draw;
    self->update = EffectSsKakera_Update;
    self->vec = initParams->unk_18;
    self->rReg0 = initParams->unk_2C;
    self->rGravity = initParams->gravity;
    self->rPitch = Rand_ZeroOne() * 32767.0f;
    self->rYaw = Rand_ZeroOne() * 32767.0f;
    self->rReg4 = initParams->unk_26;
    self->rReg5 = initParams->unk_28;
    self->rReg6 = initParams->unk_2A;
    self->rScale = initParams->scale;
    self->rReg8 = initParams->unk_30;
    self->rReg9 = initParams->unk_32;
    self->rColorIdx = initParams->colorIdx;

    return 1;
}

f32 func_809A9818(f32 arg0, f32 arg1) {
    f32 temp_f2;

    if (arg1 < 0.0f) {
        osSyncPrintf("範囲がマイナス！！(randomD_sectionUniformity)\n");
    }

    temp_f2 = Rand_ZeroOne() * arg1;
    return ((temp_f2 * 2.0f) - arg1) + arg0;
}

void EffectSsKakera_Draw(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    static Color_RGB8 colors[] = { { 255, 255, 255 }, { 235, 170, 130 } };
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad;
    f32 scale;
    s32 colorIdx;

    scale = self->rScale / 256.0f;
    colorIdx = self->rColorIdx;

    OPEN_DISPS(gfxCtx, "../z_eff_kakera.c", 241);

    if (self->rObjId != KAKERA_OBJECT_DEFAULT) {
        if ((((self->rReg4 >> 7) & 1) << 7) == 0x80) {
            gSPSegment(POLY_XLU_DISP++, 0x06, globalCtx->objectCtx.status[self->rObjBankIdx].segment);
        } else {
            gSPSegment(POLY_OPA_DISP++, 0x06, globalCtx->objectCtx.status[self->rObjBankIdx].segment);
        }
    }

    Matrix_Translate(self->pos.x, self->pos.y, self->pos.z, MTXMODE_NEW);
    Matrix_RotateY(self->rYaw * 0.01f, MTXMODE_APPLY);
    Matrix_RotateX(self->rPitch * 0.01f, MTXMODE_APPLY);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);

    if ((((self->rReg4 >> 7) & 1) << 7) == 0x80) {
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_kakera.c", 268),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_80093D84(globalCtx->state.gfxCtx);

        if (colorIdx >= 0) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, colors[colorIdx].r, colors[colorIdx].g, colors[colorIdx].b, 255);
        }

        gSPDisplayList(POLY_XLU_DISP++, self->gfx);
    } else {
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gfxCtx, "../z_eff_kakera.c", 286),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_80093D18(globalCtx->state.gfxCtx);

        if (colorIdx >= 0) {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, colors[colorIdx].r, colors[colorIdx].g, colors[colorIdx].b, 255);
        }

        gSPDisplayList(POLY_OPA_DISP++, self->gfx);
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_kakera.c", 302);
}

void func_809A9BA8(EffectSs* self, GlobalContext* globalCtx) {
    self->rObjBankIdx = Object_GetIndex(&globalCtx->objectCtx, self->rObjId);

    if ((self->rObjBankIdx < 0) || !Object_IsLoaded(&globalCtx->objectCtx, self->rObjBankIdx)) {
        self->life = 0;
        self->draw = NULL;
    }
}

void func_809A9C10(EffectSs* self) {
    f32 temp_f14;
    f32 temp_f12;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f18;
    f32 temp_f20;
    f32 temp_f0;

    temp_f18 = self->rReg5 / 1024.0f;
    temp_f20 = self->rReg6 / 1024.0f;
    temp_f14 = (self->rReg9 / 1024.0f) * 4.0f;

    temp_f2 = self->velocity.x - func_809A9818(0.0f, temp_f14);
    temp_f16 = self->velocity.y - func_809A9818(0.0f, temp_f14);
    temp_f12 = self->velocity.z - func_809A9818(0.0f, temp_f14);

    if (temp_f2 > 0.0f) {
        self->velocity.x -= ((temp_f2 * temp_f18) + (SQ(temp_f2) * temp_f20));
    } else {
        self->velocity.x -= ((temp_f2 * temp_f18) - (SQ(temp_f2) * temp_f20));
    }

    if (temp_f16 > 0.0f) {
        temp_f0 = temp_f16 * temp_f18;
        temp_f2 = SQ(temp_f16) * temp_f20;
        self->velocity.y -= (temp_f0 + temp_f2);
    } else {
        temp_f0 = temp_f16 * temp_f18;
        temp_f2 = SQ(temp_f16) * temp_f20;
        self->velocity.y -= (temp_f0 - temp_f2);
    }

    if (temp_f12 > 0.0f) {
        self->velocity.z -= (temp_f0 + temp_f2);
    } else {
        self->velocity.z -= (temp_f0 - temp_f2);
    }
}

void func_809A9DC0(EffectSs* self) {
    self->accel.x = self->accel.y = self->accel.z = 0.0f;
}

f32 func_809A9DD8(f32 arg0, s32 arg1) {
    return 1.0f;
}

static f32 D_809AA530[] = {
    1.0f, 100.0f, 40.0f, 5.0f, 100.0f, 40.0f, 5.0f, 100.0f, 40.0f, 5.0f,
};

f32 func_809A9DEC(f32 arg0, s32 arg1) {
    if (D_809AA530[arg1] < arg0) {
        return D_809AA530[arg1] / arg0;
    } else {
        return 1.0f;
    }
}

f32 func_809A9E28(f32 arg0, s32 arg1) {
    f32 temp = SQ(arg0);

    if (D_809AA530[arg1] < temp) {
        return D_809AA530[arg1] / temp;
    } else {
        return 1.0f;
    }
}

f32 func_809A9E68(f32 arg0, s32 arg1) {
    return func_809A9E28(arg0, arg1);
}

s32 func_809A9E88(EffectSs* self, Vec3f* diff, f32 dist) {
    static f32 D_809AA558[] = { 0.05f, 1.0f };
    s32 temp_v0;
    f32 phi_f0;

    temp_v0 = self->rReg0 & 3;

    if (temp_v0 != 0) {

        if (dist > 1.0f) {
            phi_f0 = 1.0f / dist;
        } else {
            phi_f0 = 1.0f;
        }

        self->accel.x += ((D_809AA558[temp_v0 - 1] * diff->z) * phi_f0);
        self->accel.z -= ((D_809AA558[temp_v0 - 1] * diff->x) * phi_f0);
    }

    return 1;
}

s32 func_809A9F10(EffectSs* self, Vec3f* diff, f32 dist) {
    static f32 D_809AA560[] = { 4.0f, 0.1f, 0.3f, 0.9f, -0.1f, -0.3f, -0.9f };
    s32 temp_v0;

    temp_v0 = (self->rReg0 >> 2) & 7;

    if (temp_v0 != 0) {
        self->accel.y += D_809AA560[temp_v0];
    }

    return 1;
}

s32 func_809A9F4C(EffectSs* self, Vec3f* diff, f32 dist) {
    static f32 D_809AA57C[] = { 0.1f, 1.0f, 6.0f };
    s32 temp_v0;
    f32 phi_f0;

    temp_v0 = (self->rReg0 >> 5) & 3;

    if (temp_v0 != 0) {

        if (dist > 1.0f) {
            phi_f0 = 1.0f / dist;
        } else {
            phi_f0 = 1.0f;
        }

        self->accel.x -= ((diff->x * D_809AA57C[temp_v0 - 1]) * phi_f0);
        self->accel.z -= ((diff->z * D_809AA57C[temp_v0 - 1]) * phi_f0);
    }

    return 1;
}

s32 func_809A9FD8(EffectSs* self, Vec3f* diff, f32 dist) {
    static f32 (*D_809AA588[])(f32 dist, s32 arg1) = {
        func_809A9DD8, func_809A9DEC, func_809A9DEC, func_809A9DEC, func_809A9E28,
        func_809A9E28, func_809A9E28, func_809A9E68, func_809A9E68, func_809A9E68,
    };
    f32 temp_f0;
    s32 temp_a1;

    temp_a1 = (self->rReg0 >> 7) & 0xF;
    temp_f0 = D_809AA588[temp_a1](dist, temp_a1);
    temp_f0 = func_809A9818(temp_f0, (self->rReg9 * temp_f0) / 1024.0f);

    self->accel.x *= temp_f0;
    self->accel.y *= temp_f0;
    self->accel.z *= temp_f0;

    self->accel.x += temp_f0 * 0.01f;
    self->accel.y += temp_f0 * 0.01f;
    self->accel.z += temp_f0 * 0.01f;

    return 1;
}

s32 func_809AA0B8(EffectSs* self, Vec3f* diff, f32 dist) {
    self->accel.y += self->rGravity / 256.0f;

    return 1;
}

s32 func_809AA0EC(EffectSs* self) {
    Vec3f diff;
    f32 dist;

    func_809A9DC0(self);

    diff.x = self->pos.x - self->vec.x;
    diff.y = self->pos.y - self->vec.y;
    diff.z = self->pos.z - self->vec.z;

    dist = sqrtf(SQ(diff.x) + SQ(diff.y) + SQ(diff.z));

    if (dist > 1000.0f) {
        return 0;
    }

    if (self->rReg0 != 0) {
        if (!func_809A9E88(self, &diff, dist)) {
            return false;
        }

        if (!func_809A9F10(self, &diff, dist)) {
            return false;
        }

        if (!func_809A9F4C(self, &diff, dist)) {
            return false;
        }

        if (!func_809A9FD8(self, &diff, dist)) {
            return false;
        }
    }

    if (!func_809AA0B8(self, &diff, dist)) {
        return false;
    }

    return true;
}

void func_809AA230(EffectSs* self, GlobalContext* globalCtx) {
    static f32 D_809AA5B0[] = { 10.0f, 20.0f, 40.0f };
    Player* player = GET_PLAYER(globalCtx);

    if (self->rReg8 == 0) {
        if ((((self->rReg4 >> 4) & 1) * 0x10) == 0x10) {
            if (self->pos.y <= (player->actor.floorHeight - ((self->rReg4 >> 2) & 3))) {
                self->rReg9 = 0;
                self->rReg0 = 0;
                self->rReg4 &= ~0x60;
                self->accel.x = self->accel.y = self->accel.z = 0.0f;
                self->velocity.x = self->velocity.y = self->velocity.z = 0.0f;
                self->rReg5 = self->rReg9;
                self->rGravity = self->rReg9;
            }
        } else {
            if (self->pos.y <= ((player->actor.floorHeight - ((self->rReg4 >> 2) & 3)) - 600.0f)) {
                self->life = 0;
            }
        }
    } else {
        switch (self->rReg4 & 3) {
            case 0:
                self->rReg8 = 0;
                break;
            case 1:
                if (self->velocity.y < 0.0f) {
                    if (BgCheck_SphVsFirstPoly(&globalCtx->colCtx, &self->pos, D_809AA5B0[(self->rReg4 >> 2) & 3])) {
                        self->velocity.x *= func_809A9818(0.9f, 0.2f);
                        self->velocity.y *= -0.8f;
                        self->velocity.z *= func_809A9818(0.9f, 0.2f);

                        if (self->rReg8 > 0) {
                            self->rReg8 -= 1;
                        }
                    }
                }
                break;
            case 2:
                if (BgCheck_SphVsFirstPoly(&globalCtx->colCtx, &self->pos, D_809AA5B0[(self->rReg4 >> 2) & 3])) {}
                break;
        }
    }
}

void EffectSsKakera_Update(GlobalContext* globalCtx, u32 index, EffectSs* self) {
    switch (((self->rReg4 >> 5) & 3) << 5) {
        case 0x20:
            self->rPitch += 0xB;
            self->rYaw += 3;
            break;
        case 0x40:
            self->rPitch += 0x41;
            self->rYaw += 0xB;
            break;
        case 0x60:
            self->rPitch += 0x9B;
            self->rYaw += 0x1F;
            break;
    }

    func_809A9C10(self);

    if (!func_809AA0EC(self)) {
        self->life = 0;
    }

    func_809AA230(self, globalCtx);

    if (self->rObjId != KAKERA_OBJECT_DEFAULT) {
        func_809A9BA8(self, globalCtx);
    }
}
