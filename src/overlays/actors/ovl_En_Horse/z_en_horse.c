/**
 * File: z_en_horse.c
 * Overlay: ovl_En_Horse
 * Description: Rideable horses
 */

#include "z_en_horse.h"
#include "overlays/actors/ovl_En_In/z_en_in.h"
#include "objects/object_horse/object_horse.h"
#include "objects/object_hni/object_hni.h"
#include "scenes/overworld/spot09/spot09_scene.h"

#define FLAGS 0x00000010

#define THIS ((EnHorse*)thisx)

typedef void (*EnHorseCsFunc)(EnHorse*, GlobalContext*, CsCmdActorAction*);
typedef void (*EnHorseActionFunc)(EnHorse*, GlobalContext*);

void EnHorse_Init(Actor* thisx, GlobalContext* globalCtx);
void EnHorse_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnHorse_Update(Actor* thisx, GlobalContext* globalCtx);
void EnHorse_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnHorse_InitCutscene(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_InitHorsebackArchery(EnHorse* self);
void EnHorse_InitFleePlayer(EnHorse* self);
void EnHorse_ResetIdleAnimation(EnHorse* self);
void EnHorse_StartIdleRidable(EnHorse* self);
void EnHorse_InitInactive(EnHorse* self);
void EnHorse_InitIngoHorse(EnHorse* self);

void EnHorse_Frozen(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_Inactive(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_Idle(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_FollowPlayer(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_UpdateIngoRace(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_MountedIdle(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_MountedIdleWhinneying(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_MountedTurn(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_MountedWalk(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_MountedTrot(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_MountedGallop(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_MountedRearing(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_Stopping(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_Reverse(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_LowJump(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_HighJump(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_BridgeJump(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_CutsceneUpdate(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_UpdateHorsebackArchery(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_FleePlayer(EnHorse* self, GlobalContext* globalCtx);

static AnimationHeader* sEponaAnimHeaders[] = {
    &gEponaIdleAnim,     &gEponaWhinnyAnim,    &gEponaRefuseAnim,  &gEponaRearingAnim,     &gEponaWalkingAnim,
    &gEponaTrottingAnim, &gEponaGallopingAnim, &gEponaJumpingAnim, &gEponaJumpingHighAnim,
};

static AnimationHeader* sHniAnimHeaders[] = {
    &gHorseIngoIdleAnim,      &gHorseIngoWhinnyAnim,  &gHorseIngoRefuseAnim,
    &gHorseIngoRearingAnim,   &gHorseIngoWalkingAnim, &gHorseIngoTrottingAnim,
    &gHorseIngoGallopingAnim, &gHorseIngoJumpingAnim, &gHorseIngoJumpingHighAnim,
};

static AnimationHeader** sAnimationHeaders[] = { sEponaAnimHeaders, sHniAnimHeaders };

static f32 sPlaybackSpeeds[] = { 0.6666667f, 0.6666667f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.6666667f, 0.6666667f };

static SkeletonHeader* sSkeletonHeaders[] = { &gEponaSkel, &gHorseIngoSkel };

const ActorInit En_Horse_InitVars = {
    ACTOR_EN_HORSE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HORSE,
    sizeof(EnHorse),
    (ActorFunc)EnHorse_Init,
    (ActorFunc)EnHorse_Destroy,
    (ActorFunc)EnHorse_Update,
    (ActorFunc)EnHorse_Draw,
};

static ColliderCylinderInit sCylinderInit1 = {
    {
        COLTYPE_NONE,
        AT_TYPE_PLAYER,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1 | OC2_UNK1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000400, 0x00, 0x04 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 20, 70, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylinderInit2 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1 | OC2_UNK1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_NONE,
        OCELEM_ON,
    },
    { 20, 70, 0, { 0, 0, 0 } },
};

static ColliderJntSphElementInit sJntSphItemsInit[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0x0001F824, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_NO_AT_INFO | BUMP_NO_DAMAGE | BUMP_NO_SWORD_SFX | BUMP_NO_HITMARK,
            OCELEM_ON,
        },
        { 13, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1 | OC2_UNK1,
        COLSHAPE_JNTSPH,
    },
    1,
    sJntSphItemsInit,
};

static CollisionCheckInfoInit D_80A65F38 = { 10, 35, 100, MASS_HEAVY };

typedef struct {
    s16 scene;
    Vec3s pos;
    s16 angle;
} EnHorseSpawnpoint;

static EnHorseSpawnpoint sHorseSpawns[] = {
    // Hyrule Field
    { SCENE_SPOT00, 16, 0, 1341, 0 },
    { SCENE_SPOT00, -1297, 0, 1459, 0 },
    { SCENE_SPOT00, -5416, -300, 1296, 0 },
    { SCENE_SPOT00, -4667, -300, 3620, 0 },
    { SCENE_SPOT00, -3837, 81, 5537, 0 },
    { SCENE_SPOT00, -5093, -226, 6661, 0 },
    { SCENE_SPOT00, -6588, -79, 5053, 0 },
    { SCENE_SPOT00, -7072, -500, 7538, 0 },
    { SCENE_SPOT00, -6139, -500, 8910, 0 },
    { SCENE_SPOT00, -8497, -300, 7802, 0 },
    { SCENE_SPOT00, -5481, -499, 12127, 0 },
    { SCENE_SPOT00, -4808, -700, 13583, 0 },
    { SCENE_SPOT00, -3416, -490, 12092, 0 },
    { SCENE_SPOT00, -2915, 100, 8339, 0 },
    { SCENE_SPOT00, -2277, -500, 13247, 0 },
    { SCENE_SPOT00, -1026, -500, 12101, 0 },
    { SCENE_SPOT00, 1427, -500, 13341, 0 },
    { SCENE_SPOT00, -200, -486, 10205, 0 },
    { SCENE_SPOT00, -1469, 100, 7496, 0 },
    { SCENE_SPOT00, -995, 168, 5652, 0 },
    { SCENE_SPOT00, 1938, 89, 6232, 0 },
    { SCENE_SPOT00, 1387, -105, 9206, 0 },
    { SCENE_SPOT00, 1571, -223, 7701, 0 },
    { SCENE_SPOT00, 3893, -121, 7068, 0 },
    { SCENE_SPOT00, 3179, 373, 5221, 0 },
    { SCENE_SPOT00, 4678, -20, 3869, 0 },
    { SCENE_SPOT00, 3460, 246, 4207, 0 },
    { SCENE_SPOT00, 3686, 128, 2366, 0 },
    { SCENE_SPOT00, 1791, 18, 152, 0 },
    { SCENE_SPOT00, 3667, -16, 1399, 0 },
    { SCENE_SPOT00, 1827, -15, 983, 0 },
    { SCENE_SPOT00, 1574, 399, 4318, 0 },
    { SCENE_SPOT00, 716, 95, 3391, 0 },
    { SCENE_SPOT00, -1189, -41, 4739, 0 },
    { SCENE_SPOT00, -1976, -171, 4172, 0 },
    { SCENE_SPOT00, 1314, 391, 5665, 0 },
    { SCENE_SPOT00, 112, 0, 1959, 0 },
    { SCENE_SPOT00, -3011, -111, 9397, 0 },
    { SCENE_SPOT00, -5674, -270, 8585, 0 },
    { SCENE_SPOT00, -8861, -300, 7836, 0 },
    { SCENE_SPOT00, -6056, -500, 7810, 0 },
    { SCENE_SPOT00, -7306, -500, 5994, 0 },
    { SCENE_SPOT00, -7305, -500, 7605, 0 },
    { SCENE_SPOT00, -7439, -300, 7600, 0 },
    { SCENE_SPOT00, -7464, -300, 6268, 0 },
    { SCENE_SPOT00, -8080, -300, 7553, 0 },
    { SCENE_SPOT00, -8091, -300, 7349, 0 },
    { SCENE_SPOT00, -8785, -300, 7383, 0 },
    { SCENE_SPOT00, -8745, -300, 7508, 0 },
    { SCENE_SPOT00, -8777, -300, 7788, 0 },
    { SCENE_SPOT00, -8048, -299, 7738, 0 },
    { SCENE_SPOT00, -7341, -184, 7730, 0 },
    { SCENE_SPOT00, -6410, -288, 7824, 0 },
    { SCENE_SPOT00, -6326, -290, 8205, 0 },
    { SCENE_SPOT00, -6546, -292, 8400, 0 },
    { SCENE_SPOT00, -7533, -180, 8459, 0 },
    { SCENE_SPOT00, -8024, -295, 7903, 0 },
    { SCENE_SPOT00, -8078, -308, 7994, 0 },
    { SCENE_SPOT00, -9425, -287, 7696, 0 },
    { SCENE_SPOT00, -9322, -292, 7577, 0 },
    { SCENE_SPOT00, -9602, -199, 7160, 0 },
    { SCENE_SPOT00, -9307, -300, 7758, 0 },
    { SCENE_SPOT00, -9230, -300, 7642, 0 },
    { SCENE_SPOT00, -7556, -499, 8695, 0 },
    { SCENE_SPOT00, -6438, -500, 8606, 0 },
    { SCENE_SPOT00, -6078, -500, 8258, 0 },
    { SCENE_SPOT00, -6233, -500, 7613, 0 },
    { SCENE_SPOT00, -5035, -205, 7814, 0 },
    { SCENE_SPOT00, -5971, -500, 8501, 0 },
    { SCENE_SPOT00, -5724, -498, 10123, 0 },
    { SCENE_SPOT00, -5094, -392, 11106, 0 },
    { SCENE_SPOT00, -5105, -393, 11312, 0 },
    { SCENE_SPOT00, -4477, -314, 11132, 0 },
    { SCENE_SPOT00, -3867, -380, 11419, 0 },
    { SCENE_SPOT00, -2952, -500, 11944, 0 },
    { SCENE_SPOT00, -2871, -488, 11743, 0 },
    { SCENE_SPOT00, -3829, -372, 11327, 0 },
    { SCENE_SPOT00, -4439, -293, 10989, 0 },
    { SCENE_SPOT00, -5014, -381, 11086, 0 },
    { SCENE_SPOT00, -5113, -188, 10968, 0 },
    { SCENE_SPOT00, -5269, -188, 11156, 0 },
    { SCENE_SPOT00, -5596, -178, 9972, 0 },
    { SCENE_SPOT00, -5801, -288, 8518, 0 },
    { SCENE_SPOT00, -4910, -178, 7931, 0 },
    { SCENE_SPOT00, -3586, 73, 8140, 0 },
    { SCENE_SPOT00, -4487, -106, 9362, 0 },
    { SCENE_SPOT00, -4339, -112, 6412, 0 },
    { SCENE_SPOT00, -3417, 105, 8194, 0 },
    { SCENE_SPOT00, -4505, -20, 6608, 0 },
    { SCENE_SPOT00, -5038, -199, 6603, 0 },
    { SCENE_SPOT00, -4481, 1, 6448, 0 },
    { SCENE_SPOT00, -5032, -168, 6418, 0 },
    { SCENE_SPOT00, -5256, -700, 14329, 0 },
    { SCENE_SPOT00, -5749, -820, 15380, 0 },
    { SCENE_SPOT00, -3122, -700, 13608, 0 },
    { SCENE_SPOT00, -3758, -525, 13228, 0 },
    { SCENE_SPOT00, -3670, -500, 13123, 0 },
    { SCENE_SPOT00, -2924, -500, 13526, 0 },
    { SCENE_SPOT00, 1389, -115, 9370, 0 },
    { SCENE_SPOT00, 548, -116, 8889, 0 },
    { SCENE_SPOT00, -106, -107, 8530, 0 },
    { SCENE_SPOT00, -1608, 85, 7646, 0 },
    { SCENE_SPOT00, -5300, -700, 13772, 0 },
    { SCENE_SPOT00, -5114, -700, 13400, 0 },
    { SCENE_SPOT00, -4561, -700, 13700, 0 },
    { SCENE_SPOT00, -4762, -700, 14084, 0 },
    { SCENE_SPOT00, -2954, 100, 8216, 0 },
    { SCENE_SPOT00, 1460, -104, 9246, 0 },
    { SCENE_SPOT00, 629, -105, 8791, 0 },
    { SCENE_SPOT00, -10, -90, 8419, 0 },
    { SCENE_SPOT00, -1462, 100, 7504, 0 },
    { SCENE_SPOT00, -3018, -500, 12493, 0 },
    { SCENE_SPOT00, -2994, -311, 10331, 0 },
    { SCENE_SPOT00, -4006, -700, 14152, 0 },
    { SCENE_SPOT00, -4341, -500, 12743, 0 },
    { SCENE_SPOT00, -5879, -497, 6799, 0 },
    { SCENE_SPOT00, 22, -473, 10103, 0 },
    { SCENE_SPOT00, -1389, -192, 8874, 0 },
    { SCENE_SPOT00, -4, 92, 6866, 0 },
    { SCENE_SPOT00, 483, 104, 6637, 0 },
    { SCENE_SPOT00, 1580, 183, 5987, 0 },
    { SCENE_SPOT00, 1548, 308, 5077, 0 },
    { SCENE_SPOT00, 1511, 399, 4267, 0 },
    { SCENE_SPOT00, 1358, 385, 4271, 0 },
    { SCENE_SPOT00, 1379, 395, 5063, 0 },
    { SCENE_SPOT00, 1360, 394, 5870, 0 },
    { SCENE_SPOT00, 813, 283, 6194, 0 },
    { SCENE_SPOT00, -57, 101, 6743, 0 },
    { SCENE_SPOT00, 91, 325, 5143, 0 },
    { SCENE_SPOT00, 1425, -214, 7659, 0 },
    { SCENE_SPOT00, 3487, -19, 880, 0 },
    { SCENE_SPOT00, 2933, 152, 2094, 0 },
    { SCENE_SPOT00, 2888, -145, 6862, 0 },
    { SCENE_SPOT00, 1511, 67, 6471, 0 },
    { SCENE_SPOT00, 4051, -47, 1722, 0 },
    { SCENE_SPOT00, -7335, -500, 8627, 0 },
    { SCENE_SPOT00, -7728, -462, 8498, 0 },
    { SCENE_SPOT00, -7791, -446, 8832, 0 },
    { SCENE_SPOT00, -2915, -435, 11334, 0 },
    { SCENE_SPOT00, 165, -278, 3352, 0 },

    // Lake Hylia
    { SCENE_SPOT06, -2109, -882, 1724, 0 },
    { SCENE_SPOT06, -328, -1238, 2705, 0 },
    { SCENE_SPOT06, -3092, -1033, 3527, 0 },

    // Gerudo Valley
    { SCENE_SPOT09, 2687, -269, 753, 0 },
    { SCENE_SPOT09, 2066, -132, 317, 0 },
    { SCENE_SPOT09, 523, -8, 635, 0 },
    { SCENE_SPOT09, 558, 36, -323, 0 },
    { SCENE_SPOT09, 615, 51, -839, 0 },
    { SCENE_SPOT09, -614, 32, 29, 0 },
    { SCENE_SPOT09, -639, -3, 553, 0 },
    { SCENE_SPOT09, -540, 10, -889, 0 },
    { SCENE_SPOT09, -1666, 58, 378, 0 },
    { SCENE_SPOT09, -3044, 210, -648, 0 },

    // Gerudo's Fortress
    { SCENE_SPOT12, -678, 21, -623, 0 },
    { SCENE_SPOT12, 149, 333, -2499, 0 },
    { SCENE_SPOT12, 499, 581, -547, 0 },
    { SCENE_SPOT12, 3187, 1413, -3775, 0 },
    { SCENE_SPOT12, 3198, 1413, 307, 0 },
    { SCENE_SPOT12, 3380, 1413, -1200, 0 },
    { SCENE_SPOT12, -966, 1, -56, 0 },
    { SCENE_SPOT12, -966, 24, -761, 0 },
    { SCENE_SPOT12, -694, 174, -2820, 0 },

    // Lon Lon Ranch
    { SCENE_SPOT20, 1039, 0, 2051, 0 },
    { SCENE_SPOT20, -1443, 0, 1429, 0 },
    { SCENE_SPOT20, 856, 0, -918, 0 }, // Hardcoded to always load in lon lon
    { SCENE_SPOT20, 882, 0, -2256, 0 },
    { SCENE_SPOT20, -1003, 0, -755, 0 }, // Hardcoded to always load in lon lon
    { SCENE_SPOT20, -2254, 0, -629, 0 },
    { SCENE_SPOT20, 907, 0, -2336, 0 },
};

typedef struct {
    s16 zMin;
    s16 zMax;

    s16 xMin;
    s16 xMax;
    s16 xOffset;

    s16 angle;
    s16 angleRange;

    Vec3s pos;
} BridgeJumpPoint;

static BridgeJumpPoint sBridgeJumps[] = {
    { -195, -40, 225, 120, 360, -0x4000, 0x7D0, -270, -52, -117 },
    { -195, -40, -240, -120, -360, 0x4000, 0x7D0, 270, -52, -117 },
};

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 speed;
    s16 angle;
} RaceWaypoint;

typedef struct {
    s32 numWaypoints;
    RaceWaypoint* waypoints;
} RaceInfo;

static RaceWaypoint sIngoRaceWaypoints[] = {
    { 1056, 1, -1540, 11, 0x2A8D },  { 1593, 1, -985, 10, 0xFC27 },   { 1645, 1, -221, 11, 0xE891 },
    { 985, 1, 403, 10, 0xBB9C },     { -1023, 1, 354, 11, 0xA37D },   { -1679, 1, -213, 10, 0x889C },
    { -1552, 1, -1008, 11, 0x638D }, { -947, -1, -1604, 10, 0x4002 },
};

static RaceInfo sIngoRace = { 8, sIngoRaceWaypoints };
static s32 sAnimSoundFrames[] = { 0, 16 };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 600, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 300, ICHAIN_STOP),
};

static u8 sResetNoInput[] = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0 };

static s32 sIdleAnimIds[] = { 1, 3, 0, 3, 1, 0 };

static s16 sIngoAnimations[] = { 7, 6, 2, 2, 1, 1, 0, 0, 0, 0 };

void EnHorse_CsMoveInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);
void EnHorse_CsJumpInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);
void EnHorse_CsRearingInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);
void EnHorse_WarpMoveInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);
void EnHorse_CsWarpRearingInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);

static EnHorseCsFunc sCutsceneInitFuncs[] = {
    NULL,
    EnHorse_CsMoveInit,
    EnHorse_CsJumpInit,
    EnHorse_CsRearingInit,
    EnHorse_WarpMoveInit,
    EnHorse_CsWarpRearingInit,
};

void EnHorse_CsMoveToPoint(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);
void EnHorse_CsJump(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);
void EnHorse_CsRearing(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);
void EnHorse_CsWarpMoveToPoint(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);
void EnHorse_CsWarpRearing(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action);

static EnHorseCsFunc sCutsceneActionFuncs[] = {
    NULL, EnHorse_CsMoveToPoint, EnHorse_CsJump, EnHorse_CsRearing, EnHorse_CsWarpMoveToPoint, EnHorse_CsWarpRearing,
};

typedef struct {
    s32 csAction;
    s32 csFuncIdx;
} CsActionEntry;

static CsActionEntry sCsActionTable[] = {
    { 36, 1 }, { 37, 2 }, { 38, 3 }, { 64, 4 }, { 65, 5 },
};

static RaceWaypoint sHbaWaypoints[] = {
    { 3600, 1413, -5055, 11, 0x8001 }, { 3360, 1413, -5220, 5, 0xC000 }, { 3100, 1413, -4900, 5, 0x0000 },
    { 3600, 1413, -4100, 11, 0x0000 }, { 3600, 1413, 360, 11, 0x0000 },
};

static RaceInfo sHbaInfo = { 5, sHbaWaypoints };

static EnHorseActionFunc sActionFuncs[] = {
    EnHorse_Frozen,
    EnHorse_Inactive,
    EnHorse_Idle,
    EnHorse_FollowPlayer,
    EnHorse_UpdateIngoRace,
    EnHorse_MountedIdle,
    EnHorse_MountedIdleWhinneying,
    EnHorse_MountedTurn,
    EnHorse_MountedWalk,
    EnHorse_MountedTrot,
    EnHorse_MountedGallop,
    EnHorse_MountedRearing,
    EnHorse_Stopping,
    EnHorse_Reverse,
    EnHorse_LowJump,
    EnHorse_HighJump,
    EnHorse_BridgeJump,
    EnHorse_CutsceneUpdate,
    EnHorse_UpdateHorsebackArchery,
    EnHorse_FleePlayer,
};

s32 EnHorse_BgCheckBridgeJumpPoint(EnHorse* self, GlobalContext* globalCtx) {
    f32 xMin;
    f32 xMax;
    s32 i;

    if (globalCtx->sceneNum != SCENE_SPOT09) {
        return false;
    }
    if (self->actor.speedXZ < 12.8f) {
        return false;
    }
    if ((gSaveContext.eventChkInf[9] & 0xF) == 0xF) {
        return false;
    }

    for (i = 0; i < 2; i++) {
        xMin = sBridgeJumps[i].xMin;
        xMax = (xMin + sBridgeJumps[i].xMax) + sBridgeJumps[i].xOffset;
        if (xMax < xMin) {
            f32 temp = xMin;
            xMin = xMax;
            xMax = temp;
        }
        if (sBridgeJumps[i].zMin < self->actor.world.pos.z && self->actor.world.pos.z < sBridgeJumps[i].zMax) {
            if (xMin < self->actor.world.pos.x && self->actor.world.pos.x < xMax) {
                if (sBridgeJumps[i].angle - sBridgeJumps[i].angleRange < self->actor.world.rot.y &&
                    self->actor.world.rot.y < sBridgeJumps[i].angle + sBridgeJumps[i].angleRange) {
                    return true;
                }
            }
        }
    }
    return false;
}

void EnHorse_StartBridgeJump(EnHorse* self, GlobalContext* globalCtx);

s32 EnHorse_CheckBridgeJumps(EnHorse* self, GlobalContext* globalCtx) {
    f32 xMin;
    f32 xMax;
    s32 i;

    if (self->actor.speedXZ < 12.8f) {
        return false;
    }

    for (i = 0; i != 2; i++) {
        xMin = sBridgeJumps[i].xMin;
        xMax = sBridgeJumps[i].xMax + xMin;

        if (xMax < xMin) {
            f32 temp = xMin;
            xMin = xMax;
            xMax = temp;
        }

        if (sBridgeJumps[i].zMin < self->actor.world.pos.z && self->actor.world.pos.z < sBridgeJumps[i].zMax) {
            if (xMin < self->actor.world.pos.x && self->actor.world.pos.x < xMax) {
                if (sBridgeJumps[i].angle - sBridgeJumps[i].angleRange < self->actor.world.rot.y &&
                    self->actor.world.rot.y < sBridgeJumps[i].angle + sBridgeJumps[i].angleRange) {
                    self->bridgeJumpIdx = i;
                    EnHorse_StartBridgeJump(self, globalCtx);
                    return true;
                }
            }
        }
    }

    return false;
}

void EnHorse_RaceWaypointPos(RaceWaypoint* waypoints, s32 idx, Vec3f* pos) {
    pos->x = waypoints[idx].x;
    pos->y = waypoints[idx].y;
    pos->z = waypoints[idx].z;
}

void EnHorse_RotateToPoint(EnHorse* self, GlobalContext* globalCtx, Vec3f* pos, s16 turnAmount) {
    func_8006DD9C(&self->actor, pos, turnAmount);
}

void EnHorse_UpdateIngoRaceInfo(EnHorse* self, GlobalContext* globalCtx, RaceInfo* raceInfo) {
    Vec3f curWaypointPos;
    Vec3f prevWaypointPos;
    f32 playerDist;
    f32 sp50;
    s16 relPlayerYaw;
    f32 px;
    f32 pz;
    f32 d;
    f32 dist;
    s32 prevWaypoint;

    EnHorse_RaceWaypointPos(raceInfo->waypoints, self->curRaceWaypoint, &curWaypointPos);
    Math3D_RotateXZPlane(&curWaypointPos, raceInfo->waypoints[self->curRaceWaypoint].angle, &px, &pz, &d);
    if (((self->actor.world.pos.x * px) + (pz * self->actor.world.pos.z) + d) > 0.0f) {
        self->curRaceWaypoint++;
        if (self->curRaceWaypoint >= raceInfo->numWaypoints) {
            self->curRaceWaypoint = 0;
        }
    }

    EnHorse_RaceWaypointPos(raceInfo->waypoints, self->curRaceWaypoint, &curWaypointPos);

    prevWaypoint = self->curRaceWaypoint - 1;
    if (prevWaypoint < 0) {
        prevWaypoint = raceInfo->numWaypoints - 1;
    }
    EnHorse_RaceWaypointPos(raceInfo->waypoints, prevWaypoint, &prevWaypointPos);
    Math3D_PointDistToLine2D(self->actor.world.pos.x, self->actor.world.pos.z, prevWaypointPos.x, prevWaypointPos.z,
                             curWaypointPos.x, curWaypointPos.z, &dist);
    EnHorse_RotateToPoint(self, globalCtx, &curWaypointPos, 400);

    if (dist < 90000.0f) {
        playerDist = self->actor.xzDistToPlayer;
        if (playerDist < 130.0f || self->jntSph.elements[0].info.ocElemFlags & 2) {
            if (Math_SinS(self->actor.yawTowardsPlayer - self->actor.world.rot.y) > 0.0f) {
                self->actor.world.rot.y = self->actor.world.rot.y - 280;
            } else {
                self->actor.world.rot.y = self->actor.world.rot.y + 280;
            }
        } else if (playerDist < 300.0f) {
            if (Math_SinS(self->actor.yawTowardsPlayer - self->actor.world.rot.y) > 0.0f) {
                self->actor.world.rot.y = self->actor.world.rot.y + 280;
            } else {
                self->actor.world.rot.y = self->actor.world.rot.y - 280;
            }
        }
        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    sp50 = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(globalCtx)->actor);
    relPlayerYaw = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.world.rot.y;
    if (sp50 <= 200.0f || (fabsf(Math_SinS(relPlayerYaw)) < 0.8f && Math_CosS(relPlayerYaw) > 0.0f)) {
        if (self->actor.speedXZ < self->ingoHorseMaxSpeed) {
            self->actor.speedXZ += 0.47f;
        } else {
            self->actor.speedXZ -= 0.47f;
        }
        self->ingoRaceFlags |= 1;
        return;
    }

    if (self->actor.speedXZ < raceInfo->waypoints[self->curRaceWaypoint].speed) {
        self->actor.speedXZ = self->actor.speedXZ + 0.4f;
    } else {
        self->actor.speedXZ = self->actor.speedXZ - 0.4f;
    }
    self->ingoRaceFlags &= ~0x1;
}

void EnHorse_PlayWalkingSound(EnHorse* self) {
    if (sAnimSoundFrames[self->soundTimer] < self->curFrame) {
        if (self->soundTimer == 0 && (sAnimSoundFrames[1] < self->curFrame)) {
            return;
        }

        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_WALK, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        if (++self->soundTimer > 1) {
            self->soundTimer = 0;
        }
    }
}

void EnHorse_PlayTrottingSound(EnHorse* self) {
    Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
}

void EnHorse_PlayGallopingSound(EnHorse* self) {
    Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
}

f32 EnHorse_SlopeSpeedMultiplier(EnHorse* self, GlobalContext* globalCtx) {
    f32 multiplier = 1.0f;

    if (Math_CosS(self->actor.shape.rot.x) < 0.939262f && Math_SinS(self->actor.shape.rot.x) < 0.0f) {
        multiplier = 0.7f;
    }
    return multiplier;
}

void func_80A5BB90(GlobalContext* globalCtx, Vec3f* vec, Vec3f* arg2, f32* arg3) {
    SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, vec, arg2, arg3);
}

s32 func_80A5BBBC(GlobalContext* globalCtx, EnHorse* self, Vec3f* pos) {
    Vec3f sp24;
    f32 sp20;
    f32 eyeDist;

    func_80A5BB90(globalCtx, pos, &sp24, &sp20);
    if (fabsf(sp20) < 0.008f) {
        return false;
    }
    eyeDist = Math3D_Vec3f_DistXYZ(pos, &globalCtx->view.eye);
    return func_800314D4(globalCtx, &self->actor, &sp24, sp20) || eyeDist < 100.0f;
}

void EnHorse_IdleAnimSounds(EnHorse* self, GlobalContext* globalCtx) {
    if (self->animationIdx == ENHORSE_ANIM_IDLE &&
        ((self->curFrame > 35.0f && self->type == HORSE_EPONA) ||
         (self->curFrame > 28.0f && self->type == HORSE_HNI)) &&
        !(self->stateFlags & ENHORSE_SANDDUST_SOUND)) {
        self->stateFlags |= ENHORSE_SANDDUST_SOUND;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_SANDDUST, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else if (self->animationIdx == ENHORSE_ANIM_REARING && self->curFrame > 25.0f &&
               !(self->stateFlags & ENHORSE_LAND2_SOUND)) {
        self->stateFlags |= ENHORSE_LAND2_SOUND;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND2, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

s32 EnHorse_Spawn(EnHorse* self, GlobalContext* globalCtx) {
    f32 minDist = 1e38f;
    s32 spawn = false;
    f32 dist;
    s32 i;
    Player* player;
    Vec3f spawnPos;

    for (i = 0; i < 169; i++) {
        if (sHorseSpawns[i].scene == globalCtx->sceneNum) {
            player = GET_PLAYER(globalCtx);
            if (globalCtx->sceneNum != SCENE_SPOT20 ||
                //! Same flag checked twice
                (Flags_GetEventChkInf(0x18) && ((gSaveContext.eventInf[0] & 0xF) != 6 || Flags_GetEventChkInf(0x18))) ||
                // always load two spawns inside lon lon
                ((sHorseSpawns[i].pos.x == 856 && sHorseSpawns[i].pos.y == 0 && sHorseSpawns[i].pos.z == -918) ||
                 (sHorseSpawns[i].pos.x == -1003 && sHorseSpawns[i].pos.y == 0 && sHorseSpawns[i].pos.z == -755))) {

                spawnPos.x = sHorseSpawns[i].pos.x;
                spawnPos.y = sHorseSpawns[i].pos.y;
                spawnPos.z = sHorseSpawns[i].pos.z;
                dist = Math3D_Vec3f_DistXYZ(&player->actor.world.pos, &spawnPos);

                if (globalCtx->sceneNum) {}
                if (!(minDist < dist) && !func_80A5BBBC(globalCtx, self, &spawnPos)) {
                    minDist = dist;
                    self->actor.world.pos.x = sHorseSpawns[i].pos.x;
                    self->actor.world.pos.y = sHorseSpawns[i].pos.y;
                    self->actor.world.pos.z = sHorseSpawns[i].pos.z;
                    self->actor.prevPos = self->actor.world.pos;
                    self->actor.world.rot.y = sHorseSpawns[i].angle;
                    self->actor.shape.rot.y = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor);
                    spawn = true;
                    SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, &self->actor.world.pos,
                                                 &self->actor.projectedPos, &self->actor.projectedW);
                }
            }
        }
    }

    return spawn;
}

void EnHorse_ResetCutscene(EnHorse* self, GlobalContext* globalCtx) {
    self->cutsceneAction = -1;
    self->cutsceneFlags = 0;
}

void EnHorse_ResetRace(EnHorse* self, GlobalContext* globalCtx) {
    self->inRace = false;
}

s32 EnHorse_PlayerCanMove(EnHorse* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((player->stateFlags1 & 1) || func_8002DD78(GET_PLAYER(globalCtx)) == 1 || (player->stateFlags1 & 0x100000) ||
        ((self->stateFlags & ENHORSE_FLAG_19) && !self->inRace) || self->action == ENHORSE_ACT_HBA ||
        player->actor.flags & 0x100 || globalCtx->csCtx.state != 0) {
        return false;
    }
    return true;
}

void EnHorse_ResetHorsebackArchery(EnHorse* self, GlobalContext* globalCtx) {
    self->unk_39C = 0;
    self->hbaStarted = 0;
    self->hbaFlags = 0;
}

void EnHorse_ClearDustFlags(u16* dustFlags) {
    *dustFlags = 0;
}

void EnHorse_Init(Actor* thisx, GlobalContext* globalCtx2) {
    EnHorse* self = THIS;
    GlobalContext* globalCtx = globalCtx2;

    AREG(6) = 0;
    Actor_ProcessInitChain(&self->actor, sInitChain);
    EnHorse_ClearDustFlags(&self->dustFlags);
    DREG(53) = 0;
    self->riderPos = self->actor.world.pos;
    self->noInputTimer = 0;
    self->noInputTimerMax = 0;
    self->riderPos.y = self->riderPos.y + 70.0f;

    if (DREG(4) == 0) {
        DREG(4) = 70;
    }

    if (self->actor.params & 0x8000) {
        self->actor.params &= ~0x8000;
        self->type = HORSE_HNI;

        if ((self->bankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_HNI)) < 0) {
            Actor_Kill(&self->actor);
            return;
        }

        do {
        } while (!Object_IsLoaded(&globalCtx->objectCtx, self->bankIndex));

        self->actor.objBankIndex = self->bankIndex;
        Actor_SetObjectDependency(globalCtx, &self->actor);
        self->boostSpeed = 12;
    } else {
        self->type = HORSE_EPONA;
        self->boostSpeed = 14;
    }

    // params was -1
    if (self->actor.params == 0x7FFF) {
        self->actor.params = 1;
    }

    if (globalCtx->sceneNum == SCENE_SOUKO) {
        self->stateFlags = ENHORSE_UNRIDEABLE;
    } else if (globalCtx->sceneNum == SCENE_SPOT12 && self->type == HORSE_HNI) {
        self->stateFlags = ENHORSE_FLAG_18 | ENHORSE_UNRIDEABLE;
    } else {
        if (self->actor.params == 3) {
            self->stateFlags = ENHORSE_FLAG_19 | ENHORSE_CANT_JUMP | ENHORSE_UNRIDEABLE;
        } else if (self->actor.params == 6) {
            self->stateFlags = ENHORSE_FLAG_19 | ENHORSE_CANT_JUMP;
            if (Flags_GetEventChkInf(0x18) || DREG(1) != 0) {
                self->stateFlags &= ~ENHORSE_CANT_JUMP;
                self->stateFlags |= ENHORSE_FLAG_26;
            } else if (gSaveContext.eventInf[0] & 0x40 && self->type == HORSE_HNI) {
                self->stateFlags |= ENHORSE_FLAG_21 | ENHORSE_FLAG_20;
            }
        } else if (self->actor.params == 1) {
            self->stateFlags = ENHORSE_FLAG_7;
        } else {
            self->stateFlags = 0;
        }
    }

    if (globalCtx->sceneNum == SCENE_SPOT20 && (gSaveContext.eventInf[0] & 0xF) == 6 &&
        Flags_GetEventChkInf(0x18) == 0 && !DREG(1)) {
        self->stateFlags |= ENHORSE_FLAG_25;
    }

    Actor_SetScale(&self->actor, 0.01f);
    self->actor.gravity = -3.5f;
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawHorse, 20.0f);
    self->action = ENHORSE_ACT_IDLE;
    self->actor.speedXZ = 0.0f;
    Collider_InitCylinder(globalCtx, &self->cyl1);
    Collider_SetCylinder(globalCtx, &self->cyl1, &self->actor, &sCylinderInit1);
    Collider_InitCylinder(globalCtx, &self->cyl2);
    Collider_SetCylinder(globalCtx, &self->cyl2, &self->actor, &sCylinderInit2);
    Collider_InitJntSph(globalCtx, &self->jntSph);
    Collider_SetJntSph(globalCtx, &self->jntSph, &self->actor, &sJntSphInit, &self->jntSphList);
    CollisionCheck_SetInfo(&self->actor.colChkInfo, DamageTable_Get(0xB), &D_80A65F38);
    self->actor.focus.pos = self->actor.world.pos;
    self->actor.focus.pos.y += 70.0f;
    self->playerControlled = false;

    if ((globalCtx->sceneNum == SCENE_SPOT20) && (gSaveContext.sceneSetupIndex < 4)) {
        if (self->type == HORSE_HNI) {
            if (self->actor.world.rot.z == 0 || gSaveContext.nightFlag) {
                Actor_Kill(&self->actor);
                return;
            }
            if (Flags_GetEventChkInf(0x18)) {
                Actor_Kill(&self->actor);
                return;
            }
            if (self->actor.world.rot.z != 5) {
                Actor_Kill(&self->actor);
                return;
            }
        } else if (!Flags_GetEventChkInf(0x18) && !DREG(1) && gSaveContext.nightFlag) {
            Actor_Kill(&self->actor);
            return;
        }
    } else if (globalCtx->sceneNum == SCENE_MALON_STABLE) {
        if (IS_DAY || Flags_GetEventChkInf(0x18) || DREG(1) != 0 || !LINK_IS_ADULT) {
            Actor_Kill(&self->actor);
            return;
        }
        self->stateFlags |= ENHORSE_UNRIDEABLE;
    }

    func_800A663C(globalCtx, &self->skin, sSkeletonHeaders[self->type],
                  sAnimationHeaders[self->type][ENHORSE_ANIM_IDLE]);
    self->animationIdx = ENHORSE_ANIM_IDLE;
    Animation_PlayOnce(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx]);
    self->numBoosts = 6;
    self->blinkTimer = self->postDrawFunc = self->boostRegenTime = 0;
    EnHorse_ResetCutscene(self, globalCtx);
    EnHorse_ResetRace(self, globalCtx);
    EnHorse_ResetHorsebackArchery(self, globalCtx);

    if (self->actor.params == 2) {
        EnHorse_InitInactive(self);
    } else if (self->actor.params == 3) {
        EnHorse_InitIngoHorse(self);
        self->rider =
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_IN, self->actor.world.pos.x, self->actor.world.pos.y,
                        self->actor.world.pos.z, self->actor.shape.rot.x, self->actor.shape.rot.y, 1, 1);
        if (self->rider == NULL) {
            __assert("self->race.rider != NULL", "../z_en_horse.c", 3077);
        }
        if (!(gSaveContext.eventInf[0] & 0x40)) {
            self->ingoHorseMaxSpeed = 12.07f;
        } else {
            self->ingoHorseMaxSpeed = 12.625f;
        }
    } else if (self->actor.params == 7) {
        EnHorse_InitCutscene(self, globalCtx);
    } else if (self->actor.params == 8) {
        EnHorse_InitHorsebackArchery(self);
        Interface_InitHorsebackArchery(globalCtx);
    } else if (globalCtx->sceneNum == SCENE_SPOT20 && !Flags_GetEventChkInf(0x18) && !DREG(1)) {
        EnHorse_InitFleePlayer(self);
    } else {
        if (globalCtx->sceneNum == SCENE_SOUKO) {
            EnHorse_ResetIdleAnimation(self);
        } else if (globalCtx->sceneNum == SCENE_SPOT12 && self->type == HORSE_HNI) {
            EnHorse_ResetIdleAnimation(self);
        } else {
            EnHorse_StartIdleRidable(self);
        }
    }
    self->actor.home.rot.z = self->actor.world.rot.z = self->actor.shape.rot.z = 0;
}

void EnHorse_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnHorse* self = THIS;

    if (self->stateFlags & ENHORSE_DRAW) {
        func_800F89E8(&self->unk_21C);
    }
    func_800A6888(globalCtx, &self->skin);
    Collider_DestroyCylinder(globalCtx, &self->cyl1);
    Collider_DestroyCylinder(globalCtx, &self->cyl2);
    Collider_DestroyJntSph(globalCtx, &self->jntSph);
}

void EnHorse_RotateToPlayer(EnHorse* self, GlobalContext* globalCtx) {
    EnHorse_RotateToPoint(self, globalCtx, &GET_PLAYER(globalCtx)->actor.world.pos, 400);
    if (self->stateFlags & ENHORSE_OBSTACLE) {
        self->actor.world.rot.y += 800.0f;
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnHorse_Freeze(EnHorse* self) {
    if (self->action != ENHORSE_ACT_CS_UPDATE && self->action != ENHORSE_ACT_HBA) {
        if (sResetNoInput[self->actor.params] != 0 && self->actor.params != 4) {
            self->noInputTimer = 0;
            self->noInputTimerMax = 0;
        }
        self->prevAction = self->action;
        self->action = ENHORSE_ACT_FROZEN;
        self->cyl1.base.ocFlags1 &= ~OC1_ON;
        self->cyl2.base.ocFlags1 &= ~OC1_ON;
        self->jntSph.base.ocFlags1 &= ~OC1_ON;
        self->animationIdx = ENHORSE_ANIM_IDLE;
    }
}

void EnHorse_ChangeIdleAnimation(EnHorse* self, s32 arg1, f32 arg2);
void EnHorse_StartMountedIdleResetAnim(EnHorse* self);
void EnHorse_StartMountedIdle(EnHorse* self);
void EnHorse_StartGalloping(EnHorse* self);

void EnHorse_Frozen(EnHorse* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 0.0f;
    self->noInputTimer--;
    if (self->noInputTimer < 0) {
        self->cyl1.base.ocFlags1 |= OC1_ON;
        self->cyl2.base.ocFlags1 |= OC1_ON;
        self->jntSph.base.ocFlags1 |= OC1_ON;
        if (self->playerControlled == true) {
            self->stateFlags &= ~ENHORSE_FLAG_7;
            if (self->actor.params == 4) {
                EnHorse_StartMountedIdleResetAnim(self);
            } else if (self->actor.params == 9) {
                self->actor.params = 5;
                if (globalCtx->csCtx.state != 0) {
                    EnHorse_StartMountedIdle(self);
                } else {
                    self->actor.speedXZ = 8.0f;
                    EnHorse_StartGalloping(self);
                }
            } else if (self->prevAction == 2) {
                EnHorse_StartMountedIdle(self);
            } else {
                EnHorse_StartMountedIdleResetAnim(self);
            }
            if (self->actor.params != 0) {
                self->actor.params = 0;
                return;
            }
        } else {
            if (self->prevAction == 5) {
                EnHorse_ChangeIdleAnimation(self, 0, 0);
                return;
            }
            if (self->prevAction == 6) {
                EnHorse_ChangeIdleAnimation(self, 0, 0);
                return;
            }
            EnHorse_ChangeIdleAnimation(self, 0, 0);
        }
    }
}

void EnHorse_StickDirection(Vec2f* curStick, f32* stickMag, s16* angle);

void EnHorse_UpdateSpeed(EnHorse* self, GlobalContext* globalCtx, f32 brakeDecel, f32 brakeAngle, f32 minStickMag,
                         f32 decel, f32 baseSpeed, s16 turnSpeed) {
    s16* stickAnglePtr; // probably fake
    f32 stickMag;
    s16 stickAngle;
    f32 temp_f12;
    f32 traction;
    s16 turn;
    if (!EnHorse_PlayerCanMove(self, globalCtx)) {
        if (self->actor.speedXZ > 8) {
            self->actor.speedXZ -= decel;
        } else if (self->actor.speedXZ < 0) {
            self->actor.speedXZ = 0;
        }

        return;
    }

    stickAnglePtr = &stickAngle;

    baseSpeed *= EnHorse_SlopeSpeedMultiplier(self, globalCtx);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (Math_CosS(stickAngle) <= brakeAngle) {
        self->actor.speedXZ -= brakeDecel;
        self->actor.speedXZ = self->actor.speedXZ < 0.0f ? 0.0f : self->actor.speedXZ;
        return;
    }

    if (stickMag < minStickMag) {
        self->stateFlags &= ~ENHORSE_BOOST;
        self->stateFlags &= ~ENHORSE_BOOST_DECEL;
        self->actor.speedXZ -= decel;
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ = 0.0f;
        }

        return;
    }

    if (self->stateFlags & ENHORSE_BOOST) {
        if ((16 - self->boostTimer) > 0) {
            self->actor.speedXZ =
                (EnHorse_SlopeSpeedMultiplier(self, globalCtx) * self->boostSpeed - self->actor.speedXZ) /
                    (16 - self->boostTimer) +
                self->actor.speedXZ;
        } else {
            self->actor.speedXZ = EnHorse_SlopeSpeedMultiplier(self, globalCtx) * self->boostSpeed;
        }

        if ((EnHorse_SlopeSpeedMultiplier(self, globalCtx) * self->boostSpeed) <= self->actor.speedXZ) {
            self->stateFlags &= ~ENHORSE_BOOST;
            self->stateFlags |= ENHORSE_BOOST_DECEL;
        }

    } else if (self->stateFlags & ENHORSE_BOOST_DECEL) {
        if (baseSpeed < self->actor.speedXZ) {
            temp_f12 = self->actor.speedXZ;
            self->actor.speedXZ = temp_f12 - 0.06f;
        } else if (self->actor.speedXZ < baseSpeed) {
            self->actor.speedXZ = baseSpeed;
            self->stateFlags &= ~ENHORSE_BOOST_DECEL;
        }
    } else {
        self->actor.speedXZ +=
            (self->actor.speedXZ <= baseSpeed * (1.0f / 54.0f) * stickMag ? 1.0f : -1.0f) * 50.0f * 0.01f;
        if (baseSpeed < self->actor.speedXZ) {
            self->actor.speedXZ = self->actor.speedXZ - decel;
            if (self->actor.speedXZ < baseSpeed) {
                self->actor.speedXZ = baseSpeed;
            }
        }
    }

    temp_f12 = *stickAnglePtr * (1 / 32236.f);
    traction = 2.2f - (self->actor.speedXZ * (1.0f / self->boostSpeed));
    turn = *stickAnglePtr * temp_f12 * temp_f12 * traction;
    turn = CLAMP(turn, -turnSpeed * traction, turnSpeed * traction);
    self->actor.world.rot.y += turn;
    self->actor.shape.rot.y = self->actor.world.rot.y;
}

void EnHorse_StartMountedIdleResetAnim(EnHorse* self) {
    self->skin.skelAnime.curFrame = 0.0f;
    EnHorse_StartMountedIdle(self);
    self->stateFlags &= ~ENHORSE_SANDDUST_SOUND;
}

void EnHorse_StartMountedIdle(EnHorse* self) {
    f32 curFrame;

    self->action = ENHORSE_ACT_MOUNTED_IDLE;
    self->animationIdx = ENHORSE_ANIM_IDLE;
    if ((self->curFrame > 35.0f && self->type == HORSE_EPONA) || (self->curFrame > 28.0f && self->type == HORSE_HNI)) {
        if (!(self->stateFlags & ENHORSE_SANDDUST_SOUND)) {
            self->stateFlags |= ENHORSE_SANDDUST_SOUND;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_SANDDUST, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
    }
    curFrame = self->skin.skelAnime.curFrame;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, curFrame,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
}

void EnHorse_StartReversingInterruptable(EnHorse* self);
void EnHorse_StartTurning(EnHorse* self);
void EnHorse_StartWalkingFromIdle(EnHorse* self);
void EnHorse_MountedIdleAnim(EnHorse* self);
void EnHorse_StartReversing(EnHorse* self);
void EnHorse_StartWalkingInterruptable(EnHorse* self);
void EnHorse_MountedIdleWhinney(EnHorse* self);
void EnHorse_StartWalking(EnHorse* self);

void EnHorse_MountedIdle(EnHorse* self, GlobalContext* globalCtx) {
    f32 mag;
    s16 angle = 0;

    self->actor.speedXZ = 0;
    EnHorse_StickDirection(&self->curStick, &mag, &angle);
    if (mag > 10.0f && EnHorse_PlayerCanMove(self, globalCtx) == true) {
        if (Math_CosS(angle) <= -0.5f) {
            EnHorse_StartReversingInterruptable(self);
        } else if (Math_CosS(angle) <= 0.7071) {
            EnHorse_StartTurning(self);
        } else {
            EnHorse_StartWalkingFromIdle(self);
        }
    }
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        EnHorse_MountedIdleAnim(self);
    }
}

void EnHorse_MountedIdleAnim(EnHorse* self) {
    self->skin.skelAnime.curFrame = 0.0f;
    EnHorse_MountedIdleWhinney(self);
}

void EnHorse_MountedIdleWhinney(EnHorse* self) {
    f32 curFrame;

    self->action = ENHORSE_ACT_MOUNTED_IDLE_WHINNEYING;
    self->animationIdx = ENHORSE_ANIM_WHINNEY;
    curFrame = self->skin.skelAnime.curFrame;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, curFrame,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
    self->unk_21C = self->unk_228;
    if (self->stateFlags & ENHORSE_DRAW) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_GROAN, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
}

void EnHorse_MountedIdleWhinneying(EnHorse* self, GlobalContext* globalCtx) {
    f32 stickMag;
    s16 stickAngle = 0;

    self->actor.speedXZ = 0;
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (stickMag > 10.0f && EnHorse_PlayerCanMove(self, globalCtx) == true) {
        if (Math_CosS(stickAngle) <= -0.5f) {
            EnHorse_StartReversingInterruptable(self);
        } else if (Math_CosS(stickAngle) <= 0.7071) {
            EnHorse_StartTurning(self);
        } else {
            EnHorse_StartWalkingFromIdle(self);
        }
    }
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        EnHorse_StartMountedIdleResetAnim(self);
    }
}

void EnHorse_StartTurning(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_TURN;
    self->soundTimer = 0;
    self->animationIdx = ENHORSE_ANIM_WALK;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
}

void EnHorse_MountedTurn(EnHorse* self, GlobalContext* globalCtx) {
    f32 stickMag;
    s16 clampedYaw;
    s16 stickAngle;

    self->actor.speedXZ = 0;
    EnHorse_PlayWalkingSound(self);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (stickMag > 10.0f) {
        if (!EnHorse_PlayerCanMove(self, globalCtx)) {
            EnHorse_StartMountedIdleResetAnim(self);
        } else if (Math_CosS(stickAngle) <= -0.5f) {
            EnHorse_StartReversingInterruptable(self);
        } else if (Math_CosS(stickAngle) <= 0.7071) {
            clampedYaw = CLAMP(stickAngle, -800.0f, 800.0f);
            self->actor.world.rot.y = self->actor.world.rot.y + clampedYaw;
            self->actor.shape.rot.y = self->actor.world.rot.y;
        } else {
            EnHorse_StartWalkingInterruptable(self);
        }
    }

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        if (Math_CosS(stickAngle) <= 0.7071) {
            EnHorse_StartTurning(self);
        } else {
            EnHorse_StartMountedIdleResetAnim(self);
        }
    }
}

void EnHorse_StartWalkingFromIdle(EnHorse* self) {
    EnHorse_StartWalkingInterruptable(self);

    if (!(self->stateFlags & ENHORSE_FLAG_8) && !(self->stateFlags & ENHORSE_FLAG_9)) {
        self->stateFlags |= ENHORSE_FLAG_9;
        self->waitTimer = 8;
        return;
    }
    self->waitTimer = 0;
}

void EnHorse_StartWalkingInterruptable(EnHorse* self) {
    self->noInputTimer = 0;
    self->noInputTimerMax = 0;
    EnHorse_StartWalking(self);
}

void EnHorse_StartWalking(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_WALK;
    self->soundTimer = 0;
    self->animationIdx = ENHORSE_ANIM_WALK;
    self->waitTimer = 0;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
}

void EnHorse_MountedWalkingReset(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_WALK;
    self->soundTimer = 0;
    self->animationIdx = ENHORSE_ANIM_WALK;
    self->waitTimer = 0;
    Animation_PlayOnce(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx]);
}

void EnHorse_StartTrotting(EnHorse* self);

void EnHorse_MountedWalk(EnHorse* self, GlobalContext* globalCtx) {
    f32 stickMag;
    s16 stickAngle;

    EnHorse_PlayWalkingSound(self);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (self->noInputTimerMax == 0.0f ||
        (self->noInputTimer > 0.0f && self->noInputTimer < self->noInputTimerMax - 20.0f)) {
        EnHorse_UpdateSpeed(self, globalCtx, 0.3f, -0.5f, 10.0f, 0.06f, 3.0f, 400);
    } else {
        self->actor.speedXZ = 3.0f;
    }

    if (self->actor.speedXZ == 0.0f) {
        self->stateFlags &= ~ENHORSE_FLAG_9;
        EnHorse_StartMountedIdleResetAnim(self);
        self->noInputTimer = 0;
        self->noInputTimerMax = 0;
    } else if (self->actor.speedXZ > 3.0f) {
        self->stateFlags &= ~ENHORSE_FLAG_9;
        EnHorse_StartTrotting(self);
        self->noInputTimer = 0;
        self->noInputTimerMax = 0;
    }

    if (self->noInputTimer > 0.0f) {
        self->noInputTimer--;
        if (self->noInputTimer <= 0.0f) {
            self->noInputTimerMax = 0;
        }
    }

    if (self->waitTimer <= 0) {
        self->stateFlags &= ~ENHORSE_FLAG_9;
        self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.75f;
        if (SkelAnime_Update(&self->skin.skelAnime) || self->actor.speedXZ == 0.0f) {
            if (self->noInputTimer <= 0.0f) {
                if (self->actor.speedXZ > 3.0f) {
                    EnHorse_StartTrotting(self);
                    self->noInputTimer = 0;
                    self->noInputTimerMax = 0;
                } else if ((stickMag < 10.0f) || (Math_CosS(stickAngle) <= -0.5f)) {
                    EnHorse_StartMountedIdleResetAnim(self);
                    self->noInputTimer = 0;
                    self->noInputTimerMax = 0;
                } else {
                    EnHorse_MountedWalkingReset(self);
                }
            }
        }
    } else {
        self->actor.speedXZ = 0.0f;
        self->waitTimer--;
    }
}

void EnHorse_StartTrotting(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_TROT;
    self->animationIdx = ENHORSE_ANIM_TROT;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
}

void EnHorse_MountedTrotReset(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_TROT;
    self->animationIdx = ENHORSE_ANIM_TROT;
    Animation_PlayOnce(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx]);
}

void EnHorse_StartGallopingInterruptable(EnHorse* self);

void EnHorse_MountedTrot(EnHorse* self, GlobalContext* globalCtx) {
    f32 stickMag;
    s16 stickAngle;

    EnHorse_UpdateSpeed(self, globalCtx, 0.3f, -0.5f, 10.0f, 0.06f, 6.0f, 400);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (self->actor.speedXZ < 3.0f) {
        EnHorse_StartWalkingInterruptable(self);
    }

    self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.375f;
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        EnHorse_PlayTrottingSound(self);
        func_800AA000(0.0f, 60, 8, 255);
        if (self->actor.speedXZ >= 6.0f) {
            EnHorse_StartGallopingInterruptable(self);
        } else if (self->actor.speedXZ < 3.0f) {
            EnHorse_StartWalkingInterruptable(self);
        } else {
            EnHorse_MountedTrotReset(self);
        }
    }
}

void EnHorse_StartGallopingInterruptable(EnHorse* self) {
    self->noInputTimerMax = 0;
    self->noInputTimer = 0;
    EnHorse_StartGalloping(self);
}

void EnHorse_StartGalloping(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_GALLOP;
    self->animationIdx = ENHORSE_ANIM_GALLOP;
    self->unk_234 = 0;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
}

void EnHorse_MountedGallopReset(EnHorse* self) {
    self->noInputTimerMax = 0;
    self->noInputTimer = 0;
    self->action = ENHORSE_ACT_MOUNTED_GALLOP;
    self->animationIdx = ENHORSE_ANIM_GALLOP;
    self->unk_234 = 0;
    Animation_PlayOnce(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx]);
}

