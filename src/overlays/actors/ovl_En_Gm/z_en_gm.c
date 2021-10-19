/*
 * File: z_en_gm.c
 * Overlay: ovl_En_Gm
 * Description: Medi-Goron
 */

#include "z_en_gm.h"
#include "objects/object_oF1d_map/object_oF1d_map.h"
#include "objects/object_gm/object_gm.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnGm*)thisx)

void EnGm_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGm_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGm_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGm_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A3D838(EnGm* self, GlobalContext* globalCtx);
void func_80A3DFBC(EnGm* self, GlobalContext* globalCtx);
void func_80A3DB04(EnGm* self, GlobalContext* globalCtx);
void func_80A3DC44(EnGm* self, GlobalContext* globalCtx);
void func_80A3DBF4(EnGm* self, GlobalContext* globalCtx);
void func_80A3DD7C(EnGm* self, GlobalContext* globalCtx);
void EnGm_ProcessChoiceIndex(EnGm* self, GlobalContext* globalCtx);
void func_80A3DF00(EnGm* self, GlobalContext* globalCtx);
void func_80A3DF60(EnGm* self, GlobalContext* globalCtx);

const ActorInit En_Gm_InitVars = {
    ACTOR_EN_GM,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_OF1D_MAP,
    sizeof(EnGm),
    (ActorFunc)EnGm_Init,
    (ActorFunc)EnGm_Destroy,
    (ActorFunc)EnGm_Update,
    NULL,
};

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
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
    { 100, 120, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 5, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 30, ICHAIN_STOP),
};

void EnGm_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGm* self = THIS;

    Actor_ProcessInitChain(&self->actor, sInitChain);

    // "Medi Goron"
    osSyncPrintf(VT_FGCOL(GREEN) "%s[%d] : 中ゴロン[%d]" VT_RST "\n", "../z_en_gm.c", 133, self->actor.params);

    self->objGmBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_GM);

    if (self->objGmBankIndex < 0) {
        osSyncPrintf(VT_COL(RED, WHITE));
        // "There is no model bank! !! (Medi Goron)"
        osSyncPrintf("モデル バンクが無いよ！！（中ゴロン）\n");
        osSyncPrintf(VT_RST);
        ASSERT(0, "0", "../z_en_gm.c", 145);
    }

    self->updateFunc = func_80A3D838;
}

void EnGm_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGm* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

s32 func_80A3D7C8(void) {
    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        return 0;
    } else if (!(gBitFlags[2] & gSaveContext.inventory.equipment)) {
        return 1;
    } else if (gBitFlags[3] & gSaveContext.inventory.equipment) {
        return 2;
    } else {
        return 3;
    }
}

void func_80A3D838(EnGm* self, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, self->objGmBankIndex)) {
        self->actor.flags &= ~0x10;
        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &gGoronSkel, NULL, self->jointTable, self->morphTable, 18);
        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->objGmBankIndex].segment);
        Animation_Change(&self->skelAnime, &object_gm_Anim_0002B8, 1.0f, 0.0f,
                         Animation_GetLastFrame(&object_gm_Anim_0002B8), ANIMMODE_LOOP, 0.0f);
        self->actor.draw = EnGm_Draw;
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        ActorShape_Init(&self->actor.shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
        Actor_SetScale(&self->actor, 0.05f);
        self->actor.colChkInfo.mass = MASS_IMMOVABLE;
        self->eyeTexIndex = 0;
        self->blinkTimer = 20;
        self->actor.textId = 0x3049;
        self->updateFunc = func_80A3DFBC;
        self->actionFunc = func_80A3DB04;
        self->actor.speedXZ = 0.0f;
        self->actor.gravity = -1.0f;
        self->actor.velocity.y = 0.0f;
    }
}

void EnGm_UpdateEye(EnGm* self) {
    if (self->blinkTimer != 0) {
        self->blinkTimer--;
    } else {
        self->eyeTexIndex++;

        if (self->eyeTexIndex >= 3) {
            self->eyeTexIndex = 0;
            self->blinkTimer = Rand_ZeroFloat(60.0f) + 20.0f;
        }
    }
}

void EnGm_SetTextID(EnGm* self) {
    switch (func_80A3D7C8()) {
        case 0:
            if (gSaveContext.infTable[11] & 1) {
                self->actor.textId = 0x304B;
            } else {
                self->actor.textId = 0x304A;
            }
            break;
        case 1:
            if (gSaveContext.infTable[11] & 2) {
                self->actor.textId = 0x304F;
            } else {
                self->actor.textId = 0x304C;
            }
            break;
        case 2:
            self->actor.textId = 0x304E;
            break;
        case 3:
            self->actor.textId = 0x304D;
            break;
    }
}

void func_80A3DB04(EnGm* self, GlobalContext* globalCtx) {
    f32 dx;
    f32 dz;
    Player* player = GET_PLAYER(globalCtx);

    dx = self->talkPos.x - player->actor.world.pos.x;
    dz = self->talkPos.z - player->actor.world.pos.z;

    if (Flags_GetSwitch(globalCtx, self->actor.params)) {
        EnGm_SetTextID(self);
        self->actionFunc = func_80A3DC44;
    } else if (func_8002F194(&self->actor, globalCtx)) {
        self->actionFunc = func_80A3DBF4;
    } else if ((self->collider.base.ocFlags1 & OC1_HIT) || (SQ(dx) + SQ(dz)) < SQ(100.0f)) {
        self->collider.base.acFlags &= ~AC_HIT;
        func_8002F2CC(&self->actor, globalCtx, 415.0f);
    }
}

