.late_rodata
glabel D_8094AF10
    .float 3.14159274101

glabel D_8094AF14
    .float 3.14159274101

glabel D_8094AF18
    .float 3.14159274101

glabel D_8094AF1C
    .float 3000.0

glabel D_8094AF20
    .float 3000.0

glabel D_8094AF24
    .float 1.99000000954

glabel D_8094AF28
 .word 0x3BA71DE7

.text
glabel func_8093C910
/* 03C40 8093C910 27BDFF40 */  addiu   $sp, $sp, 0xFF40           ## $sp = FFFFFF40
/* 03C44 8093C914 AFBF003C */  sw      $ra, 0x003C($sp)
/* 03C48 8093C918 AFB20038 */  sw      $s2, 0x0038($sp)
/* 03C4C 8093C91C AFB10034 */  sw      $s1, 0x0034($sp)
/* 03C50 8093C920 AFB00030 */  sw      $s0, 0x0030($sp)
/* 03C54 8093C924 F7B60028 */  sdc1    $f22, 0x0028($sp)
/* 03C58 8093C928 F7B40020 */  sdc1    $f20, 0x0020($sp)
/* 03C5C 8093C92C 8CAE1C44 */  lw      $t6, 0x1C44($a1)           ## 00001C44
/* 03C60 8093C930 00808825 */  or      $s1, $a0, $zero            ## $s1 = 00000000
/* 03C64 8093C934 00A09025 */  or      $s2, $a1, $zero            ## $s2 = 00000000
/* 03C68 8093C938 AFAE00A0 */  sw      $t6, 0x00A0($sp)
/* 03C6C 8093C93C 848205FA */  lh      $v0, 0x05FA($a0)           ## 000005FA
/* 03C70 8093C940 240F0001 */  addiu   $t7, $zero, 0x0001         ## $t7 = 00000001
/* 03C74 8093C944 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03C78 8093C948 1040000A */  beq     $v0, $zero, .L8093C974
/* 03C7C 8093C94C 26451D64 */  addiu   $a1, $s2, 0x1D64           ## $a1 = 00001D64
/* 03C80 8093C950 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 03C84 8093C954 10410034 */  beq     $v0, $at, .L8093CA28
/* 03C88 8093C958 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 03C8C 8093C95C 10410069 */  beq     $v0, $at, .L8093CB04
/* 03C90 8093C960 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 03C94 8093C964 4481A000 */  mtc1    $at, $f20                  ## $f20 = 1.00
/* 03C98 8093C968 4480B000 */  mtc1    $zero, $f22                ## $f22 = 0.00
/* 03C9C 8093C96C 100000A6 */  beq     $zero, $zero, .L8093CC08
/* 03CA0 8093C970 862505FC */  lh      $a1, 0x05FC($s1)           ## 000005FC
.L8093C974:
/* 03CA4 8093C974 0C019148 */  jal     func_80064520
/* 03CA8 8093C978 A62F05FA */  sh      $t7, 0x05FA($s1)           ## 000005FA
/* 03CAC 8093C97C 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03CB0 8093C980 02202825 */  or      $a1, $s1, $zero            ## $a1 = 00000000
/* 03CB4 8093C984 0C00B7D5 */  jal     func_8002DF54
/* 03CB8 8093C988 24060039 */  addiu   $a2, $zero, 0x0039         ## $a2 = 00000039
/* 03CBC 8093C98C 0C03008C */  jal     Gameplay_CreateSubCamera
/* 03CC0 8093C990 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03CC4 8093C994 A62205FC */  sh      $v0, 0x05FC($s1)           ## 000005FC
/* 03CC8 8093C998 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03CCC 8093C99C 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 03CD0 8093C9A0 0C0300C5 */  jal     Gameplay_ChangeCameraStatus
/* 03CD4 8093C9A4 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 03CD8 8093C9A8 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03CDC 8093C9AC 862505FC */  lh      $a1, 0x05FC($s1)           ## 000005FC
/* 03CE0 8093C9B0 0C0300C5 */  jal     Gameplay_ChangeCameraStatus
/* 03CE4 8093C9B4 24060007 */  addiu   $a2, $zero, 0x0007         ## $a2 = 00000007
/* 03CE8 8093C9B8 3C014448 */  lui     $at, 0x4448                ## $at = 44480000
/* 03CEC 8093C9BC 44812000 */  mtc1    $at, $f4                   ## $f4 = 800.00
/* 03CF0 8093C9C0 3C018095 */  lui     $at, %hi(D_8094AF10)       ## $at = 80950000
/* 03CF4 8093C9C4 3C188095 */  lui     $t8, %hi(koumePtr)       ## $t8 = 80950000
/* 03CF8 8093C9C8 E62406A4 */  swc1    $f4, 0x06A4($s1)           ## 000006A4
/* 03CFC 8093C9CC C426AF10 */  lwc1    $f6, %lo(D_8094AF10)($at)
/* 03D00 8093C9D0 3C198095 */  lui     $t9, %hi(koumePtr)       ## $t9 = 80950000
/* 03D04 8093C9D4 3C088095 */  lui     $t0, %hi(kotakePtr)       ## $t0 = 80950000
/* 03D08 8093C9D8 E62606AC */  swc1    $f6, 0x06AC($s1)           ## 000006AC
/* 03D0C 8093C9DC 8F18C848 */  lw      $t8, %lo(koumePtr)($t8)
/* 03D10 8093C9E0 3C098095 */  lui     $t1, %hi(kotakePtr)       ## $t1 = 80950000
/* 03D14 8093C9E4 4480B000 */  mtc1    $zero, $f22                ## $f22 = 0.00
/* 03D18 8093C9E8 A7000030 */  sh      $zero, 0x0030($t8)         ## 80950030
/* 03D1C 8093C9EC 8F39C848 */  lw      $t9, %lo(koumePtr)($t9)
/* 03D20 8093C9F0 3C014416 */  lui     $at, 0x4416                ## $at = 44160000
/* 03D24 8093C9F4 44814000 */  mtc1    $at, $f8                   ## $f8 = 600.00
/* 03D28 8093C9F8 A72000B4 */  sh      $zero, 0x00B4($t9)         ## 809500B4
/* 03D2C 8093C9FC 8D08C844 */  lw      $t0, %lo(kotakePtr)($t0)
/* 03D30 8093CA00 3C0410C8 */  lui     $a0, 0x10C8                ## $a0 = 10C80000
/* 03D34 8093CA04 348400FF */  ori     $a0, $a0, 0x00FF           ## $a0 = 10C800FF
/* 03D38 8093CA08 A5000030 */  sh      $zero, 0x0030($t0)         ## 80950030
/* 03D3C 8093CA0C 8D29C844 */  lw      $t1, %lo(kotakePtr)($t1)
/* 03D40 8093CA10 A52000B4 */  sh      $zero, 0x00B4($t1)         ## 809500B4
/* 03D44 8093CA14 E63601A8 */  swc1    $f22, 0x01A8($s1)          ## 000001A8
/* 03D48 8093CA18 E63601AC */  swc1    $f22, 0x01AC($s1)          ## 000001AC
/* 03D4C 8093CA1C 0C03E803 */  jal     Audio_SetBGM

