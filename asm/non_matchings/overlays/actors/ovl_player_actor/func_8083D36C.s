glabel func_8083D36C
/* 0B15C 8083D36C 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 0B160 8083D370 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 0B164 8083D374 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 0B168 8083D378 AFA40020 */  sw      $a0, 0x0020($sp)           
/* 0B16C 8083D37C 80AE014F */  lb      $t6, 0x014F($a1)           ## 0000014F
/* 0B170 8083D380 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 0B174 8083D384 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 0B178 8083D388 15C10005 */  bne     $t6, $at, .L8083D3A0       
/* 0B17C 8083D38C 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0B180 8083D390 94AF0088 */  lhu     $t7, 0x0088($a1)           ## 00000088
/* 0B184 8083D394 31F80001 */  andi    $t8, $t7, 0x0001           ## $t8 = 00000000
/* 0B188 8083D398 57000035 */  bnel    $t8, $zero, .L8083D470     
/* 0B18C 8083D39C 8E0F067C */  lw      $t7, 0x067C($s0)           ## 0000067C
.L8083D3A0:
/* 0B190 8083D3A0 0C20C959 */  jal     func_80832564              
/* 0B194 8083D3A4 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 0B198 8083D3A8 8219014F */  lb      $t9, 0x014F($s0)           ## 0000014F
/* 0B19C 8083D3AC 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 0B1A0 8083D3B0 5321000F */  beql    $t9, $at, .L8083D3F0       
/* 0B1A4 8083D3B4 8E0B0674 */  lw      $t3, 0x0674($s0)           ## 00000674
/* 0B1A8 8083D3B8 8E020680 */  lw      $v0, 0x0680($s0)           ## 00000680
/* 0B1AC 8083D3BC 2401FBFF */  addiu   $at, $zero, 0xFBFF         ## $at = FFFFFBFF
/* 0B1B0 8083D3C0 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 0B1B4 8083D3C4 30480400 */  andi    $t0, $v0, 0x0400           ## $t0 = 00000000
/* 0B1B8 8083D3C8 11000008 */  beq     $t0, $zero, .L8083D3EC     
/* 0B1BC 8083D3CC 00414824 */  and     $t1, $v0, $at              
/* 0B1C0 8083D3D0 AE090680 */  sw      $t1, 0x0680($s0)           ## 00000680
/* 0B1C4 8083D3D4 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0B1C8 8083D3D8 0C20F44B */  jal     func_8083D12C              
/* 0B1CC 8083D3DC 00003025 */  or      $a2, $zero, $zero          ## $a2 = 00000000
/* 0B1D0 8083D3E0 240A0001 */  addiu   $t2, $zero, 0x0001         ## $t2 = 00000001
/* 0B1D4 8083D3E4 10000021 */  beq     $zero, $zero, .L8083D46C   
/* 0B1D8 8083D3E8 A20A084F */  sb      $t2, 0x084F($s0)           ## 0000084F
.L8083D3EC:
/* 0B1DC 8083D3EC 8E0B0674 */  lw      $t3, 0x0674($s0)           ## 00000674
.L8083D3F0:
/* 0B1E0 8083D3F0 3C0C8084 */  lui     $t4, %hi(func_80844A44)    ## $t4 = 80840000
/* 0B1E4 8083D3F4 258C4A44 */  addiu   $t4, $t4, %lo(func_80844A44) ## $t4 = 80844A44
/* 0B1E8 8083D3F8 156C000C */  bne     $t3, $t4, .L8083D42C       
/* 0B1EC 8083D3FC 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0B1F0 8083D400 3C068085 */  lui     $a2, %hi(func_8084DC48)    ## $a2 = 80850000
/* 0B1F4 8083D404 24C6DC48 */  addiu   $a2, $a2, %lo(func_8084DC48) ## $a2 = 8084DC48
/* 0B1F8 8083D408 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0B1FC 8083D40C 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 0B200 8083D410 0C20D716 */  jal     func_80835C58              
/* 0B204 8083D414 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 0B208 8083D418 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0B20C 8083D41C 0C20F4CC */  jal     func_8083D330              
/* 0B210 8083D420 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 0B214 8083D424 10000012 */  beq     $zero, $zero, .L8083D470   
/* 0B218 8083D428 8E0F067C */  lw      $t7, 0x067C($s0)           ## 0000067C
.L8083D42C:
/* 0B21C 8083D42C 3C068085 */  lui     $a2, %hi(func_8084D610)    ## $a2 = 80850000
/* 0B220 8083D430 24C6D610 */  addiu   $a2, $a2, %lo(func_8084D610) ## $a2 = 8084D610
/* 0B224 8083D434 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 0B228 8083D438 0C20D716 */  jal     func_80835C58              
/* 0B22C 8083D43C 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 0B230 8083D440 960D0088 */  lhu     $t5, 0x0088($s0)           ## 00000088
/* 0B234 8083D444 3C060400 */  lui     $a2, 0x0400                ## $a2 = 04000000
/* 0B238 8083D448 24C632E0 */  addiu   $a2, $a2, 0x32E0           ## $a2 = 040032E0
/* 0B23C 8083D44C 31AE0001 */  andi    $t6, $t5, 0x0001           ## $t6 = 00000000
/* 0B240 8083D450 11C00004 */  beq     $t6, $zero, .L8083D464     
/* 0B244 8083D454 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0B248 8083D458 3C060400 */  lui     $a2, 0x0400                ## $a2 = 04000000
/* 0B24C 8083D45C 10000001 */  beq     $zero, $zero, .L8083D464   
/* 0B250 8083D460 24C63330 */  addiu   $a2, $a2, 0x3330           ## $a2 = 04003330
.L8083D464:
/* 0B254 8083D464 0C20CAC3 */  jal     func_80832B0C              
/* 0B258 8083D468 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
.L8083D46C:
/* 0B25C 8083D46C 8E0F067C */  lw      $t7, 0x067C($s0)           ## 0000067C
.L8083D470:
/* 0B260 8083D470 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0B264 8083D474 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 0B268 8083D478 000FC100 */  sll     $t8, $t7,  4               
/* 0B26C 8083D47C 07010008 */  bgez    $t8, .L8083D4A0            
/* 0B270 8083D480 240701F4 */  addiu   $a3, $zero, 0x01F4         ## $a3 = 000001F4
/* 0B274 8083D484 8E190678 */  lw      $t9, 0x0678($s0)           ## 00000678
/* 0B278 8083D488 C6040084 */  lwc1    $f4, 0x0084($s0)           ## 00000084
/* 0B27C 8083D48C C726002C */  lwc1    $f6, 0x002C($t9)           ## 0000002C
/* 0B280 8083D490 4606203C */  c.lt.s  $f4, $f6                   
/* 0B284 8083D494 00000000 */  nop
/* 0B288 8083D498 45020014 */  bc1fl   .L8083D4EC                 
/* 0B28C 8083D49C 8E09067C */  lw      $t1, 0x067C($s0)           ## 0000067C
.L8083D4A0:
/* 0B290 8083D4A0 0C20F3EA */  jal     func_8083CFA8              
/* 0B294 8083D4A4 8E060060 */  lw      $a2, 0x0060($s0)           ## 00000060
/* 0B298 8083D4A8 1040000F */  beq     $v0, $zero, .L8083D4E8     
/* 0B29C 8083D4AC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0B2A0 8083D4B0 0C00BDF7 */  jal     func_8002F7DC              
/* 0B2A4 8083D4B4 24052889 */  addiu   $a1, $zero, 0x2889         ## $a1 = 00002889
/* 0B2A8 8083D4B8 86080896 */  lh      $t0, 0x0896($s0)           ## 00000896
/* 0B2AC 8083D4BC 3C014448 */  lui     $at, 0x4448                ## $at = 44480000
/* 0B2B0 8083D4C0 44814000 */  mtc1    $at, $f8                   ## $f8 = 800.00
/* 0B2B4 8083D4C4 44885000 */  mtc1    $t0, $f10                  ## $f10 = 0.00
/* 0B2B8 8083D4C8 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0B2BC 8083D4CC 46805420 */  cvt.s.w $f16, $f10                 
/* 0B2C0 8083D4D0 4610403C */  c.lt.s  $f8, $f16                  
/* 0B2C4 8083D4D4 00000000 */  nop
/* 0B2C8 8083D4D8 45020004 */  bc1fl   .L8083D4EC                 
/* 0B2CC 8083D4DC 8E09067C */  lw      $t1, 0x067C($s0)           ## 0000067C
/* 0B2D0 8083D4E0 0C20C9A6 */  jal     func_80832698              
/* 0B2D4 8083D4E4 24056804 */  addiu   $a1, $zero, 0x6804         ## $a1 = 00006804
.L8083D4E8:
/* 0B2D8 8083D4E8 8E09067C */  lw      $t1, 0x067C($s0)           ## 0000067C
.L8083D4EC:
/* 0B2DC 8083D4EC 3C010800 */  lui     $at, 0x0800                ## $at = 08000000
/* 0B2E0 8083D4F0 8E0B0680 */  lw      $t3, 0x0680($s0)           ## 00000680
/* 0B2E4 8083D4F4 01215025 */  or      $t2, $t1, $at              ## $t2 = 08000000
/* 0B2E8 8083D4F8 44809000 */  mtc1    $zero, $f18                ## $f18 = 0.00
/* 0B2EC 8083D4FC 3C01FFF3 */  lui     $at, 0xFFF3                ## $at = FFF30000
/* 0B2F0 8083D500 3421FFFF */  ori     $at, $at, 0xFFFF           ## $at = FFF3FFFF
/* 0B2F4 8083D504 AE0A067C */  sw      $t2, 0x067C($s0)           ## 0000067C
/* 0B2F8 8083D508 01417024 */  and     $t6, $t2, $at              
/* 0B2FC 8083D50C 356C0400 */  ori     $t4, $t3, 0x0400           ## $t4 = 00000400
/* 0B300 8083D510 AE0C0680 */  sw      $t4, 0x0680($s0)           ## 00000680
/* 0B304 8083D514 AE0E067C */  sw      $t6, 0x067C($s0)           ## 0000067C
/* 0B308 8083D518 E6120854 */  swc1    $f18, 0x0854($s0)          ## 00000854
/* 0B30C 8083D51C 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0B310 8083D520 0C0239D4 */  jal     func_8008E750              
/* 0B314 8083D524 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 0B318 8083D528 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 0B31C 8083D52C 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 0B320 8083D530 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 0B324 8083D534 03E00008 */  jr      $ra                        
/* 0B328 8083D538 00000000 */  nop