void EnHorse_JumpLanding(EnHorse* self, GlobalContext* globalCtx) {
    Vec3s* jointTable;
    f32 y;

    self->action = ENHORSE_ACT_MOUNTED_GALLOP;
    self->animationIdx = ENHORSE_ANIM_GALLOP;
    Animation_PlayOnce(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx]);
    jointTable = self->skin.skelAnime.jointTable;
    y = jointTable->y;
    self->riderPos.y += y * 0.01f;
    self->postDrawFunc = NULL;
}

void EnHorse_StartBraking(EnHorse* self, GlobalContext* globalCtx);

void EnHorse_MountedGallop(EnHorse* self, GlobalContext* globalCtx) {
    f32 stickMag;
    s16 stickAngle;

    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);

    if (self->noInputTimer <= 0.0f) {
        EnHorse_UpdateSpeed(self, globalCtx, 0.3f, -0.5f, 10.0f, 0.06f, 8.0f, 0x190);
    } else if (self->noInputTimer > 0.0f) {
        self->noInputTimer -= 1;
        self->actor.speedXZ = 8.0f;
    }
    if (self->actor.speedXZ < 6.0f) {
        EnHorse_StartTrotting(self);
    }

    self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.3f;
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        EnHorse_PlayGallopingSound(self);
        func_800AA000(0, 120, 8, 255);
        if (EnHorse_PlayerCanMove(self, globalCtx) == true) {
            if (stickMag >= 10.0f && Math_CosS(stickAngle) <= -0.5f) {
                EnHorse_StartBraking(self, globalCtx);
            } else if (self->actor.speedXZ < 6.0f) {
                EnHorse_StartTrotting(self);
            } else {
                EnHorse_MountedGallopReset(self);
            }
            return;
        }
        EnHorse_MountedGallopReset(self);
    }
}

