#include <ultra64.h>
#include <z64.h>
#include "takaraya_room_1.h"
#include <z64.h>
#include <segment_symbols.h>
#include <command_macros_base.h>
#include <z64cutscene_commands.h>
#include <variables.h>
#include "takaraya_scene.h"



SCmdEchoSettings _takaraya_room_1_set0000_cmd00 = { 0x16, 0, { 0 }, 0x00 }; // 0x0000
SCmdRoomBehavior _takaraya_room_1_set0000_cmd01 = { 0x08, 0x02, 0x00000100 }; // 0x0008
SCmdSkyboxDisables _takaraya_room_1_set0000_cmd02 = { 0x12, 0, 0, 0, 0x01, 0x01 }; // 0x0010
SCmdTimeSettings _takaraya_room_1_set0000_cmd03 = { 0x10, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00 }; // 0x0018
SCmdMesh _takaraya_room_1_set0000_cmd04 = { 0x0A, 0, (u32)&_takaraya_room_1_meshHeader_00000080 }; // 0x0020
SCmdObjectList _takaraya_room_1_set0000_cmd05 = { 0x0B, 0x06, (u32)_takaraya_room_1_objectList_00000040 }; // 0x0028
SCmdActorList _takaraya_room_1_set0000_cmd06 = { 0x01, 0x03, (u32)_takaraya_room_1_actorList_0000004C }; // 0x0030
SCmdEndMarker _takaraya_room_1_set0000_cmd07 = { 0x14, 0x00, 0x00 }; // 0x0038
s16 _takaraya_room_1_objectList_00000040[] = 
{
	OBJECT_BOX,
	OBJECT_TS,
	OBJECT_GI_HEARTS,
	OBJECT_GI_KEY,
	OBJECT_GI_RUPY,
	OBJECT_DOG,
};

ActorEntry _takaraya_room_1_actorList_0000004C[3] = 
{
	{ ACTOR_EN_LIGHT, 88, 78, -409, 0, 0, 0, 0x03F5 }, //0x0000004C 
	{ ACTOR_EN_LIGHT, -67, 78, -409, 0, 0, 0, 0x03F5 }, //0x0000005C 
	{ ACTOR_EN_CHANGER, 23, 20, -232, 0, 0, 0, 0x0000 }, //0x0000006C 
};

static u32 pad7C = 0;

MeshHeader0 _takaraya_room_1_meshHeader_00000080 = { { 0 }, 0x01, (u32)&_takaraya_room_1_meshDListEntry_0000008C, (u32)&(_takaraya_room_1_meshDListEntry_0000008C) + sizeof(_takaraya_room_1_meshDListEntry_0000008C) };

MeshEntry0 _takaraya_room_1_meshDListEntry_0000008C[1] = 
{
	{ (u32)_takaraya_room_1_dlist_000017D0, 0 },
};

static u32 terminatorMaybe = 0x01000000; // This always appears after the mesh entries. Its purpose is not clear.
static u32 pad98 = 0;
static u32 pad9C = 0;

Vtx_t _takaraya_room_1_vertices_000000A0[16] = 
{
	 { -20, 0, -460, 0, -512, -9216, 255, 255, 255, 255 }, // 0x000000A0
	 { -20, 0, -440, 0, -512, -8704, 255, 255, 255, 255 }, // 0x000000B0
	 { 40, 0, -440, 0, 1024, -8704, 255, 255, 255, 255 }, // 0x000000C0
	 { 40, 0, -460, 0, 1024, -9216, 255, 255, 255, 255 }, // 0x000000D0
	 { -20, 0, -40, 0, -512, 1536, 114, 114, 114, 255 }, // 0x000000E0
	 { -20, 0, -20, 0, -512, 2048, 114, 114, 114, 255 }, // 0x000000F0
	 { 40, 0, -20, 0, 1024, 2048, 114, 114, 114, 255 }, // 0x00000100
	 { 40, 0, -40, 0, 1024, 1536, 114, 114, 114, 255 }, // 0x00000110
	 { -160, 0, -160, 0, -5120, -2048, 153, 153, 153, 255 }, // 0x00000120
	 { -200, 0, -40, 0, -6144, 1024, 114, 114, 114, 255 }, // 0x00000130
	 { 240, 0, -40, 0, 5120, 1024, 114, 114, 114, 255 }, // 0x00000140
	 { 200, 0, -160, 0, 4096, -2048, 153, 153, 153, 255 }, // 0x00000150
	 { 240, 0, -440, 0, 5120, -9216, 255, 255, 255, 255 }, // 0x00000160
	 { -200, 0, -440, 0, -6144, -9216, 255, 255, 255, 255 }, // 0x00000170
	 { -160, 0, -320, 0, -5120, -6144, 153, 153, 153, 255 }, // 0x00000180
	 { 200, 0, -320, 0, 4096, -6144, 153, 153, 153, 255 }, // 0x00000190
};

Vtx_t _takaraya_room_1_vertices_000001A0[8] = 
{
	 { -200, 0, -460, 0, 0, 0, 0, 0, 0, 0 }, // 0x000001A0
	 { 240, 0, -460, 0, 0, 0, 0, 0, 0, 0 }, // 0x000001B0
	 { -200, 0, -460, 0, 0, 0, 0, 0, 0, 0 }, // 0x000001C0
	 { 240, 0, -460, 0, 0, 0, 0, 0, 0, 0 }, // 0x000001D0
	 { -200, 0, -20, 0, 0, 0, 0, 0, 0, 0 }, // 0x000001E0
	 { 240, 0, -20, 0, 0, 0, 0, 0, 0, 0 }, // 0x000001F0
	 { -200, 0, -20, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000200
	 { 240, 0, -20, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000210
};

Gfx _takaraya_room_1_dlist_00000220[] =
{
	gsDPPipeSync(), // 0x00000220
	gsSPGeometryMode(0xFF030000, 0x00000000), // 0x00000228
	gsSPVertex(_takaraya_room_1_vertices_000001A0, 8, 0), // 0x00000230
	gsSPCullDisplayList(0, 7), // 0x00000238
	gsDPPipeSync(), // 0x00000240
	gsSPGeometryMode(0xFF000000, 0x00030000), // 0x00000248
	gsSPSetOtherMode(0xE3, 14, 2, 0x00000000), // 0x00000250
	gsSPTexture(65535, 65535, 0, 0, 1), // 0x00000258
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, _takaraya_scene_tex_000059B0), // 0x00000260
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 0, 5, 0, 0, 5, 0), // 0x00000268
	gsDPLoadSync(), // 0x00000270
	gsDPLoadBlock(7, 0, 0, 1023, 256), // 0x00000278
	gsDPPipeSync(), // 0x00000280
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, 0, 5, 0, 0, 5, 0), // 0x00000288
	gsDPSetTileSize(0, 0, 0, 124, 124), // 0x00000290
	gsDPSetCombineLERP(TEXEL0, K5, SHADE, COMBINED_ALPHA, 0, 0, 0, 1, COMBINED, K5, PRIMITIVE, COMBINED_ALPHA, 0, 0, 0, COMBINED), // 0x00000298
	gsSPSetOtherMode(0xE2, 3, 29, 0xC8112078), // 0x000002A0
	gsSPGeometryMode(0xFF0E0000, 0x00000000), // 0x000002A8
	gsSPGeometryMode(0xFF000000, 0x00010400), // 0x000002B0
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255), // 0x000002B8
	gsSPVertex(_takaraya_room_1_vertices_000000A0, 16, 0), // 0x000002C0
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), // 0x000002C8
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0), // 0x000002D0
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0), // 0x000002D8
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0), // 0x000002E0
	gsSP2Triangles(13, 9, 8, 0, 13, 8, 14, 0), // 0x000002E8
	gsSP2Triangles(11, 10, 12, 0, 11, 12, 15, 0), // 0x000002F0
	gsSPEndDisplayList(), // 0x000002F8
};

