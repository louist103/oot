.late_rodata
glabel D_8095C75C
    .float 0.1

.text
glabel func_80954168
/* 04EA8 80954168 27BDFFC0 */  addiu   $sp, $sp, 0xFFC0           ## $sp = FFFFFFC0
/* 04EAC 8095416C AFBF0034 */  sw      $ra, 0x0034($sp)
/* 04EB0 80954170 AFB00030 */  sw      $s0, 0x0030($sp)
/* 04EB4 80954174 AFA50044 */  sw      $a1, 0x0044($sp)
/* 04EB8 80954178 8C8E0198 */  lw      $t6, 0x0198($a0)           ## 00000198
/* 04EBC 8095417C 3C188096 */  lui     $t8, %hi(D_809668D0)       ## $t8 = 80960000
/* 04EC0 80954180 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 04EC4 80954184 25CF0001 */  addiu   $t7, $t6, 0x0001           ## $t7 = 00000001
/* 04EC8 80954188 AC8F0198 */  sw      $t7, 0x0198($a0)           ## 00000198
/* 04ECC 8095418C 931868D0 */  lbu     $t8, %lo(D_809668D0)($t8)
/* 04ED0 80954190 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 04ED4 80954194 240600FF */  addiu   $a2, $zero, 0x00FF         ## $a2 = 000000FF
/* 04ED8 80954198 3319007F */  andi    $t9, $t8, 0x007F           ## $t9 = 00000000
/* 04EDC 8095419C 1320002E */  beq     $t9, $zero, .L80954258
/* 04EE0 809541A0 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 04EE4 809541A4 2408000C */  addiu   $t0, $zero, 0x000C         ## $t0 = 0000000C
/* 04EE8 809541A8 0C00D09B */  jal     func_8003426C
/* 04EEC 809541AC AFA80010 */  sw      $t0, 0x0010($sp)
/* 04EF0 809541B0 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 04EF4 809541B4 00000000 */  nop
/* 04EF8 809541B8 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 04EFC 809541BC 44812000 */  mtc1    $at, $f4                   ## $f4 = 0.50
/* 04F00 809541C0 3C040601 */  lui     $a0, 0x0601                ## $a0 = 06010000
/* 04F04 809541C4 4600203C */  c.lt.s  $f4, $f0
/* 04F08 809541C8 00000000 */  nop
/* 04F0C 809541CC 45000013 */  bc1f    .L8095421C
/* 04F10 809541D0 00000000 */  nop
/* 04F14 809541D4 3C040601 */  lui     $a0, %hi(D_060162AC)                ## $a0 = 06010000
/* 04F18 809541D8 0C028800 */  jal     SkelAnime_GetFrameCount

/* 04F1C 809541DC 248462AC */  addiu   $a0, $a0, %lo(D_060162AC)           ## $a0 = 060162AC
/* 04F20 809541E0 44823000 */  mtc1    $v0, $f6                   ## $f6 = 0.00
/* 04F24 809541E4 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 04F28 809541E8 3C050601 */  lui     $a1, %hi(D_060162AC)                ## $a1 = 06010000
/* 04F2C 809541EC 46803220 */  cvt.s.w $f8, $f6
/* 04F30 809541F0 24090002 */  addiu   $t1, $zero, 0x0002         ## $t1 = 00000002
/* 04F34 809541F4 44070000 */  mfc1    $a3, $f0
/* 04F38 809541F8 AFA90014 */  sw      $t1, 0x0014($sp)
/* 04F3C 809541FC 24A562AC */  addiu   $a1, $a1, %lo(D_060162AC)           ## $a1 = 060162AC
/* 04F40 80954200 2604014C */  addiu   $a0, $s0, 0x014C           ## $a0 = 0000014C
/* 04F44 80954204 E7A80010 */  swc1    $f8, 0x0010($sp)
/* 04F48 80954208 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 04F4C 8095420C 0C029468 */  jal     SkelAnime_ChangeAnim

/* 04F50 80954210 E7A00018 */  swc1    $f0, 0x0018($sp)
/* 04F54 80954214 10000011 */  beq     $zero, $zero, .L8095425C
/* 04F58 80954218 2604014C */  addiu   $a0, $s0, 0x014C           ## $a0 = 0000014C
.L8095421C:
/* 04F5C 8095421C 0C028800 */  jal     SkelAnime_GetFrameCount

