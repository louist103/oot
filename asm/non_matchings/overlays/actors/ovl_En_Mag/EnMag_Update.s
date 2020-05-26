.late_rodata
glabel D_80AA5F30
    .float 0.8
glabel D_80AA5F34
    .float 2.4

.text
glabel EnMag_Update
/* 002A0 80AA3DA0 3C0E8016 */  lui     $t6, %hi(gSaveContext+0x1354)
/* 002A4 80AA3DA4 8DCEF9B4 */  lw      $t6, %lo(gSaveContext+0x1354)($t6)
/* 002A8 80AA3DA8 27BDFFC0 */  addiu   $sp, $sp, 0xFFC0           ## $sp = FFFFFFC0
/* 002AC 80AA3DAC AFB00020 */  sw      $s0, 0x0020($sp)           
/* 002B0 80AA3DB0 3401FEDC */  ori     $at, $zero, 0xFEDC         ## $at = 0000FEDC
/* 002B4 80AA3DB4 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 002B8 80AA3DB8 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 002BC 80AA3DBC 11C1008B */  beq     $t6, $at, .L80AA3FEC       
/* 002C0 80AA3DC0 AFA50044 */  sw      $a1, 0x0044($sp)           
/* 002C4 80AA3DC4 34018000 */  ori     $at, $zero, 0x8000         ## $at = 00008000
/* 002C8 80AA3DC8 00811021 */  addu    $v0, $a0, $at              
/* 002CC 80AA3DCC 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
/* 002D0 80AA3DD0 28610002 */  slti    $at, $v1, 0x0002           
/* 002D4 80AA3DD4 50200049 */  beql    $at, $zero, .L80AA3EFC     
/* 002D8 80AA3DD8 28610002 */  slti    $at, $v1, 0x0002           
/* 002DC 80AA3DDC 94A30020 */  lhu     $v1, 0x0020($a1)           ## 00000020
/* 002E0 80AA3DE0 2401EFFF */  addiu   $at, $zero, 0xEFFF         ## $at = FFFFEFFF
/* 002E4 80AA3DE4 3C078013 */  lui     $a3, %hi(D_801333E0)
/* 002E8 80AA3DE8 0061C027 */  nor     $t8, $v1, $at              
/* 002EC 80AA3DEC 13000007 */  beq     $t8, $zero, .L80AA3E0C     
/* 002F0 80AA3DF0 3C01FFFF */  lui     $at, 0xFFFF                ## $at = FFFF0000
/* 002F4 80AA3DF4 34217FFF */  ori     $at, $at, 0x7FFF           ## $at = FFFF7FFF
/* 002F8 80AA3DF8 0061C827 */  nor     $t9, $v1, $at              
/* 002FC 80AA3DFC 13200003 */  beq     $t9, $zero, .L80AA3E0C     
/* 00300 80AA3E00 2401BFFF */  addiu   $at, $zero, 0xBFFF         ## $at = FFFFBFFF
/* 00304 80AA3E04 00614027 */  nor     $t0, $v1, $at              
/* 00308 80AA3E08 15000039 */  bne     $t0, $zero, .L80AA3EF0     
.L80AA3E0C:
/* 0030C 80AA3E0C 3C098013 */  lui     $t1, %hi(D_801333E8)
/* 00310 80AA3E10 24E733E0 */  addiu   $a3, %lo(D_801333E0)
/* 00314 80AA3E14 252933E8 */  addiu   $t1, %lo(D_801333E8)
/* 00318 80AA3E18 3C058013 */  lui     $a1, %hi(D_801333D4)
/* 0031C 80AA3E1C 24A533D4 */  addiu   $a1, %lo(D_801333D4)
/* 00320 80AA3E20 AFA90014 */  sw      $t1, 0x0014($sp)           
/* 00324 80AA3E24 AFA70010 */  sw      $a3, 0x0010($sp)           
/* 00328 80AA3E28 24044823 */  addiu   $a0, $zero, 0x4823         ## $a0 = 00004823
/* 0032C 80AA3E2C 24060004 */  addiu   $a2, $zero, 0x0004         ## $a2 = 00000004
/* 00330 80AA3E30 0C03DCE3 */  jal     Audio_PlaySoundGeneral
              
