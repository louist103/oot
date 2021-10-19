/*
 * File: z_en_dodojr.c
 * Overlay: ovl_En_Dodojr
 * Description: Baby Dodongo
 */

#include "z_en_dodojr.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "objects/object_dodojr/object_dodojr.h"

#define FLAGS 0x00000005

#define THIS ((EnDodojr*)thisx)

void EnDodojr_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDodojr_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDodojr_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDodojr_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809F73AC(EnDodojr* self, GlobalContext* globalCtx);
void func_809F7BE4(EnDodojr* self, GlobalContext* globalCtx);
void func_809F74C4(EnDodojr* self, GlobalContext* globalCtx);
void func_809F758C(EnDodojr* self, GlobalContext* globalCtx);
void func_809F786C(EnDodojr* self, GlobalContext* globalCtx);
void func_809F799C(EnDodojr* self, GlobalContext* globalCtx);
void func_809F78EC(EnDodojr* self, GlobalContext* globalCtx);
void func_809F773C(EnDodojr* self, GlobalContext* globalCtx);
void func_809F77AC(EnDodojr* self, GlobalContext* globalCtx);
void func_809F784C(EnDodojr* self, GlobalContext* globalCtx);
void func_809F7AB8(EnDodojr* self, GlobalContext* globalCtx);
void func_809F7A00(EnDodojr* self, GlobalContext* globalCtx);
void func_809F7B3C(EnDodojr* self, GlobalContext* globalCtx);
void func_809F7C48(EnDodojr* self, GlobalContext* globalCtx);
void func_809F768C(EnDodojr* self, GlobalContext* globalCtx);

const ActorInit En_Dodojr_InitVars = {
    ACTOR_EN_DODOJR,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DODOJR,
    sizeof(EnDodojr),
    (ActorFunc)EnDodojr_Init,
    (ActorFunc)EnDodojr_Destroy,
    (ActorFunc)EnDodojr_Update,
    (ActorFunc)EnDodojr_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x08 },
        { 0xFFC5FFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 18, 20, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInit = { 1, 2, 25, 25, 0xFF };

void EnDodojr_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDodojr* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 18.0f);
    SkelAnime_Init(globalCtx, &self->skelAnime, &object_dodojr_Skel_0020E0, &object_dodojr_Anim_0009D4,
                   self->jointTable, self->morphTable, 15);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(4), &sColChkInit);

    self->actor.naviEnemyId = 0xE;
    self->actor.flags &= ~1;

    Actor_SetScale(&self->actor, 0.02f);

    self->actionFunc = func_809F73AC;
}

void EnDodojr_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDodojr* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_809F64D0(EnDodojr* self) {
    Audio_PlayActorSound2(&self->actor, NA_SE_IT_BOMB_EXPLOSION);
    Actor_SetColorFilter(&self->actor, 0x4000, 200, 0, 8);
}

void func_809F6510(EnDodojr* self, GlobalContext* globalCtx, s32 count) {
    Color_RGBA8 prim = { 170, 130, 90, 255 };
    Color_RGBA8 env = { 100, 60, 20, 0 };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };
    Vec3f pos;
    s16 angle = ((Rand_ZeroOne() - 0.5f) * 65536.0f);
    s32 i;

    pos.y = self->dustPos.y;

    for (i = count; i >= 0; i--, angle += (s16)(0x10000 / count)) {
        accel.x = (Rand_ZeroOne() - 0.5f) * 4.0f;
        accel.z = (Rand_ZeroOne() - 0.5f) * 4.0f;

        pos.x = (Math_SinS(angle) * 22.0f) + self->dustPos.x;
        pos.z = (Math_CosS(angle) * 22.0f) + self->dustPos.z;

        func_8002836C(globalCtx, &pos, &velocity, &accel, &prim, &env, 120, 40, 10);
    }
}

void func_809F6730(EnDodojr* self, GlobalContext* globalCtx, Vec3f* arg2) {
    Color_RGBA8 prim = { 170, 130, 90, 255 };
    Color_RGBA8 env = { 100, 60, 20, 0 };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };
    Vec3f pos;
    s16 angle = ((Rand_ZeroOne() - 0.5f) * 65536.0f);

    pos.y = self->actor.floorHeight;

    accel.x = (Rand_ZeroOne() - 0.5f) * 2;
    accel.z = (Rand_ZeroOne() - 0.5f) * 2;

    pos.x = (Math_SinS(angle) * 11.0f) + arg2->x;
    pos.z = (Math_CosS(angle) * 11.0f) + arg2->z;

    func_8002836C(globalCtx, &pos, &velocity, &accel, &prim, &env, 100, 60, 8);
}

