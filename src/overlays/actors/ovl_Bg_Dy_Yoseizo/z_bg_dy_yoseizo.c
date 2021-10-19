/*
 * File: z_bg_dy_yoseizo.c
 * Overlay: ovl_Bg_Dy_Yoseizo
 * Description: Great Fairy
 */

#include "z_bg_dy_yoseizo.h"
#include "objects/object_dy_obj/object_dy_obj.h"
#include "vt.h"
#include "overlays/actors/ovl_Demo_Effect/z_demo_effect.h"
#include "scenes/indoors/yousei_izumi_yoko/yousei_izumi_yoko_scene.h"
#include "scenes/indoors/daiyousei_izumi/daiyousei_izumi_scene.h"

#define FLAGS 0x02000030

#define THIS ((BgDyYoseizo*)thisx)

typedef enum {
    /* 0 */ FAIRY_UPGRADE_MAGIC,
    /* 1 */ FAIRY_UPGRADE_DOUBLE_MAGIC,
    /* 2 */ FAIRY_UPGRADE_HALF_DAMAGE
} BgDyYoseizoRewardType;

typedef enum {
    /* 0 */ FAIRY_SPELL_FARORES_WIND,
    /* 1 */ FAIRY_SPELL_DINS_FIRE,
    /* 2 */ FAIRY_SPELL_NAYRUS_LOVE
} BgDyYoseizoSpellType;

void BgDyYoseizo_Init(Actor* thisx, GlobalContext* globalCtx);
void BgDyYoseizo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgDyYoseizo_Update(Actor* thisx, GlobalContext* globalCtx);
void BgDyYoseizo_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgDyYoseizo_CheckMagicAcquired(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_ChooseType(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SetupSpinGrow_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SpinGrow_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_CompleteSpinGrow_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SetupGreetPlayer_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_GreetPlayer_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SetupHealPlayer_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_HealPlayer_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SayFarewell_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SetupSpinShrink(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SpinShrink(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_Vanish(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SetupSpinGrow_Reward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_SpinGrowSetupGive_Reward(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_Give_Reward(BgDyYoseizo* self, GlobalContext* globalCtx);

void BgDyYoseizo_ParticleInit(BgDyYoseizo* self, Vec3f* initPos, Vec3f* initVelocity, Vec3f* accel,
                              Color_RGB8* primColor, Color_RGB8* envColor, f32 scale, s16 life, s16 type);
void BgDyYoseizo_ParticleUpdate(BgDyYoseizo* self, GlobalContext* globalCtx);
void BgDyYoseizo_ParticleDraw(BgDyYoseizo* self, GlobalContext* globalCtx);

static s32 sUnusedGetItemIds[] = { GI_FARORES_WIND, GI_NAYRUS_LOVE, GI_DINS_FIRE };

const ActorInit Bg_Dy_Yoseizo_InitVars = {
    ACTOR_BG_DY_YOSEIZO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DY_OBJ,
    sizeof(BgDyYoseizo),
    (ActorFunc)BgDyYoseizo_Init,
    (ActorFunc)BgDyYoseizo_Destroy,
    (ActorFunc)BgDyYoseizo_Update,
    NULL,
};

void BgDyYoseizo_Init(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgDyYoseizo* self = THIS;

    self->fountainType = globalCtx->curSpawn;

    if (self->fountainType < 0) {
        self->fountainType = 0;
    }

    self->vanishHeight = self->actor.world.pos.y;
    self->grownHeight = self->vanishHeight + 40.0f;
    self->actor.focus.pos = self->actor.world.pos;

    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        // "Great Fairy Fountain"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 大妖精の泉 ☆☆☆☆☆ %d\n" VT_RST, globalCtx->curSpawn);
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGreatFairySkel, &gGreatFairySittingTransitionAnim,
                           self->jointTable, self->morphTable, 28);
    } else {
        // "Stone/Jewel Fairy Fountain"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 石妖精の泉 ☆☆☆☆☆ %d\n" VT_RST, globalCtx->curSpawn);
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGreatFairySkel, &gGreatFairyLayingDownTransitionAnim,
                           self->jointTable, self->morphTable, 28);
    }
    self->actionFunc = BgDyYoseizo_CheckMagicAcquired;
}

void BgDyYoseizo_Destroy(Actor* self, GlobalContext* globalCtx) {
}

static Color_RGB8 sParticlePrimColors[] = {
    { 255, 255, 255 }, { 255, 255, 100 }, { 100, 255, 100 }, { 255, 100, 100 }, { 255, 255, 170 },
    { 255, 255, 100 }, { 100, 255, 100 }, { 255, 100, 100 }, { 255, 255, 170 },
};

static Color_RGB8 sParticleEnvColors[] = {
    { 155, 255, 255 }, { 255, 255, 100 }, { 100, 255, 100 }, { 255, 100, 100 }, { 255, 100, 255 },
    { 255, 255, 100 }, { 100, 255, 100 }, { 255, 100, 100 }, { 100, 255, 255 },
};

void BgDyYoseizo_SpawnParticles(BgDyYoseizo* self, GlobalContext* globalCtx, s16 type) {
    Vec3f particleInitVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f particleAccel;
    Vec3f particleInitPos;
    Color_RGB8 particlePrimColor;
    Color_RGB8 particleEnvColor;
    f32 spawnPosVariation;
    s32 particleType;
    f32 particleScale;
    s32 i;
    s16 particleLife;

    if (!(self->scale < 0.01f)) {
        spawnPosVariation = self->scale * 3500.0f;
        particleAccel.x = Rand_ZeroOne() - 0.5f;
        particleAccel.y = Rand_ZeroOne() - 0.5f;
        particleAccel.z = Rand_ZeroOne() - 0.5f;
        for (i = 0; i < 2; i++) {
            if (type == 0) {
                particleType = 0;
                particleScale = 0.4f;
                particleLife = 90;
                particleInitPos.x = self->actor.world.pos.x;
                particleInitPos.y = self->actor.world.pos.y + spawnPosVariation +
                                    ((Rand_ZeroOne() - 0.5f) * (spawnPosVariation * 0.5f));
                particleInitPos.z = self->actor.world.pos.z + 30.0f;
            } else {
                particleLife = 50;
                particleType = type;
                particleScale = 0.2f;
                particleInitPos.x = self->actor.world.pos.x + Rand_CenteredFloat(10.0f);

                if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
                    particleInitPos.y = self->actor.world.pos.y + spawnPosVariation + 50.0f +
                                        ((Rand_ZeroOne() - 0.5f) * (spawnPosVariation * 0.1f));
                    particleInitPos.z = self->actor.world.pos.z + 30.0f;
                } else {
                    particleInitPos.y = self->actor.world.pos.y + spawnPosVariation - 30.0f +
                                        ((Rand_ZeroOne() - 0.5f) * (spawnPosVariation * 0.1f));
                    particleInitPos.z = self->actor.world.pos.z + 60.0f;
                }

                if (LINK_IS_ADULT) {
                    particleInitPos.y += 20.0f;
                }
            }

            particlePrimColor.r = sParticlePrimColors[particleType].r;
            particlePrimColor.g = sParticlePrimColors[particleType].g;
            particlePrimColor.b = sParticlePrimColors[particleType].b;
            particleEnvColor.r = sParticleEnvColors[particleType].r;
            particleEnvColor.g = sParticleEnvColors[particleType].g;
            particleEnvColor.b = sParticleEnvColors[particleType].b;
            BgDyYoseizo_ParticleInit(self, &particleInitPos, &particleInitVelocity, &particleAccel, &particlePrimColor,
                                     &particleEnvColor, particleScale, particleLife, particleType);
        }
    }
}

void BgDyYoseizo_Bob(BgDyYoseizo* self, GlobalContext* globalCtx) {
    self->targetHeight = self->grownHeight + self->bobOffset;
    Math_ApproachF(&self->actor.world.pos.y, self->targetHeight, 0.1f, 10.0f);
    Math_ApproachF(&self->bobOffset, 10.0f, 0.1f, 0.5f);

    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        self->actor.velocity.y = Math_SinS(self->bobTimer);
    } else {
        self->actor.velocity.y = Math_SinS(self->bobTimer) * 0.4f;
    }
}

void BgDyYoseizo_CheckMagicAcquired(BgDyYoseizo* self, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, 0x38)) {
        globalCtx->msgCtx.unk_E3EE = 4;
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            if (!gSaveContext.magicAcquired && (self->fountainType != FAIRY_UPGRADE_MAGIC)) {
                Actor_Kill(&self->actor);
                return;
            }
        } else {
            if (!gSaveContext.magicAcquired) {
                Actor_Kill(&self->actor);
                return;
            }
        }
        func_8002DF54(globalCtx, &self->actor, 1);
        self->actionFunc = BgDyYoseizo_ChooseType;
    }
}

