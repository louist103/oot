glabel func_80AC74F4
/* 04854 80AC74F4 24010008 */  addiu   $at, $zero, 0x0008         ## $at = 00000008
/* 04858 80AC74F8 AFA40000 */  sw      $a0, 0x0000($sp)           
/* 0485C 80AC74FC AFA60008 */  sw      $a2, 0x0008($sp)           
/* 04860 80AC7500 14A10007 */  bne     $a1, $at, .L80AC7520       
/* 04864 80AC7504 AFA7000C */  sw      $a3, 0x000C($sp)           
/* 04868 80AC7508 8FA20010 */  lw      $v0, 0x0010($sp)           
/* 0486C 80AC750C 8FAF0014 */  lw      $t7, 0x0014($sp)           
/* 04870 80AC7510 844E0000 */  lh      $t6, 0x0000($v0)           ## 00000000
/* 04874 80AC7514 85F801EE */  lh      $t8, 0x01EE($t7)           ## 000001EE
/* 04878 80AC7518 01D8C821 */  addu    $t9, $t6, $t8              
/* 0487C 80AC751C A4590000 */  sh      $t9, 0x0000($v0)           ## 00000000
.L80AC7520:
/* 04880 80AC7520 03E00008 */  jr      $ra                        
/* 04884 80AC7524 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
