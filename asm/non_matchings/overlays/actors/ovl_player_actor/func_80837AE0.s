glabel func_80837AE0
/* 058D0 80837AE0 808E0A78 */  lb      $t6, 0x0A78($a0)           ## 00000A78
/* 058D4 80837AE4 05C00003 */  bltz    $t6, .L80837AF4            
/* 058D8 80837AE8 00000000 */  nop
/* 058DC 80837AEC A0850A78 */  sb      $a1, 0x0A78($a0)           ## 00000A78
/* 058E0 80837AF0 A080088F */  sb      $zero, 0x088F($a0)         ## 0000088F
.L80837AF4:
/* 058E4 80837AF4 03E00008 */  jr      $ra                        
/* 058E8 80837AF8 00000000 */  nop