glabel func_80885554
/* 003B4 80885554 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 003B8 80885558 AFA40018 */  sw      $a0, 0x0018($sp)           
/* 003BC 8088555C 8FAE0018 */  lw      $t6, 0x0018($sp)           
/* 003C0 80885560 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 003C4 80885564 00A02025 */  or      $a0, $a1, $zero            ## $a0 = 00000000
/* 003C8 80885568 0C00B2D0 */  jal     Flags_GetSwitch
              
/* 003CC 8088556C 85C5001C */  lh      $a1, 0x001C($t6)           ## 0000001C
/* 003D0 80885570 14400004 */  bne     $v0, $zero, .L80885584     
/* 003D4 80885574 8FB80018 */  lw      $t8, 0x0018($sp)           
/* 003D8 80885578 3C0F8088 */  lui     $t7, %hi(func_80885594)    ## $t7 = 80880000
/* 003DC 8088557C 25EF5594 */  addiu   $t7, $t7, %lo(func_80885594) ## $t7 = 80885594
/* 003E0 80885580 AF0F014C */  sw      $t7, 0x014C($t8)           ## 0000014C
.L80885584:
/* 003E4 80885584 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 003E8 80885588 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 003EC 8088558C 03E00008 */  jr      $ra                        
/* 003F0 80885590 00000000 */  nop