/* 03D50 8093CA20 E62801B0 */  swc1    $f8, 0x01B0($s1)           ## 000001B0
/* 03D54 8093CA24 A6200152 */  sh      $zero, 0x0152($s1)         ## 00000152
.L8093CA28:
/* 03D58 8093CA28 86220152 */  lh      $v0, 0x0152($s1)           ## 00000152
/* 03D5C 8093CA2C 4480B000 */  mtc1    $zero, $f22                ## $f22 = 0.00
/* 03D60 8093CA30 24010014 */  addiu   $at, $zero, 0x0014         ## $at = 00000014
/* 03D64 8093CA34 14410005 */  bne     $v0, $at, .L8093CA4C
/* 03D68 8093CA38 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03D6C 8093CA3C 24056059 */  addiu   $a1, $zero, 0x6059         ## $a1 = 00006059
/* 03D70 8093CA40 0C042DA0 */  jal     func_8010B680
/* 03D74 8093CA44 00003025 */  or      $a2, $zero, $zero          ## $a2 = 00000000
/* 03D78 8093CA48 86220152 */  lh      $v0, 0x0152($s1)           ## 00000152
.L8093CA4C:
/* 03D7C 8093CA4C 24010050 */  addiu   $at, $zero, 0x0050         ## $at = 00000050
/* 03D80 8093CA50 14410004 */  bne     $v0, $at, .L8093CA64
/* 03D84 8093CA54 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03D88 8093CA58 2405605A */  addiu   $a1, $zero, 0x605A         ## $a1 = 0000605A
/* 03D8C 8093CA5C 0C042DA0 */  jal     func_8010B680
/* 03D90 8093CA60 00003025 */  or      $a2, $zero, $zero          ## $a2 = 00000000
.L8093CA64:
/* 03D94 8093CA64 3C0143DC */  lui     $at, 0x43DC                ## $at = 43DC0000
/* 03D98 8093CA68 44815000 */  mtc1    $at, $f10                  ## $f10 = 440.00
/* 03D9C 8093CA6C E636060C */  swc1    $f22, 0x060C($s1)          ## 0000060C
/* 03DA0 8093CA70 E6360614 */  swc1    $f22, 0x0614($s1)          ## 00000614
/* 03DA4 8093CA74 E62A0610 */  swc1    $f10, 0x0610($s1)          ## 00000610
/* 03DA8 8093CA78 E7B600B0 */  swc1    $f22, 0x00B0($sp)
/* 03DAC 8093CA7C E7B600B4 */  swc1    $f22, 0x00B4($sp)
/* 03DB0 8093CA80 C63006A4 */  lwc1    $f16, 0x06A4($s1)          ## 000006A4
/* 03DB4 8093CA84 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 03DB8 8093CA88 E7B000B8 */  swc1    $f16, 0x00B8($sp)
/* 03DBC 8093CA8C 0C034348 */  jal     Matrix_RotateY
/* 03DC0 8093CA90 C62C06AC */  lwc1    $f12, 0x06AC($s1)          ## 000006AC
/* 03DC4 8093CA94 27A400B0 */  addiu   $a0, $sp, 0x00B0           ## $a0 = FFFFFFF0
/* 03DC8 8093CA98 0C0346BD */  jal     Matrix_MultVec3f
/* 03DCC 8093CA9C 27A500A4 */  addiu   $a1, $sp, 0x00A4           ## $a1 = FFFFFFE4
/* 03DD0 8093CAA0 3C014396 */  lui     $at, 0x4396                ## $at = 43960000
/* 03DD4 8093CAA4 C7B200A4 */  lwc1    $f18, 0x00A4($sp)
/* 03DD8 8093CAA8 44812000 */  mtc1    $at, $f4                   ## $f4 = 300.00
/* 03DDC 8093CAAC 3C053E99 */  lui     $a1, 0x3E99                ## $a1 = 3E990000
/* 03DE0 8093CAB0 E6320600 */  swc1    $f18, 0x0600($s1)          ## 00000600
/* 03DE4 8093CAB4 E6240604 */  swc1    $f4, 0x0604($s1)           ## 00000604
/* 03DE8 8093CAB8 C7A600AC */  lwc1    $f6, 0x00AC($sp)
/* 03DEC 8093CABC 3C063CA3 */  lui     $a2, 0x3CA3                ## $a2 = 3CA30000
/* 03DF0 8093CAC0 3C073CF5 */  lui     $a3, 0x3CF5                ## $a3 = 3CF50000
/* 03DF4 8093CAC4 34E7C28F */  ori     $a3, $a3, 0xC28F           ## $a3 = 3CF5C28F
/* 03DF8 8093CAC8 34C6D70A */  ori     $a2, $a2, 0xD70A           ## $a2 = 3CA3D70A
/* 03DFC 8093CACC 34A5999A */  ori     $a1, $a1, 0x999A           ## $a1 = 3E99999A
/* 03E00 8093CAD0 262406AC */  addiu   $a0, $s1, 0x06AC           ## $a0 = 000006AC
/* 03E04 8093CAD4 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 03E08 8093CAD8 E6260608 */  swc1    $f6, 0x0608($s1)           ## 00000608
/* 03E0C 8093CADC 3C063DCC */  lui     $a2, 0x3DCC                ## $a2 = 3DCC0000
/* 03E10 8093CAE0 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3DCCCCCD
/* 03E14 8093CAE4 262406A4 */  addiu   $a0, $s1, 0x06A4           ## $a0 = 000006A4
/* 03E18 8093CAE8 3C054348 */  lui     $a1, 0x4348                ## $a1 = 43480000
/* 03E1C 8093CAEC 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 03E20 8093CAF0 3C0740A0 */  lui     $a3, 0x40A0                ## $a3 = 40A00000
/* 03E24 8093CAF4 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 03E28 8093CAF8 4481A000 */  mtc1    $at, $f20                  ## $f20 = 1.00
/* 03E2C 8093CAFC 10000042 */  beq     $zero, $zero, .L8093CC08
/* 03E30 8093CB00 862505FC */  lh      $a1, 0x05FC($s1)           ## 000005FC
.L8093CB04:
/* 03E34 8093CB04 4480B000 */  mtc1    $zero, $f22                ## $f22 = 0.00
/* 03E38 8093CB08 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 03E3C 8093CB0C E7B600B0 */  swc1    $f22, 0x00B0($sp)
/* 03E40 8093CB10 E7B600B4 */  swc1    $f22, 0x00B4($sp)
/* 03E44 8093CB14 C62806A4 */  lwc1    $f8, 0x06A4($s1)           ## 000006A4
/* 03E48 8093CB18 E7A800B8 */  swc1    $f8, 0x00B8($sp)
/* 03E4C 8093CB1C 0C034348 */  jal     Matrix_RotateY
/* 03E50 8093CB20 C62C06AC */  lwc1    $f12, 0x06AC($s1)          ## 000006AC
/* 03E54 8093CB24 27A400B0 */  addiu   $a0, $sp, 0x00B0           ## $a0 = FFFFFFF0
/* 03E58 8093CB28 0C0346BD */  jal     Matrix_MultVec3f
/* 03E5C 8093CB2C 27A500A4 */  addiu   $a1, $sp, 0x00A4           ## $a1 = FFFFFFE4
/* 03E60 8093CB30 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 03E64 8093CB34 44812000 */  mtc1    $at, $f4                   ## $f4 = 20.00
/* 03E68 8093CB38 C632069C */  lwc1    $f18, 0x069C($s1)          ## 0000069C
/* 03E6C 8093CB3C C7AA00A4 */  lwc1    $f10, 0x00A4($sp)
/* 03E70 8093CB40 3C063DCC */  lui     $a2, 0x3DCC                ## $a2 = 3DCC0000
/* 03E74 8093CB44 46049182 */  mul.s   $f6, $f18, $f4
/* 03E78 8093CB48 E62A0600 */  swc1    $f10, 0x0600($s1)          ## 00000600
/* 03E7C 8093CB4C C7B000AC */  lwc1    $f16, 0x00AC($sp)
/* 03E80 8093CB50 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3DCCCCCD
/* 03E84 8093CB54 26240604 */  addiu   $a0, $s1, 0x0604           ## $a0 = 00000604
/* 03E88 8093CB58 3C0543D2 */  lui     $a1, 0x43D2                ## $a1 = 43D20000
/* 03E8C 8093CB5C E6300608 */  swc1    $f16, 0x0608($s1)          ## 00000608
/* 03E90 8093CB60 44073000 */  mfc1    $a3, $f6
/* 03E94 8093CB64 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 03E98 8093CB68 00000000 */  nop
/* 03E9C 8093CB6C 3C0140C0 */  lui     $at, 0x40C0                ## $at = 40C00000
/* 03EA0 8093CB70 44815000 */  mtc1    $at, $f10                  ## $f10 = 6.00
/* 03EA4 8093CB74 C628069C */  lwc1    $f8, 0x069C($s1)           ## 0000069C
/* 03EA8 8093CB78 3C063DCC */  lui     $a2, 0x3DCC                ## $a2 = 3DCC0000
/* 03EAC 8093CB7C 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3DCCCCCD
/* 03EB0 8093CB80 460A4402 */  mul.s   $f16, $f8, $f10
/* 03EB4 8093CB84 26240610 */  addiu   $a0, $s1, 0x0610           ## $a0 = 00000610
/* 03EB8 8093CB88 3C0543EB */  lui     $a1, 0x43EB                ## $a1 = 43EB0000
/* 03EBC 8093CB8C 44078000 */  mfc1    $a3, $f16
/* 03EC0 8093CB90 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 03EC4 8093CB94 00000000 */  nop
/* 03EC8 8093CB98 3C053E99 */  lui     $a1, 0x3E99                ## $a1 = 3E990000
/* 03ECC 8093CB9C 3C063CA3 */  lui     $a2, 0x3CA3                ## $a2 = 3CA30000
/* 03ED0 8093CBA0 3C073CF5 */  lui     $a3, 0x3CF5                ## $a3 = 3CF50000
/* 03ED4 8093CBA4 34E7C28F */  ori     $a3, $a3, 0xC28F           ## $a3 = 3CF5C28F
/* 03ED8 8093CBA8 34C6D70A */  ori     $a2, $a2, 0xD70A           ## $a2 = 3CA3D70A
/* 03EDC 8093CBAC 34A5999A */  ori     $a1, $a1, 0x999A           ## $a1 = 3E99999A
/* 03EE0 8093CBB0 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 03EE4 8093CBB4 262406AC */  addiu   $a0, $s1, 0x06AC           ## $a0 = 000006AC
/* 03EE8 8093CBB8 3C014200 */  lui     $at, 0x4200                ## $at = 42000000
/* 03EEC 8093CBBC 44812000 */  mtc1    $at, $f4                   ## $f4 = 32.00
/* 03EF0 8093CBC0 C632069C */  lwc1    $f18, 0x069C($s1)          ## 0000069C
/* 03EF4 8093CBC4 3C063DCC */  lui     $a2, 0x3DCC                ## $a2 = 3DCC0000
/* 03EF8 8093CBC8 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3DCCCCCD
/* 03EFC 8093CBCC 46049182 */  mul.s   $f6, $f18, $f4
/* 03F00 8093CBD0 262406A4 */  addiu   $a0, $s1, 0x06A4           ## $a0 = 000006A4
/* 03F04 8093CBD4 3C054270 */  lui     $a1, 0x4270                ## $a1 = 42700000
/* 03F08 8093CBD8 44073000 */  mfc1    $a3, $f6
/* 03F0C 8093CBDC 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 03F10 8093CBE0 00000000 */  nop
/* 03F14 8093CBE4 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 03F18 8093CBE8 4481A000 */  mtc1    $at, $f20                  ## $f20 = 1.00
/* 03F1C 8093CBEC 3C073DCC */  lui     $a3, 0x3DCC                ## $a3 = 3DCC0000
/* 03F20 8093CBF0 34E7CCCD */  ori     $a3, $a3, 0xCCCD           ## $a3 = 3DCCCCCD
/* 03F24 8093CBF4 4405A000 */  mfc1    $a1, $f20
/* 03F28 8093CBF8 4406A000 */  mfc1    $a2, $f20
/* 03F2C 8093CBFC 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 03F30 8093CC00 2624069C */  addiu   $a0, $s1, 0x069C           ## $a0 = 0000069C
/* 03F34 8093CC04 862505FC */  lh      $a1, 0x05FC($s1)           ## 000005FC
.L8093CC08:
/* 03F38 8093CC08 50A0000E */  beql    $a1, $zero, .L8093CC44
/* 03F3C 8093CC0C 86220440 */  lh      $v0, 0x0440($s1)           ## 00000440
/* 03F40 8093CC10 922A05F9 */  lbu     $t2, 0x05F9($s1)           ## 000005F9
/* 03F44 8093CC14 26270600 */  addiu   $a3, $s1, 0x0600           ## $a3 = 00000600
/* 03F48 8093CC18 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03F4C 8093CC1C 15400006 */  bne     $t2, $zero, .L8093CC38
/* 03F50 8093CC20 26260630 */  addiu   $a2, $s1, 0x0630           ## $a2 = 00000630
/* 03F54 8093CC24 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 03F58 8093CC28 0C030136 */  jal     func_800C04D8
/* 03F5C 8093CC2C 2626060C */  addiu   $a2, $s1, 0x060C           ## $a2 = 0000060C
/* 03F60 8093CC30 10000004 */  beq     $zero, $zero, .L8093CC44
/* 03F64 8093CC34 86220440 */  lh      $v0, 0x0440($s1)           ## 00000440
.L8093CC38:
/* 03F68 8093CC38 0C030136 */  jal     func_800C04D8
/* 03F6C 8093CC3C 26270624 */  addiu   $a3, $s1, 0x0624           ## $a3 = 00000624
/* 03F70 8093CC40 86220440 */  lh      $v0, 0x0440($s1)           ## 00000440
.L8093CC44:
/* 03F74 8093CC44 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 03F78 8093CC48 3C048095 */  lui     $a0, %hi(kotakePtr)       ## $a0 = 80950000
/* 03F7C 8093CC4C 10400008 */  beq     $v0, $zero, .L8093CC70
/* 03F80 8093CC50 2405311F */  addiu   $a1, $zero, 0x311F         ## $a1 = 0000311F
/* 03F84 8093CC54 10410122 */  beq     $v0, $at, .L8093D0E0
/* 03F88 8093CC58 26300568 */  addiu   $s0, $s1, 0x0568           ## $s0 = 00000568
/* 03F8C 8093CC5C 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 03F90 8093CC60 10410137 */  beq     $v0, $at, .L8093D140
/* 03F94 8093CC64 00000000 */  nop
/* 03F98 8093CC68 100001D4 */  beq     $zero, $zero, .L8093D3BC
/* 03F9C 8093CC6C 8FBF003C */  lw      $ra, 0x003C($sp)
.L8093CC70:
/* 03FA0 8093CC70 0C00BE0A */  jal     Audio_PlayActorSound2

