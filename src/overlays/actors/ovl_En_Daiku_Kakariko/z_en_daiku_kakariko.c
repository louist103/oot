/*
 * File: z_en_daiku_kakariko.c
 * Overlay: ovl_En_Daiku_Kakariko
 * Description: Kakariko Village Carpenters
 */

#include "z_en_daiku_kakariko.h"
#include "objects/object_daiku/object_daiku.h"

#define FLAGS 0x00000019

#define THIS ((EnDaikuKakariko*)thisx)

typedef enum {
    /* 0x0 */ CARPENTER_ICHIRO,  // Red and purple pants, normal hair
    /* 0x1 */ CARPENTER_SABOORO, // Light blue pants
    /* 0x2 */ CARPENTER_JIRO,    // Green pants
    /* 0x3 */ CARPENTER_SHIRO    // Pink and purple pants, two-spiked hair
} KakarikoCarpenterType;

void EnDaikuKakariko_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDaikuKakariko_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDaikuKakariko_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDaikuKakariko_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnDaikuKakariko_Wait(EnDaikuKakariko* self, GlobalContext* globalCtx);
void EnDaikuKakariko_Run(EnDaikuKakariko* self, GlobalContext* globalCtx);

const ActorInit En_Daiku_Kakariko_InitVars = {
    ACTOR_EN_DAIKU_KAKARIKO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DAIKU,
    sizeof(EnDaikuKakariko),
    (ActorFunc)EnDaikuKakariko_Init,
    (ActorFunc)EnDaikuKakariko_Destroy,
    (ActorFunc)EnDaikuKakariko_Update,
    (ActorFunc)EnDaikuKakariko_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
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
    { 18, 66, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(0, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(0, 0x0),
    /* Master sword  */ DMG_ENTRY(0, 0x0),
    /* Giant's Knife */ DMG_ENTRY(0, 0x0),
    /* Fire arrow    */ DMG_ENTRY(0, 0x0),
    /* Ice arrow     */ DMG_ENTRY(0, 0x0),
    /* Light arrow   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(0, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(0, 0x0),
    /* Fire magic    */ DMG_ENTRY(0, 0x0),
    /* Ice magic     */ DMG_ENTRY(0, 0x0),
    /* Light magic   */ DMG_ENTRY(0, 0x0),
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

static struct_D_80AA1678 sAnimations[] = {
    { &object_daiku_Anim_001AB0, 1.0f, 2, -7.0f }, { &object_daiku_Anim_007DE0, 1.0f, 0, -7.0f },
    { &object_daiku_Anim_00885C, 1.0f, 0, -7.0f }, { &object_daiku_Anim_000C44, 1.0f, 0, -7.0f },
    { &object_daiku_Anim_000600, 1.0f, 0, -7.0f }, { &object_daiku_Anim_008164, 1.0f, 0, -7.0f },
};

void EnDaikuKakariko_SetAnimFromIndex(EnDaikuKakariko* self, s32 animIndex, s32* currentAnimIndex) {
    f32 morphFrames;

    if ((*currentAnimIndex < 0) || (animIndex == *currentAnimIndex)) {
        morphFrames = 0.0f;
    } else {
        morphFrames = sAnimations[animIndex].transitionRate;
    }

    Animation_Change(&self->skelAnime, sAnimations[animIndex].animation, 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimations[animIndex].animation), sAnimations[animIndex].mode,
                     morphFrames);

    *currentAnimIndex = animIndex;
}

void EnDaikuKakariko_Init(Actor* thisx, GlobalContext* globalCtx) {
    static u16 initFlags[] = { 0x0080, 0x00B0, 0x0070, 0x0470 }; // List of inital values for self->flags
    EnDaikuKakariko* self = THIS;
    s32 pad;

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        switch (globalCtx->sceneNum) {
            case SCENE_SPOT01:
                if (IS_DAY) {
                    self->flags |= 1;
                    self->flags |= initFlags[self->actor.params & 3];
                }
                break;
            case SCENE_KAKARIKO:
                if (IS_NIGHT) {
                    self->flags |= 2;
                }
                break;
            case SCENE_DRAG:
                self->flags |= 4;
                break;
        }
    }

    if (!(self->flags & 7)) {
        Actor_Kill(&self->actor);
    }

    if (IS_NIGHT) {
        self->flags |= 8;
    }

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);

    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_daiku_Skel_007958, NULL, self->jointTable, self->morphTable,
                       17);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);

    CollisionCheck_SetInfo2(&self->actor.colChkInfo, &sDamageTable, &sColChkInit);

    Animation_Change(&self->skelAnime, sAnimations->animation, 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimations->animation), sAnimations->mode, sAnimations->transitionRate);

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);

    self->actor.gravity = 0.0f;
    self->runSpeed = 3.0f;
    self->actor.uncullZoneForward = 1200.0f;
    self->actor.targetMode = 6;
    self->currentAnimIndex = -1;

    if (self->flags & 0x40) {
        self->actor.gravity = -1.0f;
    }

    if (self->flags & 0x10) {
        EnDaikuKakariko_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
        self->actionFunc = EnDaikuKakariko_Run;
    } else {
        if (self->flags & 8) {
            if (((self->actor.params & 3) == CARPENTER_SABOORO) || ((self->actor.params & 3) == CARPENTER_SHIRO)) {
                EnDaikuKakariko_SetAnimFromIndex(self, 5, &self->currentAnimIndex);
                self->flags |= 0x800;
            } else {
                EnDaikuKakariko_SetAnimFromIndex(self, 1, &self->currentAnimIndex);
            }

            self->skelAnime.curFrame = (s32)(Rand_ZeroOne() * self->skelAnime.endFrame);
        } else {
            EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
            self->skelAnime.curFrame = (s32)(Rand_ZeroOne() * self->skelAnime.endFrame);
        }

        self->flags |= 0x100;
        self->actionFunc = EnDaikuKakariko_Wait;
    }
}

