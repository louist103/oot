/*
 * ROM spec file
 */

beginseg
    name "makerom"
    include "build/asm/rom_header.o"
    include "build/asm/ipl3.o"
    include "build/asm/entry.o"
endseg

beginseg
    name "boot"
    address 0x80000460
    include "build/src/boot/boot_main.o"
    include "build/src/boot/idle.o"
    include "build/src/boot/viconfig.o"
    include "build/src/boot/z_std_dma.o"
    include "build/src/boot/yaz0.o"
    include "build/src/boot/z_locale.o"
    include "build/src/boot/assert.o"
    include "build/src/boot/is_debug.o"
    include "build/src/libultra_boot_O2/osDriveRomInit.o"
    include "build/asm/mio0.o"
    include "build/src/boot/stackcheck.o"
    include "build/src/boot/logutils.o"
    include "build/src/libultra_boot_O2/sprintf.o"
    include "build/src/libultra_boot_O1/piacs.o"
    include "build/src/libultra_boot_O1/osSendMesg.o"
    include "build/src/libultra_boot_O1/osStopThread.o"
    include "build/src/libultra_boot_O1/osViExtendVStart.o"
    include "build/src/libultra_boot_O1/osViModePalLan1.o"
    include "build/src/libultra_boot_O1/osRecvMesg.o"
    include "build/src/libultra_boot_O1/initialize.o"
    include "build/src/libultra_boot_O1/ll.o"
    include "build/asm/exceptasm.o"
    include "build/src/libultra_boot_O1/__osDequeueThread.o"
    include "build/src/libultra_boot_O1/osDestroyThread.o"
    include "build/asm/bzero.o"
    include "build/src/libultra_boot_O1/osCreateThread.o"
    include "build/asm/__osSetSR.o"
    include "build/asm/__osGetSR.o"
    include "build/asm/osWritebackDCache.o"
    include "build/src/libultra_boot_O2/osViGetNextFramebuffer.o"
    include "build/src/libultra_boot_O2/pimgr.o"
    include "build/src/libultra_boot_O2/__osDevMgrMain.o"
    include "build/src/libultra_boot_O2/__osPiRawStartDma.o"
    include "build/src/libultra_boot_O1/osVirtualToPhysical.o"
    include "build/src/libultra_boot_O2/osViBlack.o"
    include "build/src/libultra_boot_O2/__osSiRawReadIo.o"
    include "build/src/libultra_boot_O1/osGetThreadId.o"
    include "build/asm/osSetIntMask.o"
    include "build/src/libultra_boot_O2/osViSetMode.o"
    include "build/asm/__osProbeTLB.o"
    include "build/src/libultra_boot_O1/osGetMemSize.o"
    include "build/src/libultra_boot_O1/osSetEventMesg.o"
    include "build/src/libultra_boot_O2/_Printf.o"
    include "build/asm/osUnmapTLBAll.o"
    include "build/src/libultra_boot_O2/osEPiStartDma.o"
    include "build/src/libultra_boot_O2/string.o"
    include "build/asm/osInvalICache.o"
    include "build/src/libultra_boot_O1/osCreateMesgQueue.o"
    include "build/asm/osInvalDCache.o"
    include "build/src/libultra_boot_O2/__osSiDeviceBusy.o"
    include "build/src/libultra_boot_O1/osJamMesg.o"
    include "build/src/libultra_boot_O1/osSetThreadPri.o"
    include "build/src/libultra_boot_O1/osGetThreadPri.o"
    include "build/src/libultra_boot_O2/__osEPiRawReadIo.o"
    include "build/src/libultra_boot_O2/osViSwapBuffer.o"
    include "build/src/libultra_boot_O2/__osEPiRawStartDma.o"
    include "build/asm/bcmp.o"
    include "build/src/libultra_boot_O1/osGetTime.o"
    include "build/src/libultra_boot_O1/timerintr.o"
    include "build/asm/osGetCount.o"
    include "build/src/libultra_boot_O1/__osSetGlobalIntMask.o"
    include "build/asm/__osSetCompare.o"
    include "build/asm/bcopy.o"
    include "build/src/libultra_boot_O1/__osResetGlobalIntMask.o"
    include "build/asm/__osDisableInt.o"
    include "build/asm/__osRestoreInt.o"
    include "build/src/libultra_boot_O2/osViModeNtscLan1.o"
    include "build/src/libultra_boot_O2/osViModeMpalLan1.o"
    include "build/src/libultra_boot_O2/__osViInit.o"
    include "build/src/libultra_boot_O2/__osViSwapContext.o"
    include "build/src/libultra_boot_O2/osPiGetCmdQueue.o"
    include "build/src/libultra_boot_O2/osEPiReadIo.o"
    include "build/src/libultra_boot_O2/osViSetSpecialFeatures.o"
    include "build/src/libultra_boot_O2/osCartRomInit.o"
    include "build/src/libultra_boot_O2/osViModeFpalLan1.o"
    include "build/asm/__osSetFpcCsr.o"
    include "build/asm/__osGetFpcCsr.o"
    include "build/src/libultra_boot_O2/osEPiWriteIo.o"
    include "build/asm/osMapTLBRdb.o"
    include "build/src/libultra_boot_O1/osYieldThread.o"
    include "build/asm/__osGetCause.o"
    include "build/src/libultra_boot_O2/__osEPiRawWriteIo.o"
    include "build/src/libultra_boot_O2/_Litob.o"
    include "build/src/libultra_boot_O2/ldiv.o"
    include "build/src/libultra_boot_O2/_Ldtob.o"
    include "build/src/boot/build.o"
    include "build/src/libultra_boot_O2/__osSiRawWriteIo.o"
    include "build/src/libultra_boot_O2/vimgr.o"
    include "build/src/libultra_boot_O2/__osViGetCurrentContext.o"
    include "build/src/libultra_boot_O1/osStartThread.o"
    include "build/src/libultra_boot_O2/osViSetYScale.o"
    include "build/src/libultra_boot_O2/osViSetXScale.o"
    include "build/src/libultra_boot_O1/__osSetHWIntrRoutine.o"
    include "build/src/libultra_boot_O1/__osGetHWIntrRoutine.o"
    include "build/asm/__osSetWatchLo.o"
    include "build/data/rsp_boot.text.o"
    // Required for GCC support, should any functions use gcc library functions.
    // Linked in if defined.
#ifdef COMPILER_GCC
    include "build/asm/llmuldiv_gcc.o"
    include "build/src/boot/missing_gcc_functions.o"
#endif
endseg

beginseg
    name "dmadata"
    include "build/asm/dmadata.o"
endseg

beginseg
    name "Audiobank"
    address 0x10 // fake RAM address to avoid map lookup inaccuracies
    include "build/baserom/Audiobank.o"
endseg

beginseg
    name "Audioseq"
    include "build/baserom/Audioseq.o"
endseg

beginseg
    name "Audiotable"
    include "build/baserom/Audiotable.o"
endseg

beginseg
    name "link_animetion"
    include "build/baserom/link_animetion.o"
endseg

beginseg
    name "icon_item_static"
    romalign 0x1000
    include "build/assets/textures/icon_item_static/icon_item_static.o"
    number 8
endseg

beginseg
    name "icon_item_24_static"
    romalign 0x1000
    include "build/assets/textures/icon_item_24_static/icon_item_24_static.o"
    number 9
endseg

beginseg
    name "icon_item_field_static"
    compress
    romalign 0x1000
    include "build/assets/textures/icon_item_field_static/icon_item_field_static.o"
    number 12
endseg

beginseg
    name "icon_item_dungeon_static"
    compress
    romalign 0x1000
    include "build/assets/textures/icon_item_dungeon_static/icon_item_dungeon_static.o"
    number 12
endseg

beginseg
    name "icon_item_gameover_static"
    compress
    romalign 0x1000
    include "build/assets/textures/icon_item_gameover_static/icon_item_gameover_static.o"
    number 12
endseg

beginseg
    name "icon_item_nes_static"
    compress
    romalign 0x1000
    include "build/assets/textures/icon_item_nes_static/icon_item_nes_static.o"
    number 13
endseg

beginseg
    name "icon_item_ger_static"
    compress
    romalign 0x1000
    include "build/assets/textures/icon_item_ger_static/icon_item_ger_static.o"
    number 13
endseg

beginseg
    name "icon_item_fra_static"
    compress
    romalign 0x1000
    include "build/assets/textures/icon_item_fra_static/icon_item_fra_static.o"
    number 13
endseg

beginseg
    name "item_name_static"
    romalign 0x1000
    include "build/assets/textures/item_name_static/item_name_static.o"
    number 10
endseg

beginseg
    name "map_name_static"
    romalign 0x1000
    include "build/assets/textures/map_name_static/map_name_static.o"
    number 11
endseg

beginseg
    name "do_action_static"
    romalign 0x1000
    include "build/assets/textures/do_action_static/do_action_static.o"
    number 7
endseg

beginseg
    name "message_static"
    romalign 0x1000
    include "build/assets/textures/message_static/message_static.o"
    number 7
endseg

beginseg
    name "message_texture_static"
    romalign 0x1000
    include "build/assets/textures/message_texture_static/message_texture_static.o"
    number 9
endseg

beginseg
    name "nes_font_static"
    romalign 0x1000
    include "build/assets/textures/nes_font_static/nes_font_static.o"
    number 10
endseg

beginseg
    name "nes_message_data_static"
    romalign 0x1000
    include "build/assets/text/nes_message_data_static.o"
    number 07
endseg

beginseg
    name "ger_message_data_static"
    romalign 0x1000
    include "build/assets/text/ger_message_data_static.o"
    number 07
endseg

beginseg
    name "fra_message_data_static"
    romalign 0x1000
    include "build/assets/text/fra_message_data_static.o"
    number 07
endseg

beginseg
    name "staff_message_data_static"
    romalign 0x1000
    include "build/assets/text/staff_message_data_static.o"
    number 07
endseg

beginseg
    name "map_grand_static"
    romalign 0x1000
    include "build/assets/textures/map_grand_static/map_grand_static.o"
    number 11
endseg

beginseg
    name "map_48x85_static"
    romalign 0x1000
    include "build/assets/textures/map_48x85_static/map_48x85_static.o"
    number 11
endseg

beginseg
    name "map_i_static"
    romalign 0x1000
    include "build/assets/textures/map_i_static/map_i_static.o"
    number 11
endseg