void EnHorse_StartRearing(EnHorse* self) {
    self->action = ENHORSE_ACT_MOUNTED_REARING;
    self->animationIdx = ENHORSE_ANIM_REARING;
    self->stateFlags &= ~ENHORSE_LAND2_SOUND;
    self->unk_21C = self->unk_228;
    if (self->stateFlags & ENHORSE_DRAW) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
    func_800AA000(0.0f, 180, 20, 100);
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
}

void EnHorse_MountedRearing(EnHorse* self, GlobalContext* globalCtx) {
    f32 stickMag;
    s16 stickAngle;

    self->actor.speedXZ = 0;
    if (self->curFrame > 25.0f) {
        if (!(self->stateFlags & ENHORSE_LAND2_SOUND)) {
            self->stateFlags |= ENHORSE_LAND2_SOUND;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND2, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
            func_800AA000(0, 180, 20, 100);
        }
    }

    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        if (EnHorse_PlayerCanMove(self, globalCtx) == true) {
            if (self->stateFlags & ENHORSE_FORCE_REVERSING) {
                self->noInputTimer = 100;
                self->noInputTimerMax = 100;
                self->stateFlags &= ~ENHORSE_FORCE_REVERSING;
                EnHorse_StartReversing(self);
            } else if (self->stateFlags & ENHORSE_FORCE_WALKING) {
                self->noInputTimer = 100;
                self->noInputTimerMax = 100;
                self->stateFlags &= ~ENHORSE_FORCE_WALKING;
                EnHorse_StartWalking(self);
            } else if (Math_CosS(stickAngle) <= -0.5f) {
                EnHorse_StartReversingInterruptable(self);
            } else {
                EnHorse_StartMountedIdleResetAnim(self);
            }
            return;
        }
        EnHorse_StartMountedIdleResetAnim(self);
    }
}