s32 func_809F68B0(EnDodojr* self, GlobalContext* globalCtx) {
    if (self->actor.velocity.y >= 0.0f) {
        return 0;
    }

    if (self->unk_1FC == 0) {
        return 0;
    }

    if (self->actor.bgCheckFlags & 1) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_GND);
        self->dustPos = self->actor.world.pos;
        func_809F6510(self, globalCtx, 10);
        self->actor.velocity.y = 10.0f / (4 - self->unk_1FC);
        self->unk_1FC--;

        if (self->unk_1FC == 0) {
            self->actor.velocity.y = 0.0f;
            return 1;
        }
    }

    return 0;
}

void func_809F6994(EnDodojr* self) {
    f32 lastFrame = Animation_GetLastFrame(&object_dodojr_Anim_000860);

    Animation_Change(&self->skelAnime, &object_dodojr_Anim_000860, 1.8f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP, -10.0f);
    self->actor.velocity.y = 0.0f;
    self->actor.speedXZ = 2.6f;
    self->actor.gravity = -0.8f;
}

void func_809F6A20(EnDodojr* self) {
    f32 lastFrame = Animation_GetLastFrame(&object_dodojr_Anim_0004A0);

    Animation_Change(&self->skelAnime, &object_dodojr_Anim_0004A0, 1.0f, 0.0f, lastFrame, ANIMMODE_ONCE, -10.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.x = 0.0f;
    self->actor.velocity.z = 0.0f;
    self->actor.gravity = -0.8f;

    if (self->unk_1FC == 0) {
        self->unk_1FC = 3;
        self->actor.velocity.y = 10.0f;
    }
}

void func_809F6AC4(EnDodojr* self) {
    f32 lastFrame = Animation_GetLastFrame(&object_dodojr_Anim_0005F0);

    Animation_Change(&self->skelAnime, &object_dodojr_Anim_0005F0, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP, 0.0f);
    self->actor.velocity.y = 0.0f;
    self->actor.gravity = -0.8f;
}

void func_809F6B38(EnDodojr* self) {
    f32 lastFrame = Animation_GetLastFrame(&object_dodojr_Anim_000724);

    Animation_Change(&self->skelAnime, &object_dodojr_Anim_000724, 1.0f, 0.0f, lastFrame, ANIMMODE_LOOP, -10.0f);
    self->actor.gravity = -0.8f;
    self->unk_1FC = 3;
    self->actor.velocity.y = 10.0f;
}

void func_809F6BBC(EnDodojr* self) {
    self->actor.shape.shadowDraw = NULL;
    self->actor.flags &= ~1;
    self->actor.home.pos = self->actor.world.pos;
    self->actor.speedXZ = 0.0f;
    self->actor.gravity = -0.8f;
    self->timer3 = 30;
    self->dustPos = self->actor.world.pos;
}

void func_809F6C24(EnDodojr* self) {
    Animation_Change(&self->skelAnime, &object_dodojr_Anim_000724, 1.0f, 8.0f, 12.0f, ANIMMODE_ONCE, 0.0f);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_EAT);
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.x = 0.0f;
    self->actor.velocity.z = 0.0f;
    self->actor.gravity = -0.8f;
}

s32 func_809F6CA4(EnDodojr* self, GlobalContext* globalCtx) {
    Actor* bomb;
    Vec3f unkVec = { 99999.0f, 99999.0f, 99999.0f };
    s32 retVar = 0;
    f32 xDist;
    f32 yDist;
    f32 zDist;

    bomb = globalCtx->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].head;
    self->bomb = NULL;

    while (bomb != NULL) {
        if ((bomb->params != 0) || (bomb->parent != NULL) || (bomb->update == NULL)) {
            bomb = bomb->next;
            continue;
        }

        if (bomb->id != ACTOR_EN_BOM) {
            bomb = bomb->next;
            continue;
        }

        xDist = bomb->world.pos.x - self->actor.world.pos.x;
        yDist = bomb->world.pos.y - self->actor.world.pos.y;
        zDist = bomb->world.pos.z - self->actor.world.pos.z;

        if ((fabsf(xDist) >= fabsf(unkVec.x)) || (fabsf(yDist) >= fabsf(unkVec.y)) ||
            (fabsf(zDist) >= fabsf(unkVec.z))) {
            bomb = bomb->next;
            continue;
        }

        self->bomb = bomb;
        unkVec = bomb->world.pos;
        retVar = 1;
        bomb = bomb->next;
    }

    return retVar;
}

