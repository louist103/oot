.late_rodata
glabel D_809C174C
 .word 0x44E38000

.text
glabel func_809C0174
/* 008D4 809C0174 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 008D8 809C0178 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 008DC 809C017C 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 008E0 809C0180 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 008E4 809C0184 AFA50024 */  sw      $a1, 0x0024($sp)           
/* 008E8 809C0188 0C02927F */  jal     SkelAnime_Update
              
/* 008EC 809C018C 2484014C */  addiu   $a0, $a0, 0x014C           ## $a0 = 0000014C
/* 008F0 809C0190 86020196 */  lh      $v0, 0x0196($s0)           ## 00000196
/* 008F4 809C0194 3C01809C */  lui     $at, %hi(D_809C174C)       ## $at = 809C0000
/* 008F8 809C0198 10400003 */  beq     $v0, $zero, .L809C01A8     
/* 008FC 809C019C 244EFFFF */  addiu   $t6, $v0, 0xFFFF           ## $t6 = FFFFFFFF
/* 00900 809C01A0 A60E0196 */  sh      $t6, 0x0196($s0)           ## 00000196
/* 00904 809C01A4 86020196 */  lh      $v0, 0x0196($s0)           ## 00000196
.L809C01A8:
/* 00908 809C01A8 04410004 */  bgez    $v0, .L809C01BC            
/* 0090C 809C01AC 304F0003 */  andi    $t7, $v0, 0x0003           ## $t7 = 00000000
/* 00910 809C01B0 11E00002 */  beq     $t7, $zero, .L809C01BC     
/* 00914 809C01B4 00000000 */  nop
/* 00918 809C01B8 25EFFFFC */  addiu   $t7, $t7, 0xFFFC           ## $t7 = FFFFFFFC
.L809C01BC:
/* 0091C 809C01BC 15E0000C */  bne     $t7, $zero, .L809C01F0     
/* 00920 809C01C0 00000000 */  nop
/* 00924 809C01C4 0C00CFC8 */  jal     Rand_CenteredFloat
              
/* 00928 809C01C8 C42C174C */  lwc1    $f12, %lo(D_809C174C)($at) 
/* 0092C 809C01CC 86180032 */  lh      $t8, 0x0032($s0)           ## 00000032
/* 00930 809C01D0 44982000 */  mtc1    $t8, $f4                   ## $f4 = 0.00
/* 00934 809C01D4 00000000 */  nop
/* 00938 809C01D8 468021A0 */  cvt.s.w $f6, $f4                   
/* 0093C 809C01DC 46003200 */  add.s   $f8, $f6, $f0              
/* 00940 809C01E0 4600428D */  trunc.w.s $f10, $f8                  
/* 00944 809C01E4 44085000 */  mfc1    $t0, $f10                  
/* 00948 809C01E8 00000000 */  nop
/* 0094C 809C01EC A6080032 */  sh      $t0, 0x0032($s0)           ## 00000032
.L809C01F0:
/* 00950 809C01F0 0C270023 */  jal     func_809C008C              
/* 00954 809C01F4 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00958 809C01F8 86090196 */  lh      $t1, 0x0196($s0)           ## 00000196
/* 0095C 809C01FC 3C014320 */  lui     $at, 0x4320                ## $at = 43200000
/* 00960 809C0200 44819000 */  mtc1    $at, $f18                  ## $f18 = 160.00
/* 00964 809C0204 15200002 */  bne     $t1, $zero, .L809C0210     
/* 00968 809C0208 240A0020 */  addiu   $t2, $zero, 0x0020         ## $t2 = 00000020
/* 0096C 809C020C A60A0196 */  sh      $t2, 0x0196($s0)           ## 00000196
.L809C0210:
/* 00970 809C0210 C6100090 */  lwc1    $f16, 0x0090($s0)          ## 00000090
/* 00974 809C0214 4612803C */  c.lt.s  $f16, $f18                 
/* 00978 809C0218 00000000 */  nop
/* 0097C 809C021C 4502000C */  bc1fl   .L809C0250                 
/* 00980 809C0220 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00984 809C0224 C6000094 */  lwc1    $f0, 0x0094($s0)           ## 00000094
/* 00988 809C0228 3C014234 */  lui     $at, 0x4234                ## $at = 42340000
/* 0098C 809C022C 44812000 */  mtc1    $at, $f4                   ## $f4 = 45.00
/* 00990 809C0230 46000005 */  abs.s   $f0, $f0                   
/* 00994 809C0234 4604003C */  c.lt.s  $f0, $f4                   
/* 00998 809C0238 00000000 */  nop
/* 0099C 809C023C 45020004 */  bc1fl   .L809C0250                 
/* 009A0 809C0240 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 009A4 809C0244 0C26FED0 */  jal     func_809BFB40              
/* 009A8 809C0248 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 009AC 809C024C 8FBF001C */  lw      $ra, 0x001C($sp)           
.L809C0250:
/* 009B0 809C0250 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 009B4 809C0254 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 009B8 809C0258 03E00008 */  jr      $ra                        
/* 009BC 809C025C 00000000 */  nop
