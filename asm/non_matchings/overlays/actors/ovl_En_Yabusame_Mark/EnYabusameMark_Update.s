glabel EnYabusameMark_Update
/* 00584 80B43344 27BDFF90 */  addiu   $sp, $sp, 0xFF90           ## $sp = FFFFFF90
/* 00588 80B43348 AFB00048 */  sw      $s0, 0x0048($sp)           
/* 0058C 80B4334C 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 00590 80B43350 AFBF004C */  sw      $ra, 0x004C($sp)           
/* 00594 80B43354 AFA50074 */  sw      $a1, 0x0074($sp)           
/* 00598 80B43358 8E19014C */  lw      $t9, 0x014C($s0)           ## 0000014C
/* 0059C 80B4335C 0320F809 */  jalr    $ra, $t9                   
/* 005A0 80B43360 00000000 */  nop
/* 005A4 80B43364 86030150 */  lh      $v1, 0x0150($s0)           ## 00000150
/* 005A8 80B43368 3C0F80B4 */  lui     $t7, %hi(D_80B43560)       ## $t7 = 80B40000
/* 005AC 80B4336C 25EF3560 */  addiu   $t7, $t7, %lo(D_80B43560)  ## $t7 = 80B43560
/* 005B0 80B43370 00031880 */  sll     $v1, $v1,  2               
/* 005B4 80B43374 00037080 */  sll     $t6, $v1,  2               
/* 005B8 80B43378 01C37023 */  subu    $t6, $t6, $v1              
/* 005BC 80B4337C 000E7080 */  sll     $t6, $t6,  2               
/* 005C0 80B43380 01CF1021 */  addu    $v0, $t6, $t7              
/* 005C4 80B43384 C4440000 */  lwc1    $f4, 0x0000($v0)           ## 00000000
/* 005C8 80B43388 C6000024 */  lwc1    $f0, 0x0024($s0)           ## 00000024
/* 005CC 80B4338C C6020028 */  lwc1    $f2, 0x0028($s0)           ## 00000028
/* 005D0 80B43390 C60C002C */  lwc1    $f12, 0x002C($s0)          ## 0000002C
/* 005D4 80B43394 46002180 */  add.s   $f6, $f4, $f0              
/* 005D8 80B43398 26040190 */  addiu   $a0, $s0, 0x0190           ## $a0 = 00000190
/* 005DC 80B4339C 26180184 */  addiu   $t8, $s0, 0x0184           ## $t8 = 00000184
/* 005E0 80B433A0 26050160 */  addiu   $a1, $s0, 0x0160           ## $a1 = 00000160
/* 005E4 80B433A4 E6060160 */  swc1    $f6, 0x0160($s0)           ## 00000160
/* 005E8 80B433A8 C4480004 */  lwc1    $f8, 0x0004($v0)           ## 00000004
/* 005EC 80B433AC 2606016C */  addiu   $a2, $s0, 0x016C           ## $a2 = 0000016C
/* 005F0 80B433B0 26070178 */  addiu   $a3, $s0, 0x0178           ## $a3 = 00000178
/* 005F4 80B433B4 46024280 */  add.s   $f10, $f8, $f2             
/* 005F8 80B433B8 E60A0164 */  swc1    $f10, 0x0164($s0)          ## 00000164
/* 005FC 80B433BC C4500008 */  lwc1    $f16, 0x0008($v0)          ## 00000008
/* 00600 80B433C0 460C8480 */  add.s   $f18, $f16, $f12           
/* 00604 80B433C4 E6120168 */  swc1    $f18, 0x0168($s0)          ## 00000168
/* 00608 80B433C8 C444000C */  lwc1    $f4, 0x000C($v0)           ## 0000000C
/* 0060C 80B433CC 46002180 */  add.s   $f6, $f4, $f0              
/* 00610 80B433D0 E606016C */  swc1    $f6, 0x016C($s0)           ## 0000016C
/* 00614 80B433D4 C4480010 */  lwc1    $f8, 0x0010($v0)           ## 00000010
/* 00618 80B433D8 46024280 */  add.s   $f10, $f8, $f2             
/* 0061C 80B433DC E60A0170 */  swc1    $f10, 0x0170($s0)          ## 00000170
/* 00620 80B433E0 C4500014 */  lwc1    $f16, 0x0014($v0)          ## 00000014
/* 00624 80B433E4 460C8480 */  add.s   $f18, $f16, $f12           
/* 00628 80B433E8 E6120174 */  swc1    $f18, 0x0174($s0)          ## 00000174
/* 0062C 80B433EC C4440018 */  lwc1    $f4, 0x0018($v0)           ## 00000018
/* 00630 80B433F0 46002180 */  add.s   $f6, $f4, $f0              
/* 00634 80B433F4 E6060178 */  swc1    $f6, 0x0178($s0)           ## 00000178
/* 00638 80B433F8 C448001C */  lwc1    $f8, 0x001C($v0)           ## 0000001C
/* 0063C 80B433FC 46024280 */  add.s   $f10, $f8, $f2             
/* 00640 80B43400 E60A017C */  swc1    $f10, 0x017C($s0)          ## 0000017C
/* 00644 80B43404 C4500020 */  lwc1    $f16, 0x0020($v0)          ## 00000020
/* 00648 80B43408 460C8480 */  add.s   $f18, $f16, $f12           
/* 0064C 80B4340C E6120180 */  swc1    $f18, 0x0180($s0)          ## 00000180
/* 00650 80B43410 C4440024 */  lwc1    $f4, 0x0024($v0)           ## 00000024
/* 00654 80B43414 46002180 */  add.s   $f6, $f4, $f0              
/* 00658 80B43418 E6060184 */  swc1    $f6, 0x0184($s0)           ## 00000184
/* 0065C 80B4341C C4480028 */  lwc1    $f8, 0x0028($v0)           ## 00000028
/* 00660 80B43420 46024280 */  add.s   $f10, $f8, $f2             
/* 00664 80B43424 E60A0188 */  swc1    $f10, 0x0188($s0)          ## 00000188
/* 00668 80B43428 C450002C */  lwc1    $f16, 0x002C($v0)          ## 0000002C
/* 0066C 80B4342C 460C8480 */  add.s   $f18, $f16, $f12           
/* 00670 80B43430 E612018C */  swc1    $f18, 0x018C($s0)          ## 0000018C
/* 00674 80B43434 AFA40050 */  sw      $a0, 0x0050($sp)           
/* 00678 80B43438 0C0189CD */  jal     func_80062734              
/* 0067C 80B4343C AFB80010 */  sw      $t8, 0x0010($sp)           
/* 00680 80B43440 8FA40074 */  lw      $a0, 0x0074($sp)           
/* 00684 80B43444 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00688 80B43448 34211E60 */  ori     $at, $at, 0x1E60           ## $at = 00011E60
/* 0068C 80B4344C 8FA60050 */  lw      $a2, 0x0050($sp)           
/* 00690 80B43450 0C01767D */  jal     CollisionCheck_SetAC
              ## CollisionCheck_setAC