/* 04F60 80954220 248464B0 */  addiu   $a0, $a0, 0x64B0           ## $a0 = 000065FC
/* 04F64 80954224 44825000 */  mtc1    $v0, $f10                  ## $f10 = 0.00
/* 04F68 80954228 44809000 */  mtc1    $zero, $f18                ## $f18 = 0.00
/* 04F6C 8095422C 3C050601 */  lui     $a1, %hi(D_060164B0)                ## $a1 = 06010000
/* 04F70 80954230 46805420 */  cvt.s.w $f16, $f10
/* 04F74 80954234 240A0002 */  addiu   $t2, $zero, 0x0002         ## $t2 = 00000002
/* 04F78 80954238 AFAA0014 */  sw      $t2, 0x0014($sp)
/* 04F7C 8095423C 24A564B0 */  addiu   $a1, $a1, %lo(D_060164B0)           ## $a1 = 060164B0
/* 04F80 80954240 2604014C */  addiu   $a0, $s0, 0x014C           ## $a0 = 0000014C
/* 04F84 80954244 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 04F88 80954248 E7B00010 */  swc1    $f16, 0x0010($sp)
/* 04F8C 8095424C 24070000 */  addiu   $a3, $zero, 0x0000         ## $a3 = 00000000
/* 04F90 80954250 0C029468 */  jal     SkelAnime_ChangeAnim

/* 04F94 80954254 E7B20018 */  swc1    $f18, 0x0018($sp)
.L80954258:
/* 04F98 80954258 2604014C */  addiu   $a0, $s0, 0x014C           ## $a0 = 0000014C
.L8095425C:
/* 04F9C 8095425C 0C02927F */  jal     SkelAnime_FrameUpdateMatrix

/* 04FA0 80954260 AFA40038 */  sw      $a0, 0x0038($sp)
/* 04FA4 80954264 1040000F */  beq     $v0, $zero, .L809542A4
/* 04FA8 80954268 3C040601 */  lui     $a0, %hi(D_060166A8)                ## $a0 = 06010000
/* 04FAC 8095426C 0C028800 */  jal     SkelAnime_GetFrameCount

/* 04FB0 80954270 248466A8 */  addiu   $a0, $a0, %lo(D_060166A8)           ## $a0 = 060166A8
/* 04FB4 80954274 44822000 */  mtc1    $v0, $f4                   ## $f4 = 0.00
/* 04FB8 80954278 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 04FBC 8095427C 3C050601 */  lui     $a1, %hi(D_060166A8)                ## $a1 = 06010000
/* 04FC0 80954280 468021A0 */  cvt.s.w $f6, $f4
/* 04FC4 80954284 44070000 */  mfc1    $a3, $f0
/* 04FC8 80954288 24A566A8 */  addiu   $a1, $a1, %lo(D_060166A8)           ## $a1 = 060166A8
/* 04FCC 8095428C 8FA40038 */  lw      $a0, 0x0038($sp)
/* 04FD0 80954290 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 04FD4 80954294 AFA00014 */  sw      $zero, 0x0014($sp)
/* 04FD8 80954298 E7A60010 */  swc1    $f6, 0x0010($sp)
/* 04FDC 8095429C 0C029468 */  jal     SkelAnime_ChangeAnim

/* 04FE0 809542A0 E7A00018 */  swc1    $f0, 0x0018($sp)
.L809542A4:
/* 04FE4 809542A4 0C253CB2 */  jal     func_8094F2C8
/* 04FE8 809542A8 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 04FEC 809542AC 0C03F66B */  jal     Math_Rand_ZeroOne
              ## Rand.Next() float
/* 04FF0 809542B0 00000000 */  nop
/* 04FF4 809542B4 3C018096 */  lui     $at, %hi(D_8095C75C)       ## $at = 80960000
/* 04FF8 809542B8 C428C75C */  lwc1    $f8, %lo(D_8095C75C)($at)
/* 04FFC 809542BC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 05000 809542C0 4608003C */  c.lt.s  $f0, $f8
/* 05004 809542C4 00000000 */  nop
/* 05008 809542C8 45020004 */  bc1fl   .L809542DC
/* 0500C 809542CC 920B02E9 */  lbu     $t3, 0x02E9($s0)           ## 000002E9
/* 05010 809542D0 0C00BE0A */  jal     Audio_PlayActorSound2

