#include "global.h"
#include "vt.h"

StackEntry sDmaMgrStackInfo;
OSMesgQueue sDmaMgrMsgQueue;
OSMesg sDmaMgrMsgs[0x20];
OSThread sDmaMgrThread;
u8 sDmaMgrStack[0x500];
const char* sDmaMgrCurFileName;
s32 sDmaMgrCurFileLine;

u32 D_80009460 = 0;
u32 gDmaMgrDmaBuffSize = 0x2000;
u32 sDmaMgrDataExistError = 0;

const char* sDmaMgrFileNames[] = {
    "makerom",
    "boot",
    "dmadata",
    "Audiobank",
    "Audioseq",
    "Audiotable",
    "link_animetion",
    "icon_item_static",
    "icon_item_24_static",
    "icon_item_field_static",
    "icon_item_dungeon_static",
    "icon_item_gameover_static",
    "icon_item_nes_static",
    "icon_item_ger_static",
    "icon_item_fra_static",
    "item_name_static",
    "map_name_static",
    "do_action_static",
    "message_static",
    "message_texture_static",
    "nes_font_static",
    "nes_message_data_static",
    "ger_message_data_static",
    "fra_message_data_static",
    "staff_message_data_static",
    "map_grand_static",
    "map_i_static",
    "map_48x85_static",
    "code",
    "ovl_title",
    "ovl_select",
    "ovl_opening",
    "ovl_file_choose",
    "ovl_kaleido_scope",
    "ovl_player_actor",
    "ovl_map_mark_data",
    "ovl_En_Test",
    "ovl_Arms_Hook",
    "ovl_Arrow_Fire",
    "ovl_Arrow_Ice",
    "ovl_Arrow_Light",
    "ovl_Bg_Bdan_Objects",
    "ovl_Bg_Bdan_Switch",
    "ovl_Bg_Bom_Guard",
    "ovl_Bg_Bombwall",
    "ovl_Bg_Bowl_Wall",
    "ovl_Bg_Breakwall",
    "ovl_Bg_Ddan_Jd",
    "ovl_Bg_Ddan_Kd",
    "ovl_Bg_Dodoago",
    "ovl_Bg_Dy_Yoseizo",
    "ovl_Bg_Ganon_Otyuka",
    "ovl_Bg_Gate_Shutter",
    "ovl_Bg_Gjyo_Bridge",
    "ovl_Bg_Gnd_Darkmeiro",
    "ovl_Bg_Gnd_Firemeiro",
    "ovl_Bg_Gnd_Iceblock",
    "ovl_Bg_Gnd_Nisekabe",
    "ovl_Bg_Gnd_Soulmeiro",
    "ovl_Bg_Haka",
    "ovl_Bg_Haka_Gate",
    "ovl_Bg_Haka_Huta",
    "ovl_Bg_Haka_Megane",
    "ovl_Bg_Haka_MeganeBG",
    "ovl_Bg_Haka_Sgami",
    "ovl_Bg_Haka_Ship",
    "ovl_Bg_Haka_Trap",
    "ovl_Bg_Haka_Tubo",
    "ovl_Bg_Haka_Water",
    "ovl_Bg_Haka_Zou",
    "ovl_Bg_Heavy_Block",
    "ovl_Bg_Hidan_Curtain",
    "ovl_Bg_Hidan_Dalm",
    "ovl_Bg_Hidan_Firewall",
    "ovl_Bg_Hidan_Fslift",
    "ovl_Bg_Hidan_Fwbig",
    "ovl_Bg_Hidan_Hamstep",
    "ovl_Bg_Hidan_Hrock",
    "ovl_Bg_Hidan_Kousi",
    "ovl_Bg_Hidan_Kowarerukabe",
    "ovl_Bg_Hidan_Rock",
    "ovl_Bg_Hidan_Rsekizou",
    "ovl_Bg_Hidan_Sekizou",
    "ovl_Bg_Hidan_Sima",
    "ovl_Bg_Hidan_Syoku",
    "ovl_Bg_Ice_Objects",
    "ovl_Bg_Ice_Shelter",
    "ovl_Bg_Ice_Shutter",
    "ovl_Bg_Ice_Turara",
    "ovl_Bg_Ingate",
    "ovl_Bg_Jya_1flift",
    "ovl_Bg_Jya_Amishutter",
    "ovl_Bg_Jya_Bigmirror",
    "ovl_Bg_Jya_Block",
    "ovl_Bg_Jya_Bombchuiwa",
    "ovl_Bg_Jya_Bombiwa",
    "ovl_Bg_Jya_Cobra",
    "ovl_Bg_Jya_Goroiwa",
    "ovl_Bg_Jya_Haheniron",
    "ovl_Bg_Jya_Ironobj",
    "ovl_Bg_Jya_Kanaami",
    "ovl_Bg_Jya_Lift",
    "ovl_Bg_Jya_Megami",
    "ovl_Bg_Jya_Zurerukabe",
    "ovl_Bg_Menkuri_Eye",
    "ovl_Bg_Menkuri_Kaiten",
    "ovl_Bg_Menkuri_Nisekabe",
    "ovl_Bg_Mizu_Bwall",
    "ovl_Bg_Mizu_Movebg",
    "ovl_Bg_Mizu_Shutter",
    "ovl_Bg_Mizu_Uzu",
    "ovl_Bg_Mizu_Water",
    "ovl_Bg_Mjin",
    "ovl_Bg_Mori_Bigst",
    "ovl_Bg_Mori_Elevator",
    "ovl_Bg_Mori_Hashigo",
    "ovl_Bg_Mori_Hashira4",
    "ovl_Bg_Mori_Hineri",
    "ovl_Bg_Mori_Idomizu",
    "ovl_Bg_Mori_Kaitenkabe",
    "ovl_Bg_Mori_Rakkatenjo",
    "ovl_Bg_Po_Event",
    "ovl_Bg_Po_Syokudai",
    "ovl_Bg_Pushbox",
    "ovl_Bg_Relay_Objects",
    "ovl_Bg_Spot00_Break",
    "ovl_Bg_Spot00_Hanebasi",
    "ovl_Bg_Spot01_Fusya",
    "ovl_Bg_Spot01_Idohashira",
    "ovl_Bg_Spot01_Idomizu",
    "ovl_Bg_Spot01_Idosoko",
    "ovl_Bg_Spot01_Objects2",
    "ovl_Bg_Spot02_Objects",
    "ovl_Bg_Spot03_Taki",
    "ovl_Bg_Spot05_Soko",
    "ovl_Bg_Spot06_Objects",
    "ovl_Bg_Spot07_Taki",
    "ovl_Bg_Spot08_Bakudankabe",
    "ovl_Bg_Spot08_Iceblock",
    "ovl_Bg_Spot09_Obj",
    "ovl_Bg_Spot11_Bakudankabe",
    "ovl_Bg_Spot11_Oasis",
    "ovl_Bg_Spot12_Gate",
    "ovl_Bg_Spot12_Saku",
    "ovl_Bg_Spot15_Rrbox",
    "ovl_Bg_Spot15_Saku",
    "ovl_Bg_Spot16_Bombstone",
    "ovl_Bg_Spot16_Doughnut",
    "ovl_Bg_Spot17_Bakudankabe",
    "ovl_Bg_Spot17_Funen",
    "ovl_Bg_Spot18_Basket",
    "ovl_Bg_Spot18_Futa",
    "ovl_Bg_Spot18_Obj",
    "ovl_Bg_Spot18_Shutter",
    "ovl_Bg_Sst_Floor",
    "ovl_Bg_Toki_Hikari",
    "ovl_Bg_Toki_Swd",
    "ovl_Bg_Treemouth",
    "ovl_Bg_Umajump",
    "ovl_Bg_Vb_Sima",
    "ovl_Bg_Ydan_Hasi",
    "ovl_Bg_Ydan_Maruta",
    "ovl_Bg_Ydan_Sp",
    "ovl_Bg_Zg",
    "ovl_Boss_Dodongo",
    "ovl_Boss_Fd",
    "ovl_Boss_Fd2",
    "ovl_Boss_Ganon",
    "ovl_Boss_Ganon2",
    "ovl_Boss_Ganondrof",
    "ovl_Boss_Goma",
    "ovl_Boss_Mo",
    "ovl_Boss_Sst",
    "ovl_Boss_Tw",
    "ovl_Boss_Va",
    "ovl_Demo_6K",
    "ovl_Demo_Du",
    "ovl_Demo_Ec",
    "ovl_Demo_Effect",
    "ovl_Demo_Ext",
    "ovl_Demo_Geff",
    "ovl_Demo_Gj",
    "ovl_Demo_Go",
    "ovl_Demo_Gt",
    "ovl_Demo_Ik",
    "ovl_Demo_Im",
    "ovl_Demo_Kankyo",
    "ovl_Demo_Kekkai",
    "ovl_Demo_Sa",
    "ovl_Demo_Shd",
    "ovl_Demo_Tre_Lgt",
    "ovl_Door_Ana",
    "ovl_Door_Gerudo",
    "ovl_Door_Killer",
    "ovl_Door_Shutter",
    "ovl_Door_Toki",
    "ovl_Door_Warp1",
    "ovl_Efc_Erupc",
    "ovl_Eff_Dust",
    "ovl_Effect_Ss_Blast",
    "ovl_Effect_Ss_Bomb",
    "ovl_Effect_Ss_Bomb2",
    "ovl_Effect_Ss_Bubble",
    "ovl_Effect_Ss_D_Fire",
    "ovl_Effect_Ss_Dead_Db",
    "ovl_Effect_Ss_Dead_Dd",
    "ovl_Effect_Ss_Dead_Ds",
    "ovl_Effect_Ss_Dead_Sound",
    "ovl_Effect_Ss_Dt_Bubble",
    "ovl_Effect_Ss_Dust",
    "ovl_Effect_Ss_En_Fire",
    "ovl_Effect_Ss_En_Ice",
    "ovl_Effect_Ss_Extra",
    "ovl_Effect_Ss_Fcircle",
    "ovl_Effect_Ss_Fhg_Flash",
    "ovl_Effect_Ss_Fire_Tail",
    "ovl_Effect_Ss_G_Fire",
    "ovl_Effect_Ss_G_Magma",
    "ovl_Effect_Ss_G_Magma2",
    "ovl_Effect_Ss_G_Ripple",
    "ovl_Effect_Ss_G_Spk",
    "ovl_Effect_Ss_G_Splash",
    "ovl_Effect_Ss_Hahen",
    "ovl_Effect_Ss_HitMark",
    "ovl_Effect_Ss_Ice_Piece",
    "ovl_Effect_Ss_Ice_Smoke",
    "ovl_Effect_Ss_K_Fire",
    "ovl_Effect_Ss_Kakera",
    "ovl_Effect_Ss_KiraKira",
    "ovl_Effect_Ss_Lightning",
    "ovl_Effect_Ss_Sibuki",
    "ovl_Effect_Ss_Sibuki2",
    "ovl_Effect_Ss_Solder_Srch_Ball",
    "ovl_Effect_Ss_Stick",
    "ovl_Effect_Ss_Stone1",
    "ovl_Elf_Msg",
    "ovl_Elf_Msg2",
    "ovl_En_Am",
    "ovl_En_Ani",
    "ovl_En_Anubice",
    "ovl_En_Anubice_Fire",
    "ovl_En_Anubice_Tag",
    "ovl_En_Arow_Trap",
    "ovl_En_Arrow",
    "ovl_En_Attack_Niw",
    "ovl_En_Ba",
    "ovl_En_Bb",
    "ovl_En_Bdfire",
    "ovl_En_Bigokuta",
    "ovl_En_Bili",
    "ovl_En_Bird",
    "ovl_En_Blkobj",
    "ovl_En_Bom",
    "ovl_En_Bom_Bowl_Man",
    "ovl_En_Bom_Bowl_Pit",
    "ovl_En_Bom_Chu",
    "ovl_En_Bombf",
    "ovl_En_Boom",
    "ovl_En_Box",
    "ovl_En_Brob",
    "ovl_En_Bubble",
    "ovl_En_Butte",
    "ovl_En_Bw",
    "ovl_En_Bx",
    "ovl_En_Changer",
    "ovl_En_Clear_Tag",
    "ovl_En_Cow",
    "ovl_En_Crow",
    "ovl_En_Cs",
    "ovl_En_Daiku",
    "ovl_En_Daiku_Kakariko",
    "ovl_En_Dekubaba",
    "ovl_En_Dekunuts",
    "ovl_En_Dh",
    "ovl_En_Dha",
    "ovl_En_Diving_Game",
    "ovl_En_Dns",
    "ovl_En_Dnt_Demo",
    "ovl_En_Dnt_Jiji",
    "ovl_En_Dnt_Nomal",
    "ovl_En_Dodojr",
    "ovl_En_Dodongo",
    "ovl_En_Dog",
    "ovl_En_Door",
    "ovl_En_Ds",
    "ovl_En_Du",
    "ovl_En_Dy_Extra",
    "ovl_En_Eg",
    "ovl_En_Eiyer",
    "ovl_En_Elf",
    "ovl_En_Encount1",
    "ovl_En_Encount2",
    "ovl_En_Ex_Item",
    "ovl_En_Ex_Ruppy",
    "ovl_En_Fd",
    "ovl_En_Fd_Fire",
    "ovl_En_Fhg_Fire",
    "ovl_En_Fire_Rock",
    "ovl_En_Firefly",
    "ovl_En_Fish",
    "ovl_En_Floormas",
    "ovl_En_Fr",
    "ovl_En_Fu",
    "ovl_En_Fw",
    "ovl_En_Fz",
    "ovl_En_G_Switch",
    "ovl_En_Ganon_Mant",
    "ovl_En_Ganon_Organ",
    "ovl_En_Gb",
    "ovl_En_Ge1",
    "ovl_En_Ge2",
    "ovl_En_Ge3",
    "ovl_En_GeldB",
    "ovl_En_GirlA",
    "ovl_En_Gm",
    "ovl_En_Go",
    "ovl_En_Go2",
    "ovl_En_Goma",
    "ovl_En_Goroiwa",
    "ovl_En_Gs",
    "ovl_En_Guest",
    "ovl_En_Hata",
    "ovl_En_Heishi1",
    "ovl_En_Heishi2",
    "ovl_En_Heishi3",
    "ovl_En_Heishi4",
    "ovl_En_Hintnuts",
    "ovl_En_Holl",
    "ovl_En_Honotrap",
    "ovl_En_Horse",
    "ovl_En_Horse_Game_Check",
    "ovl_En_Horse_Ganon",
    "ovl_En_Horse_Link_Child",
    "ovl_En_Horse_Normal",
    "ovl_En_Horse_Zelda",
    "ovl_En_Hs",
    "ovl_En_Hs2",
    "ovl_En_Hy",
    "ovl_En_Ice_Hono",
    "ovl_En_Ik",
    "ovl_En_In",
    "ovl_En_Insect",
    "ovl_En_Ishi",
    "ovl_En_It",
    "ovl_En_Jj",
    "ovl_En_Js",
    "ovl_En_Jsjutan",
    "ovl_En_Kakasi",
    "ovl_En_Kakasi2",
    "ovl_En_Kakasi3",
    "ovl_En_Kanban",
    "ovl_En_Karebaba",
    "ovl_En_Ko",
    "ovl_En_Kusa",
    "ovl_En_Kz",
    "ovl_En_Light",
    "ovl_En_Lightbox",
    "ovl_En_M_Fire1",
    "ovl_En_M_Thunder",
    "ovl_En_Ma1",
    "ovl_En_Ma2",
    "ovl_En_Ma3",
    "ovl_En_Mag",
    "ovl_En_Mb",
    "ovl_En_Md",
    "ovl_En_Mk",
    "ovl_En_Mm",
    "ovl_En_Mm2",
    "ovl_En_Ms",
    "ovl_En_Mu",
    "ovl_En_Nb",
    "ovl_En_Niw",
    "ovl_En_Niw_Girl",
    "ovl_En_Niw_Lady",
    "ovl_En_Nutsball",
    "ovl_En_Nwc",
    "ovl_En_Ny",
    "ovl_En_OE2",
    "ovl_En_Okarina_Effect",
    "ovl_En_Okarina_Tag",
    "ovl_En_Okuta",
    "ovl_En_Ossan",
    "ovl_En_Owl",
    "ovl_En_Part",
    "ovl_En_Peehat",
    "ovl_En_Po_Desert",
    "ovl_En_Po_Field",
    "ovl_En_Po_Relay",
    "ovl_En_Po_Sisters",
    "ovl_En_Poh",
    "ovl_En_Pu_box",
    "ovl_En_Rd",
    "ovl_En_Reeba",
    "ovl_En_River_Sound",
    "ovl_En_Rl",
    "ovl_En_Rr",
    "ovl_En_Ru1",
    "ovl_En_Ru2",
    "ovl_En_Sa",
    "ovl_En_Sb",
    "ovl_En_Scene_Change",
    "ovl_En_Sda",
    "ovl_En_Shopnuts",
    "ovl_En_Si",
    "ovl_En_Siofuki",
    "ovl_En_Skb",
    "ovl_En_Skj",
    "ovl_En_Skjneedle",
    "ovl_En_Ssh",
    "ovl_En_St",
    "ovl_En_Sth",
    "ovl_En_Stream",
    "ovl_En_Sw",
    "ovl_En_Syateki_Itm",
    "ovl_En_Syateki_Man",
    "ovl_En_Syateki_Niw",
    "ovl_En_Ta",
    "ovl_En_Takara_Man",
    "ovl_En_Tana",
    "ovl_En_Tg",
    "ovl_En_Tite",
    "ovl_En_Tk",
    "ovl_En_Torch",
    "ovl_En_Torch2",
    "ovl_En_Toryo",
    "ovl_En_Tp",
    "ovl_En_Tr",
    "ovl_En_Trap",
    "ovl_En_Tubo_Trap",
    "ovl_En_Vali",
    "ovl_En_Vase",
    "ovl_En_Vb_Ball",
    "ovl_En_Viewer",
    "ovl_En_Vm",
    "ovl_En_Wall_Tubo",
    "ovl_En_Wallmas",
    "ovl_En_Weather_Tag",
    "ovl_En_Weiyer",
    "ovl_En_Wf",
    "ovl_En_Wonder_Item",
    "ovl_En_Wonder_Talk",
    "ovl_En_Wonder_Talk2",
    "ovl_En_Wood02",
    "ovl_En_Xc",
    "ovl_En_Yabusame_Mark",
    "ovl_En_Yukabyun",
    "ovl_En_Zf",
    "ovl_En_Zl1",
    "ovl_En_Zl2",
    "ovl_En_Zl3",
    "ovl_En_Zl4",
    "ovl_En_Zo",
    "ovl_En_fHG",
    "ovl_End_Title",
    "ovl_Fishing",
    "ovl_Item_B_Heart",
    "ovl_Item_Etcetera",
    "ovl_Item_Inbox",
    "ovl_Item_Ocarina",
    "ovl_Item_Shield",
    "ovl_Magic_Dark",
    "ovl_Magic_Fire",
    "ovl_Magic_Wind",
    "ovl_Mir_Ray",
    "ovl_Obj_Bean",
    "ovl_Obj_Blockstop",
    "ovl_Obj_Bombiwa",
    "ovl_Obj_Comb",
    "ovl_Obj_Dekujr",
    "ovl_Obj_Elevator",
    "ovl_Obj_Hamishi",
    "ovl_Obj_Hana",
    "ovl_Obj_Hsblock",
    "ovl_Obj_Ice_Poly",
    "ovl_Obj_Kibako",
    "ovl_Obj_Kibako2",
    "ovl_Obj_Lift",
    "ovl_Obj_Lightswitch",
    "ovl_Obj_Makekinsuta",
    "ovl_Obj_Makeoshihiki",
    "ovl_Obj_Mure",
    "ovl_Obj_Mure2",
    "ovl_Obj_Mure3",
    "ovl_Obj_Oshihiki",
    "ovl_Obj_Roomtimer",
    "ovl_Obj_Switch",
    "ovl_Obj_Syokudai",
    "ovl_Obj_Timeblock",
    "ovl_Obj_Tsubo",
    "ovl_Obj_Warp2block",
    "ovl_Object_Kankyo",
    "ovl_Oceff_Spot",
    "ovl_Oceff_Storm",
    "ovl_Oceff_Wipe",
    "ovl_Oceff_Wipe2",
    "ovl_Oceff_Wipe3",
    "ovl_Oceff_Wipe4",
    "ovl_Shot_Sun",
    "gameplay_keep",
    "gameplay_field_keep",
    "gameplay_dangeon_keep",
    "gameplay_object_exchange_static",
    "object_link_boy",
    "object_link_child",
    "object_box",////////////////////////


    "object_jya_obj",

 

    "object_ma1",////////////

    "object_gi_bottle_letter",///////////////

    
    "g_pn_01",
    "g_pn_02",
    "g_pn_03",
    "g_pn_04",
    "g_pn_05",
    "g_pn_06",
    "g_pn_07",
    "g_pn_08",
    "g_pn_09",
    "g_pn_10",
    "g_pn_11",
    "g_pn_12",
    "g_pn_13",
    "g_pn_14",
    "g_pn_15",
    "g_pn_16",
    "g_pn_17",
    "g_pn_18",
    "g_pn_19",
    "g_pn_20",
    "g_pn_21",
    "g_pn_22",
    "g_pn_23",
    "g_pn_24",
    "g_pn_25",
    "g_pn_26",
    "g_pn_27",
    "g_pn_28",
    "g_pn_29",
    "g_pn_30",
    "g_pn_31",
    "g_pn_32",
    "g_pn_33",
    "g_pn_34",
    "g_pn_35",
    "g_pn_36",
    "g_pn_37",
    "g_pn_38",
    "g_pn_39",
    "g_pn_40",
    "g_pn_41",
    "g_pn_42",
    "g_pn_43",
    "g_pn_44",
    "g_pn_45",
    "g_pn_46",
    "g_pn_47",
    "g_pn_48",
    "g_pn_49",
    "g_pn_50",
    "g_pn_51",
    "g_pn_52",
    "g_pn_53",
    "g_pn_54",
    "g_pn_55",
    "g_pn_56",
    "g_pn_57",
    "z_select_static",
    "nintendo_rogo_static",
    "title_static",
    "parameter_static",
    "vr_fine0_static",
    "vr_fine0_pal_static",
    "vr_fine1_static",
    "vr_fine1_pal_static",
    "vr_fine2_static",
    "vr_fine2_pal_static",
    "vr_fine3_static",
    "vr_fine3_pal_static",
    "vr_cloud0_static",
    "vr_cloud0_pal_static",
    "vr_cloud1_static",
    "vr_cloud1_pal_static",
    "vr_cloud2_static",
    "vr_cloud2_pal_static",
    "vr_cloud3_static",
    "vr_cloud3_pal_static",
    "vr_holy0_static",
    "vr_holy0_pal_static",
    "vr_holy1_static",
    "vr_holy1_pal_static",
    "vr_MDVR_static",
    "vr_MDVR_pal_static",
    "vr_MNVR_static",
    "vr_MNVR_pal_static",
    "vr_RUVR_static",
    "vr_RUVR_pal_static",
    "vr_LHVR_static",
    "vr_LHVR_pal_static",
    "vr_KHVR_static",
    "vr_KHVR_pal_static",
    "vr_K3VR_static",
    "vr_K3VR_pal_static",
    "vr_K4VR_static",
    "vr_K4VR_pal_static",
    "vr_K5VR_static",
    "vr_K5VR_pal_static",
    "vr_SP1a_static",
    "vr_SP1a_pal_static",
    "vr_MLVR_static",
    "vr_MLVR_pal_static",
    "vr_KKRVR_static",
    "vr_KKRVR_pal_static",
    "vr_KR3VR_static",
    "vr_KR3VR_pal_static",
    "vr_IPVR_static",
    "vr_IPVR_pal_static",
    "vr_KSVR_static",
    "vr_KSVR_pal_static",
    "vr_GLVR_static",
    "vr_GLVR_pal_static",
    "vr_ZRVR_static",
    "vr_ZRVR_pal_static",
    "vr_DGVR_static",
    "vr_DGVR_pal_static",
    "vr_ALVR_static",
    "vr_ALVR_pal_static",
    "vr_NSVR_static",
    "vr_NSVR_pal_static",
    "vr_LBVR_static",
    "vr_LBVR_pal_static",
    "vr_TTVR_static",
    "vr_TTVR_pal_static",
    "vr_FCVR_static",
    "vr_FCVR_pal_static",
    "elf_message_field",
    "elf_message_ydan",

    "test01_scene",
    "test01_room_0",
    "bump_texture_static",
    "anime_model_1_static",
    "anime_model_2_static",
    "anime_model_3_static",
    "anime_model_4_static",
    "anime_model_5_static",
    "anime_model_6_static",
    "anime_texture_1_static",
    "anime_texture_2_static",
    "anime_texture_3_static",
    "anime_texture_4_static",
    "anime_texture_5_static",
    "anime_texture_6_static",
    "softsprite_matrix_static",
};

