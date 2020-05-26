.include "macro.inc"

 # assembler directives
 .set noat      # allow manual use of $at
 .set noreorder # don't insert nops after branches
 .set gp=64     # allow use of 64-bit general purpose registers

.section .data

.balign 16

glabel D_80853410
 .word 0x00000000, 0x00000000, 0x00000101, 0x01010101, 0x01010101, 0x01010000
glabel D_80853428
 .word 0x42600000, 0x42B40000, 0x3F800000, 0x42DE0000, 0x428C0000, 0x429ECCCD, 0x426C0000, 0x42240000, 0x41980000, 0x42100000, 0x42333333, 0x42600000, 0x42880000, 0x428C0000, 0x41900000, 0x41700000, 0x428C0000, 0x0009123F, 0x01670008, 0x1256017C, 0x000917EA, 0x01670008, 0x1256017C, 0x000917EA, 0x01670009, 0x17EA0167, 0x00091E0D, 0x017C0009, 0x17EA0167, 0x00091E0D, 0x017C0008, 0x1256017C, 0x000917EA, 0x0167F9C8, 0x1256017C, 0xF9C917EA, 0x01670000, 0x00800000, 0x04002718, 0x04002720, 0x04002838, 0x04002E70, 0x04002E78, 0x04002E80, 0x04002E88, 0x04002D90, 0x04002D98, 0x04002D70, 0x04002D78, 0x04002E50, 0x04002E58, 0x04002E68, 0x04002E60, 0x42200000, 0x42700000, 0x3F25A5A6, 0x428E0000, 0x42480000, 0x423C0000, 0x421C0000, 0x41D80000, 0x41980000, 0x41B00000, 0x41ECCCCD, 0x42000000, 0x42400000, 0x42352D2E, 0x41600000, 0x41400000, 0x425C0000, 0xFFE80DED, 0x036CFFE8, 0x0D92035E, 0xFFE81371, 0x03A90008, 0x1256017C, 0x000917EA, 0x0167FFE8, 0x137103A9, 0xFFE8195F, 0x03A90009, 0x17EA0167, 0x00091E0D, 0x017C0008, 0x1256017C, 0x000917EA, 0x0167F9C8, 0x1256017C, 0xF9C917EA, 0x01670020, 0x00000000, 0x04002318, 0x04002360, 0x040023A8, 0x040023E0, 0x040023E8, 0x040023F0, 0x040023F8, 0x04002D90, 0x04002D98, 0x04002D70, 0x04002D78, 0x040023C0, 0x040023C8, 0x040023D8, 0x040023D0
glabel D_808535D0
 .word 0x00000000
glabel D_808535D4
 .word 0x00000000
glabel D_808535D8
 .word 0x00000000
glabel D_808535DC
 .word 0x00000000
glabel D_808535E0
 .word 0x00000000
glabel D_808535E4
 .word 0x00000000
glabel D_808535E8
 .word 0x3F800000
glabel D_808535EC
 .word 0x3F800000
glabel D_808535F0
 .word 0x00000000
glabel D_808535F4
 .word 0x00000000
glabel D_808535F8
 .word 0x00000000
glabel D_808535FC
 .word 0x00000000
glabel D_80853600
 .word 0x00000000
glabel D_80853604
 .word 0x00000000
glabel D_80853608
 .word 0x00000000
glabel D_8085360C
 .word 0x00000000
glabel D_80853610
 .word 0x00000000
glabel D_80853614
 .word 0x00000000
glabel D_80853618
 .word 0x00000000
glabel D_8085361C
 .word 0x680F680E, 0x68116808
glabel D_80853624
 .word 0x8E59E032, 0x00CE8C0C, 0xEE3400BB, 0x0980D833, 0x00D90380, 0x353100E9, 0x06803330, 0x00E70E80, 0x343500E8, 0x000DE537, 0x00C70A80, 0x2D3600DD, 0x0B802E4F, 0x00DD0F80, 0x363900EA, 0x23803B69, 0x00EF0880, 0x2F3A00DE, 0x11804138, 0x00F62F80, 0x5E020109, 0x14800142, 0x00C61580, 0x384300EB, 0x16803744, 0x00EB1780, 0x394500EB, 0x18800146, 0x00C61A80, 0x309800DF, 0x1B804599, 0x010B1080, 0xC24800F3, 0x25804F10, 0x01362680, 0x32110135, 0x22804448, 0x01092480, 0x31120134, 0x27805013, 0x01372B80, 0x51170138, 0x2D802901, 0x00DA2E80, 0x44480109, 0x30805403, 0x01413180, 0x53040140, 0x32804005, 0x00F53380, 0x56080143, 0x34805709, 0x01463580, 0x5A0D0149, 0x3680520E, 0x013F3780, 0x550A0142, 0x3B8074A4, 0x018D3D80, 0x434B00F8, 0x3EA0E34C, 0x00CB3FA0, 0xD44D00DC, 0x40803A4E, 0x00EE42A0, 0x3C5000F2, 0x43A03D51, 0x00F24580, 0x47530118, 0x46805F54, 0x01574B80, 0x165600BE, 0x4C801757, 0x00BE4D80, 0x185800BF, 0x4E801959, 0x00BF4F80, 0x1A5A00BF, 0x5180495B, 0x012D5280, 0x4A5C012D, 0x53802ACD, 0x00DB5480, 0x2BCE00DB, 0x6F802168, 0x00C87080, 0x247B00D7, 0x0780463A, 0x010E5850, 0xB8DC0119, 0x728013C6, 0x00BD7A80, 0x14C200BD, 0x74800AC7, 0x00B97580, 0x0B6700B8, 0x76801C66, 0x00C87780, 0xFE6000AA, 0x786FE252, 0x00CD796E, 0xE15200CD, 0x5680225E, 0x00D15780, 0x235F00D1, 0x2180299A, 0x00DA8380, 0x095500B7, 0x9248DBE6, 0x00D89349, 0xDAE600D8, 0x944AD9E6, 0x00D88400, 0x936F017F