/* 03FA4 8093CC74 8C84C844 */  lw      $a0, %lo(kotakePtr)($a0)
/* 03FA8 8093CC78 3C048095 */  lui     $a0, %hi(koumePtr)       ## $a0 = 80950000
/* 03FAC 8093CC7C 8C84C848 */  lw      $a0, %lo(koumePtr)($a0)
/* 03FB0 8093CC80 0C00BE0A */  jal     Audio_PlayActorSound2

/* 03FB4 8093CC84 2405311F */  addiu   $a1, $zero, 0x311F         ## $a1 = 0000311F
/* 03FB8 8093CC88 3C0143C8 */  lui     $at, 0x43C8                ## $at = 43C80000
/* 03FBC 8093CC8C C62801B0 */  lwc1    $f8, 0x01B0($s1)           ## 000001B0
/* 03FC0 8093CC90 44815000 */  mtc1    $at, $f10                  ## $f10 = 400.00
/* 03FC4 8093CC94 E7B600B8 */  swc1    $f22, 0x00B8($sp)
/* 03FC8 8093CC98 E7A800B0 */  swc1    $f8, 0x00B0($sp)
/* 03FCC 8093CC9C E7AA00B4 */  swc1    $f10, 0x00B4($sp)
/* 03FD0 8093CCA0 C62C01A8 */  lwc1    $f12, 0x01A8($s1)          ## 000001A8
/* 03FD4 8093CCA4 0C034348 */  jal     Matrix_RotateY
/* 03FD8 8093CCA8 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 03FDC 8093CCAC 27A400B0 */  addiu   $a0, $sp, 0x00B0           ## $a0 = FFFFFFF0
/* 03FE0 8093CCB0 0C0346BD */  jal     Matrix_MultVec3f
/* 03FE4 8093CCB4 27A500A4 */  addiu   $a1, $sp, 0x00A4           ## $a1 = FFFFFFE4
/* 03FE8 8093CCB8 3C028095 */  lui     $v0, %hi(koumePtr)       ## $v0 = 80950000
/* 03FEC 8093CCBC 2442C848 */  addiu   $v0, $v0, %lo(koumePtr)  ## $v0 = 8094C848
/* 03FF0 8093CCC0 8C4B0000 */  lw      $t3, 0x0000($v0)           ## 8094C848
/* 03FF4 8093CCC4 C7B000A4 */  lwc1    $f16, 0x00A4($sp)
/* 03FF8 8093CCC8 3C014700 */  lui     $at, 0x4700                ## $at = 47000000
/* 03FFC 8093CCCC 44810000 */  mtc1    $at, $f0                   ## $f0 = 32768.00
/* 04000 8093CCD0 E5700024 */  swc1    $f16, 0x0024($t3)          ## 00000024
/* 04004 8093CCD4 C7B200A8 */  lwc1    $f18, 0x00A8($sp)
/* 04008 8093CCD8 8C4C0000 */  lw      $t4, 0x0000($v0)           ## 8094C848
/* 0400C 8093CCDC 3C018095 */  lui     $at, %hi(D_8094AF14)       ## $at = 80950000
/* 04010 8093CCE0 3C038095 */  lui     $v1, %hi(kotakePtr)       ## $v1 = 80950000
/* 04014 8093CCE4 E5920028 */  swc1    $f18, 0x0028($t4)          ## 00000028
/* 04018 8093CCE8 C7A400AC */  lwc1    $f4, 0x00AC($sp)
/* 0401C 8093CCEC 8C4D0000 */  lw      $t5, 0x0000($v0)           ## 8094C848
/* 04020 8093CCF0 2463C844 */  addiu   $v1, $v1, %lo(kotakePtr)  ## $v1 = 8094C844
/* 04024 8093CCF4 4405B000 */  mfc1    $a1, $f22
/* 04028 8093CCF8 E5A4002C */  swc1    $f4, 0x002C($t5)           ## 0000002C
/* 0402C 8093CCFC C62601A8 */  lwc1    $f6, 0x01A8($s1)           ## 000001A8
/* 04030 8093CD00 C428AF14 */  lwc1    $f8, %lo(D_8094AF14)($at)
/* 04034 8093CD04 8C580000 */  lw      $t8, 0x0000($v0)           ## 8094C848
/* 04038 8093CD08 3C018095 */  lui     $at, %hi(D_8094AF18)       ## $at = 80950000
/* 0403C 8093CD0C 46083283 */  div.s   $f10, $f6, $f8
/* 04040 8093CD10 3C063DCC */  lui     $a2, 0x3DCC                ## $a2 = 3DCC0000
/* 04044 8093CD14 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3DCCCCCD
/* 04048 8093CD18 262401B0 */  addiu   $a0, $s1, 0x01B0           ## $a0 = 000001B0
/* 0404C 8093CD1C 3C0740E0 */  lui     $a3, 0x40E0                ## $a3 = 40E00000
/* 04050 8093CD20 46005402 */  mul.s   $f16, $f10, $f0
/* 04054 8093CD24 4600848D */  trunc.w.s $f18, $f16
/* 04058 8093CD28 440F9000 */  mfc1    $t7, $f18
/* 0405C 8093CD2C 00000000 */  nop
/* 04060 8093CD30 A70F00B6 */  sh      $t7, 0x00B6($t8)           ## 000000B6
/* 04064 8093CD34 C7A400A4 */  lwc1    $f4, 0x00A4($sp)
/* 04068 8093CD38 8C790000 */  lw      $t9, 0x0000($v1)           ## 8094C844
/* 0406C 8093CD3C 46002187 */  neg.s   $f6, $f4
/* 04070 8093CD40 E7260024 */  swc1    $f6, 0x0024($t9)           ## 00000024
/* 04074 8093CD44 8C680000 */  lw      $t0, 0x0000($v1)           ## 8094C844
/* 04078 8093CD48 C7A800A8 */  lwc1    $f8, 0x00A8($sp)
/* 0407C 8093CD4C E5080028 */  swc1    $f8, 0x0028($t0)           ## 00000028
/* 04080 8093CD50 C7AA00AC */  lwc1    $f10, 0x00AC($sp)
/* 04084 8093CD54 8C690000 */  lw      $t1, 0x0000($v1)           ## 8094C844
/* 04088 8093CD58 46005407 */  neg.s   $f16, $f10
/* 0408C 8093CD5C E530002C */  swc1    $f16, 0x002C($t1)          ## 0000002C
/* 04090 8093CD60 C424AF18 */  lwc1    $f4, %lo(D_8094AF18)($at)
/* 04094 8093CD64 C63201A8 */  lwc1    $f18, 0x01A8($s1)          ## 000001A8
/* 04098 8093CD68 8C6C0000 */  lw      $t4, 0x0000($v1)           ## 8094C844
/* 0409C 8093CD6C 46049183 */  div.s   $f6, $f18, $f4
/* 040A0 8093CD70 46003202 */  mul.s   $f8, $f6, $f0
/* 040A4 8093CD74 46004280 */  add.s   $f10, $f8, $f0
/* 040A8 8093CD78 4600540D */  trunc.w.s $f16, $f10
/* 040AC 8093CD7C 440B8000 */  mfc1    $t3, $f16
/* 040B0 8093CD80 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 040B4 8093CD84 A58B00B6 */  sh      $t3, 0x00B6($t4)           ## 000000B6
/* 040B8 8093CD88 C63201A8 */  lwc1    $f18, 0x01A8($s1)          ## 000001A8
/* 040BC 8093CD8C C62401AC */  lwc1    $f4, 0x01AC($s1)           ## 000001AC
/* 040C0 8093CD90 4406A000 */  mfc1    $a2, $f20
/* 040C4 8093CD94 3C073B83 */  lui     $a3, 0x3B83                ## $a3 = 3B830000
/* 040C8 8093CD98 46049181 */  sub.s   $f6, $f18, $f4
/* 040CC 8093CD9C 34E7126E */  ori     $a3, $a3, 0x126E           ## $a3 = 3B83126E
/* 040D0 8093CDA0 262401AC */  addiu   $a0, $s1, 0x01AC           ## $a0 = 000001AC
/* 040D4 8093CDA4 3C053F00 */  lui     $a1, 0x3F00                ## $a1 = 3F000000
/* 040D8 8093CDA8 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 040DC 8093CDAC E62601A8 */  swc1    $f6, 0x01A8($s1)           ## 000001A8
/* 040E0 8093CDB0 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 040E4 8093CDB4 44815000 */  mtc1    $at, $f10                  ## $f10 = 10.00
/* 040E8 8093CDB8 C62801B0 */  lwc1    $f8, 0x01B0($s1)           ## 000001B0
/* 040EC 8093CDBC 460A403C */  c.lt.s  $f8, $f10
/* 040F0 8093CDC0 00000000 */  nop
/* 040F4 8093CDC4 450000B5 */  bc1f    .L8093D09C
/* 040F8 8093CDC8 00000000 */  nop
/* 040FC 8093CDCC 862D0166 */  lh      $t5, 0x0166($s1)           ## 00000166
/* 04100 8093CDD0 3C048095 */  lui     $a0, %hi(koumePtr)       ## $a0 = 80950000
/* 04104 8093CDD4 24053910 */  addiu   $a1, $zero, 0x3910         ## $a1 = 00003910
/* 04108 8093CDD8 15A00005 */  bne     $t5, $zero, .L8093CDF0
/* 0410C 8093CDDC 00000000 */  nop
/* 04110 8093CDE0 0C00BE0A */  jal     Audio_PlayActorSound2

