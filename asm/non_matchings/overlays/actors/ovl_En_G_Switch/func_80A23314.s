.late_rodata
glabel D_80A23B20
    .float 0.3

.text
glabel func_80A23314
/* 01454 80A23314 27BDFFA8 */  addiu   $sp, $sp, 0xFFA8           ## $sp = FFFFFFA8
/* 01458 80A23318 AFBF0054 */  sw      $ra, 0x0054($sp)           
/* 0145C 80A2331C AFB20050 */  sw      $s2, 0x0050($sp)           
/* 01460 80A23320 AFB1004C */  sw      $s1, 0x004C($sp)           
/* 01464 80A23324 AFB00048 */  sw      $s0, 0x0048($sp)           
/* 01468 80A23328 F7BE0040 */  sdc1    $f30, 0x0040($sp)          
/* 0146C 80A2332C F7BC0038 */  sdc1    $f28, 0x0038($sp)          
/* 01470 80A23330 F7BA0030 */  sdc1    $f26, 0x0030($sp)          
/* 01474 80A23334 F7B80028 */  sdc1    $f24, 0x0028($sp)          
/* 01478 80A23338 F7B60020 */  sdc1    $f22, 0x0020($sp)          
/* 0147C 80A2333C F7B40018 */  sdc1    $f20, 0x0018($sp)          
/* 01480 80A23340 AFA5005C */  sw      $a1, 0x005C($sp)           
/* 01484 80A23344 8483015C */  lh      $v1, 0x015C($a0)           ## 0000015C
/* 01488 80A23348 00808825 */  or      $s1, $a0, $zero            ## $s1 = 00000000
/* 0148C 80A2334C 249001C8 */  addiu   $s0, $a0, 0x01C8           ## $s0 = 000001C8
/* 01490 80A23350 18600054 */  blez    $v1, .L80A234A4            
/* 01494 80A23354 00009025 */  or      $s2, $zero, $zero          ## $s2 = 00000000
/* 01498 80A23358 3C0141F0 */  lui     $at, 0x41F0                ## $at = 41F00000
/* 0149C 80A2335C 4481F000 */  mtc1    $at, $f30                  ## $f30 = 30.00
/* 014A0 80A23360 3C0180A2 */  lui     $at, %hi(D_80A23B20)       ## $at = 80A20000
/* 014A4 80A23364 C43C3B20 */  lwc1    $f28, %lo(D_80A23B20)($at) 
/* 014A8 80A23368 3C014170 */  lui     $at, 0x4170                ## $at = 41700000
/* 014AC 80A2336C 4481D000 */  mtc1    $at, $f26                  ## $f26 = 15.00
/* 014B0 80A23370 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 014B4 80A23374 4481C000 */  mtc1    $at, $f24                  ## $f24 = 10.00
/* 014B8 80A23378 00000000 */  nop
.L80A2337C:
/* 014BC 80A2337C 920E0012 */  lbu     $t6, 0x0012($s0)           ## 000001DA
/* 014C0 80A23380 51C00043 */  beql    $t6, $zero, .L80A23490     
/* 014C4 80A23384 26520001 */  addiu   $s2, $s2, 0x0001           ## $s2 = 00000001
/* 014C8 80A23388 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 014CC 80A2338C 00000000 */  nop
/* 014D0 80A23390 46180182 */  mul.s   $f6, $f0, $f24             
/* 014D4 80A23394 C6040020 */  lwc1    $f4, 0x0020($s0)           ## 000001E8
/* 014D8 80A23398 461A3200 */  add.s   $f8, $f6, $f26             
/* 014DC 80A2339C 46082280 */  add.s   $f10, $f4, $f8             
/* 014E0 80A233A0 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 014E4 80A233A4 E60A0020 */  swc1    $f10, 0x0020($s0)          ## 000001E8
/* 014E8 80A233A8 46180482 */  mul.s   $f18, $f0, $f24            
/* 014EC 80A233AC C6100024 */  lwc1    $f16, 0x0024($s0)          ## 000001EC
/* 014F0 80A233B0 461A9180 */  add.s   $f6, $f18, $f26            
/* 014F4 80A233B4 46068100 */  add.s   $f4, $f16, $f6             
/* 014F8 80A233B8 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 014FC 80A233BC E6040024 */  swc1    $f4, 0x0024($s0)           ## 000001EC
/* 01500 80A233C0 46180282 */  mul.s   $f10, $f0, $f24            
/* 01504 80A233C4 C6080028 */  lwc1    $f8, 0x0028($s0)           ## 000001F0
/* 01508 80A233C8 C6060000 */  lwc1    $f6, 0x0000($s0)           ## 000001C8
/* 0150C 80A233CC C6040014 */  lwc1    $f4, 0x0014($s0)           ## 000001DC
/* 01510 80A233D0 4406E000 */  mfc1    $a2, $f28                  
/* 01514 80A233D4 4407F000 */  mfc1    $a3, $f30                  
/* 01518 80A233D8 46043080 */  add.s   $f2, $f6, $f4              
/* 0151C 80A233DC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 000001C8
/* 01520 80A233E0 461A5480 */  add.s   $f18, $f10, $f26           
/* 01524 80A233E4 C60A0004 */  lwc1    $f10, 0x0004($s0)          ## 000001CC
/* 01528 80A233E8 44051000 */  mfc1    $a1, $f2                   
/* 0152C 80A233EC 46124400 */  add.s   $f16, $f8, $f18            
/* 01530 80A233F0 C6080018 */  lwc1    $f8, 0x0018($s0)           ## 000001E0
/* 01534 80A233F4 C6120008 */  lwc1    $f18, 0x0008($s0)          ## 000001D0
/* 01538 80A233F8 E6100028 */  swc1    $f16, 0x0028($s0)          ## 000001F0
/* 0153C 80A233FC C610001C */  lwc1    $f16, 0x001C($s0)          ## 000001E4
/* 01540 80A23400 46085500 */  add.s   $f20, $f10, $f8            
/* 01544 80A23404 0C01E107 */  jal     Math_SmoothScaleMaxF
              
