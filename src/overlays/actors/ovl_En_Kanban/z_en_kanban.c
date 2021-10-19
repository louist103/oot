/*
 * File: z_en_kanban.c
 * Overlay: ovl_En_Kanban
 * Description: Cuttable square sign
 */

#include "z_en_kanban.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_kanban/object_kanban.h"
#include "vt.h"

#define FLAGS 0x00000019

#define THIS ((EnKanban*)thisx)

#define PART_UPPER_LEFT (1 << 0)
#define PART_LEFT_UPPER (1 << 1)
#define PART_LEFT_LOWER (1 << 2)
#define PART_RIGHT_UPPER (1 << 3)
#define PART_RIGHT_LOWER (1 << 4)
#define PART_LOWER_LEFT (1 << 5)
#define PART_UPPER_RIGHT (1 << 6)
#define PART_LOWER_RIGHT (1 << 7)
#define PART_POST_UPPER (1 << 8)
#define PART_POST_LOWER (1 << 9)
#define PART_POST_STAND (1 << 10)
#define LEFT_HALF (PART_UPPER_LEFT | PART_LEFT_UPPER | PART_LEFT_LOWER | PART_LOWER_LEFT)
#define RIGHT_HALF (PART_UPPER_RIGHT | PART_RIGHT_UPPER | PART_RIGHT_LOWER | PART_LOWER_RIGHT)
#define UPPER_HALF (PART_POST_UPPER | PART_UPPER_RIGHT | PART_RIGHT_UPPER | PART_UPPER_LEFT | PART_LEFT_UPPER)
#define UPPERLEFT_HALF (PART_POST_UPPER | PART_UPPER_RIGHT | PART_LEFT_LOWER | PART_UPPER_LEFT | PART_LEFT_UPPER)
#define UPPERRIGHT_HALF (PART_POST_UPPER | PART_UPPER_RIGHT | PART_RIGHT_UPPER | PART_UPPER_LEFT | PART_RIGHT_LOWER)
#define ALL_PARTS (LEFT_HALF | RIGHT_HALF | PART_POST_UPPER | PART_POST_LOWER)

typedef enum {
    ENKANBAN_SIGN,
    ENKANBAN_AIR,
    ENKANBAN_UNUSED,
    ENKANBAN_GROUND,
    ENKANBAN_WATER,
    ENKANBAN_REPAIR
} EnKanbanActionState;

typedef enum {
    PIECE_WHOLE_SIGN,
    PIECE_UPPER_HALF,
    PIECE_LOWER_HALF,
    PIECE_RIGHT_HALF,
    PIECE_LEFT_HALF,
    PIECE_2ND_QUAD,
    PIECE_1ST_QUAD,
    PIECE_3RD_QUAD,
    PIECE_4TH_QUAD,
    PIECE_UPPER_LEFT,
    PIECE_LEFT_UPPER,
    PIECE_LEFT_LOWER,
    PIECE_LOWER_LEFT,
    PIECE_UPPER_RIGHT,
    PIECE_RIGHT_UPPER,
    PIECE_RIGHT_LOWER,
    PIECE_LOWER_RIGHT,
    PIECE_POST_UPPER,
    PIECE_POST_LOWER,
    PIECE_OTHER = 100
} EnKanbanPiece;

typedef enum {
    CUT_POST,
    CUT_VERT_L,
    CUT_HORIZ,
    CUT_DIAG_L, // lower left to upper right
    CUT_DIAG_R, // upper left to lower right
    CUT_VERT_R
} EnKanbanCutType;

void EnKanban_Init(Actor* thisx, GlobalContext* globalCtx);
void EnKanban_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnKanban_Update(Actor* thisx, GlobalContext* globalCtx);
void EnKanban_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit En_Kanban_InitVars = {
    ACTOR_EN_KANBAN,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_KANBAN,
    sizeof(EnKanban),
    (ActorFunc)EnKanban_Init,
    (ActorFunc)EnKanban_Destroy,
    (ActorFunc)EnKanban_Update,
    (ActorFunc)EnKanban_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 50, 5, { 0, 0, 0 } },
};

static u16 sPartFlags[] = {
    PART_UPPER_LEFT,  PART_LEFT_UPPER,  PART_LEFT_LOWER, PART_RIGHT_UPPER, PART_RIGHT_LOWER, PART_LOWER_LEFT,
    PART_UPPER_RIGHT, PART_LOWER_RIGHT, PART_POST_UPPER, PART_POST_LOWER,  PART_POST_STAND,
};

static Vec3f sPieceOffsets[] = {
    /* WHOLE_SIGN   */ { 0.0f, 44.0f, 0.0f },
    /* UPPER_HALF   */ { 0.0f, 50.0f, 0.0f },
    /* LOWER_HALF   */ { 0.0f, 38.0f, 0.0f },
    /* RIGHT_HALF  */ { 10.0f, 44.0f, 0.0f },
    /* LEFT_HALF  */ { -10.0f, 44.0f, 0.0f },
    /* 2ND_QUAD   */ { -10.0f, 50.0f, 0.0f },
    /* 1ST_QUAD    */ { 10.0f, 50.0f, 0.0f },
    /* 3RD_QUAD   */ { -10.0f, 38.0f, 0.0f },
    /* 4TH_QUAD    */ { 10.0f, 38.0f, 0.0f },
    /* UPPER_LEFT  */ { -7.5f, 51.0f, 0.0f },
    /* LEFT_UPPER */ { -12.5f, 48.0f, 0.0f },
    /* LEFT_LOWER */ { -12.5f, 40.0f, 0.0f },
    /* LOWER_LEFT  */ { -7.5f, 37.0f, 0.0f },
    /* UPPER_RIGHT  */ { 7.5f, 51.0f, 0.0f },
    /* RIGHT_UPPER */ { 12.5f, 48.0f, 0.0f },
    /* RIGHT_LOWER */ { 12.5f, 40.0f, 0.0f },
    /* LOWER_RIGHT  */ { 7.5f, 37.0f, 0.0f },
    /* POST_UPPER   */ { 0.0f, 50.0f, 0.0f },
    /* POST_LOWER   */ { 0.0f, 38.0f, 0.0f },
};

