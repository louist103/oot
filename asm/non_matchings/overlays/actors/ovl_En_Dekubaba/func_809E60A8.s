.late_rodata
glabel D_809E9188
    .float 0.01

.text
glabel func_809E60A8
/* 008D8 809E60A8 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 008DC 809E60AC AFB00020 */  sw      $s0, 0x0020($sp)
/* 008E0 809E60B0 AFA5002C */  sw      $a1, 0x002C($sp)
/* 008E4 809E60B4 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 008E8 809E60B8 AFBF0024 */  sw      $ra, 0x0024($sp)
/* 008EC 809E60BC 3C050600 */  lui     $a1, 0x0600                ## $a1 = 06000000
/* 008F0 809E60C0 24A50208 */  addiu   $a1, $a1, 0x0208           ## $a1 = 06000208
/* 008F4 809E60C4 2484017C */  addiu   $a0, $a0, 0x017C           ## $a0 = 0000017C
/* 008F8 809E60C8 0C029490 */  jal     SkelAnime_ChangeAnimTransitionStop
/* 008FC 809E60CC 3C06C0A0 */  lui     $a2, 0xC0A0                ## $a2 = C0A00000
/* 00900 809E60D0 920F0249 */  lbu     $t7, 0x0249($s0)           ## 00000249
/* 00904 809E60D4 8FAE002C */  lw      $t6, 0x002C($sp)
/* 00908 809E60D8 3C01809F */  lui     $at, %hi(D_809E9188)       ## $at = 809F0000
/* 0090C 809E60DC 31F8FFFE */  andi    $t8, $t7, 0xFFFE           ## $t8 = 00000000
/* 00910 809E60E0 A2180249 */  sb      $t8, 0x0249($s0)           ## 00000249
/* 00914 809E60E4 A60E01C6 */  sh      $t6, 0x01C6($s0)           ## 000001C6
/* 00918 809E60E8 C4269188 */  lwc1    $f6, %lo(D_809E9188)($at)
/* 0091C 809E60EC C6040230 */  lwc1    $f4, 0x0230($s0)           ## 00000230
/* 00920 809E60F0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00924 809E60F4 46062202 */  mul.s   $f8, $f4, $f6
/* 00928 809E60F8 44054000 */  mfc1    $a1, $f8
/* 0092C 809E60FC 0C00B58B */  jal     Actor_SetScale

/* 00930 809E6100 00000000 */  nop
/* 00934 809E6104 8FB9002C */  lw      $t9, 0x002C($sp)
/* 00938 809E6108 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 0093C 809E610C 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00940 809E6110 1721000A */  bne     $t9, $at, .L809E613C
/* 00944 809E6114 24054000 */  addiu   $a1, $zero, 0x4000         ## $a1 = 00004000
/* 00948 809E6118 2408003E */  addiu   $t0, $zero, 0x003E         ## $t0 = 0000003E
/* 0094C 809E611C AFA80010 */  sw      $t0, 0x0010($sp)
/* 00950 809E6120 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00954 809E6124 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 00958 809E6128 2406009B */  addiu   $a2, $zero, 0x009B         ## $a2 = 0000009B
/* 0095C 809E612C 0C00D09B */  jal     func_8003426C
/* 00960 809E6130 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 00964 809E6134 10000006 */  beq     $zero, $zero, .L809E6150
/* 00968 809E6138 00000000 */  nop
.L809E613C:
/* 0096C 809E613C 2409002A */  addiu   $t1, $zero, 0x002A         ## $t1 = 0000002A
/* 00970 809E6140 AFA90010 */  sw      $t1, 0x0010($sp)
/* 00974 809E6144 240600FF */  addiu   $a2, $zero, 0x00FF         ## $a2 = 000000FF
/* 00978 809E6148 0C00D09B */  jal     func_8003426C
/* 0097C 809E614C 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
.L809E6150:
/* 00980 809E6150 3C0A809E */  lui     $t2, %hi(func_809E78DC)    ## $t2 = 809E0000
/* 00984 809E6154 254A78DC */  addiu   $t2, $t2, %lo(func_809E78DC) ## $t2 = 809E78DC
/* 00988 809E6158 AE0A01C0 */  sw      $t2, 0x01C0($s0)           ## 000001C0
/* 0098C 809E615C 8FBF0024 */  lw      $ra, 0x0024($sp)
/* 00990 809E6160 8FB00020 */  lw      $s0, 0x0020($sp)
/* 00994 809E6164 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 00998 809E6168 03E00008 */  jr      $ra
/* 0099C 809E616C 00000000 */  nop
