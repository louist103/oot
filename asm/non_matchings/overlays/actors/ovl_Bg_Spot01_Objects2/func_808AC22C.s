glabel func_808AC22C
/* 000EC 808AC22C 000670C0 */  sll     $t6, $a2,  3               
/* 000F0 808AC230 008E7821 */  addu    $t7, $a0, $t6              
/* 000F4 808AC234 8DE80004 */  lw      $t0, 0x0004($t7)           ## 00000004
/* 000F8 808AC238 3C0A8016 */  lui     $t2, %hi(gSegments)
/* 000FC 808AC23C 3C0100FF */  lui     $at, 0x00FF                ## $at = 00FF0000
/* 00100 808AC240 0008C100 */  sll     $t8, $t0,  4               
/* 00104 808AC244 0018CF02 */  srl     $t9, $t8, 28               
/* 00108 808AC248 00194880 */  sll     $t1, $t9,  2               
/* 0010C 808AC24C 01495021 */  addu    $t2, $t2, $t1              
/* 00110 808AC250 8D4A6FA8 */  lw      $t2, %lo(gSegments)($t2)
/* 00114 808AC254 3421FFFF */  ori     $at, $at, 0xFFFF           ## $at = 00FFFFFF
/* 00118 808AC258 00076880 */  sll     $t5, $a3,  2               
/* 0011C 808AC25C 01A76823 */  subu    $t5, $t5, $a3              
/* 00120 808AC260 01015824 */  and     $t3, $t0, $at              
/* 00124 808AC264 000D6840 */  sll     $t5, $t5,  1               
/* 00128 808AC268 014B6021 */  addu    $t4, $t2, $t3              
/* 0012C 808AC26C 018D1821 */  addu    $v1, $t4, $t5              
/* 00130 808AC270 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 00134 808AC274 00611821 */  addu    $v1, $v1, $at              
/* 00138 808AC278 846E0000 */  lh      $t6, 0x0000($v1)           ## 00000000
/* 0013C 808AC27C 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 00140 808AC280 448E2000 */  mtc1    $t6, $f4                   ## $f4 = 0.00
/* 00144 808AC284 00000000 */  nop
/* 00148 808AC288 468021A0 */  cvt.s.w $f6, $f4                   
/* 0014C 808AC28C E4A60000 */  swc1    $f6, 0x0000($a1)           ## 00000000
/* 00150 808AC290 846F0002 */  lh      $t7, 0x0002($v1)           ## 00000002
/* 00154 808AC294 448F4000 */  mtc1    $t7, $f8                   ## $f8 = 0.00
/* 00158 808AC298 00000000 */  nop
/* 0015C 808AC29C 468042A0 */  cvt.s.w $f10, $f8                  
/* 00160 808AC2A0 E4AA0004 */  swc1    $f10, 0x0004($a1)          ## 00000004
/* 00164 808AC2A4 84780004 */  lh      $t8, 0x0004($v1)           ## 00000004
/* 00168 808AC2A8 44988000 */  mtc1    $t8, $f16                  ## $f16 = 0.00
/* 0016C 808AC2AC 00000000 */  nop
/* 00170 808AC2B0 468084A0 */  cvt.s.w $f18, $f16                 
/* 00174 808AC2B4 03E00008 */  jr      $ra                        
/* 00178 808AC2B8 E4B20008 */  swc1    $f18, 0x0008($a1)          ## 00000008