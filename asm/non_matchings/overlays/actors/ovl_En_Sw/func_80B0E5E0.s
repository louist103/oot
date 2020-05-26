.late_rodata
glabel D_80B0F248
 .word 0x469C4000

.text
glabel func_80B0E5E0
/* 02830 80B0E5E0 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 02834 80B0E5E4 AFA5003C */  sw      $a1, 0x003C($sp)           
/* 02838 80B0E5E8 8FAE003C */  lw      $t6, 0x003C($sp)           
/* 0283C 80B0E5EC AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 02840 80B0E5F0 AFB00020 */  sw      $s0, 0x0020($sp)           
/* 02844 80B0E5F4 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 02848 80B0E5F8 3C0540C0 */  lui     $a1, 0x40C0                ## $a1 = 40C00000
/* 0284C 80B0E5FC 240603E8 */  addiu   $a2, $zero, 0x03E8         ## $a2 = 000003E8
/* 02850 80B0E600 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 02854 80B0E604 0C2C390C */  jal     func_80B0E430              
/* 02858 80B0E608 AFAE0010 */  sw      $t6, 0x0010($sp)           
/* 0285C 80B0E60C 5040002C */  beql    $v0, $zero, .L80B0E6C0     
/* 02860 80B0E610 86020442 */  lh      $v0, 0x0442($s0)           ## 00000442
/* 02864 80B0E614 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 02868 80B0E618 00000000 */  nop
/* 0286C 80B0E61C 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 02870 80B0E620 E7A0002C */  swc1    $f0, 0x002C($sp)           
/* 02874 80B0E624 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 02878 80B0E628 44812000 */  mtc1    $at, $f4                   ## $f4 = 0.50
/* 0287C 80B0E62C 3C01BF80 */  lui     $at, 0xBF80                ## $at = BF800000
/* 02880 80B0E630 C7A8002C */  lwc1    $f8, 0x002C($sp)           
/* 02884 80B0E634 4600203E */  c.le.s  $f4, $f0                   
/* 02888 80B0E638 00000000 */  nop
/* 0288C 80B0E63C 45020006 */  bc1fl   .L80B0E658                 
/* 02890 80B0E640 44810000 */  mtc1    $at, $f0                   ## $f0 = -1.00
/* 02894 80B0E644 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 02898 80B0E648 44810000 */  mtc1    $at, $f0                   ## $f0 = 1.00
/* 0289C 80B0E64C 10000003 */  beq     $zero, $zero, .L80B0E65C   
/* 028A0 80B0E650 00000000 */  nop
/* 028A4 80B0E654 44810000 */  mtc1    $at, $f0                   ## $f0 = 1.00
.L80B0E658:
/* 028A8 80B0E658 00000000 */  nop
.L80B0E65C:
/* 028AC 80B0E65C 3C0180B1 */  lui     $at, %hi(D_80B0F248)       ## $at = 80B10000
/* 028B0 80B0E660 C426F248 */  lwc1    $f6, %lo(D_80B0F248)($at)  
/* 028B4 80B0E664 860A0034 */  lh      $t2, 0x0034($s0)           ## 00000034
/* 028B8 80B0E668 2404000A */  addiu   $a0, $zero, 0x000A         ## $a0 = 0000000A
/* 028BC 80B0E66C 46083282 */  mul.s   $f10, $f6, $f8             
/* 028C0 80B0E670 448A4000 */  mtc1    $t2, $f8                   ## $f8 = 0.00
/* 028C4 80B0E674 2405001E */  addiu   $a1, $zero, 0x001E         ## $a1 = 0000001E
/* 028C8 80B0E678 4600540D */  trunc.w.s $f16, $f10                 
/* 028CC 80B0E67C 468042A0 */  cvt.s.w $f10, $f8                  
/* 028D0 80B0E680 44188000 */  mfc1    $t8, $f16                  
/* 028D4 80B0E684 00000000 */  nop
/* 028D8 80B0E688 0018CC00 */  sll     $t9, $t8, 16               
/* 028DC 80B0E68C 00194403 */  sra     $t0, $t9, 16               
/* 028E0 80B0E690 25092EE0 */  addiu   $t1, $t0, 0x2EE0           ## $t1 = 00002EE0
/* 028E4 80B0E694 44899000 */  mtc1    $t1, $f18                  ## $f18 = 0.00
/* 028E8 80B0E698 00000000 */  nop
/* 028EC 80B0E69C 46809120 */  cvt.s.w $f4, $f18                  
/* 028F0 80B0E6A0 46040182 */  mul.s   $f6, $f0, $f4              
/* 028F4 80B0E6A4 460A3400 */  add.s   $f16, $f6, $f10            
/* 028F8 80B0E6A8 4600848D */  trunc.w.s $f18, $f16                 
/* 028FC 80B0E6AC 440C9000 */  mfc1    $t4, $f18                  
/* 02900 80B0E6B0 0C01DF64 */  jal     Math_Rand_S16Offset
              
