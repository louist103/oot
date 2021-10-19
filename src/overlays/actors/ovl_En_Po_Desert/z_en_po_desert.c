/*
 * File: z_en_po_desert.c
 * Overlay: ovl_En_Po_Desert
 * Description: Guide Poe (Haunted Wasteland)
 */

#include "z_en_po_desert.h"
#include "objects/object_po_field/object_po_field.h"

#define FLAGS 0x00001090

#define THIS ((EnPoDesert*)thisx)

void EnPoDesert_Init(Actor* thisx, GlobalContext* globalCtx);
void EnPoDesert_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnPoDesert_Update(Actor* thisx, GlobalContext* globalCtx);
void EnPoDesert_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnPoDesert_SetNextPathPoint(EnPoDesert* self, GlobalContext* globalCtx);
void EnPoDesert_WaitForPlayer(EnPoDesert* self, GlobalContext* globalCtx);
void EnPoDesert_MoveToNextPoint(EnPoDesert* self, GlobalContext* globalCtx);
void EnPoDesert_Disappear(EnPoDesert* self, GlobalContext* globalCtx);

const ActorInit En_Po_Desert_InitVars = {
    ACTOR_EN_PO_DESERT,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_PO_FIELD,
    sizeof(EnPoDesert),
    (ActorFunc)EnPoDesert_Init,
    (ActorFunc)EnPoDesert_Destroy,
    (ActorFunc)EnPoDesert_Update,
    (ActorFunc)EnPoDesert_Draw,
};

static ColliderCylinderInit sColliderInit = {
    {
        COLTYPE_HIT3,
        AT_NONE,
        AC_NONE,
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

static InitChainEntry sInitChain[] = {
    ICHAIN_S8(naviEnemyId, 0x5C, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3200, ICHAIN_STOP),
};

void EnPoDesert_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnPoDesert* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    SkelAnime_Init(globalCtx, &self->skelAnime, &gPoeFieldSkel, &gPoeFieldFloatAnim, self->jointTable, self->morphTable,
                   10);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sColliderInit);
    self->lightColor.r = 255;
    self->lightColor.g = 255;
    self->lightColor.b = 210;
    self->lightColor.a = 255;
    self->lightNode = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &self->lightInfo);
    Lights_PointNoGlowSetInfo(&self->lightInfo, self->actor.home.pos.x, self->actor.home.pos.y, self->actor.home.pos.z,
                              255, 255, 255, 200);
    ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 37.0f);
    self->currentPathPoint = 1;
    self->actor.params = (self->actor.params >> 8) & 0xFF;
    self->targetY = self->actor.world.pos.y;
    EnPoDesert_SetNextPathPoint(self, globalCtx);
}

void EnPoDesert_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnPoDesert* self = THIS;

    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, self->lightNode);
    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnPoDesert_SetNextPathPoint(EnPoDesert* self, GlobalContext* globalCtx) {
    Path* path = &globalCtx->setupPathList[self->actor.params];
    Vec3s* pathPoint;

    Animation_MorphToLoop(&self->skelAnime, &gPoeFieldDisappearAnim, -6.0f);
    pathPoint = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[self->currentPathPoint];
    self->actor.home.pos.x = pathPoint->x;
    self->actor.home.pos.y = pathPoint->y;
    self->actor.home.pos.z = pathPoint->z;
    self->initDistToNextPoint = Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos);
    self->initDistToNextPoint = CLAMP_MIN(self->initDistToNextPoint, 1.0f);
    self->currentPathPoint++;
    self->yDiff = self->actor.home.pos.y - self->actor.world.pos.y;
    self->actor.speedXZ = 0.0f;
    if (path->count == self->currentPathPoint) {
        self->currentPathPoint = 0;
    }
    self->actionFunc = EnPoDesert_WaitForPlayer;
}

void EnPoDesert_SetupMoveToNextPoint(EnPoDesert* self) {
    Animation_MorphToLoop(&self->skelAnime, &gPoeFieldFloatAnim, -5.0f);
    self->actionFunc = EnPoDesert_MoveToNextPoint;
}

void EnPoDesert_SetupDisappear(EnPoDesert* self) {
    Animation_MorphToPlayOnce(&self->skelAnime, &gPoeFieldDisappearAnim, -6.0f);
    self->actionTimer = 16;
    self->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&self->actor, NA_SE_EN_PO_DISAPPEAR);
    self->actionFunc = EnPoDesert_Disappear;
}

void EnPoDesert_UpdateSpeedModifier(EnPoDesert* self) {
    if (self->speedModifier == 0) {
        self->speedModifier = 32;
    }
    if (self->speedModifier != 0) {
        self->speedModifier--;
    }
    self->actor.world.pos.y = Math_SinS(self->speedModifier * 0x800) * 13.0f + self->targetY;
}

void EnPoDesert_WaitForPlayer(EnPoDesert* self, GlobalContext* globalCtx) {
    func_8002F974(&self->actor, NA_SE_EN_PO_FLY - SFX_FLAG);
    if (self->actor.xzDistToPlayer < 200.0f && (self->currentPathPoint != 2 || globalCtx->actorCtx.unk_03)) {
        if (self->currentPathPoint == 2) {
            if (Gameplay_InCsMode(globalCtx)) {
                self->actor.shape.rot.y += 0x800;
                return;
            }
            func_8010B680(globalCtx, 0x600B, NULL);
        }
        EnPoDesert_SetupMoveToNextPoint(self);
    } else {
        self->actor.shape.rot.y += 0x800;
    }
}

