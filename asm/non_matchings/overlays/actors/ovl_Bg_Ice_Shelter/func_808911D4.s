.late_rodata
glabel D_808918EC
 .word 0x38D1B717

.text
glabel func_808911D4
/* 00A94 808911D4 27BDFFC0 */  addiu   $sp, $sp, 0xFFC0           ## $sp = FFFFFFC0
/* 00A98 808911D8 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 00A9C 808911DC AFB00018 */  sw      $s0, 0x0018($sp)           
/* 00AA0 808911E0 AFA50044 */  sw      $a1, 0x0044($sp)           
/* 00AA4 808911E4 848E0200 */  lh      $t6, 0x0200($a0)           ## 00000200
/* 00AA8 808911E8 8486001C */  lh      $a2, 0x001C($a0)           ## 0000001C
/* 00AAC 808911EC 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 00AB0 808911F0 25CFFFFB */  addiu   $t7, $t6, 0xFFFB           ## $t7 = FFFFFFFB
/* 00AB4 808911F4 A48F0200 */  sh      $t7, 0x0200($a0)           ## 00000200
/* 00AB8 808911F8 84820200 */  lh      $v0, 0x0200($a0)           ## 00000200
/* 00ABC 808911FC 00063203 */  sra     $a2, $a2,  8               
/* 00AC0 80891200 30C60007 */  andi    $a2, $a2, 0x0007           ## $a2 = 00000000
/* 00AC4 80891204 04410003 */  bgez    $v0, .L80891214            
/* 00AC8 80891208 28410100 */  slti    $at, $v0, 0x0100           
/* 00ACC 8089120C 10000006 */  beq     $zero, $zero, .L80891228   
/* 00AD0 80891210 A4800200 */  sh      $zero, 0x0200($a0)         ## 00000200
.L80891214:
/* 00AD4 80891214 14200003 */  bne     $at, $zero, .L80891224     
/* 00AD8 80891218 00401825 */  or      $v1, $v0, $zero            ## $v1 = 00000000
/* 00ADC 8089121C 10000001 */  beq     $zero, $zero, .L80891224   
/* 00AE0 80891220 240300FF */  addiu   $v1, $zero, 0x00FF         ## $v1 = 000000FF
.L80891224:
/* 00AE4 80891224 A6030200 */  sh      $v1, 0x0200($s0)           ## 00000200
.L80891228:
/* 00AE8 80891228 3C018089 */  lui     $at, %hi(D_808918EC)       ## $at = 80890000
/* 00AEC 8089122C C42218EC */  lwc1    $f2, %lo(D_808918EC)($at)  
/* 00AF0 80891230 3C018089 */  lui     $at, %hi(D_808917BC)       ## $at = 80890000
/* 00AF4 80891234 00061880 */  sll     $v1, $a2,  2               
/* 00AF8 80891238 00230821 */  addu    $at, $at, $v1              
/* 00AFC 8089123C C42617BC */  lwc1    $f6, %lo(D_808917BC)($at)  
/* 00B00 80891240 C6040054 */  lwc1    $f4, 0x0054($s0)           ## 00000054
/* 00B04 80891244 46062200 */  add.s   $f8, $f4, $f6              
/* 00B08 80891248 E6080054 */  swc1    $f8, 0x0054($s0)           ## 00000054
/* 00B0C 8089124C C6000054 */  lwc1    $f0, 0x0054($s0)           ## 00000054
/* 00B10 80891250 4602003C */  c.lt.s  $f0, $f2                   
/* 00B14 80891254 00000000 */  nop
/* 00B18 80891258 45020004 */  bc1fl   .L8089126C                 
/* 00B1C 8089125C E6000054 */  swc1    $f0, 0x0054($s0)           ## 00000054
/* 00B20 80891260 10000002 */  beq     $zero, $zero, .L8089126C   
/* 00B24 80891264 E6020054 */  swc1    $f2, 0x0054($s0)           ## 00000054
/* 00B28 80891268 E6000054 */  swc1    $f0, 0x0054($s0)           ## 00000054
.L8089126C:
/* 00B2C 8089126C 86020200 */  lh      $v0, 0x0200($s0)           ## 00000200
/* 00B30 80891270 28410051 */  slti    $at, $v0, 0x0051           
/* 00B34 80891274 54200016 */  bnel    $at, $zero, .L808912D0     
/* 00B38 80891278 284100B5 */  slti    $at, $v0, 0x00B5           
/* 00B3C 8089127C 10C00005 */  beq     $a2, $zero, .L80891294     
/* 00B40 80891280 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 00B44 80891284 10C10003 */  beq     $a2, $at, .L80891294       
/* 00B48 80891288 24010004 */  addiu   $at, $zero, 0x0004         ## $at = 00000004
/* 00B4C 8089128C 54C10010 */  bnel    $a2, $at, .L808912D0       
/* 00B50 80891290 284100B5 */  slti    $at, $v0, 0x00B5           
.L80891294:
/* 00B54 80891294 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 00B58 80891298 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00B5C 8089129C 34211E60 */  ori     $at, $at, 0x1E60           ## $at = 00011E60
/* 00B60 808912A0 00812821 */  addu    $a1, $a0, $at              
/* 00B64 808912A4 AFA5002C */  sw      $a1, 0x002C($sp)           
/* 00B68 808912A8 26060168 */  addiu   $a2, $s0, 0x0168           ## $a2 = 00000168
/* 00B6C 808912AC 0C017713 */  jal     CollisionCheck_SetOC
              ## CollisionCheck_setOT
