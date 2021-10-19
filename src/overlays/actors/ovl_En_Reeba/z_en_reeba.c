
/*
 * File: z_en_reeba.c
 * Overlay: ovl_En_Reeba
 * Description: Leever
 */

#include "z_en_reeba.h"
#include "overlays/actors/ovl_En_Encount1/z_en_encount1.h"
#include "vt.h"
#include "objects/object_reeba/object_reeba.h"

#define FLAGS 0x08000015

#define THIS ((EnReeba*)thisx)

void EnReeba_Init(Actor* thisx, GlobalContext* globalCtx);
void EnReeba_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnReeba_Update(Actor* thisx, GlobalContext* globalCtx);
void EnReeba_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AE4F40(EnReeba* self, GlobalContext* globalCtx);
void func_80AE5054(EnReeba* self, GlobalContext* globalCtx);
void func_80AE5270(EnReeba* self, GlobalContext* globalCtx);
void func_80AE5688(EnReeba* self, GlobalContext* globalCtx);
void func_80AE56E0(EnReeba* self, GlobalContext* globalCtx);
void func_80AE538C(EnReeba* self, GlobalContext* globalCtx);
void func_80AE53AC(EnReeba* self, GlobalContext* globalCtx);
void func_80AE5E48(EnReeba* self, GlobalContext* globalCtx);
void func_80AE5854(EnReeba* self, GlobalContext* globalCtx);
void func_80AE5C38(EnReeba* self, GlobalContext* globalCtx);
void func_80AE5938(EnReeba* self, GlobalContext* globalCtx);
void func_80AE5A9C(EnReeba* self, GlobalContext* globalCtx);

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(2, 0xE),
    /* Slingshot     */ DMG_ENTRY(1, 0xE),
    /* Explosive     */ DMG_ENTRY(2, 0xE),
    /* Boomerang     */ DMG_ENTRY(1, 0xC),
    /* Normal arrow  */ DMG_ENTRY(2, 0xE),
    /* Hammer swing  */ DMG_ENTRY(2, 0xE),
    /* Hookshot      */ DMG_ENTRY(2, 0xD),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xE),
    /* Master sword  */ DMG_ENTRY(4, 0xE),
    /* Giant's Knife */ DMG_ENTRY(6, 0xE),
    /* Fire arrow    */ DMG_ENTRY(2, 0xE),
    /* Ice arrow     */ DMG_ENTRY(4, 0x3),
    /* Light arrow   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0xE),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0xE),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(4, 0x3),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(2, 0xE),
    /* Giant spin    */ DMG_ENTRY(8, 0xE),
    /* Master spin   */ DMG_ENTRY(4, 0xE),
    /* Kokiri jump   */ DMG_ENTRY(2, 0xE),
    /* Giant jump    */ DMG_ENTRY(8, 0xE),
    /* Master jump   */ DMG_ENTRY(4, 0xE),
    /* Unknown 1     */ DMG_ENTRY(0, 0x1),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

const ActorInit En_Reeba_InitVars = {
    ACTOR_EN_REEBA,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_REEBA,
    sizeof(EnReeba),
    (ActorFunc)EnReeba_Init,
    (ActorFunc)EnReeba_Destroy,
    (ActorFunc)EnReeba_Update,
    (ActorFunc)EnReeba_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT5,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x08, 0x08 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON | BUMP_HOOKABLE,
        OCELEM_ON,
    },
    { 20, 40, 0, { 0, 0, 0 } },
};

