glabel func_80919A40
/* 04030 80919A40 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 04034 80919A44 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 04038 80919A48 AFB10020 */  sw      $s1, 0x0020($sp)           
/* 0403C 80919A4C 240E0001 */  addiu   $t6, $zero, 0x0001         ## $t6 = 00000001
/* 04040 80919A50 00808825 */  or      $s1, $a0, $zero            ## $s1 = 00000000
/* 04044 80919A54 AFB0001C */  sw      $s0, 0x001C($sp)           
/* 04048 80919A58 AFAE0010 */  sw      $t6, 0x0010($sp)           
/* 0404C 80919A5C 24060000 */  addiu   $a2, $zero, 0x0000         ## $a2 = 00000000
/* 04050 80919A60 0C245A2C */  jal     func_809168B0              
/* 04054 80919A64 3C07C0A0 */  lui     $a3, 0xC0A0                ## $a3 = C0A00000
/* 04058 80919A68 862F0194 */  lh      $t7, 0x0194($s1)           ## 00000194
/* 0405C 80919A6C 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 04060 80919A70 31F8003F */  andi    $t8, $t7, 0x003F           ## $t8 = 00000000
/* 04064 80919A74 57000004 */  bnel    $t8, $zero, .L80919A88     
/* 04068 80919A78 262400B4 */  addiu   $a0, $s1, 0x00B4           ## $a0 = 000000B4
/* 0406C 80919A7C 0C00BE0A */  jal     Audio_PlayActorSound2
              
/* 04070 80919A80 2405380F */  addiu   $a1, $zero, 0x380F         ## $a1 = 0000380F
/* 04074 80919A84 262400B4 */  addiu   $a0, $s1, 0x00B4           ## $a0 = 000000B4
.L80919A88:
/* 04078 80919A88 24058000 */  addiu   $a1, $zero, 0x8000         ## $a1 = FFFF8000
/* 0407C 80919A8C 24060003 */  addiu   $a2, $zero, 0x0003         ## $a2 = 00000003
/* 04080 80919A90 0C01E1EF */  jal     Math_ApproachS
              
