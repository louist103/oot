/*
 * File: z_obj_dekujr.c
 * Overlay: ovl_Obj_Dekujr
 * Description: Deku Tree Sprout
 */

#include "z_obj_dekujr.h"
#include "objects/object_dekujr/object_dekujr.h"

#define FLAGS 0x00000009

#define THIS ((ObjDekujr*)thisx)

void ObjDekujr_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjDekujr_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjDekujr_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjDekujr_Draw(Actor* thisx, GlobalContext* globalCtx);

void ObjDekujr_ComeUp(ObjDekujr* self, GlobalContext* globalCtx);

const ActorInit Obj_Dekujr_InitVars = {
    ACTOR_OBJ_DEKUJR,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_DEKUJR,
    sizeof(ObjDekujr),
    (ActorFunc)ObjDekujr_Init,
    (ActorFunc)ObjDekujr_Destroy,
    (ActorFunc)ObjDekujr_Update,
    (ActorFunc)ObjDekujr_Draw,
};

static ColliderCylinderInitToActor sCylinderInit = {
    {
        NULL,
        0x00,
        0x00,
        0x39,
        COLSHAPE_CYLINDER,
    },
    { 0x02, { 0x00000000, 0x00, 0x00 }, { 0xFFCFFFFF, 0x00, 0x00 }, 0x00, 0x00, 0x01 },
    { 60, 80, 0, { 0, 0, 0 } },
};

void ObjDekujr_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjDekujr* self = THIS;
    s32 pad;

    if (gSaveContext.cutsceneIndex < 0xFFF0) {
        if (!LINK_IS_ADULT) {
            Actor_Kill(thisx);
            return;
        }
        self->unk_19C = 2;
        self->unk_19B = 0;
    } else {
        self->unk_19C = 0;
        self->unk_19B = 1;
    }
    if (!CHECK_QUEST_ITEM(QUEST_MEDALLION_FOREST)) {
        Actor_Kill(thisx);
    } else {
        ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
        Collider_InitCylinder(globalCtx, &self->collider);
        sCylinderInit.base.actor = thisx;
        Collider_SetCylinderToActor(globalCtx, &self->collider, &sCylinderInit);
        thisx->colChkInfo.mass = MASS_IMMOVABLE;
        thisx->textId = func_80037C30(globalCtx, 0xF);
        Actor_SetScale(thisx, 0.4f);
    }
}

void ObjDekujr_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void ObjDekujr_SetInitialPos(CsCmdActorAction* npcAction, Vec3f* initPos) {
    initPos->x = npcAction->startPos.x;
    initPos->y = npcAction->startPos.y;
    initPos->z = npcAction->startPos.z;
}

void ObjDekujr_SetFinalPos(CsCmdActorAction* npcAction, Vec3f* finalPos) {
    finalPos->x = npcAction->endPos.x;
    finalPos->y = npcAction->endPos.y;
    finalPos->z = npcAction->endPos.z;
}

void ObjDekujr_ComeUp(ObjDekujr* self, GlobalContext* globalCtx) {
    CsCmdActorAction* csCmdNPCAction;
    Vec3f initPos;
    Vec3f finalPos;
    Vec3f velocity = { 0.0f, 0.0f, 0.0f };
    f32 actionLength;
    f32 gravity;

    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        self->unk_19C = 2;
        self->unk_19B = 0;
    } else {
        if (globalCtx->csCtx.frames == 351) {
            Audio_PlayActorSound2(&self->actor, NA_SE_EV_COME_UP_DEKU_JR);
        }
        csCmdNPCAction = globalCtx->csCtx.npcActions[1];
        if (csCmdNPCAction != NULL) {
            ObjDekujr_SetInitialPos(csCmdNPCAction, &initPos);
            ObjDekujr_SetFinalPos(csCmdNPCAction, &finalPos);
            if (self->unk_19C == 0) {
                self->actor.world.pos = initPos;
                self->unk_19C = 1;
            }
            self->actor.shape.rot.x = csCmdNPCAction->urot.x;
            self->actor.shape.rot.y = csCmdNPCAction->urot.y;
            self->actor.shape.rot.z = csCmdNPCAction->urot.z;
            self->actor.velocity = velocity;
            if (csCmdNPCAction->endFrame >= globalCtx->csCtx.frames) {
                actionLength = csCmdNPCAction->endFrame - csCmdNPCAction->startFrame;
                self->actor.velocity.x = (finalPos.x - initPos.x) / actionLength;
                gravity = self->actor.gravity;
                self->actor.velocity.y = (finalPos.y - initPos.y) / actionLength;
                self->actor.velocity.y += gravity;
                if (self->actor.velocity.y < self->actor.minVelocityY) {
                    self->actor.velocity.y = self->actor.minVelocityY;
                }
                self->actor.velocity.z = (finalPos.z - initPos.z) / actionLength;
            }
        }
    }
}

void ObjDekujr_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjDekujr* self = THIS;
    s32 pad;

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    if ((gSaveContext.cutsceneIndex >= 0xFFF0) && (self->unk_19B == 0)) {
        self->unk_19C = 0;
        self->unk_19B = 1;
    }
    if (self->unk_19B == 1) {
        ObjDekujr_ComeUp(self, globalCtx);
        self->actor.world.pos.x += self->actor.velocity.x;
        self->actor.world.pos.y += self->actor.velocity.y;
        self->actor.world.pos.z += self->actor.velocity.z;
    } else {
        func_80037D98(globalCtx, &self->actor, 0xF, &self->unk_1A0);
        Actor_SetFocus(&self->actor, 40.0f);
    }
}

void ObjDekujr_Draw(Actor* thisx, GlobalContext* globalCtx) {
    u32 frameCount;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_dekujr.c", 370);

    func_80093D18(globalCtx->state.gfxCtx);
    func_80093D84(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_dekujr.c", 379),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_dekujr_DL_0030D0);

    frameCount = globalCtx->state.frames;
    gSPSegment(
        POLY_XLU_DISP++, 0x08,
        Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, frameCount % 128, 0, 32, 32, 1, frameCount % 128, 0, 32, 32));
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_dekujr.c", 399),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, object_dekujr_DL_0032D8);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_dekujr.c", 409);
}
