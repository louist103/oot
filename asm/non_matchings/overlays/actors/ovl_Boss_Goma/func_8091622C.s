glabel func_8091622C
/* 0081C 8091622C 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 00820 80916230 27BDFFD0 */  addiu   $sp, $sp, 0xFFD0           ## $sp = FFFFFFD0
/* 00824 80916234 3C01C0A0 */  lui     $at, 0xC0A0                ## $at = C0A00000
/* 00828 80916238 44812000 */  mtc1    $at, $f4                   ## $f4 = -5.00
/* 0082C 8091623C AFB00028 */  sw      $s0, 0x0028($sp)
/* 00830 80916240 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 00834 80916244 AFBF002C */  sw      $ra, 0x002C($sp)
/* 00838 80916248 3C050600 */  lui     $a1, %hi(D_060058C0)                ## $a1 = 06000000
/* 0083C 8091624C 240E0002 */  addiu   $t6, $zero, 0x0002         ## $t6 = 00000002
/* 00840 80916250 44070000 */  mfc1    $a3, $f0
/* 00844 80916254 AFAE0014 */  sw      $t6, 0x0014($sp)
/* 00848 80916258 24A558C0 */  addiu   $a1, $a1, %lo(D_060058C0)           ## $a1 = 060058C0
/* 0084C 8091625C 2484014C */  addiu   $a0, $a0, 0x014C           ## $a0 = 0000014C
/* 00850 80916260 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 00854 80916264 E7A00010 */  swc1    $f0, 0x0010($sp)
/* 00858 80916268 0C029468 */  jal     Animation_Change

/* 0085C 8091626C E7A40018 */  swc1    $f4, 0x0018($sp)
/* 00860 80916270 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 00864 80916274 3C01C000 */  lui     $at, 0xC000                ## $at = C0000000
/* 00868 80916278 44813000 */  mtc1    $at, $f6                   ## $f6 = -2.00
/* 0086C 8091627C 3C0F8092 */  lui     $t7, %hi(func_8091932C)    ## $t7 = 80920000
/* 00870 80916280 25EF932C */  addiu   $t7, $t7, %lo(func_8091932C) ## $t7 = 8091932C
/* 00874 80916284 AE0F0190 */  sw      $t7, 0x0190($s0)           ## 00000190
/* 00878 80916288 E6000068 */  swc1    $f0, 0x0068($s0)           ## 00000068
/* 0087C 8091628C E6000060 */  swc1    $f0, 0x0060($s0)           ## 00000060
/* 00880 80916290 E606006C */  swc1    $f6, 0x006C($s0)           ## 0000006C
/* 00884 80916294 8FBF002C */  lw      $ra, 0x002C($sp)
/* 00888 80916298 8FB00028 */  lw      $s0, 0x0028($sp)
/* 0088C 8091629C 27BD0030 */  addiu   $sp, $sp, 0x0030           ## $sp = 00000000
/* 00890 809162A0 03E00008 */  jr      $ra
/* 00894 809162A4 00000000 */  nop
