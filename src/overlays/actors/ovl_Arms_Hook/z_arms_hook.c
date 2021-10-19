#include "z_arms_hook.h"

#define FLAGS 0x00000030

#define THIS ((ArmsHook*)thisx)

void ArmsHook_Init(Actor* thisx, GlobalContext* globalCtx);
void ArmsHook_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ArmsHook_Update(Actor* thisx, GlobalContext* globalCtx);
void ArmsHook_Draw(Actor* thisx, GlobalContext* globalCtx);

void ArmsHook_Wait(ArmsHook* self, GlobalContext* globalCtx);
void ArmsHook_Shoot(ArmsHook* self, GlobalContext* globalCtx);

const ActorInit Arms_Hook_InitVars = {
    ACTOR_ARMS_HOOK,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_LINK_BOY,
    sizeof(ArmsHook),
    (ActorFunc)ArmsHook_Init,
    (ActorFunc)ArmsHook_Destroy,
    (ActorFunc)ArmsHook_Update,
    (ActorFunc)ArmsHook_Draw,
};

static ColliderQuadInit sQuadInit = {
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
        { 0x00000080, 0x00, 0x01 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_NEAREST | TOUCH_SFX_NORMAL,
        BUMP_NONE,
        OCELEM_NONE,
    },
    { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } } },
};

static Vec3f sUnusedVec1 = { 0.0f, 0.5f, 0.0f };
static Vec3f sUnusedVec2 = { 0.0f, 0.5f, 0.0f };

static Color_RGB8 sUnusedColors[] = {
    { 255, 255, 100 },
    { 255, 255, 50 },
};

static Vec3f D_80865B70 = { 0.0f, 0.0f, 0.0f };
static Vec3f D_80865B7C = { 0.0f, 0.0f, 900.0f };
static Vec3f D_80865B88 = { 0.0f, 500.0f, -3000.0f };
static Vec3f D_80865B94 = { 0.0f, -500.0f, -3000.0f };
static Vec3f D_80865BA0 = { 0.0f, 500.0f, 1200.0f };
static Vec3f D_80865BAC = { 0.0f, -500.0f, 1200.0f };

extern Gfx D_0602B288[];
extern Gfx D_0602AFF0[];

void ArmsHook_SetupAction(ArmsHook* self, ArmsHookActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void ArmsHook_Init(Actor* thisx, GlobalContext* globalCtx) {
    ArmsHook* self = THIS;

    Collider_InitQuad(globalCtx, &self->collider);
    Collider_SetQuad(globalCtx, &self->collider, &self->actor, &sQuadInit);
    ArmsHook_SetupAction(self, ArmsHook_Wait);
    self->unk_1E8 = self->actor.world.pos;
}

void ArmsHook_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ArmsHook* self = THIS;

    if (self->grabbed != NULL) {
        self->grabbed->flags &= ~0x2000;
    }
    Collider_DestroyQuad(globalCtx, &self->collider);
}

void ArmsHook_Wait(ArmsHook* self, GlobalContext* globalCtx) {
    if (self->actor.parent == NULL) {
        Player* player = GET_PLAYER(globalCtx);
        // get correct timer length for hookshot or longshot
        s32 length = (player->heldItemActionParam == PLAYER_AP_HOOKSHOT) ? 13 : 26;

        ArmsHook_SetupAction(self, ArmsHook_Shoot);
        func_8002D9A4(&self->actor, 20.0f);
        self->actor.parent = &GET_PLAYER(globalCtx)->actor;
        self->timer = length;
    }
}

void func_80865044(ArmsHook* self) {
    self->actor.child = self->actor.parent;
    self->actor.parent->parent = &self->actor;
}

s32 ArmsHook_AttachToPlayer(ArmsHook* self, Player* player) {
    player->actor.child = &self->actor;
    player->heldActor = &self->actor;
    if (self->actor.child != NULL) {
        player->actor.parent = NULL;
        self->actor.child = NULL;
        return true;
    }
    return false;
}

void ArmsHook_DetachHookFromActor(ArmsHook* self) {
    if (self->grabbed != NULL) {
        self->grabbed->flags &= ~0x2000;
        self->grabbed = NULL;
    }
}

s32 ArmsHook_CheckForCancel(ArmsHook* self) {
    Player* player = (Player*)self->actor.parent;

    if (Player_HoldsHookshot(player)) {
        if ((player->itemActionParam != player->heldItemActionParam) || ((player->actor.flags & 0x100)) ||
            ((player->stateFlags1 & 0x4000080))) {
            self->timer = 0;
            ArmsHook_DetachHookFromActor(self);
            Math_Vec3f_Copy(&self->actor.world.pos, &player->unk_3C8);
            return 1;
        }
    }
    return 0;
}

