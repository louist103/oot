#include "z_en_go.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_oF1d_map/object_oF1d_map.h"

#define FLAGS 0x00000039

#define THIS ((EnGo*)thisx)

void EnGo_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGo_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGo_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A3FEB4(EnGo* self, GlobalContext* globalCtx);
void EnGo_StopRolling(EnGo* self, GlobalContext* globalCtx);
void func_80A4008C(EnGo* self, GlobalContext* globalCtx);
void EnGo_GoronLinkRolling(EnGo* self, GlobalContext* globalCtx);
void EnGo_FireGenericActionFunc(EnGo* self, GlobalContext* globalCtx);
void EnGo_CurledUp(EnGo* self, GlobalContext* globalCtx);
void EnGo_WakeUp(EnGo* self, GlobalContext* globalCtx);

void func_80A40494(EnGo* self, GlobalContext* globalCtx);
void func_80A405CC(EnGo* self, GlobalContext* globalCtx);
void EnGo_BiggoronActionFunc(EnGo* self, GlobalContext* globalCtx);
void func_80A408D8(EnGo* self, GlobalContext* globalCtx);

void func_80A40B1C(EnGo* self, GlobalContext* globalCtx);
void EnGo_GetItem(EnGo* self, GlobalContext* globalCtx);
void func_80A40C78(EnGo* self, GlobalContext* globalCtx);
void EnGo_Eyedrops(EnGo* self, GlobalContext* globalCtx);
void func_80A40DCC(EnGo* self, GlobalContext* globalCtx);

void EnGo_AddDust(EnGo* self, Vec3f* pos, Vec3f* velocity, Vec3f* accel, u8 initialTimer, f32 scale, f32 scaleStep);
void EnGo_UpdateDust(EnGo* self);
void EnGo_DrawDust(EnGo* self, GlobalContext* globalCtx);

const ActorInit En_Go_InitVars = {
    ACTOR_EN_GO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OF1D_MAP,
    sizeof(EnGo),
    (ActorFunc)EnGo_Init,
    (ActorFunc)EnGo_Destroy,
    (ActorFunc)EnGo_Update,
    (ActorFunc)EnGo_Draw,
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
    { 20, 46, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = {
    0, 0, 0, 0, MASS_IMMOVABLE,
};

typedef struct {
    AnimationHeader* animation;
    f32 playSpeed;
    u8 mode;
    f32 morphRate;
} EnGoAnimation;

static EnGoAnimation sAnimationEntries[] = {
    { &gGoronAnim_004930, 0.0f, ANIMMODE_LOOP_INTERP, 0.0f },
    { &gGoronAnim_004930, 0.0f, ANIMMODE_LOOP_INTERP, -10.0f },
    { &gGoronAnim_0029A8, 1.0f, ANIMMODE_LOOP_INTERP, -10.0f },
    { &gGoronAnim_010590, 1.0f, ANIMMODE_LOOP_INTERP, -10.0f },
};

void EnGo_SetupAction(EnGo* self, EnGoActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

u16 EnGo_GetTextID(GlobalContext* globalCtx, Actor* thisx) {
    Player* player = GET_PLAYER(globalCtx);

    switch (thisx->params & 0xF0) {
        case 0x90:
            if (gSaveContext.bgsFlag) {
                return 0x305E;
            } else if (INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_CLAIM_CHECK) {
                if (Environment_GetBgsDayCount() >= 3) {
                    return 0x305E;
                } else {
                    return 0x305D;
                }
            } else if (INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_EYEDROPS) {
                player->exchangeItemId = EXCH_ITEM_EYEDROPS;
                return 0x3059;
            } else if (INV_CONTENT(ITEM_TRADE_ADULT) >= ITEM_PRESCRIPTION) {
                return 0x3058;
            } else {
                player->exchangeItemId = EXCH_ITEM_SWORD_BROKEN;
                return 0x3053;
            }
        case 0x00:
            if (CHECK_QUEST_ITEM(QUEST_MEDALLION_FIRE)) {
                if (gSaveContext.infTable[16] & 0x8000) {
                    return 0x3042;
                } else {
                    return 0x3041;
                }
            } else if (CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1) || (gSaveContext.infTable[16] & 0x2000)) {
                if (gSaveContext.infTable[16] & 0x4000) {
                    return 0x3038;
                } else {
                    return 0x3037;
                }
            } else {
                if (gSaveContext.infTable[16] & 0x200) {
                    if (gSaveContext.infTable[16] & 0x400) {
                        return 0x3033;
                    } else {
                        return 0x3032;
                    }
                } else {
                    return 0x3030;
                }
            }
        case 0x10:
            if (Flags_GetSwitch(globalCtx, thisx->params >> 8)) {
                return 0x3052;
            } else {
                return 0x3051;
            }
        case 0x20:
            if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
                return 0x3027;
            } else if (gSaveContext.eventChkInf[2] & 0x8) {
                return 0x3021;
            } else if (gSaveContext.infTable[14] & 0x1) {
                return 0x302A;
            } else {
                return 0x3008;
            }
        case 0x30:
            if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
                return 0x3027;
            } else if (gSaveContext.eventChkInf[2] & 0x8) {
                return 0x3026;
            } else {
                return 0x3009;
            }
        case 0x40:
            if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
                return 0x3027;
            } else if (gSaveContext.eventChkInf[2] & 0x8) {
                return 0x3026;
            } else {
                return 0x300A;
            }
        case 0x50:
            if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
                return 0x3027;
            } else if (gSaveContext.infTable[15] & 1) {
                return 0x3015;
            } else {
                return 0x3014;
            }
        case 0x60:
            if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
                return 0x3027;
            } else if (gSaveContext.infTable[15] & 0x10) {
                return 0x3017;
            } else {
                return 0x3016;
            }
        case 0x70:
            if (CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
                return 0x3027;
            } else if (gSaveContext.infTable[15] & 0x100) {
                return 0x3019;
            } else {
                return 0x3018;
            }
        default:
            return 0x0000;
    }
}

