glabel func_80A5E650
/* 03360 80A5E650 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 03364 80A5E654 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 03368 80A5E658 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 0336C 80A5E65C AFA5003C */  sw      $a1, 0x003C($sp)           
/* 03370 80A5E660 8CAF1C44 */  lw      $t7, 0x1C44($a1)           ## 00001C44
/* 03374 80A5E664 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 03378 80A5E668 0C296E72 */  jal     func_80A5B9C8              
/* 0337C 80A5E66C AFAF002C */  sw      $t7, 0x002C($sp)           
/* 03380 80A5E670 26040264 */  addiu   $a0, $s0, 0x0264           ## $a0 = 00000264
/* 03384 80A5E674 27A50034 */  addiu   $a1, $sp, 0x0034           ## $a1 = FFFFFFFC
/* 03388 80A5E678 0C298C52 */  jal     func_80A63148              
/* 0338C 80A5E67C 27A60032 */  addiu   $a2, $sp, 0x0032           ## $a2 = FFFFFFFA
/* 03390 80A5E680 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 03394 80A5E684 0C296FFE */  jal     func_80A5BFF8              
/* 03398 80A5E688 8FA5003C */  lw      $a1, 0x003C($sp)           
/* 0339C 80A5E68C 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 033A0 80A5E690 14410050 */  bne     $v0, $at, .L80A5E7D4       
/* 033A4 80A5E694 8FAB002C */  lw      $t3, 0x002C($sp)           
/* 033A8 80A5E698 8E180154 */  lw      $t8, 0x0154($s0)           ## 00000154
/* 033AC 80A5E69C 44806000 */  mtc1    $zero, $f12                ## $f12 = 0.00
/* 033B0 80A5E6A0 C7B00034 */  lwc1    $f16, 0x0034($sp)          
/* 033B4 80A5E6A4 44982000 */  mtc1    $t8, $f4                   ## $f4 = 0.00
/* 033B8 80A5E6A8 00000000 */  nop
/* 033BC 80A5E6AC 46802020 */  cvt.s.w $f0, $f4                   
/* 033C0 80A5E6B0 46006032 */  c.eq.s  $f12, $f0                  
/* 033C4 80A5E6B4 00000000 */  nop
/* 033C8 80A5E6B8 45030012 */  bc1tl   .L80A5E704                 
/* 033CC 80A5E6BC 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 033D0 80A5E6C0 8E190150 */  lw      $t9, 0x0150($s0)           ## 00000150
/* 033D4 80A5E6C4 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 033D8 80A5E6C8 C7B20034 */  lwc1    $f18, 0x0034($sp)          
/* 033DC 80A5E6CC 44993000 */  mtc1    $t9, $f6                   ## $f6 = 0.00
/* 033E0 80A5E6D0 00000000 */  nop
/* 033E4 80A5E6D4 468030A0 */  cvt.s.w $f2, $f6                   
/* 033E8 80A5E6D8 4602603C */  c.lt.s  $f12, $f2                  
/* 033EC 80A5E6DC 00000000 */  nop
/* 033F0 80A5E6E0 45020034 */  bc1fl   .L80A5E7B4                 
/* 033F4 80A5E6E4 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 033F8 80A5E6E8 44814000 */  mtc1    $at, $f8                   ## $f8 = 10.00
/* 033FC 80A5E6EC 00000000 */  nop
/* 03400 80A5E6F0 46080281 */  sub.s   $f10, $f0, $f8             
/* 03404 80A5E6F4 460A103C */  c.lt.s  $f2, $f10                  
/* 03408 80A5E6F8 00000000 */  nop
/* 0340C 80A5E6FC 4500002C */  bc1f    .L80A5E7B0                 
/* 03410 80A5E700 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
.L80A5E704:
/* 03414 80A5E704 44819000 */  mtc1    $at, $f18                  ## $f18 = 10.00
/* 03418 80A5E708 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 0341C 80A5E70C C7A80034 */  lwc1    $f8, 0x0034($sp)           
/* 03420 80A5E710 4612803C */  c.lt.s  $f16, $f18                 
/* 03424 80A5E714 00000000 */  nop
/* 03428 80A5E718 4502000F */  bc1fl   .L80A5E758                 
/* 0342C 80A5E71C 44815000 */  mtc1    $at, $f10                  ## $f10 = 10.00
/* 03430 80A5E720 8E080150 */  lw      $t0, 0x0150($s0)           ## 00000150
/* 03434 80A5E724 44882000 */  mtc1    $t0, $f4                   ## $f4 = 0.00
/* 03438 80A5E728 00000000 */  nop
/* 0343C 80A5E72C 468021A0 */  cvt.s.w $f6, $f4                   
/* 03440 80A5E730 460C303E */  c.le.s  $f6, $f12                  
/* 03444 80A5E734 00000000 */  nop
/* 03448 80A5E738 45020007 */  bc1fl   .L80A5E758                 
/* 0344C 80A5E73C 44815000 */  mtc1    $at, $f10                  ## $f10 = 10.00
/* 03450 80A5E740 0C2973CA */  jal     func_80A5CF28              
/* 03454 80A5E744 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 03458 80A5E748 44806000 */  mtc1    $zero, $f12                ## $f12 = 0.00
/* 0345C 80A5E74C 100000A5 */  beq     $zero, $zero, .L80A5E9E4   
/* 03460 80A5E750 E60C0068 */  swc1    $f12, 0x0068($s0)          ## 00000068
/* 03464 80A5E754 44815000 */  mtc1    $at, $f10                  ## $f10 = 0.00
.L80A5E758:
/* 03468 80A5E758 24098001 */  addiu   $t1, $zero, 0x8001         ## $t1 = FFFF8001
/* 0346C 80A5E75C 460A403C */  c.lt.s  $f8, $f10                  
/* 03470 80A5E760 00000000 */  nop
/* 03474 80A5E764 45000003 */  bc1f    .L80A5E774                 
/* 03478 80A5E768 00000000 */  nop
/* 0347C 80A5E76C 10000025 */  beq     $zero, $zero, .L80A5E804   
/* 03480 80A5E770 A7A90032 */  sh      $t1, 0x0032($sp)           
.L80A5E774:
/* 03484 80A5E774 0C01DE0D */  jal     Math_Coss
              ## coss?
