.late_rodata
glabel D_80139DA8
    .float 0.00003051851 # (1.0f / 32767.0f)

glabel D_80139DAC
    .float -0.8
    
.text
glabel func_80043F94
/* ABB134 80043F94 27BDFF78 */  addiu $sp, $sp, -0x88
/* ABB138 80043F98 AFBF0034 */  sw    $ra, 0x34($sp)
/* ABB13C 80043F9C AFB10030 */  sw    $s1, 0x30($sp)
/* ABB140 80043FA0 AFB0002C */  sw    $s0, 0x2c($sp)
/* ABB144 80043FA4 AFA40088 */  sw    $a0, 0x88($sp)
/* ABB148 80043FA8 8C8F008C */  lw    $t7, 0x8c($a0)
/* ABB14C 80043FAC 00C08025 */  move  $s0, $a2
/* ABB150 80043FB0 00A08825 */  move  $s1, $a1
/* ABB154 80043FB4 25F807C0 */  addiu $t8, $t7, 0x7c0
/* ABB158 80043FB8 AFB80084 */  sw    $t8, 0x84($sp)
/* ABB15C 80043FBC 0C01F124 */  jal   func_8007C490
/* ABB160 80043FC0 27A40040 */   addiu $a0, $sp, 0x40
/* ABB164 80043FC4 3C014100 */  li    $at, 0x41000000 # 0.000000
/* ABB168 80043FC8 44813000 */  mtc1  $at, $f6
/* ABB16C 80043FCC C7A40040 */  lwc1  $f4, 0x40($sp)
/* ABB170 80043FD0 27A4006C */  addiu $a0, $sp, 0x6c
/* ABB174 80043FD4 02202825 */  move  $a1, $s1
/* ABB178 80043FD8 46062200 */  add.s $f8, $f4, $f6
/* ABB17C 80043FDC 27A60040 */  addiu $a2, $sp, 0x40
/* ABB180 80043FE0 0C010F0A */  jal   func_80043C28
/* ABB184 80043FE4 E7A80040 */   swc1  $f8, 0x40($sp)
/* ABB188 80043FE8 26190018 */  addiu $t9, $s0, 0x18
/* ABB18C 80043FEC 24080001 */  li    $t0, 1
/* ABB190 80043FF0 24090001 */  li    $t1, 1
/* ABB194 80043FF4 240A0001 */  li    $t2, 1
/* ABB198 80043FF8 240BFFFF */  li    $t3, -1
/* ABB19C 80043FFC 260C0024 */  addiu $t4, $s0, 0x24
/* ABB1A0 80044000 AFAC0024 */  sw    $t4, 0x24($sp)
/* ABB1A4 80044004 AFAB0020 */  sw    $t3, 0x20($sp)
/* ABB1A8 80044008 AFAA001C */  sw    $t2, 0x1c($sp)
/* ABB1AC 8004400C AFA90018 */  sw    $t1, 0x18($sp)
/* ABB1B0 80044010 AFA80014 */  sw    $t0, 0x14($sp)
/* ABB1B4 80044014 AFB90010 */  sw    $t9, 0x10($sp)
/* ABB1B8 80044018 8FA40084 */  lw    $a0, 0x84($sp)
/* ABB1BC 8004401C 02202825 */  move  $a1, $s1
/* ABB1C0 80044020 27A6006C */  addiu $a2, $sp, 0x6c
/* ABB1C4 80044024 0C00F75B */  jal   func_8003DD6C
/* ABB1C8 80044028 27A70078 */   addiu $a3, $sp, 0x78
/* ABB1CC 8004402C 1440003A */  bnez  $v0, .L80044118
/* ABB1D0 80044030 27A40060 */   addiu $a0, $sp, 0x60
/* ABB1D4 80044034 02202825 */  move  $a1, $s1
/* ABB1D8 80044038 0C01F03E */  jal   func_8007C0F8
/* ABB1DC 8004403C 02003025 */   move  $a2, $s0
/* ABB1E0 80044040 C7AA0060 */  lwc1  $f10, 0x60($sp)
/* ABB1E4 80044044 8E0E0000 */  lw    $t6, ($s0)
/* ABB1E8 80044048 27A70078 */  addiu $a3, $sp, 0x78
/* ABB1EC 8004404C 46005407 */  neg.s $f16, $f10
/* ABB1F0 80044050 3C0140A0 */  li    $at, 0x40A00000 # 0.000000
/* ABB1F4 80044054 E610000C */  swc1  $f16, 0xc($s0)
/* ABB1F8 80044058 C7B20064 */  lwc1  $f18, 0x64($sp)
/* ABB1FC 8004405C 44818000 */  mtc1  $at, $f16
/* ABB200 80044060 27A5004C */  addiu $a1, $sp, 0x4c
/* ABB204 80044064 46009107 */  neg.s $f4, $f18
/* ABB208 80044068 27A60048 */  addiu $a2, $sp, 0x48
/* ABB20C 8004406C E6040010 */  swc1  $f4, 0x10($s0)
/* ABB210 80044070 C7A60068 */  lwc1  $f6, 0x68($sp)
/* ABB214 80044074 46003207 */  neg.s $f8, $f6
/* ABB218 80044078 E6080014 */  swc1  $f8, 0x14($s0)
/* ABB21C 8004407C ACEE0000 */  sw    $t6, ($a3)
/* ABB220 80044080 8E0D0004 */  lw    $t5, 4($s0)
/* ABB224 80044084 ACED0004 */  sw    $t5, 4($a3)
/* ABB228 80044088 8E0E0008 */  lw    $t6, 8($s0)
/* ABB22C 8004408C ACEE0008 */  sw    $t6, 8($a3)
/* ABB230 80044090 C7AA007C */  lwc1  $f10, 0x7c($sp)
/* ABB234 80044094 8FA40084 */  lw    $a0, 0x84($sp)
/* ABB238 80044098 46105480 */  add.s $f18, $f10, $f16
/* ABB23C 8004409C 0C00F329 */  jal   func_8003CCA4
/* ABB240 800440A0 E7B2007C */   swc1  $f18, 0x7c($sp)
/* ABB244 800440A4 C6020004 */  lwc1  $f2, 4($s0)
/* ABB248 800440A8 3C0140A0 */  li    $at, 0x40A00000 # 0.000000
/* ABB24C 800440AC 44812000 */  mtc1  $at, $f4
/* ABB250 800440B0 46001181 */  sub.s $f6, $f2, $f0
/* ABB254 800440B4 3C013F80 */  li    $at, 0x3F800000 # 0.000000
/* ABB258 800440B8 4606203C */  c.lt.s $f4, $f6
/* ABB25C 800440BC 00000000 */  nop   
/* ABB260 800440C0 4502000F */  bc1fl .L80044100
/* ABB264 800440C4 44818000 */   mtc1  $at, $f16
/* ABB268 800440C8 C6080000 */  lwc1  $f8, ($s0)
/* ABB26C 800440CC C60A000C */  lwc1  $f10, 0xc($s0)
/* ABB270 800440D0 C6120010 */  lwc1  $f18, 0x10($s0)
/* ABB274 800440D4 C6060008 */  lwc1  $f6, 8($s0)
/* ABB278 800440D8 460A4400 */  add.s $f16, $f8, $f10
/* ABB27C 800440DC C6080014 */  lwc1  $f8, 0x14($s0)
/* ABB280 800440E0 00001025 */  move  $v0, $zero
/* ABB284 800440E4 46121100 */  add.s $f4, $f2, $f18
/* ABB288 800440E8 E6100000 */  swc1  $f16, ($s0)
/* ABB28C 800440EC 46083280 */  add.s $f10, $f6, $f8
/* ABB290 800440F0 E6040004 */  swc1  $f4, 4($s0)
/* ABB294 800440F4 1000008D */  b     .L8004432C
/* ABB298 800440F8 E60A0008 */   swc1  $f10, 8($s0)
/* ABB29C 800440FC 44818000 */  mtc1  $at, $f16
.L80044100:
/* ABB2A0 80044100 8FAF004C */  lw    $t7, 0x4c($sp)
/* ABB2A4 80044104 46100480 */  add.s $f18, $f0, $f16
/* ABB2A8 80044108 AE0F0018 */  sw    $t7, 0x18($s0)
/* ABB2AC 8004410C 8FB80048 */  lw    $t8, 0x48($sp)
/* ABB2B0 80044110 E7B2007C */  swc1  $f18, 0x7c($sp)
/* ABB2B4 80044114 AE180024 */  sw    $t8, 0x24($s0)
.L80044118:
/* ABB2B8 80044118 8E020018 */  lw    $v0, 0x18($s0)
/* ABB2BC 8004411C 3C018014 */  lui   $at, %hi(D_80139DA8)
/* ABB2C0 80044120 C4209DA8 */  lwc1  $f0, %lo(D_80139DA8)($at)
/* ABB2C4 80044124 84590008 */  lh    $t9, 8($v0)
/* ABB2C8 80044128 3C013F00 */  li    $at, 0x3F000000 # 0.000000
/* ABB2CC 8004412C 44992000 */  mtc1  $t9, $f4
/* ABB2D0 80044130 00000000 */  nop   
/* ABB2D4 80044134 468021A0 */  cvt.s.w $f6, $f4
/* ABB2D8 80044138 46003202 */  mul.s $f8, $f6, $f0
/* ABB2DC 8004413C E608000C */  swc1  $f8, 0xc($s0)
/* ABB2E0 80044140 8448000A */  lh    $t0, 0xa($v0)
/* ABB2E4 80044144 44885000 */  mtc1  $t0, $f10
/* ABB2E8 80044148 00000000 */  nop   
/* ABB2EC 8004414C 46805420 */  cvt.s.w $f16, $f10
/* ABB2F0 80044150 44815000 */  mtc1  $at, $f10
/* ABB2F4 80044154 3C018014 */  lui   $at, %hi(D_80139DAC)
/* ABB2F8 80044158 46008482 */  mul.s $f18, $f16, $f0
/* ABB2FC 8004415C E6120010 */  swc1  $f18, 0x10($s0)
/* ABB300 80044160 8449000C */  lh    $t1, 0xc($v0)
/* ABB304 80044164 C6020010 */  lwc1  $f2, 0x10($s0)
/* ABB308 80044168 44892000 */  mtc1  $t1, $f4
/* ABB30C 8004416C 4602503C */  c.lt.s $f10, $f2
/* ABB310 80044170 468021A0 */  cvt.s.w $f6, $f4
/* ABB314 80044174 46003202 */  mul.s $f8, $f6, $f0
/* ABB318 80044178 45010007 */  bc1t  .L80044198
/* ABB31C 8004417C E6080014 */   swc1  $f8, 0x14($s0)
/* ABB320 80044180 C4309DAC */  lwc1  $f16, %lo(D_80139DAC)($at)
/* ABB324 80044184 3C028016 */  lui   $v0, %hi(D_8015CE4C) # $v0, 0x8016
/* ABB328 80044188 4610103C */  c.lt.s $f2, $f16
/* ABB32C 8004418C 00000000 */  nop   
/* ABB330 80044190 4500000D */  bc1f  .L800441C8
/* ABB334 80044194 00000000 */   nop   
.L80044198:
/* ABB338 80044198 C612000C */  lwc1  $f18, 0xc($s0)
/* ABB33C 8004419C C7A40078 */  lwc1  $f4, 0x78($sp)
/* ABB340 800441A0 C6100014 */  lwc1  $f16, 0x14($s0)
/* ABB344 800441A4 46049180 */  add.s $f6, $f18, $f4
/* ABB348 800441A8 E6060000 */  swc1  $f6, ($s0)
/* ABB34C 800441AC C7A8007C */  lwc1  $f8, 0x7c($sp)
/* ABB350 800441B0 46081280 */  add.s $f10, $f2, $f8
/* ABB354 800441B4 E60A0004 */  swc1  $f10, 4($s0)
/* ABB358 800441B8 C7B20080 */  lwc1  $f18, 0x80($sp)
/* ABB35C 800441BC 46128100 */  add.s $f4, $f16, $f18
/* ABB360 800441C0 10000059 */  b     .L80044328
/* ABB364 800441C4 E6040008 */   swc1  $f4, 8($s0)
.L800441C8:
/* ABB368 800441C8 8C42CE4C */  lw    $v0, %lo(D_8015CE4C)($v0)
/* ABB36C 800441CC 50400057 */  beql  $v0, $zero, .L8004432C
/* ABB370 800441D0 24020001 */   li    $v0, 1
/* ABB374 800441D4 844A0008 */  lh    $t2, 8($v0)
/* ABB378 800441D8 27AE006C */  addiu $t6, $sp, 0x6c
/* ABB37C 800441DC 27AF0078 */  addiu $t7, $sp, 0x78
/* ABB380 800441E0 448A3000 */  mtc1  $t2, $f6
/* ABB384 800441E4 24180001 */  li    $t8, 1
/* ABB388 800441E8 46803220 */  cvt.s.w $f8, $f6
/* ABB38C 800441EC 46004302 */  mul.s $f12, $f8, $f0
/* ABB390 800441F0 E7AC0054 */  swc1  $f12, 0x54($sp)
/* ABB394 800441F4 844B000A */  lh    $t3, 0xa($v0)
/* ABB398 800441F8 448B8000 */  mtc1  $t3, $f16
/* ABB39C 800441FC 00000000 */  nop   
/* ABB3A0 80044200 468084A0 */  cvt.s.w $f18, $f16
/* ABB3A4 80044204 46009382 */  mul.s $f14, $f18, $f0
/* ABB3A8 80044208 E7AE0058 */  swc1  $f14, 0x58($sp)
/* ABB3AC 8004420C 844C000C */  lh    $t4, 0xc($v0)
/* ABB3B0 80044210 448C3000 */  mtc1  $t4, $f6
/* ABB3B4 80044214 00000000 */  nop   
/* ABB3B8 80044218 46803220 */  cvt.s.w $f8, $f6
/* ABB3BC 8004421C 46004402 */  mul.s $f16, $f8, $f0
/* ABB3C0 80044220 E7B0005C */  swc1  $f16, 0x5c($sp)
/* ABB3C4 80044224 844D000E */  lh    $t5, 0xe($v0)
/* ABB3C8 80044228 44068000 */  mfc1  $a2, $f16
/* ABB3CC 8004422C AFB8001C */  sw    $t8, 0x1c($sp)
/* ABB3D0 80044230 448D9000 */  mtc1  $t5, $f18
/* ABB3D4 80044234 AFAF0018 */  sw    $t7, 0x18($sp)
/* ABB3D8 80044238 AFAE0014 */  sw    $t6, 0x14($sp)
/* ABB3DC 8004423C 468094A0 */  cvt.s.w $f18, $f18
/* ABB3E0 80044240 AFB10010 */  sw    $s1, 0x10($sp)
/* ABB3E4 80044244 44079000 */  mfc1  $a3, $f18
/* ABB3E8 80044248 0C033857 */  jal   func_800CE15C
/* ABB3EC 8004424C 00000000 */   nop   
/* ABB3F0 80044250 1040001B */  beqz  $v0, .L800442C0
/* ABB3F4 80044254 27A40060 */   addiu $a0, $sp, 0x60
/* ABB3F8 80044258 27B90054 */  addiu $t9, $sp, 0x54
/* ABB3FC 8004425C 8F290000 */  lw    $t1, ($t9)
/* ABB400 80044260 3C0A8016 */  lui   $t2, %hi(D_8015CE4C) # $t2, 0x8016
/* ABB404 80044264 AE09000C */  sw    $t1, 0xc($s0)
/* ABB408 80044268 8F280004 */  lw    $t0, 4($t9)
/* ABB40C 8004426C C606000C */  lwc1  $f6, 0xc($s0)
/* ABB410 80044270 AE080010 */  sw    $t0, 0x10($s0)
/* ABB414 80044274 8F290008 */  lw    $t1, 8($t9)
/* ABB418 80044278 C6040010 */  lwc1  $f4, 0x10($s0)
/* ABB41C 8004427C AE090014 */  sw    $t1, 0x14($s0)
/* ABB420 80044280 8D4ACE4C */  lw    $t2, %lo(D_8015CE4C)($t2)
/* ABB424 80044284 AE0A0018 */  sw    $t2, 0x18($s0)
/* ABB428 80044288 8FAB0088 */  lw    $t3, 0x88($sp)
/* ABB42C 8004428C 856C0146 */  lh    $t4, 0x146($t3)
/* ABB430 80044290 AE0C0024 */  sw    $t4, 0x24($s0)
/* ABB434 80044294 C7A80078 */  lwc1  $f8, 0x78($sp)
/* ABB438 80044298 46083280 */  add.s $f10, $f6, $f8
/* ABB43C 8004429C C6060014 */  lwc1  $f6, 0x14($s0)
/* ABB440 800442A0 E60A0000 */  swc1  $f10, ($s0)
/* ABB444 800442A4 C7B0007C */  lwc1  $f16, 0x7c($sp)
/* ABB448 800442A8 46102480 */  add.s $f18, $f4, $f16
/* ABB44C 800442AC E6120004 */  swc1  $f18, 4($s0)
/* ABB450 800442B0 C7A80080 */  lwc1  $f8, 0x80($sp)
/* ABB454 800442B4 46083280 */  add.s $f10, $f6, $f8
/* ABB458 800442B8 1000001B */  b     .L80044328
/* ABB45C 800442BC E60A0008 */   swc1  $f10, 8($s0)
.L800442C0:
/* ABB460 800442C0 02202825 */  move  $a1, $s1
/* ABB464 800442C4 0C01F03E */  jal   func_8007C0F8
/* ABB468 800442C8 02003025 */   move  $a2, $s0
/* ABB46C 800442CC C7A40060 */  lwc1  $f4, 0x60($sp)
/* ABB470 800442D0 00001025 */  move  $v0, $zero
/* ABB474 800442D4 46002407 */  neg.s $f16, $f4
/* ABB478 800442D8 C6040000 */  lwc1  $f4, ($s0)
/* ABB47C 800442DC E610000C */  swc1  $f16, 0xc($s0)
/* ABB480 800442E0 C7B20064 */  lwc1  $f18, 0x64($sp)
/* ABB484 800442E4 C610000C */  lwc1  $f16, 0xc($s0)
/* ABB488 800442E8 46009187 */  neg.s $f6, $f18
/* ABB48C 800442EC 46102480 */  add.s $f18, $f4, $f16
/* ABB490 800442F0 E6060010 */  swc1  $f6, 0x10($s0)
/* ABB494 800442F4 C7A80068 */  lwc1  $f8, 0x68($sp)
/* ABB498 800442F8 C6060004 */  lwc1  $f6, 4($s0)
/* ABB49C 800442FC C6040008 */  lwc1  $f4, 8($s0)
/* ABB4A0 80044300 46004287 */  neg.s $f10, $f8
/* ABB4A4 80044304 C6080010 */  lwc1  $f8, 0x10($s0)
/* ABB4A8 80044308 E60A0014 */  swc1  $f10, 0x14($s0)
/* ABB4AC 8004430C C6100014 */  lwc1  $f16, 0x14($s0)
/* ABB4B0 80044310 E6120000 */  swc1  $f18, ($s0)
/* ABB4B4 80044314 46083280 */  add.s $f10, $f6, $f8
/* ABB4B8 80044318 46102480 */  add.s $f18, $f4, $f16
/* ABB4BC 8004431C E60A0004 */  swc1  $f10, 4($s0)
/* ABB4C0 80044320 10000002 */  b     .L8004432C
/* ABB4C4 80044324 E6120008 */   swc1  $f18, 8($s0)
.L80044328:
/* ABB4C8 80044328 24020001 */  li    $v0, 1
.L8004432C:
/* ABB4CC 8004432C 8FBF0034 */  lw    $ra, 0x34($sp)
/* ABB4D0 80044330 8FB0002C */  lw    $s0, 0x2c($sp)
/* ABB4D4 80044334 8FB10030 */  lw    $s1, 0x30($sp)
/* ABB4D8 80044338 03E00008 */  jr    $ra
/* ABB4DC 8004433C 27BD0088 */   addiu $sp, $sp, 0x88
