/*
 * File: z_en_dnt_nomal
 * Overlay: ovl_En_Dnt_Nomal
 * Description: Lost Woods minigame scrubs
 */

#include "z_en_dnt_nomal.h"
#include "objects/object_dnk/object_dnk.h"
#include "overlays/actors/ovl_En_Dnt_Demo/z_en_dnt_demo.h"
#include "overlays/actors/ovl_En_Ex_Ruppy/z_en_ex_ruppy.h"
#include "overlays/actors/ovl_En_Ex_Item/z_en_ex_item.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"
#include "objects/object_hintnuts/object_hintnuts.h"
#include "vt.h"

#define FLAGS 0x00000030

#define THIS ((EnDntNomal*)thisx)

void EnDntNomal_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDntNomal_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDntNomal_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDntNomal_DrawTargetScrub(Actor* thisx, GlobalContext* globalCtx);
void EnDntNomal_DrawStageScrub(Actor* thisx, GlobalContext* globalCtx);

void EnDntNomal_WaitForObject(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetFlower(EnDntNomal* self, GlobalContext* globalCtx);

void EnDntNomal_SetupTargetWait(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetupTargetUnburrow(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetupTargetWalk(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetupTargetTalk(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetupTargetGivePrize(EnDntNomal* self, GlobalContext* globalCtx);

void EnDntNomal_TargetWait(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_TargetUnburrow(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_TargetWalk(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_TargetFacePlayer(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_TargetTalk(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_TargetGivePrize(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_TargetReturn(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_TargetBurrow(EnDntNomal* self, GlobalContext* globalCtx);

void EnDntNomal_SetupStageWait(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetupStageCelebrate(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetupStageDance(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetupStageHide(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_SetupStageAttack(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageSetupReturn(EnDntNomal* self, GlobalContext* globalCtx);

void EnDntNomal_StageWait(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageUp(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageUnburrow(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageCelebrate(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageDance(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageHide(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageAttackHide(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageAttack(EnDntNomal* self, GlobalContext* globalCtx);
void EnDntNomal_StageReturn(EnDntNomal* self, GlobalContext* globalCtx);

const ActorInit En_Dnt_Nomal_InitVars = {
    ACTOR_EN_DNT_NOMAL,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnDntNomal),
    (ActorFunc)EnDntNomal_Init,
    (ActorFunc)EnDntNomal_Destroy,
    (ActorFunc)EnDntNomal_Update,
    NULL,
};

static ColliderCylinderInit sBodyCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 16, 46, 0, { 0, 0, 0 } },
};

static ColliderQuadInit sTargetQuadInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x0001F824, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static Color_RGBA8 sLeafColors[] = {
    { 255, 255, 255, 255 }, { 255, 195, 175, 255 }, { 210, 255, 0, 255 },
    { 255, 255, 255, 255 }, { 210, 255, 0, 255 },   { 255, 195, 175, 255 },
    { 255, 255, 255, 255 }, { 255, 195, 175, 255 }, { 210, 255, 0, 255 },
};

void EnDntNomal_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDntNomal* self = THIS;

    self->type = self->actor.params;
    if (self->type < ENDNTNOMAL_TARGET) {
        self->type = ENDNTNOMAL_TARGET;
    }
    self->actor.flags &= ~1;
    self->actor.colChkInfo.mass = 0xFF;
    self->objId = -1;
    if (self->type == ENDNTNOMAL_TARGET) {
        osSyncPrintf("\n\n");
        // "Deku Scrub target"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ デグナッツ的当て ☆☆☆☆☆ \n" VT_RST);
        Collider_InitQuad(globalCtx, &self->targetQuad);
        Collider_SetQuad(globalCtx, &self->targetQuad, &self->actor, &sTargetQuadInit);
        self->actor.world.rot.y = self->actor.shape.rot.y = self->actor.yawTowardsPlayer;
        self->objId = OBJECT_HINTNUTS;
    } else {
        osSyncPrintf("\n\n");
        // "Deku Scrub mask show audience"
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ デグナッツお面品評会一般人 ☆☆☆☆☆ \n" VT_RST);
        Collider_InitCylinder(globalCtx, &self->bodyCyl);
        Collider_SetCylinder(globalCtx, &self->bodyCyl, &self->actor, &sBodyCylinderInit);
        self->objId = OBJECT_DNK;
    }
    if (self->objId >= 0) {
        self->objIndex = Object_GetIndex(&globalCtx->objectCtx, self->objId);
        if (self->objIndex < 0) {
            Actor_Kill(&self->actor);
            // "What?"
            osSyncPrintf(VT_FGCOL(PURPLE) " なにみの？ %d\n" VT_RST "\n", self->objIndex);
            // "Bank is funny"
            osSyncPrintf(VT_FGCOL(CYAN) " バンクおかしいしぞ！%d\n" VT_RST "\n", self->actor.params);
            return;
        }
    } else {
        Actor_Kill(&self->actor);
    }
    self->actionFunc = EnDntNomal_WaitForObject;
}

void EnDntNomal_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDntNomal* self = THIS;

    if (self->type == ENDNTNOMAL_TARGET) {
        Collider_DestroyQuad(globalCtx, &self->targetQuad);
    } else {
        Collider_DestroyCylinder(globalCtx, &self->bodyCyl);
    }
}

void EnDntNomal_WaitForObject(EnDntNomal* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->objIndex)) {
        gSegments[6] = PHYSICAL_TO_VIRTUAL(globalCtx->objectCtx.status[self->objIndex].segment);
        self->actor.objBankIndex = self->objIndex;
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
        self->actor.gravity = -2.0f;
        Actor_SetScale(&self->actor, 0.01f);
        if (self->type == ENDNTNOMAL_TARGET) {
            SkelAnime_Init(globalCtx, &self->skelAnime, &gHintNutsSkel, &gHintNutsBurrowAnim, self->jointTable,
                           self->morphTable, 10);
            self->actor.draw = EnDntNomal_DrawTargetScrub;
        } else {
            SkelAnime_Init(globalCtx, &self->skelAnime, &gDntStageSkel, &gDntStageHideAnim, self->jointTable,
                           self->morphTable, 11);
            self->actor.draw = EnDntNomal_DrawStageScrub;
        }
        self->actionFunc = EnDntNomal_SetFlower;
    }
}

void EnDntNomal_SetFlower(EnDntNomal* self, GlobalContext* globalCtx) {
    if (self->actor.bgCheckFlags & 1) {
        self->flowerPos = self->actor.world.pos;
        if (self->type == ENDNTNOMAL_TARGET) {
            self->actionFunc = EnDntNomal_SetupTargetWait;
        } else {
            self->actionFunc = EnDntNomal_SetupStageWait;
        }
    }
}

void EnDntNomal_SetupTargetWait(EnDntNomal* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gHintNutsBurrowAnim);
    Animation_Change(&self->skelAnime, &gHintNutsBurrowAnim, 0.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
    self->skelAnime.curFrame = 8.0f;
    self->actionFunc = EnDntNomal_TargetWait;
}

void EnDntNomal_TargetWait(EnDntNomal* self, GlobalContext* globalCtx) {
    Vec3f scorePos;
    f32 targetX = 1340.0f;
    f32 targetY = 50.0f;
    f32 targetZ = -30.0f;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f scoreAccel = { 0.0f, 0.0f, 0.0f };
    Vec3f scoreVel = { 0.0f, 0.0f, 0.0f };

    self->targetVtx[0].x = self->targetVtx[1].x = self->targetVtx[2].x = self->targetVtx[3].x = targetX;

    self->targetVtx[1].y = self->targetVtx[0].y = targetY - 24.0f;

    self->targetVtx[2].z = self->targetVtx[0].z = targetZ + 24.0f;

    self->targetVtx[3].z = self->targetVtx[1].z = targetZ - 24.0f;

    self->targetVtx[3].y = self->targetVtx[2].y = targetY + 24.0f;

    SkelAnime_Update(&self->skelAnime);
    if ((self->targetQuad.base.acFlags & AC_HIT) || BREG(0)) {
        self->targetQuad.base.acFlags &= ~AC_HIT;

        dx = fabsf(targetX - self->targetQuad.info.bumper.hitPos.x);
        dy = fabsf(targetY - self->targetQuad.info.bumper.hitPos.y);
        dz = fabsf(targetZ - self->targetQuad.info.bumper.hitPos.z);

        scoreVel.y = 5.0f;

        if (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 8.0f) {
            scorePos.x = self->actor.world.pos.x - 20.0f;
            scorePos.y = self->actor.world.pos.y + 20.0f;
            scorePos.z = self->actor.world.pos.z;
            EffectSsExtra_Spawn(globalCtx, &scorePos, &scoreVel, &scoreAccel, 4, 2);
            Audio_StopSfx(NA_SE_SY_TRE_BOX_APPEAR);
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
            // "Big hit"
            osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 大当り ☆☆☆☆☆ %d\n" VT_RST, self->hitCounter);
            if (!LINK_IS_ADULT && !(gSaveContext.itemGetInf[1] & 0x2000)) {
                self->hitCounter++;
                if (self->hitCounter >= 3) {
                    OnePointCutscene_Init(globalCtx, 4140, -99, &self->actor, MAIN_CAM);
                    func_8002DF54(globalCtx, &self->actor, 1);
                    self->timer4 = 50;
                    self->actionFunc = EnDntNomal_SetupTargetUnburrow;
                }
            }
        } else if (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 24.0f) {
            scorePos.x = self->actor.world.pos.x;
            scorePos.y = self->actor.world.pos.y + 20.0f;
            scorePos.z = self->actor.world.pos.z;
            EffectSsExtra_Spawn(globalCtx, &scorePos, &scoreVel, &scoreAccel, 4, 0);
            self->hitCounter = 0;
        }
    }
}

void EnDntNomal_SetupTargetUnburrow(EnDntNomal* self, GlobalContext* globalCtx) {
    Vec3f spawnPos;

    if (self->timer4 == 0) {
        self->endFrame = (f32)Animation_GetLastFrame(&gHintNutsUnburrowAnim);
        Animation_Change(&self->skelAnime, &gHintNutsUnburrowAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
        spawnPos = self->actor.world.pos;
        spawnPos.y = self->actor.world.pos.y + 50.0f;
        EffectSsHahen_SpawnBurst(globalCtx, &spawnPos, 4.0f, 0, 10, 3, 15, HAHEN_OBJECT_DEFAULT, 10, NULL);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
        self->actionFunc = EnDntNomal_TargetUnburrow;
    }
}

void EnDntNomal_TargetUnburrow(EnDntNomal* self, GlobalContext* globalCtx) {
    f32 frame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if (frame >= self->endFrame) {
        self->actionFunc = EnDntNomal_SetupTargetWalk;
    }
}

void EnDntNomal_SetupTargetWalk(EnDntNomal* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gHintNutsRunAnim);
    Animation_Change(&self->skelAnime, &gHintNutsRunAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->actor.speedXZ = 1.0f;
    self->actor.colChkInfo.mass = 0;
    self->actionFunc = EnDntNomal_TargetWalk;
}

void EnDntNomal_TargetWalk(EnDntNomal* self, GlobalContext* globalCtx) {
    f32 dx;
    f32 dz;

    SkelAnime_Update(&self->skelAnime);
    dx = 1340.0f + 3.0f - self->actor.world.pos.x;
    dz = 0.0f - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(dx, dz) * (0x8000 / M_PI), 0x32, 0xBB8, 0);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 6.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }
    if (self->actor.world.pos.z > -30.0f) {
        self->actor.speedXZ = 0.0f;
        self->actionFunc = EnDntNomal_TargetFacePlayer;
    }
}

void EnDntNomal_TargetFacePlayer(EnDntNomal* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 6.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }
    if (fabsf(self->actor.shape.rot.y - self->actor.yawTowardsPlayer) < 30.0f) {
        self->actionFunc = EnDntNomal_SetupTargetTalk;
    }
}

void EnDntNomal_SetupTargetTalk(EnDntNomal* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gHintNutsTalkAnim);
    Animation_Change(&self->skelAnime, &gHintNutsTalkAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->actor.textId = 0x10AF;
    func_8010B680(globalCtx, self->actor.textId, NULL);
    self->actionFunc = EnDntNomal_TargetTalk;
}

void EnDntNomal_TargetTalk(EnDntNomal* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && func_80106BC8(globalCtx)) {
        func_80106CCC(globalCtx);
        func_8005B1A4(GET_ACTIVE_CAM(globalCtx));
        GET_ACTIVE_CAM(globalCtx)->csId = 0;
        func_8002DF54(globalCtx, NULL, 8);
        self->actionFunc = EnDntNomal_SetupTargetGivePrize;
    }
}

void EnDntNomal_SetupTargetGivePrize(EnDntNomal* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gHintNutsSpitAnim);
    Animation_Change(&self->skelAnime, &gHintNutsSpitAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
    self->actionFunc = EnDntNomal_TargetGivePrize;
}

void EnDntNomal_TargetGivePrize(EnDntNomal* self, GlobalContext* globalCtx) {
    f32 frame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if ((frame >= 8.0f) && !self->spawnedItem) {
        f32 itemX = self->mouthPos.x - 10.0f;
        f32 itemY = self->mouthPos.y;
        f32 itemZ = self->mouthPos.z;

        if (Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_EX_ITEM, itemX, itemY, itemZ, 0,
                               0, 0, EXITEM_BULLET_BAG) == NULL) {
            func_8002DF54(globalCtx, NULL, 7);
            Actor_Kill(&self->actor);
        }
        self->spawnedItem = true;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_THROW);
    }
    if (frame >= self->endFrame) {
        self->endFrame = (f32)Animation_GetLastFrame(&gHintNutsRunAnim);
        Animation_Change(&self->skelAnime, &gHintNutsRunAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
        self->actionFunc = EnDntNomal_TargetReturn;
    }
}

void EnDntNomal_TargetReturn(EnDntNomal* self, GlobalContext* globalCtx) {
    f32 dx;
    f32 dz;

    SkelAnime_Update(&self->skelAnime);
    dx = self->flowerPos.x - self->actor.world.pos.x;
    dz = -180.0f - self->actor.world.pos.z;

    Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(dx, dz) * (0x8000 / M_PI), 3, 0x1388, 0);
    if (fabsf(self->actor.shape.rot.y - (s16)(Math_FAtan2F(dx, dz) * (0x8000 / M_PI))) < 20.0f) {
        self->actor.speedXZ = 1.0f;
    }
    if (Animation_OnFrame(&self->skelAnime, 0.0f) || Animation_OnFrame(&self->skelAnime, 6.0f)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (self->actor.world.pos.z < -172.0f) {
        self->endFrame = (f32)Animation_GetLastFrame(&gHintNutsBurrowAnim);
        Animation_Change(&self->skelAnime, &gHintNutsBurrowAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
        self->actor.world.pos.z = -173.0f;
        self->actor.speedXZ = 0.0f;
        self->actionFunc = EnDntNomal_TargetBurrow;
    }
}

void EnDntNomal_TargetBurrow(EnDntNomal* self, GlobalContext* globalCtx) {
    f32 frame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if (frame >= self->endFrame) {
        self->actionFunc = EnDntNomal_SetupTargetWait;
    }
}

void EnDntNomal_SetupStageWait(EnDntNomal* self, GlobalContext* globalCtx) {
    if (self->timer3 == 0) {
        self->endFrame = (f32)Animation_GetLastFrame(&gDntStageHideAnim);
        Animation_Change(&self->skelAnime, &gDntStageHideAnim, 0.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
        self->skelAnime.curFrame = 8.0f;
        self->isSolid = false;
        self->actionFunc = EnDntNomal_StageWait;
    }
}

void EnDntNomal_StageWait(EnDntNomal* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
}

void EnDntNomal_SetupStageUp(EnDntNomal* self, GlobalContext* globalCtx) {
    if (self->timer3 == 0) {
        self->endFrame = (f32)Animation_GetLastFrame(&gDntStageUpAnim);
        Animation_Change(&self->skelAnime, &gDntStageUpAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
        if (self->action != DNT_ACTION_ATTACK) {
            self->rotDirection = -1;
        }
        EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 4.0f, 0, 10, 3, 15, HAHEN_OBJECT_DEFAULT, 10, NULL);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
        self->isSolid = true;
        self->actionFunc = EnDntNomal_StageUp;
    }
}

void EnDntNomal_StageUp(EnDntNomal* self, GlobalContext* globalCtx) {
    s16 rotTarget;
    f32 frame = self->skelAnime.curFrame;
    f32 turnMod;

    SkelAnime_Update(&self->skelAnime);
    if ((frame >= self->endFrame) && (self->action == DNT_ACTION_ATTACK)) {
        self->actionFunc = EnDntNomal_SetupStageAttack;
    } else {
        if (self->timer4 == 0) {
            turnMod = 0.0f;
            if (self->stagePrize == DNT_PRIZE_NONE) {
                Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
            } else {
                f32 dx = self->targetPos.x - self->actor.world.pos.x;
                f32 dz = self->targetPos.z - self->actor.world.pos.z;

                Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(dx, dz) * (0x8000 / M_PI), 1, 0xBB8, 0);
                turnMod = 90.0f;
            }
            if ((Rand_ZeroFloat(10.0f + turnMod) < 1.0f) && (self->action != DNT_ACTION_ATTACK)) {
                self->timer4 = (s16)Rand_ZeroFloat(30.0f) + 30;
            }
        } else {
            if (self->timer2 == 0) {
                self->rotDirection++;
                if (self->rotDirection > 1) {
                    self->rotDirection = -1;
                }
                self->timer2 = (s16)Rand_ZeroFloat(10.0f) + 10;
            }
            rotTarget = self->actor.yawTowardsPlayer;
            if (self->rotDirection != 0) {
                rotTarget += self->rotDirection * 0x1388;
            }
            Math_SmoothStepToS(&self->actor.shape.rot.y, rotTarget, 3, 0x1388, 0);
        }
        if (self->actor.xzDistToPlayer < 70.0f) {
            self->actionFunc = EnDntNomal_SetupStageHide;
        }
    }
}

void EnDntNomal_SetupStageUnburrow(EnDntNomal* self, GlobalContext* globalCtx) {
    if (self->timer3 == 0) {
        self->endFrame = (f32)Animation_GetLastFrame(&gDntStageUnburrowAnim);
        Animation_Change(&self->skelAnime, &gDntStageUnburrowAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
        self->isSolid = false;
        EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 4.0f, 0, 10, 3, 15, HAHEN_OBJECT_DEFAULT, 10, NULL);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
        self->actionFunc = EnDntNomal_StageUnburrow;
    }
}

void EnDntNomal_StageUnburrow(EnDntNomal* self, GlobalContext* globalCtx) {
    f32 frame = self->skelAnime.curFrame;

    SkelAnime_Update(&self->skelAnime);
    if (frame >= self->endFrame) {
        if (self->action != DNT_ACTION_DANCE) {
            self->timer3 = (s16)Rand_ZeroFloat(2.0f) + (s16)(self->type * 0.5f);
            self->actionFunc = EnDntNomal_SetupStageCelebrate;
        } else {
            self->timer2 = 300;
            self->actionFunc = EnDntNomal_SetupStageDance;
        }
    }
}

void EnDntNomal_SetupStageCelebrate(EnDntNomal* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntStageWalkAnim);
    Animation_Change(&self->skelAnime, &gDntStageWalkAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->actor.speedXZ = 3.0f;
    self->isSolid = true;
    self->actionFunc = EnDntNomal_StageCelebrate;
}

void EnDntNomal_StageCelebrate(EnDntNomal* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if ((self->timer1 == 0) && (self->timer3 == 0)) {
        f32 dx = self->targetPos.x - self->actor.world.pos.x;
        f32 dz = self->targetPos.z - self->actor.world.pos.z;

        if ((fabsf(dx) < 10.0f) && (fabsf(dz) < 10.0f) && func_8010BDBC(&globalCtx->msgCtx)) {
            self->action = DNT_ACTION_PRIZE;
            self->actionFunc = EnDntNomal_SetupStageDance;
            self->actor.speedXZ = 0.0f;
            return;
        }
        Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(dx, dz) * (0x8000 / M_PI), 1, 0xBB8, 0);
        self->actor.world.rot.y = self->actor.shape.rot.y;
    } else {
        if (self->timer1 == 1) {
            self->timer3 = (s16)Rand_ZeroFloat(20.0f) + 20.0f;
        }
        Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer, 0x14, 0x1388, 0);
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    }
    if (self->timer5 == 0) {
        self->timer5 = 20;
        if ((self->type & 1) == 0) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DAMAGE);
        }
    } else if ((self->timer5 & 3) == 0) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }
    if ((self->actor.bgCheckFlags & 8) && (self->actor.bgCheckFlags & 1)) {
        self->actor.velocity.y = 7.5f;
    }
}

void EnDntNomal_SetupStageDance(EnDntNomal* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntStageDanceAnim);
    Animation_Change(&self->skelAnime, &gDntStageDanceAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->isSolid = true;
    self->timer3 = (s16)Rand_ZeroFloat(20.0f) + 20.0f;
    self->rotDirection = -1;
    if (Rand_ZeroFloat(1.99f) < 1.0f) {
        self->rotDirection = 1;
    }
    self->actionFunc = EnDntNomal_StageDance;
}

void EnDntNomal_StageDance(EnDntNomal* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    if (self->timer2 == 0) {
        if (self->action == DNT_ACTION_DANCE) {
            self->action = DNT_ACTION_HIGH_RUPEES;
            self->actionFunc = EnDntNomal_SetupStageHide;
        } else {
            self->action = DNT_ACTION_NONE;
            self->actionFunc = EnDntNomal_StageSetupReturn;
        }
    } else if (self->timer3 != 0) {
        Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
        if (self->timer3 == 1) {
            self->timer4 = (s16)Rand_ZeroFloat(20.0f) + 20.0f;
            self->rotDirection = -self->rotDirection;
        }
    } else if (self->timer4 != 0) {
        self->actor.shape.rot.y += self->rotDirection * 0x800;
        if (self->timer4 == 1) {
            self->timer3 = (s16)Rand_ZeroFloat(20.0f) + 20.0f;
        }
    }
}

void EnDntNomal_SetupStageHide(EnDntNomal* self, GlobalContext* globalCtx) {
    if (self->timer3 != 0) {
        if ((self->timer3 == 1) && (self->ignore == 1)) {
            func_80078884(NA_SE_SY_ERROR);
        }
    } else {
        self->endFrame = (f32)Animation_GetLastFrame(&gDntStageHideAnim);
        Animation_Change(&self->skelAnime, &gDntStageHideAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
        self->isSolid = false;
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_DOWN);
        self->actionFunc = EnDntNomal_StageHide;
    }
}

void EnDntNomal_StageHide(EnDntNomal* self, GlobalContext* globalCtx) {
    EnExRuppy* rupee;
    f32 frame = self->skelAnime.curFrame;
    s16 rupeeColor;

    SkelAnime_Update(&self->skelAnime);
    if (frame >= self->endFrame) {
        EffectSsHahen_SpawnBurst(globalCtx, &self->actor.world.pos, 4.0f, 0, 10, 3, 15, HAHEN_OBJECT_DEFAULT, 10, NULL);
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_UP);
        switch (self->action) {
            case DNT_ACTION_NONE:
                self->actionFunc = EnDntNomal_SetupStageWait;
                break;
            case DNT_ACTION_ATTACK:
                self->actionFunc = EnDntNomal_StageAttackHide;
                break;
            case DNT_ACTION_LOW_RUPEES:
            case DNT_ACTION_HIGH_RUPEES:
                rupee =
                    (EnExRuppy*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_EX_RUPPY, self->actor.world.pos.x,
                                            self->actor.world.pos.y + 20.0f, self->actor.world.pos.z, 0, 0, 0, 3);
                if (rupee != NULL) {
                    rupeeColor = self->action - DNT_ACTION_LOW_RUPEES;
                    rupee->colorIdx = rupeeColor;
                    if (Rand_ZeroFloat(3.99f) < 1.0f) {
                        rupee->colorIdx = rupeeColor + 1;
                    }
                    rupee->actor.velocity.y = 5.0f;
                    if (rupee->colorIdx == 2) {
                        rupee->actor.velocity.y = 7.0f;
                    }
                    func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
                }
                self->action = DNT_ACTION_NONE;
                self->actionFunc = EnDntNomal_SetupStageWait;
                break;
        }
    }
}

void EnDntNomal_StageAttackHide(EnDntNomal* self, GlobalContext* globalCtx) {
    if (self->actor.xzDistToPlayer > 70.0f) {
        self->actionFunc = EnDntNomal_SetupStageUp;
    }
}

void EnDntNomal_SetupStageAttack(EnDntNomal* self, GlobalContext* globalCtx) {
    if (self->timer3 == 0) {
        self->endFrame = (f32)Animation_GetLastFrame(&gDntStageSpitAnim);
        Animation_Change(&self->skelAnime, &gDntStageSpitAnim, 1.0f, 0.0f, self->endFrame, ANIMMODE_ONCE, -10.0f);
        self->actor.colChkInfo.mass = 0xFF;
        self->isSolid = true;
        self->timer2 = 0;
        Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_ENEMY);
        self->actionFunc = EnDntNomal_StageAttack;
    }
}

void EnDntNomal_StageAttack(EnDntNomal* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* nut;
    f32 frame = self->skelAnime.curFrame;
    f32 dz;
    f32 dx;
    f32 dy;

    SkelAnime_Update(&self->skelAnime);
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 3, 0x1388, 0);
    dx = player->actor.world.pos.x - self->mouthPos.x;
    dy = player->actor.world.pos.y + 30.0f - self->mouthPos.y;
    dz = player->actor.world.pos.z - self->mouthPos.z;
    Math_SmoothStepToS(&self->actor.shape.rot.x, -(s16)(Math_FAtan2F(dy, sqrtf(SQ(dx) + SQ(dz))) * (0x8000 / M_PI)), 3,
                       0x1388, 0);
    if ((frame >= self->endFrame) && (self->timer2 == 0)) {
        self->timer2 = (s16)Rand_ZeroFloat(10.0f) + 10;
    }
    if (self->timer2 == 1) {
        self->spawnedItem = false;
        self->actionFunc = EnDntNomal_SetupStageAttack;
    } else if (self->actor.xzDistToPlayer < 50.0f) {
        self->action = DNT_ACTION_ATTACK;
        self->actionFunc = EnDntNomal_SetupStageHide;
    } else if ((frame >= 8.0f) && (!self->spawnedItem)) {
        Vec3f baseOffset;
        Vec3f spawnOffset;
        f32 spawnX;
        f32 spawnY;
        f32 spawnZ;

        Matrix_RotateY(self->actor.shape.rot.y / (f32)0x8000 * M_PI, MTXMODE_NEW);
        Matrix_RotateX(self->actor.shape.rot.x / (f32)0x8000 * M_PI, MTXMODE_APPLY);
        baseOffset.x = 0.0f;
        baseOffset.y = 0.0f;
        baseOffset.z = 5.0f;
        Matrix_MultVec3f(&baseOffset, &spawnOffset);
        spawnX = self->mouthPos.x + spawnOffset.x;
        spawnY = self->mouthPos.y + spawnOffset.y;
        spawnZ = self->mouthPos.z + spawnOffset.z;

        nut = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_NUTSBALL, spawnX, spawnY, spawnZ,
                          self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z, 4);
        if (nut != NULL) {
            nut->velocity.y = spawnOffset.y * 0.5f;
        }
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_THROW);
        self->spawnedItem = true;
    }
}

void EnDntNomal_StageSetupReturn(EnDntNomal* self, GlobalContext* globalCtx) {
    self->endFrame = (f32)Animation_GetLastFrame(&gDntStageWalkAnim);
    Animation_Change(&self->skelAnime, &gDntStageWalkAnim, 1.5f, 0.0f, self->endFrame, ANIMMODE_LOOP, -10.0f);
    self->actor.speedXZ = 4.0f;
    self->isSolid = false;
    self->actionFunc = EnDntNomal_StageReturn;
}

void EnDntNomal_StageReturn(EnDntNomal* self, GlobalContext* globalCtx) {
    f32 sp2C;
    f32 sp28;

    SkelAnime_Update(&self->skelAnime);
    sp2C = self->flowerPos.x - self->actor.world.pos.x;
    sp28 = self->flowerPos.z - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.shape.rot.y, Math_FAtan2F(sp2C, sp28) * (0x8000 / M_PI), 1, 0xBB8, 0);
    if (self->timer5 == 0) {
        self->timer5 = 10;
    } else if (!(self->timer5 & 1)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EN_NUTS_WALK);
    }
    if ((fabsf(sp2C) < 7.0f) && (fabsf(sp28) < 7.0f)) {
        self->actor.world.pos.x = self->flowerPos.x;
        self->actor.world.pos.z = self->flowerPos.z;
        self->actor.speedXZ = 0.0f;
        self->actionFunc = EnDntNomal_SetupStageHide;
    }
}

void EnDntNomal_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnDntNomal* self = THIS;

    if (self->timer1 != 0) {
        self->timer1--;
    }
    if (self->timer2 != 0) {
        self->timer2--;
    }
    if (self->timer3 != 0) {
        self->timer3--;
    }
    if (self->timer4 != 0) {
        self->timer4--;
    }
    if (self->timer5 != 0) {
        self->timer5--;
    }
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    }
    self->actor.world.rot.x = self->actor.shape.rot.x;
    if (self->actionFunc != EnDntNomal_StageCelebrate) {
        self->actor.world.rot.y = self->actor.shape.rot.y;
    }
    self->unkCounter++;
    if (self->type != ENDNTNOMAL_TARGET) {
        switch (self->stageSignal) {
            case DNT_SIGNAL_LOOK:
                if (self->stagePrize == DNT_PRIZE_NONE) {
                    self->actionFunc = EnDntNomal_SetupStageUp;
                } else {
                    self->actionFunc = EnDntNomal_StageUp;
                }
                break;
            case DNT_SIGNAL_CELEBRATE:
                self->action = DNT_ACTION_NONE;
                self->actor.colChkInfo.mass = 0;
                self->timer3 = (s16)Rand_ZeroFloat(3.0f) + (s16)(self->type * 0.5f);
                self->actionFunc = EnDntNomal_SetupStageUnburrow;
                break;
            case DNT_SIGNAL_DANCE:
                self->action = DNT_ACTION_DANCE;
                self->actionFunc = EnDntNomal_SetupStageUnburrow;
                break;
            case DNT_SIGNAL_HIDE:
                self->actionFunc = EnDntNomal_SetupStageHide;
                break;
            case DNT_SIGNAL_RETURN:
                self->actionFunc = EnDntNomal_StageSetupReturn;
                break;
            case DNT_SIGNAL_UNUSED:
                self->actionFunc = EnDntNomal_SetupStageDance;
                break;
            case DNT_SIGNAL_NONE:
                break;
        }
    }
    if (self->stageSignal != DNT_SIGNAL_NONE) {
        self->stageSignal = DNT_SIGNAL_NONE;
    }
    if (self->blinkTimer == 0) {
        self->eyeState++;
        if (self->eyeState >= 3) {
            self->eyeState = 0;
            self->blinkTimer = (s16)Rand_ZeroFloat(60.0f) + 20;
        }
    }
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 60.0f, 0x1D);
    if (self->type == ENDNTNOMAL_TARGET) {
        Collider_SetQuadVertices(&self->targetQuad, &self->targetVtx[0], &self->targetVtx[1], &self->targetVtx[2],
                                 &self->targetVtx[3]);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->targetQuad.base);
    } else {
        Collider_UpdateCylinder(&self->actor, &self->bodyCyl);
        if (self->isSolid) {
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->bodyCyl.base);
        }
    }
}

