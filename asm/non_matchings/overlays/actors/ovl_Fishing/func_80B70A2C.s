.late_rodata
glabel D_80B7B534
    .float 1.2

glabel D_80B7B538
    .float 6.28

glabel D_80B7B53C
    .float 0.02

glabel D_80B7B540
    .float 0.025

.text
glabel func_80B70A2C
/* 0708C 80B70A2C 27BDFF48 */  addiu   $sp, $sp, 0xFF48           ## $sp = FFFFFF48
/* 07090 80B70A30 AFBF007C */  sw      $ra, 0x007C($sp)           
/* 07094 80B70A34 AFBE0078 */  sw      $s8, 0x0078($sp)           
/* 07098 80B70A38 AFB70074 */  sw      $s7, 0x0074($sp)           
/* 0709C 80B70A3C AFB60070 */  sw      $s6, 0x0070($sp)           
/* 070A0 80B70A40 AFB5006C */  sw      $s5, 0x006C($sp)           
/* 070A4 80B70A44 AFB40068 */  sw      $s4, 0x0068($sp)           
/* 070A8 80B70A48 AFB30064 */  sw      $s3, 0x0064($sp)           
/* 070AC 80B70A4C AFB20060 */  sw      $s2, 0x0060($sp)           
/* 070B0 80B70A50 AFB1005C */  sw      $s1, 0x005C($sp)           
/* 070B4 80B70A54 AFB00058 */  sw      $s0, 0x0058($sp)           
/* 070B8 80B70A58 F7BE0050 */  sdc1    $f30, 0x0050($sp)          
/* 070BC 80B70A5C F7BC0048 */  sdc1    $f28, 0x0048($sp)          
/* 070C0 80B70A60 F7BA0040 */  sdc1    $f26, 0x0040($sp)          
/* 070C4 80B70A64 F7B80038 */  sdc1    $f24, 0x0038($sp)          
/* 070C8 80B70A68 F7B60030 */  sdc1    $f22, 0x0030($sp)          
/* 070CC 80B70A6C F7B40028 */  sdc1    $f20, 0x0028($sp)          
/* 070D0 80B70A70 AFA600C0 */  sw      $a2, 0x00C0($sp)           
/* 070D4 80B70A74 8CAE07C0 */  lw      $t6, 0x07C0($a1)           ## 000007C0
/* 070D8 80B70A78 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 070DC 80B70A7C 44815000 */  mtc1    $at, $f10                  ## $f10 = 10.00
/* 070E0 80B70A80 8DCF0028 */  lw      $t7, 0x0028($t6)           ## 00000028
/* 070E4 80B70A84 C4840028 */  lwc1    $f4, 0x0028($a0)           ## 00000028
/* 070E8 80B70A88 30C600FF */  andi    $a2, $a2, 0x00FF           ## $a2 = 00000000
/* 070EC 80B70A8C 85F80002 */  lh      $t8, 0x0002($t7)           ## 00000002
/* 070F0 80B70A90 00A0B825 */  or      $s7, $a1, $zero            ## $s7 = 00000000
/* 070F4 80B70A94 0080F025 */  or      $s8, $a0, $zero            ## $s8 = 00000000
/* 070F8 80B70A98 44983000 */  mtc1    $t8, $f6                   ## $f6 = 0.00
/* 070FC 80B70A9C 3C014220 */  lui     $at, 0x4220                ## $at = 42200000
/* 07100 80B70AA0 46803220 */  cvt.s.w $f8, $f6                   
/* 07104 80B70AA4 460A4401 */  sub.s   $f16, $f8, $f10            
/* 07108 80B70AA8 4610203C */  c.lt.s  $f4, $f16                  
/* 0710C 80B70AAC 00000000 */  nop
/* 07110 80B70AB0 45020006 */  bc1fl   .L80B70ACC                 
/* 07114 80B70AB4 C7C001AC */  lwc1    $f0, 0x01AC($s8)           ## 000001AC
/* 07118 80B70AB8 54C00004 */  bnel    $a2, $zero, .L80B70ACC     
/* 0711C 80B70ABC C7C001AC */  lwc1    $f0, 0x01AC($s8)           ## 000001AC
/* 07120 80B70AC0 10000079 */  beq     $zero, $zero, .L80B70CA8   
/* 07124 80B70AC4 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 07128 80B70AC8 C7C001AC */  lwc1    $f0, 0x01AC($s8)           ## 000001AC
.L80B70ACC:
/* 0712C 80B70ACC 44819000 */  mtc1    $at, $f18                  ## $f18 = 40.00
/* 07130 80B70AD0 24160028 */  addiu   $s6, $zero, 0x0028         ## $s6 = 00000028
/* 07134 80B70AD4 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 07138 80B70AD8 4600903E */  c.le.s  $f18, $f0                  
/* 0713C 80B70ADC 27D20024 */  addiu   $s2, $s8, 0x0024           ## $s2 = 00000024
/* 07140 80B70AE0 27D300E4 */  addiu   $s3, $s8, 0x00E4           ## $s3 = 000000E4
/* 07144 80B70AE4 27B50098 */  addiu   $s5, $sp, 0x0098           ## $s5 = FFFFFFE0
/* 07148 80B70AE8 45020005 */  bc1fl   .L80B70B00                 
/* 0714C 80B70AEC 4481D000 */  mtc1    $at, $f26                  ## $f26 = 1.00
/* 07150 80B70AF0 3C0180B8 */  lui     $at, %hi(D_80B7B534)       ## $at = 80B80000
/* 07154 80B70AF4 10000003 */  beq     $zero, $zero, .L80B70B04   
/* 07158 80B70AF8 C43AB534 */  lwc1    $f26, %lo(D_80B7B534)($at) 
/* 0715C 80B70AFC 4481D000 */  mtc1    $at, $f26                  ## $f26 = -0.00
.L80B70B00:
/* 07160 80B70B00 2416001E */  addiu   $s6, $zero, 0x001E         ## $s6 = 0000001E
.L80B70B04:
/* 07164 80B70B04 1AC0004A */  blez    $s6, .L80B70C30            
/* 07168 80B70B08 00008025 */  or      $s0, $zero, $zero          ## $s0 = 00000000
/* 0716C 80B70B0C 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 07170 80B70B10 02E1A021 */  addu    $s4, $s7, $at              
/* 07174 80B70B14 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 07178 80B70B18 4481F000 */  mtc1    $at, $f30                  ## $f30 = 0.50
/* 0717C 80B70B1C 3C013FC0 */  lui     $at, 0x3FC0                ## $at = 3FC00000
/* 07180 80B70B20 4481E000 */  mtc1    $at, $f28                  ## $f28 = 1.50
/* 07184 80B70B24 3C014040 */  lui     $at, 0x4040                ## $at = 40400000
/* 07188 80B70B28 4481B000 */  mtc1    $at, $f22                  ## $f22 = 3.00
/* 0718C 80B70B2C 27B100A4 */  addiu   $s1, $sp, 0x00A4           ## $s1 = FFFFFFEC
.L80B70B30:
/* 07190 80B70B30 0C00CFBE */  jal     Math_Rand_ZeroFloat
              
