.rdata
glabel D_80B399E4
    .asciz "[32m ☆☆☆☆☆ 石板ＧＯ！ ☆☆☆☆☆ \n[m"
    .balign 4

glabel D_80B39A10
    .asciz "[35m☆☆☆☆☆ 注目座標	       	☆☆☆☆☆ %f\n[m"
    .balign 4

glabel D_80B39A44
    .asciz "[32m ☆☆☆☆☆ こども ☆☆☆☆☆ \n[m"
    .balign 4

glabel D_80B39A6C
    .asciz "[36m ☆☆☆☆☆ おとな ☆☆☆☆☆ \n[m"
    .balign 4

glabel D_80B39A94
    .asciz "[36m☆☆☆☆☆ this->actor.talk_message    ☆☆☆☆☆ %x\n[m"
    .balign 4

glabel D_80B39AD4
    .asciz "[32m ☆☆☆☆☆ 日記帳スタート！ ☆☆☆☆☆ \n[m"
    .balign 4

glabel D_80B39B08
    .asciz "[35m☆☆☆☆☆ 注目座標	       	☆☆☆☆☆ %f\n[m"
    .balign 4

glabel D_80B39B3C
    .asciz "[35m☆☆☆☆☆ 注目座標	       	☆☆☆☆☆ %f\n[m"
    .balign 4

glabel D_80B39B70
    .asciz "[35m☆☆☆☆☆ 注目座標	       	☆☆☆☆☆ %f\n[m"
    .balign 4

glabel D_80B39BA4
    .asciz "[35m☆☆☆☆☆ 注目座標	       	☆☆☆☆☆ %f\n[m"
    .balign 4

glabel D_80B39BD8
    .asciz "[36m☆☆☆☆☆ this->actor.talk_message    ☆☆☆☆☆ %x\n[m"
    .balign 4

.late_rodata
glabel jtbl_80B39D78
.word L80B39228
.word L80B392C0
.word L80B39314
.word L80B39358
.word L80B393B8
.word 0x00000000

.text
glabel func_80B391CC
/* 000DC 80B391CC 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 000E0 80B391D0 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 000E4 80B391D4 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 000E8 80B391D8 84860154 */  lh      $a2, 0x0154($a0)           ## 00000154
/* 000EC 80B391DC 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 000F0 80B391E0 00A03825 */  or      $a3, $a1, $zero            ## $a3 = 00000000
/* 000F4 80B391E4 04C20007 */  bltzl   $a2, .L80B39204            
/* 000F8 80B391E8 860E0150 */  lh      $t6, 0x0150($s0)           ## 00000150
/* 000FC 80B391EC 00E02025 */  or      $a0, $a3, $zero            ## $a0 = 00000000
/* 00100 80B391F0 0C00B2D0 */  jal     Flags_GetSwitch
              
/* 00104 80B391F4 00C02825 */  or      $a1, $a2, $zero            ## $a1 = 00000000
/* 00108 80B391F8 5440008C */  bnel    $v0, $zero, .L80B3942C     
/* 0010C 80B391FC 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00110 80B39200 860E0150 */  lh      $t6, 0x0150($s0)           ## 00000150
.L80B39204:
/* 00114 80B39204 25CFFFFF */  addiu   $t7, $t6, 0xFFFF           ## $t7 = FFFFFFFF
/* 00118 80B39208 2DE10005 */  sltiu   $at, $t7, 0x0005           
/* 0011C 80B3920C 1020007B */  beq     $at, $zero, .L80B393FC     
/* 00120 80B39210 000F7880 */  sll     $t7, $t7,  2               
/* 00124 80B39214 3C0180B4 */  lui     $at, %hi(jtbl_80B39D78)       ## $at = 80B40000
/* 00128 80B39218 002F0821 */  addu    $at, $at, $t7              
/* 0012C 80B3921C 8C2F9D78 */  lw      $t7, %lo(jtbl_80B39D78)($at)  
/* 00130 80B39220 01E00008 */  jr      $t7                        
/* 00134 80B39224 00000000 */  nop
glabel L80B39228
/* 00138 80B39228 3C0480B4 */  lui     $a0, %hi(D_80B399E4)       ## $a0 = 80B40000
/* 0013C 80B3922C 0C00084C */  jal     osSyncPrintf
              