/* 02904 80B0E6B4 A60C0444 */  sh      $t4, 0x0444($s0)           ## 00000444
/* 02908 80B0E6B8 A6020388 */  sh      $v0, 0x0388($s0)           ## 00000388
/* 0290C 80B0E6BC 86020442 */  lh      $v0, 0x0442($s0)           ## 00000442
.L80B0E6C0:
/* 02910 80B0E6C0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02914 80B0E6C4 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 02918 80B0E6C8 14400003 */  bne     $v0, $zero, .L80B0E6D8     
/* 0291C 80B0E6CC 244DFFFF */  addiu   $t5, $v0, 0xFFFF           ## $t5 = FFFFFFFF
/* 02920 80B0E6D0 10000003 */  beq     $zero, $zero, .L80B0E6E0   
/* 02924 80B0E6D4 00001825 */  or      $v1, $zero, $zero          ## $v1 = 00000000
.L80B0E6D8:
/* 02928 80B0E6D8 A60D0442 */  sh      $t5, 0x0442($s0)           ## 00000442
/* 0292C 80B0E6DC 86030442 */  lh      $v1, 0x0442($s0)           ## 00000442
.L80B0E6E0:
/* 02930 80B0E6E0 5460000D */  bnel    $v1, $zero, .L80B0E718     
/* 02934 80B0E6E4 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 02938 80B0E6E8 0C2C37AA */  jal     func_80B0DEA8              
/* 0293C 80B0E6EC 8FA5003C */  lw      $a1, 0x003C($sp)           
/* 02940 80B0E6F0 10400008 */  beq     $v0, $zero, .L80B0E714     
/* 02944 80B0E6F4 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02948 80B0E6F8 0C00BE0A */  jal     Audio_PlayActorSound2
              
/* 0294C 80B0E6FC 240539F2 */  addiu   $a1, $zero, 0x39F2         ## $a1 = 000039F2
/* 02950 80B0E700 3C0F80B1 */  lui     $t7, %hi(func_80B0E728)    ## $t7 = 80B10000
/* 02954 80B0E704 240E0014 */  addiu   $t6, $zero, 0x0014         ## $t6 = 00000014
/* 02958 80B0E708 25EFE728 */  addiu   $t7, $t7, %lo(func_80B0E728) ## $t7 = 80B0E728
/* 0295C 80B0E70C A60E0442 */  sh      $t6, 0x0442($s0)           ## 00000442
/* 02960 80B0E710 AE0F0190 */  sw      $t7, 0x0190($s0)           ## 00000190
.L80B0E714:
/* 02964 80B0E714 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80B0E718:
/* 02968 80B0E718 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 0296C 80B0E71C 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000
/* 02970 80B0E720 03E00008 */  jr      $ra                        
/* 02974 80B0E724 00000000 */  nop