glabel func_80AE942C
/* 00FEC 80AE942C 27BDFFD0 */  addiu   $sp, $sp, 0xFFD0           ## $sp = FFFFFFD0
/* 00FF0 80AE9430 AFBF002C */  sw      $ra, 0x002C($sp)           
/* 00FF4 80AE9434 AFB40028 */  sw      $s4, 0x0028($sp)           
/* 00FF8 80AE9438 AFB30024 */  sw      $s3, 0x0024($sp)           
/* 00FFC 80AE943C AFB20020 */  sw      $s2, 0x0020($sp)           
/* 01000 80AE9440 AFB1001C */  sw      $s1, 0x001C($sp)           
/* 01004 80AE9444 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 01008 80AE9448 AFA50034 */  sw      $a1, 0x0034($sp)           
/* 0100C 80AE944C 908E0371 */  lbu     $t6, 0x0371($a0)           ## 00000371
/* 01010 80AE9450 00809025 */  or      $s2, $a0, $zero            ## $s2 = 00000000
/* 01014 80AE9454 849301F6 */  lh      $s3, 0x01F6($a0)           ## 000001F6
/* 01018 80AE9458 15C00043 */  bne     $t6, $zero, .L80AE9568     
/* 0101C 80AE945C 00008025 */  or      $s0, $zero, $zero          ## $s0 = 00000000
/* 01020 80AE9460 00808825 */  or      $s1, $a0, $zero            ## $s1 = 00000000
.L80AE9464:
/* 01024 80AE9464 C6440200 */  lwc1    $f4, 0x0200($s2)           ## 00000200
/* 01028 80AE9468 4600218D */  trunc.w.s $f6, $f4                   
/* 0102C 80AE946C 44183000 */  mfc1    $t8, $f6                   
/* 01030 80AE9470 00000000 */  nop
/* 01034 80AE9474 0018CC00 */  sll     $t9, $t8, 16               
/* 01038 80AE9478 00194403 */  sra     $t0, $t9, 16               
/* 0103C 80AE947C 02080019 */  multu   $s0, $t0                   
/* 01040 80AE9480 00004812 */  mflo    $t1                        
/* 01044 80AE9484 00095300 */  sll     $t2, $t1, 12               
/* 01048 80AE9488 026A2021 */  addu    $a0, $s3, $t2              
/* 0104C 80AE948C 00042400 */  sll     $a0, $a0, 16               
/* 01050 80AE9490 0C01DE0D */  jal     Math_Coss
              ## coss?
/* 01054 80AE9494 00042403 */  sra     $a0, $a0, 16               
/* 01058 80AE9498 C6480214 */  lwc1    $f8, 0x0214($s2)           ## 00000214
/* 0105C 80AE949C 26100001 */  addiu   $s0, $s0, 0x0001           ## $s0 = 00000001
/* 01060 80AE94A0 2A010005 */  slti    $at, $s0, 0x0005           
/* 01064 80AE94A4 46080082 */  mul.s   $f2, $f0, $f8              
/* 01068 80AE94A8 26310040 */  addiu   $s1, $s1, 0x0040           ## $s1 = 00000040
/* 0106C 80AE94AC E622020C */  swc1    $f2, 0x020C($s1)           ## 0000024C
/* 01070 80AE94B0 1420FFEC */  bne     $at, $zero, .L80AE9464     
/* 01074 80AE94B4 E6220204 */  swc1    $f2, 0x0204($s1)           ## 00000244
/* 01078 80AE94B8 924B0371 */  lbu     $t3, 0x0371($s2)           ## 00000371
/* 0107C 80AE94BC 865301F6 */  lh      $s3, 0x01F6($s2)           ## 000001F6
/* 01080 80AE94C0 5560002A */  bnel    $t3, $zero, .L80AE956C     
/* 01084 80AE94C4 924F0376 */  lbu     $t7, 0x0376($s2)           ## 00000376
/* 01088 80AE94C8 924C0370 */  lbu     $t4, 0x0370($s2)           ## 00000370
/* 0108C 80AE94CC 24100001 */  addiu   $s0, $zero, 0x0001         ## $s0 = 00000001
/* 01090 80AE94D0 26510040 */  addiu   $s1, $s2, 0x0040           ## $s1 = 00000040
/* 01094 80AE94D4 15800024 */  bne     $t4, $zero, .L80AE9568     
/* 01098 80AE94D8 24140005 */  addiu   $s4, $zero, 0x0005         ## $s4 = 00000005
.L80AE94DC:
/* 0109C 80AE94DC C64A0204 */  lwc1    $f10, 0x0204($s2)          ## 00000204
/* 010A0 80AE94E0 4600540D */  trunc.w.s $f16, $f10                 
/* 010A4 80AE94E4 440E8000 */  mfc1    $t6, $f16                  
/* 010A8 80AE94E8 00000000 */  nop
/* 010AC 80AE94EC 000E7C00 */  sll     $t7, $t6, 16               
/* 010B0 80AE94F0 000FC403 */  sra     $t8, $t7, 16               
/* 010B4 80AE94F4 02180019 */  multu   $s0, $t8                   
/* 010B8 80AE94F8 0000C812 */  mflo    $t9                        
/* 010BC 80AE94FC 00194300 */  sll     $t0, $t9, 12               
/* 010C0 80AE9500 02682021 */  addu    $a0, $s3, $t0              
/* 010C4 80AE9504 00042400 */  sll     $a0, $a0, 16               
/* 010C8 80AE9508 0C01DE0D */  jal     Math_Coss
              ## coss?
