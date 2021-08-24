#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>  /* printf FILE fopen fclose fprintf */
#include <string.h> /* strcat */
#if defined _MSC_VER
#include <Windows.h> /* GetUserNameA GetComputerNameA */
#include <lmcons.h>  /* UNLEN */
#else
#include <unistd.h> /* getpid getpwuid*/
#include <pwd.h>    /* passwd struct */
#endif

static char name[257];
static char name2[257];

int main(void) {
#if defined _MSC_VER
    DWORD nameLength = UNLEN;
    GetUserNameA(name, &nameLength);
    nameLength = UNLEN; // Microsoft thought it would be a good idea to edit this...
    GetComputerNameA(&name2[0], &nameLength);
#else
    struct passwd* p = getpwuid(getuid());
    strcat(name, p->pw_name);
    gethostname(name2, sizeof(name2));
#endif
    strcat(name, "@");
    strcat(name, name2);
    FILE* outFile = fopen("src/boot/build.c", "w+");

    fprintf(outFile, "const char gBuildTeam[] = \"%s\";\n", name);
    fprintf(outFile, "const char gBuildDate[] = __DATE__ \" \"__TIME__;\n");
    fprintf(outFile, "const char gBuildMakeOption[] = \"\";\n");
    fclose(outFile);
    return 0;
}
