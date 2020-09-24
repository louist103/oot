.late_rodata
glabel D_80915438
    .float 0.1

.text
glabel func_80911CB0
/* 01670 80911CB0 27BDFFB8 */  addiu   $sp, $sp, 0xFFB8           ## $sp = FFFFFFB8
/* 01674 80911CB4 AFB00038 */  sw      $s0, 0x0038($sp)
/* 01678 80911CB8 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 0167C 80911CBC AFBF003C */  sw      $ra, 0x003C($sp)
/* 01680 80911CC0 3C040601 */  lui     $a0, 0x0601                ## $a0 = 06010000
/* 01684 80911CC4 AFA5004C */  sw      $a1, 0x004C($sp)
/* 01688 80911CC8 0C028800 */  jal     SkelAnime_GetFrameCount

/* 0168C 80911CCC 2484EC94 */  addiu   $a0, $a0, 0xEC94           ## $a0 = 0600EC94
/* 01690 80911CD0 44822000 */  mtc1    $v0, $f4                   ## $f4 = 0.00
/* 01694 80911CD4 3C050601 */  lui     $a1, 0x0601                ## $a1 = 06010000
/* 01698 80911CD8 24A5EC94 */  addiu   $a1, $a1, 0xEC94           ## $a1 = 0600EC94
/* 0169C 80911CDC 468021A0 */  cvt.s.w $f6, $f4
/* 016A0 80911CE0 2604014C */  addiu   $a0, $s0, 0x014C           ## $a0 = 0000014C
/* 016A4 80911CE4 3C06C0A0 */  lui     $a2, 0xC0A0                ## $a2 = C0A00000
/* 016A8 80911CE8 0C029490 */  jal     SkelAnime_ChangeAnimTransitionStop
/* 016AC 80911CEC E60601D0 */  swc1    $f6, 0x01D0($s0)           ## 000001D0
/* 016B0 80911CF0 3C0E8091 */  lui     $t6, %hi(func_80911DD8)    ## $t6 = 80910000
/* 016B4 80911CF4 25CE1DD8 */  addiu   $t6, $t6, %lo(func_80911DD8) ## $t6 = 80911DD8
/* 016B8 80911CF8 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 016BC 80911CFC AE0E0190 */  sw      $t6, 0x0190($s0)           ## 00000190
/* 016C0 80911D00 3C018091 */  lui     $at, %hi(D_80915438)       ## $at = 80910000
/* 016C4 80911D04 C4285438 */  lwc1    $f8, %lo(D_80915438)($at)
/* 016C8 80911D08 24080019 */  addiu   $t0, $zero, 0x0019         ## $t0 = 00000019
/* 016CC 80911D0C 24020019 */  addiu   $v0, $zero, 0x0019         ## $v0 = 00000019
/* 016D0 80911D10 4608003E */  c.le.s  $f0, $f8
/* 016D4 80911D14 00000000 */  nop
/* 016D8 80911D18 4502000F */  bc1fl   .L80911D58
/* 016DC 80911D1C A60001A2 */  sh      $zero, 0x01A2($s0)         ## 000001A2
/* 016E0 80911D20 860F01A6 */  lh      $t7, 0x01A6($s0)           ## 000001A6
/* 016E4 80911D24 29E1000A */  slti    $at, $t7, 0x000A
/* 016E8 80911D28 5420000B */  bnel    $at, $zero, .L80911D58
/* 016EC 80911D2C A60001A2 */  sh      $zero, 0x01A2($s0)         ## 000001A2
/* 016F0 80911D30 921801C9 */  lbu     $t8, 0x01C9($s0)           ## 000001C9
/* 016F4 80911D34 24030001 */  addiu   $v1, $zero, 0x0001         ## $v1 = 00000001
/* 016F8 80911D38 241903E8 */  addiu   $t9, $zero, 0x03E8         ## $t9 = 000003E8
/* 016FC 80911D3C 54780006 */  bnel    $v1, $t8, .L80911D58
/* 01700 80911D40 A60001A2 */  sh      $zero, 0x01A2($s0)         ## 000001A2
/* 01704 80911D44 A60301A2 */  sh      $v1, 0x01A2($s0)           ## 000001A2
/* 01708 80911D48 A61901A4 */  sh      $t9, 0x01A4($s0)           ## 000001A4
/* 0170C 80911D4C 10000003 */  beq     $zero, $zero, .L80911D5C
/* 01710 80911D50 24020020 */  addiu   $v0, $zero, 0x0020         ## $v0 = 00000020
/* 01714 80911D54 A60001A2 */  sh      $zero, 0x01A2($s0)         ## 000001A2
.L80911D58:
/* 01718 80911D58 A60801A4 */  sh      $t0, 0x01A4($s0)           ## 000001A4
.L80911D5C:
/* 0171C 80911D5C 8E09011C */  lw      $t1, 0x011C($s0)           ## 0000011C
/* 01720 80911D60 8FA6004C */  lw      $a2, 0x004C($sp)
/* 01724 80911D64 240A0026 */  addiu   $t2, $zero, 0x0026         ## $t2 = 00000026
/* 01728 80911D68 AFA90044 */  sw      $t1, 0x0044($sp)
/* 0172C 80911D6C C60A0200 */  lwc1    $f10, 0x0200($s0)          ## 00000200
/* 01730 80911D70 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 01734 80911D74 2407006D */  addiu   $a3, $zero, 0x006D         ## $a3 = 0000006D
/* 01738 80911D78 E7AA0010 */  swc1    $f10, 0x0010($sp)
/* 0173C 80911D7C C6100204 */  lwc1    $f16, 0x0204($s0)          ## 00000204
/* 01740 80911D80 24C41C24 */  addiu   $a0, $a2, 0x1C24           ## $a0 = 00001C24
/* 01744 80911D84 E7B00014 */  swc1    $f16, 0x0014($sp)
/* 01748 80911D88 C6120208 */  lwc1    $f18, 0x0208($s0)          ## 00000208
/* 0174C 80911D8C AFAA0028 */  sw      $t2, 0x0028($sp)
/* 01750 80911D90 AFA00024 */  sw      $zero, 0x0024($sp)
/* 01754 80911D94 AFA00020 */  sw      $zero, 0x0020($sp)
/* 01758 80911D98 AFA2001C */  sw      $v0, 0x001C($sp)
/* 0175C 80911D9C 0C00C916 */  jal     Actor_SpawnAsChild

