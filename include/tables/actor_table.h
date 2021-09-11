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
/* 0x0001 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_1)
/* 0x0002 */ DEFINE_ACTOR(En_Test, ACTOR_EN_TEST, ALLOCTYPE_NORMAL)
/* 0x0003 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_3)
/* 0x0004 */ DEFINE_ACTOR(En_GirlA, ACTOR_EN_GIRLA, ALLOCTYPE_NORMAL)
/* 0x0005 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_5)
/* 0x0006 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_6)
/* 0x0007 */ DEFINE_ACTOR(En_Part, ACTOR_EN_PART, ALLOCTYPE_NORMAL)
/* 0x0008 */ DEFINE_ACTOR(En_Light, ACTOR_EN_LIGHT, ALLOCTYPE_NORMAL)
/* 0x0009 */ DEFINE_ACTOR(En_Door, ACTOR_EN_DOOR, ALLOCTYPE_PERMANENT)
/* 0x000A */ DEFINE_ACTOR(En_Box, ACTOR_EN_BOX, ALLOCTYPE_NORMAL)
/* 0x000B */ DEFINE_ACTOR(Bg_Dy_Yoseizo, ACTOR_BG_DY_YOSEIZO, ALLOCTYPE_NORMAL)
/* 0x000C */ DEFINE_ACTOR(Bg_Hidan_Firewall, ACTOR_BG_HIDAN_FIREWALL, ALLOCTYPE_NORMAL)
/* 0x000D */ DEFINE_ACTOR(En_Poh, ACTOR_EN_POH, ALLOCTYPE_NORMAL)
/* 0x000E */ DEFINE_ACTOR(En_Okuta, ACTOR_EN_OKUTA, ALLOCTYPE_NORMAL)
/* 0x000F */ DEFINE_ACTOR(Bg_Ydan_Sp, ACTOR_BG_YDAN_SP, ALLOCTYPE_NORMAL)
/* 0x0010 */ DEFINE_ACTOR(En_Bom, ACTOR_EN_BOM, ALLOCTYPE_PERMANENT)
/* 0x0011 */ DEFINE_ACTOR(En_Wallmas, ACTOR_EN_WALLMAS, ALLOCTYPE_NORMAL)
/* 0x0012 */ DEFINE_ACTOR(En_Dodongo, ACTOR_EN_DODONGO, ALLOCTYPE_NORMAL)
/* 0x0013 */ DEFINE_ACTOR(En_Firefly, ACTOR_EN_FIREFLY, ALLOCTYPE_NORMAL)
/* 0x0014 */ DEFINE_ACTOR(En_Horse, ACTOR_EN_HORSE, ALLOCTYPE_NORMAL)
/* 0x0015 */ DEFINE_ACTOR_INTERNAL(En_Item00, ACTOR_EN_ITEM00, ALLOCTYPE_NORMAL)
/* 0x0016 */ DEFINE_ACTOR(En_Arrow, ACTOR_EN_ARROW, ALLOCTYPE_PERMANENT)
/* 0x0017 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_17)
/* 0x0018 */ DEFINE_ACTOR(En_Elf, ACTOR_EN_ELF, ALLOCTYPE_NORMAL)
/* 0x0019 */ DEFINE_ACTOR(En_Niw, ACTOR_EN_NIW, ALLOCTYPE_NORMAL)
/* 0x001A */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_1A)
/* 0x001B */ DEFINE_ACTOR(En_Tite, ACTOR_EN_TITE, ALLOCTYPE_NORMAL)
/* 0x001C */ DEFINE_ACTOR(En_Reeba, ACTOR_EN_REEBA, ALLOCTYPE_NORMAL)
/* 0x001D */ DEFINE_ACTOR(En_Peehat, ACTOR_EN_PEEHAT, ALLOCTYPE_NORMAL)
/* 0x001E */ DEFINE_ACTOR(En_Butte, ACTOR_EN_BUTTE, ALLOCTYPE_NORMAL)
/* 0x001F */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_1F)
/* 0x0020 */ DEFINE_ACTOR(En_Insect, ACTOR_EN_INSECT, ALLOCTYPE_NORMAL)
/* 0x0021 */ DEFINE_ACTOR(En_Fish, ACTOR_EN_FISH, ALLOCTYPE_NORMAL)
/* 0x0022 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_22)
/* 0x0023 */ DEFINE_ACTOR(En_Holl, ACTOR_EN_HOLL, ALLOCTYPE_PERMANENT)
/* 0x0024 */ DEFINE_ACTOR(En_Scene_Change, ACTOR_EN_SCENE_CHANGE, ALLOCTYPE_NORMAL)
/* 0x0025 */ DEFINE_ACTOR(En_Zf, ACTOR_EN_ZF, ALLOCTYPE_NORMAL)
/* 0x0026 */ DEFINE_ACTOR(En_Hata, ACTOR_EN_HATA, ALLOCTYPE_NORMAL)
/* 0x0027 */ DEFINE_ACTOR(Boss_Dodongo, ACTOR_BOSS_DODONGO, ALLOCTYPE_NORMAL)
/* 0x0028 */ DEFINE_ACTOR(Boss_Goma, ACTOR_BOSS_GOMA, ALLOCTYPE_NORMAL)
/* 0x0029 */ DEFINE_ACTOR(En_Zl1, ACTOR_EN_ZL1, ALLOCTYPE_NORMAL)
/* 0x002A */ DEFINE_ACTOR(En_Viewer, ACTOR_EN_VIEWER, ALLOCTYPE_NORMAL)
/* 0x002B */ DEFINE_ACTOR(En_Goma, ACTOR_EN_GOMA, ALLOCTYPE_NORMAL)
/* 0x002C */ DEFINE_ACTOR(Bg_Pushbox, ACTOR_BG_PUSHBOX, ALLOCTYPE_NORMAL)
/* 0x002D */ DEFINE_ACTOR(En_Bubble, ACTOR_EN_BUBBLE, ALLOCTYPE_NORMAL)
/* 0x002E */ DEFINE_ACTOR(Door_Shutter, ACTOR_DOOR_SHUTTER, ALLOCTYPE_PERMANENT)
/* 0x002F */ DEFINE_ACTOR(En_Dodojr, ACTOR_EN_DODOJR, ALLOCTYPE_NORMAL)
/* 0x0030 */ DEFINE_ACTOR(En_Bdfire, ACTOR_EN_BDFIRE, ALLOCTYPE_NORMAL)
/* 0x0031 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_31)
/* 0x0032 */ DEFINE_ACTOR(En_Boom, ACTOR_EN_BOOM, ALLOCTYPE_PERMANENT)
/* 0x0033 */ DEFINE_ACTOR(En_Torch2, ACTOR_EN_TORCH2, ALLOCTYPE_NORMAL)
/* 0x0034 */ DEFINE_ACTOR(En_Bili, ACTOR_EN_BILI, ALLOCTYPE_NORMAL)
/* 0x0035 */ DEFINE_ACTOR(En_Tp, ACTOR_EN_TP, ALLOCTYPE_NORMAL)
/* 0x0036 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_36)
/* 0x0037 */ DEFINE_ACTOR(En_St, ACTOR_EN_ST, ALLOCTYPE_NORMAL)
/* 0x0038 */ DEFINE_ACTOR(En_Bw, ACTOR_EN_BW, ALLOCTYPE_NORMAL)
/* 0x0039 */ DEFINE_ACTOR_INTERNAL(En_A_Obj, ACTOR_EN_A_OBJ, ALLOCTYPE_NORMAL)
/* 0x003A */ DEFINE_ACTOR(En_Eiyer, ACTOR_EN_EIYER, ALLOCTYPE_NORMAL)
/* 0x003B */ DEFINE_ACTOR(En_River_Sound, ACTOR_EN_RIVER_SOUND, ALLOCTYPE_NORMAL)
/* 0x003C */ DEFINE_ACTOR(En_Horse_Normal, ACTOR_EN_HORSE_NORMAL, ALLOCTYPE_NORMAL)
/* 0x003D */ DEFINE_ACTOR(En_Ossan, ACTOR_EN_OSSAN, ALLOCTYPE_NORMAL)
/* 0x003E */ DEFINE_ACTOR(Bg_Treemouth, ACTOR_BG_TREEMOUTH, ALLOCTYPE_NORMAL)
/* 0x003F */ DEFINE_ACTOR(Bg_Dodoago, ACTOR_BG_DODOAGO, ALLOCTYPE_NORMAL)
/* 0x0040 */ DEFINE_ACTOR(Bg_Hidan_Dalm, ACTOR_BG_HIDAN_DALM, ALLOCTYPE_NORMAL)
/* 0x0041 */ DEFINE_ACTOR(Bg_Hidan_Hrock, ACTOR_BG_HIDAN_HROCK, ALLOCTYPE_NORMAL)
/* 0x0042 */ DEFINE_ACTOR(En_Horse_Ganon, ACTOR_EN_HORSE_GANON, ALLOCTYPE_NORMAL)
/* 0x0043 */ DEFINE_ACTOR(Bg_Hidan_Rock, ACTOR_BG_HIDAN_ROCK, ALLOCTYPE_NORMAL)
/* 0x0044 */ DEFINE_ACTOR(Bg_Hidan_Rsekizou, ACTOR_BG_HIDAN_RSEKIZOU, ALLOCTYPE_NORMAL)
/* 0x0045 */ DEFINE_ACTOR(Bg_Hidan_Sekizou, ACTOR_BG_HIDAN_SEKIZOU, ALLOCTYPE_NORMAL)
/* 0x0046 */ DEFINE_ACTOR(Bg_Hidan_Sima, ACTOR_BG_HIDAN_SIMA, ALLOCTYPE_NORMAL)
/* 0x0047 */ DEFINE_ACTOR(Bg_Hidan_Syoku, ACTOR_BG_HIDAN_SYOKU, ALLOCTYPE_NORMAL)
/* 0x0048 */ DEFINE_ACTOR(En_Xc, ACTOR_EN_XC, ALLOCTYPE_NORMAL)
/* 0x0049 */ DEFINE_ACTOR(Bg_Hidan_Curtain, ACTOR_BG_HIDAN_CURTAIN, ALLOCTYPE_NORMAL)
/* 0x004A */ DEFINE_ACTOR(Bg_Spot00_Hanebasi, ACTOR_BG_SPOT00_HANEBASI, ALLOCTYPE_NORMAL)
/* 0x004B */ DEFINE_ACTOR(En_Mb, ACTOR_EN_MB, ALLOCTYPE_NORMAL)
/* 0x004C */ DEFINE_ACTOR(En_Bombf, ACTOR_EN_BOMBF, ALLOCTYPE_NORMAL)
/* 0x004D */ DEFINE_ACTOR(En_Zl2, ACTOR_EN_ZL2, ALLOCTYPE_NORMAL)
/* 0x004E */ DEFINE_ACTOR(Bg_Hidan_Fslift, ACTOR_BG_HIDAN_FSLIFT, ALLOCTYPE_NORMAL)
/* 0x004F */ DEFINE_ACTOR(En_OE2, ACTOR_EN_OE2, ALLOCTYPE_NORMAL)
/* 0x0050 */ DEFINE_ACTOR(Bg_Ydan_Hasi, ACTOR_BG_YDAN_HASI, ALLOCTYPE_NORMAL)
/* 0x0051 */ DEFINE_ACTOR(Bg_Ydan_Maruta, ACTOR_BG_YDAN_MARUTA, ALLOCTYPE_NORMAL)
/* 0x0052 */ DEFINE_ACTOR(Boss_Ganondrof, ACTOR_BOSS_GANONDROF, ALLOCTYPE_NORMAL)
/* 0x0053 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_53)
/* 0x0054 */ DEFINE_ACTOR(En_Am, ACTOR_EN_AM, ALLOCTYPE_NORMAL)
/* 0x0055 */ DEFINE_ACTOR(En_Dekubaba, ACTOR_EN_DEKUBABA, ALLOCTYPE_NORMAL)
/* 0x0056 */ DEFINE_ACTOR(En_M_Fire1, ACTOR_EN_M_FIRE1, ALLOCTYPE_PERMANENT)
/* 0x0057 */ DEFINE_ACTOR(En_M_Thunder, ACTOR_EN_M_THUNDER, ALLOCTYPE_PERMANENT)
/* 0x0058 */ DEFINE_ACTOR(Bg_Ddan_Jd, ACTOR_BG_DDAN_JD, ALLOCTYPE_NORMAL)
/* 0x0059 */ DEFINE_ACTOR(Bg_Breakwall, ACTOR_BG_BREAKWALL, ALLOCTYPE_NORMAL)
/* 0x005A */ DEFINE_ACTOR(En_Jj, ACTOR_EN_JJ, ALLOCTYPE_NORMAL)
/* 0x005B */ DEFINE_ACTOR(En_Horse_Zelda, ACTOR_EN_HORSE_ZELDA, ALLOCTYPE_NORMAL)
/* 0x005C */ DEFINE_ACTOR(Bg_Ddan_Kd, ACTOR_BG_DDAN_KD, ALLOCTYPE_NORMAL)
/* 0x005D */ DEFINE_ACTOR(Door_Warp1, ACTOR_DOOR_WARP1, ALLOCTYPE_NORMAL)
/* 0x005E */ DEFINE_ACTOR(Obj_Syokudai, ACTOR_OBJ_SYOKUDAI, ALLOCTYPE_NORMAL)
/* 0x005F */ DEFINE_ACTOR(Item_B_Heart, ACTOR_ITEM_B_HEART, ALLOCTYPE_NORMAL)
/* 0x0060 */ DEFINE_ACTOR(En_Dekunuts, ACTOR_EN_DEKUNUTS, ALLOCTYPE_NORMAL)
/* 0x0061 */ DEFINE_ACTOR(Bg_Menkuri_Kaiten, ACTOR_BG_MENKURI_KAITEN, ALLOCTYPE_NORMAL)
/* 0x0062 */ DEFINE_ACTOR(Bg_Menkuri_Eye, ACTOR_BG_MENKURI_EYE, ALLOCTYPE_NORMAL)
/* 0x0063 */ DEFINE_ACTOR(En_Vali, ACTOR_EN_VALI, ALLOCTYPE_NORMAL)
/* 0x0064 */ DEFINE_ACTOR(Bg_Mizu_Movebg, ACTOR_BG_MIZU_MOVEBG, ALLOCTYPE_NORMAL)
/* 0x0065 */ DEFINE_ACTOR(Bg_Mizu_Water, ACTOR_BG_MIZU_WATER, ALLOCTYPE_NORMAL)
/* 0x0066 */ DEFINE_ACTOR(Arms_Hook, ACTOR_ARMS_HOOK, ALLOCTYPE_PERMANENT)
/* 0x0067 */ DEFINE_ACTOR(En_fHG, ACTOR_EN_FHG, ALLOCTYPE_NORMAL)
/* 0x0068 */ DEFINE_ACTOR(Bg_Mori_Hineri, ACTOR_BG_MORI_HINERI, ALLOCTYPE_NORMAL)
/* 0x0069 */ DEFINE_ACTOR(En_Bb, ACTOR_EN_BB, ALLOCTYPE_NORMAL)
/* 0x006A */ DEFINE_ACTOR(Bg_Toki_Hikari, ACTOR_BG_TOKI_HIKARI, ALLOCTYPE_NORMAL)
/* 0x006B */ DEFINE_ACTOR(En_Yukabyun, ACTOR_EN_YUKABYUN, ALLOCTYPE_NORMAL)
/* 0x006C */ DEFINE_ACTOR(Bg_Toki_Swd, ACTOR_BG_TOKI_SWD, ALLOCTYPE_NORMAL)
/* 0x006D */ DEFINE_ACTOR(En_Fhg_Fire, ACTOR_EN_FHG_FIRE, ALLOCTYPE_NORMAL)
/* 0x006E */ DEFINE_ACTOR(Bg_Mjin, ACTOR_BG_MJIN, ALLOCTYPE_NORMAL)
/* 0x006F */ DEFINE_ACTOR(Bg_Hidan_Kousi, ACTOR_BG_HIDAN_KOUSI, ALLOCTYPE_NORMAL)
/* 0x0070 */ DEFINE_ACTOR(Door_Toki, ACTOR_DOOR_TOKI, ALLOCTYPE_NORMAL)
/* 0x0071 */ DEFINE_ACTOR(Bg_Hidan_Hamstep, ACTOR_BG_HIDAN_HAMSTEP, ALLOCTYPE_NORMAL)
/* 0x0072 */ DEFINE_ACTOR(En_Bird, ACTOR_EN_BIRD, ALLOCTYPE_NORMAL)
/* 0x0073 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_73)
/* 0x0074 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_74)
/* 0x0075 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_75)
/* 0x0076 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_76)
/* 0x0077 */ DEFINE_ACTOR(En_Wood02, ACTOR_EN_WOOD02, ALLOCTYPE_NORMAL)
/* 0x0078 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_78)
/* 0x0079 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_79)
/* 0x007A */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_7A)
/* 0x007B */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_7B)
/* 0x007C */ DEFINE_ACTOR(En_Lightbox, ACTOR_EN_LIGHTBOX, ALLOCTYPE_NORMAL)
/* 0x007D */ DEFINE_ACTOR(En_Pu_box, ACTOR_EN_PU_BOX, ALLOCTYPE_NORMAL)
/* 0x007E */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_7E)
/* 0x007F */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_7F)
/* 0x0080 */ DEFINE_ACTOR(En_Trap, ACTOR_EN_TRAP, ALLOCTYPE_NORMAL)
/* 0x0081 */ DEFINE_ACTOR(En_Arow_Trap, ACTOR_EN_AROW_TRAP, ALLOCTYPE_NORMAL)
/* 0x0082 */ DEFINE_ACTOR(En_Vase, ACTOR_EN_VASE, ALLOCTYPE_NORMAL)
/* 0x0083 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_83)
/* 0x0084 */ DEFINE_ACTOR(En_Ta, ACTOR_EN_TA, ALLOCTYPE_NORMAL)
/* 0x0085 */ DEFINE_ACTOR(En_Tk, ACTOR_EN_TK, ALLOCTYPE_NORMAL)
/* 0x0086 */ DEFINE_ACTOR(Bg_Mori_Bigst, ACTOR_BG_MORI_BIGST, ALLOCTYPE_NORMAL)
/* 0x0087 */ DEFINE_ACTOR(Bg_Mori_Elevator, ACTOR_BG_MORI_ELEVATOR, ALLOCTYPE_NORMAL)
/* 0x0088 */ DEFINE_ACTOR(Bg_Mori_Kaitenkabe, ACTOR_BG_MORI_KAITENKABE, ALLOCTYPE_NORMAL)
/* 0x0089 */ DEFINE_ACTOR(Bg_Mori_Rakkatenjo, ACTOR_BG_MORI_RAKKATENJO, ALLOCTYPE_NORMAL)
/* 0x008A */ DEFINE_ACTOR(En_Vm, ACTOR_EN_VM, ALLOCTYPE_NORMAL)
/* 0x008B */ DEFINE_ACTOR(Demo_Effect, ACTOR_DEMO_EFFECT, ALLOCTYPE_NORMAL)
/* 0x008C */ DEFINE_ACTOR(Demo_Kankyo, ACTOR_DEMO_KANKYO, ALLOCTYPE_NORMAL)
/* 0x008D */ DEFINE_ACTOR(Bg_Hidan_Fwbig, ACTOR_BG_HIDAN_FWBIG, ALLOCTYPE_NORMAL)
/* 0x008E */ DEFINE_ACTOR(En_Floormas, ACTOR_EN_FLOORMAS, ALLOCTYPE_NORMAL)
/* 0x008F */ DEFINE_ACTOR(En_Heishi1, ACTOR_EN_HEISHI1, ALLOCTYPE_NORMAL)
/* 0x0090 */ DEFINE_ACTOR(En_Rd, ACTOR_EN_RD, ALLOCTYPE_NORMAL)
/* 0x0091 */ DEFINE_ACTOR(En_Po_Sisters, ACTOR_EN_PO_SISTERS, ALLOCTYPE_NORMAL)
/* 0x0092 */ DEFINE_ACTOR(Bg_Heavy_Block, ACTOR_BG_HEAVY_BLOCK, ALLOCTYPE_NORMAL)
/* 0x0093 */ DEFINE_ACTOR(Bg_Po_Event, ACTOR_BG_PO_EVENT, ALLOCTYPE_NORMAL)
/* 0x0094 */ DEFINE_ACTOR(Obj_Mure, ACTOR_OBJ_MURE, ALLOCTYPE_NORMAL)
/* 0x0095 */ DEFINE_ACTOR(En_Sw, ACTOR_EN_SW, ALLOCTYPE_NORMAL)
/* 0x0096 */ DEFINE_ACTOR(Boss_Fd, ACTOR_BOSS_FD, ALLOCTYPE_NORMAL)
/* 0x0097 */ DEFINE_ACTOR(Object_Kankyo, ACTOR_OBJECT_KANKYO, ALLOCTYPE_NORMAL)
/* 0x0098 */ DEFINE_ACTOR(En_Du, ACTOR_EN_DU, ALLOCTYPE_NORMAL)
/* 0x0099 */ DEFINE_ACTOR(En_Fd, ACTOR_EN_FD, ALLOCTYPE_NORMAL)
/* 0x009A */ DEFINE_ACTOR(En_Horse_Link_Child, ACTOR_EN_HORSE_LINK_CHILD, ALLOCTYPE_NORMAL)
/* 0x009B */ DEFINE_ACTOR(Door_Ana, ACTOR_DOOR_ANA, ALLOCTYPE_NORMAL)
/* 0x009C */ DEFINE_ACTOR(Bg_Spot02_Objects, ACTOR_BG_SPOT02_OBJECTS, ALLOCTYPE_NORMAL)
/* 0x009D */ DEFINE_ACTOR(Bg_Haka, ACTOR_BG_HAKA, ALLOCTYPE_NORMAL)
/* 0x009E */ DEFINE_ACTOR(Magic_Wind, ACTOR_MAGIC_WIND, ALLOCTYPE_ABSOLUTE)
/* 0x009F */ DEFINE_ACTOR(Magic_Fire, ACTOR_MAGIC_FIRE, ALLOCTYPE_ABSOLUTE)
/* 0x00A0 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_A0)
/* 0x00A1 */ DEFINE_ACTOR(En_Ru1, ACTOR_EN_RU1, ALLOCTYPE_NORMAL)
/* 0x00A2 */ DEFINE_ACTOR(Boss_Fd2, ACTOR_BOSS_FD2, ALLOCTYPE_NORMAL)
/* 0x00A3 */ DEFINE_ACTOR(En_Fd_Fire, ACTOR_EN_FD_FIRE, ALLOCTYPE_NORMAL)
/* 0x00A4 */ DEFINE_ACTOR(En_Dh, ACTOR_EN_DH, ALLOCTYPE_NORMAL)
/* 0x00A5 */ DEFINE_ACTOR(En_Dha, ACTOR_EN_DHA, ALLOCTYPE_NORMAL)
/* 0x00A6 */ DEFINE_ACTOR(En_Rl, ACTOR_EN_RL, ALLOCTYPE_NORMAL)
/* 0x00A7 */ DEFINE_ACTOR(En_Encount1, ACTOR_EN_ENCOUNT1, ALLOCTYPE_NORMAL)
/* 0x00A8 */ DEFINE_ACTOR(Demo_Du, ACTOR_DEMO_DU, ALLOCTYPE_NORMAL)
/* 0x00A9 */ DEFINE_ACTOR(Demo_Im, ACTOR_DEMO_IM, ALLOCTYPE_NORMAL)
/* 0x00AA */ DEFINE_ACTOR(Demo_Tre_Lgt, ACTOR_DEMO_TRE_LGT, ALLOCTYPE_NORMAL)
/* 0x00AB */ DEFINE_ACTOR(En_Fw, ACTOR_EN_FW, ALLOCTYPE_NORMAL)
/* 0x00AC */ DEFINE_ACTOR(Bg_Vb_Sima, ACTOR_BG_VB_SIMA, ALLOCTYPE_NORMAL)
/* 0x00AD */ DEFINE_ACTOR(En_Vb_Ball, ACTOR_EN_VB_BALL, ALLOCTYPE_NORMAL)
/* 0x00AE */ DEFINE_ACTOR(Bg_Haka_Megane, ACTOR_BG_HAKA_MEGANE, ALLOCTYPE_NORMAL)
/* 0x00AF */ DEFINE_ACTOR(Bg_Haka_MeganeBG, ACTOR_BG_HAKA_MEGANEBG, ALLOCTYPE_NORMAL)
/* 0x00B0 */ DEFINE_ACTOR(Bg_Haka_Ship, ACTOR_BG_HAKA_SHIP, ALLOCTYPE_NORMAL)
/* 0x00B1 */ DEFINE_ACTOR(Bg_Haka_Sgami, ACTOR_BG_HAKA_SGAMI, ALLOCTYPE_NORMAL)
/* 0x00B2 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_B2)
/* 0x00B3 */ DEFINE_ACTOR(En_Heishi2, ACTOR_EN_HEISHI2, ALLOCTYPE_NORMAL)
/* 0x00B4 */ DEFINE_ACTOR(En_Encount2, ACTOR_EN_ENCOUNT2, ALLOCTYPE_NORMAL)
/* 0x00B5 */ DEFINE_ACTOR(En_Fire_Rock, ACTOR_EN_FIRE_ROCK, ALLOCTYPE_NORMAL)
/* 0x00B6 */ DEFINE_ACTOR(En_Brob, ACTOR_EN_BROB, ALLOCTYPE_NORMAL)
/* 0x00B7 */ DEFINE_ACTOR(Mir_Ray, ACTOR_MIR_RAY, ALLOCTYPE_NORMAL)
/* 0x00B8 */ DEFINE_ACTOR(Bg_Spot09_Obj, ACTOR_BG_SPOT09_OBJ, ALLOCTYPE_NORMAL)
/* 0x00B9 */ DEFINE_ACTOR(Bg_Spot18_Obj, ACTOR_BG_SPOT18_OBJ, ALLOCTYPE_NORMAL)
/* 0x00BA */ DEFINE_ACTOR(Boss_Va, ACTOR_BOSS_VA, ALLOCTYPE_NORMAL)
/* 0x00BB */ DEFINE_ACTOR(Bg_Haka_Tubo, ACTOR_BG_HAKA_TUBO, ALLOCTYPE_NORMAL)
/* 0x00BC */ DEFINE_ACTOR(Bg_Haka_Trap, ACTOR_BG_HAKA_TRAP, ALLOCTYPE_NORMAL)
/* 0x00BD */ DEFINE_ACTOR(Bg_Haka_Huta, ACTOR_BG_HAKA_HUTA, ALLOCTYPE_NORMAL)
/* 0x00BE */ DEFINE_ACTOR(Bg_Haka_Zou, ACTOR_BG_HAKA_ZOU, ALLOCTYPE_NORMAL)
/* 0x00BF */ DEFINE_ACTOR(Bg_Spot17_Funen, ACTOR_BG_SPOT17_FUNEN, ALLOCTYPE_NORMAL)
/* 0x00C0 */ DEFINE_ACTOR(En_Syateki_Itm, ACTOR_EN_SYATEKI_ITM, ALLOCTYPE_NORMAL)
/* 0x00C1 */ DEFINE_ACTOR(En_Syateki_Man, ACTOR_EN_SYATEKI_MAN, ALLOCTYPE_NORMAL)
/* 0x00C2 */ DEFINE_ACTOR(En_Tana, ACTOR_EN_TANA, ALLOCTYPE_NORMAL)
/* 0x00C3 */ DEFINE_ACTOR(En_Nb, ACTOR_EN_NB, ALLOCTYPE_NORMAL)
/* 0x00C4 */ DEFINE_ACTOR(Boss_Mo, ACTOR_BOSS_MO, ALLOCTYPE_NORMAL)
/* 0x00C5 */ DEFINE_ACTOR(En_Sb, ACTOR_EN_SB, ALLOCTYPE_NORMAL)
/* 0x00C6 */ DEFINE_ACTOR(En_Bigokuta, ACTOR_EN_BIGOKUTA, ALLOCTYPE_NORMAL)
/* 0x00C7 */ DEFINE_ACTOR(En_Karebaba, ACTOR_EN_KAREBABA, ALLOCTYPE_NORMAL)
/* 0x00C8 */ DEFINE_ACTOR(Bg_Bdan_Objects, ACTOR_BG_BDAN_OBJECTS, ALLOCTYPE_NORMAL)
/* 0x00C9 */ DEFINE_ACTOR(Demo_Sa, ACTOR_DEMO_SA, ALLOCTYPE_NORMAL)
/* 0x00CA */ DEFINE_ACTOR(Demo_Go, ACTOR_DEMO_GO, ALLOCTYPE_NORMAL)
/* 0x00CB */ DEFINE_ACTOR(En_In, ACTOR_EN_IN, ALLOCTYPE_NORMAL)
/* 0x00CC */ DEFINE_ACTOR(En_Tr, ACTOR_EN_TR, ALLOCTYPE_NORMAL)
/* 0x00CD */ DEFINE_ACTOR(Bg_Spot16_Bombstone, ACTOR_BG_SPOT16_BOMBSTONE, ALLOCTYPE_NORMAL)
/* 0x00CE */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_CE)
/* 0x00CF */ DEFINE_ACTOR(Bg_Hidan_Kowarerukabe, ACTOR_BG_HIDAN_KOWARERUKABE, ALLOCTYPE_NORMAL)
/* 0x00D0 */ DEFINE_ACTOR(Bg_Bombwall, ACTOR_BG_BOMBWALL, ALLOCTYPE_NORMAL)
/* 0x00D1 */ DEFINE_ACTOR(Bg_Spot08_Iceblock, ACTOR_BG_SPOT08_ICEBLOCK, ALLOCTYPE_NORMAL)
/* 0x00D2 */ DEFINE_ACTOR(En_Ru2, ACTOR_EN_RU2, ALLOCTYPE_NORMAL)
/* 0x00D3 */ DEFINE_ACTOR(Obj_Dekujr, ACTOR_OBJ_DEKUJR, ALLOCTYPE_NORMAL)
/* 0x00D4 */ DEFINE_ACTOR(Bg_Mizu_Uzu, ACTOR_BG_MIZU_UZU, ALLOCTYPE_NORMAL)
/* 0x00D5 */ DEFINE_ACTOR(Bg_Spot06_Objects, ACTOR_BG_SPOT06_OBJECTS, ALLOCTYPE_NORMAL)
/* 0x00D6 */ DEFINE_ACTOR(Bg_Ice_Objects, ACTOR_BG_ICE_OBJECTS, ALLOCTYPE_NORMAL)
/* 0x00D7 */ DEFINE_ACTOR(Bg_Haka_Water, ACTOR_BG_HAKA_WATER, ALLOCTYPE_NORMAL)
/* 0x00D8 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_D8)
/* 0x00D9 */ DEFINE_ACTOR(En_Ma2, ACTOR_EN_MA2, ALLOCTYPE_NORMAL)
/* 0x00DA */ DEFINE_ACTOR(En_Bom_Chu, ACTOR_EN_BOM_CHU, ALLOCTYPE_NORMAL)
/* 0x00DB */ DEFINE_ACTOR(En_Horse_Game_Check, ACTOR_EN_HORSE_GAME_CHECK, ALLOCTYPE_NORMAL)
/* 0x00DC */ DEFINE_ACTOR(Boss_Tw, ACTOR_BOSS_TW, ALLOCTYPE_NORMAL)
/* 0x00DD */ DEFINE_ACTOR(En_Rr, ACTOR_EN_RR, ALLOCTYPE_NORMAL)
/* 0x00DE */ DEFINE_ACTOR(En_Ba, ACTOR_EN_BA, ALLOCTYPE_NORMAL)
/* 0x00DF */ DEFINE_ACTOR(En_Bx, ACTOR_EN_BX, ALLOCTYPE_NORMAL)
/* 0x00E0 */ DEFINE_ACTOR(En_Anubice, ACTOR_EN_ANUBICE, ALLOCTYPE_NORMAL)
/* 0x00E1 */ DEFINE_ACTOR(En_Anubice_Fire, ACTOR_EN_ANUBICE_FIRE, ALLOCTYPE_NORMAL)
/* 0x00E2 */ DEFINE_ACTOR(Bg_Mori_Hashigo, ACTOR_BG_MORI_HASHIGO, ALLOCTYPE_NORMAL)
/* 0x00E3 */ DEFINE_ACTOR(Bg_Mori_Hashira4, ACTOR_BG_MORI_HASHIRA4, ALLOCTYPE_NORMAL)
/* 0x00E4 */ DEFINE_ACTOR(Bg_Mori_Idomizu, ACTOR_BG_MORI_IDOMIZU, ALLOCTYPE_NORMAL)
/* 0x00E5 */ DEFINE_ACTOR(Bg_Spot16_Doughnut, ACTOR_BG_SPOT16_DOUGHNUT, ALLOCTYPE_NORMAL)
/* 0x00E6 */ DEFINE_ACTOR(Bg_Bdan_Switch, ACTOR_BG_BDAN_SWITCH, ALLOCTYPE_NORMAL)
/* 0x00E7 */ DEFINE_ACTOR(En_Ma1, ACTOR_EN_MA1, ALLOCTYPE_NORMAL)
/* 0x00E8 */ DEFINE_ACTOR(Boss_Ganon, ACTOR_BOSS_GANON, ALLOCTYPE_NORMAL)
/* 0x00E9 */ DEFINE_ACTOR(Boss_Sst, ACTOR_BOSS_SST, ALLOCTYPE_NORMAL)
/* 0x00EA */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_EA)
/* 0x00EB */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_EB)
/* 0x00EC */ DEFINE_ACTOR(En_Ny, ACTOR_EN_NY, ALLOCTYPE_NORMAL)
/* 0x00ED */ DEFINE_ACTOR(En_Fr, ACTOR_EN_FR, ALLOCTYPE_NORMAL)
/* 0x00EE */ DEFINE_ACTOR(Item_Shield, ACTOR_ITEM_SHIELD, ALLOCTYPE_NORMAL)
/* 0x00EF */ DEFINE_ACTOR(Bg_Ice_Shelter, ACTOR_BG_ICE_SHELTER, ALLOCTYPE_NORMAL)
/* 0x00F0 */ DEFINE_ACTOR(En_Ice_Hono, ACTOR_EN_ICE_HONO, ALLOCTYPE_NORMAL)
/* 0x00F1 */ DEFINE_ACTOR(Item_Ocarina, ACTOR_ITEM_OCARINA, ALLOCTYPE_NORMAL)
/* 0x00F2 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_F2)
/* 0x00F3 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_F3)
/* 0x00F4 */ DEFINE_ACTOR(Magic_Dark, ACTOR_MAGIC_DARK, ALLOCTYPE_ABSOLUTE)
/* 0x00F5 */ DEFINE_ACTOR(Demo_6K, ACTOR_DEMO_6K, ALLOCTYPE_NORMAL)
/* 0x00F6 */ DEFINE_ACTOR(En_Anubice_Tag, ACTOR_EN_ANUBICE_TAG, ALLOCTYPE_NORMAL)
/* 0x00F7 */ DEFINE_ACTOR(Bg_Haka_Gate, ACTOR_BG_HAKA_GATE, ALLOCTYPE_NORMAL)
/* 0x00F8 */ DEFINE_ACTOR(Bg_Spot15_Saku, ACTOR_BG_SPOT15_SAKU, ALLOCTYPE_NORMAL)
/* 0x00F9 */ DEFINE_ACTOR(Bg_Jya_Goroiwa, ACTOR_BG_JYA_GOROIWA, ALLOCTYPE_NORMAL)
/* 0x00FA */ DEFINE_ACTOR(Bg_Jya_Zurerukabe, ACTOR_BG_JYA_ZURERUKABE, ALLOCTYPE_NORMAL)
/* 0x00FB */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_FB)
/* 0x00FC */ DEFINE_ACTOR(Bg_Jya_Cobra, ACTOR_BG_JYA_COBRA, ALLOCTYPE_NORMAL)
/* 0x00FD */ DEFINE_ACTOR(Bg_Jya_Kanaami, ACTOR_BG_JYA_KANAAMI, ALLOCTYPE_NORMAL)
/* 0x00FE */ DEFINE_ACTOR(Fishing, ACTOR_FISHING, ALLOCTYPE_NORMAL)
/* 0x00FF */ DEFINE_ACTOR(Obj_Oshihiki, ACTOR_OBJ_OSHIHIKI, ALLOCTYPE_NORMAL)
/* 0x0100 */ DEFINE_ACTOR(Bg_Gate_Shutter, ACTOR_BG_GATE_SHUTTER, ALLOCTYPE_NORMAL)
/* 0x0101 */ DEFINE_ACTOR(Eff_Dust, ACTOR_EFF_DUST, ALLOCTYPE_NORMAL)
/* 0x0102 */ DEFINE_ACTOR(Bg_Spot01_Fusya, ACTOR_BG_SPOT01_FUSYA, ALLOCTYPE_NORMAL)
/* 0x0103 */ DEFINE_ACTOR(Bg_Spot01_Idohashira, ACTOR_BG_SPOT01_IDOHASHIRA, ALLOCTYPE_NORMAL)
/* 0x0104 */ DEFINE_ACTOR(Bg_Spot01_Idomizu, ACTOR_BG_SPOT01_IDOMIZU, ALLOCTYPE_NORMAL)
/* 0x0105 */ DEFINE_ACTOR(Bg_Po_Syokudai, ACTOR_BG_PO_SYOKUDAI, ALLOCTYPE_NORMAL)
/* 0x0106 */ DEFINE_ACTOR(Bg_Ganon_Otyuka, ACTOR_BG_GANON_OTYUKA, ALLOCTYPE_NORMAL)
/* 0x0107 */ DEFINE_ACTOR(Bg_Spot15_Rrbox, ACTOR_BG_SPOT15_RRBOX, ALLOCTYPE_NORMAL)
/* 0x0108 */ DEFINE_ACTOR(Bg_Umajump, ACTOR_BG_UMAJUMP, ALLOCTYPE_NORMAL)
/* 0x0109 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_109)
/* 0x010A */ DEFINE_ACTOR(Arrow_Fire, ACTOR_ARROW_FIRE, ALLOCTYPE_ABSOLUTE)
/* 0x010B */ DEFINE_ACTOR(Arrow_Ice, ACTOR_ARROW_ICE, ALLOCTYPE_ABSOLUTE)
/* 0x010C */ DEFINE_ACTOR(Arrow_Light, ACTOR_ARROW_LIGHT, ALLOCTYPE_ABSOLUTE)
/* 0x010D */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_10D)
/* 0x010E */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_10E)
/* 0x010F */ DEFINE_ACTOR(Item_Etcetera, ACTOR_ITEM_ETCETERA, ALLOCTYPE_NORMAL)
/* 0x0110 */ DEFINE_ACTOR(Obj_Kibako, ACTOR_OBJ_KIBAKO, ALLOCTYPE_NORMAL)
/* 0x0111 */ DEFINE_ACTOR(Obj_Tsubo, ACTOR_OBJ_TSUBO, ALLOCTYPE_NORMAL)
/* 0x0112 */ DEFINE_ACTOR(En_Wonder_Item, ACTOR_EN_WONDER_ITEM, ALLOCTYPE_NORMAL)
/* 0x0113 */ DEFINE_ACTOR(En_Ik, ACTOR_EN_IK, ALLOCTYPE_NORMAL)
/* 0x0114 */ DEFINE_ACTOR(Demo_Ik, ACTOR_DEMO_IK, ALLOCTYPE_NORMAL)
/* 0x0115 */ DEFINE_ACTOR(En_Skj, ACTOR_EN_SKJ, ALLOCTYPE_NORMAL)
/* 0x0116 */ DEFINE_ACTOR(En_Skjneedle, ACTOR_EN_SKJNEEDLE, ALLOCTYPE_NORMAL)
/* 0x0117 */ DEFINE_ACTOR(En_G_Switch, ACTOR_EN_G_SWITCH, ALLOCTYPE_NORMAL)
/* 0x0118 */ DEFINE_ACTOR(Demo_Ext, ACTOR_DEMO_EXT, ALLOCTYPE_NORMAL)
/* 0x0119 */ DEFINE_ACTOR(Demo_Shd, ACTOR_DEMO_SHD, ALLOCTYPE_NORMAL)
/* 0x011A */ DEFINE_ACTOR(En_Dns, ACTOR_EN_DNS, ALLOCTYPE_NORMAL)
/* 0x011B */ DEFINE_ACTOR(Elf_Msg, ACTOR_ELF_MSG, ALLOCTYPE_NORMAL)
/* 0x011C */ DEFINE_ACTOR(En_Honotrap, ACTOR_EN_HONOTRAP, ALLOCTYPE_NORMAL)
/* 0x011D */ DEFINE_ACTOR(En_Tubo_Trap, ACTOR_EN_TUBO_TRAP, ALLOCTYPE_NORMAL)
/* 0x011E */ DEFINE_ACTOR(Obj_Ice_Poly, ACTOR_OBJ_ICE_POLY, ALLOCTYPE_NORMAL)
/* 0x011F */ DEFINE_ACTOR(Bg_Spot03_Taki, ACTOR_BG_SPOT03_TAKI, ALLOCTYPE_NORMAL)
/* 0x0120 */ DEFINE_ACTOR(Bg_Spot07_Taki, ACTOR_BG_SPOT07_TAKI, ALLOCTYPE_NORMAL)
/* 0x0121 */ DEFINE_ACTOR(En_Fz, ACTOR_EN_FZ, ALLOCTYPE_NORMAL)
/* 0x0122 */ DEFINE_ACTOR(En_Po_Relay, ACTOR_EN_PO_RELAY, ALLOCTYPE_NORMAL)
/* 0x0123 */ DEFINE_ACTOR(Bg_Relay_Objects, ACTOR_BG_RELAY_OBJECTS, ALLOCTYPE_NORMAL)
/* 0x0124 */ DEFINE_ACTOR(En_Diving_Game, ACTOR_EN_DIVING_GAME, ALLOCTYPE_NORMAL)
/* 0x0125 */ DEFINE_ACTOR(En_Kusa, ACTOR_EN_KUSA, ALLOCTYPE_NORMAL)
/* 0x0126 */ DEFINE_ACTOR(Obj_Bean, ACTOR_OBJ_BEAN, ALLOCTYPE_NORMAL)
/* 0x0127 */ DEFINE_ACTOR(Obj_Bombiwa, ACTOR_OBJ_BOMBIWA, ALLOCTYPE_NORMAL)
/* 0x0128 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_128)
/* 0x0129 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_129)
/* 0x012A */ DEFINE_ACTOR(Obj_Switch, ACTOR_OBJ_SWITCH, ALLOCTYPE_NORMAL)
/* 0x012B */ DEFINE_ACTOR(Obj_Elevator, ACTOR_OBJ_ELEVATOR, ALLOCTYPE_NORMAL)
/* 0x012C */ DEFINE_ACTOR(Obj_Lift, ACTOR_OBJ_LIFT, ALLOCTYPE_NORMAL)
/* 0x012D */ DEFINE_ACTOR(Obj_Hsblock, ACTOR_OBJ_HSBLOCK, ALLOCTYPE_NORMAL)
/* 0x012E */ DEFINE_ACTOR(En_Okarina_Tag, ACTOR_EN_OKARINA_TAG, ALLOCTYPE_NORMAL)
/* 0x012F */ DEFINE_ACTOR(En_Yabusame_Mark, ACTOR_EN_YABUSAME_MARK, ALLOCTYPE_NORMAL)
/* 0x0130 */ DEFINE_ACTOR(En_Goroiwa, ACTOR_EN_GOROIWA, ALLOCTYPE_NORMAL)
/* 0x0131 */ DEFINE_ACTOR(En_Ex_Ruppy, ACTOR_EN_EX_RUPPY, ALLOCTYPE_NORMAL)
/* 0x0132 */ DEFINE_ACTOR(En_Toryo, ACTOR_EN_TORYO, ALLOCTYPE_NORMAL)
/* 0x0133 */ DEFINE_ACTOR(En_Daiku, ACTOR_EN_DAIKU, ALLOCTYPE_NORMAL)
/* 0x0134 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_134)
/* 0x0135 */ DEFINE_ACTOR(En_Nwc, ACTOR_EN_NWC, ALLOCTYPE_NORMAL)
/* 0x0136 */ DEFINE_ACTOR(En_Blkobj, ACTOR_EN_BLKOBJ, ALLOCTYPE_NORMAL)
/* 0x0137 */ DEFINE_ACTOR(Item_Inbox, ACTOR_ITEM_INBOX, ALLOCTYPE_NORMAL)
/* 0x0138 */ DEFINE_ACTOR(En_Ge1, ACTOR_EN_GE1, ALLOCTYPE_NORMAL)
/* 0x0139 */ DEFINE_ACTOR(Obj_Blockstop, ACTOR_OBJ_BLOCKSTOP, ALLOCTYPE_NORMAL)
/* 0x013A */ DEFINE_ACTOR(En_Sda, ACTOR_EN_SDA, ALLOCTYPE_NORMAL)
/* 0x013B */ DEFINE_ACTOR(En_Clear_Tag, ACTOR_EN_CLEAR_TAG, ALLOCTYPE_NORMAL)
/* 0x013C */ DEFINE_ACTOR(En_Niw_Lady, ACTOR_EN_NIW_LADY, ALLOCTYPE_NORMAL)
/* 0x013D */ DEFINE_ACTOR(En_Gm, ACTOR_EN_GM, ALLOCTYPE_NORMAL)
/* 0x013E */ DEFINE_ACTOR(En_Ms, ACTOR_EN_MS, ALLOCTYPE_NORMAL)
/* 0x013F */ DEFINE_ACTOR(En_Hs, ACTOR_EN_HS, ALLOCTYPE_NORMAL)
/* 0x0140 */ DEFINE_ACTOR(Bg_Ingate, ACTOR_BG_INGATE, ALLOCTYPE_NORMAL)
/* 0x0141 */ DEFINE_ACTOR(En_Kanban, ACTOR_EN_KANBAN, ALLOCTYPE_NORMAL)
/* 0x0142 */ DEFINE_ACTOR(En_Heishi3, ACTOR_EN_HEISHI3, ALLOCTYPE_NORMAL)
/* 0x0143 */ DEFINE_ACTOR(En_Syateki_Niw, ACTOR_EN_SYATEKI_NIW, ALLOCTYPE_NORMAL)
/* 0x0144 */ DEFINE_ACTOR(En_Attack_Niw, ACTOR_EN_ATTACK_NIW, ALLOCTYPE_NORMAL)
/* 0x0145 */ DEFINE_ACTOR(Bg_Spot01_Idosoko, ACTOR_BG_SPOT01_IDOSOKO, ALLOCTYPE_NORMAL)
/* 0x0146 */ DEFINE_ACTOR(En_Sa, ACTOR_EN_SA, ALLOCTYPE_NORMAL)
/* 0x0147 */ DEFINE_ACTOR(En_Wonder_Talk, ACTOR_EN_WONDER_TALK, ALLOCTYPE_NORMAL)
/* 0x0148 */ DEFINE_ACTOR(Bg_Gjyo_Bridge, ACTOR_BG_GJYO_BRIDGE, ALLOCTYPE_NORMAL)
/* 0x0149 */ DEFINE_ACTOR(En_Ds, ACTOR_EN_DS, ALLOCTYPE_NORMAL)
/* 0x014A */ DEFINE_ACTOR(En_Mk, ACTOR_EN_MK, ALLOCTYPE_NORMAL)
/* 0x014B */ DEFINE_ACTOR(En_Bom_Bowl_Man, ACTOR_EN_BOM_BOWL_MAN, ALLOCTYPE_NORMAL)
/* 0x014C */ DEFINE_ACTOR(En_Bom_Bowl_Pit, ACTOR_EN_BOM_BOWL_PIT, ALLOCTYPE_NORMAL)
/* 0x014D */ DEFINE_ACTOR(En_Owl, ACTOR_EN_OWL, ALLOCTYPE_NORMAL)
/* 0x014E */ DEFINE_ACTOR(En_Ishi, ACTOR_EN_ISHI, ALLOCTYPE_NORMAL)
/* 0x014F */ DEFINE_ACTOR(Obj_Hana, ACTOR_OBJ_HANA, ALLOCTYPE_NORMAL)
/* 0x0150 */ DEFINE_ACTOR(Obj_Lightswitch, ACTOR_OBJ_LIGHTSWITCH, ALLOCTYPE_NORMAL)
/* 0x0151 */ DEFINE_ACTOR(Obj_Mure2, ACTOR_OBJ_MURE2, ALLOCTYPE_NORMAL)
/* 0x0152 */ DEFINE_ACTOR(En_Go, ACTOR_EN_GO, ALLOCTYPE_NORMAL)
/* 0x0153 */ DEFINE_ACTOR(En_Fu, ACTOR_EN_FU, ALLOCTYPE_NORMAL)
/* 0x0154 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_154)
/* 0x0155 */ DEFINE_ACTOR(En_Changer, ACTOR_EN_CHANGER, ALLOCTYPE_NORMAL)
/* 0x0156 */ DEFINE_ACTOR(Bg_Jya_Megami, ACTOR_BG_JYA_MEGAMI, ALLOCTYPE_NORMAL)
/* 0x0157 */ DEFINE_ACTOR(Bg_Jya_Lift, ACTOR_BG_JYA_LIFT, ALLOCTYPE_NORMAL)
/* 0x0158 */ DEFINE_ACTOR(Bg_Jya_Bigmirror, ACTOR_BG_JYA_BIGMIRROR, ALLOCTYPE_NORMAL)
/* 0x0159 */ DEFINE_ACTOR(Bg_Jya_Bombchuiwa, ACTOR_BG_JYA_BOMBCHUIWA, ALLOCTYPE_NORMAL)
/* 0x015A */ DEFINE_ACTOR(Bg_Jya_Amishutter, ACTOR_BG_JYA_AMISHUTTER, ALLOCTYPE_NORMAL)
/* 0x015B */ DEFINE_ACTOR(Bg_Jya_Bombiwa, ACTOR_BG_JYA_BOMBIWA, ALLOCTYPE_NORMAL)
/* 0x015C */ DEFINE_ACTOR(Bg_Spot18_Basket, ACTOR_BG_SPOT18_BASKET, ALLOCTYPE_NORMAL)
/* 0x015D */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_15D)
/* 0x015E */ DEFINE_ACTOR(En_Ganon_Organ, ACTOR_EN_GANON_ORGAN, ALLOCTYPE_NORMAL)
/* 0x015F */ DEFINE_ACTOR(En_Siofuki, ACTOR_EN_SIOFUKI, ALLOCTYPE_NORMAL)
/* 0x0160 */ DEFINE_ACTOR(En_Stream, ACTOR_EN_STREAM, ALLOCTYPE_NORMAL)
/* 0x0161 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_161)
/* 0x0162 */ DEFINE_ACTOR(En_Mm, ACTOR_EN_MM, ALLOCTYPE_NORMAL)
/* 0x0163 */ DEFINE_ACTOR(En_Ko, ACTOR_EN_KO, ALLOCTYPE_NORMAL)
/* 0x0164 */ DEFINE_ACTOR(En_Kz, ACTOR_EN_KZ, ALLOCTYPE_NORMAL)
/* 0x0165 */ DEFINE_ACTOR(En_Weather_Tag, ACTOR_EN_WEATHER_TAG, ALLOCTYPE_NORMAL)
/* 0x0166 */ DEFINE_ACTOR(Bg_Sst_Floor, ACTOR_BG_SST_FLOOR, ALLOCTYPE_NORMAL)
/* 0x0167 */ DEFINE_ACTOR(En_Ani, ACTOR_EN_ANI, ALLOCTYPE_NORMAL)
/* 0x0168 */ DEFINE_ACTOR(En_Ex_Item, ACTOR_EN_EX_ITEM, ALLOCTYPE_NORMAL)
/* 0x0169 */ DEFINE_ACTOR(Bg_Jya_Ironobj, ACTOR_BG_JYA_IRONOBJ, ALLOCTYPE_NORMAL)
/* 0x016A */ DEFINE_ACTOR(En_Js, ACTOR_EN_JS, ALLOCTYPE_NORMAL)
/* 0x016B */ DEFINE_ACTOR(En_Jsjutan, ACTOR_EN_JSJUTAN, ALLOCTYPE_NORMAL)
/* 0x016C */ DEFINE_ACTOR(En_Cs, ACTOR_EN_CS, ALLOCTYPE_NORMAL)
/* 0x016D */ DEFINE_ACTOR(En_Md, ACTOR_EN_MD, ALLOCTYPE_NORMAL)
/* 0x016E */ DEFINE_ACTOR(En_Hy, ACTOR_EN_HY, ALLOCTYPE_NORMAL)
/* 0x016F */ DEFINE_ACTOR(En_Ganon_Mant, ACTOR_EN_GANON_MANT, ALLOCTYPE_NORMAL)
/* 0x0170 */ DEFINE_ACTOR(En_Okarina_Effect, ACTOR_EN_OKARINA_EFFECT, ALLOCTYPE_NORMAL)
/* 0x0171 */ DEFINE_ACTOR(En_Mag, ACTOR_EN_MAG, ALLOCTYPE_NORMAL)
/* 0x0172 */ DEFINE_ACTOR(Door_Gerudo, ACTOR_DOOR_GERUDO, ALLOCTYPE_NORMAL)
/* 0x0173 */ DEFINE_ACTOR(Elf_Msg2, ACTOR_ELF_MSG2, ALLOCTYPE_NORMAL)
/* 0x0174 */ DEFINE_ACTOR(Demo_Gt, ACTOR_DEMO_GT, ALLOCTYPE_NORMAL)
/* 0x0175 */ DEFINE_ACTOR(En_Po_Field, ACTOR_EN_PO_FIELD, ALLOCTYPE_NORMAL)
/* 0x0176 */ DEFINE_ACTOR(Efc_Erupc, ACTOR_EFC_ERUPC, ALLOCTYPE_NORMAL)
/* 0x0177 */ DEFINE_ACTOR(Bg_Zg, ACTOR_BG_ZG, ALLOCTYPE_NORMAL)
/* 0x0178 */ DEFINE_ACTOR(En_Heishi4, ACTOR_EN_HEISHI4, ALLOCTYPE_NORMAL)
/* 0x0179 */ DEFINE_ACTOR(En_Zl3, ACTOR_EN_ZL3, ALLOCTYPE_NORMAL)
/* 0x017A */ DEFINE_ACTOR(Boss_Ganon2, ACTOR_BOSS_GANON2, ALLOCTYPE_NORMAL)
/* 0x017B */ DEFINE_ACTOR(En_Kakasi, ACTOR_EN_KAKASI, ALLOCTYPE_NORMAL)
/* 0x017C */ DEFINE_ACTOR(En_Takara_Man, ACTOR_EN_TAKARA_MAN, ALLOCTYPE_NORMAL)
/* 0x017D */ DEFINE_ACTOR(Obj_Makeoshihiki, ACTOR_OBJ_MAKEOSHIHIKI, ALLOCTYPE_NORMAL)
/* 0x017E */ DEFINE_ACTOR(Oceff_Spot, ACTOR_OCEFF_SPOT, ALLOCTYPE_ABSOLUTE)
/* 0x017F */ DEFINE_ACTOR(End_Title, ACTOR_END_TITLE, ALLOCTYPE_NORMAL)
/* 0x0180 */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_180)
/* 0x0181 */ DEFINE_ACTOR(En_Torch, ACTOR_EN_TORCH, ALLOCTYPE_NORMAL)
/* 0x0182 */ DEFINE_ACTOR(Demo_Ec, ACTOR_DEMO_EC, ALLOCTYPE_NORMAL)
/* 0x0183 */ DEFINE_ACTOR(Shot_Sun, ACTOR_SHOT_SUN, ALLOCTYPE_NORMAL)
/* 0x0184 */ DEFINE_ACTOR(En_Dy_Extra, ACTOR_EN_DY_EXTRA, ALLOCTYPE_NORMAL)
/* 0x0185 */ DEFINE_ACTOR(En_Wonder_Talk2, ACTOR_EN_WONDER_TALK2, ALLOCTYPE_NORMAL)
/* 0x0186 */ DEFINE_ACTOR(En_Ge2, ACTOR_EN_GE2, ALLOCTYPE_NORMAL)
/* 0x0187 */ DEFINE_ACTOR(Obj_Roomtimer, ACTOR_OBJ_ROOMTIMER, ALLOCTYPE_NORMAL)
/* 0x0188 */ DEFINE_ACTOR(En_Ssh, ACTOR_EN_SSH, ALLOCTYPE_NORMAL)
/* 0x0189 */ DEFINE_ACTOR(En_Sth, ACTOR_EN_STH, ALLOCTYPE_NORMAL)
/* 0x018A */ DEFINE_ACTOR(Oceff_Wipe, ACTOR_OCEFF_WIPE, ALLOCTYPE_ABSOLUTE)
/* 0x018B */ DEFINE_ACTOR(Oceff_Storm, ACTOR_OCEFF_STORM, ALLOCTYPE_ABSOLUTE)
/* 0x018C */ DEFINE_ACTOR(En_Weiyer, ACTOR_EN_WEIYER, ALLOCTYPE_NORMAL)
/* 0x018D */ DEFINE_ACTOR(Bg_Spot05_Soko, ACTOR_BG_SPOT05_SOKO, ALLOCTYPE_NORMAL)
/* 0x018E */ DEFINE_ACTOR(Bg_Jya_1flift, ACTOR_BG_JYA_1FLIFT, ALLOCTYPE_NORMAL)
/* 0x018F */ DEFINE_ACTOR(Bg_Jya_Haheniron, ACTOR_BG_JYA_HAHENIRON, ALLOCTYPE_NORMAL)
/* 0x0190 */ DEFINE_ACTOR(Bg_Spot12_Gate, ACTOR_BG_SPOT12_GATE, ALLOCTYPE_NORMAL)
/* 0x0191 */ DEFINE_ACTOR(Bg_Spot12_Saku, ACTOR_BG_SPOT12_SAKU, ALLOCTYPE_NORMAL)
/* 0x0192 */ DEFINE_ACTOR(En_Hintnuts, ACTOR_EN_HINTNUTS, ALLOCTYPE_NORMAL)
/* 0x0193 */ DEFINE_ACTOR(En_Nutsball, ACTOR_EN_NUTSBALL, ALLOCTYPE_NORMAL)
/* 0x0194 */ DEFINE_ACTOR(Bg_Spot00_Break, ACTOR_BG_SPOT00_BREAK, ALLOCTYPE_NORMAL)
/* 0x0195 */ DEFINE_ACTOR(En_Shopnuts, ACTOR_EN_SHOPNUTS, ALLOCTYPE_NORMAL)
/* 0x0196 */ DEFINE_ACTOR(En_It, ACTOR_EN_IT, ALLOCTYPE_NORMAL)
/* 0x0197 */ DEFINE_ACTOR(En_GeldB, ACTOR_EN_GELDB, ALLOCTYPE_NORMAL)
/* 0x0198 */ DEFINE_ACTOR(Oceff_Wipe2, ACTOR_OCEFF_WIPE2, ALLOCTYPE_ABSOLUTE)
/* 0x0199 */ DEFINE_ACTOR(Oceff_Wipe3, ACTOR_OCEFF_WIPE3, ALLOCTYPE_ABSOLUTE)
/* 0x019A */ DEFINE_ACTOR(En_Niw_Girl, ACTOR_EN_NIW_GIRL, ALLOCTYPE_NORMAL)
/* 0x019B */ DEFINE_ACTOR(En_Dog, ACTOR_EN_DOG, ALLOCTYPE_NORMAL)
/* 0x019C */ DEFINE_ACTOR(En_Si, ACTOR_EN_SI, ALLOCTYPE_NORMAL)
/* 0x019D */ DEFINE_ACTOR(Bg_Spot01_Objects2, ACTOR_BG_SPOT01_OBJECTS2, ALLOCTYPE_NORMAL)
/* 0x019E */ DEFINE_ACTOR(Obj_Comb, ACTOR_OBJ_COMB, ALLOCTYPE_NORMAL)
/* 0x019F */ DEFINE_ACTOR(Bg_Spot11_Bakudankabe, ACTOR_BG_SPOT11_BAKUDANKABE, ALLOCTYPE_NORMAL)
/* 0x01A0 */ DEFINE_ACTOR(Obj_Kibako2, ACTOR_OBJ_KIBAKO2, ALLOCTYPE_NORMAL)
/* 0x01A1 */ DEFINE_ACTOR(En_Dnt_Demo, ACTOR_EN_DNT_DEMO, ALLOCTYPE_NORMAL)
/* 0x01A2 */ DEFINE_ACTOR(En_Dnt_Jiji, ACTOR_EN_DNT_JIJI, ALLOCTYPE_NORMAL)
/* 0x01A3 */ DEFINE_ACTOR(En_Dnt_Nomal, ACTOR_EN_DNT_NOMAL, ALLOCTYPE_NORMAL)
/* 0x01A4 */ DEFINE_ACTOR(En_Guest, ACTOR_EN_GUEST, ALLOCTYPE_NORMAL)
/* 0x01A5 */ DEFINE_ACTOR(Bg_Bom_Guard, ACTOR_BG_BOM_GUARD, ALLOCTYPE_NORMAL)
/* 0x01A6 */ DEFINE_ACTOR(En_Hs2, ACTOR_EN_HS2, ALLOCTYPE_NORMAL)
/* 0x01A7 */ DEFINE_ACTOR(Demo_Kekkai, ACTOR_DEMO_KEKKAI, ALLOCTYPE_NORMAL)
/* 0x01A8 */ DEFINE_ACTOR(Bg_Spot08_Bakudankabe, ACTOR_BG_SPOT08_BAKUDANKABE, ALLOCTYPE_NORMAL)
/* 0x01A9 */ DEFINE_ACTOR(Bg_Spot17_Bakudankabe, ACTOR_BG_SPOT17_BAKUDANKABE, ALLOCTYPE_NORMAL)
/* 0x01AA */ DEFINE_ACTOR_UNSET(ACTOR_UNSET_1AA)
/* 0x01AB */ DEFINE_ACTOR(Obj_Mure3, ACTOR_OBJ_MURE3, ALLOCTYPE_NORMAL)
/* 0x01AC */ DEFINE_ACTOR(En_Tg, ACTOR_EN_TG, ALLOCTYPE_NORMAL)
/* 0x01AD */ DEFINE_ACTOR(En_Mu, ACTOR_EN_MU, ALLOCTYPE_NORMAL)
/* 0x01AE */ DEFINE_ACTOR(En_Go2, ACTOR_EN_GO2, ALLOCTYPE_NORMAL)
/* 0x01AF */ DEFINE_ACTOR(En_Wf, ACTOR_EN_WF, ALLOCTYPE_NORMAL)
/* 0x01B0 */ DEFINE_ACTOR(En_Skb, ACTOR_EN_SKB, ALLOCTYPE_NORMAL)
/* 0x01B1 */ DEFINE_ACTOR(Demo_Gj, ACTOR_DEMO_GJ, ALLOCTYPE_NORMAL)
/* 0x01B2 */ DEFINE_ACTOR(Demo_Geff, ACTOR_DEMO_GEFF, ALLOCTYPE_NORMAL)
/* 0x01B3 */ DEFINE_ACTOR(Bg_Gnd_Firemeiro, ACTOR_BG_GND_FIREMEIRO, ALLOCTYPE_NORMAL)
/* 0x01B4 */ DEFINE_ACTOR(Bg_Gnd_Darkmeiro, ACTOR_BG_GND_DARKMEIRO, ALLOCTYPE_NORMAL)
/* 0x01B5 */ DEFINE_ACTOR(Bg_Gnd_Soulmeiro, ACTOR_BG_GND_SOULMEIRO, ALLOCTYPE_NORMAL)
/* 0x01B6 */ DEFINE_ACTOR(Bg_Gnd_Nisekabe, ACTOR_BG_GND_NISEKABE, ALLOCTYPE_NORMAL)
/* 0x01B7 */ DEFINE_ACTOR(Bg_Gnd_Iceblock, ACTOR_BG_GND_ICEBLOCK, ALLOCTYPE_NORMAL)
/* 0x01B8 */ DEFINE_ACTOR(En_Gb, ACTOR_EN_GB, ALLOCTYPE_NORMAL)
/* 0x01B9 */ DEFINE_ACTOR(En_Gs, ACTOR_EN_GS, ALLOCTYPE_NORMAL)
/* 0x01BA */ DEFINE_ACTOR(Bg_Mizu_Bwall, ACTOR_BG_MIZU_BWALL, ALLOCTYPE_NORMAL)
/* 0x01BB */ DEFINE_ACTOR(Bg_Mizu_Shutter, ACTOR_BG_MIZU_SHUTTER, ALLOCTYPE_NORMAL)
/* 0x01BC */ DEFINE_ACTOR(En_Daiku_Kakariko, ACTOR_EN_DAIKU_KAKARIKO, ALLOCTYPE_NORMAL)
/* 0x01BD */ DEFINE_ACTOR(Bg_Bowl_Wall, ACTOR_BG_BOWL_WALL, ALLOCTYPE_NORMAL)
/* 0x01BE */ DEFINE_ACTOR(En_Wall_Tubo, ACTOR_EN_WALL_TUBO, ALLOCTYPE_NORMAL)
/* 0x01BF */ DEFINE_ACTOR(En_Po_Desert, ACTOR_EN_PO_DESERT, ALLOCTYPE_NORMAL)
/* 0x01C0 */ DEFINE_ACTOR(En_Crow, ACTOR_EN_CROW, ALLOCTYPE_NORMAL)
/* 0x01C1 */ DEFINE_ACTOR(Door_Killer, ACTOR_DOOR_KILLER, ALLOCTYPE_NORMAL)
/* 0x01C2 */ DEFINE_ACTOR(Bg_Spot11_Oasis, ACTOR_BG_SPOT11_OASIS, ALLOCTYPE_NORMAL)
/* 0x01C3 */ DEFINE_ACTOR(Bg_Spot18_Futa, ACTOR_BG_SPOT18_FUTA, ALLOCTYPE_NORMAL)
/* 0x01C4 */ DEFINE_ACTOR(Bg_Spot18_Shutter, ACTOR_BG_SPOT18_SHUTTER, ALLOCTYPE_NORMAL)
/* 0x01C5 */ DEFINE_ACTOR(En_Ma3, ACTOR_EN_MA3, ALLOCTYPE_NORMAL)
/* 0x01C6 */ DEFINE_ACTOR(En_Cow, ACTOR_EN_COW, ALLOCTYPE_NORMAL)
/* 0x01C7 */ DEFINE_ACTOR(Bg_Ice_Turara, ACTOR_BG_ICE_TURARA, ALLOCTYPE_NORMAL)
/* 0x01C8 */ DEFINE_ACTOR(Bg_Ice_Shutter, ACTOR_BG_ICE_SHUTTER, ALLOCTYPE_NORMAL)
/* 0x01C9 */ DEFINE_ACTOR(En_Kakasi2, ACTOR_EN_KAKASI2, ALLOCTYPE_NORMAL)
/* 0x01CA */ DEFINE_ACTOR(En_Kakasi3, ACTOR_EN_KAKASI3, ALLOCTYPE_NORMAL)
/* 0x01CB */ DEFINE_ACTOR(Oceff_Wipe4, ACTOR_OCEFF_WIPE4, ALLOCTYPE_ABSOLUTE)
/* 0x01CC */ DEFINE_ACTOR(En_Eg, ACTOR_EN_EG, ALLOCTYPE_NORMAL)
/* 0x01CD */ DEFINE_ACTOR(Bg_Menkuri_Nisekabe, ACTOR_BG_MENKURI_NISEKABE, ALLOCTYPE_NORMAL)
/* 0x01CE */ DEFINE_ACTOR(En_Zo, ACTOR_EN_ZO, ALLOCTYPE_NORMAL)
/* 0x01CF */ DEFINE_ACTOR(Obj_Makekinsuta, ACTOR_OBJ_MAKEKINSUTA, ALLOCTYPE_NORMAL)
/* 0x01D0 */ DEFINE_ACTOR(En_Ge3, ACTOR_EN_GE3, ALLOCTYPE_NORMAL)
/* 0x01D1 */ DEFINE_ACTOR(Obj_Timeblock, ACTOR_OBJ_TIMEBLOCK, ALLOCTYPE_NORMAL)
/* 0x01D2 */ DEFINE_ACTOR(Obj_Hamishi, ACTOR_OBJ_HAMISHI, ALLOCTYPE_NORMAL)
/* 0x01D3 */ DEFINE_ACTOR(En_Zl4, ACTOR_EN_ZL4, ALLOCTYPE_NORMAL)
/* 0x01D4 */ DEFINE_ACTOR(En_Mm2, ACTOR_EN_MM2, ALLOCTYPE_NORMAL)
/* 0x01D5 */ DEFINE_ACTOR(Bg_Jya_Block, ACTOR_BG_JYA_BLOCK, ALLOCTYPE_NORMAL)
/* 0x01D6 */ DEFINE_ACTOR(Obj_Warp2block, ACTOR_OBJ_WARP2BLOCK, ALLOCTYPE_NORMAL)
