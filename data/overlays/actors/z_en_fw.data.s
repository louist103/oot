.include "macro.inc"

 # assembler directives
 .set noat      # allow manual use of $at
 .set noreorder # don't insert nops after branches
 .set gp=64     # allow use of 64-bit general purpose registers

.section .data

.balign 16

glabel En_Fw_InitVars
 .word 0x00AB0500, 0x00000215, 0x009E0000, 0x00000700
.word EnFw_Init
.word EnFw_Destroy
.word EnFw_Update
.word EnFw_Draw
glabel D_80A1FB60
 .word 0x00000000, 0x00000000, 0x00040000, 0xFFCFFFFE, 0x00000000, 0x00050100, 0x020004B0, 0x00000000, 0x00100064
glabel D_80A1FB84
 .word 0x06110939, 0x10000000, 0x00000001
.word D_80A1FB60
glabel D_80A1FB94
 .word 0x08000002, 0x00190019, 0xFF000000
glabel D_80A1FBA0
 .word 0x06006CF8, 0x00000000, 0x00000000, 0xBF800000, 0x03000000, 0x00000000, 0x06007CD0, 0x3F800000, 0x00000000, 0xBF800000, 0x03000000, 0xC1000000, 0x06007DC8, 0x3F800000, 0x00000000, 0xBF800000, 0x01000000, 0xC1000000
glabel D_80A1FBE8
 .word 0x00000000, 0x00000000, 0x00000000
glabel D_80A1FBF4
 .word 0x00000000, 0x00000000, 0x00000000
glabel D_80A1FC00
 .word 0x00000000, 0x00000000, 0x00000000
glabel D_80A1FC0C
 .word 0x00000000, 0x00000000, 0x00000000
glabel D_80A1FC18
 .word 0x040539B0, 0x040535B0, 0x040531B0, 0x04052DB0, 0x040529B0, 0x040525B0, 0x040521B0, 0x04051DB0, 0x00000000, 0x00000000

