.late_rodata
glabel D_80855544
    .float 0.3

glabel D_80855548
    .float 9.99999974738e-06

.text
glabel func_8084FF7C
/* 1DD6C 8084FF7C C482085C */  lwc1    $f2, 0x085C($a0)           ## 0000085C
/* 1DD70 8084FF80 C4840858 */  lwc1    $f4, 0x0858($a0)           ## 00000858
/* 1DD74 8084FF84 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 1DD78 8084FF88 44814000 */  mtc1    $at, $f8                   ## $f8 = 5.00
/* 1DD7C 8084FF8C 46022180 */  add.s   $f6, $f4, $f2              
/* 1DD80 8084FF90 3C018085 */  lui     $at, %hi(D_80855544)       ## $at = 80850000
/* 1DD84 8084FF94 44807000 */  mtc1    $zero, $f14                ## $f14 = 0.00
/* 1DD88 8084FF98 E4860858 */  swc1    $f6, 0x0858($a0)           ## 00000858
/* 1DD8C 8084FF9C C4800858 */  lwc1    $f0, 0x0858($a0)           ## 00000858
/* 1DD90 8084FFA0 46080282 */  mul.s   $f10, $f0, $f8             
/* 1DD94 8084FFA4 460A1481 */  sub.s   $f18, $f2, $f10            
/* 1DD98 8084FFA8 E492085C */  swc1    $f18, 0x085C($a0)          ## 0000085C
/* 1DD9C 8084FFAC C484085C */  lwc1    $f4, 0x085C($a0)           ## 0000085C
/* 1DDA0 8084FFB0 C4265544 */  lwc1    $f6, %lo(D_80855544)($at)  
/* 1DDA4 8084FFB4 3C018085 */  lui     $at, %hi(D_80855548)       ## $at = 80850000
/* 1DDA8 8084FFB8 46062202 */  mul.s   $f8, $f4, $f6              
/* 1DDAC 8084FFBC E488085C */  swc1    $f8, 0x085C($a0)           ## 0000085C
/* 1DDB0 8084FFC0 C482085C */  lwc1    $f2, 0x085C($a0)           ## 0000085C
/* 1DDB4 8084FFC4 4602703E */  c.le.s  $f14, $f2                  
/* 1DDB8 8084FFC8 00000000 */  nop
/* 1DDBC 8084FFCC 45020004 */  bc1fl   .L8084FFE0                 
/* 1DDC0 8084FFD0 46001307 */  neg.s   $f12, $f2                  
/* 1DDC4 8084FFD4 10000002 */  beq     $zero, $zero, .L8084FFE0   
/* 1DDC8 8084FFD8 46001306 */  mov.s   $f12, $f2                  
/* 1DDCC 8084FFDC 46001307 */  neg.s   $f12, $f2                  
.L8084FFE0:
/* 1DDD0 8084FFE0 C4225548 */  lwc1    $f2, %lo(D_80855548)($at)  
/* 1DDD4 8084FFE4 4602603C */  c.lt.s  $f12, $f2                  
/* 1DDD8 8084FFE8 00000000 */  nop
/* 1DDDC 8084FFEC 4500000D */  bc1f    .L80850024                 
/* 1DDE0 8084FFF0 00000000 */  nop
/* 1DDE4 8084FFF4 4600703E */  c.le.s  $f14, $f0                  
/* 1DDE8 8084FFF8 44808000 */  mtc1    $zero, $f16                ## $f16 = 0.00
/* 1DDEC 8084FFFC 45000003 */  bc1f    .L8085000C                 
/* 1DDF0 80850000 E490085C */  swc1    $f16, 0x085C($a0)          ## 0000085C
/* 1DDF4 80850004 10000002 */  beq     $zero, $zero, .L80850010   
/* 1DDF8 80850008 46000306 */  mov.s   $f12, $f0                  
.L8085000C:
/* 1DDFC 8085000C 46000307 */  neg.s   $f12, $f0                  
.L80850010:
/* 1DE00 80850010 4602603C */  c.lt.s  $f12, $f2                  
/* 1DE04 80850014 00000000 */  nop
/* 1DE08 80850018 45000002 */  bc1f    .L80850024                 
/* 1DE0C 8085001C 00000000 */  nop
/* 1DE10 80850020 E4900858 */  swc1    $f16, 0x0858($a0)          ## 00000858
.L80850024:
/* 1DE14 80850024 03E00008 */  jr      $ra                        
/* 1DE18 80850028 00000000 */  nop