/*
 * File: z_en_bird.c
 * Overlay: ovl_En_Bird
 * Description: An unused brown bird
 */

#include "z_en_bird.h"

#define FLAGS 0x00000030

#define THIS ((EnBird*)thisx)

void EnBird_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Draw(Actor* thisx, GlobalContext* globalCtx);


const ActorInit En_Bird_InitVars = {
    ACTOR_EN_BIRD,  ACTORCAT_PROP,          FLAGS, OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBird), (ActorFunc)EnBird_Init, NULL,  (ActorFunc)EnBird_Update,
    NULL,
};

void EnBird_Init(Actor* thisx, GlobalContext* globalCtx) {

}

void EnBird_Update(Actor* thisx, GlobalContext* globalCtx){
    GfxPrint printer;
    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer,globalCtx->state.gfxCtx->polyOpa.p);
    GfxPrint_SetPos(&printer,0,0);
    GfxPrint_Printf(&printer,"\nHello from the magic bird\n");
    globalCtx->state.gfxCtx->polyOpa.p = GfxPrint_Close(&printer);
    GfxPrint_Destroy(&printer);
    
    switch (thisx->params) {
        case 0:
            break;
        case 1:
            globalCtx->nextEntranceIndex = 0x040B;
            globalCtx->sceneLoadFlag = 0x14;
            globalCtx->fadeTransition = 1;
            gSaveContext.nextTransition = 1;
    }
}