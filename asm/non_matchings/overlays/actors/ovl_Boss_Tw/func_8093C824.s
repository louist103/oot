glabel BossTw_TwinrovaSetupWaitForMerge
/* 03B54 8093C824 AFA50004 */  sw      $a1, 0x0004($sp)           
/* 03B58 8093C828 8C8F0004 */  lw      $t7, 0x0004($a0)           ## 00000004
/* 03B5C 8093C82C 3C01C4FA */  lui     $at, 0xC4FA                ## $at = C4FA0000
/* 03B60 8093C830 44812000 */  mtc1    $at, $f4                   ## $f4 = -2000.00
/* 03B64 8093C834 3C0E8094 */  lui     $t6, %hi(BossTw_TwinrovaWaitForMerge)    ## $t6 = 80940000
/* 03B68 8093C838 2401FFFE */  addiu   $at, $zero, 0xFFFE         ## $at = FFFFFFFE
/* 03B6C 8093C83C 25CEC858 */  addiu   $t6, $t6, %lo(BossTw_TwinrovaWaitForMerge) ## $t6 = 8093C858
/* 03B70 8093C840 01E1C024 */  and     $t8, $t7, $at              
/* 03B74 8093C844 AC8E014C */  sw      $t6, 0x014C($a0)           ## 0000014C
/* 03B78 8093C848 A0800564 */  sb      $zero, 0x0564($a0)         ## 00000564
/* 03B7C 8093C84C AC980004 */  sw      $t8, 0x0004($a0)           ## 00000004
/* 03B80 8093C850 03E00008 */  jr      $ra                        
/* 03B84 8093C854 E4840028 */  swc1    $f4, 0x0028($a0)           ## 00000028