s16 EnGo_SetFlagsGetStates(GlobalContext* globalCtx, Actor* thisx) {
    s16 unkState = 1;
    f32 xzRange;
    f32 yRange = fabsf(thisx->yDistToPlayer) + 1.0f;

    xzRange = thisx->xzDistToPlayer + 1.0f;
    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        if (globalCtx) {}
        case 2:
            switch (thisx->textId) {
                case 0x3008:
                    gSaveContext.infTable[14] |= 1;
                    unkState = 0;
                    break;
                case 0x300B:
                    gSaveContext.infTable[14] |= 0x800;
                    unkState = 0;
                    break;
                case 0x3014:
                    gSaveContext.infTable[15] |= 1;
                    unkState = 0;
                    break;
                case 0x3016:
                    gSaveContext.infTable[15] |= 0x10;
                    unkState = 0;
                    break;
                case 0x3018:
                    gSaveContext.infTable[15] |= 0x100;
                    unkState = 0;
                    break;
                case 0x3036:
                    func_8002F434(thisx, globalCtx, GI_TUNIC_GORON, xzRange, yRange);
                    gSaveContext.infTable[16] |= 0x2000; // EnGo exclusive flag
                    unkState = 2;
                    break;
                case 0x3037:
                    gSaveContext.infTable[16] |= 0x4000;
                    unkState = 0;
                    break;
                case 0x3041:
                    gSaveContext.infTable[16] |= 0x8000;
                    unkState = 0;
                    break;
                case 0x3059:
                    unkState = 2;
                    break;
                case 0x3052:
                case 0x3054:
                case 0x3055:
                case 0x305A:
                    unkState = 2;
                    break;
                case 0x305E:
                    unkState = 2;
                    break;
                default:
                    unkState = 0;
                    break;
            }
            break;
        case 4:
            if (func_80106BC8(globalCtx)) {
                switch (thisx->textId) {
                    case 0x300A:
                        if (globalCtx->msgCtx.choiceIndex == 0) {
                            if (CUR_UPG_VALUE(UPG_STRENGTH) != 0 || (gSaveContext.infTable[14] & 0x800)) {
                                thisx->textId = 0x300B;
                            } else {
                                thisx->textId = 0x300C;
                            }
                        } else {
                            thisx->textId = 0x300D;
                        }
                        func_8010B720(globalCtx, thisx->textId);
                        unkState = 1;
                        break;
                    case 0x3034:
                        if (globalCtx->msgCtx.choiceIndex == 0) {
                            if (gSaveContext.infTable[16] & 0x800) {
                                thisx->textId = 0x3033;
                            } else {
                                thisx->textId = 0x3035;
                            }
                        } else if (gSaveContext.infTable[16] & 0x800) {
                            thisx->textId = 0x3036;
                        } else {
                            thisx->textId = 0x3033;
                        }
                        func_8010B720(globalCtx, thisx->textId);
                        unkState = 1;
                        break;
                    case 0x3054:
                    case 0x3055:
                        if (globalCtx->msgCtx.choiceIndex == 0) {
                            unkState = 2;
                        } else {
                            thisx->textId = 0x3056;
                            func_8010B720(globalCtx, thisx->textId);
                            unkState = 1;
                        }
                        gSaveContext.infTable[11] |= 0x10;
                        break;
                }
            }
            break;
        case 5:
            if (func_80106BC8(globalCtx)) {
                switch (thisx->textId) {
                    case 0x3035:
                        gSaveContext.infTable[16] |= 0x800;
                    case 0x3032:
                    case 0x3033:
                        thisx->textId = 0x3034;
                        func_8010B720(globalCtx, thisx->textId);
                        unkState = 1;
                        break;
                    default:
                        unkState = 2;
                        break;
                }
            }
            break;
        case 6:
            if (func_80106BC8(globalCtx)) {
                unkState = 3;
            }
            break;
        case 0:
        case 1:
        case 3:
        case 7:
        case 9:
            break;
    }
    return unkState;
}

s32 func_80A3ED24(GlobalContext* globalCtx, EnGo* self, struct_80034A14_arg1* arg2, f32 arg3,
                  u16 (*getTextId)(GlobalContext*, Actor*), s16 (*unkFunc2)(GlobalContext*, Actor*)) {
    if (arg2->unk_00) {
        arg2->unk_00 = unkFunc2(globalCtx, &self->actor);
        return false;
    } else if (func_8002F194(&self->actor, globalCtx)) {
        arg2->unk_00 = 1;
        return true;
    } else if (!func_8002F2CC(&self->actor, globalCtx, arg3)) {
        return false;
    } else {
        self->actor.textId = getTextId(globalCtx, &self->actor);
        return false;
    }
}

void EnGo_ChangeAnimation(EnGo* self, s32 animIndex) {
    Animation_Change(&self->skelAnime, sAnimationEntries[animIndex].animation,
                     sAnimationEntries[animIndex].playSpeed * ((self->actor.params & 0xF0) == 0x90 ? 0.5f : 1.0f), 0.0f,
                     Animation_GetLastFrame(sAnimationEntries[animIndex].animation), sAnimationEntries[animIndex].mode,
                     sAnimationEntries[animIndex].morphRate);
}

