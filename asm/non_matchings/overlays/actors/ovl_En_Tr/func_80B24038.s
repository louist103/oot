.late_rodata
glabel D_80B2444C
    .float 0.1

glabel D_80B24450
    .float 0.1

glabel D_80B24454
    .float 0.1

.text
glabel func_80B24038
/* 01348 80B24038 27BDFFC0 */  addiu   $sp, $sp, 0xFFC0           ## $sp = FFFFFFC0
/* 0134C 80B2403C 00067080 */  sll     $t6, $a2,  2               
/* 01350 80B24040 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 01354 80B24044 AFB00020 */  sw      $s0, 0x0020($sp)           
/* 01358 80B24048 F7B40018 */  sdc1    $f20, 0x0018($sp)          
/* 0135C 80B2404C 00AE7821 */  addu    $t7, $a1, $t6              
/* 01360 80B24050 8DE21D8C */  lw      $v0, 0x1D8C($t7)           ## 00001D8C
/* 01364 80B24054 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 01368 80B24058 00A02025 */  or      $a0, $a1, $zero            ## $a0 = 00000000
/* 0136C 80B2405C 8C58000C */  lw      $t8, 0x000C($v0)           ## 0000000C
/* 01370 80B24060 8C590010 */  lw      $t9, 0x0010($v0)           ## 00000010
/* 01374 80B24064 8C480014 */  lw      $t0, 0x0014($v0)           ## 00000014
/* 01378 80B24068 44982000 */  mtc1    $t8, $f4                   ## $f4 = 0.00
/* 0137C 80B2406C 8C490018 */  lw      $t1, 0x0018($v0)           ## 00000018
/* 01380 80B24070 8C4A001C */  lw      $t2, 0x001C($v0)           ## 0000001C
/* 01384 80B24074 468020A0 */  cvt.s.w $f2, $f4                   
/* 01388 80B24078 44993000 */  mtc1    $t9, $f6                   ## $f6 = 0.00
/* 0138C 80B2407C 44884000 */  mtc1    $t0, $f8                   ## $f8 = 0.00
/* 01390 80B24080 44895000 */  mtc1    $t1, $f10                  ## $f10 = 0.00
/* 01394 80B24084 448A2000 */  mtc1    $t2, $f4                   ## $f4 = 0.00
/* 01398 80B24088 46803320 */  cvt.s.w $f12, $f6                  
/* 0139C 80B2408C 8C4B0020 */  lw      $t3, 0x0020($v0)           ## 00000020
/* 013A0 80B24090 E7A20034 */  swc1    $f2, 0x0034($sp)           
/* 013A4 80B24094 00C02825 */  or      $a1, $a2, $zero            ## $a1 = 00000000
/* 013A8 80B24098 448B3000 */  mtc1    $t3, $f6                   ## $f6 = 0.00
/* 013AC 80B2409C 468043A0 */  cvt.s.w $f14, $f8                  
/* 013B0 80B240A0 E7AC0038 */  swc1    $f12, 0x0038($sp)          
/* 013B4 80B240A4 46805420 */  cvt.s.w $f16, $f10                 
/* 013B8 80B240A8 E7AE003C */  swc1    $f14, 0x003C($sp)          
/* 013BC 80B240AC 468024A0 */  cvt.s.w $f18, $f4                  
/* 013C0 80B240B0 E7B00028 */  swc1    $f16, 0x0028($sp)          
/* 013C4 80B240B4 46803520 */  cvt.s.w $f20, $f6                  
/* 013C8 80B240B8 0C2C8FF7 */  jal     func_80B23FDC              
/* 013CC 80B240BC E7B2002C */  swc1    $f18, 0x002C($sp)          
/* 013D0 80B240C0 C7A20034 */  lwc1    $f2, 0x0034($sp)           
/* 013D4 80B240C4 C7B00028 */  lwc1    $f16, 0x0028($sp)          
/* 013D8 80B240C8 C7AC0038 */  lwc1    $f12, 0x0038($sp)          
/* 013DC 80B240CC C7B2002C */  lwc1    $f18, 0x002C($sp)          
/* 013E0 80B240D0 46028201 */  sub.s   $f8, $f16, $f2             
/* 013E4 80B240D4 C7AE003C */  lwc1    $f14, 0x003C($sp)          
/* 013E8 80B240D8 3C0180B2 */  lui     $at, %hi(D_80B2444C)       ## $at = 80B20000
/* 013EC 80B240DC 460C9101 */  sub.s   $f4, $f18, $f12            
/* 013F0 80B240E0 46004282 */  mul.s   $f10, $f8, $f0             
/* 013F4 80B240E4 2604005C */  addiu   $a0, $s0, 0x005C           ## $a0 = 0000005C
/* 013F8 80B240E8 460EA201 */  sub.s   $f8, $f20, $f14            
/* 013FC 80B240EC 46002182 */  mul.s   $f6, $f4, $f0              
/* 01400 80B240F0 C6040024 */  lwc1    $f4, 0x0024($s0)           ## 00000024
/* 01404 80B240F4 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 01408 80B240F8 46025080 */  add.s   $f2, $f10, $f2             
/* 0140C 80B240FC 46004282 */  mul.s   $f10, $f8, $f0             
/* 01410 80B24100 C428444C */  lwc1    $f8, %lo(D_80B2444C)($at)  
/* 01414 80B24104 460C3300 */  add.s   $f12, $f6, $f12            
/* 01418 80B24108 3C0180B2 */  lui     $at, %hi(D_80B24450)       ## $at = 80B20000
/* 0141C 80B2410C 46041181 */  sub.s   $f6, $f2, $f4              
/* 01420 80B24110 460E5380 */  add.s   $f14, $f10, $f14           
/* 01424 80B24114 C60A0028 */  lwc1    $f10, 0x0028($s0)          ## 00000028
/* 01428 80B24118 46083402 */  mul.s   $f16, $f6, $f8             
/* 0142C 80B2411C C4264450 */  lwc1    $f6, %lo(D_80B24450)($at)  
/* 01430 80B24120 460A6101 */  sub.s   $f4, $f12, $f10            
/* 01434 80B24124 C608002C */  lwc1    $f8, 0x002C($s0)           ## 0000002C
/* 01438 80B24128 3C0180B2 */  lui     $at, %hi(D_80B24454)       ## $at = 80B20000
/* 0143C 80B2412C 46062482 */  mul.s   $f18, $f4, $f6             
/* 01440 80B24130 46087281 */  sub.s   $f10, $f14, $f8            
/* 01444 80B24134 C4244454 */  lwc1    $f4, %lo(D_80B24454)($at)  
/* 01448 80B24138 44807000 */  mtc1    $zero, $f14                ## $f14 = 0.00
/* 0144C 80B2413C 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 01450 80B24140 46045502 */  mul.s   $f20, $f10, $f4            
/* 01454 80B24144 00000000 */  nop
/* 01458 80B24148 46108182 */  mul.s   $f6, $f16, $f16            
/* 0145C 80B2414C 00000000 */  nop
/* 01460 80B24150 46129202 */  mul.s   $f8, $f18, $f18            
/* 01464 80B24154 46083280 */  add.s   $f10, $f6, $f8             
/* 01468 80B24158 4614A102 */  mul.s   $f4, $f20, $f20            
/* 0146C 80B2415C 46045000 */  add.s   $f0, $f10, $f4             
/* 01470 80B24160 46000004 */  sqrt.s  $f0, $f0                   
/* 01474 80B24164 460E003C */  c.lt.s  $f0, $f14                  
/* 01478 80B24168 00000000 */  nop
/* 0147C 80B2416C 45000003 */  bc1f    .L80B2417C                 
/* 01480 80B24170 00000000 */  nop
/* 01484 80B24174 1000000B */  beq     $zero, $zero, .L80B241A4   
/* 01488 80B24178 46007306 */  mov.s   $f12, $f14                 
.L80B2417C:
/* 0148C 80B2417C 44816000 */  mtc1    $at, $f12                  ## $f12 = 20.00
/* 01490 80B24180 00000000 */  nop
/* 01494 80B24184 4600603C */  c.lt.s  $f12, $f0                  
/* 01498 80B24188 00000000 */  nop
/* 0149C 80B2418C 45020004 */  bc1fl   .L80B241A0                 
/* 014A0 80B24190 46000086 */  mov.s   $f2, $f0                   
/* 014A4 80B24194 10000002 */  beq     $zero, $zero, .L80B241A0   
/* 014A8 80B24198 46006086 */  mov.s   $f2, $f12                  
/* 014AC 80B2419C 46000086 */  mov.s   $f2, $f0                   
.L80B241A0:
/* 014B0 80B241A0 46001306 */  mov.s   $f12, $f2                  
.L80B241A4:
/* 014B4 80B241A4 460C0032 */  c.eq.s  $f0, $f12                  
/* 014B8 80B241A8 00000000 */  nop
/* 014BC 80B241AC 4503000D */  bc1tl   .L80B241E4                 
/* 014C0 80B241B0 44058000 */  mfc1    $a1, $f16                  
/* 014C4 80B241B4 460E0032 */  c.eq.s  $f0, $f14                  
/* 014C8 80B241B8 00000000 */  nop
/* 014CC 80B241BC 45030009 */  bc1tl   .L80B241E4                 
/* 014D0 80B241C0 44058000 */  mfc1    $a1, $f16                  
/* 014D4 80B241C4 46006083 */  div.s   $f2, $f12, $f0             
/* 014D8 80B241C8 46028402 */  mul.s   $f16, $f16, $f2            
/* 014DC 80B241CC 00000000 */  nop
/* 014E0 80B241D0 46029482 */  mul.s   $f18, $f18, $f2            
/* 014E4 80B241D4 00000000 */  nop
/* 014E8 80B241D8 4602A502 */  mul.s   $f20, $f20, $f2            
/* 014EC 80B241DC 00000000 */  nop
/* 014F0 80B241E0 44058000 */  mfc1    $a1, $f16                  
.L80B241E4:
/* 014F4 80B241E4 0C01DE80 */  jal     Math_StepToF
              
