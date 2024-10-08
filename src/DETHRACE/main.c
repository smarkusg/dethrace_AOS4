
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#include <windows.h>
#endif

#include "brender.h"

#ifdef __AMIGAOS4__
extern void AmigOS_tooltype2(int argc, char *argv[]);
#ifndef DETHRACE_VERSION
#define DETHRACE_VERSION "dethrace Port by BSzili compiling for AOS4 smarkusg (20.09.2024)"
#endif
static const char *__attribute__((used)) stackcookie = "$STACK: 5000000";
static const char *__attribute__((used)) version_tag = "$VER: " DETHRACE_VERSION;
#endif //AOS4

extern void Harness_Init(int* argc, char* argv[]);
extern int original_main(int pArgc, char* pArgv[]);

void BR_CALLBACK _BrBeginHook(void) {
    struct br_device* BR_EXPORT BrDrv1SoftPrimBegin(char* arguments);
    struct br_device* BR_EXPORT BrDrv1SoftRendBegin(char* arguments);

    BrDevAddStatic(NULL, BrDrv1SoftPrimBegin, NULL);
    BrDevAddStatic(NULL, BrDrv1SoftRendBegin, NULL);
    // BrDevAddStatic(NULL, BrDrv1SDL2Begin, NULL);
}

void BR_CALLBACK _BrEndHook(void) {
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    /* Attach to the console that started us if any */
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        /* We attached successfully, lets redirect IO to the consoles handles if not already redirected */
        if (_fileno(stdout) == -2 || _get_osfhandle(_fileno(stdout)) == -2) {
            freopen("CONOUT$", "w", stdout);
        }

        if (_fileno(stderr) == -2 || _get_osfhandle(_fileno(stderr)) == -2) {
            freopen("CONOUT$", "w", stderr);
        }

        if (_fileno(stdin) == -2 || _get_osfhandle(_fileno(stdin)) == -2) {
            freopen("CONIN$", "r", stdin);
        }
    }
#endif

#ifdef __AMIGAOS4__
    AmigOS_tooltype2(argc, argv);
#endif

    Harness_Init(&argc, argv);

    return original_main(argc, argv);

}
