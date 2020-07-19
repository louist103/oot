#include <ultra64.h>
#include <global.h>

#include <message_data_fmt.h>
#include <message_data_static.h>

#define DECLARE_MESSAGE_STAFF(textId, message) DECLARE_MESSAGE(textId, staff, message)

DECLARE_MESSAGE_STAFF(0x0500,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Producer/Supervisor\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SHIGERU MIYAMOTO" QUICKTEXT_DISABLE FADE2("\x00\x32")
);

DECLARE_MESSAGE_STAFF(0x0501,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x48") "Directors\n"
"\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x30") "Script Director\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x48") "TORU OSAWA" QUICKTEXT_DISABLE FADE2("\x00\x32")
);

DECLARE_MESSAGE_STAFF(0x0502,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Game System Directors\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YOICHI YAMADA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "EIJI ONOZUKA\n"
"\n"
"\n"
"\n"
SHIFT("\x24") "3D System Director/Character Design\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YOSHIAKI KOIZUMI" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0503,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Program Director\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "TOSHIO IWAWAKI\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Music Composer\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "KOJI KONDO" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x0504,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Cinema Scene Directors\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "TAKUMI KAWAGOE\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "NAOKI MORI\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Map Data Manager\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "SHIGEO KIMURA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0505,
QUICKTEXT_ENABLE SHIFT("\x24") SHIFT("\x36") "Programmers\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Main System\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "KENZO HAYAKAWA\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Display System\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "YASUNARI SOEJIMA\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Boss Enemy Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "KAZUAKI MORITA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0506,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Enemy Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MASUMI TARUKADO\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIROSHI UMEMIYA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MASARO SAKAKIBARA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0507,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SHIGEKI YOSHIDA \n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "TAKAMITSU KUZUHARA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SATORU TAKAHATA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0508,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "NOBUO OKAJIMA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "NOBUHIRO SUMIYOSHI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "ATSUSHI NISHIWAKI" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0509,
QUICKTEXT_ENABLE "\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Technical Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "KENJI MATSUTANI\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Tool Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YUICHI YAMAMOTO\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Camera Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MASATOSHI OGAWA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x050A,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Cinema Scene Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MAKOTO SASAKI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "KUNIHIRO KOMATSU\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Sound Effects Program\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YOJI INAGAKI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "TAKUYA MAEKAWA" QUICKTEXT_DISABLE FADE2("\x00\x85")
);

DECLARE_MESSAGE_STAFF(0x050B,
QUICKTEXT_ENABLE "\n"
"\n"
SHIFT("\x24") SHIFT("\x42") "Designers\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Character Design\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YOSHIKI HARUHANA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SATORU TAKIZAWA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "JIN IKEDA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SATOMI MAEKAWA" QUICKTEXT_DISABLE FADE2("\x00\x85")
);

DECLARE_MESSAGE_STAFF(0x050C,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Field Design\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MAKOTO MIYANAGA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIROMASA SHIKATA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIROMU TAKEMURA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x050D,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Dungeon Design\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "KENTA USUI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SHINICHI IKEMATSU\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "TAKESHI HOSONO" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x050E,
QUICKTEXT_ENABLE "\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Effects Design\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "TOMOAKI KUROUME\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Prerendered Background Design\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SHIGEKI YOSHIDA\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Item Design\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "REN UEHARA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x050F,
QUICKTEXT_ENABLE "\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "Cinema Scene Unit\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Assistant Director\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "DAIKI IWAMOTO\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Cinema Scene Animators\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIROSHI MATSUNAGA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "DAISUKE KAGEYAMA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0510,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "MOCAP System Directors\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SHINYA TAKAHASHI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "TSUYOSHI WATANABE" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x0511,
QUICKTEXT_ENABLE "\n"
SHIFT("\x24") SHIFT("\x48") "Actor Unit\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Character Voices\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "FUJIKO TAKIMOTO\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "NOBUYUKI HIYAMA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "JUN MIZUSAWA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0512,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Character Voices\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "TAKASHI NAGASAKO\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YAYOI JINGUJI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "KAORI MIZUHASHI" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0513,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
SHIFT("\x24") "  Motion Capture Actor\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x24") "YOSHINORI TSUJIMOTO (JAC)\n"
"\n"
"\n"
"\n"
SHIFT("\x24") "  Action Coordinator\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x24") "YUTA MOROKAJI (JAC)" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x0514,
QUICKTEXT_ENABLE "\n"
SHIFT("\x24") SHIFT("\x36") "Art Work Unit\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Character Illustration\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YUSUKE NAKANO\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Illustration Support\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "WATARU YAMAGUCHI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MINORU MAEDA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0515,
QUICKTEXT_ENABLE "Deleted" QUICKTEXT_DISABLE FADE2("\x00\x78")
);

