glabel func_808267AC
/* 12F8C 808267AC 3C0E0001 */  lui     $t6, 0x0001                ## $t6 = 00010000
/* 12F90 808267B0 01C47021 */  addu    $t6, $t6, $a0              
/* 12F94 808267B4 85CE0998 */  lh      $t6, 0x0998($t6)           ## 00010998
/* 12F98 808267B8 2403FFFC */  addiu   $v1, $zero, 0xFFFC         ## $v1 = FFFFFFFC
/* 12F9C 808267BC 24050004 */  addiu   $a1, $zero, 0x0004         ## $a1 = 00000004
/* 12FA0 808267C0 15C0004B */  bne     $t6, $zero, .L808268F0     
/* 12FA4 808267C4 24060010 */  addiu   $a2, $zero, 0x0010         ## $a2 = 00000010
/* 12FA8 808267C8 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 12FAC 808267CC 34210760 */  ori     $at, $at, 0x0760           ## $at = 00010760
/* 12FB0 808267D0 00811021 */  addu    $v0, $a0, $at              
/* 12FB4 808267D4 944801E8 */  lhu     $t0, 0x01E8($v0)           ## 000001E8
/* 12FB8 808267D8 24010001 */  addiu   $at, $zero, 0x0001         ## $at = 00000001
/* 12FBC 808267DC 2403FFFF */  addiu   $v1, $zero, 0xFFFF         ## $v1 = FFFFFFFF
/* 12FC0 808267E0 24050001 */  addiu   $a1, $zero, 0x0001         ## $a1 = 00000001
/* 12FC4 808267E4 2406000E */  addiu   $a2, $zero, 0x000E         ## $a2 = 0000000E
/* 12FC8 808267E8 2407000E */  addiu   $a3, $zero, 0x000E         ## $a3 = 0000000E
/* 12FCC 808267EC 15010018 */  bne     $t0, $at, .L80826850       
/* 12FD0 808267F0 01004825 */  or      $t1, $t0, $zero            ## $t1 = 00000000
/* 12FD4 808267F4 3C0F8083 */  lui     $t7, %hi(D_8082ABA8)       ## $t7 = 80830000
/* 12FD8 808267F8 85EFABA8 */  lh      $t7, %lo(D_8082ABA8)($t7)  
/* 12FDC 808267FC 0008C040 */  sll     $t8, $t0,  1               
/* 12FE0 80826800 2403FFFA */  addiu   $v1, $zero, 0xFFFA         ## $v1 = FFFFFFFA
/* 12FE4 80826804 15E00005 */  bne     $t7, $zero, .L8082681C     
/* 12FE8 80826808 0058C821 */  addu    $t9, $v0, $t8              
/* 12FEC 8082680C 24050006 */  addiu   $a1, $zero, 0x0006         ## $a1 = 00000006
/* 12FF0 80826810 24060004 */  addiu   $a2, $zero, 0x0004         ## $a2 = 00000004
/* 12FF4 80826814 1000003A */  beq     $zero, $zero, .L80826900   
/* 12FF8 80826818 24070004 */  addiu   $a3, $zero, 0x0004         ## $a3 = 00000004
.L8082681C:
/* 12FFC 8082681C 972A0246 */  lhu     $t2, 0x0246($t9)           ## 00000246
/* 13000 80826820 2403FFFD */  addiu   $v1, $zero, 0xFFFD         ## $v1 = FFFFFFFD
/* 13004 80826824 24050003 */  addiu   $a1, $zero, 0x0003         ## $a1 = 00000003
/* 13008 80826828 29410003 */  slti    $at, $t2, 0x0003           
/* 1300C 8082682C 14200006 */  bne     $at, $zero, .L80826848     
/* 13010 80826830 2406000D */  addiu   $a2, $zero, 0x000D         ## $a2 = 0000000D
/* 13014 80826834 2403FFFA */  addiu   $v1, $zero, 0xFFFA         ## $v1 = FFFFFFFA
/* 13018 80826838 24050005 */  addiu   $a1, $zero, 0x0005         ## $a1 = 00000005
/* 1301C 8082683C 24070007 */  addiu   $a3, $zero, 0x0007         ## $a3 = 00000007
/* 13020 80826840 1000002F */  beq     $zero, $zero, .L80826900   
/* 13024 80826844 24060013 */  addiu   $a2, $zero, 0x0013         ## $a2 = 00000013
.L80826848:
/* 13028 80826848 1000002D */  beq     $zero, $zero, .L80826900   
/* 1302C 8082684C 2407000D */  addiu   $a3, $zero, 0x000D         ## $a3 = 0000000D
.L80826850:
/* 13030 80826850 24010002 */  addiu   $at, $zero, 0x0002         ## $at = 00000002
/* 13034 80826854 1521002A */  bne     $t1, $at, .L80826900       
/* 13038 80826858 00085840 */  sll     $t3, $t0,  1               
/* 1303C 8082685C 004B6021 */  addu    $t4, $v0, $t3              
/* 13040 80826860 95840246 */  lhu     $a0, 0x0246($t4)           ## 00000246
/* 13044 80826864 24010018 */  addiu   $at, $zero, 0x0018         ## $at = 00000018
/* 13048 80826868 2403FFFC */  addiu   $v1, $zero, 0xFFFC         ## $v1 = FFFFFFFC
/* 1304C 8082686C 24050004 */  addiu   $a1, $zero, 0x0004         ## $a1 = 00000004
/* 13050 80826870 2406000C */  addiu   $a2, $zero, 0x000C         ## $a2 = 0000000C
/* 13054 80826874 14810006 */  bne     $a0, $at, .L80826890       
/* 13058 80826878 2407000C */  addiu   $a3, $zero, 0x000C         ## $a3 = 0000000C
/* 1305C 8082687C 2403FFFE */  addiu   $v1, $zero, 0xFFFE         ## $v1 = FFFFFFFE
/* 13060 80826880 24050002 */  addiu   $a1, $zero, 0x0002         ## $a1 = 00000002
/* 13064 80826884 24060020 */  addiu   $a2, $zero, 0x0020         ## $a2 = 00000020
/* 13068 80826888 1000001D */  beq     $zero, $zero, .L80826900   
/* 1306C 8082688C 24070020 */  addiu   $a3, $zero, 0x0020         ## $a3 = 00000020
.L80826890:
/* 13070 80826890 24010017 */  addiu   $at, $zero, 0x0017         ## $at = 00000017
/* 13074 80826894 54810005 */  bnel    $a0, $at, .L808268AC       
/* 13078 80826898 28810006 */  slti    $at, $a0, 0x0006           
/* 1307C 8082689C 2407000D */  addiu   $a3, $zero, 0x000D         ## $a3 = 0000000D
/* 13080 808268A0 10000017 */  beq     $zero, $zero, .L80826900   
/* 13084 808268A4 24060022 */  addiu   $a2, $zero, 0x0022         ## $a2 = 00000022
/* 13088 808268A8 28810006 */  slti    $at, $a0, 0x0006           
.L808268AC:
/* 1308C 808268AC 50200007 */  beql    $at, $zero, .L808268CC     
/* 13090 808268B0 28810006 */  slti    $at, $a0, 0x0006           
/* 13094 808268B4 2403FFFF */  addiu   $v1, $zero, 0xFFFF         ## $v1 = FFFFFFFF
/* 13098 808268B8 24050001 */  addiu   $a1, $zero, 0x0001         ## $a1 = 00000001
/* 1309C 808268BC 2406000A */  addiu   $a2, $zero, 0x000A         ## $a2 = 0000000A
/* 130A0 808268C0 1000000F */  beq     $zero, $zero, .L80826900   
/* 130A4 808268C4 2407000A */  addiu   $a3, $zero, 0x000A         ## $a3 = 0000000A
/* 130A8 808268C8 28810006 */  slti    $at, $a0, 0x0006           
.L808268CC:
/* 130AC 808268CC 1420000C */  bne     $at, $zero, .L80826900     
/* 130B0 808268D0 28810012 */  slti    $at, $a0, 0x0012           
/* 130B4 808268D4 5020000B */  beql    $at, $zero, .L80826904     
/* 130B8 808268D8 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 130BC 808268DC 2403FFFB */  addiu   $v1, $zero, 0xFFFB         ## $v1 = FFFFFFFB
/* 130C0 808268E0 24050003 */  addiu   $a1, $zero, 0x0003         ## $a1 = 00000003
/* 130C4 808268E4 24060008 */  addiu   $a2, $zero, 0x0008         ## $a2 = 00000008
/* 130C8 808268E8 10000005 */  beq     $zero, $zero, .L80826900   
/* 130CC 808268EC 24070008 */  addiu   $a3, $zero, 0x0008         ## $a3 = 00000008
.L808268F0:
/* 130D0 808268F0 3C010001 */  lui     $at, 0x0001                ## $at = 00010000
/* 130D4 808268F4 34210760 */  ori     $at, $at, 0x0760           ## $at = 00010760
/* 130D8 808268F8 00811021 */  addu    $v0, $a0, $at              
/* 130DC 808268FC 24070010 */  addiu   $a3, $zero, 0x0010         ## $a3 = 00000010
.L80826900:
/* 130E0 80826900 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
.L80826904:
/* 130E4 80826904 850D0000 */  lh      $t5, 0x0000($t0)           ## 00000000
/* 130E8 80826908 01A32021 */  addu    $a0, $t5, $v1              
/* 130EC 8082690C 00042400 */  sll     $a0, $a0, 16               
/* 130F0 80826910 00042403 */  sra     $a0, $a0, 16               
/* 130F4 80826914 A5040020 */  sh      $a0, 0x0020($t0)           ## 00000020
/* 130F8 80826918 8C4E0168 */  lw      $t6, 0x0168($v0)           ## 00000168
/* 130FC 8082691C A5C40000 */  sh      $a0, 0x0000($t6)           ## 00010000
/* 13100 80826920 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 13104 80826924 85040000 */  lh      $a0, 0x0000($t0)           ## 00000000
/* 13108 80826928 24840010 */  addiu   $a0, $a0, 0x0010           ## $a0 = 00000010
/* 1310C 8082692C 00042400 */  sll     $a0, $a0, 16               
/* 13110 80826930 00042403 */  sra     $a0, $a0, 16               
/* 13114 80826934 A5040030 */  sh      $a0, 0x0030($t0)           ## 00000030
/* 13118 80826938 8C4F0168 */  lw      $t7, 0x0168($v0)           ## 00000168
/* 1311C 8082693C A5E40010 */  sh      $a0, 0x0010($t7)           ## 80830010
/* 13120 80826940 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 13124 80826944 85180002 */  lh      $t8, 0x0002($t0)           ## 00000002
/* 13128 80826948 03052021 */  addu    $a0, $t8, $a1              
/* 1312C 8082694C 00042400 */  sll     $a0, $a0, 16               
/* 13130 80826950 00042403 */  sra     $a0, $a0, 16               
/* 13134 80826954 A5040012 */  sh      $a0, 0x0012($t0)           ## 00000012
/* 13138 80826958 8C590168 */  lw      $t9, 0x0168($v0)           ## 00000168
/* 1313C 8082695C A7240002 */  sh      $a0, 0x0002($t9)           ## 00000002
/* 13140 80826960 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 13144 80826964 85040002 */  lh      $a0, 0x0002($t0)           ## 00000002
/* 13148 80826968 2484FFF0 */  addiu   $a0, $a0, 0xFFF0           ## $a0 = 00000000
/* 1314C 8082696C 00042400 */  sll     $a0, $a0, 16               
/* 13150 80826970 00042403 */  sra     $a0, $a0, 16               
/* 13154 80826974 A5040032 */  sh      $a0, 0x0032($t0)           ## 00000032
/* 13158 80826978 8C4A0168 */  lw      $t2, 0x0168($v0)           ## 00000168
/* 1315C 8082697C A5440022 */  sh      $a0, 0x0022($t2)           ## 00000022
/* 13160 80826980 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 13164 80826984 850B0000 */  lh      $t3, 0x0000($t0)           ## 00000000
/* 13168 80826988 01662021 */  addu    $a0, $t3, $a2              
/* 1316C 8082698C 00042400 */  sll     $a0, $a0, 16               
/* 13170 80826990 00042403 */  sra     $a0, $a0, 16               
/* 13174 80826994 A5040060 */  sh      $a0, 0x0060($t0)           ## 00000060
/* 13178 80826998 8C4C0168 */  lw      $t4, 0x0168($v0)           ## 00000168
/* 1317C 8082699C A5840040 */  sh      $a0, 0x0040($t4)           ## 00000040
/* 13180 808269A0 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 13184 808269A4 85040040 */  lh      $a0, 0x0040($t0)           ## 00000040
/* 13188 808269A8 24840010 */  addiu   $a0, $a0, 0x0010           ## $a0 = 00000010
/* 1318C 808269AC 00042400 */  sll     $a0, $a0, 16               
/* 13190 808269B0 00042403 */  sra     $a0, $a0, 16               
/* 13194 808269B4 A5040070 */  sh      $a0, 0x0070($t0)           ## 00000070
/* 13198 808269B8 8C4D0168 */  lw      $t5, 0x0168($v0)           ## 00000168
/* 1319C 808269BC A5A40050 */  sh      $a0, 0x0050($t5)           ## 00000050
/* 131A0 808269C0 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 131A4 808269C4 85040002 */  lh      $a0, 0x0002($t0)           ## 00000002
/* 131A8 808269C8 A5040052 */  sh      $a0, 0x0052($t0)           ## 00000052
/* 131AC 808269CC 8C4E0168 */  lw      $t6, 0x0168($v0)           ## 00000168
/* 131B0 808269D0 A5C40042 */  sh      $a0, 0x0042($t6)           ## 00010042
/* 131B4 808269D4 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 131B8 808269D8 85040042 */  lh      $a0, 0x0042($t0)           ## 00000042
/* 131BC 808269DC 2484FFF0 */  addiu   $a0, $a0, 0xFFF0           ## $a0 = 00000000
/* 131C0 808269E0 00042400 */  sll     $a0, $a0, 16               
/* 131C4 808269E4 00042403 */  sra     $a0, $a0, 16               
/* 131C8 808269E8 A5040072 */  sh      $a0, 0x0072($t0)           ## 00000072
/* 131CC 808269EC 8C4F0168 */  lw      $t7, 0x0168($v0)           ## 00000168
/* 131D0 808269F0 A5E40062 */  sh      $a0, 0x0062($t7)           ## 80830062
/* 131D4 808269F4 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 131D8 808269F8 85040000 */  lh      $a0, 0x0000($t0)           ## 00000000
/* 131DC 808269FC A50400A0 */  sh      $a0, 0x00A0($t0)           ## 000000A0
/* 131E0 80826A00 8C580168 */  lw      $t8, 0x0168($v0)           ## 00000168
/* 131E4 80826A04 A7040080 */  sh      $a0, 0x0080($t8)           ## 00000080
/* 131E8 80826A08 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 131EC 80826A0C 85040080 */  lh      $a0, 0x0080($t0)           ## 00000080
/* 131F0 80826A10 24840010 */  addiu   $a0, $a0, 0x0010           ## $a0 = 00000010
/* 131F4 80826A14 00042400 */  sll     $a0, $a0, 16               
/* 131F8 80826A18 00042403 */  sra     $a0, $a0, 16               
/* 131FC 80826A1C A50400B0 */  sh      $a0, 0x00B0($t0)           ## 000000B0
/* 13200 80826A20 8C590168 */  lw      $t9, 0x0168($v0)           ## 00000168
/* 13204 80826A24 A7240090 */  sh      $a0, 0x0090($t9)           ## 00000090
/* 13208 80826A28 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 1320C 80826A2C 850A0002 */  lh      $t2, 0x0002($t0)           ## 00000002
/* 13210 80826A30 01472023 */  subu    $a0, $t2, $a3              
/* 13214 80826A34 00042400 */  sll     $a0, $a0, 16               
/* 13218 80826A38 00042403 */  sra     $a0, $a0, 16               
/* 1321C 80826A3C A5040092 */  sh      $a0, 0x0092($t0)           ## 00000092
/* 13220 80826A40 8C4B0168 */  lw      $t3, 0x0168($v0)           ## 00000168
/* 13224 80826A44 A5640082 */  sh      $a0, 0x0082($t3)           ## 00000082
/* 13228 80826A48 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 1322C 80826A4C 85040082 */  lh      $a0, 0x0082($t0)           ## 00000082
/* 13230 80826A50 2484FFF0 */  addiu   $a0, $a0, 0xFFF0           ## $a0 = 00000000
/* 13234 80826A54 00042400 */  sll     $a0, $a0, 16               
/* 13238 80826A58 00042403 */  sra     $a0, $a0, 16               
/* 1323C 80826A5C A50400B2 */  sh      $a0, 0x00B2($t0)           ## 000000B2
/* 13240 80826A60 8C4C0168 */  lw      $t4, 0x0168($v0)           ## 00000168
/* 13244 80826A64 A58400A2 */  sh      $a0, 0x00A2($t4)           ## 000000A2
/* 13248 80826A68 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 1324C 80826A6C 850D0000 */  lh      $t5, 0x0000($t0)           ## 00000000
/* 13250 80826A70 01A62021 */  addu    $a0, $t5, $a2              
/* 13254 80826A74 00042400 */  sll     $a0, $a0, 16               
/* 13258 80826A78 00042403 */  sra     $a0, $a0, 16               
/* 1325C 80826A7C A50400E0 */  sh      $a0, 0x00E0($t0)           ## 000000E0
/* 13260 80826A80 8C4E0168 */  lw      $t6, 0x0168($v0)           ## 00000168
/* 13264 80826A84 A5C400C0 */  sh      $a0, 0x00C0($t6)           ## 000100C0
/* 13268 80826A88 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 1326C 80826A8C 850400C0 */  lh      $a0, 0x00C0($t0)           ## 000000C0
/* 13270 80826A90 24840010 */  addiu   $a0, $a0, 0x0010           ## $a0 = 00000010
/* 13274 80826A94 00042400 */  sll     $a0, $a0, 16               
/* 13278 80826A98 00042403 */  sra     $a0, $a0, 16               
/* 1327C 80826A9C A50400F0 */  sh      $a0, 0x00F0($t0)           ## 000000F0
/* 13280 80826AA0 8C4F0168 */  lw      $t7, 0x0168($v0)           ## 00000168
/* 13284 80826AA4 A5E400D0 */  sh      $a0, 0x00D0($t7)           ## 808300D0
/* 13288 80826AA8 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 1328C 80826AAC 85180002 */  lh      $t8, 0x0002($t0)           ## 00000002
/* 13290 80826AB0 03072023 */  subu    $a0, $t8, $a3              
/* 13294 80826AB4 00042400 */  sll     $a0, $a0, 16               
/* 13298 80826AB8 00042403 */  sra     $a0, $a0, 16               
/* 1329C 80826ABC A50400D2 */  sh      $a0, 0x00D2($t0)           ## 000000D2
/* 132A0 80826AC0 8C590168 */  lw      $t9, 0x0168($v0)           ## 00000168
/* 132A4 80826AC4 A72400C2 */  sh      $a0, 0x00C2($t9)           ## 000000C2
/* 132A8 80826AC8 8C480168 */  lw      $t0, 0x0168($v0)           ## 00000168
/* 132AC 80826ACC 850400C2 */  lh      $a0, 0x00C2($t0)           ## 000000C2
/* 132B0 80826AD0 2484FFF0 */  addiu   $a0, $a0, 0xFFF0           ## $a0 = 00000000
/* 132B4 80826AD4 00042400 */  sll     $a0, $a0, 16               
/* 132B8 80826AD8 00042403 */  sra     $a0, $a0, 16               
/* 132BC 80826ADC A50400F2 */  sh      $a0, 0x00F2($t0)           ## 000000F2
/* 132C0 80826AE0 8C4A0168 */  lw      $t2, 0x0168($v0)           ## 00000168
/* 132C4 80826AE4 03E00008 */  jr      $ra                        
/* 132C8 80826AE8 A54400E2 */  sh      $a0, 0x00E2($t2)           ## 000000E2