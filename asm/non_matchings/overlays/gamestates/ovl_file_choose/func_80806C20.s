glabel func_80806C20
/* 02EE0 80806C20 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 02EE4 80806C24 34218000 */  ori     $at, $at, 0x8000           ## $at = 00018000
/* 02EE8 80806C28 00814021 */  addu    $t0, $a0, $at              
/* 02EEC 80806C2C 00204821 */  addu    $t1, $at, $zero            
/* 02EF0 80806C30 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 02EF4 80806C34 00027040 */  sll     $t6, $v0,  1               
.L80806C38:
/* 02EF8 80806C38 008E7821 */  addu    $t7, $a0, $t6              
/* 02EFC 80806C3C 01E93021 */  addu    $a2, $t7, $t1              
/* 02F00 80806C40 84C74A52 */  lh      $a3, 0x4A52($a2)           ## 00004A52
/* 02F04 80806C44 24420001 */  addiu   $v0, $v0, 0x0001           ## $v0 = 00000001
/* 02F08 80806C48 00021400 */  sll     $v0, $v0, 16               
/* 02F0C 80806C4C 04E00003 */  bltz    $a3, .L80806C5C            
/* 02F10 80806C50 00021403 */  sra     $v0, $v0, 16               
/* 02F14 80806C54 10000002 */  beq     $zero, $zero, .L80806C60   
/* 02F18 80806C58 00E01825 */  or      $v1, $a3, $zero            ## $v1 = 00000000
.L80806C5C:
/* 02F1C 80806C5C 00071823 */  subu    $v1, $zero, $a3            
.L80806C60:
/* 02F20 80806C60 85184A50 */  lh      $t8, 0x4A50($t0)           ## 00004A50
/* 02F24 80806C64 0078001A */  div     $zero, $v1, $t8            
/* 02F28 80806C68 17000002 */  bne     $t8, $zero, .L80806C74     
/* 02F2C 80806C6C 00000000 */  nop
/* 02F30 80806C70 0007000D */  break 7
.L80806C74:
/* 02F34 80806C74 2401FFFF */  addiu   $at, $zero, 0xFFFF         ## $at = FFFFFFFF
/* 02F38 80806C78 17010004 */  bne     $t8, $at, .L80806C8C       
/* 02F3C 80806C7C 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 02F40 80806C80 14610002 */  bne     $v1, $at, .L80806C8C       
/* 02F44 80806C84 00000000 */  nop
/* 02F48 80806C88 0006000D */  break 6
.L80806C8C:
/* 02F4C 80806C8C 00002812 */  mflo    $a1                        
/* 02F50 80806C90 00052C00 */  sll     $a1, $a1, 16               
/* 02F54 80806C94 28410005 */  slti    $at, $v0, 0x0005           
/* 02F58 80806C98 04E00004 */  bltz    $a3, .L80806CAC            
/* 02F5C 80806C9C 00052C03 */  sra     $a1, $a1, 16               
/* 02F60 80806CA0 00E5C823 */  subu    $t9, $a3, $a1              
/* 02F64 80806CA4 10000003 */  beq     $zero, $zero, .L80806CB4   
/* 02F68 80806CA8 A4D94A52 */  sh      $t9, 0x4A52($a2)           ## 00004A52
.L80806CAC:
/* 02F6C 80806CAC 00E55021 */  addu    $t2, $a3, $a1              
/* 02F70 80806CB0 A4CA4A52 */  sh      $t2, 0x4A52($a2)           ## 00004A52
.L80806CB4:
/* 02F74 80806CB4 5420FFE0 */  bnel    $at, $zero, .L80806C38     
/* 02F78 80806CB8 00027040 */  sll     $t6, $v0,  1               
/* 02F7C 80806CBC 850B4A92 */  lh      $t3, 0x4A92($t0)           ## 00004A92
/* 02F80 80806CC0 850D4A94 */  lh      $t5, 0x4A94($t0)           ## 00004A94
/* 02F84 80806CC4 850F4A9A */  lh      $t7, 0x4A9A($t0)           ## 00004A9A
/* 02F88 80806CC8 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 02F8C 80806CCC 256C0019 */  addiu   $t4, $t3, 0x0019           ## $t4 = 00000019
/* 02F90 80806CD0 25AE0019 */  addiu   $t6, $t5, 0x0019           ## $t6 = 00000019
/* 02F94 80806CD4 A50C4A92 */  sh      $t4, 0x4A92($t0)           ## 00004A92
/* 02F98 80806CD8 A50E4A94 */  sh      $t6, 0x4A94($t0)           ## 00004A94
/* 02F9C 80806CDC 00240821 */  addu    $at, $at, $a0              
/* 02FA0 80806CE0 25F80019 */  addiu   $t8, $t7, 0x0019           ## $t8 = 00000019
/* 02FA4 80806CE4 A438CA9A */  sh      $t8, -0x3566($at)          ## 0001CA9A
/* 02FA8 80806CE8 85194A98 */  lh      $t9, 0x4A98($t0)           ## 00004A98
/* 02FAC 80806CEC 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 02FB0 80806CF0 00240821 */  addu    $at, $at, $a0              
/* 02FB4 80806CF4 272AFFCE */  addiu   $t2, $t9, 0xFFCE           ## $t2 = FFFFFFCE
/* 02FB8 80806CF8 A50A4A98 */  sh      $t2, 0x4A98($t0)           ## 00004A98
/* 02FBC 80806CFC 850B4A98 */  lh      $t3, 0x4A98($t0)           ## 00004A98
/* 02FC0 80806D00 3C028016 */  lui     $v0, %hi(gGameInfo)
/* 02FC4 80806D04 5D600003 */  bgtzl   $t3, .L80806D14            
/* 02FC8 80806D08 850C4A6E */  lh      $t4, 0x4A6E($t0)           ## 00004A6E
/* 02FCC 80806D0C A5004A98 */  sh      $zero, 0x4A98($t0)         ## 00004A98
/* 02FD0 80806D10 850C4A6E */  lh      $t4, 0x4A6E($t0)           ## 00004A6E
.L80806D14:
/* 02FD4 80806D14 850E4A70 */  lh      $t6, 0x4A70($t0)           ## 00004A70
/* 02FD8 80806D18 85184A50 */  lh      $t8, 0x4A50($t0)           ## 00004A50
/* 02FDC 80806D1C 258DFFE1 */  addiu   $t5, $t4, 0xFFE1           ## $t5 = FFFFFFFA
/* 02FE0 80806D20 25CF001F */  addiu   $t7, $t6, 0x001F           ## $t7 = 00000038
/* 02FE4 80806D24 A50D4A6E */  sh      $t5, 0x4A6E($t0)           ## 00004A6E
/* 02FE8 80806D28 A50F4A70 */  sh      $t7, 0x4A70($t0)           ## 00004A70
/* 02FEC 80806D2C 2719FFFF */  addiu   $t9, $t8, 0xFFFF           ## $t9 = 00000018
/* 02FF0 80806D30 A439CA50 */  sh      $t9, -0x35B0($at)          ## 0001CA50
/* 02FF4 80806D34 850A4A50 */  lh      $t2, 0x4A50($t0)           ## 00004A50
/* 02FF8 80806D38 240B0046 */  addiu   $t3, $zero, 0x0046         ## $t3 = 00000046
/* 02FFC 80806D3C 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 03000 80806D40 15400015 */  bne     $t2, $zero, .L80806D98     
/* 03004 80806D44 240C0001 */  addiu   $t4, $zero, 0x0001         ## $t4 = 00000001
/* 03008 80806D48 A50B4AA8 */  sh      $t3, 0x4AA8($t0)           ## 00004AA8
/* 0300C 80806D4C 00240821 */  addu    $at, $at, $a0              
/* 03010 80806D50 A42CCAAA */  sh      $t4, -0x3556($at)          ## 0001CAAA
/* 03014 80806D54 8C42FA90 */  lw      $v0, %lo(gGameInfo)($v0)
/* 03018 80806D58 240E00C8 */  addiu   $t6, $zero, 0x00C8         ## $t6 = 000000C8
/* 0301C 80806D5C 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 03020 80806D60 844D0ADC */  lh      $t5, 0x0ADC($v0)           ## 80160ADC
/* 03024 80806D64 00240821 */  addu    $at, $at, $a0              
/* 03028 80806D68 241800FF */  addiu   $t8, $zero, 0x00FF         ## $t8 = 000000FF
/* 0302C 80806D6C A44D0ADA */  sh      $t5, 0x0ADA($v0)           ## 80160ADA
/* 03030 80806D70 850F4A66 */  lh      $t7, 0x4A66($t0)           ## 00004A66
/* 03034 80806D74 A50E4A92 */  sh      $t6, 0x4A92($t0)           ## 00004A92
/* 03038 80806D78 A5004A98 */  sh      $zero, 0x4A98($t0)         ## 00004A98
/* 0303C 80806D7C A42FCA64 */  sh      $t7, -0x359C($at)          ## 0001CA64
/* 03040 80806D80 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 03044 80806D84 A5184A6E */  sh      $t8, 0x4A6E($t0)           ## 00004A6E
/* 03048 80806D88 A5004A70 */  sh      $zero, 0x4A70($t0)         ## 00004A70
/* 0304C 80806D8C 00240821 */  addu    $at, $at, $a0              
/* 03050 80806D90 24190002 */  addiu   $t9, $zero, 0x0002         ## $t9 = 00000002
/* 03054 80806D94 A439CA3E */  sh      $t9, -0x35C2($at)          ## 0001CA3E
.L80806D98:
/* 03058 80806D98 85024A92 */  lh      $v0, 0x4A92($t0)           ## 00004A92
/* 0305C 80806D9C 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 03060 80806DA0 00240821 */  addu    $at, $at, $a0              
/* 03064 80806DA4 A5024A94 */  sh      $v0, 0x4A94($t0)           ## 00004A94
/* 03068 80806DA8 03E00008 */  jr      $ra                        
/* 0306C 80806DAC A422CA9A */  sh      $v0, -0x3566($at)          ## 0001CA9A