static Vec3f sPieceSizes[] = {
    /* WHOLE_SIGN */ { 1500.0f, 1000.0f, 0.0f },
    /* UPPER_HALF */ { 1500.0f, 500.0f, 0.0f },
    /* LOWER_HALF */ { 1500.0f, 500.0f, 0.0f },
    /* RIGHT_HALF  */ { 700.0f, 1000.0f, 0.0f },
    /* LEFT_HALF   */ { 700.0f, 1000.0f, 0.0f },
    /* 2ND_QUAD    */ { 700.0f, 500.0f, 0.0f },
    /* 1ST_QUAD    */ { 700.0f, 500.0f, 0.0f },
    /* 3RD_QUAD    */ { 700.0f, 500.0f, 0.0f },
    /* 4TH_QUAD    */ { 700.0f, 500.0f, 0.0f },
    /* UPPER_LEFT  */ { 700.0f, 500.0f, 0.0f },
    /* LEFT_UPPER  */ { 700.0f, 500.0f, 0.0f },
    /* LEFT_LOWER  */ { 700.0f, 500.0f, 0.0f },
    /* LOWER_LEFT  */ { 700.0f, 500.0f, 0.0f },
    /* UPPER_RIGHT */ { 700.0f, 500.0f, 0.0f },
    /* RIGHT_UPPER */ { 700.0f, 500.0f, 0.0f },
    /* RIGHT_LOWER */ { 700.0f, 500.0f, 0.0f },
    /* LOWER_RIGHT */ { 700.0f, 500.0f, 0.0f },
    /* POST_UPPER  */ { 200.0f, 500.0f, 0.0f },
    /* POST_LOWER  */ { 200.0f, 500.0f, 0.0f },
};

static u8 sCutTypes[] = {
    /* 1H_OVER     */ CUT_VERT_L, /* 2H_OVER     */ CUT_VERT_L,
    /* 1H_COMBO    */ CUT_DIAG_R, /* 2H_COMBO    */ CUT_DIAG_R,
    /* 1H_LEFT     */ CUT_HORIZ,  /* 2H_LEFT     */ CUT_HORIZ,
    /* 1H_COMBO    */ CUT_HORIZ,  /* 2H_COMBO    */ CUT_HORIZ,
    /* 1H_RIGHT    */ CUT_HORIZ,  /* 2H_RIGHT    */ CUT_HORIZ,
    /* 1H_COMBO    */ CUT_HORIZ,  /* 2H_COMBO    */ CUT_HORIZ,
    /* 1H_STAB     */ CUT_POST,   /* 2H_STAB     */ CUT_POST,
    /* 1H_COMBO    */ CUT_POST,   /* 2H_COMBO    */ CUT_POST,
    /* FLIP_START  */ CUT_VERT_L, /* JUMP_START  */ CUT_VERT_L,
    /* FLIP_END    */ CUT_VERT_L, /* JUMP_END    */ CUT_VERT_L,
    /* BACK_LEFT   */ CUT_HORIZ,  /* BACK_RIGHT  */ CUT_HORIZ,
    /* OVER_HAMMER */ CUT_POST,   /* SIDE_HAMMER */ CUT_POST,
    /* 1H_SPIN_ATK */ CUT_POST,   /* 2H_SPIN_ATK */ CUT_POST,
    /* 1H_BIG_SPIN */ CUT_POST,   /* 2H_BIG_SPIN */ CUT_POST,
};

static u16 sCutFlags[] = {
    /* CUT_POST   */ ALL_PARTS,       /* CUT_VERT_L */ LEFT_HALF,
    /* CUT_HORIZ  */ UPPER_HALF,      /* CUT_DIAG_L */ UPPERLEFT_HALF,
    /* CUT_DIAG_R */ UPPERRIGHT_HALF, /* CUT_VERT_R */ RIGHT_HALF,
};

void EnKanban_SetFloorRot(EnKanban* self) {
    if (self->actor.floorPoly != NULL) {
        f32 nx = COLPOLY_GET_NORMAL(self->actor.floorPoly->normal.x);
        f32 ny = COLPOLY_GET_NORMAL(self->actor.floorPoly->normal.y);
        f32 nz = COLPOLY_GET_NORMAL(self->actor.floorPoly->normal.z);

        self->floorRot.x = -Math_FAtan2F(-nz * ny, 1.0f);
        self->floorRot.z = Math_FAtan2F(-nx * ny, 1.0f);
    }
}

void EnKanban_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnKanban* self = THIS;

    Actor_SetScale(&self->actor, 0.01f);
    if (self->actor.params != ENKANBAN_PIECE) {
        self->actor.targetMode = 0;
        self->actor.flags |= 1;
        Collider_InitCylinder(globalCtx, &self->collider);
        Collider_SetCylinder(globalCtx, &self->collider, &self->actor, &sCylinderInit);
        osSyncPrintf("KANBAN ARG    %x\n", self->actor.params);
        if (self->actor.params == ENKANBAN_FISHING) {
            if (LINK_IS_CHILD) {
                self->actor.textId = 0x409D;
            } else {
                self->actor.textId = 0x4090;
            }
        } else {
            self->actor.textId = self->actor.params | 0x300;
        }
        self->bounceX = 1;
        self->partFlags = 0xFFFF;
        Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 50.0f, 4);
        EnKanban_SetFloorRot(self);
        if (LINK_IS_CHILD) {
            self->actor.world.pos.y -= 15.0f;
        }
    }
}