void BgDyYoseizo_ChooseType(BgDyYoseizo* self, GlobalContext* globalCtx) {
    s32 givingReward;

    func_8002DF54(globalCtx, &self->actor, 1);
    // "Mode"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ もうど ☆☆☆☆☆ %d\n" VT_RST, globalCtx->msgCtx.unk_E3EE);
    givingReward = false;

    if (globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) {
        switch (self->fountainType) {
            case FAIRY_SPELL_FARORES_WIND:
                if (!(gSaveContext.itemGetInf[1] & 0x100)) {
                    givingReward = true;
                }
                break;
            case FAIRY_SPELL_DINS_FIRE:
                if (!(gSaveContext.itemGetInf[1] & 0x200)) {
                    givingReward = true;
                }
                break;
            case FAIRY_SPELL_NAYRUS_LOVE:
                if (!(gSaveContext.itemGetInf[1] & 0x400)) {
                    givingReward = true;
                }
                break;
        }
    } else {
        switch (self->fountainType) {
            case FAIRY_UPGRADE_MAGIC:
                if (!gSaveContext.magicAcquired || BREG(2)) {
                    // "Spin Attack speed UP"
                    osSyncPrintf(VT_FGCOL(GREEN) " ☆☆☆☆☆ 回転切り速度ＵＰ ☆☆☆☆☆ \n" VT_RST);
                    self->givingSpell = true;
                    givingReward = true;
                }
                break;
            case FAIRY_UPGRADE_DOUBLE_MAGIC:
                if (!gSaveContext.doubleMagic) {
                    // "Magic Meter doubled"
                    osSyncPrintf(VT_FGCOL(YELLOW) " ☆☆☆☆☆ 魔法ゲージメーター倍増 ☆☆☆☆☆ \n" VT_RST);
                    self->givingSpell = true;
                    givingReward = true;
                }
                break;
            case FAIRY_UPGRADE_HALF_DAMAGE:
                if (!gSaveContext.doubleDefense) {
                    // "Damage halved"
                    osSyncPrintf(VT_FGCOL(PURPLE) " ☆☆☆☆☆ ダメージ半減 ☆☆☆☆☆ \n" VT_RST);
                    self->givingSpell = true;
                    givingReward = true;
                }
                break;
        }
    }

    if (givingReward) {
        if (gSaveContext.sceneSetupIndex < 4) {
            if (globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) {
                switch (self->fountainType) {
                    case FAIRY_SPELL_FARORES_WIND:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gGreatFairyFaroresWindCs);
                        gSaveContext.cutsceneTrigger = 1;
                        break;
                    case FAIRY_SPELL_DINS_FIRE:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gGreatFairyDinsFireCs);
                        gSaveContext.cutsceneTrigger = 1;
                        break;
                    case FAIRY_SPELL_NAYRUS_LOVE:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gGreatFairyNayrusLoveCs);
                        gSaveContext.cutsceneTrigger = 1;
                        break;
                }
            } else {
                switch (self->fountainType) {
                    case FAIRY_UPGRADE_MAGIC:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gGreatFairyMagicCs);
                        gSaveContext.cutsceneTrigger = 1;
                        break;
                    case FAIRY_UPGRADE_DOUBLE_MAGIC:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gGreatFairyDoubleMagicCs);
                        gSaveContext.cutsceneTrigger = 1;
                        break;
                    case FAIRY_UPGRADE_HALF_DAMAGE:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gGreatFairyDoubleDefenceCs);
                        gSaveContext.cutsceneTrigger = 1;
                        break;
                }
            }
        }
        self->actionFunc = BgDyYoseizo_SetupSpinGrow_Reward;
        return;
    }

    globalCtx->envCtx.unk_BF = 2;

    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        OnePointCutscene_Init(globalCtx, 8603, -99, NULL, MAIN_CAM);
    } else {
        OnePointCutscene_Init(globalCtx, 8604, -99, NULL, MAIN_CAM);
    };

    Audio_PlayActorSound2(&self->actor, NA_SE_EV_GREAT_FAIRY_APPEAR);
    self->actor.draw = BgDyYoseizo_Draw;
    self->actionFunc = BgDyYoseizo_SetupSpinGrow_NoReward;
}

