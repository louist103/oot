.late_rodata
glabel D_80B872D8
    .float 1500.0

glabel D_80B872DC
    .float 0.01

glabel D_80B872E0
    .float 0.2

glabel D_80B872E4
 .word 0x3A23D70A

.text
glabel func_80B86CA8
/* 00388 80B86CA8 27BDFF90 */  addiu   $sp, $sp, 0xFF90           ## $sp = FFFFFF90
/* 0038C 80B86CAC AFBF006C */  sw      $ra, 0x006C($sp)           
/* 00390 80B86CB0 AFB30054 */  sw      $s3, 0x0054($sp)           
/* 00394 80B86CB4 00809825 */  or      $s3, $a0, $zero            ## $s3 = 00000000
/* 00398 80B86CB8 AFBE0068 */  sw      $s8, 0x0068($sp)           
/* 0039C 80B86CBC AFB70064 */  sw      $s7, 0x0064($sp)           
/* 003A0 80B86CC0 AFB60060 */  sw      $s6, 0x0060($sp)           
/* 003A4 80B86CC4 AFB5005C */  sw      $s5, 0x005C($sp)           
/* 003A8 80B86CC8 AFB40058 */  sw      $s4, 0x0058($sp)           
/* 003AC 80B86CCC AFB20050 */  sw      $s2, 0x0050($sp)           
/* 003B0 80B86CD0 AFB1004C */  sw      $s1, 0x004C($sp)           
/* 003B4 80B86CD4 AFB00048 */  sw      $s0, 0x0048($sp)           
/* 003B8 80B86CD8 F7BC0040 */  sdc1    $f28, 0x0040($sp)          
/* 003BC 80B86CDC F7BA0038 */  sdc1    $f26, 0x0038($sp)          
/* 003C0 80B86CE0 F7B80030 */  sdc1    $f24, 0x0030($sp)          
/* 003C4 80B86CE4 F7B60028 */  sdc1    $f22, 0x0028($sp)          
/* 003C8 80B86CE8 F7B40020 */  sdc1    $f20, 0x0020($sp)          
/* 003CC 80B86CEC 0C00B638 */  jal     Actor_MoveForward
              
/* 003D0 80B86CF0 AFA50074 */  sw      $a1, 0x0074($sp)           
/* 003D4 80B86CF4 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 003D8 80B86CF8 4481E000 */  mtc1    $at, $f28                  ## $f28 = 10.00
/* 003DC 80B86CFC 44802000 */  mtc1    $zero, $f4                 ## $f4 = 0.00
/* 003E0 80B86D00 240E0005 */  addiu   $t6, $zero, 0x0005         ## $t6 = 00000005
/* 003E4 80B86D04 4406E000 */  mfc1    $a2, $f28                  
/* 003E8 80B86D08 4407E000 */  mfc1    $a3, $f28                  
/* 003EC 80B86D0C AFAE0014 */  sw      $t6, 0x0014($sp)           
/* 003F0 80B86D10 8FA40074 */  lw      $a0, 0x0074($sp)           
/* 003F4 80B86D14 02602825 */  or      $a1, $s3, $zero            ## $a1 = 00000000
/* 003F8 80B86D18 0C00B92D */  jal     func_8002E4B4              
/* 003FC 80B86D1C E7A40010 */  swc1    $f4, 0x0010($sp)           
/* 00400 80B86D20 0C01DE1C */  jal     Math_Sins
              ## sins?
/* 00404 80B86D24 866400B4 */  lh      $a0, 0x00B4($s3)           ## 000000B4
/* 00408 80B86D28 44803000 */  mtc1    $zero, $f6                 ## $f6 = 0.00
/* 0040C 80B86D2C 00000000 */  nop
/* 00410 80B86D30 4600303E */  c.le.s  $f6, $f0                   
/* 00414 80B86D34 00000000 */  nop
/* 00418 80B86D38 45000005 */  bc1f    .L80B86D50                 
/* 0041C 80B86D3C 00000000 */  nop
/* 00420 80B86D40 0C01DE1C */  jal     Math_Sins
              ## sins?
/* 00424 80B86D44 866400B4 */  lh      $a0, 0x00B4($s3)           ## 000000B4
/* 00428 80B86D48 10000004 */  beq     $zero, $zero, .L80B86D5C   
/* 0042C 80B86D4C 46000086 */  mov.s   $f2, $f0                   
.L80B86D50:
/* 00430 80B86D50 0C01DE1C */  jal     Math_Sins
              ## sins?
