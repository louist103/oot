.late_rodata
glabel D_801464D4
    .float 0.008

glabel D_801464D8
    .float 0.008

.text
glabel Math3D_LineSegMakePerpLineSeg
/* B41A88 800CA8E8 C4900000 */  lwc1  $f16, ($a0)
/* B41A8C 800CA8EC C4A40000 */  lwc1  $f4, ($a1)
/* B41A90 800CA8F0 27BDFF80 */  addiu $sp, $sp, -0x80
/* B41A94 800CA8F4 3C018014 */  lui   $at, %hi(D_801464D4)
/* B41A98 800CA8F8 46102201 */  sub.s $f8, $f4, $f16
/* B41A9C 800CA8FC E7A80074 */  swc1  $f8, 0x74($sp)
/* B41AA0 800CA900 C4A60004 */  lwc1  $f6, 4($a1)
/* B41AA4 800CA904 C4920004 */  lwc1  $f18, 4($a0)
/* B41AA8 800CA908 46123281 */  sub.s $f10, $f6, $f18
/* B41AAC 800CA90C E7AA0070 */  swc1  $f10, 0x70($sp)
/* B41AB0 800CA910 C4840008 */  lwc1  $f4, 8($a0)
/* B41AB4 800CA914 E7A4002C */  swc1  $f4, 0x2c($sp)
/* B41AB8 800CA918 C7A6002C */  lwc1  $f6, 0x2c($sp)
/* B41ABC 800CA91C C4A80008 */  lwc1  $f8, 8($a1)
/* B41AC0 800CA920 46064281 */  sub.s $f10, $f8, $f6
/* B41AC4 800CA924 E7AA006C */  swc1  $f10, 0x6c($sp)
/* B41AC8 800CA928 C4C40000 */  lwc1  $f4, ($a2)
/* B41ACC 800CA92C E7A40028 */  swc1  $f4, 0x28($sp)
/* B41AD0 800CA930 C4CA0004 */  lwc1  $f10, 4($a2)
/* B41AD4 800CA934 C7A60028 */  lwc1  $f6, 0x28($sp)
/* B41AD8 800CA938 C4E80000 */  lwc1  $f8, ($a3)
/* B41ADC 800CA93C E7AA0024 */  swc1  $f10, 0x24($sp)
/* B41AE0 800CA940 C4E40004 */  lwc1  $f4, 4($a3)
/* B41AE4 800CA944 46064081 */  sub.s $f2, $f8, $f6
/* B41AE8 800CA948 C7A80024 */  lwc1  $f8, 0x24($sp)
/* B41AEC 800CA94C C4C60008 */  lwc1  $f6, 8($a2)
/* B41AF0 800CA950 46082301 */  sub.s $f12, $f4, $f8
/* B41AF4 800CA954 E7A60020 */  swc1  $f6, 0x20($sp)
/* B41AF8 800CA958 46021202 */  mul.s $f8, $f2, $f2
/* B41AFC 800CA95C C7A40020 */  lwc1  $f4, 0x20($sp)
/* B41B00 800CA960 C4EA0008 */  lwc1  $f10, 8($a3)
/* B41B04 800CA964 460C6182 */  mul.s $f6, $f12, $f12
/* B41B08 800CA968 E7B00034 */  swc1  $f16, 0x34($sp)
/* B41B0C 800CA96C 46045381 */  sub.s $f14, $f10, $f4
/* B41B10 800CA970 E7B20030 */  swc1  $f18, 0x30($sp)
/* B41B14 800CA974 460E7102 */  mul.s $f4, $f14, $f14
/* B41B18 800CA978 46064280 */  add.s $f10, $f8, $f6
/* B41B1C 800CA97C C42664D4 */  lwc1  $f6, %lo(D_801464D4)($at)
/* B41B20 800CA980 3C013F80 */  li    $at, 0x3F800000 # 0.000000
/* B41B24 800CA984 46045200 */  add.s $f8, $f10, $f4
/* B41B28 800CA988 46004005 */  abs.s $f0, $f8
/* B41B2C 800CA98C E7A8001C */  swc1  $f8, 0x1c($sp)
/* B41B30 800CA990 4606003C */  c.lt.s $f0, $f6
/* B41B34 800CA994 C7A4001C */  lwc1  $f4, 0x1c($sp)
/* B41B38 800CA998 45020004 */  bc1fl .L800CA9AC
/* B41B3C 800CA99C 44815000 */   mtc1  $at, $f10
/* B41B40 800CA9A0 1000007A */  b     .L800CAB8C
/* B41B44 800CA9A4 00001025 */   move  $v0, $zero
/* B41B48 800CA9A8 44815000 */  mtc1  $at, $f10
.L800CA9AC:
/* B41B4C 800CA9AC C7A80074 */  lwc1  $f8, 0x74($sp)
/* B41B50 800CA9B0 3C018014 */  lui   $at, %hi(D_801464D8)
/* B41B54 800CA9B4 46045403 */  div.s $f16, $f10, $f4
/* B41B58 800CA9B8 C7AA0070 */  lwc1  $f10, 0x70($sp)
/* B41B5C 800CA9BC E7A80000 */  swc1  $f8, ($sp)
/* B41B60 800CA9C0 46024182 */  mul.s $f6, $f8, $f2
/* B41B64 800CA9C4 E7AA0004 */  swc1  $f10, 4($sp)
/* B41B68 800CA9C8 460C5102 */  mul.s $f4, $f10, $f12
/* B41B6C 800CA9CC C7AA0024 */  lwc1  $f10, 0x24($sp)
/* B41B70 800CA9D0 46043180 */  add.s $f6, $f6, $f4
/* B41B74 800CA9D4 C7A4006C */  lwc1  $f4, 0x6c($sp)
/* B41B78 800CA9D8 460E2202 */  mul.s $f8, $f4, $f14
/* B41B7C 800CA9DC E7A40008 */  swc1  $f4, 8($sp)
/* B41B80 800CA9E0 C7A40020 */  lwc1  $f4, 0x20($sp)
/* B41B84 800CA9E4 46083180 */  add.s $f6, $f6, $f8
/* B41B88 800CA9E8 C7A80034 */  lwc1  $f8, 0x34($sp)
/* B41B8C 800CA9EC 46103002 */  mul.s $f0, $f6, $f16
/* B41B90 800CA9F0 C7A60028 */  lwc1  $f6, 0x28($sp)
/* B41B94 800CA9F4 46064201 */  sub.s $f8, $f8, $f6
/* B41B98 800CA9F8 C7A60030 */  lwc1  $f6, 0x30($sp)
/* B41B9C 800CA9FC E7A0005C */  swc1  $f0, 0x5c($sp)
/* B41BA0 800CAA00 460A3181 */  sub.s $f6, $f6, $f10
/* B41BA4 800CAA04 C7AA002C */  lwc1  $f10, 0x2c($sp)
/* B41BA8 800CAA08 E7A80018 */  swc1  $f8, 0x18($sp)
/* B41BAC 800CAA0C 46045281 */  sub.s $f10, $f10, $f4
/* B41BB0 800CAA10 46024102 */  mul.s $f4, $f8, $f2
/* B41BB4 800CAA14 E7A60014 */  swc1  $f6, 0x14($sp)
/* B41BB8 800CAA18 E7AA0010 */  swc1  $f10, 0x10($sp)
/* B41BBC 800CAA1C 46066282 */  mul.s $f10, $f12, $f6
/* B41BC0 800CAA20 C7A60010 */  lwc1  $f6, 0x10($sp)
/* B41BC4 800CAA24 460A2200 */  add.s $f8, $f4, $f10
/* B41BC8 800CAA28 46067102 */  mul.s $f4, $f14, $f6
/* B41BCC 800CAA2C C7A60000 */  lwc1  $f6, ($sp)
/* B41BD0 800CAA30 46044280 */  add.s $f10, $f8, $f4
/* B41BD4 800CAA34 46105482 */  mul.s $f18, $f10, $f16
/* B41BD8 800CAA38 C7AA0004 */  lwc1  $f10, 4($sp)
/* B41BDC 800CAA3C 46001202 */  mul.s $f8, $f2, $f0
/* B41BE0 800CAA40 46083101 */  sub.s $f4, $f6, $f8
/* B41BE4 800CAA44 46006182 */  mul.s $f6, $f12, $f0
/* B41BE8 800CAA48 E7A4004C */  swc1  $f4, 0x4c($sp)
/* B41BEC 800CAA4C C7A40008 */  lwc1  $f4, 8($sp)
/* B41BF0 800CAA50 46065201 */  sub.s $f8, $f10, $f6
/* B41BF4 800CAA54 C7AA005C */  lwc1  $f10, 0x5c($sp)
/* B41BF8 800CAA58 460A7182 */  mul.s $f6, $f14, $f10
/* B41BFC 800CAA5C E7A80050 */  swc1  $f8, 0x50($sp)
/* B41C00 800CAA60 C7AA004C */  lwc1  $f10, 0x4c($sp)
/* B41C04 800CAA64 46062201 */  sub.s $f8, $f4, $f6
/* B41C08 800CAA68 C7A4004C */  lwc1  $f4, 0x4c($sp)
/* B41C0C 800CAA6C 46045182 */  mul.s $f6, $f10, $f4
/* B41C10 800CAA70 C7A40050 */  lwc1  $f4, 0x50($sp)
/* B41C14 800CAA74 C7AA0050 */  lwc1  $f10, 0x50($sp)
/* B41C18 800CAA78 E7A80030 */  swc1  $f8, 0x30($sp)
/* B41C1C 800CAA7C 46045282 */  mul.s $f10, $f10, $f4
/* B41C20 800CAA80 460A3100 */  add.s $f4, $f6, $f10
/* B41C24 800CAA84 46084182 */  mul.s $f6, $f8, $f8
/* B41C28 800CAA88 C42A64D8 */  lwc1  $f10, %lo(D_801464D8)($at)
/* B41C2C 800CAA8C 46062400 */  add.s $f16, $f4, $f6
/* B41C30 800CAA90 46008005 */  abs.s $f0, $f16
/* B41C34 800CAA94 E7B0007C */  swc1  $f16, 0x7c($sp)
/* B41C38 800CAA98 460A003C */  c.lt.s $f0, $f10
/* B41C3C 800CAA9C 00000000 */  nop
/* B41C40 800CAAA0 45000003 */  bc1f  .L800CAAB0
/* B41C44 800CAAA4 00000000 */   nop
/* B41C48 800CAAA8 10000038 */  b     .L800CAB8C
/* B41C4C 800CAAAC 00001025 */   move  $v0, $zero
.L800CAAB0:
/* B41C50 800CAAB0 46121102 */  mul.s $f4, $f2, $f18
/* B41C54 800CAAB4 C7A80018 */  lwc1  $f8, 0x18($sp)
/* B41C58 800CAAB8 C7A60014 */  lwc1  $f6, 0x14($sp)
/* B41C5C 800CAABC 46126282 */  mul.s $f10, $f12, $f18
/* B41C60 800CAAC0 8FA20090 */  lw    $v0, 0x90($sp)
/* B41C64 800CAAC4 46044401 */  sub.s $f16, $f8, $f4
/* B41C68 800CAAC8 C7A40010 */  lwc1  $f4, 0x10($sp)
/* B41C6C 800CAACC 460A3201 */  sub.s $f8, $f6, $f10
/* B41C70 800CAAD0 46127182 */  mul.s $f6, $f14, $f18
/* B41C74 800CAAD4 E7A80044 */  swc1  $f8, 0x44($sp)
/* B41C78 800CAAD8 46062281 */  sub.s $f10, $f4, $f6
/* B41C7C 800CAADC C7A40050 */  lwc1  $f4, 0x50($sp)
/* B41C80 800CAAE0 46082182 */  mul.s $f6, $f4, $f8
/* B41C84 800CAAE4 C7A4004C */  lwc1  $f4, 0x4c($sp)
/* B41C88 800CAAE8 E7AA0048 */  swc1  $f10, 0x48($sp)
/* B41C8C 800CAAEC 46102202 */  mul.s $f8, $f4, $f16
/* B41C90 800CAAF0 46064100 */  add.s $f4, $f8, $f6
/* B41C94 800CAAF4 C7A80030 */  lwc1  $f8, 0x30($sp)
/* B41C98 800CAAF8 460A4182 */  mul.s $f6, $f8, $f10
/* B41C9C 800CAAFC 46062200 */  add.s $f8, $f4, $f6
/* B41CA0 800CAB00 C7A4007C */  lwc1  $f4, 0x7c($sp)
/* B41CA4 800CAB04 C7A60074 */  lwc1  $f6, 0x74($sp)
/* B41CA8 800CAB08 46004287 */  neg.s $f10, $f8
/* B41CAC 800CAB0C 46045003 */  div.s $f0, $f10, $f4
/* B41CB0 800CAB10 C7AA0034 */  lwc1  $f10, 0x34($sp)
/* B41CB4 800CAB14 46003202 */  mul.s $f8, $f6, $f0
/* B41CB8 800CAB18 460A4100 */  add.s $f4, $f8, $f10
/* B41CBC 800CAB1C E4440000 */  swc1  $f4, ($v0)
/* B41CC0 800CAB20 C7A60070 */  lwc1  $f6, 0x70($sp)
/* B41CC4 800CAB24 C48A0004 */  lwc1  $f10, 4($a0)
/* B41CC8 800CAB28 8FA30094 */  lw    $v1, 0x94($sp)
/* B41CCC 800CAB2C 46003202 */  mul.s $f8, $f6, $f0
/* B41CD0 800CAB30 460A4100 */  add.s $f4, $f8, $f10
/* B41CD4 800CAB34 E4440004 */  swc1  $f4, 4($v0)
/* B41CD8 800CAB38 C7A6006C */  lwc1  $f6, 0x6c($sp)
/* B41CDC 800CAB3C C48A0008 */  lwc1  $f10, 8($a0)
/* B41CE0 800CAB40 46003202 */  mul.s $f8, $f6, $f0
/* B41CE4 800CAB44 460A4100 */  add.s $f4, $f8, $f10
/* B41CE8 800CAB48 E4440008 */  swc1  $f4, 8($v0)
/* B41CEC 800CAB4C C7A6005C */  lwc1  $f6, 0x5c($sp)
/* B41CF0 800CAB50 C4C40000 */  lwc1  $f4, ($a2)
/* B41CF4 800CAB54 24020001 */  li    $v0, 1
/* B41CF8 800CAB58 46003202 */  mul.s $f8, $f6, $f0
/* B41CFC 800CAB5C 46124400 */  add.s $f16, $f8, $f18
/* B41D00 800CAB60 46101282 */  mul.s $f10, $f2, $f16
/* B41D04 800CAB64 46045180 */  add.s $f6, $f10, $f4
/* B41D08 800CAB68 46106202 */  mul.s $f8, $f12, $f16
/* B41D0C 800CAB6C E4660000 */  swc1  $f6, ($v1)
/* B41D10 800CAB70 C4CA0004 */  lwc1  $f10, 4($a2)
/* B41D14 800CAB74 46107182 */  mul.s $f6, $f14, $f16
/* B41D18 800CAB78 460A4100 */  add.s $f4, $f8, $f10
/* B41D1C 800CAB7C E4640004 */  swc1  $f4, 4($v1)
/* B41D20 800CAB80 C4C80008 */  lwc1  $f8, 8($a2)
/* B41D24 800CAB84 46083280 */  add.s $f10, $f6, $f8
/* B41D28 800CAB88 E46A0008 */  swc1  $f10, 8($v1)
.L800CAB8C:
/* B41D2C 800CAB8C 03E00008 */  jr    $ra
/* B41D30 800CAB90 27BD0080 */   addiu $sp, $sp, 0x80