// Sets animations for spingrow
void BgDyYoseizo_SetupSpinGrow_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        self->frameCount = Animation_GetLastFrame(&gGreatFairySittingTransitionAnim);
        Animation_Change(&self->skelAnime, &gGreatFairySittingTransitionAnim, 1.0f, 0.0f, self->frameCount,
                         ANIMMODE_ONCE, -10.0f);
    } else {
        self->frameCount = Animation_GetLastFrame(&gGreatFairyLayingDownTransitionAnim);
        Animation_Change(&self->skelAnime, &gGreatFairyLayingDownTransitionAnim, 1.0f, 0.0f, self->frameCount,
                         ANIMMODE_ONCE, -10.0f);
    }

    Audio_PlayActorSound2(&self->actor, NA_SE_VO_FR_LAUGH_0);
    func_8002DF54(globalCtx, &self->actor, 1);
    self->actionFunc = BgDyYoseizo_SpinGrow_NoReward;
}

void BgDyYoseizo_SpinGrow_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &self->actor, 1);
    Math_ApproachF(&self->actor.world.pos.y, self->grownHeight, self->heightFraction, 100.0f);
    Math_ApproachF(&self->scale, 0.035f, self->scaleFraction, 0.005f);
    Math_ApproachF(&self->heightFraction, 0.8f, 0.1f, 0.02f);
    Math_ApproachF(&self->scaleFraction, 0.2f, 0.03f, 0.05f);
    // Finished growing
    if (self->scale >= 0.034f) {
        if ((self->actor.shape.rot.y > -8000) && (self->actor.shape.rot.y < 1000)) {
            SkelAnime_Update(&self->skelAnime);
            // Turn to front
            Math_SmoothStepToS(&self->actor.shape.rot.y, 0, 5, 1000, 0);
            if (fabsf(self->actor.shape.rot.y) < 50.0f) {
                self->actionFunc = BgDyYoseizo_CompleteSpinGrow_NoReward;
            }
        } else {
            self->actor.shape.rot.y += 3000;
        }
    } else {
        self->actor.shape.rot.y += 3000;
    }
    BgDyYoseizo_SpawnParticles(self, globalCtx, 0);
}

void BgDyYoseizo_CompleteSpinGrow_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    f32 curFrame = self->skelAnime.curFrame;

    func_8002DF54(globalCtx, &self->actor, 1);

    if ((self->frameCount * 1273.0f) <= self->bobTimer) {
        self->bobTimer = 0.0f;
    }

    SkelAnime_Update(&self->skelAnime);

    if ((self->frameCount <= curFrame) && !self->animationChanged) {
        self->actionFunc = BgDyYoseizo_SetupGreetPlayer_NoReward;
    }
}

