/*
 * File: z_obj_makekinsuta.c
 * Overlay: ovl_Obj_Makekinsuta
 * Description: Skulltula Sprouting from Bean Spot
 */

#include "z_obj_makekinsuta.h"
#include "vt.h"

#define FLAGS 0x00000010

#define THIS ((ObjMakekinsuta*)thisx)

void ObjMakekinsuta_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjMakekinsuta_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80B98320(ObjMakekinsuta* self, GlobalContext* globalCtx);
void ObjMakekinsuta_DoNothing(ObjMakekinsuta* self, GlobalContext* globalCtx);

const ActorInit Obj_Makekinsuta_InitVars = {
    ACTOR_OBJ_MAKEKINSUTA,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjMakekinsuta),
    (ActorFunc)ObjMakekinsuta_Init,
    (ActorFunc)Actor_Noop,
    (ActorFunc)ObjMakekinsuta_Update,
    NULL,
};

void ObjMakekinsuta_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjMakekinsuta* self = THIS;

    if ((self->actor.params & 0x6000) == 0x4000) {
        osSyncPrintf(VT_FGCOL(BLUE));
        // "Gold Star Enemy(arg_data %x)"
        osSyncPrintf("金スタ発生敵(arg_data %x)\n", self->actor.params);
        osSyncPrintf(VT_RST);
    } else {
        osSyncPrintf(VT_COL(YELLOW, BLACK));
        // "Invalid Argument (arg_data %x)(%s %d)"
        osSyncPrintf("引数不正 (arg_data %x)(%s %d)\n", self->actor.params, "../z_obj_makekinsuta.c", 119);
        osSyncPrintf(VT_RST);
    }
    self->actionFunc = func_80B98320;
}

void func_80B98320(ObjMakekinsuta* self, GlobalContext* globalCtx) {
    if (self->unk_152 != 0) {
        if (self->timer >= 60 && !func_8002DEEC(GET_PLAYER(globalCtx))) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_SW, self->actor.world.pos.x, self->actor.world.pos.y,
                        self->actor.world.pos.z, 0, self->actor.shape.rot.y, 0, (self->actor.params | 0x8000));
            self->actionFunc = ObjMakekinsuta_DoNothing;
        } else {
            self->timer++;
        }
    } else {
        self->timer = 0;
    }
}

void ObjMakekinsuta_DoNothing(ObjMakekinsuta* self, GlobalContext* globalCtx) {
}

void ObjMakekinsuta_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjMakekinsuta* self = THIS;

    self->actionFunc(self, globalCtx);
}
