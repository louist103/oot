glabel func_80999214
/* 00A94 80999214 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 00A98 80999218 3C01809A */  lui     $at, %hi(D_8099C73C)       ## $at = 809A0000
/* 00A9C 8099921C C424C73C */  lwc1    $f4, %lo(D_8099C73C)($at)  
/* 00AA0 80999220 AFB10020 */  sw      $s1, 0x0020($sp)           
/* 00AA4 80999224 AFB0001C */  sw      $s0, 0x001C($sp)           
/* 00AA8 80999228 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 00AAC 8099922C 00808825 */  or      $s1, $a0, $zero            ## $s1 = 00000000
/* 00AB0 80999230 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 00AB4 80999234 3C063E4C */  lui     $a2, 0x3E4C                ## $a2 = 3E4C0000
/* 00AB8 80999238 34C6CCCD */  ori     $a2, $a2, 0xCCCD           ## $a2 = 3E4CCCCD
/* 00ABC 8099923C 248401A8 */  addiu   $a0, $a0, 0x01A8           ## $a0 = 000001A8
/* 00AC0 80999240 3C05437F */  lui     $a1, 0x437F                ## $a1 = 437F0000
/* 00AC4 80999244 3C0740A0 */  lui     $a3, 0x40A0                ## $a3 = 40A00000
/* 00AC8 80999248 0C01E0C4 */  jal     Math_SmoothScaleMaxMinF
              
/* 00ACC 8099924C E7A40010 */  swc1    $f4, 0x0010($sp)           
/* 00AD0 80999250 962E0192 */  lhu     $t6, 0x0192($s1)           ## 00000192
/* 00AD4 80999254 240F0028 */  addiu   $t7, $zero, 0x0028         ## $t7 = 00000028
/* 00AD8 80999258 3C014220 */  lui     $at, 0x4220                ## $at = 42200000
/* 00ADC 8099925C 01EEC023 */  subu    $t8, $t7, $t6              
/* 00AE0 80999260 44983000 */  mtc1    $t8, $f6                   ## $f6 = 0.00
/* 00AE4 80999264 44815000 */  mtc1    $at, $f10                  ## $f10 = 40.00
/* 00AE8 80999268 44801000 */  mtc1    $zero, $f2                 ## $f2 = 0.00
/* 00AEC 8099926C 46803220 */  cvt.s.w $f8, $f6                   
/* 00AF0 80999270 3C01C37F */  lui     $at, 0xC37F                ## $at = C37F0000
/* 00AF4 80999274 44818000 */  mtc1    $at, $f16                  ## $f16 = -255.00
/* 00AF8 80999278 460A4003 */  div.s   $f0, $f8, $f10             
/* 00AFC 8099927C 4602003C */  c.lt.s  $f0, $f2                   
/* 00B00 80999280 00000000 */  nop
/* 00B04 80999284 45000002 */  bc1f    .L80999290                 
/* 00B08 80999288 00000000 */  nop
/* 00B0C 8099928C 46001006 */  mov.s   $f0, $f2                   
.L80999290:
/* 00B10 80999290 46008482 */  mul.s   $f18, $f16, $f0            
/* 00B14 80999294 00001825 */  or      $v1, $zero, $zero          ## $v1 = 00000000
/* 00B18 80999298 3C050001 */  lui     $a1, 0x0001                ## $a1 = 00010000
/* 00B1C 8099929C 4600910D */  trunc.w.s $f4, $f18                  
/* 00B20 809992A0 44042000 */  mfc1    $a0, $f4                   
/* 00B24 809992A4 00000000 */  nop
.L809992A8:
/* 00B28 809992A8 00034040 */  sll     $t0, $v1,  1               
/* 00B2C 809992AC 24630001 */  addiu   $v1, $v1, 0x0001           ## $v1 = 00000001
/* 00B30 809992B0 00031C00 */  sll     $v1, $v1, 16               
/* 00B34 809992B4 02084821 */  addu    $t1, $s0, $t0              
/* 00B38 809992B8 00031C03 */  sra     $v1, $v1, 16               
/* 00B3C 809992BC 01251021 */  addu    $v0, $t1, $a1              
/* 00B40 809992C0 28610003 */  slti    $at, $v1, 0x0003           
/* 00B44 809992C4 A4440AB6 */  sh      $a0, 0x0AB6($v0)           ## 00000AB6
/* 00B48 809992C8 A4440ABC */  sh      $a0, 0x0ABC($v0)           ## 00000ABC
/* 00B4C 809992CC 1420FFF6 */  bne     $at, $zero, .L809992A8     
/* 00B50 809992D0 A4440AB0 */  sh      $a0, 0x0AB0($v0)           ## 00000AB0
/* 00B54 809992D4 3C01C3FA */  lui     $at, 0xC3FA                ## $at = C3FA0000
/* 00B58 809992D8 44813000 */  mtc1    $at, $f6                   ## $f6 = -500.00
/* 00B5C 809992DC 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00B60 809992E0 00300821 */  addu    $at, $at, $s0              
/* 00B64 809992E4 46003202 */  mul.s   $f8, $f6, $f0              
/* 00B68 809992E8 4602003E */  c.le.s  $f0, $f2                   
/* 00B6C 809992EC 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 00B70 809992F0 3C05809A */  lui     $a1, %hi(func_80999348)    ## $a1 = 809A0000
/* 00B74 809992F4 4600428D */  trunc.w.s $f10, $f8                  
/* 00B78 809992F8 440B5000 */  mfc1    $t3, $f10                  
/* 00B7C 809992FC 00000000 */  nop
/* 00B80 80999300 A42B0AC2 */  sh      $t3, 0x0AC2($at)           ## 00010AC2
/* 00B84 80999304 962C0192 */  lhu     $t4, 0x0192($s1)           ## 00000192
/* 00B88 80999308 258D0001 */  addiu   $t5, $t4, 0x0001           ## $t5 = 00000001
/* 00B8C 8099930C 45000003 */  bc1f    .L8099931C                 
/* 00B90 80999310 A62D0192 */  sh      $t5, 0x0192($s1)           ## 00000192
/* 00B94 80999314 0C2661E0 */  jal     func_80998780              
/* 00B98 80999318 24A59348 */  addiu   $a1, $a1, %lo(func_80999348) ## $a1 = 80999348
.L8099931C:
/* 00B9C 8099931C 862F00B6 */  lh      $t7, 0x00B6($s1)           ## 000000B6
/* 00BA0 80999320 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 00BA4 80999324 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 00BA8 80999328 25EE0320 */  addiu   $t6, $t7, 0x0320           ## $t6 = 00000320
/* 00BAC 8099932C 0C266465 */  jal     func_80999194              
/* 00BB0 80999330 A62E00B6 */  sh      $t6, 0x00B6($s1)           ## 000000B6
/* 00BB4 80999334 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 00BB8 80999338 8FB0001C */  lw      $s0, 0x001C($sp)           
/* 00BBC 8099933C 8FB10020 */  lw      $s1, 0x0020($sp)           
/* 00BC0 80999340 03E00008 */  jr      $ra                        
/* 00BC4 80999344 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000