void BgDyYoseizo_SetupGreetPlayer_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &self->actor, 1);

    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        self->frameCount = Animation_GetLastFrame(&gGreatFairySittingAnim);
        Animation_Change(&self->skelAnime, &gGreatFairySittingAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_LOOP,
                         -10.0f);
    } else {
        self->frameCount = Animation_GetLastFrame(&gGreatFairyLayingSidewaysAnim);
        Animation_Change(&self->skelAnime, &gGreatFairyLayingSidewaysAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_LOOP,
                         -10.0f);
    }

    self->actor.textId = 0xDB;
    self->dialogState = 5;
    func_8010B680(globalCtx, self->actor.textId, NULL);
    BgDyYoseizo_SpawnParticles(self, globalCtx, 0);
    self->actionFunc = BgDyYoseizo_GreetPlayer_NoReward;
}

void BgDyYoseizo_GreetPlayer_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &self->actor, 1);
    self->bobTimer = self->skelAnime.curFrame * 1273.0f;

    if ((self->frameCount * 1273.0f) <= self->bobTimer) {
        self->bobTimer = 0.0f;
    }

    SkelAnime_Update(&self->skelAnime);

    if ((self->dialogState == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        Interface_ChangeAlpha(5);
        self->actionFunc = BgDyYoseizo_SetupHealPlayer_NoReward;
    }

    BgDyYoseizo_Bob(self, globalCtx);
    BgDyYoseizo_SpawnParticles(self, globalCtx, 0);
}

void BgDyYoseizo_SetupHealPlayer_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        self->frameCount = Animation_GetLastFrame(&gGreatFairyGivingUpgradeAnim);
        Animation_Change(&self->skelAnime, &gGreatFairyGivingUpgradeAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_ONCE,
                         -10.0f);
    } else {
        self->frameCount = Animation_GetLastFrame(&gGreatFairyAnim_005810);
        Animation_Change(&self->skelAnime, &gGreatFairyAnim_005810, 1.0f, 0.0f, self->frameCount, ANIMMODE_ONCE,
                         -10.0f);
    }

    Audio_PlayActorSound2(&self->actor, NA_SE_VO_FR_SMILE_0);
    self->mouthState = 1;
    self->actionFunc = BgDyYoseizo_HealPlayer_NoReward;
}

void BgDyYoseizo_HealPlayer_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 curFrame = self->skelAnime.curFrame;
    Vec3f beamPos;
    s16 beamParams;

    if (self->animationChanged) {
        self->bobTimer = self->skelAnime.curFrame * 1300.0f;
        if ((self->frameCount * 1300.0f) <= self->bobTimer) {
            self->bobTimer = 0.0f;
        }
    }

    SkelAnime_Update(&self->skelAnime);
    if ((self->frameCount <= curFrame) && !(self->animationChanged)) {
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            self->frameCount = Animation_GetLastFrame(&gGreatFairyAfterUpgradeAnim);
            Animation_Change(&self->skelAnime, &gGreatFairyAfterUpgradeAnim, 1.0f, 0.0f, self->frameCount,
                             ANIMMODE_LOOP, -10.0f);
        } else {
            self->frameCount = Animation_GetLastFrame(&gGreatFairyAfterSpellAnim);
            Animation_Change(&self->skelAnime, &gGreatFairyAfterSpellAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_LOOP,
                             -10.0f);
        }
        self->healingTimer = 150;
        self->animationChanged = true;
        if (!self->givingSpell) {
            beamPos.x = player->actor.world.pos.x;
            beamPos.y = player->actor.world.pos.y + 200.0f;
            beamPos.z = player->actor.world.pos.z;

            beamParams = ((globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) ? 0 : 1);

            self->beam =
                (EnDyExtra*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_DY_EXTRA,
                                               beamPos.x, beamPos.y, beamPos.z, 0, 0, 0, beamParams);
        }
    }
    if (self->refillTimer > 1) {
        self->refillTimer--;
    }

    if (self->healingTimer >= 110) {
        self->healingTimer--;
    }

    if (self->healingTimer == 110) {
        gSaveContext.healthAccumulator = 0x140;
        Magic_Fill(globalCtx);
        self->refillTimer = 200;
    }

    if (((gSaveContext.healthCapacity == gSaveContext.health) && (gSaveContext.magic == gSaveContext.unk_13F4)) ||
        (self->refillTimer == 1)) {
        self->healingTimer--;
        if (self->healingTimer == 90) {
            if (!self->givingSpell) {
                self->beam->trigger = 1;
            }
            self->givingSpell = false;
        }
    }

    if (self->healingTimer == 1) {
        self->actor.textId = 0xDA;
        self->dialogState = 5;
        func_8010B720(globalCtx, self->actor.textId);
        self->actionFunc = BgDyYoseizo_SayFarewell_NoReward;
        return;
    }
    BgDyYoseizo_Bob(self, globalCtx);
}

void BgDyYoseizo_SayFarewell_NoReward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    self->bobTimer = self->skelAnime.curFrame * 1400.0f;

    if (self->bobTimer >= (self->frameCount * 1400.0f)) {
        self->bobTimer = 0.0f;
    }

    SkelAnime_Update(&self->skelAnime);

    if ((self->dialogState == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        self->mouthState = 0;
        self->actionFunc = BgDyYoseizo_SetupSpinShrink;
        func_8005B1A4(GET_ACTIVE_CAM(globalCtx));
    }

    BgDyYoseizo_Bob(self, globalCtx);
    BgDyYoseizo_SpawnParticles(self, globalCtx, 0);
}

