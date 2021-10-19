/*
 * File: z_en_guest.c
 * Overlay: ovl_En_Guest
 * Description: Happy Mask Shop Customer
 */

#include "z_en_guest.h"
#include "objects/object_os_anime/object_os_anime.h"
#include "objects/object_boj/object_boj.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnGuest*)thisx)

void EnGuest_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGuest_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGuest_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGuest_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A50518(EnGuest* self, GlobalContext* globalCtx);
void func_80A5057C(EnGuest* self, GlobalContext* globalCtx);
void func_80A505CC(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Guest_InitVars = {
    ACTOR_EN_GUEST,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_BOJ,
    sizeof(EnGuest),
    (ActorFunc)EnGuest_Init,
    (ActorFunc)EnGuest_Destroy,
    (ActorFunc)EnGuest_Update,
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
    { 0x00, { 0x00000000, 0x00, 0x00 }, { 0x00000000, 0x00, 0x00 }, 0x00, 0x00, 0x01 },
    { 10, 60, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(targetMode, 6, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 500, ICHAIN_STOP),
};

extern FlexSkeletonHeader object_boj_Skel_0000F0;

void EnGuest_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGuest* self = THIS;

    if (gSaveContext.infTable[7] & 0x40) {
        Actor_Kill(&self->actor);
    } else {
        self->osAnimeBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_OS_ANIME);
        if (self->osAnimeBankIndex < 0) {
            osSyncPrintf(VT_COL(RED, WHITE));
            // "No such bank!!"
            osSyncPrintf("%s[%d] : バンクが無いよ！！\n", "../z_en_guest.c", 129);
            osSyncPrintf(VT_RST);
            ASSERT(0, "0", "../z_en_guest.c", 132);
        }
    }
}

void EnGuest_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGuest* self = THIS;

    Collider_DestroyCylinder(globalCtx, &self->collider);
}

void EnGuest_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGuest* self = THIS;
    s32 pad;

    if (Object_IsLoaded(&globalCtx->objectCtx, self->osAnimeBankIndex) != 0) {
        self->actor.flags &= ~0x10;
        Actor_ProcessInitChain(&self->actor, sInitChain);

        SkelAnime_InitFlex(globalCtx, &self->skelAnime, &object_boj_Skel_0000F0, NULL, self->jointTable,
                           self->morphTable, 16);
        gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->osAnimeBankIndex].segment);
        Animation_Change(&self->skelAnime, &gObjOsAnim_42AC, 1.0f, 0.0f, Animation_GetLastFrame(&gObjOsAnim_42AC),
                         ANIMMODE_LOOP, 0.0f);

        self->actor.draw = EnGuest_Draw;
        self->actor.update = func_80A505CC;

        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinderType1(globalCtx, &self->collider, &self->actor, &sCylinderInit);

        Actor_SetFocus(&self->actor, 60.0f);

        self->unk_30E = 0;
        self->unk_30D = 0;
        self->unk_2CA = 0;
        self->actor.textId = 0x700D;
        self->actionFunc = func_80A50518;
    }
}

void func_80A5046C(EnGuest* self) {
    if (self->unk_30D == 0) {
        if (self->unk_2CA != 0) {
            self->unk_2CA--;
        } else {
            self->unk_30D = 1;
        }
    } else {
        if (self->unk_2CA != 0) {
            self->unk_2CA--;
        } else {
            self->unk_30E += 1;
            if (self->unk_30E >= 3) {
                self->unk_30E = 0;
                self->unk_30D = 0;
                self->unk_2CA = (s32)Rand_ZeroFloat(60.0f) + 20;
            } else {
                self->unk_2CA = 1;
            }
        }
    }
}

void func_80A50518(EnGuest* self, GlobalContext* globalCtx) {
    if (func_8002F194(&self->actor, globalCtx) != 0) {
        self->actionFunc = func_80A5057C;
    } else if (self->actor.xzDistToPlayer < 100.0f) {
        func_8002F2CC(&self->actor, globalCtx, 100.0f);
    }
}