/* 00334 80AA3E34 AFA2002C */  sw      $v0, 0x002C($sp)           
/* 00338 80AA3E38 3C01437F */  lui     $at, 0x437F                ## $at = 437F0000
/* 0033C 80AA3E3C 44810000 */  mtc1    $at, $f0                   ## $f0 = 255.00
/* 00340 80AA3E40 44811000 */  mtc1    $at, $f2                   ## $f2 = 255.00
/* 00344 80AA3E44 3C014352 */  lui     $at, 0x4352                ## $at = 43520000
/* 00348 80AA3E48 44812000 */  mtc1    $at, $f4                   ## $f4 = 210.00
/* 0034C 80AA3E4C 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00350 80AA3E50 8FA2002C */  lw      $v0, 0x002C($sp)           
/* 00354 80AA3E54 00300821 */  addu    $at, $at, $s0              
/* 00358 80AA3E58 E424E300 */  swc1    $f4, -0x1D00($at)          ## 0000E300
/* 0035C 80AA3E5C 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00360 80AA3E60 00300821 */  addu    $at, $at, $s0              
/* 00364 80AA3E64 E420E304 */  swc1    $f0, -0x1CFC($at)          ## 0000E304
/* 00368 80AA3E68 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 0036C 80AA3E6C 00300821 */  addu    $at, $at, $s0              
/* 00370 80AA3E70 E420E308 */  swc1    $f0, -0x1CF8($at)          ## 0000E308
/* 00374 80AA3E74 3C014300 */  lui     $at, 0x4300                ## $at = 43000000
/* 00378 80AA3E78 44813000 */  mtc1    $at, $f6                   ## $f6 = 128.00
/* 0037C 80AA3E7C 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00380 80AA3E80 00300821 */  addu    $at, $at, $s0              
/* 00384 80AA3E84 E426E2E0 */  swc1    $f6, -0x1D20($at)          ## 0000E2E0
/* 00388 80AA3E88 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 0038C 80AA3E8C 00300821 */  addu    $at, $at, $s0              
/* 00390 80AA3E90 E422E2FC */  swc1    $f2, -0x1D04($at)          ## 0000E2FC
/* 00394 80AA3E94 3C01432A */  lui     $at, 0x432A                ## $at = 432A0000
/* 00398 80AA3E98 44814000 */  mtc1    $at, $f8                   ## $f8 = 170.00
/* 0039C 80AA3E9C 3C014348 */  lui     $at, 0x4348                ## $at = 43480000
/* 003A0 80AA3EA0 44815000 */  mtc1    $at, $f10                  ## $f10 = 200.00
/* 003A4 80AA3EA4 44808000 */  mtc1    $zero, $f16                ## $f16 = 0.00
/* 003A8 80AA3EA8 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 003AC 80AA3EAC 240A0002 */  addiu   $t2, $zero, 0x0002         ## $t2 = 00000002
/* 003B0 80AA3EB0 E44262E8 */  swc1    $f2, 0x62E8($v0)           ## 000062E8
/* 003B4 80AA3EB4 E44262EC */  swc1    $f2, 0x62EC($v0)           ## 000062EC
/* 003B8 80AA3EB8 E44262F4 */  swc1    $f2, 0x62F4($v0)           ## 000062F4
/* 003BC 80AA3EBC E44862E4 */  swc1    $f8, 0x62E4($v0)           ## 000062E4
/* 003C0 80AA3EC0 E44A62F0 */  swc1    $f10, 0x62F0($v0)          ## 000062F0
/* 003C4 80AA3EC4 E45062F8 */  swc1    $f16, 0x62F8($v0)          ## 000062F8
/* 003C8 80AA3EC8 A42A62DD */  sh      $t2, 0x62DD($at)           ## 0000E2DC
/* 003CC 80AA3ECC 3C038016 */  lui     $v1, %hi(gSaveContext)
/* 003D0 80AA3ED0 3C0180AA */  lui     $at, %hi(D_80AA5EC0)       ## $at = 80AA0000
/* 003D4 80AA3ED4 240B0014 */  addiu   $t3, $zero, 0x0014         ## $t3 = 00000014
/* 003D8 80AA3ED8 2463E660 */  addiu   $v1, %lo(gSaveContext)
/* 003DC 80AA3EDC A42B5EC0 */  sh      $t3, %lo(D_80AA5EC0)($at)  
/* 003E0 80AA3EE0 240C0001 */  addiu   $t4, $zero, 0x0001         ## $t4 = 00000001
/* 003E4 80AA3EE4 240D00FF */  addiu   $t5, $zero, 0x00FF         ## $t5 = 000000FF
/* 003E8 80AA3EE8 A06C1418 */  sb      $t4, 0x1418($v1)           ## 8015FA78
/* 003EC 80AA3EEC A06D1419 */  sb      $t5, 0x1419($v1)           ## 8015FA79
.L80AA3EF0:
/* 003F0 80AA3EF0 1000003F */  beq     $zero, $zero, .L80AA3FF0   
/* 003F4 80AA3EF4 3C01437F */  lui     $at, 0x437F                ## $at = 437F0000
/* 003F8 80AA3EF8 28610002 */  slti    $at, $v1, 0x0002           
.L80AA3EFC:
/* 003FC 80AA3EFC 1420003B */  bne     $at, $zero, .L80AA3FEC     
/* 00400 80AA3F00 3C0280AA */  lui     $v0, %hi(D_80AA5EC0)       ## $v0 = 80AA0000
/* 00404 80AA3F04 84425EC0 */  lh      $v0, %lo(D_80AA5EC0)($v0)  
/* 00408 80AA3F08 8FAE0044 */  lw      $t6, 0x0044($sp)           
/* 0040C 80AA3F0C 3C0180AA */  lui     $at, %hi(D_80AA5EC0)       ## $at = 80AA0000
/* 00410 80AA3F10 14400035 */  bne     $v0, $zero, .L80AA3FE8     
/* 00414 80AA3F14 2449FFFF */  addiu   $t1, $v0, 0xFFFF           ## $t1 = 80A9FFFF
/* 00418 80AA3F18 95C30020 */  lhu     $v1, 0x0020($t6)           ## 00000020
/* 0041C 80AA3F1C 2401EFFF */  addiu   $at, $zero, 0xEFFF         ## $at = FFFFEFFF
/* 00420 80AA3F20 8FA80044 */  lw      $t0, 0x0044($sp)           
/* 00424 80AA3F24 00617827 */  nor     $t7, $v1, $at              
/* 00428 80AA3F28 11E00007 */  beq     $t7, $zero, .L80AA3F48     
/* 0042C 80AA3F2C 3C01FFFF */  lui     $at, 0xFFFF                ## $at = FFFF0000
/* 00430 80AA3F30 34217FFF */  ori     $at, $at, 0x7FFF           ## $at = FFFF7FFF
/* 00434 80AA3F34 0061C027 */  nor     $t8, $v1, $at              
/* 00438 80AA3F38 13000003 */  beq     $t8, $zero, .L80AA3F48     
/* 0043C 80AA3F3C 2401BFFF */  addiu   $at, $zero, 0xBFFF         ## $at = FFFFBFFF
/* 00440 80AA3F40 0061C827 */  nor     $t9, $v1, $at              
/* 00444 80AA3F44 17200029 */  bne     $t9, $zero, .L80AA3FEC     
.L80AA3F48:
/* 00448 80AA3F48 3C090001 */  lui     $t1, 0x0001                ## $t1 = 00010000
/* 0044C 80AA3F4C 01284821 */  addu    $t1, $t1, $t0              
/* 00450 80AA3F50 81291E15 */  lb      $t1, 0x1E15($t1)           ## 00011E15
/* 00454 80AA3F54 24010014 */  addiu   $at, $zero, 0x0014         ## $at = 00000014
/* 00458 80AA3F58 5121001C */  beql    $t1, $at, .L80AA3FCC       
/* 0045C 80AA3F5C 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 00460 80AA3F60 0C03DA2F */  jal     func_800F68BC              
/* 00464 80AA3F64 00002025 */  or      $a0, $zero, $zero          ## $a0 = 00000000
/* 00468 80AA3F68 3C078013 */  lui     $a3, %hi(D_801333E0)
/* 0046C 80AA3F6C 3C0A8013 */  lui     $t2, %hi(D_801333E8)
/* 00470 80AA3F70 24E733E0 */  addiu   $a3, %lo(D_801333E0)
/* 00474 80AA3F74 254A33E8 */  addiu   $t2, %lo(D_801333E8)
/* 00478 80AA3F78 3C058013 */  lui     $a1, %hi(D_801333D4)
/* 0047C 80AA3F7C 24A533D4 */  addiu   $a1, %lo(D_801333D4)
/* 00480 80AA3F80 AFAA0014 */  sw      $t2, 0x0014($sp)           
/* 00484 80AA3F84 AFA70010 */  sw      $a3, 0x0010($sp)           
/* 00488 80AA3F88 24044823 */  addiu   $a0, $zero, 0x4823         ## $a0 = 00004823
/* 0048C 80AA3F8C 0C03DCE3 */  jal     Audio_PlaySoundGeneral
              