Vtx_t _takaraya_room_1_vertices_00000300[32] = 
{
	 { 80, 20, -320, 0, -3072, 0, 255, 255, 255, 255 }, // 0x00000300
	 { 80, 10, -320, 0, -3072, 512, 255, 255, 255, 255 }, // 0x00000310
	 { 80, 10, -300, 0, -2048, 512, 255, 255, 255, 255 }, // 0x00000320
	 { 80, 20, -300, 0, -2048, 0, 255, 255, 255, 255 }, // 0x00000330
	 { 80, 20, -180, 0, 4096, 0, 89, 89, 89, 255 }, // 0x00000340
	 { 80, 10, -160, 0, 5120, 512, 89, 89, 89, 255 }, // 0x00000350
	 { 80, 20, -160, 0, 5120, 0, 178, 178, 178, 255 }, // 0x00000360
	 { 80, 10, -180, 0, 4096, 512, 89, 89, 89, 255 }, // 0x00000370
	 { -40, 20, -300, 0, -2048, 0, 255, 255, 255, 255 }, // 0x00000380
	 { -40, 10, -300, 0, -2048, 512, 255, 255, 255, 255 }, // 0x00000390
	 { -40, 10, -320, 0, -3072, 512, 255, 255, 255, 255 }, // 0x000003A0
	 { -40, 20, -320, 0, -3072, 0, 255, 255, 255, 255 }, // 0x000003B0
	 { -40, 20, -160, 0, 5120, 0, 178, 178, 178, 255 }, // 0x000003C0
	 { -40, 10, -160, 0, 5120, 512, 89, 89, 89, 255 }, // 0x000003D0
	 { -40, 20, -180, 0, 4096, 0, 89, 89, 89, 255 }, // 0x000003E0
	 { -40, 10, -180, 0, 4096, 512, 89, 89, 89, 255 }, // 0x000003F0
	 { 80, 10, -300, 0, 0, -512, 255, 255, 255, 255 }, // 0x00000400
	 { -40, 10, -300, 0, 6144, -512, 255, 255, 255, 255 }, // 0x00000410
	 { -40, 20, -300, 0, 6144, -1024, 255, 255, 255, 255 }, // 0x00000420
	 { 80, 20, -300, 0, 0, -1024, 255, 255, 255, 255 }, // 0x00000430
	 { 80, 20, -180, 0, 5120, 0, 255, 255, 255, 255 }, // 0x00000440
	 { -40, 20, -180, 0, -1024, 0, 255, 255, 255, 255 }, // 0x00000450
	 { -40, 10, -180, 0, -1024, 512, 89, 89, 89, 255 }, // 0x00000460
	 { 80, 10, -180, 0, 5120, 512, 89, 89, 89, 255 }, // 0x00000470
	 { -40, 20, -160, 0, -1024, 0, 255, 255, 255, 255 }, // 0x00000480
	 { -160, 20, -160, 0, -7168, 0, 89, 89, 89, 255 }, // 0x00000490
	 { -160, 0, -160, 0, -7168, 1024, 51, 51, 51, 255 }, // 0x000004A0
	 { -40, 10, -160, 0, -1024, 512, 140, 140, 140, 255 }, // 0x000004B0
	 { 80, 10, -160, 0, 5120, 512, 140, 140, 140, 255 }, // 0x000004C0
	 { 200, 0, -160, 0, 11264, 1024, 51, 51, 51, 255 }, // 0x000004D0
	 { 200, 20, -160, 0, 11264, 0, 89, 89, 89, 255 }, // 0x000004E0
	 { 80, 20, -160, 0, 5120, 0, 140, 140, 140, 255 }, // 0x000004F0
};

Vtx_t _takaraya_room_1_vertices_00000500[32] = 
{
	 { -160, 0, -160, 0, 5120, 1024, 51, 51, 51, 255 }, // 0x00000500
	 { -160, 20, -160, 0, 5120, 0, 127, 127, 127, 255 }, // 0x00000510
	 { -160, 20, -320, 0, -3072, 0, 178, 178, 178, 255 }, // 0x00000520
	 { -160, 0, -320, 0, -3072, 1024, 178, 178, 178, 255 }, // 0x00000530
	 { 80, 20, -320, 0, 0, -1024, 255, 255, 255, 255 }, // 0x00000540
	 { 200, 20, -320, 0, -6144, -1024, 255, 255, 255, 255 }, // 0x00000550
	 { 200, 0, -320, 0, -6144, 0, 255, 255, 255, 255 }, // 0x00000560
	 { 80, 10, -320, 0, 0, -512, 255, 255, 255, 255 }, // 0x00000570
	 { -40, 10, -320, 0, 6144, -512, 255, 255, 255, 255 }, // 0x00000580
	 { -160, 0, -320, 0, 12288, 0, 255, 255, 255, 255 }, // 0x00000590
	 { -160, 20, -320, 0, 12288, -1024, 255, 255, 255, 255 }, // 0x000005A0
	 { -40, 20, -320, 0, 6144, -1024, 255, 255, 255, 255 }, // 0x000005B0
	 { 200, 0, -320, 0, -3072, 1024, 178, 178, 178, 255 }, // 0x000005C0
	 { 200, 20, -320, 0, -3072, 0, 178, 178, 178, 255 }, // 0x000005D0
	 { 200, 20, -160, 0, 5120, 0, 127, 127, 127, 255 }, // 0x000005E0
	 { 200, 0, -160, 0, 5120, 1024, 51, 51, 51, 255 }, // 0x000005F0
	 { -20, 0, -40, 0, 0, 1024, 178, 178, 178, 255 }, // 0x00000600
	 { -20, 100, -40, 0, 0, 6144, 114, 114, 114, 255 }, // 0x00000610
	 { -20, 100, -20, 0, 1024, 6144, 114, 114, 114, 255 }, // 0x00000620
	 { -20, 0, -20, 0, 1024, 1024, 178, 178, 178, 255 }, // 0x00000630
	 { 40, 100, -40, 0, 0, 1024, 76, 76, 76, 255 }, // 0x00000640
	 { 40, 100, -20, 0, 1024, 1024, 76, 76, 76, 255 }, // 0x00000650
	 { -20, 100, -20, 0, 1024, -2048, 76, 76, 76, 255 }, // 0x00000660
	 { -20, 100, -40, 0, 0, -2048, 76, 76, 76, 255 }, // 0x00000670
	 { 40, 100, -20, 0, 1024, -4096, 114, 114, 114, 255 }, // 0x00000680
	 { 40, 100, -40, 0, 0, -4096, 114, 114, 114, 255 }, // 0x00000690
	 { 40, 0, -40, 0, 0, 1024, 178, 178, 178, 255 }, // 0x000006A0
	 { 40, 0, -20, 0, 1024, 1024, 178, 178, 178, 255 }, // 0x000006B0
	 { 240, 0, -440, 0, 0, 1024, 255, 255, 255, 255 }, // 0x000006C0
	 { 240, 0, -40, 0, 6827, 1024, 89, 89, 89, 255 }, // 0x000006D0
	 { 240, 240, -440, 0, 0, -3072, 51, 51, 51, 255 }, // 0x000006E0
	 { 240, 240, -40, 0, 6827, -3072, 51, 51, 51, 255 }, // 0x000006F0
};