glabel D_808537EC
 .word 0x850192CC, 0x017F8602, 0x91F0017F, 0x728013C6, 0x00BD8280, 0x309800DF, 0x28805B14, 0x01502980, 0x5C150151, 0x2A805D16, 0x01525080, 0x58790147, 0x87148FF1, 0x017F8813, 0x8EF2017F, 0x3D80430C, 0x00F80480, 0x60700158, 0x0C806171, 0x01581280, 0x62720158, 0x71809DB4, 0x015C0580, 0x64AD015D, 0x0D8065AE, 0x015D1380, 0x66AF015D, 0x47806C07, 0x017B4880, 0x6C07017B, 0x8A0DE537, 0x00C78B0D, 0xE53700C7, 0x8C0CEE34, 0x00BB8D0C, 0xEE3400BB, 0x0259E032, 0x00CE8F59, 0xE03200CE, 0x9059E032, 0x00CE9159, 0xE03200CE, 0x9550B8DC, 0x01199680, 0xD83300D9, 0x9780D833, 0x00D91980, 0x3F4700F4, 0x1D80687A, 0x01741C80, 0x675D0173, 0x20806A97, 0x01761E80, 0x70F90176, 0x7780FEF3, 0x00AA8400, 0x93F4017F, 0x850192F5, 0x017F8602, 0x91F6017F, 0x87148FF7, 0x017F7A80, 0x14FA00BD, 0x9880E590, 0x00C79980, 0xE59100C7, 0x9A80EEA7, 0x00BB9B80, 0xEEA800BB, 0x4980736C, 0x017BFF00, 0x00000000, 0xFF000000, 0x00000000
glabel D_80853914
 .word 0x04003240, 0x04003238, 0x04003238, 0x04002BE0, 0x04003240, 0x04003240
glabel D_8085392C
 .word 0x04003290, 0x04003268, 0x04003268, 0x04002BF8, 0x04003290, 0x04003290
glabel D_80853944
 .word 0x04003140, 0x04002B38, 0x04003138, 0x04002B40, 0x04003140, 0x04003140
glabel D_8085395C
 .word 0x04002E98, 0x040029E8, 0x04002E98, 0x040029F0, 0x04002E98, 0x04002E98
glabel D_80853974
 .word 0x04002FB0, 0x04002FA8, 0x04002FB0, 0x04002A40, 0x04002FB0, 0x04002FB0
glabel D_8085398C
 .word 0x04003220, 0x04002590, 0x04002590, 0x04002BC0, 0x04003220, 0x04003220
glabel D_808539A4
 .word 0x04003230, 0x040025D0, 0x040025D0, 0x04002BD0, 0x04003230, 0x04003230
glabel D_808539BC
 .word 0x04002BB0, 0x040031F8, 0x040031F8, 0x04002BB0, 0x04002BB0, 0x04002BB0, 0x04003088, 0x04002A70, 0x04002A70, 0x04003088, 0x04003088, 0x04003088
glabel D_808539EC
 .word 0x04002750, 0x04002748, 0x04002748, 0x04002750, 0x04002750, 0x04002750
glabel D_80853A04
 .word 0x04002330, 0x04002330, 0x04002330, 0x04002330, 0x04002330, 0x04002330
glabel D_80853A1C
 .word 0x04002760, 0x04002758, 0x04002758, 0x04002760, 0x04002760, 0x04002760
glabel D_80853A34
 .word 0x04002338, 0x04002338, 0x04002338, 0x04002338, 0x04002338, 0x04002338
glabel D_80853A4C
 .word 0x04002E08, 0x04002E00, 0x04002E00, 0x04002E08, 0x04002E08, 0x04002E08
glabel D_80853A64
 .word 0x04003028, 0x04003020, 0x04003020, 0x04003028, 0x04003028, 0x04003028
glabel D_80853A7C
 .word 0x04003170, 0x04003168, 0x04003168, 0x04003170, 0x04003170, 0x04003170
glabel D_80853A94
 .word 0x04003038, 0x04003030, 0x04003030, 0x04002A68, 0x04003038, 0x04003038
glabel D_80853AAC
 .word 0x04002FC0, 0x04002FB8, 0x04002FB8, 0x04002FC8, 0x04002FC0, 0x04002FC0
glabel D_80853AC4
 .word 0x04003278, 0x04003270, 0x04003270, 0x04002BE8, 0x04003278, 0x04003278
glabel D_80853ADC
 .word 0x04003288, 0x04003280, 0x04003280, 0x04002BF0, 0x04003288, 0x04003288
glabel D_80853AF4
 .word 0x04002EB8, 0x04002EA0, 0x04002EA0, 0x04002EB8, 0x040026C8, 0x04002EB8
glabel D_80853B0C
 .word 0x04002ED8, 0x04002ED0, 0x04002ED0, 0x04002ED8, 0x040026D0, 0x04002ED8
glabel D_80853B24
 .word 0x04002EB0, 0x04002EA8, 0x04002EA8, 0x04002EB0, 0x04002EB0, 0x04002EB0
