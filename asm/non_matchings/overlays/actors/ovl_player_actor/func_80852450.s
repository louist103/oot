glabel func_80852450
/* 20240 80852450 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 20244 80852454 AFA60020 */  sw      $a2, 0x0020($sp)           
/* 20248 80852458 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 2024C 8085245C 3C060400 */  lui     $a2, 0x0400                ## $a2 = 04000000
/* 20250 80852460 3C078085 */  lui     $a3, %hi(D_808551F0)       ## $a3 = 80850000
/* 20254 80852464 24E751F0 */  addiu   $a3, $a3, %lo(D_808551F0)  ## $a3 = 808551F0
/* 20258 80852468 0C214905 */  jal     func_80852414              
/* 2025C 8085246C 24C62378 */  addiu   $a2, $a2, 0x2378           ## $a2 = 04002378
/* 20260 80852470 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 20264 80852474 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 20268 80852478 03E00008 */  jr      $ra                        
/* 2026C 8085247C 00000000 */  nop