void ArmsHook_AttachHookToActor(ArmsHook* self, Actor* actor) {
    actor->flags |= 0x2000;
    self->grabbed = actor;
    Math_Vec3f_Diff(&actor->world.pos, &self->actor.world.pos, &self->grabbedDistDiff);
}

void ArmsHook_Shoot(ArmsHook* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    Actor* touchedActor;
    Actor* grabbed;
    Vec3f bodyDistDiffVec;
    Vec3f newPos;
    f32 bodyDistDiff;
    f32 phi_f16;
    DynaPolyActor* dynaPolyActor;
    f32 sp94;
    f32 sp90;
    s32 pad;
    CollisionPoly* poly;
    s32 bgId;
    Vec3f sp78;
    Vec3f prevFrameDiff;
    Vec3f sp60;
    f32 sp5C;
    f32 sp58;
    f32 velocity;
    s32 pad1;

    if ((self->actor.parent == NULL) || (!Player_HoldsHookshot(player))) {
        ArmsHook_DetachHookFromActor(self);
        Actor_Kill(&self->actor);
        return;
    }

    func_8002F8F0(&player->actor, NA_SE_IT_HOOKSHOT_CHAIN - SFX_FLAG);
    ArmsHook_CheckForCancel(self);

    if ((self->timer != 0) && (self->collider.base.atFlags & AT_HIT) &&
        (self->collider.info.atHitInfo->elemType != ELEMTYPE_UNK4)) {
        touchedActor = self->collider.base.at;
        if ((touchedActor->update != NULL) && (touchedActor->flags & 0x600)) {
            if (self->collider.info.atHitInfo->bumperFlags & BUMP_HOOKABLE) {
                ArmsHook_AttachHookToActor(self, touchedActor);
                if ((touchedActor->flags & 0x400) == 0x400) {
                    func_80865044(self);
                }
            }
        }
        self->timer = 0;
        Audio_PlaySoundGeneral(NA_SE_IT_ARROW_STICK_CRE, &self->actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    } else if (DECR(self->timer) == 0) {
        grabbed = self->grabbed;
        if (grabbed != NULL) {
            if ((grabbed->update == NULL) || (grabbed->flags & 0x2000) != 0x2000) {
                grabbed = NULL;
                self->grabbed = NULL;
            } else if (self->actor.child != NULL) {
                sp94 = Actor_WorldDistXYZToActor(&self->actor, grabbed);
                sp90 = sqrtf(SQ(self->grabbedDistDiff.x) + SQ(self->grabbedDistDiff.y) + SQ(self->grabbedDistDiff.z));
                Math_Vec3f_Diff(&grabbed->world.pos, &self->grabbedDistDiff, &self->actor.world.pos);
                if (50.0f < (sp94 - sp90)) {
                    ArmsHook_DetachHookFromActor(self);
                    grabbed = NULL;
                }
            }
        }

        bodyDistDiff = Math_Vec3f_DistXYZAndStoreDiff(&player->unk_3C8, &self->actor.world.pos, &bodyDistDiffVec);
        if (bodyDistDiff < 30.0f) {
            velocity = 0.0f;
            phi_f16 = 0.0f;
        } else {
            if (self->actor.child != NULL) {
                velocity = 30.0f;
            } else if (grabbed != NULL) {
                velocity = 50.0f;
            } else {
                velocity = 200.0f;
            }
            phi_f16 = bodyDistDiff - velocity;
            if (bodyDistDiff <= velocity) {
                phi_f16 = 0.0f;
            }
            velocity = phi_f16 / bodyDistDiff;
        }

        newPos.x = bodyDistDiffVec.x * velocity;
        newPos.y = bodyDistDiffVec.y * velocity;
        newPos.z = bodyDistDiffVec.z * velocity;

        if (self->actor.child == NULL) {
            if ((grabbed != NULL) && (grabbed->id == ACTOR_BG_SPOT06_OBJECTS)) {
                Math_Vec3f_Diff(&grabbed->world.pos, &self->grabbedDistDiff, &self->actor.world.pos);
                phi_f16 = 1.0f;
            } else {
                Math_Vec3f_Sum(&player->unk_3C8, &newPos, &self->actor.world.pos);
                if (grabbed != NULL) {
                    Math_Vec3f_Sum(&self->actor.world.pos, &self->grabbedDistDiff, &grabbed->world.pos);
                }
            }
        } else {
            Math_Vec3f_Diff(&bodyDistDiffVec, &newPos, &player->actor.velocity);
            player->actor.world.rot.x =
                Math_Atan2S(sqrtf(SQ(bodyDistDiffVec.x) + SQ(bodyDistDiffVec.z)), -bodyDistDiffVec.y);
        }

        if (phi_f16 < 50.0f) {
            ArmsHook_DetachHookFromActor(self);
            if (phi_f16 == 0.0f) {
                ArmsHook_SetupAction(self, ArmsHook_Wait);
                if (ArmsHook_AttachToPlayer(self, player)) {
                    Math_Vec3f_Diff(&self->actor.world.pos, &player->actor.world.pos, &player->actor.velocity);
                    player->actor.velocity.y -= 20.0f;
                }
            }
        }
    } else {
        Actor_MoveForward(&self->actor);
        Math_Vec3f_Diff(&self->actor.world.pos, &self->actor.prevPos, &prevFrameDiff);
        Math_Vec3f_Sum(&self->unk_1E8, &prevFrameDiff, &self->unk_1E8);
        self->actor.shape.rot.x = Math_Atan2S(self->actor.speedXZ, -self->actor.velocity.y);
        sp60.x = self->unk_1F4.x - (self->unk_1E8.x - self->unk_1F4.x);
        sp60.y = self->unk_1F4.y - (self->unk_1E8.y - self->unk_1F4.y);
        sp60.z = self->unk_1F4.z - (self->unk_1E8.z - self->unk_1F4.z);
        if (BgCheck_EntityLineTest1(&globalCtx->colCtx, &sp60, &self->unk_1E8, &sp78, &poly, true, true, true, true,
                                    &bgId) &&
            !func_8002F9EC(globalCtx, &self->actor, poly, bgId, &sp78)) {
            sp5C = COLPOLY_GET_NORMAL(poly->normal.x);
            sp58 = COLPOLY_GET_NORMAL(poly->normal.z);
            Math_Vec3f_Copy(&self->actor.world.pos, &sp78);
            self->actor.world.pos.x += 10.0f * sp5C;
            self->actor.world.pos.z += 10.0f * sp58;
            self->timer = 0;
            if (SurfaceType_IsHookshotSurface(&globalCtx->colCtx, poly, bgId)) {
                if (bgId != BGCHECK_SCENE) {
                    dynaPolyActor = DynaPoly_GetActor(&globalCtx->colCtx, bgId);
                    if (dynaPolyActor != NULL) {
                        ArmsHook_AttachHookToActor(self, &dynaPolyActor->actor);
                    }
                }
                func_80865044(self);
                Audio_PlaySoundGeneral(NA_SE_IT_HOOKSHOT_STICK_OBJ, &self->actor.projectedPos, 4, &D_801333E0,
                                       &D_801333E0, &D_801333E8);
            } else {
                CollisionCheck_SpawnShieldParticlesMetal(globalCtx, &self->actor.world.pos);
                Audio_PlaySoundGeneral(NA_SE_IT_HOOKSHOT_REFLECT, &self->actor.projectedPos, 4, &D_801333E0,
                                       &D_801333E0, &D_801333E8);
            }
        } else if (CHECK_BTN_ANY(globalCtx->state.input[0].press.button,
                                 (BTN_A | BTN_B | BTN_R | BTN_CUP | BTN_CLEFT | BTN_CRIGHT | BTN_CDOWN))) {
            self->timer = 0;
        }
    }
}

void ArmsHook_Update(Actor* thisx, GlobalContext* globalCtx) {
    ArmsHook* self = THIS;

    self->actionFunc(self, globalCtx);
    self->unk_1F4 = self->unk_1E8;
}

void ArmsHook_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    ArmsHook* self = THIS;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f sp78;
    Vec3f sp6C;
    Vec3f sp60;
    f32 sp5C;
    f32 sp58;

    if ((player->actor.draw != NULL) && (player->rightHandType == 15)) {
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_arms_hook.c", 850);

        if ((ArmsHook_Shoot != self->actionFunc) || (self->timer <= 0)) {
            Matrix_MultVec3f(&D_80865B70, &self->unk_1E8);
            Matrix_MultVec3f(&D_80865B88, &sp6C);
            Matrix_MultVec3f(&D_80865B94, &sp60);
            self->hookInfo.active = 0;
        } else {
            Matrix_MultVec3f(&D_80865B7C, &self->unk_1E8);
            Matrix_MultVec3f(&D_80865BA0, &sp6C);
            Matrix_MultVec3f(&D_80865BAC, &sp60);
        }

        func_80090480(globalCtx, &self->collider, &self->hookInfo, &sp6C, &sp60);
        func_80093D18(globalCtx->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_arms_hook.c", 895),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, D_0602B288);
        Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
        Math_Vec3f_Diff(&player->unk_3C8, &self->actor.world.pos, &sp78);
        sp58 = SQ(sp78.x) + SQ(sp78.z);
        sp5C = sqrtf(sp58);
        Matrix_RotateY(Math_FAtan2F(sp78.x, sp78.z), MTXMODE_APPLY);
        Matrix_RotateX(Math_FAtan2F(-sp78.y, sp5C), MTXMODE_APPLY);
        Matrix_Scale(0.015f, 0.015f, sqrtf(SQ(sp78.y) + sp58) * 0.01f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_arms_hook.c", 910),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, D_0602AFF0);

        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_arms_hook.c", 913);
    }
}