glabel D_80853B3C
 .word 0x04003190, 0x04003188, 0x04003188, 0x04002B68, 0x04003190, 0x04003190
glabel D_80853B54
 .word 0x04003178, 0x04002568, 0x04002568, 0x04002B58, 0x04003178, 0x04003178
glabel D_80853B6C
 .word 0x04003180, 0x04002570, 0x04002570, 0x04002B60, 0x04003180, 0x04003180
glabel D_80853B84
 .word 0x04002D60, 0x04002D58, 0x04002D58, 0x04002D60, 0x04002D60, 0x04002D60
glabel D_80853B9C
 .word 0x04002BB8, 0x04003218, 0x04003218, 0x04002BB8, 0x04002BB8, 0x04002BB8
glabel D_80853BB4
 .word 0x04002BC8, 0x04003228, 0x04003228, 0x04002BC8, 0x04002BC8, 0x04002BC8
glabel D_80853BCC
 .word 0x040031C8, 0x040031C0, 0x040031C0, 0x040031C8, 0x040031C8, 0x040031C8
glabel D_80853BE4
 .word 0x04003118, 0x04003110, 0x04003110, 0x04003118, 0x04003118, 0x04003118
glabel D_80853BFC
 .word 0x04002DE8, 0x04002DE8, 0x04002DE8, 0x04002DE8, 0x04002DE8, 0x04002DE8
glabel D_80853C14
 .word 0x04002E30, 0x04002E18, 0x04002E18, 0x04002E30, 0x04002E30, 0x04002E30
glabel D_80853C2C
 .word 0x04002E40, 0x04002E38, 0x04002E38, 0x04002E40, 0x04002E40, 0x04002E40
glabel D_80853C44
 .word 0x04002E28, 0x04002E20, 0x04002E20, 0x04002E28, 0x04002E28, 0x04002E28
glabel D_80853C5C
 .word 0x040030C8, 0x040030C0, 0x040030C0, 0x040030C8, 0x040030C8, 0x040030C8
glabel D_80853C74
 .word 0x040030D8, 0x040030D0, 0x040030D0, 0x040030D8, 0x040030D8, 0x040030D8
glabel D_80853C8C
 .word 0x040030B8, 0x040030B0, 0x040030B0, 0x040030B8, 0x040030B8, 0x040030B8
glabel D_80853CA4
 .word 0x04002F20, 0x04002F18, 0x04002F18, 0x04002F20, 0x04002F20, 0x04002F20
glabel D_80853CBC
 .word 0x04002FF0, 0x04002FE8, 0x04002FE8, 0x04002FF0, 0x04002FF0, 0x04002FF0
glabel D_80853CD4
 .word 0x04003010, 0x04003008, 0x04003008, 0x04003010, 0x04003010, 0x04003010
glabel D_80853CEC
 .word 0x04003000, 0x04002FF8, 0x04002FF8, 0x04003000, 0x04003000, 0x04003000
glabel D_80853D04
 .word 0x04002EF0, 0x04002EE8, 0x04002EE8, 0x04002EF8, 0x04002EF0, 0x04002EF0
glabel D_80853D1C
 .word 0x040031E0, 0x040031D8, 0x040031D8, 0x040031E8, 0x040031E0, 0x040031E0
glabel D_80853D34
 .word 0x04003468, 0x04003438, 0x04003438, 0x04003468, 0x04003468, 0x04003468
glabel D_80853D4C
 .word 0x04002A28
glabel D_80853D50
 .word 0x04002A38
glabel D_80853D54
 .word 0x04002A30, 0x04002950, 0x04002960, 0x04002958, 0x040029D0, 0x040029E0, 0x040029D8, 0x04002988, 0x04002998, 0x04002990
glabel D_80853D7C
 .word 0x04003248, 0x04003200, 0x04003258, 0x04003210, 0x04003250, 0x04003208, 0x04003250, 0x04003208, 0x04003430, 0x040033F0, 0x04003430, 0x040033F0, 0x04003430, 0x040033F0, 0x040033F8, 0x040033D0, 0x04003400, 0x040033D8, 0x04003420, 0x04003420, 0x04003408, 0x040033E0, 0x04003410, 0x040033E8, 0x04003418, 0x04003418, 0x04003428, 0x04003428
glabel D_80853DEC
 .word 0x680EDFF8
glabel D_80853DF0
 .word 0x680FDFEE
glabel D_80853DF4
 .word 0x6809DFF3
glabel D_80853DF8
 .word 0x6809DFF6
glabel D_80853DFC
 .word 0x0871082C, 0x08710830, 0x08710834, 0x08710838, 0x0871F7C4
glabel D_80853E10
 .word 0x00004019, 0x0000401E, 0x0000402C, 0x00004030, 0x00004034, 0x0000BFC8
glabel D_80853E28
 .word 0x181F0810, 0x181F0814, 0x181FF7BA
glabel D_80853E34
 .word 0x1812080A, 0x6814200A, 0x18010816, 0x6800DFEA
glabel D_80853E44
 .word 0x18010827, 0x6800DFD9
glabel D_80853E4C
 .word 0x6811DFEC
.word D_80853DEC
.word D_80853DF0
.word D_80853DF4
.word D_80853DF8
.word D_80853DFC
.word D_80853E10
.word D_80853E28
.word D_80853E34
.word D_80853E44
.word D_80853E4C
.word 0x00000000
glabel D_80853E7C
 .word 0x00000101, 0x02020202, 0x0A0A0A0A, 0x0A0A0303, 0x04040808, 0x05050606, 0x07070909, 0x00000000