void func_80A3DBF4(EnGm* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && func_80106BC8(globalCtx)) {
        self->actionFunc = func_80A3DB04;
    }
}

void func_80A3DC44(EnGm* self, GlobalContext* globalCtx) {
    f32 dx;
    f32 dz;
    s32 pad;
    Player* player = GET_PLAYER(globalCtx);

    EnGm_SetTextID(self);

    dx = self->talkPos.x - player->actor.world.pos.x;
    dz = self->talkPos.z - player->actor.world.pos.z;

    if (func_8002F194(&self->actor, globalCtx)) {
        switch (func_80A3D7C8()) {
            case 0:
                gSaveContext.infTable[11] |= 1;
            case 3:
                self->actionFunc = func_80A3DD7C;
                return;
            case 1:
                gSaveContext.infTable[11] |= 2;
            case 2:
                self->actionFunc = EnGm_ProcessChoiceIndex;
            default:
                return;
        }

        self->actionFunc = EnGm_ProcessChoiceIndex;
    }
    if ((self->collider.base.ocFlags1 & OC1_HIT) || (SQ(dx) + SQ(dz)) < SQ(100.0f)) {
        self->collider.base.acFlags &= ~AC_HIT;
        func_8002F2CC(&self->actor, globalCtx, 415.0f);
    }
}

void func_80A3DD7C(EnGm* self, GlobalContext* globalCtx) {
    u8 dialogState = func_8010BDBC(&globalCtx->msgCtx);

    if ((dialogState == 6 || dialogState == 5) && func_80106BC8(globalCtx)) {
        self->actionFunc = func_80A3DC44;
        if (dialogState == 5) {
            globalCtx->msgCtx.msgMode = 0x36;
            globalCtx->msgCtx.unk_E3E7 = 4;
        }
    }
}

void EnGm_ProcessChoiceIndex(EnGm* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 4 && func_80106BC8(globalCtx)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0: // yes
                if (gSaveContext.rupees < 200) {
                    func_8010B720(globalCtx, 0xC8);
                    self->actionFunc = func_80A3DD7C;
                } else {
                    func_8002F434(&self->actor, globalCtx, GI_SWORD_KNIFE, 415.0f, 10.0f);
                    self->actionFunc = func_80A3DF00;
                }
                break;
            case 1: // no
                func_8010B720(globalCtx, 0x3050);
                self->actionFunc = func_80A3DD7C;
                break;
        }
    }
}

void func_80A3DF00(EnGm* self, GlobalContext* globalCtx) {
    if (Actor_HasParent(&self->actor, globalCtx)) {
        self->actor.parent = NULL;
        self->actionFunc = func_80A3DF60;
    } else {
        func_8002F434(&self->actor, globalCtx, GI_SWORD_KNIFE, 415.0f, 10.0f);
    }
}

void func_80A3DF60(EnGm* self, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && func_80106BC8(globalCtx)) {
        Rupees_ChangeBy(-200);
        self->actionFunc = func_80A3DC44;
    }
}

void func_80A3DFBC(EnGm* self, GlobalContext* globalCtx) {
    gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->objGmBankIndex].segment);
    self->timer++;
    self->actionFunc(self, globalCtx);
    self->actor.focus.rot.x = self->actor.world.rot.x;
    self->actor.focus.rot.y = self->actor.world.rot.y;
    self->actor.focus.rot.z = self->actor.world.rot.z;
    EnGm_UpdateEye(self);
    SkelAnime_Update(&self->skelAnime);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

void EnGm_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGm* self = THIS;

    self->updateFunc(self, globalCtx);
}

void func_80A3E090(EnGm* self) {
    Vec3f vec1;
    Vec3f vec2;

    Matrix_Push();
    Matrix_Translate(0.0f, 0.0f, 2600.0f, MTXMODE_APPLY);
    Matrix_RotateRPY(self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, MTXMODE_APPLY);
    vec1.x = vec1.y = vec1.z = 0.0f;
    Matrix_MultVec3f(&vec1, &vec2);
    self->collider.dim.pos.x = vec2.x;
    self->collider.dim.pos.y = vec2.y;
    self->collider.dim.pos.z = vec2.z;
    Matrix_Pop();
    Matrix_Push();
    Matrix_Translate(0.0f, 0.0f, 4300.0f, MTXMODE_APPLY);
    Matrix_RotateRPY(self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, MTXMODE_APPLY);
    vec1.x = vec1.y = vec1.z = 0.0f;
    Matrix_MultVec3f(&vec1, &self->talkPos);
    Matrix_Pop();
    Matrix_Translate(0.0f, 0.0f, 3800.0f, MTXMODE_APPLY);
    Matrix_RotateRPY(self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, MTXMODE_APPLY);
    vec1.x = vec1.y = vec1.z = 0.0f;
    Matrix_MultVec3f(&vec1, &self->actor.focus.pos);
    self->actor.focus.pos.y += 100.0f;
}

void EnGm_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* eyeTextures[] = { gGoronCsEyeOpenTex, gGoronCsEyeHalfTex, gGoronCsEyeClosedTex };
    EnGm* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_gm.c", 613);

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(eyeTextures[self->eyeTexIndex]));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(gGoronCsMouthNeutralTex));
    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          NULL, NULL, &self->actor);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_gm.c", 629);

    func_80A3E090(self);
}
