/*
 * File: z_bg_ydan_maruta.c
 * Overlay: ovl_Bg_Ydan_Maruta
 * Description: Rotating spike log and falling ladder in Deku Tree
 */

#include "z_bg_ydan_maruta.h"
#include "objects/object_ydan_objects/object_ydan_objects.h"

#define FLAGS 0x00000000

#define THIS ((BgYdanMaruta*)thisx)

void BgYdanMaruta_Init(Actor* thisx, GlobalContext* globalCtx);
void BgYdanMaruta_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgYdanMaruta_Update(Actor* thisx, GlobalContext* globalCtx);
void BgYdanMaruta_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808BEFF4(BgYdanMaruta* self, GlobalContext* globalCtx);
void BgYdanMaruta_DoNothing(BgYdanMaruta* self, GlobalContext* globalCtx);
void func_808BF078(BgYdanMaruta* self, GlobalContext* globalCtx);
void func_808BF108(BgYdanMaruta* self, GlobalContext* globalCtx);
void func_808BF1EC(BgYdanMaruta* self, GlobalContext* globalCtx);

const ActorInit Bg_Ydan_Maruta_InitVars = {
    ACTOR_BG_YDAN_MARUTA,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_YDAN_OBJECTS,
    sizeof(BgYdanMaruta),
    (ActorFunc)BgYdanMaruta_Init,
    (ActorFunc)BgYdanMaruta_Destroy,
    (ActorFunc)BgYdanMaruta_Update,
    (ActorFunc)BgYdanMaruta_Draw,
};

static ColliderTrisElementInit sTrisElementsInit[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0x20000000, 0x00, 0x04 },
            { 0x00000004, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_WOOD,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 220.0f, -10.0f, 0.0f }, { 220.0f, 10.0f, 0.0f }, { -220.0f, 10.0f, 0.0f } } },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x20000000, 0x00, 0x04 },
            { 0x00000004, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_WOOD,
            BUMP_ON,
            OCELEM_NONE,
        },
        { { { 16.0f, 0.0f, 0.0f }, { 16.0f, 135.0f, 0.0f }, { -16.0f, 135.0f, 0.0f } } },
    },
};

static ColliderTrisInit sTrisInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_TRIS,
    },
    2,
    sTrisElementsInit,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgYdanMaruta_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgYdanMaruta* self = THIS;
    Vec3f sp4C[3];
    s32 i;
    f32 sinRotY;
    f32 cosRotY;
    CollisionHeader* colHeader = NULL;
    ColliderTrisElementInit* triInit;

    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    Collider_InitTris(globalCtx, &self->collider);
    Collider_SetTris(globalCtx, &self->collider, &self->dyna.actor, &sTrisInit, self->elements);

    self->switchFlag = self->dyna.actor.params & 0xFFFF;
    thisx->params = (thisx->params >> 8) & 0xFF; // thisx is required to match here

    if (self->dyna.actor.params == 0) {
        triInit = &sTrisElementsInit[0];
        self->actionFunc = func_808BEFF4;
    } else {
        triInit = &sTrisElementsInit[1];
        DynaPolyActor_Init(&self->dyna, DPM_UNK);
        CollisionHeader_GetVirtual(&gDTFallingLadderCol, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);
        thisx->home.pos.y += -280.0f;
        if (Flags_GetSwitch(globalCtx, self->switchFlag)) {
            thisx->world.pos.y = thisx->home.pos.y;
            self->actionFunc = BgYdanMaruta_DoNothing;
        } else {
            self->actionFunc = func_808BF078;
        }
    }

    sinRotY = Math_SinS(self->dyna.actor.shape.rot.y);
    cosRotY = Math_CosS(self->dyna.actor.shape.rot.y);

    for (i = 0; i < 3; i++) {
        sp4C[i].x = (triInit->dim.vtx[i].x * cosRotY) + self->dyna.actor.world.pos.x;
        sp4C[i].y = triInit->dim.vtx[i].y + self->dyna.actor.world.pos.y;
        sp4C[i].z = self->dyna.actor.world.pos.z - (triInit->dim.vtx[i].x * sinRotY);
    }

    Collider_SetTrisVertices(&self->collider, 0, &sp4C[0], &sp4C[1], &sp4C[2]);

    sp4C[1].x = (triInit->dim.vtx[2].x * cosRotY) + self->dyna.actor.world.pos.x;
    sp4C[1].y = triInit->dim.vtx[0].y + self->dyna.actor.world.pos.y;
    sp4C[1].z = self->dyna.actor.world.pos.z - (triInit->dim.vtx[2].x * sinRotY);

    Collider_SetTrisVertices(&self->collider, 1, &sp4C[0], &sp4C[2], &sp4C[1]);
}

void BgYdanMaruta_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanMaruta* self = THIS;

    Collider_DestroyTris(globalCtx, &self->collider);
    if (self->dyna.actor.params == 1) {
        DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, self->dyna.bgId);
    }
}

void func_808BEFF4(BgYdanMaruta* self, GlobalContext* globalCtx) {
    if (self->collider.base.atFlags & AT_HIT) {
        func_8002F71C(globalCtx, &self->dyna.actor, 7.0f, self->dyna.actor.shape.rot.y, 6.0f);
    }
    self->dyna.actor.shape.rot.x += 0x360;
    CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    func_8002F974(&self->dyna.actor, NA_SE_EV_TOGE_STICK_ROLLING - SFX_FLAG);
}

void func_808BF078(BgYdanMaruta* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->unk_16A = 20;
        Flags_SetSwitch(globalCtx, self->switchFlag);
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        self->actionFunc = func_808BF108;
        OnePointCutscene_Init(globalCtx, 3010, 50, &self->dyna.actor, MAIN_CAM);
    } else {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }
}

void func_808BF108(BgYdanMaruta* self, GlobalContext* globalCtx) {
    s16 temp;

    if (self->unk_16A != 0) {
        self->unk_16A--;
    }
    if (self->unk_16A == 0) {
        self->actionFunc = func_808BF1EC;
    }

    if (1) {}

    temp = (self->unk_16A % 4) - 2;
    if (temp == -2) {
        temp = 0;
    } else {
        temp *= 2;
    }

    self->dyna.actor.world.pos.x = (Math_CosS(self->dyna.actor.shape.rot.y) * temp) + self->dyna.actor.home.pos.x;
    self->dyna.actor.world.pos.z = (Math_SinS(self->dyna.actor.shape.rot.y) * temp) + self->dyna.actor.home.pos.z;

    func_8002F974(&self->dyna.actor, NA_SE_EV_TRAP_OBJ_SLIDE - SFX_FLAG);
}

void func_808BF1EC(BgYdanMaruta* self, GlobalContext* globalCtx) {
    self->dyna.actor.velocity.y += 1.0f;
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, self->dyna.actor.velocity.y)) {
        Audio_PlayActorSound2(&self->dyna.actor, NA_SE_EV_LADDER_DOUND);
        self->actionFunc = BgYdanMaruta_DoNothing;
    }
}

void BgYdanMaruta_DoNothing(BgYdanMaruta* self, GlobalContext* globalCtx) {
}

void BgYdanMaruta_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanMaruta* self = THIS;

    self->actionFunc(self, globalCtx);
}

void BgYdanMaruta_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgYdanMaruta* self = THIS;

    if (self->dyna.actor.params == 0) {
        Gfx_DrawDListOpa(globalCtx, gDTRollingSpikeTrapDL);
    } else {
        Gfx_DrawDListOpa(globalCtx, gDTFallingLadderDL);
    }
}