DECLARE_MESSAGE_STAFF(0x0516,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "Support Unit\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Supervisors\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "TAKASHI TEZUKA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "TOSHIHIKO NAKAGO\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Schedule Management\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YOSHITAKA NISHIKAWA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0517,
QUICKTEXT_ENABLE "\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Script Support\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "KENSUKE TANABE\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "English Text Writer\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "DAN OWSEN\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "English Text Translation\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "HIRO YAMADA" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0518,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Technical Support\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIRONOBU KAKUI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIROHITO YOSHIMOTO\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YOSHINORI TANIMOTO" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x0519,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Technical Support\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIDEAKI SHIMIZU\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SHIN HASEGAWA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "YASUKI TAWARAISHI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "SHINGO OKAMOTO" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x051A,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Motion Editors\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIROSHI MOMOSE\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "RUI SHIMAMURA" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x051B,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "German Text Writer\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MARCUS MENOLD\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "THOMAS RINKE\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "CLAUDE MOYSE\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "CLUB NINTENDO REDAKTION" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x051C,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Progress Management\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "KIMIYOSHI FUKUI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "KEIZO KATO" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x051D,
QUICKTEXT_ENABLE "\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Special Thanks to\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x30") "MITSUHIRO HIKINO\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x30") "YOSHITAKA TAKESHITA\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x30") "GENTARO TAKAGI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x30") "ATSUSHI SAKAGUCHI\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x30") "HAJIME KITAGAWA" QUICKTEXT_DISABLE FADE2("\x00\xA6")
);

DECLARE_MESSAGE_STAFF(0x051E,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x18") "Special Thanks to\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x2A") "NINTENDO E.A.D\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x2A") "SUPER MARIO CLUB\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x2A") "NINTENDO OF AMERICA INC.\n"
"\n"
SHIFT("\x24") SHIFT("\x30") "DEBUGGING STAFF" QUICKTEXT_DISABLE FADE2("\x00\x64")
);

DECLARE_MESSAGE_STAFF(0x051F,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Executive Producer\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIROSHI YAMAUCHI" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x0520,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Special Thanks to\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "NINTENDO OF EUROPE GMBH\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "DEBUGGING STAFF\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "NINTENDO FRANCE\n"
"\n"
SHIFT("\x24") SHIFT("\x3C") "S.A.R.L STAFF" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x0521,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "Coordinator\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MITSUHIRO TAKANO\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "MASASHI GOTO\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "HIROYUKI UESUGI" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x0522,
QUICKTEXT_ENABLE "\n"
"\n"
"\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x12") "French Text Writer\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "JULIEN BARDAKOFF\n"
"\n"
"\n"
SHIFT("\x24") SHIFT("\x36") "FLEURY JEAN-BAPTISTE" QUICKTEXT_DISABLE FADE2("\x00\x42")
);

DECLARE_MESSAGE_STAFF(0x0523,
QUICKTEXT_ENABLE "Deleted" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x0524,
QUICKTEXT_ENABLE "24" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x0525,
QUICKTEXT_ENABLE "25" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x0526,
QUICKTEXT_ENABLE "26" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x0527,
QUICKTEXT_ENABLE "27" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x0528,
QUICKTEXT_ENABLE "28" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x0529,
QUICKTEXT_ENABLE "29" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x052A,
QUICKTEXT_ENABLE "2A" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x052B,
QUICKTEXT_ENABLE "2B" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x052C,
QUICKTEXT_ENABLE "2C" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x052D,
QUICKTEXT_ENABLE "2D" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x052E,
QUICKTEXT_ENABLE "2E" QUICKTEXT_DISABLE FADE2("\x00\x50")
);

DECLARE_MESSAGE_STAFF(0x052F,
QUICKTEXT_ENABLE "2F" QUICKTEXT_DISABLE FADE2("\x00\x50")
);