/* 00434 80B86D54 866400B4 */  lh      $a0, 0x00B4($s3)           ## 000000B4
/* 00438 80B86D58 46000087 */  neg.s   $f2, $f0                   
.L80B86D5C:
/* 0043C 80B86D5C 3C0180B8 */  lui     $at, %hi(D_80B872D8)       ## $at = 80B80000
/* 00440 80B86D60 C42872D8 */  lwc1    $f8, %lo(D_80B872D8)($at)  
/* 00444 80B86D64 3C0180B8 */  lui     $at, %hi(D_80B872DC)       ## $at = 80B80000
/* 00448 80B86D68 3C1780B8 */  lui     $s7, %hi(D_80B87240)       ## $s7 = 80B80000
/* 0044C 80B86D6C 46081282 */  mul.s   $f10, $f2, $f8             
/* 00450 80B86D70 3C1680B8 */  lui     $s6, %hi(D_80B871F4)       ## $s6 = 80B80000
/* 00454 80B86D74 3C1580B8 */  lui     $s5, %hi(D_80B87200)       ## $s5 = 80B80000
/* 00458 80B86D78 26B57200 */  addiu   $s5, $s5, %lo(D_80B87200)  ## $s5 = 80B87200
/* 0045C 80B86D7C 26D671F4 */  addiu   $s6, $s6, %lo(D_80B871F4)  ## $s6 = 80B871F4
/* 00460 80B86D80 26F77240 */  addiu   $s7, $s7, %lo(D_80B87240)  ## $s7 = 80B87240
/* 00464 80B86D84 02608825 */  or      $s1, $s3, $zero            ## $s1 = 00000000
/* 00468 80B86D88 E66A00BC */  swc1    $f10, 0x00BC($s3)          ## 000000BC
/* 0046C 80B86D8C C43A72DC */  lwc1    $f26, %lo(D_80B872DC)($at) 
/* 00470 80B86D90 3C01C120 */  lui     $at, 0xC120                ## $at = C1200000
/* 00474 80B86D94 4481C000 */  mtc1    $at, $f24                  ## $f24 = -10.00
/* 00478 80B86D98 3C014170 */  lui     $at, 0x4170                ## $at = 41700000
/* 0047C 80B86D9C 4481B000 */  mtc1    $at, $f22                  ## $f22 = 15.00
/* 00480 80B86DA0 3C0180B8 */  lui     $at, %hi(D_80B872E0)       ## $at = 80B80000
/* 00484 80B86DA4 C43472E0 */  lwc1    $f20, %lo(D_80B872E0)($at) 
/* 00488 80B86DA8 00009025 */  or      $s2, $zero, $zero          ## $s2 = 00000000
/* 0048C 80B86DAC 02608025 */  or      $s0, $s3, $zero            ## $s0 = 00000000
/* 00490 80B86DB0 241E0060 */  addiu   $s8, $zero, 0x0060         ## $s8 = 00000060
/* 00494 80B86DB4 2414000F */  addiu   $s4, $zero, 0x000F         ## $s4 = 0000000F
.L80B86DB8:
/* 00498 80B86DB8 C61001A8 */  lwc1    $f16, 0x01A8($s0)          ## 000001A8
/* 0049C 80B86DBC 922F019E */  lbu     $t7, 0x019E($s1)           ## 0000019E
/* 004A0 80B86DC0 3C0180B8 */  lui     $at, %hi(D_80B871F4)       ## $at = 80B80000
/* 004A4 80B86DC4 E43071F4 */  swc1    $f16, %lo(D_80B871F4)($at) 
/* 004A8 80B86DC8 C66400BC */  lwc1    $f4, 0x00BC($s3)           ## 000000BC
/* 004AC 80B86DCC 028F1823 */  subu    $v1, $s4, $t7              
/* 004B0 80B86DD0 00031080 */  sll     $v0, $v1,  2               
/* 004B4 80B86DD4 02A2C021 */  addu    $t8, $s5, $v0              
/* 004B8 80B86DD8 461A2182 */  mul.s   $f6, $f4, $f26             
/* 004BC 80B86DDC C7000000 */  lwc1    $f0, 0x0000($t8)           ## 00000000
/* 004C0 80B86DE0 C61201AC */  lwc1    $f18, 0x01AC($s0)          ## 000001AC
/* 004C4 80B86DE4 02E24021 */  addu    $t0, $s7, $v0              
/* 004C8 80B86DE8 46180282 */  mul.s   $f10, $f0, $f24            
/* 004CC 80B86DEC 2419FFFF */  addiu   $t9, $zero, 0xFFFF         ## $t9 = FFFFFFFF
/* 004D0 80B86DF0 8FA40074 */  lw      $a0, 0x0074($sp)           
/* 004D4 80B86DF4 02602825 */  or      $a1, $s3, $zero            ## $a1 = 00000000
/* 004D8 80B86DF8 02C03025 */  or      $a2, $s6, $zero            ## $a2 = 80B871F4
/* 004DC 80B86DFC 46069200 */  add.s   $f8, $f18, $f6             
/* 004E0 80B86E00 46145402 */  mul.s   $f16, $f10, $f20           
/* 004E4 80B86E04 46088100 */  add.s   $f4, $f16, $f8             
/* 004E8 80B86E08 46140182 */  mul.s   $f6, $f0, $f20             
/* 004EC 80B86E0C E42471F8 */  swc1    $f4, %lo(D_80B871F8)($at)  
/* 004F0 80B86E10 C61201B0 */  lwc1    $f18, 0x01B0($s0)          ## 000001B0
/* 004F4 80B86E14 AFB90010 */  sw      $t9, 0x0010($sp)           
/* 004F8 80B86E18 E43271FC */  swc1    $f18, %lo(D_80B871FC)($at) 
/* 004FC 80B86E1C C50A0000 */  lwc1    $f10, 0x0000($t0)          ## 00000000
/* 00500 80B86E20 44073000 */  mfc1    $a3, $f6                   
/* 00504 80B86E24 0C00A8F1 */  jal     func_8002A3C4              
/* 00508 80B86E28 E7AA0014 */  swc1    $f10, 0x0014($sp)          
/* 0050C 80B86E2C 9222019E */  lbu     $v0, 0x019E($s1)           ## 0000019E
/* 00510 80B86E30 10400003 */  beq     $v0, $zero, .L80B86E40     
/* 00514 80B86E34 2449FFFF */  addiu   $t1, $v0, 0xFFFF           ## $t1 = FFFFFFFF
/* 00518 80B86E38 10000010 */  beq     $zero, $zero, .L80B86E7C   
/* 0051C 80B86E3C A229019E */  sb      $t1, 0x019E($s1)           ## 0000019E
.L80B86E40:
/* 00520 80B86E40 866A019A */  lh      $t2, 0x019A($s3)           ## 0000019A
/* 00524 80B86E44 240B000F */  addiu   $t3, $zero, 0x000F         ## $t3 = 0000000F
/* 00528 80B86E48 29410011 */  slti    $at, $t2, 0x0011           
/* 0052C 80B86E4C 5420000C */  bnel    $at, $zero, .L80B86E80     
/* 00530 80B86E50 2652000C */  addiu   $s2, $s2, 0x000C           ## $s2 = 0000000C
/* 00534 80B86E54 A22B019E */  sb      $t3, 0x019E($s1)           ## 0000019E
/* 00538 80B86E58 0C00CFC8 */  jal     Math_Rand_CenteredFloat
              