/* 00694 80B43454 00812821 */  addu    $a1, $a0, $at              
/* 00698 80B43458 3C088016 */  lui     $t0, %hi(gGameInfo)
/* 0069C 80B4345C 8D08FA90 */  lw      $t0, %lo(gGameInfo)($t0)
/* 006A0 80B43460 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 006A4 80B43464 240C00FF */  addiu   $t4, $zero, 0x00FF         ## $t4 = 000000FF
/* 006A8 80B43468 850912D4 */  lh      $t1, 0x12D4($t0)           ## 801612D4
/* 006AC 80B4346C 240D00FF */  addiu   $t5, $zero, 0x00FF         ## $t5 = 000000FF
/* 006B0 80B43470 24190004 */  addiu   $t9, $zero, 0x0004         ## $t9 = 00000004
/* 006B4 80B43474 51200017 */  beql    $t1, $zero, .L80B434D4     
/* 006B8 80B43478 8FBF004C */  lw      $ra, 0x004C($sp)           
/* 006BC 80B4347C 860A0032 */  lh      $t2, 0x0032($s0)           ## 00000032
/* 006C0 80B43480 C60C0024 */  lwc1    $f12, 0x0024($s0)          ## 00000024
/* 006C4 80B43484 C60E0028 */  lwc1    $f14, 0x0028($s0)          ## 00000028
/* 006C8 80B43488 8E06002C */  lw      $a2, 0x002C($s0)           ## 0000002C
/* 006CC 80B4348C 86070030 */  lh      $a3, 0x0030($s0)           ## 00000030
/* 006D0 80B43490 AFAA0010 */  sw      $t2, 0x0010($sp)           
/* 006D4 80B43494 860B0034 */  lh      $t3, 0x0034($s0)           ## 00000034
/* 006D8 80B43498 44810000 */  mtc1    $at, $f0                   ## $f0 = 1.00
/* 006DC 80B4349C 8FAE0074 */  lw      $t6, 0x0074($sp)           
/* 006E0 80B434A0 AFB90034 */  sw      $t9, 0x0034($sp)           
/* 006E4 80B434A4 AFAD0030 */  sw      $t5, 0x0030($sp)           
/* 006E8 80B434A8 AFA0002C */  sw      $zero, 0x002C($sp)         
/* 006EC 80B434AC AFAC0028 */  sw      $t4, 0x0028($sp)           
/* 006F0 80B434B0 AFA00024 */  sw      $zero, 0x0024($sp)         
/* 006F4 80B434B4 AFAB0014 */  sw      $t3, 0x0014($sp)           
/* 006F8 80B434B8 E7A00018 */  swc1    $f0, 0x0018($sp)           
/* 006FC 80B434BC E7A0001C */  swc1    $f0, 0x001C($sp)           
/* 00700 80B434C0 E7A00020 */  swc1    $f0, 0x0020($sp)           
/* 00704 80B434C4 8DCF0000 */  lw      $t7, 0x0000($t6)           ## 00000000
/* 00708 80B434C8 0C018FA7 */  jal     DebugDisplay_AddObject
              
/* 0070C 80B434CC AFAF0038 */  sw      $t7, 0x0038($sp)           
/* 00710 80B434D0 8FBF004C */  lw      $ra, 0x004C($sp)           
.L80B434D4:
/* 00714 80B434D4 8FB00048 */  lw      $s0, 0x0048($sp)           
/* 00718 80B434D8 27BD0070 */  addiu   $sp, $sp, 0x0070           ## $sp = 00000000
/* 0071C 80B434DC 03E00008 */  jr      $ra                        
/* 00720 80B434E0 00000000 */  nop
/* 00724 80B434E4 00000000 */  nop
/* 00728 80B434E8 00000000 */  nop
/* 0072C 80B434EC 00000000 */  nop