void EnReeba_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnReeba* self = THIS;
    s32 surfaceType;

    self->actor.naviEnemyId = 0x47;
    self->actor.targetMode = 3;
    self->actor.gravity = -3.5f;
    self->actor.focus.pos = self->actor.world.pos;
    SkelAnime_Init(globalCtx, &self->skelanime, &object_reeba_Skel_001EE8, &object_reeba_Anim_0001E4, self->jointTable,
                   self->morphTable, 18);
    self->actor.colChkInfo.mass = MASS_HEAVY;
    self->actor.colChkInfo.health = 4;
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    self->isBig = self->actor.params;
    self->scale = 0.04f;

    if (self->isBig) {
        self->collider.dim.radius = 35;
        self->collider.dim.height = 45;
        self->scale *= 1.5f;
        osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ リーバぼす登場 ☆☆☆☆☆ %f\n" VT_RST, self->scale);
        self->actor.colChkInfo.health = 20;
        self->collider.info.toucher.effect = 4;
        self->collider.info.toucher.damage = 16;
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_ENEMY);
    }

    self->actor.shape.yOffset = self->unk_284 = self->scale * -27500.0f;
    ActorShape_Init(&self->actor.shape, self->actor.shape.yOffset, ActorShadow_DrawCircle, 0.0f);
    self->actor.colChkInfo.damageTable = &sDamageTable;
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 35.0f, 60.0f, 60.0f, 0x1D);

    surfaceType = func_80041D4C(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);

    if ((surfaceType != 4) && (surfaceType != 7)) {
        Actor_Kill(&self->actor);
        return;
    }

    self->actionfunc = func_80AE4F40;
}

void EnReeba_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnReeba* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);

    if (self->actor.parent != NULL) {
        EnEncount1* spawner = (EnEncount1*)self->actor.parent;

        if (spawner->actor.update != NULL) {
            if (spawner->curNumSpawn > 0) {
                spawner->curNumSpawn--;
            }
            if (self->isBig) {
                spawner->bigLeever = NULL;
                spawner->timer = 600;
            }
        }
    }
}

void func_80AE4F40(EnReeba* self, GlobalContext* globalCtx) {
    f32 frames = Animation_GetLastFrame(&object_reeba_Anim_0001E4);
    Player* player = GET_PLAYER(globalCtx);
    s16 playerSpeed;

    Animation_Change(&self->skelanime, &object_reeba_Anim_0001E4, 2.0f, 0.0f, frames, ANIMMODE_LOOP, -10.0f);

    playerSpeed = fabsf(player->linearVelocity);
    self->unk_278 = 20 - playerSpeed * 2;
    if (self->unk_278 < 0) {
        self->unk_278 = 2;
    }
    if (self->unk_278 > 20) {
        self->unk_278 = 20;
    }

    self->actor.flags &= ~0x08000000;
    self->actor.world.pos.y = self->actor.floorHeight;

    if (self->isBig) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIVA_BIG_APPEAR);
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIVA_APPEAR);
    }

    self->actionfunc = func_80AE5054;
}

void func_80AE5054(EnReeba* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 playerLinearVel;

    SkelAnime_Update(&self->skelanime);

    if ((globalCtx->gameplayFrames % 4) == 0) {
        Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, self->actor.shape.shadowScale, 1,
                                 8.0f, 500, 10, 1);
    }

    if (self->unk_278 == 0) {
        Math_ApproachF(&self->actor.shape.shadowScale, 12.0f, 1.0f, 1.0f);
        if (self->actor.shape.yOffset < 0.0f) {
            Math_ApproachZeroF(&self->actor.shape.yOffset, 1.0f, self->unk_288);
            Math_ApproachF(&self->unk_288, 300.0f, 1.0f, 5.0f);
        } else {
            self->unk_288 = 0.0f;
            self->actor.shape.yOffset = 0.0f;
            playerLinearVel = player->linearVelocity;

            switch (self->unk_280) {
                case 0:
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
                    break;
                case 1:
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer + (800.0f * playerLinearVel);
                    break;
                case 2:
                case 3:
                    self->actor.world.rot.y =
                        self->actor.yawTowardsPlayer +
                        (player->actor.shape.rot.y - self->actor.yawTowardsPlayer) * (playerLinearVel * 0.15f);
                    break;
                case 4:
                    self->actor.world.rot.y = self->actor.yawTowardsPlayer - (800.0f * playerLinearVel);
                    break;
            }

            if (self->isBig) {
                self->actionfunc = func_80AE538C;
            } else {
                self->unk_272 = 130;
                self->actor.speedXZ = Rand_ZeroFloat(4.0f) + 6.0f;
                self->actionfunc = func_80AE5270;
            }
        }
    }
}

