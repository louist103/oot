glabel func_80B3D4D8
/* 012F8 80B3D4D8 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 012FC 80B3D4DC 3C060601 */  lui     $a2, %hi(D_060177F8)                ## $a2 = 06010000
/* 01300 80B3D4E0 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 01304 80B3D4E4 24C677F8 */  addiu   $a2, $a2, %lo(D_060177F8)           ## $a2 = 060177F8
/* 01308 80B3D4E8 0C2CF134 */  jal     func_80B3C4D0              
/* 0130C 80B3D4EC AFA6001C */  sw      $a2, 0x001C($sp)           
/* 01310 80B3D4F0 10400019 */  beq     $v0, $zero, .L80B3D558     
/* 01314 80B3D4F4 8FA6001C */  lw      $a2, 0x001C($sp)           
/* 01318 80B3D4F8 94430000 */  lhu     $v1, 0x0000($v0)           ## 00000000
/* 0131C 80B3D4FC 2401000B */  addiu   $at, $zero, 0x000B         ## $at = 0000000B
/* 01320 80B3D500 54610005 */  bnel    $v1, $at, .L80B3D518       
/* 01324 80B3D504 2401000C */  addiu   $at, $zero, 0x000C         ## $at = 0000000C
/* 01328 80B3D508 3C060600 */  lui     $a2, %hi(D_060035C8)                ## $a2 = 06000000
/* 0132C 80B3D50C 10000012 */  beq     $zero, $zero, .L80B3D558   
/* 01330 80B3D510 24C635C8 */  addiu   $a2, $a2, %lo(D_060035C8)           ## $a2 = 060035C8
/* 01334 80B3D514 2401000C */  addiu   $at, $zero, 0x000C         ## $at = 0000000C
.L80B3D518:
/* 01338 80B3D518 54610005 */  bnel    $v1, $at, .L80B3D530       
/* 0133C 80B3D51C 2401000D */  addiu   $at, $zero, 0x000D         ## $at = 0000000D
/* 01340 80B3D520 3C060600 */  lui     $a2, %hi(D_0600289C)                ## $a2 = 06000000
/* 01344 80B3D524 1000000C */  beq     $zero, $zero, .L80B3D558   
/* 01348 80B3D528 24C6289C */  addiu   $a2, $a2, %lo(D_0600289C)           ## $a2 = 0600289C
/* 0134C 80B3D52C 2401000D */  addiu   $at, $zero, 0x000D         ## $at = 0000000D
.L80B3D530:
/* 01350 80B3D530 54610005 */  bnel    $v1, $at, .L80B3D548       
/* 01354 80B3D534 24010017 */  addiu   $at, $zero, 0x0017         ## $at = 00000017
/* 01358 80B3D538 3C060600 */  lui     $a2, %hi(D_06004570)                ## $a2 = 06000000
/* 0135C 80B3D53C 10000006 */  beq     $zero, $zero, .L80B3D558   
/* 01360 80B3D540 24C64570 */  addiu   $a2, $a2, %lo(D_06004570)           ## $a2 = 06004570
/* 01364 80B3D544 24010017 */  addiu   $at, $zero, 0x0017         ## $at = 00000017
.L80B3D548:
/* 01368 80B3D548 54610004 */  bnel    $v1, $at, .L80B3D55C       
/* 0136C 80B3D54C 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 01370 80B3D550 3C060600 */  lui     $a2, %hi(D_06000B6C)                ## $a2 = 06000000
/* 01374 80B3D554 24C60B6C */  addiu   $a2, $a2, %lo(D_06000B6C)           ## $a2 = 06000B6C
.L80B3D558:
/* 01378 80B3D558 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L80B3D55C:
/* 0137C 80B3D55C 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 01380 80B3D560 00C01025 */  or      $v0, $a2, $zero            ## $v0 = 06000B6C
/* 01384 80B3D564 03E00008 */  jr      $ra                        
/* 01388 80B3D568 00000000 */  nop
