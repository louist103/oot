.late_rodata
glabel D_8013A1B4
    .float 0.01

glabel D_8013A1B8
    .float 0.01

glabel D_8013A1BC
    .float 182.04167

glabel D_8013A1C0
    .float 182.04167

.text
glabel Camera_Data4
/* AC83F0 80051250 27BDFF78 */  addiu $sp, $sp, -0x88
/* AC83F4 80051254 AFB00014 */  sw    $s0, 0x14($sp)
/* AC83F8 80051258 00808025 */  move  $s0, $a0
/* AC83FC 8005125C AFBF001C */  sw    $ra, 0x1c($sp)
/* AC8400 80051260 AFB10018 */  sw    $s1, 0x18($sp)
/* AC8404 80051264 0C00B721 */  jal   Player_GetCameraYOffset
/* AC8408 80051268 8C840090 */   lw    $a0, 0x90($a0)
/* AC840C 8005126C E7A00038 */  swc1  $f0, 0x38($sp)
/* AC8410 80051270 8602015E */  lh    $v0, 0x15e($s0)
/* AC8414 80051274 2401000A */  li    $at, 10
/* AC8418 80051278 10400008 */  beqz  $v0, .L8005129C
/* AC841C 8005127C 00000000 */   nop
/* AC8420 80051280 10410006 */  beq   $v0, $at, .L8005129C
/* AC8424 80051284 24010014 */   li    $at, 20
/* AC8428 80051288 10410004 */  beq   $v0, $at, .L8005129C
/* AC842C 8005128C 3C0E8016 */   lui   $t6, %hi(gGameInfo) # $t6, 0x8016
/* AC8430 80051290 8DCEFA90 */  lw    $t6, %lo(gGameInfo)($t6)
/* AC8434 80051294 85C20314 */  lh    $v0, 0x314($t6)
/* AC8438 80051298 10400058 */  beqz  $v0, .L800513FC
.L8005129C:
/* AC843C 8005129C 3C018014 */   lui   $at, %hi(D_8013A1B4)
/* AC8440 800512A0 C42CA1B4 */  lwc1  $f12, %lo(D_8013A1B4)($at)
/* AC8444 800512A4 3C014288 */  li    $at, 0x42880000 # 0.000000
/* AC8448 800512A8 44814000 */  mtc1  $at, $f8
/* AC844C 800512AC C7AA0038 */  lwc1  $f10, 0x38($sp)
/* AC8450 800512B0 3C0B8016 */  lui   $t3, %hi(gGameInfo) # $t3, 0x8016
/* AC8454 800512B4 8D6BFA90 */  lw    $t3, %lo(gGameInfo)($t3)
/* AC8458 800512B8 460A4403 */  div.s $f16, $f8, $f10
/* AC845C 800512BC 860F0142 */  lh    $t7, 0x142($s0)
/* AC8460 800512C0 856C01F0 */  lh    $t4, 0x1f0($t3)
/* AC8464 800512C4 3C198012 */  lui   $t9, %hi(sCameraSettings+4)
/* AC8468 800512C8 000FC0C0 */  sll   $t8, $t7, 3
/* AC846C 800512CC 448C2000 */  mtc1  $t4, $f4
/* AC8470 800512D0 86080144 */  lh    $t0, 0x144($s0)
/* AC8474 800512D4 0338C821 */  addu  $t9, $t9, $t8
/* AC8478 800512D8 468021A0 */  cvt.s.w $f6, $f4
/* AC847C 800512DC 8F39D068 */  lw    $t9, %lo(sCameraSettings+4)($t9)
/* AC8480 800512E0 000848C0 */  sll   $t1, $t0, 3
/* AC8484 800512E4 3C013F80 */  li    $at, 0x3F800000 # 0.000000
/* AC8488 800512E8 03295021 */  addu  $t2, $t9, $t1
/* AC848C 800512EC 8D420004 */  lw    $v0, 4($t2)
/* AC8490 800512F0 44812000 */  mtc1  $at, $f4
/* AC8494 800512F4 02002025 */  move  $a0, $s0
/* AC8498 800512F8 844D0000 */  lh    $t5, ($v0)
/* AC849C 800512FC 24420008 */  addiu $v0, $v0, 8
/* AC84A0 80051300 448D4000 */  mtc1  $t5, $f8
/* AC84A4 80051304 460C3002 */  mul.s $f0, $f6, $f12
/* AC84A8 80051308 00000000 */  nop
/* AC84AC 8005130C 46100482 */  mul.s $f18, $f0, $f16
/* AC84B0 80051310 46002180 */  add.s $f6, $f4, $f0
/* AC84B4 80051314 46804420 */  cvt.s.w $f16, $f8
/* AC84B8 80051318 46123081 */  sub.s $f2, $f6, $f18
/* AC84BC 8005131C 460C8102 */  mul.s $f4, $f16, $f12
/* AC84C0 80051320 00000000 */  nop
/* AC84C4 80051324 460A2182 */  mul.s $f6, $f4, $f10
/* AC84C8 80051328 00000000 */  nop
/* AC84CC 8005132C 46023482 */  mul.s $f18, $f6, $f2
/* AC84D0 80051330 E6120000 */  swc1  $f18, ($s0)
/* AC84D4 80051334 844EFFFC */  lh    $t6, -4($v0)
/* AC84D8 80051338 448E4000 */  mtc1  $t6, $f8
/* AC84DC 8005133C 00000000 */  nop
/* AC84E0 80051340 46804420 */  cvt.s.w $f16, $f8
/* AC84E4 80051344 E6100004 */  swc1  $f16, 4($s0)
/* AC84E8 80051348 844F0000 */  lh    $t7, ($v0)
/* AC84EC 8005134C 0C0111DB */  jal   func_8004476C
/* AC84F0 80051350 A60F0008 */   sh    $t7, 8($s0)
/* AC84F4 80051354 2611000C */  addiu $s1, $s0, 0xc
/* AC84F8 80051358 02202025 */  move  $a0, $s1
/* AC84FC 8005135C 00402825 */  move  $a1, $v0
/* AC8500 80051360 0C010EF1 */  jal   func_80043BC4
/* AC8504 80051364 AFA20054 */   sw    $v0, 0x54($sp)
/* AC8508 80051368 8FA60054 */  lw    $a2, 0x54($sp)
/* AC850C 8005136C 2401FFFF */  li    $at, -1
/* AC8510 80051370 88C80006 */  lwl   $t0, 6($a2)
/* AC8514 80051374 98C80009 */  lwr   $t0, 9($a2)
/* AC8518 80051378 AA28000C */  swl   $t0, 0xc($s1)
/* AC851C 8005137C BA28000F */  swr   $t0, 0xf($s1)
/* AC8520 80051380 94C8000A */  lhu   $t0, 0xa($a2)
/* AC8524 80051384 A6280010 */  sh    $t0, 0x10($s1)
/* AC8528 80051388 84C3000C */  lh    $v1, 0xc($a2)
/* AC852C 8005138C 10610010 */  beq   $v1, $at, .L800513D0
/* AC8530 80051390 A623001C */   sh    $v1, 0x1c($s1)
/* AC8534 80051394 8622001C */  lh    $v0, 0x1c($s1)
/* AC8538 80051398 28410169 */  slti  $at, $v0, 0x169
/* AC853C 8005139C 50200007 */  beql  $at, $zero, .L800513BC
/* AC8540 800513A0 44823000 */   mtc1  $v0, $f6
/* AC8544 800513A4 44822000 */  mtc1  $v0, $f4
/* AC8548 800513A8 00000000 */  nop
/* AC854C 800513AC 468022A0 */  cvt.s.w $f10, $f4
/* AC8550 800513B0 10000007 */  b     .L800513D0
/* AC8554 800513B4 E60A0004 */   swc1  $f10, 4($s0)
/* AC8558 800513B8 44823000 */  mtc1  $v0, $f6
.L800513BC:
/* AC855C 800513BC 3C018014 */  lui   $at, %hi(D_8013A1B8)
/* AC8560 800513C0 C428A1B8 */  lwc1  $f8, %lo(D_8013A1B8)($at)
/* AC8564 800513C4 468034A0 */  cvt.s.w $f18, $f6
/* AC8568 800513C8 46089402 */  mul.s $f16, $f18, $f8
/* AC856C 800513CC E6100004 */  swc1  $f16, 4($s0)
.L800513D0:
/* AC8570 800513D0 84D9000E */  lh    $t9, 0xe($a2)
/* AC8574 800513D4 8E2A0000 */  lw    $t2, ($s1)
/* AC8578 800513D8 3C0B8016 */  lui   $t3, %hi(gGameInfo)
/* AC857C 800513DC A639001E */  sh    $t9, 0x1e($s1)
/* AC8580 800513E0 AE0A005C */  sw    $t2, 0x5c($s0)
/* AC8584 800513E4 8E290004 */  lw    $t1, 4($s1)
/* AC8588 800513E8 AE090060 */  sw    $t1, 0x60($s0)
/* AC858C 800513EC 8E2A0008 */  lw    $t2, 8($s1)
/* AC8590 800513F0 AE0A0064 */  sw    $t2, 0x64($s0)
/* AC8594 800513F4 8D6BFA90 */  lw    $t3, %lo(gGameInfo)($t3)
/* AC8598 800513F8 85620314 */  lh    $v0, 0x314($t3)
.L800513FC:
/* AC859C 800513FC 260C005C */  addiu $t4, $s0, 0x5c
/* AC85A0 80051400 AFAC0028 */  sw    $t4, 0x28($sp)
/* AC85A4 80051404 10400003 */  beqz  $v0, .L80051414
/* AC85A8 80051408 2611000C */   addiu $s1, $s0, 0xc
/* AC85AC 8005140C 0C011495 */  jal   Camera_CopyPREGToModeValues
/* AC85B0 80051410 02002025 */   move  $a0, $s0
.L80051414:
/* AC85B4 80051414 860D0008 */  lh    $t5, 8($s0)
/* AC85B8 80051418 3C018012 */  lui   $at, %hi(D_8011D3A0)
/* AC85BC 8005141C 02002025 */  move  $a0, $s0
/* AC85C0 80051420 AC2DD3A0 */  sw    $t5, %lo(D_8011D3A0)($at)
/* AC85C4 80051424 8602015E */  lh    $v0, 0x15e($s0)
/* AC85C8 80051428 14400003 */  bnez  $v0, .L80051438
/* AC85CC 8005142C 244E0001 */   addiu $t6, $v0, 1
/* AC85D0 80051430 0C010ED8 */  jal   func_80043B60
/* AC85D4 80051434 A60E015E */   sh    $t6, 0x15e($s0)
.L80051438:
/* AC85D8 80051438 26050050 */  addiu $a1, $s0, 0x50
/* AC85DC 8005143C AFA50024 */  sw    $a1, 0x24($sp)
/* AC85E0 80051440 27A40064 */  addiu $a0, $sp, 0x64
/* AC85E4 80051444 0C01F124 */  jal   OLib_Vec3fDiffToVecSphRot90
/* AC85E8 80051448 26060074 */   addiu $a2, $s0, 0x74
/* AC85EC 8005144C 02002025 */  move  $a0, $s0
/* AC85F0 80051450 27A50064 */  addiu $a1, $sp, 0x64
/* AC85F4 80051454 8E060000 */  lw    $a2, ($s0)
/* AC85F8 80051458 0C0115EA */  jal   func_800457A8
/* AC85FC 8005145C 00003825 */   move  $a3, $zero
/* AC8600 80051460 27A40074 */  addiu $a0, $sp, 0x74
/* AC8604 80051464 8FA50028 */  lw    $a1, 0x28($sp)
/* AC8608 80051468 0C01F124 */  jal   OLib_Vec3fDiffToVecSphRot90
/* AC860C 8005146C 8FA60024 */   lw    $a2, 0x24($sp)
/* AC8610 80051470 C7A40074 */  lwc1  $f4, 0x74($sp)
/* AC8614 80051474 87AE007A */  lh    $t6, 0x7a($sp)
/* AC8618 80051478 27A6006C */  addiu $a2, $sp, 0x6c
/* AC861C 8005147C E7A4006C */  swc1  $f4, 0x6c($sp)
/* AC8620 80051480 862F001E */  lh    $t7, 0x1e($s1)
/* AC8624 80051484 31F80001 */  andi  $t8, $t7, 1
/* AC8628 80051488 53000012 */  beql  $t8, $zero, .L800514D4
/* AC862C 8005148C A7AE0072 */   sh    $t6, 0x72($sp)
/* AC8630 80051490 8608012C */  lh    $t0, 0x12c($s0)
/* AC8634 80051494 3C018014 */  lui   $at, %hi(D_8013A1BC)
/* AC8638 80051498 C432A1BC */  lwc1  $f18, %lo(D_8013A1BC)($at)
/* AC863C 8005149C 44885000 */  mtc1  $t0, $f10
/* AC8640 800514A0 3C013F00 */  li    $at, 0x3F000000 # 0.000000
/* AC8644 800514A4 44818000 */  mtc1  $at, $f16
/* AC8648 800514A8 468051A0 */  cvt.s.w $f6, $f10
/* AC864C 800514AC 862C000E */  lh    $t4, 0xe($s1)
/* AC8650 800514B0 46123202 */  mul.s $f8, $f6, $f18
/* AC8654 800514B4 46104100 */  add.s $f4, $f8, $f16
/* AC8658 800514B8 4600228D */  trunc.w.s $f10, $f4
/* AC865C 800514BC 440B5000 */  mfc1  $t3, $f10
/* AC8660 800514C0 00000000 */  nop
/* AC8664 800514C4 016C6821 */  addu  $t5, $t3, $t4
/* AC8668 800514C8 10000002 */  b     .L800514D4
/* AC866C 800514CC A7AD0072 */   sh    $t5, 0x72($sp)
/* AC8670 800514D0 A7AE0072 */  sh    $t6, 0x72($sp)
.L800514D4:
/* AC8674 800514D4 862F001E */  lh    $t7, 0x1e($s1)
/* AC8678 800514D8 87AE0078 */  lh    $t6, 0x78($sp)
/* AC867C 800514DC 31F80002 */  andi  $t8, $t7, 2
/* AC8680 800514E0 53000012 */  beql  $t8, $zero, .L8005152C
/* AC8684 800514E4 A7AE0070 */   sh    $t6, 0x70($sp)
/* AC8688 800514E8 8608012E */  lh    $t0, 0x12e($s0)
/* AC868C 800514EC 3C018014 */  lui   $at, %hi(D_8013A1C0)
/* AC8690 800514F0 C428A1C0 */  lwc1  $f8, %lo(D_8013A1C0)($at)
/* AC8694 800514F4 44883000 */  mtc1  $t0, $f6
/* AC8698 800514F8 3C013F00 */  li    $at, 0x3F000000 # 0.000000
/* AC869C 800514FC 44812000 */  mtc1  $at, $f4
/* AC86A0 80051500 468034A0 */  cvt.s.w $f18, $f6
/* AC86A4 80051504 862C000C */  lh    $t4, 0xc($s1)
/* AC86A8 80051508 46089402 */  mul.s $f16, $f18, $f8
/* AC86AC 8005150C 46048280 */  add.s $f10, $f16, $f4
/* AC86B0 80051510 4600518D */  trunc.w.s $f6, $f10
/* AC86B4 80051514 440B3000 */  mfc1  $t3, $f6
/* AC86B8 80051518 00000000 */  nop
/* AC86BC 8005151C 016C6821 */  addu  $t5, $t3, $t4
/* AC86C0 80051520 10000002 */  b     .L8005152C
/* AC86C4 80051524 A7AD0070 */   sh    $t5, 0x70($sp)
/* AC86C8 80051528 A7AE0070 */  sh    $t6, 0x70($sp)
.L8005152C:
/* AC86CC 8005152C 8FA40024 */  lw    $a0, 0x24($sp)
/* AC86D0 80051530 0C010F0A */  jal   func_80043C28
/* AC86D4 80051534 8FA50028 */   lw    $a1, 0x28($sp)
/* AC86D8 80051538 8E180094 */  lw    $t8, 0x94($s0)
/* AC86DC 8005153C 27A40048 */  addiu $a0, $sp, 0x48
/* AC86E0 80051540 AC980000 */  sw    $t8, ($a0)
/* AC86E4 80051544 8E0F0098 */  lw    $t7, 0x98($s0)
/* AC86E8 80051548 AC8F0004 */  sw    $t7, 4($a0)
/* AC86EC 8005154C 8E18009C */  lw    $t8, 0x9c($s0)
/* AC86F0 80051550 AC980008 */  sw    $t8, 8($a0)
/* AC86F4 80051554 C7B2004C */  lwc1  $f18, 0x4c($sp)
/* AC86F8 80051558 C7A80038 */  lwc1  $f8, 0x38($sp)
/* AC86FC 8005155C 8FA50028 */  lw    $a1, 0x28($sp)
/* AC8700 80051560 46089400 */  add.s $f16, $f18, $f8
/* AC8704 80051564 0C01EFE4 */  jal   OLib_Vec3fDist
/* AC8708 80051568 E7B0004C */   swc1  $f16, 0x4c($sp)
/* AC870C 8005156C 44802000 */  mtc1  $zero, $f4
/* AC8710 80051570 C60A0004 */  lwc1  $f10, 4($s0)
/* AC8714 80051574 44803000 */  mtc1  $zero, $f6
/* AC8718 80051578 E60000DC */  swc1  $f0, 0xdc($s0)
/* AC871C 8005157C A600015A */  sh    $zero, 0x15a($s0)
/* AC8720 80051580 E60400D8 */  swc1  $f4, 0xd8($s0)
/* AC8724 80051584 E60A00FC */  swc1  $f10, 0xfc($s0)
/* AC8728 80051588 E6060100 */  swc1  $f6, 0x100($s0)
/* AC872C 8005158C 8FBF001C */  lw    $ra, 0x1c($sp)
/* AC8730 80051590 8FB10018 */  lw    $s1, 0x18($sp)
/* AC8734 80051594 8FB00014 */  lw    $s0, 0x14($sp)
/* AC8738 80051598 27BD0088 */  addiu $sp, $sp, 0x88
/* AC873C 8005159C 03E00008 */  jr    $ra
/* AC8740 800515A0 24020001 */   li    $v0, 1