s32 func_809F6DD0(EnDodojr* self) {
    if (self->bomb == NULL) {
        return 0;
    } else if (self->bomb->parent != NULL) {
        return 0;
    } else if (Math_Vec3f_DistXYZ(&self->actor.world.pos, &self->bomb->world.pos) > 30.0f) {
        return 0;
    } else {
        self->bomb->parent = &self->actor;
        return 1;
    }
}

void func_809F6E54(EnDodojr* self, GlobalContext* globalCtx) {
    f32 angles[] = { 0.0f, 210.0f, 60.0f, 270.0f, 120.0f, 330.0f, 180.0f, 30.0f, 240.0f, 90.0f, 300.0f, 150.0f };
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f pos;
    s16 angleIndex;

    if ((self->bomb == NULL) || (self->bomb->update == NULL) ||
        ((self->bomb != NULL) && (self->bomb->parent != NULL))) {
        func_809F6CA4(self, globalCtx);
    }

    if (self->bomb != NULL) {
        pos = self->bomb->world.pos;
    } else {
        pos = player->actor.world.pos;
    }

    if (Math_Vec3f_DistXYZ(&self->actor.world.pos, &pos) > 80.0f) {
        angleIndex = (s16)(self->actor.home.pos.x + self->actor.home.pos.y + self->actor.home.pos.z +
                           globalCtx->state.frames / 30) %
                     12;
        angleIndex = ABS(angleIndex);
        pos.x += 80.0f * sinf(angles[angleIndex]);
        pos.z += 80.0f * cosf(angles[angleIndex]);
    }

    Math_SmoothStepToS(&self->actor.world.rot.y, Math_Vec3f_Yaw(&self->actor.world.pos, &pos), 10, 1000, 1);
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

s32 func_809F706C(EnDodojr* self) {
    if (self->actor.xzDistToPlayer > 40.0f) {
        return 0;
    } else {
        return 1;
    }
}

void func_809F709C(EnDodojr* self) {
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_DEAD);
    self->actor.flags &= ~1;
    func_809F6A20(self);
    self->actionFunc = func_809F7AB8;
}

s32 func_809F70E8(EnDodojr* self, GlobalContext* globalCtx) {
    if ((self->actionFunc == func_809F773C) || (self->actionFunc == func_809F77AC) ||
        (self->actionFunc == func_809F784C) || (self->actionFunc == func_809F7A00) ||
        (self->actionFunc == func_809F7AB8) || (self->actionFunc == func_809F7B3C) ||
        (self->actionFunc == func_809F7BE4)) {
        return 0;
    }

    if (globalCtx->actorCtx.unk_02 != 0) {
        if (self->actionFunc != func_809F73AC) {
            if (self->actionFunc == func_809F74C4) {
                self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
            }

            func_809F709C(self);
        }
        return 0;
    }

    if (!(self->collider.base.acFlags & 2)) {
        return 0;
    } else {
        self->collider.base.acFlags &= ~2;

        if ((self->actionFunc == func_809F73AC) || (self->actionFunc == func_809F74C4)) {
            self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        }

        if ((self->actor.colChkInfo.damageEffect == 0) && (self->actor.colChkInfo.damage != 0)) {
            Enemy_StartFinishingBlow(globalCtx, &self->actor);
            self->timer2 = 2;
            self->actionFunc = func_809F7C48;
            return 1;
        }

        if ((self->actor.colChkInfo.damageEffect == 1) && (self->actionFunc != func_809F78EC) &&
            (self->actionFunc != func_809F786C)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
            self->timer1 = 120;
            Actor_SetColorFilter(&self->actor, 0, 200, 0, 120);
            func_809F6A20(self);
            self->actionFunc = func_809F786C;
        }

        return 0;
    }
}