void EnDaikuKakariko_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDaikuKakariko* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 EnDaikuKakariko_GetTalkState(EnDaikuKakariko* self, GlobalContext* globalCtx) {
    s32 talkState = 2;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx))) {
        switch (self->actor.textId) {
            case 0x6061:
                gSaveContext.infTable[23] |= 0x40;
                break;
            case 0x6064:
                gSaveContext.infTable[23] |= 0x100;
                break;
        }
        talkState = 0;
    }
    return talkState;
}

void EnDaikuKakariko_HandleTalking(EnDaikuKakariko* self, GlobalContext* globalCtx) {
    static s32 maskReactionSets[] = { 1, 2, 3, 4 };
    s16 sp26;
    s16 sp24;

    if (self->talkState == 2) {
        self->talkState = EnDaikuKakariko_GetTalkState(self, globalCtx);
    } else if (func_8002F194(&self->actor, globalCtx)) {
        self->talkState = 2;
    } else {
        func_8002F374(globalCtx, &self->actor, &sp26, &sp24);

        if ((sp26 >= 0) && (sp26 <= 320) && (sp24 >= 0) && (sp24 <= 240) && (self->talkState == 0) &&
            (func_8002F2CC(&self->actor, globalCtx, 100.0f) == 1)) {
            self->actor.textId = Text_GetFaceReaction(globalCtx, maskReactionSets[self->actor.params & 3]);

            if (self->actor.textId == 0) {
                switch (self->actor.params & 3) {
                    case 0:
                        if (self->flags & 8) {
                            self->actor.textId = 0x5076;
                        } else {
                            self->actor.textId = 0x5075;
                        }
                        break;
                    case 1:
                        if (self->flags & 1) {
                            self->actor.textId = 0x502A;
                        } else {
                            self->actor.textId = 0x5074;
                        }
                        break;
                    case 2:
                        if (self->flags & 1) {
                            self->actor.textId = 0x506A;
                        } else {
                            self->actor.textId = 0x506B;
                        }
                        break;
                    case 3:
                        if (self->flags & 1) {
                            self->actor.textId = 0x5077;
                        } else {
                            self->actor.textId = 0x5078;
                        }
                        break;
                }
            }
        }
    }
}

void EnDaikuKakariko_Talk(EnDaikuKakariko* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        EnDaikuKakariko_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
    }

    EnDaikuKakariko_HandleTalking(self, globalCtx);

    if (self->talkState == 0) {
        if (self->flags & 0x10) {
            EnDaikuKakariko_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
            self->flags &= ~0x0300;
            self->actionFunc = EnDaikuKakariko_Run;
            return;
        }

        if (!(self->flags & 8)) {
            EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
        }

        if ((self->flags & 0x800) == 0) {
            self->flags &= ~0x0200;
            self->flags |= 0x100;
        }

        self->actionFunc = EnDaikuKakariko_Wait;
    }
}