void BgDyYoseizo_SetupSpinShrink(BgDyYoseizo* self, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        self->frameCount = Animation_GetLastFrame(&gGreatFairyJewelFountainSpinShrinkAnim);
        Animation_Change(&self->skelAnime, &gGreatFairyJewelFountainSpinShrinkAnim, 1.0f, 0.0f, self->frameCount,
                         ANIMMODE_ONCE, -10.0f);
    } else {
        self->frameCount = Animation_GetLastFrame(&gGreatFairySpellFountainSpinShrinkAnim);
        Animation_Change(&self->skelAnime, &gGreatFairySpellFountainSpinShrinkAnim, 1.0f, 0.0f, self->frameCount,
                         ANIMMODE_ONCE, -10.0f);
    }

    self->vanishTimer = 5;
    self->scaleFraction = 0.0f;
    self->heightFraction = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_VO_FR_LAUGH_0);
    Audio_PlayActorSound2(&self->actor, NA_SE_EV_GREAT_FAIRY_VANISH);
    self->actionFunc = BgDyYoseizo_SpinShrink;
}

void BgDyYoseizo_SpinShrink(BgDyYoseizo* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->vanishTimer == 0) {
        if (self->scale < 0.003f) {
            self->vanishTimer = 30;
            self->actionFunc = BgDyYoseizo_Vanish;
        } else {
            Math_ApproachF(&self->actor.world.pos.y, self->vanishHeight, self->heightFraction, 100.0f);
            Math_ApproachZeroF(&self->scale, self->scaleFraction, 0.005f);
            Math_ApproachF(&self->heightFraction, 0.8f, 0.1f, 0.02f);
            Math_ApproachF(&self->scaleFraction, 0.2f, 0.03f, 0.05f);
            self->actor.shape.rot.y += 3000;
            BgDyYoseizo_SpawnParticles(self, globalCtx, 0);
        }
    }
}

void BgDyYoseizo_Vanish(BgDyYoseizo* self, GlobalContext* globalCtx) {
    Actor* findOcarinaSpot;

    if (self->vanishTimer == 0) {
        func_8002DF54(globalCtx, &self->actor, 7);
        globalCtx->envCtx.unk_BF = 0;
        findOcarinaSpot = globalCtx->actorCtx.actorLists[ACTORCAT_PROP].head;

        while (findOcarinaSpot != NULL) {
            if (findOcarinaSpot->id != ACTOR_EN_OKARINA_TAG) {
                findOcarinaSpot = findOcarinaSpot->next;
                continue;
            }
            Actor_Kill(findOcarinaSpot);
            break;
        }

        Flags_UnsetSwitch(globalCtx, 0x38);
        Actor_Kill(&self->actor);
    }
}

void BgDyYoseizo_SetupSpinGrow_Reward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        if ((globalCtx->csCtx.npcActions[0] != NULL) && (globalCtx->csCtx.npcActions[0]->action == 2)) {
            self->actor.draw = BgDyYoseizo_Draw;
            func_8002DF54(globalCtx, &self->actor, 1);
            self->finishedSpinGrow = false;

            if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
                self->frameCount = Animation_GetLastFrame(&gGreatFairySittingTransitionAnim);
                Animation_Change(&self->skelAnime, &gGreatFairySittingTransitionAnim, 1.0f, 0.0f, self->frameCount,
                                 ANIMMODE_ONCE, -10.0f);
            } else {
                self->frameCount = Animation_GetLastFrame(&gGreatFairyLayingDownTransitionAnim);
                Animation_Change(&self->skelAnime, &gGreatFairyLayingDownTransitionAnim, 1.0f, 0.0f, self->frameCount,
                                 ANIMMODE_ONCE, -10.0f);
            }

            Audio_PlayActorSound2(&self->actor, NA_SE_EV_GREAT_FAIRY_APPEAR);
            self->actionFunc = BgDyYoseizo_SpinGrowSetupGive_Reward;
        }
    }
}

void BgDyYoseizo_SpinGrowSetupGive_Reward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    f32 curFrame = self->skelAnime.curFrame;

    if (!self->finishedSpinGrow) {
        Math_ApproachF(&self->actor.world.pos.y, self->grownHeight, self->heightFraction, 100.0f);
        Math_ApproachF(&self->scale, 0.035f, self->scaleFraction, 0.005f);
        Math_ApproachF(&self->heightFraction, 0.8f, 0.1f, 0.02f);
        Math_ApproachF(&self->scaleFraction, 0.2f, 0.03f, 0.05f);
        // Finished growing
        if (self->scale >= 0.034f) {
            if ((self->actor.shape.rot.y > -8000) && (self->actor.shape.rot.y < 1000)) {
                SkelAnime_Update(&self->skelAnime);
                // Spin until facing front
                Math_ApproachS(&self->actor.shape.rot.y, 0, 5, 1000);
                if (fabsf(self->actor.shape.rot.y) < 50.0f) {
                    self->finishedSpinGrow = true;
                }
            } else {
                self->actor.shape.rot.y += 3000;
            }
        } else {
            self->actor.shape.rot.y += 3000;
        }
    } else {
        SkelAnime_Update(&self->skelAnime);

        if ((self->frameCount <= curFrame) && !self->animationChanged) {
            if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
                self->frameCount = Animation_GetLastFrame(&gGreatFairySittingAnim);
                Animation_Change(&self->skelAnime, &gGreatFairySittingAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_LOOP,
                                 -10.0f);
            } else {
                self->frameCount = Animation_GetLastFrame(&gGreatFairyLayingSidewaysAnim);
                Animation_Change(&self->skelAnime, &gGreatFairyLayingSidewaysAnim, 1.0f, 0.0f, self->frameCount,
                                 ANIMMODE_LOOP, -10.0f);
            }
            self->animationChanged = true;
        }

        if ((globalCtx->csCtx.state != CS_STATE_IDLE) &&
            ((globalCtx->csCtx.npcActions[0] != NULL) && (globalCtx->csCtx.npcActions[0]->action == 3))) {
            self->finishedSpinGrow = self->animationChanged = false;
            if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
                self->frameCount = Animation_GetLastFrame(&gGreatFairyGivingUpgradeAnim);
                Animation_Change(&self->skelAnime, &gGreatFairyGivingUpgradeAnim, 1.0f, 0.0f, self->frameCount,
                                 ANIMMODE_ONCE, -10.0f);
            } else {
                self->frameCount = Animation_GetLastFrame(&gGreatFairyAnim_005810);
                Animation_Change(&self->skelAnime, &gGreatFairyAnim_005810, 1.0f, 0.0f, self->frameCount, ANIMMODE_ONCE,
                                 -10.0f);
            }
            self->mouthState = 1;
            self->actionFunc = BgDyYoseizo_Give_Reward;
        }
    }
    BgDyYoseizo_SpawnParticles(self, globalCtx, 0);
}