s32 EnGo_IsActorSpawned(EnGo* self, GlobalContext* globalCtx) {
    if (((self->actor.params) & 0xF0) == 0x90) {
        return true;
    } else if (globalCtx->sceneNum == SCENE_HIDAN && !Flags_GetSwitch(globalCtx, (self->actor.params) >> 8) &&
               LINK_IS_ADULT && (self->actor.params & 0xF0) == 0x10) {
        return true;
    } else if (globalCtx->sceneNum == SCENE_SPOT18 && LINK_IS_ADULT && (self->actor.params & 0xF0) == 0x00) {
        return true;
    } else if (globalCtx->sceneNum == SCENE_SPOT16 && LINK_IS_CHILD &&
               ((self->actor.params & 0xF0) == 0x20 || (self->actor.params & 0xF0) == 0x30 ||
                (self->actor.params & 0xF0) == 0x40)) {
        return true;
    } else if (globalCtx->sceneNum == SCENE_SPOT18 && LINK_IS_CHILD &&
               ((self->actor.params & 0xF0) == 0x50 || (self->actor.params & 0xF0) == 0x60 ||
                (self->actor.params & 0xF0) == 0x70)) {
        return true;
    } else {
        return false;
    }
}

f32 EnGo_GetGoronSize(EnGo* self) {
    switch (self->actor.params & 0xF0) {
        case 0x00:
            return 10.0f;
        case 0x20:
        case 0x30:
        case 0x50:
        case 0x60:
        case 0x70:
            return 20.0f;
        case 0x40:
            return 60.0f;
        default:
            return 20.0f;
    }
}

void func_80A3F060(EnGo* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 unkVal;

    if (self->actionFunc != EnGo_BiggoronActionFunc && self->actionFunc != EnGo_FireGenericActionFunc &&
        self->actionFunc != func_80A40B1C) {
        unkVal = 1;
    }

    self->unk_1E0.unk_18 = player->actor.world.pos;
    self->unk_1E0.unk_14 = EnGo_GetGoronSize(self);
    func_80034A14(&self->actor, &self->unk_1E0, 4, unkVal);
}

void func_80A3F0E4(EnGo* self) {
    if (DECR(self->unk_214) == 0) {
        self->unk_216++;
        if (self->unk_216 >= 3) {
            self->unk_214 = Rand_S16Offset(30, 30);
            self->unk_216 = 0;
        }
    }
}

s32 EnGo_IsCameraModified(EnGo* self, GlobalContext* globalCtx) {
    f32 xyzDist;
    s16 yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
    Camera* camera = globalCtx->cameraPtrs[MAIN_CAM];

    if (fabsf(yawDiff) > 10920.0f) {
        return 0;
    }

    xyzDist = (self->actor.scale.x / 0.01f) * 10000.0f;
    if ((self->actor.params & 0xF0) == 0x90) {
        Camera_ChangeSetting(camera, CAM_SET_TEPPEN);
        xyzDist *= 4.8f;
    }

    if (fabsf(self->actor.xyzDistToPlayerSq) > xyzDist) {
        if (camera->setting == CAM_SET_TEPPEN) {
            Camera_ChangeSetting(camera, CAM_SET_NORMAL0);
        }
        return 0;
    } else {
        return 1;
    }
}

void EnGo_ReverseAnimation(EnGo* self) {
    f32 startFrame = self->skelAnime.startFrame;

    self->skelAnime.startFrame = self->skelAnime.endFrame;
    self->skelAnime.endFrame = startFrame;
}

void EnGo_UpdateShadow(EnGo* self) {
    s16 shadowAlpha;
    f32 currentFrame = self->skelAnime.curFrame;
    s16 shadowAlphaTarget = (self->skelAnime.animation == &gGoronAnim_004930 && currentFrame > 32.0f) ||
                                    self->skelAnime.animation != &gGoronAnim_004930
                                ? 255
                                : 0;

    shadowAlpha = self->actor.shape.shadowAlpha;
    Math_SmoothStepToS(&shadowAlpha, shadowAlphaTarget, 10, 60, 1);
    self->actor.shape.shadowAlpha = shadowAlpha;
}

s32 EnGo_FollowPath(EnGo* self, GlobalContext* globalCtx) {
    Path* path;
    Vec3s* pointPos;
    f32 xDist;
    f32 zDist;

    if ((self->actor.params & 0xF) == 15) {
        return false;
    }

    path = &globalCtx->setupPathList[self->actor.params & 0xF];
    pointPos = SEGMENTED_TO_VIRTUAL(path->points);
    pointPos += self->unk_218;
    xDist = pointPos->x - self->actor.world.pos.x;
    zDist = pointPos->z - self->actor.world.pos.z;
    Math_SmoothStepToS(&self->actor.world.rot.y, (s16)(Math_FAtan2F(xDist, zDist) * ((f32)0x8000 / M_PI)), 10, 1000, 1);

    if ((SQ(xDist) + SQ(zDist)) < 600.0f) {
        self->unk_218++;
        if (self->unk_218 >= path->count) {
            self->unk_218 = 0;
        }

        if ((self->actor.params & 0xF0) != 0x00) {
            return true;
        } else if (Flags_GetSwitch(globalCtx, self->actor.params >> 8)) {
            return true;
        } else if (self->unk_218 >= self->actor.shape.rot.z) {
            self->unk_218 = 0;
        }

        return true;
    }

    return false;
}

s32 EnGo_SetMovedPos(EnGo* self, GlobalContext* globalCtx) {
    Path* path;
    Vec3s* pointPos;

    if ((self->actor.params & 0xF) == 0xF) {
        return false;
    } else {
        path = &globalCtx->setupPathList[self->actor.params & 0xF];
        pointPos = SEGMENTED_TO_VIRTUAL(path->points);
        pointPos += (path->count - 1);
        self->actor.world.pos.x = pointPos->x;
        self->actor.world.pos.y = pointPos->y;
        self->actor.world.pos.z = pointPos->z;
        self->actor.home.pos = self->actor.world.pos;
        return true;
    }
}