/* 04114 8093CDE4 8C84C848 */  lw      $a0, %lo(koumePtr)($a0)
/* 04118 8093CDE8 240E0001 */  addiu   $t6, $zero, 0x0001         ## $t6 = 00000001
/* 0411C 8093CDEC A62E0166 */  sh      $t6, 0x0166($s1)           ## 00000166
.L8093CDF0:
/* 04120 8093CDF0 3C048095 */  lui     $a0, %hi(koumePtr)       ## $a0 = 80950000
/* 04124 8093CDF4 8C84C848 */  lw      $a0, %lo(koumePtr)($a0)
/* 04128 8093CDF8 3C053BA3 */  lui     $a1, 0x3BA3                ## $a1 = 3BA30000
/* 0412C 8093CDFC 4406A000 */  mfc1    $a2, $f20
/* 04130 8093CE00 3C0739C4 */  lui     $a3, 0x39C4                ## $a3 = 39C40000
/* 04134 8093CE04 34E79BA9 */  ori     $a3, $a3, 0x9BA9           ## $a3 = 39C49BA9
/* 04138 8093CE08 34A5D70C */  ori     $a1, $a1, 0xD70C           ## $a1 = 3BA3D70C
/* 0413C 8093CE0C 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 04140 8093CE10 24840050 */  addiu   $a0, $a0, 0x0050           ## $a0 = 80950050
/* 04144 8093CE14 00008025 */  or      $s0, $zero, $zero          ## $s0 = 00000000
.L8093CE18:
/* 04148 8093CE18 3C0F8095 */  lui     $t7, %hi(koumePtr)       ## $t7 = 80950000
/* 0414C 8093CE1C 8DEFC848 */  lw      $t7, %lo(koumePtr)($t7)
/* 04150 8093CE20 3C018095 */  lui     $at, %hi(D_8094AF1C)       ## $at = 80950000
/* 04154 8093CE24 C432AF1C */  lwc1    $f18, %lo(D_8094AF1C)($at)
/* 04158 8093CE28 C5F00050 */  lwc1    $f16, 0x0050($t7)          ## 80950050
/* 0415C 8093CE2C 46128502 */  mul.s   $f20, $f16, $f18
/* 04160 8093CE30 0C00CFC8 */  jal     Math_Rand_CenteredFloat

/* 04164 8093CE34 4614A300 */  add.s   $f12, $f20, $f20
/* 04168 8093CE38 3C018095 */  lui     $at, %hi(D_8094AF20)       ## $at = 80950000
/* 0416C 8093CE3C C424AF20 */  lwc1    $f4, %lo(D_8094AF20)($at)
/* 04170 8093CE40 3C0143C8 */  lui     $at, 0x43C8                ## $at = 43C80000
/* 04174 8093CE44 44813000 */  mtc1    $at, $f6                   ## $f6 = 400.00
/* 04178 8093CE48 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 0417C 8093CE4C 44816000 */  mtc1    $at, $f12                  ## $f12 = 5.00
/* 04180 8093CE50 46003200 */  add.s   $f8, $f6, $f0
/* 04184 8093CE54 E7B6009C */  swc1    $f22, 0x009C($sp)
/* 04188 8093CE58 E7A00090 */  swc1    $f0, 0x0090($sp)
/* 0418C 8093CE5C E7A40094 */  swc1    $f4, 0x0094($sp)
/* 04190 8093CE60 0C00CFBE */  jal     Math_Rand_ZeroFloat

