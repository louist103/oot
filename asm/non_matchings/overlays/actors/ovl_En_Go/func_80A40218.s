glabel func_80A40218
/* 01CA8 80A40218 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 01CAC 80A4021C AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 01CB0 80A40220 AFB00020 */  sw      $s0, 0x0020($sp)           
/* 01CB4 80A40224 AFA5002C */  sw      $a1, 0x002C($sp)           
/* 01CB8 80A40228 84820210 */  lh      $v0, 0x0210($a0)           ## 00000210
/* 01CBC 80A4022C 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 01CC0 80A40230 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 01CC4 80A40234 14400003 */  bne     $v0, $zero, .L80A40244     
/* 01CC8 80A40238 244EFFFF */  addiu   $t6, $v0, 0xFFFF           ## $t6 = FFFFFFFF
/* 01CCC 80A4023C 10000003 */  beq     $zero, $zero, .L80A4024C   
/* 01CD0 80A40240 00001825 */  or      $v1, $zero, $zero          ## $v1 = 00000000
.L80A40244:
/* 01CD4 80A40244 A60E0210 */  sh      $t6, 0x0210($s0)           ## 00000210
/* 01CD8 80A40248 86030210 */  lh      $v1, 0x0210($s0)           ## 00000210
.L80A4024C:
/* 01CDC 80A4024C 5460002E */  bnel    $v1, $zero, .L80A40308     
/* 01CE0 80A40250 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 01CE4 80A40254 0C28FC57 */  jal     func_80A3F15C              
/* 01CE8 80A40258 8FA5002C */  lw      $a1, 0x002C($sp)           
/* 01CEC 80A4025C 10400029 */  beq     $v0, $zero, .L80A40304     
/* 01CF0 80A40260 3C078013 */  lui     $a3, %hi(D_801333E0)
/* 01CF4 80A40264 3C0F8013 */  lui     $t7, %hi(D_801333E8)
/* 01CF8 80A40268 24E733E0 */  addiu   $a3, %lo(D_801333E0)
/* 01CFC 80A4026C 25EF33E8 */  addiu   $t7, %lo(D_801333E8)
/* 01D00 80A40270 AFAF0014 */  sw      $t7, 0x0014($sp)           
/* 01D04 80A40274 AFA70010 */  sw      $a3, 0x0010($sp)           
/* 01D08 80A40278 240438FC */  addiu   $a0, $zero, 0x38FC         ## $a0 = 000038FC
/* 01D0C 80A4027C 260500E4 */  addiu   $a1, $s0, 0x00E4           ## $a1 = 000000E4
/* 01D10 80A40280 0C03DCE3 */  jal     Audio_PlaySoundGeneral
              
/* 01D14 80A40284 24060004 */  addiu   $a2, $zero, 0x0004         ## $a2 = 00000004
/* 01D18 80A40288 8618001C */  lh      $t8, 0x001C($s0)           ## 0000001C
/* 01D1C 80A4028C 3C0180A4 */  lui     $at, %hi(D_80A422AC)       ## $at = 80A40000
/* 01D20 80A40290 C42422AC */  lwc1    $f4, %lo(D_80A422AC)($at)  
/* 01D24 80A40294 24010090 */  addiu   $at, $zero, 0x0090         ## $at = 00000090
/* 01D28 80A40298 331900F0 */  andi    $t9, $t8, 0x00F0           ## $t9 = 00000000
/* 01D2C 80A4029C 17210005 */  bne     $t9, $at, .L80A402B4       
/* 01D30 80A402A0 E6040168 */  swc1    $f4, 0x0168($s0)           ## 00000168
/* 01D34 80A402A4 3C013F00 */  lui     $at, 0x3F00                ## $at = 3F000000
/* 01D38 80A402A8 44810000 */  mtc1    $at, $f0                   ## $f0 = 0.50
/* 01D3C 80A402AC 10000005 */  beq     $zero, $zero, .L80A402C4   
/* 01D40 80A402B0 C6060168 */  lwc1    $f6, 0x0168($s0)           ## 00000168
.L80A402B4:
/* 01D44 80A402B4 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 01D48 80A402B8 44810000 */  mtc1    $at, $f0                   ## $f0 = 1.00
/* 01D4C 80A402BC 00000000 */  nop
/* 01D50 80A402C0 C6060168 */  lwc1    $f6, 0x0168($s0)           ## 00000168
.L80A402C4:
/* 01D54 80A402C4 3C0580A4 */  lui     $a1, %hi(func_80A40318)    ## $a1 = 80A40000
/* 01D58 80A402C8 24A50318 */  addiu   $a1, $a1, %lo(func_80A40318) ## $a1 = 80A40318
/* 01D5C 80A402CC 46003202 */  mul.s   $f8, $f6, $f0              
/* 01D60 80A402D0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 01D64 80A402D4 0C28F95C */  jal     func_80A3E570              
/* 01D68 80A402D8 E6080168 */  swc1    $f8, 0x0168($s0)           ## 00000168
/* 01D6C 80A402DC 8608001C */  lh      $t0, 0x001C($s0)           ## 0000001C
/* 01D70 80A402E0 24010090 */  addiu   $at, $zero, 0x0090         ## $at = 00000090
/* 01D74 80A402E4 8FA4002C */  lw      $a0, 0x002C($sp)           
/* 01D78 80A402E8 310900F0 */  andi    $t1, $t0, 0x00F0           ## $t1 = 00000000
/* 01D7C 80A402EC 15210005 */  bne     $t1, $at, .L80A40304       
/* 01D80 80A402F0 24051068 */  addiu   $a1, $zero, 0x1068         ## $a1 = 00001068
/* 01D84 80A402F4 2406FF9D */  addiu   $a2, $zero, 0xFF9D         ## $a2 = FFFFFF9D
/* 01D88 80A402F8 02003825 */  or      $a3, $s0, $zero            ## $a3 = 00000000
/* 01D8C 80A402FC 0C02003E */  jal     func_800800F8              
/* 01D90 80A40300 AFA00010 */  sw      $zero, 0x0010($sp)         
.L80A40304:
/* 01D94 80A40304 8FBF0024 */  lw      $ra, 0x0024($sp)           
.L80A40308:
/* 01D98 80A40308 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 01D9C 80A4030C 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 01DA0 80A40310 03E00008 */  jr      $ra                        
/* 01DA4 80A40314 00000000 */  nop