glabel D_80853E9C
 .word 0x061B1208, 0x091A0F1C, 0x1D131011, 0x0A181442, 0x2E070B19, 0x1E252726, 0x2A1F2824, 0x20212329, 0x222C2D2B, 0x3A3B3C3D, 0x3E3F4041, 0x032F3031, 0x32333435, 0x36373839, 0x090A0B04, 0x03050000
glabel D_80853EDC
 .word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_808349DC
.word func_808349DC
.word func_808349DC
.word func_8083485C
.word func_8083485C
.word func_8083501C
.word func_8083501C
.word func_8083501C
.word func_8083501C
.word func_8083501C
.word func_8083501C
.word func_8083501C
.word func_8083501C
.word func_8083501C
.word func_8083501C
.word func_808356E8
.word func_808356E8
.word func_80835800
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
.word func_8083485C
glabel D_80853FE8
 .word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_8083377C
.word func_80833790
.word func_8083379C
.word func_8083379C
.word func_8083379C
.word func_8083379C
.word func_8083379C
.word func_8083379C
.word func_8083379C
.word func_8083379C
.word func_80833910
.word func_80833910
.word func_808337D4
.word func_808337D4
.word func_80833984
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
.word func_80833770
glabel D_808540F4
 .word 0x04002F50, 0x0C000000, 0x04003080, 0x06000000, 0x04002C68, 0x08000000, 0x04003090, 0x08000000, 0x04002A20, 0x08000000, 0x04002F30, 0x0A000000, 0x04002C58, 0x07000000, 0x04002C60, 0x0B000000, 0x04002F50, 0x0C000000, 0x04003078, 0x04000000, 0x04003058, 0x04000000, 0x04002F38, 0x04000000, 0x040024E0, 0x05000000, 0x04002F48, 0x0D000000
glabel D_80854164
 .word 0x08FBFDFA, 0x080B0500, 0xFF040509, 0x03010002, 0x030906FC, 0xFE07060A, 0x08FBFDFA, 0x080B08FB, 0xFDFA080B
glabel D_80854188
 .word 0x02000010, 0x090000DA
glabel D_80854190
 .word 0x04002A80, 0x04002A90, 0x04002A88, 0x01040000, 0x040028C0, 0x040028C8, 0x04002498, 0x01040000, 0x04002A98, 0x04002AA0, 0x04002540, 0x00050000, 0x040028D0, 0x040028D8, 0x040024A0, 0x01070000, 0x04002968, 0x04002970, 0x040024C0, 0x01040000, 0x04002880, 0x04002888, 0x04002478, 0x00050000, 0x04002978, 0x04002980, 0x040024C8, 0x02080000, 0x04002890, 0x04002898, 0x04002480, 0x03080000, 0x040029A0, 0x040029A8, 0x040024D0, 0x00040000, 0x040028A0, 0x040028A8, 0x04002488, 0x00050000, 0x040029B0, 0x040029B8, 0x040024D8, 0x00060000, 0x040028B0, 0x040028B8, 0x04002490, 0x01050000, 0x04002AA8, 0x04002AB0, 0x04002548, 0x00030000, 0x040028E0, 0x040028E8, 0x040024A8, 0x00030000, 0x04002AB8, 0x04002AC0, 0x04002550, 0x01090000, 0x040028F0, 0x040028F8, 0x040024B0, 0x01080000
glabel D_80854290
 .word 0x04002A60, 0x04002A50, 0x04002A50, 0x010A0000
glabel D_808542A0
 .word 0x04002900, 0x04002910, 0x04002910, 0x010B0000, 0x04002A50, 0x04002A58, 0x04002A58, 0x01020000, 0x04002910, 0x04002908, 0x04002908, 0x01020000
glabel D_808542D0
 .word 0x04002B80, 0x04002B88, 0x04002B88, 0x01050000, 0x04002B70, 0x04002B78, 0x04002B78, 0x01040000, 0x04002C40, 0x04002C50, 0x04002C48, 0x030A0000, 0x04002C70, 0x04002C80, 0x04002C78, 0x020B0000, 0x04002B28, 0x04002B30, 0x04002560, 0x000C0000, 0x04002940, 0x04002948, 0x040024B8, 0x000F0000, 0x040029C0, 0x040029C8, 0x04002560, 0x00100000, 0x040029C0, 0x040029C8, 0x040024B8, 0x00100000
glabel D_80854350
 .word 0x04002AE8, 0x04002920
glabel D_80854358
 .word 0x04002AE0, 0x04002920
glabel D_80854360
 .word 0x04002AF0, 0x04002928
glabel D_80854368
 .word 0x04002AF8, 0x04002930
glabel D_80854370
 .word 0x04002B00, 0x04002938
glabel D_80854378
 .word 0x04002AD8, 0x04002918
glabel D_80854380
 .word 0x18190000
glabel D_80854384
 .word 0x1A1B0000
glabel D_80854388
 .word 0x40000002, 0x00040001
glabel D_80854390
 .byte 0x0C, 0x18, 0x18, 0x0C, 0x18
glabel D_80854395
 .byte 0x0C
glabel D_80854396
 .byte 0x00, 0x00, 0x18, 0x07, 0x18, 0x21, 0x18, 0x27, 0x00, 0x00
glabel D_808543A0
 .word 0x04040800
glabel D_808543A4
 .word 0x040025C0, 0x040025C8