beginseg
    name "code"
    compress
    after "dmadata"
    include "build/src/code/z_en_a_keep.o"
    include "build/src/code/z_en_item00.o"
    include "build/src/code/z_eff_blure.o"
    include "build/src/code/z_eff_shield_particle.o"
    include "build/src/code/z_eff_spark.o"
    include "build/src/code/z_eff_ss_dead.o"
    include "build/src/code/z_effect.o"
    include "build/src/code/z_effect_soft_sprite.o"
    include "build/src/code/z_effect_soft_sprite_old_init.o"
    include "build/src/code/z_effect_soft_sprite_dlftbls.o"
    include "build/src/code/flg_set.o"
    include "build/src/code/z_DLF.o"
    include "build/src/code/z_actor.o"
    include "build/src/code/z_actor_dlftbls.o"
    include "build/src/code/z_bgcheck.o"
    include "build/src/code/code_800430A0.o"
    include "build/src/code/code_80043480.o"
    include "build/src/code/z_camera.o"
    include "build/src/code/z_collision_btltbls.o"
    include "build/src/code/z_collision_check.o"
    include "build/src/code/z_common_data.o"
    include "build/src/code/z_debug.o"
    include "build/src/code/z_debug_display.o"
    include "build/src/code/z_demo.o"
    include "build/src/code/code_80069420.o"
    include "build/src/code/z_draw.o"
    include "build/src/code/code_8006BA00.o"
    include "build/src/code/z_elf_message.o"
    include "build/src/code/z_face_reaction.o"
    include "build/src/code/code_8006C3A0.o"
    include "build/src/code/code_8006C510.o"
    include "build/src/code/z_fcurve_data_skelanime.o"
    include "build/src/code/z_game_dlftbls.o"
    include "build/src/code/z_horse.o"
    include "build/src/code/z_jpeg.o"
    include "build/src/code/z_kaleido_setup.o"
    include "build/src/code/z_kanfont.o"
    include "build/src/code/z_kankyo.o"
    include "build/src/code/z_lib.o"
    include "build/src/code/z_lifemeter.o"
    include "build/src/code/z_lights.o"
    include "build/src/code/z_malloc.o"
    include "build/src/code/z_map_mark.o"
    include "build/src/code/z_moji.o"
    include "build/src/code/z_prenmi_buff.o"
    include "build/src/code/z_msgevent.o"
    include "build/src/code/z_olib.o"
    include "build/src/code/z_onepointdemo.o"
    include "build/src/code/z_map_exp.o"
    include "build/src/code/z_map_data.o"
    include "build/src/code/z_parameter.o"
    include "build/src/code/z_path.o"
    include "build/src/code/z_frame_advance.o"
    include "build/src/code/z_player_lib.o"
    include "build/src/code/z_prenmi.o"
    include "build/src/code/z_quake.o"
    include "build/src/code/z_rcp.o"
    include "build/src/code/z_room.o"
    include "build/src/code/z_sample.o"
    include "build/src/code/code_80097A00.o"
    include "build/src/code/z_scene.o"
    include "build/src/code/z_scene_table.o"
    include "build/src/code/z_skelanime.o"
    include "build/src/code/z_skin.o"
    include "build/src/code/z_skin_awb.o"
    include "build/src/code/z_skin_matrix.o"
    include "build/src/code/z_sram.o"
    include "build/src/code/z_ss_sram.o"
    include "build/src/code/code_800A9F30.o"
    include "build/data/z_text.data.o"
    include "build/src/code/z_view.o"
    include "build/src/code/z_vimode.o"
    include "build/src/code/code_800ACE70.o"
    include "build/src/code/z_vismono.o"
    include "build/src/code/code_800AD920.o"
    include "build/src/code/z_vr_box.o"
    include "build/src/code/z_vr_box_draw.o"
    include "build/src/code/z_player_call.o"
    include "build/src/code/z_fbdemo.o"
    include "build/src/code/z_fbdemo_triforce.o"
    include "build/src/code/z_fbdemo_wipe1.o"
    include "build/src/code/z_fbdemo_circle.o"
    include "build/src/code/z_fbdemo_fade.o"
    include "build/src/code/shrink_window.o"
    //include "build/src/code/db_camera.o"
    include "build/src/code/code_800BB0A0.o"
    //include "build/src/code/mempak.o"
    include "build/src/code/z_kaleido_manager.o"
    include "build/src/code/z_kaleido_scope_call.o"
    include "build/src/code/z_play.o"
    include "build/src/code/PreRender.o"
    include "build/src/code/TwoHeadArena.o"
    include "build/src/code/code_800C3C20.o"
    include "build/src/code/audioMgr.o"
    include "build/src/code/title_setup.o"
    include "build/src/code/game.o"
    include "build/src/code/gamealloc.o"
    include "build/src/code/graph.o"
    include "build/src/code/listalloc.o"
    include "build/src/code/main.o"
    include "build/src/code/padmgr.o"
    include "build/src/code/sched.o"
    include "build/src/code/speed_meter.o"
    include "build/src/code/sys_cfb.o"
    include "build/src/code/sys_math.o"
    include "build/src/code/sys_math3d.o"
    include "build/src/code/sys_math_atan.o"
    include "build/src/code/sys_matrix.o"
    include "build/src/code/sys_ucode.o"
    include "build/src/code/code_800D2E30.o"
    include "build/data/code_800D2E30.data.o"
    include "build/src/code/code_800D31A0.o"
    include "build/src/code/irqmgr.o"
    include "build/src/code/debug_malloc.o"
    include "build/src/code/fault.o"
    include "build/data/fault.bss.o"
    include "build/src/code/fault_drawer.o"
    include "build/asm/code_800D71F0.o"
    include "build/src/code/ucode_disas.o"
    include "build/src/code/audio_synthesis.o"
    include "build/data/audio_synthesis.data.o"
    include "build/src/code/audio_heap.o"
    include "build/src/code/audio_load.o"
    include "build/src/code/code_800E4FE0.o"
    include "build/src/libultra_code_O2/code_800E6840.o"
    include "build/src/libultra_code_O2/osAiSetNextBuffer.o"
    include "build/src/code/audio_playback.o"
    include "build/src/code/audio_effects.o"
    include "build/data/audio_effects.data.o"
    include "build/src/code/audio_seqplayer.o"
    include "build/src/code/code_800EC960.o"
    include "build/src/code/audio_sound_params.o"
    include "build/src/code/code_800F7260.o"
    include "build/data/code_800F7260.bss.o"
    include "build/src/code/code_800F9280.o"
    include "build/data/code_800F9280.data.o"
    include "build/src/code/audio_rodata.o"
    include "build/src/code/logseverity.o"
    include "build/src/code/gfxprint.o"
    include "build/src/code/code_800FBCE0.o"
    include "build/src/code/loadfragment2.o"
    include "build/src/code/mtxuty-cvt.o"
    include "build/src/code/relocation.o"
    include "build/src/code/code_800FC620.o"
    include "build/src/code/padutils.o"
    include "build/src/code/padsetup.o"
    include "build/src/code/code_800FCE80.o"
    include "build/asm/fp.o"
    include "build/src/code/system_malloc.o"
    include "build/src/code/code_800FD970.o"
    include "build/src/code/__osMalloc.o"
    include "build/src/code/printutils.o"
    include "build/src/code/sleep.o"
    include "build/src/code/jpegutils.o"
    include "build/src/code/jpegdecoder.o"
    include "build/src/libultra_code_O2/osPfsFreeBlocks.o"
    include "build/asm/guScale.o"
    include "build/src/libultra_code_O2/sinf.o"
    include "build/src/libultra_code_O2/sins.o"
    include "build/src/libultra_code_O2/sptask.o"
    include "build/src/libultra_code_O2/osRumblePak.o"
    include "build/src/libultra_code_O2/__osSiCreateAccessQueue.o"
    include "build/src/libultra_code_O2/osContInit.o"
    include "build/src/libultra_code_O2/osContStartReadData.o"
    include "build/src/libultra_code_O2/guPerspectiveF.o"
    include "build/src/libultra_code_O2/__osSpRawStartDma.o"
    include "build/src/libultra_code_O2/__osSiRawStartDma.o"
    include "build/src/libultra_code_O2/osSpTaskYield.o"
    include "build/src/libultra_code_O2/pfsreadwritefile.o"
    include "build/src/libultra_code_O2/__osPfsGetStatus.o"
    include "build/asm/guMtxIdentF.o"
    include "build/src/libultra_code_O2/guLookAt.o"
    include "build/src/libultra_code_O2/pfsallocatefile.o"
    include "build/src/libultra_code_O1/osStopTimer.o"
    include "build/src/libultra_code_O2/contpfs.o"
    include "build/asm/guMtxL2F.o"
    include "build/src/libultra_code_O2/osPfsFindFile.o"
    include "build/src/libultra_code_O2/sqrtf.o"
    include "build/src/libultra_code_O2/osAfterPreNMI.o"
    include "build/src/libultra_code_O2/contquery.o"
    include "build/src/libultra_code_O2/guLookAtHilite.o"
    include "build/src/libultra_code_O2/sp.o"
    include "build/asm/guMtxIdent.o"
    include "build/src/libultra_code_O2/guPosition.o"
    include "build/src/libultra_code_O2/osSpTaskYielded.o"
    include "build/src/libultra_code_O2/rotate.o"
    include "build/src/libultra_code_O2/osAiSetFrequency.o"
    include "build/src/libultra_code_O2/__osGetActiveQueue.o"
    include "build/asm/guNormalize.o"
    include "build/src/libultra_code_O2/osDpGetStatus.o"
    include "build/src/libultra_code_O2/osDpSetStatus.o"
    include "build/src/libultra_code_O2/osPfsDeleteFile.o"
    include "build/src/libultra_code_O2/ortho.o"
    include "build/src/libultra_code_O2/cosf.o"
    include "build/data/__libm_qnan_f.rodata.o"
    include "build/src/libultra_code_O2/coss.o"
    include "build/src/libultra_code_O2/osViSetEvent.o"
    include "build/src/libultra_code_O2/osPfsIsPlug.o"
    include "build/src/libultra_code_O2/guS2DInitBg.o"
    include "build/src/libultra_code_O2/__osPfsSelectBank.o"
    include "build/src/libultra_code_O2/osContSetCh.o"
    include "build/src/libultra_code_O2/osPfsFileState.o"
    include "build/src/libultra_code_O2/pfsinitpak.o"
    include "build/src/libultra_code_O2/pfschecker.o"
    include "build/src/libultra_code_O2/osAiGetLength.o"
    include "build/asm/guTranslate.o"
    include "build/src/libultra_code_O2/__osContRamWrite.o"
    include "build/src/libultra_code_O2/__osContRamRead.o"
    include "build/src/libultra_code_O2/__osContAddressCrc.o"
    include "build/src/libultra_code_O1/osSetTimer.o"
    include "build/src/libultra_code_O2/__osSpGetStatus.o"
    include "build/src/libultra_code_O2/__osSpSetStatus.o"
    include "build/asm/osWritebackDCacheAll.o"
    include "build/src/libultra_code_O2/__osGetCurrFaultedThread.o"
    include "build/asm/guMtxF2L.o"
    include "build/src/libultra_code_O1/llcvt.o"
    include "build/src/libultra_code_O2/osViGetCurrentFramebuffer.o"
    include "build/src/libultra_code_O2/__osSpSetPc.o"
    include "build/src/libultra_code_O2_g3/sqrt.o"
    include "build/src/libultra_code_O2_g3/absf.o"
    include "build/src/code/code_801067F0.o"
    include "build/src/code/code_80106860.o"
    include "build/src/code/code_801068B0.o"
    include_readonly "build/src/code/z_message_PAL.o"
    include "build/src/code/z_game_over.o"
    include "build/src/code/z_construct.o"
    include "build/data/rsp.text.o"
    include "build/data/rsp.rodata.o"


    include "build/src/overlays/gamestates/ovl_title/z_title.o"
    include "build/src/overlays/gamestates/ovl_select/z_select.o"
    include "build/src/overlays/gamestates/ovl_opening/z_opening.o"
    include "build/src/overlays/gamestates/ovl_file_choose/z_file_nameset_PAL.o"
    include "build/data/overlays/gamestates/z_file_nameset_PAL.data.o"
    include "build/data/overlays/gamestates/z_file_nameset_PAL.rodata.o"
    include "build/data/overlays/gamestates/z_file_nameset_PAL.bss.o"
    include "build/src/overlays/gamestates/ovl_file_choose/z_file_choose.o"
    include "build/data/overlays/gamestates/z_file_choose.rodata.o"

        include "build/src/overlays/actors/ovl_En_Test/z_en_test.o"
    include "build/src/overlays/actors/ovl_Arms_Hook/z_arms_hook.o"
    include "build/src/overlays/actors/ovl_Arrow_Fire/z_arrow_fire.o"
    include "build/src/overlays/actors/ovl_Arrow_Ice/z_arrow_ice.o"
    include "build/src/overlays/actors/ovl_Arrow_Light/z_arrow_light.o"
    include "build/src/overlays/actors/ovl_Bg_Bdan_Objects/z_bg_bdan_objects.o"
    include "build/src/overlays/actors/ovl_Bg_Bdan_Switch/z_bg_bdan_switch.o"
    include "build/src/overlays/actors/ovl_Bg_Bom_Guard/z_bg_bom_guard.o"
    include "build/src/overlays/actors/ovl_Bg_Bombwall/z_bg_bombwall.o"
    include "build/src/overlays/actors/ovl_Bg_Bowl_Wall/z_bg_bowl_wall.o"
    include "build/src/overlays/actors/ovl_Bg_Breakwall/z_bg_breakwall.o"
    include "build/src/overlays/actors/ovl_Bg_Ddan_Jd/z_bg_ddan_jd.o"
    include "build/src/overlays/actors/ovl_Bg_Ddan_Kd/z_bg_ddan_kd.o"
    include "build/src/overlays/actors/ovl_Bg_Dodoago/z_bg_dodoago.o"
    include "build/src/overlays/actors/ovl_Bg_Dy_Yoseizo/z_bg_dy_yoseizo.o"
    include "build/src/overlays/actors/ovl_Bg_Ganon_Otyuka/z_bg_ganon_otyuka.o"
    include "build/src/overlays/actors/ovl_Bg_Gate_Shutter/z_bg_gate_shutter.o"
    include "build/src/overlays/actors/ovl_Bg_Gjyo_Bridge/z_bg_gjyo_bridge.o"
    include "build/src/overlays/actors/ovl_Bg_Gnd_Darkmeiro/z_bg_gnd_darkmeiro.o"
    include "build/src/overlays/actors/ovl_Bg_Gnd_Firemeiro/z_bg_gnd_firemeiro.o"
    include "build/src/overlays/actors/ovl_Bg_Gnd_Iceblock/z_bg_gnd_iceblock.o"
    include "build/src/overlays/actors/ovl_Bg_Gnd_Nisekabe/z_bg_gnd_nisekabe.o"
    include "build/src/overlays/actors/ovl_Bg_Gnd_Soulmeiro/z_bg_gnd_soulmeiro.o"
    include "build/src/overlays/actors/ovl_Bg_Haka/z_bg_haka.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Gate/z_bg_haka_gate.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Huta/z_bg_haka_huta.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Megane/z_bg_haka_megane.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_MeganeBG/z_bg_haka_meganebg.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Sgami/z_bg_haka_sgami.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Ship/z_bg_haka_ship.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Trap/z_bg_haka_trap.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Tubo/z_bg_haka_tubo.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Water/z_bg_haka_water.o"
    include "build/src/overlays/actors/ovl_Bg_Haka_Zou/z_bg_haka_zou.o"
    include "build/src/overlays/actors/ovl_Bg_Heavy_Block/z_bg_heavy_block.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Curtain/z_bg_hidan_curtain.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Dalm/z_bg_hidan_dalm.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Firewall/z_bg_hidan_firewall.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Fslift/z_bg_hidan_fslift.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Fwbig/z_bg_hidan_fwbig.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Hamstep/z_bg_hidan_hamstep.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Hrock/z_bg_hidan_hrock.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Kousi/z_bg_hidan_kousi.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Kowarerukabe/z_bg_hidan_kowarerukabe.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Rock/z_bg_hidan_rock.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Rsekizou/z_bg_hidan_rsekizou.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Sekizou/z_bg_hidan_sekizou.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Sima/z_bg_hidan_sima.o"
    include "build/src/overlays/actors/ovl_Bg_Hidan_Syoku/z_bg_hidan_syoku.o"
    include "build/src/overlays/actors/ovl_Bg_Ice_Objects/z_bg_ice_objects.o"
    include "build/src/overlays/actors/ovl_Bg_Ice_Shelter/z_bg_ice_shelter.o"
    include "build/src/overlays/actors/ovl_Bg_Ice_Shutter/z_bg_ice_shutter.o"
    include "build/src/overlays/actors/ovl_Bg_Ice_Turara/z_bg_ice_turara.o"
    include "build/src/overlays/actors/ovl_Bg_Ingate/z_bg_ingate.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_1flift/z_bg_jya_1flift.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Amishutter/z_bg_jya_amishutter.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Bigmirror/z_bg_jya_bigmirror.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Block/z_bg_jya_block.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Bombchuiwa/z_bg_jya_bombchuiwa.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Bombiwa/z_bg_jya_bombiwa.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Cobra/z_bg_jya_cobra.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Goroiwa/z_bg_jya_goroiwa.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Haheniron/z_bg_jya_haheniron.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Ironobj/z_bg_jya_ironobj.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Kanaami/z_bg_jya_kanaami.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Lift/z_bg_jya_lift.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Megami/z_bg_jya_megami.o"
    include "build/src/overlays/actors/ovl_Bg_Jya_Zurerukabe/z_bg_jya_zurerukabe.o"
    include "build/src/overlays/actors/ovl_Bg_Menkuri_Eye/z_bg_menkuri_eye.o"
    include "build/src/overlays/actors/ovl_Bg_Menkuri_Kaiten/z_bg_menkuri_kaiten.o"
    include "build/src/overlays/actors/ovl_Bg_Menkuri_Nisekabe/z_bg_menkuri_nisekabe.o"
    include "build/src/overlays/actors/ovl_Bg_Mizu_Bwall/z_bg_mizu_bwall.o"
    include "build/src/overlays/actors/ovl_Bg_Mizu_Movebg/z_bg_mizu_movebg.o"
    include "build/src/overlays/actors/ovl_Bg_Mizu_Shutter/z_bg_mizu_shutter.o"
    include "build/src/overlays/actors/ovl_Bg_Mizu_Uzu/z_bg_mizu_uzu.o"
    include "build/src/overlays/actors/ovl_Bg_Mizu_Water/z_bg_mizu_water.o"
    include "build/src/overlays/actors/ovl_Bg_Mjin/z_bg_mjin.o"
    include "build/src/overlays/actors/ovl_Bg_Mori_Bigst/z_bg_mori_bigst.o"
    include "build/src/overlays/actors/ovl_Bg_Mori_Elevator/z_bg_mori_elevator.o"
    include "build/src/overlays/actors/ovl_Bg_Mori_Hashigo/z_bg_mori_hashigo.o"
    include "build/src/overlays/actors/ovl_Bg_Mori_Hashira4/z_bg_mori_hashira4.o"
    include "build/src/overlays/actors/ovl_Bg_Mori_Hineri/z_bg_mori_hineri.o"
    include "build/src/overlays/actors/ovl_Bg_Mori_Idomizu/z_bg_mori_idomizu.o"
    include "build/src/overlays/actors/ovl_Bg_Mori_Kaitenkabe/z_bg_mori_kaitenkabe.o"
    include "build/src/overlays/actors/ovl_Bg_Mori_Rakkatenjo/z_bg_mori_rakkatenjo.o"
    include "build/src/overlays/actors/ovl_Bg_Po_Event/z_bg_po_event.o"
    include "build/src/overlays/actors/ovl_Bg_Po_Syokudai/z_bg_po_syokudai.o"
    include "build/src/overlays/actors/ovl_Bg_Pushbox/z_bg_pushbox.o"
    include "build/src/overlays/actors/ovl_Bg_Relay_Objects/z_bg_relay_objects.o"
    include "build/src/overlays/actors/ovl_Bg_Spot00_Break/z_bg_spot00_break.o"
    include "build/src/overlays/actors/ovl_Bg_Spot00_Hanebasi/z_bg_spot00_hanebasi.o"
    include "build/src/overlays/actors/ovl_Bg_Spot01_Fusya/z_bg_spot01_fusya.o"
    include "build/src/overlays/actors/ovl_Bg_Spot01_Idohashira/z_bg_spot01_idohashira.o"
    include "build/src/overlays/actors/ovl_Bg_Spot01_Idomizu/z_bg_spot01_idomizu.o"
    include "build/src/overlays/actors/ovl_Bg_Spot01_Idosoko/z_bg_spot01_idosoko.o"
    include "build/src/overlays/actors/ovl_Bg_Spot01_Objects2/z_bg_spot01_objects2.o"
    include "build/src/overlays/actors/ovl_Bg_Spot02_Objects/z_bg_spot02_objects.o"
    include "build/src/overlays/actors/ovl_Bg_Spot03_Taki/z_bg_spot03_taki.o"
    include "build/src/overlays/actors/ovl_Bg_Spot05_Soko/z_bg_spot05_soko.o"
    include "build/src/overlays/actors/ovl_Bg_Spot06_Objects/z_bg_spot06_objects.o"
    include "build/src/overlays/actors/ovl_Bg_Spot07_Taki/z_bg_spot07_taki.o"
    include "build/src/overlays/actors/ovl_Bg_Spot08_Bakudankabe/z_bg_spot08_bakudankabe.o"
    include "build/src/overlays/actors/ovl_Bg_Spot08_Iceblock/z_bg_spot08_iceblock.o"
    include "build/src/overlays/actors/ovl_Bg_Spot09_Obj/z_bg_spot09_obj.o"
    include "build/src/overlays/actors/ovl_Bg_Spot11_Bakudankabe/z_bg_spot11_bakudankabe.o"
    include "build/src/overlays/actors/ovl_Bg_Spot11_Oasis/z_bg_spot11_oasis.o"
    include "build/src/overlays/actors/ovl_Bg_Spot12_Gate/z_bg_spot12_gate.o"
    include "build/src/overlays/actors/ovl_Bg_Spot12_Saku/z_bg_spot12_saku.o"
    include "build/src/overlays/actors/ovl_Bg_Spot15_Rrbox/z_bg_spot15_rrbox.o"
    include "build/src/overlays/actors/ovl_Bg_Spot15_Saku/z_bg_spot15_saku.o"
    include "build/src/overlays/actors/ovl_Bg_Spot16_Bombstone/z_bg_spot16_bombstone.o"
    include "build/src/overlays/actors/ovl_Bg_Spot16_Doughnut/z_bg_spot16_doughnut.o"
    include "build/src/overlays/actors/ovl_Bg_Spot17_Bakudankabe/z_bg_spot17_bakudankabe.o"
    include "build/src/overlays/actors/ovl_Bg_Spot17_Funen/z_bg_spot17_funen.o"
    include "build/src/overlays/actors/ovl_Bg_Spot18_Basket/z_bg_spot18_basket.o"
    include "build/src/overlays/actors/ovl_Bg_Spot18_Futa/z_bg_spot18_futa.o"
    include "build/src/overlays/actors/ovl_Bg_Spot18_Obj/z_bg_spot18_obj.o"
    include "build/src/overlays/actors/ovl_Bg_Spot18_Shutter/z_bg_spot18_shutter.o"
    include "build/src/overlays/actors/ovl_Bg_Sst_Floor/z_bg_sst_floor.o"
    include "build/src/overlays/actors/ovl_Bg_Toki_Hikari/z_bg_toki_hikari.o"
    include "build/src/overlays/actors/ovl_Bg_Toki_Swd/z_bg_toki_swd_cutscene_data_1.o"
    include "build/src/overlays/actors/ovl_Bg_Toki_Swd/z_bg_toki_swd_cutscene_data_2.o"
    include "build/src/overlays/actors/ovl_Bg_Toki_Swd/z_bg_toki_swd_cutscene_data_3.o"
    include "build/src/overlays/actors/ovl_Bg_Toki_Swd/z_bg_toki_swd.o"
    include "build/src/overlays/actors/ovl_Bg_Treemouth/z_bg_treemouth_cutscene_data.o"
    include "build/src/overlays/actors/ovl_Bg_Treemouth/z_bg_treemouth.o"
    include "build/src/overlays/actors/ovl_Bg_Umajump/z_bg_umajump.o"
    include "build/src/overlays/actors/ovl_Bg_Vb_Sima/z_bg_vb_sima.o"
    include "build/src/overlays/actors/ovl_Bg_Ydan_Hasi/z_bg_ydan_hasi.o"
    include "build/src/overlays/actors/ovl_Bg_Ydan_Maruta/z_bg_ydan_maruta.o"
    include "build/src/overlays/actors/ovl_Bg_Ydan_Sp/z_bg_ydan_sp.o"
    include "build/src/overlays/actors/ovl_Bg_Zg/z_bg_zg.o"
    include "build/src/overlays/actors/ovl_Boss_Dodongo/z_boss_dodongo.o"
    include "build/src/overlays/actors/ovl_Boss_Fd/z_boss_fd.o"
    include "build/src/overlays/actors/ovl_Boss_Fd2/z_boss_fd2.o"
    include "build/src/overlays/actors/ovl_Boss_Ganon/z_boss_ganon.o"
    include "build/src/overlays/actors/ovl_Boss_Ganon2/z_boss_ganon2.o"
    include "build/src/overlays/actors/ovl_Boss_Ganondrof/z_boss_ganondrof.o"
        include "build/data/overlays/actors/z_boss_ganon.data.o"
    include "build/data/overlays/actors/z_boss_ganon.bss.o"
    include "build/src/overlays/actors/ovl_Boss_Goma/z_boss_goma.o"
    include "build/src/overlays/actors/ovl_Boss_Mo/z_boss_mo.o"
    include "build/src/overlays/actors/ovl_Boss_Sst/z_boss_sst.o"
    include "build/src/overlays/actors/ovl_Boss_Tw/z_boss_tw.o"
    include "build/src/overlays/actors/ovl_Boss_Va/z_boss_va.o"
    include "build/src/overlays/actors/ovl_Demo_6K/z_demo_6k.o"
    include "build/src/overlays/actors/ovl_Demo_Du/z_demo_du.o"
    include "build/src/overlays/actors/ovl_Demo_Ec/z_demo_ec.o"
    include "build/src/overlays/actors/ovl_Demo_Effect/z_demo_effect.o"
    include "build/src/overlays/actors/ovl_Demo_Ext/z_demo_ext.o"
    include "build/src/overlays/actors/ovl_Demo_Geff/z_demo_geff.o"
    include "build/src/overlays/actors/ovl_Demo_Gj/z_demo_gj.o"
    include "build/src/overlays/actors/ovl_Demo_Go/z_demo_go.o"
    include "build/src/overlays/actors/ovl_Demo_Gt/z_demo_gt.o"
    include "build/src/overlays/actors/ovl_Demo_Ik/z_demo_ik.o"
    include "build/src/overlays/actors/ovl_Demo_Im/z_demo_im.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo_cutscene_data1.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo_cutscene_data2.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo_cutscene_data3.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo_cutscene_data4.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo_cutscene_data5.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo_cutscene_data6.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo_cutscene_data7.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo_cutscene_data8.o"
    include "build/src/overlays/actors/ovl_Demo_Kankyo/z_demo_kankyo.o"
    include "build/src/overlays/actors/ovl_Demo_Kekkai/z_demo_kekkai.o"
    include "build/src/overlays/actors/ovl_Demo_Sa/z_demo_sa.o"
    include "build/src/overlays/actors/ovl_Demo_Shd/z_demo_shd.o"
    include "build/src/overlays/actors/ovl_Demo_Tre_Lgt/z_demo_tre_lgt.o"
    include "build/src/overlays/actors/ovl_Door_Ana/z_door_ana.o"
    include "build/src/overlays/actors/ovl_Door_Gerudo/z_door_gerudo.o"
    include "build/src/overlays/actors/ovl_Door_Killer/z_door_killer.o"
    include "build/src/overlays/actors/ovl_Door_Shutter/z_door_shutter.o"
    include "build/src/overlays/actors/ovl_Door_Toki/z_door_toki.o"
    include "build/src/overlays/actors/ovl_Door_Warp1/z_door_warp1.o"
    include "build/src/overlays/actors/ovl_Efc_Erupc/z_efc_erupc.o"
    include "build/src/overlays/actors/ovl_Eff_Dust/z_eff_dust.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Blast/z_eff_ss_blast.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Bomb/z_eff_ss_bomb.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Bomb2/z_eff_ss_bomb2.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Bubble/z_eff_ss_bubble.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_D_Fire/z_eff_ss_d_fire.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Dead_Db/z_eff_ss_dead_db.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Dead_Dd/z_eff_ss_dead_dd.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Dead_Ds/z_eff_ss_dead_ds.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Dead_Sound/z_eff_ss_dead_sound.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Dt_Bubble/z_eff_ss_dt_bubble.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Dust/z_eff_ss_dust.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_En_Fire/z_eff_ss_en_fire.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_En_Ice/z_eff_ss_en_ice.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Extra/z_eff_ss_extra.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Fcircle/z_eff_ss_fcircle.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Fhg_Flash/z_eff_ss_fhg_flash.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Fire_Tail/z_eff_ss_fire_tail.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_G_Fire/z_eff_ss_g_fire.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_G_Magma/z_eff_ss_g_magma.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_G_Magma2/z_eff_ss_g_magma2.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_G_Ripple/z_eff_ss_g_ripple.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_G_Spk/z_eff_ss_g_spk.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_G_Splash/z_eff_ss_g_splash.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Hahen/z_eff_ss_hahen.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_HitMark/z_eff_ss_hitmark.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Ice_Piece/z_eff_ss_ice_piece.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Ice_Smoke/z_eff_ss_ice_smoke.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_K_Fire/z_eff_ss_k_fire.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_KiraKira/z_eff_ss_kirakira.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Lightning/z_eff_ss_lightning.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Sibuki/z_eff_ss_sibuki.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Sibuki2/z_eff_ss_sibuki2.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Solder_Srch_Ball/z_eff_ss_solder_srch_ball.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Stick/z_eff_ss_stick.o"
    include "build/src/overlays/effects/ovl_Effect_Ss_Stone1/z_eff_ss_stone1.o"
    include "build/src/overlays/actors/ovl_Elf_Msg/z_elf_msg.o"
    include "build/src/overlays/actors/ovl_Elf_Msg2/z_elf_msg2.o"
    include "build/src/overlays/actors/ovl_En_Am/z_en_am.o"
    include "build/src/overlays/actors/ovl_En_Ani/z_en_ani.o"
    include "build/src/overlays/actors/ovl_En_Anubice/z_en_anubice.o"
    include "build/src/overlays/actors/ovl_En_Anubice_Fire/z_en_anubice_fire.o"
    include "build/src/overlays/actors/ovl_En_Anubice_Tag/z_en_anubice_tag.o"
    include "build/src/overlays/actors/ovl_En_Arow_Trap/z_en_arow_trap.o"
    include "build/src/overlays/actors/ovl_En_Arrow/z_en_arrow.o"
    include "build/src/overlays/actors/ovl_En_Attack_Niw/z_en_attack_niw.o"
    include "build/src/overlays/actors/ovl_En_Ba/z_en_ba.o"
    include "build/src/overlays/actors/ovl_En_Bb/z_en_bb.o"
    include "build/src/overlays/actors/ovl_En_Bdfire/z_en_bdfire.o"
    include "build/src/overlays/actors/ovl_En_Bigokuta/z_en_bigokuta.o"
    include "build/src/overlays/actors/ovl_En_Bili/z_en_bili.o"
    include "build/src/overlays/actors/ovl_En_Bird/z_en_bird.o"
    include "build/src/overlays/actors/ovl_En_Blkobj/z_en_blkobj.o"
    include "build/src/overlays/actors/ovl_En_Bom/z_en_bom.o"
    include "build/src/overlays/actors/ovl_En_Bom_Bowl_Man/z_en_bom_bowl_man.o"
    include "build/src/overlays/actors/ovl_En_Bom_Bowl_Pit/z_en_bom_bowl_pit.o"
    include "build/src/overlays/actors/ovl_En_Bom_Chu/z_en_bom_chu.o"
    include "build/src/overlays/actors/ovl_En_Bombf/z_en_bombf.o"
    include "build/src/overlays/actors/ovl_En_Boom/z_en_boom.o"
    include "build/src/overlays/actors/ovl_En_Box/z_en_box.o"
    include "build/src/overlays/actors/ovl_En_Brob/z_en_brob.o"
    include "build/src/overlays/actors/ovl_En_Bubble/z_en_bubble.o"
    include "build/src/overlays/actors/ovl_En_Butte/z_en_butte.o"
    include "build/src/overlays/actors/ovl_En_Bw/z_en_bw.o"
    include "build/src/overlays/actors/ovl_En_Bx/z_en_bx.o"
    include "build/src/overlays/actors/ovl_En_Changer/z_en_changer.o"
    include "build/src/overlays/actors/ovl_En_Clear_Tag/z_en_clear_tag.o"
    include "build/src/overlays/actors/ovl_En_Cow/z_en_cow.o"
    include "build/src/overlays/actors/ovl_En_Crow/z_en_crow.o"
    include "build/src/overlays/actors/ovl_En_Cs/z_en_cs.o"
    include "build/src/overlays/actors/ovl_En_Daiku/z_en_daiku.o"
    include "build/src/overlays/actors/ovl_En_Daiku_Kakariko/z_en_daiku_kakariko.o"
    include "build/src/overlays/actors/ovl_En_Dekubaba/z_en_dekubaba.o"
    include "build/src/overlays/actors/ovl_En_Dekunuts/z_en_dekunuts.o"
    include "build/src/overlays/actors/ovl_En_Dh/z_en_dh.o"
    include "build/src/overlays/actors/ovl_En_Dha/z_en_dha.o"
    include "build/src/overlays/actors/ovl_En_Diving_Game/z_en_diving_game.o"
    include "build/src/overlays/actors/ovl_En_Dns/z_en_dns.o"
    include "build/src/overlays/actors/ovl_En_Dnt_Demo/z_en_dnt_demo.o"
    include "build/src/overlays/actors/ovl_En_Dnt_Jiji/z_en_dnt_jiji.o"
    include "build/src/overlays/actors/ovl_En_Dnt_Nomal/z_en_dnt_nomal.o"
    include "build/src/overlays/actors/ovl_En_Dodojr/z_en_dodojr.o"
    include "build/src/overlays/actors/ovl_En_Dodongo/z_en_dodongo.o"
    include "build/src/overlays/actors/ovl_En_Dog/z_en_dog.o"
    include "build/src/overlays/actors/ovl_En_Door/z_en_door.o"
    include "build/src/overlays/actors/ovl_En_Ds/z_en_ds.o"
    include "build/src/overlays/actors/ovl_En_Du/z_en_du.o"
    include "build/src/overlays/actors/ovl_En_Dy_Extra/z_en_dy_extra.o"
    include "build/src/overlays/actors/ovl_En_Eg/z_en_eg.o"
    include "build/src/overlays/actors/ovl_En_Eiyer/z_en_eiyer.o"
    include "build/src/overlays/actors/ovl_En_Elf/z_en_elf.o"
    include "build/src/overlays/actors/ovl_En_Encount1/z_en_encount1.o"
    include "build/src/overlays/actors/ovl_En_Encount2/z_en_encount2.o"
    include "build/src/overlays/actors/ovl_En_Ex_Item/z_en_ex_item.o"
    include "build/src/overlays/actors/ovl_En_Ex_Ruppy/z_en_ex_ruppy.o"
    include "build/src/overlays/actors/ovl_En_Fd/z_en_fd.o"
    include "build/src/overlays/actors/ovl_En_Fd_Fire/z_en_fd_fire.o"
    include "build/src/overlays/actors/ovl_En_Fhg_Fire/z_en_fhg_fire.o"
    include "build/src/overlays/actors/ovl_En_Fire_Rock/z_en_fire_rock.o"
    include "build/src/overlays/actors/ovl_En_Firefly/z_en_firefly.o"
    include "build/src/overlays/actors/ovl_En_Fish/z_en_fish.o"
    include "build/src/overlays/actors/ovl_En_Floormas/z_en_floormas.o"
    include "build/src/overlays/actors/ovl_En_Fr/z_en_fr.o"
    include "build/src/overlays/actors/ovl_En_Fu/z_en_fu.o"
    include "build/src/overlays/actors/ovl_En_Fw/z_en_fw.o"
    include "build/src/overlays/actors/ovl_En_Fz/z_en_fz.o"
    include "build/src/overlays/actors/ovl_En_G_Switch/z_en_g_switch.o"
    include "build/src/overlays/actors/ovl_En_Ganon_Mant/z_en_ganon_mant.o"
    include "build/src/overlays/actors/ovl_En_Ganon_Organ/z_en_ganon_organ.o"
    include "build/src/overlays/actors/ovl_En_Gb/z_en_gb.o"
    include "build/src/overlays/actors/ovl_En_Ge1/z_en_ge1.o"
    include "build/src/overlays/actors/ovl_En_Ge2/z_en_ge2.o"
    include "build/src/overlays/actors/ovl_En_Ge3/z_en_ge3.o"
    include "build/src/overlays/actors/ovl_En_GeldB/z_en_geldb.o"
    include "build/src/overlays/actors/ovl_En_GirlA/z_en_girla.o"
    include "build/src/overlays/actors/ovl_En_Gm/z_en_gm.o"
    include "build/src/overlays/actors/ovl_En_Go/z_en_go.o"
    include "build/src/overlays/actors/ovl_En_Go2/z_en_go2.o"
    include "build/src/overlays/actors/ovl_En_Goma/z_en_goma.o"
    include "build/src/overlays/actors/ovl_En_Goroiwa/z_en_goroiwa.o"
    include "build/src/overlays/actors/ovl_En_Gs/z_en_gs.o"
    include "build/src/overlays/actors/ovl_En_Guest/z_en_guest.o"
    include "build/src/overlays/actors/ovl_En_Hata/z_en_hata.o"
    include "build/src/overlays/actors/ovl_En_Heishi1/z_en_heishi1.o"
    include "build/src/overlays/actors/ovl_En_Heishi2/z_en_heishi2.o"
    include "build/src/overlays/actors/ovl_En_Heishi3/z_en_heishi3.o"
    include "build/src/overlays/actors/ovl_En_Heishi4/z_en_heishi4.o"
    include "build/src/overlays/actors/ovl_En_Hintnuts/z_en_hintnuts.o"
    include "build/src/overlays/actors/ovl_En_Holl/z_en_holl.o"
    include "build/src/overlays/actors/ovl_En_Honotrap/z_en_honotrap.o"
    include "build/src/overlays/actors/ovl_En_Horse/z_en_horse.o"
    include "build/src/overlays/actors/ovl_En_Horse_Game_Check/z_en_horse_game_check.o"
    include "build/src/overlays/actors/ovl_En_Horse_Ganon/z_en_horse_ganon.o"
    include "build/src/overlays/actors/ovl_En_Horse_Link_Child/z_en_horse_link_child.o"
    include "build/src/overlays/actors/ovl_En_Horse_Normal/z_en_horse_normal.o"
    include "build/src/overlays/actors/ovl_En_Horse_Zelda/z_en_horse_zelda.o"
    include "build/src/overlays/actors/ovl_En_Hs/z_en_hs.o"
    include "build/src/overlays/actors/ovl_En_Hs2/z_en_hs2.o"
    include "build/src/overlays/actors/ovl_En_Hy/z_en_hy.o"
    include "build/src/overlays/actors/ovl_En_Ice_Hono/z_en_ice_hono.o"
    include "build/src/overlays/actors/ovl_En_Ik/z_en_ik.o"
    include "build/src/overlays/actors/ovl_En_In/z_en_in.o"
    include "build/src/overlays/actors/ovl_En_Insect/z_en_insect.o"
    include "build/src/overlays/actors/ovl_En_Ishi/z_en_ishi.o"
    include "build/src/overlays/actors/ovl_En_It/z_en_it.o"
    include "build/src/overlays/actors/ovl_En_Jj/z_en_jj.o"
    include "build/src/overlays/actors/ovl_En_Js/z_en_js.o"
    include "build/src/overlays/actors/ovl_En_Jsjutan/z_en_jsjutan.o"
    include "build/src/overlays/actors/ovl_En_Kakasi/z_en_kakasi.o"
    include "build/src/overlays/actors/ovl_En_Kakasi2/z_en_kakasi2.o"
    include "build/src/overlays/actors/ovl_En_Kakasi3/z_en_kakasi3.o"
    include "build/src/overlays/actors/ovl_En_Kanban/z_en_kanban.o"
    include "build/src/overlays/actors/ovl_En_Karebaba/z_en_karebaba.o"
    include "build/src/overlays/actors/ovl_En_Ko/z_en_ko.o"
    include "build/src/overlays/actors/ovl_En_Kusa/z_en_kusa.o"
    include "build/src/overlays/actors/ovl_En_Kz/z_en_kz.o"
    include "build/src/overlays/actors/ovl_En_Light/z_en_light.o"
    include "build/src/overlays/actors/ovl_En_Lightbox/z_en_lightbox.o"
    include "build/src/overlays/actors/ovl_En_M_Fire1/z_en_m_fire1.o"
    include "build/src/overlays/actors/ovl_En_M_Thunder/z_en_m_thunder.o"
    include "build/src/overlays/actors/ovl_En_Ma1/z_en_ma1.o"
    include "build/src/overlays/actors/ovl_En_Ma2/z_en_ma2.o"
    include "build/src/overlays/actors/ovl_En_Ma3/z_en_ma3.o"
    include "build/src/overlays/actors/ovl_En_Mag/z_en_mag.o"
    include "build/src/overlays/actors/ovl_En_Mb/z_en_mb.o"
    include "build/src/overlays/actors/ovl_En_Md/z_en_md.o"
    include "build/src/overlays/actors/ovl_En_Mk/z_en_mk.o"
    include "build/src/overlays/actors/ovl_En_Mm/z_en_mm.o"
    include "build/src/overlays/actors/ovl_En_Mm2/z_en_mm2.o"
    include "build/src/overlays/actors/ovl_En_Ms/z_en_ms.o"
    include "build/src/overlays/actors/ovl_En_Mu/z_en_mu.o"
    include "build/src/overlays/actors/ovl_En_Nb/z_en_nb.o"
    include "build/src/overlays/actors/ovl_En_Niw/z_en_niw.o"
    include "build/src/overlays/actors/ovl_En_Niw_Girl/z_en_niw_girl.o"
    include "build/src/overlays/actors/ovl_En_Niw_Lady/z_en_niw_lady.o"
    include "build/src/overlays/actors/ovl_En_Nutsball/z_en_nutsball.o"
    include "build/src/overlays/actors/ovl_En_Nwc/z_en_nwc.o"
    include "build/src/overlays/actors/ovl_En_Ny/z_en_ny.o"
    include "build/src/overlays/actors/ovl_En_OE2/z_en_oe2.o"
    include "build/src/overlays/actors/ovl_En_Okarina_Effect/z_en_okarina_effect.o"
    include "build/src/overlays/actors/ovl_En_Okarina_Tag/z_en_okarina_tag_cutscene_data.o"
    include "build/src/overlays/actors/ovl_En_Okarina_Tag/z_en_okarina_tag.o"
    include "build/src/overlays/actors/ovl_En_Okuta/z_en_okuta.o"
    include "build/src/overlays/actors/ovl_En_Ossan/z_en_ossan.o"
    include "build/src/overlays/actors/ovl_En_Owl/z_en_owl.o"
    include "build/src/overlays/actors/ovl_En_Part/z_en_part.o"
    include "build/src/overlays/actors/ovl_En_Peehat/z_en_peehat.o"
    include "build/src/overlays/actors/ovl_En_Po_Desert/z_en_po_desert.o"
    include "build/src/overlays/actors/ovl_En_Po_Field/z_en_po_field.o"
    include "build/src/overlays/actors/ovl_En_Po_Relay/z_en_po_relay.o"
    include "build/src/overlays/actors/ovl_En_Po_Sisters/z_en_po_sisters.o"
    include "build/src/overlays/actors/ovl_En_Poh/z_en_poh.o"
    include "build/src/overlays/actors/ovl_En_Pu_box/z_en_pu_box.o"
    include "build/src/overlays/actors/ovl_En_Rd/z_en_rd.o"
    include "build/src/overlays/actors/ovl_En_Reeba/z_en_reeba.o"
    include "build/src/overlays/actors/ovl_En_River_Sound/z_en_river_sound.o"
    include "build/src/overlays/actors/ovl_En_Rl/z_en_rl.o"
    include "build/src/overlays/actors/ovl_En_Rr/z_en_rr.o"
    include "build/src/overlays/actors/ovl_En_Ru1/z_en_ru1.o"
    include "build/src/overlays/actors/ovl_En_Ru2/z_en_ru2.o"
    include "build/src/overlays/actors/ovl_En_Sa/z_en_sa.o"
    include "build/src/overlays/actors/ovl_En_Sb/z_en_sb.o"
    include "build/src/overlays/actors/ovl_En_Scene_Change/z_en_scene_change.o"
    include "build/src/overlays/actors/ovl_En_Sda/z_en_sda.o"
    include "build/src/overlays/actors/ovl_En_Shopnuts/z_en_shopnuts.o"
    include "build/src/overlays/actors/ovl_En_Si/z_en_si.o"
    include "build/src/overlays/actors/ovl_En_Siofuki/z_en_siofuki.o"
    include "build/src/overlays/actors/ovl_En_Skb/z_en_skb.o"
    include "build/src/overlays/actors/ovl_En_Skj/z_en_skj.o"
    include "build/src/overlays/actors/ovl_En_Skjneedle/z_en_skjneedle.o"
    include "build/src/overlays/actors/ovl_En_Ssh/z_en_ssh.o"
    include "build/src/overlays/actors/ovl_En_St/z_en_st.o"
    include "build/src/overlays/actors/ovl_En_Sth/z_en_sth.o"
    include "build/src/overlays/actors/ovl_En_Stream/z_en_stream.o"
    include "build/src/overlays/actors/ovl_En_Sw/z_en_sw.o"
    include "build/src/overlays/actors/ovl_En_Syateki_Itm/z_en_syateki_itm.o"
    include "build/src/overlays/actors/ovl_En_Syateki_Man/z_en_syateki_man.o"
    include "build/src/overlays/actors/ovl_En_Syateki_Niw/z_en_syateki_niw.o"
    include "build/src/overlays/actors/ovl_En_Ta/z_en_ta.o"
    include "build/src/overlays/actors/ovl_En_Takara_Man/z_en_takara_man.o"
    include "build/src/overlays/actors/ovl_En_Tana/z_en_tana.o"
    include "build/src/overlays/actors/ovl_En_Tg/z_en_tg.o"
    include "build/src/overlays/actors/ovl_En_Tite/z_en_tite.o"
    include "build/src/overlays/actors/ovl_En_Tk/z_en_tk.o"
    include "build/src/overlays/actors/ovl_En_Torch/z_en_torch.o"
    include "build/src/overlays/actors/ovl_En_Torch2/z_en_torch2.o"
    include "build/src/overlays/actors/ovl_En_Toryo/z_en_toryo.o"
    include "build/src/overlays/actors/ovl_En_Tp/z_en_tp.o"
    include "build/src/overlays/actors/ovl_En_Tr/z_en_tr.o"
    include "build/src/overlays/actors/ovl_En_Trap/z_en_trap.o"
    include "build/src/overlays/actors/ovl_En_Tubo_Trap/z_en_tubo_trap.o"
    include "build/src/overlays/actors/ovl_En_Vali/z_en_vali.o"
    include "build/src/overlays/actors/ovl_En_Vase/z_en_vase.o"
    include "build/src/overlays/actors/ovl_En_Vb_Ball/z_en_vb_ball.o"
    include "build/src/overlays/actors/ovl_En_Viewer/z_en_viewer.o"
    include "build/src/overlays/actors/ovl_En_Vm/z_en_vm.o"
    include "build/src/overlays/actors/ovl_En_Wall_Tubo/z_en_wall_tubo.o"
    include "build/src/overlays/actors/ovl_En_Wallmas/z_en_wallmas.o"
    include "build/src/overlays/actors/ovl_En_Weather_Tag/z_en_weather_tag.o"
    include "build/src/overlays/actors/ovl_En_Weiyer/z_en_weiyer.o"
    include "build/src/overlays/actors/ovl_En_Wf/z_en_wf.o"
    include "build/src/overlays/actors/ovl_En_Wonder_Item/z_en_wonder_item.o"
    include "build/src/overlays/actors/ovl_En_Wonder_Talk/z_en_wonder_talk.o"
    include "build/src/overlays/actors/ovl_En_Wonder_Talk2/z_en_wonder_talk2.o"
    include "build/src/overlays/actors/ovl_En_Wood02/z_en_wood02.o"
    include "build/src/overlays/actors/ovl_En_Xc/z_en_xc.o"
    include "build/src/overlays/actors/ovl_En_Yabusame_Mark/z_en_yabusame_mark.o"
    include "build/src/overlays/actors/ovl_En_Yukabyun/z_en_yukabyun.o"
    include "build/src/overlays/actors/ovl_En_Zf/z_en_zf.o"
    include "build/data/overlays/actors/z_en_zf.data.o"
    include "build/data/overlays/actors/z_en_zf.bss.o"
    include "build/src/overlays/actors/ovl_En_Zl1/z_en_zl1_cutscene_data.o"
    include "build/src/overlays/actors/ovl_En_Zl1/z_en_zl1.o"
    include "build/src/overlays/actors/ovl_En_Zl2/z_en_zl2.o"
    include "build/src/overlays/actors/ovl_En_Zl3/z_en_zl3.o"
    include "build/src/overlays/actors/ovl_En_Zl4/z_en_zl4.o"
    include "build/src/overlays/actors/ovl_En_Zo/z_en_zo.o"
    include "build/src/overlays/actors/ovl_En_fHG/z_en_fhg.o"
    include "build/src/overlays/actors/ovl_End_Title/z_end_title.o"
    include "build/src/overlays/actors/ovl_Fishing/z_fishing.o"
    include "build/src/overlays/actors/ovl_Item_B_Heart/z_item_b_heart.o"
    include "build/src/overlays/actors/ovl_Item_Etcetera/z_item_etcetera.o"
    include "build/src/overlays/actors/ovl_Item_Inbox/z_item_inbox.o"
    include "build/src/overlays/actors/ovl_Item_Ocarina/z_item_ocarina.o"
    include "build/src/overlays/actors/ovl_Item_Shield/z_item_shield.o"
    include "build/src/overlays/actors/ovl_Magic_Dark/z_magic_dark.o"
    include "build/src/overlays/actors/ovl_Magic_Fire/z_magic_fire.o"
    include "build/src/overlays/actors/ovl_Magic_Wind/z_magic_wind.o"
    include "build/src/overlays/actors/ovl_Mir_Ray/z_mir_ray.o"
    include "build/src/overlays/actors/ovl_Obj_Bean/z_obj_bean.o"
    include "build/src/overlays/actors/ovl_Obj_Blockstop/z_obj_blockstop.o"
    include "build/src/overlays/actors/ovl_Obj_Bombiwa/z_obj_bombiwa.o"
    include "build/src/overlays/actors/ovl_Obj_Comb/z_obj_comb.o"
    include "build/src/overlays/actors/ovl_Obj_Dekujr/z_obj_dekujr.o"
    include "build/src/overlays/actors/ovl_Obj_Elevator/z_obj_elevator.o"
    include "build/src/overlays/actors/ovl_Obj_Hamishi/z_obj_hamishi.o"
    include "build/src/overlays/actors/ovl_Obj_Hana/z_obj_hana.o"
    include "build/src/overlays/actors/ovl_Obj_Hsblock/z_obj_hsblock.o"
    include "build/src/overlays/actors/ovl_Obj_Ice_Poly/z_obj_ice_poly.o"
    include "build/src/overlays/actors/ovl_Obj_Kibako/z_obj_kibako.o"
    include "build/src/overlays/actors/ovl_Obj_Kibako2/z_obj_kibako2.o"
    include "build/src/overlays/actors/ovl_Obj_Lift/z_obj_lift.o"
    include "build/src/overlays/actors/ovl_Obj_Lightswitch/z_obj_lightswitch.o"
    include "build/src/overlays/actors/ovl_Obj_Makekinsuta/z_obj_makekinsuta.o"
    include "build/src/overlays/actors/ovl_Obj_Makeoshihiki/z_obj_makeoshihiki.o"
    include "build/src/overlays/actors/ovl_Obj_Mure/z_obj_mure.o"
    include "build/src/overlays/actors/ovl_Obj_Mure2/z_obj_mure2.o"
    include "build/src/overlays/actors/ovl_Obj_Mure3/z_obj_mure3.o"
    include "build/src/overlays/actors/ovl_Obj_Oshihiki/z_obj_oshihiki.o"
    include "build/src/overlays/actors/ovl_Obj_Roomtimer/z_obj_roomtimer.o"
    include "build/src/overlays/actors/ovl_Obj_Switch/z_obj_switch.o"
    include "build/src/overlays/actors/ovl_Obj_Syokudai/z_obj_syokudai.o"
    include "build/src/overlays/actors/ovl_Obj_Timeblock/z_obj_timeblock.o"
    include "build/src/overlays/actors/ovl_Obj_Tsubo/z_obj_tsubo.o"
    include "build/src/overlays/actors/ovl_Obj_Warp2block/z_obj_warp2block.o"
    include "build/src/overlays/actors/ovl_Object_Kankyo/z_object_kankyo.o"
    include "build/src/overlays/actors/ovl_Oceff_Spot/z_oceff_spot.o"
    include "build/src/overlays/actors/ovl_Oceff_Storm/z_oceff_storm.o"
    include "build/src/overlays/actors/ovl_Oceff_Wipe/z_oceff_wipe.o"
    include "build/src/overlays/actors/ovl_Oceff_Wipe2/z_oceff_wipe2.o"
    include "build/src/overlays/actors/ovl_Oceff_Wipe3/z_oceff_wipe3.o"
    include "build/src/overlays/actors/ovl_Oceff_Wipe4/z_oceff_wipe4.o"
    include "build/src/overlays/actors/ovl_Shot_Sun/z_shot_sun.o"
