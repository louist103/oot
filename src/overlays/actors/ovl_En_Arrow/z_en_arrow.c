/*
 * File: z_en_arrow.c
 * Overlay: ovl_En_Arrow
 * Description: Arrow, Deku Seed, and Deku Nut Projectile
 */

#include "z_en_arrow.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0x00000030

#define THIS ((EnArrow*)thisx)

void EnArrow_Init(Actor* thisx, GlobalContext* globalCtx);
void EnArrow_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnArrow_Update(Actor* thisx, GlobalContext* globalCtx);
void EnArrow_Draw(Actor* thisx, GlobalContext* globalCtx);

void EnArrow_Shoot(EnArrow* self, GlobalContext* globalCtx);
void EnArrow_Fly(EnArrow* self, GlobalContext* globalCtx);
void func_809B45E0(EnArrow* self, GlobalContext* globalCtx);
void func_809B4640(EnArrow* self, GlobalContext* globalCtx);

const ActorInit En_Arrow_InitVars = {
    ACTOR_EN_ARROW,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnArrow),
    (ActorFunc)EnArrow_Init,
    (ActorFunc)EnArrow_Destroy,
    (ActorFunc)EnArrow_Update,
    (ActorFunc)EnArrow_Draw,
};

static ColliderQuadInit sColliderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLSHAPE_QUAD,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000020, 0x00, 0x01 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_NEAREST | TOUCH_SFX_NONE,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(minVelocityY, -150, ICHAIN_STOP),
};

void EnArrow_SetupAction(EnArrow* self, EnArrowActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void EnArrow_Init(Actor* thisx, GlobalContext* globalCtx) {
    static EffectBlureInit2 blureNormal = {
        0, 4, 0, { 0, 255, 200, 255 },   { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 255, 255, 170, 255 }, { 0, 150, 0, 0 },
    };
    static EffectBlureInit2 blureFire = {
        0, 4, 0, { 0, 255, 200, 255 }, { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 255, 200, 0, 255 }, { 255, 0, 0, 0 },
    };
    static EffectBlureInit2 blureIce = {
        0, 4, 0, { 0, 255, 200, 255 },   { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 170, 255, 255, 255 }, { 0, 100, 255, 0 },
    };
    static EffectBlureInit2 blureLight = {
        0, 4, 0, { 0, 255, 200, 255 },   { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 255, 255, 170, 255 }, { 255, 255, 0, 0 },
    };
    static u32 dmgFlags[] = {
        0x00000800, 0x00000020, 0x00000020, 0x00000800, 0x00001000,
        0x00002000, 0x00010000, 0x00004000, 0x00008000, 0x00000004,
    };
    EnArrow* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);

    if (self->actor.params == ARROW_CS_NUT) {
        self->isCsNut = true;
        self->actor.params = ARROW_NUT;
    }

    if (self->actor.params <= ARROW_SEED) {

        if (self->actor.params <= ARROW_0E) {
            SkelAnime_Init(globalCtx, &self->skelAnime, &gArrowSkel, &gArrow2Anim, NULL, NULL, 0);
        }

        if (self->actor.params <= ARROW_NORMAL) {
            if (self->actor.params == ARROW_NORMAL_HORSE) {
                blureNormal.elemDuration = 4;
            } else {
                blureNormal.elemDuration = 16;
            }

            Effect_Add(globalCtx, &self->effectIndex, EFFECT_BLURE2, 0, 0, &blureNormal);

        } else if (self->actor.params == ARROW_FIRE) {

            Effect_Add(globalCtx, &self->effectIndex, EFFECT_BLURE2, 0, 0, &blureFire);

        } else if (self->actor.params == ARROW_ICE) {

            Effect_Add(globalCtx, &self->effectIndex, EFFECT_BLURE2, 0, 0, &blureIce);

        } else if (self->actor.params == ARROW_LIGHT) {

            Effect_Add(globalCtx, &self->effectIndex, EFFECT_BLURE2, 0, 0, &blureLight);
        }

        Collider_InitQuad(globalCtx, &self->collider);
        Collider_SetQuad(globalCtx, &self->collider, &self->actor, &sColliderInit);

        if (self->actor.params <= ARROW_NORMAL) {
            self->collider.info.toucherFlags &= ~0x18;
            self->collider.info.toucherFlags |= 0;
        }

        if (self->actor.params < 0) {
            self->collider.base.atFlags = (AT_ON | AT_TYPE_ENEMY);
        } else if (self->actor.params <= ARROW_SEED) {
            self->collider.info.toucher.dmgFlags = dmgFlags[self->actor.params];
            LOG_HEX("self->at_info.cl_elem.at_btl_info.at_type", self->collider.info.toucher.dmgFlags,
                    "../z_en_arrow.c", 707);
        }
    }

    EnArrow_SetupAction(self, EnArrow_Shoot);
}

