#include "z_en_skj.h"
#include "overlays/actors/ovl_En_Skjneedle/z_en_skjneedle.h"
#include "objects/object_skj/object_skj.h"

#define FLAGS 0x02000015

#define THIS ((EnSkj*)thisx)

void EnSkj_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSkj_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSkj_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSkj_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnSkj_SariasSongShortStumpUpdate(Actor* thisx, GlobalContext* globalCtx);
void EnSkj_OcarinaMinigameShortStumpUpdate(Actor* thisx, GlobalContext* globalCtx);

void func_80AFF2A0(EnSkj* self);
void func_80AFF334(EnSkj* self);

void EnSkj_CalculateCenter(EnSkj* self);
void EnSkj_OcarinaGameSetupWaitForPlayer(EnSkj* self);
void EnSkj_SetupResetFight(EnSkj* self);
void EnSkj_SetupLeaveOcarinaGame(EnSkj* self);
void EnSkj_SetupPlayOcarinaGame(EnSkj* self);
void EnSkj_Backflip(EnSkj* self);
void EnSkj_SetupNeedleRecover(EnSkj* self);
void EnSkj_SetupSpawnDeathEffect(EnSkj* self);
void EnSkj_SetupStand(EnSkj* self);
void EnSkj_SetupWaitForSong(EnSkj* self);
void EnSkj_SetupTalk(EnSkj* self);
void EnSkj_SetupMaskTrade(EnSkj* self);
void EnSkj_SetupWrongSong(EnSkj* self);
void EnSkj_SetupAfterSong(EnSkj* self);
void func_80AFFE24(EnSkj* self);
void EnSkj_SetupPostSariasSong(EnSkj* self);
void EnSkj_JumpFromStump(EnSkj* self);
void EnSkj_SetupWaitForLandAnimFinish(EnSkj* self);
void EnSkj_SetupWalkToPlayer(EnSkj* self);
void EnSkj_SetupWaitForMaskTextClear(EnSkj* self);
void EnSkj_SetupWaitForTextClear(EnSkj* self);
void EnSkj_SetupDie(EnSkj* self);
void func_80AFF1F0(EnSkj* self);
void EnSkj_OfferNextRound(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_SetupAskForMask(EnSkj* self, GlobalContext* globalCtx);
f32 EnSkj_GetItemXzRange(EnSkj* self);
s32 EnSkj_CollisionCheck(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_SetupTakeMask(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_TurnPlayer(EnSkj* self, Player* player);

void EnSkj_SetupWaitForOcarina(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_StartOcarinaMinigame(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForOcarina(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForPlayback(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_FailedMiniGame(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WonOcarinaMiniGame(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitToGiveReward(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_GiveOcarinaGameReward(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_FinishOcarinaGameRound(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForNextRound(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForOfferResponse(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_SetupWaitForOcarina(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_CleanupOcarinaGame(EnSkj* self, GlobalContext* globalCtx);

void EnSkj_Fade(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitToShootNeedle(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_SariasSongKidIdle(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForDeathAnim(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_PickNextFightAction(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForLandAnim(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_ResetFight(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_Fight(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_NeedleRecover(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_SpawnDeathEffect(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitInRange(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForSong(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_AfterSong(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_SariaSongTalk(EnSkj* self, GlobalContext* globalCtx);
void func_80AFFE44(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_ChangeModeAfterSong(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_StartMaskTrade(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForLanding(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForLandAnimFinish(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WalkToPlayer(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_AskForMask(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_TakeMask(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WaitForMaskTextClear(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_WrongSong(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_SariasSongWaitForTextClear(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_OcarinaGameWaitForPlayer(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_OcarinaGameIdle(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_PlayOcarinaGame(EnSkj* self, GlobalContext* globalCtx);
void EnSkj_LeaveOcarinaGame(EnSkj* self, GlobalContext* globalCtx);

void EnSkj_SpawnBlood(GlobalContext* globalCtx, Vec3f* pos);

void EnSkj_SetupWaitInRange(EnSkj* self);

#define songFailTimer multiuseTimer
#define battleExitTimer multiuseTimer

typedef enum {
    /* 0 */ SKJ_ANIM_BACKFLIP,
    /* 1 */ SKJ_ANIM_SHOOT_NEEDLE,
    /* 2 */ SKJ_ANIM_PLAY_FLUTE,
    /* 3 */ SKJ_ANIM_DIE,
    /* 4 */ SKJ_ANIM_HIT,
    /* 5 */ SKJ_ANIM_LAND,
    /* 6 */ SKJ_ANIM_LOOK_LEFT_RIGHT,
    /* 7 */ SKJ_ANIM_FIGHTING_STANCE,
    /* 8 */ SKJ_ANIM_WALK_TO_PLAYER,
    /* 9 */ SKJ_ANIM_WAIT
} SkullKidAnim;

typedef enum {
    /* 0 */ SKULL_KID_LEFT,
    /* 1 */ SKULL_KID_RIGHT
} SkullKidStumpSide;

typedef enum {
    /* 0 */ SKULL_KID_OCRAINA_WAIT,
    /* 1 */ SKULL_KID_OCARINA_PLAY_NOTES,
    /* 2 */ SKULL_KID_OCARINA_LEAVE_GAME
} SkullKidOcarinaGameState;

typedef enum {
    /* 00 */ SKJ_ACTION_FADE,
    /* 01 */ SKJ_ACTION_WAIT_TO_SHOOT_NEEDLE,
    /* 02 */ SKJ_ACTION_SARIA_SONG_IDLE,
    /* 03 */ SKJ_ACTION_WAIT_FOR_DEATH_ANIM,
    /* 04 */ SKJ_ACTION_PICK_NEXT_FIHGT_ACTION,
    /* 05 */ SKJ_ACTION_WAIT_FOR_LAND_ANIM,
    /* 06 */ SKJ_ACTION_RESET_FIGHT,
    /* 07 */ SKJ_ACTION_FIGHT,
    /* 08 */ SKJ_ACTION_NEEDLE_RECOVER,
    /* 09 */ SKJ_ACTION_SPAWN_DEATH_EFFECT,
    /* 10 */ SKJ_ACTION_SARIA_SONG_WAIT_IN_RANGE,
    /* 11 */ SKJ_ACTION_SARIA_SONG_WAIT_FOR_SONG,
    /* 12 */ SKJ_ACTION_SARIA_SONG_AFTER_SONG,
    /* 13 */ SKJ_ACTION_SARIA_TALK,
    /* 14 */ SKJ_ACTION_UNK14,
    /* 15 */ SKJ_ACTION_SARIA_SONG_CHANGE_MODE,
    /* 16 */ SKJ_ACTION_SARIA_SONG_START_TRADE,
    /* 17 */ SKJ_ACTION_SARIA_SONG_WAIT_FOR_LANDING,
    /* 18 */ SKJ_ACTION_SARIA_SONG_WAIT_FOR_LANDING_ANIM,
    /* 19 */ SKJ_ACTION_SARIA_SONG_WALK_TO_PLAYER,
    /* 20 */ SKJ_ACTION_SARIA_SONG_ASK_FOR_MASK,
    /* 21 */ SKJ_ACTION_SARIA_SONG_TAKE_MASK,
    /* 22 */ SKJ_ACTION_SARIA_SONG_WAIT_MASK_TEXT,
    /* 23 */ SKJ_ACTION_SARIA_SONG_WRONG_SONG,
    /* 24 */ SKJ_ACTION_SARIA_SONG_WAIT_FOR_TEXT,
    /* 25 */ SKJ_ACTION_OCARINA_GAME_WAIT_FOR_PLAYER,
    /* 26 */ SKJ_ACTION_OCARINA_GAME_IDLE,
    /* 27 */ SKJ_ACTION_OCARINA_GAME_PLAY,
    /* 28 */ SKJ_ACTION_OCARINA_GAME_LEAVE
} SkullKidAction;

typedef struct {
    u8 unk0;
    EnSkj* skullkid;
} unkSkjStruct;

static unkSkjStruct sSmallStumpSkullKid = { 0, NULL };
static unkSkjStruct sOcarinaMinigameSkullKids[] = { { 0, NULL }, { 0, NULL } };

const ActorInit En_Skj_InitVars = {
    ACTOR_EN_SKJ,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SKJ,
    sizeof(EnSkj),
    (ActorFunc)EnSkj_Init,
    (ActorFunc)EnSkj_Destroy,
    (ActorFunc)EnSkj_Update,
    (ActorFunc)EnSkj_Draw,
};

static ColliderCylinderInitType1 D_80B01678 = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x0, 0x08 },
        { 0xFFCFFFFF, 0x0, 0x0 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 8, 48, 0, { 0, 0, 0 } },
};

static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(0, 0x0),
    /* Deku stick    */ DMG_ENTRY(0, 0x0),
    /* Slingshot     */ DMG_ENTRY(0, 0x0),
    /* Explosive     */ DMG_ENTRY(0, 0x0),
    /* Boomerang     */ DMG_ENTRY(0, 0x0),
    /* Normal arrow  */ DMG_ENTRY(0, 0x0),
    /* Hammer swing  */ DMG_ENTRY(2, 0x0),
    /* Hookshot      */ DMG_ENTRY(0, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0xF),
    /* Master sword  */ DMG_ENTRY(2, 0xF),
    /* Giant's Knife */ DMG_ENTRY(4, 0xF),
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
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(4, 0x0),
    /* Master spin   */ DMG_ENTRY(2, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(2, 0x0),
    /* Giant jump    */ DMG_ENTRY(8, 0x0),
    /* Master jump   */ DMG_ENTRY(4, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(0, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static s32 sOcarinaGameRewards[] = {
    GI_RUPEE_GREEN,
    GI_RUPEE_BLUE,
    GI_HEART_PIECE,
    GI_RUPEE_RED,
};

typedef struct {
    AnimationHeader* animation;
    u8 mode;
    f32 morphFrames;
} SkullkidAnimationEntry;

static SkullkidAnimationEntry sSkullKidAnimations[] = {
    { &gSkullKidBackflipAnim, ANIMMODE_ONCE, 0.0f },
    { &gSkullKidShootNeedleAnim, ANIMMODE_ONCE, 0.0f },
    { &gSkullKidPlayFluteAnim, ANIMMODE_LOOP, 0.0f },
    { &gSkullKidDieAnim, ANIMMODE_ONCE, 0.0f },
    { &gSkullKidHitAnim, ANIMMODE_ONCE, 0.0f },
    { &gSkullKidLandAnim, ANIMMODE_ONCE, 0.0f },
    { &gSkullKidLookLeftAndRightAnim, ANIMMODE_LOOP, 0.0f },
    { &gSkullKidFightingStanceAnim, ANIMMODE_LOOP, 0.0f },
    { &gSkullKidWalkToPlayerAnim, ANIMMODE_LOOP, 0.0f },
    { &gSkullKidWaitAnim, ANIMMODE_LOOP, 0.0f },
};

static EnSkjActionFunc sActionFuncs[] = {
    EnSkj_Fade,
    EnSkj_WaitToShootNeedle,
    EnSkj_SariasSongKidIdle,
    EnSkj_WaitForDeathAnim,
    EnSkj_PickNextFightAction,
    EnSkj_WaitForLandAnim,
    EnSkj_ResetFight,
    EnSkj_Fight,
    EnSkj_NeedleRecover,
    EnSkj_SpawnDeathEffect,
    EnSkj_WaitInRange,
    EnSkj_WaitForSong,
    EnSkj_AfterSong,
    EnSkj_SariaSongTalk,
    func_80AFFE44,
    EnSkj_ChangeModeAfterSong,
    EnSkj_StartMaskTrade,
    EnSkj_WaitForLanding,
    EnSkj_WaitForLandAnimFinish,
    EnSkj_WalkToPlayer,
    EnSkj_AskForMask,
    EnSkj_TakeMask,
    EnSkj_WaitForMaskTextClear,
    EnSkj_WrongSong,
    EnSkj_SariasSongWaitForTextClear,
    EnSkj_OcarinaGameWaitForPlayer,
    EnSkj_OcarinaGameIdle,
    EnSkj_PlayOcarinaGame,
    EnSkj_LeaveOcarinaGame,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

static s32 D_80B01EA0; // gets set if actor flags & 0x100 is set

void EnSkj_ChangeAnim(EnSkj* self, u8 animIndex) {
    f32 endFrame = Animation_GetLastFrame(sSkullKidAnimations[animIndex].animation);

    self->animIndex = animIndex;
    Animation_Change(&self->skelAnime, sSkullKidAnimations[animIndex].animation, 1.0f, 0.0f, endFrame,
                     sSkullKidAnimations[animIndex].mode, sSkullKidAnimations[animIndex].morphFrames);
}

void EnSkj_SetupAction(EnSkj* self, u8 action) {
    self->action = action;
    self->actionFunc = sActionFuncs[action];

    switch (action) {
        case SKJ_ACTION_FADE:
        case SKJ_ACTION_WAIT_FOR_DEATH_ANIM:
        case SKJ_ACTION_PICK_NEXT_FIHGT_ACTION:
        case SKJ_ACTION_SPAWN_DEATH_EFFECT:
        case SKJ_ACTION_SARIA_SONG_START_TRADE:
        case SKJ_ACTION_SARIA_SONG_WAIT_FOR_LANDING:
        case SKJ_ACTION_SARIA_SONG_WAIT_FOR_LANDING_ANIM:
        case SKJ_ACTION_SARIA_SONG_WALK_TO_PLAYER:
        case SKJ_ACTION_SARIA_SONG_ASK_FOR_MASK:
        case SKJ_ACTION_SARIA_SONG_TAKE_MASK:
        case SKJ_ACTION_SARIA_SONG_WAIT_MASK_TEXT:
        case SKJ_ACTION_SARIA_SONG_WRONG_SONG:
        case SKJ_ACTION_SARIA_SONG_WAIT_FOR_TEXT:
        case SKJ_ACTION_OCARINA_GAME_WAIT_FOR_PLAYER:
        case SKJ_ACTION_OCARINA_GAME_IDLE:
        case SKJ_ACTION_OCARINA_GAME_PLAY:
        case SKJ_ACTION_OCARINA_GAME_LEAVE:
            self->unk_2D3 = 0;
            break;
        default:
            self->unk_2D3 = 1;
            break;
    }
}

void EnSkj_CalculateCenter(EnSkj* self) {
    Vec3f mult;

    mult.x = 0.0f;
    mult.y = 0.0f;
    mult.z = 120.0f;

    Matrix_RotateY((self->actor.shape.rot.y / 32768.0f) * M_PI, MTXMODE_NEW);
    Matrix_MultVec3f(&mult, &self->center);

    self->center.x += self->actor.world.pos.x;
    self->center.z += self->actor.world.pos.z;
}

void EnSkj_SetNaviId(EnSkj* self) {
    switch (self->actor.params) {
        case 0:
            if (gSaveContext.itemGetInf[3] & 0x200) {
                self->actor.naviEnemyId = 0x41; // Skull kid with skull mask
            } else if (gSaveContext.itemGetInf[1] & 0x40) {
                self->actor.naviEnemyId = 0x40; // Skull kid after Saria's song but no mask
            } else {
                self->actor.naviEnemyId = 0x3F; // No Sarias song no skull mask
            }
            break;

        case 1:
        case 2:
            self->actor.naviEnemyId = 0x3F;
            break;

        default:
            self->actor.naviEnemyId = 0x36; // Skull kid as adult
            break;
    }
}

void EnSkj_Init(Actor* thisx, GlobalContext* globalCtx2) {
    s16 type = (thisx->params >> 0xA) & 0x3F;
    EnSkj* self = (EnSkj*)thisx;
    GlobalContext* globalCtx = globalCtx2;
    s32 pad;
    Player* player;

    Actor_ProcessInitChain(thisx, sInitChain);
    switch (type) {
        case 5: // Invisible on the small stump (sarias song))
            sSmallStumpSkullKid.unk0 = 1;
            sSmallStumpSkullKid.skullkid = THIS;
            self->actor.destroy = NULL;
            self->actor.draw = NULL;
            self->actor.update = EnSkj_SariasSongShortStumpUpdate;
            self->actor.flags &= ~5;
            self->actor.flags |= 0;
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, thisx, ACTORCAT_PROP);
            break;

        case 6: // Invisible on the short stump (ocarina game)
            sSmallStumpSkullKid.unk0 = 1;
            sSmallStumpSkullKid.skullkid = THIS;
            self->actor.destroy = NULL;
            self->actor.draw = NULL;
            self->actor.update = EnSkj_OcarinaMinigameShortStumpUpdate;
            self->actor.flags &= ~5;
            self->actor.flags |= 0;
            Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, thisx, ACTORCAT_PROP);
            self->actor.focus.pos.x = 1230.0f;
            self->actor.focus.pos.y = -90.0f;
            self->actor.focus.pos.z = 450.0f;
            self->actionFunc = EnSkj_SetupWaitForOcarina;
            break;

        default:
            self->actor.params = type;
            if (((self->actor.params != 0) && (self->actor.params != 1)) && (self->actor.params != 2)) {
                if (INV_CONTENT(ITEM_TRADE_ADULT) < ITEM_SAW) {
                    Actor_Kill(&self->actor);
                    return;
                }
            }

            EnSkj_SetNaviId(self);
            SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gSkullKidSkel, &gSkullKidPlayFluteAnim, self->jointTable,
                               self->morphTable, 19);
            if ((type >= 0) && (type < 3)) {
                self->actor.flags &= ~5;
                self->actor.flags |= 9;
                Actor_ChangeCategory(globalCtx, &globalCtx->actorCtx, &self->actor, ACTORCAT_NPC);
            }

            if ((type < 0) || (type >= 7)) {
                self->actor.flags &= ~0x02000000;
            }

            if ((type > 0) && (type < 3)) {
                self->actor.targetMode = 7;
                self->posCopy = self->actor.world.pos;
                sOcarinaMinigameSkullKids[type - 1].unk0 = 1;
                sOcarinaMinigameSkullKids[type - 1].skullkid = self;
                self->minigameState = 0;
                self->alpha = 0;
                EnSkj_OcarinaGameSetupWaitForPlayer(self);
            } else {
                self->alpha = 255;
                EnSkj_SetupResetFight(self);
            }

            self->actor.colChkInfo.damageTable = &sDamageTable;
            self->actor.colChkInfo.health = 10;
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &D_80B01678);
            ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 40.0f);
            Actor_SetScale(thisx, 0.01f);
            self->actor.textId = self->textId = 0;
            self->multiuseTimer = 0;
            self->backfilpFlag = 0;
            self->needlesToShoot = 3;
            self->hitsUntilDodge = 3;
            self->actor.speedXZ = 0.0f;
            self->actor.velocity.y = 0.0f;
            self->actor.gravity = -1.0f;
            EnSkj_CalculateCenter(self);

            player = GET_PLAYER(globalCtx);
            osSyncPrintf("Player_X : %f\n", player->actor.world.pos.x);
            osSyncPrintf("Player_Z : %f\n", player->actor.world.pos.z);
            osSyncPrintf("World_X  : %f\n", self->actor.world.pos.x);
            osSyncPrintf("World_Z  : %f\n", self->actor.world.pos.z);
            osSyncPrintf("Center_X : %f\n", self->center.x);
            osSyncPrintf("Center_Z : %f\n\n", self->center.z);

            break;
    }
}

void EnSkj_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnSkj* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 EnSkj_RangeCheck(Player* player, EnSkj* self) {
    f32 xDiff = player->actor.world.pos.x - self->actor.world.pos.x;
    f32 zDiff = player->actor.world.pos.z - self->actor.world.pos.z;
    f32 yDiff = player->actor.world.pos.y - self->actor.world.pos.y;

    return (SQ(xDiff) + SQ(zDiff) <= 676.0f) && (yDiff >= 0.0f);
}

f32 EnSkj_GetItemXzRange(EnSkj* self) {
    EnSkj* temp_v0;
    f32 zDiff;
    f32 xDiff;

    temp_v0 = sSmallStumpSkullKid.skullkid;
    xDiff = temp_v0->actor.world.pos.x - self->actor.world.pos.x;
    zDiff = temp_v0->actor.world.pos.z - self->actor.world.pos.z;
    return sqrtf(SQ(xDiff) + SQ(zDiff)) + 26.0f;
}

f32 EnSkj_GetItemYRange(EnSkj* self) {
    return fabsf(sSmallStumpSkullKid.skullkid->actor.world.pos.y - self->actor.world.pos.y) + 10.0f;
}

s32 EnSkj_ShootNeedle(EnSkj* self, GlobalContext* globalCtx) {
    s32 pad;
    Vec3f pos;
    Vec3f pos2;
    EnSkjneedle* needle;

    pos.x = 1.5f;
    pos.y = 0.0f;
    pos.z = 40.0f;

    Matrix_RotateY((self->actor.shape.rot.y / 32768.0f) * M_PI, MTXMODE_NEW);
    Matrix_MultVec3f(&pos, &pos2);

    pos2.x += self->actor.world.pos.x;
    pos2.z += self->actor.world.pos.z;
    pos2.y = self->actor.world.pos.y + 27.0f;

    needle = (EnSkjneedle*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_SKJNEEDLE, pos2.x, pos2.y, pos2.z,
                                       self->actor.shape.rot.x, self->actor.shape.rot.y, self->actor.shape.rot.z, 0);
    if (needle != NULL) {
        needle->killTimer = 100;
        needle->actor.speedXZ = 24.0f;
        return 1;
    }
    return 0;
}

void EnSkj_SpawnBlood(GlobalContext* globalCtx, Vec3f* pos) {
    EffectSparkInit effect;
    s32 sp20;

    effect.position.x = pos->x;
    effect.position.y = pos->y;
    effect.position.z = pos->z;
    effect.uDiv = 5;
    effect.vDiv = 5;

    effect.colorStart[0].r = 0;
    effect.colorStart[0].g = 0;
    effect.colorStart[0].b = 128;
    effect.colorStart[0].a = 255;

    effect.colorStart[1].r = 0;
    effect.colorStart[1].g = 0;
    effect.colorStart[1].b = 128;
    effect.colorStart[1].a = 255;

    effect.colorStart[2].r = 0;
    effect.colorStart[2].g = 0;
    effect.colorStart[2].b = 128;
    effect.colorStart[2].a = 255;

    effect.colorStart[3].r = 0;
    effect.colorStart[3].g = 0;
    effect.colorStart[3].b = 128;
    effect.colorStart[3].a = 255;

    effect.colorEnd[0].r = 0;
    effect.colorEnd[0].g = 0;
    effect.colorEnd[0].b = 32;
    effect.colorEnd[0].a = 0;

    effect.colorEnd[1].r = 0;
    effect.colorEnd[1].g = 0;
    effect.colorEnd[1].b = 32;
    effect.colorEnd[1].a = 0;

    effect.colorEnd[2].r = 0;
    effect.colorEnd[2].g = 0;
    effect.colorEnd[2].b = 64;
    effect.colorEnd[2].a = 0;

    effect.colorEnd[3].r = 0;
    effect.colorEnd[3].g = 0;
    effect.colorEnd[3].b = 64;
    effect.colorEnd[3].a = 0;

    effect.speed = 8.0f;
    effect.gravity = -1.0f;

    effect.timer = 0;
    effect.duration = 8;

    Effect_Add(globalCtx, &sp20, EFFECT_SPARK, 0, 1, &effect);
}

s32 EnSkj_CollisionCheck(EnSkj* self, GlobalContext* globalCtx) {
    s16 yawDiff;
    Vec3f effectPos;

    if (!((self->unk_2D3 == 0) || (D_80B01EA0 != 0) || !(self->collider.base.acFlags & AC_HIT))) {
        self->collider.base.acFlags &= ~AC_HIT;
        if (self->actor.colChkInfo.damageEffect != 0) {
            if (self->actor.colChkInfo.damageEffect == 0xF) {
                effectPos.x = self->collider.info.bumper.hitPos.x;
                effectPos.y = self->collider.info.bumper.hitPos.y;
                effectPos.z = self->collider.info.bumper.hitPos.z;

                EnSkj_SpawnBlood(globalCtx, &effectPos);
                EffectSsHitMark_SpawnFixedScale(globalCtx, 1, &effectPos);

                yawDiff = self->actor.yawTowardsPlayer - self->actor.world.rot.y;
                if ((self->action == 2) || (self->action == 6)) {
                    if ((yawDiff > 0x6000) || (yawDiff < -0x6000)) {
                        Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);
                        EnSkj_SetupDie(self);
                        return 1;
                    }
                }

                Actor_ApplyDamage(&self->actor);
                Actor_SetColorFilter(&self->actor, 0x4000, 0xFF, 0, 8);

                if (self->actor.colChkInfo.health != 0) {
                    if (self->hitsUntilDodge != 0) {
                        self->hitsUntilDodge--;
                    }
                    if (self->dodgeResetTimer == 0) {
                        self->dodgeResetTimer = 60;
                    }
                    func_80AFF1F0(self);
                    return 1;
                }
                EnSkj_SetupDie(self);
                return 1;
            }
        } else {
            self->backfilpFlag = 1;
            EnSkj_Backflip(self);
            return 1;
        }
    }
    return 0;
}

s32 func_80AFEDF8(EnSkj* self, GlobalContext* globalCtx) {
    s16 yawDiff;

    if (self->actor.xzDistToPlayer < self->unk_2EC) {
        self = self;
        if (func_8002DDE4(globalCtx) != 0) {
            return 1;
        }
    }

    yawDiff = self->actor.yawTowardsPlayer - self->actor.world.rot.y;

    if ((yawDiff < self->unk_2C8) && (-self->unk_2C8 < yawDiff)) {
        return 1;
    }

    return 0;
}

void EnSkj_Backflip(EnSkj* self) {
    self->actor.velocity.y = 8.0f;
    self->actor.speedXZ = -8.0f;

    EnSkj_ChangeAnim(self, SKJ_ANIM_BACKFLIP);
    EnSkj_SetupAction(self, SKJ_ACTION_FADE);
}

void EnSkj_Fade(EnSkj* self, GlobalContext* globalCtx) {
    u32 alpha = self->alpha;

    if (self->unk_2D6 == 2) {
        globalCtx->msgCtx.unk_E3EE = 0;
        self->unk_2D6 = 0;
    }

    alpha -= 20;

    if (self->backfilpFlag != 0) {
        if (alpha > 255) {
            alpha = 0;
        }

        self->alpha = alpha;
        self->actor.shape.shadowAlpha = alpha;
    }

    if (self->actor.velocity.y <= 0.0f) {
        if (self->actor.bgCheckFlags & 2) {
            self->actor.bgCheckFlags &= ~2;
            func_80AFF2A0(self);
        }
    }
}

void EnSkj_SetupWaitToShootNeedle(EnSkj* self) {
    self->needlesToShoot = 3;
    self->needleShootTimer = 0;
    EnSkj_ChangeAnim(self, SKJ_ANIM_SHOOT_NEEDLE);
    EnSkj_SetupAction(self, SKJ_ACTION_WAIT_TO_SHOOT_NEEDLE);
}

void EnSkj_WaitToShootNeedle(EnSkj* self, GlobalContext* globalCtx) {
    u8 val;
    s16 lastFrame = Animation_GetLastFrame(&gSkullKidShootNeedleAnim);

    if ((self->skelAnime.curFrame == lastFrame) && (self->needleShootTimer == 0)) {
        val = self->needlesToShoot;
        if (self->needlesToShoot != 0) {
            EnSkj_ShootNeedle(self, globalCtx);
            self->needleShootTimer = 4;
            val--;
            self->needlesToShoot = val;

        } else {
            EnSkj_SetupNeedleRecover(self);
        }
    }
}

void EnSkj_SetupResetFight(EnSkj* self) {
    self->unk_2C8 = 0xAAA;
    self->unk_2EC = 200.0f;
    EnSkj_ChangeAnim(self, SKJ_ANIM_PLAY_FLUTE);
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_IDLE);
}

void EnSkj_SariasSongKidIdle(EnSkj* self, GlobalContext* globalCtx) {
    if (self->actor.params == 0) {
        if (!(gSaveContext.itemGetInf[1] & 0x40) && (self->actor.xzDistToPlayer < 200.0f)) {
            self->backfilpFlag = 1;
            EnSkj_Backflip(self);
        } else if (sSmallStumpSkullKid.unk0 != 0) {
            Player* player = GET_PLAYER(globalCtx);
            if (EnSkj_RangeCheck(player, sSmallStumpSkullKid.skullkid)) {
                EnSkj_SetupWaitInRange(self);
                player->stateFlags2 |= 0x800000;
                player->unk_6A8 = &sSmallStumpSkullKid.skullkid->actor;
            }
        }
    } else {
        if (func_80AFEDF8(self, globalCtx) != 0) {
            func_80AFF334(self);
        }
    }
}

void EnSkj_SetupDie(EnSkj* self) {
    EnSkj_ChangeAnim(self, SKJ_ANIM_DIE);
    EnSkj_SetupAction(self, SKJ_ACTION_WAIT_FOR_DEATH_ANIM);
}

void EnSkj_WaitForDeathAnim(EnSkj* self, GlobalContext* globalCtx) {
    s16 lastFrame = Animation_GetLastFrame(&gSkullKidDieAnim);

    if (self->skelAnime.curFrame == lastFrame) {
        EnSkj_SetupSpawnDeathEffect(self);
    }
}

void func_80AFF1F0(EnSkj* self) {
    EnSkj_ChangeAnim(self, SKJ_ANIM_HIT);
    EnSkj_SetupAction(self, SKJ_ACTION_PICK_NEXT_FIHGT_ACTION);
}

void EnSkj_PickNextFightAction(EnSkj* self, GlobalContext* globalCtx) {
    s16 lastFrame = Animation_GetLastFrame(&gSkullKidHitAnim);

    if (self->skelAnime.curFrame == lastFrame) {
        if (self->hitsUntilDodge == 0) {
            self->hitsUntilDodge = 3;
            EnSkj_Backflip(self);
        } else {
            EnSkj_SetupStand(self);
        }
    }
}

void func_80AFF2A0(EnSkj* self) {
    EnSkj_CalculateCenter(self);
    self->actor.speedXZ = 0.0f;
    EnSkj_ChangeAnim(self, SKJ_ANIM_LAND);
    EnSkj_SetupAction(self, SKJ_ACTION_WAIT_FOR_LAND_ANIM);
}

void EnSkj_WaitForLandAnim(EnSkj* self, GlobalContext* globalCtx) {
    s16 lastFrame = Animation_GetLastFrame(&gSkullKidLandAnim);

    if (self->skelAnime.curFrame == lastFrame) {
        EnSkj_SetupStand(self);
    }
}

void func_80AFF334(EnSkj* self) {
    self->unk_2C8 = 0x2000;
    self->battleExitTimer = 400;
    self->unk_2EC = 600.0f;
    EnSkj_ChangeAnim(self, SKJ_ANIM_LOOK_LEFT_RIGHT);
    EnSkj_SetupAction(self, SKJ_ACTION_RESET_FIGHT);
}

void EnSkj_ResetFight(EnSkj* self, GlobalContext* globalCtx) {
    if (self->battleExitTimer == 0) {
        EnSkj_SetupResetFight(self);
    } else if (func_80AFEDF8(self, globalCtx) != 0) {
        self->battleExitTimer = 600;
        EnSkj_SetupStand(self);
    }
}

void EnSkj_SetupStand(EnSkj* self) {
    self->needleShootTimer = 60;
    self->unk_2C8 = 0x2000;
    self->unk_2F0 = 0.0f;
    self->unk_2EC = 600.0f;
    EnSkj_ChangeAnim(self, SKJ_ANIM_FIGHTING_STANCE);
    EnSkj_SetupAction(self, SKJ_ACTION_FIGHT);
}

void EnSkj_Fight(EnSkj* self, GlobalContext* globalCtx) {
    Vec3f pos1;
    Vec3f pos2;
    s32 pad[3];
    f32 prevPosX;
    f32 prevPosZ;
    f32 phi_f14;
    s16 yawDistToPlayer;

    if (self->needleShootTimer == 0) {
        EnSkj_SetupWaitToShootNeedle(self);
    } else if (self->battleExitTimer != 0) {
        yawDistToPlayer =
            Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 5, self->unk_2F0, 0);
        self->actor.world.rot.y = self->actor.shape.rot.y;
        Math_ApproachF(&self->unk_2F0, 2000.0f, 1.0f, 200.0f);

        pos1.x = 0.0f;
        pos1.y = 0.0f;
        pos1.z = -120.0f;

        Matrix_RotateY((self->actor.shape.rot.y / 32768.0f) * M_PI, MTXMODE_NEW);
        Matrix_MultVec3f(&pos1, &pos2);
        prevPosX = self->actor.world.pos.x;
        prevPosZ = self->actor.world.pos.z;
        if (1) {}
        self->actor.world.pos.x = self->center.x + pos2.x;
        self->actor.world.pos.z = self->center.z + pos2.z;

        phi_f14 = sqrtf(SQ(self->actor.world.pos.x - prevPosX) + SQ(self->actor.world.pos.z - prevPosZ));
        phi_f14 = CLAMP_MAX(phi_f14, 10.0f);
        phi_f14 /= 10.0f;

        self->skelAnime.playSpeed = (yawDistToPlayer < 0) ? -(1.0f + phi_f14) : (1.0f + phi_f14);

    } else if (func_80AFEDF8(self, globalCtx) != 0) {
        self->backfilpFlag = 1;
        EnSkj_Backflip(self);
    } else {
        EnSkj_SetupResetFight(self);
    }
}

void EnSkj_SetupNeedleRecover(EnSkj* self) {
    Animation_Reverse(&self->skelAnime);
    EnSkj_SetupAction(self, SKJ_ACTION_NEEDLE_RECOVER);
}

void EnSkj_NeedleRecover(EnSkj* self, GlobalContext* globalCtx) {
    if (self->skelAnime.curFrame == 0.0f) {
        EnSkj_SetupStand(self);
    }
}

void EnSkj_SetupSpawnDeathEffect(EnSkj* self) {
    self->backfilpFlag = 1;
    EnSkj_SetupAction(self, SKJ_ACTION_SPAWN_DEATH_EFFECT);
}

void EnSkj_SpawnDeathEffect(EnSkj* self, GlobalContext* globalCtx) {
    Vec3f effectPos;
    Vec3f effectVel;
    Vec3f effectAccel;
    u32 phi_v0;

    phi_v0 = self->alpha - 4;

    if (phi_v0 > 255) {
        phi_v0 = 0;
    }
    self->alpha = phi_v0;
    self->actor.shape.shadowAlpha = phi_v0;

    effectPos.x = Rand_CenteredFloat(30.0f) + self->actor.world.pos.x;
    effectPos.y = Rand_CenteredFloat(30.0f) + self->actor.world.pos.y;
    effectPos.z = Rand_CenteredFloat(30.0f) + self->actor.world.pos.z;

    effectAccel.z = 0.0f;
    effectAccel.y = 0.0f;
    effectAccel.x = 0.0f;

    effectVel.z = 0.0f;
    effectVel.y = 0.0f;
    effectVel.x = 0.0f;

    EffectSsDeadDb_Spawn(globalCtx, &effectPos, &effectVel, &effectAccel, 100, 10, 255, 255, 255, 255, 0, 0, 255, 1, 9,
                         1);
}

void EnSkj_SetupWaitInRange(EnSkj* self) {
    self->textId = 0x10BC;

    EnSkj_ChangeAnim(self, SKJ_ANIM_WAIT);
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WAIT_IN_RANGE);
}

void EnSkj_WaitInRange(EnSkj* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    // When link pulls out the Ocarina center him on the stump
    // Link was probably supposed to be pointed towards skull kid as well
    if (player->stateFlags2 & 0x1000000) {
        player->stateFlags2 |= 0x2000000;
        player->unk_6A8 = &sSmallStumpSkullKid.skullkid->actor;
        player->actor.world.pos.x = sSmallStumpSkullKid.skullkid->actor.world.pos.x;
        player->actor.world.pos.y = sSmallStumpSkullKid.skullkid->actor.world.pos.y;
        player->actor.world.pos.z = sSmallStumpSkullKid.skullkid->actor.world.pos.z;
        EnSkj_TurnPlayer(sSmallStumpSkullKid.skullkid, player);
        func_8010BD88(globalCtx, 0x22);
        EnSkj_SetupWaitForSong(self);
    } else if (D_80B01EA0 != 0) {
        player->actor.world.pos.x = sSmallStumpSkullKid.skullkid->actor.world.pos.x;
        player->actor.world.pos.y = sSmallStumpSkullKid.skullkid->actor.world.pos.y;
        player->actor.world.pos.z = sSmallStumpSkullKid.skullkid->actor.world.pos.z;
        if ((Player_GetMask(globalCtx) == PLAYER_MASK_SKULL) && !(gSaveContext.itemGetInf[3] & 0x200)) {
            func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
            EnSkj_SetupMaskTrade(self);
        } else {
            EnSkj_SetupTalk(self);
        }
    } else if (!EnSkj_RangeCheck(player, sSmallStumpSkullKid.skullkid)) {
        EnSkj_SetupResetFight(self);
    } else {
        player->stateFlags2 |= 0x800000;
        if (gSaveContext.itemGetInf[1] & 0x40) {
            if (gSaveContext.itemGetInf[3] & 0x200) {
                self->textId = Text_GetFaceReaction(globalCtx, 0x15);
                if (self->textId == 0) {
                    self->textId = 0x1020;
                }
            } else if (Player_GetMask(globalCtx) == PLAYER_MASK_NONE) {
                self->textId = 0x10BC;
            } else if (Player_GetMask(globalCtx) == PLAYER_MASK_SKULL) {
                self->textId = 0x101B;
            } else {
                self->textId = Text_GetFaceReaction(globalCtx, 0x15);
            }
            func_8002F2CC(&self->actor, globalCtx, EnSkj_GetItemXzRange(self));
        }
    }
}

void EnSkj_SetupWaitForSong(EnSkj* self) {
    self->unk_2D6 = 0;
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WAIT_FOR_SONG);
}

void EnSkj_WaitForSong(EnSkj* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    // Played a song thats not Saria's song
    if (!(gSaveContext.itemGetInf[1] & 0x40) &&
        ((globalCtx->msgCtx.msgMode == 0xE) || (globalCtx->msgCtx.msgMode == 0xF))) {
        globalCtx->msgCtx.unk_E3EE = 4;
        func_80106CCC(globalCtx);
        player->unk_6A8 = &self->actor;
        func_8002F2CC(&self->actor, globalCtx, EnSkj_GetItemXzRange(self));
        EnSkj_SetupWrongSong(self);
    } else {
        if ((globalCtx->msgCtx.msgMode == 0xD) && (self->unk_2D6 == 0)) {
            self->unk_2D6 = 1;
            EnSkj_ChangeAnim(self, SKJ_ANIM_PLAY_FLUTE);
        } else if ((self->unk_2D6 != 0) && (globalCtx->msgCtx.msgMode == 0x1A)) {
            self->unk_2D6 = 0;
            EnSkj_ChangeAnim(self, SKJ_ANIM_WAIT);
        }
        if (globalCtx->msgCtx.unk_E3EE == 4) {
            globalCtx->msgCtx.unk_E3EE = 0;
            self->unk_2D6 = 0;
            EnSkj_ChangeAnim(self, SKJ_ANIM_WAIT);
            EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WAIT_IN_RANGE);
        } else if (globalCtx->msgCtx.unk_E3EE == 3) {
            if (!(gSaveContext.itemGetInf[1] & 0x40)) {
                // Saria's song has been played for the first titme
                globalCtx->msgCtx.unk_E3EE = 4;
                func_80078884(NA_SE_SY_CORRECT_CHIME);
                player->unk_6A8 = &self->actor;
                func_8002F2CC(&self->actor, globalCtx, EnSkj_GetItemXzRange(self));
                self->textId = 0x10BB;
                EnSkj_SetupAfterSong(self);
            } else {
                globalCtx->msgCtx.unk_E3EE = 5;
            }
        } else if (globalCtx->msgCtx.unk_E3EE == 2) {
            player->stateFlags2 &= ~0x1000000;
            Actor_Kill(&self->actor);
        } else if (globalCtx->msgCtx.unk_E3EE == 1) {
            player->stateFlags2 |= 0x800000;
        } else {
            if (globalCtx->msgCtx.unk_E3EE >= 5) {
                gSaveContext.sunsSongState = 0;
                if (gSaveContext.itemGetInf[1] & 0x40) {
                    globalCtx->msgCtx.unk_E3EE = 4;
                    player->unk_6A8 = &self->actor;
                    func_8002F2CC(&self->actor, globalCtx, EnSkj_GetItemXzRange(self));
                    self->textId = 0x10BD;
                    EnSkj_SetupAfterSong(self);
                } else {
                    globalCtx->msgCtx.unk_E3EE = 4;
                    player->unk_6A8 = &self->actor;
                    func_8002F2CC(&self->actor, globalCtx, EnSkj_GetItemXzRange(self));
                    EnSkj_SetupWrongSong(self);
                }
            }
        }
    }
}

void EnSkj_SetupAfterSong(EnSkj* self) {
    self->unk_2D6 = 0;
    EnSkj_ChangeAnim(self, SKJ_ANIM_WAIT);
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_AFTER_SONG);
}

void EnSkj_AfterSong(EnSkj* self, GlobalContext* globalCtx) {
    if (D_80B01EA0 != 0) {
        EnSkj_SetupTalk(self);
    } else {
        func_8002F2CC(&self->actor, globalCtx, EnSkj_GetItemXzRange(self));
    }
}

void EnSkj_SetupTalk(EnSkj* self) {
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_TALK);
}

void EnSkj_SariaSongTalk(EnSkj* self, GlobalContext* globalCtx) {
    s32 pad;

    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
        if (gSaveContext.itemGetInf[1] & 0x40) {
            EnSkj_SetupWaitInRange(self);
        } else {
            func_80AFFE24(self);
            func_8002F434(&self->actor, globalCtx, GI_HEART_PIECE, EnSkj_GetItemXzRange(self),
                          EnSkj_GetItemYRange(self));
        }
    }
}

void func_80AFFE24(EnSkj* self) {
    EnSkj_SetupAction(self, SKJ_ACTION_UNK14);
}

void func_80AFFE44(EnSkj* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        EnSkj_SetupPostSariasSong(self);
    } else {
        func_8002F434(&self->actor, globalCtx, GI_HEART_PIECE, EnSkj_GetItemXzRange(self), EnSkj_GetItemYRange(self));
    }
}

void EnSkj_SetupPostSariasSong(EnSkj* self) {
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_CHANGE_MODE);
}

void EnSkj_ChangeModeAfterSong(EnSkj* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
        gSaveContext.itemGetInf[1] |= 0x40;
        EnSkj_SetNaviId(self);
        EnSkj_SetupWaitInRange(self);
    }
}

void EnSkj_SetupMaskTrade(EnSkj* self) {
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_START_TRADE);
}

void EnSkj_StartMaskTrade(EnSkj* self, GlobalContext* globalCtx) {
    u8 sp1F = func_8010BDBC(&globalCtx->msgCtx);

    func_8002DF54(globalCtx, &self->actor, 1);
    if ((sp1F == 6) && (func_80106BC8(globalCtx) != 0)) {
        EnSkj_JumpFromStump(self);
    }
}

void EnSkj_JumpFromStump(EnSkj* self) {
    self->actor.velocity.y = 8.0f;
    self->actor.speedXZ = 2.0f;
    EnSkj_ChangeAnim(self, SKJ_ANIM_BACKFLIP);
    Animation_Reverse(&self->skelAnime);
    self->skelAnime.curFrame = self->skelAnime.startFrame;
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WAIT_FOR_LANDING);
}

void EnSkj_WaitForLanding(EnSkj* self, GlobalContext* globalCtx) {
    if (self->actor.velocity.y <= 0.0f) {
        if (self->actor.bgCheckFlags & 2) {
            self->actor.bgCheckFlags &= ~2;
            self->actor.speedXZ = 0.0f;
            EnSkj_SetupWaitForLandAnimFinish(self);
        }
    }
}

void EnSkj_SetupWaitForLandAnimFinish(EnSkj* self) {
    EnSkj_ChangeAnim(self, SKJ_ANIM_LAND);
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WAIT_FOR_LANDING_ANIM);
}

void EnSkj_WaitForLandAnimFinish(EnSkj* self, GlobalContext* globalCtx) {
    s16 lastFrame = Animation_GetLastFrame(&gSkullKidLandAnim);
    if (self->skelAnime.curFrame == lastFrame) {
        EnSkj_SetupWalkToPlayer(self);
    }
}

void EnSkj_SetupWalkToPlayer(EnSkj* self) {
    self->unk_2F0 = 0.0f;
    self->actor.speedXZ = 2.0f;
    EnSkj_ChangeAnim(self, SKJ_ANIM_WALK_TO_PLAYER);
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WALK_TO_PLAYER);
}

void EnSkj_WalkToPlayer(EnSkj* self, GlobalContext* globalCtx) {
    Math_SmoothStepToS(&self->actor.shape.rot.y, self->actor.yawTowardsPlayer, 0xA, self->unk_2F0, 0);
    Math_ApproachF(&self->unk_2F0, 2000.0f, 1.0f, 100.0f);
    self->actor.world.rot.y = self->actor.shape.rot.y;
    if (self->actor.xzDistToPlayer < 120.0f) {
        self->actor.speedXZ = 0.0f;
        EnSkj_SetupAskForMask(self, globalCtx);
    }
}

void EnSkj_SetupAskForMask(EnSkj* self, GlobalContext* globalCtx) {
    func_8010B680(globalCtx, 0x101C, &self->actor);
    EnSkj_ChangeAnim(self, SKJ_ANIM_WAIT);
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_ASK_FOR_MASK);
}

void EnSkj_AskForMask(EnSkj* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4) {
        if (func_80106BC8(globalCtx) != 0) {
            switch (globalCtx->msgCtx.choiceIndex) {
                case 0: // Yes
                    EnSkj_SetupTakeMask(self, globalCtx);
                    break;
                case 1: // No
                    func_8010B720(globalCtx, 0x101D);
                    EnSkj_SetupWaitForMaskTextClear(self);
                    break;
            }
        }
    }
}

void EnSkj_SetupTakeMask(EnSkj* self, GlobalContext* globalCtx) {
    func_8010B720(globalCtx, 0x101E);
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_TAKE_MASK);
}

void EnSkj_TakeMask(EnSkj* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
        Rupees_ChangeBy(10);
        gSaveContext.itemGetInf[3] |= 0x200;
        EnSkj_SetNaviId(self);
        Player_UnsetMask(globalCtx);
        Item_Give(globalCtx, ITEM_SOLD_OUT);
        func_8010B720(globalCtx, 0x101F);
        EnSkj_SetupWaitForMaskTextClear(self);
    }
}

void EnSkj_SetupWaitForMaskTextClear(EnSkj* self) {
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WAIT_MASK_TEXT);
}

void EnSkj_WaitForMaskTextClear(EnSkj* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
        func_8002DF54(globalCtx, &self->actor, 7);
        self->backfilpFlag = 1;
        EnSkj_Backflip(self);
    }
}

void EnSkj_SetupWrongSong(EnSkj* self) {
    self->textId = 0x1041;
    EnSkj_ChangeAnim(self, SKJ_ANIM_WAIT);
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WRONG_SONG);
}

void EnSkj_WrongSong(EnSkj* self, GlobalContext* globalCtx) {
    if (D_80B01EA0 != 0) {
        EnSkj_SetupWaitForTextClear(self);
    } else {
        func_8002F2CC(&self->actor, globalCtx, EnSkj_GetItemXzRange(self));
    }
}

void EnSkj_SetupWaitForTextClear(EnSkj* self) {
    EnSkj_SetupAction(self, SKJ_ACTION_SARIA_SONG_WAIT_FOR_TEXT);
}

void EnSkj_SariasSongWaitForTextClear(EnSkj* self, GlobalContext* globalCtx) {
    u8 state = func_8010BDBC(&globalCtx->msgCtx);
    Player* player = GET_PLAYER(globalCtx);

    if (state == 6 && func_80106BC8(globalCtx)) {
        EnSkj_SetupWaitInRange(self);
        player->stateFlags2 |= 0x800000;
        player->unk_6A8 = (Actor*)sSmallStumpSkullKid.skullkid;
    }
}

void EnSkj_OcarinaGameSetupWaitForPlayer(EnSkj* self) {
    self->actor.flags &= ~1;
    EnSkj_ChangeAnim(self, SKJ_ANIM_WAIT);
    EnSkj_SetupAction(self, SKJ_ACTION_OCARINA_GAME_WAIT_FOR_PLAYER);
}

void EnSkj_OcarinaGameWaitForPlayer(EnSkj* self, GlobalContext* globalCtx) {
    if (self->playerInRange) {
        self->actor.flags |= 1;
        EnSkj_SetupAction(self, SKJ_ACTION_OCARINA_GAME_IDLE);
    }
}

s32 EnSkj_IsLeavingGame(EnSkj* self) {
    s32 paramDecr = self->actor.params - 1;

    if (sOcarinaMinigameSkullKids[paramDecr].unk0 == 2) {
        EnSkj_SetupLeaveOcarinaGame(self);
        return true;
    }
    return false;
}

void EnSkj_SetupIdle(EnSkj* self) {
    EnSkj_ChangeAnim(self, SKJ_ANIM_WAIT);
    EnSkj_SetupAction(self, SKJ_ACTION_OCARINA_GAME_IDLE);
}

void EnSkj_Appear(EnSkj* self) {
    if (self->alpha != 255) {
        self->alpha += 20;

        if (self->alpha > 255) {
            self->alpha = 255;
        }
    }
}

void EnSkj_OcarinaGameIdle(EnSkj* self, GlobalContext* globalCtx) {
    EnSkj_Appear(self);

    if ((EnSkj_IsLeavingGame(self) == false) && (self->minigameState != 0)) {
        EnSkj_SetupPlayOcarinaGame(self);
    }
}

void EnSkj_SetupPlayOcarinaGame(EnSkj* self) {
    EnSkj_ChangeAnim(self, SKJ_ANIM_PLAY_FLUTE);
    EnSkj_SetupAction(self, SKJ_ACTION_OCARINA_GAME_PLAY);
}

void EnSkj_PlayOcarinaGame(EnSkj* self, GlobalContext* globalCtx) {
    EnSkj_Appear(self);

    if (!EnSkj_IsLeavingGame(self) && (self->minigameState == 0)) {
        EnSkj_SetupIdle(self);
    }
}

void EnSkj_SetupLeaveOcarinaGame(EnSkj* self) {
    self->actor.velocity.y = 8.0f;
    self->actor.speedXZ = -8.0f;
    EnSkj_ChangeAnim(self, SKJ_ANIM_BACKFLIP);
    EnSkj_SetupAction(self, SKJ_ACTION_OCARINA_GAME_LEAVE);
}

void EnSkj_LeaveOcarinaGame(EnSkj* self, GlobalContext* globalCtx) {
    s32 paramsDecr = self->actor.params - 1;

    sOcarinaMinigameSkullKids[paramsDecr].unk0 = 0;
    sOcarinaMinigameSkullKids[paramsDecr].skullkid = NULL;
    self->backfilpFlag = 1;
    EnSkj_Backflip(self);
}

void EnSkj_Update(Actor* thisx, GlobalContext* globalCtx) {
    Vec3f dropPos;
    s32 pad;
    EnSkj* self = THIS;

    D_80B01EA0 = func_8002F194(&self->actor, globalCtx);

    self->timer++;

    if (self->multiuseTimer != 0) {
        self->multiuseTimer--;
    }

    if (self->needleShootTimer != 0) {
        self->needleShootTimer--;
    }

    if (self->dodgeResetTimer != 0) {
        self->dodgeResetTimer--;
    }

    if (self->dodgeResetTimer == 0) {
        self->hitsUntilDodge = 3;
    }

    if ((self->backfilpFlag != 0) && (self->alpha == 0)) {
        if (self->action == 9) {
            dropPos.x = self->actor.world.pos.x;
            dropPos.y = self->actor.world.pos.y;
            dropPos.z = self->actor.world.pos.z;

            Item_DropCollectible(globalCtx, &dropPos, ITEM00_RUPEE_ORANGE);
        }
        Actor_Kill(&self->actor);
        return;
    }

    Actor_SetFocus(&self->actor, 30.0f);
    Actor_SetScale(&self->actor, 0.01f);
    self->actionFunc(self, globalCtx);
    self->actor.textId = self->textId;
    EnSkj_CollisionCheck(self, globalCtx);
    Collider_UpdateCylinder(&self->actor, &self->collider);

    if ((self->unk_2D3 != 0) && (D_80B01EA0 == 0)) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);

        if (self->actor.colorFilterTimer == 0) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
        }
    }

    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    SkelAnime_Update(&self->skelAnime);
    Actor_MoveForward(&self->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 20.0f, 20.0f, 20.0f, 7);
}

void EnSkj_SariasSongShortStumpUpdate(Actor* thisx, GlobalContext* globalCtx) {
    EnSkj* self = THIS;

    D_80B01EA0 = func_8002F194(&self->actor, globalCtx);

    if (BREG(0) != 0) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, 255, 0, 0, 255, 4, globalCtx->state.gfxCtx);
    }
}

void EnSkj_TurnPlayer(EnSkj* self, Player* player) {
    Math_SmoothStepToS(&player->actor.shape.rot.y, self->actor.world.rot.y, 5, 2000, 0);
    player->actor.world.rot.y = player->actor.shape.rot.y;
    player->currentYaw = player->actor.shape.rot.y;
}

void EnSkj_SetupWaitForOcarina(EnSkj* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (EnSkj_RangeCheck(player, self)) {
        sOcarinaMinigameSkullKids[SKULL_KID_LEFT].skullkid->playerInRange = true;
        sOcarinaMinigameSkullKids[SKULL_KID_RIGHT].skullkid->playerInRange = true;

        if (player->stateFlags2 & 0x1000000) {
            player->stateFlags2 |= 0x2000000;
            func_800F5BF0(4);
            EnSkj_TurnPlayer(self, player);
            player->unk_6A8 = &self->actor;
            func_8010B680(globalCtx, 0x10BE, &self->actor);
            self->actionFunc = EnSkj_StartOcarinaMinigame;
        } else {
            self->actionFunc = EnSkj_WaitForOcarina;
        }
    }
}

void EnSkj_WaitForOcarina(EnSkj* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (player->stateFlags2 & 0x1000000) {
        player->stateFlags2 |= 0x2000000;
        func_800F5BF0(4);
        EnSkj_TurnPlayer(self, player);
        player->unk_6A8 = &self->actor;
        func_8010B680(globalCtx, 0x10BE, &self->actor);
        self->actionFunc = EnSkj_StartOcarinaMinigame;
    } else if (EnSkj_RangeCheck(player, self)) {
        player->stateFlags2 |= 0x800000;
    }
}

void EnSkj_StartOcarinaMinigame(EnSkj* self, GlobalContext* globalCtx) {
    u8 dialogState = func_8010BDBC(&globalCtx->msgCtx);
    Player* player = GET_PLAYER(globalCtx);

    EnSkj_TurnPlayer(self, player);

    if (dialogState == 2) {
        func_8010BD58(globalCtx, 0x2E); // play song?
        if (sOcarinaMinigameSkullKids[SKULL_KID_LEFT].skullkid != NULL) {
            sOcarinaMinigameSkullKids[SKULL_KID_LEFT].skullkid->minigameState = SKULL_KID_OCARINA_PLAY_NOTES;
        }
        self->songFailTimer = 160;
        self->actionFunc = EnSkj_WaitForPlayback;
    }
}

void EnSkj_WaitForPlayback(EnSkj* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    EnSkj_TurnPlayer(self, player);

    if (globalCtx->msgCtx.unk_E3EE == 3) { // failed the game
        func_80106CCC(globalCtx);
        globalCtx->msgCtx.unk_E3EE = 4;
        player->unk_6A8 = &self->actor;
        func_8002F2CC(&self->actor, globalCtx, 26.0f);
        self->textId = 0x102D;
        self->actionFunc = EnSkj_FailedMiniGame;
    } else if (globalCtx->msgCtx.unk_E3EE == 0xF) { // completed the game
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        func_80106CCC(globalCtx);
        globalCtx->msgCtx.unk_E3EE = 4;
        player->unk_6A8 = &self->actor;
        func_8002F2CC(&self->actor, globalCtx, 26.0f);
        self->textId = 0x10BF;
        self->actionFunc = EnSkj_WonOcarinaMiniGame;
    } else { // playing the game
        switch (globalCtx->msgCtx.msgMode) {
            case 0x2B:
                if (sOcarinaMinigameSkullKids[SKULL_KID_LEFT].skullkid != NULL) {
                    sOcarinaMinigameSkullKids[SKULL_KID_LEFT].skullkid->minigameState = SKULL_KID_OCRAINA_WAIT;
                }
                if (!Audio_IsSfxPlaying(NA_SE_SY_METRONOME)) {
                    if (sOcarinaMinigameSkullKids[SKULL_KID_RIGHT].skullkid != NULL) {
                        sOcarinaMinigameSkullKids[SKULL_KID_RIGHT].skullkid->minigameState =
                            SKULL_KID_OCARINA_PLAY_NOTES;
                    }
                    func_80106AA8(globalCtx);
                }
                break;

            case 0x2D:
                if (sOcarinaMinigameSkullKids[SKULL_KID_RIGHT].skullkid != NULL) {
                    sOcarinaMinigameSkullKids[SKULL_KID_RIGHT].skullkid->minigameState = SKULL_KID_OCRAINA_WAIT;
                }
                if (!Audio_IsSfxPlaying(NA_SE_SY_METRONOME)) {
                    func_80106AA8(globalCtx);
                    self->songFailTimer = 160;
                }
                break;

            case 0x2E:
                if (self->songFailTimer != 0) {
                    self->songFailTimer--;
                } else { // took too long, game failed
                    func_80078884(NA_SE_SY_OCARINA_ERROR);
                    func_80106CCC(globalCtx);
                    globalCtx->msgCtx.unk_E3EE = 4;
                    player->unk_6A8 = &self->actor;
                    func_8002F2CC(&self->actor, globalCtx, 26.0f);
                    self->textId = 0x102D;
                    self->actionFunc = EnSkj_FailedMiniGame;
                }
                break;

            case 0x30:
                if (!Audio_IsSfxPlaying(NA_SE_SY_METRONOME)) {
                    if (sOcarinaMinigameSkullKids[SKULL_KID_LEFT].skullkid != NULL) {
                        sOcarinaMinigameSkullKids[SKULL_KID_LEFT].skullkid->minigameState =
                            SKULL_KID_OCARINA_PLAY_NOTES;
                    }
                    self->songFailTimer = 160;
                    func_800ED858(6); // related instrument sound (flute?)
                    func_800ED93C(0xE, 1);
                    globalCtx->msgCtx.msgMode = 0x2A;
                    globalCtx->msgCtx.unk_E3E7 = 2;
                }
                break;
        }
    }
}

void EnSkj_FailedMiniGame(EnSkj* self, GlobalContext* globalCtx) {
    if (D_80B01EA0) {
        self->actionFunc = EnSkj_WaitForNextRound;
    } else {
        func_8002F2CC(&self->actor, globalCtx, 26.0f);
    }
}

void EnSkj_WaitForNextRound(EnSkj* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 6 && func_80106BC8(globalCtx)) {
        EnSkj_OfferNextRound(self, globalCtx);
    }
}

void EnSkj_OfferNextRound(EnSkj* self, GlobalContext* globalCtx) {
    func_8010B720(globalCtx, 0x102E);
    self->actionFunc = EnSkj_WaitForOfferResponse;
}

void EnSkj_WaitForOfferResponse(EnSkj* self, GlobalContext* globalCtx) {
    Player* player;

    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // yes
                player = GET_PLAYER(globalCtx);
                player->stateFlags3 |= 0x20; // makes player take ocarina out right away after closing box
                self->actionFunc = EnSkj_SetupWaitForOcarina;
                break;
            case 1: // no
                self->actionFunc = EnSkj_CleanupOcarinaGame;
                break;
        }
    }
}

void EnSkj_WonOcarinaMiniGame(EnSkj* self, GlobalContext* globalCtx) {
    if (D_80B01EA0) {
        self->actionFunc = EnSkj_WaitToGiveReward;
    } else {
        func_8002F2CC(&self->actor, globalCtx, 26.0f);
    }
}

void EnSkj_WaitToGiveReward(EnSkj* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx))) {
        func_8002F434(&self->actor, globalCtx, sOcarinaGameRewards[gSaveContext.ocarinaGameReward], 26.0f, 26.0f);
        self->actionFunc = EnSkj_GiveOcarinaGameReward;
    }
}

void EnSkj_GiveOcarinaGameReward(EnSkj* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = EnSkj_FinishOcarinaGameRound;
    } else {
        func_8002F434(&self->actor, globalCtx, sOcarinaGameRewards[gSaveContext.ocarinaGameReward], 26.0f, 26.0f);
    }
}

void EnSkj_FinishOcarinaGameRound(EnSkj* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx))) {
        s32 ocarinaGameReward = gSaveContext.ocarinaGameReward;

        if (gSaveContext.ocarinaGameReward < 3) {
            gSaveContext.ocarinaGameReward++;
        }

        if (ocarinaGameReward == 2) {
            gSaveContext.itemGetInf[1] |= 0x80;
            self->actionFunc = EnSkj_CleanupOcarinaGame;
        } else {
            EnSkj_OfferNextRound(self, globalCtx);
        }
    }
}

