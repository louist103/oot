/*
 * File: z_en_xc.c
 * Overlay: ovl_En_Xc
 * Description: Sheik
 */

#include "z_en_xc.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "objects/object_xc/object_xc.h"
#include "scenes/overworld/spot05/spot05_scene.h"
#include "scenes/overworld/spot17/spot17_scene.h"
#include "scenes/indoors/tokinoma/tokinoma_scene.h"
#include "scenes/dungeons/ice_doukutu/ice_doukutu_scene.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((EnXc*)thisx)

void EnXc_Init(Actor* thisx, GlobalContext* globalCtx);
void EnXc_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnXc_Update(Actor* thisx, GlobalContext* globalCtx);
void EnXc_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnXc_DrawNothing(Actor* thisx, GlobalContext* globalCtx);
void EnXc_DrawDefault(Actor* thisx, GlobalContext* globalCtx);
void EnXc_DrawPullingOutHarp(Actor* thisx, GlobalContext* globalCtx);
void EnXc_DrawHarp(Actor* thisx, GlobalContext* globalCtx);
void EnXc_DrawTriforce(Actor* thisx, GlobalContext* globalCtx);
void EnXc_DrawSquintingEyes(Actor* thisx, GlobalContext* globalCtx);

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_HIT0,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_PLAYER,
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
    { 25, 80, 0, { 0, 0, 0 } },
};

static void* sEyeTextures[] = {
    gSheikEyeOpenTex,
    gSheikEyeHalfClosedTex,
    gSheikEyeShutTex,
};

void EnXc_InitCollider(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;

    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit);
}

void EnXc_UpdateCollider(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;
    Collider* colliderBase = &self->collider.base;
    s32 pad[3];

    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, colliderBase);
}

void EnXc_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnXc_CalculateHeadTurn(EnXc* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->npcInfo.unk_18 = player->actor.world.pos;
    self->npcInfo.unk_14 = kREG(16) - 3.0f;
    func_80034A14(&self->actor, &self->npcInfo, kREG(17) + 0xC, 2);
}

void EnXc_SetEyePattern(EnXc* self) {
    s32 pad[3];
    s16* blinkTimer = &self->blinkTimer;
    s16* eyePattern = &self->eyeIdx;

    if (!DECR(*blinkTimer)) {
        *blinkTimer = Rand_S16Offset(60, 60);
    }

    *eyePattern = *blinkTimer;
    if (*eyePattern >= ARRAY_COUNT(sEyeTextures)) {
        *eyePattern = 0;
    }
}

void EnXc_SpawnNut(EnXc* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f* pos = &self->actor.world.pos;
    s16 angle = self->actor.shape.rot.y;
    f32 x = (Math_SinS(angle) * 30.0f) + pos->x;
    f32 y = pos->y + 3.0f;
    f32 z = (Math_CosS(angle) * 30.0f) + pos->z;

    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_ARROW, x, y, z, 0xFA0, self->actor.shape.rot.y, 0,
                ARROW_CS_NUT);
}

void EnXc_BgCheck(EnXc* self, GlobalContext* globalCtx) {
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 75.0f, 30.0f, 30.0f, 4);
}

s32 EnXc_AnimIsFinished(EnXc* self) {
    return SkelAnime_Update(&self->skelAnime);
}

CsCmdActorAction* EnXc_GetCsCmd(GlobalContext* globalCtx, s32 npcActionIdx) {
    CsCmdActorAction* action = NULL;

    if (globalCtx->csCtx.state != 0) {
        action = globalCtx->csCtx.npcActions[npcActionIdx];
    }
    return action;
}

s32 EnXc_CompareCsAction(EnXc* self, GlobalContext* globalCtx, u16 action, s32 npcActionIdx) {
    CsCmdActorAction* csCmdActorAction = EnXc_GetCsCmd(globalCtx, npcActionIdx);

    if (csCmdActorAction != NULL && csCmdActorAction->action == action) {
        return true;
    }
    return false;
}

s32 EnXc_CsActionsAreNotEqual(EnXc* self, GlobalContext* globalCtx, u16 action, s32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction = EnXc_GetCsCmd(globalCtx, npcActionIdx);

    if (csCmdNPCAction && csCmdNPCAction->action != action) {
        return true;
    }
    return false;
}

void func_80B3C588(EnXc* self, GlobalContext* globalCtx, u32 npcActionIdx) {
    CsCmdActorAction* csCmdNPCAction = EnXc_GetCsCmd(globalCtx, npcActionIdx);
    Actor* thisx = &self->actor;

    if (csCmdNPCAction != NULL) {
        thisx->world.pos.x = csCmdNPCAction->startPos.x;
        thisx->world.pos.y = csCmdNPCAction->startPos.y;
        thisx->world.pos.z = csCmdNPCAction->startPos.z;
        thisx->world.rot.x = thisx->shape.rot.x = csCmdNPCAction->rot.x;
        thisx->world.rot.y = thisx->shape.rot.y = csCmdNPCAction->rot.y;
        thisx->world.rot.z = thisx->shape.rot.z = csCmdNPCAction->rot.z;
    }
}

void func_80B3C620(EnXc* self, GlobalContext* globalCtx, s32 npcActionIdx) {
    CsCmdActorAction* npcAction = EnXc_GetCsCmd(globalCtx, npcActionIdx);
    Vec3f* xcPos = &self->actor.world.pos;
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 endX;
    f32 endY;
    f32 endZ;
    f32 unk;

    if (npcAction != NULL) {
        unk =
            Environment_LerpWeightAccelDecel(npcAction->endFrame, npcAction->startFrame, globalCtx->csCtx.frames, 0, 0);
        startX = npcAction->startPos.x;
        startY = npcAction->startPos.y;
        startZ = npcAction->startPos.z;
        endX = npcAction->endPos.x;
        endY = npcAction->endPos.y;
        endZ = npcAction->endPos.z;
        xcPos->x = ((endX - startX) * unk) + startX;
        xcPos->y = ((endY - startY) * unk) + startY;
        xcPos->z = ((endZ - startZ) * unk) + startZ;
    }
}

void EnXc_ChangeAnimation(EnXc* self, AnimationHeader* animation, u8 mode, f32 morphFrames, s32 reverseFlag) {
    s32 pad[2];
    AnimationHeader* animationSeg = SEGMENTED_TO_VIRTUAL(animation);
    f32 frameCount = Animation_GetLastFrame(&animationSeg->common);
    f32 playbackSpeed;
    f32 startFrame;
    f32 endFrame;

    if (!reverseFlag) {
        startFrame = 0.0f;
        endFrame = frameCount;
        playbackSpeed = 1.0f;
    } else {
        startFrame = frameCount;
        endFrame = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change(&self->skelAnime, animationSeg, playbackSpeed, startFrame, endFrame, mode, morphFrames);
}

void EnXc_CheckAndSetAction(EnXc* self, s32 check, s32 set) {
    if (check != self->action) {
        self->action = set;
    }
}

void func_80B3C7D4(EnXc* self, s32 action1, s32 action2, s32 action3) {
    if (action1 != self->action) {
        if (self->action == SHEIK_ACTION_PUT_HARP_AWAY) {
            self->action = action2;
        } else {
            self->action = action3;
        }
    }
}

s32 EnXc_NoCutscenePlaying(GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state == 0) {
        return true;
    }
    return false;
}

void func_80B3C820(EnXc* self) {
    Animation_Change(&self->skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gSheikIdleAnim),
                     ANIMMODE_LOOP, 0.0f);
    self->action = SHEIK_ACTION_53;
}

void func_80B3C888(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_NoCutscenePlaying(globalCtx) && self->actor.params == SHEIK_TYPE_4) {
        func_80B3C820(self);
    }
}

void func_80B3C8CC(EnXc* self, GlobalContext* globalCtx) {
    SkelAnime* skelAnime = &self->skelAnime;

    if (skelAnime->jointTable[0].y >= skelAnime->baseTransl.y) {
        skelAnime->moveFlags |= 3;
        AnimationContext_SetMoveActor(globalCtx, &self->actor, skelAnime, 1.0f);
    }
}

void func_80B3C924(EnXc* self, GlobalContext* globalCtx) {
    self->skelAnime.moveFlags |= 3;
    AnimationContext_SetMoveActor(globalCtx, &self->actor, &self->skelAnime, 1.0f);
}

void func_80B3C964(EnXc* self, GlobalContext* globalCtx) {
    self->skelAnime.baseTransl = self->skelAnime.jointTable[0];
    self->skelAnime.prevTransl = self->skelAnime.jointTable[0];
    self->skelAnime.moveFlags |= 3;
    AnimationContext_SetMoveActor(globalCtx, &self->actor, &self->skelAnime, 1.0f);
}

void func_80B3C9DC(EnXc* self) {
    self->skelAnime.moveFlags &= ~0x3;
}

void func_80B3C9EC(EnXc* self) {
    EnXc_ChangeAnimation(self, &gSheikArmsCrossedIdleAnim, ANIMMODE_LOOP, 0.0f, false);
    self->action = SHEIK_ACTION_BLOCK_PEDESTAL;
    self->drawMode = SHEIK_DRAW_DEFAULT;
    self->unk_30C = 1;
}

void func_80B3CA38(EnXc* self, GlobalContext* globalCtx) {
    // If Player is adult but hasn't learned Minuet of Forest
    if (!(gSaveContext.eventChkInf[5] & 1) && LINK_IS_ADULT) {
        self->action = SHEIK_ACTION_INIT;
    } else {
        Actor_Kill(&self->actor);
    }
}

s32 EnXc_MinuetCS(EnXc* self, GlobalContext* globalCtx) {
    if (self->actor.params == SHEIK_TYPE_MINUET) {
        Player* player = GET_PLAYER(globalCtx);
        f32 z = player->actor.world.pos.z;
        if (z < -2225.0f) {
            if (!Gameplay_InCsMode(globalCtx)) {
                globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(&gMinuetCs);
                gSaveContext.cutsceneTrigger = 1;
                gSaveContext.eventChkInf[5] |= 1;
                Item_Give(globalCtx, ITEM_SONG_MINUET);
                return true;
            }
        }
        return false;
    }
    return true;
}