/* 04194 8093CE64 E7A80098 */  swc1    $f8, 0x0098($sp)
/* 04198 8093CE68 4614A282 */  mul.s   $f10, $f20, $f20
/* 0419C 8093CE6C C7A20090 */  lwc1    $f2, 0x0090($sp)
/* 041A0 8093CE70 E7A00054 */  swc1    $f0, 0x0054($sp)
/* 041A4 8093CE74 3C018095 */  lui     $at, %hi(D_8094AF24)       ## $at = 80950000
/* 041A8 8093CE78 46021402 */  mul.s   $f16, $f2, $f2
/* 041AC 8093CE7C C42CAF24 */  lwc1    $f12, %lo(D_8094AF24)($at)
/* 041B0 8093CE80 46105001 */  sub.s   $f0, $f10, $f16
/* 041B4 8093CE84 46000004 */  sqrt.s  $f0, $f0
/* 041B8 8093CE88 0C00CFBE */  jal     Math_Rand_ZeroFloat

/* 041BC 8093CE8C E7A00058 */  swc1    $f0, 0x0058($sp)
/* 041C0 8093CE90 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 041C4 8093CE94 44812000 */  mtc1    $at, $f4                   ## $f4 = 10.00
/* 041C8 8093CE98 C7B20054 */  lwc1    $f18, 0x0054($sp)
/* 041CC 8093CE9C 4600020D */  trunc.w.s $f8, $f0
/* 041D0 8093CEA0 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 041D4 8093CEA4 27A50094 */  addiu   $a1, $sp, 0x0094           ## $a1 = FFFFFFD4
/* 041D8 8093CEA8 46049180 */  add.s   $f6, $f18, $f4
/* 041DC 8093CEAC 44194000 */  mfc1    $t9, $f8
/* 041E0 8093CEB0 8FA70058 */  lw      $a3, 0x0058($sp)
/* 041E4 8093CEB4 44063000 */  mfc1    $a2, $f6
/* 041E8 8093CEB8 0C24E452 */  jal     func_80939148
/* 041EC 8093CEBC AFB90010 */  sw      $t9, 0x0010($sp)
/* 041F0 8093CEC0 26100001 */  addiu   $s0, $s0, 0x0001           ## $s0 = 00000001
/* 041F4 8093CEC4 00108400 */  sll     $s0, $s0, 16
/* 041F8 8093CEC8 00108403 */  sra     $s0, $s0, 16
/* 041FC 8093CECC 2A010004 */  slti    $at, $s0, 0x0004
/* 04200 8093CED0 1420FFD1 */  bne     $at, $zero, .L8093CE18
/* 04204 8093CED4 00000000 */  nop
/* 04208 8093CED8 3C088095 */  lui     $t0, %hi(koumePtr)       ## $t0 = 80950000
/* 0420C 8093CEDC 8D08C848 */  lw      $t0, %lo(koumePtr)($t0)
/* 04210 8093CEE0 3C018095 */  lui     $at, %hi(D_8094AF28)       ## $at = 80950000
/* 04214 8093CEE4 C42AAF28 */  lwc1    $f10, %lo(D_8094AF28)($at)
/* 04218 8093CEE8 C5100050 */  lwc1    $f16, 0x0050($t0)          ## 80950050
/* 0421C 8093CEEC 3C0143C8 */  lui     $at, 0x43C8                ## $at = 43C80000
/* 04220 8093CEF0 00008025 */  or      $s0, $zero, $zero          ## $s0 = 00000000
/* 04224 8093CEF4 460A803E */  c.le.s  $f16, $f10
/* 04228 8093CEF8 26220024 */  addiu   $v0, $s1, 0x0024           ## $v0 = 00000024
/* 0422C 8093CEFC 45000067 */  bc1f    .L8093D09C
/* 04230 8093CF00 00000000 */  nop
/* 04234 8093CF04 44819000 */  mtc1    $at, $f18                  ## $f18 = 400.00
/* 04238 8093CF08 3C0141C8 */  lui     $at, 0x41C8                ## $at = 41C80000
/* 0423C 8093CF0C 4481A000 */  mtc1    $at, $f20                  ## $f20 = 25.00
/* 04240 8093CF10 E6320028 */  swc1    $f18, 0x0028($s1)          ## 00000028
/* 04244 8093CF14 AFA2004C */  sw      $v0, 0x004C($sp)
/* 04248 8093CF18 8FA2004C */  lw      $v0, 0x004C($sp)
.L8093CF1C:
/* 0424C 8093CF1C 27A90080 */  addiu   $t1, $sp, 0x0080           ## $t1 = FFFFFFC0
/* 04250 8093CF20 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 04254 8093CF24 8C4B0000 */  lw      $t3, 0x0000($v0)           ## 00000024
/* 04258 8093CF28 44816000 */  mtc1    $at, $f12                  ## $f12 = 20.00
/* 0425C 8093CF2C AD2B0000 */  sw      $t3, 0x0000($t1)           ## FFFFFFC0
/* 04260 8093CF30 8C4A0004 */  lw      $t2, 0x0004($v0)           ## 00000028
/* 04264 8093CF34 AD2A0004 */  sw      $t2, 0x0004($t1)           ## FFFFFFC4
/* 04268 8093CF38 8C4B0008 */  lw      $t3, 0x0008($v0)           ## 0000002C
/* 0426C 8093CF3C 0C00CFC8 */  jal     Math_Rand_CenteredFloat

/* 04270 8093CF40 AD2B0008 */  sw      $t3, 0x0008($t1)           ## FFFFFFC8
/* 04274 8093CF44 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 04278 8093CF48 44816000 */  mtc1    $at, $f12                  ## $f12 = 20.00
/* 0427C 8093CF4C 0C00CFC8 */  jal     Math_Rand_CenteredFloat

/* 04280 8093CF50 E7A00074 */  swc1    $f0, 0x0074($sp)
/* 04284 8093CF54 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 04288 8093CF58 44816000 */  mtc1    $at, $f12                  ## $f12 = 20.00
/* 0428C 8093CF5C 0C00CFC8 */  jal     Math_Rand_CenteredFloat

/* 04290 8093CF60 E7A00078 */  swc1    $f0, 0x0078($sp)
/* 04294 8093CF64 C7A40080 */  lwc1    $f4, 0x0080($sp)
/* 04298 8093CF68 C7A60074 */  lwc1    $f6, 0x0074($sp)
/* 0429C 8093CF6C C7AA0084 */  lwc1    $f10, 0x0084($sp)
/* 042A0 8093CF70 C7B00078 */  lwc1    $f16, 0x0078($sp)
/* 042A4 8093CF74 46062200 */  add.s   $f8, $f4, $f6
/* 042A8 8093CF78 C7A40088 */  lwc1    $f4, 0x0088($sp)
/* 042AC 8093CF7C 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 042B0 8093CF80 46105480 */  add.s   $f18, $f10, $f16
/* 042B4 8093CF84 44816000 */  mtc1    $at, $f12                  ## $f12 = 10.00
/* 042B8 8093CF88 E7A0007C */  swc1    $f0, 0x007C($sp)
/* 042BC 8093CF8C 46002180 */  add.s   $f6, $f4, $f0
/* 042C0 8093CF90 E7A80080 */  swc1    $f8, 0x0080($sp)
/* 042C4 8093CF94 E7B20084 */  swc1    $f18, 0x0084($sp)
/* 042C8 8093CF98 E7B60068 */  swc1    $f22, 0x0068($sp)
/* 042CC 8093CF9C E7A60088 */  swc1    $f6, 0x0088($sp)
/* 042D0 8093CFA0 E7B6006C */  swc1    $f22, 0x006C($sp)
/* 042D4 8093CFA4 0C00CFBE */  jal     Math_Rand_ZeroFloat