void EnDaikuKakariko_Wait(EnDaikuKakariko* self, GlobalContext* globalCtx) {
    EnDaikuKakariko_HandleTalking(self, globalCtx);

    if (SkelAnime_Update(&self->skelAnime)) {
        EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
    }

    if (self->talkState != 0) {
        if (!(self->flags & 8)) {
            EnDaikuKakariko_SetAnimFromIndex(self, 4, &self->currentAnimIndex);
        }

        if (!(self->flags & 0x800)) {
            self->flags |= 0x200;
            self->flags &= ~0x0100;
        }

        self->actionFunc = EnDaikuKakariko_Talk;
    }
}

void EnDaikuKakariko_StopRunning(EnDaikuKakariko* self, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&self->skelAnime)) {
        self->timer--;

        if (self->timer <= 0) {
            EnDaikuKakariko_SetAnimFromIndex(self, 3, &self->currentAnimIndex);
            self->actionFunc = EnDaikuKakariko_Run;
        } else {
            self->skelAnime.curFrame = self->skelAnime.startFrame;
        }
    }

    EnDaikuKakariko_HandleTalking(self, globalCtx);

    if (self->talkState != 0) {
        self->flags |= 0x200;
        EnDaikuKakariko_SetAnimFromIndex(self, 4, &self->currentAnimIndex);
        self->actionFunc = EnDaikuKakariko_Talk;
    }
}

void EnDaikuKakariko_Run(EnDaikuKakariko* self, GlobalContext* globalCtx) {
    s32 pad;
    Path* path;
    Vec3s* pathPos;
    f32 xDist;
    f32 zDist;
    s16 runAngle;
    f32 runDist;
    s16 angleStepDiff;
    s32 run;

    do {
        path = &globalCtx->setupPathList[(self->actor.params >> 8) & 0xFF];
        pathPos = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[self->waypoint];
        xDist = pathPos->x - self->actor.world.pos.x;
        zDist = pathPos->z - self->actor.world.pos.z;
        runAngle = Math_FAtan2F(xDist, zDist) * (32768.0f / M_PI);
        runDist = sqrtf((xDist * xDist) + (zDist * zDist));

        run = false;

        if (runDist <= 10.0f) {
            if (self->pathContinue == false) {
                self->waypoint++;

                if (self->waypoint >= path->count) {
                    if (self->flags & 0x20) {
                        self->waypoint = path->count - 2;
                        self->pathContinue = true;
                        self->run = run = false;

                        if (self->flags & 0x400) {
                            self->timer = 2;
                            EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
                            self->actionFunc = EnDaikuKakariko_StopRunning;
                            return;
                        }
                    } else {
                        self->waypoint = 0;
                        run = true;
                    }
                } else {
                    self->run = run = true;
                }
            } else {
                self->waypoint--;

                if (self->waypoint < 0) {
                    self->waypoint = 1;
                    self->pathContinue = false;
                    self->run = run = false;

                    if (self->flags & 0x400) {
                        self->timer = 2;
                        EnDaikuKakariko_SetAnimFromIndex(self, 0, &self->currentAnimIndex);
                        self->actionFunc = EnDaikuKakariko_StopRunning;
                        return;
                    }
                } else {
                    run = true;
                }
            }
        }
    } while (run);

    angleStepDiff = Math_SmoothStepToS(&self->actor.shape.rot.y, runAngle, 1, 5000, 0);

    self->actor.world.rot.y = self->actor.shape.rot.y;

    if (self->run == false) {
        if (angleStepDiff == 0) {
            self->run = true;
        } else {
            self->actor.speedXZ = 0.0f;
        }
    }

    if (self->run == true) {
        Math_SmoothStepToF(&self->actor.speedXZ, self->runSpeed, 0.8f, runDist, 0.0f);
    }

    Actor_MoveForward(&self->actor);

    if (self->flags & 0x40) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    } else if (self->flags & 0x80) {
        self->runFlag |= 1;
        self->flags &= ~0x0080;
    } else if (self->runFlag & 1) {
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
        self->runFlag &= ~1;
    }

    SkelAnime_Update(&self->skelAnime);
    EnDaikuKakariko_HandleTalking(self, globalCtx);

    if (self->talkState != 0) {
        self->flags |= 0x200;
        EnDaikuKakariko_SetAnimFromIndex(self, 4, &self->currentAnimIndex);
        self->actionFunc = EnDaikuKakariko_Talk;
    }
}