/* 014F8 80B241E8 E7B2002C */  swc1    $f18, 0x002C($sp)          
/* 014FC 80B241EC C7B2002C */  lwc1    $f18, 0x002C($sp)          
/* 01500 80B241F0 26040060 */  addiu   $a0, $s0, 0x0060           ## $a0 = 00000060
/* 01504 80B241F4 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 01508 80B241F8 44059000 */  mfc1    $a1, $f18                  
/* 0150C 80B241FC 0C01DE80 */  jal     Math_StepToF
              
/* 01510 80B24200 00000000 */  nop
/* 01514 80B24204 4405A000 */  mfc1    $a1, $f20                  
/* 01518 80B24208 26040064 */  addiu   $a0, $s0, 0x0064           ## $a0 = 00000064
/* 0151C 80B2420C 0C01DE80 */  jal     Math_StepToF
              
/* 01520 80B24210 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 01524 80B24214 0C00B5FB */  jal     func_8002D7EC              
/* 01528 80B24218 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0152C 80B2421C 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 01530 80B24220 D7B40018 */  ldc1    $f20, 0x0018($sp)          
/* 01534 80B24224 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 01538 80B24228 03E00008 */  jr      $ra                        
/* 0153C 80B2422C 27BD0040 */  addiu   $sp, $sp, 0x0040           ## $sp = 00000000