/* 05014 809542D4 24053143 */  addiu   $a1, $zero, 0x3143         ## $a1 = 00003143
/* 05018 809542D8 920B02E9 */  lbu     $t3, 0x02E9($s0)           ## 000002E9
.L809542DC:
/* 0501C 809542DC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 05020 809542E0 316C0002 */  andi    $t4, $t3, 0x0002           ## $t4 = 00000000
/* 05024 809542E4 51800006 */  beql    $t4, $zero, .L80954300
/* 05028 809542E8 920D0114 */  lbu     $t5, 0x0114($s0)           ## 00000114
/* 0502C 809542EC 0C2550E3 */  jal     func_8095438C
/* 05030 809542F0 8FA50044 */  lw      $a1, 0x0044($sp)
/* 05034 809542F4 10000021 */  beq     $zero, $zero, .L8095437C
/* 05038 809542F8 8FBF0034 */  lw      $ra, 0x0034($sp)
/* 0503C 809542FC 920D0114 */  lbu     $t5, 0x0114($s0)           ## 00000114
.L80954300:
/* 05040 80954300 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 05044 80954304 8FA40044 */  lw      $a0, 0x0044($sp)
/* 05048 80954308 15A00004 */  bne     $t5, $zero, .L8095431C
/* 0504C 8095430C 34211E60 */  ori     $at, $at, 0x1E60           ## $at = 00011E60
/* 05050 80954310 00812821 */  addu    $a1, $a0, $at
/* 05054 80954314 0C01767D */  jal     CollisionCheck_SetAC
              ## CollisionCheck_setAC
/* 05058 80954318 260602D8 */  addiu   $a2, $s0, 0x02D8           ## $a2 = 000002D8
.L8095431C:
/* 0505C 8095431C 8E020198 */  lw      $v0, 0x0198($s0)           ## 00000198
/* 05060 80954320 8FA40044 */  lw      $a0, 0x0044($sp)
/* 05064 80954324 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 05068 80954328 304E0001 */  andi    $t6, $v0, 0x0001           ## $t6 = 00000000
/* 0506C 8095432C 15C00012 */  bne     $t6, $zero, .L80954378
/* 05070 80954330 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 05074 80954334 30580020 */  andi    $t8, $v0, 0x0020           ## $t8 = 00000000
/* 05078 80954338 0018C943 */  sra     $t9, $t8,  5
/* 0507C 8095433C 27280001 */  addiu   $t0, $t9, 0x0001           ## $t0 = 00000001
/* 05080 80954340 44889000 */  mtc1    $t0, $f18                  ## $f18 = 0.00
/* 05084 80954344 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 05088 80954348 44815000 */  mtc1    $at, $f10                  ## $f10 = 5.00
/* 0508C 8095434C 46809120 */  cvt.s.w $f4, $f18
/* 05090 80954350 44808000 */  mtc1    $zero, $f16                ## $f16 = 0.00
/* 05094 80954354 240F0005 */  addiu   $t7, $zero, 0x0005         ## $t7 = 00000005
/* 05098 80954358 24090001 */  addiu   $t1, $zero, 0x0001         ## $t1 = 00000001
/* 0509C 8095435C AFA90020 */  sw      $t1, 0x0020($sp)
/* 050A0 80954360 AFAF0018 */  sw      $t7, 0x0018($sp)
/* 050A4 80954364 E7A4001C */  swc1    $f4, 0x001C($sp)
/* 050A8 80954368 24070064 */  addiu   $a3, $zero, 0x0064         ## $a3 = 00000064
/* 050AC 8095436C E7AA0010 */  swc1    $f10, 0x0010($sp)
/* 050B0 80954370 0C253E19 */  jal     func_8094F864
/* 050B4 80954374 E7B00014 */  swc1    $f16, 0x0014($sp)
.L80954378:
/* 050B8 80954378 8FBF0034 */  lw      $ra, 0x0034($sp)
.L8095437C:
/* 050BC 8095437C 8FB00030 */  lw      $s0, 0x0030($sp)
/* 050C0 80954380 27BD0040 */  addiu   $sp, $sp, 0x0040           ## $sp = 00000000
/* 050C4 80954384 03E00008 */  jr      $ra
/* 050C8 80954388 00000000 */  nop
