.late_rodata
glabel D_80A9E8F8
 .word 0x3F1A36E2, 0xEB1C432D
glabel D_80A9E900
    .float 0.1

glabel D_80A9E904
    .float 0.1

glabel D_80A9E908
 .word 0x3F1A36E2, 0xEB1C432D

.text
glabel func_80A9E074
/* 00534 80A9E074 27BDFFC0 */  addiu   $sp, $sp, 0xFFC0           ## $sp = FFFFFFC0
/* 00538 80A9E078 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 0053C 80A9E07C AFB00020 */  sw      $s0, 0x0020($sp)           
/* 00540 80A9E080 AFA50044 */  sw      $a1, 0x0044($sp)           
/* 00544 80A9E084 848E001C */  lh      $t6, 0x001C($a0)           ## 0000001C
/* 00548 80A9E088 3C1980AA */  lui     $t9, %hi(D_80A9E840)       ## $t9 = 80AA0000
/* 0054C 80A9E08C 2739E840 */  addiu   $t9, $t9, %lo(D_80A9E840)  ## $t9 = 80A9E840
/* 00550 80A9E090 31CF000F */  andi    $t7, $t6, 0x000F           ## $t7 = 00000000
/* 00554 80A9E094 000FC0C0 */  sll     $t8, $t7,  3               
/* 00558 80A9E098 03194021 */  addu    $t0, $t8, $t9              
/* 0055C 80A9E09C AFA80038 */  sw      $t0, 0x0038($sp)           
/* 00560 80A9E0A0 91090007 */  lbu     $t1, 0x0007($t0)           ## 00000007
/* 00564 80A9E0A4 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 00568 80A9E0A8 3C014F80 */  lui     $at, 0x4F80                ## $at = 4F800000
/* 0056C 80A9E0AC 44892000 */  mtc1    $t1, $f4                   ## $f4 = 0.00
/* 00570 80A9E0B0 05210004 */  bgez    $t1, .L80A9E0C4            
/* 00574 80A9E0B4 468021A0 */  cvt.s.w $f6, $f4                   
/* 00578 80A9E0B8 44814000 */  mtc1    $at, $f8                   ## $f8 = 4294967296.00
/* 0057C 80A9E0BC 00000000 */  nop
/* 00580 80A9E0C0 46083180 */  add.s   $f6, $f6, $f8              
.L80A9E0C4:
/* 00584 80A9E0C4 3C0180AA */  lui     $at, %hi(D_80A9E8F8)       ## $at = 80AA0000
/* 00588 80A9E0C8 D430E8F8 */  ldc1    $f16, %lo(D_80A9E8F8)($at) 
/* 0058C 80A9E0CC 460032A1 */  cvt.d.s $f10, $f6                  
/* 00590 80A9E0D0 C6040050 */  lwc1    $f4, 0x0050($s0)           ## 00000050
/* 00594 80A9E0D4 46305482 */  mul.d   $f18, $f10, $f16           
/* 00598 80A9E0D8 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 0059C 80A9E0DC 46002221 */  cvt.d.s $f8, $f4                   
/* 005A0 80A9E0E0 46324183 */  div.d   $f6, $f8, $f18             
/* 005A4 80A9E0E4 462032A0 */  cvt.s.d $f10, $f6                  
/* 005A8 80A9E0E8 E7AA0030 */  swc1    $f10, 0x0030($sp)          
/* 005AC 80A9E0EC 8602001C */  lh      $v0, 0x001C($s0)           ## 0000001C
/* 005B0 80A9E0F0 304A0800 */  andi    $t2, $v0, 0x0800           ## $t2 = 00000000
/* 005B4 80A9E0F4 1140001F */  beq     $t2, $zero, .L80A9E174     
/* 005B8 80A9E0F8 304503F0 */  andi    $a1, $v0, 0x03F0           ## $a1 = 00000000
/* 005BC 80A9E0FC 304503F0 */  andi    $a1, $v0, 0x03F0           ## $a1 = 00000000
/* 005C0 80A9E100 00052903 */  sra     $a1, $a1,  4               
/* 005C4 80A9E104 0C00B2D0 */  jal     Flags_GetSwitch
              
