glabel func_80851828
/* 1F618 80851828 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 1F61C 8085182C 3C014020 */  lui     $at, 0x4020                ## $at = 40200000
/* 1F620 80851830 44812000 */  mtc1    $at, $f4                   ## $f4 = 2.50
/* 1F624 80851834 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 1F628 80851838 AFA60028 */  sw      $a2, 0x0028($sp)           
/* 1F62C 8085183C AFA40020 */  sw      $a0, 0x0020($sp)           
/* 1F630 80851840 27A6001C */  addiu   $a2, $sp, 0x001C           ## $a2 = FFFFFFFC
/* 1F634 80851844 AFA50024 */  sw      $a1, 0x0024($sp)           
/* 1F638 80851848 2407000A */  addiu   $a3, $zero, 0x000A         ## $a3 = 0000000A
/* 1F63C 8085184C 0C2116E8 */  jal     func_80845BA0              
/* 1F640 80851850 E7A4001C */  swc1    $f4, 0x001C($sp)           
/* 1F644 80851854 8FA30020 */  lw      $v1, 0x0020($sp)           
/* 1F648 80851858 24010013 */  addiu   $at, $zero, 0x0013         ## $at = 00000013
/* 1F64C 8085185C 8FA50024 */  lw      $a1, 0x0024($sp)           
/* 1F650 80851860 846E00A4 */  lh      $t6, 0x00A4($v1)           ## 000000A4
/* 1F654 80851864 55C10011 */  bnel    $t6, $at, .L808518AC       
/* 1F658 80851868 84B80850 */  lh      $t8, 0x0850($a1)           ## 00000850
/* 1F65C 8085186C 84AF0850 */  lh      $t7, 0x0850($a1)           ## 00000850
/* 1F660 80851870 246420D8 */  addiu   $a0, $v1, 0x20D8           ## $a0 = 000020D8
/* 1F664 80851874 15E00008 */  bne     $t7, $zero, .L80851898     
/* 1F668 80851878 00000000 */  nop
/* 1F66C 8085187C 246420D8 */  addiu   $a0, $v1, 0x20D8           ## $a0 = 000020D8
/* 1F670 80851880 0C042F6F */  jal     func_8010BDBC              
/* 1F674 80851884 AFA50024 */  sw      $a1, 0x0024($sp)           
/* 1F678 80851888 14400007 */  bne     $v0, $zero, .L808518A8     
/* 1F67C 8085188C 8FA50024 */  lw      $a1, 0x0024($sp)           
/* 1F680 80851890 1000000F */  beq     $zero, $zero, .L808518D0   
/* 1F684 80851894 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L80851898:
/* 1F688 80851898 0C042F6F */  jal     func_8010BDBC              
/* 1F68C 8085189C AFA50024 */  sw      $a1, 0x0024($sp)           
/* 1F690 808518A0 1440000A */  bne     $v0, $zero, .L808518CC     
/* 1F694 808518A4 8FA50024 */  lw      $a1, 0x0024($sp)           
.L808518A8:
/* 1F698 808518A8 84B80850 */  lh      $t8, 0x0850($a1)           ## 00000850
.L808518AC:
/* 1F69C 808518AC 2409000B */  addiu   $t1, $zero, 0x000B         ## $t1 = 0000000B
/* 1F6A0 808518B0 27190001 */  addiu   $t9, $t8, 0x0001           ## $t9 = 00000001
/* 1F6A4 808518B4 A4B90850 */  sh      $t9, 0x0850($a1)           ## 00000850
/* 1F6A8 808518B8 84A80850 */  lh      $t0, 0x0850($a1)           ## 00000850
/* 1F6AC 808518BC 29010015 */  slti    $at, $t0, 0x0015           
/* 1F6B0 808518C0 54200003 */  bnel    $at, $zero, .L808518D0     
/* 1F6B4 808518C4 8FBF0014 */  lw      $ra, 0x0014($sp)           
/* 1F6B8 808518C8 A0A90444 */  sb      $t1, 0x0444($a1)           ## 00000444
.L808518CC:
/* 1F6BC 808518CC 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L808518D0:
/* 1F6C0 808518D0 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 1F6C4 808518D4 03E00008 */  jr      $ra                        
/* 1F6C8 808518D8 00000000 */  nop