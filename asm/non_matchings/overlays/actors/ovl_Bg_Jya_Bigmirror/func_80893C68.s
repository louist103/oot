.rdata
glabel D_80894030
    .asciz "../z_bg_jya_bigmirror.c"
    .balign 4

glabel D_80894048
    .asciz "../z_bg_jya_bigmirror.c"
    .balign 4

glabel D_80894060
    .asciz "../z_bg_jya_bigmirror.c"
    .balign 4

glabel D_80894078
    .asciz "../z_bg_jya_bigmirror.c"
    .balign 4

.late_rodata
glabel D_80894090
    .float 0.1

glabel D_80894094
 .word 0xBA4CCCCD
glabel D_80894098
 .word 0x3FB1F0A4
glabel D_8089409C
    .float 0.1

.text
glabel func_80893C68
/* 00588 80893C68 27BDFFB0 */  addiu   $sp, $sp, 0xFFB0           ## $sp = FFFFFFB0
/* 0058C 80893C6C AFBF001C */  sw      $ra, 0x001C($sp)           
/* 00590 80893C70 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 00594 80893C74 AFA40050 */  sw      $a0, 0x0050($sp)           
/* 00598 80893C78 AFA50054 */  sw      $a1, 0x0054($sp)           
/* 0059C 80893C7C 8CA50000 */  lw      $a1, 0x0000($a1)           ## 00000000
/* 005A0 80893C80 3C068089 */  lui     $a2, %hi(D_80894030)       ## $a2 = 80890000
/* 005A4 80893C84 24C64030 */  addiu   $a2, $a2, %lo(D_80894030)  ## $a2 = 80894030
/* 005A8 80893C88 27A40034 */  addiu   $a0, $sp, 0x0034           ## $a0 = FFFFFFE4
/* 005AC 80893C8C 240701B3 */  addiu   $a3, $zero, 0x01B3         ## $a3 = 000001B3
/* 005B0 80893C90 0C031AB1 */  jal     Graph_OpenDisps              
/* 005B4 80893C94 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 005B8 80893C98 8FAF0054 */  lw      $t7, 0x0054($sp)           
/* 005BC 80893C9C 0C024F61 */  jal     func_80093D84              
/* 005C0 80893CA0 8DE40000 */  lw      $a0, 0x0000($t7)           ## 00000000
/* 005C4 80893CA4 8FA40054 */  lw      $a0, 0x0054($sp)           
/* 005C8 80893CA8 24050157 */  addiu   $a1, $zero, 0x0157         ## $a1 = 00000157
/* 005CC 80893CAC 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 005D0 80893CB0 0C00CB0F */  jal     Actor_Find
              
