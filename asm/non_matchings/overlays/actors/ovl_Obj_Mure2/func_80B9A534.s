glabel func_80B9A534
/* 005F4 80B9A534 848E001C */  lh      $t6, 0x001C($a0)           ## 0000001C
/* 005F8 80B9A538 3C0380BA */  lui     $v1, %hi(D_80B9A818)       ## $v1 = 80BA0000
/* 005FC 80B9A53C 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 00600 80B9A540 31CF0003 */  andi    $t7, $t6, 0x0003           ## $t7 = 00000000
/* 00604 80B9A544 000FC040 */  sll     $t8, $t7,  1               
/* 00608 80B9A548 00781821 */  addu    $v1, $v1, $t8              
/* 0060C 80B9A54C 8463A818 */  lh      $v1, %lo(D_80B9A818)($v1)  
/* 00610 80B9A550 00802825 */  or      $a1, $a0, $zero            ## $a1 = 00000000
/* 00614 80B9A554 1860001A */  blez    $v1, .L80B9A5C0            
/* 00618 80B9A558 00000000 */  nop
.L80B9A55C:
/* 0061C 80B9A55C 8CA60150 */  lw      $a2, 0x0150($a1)           ## 00000150
/* 00620 80B9A560 50C00014 */  beql    $a2, $zero, .L80B9A5B4     
/* 00624 80B9A564 24420001 */  addiu   $v0, $v0, 0x0001           ## $v0 = 00000001
/* 00628 80B9A568 94870180 */  lhu     $a3, 0x0180($a0)           ## 00000180
/* 0062C 80B9A56C 0047C807 */  srav    $t9, $a3, $v0              
/* 00630 80B9A570 33280001 */  andi    $t0, $t9, 0x0001           ## $t0 = 00000000
/* 00634 80B9A574 5500000F */  bnel    $t0, $zero, .L80B9A5B4     
/* 00638 80B9A578 24420001 */  addiu   $v0, $v0, 0x0001           ## $v0 = 00000002
/* 0063C 80B9A57C 8CC90130 */  lw      $t1, 0x0130($a2)           ## 00000130
/* 00640 80B9A580 240A0001 */  addiu   $t2, $zero, 0x0001         ## $t2 = 00000001
/* 00644 80B9A584 004A5804 */  sllv    $t3, $t2, $v0              
/* 00648 80B9A588 15200009 */  bne     $t1, $zero, .L80B9A5B0     
/* 0064C 80B9A58C 00EB6025 */  or      $t4, $a3, $t3              ## $t4 = 00000000
/* 00650 80B9A590 A48C0180 */  sh      $t4, 0x0180($a0)           ## 00000180
/* 00654 80B9A594 ACA00150 */  sw      $zero, 0x0150($a1)         ## 00000150
/* 00658 80B9A598 848D001C */  lh      $t5, 0x001C($a0)           ## 0000001C
/* 0065C 80B9A59C 3C0380BA */  lui     $v1, %hi(D_80B9A818)       ## $v1 = 80BA0000
/* 00660 80B9A5A0 31AE0003 */  andi    $t6, $t5, 0x0003           ## $t6 = 00000000
/* 00664 80B9A5A4 000E7840 */  sll     $t7, $t6,  1               
/* 00668 80B9A5A8 006F1821 */  addu    $v1, $v1, $t7              
/* 0066C 80B9A5AC 8463A818 */  lh      $v1, %lo(D_80B9A818)($v1)  
.L80B9A5B0:
/* 00670 80B9A5B0 24420001 */  addiu   $v0, $v0, 0x0001           ## $v0 = 00000003
.L80B9A5B4:
/* 00674 80B9A5B4 0043082A */  slt     $at, $v0, $v1              
/* 00678 80B9A5B8 1420FFE8 */  bne     $at, $zero, .L80B9A55C     
/* 0067C 80B9A5BC 24A50004 */  addiu   $a1, $a1, 0x0004           ## $a1 = 00000004
.L80B9A5C0:
/* 00680 80B9A5C0 03E00008 */  jr      $ra                        
/* 00684 80B9A5C4 00000000 */  nop