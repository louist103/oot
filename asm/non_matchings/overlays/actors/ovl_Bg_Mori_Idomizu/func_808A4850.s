glabel func_808A4850
/* 00210 808A4850 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 00214 808A4854 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 00218 808A4858 3C05808A */  lui     $a1, %hi(func_808A4874)    ## $a1 = 808A0000
/* 0021C 808A485C 0C229190 */  jal     func_808A4640              
/* 00220 808A4860 24A54874 */  addiu   $a1, $a1, %lo(func_808A4874) ## $a1 = 808A4874
/* 00224 808A4864 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 00228 808A4868 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 0022C 808A486C 03E00008 */  jr      $ra                        
/* 00230 808A4870 00000000 */  nop