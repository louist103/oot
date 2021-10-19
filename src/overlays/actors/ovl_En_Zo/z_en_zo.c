/*
 * File: z_en_zo.c
 * Overlay: ovl_En_Zo
 * Description: Zora
 */

#include "z_en_zo.h"
#include "objects/object_zo/object_zo.h"

#define FLAGS 0x00000009

#define THIS ((EnZo*)thisx)

typedef enum {
    /* 0 */ ENZO_EFFECT_NONE,
    /* 1 */ ENZO_EFFECT_RIPPLE,
    /* 2 */ ENZO_EFFECT_SPLASH,
    /* 3 */ ENZO_EFFECT_BUBBLE
} EnZoEffectType;

void EnZo_Init(Actor* thisx, GlobalContext* globalCtx);
void EnZo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnZo_Update(Actor* thisx, GlobalContext* globalCtx);
void EnZo_Draw(Actor* thisx, GlobalContext* globalCtx);

// Actions
void EnZo_Standing(EnZo* self, GlobalContext* globalCtx);
void EnZo_Submerged(EnZo* self, GlobalContext* globalCtx);
void EnZo_Surface(EnZo* self, GlobalContext* globalCtx);
void EnZo_TreadWater(EnZo* self, GlobalContext* globalCtx);
void EnZo_Dive(EnZo* self, GlobalContext* globalCtx);

void EnZo_Ripple(EnZo* self, Vec3f* pos, f32 scale, f32 targetScale, u8 alpha) {
    EnZoEffect* effect;
    Vec3f vec = { 0.0f, 0.0f, 0.0f };
    s16 i;

    effect = self->effects;
    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (effect->type == ENZO_EFFECT_NONE) {
            effect->type = ENZO_EFFECT_RIPPLE;
            effect->pos = *pos;
            effect->scale = scale;
            effect->targetScale = targetScale;
            effect->color.a = alpha;
            break;
        }
        effect++;
    }
}

void EnZo_Bubble(EnZo* self, Vec3f* pos) {
    EnZoEffect* effect;
    Vec3f vec = { 0.0f, 0.0f, 0.0f };
    Vec3f vel = { 0.0f, 1.0f, 0.0f };
    s16 i;
    f32 waterSurface;

    effect = self->effects;
    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (1) {}
        if (effect->type == ENZO_EFFECT_NONE) {
            waterSurface = self->actor.world.pos.y + self->actor.yDistToWater;
            if (!(waterSurface <= pos->y)) {
                effect->type = ENZO_EFFECT_BUBBLE;
                effect->pos = *pos;
                effect->vec = *pos;
                effect->vel = vel;
                effect->scale = ((Rand_ZeroOne() - 0.5f) * 0.02f) + 0.12f;
                break;
            }
        }
        effect++;
    }
}

void EnZo_Splash(EnZo* self, Vec3f* pos, Vec3f* vel, f32 scale) {
    EnZoEffect* effect;
    Vec3f accel = { 0.0f, -1.0f, 0.0f };
    s16 i;

    effect = self->effects;
    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (1) {}
        if (effect->type != ENZO_EFFECT_SPLASH) {
            effect->type = ENZO_EFFECT_SPLASH;
            effect->pos = *pos;
            effect->vec = accel;
            effect->vel = *vel;
            effect->color.a = (Rand_ZeroOne() * 100.0f) + 100.0f;
            effect->scale = scale;
            break;
        }
        effect++;
    }
}

void EnZo_UpdateRipples(EnZo* self) {
    EnZoEffect* effect = self->effects;
    s16 i;

    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (effect->type == ENZO_EFFECT_RIPPLE) {
            Math_ApproachF(&effect->scale, effect->targetScale, 0.2f, 0.8f);
            if (effect->color.a > 20) {
                effect->color.a -= 20;
            } else {
                effect->color.a = 0;
            }

            if (effect->color.a == 0) {
                effect->type = ENZO_EFFECT_NONE;
            }
        }
        effect++;
    }
}