void EnHorse_StartBraking(EnHorse* self, GlobalContext* globalCtx) {
    self->action = ENHORSE_ACT_STOPPING;
    self->animationIdx = ENHORSE_ANIM_STOPPING;

    Audio_PlaySoundGeneral(NA_SE_EV_HORSE_SLIP, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.5f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);

    self->stateFlags |= ENHORSE_STOPPING_NEIGH_SOUND;
    self->stateFlags &= ~ENHORSE_BOOST;
}

void EnHorse_Stopping(EnHorse* self, GlobalContext* globalCtx) {
    if (self->actor.speedXZ > 0.0f) {
        self->actor.speedXZ = self->actor.speedXZ - 0.6f;
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ = 0.0f;
        }
    }

    if (self->stateFlags & ENHORSE_STOPPING_NEIGH_SOUND && self->skin.skelAnime.curFrame > 29.0f) {
        self->actor.speedXZ = 0.0f;
        if (Rand_ZeroOne() > 0.5) {
            self->unk_21C = self->unk_228;
            if (self->stateFlags & ENHORSE_DRAW) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            }
            func_800AA000(0.0f, 180, 20, 100);
            self->stateFlags &= ~ENHORSE_STOPPING_NEIGH_SOUND;
        } else {
            EnHorse_StartMountedIdleResetAnim(self);
        }
    }

    if (self->skin.skelAnime.curFrame > 29.0f) {
        self->actor.speedXZ = 0.0f;
    } else if (self->actor.speedXZ > 3.0f && self->stateFlags & ENHORSE_FORCE_REVERSING) {
        self->actor.speedXZ = 3.0f;
    }

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        if (self->stateFlags & ENHORSE_FORCE_REVERSING) {
            self->noInputTimer = 100;
            self->noInputTimerMax = 100;
            EnHorse_StartReversing(self);
            self->stateFlags &= ~ENHORSE_FORCE_REVERSING;
        } else {
            EnHorse_StartMountedIdleResetAnim(self);
        }
    }
}

void EnHorse_StartReversingInterruptable(EnHorse* self) {
    self->noInputTimerMax = 0;
    self->noInputTimer = 0;
    EnHorse_StartReversing(self);
}

void EnHorse_StartReversing(EnHorse* self) {
    self->action = ENHORSE_ACT_REVERSE;
    self->animationIdx = ENHORSE_ANIM_WALK;
    self->soundTimer = 0;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_LOOP, -3.0f);
}

void EnHorse_Reverse(EnHorse* self, GlobalContext* globalCtx) {
    f32 stickMag;
    s16 stickAngle;
    s16 turnAmount;
    Player* player = GET_PLAYER(globalCtx);

    EnHorse_PlayWalkingSound(self);
    EnHorse_StickDirection(&self->curStick, &stickMag, &stickAngle);
    if (EnHorse_PlayerCanMove(self, globalCtx) == true) {
        if (self->noInputTimerMax == 0.0f ||
            (self->noInputTimer > 0.0f && self->noInputTimer < self->noInputTimerMax - 20.0f)) {
            if (stickMag < 10.0f && self->noInputTimer <= 0.0f) {
                EnHorse_StartMountedIdleResetAnim(self);
                self->actor.speedXZ = 0.0f;
                return;
            }
            if (stickMag < 10.0f) {
                stickAngle = -0x7FFF;
            } else if (Math_CosS(stickAngle) > -0.5f) {
                self->noInputTimerMax = 0;
                EnHorse_StartMountedIdleResetAnim(self);
                self->actor.speedXZ = 0.0f;
                return;
            }
        } else if (stickMag < 10.0f) {
            stickAngle = -0x7FFF;
        }
    } else if (player->actor.flags & 0x100) {
        EnHorse_StartMountedIdleResetAnim(self);
        self->actor.speedXZ = 0.0f;
        return;
    } else {
        stickAngle = -0x7FFF;
    }

    self->actor.speedXZ = -2.0f;
    turnAmount = 0x7FFF - stickAngle;
    turnAmount = CLAMP(turnAmount, -1200.0f, 1200.0f);
    self->actor.world.rot.y += turnAmount;
    self->actor.shape.rot.y = self->actor.world.rot.y;

    if (self->noInputTimer > 0.0f) {
        self->noInputTimer--;
        if (self->noInputTimer <= 0.0f) {
            self->noInputTimerMax = 0;
        }
    }
    self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.5f * 1.5f;
    if (SkelAnime_Update(&self->skin.skelAnime) && (f32)self->noInputTimer <= 0.0f &&
        EnHorse_PlayerCanMove(self, globalCtx) == true) {
        if (stickMag > 10.0f && Math_CosS(stickAngle) <= -0.5f) {
            self->noInputTimerMax = 0;
            EnHorse_StartReversingInterruptable(self);
        } else if (stickMag < 10.0f) {
            self->noInputTimerMax = 0;
            EnHorse_StartMountedIdleResetAnim(self);
        } else {
            EnHorse_StartReversing(self);
        }
    }
}

void EnHorse_StartLowJump(EnHorse* self, GlobalContext* globalCtx);

void EnHorse_LowJumpInit(EnHorse* self, GlobalContext* globalCtx) {
    self->skin.skelAnime.curFrame = 0.0f;
    EnHorse_StartLowJump(self, globalCtx);
}

void EnHorse_StartLowJump(EnHorse* self, GlobalContext* globalCtx) {
    f32 curFrame;
    Vec3s* jointTable;
    f32 y;

    self->action = ENHORSE_ACT_LOW_JUMP;
    self->animationIdx = ENHORSE_ANIM_LOW_JUMP;
    curFrame = self->skin.skelAnime.curFrame;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.5f, curFrame,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);

    self->postDrawFunc = NULL;
    self->jumpStartY = self->actor.world.pos.y;

    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0;

    jointTable = self->skin.skelAnime.jointTable;
    y = jointTable->y;
    self->riderPos.y -= y * 0.01f;

    Audio_PlaySoundGeneral(NA_SE_EV_HORSE_JUMP, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    func_800AA000(0.0f, 170, 10, 10);
}

void EnHorse_Stub1(EnHorse* self) {
}

void EnHorse_LowJump(EnHorse* self, GlobalContext* globalCtx) {
    Vec3f pad;
    Vec3s* jointTable;
    f32 curFrame;
    f32 y;

    curFrame = self->skin.skelAnime.curFrame;
    self->stateFlags |= ENHORSE_JUMPING;
    self->actor.speedXZ = 12.0f;
    if (curFrame > 17.0f) {
        self->actor.gravity = -3.5f;
        if (self->actor.velocity.y == 0) {
            self->actor.velocity.y = -6.0f;
        }
        if (self->actor.world.pos.y < self->actor.floorHeight + 90.0f) {
            self->skin.skelAnime.playSpeed = 1.5f;
        } else {
            self->skin.skelAnime.playSpeed = 0;
        }
    } else {
        jointTable = self->skin.skelAnime.jointTable;
        y = jointTable->y;
        self->actor.world.pos.y = self->jumpStartY + y * 0.01f;
    }

    if (SkelAnime_Update(&self->skin.skelAnime) ||
        (curFrame > 17.0f && self->actor.world.pos.y < self->actor.floorHeight - self->actor.velocity.y + 80.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        func_800AA000(0.0f, 255, 10, 80);
        self->stateFlags &= ~ENHORSE_JUMPING;
        self->actor.gravity = -3.5f;
        self->actor.world.pos.y = self->actor.floorHeight;
        func_80028A54(globalCtx, 25.0f, &self->actor.world.pos);
        EnHorse_JumpLanding(self, globalCtx);
    }
}

void EnHorse_StartHighJump(EnHorse* self, GlobalContext* globalCtx);

void EnHorse_HighJumpInit(EnHorse* self, GlobalContext* globalCtx) {
    self->skin.skelAnime.curFrame = 0.0f;
    EnHorse_StartHighJump(self, globalCtx);
}

void EnHorse_StartHighJump(EnHorse* self, GlobalContext* globalCtx) {
    f32 curFrame;
    Vec3s* jointTable;
    f32 y;

    self->action = ENHORSE_ACT_HIGH_JUMP;
    self->animationIdx = ENHORSE_ANIM_HIGH_JUMP;
    curFrame = self->skin.skelAnime.curFrame;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.5f, curFrame,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);

    self->jumpStartY = self->actor.world.pos.y;
    self->postDrawFunc = NULL;

    self->actor.gravity = 0;
    self->actor.velocity.y = 0.0f;

    jointTable = self->skin.skelAnime.jointTable;
    y = jointTable->y;
    self->riderPos.y -= y * 0.01f;

    self->stateFlags |= ENHORSE_CALC_RIDER_POS;
    Audio_PlaySoundGeneral(NA_SE_EV_HORSE_JUMP, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    func_800AA000(0.0f, 170, 10, 10);
}

void EnHorse_Stub2(EnHorse* self) {
}

void EnHorse_HighJump(EnHorse* self, GlobalContext* globalCtx) {
    Vec3f pad;
    Vec3s* jointTable;
    f32 curFrame;
    f32 y;

    curFrame = self->skin.skelAnime.curFrame;
    self->stateFlags |= ENHORSE_JUMPING;
    self->actor.speedXZ = 13.0f;
    if (curFrame > 23.0f) {
        self->actor.gravity = -3.5f;
        if (self->actor.velocity.y == 0) {
            self->actor.velocity.y = -10.5f;
        }

        if (self->actor.world.pos.y < self->actor.floorHeight + 90.0f) {
            self->skin.skelAnime.playSpeed = 1.5f;
        } else {
            self->skin.skelAnime.playSpeed = 0;
        }
    } else {
        jointTable = self->skin.skelAnime.jointTable;
        y = jointTable->y;
        self->actor.world.pos.y = self->jumpStartY + y * 0.01f;
    }

    if (SkelAnime_Update(&self->skin.skelAnime) ||
        (curFrame > 23.0f && self->actor.world.pos.y < self->actor.floorHeight - self->actor.velocity.y + 80.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        func_800AA000(0.0f, 255, 10, 80);
        self->stateFlags &= ~ENHORSE_JUMPING;
        self->actor.gravity = -3.5f;
        self->actor.world.pos.y = self->actor.floorHeight;
        func_80028A54(globalCtx, 25.0f, &self->actor.world.pos);
        EnHorse_JumpLanding(self, globalCtx);
    }
}

void EnHorse_InitInactive(EnHorse* self) {
    self->cyl1.base.ocFlags1 &= ~OC1_ON;
    self->cyl2.base.ocFlags1 &= ~OC1_ON;
    self->jntSph.base.ocFlags1 &= ~OC1_ON;
    self->action = ENHORSE_ACT_INACTIVE;
    self->animationIdx = ENHORSE_ANIM_WALK;
    self->stateFlags |= ENHORSE_INACTIVE;
    self->followTimer = 0;
}

void EnHorse_SetFollowAnimation(EnHorse* self, GlobalContext* globalCtx);

void EnHorse_Inactive(EnHorse* self, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;

    if (DREG(53) != 0 && self->type == HORSE_EPONA) {
        DREG(53) = 0;
        if (EnHorse_Spawn(self, globalCtx) != 0) {
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
            self->stateFlags &= ~ENHORSE_INACTIVE;
            gSaveContext.horseData.scene = globalCtx->sceneNum;

            // Focus the camera on Epona
            Camera_SetParam(globalCtx->cameraPtrs[0], 8, self);
            Camera_ChangeSetting(globalCtx->cameraPtrs[0], 0x38);
            Camera_SetCameraData(globalCtx->cameraPtrs[0], 4, NULL, NULL, 0x51, 0, 0);
        }
    }
    if (!(self->stateFlags & ENHORSE_INACTIVE)) {
        self->followTimer = 0;
        EnHorse_SetFollowAnimation(self, globalCtx);
        self->actor.params = 0;
        self->cyl1.base.ocFlags1 |= OC1_ON;
        self->cyl2.base.ocFlags1 |= OC1_ON;
        self->jntSph.base.ocFlags1 |= OC1_ON;
    }
}

void EnHorse_PlayIdleAnimation(EnHorse* self, s32 anim, f32 morphFrames, f32 startFrame) {
    self->action = ENHORSE_ACT_IDLE;
    self->actor.speedXZ = 0.0f;
    if (anim != ENHORSE_ANIM_IDLE && anim != ENHORSE_ANIM_WHINNEY && anim != ENHORSE_ANIM_REARING) {
        anim = ENHORSE_ANIM_IDLE;
    }
    if (anim != self->animationIdx) {
        self->animationIdx = anim;
        if (self->animationIdx == ENHORSE_ANIM_IDLE) {
            self->stateFlags &= ~ENHORSE_SANDDUST_SOUND;
        } else if (self->animationIdx == ENHORSE_ANIM_WHINNEY) {
            self->unk_21C = self->unk_228;
            if (self->stateFlags & ENHORSE_DRAW) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_GROAN, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            }
        } else if (self->animationIdx == ENHORSE_ANIM_REARING) {
            self->unk_21C = self->unk_228;
            if (self->stateFlags & ENHORSE_DRAW) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            }
            self->stateFlags &= ~ENHORSE_LAND2_SOUND;
        }

        Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, startFrame,
                         Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                         morphFrames);
    }
}

void EnHorse_ChangeIdleAnimation(EnHorse* self, s32 anim, f32 morphFrames) {
    EnHorse_PlayIdleAnimation(self, anim, morphFrames, self->curFrame);
}

void EnHorse_ResetIdleAnimation(EnHorse* self) {
    self->animationIdx = ENHORSE_ANIM_WALK; // self forces anim 0 to play from the beginning
    EnHorse_PlayIdleAnimation(self, self->animationIdx, 0, 0);
}

void EnHorse_StartIdleRidable(EnHorse* self) {
    EnHorse_ResetIdleAnimation(self);
    self->stateFlags &= ~ENHORSE_UNRIDEABLE;
}

void EnHorse_StartMovingAnimation(EnHorse* self, s32 arg1, f32 arg2, f32 arg3);

void EnHorse_Idle(EnHorse* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 0.0f;
    EnHorse_IdleAnimSounds(self, globalCtx);

    if (DREG(53) && self->type == HORSE_EPONA) {
        DREG(53) = 0;
        if (!func_80A5BBBC(globalCtx, self, &self->actor.world.pos)) {
            if (EnHorse_Spawn(self, globalCtx)) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                       &D_801333E8);
                self->followTimer = 0;
                EnHorse_SetFollowAnimation(self, globalCtx);
                Camera_SetParam(globalCtx->cameraPtrs[0], 8, self);
                Camera_ChangeSetting(globalCtx->cameraPtrs[0], 0x38);
                Camera_SetCameraData(globalCtx->cameraPtrs[0], 4, NULL, NULL, 0x51, 0, 0);
            }
        } else {
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
            self->followTimer = 0;
            EnHorse_StartMovingAnimation(self, 6, -3.0f, 0.0f);
        }
    }

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        s32 idleAnimIdx = 0;

        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            if (self->animationIdx == ENHORSE_ANIM_WHINNEY) {
                idleAnimIdx = 1;
            } else if (self->animationIdx == ENHORSE_ANIM_REARING) {
                idleAnimIdx = 2;
            }
        }

        // Play one of the two other idle animations
        EnHorse_PlayIdleAnimation(self, sIdleAnimIds[(Rand_ZeroOne() > 0.5f ? 0 : 1) + idleAnimIdx * 2], 0.0f, 0.0f);
    }
}

void EnHorse_StartMovingAnimation(EnHorse* self, s32 animId, f32 morphFrames, f32 startFrame) {
    self->action = ENHORSE_ACT_FOLLOW_PLAYER;
    self->stateFlags &= ~ENHORSE_TURNING_TO_PLAYER;
    if (animId != ENHORSE_ANIM_TROT && animId != ENHORSE_ANIM_GALLOP && animId != ENHORSE_ANIM_WALK) {
        animId = ENHORSE_ANIM_WALK;
    }
    if (self->animationIdx != animId) {
        self->animationIdx = animId;
        Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, startFrame,
                         Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                         morphFrames);
    } else {
        Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, startFrame,
                         Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                         0.0f);
    }
}

void EnHorse_SetFollowAnimation(EnHorse* self, GlobalContext* globalCtx) {
    s32 animId = ENHORSE_ANIM_WALK;
    f32 distToPlayer;

    distToPlayer = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(globalCtx)->actor);
    if (distToPlayer > 400.0f) {
        animId = ENHORSE_ANIM_GALLOP;
    } else if (!(distToPlayer <= 300.0f)) {
        if (distToPlayer <= 400.0f) {
            animId = ENHORSE_ANIM_TROT;
        }
    }

    if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        if (distToPlayer > 400.0f) {
            animId = ENHORSE_ANIM_GALLOP;
        } else {
            animId = ENHORSE_ANIM_TROT;
        }
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        if (distToPlayer > 400.0f) {
            animId = ENHORSE_ANIM_GALLOP;
        } else if (distToPlayer < 300.0f) {
            animId = ENHORSE_ANIM_WALK;
        } else {
            animId = ENHORSE_ANIM_TROT;
        }
    } else if (self->animationIdx == ENHORSE_ANIM_WALK) {
        if (distToPlayer > 300.0f) {
            animId = ENHORSE_ANIM_TROT;
        } else {
            animId = ENHORSE_ANIM_WALK;
        }
    }
    EnHorse_StartMovingAnimation(self, animId, -3.0f, 0.0f);
}

