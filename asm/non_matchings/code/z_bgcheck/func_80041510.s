glabel func_80041510
/* AB86B0 80041510 27BDFFD8 */  addiu $sp, $sp, -0x28
/* AB86B4 80041514 97A30042 */  lhu   $v1, 0x42($sp)
/* AB86B8 80041518 AFBF001C */  sw    $ra, 0x1c($sp)
/* AB86BC 8004151C AFA5002C */  sw    $a1, 0x2c($sp)
/* AB86C0 80041520 306E0001 */  andi  $t6, $v1, 1
/* AB86C4 80041524 AFA60030 */  sw    $a2, 0x30($sp)
/* AB86C8 80041528 15C00015 */  bnez  $t6, .L80041580
/* AB86CC 8004152C AFA70034 */   sw    $a3, 0x34($sp)
/* AB86D0 80041530 8FAF003C */  lw    $t7, 0x3c($sp)
/* AB86D4 80041534 C7A40038 */  lwc1  $f4, 0x38($sp)
/* AB86D8 80041538 97A5002E */  lhu   $a1, 0x2e($sp)
/* AB86DC 8004153C 000FC080 */  sll   $t8, $t7, 2
/* AB86E0 80041540 030FC023 */  subu  $t8, $t8, $t7
/* AB86E4 80041544 0018C0C0 */  sll   $t8, $t8, 3
/* AB86E8 80041548 030FC021 */  addu  $t8, $t8, $t7
/* AB86EC 8004154C 0018C080 */  sll   $t8, $t8, 2
/* AB86F0 80041550 0098C821 */  addu  $t9, $a0, $t8
/* AB86F4 80041554 2728005E */  addiu $t0, $t9, 0x5e
/* AB86F8 80041558 AFA80014 */  sw    $t0, 0x14($sp)
/* AB86FC 8004155C AFA30024 */  sw    $v1, 0x24($sp)
/* AB8700 80041560 AFA40028 */  sw    $a0, 0x28($sp)
/* AB8704 80041564 0C0104FE */  jal   func_800413F8
/* AB8708 80041568 E7A40010 */   swc1  $f4, 0x10($sp)
/* AB870C 8004156C 8FA30024 */  lw    $v1, 0x24($sp)
/* AB8710 80041570 10400003 */  beqz  $v0, .L80041580
/* AB8714 80041574 8FA40028 */   lw    $a0, 0x28($sp)
/* AB8718 80041578 1000002F */  b     .L80041638
/* AB871C 8004157C 24020001 */   li    $v0, 1
.L80041580:
/* AB8720 80041580 30690002 */  andi  $t1, $v1, 2
/* AB8724 80041584 15200016 */  bnez  $t1, .L800415E0
/* AB8728 80041588 97A5002E */   lhu   $a1, 0x2e($sp)
/* AB872C 8004158C 8FAA003C */  lw    $t2, 0x3c($sp)
/* AB8730 80041590 C7A60038 */  lwc1  $f6, 0x38($sp)
/* AB8734 80041594 8FA60030 */  lw    $a2, 0x30($sp)
/* AB8738 80041598 000A5880 */  sll   $t3, $t2, 2
/* AB873C 8004159C 016A5823 */  subu  $t3, $t3, $t2
/* AB8740 800415A0 000B58C0 */  sll   $t3, $t3, 3
/* AB8744 800415A4 016A5821 */  addu  $t3, $t3, $t2
/* AB8748 800415A8 000B5880 */  sll   $t3, $t3, 2
/* AB874C 800415AC 008B6021 */  addu  $t4, $a0, $t3
/* AB8750 800415B0 258D0060 */  addiu $t5, $t4, 0x60
/* AB8754 800415B4 AFAD0014 */  sw    $t5, 0x14($sp)
/* AB8758 800415B8 8FA70034 */  lw    $a3, 0x34($sp)
/* AB875C 800415BC AFA30024 */  sw    $v1, 0x24($sp)
/* AB8760 800415C0 AFA40028 */  sw    $a0, 0x28($sp)
/* AB8764 800415C4 0C0104FE */  jal   func_800413F8
/* AB8768 800415C8 E7A60010 */   swc1  $f6, 0x10($sp)
/* AB876C 800415CC 8FA30024 */  lw    $v1, 0x24($sp)
/* AB8770 800415D0 10400003 */  beqz  $v0, .L800415E0
/* AB8774 800415D4 8FA40028 */   lw    $a0, 0x28($sp)
/* AB8778 800415D8 10000017 */  b     .L80041638
/* AB877C 800415DC 24020001 */   li    $v0, 1
.L800415E0:
/* AB8780 800415E0 306E0004 */  andi  $t6, $v1, 4
/* AB8784 800415E4 15C00013 */  bnez  $t6, .L80041634
/* AB8788 800415E8 97A5002E */   lhu   $a1, 0x2e($sp)
/* AB878C 800415EC 8FAF003C */  lw    $t7, 0x3c($sp)
/* AB8790 800415F0 C7A80038 */  lwc1  $f8, 0x38($sp)
/* AB8794 800415F4 8FA60030 */  lw    $a2, 0x30($sp)
/* AB8798 800415F8 000FC080 */  sll   $t8, $t7, 2
/* AB879C 800415FC 030FC023 */  subu  $t8, $t8, $t7
/* AB87A0 80041600 0018C0C0 */  sll   $t8, $t8, 3
/* AB87A4 80041604 030FC021 */  addu  $t8, $t8, $t7
/* AB87A8 80041608 0018C080 */  sll   $t8, $t8, 2
/* AB87AC 8004160C 0098C821 */  addu  $t9, $a0, $t8
/* AB87B0 80041610 27280062 */  addiu $t0, $t9, 0x62
/* AB87B4 80041614 AFA80014 */  sw    $t0, 0x14($sp)
/* AB87B8 80041618 8FA70034 */  lw    $a3, 0x34($sp)
/* AB87BC 8004161C 0C0104FE */  jal   func_800413F8
/* AB87C0 80041620 E7A80010 */   swc1  $f8, 0x10($sp)
/* AB87C4 80041624 50400004 */  beql  $v0, $zero, .L80041638
/* AB87C8 80041628 00001025 */   move  $v0, $zero
/* AB87CC 8004162C 10000002 */  b     .L80041638
/* AB87D0 80041630 24020001 */   li    $v0, 1
.L80041634:
/* AB87D4 80041634 00001025 */  move  $v0, $zero
.L80041638:
/* AB87D8 80041638 8FBF001C */  lw    $ra, 0x1c($sp)
/* AB87DC 8004163C 27BD0028 */  addiu $sp, $sp, 0x28
/* AB87E0 80041640 03E00008 */  jr    $ra
/* AB87E4 80041644 00000000 */   nop   

