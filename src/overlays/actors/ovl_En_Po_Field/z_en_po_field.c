/*
 * File: z_en_po_field.c
 * Overlay: ovl_En_Po_Field
 * Description: Field Poe
 */

#include "z_en_po_field.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_po_field/object_po_field.h"

#define FLAGS 0x00001035

#define THIS ((EnPoField*)thisx)

void EnPoField_Init(Actor* thisx, GlobalContext* globalCtx);
void EnPoField_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnPoField_Update(Actor* thisx, GlobalContext* globalCtx);
void EnPoField_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnPoField_UpdateDead(Actor* thisx, GlobalContext* globalCtx);
void EnPoField_DrawSoul(Actor* thisx, GlobalContext* globalCtx);

void EnPoField_SetupWaitForSpawn(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_WaitForSpawn(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_Appear(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_CirclePlayer(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_Damage(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_Flee(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_Death(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_Disappear(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_SoulIdle(EnPoField* self, GlobalContext* globalCtx);
void func_80AD587C(EnPoField* self, GlobalContext* globalCtx);
void func_80AD58D4(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_SoulDisappear(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_SoulInteract(EnPoField* self, GlobalContext* globalCtx);
void EnPoField_SpawnFlame(EnPoField* self);

const ActorInit En_Po_Field_InitVars = {
    ACTOR_EN_PO_FIELD,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_PO_FIELD,
    sizeof(EnPoField),
    (ActorFunc)EnPoField_Init,
    (ActorFunc)EnPoField_Destroy,
    (ActorFunc)EnPoField_Update,
    (ActorFunc)EnPoField_Draw,
};

static ColliderCylinderInit D_80AD7080 = {
    {
        COLTYPE_HIT3,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
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
    { 25, 50, 20, { 0, 0, 0 } },
};

static ColliderCylinderInit D_80AD70AC = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x01, 0x04 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { 10, 30, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit D_80AD70D8 = { 4, 25, 50, 40 };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(2, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(2, 0x0),
    /* Boomerang     */ DMG_ENTRY(1, 0x1),
    /* Normal arrow  */ DMG_ENTRY(2, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(2, 0x1),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(2, 0x0),
    /* Giant's Knife */ DMG_ENTRY(4, 0x0),
    /* Fire arrow    */ DMG_ENTRY(2, 0x0),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0x0),
    /* Master spin   */ DMG_ENTRY(2, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0x0),
    /* Master jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static s32 sNumSpawned = 0;

static Vec3f sFieldMiddle = { -1000.0f, 0.0f, 6500.0f };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 3200, ICHAIN_STOP),
};

static Vec3f D_80AD7114 = { 0.0f, 3.0f, 0.0f };

static Vec3f D_80AD7120 = { 0.0f, 0.0f, 0.0f };

static EnPoFieldInfo sPoFieldInfo[2] = {
    { { 255, 170, 255 }, { 100, 0, 150 }, { 255, 85, 0 }, 248, gPoeFieldSoulTex },
    { { 255, 255, 170 }, { 255, 200, 0 }, { 160, 0, 255 }, 241, gBigPoeSoulTex },
};

static Vec3f D_80AD714C = { 0.0f, 1400.0f, 0.0f };

static Vec3s sSpawnPositions[10];
static u8 sSpawnSwitchFlags[10];
static MtxF sLimb7Mtx;

void EnPoField_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnPoField* self = THIS;
    s32 pad;

    if (sNumSpawned != 10) {
        sSpawnPositions[sNumSpawned].x = self->actor.world.pos.x;
        sSpawnPositions[sNumSpawned].y = self->actor.world.pos.y;
        sSpawnPositions[sNumSpawned].z = self->actor.world.pos.z;
        sSpawnSwitchFlags[sNumSpawned] = self->actor.params & 0xFF;
        sNumSpawned++;
    }
    if (sNumSpawned >= 2) {
        self->actor.params = 0xFF;
        Actor_Kill(&self->actor);
        return;
    }
    Actor_ProcessInitChain(&self->actor, sInitChain);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gPoeFieldSkel, &gPoeFieldFloatAnim, self->jointTable, self->morphTable,
                   10);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &D_80AD7080);
    Collider_InitCylinder(globalCtx, &self->flameCollider);
    Collider_SetCylinder(globalCtx, &self->flameCollider, &self->actor, &D_80AD70AC);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, &sDamageTable, &D_80AD70D8);
    self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);
    Lights_PointGlowSetInfo(&self->lightInfo, self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z,
                            255, 255, 255, 0);
    self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
    EnPoField_SetupWaitForSpawn(self, globalCtx);
}

void EnPoField_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnPoField* self = THIS;

    if (self->actor.params != 0xFF) {
        LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
        Collider_DestroyCylinder(globalCtx, &self->flameCollider);
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void EnPoField_SetupWaitForSpawn(EnPoField* self, GlobalContext* globalCtx) {
    self->actor.update = EnPoField_Update;
    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_ENEMY);
    self->actor.shape.rot.x = 0;
    Lights_PointSetColorAndRadius(&self->lightInfo, 0, 0, 0, 0);
    self->actionTimer = 200;
    Actor_SetScale(&self->actor, 0.0f);
    self->actor.flags &= ~0x00010001;
    self->collider.base.acFlags &= ~AC_ON;
    self->collider.base.ocFlags1 = OC1_ON | OC1_TYPE_ALL;
    self->actor.colChkInfo.health = D_80AD70D8.health;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actionFunc = EnPoField_WaitForSpawn;
}

void EnPoField_SetupAppear(EnPoField* self) {
    Animation_PlayOnce(&self->skelAnime, &gPoeFieldAppearAnim);
    self->actor.draw = EnPoField_Draw;
    self->lightColor.r = 255;
    self->lightColor.g = 255;
    self->lightColor.b = 210;
    self->lightColor.a = 0;
    self->actor.shape.shadowAlpha = 0;
    self->actor.shape.yOffset = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_APPEAR);
    self->actor.home.pos.y = self->actor.world.pos.y;
    if (self->actor.params == EN_PO_FIELD_BIG) {
        self->actor.speedXZ = 12.0f;
        self->collider.dim.radius = 35;
        self->collider.dim.height = 100;
        self->collider.dim.yShift = 10;
        self->actor.shape.shadowScale = 45.0f;
        self->scaleModifier = 0.014f;
        self->actor.naviEnemyId = 0x5A;
    } else {
        self->actor.speedXZ = 0.0f;
        self->collider.dim.radius = D_80AD7080.dim.radius;
        self->collider.dim.height = D_80AD7080.dim.height;
        self->collider.dim.yShift = D_80AD7080.dim.yShift;
        self->actor.shape.shadowScale = 37.0f;
        self->scaleModifier = 0.01f;
        self->actor.naviEnemyId = 0x5C;
    }
    self->actionFunc = EnPoField_Appear;
}

void EnPoField_SetupCirclePlayer(EnPoField* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    Animation_PlayLoop(&self->skelAnime, &gPoeFieldFloatAnim);
    self->collider.base.acFlags |= AC_ON;
    self->scaleModifier = self->actor.xzDistToPlayer;
    Math_Vec3f_Copy(&self->actor.home.pos, &player->actor.world.pos);
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    if (self->actionFunc != EnPoField_Damage) {
        self->actor.flags |= 1;
        self->actionTimer = 600;
        self->unk_194 = 32;
    }
    self->actionFunc = EnPoField_CirclePlayer;
}

void EnPoField_SetupFlee(EnPoField* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeFieldFleeAnim, -5.0f);
    self->collider.base.acFlags |= AC_ON;
    self->actionFunc = EnPoField_Flee;
    self->actor.speedXZ = 12.0f;
    if (self->actionFunc != EnPoField_Damage) {
        self->actor.flags |= 1;
        self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
        self->actionTimer = 2000;
        self->unk_194 = 32;
    }
}

void EnPoField_SetupDamage(EnPoField* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gPoeFieldDamagedAnim, -6.0f);
    if (self->collider.info.acHitInfo->toucher.dmgFlags & 0x1F824) {
        self->actor.world.rot.y = self->collider.base.ac->world.rot.y;
    } else {
        self->actor.world.rot.y = Actor_WorldYawTowardActor(&self->actor, self->collider.base.ac) + 0x8000;
    }
    self->collider.base.acFlags &= ~(AC_HIT | AC_ON);
    self->actor.speedXZ = 5.0f;
    Actor_SetColorFilter(&self->actor, 0x4000, 255, 0, 16);
    self->actionFunc = EnPoField_Damage;
}

void EnPoField_SetupDeath(EnPoField* self) {
    self->actionTimer = 0;
    self->actor.flags &= -2;
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actor.naviEnemyId = 0xFF;
    if (self->flameTimer >= 20) {
        self->flameTimer = 19;
    }
    self->actionFunc = EnPoField_Death;
}

void EnPoField_SetupDisappear(EnPoField* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeFieldDisappearAnim, -6.0f);
    self->actionTimer = 16;
    self->collider.base.acFlags &= ~(AC_HIT | AC_ON);
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DISAPPEAR);
    self->actionFunc = EnPoField_Disappear;
}

void EnPoField_SetupSoulIdle(EnPoField* self, GlobalContext* globalCtx) {
    self->actor.update = EnPoField_UpdateDead;
    self->actor.draw = EnPoField_DrawSoul;
    self->actor.shape.shadowDraw = NULL;
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.gravity = -1.0f;
    self->actor.shape.yOffset = 1500.0f;
    self->actor.shape.rot.x = -0x8000;
    self->actionTimer = 60;
    self->actor.world.pos.y -= 15.0f;
    Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_MISC);
    self->actionFunc = EnPoField_SoulIdle;
}

void func_80AD42B0(EnPoField* self) {
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, 0, 0, 0, 0);
    self->actor.shape.rot.y = 0;
    self->lightColor.a = 0;
    self->actor.shape.rot.x = 0;
    self->actor.shape.yOffset = 0.0f;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0.0f;
    self->actor.home.pos.y = self->actor.world.pos.y;
    self->actor.scale.x = 0.0f;
    self->actor.scale.y = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_METAL_BOX_BOUND);
    if (self->actor.params == EN_PO_FIELD_BIG) {
        func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
    }
    self->actionFunc = func_80AD587C;
}

void func_80AD4384(EnPoField* self) {
    self->actor.home.pos.y = self->actor.world.pos.y;
    Actor_SetFocus(&self->actor, -10.0f);
    self->collider.dim.radius = 13;
    self->collider.dim.height = 30;
    self->collider.dim.yShift = 0;
    self->collider.dim.pos.x = self->actor.world.pos.x;
    self->collider.dim.pos.y = self->actor.world.pos.y - 20.0f;
    self->collider.dim.pos.z = self->actor.world.pos.z;
    self->collider.base.ocFlags1 = OC1_ON | OC1_TYPE_PLAYER;
    self->actor.textId = 0x5005;
    self->actionTimer = 400;
    self->unk_194 = 32;
    self->actor.flags |= 1;
    self->actionFunc = func_80AD58D4;
}

void EnPoField_SetupSoulDisappear(EnPoField* self) {
    self->actionFunc = EnPoField_SoulDisappear;
}

void EnPoField_SetupInteractWithSoul(EnPoField* self) {
    self->actionFunc = EnPoField_SoulInteract;
    self->actor.home.pos.y = self->actor.world.pos.y - 15.0f;
}

void EnPoField_CorrectYPos(EnPoField* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->unk_194 == 0) {
        self->unk_194 = 32;
    }
    if (self->unk_194 != 0) {
        self->unk_194 -= 1;
    }
    if (self->actor.floorHeight == BGCHECK_Y_MIN) {
        EnPoField_SetupDisappear(self);
        return;
    }
    Math_ApproachF(
        &self->actor.home.pos.y,
        ((player->actor.world.pos.y > self->actor.floorHeight) ? player->actor.world.pos.y : self->actor.floorHeight) +
            13.0f,
        0.2f, 5.0f);
    self->actor.world.pos.y = Math_SinS(self->unk_194 * 0x800) * 13.0f + self->actor.home.pos.y;
}

f32 EnPoField_SetFleeSpeed(EnPoField* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 speed = ((player->stateFlags1 & 0x800000) && player->rideActor != NULL) ? player->rideActor->speedXZ : 12.0f;

    if (self->actor.xzDistToPlayer < 300.0f) {
        self->actor.speedXZ = speed * 1.5f + 2.0f;
    } else if (self->actor.xzDistToPlayer < 400.0f) {
        self->actor.speedXZ = speed * 1.25f + 2.0f;
    } else if (self->actor.xzDistToPlayer < 500.0f) {
        self->actor.speedXZ = speed + 2.0f;
    } else {
        self->actor.speedXZ = 12.0f;
    }
    self->actor.speedXZ = CLAMP_MIN(self->actor.speedXZ, 12.0f);
#ifdef AVOID_UB
    return 0.0f; // to silence warning, unused everywhere it seems
#endif
}

void EnPoField_WaitForSpawn(EnPoField* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 spawnDist;
    s32 i;
    s32 bgId;

    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (self->actionTimer == 0) {
        for (i = 0; i < sNumSpawned; i++) {
            if (fabsf(sSpawnPositions[i].x - player->actor.world.pos.x) < 150.0f &&
                fabsf(sSpawnPositions[i].z - player->actor.world.pos.z) < 150.0f) {
                if (Flags_GetSwitch(globalCtx, sSpawnSwitchFlags[i])) {
                    if (player->stateFlags1 & 0x800000) { // Player riding Epona
                        return;
                    } else {
                        self->actor.params = EN_PO_FIELD_SMALL;
                        spawnDist = 300.0f;
                    }
                } else if (player->stateFlags1 & 0x800000 || Rand_ZeroOne() < 0.4f) {
                    self->actor.params = EN_PO_FIELD_BIG;
                    self->spawnFlagIndex = i;
                    spawnDist = 480.0f;
                } else {
                    self->actor.params = EN_PO_FIELD_SMALL;
                    spawnDist = 300.0f;
                }
                self->actor.world.pos.x = Math_SinS(player->actor.shape.rot.y) * spawnDist + player->actor.world.pos.x;
                self->actor.world.pos.z = Math_CosS(player->actor.shape.rot.y) * spawnDist + player->actor.world.pos.z;
                self->actor.world.pos.y = player->actor.world.pos.y + 1000.0f;
                self->actor.world.pos.y = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &self->actor.floorPoly, &bgId,
                                                                      &self->actor, &self->actor.world.pos);
                if (self->actor.world.pos.y != BGCHECK_Y_MIN) {
                    self->actor.shape.rot.y = Actor_WorldYawTowardActor(&self->actor, &player->actor);
                    EnPoField_SetupAppear(self);
                } else {
                    return;
                }
            }
        }
    }
}

void EnPoField_Appear(EnPoField* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->lightColor.a = 255;
        Actor_SetScale(&self->actor, self->scaleModifier);
        if (self->actor.params == EN_PO_FIELD_BIG) {
            EnPoField_SetupFlee(self);
        } else {
            EnPoField_SetupCirclePlayer(self, globalCtx);
        }
    } else if (self->skelAnime.curFrame > 10.0f) {
        self->lightColor.a = ((self->skelAnime.curFrame - 10.0f) * 0.05f) * 255.0f;
    } else {
        self->actor.scale.x += self->scaleModifier * 0.1f;
        self->actor.scale.y = self->actor.scale.x;
        self->actor.scale.z = self->actor.scale.x;
    }
    self->actor.shape.shadowAlpha = self->lightColor.a;
    self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
    if (self->actor.params == EN_PO_FIELD_BIG) {
        self->actor.world.rot.y = self->actor.yawTowardsPlayer + 0x8000;
        EnPoField_SetFleeSpeed(self, globalCtx);
    }
}

void EnPoField_CirclePlayer(EnPoField* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 temp_v1 = 16 - self->unk_194;

    SkelAnime_Update(&self->skelAnime);
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (ABS(temp_v1) < 16) {
        self->actor.world.rot.y += 512.0f * fabsf(Math_SinS(self->unk_194 * 0x800));
    }
    Math_ApproachF(&self->scaleModifier, 180.0f, 0.5f, 10.0f);
    Math_ApproachF(&self->actor.home.pos.x, player->actor.world.pos.x, 0.2f, 6.0f);
    Math_ApproachF(&self->actor.home.pos.z, player->actor.world.pos.z, 0.2f, 6.0f);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.world.rot.y, 1, 0x800, 0x200);
    if (self->actor.home.pos.x - player->actor.world.pos.x > 100.0f) {
        self->actor.home.pos.x = player->actor.world.pos.x + 100.0f;
    } else if (self->actor.home.pos.x - player->actor.world.pos.x < -100.0f) {
        self->actor.home.pos.x = player->actor.world.pos.x + -100.0f;
    }
    if (self->actor.home.pos.z - player->actor.world.pos.z > 100.0f) {
        self->actor.home.pos.z = player->actor.world.pos.z + 100.0f;
    } else if (self->actor.home.pos.z - player->actor.world.pos.z < -100.0f) {
        self->actor.home.pos.z = player->actor.world.pos.z + -100.0f;
    }
    self->actor.world.pos.x = self->actor.home.pos.x - (Math_SinS(self->actor.world.rot.y) * self->scaleModifier);
    self->actor.world.pos.z = self->actor.home.pos.z - (Math_CosS(self->actor.world.rot.y) * self->scaleModifier);
    if (self->actionTimer == 0) {
        EnPoField_SetupDisappear(self);
    } else {
        EnPoField_SpawnFlame(self);
    }
    EnPoField_CorrectYPos(self, globalCtx);
    func_8002F974(&self->actor, NA_SE_EN_PO_FLY - SFX_FLAG);
}

void EnPoField_Flee(EnPoField* self, GlobalContext* globalCtx) {
    f32 temp_f6;
    s16 phi_t0;

    SkelAnime_Update(&self->skelAnime);
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (Actor_WorldDistXZToPoint(&self->actor, &sFieldMiddle) > 3000.0f) {
        phi_t0 = (s16)(self->actor.yawTowardsPlayer - Actor_WorldYawTowardPoint(&self->actor, &sFieldMiddle) - 0x8000) *
                 0.2f;
    } else {
        phi_t0 = 0;
    }
    Math_ApproachS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer - phi_t0, 6, 0x400);
    EnPoField_SetFleeSpeed(self, globalCtx);
    self->actor.world.rot.y = self->actor.shape.rot.y + 0x8000;
    temp_f6 = Math_SinS(self->actionTimer * 0x800) * 3.0f;
    self->actor.world.pos.x -= temp_f6 * Math_CosS(self->actor.shape.rot.y);
    self->actor.world.pos.z += temp_f6 * Math_SinS(self->actor.shape.rot.y);
    if (self->actionTimer == 0 || self->actor.xzDistToPlayer > 1500.0f) {
        EnPoField_SetupDisappear(self);
    } else {
        EnPoField_CorrectYPos(self, globalCtx);
    }
    func_8002F974(&self->actor, NA_SE_EN_PO_AWAY - SFX_FLAG);
}

void EnPoField_Damage(EnPoField* self, GlobalContext* globalCtx) {
    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.5f);
    if (SkelAnime_Update(&self->skelAnime)) {
        if (self->actor.colChkInfo.health == 0) {
            EnPoField_SetupDeath(self);
        } else if (self->actor.params == EN_PO_FIELD_BIG) {
            EnPoField_SetupFlee(self);
        } else {
            EnPoField_SetupCirclePlayer(self, globalCtx);
        }
    }
}

void EnPoField_Death(EnPoField* self, GlobalContext* globalCtx) {
    Vec3f sp6C;
    f32 sp68;
    s32 pad;
    s32 pad1;
    f32 temp_f0;

    self->actionTimer++;
    if (self->actionTimer < 8) {
        if (self->actionTimer < 5) {
            sp6C.y = Math_SinS(self->actionTimer * 0x1000 - 0x4000) * 23.0f + (self->actor.world.pos.y + 40.0f);
            sp68 = Math_CosS(self->actionTimer * 0x1000 - 0x4000) * 23.0f;
            sp6C.x =
                Math_SinS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4800) * sp68 + self->actor.world.pos.x;
            sp6C.z =
                Math_CosS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4800) * sp68 + self->actor.world.pos.z;
        } else {
            sp6C.y = self->actor.world.pos.y + 40.0f + 15.0f * (self->actionTimer - 5);
            sp6C.x =
                Math_SinS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4800) * 23.0f + self->actor.world.pos.x;
            sp6C.z =
                Math_CosS(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x4800) * 23.0f + self->actor.world.pos.z;
        }
        EffectSsDeadDb_Spawn(globalCtx, &sp6C, &D_80AD7114, &D_80AD7120, self->actionTimer * 10 + 80, 0, 255, 255, 255,
                             255, 0, 0, 255, 1, 9, 1);
        sp6C.x = (self->actor.world.pos.x + self->actor.world.pos.x) - sp6C.x;
        sp6C.z = (self->actor.world.pos.z + self->actor.world.pos.z) - sp6C.z;
        EffectSsDeadDb_Spawn(globalCtx, &sp6C, &D_80AD7114, &D_80AD7120, self->actionTimer * 10 + 80, 0, 255, 255, 255,
                             255, 0, 0, 255, 1, 9, 1);
        sp6C.x = self->actor.world.pos.x;
        sp6C.z = self->actor.world.pos.z;
        EffectSsDeadDb_Spawn(globalCtx, &sp6C, &D_80AD7114, &D_80AD7120, self->actionTimer * 10 + 80, 0, 255, 255, 255,
                             255, 0, 0, 255, 1, 9, 1);
        if (self->actionTimer == 1) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_EXTINCT);
        }
    } else if (self->actionTimer == 28) {
        EnPoField_SetupSoulIdle(self, globalCtx);
    } else if (self->actionTimer >= 19) {
        temp_f0 = (28 - self->actionTimer) * 0.001f;
        self->actor.world.pos.y += 5.0f;
        self->actor.scale.z = temp_f0;
        self->actor.scale.y = temp_f0;
        self->actor.scale.x = temp_f0;
    }
    if (self->actionTimer == 18) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DEAD2);
    }
}

void EnPoField_Disappear(EnPoField* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    self->actor.shape.rot.y += 0x1000;
    self->lightColor.a = self->actionTimer * 15.9375f;
    self->actor.shape.shadowAlpha = self->lightColor.a;
    if (self->actionTimer == 0) {
        EnPoField_SetupWaitForSpawn(self, globalCtx);
    }
}

void EnPoField_SoulIdle(EnPoField* self, GlobalContext* globalCtx) {
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (self->actor.bgCheckFlags & 1) {
        EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 6.0f, 0, 1, 1, 15, OBJECT_PO_FIELD, 10,
                                 gPoeFieldLanternDL);
        func_80AD42B0(self);
    } else if (self->actionTimer == 0) {
        EnPoField_SetupWaitForSpawn(self, globalCtx);
    }
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 10.0f, 4);
}

void EnPoField_SoulUpdateProperties(EnPoField* self, s32 arg1) {
    EnPoFieldInfo* info = &sPoFieldInfo[self->actor.params];
    f32 multiplier;

    self->lightColor.a = CLAMP(self->lightColor.a + arg1, 0, 255);
    if (arg1 < 0) {
        multiplier = self->lightColor.a * (1.0f / 255);
        self->actor.scale.x = self->actor.scale.z = 0.0056000003f * multiplier + 0.0014000001f;
        self->actor.scale.y = 0.007f - 0.007f * multiplier + 0.007f;
    } else {
        multiplier = 1.0f;
        self->actor.scale.x = self->actor.scale.y = self->actor.scale.z = self->lightColor.a * (0.007f / 255);
        self->actor.world.pos.y = self->actor.home.pos.y + ((1.0f / 17.0f) * self->lightColor.a);
    }
    self->lightColor.r = info->lightColor.r * multiplier;
    self->lightColor.g = info->lightColor.g * multiplier;
    self->lightColor.b = info->lightColor.b * multiplier;
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, info->lightColor.r, info->lightColor.g, info->lightColor.b,
                              self->lightColor.a * (200.0f / 255));
}

void func_80AD587C(EnPoField* self, GlobalContext* globalCtx) {
    self->actor.home.pos.y += 2.0f;
    EnPoField_SoulUpdateProperties(self, 20);
    if (self->lightColor.a == 255) {
        func_80AD4384(self);
    }
}

void func_80AD58D4(EnPoField* self, GlobalContext* globalCtx) {
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        EnPoField_SetupInteractWithSoul(self);
        return;
    }
    if (self->actionTimer == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
        self->actor.flags &= ~0x10000;
        EnPoField_SetupSoulDisappear(self);
        return;
    }
    if (self->collider.base.ocFlags1 & OC1_HIT) {
        self->actor.flags |= 0x10000;
        func_8002F2F4(&self->actor, globalCtx);
    } else {
        self->actor.flags &= ~0x10000;
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
    self->actor.world.pos.y = Math_SinS(self->unk_194 * 0x800) * 5.0f + self->actor.home.pos.y;
    if (self->unk_194 != 0) {
        self->unk_194 -= 1;
    }
    if (self->unk_194 == 0) {
        self->unk_194 = 32;
    }
    self->collider.dim.pos.y = self->actor.world.pos.y - 20.0f;
    Actor_SetFocus(&self->actor, -10.0f);
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                              self->actor.world.pos.z, self->lightInfo.params.point.color[0],
                              self->lightInfo.params.point.color[1], self->lightInfo.params.point.color[2],
                              self->lightColor.a * (200.0f / 255));
}

void EnPoField_SoulDisappear(EnPoField* self, GlobalContext* globalCtx) {
    EnPoField_SoulUpdateProperties(self, -13);
    if (self->lightColor.a == 0) {
        EnPoField_SetupWaitForSpawn(self, globalCtx);
    }
}

void EnPoField_SoulInteract(EnPoField* self, GlobalContext* globalCtx) {
    if (self->actor.textId != 0x5005) {
        EnPoField_SoulUpdateProperties(self, -13);
    } else {
        func_8002F974(&self->actor, NA_SE_EN_PO_BIG_CRY - SFX_FLAG);
    }
    if (func_8010BDBC(&globalCtx->msgCtx) == 4) {
        if (func_80106BC8(globalCtx) != 0) {
            func_800F8A44(&self->actor.projectedPos, NA_SE_EN_PO_BIG_CRY - SFX_FLAG);
            if (globalCtx->msgCtx.choiceIndex == 0) {
                if (Inventory_HasEmptyBottle()) {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_BIG_GET);
                    if (self->actor.params == 0) {
                        Item_Give(globalCtx, ITEM_POE);
                        self->actor.textId = 0x5008;
                    } else {
                        self->actor.textId = 0x508F;
                        Item_Give(globalCtx, ITEM_BIG_POE);
                        Flags_SetSwitch(globalCtx, sSpawnSwitchFlags[self->spawnFlagIndex]);
                    }
                } else {
                    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
                    self->actor.textId = 0x5006;
                }
            } else {
                self->actor.textId = 0x5007;
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_LAUGH);
            }
            func_8010B720(globalCtx, self->actor.textId);
            return;
        }
    } else if (func_8002F334(&self->actor, globalCtx) != 0) {
        EnPoField_SetupSoulDisappear(self);
    }
}

void EnPoField_TestForDamage(EnPoField* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        if (self->actor.colChkInfo.damageEffect != 0 || self->actor.colChkInfo.damage != 0) {
            if (Actor_ApplyDamage(&self->actor) == 0) {
                Enemy_StartFinishingBlow(globalCtx, &self->actor);
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DEAD);
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DAMAGE);
            }
            EnPoField_SetupDamage(self);
        }
    }
}

void EnPoField_SpawnFlame(EnPoField* self) {
    if (self->flameTimer == 0) {
        self->flamePosition.x = self->lightInfo.params.point.x;
        self->flamePosition.y = self->lightInfo.params.point.y;
        self->flamePosition.z = self->lightInfo.params.point.z;
        self->flameTimer = 70;
        self->flameRotation = self->actor.shape.rot.y;
    }
}

void EnPoField_UpdateFlame(EnPoField* self, GlobalContext* globalCtx) {
    if (self->flameTimer != 0) {
        if (self->flameTimer != 0) {
            self->flameTimer--;
        }
        if (self->flameCollider.base.atFlags & AT_HIT) {
            self->flameCollider.base.atFlags &= ~AT_HIT;
            self->flameTimer = 19;
        }
        if (self->flameTimer < 20) {
            Math_StepToF(&self->flameScale, 0.0f, 0.00015f);
            return;
        }
        if (Math_StepToF(&self->flameScale, 0.003f, 0.0006f) != 0) {
            self->flamePosition.x += 2.5f * Math_SinS(self->flameRotation);
            self->flamePosition.z += 2.5f * Math_CosS(self->flameRotation);
        }
        self->flameCollider.dim.pos.x = self->flamePosition.x;
        self->flameCollider.dim.pos.y = self->flamePosition.y;
        self->flameCollider.dim.pos.z = self->flamePosition.z;
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->flameCollider.base);
    }
}

void EnPoField_DrawFlame(EnPoField* self, GlobalContext* globalCtx) {
    f32 sp4C;
    s32 pad;

    if (self->flameTimer != 0) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_po_field.c", 1669);
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0,
                                    (globalCtx->gameplayFrames * -20) % 512, 32, 128));
        sp4C = self->flameScale * 85000.0f;
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 0, sp4C);
        Matrix_Translate(self->flamePosition.x, self->flamePosition.y, self->flamePosition.z, MTXMODE_NEW);
        Matrix_RotateY((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000) * (M_PI / 0x8000), MTXMODE_APPLY);
        if (self->flameTimer >= 20) {
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
            Matrix_Scale(self->flameScale, self->flameScale, self->flameScale, MTXMODE_APPLY);
        } else {
            gDPSetEnvColor(POLY_XLU_DISP++, sp4C, 0, 0, 0);
            Matrix_Scale((self->flameScale * 0.7f) + 0.00090000004f, (0.003f - self->flameScale) + 0.003f, 0.003f,
                         MTXMODE_APPLY);
        }
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_field.c", 1709),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_po_field.c", 1712);
    }
}

void func_80AD619C(EnPoField* self) {
    s16 temp_var;

    if (self->actionFunc == EnPoField_Flee) {
        self->lightColor.r = CLAMP_MAX((s16)(self->lightColor.r + 5), 80);
        self->lightColor.g = CLAMP_MAX((s16)(self->lightColor.g + 5), 255);
        temp_var = self->lightColor.b + 5;
        self->lightColor.b = CLAMP_MAX(temp_var, 225);
    } else if (self->actionFunc == EnPoField_Damage) {
        if (self->actor.colorFilterTimer & 2) {
            self->lightColor.r = 0;
            self->lightColor.g = 0;
            self->lightColor.b = 0;
        } else {
            self->lightColor.r = 80;
            self->lightColor.g = 255;
            self->lightColor.b = 225;
        }
    } else {
        self->lightColor.r = CLAMP_MAX((s16)(self->lightColor.r + 5), 255);
        self->lightColor.g = CLAMP_MAX((s16)(self->lightColor.g + 5), 255);
        if (self->lightColor.b > 210) {
            temp_var = self->lightColor.b - 5;
            self->lightColor.b = CLAMP_MIN(temp_var, 210);
        } else {
            temp_var = self->lightColor.b + 5;
            self->lightColor.b = CLAMP_MAX(temp_var, 210);
        }
    }
}

void func_80AD6330(EnPoField* self) {
    f32 rand;

    if (self->actionFunc == EnPoField_Appear && self->skelAnime.curFrame < 12.0f) {
        self->soulColor.r = self->soulColor.g = self->soulColor.b = (s16)(self->skelAnime.curFrame * 16.66f) + 55;
        self->soulColor.a = self->skelAnime.curFrame * (100.0f / 6.0f);
    } else {
        rand = Rand_ZeroOne();
        self->soulColor.r = (s16)(rand * 30.0f) + 225;
        self->soulColor.g = (s16)(rand * 100.0f) + 155;
        self->soulColor.b = (s16)(rand * 160.0f) + 95;
        self->soulColor.a = 200;
    }
}

void EnPoField_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnPoField* self = THIS;

    EnPoField_TestForDamage(self, globalCtx);
    self->actionFunc(self, globalCtx);
    EnPoField_UpdateFlame(self, globalCtx);
    if (self->actionFunc == EnPoField_Flee || self->actionFunc == EnPoField_Damage ||
        self->actionFunc == EnPoField_Appear) {
        Actor_MoveForward(&self->actor);
    }
    if (self->actionFunc != EnPoField_WaitForSpawn) {
        Actor_SetFocus(&self->actor, 42.0f);
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 27.0f, 60.0f, 4);
        func_80AD619C(self);
        func_80AD6330(self);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        if (self->collider.base.acFlags & AC_ON) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }
}

s32 EnPoField_OverrideLimbDraw2(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx, Gfx** gfxP) {
    EnPoField* self = THIS;

    if (self->lightColor.a == 0 || limbIndex == 7 || (self->actionFunc == EnPoField_Death && self->actionTimer >= 2)) {
        *dList = NULL;
    } else if (self->actor.params == EN_PO_FIELD_BIG) {
        if (limbIndex == 1) {
            *dList = gBigPoeFaceDL;
        } else if (limbIndex == 8) {
            *dList = gBigPoeCloakDL;
        } else if (limbIndex == 9) {
            *dList = gBigPoeBodyDL;
        }
    }
    if (self->actionFunc == EnPoField_Disappear && limbIndex == 7) {
        Matrix_Scale(self->actionTimer / 16.0f, self->actionTimer / 16.0f, self->actionTimer / 16.0f, MTXMODE_APPLY);
    }
    return false;
}

void EnPoField_PostLimDraw2(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfxP) {
    EnPoField* self = THIS;

    if (self->actionFunc == EnPoField_Death && self->actionTimer >= 2 && limbIndex == 8) {
        gSPMatrix((*gfxP)++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_field.c", 1916),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList((*gfxP)++, gPoeFieldBurnDL);
    }
    if (limbIndex == 7) {
        Vec3f vec;
        Matrix_MultVec3f(&D_80AD714C, &vec);
        if (self->actionFunc == EnPoField_Death && self->actionTimer >= 19 && self->actor.scale.x != 0.0f) {
            f32 mtxScale = 0.01f / self->actor.scale.x;
            Matrix_Scale(mtxScale, mtxScale, mtxScale, MTXMODE_APPLY);
        }
        Matrix_Get(&sLimb7Mtx);
        if (self->actionFunc == EnPoField_Death && self->actionTimer == 27) {
            self->actor.world.pos.x = sLimb7Mtx.xw;
            self->actor.world.pos.y = sLimb7Mtx.yw;
            self->actor.world.pos.z = sLimb7Mtx.zw;
        }
        Lights_PointGlowSetInfo(&self->lightInfo, vec.x, vec.y, vec.z, self->soulColor.r, self->soulColor.g,
                                self->soulColor.b, self->soulColor.a * (200.0f / 255));
    }
}

void EnPoField_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnPoField* self = THIS;
    EnPoFieldInfo* info = &sPoFieldInfo[self->actor.params];

    if (self->actionFunc != EnPoField_WaitForSpawn) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_po_field.c", 1976);
        func_80093D18(globalCtx->state.gfxCtx);
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 0x0A,
                   Gfx_EnvColor(globalCtx->state.gfxCtx, info->envColor.r, info->envColor.g, info->envColor.b, 255));
        if (self->lightColor.a == 255 || self->lightColor.a == 0) {
            gSPSegment(POLY_OPA_DISP++, 0x08,
                       Gfx_EnvColor(globalCtx->state.gfxCtx, self->lightColor.r, self->lightColor.g, self->lightColor.b,
                                    self->lightColor.a));
            gSPSegment(POLY_OPA_DISP++, 0x0C, D_80116280 + 2);
            POLY_OPA_DISP =
                SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                               EnPoField_OverrideLimbDraw2, EnPoField_PostLimDraw2, &self->actor, POLY_OPA_DISP);
        } else {
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_EnvColor(globalCtx->state.gfxCtx, self->lightColor.r, self->lightColor.g, self->lightColor.b,
                                    self->lightColor.a));
            gSPSegment(POLY_XLU_DISP++, 0x0C, D_80116280);
            POLY_XLU_DISP =
                SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                               EnPoField_OverrideLimbDraw2, EnPoField_PostLimDraw2, &self->actor, POLY_XLU_DISP);
        }
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, self->soulColor.r, self->soulColor.g, self->soulColor.b, 255);
        Matrix_Put(&sLimb7Mtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_field.c", 2033),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gPoeFieldLanternDL);
        gSPDisplayList(POLY_OPA_DISP++, gPoeFieldLanternTopDL);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_po_field.c", 2039);
    }
    EnPoField_DrawFlame(self, globalCtx);
}

void EnPoField_UpdateDead(Actor* thisx, GlobalContext* globalCtx) {
    EnPoField* self = THIS;

    self->actionFunc(self, globalCtx);
    if (self->actionFunc == EnPoField_SoulIdle) {
        func_80AD6330(self);
    }
    EnPoField_UpdateFlame(self, globalCtx);
}

void EnPoField_DrawSoul(Actor* thisx, GlobalContext* globalCtx) {
    EnPoField* self = THIS;
    s32 pad;
    EnPoFieldInfo* info = &sPoFieldInfo[self->actor.params];

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_po_field.c", 2077);
    if (self->actionFunc == EnPoField_SoulIdle) {
        func_80093D18(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 0x0A,
                   Gfx_EnvColor(globalCtx->state.gfxCtx, info->envColor.r, info->envColor.g, info->envColor.b, 255));
        Lights_PointGlowSetInfo(&self->lightInfo, self->actor.world.pos.x, self->actor.world.pos.y,
                                self->actor.world.pos.z, self->soulColor.r, self->soulColor.g, self->soulColor.b, 200);
        gDPSetEnvColor(POLY_OPA_DISP++, self->soulColor.r, self->soulColor.g, self->soulColor.b, 255);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_field.c", 2104),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gPoeFieldLanternDL);
        gSPDisplayList(POLY_OPA_DISP++, gPoeFieldLanternTopDL);
    } else {
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 0x20, 0x40, 1, 0,
                                    (globalCtx->gameplayFrames * info->unk_9) & 0x1FF, 0x20, 0x80));
        gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(info->soulTexture));
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, info->primColor.r, info->primColor.g, info->primColor.b,
                        self->lightColor.a);
        gDPSetEnvColor(POLY_XLU_DISP++, self->lightColor.r, self->lightColor.g, self->lightColor.b, 255);
        Matrix_RotateY((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(globalCtx)) + 0x8000) * 9.58738e-05f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_field.c", 2143),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gPoeFieldSoulDL);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_po_field.c", 2149);
    EnPoField_DrawFlame(self, globalCtx);
}
