#include "z_demo_kankyo.h"
#include "z64cutscene_commands.h"

// clang-format off
CutsceneData gChildWarpOutCS[] = {
    CS_BEGIN_CUTSCENE(5, 1167),
    CS_SCENE_TRANS_FX(0x0005, 36, 46),
    CS_SCENE_TRANS_FX(0x0001, 30, 35),
    CS_CAM_POS_PLAYER_LIST(0, 1138),
        CS_CAM_POS_PLAYER(CS_CMD_CONTINUE, 0x00, 0, 60.324859619140625f, 30, 63, 61, 0xA8A5),
        CS_CAM_POS_PLAYER(CS_CMD_CONTINUE, 0x00, 0, 60.324859619140625f, 30, 63, 61, 0xA3D9),
        CS_CAM_POS_PLAYER(CS_CMD_CONTINUE, 0x00, 0, 60.324859619140625f, 30, 63, 61, 0xF3A5),
        CS_CAM_POS_PLAYER(CS_CMD_CONTINUE, 0x00, 0, 60.32486343383789f, 29, 61, 59, 0xA5D5),
        CS_CAM_POS_PLAYER(CS_CMD_CONTINUE, 0x00, 0, 60.32486343383789f, 29, 61, 59, 0xA5E7),
        CS_CAM_POS_PLAYER(CS_CMD_CONTINUE, 0x00, 0, 60.32486343383789f, 29, 61, 59, 0xA5EC),
        CS_CAM_POS_PLAYER(CS_CMD_CONTINUE, 0x00, 0, 60.32486343383789f, 29, 61, 59, 0xBCA5),
        CS_CAM_POS_PLAYER(CS_CMD_STOP, 0x00, 0, 60.32486343383789f, 29, 61, 59, 0xEEC0),
    CS_CAM_FOCUS_POINT_PLAYER_LIST(0, 1167),
        CS_CAM_FOCUS_POINT_PLAYER(CS_CMD_CONTINUE, 0x00, 30, 60.324859619140625f, 54, 79, 31, 0xA8A5),
        CS_CAM_FOCUS_POINT_PLAYER(CS_CMD_CONTINUE, 0x00, 30, 60.324859619140625f, 54, 80, 31, 0xA3D9),
        CS_CAM_FOCUS_POINT_PLAYER(CS_CMD_CONTINUE, 0x00, 10, 60.324859619140625f, 54, 79, 31, 0xF3A5),
        CS_CAM_FOCUS_POINT_PLAYER(CS_CMD_CONTINUE, 0x00, 7, 60.32486343383789f, 15, 42, 30, 0xA5D5),
        CS_CAM_FOCUS_POINT_PLAYER(CS_CMD_CONTINUE, 0x00, 30, 60.32486343383789f, 15, 42, 30, 0xA5E7),
        CS_CAM_FOCUS_POINT_PLAYER(CS_CMD_CONTINUE, 0x00, 1000, 60.32486343383789f, 15, 42, 30, 0xA5EC),
        CS_CAM_FOCUS_POINT_PLAYER(CS_CMD_CONTINUE, 0x00, 30, 60.32486343383789f, 15, 42, 30, 0xBCA5),
        CS_CAM_FOCUS_POINT_PLAYER(CS_CMD_STOP, 0x00, 30, 60.32486343383789f, 15, 42, 30, 0xEEC0),
    CS_MISC_LIST(1),
        CS_MISC(0x000C, 95, 96, 0x0000, 0x00000000, 0x00000000, 0xFFFFFFEF, 0xFFFFFFCD, 0x00000000, 0xFFFFFFEF, 0xFFFFFFCD, 0x00000000, 0x00000000, 0x00000000),
    CS_END(),
};
// clang-format on
