glabel func_80A9062C
/* 0061C 80A9062C 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 00620 80A90630 AFA40028 */  sw      $a0, 0x0028($sp)
/* 00624 80A90634 AFBF0024 */  sw      $ra, 0x0024($sp)
/* 00628 80A90638 3C040600 */  lui     $a0, 0x0600                ## $a0 = 06000000
/* 0062C 80A9063C AFA5002C */  sw      $a1, 0x002C($sp)
/* 00630 80A90640 0C028800 */  jal     SkelAnime_GetFrameCount

/* 00634 80A90644 24840214 */  addiu   $a0, $a0, 0x0214           ## $a0 = 06000214
/* 00638 80A90648 44822000 */  mtc1    $v0, $f4                   ## $f4 = 0.00
/* 0063C 80A9064C 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 00640 80A90650 3C01C120 */  lui     $at, 0xC120                ## $at = C1200000
/* 00644 80A90654 468021A0 */  cvt.s.w $f6, $f4
/* 00648 80A90658 44819000 */  mtc1    $at, $f18                  ## $f18 = -10.00
/* 0064C 80A9065C 8FA40028 */  lw      $a0, 0x0028($sp)
/* 00650 80A90660 3C050600 */  lui     $a1, 0x0600                ## $a1 = 06000000
/* 00654 80A90664 24080002 */  addiu   $t0, $zero, 0x0002         ## $t0 = 00000002
/* 00658 80A90668 44060000 */  mfc1    $a2, $f0
/* 0065C 80A9066C 4600320D */  trunc.w.s $f8, $f6
/* 00660 80A90670 44070000 */  mfc1    $a3, $f0
/* 00664 80A90674 AFA80014 */  sw      $t0, 0x0014($sp)
/* 00668 80A90678 24A50214 */  addiu   $a1, $a1, 0x0214           ## $a1 = 06000214
/* 0066C 80A9067C 440F4000 */  mfc1    $t7, $f8
/* 00670 80A90680 24840150 */  addiu   $a0, $a0, 0x0150           ## $a0 = 00000150
/* 00674 80A90684 E7B20018 */  swc1    $f18, 0x0018($sp)
/* 00678 80A90688 000FC400 */  sll     $t8, $t7, 16
/* 0067C 80A9068C 0018CC03 */  sra     $t9, $t8, 16
/* 00680 80A90690 44995000 */  mtc1    $t9, $f10                  ## $f10 = 0.00
/* 00684 80A90694 00000000 */  nop
/* 00688 80A90698 46805420 */  cvt.s.w $f16, $f10
/* 0068C 80A9069C 0C029468 */  jal     SkelAnime_ChangeAnim

/* 00690 80A906A0 E7B00010 */  swc1    $f16, 0x0010($sp)
/* 00694 80A906A4 8FAA0028 */  lw      $t2, 0x0028($sp)
/* 00698 80A906A8 3C0980A9 */  lui     $t1, %hi(func_80A906C4)    ## $t1 = 80A90000
/* 0069C 80A906AC 252906C4 */  addiu   $t1, $t1, %lo(func_80A906C4) ## $t1 = 80A906C4
/* 006A0 80A906B0 AD49014C */  sw      $t1, 0x014C($t2)           ## 0000014C
/* 006A4 80A906B4 8FBF0024 */  lw      $ra, 0x0024($sp)
/* 006A8 80A906B8 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 006AC 80A906BC 03E00008 */  jr      $ra
/* 006B0 80A906C0 00000000 */  nop
