.late_rodata
glabel D_8013A390
    .float 0.01

glabel D_8013A394
    .float 0.01

glabel D_8013A398
    .float 182.04167

glabel D_8013A39C
    .float 0.01

glabel D_8013A3A0
    .float 2730.0

glabel D_8013A3A4
    .float 2730.0

glabel D_8013A3A8
    .float 0.01

.text
glabel Camera_Special5
/* ACD998 800567F8 27BDFF38 */  addiu $sp, $sp, -0xc8
/* ACD99C 800567FC AFB00018 */  sw    $s0, 0x18($sp)
/* ACD9A0 80056800 00808025 */  move  $s0, $a0
/* ACD9A4 80056804 AFBF001C */  sw    $ra, 0x1c($sp)
/* ACD9A8 80056808 0C00B721 */  jal   Player_GetCameraYOffset
/* ACD9AC 8005680C 8C840090 */   lw    $a0, 0x90($a0)
/* ACD9B0 80056810 8603015E */  lh    $v1, 0x15e($s0)
/* ACD9B4 80056814 10600008 */  beqz  $v1, .L80056838
/* ACD9B8 80056818 2401000A */   li    $at, 10
/* ACD9BC 8005681C 10610006 */  beq   $v1, $at, .L80056838
/* ACD9C0 80056820 24010014 */   li    $at, 20
/* ACD9C4 80056824 10610004 */  beq   $v1, $at, .L80056838
/* ACD9C8 80056828 3C0E8016 */   lui   $t6, %hi(gGameInfo) # $t6, 0x8016
/* ACD9CC 8005682C 8DCEFA90 */  lw    $t6, %lo(gGameInfo)($t6)
/* ACD9D0 80056830 85C30314 */  lh    $v1, 0x314($t6)
/* ACD9D4 80056834 10600050 */  beqz  $v1, .L80056978
.L80056838:
/* ACD9D8 80056838 3C0B8016 */   lui   $t3, %hi(gGameInfo) # $t3, 0x8016
/* ACD9DC 8005683C 8D6BFA90 */  lw    $t3, %lo(gGameInfo)($t3)
/* ACD9E0 80056840 3C018014 */  lui   $at, %hi(D_8013A390)
/* ACD9E4 80056844 C428A390 */  lwc1  $f8, %lo(D_8013A390)($at)
/* ACD9E8 80056848 856C01F0 */  lh    $t4, 0x1f0($t3)
/* ACD9EC 8005684C 3C013F80 */  li    $at, 0x3F800000 # 0.000000
/* ACD9F0 80056850 44815000 */  mtc1  $at, $f10
/* ACD9F4 80056854 448C2000 */  mtc1  $t4, $f4
/* ACD9F8 80056858 3C014288 */  li    $at, 0x42880000 # 0.000000
/* ACD9FC 8005685C 44819000 */  mtc1  $at, $f18
/* ACDA00 80056860 468021A0 */  cvt.s.w $f6, $f4
/* ACDA04 80056864 860F0142 */  lh    $t7, 0x142($s0)
/* ACDA08 80056868 3C198012 */  lui   $t9, %hi(sCameraSettings+4)
/* ACDA0C 8005686C 86080144 */  lh    $t0, 0x144($s0)
/* ACDA10 80056870 000FC0C0 */  sll   $t8, $t7, 3
/* ACDA14 80056874 46009103 */  div.s $f4, $f18, $f0
/* ACDA18 80056878 0338C821 */  addu  $t9, $t9, $t8
/* ACDA1C 8005687C 8F39D068 */  lw    $t9, %lo(sCameraSettings+4)($t9)
/* ACDA20 80056880 000848C0 */  sll   $t1, $t0, 3
/* ACDA24 80056884 46083082 */  mul.s $f2, $f6, $f8
/* ACDA28 80056888 03295021 */  addu  $t2, $t9, $t1
/* ACDA2C 8005688C 8D420004 */  lw    $v0, 4($t2)
/* ACDA30 80056890 3C018014 */  lui   $at, %hi(D_8013A394)
/* ACDA34 80056894 C432A394 */  lwc1  $f18, %lo(D_8013A394)($at)
/* ACDA38 80056898 844D0000 */  lh    $t5, ($v0)
/* ACDA3C 8005689C 3C018014 */  lui   $at, %hi(D_8013A398)
/* ACDA40 800568A0 2442001C */  addiu $v0, $v0, 0x1c
/* ACDA44 800568A4 448D4000 */  mtc1  $t5, $f8
/* ACDA48 800568A8 3C0D8016 */  lui   $t5, %hi(gGameInfo)
/* ACDA4C 800568AC 46025400 */  add.s $f16, $f10, $f2
/* ACDA50 800568B0 46041182 */  mul.s $f6, $f2, $f4
/* ACDA54 800568B4 468042A0 */  cvt.s.w $f10, $f8
/* ACDA58 800568B8 46068301 */  sub.s $f12, $f16, $f6
/* ACDA5C 800568BC 46125102 */  mul.s $f4, $f10, $f18
/* ACDA60 800568C0 00000000 */  nop
/* ACDA64 800568C4 46002402 */  mul.s $f16, $f4, $f0
/* ACDA68 800568C8 00000000 */  nop
/* ACDA6C 800568CC 460C8182 */  mul.s $f6, $f16, $f12
/* ACDA70 800568D0 E6060000 */  swc1  $f6, ($s0)
/* ACDA74 800568D4 844EFFE8 */  lh    $t6, -0x18($v0)
/* ACDA78 800568D8 448E4000 */  mtc1  $t6, $f8
/* ACDA7C 800568DC 00000000 */  nop
/* ACDA80 800568E0 468042A0 */  cvt.s.w $f10, $f8
/* ACDA84 800568E4 E60A0004 */  swc1  $f10, 4($s0)
/* ACDA88 800568E8 844FFFEC */  lh    $t7, -0x14($v0)
/* ACDA8C 800568EC 448F9000 */  mtc1  $t7, $f18
/* ACDA90 800568F0 00000000 */  nop
/* ACDA94 800568F4 46809120 */  cvt.s.w $f4, $f18
/* ACDA98 800568F8 E6040008 */  swc1  $f4, 8($s0)
/* ACDA9C 800568FC 8458FFF0 */  lh    $t8, -0x10($v0)
/* ACDAA0 80056900 A6180014 */  sh    $t8, 0x14($s0)
/* ACDAA4 80056904 8448FFF4 */  lh    $t0, -0xc($v0)
/* ACDAA8 80056908 C428A398 */  lwc1  $f8, %lo(D_8013A398)($at)
/* ACDAAC 8005690C 3C013F00 */  li    $at, 0x3F000000 # 0.000000
/* ACDAB0 80056910 44888000 */  mtc1  $t0, $f16
/* ACDAB4 80056914 44819000 */  mtc1  $at, $f18
/* ACDAB8 80056918 3C018014 */  lui   $at, %hi(D_8013A39C)
/* ACDABC 8005691C 468081A0 */  cvt.s.w $f6, $f16
/* ACDAC0 80056920 46083282 */  mul.s $f10, $f6, $f8
/* ACDAC4 80056924 46125100 */  add.s $f4, $f10, $f18
/* ACDAC8 80056928 4600240D */  trunc.w.s $f16, $f4
/* ACDACC 8005692C 44098000 */  mfc1  $t1, $f16
/* ACDAD0 80056930 00000000 */  nop
/* ACDAD4 80056934 A6090016 */  sh    $t1, 0x16($s0)
/* ACDAD8 80056938 844AFFF8 */  lh    $t2, -8($v0)
/* ACDADC 8005693C 448A3000 */  mtc1  $t2, $f6
/* ACDAE0 80056940 00000000 */  nop
/* ACDAE4 80056944 46803220 */  cvt.s.w $f8, $f6
/* ACDAE8 80056948 E608000C */  swc1  $f8, 0xc($s0)
/* ACDAEC 8005694C 844BFFFC */  lh    $t3, -4($v0)
/* ACDAF0 80056950 C424A39C */  lwc1  $f4, %lo(D_8013A39C)($at)
/* ACDAF4 80056954 448B5000 */  mtc1  $t3, $f10
/* ACDAF8 80056958 00000000 */  nop
/* ACDAFC 8005695C 468054A0 */  cvt.s.w $f18, $f10
/* ACDB00 80056960 46049402 */  mul.s $f16, $f18, $f4
/* ACDB04 80056964 E6100010 */  swc1  $f16, 0x10($s0)
/* ACDB08 80056968 844C0000 */  lh    $t4, ($v0)
/* ACDB0C 8005696C A60C0018 */  sh    $t4, 0x18($s0)
/* ACDB10 80056970 8DADFA90 */  lw    $t5, %lo(gGameInfo)($t5)
/* ACDB14 80056974 85A30314 */  lh    $v1, 0x314($t5)
.L80056978:
/* ACDB18 80056978 50600004 */  beql  $v1, $zero, .L8005698C
/* ACDB1C 8005697C 26050050 */   addiu $a1, $s0, 0x50
/* ACDB20 80056980 0C011495 */  jal   Camera_CopyPREGToModeValues
/* ACDB24 80056984 02002025 */   move  $a0, $s0
/* ACDB28 80056988 26050050 */  addiu $a1, $s0, 0x50
.L8005698C:
/* ACDB2C 8005698C 2606005C */  addiu $a2, $s0, 0x5c
/* ACDB30 80056990 AFA60038 */  sw    $a2, 0x38($sp)
/* ACDB34 80056994 AFA50034 */  sw    $a1, 0x34($sp)
/* ACDB38 80056998 0C01F124 */  jal   OLib_Vec3fDiffToVecSphGeo
/* ACDB3C 8005699C 27A40064 */   addiu $a0, $sp, 0x64
/* ACDB40 800569A0 26060074 */  addiu $a2, $s0, 0x74
/* ACDB44 800569A4 AFA60030 */  sw    $a2, 0x30($sp)
/* ACDB48 800569A8 27A4005C */  addiu $a0, $sp, 0x5c
/* ACDB4C 800569AC 0C01F124 */  jal   OLib_Vec3fDiffToVecSphGeo
/* ACDB50 800569B0 8FA50034 */   lw    $a1, 0x34($sp)
/* ACDB54 800569B4 27A400A8 */  addiu $a0, $sp, 0xa8
/* ACDB58 800569B8 0C00BBC5 */  jal   func_8002EF14
/* ACDB5C 800569BC 8E0500A8 */   lw    $a1, 0xa8($s0)
/* ACDB60 800569C0 860E0018 */  lh    $t6, 0x18($s0)
/* ACDB64 800569C4 3C018012 */  lui   $at, %hi(D_8011D3A0) # $at, 0x8012
/* ACDB68 800569C8 2606001C */  addiu $a2, $s0, 0x1c
/* ACDB6C 800569CC AC2ED3A0 */  sw    $t6, %lo(D_8011D3A0)($at)
/* ACDB70 800569D0 8603015E */  lh    $v1, 0x15e($s0)
/* ACDB74 800569D4 54600006 */  bnezl $v1, .L800569F0
/* ACDB78 800569D8 84C20000 */   lh    $v0, ($a2)
/* ACDB7C 800569DC 86180014 */  lh    $t8, 0x14($s0)
/* ACDB80 800569E0 246F0001 */  addiu $t7, $v1, 1
/* ACDB84 800569E4 A60F015E */  sh    $t7, 0x15e($s0)
/* ACDB88 800569E8 A618001C */  sh    $t8, 0x1c($s0)
/* ACDB8C 800569EC 84C20000 */  lh    $v0, ($a2)
.L800569F0:
/* ACDB90 800569F0 18400003 */  blez  $v0, .L80056A00
/* ACDB94 800569F4 2448FFFF */   addiu $t0, $v0, -1
/* ACDB98 800569F8 10000062 */  b     .L80056B84
/* ACDB9C 800569FC A4C80000 */   sh    $t0, ($a2)
.L80056A00:
/* ACDBA0 80056A00 54400061 */  bnezl $v0, .L80056B88
/* ACDBA4 80056A04 02002025 */   move  $a0, $s0
/* ACDBA8 80056A08 8E0300A8 */  lw    $v1, 0xa8($s0)
/* ACDBAC 80056A0C 50600007 */  beql  $v1, $zero, .L80056A2C
/* ACDBB0 80056A10 AE0000A8 */   sw    $zero, 0xa8($s0)
/* ACDBB4 80056A14 8C790130 */  lw    $t9, 0x130($v1)
/* ACDBB8 80056A18 2449FFFF */  addiu $t1, $v0, -1
/* ACDBBC 80056A1C 27A400A8 */  addiu $a0, $sp, 0xa8
/* ACDBC0 80056A20 17200004 */  bnez  $t9, .L80056A34
/* ACDBC4 80056A24 26050094 */   addiu $a1, $s0, 0x94
/* ACDBC8 80056A28 AE0000A8 */  sw    $zero, 0xa8($s0)
.L80056A2C:
/* ACDBCC 80056A2C 10000077 */  b     .L80056C0C
/* ACDBD0 80056A30 24020001 */   li    $v0, 1
.L80056A34:
/* ACDBD4 80056A34 A4C90000 */  sh    $t1, ($a2)
/* ACDBD8 80056A38 0C01EFE4 */  jal   OLib_Vec3fDist
/* ACDBDC 80056A3C AFA5002C */   sw    $a1, 0x2c($sp)
/* ACDBE0 80056A40 C6060008 */  lwc1  $f6, 8($s0)
/* ACDBE4 80056A44 8FA2002C */  lw    $v0, 0x2c($sp)
/* ACDBE8 80056A48 3C0141A0 */  li    $at, 0x41A00000 # 0.000000
/* ACDBEC 80056A4C 4600303C */  c.lt.s $f6, $f0
/* ACDBF0 80056A50 27A4007C */  addiu $a0, $sp, 0x7c
/* ACDBF4 80056A54 4502004C */  bc1fl .L80056B88
/* ACDBF8 80056A58 02002025 */   move  $a0, $s0
/* ACDBFC 80056A5C 844A000E */  lh    $t2, 0xe($v0)
/* ACDC00 80056A60 44814000 */  mtc1  $at, $f8
/* ACDC04 80056A64 27A500A8 */  addiu $a1, $sp, 0xa8
/* ACDC08 80056A68 A7AA0072 */  sh    $t2, 0x72($sp)
/* ACDC0C 80056A6C 844B000C */  lh    $t3, 0xc($v0)
/* ACDC10 80056A70 27A6006C */  addiu $a2, $sp, 0x6c
/* ACDC14 80056A74 E7A8006C */  swc1  $f8, 0x6c($sp)
/* ACDC18 80056A78 000B6023 */  negu  $t4, $t3
/* ACDC1C 80056A7C 0C010F0A */  jal   func_80043C28
/* ACDC20 80056A80 A7AC0070 */   sh    $t4, 0x70($sp)
/* ACDC24 80056A84 02002025 */  move  $a0, $s0
/* ACDC28 80056A88 8FA50034 */  lw    $a1, 0x34($sp)
/* ACDC2C 80056A8C 0C010F46 */  jal   func_80043D18
/* ACDC30 80056A90 27A6007C */   addiu $a2, $sp, 0x7c
/* ACDC34 80056A94 27A4006C */  addiu $a0, $sp, 0x6c
/* ACDC38 80056A98 0C01F0FD */  jal   OLib_Vec3fToVecSphGeo
/* ACDC3C 80056A9C 27A50088 */   addiu $a1, $sp, 0x88
/* ACDC40 80056AA0 8FAD002C */  lw    $t5, 0x2c($sp)
/* ACDC44 80056AA4 87AF0072 */  lh    $t7, 0x72($sp)
/* ACDC48 80056AA8 85AE000E */  lh    $t6, 0xe($t5)
/* ACDC4C 80056AAC 01CFC023 */  subu  $t8, $t6, $t7
/* ACDC50 80056AB0 A7B800A4 */  sh    $t8, 0xa4($sp)
/* ACDC54 80056AB4 C60A0004 */  lwc1  $f10, 4($s0)
/* ACDC58 80056AB8 0C03F66B */  jal   Math_Rand_ZeroOne
/* ACDC5C 80056ABC E7AA0074 */   swc1  $f10, 0x74($sp)
/* ACDC60 80056AC0 87A800A4 */  lh    $t0, 0xa4($sp)
/* ACDC64 80056AC4 8FAC002C */  lw    $t4, 0x2c($sp)
/* ACDC68 80056AC8 3C018014 */  lui   $at, %hi(D_8013A3A4)
/* ACDC6C 80056ACC 0501000E */  bgez  $t0, .L80056B08
/* ACDC70 80056AD0 00000000 */   nop
/* ACDC74 80056AD4 3C018014 */  lui   $at, %hi(D_8013A3A0)
/* ACDC78 80056AD8 C432A3A0 */  lwc1  $f18, %lo(D_8013A3A0)($at)
/* ACDC7C 80056ADC 46120102 */  mul.s $f4, $f0, $f18
/* ACDC80 80056AE0 4600240D */  trunc.w.s $f16, $f4
/* ACDC84 80056AE4 44028000 */  mfc1  $v0, $f16
/* ACDC88 80056AE8 00000000 */  nop
/* ACDC8C 80056AEC 00021400 */  sll   $v0, $v0, 0x10
/* ACDC90 80056AF0 00021403 */  sra   $v0, $v0, 0x10
/* ACDC94 80056AF4 24421553 */  addiu $v0, $v0, 0x1553
/* ACDC98 80056AF8 00021400 */  sll   $v0, $v0, 0x10
/* ACDC9C 80056AFC 00021403 */  sra   $v0, $v0, 0x10
/* ACDCA0 80056B00 1000000B */  b     .L80056B30
/* ACDCA4 80056B04 00021023 */   negu  $v0, $v0
.L80056B08:
/* ACDCA8 80056B08 C426A3A4 */  lwc1  $f6, %lo(D_8013A3A4)($at)
/* ACDCAC 80056B0C 46060202 */  mul.s $f8, $f0, $f6
/* ACDCB0 80056B10 4600428D */  trunc.w.s $f10, $f8
/* ACDCB4 80056B14 44025000 */  mfc1  $v0, $f10
/* ACDCB8 80056B18 00000000 */  nop
/* ACDCBC 80056B1C 00021400 */  sll   $v0, $v0, 0x10
/* ACDCC0 80056B20 00021403 */  sra   $v0, $v0, 0x10
/* ACDCC4 80056B24 24421553 */  addiu $v0, $v0, 0x1553
/* ACDCC8 80056B28 00021400 */  sll   $v0, $v0, 0x10
/* ACDCCC 80056B2C 00021403 */  sra   $v0, $v0, 0x10
.L80056B30:
/* ACDCD0 80056B30 858D000E */  lh    $t5, 0xe($t4)
/* ACDCD4 80056B34 8FA40030 */  lw    $a0, 0x30($sp)
/* ACDCD8 80056B38 27A500A8 */  addiu $a1, $sp, 0xa8
/* ACDCDC 80056B3C 25B88001 */  addiu $t8, $t5, -0x7fff
/* ACDCE0 80056B40 00584021 */  addu  $t0, $v0, $t8
/* ACDCE4 80056B44 A7A8007A */  sh    $t0, 0x7a($sp)
/* ACDCE8 80056B48 86190016 */  lh    $t9, 0x16($s0)
/* ACDCEC 80056B4C 27A60074 */  addiu $a2, $sp, 0x74
/* ACDCF0 80056B50 0C010F0A */  jal   func_80043C28
/* ACDCF4 80056B54 A7B90078 */   sh    $t9, 0x78($sp)
/* ACDCF8 80056B58 8FA90030 */  lw    $t1, 0x30($sp)
/* ACDCFC 80056B5C 8FA60038 */  lw    $a2, 0x38($sp)
/* ACDD00 80056B60 02002025 */  move  $a0, $s0
/* ACDD04 80056B64 8D2C0000 */  lw    $t4, ($t1)
/* ACDD08 80056B68 27A500A8 */  addiu $a1, $sp, 0xa8
/* ACDD0C 80056B6C ACCC0000 */  sw    $t4, ($a2)
/* ACDD10 80056B70 8D2A0004 */  lw    $t2, 4($t1)
/* ACDD14 80056B74 ACCA0004 */  sw    $t2, 4($a2)
/* ACDD18 80056B78 8D2C0008 */  lw    $t4, 8($t1)
/* ACDD1C 80056B7C 0C010FCD */  jal   func_80043F34
/* ACDD20 80056B80 ACCC0008 */   sw    $t4, 8($a2)
.L80056B84:
/* ACDD24 80056B84 02002025 */  move  $a0, $s0
.L80056B88:
/* ACDD28 80056B88 27A5005C */  addiu $a1, $sp, 0x5c
/* ACDD2C 80056B8C 8E060000 */  lw    $a2, ($s0)
/* ACDD30 80056B90 0C0115EA */  jal   func_800457A8
/* ACDD34 80056B94 00003825 */   move  $a3, $zero
/* ACDD38 80056B98 3C0D8016 */  lui   $t5, %hi(gGameInfo) # $t5, 0x8016
/* ACDD3C 80056B9C 8DADFA90 */  lw    $t5, %lo(gGameInfo)($t5)
/* ACDD40 80056BA0 3C018014 */  lui   $at, %hi(D_8013A3A8)
/* ACDD44 80056BA4 C430A3A8 */  lwc1  $f16, %lo(D_8013A3A8)($at)
/* ACDD48 80056BA8 85AE019C */  lh    $t6, 0x19c($t5)
/* ACDD4C 80056BAC C6080100 */  lwc1  $f8, 0x100($s0)
/* ACDD50 80056BB0 C60C000C */  lwc1  $f12, 0xc($s0)
/* ACDD54 80056BB4 448E9000 */  mtc1  $t6, $f18
/* ACDD58 80056BB8 C60E00FC */  lwc1  $f14, 0xfc($s0)
/* ACDD5C 80056BBC 3C073F80 */  lui   $a3, 0x3f80
/* ACDD60 80056BC0 46809120 */  cvt.s.w $f4, $f18
/* ACDD64 80056BC4 46102182 */  mul.s $f6, $f4, $f16
/* ACDD68 80056BC8 00000000 */  nop
/* ACDD6C 80056BCC 46083282 */  mul.s $f10, $f6, $f8
/* ACDD70 80056BD0 44065000 */  mfc1  $a2, $f10
/* ACDD74 80056BD4 0C010E27 */  jal   func_8004389C
/* ACDD78 80056BD8 00000000 */   nop
/* ACDD7C 80056BDC E60000FC */  swc1  $f0, 0xfc($s0)
/* ACDD80 80056BE0 00002025 */  move  $a0, $zero
/* ACDD84 80056BE4 8605015A */  lh    $a1, 0x15a($s0)
/* ACDD88 80056BE8 3C063F00 */  lui   $a2, 0x3f00
/* ACDD8C 80056BEC 0C010E47 */  jal   func_8004391C
/* ACDD90 80056BF0 2407000A */   li    $a3, 10
/* ACDD94 80056BF4 A602015A */  sh    $v0, 0x15a($s0)
/* ACDD98 80056BF8 02002025 */  move  $a0, $s0
/* ACDD9C 80056BFC 0C011429 */  jal   func_800450A4
/* ACDDA0 80056C00 8E050010 */   lw    $a1, 0x10($s0)
/* ACDDA4 80056C04 E6000100 */  swc1  $f0, 0x100($s0)
/* ACDDA8 80056C08 24020001 */  li    $v0, 1
.L80056C0C:
/* ACDDAC 80056C0C 8FBF001C */  lw    $ra, 0x1c($sp)
/* ACDDB0 80056C10 8FB00018 */  lw    $s0, 0x18($sp)
/* ACDDB4 80056C14 27BD00C8 */  addiu $sp, $sp, 0xc8
/* ACDDB8 80056C18 03E00008 */  jr    $ra
/* ACDDBC 80056C1C 00000000 */   nop
