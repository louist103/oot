glabel func_80837818
/* 05608 80837818 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 0560C 8083781C AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 05610 80837820 908E0846 */  lbu     $t6, 0x0846($a0)           ## 00000846
/* 05614 80837824 80980151 */  lb      $t8, 0x0151($a0)           ## 00000151
/* 05618 80837828 24010007 */  addiu   $at, $zero, 0x0007         ## $at = 00000007
/* 0561C 8083782C 008E7821 */  addu    $t7, $a0, $t6              
/* 05620 80837830 17010008 */  bne     $t8, $at, .L80837854       
/* 05624 80837834 81E5084B */  lb      $a1, 0x084B($t7)           ## 0000084B
/* 05628 80837838 04A10002 */  bgez    $a1, .L80837844            
/* 0562C 8083783C 3C038085 */  lui     $v1, %hi(D_80854484)       ## $v1 = 80850000
/* 05630 80837840 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
.L80837844:
/* 05634 80837844 00651821 */  addu    $v1, $v1, $a1              
/* 05638 80837848 80634484 */  lb      $v1, %lo(D_80854484)($v1)  
/* 0563C 8083784C 1000002D */  beq     $zero, $zero, .L80837904   
/* 05640 80837850 A0800845 */  sb      $zero, 0x0845($a0)         ## 00000845
.L80837854:
/* 05644 80837854 AFA40020 */  sw      $a0, 0x0020($sp)           
/* 05648 80837858 0C20DD76 */  jal     func_808375D8              
/* 0564C 8083785C AFA5001C */  sw      $a1, 0x001C($sp)           
/* 05650 80837860 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 05654 80837864 10400003 */  beq     $v0, $zero, .L80837874     
/* 05658 80837868 8FA5001C */  lw      $a1, 0x001C($sp)           
/* 0565C 8083786C 10000020 */  beq     $zero, $zero, .L808378F0   
/* 05660 80837870 24030018 */  addiu   $v1, $zero, 0x0018         ## $v1 = 00000018
.L80837874:
/* 05664 80837874 04A10009 */  bgez    $a1, .L8083789C            
/* 05668 80837878 3C038085 */  lui     $v1, %hi(D_80854480)       ## $v1 = 80850000
/* 0566C 8083787C 0C20CEF3 */  jal     func_80833BCC              
/* 05670 80837880 AFA40020 */  sw      $a0, 0x0020($sp)           
/* 05674 80837884 10400003 */  beq     $v0, $zero, .L80837894     
/* 05678 80837888 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0567C 8083788C 10000013 */  beq     $zero, $zero, .L808378DC   
/* 05680 80837890 00001825 */  or      $v1, $zero, $zero          ## $v1 = 00000000
.L80837894:
/* 05684 80837894 10000011 */  beq     $zero, $zero, .L808378DC   
/* 05688 80837898 24030004 */  addiu   $v1, $zero, 0x0004         ## $v1 = 00000004
.L8083789C:
/* 0568C 8083789C 00651821 */  addu    $v1, $v1, $a1              
/* 05690 808378A0 80634480 */  lb      $v1, %lo(D_80854480)($v1)  
/* 05694 808378A4 2401000C */  addiu   $at, $zero, 0x000C         ## $at = 0000000C
/* 05698 808378A8 5461000D */  bnel    $v1, $at, .L808378E0       
/* 0569C 808378AC 80890151 */  lb      $t1, 0x0151($a0)           ## 00000151
/* 056A0 808378B0 8C990680 */  lw      $t9, 0x0680($a0)           ## 00000680
/* 056A4 808378B4 3C014000 */  lui     $at, 0x4000                ## $at = 40000000
/* 056A8 808378B8 03214025 */  or      $t0, $t9, $at              ## $t0 = 40000000
/* 056AC 808378BC AC880680 */  sw      $t0, 0x0680($a0)           ## 00000680
/* 056B0 808378C0 AFA40020 */  sw      $a0, 0x0020($sp)           
/* 056B4 808378C4 0C20CEF3 */  jal     func_80833BCC              
/* 056B8 808378C8 AFA30018 */  sw      $v1, 0x0018($sp)           
/* 056BC 808378CC 8FA30018 */  lw      $v1, 0x0018($sp)           
/* 056C0 808378D0 14400002 */  bne     $v0, $zero, .L808378DC     
/* 056C4 808378D4 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 056C8 808378D8 00001825 */  or      $v1, $zero, $zero          ## $v1 = 00000000
.L808378DC:
/* 056CC 808378DC 80890151 */  lb      $t1, 0x0151($a0)           ## 00000151
.L808378E0:
/* 056D0 808378E0 24010006 */  addiu   $at, $zero, 0x0006         ## $at = 00000006
/* 056D4 808378E4 15210002 */  bne     $t1, $at, .L808378F0       
/* 056D8 808378E8 00000000 */  nop
/* 056DC 808378EC 00001825 */  or      $v1, $zero, $zero          ## $v1 = 00000000
.L808378F0:
/* 056E0 808378F0 0C023C68 */  jal     func_8008F1A0              
/* 056E4 808378F4 AFA30018 */  sw      $v1, 0x0018($sp)           
/* 056E8 808378F8 10400002 */  beq     $v0, $zero, .L80837904     
/* 056EC 808378FC 8FA30018 */  lw      $v1, 0x0018($sp)           
/* 056F0 80837900 24630001 */  addiu   $v1, $v1, 0x0001           ## $v1 = 00000001
.L80837904:
/* 056F4 80837904 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 056F8 80837908 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 056FC 8083790C 00601025 */  or      $v0, $v1, $zero            ## $v0 = 00000001
/* 05700 80837910 03E00008 */  jr      $ra                        
/* 05704 80837914 00000000 */  nop