void func_80B3CB58(EnXc* self, GlobalContext* globalCtx) {
    // If hasn't learned Bolero and Player is Adult
    if (!(gSaveContext.eventChkInf[5] & 2) && LINK_IS_ADULT) {
        self->action = SHEIK_ACTION_INIT;
    } else {
        Actor_Kill(&self->actor);
    }
}

s32 EnXc_BoleroCS(EnXc* self, GlobalContext* globalCtx) {
    Player* player;
    PosRot* posRot;

    if (self->actor.params == SHEIK_TYPE_BOLERO) {
        player = GET_PLAYER(globalCtx);
        posRot = &player->actor.world;
        if ((posRot->pos.x > -784.0f) && (posRot->pos.x < -584.0f) && (posRot->pos.y > 447.0f) &&
            (posRot->pos.y < 647.0f) && (posRot->pos.z > -446.0f) && (posRot->pos.z < -246.0f) &&
            !Gameplay_InCsMode(globalCtx)) {
            globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(&gBoleroCs);
            gSaveContext.cutsceneTrigger = 1;
            gSaveContext.eventChkInf[5] |= 2;
            Item_Give(globalCtx, ITEM_SONG_BOLERO);
            return true;
        }
        return false;
    }
    return true;
}

void EnXc_SetupSerenadeAction(EnXc* self, GlobalContext* globalCtx) {
    // Player is adult and does not have iron boots and has not learned Serenade
    if ((!CHECK_OWNED_EQUIP(EQUIP_BOOTS, 1) && !(gSaveContext.eventChkInf[5] & 4)) && LINK_IS_ADULT) {
        self->action = SHEIK_ACTION_SERENADE;
        osSyncPrintf("水のセレナーデ シーク誕生!!!!!!!!!!!!!!!!!!\n");
    } else {
        Actor_Kill(&self->actor);
        osSyncPrintf("水のセレナーデ シーク消滅!!!!!!!!!!!!!!!!!!\n");
    }
}

s32 EnXc_SerenadeCS(EnXc* self, GlobalContext* globalCtx) {
    if (self->actor.params == SHEIK_TYPE_SERENADE) {
        Player* player = GET_PLAYER(globalCtx);
        s32 stateFlags = player->stateFlags1;
        if (CHECK_OWNED_EQUIP(EQUIP_BOOTS, 1) && !(gSaveContext.eventChkInf[5] & 4) && !(stateFlags & 0x20000000) &&
            !Gameplay_InCsMode(globalCtx)) {
            Cutscene_SetSegment(globalCtx, &gIceCavernSerenadeCs);
            gSaveContext.cutsceneTrigger = 1;
            gSaveContext.eventChkInf[5] |= 4; // Learned Serenade of Water Flag
            Item_Give(globalCtx, ITEM_SONG_SERENADE);
            osSyncPrintf("ブーツを取った!!!!!!!!!!!!!!!!!!\n");
            return true;
        }
        osSyncPrintf("はやくブーツを取るべし!!!!!!!!!!!!!!!!!!\n");
        return false;
    }
    return true;
}

void EnXc_DoNothing(EnXc* self, GlobalContext* globalCtx) {
}

void EnXc_SetWalkingSFX(EnXc* self, GlobalContext* globalCtx) {
    s32 pad[2];
    u32 sfxId;
    s32 pad2;

    if (Animation_OnFrame(&self->skelAnime, 11.0f) || Animation_OnFrame(&self->skelAnime, 23.0f)) {
        if (self->actor.bgCheckFlags & 1) {
            sfxId = SFX_FLAG;
            sfxId += SurfaceType_GetSfx(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);
            func_80078914(&self->actor.projectedPos, sfxId);
        }
    }
}

void EnXc_SetNutThrowSFX(EnXc* self, GlobalContext* globalCtx) {
    s32 pad[2];
    u32 sfxId;
    s32 pad2;

    if (Animation_OnFrame(&self->skelAnime, 7.0f)) {
        if (self->actor.bgCheckFlags & 1) {
            sfxId = SFX_FLAG;
            sfxId += SurfaceType_GetSfx(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);
            func_80078914(&self->actor.projectedPos, sfxId);
        }
    }
    if (Animation_OnFrame(&self->skelAnime, 20.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_SK_SHOUT);
    }
}

void EnXc_SetLandingSFX(EnXc* self, GlobalContext* globalCtx) {
    u32 sfxId;
    s16 sceneNum = globalCtx->sceneNum;

    if ((gSaveContext.sceneSetupIndex != 4) || (sceneNum != SCENE_SPOT11)) {
        if (Animation_OnFrame(&self->skelAnime, 11.0f)) {
            sfxId = SFX_FLAG;
            sfxId += SurfaceType_GetSfx(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);
            func_80078914(&self->actor.projectedPos, sfxId);
        }
    }
}

void EnXc_SetColossusAppearSFX(EnXc* self, GlobalContext* globalCtx) {
    static Vec3f sXyzDist;
    s16 sceneNum;

    if (gSaveContext.sceneSetupIndex == 4) {
        sceneNum = globalCtx->sceneNum;
        if (sceneNum == SCENE_SPOT11) {
            CutsceneContext* csCtx = &globalCtx->csCtx;
            u16 frameCount = csCtx->frames;
            f32 wDest[2];

            if (frameCount == 119) {
                Vec3f pos = { -611.0f, 728.0f, -2.0f };

                SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, &pos, &sXyzDist, wDest);
                func_80078914(&sXyzDist, NA_SE_EV_JUMP_CONC);
            } else if (frameCount == 164) {
                Vec3f pos = { -1069.0f, 38.0f, 0.0f };
                s32 pad;

                SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, &pos, &sXyzDist, wDest);
                func_80078914(&sXyzDist, NA_SE_PL_WALK_CONCRETE);
            }
        }
    }
}

void func_80B3D118(GlobalContext* globalCtx) {
    s16 sceneNum;

    if ((gSaveContext.sceneSetupIndex != 4) || (sceneNum = globalCtx->sceneNum, sceneNum != SCENE_SPOT11)) {
        func_800788CC(NA_SE_PL_SKIP);
    }
}

static Vec3f D_80B42DA0;

void EnXc_SetColossusWindSFX(GlobalContext* globalCtx) {
    if (gSaveContext.sceneSetupIndex == 4) {
        static s32 D_80B41D90 = 0;
        static Vec3f sPos = { 0.0f, 0.0f, 0.0f };
        static f32 sMaxSpeed = 0.0f;
        static Vec3f D_80B42DB0;
        s32 pad;
        s16 sceneNum = globalCtx->sceneNum;

        if (sceneNum == SCENE_SPOT11) {
            CutsceneContext* csCtx = &globalCtx->csCtx;
            u16 frameCount = csCtx->frames;

            if ((frameCount >= 120) && (frameCount < 164)) {
                s32 pad;
                Vec3f* eye = &globalCtx->view.eye;

                if (D_80B41D90 != 0) {
                    f32 speed = Math3D_Vec3f_DistXYZ(&D_80B42DB0, eye) / 7.058922f;

                    sMaxSpeed = CLAMP_MIN(sMaxSpeed, speed);

                    osSyncPrintf("MAX speed = %f\n", sMaxSpeed);

                    speed = CLAMP_MAX(speed, 2.0f);
                    func_800F436C(&sPos, NA_SE_EV_FLYING_AIR - SFX_FLAG, 0.6f + (0.4f * speed));
                }

                D_80B42DB0.x = eye->x;
                D_80B42DB0.y = eye->y;
                D_80B42DB0.z = eye->z;
                D_80B41D90 = 1;
            }
        }
    }
}

void EnXc_SpawnFlame(EnXc* self, GlobalContext* globalCtx) {
    static s32 sFlameSpawned = false;

    if (!sFlameSpawned) {
        CsCmdActorAction* npcAction = EnXc_GetCsCmd(globalCtx, 0);
        f32 xPos = npcAction->startPos.x;
        f32 yPos = npcAction->startPos.y;
        f32 zPos = npcAction->startPos.z;
        self->flameActor = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_LIGHT, xPos, yPos, zPos, 0, 0, 0, 5);
        sFlameSpawned = true;
    }
}

void EnXc_SetupFlamePos(EnXc* self, GlobalContext* globalCtx) {
    Vec3f* attachedPos;
    CsCmdActorAction* npcAction = EnXc_GetCsCmd(globalCtx, 0);

    if (self->flameActor != NULL) {
        attachedPos = &self->flameActor->world.pos;
        if (!self) {}
        attachedPos->x = npcAction->startPos.x;
        attachedPos->y = npcAction->startPos.y;
        attachedPos->z = npcAction->startPos.z;
    }
}

void EnXc_DestroyFlame(EnXc* self) {
    if (self->flameActor != NULL) {
        Actor_Kill(self->flameActor);
        self->flameActor = NULL;
    }
    Actor_Kill(&self->actor);
}

void EnXc_InitFlame(EnXc* self, GlobalContext* globalCtx) {
    static s32 D_80B41DA8 = 1;
    s32 pad;
    s16 sceneNum = globalCtx->sceneNum;

    if (sceneNum == SCENE_SPOT17) {
        CsCmdActorAction* npcAction = EnXc_GetCsCmd(globalCtx, 0);
        if (npcAction != NULL) {
            s32 action = npcAction->action;
            if (D_80B41DA8 != action) {
                if (action != 1) {
                    EnXc_SpawnFlame(self, globalCtx);
                }

                if (action == 1) {
                    EnXc_DestroyFlame(self);
                }

                D_80B41DA8 = action;
            }

            EnXc_SetupFlamePos(self, globalCtx);
        }
    }
}

void func_80B3D48C(EnXc* self, GlobalContext* globalCtx) {
    CutsceneContext* csCtx = &globalCtx->csCtx;
    CsCmdActorAction* linkAction = csCtx->linkAction;
    s16 yaw;

    if (linkAction != NULL) {
        yaw = linkAction->urot.y + 0x8000;
    } else {
        Player* player = GET_PLAYER(globalCtx);
        yaw = player->actor.world.rot.y + 0x8000;
    }

    self->actor.shape.rot.y = self->actor.world.rot.y = yaw;
}