s32 DmaMgr_CompareName(const char* name1, const char* name2) {
    while (*name1 != 0u) {
        if (*name1 > *name2) {
            return 1;
        }

        if (*name1 < *name2) {
            return -1;
        }

        name1++;
        name2++;
    }

    if (*name2 > 0) {
        return -1;
    }

    return 0;
}

s32 DmaMgr_DMARomToRam(u32 rom, u32 ram, u32 size) {
    OSIoMesg ioMsg;
    OSMesgQueue queue;
    OSMesg msg;
    s32 ret;
    u32 buffSize = gDmaMgrDmaBuffSize;
    s32 pad[2];

    if (buffSize == 0) {
        buffSize = 0x2000;
    }

    osInvalICache((void*)ram, size);
    osInvalDCache((void*)ram, size);
    osCreateMesgQueue(&queue, &msg, 1);

    while (size > buffSize) {
        if (1) {} // Necessary to match

        ioMsg.hdr.pri = OS_MESG_PRI_NORMAL;
        ioMsg.hdr.retQueue = &queue;
        ioMsg.devAddr = rom;
        ioMsg.dramAddr = (void*)ram;
        ioMsg.size = buffSize;

        if (D_80009460 == 10) {
            osSyncPrintf("%10lld ノーマルＤＭＡ %08x %08x %08x (%d)\n", OS_CYCLES_TO_USEC(osGetTime()), ioMsg.dramAddr,
                         ioMsg.devAddr, ioMsg.size, gPiMgrCmdQ.validCount);
        }

        ret = osEPiStartDma(gCartHandle, &ioMsg, OS_READ);
        if (ret) {
            goto DmaMgr_DMARomToRam_end;
        }

        if (D_80009460 == 10) {
            osSyncPrintf("%10lld ノーマルＤＭＡ START (%d)\n", OS_CYCLES_TO_USEC(osGetTime()), gPiMgrCmdQ.validCount);
        }

        osRecvMesg(&queue, NULL, 1);
        if (D_80009460 == 10) {
            osSyncPrintf("%10lld ノーマルＤＭＡ END (%d)\n", OS_CYCLES_TO_USEC(osGetTime()), gPiMgrCmdQ.validCount);
        }

        size -= buffSize;
        rom += buffSize;
        ram += buffSize;
    }

    if (1) {} // Also necessary to match

    ioMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    ioMsg.hdr.retQueue = &queue;
    ioMsg.devAddr = rom;
    ioMsg.dramAddr = (void*)ram;
    ioMsg.size = size;

    if (D_80009460 == 10) {
        osSyncPrintf("%10lld ノーマルＤＭＡ %08x %08x %08x (%d)\n", OS_CYCLES_TO_USEC(osGetTime()), ioMsg.dramAddr,
                     ioMsg.devAddr, ioMsg.size, gPiMgrCmdQ.validCount);
    }

    ret = osEPiStartDma(gCartHandle, &ioMsg, OS_READ);
    if (ret) {
        goto DmaMgr_DMARomToRam_end;
    }

    osRecvMesg(&queue, NULL, 1);
    if (D_80009460 == 10) {
        osSyncPrintf("%10lld ノーマルＤＭＡ END (%d)\n", OS_CYCLES_TO_USEC(osGetTime()), gPiMgrCmdQ.validCount);
    }

DmaMgr_DMARomToRam_end:
    osInvalICache((void*)ram, size);
    osInvalDCache((void*)ram, size);

    return ret;
}