/* 00140 80B39230 248499E4 */  addiu   $a0, $a0, %lo(D_80B399E4)  ## $a0 = 80B399E4
/* 00144 80B39234 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 00148 80B39238 3C0142A0 */  lui     $at, 0x42A0                ## $at = 42A00000
/* 0014C 80B3923C 44812000 */  mtc1    $at, $f4                   ## $f4 = 80.00
/* 00150 80B39240 460001A1 */  cvt.d.s $f6, $f0                   
/* 00154 80B39244 3C0480B4 */  lui     $a0, %hi(D_80B39A10)       ## $a0 = 80B40000
/* 00158 80B39248 44073000 */  mfc1    $a3, $f6                   
/* 0015C 80B3924C 44063800 */  mfc1    $a2, $f7                   
/* 00160 80B39250 24849A10 */  addiu   $a0, $a0, %lo(D_80B39A10)  ## $a0 = 80B39A10
/* 00164 80B39254 E6000160 */  swc1    $f0, 0x0160($s0)           ## 00000160
/* 00168 80B39258 0C00084C */  jal     osSyncPrintf
              
/* 0016C 80B3925C E604015C */  swc1    $f4, 0x015C($s0)           ## 0000015C
/* 00170 80B39260 3C188016 */  lui     $t8, %hi(gSaveContext+4)
/* 00174 80B39264 8F18E664 */  lw      $t8, %lo(gSaveContext+4)($t8)
/* 00178 80B39268 24197040 */  addiu   $t9, $zero, 0x7040         ## $t9 = 00007040
/* 0017C 80B3926C 3C0480B4 */  lui     $a0, %hi(D_80B39A6C)       ## $a0 = 80B40000
/* 00180 80B39270 13000007 */  beq     $t8, $zero, .L80B39290     
/* 00184 80B39274 00000000 */  nop
/* 00188 80B39278 3C0480B4 */  lui     $a0, %hi(D_80B39A44)       ## $a0 = 80B40000
/* 0018C 80B3927C A619010E */  sh      $t9, 0x010E($s0)           ## 0000010E
/* 00190 80B39280 0C00084C */  jal     osSyncPrintf
              
/* 00194 80B39284 24849A44 */  addiu   $a0, $a0, %lo(D_80B39A44)  ## $a0 = 80B39A44
/* 00198 80B39288 10000006 */  beq     $zero, $zero, .L80B392A4   
/* 0019C 80B3928C 24090005 */  addiu   $t1, $zero, 0x0005         ## $t1 = 00000005
.L80B39290:
/* 001A0 80B39290 0C00084C */  jal     osSyncPrintf
              
/* 001A4 80B39294 24849A6C */  addiu   $a0, $a0, %lo(D_80B39A6C)  ## $a0 = FFFF9A6C
/* 001A8 80B39298 24087088 */  addiu   $t0, $zero, 0x7088         ## $t0 = 00007088
/* 001AC 80B3929C A608010E */  sh      $t0, 0x010E($s0)           ## 0000010E
/* 001B0 80B392A0 24090005 */  addiu   $t1, $zero, 0x0005         ## $t1 = 00000005
.L80B392A4:
/* 001B4 80B392A4 3C0480B4 */  lui     $a0, %hi(D_80B39A94)       ## $a0 = 80B40000
/* 001B8 80B392A8 A6090156 */  sh      $t1, 0x0156($s0)           ## 00000156
/* 001BC 80B392AC 24849A94 */  addiu   $a0, $a0, %lo(D_80B39A94)  ## $a0 = 80B39A94
/* 001C0 80B392B0 0C00084C */  jal     osSyncPrintf
              
/* 001C4 80B392B4 9605010E */  lhu     $a1, 0x010E($s0)           ## 0000010E
/* 001C8 80B392B8 10000054 */  beq     $zero, $zero, .L80B3940C   
/* 001CC 80B392BC 00000000 */  nop
glabel L80B392C0
/* 001D0 80B392C0 3C0480B4 */  lui     $a0, %hi(D_80B39AD4)       ## $a0 = 80B40000
/* 001D4 80B392C4 0C00084C */  jal     osSyncPrintf
              