void EnHorse_FollowPlayer(EnHorse* self, GlobalContext* globalCtx) {
    f32 distToPlayer;
    f32 angleDiff;

    DREG(53) = 0;
    distToPlayer = Actor_WorldDistXZToActor(&self->actor, &GET_PLAYER(globalCtx)->actor);

    // First rotate if the player is behind
    if ((self->playerDir == PLAYER_DIR_BACK_R || self->playerDir == PLAYER_DIR_BACK_L) &&
        (distToPlayer > 300.0f && !(self->stateFlags & ENHORSE_TURNING_TO_PLAYER))) {
        self->animationIdx = ENHORSE_ANIM_REARING;
        self->stateFlags |= ENHORSE_TURNING_TO_PLAYER;
        self->angleToPlayer = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor);
        angleDiff = (f32)self->angleToPlayer - (f32)self->actor.world.rot.y;
        if (angleDiff > 32767.f) {
            angleDiff -= 32767.0f;
        } else if (angleDiff < -32767) {
            angleDiff += 32767;
        }

        self->followPlayerTurnSpeed =
            angleDiff / Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]);
        Animation_PlayOnce(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx]);
        self->skin.skelAnime.playSpeed = 1.0f;
        self->stateFlags &= ~ENHORSE_LAND2_SOUND;
        self->unk_21C = self->unk_228;
    } else if (self->stateFlags & ENHORSE_TURNING_TO_PLAYER) {
        self->actor.world.rot.y = self->actor.world.rot.y + self->followPlayerTurnSpeed;
        self->actor.shape.rot.y = self->actor.world.rot.y;
        if (self->curFrame > 25.0f) {
            if (!(self->stateFlags & ENHORSE_LAND2_SOUND)) {
                self->stateFlags |= ENHORSE_LAND2_SOUND;
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND2, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                       &D_801333E8);
            }
        }
    } else {
        EnHorse_RotateToPlayer(self, globalCtx);
    }

    if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        self->actor.speedXZ = 8;
        self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.3f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        self->actor.speedXZ = 6;
        self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.375f;
    } else if (self->animationIdx == ENHORSE_ANIM_WALK) {
        self->actor.speedXZ = 3;
        EnHorse_PlayWalkingSound(self);
        self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.75f;
    } else {
        self->actor.speedXZ = 0;
        self->skin.skelAnime.playSpeed = 1.0f;
    }

    if (!(self->stateFlags & ENHORSE_TURNING_TO_PLAYER) && ++self->followTimer > 300) {
        EnHorse_StartIdleRidable(self);
        self->unk_21C = self->unk_228;

        if (self->stateFlags & ENHORSE_DRAW) {
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        }
    }

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
            EnHorse_PlayGallopingSound(self);
        } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
            EnHorse_PlayTrottingSound(self);
        }
        self->stateFlags &= ~ENHORSE_TURNING_TO_PLAYER;
        if (distToPlayer < 100.0f) {
            EnHorse_StartIdleRidable(self);
        } else {
            EnHorse_SetFollowAnimation(self, globalCtx);
        }
    }
}

void EnHorse_UpdateIngoHorseAnim(EnHorse* self);

void EnHorse_InitIngoHorse(EnHorse* self) {
    self->curRaceWaypoint = 0;
    self->soundTimer = 0;
    self->actor.speedXZ = 0.0f;
    EnHorse_UpdateIngoHorseAnim(self);
    self->unk_21C = self->unk_228;
    if (self->stateFlags & ENHORSE_DRAW) {
        Audio_PlaySoundGeneral(NA_SE_IT_INGO_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
}

void EnHorse_SetIngoAnimation(s32 idx, f32 curFrame, s32 arg2, s16* animIdxOut, f32* curFrameOut) {
    *animIdxOut = sIngoAnimations[idx];
    *curFrameOut = curFrame;
    if (idx == 3 || idx == 7 || idx == 8 || idx == 4) {
        *curFrameOut = 0.0f;
    }
    if (arg2 == 1) {
        if (idx == 5) {
            *animIdxOut = 4;
            *curFrameOut = curFrame;
        } else if (idx == 6) {
            *animIdxOut = 3;
            *curFrameOut = curFrame;
        }
    }
}

void EnHorse_UpdateIngoHorseAnim(EnHorse* self) {
    s32 animChanged = 0;
    f32 animSpeed;

    self->action = ENHORSE_ACT_INGO_RACE;
    self->stateFlags &= ~ENHORSE_SANDDUST_SOUND;
    if (self->actor.speedXZ == 0.0f) {
        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_IDLE;
    } else if (self->actor.speedXZ <= 3.0f) {
        if (self->animationIdx != ENHORSE_ANIM_WALK) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_WALK;
    } else if (self->actor.speedXZ <= 6.0f) {
        if (self->animationIdx != ENHORSE_ANIM_TROT) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_TROT;
    } else {
        if (self->animationIdx != ENHORSE_ANIM_GALLOP) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_GALLOP;
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        animSpeed = self->actor.speedXZ * 0.5f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        animSpeed = self->actor.speedXZ * 0.25f;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    } else if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        animSpeed = self->actor.speedXZ * 0.2f;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    } else {
        animSpeed = 1.0f;
    }

    if (animChanged == true) {
        Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                         sPlaybackSpeeds[self->animationIdx] * animSpeed * 1.5f, 0,
                         Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3);
    } else {
        Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                         sPlaybackSpeeds[self->animationIdx] * animSpeed * 1.5f, 0,
                         Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, 0);
    }
}

void EnHorse_UpdateIngoRace(EnHorse* self, GlobalContext* globalCtx) {
    f32 playSpeed;

    if (self->animationIdx == ENHORSE_ANIM_IDLE || self->animationIdx == ENHORSE_ANIM_WHINNEY) {
        EnHorse_IdleAnimSounds(self, globalCtx);
    } else if (self->animationIdx == ENHORSE_ANIM_WALK) {
        EnHorse_PlayWalkingSound(self);
    }

    EnHorse_UpdateIngoRaceInfo(self, globalCtx, &sIngoRace);
    if (!self->inRace) {
        self->actor.speedXZ = 0.0f;
        self->rider->speedXZ = 0.0f;
        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            EnHorse_UpdateIngoHorseAnim(self);
        }
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        playSpeed = self->actor.speedXZ * 0.5f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        playSpeed = self->actor.speedXZ * 0.25f;
    } else if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        playSpeed = self->actor.speedXZ * 0.2f;
    } else {
        playSpeed = 1.0f;
    }
    self->skin.skelAnime.playSpeed = playSpeed;
    if (SkelAnime_Update(&self->skin.skelAnime) ||
        (self->animationIdx == ENHORSE_ANIM_IDLE && self->actor.speedXZ != 0.0f)) {
        EnHorse_UpdateIngoHorseAnim(self);
    }

    if (self->stateFlags & ENHORSE_INGO_WON) {
        ((EnIn*)self->rider)->animationIdx = 7;
        ((EnIn*)self->rider)->unk_1E0 = 0;
        return;
    }

    EnHorse_SetIngoAnimation(self->animationIdx, self->skin.skelAnime.curFrame, self->ingoRaceFlags & 1,
                             &((EnIn*)self->rider)->animationIdx, &((EnIn*)self->rider)->unk_1E0);
}

void EnHorse_CsMoveInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    self->animationIdx = ENHORSE_ANIM_GALLOP;
    self->cutsceneAction = 1;
    Animation_PlayOnceSetSpeed(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                               self->actor.speedXZ * 0.3f);
}

void EnHorse_CsPlayHighJumpAnim(EnHorse* self, GlobalContext* globalCtx);

void EnHorse_CsMoveToPoint(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    Vec3f endPos;
    f32 speed = 8.0f;

    endPos.x = action->endPos.x;
    endPos.y = action->endPos.y;
    endPos.z = action->endPos.z;
    if (Math3D_Vec3f_DistXYZ(&endPos, &self->actor.world.pos) > speed) {
        EnHorse_RotateToPoint(self, globalCtx, &endPos, 400);
        self->actor.speedXZ = speed;
        self->skin.skelAnime.playSpeed = speed * 0.3f;
    } else {
        self->actor.world.pos = endPos;
        self->actor.speedXZ = 0.0f;
    }

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        EnHorse_PlayGallopingSound(self);
        func_800AA000(0.0f, 120, 8, 255);
        Animation_PlayOnceSetSpeed(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                                   self->actor.speedXZ * 0.3f);
    }
}

void EnHorse_CsSetAnimHighJump(EnHorse* self, GlobalContext* globalCtx) {
    self->skin.skelAnime.curFrame = 0.0f;
    EnHorse_CsPlayHighJumpAnim(self, globalCtx);
}

void EnHorse_CsPlayHighJumpAnim(EnHorse* self, GlobalContext* globalCtx) {
    f32 curFrame;
    f32 y;
    Vec3s* jointTable;

    self->animationIdx = ENHORSE_ANIM_HIGH_JUMP;
    curFrame = self->skin.skelAnime.curFrame;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.5f, curFrame,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
    self->postDrawFunc = NULL;
    self->jumpStartY = self->actor.world.pos.y;
    self->actor.gravity = 0.0f;
    self->actor.velocity.y = 0;

    jointTable = self->skin.skelAnime.jointTable;
    y = jointTable->y;
    self->riderPos.y -= y * 0.01f;

    self->stateFlags |= ENHORSE_CALC_RIDER_POS;
    Audio_PlaySoundGeneral(NA_SE_EV_HORSE_JUMP, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    func_800AA000(0.0f, 170, 10, 10);
}

void EnHorse_CsJumpInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    EnHorse_CsSetAnimHighJump(self, globalCtx);
    self->cutsceneAction = 2;
    self->cutsceneFlags &= ~1;
}

void EnHorse_CsJump(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    f32 temp_f2;

    if (self->cutsceneFlags & 1) {
        EnHorse_CsMoveToPoint(self, globalCtx, action);
        return;
    }
    temp_f2 = self->skin.skelAnime.curFrame;
    self->stateFlags |= ENHORSE_JUMPING;
    self->actor.speedXZ = 13.0f;
    if (temp_f2 > 19.0f) {
        self->actor.gravity = -3.5f;
        if (self->actor.velocity.y == 0.0f) {
            self->actor.velocity.y = -10.5f;
        }
        if (self->actor.world.pos.y < (self->actor.floorHeight + 90.0f)) {
            self->skin.skelAnime.playSpeed = 1.5f;
        } else {
            self->skin.skelAnime.playSpeed = 0.0f;
        }
    } else {
        Vec3s* jointTable;
        f32 y;

        jointTable = self->skin.skelAnime.jointTable;
        y = jointTable->y;
        self->actor.world.pos.y = self->jumpStartY + y * 0.01f;
    }
    if (SkelAnime_Update(&self->skin.skelAnime) ||
        (temp_f2 > 19.0f && self->actor.world.pos.y < (self->actor.floorHeight - self->actor.velocity.y) + 80.0f)) {
        Vec3s* jointTable;
        f32 y;

        self->cutsceneFlags |= 1;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        func_800AA000(0.0f, 255, 10, 80);
        self->stateFlags &= ~ENHORSE_JUMPING;
        self->actor.gravity = -3.5f;
        self->actor.velocity.y = 0;
        self->actor.world.pos.y = self->actor.floorHeight;
        func_80028A54(globalCtx, 25.0f, &self->actor.world.pos);
        self->animationIdx = ENHORSE_ANIM_GALLOP;
        Animation_PlayOnceSetSpeed(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                                   sPlaybackSpeeds[6]);
        jointTable = self->skin.skelAnime.jointTable;
        y = jointTable->y;
        self->riderPos.y += y * 0.01f;
        self->postDrawFunc = NULL;
    }
}

void EnHorse_CsRearingInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    self->animationIdx = ENHORSE_ANIM_REARING;
    self->cutsceneAction = 3;
    self->cutsceneFlags &= ~4;
    self->stateFlags &= ~ENHORSE_LAND2_SOUND;
    self->unk_21C = self->unk_228;
    if (self->stateFlags & ENHORSE_DRAW) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
}

void EnHorse_CsRearing(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    self->actor.speedXZ = 0.0f;
    if (self->curFrame > 25.0f) {
        if (!(self->stateFlags & ENHORSE_LAND2_SOUND)) {
            self->stateFlags |= ENHORSE_LAND2_SOUND;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND2, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
    }
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        self->animationIdx = ENHORSE_ANIM_IDLE;
        if (!(self->cutsceneFlags & 4)) {
            self->cutsceneFlags |= 4;
            Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                             Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                             -3.0f);
        } else {
            Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                             Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), 0, 0.0f);
        }
    }
}

void EnHorse_WarpMoveInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    self->actor.world.pos.x = action->startPos.x;
    self->actor.world.pos.y = action->startPos.y;
    self->actor.world.pos.z = action->startPos.z;
    self->actor.prevPos = self->actor.world.pos;
    self->actor.world.rot.y = action->urot.y;
    self->actor.shape.rot = self->actor.world.rot;
    self->animationIdx = ENHORSE_ANIM_GALLOP;
    self->cutsceneAction = 4;
    Animation_PlayOnceSetSpeed(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                               self->actor.speedXZ * 0.3f);
}

void EnHorse_CsWarpMoveToPoint(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    Vec3f endPos;
    f32 speed = 8.0f;

    endPos.x = action->endPos.x;
    endPos.y = action->endPos.y;
    endPos.z = action->endPos.z;
    if (Math3D_Vec3f_DistXYZ(&endPos, &self->actor.world.pos) > speed) {
        EnHorse_RotateToPoint(self, globalCtx, &endPos, 400);
        self->actor.speedXZ = speed;
        self->skin.skelAnime.playSpeed = speed * 0.3f;
    } else {
        self->actor.world.pos = endPos;
        self->actor.speedXZ = 0.0f;
    }

    if (SkelAnime_Update(&self->skin.skelAnime)) {
        EnHorse_PlayGallopingSound(self);
        func_800AA000(0.0f, 120, 8, 255);
        Animation_PlayOnceSetSpeed(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                                   self->actor.speedXZ * 0.3f);
    }
}

void EnHorse_CsWarpRearingInit(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    self->actor.world.pos.x = action->startPos.x;
    self->actor.world.pos.y = action->startPos.y;
    self->actor.world.pos.z = action->startPos.z;
    self->actor.prevPos = self->actor.world.pos;
    self->actor.world.rot.y = action->urot.y;
    self->actor.shape.rot = self->actor.world.rot;
    self->animationIdx = ENHORSE_ANIM_REARING;
    self->cutsceneAction = 5;
    self->cutsceneFlags &= ~4;
    self->stateFlags &= ~ENHORSE_LAND2_SOUND;
    self->unk_21C = self->unk_228;
    if (self->stateFlags & ENHORSE_DRAW) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
}

void EnHorse_CsWarpRearing(EnHorse* self, GlobalContext* globalCtx, CsCmdActorAction* action) {
    self->actor.speedXZ = 0.0f;
    if (self->curFrame > 25.0f) {
        if (!(self->stateFlags & ENHORSE_LAND2_SOUND)) {
            self->stateFlags |= ENHORSE_LAND2_SOUND;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND2, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
    }
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        self->animationIdx = ENHORSE_ANIM_IDLE;
        if (!(self->cutsceneFlags & 4)) {
            self->cutsceneFlags |= 4;
            Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                             Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                             -3.0f);
        } else {
            Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                             Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), 0, 0.0f);
        }
    }
}

void EnHorse_InitCutscene(EnHorse* self, GlobalContext* globalCtx) {
    self->playerControlled = false;
    self->action = ENHORSE_ACT_CS_UPDATE;
    self->cutsceneAction = 0;
    self->actor.speedXZ = 0.0f;
}

s32 EnHorse_GetCutsceneFunctionIndex(s32 csAction) {
    s32 numActions = ARRAY_COUNT(sCsActionTable); // prevents unrolling
    s32 i;

    for (i = 0; i < numActions; i++) {
        if (csAction == sCsActionTable[i].csAction) {
            return sCsActionTable[i].csFuncIdx;
        }
        if (csAction < sCsActionTable[i].csAction) {
            return 0;
        }
    }
    return 0;
}

void EnHorse_CutsceneUpdate(EnHorse* self, GlobalContext* globalCtx) {
    s32 csFunctionIdx;
    CsCmdActorAction* linkCsAction = globalCtx->csCtx.linkAction;

    if (globalCtx->csCtx.state == 3) {
        self->playerControlled = 1;
        self->actor.params = 10;
        self->action = ENHORSE_ACT_IDLE;
        EnHorse_Freeze(self);
        return;
    }
    if (linkCsAction != 0) {
        csFunctionIdx = EnHorse_GetCutsceneFunctionIndex(linkCsAction->action);
        if (csFunctionIdx != 0) {
            if (self->cutsceneAction != csFunctionIdx) {
                if (self->cutsceneAction == 0) {
                    self->actor.world.pos.x = linkCsAction->startPos.x;
                    self->actor.world.pos.y = linkCsAction->startPos.y;
                    self->actor.world.pos.z = linkCsAction->startPos.z;
                    self->actor.world.rot.y = linkCsAction->urot.y;
                    self->actor.shape.rot = self->actor.world.rot;
                    self->actor.prevPos = self->actor.world.pos;
                }
                self->cutsceneAction = csFunctionIdx;
                sCutsceneInitFuncs[self->cutsceneAction](self, globalCtx, linkCsAction);
            }
            sCutsceneActionFuncs[self->cutsceneAction](self, globalCtx, linkCsAction);
        }
    }
}

s32 EnHorse_UpdateHbaRaceInfo(EnHorse* self, GlobalContext* globalCtx, RaceInfo* raceInfo) {
    Vec3f pos;
    f32 px;
    f32 pz;
    f32 d;

    EnHorse_RaceWaypointPos(raceInfo->waypoints, self->curRaceWaypoint, &pos);
    Math3D_RotateXZPlane(&pos, raceInfo->waypoints[self->curRaceWaypoint].angle, &px, &pz, &d);

    if (self->curRaceWaypoint >= raceInfo->numWaypoints - 1 &&
        Math3D_Vec3f_DistXYZ(&pos, &self->actor.world.pos) < DREG(8)) {
        self->hbaFlags |= 2;
    }

    if (((self->actor.world.pos.x * px) + (pz * self->actor.world.pos.z) + d) > 0.0f) {
        self->curRaceWaypoint++;
        if (self->curRaceWaypoint >= raceInfo->numWaypoints) {
            self->hbaFlags |= 1;
            return 1;
        }
    }

    if (!(self->hbaFlags & 1)) {
        EnHorse_RotateToPoint(self, globalCtx, &pos, 800);
    }

    self->actor.shape.rot.y = self->actor.world.rot.y;
    if (self->actor.speedXZ < raceInfo->waypoints[self->curRaceWaypoint].speed && !(self->hbaFlags & 1)) {
        self->actor.speedXZ += 0.4f;
    } else {
        self->actor.speedXZ -= 0.4f;
        if (self->actor.speedXZ < 0.0f) {
            self->actor.speedXZ = 0.0f;
        }
    }
    return 0;
}

void EnHorse_UpdateHbaAnim(EnHorse* self);

void EnHorse_InitHorsebackArchery(EnHorse* self) {
    self->hbaStarted = 0;
    self->soundTimer = 0;
    self->curRaceWaypoint = 0;
    self->hbaTimer = 0;
    self->actor.speedXZ = 0.0f;
    EnHorse_UpdateHbaAnim(self);
}