s32 DmaMgr_DmaCallback0(OSPiHandle* pihandle, OSIoMesg* mb, s32 direction) {
    s32 ret;

    ASSERT(pihandle == gCartHandle, "pihandle == carthandle", "../z_std_dma.c", 530);
    ASSERT(direction == OS_READ, "direction == OS_READ", "../z_std_dma.c", 531);
    ASSERT(mb != NULL, "mb != NULL", "../z_std_dma.c", 532);

    if (D_80009460 == 10) {
        osSyncPrintf("%10lld サウンドＤＭＡ %08x %08x %08x (%d)\n", OS_CYCLES_TO_USEC(osGetTime()), mb->dramAddr,
                     mb->devAddr, mb->size, gPiMgrCmdQ.validCount);
    }

    ret = osEPiStartDma(pihandle, mb, direction);
    if (ret) {
        osSyncPrintf("OOPS!!\n");
    }
    return ret;
}

void DmaMgr_DmaCallback1(u32 ram, u32 rom, u32 size) {
    OSPiHandle* handle = osDriveRomInit();
    OSMesgQueue queue;
    OSMesg msg;
    OSIoMesg ioMsg;

    osInvalICache((void*)ram, size);
    osInvalDCache((void*)ram, size);
    osCreateMesgQueue(&queue, &msg, 1);

    ioMsg.hdr.retQueue = &queue;
    ioMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    ioMsg.devAddr = rom;
    ioMsg.dramAddr = (void*)ram;
    ioMsg.size = size;
    handle->transferInfo.cmdType = 2;

    osEPiStartDma(handle, &ioMsg, 0);
    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
    return;
}