glabel D_808543AC
 .word 0x04002580, 0x04002588
glabel D_808543B4
 .word 0x04002510, 0x04002518
glabel D_808543BC
 .word 0x04002510, 0x04002520
glabel D_808543C4
 .word 0x04002EC0, 0x04002A08
glabel D_808543CC
 .word 0x040026F0, 0x04002CC8
glabel D_808543D4
 .byte 0x04, 0x00, 0x26, 0xC0, 0x04, 0x00
glabel D_808543DA
 .byte 0x2C, 0xC0, 0x18, 0x30, 0x18, 0x35
glabel D_808543E0
 .word 0x0D020409, 0x0A0B08F9
glabel D_808543E8
 .word 0x0D010205, 0x0304090A, 0x0B0708FA
glabel D_808543F4
 .word 0x0D010203, 0x04090A0B, 0x0807FA00
glabel D_80854400
 .word 0x0D020409, 0x0A0B08F9
glabel D_80854408
 .word 0x0D020409, 0x0A0B0C08, 0xF9000000
glabel D_80854414
 .word 0xF9000000
glabel D_80854418
 .word 0x000B0102, 0x03050409, 0x0807FA00
glabel D_80854424
 .word 0x000B0102, 0x030C0504, 0x090807FA
glabel D_80854430
 .word 0x0D010203, 0x0C050409, 0x0A0B0807, 0xFA000000
glabel D_80854440
 .word 0x0A08F900
glabel D_80854444
 .word 0x000C05FC
glabel D_80854448
 .word func_8083B998
.word func_80839800
.word func_8083E5A8
.word func_8083E0FC
.word func_8083B644
.word func_8083F7BC
.word func_8083C1DC
.word func_80850224
.word func_8083C544
.word func_8083EB44
.word func_8083BDBC
.word func_8083C2B0
.word func_80838A14
.word func_8083B040
glabel D_80854480
 .word 0x0C040408
glabel D_80854484
 .word 0x16171617
glabel D_80854488
 .word 0x00000200
glabel D_8085448C
 .word 0x08000000, 0x00000100, 0x02000000, 0x00000400, 0x04000000, 0x00000002, 0x08000000, 0x00000040, 0x40000000
glabel D_808544B0
 .word 0x04002F80, 0x04002F78, 0x04002DE0, 0x04002DD8
glabel D_808544C0
 .word 0x04002F70, 0x04002528, 0x04002DC8, 0x040024F0
glabel D_808544D0
 .word 0x02010100
glabel D_808544D4
 .word 0x44F90000, 0x43C98000, 0xC5568000, 0x00000000
glabel D_808544E4
 .word 0x44960000, 0xC4A7E000, 0x4570A000, 0x00000000
glabel D_808544F4
 .word 0x783C0000
glabel D_808544F8
 .word 0x045B0482, 0x0340044B, 0x02A20201, 0x03B804EE, 0x03C00463, 0x01CD0394, 0x0340057C
glabel D_80854514
 .word 0x0B090305, 0x07000000
glabel D_8085451C
 .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0xC8, 0x00
glabel D_80854527
 .byte 0x00
glabel D_80854528
 .word 0x0B471916, 0x1D1E0E1F, 0x20212223, 0x24252617, 0x181A1B1C, 0x51525315, 0x15151515, 0x15000000
glabel D_80854548
 .word 0x04002F88, 0x04002690, 0x04003198
glabel D_80854554
 .word 0x04002648, 0x04002640, 0x02030000, 0x04002680, 0x04002678, 0x05030000
glabel D_8085456C
 .word 0x00000000, 0x42C80000, 0x42200000
glabel D_80854578
 .word 0x04003398, 0x420CAE14, 0x40D3851E, 0x040033A8, 0xC208A3D7, 0x40FD1EB8
glabel D_80854590
 .word 0x04002EE0, 0x040031D0, 0xFFDB0871, 0xF8310000, 0x00940470, 0xF3980000, 0xFFB504A9, 0x0C9F0000, 0x08010402
glabel D_808545B4
 .word 0x00000000, 0x00000000, 0x00000000
glabel D_808545C0
 .word 0x00000000, 0x00000000, 0x00000000
glabel D_808545CC
 .word 0x04002B10, 0x04002B20, 0x04002B08, 0x04002B18
glabel D_808545DC
 .word 0x00004014, 0x0000BFE2
glabel D_808545E4
 .word 0x00000000, 0x00000000, 0x40A00000
glabel D_808545F0
 .word 0x0850103C, 0x0000408C, 0x000040A4, 0x0000BF56
glabel D_80854600
 .word 0xF8B42864, 0x681AF0FF, 0x8C96681A
glabel D_8085460C
 .word 0x68002001, 0x08001806, 0x083C0806, 0x0000D7EE
glabel D_8085461C
 .word 0x68012031, 0x6800DF1A
glabel D_80854624
 .word 0x05001139, 0x08010000, 0x01000000, 0x00000000, 0x00000000, 0xFFCFFFFF, 0x00000000, 0x00010100, 0x000C003C, 0x00000000, 0x00000000
glabel D_80854650
 .word 0x0A090000, 0x08030000, 0x02000000, 0x00000100, 0x00010000, 0xFFCFFFFF, 0x00000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
glabel D_808546A0
 .word 0x09091500, 0x08030000, 0x02000000, 0x00100000, 0x00000000, 0xDFCFFFFF, 0x00000000, 0x01010000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
glabel D_808546F0
 .word 0x3C3B0000
