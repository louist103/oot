#include "ultra64.h"
#include "global.h"

static UNK_TYPE D_80991680[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000005, 0x08000000, 0x00000000, 0x00000000, 0x00000524, 0x31090000,
    0x00000000, 0x00000000, 0x00041443, 0x4C120001, 0x03000000, 0x00000500, 0x072D5D88, 0x7C37100D, 0x12030000,
    0x00051808, 0x2479B9D7, 0xC37C4C3A, 0x2C060000, 0x00051A0D, 0x4ABEF1FC, 0xEEAE8D79, 0x4B0A0000, 0x00041612,
    0x68E0FEFF, 0xF5BBB8B6, 0x6D100000, 0x00031423, 0x84EEFFFF, 0xF4C4D1DB, 0x8A1D0300, 0x00041C3C, 0xA0F4FFFE,
    0xF1DAE7E9, 0x93200400, 0x00083154, 0xB6F7FFFA, 0xF4F7FBEA, 0x84140100, 0x000C4767, 0xC3F9FCEA, 0xEAFCFFED,
    0x7C0F0200, 0x000D4E77, 0xCCFAFBE5, 0xE3FAFFEE, 0x7D0F0200, 0x000A3F83, 0xD6FBFEF0, 0xE4F6FEF0, 0x84110200,
    0x00052688, 0xE5FDFDF0, 0xE6F9FFEF, 0x7F100200, 0x0002147E, 0xECFFFCE6, 0xDFFAFFEC, 0x710E0200, 0x00020C65,
    0xE2FEFAD9, 0xCFF7FFEA, 0x660B0200, 0x00020953, 0xD2FBFADA, 0xC9F5FFEB, 0x6B0D0200, 0x00020951, 0xCBFAFAE0,
    0xD4F6FFED, 0x750F0200, 0x00020C5C, 0xD4FAF1E2, 0xEBFCFFEB, 0x740F0100, 0x00010B60, 0xDEF8DDD7, 0xF6FFFEE9,
    0x690D0100, 0x00010B60, 0xE2F8D7CF, 0xF1FDFFE9, 0x5F0A0100, 0x00000C63, 0xDAFAE6D4, 0xE1F1FDE7, 0x5B0A0000,
    0x00000C5F, 0xC6F8F8E1, 0xDAE9FAD9, 0x52090000, 0x00000B59, 0xB4F0FBE0, 0xD1DDF4C0, 0x44070000, 0x00000A54,
    0xAFEFF3C6, 0xBBC0EDB0, 0x40070000, 0x00000B57, 0xB1E8DAA4, 0xA092DCA6, 0x45080000, 0x00000C5A, 0xAEDAB480,
    0x8C68BF92, 0x40080000, 0x00000C52, 0xA2C99264, 0x814FA06F, 0x23030000, 0x00010F48, 0x97BC7B4A, 0x75459760,
    0x0F000000, 0x00010E41, 0x95B46B36, 0x683E9663, 0x0D000000, 0x00000941, 0x9AAD602B, 0x5A328C64, 0x0E000000,
    0x00000841, 0x989E5524, 0x54347951, 0x0B000000, 0x0000073D, 0x88834C22, 0x4F3D6330, 0x07000000, 0x0000073B,
    0x73624726, 0x463E4B17, 0x0B010000, 0x00000941, 0x6748482C, 0x392A3413, 0x1C050000, 0x00000A46, 0x5D303B2E,
    0x3310211B, 0x2D070000, 0x00000941, 0x4B131B2C, 0x3F0D1821, 0x33080000, 0x0000093F, 0x4109092D, 0x490D0C20,
    0x32080000, 0x00000A43, 0x3F09062C, 0x450B061E, 0x2B070000, 0x00000B47, 0x410A0722, 0x3108081E, 0x20040000,
    0x00000838, 0x35090C19, 0x21050920, 0x20040000, 0x00000522, 0x1F060B0A, 0x1B04061E, 0x29060000, 0x00000313,
    0x0B020608, 0x28070319, 0x34090000, 0x0000020E, 0x03000108, 0x2F080214, 0x2F090000, 0x0000020D, 0x03000007,
    0x2407020C, 0x0B000000, 0x00000311, 0x06010001, 0x0D010413, 0x04000000, 0x0000051C, 0x0B010001, 0x0801020C,
    0x02000000, 0x00000623, 0x0D010003, 0x12030002, 0x00000000, 0x0000051D, 0x0A010005, 0x1B050000, 0x00000000,
    0x0000020C, 0x03000005, 0x18050000, 0x00000000, 0x00000002, 0x00000000, 0x05000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

static UNK_TYPE D_80991E80[] = {
    0x2F3E56AB, 0xA4582102, 0x10417AB0, 0x805C312A, 0x32376DD4, 0xA2633907, 0x0B387DA2, 0x6D81470C, 0x3C5C60AC,
    0xA5571801, 0x154C97B9, 0x68533236, 0x304276D6, 0x9B533108, 0x154A96B7, 0x7C9B400B, 0x3E756FAE, 0xAF5D1301,
    0x1D59B4BF, 0x53432C38, 0x2F487DCC, 0x8A4A2406, 0x1F5DB0C8, 0x90B55110, 0x408B7FAA, 0xB76D1503, 0x286ACCBE,
    0x43312030, 0x2B4C82BA, 0x77431607, 0x2B70C7D5, 0xA6B95B13, 0x409C93A6, 0xB87D1D08, 0x367FE1C1, 0x3C261629,
    0x2A508AAB, 0x69410C0C, 0x3986DDDB, 0xB5AE6316, 0x40A0A7A1, 0xAA7D2610, 0x4596F1CD, 0x47281E36, 0x365C97A3,
    0x63450713, 0x499DF0D8, 0xB49E721A, 0x409AB59F, 0x8A662D1C, 0x57AEFBDF, 0x6B404456, 0x536EA49B, 0x6A4D061E,
    0x5CB6FCC6, 0x9A8E7F20, 0x428EBEA0, 0x623F2E2A, 0x69C6FFF0, 0x9A686569, 0x6F83AC92, 0x7F5D0B2A, 0x70CDFFB2,
    0x77848D29, 0x4787C4A0, 0x401D2E3E, 0x7ED9FFFC, 0xC48F7973, 0x8598AE86, 0x9A701339, 0x85E1FFA6, 0x5E859534,
    0x5189C99B, 0x2C0B3455, 0x92E9FFFA, 0xDCAB8071, 0x94ACAD7A, 0xB1882249, 0x9AF1FFB0, 0x6A8F9746, 0x6394CD8D,
    0x290C416F, 0xA5EEFFF4, 0xDCB46B6D, 0xA1BFAD77, 0xB6A43A58, 0xAAF8FFC8, 0x84959963, 0x7BA4CE78, 0x351A588E,
    0xB5E3F9F1, 0xCFAC6077, 0xABCFAC77, 0xA8C05A6A, 0xB2F3FFE1, 0xA3959471, 0x94B9D065, 0x473274A9, 0xBDCDECEF,
    0xC2A05F86, 0xBADDAC78, 0x92D87C7D, 0xAFDEFDF1, 0xBC8C8971, 0xA6D2D45A, 0x584B95BB, 0xC1B3D5ED, 0xB8956697,
    0xC5E9A670, 0x7AE6978F, 0xA4BEF0EF, 0xB9797360, 0xB1E8DD5D, 0x5D5FAFC7, 0xC49CB8E3, 0xAF8C78A5, 0xCBF19D5F,
    0x6AE5A6A3, 0x9398DBE7, 0xB46A5841, 0xB8F2E871, 0x586DC1CD, 0xC78C9DD5, 0xAA878CB1, 0xD8F89249, 0x63DCABB3,
    0x8576C5D8, 0xA95E4430, 0xC0EBEE91, 0x5378CACE, 0xC58087C5, 0xA4859EAE, 0xE1FD8A36, 0x60D0ACBD, 0x7765B9C5,
    0x9C563A2B, 0xCED7E7B4, 0x5484C7CC, 0xB66F7CBB, 0x9F81A898, 0xE3FF8D2E, 0x63C8B0BA, 0x6D69B8B0, 0x93573B2E,
    0xE0C2CED2, 0x6898C0C3, 0x97587EBC, 0x9D749570, 0xDEFF9D36, 0x67C8B4A8, 0x6682BD9B, 0x93643E37, 0xF0B4A8DE,
    0x86A9B6B0, 0x70448BBE, 0x99617545, 0xD1FFB84E, 0x70CAB48C, 0x64A4BD8D, 0x96763D41, 0xFAAC82DA, 0xA7B6AE9A,
    0x4D359AC0, 0x90494F27, 0xC0FFD675, 0x80CCAD6D, 0x63C1B681, 0x9C85374E, 0xFFA069C9, 0xC1BAA785, 0x3936A4B6,
    0x7D342E18, 0xA9FFEEA5, 0x9AC59F56, 0x60D0A873, 0x9288315E, 0xFB8669B6, 0xD0B6A473, 0x3C47A39A, 0x6228241B,
    0x8BFFF6CD, 0xB7BB8F47, 0x57CFA169, 0x77782C6B, 0xE56478AA, 0xD6AC9E64, 0x4E61977C, 0x4B232428, 0x6FFFEBE5,
    0xD0AF813C, 0x44C2A266, 0x5B61296E, 0xC04188A8, 0xD4A09051, 0x5D788B61, 0x3E202234, 0x58F9D7DF, 0xDEA27332,
    0x31AE9A6C, 0x3E4B2F62, 0x902688B3, 0xD0937A3F, 0x5D7D8B58, 0x37241F35, 0x46F2C5BF, 0xD7956528, 0x1E918E76,
    0x3442424A, 0x601774C3, 0xCB8F6630, 0x48698261, 0x333C2729, 0x3CECBF95, 0xC2885420, 0x11717B78, 0x4952673A,
    0x3D1153CD, 0xC994602F, 0x2D496F6D, 0x31674418, 0x32E7CA6C, 0xA77D4318, 0x09526672, 0x6B6F963E, 0x2B1036D0,
    0xCAA26E3C, 0x1B2A5A7B, 0x3798700F, 0x2BDFD94D, 0x8C773712, 0x043C606C, 0x9086B955, 0x281225C9, 0xC7AD8954,
    0x141B4085, 0x53CB9C10, 0x26D3E63A, 0x757A3B11, 0x0131666F, 0xAB87BA6A, 0x2E151FBF, 0xC1AEA16A, 0x1721388C,
    0x81ECBB17, 0x23C1EE30, 0x6688561E, 0x02317381, 0xA36A9566, 0x3B171FB9, 0xB5A5AF7A, 0x1B384395, 0xABFBC81D,
    0x20AAEB28, 0x61987838, 0x05358299, 0x85405D4E, 0x45181FB5, 0xA695B083, 0x1C55569C, 0xCEFFCF1E, 0x1A8CDA24,
    0x62A39755, 0x0A398FAE, 0x601A2A2C, 0x49161FB1, 0x9A8BAC8A, 0x23736DA0, 0xE0FFD61E, 0x136BAF23, 0x70ABAD6D,
    0x143D95BD, 0x41030D16, 0x41151DAA, 0x938BAA94, 0x2E8A8BA7, 0xE6FFD921, 0x0C4A7D2D, 0x7CACAB70, 0x244093C3,
    0x38020F11, 0x2F151CA3, 0x9293ABA1, 0x3995AAB7, 0xEDF7D924, 0x072F5842, 0x83AE9C66, 0x3F498EC7, 0x420D2812,
    0x1E151F9B, 0x989CB0B2, 0x4697C5CE, 0xF5F0D626, 0x061B435B, 0x85B18A5B, 0x5F568BCD, 0x54245213, 0x12172593,
    0x9FA1BAC0, 0x5291D4E1, 0xFAF0D123, 0x0711416F, 0x82B2765B, 0x84668FCB, 0x684D8212, 0x1318308A, 0xA3A2C2C6,
    0x5F88D3EE, 0xFEF6C81B, 0x0B104276, 0x85B06A72, 0xA97896C3, 0x7C81AC19, 0x24194083, 0xA2A0C6C4, 0x6A82CCF1,
    0xFFFFB910, 0x14173E77, 0x8BAE6992, 0xC6879BB4, 0x92B3C933, 0x3C1D4F7D, 0x9DA2CCBE, 0x7381C6E6, 0xFFFFB111,
    0x26233975, 0x92AF6DAF, 0xDB949AA3, 0xADDBDD64, 0x5927597B, 0x99AAD4B9, 0x7687C6D5, 0xFCFFB736, 0x432D3A78,
    0x98B371BD, 0xE5A0989E, 0xCCF2EAA0, 0x753C5C80, 0x9DB9E3BC, 0x7896CFBF, 0xF6FFCB72, 0x64323F81, 0x9CBC70B4,
    0xE3AC9EAD, 0xE6FBF6CF, 0x8B5C5C85, 0xA7C8F4C2, 0x7AA6D9A3, 0xF0FFE2A9, 0x8032448D, 0xA4C970A0, 0xDBB5AAC6,
    0xF7FEFDE7, 0xA3806187, 0xB0D6FFC9, 0x7FB5D686, 0xE7FFF0D2, 0x8D2F4E98, 0xABD7768D, 0xCFB9B4E1, 0xF8F8FFE7,
    0xB29B7089, 0xB7E4FFCD, 0x8CB8BF65, 0xD5FFF0D7, 0x8D34609D, 0xB4E48385, 0xC3BAB0F1, 0xE2E8FEDC, 0xB5A07A8D,
    0xBDEFFFD0, 0x9AAB9543, 0xB9FFE9C3, 0x80447497, 0xC0EF9C90, 0xBDB997E5, 0xC2CCFBD4, 0xAB8F7387, 0xC8F8FFD4,
    0xA4936324, 0x98FFE5B3, 0x6F577E8A, 0xCEFABCA8, 0xC0BC71C7, 0x9EADF4D2, 0x916D5D7B, 0xD4FEFFD6, 0xA575390F,
    0x76FFE4A6, 0x656A7F7D, 0xDDFFDABF, 0xC8BF4CA1, 0x8095E8CC, 0x744A3B66, 0xDAFFFFD6, 0x9D5B1E04, 0x5DFDDF9A,
    0x6075787A, 0xECFFF1CD, 0xCEC42E7C, 0x7087D6BF, 0x5A311F49, 0xD4FFFFD0, 0x8E4B1100, 0x50F7D892, 0x68767788,
    0xF6FFFDCE, 0xCDC31E65, 0x6E88C3A8, 0x4922153A, 0xC3FFFDC4, 0x7F410B03, 0x4BEED08B, 0x747682A0, 0xFDFFFEBF,
    0xBCB61456, 0x7792B18B, 0x3E19153D, 0xB5FFF8B2, 0x733B0908, 0x4DE0C487, 0x827797BC, 0xFFFFF8AB, 0x9E9B0E50,
    0x869E9F6E, 0x3714184D, 0xB5FFF09E, 0x67380C11, 0x56CFB285, 0x8F7EADD4, 0xFFFFED96, 0x78730A53, 0x9CA98F57,
    0x310F1C69, 0xC8FFE18A, 0x5D35161D, 0x64BC9E84, 0x978BBFE7, 0xFFFFDD81, 0x5348075A, 0xB5B07F48, 0x290A2088,
    0xE1FFCF75, 0x5335272B, 0x74AD877F, 0x9C9DCFF3, 0xFFFEC86E, 0x35260864, 0xC6B16F3E, 0x200422A6, 0xF8FCB95F,
    0x47373A3C, 0x86A36F73, 0x9FB0D9FA, 0xFFF7B15B, 0x21110E70, 0xCDAB6139, 0x160027C1, 0xFFF09F4C, 0x3B3A4B4D,
    0x989D5B5D, 0x9DB5D9FE, 0xFFEA994C, 0x1509177B, 0xC8A15C3C, 0x0E022BD1, 0xFFE1873B, 0x2F3E5460, 0xA8954F44,
    0x96A6CEFF, 0xFFD7823C, 0x0C0A2784, 0xB9956343, 0x0B0731DB, 0xFFCE722C, 0x22415672, 0xB58C4A31, 0x8A88BAFF,
    0xFAC56E2E, 0x060F398F, 0xA989764A, 0x0A0C3AE1, 0xFFB85F20, 0x17425787, 0xBF824826, 0x775FA3FF, 0xF1B66121,
    0x03164B98, 0x9A818E4A, 0x0A1040D4, 0xE8974D14, 0x11405495, 0xBB74401F, 0x603D86F1, 0xD7A35414, 0x02195694,
    0x8476943E, 0x171A46C5, 0xCC7D3D0C, 0x0D3E599E, 0xAE6C371C, 0x4A2D72E3, 0xC38F4C0E, 0x03205F96, 0x766A792A,
    0x242B4FB8, 0xB66A2E06, 0x0D3E66AA, 0x9B663322, 0x3B2D6CDD, 0xB47B450A, 0x062B6E9C, 0x6E705D17,
};

static Vtx D_80992680[] = {
    VTX(405, 1572, -1643, 401, -3470, 0x00, 0x78, 0x00, 0xFF),
    VTX(470, 1488, -1799, 512, -3755, 0x00, 0x78, 0x00, 0xFF),
    VTX(207, 1312, -1162, 251, -3368, 0x00, 0x78, 0x00, 0xFF),
    VTX(207, 820, -1162, 61, -3381, 0x00, 0x78, 0x00, 0xFF),
    VTX(973, 820, -847, 512, -2788, 0x00, 0x78, 0x00, 0xFF),
    VTX(207, 1312, -1162, 252, -3368, 0x00, 0x78, 0x00, 0xFF),
    VTX(973, 1312, -847, 512, -3015, 0x00, 0x78, 0x00, 0xFF),
    VTX(1340, 1012, -209, 512, -455, 0x00, 0x78, 0x00, 0xFF),
    VTX(1340, 220, -209, 512, -1081, 0x00, 0x78, 0x00, 0xFF),
    VTX(939, 520, -209, 201, -1404, 0x00, 0x78, 0x00, 0xFF),
    VTX(939, 220, -209, 391, -1391, 0x00, 0x78, 0x00, 0xFF),
    VTX(1223, 177, 831, 0, 1707, 0x00, 0x78, 0x00, 0xFF),
    VTX(1309, 220, 831, 143, 1679, 0x00, 0x78, 0x00, 0xFF),
    VTX(1223, 500, 831, 0, 1365, 0x00, 0x78, 0x00, 0xFF),
    VTX(1871, 220, 831, 512, 1593, 0x00, 0x78, 0x00, 0xFF),
    VTX(973, 1312, -847, 512, -3015, 0x00, 0x78, 0x00, 0xFF),
    VTX(612, 1572, -1558, 512, -3300, 0x00, 0x78, 0x00, 0xFF),
    VTX(612, 1572, -1558, 512, -3300, 0x00, 0x78, 0x00, 0xFF),
    VTX(839, 1012, -209, 143, -626, 0x00, 0x78, 0x00, 0xFF),
    VTX(1602, 1272, 312, 512, 284, 0x00, 0x78, 0x00, 0xFF),
    VTX(839, 1176, 119, 0, -284, 0x00, 0x78, 0x00, 0xFF),
    VTX(953, 1272, 312, 0, 114, 0x00, 0x78, 0x00, 0xFF),
    VTX(1223, 1014, 831, 0, 853, 0x00, 0x78, 0x00, 0xFF),
    VTX(1871, 1014, 831, 512, 853, 0x00, 0x78, 0x00, 0xFF),
    VTX(691, 520, -209, 0, -1536, 0x00, 0x78, 0x00, 0xFF),
    VTX(839, 676, 119, 0, -967, 0x00, 0x78, 0x00, 0xFF),
    VTX(839, 520, -209, 127, -1452, 0x00, 0x78, 0x00, 0xFF),
    VTX(1905, 220, 1026, 396, 2481, 0x00, 0x78, 0x00, 0xFF),
    VTX(1658, 220, 1169, 101, 2487, 0x00, 0x78, 0x00, 0xFF),
    VTX(1905, 295, 1026, 410, 2617, 0x00, 0x78, 0x00, 0xFF),
    VTX(1658, 295, 1169, 102, 2617, 0x00, 0x78, 0x00, 0xFF),
    VTX(1915, 295, 1153, 410, 2901, 0x00, 0x78, 0x00, 0xFF),
};

static Vtx D_80992880[] = {
    VTX(1905, 295, 1026, 410, 2617, 0x00, 0x78, 0x00, 0xFF),  VTX(1940, 295, 1047, 512, 2674, 0x00, 0x78, 0x00, 0xFF),
    VTX(1905, 220, 1026, 396, 2481, 0x00, 0x78, 0x00, 0xFF),  VTX(1940, 220, 1047, 512, 2560, 0x00, 0x78, 0x00, 0xFF),
    VTX(1658, 220, 1169, 101, 2487, 0x00, 0x78, 0x00, 0xFF),  VTX(1658, 220, 1215, 0, 2560, 0x00, 0x78, 0x00, 0xFF),
    VTX(1658, 295, 1169, 102, 2617, 0x00, 0x78, 0x00, 0xFF),  VTX(1658, 295, 1215, 0, 2674, 0x00, 0x78, 0x00, 0xFF),
    VTX(1968, 295, 1172, 512, 2958, 0x00, 0x78, 0x00, 0xFF),  VTX(1915, 295, 1153, 410, 2901, 0x00, 0x78, 0x00, 0xFF),
    VTX(1763, 295, 1241, 102, 2901, 0x00, 0x78, 0x00, 0xFF),  VTX(1753, 295, 1295, 0, 2958, 0x00, 0x78, 0x00, 0xFF),
    VTX(973, 820, -847, 512, -2788, 0x00, 0x78, 0x00, 0xFF),  VTX(207, 820, -1162, 61, -3381, 0x00, 0x78, 0x00, 0xFF),
    VTX(973, 520, -847, 512, -2389, 0x00, 0x78, 0x00, 0xFF),  VTX(142, 520, -1189, 0, -3015, 0x00, 0x78, 0x00, 0xFF),
    VTX(1116, 520, -638, 512, -2105, 0x00, 0x78, 0x00, 0xFF), VTX(939, 520, -209, 201, -1404, 0x00, 0x78, 0x00, 0xFF),
    VTX(1116, 220, -638, 512, -1764, 0x00, 0x78, 0x00, 0xFF), VTX(939, 220, -209, 391, -1391, 0x00, 0x78, 0x00, 0xFF),
    VTX(1309, 220, 831, 143, 1679, 0x00, 0x78, 0x00, 0xFF),   VTX(1907, 220, 920, 512, 2332, 0x00, 0x78, 0x00, 0xFF),
    VTX(1871, 220, 831, 512, 1593, 0x00, 0x78, 0x00, 0xFF),   VTX(1223, 177, 831, 0, 1707, 0x00, 0x78, 0x00, 0xFF),
    VTX(1309, 220, 910, 0, 1820, 0x00, 0x78, 0x00, 0xFF),     VTX(1932, 220, 1015, 512, 2503, 0x00, 0x78, 0x00, 0xFF),
    VTX(1340, 220, -209, 512, -1081, 0x00, 0x78, 0x00, 0xFF), VTX(1636, 220, 1194, 0, 2503, 0x00, 0x78, 0x00, 0xFF),
    VTX(691, 520, -209, 0, -1536, 0x00, 0x78, 0x00, 0xFF),    VTX(839, 520, -209, 127, -1452, 0x00, 0x78, 0x00, 0xFF),
    VTX(1915, -200, 1153, 410, 4096, 0x00, 0x78, 0x00, 0xFF), VTX(1968, -200, 1172, 512, 4096, 0x00, 0x78, 0x00, 0xFF),
};

static Vtx D_80992A80[] = {
    VTX(1915, 295, 1153, 410, 2901, 0x00, 0x78, 0x00, 0xFF),
    VTX(1968, -200, 1172, 512, 4096, 0x00, 0x78, 0x00, 0xFF),
    VTX(1968, 295, 1172, 512, 2958, 0x00, 0x78, 0x00, 0xFF),
    VTX(1763, 295, 1241, 102, 2901, 0x00, 0x78, 0x00, 0xFF),
    VTX(1763, -200, 1241, 102, 4096, 0x00, 0x78, 0x00, 0xFF),
    VTX(1915, -200, 1153, 410, 4096, 0x00, 0x78, 0x00, 0xFF),
    VTX(1753, 295, 1295, 0, 2958, 0x00, 0x78, 0x00, 0xFF),
    VTX(1753, -200, 1295, 0, 4096, 0x00, 0x78, 0x00, 0xFF),
    VTX(1340, 1012, -209, 512, -455, 0x00, 0x78, 0x00, 0xFF),
    VTX(939, 520, -209, 201, -1404, 0x00, 0x78, 0x00, 0xFF),
    VTX(839, 1012, -209, 143, -626, 0x00, 0x78, 0x00, 0xFF),
    VTX(839, 520, -209, 127, -1452, 0x00, 0x78, 0x00, 0xFF),
    VTX(839, 676, 119, 0, -967, 0x00, 0x78, 0x00, 0xFF),
    VTX(839, 1176, 119, 0, -284, 0x00, 0x78, 0x00, 0xFF),
    VTX(1116, 520, -638, 512, -2105, 0x00, 0x78, 0x00, 0xFF),
    VTX(469, 820, -1799, 512, -4096, 0x00, 0x78, 0x00, 0xFF),
    VTX(207, 820, -1162, 61, -3381, 0x00, 0x78, 0x00, 0xFF),
    VTX(207, 1312, -1162, 251, -3368, 0x00, 0x78, 0x00, 0xFF),
    VTX(470, 1488, -1799, 512, -3755, 0x00, 0x78, 0x00, 0xFF),
    VTX(1223, 1014, 831, 0, 853, 0x00, 0x78, 0x00, 0xFF),
    VTX(1223, 500, 831, 0, 1365, 0x00, 0x78, 0x00, 0xFF),
    VTX(1871, 1014, 831, 512, 853, 0x00, 0x78, 0x00, 0xFF),
    VTX(1871, 220, 831, 512, 1593, 0x00, 0x78, 0x00, 0xFF),
    VTX(142, 520, -1189, 0, -3015, 0x00, 0x78, 0x00, 0xFF),
    VTX(142, 820, -1189, 0, -3413, 0x00, 0x78, 0x00, 0xFF),
    VTX(-76, 820, -1633, 0, -4096, 0x00, 0x78, 0x00, 0xFF),
};

static Vtx D_80992C20[] = {
    VTX(1658, 295, 1169, 205, 1308, 0x00, 0x78, 0x00, 0xFF),
    VTX(1763, 295, 1241, 205, 1451, 0x00, 0x78, 0x00, 0xFF),
    VTX(1915, 295, 1153, 819, 1451, 0x00, 0x78, 0x00, 0xFF),
};

static Vtx D_80992C50[] = {
    VTX(-389, 800, -1884, 102, 4096, 0x2E, 0x00, 0x92, 0xFF),
    VTX(-989, 800, -2131, 512, 4096, 0x2E, 0x00, 0x92, 0xFF),
    VTX(-389, 1312, -1884, 102, 3589, 0x2B, 0x29, 0x98, 0xFF),
    VTX(-989, 1312, -2131, 512, 3183, 0x1C, 0x5E, 0xBC, 0xFF),
    VTX(-1491, 1521, -1887, 512, 2676, 0x14, 0x6B, 0xCF, 0xFF),
    VTX(-1526, 1573, -1788, 432, 2534, 0x07, 0x76, 0xEE, 0xFF),
    VTX(-960, 1573, -1555, 0, 2929, 0x07, 0x76, 0xEE, 0xFF),
    VTX(-431, 1368, -1782, 0, 3487, 0x14, 0x6B, 0xCF, 0xFF),
    VTX(-3551, 128, 2017, 512, -8687, 0x00, 0x78, 0x00, 0xFF),
    VTX(-3362, 110, 1813, 512, -9143, 0x00, 0x78, 0x00, 0xFF),
    VTX(-3663, 171, 1333, 0, -9143, 0x00, 0x78, 0x00, 0xFF),
    VTX(-2482, 20, 1094, 0, -10005, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4018, 201, 1769, 0, -8687, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4183, 210, 2614, 0, -8129, 0x00, 0x78, 0x00, 0xFF),
    VTX(-3596, 132, 2353, 512, -8129, 0x00, 0x78, 0x00, 0xFF),
    VTX(-2655, 20, 1678, 508, -9752, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4574, 262, 2787, 0, -4832, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4684, 277, 2678, 0, -4679, 0x00, 0x78, 0x00, 0xFF),
    VTX(-5168, 341, 2737, 512, -4527, 0x00, 0x78, 0x00, 0xFF),
    VTX(-5194, 351, 2079, 512, -3868, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4997, 318, 2975, 512, -4832, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4710, 283, 1899, 0, -4121, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4653, 291, 1728, 0, -3868, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1962, 20, 988, 0, -10462, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1824, 20, 1516, 512, -10462, 0x00, 0x78, 0x00, 0xFF),
    VTX(-116, 20, 907, 0, -12034, 0x00, 0x78, 0x00, 0xFF),
    VTX(-253, 20, 908, 0, -11882, 0x00, 0x78, 0x00, 0xFF),
    VTX(-261, 20, 1460, 512, -11882, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1189, 20, 1466, 512, -11121, 0x00, 0x78, 0x00, 0xFF),
    VTX(105, 20, 905, 0, -12288, 0x00, 0x78, 0x00, 0xFF),
    VTX(-42, 20, 906, 0, -12136, 0x00, 0x78, 0x00, 0xFF),
    VTX(112, 20, 1211, 256, -12288, 0x00, 0x78, 0x00, 0xFF),
};

static Vtx D_80992E50[] = {
    VTX(-1481, 1314, -1874, 512, 2676, 0x4D, 0x56, 0x1F, 0xFF),
    VTX(-2157, 1810, -1555, 512, 1661, 0x49, 0x56, 0x28, 0xFF),
    VTX(-1708, 1312, -1303, 181, 2088, 0x47, 0x53, 0x31, 0xFF),
    VTX(-4127, 2148, -205, 512, -774, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-4859, 1836, 734, 512, -1675, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-4433, 1483, 1025, 0, -1940, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-5178, 1470, 1540, 512, -2352, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-5210, 1260, 1920, 512, -2803, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-5196, 1149, 2080, 512, -2955, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-4655, 1124, 1729, 0, -2955, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-2443, 1728, -884, 0, 900, 0x49, 0x55, 0x2A, 0xFF),
    VTX(-1743, 1282, -1195, 0, 1813, 0x65, 0x2F, 0x2C, 0xFF),
    VTX(-1601, 520, -1259, 0, 2168, 0xD2, 0x00, 0x6E, 0xFF),
    VTX(-1601, 1312, -1259, 0, 2371, 0xD5, 0x29, 0x68, 0xFF),
    VTX(-1708, 520, -1303, 86, 2090, 0xD2, 0x00, 0x6E, 0xFF),
    VTX(-1708, 1312, -1303, 181, 2088, 0xD2, 0x00, 0x6E, 0xFF),
    VTX(-1708, 520, -1303, 86, 2090, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1743, 520, -1195, 0, 2016, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1601, 520, -1259, 0, 2168, 0x00, 0x78, 0x00, 0xFF),
    VTX(-960, 1573, -1555, 0, 2929, 0x07, 0x76, 0xEE, 0xFF),
    VTX(-1526, 1573, -1788, 432, 2534, 0x07, 0x76, 0xEE, 0xFF),
    VTX(-431, 800, -1782, 0, 4096, 0x6E, 0x00, 0x2E, 0xFF),
    VTX(-389, 1312, -1884, 102, 3589, 0x6E, 0x00, 0x2E, 0xFF),
    VTX(-431, 1368, -1782, 0, 3487, 0x6E, 0x00, 0x2E, 0xFF),
    VTX(-389, 800, -1884, 102, 4096, 0x6E, 0x00, 0x2E, 0xFF),
    VTX(-1708, 520, -1303, 86, 2090, 0x72, 0x00, 0x25, 0xFF),
    VTX(-1743, 520, -1195, 0, 2016, 0x72, 0x00, 0x25, 0xFF),
    VTX(-2793, 2196, -1235, 512, 748, 0x3B, 0x5A, 0x34, 0xFF),
    VTX(-2584, 1794, -774, 0, 697, 0x3B, 0x5A, 0x34, 0xFF),
    VTX(-2717, 2160, -1291, 512, 850, 0x48, 0x55, 0x2A, 0xFF),
};

static Vtx D_80993030[] = {
    VTX(-5196, 1149, 2080, 512, -2955, 0x00, 0x78, 0x00, 0xFF),
    VTX(-5194, 351, 2079, 512, -3868, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4653, 291, 1728, 0, -3868, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4655, 1124, 1729, 0, -2955, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4574, 262, 2787, 0, -4832, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4997, 318, 2975, 512, -4832, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4997, 1136, 2975, 512, -5795, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4574, 1112, 2787, 0, -5795, 0x00, 0x78, 0x00, 0xFF),
    VTX(-3596, 1057, 2353, 512, -7266, 0x00, 0x78, 0x00, 0xFF),
    VTX(-3596, 132, 2353, 512, -8129, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4183, 210, 2614, 0, -8129, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4183, 1090, 2614, 0, -7266, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1189, 20, 1466, 512, -11121, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1194, 20, 914, 0, -11071, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1577, 20, 1468, 512, -10716, 0x00, 0x78, 0x00, 0xFF),
    VTX(-253, 20, 908, 0, -11882, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1610, 20, 917, 0, -10766, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1962, 20, 988, 0, -10462, 0x00, 0x78, 0x00, 0xFF),
    VTX(-1824, 20, 1516, 512, -10462, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4433, 1483, 1025, 0, -1940, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-3678, 1748, 81, 0, -621, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-4127, 2148, -205, 512, -774, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-3641, 2217, -657, 512, -114, 0x2C, 0x5D, 0x3D, 0xFF),
    VTX(-2584, 1794, -774, 0, 697, 0x3B, 0x5A, 0x34, 0xFF),
    VTX(-2584, 1794, -774, 0, 697, 0x3B, 0x5A, 0x34, 0xFF),
    VTX(-2793, 2196, -1235, 512, 748, 0x3B, 0x5A, 0x34, 0xFF),
    VTX(-4997, 1136, 2975, 512, -5795, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4876, 1279, 3143, 512, -6089, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4574, 1112, 2787, 0, -5795, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4445, 1191, 2858, 0, -6163, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4488, 1519, 3360, 512, -6384, 0x00, 0x78, 0x00, 0xFF),
};

static Vtx D_80993220[] = {
    VTX(-4488, 1519, 3360, 512, -6384, 0x00, 0x78, 0x00, 0xFF),
    VTX(-3980, 1539, 3206, 512, -6678, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4281, 1193, 2801, 0, -6531, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4445, 1191, 2858, 0, -6163, 0x00, 0x78, 0x00, 0xFF),
    VTX(-3643, 1337, 2785, 512, -6972, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4203, 1151, 2711, 0, -6899, 0x00, 0x78, 0x00, 0xFF),
    VTX(-3596, 1057, 2353, 512, -7266, 0x00, 0x78, 0x00, 0xFF),
    VTX(-4183, 1090, 2614, 0, -7266, 0x00, 0x78, 0x00, 0xFF),
};

static Vtx D_809932A0[] = {
    VTX(117, 20, 1458, 1024, -6144, 0x00, 0x78, 0x00, 0xFF),
    VTX(112, 20, 1211, 512, -6144, 0x00, 0x78, 0x00, 0xFF),
    VTX(-261, 20, 1460, 1024, -5941, 0x00, 0x78, 0x00, 0xFF),
};

static Gfx D_809932D0[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(65535, 65535, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(D_80991680, G_IM_FMT_I, G_IM_SIZ_8b, 16, 128, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                         G_TX_NOMIRROR | G_TX_WRAP, 4, 7, G_TX_NOLOD, 1),
    gsDPLoadMultiBlock(D_80991E80, 0x0100, 1, G_IM_FMT_I, G_IM_SIZ_8b, 32, 64, 0, G_TX_NOMIRROR | G_TX_WRAP,
                       G_TX_NOMIRROR | G_TX_WRAP, 5, 6, G_TX_NOLOD, 15),
    gsDPSetCombineLERP(NOISE, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL1, 1, ENVIRONMENT, TEXEL0, 0, 0, 0, COMBINED, 0,
                       0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_FOG | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_LIGHTING),
    gsDPSetPrimColor(0, 0, 0x00, 0x32, 0x64, 0xFF),
    gsDPSetEnvColor(0x32, 0x14, 0x00, 0x80),
    gsSPEndDisplayList(),
};

static Gfx D_80993390[] = {
    gsSPVertex(D_80992680, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(4, 6, 5, 0, 7, 8, 9, 0),
    gsSP2Triangles(8, 10, 9, 0, 11, 12, 13, 0),
    gsSP2Triangles(12, 14, 13, 0, 15, 16, 2, 0),
    gsSP2Triangles(16, 0, 2, 0, 17, 1, 0, 0),
    gsSP2Triangles(7, 18, 19, 0, 18, 20, 19, 0),
    gsSP2Triangles(20, 21, 19, 0, 22, 23, 21, 0),
    gsSP2Triangles(23, 19, 21, 0, 24, 25, 26, 0),
    gsSP2Triangles(27, 28, 29, 0, 28, 30, 29, 0),
    gsSP2Triangles(29, 30, 31, 0, 26, 25, 18, 0),
    gsSPVertex(D_80992880, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 5, 7, 6, 0),
    gsSP2Triangles(1, 0, 8, 0, 0, 9, 8, 0),
    gsSP2Triangles(6, 7, 10, 0, 7, 11, 10, 0),
    gsSP2Triangles(12, 13, 14, 0, 13, 15, 14, 0),
    gsSP2Triangles(16, 17, 18, 0, 17, 19, 18, 0),
    gsSP2Triangles(4, 2, 20, 0, 2, 21, 20, 0),
    gsSP2Triangles(21, 22, 20, 0, 23, 24, 20, 0),
    gsSP2Triangles(25, 21, 2, 0, 3, 25, 2, 0),
    gsSP2Triangles(26, 18, 19, 0, 24, 27, 20, 0),
    gsSP2Triangles(27, 4, 20, 0, 27, 5, 4, 0),
    gsSP2Triangles(28, 29, 15, 0, 29, 16, 15, 0),
    gsSP2Triangles(16, 14, 15, 0, 9, 30, 31, 0),
    gsSPVertex(D_80992A80, 26, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 5, 0, 0, 6, 7, 4, 0),
    gsSP2Triangles(6, 4, 3, 0, 8, 9, 10, 0),
    gsSP2Triangles(9, 11, 10, 0, 10, 12, 13, 0),
    gsSP2Triangles(14, 11, 9, 0, 15, 16, 17, 0),
    gsSP2Triangles(15, 17, 18, 0, 19, 20, 21, 0),
    gsSP2Triangles(20, 22, 21, 0, 23, 16, 24, 0),
    gsSP2Triangles(15, 25, 16, 0, 25, 24, 16, 0),
    gsSPVertex(D_80992C20, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};

static Gfx D_809934B8[] = {
    gsSPVertex(D_80992C50, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP2Triangles(4, 5, 3, 0, 5, 6, 3, 0),
    gsSP2Triangles(6, 7, 3, 0, 7, 2, 3, 0),
    gsSP2Triangles(8, 9, 10, 0, 9, 11, 10, 0),
    gsSP2Triangles(10, 12, 8, 0, 12, 13, 8, 0),
    gsSP2Triangles(13, 14, 8, 0, 9, 15, 11, 0),
    gsSP2Triangles(16, 17, 18, 0, 17, 19, 18, 0),
    gsSP2Triangles(18, 20, 16, 0, 17, 21, 19, 0),
    gsSP2Triangles(21, 22, 19, 0, 23, 11, 24, 0),
    gsSP2Triangles(11, 15, 24, 0, 25, 26, 27, 0),
    gsSP2Triangles(26, 28, 27, 0, 29, 30, 31, 0),
    gsSP2Triangles(31, 30, 27, 0, 30, 25, 27, 0),
    gsSPVertex(D_80992E50, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(4, 6, 5, 0, 6, 7, 5, 0),
    gsSP2Triangles(7, 8, 5, 0, 8, 9, 5, 0),
    gsSP2Triangles(10, 11, 1, 0, 11, 2, 1, 0),
    gsSP2Triangles(12, 13, 14, 0, 13, 15, 14, 0),
    gsSP2Triangles(16, 17, 18, 0, 2, 13, 19, 0),
    gsSP2Triangles(2, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(21, 24, 22, 0, 11, 25, 2, 0),
    gsSP2Triangles(11, 26, 25, 0, 27, 28, 29, 0),
    gsSP2Triangles(28, 10, 29, 0, 10, 1, 29, 0),
    gsSPVertex(D_80993030, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 15, 13, 0),
    gsSP2Triangles(13, 16, 14, 0, 16, 17, 14, 0),
    gsSP2Triangles(17, 18, 14, 0, 19, 20, 21, 0),
    gsSP2Triangles(20, 22, 21, 0, 20, 23, 22, 0),
    gsSP2Triangles(24, 25, 22, 0, 26, 27, 28, 0),
    gsSP2Triangles(27, 29, 28, 0, 27, 30, 29, 0),
    gsSPVertex(D_80993220, 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(1, 4, 5, 0, 1, 5, 2, 0),
    gsSP2Triangles(4, 6, 7, 0, 4, 7, 5, 0),
    gsSPVertex(D_809932A0, 3, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSPEndDisplayList(),
};