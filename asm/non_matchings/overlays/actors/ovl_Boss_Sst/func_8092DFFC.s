.late_rodata
glabel D_809379AC
    .float 0.2

glabel D_809379B0
 .word 0x44228000

.text
glabel func_8092DFFC
/* 01A2C 8092DFFC 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 01A30 8092E000 AFB00020 */  sw      $s0, 0x0020($sp)           
/* 01A34 8092E004 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 01A38 8092E008 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 01A3C 8092E00C 2484014C */  addiu   $a0, $a0, 0x014C           ## $a0 = 0000014C
/* 01A40 8092E010 AFA5003C */  sw      $a1, 0x003C($sp)           
/* 01A44 8092E014 0C02927F */  jal     SkelAnime_Update
              
/* 01A48 8092E018 AFA4002C */  sw      $a0, 0x002C($sp)           
/* 01A4C 8092E01C AFA20030 */  sw      $v0, 0x0030($sp)           
/* 01A50 8092E020 820E0195 */  lb      $t6, 0x0195($s0)           ## 00000195
/* 01A54 8092E024 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 01A58 8092E028 55C0000D */  bnel    $t6, $zero, .L8092E060     
/* 01A5C 8092E02C 3C014234 */  lui     $at, 0x4234                ## $at = 42340000
/* 01A60 8092E030 0C0295B2 */  jal     Animation_OnFrame              
/* 01A64 8092E034 3C0540C0 */  lui     $a1, 0x40C0                ## $a1 = 40C00000
/* 01A68 8092E038 10400008 */  beq     $v0, $zero, .L8092E05C     
/* 01A6C 8092E03C 240F0001 */  addiu   $t7, $zero, 0x0001         ## $t7 = 00000001
/* 01A70 8092E040 3C013E80 */  lui     $at, 0x3E80                ## $at = 3E800000
/* 01A74 8092E044 44812000 */  mtc1    $at, $f4                   ## $f4 = 0.25
/* 01A78 8092E048 A20F0195 */  sb      $t7, 0x0195($s0)           ## 00000195
/* 01A7C 8092E04C 3C018093 */  lui     $at, %hi(D_809379AC)       ## $at = 80930000
/* 01A80 8092E050 E6040068 */  swc1    $f4, 0x0068($s0)           ## 00000068
/* 01A84 8092E054 C42679AC */  lwc1    $f6, %lo(D_809379AC)($at)  
/* 01A88 8092E058 E6060168 */  swc1    $f6, 0x0168($s0)           ## 00000168
.L8092E05C:
/* 01A8C 8092E05C 3C014234 */  lui     $at, 0x4234                ## $at = 42340000
.L8092E060:
/* 01A90 8092E060 44811000 */  mtc1    $at, $f2                   ## $f2 = 45.00
/* 01A94 8092E064 3C013FA0 */  lui     $at, 0x3FA0                ## $at = 3FA00000
/* 01A98 8092E068 44815000 */  mtc1    $at, $f10                  ## $f10 = 1.25
/* 01A9C 8092E06C C6080068 */  lwc1    $f8, 0x0068($s0)           ## 00000068
/* 01AA0 8092E070 3C063ECC */  lui     $a2, 0x3ECC                ## $a2 = 3ECC0000
/* 01AA4 8092E074 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 01AA8 8092E078 460A4402 */  mul.s   $f16, $f8, $f10            
/* 01AAC 8092E07C 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3ECCCCCD
/* 01AB0 8092E080 260403C4 */  addiu   $a0, $s0, 0x03C4           ## $a0 = 000003C4
/* 01AB4 8092E084 3C05C42F */  lui     $a1, 0xC42F                ## $a1 = C42F0000
/* 01AB8 8092E088 E6100068 */  swc1    $f16, 0x0068($s0)          ## 00000068
/* 01ABC 8092E08C C6000068 */  lwc1    $f0, 0x0068($s0)           ## 00000068
/* 01AC0 8092E090 4600103C */  c.lt.s  $f2, $f0                   
/* 01AC4 8092E094 00000000 */  nop
/* 01AC8 8092E098 45020004 */  bc1fl   .L8092E0AC                 
/* 01ACC 8092E09C E6000068 */  swc1    $f0, 0x0068($s0)           ## 00000068
/* 01AD0 8092E0A0 10000002 */  beq     $zero, $zero, .L8092E0AC   
/* 01AD4 8092E0A4 E6020068 */  swc1    $f2, 0x0068($s0)           ## 00000068
/* 01AD8 8092E0A8 E6000068 */  swc1    $f0, 0x0068($s0)           ## 00000068
.L8092E0AC:
/* 01ADC 8092E0AC 82180195 */  lb      $t8, 0x0195($s0)           ## 00000195
/* 01AE0 8092E0B0 13000037 */  beq     $t8, $zero, .L8092E190     
/* 01AE4 8092E0B4 00000000 */  nop
/* 01AE8 8092E0B8 44819000 */  mtc1    $at, $f18                  ## $f18 = 1.00
/* 01AEC 8092E0BC 3C054422 */  lui     $a1, 0x4422                ## $a1 = 44220000
/* 01AF0 8092E0C0 3C063ECC */  lui     $a2, 0x3ECC                ## $a2 = 3ECC0000
/* 01AF4 8092E0C4 8E070068 */  lw      $a3, 0x0068($s0)           ## 00000068
/* 01AF8 8092E0C8 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3ECCCCCD
/* 01AFC 8092E0CC 34A58000 */  ori     $a1, $a1, 0x8000           ## $a1 = 44228000
/* 01B00 8092E0D0 260403C4 */  addiu   $a0, $s0, 0x03C4           ## $a0 = 000003C4
/* 01B04 8092E0D4 0C01E0C4 */  jal     Math_SmoothStepToF
              