void EnKanban_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnKanban* self = THIS;

    if (self->actionState == ENKANBAN_SIGN) {
        Collider_DestroyCylinder(globalCtx, &self->collider);
    }
}

void EnKanban_Message(EnKanban* self, GlobalContext* globalCtx) {
    if (!self->msgFlag) {
        if (self->msgTimer == 0) {
            if (ABS((s16)(self->actor.yawTowardsPlayer - self->actor.shape.rot.y)) < 0x2800) {
                if (func_8002F194(&self->actor, globalCtx)) {
                    self->msgFlag = true;
                } else {
                    func_8002F2CC(&self->actor, globalCtx, 68.0f);
                }
            }
        } else {
            self->msgTimer--;
        }
    } else {
        if (func_8002F334(&self->actor, globalCtx)) {
            self->msgFlag = false;
            self->msgTimer = 20;
        }
    }
}

void EnKanban_Update(Actor* thisx, GlobalContext* globalCtx2) {
    u8 bounced = false;
    GlobalContext* globalCtx = globalCtx2;
    EnKanban* self = THIS;
    EnKanban* signpost;
    EnKanban* piece;
    Player* player = GET_PLAYER(globalCtx);
    Vec3f offset;

    self->frameCount++;
    switch (self->actionState) {
        case ENKANBAN_SIGN:
            if (self->invincibilityTimer != 0) {
                self->invincibilityTimer--;
            }
            if (self->zTargetTimer != 0) {
                self->zTargetTimer--;
            }
            if (self->zTargetTimer == 1) {
                self->actor.flags &= ~1;
            }
            if (self->partFlags == 0xFFFF) {
                EnKanban_Message(self, globalCtx);
            }
            if ((self->invincibilityTimer == 0) && (self->collider.base.acFlags & AC_HIT)) {
                self->collider.base.acFlags &= ~AC_HIT;
                self->invincibilityTimer = 6;
                piece = (EnKanban*)Actor_SpawnAsChild(&globalCtx->actorCtx, &self->actor, globalCtx, ACTOR_EN_KANBAN,
                                                      self->actor.world.pos.x, self->actor.world.pos.y,
                                                      self->actor.world.pos.z, self->actor.shape.rot.x,
                                                      self->actor.shape.rot.y, self->actor.shape.rot.z, ENKANBAN_PIECE);
                if (piece != NULL) {
                    ColliderInfo* hitItem = self->collider.info.acHitInfo;
                    s16 yawDiff = self->actor.yawTowardsPlayer - self->actor.shape.rot.y;
                    u8 i;

                    if (hitItem->toucher.dmgFlags & 0x700) {
                        self->cutType = sCutTypes[player->swordAnimation];
                    } else {
                        self->cutType = CUT_POST;
                    }
                    if (ABS(yawDiff) > 0x4000) {
                        if (self->cutType == CUT_DIAG_R) {
                            self->cutType = CUT_DIAG_L;
                        } else if (self->cutType == CUT_VERT_L) {
                            self->cutType = CUT_VERT_R;
                        }
                    }
                    piece->partFlags = sCutFlags[self->cutType] & self->partFlags;
                    if (piece->partFlags == 0) {
                        Actor_Kill(&piece->actor);
                        return;
                    }
                    piece->partCount = 0;
                    for (i = 0; i < ARRAY_COUNT(sPartFlags); i++) {
                        if (sPartFlags[i] & piece->partFlags) {
                            piece->partCount++;
                        }
                    }
                    self->partFlags &= ~sCutFlags[self->cutType];
                    if (!(self->partFlags & ALL_PARTS)) {
                        self->zTargetTimer = 10;
                    }
                    if ((piece->partFlags & PART_UPPER_LEFT) && (piece->partFlags & PART_LOWER_RIGHT)) {
                        piece->pieceType = PIECE_WHOLE_SIGN;
                    } else if ((piece->partFlags & PART_LEFT_UPPER) && (piece->partFlags & PART_RIGHT_UPPER)) {
                        piece->pieceType = PIECE_UPPER_HALF;
                    } else if ((piece->partFlags & PART_LEFT_LOWER) && (piece->partFlags & PART_RIGHT_LOWER)) {
                        piece->pieceType = PIECE_LOWER_HALF;
                    } else if ((piece->partFlags & PART_UPPER_RIGHT) && (piece->partFlags & PART_LOWER_RIGHT)) {
                        piece->pieceType = PIECE_RIGHT_HALF;
                    } else if ((piece->partFlags & PART_UPPER_LEFT) && (piece->partFlags & PART_LOWER_LEFT)) {
                        piece->pieceType = PIECE_LEFT_HALF;
                    } else if ((piece->partFlags & PART_UPPER_LEFT) && (piece->partFlags & PART_LEFT_UPPER)) {
                        piece->pieceType = PIECE_2ND_QUAD;
                    } else if ((piece->partFlags & PART_UPPER_RIGHT) && (piece->partFlags & PART_RIGHT_UPPER)) {
                        piece->pieceType = PIECE_1ST_QUAD;
                    } else if ((piece->partFlags & PART_LEFT_LOWER) && (piece->partFlags & PART_LOWER_LEFT)) {
                        piece->pieceType = PIECE_3RD_QUAD;
                    } else if ((piece->partFlags & PART_RIGHT_LOWER) && (piece->partFlags & PART_LOWER_RIGHT)) {
                        piece->pieceType = PIECE_4TH_QUAD;
                    } else if (piece->partFlags & PART_UPPER_LEFT) {
                        piece->pieceType = PIECE_UPPER_LEFT;
                    } else if (piece->partFlags & PART_LEFT_UPPER) {
                        piece->pieceType = PIECE_LEFT_UPPER;
                    } else if (piece->partFlags & PART_LEFT_LOWER) {
                        piece->pieceType = PIECE_LEFT_LOWER;
                    } else if (piece->partFlags & PART_LOWER_LEFT) {
                        piece->pieceType = PIECE_LOWER_LEFT;
                    } else if (piece->partFlags & PART_UPPER_RIGHT) {
                        piece->pieceType = PIECE_UPPER_RIGHT;
                    } else if (piece->partFlags & PART_RIGHT_UPPER) {
                        piece->pieceType = PIECE_RIGHT_UPPER;
                    } else if (piece->partFlags & PART_RIGHT_LOWER) {
                        piece->pieceType = PIECE_RIGHT_LOWER;
                    } else if (piece->partFlags & PART_LOWER_RIGHT) {
                        piece->pieceType = PIECE_LOWER_RIGHT;
                    } else if (piece->partFlags & PART_POST_UPPER) {
                        piece->pieceType = PIECE_POST_UPPER;
                    } else if (piece->partFlags & PART_POST_LOWER) {
                        piece->pieceType = PIECE_POST_LOWER;
                    } else {
                        piece->pieceType = PIECE_OTHER;
                    }
                    if (piece->pieceType == 100) {
                        piece->pieceType = PIECE_WHOLE_SIGN;
                    }

                    Matrix_RotateY((self->actor.shape.rot.y / (f32)0x8000) * M_PI, MTXMODE_NEW);
                    Matrix_MultVec3f(&sPieceOffsets[piece->pieceType], &offset);
                    piece->actor.world.pos.x += offset.x;
                    piece->actor.world.pos.y += offset.y;
                    piece->actor.world.pos.z += offset.z;
                    piece->offset.x = -sPieceOffsets[piece->pieceType].x / self->actor.scale.x;
                    piece->offset.y = -sPieceOffsets[piece->pieceType].y / self->actor.scale.x;
                    piece->offset.z = -sPieceOffsets[piece->pieceType].z / self->actor.scale.x;
                    piece->pieceWidth = sPieceSizes[piece->pieceType].x;
                    piece->pieceHeight = sPieceSizes[piece->pieceType].y;
                    piece->actor.gravity = -1.0f;
                    piece->actionState = ENKANBAN_AIR;
                    piece->actor.world.rot.y = (s16)Rand_CenteredFloat(0x3000) + self->actor.yawTowardsPlayer + 0x8000;
                    piece->actor.velocity.y = Rand_ZeroFloat(2.0f) + 3.0f;
                    piece->actor.speedXZ = Rand_ZeroFloat(2.0f) + 3.0f;
                    if (piece->partCount >= 4) {
                        piece->bounceX = (s16)Rand_ZeroFloat(10.0f) + 6;
                        piece->bounceZ = (s16)Rand_ZeroFloat(10.0f) + 6;
                    } else {
                        piece->bounceX = (s16)Rand_ZeroFloat(7.0f) + 3;
                        piece->bounceZ = (s16)Rand_ZeroFloat(7.0f) + 3;
                    }
                    piece->spinVel.y = Rand_CenteredFloat(0x1800);
                    if (Rand_ZeroOne() < 0.5f) {
                        piece->direction = 1;
                    } else {
                        piece->direction = -1;
                    }
                    piece->airTimer = 100;
                    piece->actor.flags &= ~1;
                    piece->actor.flags |= 0x02000000;
                    self->cutMarkTimer = 5;
                    Audio_PlayActorSound2(&self->actor, NA_SE_IT_SWORD_STRIKE);
                }
            }
            self->actor.focus.pos = self->actor.world.pos;
            self->actor.focus.pos.y += 44.0f;
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &self->collider.base);
            if (self->actor.xzDistToPlayer > 500.0f) {
                self->actor.flags |= 1;
                self->partFlags = 0xFFFF;
            }
            if (self->cutMarkTimer != 0) {
                if (self->cutMarkTimer >= 5) {
                    self->cutMarkAlpha += 255;
                    if (self->cutMarkAlpha > 255) {
                        self->cutMarkAlpha = 255;
                    }
                } else {
                    self->cutMarkAlpha -= 65;
                    if (self->cutMarkAlpha < 0) {
                        self->cutMarkAlpha = 0;
                    }
                }
                self->cutMarkTimer--;
            }
            break;
        case ENKANBAN_AIR:
        case ENKANBAN_UNUSED: {
            u16 tempBgFlags;
            f32 tempX;
            f32 tempY;
            f32 tempZ;
            f32 tempYDistToWater;
            u8 onGround;

            Actor_MoveForward(&self->actor);
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 30.0f, 30.0f, 50.0f, 5);

            tempX = self->actor.world.pos.x;
            tempY = self->actor.world.pos.y;
            tempZ = self->actor.world.pos.z;
            tempBgFlags = self->actor.bgCheckFlags;
            tempYDistToWater = self->actor.yDistToWater;

            self->actor.world.pos.z += ((self->actor.world.pos.y - self->actor.floorHeight) * -50.0f) / 100.0f;
            Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 50.0f, 4);
            EnKanban_SetFloorRot(self);

            self->actor.world.pos.x = tempX;
            self->actor.world.pos.y = tempY;
            self->actor.world.pos.z = tempZ;
            self->actor.bgCheckFlags = tempBgFlags;
            self->actor.yDistToWater = tempYDistToWater;

            osSyncPrintf(VT_RST);
            onGround = (self->actor.bgCheckFlags & 1);
            if (self->spinXFlag) {
                self->spinRot.x += self->spinVel.x;
                self->spinVel.x -= 0x800;
                if ((self->spinRot.x <= 0) && onGround) {
                    self->spinRot.x = 0;
                    self->spinVel.x = 0;
                }
            } else {
                self->spinRot.x -= self->spinVel.x;
                self->spinVel.x -= 0x800;
                if ((self->spinRot.x >= 0) && onGround) {
                    self->spinRot.x = 0;
                    self->spinVel.x = 0;
                }
            }
            if (self->spinVel.x < -0xC00) {
                self->spinVel.x = -0xC00;
            }
            if (self->spinZFlag) {
                self->spinRot.z += self->spinVel.z;
                self->spinVel.z -= 0x800;
                if ((self->spinRot.z <= 0) && onGround) {
                    self->spinRot.z = 0;
                    self->spinVel.z = 0;
                }
            } else {
                self->spinRot.z -= self->spinVel.z;
                self->spinVel.z -= 0x800;
                if ((self->spinRot.z >= 0) && onGround) {
                    self->spinRot.z = 0;
                    self->spinVel.z = 0;
                }
            }
            if (self->spinVel.z < -0xC00) {
                self->spinVel.z = -0xC00;
            }
            if (self->actor.bgCheckFlags & 8) {
                self->actor.speedXZ *= -0.5f;
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_WOODPLATE_BOUND);
            }
            if (self->actor.bgCheckFlags & 0x40) {
                self->actionState = ENKANBAN_WATER;
                Audio_PlayActorSound2(&self->actor, NA_SE_EV_BOMB_DROP_WATER);
                self->bounceX = self->bounceZ = 0;
                self->actor.world.pos.y += self->actor.yDistToWater;
                EffectSsGSplash_Spawn(globalCtx, &self->actor.world.pos, NULL, NULL, 0, (self->partCount * 20) + 300);
                EffectSsGRipple_Spawn(globalCtx, &self->actor.world.pos, 150, 650, 0);
                EffectSsGRipple_Spawn(globalCtx, &self->actor.world.pos, 300, 800, 5);
                self->actor.velocity.y = 0.0f;
                self->actor.gravity = 0.0f;
                osSyncPrintf(" WAT  Y  = %f\n", self->actor.yDistToWater);
                osSyncPrintf(" POS  Y  = %f\n", self->actor.world.pos.y);
                osSyncPrintf(" GROUND Y  = %f\n", self->actor.floorHeight);
                break;
            }

            if (onGround) {
                if (self->bounceCount <= 0) {
                    self->bounceCount++;
                    self->actor.velocity.y *= -0.3f;
                    self->actor.world.rot.y += (s16)Rand_CenteredFloat(16384.0f);
                } else {
                    self->actor.velocity.y = 0.0f;
                }
                self->actor.speedXZ *= 0.7f;
                if ((self->spinRot.x == 0) && (self->bounceX != 0)) {
                    self->spinVel.x = self->bounceX * 0x200;
                    if (self->bounceX != 0) {
                        self->bounceX -= 5;
                        if (self->bounceX <= 0) {
                            self->bounceX = 0;
                        }
                    }
                    if (Rand_ZeroOne() < 0.5f) {
                        self->spinXFlag = true;
                    } else {
                        self->spinXFlag = false;
                    }
                    bounced = true;
                }
                if ((self->spinRot.z == 0) && (self->bounceZ != 0)) {
                    self->spinVel.z = self->bounceZ * 0x200;
                    if (self->bounceZ != 0) {
                        self->bounceZ -= 5;
                        if (self->bounceZ <= 0) {
                            self->bounceZ = 0;
                        }
                    }
                    if (Rand_ZeroOne() < 0.5f) {
                        self->spinZFlag = true;
                    } else {
                        self->spinZFlag = false;
                    }
                    bounced = true;
                }
                Math_ApproachS(&self->actor.shape.rot.x, self->direction * 0x4000, 1, 0x2000);
            } else {
                self->actor.shape.rot.y += self->spinVel.y;
                self->actor.shape.rot.x += self->direction * 0x7D0;
            }
            if (bounced) {
                s16 dustCount;
                s16 j;
                Vec3f velocity = { 0.0f, 0.0f, 0.0f };
                Vec3f accel;
                Vec3f pos;

                Audio_PlayActorSound2(&self->actor, NA_SE_EV_WOODPLATE_BOUND);
                accel.x = 0.0f;
                accel.y = 0.1f;
                accel.z = 0.0f;
                pos.y = self->actor.floorHeight + 3.0f;
                dustCount = self->partCount * 0.5f;
                for (j = 0; j < dustCount + 3; j++) {
                    pos.x = self->actor.world.pos.x + Rand_CenteredFloat((self->partCount * 0.5f) + 20.0f);
                    pos.z = self->actor.world.pos.z + Rand_CenteredFloat((self->partCount * 0.5f) + 20.0f);
                    func_800286CC(globalCtx, &pos, &velocity, &accel, 100, 5);
                }
            }
            if (DECR(self->airTimer) == 0) {
                self->actionState = ENKANBAN_GROUND;
            }
        }
        case ENKANBAN_GROUND:
        case ENKANBAN_WATER:
            signpost = (EnKanban*)self->actor.parent;

            if (signpost->partFlags == 0xFFFF) {
                Actor_Kill(&self->actor);
            }
            Math_ApproachF(&self->actor.shape.yOffset, 100.0f, 1.0f, 5.0f);
            if (self->actionState == ENKANBAN_WATER) {
                s32 rippleDelay;
                s32 rippleScale;

                if ((player->actor.speedXZ > 0.0f) && (player->actor.world.pos.y < self->actor.world.pos.y) &&
                    (self->actor.xyzDistToPlayerSq < 2500.0f)) {
                    Math_ApproachF(&self->actor.speedXZ, player->actor.speedXZ, 1.0f, 0.2f);
                    if (self->actor.speedXZ > 1.0f) {
                        self->actor.speedXZ = 1.0f;
                    }
                    if (Math_SmoothStepToS(&self->actor.world.rot.y, self->actor.yawTowardsPlayer + 0x8000, 1, 0x1000,
                                           0) > 0) {
                        self->spinVel.y = self->actor.speedXZ * 1000.0f;
                    } else {
                        self->spinVel.y = self->actor.speedXZ * -1000.0f;
                    }
                }
                if (self->actor.bgCheckFlags & 1) {
                    self->actor.speedXZ = 0.0f;
                }
                Actor_MoveForward(&self->actor);
                if (self->actor.speedXZ != 0.0f) {
                    Actor_UpdateBgCheckInfo(globalCtx, &self->actor, 10.0f, 10.0f, 50.0f, 5);
                    if (self->actor.bgCheckFlags & 8) {
                        self->actor.speedXZ *= -0.5f;
                        if (self->spinVel.y > 0) {
                            self->spinVel.y = -0x7D0;
                        } else {
                            self->spinVel.y = 0x7D0;
                        }
                    }
                    Math_ApproachZeroF(&self->actor.speedXZ, 1.0f, 0.15f);
                }
                self->actor.shape.rot.y += self->spinVel.y;
                Math_ApproachS(&self->spinVel.y, 0, 1, 0x3A);
                Math_ApproachS(&self->actor.shape.rot.x, self->direction * 0x4000, 2, 0x1000);
                Math_ApproachS(&self->spinRot.x, Math_SinS(2500 * self->frameCount) * 500.0f, 2, 0x1000);
                Math_ApproachS(&self->spinRot.z, Math_CosS(3000 * self->frameCount) * 500.0f, 2, 0x1000);
                Math_ApproachZeroF(&self->floorRot.x, 0.5f, 0.2f);
                Math_ApproachZeroF(&self->floorRot.z, 0.5f, 0.2f);
                if (fabsf(self->actor.speedXZ) > 1.0f) {
                    rippleDelay = 0;
                } else if (fabsf(self->actor.speedXZ) > 0.5f) {
                    rippleDelay = 3;
                } else {
                    rippleDelay = 7;
                }
                if (!(self->frameCount & rippleDelay)) {
                    if (self->partCount < 3) {
                        rippleScale = 0;
                    } else if (self->partCount < 6) {
                        rippleScale = 100;
                    } else {
                        rippleScale = 200;
                    }
                    EffectSsGRipple_Spawn(globalCtx, &self->actor.world.pos, rippleScale, rippleScale + 500, 0);
                }
            } else if ((globalCtx->actorCtx.unk_02 != 0) && (self->actor.xyzDistToPlayerSq < SQ(100.0f))) {
                f32 hammerStrength = (100.0f - sqrtf(self->actor.xyzDistToPlayerSq)) * 0.05f;

                self->actionState = ENKANBAN_AIR;
                self->actor.gravity = -1.0f;
                self->actor.world.rot.y = Rand_CenteredFloat(0x10000);
                if (self->partCount >= 4) {
                    self->bounceX = (s16)Rand_ZeroFloat(10.0f) + 6;
                    self->bounceZ = (s16)Rand_ZeroFloat(10.0f) + 6;
                    self->actor.velocity.y = 2.0f + hammerStrength;
                    self->actor.speedXZ = Rand_ZeroFloat(1.0f);
                } else {
                    self->bounceX = (s16)Rand_ZeroFloat(7.0f) + 3;
                    self->bounceZ = (s16)Rand_ZeroFloat(7.0f) + 3;
                    self->actor.velocity.y = 3.0f + hammerStrength;
                    self->actor.speedXZ = Rand_ZeroFloat(1.5f);
                }
                self->spinVel.y = Rand_CenteredFloat(0x1800);
                if (Rand_ZeroOne() < 0.5f) {
                    self->direction = 1;
                } else {
                    self->direction = -1;
                }
                self->airTimer = 70;
            }
            if (self->bounceX == 0) {
                Actor* bomb = globalCtx->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].head;
                f32 dx;
                f32 dy;
                f32 dz;

                while (bomb != NULL) {
                    if (bomb->params != 1) {
                        bomb = bomb->next;
                        continue;
                    }
                    dx = self->actor.world.pos.x - bomb->world.pos.x;
                    dy = self->actor.world.pos.y - bomb->world.pos.y;
                    dz = self->actor.world.pos.z - bomb->world.pos.z;
                    if (sqrtf(SQ(dx) + SQ(dy) + SQ(dz)) < 100.0f) {
                        f32 bombStrength = (100.0f - sqrtf(SQ(dx) + SQ(dy) + SQ(dz))) * 0.05f;

                        self->actionState = ENKANBAN_AIR;
                        self->actor.gravity = -1.0f;
                        self->actor.world.rot.y = Math_FAtan2F(dx, dz) * (0x8000 / M_PI);
                        if (self->partCount >= 4) {
                            self->bounceX = (s16)Rand_ZeroFloat(10.0f) + 6;
                            self->bounceZ = (s16)Rand_ZeroFloat(10.0f) + 6;
                            self->actor.velocity.y = 2.5f + bombStrength;
                            self->actor.speedXZ = 3.0f + bombStrength;
                        } else {
                            self->bounceX = (s16)Rand_ZeroFloat(7.0f) + 3;
                            self->bounceZ = (s16)Rand_ZeroFloat(7.0f) + 3;
                            self->actor.velocity.y = 5.0f + bombStrength;
                            self->actor.speedXZ = 4.0f + bombStrength;
                        }
                        self->spinVel.y = Rand_CenteredFloat(0x1800);
                        if (Rand_ZeroOne() < 0.5f) {
                            self->direction = 1;
                        } else {
                            self->direction = -1;
                        }
                        self->airTimer = 70;
                    }
                    bomb = bomb->next;
                }
            }
            osSyncPrintf(VT_FGCOL(GREEN));
            osSyncPrintf("OCARINA_MODE %d\n", globalCtx->msgCtx.unk_E3EE);
            osSyncPrintf(VT_RST);
            switch (self->ocarinaFlag) {
                case 0:
                    if (globalCtx->msgCtx.unk_E3EE == 1) {
                        self->ocarinaFlag = 1;
                    }
                    break;
                case 1:
                    if ((globalCtx->msgCtx.unk_E3EE == 4) && (globalCtx->msgCtx.unk_E3F2 == 8)) {
                        self->actionState = ENKANBAN_REPAIR;
                        self->bounceX = 1;
                        Audio_PlaySoundGeneral(NA_SE_SY_TRE_BOX_APPEAR, &D_801333D4, 4, &D_801333E0, &D_801333E0,
                                               &D_801333E8);
                    }
                    break;
            }
            break;
        case ENKANBAN_REPAIR: {
            f32 distX;
            f32 distY;
            f32 distZ;
            s16 pDiff;
            s16 yDiff;
            s16 rDiff;
            signpost = (EnKanban*)self->actor.parent;

            if (signpost->partFlags == 0xFFFF) {
                Actor_Kill(&self->actor);
            }

            Matrix_RotateY((signpost->actor.shape.rot.y / (f32)0x8000) * M_PI, MTXMODE_NEW);
            Matrix_MultVec3f(&sPieceOffsets[self->pieceType], &offset);
            distX =
                Math_SmoothStepToF(&self->actor.world.pos.x, signpost->actor.world.pos.x + offset.x, 1.0f, 3.0f, 0.0f);
            distY =
                Math_SmoothStepToF(&self->actor.world.pos.y, signpost->actor.world.pos.y + offset.y, 1.0f, 3.0f, 0.0f);
            distZ =
                Math_SmoothStepToF(&self->actor.world.pos.z, signpost->actor.world.pos.z + offset.z, 1.0f, 3.0f, 0.0f);
            pDiff = Math_SmoothStepToS(&self->actor.shape.rot.x, signpost->actor.shape.rot.x, 1, 0x200, 0);
            yDiff = Math_SmoothStepToS(&self->actor.shape.rot.y, signpost->actor.shape.rot.y, 1, 0x200, 0);
            rDiff = Math_SmoothStepToS(&self->actor.shape.rot.z, signpost->actor.shape.rot.z, 1, 0x200, 0);
            Math_ApproachS(&self->spinRot.x, 0, 1, 0x200);
            Math_ApproachS(&self->spinRot.z, 0, 1, 0x200);
            Math_ApproachZeroF(&self->floorRot.x, 1.0f, 0.05f);
            Math_ApproachZeroF(&self->floorRot.z, 1.0f, 0.05f);
            Math_ApproachZeroF(&self->actor.shape.yOffset, 1.0f, 2.0f);
            if (((distX + distY + distZ) == 0.0f) &&
                ((pDiff + yDiff + rDiff + self->spinRot.x + self->spinRot.z) == 0) && (self->floorRot.x == 0.0f) &&
                (self->floorRot.z == 0.0f)) {
                signpost->partFlags |= self->partFlags;
                signpost->actor.flags |= 1;
                Actor_Kill(&self->actor);
            }
        } break;
    }
}

