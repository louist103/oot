glabel func_80B92578
/* 00158 80B92578 8C8E0018 */  lw      $t6, 0x0018($a0)           ## 00000018
/* 0015C 80B9257C 448E2000 */  mtc1    $t6, $f4                   ## $f4 = 0.00
/* 00160 80B92580 00000000 */  nop
/* 00164 80B92584 468021A0 */  cvt.s.w $f6, $f4                   
/* 00168 80B92588 E4A60000 */  swc1    $f6, 0x0000($a1)           ## 00000000
/* 0016C 80B9258C 8C8F001C */  lw      $t7, 0x001C($a0)           ## 0000001C
/* 00170 80B92590 448F4000 */  mtc1    $t7, $f8                   ## $f8 = 0.00
/* 00174 80B92594 00000000 */  nop
/* 00178 80B92598 468042A0 */  cvt.s.w $f10, $f8                  
/* 0017C 80B9259C E4AA0004 */  swc1    $f10, 0x0004($a1)          ## 00000004
/* 00180 80B925A0 8C980020 */  lw      $t8, 0x0020($a0)           ## 00000020
/* 00184 80B925A4 44988000 */  mtc1    $t8, $f16                  ## $f16 = 0.00
/* 00188 80B925A8 00000000 */  nop
/* 0018C 80B925AC 468084A0 */  cvt.s.w $f18, $f16                 
/* 00190 80B925B0 03E00008 */  jr      $ra                        
/* 00194 80B925B4 E4B20008 */  swc1    $f18, 0x0008($a1)          ## 00000008