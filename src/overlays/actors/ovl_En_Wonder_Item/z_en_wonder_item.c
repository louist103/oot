/*
 * File: z_en_wonder_item.c
 * Overlay: ovl_En_Wonder_Item
 * Description: Invisible Collectable; Used in MQ to create "Cow" switches
 */

#include "z_en_wonder_item.h"
#include "vt.h"

#define FLAGS 0x00000000

#define THIS ((EnWonderItem*)thisx)

void EnWonderItem_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWonderItem_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWonderItem_Update(Actor* thisx, GlobalContext* globalCtx);

void EnWonderItem_MultitagFree(EnWonderItem* self, GlobalContext* globalCtx);
void EnWonderItem_ProximityDrop(EnWonderItem* self, GlobalContext* globalCtx);
void EnWonderItem_InteractSwitch(EnWonderItem* self, GlobalContext* globalCtx);
void EnWonderItem_ProximitySwitch(EnWonderItem* self, GlobalContext* globalCtx);
void EnWonderItem_MultitagOrdered(EnWonderItem* self, GlobalContext* globalCtx);
void EnWonderItem_BombSoldier(EnWonderItem* self, GlobalContext* globalCtx);
void EnWonderItem_RollDrop(EnWonderItem* self, GlobalContext* globalCtx);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 20, 30, 0, { 0, 0, 0 } },
};

const ActorInit En_Wonder_Item_InitVars = {
    ACTOR_EN_WONDER_ITEM,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWonderItem),
    (ActorFunc)EnWonderItem_Init,
    (ActorFunc)EnWonderItem_Destroy,
    (ActorFunc)EnWonderItem_Update,
    NULL,
};

static Vec3f sTagPointsFree[9];
static Vec3f sTagPointsOrdered[9];

void EnWonderItem_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnWonderItem* self = THIS;

    if ((self->collider.dim.radius != 0) || (self->collider.dim.height != 0)) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void EnWonderItem_DropCollectible(EnWonderItem* self, GlobalContext* globalCtx, s32 autoCollect) {
    static s16 dropTable[] = {
        ITEM00_NUTS,        ITEM00_HEART_PIECE,  ITEM00_MAGIC_LARGE,   ITEM00_MAGIC_SMALL,
        ITEM00_HEART,       ITEM00_ARROWS_SMALL, ITEM00_ARROWS_MEDIUM, ITEM00_ARROWS_LARGE,
        ITEM00_RUPEE_GREEN, ITEM00_RUPEE_BLUE,   ITEM00_RUPEE_RED,     ITEM00_FLEXIBLE,
    };
    s32 i;
    s32 randomDrop;

    func_80078884(NA_SE_SY_GET_ITEM);

    if (self->dropCount == 0) {
        self->dropCount++;
    }
    for (i = self->dropCount; i > 0; i--) {
        if (self->itemDrop < WONDERITEM_DROP_RANDOM) {
            if ((self->itemDrop == WONDERITEM_DROP_FLEXIBLE) || !autoCollect) {
                Item_DropCollectible(globalCtx, &self->actor.world.pos, dropTable[self->itemDrop]);
            } else {
                Item_DropCollectible(globalCtx, &self->actor.world.pos, dropTable[self->itemDrop] | 0x8000);
            }
        } else {
            randomDrop = self->itemDrop - WONDERITEM_DROP_RANDOM;
            if (!autoCollect) {
                Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, randomDrop);
            } else {
                Item_DropCollectibleRandom(globalCtx, NULL, &self->actor.world.pos, randomDrop | 0x8000);
            }
        }
    }
    if (self->switchFlag >= 0) {
        Flags_SetSwitch(globalCtx, self->switchFlag);
    }
    Actor_Kill(&self->actor);
}