/* 00490 80AA3F90 24060004 */  addiu   $a2, $zero, 0x0004         ## $a2 = 00000004
/* 00494 80AA3F94 8FAD0044 */  lw      $t5, 0x0044($sp)           
/* 00498 80AA3F98 240B0002 */  addiu   $t3, $zero, 0x0002         ## $t3 = 00000002
/* 0049C 80AA3F9C 3C018016 */  lui     $at, %hi(gSaveContext+0x135c)
/* 004A0 80AA3FA0 AC2BF9BC */  sw      $t3, %lo(gSaveContext+0x135c)($at)
/* 004A4 80AA3FA4 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 004A8 80AA3FA8 240C0014 */  addiu   $t4, $zero, 0x0014         ## $t4 = 00000014
/* 004AC 80AA3FAC 002D0821 */  addu    $at, $at, $t5              
/* 004B0 80AA3FB0 A02C1E15 */  sb      $t4, 0x1E15($at)           ## 00011E15
/* 004B4 80AA3FB4 8FAF0044 */  lw      $t7, 0x0044($sp)           
/* 004B8 80AA3FB8 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 004BC 80AA3FBC 240E0002 */  addiu   $t6, $zero, 0x0002         ## $t6 = 00000002
/* 004C0 80AA3FC0 002F0821 */  addu    $at, $at, $t7              
/* 004C4 80AA3FC4 A02E1E5E */  sb      $t6, 0x1E5E($at)           ## 00011E5E
/* 004C8 80AA3FC8 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
.L80AA3FCC:
/* 004CC 80AA3FCC 2418000F */  addiu   $t8, $zero, 0x000F         ## $t8 = 0000000F
/* 004D0 80AA3FD0 24190019 */  addiu   $t9, $zero, 0x0019         ## $t9 = 00000019
/* 004D4 80AA3FD4 24080003 */  addiu   $t0, $zero, 0x0003         ## $t0 = 00000003
/* 004D8 80AA3FD8 A4386313 */  sh      $t8, 0x6313($at)           ## 0000E312
/* 004DC 80AA3FDC A4396315 */  sh      $t9, 0x6315($at)           ## 0000E314
/* 004E0 80AA3FE0 10000002 */  beq     $zero, $zero, .L80AA3FEC   
/* 004E4 80AA3FE4 A42862DD */  sh      $t0, 0x62DD($at)           ## 0000E2DC
.L80AA3FE8:
/* 004E8 80AA3FE8 A4295EC0 */  sh      $t1, %lo(D_80AA5EC0)($at)  
.L80AA3FEC:
/* 004EC 80AA3FEC 3C01437F */  lui     $at, 0x437F                ## $at = 437F0000
.L80AA3FF0:
/* 004F0 80AA3FF0 44811000 */  mtc1    $at, $f2                   ## $f2 = 255.00
/* 004F4 80AA3FF4 34018000 */  ori     $at, $zero, 0x8000         ## $at = 00008000
/* 004F8 80AA3FF8 02011021 */  addu    $v0, $s0, $at              
/* 004FC 80AA3FFC 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
/* 00500 80AA4000 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 00504 80AA4004 24010003 */  addiu   $at, $zero, 0x0003         ## $at = 00000003
/* 00508 80AA4008 14C300BC */  bne     $a2, $v1, .L80AA42FC       
/* 0050C 80AA400C 24050003 */  addiu   $a1, $zero, 0x0003         ## $a1 = 00000003
/* 00510 80AA4010 844362D8 */  lh      $v1, 0x62D8($v0)           ## 000062D8
/* 00514 80AA4014 3C0140CC */  lui     $at, 0x40CC                ## $at = 40CC0000
/* 00518 80AA4018 1460003A */  bne     $v1, $zero, .L80AA4104     
/* 0051C 80AA401C 00000000 */  nop
/* 00520 80AA4020 44810000 */  mtc1    $at, $f0                   ## $f0 = 6.38
/* 00524 80AA4024 C45262FC */  lwc1    $f18, 0x62FC($v0)          ## 000062FC
/* 00528 80AA4028 3C014078 */  lui     $at, 0x4078                ## $at = 40780000
/* 0052C 80AA402C 44816000 */  mtc1    $at, $f12                  ## $f12 = 3.88
/* 00530 80AA4030 46009100 */  add.s   $f4, $f18, $f0             
/* 00534 80AA4034 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00538 80AA4038 00300821 */  addu    $at, $at, $s0              
/* 0053C 80AA403C 240D0028 */  addiu   $t5, $zero, 0x0028         ## $t5 = 00000028
/* 00540 80AA4040 E424E2FC */  swc1    $f4, -0x1D04($at)          ## 0000E2FC
/* 00544 80AA4044 3C0180AA */  lui     $at, %hi(D_80AA5F30)       ## $at = 80AA0000
/* 00548 80AA4048 C4285F30 */  lwc1    $f8, %lo(D_80AA5F30)($at)  
/* 0054C 80AA404C C44662E0 */  lwc1    $f6, 0x62E0($v0)           ## 000062E0
/* 00550 80AA4050 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00554 80AA4054 00300821 */  addu    $at, $at, $s0              
/* 00558 80AA4058 46083280 */  add.s   $f10, $f6, $f8             
/* 0055C 80AA405C E42AE2E0 */  swc1    $f10, -0x1D20($at)         ## 0000E2E0
/* 00560 80AA4060 C45062E4 */  lwc1    $f16, 0x62E4($v0)          ## 000062E4
/* 00564 80AA4064 C44462E8 */  lwc1    $f4, 0x62E8($v0)           ## 000062E8
/* 00568 80AA4068 3C014008 */  lui     $at, 0x4008                ## $at = 40080000
/* 0056C 80AA406C 46008480 */  add.s   $f18, $f16, $f0            
/* 00570 80AA4070 44815000 */  mtc1    $at, $f10                  ## $f10 = 2.12
/* 00574 80AA4074 C44862EC */  lwc1    $f8, 0x62EC($v0)           ## 000062EC
/* 00578 80AA4078 460C2180 */  add.s   $f6, $f4, $f12             
/* 0057C 80AA407C E45262E4 */  swc1    $f18, 0x62E4($v0)          ## 000062E4
/* 00580 80AA4080 C45262F0 */  lwc1    $f18, 0x62F0($v0)          ## 000062F0
/* 00584 80AA4084 460A4400 */  add.s   $f16, $f8, $f10            
/* 00588 80AA4088 E44662E8 */  swc1    $f6, 0x62E8($v0)           ## 000062E8
/* 0058C 80AA408C C44662F4 */  lwc1    $f6, 0x62F4($v0)           ## 000062F4
/* 00590 80AA4090 46009100 */  add.s   $f4, $f18, $f0             
/* 00594 80AA4094 844A62DA */  lh      $t2, 0x62DA($v0)           ## 000062DA
/* 00598 80AA4098 E45062EC */  swc1    $f16, 0x62EC($v0)          ## 000062EC
/* 0059C 80AA409C 460C3200 */  add.s   $f8, $f6, $f12             
/* 005A0 80AA40A0 E44462F0 */  swc1    $f4, 0x62F0($v0)           ## 000062F0
/* 005A4 80AA40A4 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 005A8 80AA40A8 254BFFFF */  addiu   $t3, $t2, 0xFFFF           ## $t3 = FFFFFFFF
/* 005AC 80AA40AC E44862F4 */  swc1    $f8, 0x62F4($v0)           ## 000062F4
/* 005B0 80AA40B0 A42B62DB */  sh      $t3, 0x62DB($at)           ## 0000E2DA
/* 005B4 80AA40B4 844C62DA */  lh      $t4, 0x62DA($v0)           ## 000062DA
/* 005B8 80AA40B8 3C014200 */  lui     $at, 0x4200                ## $at = 42000000
/* 005BC 80AA40BC 5580003B */  bnel    $t4, $zero, .L80AA41AC     
/* 005C0 80AA40C0 3C014320 */  lui     $at, 0x4320                ## $at = 43200000
/* 005C4 80AA40C4 44815000 */  mtc1    $at, $f10                  ## $f10 = 160.00
/* 005C8 80AA40C8 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 005CC 80AA40CC 00300821 */  addu    $at, $at, $s0              
/* 005D0 80AA40D0 E42AE2E0 */  swc1    $f10, -0x1D20($at)         ## 0000E2E0
/* 005D4 80AA40D4 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 005D8 80AA40D8 00300821 */  addu    $at, $at, $s0              
/* 005DC 80AA40DC E422E2FC */  swc1    $f2, -0x1D04($at)          ## 0000E2FC
/* 005E0 80AA40E0 E44262E4 */  swc1    $f2, 0x62E4($v0)           ## 000062E4
/* 005E4 80AA40E4 E44262E8 */  swc1    $f2, 0x62E8($v0)           ## 000062E8
/* 005E8 80AA40E8 E44262EC */  swc1    $f2, 0x62EC($v0)           ## 000062EC
/* 005EC 80AA40EC E44262F0 */  swc1    $f2, 0x62F0($v0)           ## 000062F0
/* 005F0 80AA40F0 E44262F4 */  swc1    $f2, 0x62F4($v0)           ## 000062F4
/* 005F4 80AA40F4 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 005F8 80AA40F8 A42D62DB */  sh      $t5, 0x62DB($at)           ## 0000E2DA
/* 005FC 80AA40FC 1000002A */  beq     $zero, $zero, .L80AA41A8   
/* 00600 80AA4100 A42662D9 */  sh      $a2, 0x62D9($at)           ## 0000E2D8
.L80AA4104:
/* 00604 80AA4104 14C30028 */  bne     $a2, $v1, .L80AA41A8       
/* 00608 80AA4108 3C01C008 */  lui     $at, 0xC008                ## $at = C0080000
/* 0060C 80AA410C 44819000 */  mtc1    $at, $f18                  ## $f18 = -2.12
/* 00610 80AA4110 3C01BFB0 */  lui     $at, 0xBFB0                ## $at = BFB00000
/* 00614 80AA4114 C45062E4 */  lwc1    $f16, 0x62E4($v0)          ## 000062E4
/* 00618 80AA4118 44814000 */  mtc1    $at, $f8                   ## $f8 = -1.38
/* 0061C 80AA411C C44662F0 */  lwc1    $f6, 0x62F0($v0)           ## 000062F0
/* 00620 80AA4120 46128100 */  add.s   $f4, $f16, $f18            
/* 00624 80AA4124 3C0180AA */  lui     $at, %hi(D_80AA5F34)       ## $at = 80AA0000
/* 00628 80AA4128 C45062E0 */  lwc1    $f16, 0x62E0($v0)          ## 000062E0
/* 0062C 80AA412C 46083280 */  add.s   $f10, $f6, $f8             
/* 00630 80AA4130 E44462E4 */  swc1    $f4, 0x62E4($v0)           ## 000062E4
/* 00634 80AA4134 24190020 */  addiu   $t9, $zero, 0x0020         ## $t9 = 00000020
/* 00638 80AA4138 24080002 */  addiu   $t0, $zero, 0x0002         ## $t0 = 00000002
/* 0063C 80AA413C E44A62F0 */  swc1    $f10, 0x62F0($v0)          ## 000062F0
/* 00640 80AA4140 C4325F34 */  lwc1    $f18, %lo(D_80AA5F34)($at) 
/* 00644 80AA4144 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00648 80AA4148 00300821 */  addu    $at, $at, $s0              
/* 0064C 80AA414C 46128100 */  add.s   $f4, $f16, $f18            
/* 00650 80AA4150 E424E2E0 */  swc1    $f4, -0x1D20($at)          ## 0000E2E0
/* 00654 80AA4154 844E62DA */  lh      $t6, 0x62DA($v0)           ## 000062DA
/* 00658 80AA4158 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 0065C 80AA415C 25CFFFFF */  addiu   $t7, $t6, 0xFFFF           ## $t7 = 00000001
/* 00660 80AA4160 A42F62DB */  sh      $t7, 0x62DB($at)           ## 0000E2DA
/* 00664 80AA4164 845862DA */  lh      $t8, 0x62DA($v0)           ## 000062DA
/* 00668 80AA4168 3C014300 */  lui     $at, 0x4300                ## $at = 43000000
/* 0066C 80AA416C 5700000F */  bnel    $t8, $zero, .L80AA41AC     
/* 00670 80AA4170 3C014320 */  lui     $at, 0x4320                ## $at = 43200000
/* 00674 80AA4174 44813000 */  mtc1    $at, $f6                   ## $f6 = 160.00
/* 00678 80AA4178 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 0067C 80AA417C 00300821 */  addu    $at, $at, $s0              
/* 00680 80AA4180 E426E2E0 */  swc1    $f6, -0x1D20($at)          ## 0000E2E0
/* 00684 80AA4184 3C01432A */  lui     $at, 0x432A                ## $at = 432A0000
/* 00688 80AA4188 44814000 */  mtc1    $at, $f8                   ## $f8 = 170.00
/* 0068C 80AA418C 3C014348 */  lui     $at, 0x4348                ## $at = 43480000
/* 00690 80AA4190 44815000 */  mtc1    $at, $f10                  ## $f10 = 200.00
/* 00694 80AA4194 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 00698 80AA4198 E44862E4 */  swc1    $f8, 0x62E4($v0)           ## 000062E4
/* 0069C 80AA419C E44A62F0 */  swc1    $f10, 0x62F0($v0)          ## 000062F0
/* 006A0 80AA41A0 A43962DB */  sh      $t9, 0x62DB($at)           ## 0000E2DA
/* 006A4 80AA41A4 A42862D9 */  sh      $t0, 0x62D9($at)           ## 0000E2D8
.L80AA41A8:
/* 006A8 80AA41A8 3C014320 */  lui     $at, 0x4320                ## $at = 43200000
.L80AA41AC:
/* 006AC 80AA41AC 44818000 */  mtc1    $at, $f16                  ## $f16 = 160.00
/* 006B0 80AA41B0 C45262FC */  lwc1    $f18, 0x62FC($v0)          ## 000062FC
/* 006B4 80AA41B4 3C038016 */  lui     $v1, %hi(gGameInfo)
/* 006B8 80AA41B8 2463FA90 */  addiu   $v1, %lo(gGameInfo)
/* 006BC 80AA41BC 4612803C */  c.lt.s  $f16, $f18                 
/* 006C0 80AA41C0 00000000 */  nop
/* 006C4 80AA41C4 45020094 */  bc1fl   .L80AA4418                 
/* 006C8 80AA41C8 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
/* 006CC 80AA41CC 8C690000 */  lw      $t1, 0x0000($v1)           ## 8015FA90
/* 006D0 80AA41D0 C4446300 */  lwc1    $f4, 0x6300($v0)           ## 00006300
/* 006D4 80AA41D4 3C014352 */  lui     $at, 0x4352                ## $at = 43520000
/* 006D8 80AA41D8 852A0F1E */  lh      $t2, 0x0F1E($t1)           ## 00000F1E
/* 006DC 80AA41DC 44816000 */  mtc1    $at, $f12                  ## $f12 = 210.00
/* 006E0 80AA41E0 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 006E4 80AA41E4 448A3000 */  mtc1    $t2, $f6                   ## $f6 = 0.00
/* 006E8 80AA41E8 00300821 */  addu    $at, $at, $s0              
/* 006EC 80AA41EC 46803220 */  cvt.s.w $f8, $f6                   
/* 006F0 80AA41F0 46082280 */  add.s   $f10, $f4, $f8             
/* 006F4 80AA41F4 E42AE300 */  swc1    $f10, -0x1D00($at)         ## 0000E300
/* 006F8 80AA41F8 C4406300 */  lwc1    $f0, 0x6300($v0)           ## 00006300
/* 006FC 80AA41FC 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00700 80AA4200 00300821 */  addu    $at, $at, $s0              
/* 00704 80AA4204 4600603E */  c.le.s  $f12, $f0                  
/* 00708 80AA4208 00000000 */  nop
/* 0070C 80AA420C 45020004 */  bc1fl   .L80AA4220                 
/* 00710 80AA4210 3C014352 */  lui     $at, 0x4352                ## $at = 43520000
/* 00714 80AA4214 E42CE300 */  swc1    $f12, -0x1D00($at)         ## 4351E300
/* 00718 80AA4218 C4406300 */  lwc1    $f0, 0x6300($v0)           ## 00006300
/* 0071C 80AA421C 3C014352 */  lui     $at, 0x4352                ## $at = 43520000
.L80AA4220:
/* 00720 80AA4220 44818000 */  mtc1    $at, $f16                  ## $f16 = 210.00
/* 00724 80AA4224 00000000 */  nop
/* 00728 80AA4228 4600803E */  c.le.s  $f16, $f0                  
/* 0072C 80AA422C 00000000 */  nop
/* 00730 80AA4230 45020079 */  bc1fl   .L80AA4418                 
/* 00734 80AA4234 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
/* 00738 80AA4238 8C6B0000 */  lw      $t3, 0x0000($v1)           ## 8015FA90
/* 0073C 80AA423C C4526304 */  lwc1    $f18, 0x6304($v0)          ## 00006304
/* 00740 80AA4240 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00744 80AA4244 856C0F20 */  lh      $t4, 0x0F20($t3)           ## 00000F1F
/* 00748 80AA4248 00300821 */  addu    $at, $at, $s0              
/* 0074C 80AA424C 448C3000 */  mtc1    $t4, $f6                   ## $f6 = 0.00
/* 00750 80AA4250 00000000 */  nop
/* 00754 80AA4254 46803120 */  cvt.s.w $f4, $f6                   
/* 00758 80AA4258 46049200 */  add.s   $f8, $f18, $f4             
/* 0075C 80AA425C E428E304 */  swc1    $f8, -0x1CFC($at)          ## 0000E304
/* 00760 80AA4260 C4406304 */  lwc1    $f0, 0x6304($v0)           ## 00006304
/* 00764 80AA4264 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00768 80AA4268 00300821 */  addu    $at, $at, $s0              
/* 0076C 80AA426C 4600103E */  c.le.s  $f2, $f0                   
/* 00770 80AA4270 00000000 */  nop
/* 00774 80AA4274 45020004 */  bc1fl   .L80AA4288                 
/* 00778 80AA4278 3C014348 */  lui     $at, 0x4348                ## $at = 43480000
/* 0077C 80AA427C E422E304 */  swc1    $f2, -0x1CFC($at)          ## 4347E304
/* 00780 80AA4280 C4406304 */  lwc1    $f0, 0x6304($v0)           ## 00006304
/* 00784 80AA4284 3C014348 */  lui     $at, 0x4348                ## $at = 43480000
.L80AA4288:
/* 00788 80AA4288 44815000 */  mtc1    $at, $f10                  ## $f10 = 200.00
/* 0078C 80AA428C 00000000 */  nop
/* 00790 80AA4290 4600503E */  c.le.s  $f10, $f0                  
/* 00794 80AA4294 00000000 */  nop
/* 00798 80AA4298 4502005F */  bc1fl   .L80AA4418                 
/* 0079C 80AA429C 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
/* 007A0 80AA42A0 844D6312 */  lh      $t5, 0x6312($v0)           ## 00006312
/* 007A4 80AA42A4 C4506308 */  lwc1    $f16, 0x6308($v0)          ## 00006308
/* 007A8 80AA42A8 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 007AC 80AA42AC 448D3000 */  mtc1    $t5, $f6                   ## $f6 = 0.00
/* 007B0 80AA42B0 00300821 */  addu    $at, $at, $s0              
/* 007B4 80AA42B4 240E0002 */  addiu   $t6, $zero, 0x0002         ## $t6 = 00000002
/* 007B8 80AA42B8 468034A0 */  cvt.s.w $f18, $f6                  
/* 007BC 80AA42BC 240F0014 */  addiu   $t7, $zero, 0x0014         ## $t7 = 00000014
/* 007C0 80AA42C0 46128100 */  add.s   $f4, $f16, $f18            
/* 007C4 80AA42C4 E424E308 */  swc1    $f4, -0x1CF8($at)          ## 0000E308
/* 007C8 80AA42C8 C4486308 */  lwc1    $f8, 0x6308($v0)           ## 00006308
/* 007CC 80AA42CC 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 007D0 80AA42D0 00300821 */  addu    $at, $at, $s0              
/* 007D4 80AA42D4 4608103E */  c.le.s  $f2, $f8                   
/* 007D8 80AA42D8 00000000 */  nop
/* 007DC 80AA42DC 4502004E */  bc1fl   .L80AA4418                 
/* 007E0 80AA42E0 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
/* 007E4 80AA42E4 E422E308 */  swc1    $f2, -0x1CF8($at)          ## 0000E308
/* 007E8 80AA42E8 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 007EC 80AA42EC A42E62DD */  sh      $t6, 0x62DD($at)           ## 0000E2DC
/* 007F0 80AA42F0 3C0180AA */  lui     $at, %hi(D_80AA5EC0)       ## $at = 80AA0000
/* 007F4 80AA42F4 10000047 */  beq     $zero, $zero, .L80AA4414   
/* 007F8 80AA42F8 A42F5EC0 */  sh      $t7, %lo(D_80AA5EC0)($at)  
.L80AA42FC:
/* 007FC 80AA42FC 54610046 */  bnel    $v1, $at, .L80AA4418       
/* 00800 80AA4300 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
/* 00804 80AA4304 84586314 */  lh      $t8, 0x6314($v0)           ## 00006314
/* 00808 80AA4308 C44A62FC */  lwc1    $f10, 0x62FC($v0)          ## 000062FC
/* 0080C 80AA430C 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00810 80AA4310 44983000 */  mtc1    $t8, $f6                   ## $f6 = 0.00
/* 00814 80AA4314 00300821 */  addu    $at, $at, $s0              
/* 00818 80AA4318 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 0081C 80AA431C 46803420 */  cvt.s.w $f16, $f6                  
/* 00820 80AA4320 240A0004 */  addiu   $t2, $zero, 0x0004         ## $t2 = 00000004
/* 00824 80AA4324 46105481 */  sub.s   $f18, $f10, $f16           
/* 00828 80AA4328 E432E2FC */  swc1    $f18, -0x1D04($at)         ## 0000E2FC
/* 0082C 80AA432C C44462FC */  lwc1    $f4, 0x62FC($v0)           ## 000062FC
/* 00830 80AA4330 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00834 80AA4334 00300821 */  addu    $at, $at, $s0              
/* 00838 80AA4338 4600203C */  c.lt.s  $f4, $f0                   
/* 0083C 80AA433C 00000000 */  nop
/* 00840 80AA4340 45020003 */  bc1fl   .L80AA4350                 
/* 00844 80AA4344 84596314 */  lh      $t9, 0x6314($v0)           ## 00006314
/* 00848 80AA4348 E420E2FC */  swc1    $f0, -0x1D04($at)          ## 0000E2FC
/* 0084C 80AA434C 84596314 */  lh      $t9, 0x6314($v0)           ## 00006314
.L80AA4350:
/* 00850 80AA4350 C4486300 */  lwc1    $f8, 0x6300($v0)           ## 00006300
/* 00854 80AA4354 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00858 80AA4358 44993000 */  mtc1    $t9, $f6                   ## $f6 = 0.00
/* 0085C 80AA435C 00300821 */  addu    $at, $at, $s0              
/* 00860 80AA4360 468032A0 */  cvt.s.w $f10, $f6                  
/* 00864 80AA4364 460A4401 */  sub.s   $f16, $f8, $f10            
/* 00868 80AA4368 E430E300 */  swc1    $f16, -0x1D00($at)         ## 0000E300
/* 0086C 80AA436C C4526300 */  lwc1    $f18, 0x6300($v0)          ## 00006300
/* 00870 80AA4370 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00874 80AA4374 00300821 */  addu    $at, $at, $s0              
/* 00878 80AA4378 4600903C */  c.lt.s  $f18, $f0                  
/* 0087C 80AA437C 00000000 */  nop
/* 00880 80AA4380 45020003 */  bc1fl   .L80AA4390                 
/* 00884 80AA4384 84486314 */  lh      $t0, 0x6314($v0)           ## 00006314
/* 00888 80AA4388 E420E300 */  swc1    $f0, -0x1D00($at)          ## 0000E300
/* 0088C 80AA438C 84486314 */  lh      $t0, 0x6314($v0)           ## 00006314
.L80AA4390:
/* 00890 80AA4390 C4446304 */  lwc1    $f4, 0x6304($v0)           ## 00006304
/* 00894 80AA4394 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00898 80AA4398 44883000 */  mtc1    $t0, $f6                   ## $f6 = 0.00
/* 0089C 80AA439C 00300821 */  addu    $at, $at, $s0              
/* 008A0 80AA43A0 46803220 */  cvt.s.w $f8, $f6                   
/* 008A4 80AA43A4 46082281 */  sub.s   $f10, $f4, $f8             
/* 008A8 80AA43A8 E42AE304 */  swc1    $f10, -0x1CFC($at)         ## 0000E304
/* 008AC 80AA43AC C4506304 */  lwc1    $f16, 0x6304($v0)          ## 00006304
/* 008B0 80AA43B0 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 008B4 80AA43B4 00300821 */  addu    $at, $at, $s0              
/* 008B8 80AA43B8 4600803C */  c.lt.s  $f16, $f0                  
/* 008BC 80AA43BC 00000000 */  nop
/* 008C0 80AA43C0 45020003 */  bc1fl   .L80AA43D0                 
/* 008C4 80AA43C4 84496312 */  lh      $t1, 0x6312($v0)           ## 00006312
/* 008C8 80AA43C8 E420E304 */  swc1    $f0, -0x1CFC($at)          ## 0000E304
/* 008CC 80AA43CC 84496312 */  lh      $t1, 0x6312($v0)           ## 00006312
.L80AA43D0:
/* 008D0 80AA43D0 C4526308 */  lwc1    $f18, 0x6308($v0)          ## 00006308
/* 008D4 80AA43D4 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 008D8 80AA43D8 44893000 */  mtc1    $t1, $f6                   ## $f6 = 0.00
/* 008DC 80AA43DC 00300821 */  addu    $at, $at, $s0              
/* 008E0 80AA43E0 46803120 */  cvt.s.w $f4, $f6                   
/* 008E4 80AA43E4 46049201 */  sub.s   $f8, $f18, $f4             
/* 008E8 80AA43E8 E428E308 */  swc1    $f8, -0x1CF8($at)          ## 0000E308
/* 008EC 80AA43EC C44A6308 */  lwc1    $f10, 0x6308($v0)          ## 00006308
/* 008F0 80AA43F0 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 008F4 80AA43F4 00300821 */  addu    $at, $at, $s0              
/* 008F8 80AA43F8 4600503C */  c.lt.s  $f10, $f0                  
/* 008FC 80AA43FC 00000000 */  nop
/* 00900 80AA4400 45020005 */  bc1fl   .L80AA4418                 
/* 00904 80AA4404 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
/* 00908 80AA4408 E420E308 */  swc1    $f0, -0x1CF8($at)          ## 0000E308
/* 0090C 80AA440C 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 00910 80AA4410 A42A62DD */  sh      $t2, 0x62DD($at)           ## 0000E2DC
.L80AA4414:
/* 00914 80AA4414 844362DC */  lh      $v1, 0x62DC($v0)           ## 000062DC
.L80AA4418:
/* 00918 80AA4418 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 0091C 80AA441C 1460000A */  bne     $v1, $zero, .L80AA4448     
/* 00920 80AA4420 00000000 */  nop
/* 00924 80AA4424 0C01B129 */  jal     Flags_GetEnv              
/* 00928 80AA4428 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 0092C 80AA442C 1040000E */  beq     $v0, $zero, .L80AA4468     
/* 00930 80AA4430 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 00934 80AA4434 240B0028 */  addiu   $t3, $zero, 0x0028         ## $t3 = 00000028
/* 00938 80AA4438 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 0093C 80AA443C A42B62DB */  sh      $t3, 0x62DB($at)           ## 0000E2DA
/* 00940 80AA4440 10000009 */  beq     $zero, $zero, .L80AA4468   
/* 00944 80AA4444 A42662DD */  sh      $a2, 0x62DD($at)           ## 0000E2DC
.L80AA4448:
/* 00948 80AA4448 14610007 */  bne     $v1, $at, .L80AA4468       
/* 0094C 80AA444C 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 00950 80AA4450 0C01B129 */  jal     Flags_GetEnv              
/* 00954 80AA4454 24050004 */  addiu   $a1, $zero, 0x0004         ## $a1 = 00000004
/* 00958 80AA4458 10400003 */  beq     $v0, $zero, .L80AA4468     
/* 0095C 80AA445C 240C0003 */  addiu   $t4, $zero, 0x0003         ## $t4 = 00000003
/* 00960 80AA4460 26017FFF */  addiu   $at, $s0, 0x7FFF           ## $at = 00007FFF
/* 00964 80AA4464 A42C62DD */  sh      $t4, 0x62DD($at)           ## 0000E2DC
.L80AA4468:
/* 00968 80AA4468 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 0096C 80AA446C 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 00970 80AA4470 27BD0040 */  addiu   $sp, $sp, 0x0040           ## $sp = 00000000
/* 00974 80AA4474 03E00008 */  jr      $ra                        
/* 00978 80AA4478 00000000 */  nop