Vtx_t _takaraya_room_1_vertices_00000700[32] = 
{
	 { 40, 0, -440, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00000700
	 { 40, 100, -440, 0, 1024, 3584, 114, 114, 114, 255 }, // 0x00000710
	 { 40, 100, -460, 0, 512, 3584, 114, 114, 114, 255 }, // 0x00000720
	 { 40, 0, -460, 0, 512, 1024, 255, 255, 255, 255 }, // 0x00000730
	 { -20, 100, -460, 0, 512, 3584, 114, 114, 114, 255 }, // 0x00000740
	 { -20, 100, -440, 0, 1024, 3584, 114, 114, 114, 255 }, // 0x00000750
	 { -20, 0, -440, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00000760
	 { -20, 0, -460, 0, 512, 1024, 255, 255, 255, 255 }, // 0x00000770
	 { 40, 100, -460, 0, 512, 1024, 76, 76, 76, 255 }, // 0x00000780
	 { 40, 100, -440, 0, 1024, 1024, 76, 76, 76, 255 }, // 0x00000790
	 { -20, 100, -440, 0, 1024, -512, 76, 76, 76, 255 }, // 0x000007A0
	 { -20, 100, -460, 0, 512, -512, 76, 76, 76, 255 }, // 0x000007B0
	 { -200, 240, -440, 0, 0, -3072, 51, 51, 51, 255 }, // 0x000007C0
	 { -200, 0, -40, 0, 6827, 1024, 89, 89, 89, 255 }, // 0x000007D0
	 { -200, 0, -440, 0, 0, 1024, 255, 255, 255, 255 }, // 0x000007E0
	 { -200, 240, -40, 0, 6827, -3072, 51, 51, 51, 255 }, // 0x000007F0
	 { 240, 240, -40, 0, 6827, -6485, 38, 38, 38, 255 }, // 0x00000800
	 { 93, 240, -440, 0, 0, -3982, 178, 178, 178, 255 }, // 0x00000810
	 { 240, 240, -440, 0, 0, -6485, 38, 38, 38, 255 }, // 0x00000820
	 { -200, 240, -40, 0, 6827, 1024, 38, 38, 38, 255 }, // 0x00000830
	 { -53, 240, -440, 0, 0, -1479, 178, 178, 178, 255 }, // 0x00000840
	 { -200, 240, -440, 0, 0, 1024, 38, 38, 38, 255 }, // 0x00000850
	 { 60, 0, -440, 0, -3413, 1024, 229, 229, 229, 255 }, // 0x00000860
	 { 240, 0, -440, 0, -6485, 1024, 178, 178, 178, 255 }, // 0x00000870
	 { 120, 80, -440, 0, -4437, -341, 255, 255, 255, 255 }, // 0x00000880
	 { 240, 240, -440, 0, -6485, -3072, 76, 76, 76, 255 }, // 0x00000890
	 { 93, 240, -440, 0, -3982, -3072, 114, 114, 114, 255 }, // 0x000008A0
	 { 60, 120, -440, 0, -3413, -1024, 153, 153, 153, 255 }, // 0x000008B0
	 { 40, 100, -440, 0, 0, -4096, 153, 153, 153, 255 }, // 0x000008C0
	 { 40, 0, -440, 0, 0, 1024, 255, 255, 255, 255 }, // 0x000008D0
	 { 60, 0, -440, 0, 1024, 1024, 229, 229, 229, 255 }, // 0x000008E0
	 { 60, 120, -440, 0, 1024, -5120, 153, 153, 153, 255 }, // 0x000008F0
};

Vtx_t _takaraya_room_1_vertices_00000900[32] = 
{
	 { -53, 240, -440, 0, -1479, -3072, 114, 114, 114, 255 }, // 0x00000900
	 { -40, 120, -440, 0, -1707, -1024, 153, 153, 153, 255 }, // 0x00000910
	 { 60, 120, -440, 0, -3413, -1024, 153, 153, 153, 255 }, // 0x00000920
	 { 93, 240, -440, 0, -3982, -3072, 114, 114, 114, 255 }, // 0x00000930
	 { -20, 100, -440, 0, 1038, -14, 153, 153, 153, 255 }, // 0x00000940
	 { 40, 100, -440, 0, 1038, -3128, 153, 153, 153, 255 }, // 0x00000950
	 { 60, 120, -440, 0, 0, -4166, 153, 153, 153, 255 }, // 0x00000960
	 { -40, 120, -440, 0, 0, 1024, 153, 153, 153, 255 }, // 0x00000970
	 { -40, 0, -440, 0, 0, 1024, 229, 229, 229, 255 }, // 0x00000980
	 { -20, 0, -440, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00000990
	 { -20, 100, -440, 0, 1024, -4096, 153, 153, 153, 255 }, // 0x000009A0
	 { -40, 120, -440, 0, 0, -5120, 153, 153, 153, 255 }, // 0x000009B0
	 { -93, 80, -440, 0, -796, -341, 255, 255, 255, 255 }, // 0x000009C0
	 { -200, 240, -440, 0, 1024, -3072, 76, 76, 76, 255 }, // 0x000009D0
	 { -40, 0, -440, 0, -1707, 1024, 229, 229, 229, 255 }, // 0x000009E0
	 { -200, 0, -440, 0, 1024, 1024, 178, 178, 178, 255 }, // 0x000009F0
	 { -93, 80, -440, 0, -796, -341, 255, 255, 255, 255 }, // 0x00000A00
	 { -40, 120, -40, 0, 0, -5120, 153, 153, 153, 255 }, // 0x00000A10
	 { -20, 100, -40, 0, 1024, -4096, 153, 153, 153, 255 }, // 0x00000A20
	 { -20, 0, -40, 0, 1024, 1024, 153, 153, 153, 255 }, // 0x00000A30
	 { -40, 0, -40, 0, 0, 1024, 153, 153, 153, 255 }, // 0x00000A40
	 { 60, 120, -40, 0, 1024, -5120, 153, 153, 153, 255 }, // 0x00000A50
	 { 60, 0, -40, 0, 1024, 1024, 153, 153, 153, 255 }, // 0x00000A60
	 { 40, 0, -40, 0, 0, 1024, 153, 153, 153, 255 }, // 0x00000A70
	 { 40, 100, -40, 0, 0, -4096, 153, 153, 153, 255 }, // 0x00000A80
	 { -40, 120, -40, 0, 0, 1024, 153, 153, 153, 255 }, // 0x00000A90
	 { 60, 120, -40, 0, 0, -4166, 153, 153, 153, 255 }, // 0x00000AA0
	 { 40, 100, -40, 0, 1038, -3128, 153, 153, 153, 255 }, // 0x00000AB0
	 { -20, 100, -40, 0, 1038, -14, 153, 153, 153, 255 }, // 0x00000AC0
	 { -200, 240, -40, 0, 1024, -3072, 76, 76, 76, 255 }, // 0x00000AD0
	 { -40, 120, -40, 0, -1707, -1024, 153, 153, 153, 255 }, // 0x00000AE0
	 { -40, 0, -40, 0, -1707, 1024, 153, 153, 153, 255 }, // 0x00000AF0
};

Vtx_t _takaraya_room_1_vertices_00000B00[8] = 
{
	 { -200, 240, -40, 0, 1024, -3072, 76, 76, 76, 255 }, // 0x00000B00
	 { -40, 0, -40, 0, -1707, 1024, 153, 153, 153, 255 }, // 0x00000B10
	 { -200, 0, -40, 0, 1024, 1024, 114, 114, 114, 255 }, // 0x00000B20
	 { 240, 240, -40, 0, -6485, -3072, 76, 76, 76, 255 }, // 0x00000B30
	 { 60, 120, -40, 0, -3413, -1024, 153, 153, 153, 255 }, // 0x00000B40
	 { -40, 120, -40, 0, -1707, -1024, 153, 153, 153, 255 }, // 0x00000B50
	 { 240, 0, -40, 0, -6485, 1024, 114, 114, 114, 255 }, // 0x00000B60
	 { 60, 0, -40, 0, -3413, 1024, 153, 153, 153, 255 }, // 0x00000B70
};

Vtx_t _takaraya_room_1_vertices_00000B80[8] = 
{
	 { -200, 0, -460, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000B80
	 { 240, 0, -460, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000B90
	 { -200, 240, -460, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000BA0
	 { 240, 240, -460, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000BB0
	 { -200, 0, -20, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000BC0
	 { 240, 0, -20, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000BD0
	 { -200, 240, -20, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000BE0
	 { 240, 240, -20, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000BF0
};

Gfx _takaraya_room_1_dlist_00000C00[] =
{
	gsDPPipeSync(), // 0x00000C00
	gsSPGeometryMode(0xFF030000, 0x00000000), // 0x00000C08
	gsSPVertex(_takaraya_room_1_vertices_00000B80, 8, 0), // 0x00000C10
	gsSPCullDisplayList(0, 7), // 0x00000C18
	gsDPPipeSync(), // 0x00000C20
	gsSPGeometryMode(0xFF000000, 0x00030000), // 0x00000C28
	gsSPSetOtherMode(0xE3, 14, 2, 0x00000000), // 0x00000C30
	gsSPTexture(65535, 65535, 0, 0, 1), // 0x00000C38
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, _takaraya_scene_tex_000051B0), // 0x00000C40
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 0, 5, 0, 0, 5, 0), // 0x00000C48
	gsDPLoadSync(), // 0x00000C50
	gsDPLoadBlock(7, 0, 0, 1023, 256), // 0x00000C58
	gsDPPipeSync(), // 0x00000C60
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, 0, 5, 0, 0, 5, 0), // 0x00000C68
	gsDPSetTileSize(0, 0, 0, 124, 124), // 0x00000C70
	gsDPSetCombineLERP(TEXEL0, K5, SHADE, COMBINED_ALPHA, 0, 0, 0, 1, COMBINED, K5, PRIMITIVE, COMBINED_ALPHA, 0, 0, 0, COMBINED), // 0x00000C78
	gsSPSetOtherMode(0xE2, 3, 29, 0xC8112078), // 0x00000C80
	gsSPGeometryMode(0xFF0E0000, 0x00000000), // 0x00000C88
	gsSPGeometryMode(0xFF000000, 0x00010400), // 0x00000C90
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255), // 0x00000C98
	gsSPVertex(_takaraya_room_1_vertices_00000300, 32, 0), // 0x00000CA0
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), // 0x00000CA8
	gsSP2Triangles(4, 5, 6, 0, 4, 7, 5, 0), // 0x00000CB0
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0), // 0x00000CB8
	gsSP2Triangles(12, 13, 14, 0, 13, 15, 14, 0), // 0x00000CC0
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0), // 0x00000CC8
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0), // 0x00000CD0
	gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0), // 0x00000CD8
	gsSP2Triangles(28, 27, 26, 0, 28, 26, 29, 0), // 0x00000CE0
	gsSP2Triangles(29, 30, 31, 0, 29, 31, 28, 0), // 0x00000CE8
	gsSPVertex(_takaraya_room_1_vertices_00000500, 32, 0), // 0x00000CF0
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), // 0x00000CF8
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0), // 0x00000D00
	gsSP2Triangles(8, 7, 6, 0, 8, 6, 9, 0), // 0x00000D08
	gsSP2Triangles(9, 10, 11, 0, 9, 11, 8, 0), // 0x00000D10
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0), // 0x00000D18
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0), // 0x00000D20
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0), // 0x00000D28
	gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0), // 0x00000D30
	gsSP2Triangles(28, 29, 30, 0, 29, 31, 30, 0), // 0x00000D38
	gsSPVertex(_takaraya_room_1_vertices_00000700, 32, 0), // 0x00000D40
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), // 0x00000D48
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0), // 0x00000D50
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0), // 0x00000D58
	gsSP2Triangles(12, 13, 14, 0, 12, 15, 13, 0), // 0x00000D60
	gsSP2Triangles(16, 17, 18, 0, 16, 19, 20, 0), // 0x00000D68
	gsSP2Triangles(16, 20, 17, 0, 19, 21, 20, 0), // 0x00000D70
	gsSP2Triangles(22, 23, 24, 0, 23, 25, 24, 0), // 0x00000D78
	gsSP2Triangles(24, 25, 26, 0, 27, 24, 26, 0), // 0x00000D80
	gsSP2Triangles(22, 24, 27, 0, 28, 29, 30, 0), // 0x00000D88
	gsSP1Triangle(28, 30, 31, 0), // 0x00000D90
	gsSPVertex(_takaraya_room_1_vertices_00000900, 32, 0), // 0x00000D98
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), // 0x00000DA0
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0), // 0x00000DA8
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0), // 0x00000DB0
	gsSP2Triangles(0, 12, 1, 0, 13, 12, 0, 0), // 0x00000DB8
	gsSP2Triangles(12, 14, 1, 0, 13, 15, 16, 0), // 0x00000DC0
	gsSP2Triangles(15, 14, 16, 0, 17, 18, 19, 0), // 0x00000DC8
	gsSP2Triangles(17, 19, 20, 0, 21, 22, 23, 0), // 0x00000DD0
	gsSP2Triangles(21, 23, 24, 0, 25, 26, 27, 0), // 0x00000DD8
	gsSP2Triangles(25, 27, 28, 0, 29, 30, 31, 0), // 0x00000DE0
	gsSPVertex(_takaraya_room_1_vertices_00000B00, 8, 0), // 0x00000DE8
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 4, 0), // 0x00000DF0
	gsSP2Triangles(0, 4, 5, 0, 3, 6, 7, 0), // 0x00000DF8
	gsSP1Triangle(3, 7, 4, 0), // 0x00000E00
	gsSPEndDisplayList(), // 0x00000E08
};

