glabel func_80832DBC
/* 00BAC 80832DBC 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 00BB0 80832DC0 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 00BB4 80832DC4 908E01E9 */  lbu     $t6, 0x01E9($a0)           ## 000001E9
/* 00BB8 80832DC8 51C0001C */  beql    $t6, $zero, .L80832E3C     
/* 00BBC 80832DCC 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 00BC0 80832DD0 0C20C8BF */  jal     func_808322FC              
/* 00BC4 80832DD4 AFA40018 */  sw      $a0, 0x0018($sp)           
/* 00BC8 80832DD8 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 00BCC 80832DDC 848F01F2 */  lh      $t7, 0x01F2($a0)           ## 000001F2
/* 00BD0 80832DE0 8C9801D4 */  lw      $t8, 0x01D4($a0)           ## 000001D4
/* 00BD4 80832DE4 A70F0000 */  sh      $t7, 0x0000($t8)           ## 00000000
/* 00BD8 80832DE8 8C8801D4 */  lw      $t0, 0x01D4($a0)           ## 000001D4
/* 00BDC 80832DEC 849901F6 */  lh      $t9, 0x01F6($a0)           ## 000001F6
/* 00BE0 80832DF0 A5190004 */  sh      $t9, 0x0004($t0)           ## 00000004
/* 00BE4 80832DF4 908201E9 */  lbu     $v0, 0x01E9($a0)           ## 000001E9
/* 00BE8 80832DF8 30490008 */  andi    $t1, $v0, 0x0008           ## $t1 = 00000000
/* 00BEC 80832DFC 11200007 */  beq     $t1, $zero, .L80832E1C     
/* 00BF0 80832E00 304A0002 */  andi    $t2, $v0, 0x0002           ## $t2 = 00000000
/* 00BF4 80832E04 11400008 */  beq     $t2, $zero, .L80832E28     
/* 00BF8 80832E08 00000000 */  nop
/* 00BFC 80832E0C 848B01EE */  lh      $t3, 0x01EE($a0)           ## 000001EE
/* 00C00 80832E10 8C8C01D4 */  lw      $t4, 0x01D4($a0)           ## 000001D4
/* 00C04 80832E14 10000004 */  beq     $zero, $zero, .L80832E28   
/* 00C08 80832E18 A58B0002 */  sh      $t3, 0x0002($t4)           ## 00000002
.L80832E1C:
/* 00C0C 80832E1C 848D01F4 */  lh      $t5, 0x01F4($a0)           ## 000001F4
/* 00C10 80832E20 8C8E01D4 */  lw      $t6, 0x01D4($a0)           ## 000001D4
/* 00C14 80832E24 A5CD0002 */  sh      $t5, 0x0002($t6)           ## 00000002
.L80832E28:
/* 00C18 80832E28 0C20CB3F */  jal     func_80832CFC              
/* 00C1C 80832E2C AFA40018 */  sw      $a0, 0x0018($sp)           
/* 00C20 80832E30 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 00C24 80832E34 A08001E9 */  sb      $zero, 0x01E9($a0)         ## 000001E9
/* 00C28 80832E38 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L80832E3C:
/* 00C2C 80832E3C 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 00C30 80832E40 03E00008 */  jr      $ra                        
/* 00C34 80832E44 00000000 */  nop