/* 01548 80A23408 46109580 */  add.s   $f22, $f18, $f16           
/* 0154C 80A2340C 4405A000 */  mfc1    $a1, $f20                  
/* 01550 80A23410 3C063F4C */  lui     $a2, 0x3F4C                ## $a2 = 3F4C0000
/* 01554 80A23414 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3F4CCCCD
/* 01558 80A23418 26040004 */  addiu   $a0, $s0, 0x0004           ## $a0 = 000001CC
/* 0155C 80A2341C 0C01E107 */  jal     Math_SmoothScaleMaxF
              
/* 01560 80A23420 3C07437A */  lui     $a3, 0x437A                ## $a3 = 437A0000
/* 01564 80A23424 4405B000 */  mfc1    $a1, $f22                  
/* 01568 80A23428 4406E000 */  mfc1    $a2, $f28                  
/* 0156C 80A2342C 4407F000 */  mfc1    $a3, $f30                  
/* 01570 80A23430 0C01E107 */  jal     Math_SmoothScaleMaxF
              
/* 01574 80A23434 26040008 */  addiu   $a0, $s0, 0x0008           ## $a0 = 000001D0
/* 01578 80A23438 3C063F66 */  lui     $a2, 0x3F66                ## $a2 = 3F660000
/* 0157C 80A2343C 34C66666 */  ori     $a2, $a2, 0x6666           ## $a2 = 3F666666
/* 01580 80A23440 26040018 */  addiu   $a0, $s0, 0x0018           ## $a0 = 000001E0
/* 01584 80A23444 3C05C1A0 */  lui     $a1, 0xC1A0                ## $a1 = C1A00000
/* 01588 80A23448 0C01E107 */  jal     Math_SmoothScaleMaxF
              
/* 0158C 80A2344C 3C073F80 */  lui     $a3, 0x3F80                ## $a3 = 3F800000
/* 01590 80A23450 8602000E */  lh      $v0, 0x000E($s0)           ## 000001D6
/* 01594 80A23454 10400004 */  beq     $v0, $zero, .L80A23468     
/* 01598 80A23458 244FFFFF */  addiu   $t7, $v0, 0xFFFF           ## $t7 = FFFFFFFF
/* 0159C 80A2345C A60F000E */  sh      $t7, 0x000E($s0)           ## 000001D6
/* 015A0 80A23460 1000000A */  beq     $zero, $zero, .L80A2348C   
/* 015A4 80A23464 8623015C */  lh      $v1, 0x015C($s1)           ## 0000015C
.L80A23468:
/* 015A8 80A23468 8602000C */  lh      $v0, 0x000C($s0)           ## 000001D4
/* 015AC 80A2346C 2841000A */  slti    $at, $v0, 0x000A           
/* 015B0 80A23470 10200004 */  beq     $at, $zero, .L80A23484     
/* 015B4 80A23474 2458FFFE */  addiu   $t8, $v0, 0xFFFE           ## $t8 = FFFFFFFE
/* 015B8 80A23478 A2000012 */  sb      $zero, 0x0012($s0)         ## 000001DA
/* 015BC 80A2347C 10000003 */  beq     $zero, $zero, .L80A2348C   
/* 015C0 80A23480 8623015C */  lh      $v1, 0x015C($s1)           ## 0000015C
.L80A23484:
/* 015C4 80A23484 A618000C */  sh      $t8, 0x000C($s0)           ## 000001D4
/* 015C8 80A23488 8623015C */  lh      $v1, 0x015C($s1)           ## 0000015C
.L80A2348C:
/* 015CC 80A2348C 26520001 */  addiu   $s2, $s2, 0x0001           ## $s2 = 00000002
.L80A23490:
/* 015D0 80A23490 00129400 */  sll     $s2, $s2, 16               
/* 015D4 80A23494 00129403 */  sra     $s2, $s2, 16               
/* 015D8 80A23498 0243082A */  slt     $at, $s2, $v1              
/* 015DC 80A2349C 1420FFB7 */  bne     $at, $zero, .L80A2337C     
/* 015E0 80A234A0 2610002C */  addiu   $s0, $s0, 0x002C           ## $s0 = 000001F4
.L80A234A4:
/* 015E4 80A234A4 8FBF0054 */  lw      $ra, 0x0054($sp)           
/* 015E8 80A234A8 D7B40018 */  ldc1    $f20, 0x0018($sp)          
/* 015EC 80A234AC D7B60020 */  ldc1    $f22, 0x0020($sp)          
/* 015F0 80A234B0 D7B80028 */  ldc1    $f24, 0x0028($sp)          
/* 015F4 80A234B4 D7BA0030 */  ldc1    $f26, 0x0030($sp)          
/* 015F8 80A234B8 D7BC0038 */  ldc1    $f28, 0x0038($sp)          
/* 015FC 80A234BC D7BE0040 */  ldc1    $f30, 0x0040($sp)          
/* 01600 80A234C0 8FB00048 */  lw      $s0, 0x0048($sp)           
/* 01604 80A234C4 8FB1004C */  lw      $s1, 0x004C($sp)           
/* 01608 80A234C8 8FB20050 */  lw      $s2, 0x0050($sp)           
/* 0160C 80A234CC 03E00008 */  jr      $ra                        
/* 01610 80A234D0 27BD0058 */  addiu   $sp, $sp, 0x0058           ## $sp = 00000000