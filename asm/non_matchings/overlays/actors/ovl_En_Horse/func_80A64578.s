glabel func_80A64578
/* 09288 80A64578 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 0928C 80A6457C AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 09290 80A64580 AFA5002C */  sw      $a1, 0x002C($sp)           
/* 09294 80A64584 8C8F0078 */  lw      $t7, 0x0078($a0)           ## 00000078
/* 09298 80A64588 8CA21C44 */  lw      $v0, 0x1C44($a1)           ## 00001C44
/* 0929C 80A6458C 00803825 */  or      $a3, $a0, $zero            ## $a3 = 00000000
/* 092A0 80A64590 15E00006 */  bne     $t7, $zero, .L80A645AC     
/* 092A4 80A64594 AFAF001C */  sw      $t7, 0x001C($sp)           
/* 092A8 80A64598 8C590440 */  lw      $t9, 0x0440($v0)           ## 00000440
/* 092AC 80A6459C 50990004 */  beql    $a0, $t9, .L80A645B0       
/* 092B0 80A645A0 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 092B4 80A645A4 10000031 */  beq     $zero, $zero, .L80A6466C   
/* 092B8 80A645A8 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
.L80A645AC:
/* 092BC 80A645AC 8FA4002C */  lw      $a0, 0x002C($sp)           
.L80A645B0:
/* 092C0 80A645B0 90E6007D */  lbu     $a2, 0x007D($a3)           ## 0000007D
/* 092C4 80A645B4 AFA70028 */  sw      $a3, 0x0028($sp)           
/* 092C8 80A645B8 8FA5001C */  lw      $a1, 0x001C($sp)           
/* 092CC 80A645BC 0C010839 */  jal     func_800420E4              
/* 092D0 80A645C0 248407C0 */  addiu   $a0, $a0, 0x07C0           ## $a0 = 000007C0
/* 092D4 80A645C4 8FA70028 */  lw      $a3, 0x0028($sp)           
/* 092D8 80A645C8 00025280 */  sll     $t2, $v0, 10               
/* 092DC 80A645CC 3C014448 */  lui     $at, 0x4448                ## $at = 44480000
/* 092E0 80A645D0 84E30032 */  lh      $v1, 0x0032($a3)           ## 00000032
/* 092E4 80A645D4 44811000 */  mtc1    $at, $f2                   ## $f2 = 800.00
/* 092E8 80A645D8 3C01C448 */  lui     $at, 0xC448                ## $at = C4480000
/* 092EC 80A645DC 01432023 */  subu    $a0, $t2, $v1              
/* 092F0 80A645E0 00042400 */  sll     $a0, $a0, 16               
/* 092F4 80A645E4 00042403 */  sra     $a0, $a0, 16               
/* 092F8 80A645E8 44842000 */  mtc1    $a0, $f4                   ## $f4 = 0.00
/* 092FC 80A645EC 00000000 */  nop
/* 09300 80A645F0 46802020 */  cvt.s.w $f0, $f4                   
/* 09304 80A645F4 4600103C */  c.lt.s  $f2, $f0                   
/* 09308 80A645F8 00000000 */  nop
/* 0930C 80A645FC 4502000A */  bc1fl   .L80A64628                 
/* 09310 80A64600 44819000 */  mtc1    $at, $f18                  ## $f18 = -800.00
/* 09314 80A64604 44833000 */  mtc1    $v1, $f6                   ## $f6 = 0.00
/* 09318 80A64608 00000000 */  nop
/* 0931C 80A6460C 46803220 */  cvt.s.w $f8, $f6                   
/* 09320 80A64610 46024280 */  add.s   $f10, $f8, $f2             
/* 09324 80A64614 4600540D */  trunc.w.s $f16, $f10                 
/* 09328 80A64618 440C8000 */  mfc1    $t4, $f16                  
/* 0932C 80A6461C 10000010 */  beq     $zero, $zero, .L80A64660   
/* 09330 80A64620 A4EC0032 */  sh      $t4, 0x0032($a3)           ## 00000032
/* 09334 80A64624 44819000 */  mtc1    $at, $f18                  ## $f18 = -800.00
.L80A64628:
/* 09338 80A64628 00647821 */  addu    $t7, $v1, $a0              
/* 0933C 80A6462C 4612003C */  c.lt.s  $f0, $f18                  
/* 09340 80A64630 00000000 */  nop
/* 09344 80A64634 4502000A */  bc1fl   .L80A64660                 
/* 09348 80A64638 A4EF0032 */  sh      $t7, 0x0032($a3)           ## 00000032
/* 0934C 80A6463C 44832000 */  mtc1    $v1, $f4                   ## $f4 = 0.00
/* 09350 80A64640 00000000 */  nop
/* 09354 80A64644 468021A0 */  cvt.s.w $f6, $f4                   
/* 09358 80A64648 46023201 */  sub.s   $f8, $f6, $f2              
/* 0935C 80A6464C 4600428D */  trunc.w.s $f10, $f8                  
/* 09360 80A64650 440E5000 */  mfc1    $t6, $f10                  
/* 09364 80A64654 10000002 */  beq     $zero, $zero, .L80A64660   
/* 09368 80A64658 A4EE0032 */  sh      $t6, 0x0032($a3)           ## 00000032
/* 0936C 80A6465C A4EF0032 */  sh      $t7, 0x0032($a3)           ## 00000032
.L80A64660:
/* 09370 80A64660 84F80032 */  lh      $t8, 0x0032($a3)           ## 00000032
/* 09374 80A64664 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
/* 09378 80A64668 A4F800B6 */  sh      $t8, 0x00B6($a3)           ## 000000B6
.L80A6466C:
/* 0937C 80A6466C 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 09380 80A64670 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 09384 80A64674 03E00008 */  jr      $ra                        
/* 09388 80A64678 00000000 */  nop