/* 001D8 80B392C8 24849AD4 */  addiu   $a0, $a0, %lo(D_80B39AD4)  ## $a0 = 80B39AD4
/* 001DC 80B392CC 3C0141F0 */  lui     $at, 0x41F0                ## $at = 41F00000
/* 001E0 80B392D0 44810000 */  mtc1    $at, $f0                   ## $f0 = 30.00
/* 001E4 80B392D4 3C014220 */  lui     $at, 0x4220                ## $at = 42200000
/* 001E8 80B392D8 44814000 */  mtc1    $at, $f8                   ## $f8 = 40.00
/* 001EC 80B392DC 460002A1 */  cvt.d.s $f10, $f0                  
/* 001F0 80B392E0 240A5002 */  addiu   $t2, $zero, 0x5002         ## $t2 = 00005002
/* 001F4 80B392E4 240B0004 */  addiu   $t3, $zero, 0x0004         ## $t3 = 00000004
/* 001F8 80B392E8 44075000 */  mfc1    $a3, $f10                  
/* 001FC 80B392EC 44065800 */  mfc1    $a2, $f11                  
/* 00200 80B392F0 3C0480B4 */  lui     $a0, %hi(D_80B39B08)       ## $a0 = 80B40000
/* 00204 80B392F4 A60A010E */  sh      $t2, 0x010E($s0)           ## 0000010E
/* 00208 80B392F8 A60B0156 */  sh      $t3, 0x0156($s0)           ## 00000156
/* 0020C 80B392FC 24849B08 */  addiu   $a0, $a0, %lo(D_80B39B08)  ## $a0 = 80B39B08
/* 00210 80B39300 E6000160 */  swc1    $f0, 0x0160($s0)           ## 00000160
/* 00214 80B39304 0C00084C */  jal     osSyncPrintf
              
/* 00218 80B39308 E608015C */  swc1    $f8, 0x015C($s0)           ## 0000015C
/* 0021C 80B3930C 1000003F */  beq     $zero, $zero, .L80B3940C   
/* 00220 80B39310 00000000 */  nop
glabel L80B39314
/* 00224 80B39314 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 00228 80B39318 3C0142DC */  lui     $at, 0x42DC                ## $at = 42DC0000
/* 0022C 80B3931C 44818000 */  mtc1    $at, $f16                  ## $f16 = 110.00
/* 00230 80B39320 460004A1 */  cvt.d.s $f18, $f0                  
/* 00234 80B39324 240C501E */  addiu   $t4, $zero, 0x501E         ## $t4 = 0000501E
/* 00238 80B39328 240D0005 */  addiu   $t5, $zero, 0x0005         ## $t5 = 00000005
/* 0023C 80B3932C 44079000 */  mfc1    $a3, $f18                  
/* 00240 80B39330 44069800 */  mfc1    $a2, $f19                  
/* 00244 80B39334 3C0480B4 */  lui     $a0, %hi(D_80B39B3C)       ## $a0 = 80B40000
/* 00248 80B39338 A60C010E */  sh      $t4, 0x010E($s0)           ## 0000010E
/* 0024C 80B3933C A60D0156 */  sh      $t5, 0x0156($s0)           ## 00000156
/* 00250 80B39340 24849B3C */  addiu   $a0, $a0, %lo(D_80B39B3C)  ## $a0 = 80B39B3C
/* 00254 80B39344 E6000160 */  swc1    $f0, 0x0160($s0)           ## 00000160
/* 00258 80B39348 0C00084C */  jal     osSyncPrintf
              
/* 0025C 80B3934C E610015C */  swc1    $f16, 0x015C($s0)          ## 0000015C
/* 00260 80B39350 1000002E */  beq     $zero, $zero, .L80B3940C   
/* 00264 80B39354 00000000 */  nop
glabel L80B39358
/* 00268 80B39358 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 0026C 80B3935C 240E5020 */  addiu   $t6, $zero, 0x5020         ## $t6 = 00005020
/* 00270 80B39360 240F0006 */  addiu   $t7, $zero, 0x0006         ## $t7 = 00000006
/* 00274 80B39364 46000121 */  cvt.d.s $f4, $f0                   
/* 00278 80B39368 3C0480B4 */  lui     $a0, %hi(D_80B39B70)       ## $a0 = 80B40000
/* 0027C 80B3936C 44072000 */  mfc1    $a3, $f4                   
/* 00280 80B39370 44062800 */  mfc1    $a2, $f5                   
/* 00284 80B39374 A60E010E */  sh      $t6, 0x010E($s0)           ## 0000010E
/* 00288 80B39378 A60F0156 */  sh      $t7, 0x0156($s0)           ## 00000156
/* 0028C 80B3937C 24849B70 */  addiu   $a0, $a0, %lo(D_80B39B70)  ## $a0 = 80B39B70
/* 00290 80B39380 0C00084C */  jal     osSyncPrintf
              