glabel D_808546F4
 .word 0xBF800000, 0x428A0000, 0x41A00000
glabel D_80854700
 .word 0x009E00F4, 0x009F0000
glabel D_80854708
 .word 0x304C01F4
glabel D_8085470C
 .word 0x00000000, 0x00080000, 0xFFFFFFFF, 0xFFFFFF40, 0xFFFFFF00, 0xFFFFFF00, 0x04000200, 0x00000000, 0x00000000
glabel D_80854730
 .word 0xFFC70D31, 0x00000000
glabel D_80854738
 .word func_80846648
.word func_808467D4
.word func_80846660
.word func_808468A8
.word func_808468E8
.word func_808469BC
.word func_80846A68
.word func_80846978
.word func_8083CA54
.word func_8083CA54
.word func_8083CA54
.word func_8083CA54
.word func_8083CA54
.word func_8083CA20
.word func_8083CA54
.word func_8083CA9C
glabel D_80854778
 .word 0x00000000, 0x42480000, 0x00000000
glabel D_80854784
 .word 0x42F00000, 0x43700000, 0x43B40000
glabel D_80854790
 .word 0x15161718, 0x191A1B1C
glabel D_80854798
 .word 0x00000000
glabel D_8085479C
 .word 0x41900000
glabel D_808547A0
 .word 0x00000000
glabel D_808547A4
 .word 0x00000000, 0x3F000000, 0x00000000
glabel D_808547B0
 .word 0x00000000, 0x3F000000, 0x00000000
glabel D_808547BC
 .word 0xFFFF64FF
glabel D_808547C0
 .word 0xFF320000
glabel D_808547C4
 .word 0x00030305, 0x0408090D, 0x0E0F1011, 0x12EA1718, 0x191A1B1C, 0x1D1F2021, 0x22DD1E24, 0x26D9D8D7, 0x2A2B2D2E, 0x00000043, 0x302FCE33, 0xCCCB3637, 0x38393A3B, 0x3C3D3E3F, 0x40BFBE44, 0x0B454647, 0x08084849, 0x4E4F5059, 0x5A5B5C4D, 0x135E0000
glabel D_80854814
 .word 0x00000000, 0x00000000, 0x43480000
glabel D_80854820
 .word 0x40000000, 0x40800000, 0x40E00000
glabel D_8085482C
 .word 0x3F000000, 0x3F800000, 0x40400000
glabel D_80854838
 .word 0x00000000, 0x00000000
glabel D_80854840
 .word 0xC1F00000, 0x0602B060, 0x0602AD40, 0x0602AF70, 0x0602CA38, 0x0602B350, 0x0602B580, 0x0602B788, 0x0602B1F0
glabel D_80854864
 .word 0x00000000, 0x00000000
glabel D_8085486C
 .word 0x000000FF
glabel D_80854870
 .word 0x08401003, 0x0840EFEB
glabel D_80854878
 .word 0x08401004, 0x0840EFE8
glabel D_80854880
 .word 0x00000000, 0x41D00000, 0xC2200000
glabel D_8085488C
 .word 0x00000000
glabel D_80854890
 .word 0x00000000, 0x41D00000
glabel D_80854898
 .word 0x41200000, 0x41A00000
glabel D_808548A0
 .word 0x42200000, 0x42480000
glabel D_808548A8
 .word 0x080A080A, 0x080A0814, 0x080AF7E2
glabel D_808548B4
 .word 0x00003028, 0x00003030, 0x00003038, 0x00003040, 0x00003048, 0x00003050, 0x00003058, 0x00003060, 0x0000CF98
glabel D_808548D8
 .word 0x0000300A, 0x00003012, 0x0000301A, 0x00003022, 0x00003034, 0x0000303C, 0x00003044, 0x0000304C, 0x0000CFAC
glabel D_808548FC
 .word 0x42200000, 0x00000000, 0x00000000, 0xC2200000, 0x00000000, 0x00000000
glabel D_80854914
 .word 0x42700000, 0x41A00000, 0x00000000, 0xC2700000, 0x41A00000, 0x00000000
glabel D_8085492C
 .word 0x42700000, 0xC1A00000, 0x00000000, 0xC2700000
glabel D_8085493C
 .word 0xC1A00000, 0x00000000, 0x04003370, 0x04003368, 0x04003380, 0x04003358, 0x04003338, 0x04003348, 0x04003350
glabel D_80854960
 .word 0x00000000, 0x00000000, 0x04003388, 0x04003388, 0x04003388, 0x04003360, 0x04003340, 0x04003340, 0x04003340, 0x00000000, 0x00000000
glabel D_8085498C
 .word 0x040033C8, 0x040033B8, 0x040033C0
glabel D_80854998
 .word 0x203A192A
glabel D_8085499C
 .word 0xFFBB1BEA, 0xFEF60000
glabel D_808549A4
 .word 0x08710830, 0x0871083A, 0x08710844, 0x0872085C, 0x0872086E, 0x0872087E, 0x08720884, 0x0872F778
glabel D_808549C4
 .word 0x00002800, 0x0833080A, 0x0830F7E7
glabel D_808549D0
 .word 0x0839F800
glabel D_808549D4
 .byte 0x06, 0x00, 0x04
glabel D_808549D7
 .byte 0xF6, 0x06, 0x04, 0x01, 0xF1, 0x05, 0x68, 0x05, 0xF4
glabel D_808549E0
 .word 0x00003857, 0x68042057, 0x68142045, 0x0000D785
glabel D_808549F0
 .word 0x184EF7C4
