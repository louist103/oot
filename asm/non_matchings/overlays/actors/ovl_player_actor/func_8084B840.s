glabel func_8084B840
/* 19630 8084B840 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 19634 8084B844 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 19638 8084B848 AFA40018 */  sw      $a0, 0x0018($sp)           
/* 1963C 8084B84C AFA60020 */  sw      $a2, 0x0020($sp)           
/* 19640 8084B850 90A6007C */  lbu     $a2, 0x007C($a1)           ## 0000007C
/* 19644 8084B854 24010032 */  addiu   $at, $zero, 0x0032         ## $at = 00000032
/* 19648 8084B858 00A03825 */  or      $a3, $a1, $zero            ## $a3 = 00000000
/* 1964C 8084B85C 10C1000A */  beq     $a2, $at, .L8084B888       
/* 19650 8084B860 248407C0 */  addiu   $a0, $a0, 0x07C0           ## $a0 = 000007C0
/* 19654 8084B864 00C02825 */  or      $a1, $a2, $zero            ## $a1 = 00000000
/* 19658 8084B868 0C00FAE1 */  jal     DynaPolyInfo_GetActor
              ## DynaPolyInfo_getActor
/* 1965C 8084B86C AFA7001C */  sw      $a3, 0x001C($sp)           
/* 19660 8084B870 8FA7001C */  lw      $a3, 0x001C($sp)           
/* 19664 8084B874 10400004 */  beq     $v0, $zero, .L8084B888     
/* 19668 8084B878 00402025 */  or      $a0, $v0, $zero            ## $a0 = 00000000
/* 1966C 8084B87C 8FA50020 */  lw      $a1, 0x0020($sp)           
/* 19670 8084B880 0C00B7E9 */  jal     func_8002DFA4              
/* 19674 8084B884 84E60032 */  lh      $a2, 0x0032($a3)           ## 00000032
.L8084B888:
/* 19678 8084B888 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 1967C 8084B88C 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 19680 8084B890 03E00008 */  jr      $ra                        
/* 19684 8084B894 00000000 */  nop
