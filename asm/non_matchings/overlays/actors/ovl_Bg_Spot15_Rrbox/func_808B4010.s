glabel func_808B4010
/* 006B0 808B4010 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 006B4 808B4014 AFA50024 */  sw      $a1, 0x0024($sp)           
/* 006B8 808B4018 00802825 */  or      $a1, $a0, $zero            ## $a1 = 00000000
/* 006BC 808B401C AFBF001C */  sw      $ra, 0x001C($sp)           
/* 006C0 808B4020 AFA40020 */  sw      $a0, 0x0020($sp)           
/* 006C4 808B4024 3C014391 */  lui     $at, 0x4391                ## $at = 43910000
/* 006C8 808B4028 44813000 */  mtc1    $at, $f6                   ## $f6 = 290.00
/* 006CC 808B402C C4A40050 */  lwc1    $f4, 0x0050($a1)           ## 00000050
/* 006D0 808B4030 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 006D4 808B4034 44815000 */  mtc1    $at, $f10                  ## $f10 = 20.00
/* 006D8 808B4038 46062002 */  mul.s   $f0, $f4, $f6              
/* 006DC 808B403C 24180001 */  addiu   $t8, $zero, 0x0001         ## $t8 = 00000001
/* 006E0 808B4040 AFB80010 */  sw      $t8, 0x0010($sp)           
/* 006E4 808B4044 8FA40024 */  lw      $a0, 0x0024($sp)           
/* 006E8 808B4048 460A0400 */  add.s   $f16, $f0, $f10            
/* 006EC 808B404C 4600020D */  trunc.w.s $f8, $f0                   
/* 006F0 808B4050 4600848D */  trunc.w.s $f18, $f16                 
/* 006F4 808B4054 44064000 */  mfc1    $a2, $f8                   
/* 006F8 808B4058 44079000 */  mfc1    $a3, $f18                  
/* 006FC 808B405C 00063400 */  sll     $a2, $a2, 16               
/* 00700 808B4060 00063403 */  sra     $a2, $a2, 16               
/* 00704 808B4064 00073C00 */  sll     $a3, $a3, 16               
/* 00708 808B4068 0C010D76 */  jal     func_800435D8              
/* 0070C 808B406C 00073C03 */  sra     $a3, $a3, 16               
/* 00710 808B4070 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00714 808B4074 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 00718 808B4078 2C420001 */  sltiu   $v0, $v0, 0x0001           
/* 0071C 808B407C 03E00008 */  jr      $ra                        
/* 00720 808B4080 00000000 */  nop