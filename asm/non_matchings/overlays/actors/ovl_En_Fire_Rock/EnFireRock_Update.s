.rdata
glabel D_80A12E24
    .asciz "[33m☆☆☆☆☆ シールド防御 Lv１ ☆☆☆☆☆ \n[m"
    .balign 4

.late_rodata
glabel D_80A12EDC
 .word 0xBE99999A

.text
glabel EnFireRock_Update
/* 00BF4 80A12814 27BDFFB8 */  addiu   $sp, $sp, 0xFFB8           ## $sp = FFFFFFB8
/* 00BF8 80A12818 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 00BFC 80A1281C AFB00020 */  sw      $s0, 0x0020($sp)           
/* 00C00 80A12820 8CA21C44 */  lw      $v0, 0x1C44($a1)           ## 00001C44
/* 00C04 80A12824 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 00C08 80A12828 AFA2003C */  sw      $v0, 0x003C($sp)           
/* 00C0C 80A1282C AFA20038 */  sw      $v0, 0x0038($sp)           
/* 00C10 80A12830 8483018A */  lh      $v1, 0x018A($a0)           ## 0000018A
/* 00C14 80A12834 10600002 */  beq     $v1, $zero, .L80A12840     
/* 00C18 80A12838 246EFFFF */  addiu   $t6, $v1, 0xFFFF           ## $t6 = FFFFFFFF
/* 00C1C 80A1283C A48E018A */  sh      $t6, 0x018A($a0)           ## 0000018A
.L80A12840:
/* 00C20 80A12840 86020188 */  lh      $v0, 0x0188($s0)           ## 00000188
/* 00C24 80A12844 10400002 */  beq     $v0, $zero, .L80A12850     
/* 00C28 80A12848 244FFFFF */  addiu   $t7, $v0, 0xFFFF           ## $t7 = FFFFFFFF
/* 00C2C 80A1284C A60F0188 */  sh      $t7, 0x0188($s0)           ## 00000188
.L80A12850:
/* 00C30 80A12850 AFA5004C */  sw      $a1, 0x004C($sp)           
/* 00C34 80A12854 8E190168 */  lw      $t9, 0x0168($s0)           ## 00000168
/* 00C38 80A12858 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00C3C 80A1285C 0320F809 */  jalr    $ra, $t9                   
/* 00C40 80A12860 00000000 */  nop
/* 00C44 80A12864 8618018C */  lh      $t8, 0x018C($s0)           ## 0000018C
/* 00C48 80A12868 24010005 */  addiu   $at, $zero, 0x0005         ## $at = 00000005
/* 00C4C 80A1286C 53010094 */  beql    $t8, $at, .L80A12AC0       
/* 00C50 80A12870 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 00C54 80A12874 C60A015C */  lwc1    $f10, 0x015C($s0)          ## 0000015C
/* 00C58 80A12878 C6100150 */  lwc1    $f16, 0x0150($s0)          ## 00000150
/* 00C5C 80A1287C C6040158 */  lwc1    $f4, 0x0158($s0)           ## 00000158
/* 00C60 80A12880 C606014C */  lwc1    $f6, 0x014C($s0)           ## 0000014C
/* 00C64 80A12884 46105480 */  add.s   $f18, $f10, $f16           
/* 00C68 80A12888 3C014120 */  lui     $at, 0x4120                ## $at = 41200000
/* 00C6C 80A1288C 44811000 */  mtc1    $at, $f2                   ## $f2 = 10.00
/* 00C70 80A12890 46062200 */  add.s   $f8, $f4, $f6              
/* 00C74 80A12894 3C014040 */  lui     $at, 0x4040                ## $at = 40400000
/* 00C78 80A12898 44815000 */  mtc1    $at, $f10                  ## $f10 = 3.00
/* 00C7C 80A1289C C6060154 */  lwc1    $f6, 0x0154($s0)           ## 00000154
/* 00C80 80A128A0 C6040160 */  lwc1    $f4, 0x0160($s0)           ## 00000160
/* 00C84 80A128A4 E612015C */  swc1    $f18, 0x015C($s0)          ## 0000015C
/* 00C88 80A128A8 3C014396 */  lui     $at, 0x4396                ## $at = 43960000
/* 00C8C 80A128AC 44819000 */  mtc1    $at, $f18                  ## $f18 = 300.00
/* 00C90 80A128B0 C610016C */  lwc1    $f16, 0x016C($s0)          ## 0000016C
/* 00C94 80A128B4 E6080158 */  swc1    $f8, 0x0158($s0)           ## 00000158
/* 00C98 80A128B8 46062200 */  add.s   $f8, $f4, $f6              
/* 00C9C 80A128BC 46128102 */  mul.s   $f4, $f16, $f18            
/* 00CA0 80A128C0 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 00CA4 80A128C4 E60A0174 */  swc1    $f10, 0x0174($s0)          ## 00000174
/* 00CA8 80A128C8 E6080160 */  swc1    $f8, 0x0160($s0)           ## 00000160
/* 00CAC 80A128CC 46022000 */  add.s   $f0, $f4, $f2              
/* 00CB0 80A128D0 4602003C */  c.lt.s  $f0, $f2                   
/* 00CB4 80A128D4 E60000C4 */  swc1    $f0, 0x00C4($s0)           ## 000000C4
/* 00CB8 80A128D8 44810000 */  mtc1    $at, $f0                   ## $f0 = 20.00
/* 00CBC 80A128DC 24010003 */  addiu   $at, $zero, 0x0003         ## $at = 00000003
/* 00CC0 80A128E0 45020003 */  bc1fl   .L80A128F0                 
/* 00CC4 80A128E4 C60600C4 */  lwc1    $f6, 0x00C4($s0)           ## 000000C4
/* 00CC8 80A128E8 E60200C4 */  swc1    $f2, 0x00C4($s0)           ## 000000C4
/* 00CCC 80A128EC C60600C4 */  lwc1    $f6, 0x00C4($s0)           ## 000000C4
.L80A128F0:
/* 00CD0 80A128F0 4606003C */  c.lt.s  $f0, $f6                   
/* 00CD4 80A128F4 00000000 */  nop
/* 00CD8 80A128F8 45020003 */  bc1fl   .L80A12908                 
/* 00CDC 80A128FC 8603018C */  lh      $v1, 0x018C($s0)           ## 0000018C
/* 00CE0 80A12900 E60000C4 */  swc1    $f0, 0x00C4($s0)           ## 000000C4
/* 00CE4 80A12904 8603018C */  lh      $v1, 0x018C($s0)           ## 0000018C
.L80A12908:
/* 00CE8 80A12908 50600003 */  beql    $v1, $zero, .L80A12918     
/* 00CEC 80A1290C 3C0140E0 */  lui     $at, 0x40E0                ## $at = 40E00000
/* 00CF0 80A12910 14610009 */  bne     $v1, $at, .L80A12938       
/* 00CF4 80A12914 3C0140E0 */  lui     $at, 0x40E0                ## $at = 40E00000
.L80A12918:
/* 00CF8 80A12918 44815000 */  mtc1    $at, $f10                  ## $f10 = 7.00
/* 00CFC 80A1291C C608016C */  lwc1    $f8, 0x016C($s0)           ## 0000016C
/* 00D00 80A12920 3C0180A1 */  lui     $at, %hi(D_80A12EDC)       ## $at = 80A10000
/* 00D04 80A12924 C4322EDC */  lwc1    $f18, %lo(D_80A12EDC)($at) 
/* 00D08 80A12928 460A4402 */  mul.s   $f16, $f8, $f10            
/* 00D0C 80A1292C 8603018C */  lh      $v1, 0x018C($s0)           ## 0000018C
/* 00D10 80A12930 46109101 */  sub.s   $f4, $f18, $f16            
/* 00D14 80A12934 E604006C */  swc1    $f4, 0x006C($s0)           ## 0000006C
.L80A12938:
/* 00D18 80A12938 24010006 */  addiu   $at, $zero, 0x0006         ## $at = 00000006
/* 00D1C 80A1293C 50610011 */  beql    $v1, $at, .L80A12984       
/* 00D20 80A12940 8E090168 */  lw      $t1, 0x0168($s0)           ## 00000168
/* 00D24 80A12944 0C00B638 */  jal     Actor_MoveForward
              