void EnArrow_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnArrow* self = THIS;

    if (self->actor.params <= ARROW_LIGHT) {
        Effect_Delete(globalCtx, self->effectIndex);
    }

    SkelAnime_Free(&self->skelAnime, globalCtx);
    Collider_DestroyQuad(globalCtx, &self->collider);

    if ((self->hitActor != NULL) && (self->hitActor->update != NULL)) {
        self->hitActor->flags &= ~0x8000;
    }
}

void EnArrow_Shoot(EnArrow* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if (self->actor.parent == NULL) {
        if ((self->actor.params != ARROW_NUT) && (player->unk_A73 == 0)) {
            Actor_Kill(&self->actor);
            return;
        }

        switch (self->actor.params) {
            case ARROW_SEED:
                func_8002F7DC(&player->actor, NA_SE_IT_SLING_SHOT);
                break;

            case ARROW_NORMAL_LIT:
            case ARROW_NORMAL_HORSE:
            case ARROW_NORMAL:
                func_8002F7DC(&player->actor, NA_SE_IT_ARROW_SHOT);
                break;

            case ARROW_FIRE:
            case ARROW_ICE:
            case ARROW_LIGHT:
                func_8002F7DC(&player->actor, NA_SE_IT_MAGIC_ARROW_SHOT);
                break;
        }

        EnArrow_SetupAction(self, EnArrow_Fly);
        Math_Vec3f_Copy(&self->unk_210, &self->actor.world.pos);

        if (self->actor.params >= ARROW_SEED) {
            func_8002D9A4(&self->actor, 80.0f);
            self->timer = 15;
            self->actor.shape.rot.x = self->actor.shape.rot.y = self->actor.shape.rot.z = 0;
        } else {
            func_8002D9A4(&self->actor, 150.0f);
            self->timer = 12;
        }
    }
}

void func_809B3CEC(GlobalContext* globalCtx, EnArrow* self) {
    EnArrow_SetupAction(self, func_809B4640);
    Animation_PlayOnce(&self->skelAnime, &gArrow1Anim);
    self->actor.world.rot.y += (s32)(24576.0f * (Rand_ZeroOne() - 0.5f)) + 0x8000;
    self->actor.velocity.y += (self->actor.speedXZ * (0.4f + (0.4f * Rand_ZeroOne())));
    self->actor.speedXZ *= (0.04f + 0.3f * Rand_ZeroOne());
    self->timer = 50;
    self->actor.gravity = -1.5f;
}

