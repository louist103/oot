#ifdef __GNUC__
const char gBuildTeam[] = "X5";
const char gBuildDate[] = __DATE__ " " __TIME__;
const char gBuildMakeOption[] = "";
#else
const char gBuildTeam[] = "X5";
const char gBuildDate[] = "IDO DOESNT SUPPORT DATES";
const char gBuildMakeOption[] = "";
#endif
