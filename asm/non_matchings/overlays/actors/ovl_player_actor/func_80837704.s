glabel func_80837704
/* 054F4 80837704 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 054F8 80837708 AFBF002C */  sw      $ra, 0x002C($sp)
/* 054FC 8083770C AFB00028 */  sw      $s0, 0x0028($sp)
/* 05500 80837710 AFA40038 */  sw      $a0, 0x0038($sp)
/* 05504 80837714 80A20842 */  lb      $v0, 0x0842($a1)           ## 00000842
/* 05508 80837718 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 0550C 8083771C 28410004 */  slti    $at, $v0, 0x0004
/* 05510 80837720 1420000A */  bne     $at, $zero, .L8083774C
/* 05514 80837724 28410008 */  slti    $at, $v0, 0x0008
/* 05518 80837728 10200008 */  beq     $at, $zero, .L8083774C
/* 0551C 8083772C 00000000 */  nop
/* 05520 80837730 0C023C68 */  jal     func_8008F1A0
/* 05524 80837734 00A02025 */  or      $a0, $a1, $zero            ## $a0 = 00000000
/* 05528 80837738 00027080 */  sll     $t6, $v0,  2
/* 0552C 8083773C 3C068085 */  lui     $a2, %hi(D_80854358)       ## $a2 = 80850000
/* 05530 80837740 00CE3021 */  addu    $a2, $a2, $t6
/* 05534 80837744 10000007 */  beq     $zero, $zero, .L80837764
/* 05538 80837748 8CC64358 */  lw      $a2, %lo(D_80854358)($a2)
.L8083774C:
/* 0553C 8083774C 0C023C68 */  jal     func_8008F1A0
/* 05540 80837750 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 05544 80837754 00027880 */  sll     $t7, $v0,  2
/* 05548 80837758 3C068085 */  lui     $a2, %hi(D_80854350)       ## $a2 = 80850000
/* 0554C 8083775C 00CF3021 */  addu    $a2, $a2, $t7
/* 05550 80837760 8CC64350 */  lw      $a2, %lo(D_80854350)($a2)
.L80837764:
/* 05554 80837764 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 05558 80837768 0C20C8C6 */  jal     func_80832318
/* 0555C 8083776C AFA60034 */  sw      $a2, 0x0034($sp)
/* 05560 80837770 0C028800 */  jal     SkelAnime_GetFrameCount

/* 05564 80837774 8FA40034 */  lw      $a0, 0x0034($sp)
/* 05568 80837778 44823000 */  mtc1    $v0, $f6                   ## $f6 = 0.00
/* 0556C 8083777C 3C014100 */  lui     $at, 0x4100                ## $at = 41000000
/* 05570 80837780 44812000 */  mtc1    $at, $f4                   ## $f4 = 8.00
/* 05574 80837784 46803220 */  cvt.s.w $f8, $f6
/* 05578 80837788 3C01C110 */  lui     $at, 0xC110                ## $at = C1100000
/* 0557C 8083778C 44815000 */  mtc1    $at, $f10                  ## $f10 = -9.00
/* 05580 80837790 24180002 */  addiu   $t8, $zero, 0x0002         ## $t8 = 00000002
/* 05584 80837794 AFB80018 */  sw      $t8, 0x0018($sp)
/* 05588 80837798 8FA60034 */  lw      $a2, 0x0034($sp)
/* 0558C 8083779C E7A80014 */  swc1    $f8, 0x0014($sp)
/* 05590 808377A0 8FA40038 */  lw      $a0, 0x0038($sp)
/* 05594 808377A4 260501B4 */  addiu   $a1, $s0, 0x01B4           ## $a1 = 000001B4
/* 05598 808377A8 3C073F80 */  lui     $a3, 0x3F80                ## $a3 = 3F800000
/* 0559C 808377AC E7A40010 */  swc1    $f4, 0x0010($sp)
/* 055A0 808377B0 0C028FC2 */  jal     SkelAnime_ChangeLinkAnim
/* 055A4 808377B4 E7AA001C */  swc1    $f10, 0x001C($sp)
/* 055A8 808377B8 8FA40038 */  lw      $a0, 0x0038($sp)
/* 055AC 808377BC 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 055B0 808377C0 0C20DD4C */  jal     func_80837530
/* 055B4 808377C4 24060200 */  addiu   $a2, $zero, 0x0200         ## $a2 = 00000200
/* 055B8 808377C8 8FBF002C */  lw      $ra, 0x002C($sp)
/* 055BC 808377CC 8FB00028 */  lw      $s0, 0x0028($sp)
/* 055C0 808377D0 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000
/* 055C4 808377D4 03E00008 */  jr      $ra
/* 055C8 808377D8 00000000 */  nop