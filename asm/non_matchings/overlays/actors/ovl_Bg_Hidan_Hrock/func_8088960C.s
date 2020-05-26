glabel func_8088960C
/* 004DC 8088960C 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 004E0 80889610 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 004E4 80889614 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 004E8 80889618 AFA50024 */  sw      $a1, 0x0024($sp)           
/* 004EC 8088961C 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 004F0 80889620 44813000 */  mtc1    $at, $f6                   ## $f6 = 1.00
/* 004F4 80889624 C4840060 */  lwc1    $f4, 0x0060($a0)           ## 00000060
/* 004F8 80889628 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 004FC 8088962C 24840028 */  addiu   $a0, $a0, 0x0028           ## $a0 = 00000028
/* 00500 80889630 46062200 */  add.s   $f8, $f4, $f6              
/* 00504 80889634 E4880038 */  swc1    $f8, 0x0038($a0)           ## 00000060
/* 00508 80889638 8E060060 */  lw      $a2, 0x0060($s0)           ## 00000060
/* 0050C 8088963C 0C01DE80 */  jal     Math_ApproxF
              
/* 00510 80889640 8E05000C */  lw      $a1, 0x000C($s0)           ## 0000000C
/* 00514 80889644 10400017 */  beq     $v0, $zero, .L808896A4     
/* 00518 80889648 2401FFCF */  addiu   $at, $zero, 0xFFCF         ## $at = FFFFFFCF
/* 0051C 8088964C 8E0E0004 */  lw      $t6, 0x0004($s0)           ## 00000004
/* 00520 80889650 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00524 80889654 24052835 */  addiu   $a1, $zero, 0x2835         ## $a1 = 00002835
/* 00528 80889658 01C17824 */  and     $t7, $t6, $at              
/* 0052C 8088965C 0C00BE0A */  jal     Audio_PlayActorSound2
              
/* 00530 80889660 AE0F0004 */  sw      $t7, 0x0004($s0)           ## 00000004
/* 00534 80889664 8618001C */  lh      $t8, 0x001C($s0)           ## 0000001C
/* 00538 80889668 8FB90024 */  lw      $t9, 0x0024($sp)           
/* 0053C 8088966C 3C080001 */  lui     $t0, 0x0001                ## $t0 = 00010000
/* 00540 80889670 17000009 */  bne     $t8, $zero, .L80889698     
/* 00544 80889674 01194021 */  addu    $t0, $t0, $t9              
/* 00548 80889678 81081CBC */  lb      $t0, 0x1CBC($t0)           ## 00011CBC
/* 0054C 8088967C 2402000A */  addiu   $v0, $zero, 0x000A         ## $v0 = 0000000A
/* 00550 80889680 14480003 */  bne     $v0, $t0, .L80889690       
/* 00554 80889684 00000000 */  nop
/* 00558 80889688 10000003 */  beq     $zero, $zero, .L80889698   
/* 0055C 8088968C A2020003 */  sb      $v0, 0x0003($s0)           ## 00000003
.L80889690:
/* 00560 80889690 0C00B55C */  jal     Actor_Kill
              
/* 00564 80889694 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
.L80889698:
/* 00568 80889698 3C098089 */  lui     $t1, %hi(func_808894A4)    ## $t1 = 80890000
/* 0056C 8088969C 252994A4 */  addiu   $t1, $t1, %lo(func_808894A4) ## $t1 = 808894A4
/* 00570 808896A0 AE090164 */  sw      $t1, 0x0164($s0)           ## 00000164
.L808896A4:
/* 00574 808896A4 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00578 808896A8 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 0057C 808896AC 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 00580 808896B0 03E00008 */  jr      $ra                        
/* 00584 808896B4 00000000 */  nop