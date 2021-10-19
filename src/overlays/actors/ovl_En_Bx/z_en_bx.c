/*
 * File: z_en_bx.c
 * Overlay: ovl_En_Bx
 * Description: Jabu-Jabu Electrified Tentacle
 */

#include "z_en_bx.h"
#include "objects/object_bxa/object_bxa.h"

#define FLAGS 0x00000010

#define THIS ((EnBx*)thisx)

void EnBx_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBx_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBx_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBx_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Bx_InitVars = {
    ACTOR_EN_BX,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_BXA,
    sizeof(EnBx),
    (ActorFunc)EnBx_Init,
    (ActorFunc)EnBx_Destroy,
    (ActorFunc)EnBx_Update,
    (ActorFunc)EnBx_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0xFFCFFFFF, 0x03, 0x04 },
        { 0xFFCFFFFF, 0x01, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 60, 100, 100, { 0, 0, 0 } },
};

static ColliderQuadInit sQuadInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x03, 0x04 },
        { 0x00000000, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

void EnBx_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBx* self = THIS;
    Vec3f sp48 = { 0.015f, 0.015f, 0.015f };
    Vec3f sp3C = { 0.0f, 0.0f, 0.0f };
    static InitChainEntry sInitChain[] = {
        ICHAIN_F32(targetArrowOffset, 5300, ICHAIN_STOP),
    };
    s32 i;
    s32 pad;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    thisx->scale.x = thisx->scale.z = 0.01f;
    thisx->scale.y = 0.03f;

    thisx->world.pos.y = thisx->world.pos.y - 100.0f;
    for (i = 0; i < 4; i++) {
        self->unk_184[i] = sp48;
        if (i == 0) {
            self->unk_1B4[i].x = thisx->shape.rot.x - 0x4000;
        }
        self->unk_154[i] = thisx->world.pos;
        self->unk_154[i].y = thisx->world.pos.y + ((i + 1) * 140.0f);
    }

    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 48.0f);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    Collider_InitQuad(globalCtx, &self->colliderQuad);
    Collider_SetQuad(globalCtx, &self->colliderQuad, &self->actor, &sQuadInit);
    thisx->colChkInfo.mass = MASS_IMMOVABLE;
    self->unk_14C = 0;
    thisx->uncullZoneDownward = 2000.0f;
    if (Flags_GetSwitch(globalCtx, (thisx->params >> 8) & 0xFF)) {
        Actor_Kill(&self->actor);
    }
    thisx->params &= 0xFF;
}

void EnBx_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnBx* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void func_809D1D0C(Actor* thisx, GlobalContext* globalCtx) {
    Vec3f sp5C = { 8000.0f, 15000.0f, 2500.0f };
    Vec3f sp50 = { 8000.0f, 10000.0f, 2500.0f };
    static Vec3f D_809D2540 = { -8000.0f, 15000.0f, 2500.0f };
    static Vec3f D_809D254C = { -8000.0f, 10000.0f, 2500.0f };
    Vec3f sp44;
    Vec3f sp38;
    EnBx* self = THIS;

    Matrix_MultVec3f(&D_809D2540, &sp44);
    Matrix_MultVec3f(&D_809D254C, &sp38);
    Matrix_MultVec3f(&sp5C, &self->colliderQuad.dim.quad[1]);
    Matrix_MultVec3f(&sp50, &self->colliderQuad.dim.quad[0]);
    Collider_SetQuadVertices(&self->colliderQuad, &sp38, &sp44, &self->colliderQuad.dim.quad[0],
                             &self->colliderQuad.dim.quad[1]);
}

