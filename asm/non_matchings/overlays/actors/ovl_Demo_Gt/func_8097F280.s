glabel func_8097F280
/* 01C10 8097F280 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 01C14 8097F284 AFBF0024 */  sw      $ra, 0x0024($sp)           
/* 01C18 8097F288 AFB00020 */  sw      $s0, 0x0020($sp)           
/* 01C1C 8097F28C 94A61D74 */  lhu     $a2, 0x1D74($a1)           ## 00001D74
/* 01C20 8097F290 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 01C24 8097F294 24050064 */  addiu   $a1, $zero, 0x0064         ## $a1 = 00000064
/* 01C28 8097F298 28C100A0 */  slti    $at, $a2, 0x00A0           
/* 01C2C 8097F29C 10200014 */  beq     $at, $zero, .L8097F2F0     
/* 01C30 8097F2A0 00C01025 */  or      $v0, $a2, $zero            ## $v0 = 00000000
/* 01C34 8097F2A4 24820178 */  addiu   $v0, $a0, 0x0178           ## $v0 = 00000178
/* 01C38 8097F2A8 240600FF */  addiu   $a2, $zero, 0x00FF         ## $a2 = 000000FF
/* 01C3C 8097F2AC 240E00C8 */  addiu   $t6, $zero, 0x00C8         ## $t6 = 000000C8
/* 01C40 8097F2B0 AC450000 */  sw      $a1, 0x0000($v0)           ## 00000178
/* 01C44 8097F2B4 AC460004 */  sw      $a2, 0x0004($v0)           ## 0000017C
/* 01C48 8097F2B8 AC4E0008 */  sw      $t6, 0x0008($v0)           ## 00000180
/* 01C4C 8097F2BC 24830188 */  addiu   $v1, $a0, 0x0188           ## $v1 = 00000188
/* 01C50 8097F2C0 240F0078 */  addiu   $t7, $zero, 0x0078         ## $t7 = 00000078
/* 01C54 8097F2C4 AC660000 */  sw      $a2, 0x0000($v1)           ## 00000188
/* 01C58 8097F2C8 AC6F0004 */  sw      $t7, 0x0004($v1)           ## 0000018C
/* 01C5C 8097F2CC AC650008 */  sw      $a1, 0x0008($v1)           ## 00000190
/* 01C60 8097F2D0 8C980198 */  lw      $t8, 0x0198($a0)           ## 00000198
/* 01C64 8097F2D4 8C88019C */  lw      $t0, 0x019C($a0)           ## 0000019C
/* 01C68 8097F2D8 24840198 */  addiu   $a0, $a0, 0x0198           ## $a0 = 00000198
/* 01C6C 8097F2DC 27190001 */  addiu   $t9, $t8, 0x0001           ## $t9 = 00000001
/* 01C70 8097F2E0 2509FFFF */  addiu   $t1, $t0, 0xFFFF           ## $t1 = FFFFFFFF
/* 01C74 8097F2E4 AC990000 */  sw      $t9, 0x0000($a0)           ## 00000198
/* 01C78 8097F2E8 1000003B */  beq     $zero, $zero, .L8097F3D8   
/* 01C7C 8097F2EC AC890004 */  sw      $t1, 0x0004($a0)           ## 0000019C
.L8097F2F0:
/* 01C80 8097F2F0 284100AA */  slti    $at, $v0, 0x00AA           
/* 01C84 8097F2F4 10200030 */  beq     $at, $zero, .L8097F3B8     
/* 01C88 8097F2F8 24050064 */  addiu   $a1, $zero, 0x0064         ## $a1 = 00000064
/* 01C8C 8097F2FC 240400AA */  addiu   $a0, $zero, 0x00AA         ## $a0 = 000000AA
/* 01C90 8097F300 240500A0 */  addiu   $a1, $zero, 0x00A0         ## $a1 = 000000A0
/* 01C94 8097F304 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 01C98 8097F308 0C01BE6F */  jal     func_8006F9BC              
/* 01C9C 8097F30C AFA00010 */  sw      $zero, 0x0010($sp)         
/* 01CA0 8097F310 3C01C27C */  lui     $at, 0xC27C                ## $at = C27C0000
/* 01CA4 8097F314 44812000 */  mtc1    $at, $f4                   ## $f4 = -63.00
/* 01CA8 8097F318 3C014323 */  lui     $at, 0x4323                ## $at = 43230000
/* 01CAC 8097F31C 44814000 */  mtc1    $at, $f8                   ## $f8 = 163.00
/* 01CB0 8097F320 46040182 */  mul.s   $f6, $f0, $f4              
/* 01CB4 8097F324 3C01C31B */  lui     $at, 0xC31B                ## $at = C31B0000
/* 01CB8 8097F328 44819000 */  mtc1    $at, $f18                  ## $f18 = -155.00
/* 01CBC 8097F32C 3C01437F */  lui     $at, 0x437F                ## $at = 437F0000
/* 01CC0 8097F330 26020178 */  addiu   $v0, $s0, 0x0178           ## $v0 = 00000178
/* 01CC4 8097F334 46120102 */  mul.s   $f4, $f0, $f18             
/* 01CC8 8097F338 26030188 */  addiu   $v1, $s0, 0x0188           ## $v1 = 00000188
/* 01CCC 8097F33C 46083280 */  add.s   $f10, $f6, $f8             
/* 01CD0 8097F340 44813000 */  mtc1    $at, $f6                   ## $f6 = 255.00
/* 01CD4 8097F344 3C01C2C8 */  lui     $at, 0xC2C8                ## $at = C2C80000
/* 01CD8 8097F348 24050064 */  addiu   $a1, $zero, 0x0064         ## $a1 = 00000064
/* 01CDC 8097F34C 4600540D */  trunc.w.s $f16, $f10                 
/* 01CE0 8097F350 46062200 */  add.s   $f8, $f4, $f6              
/* 01CE4 8097F354 440B8000 */  mfc1    $t3, $f16                  
/* 01CE8 8097F358 44818000 */  mtc1    $at, $f16                  ## $f16 = -100.00
/* 01CEC 8097F35C 3C014348 */  lui     $at, 0x4348                ## $at = 43480000
/* 01CF0 8097F360 4600428D */  trunc.w.s $f10, $f8                  
/* 01CF4 8097F364 44812000 */  mtc1    $at, $f4                   ## $f4 = 200.00
/* 01CF8 8097F368 46100482 */  mul.s   $f18, $f0, $f16            
/* 01CFC 8097F36C 3C01C1A0 */  lui     $at, 0xC1A0                ## $at = C1A00000
/* 01D00 8097F370 44045000 */  mfc1    $a0, $f10                  
/* 01D04 8097F374 44815000 */  mtc1    $at, $f10                  ## $f10 = -20.00
/* 01D08 8097F378 3C0142F0 */  lui     $at, 0x42F0                ## $at = 42F00000
/* 01D0C 8097F37C AC4B0000 */  sw      $t3, 0x0000($v0)           ## 00000178
/* 01D10 8097F380 460A0402 */  mul.s   $f16, $f0, $f10            
/* 01D14 8097F384 46049180 */  add.s   $f6, $f18, $f4             
/* 01D18 8097F388 44819000 */  mtc1    $at, $f18                  ## $f18 = 120.00
/* 01D1C 8097F38C AC440004 */  sw      $a0, 0x0004($v0)           ## 0000017C
/* 01D20 8097F390 4600320D */  trunc.w.s $f8, $f6                   
/* 01D24 8097F394 46128100 */  add.s   $f4, $f16, $f18            
/* 01D28 8097F398 440E4000 */  mfc1    $t6, $f8                   
/* 01D2C 8097F39C 4600218D */  trunc.w.s $f6, $f4                   
/* 01D30 8097F3A0 AC4E0008 */  sw      $t6, 0x0008($v0)           ## 00000180
/* 01D34 8097F3A4 AC650008 */  sw      $a1, 0x0008($v1)           ## 00000190
/* 01D38 8097F3A8 AC640000 */  sw      $a0, 0x0000($v1)           ## 00000188
/* 01D3C 8097F3AC 44183000 */  mfc1    $t8, $f6                   
/* 01D40 8097F3B0 10000009 */  beq     $zero, $zero, .L8097F3D8   
/* 01D44 8097F3B4 AC780004 */  sw      $t8, 0x0004($v1)           ## 0000018C
.L8097F3B8:
/* 01D48 8097F3B8 26020178 */  addiu   $v0, $s0, 0x0178           ## $v0 = 00000178
/* 01D4C 8097F3BC AC450000 */  sw      $a1, 0x0000($v0)           ## 00000178
/* 01D50 8097F3C0 AC450004 */  sw      $a1, 0x0004($v0)           ## 0000017C
/* 01D54 8097F3C4 AC450008 */  sw      $a1, 0x0008($v0)           ## 00000180
/* 01D58 8097F3C8 26030188 */  addiu   $v1, $s0, 0x0188           ## $v1 = 00000188
/* 01D5C 8097F3CC AC650000 */  sw      $a1, 0x0000($v1)           ## 00000188
/* 01D60 8097F3D0 AC650004 */  sw      $a1, 0x0004($v1)           ## 0000018C
/* 01D64 8097F3D4 AC650008 */  sw      $a1, 0x0008($v1)           ## 00000190
.L8097F3D8:
/* 01D68 8097F3D8 8FBF0024 */  lw      $ra, 0x0024($sp)           
/* 01D6C 8097F3DC 8FB00020 */  lw      $s0, 0x0020($sp)           
/* 01D70 8097F3E0 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 01D74 8097F3E4 03E00008 */  jr      $ra                        
/* 01D78 8097F3E8 00000000 */  nop