void func_80AE5270(EnReeba* self, GlobalContext* globalCtx) {
    s32 surfaceType;

    SkelAnime_Update(&self->skelanime);

    if (self->actor.shape.shadowScale < 12.0f) {
        Math_ApproachF(&self->actor.shape.shadowScale, 12.0f, 3.0f, 1.0f);
    }

    surfaceType = func_80041D4C(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);

    if ((surfaceType != 4) && (surfaceType != 7)) {
        self->actor.speedXZ = 0.0f;
        self->actionfunc = func_80AE5688;
    } else if ((self->unk_272 == 0) || (self->actor.xzDistToPlayer < 30.0f) || (self->actor.xzDistToPlayer > 400.0f) ||
               (self->actor.bgCheckFlags & 8)) {
        self->actionfunc = func_80AE5688;
    } else if (self->unk_274 == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIVA_MOVE);
        self->unk_274 = 10;
    }
}

void func_80AE538C(EnReeba* self, GlobalContext* globalCtx) {
    self->actor.flags |= 5;
    self->actionfunc = func_80AE53AC;
}

void func_80AE53AC(EnReeba* self, GlobalContext* globalCtx) {
    f32 speed;
    s16 yawDiff;
    s16 yaw;
    s32 surfaceType;

    SkelAnime_Update(&self->skelanime);

    if (self->actor.shape.shadowScale < 12.0f) {
        Math_ApproachF(&self->actor.shape.shadowScale, 12.0f, 3.0f, 1.0f);
    }

    surfaceType = func_80041D4C(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);

    if (((surfaceType != 4) && (surfaceType != 7)) || (self->actor.xzDistToPlayer > 400.0f) ||
        (self->actor.bgCheckFlags & 8)) {
        self->actionfunc = func_80AE5688;
    } else {
        if ((self->actor.xzDistToPlayer < 70.0f) && (self->unk_270 == 0)) {
            self->unk_270 = 30;
        }

        speed = (self->actor.xzDistToPlayer - 20.0f) / ((Rand_ZeroOne() * 50.0f) + 150.0f);
        self->actor.speedXZ += speed * 1.8f;
        if (self->actor.speedXZ >= 3.0f) {
            self->actor.speedXZ = 3.0f;
        }
        if (self->actor.speedXZ < -3.0f) {
            self->actor.speedXZ = -3.0f;
        }

        yawDiff = (self->unk_270 == 0) ? self->actor.yawTowardsPlayer : -self->actor.yawTowardsPlayer;
        yawDiff -= self->actor.world.rot.y;
        yaw = (yawDiff > 0) ? ((yawDiff / 31.0f) + 10.0f) : ((yawDiff / 31.0f) - 10.0f);
        self->actor.world.rot.y += yaw * 2.0f;

        if (self->unk_274 == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIVA_MOVE);
            self->unk_274 = 20;
        }
    }
}

void func_80AE561C(EnReeba* self, GlobalContext* globalCtx) {
    Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 0.3f);

    if (self->unk_272 == 0) {
        if (self->isBig) {
            self->actionfunc = func_80AE538C;
        } else {
            self->actionfunc = func_80AE5688;
        }
    }
}

void func_80AE5688(EnReeba* self, GlobalContext* globalCtx) {
    self->unk_27E = 0;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_AKINDONUTS_HIDE);
    self->actor.flags |= 0x8000000;
    self->actor.flags &= ~5;
    self->actionfunc = func_80AE56E0;
}