/* 04084 80919A94 240703E8 */  addiu   $a3, $zero, 0x03E8         ## $a3 = 000003E8
/* 04088 80919A98 96390088 */  lhu     $t9, 0x0088($s1)           ## 00000088
/* 0408C 80919A9C 33280008 */  andi    $t0, $t9, 0x0008           ## $t0 = 00000000
/* 04090 80919AA0 51000048 */  beql    $t0, $zero, .L80919BC4     
/* 04094 80919AA4 862B01D2 */  lh      $t3, 0x01D2($s1)           ## 000001D2
/* 04098 80919AA8 863000B6 */  lh      $s0, 0x00B6($s1)           ## 000000B6
/* 0409C 80919AAC 34018000 */  ori     $at, $zero, 0x8000         ## $at = 00008000
/* 040A0 80919AB0 8622007E */  lh      $v0, 0x007E($s1)           ## 0000007E
/* 040A4 80919AB4 02018021 */  addu    $s0, $s0, $at              
/* 040A8 80919AB8 00108400 */  sll     $s0, $s0, 16               
/* 040AC 80919ABC 00108403 */  sra     $s0, $s0, 16               
/* 040B0 80919AC0 0202082A */  slt     $at, $s0, $v0              
/* 040B4 80919AC4 1020000C */  beq     $at, $zero, .L80919AF8     
/* 040B8 80919AC8 02021823 */  subu    $v1, $s0, $v0              
/* 040BC 80919ACC 00501823 */  subu    $v1, $v0, $s0              
/* 040C0 80919AD0 00031C00 */  sll     $v1, $v1, 16               
/* 040C4 80919AD4 00031C03 */  sra     $v1, $v1, 16               
/* 040C8 80919AD8 04610003 */  bgez    $v1, .L80919AE8            
/* 040CC 80919ADC 00034843 */  sra     $t1, $v1,  1               
/* 040D0 80919AE0 24610001 */  addiu   $at, $v1, 0x0001           ## $at = 00000001
/* 040D4 80919AE4 00014843 */  sra     $t1, $at,  1               
.L80919AE8:
/* 040D8 80919AE8 02098021 */  addu    $s0, $s0, $t1              
/* 040DC 80919AEC 00108400 */  sll     $s0, $s0, 16               
/* 040E0 80919AF0 1000000A */  beq     $zero, $zero, .L80919B1C   
/* 040E4 80919AF4 00108403 */  sra     $s0, $s0, 16               
.L80919AF8:
/* 040E8 80919AF8 00031C00 */  sll     $v1, $v1, 16               
/* 040EC 80919AFC 00031C03 */  sra     $v1, $v1, 16               
/* 040F0 80919B00 04610003 */  bgez    $v1, .L80919B10            
/* 040F4 80919B04 00035043 */  sra     $t2, $v1,  1               
/* 040F8 80919B08 24610001 */  addiu   $at, $v1, 0x0001           ## $at = 00000001
/* 040FC 80919B0C 00015043 */  sra     $t2, $at,  1               
.L80919B10:
/* 04100 80919B10 004A8021 */  addu    $s0, $v0, $t2              
/* 04104 80919B14 00108400 */  sll     $s0, $s0, 16               
/* 04108 80919B18 00108403 */  sra     $s0, $s0, 16               
.L80919B1C:
/* 0410C 80919B1C 3C014000 */  lui     $at, 0x4000                ## $at = 40000000
/* 04110 80919B20 44816000 */  mtc1    $at, $f12                  ## $f12 = 2.00
/* 04114 80919B24 0C00CFC8 */  jal     Rand_CenteredFloat
              
/* 04118 80919B28 00000000 */  nop
/* 0411C 80919B2C 0C03F66B */  jal     Rand_ZeroOne
              ## Rand.Next() float
/* 04120 80919B30 E7A00028 */  swc1    $f0, 0x0028($sp)           
/* 04124 80919B34 00102400 */  sll     $a0, $s0, 16               
/* 04128 80919B38 00042403 */  sra     $a0, $a0, 16               
/* 0412C 80919B3C 0C01DE0D */  jal     Math_CosS
              ## coss?
/* 04130 80919B40 E7A0002C */  swc1    $f0, 0x002C($sp)           
/* 04134 80919B44 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 04138 80919B48 44811000 */  mtc1    $at, $f2                   ## $f2 = 5.00
/* 0413C 80919B4C C7A4002C */  lwc1    $f4, 0x002C($sp)           
/* 04140 80919B50 C7B00028 */  lwc1    $f16, 0x0028($sp)          
/* 04144 80919B54 3C014000 */  lui     $at, 0x4000                ## $at = 40000000
/* 04148 80919B58 46022182 */  mul.s   $f6, $f4, $f2              
/* 0414C 80919B5C C624002C */  lwc1    $f4, 0x002C($s1)           ## 0000002C
/* 04150 80919B60 44816000 */  mtc1    $at, $f12                  ## $f12 = 2.00
/* 04154 80919B64 46061200 */  add.s   $f8, $f2, $f6              
/* 04158 80919B68 46080282 */  mul.s   $f10, $f0, $f8             
/* 0415C 80919B6C 46105480 */  add.s   $f18, $f10, $f16           
/* 04160 80919B70 46122180 */  add.s   $f6, $f4, $f18             
/* 04164 80919B74 0C00CFC8 */  jal     Rand_CenteredFloat
              
/* 04168 80919B78 E626002C */  swc1    $f6, 0x002C($s1)           ## 0000002C
/* 0416C 80919B7C 0C03F66B */  jal     Rand_ZeroOne
              ## Rand.Next() float
