glabel func_8003992C
/* AB0ACC 8003992C 27BDFFD0 */  addiu $sp, $sp, -0x30
/* AB0AD0 80039930 AFB00028 */  sw    $s0, 0x28($sp)
/* AB0AD4 80039934 8FB00044 */  lw    $s0, 0x44($sp)
/* AB0AD8 80039938 C7AC004C */  lwc1  $f12, 0x4c($sp)
/* AB0ADC 8003993C AFBF002C */  sw    $ra, 0x2c($sp)
/* AB0AE0 80039940 320E0004 */  andi  $t6, $s0, 4
/* AB0AE4 80039944 AFA40030 */  sw    $a0, 0x30($sp)
/* AB0AE8 80039948 AFA50034 */  sw    $a1, 0x34($sp)
/* AB0AEC 8003994C AFA60038 */  sw    $a2, 0x38($sp)
/* AB0AF0 80039950 AFA7003C */  sw    $a3, 0x3c($sp)
/* AB0AF4 80039954 11C0000C */  beqz  $t6, .L80039988
/* AB0AF8 80039958 46006086 */   mov.s $f2, $f12
/* AB0AFC 8003995C 8FAF0040 */  lw    $t7, 0x40($sp)
/* AB0B00 80039960 C7A40048 */  lwc1  $f4, 0x48($sp)
/* AB0B04 80039964 00A02025 */  move  $a0, $a1
/* AB0B08 80039968 97A5003A */  lhu   $a1, 0x3a($sp)
/* AB0B0C 8003996C 8FA60030 */  lw    $a2, 0x30($sp)
/* AB0B10 80039970 E7AC0014 */  swc1  $f12, 0x14($sp)
/* AB0B14 80039974 AFA0001C */  sw    $zero, 0x1c($sp)
/* AB0B18 80039978 AFAF0010 */  sw    $t7, 0x10($sp)
/* AB0B1C 8003997C 0C00E5BC */  jal   func_800396F0
/* AB0B20 80039980 E7A40018 */   swc1  $f4, 0x18($sp)
/* AB0B24 80039984 46000086 */  mov.s $f2, $f0
.L80039988:
/* AB0B28 80039988 32180002 */  andi  $t8, $s0, 2
/* AB0B2C 8003998C 17000002 */  bnez  $t8, .L80039998
/* AB0B30 80039990 32190008 */   andi  $t9, $s0, 8
/* AB0B34 80039994 13200011 */  beqz  $t9, .L800399DC
.L80039998:
/* AB0B38 80039998 32080010 */   andi  $t0, $s0, 0x10
/* AB0B3C 8003999C 11000002 */  beqz  $t0, .L800399A8
/* AB0B40 800399A0 00001025 */   move  $v0, $zero
/* AB0B44 800399A4 24020001 */  li    $v0, 1
.L800399A8:
/* AB0B48 800399A8 8FA60030 */  lw    $a2, 0x30($sp)
/* AB0B4C 800399AC 8FA90040 */  lw    $t1, 0x40($sp)
/* AB0B50 800399B0 C7A60048 */  lwc1  $f6, 0x48($sp)
/* AB0B54 800399B4 8FA40034 */  lw    $a0, 0x34($sp)
/* AB0B58 800399B8 97A5003A */  lhu   $a1, 0x3a($sp)
/* AB0B5C 800399BC 8FA7003C */  lw    $a3, 0x3c($sp)
/* AB0B60 800399C0 E7A20014 */  swc1  $f2, 0x14($sp)
/* AB0B64 800399C4 AFA2001C */  sw    $v0, 0x1c($sp)
/* AB0B68 800399C8 24C60002 */  addiu $a2, $a2, 2
/* AB0B6C 800399CC AFA90010 */  sw    $t1, 0x10($sp)
/* AB0B70 800399D0 0C00E5BC */  jal   func_800396F0
/* AB0B74 800399D4 E7A60018 */   swc1  $f6, 0x18($sp)
/* AB0B78 800399D8 46000086 */  mov.s $f2, $f0
.L800399DC:
/* AB0B7C 800399DC 320A0001 */  andi  $t2, $s0, 1
/* AB0B80 800399E0 11400011 */  beqz  $t2, .L80039A28
/* AB0B84 800399E4 320B0010 */   andi  $t3, $s0, 0x10
/* AB0B88 800399E8 11600002 */  beqz  $t3, .L800399F4
/* AB0B8C 800399EC 00001025 */   move  $v0, $zero
/* AB0B90 800399F0 24020001 */  li    $v0, 1
.L800399F4:
/* AB0B94 800399F4 8FA60030 */  lw    $a2, 0x30($sp)
/* AB0B98 800399F8 8FAC0040 */  lw    $t4, 0x40($sp)
/* AB0B9C 800399FC C7A80048 */  lwc1  $f8, 0x48($sp)
/* AB0BA0 80039A00 8FA40034 */  lw    $a0, 0x34($sp)
/* AB0BA4 80039A04 97A5003A */  lhu   $a1, 0x3a($sp)
/* AB0BA8 80039A08 8FA7003C */  lw    $a3, 0x3c($sp)
/* AB0BAC 80039A0C E7A20014 */  swc1  $f2, 0x14($sp)
/* AB0BB0 80039A10 AFA2001C */  sw    $v0, 0x1c($sp)
/* AB0BB4 80039A14 24C60004 */  addiu $a2, $a2, 4
/* AB0BB8 80039A18 AFAC0010 */  sw    $t4, 0x10($sp)
/* AB0BBC 80039A1C 0C00E5BC */  jal   func_800396F0
/* AB0BC0 80039A20 E7A80018 */   swc1  $f8, 0x18($sp)
/* AB0BC4 80039A24 46000086 */  mov.s $f2, $f0
.L80039A28:
/* AB0BC8 80039A28 8FBF002C */  lw    $ra, 0x2c($sp)
/* AB0BCC 80039A2C 8FB00028 */  lw    $s0, 0x28($sp)
/* AB0BD0 80039A30 27BD0030 */  addiu $sp, $sp, 0x30
/* AB0BD4 80039A34 03E00008 */  jr    $ra
/* AB0BD8 80039A38 46001006 */   mov.s $f0, $f2