s32 EnGo_SpawnDust(EnGo* self, u8 initialTimer, f32 scale, f32 scaleStep, s32 numDustEffects, f32 radius, f32 xzAccel) {
    Vec3f pos = { 0.0f, 0.0f, 0.0f };
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    Vec3f accel = { 0.0f, 0.3f, 0.0f };
    s16 angle;
    s32 i;

    pos = self->actor.world.pos; // Overwrites pos
    pos.y = self->actor.floorHeight;
    angle = (Rand_ZeroOne() - 0.5f) * 0x10000;
    i = numDustEffects;
    while (i >= 0) {
        accel.x = (Rand_ZeroOne() - 0.5f) * xzAccel;
        accel.z = (Rand_ZeroOne() - 0.5f) * xzAccel;
        pos.x = (Math_SinS(angle) * radius) + self->actor.world.pos.x;
        pos.z = (Math_CosS(angle) * radius) + self->actor.world.pos.z;
        EnGo_AddDust(self, &pos, &velocity, &accel, initialTimer, scale, scaleStep);
        angle += (s16)(0x10000 / numDustEffects);
        i--;
    }
    return 0;
}

s32 EnGo_IsRollingOnGround(EnGo* self, s16 unkArg1, f32 unkArg2) {
    if ((self->actor.bgCheckFlags & 1) == 0 || self->actor.velocity.y > 0.0f) {
        return false;
    } else if (self->unk_1E0.unk_00 != 0) {
        return true;
    } else if (DECR(self->unk_21C)) {
        if ((self->unk_21C & 1)) {
            self->actor.world.pos.y += 1.5f;
        } else {
            self->actor.world.pos.y -= 1.5f;
        }
        return true;
    } else {
        self->unk_21A--;
        if (self->unk_21A <= 0) {
            if (self->unk_21A == 0) {
                self->unk_21C = Rand_S16Offset(60, 30);
                self->unk_21A = 0;
                self->actor.velocity.y = 0.0f;
                return true;
            }
            self->unk_21A = unkArg1;
        }
        self->actor.velocity.y = ((f32)self->unk_21A / (f32)unkArg1) * unkArg2;
        return true;
    }
}

void func_80A3F908(EnGo* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 float1;
    s32 isUnkCondition;

    if (self->actionFunc == EnGo_BiggoronActionFunc || self->actionFunc == EnGo_GoronLinkRolling ||
        self->actionFunc == EnGo_FireGenericActionFunc || self->actionFunc == EnGo_Eyedrops ||
        self->actionFunc == func_80A40DCC || self->actionFunc == EnGo_GetItem || self->actionFunc == func_80A40C78 ||
        self->actionFunc == func_80A40B1C) {

        float1 = (self->collider.dim.radius + 30.0f);
        float1 *= (self->actor.scale.x / 0.01f);
        if ((self->actor.params & 0xF0) == 0x90) {
            float1 *= 4.8f;
        }

        if ((self->actor.params & 0xF0) == 0x90) {
            isUnkCondition =
                func_80A3ED24(globalCtx, self, &self->unk_1E0, float1, EnGo_GetTextID, EnGo_SetFlagsGetStates);
        } else {
            isUnkCondition = func_800343CC(globalCtx, &self->actor, &self->unk_1E0.unk_00, float1, EnGo_GetTextID,
                                           EnGo_SetFlagsGetStates);
        }

        if (((self->actor.params & 0xF0) == 0x90) && (isUnkCondition == true)) {
            if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_SWORD_BROKEN) {
                if (func_8002F368(globalCtx) == EXCH_ITEM_SWORD_BROKEN) {
                    if (gSaveContext.infTable[11] & 0x10) {
                        self->actor.textId = 0x3055;
                    } else {
                        self->actor.textId = 0x3054;
                    }
                } else {
                    self->actor.textId = 0x3053;
                }
                player->actor.textId = self->actor.textId;
            }

            if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_EYEDROPS) {
                if (func_8002F368(globalCtx) == EXCH_ITEM_EYEDROPS) {
                    self->actor.textId = 0x3059;
                } else {
                    self->actor.textId = 0x3058;
                }
                player->actor.textId = self->actor.textId;
            }
        }
    }
}

void EnGo_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGo* self = THIS;
    s32 pad;
    Vec3f D_80A41B9C = { 0.0f, 0.0f, 0.0f }; // unused
    Vec3f D_80A41BA8 = { 0.0f, 0.0f, 0.0f }; // unused

    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGoronSkel, NULL, 0, 0, 0);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(0x16), &sColChkInfoInit);

    if (!EnGo_IsActorSpawned(self, globalCtx)) {
        Actor_Kill(&self->actor);
        return;
    }

    if ((self->actor.params & 0xF0) && ((self->actor.params & 0xF0) != 0x90)) {
        self->actor.flags &= ~0x10;
        self->actor.flags &= ~0x20;
    }

    EnGo_ChangeAnimation(self, 0);
    self->actor.targetMode = 6;
    self->unk_1E0.unk_00 = 0;
    self->actor.gravity = -1.0f;

    switch (self->actor.params & 0xF0) {
        case 0x00:
            Actor_SetScale(&self->actor, 0.008f);
            if (CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1)) {
                EnGo_SetMovedPos(self, globalCtx);
                EnGo_SetupAction(self, EnGo_CurledUp);
            } else {
                self->actor.shape.yOffset = 1400.0f;
                self->actor.speedXZ = 3.0f;
                EnGo_SetupAction(self, EnGo_GoronLinkRolling);
            }
            break;
        case 0x10:
            self->skelAnime.curFrame = Animation_GetLastFrame(&gGoronAnim_004930);
            Actor_SetScale(&self->actor, 0.01f);
            EnGo_SetupAction(self, EnGo_FireGenericActionFunc);
            break;
        case 0x40:
            if (gSaveContext.infTable[14] & 0x800) {
                EnGo_SetMovedPos(self, globalCtx);
            }
            Actor_SetScale(&self->actor, 0.015f);
            EnGo_SetupAction(self, EnGo_CurledUp);
            break;
        case 0x30:
            self->actor.shape.yOffset = 1400.0f;
            Actor_SetScale(&self->actor, 0.01f);
            EnGo_SetupAction(self, func_80A3FEB4);
            break;
        case 0x90:
            self->actor.targetMode = 5;
            Actor_SetScale(&self->actor, 0.16f);
            EnGo_SetupAction(self, EnGo_CurledUp);
            break;
        case 0x20:
        case 0x50:
        case 0x60:
        case 0x70:
            Actor_SetScale(&self->actor, 0.01f);
            EnGo_SetupAction(self, EnGo_CurledUp);
            break;
        default:
            Actor_Kill(&self->actor);
    }
}

