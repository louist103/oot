glabel func_809E6000
/* 00830 809E6000 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 00834 809E6004 AFA40028 */  sw      $a0, 0x0028($sp)
/* 00838 809E6008 AFBF0024 */  sw      $ra, 0x0024($sp)
/* 0083C 809E600C 3C040600 */  lui     $a0, 0x0600                ## $a0 = 06000000
/* 00840 809E6010 0C028800 */  jal     SkelAnime_GetFrameCount

/* 00844 809E6014 24840208 */  addiu   $a0, $a0, 0x0208           ## $a0 = 06000208
/* 00848 809E6018 44822000 */  mtc1    $v0, $f4                   ## $f4 = 0.00
/* 0084C 809E601C 3C01C040 */  lui     $at, 0xC040                ## $at = C0400000
/* 00850 809E6020 44814000 */  mtc1    $at, $f8                   ## $f8 = -3.00
/* 00854 809E6024 468021A0 */  cvt.s.w $f6, $f4
/* 00858 809E6028 8FA40028 */  lw      $a0, 0x0028($sp)
/* 0085C 809E602C 3C050600 */  lui     $a1, 0x0600                ## $a1 = 06000000
/* 00860 809E6030 240E0002 */  addiu   $t6, $zero, 0x0002         ## $t6 = 00000002
/* 00864 809E6034 AFAE0014 */  sw      $t6, 0x0014($sp)
/* 00868 809E6038 24A50208 */  addiu   $a1, $a1, 0x0208           ## $a1 = 06000208
/* 0086C 809E603C E7A60010 */  swc1    $f6, 0x0010($sp)
/* 00870 809E6040 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 00874 809E6044 3C074170 */  lui     $a3, 0x4170                ## $a3 = 41700000
/* 00878 809E6048 E7A80018 */  swc1    $f8, 0x0018($sp)
/* 0087C 809E604C 0C029468 */  jal     SkelAnime_ChangeAnim

/* 00880 809E6050 2484017C */  addiu   $a0, $a0, 0x017C           ## $a0 = 0000017C
/* 00884 809E6054 8FA20028 */  lw      $v0, 0x0028($sp)
/* 00888 809E6058 3C0F809E */  lui     $t7, %hi(func_809E7458)    ## $t7 = 809E0000
/* 0088C 809E605C 25EF7458 */  addiu   $t7, $t7, %lo(func_809E7458) ## $t7 = 809E7458
/* 00890 809E6060 A44001C6 */  sh      $zero, 0x01C6($v0)         ## 000001C6
/* 00894 809E6064 AC4F01C0 */  sw      $t7, 0x01C0($v0)           ## 000001C0
/* 00898 809E6068 8FBF0024 */  lw      $ra, 0x0024($sp)
/* 0089C 809E606C 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 008A0 809E6070 03E00008 */  jr      $ra
/* 008A4 809E6074 00000000 */  nop