void func_809F72A4(EnDodojr* self, GlobalContext* globalCtx) {
    Collider_UpdateCylinder(&self->actor, &self->collider);

    if ((self->actionFunc != func_809F73AC) && (self->actionFunc != func_809F7BE4)) {
        if ((self->actionFunc == func_809F74C4) || (self->actionFunc == func_809F758C) ||
            (self->actionFunc == func_809F799C)) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }

        if ((self->actionFunc == func_809F74C4) || (self->actionFunc == func_809F758C) ||
            (self->actionFunc == func_809F786C) || (self->actionFunc == func_809F78EC) ||
            (self->actionFunc == func_809F799C)) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }

        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void func_809F73AC(EnDodojr* self, GlobalContext* globalCtx) {
    f32 lastFrame = Animation_GetLastFrame(&object_dodojr_Anim_000860);
    Player* player = GET_PLAYER(globalCtx);
    f32 dist;

    if (!(self->actor.xzDistToPlayer >= 320.0f)) {
        dist = self->actor.world.pos.y - player->actor.world.pos.y;

        if (!(dist >= 40.0f)) {
            Animation_Change(&self->skelAnime, &object_dodojr_Anim_000860, 1.8f, 0.0f, lastFrame, ANIMMODE_LOOP_INTERP,
                             -10.0f);
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_UP);
            self->actor.world.pos.y -= 60.0f;
            self->actor.flags |= 1;
            self->actor.world.rot.x -= 0x4000;
            self->actor.shape.rot.x = self->actor.world.rot.x;
            self->dustPos = self->actor.world.pos;
            self->dustPos.y = self->actor.floorHeight;
            self->actionFunc = func_809F74C4;
        }
    }
}

void func_809F74C4(EnDodojr* self, GlobalContext* globalCtx) {
    f32 sp2C;

    Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 4, 0x3E8, 0x64);
    sp2C = self->actor.shape.rot.x;
    sp2C /= 16384.0f;
    self->actor.world.pos.y = self->actor.home.pos.y + (60.0f * sp2C);
    func_809F6510(self, globalCtx, 3);

    if (sp2C == 0.0f) {
        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        self->actor.world.rot.x = self->actor.shape.rot.x;
        self->actor.speedXZ = 2.6f;
        self->actionFunc = func_809F758C;
    }
}

void func_809F758C(EnDodojr* self, GlobalContext* globalCtx) {
    func_8002D868(&self->actor);
    func_809F6730(self, globalCtx, &self->actor.world.pos);

    if (DECR(self->timer4) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_MOVE);
        self->timer4 = 5;
    }

    if (func_809F6DD0(self) != 0) {
        func_809F6C24(self);
        self->actionFunc = func_809F768C;
        return;
    }

    func_809F6E54(self, globalCtx);

    if (func_809F706C(self) != 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_CRY);
        func_809F6B38(self);
        self->actionFunc = func_809F799C;
    }

    if (self->actor.bgCheckFlags & 8) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_M_DOWN);
        func_809F6BBC(self);
        self->actionFunc = func_809F7A00;
    }
}

void func_809F768C(EnDodojr* self, GlobalContext* globalCtx) {
    EnBom* bomb;

    if (((s16)self->skelAnime.curFrame - 8) < 4) {
        bomb = (EnBom*)self->bomb;
        bomb->timer++;
        self->bomb->world.pos = self->headPos;
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_DODO_K_DRINK);
        Actor_Kill(self->bomb);
        self->timer3 = 24;
        self->unk_1FC = 0;
        self->actionFunc = func_809F773C;
    }
}

void func_809F773C(EnDodojr* self, GlobalContext* globalCtx) {
    if (DECR(self->timer3) == 0) {
        func_809F64D0(self);
        self->actor.flags &= ~1;
        func_809F6A20(self);
        self->actionFunc = func_809F77AC;
    }
}

void func_809F77AC(EnDodojr* self, GlobalContext* globalCtx) {
    self->rootScale = 1.2f;
    self->rootScale *= ((f32)self->actor.colorFilterTimer / 8);
    func_8002D868(&self->actor);

    if (func_809F68B0(self, globalCtx) != 0) {
        self->timer3 = 60;
        func_809F6AC4(self);
        self->unk_1FC = 7;
        self->actionFunc = func_809F784C;
    }
}

void func_809F784C(EnDodojr* self, GlobalContext* globalCtx) {
    func_809F7B3C(self, globalCtx);
}

void func_809F786C(EnDodojr* self, GlobalContext* globalCtx) {
    func_8002D868(&self->actor);

    if (func_809F68B0(self, globalCtx) != 0) {
        func_809F6AC4(self);
        self->actionFunc = func_809F78EC;
    }

    Math_SmoothStepToS(&self->actor.shape.rot.y, 0, 4, 1000, 10);
    self->actor.world.rot.x = self->actor.shape.rot.x;
    self->actor.colorFilterTimer = self->timer1;
}