/* 042D8 8093CFA8 E7B60070 */  swc1    $f22, 0x0070($sp)
/* 042DC 8093CFAC C7AA0074 */  lwc1    $f10, 0x0074($sp)
/* 042E0 8093CFB0 46140200 */  add.s   $f8, $f0, $f20
/* 042E4 8093CFB4 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 042E8 8093CFB8 27A50080 */  addiu   $a1, $sp, 0x0080           ## $a1 = FFFFFFC0
/* 042EC 8093CFBC 4616503C */  c.lt.s  $f10, $f22
/* 042F0 8093CFC0 E7A80010 */  swc1    $f8, 0x0010($sp)
/* 042F4 8093CFC4 27A60074 */  addiu   $a2, $sp, 0x0074           ## $a2 = FFFFFFB4
/* 042F8 8093CFC8 27A70068 */  addiu   $a3, $sp, 0x0068           ## $a3 = FFFFFFA8
/* 042FC 8093CFCC 45000002 */  bc1f    .L8093CFD8
/* 04300 8093CFD0 00006025 */  or      $t4, $zero, $zero          ## $t4 = 00000000
/* 04304 8093CFD4 240C0001 */  addiu   $t4, $zero, 0x0001         ## $t4 = 00000001
.L8093CFD8:
/* 04308 8093CFD8 0C24E41C */  jal     func_80939070
/* 0430C 8093CFDC AFAC0014 */  sw      $t4, 0x0014($sp)
/* 04310 8093CFE0 26100001 */  addiu   $s0, $s0, 0x0001           ## $s0 = 00000001
/* 04314 8093CFE4 00108400 */  sll     $s0, $s0, 16
/* 04318 8093CFE8 00108403 */  sra     $s0, $s0, 16
/* 0431C 8093CFEC 2A010032 */  slti    $at, $s0, 0x0032
/* 04320 8093CFF0 5420FFCA */  bnel    $at, $zero, .L8093CF1C
/* 04324 8093CFF4 8FA2004C */  lw      $v0, 0x004C($sp)
/* 04328 8093CFF8 8E2F0004 */  lw      $t7, 0x0004($s1)           ## 00000004
/* 0432C 8093CFFC 240D0001 */  addiu   $t5, $zero, 0x0001         ## $t5 = 00000001
/* 04330 8093D000 240E0001 */  addiu   $t6, $zero, 0x0001         ## $t6 = 00000001
/* 04334 8093D004 35F80001 */  ori     $t8, $t7, 0x0001           ## $t8 = 00000001
/* 04338 8093D008 A62D0440 */  sh      $t5, 0x0440($s1)           ## 00000440
/* 0433C 8093D00C A22E0564 */  sb      $t6, 0x0564($s1)           ## 00000564
/* 04340 8093D010 AE380004 */  sw      $t8, 0x0004($s1)           ## 00000004
/* 04344 8093D014 A62000B6 */  sh      $zero, 0x00B6($s1)         ## 000000B6
/* 04348 8093D018 3C048095 */  lui     $a0, %hi(kotakePtr)       ## $a0 = 80950000
/* 0434C 8093D01C 8C84C844 */  lw      $a0, %lo(kotakePtr)($a0)
/* 04350 8093D020 0C24F209 */  jal     BossTw_TwinrovaSetupWaitForMerge
/* 04354 8093D024 02402825 */  or      $a1, $s2, $zero            ## $a1 = 00000000
/* 04358 8093D028 3C048095 */  lui     $a0, %hi(koumePtr)       ## $a0 = 80950000
/* 0435C 8093D02C 8C84C848 */  lw      $a0, %lo(koumePtr)($a0)
/* 04360 8093D030 0C24F209 */  jal     BossTw_TwinrovaSetupWaitForMerge
/* 04364 8093D034 02402825 */  or      $a1, $s2, $zero            ## $a1 = 00000000
/* 04368 8093D038 4405B000 */  mfc1    $a1, $f22
/* 0436C 8093D03C 0C00B58B */  jal     Actor_SetScale

/* 04370 8093D040 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 04374 8093D044 3C100604 */  lui     $s0, 0x0604                ## $s0 = 06040000
/* 04378 8093D048 26108E2C */  addiu   $s0, $s0, 0x8E2C           ## $s0 = 06038E2C
/* 0437C 8093D04C 4406B000 */  mfc1    $a2, $f22
/* 04380 8093D050 02002825 */  or      $a1, $s0, $zero            ## $a1 = 06038E2C
/* 04384 8093D054 0C029490 */  jal     SkelAnime_ChangeAnimTransitionStop
/* 04388 8093D058 26240568 */  addiu   $a0, $s1, 0x0568           ## $a0 = 00000568
/* 0438C 8093D05C 0C028800 */  jal     SkelAnime_GetFrameCount

/* 04390 8093D060 02002025 */  or      $a0, $s0, $zero            ## $a0 = 06038E2C
/* 04394 8093D064 44828000 */  mtc1    $v0, $f16                  ## $f16 = 0.00
/* 04398 8093D068 24190032 */  addiu   $t9, $zero, 0x0032         ## $t9 = 00000032
/* 0439C 8093D06C A6390178 */  sh      $t9, 0x0178($s1)           ## 00000178
/* 043A0 8093D070 468084A0 */  cvt.s.w $f18, $f16
/* 043A4 8093D074 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 043A8 8093D078 02202825 */  or      $a1, $s1, $zero            ## $a1 = 00000000
/* 043AC 8093D07C 24060002 */  addiu   $a2, $zero, 0x0002         ## $a2 = 00000002
/* 043B0 8093D080 0C00B7D5 */  jal     func_8002DF54
/* 043B4 8093D084 E63201A4 */  swc1    $f18, 0x01A4($s1)          ## 000001A4
/* 043B8 8093D088 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 043BC 8093D08C 0C00BE0A */  jal     Audio_PlayActorSound2

/* 043C0 8093D090 2405390D */  addiu   $a1, $zero, 0x390D         ## $a1 = 0000390D
/* 043C4 8093D094 0C03E803 */  jal     Audio_SetBGM

/* 043C8 8093D098 2404001B */  addiu   $a0, $zero, 0x001B         ## $a0 = 0000001B
.L8093D09C:
/* 043CC 8093D09C 3C028095 */  lui     $v0, %hi(koumePtr)       ## $v0 = 80950000
/* 043D0 8093D0A0 8C42C848 */  lw      $v0, %lo(koumePtr)($v0)
/* 043D4 8093D0A4 3C088095 */  lui     $t0, %hi(koumePtr)       ## $t0 = 80950000
/* 043D8 8093D0A8 3C098095 */  lui     $t1, %hi(kotakePtr)       ## $t1 = 80950000
/* 043DC 8093D0AC C4400050 */  lwc1    $f0, 0x0050($v0)           ## 80950050
/* 043E0 8093D0B0 3C0A8095 */  lui     $t2, %hi(kotakePtr)       ## $t2 = 80950000
/* 043E4 8093D0B4 3C0B8095 */  lui     $t3, %hi(kotakePtr)       ## $t3 = 80950000
/* 043E8 8093D0B8 E4400058 */  swc1    $f0, 0x0058($v0)           ## 80950058
/* 043EC 8093D0BC 8D08C848 */  lw      $t0, %lo(koumePtr)($t0)
/* 043F0 8093D0C0 E5000054 */  swc1    $f0, 0x0054($t0)           ## 80950054
/* 043F4 8093D0C4 8D29C844 */  lw      $t1, %lo(kotakePtr)($t1)
/* 043F8 8093D0C8 E5200058 */  swc1    $f0, 0x0058($t1)           ## 80950058
/* 043FC 8093D0CC 8D4AC844 */  lw      $t2, %lo(kotakePtr)($t2)
/* 04400 8093D0D0 E5400054 */  swc1    $f0, 0x0054($t2)           ## 80950054
/* 04404 8093D0D4 8D6BC844 */  lw      $t3, %lo(kotakePtr)($t3)
/* 04408 8093D0D8 100000B7 */  beq     $zero, $zero, .L8093D3B8
/* 0440C 8093D0DC E5600050 */  swc1    $f0, 0x0050($t3)           ## 80950050
.L8093D0E0:
/* 04410 8093D0E0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 06038E2C
/* 04414 8093D0E4 0C0295B2 */  jal     func_800A56C8
/* 04418 8093D0E8 8E2501A4 */  lw      $a1, 0x01A4($s1)           ## 000001A4
/* 0441C 8093D0EC 10400005 */  beq     $v0, $zero, .L8093D104
/* 04420 8093D0F0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 06038E2C
/* 04424 8093D0F4 3C050603 */  lui     $a1, 0x0603                ## $a1 = 06030000
/* 04428 8093D0F8 24A52BF8 */  addiu   $a1, $a1, 0x2BF8           ## $a1 = 06032BF8
/* 0442C 8093D0FC 0C0294D3 */  jal     SkelAnime_ChangeAnimTransitionRepeat
/* 04430 8093D100 3C06C170 */  lui     $a2, 0xC170                ## $a2 = C1700000
.L8093D104:
/* 04434 8093D104 240CFFFF */  addiu   $t4, $zero, 0xFFFF         ## $t4 = FFFFFFFF
/* 04438 8093D108 3C018095 */  lui     $at, %hi(D_8094C840)       ## $at = 80950000
/* 0443C 8093D10C A02CC840 */  sb      $t4, %lo(D_8094C840)($at)
/* 04440 8093D110 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 04444 8093D114 00320821 */  addu    $at, $at, $s2
/* 04448 8093D118 240D0004 */  addiu   $t5, $zero, 0x0004         ## $t5 = 00000004
/* 0444C 8093D11C A02D0AE1 */  sb      $t5, 0x0AE1($at)           ## 00010AE1
/* 04450 8093D120 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 04454 8093D124 34210AFC */  ori     $at, $at, 0x0AFC           ## $at = 00010AFC
/* 04458 8093D128 4405A000 */  mfc1    $a1, $f20
/* 0445C 8093D12C 4406A000 */  mfc1    $a2, $f20
/* 04460 8093D130 3C073DCC */  lui     $a3, 0x3DCC                ## $a3 = 3DCC0000
/* 04464 8093D134 34E7CCCD */  ori     $a3, $a3, 0xCCCD           ## $a3 = 3DCCCCCD
/* 04468 8093D138 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 0446C 8093D13C 02412021 */  addu    $a0, $s2, $at
.L8093D140:
/* 04470 8093D140 0C02927F */  jal     SkelAnime_FrameUpdateMatrix

