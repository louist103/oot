glabel func_8087C794
/* 009A4 8087C794 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 009A8 8087C798 00803825 */  or      $a3, $a0, $zero            ## $a3 = 00000000
/* 009AC 8087C79C AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 009B0 8087C7A0 AFA5001C */  sw      $a1, 0x001C($sp)           
/* 009B4 8087C7A4 00A02025 */  or      $a0, $a1, $zero            ## $a0 = 00000000
/* 009B8 8087C7A8 90E50168 */  lbu     $a1, 0x0168($a3)           ## 00000168
/* 009BC 8087C7AC 0C00B2D0 */  jal     Flags_GetSwitch
              
/* 009C0 8087C7B0 AFA70018 */  sw      $a3, 0x0018($sp)           
/* 009C4 8087C7B4 10400007 */  beq     $v0, $zero, .L8087C7D4     
/* 009C8 8087C7B8 8FA70018 */  lw      $a3, 0x0018($sp)           
/* 009CC 8087C7BC 24E4016E */  addiu   $a0, $a3, 0x016E           ## $a0 = 0000016E
/* 009D0 8087C7C0 2405015E */  addiu   $a1, $zero, 0x015E         ## $a1 = 0000015E
/* 009D4 8087C7C4 24060014 */  addiu   $a2, $zero, 0x0014         ## $a2 = 00000014
/* 009D8 8087C7C8 0C01DE5F */  jal     Math_ApproxS
              
/* 009DC 8087C7CC AFA70018 */  sw      $a3, 0x0018($sp)           
/* 009E0 8087C7D0 8FA70018 */  lw      $a3, 0x0018($sp)           
.L8087C7D4:
/* 009E4 8087C7D4 8FAE001C */  lw      $t6, 0x001C($sp)           
/* 009E8 8087C7D8 91CF1C27 */  lbu     $t7, 0x1C27($t6)           ## 00001C27
/* 009EC 8087C7DC 51E00006 */  beql    $t7, $zero, .L8087C7F8     
/* 009F0 8087C7E0 8CE80004 */  lw      $t0, 0x0004($a3)           ## 00000004
/* 009F4 8087C7E4 8CF80004 */  lw      $t8, 0x0004($a3)           ## 00000004
/* 009F8 8087C7E8 37190080 */  ori     $t9, $t8, 0x0080           ## $t9 = 00000080
/* 009FC 8087C7EC 10000005 */  beq     $zero, $zero, .L8087C804   
/* 00A00 8087C7F0 ACF90004 */  sw      $t9, 0x0004($a3)           ## 00000004
/* 00A04 8087C7F4 8CE80004 */  lw      $t0, 0x0004($a3)           ## 00000004
.L8087C7F8:
/* 00A08 8087C7F8 2401FF7F */  addiu   $at, $zero, 0xFF7F         ## $at = FFFFFF7F
/* 00A0C 8087C7FC 01014824 */  and     $t1, $t0, $at              
/* 00A10 8087C800 ACE90004 */  sw      $t1, 0x0004($a3)           ## 00000004
.L8087C804:
/* 00A14 8087C804 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 00A18 8087C808 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 00A1C 8087C80C 03E00008 */  jr      $ra                        
/* 00A20 8087C810 00000000 */  nop