void DmaMgr_Error(DmaRequest* req, const char* file, const char* errorName, const char* errorDesc) {
    u32 vrom = req->vromAddr;
    u32 ram = (u32)req->dramAddr;
    u32 size = req->size;
    char buff1[80];
    char buff2[80];

    osSyncPrintf("%c", 7);
    osSyncPrintf(VT_FGCOL(RED));
    osSyncPrintf("DMA致命的エラー(%s)\nROM:%X RAM:%X SIZE:%X %s\n",
                 errorDesc != NULL ? errorDesc : (errorName != NULL ? errorName : "???"), vrom, ram, size,
                 file != NULL ? file : "???");

    if (req->filename) {
        osSyncPrintf("DMA ERROR: %s %d", req->filename, req->line);
    } else if (sDmaMgrCurFileName) {
        osSyncPrintf("DMA ERROR: %s %d", sDmaMgrCurFileName, sDmaMgrCurFileLine);
    }

    osSyncPrintf(VT_RST);

    if (req->filename) {
        sprintf(buff1, "DMA ERROR: %s %d", req->filename, req->line);
    } else if (sDmaMgrCurFileName) {
        sprintf(buff1, "DMA ERROR: %s %d", sDmaMgrCurFileName, sDmaMgrCurFileLine);
    } else {
        sprintf(buff1, "DMA ERROR: %s", errorName != NULL ? errorName : "???");
    }

    sprintf(buff2, "%07X %08X %X %s", vrom, ram, size, file != NULL ? file : "???");
    Fault_AddHungupAndCrashImpl(buff1, buff2);
}

