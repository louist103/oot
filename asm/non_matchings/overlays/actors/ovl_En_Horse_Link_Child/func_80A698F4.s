glabel func_80A698F4
/* 00524 80A698F4 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 00528 80A698F8 44802000 */  mtc1    $zero, $f4                 ## $f4 = 0.00
/* 0052C 80A698FC AFA5001C */  sw      $a1, 0x001C($sp)           
/* 00530 80A69900 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 00534 80A69904 AFA40018 */  sw      $a0, 0x0018($sp)           
/* 00538 80A69908 00802825 */  or      $a1, $a0, $zero            ## $a1 = 00000000
/* 0053C 80A6990C E4840068 */  swc1    $f4, 0x0068($a0)           ## 00000068
/* 00540 80A69910 AFA50018 */  sw      $a1, 0x0018($sp)           
/* 00544 80A69914 0C02927F */  jal     SkelAnime_FrameUpdateMatrix
              
/* 00548 80A69918 248401A0 */  addiu   $a0, $a0, 0x01A0           ## $a0 = 000001A0
/* 0054C 80A6991C 10400003 */  beq     $v0, $zero, .L80A6992C     
/* 00550 80A69920 8FA50018 */  lw      $a1, 0x0018($sp)           
/* 00554 80A69924 0C29A623 */  jal     func_80A6988C              
/* 00558 80A69928 00A02025 */  or      $a0, $a1, $zero            ## $a0 = 00000000
.L80A6992C:
/* 0055C 80A6992C 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 00560 80A69930 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 00564 80A69934 03E00008 */  jr      $ra                        
/* 00568 80A69938 00000000 */  nop