endseg

beginseg
    name "buffers"
    compress
    align 0x40
    include "build/src/buffers/zbuffer.o"
    include "build/src/buffers/gfxbuffers.o"
    include "build/src/buffers/heaps.o"
endseg



beginseg
    name "ovl_kaleido_scope"
    compress
    include "build/src/overlays/actors/ovl_kaleido_scope/z_kaleido_collect.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/z_kaleido_debug.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/z_kaleido_equipment.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/z_kaleido_item.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/z_kaleido_map_PAL.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/z_kaleido_prompt.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/z_kaleido_scope_PAL.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/z_lmap_mark.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/z_lmap_mark_data.o"
    include "build/src/overlays/actors/ovl_kaleido_scope/ovl_kaleido_scope_reloc.o"
endseg

beginseg
    name "ovl_player_actor"
    compress
    include "build/src/overlays/actors/ovl_player_actor/z_player.o"
    include "build/src/overlays/actors/ovl_player_actor/ovl_player_actor_reloc.o"
endseg

beginseg
    name "ovl_map_mark_data"
    compress
    include "build/src/overlays/ovl_map_mark_data/z_map_mark_data.o"
    include "build/src/overlays/ovl_map_mark_data/ovl_map_mark_data_reloc.o"
endseg


beginseg
    name "gameplay_keep"
    compress
    romalign 0x1000
    include "build/assets/objects/gameplay_keep/gameplay_keep.o"
    number 4
