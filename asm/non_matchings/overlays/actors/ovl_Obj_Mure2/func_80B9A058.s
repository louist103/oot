glabel func_80B9A058
/* 00118 80B9A058 27BDFFD0 */  addiu   $sp, $sp, 0xFFD0           ## $sp = FFFFFFD0
/* 0011C 80B9A05C AFBF002C */  sw      $ra, 0x002C($sp)           
/* 00120 80B9A060 AFB50028 */  sw      $s5, 0x0028($sp)           
/* 00124 80B9A064 AFB40024 */  sw      $s4, 0x0024($sp)           
/* 00128 80B9A068 AFB30020 */  sw      $s3, 0x0020($sp)           
/* 0012C 80B9A06C AFB2001C */  sw      $s2, 0x001C($sp)           
/* 00130 80B9A070 AFB10018 */  sw      $s1, 0x0018($sp)           
/* 00134 80B9A074 AFB00014 */  sw      $s0, 0x0014($sp)           
/* 00138 80B9A078 84AE001C */  lh      $t6, 0x001C($a1)           ## 0000001C
/* 0013C 80B9A07C 3C1580BA */  lui     $s5, %hi(D_80B9A818)       ## $s5 = 80BA0000
/* 00140 80B9A080 26B5A818 */  addiu   $s5, $s5, %lo(D_80B9A818)  ## $s5 = 80B9A818
/* 00144 80B9A084 31CF0003 */  andi    $t7, $t6, 0x0003           ## $t7 = 00000000
/* 00148 80B9A088 000FC040 */  sll     $t8, $t7,  1               
/* 0014C 80B9A08C 02B8C821 */  addu    $t9, $s5, $t8              
/* 00150 80B9A090 87280000 */  lh      $t0, 0x0000($t9)           ## 00000000
/* 00154 80B9A094 00A0A025 */  or      $s4, $a1, $zero            ## $s4 = 00000000
/* 00158 80B9A098 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 0015C 80B9A09C 19000025 */  blez    $t0, .L80B9A134            
/* 00160 80B9A0A0 00009025 */  or      $s2, $zero, $zero          ## $s2 = 00000000
/* 00164 80B9A0A4 3C1180BA */  lui     $s1, %hi(D_80B9A828)       ## $s1 = 80BA0000
/* 00168 80B9A0A8 2631A828 */  addiu   $s1, $s1, %lo(D_80B9A828)  ## $s1 = 80B9A828
/* 0016C 80B9A0AC 24B30024 */  addiu   $s3, $a1, 0x0024           ## $s3 = 00000024
/* 00170 80B9A0B0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
.L80B9A0B4:
/* 00174 80B9A0B4 0C01DF90 */  jal     Math_Vec3f_Copy
              ## Vec3f_Copy
/* 00178 80B9A0B8 02602825 */  or      $a1, $s3, $zero            ## $a1 = 00000024
/* 0017C 80B9A0BC 0C01DE0D */  jal     Math_CosS
              ## coss?
/* 00180 80B9A0C0 86240002 */  lh      $a0, 0x0002($s1)           ## 80B9A82A
/* 00184 80B9A0C4 86290000 */  lh      $t1, 0x0000($s1)           ## 80B9A828
/* 00188 80B9A0C8 C6040000 */  lwc1    $f4, 0x0000($s0)           ## 00000000
/* 0018C 80B9A0CC 44893000 */  mtc1    $t1, $f6                   ## $f6 = 0.00
/* 00190 80B9A0D0 00000000 */  nop
/* 00194 80B9A0D4 46803220 */  cvt.s.w $f8, $f6                   
/* 00198 80B9A0D8 46004282 */  mul.s   $f10, $f8, $f0             
/* 0019C 80B9A0DC 460A2400 */  add.s   $f16, $f4, $f10            
/* 001A0 80B9A0E0 E6100000 */  swc1    $f16, 0x0000($s0)          ## 00000000
/* 001A4 80B9A0E4 0C01DE1C */  jal     Math_SinS
              ## sins?
/* 001A8 80B9A0E8 86240002 */  lh      $a0, 0x0002($s1)           ## 80B9A82A
/* 001AC 80B9A0EC 862A0000 */  lh      $t2, 0x0000($s1)           ## 80B9A828
/* 001B0 80B9A0F0 C6120008 */  lwc1    $f18, 0x0008($s0)          ## 00000008
/* 001B4 80B9A0F4 26520001 */  addiu   $s2, $s2, 0x0001           ## $s2 = 00000001
/* 001B8 80B9A0F8 448A3000 */  mtc1    $t2, $f6                   ## $f6 = 0.00
/* 001BC 80B9A0FC 2610000C */  addiu   $s0, $s0, 0x000C           ## $s0 = 0000000C
/* 001C0 80B9A100 26310004 */  addiu   $s1, $s1, 0x0004           ## $s1 = 80B9A82C
/* 001C4 80B9A104 46803220 */  cvt.s.w $f8, $f6                   
/* 001C8 80B9A108 46004102 */  mul.s   $f4, $f8, $f0              
/* 001CC 80B9A10C 46049281 */  sub.s   $f10, $f18, $f4            
/* 001D0 80B9A110 E60AFFFC */  swc1    $f10, -0x0004($s0)         ## 00000008
/* 001D4 80B9A114 868B001C */  lh      $t3, 0x001C($s4)           ## 0000001C
/* 001D8 80B9A118 316C0003 */  andi    $t4, $t3, 0x0003           ## $t4 = 00000000
/* 001DC 80B9A11C 000C6840 */  sll     $t5, $t4,  1               
/* 001E0 80B9A120 02AD7021 */  addu    $t6, $s5, $t5              
/* 001E4 80B9A124 85CF0000 */  lh      $t7, 0x0000($t6)           ## 00000000
/* 001E8 80B9A128 024F082A */  slt     $at, $s2, $t7              
/* 001EC 80B9A12C 5420FFE1 */  bnel    $at, $zero, .L80B9A0B4     
/* 001F0 80B9A130 02002025 */  or      $a0, $s0, $zero            ## $a0 = 0000000C
.L80B9A134:
/* 001F4 80B9A134 8FBF002C */  lw      $ra, 0x002C($sp)           
/* 001F8 80B9A138 8FB00014 */  lw      $s0, 0x0014($sp)           
/* 001FC 80B9A13C 8FB10018 */  lw      $s1, 0x0018($sp)           
/* 00200 80B9A140 8FB2001C */  lw      $s2, 0x001C($sp)           
/* 00204 80B9A144 8FB30020 */  lw      $s3, 0x0020($sp)           
/* 00208 80B9A148 8FB40024 */  lw      $s4, 0x0024($sp)           
/* 0020C 80B9A14C 8FB50028 */  lw      $s5, 0x0028($sp)           
/* 00210 80B9A150 03E00008 */  jr      $ra                        
/* 00214 80B9A154 27BD0030 */  addiu   $sp, $sp, 0x0030           ## $sp = 00000000