/* 04474 8093D144 26240568 */  addiu   $a0, $s1, 0x0568           ## $a0 = 00000568
/* 04478 8093D148 3C053BE5 */  lui     $a1, 0x3BE5                ## $a1 = 3BE50000
/* 0447C 8093D14C 4406A000 */  mfc1    $a2, $f20
/* 04480 8093D150 3C073A37 */  lui     $a3, 0x3A37                ## $a3 = 3A370000
/* 04484 8093D154 34E78033 */  ori     $a3, $a3, 0x8033           ## $a3 = 3A378033
/* 04488 8093D158 34A56040 */  ori     $a1, $a1, 0x6040           ## $a1 = 3BE56040
/* 0448C 8093D15C 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 04490 8093D160 26240050 */  addiu   $a0, $s1, 0x0050           ## $a0 = 00000050
/* 04494 8093D164 C6200050 */  lwc1    $f0, 0x0050($s1)           ## 00000050
/* 04498 8093D168 862E0178 */  lh      $t6, 0x0178($s1)           ## 00000178
/* 0449C 8093D16C 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 044A0 8093D170 E6200058 */  swc1    $f0, 0x0058($s1)           ## 00000058
/* 044A4 8093D174 15C10026 */  bne     $t6, $at, .L8093D210
/* 044A8 8093D178 E6200054 */  swc1    $f0, 0x0054($s1)           ## 00000054
/* 044AC 8093D17C 8FA700A0 */  lw      $a3, 0x00A0($sp)
/* 044B0 8093D180 3C014370 */  lui     $at, 0x4370                ## $at = 43700000
/* 044B4 8093D184 44812000 */  mtc1    $at, $f4                   ## $f4 = 240.00
/* 044B8 8093D188 240F0002 */  addiu   $t7, $zero, 0x0002         ## $t7 = 00000002
/* 044BC 8093D18C 24180041 */  addiu   $t8, $zero, 0x0041         ## $t8 = 00000041
/* 044C0 8093D190 2419005A */  addiu   $t9, $zero, 0x005A         ## $t9 = 0000005A
/* 044C4 8093D194 24080032 */  addiu   $t0, $zero, 0x0032         ## $t0 = 00000032
/* 044C8 8093D198 A62F05FA */  sh      $t7, 0x05FA($s1)           ## 000005FA
/* 044CC 8093D19C E636069C */  swc1    $f22, 0x069C($s1)          ## 0000069C
/* 044D0 8093D1A0 A638017A */  sh      $t8, 0x017A($s1)           ## 0000017A
/* 044D4 8093D1A4 A639017C */  sh      $t9, 0x017C($s1)           ## 0000017C
/* 044D8 8093D1A8 A628017E */  sh      $t0, 0x017E($s1)           ## 0000017E
/* 044DC 8093D1AC 3C014387 */  lui     $at, 0x4387                ## $at = 43870000
/* 044E0 8093D1B0 24098000 */  addiu   $t1, $zero, 0x8000         ## $t1 = FFFF8000
/* 044E4 8093D1B4 44813000 */  mtc1    $at, $f6                   ## $f6 = 270.00
/* 044E8 8093D1B8 A4E900B6 */  sh      $t1, 0x00B6($a3)           ## 000000B6
/* 044EC 8093D1BC 84EA00B6 */  lh      $t2, 0x00B6($a3)           ## 000000B6
/* 044F0 8093D1C0 3C014391 */  lui     $at, 0x4391                ## $at = 43910000
/* 044F4 8093D1C4 44814000 */  mtc1    $at, $f8                   ## $f8 = 290.00
/* 044F8 8093D1C8 3C01435E */  lui     $at, 0x435E                ## $at = 435E0000
/* 044FC 8093D1CC 44815000 */  mtc1    $at, $f10                  ## $f10 = 222.00
/* 04500 8093D1D0 E4F60024 */  swc1    $f22, 0x0024($a3)          ## 00000024
/* 04504 8093D1D4 E4E40028 */  swc1    $f4, 0x0028($a3)           ## 00000028
/* 04508 8093D1D8 E4E6002C */  swc1    $f6, 0x002C($a3)           ## 0000002C
/* 0450C 8093D1DC A4EA0032 */  sh      $t2, 0x0032($a3)           ## 00000032
/* 04510 8093D1E0 E6360624 */  swc1    $f22, 0x0624($s1)          ## 00000624
/* 04514 8093D1E4 E6280628 */  swc1    $f8, 0x0628($s1)           ## 00000628
/* 04518 8093D1E8 E62A062C */  swc1    $f10, 0x062C($s1)          ## 0000062C
/* 0451C 8093D1EC C4F00024 */  lwc1    $f16, 0x0024($a3)          ## 00000024
/* 04520 8093D1F0 3C014258 */  lui     $at, 0x4258                ## $at = 42580000
/* 04524 8093D1F4 44812000 */  mtc1    $at, $f4                   ## $f4 = 54.00
/* 04528 8093D1F8 E6300630 */  swc1    $f16, 0x0630($s1)          ## 00000630
/* 0452C 8093D1FC C4F20028 */  lwc1    $f18, 0x0028($a3)          ## 00000028
/* 04530 8093D200 46049180 */  add.s   $f6, $f18, $f4
/* 04534 8093D204 E6260634 */  swc1    $f6, 0x0634($s1)           ## 00000634
/* 04538 8093D208 C4E8002C */  lwc1    $f8, 0x002C($a3)           ## 0000002C
/* 0453C 8093D20C E6280638 */  swc1    $f8, 0x0638($s1)           ## 00000638
.L8093D210:
/* 04540 8093D210 8623017E */  lh      $v1, 0x017E($s1)           ## 0000017E
/* 04544 8093D214 24010013 */  addiu   $at, $zero, 0x0013         ## $at = 00000013
/* 04548 8093D218 8FA700A0 */  lw      $a3, 0x00A0($sp)
/* 0454C 8093D21C 14610006 */  bne     $v1, $at, .L8093D238
/* 04550 8093D220 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 04554 8093D224 02202825 */  or      $a1, $s1, $zero            ## $a1 = 00000000
/* 04558 8093D228 0C00B7D5 */  jal     func_8002DF54
/* 0455C 8093D22C 24060005 */  addiu   $a2, $zero, 0x0005         ## $a2 = 00000005
/* 04560 8093D230 8623017E */  lh      $v1, 0x017E($s1)           ## 0000017E
/* 04564 8093D234 8FA700A0 */  lw      $a3, 0x00A0($sp)
.L8093D238:
/* 04568 8093D238 24010010 */  addiu   $at, $zero, 0x0010         ## $at = 00000010
/* 0456C 8093D23C 14610008 */  bne     $v1, $at, .L8093D260
/* 04570 8093D240 00000000 */  nop
/* 04574 8093D244 8CEB0678 */  lw      $t3, 0x0678($a3)           ## 00000678
/* 04578 8093D248 00E02025 */  or      $a0, $a3, $zero            ## $a0 = 00000000
/* 0457C 8093D24C 95650092 */  lhu     $a1, 0x0092($t3)           ## 00000092
/* 04580 8093D250 24A56816 */  addiu   $a1, $a1, 0x6816           ## $a1 = 00006816
/* 04584 8093D254 0C00BDF7 */  jal     func_8002F7DC
/* 04588 8093D258 30A5FFFF */  andi    $a1, $a1, 0xFFFF           ## $a1 = 00006816
/* 0458C 8093D25C 8623017E */  lh      $v1, 0x017E($s1)           ## 0000017E
.L8093D260:
/* 04590 8093D260 1060000C */  beq     $v1, $zero, .L8093D294
/* 04594 8093D264 28610014 */  slti    $at, $v1, 0x0014
/* 04598 8093D268 1020000A */  beq     $at, $zero, .L8093D294
/* 0459C 8093D26C 240C0001 */  addiu   $t4, $zero, 0x0001         ## $t4 = 00000001
/* 045A0 8093D270 3C063E4C */  lui     $a2, 0x3E4C                ## $a2 = 3E4C0000
/* 045A4 8093D274 A22C05F9 */  sb      $t4, 0x05F9($s1)           ## 000005F9
/* 045A8 8093D278 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3E4CCCCD
/* 045AC 8093D27C 2624062C */  addiu   $a0, $s1, 0x062C           ## $a0 = 0000062C
/* 045B0 8093D280 3C054372 */  lui     $a1, 0x4372                ## $a1 = 43720000
/* 045B4 8093D284 0C01E107 */  jal     Math_SmoothScaleMaxF