endseg

beginseg
    name "gameplay_field_keep"
    compress
    romalign 0x1000
    include "build/assets/objects/gameplay_field_keep/gameplay_field_keep.o"
    number 5
endseg

beginseg
    name "gameplay_dangeon_keep"
    compress
    romalign 0x1000
    include "build/assets/objects/gameplay_dangeon_keep/gameplay_dangeon_keep.o"
    number 5
endseg

beginseg
    name "gameplay_object_exchange_static"
    compress
    romalign 0x1000
    include "build/baserom/gameplay_object_exchange_static.o"
endseg

beginseg
    name "object_link_boy"
    compress
    romalign 0x1000
    include "build/baserom/object_link_boy.o"
    number 6
endseg

beginseg
    name "object_link_child"
    compress
    romalign 0x1000
    include "build/assets/objects/object_link_child/object_link_child.o"
    number 6
endseg

beginseg
    name "object_box"
    compress
    romalign 0x1000
    include "build/assets/objects/object_box/object_box.o"
    number 6
endseg

beginseg
    name "object_human"
    compress
    romalign 0x1000
    include "build/assets/objects/object_human/object_human.o"
    number 6
endseg

beginseg
    name "object_okuta"
    compress
    romalign 0x1000
    include "build/assets/objects/object_okuta/object_okuta.o"
    number 6
endseg

beginseg
    name "object_poh"
    compress
    romalign 0x1000
    include "build/assets/objects/object_poh/object_poh.o"
    number 6
endseg

beginseg
    name "object_wallmaster"
    compress
    romalign 0x1000
    include "build/assets/objects/object_wallmaster/object_wallmaster.o"
    number 6
endseg

beginseg
    name "object_dy_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dy_obj/object_dy_obj.o"
    number 6
endseg

beginseg
    name "object_firefly"
    compress
    romalign 0x1000
    include "build/assets/objects/object_firefly/object_firefly.o"
    number 6
endseg

beginseg
    name "object_dodongo"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dodongo/object_dodongo.o"
    number 6
endseg

beginseg
    name "object_fire"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fire/object_fire.o"
    number 6
endseg

beginseg
    name "object_niw"
    compress
    romalign 0x1000
    include "build/assets/objects/object_niw/object_niw.o"
    number 6
endseg

beginseg
    name "object_tite"
    compress
    romalign 0x1000
    include "build/assets/objects/object_tite/object_tite.o"
    number 6
endseg

beginseg
    name "object_reeba"
    compress
    romalign 0x1000
    include "build/assets/objects/object_reeba/object_reeba.o"
    number 6
endseg

beginseg
    name "object_peehat"
    compress
    romalign 0x1000
    include "build/assets/objects/object_peehat/object_peehat.o"
    number 6
endseg

beginseg
    name "object_kingdodongo"
    compress
    romalign 0x1000
    include "build/assets/objects/object_kingdodongo/object_kingdodongo.o"
    number 6
endseg

beginseg
    name "object_horse"
    compress
    romalign 0x1000
    include "build/assets/objects/object_horse/object_horse.o"
    number 6
endseg

beginseg
    name "object_zf"
    compress
    romalign 0x1000
    include "build/assets/objects/object_zf/object_zf.o"
    number 6
endseg

beginseg
    name "object_goma"
    compress
    romalign 0x1000
    include "build/assets/objects/object_goma/object_goma.o"
    number 6
endseg

beginseg
    name "object_zl1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_zl1/object_zl1.o"
    number 6
endseg

beginseg
    name "object_gol"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gol/object_gol.o"
    number 6
endseg

beginseg
    name "object_bubble"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bubble/object_bubble.o"
    number 6
endseg

beginseg
    name "object_dodojr"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dodojr/object_dodojr.o"
    number 6
endseg

beginseg
    name "object_torch2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_torch2/object_torch2.o"
    number 6
endseg

beginseg
    name "object_bl"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bl/object_bl.o"
    number 6
endseg

beginseg
    name "object_tp"
    compress
    romalign 0x1000
    include "build/assets/objects/object_tp/object_tp.o"
    number 6
endseg

beginseg
    name "object_oA1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA1/object_oA1.o"
    number 6
endseg

beginseg
    name "object_st"
    compress
    romalign 0x1000
    include "build/assets/objects/object_st/object_st.o"
    number 6
endseg

beginseg
    name "object_bw"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bw/object_bw.o"
    number 6
endseg

beginseg
    name "object_ei"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ei/object_ei.o"
    number 6
endseg

beginseg
    name "object_horse_normal"
    compress
    romalign 0x1000
    include "build/assets/objects/object_horse_normal/object_horse_normal.o"
    number 6
endseg

beginseg
    name "object_oB1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oB1/object_oB1.o"
    number 6
endseg

beginseg
    name "object_o_anime"
    compress
    romalign 0x1000
    include "build/assets/objects/object_o_anime/object_o_anime.o"
    number 6
endseg

beginseg
    name "object_spot04_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot04_objects/object_spot04_objects.o"
    number 6
endseg

beginseg
    name "object_ddan_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ddan_objects/object_ddan_objects.o"
    number 6
endseg

beginseg
    name "object_hidan_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_hidan_objects/object_hidan_objects.o"
    number 6
endseg

beginseg
    name "object_horse_ganon"
    compress
    romalign 0x1000
    include "build/assets/objects/object_horse_ganon/object_horse_ganon.o"
    number 6
endseg

beginseg
    name "object_oA2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA2/object_oA2.o"
    number 6
endseg

beginseg
    name "object_spot00_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot00_objects/object_spot00_objects.o"
    number 6
endseg

beginseg
    name "object_mb"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mb/object_mb.o"
    number 6
endseg

beginseg
    name "object_bombf"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bombf/object_bombf.o"
    number 6
endseg

beginseg
    name "object_sk2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_sk2/object_sk2.o"
    number 6
endseg

beginseg
    name "object_oE1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE1/object_oE1.o"
    number 6
endseg

beginseg
    name "object_oE_anime"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE_anime/object_oE_anime.o"
    number 6
endseg

beginseg
    name "object_oE2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE2/object_oE2.o"
    number 6
endseg

beginseg
    name "object_ydan_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ydan_objects/object_ydan_objects.o"
    number 6
endseg

beginseg
    name "object_gnd"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gnd/object_gnd.o"
    number 6
endseg

beginseg
    name "object_am"
    compress
    romalign 0x1000
    include "build/assets/objects/object_am/object_am.o"
    number 6
endseg

beginseg
    name "object_dekubaba"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dekubaba/object_dekubaba.o"
    number 6
endseg

beginseg
    name "object_oA3"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA3/object_oA3.o"
    number 6
endseg

beginseg
    name "object_oA4"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA4/object_oA4.o"
    number 6
endseg

beginseg
    name "object_oA5"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA5/object_oA5.o"
    number 6
endseg

beginseg
    name "object_oA6"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA6/object_oA6.o"
    number 6
endseg

beginseg
    name "object_oA7"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA7/object_oA7.o"
    number 6
endseg

beginseg
    name "object_jj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_jj/object_jj.o"
    number 6
endseg

beginseg
    name "object_oA8"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA8/object_oA8.o"
    number 6
endseg

beginseg
    name "object_oA9"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA9/object_oA9.o"
    number 6
endseg

beginseg
    name "object_oB2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oB2/object_oB2.o"
    number 6
endseg

beginseg
    name "object_oB3"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oB3/object_oB3.o"
    number 6
endseg

beginseg
    name "object_oB4"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oB4/object_oB4.o"
    number 6
endseg

beginseg
    name "object_horse_zelda"
    compress
    romalign 0x1000
    include "build/assets/objects/object_horse_zelda/object_horse_zelda.o"
    number 6
endseg

beginseg
    name "object_opening_demo1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_opening_demo1/object_opening_demo1.o"
    number 6
endseg

beginseg
    name "object_warp1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_warp1/object_warp1.o"
    number 6
endseg

beginseg
    name "object_b_heart"
    compress
    romalign 0x1000
    include "build/assets/objects/object_b_heart/object_b_heart.o"
    number 6
endseg

beginseg
    name "object_dekunuts"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dekunuts/object_dekunuts.o"
    number 6
endseg

beginseg
    name "object_oE3"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE3/object_oE3.o"
    number 6
endseg

beginseg
    name "object_oE4"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE4/object_oE4.o"
    number 6
endseg

beginseg
    name "object_menkuri_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_menkuri_objects/object_menkuri_objects.o"
    number 6
endseg

beginseg
    name "object_oE5"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE5/object_oE5.o"
    number 6
endseg

beginseg
    name "object_oE6"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE6/object_oE6.o"
    number 6
endseg

beginseg
    name "object_oE7"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE7/object_oE7.o"
    number 6
endseg

beginseg
    name "object_oE8"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE8/object_oE8.o"
    number 6
endseg

beginseg
    name "object_oE9"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE9/object_oE9.o"
    number 6
endseg

beginseg
    name "object_oE10"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE10/object_oE10.o"
    number 6
endseg

beginseg
    name "object_oE11"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE11/object_oE11.o"
    number 6
endseg

beginseg
    name "object_oE12"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE12/object_oE12.o"
    number 6
endseg

beginseg
    name "object_vali"
    compress
    romalign 0x1000
    include "build/assets/objects/object_vali/object_vali.o"
    number 6
endseg

beginseg
    name "object_oA10"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA10/object_oA10.o"
    number 6
endseg

beginseg
    name "object_oA11"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oA11/object_oA11.o"
    number 6
endseg

beginseg
    name "object_mizu_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mizu_objects/object_mizu_objects.o"
    number 6
endseg

beginseg
    name "object_fhg"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fhg/object_fhg.o"
    number 6
endseg

beginseg
    name "object_ossan"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ossan/object_ossan.o"
    number 6
endseg

beginseg
    name "object_mori_hineri1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mori_hineri1/object_mori_hineri1.o"
    number 6
endseg

beginseg
    name "object_Bb"
    compress
    romalign 0x1000
    include "build/assets/objects/object_Bb/object_Bb.o"
    number 6
endseg

beginseg
    name "object_toki_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_toki_objects/object_toki_objects.o"
    number 6
endseg

beginseg
    name "object_yukabyun"
    compress
    romalign 0x1000
    include "build/assets/objects/object_yukabyun/object_yukabyun.o"
    number 6
endseg

beginseg
    name "object_zl2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_zl2/object_zl2.o"
    number 6
endseg

beginseg
    name "object_mjin"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mjin/object_mjin.o"
    number 6
endseg

beginseg
    name "object_mjin_flash"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mjin_flash/object_mjin_flash.o"
    number 6
endseg

beginseg
    name "object_mjin_dark"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mjin_dark/object_mjin_dark.o"
    number 6
endseg

beginseg
    name "object_mjin_flame"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mjin_flame/object_mjin_flame.o"
    number 6
endseg

beginseg
    name "object_mjin_ice"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mjin_ice/object_mjin_ice.o"
    number 6
endseg

beginseg
    name "object_mjin_soul"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mjin_soul/object_mjin_soul.o"
    number 6
endseg

beginseg
    name "object_mjin_wind"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mjin_wind/object_mjin_wind.o"
    number 6
endseg

beginseg
    name "object_mjin_oka"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mjin_oka/object_mjin_oka.o"
    number 6
endseg

beginseg
    name "object_haka_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_haka_objects/object_haka_objects.o"
    number 6
endseg

beginseg
    name "object_spot06_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot06_objects/object_spot06_objects.o"
    number 6
endseg

beginseg
    name "object_ice_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ice_objects/object_ice_objects.o"
    number 6
endseg

beginseg
    name "object_relay_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_relay_objects/object_relay_objects.o"
    number 6
endseg

beginseg
    name "object_mori_hineri1a"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mori_hineri1a/object_mori_hineri1a.o"
    number 6
endseg

beginseg
    name "object_mori_hineri2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mori_hineri2/object_mori_hineri2.o"
    number 6
endseg

beginseg
    name "object_mori_hineri2a"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mori_hineri2a/object_mori_hineri2a.o"
    number 6
endseg

beginseg
    name "object_mori_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mori_objects/object_mori_objects.o"
    number 6
endseg

beginseg
    name "object_mori_tex"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mori_tex/object_mori_tex.o"
    number 6
endseg

beginseg
    name "object_spot08_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot08_obj/object_spot08_obj.o"
    number 6
endseg

beginseg
    name "object_warp2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_warp2/object_warp2.o"
    number 6
endseg

beginseg
    name "object_hata"
    compress
    romalign 0x1000
    include "build/assets/objects/object_hata/object_hata.o"
    number 6
endseg

beginseg
    name "object_bird"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bird/object_bird.o"
    number 6
endseg

beginseg
    name "object_wood02"
    compress
    romalign 0x1000
    include "build/assets/objects/object_wood02/object_wood02.o"
    number 6
endseg

beginseg
    name "object_lightbox"
    compress
    romalign 0x1000
    include "build/assets/objects/object_lightbox/object_lightbox.o"
    number 6
endseg

beginseg
    name "object_pu_box"
    compress
    romalign 0x1000
    include "build/assets/objects/object_pu_box/object_pu_box.o"
    number 6
endseg

beginseg
    name "object_trap"
    compress
    romalign 0x1000
    include "build/assets/objects/object_trap/object_trap.o"
    number 6
endseg

beginseg
    name "object_vase"
    compress
    romalign 0x1000
    include "build/assets/objects/object_vase/object_vase.o"
    number 6
endseg

beginseg
    name "object_im"
    compress
    romalign 0x1000
    include "build/assets/objects/object_im/object_im.o"
    number 6
endseg

beginseg
    name "object_ta"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ta/object_ta.o"
    number 6
endseg

beginseg
    name "object_tk"
    compress
    romalign 0x1000
    include "build/assets/objects/object_tk/object_tk.o"
    number 6
endseg

beginseg
    name "object_xc"
    compress
    romalign 0x1000
    include "build/assets/objects/object_xc/object_xc.o"
    number 6
endseg

beginseg
    name "object_vm"
    compress
    romalign 0x1000
    include "build/assets/objects/object_vm/object_vm.o"
    number 6
endseg

beginseg
    name "object_bv"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bv/object_bv.o"
    number 6
endseg

beginseg
    name "object_hakach_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_hakach_objects/object_hakach_objects.o"
    number 6
endseg

beginseg
    name "object_efc_crystal_light"
    compress
    romalign 0x1000
    include "build/assets/objects/object_efc_crystal_light/object_efc_crystal_light.o"
    number 6
endseg

beginseg
    name "object_efc_fire_ball"
    compress
    romalign 0x1000
    include "build/assets/objects/object_efc_fire_ball/object_efc_fire_ball.o"
    number 6
endseg

beginseg
    name "object_efc_flash"
    compress
    romalign 0x1000
    include "build/assets/objects/object_efc_flash/object_efc_flash.o"
    number 6
endseg

beginseg
    name "object_efc_lgt_shower"
    compress
    romalign 0x1000
    include "build/assets/objects/object_efc_lgt_shower/object_efc_lgt_shower.o"
    number 6
endseg

beginseg
    name "object_efc_star_field"
    compress
    romalign 0x1000
    include "build/assets/objects/object_efc_star_field/object_efc_star_field.o"
    number 6
endseg

beginseg
    name "object_god_lgt"
    compress
    romalign 0x1000
    include "build/assets/objects/object_god_lgt/object_god_lgt.o"
    number 6
endseg

beginseg
    name "object_light_ring"
    compress
    romalign 0x1000
    include "build/assets/objects/object_light_ring/object_light_ring.o"
    number 6
endseg

beginseg
    name "object_triforce_spot"
    compress
    romalign 0x1000
    include "build/assets/objects/object_triforce_spot/object_triforce_spot.o"
    number 6
endseg