void EnWonderItem_Init(Actor* thisx, GlobalContext* globalCtx) {
    static u32 collisionTypes[] = {
        0x00000702 /* sword slash */, 0x0001F820 /* arrow */,     0x00000040 /* hammer */,   0x00000008 /* bomb */,
        0x00000004 /* slingshot */,   0x00000010 /* boomerang */, 0x00000080 /* hookshot */,
    };
    s32 pad;
    s16 colTypeIndex;
    EnWonderItem* self = THIS;
    s16 rotZover10;
    s16 tagIndex;

    osSyncPrintf("\n\n");
    // "Mysterious mystery, very mysterious"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 不思議不思議まか不思議 \t   ☆☆☆☆☆ %x\n" VT_RST, self->actor.params);
    self->actor.flags &= ~1;

    self->wonderMode = (self->actor.params >> 0xB) & 0x1F;
    self->itemDrop = (self->actor.params >> 6) & 0x1F;
    self->switchFlag = self->actor.params & 0x3F;
    if (self->switchFlag == 0x3F) {
        self->switchFlag = -1;
    }
    self->actor.targetMode = 1;
    if ((self->switchFlag >= 0) && Flags_GetSwitch(globalCtx, self->switchFlag)) {
        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ Ｙｏｕ ａｒｅ Ｓｈｏｃｋ！  ☆☆☆☆☆ %d\n" VT_RST, self->switchFlag);
        Actor_Kill(&self->actor);
        return;
    }
    switch (self->wonderMode) {
        case WONDERITEM_MULTITAG_FREE:
            self->numTagPoints = self->actor.world.rot.z & 0xF;
            rotZover10 = 0;
            if (self->actor.world.rot.z >= 10) {
                rotZover10 = self->actor.world.rot.z / 10;
                self->timerMod = rotZover10 * 20;
            }
            self->numTagPoints = self->actor.world.rot.z - rotZover10 * 10;
            // i.e timerMod = rot.z / 10 seconds, numTagPoints = rot.z % 10
            self->updateFunc = EnWonderItem_MultitagFree;
            break;
        case WONDERITEM_TAG_POINT_FREE:
            tagIndex = self->actor.world.rot.z & 0xFF;
            sTagPointsFree[tagIndex] = self->actor.world.pos;
            Actor_Kill(&self->actor);
            break;
        case WONDERITEM_PROXIMITY_DROP:
            self->dropCount = self->actor.world.rot.z & 0xFF;
            self->updateFunc = EnWonderItem_ProximityDrop;
            break;
        case WONDERITEM_INTERACT_SWITCH:
            colTypeIndex = self->actor.world.rot.z & 0xFF;
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
            self->collider.info.bumper.dmgFlags = collisionTypes[colTypeIndex];
            self->collider.dim.radius = 20;
            self->collider.dim.height = 30;
            self->updateFunc = EnWonderItem_InteractSwitch;
            break;
        case WONDERITEM_UNUSED:
            break;
        case WONDERITEM_MULTITAG_ORDERED:
            self->numTagPoints = self->actor.world.rot.z & 0xF;
            rotZover10 = 0;
            if (self->actor.world.rot.z >= 10) {
                rotZover10 = self->actor.world.rot.z / 10;
                self->timerMod = rotZover10 * 20;
            }
            self->numTagPoints = self->actor.world.rot.z - rotZover10 * 10;
            // i.e timerMod = rot.z / 10 seconds, numTagPoints = rot.z % 10
            self->updateFunc = EnWonderItem_MultitagOrdered;
            break;
        case WONDERITEM_TAG_POINT_ORDERED:
            tagIndex = self->actor.world.rot.z & 0xFF;
            sTagPointsOrdered[tagIndex] = self->actor.world.pos;
            Actor_Kill(&self->actor);
            break;
        case WONDERITEM_PROXIMITY_SWITCH:
            self->updateFunc = EnWonderItem_ProximitySwitch;
            break;
        case WONDERITEM_BOMB_SOLDIER:
            Collider_InitCylinder(globalCtx, &self->collider);
            Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
            self->collider.info.bumper.dmgFlags = 0x00000004; // slingshot
            self->unkPos = self->actor.world.pos;
            self->collider.dim.radius = 35;
            self->collider.dim.height = 75;
            self->updateFunc = EnWonderItem_BombSoldier;
            break;
        case WONDERITEM_ROLL_DROP:
            self->dropCount = self->actor.world.rot.z & 0xFF;
            self->updateFunc = EnWonderItem_RollDrop;
            break;
        default:
            Actor_Kill(&self->actor);
            break;
    }
}

void EnWonderItem_MultitagFree(EnWonderItem* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 prevTagFlags = self->tagFlags;
    s32 i;
    s32 mask;

    for (i = 0, mask = 1; i < self->numTagPoints; i++, mask <<= 1) {
        if (!(prevTagFlags & mask)) {
            f32 dx = player->actor.world.pos.x - sTagPointsFree[i].x;
            f32 dy = player->actor.world.pos.y - sTagPointsFree[i].y;
            f32 dz = player->actor.world.pos.z - sTagPointsFree[i].z;

            if (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 50.0f) {
                self->tagFlags |= mask;
                self->tagCount++;
                self->timer = self->timerMod + 81;
                return;
            }
            if (BREG(0) != 0) {
                DebugDisplay_AddObject(sTagPointsFree[i].x, sTagPointsFree[i].y, sTagPointsFree[i].z,
                                       self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 0, 255, 0, 255, 4, globalCtx->state.gfxCtx);
            }
        }
    }
    if (self->timer == 1) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->tagCount == self->numTagPoints) {
        if (self->switchFlag >= 0) {
            Flags_SetSwitch(globalCtx, self->switchFlag);
        }
        EnWonderItem_DropCollectible(self, globalCtx, true);
    }
}