void EnArrow_CarryActor(EnArrow* self, GlobalContext* globalCtx) {
    CollisionPoly* hitPoly;
    Vec3f posDiffLastFrame;
    Vec3f actorNextPos;
    Vec3f hitPos;
    f32 temp_f12;
    f32 scale;
    s32 bgId;

    Math_Vec3f_Diff(&self->actor.world.pos, &self->unk_210, &posDiffLastFrame);

    temp_f12 = ((self->actor.world.pos.x - self->hitActor->world.pos.x) * posDiffLastFrame.x) +
               ((self->actor.world.pos.y - self->hitActor->world.pos.y) * posDiffLastFrame.y) +
               ((self->actor.world.pos.z - self->hitActor->world.pos.z) * posDiffLastFrame.z);

    if (!(temp_f12 < 0.0f)) {
        scale = Math3D_Vec3fMagnitudeSq(&posDiffLastFrame);

        if (!(scale < 1.0f)) {
            scale = temp_f12 / scale;
            Math_Vec3f_Scale(&posDiffLastFrame, scale);
            Math_Vec3f_Sum(&self->hitActor->world.pos, &posDiffLastFrame, &actorNextPos);

            if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &self->hitActor->world.pos, &actorNextPos, &hitPos,
                                        &hitPoly, true, true, true, true, &bgId)) {
                self->hitActor->world.pos.x = hitPos.x + ((actorNextPos.x <= hitPos.x) ? 1.0f : -1.0f);
                self->hitActor->world.pos.y = hitPos.y + ((actorNextPos.y <= hitPos.y) ? 1.0f : -1.0f);
                self->hitActor->world.pos.z = hitPos.z + ((actorNextPos.z <= hitPos.z) ? 1.0f : -1.0f);
            } else {
                Math_Vec3f_Copy(&self->hitActor->world.pos, &actorNextPos);
            }
        }
    }
}

