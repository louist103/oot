.late_rodata
glabel D_809E29CC
 .word 0x4622F983
glabel D_809E29D0
 .word 0x41270A3D

.text
glabel func_809E1F20
/* 00670 809E1F20 27BDFF88 */  addiu   $sp, $sp, 0xFF88           ## $sp = FFFFFF88
/* 00674 809E1F24 AFBF004C */  sw      $ra, 0x004C($sp)           
/* 00678 809E1F28 AFB20048 */  sw      $s2, 0x0048($sp)           
/* 0067C 809E1F2C AFB10044 */  sw      $s1, 0x0044($sp)           
/* 00680 809E1F30 AFB00040 */  sw      $s0, 0x0040($sp)           
/* 00684 809E1F34 AFA5007C */  sw      $a1, 0x007C($sp)           
/* 00688 809E1F38 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 0068C 809E1F3C F7BA0038 */  sdc1    $f26, 0x0038($sp)          
/* 00690 809E1F40 F7B80030 */  sdc1    $f24, 0x0030($sp)          
/* 00694 809E1F44 F7B60028 */  sdc1    $f22, 0x0028($sp)          
/* 00698 809E1F48 F7B40020 */  sdc1    $f20, 0x0020($sp)          
/* 0069C 809E1F4C 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 006A0 809E1F50 27B20064 */  addiu   $s2, $sp, 0x0064           ## $s2 = FFFFFFEC
/* 006A4 809E1F54 00A18821 */  addu    $s1, $a1, $at              
/* 006A8 809E1F58 8E241E08 */  lw      $a0, 0x1E08($s1)           ## 00001E08
/* 006AC 809E1F5C 02402825 */  or      $a1, $s2, $zero            ## $a1 = FFFFFFEC
/* 006B0 809E1F60 8E0701FC */  lw      $a3, 0x01FC($s0)           ## 000001FC
/* 006B4 809E1F64 0C2787A4 */  jal     func_809E1E90              
/* 006B8 809E1F68 8E0601F4 */  lw      $a2, 0x01F4($s0)           ## 000001F4
/* 006BC 809E1F6C C7A40064 */  lwc1    $f4, 0x0064($sp)           
/* 006C0 809E1F70 C6060024 */  lwc1    $f6, 0x0024($s0)           ## 00000024
/* 006C4 809E1F74 C7A8006C */  lwc1    $f8, 0x006C($sp)           
/* 006C8 809E1F78 C60A002C */  lwc1    $f10, 0x002C($s0)          ## 0000002C
/* 006CC 809E1F7C 46062501 */  sub.s   $f20, $f4, $f6             
/* 006D0 809E1F80 460A4581 */  sub.s   $f22, $f8, $f10            
/* 006D4 809E1F84 4600A306 */  mov.s   $f12, $f20                 
/* 006D8 809E1F88 0C03F494 */  jal     Math_atan2f              
/* 006DC 809E1F8C 4600B386 */  mov.s   $f14, $f22                 
/* 006E0 809E1F90 3C01809E */  lui     $at, %hi(D_809E29CC)       ## $at = 809E0000
/* 006E4 809E1F94 C43829CC */  lwc1    $f24, %lo(D_809E29CC)($at) 
/* 006E8 809E1F98 3C01809E */  lui     $at, %hi(D_809E29D0)       ## $at = 809E0000
/* 006EC 809E1F9C C43A29D0 */  lwc1    $f26, %lo(D_809E29D0)($at) 
/* 006F0 809E1FA0 46180402 */  mul.s   $f16, $f0, $f24            
/* 006F4 809E1FA4 00000000 */  nop
/* 006F8 809E1FA8 4614A202 */  mul.s   $f8, $f20, $f20            
/* 006FC 809E1FAC 00000000 */  nop
/* 00700 809E1FB0 4616B282 */  mul.s   $f10, $f22, $f22           
/* 00704 809E1FB4 4600848D */  trunc.w.s $f18, $f16                 
/* 00708 809E1FB8 460A4000 */  add.s   $f0, $f8, $f10             
/* 0070C 809E1FBC 440F9000 */  mfc1    $t7, $f18                  
/* 00710 809E1FC0 46000004 */  sqrt.s  $f0, $f0                   
/* 00714 809E1FC4 000FC400 */  sll     $t8, $t7, 16               
/* 00718 809E1FC8 0018CC03 */  sra     $t9, $t8, 16               
/* 0071C 809E1FCC 44992000 */  mtc1    $t9, $f4                   ## $f4 = 0.00
/* 00720 809E1FD0 00000000 */  nop
/* 00724 809E1FD4 468021A0 */  cvt.s.w $f6, $f4                   
/* 00728 809E1FD8 E6000208 */  swc1    $f0, 0x0208($s0)           ## 00000208
/* 0072C 809E1FDC 461A003E */  c.le.s  $f0, $f26                  
/* 00730 809E1FE0 E6060204 */  swc1    $f6, 0x0204($s0)           ## 00000204
/* 00734 809E1FE4 4500002E */  bc1f    .L809E20A0                 
/* 00738 809E1FE8 00000000 */  nop
.L809E1FEC:
/* 0073C 809E1FEC 8E0801FC */  lw      $t0, 0x01FC($s0)           ## 000001FC
/* 00740 809E1FF0 8E0501F4 */  lw      $a1, 0x01F4($s0)           ## 000001F4
/* 00744 809E1FF4 25090001 */  addiu   $t1, $t0, 0x0001           ## $t1 = 00000001
/* 00748 809E1FF8 AE0901FC */  sw      $t1, 0x01FC($s0)           ## 000001FC
/* 0074C 809E1FFC 0C2787A0 */  jal     func_809E1E80              
/* 00750 809E2000 8E241E08 */  lw      $a0, 0x1E08($s1)           ## 00001E08
/* 00754 809E2004 8E0701FC */  lw      $a3, 0x01FC($s0)           ## 000001FC
/* 00758 809E2008 02402825 */  or      $a1, $s2, $zero            ## $a1 = FFFFFFEC
/* 0075C 809E200C 04E00003 */  bltz    $a3, .L809E201C            
/* 00760 809E2010 00E2082A */  slt     $at, $a3, $v0              
/* 00764 809E2014 54200004 */  bnel    $at, $zero, .L809E2028     
/* 00768 809E2018 8E241E08 */  lw      $a0, 0x1E08($s1)           ## 00001E08
.L809E201C:
/* 0076C 809E201C AE0001FC */  sw      $zero, 0x01FC($s0)         ## 000001FC
/* 00770 809E2020 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 00774 809E2024 8E241E08 */  lw      $a0, 0x1E08($s1)           ## 00001E08
.L809E2028:
/* 00778 809E2028 0C2787A4 */  jal     func_809E1E90              
/* 0077C 809E202C 8E0601F4 */  lw      $a2, 0x01F4($s0)           ## 000001F4
/* 00780 809E2030 C7B00064 */  lwc1    $f16, 0x0064($sp)          
/* 00784 809E2034 C6120024 */  lwc1    $f18, 0x0024($s0)          ## 00000024
/* 00788 809E2038 C7A4006C */  lwc1    $f4, 0x006C($sp)           
/* 0078C 809E203C C606002C */  lwc1    $f6, 0x002C($s0)           ## 0000002C
/* 00790 809E2040 46128501 */  sub.s   $f20, $f16, $f18           
/* 00794 809E2044 46062581 */  sub.s   $f22, $f4, $f6             
/* 00798 809E2048 4600A306 */  mov.s   $f12, $f20                 
/* 0079C 809E204C 0C03F494 */  jal     Math_atan2f              
/* 007A0 809E2050 4600B386 */  mov.s   $f14, $f22                 
/* 007A4 809E2054 46180202 */  mul.s   $f8, $f0, $f24             
/* 007A8 809E2058 00000000 */  nop
/* 007AC 809E205C 4614A102 */  mul.s   $f4, $f20, $f20            
/* 007B0 809E2060 00000000 */  nop
/* 007B4 809E2064 4616B182 */  mul.s   $f6, $f22, $f22            
/* 007B8 809E2068 4600428D */  trunc.w.s $f10, $f8                  
/* 007BC 809E206C 46062000 */  add.s   $f0, $f4, $f6              
/* 007C0 809E2070 440B5000 */  mfc1    $t3, $f10                  
/* 007C4 809E2074 46000004 */  sqrt.s  $f0, $f0                   
/* 007C8 809E2078 000B6400 */  sll     $t4, $t3, 16               
/* 007CC 809E207C 000C6C03 */  sra     $t5, $t4, 16               
/* 007D0 809E2080 448D8000 */  mtc1    $t5, $f16                  ## $f16 = 0.00
/* 007D4 809E2084 00000000 */  nop
/* 007D8 809E2088 468084A0 */  cvt.s.w $f18, $f16                 
/* 007DC 809E208C E6000208 */  swc1    $f0, 0x0208($s0)           ## 00000208
/* 007E0 809E2090 461A003E */  c.le.s  $f0, $f26                  
/* 007E4 809E2094 E6120204 */  swc1    $f18, 0x0204($s0)          ## 00000204
/* 007E8 809E2098 4501FFD4 */  bc1t    .L809E1FEC                 
/* 007EC 809E209C 00000000 */  nop
.L809E20A0:
/* 007F0 809E20A0 C6080204 */  lwc1    $f8, 0x0204($s0)           ## 00000204
/* 007F4 809E20A4 AFA00010 */  sw      $zero, 0x0010($sp)         
/* 007F8 809E20A8 260400B6 */  addiu   $a0, $s0, 0x00B6           ## $a0 = 000000B6
/* 007FC 809E20AC 4600428D */  trunc.w.s $f10, $f8                  
/* 00800 809E20B0 24060001 */  addiu   $a2, $zero, 0x0001         ## $a2 = 00000001
/* 00804 809E20B4 240709C4 */  addiu   $a3, $zero, 0x09C4         ## $a3 = 000009C4
/* 00808 809E20B8 44055000 */  mfc1    $a1, $f10                  
/* 0080C 809E20BC 00000000 */  nop
/* 00810 809E20C0 00052C00 */  sll     $a1, $a1, 16               
/* 00814 809E20C4 0C01E1A7 */  jal     Math_SmoothScaleMaxMinS
              