const char* DmaMgr_GetFileNameImpl(u32 vrom) {
    DmaEntry* iter = gDmaDataTable;
    const char** name = sDmaMgrFileNames;

    while (iter->vromEnd) {
        if (vrom >= iter->vromStart && vrom < iter->vromEnd) {
            return *name;
        }

        iter++;
        name++;
    }
    //! @bug Since there is no return, in case the file isn't found, the return value will be a pointer to the end
    // of gDmaDataTable
#ifdef AVOID_UB
    return *name;
#endif
}

const char* DmaMgr_GetFileName(u32 vrom) {
    const char* ret = DmaMgr_GetFileNameImpl(vrom);

    if (ret == NULL) {
        return "(unknown)";
    }

    if (DmaMgr_CompareName(ret, "kanji") == 0 || DmaMgr_CompareName(ret, "link_animetion") == 0) {
        return NULL;
    }

    return ret;
}

void DmaMgr_ProcessMsg(DmaRequest* req) {
    u32 vrom = req->vromAddr;
    void* ram = req->dramAddr;
    u32 size = req->size;
    u32 romStart;
    u32 romSize;
    u8 found = false;
    DmaEntry* iter;
    const char* filename;

    if (0) {
        // the string is defined in .rodata but not used, suggesting
        // a debug print is here but optimized out in some way
        osSyncPrintf("DMA ROM:%08X RAM:%08X SIZE:%08X %s\n");
        // the last arg of this print looks like it may be filename, but
        // filename above this block does not match
    }

    filename = DmaMgr_GetFileName(vrom);
    iter = gDmaDataTable;

    while (iter->vromEnd) {
        if (vrom >= iter->vromStart && vrom < iter->vromEnd) {
            if (1) {} // Necessary to match

            if (iter->romEnd == 0) {
                if (iter->vromEnd < vrom + size) {
                    DmaMgr_Error(req, filename, "Segment Alignment Error",
                                 "セグメント境界をまたがってＤＭＡ転送することはできません");
                }

                DmaMgr_DMARomToRam(iter->romStart + (vrom - iter->vromStart), (u32)ram, size);
                found = true;

                if (0) {
                    osSyncPrintf("No Press ROM:%08X RAM:%08X SIZE:%08X\n", vrom, ram, size);
                }
            } else {
                romStart = iter->romStart;
                romSize = iter->romEnd - iter->romStart;

                if (vrom != iter->vromStart) {
                    DmaMgr_Error(req, filename, "Can't Transfer Segment",
                                 "圧縮されたセグメントの途中からはＤＭＡ転送することはできません");
                }

                if (size != iter->vromEnd - iter->vromStart) {
                    DmaMgr_Error(req, filename, "Can't Transfer Segment",
                                 "圧縮されたセグメントの一部だけをＤＭＡ転送することはできません");
                }

                osSetThreadPri(NULL, Z_PRIORITY_MAIN);
                Yaz0_Decompress(romStart, ram, romSize);
                osSetThreadPri(NULL, Z_PRIORITY_DMAMGR);
                found = true;

                if (0) {
                    osSyncPrintf("   Press ROM:%X RAM:%X SIZE:%X\n", vrom, ram, size);
                }
            }
            break;
        }
        iter++;
    }

    if (!found) {
        if (sDmaMgrDataExistError) {
            DmaMgr_Error(req, NULL, "DATA DON'T EXIST", "該当するデータが存在しません");
            return;
        }

        DmaMgr_DMARomToRam(vrom, (u32)ram, size);

        if (0) {
            osSyncPrintf("No Press ROM:%08X RAM:%08X SIZE:%08X (非公式)\n", vrom, ram, size);
        }
    }
}

