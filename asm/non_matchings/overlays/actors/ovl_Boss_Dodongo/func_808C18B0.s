glabel func_808C18B0
/* 00720 808C18B0 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 00724 808C18B4 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 00728 808C18B8 8CA61C4C */  lw      $a2, 0x1C4C($a1)           ## 00001C4C
/* 0072C 808C18BC 3C014220 */  lui     $at, 0x4220                ## $at = 42200000
/* 00730 808C18C0 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 00734 808C18C4 10C00026 */  beq     $a2, $zero, .L808C1960     
/* 00738 808C18C8 00000000 */  nop
/* 0073C 808C18CC 44818000 */  mtc1    $at, $f16                  ## $f16 = 40.00
/* 00740 808C18D0 00000000 */  nop
.L808C18D4:
/* 00744 808C18D4 54C40004 */  bnel    $a2, $a0, .L808C18E8       
/* 00748 808C18D8 C4C40024 */  lwc1    $f4, 0x0024($a2)           ## 00000024
/* 0074C 808C18DC 1000001E */  beq     $zero, $zero, .L808C1958   
/* 00750 808C18E0 8CC60124 */  lw      $a2, 0x0124($a2)           ## 00000124
/* 00754 808C18E4 C4C40024 */  lwc1    $f4, 0x0024($a2)           ## 00000024
.L808C18E8:
/* 00758 808C18E8 C486041C */  lwc1    $f6, 0x041C($a0)           ## 0000041C
/* 0075C 808C18EC C4C80028 */  lwc1    $f8, 0x0028($a2)           ## 00000028
/* 00760 808C18F0 C48A0420 */  lwc1    $f10, 0x0420($a0)          ## 00000420
/* 00764 808C18F4 46062081 */  sub.s   $f2, $f4, $f6              
/* 00768 808C18F8 C4840424 */  lwc1    $f4, 0x0424($a0)           ## 00000424
/* 0076C 808C18FC C4D2002C */  lwc1    $f18, 0x002C($a2)          ## 0000002C
/* 00770 808C1900 460A4301 */  sub.s   $f12, $f8, $f10            
/* 00774 808C1904 46001005 */  abs.s   $f0, $f2                   
/* 00778 808C1908 46049381 */  sub.s   $f14, $f18, $f4            
/* 0077C 808C190C 4610003C */  c.lt.s  $f0, $f16                  
/* 00780 808C1910 00000000 */  nop
/* 00784 808C1914 45020010 */  bc1fl   .L808C1958                 
/* 00788 808C1918 8CC60124 */  lw      $a2, 0x0124($a2)           ## 00000124
/* 0078C 808C191C 46006005 */  abs.s   $f0, $f12                  
/* 00790 808C1920 4610003C */  c.lt.s  $f0, $f16                  
/* 00794 808C1924 00000000 */  nop
/* 00798 808C1928 4502000B */  bc1fl   .L808C1958                 
/* 0079C 808C192C 8CC60124 */  lw      $a2, 0x0124($a2)           ## 00000124
/* 007A0 808C1930 46007005 */  abs.s   $f0, $f14                  
/* 007A4 808C1934 4610003C */  c.lt.s  $f0, $f16                  
/* 007A8 808C1938 00000000 */  nop
/* 007AC 808C193C 45020006 */  bc1fl   .L808C1958                 
/* 007B0 808C1940 8CC60124 */  lw      $a2, 0x0124($a2)           ## 00000124
/* 007B4 808C1944 0C00B55C */  jal     Actor_Kill
              
/* 007B8 808C1948 00C02025 */  or      $a0, $a2, $zero            ## $a0 = 00000000
/* 007BC 808C194C 10000004 */  beq     $zero, $zero, .L808C1960   
/* 007C0 808C1950 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
/* 007C4 808C1954 8CC60124 */  lw      $a2, 0x0124($a2)           ## 00000124
.L808C1958:
/* 007C8 808C1958 14C0FFDE */  bne     $a2, $zero, .L808C18D4     
/* 007CC 808C195C 00000000 */  nop
.L808C1960:
/* 007D0 808C1960 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 007D4 808C1964 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 007D8 808C1968 03E00008 */  jr      $ra                        
/* 007DC 808C196C 00000000 */  nop