glabel func_80A78FB0
/* 00000 80A78FB0 3C028016 */  lui     $v0, %hi(gSaveContext)
/* 00004 80A78FB4 2442E660 */  addiu   $v0, %lo(gSaveContext)
/* 00008 80A78FB8 944E0ED6 */  lhu     $t6, 0x0ED6($v0)           ## 8015F536
/* 0000C 80A78FBC AFA40000 */  sw      $a0, 0x0000($sp)           
/* 00010 80A78FC0 31CF0010 */  andi    $t7, $t6, 0x0010           ## $t7 = 00000000
/* 00014 80A78FC4 51E0000A */  beql    $t7, $zero, .L80A78FF0     
/* 00018 80A78FC8 94480F0A */  lhu     $t0, 0x0F0A($v0)           ## 8015F56A
/* 0001C 80A78FCC 94580F0A */  lhu     $t8, 0x0F0A($v0)           ## 8015F56A
/* 00020 80A78FD0 33190080 */  andi    $t9, $t8, 0x0080           ## $t9 = 00000000
/* 00024 80A78FD4 13200003 */  beq     $t9, $zero, .L80A78FE4     
/* 00028 80A78FD8 00000000 */  nop
/* 0002C 80A78FDC 03E00008 */  jr      $ra                        
/* 00030 80A78FE0 24022046 */  addiu   $v0, $zero, 0x2046         ## $v0 = 00002046
.L80A78FE4:
/* 00034 80A78FE4 03E00008 */  jr      $ra                        
/* 00038 80A78FE8 24022045 */  addiu   $v0, $zero, 0x2045         ## $v0 = 00002045
.L80A78FEC:
/* 0003C 80A78FEC 94480F0A */  lhu     $t0, 0x0F0A($v0)           ## 00002F4F
.L80A78FF0:
/* 00040 80A78FF0 2402203F */  addiu   $v0, $zero, 0x203F         ## $v0 = 0000203F
/* 00044 80A78FF4 31090010 */  andi    $t1, $t0, 0x0010           ## $t1 = 00000000
/* 00048 80A78FF8 11200003 */  beq     $t1, $zero, .L80A79008     
/* 0004C 80A78FFC 00000000 */  nop
/* 00050 80A79000 03E00008 */  jr      $ra                        
/* 00054 80A79004 24022040 */  addiu   $v0, $zero, 0x2040         ## $v0 = 00002040
.L80A79008:
/* 00058 80A79008 03E00008 */  jr      $ra                        
/* 0005C 80A7900C 00000000 */  nop