glabel func_80B582C8
/* 04F18 80B582C8 3C038016 */  lui     $v1, %hi(gGameInfo)
/* 04F1C 80B582CC 8C63FA90 */  lw      $v1, %lo(gGameInfo)($v1)
/* 04F20 80B582D0 27BDFFD0 */  addiu   $sp, $sp, 0xFFD0           ## $sp = FFFFFFD0
/* 04F24 80B582D4 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 04F28 80B582D8 AFA50034 */  sw      $a1, 0x0034($sp)           
/* 04F2C 80B582DC 846E1470 */  lh      $t6, 0x1470($v1)           ## 80161470
/* 04F30 80B582E0 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 04F34 80B582E4 44815000 */  mtc1    $at, $f10                  ## $f10 = 10.00
/* 04F38 80B582E8 448E3000 */  mtc1    $t6, $f6                   ## $f6 = 0.00
/* 04F3C 80B582EC C48403CC */  lwc1    $f4, 0x03CC($a0)           ## 000003CC
/* 04F40 80B582F0 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 04F44 80B582F4 46803220 */  cvt.s.w $f8, $f6                   
/* 04F48 80B582F8 248203CC */  addiu   $v0, $a0, 0x03CC           ## $v0 = 000003CC
/* 04F4C 80B582FC 460A4400 */  add.s   $f16, $f8, $f10            
/* 04F50 80B58300 46102032 */  c.eq.s  $f4, $f16                  
/* 04F54 80B58304 00000000 */  nop
/* 04F58 80B58308 45020012 */  bc1fl   .L80B58354                 
/* 04F5C 80B5830C 846F1472 */  lh      $t7, 0x1472($v1)           ## 80161472
/* 04F60 80B58310 44816000 */  mtc1    $at, $f12                  ## $f12 = 1.00
/* 04F64 80B58314 C4520000 */  lwc1    $f18, 0x0000($v0)          ## 000003CC
/* 04F68 80B58318 3C050601 */  lui     $a1, 0x0601                ## $a1 = 06010000
/* 04F6C 80B5831C 24A58050 */  addiu   $a1, $a1, 0x8050           ## $a1 = 06008050
/* 04F70 80B58320 460C9180 */  add.s   $f6, $f18, $f12            
/* 04F74 80B58324 00003025 */  or      $a2, $zero, $zero          ## $a2 = 00000000
/* 04F78 80B58328 3C07C140 */  lui     $a3, 0xC140                ## $a3 = C1400000
/* 04F7C 80B5832C E4460000 */  swc1    $f6, 0x0000($v0)           ## 000003CC
/* 04F80 80B58330 AFA40030 */  sw      $a0, 0x0030($sp)           
/* 04F84 80B58334 0C2D5385 */  jal     func_80B54E14              
/* 04F88 80B58338 AFA00010 */  sw      $zero, 0x0010($sp)         
/* 04F8C 80B5833C 8FA40030 */  lw      $a0, 0x0030($sp)           
/* 04F90 80B58340 0C2D5DC1 */  jal     func_80B57704              
/* 04F94 80B58344 8FA50034 */  lw      $a1, 0x0034($sp)           
/* 04F98 80B58348 10000057 */  beq     $zero, $zero, .L80B584A8   
/* 04F9C 80B5834C 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 04FA0 80B58350 846F1472 */  lh      $t7, 0x1472($v1)           ## 00001472
.L80B58354:
/* 04FA4 80B58354 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 04FA8 80B58358 44812000 */  mtc1    $at, $f4                   ## $f4 = 20.00
/* 04FAC 80B5835C 448F4000 */  mtc1    $t7, $f8                   ## $f8 = 0.00
/* 04FB0 80B58360 248203CC */  addiu   $v0, $a0, 0x03CC           ## $v0 = 000003CC
/* 04FB4 80B58364 C4400000 */  lwc1    $f0, 0x0000($v0)           ## 000003CC
/* 04FB8 80B58368 468042A0 */  cvt.s.w $f10, $f8                  
/* 04FBC 80B5836C 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 04FC0 80B58370 46045400 */  add.s   $f16, $f10, $f4            
/* 04FC4 80B58374 46100032 */  c.eq.s  $f0, $f16                  
/* 04FC8 80B58378 00000000 */  nop
/* 04FCC 80B5837C 45020011 */  bc1fl   .L80B583C4                 
/* 04FD0 80B58380 84781474 */  lh      $t8, 0x1474($v1)           ## 00001474
/* 04FD4 80B58384 44816000 */  mtc1    $at, $f12                  ## $f12 = 1.00
/* 04FD8 80B58388 00000000 */  nop
/* 04FDC 80B5838C 460C0480 */  add.s   $f18, $f0, $f12            
/* 04FE0 80B58390 E4520000 */  swc1    $f18, 0x0000($v0)          ## 000003CC
/* 04FE4 80B58394 0C2D5B72 */  jal     func_80B56DC8              
/* 04FE8 80B58398 AFA40030 */  sw      $a0, 0x0030($sp)           
/* 04FEC 80B5839C 3C050600 */  lui     $a1, 0x0600                ## $a1 = 06000000
/* 04FF0 80B583A0 8FA40030 */  lw      $a0, 0x0030($sp)           
/* 04FF4 80B583A4 24A53FF8 */  addiu   $a1, $a1, 0x3FF8           ## $a1 = 06003FF8
/* 04FF8 80B583A8 00003025 */  or      $a2, $zero, $zero          ## $a2 = 00000000
/* 04FFC 80B583AC 3C07C140 */  lui     $a3, 0xC140                ## $a3 = C1400000
/* 05000 80B583B0 0C2D5385 */  jal     func_80B54E14              
/* 05004 80B583B4 AFA00010 */  sw      $zero, 0x0010($sp)         
/* 05008 80B583B8 1000003B */  beq     $zero, $zero, .L80B584A8   
/* 0500C 80B583BC 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 05010 80B583C0 84781474 */  lh      $t8, 0x1474($v1)           ## 00001474
.L80B583C4:
/* 05014 80B583C4 3C0141F0 */  lui     $at, 0x41F0                ## $at = 41F00000
/* 05018 80B583C8 44815000 */  mtc1    $at, $f10                  ## $f10 = 30.00
/* 0501C 80B583CC 44983000 */  mtc1    $t8, $f6                   ## $f6 = 0.00
/* 05020 80B583D0 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 05024 80B583D4 46803220 */  cvt.s.w $f8, $f6                   
/* 05028 80B583D8 460A4100 */  add.s   $f4, $f8, $f10             
/* 0502C 80B583DC 46040032 */  c.eq.s  $f0, $f4                   
/* 05030 80B583E0 00000000 */  nop
/* 05034 80B583E4 4502000A */  bc1fl   .L80B58410                 
/* 05038 80B583E8 84791476 */  lh      $t9, 0x1476($v1)           ## 00001476
/* 0503C 80B583EC 44816000 */  mtc1    $at, $f12                  ## $f12 = 1.00
/* 05040 80B583F0 00000000 */  nop
/* 05044 80B583F4 460C0400 */  add.s   $f16, $f0, $f12            
/* 05048 80B583F8 E4500000 */  swc1    $f16, 0x0000($v0)          ## 00000000
/* 0504C 80B583FC 0C2D5E16 */  jal     func_80B57858              
/* 05050 80B58400 8FA40034 */  lw      $a0, 0x0034($sp)           
/* 05054 80B58404 10000028 */  beq     $zero, $zero, .L80B584A8   
/* 05058 80B58408 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 0505C 80B5840C 84791476 */  lh      $t9, 0x1476($v1)           ## 00001476
.L80B58410:
/* 05060 80B58410 3C014220 */  lui     $at, 0x4220                ## $at = 42200000
/* 05064 80B58414 44814000 */  mtc1    $at, $f8                   ## $f8 = 40.00
/* 05068 80B58418 44999000 */  mtc1    $t9, $f18                  ## $f18 = 0.00
/* 0506C 80B5841C 8FA80034 */  lw      $t0, 0x0034($sp)           
/* 05070 80B58420 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 05074 80B58424 468091A0 */  cvt.s.w $f6, $f18                  
/* 05078 80B58428 46083080 */  add.s   $f2, $f6, $f8              
/* 0507C 80B5842C 46020032 */  c.eq.s  $f0, $f2                   
/* 05080 80B58430 00000000 */  nop
/* 05084 80B58434 4502000F */  bc1fl   .L80B58474                 
/* 05088 80B58438 44816000 */  mtc1    $at, $f12                  ## $f12 = 1.00
/* 0508C 80B5843C 850907A0 */  lh      $t1, 0x07A0($t0)           ## 000007A0
/* 05090 80B58440 00095080 */  sll     $t2, $t1,  2               
/* 05094 80B58444 010A5821 */  addu    $t3, $t0, $t2              
/* 05098 80B58448 8D640790 */  lw      $a0, 0x0790($t3)           ## 00000790
/* 0509C 80B5844C 0C016C69 */  jal     func_8005B1A4              
/* 050A0 80B58450 AFA20024 */  sw      $v0, 0x0024($sp)           
/* 050A4 80B58454 8FA20024 */  lw      $v0, 0x0024($sp)           
/* 050A8 80B58458 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 050AC 80B5845C 44816000 */  mtc1    $at, $f12                  ## $f12 = 1.00
/* 050B0 80B58460 C44A0000 */  lwc1    $f10, 0x0000($v0)          ## 00000000
/* 050B4 80B58464 460C5100 */  add.s   $f4, $f10, $f12            
/* 050B8 80B58468 1000000E */  beq     $zero, $zero, .L80B584A4   
/* 050BC 80B5846C E4440000 */  swc1    $f4, 0x0000($v0)           ## 00000000
/* 050C0 80B58470 44816000 */  mtc1    $at, $f12                  ## $f12 = 1.00
.L80B58474:
/* 050C4 80B58474 240C0020 */  addiu   $t4, $zero, 0x0020         ## $t4 = 00000020
/* 050C8 80B58478 460C1400 */  add.s   $f16, $f2, $f12            
/* 050CC 80B5847C 4600803E */  c.le.s  $f16, $f0                  
/* 050D0 80B58480 00000000 */  nop
/* 050D4 80B58484 45020006 */  bc1fl   .L80B584A0                 
/* 050D8 80B58488 460C0180 */  add.s   $f6, $f0, $f12             
/* 050DC 80B5848C 44809000 */  mtc1    $zero, $f18                ## $f18 = 0.00
/* 050E0 80B58490 AC8C024C */  sw      $t4, 0x024C($a0)           ## 0000024C
/* 050E4 80B58494 10000003 */  beq     $zero, $zero, .L80B584A4   
/* 050E8 80B58498 E4520000 */  swc1    $f18, 0x0000($v0)          ## 00000000
/* 050EC 80B5849C 460C0180 */  add.s   $f6, $f0, $f12             
.L80B584A0:
/* 050F0 80B584A0 E4460000 */  swc1    $f6, 0x0000($v0)           ## 00000000
.L80B584A4:
/* 050F4 80B584A4 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80B584A8:
/* 050F8 80B584A8 27BD0030 */  addiu   $sp, $sp, 0x0030           ## $sp = 00000000
/* 050FC 80B584AC 03E00008 */  jr      $ra                        
/* 05100 80B584B0 00000000 */  nop