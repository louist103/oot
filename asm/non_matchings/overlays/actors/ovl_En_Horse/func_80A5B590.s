glabel func_80A5B590
/* 002A0 80A5B590 00057080 */  sll     $t6, $a1,  2               
/* 002A4 80A5B594 01C57021 */  addu    $t6, $t6, $a1              
/* 002A8 80A5B598 000E7040 */  sll     $t6, $t6,  1               
/* 002AC 80A5B59C 008E1021 */  addu    $v0, $a0, $t6              
/* 002B0 80A5B5A0 844F0000 */  lh      $t7, 0x0000($v0)           ## 00000000
/* 002B4 80A5B5A4 448F2000 */  mtc1    $t7, $f4                   ## $f4 = 0.00
/* 002B8 80A5B5A8 00000000 */  nop
/* 002BC 80A5B5AC 468021A0 */  cvt.s.w $f6, $f4                   
/* 002C0 80A5B5B0 E4C60000 */  swc1    $f6, 0x0000($a2)           ## 00000000
/* 002C4 80A5B5B4 84580002 */  lh      $t8, 0x0002($v0)           ## 00000002
/* 002C8 80A5B5B8 44984000 */  mtc1    $t8, $f8                   ## $f8 = 0.00
/* 002CC 80A5B5BC 00000000 */  nop
/* 002D0 80A5B5C0 468042A0 */  cvt.s.w $f10, $f8                  
/* 002D4 80A5B5C4 E4CA0004 */  swc1    $f10, 0x0004($a2)          ## 00000004
/* 002D8 80A5B5C8 84590004 */  lh      $t9, 0x0004($v0)           ## 00000004
/* 002DC 80A5B5CC 44998000 */  mtc1    $t9, $f16                  ## $f16 = 0.00
/* 002E0 80A5B5D0 00000000 */  nop
/* 002E4 80A5B5D4 468084A0 */  cvt.s.w $f18, $f16                 
/* 002E8 80A5B5D8 03E00008 */  jr      $ra                        
/* 002EC 80A5B5DC E4D20008 */  swc1    $f18, 0x0008($a2)          ## 00000008