void func_80AE56E0(EnReeba* self, GlobalContext* globalCtx) {
    Math_ApproachZeroF(&self->actor.shape.shadowScale, 1.0f, 0.3f);
    Math_ApproachZeroF(&self->actor.speedXZ, 0.1f, 0.3f);
    SkelAnime_Update(&self->skelanime);

    if ((self->unk_284 + 10.0f) <= self->actor.shape.yOffset) {
        if ((globalCtx->gameplayFrames % 4) == 0) {
            Actor_SpawnFloorDustRing(globalCtx, &self->actor, &self->actor.world.pos, self->actor.shape.shadowScale, 1,
                                     8.0f, 500, 10, 1);
        }

        Math_ApproachF(&self->actor.shape.yOffset, self->unk_284, 1.0f, self->unk_288);
        Math_ApproachF(&self->unk_288, 300.0f, 1.0f, 5.0f);
    } else {
        Actor_Kill(&self->actor);
    }
}

void func_80AE57F0(EnReeba* self, GlobalContext* globalCtx) {
    self->unk_276 = 14;
    self->actor.speedXZ = -8.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
    self->actionfunc = func_80AE5854;
}

void func_80AE5854(EnReeba* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelanime);

    if (self->actor.speedXZ < 0.0f) {
        self->actor.speedXZ += 1.0f;
    }

    if (self->unk_276 == 0) {
        if (self->isBig) {
            self->unk_270 = 30;
            self->actionfunc = func_80AE538C;
        } else {
            self->actionfunc = func_80AE5688;
        }
    }
}

void func_80AE58EC(EnReeba* self, GlobalContext* globalCtx) {
    self->unk_278 = 14;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    self->actor.speedXZ = -8.0f;
    self->actor.flags |= 0x8000000;
    self->actor.flags &= ~5;
    self->actionfunc = func_80AE5938;
}

void func_80AE5938(EnReeba* self, GlobalContext* globalCtx) {
    Vec3f pos;
    f32 scale;

    if (self->unk_278 != 0) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 1.0f;
        }
    } else {
        self->actor.speedXZ = 0.0f;

        if ((self->unk_27E == 4) || (self->actor.colChkInfo.health != 0)) {
            if (self->unk_27E == 2) {
                pos.x = self->actor.world.pos.x + Rand_CenteredFloat(20.0f);
                pos.y = self->actor.world.pos.y + Rand_CenteredFloat(20.0f);
                pos.z = self->actor.world.pos.z + Rand_CenteredFloat(20.0f);
                scale = 3.0f;

                if (self->isBig) {
                    scale = 6.0f;
                }

                EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &pos, 150, 150, 150, 250, 235, 245, 255, scale);
            }

            self->unk_278 = 66;
            self->actionfunc = func_80AE5E48;
        } else {
            self->unk_278 = 30;
            self->actionfunc = func_80AE5A9C;
        }
    }
}

void func_80AE5A9C(EnReeba* self, GlobalContext* globalCtx) {
    Vec3f pos;
    f32 scale;

    if (self->unk_278 != 0) {
        if ((self->unk_27E == 2) && ((self->unk_278 & 0xF) == 0)) {
            pos.x = self->actor.world.pos.x + Rand_CenteredFloat(20.0f);
            pos.y = self->actor.world.pos.y + Rand_CenteredFloat(20.0f);
            pos.z = self->actor.world.pos.z + Rand_CenteredFloat(20.0f);

            scale = 3.0f;
            if (self->isBig) {
                scale = 6.0f;
            }

            EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &self->actor, &pos, 150, 150, 150, 250, 235, 245, 255, scale);
        }
    } else {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIVA_DEAD);
        Enemy_StartFinishingBlow(globalCtx, &self->actor);
        self->actionfunc = func_80AE5C38;
    }
}