void EnSkj_CleanupOcarinaGame(EnSkj* self, GlobalContext* globalCtx) {
    if (sOcarinaMinigameSkullKids[SKULL_KID_LEFT].skullkid != NULL) {
        sOcarinaMinigameSkullKids[SKULL_KID_LEFT].unk0 = 2;
    }

    if (sOcarinaMinigameSkullKids[SKULL_KID_RIGHT].skullkid != NULL) {
        sOcarinaMinigameSkullKids[SKULL_KID_RIGHT].unk0 = 2;
    }

    if ((sOcarinaMinigameSkullKids[SKULL_KID_LEFT].unk0 == 2) &&
        (sOcarinaMinigameSkullKids[SKULL_KID_RIGHT].unk0 == 2)) {
        func_800F5C2C();
        Actor_Kill(&self->actor);
    }
}

void EnSkj_OcarinaMinigameShortStumpUpdate(Actor* thisx, GlobalContext* globalCtx) {
    EnSkj* self = THIS;

    D_80B01EA0 = func_8002F194(&self->actor, globalCtx);
    self->timer++;

    self->actor.focus.pos.x = 1230.0f;
    self->actor.focus.pos.y = -90.0f;
    self->actor.focus.pos.z = 450.0f;

    if (BREG(0) != 0) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, 255, 0, 0, 255, 4, globalCtx->state.gfxCtx);
    }

    self->actionFunc(self, globalCtx);

    self->actor.textId = self->textId;
    self->actor.xzDistToPlayer = 50.0;
}