/* 00D28 80A12948 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00D2C 80A1294C 3C014248 */  lui     $at, 0x4248                ## $at = 42480000
/* 00D30 80A12950 44810000 */  mtc1    $at, $f0                   ## $f0 = 50.00
/* 00D34 80A12954 3C0142C8 */  lui     $at, 0x42C8                ## $at = 42C80000
/* 00D38 80A12958 44813000 */  mtc1    $at, $f6                   ## $f6 = 100.00
/* 00D3C 80A1295C 2408001C */  addiu   $t0, $zero, 0x001C         ## $t0 = 0000001C
/* 00D40 80A12960 44060000 */  mfc1    $a2, $f0                   
/* 00D44 80A12964 44070000 */  mfc1    $a3, $f0                   
/* 00D48 80A12968 AFA80014 */  sw      $t0, 0x0014($sp)           
/* 00D4C 80A1296C 8FA4004C */  lw      $a0, 0x004C($sp)           
/* 00D50 80A12970 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 00D54 80A12974 0C00B92D */  jal     func_8002E4B4              
/* 00D58 80A12978 E7A60010 */  swc1    $f6, 0x0010($sp)           
/* 00D5C 80A1297C 8603018C */  lh      $v1, 0x018C($s0)           ## 0000018C
/* 00D60 80A12980 8E090168 */  lw      $t1, 0x0168($s0)           ## 00000168
.L80A12984:
/* 00D64 80A12984 3C0680A1 */  lui     $a2, %hi(func_80A1241C)    ## $a2 = 80A10000
/* 00D68 80A12988 24C6241C */  addiu   $a2, $a2, %lo(func_80A1241C) ## $a2 = 80A1241C
/* 00D6C 80A1298C 10C9001B */  beq     $a2, $t1, .L80A129FC       
/* 00D70 80A12990 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 00D74 80A12994 10600005 */  beq     $v1, $zero, .L80A129AC     
/* 00D78 80A12998 24010003 */  addiu   $at, $zero, 0x0003         ## $at = 00000003
/* 00D7C 80A1299C 10610003 */  beq     $v1, $at, .L80A129AC       
/* 00D80 80A129A0 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 00D84 80A129A4 54610016 */  bnel    $v1, $at, .L80A12A00       
/* 00D88 80A129A8 24010006 */  addiu   $at, $zero, 0x0006         ## $at = 00000006
.L80A129AC:
/* 00D8C 80A129AC 920201A4 */  lbu     $v0, 0x01A4($s0)           ## 000001A4
/* 00D90 80A129B0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00D94 80A129B4 2405280E */  addiu   $a1, $zero, 0x280E         ## $a1 = 0000280E
/* 00D98 80A129B8 304A0004 */  andi    $t2, $v0, 0x0004           ## $t2 = 00000000
/* 00D9C 80A129BC 1140000E */  beq     $t2, $zero, .L80A129F8     
/* 00DA0 80A129C0 304BFFFB */  andi    $t3, $v0, 0xFFFB           ## $t3 = 00000000
/* 00DA4 80A129C4 A20B01A4 */  sb      $t3, 0x01A4($s0)           ## 000001A4
/* 00DA8 80A129C8 0C00BE0A */  jal     Audio_PlayActorSound2
              
