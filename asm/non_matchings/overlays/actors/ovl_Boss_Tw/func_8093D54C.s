glabel BossTw_SetupCSWait
/* 0487C 8093D54C AFA50004 */  sw      $a1, 0x0004($sp)           
/* 04880 8093D550 8C8F0004 */  lw      $t7, 0x0004($a0)           ## 00000004
/* 04884 8093D554 3C01C4FA */  lui     $at, 0xC4FA                ## $at = C4FA0000
/* 04888 8093D558 44812000 */  mtc1    $at, $f4                   ## $f4 = -2000.00
/* 0488C 8093D55C 3C0E8094 */  lui     $t6, %hi(BossTw_CSWait)    ## $t6 = 80940000
/* 04890 8093D560 2401FFFE */  addiu   $at, $zero, 0xFFFE         ## $at = FFFFFFFE
/* 04894 8093D564 25CED580 */  addiu   $t6, $t6, %lo(BossTw_CSWait) ## $t6 = 8093D580
/* 04898 8093D568 01E1C024 */  and     $t8, $t7, $at              
/* 0489C 8093D56C AC8E014C */  sw      $t6, 0x014C($a0)           ## 0000014C
/* 048A0 8093D570 A0800564 */  sb      $zero, 0x0564($a0)         ## 00000564
/* 048A4 8093D574 AC980004 */  sw      $t8, 0x0004($a0)           ## 00000004
/* 048A8 8093D578 03E00008 */  jr      $ra                        
/* 048AC 8093D57C E4840028 */  swc1    $f4, 0x0028($a0)           ## 00000028