beginseg
    name "object_medal"
    compress
    romalign 0x1000
    include "build/assets/objects/object_medal/object_medal.o"
    number 6
endseg

beginseg
    name "object_bdan_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bdan_objects/object_bdan_objects.o"
    number 6
endseg

beginseg
    name "object_sd"
    compress
    romalign 0x1000
    include "build/assets/objects/object_sd/object_sd.o"
    number 6
endseg

beginseg
    name "object_rd"
    compress
    romalign 0x1000
    include "build/assets/objects/object_rd/object_rd.o"
    number 6
endseg

beginseg
    name "object_po_sisters"
    compress
    romalign 0x1000
    include "build/assets/objects/object_po_sisters/object_po_sisters.o"
    number 6
endseg

beginseg
    name "object_heavy_object"
    compress
    romalign 0x1000
    include "build/assets/objects/object_heavy_object/object_heavy_object.o"
    number 6
endseg

beginseg
    name "object_gndd"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gndd/object_gndd.o"
    number 6
endseg

beginseg
    name "object_fd"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fd/object_fd.o"
    number 6
endseg

beginseg
    name "object_du"
    compress
    romalign 0x1000
    include "build/assets/objects/object_du/object_du.o"
    number 6
endseg

beginseg
    name "object_fw"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fw/object_fw.o"
    number 6
endseg

beginseg
    name "object_horse_link_child"
    compress
    romalign 0x1000
    include "build/assets/objects/object_horse_link_child/object_horse_link_child.o"
    number 6
endseg

beginseg
    name "object_spot02_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot02_objects/object_spot02_objects.o"
    number 6
endseg

beginseg
    name "object_haka"
    compress
    romalign 0x1000
    include "build/assets/objects/object_haka/object_haka.o"
    number 6
endseg

beginseg
    name "object_ru1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ru1/object_ru1.o"
    number 6
endseg

beginseg
    name "object_syokudai"
    compress
    romalign 0x1000
    include "build/assets/objects/object_syokudai/object_syokudai.o"
    number 6
endseg

beginseg
    name "object_fd2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fd2/object_fd2.o"
    number 6
endseg

beginseg
    name "object_dh"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dh/object_dh.o"
    number 6
endseg

beginseg
    name "object_rl"
    compress
    romalign 0x1000
    include "build/assets/objects/object_rl/object_rl.o"
    number 6
endseg

beginseg
    name "object_efc_tw"
    compress
    romalign 0x1000
    include "build/assets/objects/object_efc_tw/object_efc_tw.o"
    number 6
endseg

beginseg
    name "object_demo_tre_lgt"
    compress
    romalign 0x1000
    include "build/assets/objects/object_demo_tre_lgt/object_demo_tre_lgt.o"
    number 6
endseg

beginseg
    name "object_gi_key"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_key/object_gi_key.o"
    number 6
endseg

beginseg
    name "object_mir_ray"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mir_ray/object_mir_ray.o"
    number 6
endseg

beginseg
    name "object_brob"
    compress
    romalign 0x1000
    include "build/assets/objects/object_brob/object_brob.o"
    number 6
endseg

beginseg
    name "object_gi_jewel"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_jewel/object_gi_jewel.o"
    number 6
endseg

beginseg
    name "object_spot09_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot09_obj/object_spot09_obj.o"
    number 6
endseg

beginseg
    name "object_spot18_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot18_obj/object_spot18_obj.o"
    number 6
endseg

beginseg
    name "object_bdoor"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bdoor/object_bdoor.o"
    number 6
endseg

beginseg
    name "object_spot17_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot17_obj/object_spot17_obj.o"
    number 6
endseg

beginseg
    name "object_shop_dungen"
    compress
    romalign 0x1000
    include "build/assets/objects/object_shop_dungen/object_shop_dungen.o"
    number 6
endseg

beginseg
    name "object_nb"
    compress
    romalign 0x1000
    include "build/assets/objects/object_nb/object_nb.o"
    number 6
endseg

beginseg
    name "object_mo"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mo/object_mo.o"
    number 6
endseg

beginseg
    name "object_sb"
    compress
    romalign 0x1000
    include "build/assets/objects/object_sb/object_sb.o"
    number 6
endseg

beginseg
    name "object_gi_melody"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_melody/object_gi_melody.o"
    number 6
endseg

beginseg
    name "object_gi_heart"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_heart/object_gi_heart.o"
    number 6
endseg

beginseg
    name "object_gi_compass"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_compass/object_gi_compass.o"
    number 6
endseg

beginseg
    name "object_gi_bosskey"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bosskey/object_gi_bosskey.o"
    number 6
endseg

beginseg
    name "object_gi_medal"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_medal/object_gi_medal.o"
    number 6
endseg

beginseg
    name "object_gi_nuts"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_nuts/object_gi_nuts.o"
    number 6
endseg

beginseg
    name "object_sa"
    compress
    romalign 0x1000
    include "build/assets/objects/object_sa/object_sa.o"
    number 6
endseg

beginseg
    name "object_gi_hearts"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_hearts/object_gi_hearts.o"
    number 6
endseg

beginseg
    name "object_gi_arrowcase"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_arrowcase/object_gi_arrowcase.o"
    number 6
endseg

beginseg
    name "object_gi_bombpouch"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bombpouch/object_gi_bombpouch.o"
    number 6
endseg

beginseg
    name "object_in"
    compress
    romalign 0x1000
    include "build/assets/objects/object_in/object_in.o"
    number 6
endseg

beginseg
    name "object_tr"
    compress
    romalign 0x1000
    include "build/assets/objects/object_tr/object_tr.o"
    number 6
endseg

beginseg
    name "object_spot16_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot16_obj/object_spot16_obj.o"
    number 6
endseg

beginseg
    name "object_oE1s"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE1s/object_oE1s.o"
    number 6
endseg

beginseg
    name "object_oE4s"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oE4s/object_oE4s.o"
    number 6
endseg

beginseg
    name "object_os_anime"
    compress
    romalign 0x1000
    include "build/assets/objects/object_os_anime/object_os_anime.o"
    number 6
endseg

beginseg
    name "object_gi_bottle"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bottle/object_gi_bottle.o"
    number 6
endseg

beginseg
    name "object_gi_stick"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_stick/object_gi_stick.o"
    number 6
endseg

beginseg
    name "object_gi_map"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_map/object_gi_map.o"
    number 6
endseg

beginseg
    name "object_oF1d_map"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oF1d_map/object_oF1d_map.o"
    number 6
endseg

beginseg
    name "object_ru2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ru2/object_ru2.o"
    number 6
endseg

beginseg
    name "object_gi_shield_1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_shield_1/object_gi_shield_1.o"
    number 6
endseg

beginseg
    name "object_dekujr"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dekujr/object_dekujr.o"
    number 6
endseg

beginseg
    name "object_gi_magicpot"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_magicpot/object_gi_magicpot.o"
    number 6
endseg

beginseg
    name "object_gi_bomb_1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bomb_1/object_gi_bomb_1.o"
    number 6
endseg

beginseg
    name "object_oF1s"
    compress
    romalign 0x1000
    include "build/assets/objects/object_oF1s/object_oF1s.o"
    number 6
endseg

beginseg
    name "object_ma2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ma2/object_ma2.o"
    number 6
endseg

beginseg
    name "object_gi_purse"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_purse/object_gi_purse.o"
    number 6
endseg

beginseg
    name "object_hni"
    compress
    romalign 0x1000
    include "build/assets/objects/object_hni/object_hni.o"
    number 6
endseg

beginseg
    name "object_tw"
    compress
    romalign 0x1000
    include "build/assets/objects/object_tw/object_tw.o"
    number 6
endseg

beginseg
    name "object_rr"
    compress
    romalign 0x1000
    include "build/assets/objects/object_rr/object_rr.o"
    number 6
endseg

beginseg
    name "object_bxa"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bxa/object_bxa.o"
    number 6
endseg

beginseg
    name "object_anubice"
    compress
    romalign 0x1000
    include "build/assets/objects/object_anubice/object_anubice.o"
    number 6
endseg

beginseg
    name "object_gi_gerudo"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_gerudo/object_gi_gerudo.o"
    number 6
endseg

beginseg
    name "object_gi_arrow"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_arrow/object_gi_arrow.o"
    number 6
endseg

beginseg
    name "object_gi_bomb_2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bomb_2/object_gi_bomb_2.o"
    number 6
endseg

beginseg
    name "object_gi_egg"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_egg/object_gi_egg.o"
    number 6
endseg

beginseg
    name "object_gi_scale"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_scale/object_gi_scale.o"
    number 6
endseg

beginseg
    name "object_gi_shield_2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_shield_2/object_gi_shield_2.o"
    number 6
endseg

beginseg
    name "object_gi_hookshot"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_hookshot/object_gi_hookshot.o"
    number 6
endseg

beginseg
    name "object_gi_ocarina"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_ocarina/object_gi_ocarina.o"
    number 6
endseg

beginseg
    name "object_gi_milk"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_milk/object_gi_milk.o"
    number 6
endseg

beginseg
    name "object_ma1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ma1/object_ma1.o"
    number 6
endseg

beginseg
    name "object_ganon"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ganon/object_ganon.o"
    number 6
endseg

beginseg
    name "object_sst"
    compress
    romalign 0x1000
    include "build/assets/objects/object_sst/object_sst.o"
    number 6
endseg

beginseg
    name "object_ny"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ny/object_ny.o"
    number 6
endseg

beginseg
    name "object_fr"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fr/object_fr.o"
    number 6
endseg

beginseg
    name "object_gi_pachinko"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_pachinko/object_gi_pachinko.o"
    number 6
endseg

beginseg
    name "object_gi_boomerang"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_boomerang/object_gi_boomerang.o"
    number 6
endseg

beginseg
    name "object_gi_bow"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bow/object_gi_bow.o"
    number 6
endseg

beginseg
    name "object_gi_glasses"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_glasses/object_gi_glasses.o"
    number 6
endseg

beginseg
    name "object_gi_liquid"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_liquid/object_gi_liquid.o"
    number 6
endseg

beginseg
    name "object_ani"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ani/object_ani.o"
    number 6
endseg

beginseg
    name "object_demo_6k"
    compress
    romalign 0x1000
    include "build/assets/objects/object_demo_6k/object_demo_6k.o"
    number 6
endseg

beginseg
    name "object_gi_shield_3"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_shield_3/object_gi_shield_3.o"
    number 6
endseg

beginseg
    name "object_gi_letter"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_letter/object_gi_letter.o"
    number 6
endseg

beginseg
    name "object_spot15_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot15_obj/object_spot15_obj.o"
    number 6
endseg

beginseg
    name "object_jya_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_jya_obj/object_jya_obj.o"
    number 6
endseg

beginseg
    name "object_gi_clothes"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_clothes/object_gi_clothes.o"
    number 6
endseg

beginseg
    name "object_gi_bean"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bean/object_gi_bean.o"
    number 6
endseg

beginseg
    name "object_gi_fish"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_fish/object_gi_fish.o"
    number 6
endseg

beginseg
    name "object_gi_saw"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_saw/object_gi_saw.o"
    number 6
endseg

beginseg
    name "object_gi_hammer"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_hammer/object_gi_hammer.o"
    number 6
endseg

beginseg
    name "object_gi_grass"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_grass/object_gi_grass.o"
    number 6
endseg

beginseg
    name "object_gi_longsword"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_longsword/object_gi_longsword.o"
    number 6
endseg

beginseg
    name "object_spot01_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot01_objects/object_spot01_objects.o"
    number 6
endseg

beginseg
    name "object_md"
    compress
    romalign 0x1000
    include "build/assets/objects/object_md/object_md.o"
    number 6
endseg

beginseg
    name "object_km1"
    compress
    romalign 0x1000
    include "build/baserom/object_km1.o"
    //include "build/assets/objects/object_km1/object_km1.o"
    number 6
endseg

beginseg
    name "object_kw1"
    compress
    romalign 0x1000
    include "build/baserom/object_kw1.o"
    //include "build/assets/objects/object_kw1/object_kw1.o"
    number 6
endseg

beginseg
    name "object_zo"
    compress
    romalign 0x1000
    include "build/assets/objects/object_zo/object_zo.o"
    number 6
endseg

beginseg
    name "object_kz"
    compress
    romalign 0x1000
    include "build/assets/objects/object_kz/object_kz.o"
    number 6
endseg

beginseg
    name "object_umajump"
    compress
    romalign 0x1000
    include "build/assets/objects/object_umajump/object_umajump.o"
    number 6
endseg

beginseg
    name "object_masterkokiri"
    compress
    romalign 0x1000
    include "build/assets/objects/object_masterkokiri/object_masterkokiri.o"
    number 6
endseg

beginseg
    name "object_masterkokirihead"
    compress
    romalign 0x1000
    include "build/assets/objects/object_masterkokirihead/object_masterkokirihead.o"
    number 6
endseg

beginseg
    name "object_mastergolon"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mastergolon/object_mastergolon.o"
    number 6
endseg

beginseg
    name "object_masterzoora"
    compress
    romalign 0x1000
    include "build/assets/objects/object_masterzoora/object_masterzoora.o"
    number 6
endseg

beginseg
    name "object_aob"
    compress
    romalign 0x1000
    include "build/assets/objects/object_aob/object_aob.o"
    number 6
endseg

beginseg
    name "object_ik"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ik/object_ik.o"
    number 6
endseg

beginseg
    name "object_ahg"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ahg/object_ahg.o"
    number 6
endseg

beginseg
    name "object_cne"
    compress
    romalign 0x1000
    include "build/assets/objects/object_cne/object_cne.o"
    number 6
endseg

beginseg
    name "object_gi_niwatori"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_niwatori/object_gi_niwatori.o"
    number 6
endseg

beginseg
    name "object_skj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_skj/object_skj.o"
    number 6
endseg

beginseg
    name "object_gi_bottle_letter"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bottle_letter/object_gi_bottle_letter.o"
    number 6
endseg

beginseg
    name "object_bji"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bji/object_bji.o"
    number 6
endseg

beginseg
    name "object_bba"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bba/object_bba.o"
    number 6
endseg

beginseg
    name "object_gi_ocarina_0"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_ocarina_0/object_gi_ocarina_0.o"
    number 6
endseg

beginseg
    name "object_ds"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ds/object_ds.o"
    number 6
endseg

beginseg
    name "object_ane"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ane/object_ane.o"
    number 6
endseg

beginseg
    name "object_boj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_boj/object_boj.o"
    number 6
endseg

beginseg
    name "object_spot03_object"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot03_object/object_spot03_object.o"
    number 6
endseg

beginseg
    name "object_spot07_object"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot07_object/object_spot07_object.o"
    number 6
endseg

beginseg
    name "object_fz"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fz/object_fz.o"
    number 6
endseg

beginseg
    name "object_bob"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bob/object_bob.o"
    number 6
endseg

beginseg
    name "object_ge1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ge1/object_ge1.o"
    number 6
endseg

beginseg
    name "object_yabusame_point"
    compress
    romalign 0x1000
    include "build/assets/objects/object_yabusame_point/object_yabusame_point.o"
    number 6
endseg

beginseg
    name "object_gi_boots_2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_boots_2/object_gi_boots_2.o"
    number 6
endseg

beginseg
    name "object_gi_seed"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_seed/object_gi_seed.o"
    number 6
endseg

beginseg
    name "object_gnd_magic"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gnd_magic/object_gnd_magic.o"
    number 6
endseg

beginseg
    name "object_d_elevator"
    compress
    romalign 0x1000
    include "build/assets/objects/object_d_elevator/object_d_elevator.o"
    number 6
endseg

beginseg
    name "object_d_hsblock"
    compress
    romalign 0x1000
    include "build/assets/objects/object_d_hsblock/object_d_hsblock.o"
    number 6
endseg

beginseg
    name "object_d_lift"
    compress
    romalign 0x1000
    include "build/assets/objects/object_d_lift/object_d_lift.o"
    number 6
endseg

beginseg
    name "object_mamenoki"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mamenoki/object_mamenoki.o"
    number 6
endseg

beginseg
    name "object_goroiwa"
    compress
    romalign 0x1000
    include "build/assets/objects/object_goroiwa/object_goroiwa.o"
    number 6
endseg

beginseg
    name "object_toryo"
    compress
    romalign 0x1000
    include "build/assets/objects/object_toryo/object_toryo.o"
    number 6
endseg

beginseg
    name "object_daiku"
    compress
    romalign 0x1000
    include "build/assets/objects/object_daiku/object_daiku.o"
    number 6
endseg

beginseg
    name "object_nwc"
    compress
    romalign 0x1000
    include "build/assets/objects/object_nwc/object_nwc.o"
    number 6
endseg

beginseg
    name "object_blkobj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_blkobj/object_blkobj.o"
    number 6
endseg

beginseg
    name "object_gm"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gm/object_gm.o"
    number 6
endseg

beginseg
    name "object_ms"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ms/object_ms.o"
    number 6
endseg

beginseg
    name "object_hs"
    compress
    romalign 0x1000
    include "build/assets/objects/object_hs/object_hs.o"
    number 6
endseg

beginseg
    name "object_ingate"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ingate/object_ingate.o"
    number 6
endseg

beginseg
    name "object_lightswitch"
    compress
    romalign 0x1000
    include "build/assets/objects/object_lightswitch/object_lightswitch.o"
    number 6
endseg

beginseg
    name "object_kusa"
    compress
    romalign 0x1000
    include "build/assets/objects/object_kusa/object_kusa.o"
    number 6
endseg

beginseg
    name "object_tsubo"
    compress
    romalign 0x1000
    include "build/assets/objects/object_tsubo/object_tsubo.o"
    number 6
endseg

beginseg
    name "object_gi_gloves"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_gloves/object_gi_gloves.o"
    number 6
endseg

beginseg
    name "object_gi_coin"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_coin/object_gi_coin.o"
    number 6
endseg

beginseg
    name "object_kanban"
    compress
    romalign 0x1000
    include "build/assets/objects/object_kanban/object_kanban.o"
    number 6
endseg

beginseg
    name "object_gjyo_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gjyo_objects/object_gjyo_objects.o"
    number 6
endseg

beginseg
    name "object_owl"
    compress
    romalign 0x1000
    include "build/assets/objects/object_owl/object_owl.o"
    number 6
endseg

beginseg
    name "object_mk"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mk/object_mk.o"
    number 6
endseg

beginseg
    name "object_fu"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fu/object_fu.o"
    number 6
endseg

beginseg
    name "object_gi_ki_tan_mask"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_ki_tan_mask/object_gi_ki_tan_mask.o"
    number 6
endseg

beginseg
    name "object_gi_redead_mask"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_redead_mask/object_gi_redead_mask.o"
    number 6
endseg

beginseg
    name "object_gi_skj_mask"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_skj_mask/object_gi_skj_mask.o"
    number 6
endseg

beginseg
    name "object_gi_rabit_mask"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_rabit_mask/object_gi_rabit_mask.o"
    number 6
endseg

beginseg
    name "object_gi_truth_mask"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_truth_mask/object_gi_truth_mask.o"
    number 6
endseg

beginseg
    name "object_ganon_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ganon_objects/object_ganon_objects.o"
    number 6
endseg

beginseg
    name "object_siofuki"
    compress
    romalign 0x1000
    include "build/assets/objects/object_siofuki/object_siofuki.o"
    number 6
endseg

beginseg
    name "object_stream"
    compress
    romalign 0x1000
    include "build/assets/objects/object_stream/object_stream.o"
    number 6
endseg

beginseg
    name "object_mm"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mm/object_mm.o"
    number 6
endseg

beginseg
    name "object_fa"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fa/object_fa.o"
    number 6
endseg

beginseg
    name "object_os"
    compress
    romalign 0x1000
    include "build/assets/objects/object_os/object_os.o"
    number 6
endseg

beginseg
    name "object_gi_eye_lotion"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_eye_lotion/object_gi_eye_lotion.o"
    number 6
endseg

beginseg
    name "object_gi_powder"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_powder/object_gi_powder.o"
    number 6
endseg

beginseg
    name "object_gi_mushroom"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_mushroom/object_gi_mushroom.o"
    number 6
endseg

beginseg
    name "object_gi_ticketstone"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_ticketstone/object_gi_ticketstone.o"
    number 6
endseg

beginseg
    name "object_gi_brokensword"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_brokensword/object_gi_brokensword.o"
    number 6
endseg

beginseg
    name "object_js"
    compress
    romalign 0x1000
    include "build/assets/objects/object_js/object_js.o"
    number 6
endseg

beginseg
    name "object_cs"
    compress
    romalign 0x1000
    include "build/assets/objects/object_cs/object_cs.o"
    number 6