void func_809F78EC(EnDodojr* self, GlobalContext* globalCtx) {
    if (DECR(self->timer1) != 0) {
        if (self->timer1 < 30) {
            if ((self->timer1 & 1) != 0) {
                self->actor.world.pos.x += 1.5f;
                self->actor.world.pos.z += 1.5f;
            } else {
                self->actor.world.pos.x -= 1.5f;
                self->actor.world.pos.z -= 1.5f;
            }

            return;
        }
    } else {
        func_809F6994(self);
        self->actionFunc = func_809F758C;
    }
}

void func_809F799C(EnDodojr* self, GlobalContext* globalCtx) {
    self->actor.flags |= 0x1000000;
    func_8002D868(&self->actor);

    if (func_809F68B0(self, globalCtx) != 0) {
        func_809F6994(self);
        self->actionFunc = func_809F758C;
    }
}

void func_809F7A00(EnDodojr* self, GlobalContext* globalCtx) {
    f32 tmp;

    Math_SmoothStepToS(&self->actor.shape.rot.x, 0x4000, 4, 1000, 100);

    if (DECR(self->timer3) != 0) {
        tmp = (30 - self->timer3) / 30.0f;
        self->actor.world.pos.y = self->actor.home.pos.y - (60.0f * tmp);
    } else {
        Actor_Kill(&self->actor);
    }

    func_809F6510(self, globalCtx, 3);
}

void func_809F7AB8(EnDodojr* self, GlobalContext* globalCtx) {
    func_8002D868(&self->actor);
    Math_SmoothStepToS(&self->actor.shape.rot.y, 0, 4, 1000, 10);
    self->actor.world.rot.x = self->actor.shape.rot.x;

    if (func_809F68B0(self, globalCtx) != 0) {
        self->timer3 = 60;
        func_809F6AC4(self);
        self->unk_1FC = 7;
        self->actionFunc = func_809F7B3C;
    }
}

void func_809F7B3C(EnDodojr* self, GlobalContext* globalCtx) {
    EnBom* bomb;

    if (self->unk_1FC != 0) {
        if (self->actor.colorFilterTimer == 0) {
            Actor_SetColorFilter(&self->actor, 0x4000, 200, 0, self->unk_1FC);
            self->unk_1FC--;
        }
    } else {
        bomb = (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, self->actor.world.pos.x,
                                   self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, BOMB_BODY);

        if (bomb != NULL) {
            bomb->timer = 0;
        }

        self->timer3 = 8;
        self->actionFunc = func_809F7BE4;
    }
}

void func_809F7BE4(EnDodojr* self, GlobalContext* globalCtx) {
    if (DECR(self->timer3) == 0) {
        Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, 0x40);
        Actor_Kill(&self->actor);
    }
}

void func_809F7C48(EnDodojr* self, GlobalContext* globalCtx) {
    if (DECR(self->timer2) == 0) {
        func_809F709C(self);
    }
}

void EnDodojr_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDodojr* self = THIS;

    SkelAnime_Update(&self->skelAnime);
    Actor_MoveForward(&self->actor);
    func_809F70E8(self, globalCtx);

    if (self->actionFunc != func_809F73AC) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, self->collider.dim.radius, self->collider.dim.height, 0.0f, 5);
    }

    self->actionFunc(self, globalCtx);
    Actor_SetFocus(&self->actor, 10.0f);
    func_809F72A4(self, globalCtx);
}

s32 func_809F7D50(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnDodojr* self = THIS;
    Vec3f D_809F7F64 = { 480.0f, 620.0f, 0.0f };

    if (limbIndex == 1) {
        Matrix_Scale((self->rootScale * 0.5f) + 1.0f, self->rootScale + 1.0f, (self->rootScale * 0.5f) + 1.0f,
                     MTXMODE_APPLY);
    }

    if (limbIndex == 4) {
        Matrix_MultVec3f(&D_809F7F64, &self->headPos);
    }

    return false;
}

void func_809F7DFC(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
}

void EnDodojr_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDodojr* self = THIS;

    if ((self->actionFunc != func_809F73AC) && (self->actionFunc != func_809F7BE4)) {
        func_80093D18(globalCtx->state.gfxCtx);
        SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, func_809F7D50, func_809F7DFC,
                          &self->actor);
    }
}