/* 0053C 80B86E5C 4600B306 */  mov.s   $f12, $f22                 
/* 00540 80B86E60 E60001A8 */  swc1    $f0, 0x01A8($s0)           ## 000001A8
/* 00544 80B86E64 0C00CFC8 */  jal     Math_Rand_CenteredFloat
              
/* 00548 80B86E68 4600E306 */  mov.s   $f12, $f28                 
/* 0054C 80B86E6C E60001AC */  swc1    $f0, 0x01AC($s0)           ## 000001AC
/* 00550 80B86E70 0C00CFC8 */  jal     Math_Rand_CenteredFloat
              
/* 00554 80B86E74 4600B306 */  mov.s   $f12, $f22                 
/* 00558 80B86E78 E60001B0 */  swc1    $f0, 0x01B0($s0)           ## 000001B0
.L80B86E7C:
/* 0055C 80B86E7C 2652000C */  addiu   $s2, $s2, 0x000C           ## $s2 = 00000018
.L80B86E80:
/* 00560 80B86E80 26310001 */  addiu   $s1, $s1, 0x0001           ## $s1 = 00000001
/* 00564 80B86E84 165EFFCC */  bne     $s2, $s8, .L80B86DB8       
/* 00568 80B86E88 2610000C */  addiu   $s0, $s0, 0x000C           ## $s0 = 0000000C
/* 0056C 80B86E8C 966C0088 */  lhu     $t4, 0x0088($s3)           ## 00000088
/* 00570 80B86E90 318D0001 */  andi    $t5, $t4, 0x0001           ## $t5 = 00000000
/* 00574 80B86E94 51A00024 */  beql    $t5, $zero, .L80B86F28     
/* 00578 80B86E98 8FBF006C */  lw      $ra, 0x006C($sp)           
/* 0057C 80B86E9C 866200B4 */  lh      $v0, 0x00B4($s3)           ## 000000B4
/* 00580 80B86EA0 866E0198 */  lh      $t6, 0x0198($s3)           ## 00000198
/* 00584 80B86EA4 8670019A */  lh      $s0, 0x019A($s3)           ## 0000019A
/* 00588 80B86EA8 00027843 */  sra     $t7, $v0,  1               
/* 0058C 80B86EAC 01CFC023 */  subu    $t8, $t6, $t7              
/* 00590 80B86EB0 A6780198 */  sh      $t8, 0x0198($s3)           ## 00000198
/* 00594 80B86EB4 86630198 */  lh      $v1, 0x0198($s3)           ## 00000198
/* 00598 80B86EB8 2A010008 */  slti    $at, $s0, 0x0008           
/* 0059C 80B86EBC 0003C883 */  sra     $t9, $v1,  2               
/* 005A0 80B86EC0 00794023 */  subu    $t0, $v1, $t9              
/* 005A4 80B86EC4 A6680198 */  sh      $t0, 0x0198($s3)           ## 00000198
/* 005A8 80B86EC8 86690198 */  lh      $t1, 0x0198($s3)           ## 00000198
/* 005AC 80B86ECC 00495021 */  addu    $t2, $v0, $t1              
/* 005B0 80B86ED0 1420000E */  bne     $at, $zero, .L80B86F0C     
/* 005B4 80B86ED4 A66A00B4 */  sh      $t2, 0x00B4($s3)           ## 000000B4
/* 005B8 80B86ED8 2A010018 */  slti    $at, $s0, 0x0018           
/* 005BC 80B86EDC 1020000B */  beq     $at, $zero, .L80B86F0C     
/* 005C0 80B86EE0 260BFFF8 */  addiu   $t3, $s0, 0xFFF8           ## $t3 = 00000004
/* 005C4 80B86EE4 448B8000 */  mtc1    $t3, $f16                  ## $f16 = 0.00
/* 005C8 80B86EE8 3C0180B8 */  lui     $at, %hi(D_80B872E4)       ## $at = 80B80000
/* 005CC 80B86EEC C42472E4 */  lwc1    $f4, %lo(D_80B872E4)($at)  
/* 005D0 80B86EF0 46808220 */  cvt.s.w $f8, $f16                  
/* 005D4 80B86EF4 02602025 */  or      $a0, $s3, $zero            ## $a0 = 00000000
/* 005D8 80B86EF8 46044482 */  mul.s   $f18, $f8, $f4             
/* 005DC 80B86EFC 44059000 */  mfc1    $a1, $f18                  
/* 005E0 80B86F00 0C00B58B */  jal     Actor_SetScale
              
