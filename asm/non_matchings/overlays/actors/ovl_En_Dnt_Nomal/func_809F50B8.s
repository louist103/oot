glabel func_809F50B8
/* 01DF8 809F50B8 AFA50004 */  sw      $a1, 0x0004($sp)           
/* 01DFC 809F50BC 3C01428C */  lui     $at, 0x428C                ## $at = 428C0000
/* 01E00 809F50C0 44812000 */  mtc1    $at, $f4                   ## $f4 = 70.00
/* 01E04 809F50C4 C4860090 */  lwc1    $f6, 0x0090($a0)           ## 00000090
/* 01E08 809F50C8 3C0E809F */  lui     $t6, %hi(func_809F43F0)    ## $t6 = 809F0000
/* 01E0C 809F50CC 25CE43F0 */  addiu   $t6, $t6, %lo(func_809F43F0) ## $t6 = 809F43F0
/* 01E10 809F50D0 4606203C */  c.lt.s  $f4, $f6                   
/* 01E14 809F50D4 00000000 */  nop
/* 01E18 809F50D8 45000002 */  bc1f    .L809F50E4                 
/* 01E1C 809F50DC 00000000 */  nop
/* 01E20 809F50E0 AC8E0214 */  sw      $t6, 0x0214($a0)           ## 00000214
.L809F50E4:
/* 01E24 809F50E4 03E00008 */  jr      $ra                        
/* 01E28 809F50E8 00000000 */  nop