void DmaMgr_ThreadEntry(void* arg0) {
    OSMesg msg;
    DmaRequest* req;

    osSyncPrintf("ＤＭＡマネージャスレッド実行開始\n");
    while (true) {
        osRecvMesg(&sDmaMgrMsgQueue, &msg, 1);
        req = (DmaRequest*)msg;
        if (req == NULL) {
            break;
        }

        if (0) {
            osSyncPrintf("ＤＭＡ登録受付 dmap=%08x\n", req);
        }

        DmaMgr_ProcessMsg(req);
        if (req->notifyQueue) {
            osSendMesg(req->notifyQueue, req->notifyMsg, OS_MESG_NOBLOCK);
            if (0) {
                osSyncPrintf("osSendMesg: dmap=%08x, mq=%08x, m=%08x \n", req, req->notifyQueue, req->notifyMsg);
            }
        }
    }
    osSyncPrintf("ＤＭＡマネージャスレッド実行終了\n");
}

s32 DmaMgr_SendRequestImpl(DmaRequest* req, u32 ram, u32 vrom, u32 size, u32 unk, OSMesgQueue* queue, OSMesg msg) {
    static s32 sDmaMgrQueueFullLogged = 0;

    if ((1 && (ram == 0)) || (osMemSize < ram + size + 0x80000000) || (vrom & 1) || (vrom > 0x4000000) ||
        (size == 0) || (size & 1)) {
        DmaMgr_Error(req, NULL, "ILLIGAL DMA-FUNCTION CALL", "パラメータ異常です");
    }

    req->vromAddr = vrom;
    req->dramAddr = (void*)ram;
    req->size = size;
    req->unk_14 = 0;
    req->notifyQueue = queue;
    req->notifyMsg = msg;

    if (1) {
        if ((sDmaMgrQueueFullLogged == 0) && (sDmaMgrMsgQueue.validCount >= sDmaMgrMsgQueue.msgCount)) {
            sDmaMgrQueueFullLogged++;
            osSyncPrintf("%c", 7);
            osSyncPrintf(VT_FGCOL(RED));
            osSyncPrintf("dmaEntryMsgQが一杯です。キューサイズの再検討をおすすめします。");
            LOG_NUM("(sizeof(dmaEntryMsgBufs) / sizeof(dmaEntryMsgBufs[0]))", ARRAY_COUNT(sDmaMgrMsgs),
                    "../z_std_dma.c", 952);
            osSyncPrintf(VT_RST);
        }
    }

    osSendMesg(&sDmaMgrMsgQueue, req, OS_MESG_BLOCK);
    return 0;
}

