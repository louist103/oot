glabel func_8084D980
/* 1B770 8084D980 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 1B774 8084D984 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 1B778 8084D988 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 1B77C 8084D98C AFA40028 */  sw      $a0, 0x0028($sp)           
/* 1B780 8084D990 AFA60030 */  sw      $a2, 0x0030($sp)           
/* 1B784 8084D994 AFA70034 */  sw      $a3, 0x0034($sp)           
/* 1B788 8084D998 84F80000 */  lh      $t8, 0x0000($a3)           ## 00000000
/* 1B78C 8084D99C 84AE083C */  lh      $t6, 0x083C($a1)           ## 0000083C
/* 1B790 8084D9A0 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 1B794 8084D9A4 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 1B798 8084D9A8 01D81023 */  subu    $v0, $t6, $t8              
/* 1B79C 8084D9AC 00021400 */  sll     $v0, $v0, 16               
/* 1B7A0 8084D9B0 00021403 */  sra     $v0, $v0, 16               
/* 1B7A4 8084D9B4 04400003 */  bltz    $v0, .L8084D9C4            
/* 1B7A8 8084D9B8 00021823 */  subu    $v1, $zero, $v0            
/* 1B7AC 8084D9BC 10000001 */  beq     $zero, $zero, .L8084D9C4   
/* 1B7B0 8084D9C0 00401825 */  or      $v1, $v0, $zero            ## $v1 = 00000000
.L8084D9C4:
/* 1B7B4 8084D9C4 28616001 */  slti    $at, $v1, 0x6001           
/* 1B7B8 8084D9C8 14200014 */  bne     $at, $zero, .L8084DA1C     
/* 1B7BC 8084D9CC 8FA50030 */  lw      $a1, 0x0030($sp)           
/* 1B7C0 8084D9D0 3C070400 */  lui     $a3, 0x0400                ## $a3 = 04000000
/* 1B7C4 8084D9D4 24E73328 */  addiu   $a3, $a3, 0x3328           ## $a3 = 04003328
/* 1B7C8 8084D9D8 AFA70024 */  sw      $a3, 0x0024($sp)           
/* 1B7CC 8084D9DC 26040838 */  addiu   $a0, $s0, 0x0838           ## $a0 = 00000838
/* 1B7D0 8084D9E0 24050000 */  addiu   $a1, $zero, 0x0000         ## $a1 = 00000000
/* 1B7D4 8084D9E4 0C01DE80 */  jal     Math_ApproxF
              
/* 1B7D8 8084D9E8 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 1B7DC 8084D9EC 8FA30034 */  lw      $v1, 0x0034($sp)           
/* 1B7E0 8084D9F0 10400004 */  beq     $v0, $zero, .L8084DA04     
/* 1B7E4 8084D9F4 8FA70024 */  lw      $a3, 0x0024($sp)           
/* 1B7E8 8084D9F8 84790000 */  lh      $t9, 0x0000($v1)           ## 00000000
/* 1B7EC 8084D9FC 1000001F */  beq     $zero, $zero, .L8084DA7C   
/* 1B7F0 8084DA00 A619083C */  sh      $t9, 0x083C($s0)           ## 0000083C
.L8084DA04:
/* 1B7F4 8084DA04 44802000 */  mtc1    $zero, $f4                 ## $f4 = 0.00
/* 1B7F8 8084DA08 8FA80030 */  lw      $t0, 0x0030($sp)           
/* 1B7FC 8084DA0C E5040000 */  swc1    $f4, 0x0000($t0)           ## 00000000
/* 1B800 8084DA10 8609083C */  lh      $t1, 0x083C($s0)           ## 0000083C
/* 1B804 8084DA14 10000019 */  beq     $zero, $zero, .L8084DA7C   
/* 1B808 8084DA18 A4690000 */  sh      $t1, 0x0000($v1)           ## 00000000
.L8084DA1C:
/* 1B80C 8084DA1C 8FA60034 */  lw      $a2, 0x0034($sp)           
/* 1B810 8084DA20 0C20FF5E */  jal     func_8083FD78              
/* 1B814 8084DA24 8FA70028 */  lw      $a3, 0x0028($sp)           
/* 1B818 8084DA28 18400004 */  blez    $v0, .L8084DA3C            
/* 1B81C 8084DA2C 8FA30034 */  lw      $v1, 0x0034($sp)           
/* 1B820 8084DA30 3C070400 */  lui     $a3, 0x0400                ## $a3 = 04000000
/* 1B824 8084DA34 10000011 */  beq     $zero, $zero, .L8084DA7C   
/* 1B828 8084DA38 24E732F0 */  addiu   $a3, $a3, 0x32F0           ## $a3 = 040032F0
.L8084DA3C:
/* 1B82C 8084DA3C 04410003 */  bgez    $v0, .L8084DA4C            
/* 1B830 8084DA40 3C070400 */  lui     $a3, 0x0400                ## $a3 = 04000000
/* 1B834 8084DA44 1000000D */  beq     $zero, $zero, .L8084DA7C   
/* 1B838 8084DA48 24E732D8 */  addiu   $a3, $a3, 0x32D8           ## $a3 = 040032D8
.L8084DA4C:
/* 1B83C 8084DA4C 860A00B6 */  lh      $t2, 0x00B6($s0)           ## 000000B6
/* 1B840 8084DA50 846B0000 */  lh      $t3, 0x0000($v1)           ## 00000000
/* 1B844 8084DA54 3C070400 */  lui     $a3, 0x0400                ## $a3 = 04000000
/* 1B848 8084DA58 24E732C8 */  addiu   $a3, $a3, 0x32C8           ## $a3 = 040032C8
/* 1B84C 8084DA5C 014B1023 */  subu    $v0, $t2, $t3              
/* 1B850 8084DA60 00021400 */  sll     $v0, $v0, 16               
/* 1B854 8084DA64 00021403 */  sra     $v0, $v0, 16               
/* 1B858 8084DA68 18400004 */  blez    $v0, .L8084DA7C            
/* 1B85C 8084DA6C 00000000 */  nop
/* 1B860 8084DA70 3C070400 */  lui     $a3, 0x0400                ## $a3 = 04000000
/* 1B864 8084DA74 10000001 */  beq     $zero, $zero, .L8084DA7C   
/* 1B868 8084DA78 24E732D0 */  addiu   $a3, $a3, 0x32D0           ## $a3 = 040032D0
.L8084DA7C:
/* 1B86C 8084DA7C 8E0C01BC */  lw      $t4, 0x01BC($s0)           ## 000001BC
/* 1B870 8084DA80 8FA40028 */  lw      $a0, 0x0028($sp)           
/* 1B874 8084DA84 02002825 */  or      $a1, $s0, $zero            ## $a1 = 00000000
/* 1B878 8084DA88 10EC0005 */  beq     $a3, $t4, .L8084DAA0       
/* 1B87C 8084DA8C 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 1B880 8084DA90 0C20CB1B */  jal     func_80832C6C              
/* 1B884 8084DA94 00E03025 */  or      $a2, $a3, $zero            ## $a2 = 040032D0
/* 1B888 8084DA98 10000001 */  beq     $zero, $zero, .L8084DAA0   
/* 1B88C 8084DA9C 24020001 */  addiu   $v0, $zero, 0x0001         ## $v0 = 00000001
.L8084DAA0:
/* 1B890 8084DAA0 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 1B894 8084DAA4 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 1B898 8084DAA8 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 1B89C 8084DAAC 03E00008 */  jr      $ra                        
/* 1B8A0 8084DAB0 00000000 */  nop