static s16 sDemoEffectLightColors[] = { DEMO_EFFECT_LIGHT_GREEN, DEMO_EFFECT_LIGHT_RED, DEMO_EFFECT_LIGHT_BLUE };

static s16 sExItemTypes[] = { EXITEM_MAGIC_WIND, EXITEM_MAGIC_FIRE, EXITEM_MAGIC_DARK };

static s16 sItemGetFlags[] = { 0x100, 0x200, 0x400 };

static u8 sItemIds[] = { ITEM_FARORES_WIND, ITEM_DINS_FIRE, ITEM_NAYRUS_LOVE };

void BgDyYoseizo_Give_Reward(BgDyYoseizo* self, GlobalContext* globalCtx) {
    f32 curFrame = self->skelAnime.curFrame;
    Player* player = GET_PLAYER(globalCtx);
    s16 actionIndex;
    s16 demoEffectParams;
    Vec3f itemPos;

    if (self->animationChanged) {
        self->bobTimer = self->skelAnime.curFrame * 1400.0f;
        if ((self->frameCount * 1400.0f) <= self->bobTimer) {
            self->bobTimer = 0.0f;
        }
    }
    SkelAnime_Update(&self->skelAnime);

    if ((self->frameCount <= curFrame) && !self->animationChanged) {
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            self->frameCount = Animation_GetLastFrame(&gGreatFairyAfterUpgradeAnim);
            Animation_Change(&self->skelAnime, &gGreatFairyAfterUpgradeAnim, 1.0f, 0.0f, self->frameCount,
                             ANIMMODE_LOOP, -10.0f);
        } else {
            self->frameCount = Animation_GetLastFrame(&gGreatFairyAfterSpellAnim);
            Animation_Change(&self->skelAnime, &gGreatFairyAfterSpellAnim, 1.0f, 0.0f, self->frameCount, ANIMMODE_LOOP,
                             -10.0f);
        }
        self->animationChanged = true;
    }

    if (globalCtx->csCtx.npcActions[0]->action == 13) {
        self->actionFunc = BgDyYoseizo_SetupSpinShrink;
        return;
    }

    if ((globalCtx->csCtx.npcActions[0]->action >= 4) && (globalCtx->csCtx.npcActions[0]->action < 7)) {
        actionIndex = globalCtx->csCtx.npcActions[0]->action - 4;
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            actionIndex++;
            BgDyYoseizo_SpawnParticles(self, globalCtx, actionIndex);

        } else if (!self->lightBallSpawned) {
            demoEffectParams = ((s16)(sDemoEffectLightColors[actionIndex] << 0xC) | DEMO_EFFECT_LIGHT);
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, self->actor.world.pos.x,
                        self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, (s32)demoEffectParams);
            self->lightBallSpawned = true;
        }
    } else {
        BgDyYoseizo_SpawnParticles(self, globalCtx, 0);
    }

    if ((globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) && (globalCtx->csCtx.npcActions[0]->action >= 10) &&
        (globalCtx->csCtx.npcActions[0]->action < 13)) {
        actionIndex = globalCtx->csCtx.npcActions[0]->action - 10;

        switch (actionIndex) {
            case FAIRY_UPGRADE_MAGIC:
                gSaveContext.magicAcquired = true;
                gSaveContext.unk_13F6 = 0x30;
                Interface_ChangeAlpha(9);
                break;
            case FAIRY_UPGRADE_DOUBLE_MAGIC:
                if (!gSaveContext.magicAcquired) {
                    gSaveContext.magicAcquired = true;
                }
                gSaveContext.doubleMagic = true;
                gSaveContext.unk_13F6 = 0x60;
                gSaveContext.magicLevel = 0;
                Interface_ChangeAlpha(9);
                break;
            case FAIRY_UPGRADE_HALF_DAMAGE:
                gSaveContext.doubleDefense = true;
                Interface_ChangeAlpha(9);
                break;
        }

        if (!self->healing) {
            gSaveContext.healthAccumulator = 0x140;
            self->healing = true;
            if (actionIndex == 2) {
                Magic_Fill(globalCtx);
            }
        }
    }

    if ((globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) && (globalCtx->csCtx.npcActions[0]->action >= 14) &&
        (globalCtx->csCtx.npcActions[0]->action < 17)) {
        actionIndex = globalCtx->csCtx.npcActions[0]->action - 14;

        if (!self->itemSpawned) {
            itemPos.x = player->actor.world.pos.x;
            itemPos.y = (LINK_IS_ADULT ? player->actor.world.pos.y + 73.0f : player->actor.world.pos.y + 53.0f);
            itemPos.z = player->actor.world.pos.z;

            self->item =
                (EnExItem*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_EX_ITEM,
                                              itemPos.x, itemPos.y, itemPos.z, 0, 0, 0, sExItemTypes[actionIndex]);

            if (self->item != NULL) {
                if (gSaveContext.magicAcquired == 0) {
                    gSaveContext.magicAcquired = 1;
                } else {
                    Magic_Fill(globalCtx);
                }

                self->itemSpawned = true;
                gSaveContext.healthAccumulator = 0x140;
                Interface_ChangeAlpha(9);
                gSaveContext.itemGetInf[1] |= sItemGetFlags[actionIndex];
                Item_Give(globalCtx, sItemIds[actionIndex]);
            }
        } else {
            self->item->actor.world.pos.x = player->actor.world.pos.x;
            self->item->actor.world.pos.y =
                (LINK_IS_ADULT ? player->actor.world.pos.y + 73.0f : player->actor.world.pos.y + 53.0f);
            self->item->actor.world.pos.z = player->actor.world.pos.z;
            self->item->scale = 0.3f;
        }
    }

    if ((globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) && (globalCtx->csCtx.npcActions[0]->action == 17) &&
        (self->item != NULL)) {
        Actor_Kill(&self->item->actor);
        self->item = NULL;
    }

    if ((globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) && (globalCtx->csCtx.npcActions[0]->action == 18)) {
        self->giveDefenseHearts = true;
    }

    if (self->giveDefenseHearts) {
        if (gSaveContext.inventory.defenseHearts < 20) {
            gSaveContext.inventory.defenseHearts++;
        }
    }

    if ((globalCtx->csCtx.npcActions[0]->action >= 19) && (globalCtx->csCtx.npcActions[0]->action < 22) &&
        !self->warpEffectSpawned) {
        actionIndex = globalCtx->csCtx.npcActions[0]->action - 11;
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_WARP1, player->actor.world.pos.x,
                    player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0, actionIndex);
        self->warpEffectSpawned = true;
    }
    BgDyYoseizo_Bob(self, globalCtx);
}