void EnZo_UpdateBubbles(EnZo* self) {
    EnZoEffect* effect;
    f32 waterSurface;
    s16 i;

    effect = self->effects;
    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (effect->type == ENZO_EFFECT_BUBBLE) {
            effect->pos.x = ((Rand_ZeroOne() * 0.5f) - 0.25f) + effect->vec.x;
            effect->pos.z = ((Rand_ZeroOne() * 0.5f) - 0.25f) + effect->vec.z;
            effect->pos.y += effect->vel.y;

            // Bubbles turn into ripples when they reach the surface
            waterSurface = self->actor.world.pos.y + self->actor.yDistToWater;
            if (waterSurface <= effect->pos.y) {
                effect->type = ENZO_EFFECT_NONE;
                effect->pos.y = waterSurface;
                EnZo_Ripple(self, &effect->pos, 0.06f, 0.12f, 200);
            }
        }
        effect++;
    }
}

void EnZo_UpdateSplashes(EnZo* self) {
    EnZoEffect* effect;
    f32 waterSurface;
    s16 i;

    effect = self->effects;
    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (effect->type == ENZO_EFFECT_SPLASH) {
            effect->pos.x += effect->vel.x;
            effect->pos.y += effect->vel.y;
            effect->pos.z += effect->vel.z;

            if (effect->vel.y >= -20.0f) {
                effect->vel.y += effect->vec.y;
            } else {
                effect->vel.y = -20.0f;
                effect->vec.y = 0.0f;
            }

            // Splash particles turn into ripples when they hit the surface
            waterSurface = self->actor.world.pos.y + self->actor.yDistToWater;
            if (effect->pos.y < waterSurface) {
                effect->type = ENZO_EFFECT_NONE;
                effect->pos.y = waterSurface;
                EnZo_Ripple(self, &effect->pos, 0.06f, 0.12f, 200);
            }
        }
        effect++;
    }
}

void EnZo_DrawRipples(EnZo* self, GlobalContext* globalCtx) {
    EnZoEffect* effect;
    s16 i;
    u8 setup;

    effect = self->effects;
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 217);
    setup = false;
    func_80093D84(globalCtx->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (effect->type == ENZO_EFFECT_RIPPLE) {
            if (!setup) {
                if (1) {}
                gDPPipeSync(POLY_XLU_DISP++);
                gSPDisplayList(POLY_XLU_DISP++, gZoraRipplesVisualDL);
                gDPSetEnvColor(POLY_XLU_DISP++, 155, 155, 155, 0);
                setup = true;
            }

            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, effect->color.a);
            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            Matrix_Scale(effect->scale, 1.0f, effect->scale, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 242),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gZoraRipplesGeometryDL);
        }
        effect++;
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 248);
}

void EnZo_DrawBubbles(EnZo* self, GlobalContext* globalCtx) {
    EnZoEffect* effect = self->effects;
    s16 i;
    u8 setup;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 260);
    setup = false;
    func_80093D84(globalCtx->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (effect->type == ENZO_EFFECT_BUBBLE) {
            if (!setup) {
                if (1) {}
                gSPDisplayList(POLY_XLU_DISP++, gZoraBubblesVisualDL);
                gDPPipeSync(POLY_XLU_DISP++);
                gDPSetEnvColor(POLY_XLU_DISP++, 150, 150, 150, 0);
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);

                setup = true;
            }

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(effect->scale, effect->scale, 1.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 281),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gZoraBubblesGeometryDL);
        }
        effect++;
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 286);
}

void EnZo_DrawSplashes(EnZo* self, GlobalContext* globalCtx) {
    EnZoEffect* effect;
    s16 i;
    u8 setup;

    effect = self->effects;
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 298);
    setup = false;
    func_80093D84(globalCtx->state.gfxCtx);
    for (i = 0; i < ARRAY_COUNT(self->effects); i++) {
        if (effect->type == ENZO_EFFECT_SPLASH) {
            if (!setup) {
                if (1) {}
                gSPDisplayList(POLY_XLU_DISP++, gZoraSplashesVisualDL);
                gDPPipeSync(POLY_XLU_DISP++);
                gDPSetEnvColor(POLY_XLU_DISP++, 200, 200, 200, 0);
                setup = true;
            }
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 180, 180, 180, effect->color.a);

            Matrix_Translate(effect->pos.x, effect->pos.y, effect->pos.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(effect->scale, effect->scale, 1.0f, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 325),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            gSPDisplayList(POLY_XLU_DISP++, gZoraSplashesGeometryDL);
        }
        effect++;
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_zo_eff.c", 331);
}