void EnBx_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBx* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    s32 i;
    s16 tmp32;
    s32 tmp33;

    if ((thisx->xzDistToPlayer <= 70.0f) || (self->collider.base.atFlags & AT_HIT) ||
        (self->collider.base.acFlags & AC_HIT) || (self->colliderQuad.base.atFlags & AT_HIT)) {
        if ((thisx->xzDistToPlayer <= 70.0f) || (&player->actor == self->collider.base.at) ||
            (&player->actor == self->collider.base.ac) || (&player->actor == self->colliderQuad.base.at)) {
            tmp33 = player->invincibilityTimer & 0xFF;
            tmp32 = thisx->world.rot.y;
            if (!(thisx->params & 0x80)) {
                tmp32 = thisx->yawTowardsPlayer;
            }
            if ((&player->actor != self->collider.base.at) && (&player->actor != self->collider.base.ac) &&
                (&player->actor != self->colliderQuad.base.at) && (player->invincibilityTimer <= 0)) {
                if (player->invincibilityTimer < -39) {
                    player->invincibilityTimer = 0;
                } else {
                    player->invincibilityTimer = 0;
                    globalCtx->damagePlayer(globalCtx, -4);
                }
            }
            func_8002F71C(globalCtx, &self->actor, 6.0f, tmp32, 6.0f);
            player->invincibilityTimer = tmp33;
        }

        self->collider.base.atFlags &= ~AT_HIT;
        self->collider.base.acFlags &= ~AC_HIT;
        self->colliderQuad.base.atFlags &= ~AT_HIT;
        self->colliderQuad.base.at = NULL;
        self->collider.base.ac = NULL;
        self->collider.base.at = NULL;
        self->unk_14C = 0x14;
    }

    if (self->unk_14C != 0) {
        self->unk_14C--;
        for (i = 0; i < 4; i++) {
            if (!((self->unk_14C + (i << 1)) % 4)) {
                static Color_RGBA8 primColor = { 255, 255, 255, 255 };
                static Color_RGBA8 envColor = { 200, 255, 255, 255 };
                Vec3f pos;
                s16 yaw;

                yaw = (s32)Rand_CenteredFloat(12288.0f);
                yaw = (yaw + (i * 0x4000)) + 0x2000;
                pos.x = Rand_CenteredFloat(5.0f) + thisx->world.pos.x;
                pos.y = Rand_CenteredFloat(30.0f) + thisx->world.pos.y + 170.0f;
                pos.z = Rand_CenteredFloat(5.0f) + thisx->world.pos.z;
                EffectSsLightning_Spawn(globalCtx, &pos, &primColor, &envColor, 230, yaw, 6, 0);
            }
        }

        Audio_PlayActorSound2(thisx, NA_SE_EN_BIRI_SPARK - SFX_FLAG);
    }
    thisx->focus.pos = thisx->world.pos;
    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    if (thisx->params & 0x80) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->colliderQuad.base);
    }
}

void EnBx_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* D_809D2560[] = { 0x060024F0, 0x060027F0, 0x060029F0 };
    EnBx* self = THIS;
    s32 pad;
    Mtx* mtx = Graph_Alloc(globalCtx->state.gfxCtx, 4 * sizeof(Mtx));
    s16 i;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bx.c", 464);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x0C, mtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_809D2560[self->actor.params & 0x7F]));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 16, 16, 1, 0, (globalCtx->gameplayFrames * -10) % 128,
                                32, 32));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_bx.c", 478),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (self->actor.params & 0x80) {
        func_809D1D0C(&self->actor, globalCtx);
    }

    self->unk_14E -= 0xBB8;
    thisx->scale.z = thisx->scale.x = (Math_CosS(self->unk_14E) * 0.0075f) + 0.015f;

    for (i = 3; i >= 0; i--) {
        s16 off = (0x2000 * i);
        self->unk_184[i].z = self->unk_184[i].x = (Math_CosS(self->unk_14E + off) * 0.0075f) + 0.015f;
        self->unk_1B4[i].x = thisx->shape.rot.x;
        self->unk_1B4[i].y = thisx->shape.rot.y;
        self->unk_1B4[i].z = thisx->shape.rot.z;
    }

    for (i = 0; i < 4; i++, mtx++) {
        Matrix_Translate(self->unk_154[i].x, self->unk_154[i].y, self->unk_154[i].z, MTXMODE_NEW);
        Matrix_RotateRPY(self->unk_1B4[i].x, self->unk_1B4[i].y, self->unk_1B4[i].z, MTXMODE_APPLY);
        Matrix_Scale(self->unk_184[i].x, self->unk_184[i].y, self->unk_184[i].z, MTXMODE_APPLY);
        Matrix_ToMtx(mtx, "../z_en_bx.c", 507);
    }

    gSPDisplayList(POLY_OPA_DISP++, object_bxa_DL_0022F0);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bx.c", 511);
}
