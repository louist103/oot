glabel func_80A4320C
/* 004DC 80A4320C 27BDFF60 */  addiu   $sp, $sp, 0xFF60           ## $sp = FFFFFF60
/* 004E0 80A43210 3C0E80A5 */  lui     $t6, %hi(D_80A480F0)       ## $t6 = 80A50000
/* 004E4 80A43214 AFBF0064 */  sw      $ra, 0x0064($sp)           
/* 004E8 80A43218 AFB70060 */  sw      $s7, 0x0060($sp)           
/* 004EC 80A4321C AFB6005C */  sw      $s6, 0x005C($sp)           
/* 004F0 80A43220 AFB50058 */  sw      $s5, 0x0058($sp)           
/* 004F4 80A43224 AFB40054 */  sw      $s4, 0x0054($sp)           
/* 004F8 80A43228 AFB30050 */  sw      $s3, 0x0050($sp)           
/* 004FC 80A4322C AFB2004C */  sw      $s2, 0x004C($sp)           
/* 00500 80A43230 AFB10048 */  sw      $s1, 0x0048($sp)           
/* 00504 80A43234 AFB00044 */  sw      $s0, 0x0044($sp)           
/* 00508 80A43238 F7BA0038 */  sdc1    $f26, 0x0038($sp)          
/* 0050C 80A4323C F7B80030 */  sdc1    $f24, 0x0030($sp)          
/* 00510 80A43240 F7B60028 */  sdc1    $f22, 0x0028($sp)          
/* 00514 80A43244 F7B40020 */  sdc1    $f20, 0x0020($sp)          
/* 00518 80A43248 AFA500A4 */  sw      $a1, 0x00A4($sp)           
/* 0051C 80A4324C 25CE80F0 */  addiu   $t6, $t6, %lo(D_80A480F0)  ## $t6 = 80A480F0
/* 00520 80A43250 8DD80000 */  lw      $t8, 0x0000($t6)           ## 80A480F0
/* 00524 80A43254 27B40094 */  addiu   $s4, $sp, 0x0094           ## $s4 = FFFFFFF4
/* 00528 80A43258 3C1980A5 */  lui     $t9, %hi(D_80A480FC)       ## $t9 = 80A50000
/* 0052C 80A4325C AE980000 */  sw      $t8, 0x0000($s4)           ## FFFFFFF4
/* 00530 80A43260 8DCF0004 */  lw      $t7, 0x0004($t6)           ## 80A480F4
/* 00534 80A43264 273980FC */  addiu   $t9, $t9, %lo(D_80A480FC)  ## $t9 = 80A480FC
/* 00538 80A43268 27B60088 */  addiu   $s6, $sp, 0x0088           ## $s6 = FFFFFFE8
/* 0053C 80A4326C AE8F0004 */  sw      $t7, 0x0004($s4)           ## FFFFFFF8
/* 00540 80A43270 8DD80008 */  lw      $t8, 0x0008($t6)           ## 80A480F8
/* 00544 80A43274 3C0A80A5 */  lui     $t2, %hi(D_80A48108)       ## $t2 = 80A50000
/* 00548 80A43278 254A8108 */  addiu   $t2, $t2, %lo(D_80A48108)  ## $t2 = 80A48108
/* 0054C 80A4327C AE980008 */  sw      $t8, 0x0008($s4)           ## FFFFFFFC
/* 00550 80A43280 8F290000 */  lw      $t1, 0x0000($t9)           ## 80A480FC
/* 00554 80A43284 27B7007C */  addiu   $s7, $sp, 0x007C           ## $s7 = FFFFFFDC
/* 00558 80A43288 4486C000 */  mtc1    $a2, $f24                  ## $f24 = 0.00
/* 0055C 80A4328C AEC90000 */  sw      $t1, 0x0000($s6)           ## FFFFFFE8
/* 00560 80A43290 8F280004 */  lw      $t0, 0x0004($t9)           ## 80A48100
/* 00564 80A43294 4487D000 */  mtc1    $a3, $f26                  ## $f26 = 0.00
/* 00568 80A43298 00808825 */  or      $s1, $a0, $zero            ## $s1 = 00000000
/* 0056C 80A4329C AEC80004 */  sw      $t0, 0x0004($s6)           ## FFFFFFEC
/* 00570 80A432A0 8F290008 */  lw      $t1, 0x0008($t9)           ## 80A48104
/* 00574 80A432A4 30B500FF */  andi    $s5, $a1, 0x00FF           ## $s5 = 00000000
/* 00578 80A432A8 AEC90008 */  sw      $t1, 0x0008($s6)           ## FFFFFFF0
/* 0057C 80A432AC 8D4C0000 */  lw      $t4, 0x0000($t2)           ## 80A48108
/* 00580 80A432B0 AEEC0000 */  sw      $t4, 0x0000($s7)           ## FFFFFFDC
/* 00584 80A432B4 8D4B0004 */  lw      $t3, 0x0004($t2)           ## 80A4810C
/* 00588 80A432B8 AEEB0004 */  sw      $t3, 0x0004($s7)           ## FFFFFFE0
/* 0058C 80A432BC 8D4C0008 */  lw      $t4, 0x0008($t2)           ## 80A48110
/* 00590 80A432C0 AEEC0008 */  sw      $t4, 0x0008($s7)           ## FFFFFFE4
/* 00594 80A432C4 8C8E0024 */  lw      $t6, 0x0024($a0)           ## 00000024
/* 00598 80A432C8 AE8E0000 */  sw      $t6, 0x0000($s4)           ## FFFFFFF4
/* 0059C 80A432CC 8C8D0028 */  lw      $t5, 0x0028($a0)           ## 00000028
/* 005A0 80A432D0 AE8D0004 */  sw      $t5, 0x0004($s4)           ## FFFFFFF8
/* 005A4 80A432D4 8C8E002C */  lw      $t6, 0x002C($a0)           ## 0000002C
/* 005A8 80A432D8 AE8E0008 */  sw      $t6, 0x0008($s4)           ## FFFFFFFC
/* 005AC 80A432DC C4840080 */  lwc1    $f4, 0x0080($a0)           ## 00000080
/* 005B0 80A432E0 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 005B4 80A432E4 E7A40098 */  swc1    $f4, 0x0098($sp)           
/* 005B8 80A432E8 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 005BC 80A432EC 44813000 */  mtc1    $at, $f6                   ## $f6 = 0.50
/* 005C0 80A432F0 3C014780 */  lui     $at, 0x4780                ## $at = 47800000
/* 005C4 80A432F4 44815000 */  mtc1    $at, $f10                  ## $f10 = 65536.00
/* 005C8 80A432F8 46060201 */  sub.s   $f8, $f0, $f6              
/* 005CC 80A432FC 8FA200B0 */  lw      $v0, 0x00B0($sp)           
/* 005D0 80A43300 3C180001 */  lui     $t8, 0x0001                ## $t8 = 00010000
/* 005D4 80A43304 C7B600B8 */  lwc1    $f22, 0x00B8($sp)          
/* 005D8 80A43308 460A4402 */  mul.s   $f16, $f8, $f10            
/* 005DC 80A4330C C7B400B4 */  lwc1    $f20, 0x00B4($sp)          
/* 005E0 80A43310 00409025 */  or      $s2, $v0, $zero            ## $s2 = 00000000
/* 005E4 80A43314 4600848D */  trunc.w.s $f18, $f16                 
/* 005E8 80A43318 44109000 */  mfc1    $s0, $f18                  
/* 005EC 80A4331C 00000000 */  nop
/* 005F0 80A43320 00108400 */  sll     $s0, $s0, 16               
/* 005F4 80A43324 0440002F */  bltz    $v0, .L80A433E4            
/* 005F8 80A43328 00108403 */  sra     $s0, $s0, 16               
/* 005FC 80A4332C 0302001A */  div     $zero, $t8, $v0            
/* 00600 80A43330 00009812 */  mflo    $s3                        
/* 00604 80A43334 00139C00 */  sll     $s3, $s3, 16               
/* 00608 80A43338 00139C03 */  sra     $s3, $s3, 16               
/* 0060C 80A4333C 14400002 */  bne     $v0, $zero, .L80A43348     
/* 00610 80A43340 00000000 */  nop
/* 00614 80A43344 0007000D */  break 7
.L80A43348:
/* 00618 80A43348 2401FFFF */  addiu   $at, $zero, 0xFFFF         ## $at = FFFFFFFF
/* 0061C 80A4334C 14410004 */  bne     $v0, $at, .L80A43360       
/* 00620 80A43350 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 00624 80A43354 17010002 */  bne     $t8, $at, .L80A43360       
/* 00628 80A43358 00000000 */  nop
/* 0062C 80A4335C 0006000D */  break 6
.L80A43360:
/* 00630 80A43360 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 00634 80A43364 00000000 */  nop
/* 00638 80A43368 46160182 */  mul.s   $f6, $f0, $f22             
/* 0063C 80A4336C C7A40080 */  lwc1    $f4, 0x0080($sp)           
/* 00640 80A43370 00102400 */  sll     $a0, $s0, 16               
/* 00644 80A43374 00042403 */  sra     $a0, $a0, 16               
/* 00648 80A43378 46062200 */  add.s   $f8, $f4, $f6              
/* 0064C 80A4337C 0C01DE1C */  jal     Math_Sins
              ## sins?