/* 04170 80919B80 E7A00028 */  swc1    $f0, 0x0028($sp)           
/* 04174 80919B84 00102400 */  sll     $a0, $s0, 16               
/* 04178 80919B88 00042403 */  sra     $a0, $a0, 16               
/* 0417C 80919B8C 0C01DE1C */  jal     Math_SinS
              ## sins?
/* 04180 80919B90 E7A0002C */  swc1    $f0, 0x002C($sp)           
/* 04184 80919B94 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 04188 80919B98 44811000 */  mtc1    $at, $f2                   ## $f2 = 5.00
/* 0418C 80919B9C C7A8002C */  lwc1    $f8, 0x002C($sp)           
/* 04190 80919BA0 C7B20028 */  lwc1    $f18, 0x0028($sp)          
/* 04194 80919BA4 46024282 */  mul.s   $f10, $f8, $f2             
/* 04198 80919BA8 C6280024 */  lwc1    $f8, 0x0024($s1)           ## 00000024
/* 0419C 80919BAC 460A1400 */  add.s   $f16, $f2, $f10            
/* 041A0 80919BB0 46100102 */  mul.s   $f4, $f0, $f16             
/* 041A4 80919BB4 46122180 */  add.s   $f6, $f4, $f18             
/* 041A8 80919BB8 46064280 */  add.s   $f10, $f8, $f6             
/* 041AC 80919BBC E62A0024 */  swc1    $f10, 0x0024($s1)          ## 00000024
/* 041B0 80919BC0 862B01D2 */  lh      $t3, 0x01D2($s1)           ## 000001D2
.L80919BC4:
/* 041B4 80919BC4 3C0142C8 */  lui     $at, 0x42C8                ## $at = 42C80000
/* 041B8 80919BC8 55600017 */  bnel    $t3, $zero, .L80919C28     
/* 041BC 80919BCC 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 041C0 80919BD0 44811000 */  mtc1    $at, $f2                   ## $f2 = 100.00
/* 041C4 80919BD4 3C01C316 */  lui     $at, 0xC316                ## $at = C3160000
/* 041C8 80919BD8 44818000 */  mtc1    $at, $f16                  ## $f16 = -150.00
/* 041CC 80919BDC C6240024 */  lwc1    $f4, 0x0024($s1)           ## 00000024
/* 041D0 80919BE0 3C01C3AF */  lui     $at, 0xC3AF                ## $at = C3AF0000
/* 041D4 80919BE4 46048001 */  sub.s   $f0, $f16, $f4             
/* 041D8 80919BE8 46000005 */  abs.s   $f0, $f0                   
/* 041DC 80919BEC 4602003C */  c.lt.s  $f0, $f2                   
/* 041E0 80919BF0 00000000 */  nop
/* 041E4 80919BF4 4502000C */  bc1fl   .L80919C28                 
/* 041E8 80919BF8 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 041EC 80919BFC 44819000 */  mtc1    $at, $f18                  ## $f18 = -350.00
/* 041F0 80919C00 C628002C */  lwc1    $f8, 0x002C($s1)           ## 0000002C
/* 041F4 80919C04 46089001 */  sub.s   $f0, $f18, $f8             
/* 041F8 80919C08 46000005 */  abs.s   $f0, $f0                   
/* 041FC 80919C0C 4602003C */  c.lt.s  $f0, $f2                   
/* 04200 80919C10 00000000 */  nop
/* 04204 80919C14 45020004 */  bc1fl   .L80919C28                 
/* 04208 80919C18 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 0420C 80919C1C 0C24584B */  jal     func_8091612C              
/* 04210 80919C20 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 04214 80919C24 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80919C28:
/* 04218 80919C28 8FB0001C */  lw      $s0, 0x001C($sp)           
/* 0421C 80919C2C 8FB10020 */  lw      $s1, 0x0020($sp)           
/* 04220 80919C30 03E00008 */  jr      $ra                        
/* 04224 80919C34 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000