void EnHorse_UpdateHbaAnim(EnHorse* self) {
    s32 animChanged = 0;
    f32 animSpeed;

    self->action = ENHORSE_ACT_HBA;
    if (self->actor.speedXZ == 0.0f) {
        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_IDLE;
    } else if (self->actor.speedXZ <= 3.0f) {
        if (self->animationIdx != ENHORSE_ANIM_WALK) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_WALK;
    } else if (self->actor.speedXZ <= 6.0f) {
        if (self->animationIdx != ENHORSE_ANIM_TROT) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_TROT;
    } else {
        if (self->animationIdx != ENHORSE_ANIM_GALLOP) {
            animChanged = true;
        }
        self->animationIdx = ENHORSE_ANIM_GALLOP;
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        animSpeed = self->actor.speedXZ * 0.5f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        animSpeed = self->actor.speedXZ * 0.25f;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        func_800AA000(0.0f, 60, 8, 255);
    } else if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        animSpeed = self->actor.speedXZ * 0.2f;
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_RUN, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        func_800AA000(0.0f, 120, 8, 255);
    } else {
        animSpeed = 1.0f;
    }

    if (animChanged == true) {
        Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                         sPlaybackSpeeds[self->animationIdx] * animSpeed * 1.5f, 0,
                         Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                         -3.0f);
    } else {
        Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx],
                         sPlaybackSpeeds[self->animationIdx] * animSpeed * 1.5f, 0,
                         Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, 0);
    }
}

void EnHorse_UpdateHorsebackArchery(EnHorse* self, GlobalContext* globalCtx) {
    f32 playSpeed;
    s32 sp20;

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        EnHorse_PlayWalkingSound(self);
    }
    if (globalCtx->interfaceCtx.hbaAmmo == 0) {
        self->hbaTimer++;
    }

    sp20 = func_800F5A58(65);
    EnHorse_UpdateHbaRaceInfo(self, globalCtx, &sHbaInfo);
    if (self->hbaFlags & 1 || self->hbaTimer >= 46) {
        if (sp20 != 1 && gSaveContext.minigameState != 3) {
            gSaveContext.cutsceneIndex = 0;
            globalCtx->nextEntranceIndex = 0x3B0;
            globalCtx->sceneLoadFlag = 0x14;
            globalCtx->fadeTransition = 0x20;
        }
    }

    if (globalCtx->interfaceCtx.hbaAmmo != 0) {
        if (!(self->hbaFlags & 2)) {
            if (gSaveContext.infTable[25] & 1) {
                if ((s32)gSaveContext.minigameScore >= 1500) {
                    self->hbaFlags |= 4;
                }
            } else {
                if ((s32)gSaveContext.minigameScore >= 1000) {
                    self->hbaFlags |= 4;
                }
            }
        }
    }

    if ((globalCtx->interfaceCtx.hbaAmmo == 0) || (self->hbaFlags & 2)) {
        if (self->hbaFlags & 4) {
            self->hbaFlags &= ~4;
            Audio_QueueSeqCmd(65);
        }
    }

    if (!self->hbaStarted) {
        self->actor.speedXZ = 0.0f;
        if (self->animationIdx != ENHORSE_ANIM_IDLE) {
            EnHorse_UpdateHbaAnim(self);
        }
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        playSpeed = self->actor.speedXZ * 0.5f;
    } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
        playSpeed = self->actor.speedXZ * 0.25f;
    } else if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
        playSpeed = self->actor.speedXZ * 0.2f;
    } else {
        playSpeed = 1.0f;
    }

    self->skin.skelAnime.playSpeed = playSpeed;
    if (SkelAnime_Update(&self->skin.skelAnime) ||
        (self->animationIdx == ENHORSE_ANIM_IDLE && self->actor.speedXZ != 0.0f)) {
        EnHorse_UpdateHbaAnim(self);
    }
}

void EnHorse_InitFleePlayer(EnHorse* self) {
    self->action = ENHORSE_ACT_FLEE_PLAYER;
    self->stateFlags |= ENHORSE_UNRIDEABLE;
    self->actor.speedXZ = 0.0f;
}

void EnHorse_FleePlayer(EnHorse* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    f32 distToHome;
    f32 playerDistToHome;
    f32 distToPlayer;
    s32 nextAnim = self->animationIdx;
    s32 animFinished;
    s16 yaw;

    if (DREG(53) || self->type == HORSE_HNI) {
        EnHorse_StartIdleRidable(self);
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }

    distToHome = Math3D_Vec3f_DistXYZ(&self->actor.home.pos, &self->actor.world.pos);
    playerDistToHome = Math3D_Vec3f_DistXYZ(&player->actor.world.pos, &self->actor.home.pos);
    distToPlayer = Math3D_Vec3f_DistXYZ(&player->actor.world.pos, &self->actor.world.pos);

    // Run home
    if (playerDistToHome > 300.0f) {
        if (distToHome > 150.0f) {
            self->actor.speedXZ += 0.4f;
            if (self->actor.speedXZ > 8.0f) {
                self->actor.speedXZ = 8.0f;
            }
        } else {
            self->actor.speedXZ -= 0.47f;
            if (self->actor.speedXZ < 0.0f) {
                self->actor.speedXZ = 0.0f;
            }
        }
    } else {
        // Run away from Link
        if (distToPlayer < 300.0f) {
            self->actor.speedXZ += 0.4f;
            if (self->actor.speedXZ > 8.0f) {
                self->actor.speedXZ = 8.0f;
            }
        } else {
            self->actor.speedXZ -= 0.47f;
            if (self->actor.speedXZ < 0.0f) {
                self->actor.speedXZ = 0.0f;
            }
        }
    }

    if (self->actor.speedXZ >= 6.0f) { // hoof it
        self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.3f;
        nextAnim = ENHORSE_ANIM_GALLOP;
    } else if (self->actor.speedXZ >= 3.0f) { // trot
        self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.375f;
        nextAnim = ENHORSE_ANIM_TROT;
    } else if (self->actor.speedXZ > 0.1f) { // walk
        self->skin.skelAnime.playSpeed = self->actor.speedXZ * 0.75f;
        nextAnim = ENHORSE_ANIM_WALK;
        EnHorse_PlayWalkingSound(self);
    } else { // idle
        nextAnim = Rand_ZeroOne() > 0.5f ? 1 : 0;
        EnHorse_IdleAnimSounds(self, globalCtx);
        self->skin.skelAnime.playSpeed = 1.0f;
    }

    // Turn away from Link, or towards home
    if (nextAnim == ENHORSE_ANIM_GALLOP || nextAnim == ENHORSE_ANIM_TROT || nextAnim == ENHORSE_ANIM_WALK) {
        if (playerDistToHome < 300.0f) {
            yaw = player->actor.shape.rot.y;
            yaw += (Actor_WorldYawTowardActor(&self->actor, &player->actor) > 0 ? 1 : -1) * 0x3FFF;
        } else {
            yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos) - self->actor.world.rot.y;
        }

        if (yaw > 400) {
            self->actor.world.rot.y += 400;
        } else if (yaw < -400) {
            self->actor.world.rot.y -= 400;
        } else {
            self->actor.world.rot.y += yaw;
        }

        self->actor.shape.rot.y = self->actor.world.rot.y;
    }

    animFinished = SkelAnime_Update(&self->skin.skelAnime);

    if (self->animationIdx == ENHORSE_ANIM_IDLE || self->animationIdx == ENHORSE_ANIM_WHINNEY) {
        if (nextAnim == ENHORSE_ANIM_GALLOP || nextAnim == ENHORSE_ANIM_TROT || nextAnim == ENHORSE_ANIM_WALK) {
            self->animationIdx = nextAnim;
            Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                             Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                             -3.0f);
            if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
                EnHorse_PlayGallopingSound(self);
            } else if (self->animationIdx == ENHORSE_ANIM_TROT) {
                EnHorse_PlayTrottingSound(self);
            }
            return;
        }
    }

    if (animFinished) {
        if (nextAnim == ENHORSE_ANIM_GALLOP) {
            EnHorse_PlayGallopingSound(self);
        } else if (nextAnim == ENHORSE_ANIM_TROT) {
            EnHorse_PlayTrottingSound(self);
        }

        if (self->animationIdx == ENHORSE_ANIM_IDLE || self->animationIdx == ENHORSE_ANIM_WHINNEY) {
            if (nextAnim != self->animationIdx) {
                self->animationIdx = nextAnim;
                Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                                 Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]),
                                 ANIMMODE_ONCE, -3.0f);
                return;
            } else {
                if (Rand_ZeroOne() > 0.5f) {
                    self->animationIdx = ENHORSE_ANIM_IDLE;
                    self->stateFlags &= ~ENHORSE_SANDDUST_SOUND;
                } else {
                    self->animationIdx = ENHORSE_ANIM_WHINNEY;
                    self->unk_21C = self->unk_228;
                    if (self->stateFlags & ENHORSE_DRAW) {
                        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_GROAN, &self->unk_21C, 4, &D_801333E0, &D_801333E0,
                                               &D_801333E8);
                    }
                }
                Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                                 Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]),
                                 ANIMMODE_ONCE, -3.0f);
                return;
            }
        }

        if (nextAnim != self->animationIdx) {
            self->animationIdx = nextAnim;
            Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                             Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                             -3.0f);
        } else {
            Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                             Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                             0.0f);
        }
        return;
    }

    if (self->animationIdx == ENHORSE_ANIM_WALK) {
        if (nextAnim == ENHORSE_ANIM_IDLE || nextAnim == ENHORSE_ANIM_WHINNEY) {
            self->animationIdx = nextAnim;
            Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.0f, 0.0f,
                             Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE,
                             -3.0f);
        }
    }
}

void EnHorse_BridgeJumpInit(EnHorse* self, GlobalContext* globalCtx) {
    f32 y;

    func_80028A54(globalCtx, 25.0f, &self->actor.world.pos);
    self->action = ENHORSE_ACT_BRIDGE_JUMP;
    self->stateFlags |= ENHORSE_JUMPING;
    self->animationIdx = ENHORSE_ANIM_HIGH_JUMP;
    y = self->skin.skelAnime.jointTable->y;
    y = y * 0.01f;
    self->bridgeJumpStart = self->actor.world.pos;
    self->bridgeJumpStart.y += y;
    self->bridgeJumpYVel =
        (((sBridgeJumps[self->bridgeJumpIdx].pos.y + 48.7f) - self->bridgeJumpStart.y) - -360.0f) / 30.0f;
    self->riderPos.y -= y;
    self->stateFlags |= ENHORSE_CALC_RIDER_POS;
    self->bridgeJumpRelAngle = self->actor.world.rot.y - sBridgeJumps[self->bridgeJumpIdx].angle;
    self->bridgeJumpTimer = 0;
    self->actor.gravity = 0.0f;
    self->actor.speedXZ = 0;
    Animation_Change(&self->skin.skelAnime, sAnimationHeaders[self->type][self->animationIdx], 1.5f, 0.0f,
                     Animation_GetLastFrame(sAnimationHeaders[self->type][self->animationIdx]), ANIMMODE_ONCE, -3.0f);
    self->unk_21C = self->unk_228;
    if (self->stateFlags & ENHORSE_DRAW) {
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
    Audio_PlaySoundGeneral(NA_SE_EV_HORSE_JUMP, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    func_800AA000(0.0f, 170, 10, 10);
    self->postDrawFunc = NULL;
}

void EnHorse_StartBridgeJump(EnHorse* self, GlobalContext* globalCtx) {
    self->postDrawFunc = EnHorse_BridgeJumpInit;
    if (self->bridgeJumpIdx == 0) {
        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gGerudoValleyBridgeJumpFieldFortressCs);
        gSaveContext.cutsceneTrigger = 1;
    } else {
        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(gGerudoValleyBridgeJumpFortressToFieldCs);
        gSaveContext.cutsceneTrigger = 1;
    }
}

void EnHorse_BridgeJumpMove(EnHorse* self, GlobalContext* globalCtx) {
    f32 interp;
    f32 timeSq;

    interp = self->bridgeJumpTimer / 30.0f;
    timeSq = (self->bridgeJumpTimer * self->bridgeJumpTimer);

    self->actor.world.pos.x =
        ((sBridgeJumps[self->bridgeJumpIdx].pos.x - self->bridgeJumpStart.x) * interp) + self->bridgeJumpStart.x;
    self->actor.world.pos.z =
        ((sBridgeJumps[self->bridgeJumpIdx].pos.z - self->bridgeJumpStart.z) * interp) + self->bridgeJumpStart.z;

    self->actor.world.pos.y =
        (self->bridgeJumpStart.y + (self->bridgeJumpYVel * self->bridgeJumpTimer) + (-0.4f * timeSq));

    self->actor.world.rot.y = self->actor.shape.rot.y =
        (sBridgeJumps[self->bridgeJumpIdx].angle + ((1.0f - interp) * self->bridgeJumpRelAngle));
    self->skin.skelAnime.curFrame = 23.0f * interp;
    SkelAnime_Update(&self->skin.skelAnime);
    if (self->bridgeJumpTimer < 30) {
        self->stateFlags |= ENHORSE_FLAG_24;
    }
}

void EnHorse_CheckBridgeJumpLanding(EnHorse* self, GlobalContext* globalCtx) {
    self->actor.speedXZ = 8.0f;
    self->skin.skelAnime.playSpeed = 1.5f;
    if (SkelAnime_Update(&self->skin.skelAnime)) {
        self->stateFlags &= ~ENHORSE_JUMPING;
        self->actor.gravity = -3.5f;
        self->actor.world.pos.y = sBridgeJumps[self->bridgeJumpIdx].pos.y;
        func_80028A54(globalCtx, 25.0f, &self->actor.world.pos);
        EnHorse_JumpLanding(self, globalCtx);
        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_LAND, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        func_800AA000(0.0f, 255, 10, 80);
    }
}

void EnHorse_BridgeJump(EnHorse* self, GlobalContext* globalCtx) {
    self->bridgeJumpTimer++;
    if (self->bridgeJumpTimer < 30) {
        EnHorse_BridgeJumpMove(self, globalCtx);
        return;
    }
    EnHorse_CheckBridgeJumpLanding(self, globalCtx);
}

void EnHorse_Vec3fOffset(Vec3f* src, s16 yaw, f32 dist, f32 height, Vec3f* dst) {
    dst->x = src->x + Math_SinS(yaw) * dist;
    dst->y = src->y + height;
    dst->z = src->z + Math_CosS(yaw) * dist;
}

s32 EnHorse_CalcFloorHeight(EnHorse* self, GlobalContext* globalCtx, Vec3f* pos, CollisionPoly** floorPoly,
                            f32* floorHeight) {
    s32 bgId;
    f32 waterY;
    WaterBox* waterBox;

    *floorPoly = NULL;
    *floorHeight = BgCheck_EntityRaycastFloor3(&globalCtx->colCtx, floorPoly, &bgId, pos);

    if (*floorHeight == BGCHECK_Y_MIN) {
        return 1; // No floor
    }

    if (WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, pos->x, pos->z, &waterY, &waterBox) == 1 &&
        *floorHeight < waterY) {
        return 2; // Water
    }

    if ((*floorPoly)->normal.y * 0.00003051851f < 0.81915206f ||
        SurfaceType_IsHorseBlocked(&globalCtx->colCtx, *floorPoly, bgId) ||
        func_80041D4C(&globalCtx->colCtx, *floorPoly, bgId) == 7) {
        return 3; // Horse blocked surface
    }
    return 0;
}

/**
 * obstacleType:
 *  1: Water in front
 *  2: Water behind?
 *  3: ?
 *  4: Obstructed in front
 *  5: Obstructed behind
 */
void EnHorse_ObstructMovement(EnHorse* self, GlobalContext* globalCtx, s32 obstacleType, s32 galloping) {
    if (self->action == ENHORSE_ACT_CS_UPDATE || EnHorse_BgCheckBridgeJumpPoint(self, globalCtx)) {
        return;
    }

    self->actor.world.pos = self->lastPos;
    self->actor.shape.rot.y = self->lastYaw;
    self->actor.world.rot.y = self->lastYaw;
    self->stateFlags |= ENHORSE_OBSTACLE;

    if (!self->playerControlled) {
        if (self->animationIdx != ENHORSE_ANIM_REARING) {
            return;
        }
    } else if (self->action != ENHORSE_ACT_MOUNTED_REARING) {
        if (self->stateFlags & ENHORSE_JUMPING) {
            self->stateFlags &= ~ENHORSE_JUMPING;
            self->actor.gravity = -3.5f;
            self->actor.world.pos.y = self->actor.floorHeight;
        }
        if (obstacleType == 1 || obstacleType == 4) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
        } else if (obstacleType == 2 || obstacleType == 5) {
            self->stateFlags |= ENHORSE_FORCE_WALKING;
        }
        if (galloping == true) {
            EnHorse_StartRearing(self);
        }
    }
}

void EnHorse_CheckFloors(EnHorse* self, GlobalContext* globalCtx) {
    s32 status;
    CollisionPoly* frontFloor;
    CollisionPoly* backFloor;
    s16 floorSlope;
    Vec3f frontPos;
    Vec3f backPos;
    Vec3f pos;
    f32 nx;
    f32 ny;
    f32 nz;
    s32 galloping = self->actor.speedXZ > 8;
    f32 dist;
    f32 waterHeight;
    WaterBox* waterBox;
    s32 pad;

    if (WaterBox_GetSurfaceImpl(globalCtx, &globalCtx->colCtx, self->actor.world.pos.x, self->actor.world.pos.z,
                                &waterHeight, &waterBox) == 1 &&
        self->actor.floorHeight < waterHeight) {
        EnHorse_ObstructMovement(self, globalCtx, 1, galloping);
        return;
    }

    EnHorse_Vec3fOffset(&self->actor.world.pos, self->actor.shape.rot.y, 30.0f, 60.0f, &frontPos);
    status = EnHorse_CalcFloorHeight(self, globalCtx, &frontPos, &frontFloor, &self->yFront);
    if (status == 1) {
        self->actor.shape.rot.x = 0;
        EnHorse_ObstructMovement(self, globalCtx, 4, galloping);
        return;
    }
    if (status == 2) {
        EnHorse_ObstructMovement(self, globalCtx, 4, galloping);
        return;
    }
    if (status == 3) {
        EnHorse_ObstructMovement(self, globalCtx, 4, galloping);
        return;
    }

    EnHorse_Vec3fOffset(&self->actor.world.pos, self->actor.shape.rot.y, -30.0f, 60.0f, &backPos);
    status = EnHorse_CalcFloorHeight(self, globalCtx, &backPos, &backFloor, &self->yBack);
    if (status == 1) {
        self->actor.shape.rot.x = 0;
        EnHorse_ObstructMovement(self, globalCtx, 5, galloping);
        return;
    }
    if (status == 2) {
        EnHorse_ObstructMovement(self, globalCtx, 5, galloping);
        return;
    }
    if (status == 3) {
        EnHorse_ObstructMovement(self, globalCtx, 5, galloping);
        return;
    }

    floorSlope = Math_FAtan2F(self->yBack - self->yFront, 60.0f) * 10430.378f;
    if (self->actor.floorPoly != 0) {
        nx = self->actor.floorPoly->normal.x * COLPOLY_NORMAL_FRAC;
        ny = self->actor.floorPoly->normal.y * COLPOLY_NORMAL_FRAC;
        nz = self->actor.floorPoly->normal.z * COLPOLY_NORMAL_FRAC;
        pos = frontPos;
        pos.y = self->yFront;
        dist = Math3D_DistPlaneToPos(nx, ny, nz, self->actor.floorPoly->dist, &pos);
        if ((frontFloor != self->actor.floorPoly) && (self->actor.speedXZ >= 0.0f)) {
            if ((!(self->stateFlags & ENHORSE_JUMPING) && dist < -40.0f) ||
                (self->stateFlags & ENHORSE_JUMPING && dist < -200.0f)) {
                EnHorse_ObstructMovement(self, globalCtx, 4, galloping);
                return;
            }
        }

        pos = backPos;
        pos.y = self->yBack;
        dist = Math3D_DistPlaneToPos(nx, ny, nz, self->actor.floorPoly->dist, &pos);
        if (((backFloor != self->actor.floorPoly) && (self->actor.speedXZ <= 0.0f) &&
             !(self->stateFlags & ENHORSE_JUMPING) && (dist < -40.0f)) ||
            (self->stateFlags & ENHORSE_JUMPING && dist < -200.0f)) {
            EnHorse_ObstructMovement(self, globalCtx, 5, galloping);
            return;
        }

        if (ny < 0.81915206f ||
            SurfaceType_IsHorseBlocked(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId) ||
            func_80041D4C(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId) == 7) {
            if ((self->actor.speedXZ >= 0.0f)) {
                EnHorse_ObstructMovement(self, globalCtx, 4, galloping);
            } else {
                EnHorse_ObstructMovement(self, globalCtx, 5, galloping);
            }
            return;
        }

        if (self->stateFlags & ENHORSE_JUMPING) {
            self->actor.shape.rot.x = 0;
            return;
        }

        if (self->actor.floorHeight + 4.0f < self->actor.world.pos.y) {
            self->actor.shape.rot.x = 0;
            return;
        }

        if (fabsf(floorSlope) > 8191.0f) {
            return;
        }

        self->actor.shape.rot.x = floorSlope;
        self->actor.shape.yOffset =
            (self->yFront + (((self->yBack - self->yFront) * 20.0f) / 45.0f)) - self->actor.floorHeight;
    }
}