static Gfx* sDisplayLists[] = {
    object_kanban_DL_000CB0, object_kanban_DL_000DB8, object_kanban_DL_000E78, object_kanban_DL_000F38,
    object_kanban_DL_000FF8, object_kanban_DL_0010B8, object_kanban_DL_0011C0, object_kanban_DL_0012C8,
    object_kanban_DL_0013D0, object_kanban_DL_001488, object_kanban_DL_001540,
};

#include "z_en_kanban_gfx.c"

static f32 sCutAngles[] = {
    /* CUT_POST   */ 0.50f * M_PI,
    /* CUT_VERT_L */ 0.00f * M_PI,
    /* CUT_HORIZ  */ 0.50f * M_PI,
    /* CUT_DIAG_L */ 0.66f * M_PI,
    /* CUT_DIAG_R */ 0.34f * M_PI,
    /* CUT_VERT_R */ 0.00f * M_PI,
};

static s32 sUnused[] = { 0, 0, 0 }; // Unused zero vector?

static Vtx sShadowVertices[] = {
    VTX(-2000, 0, 0, 0, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(2000, 0, 0, 1024, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(2000, 6000, 0, 1024, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(-2000, 6000, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF),
};

static Gfx sShadowDList[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(0x08000000, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(PRIMITIVE, 0, TEXEL0, 0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_OVL_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPVertex(sShadowVertices, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

void EnKanban_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnKanban* self = THIS;
    f32 zShift;
    f32 zShift2;
    s16 i;
    u8* shadowTex = Graph_Alloc(globalCtx->state.gfxCtx, 0x400);

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_kanban.c", 1659);
    func_80093D18(globalCtx->state.gfxCtx);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, object_kanban_DL_000C30);
    if (self->actionState != ENKANBAN_SIGN) {
        Matrix_Translate(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z, MTXMODE_NEW);
        Matrix_Scale(self->actor.scale.x, self->actor.scale.y, self->actor.scale.z, MTXMODE_APPLY);
        Matrix_RotateX(self->floorRot.x, MTXMODE_APPLY);
        Matrix_RotateZ(self->floorRot.z, MTXMODE_APPLY);
        Matrix_Translate(0.0f, self->actor.shape.yOffset, 0.0f, MTXMODE_APPLY);
        Matrix_RotateY((self->actor.shape.rot.y / (f32)0x8000) * M_PI, MTXMODE_APPLY);
        Matrix_RotateX((self->actor.shape.rot.x / (f32)0x8000) * M_PI, MTXMODE_APPLY);
        zShift = fabsf(Math_SinS(self->spinRot.x) * self->pieceHeight);
        zShift2 = fabsf(Math_SinS(self->spinRot.z) * self->pieceWidth);
        zShift = MAX(zShift2, zShift);
        zShift *= -(f32)self->direction;
        Matrix_Translate(0.0f, 0.0f, zShift, MTXMODE_APPLY);
        Matrix_RotateX((self->spinRot.x / (f32)0x8000) * M_PI, MTXMODE_APPLY);
        Matrix_RotateY((self->spinRot.z / (f32)0x8000) * M_PI, MTXMODE_APPLY);
        Matrix_Translate(self->offset.x, self->offset.y, self->offset.z - 100.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_kanban.c", 1715),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        for (i = 0; i < ARRAY_COUNT(sPartFlags); i++) {
            if (sPartFlags[i] & self->partFlags) {
                gSPDisplayList(POLY_OPA_DISP++, sDisplayLists[i]);
            }
        }
    } else {
        Matrix_Translate(0.0f, 0.0f, -100.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_kanban.c", 1725),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        if (self->partFlags == 0xFFFF) {
            gSPDisplayList(POLY_OPA_DISP++, gSignRectangularDL);
        } else {
            for (i = 0; i < ARRAY_COUNT(sPartFlags); i++) {
                if (sPartFlags[i] & self->partFlags) {
                    gSPDisplayList(POLY_OPA_DISP++, sDisplayLists[i]);
                }
            }
        }
        if (self->cutMarkAlpha != 0) {
            f32 cutOffset = (self->cutType == CUT_POST) ? -1200.0f : 0.0f;

            Matrix_Translate(0.0f, 4400.0f + cutOffset, 200.0f, MTXMODE_APPLY);
            Matrix_RotateZ(sCutAngles[self->cutType], MTXMODE_APPLY);
            Matrix_Scale(0.0f, 10.0f, 2.0f, MTXMODE_APPLY);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, 255, 255, 255, self->cutMarkAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 150, 0);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_kanban.c", 1773),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_kanban_DL_001630);
        }
    }
    if ((self->actor.projectedPos.z <= 400.0f) && (self->actor.projectedPos.z > 0.0f) &&
        (self->actor.floorHeight > -3000.0f)) {
        if ((self->bounceX != 0) || (self->bounceZ != 0)) {
            u16 dayTime = gSaveContext.dayTime;
            f32 shadowAlpha;

            if (dayTime >= 0x8000) {
                dayTime = 0xFFFF - dayTime;
            }
            shadowAlpha = (dayTime * 0.00275f) + 10.0f;
            if (self->actor.projectedPos.z > 300.0f) {
                shadowAlpha *= ((400.0f - self->actor.projectedPos.z) * 0.01f);
            }
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, 0, 0, 0, (s8)shadowAlpha);

            if ((self->actionState == ENKANBAN_SIGN) && LINK_IS_CHILD) {
                zShift = 0.0f;
            } else {
                zShift = ((self->actor.world.pos.y - self->actor.floorHeight) * -50.0f) / 100.0f;
            }

            Matrix_Translate(self->actor.world.pos.x, self->actor.floorHeight, self->actor.world.pos.z + zShift,
                             MTXMODE_NEW);
            Matrix_RotateX(self->floorRot.x, MTXMODE_APPLY);
            Matrix_RotateZ(self->floorRot.z, MTXMODE_APPLY);
            Matrix_Scale(self->actor.scale.x, 0.0f, self->actor.scale.z, MTXMODE_APPLY);
            if (self->actionState == ENKANBAN_SIGN) {
                Matrix_RotateX(-M_PI / 5, MTXMODE_APPLY);
            }
            Matrix_RotateY((self->actor.shape.rot.y / (f32)0x8000) * M_PI, MTXMODE_APPLY);
            Matrix_RotateX((self->actor.shape.rot.x / (f32)0x8000) * M_PI, MTXMODE_APPLY);
            Matrix_RotateX((self->spinRot.x / (f32)0x8000) * M_PI, MTXMODE_APPLY);
            Matrix_RotateY((self->spinRot.z / (f32)0x8000) * M_PI, MTXMODE_APPLY);
            Matrix_Translate(self->offset.x, self->offset.y, self->offset.z, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_kanban.c", 1833),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            for (i = 0; i < 0x400; i++) {
                if (sShadowTexFlags[i] & self->partFlags) {
                    shadowTex[i] = 0xFF;
                } else {
                    shadowTex[i] = 0;
                }
            }
            gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(shadowTex));
            gSPDisplayList(POLY_XLU_DISP++, sShadowDList);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_kanban.c", 1857);
}