/* 00B70 808912B0 AFA30028 */  sw      $v1, 0x0028($sp)           
/* 00B74 808912B4 8FA5002C */  lw      $a1, 0x002C($sp)           
/* 00B78 808912B8 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 00B7C 808912BC 0C01767D */  jal     CollisionCheck_SetAC
              ## CollisionCheck_setAC
/* 00B80 808912C0 260601B4 */  addiu   $a2, $s0, 0x01B4           ## $a2 = 000001B4
/* 00B84 808912C4 8FA30028 */  lw      $v1, 0x0028($sp)           
/* 00B88 808912C8 86020200 */  lh      $v0, 0x0200($s0)           ## 00000200
/* 00B8C 808912CC 284100B5 */  slti    $at, $v0, 0x00B5           
.L808912D0:
/* 00B90 808912D0 14200005 */  bne     $at, $zero, .L808912E8     
/* 00B94 808912D4 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00B98 808912D8 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 00B9C 808912DC 44810000 */  mtc1    $at, $f0                   ## $f0 = 1.00
/* 00BA0 808912E0 10000009 */  beq     $zero, $zero, .L80891308   
/* 00BA4 808912E4 00000000 */  nop
.L808912E8:
/* 00BA8 808912E8 2841003D */  slti    $at, $v0, 0x003D           
/* 00BAC 808912EC 14200004 */  bne     $at, $zero, .L80891300     
/* 00BB0 808912F0 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 00BB4 808912F4 44810000 */  mtc1    $at, $f0                   ## $f0 = 0.50
/* 00BB8 808912F8 10000003 */  beq     $zero, $zero, .L80891308   
/* 00BBC 808912FC 00000000 */  nop
.L80891300:
/* 00BC0 80891300 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 00BC4 80891304 00000000 */  nop
.L80891308:
/* 00BC8 80891308 3C198089 */  lui     $t9, %hi(D_808917E4)       ## $t9 = 80890000
/* 00BCC 8089130C 273917E4 */  addiu   $t9, $t9, %lo(D_808917E4)  ## $t9 = 808917E4
/* 00BD0 80891310 00791021 */  addu    $v0, $v1, $t9              
/* 00BD4 80891314 8C590000 */  lw      $t9, 0x0000($v0)           ## 00000000
/* 00BD8 80891318 3C078089 */  lui     $a3, %hi(D_808917D0)       ## $a3 = 80890000
/* 00BDC 8089131C 00E33821 */  addu    $a3, $a3, $v1              
/* 00BE0 80891320 44060000 */  mfc1    $a2, $f0                   
/* 00BE4 80891324 8CE717D0 */  lw      $a3, %lo(D_808917D0)($a3)  
/* 00BE8 80891328 AFA20024 */  sw      $v0, 0x0024($sp)           
/* 00BEC 8089132C 0320F809 */  jalr    $ra, $t9                   
/* 00BF0 80891330 8FA50044 */  lw      $a1, 0x0044($sp)           
/* 00BF4 80891334 86080200 */  lh      $t0, 0x0200($s0)           ## 00000200
/* 00BF8 80891338 5D000013 */  bgtzl   $t0, .L80891388            
/* 00BFC 8089133C 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 00C00 80891340 8602001C */  lh      $v0, 0x001C($s0)           ## 0000001C
/* 00C04 80891344 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 00C08 80891348 00024983 */  sra     $t1, $v0,  6               
/* 00C0C 8089134C 312A0001 */  andi    $t2, $t1, 0x0001           ## $t2 = 00000000
/* 00C10 80891350 55400004 */  bnel    $t2, $zero, .L80891364     
/* 00C14 80891354 8FAB0024 */  lw      $t3, 0x0024($sp)           
/* 00C18 80891358 0C00B2DD */  jal     Flags_SetSwitch
              
/* 00C1C 8089135C 3045003F */  andi    $a1, $v0, 0x003F           ## $a1 = 00000000
/* 00C20 80891360 8FAB0024 */  lw      $t3, 0x0024($sp)           
.L80891364:
/* 00C24 80891364 3C0C8089 */  lui     $t4, %hi(D_808917F4)       ## $t4 = 80890000
/* 00C28 80891368 258C17F4 */  addiu   $t4, $t4, %lo(D_808917F4)  ## $t4 = 808917F4
/* 00C2C 8089136C 156C0003 */  bne     $t3, $t4, .L8089137C       
/* 00C30 80891370 00000000 */  nop
/* 00C34 80891374 0C01E221 */  jal     func_80078884              
/* 00C38 80891378 24044802 */  addiu   $a0, $zero, 0x4802         ## $a0 = 00004802
.L8089137C:
/* 00C3C 8089137C 0C00B55C */  jal     Actor_Kill
              
/* 00C40 80891380 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00C44 80891384 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80891388:
/* 00C48 80891388 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 00C4C 8089138C 27BD0040 */  addiu   $sp, $sp, 0x0040           ## $sp = 00000000
/* 00C50 80891390 03E00008 */  jr      $ra                        
/* 00C54 80891394 00000000 */  nop