glabel func_80B3DA00
/* 01820 80B3DA00 3C0F8016 */  lui     $t7, %hi(gGameInfo)
/* 01824 80B3DA04 8DEFFA90 */  lw      $t7, %lo(gGameInfo)($t7)
/* 01828 80B3DA08 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 0182C 80B3DA0C AFBF0024 */  sw      $ra, 0x0024($sp)
/* 01830 80B3DA10 AFA40028 */  sw      $a0, 0x0028($sp)
/* 01834 80B3DA14 85F8145A */  lh      $t8, 0x145A($t7)           ## 8016145A
/* 01838 80B3DA18 3C0142BE */  lui     $at, 0x42BE                ## $at = 42BE0000
/* 0183C 80B3DA1C 44814000 */  mtc1    $at, $f8                   ## $f8 = 95.00
/* 01840 80B3DA20 44982000 */  mtc1    $t8, $f4                   ## $f4 = 0.00
/* 01844 80B3DA24 C4800090 */  lwc1    $f0, 0x0090($a0)           ## 00000090
/* 01848 80B3DA28 3C040600 */  lui     $a0, 0x0600                ## $a0 = 06000000
/* 0184C 80B3DA2C 468021A0 */  cvt.s.w $f6, $f4
/* 01850 80B3DA30 46083280 */  add.s   $f10, $f6, $f8
/* 01854 80B3DA34 460A003E */  c.le.s  $f0, $f10
/* 01858 80B3DA38 00000000 */  nop
/* 0185C 80B3DA3C 45020017 */  bc1fl   .L80B3DA9C
/* 01860 80B3DA40 8FBF0024 */  lw      $ra, 0x0024($sp)
/* 01864 80B3DA44 0C028800 */  jal     SkelAnime_GetFrameCount

/* 01868 80B3DA48 24844828 */  addiu   $a0, $a0, 0x4828           ## $a0 = 06004828
/* 0186C 80B3DA4C 44828000 */  mtc1    $v0, $f16                  ## $f16 = 0.00
/* 01870 80B3DA50 3C01C140 */  lui     $at, 0xC140                ## $at = C1400000
/* 01874 80B3DA54 44812000 */  mtc1    $at, $f4                   ## $f4 = -12.00
/* 01878 80B3DA58 468084A0 */  cvt.s.w $f18, $f16
/* 0187C 80B3DA5C 8FA40028 */  lw      $a0, 0x0028($sp)
/* 01880 80B3DA60 3C050600 */  lui     $a1, 0x0600                ## $a1 = 06000000
/* 01884 80B3DA64 24A54828 */  addiu   $a1, $a1, 0x4828           ## $a1 = 06004828
/* 01888 80B3DA68 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 0188C 80B3DA6C 24070000 */  addiu   $a3, $zero, 0x0000         ## $a3 = 00000000
/* 01890 80B3DA70 E7B20010 */  swc1    $f18, 0x0010($sp)
/* 01894 80B3DA74 AFA00014 */  sw      $zero, 0x0014($sp)
/* 01898 80B3DA78 E7A40018 */  swc1    $f4, 0x0018($sp)
/* 0189C 80B3DA7C 0C029468 */  jal     SkelAnime_ChangeAnim

/* 018A0 80B3DA80 2484014C */  addiu   $a0, $a0, 0x014C           ## $a0 = 0000014C
/* 018A4 80B3DA84 8FA20028 */  lw      $v0, 0x0028($sp)
/* 018A8 80B3DA88 44803000 */  mtc1    $zero, $f6                 ## $f6 = 0.00
/* 018AC 80B3DA8C 24190005 */  addiu   $t9, $zero, 0x0005         ## $t9 = 00000005
/* 018B0 80B3DA90 AC590260 */  sw      $t9, 0x0260($v0)           ## 00000260
/* 018B4 80B3DA94 E4460268 */  swc1    $f6, 0x0268($v0)           ## 00000268
/* 018B8 80B3DA98 8FBF0024 */  lw      $ra, 0x0024($sp)
.L80B3DA9C:
/* 018BC 80B3DA9C 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 018C0 80B3DAA0 03E00008 */  jr      $ra
/* 018C4 80B3DAA4 00000000 */  nop