void func_80AE5BC4(EnReeba* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = -8.0f;
    self->actor.world.rot.y = self->actor.yawTowardsPlayer;
    Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
    self->unk_278 = 14;
    self->actor.flags &= ~1;
    self->actionfunc = func_80AE5C38;
}

void func_80AE5C38(EnReeba* self, GlobalContext* globalCtx) {
    Vec3f pos;
    Vec3f accel = { 0.0f, 0.0f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };

    if (self->unk_278 != 0) {
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ += 1.0f;
        }
    } else {
        self->actor.speedXZ = 0.0f;
        Math_ApproachZeroF(&self->scale, 0.1f, 0.01f);

        if (self->scale < 0.01f) {
            pos.x = self->actor.world.pos.x;
            pos.y = self->actor.world.pos.y;
            pos.z = self->actor.world.pos.z;

            velocity.y = 4.0f;

            EffectSsDeadDb_Spawn(globalCtx, &pos, &velocity, &accel, 120, 0, 255, 255, 255, 255, 255, 0, 0, 1, 9, true);

            if (!self->isBig) {
                Item_DropCollectibleRandom(globalCtx, &self->actor, &pos, 0xE0);
            } else {
                Item_DropCollectibleRandom(globalCtx, &self->actor, &pos, 0xC0);
            }

            if (self->actor.parent != NULL) {
                EnEncount1* spawner = (EnEncount1*)self->actor.parent;

                if ((spawner->actor.update != NULL) && !self->isBig) {
                    if (spawner->killCount < 10) {
                        spawner->killCount++;
                    }
                    // "How many are dead?"
                    osSyncPrintf("\n\n");
                    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 何匹ＤＥＡＤ？ ☆☆☆☆☆%d\n" VT_RST, spawner->killCount);
                    osSyncPrintf("\n\n");
                }

                Actor_Kill(&self->actor);
            }
        }
    }
}

void func_80AE5E48(EnReeba* self, GlobalContext* globalCtx) {
    if (self->unk_278 < 37) {
        self->actor.shape.rot.x = Rand_CenteredFloat(3000.0f);
        self->actor.shape.rot.z = Rand_CenteredFloat(3000.0f);

        if (self->unk_278 == 0) {
            if (self->isBig) {
                self->actionfunc = func_80AE538C;
            } else {
                self->actionfunc = func_80AE5688;
            }
        }
    }
}

void func_80AE5EDC(EnReeba* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;

        if ((self->actionfunc != func_80AE5C38) && (self->actionfunc != func_80AE5854)) {
            self->actor.shape.rot.x = self->actor.shape.rot.z = 0;
            self->unk_27E = 0;

            switch (self->actor.colChkInfo.damageEffect) {
                case 11: // none
                case 12: // boomerang
                    if ((self->actor.colChkInfo.health > 1) && (self->unk_27E != 4)) {
                        self->unk_27E = 4;
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
                        self->actionfunc = func_80AE58EC;
                        break;
                    }
                case 13: // hookshot/longshot
                    if ((self->actor.colChkInfo.health > 2) && (self->unk_27E != 4)) {
                        self->unk_27E = 4;
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 0x50);
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_GOMA_JR_FREEZE);
                        self->actionfunc = func_80AE58EC;
                        break;
                    }
                case 14:
                    self->unk_27C = 6;
                    Actor_ApplyDamage(&self->actor);
                    if (self->actor.colChkInfo.health == 0) {
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIVA_DEAD);
                        Enemy_StartFinishingBlow(globalCtx, &self->actor);
                        self->actionfunc = func_80AE5BC4;
                    } else {
                        if (self->actionfunc == func_80AE5E48) {
                            self->actor.shape.rot.x = self->actor.shape.rot.z = 0;
                        }
                        Audio_PlayActorSound2(&self->actor, NA_SE_EN_RIVA_DAMAGE);
                        self->actionfunc = func_80AE57F0;
                    }
                    break;
                case 3: // ice arrows/ice magic
                    Actor_ApplyDamage(&self->actor);
                    self->unk_27C = 2;
                    self->unk_27E = 2;
                    Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 80);
                    self->actionfunc = func_80AE58EC;
                    break;
                case 1: // unknown
                    if (self->unk_27E != 4) {
                        self->unk_27E = 4;
                        Actor_SetColorFilter(&self->actor, 0, 0xFF, 0, 80);
                        self->actionfunc = func_80AE58EC;
                    }
                    break;
            }
        }
    }
}

