.late_rodata
glabel D_809CE4C8
    .float 5000.0

.text
glabel EnButte_Update
/* 010AC 809CE11C 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 010B0 809CE120 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 010B4 809CE124 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 010B8 809CE128 8C82011C */  lw      $v0, 0x011C($a0)           ## 0000011C
/* 010BC 809CE12C 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 010C0 809CE130 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 010C4 809CE134 50400008 */  beql    $v0, $zero, .L809CE158     
/* 010C8 809CE138 86020254 */  lh      $v0, 0x0254($s0)           ## 00000254
/* 010CC 809CE13C 8C4E0130 */  lw      $t6, 0x0130($v0)           ## 00000130
/* 010D0 809CE140 55C00005 */  bnel    $t6, $zero, .L809CE158     
/* 010D4 809CE144 86020254 */  lh      $v0, 0x0254($s0)           ## 00000254
/* 010D8 809CE148 50820003 */  beql    $a0, $v0, .L809CE158       
/* 010DC 809CE14C 86020254 */  lh      $v0, 0x0254($s0)           ## 00000254
/* 010E0 809CE150 AC80011C */  sw      $zero, 0x011C($a0)         ## 0000011C
/* 010E4 809CE154 86020254 */  lh      $v0, 0x0254($s0)           ## 00000254
.L809CE158:
/* 010E8 809CE158 18400002 */  blez    $v0, .L809CE164            
/* 010EC 809CE15C 244FFFFF */  addiu   $t7, $v0, 0xFFFF           ## $t7 = FFFFFFFF
/* 010F0 809CE160 A60F0254 */  sh      $t7, 0x0254($s0)           ## 00000254
.L809CE164:
/* 010F4 809CE164 8618025C */  lh      $t8, 0x025C($s0)           ## 0000025C
/* 010F8 809CE168 8608025E */  lh      $t0, 0x025E($s0)           ## 0000025E
/* 010FC 809CE16C 860A0260 */  lh      $t2, 0x0260($s0)           ## 00000260
/* 01100 809CE170 860C001C */  lh      $t4, 0x001C($s0)           ## 0000001C
/* 01104 809CE174 27190222 */  addiu   $t9, $t8, 0x0222           ## $t9 = 00000222
/* 01108 809CE178 25091000 */  addiu   $t1, $t0, 0x1000           ## $t1 = 00001000
/* 0110C 809CE17C 254B0600 */  addiu   $t3, $t2, 0x0600           ## $t3 = 00000600
/* 01110 809CE180 318D0001 */  andi    $t5, $t4, 0x0001           ## $t5 = 00000000
/* 01114 809CE184 A619025C */  sh      $t9, 0x025C($s0)           ## 0000025C
/* 01118 809CE188 A609025E */  sh      $t1, 0x025E($s0)           ## 0000025E
/* 0111C 809CE18C 15A1000C */  bne     $t5, $at, .L809CE1C0       
/* 01120 809CE190 A60B0260 */  sh      $t3, 0x0260($s0)           ## 00000260
/* 01124 809CE194 8CAE1C44 */  lw      $t6, 0x1C44($a1)           ## 00001C44
/* 01128 809CE198 24190050 */  addiu   $t9, $zero, 0x0050         ## $t9 = 00000050
/* 0112C 809CE19C 81CF0843 */  lb      $t7, 0x0843($t6)           ## 00000843
/* 01130 809CE1A0 55E00007 */  bnel    $t7, $zero, .L809CE1C0     
/* 01134 809CE1A4 A619025A */  sh      $t9, 0x025A($s0)           ## 0000025A
/* 01138 809CE1A8 8602025A */  lh      $v0, 0x025A($s0)           ## 0000025A
/* 0113C 809CE1AC 18400004 */  blez    $v0, .L809CE1C0            
/* 01140 809CE1B0 2458FFFF */  addiu   $t8, $v0, 0xFFFF           ## $t8 = FFFFFFFF
/* 01144 809CE1B4 10000002 */  beq     $zero, $zero, .L809CE1C0   
/* 01148 809CE1B8 A618025A */  sh      $t8, 0x025A($s0)           ## 0000025A
/* 0114C 809CE1BC A619025A */  sh      $t9, 0x025A($s0)           ## 0000025A
.L809CE1C0:
/* 01150 809CE1C0 AFA50024 */  sw      $a1, 0x0024($sp)           
/* 01154 809CE1C4 8E190250 */  lw      $t9, 0x0250($s0)           ## 00000250
/* 01158 809CE1C8 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0115C 809CE1CC 0320F809 */  jalr    $ra, $t9                   
/* 01160 809CE1D0 00000000 */  nop
/* 01164 809CE1D4 8E080130 */  lw      $t0, 0x0130($s0)           ## 00000130
/* 01168 809CE1D8 5100001C */  beql    $t0, $zero, .L809CE24C     
/* 0116C 809CE1DC 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 01170 809CE1E0 0C00B638 */  jal     Actor_MoveForward
              
/* 01174 809CE1E4 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 01178 809CE1E8 3C063F19 */  lui     $a2, 0x3F19                ## $a2 = 3F190000
/* 0117C 809CE1EC 34C6999A */  ori     $a2, $a2, 0x999A           ## $a2 = 3F19999A
/* 01180 809CE1F0 26040028 */  addiu   $a0, $s0, 0x0028           ## $a0 = 00000028
/* 01184 809CE1F4 0C01DE80 */  jal     Math_ApproxF
              
/* 01188 809CE1F8 8E050264 */  lw      $a1, 0x0264($s0)           ## 00000264
/* 0118C 809CE1FC 3C01809D */  lui     $at, %hi(D_809CE4C8)       ## $at = 809D0000
/* 01190 809CE200 C426E4C8 */  lwc1    $f6, %lo(D_809CE4C8)($at)  
/* 01194 809CE204 C604008C */  lwc1    $f4, 0x008C($s0)           ## 0000008C
/* 01198 809CE208 8FA40024 */  lw      $a0, 0x0024($sp)           
/* 0119C 809CE20C 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 011A0 809CE210 4606203C */  c.lt.s  $f4, $f6                   
/* 011A4 809CE214 34211E60 */  ori     $at, $at, 0x1E60           ## $at = 00011E60
/* 011A8 809CE218 00812821 */  addu    $a1, $a0, $at              
/* 011AC 809CE21C 45020004 */  bc1fl   .L809CE230                 
/* 011B0 809CE220 C60800BC */  lwc1    $f8, 0x00BC($s0)           ## 000000BC
/* 011B4 809CE224 0C017713 */  jal     CollisionCheck_SetOC
              ## CollisionCheck_setOT
/* 011B8 809CE228 2606014C */  addiu   $a2, $s0, 0x014C           ## $a2 = 0000014C
/* 011BC 809CE22C C60800BC */  lwc1    $f8, 0x00BC($s0)           ## 000000BC
.L809CE230:
/* 011C0 809CE230 C60A0054 */  lwc1    $f10, 0x0054($s0)          ## 00000054
/* 011C4 809CE234 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 011C8 809CE238 460A4402 */  mul.s   $f16, $f8, $f10            
/* 011CC 809CE23C 44058000 */  mfc1    $a1, $f16                  
/* 011D0 809CE240 0C00B56E */  jal     Actor_SetHeight
              
/* 011D4 809CE244 00000000 */  nop
/* 011D8 809CE248 8FBF001C */  lw      $ra, 0x001C($sp)           
.L809CE24C:
/* 011DC 809CE24C 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 011E0 809CE250 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 011E4 809CE254 03E00008 */  jr      $ra                        
/* 011E8 809CE258 00000000 */  nop