void EnGo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGo* self = THIS;

    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_80A3FEB4(EnGo* self, GlobalContext* globalCtx) {
    if (!(self->actor.xyzDistToPlayerSq > SQ(1200.0f))) {
        EnGo_SetupAction(self, EnGo_StopRolling);
    }
}

void EnGo_StopRolling(EnGo* self, GlobalContext* globalCtx) {
    EnBom* bomb;

    if (DECR(self->unk_20E) == 0) {
        if (self->collider.base.ocFlags2 & 1) {
            self->collider.base.ocFlags2 &= ~1;
            globalCtx->damagePlayer(globalCtx, -4);
            func_8002F71C(globalCtx, &self->actor, 4.0f, self->actor.yawTowardsPlayer, 6.0f);
            self->unk_20E = 0x10;
        }
    }

    self->actor.speedXZ = 3.0f;
    if ((EnGo_FollowPath(self, globalCtx) == true) && (self->unk_218 == 0)) {
        bomb = (EnBom*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_BOM, self->actor.world.pos.x,
                                   self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
        if (bomb != NULL) {
            bomb->timer = 0;
        }

        self->actor.speedXZ = 0.0f;
        EnGo_SetupAction(self, func_80A4008C);
    }

    self->actor.shape.rot = self->actor.world.rot;

    if (EnGo_IsRollingOnGround(self, 3, 6.0f)) {
        EnGo_SpawnDust(self, 12, 0.16f, 0.1f, 1, 10.0f, 20.0f);
    }
}

void func_80A4008C(EnGo* self, GlobalContext* globalCtx) {
    if (EnGo_IsRollingOnGround(self, 3, 6.0f)) {
        if (self->unk_21A == 0) {
            self->actor.shape.yOffset = 0.0f;
            EnGo_SetupAction(self, EnGo_CurledUp);
        } else {
            EnGo_SpawnDust(self, 12, 0.16f, 0.1f, 1, 10.0f, 20.0f);
        }
    }
}

void EnGo_GoronLinkRolling(EnGo* self, GlobalContext* globalCtx) {
    if ((EnGo_FollowPath(self, globalCtx) == true) && Flags_GetSwitch(globalCtx, self->actor.params >> 8) &&
        (self->unk_218 == 0)) {
        self->actor.speedXZ = 0.0f;
        EnGo_SetupAction(self, func_80A4008C);
        gSaveContext.infTable[16] |= 0x200;
    }

    self->actor.shape.rot = self->actor.world.rot;

    if (EnGo_IsRollingOnGround(self, 3, 6.0f)) {
        EnGo_SpawnDust(self, 12, 0.18f, 0.2f, 2, 13.0f, 20.0f);
    }
}

void EnGo_FireGenericActionFunc(EnGo* self, GlobalContext* globalCtx) {
}

void EnGo_CurledUp(EnGo* self, GlobalContext* globalCtx) {
    if ((DECR(self->unk_210) == 0) && EnGo_IsCameraModified(self, globalCtx)) {
        Audio_PlaySoundGeneral(NA_SE_EN_GOLON_WAKE_UP, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);

        self->skelAnime.playSpeed = 0.1f;
        self->skelAnime.playSpeed *= (self->actor.params & 0xF0) == 0x90 ? 0.5f : 1.0f;

        EnGo_SetupAction(self, EnGo_WakeUp);
        if ((self->actor.params & 0xF0) == 0x90) {
            OnePointCutscene_Init(globalCtx, 4200, -99, &self->actor, MAIN_CAM);
        }
    }
}

void EnGo_WakeUp(EnGo* self, GlobalContext* globalCtx) {
    f32 frame;

    if (self->skelAnime.playSpeed != 0.0f) {
        Math_SmoothStepToF(&self->skelAnime.playSpeed, ((self->actor.params & 0xF0) == 0x90 ? 0.5f : 1.0f) * 0.5f, 0.1f,
                           1000.0f, 0.1f);
        frame = self->skelAnime.curFrame;
        frame += self->skelAnime.playSpeed;

        if (frame <= 12.0f) {
            return;
        } else {
            self->skelAnime.curFrame = 12.0f;
            self->skelAnime.playSpeed = 0.0f;
            if ((self->actor.params & 0xF0) != 0x90) {
                self->unk_212 = 30;
                return;
            }
        }
    }

    if (DECR(self->unk_212) == 0) {
        Audio_PlaySoundGeneral(NA_SE_EN_GOLON_SIT_DOWN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        EnGo_SetupAction(self, func_80A405CC);
    } else if (!EnGo_IsCameraModified(self, globalCtx)) {
        EnGo_ReverseAnimation(self);
        self->skelAnime.playSpeed = 0.0f;
        EnGo_SetupAction(self, func_80A40494);
    }
}

void func_80A40494(EnGo* self, GlobalContext* globalCtx) {
    f32 frame;

    Math_SmoothStepToF(&self->skelAnime.playSpeed, ((self->actor.params & 0xF0) == 0x90 ? 0.5f : 1.0f) * -0.5f, 0.1f,
                       1000.0f, 0.1f);
    frame = self->skelAnime.curFrame;
    frame += self->skelAnime.playSpeed;

    if (!(frame >= 0.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EN_DODO_M_GND, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        EnGo_SpawnDust(self, 10, 0.4f, 0.1f, 16, 26.0f, 2.0f);
        EnGo_ReverseAnimation(self);
        self->skelAnime.playSpeed = 0.0f;
        self->skelAnime.curFrame = 0.0f;
        self->unk_210 = Rand_S16Offset(30, 30);
        EnGo_SetupAction(self, EnGo_CurledUp);
    }
}

void func_80A405CC(EnGo* self, GlobalContext* globalCtx) {
    f32 lastFrame;
    f32 frame;

    lastFrame = Animation_GetLastFrame(&gGoronAnim_004930);
    Math_SmoothStepToF(&self->skelAnime.playSpeed, (self->actor.params & 0xF0) == 0x90 ? 0.5f : 1.0f, 0.1f, 1000.0f,
                       0.1f);

    frame = self->skelAnime.curFrame;
    frame += self->skelAnime.playSpeed;

    if (!(frame < lastFrame)) {
        self->skelAnime.curFrame = lastFrame;
        self->skelAnime.playSpeed = 0.0f;
        self->unk_212 = Rand_S16Offset(30, 30);
        if (((self->actor.params & 0xF0) == 0x40) && ((gSaveContext.infTable[14] & 0x800) == 0)) {
            EnGo_SetupAction(self, func_80A40B1C);
        } else {
            EnGo_SetupAction(self, EnGo_BiggoronActionFunc);
        }
    }
}

void EnGo_BiggoronActionFunc(EnGo* self, GlobalContext* globalCtx) {
    if (((self->actor.params & 0xF0) == 0x90) && (self->unk_1E0.unk_00 == 2)) {
        if (gSaveContext.bgsFlag) {
            self->unk_1E0.unk_00 = 0;
        } else {
            if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_EYEDROPS) {
                EnGo_ChangeAnimation(self, 2);
                self->unk_21E = 100;
                self->unk_1E0.unk_00 = 0;
                EnGo_SetupAction(self, EnGo_Eyedrops);
                globalCtx->msgCtx.msgMode = 0x37;
                gSaveContext.timer2State = 0;
                OnePointCutscene_Init(globalCtx, 4190, -99, &self->actor, MAIN_CAM);
            } else {
                self->unk_1E0.unk_00 = 0;
                EnGo_SetupAction(self, EnGo_GetItem);
                func_80106CCC(globalCtx);
                EnGo_GetItem(self, globalCtx);
            }
        }
    } else if (((self->actor.params & 0xF0) == 0) && (self->unk_1E0.unk_00 == 2)) {
        EnGo_SetupAction(self, EnGo_GetItem);
        globalCtx->msgCtx.unk_E3E7 = 4;
        globalCtx->msgCtx.msgMode = 0x36;
    } else {
        if ((DECR(self->unk_212) == 0) && !EnGo_IsCameraModified(self, globalCtx)) {
            EnGo_ReverseAnimation(self);
            self->skelAnime.playSpeed = -0.1f;
            self->skelAnime.playSpeed *= (self->actor.params & 0xF0) == 0x90 ? 0.5f : 1.0f;
            EnGo_SetupAction(self, func_80A408D8);
        }
    }
}

void func_80A408D8(EnGo* self, GlobalContext* globalCtx) {
    f32 frame;

    if (self->skelAnime.playSpeed != 0.0f) {
        Math_SmoothStepToF(&self->skelAnime.playSpeed, ((self->actor.params & 0xF0) == 0x90 ? 0.5f : 1.0f) * -1.0f,
                           0.1f, 1000.0f, 0.1f);
        frame = self->skelAnime.curFrame;
        frame += self->skelAnime.playSpeed;
        if (frame >= 12.0f) {
            return;
        } else {
            self->skelAnime.curFrame = 12.0f;
            self->skelAnime.playSpeed = 0.0f;
            if ((self->actor.params & 0xF0) != 0x90) {
                self->unk_212 = 30;
                return;
            }
        }
    }

    if (DECR(self->unk_212) == 0) {
        EnGo_SetupAction(self, func_80A40494);
    } else if (EnGo_IsCameraModified(self, globalCtx)) {
        EnGo_ReverseAnimation(self);
        Audio_PlaySoundGeneral(NA_SE_EN_GOLON_SIT_DOWN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        self->skelAnime.playSpeed = 0.0f;
        EnGo_SetupAction(self, func_80A405CC);
    }
}

void func_80A40A54(EnGo* self, GlobalContext* globalCtx) {
    f32 float1 = ((f32)0x8000 / Animation_GetLastFrame(&gGoronAnim_010590));
    f32 float2 = self->skelAnime.curFrame * float1;

    self->actor.speedXZ = Math_SinS((s16)float2);
    if (EnGo_FollowPath(self, globalCtx) && self->unk_218 == 0) {
        EnGo_ChangeAnimation(self, 1);
        self->skelAnime.curFrame = Animation_GetLastFrame(&gGoronAnim_004930);
        self->actor.speedXZ = 0.0f;
        EnGo_SetupAction(self, EnGo_BiggoronActionFunc);
    }
}

void func_80A40B1C(EnGo* self, GlobalContext* globalCtx) {
    if (gSaveContext.infTable[14] & 0x800) {
        EnGo_ChangeAnimation(self, 3);
        EnGo_SetupAction(self, func_80A40A54);
    } else {
        EnGo_BiggoronActionFunc(self, globalCtx);
    }
}

void EnGo_GetItem(EnGo* self, GlobalContext* globalCtx) {
    f32 xzDist;
    f32 yDist;
    s32 getItemId;

    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->unk_1E0.unk_00 = 2;
        self->actor.parent = NULL;
        EnGo_SetupAction(self, func_80A40C78);
    } else {
        self->unk_20C = 0;
        if ((self->actor.params & 0xF0) == 0x90) {
            if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_CLAIM_CHECK) {
                getItemId = GI_SWORD_BGS;
                self->unk_20C = 1;
            }
            if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_EYEDROPS) {
                getItemId = GI_CLAIM_CHECK;
            }
            if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_SWORD_BROKEN) {
                getItemId = GI_PRESCRIPTION;
            }
        }

        if ((self->actor.params & 0xF0) == 0) {
            getItemId = GI_TUNIC_GORON;
        }

        yDist = fabsf(self->actor.yDistToPlayer) + 1.0f;
        xzDist = self->actor.xzDistToPlayer + 1.0f;
        func_8002F434(&self->actor, globalCtx, getItemId, xzDist, yDist);
    }
}