void EnWonderItem_ProximityDrop(EnWonderItem* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((self->actor.xzDistToPlayer < 50.0f) && (fabsf(self->actor.world.pos.y - player->actor.world.pos.y) < 30.0f)) {
        EnWonderItem_DropCollectible(self, globalCtx, true);
    }
}

void EnWonderItem_InteractSwitch(EnWonderItem* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        EnWonderItem_DropCollectible(self, globalCtx, false);
    }
}

void EnWonderItem_ProximitySwitch(EnWonderItem* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((self->actor.xzDistToPlayer < 50.0f) && (fabsf(self->actor.world.pos.y - player->actor.world.pos.y) < 30.0f)) {
        if (self->switchFlag >= 0) {
            Flags_SetSwitch(globalCtx, self->switchFlag);
        }
        Actor_Kill(&self->actor);
    }
}

void EnWonderItem_MultitagOrdered(EnWonderItem* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);
    s32 prevTagFlags = self->tagFlags;
    s32 i;
    s32 mask;

    for (i = 0, mask = 1; i < self->numTagPoints; i++, mask <<= 1) {
        if (!(prevTagFlags & mask)) {
            f32 dx = player->actor.world.pos.x - sTagPointsOrdered[i].x;
            f32 dy = player->actor.world.pos.y - sTagPointsOrdered[i].y;
            f32 dz = player->actor.world.pos.z - sTagPointsOrdered[i].z;

            if (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 50.0f) {
                if (prevTagFlags & mask) {
                    return;
                } else if (i == self->nextTag) {
                    self->tagFlags |= mask;
                    self->tagCount++;
                    self->nextTag++;
                    self->timer = self->timerMod + 81;
                    return;
                } else {
                    Actor_Kill(&self->actor);
                    return;
                }
            } else if (BREG(0) != 0) {
                DebugDisplay_AddObject(sTagPointsOrdered[i].x, sTagPointsOrdered[i].y, sTagPointsOrdered[i].z,
                                       self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 0, 0, 255, 255, 4, globalCtx->state.gfxCtx);
            }
        }
    }
    if (self->timer == 1) {
        Actor_Kill(&self->actor);
        return;
    }
    if (self->tagCount == self->numTagPoints) {
        EnWonderItem_DropCollectible(self, globalCtx, true);
    }
}

void EnWonderItem_BombSoldier(EnWonderItem* self, GlobalContext* globalCtx) {
    if (self->collider.base.acFlags & AC_HIT) {
        self->collider.base.acFlags &= ~AC_HIT;
        if (Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_HEISHI2, self->actor.world.pos.x,
                        self->actor.world.pos.y, self->actor.world.pos.z, 0, self->actor.yawTowardsPlayer, 0,
                        9) != NULL) {
            // "Careless soldier spawned"
            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ うっかり兵セット完了 ☆☆☆☆☆ \n" VT_RST);
        }
        if (self->switchFlag >= 0) {
            Flags_SetSwitch(globalCtx, self->switchFlag);
        }
        Actor_Kill(&self->actor);
    }
}

void EnWonderItem_RollDrop(EnWonderItem* self, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    if ((self->actor.xzDistToPlayer < 50.0f) && (player->invincibilityTimer < 0) &&
        (fabsf(self->actor.world.pos.y - player->actor.world.pos.y) < 30.0f)) {
        EnWonderItem_DropCollectible(self, globalCtx, true);
    }
}

void EnWonderItem_Update(Actor* thisx, GlobalContext* globalCtx) {
    static s16 debugArrowColors[] = {
        255, 255, 0,   255, 0,   255, 0,   255, 255, 255, 0,   0, 0, 255, 0,   0, 0, 255, 128, 128,
        128, 128, 128, 0,   128, 0,   128, 0,   128, 0,   128, 0, 0, 0,   128, 0, 0, 0,   128,
    }; // These seem to be mistyped. Logically they should be s16[13][3] and be indexed as [colorIndex][i]
    s32 pad;
    EnWonderItem* self = THIS;
    s32 colorIndex;

    if (self->timer != 0) {
        self->timer--;
    }
    self->updateFunc(self, globalCtx);

    if (self->wonderMode == WONDERITEM_UNUSED) {
        Actor_SetFocus(&self->actor, self->unkHeight);
    }
    if ((self->wonderMode == WONDERITEM_INTERACT_SWITCH) || (self->wonderMode == WONDERITEM_BOMB_SOLDIER)) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
    }

    colorIndex = self->wonderMode;
    if (self->wonderMode > 12) {
        colorIndex = 0;
    }
    if (BREG(0) != 0) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, debugArrowColors[colorIndex], debugArrowColors[colorIndex + 1],
                               debugArrowColors[colorIndex + 2], 255, 4, globalCtx->state.gfxCtx);
    }
}
