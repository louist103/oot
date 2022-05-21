/*
 * File: z_en_dha.c
 * Overlay: ovl_En_Dha
 * Description: Dead Hand's Hand
 */

#include "z_en_dha.h"
#include "overlays/actors/ovl_En_Dh/z_en_dh.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_2 | ACTOR_FLAG_4)

void EnDha_Init(Actor* thisx, PlayState* play);
void EnDha_Destroy(Actor* thisx, PlayState* play);
void EnDha_Update(Actor* thisx, PlayState* play);
void EnDha_Draw(Actor* thisx, PlayState* play);

/*
const ActorInit En_Dha_InitVars = {
    ACTOR_EN_DHA,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DH,
    sizeof(EnDha),
    (ActorFunc)EnDha_Init,
    (ActorFunc)EnDha_Destroy,
    (ActorFunc)EnDha_Update,
    (ActorFunc)EnDha_Draw,
};
*/
#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809EC880.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/EnDha_Init.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/EnDha_Destroy.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809EC9C8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809ECA50.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809ECF60.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809ECF8C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809ED054.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809ED0C4.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809ED27C.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/EnDha_Update.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809ED414.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/func_809ED4B8.s")

#pragma GLOBAL_ASM("asm/non_matchings/overlays/actors/ovl_En_Dha/EnDha_Draw.s")