endseg

beginseg
    name "object_gi_prescription"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_prescription/object_gi_prescription.o"
    number 6
endseg

beginseg
    name "object_gi_bracelet"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_bracelet/object_gi_bracelet.o"
    number 6
endseg

beginseg
    name "object_gi_soldout"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_soldout/object_gi_soldout.o"
    number 6
endseg

beginseg
    name "object_gi_frog"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_frog/object_gi_frog.o"
    number 6
endseg

beginseg
    name "object_mag"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mag/object_mag.o"
    number 6
endseg

beginseg
    name "object_door_gerudo"
    compress
    romalign 0x1000
    include "build/assets/objects/object_door_gerudo/object_door_gerudo.o"
    number 6
endseg

beginseg
    name "object_gt"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gt/object_gt.o"
    number 6
endseg

beginseg
    name "object_efc_erupc"
    compress
    romalign 0x1000
    include "build/assets/objects/object_efc_erupc/object_efc_erupc.o"
    number 6
endseg

beginseg
    name "object_zl2_anime1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_zl2_anime1/object_zl2_anime1.o"
    number 6
endseg

beginseg
    name "object_zl2_anime2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_zl2_anime2/object_zl2_anime2.o"
    number 6
endseg

beginseg
    name "object_gi_golonmask"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_golonmask/object_gi_golonmask.o"
    number 6
endseg

beginseg
    name "object_gi_zoramask"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_zoramask/object_gi_zoramask.o"
    number 6
endseg

beginseg
    name "object_gi_gerudomask"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_gerudomask/object_gi_gerudomask.o"
    number 6
endseg

beginseg
    name "object_ganon2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ganon2/object_ganon2.o"
    number 6
endseg

beginseg
    name "object_ka"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ka/object_ka.o"
    number 6
endseg

beginseg
    name "object_ts"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ts/object_ts.o"
    number 6
endseg

beginseg
    name "object_zg"
    compress
    romalign 0x1000
    include "build/assets/objects/object_zg/object_zg.o"
    number 6
endseg

beginseg
    name "object_gi_hoverboots"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_hoverboots/object_gi_hoverboots.o"
    number 6
endseg

beginseg
    name "object_gi_m_arrow"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_m_arrow/object_gi_m_arrow.o"
    number 6
endseg

beginseg
    name "object_ds2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ds2/object_ds2.o"
    number 6
endseg

beginseg
    name "object_ec"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ec/object_ec.o"
    number 6
endseg

beginseg
    name "object_fish"
    compress
    romalign 0x1000
    include "build/assets/objects/object_fish/object_fish.o"
    number 6
endseg

beginseg
    name "object_gi_sutaru"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_sutaru/object_gi_sutaru.o"
    number 6
endseg

beginseg
    name "object_gi_goddess"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_goddess/object_gi_goddess.o"
    number 6
endseg

beginseg
    name "object_ssh"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ssh/object_ssh.o"
    number 6
endseg

beginseg
    name "object_bigokuta"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bigokuta/object_bigokuta.o"
    number 6
endseg

beginseg
    name "object_bg"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bg/object_bg.o"
    number 6
endseg

beginseg
    name "object_spot05_objects"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot05_objects/object_spot05_objects.o"
    number 6
endseg

beginseg
    name "object_spot12_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot12_obj/object_spot12_obj.o"
    number 6
endseg

beginseg
    name "object_bombiwa"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bombiwa/object_bombiwa.o"
    number 6
endseg

beginseg
    name "object_hintnuts"
    compress
    romalign 0x1000
    include "build/assets/objects/object_hintnuts/object_hintnuts.o"
    number 6
endseg

beginseg
    name "object_rs"
    compress
    romalign 0x1000
    include "build/assets/objects/object_rs/object_rs.o"
    number 6
endseg

beginseg
    name "object_spot00_break"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot00_break/object_spot00_break.o"
    number 6
endseg

beginseg
    name "object_gla"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gla/object_gla.o"
    number 6
endseg

beginseg
    name "object_shopnuts"
    compress
    romalign 0x1000
    include "build/assets/objects/object_shopnuts/object_shopnuts.o"
    number 6
endseg

beginseg
    name "object_geldb"
    compress
    romalign 0x1000
    include "build/assets/objects/object_geldb/object_geldb.o"
    number 6
endseg

beginseg
    name "object_gr"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gr/object_gr.o"
    number 6
endseg

beginseg
    name "object_dog"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dog/object_dog.o"
    number 6
endseg

beginseg
    name "object_jya_iron"
    compress
    romalign 0x1000
    include "build/assets/objects/object_jya_iron/object_jya_iron.o"
    number 6
endseg

beginseg
    name "object_jya_door"
    compress
    romalign 0x1000
    include "build/assets/objects/object_jya_door/object_jya_door.o"
    number 6
endseg

beginseg
    name "object_spot01_objects2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot01_objects2/object_spot01_objects2.o"
    number 6
endseg

beginseg
    name "object_spot11_obj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot11_obj/object_spot11_obj.o"
    number 6
endseg

beginseg
    name "object_kibako2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_kibako2/object_kibako2.o"
    number 6
endseg

beginseg
    name "object_dns"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dns/object_dns.o"
    number 6
endseg

beginseg
    name "object_dnk"
    compress
    romalign 0x1000
    include "build/assets/objects/object_dnk/object_dnk.o"
    number 6
endseg

beginseg
    name "object_gi_fire"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_fire/object_gi_fire.o"
    number 6
endseg

beginseg
    name "object_gi_insect"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_insect/object_gi_insect.o"
    number 6
endseg

beginseg
    name "object_gi_butterfly"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_butterfly/object_gi_butterfly.o"
    number 6
endseg

beginseg
    name "object_gi_ghost"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_ghost/object_gi_ghost.o"
    number 6
endseg

beginseg
    name "object_gi_soul"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_soul/object_gi_soul.o"
    number 6
endseg

beginseg
    name "object_bowl"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bowl/object_bowl.o"
    number 6
endseg

beginseg
    name "object_po_field"
    compress
    romalign 0x1000
    include "build/assets/objects/object_po_field/object_po_field.o"
    number 6
endseg

beginseg
    name "object_demo_kekkai"
    compress
    romalign 0x1000
    include "build/assets/objects/object_demo_kekkai/object_demo_kekkai.o"
    number 6
endseg

beginseg
    name "object_efc_doughnut"
    compress
    romalign 0x1000
    include "build/assets/objects/object_efc_doughnut/object_efc_doughnut.o"
    number 6
endseg

beginseg
    name "object_gi_dekupouch"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_dekupouch/object_gi_dekupouch.o"
    number 6
endseg

beginseg
    name "object_ganon_anime1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ganon_anime1/object_ganon_anime1.o"
    number 6
endseg

beginseg
    name "object_ganon_anime2"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ganon_anime2/object_ganon_anime2.o"
    number 6
endseg

beginseg
    name "object_ganon_anime3"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ganon_anime3/object_ganon_anime3.o"
    number 6
endseg

beginseg
    name "object_gi_rupy"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_rupy/object_gi_rupy.o"
    number 6
endseg

beginseg
    name "object_spot01_matoya"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot01_matoya/object_spot01_matoya.o"
    number 6
endseg

beginseg
    name "object_spot01_matoyab"
    compress
    romalign 0x1000
    include "build/assets/objects/object_spot01_matoyab/object_spot01_matoyab.o"
    number 6
endseg

beginseg
    name "object_po_composer"
    compress
    romalign 0x1000
    include "build/assets/objects/object_po_composer/object_po_composer.o"
    number 6
endseg

beginseg
    name "object_mu"
    compress
    romalign 0x1000
    include "build/assets/objects/object_mu/object_mu.o"
    number 6
endseg

beginseg
    name "object_wf"
    compress
    romalign 0x1000
    include "build/assets/objects/object_wf/object_wf.o"
    number 6
endseg

beginseg
    name "object_skb"
    compress
    romalign 0x1000
    include "build/assets/objects/object_skb/object_skb.o"
    number 6
endseg

beginseg
    name "object_gj"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gj/object_gj.o"
    number 6
endseg

beginseg
    name "object_geff"
    compress
    romalign 0x1000
    include "build/assets/objects/object_geff/object_geff.o"
    number 6
endseg

beginseg
    name "object_haka_door"
    compress
    romalign 0x1000
    include "build/assets/objects/object_haka_door/object_haka_door.o"
    number 6
endseg

beginseg
    name "object_gs"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gs/object_gs.o"
    number 6
endseg

beginseg
    name "object_ps"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ps/object_ps.o"
    number 6
endseg

beginseg
    name "object_bwall"
    compress
    romalign 0x1000
    include "build/assets/objects/object_bwall/object_bwall.o"
    number 6
endseg

beginseg
    name "object_crow"
    compress
    romalign 0x1000
    include "build/assets/objects/object_crow/object_crow.o"
    number 6
endseg

beginseg
    name "object_cow"
    compress
    romalign 0x1000
    include "build/assets/objects/object_cow/object_cow.o"
    number 6
endseg

beginseg
    name "object_cob"
    compress
    romalign 0x1000
    include "build/assets/objects/object_cob/object_cob.o"
    number 6
endseg

beginseg
    name "object_gi_sword_1"
    compress
    romalign 0x1000
    include "build/assets/objects/object_gi_sword_1/object_gi_sword_1.o"
    number 6
endseg

beginseg
    name "object_door_killer"
    compress
    romalign 0x1000
    include "build/assets/objects/object_door_killer/object_door_killer.o"
    number 6
endseg

beginseg
    name "object_ouke_haka"
    compress
    romalign 0x1000
    include "build/assets/objects/object_ouke_haka/object_ouke_haka.o"
    number 6
endseg

beginseg
    name "object_timeblock"
    compress
    romalign 0x1000
    include "build/assets/objects/object_timeblock/object_timeblock.o"
    number 6
endseg

beginseg
    name "object_zl4"
    compress
    romalign 0x1000
    include "build/assets/objects/object_zl4/object_zl4.o"
    number 6
endseg

beginseg
    name "g_pn_01"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_01.o"
endseg

beginseg
    name "g_pn_02"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_02.o"
endseg

beginseg
    name "g_pn_03"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_03.o"
endseg

beginseg
    name "g_pn_04"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_04.o"
endseg

beginseg
    name "g_pn_05"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_05.o"
endseg

beginseg
    name "g_pn_06"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_06.o"
endseg

beginseg
    name "g_pn_07"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_07.o"
endseg

beginseg
    name "g_pn_08"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_08.o"
endseg

beginseg
    name "g_pn_09"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_09.o"
endseg

beginseg
    name "g_pn_10"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_10.o"
endseg

beginseg
    name "g_pn_11"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_11.o"
endseg

beginseg
    name "g_pn_12"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_12.o"
endseg

beginseg
    name "g_pn_13"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_13.o"
endseg

beginseg
    name "g_pn_14"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_14.o"
endseg

beginseg
    name "g_pn_15"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_15.o"
endseg

beginseg
    name "g_pn_16"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_16.o"
endseg

beginseg
    name "g_pn_17"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_17.o"
endseg

beginseg
    name "g_pn_18"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_18.o"
endseg

beginseg
    name "g_pn_19"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_19.o"
endseg

beginseg
    name "g_pn_20"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_20.o"
endseg

beginseg
    name "g_pn_21"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_21.o"
endseg

beginseg
    name "g_pn_22"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_22.o"
endseg

beginseg
    name "g_pn_23"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_23.o"
endseg

beginseg
    name "g_pn_24"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_24.o"
endseg

beginseg
    name "g_pn_25"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_25.o"
endseg

beginseg
    name "g_pn_26"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_26.o"
endseg

beginseg
    name "g_pn_27"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_27.o"
endseg

beginseg
    name "g_pn_28"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_28.o"
endseg

beginseg
    name "g_pn_29"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_29.o"
endseg

beginseg
    name "g_pn_30"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_30.o"
endseg

beginseg
    name "g_pn_31"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_31.o"
endseg

beginseg
    name "g_pn_32"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_32.o"
endseg

beginseg
    name "g_pn_33"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_33.o"
endseg

beginseg
    name "g_pn_34"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_34.o"
endseg

beginseg
    name "g_pn_35"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_35.o"
endseg

beginseg
    name "g_pn_36"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_36.o"
endseg

beginseg
    name "g_pn_37"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_37.o"
endseg

beginseg
    name "g_pn_38"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_38.o"
endseg

beginseg
    name "g_pn_39"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_39.o"
endseg

beginseg
    name "g_pn_40"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_40.o"
endseg

beginseg
    name "g_pn_41"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_41.o"
endseg

beginseg
    name "g_pn_42"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_42.o"
endseg

beginseg
    name "g_pn_43"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_43.o"
endseg

beginseg
    name "g_pn_44"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_44.o"
endseg

beginseg
    name "g_pn_45"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_45.o"
endseg

beginseg
    name "g_pn_46"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_46.o"
endseg

beginseg
    name "g_pn_47"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_47.o"
endseg

beginseg
    name "g_pn_48"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_48.o"
endseg

beginseg
    name "g_pn_49"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_49.o"
endseg

beginseg
    name "g_pn_50"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_50.o"
endseg

beginseg
    name "g_pn_51"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_51.o"
endseg

beginseg
    name "g_pn_52"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_52.o"
endseg

beginseg
    name "g_pn_53"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_53.o"
endseg

beginseg
    name "g_pn_54"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_54.o"
endseg

beginseg
    name "g_pn_55"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_55.o"
endseg

beginseg
    name "g_pn_56"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_56.o"
endseg

beginseg
    name "g_pn_57"
    compress
    romalign 0x1000
    include "build/assets/textures/place_title_cards/g_pn_57.o"
endseg

beginseg
    name "z_select_static"
    compress
    romalign 0x1000
    include "build/baserom/z_select_static.o"
endseg

beginseg
    name "nintendo_rogo_static"
    compress
    romalign 0x1000
    include "build/assets/textures/nintendo_rogo_static/nintendo_rogo_static.o"
    number 1
endseg

beginseg
    name "title_static"
    compress
    romalign 0x1000
    include "build/assets/textures/title_static/title_static.o"
    number 1
endseg

beginseg
    name "parameter_static"
    compress
    romalign 0x1000
    include "build/assets/textures/parameter_static/parameter_static.o"
    number 2
endseg

beginseg
    name "vr_fine0_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_fine0_static.o"
endseg

beginseg
    name "vr_fine0_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_fine0_pal_static.o"
endseg

beginseg
    name "vr_fine1_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_fine1_static.o"
endseg

beginseg
    name "vr_fine1_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_fine1_pal_static.o"
endseg

beginseg
    name "vr_fine2_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_fine2_static.o"
endseg

beginseg
    name "vr_fine2_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_fine2_pal_static.o"
endseg

beginseg
    name "vr_fine3_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_fine3_static.o"
endseg

beginseg
    name "vr_fine3_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_fine3_pal_static.o"
endseg

beginseg
    name "vr_cloud0_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_cloud0_static.o"
endseg

beginseg
    name "vr_cloud0_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_cloud0_pal_static.o"
endseg

beginseg
    name "vr_cloud1_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_cloud1_static.o"
endseg

beginseg
    name "vr_cloud1_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_cloud1_pal_static.o"
endseg

beginseg
    name "vr_cloud2_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_cloud2_static.o"
endseg

beginseg
    name "vr_cloud2_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_cloud2_pal_static.o"
endseg

beginseg
    name "vr_cloud3_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_cloud3_static.o"
endseg

beginseg
    name "vr_cloud3_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_cloud3_pal_static.o"
endseg

beginseg
    name "vr_holy0_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_holy0_static.o"
endseg

beginseg
    name "vr_holy0_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_holy0_pal_static.o"
endseg

beginseg
    name "vr_holy1_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_holy1_static.o"
endseg

beginseg
    name "vr_holy1_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_holy1_pal_static.o"
endseg

beginseg
    name "vr_MDVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_MDVR_static.o"
endseg

beginseg
    name "vr_MDVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_MDVR_pal_static.o"
endseg

beginseg
    name "vr_MNVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_MNVR_static.o"
endseg

beginseg
    name "vr_MNVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_MNVR_pal_static.o"
endseg

beginseg
    name "vr_RUVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_RUVR_static.o"
endseg

beginseg
    name "vr_RUVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_RUVR_pal_static.o"
endseg

beginseg
    name "vr_LHVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_LHVR_static.o"
endseg

beginseg
    name "vr_LHVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_LHVR_pal_static.o"
endseg

beginseg
    name "vr_KHVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_KHVR_static.o"
endseg

beginseg
    name "vr_KHVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_KHVR_pal_static.o"
endseg

beginseg
    name "vr_K3VR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_K3VR_static.o"
endseg

beginseg
    name "vr_K3VR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_K3VR_pal_static.o"
endseg

beginseg
    name "vr_K4VR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_K4VR_static.o"
endseg

beginseg
    name "vr_K4VR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_K4VR_pal_static.o"
endseg

beginseg
    name "vr_K5VR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_K5VR_static.o"
endseg

beginseg
    name "vr_K5VR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_K5VR_pal_static.o"
endseg

beginseg
    name "vr_SP1a_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_SP1a_static.o"
endseg

beginseg
    name "vr_SP1a_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_SP1a_pal_static.o"
endseg

beginseg
    name "vr_MLVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_MLVR_static.o"
endseg

beginseg
    name "vr_MLVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_MLVR_pal_static.o"
endseg

beginseg
    name "vr_KKRVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_KKRVR_static.o"
endseg

beginseg
    name "vr_KKRVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_KKRVR_pal_static.o"
endseg

beginseg
    name "vr_KR3VR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_KR3VR_static.o"
endseg

beginseg
    name "vr_KR3VR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_KR3VR_pal_static.o"
endseg

beginseg
    name "vr_IPVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_IPVR_static.o"
endseg

beginseg
    name "vr_IPVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_IPVR_pal_static.o"
endseg

beginseg
    name "vr_KSVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_KSVR_static.o"
endseg

beginseg
    name "vr_KSVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_KSVR_pal_static.o"
endseg

beginseg
    name "vr_GLVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_GLVR_static.o"
endseg

beginseg
    name "vr_GLVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_GLVR_pal_static.o"
endseg

beginseg
    name "vr_ZRVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_ZRVR_static.o"
endseg

beginseg
    name "vr_ZRVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_ZRVR_pal_static.o"
endseg

beginseg
    name "vr_DGVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_DGVR_static.o"
endseg

beginseg
    name "vr_DGVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_DGVR_pal_static.o"
endseg

beginseg
    name "vr_ALVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_ALVR_static.o"
endseg

beginseg
    name "vr_ALVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_ALVR_pal_static.o"
endseg

beginseg
    name "vr_NSVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_NSVR_static.o"
endseg

beginseg
    name "vr_NSVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_NSVR_pal_static.o"
endseg

beginseg
    name "vr_LBVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_LBVR_static.o"
endseg

beginseg
    name "vr_LBVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_LBVR_pal_static.o"
endseg

beginseg
    name "vr_TTVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_TTVR_static.o"
endseg

beginseg
    name "vr_TTVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_TTVR_pal_static.o"
endseg

beginseg
    name "vr_FCVR_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_FCVR_static.o"
endseg

beginseg
    name "vr_FCVR_pal_static"
    compress
    romalign 0x1000
    include "build/baserom/vr_FCVR_pal_static.o"
endseg

beginseg
    name "elf_message_field"
    compress
    romalign 0x1000
    include "build/baserom/elf_message_field.o"
endseg

beginseg
    name "elf_message_ydan"
    compress
    romalign 0x1000
    include "build/baserom/elf_message_ydan.o"
endseg

beginseg
    name "ydan_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_scene.o"
    number 2
endseg

beginseg
    name "ydan_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_0.o"
    number 3
endseg

beginseg
    name "ydan_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_1.o"
    number 3
endseg

beginseg
    name "ydan_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_2.o"
    number 3
endseg

beginseg
    name "ydan_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_3.o"
    number 3
endseg

beginseg
    name "ydan_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_4.o"
    number 3
endseg

beginseg
    name "ydan_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_5.o"
    number 3
endseg

beginseg
    name "ydan_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_6.o"
    number 3
endseg

beginseg
    name "ydan_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_7.o"
    number 3
endseg

beginseg
    name "ydan_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_8.o"
    number 3
endseg

beginseg
    name "ydan_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_9.o"
    number 3
endseg

beginseg
    name "ydan_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_10.o"
    number 3
endseg

beginseg
    name "ydan_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan/ydan_room_11.o"
    number 3