/* 00294 80B39384 E6000160 */  swc1    $f0, 0x0160($s0)           ## 00000160
/* 00298 80B39388 3C0142F0 */  lui     $at, 0x42F0                ## $at = 42F00000
/* 0029C 80B3938C 44813000 */  mtc1    $at, $f6                   ## $f6 = 120.00
/* 002A0 80B39390 3C188016 */  lui     $t8, %hi(gSaveContext+0xed6)
/* 002A4 80B39394 E606015C */  swc1    $f6, 0x015C($s0)           ## 0000015C
/* 002A8 80B39398 9718F536 */  lhu     $t8, %lo(gSaveContext+0xed6)($t8)
/* 002AC 80B3939C 33192000 */  andi    $t9, $t8, 0x2000           ## $t9 = 00000000
/* 002B0 80B393A0 1320001A */  beq     $t9, $zero, .L80B3940C     
/* 002B4 80B393A4 00000000 */  nop
/* 002B8 80B393A8 0C00B55C */  jal     Actor_Kill
              
/* 002BC 80B393AC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 002C0 80B393B0 10000016 */  beq     $zero, $zero, .L80B3940C   
/* 002C4 80B393B4 00000000 */  nop
glabel L80B393B8
/* 002C8 80B393B8 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 002CC 80B393BC 3C0142DC */  lui     $at, 0x42DC                ## $at = 42DC0000
/* 002D0 80B393C0 44814000 */  mtc1    $at, $f8                   ## $f8 = 110.00
/* 002D4 80B393C4 460002A1 */  cvt.d.s $f10, $f0                  
/* 002D8 80B393C8 2408501F */  addiu   $t0, $zero, 0x501F         ## $t0 = 0000501F
/* 002DC 80B393CC 24090005 */  addiu   $t1, $zero, 0x0005         ## $t1 = 00000005
/* 002E0 80B393D0 44075000 */  mfc1    $a3, $f10                  
/* 002E4 80B393D4 44065800 */  mfc1    $a2, $f11                  
/* 002E8 80B393D8 3C0480B4 */  lui     $a0, %hi(D_80B39BA4)       ## $a0 = 80B40000
/* 002EC 80B393DC A608010E */  sh      $t0, 0x010E($s0)           ## 0000010E
/* 002F0 80B393E0 A6090156 */  sh      $t1, 0x0156($s0)           ## 00000156
/* 002F4 80B393E4 24849BA4 */  addiu   $a0, $a0, %lo(D_80B39BA4)  ## $a0 = 80B39BA4
/* 002F8 80B393E8 E6000160 */  swc1    $f0, 0x0160($s0)           ## 00000160
/* 002FC 80B393EC 0C00084C */  jal     osSyncPrintf
              
/* 00300 80B393F0 E608015C */  swc1    $f8, 0x015C($s0)           ## 0000015C
/* 00304 80B393F4 10000005 */  beq     $zero, $zero, .L80B3940C   
/* 00308 80B393F8 00000000 */  nop
.L80B393FC:
/* 0030C 80B393FC 240A7072 */  addiu   $t2, $zero, 0x7072         ## $t2 = 00007072
/* 00310 80B39400 240B0005 */  addiu   $t3, $zero, 0x0005         ## $t3 = 00000005
/* 00314 80B39404 A60A010E */  sh      $t2, 0x010E($s0)           ## 0000010E
/* 00318 80B39408 A60B0156 */  sh      $t3, 0x0156($s0)           ## 00000156
.L80B3940C:
/* 0031C 80B3940C 3C0480B4 */  lui     $a0, %hi(D_80B39BD8)       ## $a0 = 80B40000
/* 00320 80B39410 24849BD8 */  addiu   $a0, $a0, %lo(D_80B39BD8)  ## $a0 = 80B39BD8
/* 00324 80B39414 0C00084C */  jal     osSyncPrintf
              
/* 00328 80B39418 9605010E */  lhu     $a1, 0x010E($s0)           ## 0000010E
/* 0032C 80B3941C 3C0C80B4 */  lui     $t4, %hi(func_80B3943C)    ## $t4 = 80B40000
/* 00330 80B39420 258C943C */  addiu   $t4, $t4, %lo(func_80B3943C) ## $t4 = 80B3943C
/* 00334 80B39424 AE0C014C */  sw      $t4, 0x014C($s0)           ## 0000014C
/* 00338 80B39428 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80B3942C:
/* 0033C 80B3942C 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 00340 80B39430 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 00344 80B39434 03E00008 */  jr      $ra                        
/* 00348 80B39438 00000000 */  nop