.late_rodata
glabel D_809D1690
    .float 0.7

glabel D_809D1694
 .word 0x3C1374BC

.text
glabel func_809D0424
/* 01D44 809D0424 27BDFFC0 */  addiu   $sp, $sp, 0xFFC0           ## $sp = FFFFFFC0
/* 01D48 809D0428 AFBF0034 */  sw      $ra, 0x0034($sp)           
/* 01D4C 809D042C AFB00030 */  sw      $s0, 0x0030($sp)           
/* 01D50 809D0430 AFA50044 */  sw      $a1, 0x0044($sp)           
/* 01D54 809D0434 908E0114 */  lbu     $t6, 0x0114($a0)           ## 00000114
/* 01D58 809D0438 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 01D5C 809D043C 55C0004D */  bnel    $t6, $zero, .L809D0574     
/* 01D60 809D0440 8FBF0034 */  lw      $ra, 0x0034($sp)           
/* 01D64 809D0444 908F00AF */  lbu     $t7, 0x00AF($a0)           ## 000000AF
/* 01D68 809D0448 A080023C */  sb      $zero, 0x023C($a0)         ## 0000023C
/* 01D6C 809D044C 260502E0 */  addiu   $a1, $s0, 0x02E0           ## $a1 = 000002E0
/* 01D70 809D0450 11E00025 */  beq     $t7, $zero, .L809D04E8     
/* 01D74 809D0454 00000000 */  nop
/* 01D78 809D0458 90820220 */  lbu     $v0, 0x0220($a0)           ## 00000220
/* 01D7C 809D045C 24030005 */  addiu   $v1, $zero, 0x0005         ## $v1 = 00000005
/* 01D80 809D0460 24010006 */  addiu   $at, $zero, 0x0006         ## $at = 00000006
/* 01D84 809D0464 10620015 */  beq     $v1, $v0, .L809D04BC       
/* 01D88 809D0468 00000000 */  nop
/* 01D8C 809D046C 10410013 */  beq     $v0, $at, .L809D04BC       
/* 01D90 809D0470 00000000 */  nop
/* 01D94 809D0474 0C273A6A */  jal     func_809CE9A8              
/* 01D98 809D0478 00000000 */  nop
/* 01D9C 809D047C 241900C8 */  addiu   $t9, $zero, 0x00C8         ## $t9 = 000000C8
/* 01DA0 809D0480 860B0236 */  lh      $t3, 0x0236($s0)           ## 00000236
/* 01DA4 809D0484 240800FF */  addiu   $t0, $zero, 0x00FF         ## $t0 = 000000FF
/* 01DA8 809D0488 24090258 */  addiu   $t1, $zero, 0x0258         ## $t1 = 00000258
/* 01DAC 809D048C 240A0001 */  addiu   $t2, $zero, 0x0001         ## $t2 = 00000001
/* 01DB0 809D0490 A219022D */  sb      $t9, 0x022D($s0)           ## 0000022D
/* 01DB4 809D0494 A219022C */  sb      $t9, 0x022C($s0)           ## 0000022C
/* 01DB8 809D0498 A208022E */  sb      $t0, 0x022E($s0)           ## 0000022E
/* 01DBC 809D049C A6090224 */  sh      $t1, 0x0224($s0)           ## 00000224
/* 01DC0 809D04A0 A20A0221 */  sb      $t2, 0x0221($s0)           ## 00000221
/* 01DC4 809D04A4 3C01809D */  lui     $at, %hi(D_809D1690)       ## $at = 809D0000
/* 01DC8 809D04A8 C4241690 */  lwc1    $f4, %lo(D_809D1690)($at)  
/* 01DCC 809D04AC 256C0001 */  addiu   $t4, $t3, 0x0001           ## $t4 = 00000001
/* 01DD0 809D04B0 A60C0236 */  sh      $t4, 0x0236($s0)           ## 00000236
/* 01DD4 809D04B4 1000002E */  beq     $zero, $zero, .L809D0570   
/* 01DD8 809D04B8 E6040250 */  swc1    $f4, 0x0250($s0)           ## 00000250
.L809D04BC:
/* 01DDC 809D04BC 10620006 */  beq     $v1, $v0, .L809D04D8       
/* 01DE0 809D04C0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 01DE4 809D04C4 3C05809D */  lui     $a1, %hi(func_809CFF98)    ## $a1 = 809D0000
/* 01DE8 809D04C8 0C2739B8 */  jal     func_809CE6E0              
/* 01DEC 809D04CC 24A5FF98 */  addiu   $a1, $a1, %lo(func_809CFF98) ## $a1 = 809CFF98
/* 01DF0 809D04D0 10000028 */  beq     $zero, $zero, .L809D0574   
/* 01DF4 809D04D4 8FBF0034 */  lw      $ra, 0x0034($sp)           
.L809D04D8:
/* 01DF8 809D04D8 0C273FC4 */  jal     func_809CFF10              
/* 01DFC 809D04DC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 01E00 809D04E0 10000024 */  beq     $zero, $zero, .L809D0574   
/* 01E04 809D04E4 8FBF0034 */  lw      $ra, 0x0034($sp)           
.L809D04E8:
/* 01E08 809D04E8 0C00D579 */  jal     func_800355E4              
/* 01E0C 809D04EC 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 01E10 809D04F0 10400017 */  beq     $v0, $zero, .L809D0550     
/* 01E14 809D04F4 240E0001 */  addiu   $t6, $zero, 0x0001         ## $t6 = 00000001
/* 01E18 809D04F8 A2000230 */  sb      $zero, 0x0230($s0)         ## 00000230
/* 01E1C 809D04FC 3C01809D */  lui     $at, %hi(D_809D1694)       ## $at = 809D0000
/* 01E20 809D0500 C4281694 */  lwc1    $f8, %lo(D_809D1694)($at)  
/* 01E24 809D0504 C6060054 */  lwc1    $f6, 0x0054($s0)           ## 00000054
/* 01E28 809D0508 3C014080 */  lui     $at, 0x4080                ## $at = 40800000
/* 01E2C 809D050C 44818000 */  mtc1    $at, $f16                  ## $f16 = 4.00
/* 01E30 809D0510 46083281 */  sub.s   $f10, $f6, $f8             
/* 01E34 809D0514 26060024 */  addiu   $a2, $s0, 0x0024           ## $a2 = 00000024
/* 01E38 809D0518 240D000B */  addiu   $t5, $zero, 0x000B         ## $t5 = 0000000B
/* 01E3C 809D051C 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 01E40 809D0520 E60A0054 */  swc1    $f10, 0x0054($s0)          ## 00000054
/* 01E44 809D0524 AFA6003C */  sw      $a2, 0x003C($sp)           
/* 01E48 809D0528 AFA00020 */  sw      $zero, 0x0020($sp)         
/* 01E4C 809D052C AFA0001C */  sw      $zero, 0x001C($sp)         
/* 01E50 809D0530 AFA00018 */  sw      $zero, 0x0018($sp)         
/* 01E54 809D0534 AFAD0010 */  sw      $t5, 0x0010($sp)           
/* 01E58 809D0538 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 01E5C 809D053C 3C0741F0 */  lui     $a3, 0x41F0                ## $a3 = 41F00000
/* 01E60 809D0540 0C00CC98 */  jal     func_80033260              
/* 01E64 809D0544 E7B00014 */  swc1    $f16, 0x0014($sp)          
/* 01E68 809D0548 10000003 */  beq     $zero, $zero, .L809D0558   
/* 01E6C 809D054C 8FA6003C */  lw      $a2, 0x003C($sp)           
.L809D0550:
/* 01E70 809D0550 A20E0230 */  sb      $t6, 0x0230($s0)           ## 00000230
/* 01E74 809D0554 26060024 */  addiu   $a2, $s0, 0x0024           ## $a2 = 00000024
.L809D0558:
/* 01E78 809D0558 8FA40044 */  lw      $a0, 0x0044($sp)           
/* 01E7C 809D055C 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 01E80 809D0560 0C007E50 */  jal     Item_DropCollectibleRandom
              
/* 01E84 809D0564 24070090 */  addiu   $a3, $zero, 0x0090         ## $a3 = 00000090
/* 01E88 809D0568 0C27403D */  jal     func_809D00F4              
/* 01E8C 809D056C 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
.L809D0570:
/* 01E90 809D0570 8FBF0034 */  lw      $ra, 0x0034($sp)           
.L809D0574:
/* 01E94 809D0574 8FB00030 */  lw      $s0, 0x0030($sp)           
/* 01E98 809D0578 27BD0040 */  addiu   $sp, $sp, 0x0040           ## $sp = 00000000
/* 01E9C 809D057C 03E00008 */  jr      $ra                        
/* 01EA0 809D0580 00000000 */  nop