glabel func_80A44C68
/* 01F38 80A44C68 AFA50004 */  sw      $a1, 0x0004($sp)           
/* 01F3C 80A44C6C 8C820208 */  lw      $v0, 0x0208($a0)           ## 00000208
/* 01F40 80A44C70 54400004 */  bnel    $v0, $zero, .L80A44C84     
/* 01F44 80A44C74 90430000 */  lbu     $v1, 0x0000($v0)           ## 00000000
/* 01F48 80A44C78 03E00008 */  jr      $ra                        
/* 01F4C 80A44C7C 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
.L80A44C80:
/* 01F50 80A44C80 90430000 */  lbu     $v1, 0x0000($v0)           ## 00000000
.L80A44C84:
/* 01F54 80A44C84 908E020E */  lbu     $t6, 0x020E($a0)           ## 0000020E
/* 01F58 80A44C88 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
/* 01F5C 80A44C8C 2463FFFF */  addiu   $v1, $v1, 0xFFFF           ## $v1 = FFFFFFFF
/* 01F60 80A44C90 11C0000A */  beq     $t6, $zero, .L80A44CBC     
/* 01F64 80A44C94 306300FF */  andi    $v1, $v1, 0x00FF           ## $v1 = 000000FF
/* 01F68 80A44C98 808F0210 */  lb      $t7, 0x0210($a0)           ## 00000210
/* 01F6C 80A44C9C 2468FFFF */  addiu   $t0, $v1, 0xFFFF           ## $t0 = 000000FE
/* 01F70 80A44CA0 25F8FFFF */  addiu   $t8, $t7, 0xFFFF           ## $t8 = FFFFFFFF
/* 01F74 80A44CA4 A0980210 */  sb      $t8, 0x0210($a0)           ## 00000210
/* 01F78 80A44CA8 80990210 */  lb      $t9, 0x0210($a0)           ## 00000210
/* 01F7C 80A44CAC 0721000B */  bgez    $t9, .L80A44CDC            
/* 01F80 80A44CB0 00000000 */  nop
/* 01F84 80A44CB4 10000009 */  beq     $zero, $zero, .L80A44CDC   
/* 01F88 80A44CB8 A0880210 */  sb      $t0, 0x0210($a0)           ## 00000210
.L80A44CBC:
/* 01F8C 80A44CBC 80890210 */  lb      $t1, 0x0210($a0)           ## 00000210
/* 01F90 80A44CC0 252A0001 */  addiu   $t2, $t1, 0x0001           ## $t2 = 00000001
/* 01F94 80A44CC4 A08A0210 */  sb      $t2, 0x0210($a0)           ## 00000210
/* 01F98 80A44CC8 808B0210 */  lb      $t3, 0x0210($a0)           ## 00000210
/* 01F9C 80A44CCC 0163082A */  slt     $at, $t3, $v1              
/* 01FA0 80A44CD0 14200002 */  bne     $at, $zero, .L80A44CDC     
/* 01FA4 80A44CD4 00000000 */  nop
/* 01FA8 80A44CD8 A0800210 */  sb      $zero, 0x0210($a0)         ## 00000210
.L80A44CDC:
/* 01FAC 80A44CDC 03E00008 */  jr      $ra                        
/* 01FB0 80A44CE0 00000000 */  nop