AnimationHeader* EnXc_GetCurrentHarpAnim(GlobalContext* globalCtx, s32 index) {
    AnimationHeader* animation = &gSheikPlayingHarp5Anim;
    CsCmdActorAction* npcAction = EnXc_GetCsCmd(globalCtx, index);

    if (npcAction != NULL) {
        u16 action = npcAction->action;
        if (action == 11) {
            animation = &gSheikPlayingHarp3Anim;
        } else if (action == 12) {
            animation = &gSheikPlayingHarp2Anim;
        } else if (action == 13) {
            animation = &gSheikPlayingHarp4Anim;
        } else if (action == 23) {
            animation = &gSheikPlayingHarpAnim;
        } else {
            animation = &gSheikPlayingHarp5Anim;
        }
    }
    return animation;
}

void EnXc_CalcXZAccel(EnXc* self) {
    f32 timer = self->timer;
    f32* speedXZ = &self->actor.speedXZ;

    if (timer < 9.0f) {
        *speedXZ = 0.0f;
    } else if (timer < 3.0f) {
        *speedXZ = (((kREG(2) * 0.01f) + 1.2f) / 3.0f) * (timer - 9.0f);
    } else {
        *speedXZ = (kREG(2) * 0.01f) + 1.2f;
    }

    Actor_MoveForward(&self->actor);
}

void func_80B3D644(EnXc* self) {
    Actor_MoveForward(&self->actor);
}

void EnXc_CalcXZSpeed(EnXc* self) {
    f32 timer = self->timer;
    f32* speedXZ = &self->actor.speedXZ;

    if (timer < 3.0f) {
        *speedXZ = (((kREG(2) * 0.01f) + 1.2f) / 3.0f) * (3.0f - timer);
    } else {
        *speedXZ = 0.0f;
    }
    Actor_MoveForward(&self->actor);
}

void func_80B3D6F0(EnXc* self) {
    EnXc_CalcXZAccel(self);
}

void func_80B3D710(EnXc* self) {
    Actor_MoveForward(&self->actor);
}

void func_80B3D730(EnXc* self) {
    EnXc_CalcXZSpeed(self);
}

void func_80B3D750(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_MinuetCS(self, globalCtx) && EnXc_BoleroCS(self, globalCtx)) {
        self->action = SHEIK_ACTION_WAIT;
    }
}

void EnXc_SetupFallFromSkyAction(EnXc* self, GlobalContext* globalCtx) {
    s32 pad;
    CutsceneContext* csCtx = &globalCtx->csCtx;

    if (csCtx->state != 0) {
        CsCmdActorAction* npcAction = csCtx->npcActions[4];

        if (npcAction && npcAction->action == 2) {
            s32 pad;
            Vec3f* pos = &self->actor.world.pos;
            SkelAnime* skelAnime = &self->skelAnime;
            f32 frameCount = Animation_GetLastFrame(&gSheikFallingFromSkyAnim);

            self->action = SHEIK_ACTION_GRACEFUL_FALL;
            self->drawMode = SHEIK_DRAW_DEFAULT;

            pos->x = npcAction->startPos.x;
            pos->y = npcAction->startPos.y;
            pos->z = npcAction->startPos.z;

            func_80B3D48C(self, globalCtx);
            func_80B3C964(self, globalCtx);
            Animation_Change(skelAnime, &gSheikFallingFromSkyAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, 0.0f);
            func_80B3D118(globalCtx);
        }
    }
}

void func_80B3D8A4(EnXc* self, GlobalContext* globalCtx, s32 animFinished) {
    if (animFinished) {
        SkelAnime* skelAnime = &self->skelAnime;
        f32 frameCount = Animation_GetLastFrame(&gSheikWalkingAnim);

        Animation_Change(skelAnime, &gSheikWalkingAnim, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP, -8.0f);

        self->action = SHEIK_ACTION_ACCEL;
        self->timer = 0.0f;

        func_80B3C9DC(self);
        self->actor.gravity = -((kREG(1) * 0.01f) + 13.0f);
        self->actor.minVelocityY = -((kREG(1) * 0.01f) + 13.0f);
    } else {
        func_80B3C8CC(self, globalCtx);
    }
}

void EnXc_SetupWalkAction(EnXc* self) {
    f32* timer = &self->timer;

    *timer += 1.0f;
    if (*timer >= 12.0f) {
        self->actor.speedXZ = (kREG(2) * 0.01f) + 1.2f;
        self->action = SHEIK_ACTION_WALK;
    }
}

void EnXc_SetupHaltAction(EnXc* self) {
    SkelAnime* skelAnime = &self->skelAnime;
    f32 xzDistToPlayer = self->actor.xzDistToPlayer;

    if (xzDistToPlayer <= (kREG(3) + 95.0f)) {
        f32 frameCount = Animation_GetLastFrame(&gSheikIdleAnim);

        Animation_Change(skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP, -12.0f);
        self->action = SHEIK_ACTION_HALT;
        self->timer = 0.0f;
    }
}

void EnXc_SetupStoppedAction(EnXc* self) {
    f32* timer = &self->timer;

    *timer += 1.0f;
    if (*timer >= 12.0f) {
        self->action = SHEIK_ACTION_STOPPED;
        self->actor.speedXZ = 0.0f;
    }
}

void func_80B3DAF0(EnXc* self, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = EnXc_GetCsCmd(globalCtx, 4);
    u16 action;

    if (npcAction &&
        (action = npcAction->action, action == 3 || action == 11 || action == 12 || action == 13 || action == 23)) {
        f32 frameCount;

        frameCount = Animation_GetLastFrame(&gSheikPullingOutHarpAnim);
        Animation_Change(&self->skelAnime, &gSheikPullingOutHarpAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, -4.0f);
        self->action = SHEIK_ACTION_7;
        self->drawMode = SHEIK_DRAW_PULLING_OUT_HARP;
    }
}

void EnXc_SetupInitialHarpAction(EnXc* self, s32 animFinished) {
    SkelAnime* skelAnime;
    f32 frameCount;

    if (animFinished) {
        skelAnime = &self->skelAnime;
        frameCount = Animation_GetLastFrame(&gSheikInitialHarpAnim);
        Animation_Change(skelAnime, &gSheikInitialHarpAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, 0.0f);
        self->action = SHEIK_ACTION_HARP_READY;
        self->drawMode = SHEIK_DRAW_HARP;
    }
}

void EnXc_SetupPlayingHarpAction(EnXc* self, GlobalContext* globalCtx, s32 animFinished) {
    s32 pad;
    SkelAnime* skelAnime;
    AnimationHeader* animation;
    f32 frameCount;

    if (animFinished) {
        skelAnime = &self->skelAnime;
        animation = EnXc_GetCurrentHarpAnim(globalCtx, 4);
        frameCount = Animation_GetLastFrame(animation);
        Animation_Change(skelAnime, animation, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP, -8.0f);
        self->action = SHEIK_PLAYING_HARP;
        self->drawMode = SHEIK_DRAW_HARP;
    }
}

void func_80B3DCA8(EnXc* self, GlobalContext* globalCtx) {
    f32 frameCount;

    if (globalCtx->csCtx.state != 0) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[4];

        if (npcAction != NULL && npcAction->action == 8) {
            frameCount = Animation_GetLastFrame(&gSheikInitialHarpAnim);
            Animation_Change(&self->skelAnime, &gSheikInitialHarpAnim, 0.0f, frameCount, frameCount, ANIMMODE_LOOP,
                             -8.0f);
            self->action = SHEIK_ACTION_10;
        }
    }
}

void EnXc_SetupHarpPutawayAction(EnXc* self, GlobalContext* globalCtx) {
    f32 curFrame;
    f32 animFrameCount;

    if (EnXc_CompareCsAction(self, globalCtx, 5, 4)) {
        curFrame = self->skelAnime.curFrame;
        animFrameCount = self->skelAnime.endFrame;
        if (curFrame >= animFrameCount) {
            Animation_Change(&self->skelAnime, &gSheikInitialHarpAnim, -1.0f,
                             Animation_GetLastFrame(&gSheikInitialHarpAnim), 0.0f, ANIMMODE_ONCE, 0.0f);
            self->action = SHEIK_ACTION_PUT_HARP_AWAY;
        }
    } else if (EnXc_CsActionsAreNotEqual(self, globalCtx, 8, 4)) {
        EnXc_SetupPlayingHarpAction(self, globalCtx, true);
    }
}

void func_80B3DE00(EnXc* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gSheikPullingOutHarpAnim, -1.0f,
                         Animation_GetLastFrame(&gSheikPullingOutHarpAnim), 0.0f, ANIMMODE_ONCE, 0.0f);
        self->action = SHEIK_ACTION_12;
        self->drawMode = SHEIK_DRAW_PULLING_OUT_HARP;
    }
}

void func_80B3DE78(EnXc* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gSheikIdleAnim),
                         ANIMMODE_LOOP, 0.0f);
        self->action = SHEIK_ACTION_13;
        self->drawMode = SHEIK_DRAW_DEFAULT;
        self->timer = 0.0f;
    }
}

void EnXc_SetupReverseAccel(EnXc* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != 0) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[4];

        if (npcAction != NULL && npcAction->action == 4) {
            Animation_Change(&self->skelAnime, &gSheikWalkingAnim, -1.0f, Animation_GetLastFrame(&gSheikWalkingAnim),
                             0.0f, ANIMMODE_LOOP, -12.0f);
            self->action = SHEIK_ACTION_REVERSE_ACCEL;
            self->actor.world.rot.y += 0x8000;
            self->timer = 0.0f;
        }
    }
}

void EnXc_SetupReverseWalkAction(EnXc* self) {
    self->timer++;
    if (self->timer >= 12.0f) {
        self->actor.speedXZ = (kREG(2) * 0.01f) + 1.2f;
        self->action = SHEIK_ACTION_REVERSE_WALK;
    }
}

void EnXc_SetupReverseHaltAction(EnXc* self) {
    f32 xzDistToPlayer = self->actor.xzDistToPlayer;

    if (xzDistToPlayer >= kREG(5) + 140.0f) {
        Animation_Change(&self->skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gSheikIdleAnim),
                         ANIMMODE_LOOP, -12.0f);
        self->action = SHEIK_ACTION_REVERSE_HALT;
        self->timer = 0.0f;
    }
}

