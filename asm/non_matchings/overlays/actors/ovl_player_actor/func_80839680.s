glabel func_80839680
/* 07470 80839680 27BDFFC0 */  addiu   $sp, $sp, 0xFFC0           ## $sp = FFFFFFC0
/* 07474 80839684 AFA40040 */  sw      $a0, 0x0040($sp)           
/* 07478 80839688 AFA50044 */  sw      $a1, 0x0044($sp)           
/* 0747C 8083968C 00A02025 */  or      $a0, $a1, $zero            ## $a0 = 00000000
/* 07480 80839690 AFA60048 */  sw      $a2, 0x0048($sp)           
/* 07484 80839694 00C02825 */  or      $a1, $a2, $zero            ## $a1 = 00000000
/* 07488 80839698 AFBF002C */  sw      $ra, 0x002C($sp)           
/* 0748C 8083969C AFA7004C */  sw      $a3, 0x004C($sp)           
/* 07490 808396A0 00E03025 */  or      $a2, $a3, $zero            ## $a2 = 00000000
/* 07494 808396A4 0C20E577 */  jal     func_808395DC              
/* 07498 808396A8 27A70034 */  addiu   $a3, $sp, 0x0034           ## $a3 = FFFFFFF4
/* 0749C 808396AC 8FA50040 */  lw      $a1, 0x0040($sp)           
/* 074A0 808396B0 C7A40038 */  lwc1    $f4, 0x0038($sp)           
/* 074A4 808396B4 C7A6003C */  lwc1    $f6, 0x003C($sp)           
/* 074A8 808396B8 8FAE0050 */  lw      $t6, 0x0050($sp)           
/* 074AC 808396BC 24060018 */  addiu   $a2, $zero, 0x0018         ## $a2 = 00000018
/* 074B0 808396C0 8FA70034 */  lw      $a3, 0x0034($sp)           
/* 074B4 808396C4 AFA00018 */  sw      $zero, 0x0018($sp)         
/* 074B8 808396C8 AFA0001C */  sw      $zero, 0x001C($sp)         
/* 074BC 808396CC AFA00020 */  sw      $zero, 0x0020($sp)         
/* 074C0 808396D0 24A41C24 */  addiu   $a0, $a1, 0x1C24           ## $a0 = 00001C24
/* 074C4 808396D4 E7A40010 */  swc1    $f4, 0x0010($sp)           
/* 074C8 808396D8 E7A60014 */  swc1    $f6, 0x0014($sp)           
/* 074CC 808396DC 0C00C7D4 */  jal     Actor_Spawn
              ## ActorSpawn
/* 074D0 808396E0 AFAE0024 */  sw      $t6, 0x0024($sp)           
/* 074D4 808396E4 8FBF002C */  lw      $ra, 0x002C($sp)           
/* 074D8 808396E8 27BD0040 */  addiu   $sp, $sp, 0x0040           ## $sp = 00000000
/* 074DC 808396EC 03E00008 */  jr      $ra                        
/* 074E0 808396F0 00000000 */  nop