/* 01B08 8092E0D8 E7B20010 */  swc1    $f18, 0x0010($sp)          
/* 01B0C 8092E0DC 3C018093 */  lui     $at, %hi(D_809379B0)       ## $at = 80930000
/* 01B10 8092E0E0 C42C79B0 */  lwc1    $f12, %lo(D_809379B0)($at) 
/* 01B14 8092E0E4 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 01B18 8092E0E8 44812000 */  mtc1    $at, $f4                   ## $f4 = 10.00
/* 01B1C 8092E0EC 3C014334 */  lui     $at, 0x4334                ## $at = 43340000
/* 01B20 8092E0F0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 01B24 8092E0F4 4604003C */  c.lt.s  $f0, $f4                   
/* 01B28 8092E0F8 00000000 */  nop
/* 01B2C 8092E0FC 45020006 */  bc1fl   .L8092E118                 
/* 01B30 8092E100 C60603C4 */  lwc1    $f6, 0x03C4($s0)           ## 000003C4
/* 01B34 8092E104 0C24B897 */  jal     func_8092E25C              
/* 01B38 8092E108 E60C03C4 */  swc1    $f12, 0x03C4($s0)          ## 000003C4
/* 01B3C 8092E10C 10000011 */  beq     $zero, $zero, .L8092E154   
/* 01B40 8092E110 8FB90030 */  lw      $t9, 0x0030($sp)           
/* 01B44 8092E114 C60603C4 */  lwc1    $f6, 0x03C4($s0)           ## 000003C4
.L8092E118:
/* 01B48 8092E118 44811000 */  mtc1    $at, $f2                   ## $f2 = 0.00
/* 01B4C 8092E11C 3C014040 */  lui     $at, 0x4040                ## $at = 40400000
/* 01B50 8092E120 46066201 */  sub.s   $f8, $f12, $f6             
/* 01B54 8092E124 44815000 */  mtc1    $at, $f10                  ## $f10 = 3.00
/* 01B58 8092E128 00000000 */  nop
/* 01B5C 8092E12C 460A4002 */  mul.s   $f0, $f8, $f10             
/* 01B60 8092E130 4600103C */  c.lt.s  $f2, $f0                   
/* 01B64 8092E134 00000000 */  nop
/* 01B68 8092E138 45020003 */  bc1fl   .L8092E148                 
/* 01B6C 8092E13C C610000C */  lwc1    $f16, 0x000C($s0)          ## 0000000C
/* 01B70 8092E140 46001006 */  mov.s   $f0, $f2                   
/* 01B74 8092E144 C610000C */  lwc1    $f16, 0x000C($s0)          ## 0000000C
.L8092E148:
/* 01B78 8092E148 46008481 */  sub.s   $f18, $f16, $f0            
/* 01B7C 8092E14C E6120028 */  swc1    $f18, 0x0028($s0)          ## 00000028
/* 01B80 8092E150 8FB90030 */  lw      $t9, 0x0030($sp)           
.L8092E154:
/* 01B84 8092E154 3C038094 */  lui     $v1, %hi(D_80938CA8)       ## $v1 = 80940000
/* 01B88 8092E158 24638CA8 */  addiu   $v1, $v1, %lo(D_80938CA8)  ## $v1 = 80938CA8
/* 01B8C 8092E15C 17200008 */  bne     $t9, $zero, .L8092E180     
/* 01B90 8092E160 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 01B94 8092E164 44810000 */  mtc1    $at, $f0                   ## $f0 = 5.00
/* 01B98 8092E168 C4640008 */  lwc1    $f4, 0x0008($v1)           ## 80938CB0
/* 01B9C 8092E16C C4680014 */  lwc1    $f8, 0x0014($v1)           ## 80938CBC
/* 01BA0 8092E170 46002180 */  add.s   $f6, $f4, $f0              
/* 01BA4 8092E174 46004280 */  add.s   $f10, $f8, $f0             
/* 01BA8 8092E178 E4660008 */  swc1    $f6, 0x0008($v1)           ## 80938CB0
/* 01BAC 8092E17C E46A0014 */  swc1    $f10, 0x0014($v1)          ## 80938CBC
.L8092E180:
/* 01BB0 8092E180 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 01BB4 8092E184 44810000 */  mtc1    $at, $f0                   ## $f0 = 5.00
/* 01BB8 8092E188 10000017 */  beq     $zero, $zero, .L8092E1E8   
/* 01BBC 8092E18C 920903E4 */  lbu     $t1, 0x03E4($s0)           ## 000003E4
.L8092E190:
/* 01BC0 8092E190 0C01E107 */  jal     Math_ApproachF
              
