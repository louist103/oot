.late_rodata

glabel D_8087A7EC
    .float 2552.0

.text
glabel func_80879C04
/* 00164 80879C04 3C0E8088 */  lui     $t6, %hi(D_8087A758)       ## $t6 = 80880000
/* 00168 80879C08 25CEA758 */  addiu   $t6, $t6, %lo(D_8087A758)  ## $t6 = 8087A758
/* 0016C 80879C0C 8DD80000 */  lw      $t8, 0x0000($t6)           ## 8087A758
/* 00170 80879C10 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 00174 80879C14 27A2001C */  addiu   $v0, $sp, 0x001C           ## $v0 = FFFFFFE4
/* 00178 80879C18 AC580000 */  sw      $t8, 0x0000($v0)           ## FFFFFFE4
/* 0017C 80879C1C 8DCF0004 */  lw      $t7, 0x0004($t6)           ## 8087A75C
/* 00180 80879C20 3C198088 */  lui     $t9, %hi(D_8087A770)       ## $t9 = 80880000
/* 00184 80879C24 2739A770 */  addiu   $t9, $t9, %lo(D_8087A770)  ## $t9 = 8087A770
/* 00188 80879C28 AC4F0004 */  sw      $t7, 0x0004($v0)           ## FFFFFFE8
/* 0018C 80879C2C 8DD80008 */  lw      $t8, 0x0008($t6)           ## 8087A760
/* 00190 80879C30 27A30004 */  addiu   $v1, $sp, 0x0004           ## $v1 = FFFFFFCC
/* 00194 80879C34 3C0142F0 */  lui     $at, 0x42F0                ## $at = 42F00000
/* 00198 80879C38 AC580008 */  sw      $t8, 0x0008($v0)           ## FFFFFFEC
/* 0019C 80879C3C 8DCF000C */  lw      $t7, 0x000C($t6)           ## 8087A764
/* 001A0 80879C40 44810000 */  mtc1    $at, $f0                   ## $f0 = 120.00
/* 001A4 80879C44 3C018088 */  lui     $at, %hi(D_8087AC40)       ## $at = 80880000
/* 001A8 80879C48 AC4F000C */  sw      $t7, 0x000C($v0)           ## FFFFFFF0
/* 001AC 80879C4C 8DD80010 */  lw      $t8, 0x0010($t6)           ## 8087A768
/* 001B0 80879C50 00455821 */  addu    $t3, $v0, $a1              
/* 001B4 80879C54 AC580010 */  sw      $t8, 0x0010($v0)           ## FFFFFFF4
/* 001B8 80879C58 95D80014 */  lhu     $t8, 0x0014($t6)           ## 8087A76C
/* 001BC 80879C5C A4580014 */  sh      $t8, 0x0014($v0)           ## FFFFFFF8
/* 001C0 80879C60 8F290000 */  lw      $t1, 0x0000($t9)           ## 8087A770
/* 001C4 80879C64 AC690000 */  sw      $t1, 0x0000($v1)           ## FFFFFFCC
/* 001C8 80879C68 8F280004 */  lw      $t0, 0x0004($t9)           ## 8087A774
/* 001CC 80879C6C AC680004 */  sw      $t0, 0x0004($v1)           ## FFFFFFD0
/* 001D0 80879C70 8F290008 */  lw      $t1, 0x0008($t9)           ## 8087A778
/* 001D4 80879C74 AC690008 */  sw      $t1, 0x0008($v1)           ## FFFFFFD4
/* 001D8 80879C78 8F28000C */  lw      $t0, 0x000C($t9)           ## 8087A77C
/* 001DC 80879C7C AC68000C */  sw      $t0, 0x000C($v1)           ## FFFFFFD8
/* 001E0 80879C80 8F290010 */  lw      $t1, 0x0010($t9)           ## 8087A780
/* 001E4 80879C84 AC690010 */  sw      $t1, 0x0010($v1)           ## FFFFFFDC
/* 001E8 80879C88 97290014 */  lhu     $t1, 0x0014($t9)           ## 8087A784
/* 001EC 80879C8C A4690014 */  sh      $t1, 0x0014($v1)           ## FFFFFFE0
/* 001F0 80879C90 848A001C */  lh      $t2, 0x001C($a0)           ## 0000001C
/* 001F4 80879C94 002A0821 */  addu    $at, $at, $t2              
/* 001F8 80879C98 A025AC40 */  sb      $a1, %lo(D_8087AC40)($at)  
/* 001FC 80879C9C 916C0000 */  lbu     $t4, 0x0000($t3)           ## 00000000
/* 00200 80879CA0 3C014F80 */  lui     $at, 0x4F80                ## $at = 4F800000
/* 00204 80879CA4 448C2000 */  mtc1    $t4, $f4                   ## $f4 = 0.00
/* 00208 80879CA8 05810004 */  bgez    $t4, .L80879CBC            
/* 0020C 80879CAC 468021A0 */  cvt.s.w $f6, $f4                   
/* 00210 80879CB0 44814000 */  mtc1    $at, $f8                   ## $f8 = 4294967296.00
/* 00214 80879CB4 00000000 */  nop
/* 00218 80879CB8 46083180 */  add.s   $f6, $f6, $f8              
.L80879CBC:
/* 0021C 80879CBC 46003282 */  mul.s   $f10, $f6, $f0             
/* 00220 80879CC0 3C018088 */  lui     $at, %hi(D_8087A7EC)       ## $at = 80880000
/* 00224 80879CC4 C430A7EC */  lwc1    $f16, %lo(D_8087A7EC)($at) 
/* 00228 80879CC8 00656821 */  addu    $t5, $v1, $a1              
/* 0022C 80879CCC 3C01C407 */  lui     $at, 0xC407                ## $at = C4070000
/* 00230 80879CD0 44812000 */  mtc1    $at, $f4                   ## $f4 = -540.00
/* 00234 80879CD4 3C014F80 */  lui     $at, 0x4F80                ## $at = 4F800000
/* 00238 80879CD8 46105480 */  add.s   $f18, $f10, $f16           
/* 0023C 80879CDC E4920168 */  swc1    $f18, 0x0168($a0)          ## 00000168
/* 00240 80879CE0 91AE0000 */  lbu     $t6, 0x0000($t5)           ## 00000000
/* 00244 80879CE4 448E4000 */  mtc1    $t6, $f8                   ## $f8 = -0.00
/* 00248 80879CE8 05C10004 */  bgez    $t6, .L80879CFC            
/* 0024C 80879CEC 468041A0 */  cvt.s.w $f6, $f8                   
/* 00250 80879CF0 44815000 */  mtc1    $at, $f10                  ## $f10 = 4294967296.00
/* 00254 80879CF4 00000000 */  nop
/* 00258 80879CF8 460A3180 */  add.s   $f6, $f6, $f10             
.L80879CFC:
/* 0025C 80879CFC 46003402 */  mul.s   $f16, $f6, $f0             
/* 00260 80879D00 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000
/* 00264 80879D04 46102481 */  sub.s   $f18, $f4, $f16            
/* 00268 80879D08 03E00008 */  jr      $ra                        
/* 0026C 80879D0C E4920170 */  swc1    $f18, 0x0170($a0)          ## 00000170