/* 010CC 80AE950C 00042403 */  sra     $a0, $a0, 16               
/* 010D0 80AE9510 C652021C */  lwc1    $f18, 0x021C($s2)          ## 0000021C
/* 010D4 80AE9514 46120102 */  mul.s   $f4, $f0, $f18             
/* 010D8 80AE9518 E6240250 */  swc1    $f4, 0x0250($s1)           ## 00000290
/* 010DC 80AE951C C646020C */  lwc1    $f6, 0x020C($s2)           ## 0000020C
/* 010E0 80AE9520 4600320D */  trunc.w.s $f8, $f6                   
/* 010E4 80AE9524 440A4000 */  mfc1    $t2, $f8                   
/* 010E8 80AE9528 00000000 */  nop
/* 010EC 80AE952C 000A5C00 */  sll     $t3, $t2, 16               
/* 010F0 80AE9530 000B6403 */  sra     $t4, $t3, 16               
/* 010F4 80AE9534 020C0019 */  multu   $s0, $t4                   
/* 010F8 80AE9538 00006812 */  mflo    $t5                        
/* 010FC 80AE953C 000D7300 */  sll     $t6, $t5, 12               
/* 01100 80AE9540 026E2021 */  addu    $a0, $s3, $t6              
/* 01104 80AE9544 00042400 */  sll     $a0, $a0, 16               
/* 01108 80AE9548 0C01DE1C */  jal     Math_Sins
              ## sins?
/* 0110C 80AE954C 00042403 */  sra     $a0, $a0, 16               
/* 01110 80AE9550 C64A021C */  lwc1    $f10, 0x021C($s2)          ## 0000021C
/* 01114 80AE9554 26100001 */  addiu   $s0, $s0, 0x0001           ## $s0 = 00000002
/* 01118 80AE9558 26310040 */  addiu   $s1, $s1, 0x0040           ## $s1 = 00000080
/* 0111C 80AE955C 460A0402 */  mul.s   $f16, $f0, $f10            
/* 01120 80AE9560 1614FFDE */  bne     $s0, $s4, .L80AE94DC       
/* 01124 80AE9564 E6300218 */  swc1    $f16, 0x0218($s1)          ## 00000298
.L80AE9568:
/* 01128 80AE9568 924F0376 */  lbu     $t7, 0x0376($s2)           ## 00000376
.L80AE956C:
/* 0112C 80AE956C 55E00009 */  bnel    $t7, $zero, .L80AE9594     
/* 01130 80AE9570 8FBF002C */  lw      $ra, 0x002C($sp)           
/* 01134 80AE9574 C65201F8 */  lwc1    $f18, 0x01F8($s2)          ## 000001F8
/* 01138 80AE9578 865801F6 */  lh      $t8, 0x01F6($s2)           ## 000001F6
/* 0113C 80AE957C 4600910D */  trunc.w.s $f4, $f18                  
/* 01140 80AE9580 440A2000 */  mfc1    $t2, $f4                   
/* 01144 80AE9584 00000000 */  nop
/* 01148 80AE9588 030A5821 */  addu    $t3, $t8, $t2              
/* 0114C 80AE958C A64B01F6 */  sh      $t3, 0x01F6($s2)           ## 000001F6
/* 01150 80AE9590 8FBF002C */  lw      $ra, 0x002C($sp)           
.L80AE9594:
/* 01154 80AE9594 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 01158 80AE9598 8FB1001C */  lw      $s1, 0x001C($sp)           
/* 0115C 80AE959C 8FB20020 */  lw      $s2, 0x0020($sp)           
/* 01160 80AE95A0 8FB30024 */  lw      $s3, 0x0024($sp)           
/* 01164 80AE95A4 8FB40028 */  lw      $s4, 0x0028($sp)           
/* 01168 80AE95A8 03E00008 */  jr      $ra                        
/* 0116C 80AE95AC 27BD0030 */  addiu   $sp, $sp, 0x0030           ## $sp = 00000000