/* 03488 80A5E778 87A40032 */  lh      $a0, 0x0032($sp)           
/* 0348C 80A5E77C 3C01BF00 */  lui     $at, 0xBF00                ## $at = BF000000
/* 03490 80A5E780 44818000 */  mtc1    $at, $f16                  ## $f16 = -0.50
/* 03494 80A5E784 44806000 */  mtc1    $zero, $f12                ## $f12 = 0.00
/* 03498 80A5E788 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0349C 80A5E78C 4600803C */  c.lt.s  $f16, $f0                  
/* 034A0 80A5E790 00000000 */  nop
/* 034A4 80A5E794 4502001C */  bc1fl   .L80A5E808                 
/* 034A8 80A5E798 3C01C496 */  lui     $at, 0xC496                ## $at = C4960000
/* 034AC 80A5E79C 0C2973CA */  jal     func_80A5CF28              
/* 034B0 80A5E7A0 AE000154 */  sw      $zero, 0x0154($s0)         ## 00000154
/* 034B4 80A5E7A4 44806000 */  mtc1    $zero, $f12                ## $f12 = 0.00
/* 034B8 80A5E7A8 1000008E */  beq     $zero, $zero, .L80A5E9E4   
/* 034BC 80A5E7AC E60C0068 */  swc1    $f12, 0x0068($s0)          ## 00000068
.L80A5E7B0:
/* 034C0 80A5E7B0 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
.L80A5E7B4:
/* 034C4 80A5E7B4 44812000 */  mtc1    $at, $f4                   ## $f4 = 10.00
/* 034C8 80A5E7B8 240A8001 */  addiu   $t2, $zero, 0x8001         ## $t2 = FFFF8001
/* 034CC 80A5E7BC 4604903C */  c.lt.s  $f18, $f4                  
/* 034D0 80A5E7C0 00000000 */  nop
/* 034D4 80A5E7C4 45020010 */  bc1fl   .L80A5E808                 
/* 034D8 80A5E7C8 3C01C496 */  lui     $at, 0xC496                ## $at = C4960000
/* 034DC 80A5E7CC 1000000D */  beq     $zero, $zero, .L80A5E804   
/* 034E0 80A5E7D0 A7AA0032 */  sh      $t2, 0x0032($sp)           
.L80A5E7D4:
/* 034E4 80A5E7D4 8D6C0004 */  lw      $t4, 0x0004($t3)           ## 00000004
/* 034E8 80A5E7D8 240E8001 */  addiu   $t6, $zero, 0x8001         ## $t6 = FFFF8001
/* 034EC 80A5E7DC 318D0100 */  andi    $t5, $t4, 0x0100           ## $t5 = 00000000
/* 034F0 80A5E7E0 51A00007 */  beql    $t5, $zero, .L80A5E800     
/* 034F4 80A5E7E4 44806000 */  mtc1    $zero, $f12                ## $f12 = 0.00
/* 034F8 80A5E7E8 0C2973CA */  jal     func_80A5CF28              
/* 034FC 80A5E7EC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 03500 80A5E7F0 44806000 */  mtc1    $zero, $f12                ## $f12 = 0.00
/* 03504 80A5E7F4 1000007B */  beq     $zero, $zero, .L80A5E9E4   
/* 03508 80A5E7F8 E60C0068 */  swc1    $f12, 0x0068($s0)          ## 00000068
/* 0350C 80A5E7FC 44806000 */  mtc1    $zero, $f12                ## $f12 = 0.00
.L80A5E800:
/* 03510 80A5E800 A7AE0032 */  sh      $t6, 0x0032($sp)           
.L80A5E804:
/* 03514 80A5E804 3C01C496 */  lui     $at, 0xC496                ## $at = C4960000
.L80A5E808:
/* 03518 80A5E808 44811000 */  mtc1    $at, $f2                   ## $f2 = -1200.00
/* 0351C 80A5E80C 3C01C000 */  lui     $at, 0xC000                ## $at = C0000000
/* 03520 80A5E810 44813000 */  mtc1    $at, $f6                   ## $f6 = -2.00
/* 03524 80A5E814 24187FFF */  addiu   $t8, $zero, 0x7FFF         ## $t8 = 00007FFF
/* 03528 80A5E818 3C014496 */  lui     $at, 0x4496                ## $at = 44960000
/* 0352C 80A5E81C E6060068 */  swc1    $f6, 0x0068($s0)           ## 00000068
/* 03530 80A5E820 87AF0032 */  lh      $t7, 0x0032($sp)           
/* 03534 80A5E824 030FC823 */  subu    $t9, $t8, $t7              
/* 03538 80A5E828 00194400 */  sll     $t0, $t9, 16               
/* 0353C 80A5E82C 00084C03 */  sra     $t1, $t0, 16               
/* 03540 80A5E830 44894000 */  mtc1    $t1, $f8                   ## $f8 = 0.00
/* 03544 80A5E834 00000000 */  nop
/* 03548 80A5E838 46804020 */  cvt.s.w $f0, $f8                   
/* 0354C 80A5E83C 4602003C */  c.lt.s  $f0, $f2                   
/* 03550 80A5E840 00000000 */  nop
/* 03554 80A5E844 45020008 */  bc1fl   .L80A5E868                 
/* 03558 80A5E848 44817000 */  mtc1    $at, $f14                  ## $f14 = 1200.00
/* 0355C 80A5E84C 4600128D */  trunc.w.s $f10, $f2                  
/* 03560 80A5E850 44035000 */  mfc1    $v1, $f10                  
/* 03564 80A5E854 00000000 */  nop
/* 03568 80A5E858 00031C00 */  sll     $v1, $v1, 16               
/* 0356C 80A5E85C 1000000F */  beq     $zero, $zero, .L80A5E89C   
/* 03570 80A5E860 00031C03 */  sra     $v1, $v1, 16               
/* 03574 80A5E864 44817000 */  mtc1    $at, $f14                  ## $f14 = 1200.00
.L80A5E868:
/* 03578 80A5E868 00000000 */  nop
/* 0357C 80A5E86C 4600703C */  c.lt.s  $f14, $f0                  
/* 03580 80A5E870 00000000 */  nop
/* 03584 80A5E874 45020004 */  bc1fl   .L80A5E888                 
/* 03588 80A5E878 46000086 */  mov.s   $f2, $f0                   
/* 0358C 80A5E87C 10000002 */  beq     $zero, $zero, .L80A5E888   
/* 03590 80A5E880 46007086 */  mov.s   $f2, $f14                  
/* 03594 80A5E884 46000086 */  mov.s   $f2, $f0                   
.L80A5E888:
/* 03598 80A5E888 4600140D */  trunc.w.s $f16, $f2                  
/* 0359C 80A5E88C 44038000 */  mfc1    $v1, $f16                  
/* 035A0 80A5E890 00000000 */  nop
/* 035A4 80A5E894 00031C00 */  sll     $v1, $v1, 16               
/* 035A8 80A5E898 00031C03 */  sra     $v1, $v1, 16               
.L80A5E89C:
/* 035AC 80A5E89C 8E020150 */  lw      $v0, 0x0150($s0)           ## 00000150
/* 035B0 80A5E8A0 860C0032 */  lh      $t4, 0x0032($s0)           ## 00000032
/* 035B4 80A5E8A4 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 035B8 80A5E8A8 44829000 */  mtc1    $v0, $f18                  ## $f18 = 0.00
/* 035BC 80A5E8AC 01836821 */  addu    $t5, $t4, $v1              
/* 035C0 80A5E8B0 A60D0032 */  sh      $t5, 0x0032($s0)           ## 00000032
/* 035C4 80A5E8B4 46809120 */  cvt.s.w $f4, $f18                  
/* 035C8 80A5E8B8 860E0032 */  lh      $t6, 0x0032($s0)           ## 00000032
/* 035CC 80A5E8BC 44818000 */  mtc1    $at, $f16                  ## $f16 = 0.50
/* 035D0 80A5E8C0 244FFFFF */  addiu   $t7, $v0, 0xFFFF           ## $t7 = FFFFFFFF
/* 035D4 80A5E8C4 A60E00B6 */  sh      $t6, 0x00B6($s0)           ## 000000B6
/* 035D8 80A5E8C8 4604603C */  c.lt.s  $f12, $f4                  
/* 035DC 80A5E8CC 00000000 */  nop
/* 035E0 80A5E8D0 4502000A */  bc1fl   .L80A5E8FC                 
/* 035E4 80A5E8D4 C60A0068 */  lwc1    $f10, 0x0068($s0)          ## 00000068
/* 035E8 80A5E8D8 448F3000 */  mtc1    $t7, $f6                   ## $f6 = NaN
/* 035EC 80A5E8DC AE0F0150 */  sw      $t7, 0x0150($s0)           ## 00000150
/* 035F0 80A5E8E0 46803220 */  cvt.s.w $f8, $f6                   
/* 035F4 80A5E8E4 460C403E */  c.le.s  $f8, $f12                  
/* 035F8 80A5E8E8 00000000 */  nop
/* 035FC 80A5E8EC 45020003 */  bc1fl   .L80A5E8FC                 
/* 03600 80A5E8F0 C60A0068 */  lwc1    $f10, 0x0068($s0)          ## 00000068
/* 03604 80A5E8F4 AE000154 */  sw      $zero, 0x0154($s0)         ## 00000154
/* 03608 80A5E8F8 C60A0068 */  lwc1    $f10, 0x0068($s0)          ## 00000068
.L80A5E8FC:
/* 0360C 80A5E8FC 3C013FC0 */  lui     $at, 0x3FC0                ## $at = 3FC00000
/* 03610 80A5E900 44812000 */  mtc1    $at, $f4                   ## $f4 = 1.50
/* 03614 80A5E904 46105482 */  mul.s   $f18, $f10, $f16           
/* 03618 80A5E908 260401AC */  addiu   $a0, $s0, 0x01AC           ## $a0 = 000001AC
/* 0361C 80A5E90C 46049182 */  mul.s   $f6, $f18, $f4             
/* 03620 80A5E910 0C02927F */  jal     SkelAnime_FrameUpdateMatrix
              