void func_80A40C78(EnGo* self, GlobalContext* globalCtx) {
    if (self->unk_1E0.unk_00 == 3) {
        EnGo_SetupAction(self, EnGo_BiggoronActionFunc);
        if ((self->actor.params & 0xF0) != 0x90) {
            self->unk_1E0.unk_00 = 0;
        } else if (self->unk_20C) {
            self->unk_1E0.unk_00 = 0;
            gSaveContext.bgsFlag = true;
        } else if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_PRESCRIPTION) {
            self->actor.textId = 0x3058;
            func_8010B720(globalCtx, self->actor.textId);
            self->unk_1E0.unk_00 = 1;
        } else if (INV_CONTENT(ITEM_TRADE_ADULT) == ITEM_CLAIM_CHECK) {
            self->actor.textId = 0x305C;
            func_8010B720(globalCtx, self->actor.textId);
            self->unk_1E0.unk_00 = 1;
            Environment_ClearBgsDayCount();
        }
    }
}

void EnGo_Eyedrops(EnGo* self, GlobalContext* globalCtx) {
    if (DECR(self->unk_21E) == 0) {
        self->actor.textId = 0x305A;
        func_8010B720(globalCtx, self->actor.textId);
        self->unk_1E0.unk_00 = 1;
        EnGo_SetupAction(self, func_80A40DCC);
    }
}

