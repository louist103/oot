glabel func_80932D10
/* 06740 80932D10 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 06744 80932D14 AFBF0014 */  sw      $ra, 0x0014($sp)
/* 06748 80932D18 848F001C */  lh      $t7, 0x001C($a0)           ## 0000001C
/* 0674C 80932D1C 3C018093 */  lui     $at, %hi(D_8093746C)       ## $at = 80930000
/* 06750 80932D20 240E000B */  addiu   $t6, $zero, 0x000B         ## $t6 = 0000000B
/* 06754 80932D24 000FC080 */  sll     $t8, $t7,  2
/* 06758 80932D28 00380821 */  addu    $at, $at, $t8
/* 0675C 80932D2C 00803825 */  or      $a3, $a0, $zero            ## $a3 = 00000000
/* 06760 80932D30 AC2E746C */  sw      $t6, %lo(D_8093746C)($at)
/* 06764 80932D34 84F9001C */  lh      $t9, 0x001C($a3)           ## 0000001C
/* 06768 80932D38 3C058093 */  lui     $a1, %hi(D_8093785C)       ## $a1 = 80930000
/* 0676C 80932D3C AFA70018 */  sw      $a3, 0x0018($sp)
/* 06770 80932D40 00194080 */  sll     $t0, $t9,  2
/* 06774 80932D44 00A82821 */  addu    $a1, $a1, $t0
/* 06778 80932D48 8CA5785C */  lw      $a1, %lo(D_8093785C)($a1)
/* 0677C 80932D4C 2484014C */  addiu   $a0, $a0, 0x014C           ## $a0 = 0000014C
/* 06780 80932D50 0C029490 */  jal     Animation_MorphToPlayOnce
/* 06784 80932D54 3C064000 */  lui     $a2, 0x4000                ## $a2 = 40000000
/* 06788 80932D58 8FA70018 */  lw      $a3, 0x0018($sp)
/* 0678C 80932D5C 240900A0 */  addiu   $t1, $zero, 0x00A0         ## $t1 = 000000A0
/* 06790 80932D60 240D0180 */  addiu   $t5, $zero, 0x0180         ## $t5 = 00000180
/* 06794 80932D64 84EA001C */  lh      $t2, 0x001C($a3)           ## 0000001C
/* 06798 80932D68 3C0F8093 */  lui     $t7, %hi(func_80932DAC)    ## $t7 = 80930000
/* 0679C 80932D6C A4E000B4 */  sh      $zero, 0x00B4($a3)         ## 000000B4
/* 067A0 80932D70 15400004 */  bne     $t2, $zero, .L80932D84
/* 067A4 80932D74 A4E90198 */  sh      $t1, 0x0198($a3)           ## 00000198
/* 067A8 80932D78 240BF800 */  addiu   $t3, $zero, 0xF800         ## $t3 = FFFFF800
/* 067AC 80932D7C 10000004 */  beq     $zero, $zero, .L80932D90
/* 067B0 80932D80 A4EB01A2 */  sh      $t3, 0x01A2($a3)           ## 000001A2
.L80932D84:
/* 067B4 80932D84 240CF800 */  addiu   $t4, $zero, 0xF800         ## $t4 = FFFFF800
/* 067B8 80932D88 A4E001A2 */  sh      $zero, 0x01A2($a3)         ## 000001A2
/* 067BC 80932D8C A4EC00B4 */  sh      $t4, 0x00B4($a3)           ## 000000B4
.L80932D90:
/* 067C0 80932D90 25EF2DAC */  addiu   $t7, $t7, %lo(func_80932DAC) ## $t7 = 80932DAC
/* 067C4 80932D94 A4ED019A */  sh      $t5, 0x019A($a3)           ## 0000019A
/* 067C8 80932D98 ACEF0190 */  sw      $t7, 0x0190($a3)           ## 00000190
/* 067CC 80932D9C 8FBF0014 */  lw      $ra, 0x0014($sp)
/* 067D0 80932DA0 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 067D4 80932DA4 03E00008 */  jr      $ra
/* 067D8 80932DA8 00000000 */  nop
