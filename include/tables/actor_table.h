/**
 * Actor Table
 *
 * DEFINE_ACTOR should be used for normal actors
 *    - Argument 1: Name of the actor (without the ovl_ part)
 *    - Argument 2: Enum value for this actor
 *    - Argument 3: Allocation type (normal, permanent or absolute)
 *
 * DEFINE_ACTOR_INTERNAL should be used for actors that aren't an overlay, with the same arguments as DEFINE_ACTOR
 *
 * DEFINE_ACTOR_UNSET is needed to define empty entries from the original game
 */
/* 0x0000 */ DEFINE_ACTOR_INTERNAL(Player, ACTOR_PLAYER, ALLOCTYPE_NORMAL)

/* 0x000A */ DEFINE_ACTOR(En_Box, ACTOR_EN_BOX, ALLOCTYPE_NORMAL) ///////////////

/* 0x0018 */ DEFINE_ACTOR(En_Elf, ACTOR_EN_ELF, ALLOCTYPE_NORMAL) //////////////////

/* 0x00E7 */ DEFINE_ACTOR(En_Ma1, ACTOR_EN_MA1, ALLOCTYPE_NORMAL) //////////////////

/* 0x0157 */ DEFINE_ACTOR(Bg_Jya_1flift, ACTOR_BG_JYA_1FLIFT, ALLOCTYPE_NORMAL)//////////////