/* 00DAC 80A129CC AFA60030 */  sw      $a2, 0x0030($sp)           
/* 00DB0 80A129D0 44800000 */  mtc1    $zero, $f0                 ## $f0 = 0.00
/* 00DB4 80A129D4 8FA60030 */  lw      $a2, 0x0030($sp)           
/* 00DB8 80A129D8 3C0480A1 */  lui     $a0, %hi(D_80A12E24)       ## $a0 = 80A10000
/* 00DBC 80A129DC 24842E24 */  addiu   $a0, $a0, %lo(D_80A12E24)  ## $a0 = 80A12E24
/* 00DC0 80A129E0 E6000060 */  swc1    $f0, 0x0060($s0)           ## 00000060
/* 00DC4 80A129E4 E6000068 */  swc1    $f0, 0x0068($s0)           ## 00000068
/* 00DC8 80A129E8 0C00084C */  jal     osSyncPrintf
              
/* 00DCC 80A129EC AE060168 */  sw      $a2, 0x0168($s0)           ## 00000168
/* 00DD0 80A129F0 10000033 */  beq     $zero, $zero, .L80A12AC0   
/* 00DD4 80A129F4 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80A129F8:
/* 00DD8 80A129F8 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
.L80A129FC:
/* 00DDC 80A129FC 24010006 */  addiu   $at, $zero, 0x0006         ## $at = 00000006
.L80A12A00:
/* 00DE0 80A12A00 1461001E */  bne     $v1, $at, .L80A12A7C       
/* 00DE4 80A12A04 00000000 */  nop
/* 00DE8 80A12A08 920201A4 */  lbu     $v0, 0x01A4($s0)           ## 000001A4
/* 00DEC 80A12A0C 304C0002 */  andi    $t4, $v0, 0x0002           ## $t4 = 00000000
/* 00DF0 80A12A10 11800019 */  beq     $t4, $zero, .L80A12A78     
/* 00DF4 80A12A14 304DFFFD */  andi    $t5, $v0, 0xFFFD           ## $t5 = 00000001
/* 00DF8 80A12A18 A20D01A4 */  sb      $t5, 0x01A4($s0)           ## 000001A4
/* 00DFC 80A12A1C 8FAE0038 */  lw      $t6, 0x0038($sp)           
/* 00E00 80A12A20 8E0F0198 */  lw      $t7, 0x0198($s0)           ## 00000198
/* 00E04 80A12A24 8FA2003C */  lw      $v0, 0x003C($sp)           
/* 00E08 80A12A28 55CF0014 */  bnel    $t6, $t7, .L80A12A7C       
/* 00E0C 80A12A2C 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
/* 00E10 80A12A30 8C59067C */  lw      $t9, 0x067C($v0)           ## 0000067D
/* 00E14 80A12A34 8FA4004C */  lw      $a0, 0x004C($sp)           
/* 00E18 80A12A38 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 00E1C 80A12A3C 0019C140 */  sll     $t8, $t9,  5               
/* 00E20 80A12A40 0700001E */  bltz    $t8, .L80A12ABC            
/* 00E24 80A12A44 3C064000 */  lui     $a2, 0x4000                ## $a2 = 40000000
/* 00E28 80A12A48 84470032 */  lh      $a3, 0x0032($v0)           ## 00000033
/* 00E2C 80A12A4C 3C014040 */  lui     $at, 0x4040                ## $at = 40400000
/* 00E30 80A12A50 44814000 */  mtc1    $at, $f8                   ## $f8 = 3.00
/* 00E34 80A12A54 00073823 */  subu    $a3, $zero, $a3            
/* 00E38 80A12A58 00073C00 */  sll     $a3, $a3, 16               
/* 00E3C 80A12A5C 24080004 */  addiu   $t0, $zero, 0x0004         ## $t0 = 00000004
/* 00E40 80A12A60 AFA80014 */  sw      $t0, 0x0014($sp)           
/* 00E44 80A12A64 00073C03 */  sra     $a3, $a3, 16               
/* 00E48 80A12A68 0C00BDD6 */  jal     func_8002F758              
/* 00E4C 80A12A6C E7A80010 */  swc1    $f8, 0x0010($sp)           
/* 00E50 80A12A70 10000013 */  beq     $zero, $zero, .L80A12AC0   
/* 00E54 80A12A74 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80A12A78:
/* 00E58 80A12A78 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
.L80A12A7C:
/* 00E5C 80A12A7C 1040000F */  beq     $v0, $zero, .L80A12ABC     
/* 00E60 80A12A80 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 00E64 80A12A84 26050194 */  addiu   $a1, $s0, 0x0194           ## $a1 = 00000194
/* 00E68 80A12A88 0C0189B7 */  jal     Collider_CylinderUpdate
              