void BgDyYoseizo_Update(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    BgDyYoseizo* self = THIS;
    s32 phi_v1;

    self->absoluteTimer++;

    if (self->vanishTimer != 0) {
        self->vanishTimer--;
    }
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    }
    if (self->unusedTimer != 0) {
        self->unusedTimer--;
    }

    self->actionFunc(self, globalCtx);

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        phi_v1 = 0;
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            if ((globalCtx->csCtx.frames == 32) || (globalCtx->csCtx.frames == 291) ||
                (globalCtx->csCtx.frames == 426) || (globalCtx->csCtx.frames == 851)) {
                phi_v1 = 1;
            }
            if (globalCtx->csCtx.frames == 101) {
                phi_v1 = 2;
            }
        } else {
            if ((globalCtx->csCtx.frames == 35) || (globalCtx->csCtx.frames == 181) ||
                (globalCtx->csCtx.frames == 462) || (globalCtx->csCtx.frames == 795)) {
                phi_v1 = 1;
            }
            if (globalCtx->csCtx.frames == 90) {
                phi_v1 = 2;
            }
        }

        if (phi_v1 == 1) {
            Audio_PlayActorSound2(&self->actor, NA_SE_VO_FR_SMILE_0);
        }
        if (phi_v1 == 2) {
            Audio_PlayActorSound2(&self->actor, NA_SE_VO_FR_LAUGH_0);
        }
    }

    if ((self->blinkTimer == 0) && (self->actionFunc != BgDyYoseizo_HealPlayer_NoReward)) {
        self->eyeState++;
        self->eyeState2++;
        if (self->eyeState >= 3) {
            self->eyeState = self->eyeState2 = 0;
            self->blinkTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }

    Actor_MoveForward(&self->actor);
    self->heightOffset = self->scale * 7500.0f;
    Actor_SetFocus(&self->actor, self->heightOffset);
    self->actor.focus.pos.y = self->heightOffset;
    func_80038290(globalCtx, &self->actor, &self->headRot, &self->torsoRot, self->actor.focus.pos);
    BgDyYoseizo_ParticleUpdate(self, globalCtx);
    Actor_SetScale(&self->actor, self->scale);
}

s32 BgDyYoseizo_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                 void* thisx) {
    BgDyYoseizo* self = THIS;

    if (limbIndex == 8) { // Torso
        rot->x += self->torsoRot.y;
    }
    if (limbIndex == 15) { // Head
        rot->x += self->headRot.y;
        rot->z += self->headRot.z;
    }
    return 0;
}

