glabel func_8003AB28
/* AB1CC8 8003AB28 27BDFFE0 */  addiu $sp, $sp, -0x20
/* AB1CCC 8003AB2C AFBF001C */  sw    $ra, 0x1c($sp)
/* AB1CD0 8003AB30 AFA40020 */  sw    $a0, 0x20($sp)
/* AB1CD4 8003AB34 AFA50024 */  sw    $a1, 0x24($sp)
/* AB1CD8 8003AB38 AFA7002C */  sw    $a3, 0x2c($sp)
/* AB1CDC 8003AB3C 94820000 */  lhu   $v0, ($a0)
/* AB1CE0 8003AB40 97AF003A */  lhu   $t7, 0x3a($sp)
/* AB1CE4 8003AB44 3401FFFF */  li    $at, 65535
/* AB1CE8 8003AB48 10410010 */  beq   $v0, $at, .L8003AB8C
/* AB1CEC 8003AB4C 31F80004 */   andi  $t8, $t7, 4
/* AB1CF0 8003AB50 1700000E */  bnez  $t8, .L8003AB8C
/* AB1CF4 8003AB54 00024080 */   sll   $t0, $v0, 2
/* AB1CF8 8003AB58 8CD90048 */  lw    $t9, 0x48($a2)
/* AB1CFC 8003AB5C C7A40030 */  lwc1  $f4, 0x30($sp)
/* AB1D00 8003AB60 8FA90034 */  lw    $t1, 0x34($sp)
/* AB1D04 8003AB64 AFA60028 */  sw    $a2, 0x28($sp)
/* AB1D08 8003AB68 97A50026 */  lhu   $a1, 0x26($sp)
/* AB1D0C 8003AB6C 03282021 */  addu  $a0, $t9, $t0
/* AB1D10 8003AB70 E7A40010 */  swc1  $f4, 0x10($sp)
/* AB1D14 8003AB74 0C00EA57 */  jal   func_8003A95C
/* AB1D18 8003AB78 AFA90014 */   sw    $t1, 0x14($sp)
/* AB1D1C 8003AB7C 10400003 */  beqz  $v0, .L8003AB8C
/* AB1D20 8003AB80 8FA60028 */   lw    $a2, 0x28($sp)
/* AB1D24 8003AB84 1000002F */  b     .L8003AC44
/* AB1D28 8003AB88 24020001 */   li    $v0, 1
.L8003AB8C:
/* AB1D2C 8003AB8C 8FAA0020 */  lw    $t2, 0x20($sp)
/* AB1D30 8003AB90 97AB003A */  lhu   $t3, 0x3a($sp)
/* AB1D34 8003AB94 3401FFFF */  li    $at, 65535
/* AB1D38 8003AB98 95420002 */  lhu   $v0, 2($t2)
/* AB1D3C 8003AB9C 316C0002 */  andi  $t4, $t3, 2
/* AB1D40 8003ABA0 50410012 */  beql  $v0, $at, .L8003ABEC
/* AB1D44 8003ABA4 8FB80020 */   lw    $t8, 0x20($sp)
/* AB1D48 8003ABA8 1580000F */  bnez  $t4, .L8003ABE8
/* AB1D4C 8003ABAC 00027080 */   sll   $t6, $v0, 2
/* AB1D50 8003ABB0 8CCD0048 */  lw    $t5, 0x48($a2)
/* AB1D54 8003ABB4 C7A60030 */  lwc1  $f6, 0x30($sp)
/* AB1D58 8003ABB8 8FAF0034 */  lw    $t7, 0x34($sp)
/* AB1D5C 8003ABBC AFA60028 */  sw    $a2, 0x28($sp)
/* AB1D60 8003ABC0 97A50026 */  lhu   $a1, 0x26($sp)
/* AB1D64 8003ABC4 8FA7002C */  lw    $a3, 0x2c($sp)
/* AB1D68 8003ABC8 01AE2021 */  addu  $a0, $t5, $t6
/* AB1D6C 8003ABCC E7A60010 */  swc1  $f6, 0x10($sp)
/* AB1D70 8003ABD0 0C00EA57 */  jal   func_8003A95C
/* AB1D74 8003ABD4 AFAF0014 */   sw    $t7, 0x14($sp)
/* AB1D78 8003ABD8 10400003 */  beqz  $v0, .L8003ABE8
/* AB1D7C 8003ABDC 8FA60028 */   lw    $a2, 0x28($sp)
/* AB1D80 8003ABE0 10000018 */  b     .L8003AC44
/* AB1D84 8003ABE4 24020001 */   li    $v0, 1
.L8003ABE8:
/* AB1D88 8003ABE8 8FB80020 */  lw    $t8, 0x20($sp)
.L8003ABEC:
/* AB1D8C 8003ABEC 97B9003A */  lhu   $t9, 0x3a($sp)
/* AB1D90 8003ABF0 3401FFFF */  li    $at, 65535
/* AB1D94 8003ABF4 97020004 */  lhu   $v0, 4($t8)
/* AB1D98 8003ABF8 33280001 */  andi  $t0, $t9, 1
/* AB1D9C 8003ABFC 50410011 */  beql  $v0, $at, .L8003AC44
/* AB1DA0 8003AC00 00001025 */   move  $v0, $zero
/* AB1DA4 8003AC04 1500000E */  bnez  $t0, .L8003AC40
/* AB1DA8 8003AC08 00025080 */   sll   $t2, $v0, 2
/* AB1DAC 8003AC0C 8CC90048 */  lw    $t1, 0x48($a2)
/* AB1DB0 8003AC10 C7A80030 */  lwc1  $f8, 0x30($sp)
/* AB1DB4 8003AC14 8FAB0034 */  lw    $t3, 0x34($sp)
/* AB1DB8 8003AC18 97A50026 */  lhu   $a1, 0x26($sp)
/* AB1DBC 8003AC1C 8FA7002C */  lw    $a3, 0x2c($sp)
/* AB1DC0 8003AC20 012A2021 */  addu  $a0, $t1, $t2
/* AB1DC4 8003AC24 E7A80010 */  swc1  $f8, 0x10($sp)
/* AB1DC8 8003AC28 0C00EA57 */  jal   func_8003A95C
/* AB1DCC 8003AC2C AFAB0014 */   sw    $t3, 0x14($sp)
/* AB1DD0 8003AC30 50400004 */  beql  $v0, $zero, .L8003AC44
/* AB1DD4 8003AC34 00001025 */   move  $v0, $zero
/* AB1DD8 8003AC38 10000002 */  b     .L8003AC44
/* AB1DDC 8003AC3C 24020001 */   li    $v0, 1
.L8003AC40:
/* AB1DE0 8003AC40 00001025 */  move  $v0, $zero
.L8003AC44:
/* AB1DE4 8003AC44 8FBF001C */  lw    $ra, 0x1c($sp)
/* AB1DE8 8003AC48 27BD0020 */  addiu $sp, $sp, 0x20
/* AB1DEC 8003AC4C 03E00008 */  jr    $ra
/* AB1DF0 8003AC50 00000000 */   nop   

