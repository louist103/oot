glabel func_80A13764
/* 006F4 80A13764 3C014040 */  lui     $at, 0x4040                ## $at = 40400000
/* 006F8 80A13768 44810000 */  mtc1    $at, $f0                   ## $f0 = 3.00
/* 006FC 80A1376C 848F008A */  lh      $t7, 0x008A($a0)           ## 0000008A
/* 00700 80A13770 3C1980A1 */  lui     $t9, %hi(func_80A143B4)    ## $t9 = 80A10000
/* 00704 80A13774 240E1554 */  addiu   $t6, $zero, 0x1554         ## $t6 = 00001554
/* 00708 80A13778 24180032 */  addiu   $t8, $zero, 0x0032         ## $t8 = 00000032
/* 0070C 80A1377C 273943B4 */  addiu   $t9, $t9, %lo(func_80A143B4) ## $t9 = 80A143B4
/* 00710 80A13780 A48E00B4 */  sh      $t6, 0x00B4($a0)           ## 000000B4
/* 00714 80A13784 A49801BA */  sh      $t8, 0x01BA($a0)           ## 000001BA
/* 00718 80A13788 AC9901B4 */  sw      $t9, 0x01B4($a0)           ## 000001B4
/* 0071C 80A1378C E480018C */  swc1    $f0, 0x018C($a0)           ## 0000018C
/* 00720 80A13790 A48F00B6 */  sh      $t7, 0x00B6($a0)           ## 000000B6
/* 00724 80A13794 03E00008 */  jr      $ra                        
/* 00728 80A13798 E4800068 */  swc1    $f0, 0x0068($a0)           ## 00000068