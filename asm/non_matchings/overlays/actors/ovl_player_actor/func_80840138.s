glabel func_80840138
/* 0DF28 80840138 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 0DF2C 8084013C 44856000 */  mtc1    $a1, $f12                  ## $f12 = 0.00
/* 0DF30 80840140 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 0DF34 80840144 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 0DF38 80840148 AFA60020 */  sw      $a2, 0x0020($sp)           
/* 0DF3C 8084014C 848E00B6 */  lh      $t6, 0x00B6($a0)           ## 000000B6
/* 0DF40 80840150 460C003C */  c.lt.s  $f0, $f12                  
/* 0DF44 80840154 00063400 */  sll     $a2, $a2, 16               
/* 0DF48 80840158 00063403 */  sra     $a2, $a2, 16               
/* 0DF4C 8084015C 00CE1023 */  subu    $v0, $a2, $t6              
/* 0DF50 80840160 00021400 */  sll     $v0, $v0, 16               
/* 0DF54 80840164 00803825 */  or      $a3, $a0, $zero            ## $a3 = 00000000
/* 0DF58 80840168 45000008 */  bc1f    .L8084018C                 
/* 0DF5C 8084016C 00021403 */  sra     $v0, $v0, 16               
/* 0DF60 80840170 04410003 */  bgez    $v0, .L80840180            
/* 0DF64 80840174 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 0DF68 80840178 10000004 */  beq     $zero, $zero, .L8084018C   
/* 0DF6C 8084017C E4800874 */  swc1    $f0, 0x0874($a0)           ## 00000874
.L80840180:
/* 0DF70 80840180 44812000 */  mtc1    $at, $f4                   ## $f4 = 1.00
/* 0DF74 80840184 00000000 */  nop
/* 0DF78 80840188 E4E40874 */  swc1    $f4, 0x0874($a3)           ## 00000874
.L8084018C:
/* 0DF7C 8084018C 3C063E99 */  lui     $a2, 0x3E99                ## $a2 = 3E990000
/* 0DF80 80840190 34C6999A */  ori     $a2, $a2, 0x999A           ## $a2 = 3E99999A
/* 0DF84 80840194 24E40870 */  addiu   $a0, $a3, 0x0870           ## $a0 = 00000870
/* 0DF88 80840198 0C01DE80 */  jal     Math_ApproxF
              
/* 0DF8C 8084019C 8CE50874 */  lw      $a1, 0x0874($a3)           ## 00000874
/* 0DF90 808401A0 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 0DF94 808401A4 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 0DF98 808401A8 03E00008 */  jr      $ra                        
/* 0DF9C 808401AC 00000000 */  nop