void func_80A5057C(EnGuest* self, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 6) {
        if (func_80106BC8(globalCtx) != 0) {
            self->actionFunc = func_80A50518;
        }
    }
}

void func_80A505CC(Actor* thisx, GlobalContext* globalCtx) {
    EnGuest* self = THIS;
    s32 pad;
    Player* player;

    player = GET_PLAYER(globalCtx);
    self->unk_2C8++;

    func_80A5046C(self);
    self->actionFunc(self, globalCtx);

    self->unk_2A0.unk_18 = player->actor.world.pos;
    if (LINK_IS_ADULT) {
        self->unk_2A0.unk_14 = 10.0f;
    } else {
        self->unk_2A0.unk_14 = 20.0f;
    }
    func_80034A14(&self->actor, &self->unk_2A0, 6, 2);

    func_80034F54(globalCtx, self->unk_2CC, self->unk_2EC, 16);

    gSegments[6] = VIRTUAL_TO_PHYSICAL(globalCtx->objectCtx.status[self->osAnimeBankIndex].segment);

    SkelAnime_Update(&self->skelAnime);
    Actor_SetFocus(&self->actor, 60.0f);

    Collider_UpdateCylinder(&self->actor, &self->collider);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
}

Gfx* func_80A50708(GraphicsContext* gfxCtx, u8 r, u8 g, u8 b, u8 a) {
    Gfx* dlist;

    dlist = Graph_Alloc(gfxCtx, 2 * sizeof(Gfx));
    gDPSetEnvColor(dlist, r, g, b, a);
    gSPEndDisplayList(dlist + 1);

    return dlist;
}

s32 EnGuest_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                             void* thisx) {
    EnGuest* self = THIS;
    Vec3s sp3C;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_guest.c", 352);

    if (limbIndex == 15) {
        *dList = object_boj_DL_0059B0;
        Matrix_Translate(1400.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        sp3C = self->unk_2A0.unk_08;
        Matrix_RotateX((sp3C.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((sp3C.x / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_Translate(-1400.0f, 0.0f, 0.0f, MTXMODE_APPLY);
    }

    if (limbIndex == 8) {
        sp3C = self->unk_2A0.unk_0E;
        Matrix_RotateX((-sp3C.y / 32768.0f) * M_PI, MTXMODE_APPLY);
        Matrix_RotateZ((sp3C.x / 32768.0f) * M_PI, MTXMODE_APPLY);
    }

    if (limbIndex == 8 || limbIndex == 9 || limbIndex == 12) {
        rot->y += Math_SinS(self->unk_2CC[limbIndex]) * 200.0f;
        rot->z += Math_CosS(self->unk_2EC[limbIndex]) * 200.0f;
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_guest.c", 388);

    return false;
}

void EnGuest_Draw(Actor* thisx, GlobalContext* globalCtx) {
    static void* D_80A50BA4[] = {
        0x060005FC,
        0x060006FC,
        0x060007FC,
    };
    EnGuest* self = THIS;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_guest.c", 404);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, func_80A50708(globalCtx->state.gfxCtx, 0xFF, 0xFF, 0xFF, 0xFF));
    gSPSegment(POLY_OPA_DISP++, 0x09, func_80A50708(globalCtx->state.gfxCtx, 0xA0, 0x3C, 0xDC, 0xFF));
    gSPSegment(POLY_OPA_DISP++, 0x0A, SEGMENTED_TO_VIRTUAL(D_80A50BA4[self->unk_30E]));

    SkelAnime_DrawFlexOpa(globalCtx, self->skelAnime.skeleton, self->skelAnime.jointTable, self->skelAnime.dListCount,
                          EnGuest_OverrideLimbDraw, NULL, self);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_guest.c", 421);
}
