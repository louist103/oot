glabel func_8093C1C4
/* 034F4 8093C1C4 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 034F8 8093C1C8 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 034FC 8093C1CC 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 03500 8093C1D0 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 03504 8093C1D4 24840568 */  addiu   $a0, $a0, 0x0568           ## $a0 = 00000568
/* 03508 8093C1D8 AFA5002C */  sw      $a1, 0x002C($sp)           
/* 0350C 8093C1DC 0C02927F */  jal     SkelAnime_FrameUpdateMatrix
              
/* 03510 8093C1E0 AFA40024 */  sw      $a0, 0x0024($sp)           
/* 03514 8093C1E4 260404D0 */  addiu   $a0, $s0, 0x04D0           ## $a0 = 000004D0
/* 03518 8093C1E8 24050000 */  addiu   $a1, $zero, 0x0000         ## $a1 = 00000000
/* 0351C 8093C1EC 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 03520 8093C1F0 0C01E107 */  jal     Math_SmoothScaleMaxF
              
/* 03524 8093C1F4 3C074120 */  lui     $a3, 0x4120                ## $a3 = 41200000
/* 03528 8093C1F8 8FA40024 */  lw      $a0, 0x0024($sp)           
/* 0352C 8093C1FC 0C0295B2 */  jal     func_800A56C8              
/* 03530 8093C200 8E0501A4 */  lw      $a1, 0x01A4($s0)           ## 000001A4
/* 03534 8093C204 10400010 */  beq     $v0, $zero, .L8093C248     
/* 03538 8093C208 3C0E8095 */  lui     $t6, %hi(twinrovaPtr)       ## $t6 = 80950000
/* 0353C 8093C20C 8DCEC84C */  lw      $t6, %lo(twinrovaPtr)($t6)  
/* 03540 8093C210 8FA5002C */  lw      $a1, 0x002C($sp)           
/* 03544 8093C214 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 03548 8093C218 85CF017C */  lh      $t7, 0x017C($t6)           ## 8095017C
/* 0354C 8093C21C 15E00005 */  bne     $t7, $zero, .L8093C234     
/* 03550 8093C220 00000000 */  nop
/* 03554 8093C224 0C24E82A */  jal     func_8093A0A8              
/* 03558 8093C228 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0355C 8093C22C 10000004 */  beq     $zero, $zero, .L8093C240   
/* 03560 8093C230 44802000 */  mtc1    $zero, $f4                 ## $f4 = 0.00
.L8093C234:
/* 03564 8093C234 0C24F163 */  jal     func_8093C58C              
/* 03568 8093C238 8FA5002C */  lw      $a1, 0x002C($sp)           
/* 0356C 8093C23C 44802000 */  mtc1    $zero, $f4                 ## $f4 = 0.00
.L8093C240:
/* 03570 8093C240 00000000 */  nop
/* 03574 8093C244 E60404D0 */  swc1    $f4, 0x04D0($s0)           ## 000004D0
.L8093C248:
/* 03578 8093C248 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 0357C 8093C24C 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 03580 8093C250 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 03584 8093C254 03E00008 */  jr      $ra                        
/* 03588 8093C258 00000000 */  nop