/* 03624 80A5E914 E60601C8 */  swc1    $f6, 0x01C8($s0)           ## 000001C8
/* 03628 80A5E918 44806000 */  mtc1    $zero, $f12                ## $f12 = 0.00
/* 0362C 80A5E91C 50400032 */  beql    $v0, $zero, .L80A5E9E8     
/* 03630 80A5E920 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 03634 80A5E924 8E190150 */  lw      $t9, 0x0150($s0)           ## 00000150
/* 03638 80A5E928 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0363C 80A5E92C 44994000 */  mtc1    $t9, $f8                   ## $f8 = 0.00
/* 03640 80A5E930 00000000 */  nop
/* 03644 80A5E934 468042A0 */  cvt.s.w $f10, $f8                  
/* 03648 80A5E938 460C503E */  c.le.s  $f10, $f12                 
/* 0364C 80A5E93C 00000000 */  nop
/* 03650 80A5E940 45020029 */  bc1fl   .L80A5E9E8                 
/* 03654 80A5E944 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 03658 80A5E948 0C296FFE */  jal     func_80A5BFF8              
/* 0365C 80A5E94C 8FA5003C */  lw      $a1, 0x003C($sp)           
/* 03660 80A5E950 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 03664 80A5E954 14410023 */  bne     $v0, $at, .L80A5E9E4       
/* 03668 80A5E958 C7B20034 */  lwc1    $f18, 0x0034($sp)          
/* 0366C 80A5E95C 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 03670 80A5E960 44818000 */  mtc1    $at, $f16                  ## $f16 = 10.00
/* 03674 80A5E964 00000000 */  nop
/* 03678 80A5E968 4612803C */  c.lt.s  $f16, $f18                 
/* 0367C 80A5E96C 00000000 */  nop
/* 03680 80A5E970 4502000F */  bc1fl   .L80A5E9B0                 
/* 03684 80A5E974 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 03688 80A5E978 0C01DE0D */  jal     Math_Coss
              ## coss?