/* 005C8 80A9E108 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 005CC 80A9E10C 10400008 */  beq     $v0, $zero, .L80A9E130     
/* 005D0 80A9E110 C7B00030 */  lwc1    $f16, 0x0030($sp)          
/* 005D4 80A9E114 3C063D4C */  lui     $a2, 0x3D4C                ## $a2 = 3D4C0000
/* 005D8 80A9E118 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3D4CCCCD
/* 005DC 80A9E11C 27A40030 */  addiu   $a0, $sp, 0x0030           ## $a0 = FFFFFFF0
/* 005E0 80A9E120 0C01DE80 */  jal     Math_ApproxF
              
/* 005E4 80A9E124 3C053F80 */  lui     $a1, 0x3F80                ## $a1 = 3F800000
/* 005E8 80A9E128 1000002D */  beq     $zero, $zero, .L80A9E1E0   
/* 005EC 80A9E12C 8FAB0038 */  lw      $t3, 0x0038($sp)           
.L80A9E130:
/* 005F0 80A9E130 3C0180AA */  lui     $at, %hi(D_80A9E900)       ## $at = 80AA0000
/* 005F4 80A9E134 C424E900 */  lwc1    $f4, %lo(D_80A9E900)($at)  
/* 005F8 80A9E138 27A40030 */  addiu   $a0, $sp, 0x0030           ## $a0 = FFFFFFF0
/* 005FC 80A9E13C 24050000 */  addiu   $a1, $zero, 0x0000         ## $a1 = 00000000
/* 00600 80A9E140 4604803C */  c.lt.s  $f16, $f4                  
/* 00604 80A9E144 3C063D4C */  lui     $a2, 0x3D4C                ## $a2 = 3D4C0000
/* 00608 80A9E148 45000006 */  bc1f    .L80A9E164                 
/* 0060C 80A9E14C 00000000 */  nop
/* 00610 80A9E150 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00614 80A9E154 0C00B58B */  jal     Actor_SetScale
              
/* 00618 80A9E158 24050000 */  addiu   $a1, $zero, 0x0000         ## $a1 = 00000000
/* 0061C 80A9E15C 100000CF */  beq     $zero, $zero, .L80A9E49C   
/* 00620 80A9E160 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80A9E164:
/* 00624 80A9E164 0C01DE80 */  jal     Math_ApproxF
              
/* 00628 80A9E168 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 0000CCCD
/* 0062C 80A9E16C 1000001C */  beq     $zero, $zero, .L80A9E1E0   
/* 00630 80A9E170 8FAB0038 */  lw      $t3, 0x0038($sp)           
.L80A9E174:
/* 00634 80A9E174 0C00B2D0 */  jal     Flags_GetSwitch
              
/* 00638 80A9E178 00052903 */  sra     $a1, $a1,  4               
/* 0063C 80A9E17C 10400013 */  beq     $v0, $zero, .L80A9E1CC     
/* 00640 80A9E180 27A40030 */  addiu   $a0, $sp, 0x0030           ## $a0 = FFFFFFF0
/* 00644 80A9E184 3C0180AA */  lui     $at, %hi(D_80A9E904)       ## $at = 80AA0000
/* 00648 80A9E188 C432E904 */  lwc1    $f18, %lo(D_80A9E904)($at) 
/* 0064C 80A9E18C C7A80030 */  lwc1    $f8, 0x0030($sp)           
/* 00650 80A9E190 27A40030 */  addiu   $a0, $sp, 0x0030           ## $a0 = FFFFFFF0
/* 00654 80A9E194 24050000 */  addiu   $a1, $zero, 0x0000         ## $a1 = 00000000
/* 00658 80A9E198 4612403C */  c.lt.s  $f8, $f18                  
/* 0065C 80A9E19C 3C063D4C */  lui     $a2, 0x3D4C                ## $a2 = 3D4C0000
/* 00660 80A9E1A0 45000006 */  bc1f    .L80A9E1BC                 
/* 00664 80A9E1A4 00000000 */  nop
/* 00668 80A9E1A8 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0066C 80A9E1AC 0C00B58B */  jal     Actor_SetScale
              
/* 00670 80A9E1B0 24050000 */  addiu   $a1, $zero, 0x0000         ## $a1 = 00000000
/* 00674 80A9E1B4 100000B9 */  beq     $zero, $zero, .L80A9E49C   
/* 00678 80A9E1B8 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80A9E1BC:
/* 0067C 80A9E1BC 0C01DE80 */  jal     Math_ApproxF
              