s32 EnHorse_GetMountSide(EnHorse* self, GlobalContext* globalCtx);

void EnHorse_MountDismount(EnHorse* self, GlobalContext* globalCtx) {
    s32 pad[2];
    s32 mountSide;
    Player* player = GET_PLAYER(globalCtx);

    mountSide = EnHorse_GetMountSide(self, globalCtx);
    if (mountSide != 0 && !(self->stateFlags & ENHORSE_UNRIDEABLE) && player->rideActor == NULL) {
        Actor_SetRideActor(globalCtx, &self->actor, mountSide);
    }

    if (self->playerControlled == false && Actor_IsMounted(globalCtx, &self->actor) == true) {
        self->noInputTimer = 55;
        self->noInputTimerMax = 55;
        self->playerControlled = 1;
        EnHorse_Freeze(self);
    } else if (self->playerControlled == true && Actor_NotMounted(globalCtx, &self->actor) == true) {
        self->noInputTimer = 35;
        self->noInputTimerMax = 35;
        self->stateFlags &= ~ENHORSE_UNRIDEABLE;
        self->playerControlled = 0;
        EnHorse_Freeze(self);
    }
}

void EnHorse_StickDirection(Vec2f* curStick, f32* stickMag, s16* angle) {
    f32 dist;
    f32 y;
    f32 x;

    x = curStick->x;
    y = curStick->y;
    dist = sqrtf(SQ(x) + SQ(y));

    *stickMag = dist;
    if (dist > 60.0f) {
        *stickMag = 60.0f;
    } else {
        *stickMag = *stickMag;
    }

    *angle = Math_FAtan2F(-curStick->x, curStick->y) * (32768.0f / M_PI);
}

void EnHorse_UpdateStick(EnHorse* self, GlobalContext* globalCtx) {
    self->lastStick = self->curStick;
    self->curStick.x = globalCtx->state.input[0].rel.stick_x;
    self->curStick.y = globalCtx->state.input[0].rel.stick_y;
}

void EnHorse_ResolveCollision(EnHorse* self, GlobalContext* globalCtx, CollisionPoly* colPoly) {
    f32 dist;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 offset;

    nx = colPoly->normal.x * 0.00003051851f;
    ny = colPoly->normal.y * 0.00003051851f;
    nz = colPoly->normal.z * 0.00003051851f;
    if (!(Math_CosS(self->actor.world.rot.y - (s16)(Math_FAtan2F(colPoly->normal.x, colPoly->normal.z) * 10430.378f) -
                    0x7FFF) < 0.7071f)) {
        dist = Math3D_DistPlaneToPos(nx, ny, nz, colPoly->dist, &self->actor.world.pos);
        offset = (1.0f / sqrtf(SQ(nx) + SQ(nz)));
        offset = (30.0f - dist) * offset;
        self->actor.world.pos.x += offset * nx;
        self->actor.world.pos.z += offset * nz;
    }
}

void EnHorse_BgCheckSlowMoving(EnHorse* self, GlobalContext* globalCtx) {
    f32 yOffset;
    Vec3f start;
    Vec3f end;
    Vec3f intersect;
    CollisionPoly* colPoly;
    s32 bgId;

    if (globalCtx->sceneNum == SCENE_SPOT20) {
        yOffset = 19.0f;
    } else {
        yOffset = 40.0f;
    }
    Math_Vec3f_Copy(&start, &self->actor.world.pos);
    start.y = start.y + yOffset;

    Math_Vec3f_Copy(&end, &start);
    end.x += 30.0f * Math_SinS(self->actor.world.rot.y);
    end.y += 30.0f * Math_SinS(-self->actor.shape.rot.x);
    end.z += 30.0f * Math_CosS(self->actor.world.rot.y);
    if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &start, &end, &intersect, &colPoly, 1, 0, 0, 1, &bgId) != 0) {
        EnHorse_ResolveCollision(self, globalCtx, colPoly);
    }
}

void EnHorse_HighJumpInit(EnHorse* self, GlobalContext* globalCtx);
void EnHorse_Stub2(EnHorse* self);
void EnHorse_Stub1(EnHorse* self);

void EnHorse_UpdateBgCheckInfo(EnHorse* self, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;
    Vec3f startPos;
    Vec3f endPos;
    Vec3f obstaclePos;
    f32 pad3;
    f32 intersectDist;
    CollisionPoly* wall = NULL;
    CollisionPoly* obstacleFloor = NULL;
    s32 bgId;
    f32 obstacleHeight;
    f32 behindObstacleHeight;
    f32 ny;
    s32 movingFast;
    s32 pad5;
    DynaPolyActor* dynaPoly;
    Vec3f intersect;
    Vec3f obstacleTop;

    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, globalCtx->sceneNum == SCENE_SPOT20 ? 19.0f : 40.0f, 35.0f, 100.0f,
                            29);

    if (EnHorse_BgCheckBridgeJumpPoint(self, globalCtx)) {
        return;
    }

    // void 0 trick required to match, but is surely not real. revisit at a later time
    if (self->actor.bgCheckFlags & 8 && Math_CosS(self->actor.wallYaw - ((void)0, self->actor.world).rot.y) < -0.3f) {
        if (self->actor.speedXZ > 4.0f) {
            self->actor.speedXZ -= 1.0f;
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_SANDDUST, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
    }

    if (self->stateFlags & ENHORSE_JUMPING || !self->playerControlled) {
        return;
    }

    if (self->actor.speedXZ < 0.0f) {
        return;
    }

    // Braking or rearing from obstacle
    if (self->action == ENHORSE_ACT_STOPPING || self->action == ENHORSE_ACT_MOUNTED_REARING) {
        return;
    }

    if (self->actor.speedXZ > 8.0f) {
        if (self->actor.speedXZ < 12.8f) {
            intersectDist = 160.0f;
            movingFast = 0;
        } else {
            intersectDist = 230.0f;
            movingFast = 1;
        }
    } else {
        EnHorse_BgCheckSlowMoving(self, globalCtx);
        return;
    }

    startPos = self->actor.world.pos;
    startPos.y += 19.0f;
    endPos = startPos;
    endPos.x += (intersectDist * Math_SinS(self->actor.world.rot.y));
    endPos.y += (intersectDist * Math_SinS(-self->actor.shape.rot.x));
    endPos.z += (intersectDist * Math_CosS(self->actor.world.rot.y));
    intersect = endPos;
    wall = NULL;
    if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &startPos, &endPos, &intersect, &wall, 1, 0, 0, 1, &bgId) == 1) {
        intersectDist = sqrt(Math3D_Vec3fDistSq(&startPos, &intersect));
        self->stateFlags |= ENHORSE_OBSTACLE;
    }

    if (wall != NULL) {
        if (intersectDist < 30.0f) {
            EnHorse_ResolveCollision(self, globalCtx, wall);
        }
        if ((Math_CosS(self->actor.world.rot.y - (s16)(Math_FAtan2F(wall->normal.x, wall->normal.z) * 10430.378f) -
                       0x7FFF) < 0.5f) ||
            SurfaceType_IsHorseBlocked(&globalCtx->colCtx, wall, bgId) != 0) {
            return;
        }

        // too close to jump
        if ((movingFast == false && intersectDist < 80.0f) || (movingFast == true && intersectDist < 150.0f)) {
            if (movingFast == false) {
                self->stateFlags |= ENHORSE_FORCE_REVERSING;
            } else if (movingFast == true) {
                self->stateFlags |= ENHORSE_FORCE_REVERSING;
                EnHorse_StartBraking(self, globalCtx);
            }
            return;
        }

        dynaPoly = DynaPoly_GetActor(&globalCtx->colCtx, bgId);
        if ((self->stateFlags & ENHORSE_FLAG_26) && ((dynaPoly && dynaPoly->actor.id != 0x108) || dynaPoly == 0)) {
            if (movingFast == false) {
                self->stateFlags |= ENHORSE_FORCE_REVERSING;
            } else if (movingFast == true) {
                self->stateFlags |= ENHORSE_FORCE_REVERSING;
                EnHorse_StartBraking(self, globalCtx);
            }
            return;
        }
    }

    // Get obstacle's height
    intersectDist += 5.0f;
    obstaclePos = startPos;
    obstaclePos.x += intersectDist * Math_SinS(self->actor.world.rot.y);
    obstaclePos.y = self->actor.world.pos.y + 120.0f;
    obstaclePos.z += intersectDist * Math_CosS(self->actor.world.rot.y);
    obstacleTop = obstaclePos;
    obstacleTop.y = BgCheck_EntityRaycastFloor3(&globalCtx->colCtx, &obstacleFloor, &bgId, &obstaclePos);
    if (obstacleTop.y == BGCHECK_Y_MIN) {
        return;
    }
    obstacleHeight = obstacleTop.y - self->actor.world.pos.y;
    if (self->actor.floorPoly == NULL || obstacleFloor == NULL) {
        return;
    }

    if (Math3D_DistPlaneToPos(self->actor.floorPoly->normal.x * COLPOLY_NORMAL_FRAC,
                              self->actor.floorPoly->normal.y * COLPOLY_NORMAL_FRAC,
                              self->actor.floorPoly->normal.z * COLPOLY_NORMAL_FRAC, self->actor.floorPoly->dist,
                              &obstacleTop) < -40.0f &&
        Math3D_DistPlaneToPos(
            obstacleFloor->normal.x * COLPOLY_NORMAL_FRAC, obstacleFloor->normal.y * COLPOLY_NORMAL_FRAC,
            obstacleFloor->normal.z * COLPOLY_NORMAL_FRAC, obstacleFloor->dist, &self->actor.world.pos) > 40.0f) {
        if (movingFast == true && self->action != ENHORSE_ACT_STOPPING) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
            EnHorse_StartBraking(self, globalCtx);
        }
        self->stateFlags |= ENHORSE_OBSTACLE;
        return;
    }

    ny = obstacleFloor->normal.y * COLPOLY_NORMAL_FRAC;
    if (ny < 0.81915206f || (SurfaceType_IsHorseBlocked(&globalCtx->colCtx, obstacleFloor, bgId) != 0) ||
        (func_80041D4C(&globalCtx->colCtx, obstacleFloor, bgId) == 7)) {
        if (movingFast == true && self->action != ENHORSE_ACT_STOPPING) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
            EnHorse_StartBraking(self, globalCtx);
        }
        return;
    }

    if (wall == NULL || obstacleTop.y < intersect.y || (self->stateFlags & ENHORSE_CANT_JUMP)) {
        return;
    }

    obstaclePos = startPos;
    obstaclePos.y = self->actor.world.pos.y + 120.0f;
    if (movingFast == false) {
        obstaclePos.x += (276.0f * Math_SinS(self->actor.world.rot.y));
        obstaclePos.z += (276.0f * Math_CosS(self->actor.world.rot.y));
    } else {
        obstaclePos.x += (390.0f * Math_SinS(self->actor.world.rot.y));
        obstaclePos.z += (390.0f * Math_CosS(self->actor.world.rot.y));
    }

    obstacleTop = obstaclePos;
    obstacleTop.y = BgCheck_EntityRaycastFloor3(&globalCtx->colCtx, &obstacleFloor, &bgId, &obstaclePos);
    if (obstacleTop.y == BGCHECK_Y_MIN) {
        return;
    }

    behindObstacleHeight = obstacleTop.y - self->actor.world.pos.y;

    if (obstacleFloor == NULL) {
        return;
    }

    ny = obstacleFloor->normal.y * COLPOLY_NORMAL_FRAC;
    if (ny < 0.81915206f || SurfaceType_IsHorseBlocked(&globalCtx->colCtx, obstacleFloor, bgId) ||
        func_80041D4C(&globalCtx->colCtx, obstacleFloor, bgId) == 7) {
        if (movingFast == true && self->action != ENHORSE_ACT_STOPPING) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
            EnHorse_StartBraking(self, globalCtx);
        }
    } else if (behindObstacleHeight < -DREG(4)) { // -70
        if (movingFast == true && self->action != ENHORSE_ACT_STOPPING) {
            self->stateFlags |= ENHORSE_FORCE_REVERSING;
            EnHorse_StartBraking(self, globalCtx);
        }
    } else if (movingFast == false && obstacleHeight > 19.0f && obstacleHeight <= 40.0f) {
        EnHorse_Stub1(self);
        self->postDrawFunc = EnHorse_LowJumpInit;
    } else if ((movingFast == true && self->actor.speedXZ < 13.8f && obstacleHeight > 19.0f &&
                obstacleHeight <= 72.0f) ||
               (self->actor.speedXZ > 13.8f && obstacleHeight <= 112.0f)) {

        EnHorse_Stub2(self);
        self->postDrawFunc = EnHorse_HighJumpInit;
    }
}

void EnHorse_CheckBoost(EnHorse* thisx, GlobalContext* globalCtx2) {
    EnHorse* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    s32 pad;

    if (self->action == ENHORSE_ACT_MOUNTED_WALK || self->action == ENHORSE_ACT_MOUNTED_TROT ||
        self->action == ENHORSE_ACT_MOUNTED_GALLOP) {
        if (CHECK_BTN_ALL(globalCtx->state.input[0].press.button, BTN_A) && (globalCtx->interfaceCtx.unk_1EE == 8)) {
            if (!(self->stateFlags & ENHORSE_BOOST) && !(self->stateFlags & ENHORSE_FLAG_8) &&
                !(self->stateFlags & ENHORSE_FLAG_9)) {
                if (self->numBoosts > 0) {
                    func_800AA000(0.0f, 180, 20, 100);
                    self->stateFlags |= ENHORSE_BOOST;
                    self->stateFlags |= ENHORSE_FIRST_BOOST_REGEN;
                    self->stateFlags |= ENHORSE_FLAG_8;
                    self->numBoosts--;
                    self->boostTimer = 0;
                    if (self->numBoosts == 0) {
                        self->boostRegenTime = 140;
                        return;
                    }
                    if (self->type == HORSE_EPONA) {
                        if (self->stateFlags & ENHORSE_FIRST_BOOST_REGEN) {
                            self->boostRegenTime = 60;
                            self->stateFlags &= ~ENHORSE_FIRST_BOOST_REGEN;
                        } else {
                            self->boostRegenTime = 8; // Never happens
                        }
                    } else {
                        self->boostRegenTime = 70;
                    }
                    return;
                }
                self->unk_21C = self->unk_228;
                if (self->stateFlags & ENHORSE_DRAW) {
                    if (Rand_ZeroOne() < 0.1f) {
                        Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0,
                                               &D_801333E8);
                    }
                }
            }
        }
    }
}