/* 01BC4 8092E194 8E070068 */  lw      $a3, 0x0068($s0)           ## 00000068
/* 01BC8 8092E198 3C014334 */  lui     $at, 0x4334                ## $at = 43340000
/* 01BCC 8092E19C 44811000 */  mtc1    $at, $f2                   ## $f2 = 180.00
/* 01BD0 8092E1A0 C610000C */  lwc1    $f16, 0x000C($s0)          ## 0000000C
/* 01BD4 8092E1A4 26040028 */  addiu   $a0, $s0, 0x0028           ## $a0 = 00000028
/* 01BD8 8092E1A8 3C0641A0 */  lui     $a2, 0x41A0                ## $a2 = 41A00000
/* 01BDC 8092E1AC 46028481 */  sub.s   $f18, $f16, $f2            
/* 01BE0 8092E1B0 44059000 */  mfc1    $a1, $f18                  
/* 01BE4 8092E1B4 0C01DE80 */  jal     Math_StepToF
              
/* 01BE8 8092E1B8 00000000 */  nop
/* 01BEC 8092E1BC 3C038094 */  lui     $v1, %hi(D_80938CA8)       ## $v1 = 80940000
/* 01BF0 8092E1C0 24638CA8 */  addiu   $v1, $v1, %lo(D_80938CA8)  ## $v1 = 80938CA8
/* 01BF4 8092E1C4 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 01BF8 8092E1C8 44810000 */  mtc1    $at, $f0                   ## $f0 = 5.00
/* 01BFC 8092E1CC C4640004 */  lwc1    $f4, 0x0004($v1)           ## 80938CAC
/* 01C00 8092E1D0 C4680010 */  lwc1    $f8, 0x0010($v1)           ## 80938CB8
/* 01C04 8092E1D4 46002180 */  add.s   $f6, $f4, $f0              
/* 01C08 8092E1D8 46004280 */  add.s   $f10, $f8, $f0             
/* 01C0C 8092E1DC E4660004 */  swc1    $f6, 0x0004($v1)           ## 80938CAC
/* 01C10 8092E1E0 E46A0010 */  swc1    $f10, 0x0010($v1)          ## 80938CB8
/* 01C14 8092E1E4 920903E4 */  lbu     $t1, 0x03E4($s0)           ## 000003E4
.L8092E1E8:
/* 01C18 8092E1E8 3C0A8094 */  lui     $t2, %hi(D_80938C98)       ## $t2 = 80940000
/* 01C1C 8092E1EC 254A8C98 */  addiu   $t2, $t2, %lo(D_80938C98)  ## $t2 = 80938C98
/* 01C20 8092E1F0 312B0002 */  andi    $t3, $t1, 0x0002           ## $t3 = 00000000
/* 01C24 8092E1F4 11600014 */  beq     $t3, $zero, .L8092E248     
/* 01C28 8092E1F8 312CFFFC */  andi    $t4, $t1, 0xFFFC           ## $t4 = 00000000
/* 01C2C 8092E1FC A20C03E4 */  sb      $t4, 0x03E4($s0)           ## 000003E4
/* 01C30 8092E200 8D420000 */  lw      $v0, 0x0000($t2)           ## 80938C98
/* 01C34 8092E204 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 01C38 8092E208 3C064120 */  lui     $a2, 0x4120                ## $a2 = 41200000
/* 01C3C 8092E20C 904D03E4 */  lbu     $t5, 0x03E4($v0)           ## 000003E4
/* 01C40 8092E210 31AEFFFC */  andi    $t6, $t5, 0xFFFC           ## $t6 = 00000000
/* 01C44 8092E214 A04E03E4 */  sb      $t6, 0x03E4($v0)           ## 000003E4
/* 01C48 8092E218 8D430004 */  lw      $v1, 0x0004($t2)           ## 80938C9C
/* 01C4C 8092E21C 906F03E4 */  lbu     $t7, 0x03E4($v1)           ## 8093908C
/* 01C50 8092E220 31F8FFFC */  andi    $t8, $t7, 0xFFFC           ## $t8 = 00000000
/* 01C54 8092E224 A07803E4 */  sb      $t8, 0x03E4($v1)           ## 8093908C
/* 01C58 8092E228 860700B6 */  lh      $a3, 0x00B6($s0)           ## 000000B6
/* 01C5C 8092E22C E7A00010 */  swc1    $f0, 0x0010($sp)           
/* 01C60 8092E230 0C00BDC7 */  jal     func_8002F71C              
/* 01C64 8092E234 8FA4003C */  lw      $a0, 0x003C($sp)           
/* 01C68 8092E238 8FB9003C */  lw      $t9, 0x003C($sp)           
/* 01C6C 8092E23C 2405083E */  addiu   $a1, $zero, 0x083E         ## $a1 = 0000083E
/* 01C70 8092E240 0C00BDF7 */  jal     func_8002F7DC              
/* 01C74 8092E244 8F241C44 */  lw      $a0, 0x1C44($t9)           ## 00001C44
.L8092E248:
/* 01C78 8092E248 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 01C7C 8092E24C 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 01C80 8092E250 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000
/* 01C84 8092E254 03E00008 */  jr      $ra                        
/* 01C88 8092E258 00000000 */  nop