endseg

beginseg
    name "ddan_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_scene.o"
    number 2
endseg

beginseg
    name "ddan_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_0.o"
    number 3
endseg

beginseg
    name "ddan_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_1.o"
    number 3
endseg

beginseg
    name "ddan_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_2.o"
    number 3
endseg

beginseg
    name "ddan_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_3.o"
    number 3
endseg

beginseg
    name "ddan_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_4.o"
    number 3
endseg

beginseg
    name "ddan_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_5.o"
    number 3
endseg

beginseg
    name "ddan_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_6.o"
    number 3
endseg

beginseg
    name "ddan_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_7.o"
    number 3
endseg

beginseg
    name "ddan_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_8.o"
    number 3
endseg

beginseg
    name "ddan_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_9.o"
    number 3
endseg

beginseg
    name "ddan_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_10.o"
    number 3
endseg

beginseg
    name "ddan_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_11.o"
    number 3
endseg

beginseg
    name "ddan_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_12.o"
    number 3
endseg

beginseg
    name "ddan_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_13.o"
    number 3
endseg

beginseg
    name "ddan_room_14"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_14.o"
    number 3
endseg

beginseg
    name "ddan_room_15"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_15.o"
    number 3
endseg

beginseg
    name "ddan_room_16"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan/ddan_room_16.o"
    number 3
endseg

beginseg
    name "bdan_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_scene.o"
    number 2
endseg

beginseg
    name "bdan_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_0.o"
    number 3
endseg

beginseg
    name "bdan_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_1.o"
    number 3
endseg

beginseg
    name "bdan_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_2.o"
    number 3
endseg

beginseg
    name "bdan_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_3.o"
    number 3
endseg

beginseg
    name "bdan_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_4.o"
    number 3
endseg

beginseg
    name "bdan_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_5.o"
    number 3
endseg

beginseg
    name "bdan_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_6.o"
    number 3
endseg

beginseg
    name "bdan_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_7.o"
    number 3
endseg

beginseg
    name "bdan_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_8.o"
    number 3
endseg

beginseg
    name "bdan_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_9.o"
    number 3
endseg

beginseg
    name "bdan_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_10.o"
    number 3
endseg

beginseg
    name "bdan_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_11.o"
    number 3
endseg

beginseg
    name "bdan_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_12.o"
    number 3
endseg

beginseg
    name "bdan_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_13.o"
    number 3
endseg

beginseg
    name "bdan_room_14"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_14.o"
    number 3
endseg

beginseg
    name "bdan_room_15"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan/bdan_room_15.o"
    number 3
endseg

beginseg
    name "Bmori1_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_scene.o"
    number 2
endseg

beginseg
    name "Bmori1_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_0.o"
    number 3
endseg

beginseg
    name "Bmori1_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_1.o"
    number 3
endseg

beginseg
    name "Bmori1_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_2.o"
    number 3
endseg

beginseg
    name "Bmori1_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_3.o"
    number 3
endseg

beginseg
    name "Bmori1_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_4.o"
    number 3
endseg

beginseg
    name "Bmori1_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_5.o"
    number 3
endseg

beginseg
    name "Bmori1_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_6.o"
    number 3
endseg

beginseg
    name "Bmori1_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_7.o"
    number 3
endseg

beginseg
    name "Bmori1_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_8.o"
    number 3
endseg

beginseg
    name "Bmori1_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_9.o"
    number 3
endseg

beginseg
    name "Bmori1_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_10.o"
    number 3
endseg

beginseg
    name "Bmori1_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_11.o"
    number 3
endseg

beginseg
    name "Bmori1_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_12.o"
    number 3
endseg

beginseg
    name "Bmori1_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_13.o"
    number 3
endseg

beginseg
    name "Bmori1_room_14"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_14.o"
    number 3
endseg

beginseg
    name "Bmori1_room_15"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_15.o"
    number 3
endseg

beginseg
    name "Bmori1_room_16"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_16.o"
    number 3
endseg

beginseg
    name "Bmori1_room_17"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_17.o"
    number 3
endseg

beginseg
    name "Bmori1_room_18"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_18.o"
    number 3
endseg

beginseg
    name "Bmori1_room_19"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_19.o"
    number 3
endseg

beginseg
    name "Bmori1_room_20"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_20.o"
    number 3
endseg

beginseg
    name "Bmori1_room_21"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_21.o"
    number 3
endseg

beginseg
    name "Bmori1_room_22"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/Bmori1/Bmori1_room_22.o"
    number 3
endseg

beginseg
    name "HIDAN_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_scene.o"
    number 2
endseg

beginseg
    name "HIDAN_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_0.o"
    number 3
endseg

beginseg
    name "HIDAN_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_1.o"
    number 3
endseg

beginseg
    name "HIDAN_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_2.o"
    number 3
endseg

beginseg
    name "HIDAN_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_3.o"
    number 3
endseg

beginseg
    name "HIDAN_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_4.o"
    number 3
endseg

beginseg
    name "HIDAN_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_5.o"
    number 3
endseg

beginseg
    name "HIDAN_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_6.o"
    number 3
endseg

beginseg
    name "HIDAN_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_7.o"
    number 3
endseg

beginseg
    name "HIDAN_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_8.o"
    number 3
endseg

beginseg
    name "HIDAN_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_9.o"
    number 3
endseg

beginseg
    name "HIDAN_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_10.o"
    number 3
endseg

beginseg
    name "HIDAN_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_11.o"
    number 3
endseg

beginseg
    name "HIDAN_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_12.o"
    number 3
endseg

beginseg
    name "HIDAN_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_13.o"
    number 3
endseg

beginseg
    name "HIDAN_room_14"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_14.o"
    number 3
endseg

beginseg
    name "HIDAN_room_15"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_15.o"
    number 3
endseg

beginseg
    name "HIDAN_room_16"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_16.o"
    number 3
endseg

beginseg
    name "HIDAN_room_17"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_17.o"
    number 3
endseg

beginseg
    name "HIDAN_room_18"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_18.o"
    number 3
endseg

beginseg
    name "HIDAN_room_19"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_19.o"
    number 3
endseg

beginseg
    name "HIDAN_room_20"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_20.o"
    number 3
endseg

beginseg
    name "HIDAN_room_21"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_21.o"
    number 3
endseg

beginseg
    name "HIDAN_room_22"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_22.o"
    number 3
endseg

beginseg
    name "HIDAN_room_23"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_23.o"
    number 3
endseg

beginseg
    name "HIDAN_room_24"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_24.o"
    number 3
endseg

beginseg
    name "HIDAN_room_25"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_25.o"
    number 3
endseg

beginseg
    name "HIDAN_room_26"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HIDAN/HIDAN_room_26.o"
    number 3
endseg

beginseg
    name "MIZUsin_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_scene.o"
    number 2
endseg

beginseg
    name "MIZUsin_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_0.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_1.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_2.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_3.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_4.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_5.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_6.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_7.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_8.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_9.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_10.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_11.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_12.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_13.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_14"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_14.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_15"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_15.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_16"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_16.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_17"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_17.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_18"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_18.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_19"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_19.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_20"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_20.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_21"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_21.o"
    number 3
endseg

beginseg
    name "MIZUsin_room_22"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin/MIZUsin_room_22.o"
    number 3
endseg

beginseg
    name "jyasinzou_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_scene.o"
    number 2
endseg

beginseg
    name "jyasinzou_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_0.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_1.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_2.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_3.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_4.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_5.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_6.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_7.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_8.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_9.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_10.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_11.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_12.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_13.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_14"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_14.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_15"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_15.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_16"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_16.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_17"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_17.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_18"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_18.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_19"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_19.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_20"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_20.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_21"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_21.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_22"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_22.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_23"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_23.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_24"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_24.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_25"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_25.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_26"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_26.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_27"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_27.o"
    number 3
endseg

beginseg
    name "jyasinzou_room_28"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinzou/jyasinzou_room_28.o"
    number 3
endseg

beginseg
    name "HAKAdan_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_scene.o"
    number 2
endseg

beginseg
    name "HAKAdan_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_0.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_1.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_2.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_3.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_4.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_5.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_6.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_7.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_8.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_9.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_10.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_11.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_12.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_13.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_14"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_14.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_15"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_15.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_16"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_16.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_17"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_17.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_18"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_18.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_19"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_19.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_20"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_20.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_21"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_21.o"
    number 3
endseg

beginseg
    name "HAKAdan_room_22"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan/HAKAdan_room_22.o"
    number 3
endseg

beginseg
    name "HAKAdanCH_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdanCH/HAKAdanCH_scene.o"
    number 2
endseg

beginseg
    name "HAKAdanCH_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdanCH/HAKAdanCH_room_0.o"
    number 3
endseg

beginseg
    name "HAKAdanCH_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdanCH/HAKAdanCH_room_1.o"
    number 3
endseg

beginseg
    name "HAKAdanCH_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdanCH/HAKAdanCH_room_2.o"
    number 3
endseg

beginseg
    name "HAKAdanCH_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdanCH/HAKAdanCH_room_3.o"
    number 3
endseg

beginseg
    name "HAKAdanCH_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdanCH/HAKAdanCH_room_4.o"
    number 3
endseg

beginseg
    name "HAKAdanCH_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdanCH/HAKAdanCH_room_5.o"
    number 3
endseg

beginseg
    name "HAKAdanCH_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdanCH/HAKAdanCH_room_6.o"
    number 3
endseg

beginseg
    name "ice_doukutu_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_scene.o"
    number 2
endseg

beginseg
    name "ice_doukutu_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_0.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_1.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_2.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_3.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_4.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_5.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_6.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_7.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_8.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_9.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_10.o"
    number 3
endseg

beginseg
    name "ice_doukutu_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ice_doukutu/ice_doukutu_room_11.o"
    number 3
endseg

beginseg
    name "men_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_scene.o"
    number 2
endseg

beginseg
    name "men_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_0.o"
    number 3
endseg

beginseg
    name "men_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_1.o"
    number 3
endseg

beginseg
    name "men_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_2.o"
    number 3
endseg

beginseg
    name "men_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_3.o"
    number 3
endseg

beginseg
    name "men_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_4.o"
    number 3
endseg

beginseg
    name "men_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_5.o"
    number 3
endseg

beginseg
    name "men_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_6.o"
    number 3
endseg

beginseg
    name "men_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_7.o"
    number 3
endseg

beginseg
    name "men_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_8.o"
    number 3
endseg

beginseg
    name "men_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_9.o"
    number 3
endseg

beginseg
    name "men_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/men/men_room_10.o"
    number 3
endseg

beginseg
    name "ganontika_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_scene.o"
    number 2
endseg

beginseg
    name "ganontika_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_0.o"
    number 3
endseg

beginseg
    name "ganontika_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_1.o"
    number 3
endseg

beginseg
    name "ganontika_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_2.o"
    number 3
endseg

beginseg
    name "ganontika_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_3.o"
    number 3
endseg

beginseg
    name "ganontika_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_4.o"
    number 3
endseg

beginseg
    name "ganontika_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_5.o"
    number 3
endseg

beginseg
    name "ganontika_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_6.o"
    number 3
endseg

beginseg
    name "ganontika_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_7.o"
    number 3
endseg

beginseg
    name "ganontika_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_8.o"
    number 3
endseg

beginseg
    name "ganontika_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_9.o"
    number 3
endseg

beginseg
    name "ganontika_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_10.o"
    number 3
endseg

beginseg
    name "ganontika_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_11.o"
    number 3
endseg

beginseg
    name "ganontika_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_12.o"
    number 3
endseg

beginseg
    name "ganontika_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_13.o"
    number 3
endseg

beginseg
    name "ganontika_room_14"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_14.o"
    number 3
endseg

beginseg
    name "ganontika_room_15"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_15.o"
    number 3
endseg

beginseg
    name "ganontika_room_16"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_16.o"
    number 3
endseg

beginseg
    name "ganontika_room_17"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_17.o"
    number 3
endseg

beginseg
    name "ganontika_room_18"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_18.o"
    number 3
endseg

beginseg
    name "ganontika_room_19"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontika/ganontika_room_19.o"
    number 3
endseg

beginseg
    name "syotes_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/syotes/syotes_scene.o"
    number 2
endseg

beginseg
    name "syotes_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/syotes/syotes_room_0.o"
    number 3
endseg

beginseg
    name "syotes2_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/syotes2/syotes2_scene.o"
    number 2
endseg

beginseg
    name "syotes2_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/syotes2/syotes2_room_0.o"
    number 3
endseg

beginseg
    name "depth_test_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/depth_test/depth_test_scene.o"
    number 2
endseg

beginseg
    name "depth_test_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/depth_test/depth_test_room_0.o"
    number 3
endseg

beginseg
    name "spot00_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot00/spot00_scene.o"
    number 2
endseg

beginseg
    name "spot00_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot00/spot00_room_0.o"
    number 3
endseg

beginseg
    name "spot01_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot01/spot01_scene.o"
    number 2
endseg

beginseg
    name "spot01_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot01/spot01_room_0.o"
    number 3
endseg

beginseg
    name "spot02_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot02/spot02_scene.o"
    number 2
endseg

beginseg
    name "spot02_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot02/spot02_room_0.o"
    number 3
endseg

beginseg
    name "spot02_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot02/spot02_room_1.o"
    number 3
endseg

beginseg
    name "spot03_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot03/spot03_scene.o"
    number 2
endseg

beginseg
    name "spot03_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot03/spot03_room_0.o"
    number 3
endseg

beginseg
    name "spot03_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot03/spot03_room_1.o"
    number 3
endseg

beginseg
    name "spot04_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot04/spot04_scene.o"
    number 2
endseg

beginseg
    name "spot04_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot04/spot04_room_0.o"
    number 3
endseg

beginseg
    name "spot04_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot04/spot04_room_1.o"
    number 3
endseg

beginseg
    name "spot04_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot04/spot04_room_2.o"
    number 3
endseg

beginseg
    name "spot05_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot05/spot05_scene.o"
    number 2
endseg

beginseg
    name "spot05_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot05/spot05_room_0.o"
    number 3
endseg

beginseg
    name "spot06_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot06/spot06_scene.o"
    number 2
endseg

beginseg
    name "spot06_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot06/spot06_room_0.o"
    number 3
endseg

beginseg
    name "spot07_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot07/spot07_scene.o"
    number 2
endseg

beginseg
    name "spot07_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot07/spot07_room_0.o"
    number 3
endseg

beginseg
    name "spot07_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot07/spot07_room_1.o"
    number 3
endseg

beginseg
    name "spot08_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot08/spot08_scene.o"
    number 2
endseg

beginseg
    name "spot08_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot08/spot08_room_0.o"
    number 3
endseg

beginseg
    name "spot09_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot09/spot09_scene.o"
    number 2
endseg

beginseg
    name "spot09_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot09/spot09_room_0.o"
    number 3
endseg

beginseg
    name "spot10_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_scene.o"
    number 2
endseg

beginseg
    name "spot10_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_0.o"
    number 3
endseg

beginseg
    name "spot10_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_1.o"
    number 3
endseg

beginseg
    name "spot10_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_2.o"
    number 3
endseg

beginseg
    name "spot10_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_3.o"
    number 3
endseg

beginseg
    name "spot10_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_4.o"
    number 3
endseg

beginseg
    name "spot10_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_5.o"
    number 3
endseg

beginseg
    name "spot10_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_6.o"
    number 3
endseg

beginseg
    name "spot10_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_7.o"
    number 3
endseg

beginseg
    name "spot10_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_8.o"
    number 3
endseg

beginseg
    name "spot10_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot10/spot10_room_9.o"
    number 3
endseg

beginseg
    name "spot11_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot11/spot11_scene.o"
    number 2
endseg

beginseg
    name "spot11_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot11/spot11_room_0.o"
    number 3
endseg

beginseg
    name "spot12_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot12/spot12_scene.o"
    number 2
endseg

beginseg
    name "spot12_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot12/spot12_room_0.o"
    number 3
endseg

beginseg
    name "spot12_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot12/spot12_room_1.o"
    number 3
endseg

beginseg
    name "spot13_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot13/spot13_scene.o"
    number 2
endseg

beginseg
    name "spot13_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot13/spot13_room_0.o"
    number 3
endseg

beginseg
    name "spot13_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot13/spot13_room_1.o"
    number 3
endseg

beginseg
    name "spot15_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot15/spot15_scene.o"
    number 2
endseg

beginseg
    name "spot15_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot15/spot15_room_0.o"
    number 3
endseg

beginseg
    name "spot16_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot16/spot16_scene.o"
    number 2
endseg

beginseg
    name "spot16_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot16/spot16_room_0.o"
    number 3
endseg

beginseg
    name "spot17_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot17/spot17_scene.o"
    number 2
endseg

beginseg
    name "spot17_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot17/spot17_room_0.o"
    number 3
endseg

beginseg
    name "spot17_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot17/spot17_room_1.o"
    number 3
endseg

beginseg
    name "spot18_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot18/spot18_scene.o"
    number 2
endseg

beginseg
    name "spot18_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot18/spot18_room_0.o"
    number 3
endseg

beginseg
    name "spot18_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot18/spot18_room_1.o"
    number 3
endseg

beginseg
    name "spot18_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot18/spot18_room_2.o"
    number 3
endseg

beginseg
    name "spot18_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot18/spot18_room_3.o"
    number 3
endseg

beginseg
    name "market_day_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_day/market_day_scene.o"
    number 2
endseg

beginseg
    name "market_day_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_day/market_day_room_0.o"
    number 3
endseg

beginseg
    name "market_night_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_night/market_night_scene.o"
    number 2
endseg

beginseg
    name "market_night_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_night/market_night_room_0.o"
    number 3
endseg

beginseg
    name "testroom_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/testroom/testroom_scene.o"
    number 2
endseg

beginseg
    name "testroom_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/testroom/testroom_room_0.o"
    number 3
endseg

beginseg
    name "testroom_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/testroom/testroom_room_1.o"
    number 3
endseg

beginseg
    name "testroom_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/testroom/testroom_room_2.o"
    number 3
endseg

beginseg
    name "testroom_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/testroom/testroom_room_3.o"
    number 3
endseg

beginseg
    name "testroom_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/testroom/testroom_room_4.o"
    number 3
endseg

beginseg
    name "kenjyanoma_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kenjyanoma/kenjyanoma_scene.o"
    number 2
endseg

beginseg
    name "kenjyanoma_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kenjyanoma/kenjyanoma_room_0.o"
    number 3
endseg

beginseg
    name "tokinoma_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/tokinoma/tokinoma_scene.o"
    number 2
endseg

beginseg
    name "tokinoma_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/tokinoma/tokinoma_room_0.o"
    number 3
endseg

beginseg
    name "tokinoma_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/tokinoma/tokinoma_room_1.o"
    number 3
endseg

beginseg
    name "sutaru_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/sutaru/sutaru_scene.o"
    number 2
endseg

beginseg
    name "sutaru_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/sutaru/sutaru_room_0.o"
    number 3
endseg

beginseg
    name "link_home_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/link_home/link_home_scene.o"
    number 2
endseg

beginseg
    name "link_home_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/link_home/link_home_room_0.o"
    number 3
endseg

beginseg
    name "kokiri_shop_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/kokiri_shop/kokiri_shop_scene.o"
    number 2
endseg

beginseg
    name "kokiri_shop_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/kokiri_shop/kokiri_shop_room_0.o"
    number 3
endseg

beginseg
    name "kokiri_home_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kokiri_home/kokiri_home_scene.o"
    number 2
endseg

beginseg
    name "kokiri_home_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kokiri_home/kokiri_home_room_0.o"
    number 3
endseg

beginseg
    name "kakusiana_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_scene.o"
    number 2
endseg

beginseg
    name "kakusiana_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_0.o"
    number 3
endseg

beginseg
    name "kakusiana_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_1.o"
    number 3
endseg

beginseg
    name "kakusiana_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_2.o"
    number 3
endseg

beginseg
    name "kakusiana_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_3.o"
    number 3
endseg

beginseg
    name "kakusiana_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_4.o"
    number 3
endseg

beginseg
    name "kakusiana_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_5.o"
    number 3
endseg

beginseg
    name "kakusiana_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_6.o"
    number 3
endseg

beginseg
    name "kakusiana_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_7.o"
    number 3
endseg

beginseg
    name "kakusiana_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_8.o"
    number 3
endseg

beginseg
    name "kakusiana_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_9.o"
    number 3
endseg

beginseg
    name "kakusiana_room_10"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_10.o"
    number 3
endseg

