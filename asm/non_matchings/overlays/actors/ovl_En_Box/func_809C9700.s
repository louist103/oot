.late_rodata
glabel D_809CA8F8
 .word 0x46AFC800

.text
glabel func_809C9700
/* 00940 809C9700 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 00944 809C9704 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 00948 809C9708 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 0094C 809C970C 8482001C */  lh      $v0, 0x001C($a0)           ## 0000001C
/* 00950 809C9710 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 00954 809C9714 00A03025 */  or      $a2, $a1, $zero            ## $a2 = 00000000
/* 00958 809C9718 3042001F */  andi    $v0, $v0, 0x001F           ## $v0 = 00000000
/* 0095C 809C971C 28410014 */  slti    $at, $v0, 0x0014           
/* 00960 809C9720 14200009 */  bne     $at, $zero, .L809C9748     
/* 00964 809C9724 8CA31C44 */  lw      $v1, 0x1C44($a1)           ## 00001C44
/* 00968 809C9728 28410020 */  slti    $at, $v0, 0x0020           
/* 0096C 809C972C 50200007 */  beql    $at, $zero, .L809C974C     
/* 00970 809C9730 26040024 */  addiu   $a0, $s0, 0x0024           ## $a0 = 00000024
/* 00974 809C9734 AFA30020 */  sw      $v1, 0x0020($sp)           
/* 00978 809C9738 0C00BD7C */  jal     func_8002F5F0              
/* 0097C 809C973C AFA6002C */  sw      $a2, 0x002C($sp)           
/* 00980 809C9740 8FA30020 */  lw      $v1, 0x0020($sp)           
/* 00984 809C9744 8FA6002C */  lw      $a2, 0x002C($sp)           
.L809C9748:
/* 00988 809C9748 26040024 */  addiu   $a0, $s0, 0x0024           ## $a0 = 00000024
.L809C974C:
/* 0098C 809C974C 24650024 */  addiu   $a1, $v1, 0x0024           ## $a1 = 00000024
/* 00990 809C9750 AFA30020 */  sw      $v1, 0x0020($sp)           
/* 00994 809C9754 0C032D94 */  jal     func_800CB650              
/* 00998 809C9758 AFA6002C */  sw      $a2, 0x002C($sp)           
/* 0099C 809C975C 3C01809D */  lui     $at, %hi(D_809CA8F8)       ## $at = 809D0000
/* 009A0 809C9760 C424A8F8 */  lwc1    $f4, %lo(D_809CA8F8)($at)  
/* 009A4 809C9764 8FA30020 */  lw      $v1, 0x0020($sp)           
/* 009A8 809C9768 8FA6002C */  lw      $a2, 0x002C($sp)           
/* 009AC 809C976C 4600203C */  c.lt.s  $f4, $f0                   
/* 009B0 809C9770 00000000 */  nop
/* 009B4 809C9774 45020004 */  bc1fl   .L809C9788                 
/* 009B8 809C9778 920201FB */  lbu     $v0, 0x01FB($s0)           ## 000001FB
/* 009BC 809C977C 1000003E */  beq     $zero, $zero, .L809C9878   
/* 009C0 809C9780 A20001FB */  sb      $zero, 0x01FB($s0)         ## 000001FB
/* 009C4 809C9784 920201FB */  lbu     $v0, 0x01FB($s0)           ## 000001FB
.L809C9788:
/* 009C8 809C9788 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 009CC 809C978C 1440000C */  bne     $v0, $zero, .L809C97C0     
/* 009D0 809C9790 00000000 */  nop
/* 009D4 809C9794 8C620680 */  lw      $v0, 0x0680($v1)           ## 00000680
/* 009D8 809C9798 24180001 */  addiu   $t8, $zero, 0x0001         ## $t8 = 00000001
/* 009DC 809C979C 000271C0 */  sll     $t6, $v0,  7               
/* 009E0 809C97A0 05C20006 */  bltzl   $t6, .L809C97BC            
/* 009E4 809C97A4 A21801FB */  sb      $t8, 0x01FB($s0)           ## 000001FB
/* 009E8 809C97A8 3C010080 */  lui     $at, 0x0080                ## $at = 00800000
/* 009EC 809C97AC 00417825 */  or      $t7, $v0, $at              ## $t7 = 00800000
/* 009F0 809C97B0 10000031 */  beq     $zero, $zero, .L809C9878   
/* 009F4 809C97B4 AC6F0680 */  sw      $t7, 0x0680($v1)           ## 00000680
/* 009F8 809C97B8 A21801FB */  sb      $t8, 0x01FB($s0)           ## 000001FB
.L809C97BC:
/* 009FC 809C97BC 330200FF */  andi    $v0, $t8, 0x00FF           ## $v0 = 00000001
.L809C97C0:
/* 00A00 809C97C0 14410006 */  bne     $v0, $at, .L809C97DC       
/* 00A04 809C97C4 00C02025 */  or      $a0, $a2, $zero            ## $a0 = 00000000
/* 00A08 809C97C8 0C042F56 */  jal     func_8010BD58              
/* 00A0C 809C97CC 24050001 */  addiu   $a1, $zero, 0x0001         ## $a1 = 00000001
/* 00A10 809C97D0 24190002 */  addiu   $t9, $zero, 0x0002         ## $t9 = 00000002
/* 00A14 809C97D4 10000028 */  beq     $zero, $zero, .L809C9878   
/* 00A18 809C97D8 A21901FB */  sb      $t9, 0x01FB($s0)           ## 000001FB
.L809C97DC:
/* 00A1C 809C97DC 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 00A20 809C97E0 14410025 */  bne     $v0, $at, .L809C9878       
/* 00A24 809C97E4 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00A28 809C97E8 00C11021 */  addu    $v0, $a2, $at              
/* 00A2C 809C97EC 944804C6 */  lhu     $t0, 0x04C6($v0)           ## 000004C6
/* 00A30 809C97F0 24010004 */  addiu   $at, $zero, 0x0004         ## $at = 00000004
/* 00A34 809C97F4 55010021 */  bnel    $t0, $at, .L809C987C       
/* 00A38 809C97F8 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00A3C 809C97FC 944304C4 */  lhu     $v1, 0x04C4($v0)           ## 000004C4
/* 00A40 809C9800 24010008 */  addiu   $at, $zero, 0x0008         ## $at = 00000008
/* 00A44 809C9804 54610005 */  bnel    $v1, $at, .L809C981C       
/* 00A48 809C9808 24010009 */  addiu   $at, $zero, 0x0009         ## $at = 00000009
/* 00A4C 809C980C 920901F9 */  lbu     $t1, 0x01F9($s0)           ## 000001F9
/* 00A50 809C9810 24010009 */  addiu   $at, $zero, 0x0009         ## $at = 00000009
/* 00A54 809C9814 11210007 */  beq     $t1, $at, .L809C9834       
/* 00A58 809C9818 24010009 */  addiu   $at, $zero, 0x0009         ## $at = 00000009
.L809C981C:
/* 00A5C 809C981C 54610016 */  bnel    $v1, $at, .L809C9878       
/* 00A60 809C9820 A20001FB */  sb      $zero, 0x01FB($s0)         ## 000001FB
/* 00A64 809C9824 920A01F9 */  lbu     $t2, 0x01F9($s0)           ## 000001F9
/* 00A68 809C9828 2401000A */  addiu   $at, $zero, 0x000A         ## $at = 0000000A
/* 00A6C 809C982C 55410012 */  bnel    $t2, $at, .L809C9878       
/* 00A70 809C9830 A20001FB */  sb      $zero, 0x01FB($s0)         ## 000001FB
.L809C9834:
/* 00A74 809C9834 8E0B0004 */  lw      $t3, 0x0004($s0)           ## 00000004
/* 00A78 809C9838 3C01FDFF */  lui     $at, 0xFDFF                ## $at = FDFF0000
/* 00A7C 809C983C 3421FFFF */  ori     $at, $at, 0xFFFF           ## $at = FDFFFFFF
/* 00A80 809C9840 01616024 */  and     $t4, $t3, $at              
/* 00A84 809C9844 AE0C0004 */  sw      $t4, 0x0004($s0)           ## 00000004
/* 00A88 809C9848 3C05809D */  lui     $a1, %hi(func_809C99C4)    ## $a1 = 809D0000
/* 00A8C 809C984C 24A599C4 */  addiu   $a1, $a1, %lo(func_809C99C4) ## $a1 = 809C99C4
/* 00A90 809C9850 AFA6002C */  sw      $a2, 0x002C($sp)           
/* 00A94 809C9854 0C272370 */  jal     func_809C8DC0              
/* 00A98 809C9858 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00A9C 809C985C 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 00AA0 809C9860 0C020120 */  jal     func_80080480              
/* 00AA4 809C9864 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 00AA8 809C9868 AE0001A8 */  sw      $zero, 0x01A8($s0)         ## 000001A8
/* 00AAC 809C986C 10000002 */  beq     $zero, $zero, .L809C9878   
/* 00AB0 809C9870 A20001FB */  sb      $zero, 0x01FB($s0)         ## 000001FB
/* 00AB4 809C9874 A20001FB */  sb      $zero, 0x01FB($s0)         ## 000001FB
.L809C9878:
/* 00AB8 809C9878 8FBF001C */  lw      $ra, 0x001C($sp)           
.L809C987C:
/* 00ABC 809C987C 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 00AC0 809C9880 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 00AC4 809C9884 03E00008 */  jr      $ra                        
/* 00AC8 809C9888 00000000 */  nop