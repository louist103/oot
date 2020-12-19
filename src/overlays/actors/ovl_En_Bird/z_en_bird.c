/*
 * File: z_en_bird.c
 * Overlay: ovl_En_Bird
 * Description: An unused brown bird
 */

#include "z_en_bird.h"

#define FLAGS 0x00000000

#define THIS ((EnBird*)thisx)

void EnBird_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBird_Draw(Actor* thisx, GlobalContext* globalCtx);



const ActorInit En_Bird_InitVars = {
    ACTOR_EN_BIRD,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_BIRD,
    sizeof(EnBird),
    (ActorFunc)EnBird_Init,
    (ActorFunc)EnBird_Destroy,
    (ActorFunc)EnBird_Update,
    (ActorFunc)EnBird_Draw,
};





void EnBird_Init(Actor* thisx, GlobalContext* globalCtx) {

}

void EnBird_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}



void EnBird_Update(Actor* thisx, GlobalContext* globalCtx) {

}
static Gfx* gfx[] = {
    0x04039C00, 0x04039C00, 0x04039C00, 0x0403A2D0, 0x0403A2D0, 0x0403A630,
    0x06000210, 0x0403AB80, 0x0403A9B0, 0x0403C050, 0x0403C5B0, 0x0400D340,
};

void EnBird_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBird* this = THIS;


}
