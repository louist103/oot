.rdata
glabel D_8096D090
    .asciz "../z_demo_du.c"
    .balign 4

glabel D_8096D0A0
    .asciz "../z_demo_du.c"
    .balign 4

.text
glabel func_8096BF60
/* 02490 8096BF60 27BDFF88 */  addiu   $sp, $sp, 0xFF88           ## $sp = FFFFFF88
/* 02494 8096BF64 AFBF0024 */  sw      $ra, 0x0024($sp)
/* 02498 8096BF68 AFA40078 */  sw      $a0, 0x0078($sp)
/* 0249C 8096BF6C AFA5007C */  sw      $a1, 0x007C($sp)
/* 024A0 8096BF70 84820190 */  lh      $v0, 0x0190($a0)           ## 00000190
/* 024A4 8096BF74 3C188097 */  lui     $t8, %hi(D_8096CE74)       ## $t8 = 80970000
/* 024A8 8096BF78 3C098097 */  lui     $t1, %hi(D_8096CE84)       ## $t1 = 80970000
/* 024AC 8096BF7C 00027880 */  sll     $t7, $v0,  2
/* 024B0 8096BF80 030FC021 */  addu    $t8, $t8, $t7
/* 024B4 8096BF84 8F18CE74 */  lw      $t8, %lo(D_8096CE74)($t8)
/* 024B8 8096BF88 3C068097 */  lui     $a2, %hi(D_8096D090)       ## $a2 = 80970000
/* 024BC 8096BF8C 24C6D090 */  addiu   $a2, $a2, %lo(D_8096D090)  ## $a2 = 8096D090
/* 024C0 8096BF90 AFB80068 */  sw      $t8, 0x0068($sp)
/* 024C4 8096BF94 84830194 */  lh      $v1, 0x0194($a0)           ## 00000194
/* 024C8 8096BF98 27A40044 */  addiu   $a0, $sp, 0x0044           ## $a0 = FFFFFFCC
/* 024CC 8096BF9C 24070267 */  addiu   $a3, $zero, 0x0267         ## $a3 = 00000267
/* 024D0 8096BFA0 0003C880 */  sll     $t9, $v1,  2
/* 024D4 8096BFA4 01394821 */  addu    $t1, $t1, $t9
/* 024D8 8096BFA8 8D29CE84 */  lw      $t1, %lo(D_8096CE84)($t1)
/* 024DC 8096BFAC AFA9005C */  sw      $t1, 0x005C($sp)
/* 024E0 8096BFB0 8CA50000 */  lw      $a1, 0x0000($a1)           ## 00000000
/* 024E4 8096BFB4 0C031AB1 */  jal     Graph_OpenDisps
/* 024E8 8096BFB8 AFA50054 */  sw      $a1, 0x0054($sp)
/* 024EC 8096BFBC 8FAB007C */  lw      $t3, 0x007C($sp)
/* 024F0 8096BFC0 0C024F46 */  jal     func_80093D18
/* 024F4 8096BFC4 8D640000 */  lw      $a0, 0x0000($t3)           ## 00000000
/* 024F8 8096BFC8 8FA80054 */  lw      $t0, 0x0054($sp)
/* 024FC 8096BFCC 8FA40068 */  lw      $a0, 0x0068($sp)
/* 02500 8096BFD0 3C058016 */  lui     $a1, %hi(gSegments)
/* 02504 8096BFD4 8D0202C0 */  lw      $v0, 0x02C0($t0)           ## 000002C0
/* 02508 8096BFD8 00047900 */  sll     $t7, $a0,  4
/* 0250C 8096BFDC 000FC702 */  srl     $t8, $t7, 28
/* 02510 8096BFE0 3C0DDB06 */  lui     $t5, 0xDB06                ## $t5 = DB060000
/* 02514 8096BFE4 244C0008 */  addiu   $t4, $v0, 0x0008           ## $t4 = 00000008
/* 02518 8096BFE8 35AD0020 */  ori     $t5, $t5, 0x0020           ## $t5 = DB060020
/* 0251C 8096BFEC 00187080 */  sll     $t6, $t8,  2
/* 02520 8096BFF0 24A56FA8 */  addiu   $a1, %lo(gSegments)
/* 02524 8096BFF4 AD0C02C0 */  sw      $t4, 0x02C0($t0)           ## 000002C0
/* 02528 8096BFF8 00AEC821 */  addu    $t9, $a1, $t6
/* 0252C 8096BFFC AC4D0000 */  sw      $t5, 0x0000($v0)           ## 00000000
/* 02530 8096C000 8F290000 */  lw      $t1, 0x0000($t9)           ## 00000000
/* 02534 8096C004 3C0600FF */  lui     $a2, 0x00FF                ## $a2 = 00FF0000
/* 02538 8096C008 34C6FFFF */  ori     $a2, $a2, 0xFFFF           ## $a2 = 00FFFFFF
/* 0253C 8096C00C 00865024 */  and     $t2, $a0, $a2
/* 02540 8096C010 3C078000 */  lui     $a3, 0x8000                ## $a3 = 80000000
/* 02544 8096C014 012A5821 */  addu    $t3, $t1, $t2
/* 02548 8096C018 01676021 */  addu    $t4, $t3, $a3
/* 0254C 8096C01C AC4C0004 */  sw      $t4, 0x0004($v0)           ## 00000004
/* 02550 8096C020 8FA4005C */  lw      $a0, 0x005C($sp)
/* 02554 8096C024 8D0202C0 */  lw      $v0, 0x02C0($t0)           ## 000002C0
/* 02558 8096C028 3C0FDB06 */  lui     $t7, 0xDB06                ## $t7 = DB060000
/* 0255C 8096C02C 0004C100 */  sll     $t8, $a0,  4
/* 02560 8096C030 00187702 */  srl     $t6, $t8, 28
/* 02564 8096C034 244D0008 */  addiu   $t5, $v0, 0x0008           ## $t5 = 00000008
/* 02568 8096C038 000EC880 */  sll     $t9, $t6,  2
/* 0256C 8096C03C 35EF0024 */  ori     $t7, $t7, 0x0024           ## $t7 = DB060024
/* 02570 8096C040 AD0D02C0 */  sw      $t5, 0x02C0($t0)           ## 000002C0
/* 02574 8096C044 00B94821 */  addu    $t1, $a1, $t9
/* 02578 8096C048 AC4F0000 */  sw      $t7, 0x0000($v0)           ## 00000000
/* 0257C 8096C04C 8D2A0000 */  lw      $t2, 0x0000($t1)           ## 00000000
/* 02580 8096C050 00865824 */  and     $t3, $a0, $a2
/* 02584 8096C054 3C040600 */  lui     $a0, 0x0600                ## $a0 = 06000000
/* 02588 8096C058 014B6021 */  addu    $t4, $t2, $t3
/* 0258C 8096C05C 01876821 */  addu    $t5, $t4, $a3
/* 02590 8096C060 AC4D0004 */  sw      $t5, 0x0004($v0)           ## 00000004
/* 02594 8096C064 8D0202C0 */  lw      $v0, 0x02C0($t0)           ## 000002C0
/* 02598 8096C068 24847FC0 */  addiu   $a0, $a0, 0x7FC0           ## $a0 = 06007FC0
/* 0259C 8096C06C 00047100 */  sll     $t6, $a0,  4
/* 025A0 8096C070 000ECF02 */  srl     $t9, $t6, 28
/* 025A4 8096C074 3C18DB06 */  lui     $t8, 0xDB06                ## $t8 = DB060000
/* 025A8 8096C078 244F0008 */  addiu   $t7, $v0, 0x0008           ## $t7 = 00000008
/* 025AC 8096C07C 37180028 */  ori     $t8, $t8, 0x0028           ## $t8 = DB060028
/* 025B0 8096C080 00194880 */  sll     $t1, $t9,  2
/* 025B4 8096C084 AD0F02C0 */  sw      $t7, 0x02C0($t0)           ## 000002C0
/* 025B8 8096C088 00A95021 */  addu    $t2, $a1, $t1
/* 025BC 8096C08C AC580000 */  sw      $t8, 0x0000($v0)           ## 00000000
/* 025C0 8096C090 8D4B0000 */  lw      $t3, 0x0000($t2)           ## 00000000
/* 025C4 8096C094 00866024 */  and     $t4, $a0, $a2
/* 025C8 8096C098 241900FF */  addiu   $t9, $zero, 0x00FF         ## $t9 = 000000FF
/* 025CC 8096C09C 016C6821 */  addu    $t5, $t3, $t4
/* 025D0 8096C0A0 01A77821 */  addu    $t7, $t5, $a3
/* 025D4 8096C0A4 AC4F0004 */  sw      $t7, 0x0004($v0)           ## 00000004
/* 025D8 8096C0A8 8D0202C0 */  lw      $v0, 0x02C0($t0)           ## 000002C0
/* 025DC 8096C0AC 3C0EFB00 */  lui     $t6, 0xFB00                ## $t6 = FB000000
/* 025E0 8096C0B0 3C0B8011 */  lui     $t3, %hi(D_80116280+0x10)
/* 025E4 8096C0B4 24580008 */  addiu   $t8, $v0, 0x0008           ## $t8 = 00000008
/* 025E8 8096C0B8 AD1802C0 */  sw      $t8, 0x02C0($t0)           ## 000002C0
/* 025EC 8096C0BC AC590004 */  sw      $t9, 0x0004($v0)           ## 00000004
/* 025F0 8096C0C0 AC4E0000 */  sw      $t6, 0x0000($v0)           ## 00000000
/* 025F4 8096C0C4 8D0202C0 */  lw      $v0, 0x02C0($t0)           ## 000002C0
/* 025F8 8096C0C8 3C0ADB06 */  lui     $t2, 0xDB06                ## $t2 = DB060000
/* 025FC 8096C0CC 354A0030 */  ori     $t2, $t2, 0x0030           ## $t2 = DB060030
/* 02600 8096C0D0 24490008 */  addiu   $t1, $v0, 0x0008           ## $t1 = 00000008
/* 02604 8096C0D4 AD0902C0 */  sw      $t1, 0x02C0($t0)           ## 000002C0
/* 02608 8096C0D8 256B6290 */  addiu   $t3, %lo(D_80116280+0x10)
/* 0260C 8096C0DC AC4B0004 */  sw      $t3, 0x0004($v0)           ## 00000004
/* 02610 8096C0E0 AC4A0000 */  sw      $t2, 0x0000($v0)           ## 00000000
/* 02614 8096C0E4 8FA30078 */  lw      $v1, 0x0078($sp)
/* 02618 8096C0E8 8FA4007C */  lw      $a0, 0x007C($sp)
/* 0261C 8096C0EC 2462014C */  addiu   $v0, $v1, 0x014C           ## $v0 = 0000014C
/* 02620 8096C0F0 8C450004 */  lw      $a1, 0x0004($v0)           ## 00000150
/* 02624 8096C0F4 8C460020 */  lw      $a2, 0x0020($v0)           ## 0000016C
/* 02628 8096C0F8 90470002 */  lbu     $a3, 0x0002($v0)           ## 0000014E
/* 0262C 8096C0FC AFA00014 */  sw      $zero, 0x0014($sp)
/* 02630 8096C100 AFA00010 */  sw      $zero, 0x0010($sp)
/* 02634 8096C104 0C0286B2 */  jal     SkelAnime_DrawSV
/* 02638 8096C108 AFA30018 */  sw      $v1, 0x0018($sp)
/* 0263C 8096C10C 8FAC007C */  lw      $t4, 0x007C($sp)
/* 02640 8096C110 3C068097 */  lui     $a2, %hi(D_8096D0A0)       ## $a2 = 80970000
/* 02644 8096C114 24C6D0A0 */  addiu   $a2, $a2, %lo(D_8096D0A0)  ## $a2 = 8096D0A0
/* 02648 8096C118 27A40044 */  addiu   $a0, $sp, 0x0044           ## $a0 = FFFFFFCC
/* 0264C 8096C11C 2407027E */  addiu   $a3, $zero, 0x027E         ## $a3 = 0000027E
/* 02650 8096C120 0C031AD5 */  jal     Graph_CloseDisps
/* 02654 8096C124 8D850000 */  lw      $a1, 0x0000($t4)           ## 00000000
/* 02658 8096C128 8FBF0024 */  lw      $ra, 0x0024($sp)
/* 0265C 8096C12C 27BD0078 */  addiu   $sp, $sp, 0x0078           ## $sp = 00000000
/* 02660 8096C130 03E00008 */  jr      $ra
/* 02664 8096C134 00000000 */  nop