Vtx_t _takaraya_room_1_vertices_00000E10[24] = 
{
	 { -66, 20, -301, 0, 6717, 2048, 255, 255, 255, 255 }, // 0x00000E10
	 { -100, 20, -261, 0, 4361, -10, 89, 89, 89, 255 }, // 0x00000E20
	 { -66, 20, -261, 0, 4361, 2048, 140, 140, 140, 255 }, // 0x00000E30
	 { -66, 20, -182, 0, -352, 2048, 255, 255, 255, 255 }, // 0x00000E40
	 { -100, 20, -301, 0, 6717, -10, 255, 255, 255, 255 }, // 0x00000E50
	 { -100, 20, -182, 0, -352, -10, 255, 255, 255, 255 }, // 0x00000E60
	 { -100, 20, -301, 0, -7061, 3, 255, 255, 255, 255 }, // 0x00000E70
	 { -134, 20, -301, 0, -7061, 2048, 255, 255, 255, 255 }, // 0x00000E80
	 { -100, 20, -261, 0, -4536, 3, 89, 89, 89, 255 }, // 0x00000E90
	 { -134, 20, -261, 0, -4536, 2048, 140, 140, 140, 255 }, // 0x00000EA0
	 { -134, 20, -182, 0, 512, 2048, 255, 255, 255, 255 }, // 0x00000EB0
	 { -100, 20, -182, 0, 512, 3, 255, 255, 255, 255 }, // 0x00000EC0
	 { 106, 20, -182, 0, 512, 2048, 255, 255, 255, 255 }, // 0x00000ED0
	 { 140, 20, -182, 0, 512, 3, 255, 255, 255, 255 }, // 0x00000EE0
	 { 140, 20, -261, 0, -4536, 3, 89, 89, 89, 255 }, // 0x00000EF0
	 { 106, 20, -261, 0, -4536, 2048, 140, 140, 140, 255 }, // 0x00000F00
	 { 106, 20, -301, 0, -7061, 2048, 255, 255, 255, 255 }, // 0x00000F10
	 { 140, 20, -301, 0, -7061, 3, 255, 255, 255, 255 }, // 0x00000F20
	 { 140, 20, -261, 0, 4361, -10, 89, 89, 89, 255 }, // 0x00000F30
	 { 140, 20, -182, 0, -352, -10, 255, 255, 255, 255 }, // 0x00000F40
	 { 174, 20, -182, 0, -352, 2048, 255, 255, 255, 255 }, // 0x00000F50
	 { 174, 20, -301, 0, 6717, 2048, 255, 255, 255, 255 }, // 0x00000F60
	 { 140, 20, -301, 0, 6717, -10, 255, 255, 255, 255 }, // 0x00000F70
	 { 174, 20, -261, 0, 4361, 2048, 140, 140, 140, 255 }, // 0x00000F80
};