/* 01760 80911DA0 E7B20018 */  swc1    $f18, 0x0018($sp)
/* 01764 80911DA4 860C01A6 */  lh      $t4, 0x01A6($s0)           ## 000001A6
/* 01768 80911DA8 8FAB0044 */  lw      $t3, 0x0044($sp)
/* 0176C 80911DAC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 01770 80911DB0 258D0001 */  addiu   $t5, $t4, 0x0001           ## $t5 = 00000001
/* 01774 80911DB4 A60D01A6 */  sh      $t5, 0x01A6($s0)           ## 000001A6
/* 01778 80911DB8 240538AA */  addiu   $a1, $zero, 0x38AA         ## $a1 = 000038AA
/* 0177C 80911DBC 0C00BE0A */  jal     Audio_PlayActorSound2

/* 01780 80911DC0 AE0B011C */  sw      $t3, 0x011C($s0)           ## 0000011C
/* 01784 80911DC4 8FBF003C */  lw      $ra, 0x003C($sp)
/* 01788 80911DC8 8FB00038 */  lw      $s0, 0x0038($sp)
/* 0178C 80911DCC 27BD0048 */  addiu   $sp, $sp, 0x0048           ## $sp = 00000000
/* 01790 80911DD0 03E00008 */  jr      $ra
/* 01794 80911DD4 00000000 */  nop