/* 07194 80B70B34 4600E306 */  mov.s   $f12, $f28                 
/* 07198 80B70B38 461E0180 */  add.s   $f6, $f0, $f30             
/* 0719C 80B70B3C 3C0180B8 */  lui     $at, %hi(D_80B7B538)       ## $at = 80B80000
/* 071A0 80B70B40 C42CB538 */  lwc1    $f12, %lo(D_80B7B538)($at) 
/* 071A4 80B70B44 461A3502 */  mul.s   $f20, $f6, $f26            
/* 071A8 80B70B48 0C00CFBE */  jal     Math_Rand_ZeroFloat
              
/* 071AC 80B70B4C 00000000 */  nop
/* 071B0 80B70B50 46000606 */  mov.s   $f24, $f0                  
/* 071B4 80B70B54 0C0400A4 */  jal     sinf
              
/* 071B8 80B70B58 46000306 */  mov.s   $f12, $f0                  
/* 071BC 80B70B5C 46140202 */  mul.s   $f8, $f0, $f20             
/* 071C0 80B70B60 4600C306 */  mov.s   $f12, $f24                 
/* 071C4 80B70B64 0C041184 */  jal     cosf
              
/* 071C8 80B70B68 E7A80098 */  swc1    $f8, 0x0098($sp)           
/* 071CC 80B70B6C 46140282 */  mul.s   $f10, $f0, $f20            
/* 071D0 80B70B70 4600B306 */  mov.s   $f12, $f22                 
/* 071D4 80B70B74 0C00CFBE */  jal     Math_Rand_ZeroFloat
              
