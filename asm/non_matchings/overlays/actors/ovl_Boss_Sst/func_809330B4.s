glabel func_809330B4
/* 06AE4 809330B4 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 06AE8 809330B8 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 06AEC 809330BC AFA40018 */  sw      $a0, 0x0018($sp)           
/* 06AF0 809330C0 AFA5001C */  sw      $a1, 0x001C($sp)           
/* 06AF4 809330C4 0C02927F */  jal     SkelAnime_Update
              
/* 06AF8 809330C8 2484014C */  addiu   $a0, $a0, 0x014C           ## $a0 = 0000014C
/* 06AFC 809330CC 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 06B00 809330D0 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 06B04 809330D4 24060400 */  addiu   $a2, $zero, 0x0400         ## $a2 = 00000400
/* 06B08 809330D8 0C01DE2B */  jal     Math_ScaledStepToS
              
/* 06B0C 809330DC 248400B4 */  addiu   $a0, $a0, 0x00B4           ## $a0 = 000000B4
/* 06B10 809330E0 3C028094 */  lui     $v0, %hi(D_80938C90)       ## $v0 = 80940000
/* 06B14 809330E4 24428C90 */  addiu   $v0, $v0, %lo(D_80938C90)  ## $v0 = 80938C90
/* 06B18 809330E8 8C4E0000 */  lw      $t6, 0x0000($v0)           ## 80938C90
/* 06B1C 809330EC 3C014366 */  lui     $at, 0x4366                ## $at = 43660000
/* 06B20 809330F0 44813000 */  mtc1    $at, $f6                   ## $f6 = 230.00
/* 06B24 809330F4 C5C40028 */  lwc1    $f4, 0x0028($t6)           ## 00000028
/* 06B28 809330F8 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 06B2C 809330FC 3C0F8093 */  lui     $t7, %hi(func_8092F7DC)    ## $t7 = 80930000
/* 06B30 80933100 46062200 */  add.s   $f8, $f4, $f6              
/* 06B34 80933104 25EFF7DC */  addiu   $t7, $t7, %lo(func_8092F7DC) ## $t7 = 8092F7DC
/* 06B38 80933108 E4880028 */  swc1    $f8, 0x0028($a0)           ## 00000028
/* 06B3C 8093310C 8C580000 */  lw      $t8, 0x0000($v0)           ## 80938C90
/* 06B40 80933110 8F190190 */  lw      $t9, 0x0190($t8)           ## 00000190
/* 06B44 80933114 55F90004 */  bnel    $t7, $t9, .L80933128       
/* 06B48 80933118 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 06B4C 8093311C 0C24CC4D */  jal     func_80933134              
/* 06B50 80933120 00000000 */  nop
/* 06B54 80933124 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L80933128:
/* 06B58 80933128 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 06B5C 8093312C 03E00008 */  jr      $ra                        
/* 06B60 80933130 00000000 */  nop