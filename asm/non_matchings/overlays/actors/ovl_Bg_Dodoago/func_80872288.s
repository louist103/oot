glabel func_80872288
/* 00888 80872288 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 0088C 8087228C AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 00890 80872290 848E0164 */  lh      $t6, 0x0164($a0)           ## 00000164
/* 00894 80872294 3C030001 */  lui     $v1, 0x0001                ## $v1 = 00010000
/* 00898 80872298 240100FF */  addiu   $at, $zero, 0x00FF         ## $at = 000000FF
/* 0089C 8087229C 000E7840 */  sll     $t7, $t6,  1               
/* 008A0 808722A0 00AFC021 */  addu    $t8, $a1, $t7              
/* 008A4 808722A4 03031021 */  addu    $v0, $t8, $v1              
/* 008A8 808722A8 84591D30 */  lh      $t9, 0x1D30($v0)           ## 00001D30
/* 008AC 808722AC 27280005 */  addiu   $t0, $t9, 0x0005           ## $t0 = 00000005
/* 008B0 808722B0 A4481D30 */  sh      $t0, 0x1D30($v0)           ## 00001D30
/* 008B4 808722B4 84890164 */  lh      $t1, 0x0164($a0)           ## 00000164
/* 008B8 808722B8 00095040 */  sll     $t2, $t1,  1               
/* 008BC 808722BC 00AA5821 */  addu    $t3, $a1, $t2              
/* 008C0 808722C0 01636021 */  addu    $t4, $t3, $v1              
/* 008C4 808722C4 858D1D30 */  lh      $t5, 0x1D30($t4)           ## 00001D30
/* 008C8 808722C8 3C058087 */  lui     $a1, %hi(func_80871CF4)    ## $a1 = 80870000
/* 008CC 808722CC 55A10004 */  bnel    $t5, $at, .L808722E0       
/* 008D0 808722D0 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 008D4 808722D4 0C21C680 */  jal     BgDodoago_SetupAction              
/* 008D8 808722D8 24A51CF4 */  addiu   $a1, $a1, %lo(func_80871CF4) ## $a1 = 80871CF4
/* 008DC 808722DC 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L808722E0:
/* 008E0 808722E0 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 008E4 808722E4 03E00008 */  jr      $ra                        
/* 008E8 808722E8 00000000 */  nop