/* 00818 809E20C8 00052C03 */  sra     $a1, $a1, 16               
/* 0081C 809E20CC 860F00B6 */  lh      $t7, 0x00B6($s0)           ## 000000B6
/* 00820 809E20D0 C610020C */  lwc1    $f16, 0x020C($s0)          ## 0000020C
/* 00824 809E20D4 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00828 809E20D8 A60F0032 */  sh      $t7, 0x0032($s0)           ## 00000032
/* 0082C 809E20DC 0C00B638 */  jal     Actor_MoveForward
              
/* 00830 809E20E0 E6100068 */  swc1    $f16, 0x0068($s0)          ## 00000068
/* 00834 809E20E4 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 00838 809E20E8 24180004 */  addiu   $t8, $zero, 0x0004         ## $t8 = 00000004
/* 0083C 809E20EC AFB80014 */  sw      $t8, 0x0014($sp)           
/* 00840 809E20F0 44060000 */  mfc1    $a2, $f0                   
/* 00844 809E20F4 44070000 */  mfc1    $a3, $f0                   
/* 00848 809E20F8 8FA4007C */  lw      $a0, 0x007C($sp)           
/* 0084C 809E20FC 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 00850 809E2100 0C00B92D */  jal     func_8002E4B4              
/* 00854 809E2104 E7A00010 */  swc1    $f0, 0x0010($sp)           
/* 00858 809E2108 8FBF004C */  lw      $ra, 0x004C($sp)           
/* 0085C 809E210C D7B40020 */  ldc1    $f20, 0x0020($sp)          
/* 00860 809E2110 D7B60028 */  ldc1    $f22, 0x0028($sp)          
/* 00864 809E2114 D7B80030 */  ldc1    $f24, 0x0030($sp)          
/* 00868 809E2118 D7BA0038 */  ldc1    $f26, 0x0038($sp)          
/* 0086C 809E211C 8FB00040 */  lw      $s0, 0x0040($sp)           
/* 00870 809E2120 8FB10044 */  lw      $s1, 0x0044($sp)           
/* 00874 809E2124 8FB20048 */  lw      $s2, 0x0048($sp)           
/* 00878 809E2128 27BD0078 */  addiu   $sp, $sp, 0x0078           ## $sp = 00000000
/* 0087C 809E212C 03E00008 */  jr      $ra                        
/* 00880 809E2130 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000