/* 005E4 80B86F04 00000000 */  nop
/* 005E8 80B86F08 8670019A */  lh      $s0, 0x019A($s3)           ## 0000019A
.L80B86F0C:
/* 005EC 80B86F0C 12000003 */  beq     $s0, $zero, .L80B86F1C     
/* 005F0 80B86F10 260CFFFF */  addiu   $t4, $s0, 0xFFFF           ## $t4 = 0000000B
/* 005F4 80B86F14 10000003 */  beq     $zero, $zero, .L80B86F24   
/* 005F8 80B86F18 A66C019A */  sh      $t4, 0x019A($s3)           ## 0000019A
.L80B86F1C:
/* 005FC 80B86F1C 0C00B55C */  jal     Actor_Kill
              
/* 00600 80B86F20 02602025 */  or      $a0, $s3, $zero            ## $a0 = 00000000
.L80B86F24:
/* 00604 80B86F24 8FBF006C */  lw      $ra, 0x006C($sp)           
.L80B86F28:
/* 00608 80B86F28 D7B40020 */  ldc1    $f20, 0x0020($sp)          
/* 0060C 80B86F2C D7B60028 */  ldc1    $f22, 0x0028($sp)          
/* 00610 80B86F30 D7B80030 */  ldc1    $f24, 0x0030($sp)          
/* 00614 80B86F34 D7BA0038 */  ldc1    $f26, 0x0038($sp)          
/* 00618 80B86F38 D7BC0040 */  ldc1    $f28, 0x0040($sp)          
/* 0061C 80B86F3C 8FB00048 */  lw      $s0, 0x0048($sp)           
/* 00620 80B86F40 8FB1004C */  lw      $s1, 0x004C($sp)           
/* 00624 80B86F44 8FB20050 */  lw      $s2, 0x0050($sp)           
/* 00628 80B86F48 8FB30054 */  lw      $s3, 0x0054($sp)           
/* 0062C 80B86F4C 8FB40058 */  lw      $s4, 0x0058($sp)           
/* 00630 80B86F50 8FB5005C */  lw      $s5, 0x005C($sp)           
/* 00634 80B86F54 8FB60060 */  lw      $s6, 0x0060($sp)           
/* 00638 80B86F58 8FB70064 */  lw      $s7, 0x0064($sp)           
/* 0063C 80B86F5C 8FBE0068 */  lw      $s8, 0x0068($sp)           
/* 00640 80B86F60 03E00008 */  jr      $ra                        
/* 00644 80B86F64 27BD0070 */  addiu   $sp, $sp, 0x0070           ## $sp = 00000000