void EnPoDesert_MoveToNextPoint(EnPoDesert* self, GlobalContext* globalCtx) {
    f32 temp_f20;

    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    temp_f20 = sinf(self->actionTimer * (M_PI / 20.0f)) * 5.0f;
    self->actor.world.pos.x += temp_f20 * Math_CosS(self->actor.shape.rot.y);
    self->actor.world.pos.z += temp_f20 * Math_SinS(self->actor.shape.rot.y);
    if (self->actionTimer == 0) {
        self->actionTimer = 40;
    }
    temp_f20 = Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos);
    self->actor.world.rot.y = Actor_WorldYawTowardPoint(&self->actor, &self->actor.home.pos);
    Math_ApproachS(&self->actor.shape.rot.y, self->actor.world.rot.y + 0x8000, 5, 0x400);
    self->actor.speedXZ = sinf(self->speedModifier * (M_PI / 32.0f)) * 2.5f + 5.5f;
    func_8002F974(&self->actor, NA_SE_EN_PO_FLY - SFX_FLAG);
    self->targetY = self->actor.home.pos.y - ((temp_f20 * self->yDiff) / self->initDistToNextPoint);
    if (temp_f20 < 40.0f) {
        if (self->currentPathPoint != 0) {
            EnPoDesert_SetNextPathPoint(self, globalCtx);
        } else {
            EnPoDesert_SetupDisappear(self);
        }
    }
}

void EnPoDesert_Disappear(EnPoDesert* self, GlobalContext* globalCtx) {
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    self->actor.shape.rot.y += 0x2000;
    self->lightColor.a = self->actionTimer * 15.9375f;
    self->actor.shape.shadowAlpha = self->lightColor.a;
    if (self->actionTimer == 0) {
        Actor_Kill(&self->actor);
    }
}

void EnPoDesert_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnPoDesert* self = THIS;
    s32 pad;

    SkelAnime_Update(&self->skelAnime);
    self->actionFunc(self, globalCtx);
    Actor_MoveForward(&self->actor);
    EnPoDesert_UpdateSpeedModifier(self);
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 0.0f, 27.0f, 60.0f, 4);
    Actor_SetFocus(&self->actor, 42.0f);
    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    if (globalCtx->actorCtx.unk_03) {
        self->actor.flags |= 0x81;
        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
    } else {
        self->actor.shape.shadowDraw = NULL;
        self->actor.flags &= ~0x81;
    }
}

s32 EnPoDesert_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                void* thisx, Gfx** gfxP) {
    EnPoDesert* self = THIS;
    f32 mtxScale;

    if (self->actionFunc == EnPoDesert_Disappear && limbIndex == 7) {
        mtxScale = self->actionTimer / 16.0f;
        Matrix_Scale(mtxScale, mtxScale, mtxScale, MTXMODE_APPLY);
    }
    if ((self->actor.flags & 0x80) != 0x80) {
        *dList = NULL;
    }
    return false;
}

void EnPoDesert_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx,
                             Gfx** gfxP) {
    static Vec3f baseLightPos = { 0.0f, 1400.0f, 0.0f };

    EnPoDesert* self = THIS;
    f32 rand;
    Color_RGBA8 color;
    Vec3f lightPos;

    if (limbIndex == 7) {
        Matrix_MultVec3f(&baseLightPos, &lightPos);
        rand = Rand_ZeroOne();
        color.r = (s16)(rand * 30.0f) + 225;
        color.g = (s16)(rand * 100.0f) + 155;
        color.b = (s16)(rand * 160.0f) + 95;
        if ((self->actor.flags & 0x80) == 0x80) {
            gDPPipeSync((*gfxP)++);
            gDPSetEnvColor((*gfxP)++, color.r, color.g, color.b, 255);
            gSPMatrix((*gfxP)++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_po_desert.c", 523),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList((*gfxP)++, gPoeFieldLanternDL);
            gSPDisplayList((*gfxP)++, gPoeFieldLanternTopDL);
            gDPPipeSync((*gfxP)++);
            gDPSetEnvColor((*gfxP)++, self->lightColor.r, self->lightColor.g, self->lightColor.b, self->lightColor.a);
        }
        Lights_PointNoGlowSetInfo(&self->lightInfo, lightPos.x, lightPos.y, lightPos.z, color.r, color.g, color.b, 200);
    }
}

void EnPoDesert_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnPoDesert* self = THIS;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_po_desert.c", 559);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_XLU_DISP++, 0x0A, Gfx_EnvColor(globalCtx->state.gfxCtx, 255, 85, 0, 255));
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_EnvColor(globalCtx->state.gfxCtx, self->lightColor.r, self->lightColor.g, self->lightColor.b,
                            self->lightColor.a));
    if (self->actionFunc == EnPoDesert_Disappear) {
        gSPSegment(POLY_XLU_DISP++, 0x0C, D_80116280);
    } else {
        gSPSegment(POLY_XLU_DISP++, 0x0C, D_80116280 + 2);
    }
    POLY_XLU_DISP = SkelAnime_Draw(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable,
                                   EnPoDesert_OverrideLimbDraw, EnPoDesert_PostLimbDraw, &self->actor, POLY_XLU_DISP);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_po_desert.c", 597);
}
