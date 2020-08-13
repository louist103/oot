.rdata
glabel D_8090D8B8
    .asciz "../z_boss_ganon2.c"
    .balign 4

glabel D_8090D8CC
    .asciz "../z_boss_ganon2.c"
    .balign 4

glabel D_8090D8E0
    .asciz "../z_boss_ganon2.c"
    .balign 4

glabel D_8090D8F4
    .asciz "../z_boss_ganon2.c"
    .balign 4

.text
glabel func_80904FC8
/* 08088 80904FC8 27BDFFA0 */  addiu   $sp, $sp, 0xFFA0           ## $sp = FFFFFFA0
/* 0808C 80904FCC AFBF001C */  sw      $ra, 0x001C($sp)           
/* 08090 80904FD0 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 08094 80904FD4 AFA40060 */  sw      $a0, 0x0060($sp)           
/* 08098 80904FD8 AFA50064 */  sw      $a1, 0x0064($sp)           
/* 0809C 80904FDC 8CA50000 */  lw      $a1, 0x0000($a1)           ## 00000000
/* 080A0 80904FE0 3C068091 */  lui     $a2, %hi(D_8090D8B8)       ## $a2 = 80910000
/* 080A4 80904FE4 24C6D8B8 */  addiu   $a2, $a2, %lo(D_8090D8B8)  ## $a2 = 8090D8B8
/* 080A8 80904FE8 27A40048 */  addiu   $a0, $sp, 0x0048           ## $a0 = FFFFFFE8
/* 080AC 80904FEC 24071600 */  addiu   $a3, $zero, 0x1600         ## $a3 = 00001600
/* 080B0 80904FF0 0C031AB1 */  jal     Graph_OpenDisps              
/* 080B4 80904FF4 00A08025 */  or      $s0, $a1, $zero            ## $s0 = 00000000
/* 080B8 80904FF8 8FAF0060 */  lw      $t7, 0x0060($sp)           
/* 080BC 80904FFC 44802000 */  mtc1    $zero, $f4                 ## $f4 = 0.00
/* 080C0 80905000 8FB80064 */  lw      $t8, 0x0064($sp)           
/* 080C4 80905004 C5E60384 */  lwc1    $f6, 0x0384($t7)           ## 00000384
/* 080C8 80905008 4606203C */  c.lt.s  $f4, $f6                   
/* 080CC 8090500C 00000000 */  nop
/* 080D0 80905010 4502007F */  bc1fl   .L80905210                 
/* 080D4 80905014 8FAD0064 */  lw      $t5, 0x0064($sp)           
/* 080D8 80905018 0C024F61 */  jal     func_80093D84              
/* 080DC 8090501C 8F040000 */  lw      $a0, 0x0000($t8)           ## 00000000
/* 080E0 80905020 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 080E4 80905024 3C09FA00 */  lui     $t1, 0xFA00                ## $t1 = FA000000
/* 080E8 80905028 240AFFC8 */  addiu   $t2, $zero, 0xFFC8         ## $t2 = FFFFFFC8
/* 080EC 8090502C 24590008 */  addiu   $t9, $v0, 0x0008           ## $t9 = 00000008
/* 080F0 80905030 AE1902D0 */  sw      $t9, 0x02D0($s0)           ## 000002D0
/* 080F4 80905034 AC4A0004 */  sw      $t2, 0x0004($v0)           ## 00000004
/* 080F8 80905038 AC490000 */  sw      $t1, 0x0000($v0)           ## 00000000
/* 080FC 8090503C 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 08100 80905040 3C0CFB00 */  lui     $t4, 0xFB00                ## $t4 = FB000000
/* 08104 80905044 3C0DFFC8 */  lui     $t5, 0xFFC8                ## $t5 = FFC80000
/* 08108 80905048 244B0008 */  addiu   $t3, $v0, 0x0008           ## $t3 = 00000008
/* 0810C 8090504C AE0B02D0 */  sw      $t3, 0x02D0($s0)           ## 000002D0
/* 08110 80905050 AC4D0004 */  sw      $t5, 0x0004($v0)           ## 00000004
/* 08114 80905054 AC4C0000 */  sw      $t4, 0x0000($v0)           ## 00000000
/* 08118 80905058 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 0811C 8090505C 3C188091 */  lui     $t8, %hi(D_80908248)       ## $t8 = 80910000
/* 08120 80905060 27188248 */  addiu   $t8, $t8, %lo(D_80908248)  ## $t8 = 80908248
/* 08124 80905064 244E0008 */  addiu   $t6, $v0, 0x0008           ## $t6 = 00000008
/* 08128 80905068 AE0E02D0 */  sw      $t6, 0x02D0($s0)           ## 000002D0
/* 0812C 8090506C 3C0FDE00 */  lui     $t7, 0xDE00                ## $t7 = DE000000
/* 08130 80905070 AC4F0000 */  sw      $t7, 0x0000($v0)           ## 00000000
/* 08134 80905074 AC580004 */  sw      $t8, 0x0004($v0)           ## 00000004
/* 08138 80905078 3C028091 */  lui     $v0, %hi(D_8090EB2C)       ## $v0 = 80910000
/* 0813C 8090507C 8C42EB2C */  lw      $v0, %lo(D_8090EB2C)($v0)  
/* 08140 80905080 3C0142A0 */  lui     $at, 0x42A0                ## $at = 42A00000
/* 08144 80905084 44815000 */  mtc1    $at, $f10                  ## $f10 = 80.00
/* 08148 80905088 C4480028 */  lwc1    $f8, 0x0028($v0)           ## 80910028
/* 0814C 8090508C 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 08150 80905090 C44C0024 */  lwc1    $f12, 0x0024($v0)          ## 80910024
/* 08154 80905094 8C46002C */  lw      $a2, 0x002C($v0)           ## 8091002C
/* 08158 80905098 0C034261 */  jal     Matrix_Translate              
/* 0815C 8090509C 460A4380 */  add.s   $f14, $f8, $f10            
/* 08160 809050A0 8FA40064 */  lw      $a0, 0x0064($sp)           
/* 08164 809050A4 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 08168 809050A8 34211DA0 */  ori     $at, $at, 0x1DA0           ## $at = 00011DA0
/* 0816C 809050AC 0C0347F5 */  jal     func_800D1FD4              
/* 08170 809050B0 00812021 */  addu    $a0, $a0, $at              
/* 08174 809050B4 8FB90060 */  lw      $t9, 0x0060($sp)           
/* 08178 809050B8 24070001 */  addiu   $a3, $zero, 0x0001         ## $a3 = 00000001
/* 0817C 809050BC C72C0384 */  lwc1    $f12, 0x0384($t9)          ## 00000384
/* 08180 809050C0 44066000 */  mfc1    $a2, $f12                  
/* 08184 809050C4 0C0342A3 */  jal     Matrix_Scale              
/* 08188 809050C8 46006386 */  mov.s   $f14, $f12                 
/* 0818C 809050CC 8FA90060 */  lw      $t1, 0x0060($sp)           
/* 08190 809050D0 24050001 */  addiu   $a1, $zero, 0x0001         ## $a1 = 00000001
/* 08194 809050D4 0C0343B5 */  jal     Matrix_RotateZ              
/* 08198 809050D8 C52C0388 */  lwc1    $f12, 0x0388($t1)          ## 00000388
/* 0819C 809050DC 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 081A0 809050E0 3C0BDA38 */  lui     $t3, 0xDA38                ## $t3 = DA380000
/* 081A4 809050E4 356B0003 */  ori     $t3, $t3, 0x0003           ## $t3 = DA380003
/* 081A8 809050E8 244A0008 */  addiu   $t2, $v0, 0x0008           ## $t2 = 00000008
/* 081AC 809050EC AE0A02D0 */  sw      $t2, 0x02D0($s0)           ## 000002D0
/* 081B0 809050F0 AC4B0000 */  sw      $t3, 0x0000($v0)           ## 00000000
/* 081B4 809050F4 8FAC0064 */  lw      $t4, 0x0064($sp)           
/* 081B8 809050F8 3C058091 */  lui     $a1, %hi(D_8090D8CC)       ## $a1 = 80910000
/* 081BC 809050FC 24A5D8CC */  addiu   $a1, $a1, %lo(D_8090D8CC)  ## $a1 = 8090D8CC
/* 081C0 80905100 8D840000 */  lw      $a0, 0x0000($t4)           ## 00000000
/* 081C4 80905104 2406161D */  addiu   $a2, $zero, 0x161D         ## $a2 = 0000161D
/* 081C8 80905108 0C0346A2 */  jal     Matrix_NewMtx              
/* 081CC 8090510C AFA20038 */  sw      $v0, 0x0038($sp)           
/* 081D0 80905110 8FA30038 */  lw      $v1, 0x0038($sp)           
/* 081D4 80905114 3C048091 */  lui     $a0, %hi(D_809082B8)       ## $a0 = 80910000
/* 081D8 80905118 248482B8 */  addiu   $a0, $a0, %lo(D_809082B8)  ## $a0 = 809082B8
/* 081DC 8090511C AC620004 */  sw      $v0, 0x0004($v1)           ## 00000004
/* 081E0 80905120 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 081E4 80905124 00047900 */  sll     $t7, $a0,  4               
/* 081E8 80905128 000FC702 */  srl     $t8, $t7, 28               
/* 081EC 8090512C 3C098016 */  lui     $t1, %hi(gSegments)
/* 081F0 80905130 244D0008 */  addiu   $t5, $v0, 0x0008           ## $t5 = 00000008
/* 081F4 80905134 AE0D02D0 */  sw      $t5, 0x02D0($s0)           ## 000002D0
/* 081F8 80905138 25296FA8 */  addiu   $t1, %lo(gSegments)
/* 081FC 8090513C 0018C880 */  sll     $t9, $t8,  2               
/* 08200 80905140 3C0EDE00 */  lui     $t6, 0xDE00                ## $t6 = DE000000
/* 08204 80905144 03293821 */  addu    $a3, $t9, $t1              
/* 08208 80905148 3C0100FF */  lui     $at, 0x00FF                ## $at = 00FF0000
/* 0820C 8090514C AC4E0000 */  sw      $t6, 0x0000($v0)           ## 00000000
/* 08210 80905150 8CEA0000 */  lw      $t2, 0x0000($a3)           ## 00000000
/* 08214 80905154 3421FFFF */  ori     $at, $at, 0xFFFF           ## $at = 00FFFFFF
/* 08218 80905158 00814024 */  and     $t0, $a0, $at              
/* 0821C 8090515C 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 08220 80905160 01485821 */  addu    $t3, $t2, $t0              
/* 08224 80905164 01616021 */  addu    $t4, $t3, $at              
/* 08228 80905168 AC4C0004 */  sw      $t4, 0x0004($v0)           ## 00000004
/* 0822C 8090516C 8FAD0060 */  lw      $t5, 0x0060($sp)           
/* 08230 80905170 3C01C000 */  lui     $at, 0xC000                ## $at = C0000000
/* 08234 80905174 44819000 */  mtc1    $at, $f18                  ## $f18 = -2.00
/* 08238 80905178 C5B00388 */  lwc1    $f16, 0x0388($t5)          ## 00000390
/* 0823C 8090517C AFA80024 */  sw      $t0, 0x0024($sp)           
/* 08240 80905180 AFA70028 */  sw      $a3, 0x0028($sp)           
/* 08244 80905184 46128302 */  mul.s   $f12, $f16, $f18           
/* 08248 80905188 0C0343B5 */  jal     Matrix_RotateZ              
/* 0824C 8090518C 24050001 */  addiu   $a1, $zero, 0x0001         ## $a1 = 00000001
/* 08250 80905190 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 08254 80905194 8FA70028 */  lw      $a3, 0x0028($sp)           
/* 08258 80905198 8FA80024 */  lw      $t0, 0x0024($sp)           
/* 0825C 8090519C 3C0FDA38 */  lui     $t7, 0xDA38                ## $t7 = DA380000
/* 08260 809051A0 244E0008 */  addiu   $t6, $v0, 0x0008           ## $t6 = 00000008
/* 08264 809051A4 AE0E02D0 */  sw      $t6, 0x02D0($s0)           ## 000002D0
/* 08268 809051A8 35EF0003 */  ori     $t7, $t7, 0x0003           ## $t7 = DA380003
/* 0826C 809051AC AC4F0000 */  sw      $t7, 0x0000($v0)           ## 00000000
/* 08270 809051B0 8FB80064 */  lw      $t8, 0x0064($sp)           
/* 08274 809051B4 3C058091 */  lui     $a1, %hi(D_8090D8E0)       ## $a1 = 80910000
/* 08278 809051B8 24A5D8E0 */  addiu   $a1, $a1, %lo(D_8090D8E0)  ## $a1 = 8090D8E0
/* 0827C 809051BC 8F040000 */  lw      $a0, 0x0000($t8)           ## 00000000
/* 08280 809051C0 24061620 */  addiu   $a2, $zero, 0x1620         ## $a2 = 00001620
/* 08284 809051C4 AFA20030 */  sw      $v0, 0x0030($sp)           
/* 08288 809051C8 AFA70028 */  sw      $a3, 0x0028($sp)           
/* 0828C 809051CC 0C0346A2 */  jal     Matrix_NewMtx              
/* 08290 809051D0 AFA80024 */  sw      $t0, 0x0024($sp)           
/* 08294 809051D4 8FA30030 */  lw      $v1, 0x0030($sp)           
/* 08298 809051D8 8FA70028 */  lw      $a3, 0x0028($sp)           
/* 0829C 809051DC 8FA80024 */  lw      $t0, 0x0024($sp)           
/* 082A0 809051E0 AC620004 */  sw      $v0, 0x0004($v1)           ## 00000004
/* 082A4 809051E4 8E0202D0 */  lw      $v0, 0x02D0($s0)           ## 000002D0
/* 082A8 809051E8 3C09DE00 */  lui     $t1, 0xDE00                ## $t1 = DE000000
/* 082AC 809051EC 3C018000 */  lui     $at, 0x8000                ## $at = 80000000
/* 082B0 809051F0 24590008 */  addiu   $t9, $v0, 0x0008           ## $t9 = 00000008
/* 082B4 809051F4 AE1902D0 */  sw      $t9, 0x02D0($s0)           ## 000002D0
/* 082B8 809051F8 AC490000 */  sw      $t1, 0x0000($v0)           ## 00000000
/* 082BC 809051FC 8CEA0000 */  lw      $t2, 0x0000($a3)           ## 00000000
/* 082C0 80905200 01485821 */  addu    $t3, $t2, $t0              
/* 082C4 80905204 01616021 */  addu    $t4, $t3, $at              
/* 082C8 80905208 AC4C0004 */  sw      $t4, 0x0004($v0)           ## 00000004
/* 082CC 8090520C 8FAD0064 */  lw      $t5, 0x0064($sp)           
.L80905210:
/* 082D0 80905210 3C068091 */  lui     $a2, %hi(D_8090D8F4)       ## $a2 = 80910000
/* 082D4 80905214 24C6D8F4 */  addiu   $a2, $a2, %lo(D_8090D8F4)  ## $a2 = 8090D8F4
/* 082D8 80905218 27A40048 */  addiu   $a0, $sp, 0x0048           ## $a0 = FFFFFFE8
/* 082DC 8090521C 24071623 */  addiu   $a3, $zero, 0x1623         ## $a3 = 00001623
/* 082E0 80905220 0C031AD5 */  jal     Graph_CloseDisps              
/* 082E4 80905224 8DA50000 */  lw      $a1, 0x0000($t5)           ## 00000000
/* 082E8 80905228 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 082EC 8090522C 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 082F0 80905230 27BD0060 */  addiu   $sp, $sp, 0x0060           ## $sp = 00000000
/* 082F4 80905234 03E00008 */  jr      $ra                        
/* 082F8 80905238 00000000 */  nop
