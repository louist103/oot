glabel func_80B415B8
/* 053D8 80B415B8 3C038016 */  lui     $v1, %hi(gSaveContext)
/* 053DC 80B415BC 2463E660 */  addiu   $v1, %lo(gSaveContext)
/* 053E0 80B415C0 8C6E0004 */  lw      $t6, 0x0004($v1)           ## 8015E664
/* 053E4 80B415C4 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 053E8 80B415C8 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 053EC 80B415CC 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 053F0 80B415D0 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 053F4 80B415D4 15C00049 */  bne     $t6, $zero, .L80B416FC     
/* 053F8 80B415D8 00A03825 */  or      $a3, $a1, $zero            ## $a3 = 00000000
/* 053FC 80B415DC 94660EEC */  lhu     $a2, 0x0EEC($v1)           ## 8015F54C
/* 05400 80B415E0 3C020200 */  lui     $v0, 0x0200                ## $v0 = 02000000
/* 05404 80B415E4 244246F0 */  addiu   $v0, $v0, 0x46F0           ## $v0 = 020046F0
/* 05408 80B415E8 30CF0020 */  andi    $t7, $a2, 0x0020           ## $t7 = 00000000
/* 0540C 80B415EC 15E00014 */  bne     $t7, $zero, .L80B41640     
/* 05410 80B415F0 34D80020 */  ori     $t8, $a2, 0x0020           ## $t8 = 00000020
/* 05414 80B415F4 0002C900 */  sll     $t9, $v0,  4               
/* 05418 80B415F8 00194702 */  srl     $t0, $t9, 28               
/* 0541C 80B415FC 00084880 */  sll     $t1, $t0,  2               
/* 05420 80B41600 3C0A8016 */  lui     $t2, %hi(gSegments)
/* 05424 80B41604 A4780EEC */  sh      $t8, 0x0EEC($v1)           ## 8015F54C
/* 05428 80B41608 01495021 */  addu    $t2, $t2, $t1              
/* 0542C 80B4160C 3C0100FF */  lui     $at, 0x00FF                ## $at = 00FF0000
/* 05430 80B41610 8D4A6FA8 */  lw      $t2, %lo(gSegments)($t2)
/* 05434 80B41614 3421FFFF */  ori     $at, $at, 0xFFFF           ## $at = 00FFFFFF
/* 05438 80B41618 00415824 */  and     $t3, $v0, $at              
/* 0543C 80B4161C 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 05440 80B41620 014B6021 */  addu    $t4, $t2, $t3              
/* 05444 80B41624 01816821 */  addu    $t5, $t4, $at              
/* 05448 80B41628 ACAD1D68 */  sw      $t5, 0x1D68($a1)           ## 00001D68
/* 0544C 80B4162C 240E0001 */  addiu   $t6, $zero, 0x0001         ## $t6 = 00000001
/* 05450 80B41630 0C2CFAFC */  jal     func_80B3EBF0              
/* 05454 80B41634 A06E1414 */  sb      $t6, 0x1414($v1)           ## 8015FA74
/* 05458 80B41638 10000033 */  beq     $zero, $zero, .L80B41708   
/* 0545C 80B4163C 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80B41640:
/* 05460 80B41640 94620EDE */  lhu     $v0, 0x0EDE($v1)           ## 00000EDE
/* 05464 80B41644 30440020 */  andi    $a0, $v0, 0x0020           ## $a0 = 00000000
/* 05468 80B41648 14800022 */  bne     $a0, $zero, .L80B416D4     
/* 0546C 80B4164C 00000000 */  nop
/* 05470 80B41650 946F0EDC */  lhu     $t7, 0x0EDC($v1)           ## 00000EDC
/* 05474 80B41654 34590020 */  ori     $t9, $v0, 0x0020           ## $t9 = 00000020
/* 05478 80B41658 2405005F */  addiu   $a1, $zero, 0x005F         ## $a1 = 0000005F
/* 0547C 80B4165C 31F80100 */  andi    $t8, $t7, 0x0100           ## $t8 = 00000000
/* 05480 80B41660 1300001C */  beq     $t8, $zero, .L80B416D4     
/* 05484 80B41664 00000000 */  nop
/* 05488 80B41668 A4790EDE */  sh      $t9, 0x0EDE($v1)           ## 00000EDE
/* 0548C 80B4166C 00E02025 */  or      $a0, $a3, $zero            ## $a0 = 00000000
/* 05490 80B41670 0C021344 */  jal     Item_Give              
/* 05494 80B41674 AFA70024 */  sw      $a3, 0x0024($sp)           
/* 05498 80B41678 3C020200 */  lui     $v0, 0x0200                ## $v0 = 02000000
/* 0549C 80B4167C 24426D20 */  addiu   $v0, $v0, 0x6D20           ## $v0 = 02006D20
/* 054A0 80B41680 00024100 */  sll     $t0, $v0,  4               
/* 054A4 80B41684 00084F02 */  srl     $t1, $t0, 28               
/* 054A8 80B41688 00095080 */  sll     $t2, $t1,  2               
/* 054AC 80B4168C 3C0B8016 */  lui     $t3, %hi(gSegments)
/* 054B0 80B41690 016A5821 */  addu    $t3, $t3, $t2              
/* 054B4 80B41694 3C0100FF */  lui     $at, 0x00FF                ## $at = 00FF0000
/* 054B8 80B41698 8D6B6FA8 */  lw      $t3, %lo(gSegments)($t3)
/* 054BC 80B4169C 3421FFFF */  ori     $at, $at, 0xFFFF           ## $at = 00FFFFFF
/* 054C0 80B416A0 00416024 */  and     $t4, $v0, $at              
/* 054C4 80B416A4 8FA70024 */  lw      $a3, 0x0024($sp)           
/* 054C8 80B416A8 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 054CC 80B416AC 016C6821 */  addu    $t5, $t3, $t4              
/* 054D0 80B416B0 3C038016 */  lui     $v1, %hi(gSaveContext)
/* 054D4 80B416B4 01A17021 */  addu    $t6, $t5, $at              
/* 054D8 80B416B8 2463E660 */  addiu   $v1, %lo(gSaveContext)
/* 054DC 80B416BC 240F0001 */  addiu   $t7, $zero, 0x0001         ## $t7 = 00000001
/* 054E0 80B416C0 ACEE1D68 */  sw      $t6, 0x1D68($a3)           ## 00001D68
/* 054E4 80B416C4 A06F1414 */  sb      $t7, 0x1414($v1)           ## 8015FA74
/* 054E8 80B416C8 2418001E */  addiu   $t8, $zero, 0x001E         ## $t8 = 0000001E
/* 054EC 80B416CC 1000000D */  beq     $zero, $zero, .L80B41704   
/* 054F0 80B416D0 AE180260 */  sw      $t8, 0x0260($s0)           ## 00000260
.L80B416D4:
/* 054F4 80B416D4 14800005 */  bne     $a0, $zero, .L80B416EC     
/* 054F8 80B416D8 00000000 */  nop
/* 054FC 80B416DC 0C2CF27B */  jal     func_80B3C9EC              
/* 05500 80B416E0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 05504 80B416E4 10000008 */  beq     $zero, $zero, .L80B41708   
/* 05508 80B416E8 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80B416EC:
/* 0550C 80B416EC 0C00B55C */  jal     Actor_Kill
              
/* 05510 80B416F0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 05514 80B416F4 10000004 */  beq     $zero, $zero, .L80B41708   
/* 05518 80B416F8 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80B416FC:
/* 0551C 80B416FC 0C00B55C */  jal     Actor_Kill
              
/* 05520 80B41700 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
.L80B41704:
/* 05524 80B41704 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80B41708:
/* 05528 80B41708 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 0552C 80B4170C 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 05530 80B41710 03E00008 */  jr      $ra                        
/* 05534 80B41714 00000000 */  nop