/* 00680 80A9E1C0 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 0000CCCD
/* 00684 80A9E1C4 10000006 */  beq     $zero, $zero, .L80A9E1E0   
/* 00688 80A9E1C8 8FAB0038 */  lw      $t3, 0x0038($sp)           
.L80A9E1CC:
/* 0068C 80A9E1CC 3C063D4C */  lui     $a2, 0x3D4C                ## $a2 = 3D4C0000
/* 00690 80A9E1D0 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3D4CCCCD
/* 00694 80A9E1D4 0C01DE80 */  jal     Math_ApproxF
              
/* 00698 80A9E1D8 3C053F80 */  lui     $a1, 0x3F80                ## $a1 = 3F800000
/* 0069C 80A9E1DC 8FAB0038 */  lw      $t3, 0x0038($sp)           
.L80A9E1E0:
/* 006A0 80A9E1E0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 006A4 80A9E1E4 916C0007 */  lbu     $t4, 0x0007($t3)           ## 00000007
/* 006A8 80A9E1E8 448C3000 */  mtc1    $t4, $f6                   ## $f6 = 0.00
/* 006AC 80A9E1EC 05810005 */  bgez    $t4, .L80A9E204            
/* 006B0 80A9E1F0 468032A0 */  cvt.s.w $f10, $f6                  
/* 006B4 80A9E1F4 3C014F80 */  lui     $at, 0x4F80                ## $at = 4F800000
/* 006B8 80A9E1F8 44818000 */  mtc1    $at, $f16                  ## $f16 = 4294967296.00
/* 006BC 80A9E1FC 00000000 */  nop
/* 006C0 80A9E200 46105280 */  add.s   $f10, $f10, $f16           
.L80A9E204:
/* 006C4 80A9E204 3C0180AA */  lui     $at, %hi(D_80A9E908)       ## $at = 80AA0000
/* 006C8 80A9E208 D428E908 */  ldc1    $f8, %lo(D_80A9E908)($at)  
/* 006CC 80A9E20C 46005121 */  cvt.d.s $f4, $f10                  
/* 006D0 80A9E210 C7A60030 */  lwc1    $f6, 0x0030($sp)           
/* 006D4 80A9E214 46282482 */  mul.d   $f18, $f4, $f8             
/* 006D8 80A9E218 46003421 */  cvt.d.s $f16, $f6                  
/* 006DC 80A9E21C 46309282 */  mul.d   $f10, $f18, $f16           
/* 006E0 80A9E220 46205120 */  cvt.s.d $f4, $f10                  
/* 006E4 80A9E224 44052000 */  mfc1    $a1, $f4                   
/* 006E8 80A9E228 0C00B58B */  jal     Actor_SetScale
              
