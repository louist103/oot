glabel func_8080510C
/* 013CC 8080510C 27BDFFE0 */  addiu   $sp, $sp, 0xFFE0           ## $sp = FFFFFFE0
/* 013D0 80805110 3C080001 */  lui     $t0, 0x0001                ## $t0 = 00010000
/* 013D4 80805114 35088000 */  ori     $t0, $t0, 0x8000           ## $t0 = 00018000
/* 013D8 80805118 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 013DC 8080511C AFB00018 */  sw      $s0, 0x0018($sp)           
/* 013E0 80805120 00881021 */  addu    $v0, $a0, $t0              
/* 013E4 80805124 844E4A6E */  lh      $t6, 0x4A6E($v0)           ## 00004A6E
/* 013E8 80805128 84584A96 */  lh      $t8, 0x4A96($v0)           ## 00004A96
/* 013EC 8080512C 84494A98 */  lh      $t1, 0x4A98($v0)           ## 00004A98
/* 013F0 80805130 844B4A50 */  lh      $t3, 0x4A50($v0)           ## 00004A50
/* 013F4 80805134 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 013F8 80805138 25CFFFE1 */  addiu   $t7, $t6, 0xFFE1           ## $t7 = FFFFFFE1
/* 013FC 8080513C 2719FFE7 */  addiu   $t9, $t8, 0xFFE7           ## $t9 = FFFFFFE7
/* 01400 80805140 252AFFE7 */  addiu   $t2, $t1, 0xFFE7           ## $t2 = FFFFFFE7
/* 01404 80805144 A44F4A6E */  sh      $t7, 0x4A6E($v0)           ## 00004A6E
/* 01408 80805148 A4594A96 */  sh      $t9, 0x4A96($v0)           ## 00004A96
/* 0140C 8080514C A44A4A98 */  sh      $t2, 0x4A98($v0)           ## 00004A98
/* 01410 80805150 00240821 */  addu    $at, $at, $a0              
/* 01414 80805154 256CFFFF */  addiu   $t4, $t3, 0xFFFF           ## $t4 = FFFFFFFF
/* 01418 80805158 A42CCA50 */  sh      $t4, -0x35B0($at)          ## 0001CA50
/* 0141C 8080515C 844D4A50 */  lh      $t5, 0x4A50($v0)           ## 00004A50
/* 01420 80805160 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 01424 80805164 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 01428 80805168 15A00012 */  bne     $t5, $zero, .L808051B4     
/* 0142C 8080516C 240E0008 */  addiu   $t6, $zero, 0x0008         ## $t6 = 00000008
/* 01430 80805170 A4404A6E */  sh      $zero, 0x4A6E($v0)         ## 00004A6E
/* 01434 80805174 00240821 */  addu    $at, $at, $a0              
/* 01438 80805178 A42ECA50 */  sh      $t6, -0x35B0($at)          ## 0001CA50
/* 0143C 8080517C 844F4A3E */  lh      $t7, 0x4A3E($v0)           ## 00004A3E
/* 01440 80805180 3C010002 */  lui     $at, 0x0002                ## $at = 00020000
/* 01444 80805184 00240821 */  addu    $at, $at, $a0              
/* 01448 80805188 25F80001 */  addiu   $t8, $t7, 0x0001           ## $t8 = FFFFFFE2
/* 0144C 8080518C A438CA3E */  sh      $t8, -0x35C2($at)          ## 0001CA3E
/* 01450 80805190 84464A5E */  lh      $a2, 0x4A5E($v0)           ## 00004A5E
/* 01454 80805194 3C048081 */  lui     $a0, %hi(D_80812A60)       ## $a0 = 80810000
/* 01458 80805198 24842A60 */  addiu   $a0, $a0, %lo(D_80812A60)  ## $a0 = 80812A60
/* 0145C 8080519C 0006C840 */  sll     $t9, $a2,  1               
/* 01460 808051A0 02194821 */  addu    $t1, $s0, $t9              
/* 01464 808051A4 01281821 */  addu    $v1, $t1, $t0              
/* 01468 808051A8 84654A86 */  lh      $a1, 0x4A86($v1)           ## 00004A86
/* 0146C 808051AC 0C00084C */  jal     osSyncPrintf
              
/* 01470 808051B0 84674A8C */  lh      $a3, 0x4A8C($v1)           ## 00004A8C
.L808051B4:
/* 01474 808051B4 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 01478 808051B8 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 0147C 808051BC 27BD0020 */  addiu   $sp, $sp, 0x0020           ## $sp = 00000000
/* 01480 808051C0 03E00008 */  jr      $ra                        
/* 01484 808051C4 00000000 */  nop