void func_80A40DCC(EnGo* self, GlobalContext* globalCtx) {
    if (self->unk_1E0.unk_00 == 2) {
        EnGo_ChangeAnimation(self, 1);
        self->skelAnime.curFrame = Animation_GetLastFrame(&gGoronAnim_004930);
        func_80106CCC(globalCtx);
        EnGo_SetupAction(self, EnGo_GetItem);
        EnGo_GetItem(self, globalCtx);
    }
}

void EnGo_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnGo* self = THIS;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    SkelAnime_Update(&self->skelAnime);

    if (self->actionFunc == EnGo_BiggoronActionFunc || self->actionFunc == EnGo_FireGenericActionFunc ||
        self->actionFunc == func_80A40B1C) {
        func_80034F54(globalCtx, self->jointTable, self->morphTable, 18);
    }

    EnGo_UpdateShadow(self);

    if (self->unk_1E0.unk_00 == 0) {
        Actor_MoveForward(&self->actor);
    }

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 0.0f, 0.0f, 4);
    func_80A3F0E4(self);
    func_80A3F908(self, globalCtx);
    self->actionFunc(self, globalCtx);
    func_80A3F060(self, globalCtx);
}

void EnGo_DrawCurledUp(EnGo* self, GlobalContext* globalCtx) {
    Vec3f D_80A41BB4 = { 0.0f, 0.0f, 0.0f };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_go.c", 2320);

    Matrix_Push();
    func_80093D18(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_go.c", 2326),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, gGoronDL_00BD80);

    Matrix_MultVec3f(&D_80A41BB4, &self->actor.focus.pos);
    Matrix_Pop();

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_go.c", 2341);
}

void EnGo_DrawRolling(EnGo* self, GlobalContext* globalCtx) {
    Vec3f D_80A41BC0 = { 0.0f, 0.0f, 0.0f };

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_go.c", 2355);

    Matrix_Push();
    func_80093D18(globalCtx->state.gfxCtx);
    Matrix_RotateRPY((s16)(globalCtx->state.frames * ((s16)self->actor.speedXZ * 1400)), 0, self->actor.shape.rot.z,
                     MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_go.c", 2368),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gGoronDL_00C140);
    Matrix_MultVec3f(&D_80A41BC0, &self->actor.focus.pos);
    Matrix_Pop();

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_go.c", 2383);
}

s32 EnGo_OverrideLimbDraw(GlobalContext* globalCtx, s32 limb, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    EnGo* self = THIS;
    Vec3s vec1;
    f32 float1;

    if (limb == 17) {
        Matrix_Translate(2800.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        vec1 = self->unk_1E0.unk_08;
        float1 = (vec1.y / (f32)0x8000) * M_PI;
        Matrix_RotateX(float1, MTXMODE_APPLY);
        float1 = (vec1.x / (f32)0x8000) * M_PI;
        Matrix_RotateZ(float1, MTXMODE_APPLY);
        Matrix_Translate(-2800.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }

    if (limb == 10) {
        vec1 = self->unk_1E0.unk_0E;
        float1 = (vec1.y / (f32)0x8000) * M_PI;
        Matrix_RotateY(float1, MTXMODE_APPLY);
        float1 = (vec1.x / (f32)0x8000) * M_PI;
        Matrix_RotateX(float1, MTXMODE_APPLY);
    }

    if ((limb == 10) || (limb == 11) || (limb == 14)) {
        float1 = Math_SinS(self->jointTable[limb]);
        rot->y += float1 * 200.0f;
        float1 = Math_CosS(self->morphTable[limb]);
        rot->z += float1 * 200.0f;
    }

    return 0;
}

void EnGo_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    EnGo* self = THIS;
    Vec3f D_80A41BCC = { 600.0f, 0.0f, 0.0f };

    if (limbIndex == 17) {
        Matrix_MultVec3f(&D_80A41BCC, &self->actor.focus.pos);
    }
}

void EnGo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnGo* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_go.c", 2479);

    EnGo_UpdateDust(self);
    Matrix_Push();
    EnGo_DrawDust(self, globalCtx);
    Matrix_Pop();

    if (self->actionFunc == EnGo_CurledUp) {
        EnGo_DrawCurledUp(self, globalCtx);
        return; // needed for match?
    } else if (self->actionFunc == EnGo_GoronLinkRolling || self->actionFunc == func_80A3FEB4 ||
               self->actionFunc == EnGo_StopRolling || self->actionFunc == func_80A3FEB4) {
        EnGo_DrawRolling(self, globalCtx);
        return; // needed for match?
    } else {
        func_800943C8(globalCtx->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(gGoronCsEyeOpenTex));
        gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(gGoronCsMouthNeutralTex));

        SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                              self->skelAnime.dListCount, EnGo_OverrideLimbDraw, EnGo_PostLimbDraw, &self->actor);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_go.c", 2525);
        EnGo_DrawDust(self, globalCtx);
    }
}

