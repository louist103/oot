.rdata
glabel D_8090DA5C
    .asciz "../z_boss_ganon2.c"
    .balign 4

glabel D_8090DA70
    .asciz "../z_boss_ganon2.c"
    .balign 4

glabel D_8090DA84
    .asciz "../z_boss_ganon2.c"
    .balign 4

.late_rodata
glabel D_8090DD98
 .word 0x3FD33333, 0x00000000

.text
glabel func_80906AB0
/* 09B70 80906AB0 27BDFF88 */  addiu   $sp, $sp, 0xFF88           ## $sp = FFFFFF88
/* 09B74 80906AB4 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 09B78 80906AB8 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 09B7C 80906ABC AFA40078 */  sw      $a0, 0x0078($sp)           
/* 09B80 80906AC0 AFA5007C */  sw      $a1, 0x007C($sp)           
/* 09B84 80906AC4 AFA60080 */  sw      $a2, 0x0080($sp)           
/* 09B88 80906AC8 8CD00000 */  lw      $s0, 0x0000($a2)           ## 00000000
/* 09B8C 80906ACC 3C068091 */  lui     $a2, %hi(D_8090DA5C)       ## $a2 = 80910000
/* 09B90 80906AD0 24C6DA5C */  addiu   $a2, $a2, %lo(D_8090DA5C)  ## $a2 = 8090DA5C
/* 09B94 80906AD4 27A40058 */  addiu   $a0, $sp, 0x0058           ## $a0 = FFFFFFE0
/* 09B98 80906AD8 2407191E */  addiu   $a3, $zero, 0x191E         ## $a3 = 0000191E
/* 09B9C 80906ADC 0C031AB1 */  jal     Graph_OpenDisps              
/* 09BA0 80906AE0 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 09BA4 80906AE4 8FAF0080 */  lw      $t7, 0x0080($sp)           
/* 09BA8 80906AE8 0C024F46 */  jal     func_80093D18              
/* 09BAC 80906AEC 8DE40000 */  lw      $a0, 0x0000($t7)           ## 00000000
/* 09BB0 80906AF0 8FA20080 */  lw      $v0, 0x0080($sp)           
/* 09BB4 80906AF4 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 09BB8 80906AF8 00411021 */  addu    $v0, $v0, $at              
/* 09BBC 80906AFC 90430AE1 */  lbu     $v1, 0x0AE1($v0)           ## 00000AE1
/* 09BC0 80906B00 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 09BC4 80906B04 10610003 */  beq     $v1, $at, .L80906B14       
/* 09BC8 80906B08 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 09BCC 80906B0C 1461000E */  bne     $v1, $at, .L80906B48       
/* 09BD0 80906B10 24040078 */  addiu   $a0, $zero, 0x0078         ## $a0 = 00000078
.L80906B14:
/* 09BD4 80906B14 3C014334 */  lui     $at, 0x4334                ## $at = 43340000
/* 09BD8 80906B18 44813000 */  mtc1    $at, $f6                   ## $f6 = 180.00
/* 09BDC 80906B1C C4440AFC */  lwc1    $f4, 0x0AFC($v0)           ## 00000AFC
/* 09BE0 80906B20 46062202 */  mul.s   $f8, $f4, $f6              
/* 09BE4 80906B24 4600428D */  trunc.w.s $f10, $f8                  
/* 09BE8 80906B28 44045000 */  mfc1    $a0, $f10                  
/* 09BEC 80906B2C 00000000 */  nop
/* 09BF0 80906B30 00042400 */  sll     $a0, $a0, 16               
/* 09BF4 80906B34 00042403 */  sra     $a0, $a0, 16               
/* 09BF8 80906B38 2484001E */  addiu   $a0, $a0, 0x001E           ## $a0 = 00000096
/* 09BFC 80906B3C 00042400 */  sll     $a0, $a0, 16               
/* 09C00 80906B40 10000001 */  beq     $zero, $zero, .L80906B48   
/* 09C04 80906B44 00042403 */  sra     $a0, $a0, 16               
.L80906B48:
/* 09C08 80906B48 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09C0C 80906B4C 3C08FA00 */  lui     $t0, 0xFA00                ## $t0 = FA000000
/* 09C10 80906B50 308900FF */  andi    $t1, $a0, 0x00FF           ## $t1 = 00000096
/* 09C14 80906B54 24590008 */  addiu   $t9, $v0, 0x0008           ## $t9 = 00000008
/* 09C18 80906B58 AE1902C0 */  sw      $t9, 0x02C0($s0)           ## 000002C0
/* 09C1C 80906B5C AC490004 */  sw      $t1, 0x0004($v0)           ## 00000004
/* 09C20 80906B60 AC480000 */  sw      $t0, 0x0000($v0)           ## 00000000
/* 09C24 80906B64 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09C28 80906B68 3C0BFB00 */  lui     $t3, 0xFB00                ## $t3 = FB000000
/* 09C2C 80906B6C 3C0141A0 */  lui     $at, 0x41A0                ## $at = 41A00000
/* 09C30 80906B70 244A0008 */  addiu   $t2, $v0, 0x0008           ## $t2 = 00000008
/* 09C34 80906B74 AE0A02C0 */  sw      $t2, 0x02C0($s0)           ## 000002C0
/* 09C38 80906B78 AC400004 */  sw      $zero, 0x0004($v0)         ## 00000004
/* 09C3C 80906B7C AC4B0000 */  sw      $t3, 0x0000($v0)           ## 00000000
/* 09C40 80906B80 8FA2007C */  lw      $v0, 0x007C($sp)           
/* 09C44 80906B84 44819000 */  mtc1    $at, $f18                  ## $f18 = 20.00
/* 09C48 80906B88 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 09C4C 80906B8C C450002C */  lwc1    $f16, 0x002C($v0)          ## 0000002C
/* 09C50 80906B90 C44C0024 */  lwc1    $f12, 0x0024($v0)          ## 00000024
/* 09C54 80906B94 C44E0080 */  lwc1    $f14, 0x0080($v0)          ## 00000080
/* 09C58 80906B98 46128101 */  sub.s   $f4, $f16, $f18            
/* 09C5C 80906B9C 44062000 */  mfc1    $a2, $f4                   
/* 09C60 80906BA0 0C034261 */  jal     Matrix_Translate              
/* 09C64 80906BA4 00000000 */  nop
/* 09C68 80906BA8 3C018091 */  lui     $at, %hi(D_8090DD98)       ## $at = 80910000
/* 09C6C 80906BAC C42CDD98 */  lwc1    $f12, %lo(D_8090DD98)($at) 
/* 09C70 80906BB0 3C013F80 */  lui     $at, 0x3F80                ## $at = 3F800000
/* 09C74 80906BB4 44817000 */  mtc1    $at, $f14                  ## $f14 = 1.00
/* 09C78 80906BB8 44066000 */  mfc1    $a2, $f12                  
/* 09C7C 80906BBC 0C0342A3 */  jal     Matrix_Scale              
/* 09C80 80906BC0 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 09C84 80906BC4 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09C88 80906BC8 3C0DDA38 */  lui     $t5, 0xDA38                ## $t5 = DA380000
/* 09C8C 80906BCC 35AD0003 */  ori     $t5, $t5, 0x0003           ## $t5 = DA380003
/* 09C90 80906BD0 244C0008 */  addiu   $t4, $v0, 0x0008           ## $t4 = 00000008
/* 09C94 80906BD4 AE0C02C0 */  sw      $t4, 0x02C0($s0)           ## 000002C0
/* 09C98 80906BD8 AC4D0000 */  sw      $t5, 0x0000($v0)           ## 00000000
/* 09C9C 80906BDC 8FAE0080 */  lw      $t6, 0x0080($sp)           
/* 09CA0 80906BE0 3C058091 */  lui     $a1, %hi(D_8090DA70)       ## $a1 = 80910000
/* 09CA4 80906BE4 24A5DA70 */  addiu   $a1, $a1, %lo(D_8090DA70)  ## $a1 = 8090DA70
/* 09CA8 80906BE8 8DC40000 */  lw      $a0, 0x0000($t6)           ## 00000000
/* 09CAC 80906BEC 24061939 */  addiu   $a2, $zero, 0x1939         ## $a2 = 00001939
/* 09CB0 80906BF0 0C0346A2 */  jal     Matrix_NewMtx              
/* 09CB4 80906BF4 AFA2004C */  sw      $v0, 0x004C($sp)           
/* 09CB8 80906BF8 8FA3004C */  lw      $v1, 0x004C($sp)           
/* 09CBC 80906BFC 3C188091 */  lui     $t8, %hi(D_80908310)       ## $t8 = 80910000
/* 09CC0 80906C00 27188310 */  addiu   $t8, $t8, %lo(D_80908310)  ## $t8 = 80908310
/* 09CC4 80906C04 AC620004 */  sw      $v0, 0x0004($v1)           ## 00000004
/* 09CC8 80906C08 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09CCC 80906C0C 3C04DE00 */  lui     $a0, 0xDE00                ## $a0 = DE000000
/* 09CD0 80906C10 3C08FD90 */  lui     $t0, 0xFD90                ## $t0 = FD900000
/* 09CD4 80906C14 244F0008 */  addiu   $t7, $v0, 0x0008           ## $t7 = 00000008
/* 09CD8 80906C18 AE0F02C0 */  sw      $t7, 0x02C0($s0)           ## 000002C0
/* 09CDC 80906C1C AC580004 */  sw      $t8, 0x0004($v0)           ## 00000004
/* 09CE0 80906C20 AC440000 */  sw      $a0, 0x0000($v0)           ## 00000000
/* 09CE4 80906C24 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09CE8 80906C28 3C0C0709 */  lui     $t4, 0x0709                ## $t4 = 07090000
/* 09CEC 80906C2C 358C8260 */  ori     $t4, $t4, 0x8260           ## $t4 = 07098260
/* 09CF0 80906C30 24590008 */  addiu   $t9, $v0, 0x0008           ## $t9 = 00000008
/* 09CF4 80906C34 AE1902C0 */  sw      $t9, 0x02C0($s0)           ## 000002C0
/* 09CF8 80906C38 AC480000 */  sw      $t0, 0x0000($v0)           ## 00000000
/* 09CFC 80906C3C 8FA90078 */  lw      $t1, 0x0078($sp)           
/* 09D00 80906C40 3C0BF590 */  lui     $t3, 0xF590                ## $t3 = F5900000
/* 09D04 80906C44 3C0EE600 */  lui     $t6, 0xE600                ## $t6 = E6000000
/* 09D08 80906C48 AC490004 */  sw      $t1, 0x0004($v0)           ## 00000004
/* 09D0C 80906C4C 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09D10 80906C50 3C19077F */  lui     $t9, 0x077F                ## $t9 = 077F0000
/* 09D14 80906C54 3739F100 */  ori     $t9, $t9, 0xF100           ## $t9 = 077FF100
/* 09D18 80906C58 244A0008 */  addiu   $t2, $v0, 0x0008           ## $t2 = 00000008
/* 09D1C 80906C5C AE0A02C0 */  sw      $t2, 0x02C0($s0)           ## 000002C0
/* 09D20 80906C60 AC4C0004 */  sw      $t4, 0x0004($v0)           ## 00000004
/* 09D24 80906C64 AC4B0000 */  sw      $t3, 0x0000($v0)           ## 00000000
/* 09D28 80906C68 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09D2C 80906C6C 3C18F300 */  lui     $t8, 0xF300                ## $t8 = F3000000
/* 09D30 80906C70 3C09E700 */  lui     $t1, 0xE700                ## $t1 = E7000000
/* 09D34 80906C74 244D0008 */  addiu   $t5, $v0, 0x0008           ## $t5 = 00000008
/* 09D38 80906C78 AE0D02C0 */  sw      $t5, 0x02C0($s0)           ## 000002C0
/* 09D3C 80906C7C AC400004 */  sw      $zero, 0x0004($v0)         ## 00000004
/* 09D40 80906C80 AC4E0000 */  sw      $t6, 0x0000($v0)           ## 00000000
/* 09D44 80906C84 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09D48 80906C88 3C0BF588 */  lui     $t3, 0xF588                ## $t3 = F5880000
/* 09D4C 80906C8C 3C0C0009 */  lui     $t4, 0x0009                ## $t4 = 00090000
/* 09D50 80906C90 244F0008 */  addiu   $t7, $v0, 0x0008           ## $t7 = 00000008
/* 09D54 80906C94 AE0F02C0 */  sw      $t7, 0x02C0($s0)           ## 000002C0
/* 09D58 80906C98 AC590004 */  sw      $t9, 0x0004($v0)           ## 00000004
/* 09D5C 80906C9C AC580000 */  sw      $t8, 0x0000($v0)           ## 00000000
/* 09D60 80906CA0 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09D64 80906CA4 358C8260 */  ori     $t4, $t4, 0x8260           ## $t4 = 00098260
/* 09D68 80906CA8 356B1000 */  ori     $t3, $t3, 0x1000           ## $t3 = F5881000
/* 09D6C 80906CAC 24480008 */  addiu   $t0, $v0, 0x0008           ## $t0 = 00000008
/* 09D70 80906CB0 AE0802C0 */  sw      $t0, 0x02C0($s0)           ## 000002C0
/* 09D74 80906CB4 AC400004 */  sw      $zero, 0x0004($v0)         ## 00000004
/* 09D78 80906CB8 AC490000 */  sw      $t1, 0x0000($v0)           ## 00000000
/* 09D7C 80906CBC 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09D80 80906CC0 3C0F000F */  lui     $t7, 0x000F                ## $t7 = 000F0000
/* 09D84 80906CC4 35EFC0FC */  ori     $t7, $t7, 0xC0FC           ## $t7 = 000FC0FC
/* 09D88 80906CC8 244A0008 */  addiu   $t2, $v0, 0x0008           ## $t2 = 00000008
/* 09D8C 80906CCC AE0A02C0 */  sw      $t2, 0x02C0($s0)           ## 000002C0
/* 09D90 80906CD0 AC4C0004 */  sw      $t4, 0x0004($v0)           ## 00000004
/* 09D94 80906CD4 AC4B0000 */  sw      $t3, 0x0000($v0)           ## 00000000
/* 09D98 80906CD8 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09D9C 80906CDC 3C0EF200 */  lui     $t6, 0xF200                ## $t6 = F2000000
/* 09DA0 80906CE0 3C198091 */  lui     $t9, %hi(D_80908330)       ## $t9 = 80910000
/* 09DA4 80906CE4 244D0008 */  addiu   $t5, $v0, 0x0008           ## $t5 = 00000008
/* 09DA8 80906CE8 AE0D02C0 */  sw      $t5, 0x02C0($s0)           ## 000002C0
/* 09DAC 80906CEC AC4F0004 */  sw      $t7, 0x0004($v0)           ## 00000004
/* 09DB0 80906CF0 AC4E0000 */  sw      $t6, 0x0000($v0)           ## 00000000
/* 09DB4 80906CF4 8E0202C0 */  lw      $v0, 0x02C0($s0)           ## 000002C0
/* 09DB8 80906CF8 27398330 */  addiu   $t9, $t9, %lo(D_80908330)  ## $t9 = 80908330
/* 09DBC 80906CFC 3C068091 */  lui     $a2, %hi(D_8090DA84)       ## $a2 = 80910000
/* 09DC0 80906D00 24580008 */  addiu   $t8, $v0, 0x0008           ## $t8 = 00000008
/* 09DC4 80906D04 AE1802C0 */  sw      $t8, 0x02C0($s0)           ## 000002C0
/* 09DC8 80906D08 AC440000 */  sw      $a0, 0x0000($v0)           ## 00000000
/* 09DCC 80906D0C 27A40058 */  addiu   $a0, $sp, 0x0058           ## $a0 = FFFFFFE0
/* 09DD0 80906D10 24C6DA84 */  addiu   $a2, $a2, %lo(D_8090DA84)  ## $a2 = 8090DA84
/* 09DD4 80906D14 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 09DD8 80906D18 2407194F */  addiu   $a3, $zero, 0x194F         ## $a3 = 0000194F
/* 09DDC 80906D1C 0C031AD5 */  jal     Graph_CloseDisps              
/* 09DE0 80906D20 AC590004 */  sw      $t9, 0x0004($v0)           ## 00000004
/* 09DE4 80906D24 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 09DE8 80906D28 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 09DEC 80906D2C 27BD0078 */  addiu   $sp, $sp, 0x0078           ## $sp = 00000000
/* 09DF0 80906D30 03E00008 */  jr      $ra                        
/* 09DF4 80906D34 00000000 */  nop
/* 09DF8 80906D38 00000000 */  nop
/* 09DFC 80906D3C 00000000 */  nop
