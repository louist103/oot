.rdata
glabel D_80897684
    .asciz "../z_bg_jya_cobra.c"
    .balign 4

glabel D_80897698
    .asciz "../z_bg_jya_cobra.c"
    .balign 4

glabel D_808976AC
    .asciz "../z_bg_jya_cobra.c"
    .balign 4

.late_rodata
glabel D_80897740
    .float 0.1

.text
glabel func_80896D78
/* 01488 80896D78 27BDFFB0 */  addiu   $sp, $sp, 0xFFB0           ## $sp = FFFFFFB0
/* 0148C 80896D7C AFBF001C */  sw      $ra, 0x001C($sp)           
/* 01490 80896D80 AFB10018 */  sw      $s1, 0x0018($sp)           
/* 01494 80896D84 AFB00014 */  sw      $s0, 0x0014($sp)           
/* 01498 80896D88 AFA50054 */  sw      $a1, 0x0054($sp)           
/* 0149C 80896D8C 8CA50000 */  lw      $a1, 0x0000($a1)           ## 00000000
/* 014A0 80896D90 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 014A4 80896D94 3C068089 */  lui     $a2, %hi(D_80897684)       ## $a2 = 80890000
/* 014A8 80896D98 24C67684 */  addiu   $a2, $a2, %lo(D_80897684)  ## $a2 = 80897684
/* 014AC 80896D9C 27A40030 */  addiu   $a0, $sp, 0x0030           ## $a0 = FFFFFFE0
/* 014B0 80896DA0 2407039C */  addiu   $a3, $zero, 0x039C         ## $a3 = 0000039C
/* 014B4 80896DA4 0C031AB1 */  jal     Graph_OpenDisps              
/* 014B8 80896DA8 00A08825 */  or      $s1, $a1, $zero            ## $s1 = 00000000
/* 014BC 80896DAC 8FAF0054 */  lw      $t7, 0x0054($sp)           
/* 014C0 80896DB0 0C024F61 */  jal     func_80093D84              
/* 014C4 80896DB4 8DE40000 */  lw      $a0, 0x0000($t7)           ## 00000000
/* 014C8 80896DB8 8618001C */  lh      $t8, 0x001C($s0)           ## 0000001C
/* 014CC 80896DBC 3C098089 */  lui     $t1, %hi(D_80897308)       ## $t1 = 80890000
/* 014D0 80896DC0 860A00B4 */  lh      $t2, 0x00B4($s0)           ## 000000B4
/* 014D4 80896DC4 33190003 */  andi    $t9, $t8, 0x0003           ## $t9 = 00000000
/* 014D8 80896DC8 00194040 */  sll     $t0, $t9,  1               
/* 014DC 80896DCC 01284821 */  addu    $t1, $t1, $t0              
/* 014E0 80896DD0 85297308 */  lh      $t1, %lo(D_80897308)($t1)  
/* 014E4 80896DD4 27A70044 */  addiu   $a3, $sp, 0x0044           ## $a3 = FFFFFFF4
/* 014E8 80896DD8 012A5821 */  addu    $t3, $t1, $t2              
/* 014EC 80896DDC A7AB0044 */  sh      $t3, 0x0044($sp)           
/* 014F0 80896DE0 860C00B6 */  lh      $t4, 0x00B6($s0)           ## 000000B6
/* 014F4 80896DE4 A7AC0046 */  sh      $t4, 0x0046($sp)           
/* 014F8 80896DE8 860D00B8 */  lh      $t5, 0x00B8($s0)           ## 000000B8
/* 014FC 80896DEC A7AD0048 */  sh      $t5, 0x0048($sp)           
/* 01500 80896DF0 8E060188 */  lw      $a2, 0x0188($s0)           ## 00000188
/* 01504 80896DF4 C60E0184 */  lwc1    $f14, 0x0184($s0)          ## 00000184
/* 01508 80896DF8 0C0345A5 */  jal     func_800D1694              
/* 0150C 80896DFC C60C0180 */  lwc1    $f12, 0x0180($s0)          ## 00000180
/* 01510 80896E00 3C018089 */  lui     $at, %hi(D_80897740)       ## $at = 80890000
/* 01514 80896E04 C42C7740 */  lwc1    $f12, %lo(D_80897740)($at) 
/* 01518 80896E08 8E060190 */  lw      $a2, 0x0190($s0)           ## 00000190
/* 0151C 80896E0C 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 01520 80896E10 0C0342A3 */  jal     Matrix_Scale              
/* 01524 80896E14 46006386 */  mov.s   $f14, $f12                 
/* 01528 80896E18 8E2202D0 */  lw      $v0, 0x02D0($s1)           ## 000002D0
/* 0152C 80896E1C 3C0FDA38 */  lui     $t7, 0xDA38                ## $t7 = DA380000
/* 01530 80896E20 35EF0003 */  ori     $t7, $t7, 0x0003           ## $t7 = DA380003
/* 01534 80896E24 244E0008 */  addiu   $t6, $v0, 0x0008           ## $t6 = 00000008
/* 01538 80896E28 AE2E02D0 */  sw      $t6, 0x02D0($s1)           ## 000002D0
/* 0153C 80896E2C AC4F0000 */  sw      $t7, 0x0000($v0)           ## 00000000
/* 01540 80896E30 8FB80054 */  lw      $t8, 0x0054($sp)           
/* 01544 80896E34 3C058089 */  lui     $a1, %hi(D_80897698)       ## $a1 = 80890000
/* 01548 80896E38 24A57698 */  addiu   $a1, $a1, %lo(D_80897698)  ## $a1 = 80897698
/* 0154C 80896E3C 8F040000 */  lw      $a0, 0x0000($t8)           ## 00000000
/* 01550 80896E40 240603AB */  addiu   $a2, $zero, 0x03AB         ## $a2 = 000003AB
/* 01554 80896E44 0C0346A2 */  jal     Matrix_NewMtx              
/* 01558 80896E48 AFA2002C */  sw      $v0, 0x002C($sp)           
/* 0155C 80896E4C 8FA3002C */  lw      $v1, 0x002C($sp)           
/* 01560 80896E50 3C08FA00 */  lui     $t0, 0xFA00                ## $t0 = FA000000
/* 01564 80896E54 3C01430C */  lui     $at, 0x430C                ## $at = 430C0000
/* 01568 80896E58 AC620004 */  sw      $v0, 0x0004($v1)           ## 00000004
/* 0156C 80896E5C 8E2202D0 */  lw      $v0, 0x02D0($s1)           ## 000002D0
/* 01570 80896E60 44813000 */  mtc1    $at, $f6                   ## $f6 = 140.00
/* 01574 80896E64 2401FF00 */  addiu   $at, $zero, 0xFF00         ## $at = FFFFFF00
/* 01578 80896E68 24590008 */  addiu   $t9, $v0, 0x0008           ## $t9 = 00000008
/* 0157C 80896E6C AE3902D0 */  sw      $t9, 0x02D0($s1)           ## 000002D0
/* 01580 80896E70 AC480000 */  sw      $t0, 0x0000($v0)           ## 00000000
/* 01584 80896E74 C604018C */  lwc1    $f4, 0x018C($s0)           ## 0000018C
/* 01588 80896E78 3C0F0601 */  lui     $t7, %hi(D_060117D0)                ## $t7 = 06010000
/* 0158C 80896E7C 25EF17D0 */  addiu   $t7, $t7, %lo(D_060117D0)           ## $t7 = 060117D0
/* 01590 80896E80 46062202 */  mul.s   $f8, $f4, $f6              
/* 01594 80896E84 3C0EDE00 */  lui     $t6, 0xDE00                ## $t6 = DE000000
/* 01598 80896E88 3C068089 */  lui     $a2, %hi(D_808976AC)       ## $a2 = 80890000
/* 0159C 80896E8C 24C676AC */  addiu   $a2, $a2, %lo(D_808976AC)  ## $a2 = 808976AC
/* 015A0 80896E90 27A40030 */  addiu   $a0, $sp, 0x0030           ## $a0 = FFFFFFE0
/* 015A4 80896E94 240703B3 */  addiu   $a3, $zero, 0x03B3         ## $a3 = 000003B3
/* 015A8 80896E98 4600428D */  trunc.w.s $f10, $f8                  
/* 015AC 80896E9C 440A5000 */  mfc1    $t2, $f10                  
/* 015B0 80896EA0 00000000 */  nop
/* 015B4 80896EA4 314B00FF */  andi    $t3, $t2, 0x00FF           ## $t3 = 00000000
/* 015B8 80896EA8 01616025 */  or      $t4, $t3, $at              ## $t4 = FFFFFF00
/* 015BC 80896EAC AC4C0004 */  sw      $t4, 0x0004($v0)           ## 00000004
/* 015C0 80896EB0 8E2202D0 */  lw      $v0, 0x02D0($s1)           ## 000002D0
/* 015C4 80896EB4 244D0008 */  addiu   $t5, $v0, 0x0008           ## $t5 = 00000008
/* 015C8 80896EB8 AE2D02D0 */  sw      $t5, 0x02D0($s1)           ## 000002D0
/* 015CC 80896EBC AC4F0004 */  sw      $t7, 0x0004($v0)           ## 00000004
/* 015D0 80896EC0 AC4E0000 */  sw      $t6, 0x0000($v0)           ## 00000000
/* 015D4 80896EC4 8FB80054 */  lw      $t8, 0x0054($sp)           
/* 015D8 80896EC8 0C031AD5 */  jal     Graph_CloseDisps              
/* 015DC 80896ECC 8F050000 */  lw      $a1, 0x0000($t8)           ## 00000000
/* 015E0 80896ED0 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 015E4 80896ED4 8FB00014 */  lw      $s0, 0x0014($sp)           
/* 015E8 80896ED8 8FB10018 */  lw      $s1, 0x0018($sp)           
/* 015EC 80896EDC 03E00008 */  jr      $ra                        
/* 015F0 80896EE0 27BD0050 */  addiu   $sp, $sp, 0x0050           ## $sp = 00000000
