glabel func_80977874
/* 004E4 80977874 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 004E8 80977878 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 004EC 8097787C AFA40018 */  sw      $a0, 0x0018($sp)           
/* 004F0 80977880 0C25DD14 */  jal     func_80977450              
/* 004F4 80977884 AFA5001C */  sw      $a1, 0x001C($sp)           
/* 004F8 80977888 0C25DDB4 */  jal     func_809776D0              
/* 004FC 8097788C 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 00500 80977890 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 00504 80977894 0C25DD84 */  jal     func_80977610              
/* 00508 80977898 8FA5001C */  lw      $a1, 0x001C($sp)           
/* 0050C 8097789C 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 00510 809778A0 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 00514 809778A4 03E00008 */  jr      $ra                        
/* 00518 809778A8 00000000 */  nop