void EnXc_SetupNutThrow(EnXc* self) {
    self->timer++;
    if (self->timer >= 12.0f) {
        Animation_Change(&self->skelAnime, &gSheikThrowingNutAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gSheikThrowingNutAnim), ANIMMODE_ONCE, 0.0f);
        self->action = SHEIK_ACTION_THROW_NUT;
        self->timer = 0.0f;
        self->actor.speedXZ = 0.0f;
    }
}

void func_80B3E164(EnXc* self, GlobalContext* globalCtx) {
    self->timer++;
    if (self->timer >= 30.0f) {
        self->action = SHEIK_ACTION_DELETE;
        EnXc_SpawnNut(self, globalCtx);
    }
}

void EnXc_SetupDisappear(EnXc* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.state != 0) {
        CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[4];

        if (npcAction != NULL && npcAction->action == 9) {
            s16 sceneNum = globalCtx->sceneNum;

            // Sheik fades away if end of Bolero CS, kill actor otherwise
            if (sceneNum == SCENE_SPOT17) {
                self->action = SHEIK_ACTION_FADE;
                self->drawMode = SHEIK_DRAW_NOTHING;
                self->actor.shape.shadowAlpha = 0;
            } else {
                Actor_Kill(&self->actor);
            }
        }
    }
}

void EnXc_ActionFunc0(EnXc* self, GlobalContext* globalCtx) {
    EnXc_SetColossusAppearSFX(self, globalCtx);
    EnXc_SetColossusWindSFX(globalCtx);
    func_80B3D750(self, globalCtx);
}

void EnXc_ActionFunc1(EnXc* self, GlobalContext* globalCtx) {
    EnXc_SetColossusAppearSFX(self, globalCtx);
    EnXc_SetColossusWindSFX(globalCtx);
    EnXc_SetupFallFromSkyAction(self, globalCtx);
}

void EnXc_GracefulFall(EnXc* self, GlobalContext* globalCtx) {
    s32 animFinished = EnXc_AnimIsFinished(self);

    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetLandingSFX(self, globalCtx);
    EnXc_SetColossusAppearSFX(self, globalCtx);
    EnXc_SetColossusWindSFX(globalCtx);
    func_80B3D8A4(self, globalCtx, animFinished);
}

void EnXc_Accelerate(EnXc* self, GlobalContext* globalCtx) {
    EnXc_CalcXZAccel(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    EnXc_SetupWalkAction(self);
}

void EnXc_Walk(EnXc* self, GlobalContext* globalCtx) {
    func_80B3D644(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    EnXc_SetupHaltAction(self);
}

void EnXc_Stopped(EnXc* self, GlobalContext* globalCtx) {
    EnXc_CalcXZSpeed(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    EnXc_SetupStoppedAction(self);
}

void EnXc_ActionFunc6(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    func_80B3DAF0(self, globalCtx);
}

void EnXc_ActionFunc7(EnXc* self, GlobalContext* globalCtx) {
    s32 animFinished = EnXc_AnimIsFinished(self);

    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupInitialHarpAction(self, animFinished);
}

void EnXc_ActionFunc8(EnXc* self, GlobalContext* globalCtx) {
    s32 animFinished = EnXc_AnimIsFinished(self);

    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupPlayingHarpAction(self, globalCtx, animFinished);
}

void EnXc_ActionFunc9(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    func_80B3DCA8(self, globalCtx);
}

void EnXc_ActionFunc10(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupHarpPutawayAction(self, globalCtx);
}

void EnXc_ActionFunc11(EnXc* self, GlobalContext* globalCtx) {
    s32 animFinished = EnXc_AnimIsFinished(self);

    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    func_80B3DE00(self, animFinished);
}

void EnXc_ActionFunc12(EnXc* self, GlobalContext* globalCtx) {
    s32 animFinished = EnXc_AnimIsFinished(self);

    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    func_80B3DE78(self, animFinished);
}

void EnXc_ActionFunc13(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_InitFlame(self, globalCtx);
    EnXc_SetupReverseAccel(self, globalCtx);
}

void EnXc_ReverseAccelerate(EnXc* self, GlobalContext* globalCtx) {
    func_80B3D6F0(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    EnXc_InitFlame(self, globalCtx);
    EnXc_SetupReverseWalkAction(self);
}

void EnXc_ActionFunc15(EnXc* self, GlobalContext* globalCtx) {
    func_80B3D710(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    EnXc_InitFlame(self, globalCtx);
    EnXc_SetupReverseHaltAction(self);
}

void EnXc_HaltAndWaitToThrowNut(EnXc* self, GlobalContext* globalCtx) {
    func_80B3D730(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    EnXc_InitFlame(self, globalCtx);
    EnXc_SetupNutThrow(self);
}

void EnXc_ThrowNut(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetNutThrowSFX(self, globalCtx);
    EnXc_InitFlame(self, globalCtx);
    func_80B3E164(self, globalCtx);
}

void EnXc_Delete(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_InitFlame(self, globalCtx);
    EnXc_SetupDisappear(self, globalCtx);
}

void EnXc_Fade(EnXc* self, GlobalContext* globalCtx) {
    EnXc_InitFlame(self, globalCtx);
}

void func_80B3E87C(Gfx** dList, EnXc* self) {
    f32 currentFrame = self->skelAnime.curFrame;

    if (currentFrame >= 34.0f) {
        *dList = gSheikHarpDL;
    }
}

s32 EnXc_PullingOutHarpOverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                        void* thisx) {
    EnXc* self = THIS;

    if (limbIndex == 12) {
        func_80B3E87C(dList, self);
    }

    return 0;
}

s32 EnXc_HarpOverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                              void* thisx) {
    EnXc* self = THIS;

    if (limbIndex == 12) {
        *dList = gSheikHarpDL;
    }

    return 0;
}

void EnXc_DrawPullingOutHarp(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;
    s32 pad;
    s16 eyePattern = self->eyeIdx;
    void* eyeTexture = sEyeTextures[eyePattern];
    SkelAnime* skelAnime = &self->skelAnime;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad2;

    OPEN_DISPS(gfxCtx, "../z_en_oA2_inSpot05.c", 1444);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 20, 0);
    gDPSetEnvColor(POLY_OPA_DISP++, 60, 0, 0, 0);

    func_80093D18(gfxCtx);
    func_8002EBCC(&self->actor, globalCtx, 0);
    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                          EnXc_PullingOutHarpOverrideLimbDraw, NULL, self);
    CLOSE_DISPS(gfxCtx, "../z_en_oA2_inSpot05.c", 1497);
}

void EnXc_DrawHarp(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;
    s32 pad;
    s16 eyePattern = self->eyeIdx;
    void* eyeTexture = sEyeTextures[eyePattern];
    SkelAnime* skelAnime = &self->skelAnime;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad2;

    OPEN_DISPS(gfxCtx, "../z_en_oA2_inSpot05.c", 1511);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 20, 0);
    gDPSetEnvColor(POLY_OPA_DISP++, 60, 0, 0, 0);

    func_80093D18(gfxCtx);
    func_8002EBCC(&self->actor, globalCtx, 0);
    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                          EnXc_HarpOverrideLimbDraw, NULL, self);
    CLOSE_DISPS(gfxCtx, "../z_en_oA2_inSpot05.c", 1564);
}

void func_80B3EBF0(EnXc* self, GlobalContext* globalCtx) {
    self->action = SHEIK_ACTION_20;
}

void func_80B3EC00(EnXc* self) {
    self->action = SHEIK_ACTION_21;
}

void func_80B3EC0C(EnXc* self, GlobalContext* globalCtx) {
    CutsceneContext* csCtx = &globalCtx->csCtx;

    if (csCtx->state != 0) {
        CsCmdActorAction* npcAction = csCtx->npcActions[4];

        if ((npcAction != NULL) && (npcAction->action != 1)) {
            PosRot* posRot = &self->actor.world;
            Vec3i* startPos = &npcAction->startPos;
            ActorShape* shape = &self->actor.shape;

            posRot->pos.x = startPos->x;
            posRot->pos.y = startPos->y;
            posRot->pos.z = startPos->z;

            posRot->rot.y = shape->rot.y = npcAction->rot.y;

            self->action = SHEIK_ACTION_22;
            self->drawMode = SHEIK_DRAW_DEFAULT;
        }
    }
}

void func_80B3EC90(EnXc* self, GlobalContext* globalCtx) {
    CutsceneContext* csCtx = &globalCtx->csCtx;

    if (csCtx->state != 0) {
        CsCmdActorAction* npcAction = csCtx->npcActions[4];

        if (npcAction != NULL && npcAction->action != 6) {
            func_80B3C9EC(self);
        }
    }
}

void func_80B3ECD8(EnXc* self) {
    self->timer++;
    if (self->timer >= 12.0f) {
        self->actor.speedXZ = kREG(2) * 0.01f + 1.2f;
        self->action = SHEIK_ACTION_24;
    }
}

void EnXc_ActionFunc20(EnXc* self, GlobalContext* globalCtx) {
    func_80B3EC00(self);
}

void EnXc_ActionFunc21(EnXc* self, GlobalContext* globalCtx) {
    func_80B3EC0C(self, globalCtx);
}

void EnXc_ActionFunc22(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    func_80B3EC90(self, globalCtx);
}

void EnXc_ActionFunc23(EnXc* self, GlobalContext* globalCtx) {
    func_80B3D6F0(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    func_80B3ECD8(self);
}

void EnXc_ActionFunc24(EnXc* self, GlobalContext* globalCtx) {
}

void EnXc_ActionFunc25(EnXc* self, GlobalContext* globalCtx) {
}

void EnXc_ActionFunc26(EnXc* self, GlobalContext* globalCtx) {
}

void EnXc_ActionFunc27(EnXc* self, GlobalContext* globalCtx) {
}

void EnXc_ActionFunc28(EnXc* self, GlobalContext* globalCtx) {
}

void func_80B3EE64(EnXc* self, GlobalContext* globalCtx) {
    self->action = SHEIK_ACTION_SERENADE;
}

void func_80B3EE74(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_SerenadeCS(self, globalCtx)) {
        self->action = SHEIK_ACTION_30;
    }
}

void func_80B3EEA4(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_30, SHEIK_ACTION_31);
}

void func_80B3EEC8(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_31, SHEIK_ACTION_32);
}

void func_80B3EEEC(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_32, SHEIK_ACTION_33);
}

