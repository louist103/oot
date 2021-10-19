/*
 * File: z_en_gs.c
 * Overlay: ovl_En_Gs
 * Description: Gossip Stone
 */

#include "z_en_gs.h"
#include "objects/object_gs/object_gs.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x02000009

#define THIS ((EnGs*)thisx)

void EnGs_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGs_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGs_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGs_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A4F734(EnGs* self, GlobalContext* globalCtx);
void func_80A4F700(EnGs* self, GlobalContext* globalCtx);

void func_80A4F77C(EnGs* self);

const ActorInit En_Gs_InitVars = {
    ACTOR_EN_GS,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GS,
    sizeof(EnGs),
    (ActorFunc)EnGs_Init,
    (ActorFunc)EnGs_Destroy,
    (ActorFunc)EnGs_Update,
    (ActorFunc)EnGs_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HARD,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 21, 48, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0xE),
    /* Explosive     */ DMG_ENTRY(0, 0xC),
    /* Boomerang     */ DMG_ENTRY(0, 0xE),
    /* Normal arrow  */ DMG_ENTRY(0, 0xE),
    /* Hammer swing  */ DMG_ENTRY(0, 0xD),
    /* Hookshot      */ DMG_ENTRY(0, 0xE),
    /* Kokiri sword  */ DMG_ENTRY(0, 0xF),
    /* Master sword  */ DMG_ENTRY(0, 0xF),
    /* Giant's Knife */ DMG_ENTRY(0, 0xF),
    /* Fire arrow    */ DMG_ENTRY(0, 0xB),
    /* Ice arrow     */ DMG_ENTRY(0, 0xB),
    /* Light arrow   */ DMG_ENTRY(0, 0xB),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0xB),
    /* Ice magic     */ DMG_ENTRY(0, 0xB),
    /* Light magic   */ DMG_ENTRY(0, 0xB),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(0, 0x0),
    /* Giant spin    */ DMG_ENTRY(0, 0x0),
    /* Master spin   */ DMG_ENTRY(0, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(0, 0x0),
    /* Giant jump    */ DMG_ENTRY(0, 0x0),
    /* Master jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void EnGs_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGs* self = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, thisx, &sCylinderInit);
    CollisionCheck_SetInfo2(&thisx->colChkInfo, &sDamageTable, &sColChkInfoInit);

    thisx->targetMode = 6;
    self->unk_1D8 = thisx->world.pos;
    self->actionFunc = func_80A4F734;
    self->unk_1B4[0].x = 1.0f;
    self->unk_1B4[0].y = 1.0f;
    self->unk_1B4[0].z = 1.0f;
    self->unk_1B4[1].x = 1.0f;
    self->unk_1B4[1].y = 1.0f;
    self->unk_1B4[1].z = 1.0f;
}

void EnGs_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

s32 func_80A4E3EC(EnGs* self, GlobalContext* globalCtx) {
    s32 ret = 2;
    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 6:
            if (func_80106BC8(globalCtx)) {
                switch (self->actor.textId) {
                    case 0x2054:
                        self->actor.textId = (self->actor.params & 0xFF) + 0x400;
                        ret = 1;
                        break;
                    default:
                        ret = 0;
                        break;
                }
            }
            break;
    }
    return ret;
}

void func_80A4E470(EnGs* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    bREG(15) = 0;
    if (self->actor.xzDistToPlayer <= 100.0f) {
        bREG(15) = 1;
        if (self->unk_19D == 0) {
            player->stateFlags2 |= 0x800000;
            if (player->stateFlags2 & 0x1000000) {
                func_8010BD58(globalCtx, 1);
                self->unk_19D |= 1;
            }

        } else if (self->unk_19D & 1) {
            if (globalCtx->msgCtx.unk_E3EE == 4) {
                if ((globalCtx->msgCtx.unk_E3F2 == 6) || (globalCtx->msgCtx.unk_E3F2 == 7) ||
                    (globalCtx->msgCtx.unk_E3F2 == 8) || (globalCtx->msgCtx.unk_E3F2 == 9) ||
                    (globalCtx->msgCtx.unk_E3F2 == 10)) {
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ELF, self->actor.world.pos.x,
                                self->actor.world.pos.y + 40.0f, self->actor.world.pos.z, 0, 0, 0, FAIRY_HEAL_TIMED);
                    Audio_PlayActorSound2(&self->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                } else if (globalCtx->msgCtx.unk_E3F2 == 11) {
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ELF, self->actor.world.pos.x,
                                self->actor.world.pos.y + 40.0f, self->actor.world.pos.z, 0, 0, 0, FAIRY_HEAL_BIG);
                    Audio_PlayActorSound2(&self->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                }
                self->unk_19D = 0;
                Flags_SetSwitch(globalCtx, (self->actor.params >> 8) & 0x3F);
            } else if (globalCtx->msgCtx.unk_E3EE == 1) {
                player->stateFlags2 |= 0x800000;
            }
        }
    }
}

void func_80A4E648(EnGs* self, GlobalContext* globalCtx) {
    s16 sp26;
    s16 sp24;

    if (self->unk_19C == 1) {
        func_8010B720(globalCtx, self->actor.textId);
        self->unk_19C = 2;
    } else if (self->unk_19C == 2) {
        self->unk_19C = func_80A4E3EC(self, globalCtx);
    } else if (func_8002F194(&self->actor, globalCtx)) {
        self->unk_19C = 2;
    } else {
        func_8002F374(globalCtx, &self->actor, &sp26, &sp24);
        if ((sp26 >= 0) && (sp26 <= SCREEN_WIDTH) && (sp24 >= 0) && (sp24 <= SCREEN_HEIGHT) && (self->unk_19C != 3)) {
            if (func_8002F2CC(&self->actor, globalCtx, 40.0f) == 1) {
                if (Player_GetMask(globalCtx) == PLAYER_MASK_TRUTH) {
                    self->actor.textId = 0x2054;
                } else {
                    self->actor.textId = 0x2053;
                }
            }
        }
    }
}

f32 func_80A4E754(EnGs* self, GlobalContext* globalCtx, f32* arg2, f32* arg3, u16* arg4, f32 arg5, f32 arg6, f32 arg7,
                  s32 arg8, s32 arg9) {
    f32 sp2C = Math_SmoothStepToF(arg2, *arg3, arg5, arg6, arg7);

    if (arg9 == 0) {
        sp2C = Math_SmoothStepToF(arg2, *arg3, arg5, arg6, arg7);
        self->unk_1B4[0].x = 1.0f + (sinf((((*arg4 % arg8) * (1.0f / arg8)) * 360.0f) * (M_PI / 180.0f)) * *arg2);
        self->unk_1B4[0].y = 1.0f - (sinf((((*arg4 % arg8) * (1.0f / arg8)) * 360.0f) * (M_PI / 180.0f)) * *arg2);
        *arg4 += 1;
    }
    return sp2C;
}

void func_80A4E910(EnGs* self, GlobalContext* globalCtx) {
    if (self->unk_19F == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALKID_ATTACK);
        self->unk_200 = 0;
        self->unk_19F = 1;
        self->unk_1E8 = 0.5f;
        self->unk_1EC = 0.0f;
    } else if ((self->unk_19F == 1) && (func_80A4E754(self, globalCtx, &self->unk_1E8, &self->unk_1EC, &self->unk_200,
                                                      0.8f, 0.007f, 0.001f, 7, 0) == 0.0f)) {
        if (!Gameplay_InCsMode(globalCtx)) {
            func_8010B680(globalCtx, 0x71B1, NULL);
        }
        self->unk_19C = 0;
        self->actionFunc = func_80A4F734;
    }
}

void func_80A4EA08(EnGs* self, GlobalContext* globalCtx) {
    if (self->unk_19F == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALKID_ATTACK);
        self->unk_1E8 = 0.3f;
        self->unk_1EC = 0.0f;
        self->unk_200 = 0;
        self->unk_19F = 1;
    } else if (self->unk_19F == 1) {
        self->unk_1A0[0].z = (((self->unk_200 % 8) / 8.0f) * 360.0f) * (0x10000 / 360.0f);
        self->unk_1A0[1].z = -self->unk_1A0[0].z;
        if (func_80A4E754(self, globalCtx, &self->unk_1E8, &self->unk_1EC, &self->unk_200, 0.8f, 0.005f, 0.001f, 7,
                          0) == 0.0f) {
            self->unk_19C = 0;
            self->actionFunc = func_80A4F734;
        }
    }
}

void func_80A4EB3C(EnGs* self, GlobalContext* globalCtx) {
    f32 ret;

    if (self->unk_19F == 0) {
        self->unk_1E8 = self->unk_1B4[0].y - 1.0f;
        self->unk_1EC = -0.8f;
        self->unk_19F++;
    } else if (self->unk_19F == 1) {
        ret = Math_SmoothStepToF(&self->unk_1E8, self->unk_1EC, 1.0f, 0.4f, 0.001f);
        self->unk_1B4[0].y = self->unk_1E8 + 1.0f;
        if (ret == 0.0f) {
            self->unk_200 = 0;
            self->unk_19F++;
        }
    } else if (self->unk_19F == 2) {
        self->unk_200++;
        if (self->unk_200 >= 100) {
            self->unk_19F++;
            self->unk_1E8 = self->unk_1B4[0].y - 1.0f;
            self->unk_1EC = 0.0f;
        }
    } else if (self->unk_19F == 3) {
        ret = Math_SmoothStepToF(&self->unk_1E8, self->unk_1EC, 1.0f, 0.5f, 0.001f);
        self->unk_1B4[0].y = self->unk_1E8 + 1.0f;
        if (ret == 0.0f) {
            self->unk_1E8 = 0.5f;
            self->unk_1EC = 0.0f;
            self->unk_200 = 0;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALKID_ATTACK);
            self->unk_19F++;
        }
    } else if (self->unk_19F == 4) {
        if (func_80A4E754(self, globalCtx, &self->unk_1E8, &self->unk_1EC, &self->unk_200, 1.0f, 0.03f, 0.001f, 5, 0) ==
            0.0f) {
            self->unk_19C = 0;
            self->actionFunc = func_80A4F734;
        }
    }
}

void func_80A4ED34(EnGs* self, GlobalContext* globalCtx) {
    static Color_RGBA8 flashRed = { 255, 50, 50, 0 };
    static Color_RGBA8 flashBlue = { 50, 50, 255, 0 };
    static Color_RGBA8 baseWhite = { 255, 255, 255, 0 };
    static Vec3f dustAccel = { 0.0f, -0.3f, 0.0f };
    static Color_RGBA8 dustPrim = { 200, 200, 200, 128 };
    static Color_RGBA8 dustEnv = { 100, 100, 100, 0 };
    static Vec3f bomb2Velocity = { 0.0f, 0.0f, 0.0f };
    static Vec3f bomb2Accel = { 0.0f, 0.0f, 0.0f };
    u8 i;
    Vec3f dustPos;
    Vec3f dustVelocity;
    Vec3f bomb2Pos;

    if (self->unk_19F == 0) {
        self->unk_200 = 40;
        self->unk_19F++;
    }

    if (self->unk_19F == 1) {
        if ((u32)self->unk_200-- <= 0) {
            self->unk_200 = 80;
            self->unk_19E |= 4;
            self->unk_19F++;
        }
    }

    if (self->unk_19F == 2) {
        self->unk_200--;
        Color_RGBA8_Copy(&self->flashColor, &baseWhite);
        if ((self->unk_200 < 80) && ((self->unk_200 % 20) < 8)) {
            if (self->unk_200 < 20) {
                Color_RGBA8_Copy(&self->flashColor, &flashRed);
                if ((self->unk_200 % 20) == 7) {
                    Audio_PlaySoundGeneral(NA_SE_SY_WARNING_COUNT_E, &D_801333D4, 4, &D_801333E0, &D_801333E0,
                                           &D_801333E8);
                }
            } else {
                Color_RGBA8_Copy(&self->flashColor, &flashBlue);
                if ((self->unk_200 % 20) == 7) {
                    Audio_PlaySoundGeneral(NA_SE_SY_WARNING_COUNT_N, &D_801333D4, 4, &D_801333E0, &D_801333E0,
                                           &D_801333E8);
                }
            }
        }
        if (self->unk_200 <= 0) {
            self->unk_200 = 0;
            self->unk_19E &= ~4;
            self->unk_19F++;
        }
    }

    if (self->unk_19F == 3) {
        for (i = 0; i < 3; i++) {
            dustVelocity.x = Rand_CenteredFloat(15.0f);
            dustVelocity.y = Rand_ZeroFloat(-1.0f);
            dustVelocity.z = Rand_CenteredFloat(15.0f);
            dustPos.x = self->actor.world.pos.x + (dustVelocity.x + dustVelocity.x);
            dustPos.y = self->actor.world.pos.y + 7.0f;
            dustPos.z = self->actor.world.pos.z + (dustVelocity.z + dustVelocity.z);
            func_8002836C(globalCtx, &dustPos, &dustVelocity, &dustAccel, &dustPrim, &dustEnv,
                          (s16)Rand_ZeroFloat(50.0f) + 200, 40, 15);
        }

        func_8002F974(&self->actor, NA_SE_EV_FIRE_PILLAR - SFX_FLAG);
        if (self->unk_200++ >= 40) {
            self->unk_19E |= 0x10;
            self->actor.flags |= 0x10;
            self->actor.uncullZoneForward = 12000.0f;

            self->actor.gravity = 0.3f;
            self->unk_19F++;
        }
    }

    if (self->unk_19F == 4) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 60.0f, 3);
        if (self->actor.bgCheckFlags & 0x18) {
            bomb2Pos.x = self->actor.world.pos.x;
            bomb2Pos.y = self->actor.world.pos.y;
            bomb2Pos.z = self->actor.world.pos.z;
            Audio_PlayActorSound2(&self->actor, NA_SE_IT_BOMB_EXPLOSION);
            EffectSsBomb2_SpawnLayered(globalCtx, &bomb2Pos, &bomb2Velocity, &bomb2Accel, 100, 20);
            self->unk_200 = 10;
            self->unk_19E |= 8;
            self->actionFunc = func_80A4F700;
        } else {
            func_8002F974(&self->actor, NA_SE_EV_STONE_LAUNCH - SFX_FLAG);
        }

        Actor_MoveForward(&self->actor);
        if (self->actor.yDistToPlayer < -12000.0f) {
            Actor_Kill(&self->actor);
        }
    }
}

void func_80A4F13C(EnGs* self, GlobalContext* globalCtx) {
    f32 tmpf1;
    f32 tmpf2;
    f32 tmpf3;
    s32 tmp;
    s16 tmp2 = self->unk_1A0[0].y;

    if (self->unk_19F == 0) {
        self->unk_1E8 = 0.0f;
        self->unk_1EC = 90.0f;
        self->unk_1F0 = 0.1f;
        self->unk_1F4 = 2.0f;
        self->unk_1F8 = 0;
        self->unk_19F = 1;
    }
    if (self->unk_19F == 1) {
        Math_SmoothStepToF(&self->unk_1F0, self->unk_1F4, 1.0f, 0.1f, 0.001f);
        tmpf1 = Math_SmoothStepToF(&self->unk_1E8, self->unk_1EC, 1.0f, self->unk_1F0, 0.001f);
        self->unk_1A0[0].y += (s32)(self->unk_1E8 * (0x10000 / 360.0f));
        if (tmpf1 == 0.0f) {
            self->unk_200 = 0;
            self->unk_19F = 2;
        }
    }
    if (self->unk_19F == 2) {
        self->unk_1A0[0].y += (s32)(self->unk_1E8 * (0x10000 / 360.0f));
        if (self->unk_200++ > 40) {
            self->unk_1E8 = self->unk_1B4[0].y - 1.0f;
            self->unk_1EC = 1.5f;
            self->unk_1F0 = self->unk_1B4[1].y - 1.0f;
            self->unk_1F4 = -0.3f;
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_STONE_GROW_UP);
            self->unk_19F = 3;
        }
    }
    if (self->unk_19F == 3) {
        self->unk_1A0[0].y += 0x4000;
        tmpf1 = Math_SmoothStepToF(&self->unk_1E8, self->unk_1EC, 0.8f, 0.2f, 0.001f);
        Math_SmoothStepToF(&self->unk_1F0, self->unk_1F4, 0.8f, 0.2f, 0.001f);
        self->unk_1B4[0].x = self->unk_1F0 + 1.0f;
        self->unk_1B4[0].y = self->unk_1E8 + 1.0f;
        if (tmpf1 == 0.0f) {
            self->unk_1E8 = 0.34906584f * 2.0f;
            self->unk_1EC = 0.34906584f; // M_PI / 9
            self->unk_19F = 4;
        }
    }
    if (self->unk_19F == 4) {
        tmpf1 = Math_SmoothStepToF(&self->unk_1E8, self->unk_1EC, 0.8f, 16384.0f, 3640.0f);
        self->unk_1A0[0].y += (s16)self->unk_1E8;
        if (tmpf1 == 0.0f) {

            tmp = self->unk_1A0[0].y;
            if (tmp > 0) {
                tmp += 0xFFFF0000;
            }
            self->unk_1E8 = tmp;

            self->unk_1EC = 0.0f;
            self->unk_19F = 5;
        }
    }
    if (self->unk_19F == 5) {
        tmp = self->unk_1A0[0].y;
        if (tmp > 0) {
            tmp += 0xFFFF0001;
        }
        self->unk_1E8 = tmp;
        tmpf1 = Math_SmoothStepToF(&self->unk_1E8, self->unk_1EC, 0.8f, 3640.0f, 0.001f);
        self->unk_1A0[0].y = self->unk_1E8;
        if (tmpf1 == 0.0f) {
            self->unk_1E8 = self->unk_1B4[0].y - 1.0f;
            self->unk_1EC = 0;
            self->unk_1A0[0].y = 0;
            self->unk_200 = 0;
            self->unk_1F0 = self->unk_1B4[0].x - 1.0f;
            self->unk_1F4 = 0;
            self->unk_1F8 = 0.5f;
            self->unk_1FC = 0;
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_STALKID_ATTACK);
            self->unk_19F = 6;
        }
    }
    if (self->unk_19F == 6) {
        tmpf1 = Math_SmoothStepToF(&self->unk_1E8, self->unk_1EC, 0.8f, 0.1f, 0.001f);
        tmpf2 = Math_SmoothStepToF(&self->unk_1F0, self->unk_1F4, 0.8f, 0.1f, 0.001f);
        tmpf3 = Math_SmoothStepToF(&self->unk_1F8, self->unk_1FC, 0.8f, 0.02f, 0.001f);
        self->unk_1B4[0].x = self->unk_1F0 + 1.0f;
        self->unk_1B4[0].y = self->unk_1E8 + 1.0f;
        self->unk_1B4[0].x += sinf((((self->unk_200 % 10) * 0.1f) * 360.0f) * (M_PI / 180.0f)) * self->unk_1F8;
        self->unk_1B4[0].y += sinf((((self->unk_200 % 10) * 0.1f) * 360.0f) * (M_PI / 180.0f)) * self->unk_1F8;
        self->unk_200++;
        if ((tmpf1 == 0.0f) && (tmpf2 == 0.0f) && (tmpf3 == 0.0f)) {
            self->unk_19C = 0;
            self->actionFunc = func_80A4F734;
        }
    }
    if ((u16)self->unk_1A0[0].y < (u16)tmp2) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_STONE_ROLLING);
    }
}

void func_80A4F700(EnGs* self, GlobalContext* globalCtx) {
    if (self->unk_200-- <= 0) {
        Actor_Kill(&self->actor);
    }
}

void func_80A4F734(EnGs* self, GlobalContext* globalCtx) {
    if (!Flags_GetSwitch(globalCtx, (self->actor.params >> 8) & 0x3F)) {
        func_80A4E470(self, globalCtx);
    }
}

void func_80A4F77C(EnGs* self) {
    static Vec3s D_80A4FE34 = { 0, 0, 0 };
    static Vec3f D_80A4FE3C = { 1.0f, 1.0f, 1.0f };
    s32 i;

    for (i = 0; i < 3; i++) {
        self->unk_1A0[i] = D_80A4FE34;
    }

    for (i = 0; i < 2; i++) {
        self->unk_1B4[i] = D_80A4FE3C;
    }

    self->unk_19C = 3;
}

void EnGs_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnGs* self = THIS;

    Actor_SetFocus(&self->actor, 23.0f);
    if (!(self->unk_19E & 0x10)) {
        if (self->collider.base.acFlags & AC_HIT) {
            self->unk_19F = 0;
            self->collider.base.acFlags &= ~AC_HIT;

            switch (self->actor.colChkInfo.damageEffect) {
                case 15:
                    self->unk_19E |= 1;
                    func_80A4F77C(self);
                    self->actionFunc = func_80A4E910;
                    break;
                case 14:
                    self->unk_19E |= 1;
                    func_80A4F77C(self);
                    self->actionFunc = func_80A4EA08;
                    break;
                case 13:
                    self->unk_19E |= 1;
                    func_80A4F77C(self);
                    self->actionFunc = func_80A4EB3C;
                    break;
                case 12:
                    self->unk_19E |= 2;
                    func_80A4F77C(self);
                    self->actionFunc = func_80A4ED34;
                    break;
                case 11:
                    self->unk_19E |= 1;
                    func_80A4F77C(self);
                    self->actionFunc = func_80A4F13C;
                    break;
            }
        }
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    self->actionFunc(self, globalCtx);
    func_80A4E648(self, globalCtx);
}

void EnGs_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnGs* self = THIS;
    s32 tmp;
    u32 frames;

    if (!(self->unk_19E & 8)) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_gs.c", 1046);

        frames = globalCtx->gameplayFrames;
        func_80093D18(globalCtx->state.gfxCtx);
        Matrix_Push();
        if (self->unk_19E & 1) {
            Matrix_RotateY(self->unk_1A0[0].y * (M_PI / 0x8000), MTXMODE_APPLY);
            Matrix_RotateX(self->unk_1A0[0].x * (M_PI / 0x8000), MTXMODE_APPLY);
            Matrix_RotateZ(self->unk_1A0[0].z * (M_PI / 0x8000), MTXMODE_APPLY);
            Matrix_Scale(self->unk_1B4[0].x, self->unk_1B4[0].y, self->unk_1B4[0].z, MTXMODE_APPLY);
            Matrix_RotateY(self->unk_1A0[1].y * (M_PI / 0x8000), MTXMODE_APPLY);
            Matrix_RotateX(self->unk_1A0[1].x * (M_PI / 0x8000), MTXMODE_APPLY);
            Matrix_RotateZ(self->unk_1A0[1].z * (M_PI / 0x8000), MTXMODE_APPLY);
        }

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_gs.c", 1064),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gGossipStoneSetupDL);

        if (self->unk_19E & 4) {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, self->flashColor.r, self->flashColor.g, self->flashColor.b,
                            self->flashColor.a);
        } else {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
        }

        gSPDisplayList(POLY_OPA_DISP++, gGossipStoneDL);
        gSPDisplayList(POLY_OPA_DISP++, gGossipStoneSquishedDL);

        Matrix_Pop();
        if (self->unk_19E & 2) {
            func_80093D84(globalCtx->state.gfxCtx);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(0.05f, -0.05f, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_gs.c", 1087),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(
                POLY_XLU_DISP++, 0x08,
                Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0, -frames * 0x14, 0x20, 0x80));
            gDPSetPrimColor(POLY_XLU_DISP++, 128, 128, 255, 255, 0, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
            gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
        }

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_gs.c", 1101);
    }
}
