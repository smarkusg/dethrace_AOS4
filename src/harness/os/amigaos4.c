#define _GNU_SOURCE
#include "harness/config.h"
#include "harness/os.h"
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>


static char _program_name[1024];
static char name_buf[4096];

/*
void resolve_full_path(char* path, const char* argv0) {
    return;
}
*/

void resolve_full_path(char* path, const char* argv0) {
    if (argv0[0] == '/') { // run with absolute path
        strcpy(path, argv0);
    } else { // run with relative path
        if (NULL == getcwd(path, PATH_MAX)) {
            perror("getcwd error");
            return;
        }
        strcat(path, "/");
        strcat(path, argv0);
    }
}

/*
FILE* OS_fopen(const char* pathname, const char* mode) {
    FILE* f = fopen(pathname, mode);
    if (f != NULL) {
        return f;
    }
    return NULL;
}
*/

FILE* OS_fopen(const char* pathname, const char* mode) {
    FILE* f = fopen(pathname, mode);
    if (f != NULL) {
        return f;
    }
    char buffer[512];
    char buffer2[512];
    strcpy(buffer, pathname);
    strcpy(buffer2, pathname);
    char* pDirName = dirname(buffer);
    char* pBaseName = basename(buffer2);
    DIR* pDir = opendir(pDirName);
    if (pDir == NULL) {
        return NULL;
    }
    for (struct dirent* pDirent = readdir(pDir); pDirent != NULL; pDirent = readdir(pDir)) {
        if (strcasecmp(pBaseName, pDirent->d_name) == 0) {
            strcat(pDirName, "/");
            strcat(pDirName, pDirent->d_name);
            f = fopen(pDirName, mode);
            break;
        }
    }
    closedir(pDir);
    if (harness_game_config.verbose) {
        if (f == NULL) {
            fprintf(stderr, "Failed to open \"%s\" (%s)\n", pathname, strerror(errno));
        }
    }
    return f;
}
/*
size_t OS_ConsoleReadPassword(char* pBuffer, size_t pBufferLen) {
    return 0;
}
*/

size_t OS_ConsoleReadPassword(char* pBuffer, size_t pBufferLen) {
    struct termios old, new;
    char c;
    size_t len;

    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);

    len = 0;
    pBuffer[len] = '\0';
    while (1) {
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 0x7f) {
                if (len > 0) {
                    len--;
                    pBuffer[len] = '\0';
                    printf("\033[1D \033[1D");
                    fflush(stdout);
                    continue;
                }
            } else if (c == '\r' || c == '\n') {
                printf("\n");
                fflush(stdout);
                break;
            } else if (len < pBufferLen - 1) {
                if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    pBuffer[len] = c;
                    printf("*");
                    fflush(stdout);
                    len++;
                    pBuffer[len] = '\0';
                }
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return len;
}

/*
char* OS_Basename(const char* path) {
    return "";
}

char* OS_GetWorkingDirectory(char* argv0) {
    return "";
}
*/


char* OS_Dirname(const char* path) {
    strcpy(name_buf, path);
    return dirname(name_buf);
}

char* OS_Basename(const char* path) {
    strcpy(name_buf, path);
    return basename(name_buf);
}

char* OS_GetWorkingDirectory(char* argv0) {
    return OS_Dirname(argv0);
}

void OS_InstallSignalHandler(char* program_name) {
    resolve_full_path(_program_name, program_name);
}