void EnHorse_RegenBoost(EnHorse* self, GlobalContext* globalCtx) {
    if (self->numBoosts < 6 && self->numBoosts > 0) {
        self->boostRegenTime--;
        self->boostTimer++;

        if (self->boostRegenTime <= 0) {
            self->numBoosts = self->numBoosts + 1;

            if (!EN_HORSE_CHECK_4(self)) {
                Audio_PlaySoundGeneral(NA_SE_SY_CARROT_RECOVER, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            }

            if (self->numBoosts < 6) {
                self->boostRegenTime = 11;
            }
        }
    } else if (self->numBoosts == 0) {
        self->boostRegenTime--;
        self->boostTimer++;

        if (self->boostRegenTime <= 0) {
            self->boostRegenTime = 0;
            self->numBoosts = 6;

            if (!EN_HORSE_CHECK_4(self)) {
                Audio_PlaySoundGeneral(NA_SE_SY_CARROT_RECOVER, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            }
        }
    }

    if (self->boostTimer == 8 && Rand_ZeroOne() < 0.25f) {
        self->unk_21C = self->unk_228;
        if (self->stateFlags & ENHORSE_DRAW) {
            Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        }
    }
    globalCtx->interfaceCtx.numHorseBoosts = self->numBoosts;
}

void EnHorse_UpdatePlayerDir(EnHorse* self, GlobalContext* globalCtx) {
    EnHorse* pad;
    s16 angle;
    f32 s;
    f32 c;

    angle = Actor_WorldYawTowardActor(&self->actor, &GET_PLAYER(globalCtx)->actor) - self->actor.world.rot.y;
    s = Math_SinS(angle);
    c = Math_CosS(angle);
    if (s > 0.8660254f) {
        self->playerDir = PLAYER_DIR_SIDE_L;
        return;
    }
    if (s < -0.8660254f) {
        self->playerDir = PLAYER_DIR_SIDE_R;
        return;
    }
    if (c > 0.0f) {
        if (s > 0) {
            self->playerDir = PLAYER_DIR_FRONT_L;
        } else {
            self->playerDir = PLAYER_DIR_FRONT_R;
        }
    } else {
        if (s > 0) {
            self->playerDir = PLAYER_DIR_BACK_L;
        } else {
            self->playerDir = PLAYER_DIR_BACK_R;
        }
    }
}

void EnHorse_TiltBody(EnHorse* self, GlobalContext* globalCtx) {
    f32 speed;
    f32 rollDiff;
    s32 targetRoll;
    s16 turnVel;

    speed = self->actor.speedXZ / self->boostSpeed;
    turnVel = self->actor.shape.rot.y - self->lastYaw;
    targetRoll = -((s16)((1820.0f * speed) * (turnVel / 480.00003f)));
    rollDiff = targetRoll - self->actor.world.rot.z;

    if (fabsf(targetRoll) < 100.0f) {
        self->actor.world.rot.z = 0;
    } else if (fabsf(rollDiff) < 100.0f) {
        self->actor.world.rot.z = targetRoll;
    } else if (rollDiff > 0.0f) {
        self->actor.world.rot.z += 100;
    } else {
        self->actor.world.rot.z -= 100;
    }

    self->actor.shape.rot.z = self->actor.world.rot.z;
}

s32 EnHorse_UpdateConveyors(EnHorse* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s16 conveyorDir;

    if ((self->actor.floorPoly == NULL) && (self != (EnHorse*)player->rideActor)) {
        return 0;
    }
    conveyorDir = SurfaceType_GetConveyorDirection(&globalCtx->colCtx, self->actor.floorPoly, self->actor.floorBgId);
    conveyorDir = (conveyorDir << 10) - self->actor.world.rot.y;
    if (conveyorDir > 800.0f) {
        self->actor.world.rot.y += 800.0f;
    } else if (conveyorDir < -800.0f) {
        self->actor.world.rot.y -= 800.0f;
    } else {
        self->actor.world.rot.y += conveyorDir;
    }
    self->actor.shape.rot.y = self->actor.world.rot.y;

    return 1;
}

s32 EnHorse_RandInt(f32 range) {
    return Rand_ZeroOne() * range;
}

void EnHorse_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnHorse* self = THIS;
    GlobalContext* globalCtx = globalCtx2;
    Vec3f dustAcc = { 0.0f, 0.0f, 0.0f };
    Vec3f dustVel = { 0.0f, 1.0f, 0.0f };
    Player* player = GET_PLAYER(globalCtx);

    self->lastYaw = thisx->shape.rot.y;
    EnHorse_UpdateStick(self, globalCtx);
    EnHorse_UpdatePlayerDir(self, globalCtx);

    if (!(self->stateFlags & ENHORSE_INACTIVE)) {
        EnHorse_MountDismount(self, globalCtx);
    }

    if (self->stateFlags & ENHORSE_FLAG_19) {
        if (self->stateFlags & ENHORSE_FLAG_20 && self->inRace == true) {
            self->stateFlags &= ~ENHORSE_FLAG_20;
            EnHorse_StartRearing(self);
        } else if (!(self->stateFlags & ENHORSE_FLAG_20) && self->stateFlags & ENHORSE_FLAG_21 &&
                   self->action != ENHORSE_ACT_MOUNTED_REARING && self->inRace == true) {
            self->stateFlags &= ~ENHORSE_FLAG_21;
            EnHorse_StartRearing(self);
        }
    }

    sActionFuncs[self->action](self, globalCtx);
    self->stateFlags &= ~ENHORSE_OBSTACLE;
    self->curFrame = self->skin.skelAnime.curFrame;
    self->lastPos = thisx->world.pos;
    if (!(self->stateFlags & ENHORSE_INACTIVE)) {
        if (self->action == ENHORSE_ACT_MOUNTED_GALLOP || self->action == ENHORSE_ACT_MOUNTED_TROT ||
            self->action == ENHORSE_ACT_MOUNTED_WALK) {
            EnHorse_CheckBoost(self, globalCtx);
        }
        if (self->playerControlled == true) {
            EnHorse_RegenBoost(self, globalCtx);
        }
        Actor_MoveForward(thisx);
        if (self->action == ENHORSE_ACT_INGO_RACE) {
            if (self->rider != NULL) {
                self->rider->world.pos.x = thisx->world.pos.x;
                self->rider->world.pos.y = thisx->world.pos.y + 10.0f;
                self->rider->world.pos.z = thisx->world.pos.z;
                self->rider->shape.rot.x = thisx->shape.rot.x;
                self->rider->shape.rot.y = thisx->shape.rot.y;
            }
        }
        if (self->jntSph.elements[0].info.ocElemFlags & 2) {
            if (thisx->speedXZ > 6.0f) {
                thisx->speedXZ -= 1.0f;
            }
        }
        if (self->jntSph.base.acFlags & 2) {
            self->unk_21C = self->unk_228;
            if (self->stateFlags & ENHORSE_DRAW) {
                Audio_PlaySoundGeneral(NA_SE_EV_HORSE_NEIGH, &self->unk_21C, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            }
        }
        if (self->action != ENHORSE_ACT_INGO_RACE) {
            EnHorse_TiltBody(self, globalCtx);
        }
        Collider_UpdateCylinder(thisx, &self->cyl1);
        Collider_UpdateCylinder(thisx, &self->cyl2);

        // Required to match
        self->cyl1.dim.pos.x = self->cyl1.dim.pos.x + (s16)(Math_SinS(thisx->shape.rot.y) * 11.0f);
        self->cyl1.dim.pos.z = self->cyl1.dim.pos.z + (s16)(Math_CosS(thisx->shape.rot.y) * 11.0f);
        self->cyl2.dim.pos.x = self->cyl2.dim.pos.x + (s16)(Math_SinS(thisx->shape.rot.y) * -18.0f);
        self->cyl2.dim.pos.z = self->cyl2.dim.pos.z + (s16)(Math_CosS(thisx->shape.rot.y) * -18.0f);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->cyl1.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->cyl1.base);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->cyl2.base);
        if ((player->stateFlags1 & 1) && player->rideActor != NULL) {
            if (globalCtx->sceneNum != SCENE_SPOT20 ||
                (globalCtx->sceneNum == SCENE_SPOT20 && (thisx->world.pos.z < -2400.0f))) {
                EnHorse_UpdateConveyors(self, globalCtx);
            }
        }
        if (!(self->stateFlags & ENHORSE_FLAG_24)) {
            EnHorse_UpdateBgCheckInfo(self, globalCtx);
            EnHorse_CheckFloors(self, globalCtx);
            if (thisx->world.pos.y < self->yFront && thisx->world.pos.y < self->yBack) {
                if (self->yBack < self->yFront) {
                    thisx->world.pos.y = self->yBack;
                } else {
                    thisx->world.pos.y = self->yFront;
                }
            }

        } else {
            self->stateFlags &= ~ENHORSE_FLAG_24;
        }

        if (globalCtx->sceneNum == SCENE_SPOT09 && (gSaveContext.eventChkInf[9] & 0xF) != 0xF) {
            EnHorse_CheckBridgeJumps(self, globalCtx);
        }

        thisx->focus.pos = thisx->world.pos;
        thisx->focus.pos.y += 70.0f;
        if ((Rand_ZeroOne() < 0.025f) && self->blinkTimer == 0) {
            self->blinkTimer++;
        } else if (self->blinkTimer > 0) {
            self->blinkTimer++;
            if (self->blinkTimer >= 4) {
                self->blinkTimer = 0;
            }
        }

        if (thisx->speedXZ == 0.0f && !(self->stateFlags & ENHORSE_FLAG_19)) {
            thisx->colChkInfo.mass = 0xFF;
        } else {
            thisx->colChkInfo.mass = 0xFE;
        }

        if (thisx->speedXZ >= 5.0f) {
            self->cyl1.base.atFlags |= 1;
        } else {
            self->cyl1.base.atFlags &= ~1;
        }

        if (gSaveContext.entranceIndex != 343 || gSaveContext.sceneSetupIndex != 9) {
            if (self->dustFlags & 1) {
                self->dustFlags &= ~1;
                func_800287AC(globalCtx, &self->frontRightHoof, &dustVel, &dustAcc, EnHorse_RandInt(100) + 200,
                              EnHorse_RandInt(10) + 30, EnHorse_RandInt(20) + 30);
            } else if (self->dustFlags & 2) {
                self->dustFlags &= ~2;
                func_800287AC(globalCtx, &self->frontLeftHoof, &dustVel, &dustAcc, EnHorse_RandInt(100) + 200,
                              EnHorse_RandInt(10) + 30, EnHorse_RandInt(20) + 30);
            } else if (self->dustFlags & 4) {
                self->dustFlags &= ~4;
                func_800287AC(globalCtx, &self->backRightHoof, &dustVel, &dustAcc, EnHorse_RandInt(100) + 200,
                              EnHorse_RandInt(10) + 30, EnHorse_RandInt(20) + 30);
            } else if (self->dustFlags & 8) {
                self->dustFlags &= ~8;
                func_800287AC(globalCtx, &self->backLeftHoof, &dustVel, &dustAcc, EnHorse_RandInt(100) + 200,
                              EnHorse_RandInt(10) + 30, EnHorse_RandInt(20) + 30);
            }
        }
        self->stateFlags &= ~ENHORSE_DRAW;
    }
}

s32 EnHorse_PlayerDirToMountSide(EnHorse* self, GlobalContext* globalCtx, Player* player) {
    if (self->playerDir == PLAYER_DIR_SIDE_L) {
        return -1;
    }
    if (self->playerDir == PLAYER_DIR_SIDE_R) {
        return 1;
    }
    return 0;
}

s32 EnHorse_MountSideCheck(EnHorse* self, GlobalContext* globalCtx, Player* player) {
    s32 mountSide;

    if (Actor_WorldDistXZToActor(&self->actor, &player->actor) > 75.0f) {
        return 0;
    } else if (fabsf(self->actor.world.pos.y - player->actor.world.pos.y) > 30.0f) {
        return 0;
    } else if (Math_CosS(Actor_WorldYawTowardActor(&player->actor, &self->actor) - player->actor.world.rot.y) <
               0.17364818f) {
        return 0;
    } else {
        mountSide = EnHorse_PlayerDirToMountSide(self, globalCtx, player);
        if (mountSide == -1) {
            return -1;
        }
        if (mountSide == 1) {
            return 1;
        }
    }
    return 0;
}

s32 EnHorse_GetMountSide(EnHorse* self, GlobalContext* globalCtx) {
    if (self->action != ENHORSE_ACT_IDLE) {
        return 0;
    }
    if ((self->animationIdx != ENHORSE_ANIM_IDLE) && (self->animationIdx != ENHORSE_ANIM_WHINNEY)) {
        return 0;
    }
    return EnHorse_MountSideCheck(self, globalCtx, GET_PLAYER(globalCtx));
}

void EnHorse_RandomOffset(Vec3f* src, f32 dist, Vec3f* dst) {
    dst->x = (Rand_ZeroOne() * (dist * 2.0f) + src->x) - dist;
    dst->y = (Rand_ZeroOne() * (dist * 2.0f) + src->y) - dist;
    dst->z = (Rand_ZeroOne() * (dist * 2.0f) + src->z) - dist;
}

void EnHorse_SkinCallback1(Actor* thisx, GlobalContext* globalCtx, PSkinAwb* skin) {
    EnHorse* self = THIS;
    s32 pad;
    Vec3f sp94 = { 0.0f, 0.0f, 0.0f };
    Vec3f hoofOffset = { 5.0f, -4.0f, 5.0f };
    Vec3f riderOffset = { 600.0f, -1670.0f, 0.0f };
    Vec3f sp70;
    Vec3f sp64 = { 0.0f, 0.0f, 0.0f };
    Vec3f sp58 = { 0.0f, -1.0f, 0.0f };

    f32 frame = self->skin.skelAnime.curFrame;
    Vec3f center;
    Vec3f newCenter;
    s32 i;
    Vec3f sp2C;
    f32 sp28;

    if (!(self->stateFlags & ENHORSE_CALC_RIDER_POS)) {
        func_800A6408(skin, 30, &riderOffset, &self->riderPos);
        self->riderPos.x = self->riderPos.x - self->actor.world.pos.x;
        self->riderPos.y = self->riderPos.y - self->actor.world.pos.y;
        self->riderPos.z = self->riderPos.z - self->actor.world.pos.z;
    } else {
        self->stateFlags &= ~ENHORSE_CALC_RIDER_POS;
    }

    func_800A6408(skin, 13, &sp94, &sp2C);
    SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, &sp2C, &self->unk_228, &sp28);
    if ((self->animationIdx == ENHORSE_ANIM_IDLE && self->action != ENHORSE_ACT_FROZEN) &&
        ((frame > 40.0f && frame < 45.0f && self->type == HORSE_EPONA) ||
         (frame > 28.0f && frame < 33.0f && self->type == HORSE_HNI))) {
        if (Rand_ZeroOne() < 0.6f) {
            self->dustFlags |= 1;
            func_800A6408(skin, 28, &hoofOffset, &self->frontRightHoof);
            self->frontRightHoof.y = self->frontRightHoof.y - 5.0f;
        }
    } else {
        if (self->action == ENHORSE_ACT_STOPPING) {
            if ((frame > 10.0f && frame < 13.0f) || (frame > 25.0f && frame < 33.0f)) {
                if (Rand_ZeroOne() < 0.7f) {
                    self->dustFlags |= 2;
                    func_800A6408(skin, 20, &hoofOffset, &sp70);
                    EnHorse_RandomOffset(&sp70, 10.0f, &self->frontLeftHoof);
                }
                if (Rand_ZeroOne() < 0.7f) {
                    self->dustFlags |= 1;
                    func_800A6408(skin, 28, &hoofOffset, &sp70);
                    EnHorse_RandomOffset(&sp70, 10.0f, &self->frontRightHoof);
                }
            }

            if ((frame > 6.0f && frame < 10.0f) || (frame > 23.0f && frame < 29.0f)) {
                if (Rand_ZeroOne() < 0.7f) {
                    self->dustFlags |= 8;
                    func_800A6408(skin, 37, &hoofOffset, &sp70);
                    EnHorse_RandomOffset(&sp70, 10.0f, &self->backLeftHoof);
                }
            }

            if ((frame > 7.0f && frame < 14.0f) || (frame > 26.0f && frame < 30.0f)) {
                if (Rand_ZeroOne() < 0.7f) {
                    self->dustFlags |= 4;
                    func_800A6408(skin, 45, &hoofOffset, &sp70);
                    EnHorse_RandomOffset(&sp70, 10.0f, &self->backRightHoof);
                }
            }
        } else if (self->animationIdx == ENHORSE_ANIM_GALLOP) {
            if ((frame > 14.0f) && (frame < 16.0f)) {
                self->dustFlags |= 1;
                func_800A6408(skin, 28, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 5.0f, &self->frontRightHoof);
            } else if (frame > 8.0f && frame < 10.0f) {
                self->dustFlags |= 2;
                func_800A6408(skin, 20, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->frontLeftHoof);
            } else if (frame > 1.0f && frame < 3.0f) {
                self->dustFlags |= 4;
                func_800A6408(skin, 45, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->backRightHoof);
            } else if ((frame > 26.0f) && (frame < 28.0f)) {
                self->dustFlags |= 8;
                func_800A6408(skin, 37, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->backLeftHoof);
            }
        } else if (self->action == ENHORSE_ACT_LOW_JUMP && frame > 6.0f &&
                   Rand_ZeroOne() < 1.0f - (frame - 6.0f) * 0.05882353f) {
            if (Rand_ZeroOne() < 0.5f) {
                self->dustFlags |= 8;
                func_800A6408(skin, 37, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->backLeftHoof);
            }
            if (Rand_ZeroOne() < 0.5f) {
                self->dustFlags |= 4;
                func_800A6408(skin, 45, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->backRightHoof);
            }
        } else if (self->action == ENHORSE_ACT_HIGH_JUMP && frame > 5.0f &&
                   Rand_ZeroOne() < 1.0f - (frame - 5.0f) * 0.04f) {
            if (Rand_ZeroOne() < 0.5f) {
                self->dustFlags |= 8;
                func_800A6408(skin, 37, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->backLeftHoof);
            }
            if (Rand_ZeroOne() < 0.5f) {
                self->dustFlags |= 4;
                func_800A6408(skin, 45, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->backRightHoof);
            }
        } else if (self->action == ENHORSE_ACT_BRIDGE_JUMP && Rand_ZeroOne() < 0.5f) {
            if (Rand_ZeroOne() < 0.5f) {
                self->dustFlags |= 8;
                func_800A6408(skin, 37, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->backLeftHoof);
            } else {
                self->dustFlags |= 4;
                func_800A6408(skin, 45, &hoofOffset, &sp70);
                EnHorse_RandomOffset(&sp70, 10.0f, &self->backRightHoof);
            }
        }
    }

    for (i = 0; i < self->jntSph.count; i++) {
        center.x = self->jntSph.elements[i].dim.modelSphere.center.x;
        center.y = self->jntSph.elements[i].dim.modelSphere.center.y;
        center.z = self->jntSph.elements[i].dim.modelSphere.center.z;

        func_800A6408(skin, self->jntSph.elements[i].dim.limb, &center, &newCenter);
        self->jntSph.elements[i].dim.worldSphere.center.x = newCenter.x;
        self->jntSph.elements[i].dim.worldSphere.center.y = newCenter.y;
        self->jntSph.elements[i].dim.worldSphere.center.z = newCenter.z;
        self->jntSph.elements[i].dim.worldSphere.radius =
            self->jntSph.elements[i].dim.modelSphere.radius * self->jntSph.elements[i].dim.scale;
    }

    //! @bug Setting colliders in a draw function allows for duplicate entries to be added to their respective lists
    //! under certain conditions, like when pausing and unpausing the game.
    //! Actors will draw for a couple of frames between the pauses, but some important logic updates will not occur.
    //! In the case of OC, self can cause unwanted effects such as a very large amount of displacement being applied to
    //! a colliding actor.
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->jntSph.base);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->jntSph.base);
}

// unused
static s32 D_80A667DC[] = { 0, 3, 7, 14 };

s32 EnHorse_SkinCallback2(Actor* thisx, GlobalContext* globalCtx, s32 limbIndex, PSkinAwb* arg3) {
    static void* eyeTextures[] = {
        gEponaEyeOpenTex,
        gEponaEyeHalfTex,
        gEponaEyeClosedTex,
    };
    static u8 eyeBlinkIndexes[] = { 0, 1, 2, 1 };
    EnHorse* self = THIS;
    s32 drawOriginalLimb = true;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_horse.c", 8582);
    if (limbIndex == 13 && self->type == HORSE_EPONA) {
        u8 index = eyeBlinkIndexes[self->blinkTimer];

        gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[index]));
    } else if (self->type == HORSE_HNI && self->stateFlags & ENHORSE_FLAG_18 && limbIndex == 30) {
        func_800A5F60(globalCtx->state.gfxCtx, &self->skin, limbIndex, gHorseIngoGerudoSaddleDL, 0);
        drawOriginalLimb = false;
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_horse.c", 8601);
    return drawOriginalLimb;
}

void EnHorse_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnHorse* self = THIS;

    if (!(self->stateFlags & ENHORSE_INACTIVE)) {
        func_80093D18(globalCtx->state.gfxCtx);
        self->stateFlags |= ENHORSE_DRAW;
        if (self->stateFlags & ENHORSE_JUMPING) {
            func_800A6360(thisx, globalCtx, &self->skin, &EnHorse_SkinCallback1, &EnHorse_SkinCallback2, 0);
        } else {
            func_800A6360(thisx, globalCtx, &self->skin, &EnHorse_SkinCallback1, &EnHorse_SkinCallback2, 1);
        }
        if (self->postDrawFunc != NULL) {
            self->postDrawFunc(self, globalCtx);
        }
    }
}
