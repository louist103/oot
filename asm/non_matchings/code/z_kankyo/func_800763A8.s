glabel func_800763A8
/* AED548 800763A8 27BDFFC0 */  addiu $sp, $sp, -0x40
/* AED54C 800763AC AFB2002C */  sw    $s2, 0x2c($sp)
/* AED550 800763B0 3C128016 */  lui   $s2, %hi(D_8015FDAE)
/* AED554 800763B4 2652FDAE */  addiu $s2, %lo(D_8015FDAE) # addiu $s2, $s2, -0x252
/* AED558 800763B8 92420000 */  lbu   $v0, ($s2)
/* AED55C 800763BC AFBF003C */  sw    $ra, 0x3c($sp)
/* AED560 800763C0 AFB50038 */  sw    $s5, 0x38($sp)
/* AED564 800763C4 AFB40034 */  sw    $s4, 0x34($sp)
/* AED568 800763C8 AFB30030 */  sw    $s3, 0x30($sp)
/* AED56C 800763CC AFB10028 */  sw    $s1, 0x28($sp)
/* AED570 800763D0 AFB00024 */  sw    $s0, 0x24($sp)
/* AED574 800763D4 28410003 */  slti  $at, $v0, 3
/* AED578 800763D8 0080A825 */  move  $s5, $a0
/* AED57C 800763DC 14200004 */  bnez  $at, .L800763F0
/* AED580 800763E0 8C911C44 */   lw    $s1, 0x1c44($a0)
/* AED584 800763E4 244EFFFD */  addiu $t6, $v0, -3
/* AED588 800763E8 10000002 */  b     .L800763F4
/* AED58C 800763EC A24E0000 */   sb    $t6, ($s2)
.L800763F0:
/* AED590 800763F0 A2400000 */  sb    $zero, ($s2)
.L800763F4:
/* AED594 800763F4 92500000 */  lbu   $s0, ($s2)
/* AED598 800763F8 24010001 */  li    $at, 1
/* AED59C 800763FC 02A02025 */  move  $a0, $s5
/* AED5A0 80076400 1601000D */  bne   $s0, $at, .L80076438
/* AED5A4 80076404 02001025 */   move  $v0, $s0
/* AED5A8 80076408 26B007A8 */  addiu $s0, $s5, 0x7a8
/* AED5AC 8007640C 3C068016 */  lui   $a2, %hi(D_8015FD84)
/* AED5B0 80076410 8CC6FD84 */  lw    $a2, %lo(D_8015FD84)($a2)
/* AED5B4 80076414 0C01E9F1 */  jal   Lights_Remove
/* AED5B8 80076418 02002825 */   move  $a1, $s0
/* AED5BC 8007641C 3C068016 */  lui   $a2, %hi(D_8015FD98)
/* AED5C0 80076420 8CC6FD98 */  lw    $a2, %lo(D_8015FD98)($a2)
/* AED5C4 80076424 02A02025 */  move  $a0, $s5
/* AED5C8 80076428 0C01E9F1 */  jal   Lights_Remove
/* AED5CC 8007642C 02002825 */   move  $a1, $s0
/* AED5D0 80076430 10000063 */  b     .L800765C0
/* AED5D4 80076434 00000000 */   nop   
.L80076438:
/* AED5D8 80076438 28410002 */  slti  $at, $v0, 2
/* AED5DC 8007643C 14200060 */  bnez  $at, .L800765C0
/* AED5E0 80076440 00000000 */   nop   
/* AED5E4 80076444 C6240024 */  lwc1  $f4, 0x24($s1)
/* AED5E8 80076448 3C014120 */  li    $at, 0x41200000 # 0.000000
/* AED5EC 8007644C 44810000 */  mtc1  $at, $f0
/* AED5F0 80076450 4600218D */  trunc.w.s $f6, $f4
/* AED5F4 80076454 C6240028 */  lwc1  $f4, 0x28($s1)
/* AED5F8 80076458 3C048016 */  lui   $a0, %hi(D_8015FD88)
/* AED5FC 8007645C 240A00FF */  li    $t2, 255
/* AED600 80076460 44183000 */  mfc1  $t8, $f6
/* AED604 80076464 4600218D */  trunc.w.s $f6, $f4
/* AED608 80076468 C624002C */  lwc1  $f4, 0x2c($s1)
/* AED60C 8007646C 0018CC00 */  sll   $t9, $t8, 0x10
/* AED610 80076470 00194403 */  sra   $t0, $t9, 0x10
/* AED614 80076474 44884000 */  mtc1  $t0, $f8
/* AED618 80076478 440B3000 */  mfc1  $t3, $f6
/* AED61C 8007647C 4600218D */  trunc.w.s $f6, $f4
/* AED620 80076480 AFAA001C */  sw    $t2, 0x1c($sp)
/* AED624 80076484 000B6400 */  sll   $t4, $t3, 0x10
/* AED628 80076488 468042A0 */  cvt.s.w $f10, $f8
/* AED62C 8007648C 000C6C03 */  sra   $t5, $t4, 0x10
/* AED630 80076490 448D4000 */  mtc1  $t5, $f8
/* AED634 80076494 44183000 */  mfc1  $t8, $f6
/* AED638 80076498 AFB00018 */  sw    $s0, 0x18($sp)
/* AED63C 8007649C AFB00014 */  sw    $s0, 0x14($sp)
/* AED640 800764A0 46005401 */  sub.s $f16, $f10, $f0
/* AED644 800764A4 0018CC00 */  sll   $t9, $t8, 0x10
/* AED648 800764A8 00194403 */  sra   $t0, $t9, 0x10
/* AED64C 800764AC 468042A0 */  cvt.s.w $f10, $f8
/* AED650 800764B0 44884000 */  mtc1  $t0, $f8
/* AED654 800764B4 AFB00010 */  sw    $s0, 0x10($sp)
/* AED658 800764B8 2484FD88 */  addiu $a0, %lo(D_8015FD88)
/* AED65C 800764BC 4600848D */  trunc.w.s $f18, $f16
/* AED660 800764C0 46005400 */  add.s $f16, $f10, $f0
/* AED664 800764C4 44059000 */  mfc1  $a1, $f18
/* AED668 800764C8 468042A0 */  cvt.s.w $f10, $f8
/* AED66C 800764CC 00052C00 */  sll   $a1, $a1, 0x10
/* AED670 800764D0 00052C03 */  sra   $a1, $a1, 0x10
/* AED674 800764D4 4600848D */  trunc.w.s $f18, $f16
/* AED678 800764D8 46005401 */  sub.s $f16, $f10, $f0
/* AED67C 800764DC 44069000 */  mfc1  $a2, $f18
/* AED680 800764E0 4600848D */  trunc.w.s $f18, $f16
/* AED684 800764E4 00063400 */  sll   $a2, $a2, 0x10
/* AED688 800764E8 00063403 */  sra   $a2, $a2, 0x10
/* AED68C 800764EC 44079000 */  mfc1  $a3, $f18
/* AED690 800764F0 00000000 */  nop   
/* AED694 800764F4 00073C00 */  sll   $a3, $a3, 0x10
/* AED698 800764F8 0C01E763 */  jal   Lights_InitType0PositionalLight
/* AED69C 800764FC 00073C03 */   sra   $a3, $a3, 0x10
/* AED6A0 80076500 C6240024 */  lwc1  $f4, 0x24($s1)
/* AED6A4 80076504 3C014120 */  li    $at, 0x41200000 # 0.000000
/* AED6A8 80076508 44810000 */  mtc1  $at, $f0
/* AED6AC 8007650C 4600218D */  trunc.w.s $f6, $f4
/* AED6B0 80076510 C6240028 */  lwc1  $f4, 0x28($s1)
/* AED6B4 80076514 92500000 */  lbu   $s0, ($s2)
/* AED6B8 80076518 3C048016 */  lui   $a0, %hi(D_8015FDA0)
/* AED6BC 8007651C 440C3000 */  mfc1  $t4, $f6
/* AED6C0 80076520 4600218D */  trunc.w.s $f6, $f4
/* AED6C4 80076524 C624002C */  lwc1  $f4, 0x2c($s1)
/* AED6C8 80076528 000C6C00 */  sll   $t5, $t4, 0x10
/* AED6CC 8007652C 000D7403 */  sra   $t6, $t5, 0x10
/* AED6D0 80076530 448E4000 */  mtc1  $t6, $f8
/* AED6D4 80076534 44193000 */  mfc1  $t9, $f6
/* AED6D8 80076538 4600218D */  trunc.w.s $f6, $f4
/* AED6DC 8007653C 241800FF */  li    $t8, 255
/* AED6E0 80076540 00194400 */  sll   $t0, $t9, 0x10
/* AED6E4 80076544 468042A0 */  cvt.s.w $f10, $f8
/* AED6E8 80076548 00084C03 */  sra   $t1, $t0, 0x10
/* AED6EC 8007654C 44894000 */  mtc1  $t1, $f8
/* AED6F0 80076550 440C3000 */  mfc1  $t4, $f6
/* AED6F4 80076554 AFB8001C */  sw    $t8, 0x1c($sp)
/* AED6F8 80076558 2484FDA0 */  addiu $a0, %lo(D_8015FDA0)
/* AED6FC 8007655C 46005400 */  add.s $f16, $f10, $f0
/* AED700 80076560 000C6C00 */  sll   $t5, $t4, 0x10
/* AED704 80076564 000D7403 */  sra   $t6, $t5, 0x10
/* AED708 80076568 468042A0 */  cvt.s.w $f10, $f8
/* AED70C 8007656C 448E4000 */  mtc1  $t6, $f8
/* AED710 80076570 AFB00018 */  sw    $s0, 0x18($sp)
/* AED714 80076574 AFB00014 */  sw    $s0, 0x14($sp)
/* AED718 80076578 AFB00010 */  sw    $s0, 0x10($sp)
/* AED71C 8007657C 4600848D */  trunc.w.s $f18, $f16
/* AED720 80076580 46005400 */  add.s $f16, $f10, $f0
/* AED724 80076584 44059000 */  mfc1  $a1, $f18
/* AED728 80076588 468042A0 */  cvt.s.w $f10, $f8
/* AED72C 8007658C 00052C00 */  sll   $a1, $a1, 0x10
/* AED730 80076590 00052C03 */  sra   $a1, $a1, 0x10
/* AED734 80076594 4600848D */  trunc.w.s $f18, $f16
/* AED738 80076598 46005400 */  add.s $f16, $f10, $f0
/* AED73C 8007659C 44069000 */  mfc1  $a2, $f18
/* AED740 800765A0 4600848D */  trunc.w.s $f18, $f16
/* AED744 800765A4 00063400 */  sll   $a2, $a2, 0x10
/* AED748 800765A8 00063403 */  sra   $a2, $a2, 0x10
/* AED74C 800765AC 44079000 */  mfc1  $a3, $f18
/* AED750 800765B0 00000000 */  nop   
/* AED754 800765B4 00073C00 */  sll   $a3, $a3, 0x10
/* AED758 800765B8 0C01E763 */  jal   Lights_InitType0PositionalLight
/* AED75C 800765BC 00073C03 */   sra   $a3, $a3, 0x10
.L800765C0:
/* AED760 800765C0 0C03032E */  jal   func_800C0CB8
/* AED764 800765C4 02A02025 */   move  $a0, $s5
/* AED768 800765C8 10400025 */  beqz  $v0, .L80076660
/* AED76C 800765CC 3C140001 */   lui   $s4, 1
/* AED770 800765D0 3C130001 */  lui   $s3, (0x00010AB6 >> 16) # lui $s3, 1
/* AED774 800765D4 3C120001 */  lui   $s2, (0x00010AB0 >> 16) # lui $s2, 1
/* AED778 800765D8 36520AB0 */  ori   $s2, (0x00010AB0 & 0xFFFF) # ori $s2, $s2, 0xab0
/* AED77C 800765DC 36730AB6 */  ori   $s3, (0x00010AB6 & 0xFFFF) # ori $s3, $s3, 0xab6
/* AED780 800765E0 00008025 */  move  $s0, $zero
/* AED784 800765E4 3C140001 */  lui   $s4, 1
.L800765E8:
/* AED788 800765E8 0010C840 */  sll   $t9, $s0, 1
/* AED78C 800765EC 02B98821 */  addu  $s1, $s5, $t9
/* AED790 800765F0 24080001 */  li    $t0, 1
/* AED794 800765F4 AFA80010 */  sw    $t0, 0x10($sp)
/* AED798 800765F8 02322021 */  addu  $a0, $s1, $s2
/* AED79C 800765FC 00002825 */  move  $a1, $zero
/* AED7A0 80076600 24060005 */  li    $a2, 5
/* AED7A4 80076604 0C01E1A7 */  jal   Math_SmoothScaleMaxMinS
/* AED7A8 80076608 2407000C */   li    $a3, 12
/* AED7AC 8007660C 24090001 */  li    $t1, 1
/* AED7B0 80076610 AFA90010 */  sw    $t1, 0x10($sp)
/* AED7B4 80076614 02332021 */  addu  $a0, $s1, $s3
/* AED7B8 80076618 00002825 */  move  $a1, $zero
/* AED7BC 8007661C 24060005 */  li    $a2, 5
/* AED7C0 80076620 0C01E1A7 */  jal   Math_SmoothScaleMaxMinS
/* AED7C4 80076624 2407000C */   li    $a3, 12
/* AED7C8 80076628 26100001 */  addiu $s0, $s0, 1
/* AED7CC 8007662C 00108400 */  sll   $s0, $s0, 0x10
/* AED7D0 80076630 00108403 */  sra   $s0, $s0, 0x10
/* AED7D4 80076634 2A010003 */  slti  $at, $s0, 3
/* AED7D8 80076638 02345021 */  addu  $t2, $s1, $s4
/* AED7DC 8007663C 1420FFEA */  bnez  $at, .L800765E8
/* AED7E0 80076640 A5400ABC */   sh    $zero, 0xabc($t2)
/* AED7E4 80076644 3C010001 */  lui   $at, 1
/* AED7E8 80076648 00350821 */  addu  $at, $at, $s5
/* AED7EC 8007664C A4200AC4 */  sh    $zero, 0xac4($at)
/* AED7F0 80076650 3C010001 */  lui   $at, 1
/* AED7F4 80076654 00350821 */  addu  $at, $at, $s5
/* AED7F8 80076658 10000011 */  b     .L800766A0
/* AED7FC 8007665C A4200AC2 */   sh    $zero, 0xac2($at)
.L80076660:
/* AED800 80076660 3C010001 */  lui   $at, 1
/* AED804 80076664 00350821 */  addu  $at, $at, $s5
/* AED808 80076668 240B0001 */  li    $t3, 1
/* AED80C 8007666C A02B0B05 */  sb    $t3, 0xb05($at)
/* AED810 80076670 02B41021 */  addu  $v0, $s5, $s4
/* AED814 80076674 A0400B06 */  sb    $zero, 0xb06($v0)
/* AED818 80076678 A0400B07 */  sb    $zero, 0xb07($v0)
/* AED81C 8007667C A0400B08 */  sb    $zero, 0xb08($v0)
/* AED820 80076680 924C0000 */  lbu   $t4, ($s2)
/* AED824 80076684 3C010001 */  lui   $at, 1
/* AED828 80076688 00350821 */  addu  $at, $at, $s5
/* AED82C 8007668C A04C0B09 */  sb    $t4, 0xb09($v0)
/* AED830 80076690 924D0000 */  lbu   $t5, ($s2)
/* AED834 80076694 55A00003 */  bnezl $t5, .L800766A4
/* AED838 80076698 8FBF003C */   lw    $ra, 0x3c($sp)
/* AED83C 8007669C A0200B05 */  sb    $zero, 0xb05($at)
.L800766A0:
/* AED840 800766A0 8FBF003C */  lw    $ra, 0x3c($sp)
.L800766A4:
/* AED844 800766A4 8FB00024 */  lw    $s0, 0x24($sp)
/* AED848 800766A8 8FB10028 */  lw    $s1, 0x28($sp)
/* AED84C 800766AC 8FB2002C */  lw    $s2, 0x2c($sp)
/* AED850 800766B0 8FB30030 */  lw    $s3, 0x30($sp)
/* AED854 800766B4 8FB40034 */  lw    $s4, 0x34($sp)
/* AED858 800766B8 8FB50038 */  lw    $s5, 0x38($sp)
/* AED85C 800766BC 03E00008 */  jr    $ra
/* AED860 800766C0 27BD0040 */   addiu $sp, $sp, 0x40
