glabel func_808A4DC0
/* 00000 808A4DC0 C4A40004 */  lwc1    $f4, 0x0004($a1)           ## 00000004
/* 00004 808A4DC4 C4C60008 */  lwc1    $f6, 0x0008($a2)           ## 00000008
/* 00008 808A4DC8 C4AA0008 */  lwc1    $f10, 0x0008($a1)          ## 00000008
/* 0000C 808A4DCC C4D00004 */  lwc1    $f16, 0x0004($a2)          ## 00000004
/* 00010 808A4DD0 46062202 */  mul.s   $f8, $f4, $f6              
/* 00014 808A4DD4 00000000 */  nop
/* 00018 808A4DD8 46105482 */  mul.s   $f18, $f10, $f16           
/* 0001C 808A4DDC 46124101 */  sub.s   $f4, $f8, $f18             
/* 00020 808A4DE0 E4840000 */  swc1    $f4, 0x0000($a0)           ## 00000000
/* 00024 808A4DE4 C4CA0000 */  lwc1    $f10, 0x0000($a2)          ## 00000000
/* 00028 808A4DE8 C4A60008 */  lwc1    $f6, 0x0008($a1)           ## 00000008
/* 0002C 808A4DEC C4A80000 */  lwc1    $f8, 0x0000($a1)           ## 00000000
/* 00030 808A4DF0 C4D20008 */  lwc1    $f18, 0x0008($a2)          ## 00000008
/* 00034 808A4DF4 460A3402 */  mul.s   $f16, $f6, $f10            
/* 00038 808A4DF8 00000000 */  nop
/* 0003C 808A4DFC 46124102 */  mul.s   $f4, $f8, $f18             
/* 00040 808A4E00 46048181 */  sub.s   $f6, $f16, $f4             
/* 00044 808A4E04 E4860004 */  swc1    $f6, 0x0004($a0)           ## 00000004
/* 00048 808A4E08 C4C80004 */  lwc1    $f8, 0x0004($a2)           ## 00000004
/* 0004C 808A4E0C C4AA0000 */  lwc1    $f10, 0x0000($a1)          ## 00000000
/* 00050 808A4E10 C4C40000 */  lwc1    $f4, 0x0000($a2)           ## 00000000
/* 00054 808A4E14 C4B00004 */  lwc1    $f16, 0x0004($a1)          ## 00000004
/* 00058 808A4E18 46085482 */  mul.s   $f18, $f10, $f8            
/* 0005C 808A4E1C 00000000 */  nop
/* 00060 808A4E20 46048182 */  mul.s   $f6, $f16, $f4             
/* 00064 808A4E24 46069281 */  sub.s   $f10, $f18, $f6            
/* 00068 808A4E28 03E00008 */  jr      $ra                        
/* 0006C 808A4E2C E48A0008 */  swc1    $f10, 0x0008($a0)          ## 00000008