void EnArrow_Fly(EnArrow* self, GlobalContext* globalCtx) {
    CollisionPoly* hitPoly;
    s32 bgId;
    Vec3f hitPoint;
    Vec3f posCopy;
    s32 atTouched;
    u16 sfxId;
    Actor* hitActor;
    Vec3f sp60;
    Vec3f sp54;

    if (DECR(self->timer) == 0) {
        Actor_Kill(&self->actor);
        return;
    }

    if (self->timer < 7.2000003f) {
        self->actor.gravity = -0.4f;
    }

    atTouched = (self->actor.params != ARROW_NORMAL_LIT) && (self->actor.params <= ARROW_SEED) &&
                (self->collider.base.atFlags & AT_HIT);

    if (atTouched || self->touchedPoly) {
        if (self->actor.params >= ARROW_SEED) {
            if (atTouched) {
                self->actor.world.pos.x = (self->actor.world.pos.x + self->actor.prevPos.x) * 0.5f;
                self->actor.world.pos.y = (self->actor.world.pos.y + self->actor.prevPos.y) * 0.5f;
                self->actor.world.pos.z = (self->actor.world.pos.z + self->actor.prevPos.z) * 0.5f;
            }

            if (self->actor.params == ARROW_NUT) {
                iREG(50) = -1;
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_M_FIRE1, self->actor.world.pos.x,
                            self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
                sfxId = NA_SE_IT_DEKU;
            } else {
                sfxId = NA_SE_IT_SLING_REFLECT;
            }

            EffectSsStone1_Spawn(globalCtx, &self->actor.world.pos, 0);
            Audio_PlaySoundAtPosition(globalCtx, &self->actor.world.pos, 20, sfxId);
            Actor_Kill(&self->actor);
        } else {
            EffectSsHitMark_SpawnCustomScale(globalCtx, 0, 150, &self->actor.world.pos);

            if (atTouched && (self->collider.info.atHitInfo->elemType != ELEMTYPE_UNK4)) {
                hitActor = self->collider.base.at;

                if ((hitActor->update != NULL) && (!(self->collider.base.atFlags & AT_BOUNCED)) &&
                    (hitActor->flags & 0x4000)) {
                    self->hitActor = hitActor;
                    EnArrow_CarryActor(self, globalCtx);
                    Math_Vec3f_Diff(&hitActor->world.pos, &self->actor.world.pos, &self->unk_250);
                    hitActor->flags |= 0x8000;
                    self->collider.base.atFlags &= ~AT_HIT;
                    self->actor.speedXZ /= 2.0f;
                    self->actor.velocity.y /= 2.0f;
                } else {
                    self->hitFlags |= 1;
                    self->hitFlags |= 2;

                    if (self->collider.info.atHitInfo->bumperFlags & 2) {
                        self->actor.world.pos.x = self->collider.info.atHitInfo->bumper.hitPos.x;
                        self->actor.world.pos.y = self->collider.info.atHitInfo->bumper.hitPos.y;
                        self->actor.world.pos.z = self->collider.info.atHitInfo->bumper.hitPos.z;
                    }

                    func_809B3CEC(globalCtx, self);
                    Audio_PlayActorSound2(&self->actor, NA_SE_IT_ARROW_STICK_CRE);
                }
            } else if (self->touchedPoly) {
                EnArrow_SetupAction(self, func_809B45E0);
                Animation_PlayOnce(&self->skelAnime, &gArrow2Anim);

                if (self->actor.params >= ARROW_NORMAL_LIT) {
                    self->timer = 60;
                } else {
                    self->timer = 20;
                }

                Audio_PlayActorSound2(&self->actor, NA_SE_IT_ARROW_STICK_OBJ);
                self->hitFlags |= 1;
            }
        }
    } else {
        Math_Vec3f_Copy(&self->unk_210, &self->actor.world.pos);
        Actor_MoveForward(&self->actor);

        if ((self->touchedPoly =
                 BgCheck_ProjectileLineTest(&globalCtx->colCtx, &self->actor.prevPos, &self->actor.world.pos, &hitPoint,
                                            &self->actor.wallPoly, true, true, true, true, &bgId))) {
            func_8002F9EC(globalCtx, &self->actor, self->actor.wallPoly, bgId, &hitPoint);
            Math_Vec3f_Copy(&posCopy, &self->actor.world.pos);
            Math_Vec3f_Copy(&self->actor.world.pos, &hitPoint);
        }

        if (self->actor.params <= ARROW_0E) {
            self->actor.shape.rot.x = Math_Atan2S(self->actor.speedXZ, -self->actor.velocity.y);
        }
    }

    if (self->hitActor != NULL) {
        if (self->hitActor->update != NULL) {
            Math_Vec3f_Sum(&self->unk_210, &self->unk_250, &sp60);
            Math_Vec3f_Sum(&self->actor.world.pos, &self->unk_250, &sp54);

            if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &sp60, &sp54, &hitPoint, &hitPoly, true, true, true, true,
                                        &bgId)) {
                self->hitActor->world.pos.x = hitPoint.x + ((sp54.x <= hitPoint.x) ? 1.0f : -1.0f);
                self->hitActor->world.pos.y = hitPoint.y + ((sp54.y <= hitPoint.y) ? 1.0f : -1.0f);
                self->hitActor->world.pos.z = hitPoint.z + ((sp54.z <= hitPoint.z) ? 1.0f : -1.0f);
                Math_Vec3f_Diff(&self->hitActor->world.pos, &self->actor.world.pos, &self->unk_250);
                self->hitActor->flags &= ~0x8000;
                self->hitActor = NULL;
            } else {
                Math_Vec3f_Sum(&self->actor.world.pos, &self->unk_250, &self->hitActor->world.pos);
            }

            if (self->touchedPoly && (self->hitActor != NULL)) {
                self->hitActor->flags &= ~0x8000;
                self->hitActor = NULL;
            }
        } else {
            self->hitActor = NULL;
        }
    }
}

void func_809B45E0(EnArrow* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);

    if (DECR(self->timer) == 0) {
        Actor_Kill(&self->actor);
    }
}

void func_809B4640(EnArrow* self, GlobalContext* globalCtx) {
    SkelAnime_Update(&self->skelAnime);
    Actor_MoveForward(&self->actor);

    if (DECR(self->timer) == 0) {
        Actor_Kill(&self->actor);
    }
}