s32 DmaMgr_SendRequest0(u32 ram, u32 vrom, u32 size) {
    DmaRequest req;
    OSMesgQueue queue;
    OSMesg msg;
    s32 ret;

    osCreateMesgQueue(&queue, &msg, 1);
    ret = DmaMgr_SendRequestImpl(&req, ram, vrom, size, 0, &queue, NULL);
    if (ret == -1) {
        return ret;
    }

    osRecvMesg(&queue, NULL, OS_MESG_BLOCK);
    return 0;
}

void DmaMgr_Init(void) {
    const char** name;
    s32 idx;
    DmaEntry* iter;

    DmaMgr_DMARomToRam((u32)_dmadataSegmentRomStart, (u32)_dmadataSegmentStart,
                       (u32)(_dmadataSegmentRomEnd - _dmadataSegmentRomStart));
    osSyncPrintf("dma_rom_ad[]\n");

    sDmaMgrDataExistError = 0;
    name = sDmaMgrFileNames;
    iter = gDmaDataTable;
    idx = 0;

    while (iter->vromEnd != 0) {
        if (iter->romEnd != 0) {
            sDmaMgrDataExistError = 1;
        }

        osSyncPrintf(
            "%3d %08x %08x %08x %08x %08x %c %s\n", idx, iter->vromStart, iter->vromEnd, iter->romStart, iter->romEnd,
            (iter->romEnd != 0) ? iter->romEnd - iter->romStart : iter->vromEnd - iter->vromStart,
            (((iter->romEnd != 0) ? iter->romEnd - iter->romStart : 0) > 0x10000) ? '*' : ' ', name ? *name : "");

        idx++;
        iter++;

        if (name) {
            name++;
        }
    }

    if ((u32)_bootSegmentRomStart != gDmaDataTable[0].vromEnd) {
        osSyncPrintf("_bootSegmentRomStart(%08x) != dma_rom_ad[0].rom_b(%08x)\n", _bootSegmentRomStart,
                     gDmaDataTable[0].vromEnd);
        Fault_AddHungupAndCrash("../z_std_dma.c", 1055);
    }

    osCreateMesgQueue(&sDmaMgrMsgQueue, sDmaMgrMsgs, ARRAY_COUNT(sDmaMgrMsgs));
    StackCheck_Init(&sDmaMgrStackInfo, sDmaMgrStack, sDmaMgrStack + sizeof(sDmaMgrStack), 0, 0x100, "dmamgr");
    osCreateThread(&sDmaMgrThread, 0x12, &DmaMgr_ThreadEntry, 0, sDmaMgrStack + sizeof(sDmaMgrStack),
                   Z_PRIORITY_DMAMGR);
    osStartThread(&sDmaMgrThread);
}

BAD_RETURN(s32) DmaMgr_SendRequest2(DmaRequest* req, u32 ram, u32 vrom, u32 size, u32 unk5, OSMesgQueue* queue, OSMesg msg,
                        const char* file, s32 line) {
    req->filename = file;
    req->line = line;
    DmaMgr_SendRequestImpl(req, ram, vrom, size, unk5, queue, msg);
}

s32 DmaMgr_SendRequest1(void* ram0, u32 vrom, u32 size, const char* file, s32 line) {
    DmaRequest req;
    s32 ret;
    OSMesgQueue queue;
    OSMesg msg;
    u32 ram = (u32)ram0;

    req.filename = file;
    req.line = line;
    osCreateMesgQueue(&queue, &msg, 1);
    ret = DmaMgr_SendRequestImpl(&req, ram, vrom, size, 0, &queue, 0);
    if (ret == -1) {
        return ret;
    }

    osRecvMesg(&queue, NULL, 1);
    return 0;
}
