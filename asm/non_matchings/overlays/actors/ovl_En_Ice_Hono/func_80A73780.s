glabel func_80A73780
/* 00790 80A73780 3C0E80A7 */  lui     $t6, %hi(func_80A737A0)    ## $t6 = 80A70000
/* 00794 80A73784 25CE37A0 */  addiu   $t6, $t6, %lo(func_80A737A0) ## $t6 = 80A737A0
/* 00798 80A73788 240F003C */  addiu   $t7, $zero, 0x003C         ## $t7 = 0000003C
/* 0079C 80A7378C 241800FF */  addiu   $t8, $zero, 0x00FF         ## $t8 = 000000FF
/* 007A0 80A73790 AC8E014C */  sw      $t6, 0x014C($a0)           ## 0000014C
/* 007A4 80A73794 A48F0152 */  sh      $t7, 0x0152($a0)           ## 00000152
/* 007A8 80A73798 03E00008 */  jr      $ra                        
/* 007AC 80A7379C A4980150 */  sh      $t8, 0x0150($a0)           ## 00000150