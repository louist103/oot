glabel func_8083A2F8
/* 080E8 8083A2F8 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 080EC 8083A2FC AFBF001C */  sw      $ra, 0x001C($sp)           
/* 080F0 8083A300 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 080F4 8083A304 3C068085 */  lui     $a2, %hi(func_8084B530)    ## $a2 = 80850000
/* 080F8 8083A308 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 080FC 8083A30C AFA40020 */  sw      $a0, 0x0020($sp)           
/* 08100 8083A310 24C6B530 */  addiu   $a2, $a2, %lo(func_8084B530) ## $a2 = 8084B530
/* 08104 8083A314 0C20D76B */  jal     func_80835DAC              
/* 08108 8083A318 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 0810C 8083A31C 8E0E067C */  lw      $t6, 0x067C($s0)           ## 0000067C
/* 08110 8083A320 9605010E */  lhu     $a1, 0x010E($s0)           ## 0000010E
/* 08114 8083A324 3C012000 */  lui     $at, 0x2000                ## $at = 20000000
/* 08118 8083A328 34210040 */  ori     $at, $at, 0x0040           ## $at = 20000040
/* 0811C 8083A32C 01C17825 */  or      $t7, $t6, $at              ## $t7 = 20000040
/* 08120 8083A330 10A00006 */  beq     $a1, $zero, .L8083A34C     
/* 08124 8083A334 AE0F067C */  sw      $t7, 0x067C($s0)           ## 0000067C
/* 08128 8083A338 8FA40020 */  lw      $a0, 0x0020($sp)           
/* 0812C 8083A33C 0C042DA0 */  jal     func_8010B680              
/* 08130 8083A340 8E060694 */  lw      $a2, 0x0694($s0)           ## 00000694
/* 08134 8083A344 8E180694 */  lw      $t8, 0x0694($s0)           ## 00000694
/* 08138 8083A348 AE180664 */  sw      $t8, 0x0664($s0)           ## 00000664
.L8083A34C:
/* 0813C 8083A34C 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 08140 8083A350 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 08144 8083A354 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 08148 8083A358 03E00008 */  jr      $ra                        
/* 0814C 8083A35C 00000000 */  nop