/* 045B8 8093D288 3C0742C8 */  lui     $a3, 0x42C8                ## $a3 = 42C80000
/* 045BC 8093D28C 10000003 */  beq     $zero, $zero, .L8093D29C
/* 045C0 8093D290 862D017A */  lh      $t5, 0x017A($s1)           ## 0000017A
.L8093D294:
/* 045C4 8093D294 A22005F9 */  sb      $zero, 0x05F9($s1)         ## 000005F9
/* 045C8 8093D298 862D017A */  lh      $t5, 0x017A($s1)           ## 0000017A
.L8093D29C:
/* 045CC 8093D29C 24020008 */  addiu   $v0, $zero, 0x0008         ## $v0 = 00000008
/* 045D0 8093D2A0 240439BB */  addiu   $a0, $zero, 0x39BB         ## $a0 = 000039BB
/* 045D4 8093D2A4 544D0004 */  bnel    $v0, $t5, .L8093D2B8
/* 045D8 8093D2A8 8622017C */  lh      $v0, 0x017C($s1)           ## 0000017C
/* 045DC 8093D2AC 0C01E221 */  jal     func_80078884
/* 045E0 8093D2B0 A6220162 */  sh      $v0, 0x0162($s1)           ## 00000162
/* 045E4 8093D2B4 8622017C */  lh      $v0, 0x017C($s1)           ## 0000017C
.L8093D2B8:
/* 045E8 8093D2B8 24010004 */  addiu   $at, $zero, 0x0004         ## $at = 00000004
/* 045EC 8093D2BC 240E0005 */  addiu   $t6, $zero, 0x0005         ## $t6 = 00000005
/* 045F0 8093D2C0 14410007 */  bne     $v0, $at, .L8093D2E0
/* 045F4 8093D2C4 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 045F8 8093D2C8 3C018095 */  lui     $at, %hi(D_8094C840)       ## $at = 80950000
/* 045FC 8093D2CC A020C840 */  sb      $zero, %lo(D_8094C840)($at)
/* 04600 8093D2D0 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 04604 8093D2D4 00320821 */  addu    $at, $at, $s2
/* 04608 8093D2D8 A02E0AE2 */  sb      $t6, 0x0AE2($at)           ## 00010AE2
/* 0460C 8093D2DC 8622017C */  lh      $v0, 0x017C($s1)           ## 0000017C
.L8093D2E0:
/* 04610 8093D2E0 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 04614 8093D2E4 14410034 */  bne     $v0, $at, .L8093D3B8
/* 04618 8093D2E8 00002825 */  or      $a1, $zero, $zero          ## $a1 = 00000000
/* 0461C 8093D2EC 264F1D64 */  addiu   $t7, $s2, 0x1D64           ## $t7 = 00001D64
/* 04620 8093D2F0 2638060C */  addiu   $t8, $s1, 0x060C           ## $t8 = 0000060C
/* 04624 8093D2F4 AFB80048 */  sw      $t8, 0x0048($sp)
/* 04628 8093D2F8 AFAF004C */  sw      $t7, 0x004C($sp)
/* 0462C 8093D2FC 0C030129 */  jal     Gameplay_GetCamera
/* 04630 8093D300 26300600 */  addiu   $s0, $s1, 0x0600           ## $s0 = 00000600
/* 04634 8093D304 8E080000 */  lw      $t0, 0x0000($s0)           ## 00000600
/* 04638 8093D308 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 0463C 8093D30C 00003025 */  or      $a2, $zero, $zero          ## $a2 = 00000000
/* 04640 8093D310 AC48005C */  sw      $t0, 0x005C($v0)           ## 0000005C
/* 04644 8093D314 8E190004 */  lw      $t9, 0x0004($s0)           ## 00000604
/* 04648 8093D318 AC590060 */  sw      $t9, 0x0060($v0)           ## 00000060
/* 0464C 8093D31C 8E080008 */  lw      $t0, 0x0008($s0)           ## 00000608
/* 04650 8093D320 AC480064 */  sw      $t0, 0x0064($v0)           ## 00000064
/* 04654 8093D324 8E0A0000 */  lw      $t2, 0x0000($s0)           ## 00000600
/* 04658 8093D328 AC4A0074 */  sw      $t2, 0x0074($v0)           ## 00000074
/* 0465C 8093D32C 8E090004 */  lw      $t1, 0x0004($s0)           ## 00000604
/* 04660 8093D330 AC490078 */  sw      $t1, 0x0078($v0)           ## 00000078
/* 04664 8093D334 8E0A0008 */  lw      $t2, 0x0008($s0)           ## 00000608
/* 04668 8093D338 AC4A007C */  sw      $t2, 0x007C($v0)           ## 0000007C
/* 0466C 8093D33C 8FAB0048 */  lw      $t3, 0x0048($sp)
/* 04670 8093D340 8D6D0000 */  lw      $t5, 0x0000($t3)           ## 00000000
/* 04674 8093D344 AC4D0050 */  sw      $t5, 0x0050($v0)           ## 00000050
/* 04678 8093D348 8D6C0004 */  lw      $t4, 0x0004($t3)           ## 00000004
/* 0467C 8093D34C AC4C0054 */  sw      $t4, 0x0054($v0)           ## 00000054
/* 04680 8093D350 8D6D0008 */  lw      $t5, 0x0008($t3)           ## 00000008
/* 04684 8093D354 AC4D0058 */  sw      $t5, 0x0058($v0)           ## 00000058
/* 04688 8093D358 0C03022B */  jal     func_800C08AC
/* 0468C 8093D35C 862505FC */  lh      $a1, 0x05FC($s1)           ## 000005FC
/* 04690 8093D360 A62005FC */  sh      $zero, 0x05FC($s1)         ## 000005FC
/* 04694 8093D364 862E05FC */  lh      $t6, 0x05FC($s1)           ## 000005FC
/* 04698 8093D368 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 0469C 8093D36C A62E05FA */  sh      $t6, 0x05FA($s1)           ## 000005FA
/* 046A0 8093D370 0C01914D */  jal     func_80064534
/* 046A4 8093D374 8FA5004C */  lw      $a1, 0x004C($sp)
/* 046A8 8093D378 02402025 */  or      $a0, $s2, $zero            ## $a0 = 00000000
/* 046AC 8093D37C 02202825 */  or      $a1, $s1, $zero            ## $a1 = 00000000
/* 046B0 8093D380 0C00B7D5 */  jal     func_8002DF54
/* 046B4 8093D384 24060007 */  addiu   $a2, $zero, 0x0007         ## $a2 = 00000007
/* 046B8 8093D388 3C0F8095 */  lui     $t7, %hi(D_8094A86C)       ## $t7 = 80950000
/* 046BC 8093D38C A6200154 */  sh      $zero, 0x0154($s1)         ## 00000154
/* 046C0 8093D390 25EFA86C */  addiu   $t7, $t7, %lo(D_8094A86C)  ## $t7 = 8094A86C
/* 046C4 8093D394 8DF90000 */  lw      $t9, 0x0000($t7)           ## 8094A86C
/* 046C8 8093D398 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 046CC 8093D39C 02402825 */  or      $a1, $s2, $zero            ## $a1 = 00000000
/* 046D0 8093D3A0 AE3904B0 */  sw      $t9, 0x04B0($s1)           ## 000004B0
/* 046D4 8093D3A4 8DF80004 */  lw      $t8, 0x0004($t7)           ## 8094A870
/* 046D8 8093D3A8 AE3804B4 */  sw      $t8, 0x04B4($s1)           ## 000004B4
/* 046DC 8093D3AC 8DF90008 */  lw      $t9, 0x0008($t7)           ## 8094A874
/* 046E0 8093D3B0 0C252870 */  jal     func_8094A1C0
/* 046E4 8093D3B4 AE3904B8 */  sw      $t9, 0x04B8($s1)           ## 000004B8
.L8093D3B8:
/* 046E8 8093D3B8 8FBF003C */  lw      $ra, 0x003C($sp)
.L8093D3BC:
/* 046EC 8093D3BC D7B40020 */  ldc1    $f20, 0x0020($sp)
/* 046F0 8093D3C0 D7B60028 */  ldc1    $f22, 0x0028($sp)
/* 046F4 8093D3C4 8FB00030 */  lw      $s0, 0x0030($sp)
/* 046F8 8093D3C8 8FB10034 */  lw      $s1, 0x0034($sp)
/* 046FC 8093D3CC 8FB20038 */  lw      $s2, 0x0038($sp)
/* 04700 8093D3D0 03E00008 */  jr      $ra
/* 04704 8093D3D4 27BD00C0 */  addiu   $sp, $sp, 0x00C0           ## $sp = 00000000