void EnReeba_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    EnReeba* self = THIS;
    Player* player = GET_PLAYER(globalCtx);

    func_80AE5EDC(self, globalCtx);
    self->actionfunc(self, globalCtx);
    Actor_SetScale(&self->actor, self->scale);

    if (self->unk_270 != 0) {
        self->unk_270--;
    }

    if (self->unk_272 != 0) {
        self->unk_272--;
    }

    if (self->unk_278 != 0) {
        self->unk_278--;
    }

    if (self->unk_274 != 0) {
        self->unk_274--;
    }

    if (self->unk_276 != 0) {
        self->unk_276--;
    }

    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 35.0f, 60.0f, 60.0f, 0x1D);

    if (self->collider.base.atFlags & AT_BOUNCED) {
        self->collider.base.atFlags &= ~AT_BOUNCED;

        if ((self->actionfunc == func_80AE5270) || (self->actionfunc == func_80AE53AC)) {
            self->actor.speedXZ = 8.0f;
            self->actor.world.rot.y *= -1.0f;
            self->unk_272 = 14;
            self->actionfunc = func_80AE561C;
            return;
        }
    }

    if (self->collider.base.atFlags & AT_HIT) {
        self->collider.base.atFlags &= ~AT_HIT;
        if ((self->collider.base.at == &player->actor) && !self->isBig && (self->actionfunc != func_80AE56E0)) {
            self->actionfunc = func_80AE5688;
        }
    }

    self->actor.focus.pos = self->actor.world.pos;

    if (!self->isBig) {
        self->actor.focus.pos.y += 15.0f;
    } else {
        self->actor.focus.pos.y += 30.0f;
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);

    if ((self->actor.shape.yOffset >= -700.0f) && (self->actor.colChkInfo.health > 0) &&
        (self->actionfunc != func_80AE56E0)) {
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

        if (!(self->actor.shape.yOffset < 0.0f)) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

            if ((self->actionfunc == func_80AE5270) || (self->actionfunc == func_80AE53AC)) {
                CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            }
        }
    }
}

void EnReeba_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnReeba* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_reeba.c", 1062);

    func_80093D18(globalCtx->state.gfxCtx);

    if (self->isBig) {
        gDPSetPrimColor(POLY_OPA_DISP++, 0x0, 0x01, 155, 55, 255, 255);
    } else {
        gDPSetPrimColor(POLY_OPA_DISP++, 0x0, 0x01, 255, 255, 255, 255);
    }

    SkelAnime_DrawOpa(globalCtx, self->skelanime.skeleton, self->skelanime.jointTable, NULL, NULL, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_reeba.c", 1088);

    if (BREG(0)) {
        Vec3f debugPos;

        debugPos.x = (Math_SinS(self->actor.world.rot.y) * 30.0f) + self->actor.world.pos.x;
        debugPos.y = self->actor.world.pos.y + 20.0f;
        debugPos.z = (Math_CosS(self->actor.world.rot.y) * 30.0f) + self->actor.world.pos.z;
        DebugDisplay_AddObject(debugPos.x, debugPos.y, debugPos.z, self->actor.world.rot.x, self->actor.world.rot.y,
                               self->actor.world.rot.z, 1.0f, 1.0f, 1.0f, 255, 0, 0, 255, 4, globalCtx->state.gfxCtx);
    }
}