beginseg
    name "kakusiana_room_11"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_11.o"
    number 3
endseg

beginseg
    name "kakusiana_room_12"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_12.o"
    number 3
endseg

beginseg
    name "kakusiana_room_13"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakusiana/kakusiana_room_13.o"
    number 3
endseg

beginseg
    name "entra_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/entra/entra_scene.o"
    number 2
endseg

beginseg
    name "entra_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/entra/entra_room_0.o"
    number 3
endseg

beginseg
    name "moribossroom_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/moribossroom/moribossroom_scene.o"
    number 2
endseg

beginseg
    name "moribossroom_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/moribossroom/moribossroom_room_0.o"
    number 3
endseg

beginseg
    name "moribossroom_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/moribossroom/moribossroom_room_1.o"
    number 3
endseg

beginseg
    name "syatekijyou_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/syatekijyou/syatekijyou_scene.o"
    number 2
endseg

beginseg
    name "syatekijyou_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/syatekijyou/syatekijyou_room_0.o"
    number 3
endseg

beginseg
    name "shop1_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/shop1/shop1_scene.o"
    number 2
endseg

beginseg
    name "shop1_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/shop1/shop1_room_0.o"
    number 3
endseg

beginseg
    name "hairal_niwa_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hairal_niwa/hairal_niwa_scene.o"
    number 2
endseg

beginseg
    name "hairal_niwa_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hairal_niwa/hairal_niwa_room_0.o"
    number 3
endseg

beginseg
    name "ganon_tou_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_tou/ganon_tou_scene.o"
    number 2
endseg

beginseg
    name "ganon_tou_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_tou/ganon_tou_room_0.o"
    number 3
endseg

beginseg
    name "sasatest_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/sasatest/sasatest_scene.o"
    number 2
endseg

beginseg
    name "sasatest_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/sasatest/sasatest_room_0.o"
    number 3
endseg

beginseg
    name "market_alley_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_alley/market_alley_scene.o"
    number 2
endseg

beginseg
    name "market_alley_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_alley/market_alley_room_0.o"
    number 3
endseg

beginseg
    name "spot20_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot20/spot20_scene.o"
    number 2
endseg

beginseg
    name "spot20_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/spot20/spot20_room_0.o"
    number 3
endseg

beginseg
    name "market_ruins_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_ruins/market_ruins_scene.o"
    number 2
endseg

beginseg
    name "market_ruins_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_ruins/market_ruins_room_0.o"
    number 3
endseg

beginseg
    name "entra_n_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/entra_n/entra_n_scene.o"
    number 2
endseg

beginseg
    name "entra_n_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/entra_n/entra_n_room_0.o"
    number 3
endseg

beginseg
    name "enrui_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/enrui/enrui_scene.o"
    number 2
endseg

beginseg
    name "enrui_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/enrui/enrui_room_0.o"
    number 3
endseg

beginseg
    name "market_alley_n_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_alley_n/market_alley_n_scene.o"
    number 2
endseg

beginseg
    name "market_alley_n_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/market_alley_n/market_alley_n_room_0.o"
    number 3
endseg

beginseg
    name "hiral_demo_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hiral_demo/hiral_demo_scene.o"
    number 2
endseg

beginseg
    name "hiral_demo_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hiral_demo/hiral_demo_room_0.o"
    number 3
endseg

beginseg
    name "kokiri_home3_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kokiri_home3/kokiri_home3_scene.o"
    number 2
endseg

beginseg
    name "kokiri_home3_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kokiri_home3/kokiri_home3_room_0.o"
    number 3
endseg

beginseg
    name "malon_stable_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/malon_stable/malon_stable_scene.o"
    number 2
endseg

beginseg
    name "malon_stable_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/malon_stable/malon_stable_room_0.o"
    number 3
endseg

beginseg
    name "kakariko_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kakariko/kakariko_scene.o"
    number 2
endseg

beginseg
    name "kakariko_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kakariko/kakariko_room_0.o"
    number 3
endseg

beginseg
    name "bdan_boss_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan_boss/bdan_boss_scene.o"
    number 2
endseg

beginseg
    name "bdan_boss_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan_boss/bdan_boss_room_0.o"
    number 3
endseg

beginseg
    name "bdan_boss_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/bdan_boss/bdan_boss_room_1.o"
    number 3
endseg

beginseg
    name "FIRE_bs_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/FIRE_bs/FIRE_bs_scene.o"
    number 2
endseg

beginseg
    name "FIRE_bs_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/FIRE_bs/FIRE_bs_room_0.o"
    number 3
endseg

beginseg
    name "FIRE_bs_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/FIRE_bs/FIRE_bs_room_1.o"
    number 3
endseg

beginseg
    name "hut_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hut/hut_scene.o"
    number 2
endseg

beginseg
    name "hut_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hut/hut_room_0.o"
    number 3
endseg

beginseg
    name "daiyousei_izumi_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/daiyousei_izumi/daiyousei_izumi_scene.o"
    number 2
endseg

beginseg
    name "daiyousei_izumi_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/daiyousei_izumi/daiyousei_izumi_room_0.o"
    number 3
endseg

beginseg
    name "hakaana_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hakaana/hakaana_scene.o"
    number 2
endseg

beginseg
    name "hakaana_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hakaana/hakaana_room_0.o"
    number 3
endseg

beginseg
    name "yousei_izumi_tate_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/yousei_izumi_tate/yousei_izumi_tate_scene.o"
    number 2
endseg

beginseg
    name "yousei_izumi_tate_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/yousei_izumi_tate/yousei_izumi_tate_room_0.o"
    number 3
endseg

beginseg
    name "yousei_izumi_yoko_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/yousei_izumi_yoko/yousei_izumi_yoko_scene.o"
    number 2
endseg

beginseg
    name "yousei_izumi_yoko_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/yousei_izumi_yoko/yousei_izumi_yoko_room_0.o"
    number 3
endseg

beginseg
    name "golon_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/golon/golon_scene.o"
    number 2
endseg

beginseg
    name "golon_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/golon/golon_room_0.o"
    number 3
endseg

beginseg
    name "zoora_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/zoora/zoora_scene.o"
    number 2
endseg

beginseg
    name "zoora_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/zoora/zoora_room_0.o"
    number 3
endseg

beginseg
    name "drag_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/drag/drag_scene.o"
    number 2
endseg

beginseg
    name "drag_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/drag/drag_room_0.o"
    number 3
endseg

beginseg
    name "alley_shop_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/alley_shop/alley_shop_scene.o"
    number 2
endseg

beginseg
    name "alley_shop_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/alley_shop/alley_shop_room_0.o"
    number 3
endseg

beginseg
    name "night_shop_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/night_shop/night_shop_scene.o"
    number 2
endseg

beginseg
    name "night_shop_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/night_shop/night_shop_room_0.o"
    number 3
endseg

beginseg
    name "impa_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/impa/impa_scene.o"
    number 2
endseg

beginseg
    name "impa_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/impa/impa_room_0.o"
    number 3
endseg

beginseg
    name "labo_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/labo/labo_scene.o"
    number 2
endseg

beginseg
    name "labo_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/labo/labo_room_0.o"
    number 3
endseg

beginseg
    name "tent_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/tent/tent_scene.o"
    number 2
endseg

beginseg
    name "tent_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/tent/tent_room_0.o"
    number 3
endseg

beginseg
    name "nakaniwa_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/nakaniwa/nakaniwa_scene.o"
    number 2
endseg

beginseg
    name "nakaniwa_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/nakaniwa/nakaniwa_room_0.o"
    number 3
endseg

beginseg
    name "ddan_boss_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan_boss/ddan_boss_scene.o"
    number 2
endseg

beginseg
    name "ddan_boss_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan_boss/ddan_boss_room_0.o"
    number 3
endseg

beginseg
    name "ddan_boss_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ddan_boss/ddan_boss_room_1.o"
    number 3
endseg

beginseg
    name "ydan_boss_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan_boss/ydan_boss_scene.o"
    number 2
endseg

beginseg
    name "ydan_boss_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan_boss/ydan_boss_room_0.o"
    number 3
endseg

beginseg
    name "ydan_boss_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ydan_boss/ydan_boss_room_1.o"
    number 3
endseg

beginseg
    name "HAKAdan_bs_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan_bs/HAKAdan_bs_scene.o"
    number 2
endseg

beginseg
    name "HAKAdan_bs_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan_bs/HAKAdan_bs_room_0.o"
    number 3
endseg

beginseg
    name "HAKAdan_bs_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/HAKAdan_bs/HAKAdan_bs_room_1.o"
    number 3
endseg

beginseg
    name "MIZUsin_bs_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin_bs/MIZUsin_bs_scene.o"
    number 2
endseg

beginseg
    name "MIZUsin_bs_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin_bs/MIZUsin_bs_room_0.o"
    number 3
endseg

beginseg
    name "MIZUsin_bs_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/MIZUsin_bs/MIZUsin_bs_room_1.o"
    number 3
endseg

beginseg
    name "ganon_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_scene.o"
    number 2
endseg

beginseg
    name "ganon_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_0.o"
    number 3
endseg

beginseg
    name "ganon_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_1.o"
    number 3
endseg

beginseg
    name "ganon_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_2.o"
    number 3
endseg

beginseg
    name "ganon_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_3.o"
    number 3
endseg

beginseg
    name "ganon_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_4.o"
    number 3
endseg

beginseg
    name "ganon_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_5.o"
    number 3
endseg

beginseg
    name "ganon_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_6.o"
    number 3
endseg

beginseg
    name "ganon_room_7"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_7.o"
    number 3
endseg

beginseg
    name "ganon_room_8"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_8.o"
    number 3
endseg

beginseg
    name "ganon_room_9"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon/ganon_room_9.o"
    number 3
endseg

beginseg
    name "ganon_boss_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_boss/ganon_boss_scene.o"
    number 2
endseg

beginseg
    name "ganon_boss_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_boss/ganon_boss_room_0.o"
    number 3
endseg

beginseg
    name "jyasinboss_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinboss/jyasinboss_scene.o"
    number 2
endseg

beginseg
    name "jyasinboss_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinboss/jyasinboss_room_0.o"
    number 3
endseg

beginseg
    name "jyasinboss_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinboss/jyasinboss_room_1.o"
    number 3
endseg

beginseg
    name "jyasinboss_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinboss/jyasinboss_room_2.o"
    number 3
endseg

beginseg
    name "jyasinboss_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/jyasinboss/jyasinboss_room_3.o"
    number 3
endseg

beginseg
    name "kokiri_home4_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kokiri_home4/kokiri_home4_scene.o"
    number 2
endseg

beginseg
    name "kokiri_home4_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kokiri_home4/kokiri_home4_room_0.o"
    number 3
endseg

beginseg
    name "kokiri_home5_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kokiri_home5/kokiri_home5_scene.o"
    number 2
endseg

beginseg
    name "kokiri_home5_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/kokiri_home5/kokiri_home5_room_0.o"
    number 3
endseg

beginseg
    name "ganon_final_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_final/ganon_final_scene.o"
    number 2
endseg

beginseg
    name "ganon_final_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_final/ganon_final_room_0.o"
    number 3
endseg

beginseg
    name "kakariko3_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakariko3/kakariko3_scene.o"
    number 2
endseg

beginseg
    name "kakariko3_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kakariko3/kakariko3_room_0.o"
    number 3
endseg

beginseg
    name "hairal_niwa2_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hairal_niwa2/hairal_niwa2_scene.o"
    number 2
endseg

beginseg
    name "hairal_niwa2_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hairal_niwa2/hairal_niwa2_room_0.o"
    number 3
endseg

beginseg
    name "hakasitarelay_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hakasitarelay/hakasitarelay_scene.o"
    number 2
endseg

beginseg
    name "hakasitarelay_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hakasitarelay/hakasitarelay_room_0.o"
    number 3
endseg

beginseg
    name "hakasitarelay_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hakasitarelay/hakasitarelay_room_1.o"
    number 3
endseg

beginseg
    name "hakasitarelay_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hakasitarelay/hakasitarelay_room_2.o"
    number 3
endseg

beginseg
    name "hakasitarelay_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hakasitarelay/hakasitarelay_room_3.o"
    number 3
endseg

beginseg
    name "hakasitarelay_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hakasitarelay/hakasitarelay_room_4.o"
    number 3
endseg

beginseg
    name "hakasitarelay_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hakasitarelay/hakasitarelay_room_5.o"
    number 3
endseg

beginseg
    name "hakasitarelay_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hakasitarelay/hakasitarelay_room_6.o"
    number 3
endseg

beginseg
    name "shrine_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/shrine/shrine_scene.o"
    number 2
endseg

beginseg
    name "shrine_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/shrine/shrine_room_0.o"
    number 3
endseg

beginseg
    name "turibori_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/turibori/turibori_scene.o"
    number 2
endseg

beginseg
    name "turibori_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/turibori/turibori_room_0.o"
    number 3
endseg

beginseg
    name "shrine_n_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/shrine_n/shrine_n_scene.o"
    number 2
endseg

beginseg
    name "shrine_n_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/shrine_n/shrine_n_room_0.o"
    number 3
endseg

beginseg
    name "shrine_r_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/shrine_r/shrine_r_scene.o"
    number 2
endseg

beginseg
    name "shrine_r_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/shrine_r/shrine_r_room_0.o"
    number 3
endseg

beginseg
    name "hakaana2_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hakaana2/hakaana2_scene.o"
    number 2
endseg

beginseg
    name "hakaana2_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hakaana2/hakaana2_room_0.o"
    number 3
endseg

beginseg
    name "gerudoway_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/gerudoway/gerudoway_scene.o"
    number 2
endseg

beginseg
    name "gerudoway_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/gerudoway/gerudoway_room_0.o"
    number 3
endseg

beginseg
    name "gerudoway_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/gerudoway/gerudoway_room_1.o"
    number 3
endseg

beginseg
    name "gerudoway_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/gerudoway/gerudoway_room_2.o"
    number 3
endseg

beginseg
    name "gerudoway_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/gerudoway/gerudoway_room_3.o"
    number 3
endseg

beginseg
    name "gerudoway_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/gerudoway/gerudoway_room_4.o"
    number 3
endseg

beginseg
    name "gerudoway_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/gerudoway/gerudoway_room_5.o"
    number 3
endseg

beginseg
    name "hairal_niwa_n_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hairal_niwa_n/hairal_niwa_n_scene.o"
    number 2
endseg

beginseg
    name "hairal_niwa_n_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hairal_niwa_n/hairal_niwa_n_room_0.o"
    number 3
endseg

beginseg
    name "bowling_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/bowling/bowling_scene.o"
    number 2
endseg

beginseg
    name "bowling_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/bowling/bowling_room_0.o"
    number 3
endseg

beginseg
    name "hakaana_ouke_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hakaana_ouke/hakaana_ouke_scene.o"
    number 2
endseg

beginseg
    name "hakaana_ouke_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hakaana_ouke/hakaana_ouke_room_0.o"
    number 3
endseg

beginseg
    name "hakaana_ouke_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hakaana_ouke/hakaana_ouke_room_1.o"
    number 3
endseg

beginseg
    name "hakaana_ouke_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/hakaana_ouke/hakaana_ouke_room_2.o"
    number 3
endseg

beginseg
    name "hylia_labo_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hylia_labo/hylia_labo_scene.o"
    number 2
endseg

beginseg
    name "hylia_labo_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/hylia_labo/hylia_labo_room_0.o"
    number 3
endseg

beginseg
    name "souko_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/souko/souko_scene.o"
    number 2
endseg

beginseg
    name "souko_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/souko/souko_room_0.o"
    number 3
endseg

beginseg
    name "souko_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/souko/souko_room_1.o"
    number 3
endseg

beginseg
    name "souko_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/overworld/souko/souko_room_2.o"
    number 3
endseg

beginseg
    name "miharigoya_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/miharigoya/miharigoya_scene.o"
    number 2
endseg

beginseg
    name "miharigoya_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/miharigoya/miharigoya_room_0.o"
    number 3
endseg

beginseg
    name "mahouya_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/mahouya/mahouya_scene.o"
    number 2
endseg

beginseg
    name "mahouya_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/mahouya/mahouya_room_0.o"
    number 3
endseg

beginseg
    name "takaraya_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/takaraya/takaraya_scene.o"
    number 2
endseg

beginseg
    name "takaraya_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/takaraya/takaraya_room_0.o"
    number 3
endseg

beginseg
    name "takaraya_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/takaraya/takaraya_room_1.o"
    number 3
endseg

beginseg
    name "takaraya_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/takaraya/takaraya_room_2.o"
    number 3
endseg

beginseg
    name "takaraya_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/takaraya/takaraya_room_3.o"
    number 3
endseg

beginseg
    name "takaraya_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/takaraya/takaraya_room_4.o"
    number 3
endseg

beginseg
    name "takaraya_room_5"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/takaraya/takaraya_room_5.o"
    number 3
endseg

beginseg
    name "takaraya_room_6"
    compress
    romalign 0x1000
    include "build/assets/scenes/indoors/takaraya/takaraya_room_6.o"
    number 3
endseg

beginseg
    name "ganon_sonogo_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_sonogo/ganon_sonogo_scene.o"
    number 2
endseg

beginseg
    name "ganon_sonogo_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_sonogo/ganon_sonogo_room_0.o"
    number 3
endseg

beginseg
    name "ganon_sonogo_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_sonogo/ganon_sonogo_room_1.o"
    number 3
endseg

beginseg
    name "ganon_sonogo_room_2"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_sonogo/ganon_sonogo_room_2.o"
    number 3
endseg

beginseg
    name "ganon_sonogo_room_3"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_sonogo/ganon_sonogo_room_3.o"
    number 3
endseg

beginseg
    name "ganon_sonogo_room_4"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_sonogo/ganon_sonogo_room_4.o"
    number 3
endseg

beginseg
    name "ganon_demo_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_demo/ganon_demo_scene.o"
    number 2
endseg

beginseg
    name "ganon_demo_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganon_demo/ganon_demo_room_0.o"
    number 3
endseg

beginseg
    name "besitu_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/besitu/besitu_scene.o"
    number 2
endseg

beginseg
    name "besitu_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/besitu/besitu_room_0.o"
    number 3
endseg

beginseg
    name "face_shop_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/face_shop/face_shop_scene.o"
    number 2
endseg

beginseg
    name "face_shop_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/shops/face_shop/face_shop_room_0.o"
    number 3
endseg

beginseg
    name "kinsuta_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kinsuta/kinsuta_scene.o"
    number 2
endseg

beginseg
    name "kinsuta_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/misc/kinsuta/kinsuta_room_0.o"
    number 3
endseg

beginseg
    name "ganontikasonogo_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontikasonogo/ganontikasonogo_scene.o"
    number 2
endseg

beginseg
    name "ganontikasonogo_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontikasonogo/ganontikasonogo_room_0.o"
    number 3
endseg

beginseg
    name "ganontikasonogo_room_1"
    compress
    romalign 0x1000
    include "build/assets/scenes/dungeons/ganontikasonogo/ganontikasonogo_room_1.o"
    number 3
endseg

beginseg
    name "test01_scene"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/test01/test01_scene.o"
    number 2
endseg

beginseg
    name "test01_room_0"
    compress
    romalign 0x1000
    include "build/assets/scenes/test_levels/test01/test01_room_0.o"
    number 3
endseg

beginseg
    name "bump_texture_static"
    compress
    romalign 0x1000
    include "build/baserom/bump_texture_static.o"
endseg

beginseg
    name "anime_model_1_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_model_1_static.o"
endseg

beginseg
    name "anime_model_2_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_model_2_static.o"
endseg

beginseg
    name "anime_model_3_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_model_3_static.o"
endseg

beginseg
    name "anime_model_4_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_model_4_static.o"
endseg

beginseg
    name "anime_model_5_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_model_5_static.o"
endseg

beginseg
    name "anime_model_6_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_model_6_static.o"
endseg

beginseg
    name "anime_texture_1_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_texture_1_static.o"
endseg

beginseg
    name "anime_texture_2_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_texture_2_static.o"
endseg

beginseg
    name "anime_texture_3_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_texture_3_static.o"
endseg

beginseg
    name "anime_texture_4_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_texture_4_static.o"
endseg

beginseg
    name "anime_texture_5_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_texture_5_static.o"
endseg

beginseg
    name "anime_texture_6_static"
    compress
    romalign 0x1000
    include "build/baserom/anime_texture_6_static.o"
endseg

beginseg
    name "softsprite_matrix_static"
    compress
    romalign 0x1000
    include "build/baserom/softsprite_matrix_static.o"
endseg
