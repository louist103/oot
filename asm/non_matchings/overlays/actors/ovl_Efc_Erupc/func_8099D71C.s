glabel func_8099D71C
/* 00A6C 8099D71C 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
.L8099D720:
/* 00A70 8099D720 24420001 */  addiu   $v0, $v0, 0x0001           ## $v0 = 00000001
/* 00A74 8099D724 00021400 */  sll     $v0, $v0, 16               
/* 00A78 8099D728 00021403 */  sra     $v0, $v0, 16               
/* 00A7C 8099D72C 28410064 */  slti    $at, $v0, 0x0064           
/* 00A80 8099D730 2484003C */  addiu   $a0, $a0, 0x003C           ## $a0 = 0000003C
/* 00A84 8099D734 1420FFFA */  bne     $at, $zero, .L8099D720     
/* 00A88 8099D738 A080FFE8 */  sb      $zero, -0x0018($a0)        ## 00000024
/* 00A8C 8099D73C 03E00008 */  jr      $ra                        
/* 00A90 8099D740 00000000 */  nop
/* 00A94 8099D744 00000000 */  nop
/* 00A98 8099D748 00000000 */  nop
/* 00A9C 8099D74C 00000000 */  nop