void EnGo_AddDust(EnGo* self, Vec3f* pos, Vec3f* velocity, Vec3f* accel, u8 initialTimer, f32 scale, f32 scaleStep) {
    EnGoEffect* dustEffect = self->dustEffects;
    s16 i;
    s16 timer;

    for (i = 0; i < ARRAY_COUNT(self->dustEffects); i++, dustEffect++) {
        if (dustEffect->type != 1) {
            dustEffect->scale = scale;
            dustEffect->scaleStep = scaleStep;
            if (1) {}
            timer = initialTimer;
            dustEffect->timer = timer;
            dustEffect->type = 1;
            dustEffect->initialTimer = initialTimer;
            dustEffect->pos = *pos;
            dustEffect->accel = *accel;
            dustEffect->velocity = *velocity;
            return;
        }
    }
}

void EnGo_UpdateDust(EnGo* self) {
    EnGoEffect* dustEffect = self->dustEffects;
    f32 randomNumber;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(self->dustEffects); i++, dustEffect++) {
        if (dustEffect->type) {
            dustEffect->timer--;
            if (dustEffect->timer == 0) {
                dustEffect->type = 0;
            }

            dustEffect->accel.x = (Rand_ZeroOne() * 0.4f) - 0.2f;
            randomNumber = Rand_ZeroOne() * 0.4f;
            dustEffect->accel.z = randomNumber - 0.2f;
            dustEffect->pos.x += dustEffect->velocity.x;
            dustEffect->pos.y += dustEffect->velocity.y;
            dustEffect->pos.z += dustEffect->velocity.z;
            dustEffect->velocity.x += dustEffect->accel.x;
            dustEffect->velocity.y += dustEffect->accel.y;
            dustEffect->velocity.z += randomNumber - 0.2f;
            dustEffect->scale += dustEffect->scaleStep;
        }
    }
}

void EnGo_DrawDust(EnGo* self, GlobalContext* globalCtx) {
    static void* dustTex[] = { gDust8Tex, gDust7Tex, gDust6Tex, gDust5Tex, gDust4Tex, gDust3Tex, gDust2Tex, gDust1Tex };
    EnGoEffect* dustEffect = self->dustEffects;
    s16 alpha;
    s16 firstDone;
    s16 index;
    s16 i;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_go.c", 2626);
    firstDone = false;
    func_80093D84(globalCtx->state.gfxCtx);
    if (1) {}
    for (i = 0; i < ARRAY_COUNT(self->dustEffects); i++, dustEffect++) {
        if (dustEffect->type) {
            if (!firstDone) {
                POLY_XLU_DISP = Gfx_CallSetupDL(POLY_XLU_DISP, 0);
                gSPDisplayList(POLY_XLU_DISP++, gGoronDL_00FD40);
                gDPSetEnvColor(POLY_XLU_DISP++, 100, 60, 20, 0);
                firstDone = true;
            }

            alpha = dustEffect->timer * (255.0f / dustEffect->initialTimer);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 170, 130, 90, alpha);
            gDPPipeSync(POLY_XLU_DISP++);
            Matrix_Translate(dustEffect->pos.x, dustEffect->pos.y, dustEffect->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(dustEffect->scale, dustEffect->scale, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_go.c", 2664),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            index = dustEffect->timer * (8.0f / dustEffect->initialTimer);
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(dustTex[index]));
            gSPDisplayList(POLY_XLU_DISP++, gGoronDL_00FD50);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_go.c", 2678);
}
