glabel func_800E2768
/* B59908 800E2768 27BDFFE8 */  addiu $sp, $sp, -0x18
/* B5990C 800E276C AFBF0014 */  sw    $ra, 0x14($sp)
/* B59910 800E2770 0C0389FE */  jal   func_800E27F8
/* B59914 800E2774 AFA5001C */   sw    $a1, 0x1c($sp)
/* B59918 800E2778 8FA5001C */  lw    $a1, 0x1c($sp)
/* B5991C 800E277C 8FBF0014 */  lw    $ra, 0x14($sp)
/* B59920 800E2780 00057100 */  sll   $t6, $a1, 4
/* B59924 800E2784 004E1821 */  addu  $v1, $v0, $t6
/* B59928 800E2788 8C6F0014 */  lw    $t7, 0x14($v1)
/* B5992C 800E278C 55E00003 */  bnezl $t7, .L800E279C
/* B59930 800E2790 00A01025 */   move  $v0, $a1
/* B59934 800E2794 8C650010 */  lw    $a1, 0x10($v1)
/* B59938 800E2798 00A01025 */  move  $v0, $a1
.L800E279C:
/* B5993C 800E279C 03E00008 */  jr    $ra
/* B59940 800E27A0 27BD0018 */   addiu $sp, $sp, 0x18

