glabel func_809E5A38
/* 00268 809E5A38 8C830254 */  lw      $v1, 0x0254($a0)           ## 00000254
/* 0026C 809E5A3C 24020003 */  addiu   $v0, $zero, 0x0003         ## $v0 = 00000003
/* 00270 809E5A40 906E0056 */  lbu     $t6, 0x0056($v1)           ## 00000056
/* 00274 809E5A44 24630040 */  addiu   $v1, $v1, 0x0040           ## $v1 = 00000040
/* 00278 809E5A48 31CFFFFE */  andi    $t7, $t6, 0xFFFE           ## $t7 = 00000000
/* 0027C 809E5A4C A06F0016 */  sb      $t7, 0x0016($v1)           ## 00000056
/* 00280 809E5A50 8C830254 */  lw      $v1, 0x0254($a0)           ## 00000254
/* 00284 809E5A54 90780096 */  lbu     $t8, 0x0096($v1)           ## 000000D6
/* 00288 809E5A58 24630040 */  addiu   $v1, $v1, 0x0040           ## $v1 = 00000080
/* 0028C 809E5A5C 3319FFFE */  andi    $t9, $t8, 0xFFFE           ## $t9 = 00000000
/* 00290 809E5A60 A0790056 */  sb      $t9, 0x0056($v1)           ## 000000D6
/* 00294 809E5A64 8C880254 */  lw      $t0, 0x0254($a0)           ## 00000254
/* 00298 809E5A68 00021980 */  sll     $v1, $v0,  6               
/* 0029C 809E5A6C 01032821 */  addu    $a1, $t0, $v1              
/* 002A0 809E5A70 90A90016 */  lbu     $t1, 0x0016($a1)           ## 00000016
/* 002A4 809E5A74 312AFFFE */  andi    $t2, $t1, 0xFFFE           ## $t2 = 00000000
/* 002A8 809E5A78 A0AA0016 */  sb      $t2, 0x0016($a1)           ## 00000016
/* 002AC 809E5A7C 8C8B0254 */  lw      $t3, 0x0254($a0)           ## 00000254
/* 002B0 809E5A80 01632821 */  addu    $a1, $t3, $v1              
/* 002B4 809E5A84 90AC0056 */  lbu     $t4, 0x0056($a1)           ## 00000056
/* 002B8 809E5A88 318DFFFE */  andi    $t5, $t4, 0xFFFE           ## $t5 = 00000000
/* 002BC 809E5A8C A0AD0056 */  sb      $t5, 0x0056($a1)           ## 00000056
/* 002C0 809E5A90 8C8E0254 */  lw      $t6, 0x0254($a0)           ## 00000254
/* 002C4 809E5A94 01C32821 */  addu    $a1, $t6, $v1              
/* 002C8 809E5A98 90AF0096 */  lbu     $t7, 0x0096($a1)           ## 00000096
/* 002CC 809E5A9C 31F8FFFE */  andi    $t8, $t7, 0xFFFE           ## $t8 = 00000000
/* 002D0 809E5AA0 A0B80096 */  sb      $t8, 0x0096($a1)           ## 00000096
/* 002D4 809E5AA4 8C990254 */  lw      $t9, 0x0254($a0)           ## 00000254
/* 002D8 809E5AA8 03232821 */  addu    $a1, $t9, $v1              
/* 002DC 809E5AAC 90A800D6 */  lbu     $t0, 0x00D6($a1)           ## 000000D6
/* 002E0 809E5AB0 3109FFFE */  andi    $t1, $t0, 0xFFFE           ## $t1 = 00000000
/* 002E4 809E5AB4 03E00008 */  jr      $ra                        
/* 002E8 809E5AB8 A0A900D6 */  sb      $t1, 0x00D6($a1)           ## 000000D6