void EnDaikuKakariko_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDaikuKakariko* self = THIS;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);
    s32 pad2;

    if (self->currentAnimIndex == 3) {
        if (((s32)self->skelAnime.curFrame == 6) || ((s32)self->skelAnime.curFrame == 15)) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EN_MORIBLIN_WALK);
        }
    }

    Collider_UpdateCylinder(&self->actor, &self->collider);

    if (self->flags & 4) {
        self->collider.dim.pos.x -= 27;
        self->collider.dim.pos.z -= 27;
        self->collider.dim.radius = 63;
    }

    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

    self->actionFunc(self, globalCtx);

    self->npcInfo.unk_18.x = player->actor.focus.pos.x;
    self->npcInfo.unk_18.y = player->actor.focus.pos.y;
    self->npcInfo.unk_18.z = player->actor.focus.pos.z;

    if (self->flags & 0x100) {
        self->neckAngleTarget.x = 5900;
        self->flags |= 0x1000;
        func_80034A14(&self->actor, &self->npcInfo, 0, 2);
    } else if (self->flags & 0x200) {
        self->neckAngleTarget.x = 5900;
        self->flags |= 0x1000;
        func_80034A14(&self->actor, &self->npcInfo, 0, 4);
    }

    Math_SmoothStepToS(&self->neckAngle.x, self->neckAngleTarget.x, 1, 1820, 0);
}

s32 EnDaikuKakariko_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                     void* thisx) {
    EnDaikuKakariko* self = THIS;
    Vec3s angle;

    switch (limbIndex) {
        case 8:
            angle = self->npcInfo.unk_0E;
            Matrix_RotateX(-(angle.y * (M_PI / 32768.0f)), MTXMODE_APPLY);
            Matrix_RotateZ(-(angle.x * (M_PI / 32768.0f)), MTXMODE_APPLY);
            break;
        case 15:
            Matrix_Translate(1400.0f, 0.0f, 0.0f, MTXMODE_APPLY);
            angle = self->npcInfo.unk_08;

            if (self->flags & 0x1000) {
                osSyncPrintf("<%d>\n", self->neckAngle.x);
                Matrix_RotateX((angle.y + self->neckAngle.y) * (M_PI / 32768.0f), MTXMODE_APPLY);
                Matrix_RotateZ((angle.x + self->neckAngle.x) * (M_PI / 32768.0f), MTXMODE_APPLY);
            } else {
                Matrix_RotateX(angle.y * (M_PI / 32768.0f), MTXMODE_APPLY);
                Matrix_RotateZ(angle.x * (M_PI / 32768.0f), MTXMODE_APPLY);
            }

            Matrix_Translate(-1400.0f, 0.0f, 0.0f, MTXMODE_APPLY);
            break;
    }

    return 0;
}

void EnDaikuKakariko_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    static Gfx* carpenterHeadDLists[] = { object_daiku_DL_005BD0, object_daiku_DL_005AC0, object_daiku_DL_005990,
                                          object_daiku_DL_005880 };
    static Vec3f unkVec = { 700.0f, 1100.0f, 0.0f };
    EnDaikuKakariko* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_daiku_kakariko.c", 1104);

    if (limbIndex == 15) {
        Matrix_MultVec3f(&unkVec, &self->actor.focus.pos);
        gSPDisplayList(POLY_OPA_DISP++, carpenterHeadDLists[self->actor.params & 3]);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_daiku_kakariko.c", 1113);
}

void EnDaikuKakariko_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDaikuKakariko* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_daiku_kakariko.c", 1124);

    func_80093D18(globalCtx->state.gfxCtx);

    if ((thisx->params & 3) == CARPENTER_ICHIRO) {
        gDPSetEnvColor(POLY_OPA_DISP++, 170, 10, 70, 255);
    } else if ((thisx->params & 3) == CARPENTER_SABOORO) {
        gDPSetEnvColor(POLY_OPA_DISP++, 170, 200, 255, 255);
    } else if ((thisx->params & 3) == CARPENTER_JIRO) {
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 230, 70, 255);
    } else if ((thisx->params & 3) == CARPENTER_SHIRO) {
        gDPSetEnvColor(POLY_OPA_DISP++, 200, 0, 150, 255);
    }

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnDaikuKakariko_OverrideLimbDraw, EnDaikuKakariko_PostLimbDraw, thisx);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_daiku_kakariko.c", 1151);
}
