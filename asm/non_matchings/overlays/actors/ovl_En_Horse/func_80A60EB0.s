glabel func_80A60EB0
/* 05BC0 80A60EB0 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 05BC4 80A60EB4 AFBF002C */  sw      $ra, 0x002C($sp)
/* 05BC8 80A60EB8 AFB00028 */  sw      $s0, 0x0028($sp)
/* 05BCC 80A60EBC AFA5003C */  sw      $a1, 0x003C($sp)
/* 05BD0 80A60EC0 AFA60040 */  sw      $a2, 0x0040($sp)
/* 05BD4 80A60EC4 3C0141C8 */  lui     $at, 0x41C8                ## $at = 41C80000
/* 05BD8 80A60EC8 44813000 */  mtc1    $at, $f6                   ## $f6 = 25.00
/* 05BDC 80A60ECC C4880214 */  lwc1    $f8, 0x0214($a0)           ## 00000214
/* 05BE0 80A60ED0 44802000 */  mtc1    $zero, $f4                 ## $f4 = 0.00
/* 05BE4 80A60ED4 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 05BE8 80A60ED8 4608303C */  c.lt.s  $f6, $f8
/* 05BEC 80A60EDC E4840068 */  swc1    $f4, 0x0068($a0)           ## 00000068
/* 05BF0 80A60EE0 45020011 */  bc1fl   .L80A60F28
/* 05BF4 80A60EE4 260401AC */  addiu   $a0, $s0, 0x01AC           ## $a0 = 000001AC
/* 05BF8 80A60EE8 8C8201F0 */  lw      $v0, 0x01F0($a0)           ## 0000039C
/* 05BFC 80A60EEC 3C078013 */  lui     $a3, %hi(D_801333E0)
/* 05C00 80A60EF0 24E733E0 */  addiu   $a3, %lo(D_801333E0)
/* 05C04 80A60EF4 304E0800 */  andi    $t6, $v0, 0x0800           ## $t6 = 00000000
/* 05C08 80A60EF8 15C0000A */  bne     $t6, $zero, .L80A60F24
/* 05C0C 80A60EFC 344F0800 */  ori     $t7, $v0, 0x0800           ## $t7 = 00000800
/* 05C10 80A60F00 AC8F01F0 */  sw      $t7, 0x01F0($a0)           ## 0000039C
/* 05C14 80A60F04 3C188013 */  lui     $t8, %hi(D_801333E8)
/* 05C18 80A60F08 271833E8 */  addiu   $t8, %lo(D_801333E8)
/* 05C1C 80A60F0C AFB80014 */  sw      $t8, 0x0014($sp)
/* 05C20 80A60F10 AFA70010 */  sw      $a3, 0x0010($sp)
/* 05C24 80A60F14 2404282B */  addiu   $a0, $zero, 0x282B         ## $a0 = 0000282B
/* 05C28 80A60F18 260500E4 */  addiu   $a1, $s0, 0x00E4           ## $a1 = 000000E4
/* 05C2C 80A60F1C 0C03DCE3 */  jal     Audio_PlaySoundGeneral

/* 05C30 80A60F20 24060004 */  addiu   $a2, $zero, 0x0004         ## $a2 = 00000004
.L80A60F24:
/* 05C34 80A60F24 260401AC */  addiu   $a0, $s0, 0x01AC           ## $a0 = 000001AC
.L80A60F28:
/* 05C38 80A60F28 0C02927F */  jal     SkelAnime_FrameUpdateMatrix

/* 05C3C 80A60F2C AFA40030 */  sw      $a0, 0x0030($sp)
/* 05C40 80A60F30 50400045 */  beql    $v0, $zero, .L80A61048
/* 05C44 80A60F34 8FBF002C */  lw      $ra, 0x002C($sp)
/* 05C48 80A60F38 96020384 */  lhu     $v0, 0x0384($s0)           ## 00000384
/* 05C4C 80A60F3C AE000210 */  sw      $zero, 0x0210($s0)         ## 00000210
/* 05C50 80A60F40 30590004 */  andi    $t9, $v0, 0x0004           ## $t9 = 00000000
/* 05C54 80A60F44 17200022 */  bne     $t9, $zero, .L80A60FD0
/* 05C58 80A60F48 34480004 */  ori     $t0, $v0, 0x0004           ## $t0 = 00000004
/* 05C5C 80A60F4C 8E090158 */  lw      $t1, 0x0158($s0)           ## 00000158
/* 05C60 80A60F50 3C0B80A6 */  lui     $t3, %hi(D_80A65E58)       ## $t3 = 80A60000
/* 05C64 80A60F54 A6080384 */  sh      $t0, 0x0384($s0)           ## 00000384
/* 05C68 80A60F58 00095080 */  sll     $t2, $t1,  2
/* 05C6C 80A60F5C 016A5821 */  addu    $t3, $t3, $t2
/* 05C70 80A60F60 8D6B5E58 */  lw      $t3, %lo(D_80A65E58)($t3)
/* 05C74 80A60F64 00006880 */  sll     $t5, $zero,  2
/* 05C78 80A60F68 016D7021 */  addu    $t6, $t3, $t5
/* 05C7C 80A60F6C 0C028800 */  jal     SkelAnime_GetFrameCount