/* 0368C 80A5E97C 87A40032 */  lh      $a0, 0x0032($sp)           
/* 03690 80A5E980 3C01BF00 */  lui     $at, 0xBF00                ## $at = BF000000
/* 03694 80A5E984 44812000 */  mtc1    $at, $f4                   ## $f4 = -0.50
/* 03698 80A5E988 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0369C 80A5E98C 4604003E */  c.le.s  $f0, $f4                   
/* 036A0 80A5E990 00000000 */  nop
/* 036A4 80A5E994 45020006 */  bc1fl   .L80A5E9B0                 
/* 036A8 80A5E998 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 036AC 80A5E99C 0C297962 */  jal     func_80A5E588              
/* 036B0 80A5E9A0 AE000154 */  sw      $zero, 0x0154($s0)         ## 00000154
/* 036B4 80A5E9A4 10000010 */  beq     $zero, $zero, .L80A5E9E8   
/* 036B8 80A5E9A8 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 036BC 80A5E9AC 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
.L80A5E9B0:
/* 036C0 80A5E9B0 44814000 */  mtc1    $at, $f8                   ## $f8 = 10.00
/* 036C4 80A5E9B4 C7A60034 */  lwc1    $f6, 0x0034($sp)           
/* 036C8 80A5E9B8 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 036CC 80A5E9BC 4608303C */  c.lt.s  $f6, $f8                   
/* 036D0 80A5E9C0 00000000 */  nop
/* 036D4 80A5E9C4 45000005 */  bc1f    .L80A5E9DC                 
/* 036D8 80A5E9C8 00000000 */  nop
/* 036DC 80A5E9CC 0C2973CA */  jal     func_80A5CF28              
/* 036E0 80A5E9D0 AE000154 */  sw      $zero, 0x0154($s0)         ## 00000154
/* 036E4 80A5E9D4 10000004 */  beq     $zero, $zero, .L80A5E9E8   
/* 036E8 80A5E9D8 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80A5E9DC:
/* 036EC 80A5E9DC 0C29796B */  jal     func_80A5E5AC              
/* 036F0 80A5E9E0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
.L80A5E9E4:
/* 036F4 80A5E9E4 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80A5E9E8:
/* 036F8 80A5E9E8 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 036FC 80A5E9EC 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000
/* 03700 80A5E9F0 03E00008 */  jr      $ra                        
/* 03704 80A5E9F4 00000000 */  nop