void func_80B3EF10(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_33, SHEIK_ACTION_34);
}

void func_80B3EF34(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_34, SHEIK_ACTION_35);
}

void func_80B3EF58(EnXc* self) {
    func_80B3C7D4(self, SHEIK_ACTION_35, SHEIK_ACTION_36, SHEIK_ACTION_34);
}

void func_80B3EF80(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_36, SHEIK_ACTION_37);
}

void func_80B3EFA4(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_37, SHEIK_ACTION_38);
}

void func_80B3EFC8(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_38, SHEIK_ACTION_39);
}

void func_80B3EFEC(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_39, SHEIK_ACTION_40);
}

void func_80B3F010(EnXc* self) {
    f32 xzDistToPlayer = self->actor.xzDistToPlayer;

    if (kREG(5) + 140.0f <= xzDistToPlayer) {
        Animation_Change(&self->skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gSheikIdleAnim),
                         ANIMMODE_LOOP, -12.0f);
        self->action = SHEIK_ACTION_41;
        self->timer = 0.0f;
    }
}

void func_80B3F0B8(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_41, SHEIK_ACTION_42);
}

void func_80B3F0DC(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_42, SHEIK_ACTION_43);
}

void func_80B3F100(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_43, SHEIK_ACTION_44);
}

void EnXc_Serenade(EnXc* self, GlobalContext* globalCtx) {
    func_80B3EE74(self, globalCtx);
}

void EnXc_ActionFunc30(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc21(self, globalCtx);
    func_80B3EEA4(self);
}

void EnXc_ActionFunc31(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc6(self, globalCtx);
    func_80B3C588(self, globalCtx, 4);
    func_80B3EEC8(self);
}

void EnXc_ActionFunc32(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc7(self, globalCtx);
    func_80B3EEEC(self);
}

void EnXc_ActionFunc33(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc8(self, globalCtx);
    func_80B3EF10(self);
}

void EnXc_ActionFunc34(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc9(self, globalCtx);
    func_80B3EF34(self);
}

void EnXc_ActionFunc35(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc10(self, globalCtx);
    func_80B3EF58(self);
}

void EnXc_ActionFunc36(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc11(self, globalCtx);
    func_80B3EF80(self);
}

void EnXc_ActionFunc37(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc12(self, globalCtx);
    func_80B3EFA4(self);
}

void EnXc_ActionFunc38(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc13(self, globalCtx);
    func_80B3EFC8(self);
}

void EnXc_ActionFunc39(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ReverseAccelerate(self, globalCtx);
    func_80B3EFEC(self);
}

void EnXc_ActionFunc40(EnXc* self, GlobalContext* globalCtx) {
    func_80B3D710(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    func_80B3F010(self);
}

void EnXc_ActionFunc41(EnXc* self, GlobalContext* globalCtx) {
    EnXc_HaltAndWaitToThrowNut(self, globalCtx);
    func_80B3F0B8(self);
}

void EnXc_ActionFunc42(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ThrowNut(self, globalCtx);
    func_80B3F0DC(self);
}

void EnXc_ActionFunc43(EnXc* self, GlobalContext* globalCtx) {
    EnXc_Delete(self, globalCtx);
    func_80B3F100(self);
}

void EnXc_ActionFunc44(EnXc* self, GlobalContext* globalCtx) {
}

void func_80B3F3C8(EnXc* self, GlobalContext* globalCtx) {
    self->action = SHEIK_ACTION_45;
}

void func_80B3F3D8() {
    func_800788CC(NA_SE_PL_SKIP);
}

void EnXc_PlayDiveSFX(Vec3f* src, GlobalContext* globalCtx) {
    f32 wDest[2];

    SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, src, &D_80B42DA0, wDest);
    func_80078914(&D_80B42DA0, NA_SE_EV_DIVE_INTO_WATER);
}

void EnXc_LakeHyliaDive(GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = npcAction = EnXc_GetCsCmd(globalCtx, 0);

    if (npcAction != NULL) {
        Vec3f startPos;

        startPos.x = npcAction->startPos.x;
        startPos.y = npcAction->startPos.y;
        startPos.z = npcAction->startPos.z;

        EffectSsGRipple_Spawn(globalCtx, &startPos, 100, 500, 0);
        EffectSsGRipple_Spawn(globalCtx, &startPos, 100, 500, 10);
        EffectSsGRipple_Spawn(globalCtx, &startPos, 100, 500, 20);
        EffectSsGSplash_Spawn(globalCtx, &startPos, NULL, NULL, 1, 0);
        EnXc_PlayDiveSFX(&startPos, globalCtx);
    }
}

void func_80B3F534(GlobalContext* globalCtx) {
    CutsceneContext* csCtx = &globalCtx->csCtx;
    u16 frameCount = csCtx->frames;

    if (frameCount == 310) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_WARP1, -1044.0f, -1243.0f, 7458.0f, 0, 0, 0,
                    WARP_DESTINATION);
    }
}

void func_80B3F59C(EnXc* self, GlobalContext* globalCtx) {
    static s32 D_80B41DAC = 1;
    CsCmdActorAction* npcAction = EnXc_GetCsCmd(globalCtx, 0);

    if (npcAction != NULL) {
        s32 action = npcAction->action;

        if (action != D_80B41DAC) {
            switch (action) {
                case 2:
                    func_80B3F3D8();
                    break;
                case 3:
                    EnXc_LakeHyliaDive(globalCtx);
                    break;
                default:
                    break;
            }
            D_80B41DAC = action;
        }
    }
}

void func_80B3F620(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_45, SHEIK_ACTION_46);
}

void func_80B3F644(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_46, SHEIK_ACTION_47);
}

void func_80B3F668(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_CompareCsAction(self, globalCtx, 4, 4)) {
        EnXc_ChangeAnimation(self, &gSheikWalkingAnim, ANIMMODE_LOOP, -12.0f, true);
        self->action = SHEIK_ACTION_48;
        self->actor.world.rot.y += 0x8000;
        self->timer = 0.0f;
    }
}

void func_80B3F6DC(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_48, SHEIK_ACTION_49);
}

void EnXc_SetupKneelAction(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_CompareCsAction(self, globalCtx, 16, 4)) {
        EnXc_ChangeAnimation(self, &gSheikKneelingAnim, ANIMMODE_LOOP, 0.0f, false);
        self->action = SHEIK_ACTION_KNEEL;
    }
}

void func_80B3F754(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_CompareCsAction(self, globalCtx, 22, 4)) {
        EnXc_ChangeAnimation(self, &gSheikAnim_01A048, ANIMMODE_LOOP, 0.0f, false);
        self->action = SHEIK_ACTION_51;
        func_80B3C588(self, globalCtx, 4);
    }
}

void func_80B3F7BC(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_CompareCsAction(self, globalCtx, 9, 4)) {
        self->action = SHEIK_ACTION_52;
        self->drawMode = SHEIK_DRAW_NOTHING;
    }
}

void EnXc_ActionFunc45(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc20(self, globalCtx);
    func_80B3F620(self);
}

void EnXc_ActionFunc46(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc21(self, globalCtx);
    func_80B3F644(self);
}

void EnXc_ActionFunc47(EnXc* self, GlobalContext* globalCtx) {
    func_80B3F534(globalCtx);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    func_80B3C588(self, globalCtx, 4);
    func_80B3F668(self, globalCtx);
}

void EnXc_ActionFunc48(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc23(self, globalCtx);
    func_80B3F6DC(self);
}

void EnXc_ActionFunc49(EnXc* self, GlobalContext* globalCtx) {
    func_80B3D710(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetWalkingSFX(self, globalCtx);
    EnXc_SetupKneelAction(self, globalCtx);
}

void EnXc_Kneel(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    func_80B3F59C(self, globalCtx);
    func_80B3C588(self, globalCtx, 4);
    func_80B3F754(self, globalCtx);
}

void EnXc_ActionFunc51(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    func_80B3F59C(self, globalCtx);
    func_80B3C620(self, globalCtx, 4);
    func_80B3F7BC(self, globalCtx);
}

void EnXc_ActionFunc52(EnXc* self, GlobalContext* globalCtx) {
    func_80B3F59C(self, globalCtx);
}

void func_80B3FA08(EnXc* self, GlobalContext* globalCtx) {
    self->action = SHEIK_ACTION_53;
    self->triforceAngle = kREG(24) + 0x53FC;
}

void func_80B3FA2C(void) {
    func_800F3F3C(1);
}

void EnXc_PlayTriforceSFX(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;

    if (self->unk_2A8) {
        s32 pad;
        Vec3f src;
        Vec3f pos;
        Vec3f sp1C = { 0.0f, 0.0f, 0.0f };
        f32 wDest;

        Matrix_MultVec3f(&sp1C, &src);
        SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, &src, &pos, &wDest);
        Audio_PlaySoundAtPosition(globalCtx, &pos, 80, NA_SE_EV_TRIFORCE_MARK);
        self->unk_2A8 = 0;
    }
}

void func_80B3FAE0(EnXc* self) {
    if (Animation_OnFrame(&self->skelAnime, 38.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_SK_SHOUT);
        func_80B3FA2C();
    }
}

