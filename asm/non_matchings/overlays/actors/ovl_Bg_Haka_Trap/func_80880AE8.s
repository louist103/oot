.late_rodata
glabel D_80881060
    .float 0.03
glabel D_80881064
    .float 0.03

.text
glabel func_80880AE8
/* 00E48 80880AE8 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 00E4C 80880AEC AFBF001C */  sw      $ra, 0x001C($sp)           
/* 00E50 80880AF0 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 00E54 80880AF4 AFA50024 */  sw      $a1, 0x0024($sp)           
/* 00E58 80880AF8 908E0168 */  lbu     $t6, 0x0168($a0)           ## 00000168
/* 00E5C 80880AFC 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 00E60 80880B00 51C0001A */  beql    $t6, $zero, .L80880B6C     
/* 00E64 80880B04 86090034 */  lh      $t1, 0x0034($s0)           ## 00000034
/* 00E68 80880B08 860F0034 */  lh      $t7, 0x0034($s0)           ## 00000034
/* 00E6C 80880B0C 3C018088 */  lui     $at, %hi(D_80881060)       ## $at = 80880000
/* 00E70 80880B10 C4281060 */  lwc1    $f8, %lo(D_80881060)($at)  
/* 00E74 80880B14 448F2000 */  mtc1    $t7, $f4                   ## $f4 = 0.00
/* 00E78 80880B18 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 00E7C 80880B1C 44818000 */  mtc1    $at, $f16                  ## $f16 = 5.00
/* 00E80 80880B20 468021A0 */  cvt.s.w $f6, $f4                   
/* 00E84 80880B24 24840034 */  addiu   $a0, $a0, 0x0034           ## $a0 = 00000034
/* 00E88 80880B28 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 00E8C 80880B2C 46083282 */  mul.s   $f10, $f6, $f8             
/* 00E90 80880B30 46105480 */  add.s   $f18, $f10, $f16           
/* 00E94 80880B34 4600910D */  trunc.w.s $f4, $f18                  
/* 00E98 80880B38 44062000 */  mfc1    $a2, $f4                   
/* 00E9C 80880B3C 00000000 */  nop
/* 00EA0 80880B40 00063400 */  sll     $a2, $a2, 16               
/* 00EA4 80880B44 0C01DE2B */  jal     Math_ApproxUpdateScaledS
              
/* 00EA8 80880B48 00063403 */  sra     $a2, $a2, 16               
/* 00EAC 80880B4C 1040001D */  beq     $v0, $zero, .L80880BC4     
/* 00EB0 80880B50 24190028 */  addiu   $t9, $zero, 0x0028         ## $t9 = 00000028
/* 00EB4 80880B54 3C088088 */  lui     $t0, %hi(func_808809B0)    ## $t0 = 80880000
/* 00EB8 80880B58 250809B0 */  addiu   $t0, $t0, %lo(func_808809B0) ## $t0 = 808809B0
/* 00EBC 80880B5C A2190168 */  sb      $t9, 0x0168($s0)           ## 00000168
/* 00EC0 80880B60 10000018 */  beq     $zero, $zero, .L80880BC4   
/* 00EC4 80880B64 AE080164 */  sw      $t0, 0x0164($s0)           ## 00000164
/* 00EC8 80880B68 86090034 */  lh      $t1, 0x0034($s0)           ## 00000034
.L80880B6C:
/* 00ECC 80880B6C 3C018088 */  lui     $at, %hi(D_80881064)       ## $at = 80880000
/* 00ED0 80880B70 C42A1064 */  lwc1    $f10, %lo(D_80881064)($at) 
/* 00ED4 80880B74 44893000 */  mtc1    $t1, $f6                   ## $f6 = 0.00
/* 00ED8 80880B78 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 00EDC 80880B7C 44819000 */  mtc1    $at, $f18                  ## $f18 = 5.00
/* 00EE0 80880B80 46803220 */  cvt.s.w $f8, $f6                   
/* 00EE4 80880B84 26040034 */  addiu   $a0, $s0, 0x0034           ## $a0 = 00000034
/* 00EE8 80880B88 24053A00 */  addiu   $a1, $zero, 0x3A00         ## $a1 = 00003A00
/* 00EEC 80880B8C 460A4402 */  mul.s   $f16, $f8, $f10            
/* 00EF0 80880B90 46128100 */  add.s   $f4, $f16, $f18            
/* 00EF4 80880B94 4600218D */  trunc.w.s $f6, $f4                   
/* 00EF8 80880B98 44063000 */  mfc1    $a2, $f6                   
/* 00EFC 80880B9C 00000000 */  nop
/* 00F00 80880BA0 00063400 */  sll     $a2, $a2, 16               
/* 00F04 80880BA4 0C01DE2B */  jal     Math_ApproxUpdateScaledS
              
/* 00F08 80880BA8 00063403 */  sra     $a2, $a2, 16               
/* 00F0C 80880BAC 10400005 */  beq     $v0, $zero, .L80880BC4     
/* 00F10 80880BB0 240B0064 */  addiu   $t3, $zero, 0x0064         ## $t3 = 00000064
/* 00F14 80880BB4 3C0C8088 */  lui     $t4, %hi(func_80880C0C)    ## $t4 = 80880000
/* 00F18 80880BB8 258C0C0C */  addiu   $t4, $t4, %lo(func_80880C0C) ## $t4 = 80880C0C
/* 00F1C 80880BBC A20B0168 */  sb      $t3, 0x0168($s0)           ## 00000168
/* 00F20 80880BC0 AE0C0164 */  sw      $t4, 0x0164($s0)           ## 00000164
.L80880BC4:
/* 00F24 80880BC4 86060034 */  lh      $a2, 0x0034($s0)           ## 00000034
/* 00F28 80880BC8 860D00B8 */  lh      $t5, 0x00B8($s0)           ## 000000B8
/* 00F2C 80880BCC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00F30 80880BD0 28C11801 */  slti    $at, $a2, 0x1801           
/* 00F34 80880BD4 01A67021 */  addu    $t6, $t5, $a2              
/* 00F38 80880BD8 14200004 */  bne     $at, $zero, .L80880BEC     
/* 00F3C 80880BDC A60E00B8 */  sh      $t6, 0x00B8($s0)           ## 000000B8
/* 00F40 80880BE0 0C00BE5D */  jal     func_8002F974              
/* 00F44 80880BE4 24052057 */  addiu   $a1, $zero, 0x2057         ## $a1 = 00002057
/* 00F48 80880BE8 86060034 */  lh      $a2, 0x0034($s0)           ## 00000034
.L80880BEC:
/* 00F4C 80880BEC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00F50 80880BF0 0C220279 */  jal     func_808809E4              
/* 00F54 80880BF4 8FA50024 */  lw      $a1, 0x0024($sp)           
/* 00F58 80880BF8 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00F5C 80880BFC 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 00F60 80880C00 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 00F64 80880C04 03E00008 */  jr      $ra                        
/* 00F68 80880C08 00000000 */  nop