void EnZo_TreadWaterRipples(EnZo* self, f32 scale, f32 targetScale, u8 alpha) {
    Vec3f pos = { 0.0f, 0.0f, 0.0f };

    pos.x = self->actor.world.pos.x;
    pos.y = self->actor.world.pos.y + self->actor.yDistToWater;
    pos.z = self->actor.world.pos.z;
    EnZo_Ripple(self, &pos, scale, targetScale, alpha);
}

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
    { 26, 64, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

const ActorInit En_Zo_InitVars = {
    ACTOR_EN_ZO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_ZO,
    sizeof(EnZo),
    (ActorFunc)EnZo_Init,
    (ActorFunc)EnZo_Destroy,
    (ActorFunc)EnZo_Update,
    (ActorFunc)EnZo_Draw,
};

static struct_80034EC0_Entry sAnimations[] = {
    { &gZoraIdleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    { &gZoraIdleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    { &gZoraSurfaceAnim, 0.0f, 1.0f, 1.0f, ANIMMODE_ONCE, 0.0f },
    { &gZoraSurfaceAnim, 1.0f, 1.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    { &gZoraSurfaceAnim, 1.0f, 8.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    { &gZoraThrowRupeesAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    { &gZoraHandsOnHipsTappingFootAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    { &gZoraOpenArmsAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
};

void EnZo_SpawnSplashes(EnZo* self) {
    Vec3f pos;
    Vec3f vel;
    s32 i;

    // Convert 20 particles into splashes (all of them since there are only 15)
    for (i = 0; i < 20; i++) {
        f32 speed = Rand_ZeroOne() * 1.5f + 0.5f;
        f32 angle = Rand_ZeroOne() * 6.28f; // ~pi * 2

        vel.y = Rand_ZeroOne() * 3.0f + 3.0f;

        vel.x = sinf(angle) * speed;
        vel.z = cosf(angle) * speed;

        pos = self->actor.world.pos;
        pos.x += vel.x * 6.0f;
        pos.z += vel.z * 6.0f;
        pos.y += self->actor.yDistToWater;
        EnZo_Splash(self, &pos, &vel, 0.08f);
    }
}

u16 func_80B61024(GlobalContext* globalCtx, Actor* thisx) {
    u16 textId;

    textId = Text_GetFaceReaction(globalCtx, 29);
    if (textId != 0) {
        return textId;
    }

    switch (thisx->params & 0x3F) {
        case 8:
            if (gSaveContext.eventChkInf[3] & 1) {
                return 0x402A;
            }
            break;

        case 6:
            return 0x4020;

        case 7:
            return 0x4021;

        case 0:
            if (CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
                return 0x402D;
            }
            if (gSaveContext.eventChkInf[3] & 1) {
                return 0x4007;
            }
            break;

        case 1:
            if (CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
                return 0x402E;
            }

            if (gSaveContext.eventChkInf[3] & 1) {
                return (gSaveContext.infTable[18] & 0x10) ? 0x4009 : 0x4008;
            }
            break;

        case 2:
            if (CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
                return 0x402D;
            }
            if (gSaveContext.eventChkInf[3] & 2) {
                return (gSaveContext.infTable[18] & 0x200) ? 0x400B : 0x402F;
            }
            if (gSaveContext.eventChkInf[3] & 1) {
                return 0x400A;
            }
            break;

        case 3:
            if (CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
                return 0x402E;
            }
            if (gSaveContext.eventChkInf[3] & 1) {
                return 0x400C;
            }
            break;

        case 4:
            if (CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
                return 0x402D;
            }

            if (gSaveContext.eventChkInf[3] & 8) {
                return 0x4010;
            }
            if (gSaveContext.eventChkInf[3] & 1) {
                return 0x400F;
            }
            break;

        case 5:
            if (CHECK_QUEST_ITEM(QUEST_ZORA_SAPPHIRE)) {
                return 0x402E;
            }
            if (gSaveContext.eventChkInf[3] & 1) {
                return 0x4011;
            }
            break;
    }
    return 0x4006;
}

s16 func_80B61298(GlobalContext* globalCtx, Actor* thisx) {
    switch (func_8010BDBC(&globalCtx->msgCtx)) {
        case 0:
        case 1:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
            return 1;

        case 2:
            switch (thisx->textId) {
                case 0x4020:
                case 0x4021:
                    return 0;
                case 0x4008:
                    gSaveContext.infTable[18] |= 0x10;
                    break;
                case 0x402F:
                    gSaveContext.infTable[18] |= 0x200;
                    break;
            }
            gSaveContext.eventChkInf[3] |= 1;
            return 0;

        case 4:
            switch (func_80106BC8(globalCtx)) {
                case 0:
                    return 1;
                default:
                    if (thisx->textId == 0x400C) {
                        thisx->textId = (globalCtx->msgCtx.choiceIndex == 0) ? 0x400D : 0x400E;
                        func_8010B720(globalCtx, thisx->textId);
                    }
                    break;
            }
            return 1;

        case 5:
            switch (func_80106BC8(globalCtx)) {
                case 0:
                    return 1;
                default:
                    return 2;
            }
    }

    return 1;
}

void EnZo_Blink(EnZo* self) {
    if (DECR(self->blinkTimer) == 0) {
        self->eyeTexture++;
        if (self->eyeTexture >= 3) {
            self->blinkTimer = Rand_S16Offset(30, 30);
            self->eyeTexture = 0;
        }
    }
}

void EnZo_Dialog(EnZo* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    self->unk_194.unk_18 = player->actor.world.pos;
    if (self->actionFunc == EnZo_Standing) {
        // Look down at link if young, look up if old
        self->unk_194.unk_14 = !LINK_IS_ADULT ? 10.0f : -10.0f;
    } else {
        self->unk_194.unk_18.y = self->actor.world.pos.y;
    }
    func_80034A14(&self->actor, &self->unk_194, 11, self->unk_64C);
    if (self->canSpeak == true) {
        func_800343CC(globalCtx, &self->actor, &self->unk_194.unk_00, self->dialogRadius, func_80B61024, func_80B61298);
    }
}

s32 EnZo_PlayerInProximity(EnZo* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f surfacePos;
    f32 yDist;
    f32 hDist;

    surfacePos.x = self->actor.world.pos.x;
    surfacePos.y = self->actor.world.pos.y + self->actor.yDistToWater;
    surfacePos.z = self->actor.world.pos.z;

    hDist = Math_Vec3f_DistXZ(&surfacePos, &player->actor.world.pos);
    yDist = fabsf(player->actor.world.pos.y - surfacePos.y);

    if (hDist < 240.0f && yDist < 80.0f) {
        return 1;
    }
    return 0;
}

void EnZo_SetAnimation(EnZo* self) {
    s32 animId = 8;

    if (self->skelAnime.animation == &gZoraHandsOnHipsTappingFootAnim ||
        self->skelAnime.animation == &gZoraOpenArmsAnim) {
        if (self->unk_194.unk_00 == 0) {
            if (self->actionFunc == EnZo_Standing) {
                animId = 0;
            } else {
                animId = 3;
            }
        }
    }

    if (self->unk_194.unk_00 != 0 && self->actor.textId == 0x4006 &&
        self->skelAnime.animation != &gZoraHandsOnHipsTappingFootAnim) {
        animId = 6;
    }

    if (self->unk_194.unk_00 != 0 && self->actor.textId == 0x4007 && self->skelAnime.animation != &gZoraOpenArmsAnim) {
        animId = 7;
    }

    if (animId != 8) {
        func_80034EC0(&self->skelAnime, sAnimations, animId);
        if (animId == 3) {
            self->skelAnime.curFrame = self->skelAnime.endFrame;
            self->skelAnime.playSpeed = 0.0f;
        }
    }
}

void EnZo_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnZo* self = THIS;

    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gZoraSkel, NULL, self->jointTable, self->morphTable, 20);
    Collider_InitCylinder(globalCtx, &self->collider);
    Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &sColChkInit);

    if (LINK_IS_ADULT && ((self->actor.params & 0x3F) == 8)) {
        Actor_Kill(&self->actor);
        return;
    }

    func_80034EC0(&self->skelAnime, sAnimations, 2);
    Actor_SetScale(&self->actor, 0.01f);
    self->actor.targetMode = 6;
    self->dialogRadius = self->collider.dim.radius + 30.0f;
    self->unk_64C = 1;
    self->canSpeak = false;
    self->unk_194.unk_00 = 0;
    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, self->collider.dim.height * 0.5f, self->collider.dim.radius, 0.0f,
                            5);

    if (self->actor.yDistToWater < 54.0f || (self->actor.params & 0x3F) == 8) {
        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        self->actor.shape.shadowScale = 24.0f;
        func_80034EC0(&self->skelAnime, sAnimations, 1);
        self->canSpeak = true;
        self->alpha = 255.0f;
        self->actionFunc = EnZo_Standing;
    } else {
        self->actor.flags &= ~1;
        self->actionFunc = EnZo_Submerged;
    }
}

void EnZo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnZo_Standing(EnZo* self, GlobalContext* globalCtx) {
    s16 angle;

    func_80034F54(globalCtx, self->unk_656, self->unk_67E, 20);
    EnZo_SetAnimation(self);
    if (self->unk_194.unk_00 != 0) {
        self->unk_64C = 4;
        return;
    }

    angle = ABS((s16)((f32)self->actor.yawTowardsPlayer - (f32)self->actor.shape.rot.y));
    if (angle < 0x4718) {
        if (EnZo_PlayerInProximity(self, globalCtx)) {
            self->unk_64C = 2;
        } else {
            self->unk_64C = 1;
        }
    } else {
        self->unk_64C = 1;
    }
}

void EnZo_Submerged(EnZo* self, GlobalContext* globalCtx) {
    if (EnZo_PlayerInProximity(self, globalCtx)) {
        self->actionFunc = EnZo_Surface;
        self->actor.velocity.y = 4.0f;
    }
}

void EnZo_Surface(EnZo* self, GlobalContext* globalCtx) {
    if (self->actor.yDistToWater < 54.0f) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_OUT_OF_WATER);
        EnZo_SpawnSplashes(self);
        func_80034EC0(&self->skelAnime, sAnimations, 3);
        self->actor.flags |= 1;
        self->actionFunc = EnZo_TreadWater;
        self->actor.velocity.y = 0.0f;
        self->alpha = 255.0f;
    } else if (self->actor.yDistToWater < 80.0f) {
        Math_ApproachF(&self->actor.velocity.y, 2.0f, 0.4f, 0.6f);
        Math_ApproachF(&self->alpha, 255.0f, 0.3f, 10.0f);
    }
}

void EnZo_TreadWater(EnZo* self, GlobalContext* globalCtx) {
    func_80034F54(globalCtx, self->unk_656, self->unk_67E, 20);
    if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
        self->canSpeak = true;
        self->unk_64C = 4;
        self->skelAnime.playSpeed = 0.0f;
    }
    EnZo_SetAnimation(self);

    Math_ApproachF(&self->actor.velocity.y, self->actor.yDistToWater < 54.0f ? -0.6f : 0.6f, 0.3f, 0.2f);
    if (self->rippleTimer != 0) {
        self->rippleTimer--;
        if ((self->rippleTimer == 3) || (self->rippleTimer == 6)) {
            EnZo_TreadWaterRipples(self, 0.2f, 1.0f, 200);
        }
    } else {
        EnZo_TreadWaterRipples(self, 0.2f, 1.0f, 200);
        self->rippleTimer = 12;
    }

    if (EnZo_PlayerInProximity(self, globalCtx) != 0) {
        self->timeToDive = Rand_S16Offset(40, 40);
    } else if (DECR(self->timeToDive) == 0) {
        f32 startFrame;
        func_80034EC0(&self->skelAnime, sAnimations, 4);
        self->canSpeak = false;
        self->unk_64C = 1;
        self->actionFunc = EnZo_Dive;
        startFrame = self->skelAnime.startFrame;
        self->skelAnime.startFrame = self->skelAnime.endFrame;
        self->skelAnime.curFrame = self->skelAnime.endFrame;
        self->skelAnime.endFrame = startFrame;
        self->skelAnime.playSpeed = -1.0f;
    }
}

void EnZo_Dive(EnZo* self, GlobalContext* globalCtx) {
    if (Animation_OnFrame(&self->skelAnime, self->skelAnime.endFrame)) {
        Audio_PlayActorSound2(&self->actor, NA_SE_EV_DIVE_WATER);
        EnZo_SpawnSplashes(self);
        self->actor.flags &= ~1;
        self->actor.velocity.y = -4.0f;
        self->skelAnime.playSpeed = 0.0f;
    }

    if (self->skelAnime.playSpeed > 0.0f) {
        return;
    }

    if (self->actor.yDistToWater > 80.0f || self->actor.bgCheckFlags & 1) {
        Math_ApproachF(&self->actor.velocity.y, -1.0f, 0.4f, 0.6f);
        Math_ApproachF(&self->alpha, 0.0f, 0.3f, 10.0f);
    }

    if ((s16)self->alpha == 0) {
        func_80034EC0(&self->skelAnime, sAnimations, 2);
        self->actor.world.pos = self->actor.home.pos;
        self->alpha = 0.0f;
        self->actionFunc = EnZo_Submerged;
    }
}

void EnZo_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnZo* self = THIS;
    u32 pad;
    Vec3f pos;

    if ((s32)self->alpha != 0) {
        SkelAnime_Update(&self->skelAnime);
        EnZo_Blink(self);
    }

    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(globalCtx, thisx, self->collider.dim.radius, self->collider.dim.height * 0.25f, 0.0f, 5);
    self->actionFunc(self, globalCtx);
    EnZo_Dialog(self, globalCtx);

    // Spawn air bubbles
    if (globalCtx->state.frames & 8) {
        pos = self->actor.world.pos;

        pos.y += (Rand_ZeroOne() - 0.5f) * 10.0f + 18.0f;
        pos.x += (Rand_ZeroOne() - 0.5f) * 28.0f;
        pos.z += (Rand_ZeroOne() - 0.5f) * 28.0f;
        EnZo_Bubble(self, &pos);
    }

    if ((s32)self->alpha != 0) {
        Collider_UpdateCylinder(thisx, &self->collider);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    EnZo_UpdateRipples(self);
    EnZo_UpdateBubbles(self);
    EnZo_UpdateSplashes(self);
}

s32 EnZo_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                          Gfx** gfx) {
    EnZo* self = THIS;
    Vec3s vec;

    if (limbIndex == 15) {
        Matrix_Translate(1800.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        vec = self->unk_194.unk_08;
        Matrix_RotateX((vec.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((vec.x / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_Translate(-1800.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }

    if (limbIndex == 8) {
        vec = self->unk_194.unk_0E;
        Matrix_RotateX((-vec.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((vec.x / 32768.0f) * M_PI, MTXMODE_APPLY);
    }

    if ((limbIndex == 8) || (limbIndex == 9) || (limbIndex == 12)) {
        rot->y += (Math_SinS(self->unk_656[limbIndex]) * 200.0f);
        rot->z += (Math_CosS(self->unk_67E[limbIndex]) * 200.0f);
    }

    return 0;
}

void EnZo_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnZo* self = THIS;
    Vec3f vec = { 0.0f, 600.0f, 0.0f };

    if (limbIndex == 15) {
        Matrix_MultVec3f(&vec, &self->actor.focus.pos);
    }
}

void EnZo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnZo* self = THIS;
    void* eyeTextures[] = { gZoraEyeOpenTex, gZoraEyeHalfTex, gZoraEyeClosedTex };

    Matrix_Push();
    EnZo_DrawRipples(self, globalCtx);
    EnZo_DrawBubbles(self, globalCtx);
    EnZo_DrawSplashes(self, globalCtx);
    Matrix_Pop();

    if ((s32)self->alpha != 0) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_zo.c", 1008);

        if (self->alpha == 255.0f) {
            gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTexture]));
            func_80034BA0(globalCtx, &self->skelAnime, EnZo_OverrideLimbDraw, EnZo_PostLimbDraw, thisx, self->alpha);
        } else {
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTexture]));
            func_80034CC4(globalCtx, &self->skelAnime, EnZo_OverrideLimbDraw, EnZo_PostLimbDraw, thisx, self->alpha);
        }

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_zo.c", 1025);
    }
}