void EnXc_CalcTriforce(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;

    if (EnXc_CompareCsAction(self, globalCtx, 21, 4)) {
        self->unk_274 = 1;
        if (self->unk_2AC == 0) {
            self->unk_2AC = 1;
            self->unk_2A8 = 1;
        }
    } else if (EnXc_CompareCsAction(self, globalCtx, 19, 4)) {
        self->unk_274 = 2;
    }
    if (self->unk_274 != 0) {
        f32* timer = &self->timer;
        s32* prim = self->triforcePrimColor;
        s32* env = self->triforceEnvColor;
        f32* scale = self->triforceScale;

        if (self->unk_274 == 1) {
            if (*timer < kREG(25) + 40.0f) {
                f32 div = *timer / (kREG(25) + 40.0f);
                prim[2] = -85.0f * div + 255;
                prim[3] = 255.0f * div;
                env[1] = 100.0f * div + 100;
                *timer += 1.0f;
            } else {
                prim[2] = 170;
                prim[3] = 255;
                env[1] = 200;
            }
            scale[0] = kREG(19) * 0.1f + 40.0f;
            scale[1] = kREG(20) * 0.1f + 40.0f;
            scale[2] = kREG(21) * 0.1f + 40.0f;
        } else if (self->unk_274 == 2) {
            f32 maxTime = (kREG(25) + 40.0f) + (kREG(27) + 90.0f);
            if (*timer < maxTime) {
                f32 div = (*timer - (kREG(25) + 40.0f)) / (kREG(27) + 90.0f);
                scale[0] = (kREG(19) * 0.1f + 40.0f) + div * ((kREG(26) + 50.0f) * (kREG(19) * 0.1f + 40.0f));
                scale[1] = (kREG(20) * 0.1f + 40.0f) + div * ((kREG(26) + 50.0f) * (kREG(20) * 0.1f + 40.0f));
                scale[2] = (kREG(21) * 0.1f + 40.0f) + div * ((kREG(26) + 50.0f) * (kREG(21) * 0.1f + 40.0f));
                *timer += 1.0f;
            } else {
                scale[0] = (kREG(19) * 0.1f + 40.0f) * (kREG(26) + 50.0f);
                scale[1] = (kREG(20) * 0.1f + 40.0f) * (kREG(26) + 50.0f);
                scale[2] = (kREG(21) * 0.1f + 40.0f) * (kREG(26) + 50.0f);
            }
            self->triforceAngle += (s16)(kREG(28) + 0x2EE0);
        }
    }
}

void func_80B3FF0C(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_CsActionsAreNotEqual(self, globalCtx, 1, 4)) {
        CutsceneContext* csCtx = &globalCtx->csCtx;

        if (csCtx->state != 0) {
            CsCmdActorAction* npcAction = globalCtx->csCtx.npcActions[4];

            if (npcAction != NULL) {
                PosRot* posRot = &self->actor.world;
                ActorShape* shape = &self->actor.shape;
                Vec3i* startPos = &npcAction->startPos;

                posRot->pos.x = startPos->x;
                posRot->pos.y = startPos->y;
                posRot->pos.z = startPos->z;

                posRot->rot.y = shape->rot.y = npcAction->rot.y;
            }
        }

        self->action = SHEIK_ACTION_54;
        self->drawMode = SHEIK_DRAW_DEFAULT;
    }
}

void EnXc_SetupShowTriforceAction(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_CompareCsAction(self, globalCtx, 10, 4)) {
        Animation_Change(&self->skelAnime, &gSheikShowingTriforceOnHandAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gSheikShowingTriforceOnHandAnim), ANIMMODE_ONCE, -8.0f);
        self->action = SHEIK_ACTION_SHOW_TRIFORCE;
        self->drawMode = SHEIK_DRAW_TRIFORCE;
    }
}

void EnXc_SetupShowTriforceIdleAction(EnXc* self, s32 animFinished) {
    if (animFinished) {
        Animation_Change(&self->skelAnime, &gSheikShowingTriforceOnHandIdleAnim, 1.0f, 0.0f,
                         Animation_GetLastFrame(&gSheikShowingTriforceOnHandIdleAnim), ANIMMODE_LOOP, 0.0f);
        self->action = SHEIK_ACTION_SHOW_TRIFORCE_IDLE;
    }
}
void func_80B400AC(EnXc* self, GlobalContext* globalCtx) {
    if (EnXc_CompareCsAction(self, globalCtx, 9, 4)) {
        Actor_Kill(&self->actor);
    }
}

void EnXc_ActionFunc53(EnXc* self, GlobalContext* globalCtx) {
    func_80B3FF0C(self, globalCtx);
}

void EnXc_ActionFunc54(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupShowTriforceAction(self, globalCtx);
    func_80B3C888(self, globalCtx);
}

void EnXc_ShowTriforce(EnXc* self, GlobalContext* globalCtx) {
    s32 animFinished = EnXc_AnimIsFinished(self);

    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_CalcTriforce(&self->actor, globalCtx);
    func_80B3FAE0(self);
    EnXc_SetupShowTriforceIdleAction(self, animFinished);
    func_80B3C888(self, globalCtx);
}

void EnXc_ShowTriforceIdle(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_CalcTriforce(&self->actor, globalCtx);
    func_80B400AC(self, globalCtx);
}

s32 EnXc_TriforceOverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                  void* thisx) {
    if (limbIndex == 15) {
        *dList = gSheikDL_011620;
    }
    return 0;
}

void EnXc_TriforcePostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    s32 pad[2];
    EnXc* self = THIS;

    if (limbIndex == 15) {
        Vec3f vec = { 0.0f, 0.0f, 0.0f };
        EnXc_PlayTriforceSFX(&self->actor, globalCtx);
        Matrix_MultVec3f(&vec, &self->handPos);
        self->unk_2BC = 1;
    }
}

void EnXc_DrawTriforce(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;
    s32 pad;
    s16 eyeIdx = self->eyeIdx;
    void* eyeTexture = sEyeTextures[eyeIdx];
    SkelAnime* skelAnime = &self->skelAnime;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    s32 pad2;

    OPEN_DISPS(gfxCtx, "../z_en_oA2_inMetamol.c", 565);
    if (self->unk_2BC != 0) {
        Mtx* mtx = Graph_Alloc(gfxCtx, sizeof(Mtx));
        s32* primColor = self->triforcePrimColor;
        s32* envColor = self->triforceEnvColor;
        f32* scale = self->triforceScale;

        Matrix_Push();
        Matrix_Translate(kREG(16) + 100.0f, kREG(17) + 4460.0f, kREG(18) + 1190.0f, MTXMODE_APPLY);
        Matrix_RotateRPY(kREG(22), kREG(23), self->triforceAngle, MTXMODE_APPLY);
        Matrix_Scale(scale[0], scale[1], scale[2], MTXMODE_APPLY);
        Matrix_ToMtx(mtx, "../z_en_oA2_inMetamol.c", 602);
        Matrix_Pop();
        func_80093D84(gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 255, 255, primColor[2], primColor[3]);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, envColor[1], 0, 128);
        gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gSheikDL_012970);
    }

    func_8002EBCC(thisx, globalCtx, 0);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTexture));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeTexture));
    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                          EnXc_TriforceOverrideLimbDraw, EnXc_TriforcePostLimbDraw, self);
    CLOSE_DISPS(gfxCtx, "../z_en_oA2_inMetamol.c", 668);
}

void func_80B40590(EnXc* self, GlobalContext* globalCtx) {
    self->action = SHEIK_ACTION_NOCTURNE_INIT;
    self->drawMode = SHEIK_DRAW_SQUINT;
}

void EnXc_SetThrownAroundSFX(EnXc* self) {
    SkelAnime* skelAnime = &self->skelAnime;

    if (Animation_OnFrame(skelAnime, 9.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_BOUND_GRASS);
        func_80078914(&self->actor.projectedPos, NA_SE_VO_SK_CRASH);
    } else if (Animation_OnFrame(skelAnime, 26.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_BOUND_GRASS);
    } else if (Animation_OnFrame(skelAnime, 28.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_WALK_GRASS);
    } else if (Animation_OnFrame(skelAnime, 34.0f)) {
        func_80078914(&self->actor.projectedPos, NA_SE_PL_WALK_GRASS);
    }
}

void EnXc_PlayLinkScreamSFX(EnXc* self, GlobalContext* globalCtx) {
    if (globalCtx->csCtx.frames == 1455) {
        func_800F3F3C(7);
    }
}

void EnXc_SetCrySFX(EnXc* self, GlobalContext* globalCtx) {
    CutsceneContext* csCtx = &globalCtx->csCtx;

    if (csCtx->frames == 869) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_SK_CRY_0);
    } else if (csCtx->frames == 939) {
        func_80078914(&self->actor.projectedPos, NA_SE_VO_SK_CRY_1);
    }
}

void func_80B406F8(Actor* thisx) {
    EnXc* self = THIS;

    self->action = SHEIK_ACTION_NOCTURNE_INIT;
    self->drawMode = SHEIK_DRAW_NOTHING;
    self->actor.shape.shadowAlpha = 0;
}

void EnXc_SetupIdleInNocturne(EnXc* self, GlobalContext* globalCtx) {
    s32 pad;
    ActorShape* actorShape = &self->actor.shape;
    SkelAnime* skelAnime = &self->skelAnime;
    f32 frameCount = Animation_GetLastFrame(&gSheikIdleAnim);

    func_80B3C9DC(self);
    func_80B3C588(self, globalCtx, 4);
    Animation_Change(skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP, 0.0f);
    self->action = SHEIK_ACTION_NOCTURNE_IDLE;
    self->drawMode = SHEIK_DRAW_SQUINT;
    actorShape->shadowAlpha = 255;
}

void EnXc_SetupDefenseStance(Actor* thisx) {
    EnXc* self = THIS;
    SkelAnime* skelAnime = &self->skelAnime;
    f32 frameCount = Animation_GetLastFrame(&gSheikDefenseStanceAnim);

    Animation_Change(skelAnime, &gSheikDefenseStanceAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, -8.0f);
    self->action = SHEIK_ACTION_DEFENSE_STANCE;
    self->drawMode = SHEIK_DRAW_DEFAULT;
}

void EnXc_SetupContortions(EnXc* self, GlobalContext* globalCtx) {
    s32 pad;
    SkelAnime* skelAnime = &self->skelAnime;
    f32 frameCount = Animation_GetLastFrame(&gSheikIdleAnim);

    Animation_Change(skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP, 0.0f);
    func_80B3C588(self, globalCtx, 4);
    func_80B3C964(self, globalCtx);
    Animation_Change(skelAnime, &gSheikContortionsAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gSheikContortionsAnim),
                     ANIMMODE_ONCE, 0.0f);
    self->action = SHEIK_ACTION_CONTORT;
    self->drawMode = SHEIK_DRAW_DEFAULT;
    self->actor.shape.shadowAlpha = 255;
}

void EnXc_SetupFallInNocturne(EnXc* self, GlobalContext* globalCtx) {
    s32 pad;
    SkelAnime* skelAnime = &self->skelAnime;
    f32 frameCount = Animation_GetLastFrame(&gSheikIdleAnim);

    Animation_Change(skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP, 0.0f);
    func_80B3C588(self, globalCtx, 4);
    func_80B3C964(self, globalCtx);
    Animation_Change(skelAnime, &gSheikFallingFromContortionsAnim, 1.0f, 0.0f,
                     Animation_GetLastFrame(&gSheikFallingFromContortionsAnim), ANIMMODE_ONCE, 0.0f);
    self->action = SHEIK_ACTION_NOCTURNE_FALL;
    self->drawMode = SHEIK_DRAW_DEFAULT;
    self->actor.shape.shadowAlpha = 255;
}

