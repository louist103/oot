glabel func_809E1E90
/* 005E0 809E1E90 000670C0 */  sll     $t6, $a2,  3               
/* 005E4 809E1E94 008E7821 */  addu    $t7, $a0, $t6              
/* 005E8 809E1E98 8DE80004 */  lw      $t0, 0x0004($t7)           ## 00000004
/* 005EC 809E1E9C 3C0A8016 */  lui     $t2, %hi(gSegments)
/* 005F0 809E1EA0 3C0100FF */  lui     $at, 0x00FF                ## $at = 00FF0000
/* 005F4 809E1EA4 0008C100 */  sll     $t8, $t0,  4               
/* 005F8 809E1EA8 0018CF02 */  srl     $t9, $t8, 28               
/* 005FC 809E1EAC 00194880 */  sll     $t1, $t9,  2               
/* 00600 809E1EB0 01495021 */  addu    $t2, $t2, $t1              
/* 00604 809E1EB4 8D4A6FA8 */  lw      $t2, %lo(gSegments)($t2)
/* 00608 809E1EB8 3421FFFF */  ori     $at, $at, 0xFFFF           ## $at = 00FFFFFF
/* 0060C 809E1EBC 00076880 */  sll     $t5, $a3,  2               
/* 00610 809E1EC0 01A76823 */  subu    $t5, $t5, $a3              
/* 00614 809E1EC4 01015824 */  and     $t3, $t0, $at              
/* 00618 809E1EC8 000D6840 */  sll     $t5, $t5,  1               
/* 0061C 809E1ECC 014B6021 */  addu    $t4, $t2, $t3              
/* 00620 809E1ED0 018D1821 */  addu    $v1, $t4, $t5              
/* 00624 809E1ED4 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 00628 809E1ED8 00611821 */  addu    $v1, $v1, $at              
/* 0062C 809E1EDC 846E0000 */  lh      $t6, 0x0000($v1)           ## 00000000
/* 00630 809E1EE0 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 00634 809E1EE4 448E2000 */  mtc1    $t6, $f4                   ## $f4 = 0.00
/* 00638 809E1EE8 00000000 */  nop
/* 0063C 809E1EEC 468021A0 */  cvt.s.w $f6, $f4                   
/* 00640 809E1EF0 E4A60000 */  swc1    $f6, 0x0000($a1)           ## 00000000
/* 00644 809E1EF4 846F0002 */  lh      $t7, 0x0002($v1)           ## 00000002
/* 00648 809E1EF8 448F4000 */  mtc1    $t7, $f8                   ## $f8 = 0.00
/* 0064C 809E1EFC 00000000 */  nop
/* 00650 809E1F00 468042A0 */  cvt.s.w $f10, $f8                  
/* 00654 809E1F04 E4AA0004 */  swc1    $f10, 0x0004($a1)          ## 00000004
/* 00658 809E1F08 84780004 */  lh      $t8, 0x0004($v1)           ## 00000004
/* 0065C 809E1F0C 44988000 */  mtc1    $t8, $f16                  ## $f16 = 0.00
/* 00660 809E1F10 00000000 */  nop
/* 00664 809E1F14 468084A0 */  cvt.s.w $f18, $f16                 
/* 00668 809E1F18 03E00008 */  jr      $ra                        
/* 0066C 809E1F1C E4B20008 */  swc1    $f18, 0x0008($a1)          ## 00000008