#include "z_bg_spot18_basket.h"
#include "objects/object_spot18_obj/object_spot18_obj.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((BgSpot18Basket*)thisx)

void BgSpot18Basket_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Basket_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Basket_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot18Basket_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808B7BCC(BgSpot18Basket* self, GlobalContext* globalCtx);
void func_808B7AEC(BgSpot18Basket* self);
void func_808B7B58(BgSpot18Basket* self);
void func_808B7BB0(BgSpot18Basket* self);
void func_808B7D38(BgSpot18Basket* self);
void func_808B7F74(BgSpot18Basket* self);
void func_808B818C(BgSpot18Basket* self);
void func_808B7AFC(BgSpot18Basket* self, GlobalContext* globalCtx);
void func_808B7B6C(BgSpot18Basket* self, GlobalContext* globalCtx);
void func_808B7D50(BgSpot18Basket* self, GlobalContext* globalCtx);
void func_808B7FC0(BgSpot18Basket* self, GlobalContext* globalCtx);
void func_808B81A0(BgSpot18Basket* self, GlobalContext* globalCtx);

const ActorInit Bg_Spot18_Basket_InitVars = {
    ACTOR_BG_SPOT18_BASKET,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_SPOT18_OBJ,
    sizeof(BgSpot18Basket),
    (ActorFunc)BgSpot18Basket_Init,
    (ActorFunc)BgSpot18Basket_Destroy,
    (ActorFunc)BgSpot18Basket_Update,
    (ActorFunc)BgSpot18Basket_Draw,
};

static ColliderJntSphElementInit sJntSphElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_NONE,
            OCELEM_ON,
        },
        { 0, { { 0, 2040, 0 }, 54 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x00000008, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 1, { { 0, 1400, 0 }, 13 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_2,
        COLSHAPE_JNTSPH,
    },
    2,
    sJntSphElementsInit,
};

static s16 D_808B85C8[] = { 0x8000, 0x2AAA, 0xD555, 0x0000 };

void func_808B7710(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot18Basket* self = THIS;

    Collider_InitJntSph(globalCtx, &self->colliderJntSph);
    Collider_SetJntSph(globalCtx, &self->colliderJntSph, &self->dyna.actor, &sJntSphInit, self->ColliderJntSphElements);
    self->dyna.actor.colChkInfo.mass = MASS_IMMOVABLE;
}

void func_808B7770(BgSpot18Basket* self, GlobalContext* globalCtx, f32 arg2) {
    static s16 D_808B85D0 = 0;
    Vec3f acceleration;
    Vec3f velocity;
    Vec3f position;
    f32 cosValue;
    s32 i;
    f32 randomValue;
    f32 sinValue;
    s32 count;

    for (i = 0, count = 2; i != count; i++) {
        if (globalCtx) {}
        if (!(arg2 < Rand_ZeroOne())) {
            D_808B85D0 += 0x7530;

            sinValue = Math_SinS(D_808B85D0);
            cosValue = Math_CosS(D_808B85D0);

            randomValue = (Rand_ZeroOne() * 35.0f) + 35.0f;

            position.x = (randomValue * sinValue) + self->dyna.actor.world.pos.x;
            position.y = self->dyna.actor.world.pos.y + 10.0f;
            position.z = (randomValue * cosValue) + self->dyna.actor.world.pos.z;

            velocity.x = sinValue;
            velocity.y = 0.0f;
            velocity.z = cosValue;

            acceleration.x = 0.0f;
            acceleration.y = 0.5f;
            acceleration.z = 0.0f;

            func_800286CC(globalCtx, &position, &velocity, &acceleration, ((Rand_ZeroOne() * 16) + 80),
                          ((Rand_ZeroOne() * 30) + 80));
        }
    }
}

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgSpot18Basket_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgSpot18Basket* self = THIS;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&self->dyna, DPM_UNK3);
    func_808B7710(&self->dyna.actor, globalCtx);
    CollisionHeader_GetVirtual(&gGoronCityVaseCol, &colHeader);

    self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &self->dyna.actor, colHeader);

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    ActorShape_Init(&self->dyna.actor.shape, 0.0f, ActorShadow_DrawCircle, 15.0f);
    self->dyna.actor.home.pos.y += 0.01f;
    self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y;

    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
        func_808B7BB0(self);
        return;
    }

    func_808B7AEC(self);
    Actor_SpawnAsChild(&globalCtx->actorCtx, &self->dyna.actor, globalCtx, ACTOR_BG_SPOT18_FUTA,
                       self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z,
                       self->dyna.actor.shape.rot.x, self->dyna.actor.shape.rot.y + 0x1555,
                       self->dyna.actor.shape.rot.z, -1);

    if (self->dyna.actor.child == NULL) {
        osSyncPrintf(VT_FGCOL(RED));
        osSyncPrintf("Ｅｒｒｏｒ : 変化壷蓋発生失敗(%s %d)\n", "../z_bg_spot18_basket.c", 351);
        osSyncPrintf(VT_RST);
        Actor_Kill(&self->dyna.actor);
    }
}