void EnXc_SetupHittingGroundInNocturne(EnXc* self, GlobalContext* globalCtx) {
    s32 pad[3];
    f32 frameCount = Animation_GetLastFrame(&gSheikHittingGroundAnim);

    func_80B3C9DC(self);
    func_80B3C588(self, globalCtx, 4);
    Animation_Change(&self->skelAnime, &gSheikHittingGroundAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, 0.0f);
    self->action = SHEIK_ACTION_NOCTURNE_HIT_GROUND;
    self->drawMode = SHEIK_DRAW_DEFAULT;
    self->actor.shape.shadowAlpha = 255;
}

void func_80B40A78(EnXc* self, GlobalContext* globalCtx) {
    s32 pad[3];
    f32 frameCount = Animation_GetLastFrame(&gSheikHittingGroundAnim);

    func_80B3C9DC(self);
    func_80B3C588(self, globalCtx, 4);
    Animation_Change(&self->skelAnime, &gSheikHittingGroundAnim, 1.0f, 0.0f, frameCount, ANIMMODE_ONCE, 0.0f);
    self->action = SHEIK_ACTION_63;
    self->drawMode = SHEIK_DRAW_DEFAULT;
    self->actor.shape.shadowAlpha = 255;
}

void EnXc_SetupKneelInNocturne(EnXc* self, GlobalContext* globalCtx) {
    s32 pad[3];
    f32 frameCount = Animation_GetLastFrame(&gSheikKneelingAnim);

    func_80B3C9DC(self);
    func_80B3C588(self, globalCtx, 4);
    Animation_Change(&self->skelAnime, &gSheikKneelingAnim, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP, 0.0f);
    self->action = SHEIK_ACTION_NOCTURNE_KNEEL;
    self->drawMode = SHEIK_DRAW_DEFAULT;
    self->actor.shape.shadowAlpha = 255;
}

void func_80B40BB4(EnXc* self, GlobalContext* globalCtx) {
    s32 pad[3];
    f32 frameCount = Animation_GetLastFrame(&gSheikIdleAnim);
    func_80B3C9DC(self);
    func_80B3C588(self, globalCtx, 4);
    Animation_Change(&self->skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, frameCount, ANIMMODE_LOOP, 0.0f);
    self->action = SHEIK_ACTION_65;
    self->drawMode = SHEIK_DRAW_DEFAULT;
    self->actor.shape.shadowAlpha = 255;
}

void func_80B40C50(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_65, SHEIK_ACTION_66);
}

void func_80B40C74(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_66, SHEIK_ACTION_67);
}

void func_80B40C98(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_67, SHEIK_ACTION_68);
}

void func_80B40CBC(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_68, SHEIK_ACTION_69);
}

void func_80B40CE0(EnXc* self) {
    func_80B3C7D4(self, SHEIK_ACTION_69, SHEIK_ACTION_70, SHEIK_ACTION_68);
}

void func_80B40D08(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_70, SHEIK_ACTION_71);
}

void func_80B40D2C(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_71, SHEIK_ACTION_72);
}

void func_80B40D50(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_72, SHEIK_ACTION_NOCTURNE_REVERSE_ACCEL);
}

void func_80B40D74(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_NOCTURNE_REVERSE_ACCEL, SHEIK_ACTION_NOCTURNE_REVERSE_WALK);
}

void EnXc_SetupReverseHaltInNocturneCS(EnXc* self) {
    f32 xzDistToPlayer = self->actor.xzDistToPlayer;

    if (kREG(5) + 140.0f <= xzDistToPlayer) {
        Animation_Change(&self->skelAnime, &gSheikIdleAnim, 1.0f, 0.0f, Animation_GetLastFrame(&gSheikIdleAnim),
                         ANIMMODE_LOOP, -12.0f);
        self->action = SHEIK_ACTION_NOCTURNE_REVERSE_HALT;
        self->timer = 0.0f;
    }
}

void func_80B40E40(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_NOCTURNE_REVERSE_HALT, SHEIK_ACTION_NOCTURNE_THROW_NUT);
}

void func_80B40E64(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_NOCTURNE_THROW_NUT, SHEIK_ACTION_77);
}

void func_80B40E88(EnXc* self) {
    EnXc_CheckAndSetAction(self, SHEIK_ACTION_77, SHEIK_ACTION_78);
}

s32 EnXc_SetupNocturneState(Actor* thisx, GlobalContext* globalCtx) {
    CsCmdActorAction* npcAction = EnXc_GetCsCmd(globalCtx, 4);

    if (npcAction != NULL) {
        s32 action = npcAction->action;
        EnXc* self = THIS;
        s32 prevAction = self->unk_26C;

        if (action != prevAction) {
            switch (action) {
                case 1:
                    func_80B406F8(thisx);
                    break;
                case 6:
                    EnXc_SetupIdleInNocturne(self, globalCtx);
                    break;
                case 20:
                    EnXc_SetupDefenseStance(thisx);
                    break;
                case 18:
                    EnXc_SetupContortions(self, globalCtx);
                    break;
                case 14:
                    EnXc_SetupFallInNocturne(self, globalCtx);
                    break;
                case 19:
                    EnXc_SetupHittingGroundInNocturne(self, globalCtx);
                    break;
                case 15:
                    func_80B40A78(self, globalCtx);
                    break;
                case 16:
                    EnXc_SetupKneelInNocturne(self, globalCtx);
                    break;
                case 17:
                    func_80B40BB4(self, globalCtx);
                    break;
                case 9:
                    Actor_Kill(thisx);
                    break;
                default:
                    osSyncPrintf("En_Oa2_Stalker_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
                    break;
            }

            self->unk_26C = action;
            return 1;
        }
    }
    return 0;
}

void EnXc_InitialNocturneAction(EnXc* self, GlobalContext* globalCtx) {
    EnXc_SetupNocturneState(&self->actor, globalCtx);
}

void EnXc_IdleInNocturne(EnXc* self, GlobalContext* globalCtx) {
    func_80B3C588(self, globalCtx, 4);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetupNocturneState(&self->actor, globalCtx);
}

void EnXc_DefenseStance(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupNocturneState(&self->actor, globalCtx);
}

void EnXc_Contort(EnXc* self, GlobalContext* globalCtx) {
    EnXc_SetCrySFX(self, globalCtx);
    EnXc_AnimIsFinished(self);
    EnXc_SetEyePattern(self);
    if (!EnXc_SetupNocturneState(&self->actor, globalCtx)) {
        func_80B3C924(self, globalCtx);
        EnXc_BgCheck(self, globalCtx);
    }
}

void EnXc_FallInNocturne(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_SetEyePattern(self);
    EnXc_SetThrownAroundSFX(self);
    if (!EnXc_SetupNocturneState(&self->actor, globalCtx)) {
        func_80B3C8CC(self, globalCtx);
        EnXc_BgCheck(self, globalCtx);
    }
}

void EnXc_HitGroundInNocturne(EnXc* self, GlobalContext* globalCtx) {
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupNocturneState(&self->actor, globalCtx);
}

void EnXc_ActionFunc63(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_PlayLinkScreamSFX(self, globalCtx);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupNocturneState(&self->actor, globalCtx);
}

void EnXc_KneelInNocturneCS(EnXc* self, GlobalContext* globalCtx) {
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupNocturneState(&self->actor, globalCtx);
}

void EnXc_ActionFunc65(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc6(self, globalCtx);
    func_80B3C588(self, globalCtx, 4);
    func_80B40C50(self);
}

void EnXc_ActionFunc66(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc7(self, globalCtx);
    func_80B40C74(self);
}

void EnXc_ActionFunc67(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc8(self, globalCtx);
    func_80B40C98(self);
}

void EnXc_ActionFunc68(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc9(self, globalCtx);
    func_80B40CBC(self);
}

void EnXc_ActionFunc69(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc10(self, globalCtx);
    func_80B40CE0(self);
}

void EnXc_ActionFunc70(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc11(self, globalCtx);
    func_80B40D08(self);
}

void EnXc_ActionFunc71(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc12(self, globalCtx);
    func_80B40D2C(self);
}

void EnXc_ActionFunc72(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ActionFunc13(self, globalCtx);
    func_80B40D50(self);
}

void EnXc_ReverseAccelInNocturneCS(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ReverseAccelerate(self, globalCtx);
    func_80B40D74(self);
}

void EnXc_ReverseWalkInNocturneCS(EnXc* self, GlobalContext* globalCtx) {
    func_80B3D710(self);
    EnXc_AnimIsFinished(self);
    EnXc_BgCheck(self, globalCtx);
    EnXc_SetEyePattern(self);
    EnXc_SetupReverseHaltInNocturneCS(self);
}

void EnXc_ReverseHaltInNocturneCS(EnXc* self, GlobalContext* globalCtx) {
    EnXc_HaltAndWaitToThrowNut(self, globalCtx);
    func_80B40E40(self);
}

void EnXc_ThrowNutInNocturneCS(EnXc* self, GlobalContext* globalCtx) {
    EnXc_ThrowNut(self, globalCtx);
    func_80B40E64(self);
}

void EnXc_DeleteInNocturneCS(EnXc* self, GlobalContext* globalCtx) {
    EnXc_Delete(self, globalCtx);
    func_80B40E88(self);
}

void EnXc_KillInNocturneCS(EnXc* self, GlobalContext* globalCtx) {
    Actor_Kill(&self->actor);
}

void EnXc_DrawSquintingEyes(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;
    SkelAnime* skelAnime = &self->skelAnime;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;

    OPEN_DISPS(gfxCtx, "../z_en_oA2_inStalker.c", 839);
    func_80093D18(gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gSheikEyeSquintingTex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(gSheikEyeSquintingTex));
    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL, NULL,
                          NULL);
    CLOSE_DISPS(gfxCtx, "../z_en_oA2_inStalker.c", 854);
}