s32 EnDntNomal_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx) {
    EnDntNomal* self = THIS;

    if ((limbIndex == 1) || (limbIndex == 3) || (limbIndex == 4) || (limbIndex == 5) || (limbIndex == 6)) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dnt_nomal.c", 1733);
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, sLeafColors[self->type - ENDNTNOMAL_STAGE].r,
                       sLeafColors[self->type - ENDNTNOMAL_STAGE].g, sLeafColors[self->type - ENDNTNOMAL_STAGE].b, 255);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dnt_nomal.c", 1743);
    }
    return false;
}

void EnDntNomal_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnDntNomal* self = THIS;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    if (self->type == ENDNTNOMAL_TARGET) {
        if (limbIndex == 5) {
            Matrix_MultVec3f(&zeroVec, &self->mouthPos);
        }
    } else if (limbIndex == 7) {
        Matrix_MultVec3f(&zeroVec, &self->mouthPos);
    }
}

void EnDntNomal_DrawStageScrub(Actor* thisx, GlobalContext* globalCtx) {
    static void* blinkTex[] = { &gDntStageEyeOpenTex, &gDntStageEyeHalfTex, &gDntStageEyeShutTex };
    EnDntNomal* self = THIS;
    Vec3f dustScale = { 0.25f, 0.25f, 0.25f };
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dnt_nomal.c", 1790);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(blinkTex[self->eyeState]));
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, EnDntNomal_OverrideLimbDraw,
                      EnDntNomal_PostLimbDraw, self);
    Matrix_Translate(self->flowerPos.x, self->flowerPos.y, self->flowerPos.z, MTXMODE_NEW);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetEnvColor(POLY_OPA_DISP++, sLeafColors[self->type - ENDNTNOMAL_STAGE].r,
                   sLeafColors[self->type - ENDNTNOMAL_STAGE].g, sLeafColors[self->type - ENDNTNOMAL_STAGE].b, 255);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dnt_nomal.c", 1814),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gDntStageFlowerDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dnt_nomal.c", 1817);
    if (self->actionFunc == EnDntNomal_StageCelebrate) {
        func_80033C30(&self->actor.world.pos, &dustScale, 255, globalCtx);
    }
}

void EnDntNomal_DrawTargetScrub(Actor* thisx, GlobalContext* globalCtx) {
    EnDntNomal* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_dnt_nomal.c", 1833);
    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, EnDntNomal_PostLimbDraw,
                      self);
    Matrix_Translate(self->flowerPos.x, self->flowerPos.y, self->flowerPos.z, MTXMODE_NEW);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_dnt_nomal.c", 1848),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gHintNutsFlowerDL);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_dnt_nomal.c", 1851);
}