void EnArrow_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnArrow* self = THIS;
    Player* player = GET_PLAYER(globalCtx);

    if (self->isCsNut || ((self->actor.params >= ARROW_NORMAL_LIT) && (player->unk_A73 != 0)) ||
        !Player_InBlockingCsMode(globalCtx, player)) {
        self->actionFunc(self, globalCtx);
    }

    if ((self->actor.params >= ARROW_FIRE) && (self->actor.params <= ARROW_0E)) {
        s16 elementalActorIds[] = { ACTOR_ARROW_FIRE, ACTOR_ARROW_ICE,  ACTOR_ARROW_LIGHT,
                                    ACTOR_ARROW_FIRE, ACTOR_ARROW_FIRE, ACTOR_ARROW_FIRE };

        if (self->actor.child == NULL) {
            Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, elementalActorIds[self->actor.params - 3],
                               self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, 0, 0, 0, 0);
        }
    } else if (self->actor.params == ARROW_NORMAL_LIT) {
        static Vec3f velocity = { 0.0f, 0.5f, 0.0f };
        static Vec3f accel = { 0.0f, 0.5f, 0.0f };
        static Color_RGBA8 primColor = { 255, 255, 100, 255 };
        static Color_RGBA8 envColor = { 255, 50, 0, 0 };
        // spawn dust for the flame
        func_8002836C(globalCtx, &self->unk_21C, &velocity, &accel, &primColor, &envColor, 100, 0, 8);
    }
}

void func_809B4800(EnArrow* self, GlobalContext* globalCtx) {
    static Vec3f D_809B4E88 = { 0.0f, 400.0f, 1500.0f };
    static Vec3f D_809B4E94 = { 0.0f, -400.0f, 1500.0f };
    static Vec3f D_809B4EA0 = { 0.0f, 0.0f, -300.0f };
    Vec3f sp44;
    Vec3f sp38;
    s32 addBlureVertex;

    Matrix_MultVec3f(&D_809B4EA0, &self->unk_21C);

    if (EnArrow_Fly == self->actionFunc) {
        Matrix_MultVec3f(&D_809B4E88, &sp44);
        Matrix_MultVec3f(&D_809B4E94, &sp38);

        if (self->actor.params <= ARROW_SEED) {
            addBlureVertex = self->actor.params <= ARROW_LIGHT;

            if (self->hitActor == NULL) {
                addBlureVertex &= func_80090480(globalCtx, &self->collider, &self->weaponInfo, &sp44, &sp38);
            } else {
                if (addBlureVertex) {
                    if ((sp44.x == self->weaponInfo.tip.x) && (sp44.y == self->weaponInfo.tip.y) &&
                        (sp44.z == self->weaponInfo.tip.z) && (sp38.x == self->weaponInfo.base.x) &&
                        (sp38.y == self->weaponInfo.base.y) && (sp38.z == self->weaponInfo.base.z)) {
                        addBlureVertex = false;
                    }
                }
            }

            if (addBlureVertex) {
                EffectBlure_AddVertex(Effect_GetByIndex(self->effectIndex), &sp44, &sp38);
            }
        }
    }
}

void EnArrow_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnArrow* self = THIS;
    u8 alpha;
    f32 scale;

    if (self->actor.params <= ARROW_0E) {
        func_80093D18(globalCtx->state.gfxCtx);
        SkelAnime_DrawLod(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, NULL, NULL, self,
                          (self->actor.projectedPos.z < MREG(95)) ? 0 : 1);
    } else if (self->actor.speedXZ != 0.0f) {
        alpha = (Math_CosS(self->timer * 5000) * 127.5f) + 127.5f;

        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_arrow.c", 1346);

        func_80093C14(globalCtx->state.gfxCtx);

        if (self->actor.params == ARROW_SEED) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 255, alpha);
            scale = 50.0f;
        } else {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 12, 0, 0, 255);
            gDPSetEnvColor(POLY_XLU_DISP++, 250, 250, 0, alpha);
            scale = 150.0f;
        }

        Matrix_Push();
        Matrix_Mult(&globalCtx->mf_11DA0, MTXMODE_APPLY);
        // redundant check because self is contained in an if block for non-zero speed
        Matrix_RotateZ((self->actor.speedXZ == 0.0f) ? 0.0f
                                                     : ((globalCtx->gameplayFrames & 0xFF) * 4000) * (M_PI / 0x8000),
                       MTXMODE_APPLY);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_arrow.c", 1374),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, gEffSparklesDL);
        Matrix_Pop();
        Matrix_RotateY(self->actor.world.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_arrow.c", 1381);
    }

    func_809B4800(self, globalCtx);
}