s32 EnSkj_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    return 0;
}

void EnSkj_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_skj.c", 2417);

    if ((limbIndex == 11) && (gSaveContext.itemGetInf[3] & 0x200)) {
        func_80093D18(globalCtx->state.gfxCtx);
        Matrix_Push();
        Matrix_RotateRPY(-0x4000, 0, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_skj.c", 2430),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, gSKJskullMaskDL);
        Matrix_Pop();
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_skj.c", 2437);
}

Gfx* EnSkj_TranslucentDL(GraphicsContext* gfxCtx, u32 alpha) {
    Gfx* dList;
    Gfx* dListHead;

    //! @bug This only allocates space for 1 command but uses 3
    dList = dListHead = Graph_Alloc(gfxCtx, sizeof(Gfx));
    gDPSetRenderMode(dListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2);
    gDPSetEnvColor(dListHead++, 0, 0, 0, alpha);
    gSPEndDisplayList(dListHead++);

    return dList;
}

Gfx* EnSkj_OpaqueDL(GraphicsContext* gfxCtx, u32 alpha) {
    Gfx* dList;
    Gfx* dListHead;

    //! @bug This only allocates space for 1 command but uses 2
    dList = dListHead = Graph_Alloc(gfxCtx, sizeof(Gfx));
    gDPSetEnvColor(dListHead++, 0, 0, 0, alpha);
    gSPEndDisplayList(dListHead++);

    return dList;
}

void EnSkj_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnSkj* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_skj.c", 2475);

    func_80093D18(globalCtx->state.gfxCtx);

    if (self->alpha < 255) {
        gSPSegment(POLY_OPA_DISP++, 0x0C, EnSkj_TranslucentDL(globalCtx->state.gfxCtx, self->alpha));
    } else {
        gSPSegment(POLY_OPA_DISP++, 0x0C, EnSkj_OpaqueDL(globalCtx->state.gfxCtx, self->alpha));
    }

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnSkj_OverrideLimbDraw, EnSkj_PostLimbDraw, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_skj.c", 2495);
}