/* 005D4 80893CB4 24841C24 */  addiu   $a0, $a0, 0x1C24           ## $a0 = 00001C24
/* 005D8 80893CB8 10400004 */  beq     $v0, $zero, .L80893CCC     
/* 005DC 80893CBC 00404025 */  or      $t0, $v0, $zero            ## $t0 = 00000000
/* 005E0 80893CC0 8FA30050 */  lw      $v1, 0x0050($sp)           
/* 005E4 80893CC4 C4440028 */  lwc1    $f4, 0x0028($v0)           ## 00000028
/* 005E8 80893CC8 E4640170 */  swc1    $f4, 0x0170($v1)           ## 00000170
.L80893CCC:
/* 005EC 80893CCC 8FA30050 */  lw      $v1, 0x0050($sp)           
/* 005F0 80893CD0 3C014220 */  lui     $at, 0x4220                ## $at = 42200000
/* 005F4 80893CD4 44814000 */  mtc1    $at, $f8                   ## $f8 = 40.00
/* 005F8 80893CD8 C4660028 */  lwc1    $f6, 0x0028($v1)           ## 00000028
/* 005FC 80893CDC C46C0024 */  lwc1    $f12, 0x0024($v1)          ## 00000024
/* 00600 80893CE0 8C66002C */  lw      $a2, 0x002C($v1)           ## 0000002C
/* 00604 80893CE4 AFA80048 */  sw      $t0, 0x0048($sp)           
/* 00608 80893CE8 246700B4 */  addiu   $a3, $v1, 0x00B4           ## $a3 = 000000B4
/* 0060C 80893CEC 0C0345A5 */  jal     func_800D1694              
/* 00610 80893CF0 46083380 */  add.s   $f14, $f6, $f8             
/* 00614 80893CF4 8FB80050 */  lw      $t8, 0x0050($sp)           
/* 00618 80893CF8 3C018089 */  lui     $at, %hi(D_80894090)       ## $at = 80890000
/* 0061C 80893CFC C42C4090 */  lwc1    $f12, %lo(D_80894090)($at) 
/* 00620 80893D00 3C018089 */  lui     $at, %hi(D_80894094)       ## $at = 80890000
/* 00624 80893D04 C4304094 */  lwc1    $f16, %lo(D_80894094)($at) 
/* 00628 80893D08 C70A0170 */  lwc1    $f10, 0x0170($t8)          ## 00000170
/* 0062C 80893D0C 3C018089 */  lui     $at, %hi(D_80894098)       ## $at = 80890000
/* 00630 80893D10 C4244098 */  lwc1    $f4, %lo(D_80894098)($at)  
/* 00634 80893D14 46105482 */  mul.s   $f18, $f10, $f16           
/* 00638 80893D18 44066000 */  mfc1    $a2, $f12                  
/* 0063C 80893D1C 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 00640 80893D20 0C0342A3 */  jal     Matrix_Scale              
/* 00644 80893D24 46049380 */  add.s   $f14, $f18, $f4            
/* 00648 80893D28 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 0064C 80893D2C 8FA80048 */  lw      $t0, 0x0048($sp)           
/* 00650 80893D30 3C09DA38 */  lui     $t1, 0xDA38                ## $t1 = DA380000
/* 00654 80893D34 24590008 */  addiu   $t9, $v0, 0x0008           ## $t9 = 00000008
/* 00658 80893D38 AE1902D0 */  sw      $t9, 0x02D0($s0)           ## 000002D0
/* 0065C 80893D3C 35290003 */  ori     $t1, $t1, 0x0003           ## $t1 = DA380003
/* 00660 80893D40 AC490000 */  sw      $t1, 0x0000($v0)           ## 00000000
/* 00664 80893D44 8FAA0054 */  lw      $t2, 0x0054($sp)           
/* 00668 80893D48 3C058089 */  lui     $a1, %hi(D_80894048)       ## $a1 = 80890000
/* 0066C 80893D4C 24A54048 */  addiu   $a1, $a1, %lo(D_80894048)  ## $a1 = 80894048
/* 00670 80893D50 8D440000 */  lw      $a0, 0x0000($t2)           ## 00000000
/* 00674 80893D54 240601C9 */  addiu   $a2, $zero, 0x01C9         ## $a2 = 000001C9
/* 00678 80893D58 AFA20030 */  sw      $v0, 0x0030($sp)           
/* 0067C 80893D5C 0C0346A2 */  jal     Matrix_NewMtx              
/* 00680 80893D60 AFA80048 */  sw      $t0, 0x0048($sp)           
/* 00684 80893D64 8FA30030 */  lw      $v1, 0x0030($sp)           
/* 00688 80893D68 8FA80048 */  lw      $t0, 0x0048($sp)           
/* 0068C 80893D6C 3C0D0601 */  lui     $t5, 0x0601                ## $t5 = 06010000
/* 00690 80893D70 AC620004 */  sw      $v0, 0x0004($v1)           ## 00000004
/* 00694 80893D74 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 00698 80893D78 25ADBC70 */  addiu   $t5, $t5, 0xBC70           ## $t5 = 0600BC70
/* 0069C 80893D7C 3C0CDE00 */  lui     $t4, 0xDE00                ## $t4 = DE000000
/* 006A0 80893D80 244B0008 */  addiu   $t3, $v0, 0x0008           ## $t3 = 00000008
/* 006A4 80893D84 AE0B02D0 */  sw      $t3, 0x02D0($s0)           ## 000002D0
/* 006A8 80893D88 AC4D0004 */  sw      $t5, 0x0004($v0)           ## 00000004
/* 006AC 80893D8C 11000024 */  beq     $t0, $zero, .L80893E20     
/* 006B0 80893D90 AC4C0000 */  sw      $t4, 0x0000($v0)           ## 00000000
/* 006B4 80893D94 3C078089 */  lui     $a3, %hi(D_80893F4C)       ## $a3 = 80890000
/* 006B8 80893D98 24E73F4C */  addiu   $a3, $a3, %lo(D_80893F4C)  ## $a3 = 80893F4C
/* 006BC 80893D9C C50C0024 */  lwc1    $f12, 0x0024($t0)          ## 00000024
/* 006C0 80893DA0 C50E0028 */  lwc1    $f14, 0x0028($t0)          ## 00000028
/* 006C4 80893DA4 0C0345A5 */  jal     func_800D1694              
/* 006C8 80893DA8 8D06002C */  lw      $a2, 0x002C($t0)           ## 0000002C
/* 006CC 80893DAC 3C018089 */  lui     $at, %hi(D_8089409C)       ## $at = 80890000
/* 006D0 80893DB0 C42C409C */  lwc1    $f12, %lo(D_8089409C)($at) 
/* 006D4 80893DB4 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 006D8 80893DB8 44066000 */  mfc1    $a2, $f12                  
/* 006DC 80893DBC 0C0342A3 */  jal     Matrix_Scale              
/* 006E0 80893DC0 46006386 */  mov.s   $f14, $f12                 
/* 006E4 80893DC4 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 006E8 80893DC8 3C0FDA38 */  lui     $t7, 0xDA38                ## $t7 = DA380000
/* 006EC 80893DCC 35EF0003 */  ori     $t7, $t7, 0x0003           ## $t7 = DA380003
/* 006F0 80893DD0 244E0008 */  addiu   $t6, $v0, 0x0008           ## $t6 = 00000008
/* 006F4 80893DD4 AE0E02D0 */  sw      $t6, 0x02D0($s0)           ## 000002D0
/* 006F8 80893DD8 AC4F0000 */  sw      $t7, 0x0000($v0)           ## 00000000
/* 006FC 80893DDC 8FB80054 */  lw      $t8, 0x0054($sp)           
/* 00700 80893DE0 3C058089 */  lui     $a1, %hi(D_80894060)       ## $a1 = 80890000
/* 00704 80893DE4 24A54060 */  addiu   $a1, $a1, %lo(D_80894060)  ## $a1 = 80894060
/* 00708 80893DE8 8F040000 */  lw      $a0, 0x0000($t8)           ## 00000000
/* 0070C 80893DEC 240601D3 */  addiu   $a2, $zero, 0x01D3         ## $a2 = 000001D3
/* 00710 80893DF0 0C0346A2 */  jal     Matrix_NewMtx              
/* 00714 80893DF4 AFA20028 */  sw      $v0, 0x0028($sp)           
/* 00718 80893DF8 8FA30028 */  lw      $v1, 0x0028($sp)           
/* 0071C 80893DFC 3C0A0601 */  lui     $t2, 0x0601                ## $t2 = 06010000
/* 00720 80893E00 254ABD80 */  addiu   $t2, $t2, 0xBD80           ## $t2 = 0600BD80
/* 00724 80893E04 AC620004 */  sw      $v0, 0x0004($v1)           ## 00000004
/* 00728 80893E08 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 0072C 80893E0C 3C09DE00 */  lui     $t1, 0xDE00                ## $t1 = DE000000
/* 00730 80893E10 24590008 */  addiu   $t9, $v0, 0x0008           ## $t9 = 00000008
/* 00734 80893E14 AE1902D0 */  sw      $t9, 0x02D0($s0)           ## 000002D0
/* 00738 80893E18 AC4A0004 */  sw      $t2, 0x0004($v0)           ## 00000004
/* 0073C 80893E1C AC490000 */  sw      $t1, 0x0000($v0)           ## 00000000
.L80893E20:
/* 00740 80893E20 8FAB0054 */  lw      $t3, 0x0054($sp)           
/* 00744 80893E24 3C068089 */  lui     $a2, %hi(D_80894078)       ## $a2 = 80890000
/* 00748 80893E28 24C64078 */  addiu   $a2, $a2, %lo(D_80894078)  ## $a2 = 80894078
/* 0074C 80893E2C 27A40034 */  addiu   $a0, $sp, 0x0034           ## $a0 = FFFFFFE4
/* 00750 80893E30 240701DC */  addiu   $a3, $zero, 0x01DC         ## $a3 = 000001DC
/* 00754 80893E34 0C031AD5 */  jal     Graph_CloseDisps              
/* 00758 80893E38 8D650000 */  lw      $a1, 0x0000($t3)           ## 00000000
/* 0075C 80893E3C 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00760 80893E40 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 00764 80893E44 27BD0050 */  addiu   $sp, $sp, 0x0050           ## $sp = 00000000
/* 00768 80893E48 03E00008 */  jr      $ra                        
/* 0076C 80893E4C 00000000 */  nop
