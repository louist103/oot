glabel func_8096B6D0
/* 01C00 8096B6D0 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 01C04 8096B6D4 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 01C08 8096B6D8 AFA5001C */  sw      $a1, 0x001C($sp)           
/* 01C0C 8096B6DC AFA40018 */  sw      $a0, 0x0018($sp)           
/* 01C10 8096B6E0 24060004 */  addiu   $a2, $zero, 0x0004         ## $a2 = 00000004
/* 01C14 8096B6E4 0C25A744 */  jal     func_80969D10              
/* 01C18 8096B6E8 24070002 */  addiu   $a3, $zero, 0x0002         ## $a3 = 00000002
/* 01C1C 8096B6EC 1040001A */  beq     $v0, $zero, .L8096B758     
/* 01C20 8096B6F0 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 01C24 8096B6F4 240E0016 */  addiu   $t6, $zero, 0x0016         ## $t6 = 00000016
/* 01C28 8096B6F8 240F0002 */  addiu   $t7, $zero, 0x0002         ## $t7 = 00000002
/* 01C2C 8096B6FC AC8E0198 */  sw      $t6, 0x0198($a0)           ## 00000198
/* 01C30 8096B700 AC8F019C */  sw      $t7, 0x019C($a0)           ## 0000019C
/* 01C34 8096B704 3C188016 */  lui     $t8, %hi(gGameInfo)
/* 01C38 8096B708 8F18FA90 */  lw      $t8, %lo(gGameInfo)($t8)
/* 01C3C 8096B70C 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 01C40 8096B710 44814000 */  mtc1    $at, $f8                   ## $f8 = 10.00
/* 01C44 8096B714 8719145E */  lh      $t9, 0x145E($t8)           ## 8016145E
/* 01C48 8096B718 8C8901AC */  lw      $t1, 0x01AC($a0)           ## 000001AC
/* 01C4C 8096B71C 240800FF */  addiu   $t0, $zero, 0x00FF         ## $t0 = 000000FF
/* 01C50 8096B720 44992000 */  mtc1    $t9, $f4                   ## $f4 = 0.00
/* 01C54 8096B724 AC8801A8 */  sw      $t0, 0x01A8($a0)           ## 000001A8
/* 01C58 8096B728 468021A0 */  cvt.s.w $f6, $f4                   
/* 01C5C 8096B72C 46083280 */  add.s   $f10, $f6, $f8             
/* 01C60 8096B730 15200007 */  bne     $t1, $zero, .L8096B750     
/* 01C64 8096B734 E48A01A4 */  swc1    $f10, 0x01A4($a0)          ## 000001A4
/* 01C68 8096B738 8FA5001C */  lw      $a1, 0x001C($sp)           
/* 01C6C 8096B73C 0C25AD2A */  jal     func_8096B4A8              
/* 01C70 8096B740 AFA40018 */  sw      $a0, 0x0018($sp)           
/* 01C74 8096B744 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 01C78 8096B748 240A0001 */  addiu   $t2, $zero, 0x0001         ## $t2 = 00000001
/* 01C7C 8096B74C AC8A01AC */  sw      $t2, 0x01AC($a0)           ## 000001AC
.L8096B750:
/* 01C80 8096B750 240B00FF */  addiu   $t3, $zero, 0x00FF         ## $t3 = 000000FF
/* 01C84 8096B754 A08B00C8 */  sb      $t3, 0x00C8($a0)           ## 000000C8
.L8096B758:
/* 01C88 8096B758 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 01C8C 8096B75C 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 01C90 8096B760 03E00008 */  jr      $ra                        
/* 01C94 8096B764 00000000 */  nop