/* 05C80 80A60F70 8DC40000 */  lw      $a0, 0x0000($t6)           ## 00000000
/* 05C84 80A60F74 8E0F0158 */  lw      $t7, 0x0158($s0)           ## 00000158
/* 05C88 80A60F78 44825000 */  mtc1    $v0, $f10                  ## $f10 = 0.00
/* 05C8C 80A60F7C 3C1980A6 */  lui     $t9, %hi(D_80A65E58)       ## $t9 = 80A60000
/* 05C90 80A60F80 000FC080 */  sll     $t8, $t7,  2
/* 05C94 80A60F84 8E080210 */  lw      $t0, 0x0210($s0)           ## 00000210
/* 05C98 80A60F88 0338C821 */  addu    $t9, $t9, $t8
/* 05C9C 80A60F8C 8F395E58 */  lw      $t9, %lo(D_80A65E58)($t9)
/* 05CA0 80A60F90 46805420 */  cvt.s.w $f16, $f10
/* 05CA4 80A60F94 00084880 */  sll     $t1, $t0,  2
/* 05CA8 80A60F98 3C01C040 */  lui     $at, 0xC040                ## $at = C0400000
/* 05CAC 80A60F9C 03295021 */  addu    $t2, $t9, $t1
/* 05CB0 80A60FA0 44819000 */  mtc1    $at, $f18                  ## $f18 = -3.00
/* 05CB4 80A60FA4 8D450000 */  lw      $a1, 0x0000($t2)           ## 00000000
/* 05CB8 80A60FA8 240C0002 */  addiu   $t4, $zero, 0x0002         ## $t4 = 00000002
/* 05CBC 80A60FAC AFAC0014 */  sw      $t4, 0x0014($sp)
/* 05CC0 80A60FB0 E7B00010 */  swc1    $f16, 0x0010($sp)
/* 05CC4 80A60FB4 8FA40030 */  lw      $a0, 0x0030($sp)
/* 05CC8 80A60FB8 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 05CCC 80A60FBC 24070000 */  addiu   $a3, $zero, 0x0000         ## $a3 = 00000000
/* 05CD0 80A60FC0 0C029468 */  jal     SkelAnime_ChangeAnim

/* 05CD4 80A60FC4 E7B20018 */  swc1    $f18, 0x0018($sp)
/* 05CD8 80A60FC8 1000001F */  beq     $zero, $zero, .L80A61048
/* 05CDC 80A60FCC 8FBF002C */  lw      $ra, 0x002C($sp)
.L80A60FD0:
/* 05CE0 80A60FD0 8E0B0158 */  lw      $t3, 0x0158($s0)           ## 00000158
/* 05CE4 80A60FD4 3C0E80A6 */  lui     $t6, %hi(D_80A65E58)       ## $t6 = 80A60000
/* 05CE8 80A60FD8 8E0F0210 */  lw      $t7, 0x0210($s0)           ## 00000210
/* 05CEC 80A60FDC 000B6880 */  sll     $t5, $t3,  2
/* 05CF0 80A60FE0 01CD7021 */  addu    $t6, $t6, $t5
/* 05CF4 80A60FE4 8DCE5E58 */  lw      $t6, %lo(D_80A65E58)($t6)
/* 05CF8 80A60FE8 000FC080 */  sll     $t8, $t7,  2
/* 05CFC 80A60FEC 01D84021 */  addu    $t0, $t6, $t8
/* 05D00 80A60FF0 0C028800 */  jal     SkelAnime_GetFrameCount

/* 05D04 80A60FF4 8D040000 */  lw      $a0, 0x0000($t0)           ## 00000000
/* 05D08 80A60FF8 8E190158 */  lw      $t9, 0x0158($s0)           ## 00000158
/* 05D0C 80A60FFC 44822000 */  mtc1    $v0, $f4                   ## $f4 = 0.00
/* 05D10 80A61000 3C0A80A6 */  lui     $t2, %hi(D_80A65E58)       ## $t2 = 80A60000
/* 05D14 80A61004 00194880 */  sll     $t1, $t9,  2
/* 05D18 80A61008 8E0C0210 */  lw      $t4, 0x0210($s0)           ## 00000210
/* 05D1C 80A6100C 01495021 */  addu    $t2, $t2, $t1
/* 05D20 80A61010 8D4A5E58 */  lw      $t2, %lo(D_80A65E58)($t2)
/* 05D24 80A61014 468021A0 */  cvt.s.w $f6, $f4
/* 05D28 80A61018 000C5880 */  sll     $t3, $t4,  2
/* 05D2C 80A6101C 014B6821 */  addu    $t5, $t2, $t3
/* 05D30 80A61020 44804000 */  mtc1    $zero, $f8                 ## $f8 = 0.00
/* 05D34 80A61024 8DA50000 */  lw      $a1, 0x0000($t5)           ## 00000000
/* 05D38 80A61028 AFA00014 */  sw      $zero, 0x0014($sp)
/* 05D3C 80A6102C E7A60010 */  swc1    $f6, 0x0010($sp)
/* 05D40 80A61030 8FA40030 */  lw      $a0, 0x0030($sp)
/* 05D44 80A61034 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 05D48 80A61038 24070000 */  addiu   $a3, $zero, 0x0000         ## $a3 = 00000000
/* 05D4C 80A6103C 0C029468 */  jal     SkelAnime_ChangeAnim

/* 05D50 80A61040 E7A80018 */  swc1    $f8, 0x0018($sp)
/* 05D54 80A61044 8FBF002C */  lw      $ra, 0x002C($sp)
.L80A61048:
/* 05D58 80A61048 8FB00028 */  lw      $s0, 0x0028($sp)
/* 05D5C 80A6104C 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000
/* 05D60 80A61050 03E00008 */  jr      $ra
/* 05D64 80A61054 00000000 */  nop