/* 071D8 80B70B78 E7AA00A0 */  swc1    $f10, 0x00A0($sp)          
/* 071DC 80B70B7C 46160100 */  add.s   $f4, $f0, $f22             
/* 071E0 80B70B80 3C0180B8 */  lui     $at, %hi(D_80B7B53C)       ## $at = 80B80000
/* 071E4 80B70B84 461A2402 */  mul.s   $f16, $f4, $f26            
/* 071E8 80B70B88 E7B0009C */  swc1    $f16, 0x009C($sp)          
/* 071EC 80B70B8C 8E480000 */  lw      $t0, 0x0000($s2)           ## 00000024
/* 071F0 80B70B90 AE280000 */  sw      $t0, 0x0000($s1)           ## FFFFFFEC
/* 071F4 80B70B94 8E590004 */  lw      $t9, 0x0004($s2)           ## 00000028
/* 071F8 80B70B98 AE390004 */  sw      $t9, 0x0004($s1)           ## FFFFFFF0
/* 071FC 80B70B9C 8E480008 */  lw      $t0, 0x0008($s2)           ## 0000002C
/* 07200 80B70BA0 AE280008 */  sw      $t0, 0x0008($s1)           ## FFFFFFF4
/* 07204 80B70BA4 C7A60098 */  lwc1    $f6, 0x0098($sp)           
/* 07208 80B70BA8 C7B200A4 */  lwc1    $f18, 0x00A4($sp)          
/* 0720C 80B70BAC C42CB53C */  lwc1    $f12, %lo(D_80B7B53C)($at) 
/* 07210 80B70BB0 46163202 */  mul.s   $f8, $f6, $f22             
/* 07214 80B70BB4 C7A600AC */  lwc1    $f6, 0x00AC($sp)           
/* 07218 80B70BB8 46089280 */  add.s   $f10, $f18, $f8            
/* 0721C 80B70BBC C7B200A0 */  lwc1    $f18, 0x00A0($sp)          
/* 07220 80B70BC0 E7AA00A4 */  swc1    $f10, 0x00A4($sp)          
/* 07224 80B70BC4 8EE907C0 */  lw      $t1, 0x07C0($s7)           ## 000007C0
/* 07228 80B70BC8 46169202 */  mul.s   $f8, $f18, $f22            
/* 0722C 80B70BCC 8D2A0028 */  lw      $t2, 0x0028($t1)           ## 00000028
/* 07230 80B70BD0 854B0002 */  lh      $t3, 0x0002($t2)           ## 00000002
/* 07234 80B70BD4 46083280 */  add.s   $f10, $f6, $f8             
/* 07238 80B70BD8 448B2000 */  mtc1    $t3, $f4                   ## $f4 = 0.00
/* 0723C 80B70BDC 00000000 */  nop
/* 07240 80B70BE0 46802420 */  cvt.s.w $f16, $f4                  
/* 07244 80B70BE4 E7AA00AC */  swc1    $f10, 0x00AC($sp)          
/* 07248 80B70BE8 0C00CFBE */  jal     Math_Rand_ZeroFloat
              