glabel D_808549F4
 .byte 0x68, 0x14, 0x20, 0x05, 0x00, 0x00, 0xD7, 0xF1, 0x01, 0x03, 0x02, 0x04, 0x04, 0x00
glabel D_80854A02
 .byte 0x00, 0x00
glabel D_80854A04
 .word 0x0018182A, 0x46000021, 0x191F4700, 0x00F01C20, 0x5D000020, 0x1D217A00
glabel D_80854A1C
 .word 0x00000000, 0x00000000, 0x40A00000
glabel D_80854A28
 .word 0x00210000, 0x00F00000, 0x00200002
glabel D_80854A34
 .word 0x68142026, 0x286CF7D8
glabel D_80854A3C
 .word 0x0877F7E2
glabel D_80854A40
 .word 0x00000000, 0x42200000, 0x42340000
glabel D_80854A4C
 .word 0x00000000, 0x00000000, 0x00000000
glabel D_80854A58
 .word 0x04002CF8, 0x04002CE0, 0x04002D10
glabel D_80854A64
 .word 0x04002D00, 0x04002CE8, 0x04002D18
glabel D_80854A70
 .word 0x04002D08, 0x04002CF0, 0x04002D20
glabel D_80854A7C
 .word 0x460A0A00
glabel D_80854A80
 .word 0x083D0814, 0x68002014, 0x0000CFE6
glabel D_80854A8C
 .word 0x00004014, 0x6817DFE2, 0x00004014, 0x6815DFD4, 0x681C2014, 0x1818F7EC
glabel D_80854AA4
 .word 0x00000000
.word func_80851008
.word func_80851030
.word func_80851094
.word func_808510B4
.word func_808510D4
.word func_808510F4
.word func_80851114
.word func_80851134
.word func_80851154
.word func_80851174
.word func_808511D4
.word func_808511FC
.word func_80851294
.word func_80851050
.word func_80851194
.word func_808511B4
.word func_80851248
.word func_808512E0
glabel D_80854AF0
 .word 0x00002822, 0x0871082D, 0x08710833, 0x0871F7C0
glabel D_80854B00
 .word 0x68162003, 0x0000300F, 0x00003018, 0x0000301E, 0x6808DFE1
glabel D_80854B14
 .word 0x0000CFF6
glabel D_80854B18
 .word 0x00000000, 0x00000000, 0xFF000000
.word func_808515A4
.word 0x02000000, 0x04002790, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x03000000, 0x04002740, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0xFF000000
.word func_808515A4
.word 0x02000000, 0x04002778, 0xFF000000
.word func_80851788
.word 0x03000000, 0x04002860, 0xFF000000
.word func_808518DC
.word 0x07000000, 0x04002348, 0x05000000, 0x04002350, 0x05000000, 0x04002358, 0x05000000, 0x040023B0, 0x07000000, 0x040023B8, 0xFF000000
.word func_808519EC
.word 0x02000000, 0x04002728, 0x02000000, 0x04002738, 0x00000000, 0x00000000, 0xFF000000
.word func_80851B90
.word 0x03000000, 0x040027A8, 0x09000000, 0x04002DB0, 0x02000000, 0x04002DC0, 0xFF000000
.word func_80851D2C
.word 0x02000000, 0x04003098, 0x03000000, 0x04002780, 0xFF000000
.word func_808515A4
.word 0x02000000, 0x04003088, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x05000000, 0x04002320, 0xFF000000
.word func_80851368
.word 0xFF000000
.word func_80851E64
.word 0x05000000, 0x04002328, 0x10000000, 0x04002F90, 0xFF000000
.word func_80851F84
.word 0xFF000000
.word func_80851E90
.word 0x06000000, 0x04002410, 0x06000000, 0x04002418, 0xFF000000
.word func_80852080
.word 0x05000000, 0x04002390, 0xFF000000
.word func_808521F4
.word 0xFF000000
.word func_8085225C
.word 0xFF000000
.word func_80852280
.word 0x05000000, 0x040023A0, 0x05000000, 0x04002368, 0xFF000000
.word func_808515A4
.word 0x05000000, 0x04002370, 0x05000000, 0x040027B0, 0x05000000, 0x040027B8, 0x05000000, 0x040027C0, 0x03000000, 0x04002768, 0x03000000, 0x040027D8, 0x04000000, 0x040027E0, 0x03000000, 0x04002380, 0x03000000, 0x04002828, 0x06000000, 0x04002470, 0x06000000, 0x040032A8, 0x0E000000, 0x040032A0, 0x03000000, 0x040032A0, 0x05000000, 0x04002AE8, 0x10000000, 0x04002450, 0x0F000000, 0x04002460, 0x0F000000, 0x04002458, 0x03000000, 0x04002440, 0x03000000, 0x04002438, 0x03000000, 0x04002C88, 0x06000000, 0x04003450, 0x06000000, 0x04003448, 0x06000000, 0x04003460, 0x06000000, 0x04003440, 0x03000000, 0x04002798, 0x03000000, 0x04002818, 0x04000000, 0x04002848, 0x03000000, 0x04002850, 0x03000000, 0x040034E0, 0x03000000, 0x040034D8, 0x06000000, 0x040034C8, 0x03000000, 0x04003470, 0x03000000, 0x04003478, 0x03000000, 0x040034C0, 0x03000000, 0x04003480, 0x03000000, 0x04003490, 0x03000000, 0x04003488, 0x03000000, 0x04003498, 0x03000000, 0x040034B0, 0xFF000000
.word func_808524B0
.word 0x03000000, 0x04003420, 0xFF000000
.word func_80852544
.word 0xFF000000
.word func_80852564
.word 0x03000000, 0x04003250, 0xFF000000
.word func_80852608
.word 0x03000000, 0x04002810, 0x03000000, 0x04002838, 0x03000000, 0x04002CD0, 0x03000000, 0x04002CD8, 0x03000000, 0x04002868, 0x03000000, 0x040027E8, 0x03000000, 0x040027F8, 0x03000000, 0x04002800
glabel D_80854E50
 .word 0x00000000, 0x00000000, 0xFF000000