void BgSpot18Basket_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot18Basket* self = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(globalCtx, &self->colliderJntSph);
}

void func_808B7AEC(BgSpot18Basket* self) {
    self->actionFunc = func_808B7AFC;
}

void func_808B7AFC(BgSpot18Basket* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (self->dyna.actor.params >> 8) & 0x3F)) {
        OnePointCutscene_Init(globalCtx, 4220, 80, &self->dyna.actor, MAIN_CAM);
        func_808B7B58(self);
    }
}

void func_808B7B58(BgSpot18Basket* self) {
    self->actionFunc = func_808B7B6C;
    self->unk_216 = 0;
}

void func_808B7B6C(BgSpot18Basket* self, GlobalContext* globalCtx) {
    if (self->unk_216 > 20) {
        func_808B7BB0(self);
        self->dyna.actor.child->parent = NULL;
        self->dyna.actor.child = NULL;
    }
}

void func_808B7BB0(BgSpot18Basket* self) {
    self->actionFunc = func_808B7BCC;
    self->unk_210 = self->unk_20C = 0;
}

void func_808B7BCC(BgSpot18Basket* self, GlobalContext* globalCtx) {
    f32 positionDiff;
    Actor* colliderBaseAc;

    Math_StepToS(&self->unk_210, 0x1F4, 0x1E);

    self->dyna.actor.shape.rot.y += self->unk_210;

    Math_StepToF(&self->unk_208, 50.0f, 1.5f);
    Math_StepToS(&self->unk_20C, 400, 15);

    self->unk_20E += self->unk_20C;

    self->dyna.actor.world.pos.x = (Math_SinS(self->unk_20E) * self->unk_208) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = (Math_CosS(self->unk_20E) * self->unk_208) + self->dyna.actor.home.pos.z;

    if (self->colliderJntSph.base.acFlags & AC_HIT) {
        colliderBaseAc = self->colliderJntSph.base.ac;

        if (colliderBaseAc != NULL) {
            positionDiff = colliderBaseAc->world.pos.y - self->dyna.actor.world.pos.y;

            if (positionDiff > 120.0f && positionDiff < 200.0f) {
                if (Math3D_Dist2DSq(colliderBaseAc->world.pos.z, self->colliderJntSph.base.ac->world.pos.x,
                                    self->dyna.actor.world.pos.z, self->dyna.actor.world.pos.x) < SQ(32.0f)) {
                    OnePointCutscene_Init(globalCtx, 4210, 240, &self->dyna.actor, MAIN_CAM);
                    func_808B7D38(self);
                    func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
                }
            }
        }
    }
    func_8002F974(&self->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
}

void func_808B7D38(BgSpot18Basket* self) {
    self->actionFunc = func_808B7D50;
    self->unk_216 = 0;
    self->unk_214 = 0;
}

void func_808B7D50(BgSpot18Basket* self, GlobalContext* globalCtx) {
    f32 tempValue2;
    f32 tempValue;

    if (self->unk_216 > 120) {
        Math_StepToS(&self->unk_210, 0x3E8, 0x32);
    } else {
        Math_StepToS(&self->unk_210, 0xBB8, 0x64);
    }

    self->dyna.actor.shape.rot.y = self->dyna.actor.shape.rot.y + self->unk_210;

    if (self->unk_216 < 70) {
        Math_StepToF(&self->unk_208, 100.0f, 2.0f);
    } else {
        Math_StepToF(&self->unk_208, 0.0f, 2.0f);
    }

    Math_StepToS(&self->unk_20C, 1000, 20);

    self->unk_20E += self->unk_20C;

    self->dyna.actor.world.pos.x = (Math_SinS(self->unk_20E) * self->unk_208) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = (Math_CosS(self->unk_20E) * self->unk_208) + self->dyna.actor.home.pos.z;

    self->unk_212 += 0xBB8;

    Math_StepToS(&self->unk_214, 0x5DC, 0x1E);

    self->dyna.actor.shape.rot.x = Math_CosS(self->unk_212) * self->unk_214;
    self->dyna.actor.shape.rot.z = -Math_SinS(self->unk_212) * self->unk_214;

    if (self->unk_216 > 140) {
        func_808B7F74(self);
    }

    if (self->unk_216 < 80) {
        func_808B7770(self, globalCtx, 1.0f);
    } else {
        func_808B7770(self, globalCtx, 0.8f);
    }

    tempValue2 = (self->unk_210 - 500) * 0.0006f;

    tempValue = CLAMP(tempValue2, 0.0f, 1.5f);

    func_800F436C(&self->dyna.actor.projectedPos, NA_SE_EV_WALL_MOVE_SP - SFX_FLAG, tempValue);
}

void func_808B7F74(BgSpot18Basket* self) {
    s16 shapeRotY;

    shapeRotY = self->dyna.actor.shape.rot.y;
    self->actionFunc = func_808B7FC0;

    if ((shapeRotY < -0x2E93) || (shapeRotY >= 0x7C19)) {
        self->unk_218 = 2;
    } else if (shapeRotY < 0x26C2) {
        self->unk_218 = 1;
    } else {
        self->unk_218 = 0;
    }

    self->unk_216 = 0;
}

void func_808B7FC0(BgSpot18Basket* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 tempUnk214;
    f32 tempUnk210;
    s16 arrayValue;
    f32 clampedTempUnk210;

    self->unk_212 += 0xBB8;

    if (self->unk_216 >= 13) {
        tempUnk214 = Math_StepToS(&self->unk_214, 0, 55);
    } else {
        tempUnk214 = 0;
    }

    self->dyna.actor.shape.rot.x = Math_CosS(self->unk_212) * self->unk_214;
    self->dyna.actor.shape.rot.z = -Math_SinS(self->unk_212) * self->unk_214;

    Math_StepToS(&self->unk_210, 0x1F4, 0xA);
    self->dyna.actor.shape.rot.y += self->unk_210;

    if (tempUnk214 != 0) {
        arrayValue = D_808B85C8[self->unk_218];

        if ((s16)(self->dyna.actor.shape.rot.y - arrayValue) >= 0) {
            self->dyna.actor.shape.rot.y = arrayValue;

            func_808B818C(self);
            func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
        }
    }

    if (self->unk_216 < 30) {
        func_808B7770(self, globalCtx, 0.5f);
    } else {
        func_808B7770(self, globalCtx, 0.3f);
    }

    tempUnk210 = (self->unk_210 - 500) * 0.0006f;

    clampedTempUnk210 = CLAMP(tempUnk210, 0.0f, 1.5f);

    func_800F436C(&self->dyna.actor.projectedPos, NA_SE_EV_WALL_MOVE_SP - SFX_FLAG, clampedTempUnk210);
}

void func_808B818C(BgSpot18Basket* self) {
    self->actionFunc = func_808B81A0;
    self->unk_216 = 0;
}

static s16 D_808B85E4[] = { -0x0FA0, 0x0320, 0x0FA0 };

void func_808B81A0(BgSpot18Basket* self, GlobalContext* globalCtx) {
    s32 i;
    Actor* actor = &self->dyna.actor;
    Vec3f tempVector;
    EnItem00* collectible;

    if (self->unk_216 == 1) {
        tempVector.x = actor->world.pos.x;
        tempVector.y = actor->world.pos.y + 170.0f;
        tempVector.z = actor->world.pos.z;

        if (self->unk_218 == 0) {
            for (i = 0; i < ARRAY_COUNT(D_808B85E4); i++) {
                collectible = Item_DropCollectible(globalCtx, &tempVector, ITEM00_BOMBS_A);
                if (collectible != NULL) {
                    collectible->actor.velocity.y = 11.0f;
                    collectible->actor.world.rot.y = D_808B85E4[i];
                }
            }
        } else if (self->unk_218 == 1) {
            for (i = 0; i < ARRAY_COUNT(D_808B85E4); i++) {
                collectible = Item_DropCollectible(globalCtx, &tempVector, ITEM00_RUPEE_GREEN);
                if (collectible != NULL) {
                    collectible->actor.velocity.y = 11.0f;
                    collectible->actor.world.rot.y = D_808B85E4[i];
                }
            }
        } else if (self->unk_218 == 2) {
            if ((self->unk_21A != 0) || Flags_GetCollectible(globalCtx, (actor->params & 0x3F))) {
                collectible = Item_DropCollectible(globalCtx, &tempVector, ITEM00_RUPEE_PURPLE);
                if (collectible != NULL) {
                    collectible->actor.velocity.y = 11.0f;
                    collectible->actor.world.rot.y = D_808B85E4[1];
                }
            } else {
                collectible =
                    Item_DropCollectible(globalCtx, &tempVector, ((actor->params & 0x3F) << 8) | ITEM00_HEART_PIECE);
                if (collectible != NULL) {
                    collectible->actor.velocity.y = 11.0f;
                    collectible->actor.world.rot.y = D_808B85E4[1];
                    self->unk_21A = 1;
                }
            }

            collectible = Item_DropCollectible(globalCtx, &tempVector, ITEM00_RUPEE_RED);
            if (collectible != NULL) {
                collectible->actor.velocity.y = 11.0f;
                collectible->actor.world.rot.y = D_808B85E4[0];
            }

            collectible = Item_DropCollectible(globalCtx, &tempVector, ITEM00_RUPEE_BLUE);
            if (collectible != NULL) {
                collectible->actor.velocity.y = 11.0f;
                collectible->actor.world.rot.y = D_808B85E4[2];
            }
        }
    } else if (self->unk_216 == 2) {
        if (self->unk_218 == 2) {
            func_80078884(NA_SE_SY_CORRECT_CHIME);
        } else {
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
        }
    } else if (self->unk_216 == 200) {
        func_808B7BB0(self);
    }
}

void BgSpot18Basket_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgSpot18Basket* self = THIS;
    s32 bgId;

    self->unk_216++;
    self->actionFunc(self, globalCtx);
    self->dyna.actor.floorHeight = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->dyna.actor.floorPoly, &bgId,
                                                               &self->dyna.actor, &self->dyna.actor.world.pos);
    if (self->actionFunc != func_808B7AFC) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
        if (self->actionFunc != func_808B7B6C) {
            self->colliderJntSph.base.acFlags &= ~AC_HIT;
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->colliderJntSph.base);
        }
    }
}

void BgSpot18Basket_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot18Basket* self = THIS;

    Collider_UpdateSpheres(0, &self->colliderJntSph);
    Collider_UpdateSpheres(1, &self->colliderJntSph);
    Gfx_DrawDListOpa(globalCtx, gGoronCityVaseDL);
}
