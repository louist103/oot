.rdata
glabel D_8088F250
    .asciz "../z_bg_hidan_sima.c"
    .balign 4

glabel D_8088F268
    .asciz "../z_bg_hidan_sima.c"
    .balign 4

glabel D_8088F280
    .asciz "../z_bg_hidan_sima.c"
    .balign 4

.text
glabel BgHidanSima_Draw
/* 00C38 8088F008 27BDFFB0 */  addiu   $sp, $sp, 0xFFB0           ## $sp = FFFFFFB0
/* 00C3C 8088F00C AFBF001C */  sw      $ra, 0x001C($sp)           
/* 00C40 8088F010 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 00C44 8088F014 AFA40050 */  sw      $a0, 0x0050($sp)           
/* 00C48 8088F018 AFA50054 */  sw      $a1, 0x0054($sp)           
/* 00C4C 8088F01C 8CA50000 */  lw      $a1, 0x0000($a1)           ## 00000000
/* 00C50 8088F020 3C068089 */  lui     $a2, %hi(D_8088F250)       ## $a2 = 80890000
/* 00C54 8088F024 24C6F250 */  addiu   $a2, $a2, %lo(D_8088F250)  ## $a2 = 8088F250
/* 00C58 8088F028 27A40038 */  addiu   $a0, $sp, 0x0038           ## $a0 = FFFFFFE8
/* 00C5C 8088F02C 24070281 */  addiu   $a3, $zero, 0x0281         ## $a3 = 00000281
/* 00C60 8088F030 0C031AB1 */  jal     Graph_OpenDisps              
/* 00C64 8088F034 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 00C68 8088F038 8FAF0054 */  lw      $t7, 0x0054($sp)           
/* 00C6C 8088F03C 0C024F46 */  jal     func_80093D18              
/* 00C70 8088F040 8DE40000 */  lw      $a0, 0x0000($t7)           ## 00000000
/* 00C74 8088F044 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 00C78 8088F048 3C19DA38 */  lui     $t9, 0xDA38                ## $t9 = DA380000
/* 00C7C 8088F04C 37390003 */  ori     $t9, $t9, 0x0003           ## $t9 = DA380003
/* 00C80 8088F050 24580008 */  addiu   $t8, $v0, 0x0008           ## $t8 = 00000008
/* 00C84 8088F054 AE1802C0 */  sw      $t8, 0x02C0($s0)           ## 000002C0
/* 00C88 8088F058 AC590000 */  sw      $t9, 0x0000($v0)           ## 00000000
/* 00C8C 8088F05C 8FA80054 */  lw      $t0, 0x0054($sp)           
/* 00C90 8088F060 3C058089 */  lui     $a1, %hi(D_8088F268)       ## $a1 = 80890000
/* 00C94 8088F064 24A5F268 */  addiu   $a1, $a1, %lo(D_8088F268)  ## $a1 = 8088F268
/* 00C98 8088F068 8D040000 */  lw      $a0, 0x0000($t0)           ## 00000000
/* 00C9C 8088F06C 24060285 */  addiu   $a2, $zero, 0x0285         ## $a2 = 00000285
/* 00CA0 8088F070 0C0346A2 */  jal     Matrix_NewMtx              
/* 00CA4 8088F074 AFA20034 */  sw      $v0, 0x0034($sp)           
/* 00CA8 8088F078 8FA30034 */  lw      $v1, 0x0034($sp)           
/* 00CAC 8088F07C 3C0BDE00 */  lui     $t3, 0xDE00                ## $t3 = DE000000
/* 00CB0 8088F080 3C0EDE00 */  lui     $t6, 0xDE00                ## $t6 = DE000000
/* 00CB4 8088F084 AC620004 */  sw      $v0, 0x0004($v1)           ## 00000004
/* 00CB8 8088F088 8FA60050 */  lw      $a2, 0x0050($sp)           
/* 00CBC 8088F08C 84C9001C */  lh      $t1, 0x001C($a2)           ## 0000001C
/* 00CC0 8088F090 5520000A */  bnel    $t1, $zero, .L8088F0BC     
/* 00CC4 8088F094 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 00CC8 8088F098 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 00CCC 8088F09C 3C0C0601 */  lui     $t4, 0x0601                ## $t4 = 06010000
/* 00CD0 8088F0A0 258CC338 */  addiu   $t4, $t4, 0xC338           ## $t4 = 0600C338
/* 00CD4 8088F0A4 244A0008 */  addiu   $t2, $v0, 0x0008           ## $t2 = 00000008
/* 00CD8 8088F0A8 AE0A02C0 */  sw      $t2, 0x02C0($s0)           ## 000002C0
/* 00CDC 8088F0AC AC4C0004 */  sw      $t4, 0x0004($v0)           ## 00000004
/* 00CE0 8088F0B0 10000025 */  beq     $zero, $zero, .L8088F148   
/* 00CE4 8088F0B4 AC4B0000 */  sw      $t3, 0x0000($v0)           ## 00000000
/* 00CE8 8088F0B8 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
.L8088F0BC:
/* 00CEC 8088F0BC 3C0F0601 */  lui     $t7, 0x0601                ## $t7 = 06010000
/* 00CF0 8088F0C0 25EFC470 */  addiu   $t7, $t7, 0xC470           ## $t7 = 0600C470
/* 00CF4 8088F0C4 244D0008 */  addiu   $t5, $v0, 0x0008           ## $t5 = 00000008
/* 00CF8 8088F0C8 AE0D02C0 */  sw      $t5, 0x02C0($s0)           ## 000002C0
/* 00CFC 8088F0CC AC4F0004 */  sw      $t7, 0x0004($v0)           ## 00000004
/* 00D00 8088F0D0 AC4E0000 */  sw      $t6, 0x0000($v0)           ## 00000000
/* 00D04 8088F0D4 8CD90164 */  lw      $t9, 0x0164($a2)           ## 00000164
/* 00D08 8088F0D8 3C188089 */  lui     $t8, %hi(func_8088E7A8)    ## $t8 = 80890000
/* 00D0C 8088F0DC 2718E7A8 */  addiu   $t8, $t8, %lo(func_8088E7A8) ## $t8 = 8088E7A8
/* 00D10 8088F0E0 17190019 */  bne     $t8, $t9, .L8088F148       
/* 00D14 8088F0E4 24050014 */  addiu   $a1, $zero, 0x0014         ## $a1 = 00000014
/* 00D18 8088F0E8 0C024DDD */  jal     Gfx_CallSetupDL              
/* 00D1C 8088F0EC 8E0402D0 */  lw      $a0, 0x02D0($s0)           ## 000002D0
/* 00D20 8088F0F0 AE0202D0 */  sw      $v0, 0x02D0($s0)           ## 000002D0
/* 00D24 8088F0F4 24480008 */  addiu   $t0, $v0, 0x0008           ## $t0 = 00000008
/* 00D28 8088F0F8 AE0802D0 */  sw      $t0, 0x02D0($s0)           ## 000002D0
/* 00D2C 8088F0FC 3C09FA00 */  lui     $t1, 0xFA00                ## $t1 = FA000000
/* 00D30 8088F100 3C0AFFFF */  lui     $t2, 0xFFFF                ## $t2 = FFFF0000
/* 00D34 8088F104 354A0096 */  ori     $t2, $t2, 0x0096           ## $t2 = FFFF0096
/* 00D38 8088F108 35290001 */  ori     $t1, $t1, 0x0001           ## $t1 = FA000001
/* 00D3C 8088F10C AC490000 */  sw      $t1, 0x0000($v0)           ## 00000000
/* 00D40 8088F110 AC4A0004 */  sw      $t2, 0x0004($v0)           ## 00000004
/* 00D44 8088F114 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 00D48 8088F118 3C0DFF00 */  lui     $t5, 0xFF00                ## $t5 = FF000000
/* 00D4C 8088F11C 35AD00FF */  ori     $t5, $t5, 0x00FF           ## $t5 = FF0000FF
/* 00D50 8088F120 244B0008 */  addiu   $t3, $v0, 0x0008           ## $t3 = 00000008
/* 00D54 8088F124 AE0B02D0 */  sw      $t3, 0x02D0($s0)           ## 000002D0
/* 00D58 8088F128 3C0CFB00 */  lui     $t4, 0xFB00                ## $t4 = FB000000
/* 00D5C 8088F12C AC4C0000 */  sw      $t4, 0x0000($v0)           ## 00000000
/* 00D60 8088F130 AC4D0004 */  sw      $t5, 0x0004($v0)           ## 00000004
/* 00D64 8088F134 8E0602D0 */  lw      $a2, 0x02D0($s0)           ## 000002D0
/* 00D68 8088F138 8FA50050 */  lw      $a1, 0x0050($sp)           
/* 00D6C 8088F13C 0C223AD5 */  jal     func_8088EB54              
/* 00D70 8088F140 8FA40054 */  lw      $a0, 0x0054($sp)           
/* 00D74 8088F144 AE0202D0 */  sw      $v0, 0x02D0($s0)           ## 000002D0
.L8088F148:
/* 00D78 8088F148 8FAE0054 */  lw      $t6, 0x0054($sp)           
/* 00D7C 8088F14C 3C068089 */  lui     $a2, %hi(D_8088F280)       ## $a2 = 80890000
/* 00D80 8088F150 24C6F280 */  addiu   $a2, $a2, %lo(D_8088F280)  ## $a2 = 8088F280
/* 00D84 8088F154 27A40038 */  addiu   $a0, $sp, 0x0038           ## $a0 = FFFFFFE8
/* 00D88 8088F158 2407029C */  addiu   $a3, $zero, 0x029C         ## $a3 = 0000029C
/* 00D8C 8088F15C 0C031AD5 */  jal     Graph_CloseDisps              
/* 00D90 8088F160 8DC50000 */  lw      $a1, 0x0000($t6)           ## 00000000
/* 00D94 8088F164 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00D98 8088F168 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 00D9C 8088F16C 27BD0050 */  addiu   $sp, $sp, 0x0050           ## $sp = 00000000
/* 00DA0 8088F170 03E00008 */  jr      $ra                        
/* 00DA4 8088F174 00000000 */  nop
/* 00DA8 8088F178 00000000 */  nop
/* 00DAC 8088F17C 00000000 */  nop