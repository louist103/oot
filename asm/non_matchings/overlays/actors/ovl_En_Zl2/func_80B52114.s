.rdata
glabel D_80B52A30
    .asciz "[31m En_Oa2 の arg_data がおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n[m"
    .balign 4

.text
glabel func_80B52114
/* 03784 80B52114 27BDFFE8 */  addiu   $sp, $sp, 0xFFE8           ## $sp = FFFFFFE8
/* 03788 80B52118 AFBF0014 */  sw      $ra, 0x0014($sp)           
/* 0378C 80B5211C 8482001C */  lh      $v0, 0x001C($a0)           ## 0000001C
/* 03790 80B52120 00803025 */  or      $a2, $a0, $zero            ## $a2 = 00000000
/* 03794 80B52124 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 03798 80B52128 1040000F */  beq     $v0, $zero, .L80B52168     
/* 0379C 80B5212C 00000000 */  nop
/* 037A0 80B52130 10410005 */  beq     $v0, $at, .L80B52148       
/* 037A4 80B52134 24010004 */  addiu   $at, $zero, 0x0004         ## $at = 00000004
/* 037A8 80B52138 10410007 */  beq     $v0, $at, .L80B52158       
/* 037AC 80B5213C 3C0480B5 */  lui     $a0, %hi(D_80B52A30)       ## $a0 = 80B50000
/* 037B0 80B52140 1000000D */  beq     $zero, $zero, .L80B52178   
/* 037B4 80B52144 24842A30 */  addiu   $a0, $a0, %lo(D_80B52A30)  ## $a0 = 80B52A30
.L80B52148:
/* 037B8 80B52148 0C2D44D7 */  jal     func_80B5135C              
/* 037BC 80B5214C 00C02025 */  or      $a0, $a2, $zero            ## $a0 = 00000000
/* 037C0 80B52150 10000010 */  beq     $zero, $zero, .L80B52194   
/* 037C4 80B52154 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L80B52158:
/* 037C8 80B52158 0C2D4743 */  jal     func_80B51D0C              
/* 037CC 80B5215C 00C02025 */  or      $a0, $a2, $zero            ## $a0 = 00000000
/* 037D0 80B52160 1000000C */  beq     $zero, $zero, .L80B52194   
/* 037D4 80B52164 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L80B52168:
/* 037D8 80B52168 0C2D3F64 */  jal     func_80B4FD90              
/* 037DC 80B5216C 00C02025 */  or      $a0, $a2, $zero            ## $a0 = 00000000
/* 037E0 80B52170 10000008 */  beq     $zero, $zero, .L80B52194   
/* 037E4 80B52174 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L80B52178:
/* 037E8 80B52178 AFA5001C */  sw      $a1, 0x001C($sp)           
/* 037EC 80B5217C 0C00084C */  jal     osSyncPrintf
              
/* 037F0 80B52180 AFA60018 */  sw      $a2, 0x0018($sp)           
/* 037F4 80B52184 8FA5001C */  lw      $a1, 0x001C($sp)           
/* 037F8 80B52188 0C2D3F64 */  jal     func_80B4FD90              
/* 037FC 80B5218C 8FA40018 */  lw      $a0, 0x0018($sp)           
/* 03800 80B52190 8FBF0014 */  lw      $ra, 0x0014($sp)           
.L80B52194:
/* 03804 80B52194 27BD0018 */  addiu   $sp, $sp, 0x0018           ## $sp = 00000000
/* 03808 80B52198 03E00008 */  jr      $ra                        
/* 0380C 80B5219C 00000000 */  nop