/* 006EC 80A9E22C 00000000 */  nop
/* 006F0 80A9E230 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 006F4 80A9E234 00000000 */  nop
/* 006F8 80A9E238 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 006FC 80A9E23C 44816000 */  mtc1    $at, $f12                  ## $f12 = 0.50
/* 00700 80A9E240 8FAD0038 */  lw      $t5, 0x0038($sp)           
/* 00704 80A9E244 3C014F80 */  lui     $at, 0x4F80                ## $at = 4F800000
/* 00708 80A9E248 460C0202 */  mul.s   $f8, $f0, $f12             
/* 0070C 80A9E24C 91AE0000 */  lbu     $t6, 0x0000($t5)           ## 00000000
/* 00710 80A9E250 26040154 */  addiu   $a0, $s0, 0x0154           ## $a0 = 00000154
/* 00714 80A9E254 448E3000 */  mtc1    $t6, $f6                   ## $f6 = 0.00
/* 00718 80A9E258 460C4080 */  add.s   $f2, $f8, $f12             
/* 0071C 80A9E25C 05C10004 */  bgez    $t6, .L80A9E270            
/* 00720 80A9E260 468034A0 */  cvt.s.w $f18, $f6                  
/* 00724 80A9E264 44818000 */  mtc1    $at, $f16                  ## $f16 = 4294967296.00
/* 00728 80A9E268 00000000 */  nop
/* 0072C 80A9E26C 46109480 */  add.s   $f18, $f18, $f16           
.L80A9E270:
/* 00730 80A9E270 46029282 */  mul.s   $f10, $f18, $f2            
/* 00734 80A9E274 24050001 */  addiu   $a1, $zero, 0x0001         ## $a1 = 00000001
/* 00738 80A9E278 3C014F00 */  lui     $at, 0x4F00                ## $at = 4F000000
/* 0073C 80A9E27C 444FF800 */  cfc1    $t7, $31
/* 00740 80A9E280 44C5F800 */  ctc1    $a1, $31
/* 00744 80A9E284 00000000 */  nop
/* 00748 80A9E288 46005124 */  cvt.w.s $f4, $f10                  
/* 0074C 80A9E28C 4445F800 */  cfc1    $a1, $31
/* 00750 80A9E290 00000000 */  nop
/* 00754 80A9E294 30A50078 */  andi    $a1, $a1, 0x0078           ## $a1 = 00000000
/* 00758 80A9E298 50A00013 */  beql    $a1, $zero, .L80A9E2E8     
/* 0075C 80A9E29C 44052000 */  mfc1    $a1, $f4                   
/* 00760 80A9E2A0 44812000 */  mtc1    $at, $f4                   ## $f4 = 2147483648.00
/* 00764 80A9E2A4 24050001 */  addiu   $a1, $zero, 0x0001         ## $a1 = 00000001
/* 00768 80A9E2A8 46045101 */  sub.s   $f4, $f10, $f4             
/* 0076C 80A9E2AC 44C5F800 */  ctc1    $a1, $31
/* 00770 80A9E2B0 00000000 */  nop
/* 00774 80A9E2B4 46002124 */  cvt.w.s $f4, $f4                   
/* 00778 80A9E2B8 4445F800 */  cfc1    $a1, $31
/* 0077C 80A9E2BC 00000000 */  nop
/* 00780 80A9E2C0 30A50078 */  andi    $a1, $a1, 0x0078           ## $a1 = 00000000
/* 00784 80A9E2C4 14A00005 */  bne     $a1, $zero, .L80A9E2DC     
/* 00788 80A9E2C8 00000000 */  nop
/* 0078C 80A9E2CC 44052000 */  mfc1    $a1, $f4                   
/* 00790 80A9E2D0 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 00794 80A9E2D4 10000007 */  beq     $zero, $zero, .L80A9E2F4   
/* 00798 80A9E2D8 00A12825 */  or      $a1, $a1, $at              ## $a1 = 80000000
.L80A9E2DC:
/* 0079C 80A9E2DC 10000005 */  beq     $zero, $zero, .L80A9E2F4   
/* 007A0 80A9E2E0 2405FFFF */  addiu   $a1, $zero, 0xFFFF         ## $a1 = FFFFFFFF
/* 007A4 80A9E2E4 44052000 */  mfc1    $a1, $f4                   
.L80A9E2E8:
/* 007A8 80A9E2E8 00000000 */  nop
/* 007AC 80A9E2EC 04A0FFFB */  bltz    $a1, .L80A9E2DC            
/* 007B0 80A9E2F0 00000000 */  nop
.L80A9E2F4:
/* 007B4 80A9E2F4 91B80001 */  lbu     $t8, 0x0001($t5)           ## 00000001
/* 007B8 80A9E2F8 44CFF800 */  ctc1    $t7, $31
/* 007BC 80A9E2FC 30A500FF */  andi    $a1, $a1, 0x00FF           ## $a1 = 000000FF
/* 007C0 80A9E300 44984000 */  mtc1    $t8, $f8                   ## $f8 = 0.00
/* 007C4 80A9E304 3C014F80 */  lui     $at, 0x4F80                ## $at = 4F800000
/* 007C8 80A9E308 07010004 */  bgez    $t8, .L80A9E31C            
/* 007CC 80A9E30C 468041A0 */  cvt.s.w $f6, $f8                   
/* 007D0 80A9E310 44818000 */  mtc1    $at, $f16                  ## $f16 = 4294967296.00
/* 007D4 80A9E314 00000000 */  nop
/* 007D8 80A9E318 46103180 */  add.s   $f6, $f6, $f16             
.L80A9E31C:
/* 007DC 80A9E31C 46023482 */  mul.s   $f18, $f6, $f2             
/* 007E0 80A9E320 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 007E4 80A9E324 3C014F00 */  lui     $at, 0x4F00                ## $at = 4F000000
/* 007E8 80A9E328 4459F800 */  cfc1    $t9, $31
/* 007EC 80A9E32C 44C6F800 */  ctc1    $a2, $31
/* 007F0 80A9E330 00000000 */  nop
/* 007F4 80A9E334 460092A4 */  cvt.w.s $f10, $f18                 
/* 007F8 80A9E338 4446F800 */  cfc1    $a2, $31
/* 007FC 80A9E33C 00000000 */  nop
/* 00800 80A9E340 30C60078 */  andi    $a2, $a2, 0x0078           ## $a2 = 00000000
/* 00804 80A9E344 50C00013 */  beql    $a2, $zero, .L80A9E394     
/* 00808 80A9E348 44065000 */  mfc1    $a2, $f10                  
/* 0080C 80A9E34C 44815000 */  mtc1    $at, $f10                  ## $f10 = 2147483648.00
/* 00810 80A9E350 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 00814 80A9E354 460A9281 */  sub.s   $f10, $f18, $f10           
/* 00818 80A9E358 44C6F800 */  ctc1    $a2, $31
/* 0081C 80A9E35C 00000000 */  nop
/* 00820 80A9E360 460052A4 */  cvt.w.s $f10, $f10                 
/* 00824 80A9E364 4446F800 */  cfc1    $a2, $31
/* 00828 80A9E368 00000000 */  nop
/* 0082C 80A9E36C 30C60078 */  andi    $a2, $a2, 0x0078           ## $a2 = 00000000
/* 00830 80A9E370 14C00005 */  bne     $a2, $zero, .L80A9E388     
/* 00834 80A9E374 00000000 */  nop
/* 00838 80A9E378 44065000 */  mfc1    $a2, $f10                  
/* 0083C 80A9E37C 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 00840 80A9E380 10000007 */  beq     $zero, $zero, .L80A9E3A0   
/* 00844 80A9E384 00C13025 */  or      $a2, $a2, $at              ## $a2 = 80000000
.L80A9E388:
/* 00848 80A9E388 10000005 */  beq     $zero, $zero, .L80A9E3A0   
/* 0084C 80A9E38C 2406FFFF */  addiu   $a2, $zero, 0xFFFF         ## $a2 = FFFFFFFF
/* 00850 80A9E390 44065000 */  mfc1    $a2, $f10                  
.L80A9E394:
/* 00854 80A9E394 00000000 */  nop
/* 00858 80A9E398 04C0FFFB */  bltz    $a2, .L80A9E388            
/* 0085C 80A9E39C 00000000 */  nop
.L80A9E3A0:
/* 00860 80A9E3A0 91A80002 */  lbu     $t0, 0x0002($t5)           ## 00000002
/* 00864 80A9E3A4 44D9F800 */  ctc1    $t9, $31
/* 00868 80A9E3A8 30C600FF */  andi    $a2, $a2, 0x00FF           ## $a2 = 000000FF
/* 0086C 80A9E3AC 44882000 */  mtc1    $t0, $f4                   ## $f4 = 0.00
/* 00870 80A9E3B0 3C014F80 */  lui     $at, 0x4F80                ## $at = 4F800000
/* 00874 80A9E3B4 05010004 */  bgez    $t0, .L80A9E3C8            
/* 00878 80A9E3B8 46802220 */  cvt.s.w $f8, $f4                   
/* 0087C 80A9E3BC 44818000 */  mtc1    $at, $f16                  ## $f16 = 4294967296.00
/* 00880 80A9E3C0 00000000 */  nop
/* 00884 80A9E3C4 46104200 */  add.s   $f8, $f8, $f16             
.L80A9E3C8:
/* 00888 80A9E3C8 46024182 */  mul.s   $f6, $f8, $f2              
/* 0088C 80A9E3CC 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 00890 80A9E3D0 3C014F00 */  lui     $at, 0x4F00                ## $at = 4F000000
/* 00894 80A9E3D4 4449F800 */  cfc1    $t1, $31
/* 00898 80A9E3D8 44C7F800 */  ctc1    $a3, $31
/* 0089C 80A9E3DC 00000000 */  nop
/* 008A0 80A9E3E0 460034A4 */  cvt.w.s $f18, $f6                  
/* 008A4 80A9E3E4 4447F800 */  cfc1    $a3, $31
/* 008A8 80A9E3E8 00000000 */  nop
/* 008AC 80A9E3EC 30E70078 */  andi    $a3, $a3, 0x0078           ## $a3 = 00000000
/* 008B0 80A9E3F0 50E00013 */  beql    $a3, $zero, .L80A9E440     
/* 008B4 80A9E3F4 44079000 */  mfc1    $a3, $f18                  
/* 008B8 80A9E3F8 44819000 */  mtc1    $at, $f18                  ## $f18 = 2147483648.00
/* 008BC 80A9E3FC 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 008C0 80A9E400 46123481 */  sub.s   $f18, $f6, $f18            
/* 008C4 80A9E404 44C7F800 */  ctc1    $a3, $31
/* 008C8 80A9E408 00000000 */  nop
/* 008CC 80A9E40C 460094A4 */  cvt.w.s $f18, $f18                 
/* 008D0 80A9E410 4447F800 */  cfc1    $a3, $31
/* 008D4 80A9E414 00000000 */  nop
/* 008D8 80A9E418 30E70078 */  andi    $a3, $a3, 0x0078           ## $a3 = 00000000
/* 008DC 80A9E41C 14E00005 */  bne     $a3, $zero, .L80A9E434     
/* 008E0 80A9E420 00000000 */  nop
/* 008E4 80A9E424 44079000 */  mfc1    $a3, $f18                  
/* 008E8 80A9E428 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 008EC 80A9E42C 10000007 */  beq     $zero, $zero, .L80A9E44C   
/* 008F0 80A9E430 00E13825 */  or      $a3, $a3, $at              ## $a3 = 80000000
.L80A9E434:
/* 008F4 80A9E434 10000005 */  beq     $zero, $zero, .L80A9E44C   
/* 008F8 80A9E438 2407FFFF */  addiu   $a3, $zero, 0xFFFF         ## $a3 = FFFFFFFF
/* 008FC 80A9E43C 44079000 */  mfc1    $a3, $f18                  
.L80A9E440:
/* 00900 80A9E440 00000000 */  nop
/* 00904 80A9E444 04E0FFFB */  bltz    $a3, .L80A9E434            
/* 00908 80A9E448 00000000 */  nop
.L80A9E44C:
/* 0090C 80A9E44C 3C014396 */  lui     $at, 0x4396                ## $at = 43960000
/* 00910 80A9E450 44815000 */  mtc1    $at, $f10                  ## $f10 = 300.00
/* 00914 80A9E454 44C9F800 */  ctc1    $t1, $31
/* 00918 80A9E458 C7A40030 */  lwc1    $f4, 0x0030($sp)           
/* 0091C 80A9E45C 30E700FF */  andi    $a3, $a3, 0x00FF           ## $a3 = 000000FF
/* 00920 80A9E460 46045402 */  mul.s   $f16, $f10, $f4            
/* 00924 80A9E464 4600820D */  trunc.w.s $f8, $f16                  
/* 00928 80A9E468 440B4000 */  mfc1    $t3, $f8                   
/* 0092C 80A9E46C 0C01E796 */  jal     Lights_SetPositionalLightColorAndRadius
              
/* 00930 80A9E470 AFAB0010 */  sw      $t3, 0x0010($sp)           
/* 00934 80A9E474 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00938 80A9E478 0C2A7754 */  jal     func_80A9DD50              
/* 0093C 80A9E47C 8FA50044 */  lw      $a1, 0x0044($sp)           
/* 00940 80A9E480 860C001C */  lh      $t4, 0x001C($s0)           ## 0000001C
/* 00944 80A9E484 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00948 80A9E488 05820004 */  bltzl   $t4, .L80A9E49C            
/* 0094C 80A9E48C 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 00950 80A9E490 0C00BE0A */  jal     Audio_PlayActorSound2
              
/* 00954 80A9E494 24052031 */  addiu   $a1, $zero, 0x2031         ## $a1 = 00002031
/* 00958 80A9E498 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80A9E49C:
/* 0095C 80A9E49C 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 00960 80A9E4A0 27BD0040 */  addiu   $sp, $sp, 0x0040           ## $sp = 00000000
/* 00964 80A9E4A4 03E00008 */  jr      $ra                        
/* 00968 80A9E4A8 00000000 */  nop