void EnXc_InitTempleOfTime(EnXc* self, GlobalContext* globalCtx) {
    if (LINK_IS_ADULT) {
        if (!(gSaveContext.eventChkInf[12] & 0x20)) {
            gSaveContext.eventChkInf[12] |= 0x20;
            globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gFirstAdultCs);
            gSaveContext.cutsceneTrigger = 1;
            func_80B3EBF0(self, globalCtx);
        } else if (!(gSaveContext.eventChkInf[5] & 0x20) && (gSaveContext.eventChkInf[4] & 0x100)) {
            gSaveContext.eventChkInf[5] |= 0x20;
            Item_Give(globalCtx, ITEM_SONG_PRELUDE);
            globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gPreludeCs);
            gSaveContext.cutsceneTrigger = 1;
            self->action = SHEIK_ACTION_30;
        } else if (!(gSaveContext.eventChkInf[5] & 0x20)) {
            func_80B3C9EC(self);
        } else {
            Actor_Kill(&self->actor);
        }
    } else {
        Actor_Kill(&self->actor);
    }
}

void EnXc_SetupDialogueAction(EnXc* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx)) {
        self->action = SHEIK_ACTION_IN_DIALOGUE;
    } else {
        self->actor.flags |= 9;
        if (INV_CONTENT(ITEM_HOOKSHOT) != ITEM_NONE) {
            self->actor.textId = 0x7010;
        } else {
            self->actor.textId = 0x700F;
        }
        func_8002F2F4(&self->actor, globalCtx);
    }
}

void func_80B41798(EnXc* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        self->action = SHEIK_ACTION_BLOCK_PEDESTAL;
        self->actor.flags &= ~9;
    }
}

void EnXc_BlockingPedestalAction(EnXc* self, GlobalContext* globalCtx) {
    EnXc_BgCheck(self, globalCtx);
    EnXc_UpdateCollider(&self->actor, globalCtx);
    EnXc_CalculateHeadTurn(self, globalCtx);
    EnXc_AnimIsFinished(self);
    EnXc_SetEyePattern(self);
    EnXc_SetupDialogueAction(self, globalCtx);
}

void EnXc_ActionFunc80(EnXc* self, GlobalContext* globalCtx) {
    EnXc_BgCheck(self, globalCtx);
    EnXc_UpdateCollider(&self->actor, globalCtx);
    EnXc_CalculateHeadTurn(self, globalCtx);
    EnXc_AnimIsFinished(self);
    EnXc_SetEyePattern(self);
    func_80B41798(self, globalCtx);
}

static EnXcActionFunc sActionFuncs[] = {
    EnXc_ActionFunc0,
    EnXc_ActionFunc1,
    EnXc_GracefulFall,
    EnXc_Accelerate,
    EnXc_Walk,
    EnXc_Stopped,
    EnXc_ActionFunc6,
    EnXc_ActionFunc7,
    EnXc_ActionFunc8,
    EnXc_ActionFunc9,
    EnXc_ActionFunc10,
    EnXc_ActionFunc11,
    EnXc_ActionFunc12,
    EnXc_ActionFunc13,
    EnXc_ReverseAccelerate,
    EnXc_ActionFunc15,
    EnXc_HaltAndWaitToThrowNut,
    EnXc_ThrowNut,
    EnXc_Delete,
    EnXc_Fade,
    EnXc_ActionFunc20,
    EnXc_ActionFunc21,
    EnXc_ActionFunc22,
    EnXc_ActionFunc23,
    EnXc_ActionFunc24,
    EnXc_ActionFunc25,
    EnXc_ActionFunc26,
    EnXc_ActionFunc27,
    EnXc_ActionFunc28,
    EnXc_Serenade,
    EnXc_ActionFunc30,
    EnXc_ActionFunc31,
    EnXc_ActionFunc32,
    EnXc_ActionFunc33,
    EnXc_ActionFunc34,
    EnXc_ActionFunc35,
    EnXc_ActionFunc36,
    EnXc_ActionFunc37,
    EnXc_ActionFunc38,
    EnXc_ActionFunc39,
    EnXc_ActionFunc40,
    EnXc_ActionFunc41,
    EnXc_ActionFunc42,
    EnXc_ActionFunc43,
    EnXc_ActionFunc44,
    EnXc_ActionFunc45,
    EnXc_ActionFunc46,
    EnXc_ActionFunc47,
    EnXc_ActionFunc48,
    EnXc_ActionFunc49,
    EnXc_Kneel,
    EnXc_ActionFunc51,
    EnXc_ActionFunc52,
    EnXc_ActionFunc53,
    EnXc_ActionFunc54,
    EnXc_ShowTriforce,
    EnXc_ShowTriforceIdle,
    EnXc_InitialNocturneAction,
    EnXc_IdleInNocturne,
    EnXc_DefenseStance,
    EnXc_Contort,
    EnXc_FallInNocturne,
    EnXc_HitGroundInNocturne,
    EnXc_ActionFunc63,
    EnXc_KneelInNocturneCS,
    EnXc_ActionFunc65,
    EnXc_ActionFunc66,
    EnXc_ActionFunc67,
    EnXc_ActionFunc68,
    EnXc_ActionFunc69,
    EnXc_ActionFunc70,
    EnXc_ActionFunc71,
    EnXc_ActionFunc72,
    EnXc_ReverseAccelInNocturneCS,
    EnXc_ReverseWalkInNocturneCS,
    EnXc_ReverseHaltInNocturneCS,
    EnXc_ThrowNutInNocturneCS,
    EnXc_DeleteInNocturneCS,
    EnXc_KillInNocturneCS,
    EnXc_BlockingPedestalAction,
    EnXc_ActionFunc80,
};

void EnXc_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;
    s32 action = self->action;

    if ((action < 0) || (action >= ARRAY_COUNT(sActionFuncs)) || (sActionFuncs[action] == NULL)) {
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sActionFuncs[action](self, globalCtx);
    }
}

void EnXc_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gSheikSkel, &gSheikIdleAnim, self->jointTable, self->morphTable,
                       ARRAY_COUNT(self->jointTable));
    EnXc_InitCollider(thisx, globalCtx);

    switch (self->actor.params) {
        case SHEIK_TYPE_1:
            func_80B3EBF0(self, globalCtx);
            break;
        case SHEIK_TYPE_2: // Beta Serenade Cutscene or Learning Prelude
            func_80B3EE64(self, globalCtx);
            break;
        case SHEIK_TYPE_3:
            func_80B3F3C8(self, globalCtx);
            break;
        case SHEIK_TYPE_4:
            func_80B3FA08(self, globalCtx);
            break;
        case SHEIK_TYPE_5:
            func_80B40590(self, globalCtx);
            break;
        case SHEIK_TYPE_MINUET:
            func_80B3CA38(self, globalCtx);
            break;
        case SHEIK_TYPE_BOLERO:
            func_80B3CB58(self, globalCtx);
            break;
        case SHEIK_TYPE_SERENADE:
            EnXc_SetupSerenadeAction(self, globalCtx);
            break;
        case SHEIK_TYPE_9:
            EnXc_InitTempleOfTime(self, globalCtx);
            break;
        case SHEIK_TYPE_0:
            EnXc_DoNothing(self, globalCtx);
            break;
        default:
            osSyncPrintf(VT_FGCOL(RED) " En_Oa2 の arg_data がおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
            EnXc_DoNothing(self, globalCtx);
    }
}

s32 EnXc_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnXc* self = THIS;

    if (self->unk_30C != 0) {
        if (limbIndex == 9) {
            rot->x += self->npcInfo.unk_0E.y;
            rot->y -= self->npcInfo.unk_0E.x;
        } else if (limbIndex == 16) {
            rot->x += self->npcInfo.unk_08.y;
            rot->z += self->npcInfo.unk_08.x;
        }
    }
    return 0;
}

void EnXc_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    if (limbIndex == 16) {
        EnXc* self = THIS;
        Vec3f src = { 0.0f, 10.0f, 0.0f };
        Vec3f dest;

        Matrix_MultVec3f(&src, &dest);
        self->actor.focus.pos.x = dest.x;
        self->actor.focus.pos.y = dest.y;
        self->actor.focus.pos.z = dest.z;
        self->actor.focus.rot.x = self->actor.world.rot.x;
        self->actor.focus.rot.y = self->actor.world.rot.y;
        self->actor.focus.rot.z = self->actor.world.rot.z;
    }
}

void EnXc_DrawNothing(Actor* thisx, GlobalContext* globalCtx) {
}

void EnXc_DrawDefault(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnXc* self = THIS;
    s16 eyeIdx = self->eyeIdx;
    void* eyeSegment = sEyeTextures[eyeIdx];
    SkelAnime* skelAnime = &self->skelAnime;
    GraphicsContext* localGfxCtx = globalCtx->state.gfxCtx;
    GraphicsContext* gfxCtx = localGfxCtx;

    OPEN_DISPS(gfxCtx, "../z_en_oA2.c", 1164);
    func_8002EBCC(&self->actor, globalCtx, 0);
    func_80093D18(gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeSegment));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(eyeSegment));
    SkelAnime_DrawFlexOpa(globalCtx, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount,
                          EnXc_OverrideLimbDraw, EnXc_PostLimbDraw, self);
    CLOSE_DISPS(gfxCtx, "../z_en_oA2.c", 1207);
}

static EnXcDrawFunc sDrawFuncs[] = {
    EnXc_DrawNothing, EnXc_DrawDefault,  EnXc_DrawPullingOutHarp,
    EnXc_DrawHarp,    EnXc_DrawTriforce, EnXc_DrawSquintingEyes,
};

void EnXc_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnXc* self = THIS;

    if (self->drawMode < 0 || self->drawMode > 5 || sDrawFuncs[self->drawMode] == NULL) {
        // "Draw mode is abnormal!!!!!!!!!!!!!!!!!!!!!!!!!"
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
    } else {
        sDrawFuncs[self->drawMode](thisx, globalCtx);
    }
}

const ActorInit En_Xc_InitVars = {
    ACTOR_EN_XC,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_XC,
    sizeof(EnXc),
    (ActorFunc)EnXc_Init,
    (ActorFunc)EnXc_Destroy,
    (ActorFunc)EnXc_Update,
    (ActorFunc)EnXc_Draw,
};