static void* sEyeTextures[] = {
    &gGreatFairyEyeOpenTex,   // Open
    &gGreatFairyEyeHalfTex,   // Half
    &gGreatFairyEyeClosedTex, // Closed
};

static void* sMouthTextures[] = {
    &gGreatFairyMouthClosedTex, // Closed
    &gGreatFairyMouthOpenTex,   // Open
};

void BgDyYoseizo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgDyYoseizo* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_dy_yoseizo.c", 1609);
    if (self->actionFunc != BgDyYoseizo_Vanish) {
        func_80093D18(globalCtx->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeState]));

        // This was probably intended to allow self actor to wink, but segment 09 is not used in the dList for the head,
        // so it can only blink
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sEyeTextures[self->eyeState2]));

        gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(sMouthTextures[self->mouthState]));

        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, BgDyYoseizo_OverrideLimbDraw, NULL, self);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_dy_yoseizo.c", 1629);
    BgDyYoseizo_ParticleDraw(self, globalCtx);
}

void BgDyYoseizo_ParticleInit(BgDyYoseizo* self, Vec3f* initPos, Vec3f* initVelocity, Vec3f* accel,
                              Color_RGB8* primColor, Color_RGB8* envColor, f32 scale, s16 life, s16 type) {
    BgDyYoseizoParticle* particle;
    s16 i;

    particle = self->particles;

    for (i = 0; i < 200; i++, particle++) {
        if (particle->alive == 0) {
            particle->alive = 1;
            particle->pos = *initPos;
            particle->velocity = *initVelocity;
            particle->accel = *accel;
            particle->primColor = *primColor;
            particle->alpha = 0;
            particle->envColor = *envColor;
            particle->scale = scale;
            particle->timer = life;
            particle->type = type;
            particle->pitch = 0.0f;
            particle->yaw = Rand_CenteredFloat(30000.0f);
            particle->roll = 0.0f;
            return;
        }
    }
}

void BgDyYoseizo_ParticleUpdate(BgDyYoseizo* self, GlobalContext* globalCtx) {
    BgDyYoseizoParticle* particle = self->particles;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f sp94;
    Vec3f sp88;
    f32 goalPitch;
    f32 goalYaw;
    s16 i = 0;

    for (i = 0; i < 200; i++, particle++) {
        if (particle->alive != 0) {
            particle->roll += 3000.0f;

            if (particle->type == 0) {
                particle->pos.x += particle->velocity.x;
                particle->pos.y += particle->velocity.y;
                particle->pos.z += particle->velocity.z;
                particle->velocity.x += particle->accel.x;
                particle->velocity.y += particle->accel.y;
                particle->velocity.z += particle->accel.z;
            } else {
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_HEALING - SFX_FLAG);

                sp94 = player->actor.world.pos;
                sp94.y = player->actor.world.pos.y - 150.0f;
                sp94.z = player->actor.world.pos.z - 50.0f;

                goalPitch = Math_Vec3f_Pitch(&particle->pos, &sp94);
                goalYaw = Math_Vec3f_Yaw(&particle->pos, &sp94);

                Math_ApproachF(&particle->pitch, goalPitch, 0.9f, 5000.0f);
                Math_ApproachF(&particle->yaw, goalYaw, 0.9f, 5000.0f);
                Matrix_Push();
                Matrix_RotateY(BINANG_TO_RAD(particle->yaw), MTXMODE_NEW);
                Matrix_RotateX(BINANG_TO_RAD(particle->pitch), MTXMODE_APPLY);

                sp94.x = sp94.y = sp94.z = 3.0f;

                Matrix_MultVec3f(&sp94, &sp88);
                Matrix_Pop();
                particle->pos.x += sp88.x;
                particle->pos.y += sp88.y;
                particle->pos.z += sp88.z;
            }
        }

        // fade up, fade down, vanish and reset
        if (particle->timer != 0) {
            particle->timer--;
            particle->alpha += 30;

            if (particle->alpha > 255) {
                particle->alpha = 255;
            }
        } else {
            particle->alpha -= 30;

            if (particle->alpha <= 0) {
                particle->alpha = particle->alive = 0;
            }
        }
    }
}

void BgDyYoseizo_ParticleDraw(BgDyYoseizo* self, GlobalContext* globalCtx) {
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    u8 phi_s3 = 0;
    BgDyYoseizoParticle* particle = self->particles;
    s16 i;

    OPEN_DISPS(gfxCtx, "../z_bg_dy_yoseizo.c", 1767);
    func_80093D84(globalCtx->state.gfxCtx);

    for (i = 0; i < 200; i++, particle++) {
        if (particle->alive == 1) {
            if (phi_s3 == 0) {
                gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(gGreatFairyParticleAppearDL));
                gDPPipeSync(POLY_XLU_DISP++);

                phi_s3++;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, particle->primColor.r, particle->primColor.g, particle->primColor.b,
                            particle->alpha);
            gDPSetEnvColor(POLY_XLU_DISP++, particle->envColor.r, particle->envColor.g, particle->envColor.b, 0);

            Matrix_Translate(particle->pos.x, particle->pos.y, particle->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(particle->scale, particle->scale, 1.0f, MTXMODE_APPLY);
            Matrix_RotateZ(particle->roll, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_bg_dy_yoseizo.c", 1810),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(&gGreatFairyParticleAliveDL));
        }
    }

    CLOSE_DISPS(gfxCtx, "../z_bg_dy_yoseizo.c", 1819);
}