/* 00E6C 80A12A8C AFA5002C */  sw      $a1, 0x002C($sp)           
/* 00E70 80A12A90 8FA4004C */  lw      $a0, 0x004C($sp)           
/* 00E74 80A12A94 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 00E78 80A12A98 34211E60 */  ori     $at, $at, 0x1E60           ## $at = 00011E60
/* 00E7C 80A12A9C 00812821 */  addu    $a1, $a0, $at              
/* 00E80 80A12AA0 AFA50030 */  sw      $a1, 0x0030($sp)           
/* 00E84 80A12AA4 0C0175E7 */  jal     CollisionCheck_SetAT
              ## CollisionCheck_setAT
/* 00E88 80A12AA8 8FA6002C */  lw      $a2, 0x002C($sp)           
/* 00E8C 80A12AAC 8FA50030 */  lw      $a1, 0x0030($sp)           
/* 00E90 80A12AB0 8FA4004C */  lw      $a0, 0x004C($sp)           
/* 00E94 80A12AB4 0C01767D */  jal     CollisionCheck_SetAC
              ## CollisionCheck_setAC
/* 00E98 80A12AB8 8FA6002C */  lw      $a2, 0x002C($sp)           
.L80A12ABC:
/* 00E9C 80A12ABC 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80A12AC0:
/* 00EA0 80A12AC0 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 00EA4 80A12AC4 27BD0048 */  addiu   $sp, $sp, 0x0048           ## $sp = 00000000
/* 00EA8 80A12AC8 03E00008 */  jr      $ra                        
/* 00EAC 80A12ACC 00000000 */  nop