glabel func_80835800
/* 035F0 80835800 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 035F4 80835804 00803025 */  or      $a2, $a0, $zero            ## $a2 = 00000000
/* 035F8 80835808 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 035FC 8083580C AFA5001C */  sw      $a1, 0x001C($sp)           
/* 03600 80835810 00A02025 */  or      $a0, $a1, $zero            ## $a0 = 00000000
/* 03604 80835814 00C02825 */  or      $a1, $a2, $zero            ## $a1 = 00000000
/* 03608 80835818 0C20D1D6 */  jal     func_80834758              
/* 0360C 8083581C AFA60018 */  sw      $a2, 0x0018($sp)           
/* 03610 80835820 10400003 */  beq     $v0, $zero, .L80835830     
/* 03614 80835824 8FA60018 */  lw      $a2, 0x0018($sp)           
/* 03618 80835828 10000012 */  beq     $zero, $zero, .L80835874   
/* 0361C 8083582C 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
.L80835830:
/* 03620 80835830 8CCE067C */  lw      $t6, 0x067C($a2)           ## 0000067C
/* 03624 80835834 3C058083 */  lui     $a1, %hi(func_80835B60)    ## $a1 = 80830000
/* 03628 80835838 24A55B60 */  addiu   $a1, $a1, %lo(func_80835B60) ## $a1 = 80835B60
/* 0362C 8083583C 000E7980 */  sll     $t7, $t6,  6               
/* 03630 80835840 05E10005 */  bgez    $t7, .L80835858            
/* 03634 80835844 00C02025 */  or      $a0, $a2, $zero            ## $a0 = 00000000
/* 03638 80835848 0C20CD8E */  jal     func_80833638              
/* 0363C 8083584C 00C02025 */  or      $a0, $a2, $zero            ## $a0 = 00000000
/* 03640 80835850 10000008 */  beq     $zero, $zero, .L80835874   
/* 03644 80835854 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
.L80835858:
/* 03648 80835858 0C20D3CB */  jal     func_80834F2C              
/* 0364C 8083585C 8FA5001C */  lw      $a1, 0x001C($sp)           
/* 03650 80835860 50400004 */  beql    $v0, $zero, .L80835874     
/* 03654 80835864 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 03658 80835868 10000002 */  beq     $zero, $zero, .L80835874   
/* 0365C 8083586C 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
/* 03660 80835870 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
.L80835874:
/* 03664 80835874 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 03668 80835878 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 0366C 8083587C 03E00008 */  jr      $ra                        
/* 03670 80835880 00000000 */  nop