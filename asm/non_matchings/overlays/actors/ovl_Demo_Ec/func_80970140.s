.rdata
glabel D_8097075C
    .asciz "\x1B[31mDemo_Ec_main_bank:バンクを読めない arg_data = %d!\n\x1B[m"
    .balign 4

.text
glabel func_80970140
/* 02C90 80970140 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 02C94 80970144 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 02C98 80970148 AFB10018 */  sw      $s1, 0x0018($sp)           
/* 02C9C 8097014C AFB00014 */  sw      $s0, 0x0014($sp)           
/* 02CA0 80970150 AFA5003C */  sw      $a1, 0x003C($sp)           
/* 02CA4 80970154 8483001C */  lh      $v1, 0x001C($a0)           ## 0000001C
/* 02CA8 80970158 8FB0003C */  lw      $s0, 0x003C($sp)           
/* 02CAC 8097015C 3C0E8097 */  lui     $t6, %hi(D_80970398)       ## $t6 = 80970000
/* 02CB0 80970160 00031040 */  sll     $v0, $v1,  1               
/* 02CB4 80970164 01C27021 */  addu    $t6, $t6, $v0              
/* 02CB8 80970168 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 02CBC 8097016C 85CE0398 */  lh      $t6, %lo(D_80970398)($t6)  
/* 02CC0 80970170 3C058097 */  lui     $a1, %hi(D_80970350)       ## $a1 = 80970000
/* 02CC4 80970174 342117A4 */  ori     $at, $at, 0x17A4           ## $at = 000117A4
/* 02CC8 80970178 00808825 */  or      $s1, $a0, $zero            ## $s1 = 00000000
/* 02CCC 8097017C 00A22821 */  addu    $a1, $a1, $v0              
/* 02CD0 80970180 02018021 */  addu    $s0, $s0, $at              
/* 02CD4 80970184 84A50350 */  lh      $a1, %lo(D_80970350)($a1)  
/* 02CD8 80970188 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02CDC 8097018C AFA3002C */  sw      $v1, 0x002C($sp)           
/* 02CE0 80970190 0C02604B */  jal     Object_GetIndex
              ## ObjectIndex
/* 02CE4 80970194 A7AE0028 */  sh      $t6, 0x0028($sp)           
/* 02CE8 80970198 AFA20024 */  sw      $v0, 0x0024($sp)           
/* 02CEC 8097019C 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02CF0 809701A0 0C02604B */  jal     Object_GetIndex
              ## ObjectIndex
/* 02CF4 809701A4 87A50028 */  lh      $a1, 0x0028($sp)           
/* 02CF8 809701A8 04400003 */  bltz    $v0, .L809701B8            
/* 02CFC 809701AC AFA20020 */  sw      $v0, 0x0020($sp)           
/* 02D00 809701B0 8FA50024 */  lw      $a1, 0x0024($sp)           
/* 02D04 809701B4 04A10008 */  bgez    $a1, .L809701D8            
.L809701B8:
/* 02D08 809701B8 3C048097 */  lui     $a0, %hi(D_8097075C)       ## $a0 = 80970000
/* 02D0C 809701BC 2484075C */  addiu   $a0, $a0, %lo(D_8097075C)  ## $a0 = 8097075C
/* 02D10 809701C0 0C00084C */  jal     osSyncPrintf
              
/* 02D14 809701C4 8FA5002C */  lw      $a1, 0x002C($sp)           
/* 02D18 809701C8 0C00B55C */  jal     Actor_Kill
              
/* 02D1C 809701CC 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 02D20 809701D0 10000010 */  beq     $zero, $zero, .L80970214   
/* 02D24 809701D4 8FBF001C */  lw      $ra, 0x001C($sp)           
.L809701D8:
/* 02D28 809701D8 0C026062 */  jal     Object_IsLoaded
              
/* 02D2C 809701DC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02D30 809701E0 1040000B */  beq     $v0, $zero, .L80970210     
/* 02D34 809701E4 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02D38 809701E8 0C026062 */  jal     Object_IsLoaded
              
/* 02D3C 809701EC 8FA50020 */  lw      $a1, 0x0020($sp)           
/* 02D40 809701F0 10400007 */  beq     $v0, $zero, .L80970210     
/* 02D44 809701F4 8FAF0024 */  lw      $t7, 0x0024($sp)           
/* 02D48 809701F8 AE2F01A0 */  sw      $t7, 0x01A0($s1)           ## 000001A0
/* 02D4C 809701FC 8FB80020 */  lw      $t8, 0x0020($sp)           
/* 02D50 80970200 02202025 */  or      $a0, $s1, $zero            ## $a0 = 00000000
/* 02D54 80970204 AE3801A4 */  sw      $t8, 0x01A4($s1)           ## 000001A4
/* 02D58 80970208 0C25C037 */  jal     func_809700DC              
/* 02D5C 8097020C 8FA5003C */  lw      $a1, 0x003C($sp)           
.L80970210:
/* 02D60 80970210 8FBF001C */  lw      $ra, 0x001C($sp)           
.L80970214:
/* 02D64 80970214 8FB00014 */  lw      $s0, 0x0014($sp)           
/* 02D68 80970218 8FB10018 */  lw      $s1, 0x0018($sp)           
/* 02D6C 8097021C 03E00008 */  jr      $ra                        
/* 02D70 80970220 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000