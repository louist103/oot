glabel func_80B15FE8
/* 02548 80B15FE8 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 0254C 80B15FEC AFB00020 */  sw      $s0, 0x0020($sp)           
/* 02550 80B15FF0 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 02554 80B15FF4 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 02558 80B15FF8 AFA5002C */  sw      $a1, 0x002C($sp)           
/* 0255C 80B15FFC 0C042F6F */  jal     func_8010BDBC              
/* 02560 80B16000 24A420D8 */  addiu   $a0, $a1, 0x20D8           ## $a0 = 000020D8
/* 02564 80B16004 24010004 */  addiu   $at, $zero, 0x0004         ## $at = 00000004
/* 02568 80B16008 54410064 */  bnel    $v0, $at, .L80B1619C       
/* 0256C 80B1600C 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
/* 02570 80B16010 0C041AF2 */  jal     func_80106BC8              
/* 02574 80B16014 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 02578 80B16018 1040005F */  beq     $v0, $zero, .L80B16198     
/* 0257C 80B1601C 8FA6002C */  lw      $a2, 0x002C($sp)           
/* 02580 80B16020 3C020001 */  lui     $v0, 0x0001                ## $v0 = 00010000
/* 02584 80B16024 00461021 */  addu    $v0, $v0, $a2              
/* 02588 80B16028 904204BD */  lbu     $v0, 0x04BD($v0)           ## 000104BD
/* 0258C 80B1602C 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 02590 80B16030 10400008 */  beq     $v0, $zero, .L80B16054     
/* 02594 80B16034 00000000 */  nop
/* 02598 80B16038 1041003B */  beq     $v0, $at, .L80B16128       
/* 0259C 80B1603C 3C188016 */  lui     $t8, %hi(gSaveContext+0x34)
/* 025A0 80B16040 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 025A4 80B16044 1041004E */  beq     $v0, $at, .L80B16180       
/* 025A8 80B16048 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 025AC 80B1604C 10000053 */  beq     $zero, $zero, .L80B1619C   
/* 025B0 80B16050 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
.L80B16054:
/* 025B4 80B16054 0C2C5376 */  jal     func_80B14DD8              
/* 025B8 80B16058 00000000 */  nop
/* 025BC 80B1605C 10400009 */  beq     $v0, $zero, .L80B16084     
/* 025C0 80B16060 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 025C4 80B16064 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 025C8 80B16068 10410010 */  beq     $v0, $at, .L80B160AC       
/* 025CC 80B1606C 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 025D0 80B16070 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 025D4 80B16074 10410017 */  beq     $v0, $at, .L80B160D4       
/* 025D8 80B16078 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 025DC 80B1607C 10000047 */  beq     $zero, $zero, .L80B1619C   
/* 025E0 80B16080 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
.L80B16084:
/* 025E4 80B16084 0C042DC8 */  jal     func_8010B720              
/* 025E8 80B16088 24050085 */  addiu   $a1, $zero, 0x0085         ## $a1 = 00000085
/* 025EC 80B1608C 3C0580B1 */  lui     $a1, %hi(func_80B15034)    ## $a1 = 80B10000
/* 025F0 80B16090 3C0680B1 */  lui     $a2, %hi(func_80B16938)    ## $a2 = 80B10000
/* 025F4 80B16094 24C66938 */  addiu   $a2, $a2, %lo(func_80B16938) ## $a2 = 80B16938
/* 025F8 80B16098 24A55034 */  addiu   $a1, $a1, %lo(func_80B15034) ## $a1 = 80B15034
/* 025FC 80B1609C 0C2C4EA8 */  jal     func_80B13AA0              
/* 02600 80B160A0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02604 80B160A4 1000003D */  beq     $zero, $zero, .L80B1619C   
/* 02608 80B160A8 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
.L80B160AC:
/* 0260C 80B160AC 0C042DC8 */  jal     func_8010B720              
/* 02610 80B160B0 2405208A */  addiu   $a1, $zero, 0x208A         ## $a1 = 0000208A
/* 02614 80B160B4 3C0580B1 */  lui     $a1, %hi(func_80B15E28)    ## $a1 = 80B10000
/* 02618 80B160B8 3C0680B1 */  lui     $a2, %hi(func_80B16938)    ## $a2 = 80B10000
/* 0261C 80B160BC 24C66938 */  addiu   $a2, $a2, %lo(func_80B16938) ## $a2 = 80B16938
/* 02620 80B160C0 24A55E28 */  addiu   $a1, $a1, %lo(func_80B15E28) ## $a1 = 80B15E28
/* 02624 80B160C4 0C2C4EA8 */  jal     func_80B13AA0              
/* 02628 80B160C8 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0262C 80B160CC 10000033 */  beq     $zero, $zero, .L80B1619C   
/* 02630 80B160D0 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
.L80B160D4:
/* 02634 80B160D4 960E02E0 */  lhu     $t6, 0x02E0($s0)           ## 000002E0
/* 02638 80B160D8 3C0580B1 */  lui     $a1, %hi(func_80B15E80)    ## $a1 = 80B10000
/* 0263C 80B160DC 3C0680B1 */  lui     $a2, %hi(func_80B16938)    ## $a2 = 80B10000
/* 02640 80B160E0 35CF0002 */  ori     $t7, $t6, 0x0002           ## $t7 = 00000002
/* 02644 80B160E4 A60F02E0 */  sh      $t7, 0x02E0($s0)           ## 000002E0
/* 02648 80B160E8 24C66938 */  addiu   $a2, $a2, %lo(func_80B16938) ## $a2 = 80B16938
/* 0264C 80B160EC 0C2C4EA8 */  jal     func_80B13AA0              
/* 02650 80B160F0 24A55E80 */  addiu   $a1, $a1, %lo(func_80B15E80) ## $a1 = 80B15E80
/* 02654 80B160F4 0C021CC3 */  jal     Rupees_ChangeBy              
/* 02658 80B160F8 2404FFE2 */  addiu   $a0, $zero, 0xFFE2         ## $a0 = FFFFFFE2
/* 0265C 80B160FC 3C014248 */  lui     $at, 0x4248                ## $at = 42480000
/* 02660 80B16100 44812000 */  mtc1    $at, $f4                   ## $f4 = 50.00
/* 02664 80B16104 3C07461C */  lui     $a3, 0x461C                ## $a3 = 461C0000
/* 02668 80B16108 34E74000 */  ori     $a3, $a3, 0x4000           ## $a3 = 461C4000
/* 0266C 80B1610C 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02670 80B16110 8FA5002C */  lw      $a1, 0x002C($sp)           
/* 02674 80B16114 24060050 */  addiu   $a2, $zero, 0x0050         ## $a2 = 00000050
/* 02678 80B16118 0C00BD0D */  jal     func_8002F434              
/* 0267C 80B1611C E7A40010 */  swc1    $f4, 0x0010($sp)           
/* 02680 80B16120 1000001E */  beq     $zero, $zero, .L80B1619C   
/* 02684 80B16124 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
.L80B16128:
/* 02688 80B16128 8718E694 */  lh      $t8, %lo(gSaveContext+0x34)($t8)
/* 0268C 80B1612C 00C02025 */  or      $a0, $a2, $zero            ## $a0 = 00000000
/* 02690 80B16130 2B01000A */  slti    $at, $t8, 0x000A           
/* 02694 80B16134 1020000B */  beq     $at, $zero, .L80B16164     
/* 02698 80B16138 00000000 */  nop
/* 0269C 80B1613C 0C042DC8 */  jal     func_8010B720              
/* 026A0 80B16140 24050085 */  addiu   $a1, $zero, 0x0085         ## $a1 = 00000085
/* 026A4 80B16144 3C0580B1 */  lui     $a1, %hi(func_80B15034)    ## $a1 = 80B10000
/* 026A8 80B16148 3C0680B1 */  lui     $a2, %hi(func_80B16938)    ## $a2 = 80B10000
/* 026AC 80B1614C 24C66938 */  addiu   $a2, $a2, %lo(func_80B16938) ## $a2 = 80B16938
/* 026B0 80B16150 24A55034 */  addiu   $a1, $a1, %lo(func_80B15034) ## $a1 = 80B15034
/* 026B4 80B16154 0C2C4EA8 */  jal     func_80B13AA0              
/* 026B8 80B16158 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 026BC 80B1615C 1000000F */  beq     $zero, $zero, .L80B1619C   
/* 026C0 80B16160 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
.L80B16164:
/* 026C4 80B16164 0C021CC3 */  jal     Rupees_ChangeBy              
/* 026C8 80B16168 2404FFF6 */  addiu   $a0, $zero, 0xFFF6         ## $a0 = FFFFFFF6
/* 026CC 80B1616C 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 026D0 80B16170 0C2C5764 */  jal     func_80B15D90              
/* 026D4 80B16174 8FA5002C */  lw      $a1, 0x002C($sp)           
/* 026D8 80B16178 10000008 */  beq     $zero, $zero, .L80B1619C   
/* 026DC 80B1617C 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
.L80B16180:
/* 026E0 80B16180 3C0580B1 */  lui     $a1, %hi(func_80B16504)    ## $a1 = 80B10000
/* 026E4 80B16184 0C2C53C8 */  jal     func_80B14F20              
/* 026E8 80B16188 24A56504 */  addiu   $a1, $a1, %lo(func_80B16504) ## $a1 = 80B16504
/* 026EC 80B1618C 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 026F0 80B16190 0C2C4EAB */  jal     func_80B13AAC              
/* 026F4 80B16194 8FA5002C */  lw      $a1, 0x002C($sp)           
.L80B16198:
/* 026F8 80B16198 960202E0 */  lhu     $v0, 0x02E0($s0)           ## 000002E0
.L80B1619C:
/* 026FC 80B1619C 30590010 */  andi    $t9, $v0, 0x0010           ## $t9 = 00000000
/* 02700 80B161A0 13200002 */  beq     $t9, $zero, .L80B161AC     
/* 02704 80B161A4 34480001 */  ori     $t0, $v0, 0x0001           ## $t0 = 00000001
/* 02708 80B161A8 A60802E0 */  sh      $t0, 0x02E0($s0)           ## 000002E0
.L80B161AC:
/* 0270C 80B161AC 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 02710 80B161B0 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 02714 80B161B4 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 02718 80B161B8 03E00008 */  jr      $ra                        
/* 0271C 80B161BC 00000000 */  nop
