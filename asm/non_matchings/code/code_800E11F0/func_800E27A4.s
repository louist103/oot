glabel func_800E27A4
/* B59944 800E27A4 27BDFFE8 */  addiu $sp, $sp, -0x18
/* B59948 800E27A8 AFBF0014 */  sw    $ra, 0x14($sp)
/* B5994C 800E27AC AFA40018 */  sw    $a0, 0x18($sp)
/* B59950 800E27B0 0C03813A */  jal   func_800E04E8
/* B59954 800E27B4 AFA5001C */   sw    $a1, 0x1c($sp)
/* B59958 800E27B8 10400003 */  beqz  $v0, .L800E27C8
/* B5995C 800E27BC 8FA40018 */   lw    $a0, 0x18($sp)
/* B59960 800E27C0 1000000A */  b     .L800E27EC
/* B59964 800E27C4 8FBF0014 */   lw    $ra, 0x14($sp)
.L800E27C8:
/* B59968 800E27C8 24050002 */  li    $a1, 2
/* B5996C 800E27CC 0C037C1D */  jal   func_800DF074
/* B59970 800E27D0 8FA6001C */   lw    $a2, 0x1c($sp)
/* B59974 800E27D4 50400004 */  beql  $v0, $zero, .L800E27E8
/* B59978 800E27D8 00001025 */   move  $v0, $zero
/* B5997C 800E27DC 10000003 */  b     .L800E27EC
/* B59980 800E27E0 8FBF0014 */   lw    $ra, 0x14($sp)
/* B59984 800E27E4 00001025 */  move  $v0, $zero
.L800E27E8:
/* B59988 800E27E8 8FBF0014 */  lw    $ra, 0x14($sp)
.L800E27EC:
/* B5998C 800E27EC 27BD0018 */  addiu $sp, $sp, 0x18
/* B59990 800E27F0 03E00008 */  jr    $ra
/* B59994 800E27F4 00000000 */   nop

