/*
 * File: z_en_wall_tubo.c
 * Overlay: ovl_En_Wall_Tubo
 * Description: Bombchu Bowling Alley Bullseyes/Pits
 */

#include "z_en_wall_tubo.h"
#include "vt.h"
#include "overlays/actors/ovl_En_Bom_Chu/z_en_bom_chu.h"
#include "overlays/actors/ovl_Bg_Bowl_Wall/z_bg_bowl_wall.h"
#include "overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.h"

#define FLAGS 0x00000010

#define THIS ((EnWallTubo*)thisx)

void EnWallTubo_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWallTubo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWallTubo_Update(Actor* thisx, GlobalContext* globalCtx);

void EnWallTubo_FindGirl(EnWallTubo* self, GlobalContext* globalCtx);
void EnWallTubo_DetectChu(EnWallTubo* self, GlobalContext* globalCtx);
void EnWallTubo_SetWallFall(EnWallTubo* self, GlobalContext* globalCtx);

const ActorInit En_Wall_Tubo_InitVars = {
    ACTOR_EN_WALL_TUBO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWallTubo),
    (ActorFunc)EnWallTubo_Init,
    (ActorFunc)EnWallTubo_Destroy,
    (ActorFunc)EnWallTubo_Update,
    NULL,
};

void EnWallTubo_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnWallTubo* self = THIS;

    osSyncPrintf("\n\n");
    // "Wall Target"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 壁のツボ ☆☆☆☆☆ \n" VT_RST);
    self->unk_164 = self->actor.world.pos;
    self->actionFunc = EnWallTubo_FindGirl;
}

void EnWallTubo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnWallTubo_FindGirl(EnWallTubo* self, GlobalContext* globalCtx) {
    Actor* lookForGirl;

    lookForGirl = globalCtx->actorCtx.actorLists[ACTORCAT_NPC].head;

    while (lookForGirl != NULL) {
        if (lookForGirl->id != ACTOR_EN_BOM_BOWL_MAN) {
            lookForGirl = lookForGirl->next;
        } else {
            self->chuGirl = (EnBomBowlMan*)lookForGirl;
            break;
        }
    }

    self->actionFunc = EnWallTubo_DetectChu;
}

void EnWallTubo_DetectChu(EnWallTubo* self, GlobalContext* globalCtx) {
    EnBomChu* chu;
    s32 pad;
    Vec3f effAccel = { 0.0f, 0.1f, 0.0f };
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f chuPosDiff;
    s16 quakeIndex;

    if (self->chuGirl->minigamePlayStatus != 0) {
        if (globalCtx->cameraPtrs[MAIN_CAM]->setting == CAM_SET_FIXED1) {
            chu = (EnBomChu*)globalCtx->actorCtx.actorLists[ACTORCAT_EXPLOSIVE].head;

            while (chu != NULL) {
                if ((&chu->actor == &self->actor) || (chu->actor.id != ACTOR_EN_BOM_CHU)) {
                    chu = (EnBomChu*)chu->actor.next;
                    continue;
                }

                chuPosDiff.x = chu->actor.world.pos.x - self->actor.world.pos.x;
                chuPosDiff.y = chu->actor.world.pos.y - self->actor.world.pos.y;
                chuPosDiff.z = chu->actor.world.pos.z - self->actor.world.pos.z;

                if (((fabsf(chuPosDiff.x) < 40.0f) || (BREG(2))) && ((fabsf(chuPosDiff.y) < 40.0f) || (BREG(2))) &&
                    (fabsf(chuPosDiff.z) < 40.0f || (BREG(2)))) {
                    self->chuGirl->wallStatus[self->actor.params] = 1;
                    chu->timer = 2;
                    func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
                    self->timer = 60;
                    EffectSsBomb2_SpawnLayered(globalCtx, &self->explosionCenter, &effVelocity, &effAccel, 200, 40);
                    quakeIndex = Quake_Add(GET_ACTIVE_CAM(globalCtx), 1);
                    Quake_SetSpeed(quakeIndex, 0x7FFF);
                    Quake_SetQuakeValues(quakeIndex, 100, 0, 0, 0);
                    Quake_SetCountdown(quakeIndex, 100);
                    self->actionFunc = EnWallTubo_SetWallFall;
                    break;
                }

                chu = (EnBomChu*)chu->actor.next;
            }
        }
    }
}

void EnWallTubo_SetWallFall(EnWallTubo* self, GlobalContext* globalCtx) {
    BgBowlWall* wall;
    Vec3f effAccel = { 0.0f, 0.1f, 0.0f };
    Vec3f effVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3f effPos;

    if ((globalCtx->gameplayFrames & 1) == 0) {
        effPos.x = self->explosionCenter.x + Rand_CenteredFloat(300.0f);
        effPos.y = self->explosionCenter.y + Rand_CenteredFloat(300.0f);
        effPos.z = self->explosionCenter.z;
        EffectSsBomb2_SpawnLayered(globalCtx, &effPos, &effVelocity, &effAccel, 100, 30);
        EffectSsHahen_SpawnBurst(globalCtx, &effPos, 10.0f, 0, 50, 15, 3, HAHEN_OBJECT_DEFAULT, 10, NULL);
        Audio_PlayActorSound2(&self->actor, NA_SE_IT_BOMB_EXPLOSION);
    }

    if (self->timer == 0) {
        wall = (BgBowlWall*)self->actor.parent;

        if ((wall != NULL) && (wall->dyna.actor.update != NULL)) {
            wall->isHit = true;
            // "You did it field!" (repeated 5 times)
            osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆ やった原！ ☆☆☆☆☆ \n" VT_RST);
            osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆ やった原！ ☆☆☆☆☆ \n" VT_RST);
            osSyncPrintf(VT_FGCOL(BLUE) "☆☆☆☆ やった原！ ☆☆☆☆☆ \n" VT_RST);
            osSyncPrintf(VT_FGCOL(PURPLE) "☆☆☆☆ やった原！ ☆☆☆☆☆ \n" VT_RST);
            osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆ やった原！ ☆☆☆☆☆ \n" VT_RST);
        }

        Actor_Kill(&self->actor);
    }
}

void EnWallTubo_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnWallTubo* self = THIS;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actionFunc(self, globalCtx);

    if (BREG(0)) {
        DebugDisplay_AddObject(self->actor.world.pos.x, self->actor.world.pos.y, self->actor.world.pos.z,
                               self->actor.world.rot.x, self->actor.world.rot.y, self->actor.world.rot.z, 1.0f, 1.0f,
                               1.0f, 0, 0, 255, 255, 4, globalCtx->state.gfxCtx);
    }
}
