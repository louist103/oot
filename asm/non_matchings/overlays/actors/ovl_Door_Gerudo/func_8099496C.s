glabel func_8099496C
/* 0038C 8099496C 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 00390 80994970 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 00394 80994974 AFA5001C */  sw      $a1, 0x001C($sp)           
/* 00398 80994978 90820166 */  lbu     $v0, 0x0166($a0)           ## 00000166
/* 0039C 8099497C 24052814 */  addiu   $a1, $zero, 0x2814         ## $a1 = 00002814
/* 003A0 80994980 14400003 */  bne     $v0, $zero, .L80994990     
/* 003A4 80994984 244EFFFF */  addiu   $t6, $v0, 0xFFFF           ## $t6 = FFFFFFFF
/* 003A8 80994988 10000003 */  beq     $zero, $zero, .L80994998   
/* 003AC 8099498C 00001825 */  or      $v1, $zero, $zero          ## $v1 = 00000000
.L80994990:
/* 003B0 80994990 A08E0166 */  sb      $t6, 0x0166($a0)           ## 00000166
/* 003B4 80994994 31C300FF */  andi    $v1, $t6, 0x00FF           ## $v1 = 000000FF
.L80994998:
/* 003B8 80994998 54600008 */  bnel    $v1, $zero, .L809949BC     
/* 003BC 8099499C 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 003C0 809949A0 0C00BE0A */  jal     Audio_PlayActorSound2
              
/* 003C4 809949A4 AFA40018 */  sw      $a0, 0x0018($sp)           
/* 003C8 809949A8 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 003CC 809949AC 3C0F8099 */  lui     $t7, %hi(func_809949C8)    ## $t7 = 80990000
/* 003D0 809949B0 25EF49C8 */  addiu   $t7, $t7, %lo(func_809949C8) ## $t7 = 809949C8
/* 003D4 809949B4 AC8F0168 */  sw      $t7, 0x0168($a0)           ## 00000168
/* 003D8 809949B8 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L809949BC:
/* 003DC 809949BC 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 003E0 809949C0 03E00008 */  jr      $ra                        
/* 003E4 809949C4 00000000 */  nop