Vtx_t _takaraya_room_1_vertices_00000F90[8] = 
{
	 { -134, 20, -301, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000F90
	 { 174, 20, -301, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000FA0
	 { -134, 20, -301, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000FB0
	 { 174, 20, -301, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000FC0
	 { -134, 20, -182, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000FD0
	 { 174, 20, -182, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000FE0
	 { -134, 20, -182, 0, 0, 0, 0, 0, 0, 0 }, // 0x00000FF0
	 { 174, 20, -182, 0, 0, 0, 0, 0, 0, 0 }, // 0x00001000
};

Gfx _takaraya_room_1_dlist_00001010[] =
{
	gsDPPipeSync(), // 0x00001010
	gsSPGeometryMode(0xFF030000, 0x00000000), // 0x00001018
	gsSPVertex(_takaraya_room_1_vertices_00000F90, 8, 0), // 0x00001020
	gsSPCullDisplayList(0, 7), // 0x00001028
	gsDPPipeSync(), // 0x00001030
	gsSPGeometryMode(0xFF000000, 0x00030000), // 0x00001038
	gsSPSetOtherMode(0xE3, 14, 2, 0x00000000), // 0x00001040
	gsSPTexture(65535, 65535, 0, 0, 1), // 0x00001048
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, _takaraya_room_1_tex_000017F8), // 0x00001050
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 1, 6, 0, 0, 4, 0), // 0x00001058
	gsDPLoadSync(), // 0x00001060
	gsDPLoadBlock(7, 0, 0, 1023, 512), // 0x00001068
	gsDPPipeSync(), // 0x00001070
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, 1, 6, 0, 0, 4, 0), // 0x00001078
	gsDPSetTileSize(0, 0, 0, 60, 252), // 0x00001080
	gsDPSetCombineLERP(TEXEL0, K5, SHADE, COMBINED_ALPHA, 0, 0, 0, 1, COMBINED, K5, PRIMITIVE, COMBINED_ALPHA, 0, 0, 0, COMBINED), // 0x00001088
	gsSPSetOtherMode(0xE2, 3, 29, 0xC8112078), // 0x00001090
	gsSPGeometryMode(0xFF0E0000, 0x00000000), // 0x00001098
	gsSPGeometryMode(0xFF000000, 0x00010400), // 0x000010A0
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255), // 0x000010A8
	gsSPVertex(_takaraya_room_1_vertices_00000E10, 24, 0), // 0x000010B0
	gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0), // 0x000010B8
	gsSP2Triangles(0, 4, 1, 0, 1, 5, 3, 0), // 0x000010C0
	gsSP2Triangles(6, 7, 8, 0, 7, 9, 8, 0), // 0x000010C8
	gsSP2Triangles(9, 10, 8, 0, 10, 11, 8, 0), // 0x000010D0
	gsSP2Triangles(12, 13, 14, 0, 15, 12, 14, 0), // 0x000010D8
	gsSP2Triangles(16, 15, 14, 0, 17, 16, 14, 0), // 0x000010E0
	gsSP2Triangles(18, 19, 20, 0, 21, 22, 18, 0), // 0x000010E8
	gsSP2Triangles(18, 20, 23, 0, 21, 18, 23, 0), // 0x000010F0
	gsSPEndDisplayList(), // 0x000010F8
};

Vtx_t _takaraya_room_1_vertices_00001100[77] = 
{
	 { 80, 20, -160, 0, 2048, 1024, 178, 178, 178, 255 }, // 0x00001100
	 { 200, 20, -160, 0, 5120, 1024, 178, 178, 178, 255 }, // 0x00001110
	 { 174, 20, -182, 0, 4442, 456, 191, 191, 191, 255 }, // 0x00001120
	 { 106, 20, -182, 0, 2726, 456, 191, 191, 191, 255 }, // 0x00001130
	 { 80, 20, -320, 0, 2048, -3072, 255, 255, 255, 255 }, // 0x00001140
	 { 106, 20, -301, 0, 2726, -2573, 255, 255, 255, 255 }, // 0x00001150
	 { 200, 20, -320, 0, 5120, -3072, 255, 255, 255, 255 }, // 0x00001160
	 { 174, 20, -301, 0, 4442, -2573, 255, 255, 255, 255 }, // 0x00001170
	 { -40, 20, -320, 0, -1024, -3072, 255, 255, 255, 255 }, // 0x00001180
	 { -66, 20, -301, 0, -1702, -2573, 255, 255, 255, 255 }, // 0x00001190
	 { -66, 20, -182, 0, -1702, 456, 191, 191, 191, 255 }, // 0x000011A0
	 { -40, 20, -160, 0, -1024, 1024, 178, 178, 178, 255 }, // 0x000011B0
	 { 80, 10, -320, 0, 2048, -3072, 255, 255, 255, 255 }, // 0x000011C0
	 { -40, 10, -320, 0, -1024, -3072, 255, 255, 255, 255 }, // 0x000011D0
	 { -40, 10, -300, 0, -1024, -2560, 153, 153, 153, 255 }, // 0x000011E0
	 { 80, 10, -300, 0, 2048, -2560, 153, 153, 153, 255 }, // 0x000011F0
	 { -40, 20, -300, 0, -1024, -2560, 255, 255, 255, 255 }, // 0x00001200
	 { -40, 20, -180, 0, -1024, 512, 191, 191, 191, 255 }, // 0x00001210
	 { 80, 20, -180, 0, 2048, 512, 191, 191, 191, 255 }, // 0x00001220
	 { 80, 20, -300, 0, 2048, -2560, 255, 255, 255, 255 }, // 0x00001230
	 { -40, 10, -180, 0, -1024, 512, 127, 127, 127, 255 }, // 0x00001240
	 { -40, 10, -160, 0, -1024, 1024, 255, 255, 255, 255 }, // 0x00001250
	 { 80, 10, -160, 0, 2048, 1024, 255, 255, 255, 255 }, // 0x00001260
	 { 80, 10, -180, 0, 2048, 512, 127, 127, 127, 255 }, // 0x00001270
	 { -160, 20, -320, 0, -4096, -3072, 255, 255, 255, 255 }, // 0x00001280
	 { -134, 20, -301, 0, -3418, -2573, 255, 255, 255, 255 }, // 0x00001290
	 { -160, 20, -160, 0, -4096, 1024, 178, 178, 178, 255 }, // 0x000012A0
	 { -134, 20, -182, 0, -3418, 456, 191, 191, 191, 255 }, // 0x000012B0
	 { -74, 0, -417, 0, 0, 1024, 178, 178, 178, 255 }, // 0x000012C0
	 { -74, 0, -402, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x000012D0
	 { -67, 20, -409, 0, 512, -434, 89, 89, 89, 255 }, // 0x000012E0
	 { -74, 0, -402, 0, 0, 1024, 178, 178, 178, 255 }, // 0x000012F0
	 { -59, 0, -402, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001300
	 { -67, 20, -409, 0, 512, -434, 89, 89, 89, 255 }, // 0x00001310
	 { -59, 0, -402, 0, 0, 1024, 178, 178, 178, 255 }, // 0x00001320
	 { -59, 0, -417, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001330
	 { -59, 0, -417, 0, 0, 1024, 178, 178, 178, 255 }, // 0x00001340
	 { -74, 0, -417, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001350
	 { -67, 0, -409, 0, 512, 4138, 89, 89, 89, 255 }, // 0x00001360
	 { -77, 80, -419, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001370
	 { -57, 80, -419, 0, 0, 1024, 255, 255, 255, 255 }, // 0x00001380
	 { -67, 0, -409, 0, 506, 4137, 89, 89, 89, 255 }, // 0x00001390
	 { -57, 80, -419, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x000013A0
	 { -57, 80, -399, 0, 0, 1022, 255, 255, 255, 255 }, // 0x000013B0
	 { -57, 80, -399, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x000013C0
	 { -77, 80, -399, 0, 0, 1024, 178, 178, 178, 255 }, // 0x000013D0
	 { -77, 80, -399, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x000013E0
	 { -77, 80, -419, 0, 0, 1024, 178, 178, 178, 255 }, // 0x000013F0
	 { -77, 80, -419, 0, 0, 0, 255, 255, 255, 255 }, // 0x00001400
	 { -67, 80, -409, 0, 512, 512, 43, 63, 38, 255 }, // 0x00001410
	 { -57, 80, -419, 0, 1024, 0, 255, 255, 255, 255 }, // 0x00001420
	 { -57, 80, -399, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001430
	 { -77, 80, -399, 0, 0, 1024, 255, 255, 255, 255 }, // 0x00001440
	 { 80, 0, -417, 0, 0, 1024, 178, 178, 178, 255 }, // 0x00001450
	 { 80, 0, -402, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001460
	 { 88, 20, -409, 0, 512, -434, 89, 89, 89, 255 }, // 0x00001470
	 { 80, 0, -402, 0, 0, 1024, 178, 178, 178, 255 }, // 0x00001480
	 { 95, 0, -402, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001490
	 { 95, 0, -402, 0, 0, 1024, 178, 178, 178, 255 }, // 0x000014A0
	 { 95, 0, -417, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x000014B0
	 { 95, 0, -417, 0, 0, 1024, 178, 178, 178, 255 }, // 0x000014C0
	 { 80, 0, -417, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x000014D0
	 { 88, 0, -409, 0, 512, 4138, 89, 89, 89, 255 }, // 0x000014E0
	 { 78, 80, -419, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x000014F0
	 { 98, 80, -419, 0, 0, 1024, 255, 255, 255, 255 }, // 0x00001500
	 { 88, 0, -409, 0, 506, 4137, 89, 89, 89, 255 }, // 0x00001510
	 { 98, 80, -419, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001520
	 { 98, 80, -399, 0, 0, 1022, 255, 255, 255, 255 }, // 0x00001530
	 { 98, 80, -399, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001540
	 { 78, 80, -399, 0, 0, 1024, 178, 178, 178, 255 }, // 0x00001550
	 { 78, 80, -399, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x00001560
	 { 78, 80, -419, 0, 0, 1024, 178, 178, 178, 255 }, // 0x00001570
	 { 78, 80, -419, 0, 0, 0, 255, 255, 255, 255 }, // 0x00001580
	 { 88, 80, -409, 0, 512, 512, 43, 63, 38, 255 }, // 0x00001590
	 { 98, 80, -419, 0, 1024, 0, 255, 255, 255, 255 }, // 0x000015A0
	 { 98, 80, -399, 0, 1024, 1024, 255, 255, 255, 255 }, // 0x000015B0
	 { 78, 80, -399, 0, 0, 1024, 255, 255, 255, 255 }, // 0x000015C0
};

Vtx_t _takaraya_room_1_vertices_000015D0[8] = 
{
	 { -160, 0, -419, 0, 0, 0, 0, 0, 0, 0 }, // 0x000015D0
	 { 200, 0, -419, 0, 0, 0, 0, 0, 0, 0 }, // 0x000015E0
	 { -160, 80, -419, 0, 0, 0, 0, 0, 0, 0 }, // 0x000015F0
	 { 200, 80, -419, 0, 0, 0, 0, 0, 0, 0 }, // 0x00001600
	 { -160, 0, -160, 0, 0, 0, 0, 0, 0, 0 }, // 0x00001610
	 { 200, 0, -160, 0, 0, 0, 0, 0, 0, 0 }, // 0x00001620
	 { -160, 80, -160, 0, 0, 0, 0, 0, 0, 0 }, // 0x00001630
	 { 200, 80, -160, 0, 0, 0, 0, 0, 0, 0 }, // 0x00001640
};

Gfx _takaraya_room_1_dlist_00001650[] =
{
	gsDPPipeSync(), // 0x00001650
	gsSPGeometryMode(0xFF030000, 0x00000000), // 0x00001658
	gsSPVertex(_takaraya_room_1_vertices_000015D0, 8, 0), // 0x00001660
	gsSPCullDisplayList(0, 7), // 0x00001668
	gsDPPipeSync(), // 0x00001670
	gsSPGeometryMode(0xFF000000, 0x00030000), // 0x00001678
	gsSPSetOtherMode(0xE3, 14, 2, 0x00000000), // 0x00001680
	gsSPTexture(65535, 65535, 0, 0, 1), // 0x00001688
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, _takaraya_scene_tex_000069B0), // 0x00001690
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 0, 5, 0, 0, 5, 0), // 0x00001698
	gsDPLoadSync(), // 0x000016A0
	gsDPLoadBlock(7, 0, 0, 1023, 256), // 0x000016A8
	gsDPPipeSync(), // 0x000016B0
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, 0, 5, 0, 0, 5, 0), // 0x000016B8
	gsDPSetTileSize(0, 0, 0, 124, 124), // 0x000016C0
	gsDPSetCombineLERP(TEXEL0, K5, SHADE, COMBINED_ALPHA, 0, 0, 0, 1, COMBINED, K5, PRIMITIVE, COMBINED_ALPHA, 0, 0, 0, COMBINED), // 0x000016C8
	gsSPSetOtherMode(0xE2, 3, 29, 0xC8112078), // 0x000016D0
	gsSPGeometryMode(0xFF0E0000, 0x00000000), // 0x000016D8
	gsSPGeometryMode(0xFF000000, 0x00010400), // 0x000016E0
	gsDPSetPrimColor(0, 0, 132, 224, 169, 255), // 0x000016E8
	gsSPVertex(_takaraya_room_1_vertices_00001100, 32, 0), // 0x000016F0
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0), // 0x000016F8
	gsSP2Triangles(4, 0, 3, 0, 4, 3, 5, 0), // 0x00001700
	gsSP2Triangles(6, 4, 5, 0, 6, 5, 7, 0), // 0x00001708
	gsSP2Triangles(6, 7, 2, 0, 6, 2, 1, 0), // 0x00001710
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0), // 0x00001718
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0), // 0x00001720
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0), // 0x00001728
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0), // 0x00001730
	gsSP2Triangles(8, 24, 25, 0, 8, 25, 9, 0), // 0x00001738
	gsSP2Triangles(24, 26, 27, 0, 24, 27, 25, 0), // 0x00001740
	gsSP2Triangles(26, 11, 10, 0, 26, 10, 27, 0), // 0x00001748
	gsSP1Triangle(28, 29, 30, 0), // 0x00001750
	gsSPVertex(_takaraya_room_1_vertices_000012F0, 32, 0), // 0x00001758
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 2, 0), // 0x00001760
	gsSP2Triangles(5, 6, 2, 0, 7, 8, 9, 0), // 0x00001768
	gsSP2Triangles(10, 11, 12, 0, 7, 13, 14, 0), // 0x00001770
	gsSP2Triangles(7, 15, 16, 0, 17, 18, 19, 0), // 0x00001778
	gsSP2Triangles(18, 20, 19, 0, 17, 21, 18, 0), // 0x00001780
	gsSP2Triangles(21, 20, 18, 0, 22, 23, 24, 0), // 0x00001788
	gsSP2Triangles(25, 26, 24, 0, 27, 28, 24, 0), // 0x00001790
	gsSP1Triangle(29, 30, 24, 0), // 0x00001798
	gsSPVertex(_takaraya_room_1_vertices_000014E0, 15, 0), // 0x000017A0
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0), // 0x000017A8
	gsSP2Triangles(0, 6, 7, 0, 0, 8, 9, 0), // 0x000017B0
	gsSP2Triangles(10, 11, 12, 0, 11, 13, 12, 0), // 0x000017B8
	gsSP2Triangles(10, 14, 11, 0, 14, 13, 11, 0), // 0x000017C0
	gsSPEndDisplayList(), // 0x000017C8
};

Gfx _takaraya_room_1_dlist_000017D0[] =
{
	gsSPDisplayList(_takaraya_room_1_dlist_00000220), // 0x000017D0
	gsSPDisplayList(_takaraya_room_1_dlist_00000C00), // 0x000017D8
	gsSPDisplayList(_takaraya_room_1_dlist_00001010), // 0x000017E0
	gsSPDisplayList(_takaraya_room_1_dlist_00001650), // 0x000017E8
	gsSPEndDisplayList(), // 0x000017F0
};

u64 _takaraya_room_1_tex_000017F8[] = 
{
	0xCEF5CEF5D6F5CEF5, 0xCEF5CEF7D737D737, 0xD737DF39D737D737, 0xDF37B66FBEB1BEB1,  // 0x000017F8 
	0xCEF5C6F3D737DF37, 0xD6F7D737D737D6F7, 0xCEF5CEF5CEF5CEF5, 0xD6F7C6F3D6F5CEF5,  // 0x00001818 
	0xC6B3C6B3D737D737, 0xCEF5CEF5CEF5D6F5, 0xCEF3BEB1CEF5DF37, 0xD737D737D737DF79,  // 0x00001838 
	0xD737D6F7C6F3CEF5, 0xCEF5CEF5C6B3C6B3, 0xC6B3BEB1C6B3CEF5, 0xCEF5B66FAE2DBEB1,  // 0x00001858 
	0xCEB5C6B3C6B3CEF5, 0xD737BEB1BEB1C6B3, 0xC6B3C6B3CEF3CEF5, 0xC6B3BEB1C6B3BEB1,  // 0x00001878 
	0xBE7195E7B66FC6B3, 0xC6B3CEF5C6F3D737, 0xD737CEF5C6B3C6B3, 0xC6B3AE6DC6F3C6F3,  // 0x00001898 
	0xD6F5CEF5C6B3C6F3, 0xC6B3BE71BEB1CEF5, 0xC6B1AE2DB62DAE2D, 0xC6F3B66FC6B3D737,  // 0x000018B8 
	0xC6F3C6B3AE6FCEF3, 0xCEF5CEF5BE71BE71, 0xB62DA62BC6B3C6F3, 0xC6B3CEF5CEF5CEF5,  // 0x000018D8 
	0xCEF5CEF5B66FAE2D, 0xBE6FB62FC6B3C6B3, 0xC6B3C6B3BEB1CEF5, 0xC6B3CEF5D6F5C6B3,  // 0x000018F8 
	0xCEF5C6B3AE2DB66F, 0xB66F9DEBB66FBE71, 0xC6B3CEF5CEF5B66F, 0xBE71BE6FBEB1B66F,  // 0x00001918 
	0xBE6FBE71BEB1D6F7, 0xC6B3BE71C6B3CEF3, 0xBEB1C6B3CEF5CEF5, 0xCEF5C6B3C6F3CEF5,  // 0x00001938 
	0xB66FC6B3CEF5C6B3, 0xB66FCEF5BEB1B66D, 0xB66FAE6DB66FB66F, 0xC6B3CEF5BE71AE2D,  // 0x00001958 
	0x9DE9A62BAE2D9DE9, 0xB66FBEB1BEB1CEF3, 0xCEF5C6F3C6B3CEF5, 0xC6B3BEB1C6B1B66F,  // 0x00001978 
	0xA66DBEB3B66FAE2D, 0xB66FB66FB66FA62D, 0x9E29B66FCEF5A62B, 0xA5EB9DE99DE9BEB1,  // 0x00001998 
	0xA62BB66FB66FC6B1, 0xBE6FB66FBE71B66F, 0xB66FC6B3B66FA62B, 0xC6B3B66FAE6DBEB1,  // 0x000019B8 
	0x9E2B9629BEB1BE71, 0xB66FAE6FB66FB66F, 0xA62D8DA58DA57561, 0x7DA5AE6DBEB1BEB1,  // 0x000019D8 
	0xAE6FA66DB66FB66F, 0x9E2BB6AFB66FBEB1, 0xB66FA62BB6B1BEB3, 0xC6F3CEF5B66F7DA3,  // 0x000019F8 
	0xBEB1B6AF9E29A66D, 0xAE6DAE6FA62B95E9, 0xC6F3AE6F9E2B9E2B, 0x9E2BB66FA66D95E9,  // 0x00001A18 
	0x7DA38DA58DE785A5, 0x8DA7A62BB66FB6B1, 0xB66FAE2DB66F95E9, 0x85A59DE97D618DA5,  // 0x00001A38 
	0x7D63651D7DA3A66D, 0xAE6D95A79DE99DE9, 0x8DA7B66FB6AF85E5, 0x7DA3AE6FAE6FB6B1,  // 0x00001A58 
	0xA66DAEAF8E299E2B, 0xB6B1C6F3A62B8563, 0x7561756385E785E7, 0x7DA59E2BAEAF8DE7,  // 0x00001A78 
	0x85E5A66DB66F7DA3, 0x6D6175619E2BAE6F, 0xB6AFAE6F8DE78DE9, 0xA66DAE6FA66D6D61,  // 0x00001A98 
	0x85E795E7AE2DB6AF, 0x8DE985E775A37DA5, 0xA66DB6B18E29962B, 0x9E2D6D5F6D5F6DA1,  // 0x00001AB8 
	0xB6B15D1D7DA36D5F, 0x5D1D6D5F7DA38DE7, 0x7561756175637DA5, 0x7DA5655F75A375A3,  // 0x00001AD8 
	0x5D1B54D9962975A1, 0x6D615D1D651D5519, 0x4C956D5F85A57561, 0x8DE99E6D7DA55D1D,  // 0x00001AF8 
	0x5D195CDB6D5F7DA1, 0x655D651D655B6D5F, 0x85E37DA175A17DA3, 0x755F651B651B655D,  // 0x00001B18 
	0x85E575A16D5D7DA1, 0x85E5655D6D5F75A1, 0x75A175A175A16D5F, 0x4C953C137DA3962B,  // 0x00001B38 
	0x6D5D5CD76D5D755D, 0x6D5D5CD95D19655B, 0x6D5D755F7DA17DA1, 0x6D5D7DE385E375A1,  // 0x00001B58 
	0x5D175D196D5D6559, 0x5D176D5B6D5B54D5, 0x5D17651B759D7DA1, 0x75A185E36D5B655B,  // 0x00001B78 
	0x7DA185E35CD76519, 0x55175D17759F655B, 0x755D755D5495655B, 0x759F759D5D176D5F,  // 0x00001B98 
	0x6D5D7D9F5D1564D5, 0x755B759D6D5B6517, 0x65195D1965196559, 0x759D65154C9154D7,  // 0x00001BB8 
	0x6D5964D56D175CD3, 0x6D597D9F755B7517, 0x759D6D5D5D176D5B, 0x6D597D9F6D9D75A1,  // 0x00001BD8 
	0x5CD55C9354515CD3, 0x65156D15651764D5, 0x64D35CD5755B8DE3, 0x7D9D755B755B6D19,  // 0x00001BF8 
	0x64D36D1564D36CD3, 0x6CD36CD364D35CD3, 0x64D36D1364D36D59, 0x6D195C91540F5CD1,  // 0x00001C18 
	0x6CD35C515C9164D3, 0x6CD3649364D364D3, 0x651364D365135451, 0x440F5CD55CD364D3,  // 0x00001C38 
	0x5C91540F6CD364D3, 0x64D35C9154515C93, 0x5C5164D364D54BCF, 0x54515D156D575CD5,  // 0x00001C58 
	0x6D1364D34C514C11, 0x5C936CD35C915C91, 0x64935CD3549164D3, 0x5CD35CD364D35C93,  // 0x00001C78 
	0x5C9354915C935CD3, 0x5C915CD35C935491, 0x5CD35CD34C115491, 0x4C515C91540F5411,  // 0x00001C98 
	0x5C9365134BCF5451, 0x64D364D364D35C91, 0x545164D35C935CD3, 0x64D35CD35C915C91,  // 0x00001CB8 
	0x64D364D354514C51, 0x4C934C515C9364D3, 0x5C914C1154915C51, 0x64D3545154915C91,  // 0x00001CD8 
	0x4C514C514C515451, 0x43CF438D4C115CD3, 0x54914C5144115C91, 0x64D354515CD55CD3,  // 0x00001CF8 
	0x4C1154114C515491, 0x54514BCF44514C51, 0x54515CD35CD35C93, 0x5C914C0F4C0F4BCF,  // 0x00001D18 
	0x43CF44114C115C51, 0x5411438F4BCF5451, 0x4C0F440F43CF438F, 0x3BCF438F5C935CD5,  // 0x00001D38 
	0x4451440F438D3B0D, 0x3B4D3BCF438F4BCF, 0x440F440F43CF3BCF, 0x440F43CF4BCF4C11,  // 0x00001D58 
	0x438D43CF3B8F4411, 0x445143CF440F43CF, 0x3B8D3B4D3B8D43CF, 0x440F440F440F3B8F,  // 0x00001D78 
	0x43CF441144114451, 0x4411434D440F438F, 0x3B0B3BCF3BCF440F, 0x440F43CF43CF3B0D,  // 0x00001D98 
	0x44113B8F3B8F438F, 0x438F3B8D440F440F, 0x441144514411440F, 0x43CF440F540F4BCF,  // 0x00001DB8 
	0x445144115C934C11, 0x4411440F4C115451, 0x4C0F43CF4C0F5C51, 0x64D35C934C5143CF,  // 0x00001DD8 
	0x43CF438F54114451, 0x4CD34CD34C91440F, 0x440F4C514BCF4BCF, 0x4C1154915CD34C51,  // 0x00001DF8 
	0x440F441154935451, 0x4C114C514C514451, 0x4C114C0F4C514C0F, 0x5C914C514C114BCF,  // 0x00001E18 
	0x440F4C514C915451, 0x5C514C914C915CD5, 0x5CD35C935451440F, 0x4BCF434D4C0F5C93,  // 0x00001E38 
	0x440F5C5154114C11, 0x5C935CD354515493, 0x5C9164D354D34451, 0x5451545154515491,  // 0x00001E58 
	0x43CF5C515C915493, 0x545154515C915C91, 0x4C1154515411438F, 0x4C93545154515491,  // 0x00001E78 
	0x110508C310C31105, 0x1105114510C310C3, 0x10C308C3088310C3, 0x1105110508C310C3,  // 0x00001E98 
	0x32CB3B0D328B32CB, 0x3B0D3ACB32CB434D, 0x3B0B32CB3B0B32CB, 0x328B3B0B3B0B328B,  // 0x00001EB8 
	0x3B0D3B4D3B0D434D, 0x4B4D3B0D330B434D, 0x434D3B0D434D3B0B, 0x3ACB430D434D434D,  // 0x00001ED8 
	0x1105194519451105, 0x1945194519051105, 0x1945194511031105, 0x19851945110510C3,  // 0x00001EF8 
	0x1987194519452209, 0x220921C719871945, 0x1945194519451985, 0x21C7218721872209,  // 0x00001F18 
	0x1945198721C721C7, 0x1985218719451945, 0x1987198519C72207, 0x21C7194519851987,  // 0x00001F38 
	0x21C721C721C721C7, 0x218721C719851145, 0x21C721C919471945, 0x2187198519451945,  // 0x00001F58 
	0x6D5D5CD76D1D7D9F, 0x5CD35CD354D5651B, 0x65195CD7651B6519, 0x6D5B651965176D19,  // 0x00001F78 
	0x5D195D197DA1759F, 0x55176D9F759F6D5D, 0x65595517655B759D, 0x759D6D5B5CD56D9D,  // 0x00001F98 
	0x655B6D5B6D5B759F, 0x7DA1759D759F6519, 0x655B655B759F7DA1, 0x6D5965176559759F,  // 0x00001FB8 
	0x75A165196D5D759F, 0x6D5D655B6D9D7D9F, 0x759F759F759F655B, 0x65597DA1759F54D9,  // 0x00001FD8 
};

static u8 unaccounted1FF8[] = 
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};