/* 0724C 80B70BEC E7B000A8 */  swc1    $f16, 0x00A8($sp)          
/* 07250 80B70BF0 3C0180B8 */  lui     $at, %hi(D_80B7B540)       ## $at = 80B80000
/* 07254 80B70BF4 C424B540 */  lwc1    $f4, %lo(D_80B7B540)($at)  
/* 07258 80B70BF8 8E851E10 */  lw      $a1, 0x1E10($s4)           ## 00001E10
/* 0725C 80B70BFC 02602025 */  or      $a0, $s3, $zero            ## $a0 = 000000E4
/* 07260 80B70C00 46040400 */  add.s   $f16, $f0, $f4             
/* 07264 80B70C04 02203025 */  or      $a2, $s1, $zero            ## $a2 = FFFFFFEC
/* 07268 80B70C08 02A03825 */  or      $a3, $s5, $zero            ## $a3 = FFFFFFE0
/* 0726C 80B70C0C 461A8482 */  mul.s   $f18, $f16, $f26           
/* 07270 80B70C10 0C2DA762 */  jal     func_80B69D88              
/* 07274 80B70C14 E7B20010 */  swc1    $f18, 0x0010($sp)          
/* 07278 80B70C18 26100001 */  addiu   $s0, $s0, 0x0001           ## $s0 = 00000001
/* 0727C 80B70C1C 00108400 */  sll     $s0, $s0, 16               
/* 07280 80B70C20 00108403 */  sra     $s0, $s0, 16               
/* 07284 80B70C24 0216082A */  slt     $at, $s0, $s6              
/* 07288 80B70C28 1420FFC1 */  bne     $at, $zero, .L80B70B30     
/* 0728C 80B70C2C 00000000 */  nop
.L80B70C30:
/* 07290 80B70C30 8FCD0024 */  lw      $t5, 0x0024($s8)           ## 00000024
/* 07294 80B70C34 27A600A4 */  addiu   $a2, $sp, 0x00A4           ## $a2 = FFFFFFEC
/* 07298 80B70C38 3C050001 */  lui     $a1, 0x0001                ## $a1 = 00010000
/* 0729C 80B70C3C ACCD0000 */  sw      $t5, 0x0000($a2)           ## FFFFFFEC
/* 072A0 80B70C40 8FCC0028 */  lw      $t4, 0x0028($s8)           ## 00000028
/* 072A4 80B70C44 00B72821 */  addu    $a1, $a1, $s7              
/* 072A8 80B70C48 3C014448 */  lui     $at, 0x4448                ## $at = 44480000
/* 072AC 80B70C4C ACCC0004 */  sw      $t4, 0x0004($a2)           ## FFFFFFF0
/* 072B0 80B70C50 8FCD002C */  lw      $t5, 0x002C($s8)           ## 0000002C
/* 072B4 80B70C54 44815000 */  mtc1    $at, $f10                  ## $f10 = 800.00
/* 072B8 80B70C58 24190096 */  addiu   $t9, $zero, 0x0096         ## $t9 = 00000096
/* 072BC 80B70C5C ACCD0008 */  sw      $t5, 0x0008($a2)           ## FFFFFFF4
/* 072C0 80B70C60 8EEE07C0 */  lw      $t6, 0x07C0($s7)           ## 000007C0
/* 072C4 80B70C64 2408005A */  addiu   $t0, $zero, 0x005A         ## $t0 = 0000005A
/* 072C8 80B70C68 27C400E4 */  addiu   $a0, $s8, 0x00E4           ## $a0 = 000000E4
/* 072CC 80B70C6C 8DCF0028 */  lw      $t7, 0x0028($t6)           ## 00000028
/* 072D0 80B70C70 3C0742C8 */  lui     $a3, 0x42C8                ## $a3 = 42C80000
/* 072D4 80B70C74 85F80002 */  lh      $t8, 0x0002($t7)           ## 00000002
/* 072D8 80B70C78 44983000 */  mtc1    $t8, $f6                   ## $f6 = 0.00
/* 072DC 80B70C7C 00000000 */  nop
/* 072E0 80B70C80 46803220 */  cvt.s.w $f8, $f6                   
/* 072E4 80B70C84 E7A800A8 */  swc1    $f8, 0x00A8($sp)           
/* 072E8 80B70C88 8CA51E10 */  lw      $a1, 0x1E10($a1)           ## 00011E10
/* 072EC 80B70C8C AFA80018 */  sw      $t0, 0x0018($sp)           
/* 072F0 80B70C90 AFB90014 */  sw      $t9, 0x0014($sp)           
/* 072F4 80B70C94 0C2DA70B */  jal     func_80B69C2C              
/* 072F8 80B70C98 E7AA0010 */  swc1    $f10, 0x0010($sp)          
/* 072FC 80B70C9C 2409001E */  addiu   $t1, $zero, 0x001E         ## $t1 = 0000001E
/* 07300 80B70CA0 A3C90151 */  sb      $t1, 0x0151($s8)           ## 00000151
/* 07304 80B70CA4 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
.L80B70CA8:
/* 07308 80B70CA8 8FBF007C */  lw      $ra, 0x007C($sp)           
/* 0730C 80B70CAC D7B40028 */  ldc1    $f20, 0x0028($sp)          
/* 07310 80B70CB0 D7B60030 */  ldc1    $f22, 0x0030($sp)          
/* 07314 80B70CB4 D7B80038 */  ldc1    $f24, 0x0038($sp)          
/* 07318 80B70CB8 D7BA0040 */  ldc1    $f26, 0x0040($sp)          
/* 0731C 80B70CBC D7BC0048 */  ldc1    $f28, 0x0048($sp)          
/* 07320 80B70CC0 D7BE0050 */  ldc1    $f30, 0x0050($sp)          
/* 07324 80B70CC4 8FB00058 */  lw      $s0, 0x0058($sp)           
/* 07328 80B70CC8 8FB1005C */  lw      $s1, 0x005C($sp)           
/* 0732C 80B70CCC 8FB20060 */  lw      $s2, 0x0060($sp)           
/* 07330 80B70CD0 8FB30064 */  lw      $s3, 0x0064($sp)           
/* 07334 80B70CD4 8FB40068 */  lw      $s4, 0x0068($sp)           
/* 07338 80B70CD8 8FB5006C */  lw      $s5, 0x006C($sp)           
/* 0733C 80B70CDC 8FB60070 */  lw      $s6, 0x0070($sp)           
/* 07340 80B70CE0 8FB70074 */  lw      $s7, 0x0074($sp)           
/* 07344 80B70CE4 8FBE0078 */  lw      $s8, 0x0078($sp)           
/* 07348 80B70CE8 03E00008 */  jr      $ra                        
/* 0734C 80B70CEC 27BD00B8 */  addiu   $sp, $sp, 0x00B8           ## $sp = 00000000
