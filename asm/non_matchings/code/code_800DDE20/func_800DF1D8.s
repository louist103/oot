.late_rodata
glabel D_801493F8
    .float 262159

.text
glabel func_800DF1D8
/* B56378 800DF1D8 3C018015 */  lui   $at, %hi(D_801493F8)
/* B5637C 800DF1DC C42093F8 */  lwc1  $f0, %lo(D_801493F8)($at)
/* B56380 800DF1E0 27BDFFB0 */  addiu $sp, $sp, -0x50
/* B56384 800DF1E4 27A40014 */  addiu $a0, $sp, 0x14
/* B56388 800DF1E8 46007102 */  mul.s $f4, $f14, $f0
/* B5638C 800DF1EC 27A2002C */  addiu $v0, $sp, 0x2c
/* B56390 800DF1F0 24C30004 */  addiu $v1, $a2, 4
/* B56394 800DF1F4 46006182 */  mul.s $f6, $f12, $f0
/* B56398 800DF1F8 00000000 */  nop   
/* B5639C 800DF1FC 460C7202 */  mul.s $f8, $f14, $f12
/* B563A0 800DF200 E7A4000C */  swc1  $f4, 0xc($sp)
/* B563A4 800DF204 E7A6002C */  swc1  $f6, 0x2c($sp)
/* B563A8 800DF208 46004282 */  mul.s $f10, $f8, $f0
/* B563AC 800DF20C 00000000 */  nop   
/* B563B0 800DF210 460C6402 */  mul.s $f16, $f12, $f12
/* B563B4 800DF214 E7AA0010 */  swc1  $f10, 0x10($sp)
/* B563B8 800DF218 460E8480 */  add.s $f18, $f16, $f14
/* B563BC 800DF21C 46009102 */  mul.s $f4, $f18, $f0
/* B563C0 800DF220 E7A40030 */  swc1  $f4, 0x30($sp)
.L800DF224:
/* B563C4 800DF224 C486FFFC */  lwc1  $f6, -4($a0)
/* B563C8 800DF228 C48AFFF8 */  lwc1  $f10, -8($a0)
/* B563CC 800DF22C 240F0001 */  li    $t7, 1
/* B563D0 800DF230 460C3202 */  mul.s $f8, $f6, $f12
/* B563D4 800DF234 3C014F00 */  li    $at, 0x4F000000 # 0.000000
/* B563D8 800DF238 460A7402 */  mul.s $f16, $f14, $f10
/* B563DC 800DF23C 46104480 */  add.s $f18, $f8, $f16
/* B563E0 800DF240 444EF800 */  cfc1  $t6, $31
/* B563E4 800DF244 44CFF800 */  ctc1  $t7, $31
/* B563E8 800DF248 00000000 */  nop   
/* B563EC 800DF24C 46009124 */  cvt.w.s $f4, $f18
/* B563F0 800DF250 444FF800 */  cfc1  $t7, $31
/* B563F4 800DF254 00000000 */  nop   
/* B563F8 800DF258 31EF0078 */  andi  $t7, $t7, 0x78
/* B563FC 800DF25C 51E00013 */  beql  $t7, $zero, .L800DF2AC
/* B56400 800DF260 440F2000 */   mfc1  $t7, $f4
/* B56404 800DF264 44812000 */  mtc1  $at, $f4
/* B56408 800DF268 240F0001 */  li    $t7, 1
/* B5640C 800DF26C 46049101 */  sub.s $f4, $f18, $f4
/* B56410 800DF270 44CFF800 */  ctc1  $t7, $31
/* B56414 800DF274 00000000 */  nop   
/* B56418 800DF278 46002124 */  cvt.w.s $f4, $f4
/* B5641C 800DF27C 444FF800 */  cfc1  $t7, $31
/* B56420 800DF280 00000000 */  nop   
/* B56424 800DF284 31EF0078 */  andi  $t7, $t7, 0x78
/* B56428 800DF288 15E00005 */  bnez  $t7, .L800DF2A0
/* B5642C 800DF28C 00000000 */   nop   
/* B56430 800DF290 440F2000 */  mfc1  $t7, $f4
/* B56434 800DF294 3C018000 */  lui   $at, 0x8000
/* B56438 800DF298 10000007 */  b     .L800DF2B8
/* B5643C 800DF29C 01E17825 */   or    $t7, $t7, $at
.L800DF2A0:
/* B56440 800DF2A0 10000005 */  b     .L800DF2B8
/* B56444 800DF2A4 240FFFFF */   li    $t7, -1
/* B56448 800DF2A8 440F2000 */  mfc1  $t7, $f4
.L800DF2AC:
/* B5644C 800DF2AC 00000000 */  nop   
/* B56450 800DF2B0 05E0FFFB */  bltz  $t7, .L800DF2A0
/* B56454 800DF2B4 00000000 */   nop   
.L800DF2B8:
/* B56458 800DF2B8 A46F0000 */  sh    $t7, ($v1)
/* B5645C 800DF2BC 44CEF800 */  ctc1  $t6, $31
/* B56460 800DF2C0 C486001C */  lwc1  $f6, 0x1c($a0)
/* B56464 800DF2C4 C4880018 */  lwc1  $f8, 0x18($a0)
/* B56468 800DF2C8 24190001 */  li    $t9, 1
/* B5646C 800DF2CC 460C3282 */  mul.s $f10, $f6, $f12
/* B56470 800DF2D0 24840004 */  addiu $a0, $a0, 4
/* B56474 800DF2D4 3C014F00 */  li    $at, 0x4F000000 # 0.000000
/* B56478 800DF2D8 46087402 */  mul.s $f16, $f14, $f8
/* B5647C 800DF2DC 46105480 */  add.s $f18, $f10, $f16
/* B56480 800DF2E0 4458F800 */  cfc1  $t8, $31
/* B56484 800DF2E4 44D9F800 */  ctc1  $t9, $31
/* B56488 800DF2E8 00000000 */  nop   
/* B5648C 800DF2EC 46009124 */  cvt.w.s $f4, $f18
/* B56490 800DF2F0 4459F800 */  cfc1  $t9, $31
/* B56494 800DF2F4 00000000 */  nop   
/* B56498 800DF2F8 33390078 */  andi  $t9, $t9, 0x78
/* B5649C 800DF2FC 53200013 */  beql  $t9, $zero, .L800DF34C
/* B564A0 800DF300 44192000 */   mfc1  $t9, $f4
/* B564A4 800DF304 44812000 */  mtc1  $at, $f4
/* B564A8 800DF308 24190001 */  li    $t9, 1
/* B564AC 800DF30C 46049101 */  sub.s $f4, $f18, $f4
/* B564B0 800DF310 44D9F800 */  ctc1  $t9, $31
/* B564B4 800DF314 00000000 */  nop   
/* B564B8 800DF318 46002124 */  cvt.w.s $f4, $f4
/* B564BC 800DF31C 4459F800 */  cfc1  $t9, $31
/* B564C0 800DF320 00000000 */  nop   
/* B564C4 800DF324 33390078 */  andi  $t9, $t9, 0x78
/* B564C8 800DF328 17200005 */  bnez  $t9, .L800DF340
/* B564CC 800DF32C 00000000 */   nop   
/* B564D0 800DF330 44192000 */  mfc1  $t9, $f4
/* B564D4 800DF334 3C018000 */  lui   $at, 0x8000
/* B564D8 800DF338 10000007 */  b     .L800DF358
/* B564DC 800DF33C 0321C825 */   or    $t9, $t9, $at
.L800DF340:
/* B564E0 800DF340 10000005 */  b     .L800DF358
/* B564E4 800DF344 2419FFFF */   li    $t9, -1
/* B564E8 800DF348 44192000 */  mfc1  $t9, $f4
.L800DF34C:
/* B564EC 800DF34C 00000000 */  nop   
/* B564F0 800DF350 0720FFFB */  bltz  $t9, .L800DF340
/* B564F4 800DF354 00000000 */   nop   
.L800DF358:
/* B564F8 800DF358 0082082B */  sltu  $at, $a0, $v0
/* B564FC 800DF35C 44D8F800 */  ctc1  $t8, $31
/* B56500 800DF360 24630002 */  addiu $v1, $v1, 2
/* B56504 800DF364 1420FFAF */  bnez  $at, .L800DF224
/* B56508 800DF368 A479000E */   sh    $t9, 0xe($v1)
/* B5650C 800DF36C 00C01825 */  move  $v1, $a2
/* B56510 800DF370 27A4000C */  addiu $a0, $sp, 0xc
/* B56514 800DF374 27A2004C */  addiu $v0, $sp, 0x4c
.L800DF378:
/* B56518 800DF378 4448F800 */  cfc1  $t0, $31
/* B5651C 800DF37C 24090001 */  li    $t1, 1
/* B56520 800DF380 44C9F800 */  ctc1  $t1, $31
/* B56524 800DF384 C4860000 */  lwc1  $f6, ($a0)
/* B56528 800DF388 3C014F00 */  li    $at, 0x4F000000 # 0.000000
/* B5652C 800DF38C 46003224 */  cvt.w.s $f8, $f6
/* B56530 800DF390 4449F800 */  cfc1  $t1, $31
/* B56534 800DF394 00000000 */  nop   
/* B56538 800DF398 31290078 */  andi  $t1, $t1, 0x78
/* B5653C 800DF39C 51200013 */  beql  $t1, $zero, .L800DF3EC
/* B56540 800DF3A0 44094000 */   mfc1  $t1, $f8
/* B56544 800DF3A4 44814000 */  mtc1  $at, $f8
/* B56548 800DF3A8 24090001 */  li    $t1, 1
/* B5654C 800DF3AC 46083201 */  sub.s $f8, $f6, $f8
/* B56550 800DF3B0 44C9F800 */  ctc1  $t1, $31
/* B56554 800DF3B4 00000000 */  nop   
/* B56558 800DF3B8 46004224 */  cvt.w.s $f8, $f8
/* B5655C 800DF3BC 4449F800 */  cfc1  $t1, $31
/* B56560 800DF3C0 00000000 */  nop   
/* B56564 800DF3C4 31290078 */  andi  $t1, $t1, 0x78
/* B56568 800DF3C8 15200005 */  bnez  $t1, .L800DF3E0
/* B5656C 800DF3CC 00000000 */   nop   
/* B56570 800DF3D0 44094000 */  mfc1  $t1, $f8
/* B56574 800DF3D4 3C018000 */  lui   $at, 0x8000
/* B56578 800DF3D8 10000007 */  b     .L800DF3F8
/* B5657C 800DF3DC 01214825 */   or    $t1, $t1, $at
.L800DF3E0:
/* B56580 800DF3E0 10000005 */  b     .L800DF3F8
/* B56584 800DF3E4 2409FFFF */   li    $t1, -1
/* B56588 800DF3E8 44094000 */  mfc1  $t1, $f8
.L800DF3EC:
/* B5658C 800DF3EC 00000000 */  nop   
/* B56590 800DF3F0 0520FFFB */  bltz  $t1, .L800DF3E0
/* B56594 800DF3F4 00000000 */   nop   
.L800DF3F8:
/* B56598 800DF3F8 44C8F800 */  ctc1  $t0, $31
/* B5659C 800DF3FC 240B0001 */  li    $t3, 1
/* B565A0 800DF400 A4690000 */  sh    $t1, ($v1)
/* B565A4 800DF404 C48A0004 */  lwc1  $f10, 4($a0)
/* B565A8 800DF408 444AF800 */  cfc1  $t2, $31
/* B565AC 800DF40C 44CBF800 */  ctc1  $t3, $31
/* B565B0 800DF410 3C014F00 */  li    $at, 0x4F000000 # 0.000000
/* B565B4 800DF414 46005424 */  cvt.w.s $f16, $f10
/* B565B8 800DF418 444BF800 */  cfc1  $t3, $31
/* B565BC 800DF41C 00000000 */  nop   
/* B565C0 800DF420 316B0078 */  andi  $t3, $t3, 0x78
/* B565C4 800DF424 51600013 */  beql  $t3, $zero, .L800DF474
/* B565C8 800DF428 440B8000 */   mfc1  $t3, $f16
/* B565CC 800DF42C 44818000 */  mtc1  $at, $f16
/* B565D0 800DF430 240B0001 */  li    $t3, 1
/* B565D4 800DF434 46105401 */  sub.s $f16, $f10, $f16
/* B565D8 800DF438 44CBF800 */  ctc1  $t3, $31
/* B565DC 800DF43C 00000000 */  nop   
/* B565E0 800DF440 46008424 */  cvt.w.s $f16, $f16
/* B565E4 800DF444 444BF800 */  cfc1  $t3, $31
/* B565E8 800DF448 00000000 */  nop   
/* B565EC 800DF44C 316B0078 */  andi  $t3, $t3, 0x78
/* B565F0 800DF450 15600005 */  bnez  $t3, .L800DF468
/* B565F4 800DF454 00000000 */   nop   
/* B565F8 800DF458 440B8000 */  mfc1  $t3, $f16
/* B565FC 800DF45C 3C018000 */  lui   $at, 0x8000
/* B56600 800DF460 10000007 */  b     .L800DF480
/* B56604 800DF464 01615825 */   or    $t3, $t3, $at
.L800DF468:
/* B56608 800DF468 10000005 */  b     .L800DF480
/* B5660C 800DF46C 240BFFFF */   li    $t3, -1
/* B56610 800DF470 440B8000 */  mfc1  $t3, $f16
.L800DF474:
/* B56614 800DF474 00000000 */  nop   
/* B56618 800DF478 0560FFFB */  bltz  $t3, .L800DF468
/* B5661C 800DF47C 00000000 */   nop   
.L800DF480:
/* B56620 800DF480 44CAF800 */  ctc1  $t2, $31
/* B56624 800DF484 A46B0002 */  sh    $t3, 2($v1)
/* B56628 800DF488 240D0001 */  li    $t5, 1
/* B5662C 800DF48C C4920008 */  lwc1  $f18, 8($a0)
/* B56630 800DF490 444CF800 */  cfc1  $t4, $31
/* B56634 800DF494 44CDF800 */  ctc1  $t5, $31
/* B56638 800DF498 3C014F00 */  li    $at, 0x4F000000 # 0.000000
/* B5663C 800DF49C 46009124 */  cvt.w.s $f4, $f18
/* B56640 800DF4A0 444DF800 */  cfc1  $t5, $31
/* B56644 800DF4A4 00000000 */  nop   
/* B56648 800DF4A8 31AD0078 */  andi  $t5, $t5, 0x78
/* B5664C 800DF4AC 51A00013 */  beql  $t5, $zero, .L800DF4FC
/* B56650 800DF4B0 440D2000 */   mfc1  $t5, $f4
/* B56654 800DF4B4 44812000 */  mtc1  $at, $f4
/* B56658 800DF4B8 240D0001 */  li    $t5, 1
/* B5665C 800DF4BC 46049101 */  sub.s $f4, $f18, $f4
/* B56660 800DF4C0 44CDF800 */  ctc1  $t5, $31
/* B56664 800DF4C4 00000000 */  nop   
/* B56668 800DF4C8 46002124 */  cvt.w.s $f4, $f4
/* B5666C 800DF4CC 444DF800 */  cfc1  $t5, $31
/* B56670 800DF4D0 00000000 */  nop   
/* B56674 800DF4D4 31AD0078 */  andi  $t5, $t5, 0x78
/* B56678 800DF4D8 15A00005 */  bnez  $t5, .L800DF4F0
/* B5667C 800DF4DC 00000000 */   nop   
/* B56680 800DF4E0 440D2000 */  mfc1  $t5, $f4
/* B56684 800DF4E4 3C018000 */  lui   $at, 0x8000
/* B56688 800DF4E8 10000007 */  b     .L800DF508
/* B5668C 800DF4EC 01A16825 */   or    $t5, $t5, $at
.L800DF4F0:
/* B56690 800DF4F0 10000005 */  b     .L800DF508
/* B56694 800DF4F4 240DFFFF */   li    $t5, -1
/* B56698 800DF4F8 440D2000 */  mfc1  $t5, $f4
.L800DF4FC:
/* B5669C 800DF4FC 00000000 */  nop   
/* B566A0 800DF500 05A0FFFB */  bltz  $t5, .L800DF4F0
/* B566A4 800DF504 00000000 */   nop   
.L800DF508:
/* B566A8 800DF508 44CCF800 */  ctc1  $t4, $31
/* B566AC 800DF50C A46D0004 */  sh    $t5, 4($v1)
/* B566B0 800DF510 240F0001 */  li    $t7, 1
/* B566B4 800DF514 C486000C */  lwc1  $f6, 0xc($a0)
/* B566B8 800DF518 444EF800 */  cfc1  $t6, $31
/* B566BC 800DF51C 44CFF800 */  ctc1  $t7, $31
/* B566C0 800DF520 24840010 */  addiu $a0, $a0, 0x10
/* B566C4 800DF524 3C014F00 */  li    $at, 0x4F000000 # 0.000000
/* B566C8 800DF528 46003224 */  cvt.w.s $f8, $f6
/* B566CC 800DF52C 444FF800 */  cfc1  $t7, $31
/* B566D0 800DF530 00000000 */  nop   
/* B566D4 800DF534 31EF0078 */  andi  $t7, $t7, 0x78
/* B566D8 800DF538 51E00013 */  beql  $t7, $zero, .L800DF588
/* B566DC 800DF53C 440F4000 */   mfc1  $t7, $f8
/* B566E0 800DF540 44814000 */  mtc1  $at, $f8
/* B566E4 800DF544 240F0001 */  li    $t7, 1
/* B566E8 800DF548 46083201 */  sub.s $f8, $f6, $f8
/* B566EC 800DF54C 44CFF800 */  ctc1  $t7, $31
/* B566F0 800DF550 00000000 */  nop   
/* B566F4 800DF554 46004224 */  cvt.w.s $f8, $f8
/* B566F8 800DF558 444FF800 */  cfc1  $t7, $31
/* B566FC 800DF55C 00000000 */  nop   
/* B56700 800DF560 31EF0078 */  andi  $t7, $t7, 0x78
/* B56704 800DF564 15E00005 */  bnez  $t7, .L800DF57C
/* B56708 800DF568 00000000 */   nop   
/* B5670C 800DF56C 440F4000 */  mfc1  $t7, $f8
/* B56710 800DF570 3C018000 */  lui   $at, 0x8000
/* B56714 800DF574 10000007 */  b     .L800DF594
/* B56718 800DF578 01E17825 */   or    $t7, $t7, $at
.L800DF57C:
/* B5671C 800DF57C 10000005 */  b     .L800DF594
/* B56720 800DF580 240FFFFF */   li    $t7, -1
/* B56724 800DF584 440F4000 */  mfc1  $t7, $f8
.L800DF588:
/* B56728 800DF588 00000000 */  nop   
/* B5672C 800DF58C 05E0FFFB */  bltz  $t7, .L800DF57C
/* B56730 800DF590 00000000 */   nop   
.L800DF594:
/* B56734 800DF594 44CEF800 */  ctc1  $t6, $31
/* B56738 800DF598 A46F0006 */  sh    $t7, 6($v1)
/* B5673C 800DF59C 1482FF76 */  bne   $a0, $v0, .L800DF378
/* B56740 800DF5A0 24630008 */   addiu $v1, $v1, 8
/* B56744 800DF5A4 03E00008 */  jr    $ra
/* B56748 800DF5A8 27BD0050 */   addiu $sp, $sp, 0x50

