glabel func_8084409C
/* 11E8C 8084409C 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 11E90 808440A0 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 11E94 808440A4 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 11E98 808440A8 AFA40028 */  sw      $a0, 0x0028($sp)           
/* 11E9C 808440AC AFA60030 */  sw      $a2, 0x0030($sp)           
/* 11EA0 808440B0 AFA70034 */  sw      $a3, 0x0034($sp)           
/* 11EA4 808440B4 8CA603AC */  lw      $a2, 0x03AC($a1)           ## 000003AC
/* 11EA8 808440B8 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 11EAC 808440BC 0C20D591 */  jal     func_80835644              
/* 11EB0 808440C0 AFA60024 */  sw      $a2, 0x0024($sp)           
/* 11EB4 808440C4 14400010 */  bne     $v0, $zero, .L80844108     
/* 11EB8 808440C8 8FA60024 */  lw      $a2, 0x0024($sp)           
/* 11EBC 808440CC 860E00B6 */  lh      $t6, 0x00B6($s0)           ## 000000B6
/* 11EC0 808440D0 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 11EC4 808440D4 A4CE0032 */  sh      $t6, 0x0032($a2)           ## 00000032
/* 11EC8 808440D8 C7A40030 */  lwc1    $f4, 0x0030($sp)           
/* 11ECC 808440DC E4C40068 */  swc1    $f4, 0x0068($a2)           ## 00000068
/* 11ED0 808440E0 C7A60034 */  lwc1    $f6, 0x0034($sp)           
/* 11ED4 808440E4 E4C60060 */  swc1    $f6, 0x0060($a2)           ## 00000060
/* 11ED8 808440E8 0C20D191 */  jal     func_80834644              
/* 11EDC 808440EC 8FA40028 */  lw      $a0, 0x0028($sp)           
/* 11EE0 808440F0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 11EE4 808440F4 0C00BDF7 */  jal     func_8002F7DC              
/* 11EE8 808440F8 2405083A */  addiu   $a1, $zero, 0x083A         ## $a1 = 0000083A
/* 11EEC 808440FC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 11EF0 80844100 0C20C9A6 */  jal     func_80832698              
/* 11EF4 80844104 24056800 */  addiu   $a1, $zero, 0x6800         ## $a1 = 00006800
.L80844108:
/* 11EF8 80844108 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 11EFC 8084410C 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 11F00 80844110 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 11F04 80844114 03E00008 */  jr      $ra                        
/* 11F08 80844118 00000000 */  nop