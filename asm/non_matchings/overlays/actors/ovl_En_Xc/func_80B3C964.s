glabel func_80B3C964
/* 00784 80B3C964 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 00788 80B3C968 AFBF0014 */  sw      $ra, 0x0014($sp)
/* 0078C 80B3C96C AFA40018 */  sw      $a0, 0x0018($sp)
/* 00790 80B3C970 AFA5001C */  sw      $a1, 0x001C($sp)
/* 00794 80B3C974 8C8E016C */  lw      $t6, 0x016C($a0)           ## 0000016C
/* 00798 80B3C978 8C99016C */  lw      $t9, 0x016C($a0)           ## 0000016C
/* 0079C 80B3C97C 908A0181 */  lbu     $t2, 0x0181($a0)           ## 00000181
/* 007A0 80B3C980 89D80000 */  lwl     $t8, 0x0000($t6)           ## 00000000
/* 007A4 80B3C984 99D80003 */  lwr     $t8, 0x0003($t6)           ## 00000003
/* 007A8 80B3C988 354B0003 */  ori     $t3, $t2, 0x0003           ## $t3 = 00000003
/* 007AC 80B3C98C 00802825 */  or      $a1, $a0, $zero            ## $a1 = 00000000
/* 007B0 80B3C990 A898018A */  swl     $t8, 0x018A($a0)           ## 0000018A
/* 007B4 80B3C994 B898018D */  swr     $t8, 0x018D($a0)           ## 0000018D
/* 007B8 80B3C998 95D80004 */  lhu     $t8, 0x0004($t6)           ## 00000004
/* 007BC 80B3C99C 24A6014C */  addiu   $a2, $a1, 0x014C           ## $a2 = 0000014C
/* 007C0 80B3C9A0 3C073F80 */  lui     $a3, 0x3F80                ## $a3 = 3F800000
/* 007C4 80B3C9A4 A498018E */  sh      $t8, 0x018E($a0)           ## 0000018E
/* 007C8 80B3C9A8 8B290000 */  lwl     $t1, 0x0000($t9)           ## 00000000
/* 007CC 80B3C9AC 9B290003 */  lwr     $t1, 0x0003($t9)           ## 00000003
/* 007D0 80B3C9B0 A8890184 */  swl     $t1, 0x0184($a0)           ## 00000184
/* 007D4 80B3C9B4 B8890187 */  swr     $t1, 0x0187($a0)           ## 00000187
/* 007D8 80B3C9B8 97290004 */  lhu     $t1, 0x0004($t9)           ## 00000004
/* 007DC 80B3C9BC A08B0181 */  sb      $t3, 0x0181($a0)           ## 00000181
/* 007E0 80B3C9C0 A4890188 */  sh      $t1, 0x0188($a0)           ## 00000188
/* 007E4 80B3C9C4 0C028D88 */  jal     SkelAnime_LoadAnimationType5
/* 007E8 80B3C9C8 8FA4001C */  lw      $a0, 0x001C($sp)
/* 007EC 80B3C9CC 8FBF0014 */  lw      $ra, 0x0014($sp)
/* 007F0 80B3C9D0 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 007F4 80B3C9D4 03E00008 */  jr      $ra
/* 007F8 80B3C9D8 00000000 */  nop