.word func_808514C0
.word 0xFF000000
.word func_8085157C
.word 0xFF000000
.word func_80851998
.word 0xFF000000
.word func_808519C0
.word 0x0B000000, 0x00000000, 0xFF000000
.word func_80852C50
.word 0xFF000000
.word func_80852944
.word 0xFF000000
.word func_80851688
.word 0xFF000000
.word func_80851750
.word 0xFF000000
.word func_80851828
.word 0xFF000000
.word func_808521B8
.word 0xFF000000
.word func_8085190C
.word 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x12000000
.word D_80854AF0
.word 0x0B000000, 0x00000000, 0xFF000000
.word func_80851A50
.word 0x0C000000, 0x04002730, 0x0B000000, 0x00000000, 0x00000000, 0x00000000, 0xFF000000
.word func_80851BE8
.word 0x0B000000, 0x00000000, 0xFF000000
.word func_80851CA4
.word 0x0B000000, 0x00000000, 0x11000000, 0x040030A8, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0xFF000000
.word func_80851D80
.word 0xFF000000
.word func_80851DEC
.word 0xFF000000
.word func_80851E28
.word 0x12000000
.word D_80854B00
.word 0xFF000000
.word func_808513BC
.word 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0xFF000000
.word func_80851ECC
.word 0xFF000000
.word func_80851FB0
.word 0xFF000000
.word func_80852048
.word 0xFF000000
.word func_80852174
.word 0x0D000000, 0x04002398, 0xFF000000
.word func_80852234
.word 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0B000000, 0x00000000, 0xFF000000
.word func_80852450
.word 0xFF000000
.word func_80851688
.word 0xFF000000
.word func_80852298
.word 0x0D000000, 0x040027D0, 0xFF000000
.word func_80852480
.word 0x0D000000, 0x040027C8, 0xFF000000
.word func_80852328
.word 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0C000000, 0x04002388, 0xFF000000
.word func_80852358
.word 0x0B000000, 0x00000000, 0x12000000
.word D_80854B14
.word 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0xFF000000
.word func_80852388
.word 0x11000000, 0x04002450, 0x0C000000, 0x04002448, 0x0C000000, 0x04002450, 0x0B000000, 0x00000000, 0xFF000000
.word func_808526EC
.word 0x11000000, 0x04003468, 0xFF000000
.word func_808526EC
.word 0x11000000, 0x04003468, 0x0C000000, 0x040027A0, 0x0C000000, 0x04002820, 0x0B000000, 0x00000000, 0x0C000000, 0x04002858, 0x0C000000, 0x040034D0, 0x0D000000, 0x040034F0, 0x0C000000, 0x040034E8, 0x0C000000, 0x040034A8, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0xFF000000
.word func_80852648
.word 0x0B000000, 0x00000000, 0x0C000000, 0x040034A0, 0xFF000000
.word func_808524D0
.word 0xFF000000
.word func_80852514
.word 0xFF000000
.word func_80852554
.word 0xFF000000
.word func_808525C0
.word 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0x0B000000, 0x00000000, 0xFF000000
.word func_8085283C
.word 0xFF000000
.word func_808528C8
.word 0xFF000000
.word func_808528C8
.word 0x0C000000, 0x04002870, 0x0C000000, 0x040027F0, 0x0C000000, 0x04002808, 0x0C000000, 0x04002450
glabel D_80855188
 .word 0x0000302A, 0x0000CFD0, 0x04002720, 0x04002360
glabel D_80855198
 .word 0xBF800000, 0x428C0000, 0x41A00000
glabel D_808551A4
 .word 0x182D0000, 0x18366800
glabel D_808551AC
 .word 0x0000401D, 0x0000BFD9
glabel D_808551B4
 .word 0x0000D7E2
glabel D_808551B8
 .word 0x1803F7F4
glabel D_808551BC
 .word 0x68112023, 0x083008EC, 0x0830F700
glabel D_808551C8
 .word 0x082A0843, 0x00004854, 0x0000485A, 0x0000B7A0
glabel D_808551D8
 .word 0x08501014, 0x0850EFE2
glabel D_808551E0
 .word 0x0000300A, 0x0000CFE8
glabel D_808551E8
 .word 0x0000400F, 0x0000BFDD
glabel D_808551F0
 .word 0x0000300F, 0x0000CFDF
glabel D_808551F8
 .word 0x0870F7B2
glabel D_808551FC
 .word func_8084377C
.word func_80843954
.word func_80843A38
glabel D_80855208
 .word 0x040034B8, 0x04003458
glabel D_80855210
 .word 0xFF3802BC, 0x00640320, 0x02580320, 0xFF3801F4, 0x00000258, 0x01900258
glabel D_80855228
 .word 0x00000000, 0x00000000, 0x00000000
glabel D_80855234
 .word 0xFFFFFF00
glabel D_80855238
 .word 0x00808000, 0x00000000

