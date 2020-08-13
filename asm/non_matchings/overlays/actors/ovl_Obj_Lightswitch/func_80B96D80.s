.late_rodata
glabel D_80B980F8
 .word 0x38808081, 0x00000000

.text
glabel func_80B96D80
/* 001E0 80B96D80 27BDFF80 */  addiu   $sp, $sp, 0xFF80           ## $sp = FFFFFF80
/* 001E4 80B96D84 AFB00048 */  sw      $s0, 0x0048($sp)           
/* 001E8 80B96D88 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 001EC 80B96D8C AFBF004C */  sw      $ra, 0x004C($sp)           
/* 001F0 80B96D90 AFA50084 */  sw      $a1, 0x0084($sp)           
/* 001F4 80B96D94 0C01DE1C */  jal     Math_Sins
              ## sins?
/* 001F8 80B96D98 848400B6 */  lh      $a0, 0x00B6($a0)           ## 000000B6
/* 001FC 80B96D9C E7A00070 */  swc1    $f0, 0x0070($sp)           
/* 00200 80B96DA0 0C01DE0D */  jal     Math_Coss
              ## coss?
/* 00204 80B96DA4 860400B6 */  lh      $a0, 0x00B6($s0)           ## 000000B6
/* 00208 80B96DA8 E7A0006C */  swc1    $f0, 0x006C($sp)           
/* 0020C 80B96DAC 860201BC */  lh      $v0, 0x01BC($s0)           ## 000001BC
/* 00210 80B96DB0 28411900 */  slti    $at, $v0, 0x1900           
/* 00214 80B96DB4 54200078 */  bnel    $at, $zero, .L80B96F98     
/* 00218 80B96DB8 8FBF004C */  lw      $ra, 0x004C($sp)           
/* 0021C 80B96DBC 44822000 */  mtc1    $v0, $f4                   ## $f4 = 0.00
/* 00220 80B96DC0 3C014270 */  lui     $at, 0x4270                ## $at = 42700000
/* 00224 80B96DC4 44811000 */  mtc1    $at, $f2                   ## $f2 = 60.00
/* 00228 80B96DC8 468021A0 */  cvt.s.w $f6, $f4                   
/* 0022C 80B96DCC 3C0180BA */  lui     $at, %hi(D_80B980F8)       ## $at = 80BA0000
/* 00230 80B96DD0 C42880F8 */  lwc1    $f8, %lo(D_80B980F8)($at)  
/* 00234 80B96DD4 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 00238 80B96DD8 44812000 */  mtc1    $at, $f4                   ## $f4 = 1.00
/* 0023C 80B96DDC 3C0143C8 */  lui     $at, 0x43C8                ## $at = 43C80000
/* 00240 80B96DE0 46083282 */  mul.s   $f10, $f6, $f8             
/* 00244 80B96DE4 44814000 */  mtc1    $at, $f8                   ## $f8 = 400.00
/* 00248 80B96DE8 460A2181 */  sub.s   $f6, $f4, $f10             
/* 0024C 80B96DEC 46083002 */  mul.s   $f0, $f6, $f8              
/* 00250 80B96DF0 4600103C */  c.lt.s  $f2, $f0                   
/* 00254 80B96DF4 00000000 */  nop
/* 00258 80B96DF8 45020004 */  bc1fl   .L80B96E0C                 
/* 0025C 80B96DFC 46000386 */  mov.s   $f14, $f0                  
/* 00260 80B96E00 10000002 */  beq     $zero, $zero, .L80B96E0C   
/* 00264 80B96E04 46001386 */  mov.s   $f14, $f2                  
/* 00268 80B96E08 46000386 */  mov.s   $f14, $f0                  
.L80B96E0C:
/* 0026C 80B96E0C 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 00270 80B96E10 E7AE0058 */  swc1    $f14, 0x0058($sp)          
/* 00274 80B96E14 3C0141F0 */  lui     $at, 0x41F0                ## $at = 41F00000
/* 00278 80B96E18 44818000 */  mtc1    $at, $f16                  ## $f16 = 30.00
/* 0027C 80B96E1C C7AE0058 */  lwc1    $f14, 0x0058($sp)          
/* 00280 80B96E20 46107101 */  sub.s   $f4, $f14, $f16            
/* 00284 80B96E24 46102280 */  add.s   $f10, $f4, $f16            
/* 00288 80B96E28 460A0082 */  mul.s   $f2, $f0, $f10             
/* 0028C 80B96E2C 4602803C */  c.lt.s  $f16, $f2                  
/* 00290 80B96E30 46101301 */  sub.s   $f12, $f2, $f16            
/* 00294 80B96E34 45000003 */  bc1f    .L80B96E44                 
/* 00298 80B96E38 E7AC0064 */  swc1    $f12, 0x0064($sp)          
/* 0029C 80B96E3C 1000000F */  beq     $zero, $zero, .L80B96E7C   
/* 002A0 80B96E40 46008086 */  mov.s   $f2, $f16                  
.L80B96E44:
/* 002A4 80B96E44 460C6202 */  mul.s   $f8, $f12, $f12            
/* 002A8 80B96E48 3C0142C8 */  lui     $at, 0x42C8                ## $at = 42C80000
/* 002AC 80B96E4C 44817000 */  mtc1    $at, $f14                  ## $f14 = 100.00
/* 002B0 80B96E50 3C014461 */  lui     $at, 0x4461                ## $at = 44610000
/* 002B4 80B96E54 44813000 */  mtc1    $at, $f6                   ## $f6 = 900.00
/* 002B8 80B96E58 00000000 */  nop
/* 002BC 80B96E5C 46083081 */  sub.s   $f2, $f6, $f8              
/* 002C0 80B96E60 460E103C */  c.lt.s  $f2, $f14                  
/* 002C4 80B96E64 00000000 */  nop
/* 002C8 80B96E68 45020003 */  bc1fl   .L80B96E78                 
/* 002CC 80B96E6C 46001004 */  sqrt.s  $f0, $f2                   
/* 002D0 80B96E70 46007086 */  mov.s   $f2, $f14                  
/* 002D4 80B96E74 46001004 */  sqrt.s  $f0, $f2                   
.L80B96E78:
/* 002D8 80B96E78 46000086 */  mov.s   $f2, $f0                   
.L80B96E7C:
/* 002DC 80B96E7C 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 002E0 80B96E80 E7A20068 */  swc1    $f2, 0x0068($sp)           
/* 002E4 80B96E84 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 002E8 80B96E88 44812000 */  mtc1    $at, $f4                   ## $f4 = 0.50
/* 002EC 80B96E8C C7A20068 */  lwc1    $f2, 0x0068($sp)           
/* 002F0 80B96E90 46040281 */  sub.s   $f10, $f0, $f4             
/* 002F4 80B96E94 46025302 */  mul.s   $f12, $f10, $f2            
/* 002F8 80B96E98 460C6080 */  add.s   $f2, $f12, $f12            
/* 002FC 80B96E9C 46001385 */  abs.s   $f14, $f2                  
/* 00300 80B96EA0 E7A20068 */  swc1    $f2, 0x0068($sp)           
/* 00304 80B96EA4 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 00308 80B96EA8 E7AE0058 */  swc1    $f14, 0x0058($sp)          
/* 0030C 80B96EAC 3C0141F0 */  lui     $at, 0x41F0                ## $at = 41F00000
/* 00310 80B96EB0 44818000 */  mtc1    $at, $f16                  ## $f16 = 30.00
/* 00314 80B96EB4 C7AE0058 */  lwc1    $f14, 0x0058($sp)          
/* 00318 80B96EB8 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 0031C 80B96EBC 44819000 */  mtc1    $at, $f18                  ## $f18 = 10.00
/* 00320 80B96EC0 460E8181 */  sub.s   $f6, $f16, $f14            
/* 00324 80B96EC4 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 00328 80B96EC8 44814000 */  mtc1    $at, $f8                   ## $f8 = 0.50
/* 0032C 80B96ECC C7A20068 */  lwc1    $f2, 0x0068($sp)           
/* 00330 80B96ED0 3C0680B9 */  lui     $a2, %hi(D_80B97F74)       ## $a2 = 80B90000
/* 00334 80B96ED4 46083102 */  mul.s   $f4, $f6, $f8              
/* 00338 80B96ED8 C7A60070 */  lwc1    $f6, 0x0070($sp)           
/* 0033C 80B96EDC 24C67F74 */  addiu   $a2, $a2, %lo(D_80B97F74)  ## $a2 = 80B97F74
/* 00340 80B96EE0 46120282 */  mul.s   $f10, $f0, $f18            
/* 00344 80B96EE4 240E0064 */  addiu   $t6, $zero, 0x0064         ## $t6 = 00000064
/* 00348 80B96EE8 240F00FF */  addiu   $t7, $zero, 0x00FF         ## $t7 = 000000FF
/* 0034C 80B96EEC 241800FF */  addiu   $t8, $zero, 0x00FF         ## $t8 = 000000FF
/* 00350 80B96EF0 241900A0 */  addiu   $t9, $zero, 0x00A0         ## $t9 = 000000A0
/* 00354 80B96EF4 240800A0 */  addiu   $t0, $zero, 0x00A0         ## $t0 = 000000A0
/* 00358 80B96EF8 240900FF */  addiu   $t1, $zero, 0x00FF         ## $t1 = 000000FF
/* 0035C 80B96EFC 46045300 */  add.s   $f12, $f10, $f4            
/* 00360 80B96F00 C7AA006C */  lwc1    $f10, 0x006C($sp)          
/* 00364 80B96F04 240A0001 */  addiu   $t2, $zero, 0x0001         ## $t2 = 00000001
/* 00368 80B96F08 240B0009 */  addiu   $t3, $zero, 0x0009         ## $t3 = 00000009
/* 0036C 80B96F0C 46066202 */  mul.s   $f8, $f12, $f6             
/* 00370 80B96F10 240C0001 */  addiu   $t4, $zero, 0x0001         ## $t4 = 00000001
/* 00374 80B96F14 00C03825 */  or      $a3, $a2, $zero            ## $a3 = 80B97F74
/* 00378 80B96F18 460A1102 */  mul.s   $f4, $f2, $f10             
/* 0037C 80B96F1C 8FA40084 */  lw      $a0, 0x0084($sp)           
/* 00380 80B96F20 27A50074 */  addiu   $a1, $sp, 0x0074           ## $a1 = FFFFFFF4
/* 00384 80B96F24 46044200 */  add.s   $f8, $f8, $f4              
/* 00388 80B96F28 C6040024 */  lwc1    $f4, 0x0024($s0)           ## 00000024
/* 0038C 80B96F2C 46082100 */  add.s   $f4, $f4, $f8              
/* 00390 80B96F30 E7A40074 */  swc1    $f4, 0x0074($sp)           
/* 00394 80B96F34 C6080028 */  lwc1    $f8, 0x0028($s0)           ## 00000028
/* 00398 80B96F38 C7A40064 */  lwc1    $f4, 0x0064($sp)           
/* 0039C 80B96F3C 46044200 */  add.s   $f8, $f8, $f4              
/* 003A0 80B96F40 46124100 */  add.s   $f4, $f8, $f18             
/* 003A4 80B96F44 460A6202 */  mul.s   $f8, $f12, $f10            
/* 003A8 80B96F48 E7A40078 */  swc1    $f4, 0x0078($sp)           
/* 003AC 80B96F4C 46061102 */  mul.s   $f4, $f2, $f6              
/* 003B0 80B96F50 C606002C */  lwc1    $f6, 0x002C($s0)           ## 0000002C
/* 003B4 80B96F54 AFAC003C */  sw      $t4, 0x003C($sp)           
/* 003B8 80B96F58 AFAB0038 */  sw      $t3, 0x0038($sp)           
/* 003BC 80B96F5C AFAA0034 */  sw      $t2, 0x0034($sp)           
/* 003C0 80B96F60 AFA00030 */  sw      $zero, 0x0030($sp)         
/* 003C4 80B96F64 AFA0002C */  sw      $zero, 0x002C($sp)         
/* 003C8 80B96F68 46044281 */  sub.s   $f10, $f8, $f4             
/* 003CC 80B96F6C AFA90028 */  sw      $t1, 0x0028($sp)           
/* 003D0 80B96F70 AFA80024 */  sw      $t0, 0x0024($sp)           
/* 003D4 80B96F74 AFB90020 */  sw      $t9, 0x0020($sp)           
/* 003D8 80B96F78 460A3200 */  add.s   $f8, $f6, $f10             
/* 003DC 80B96F7C AFB8001C */  sw      $t8, 0x001C($sp)           
/* 003E0 80B96F80 AFAF0018 */  sw      $t7, 0x0018($sp)           
/* 003E4 80B96F84 AFA00014 */  sw      $zero, 0x0014($sp)         
/* 003E8 80B96F88 E7A8007C */  swc1    $f8, 0x007C($sp)           
/* 003EC 80B96F8C 0C00A9AE */  jal     func_8002A6B8              
/* 003F0 80B96F90 AFAE0010 */  sw      $t6, 0x0010($sp)           
/* 003F4 80B96F94 8FBF004C */  lw      $ra, 0x004C($sp)           
.L80B96F98:
/* 003F8 80B96F98 8FB00048 */  lw      $s0, 0x0048($sp)           
/* 003FC 80B96F9C 27BD0080 */  addiu   $sp, $sp, 0x0080           ## $sp = 00000000
/* 00400 80B96FA0 03E00008 */  jr      $ra                        
/* 00404 80B96FA4 00000000 */  nop