/* 00650 80A43380 E7A80080 */  swc1    $f8, 0x0080($sp)           
/* 00654 80A43384 46140282 */  mul.s   $f10, $f0, $f20            
/* 00658 80A43388 C6300024 */  lwc1    $f16, 0x0024($s1)          ## 00000024
/* 0065C 80A4338C 00102400 */  sll     $a0, $s0, 16               
/* 00660 80A43390 00042403 */  sra     $a0, $a0, 16               
/* 00664 80A43394 46105480 */  add.s   $f18, $f10, $f16           
/* 00668 80A43398 0C01DE0D */  jal     Math_Coss
              ## coss?
/* 0066C 80A4339C E7B20094 */  swc1    $f18, 0x0094($sp)          
/* 00670 80A433A0 46140102 */  mul.s   $f4, $f0, $f20             
/* 00674 80A433A4 C626002C */  lwc1    $f6, 0x002C($s1)           ## 0000002C
/* 00678 80A433A8 E7BA0018 */  swc1    $f26, 0x0018($sp)          
/* 0067C 80A433AC E7B80014 */  swc1    $f24, 0x0014($sp)          
/* 00680 80A433B0 AFB50010 */  sw      $s5, 0x0010($sp)           
/* 00684 80A433B4 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 00688 80A433B8 02802825 */  or      $a1, $s4, $zero            ## $a1 = FFFFFFF4
/* 0068C 80A433BC 46062200 */  add.s   $f8, $f4, $f6              
/* 00690 80A433C0 02C03025 */  or      $a2, $s6, $zero            ## $a2 = FFFFFFE8
/* 00694 80A433C4 02E03825 */  or      $a3, $s7, $zero            ## $a3 = FFFFFFDC
/* 00698 80A433C8 0C290B4C */  jal     func_80A42D30              
/* 0069C 80A433CC E7A8009C */  swc1    $f8, 0x009C($sp)           
/* 006A0 80A433D0 02138021 */  addu    $s0, $s0, $s3              
/* 006A4 80A433D4 00108400 */  sll     $s0, $s0, 16               
/* 006A8 80A433D8 2652FFFF */  addiu   $s2, $s2, 0xFFFF           ## $s2 = FFFFFFFF
/* 006AC 80A433DC 0641FFE0 */  bgez    $s2, .L80A43360            
/* 006B0 80A433E0 00108403 */  sra     $s0, $s0, 16               
.L80A433E4:
/* 006B4 80A433E4 8FBF0064 */  lw      $ra, 0x0064($sp)           
/* 006B8 80A433E8 D7B40020 */  ldc1    $f20, 0x0020($sp)          
/* 006BC 80A433EC D7B60028 */  ldc1    $f22, 0x0028($sp)          
/* 006C0 80A433F0 D7B80030 */  ldc1    $f24, 0x0030($sp)          
/* 006C4 80A433F4 D7BA0038 */  ldc1    $f26, 0x0038($sp)          
/* 006C8 80A433F8 8FB00044 */  lw      $s0, 0x0044($sp)           
/* 006CC 80A433FC 8FB10048 */  lw      $s1, 0x0048($sp)           
/* 006D0 80A43400 8FB2004C */  lw      $s2, 0x004C($sp)           
/* 006D4 80A43404 8FB30050 */  lw      $s3, 0x0050($sp)           
/* 006D8 80A43408 8FB40054 */  lw      $s4, 0x0054($sp)           
/* 006DC 80A4340C 8FB50058 */  lw      $s5, 0x0058($sp)           
/* 006E0 80A43410 8FB6005C */  lw      $s6, 0x005C($sp)           
/* 006E4 80A43414 8FB70060 */  lw      $s7, 0x0060($sp)           
/* 006E8 80A43418 27BD00A0 */  addiu   $sp, $sp, 0x00A0           ## $sp = 00000000
/* 006EC 80A4341C 03E00008 */  jr      $ra                        
/* 006F0 80A43420 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000