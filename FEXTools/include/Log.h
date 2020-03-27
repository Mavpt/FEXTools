/* Log */

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

// Colors using ANSI
#define BlackPrint(X, ...)   printf("\x1b[30m" X "\x1b[0m", __VA_ARGS__)
#define RedPrint(X, ...)     printf("\x1b[31m" X "\x1b[0m", __VA_ARGS__)
#define GreenPrint(X, ...)   printf("\x1b[32m" X "\x1b[0m", __VA_ARGS__)
#define YellowPrint(X, ...)  printf("\x1b[33m" X "\x1b[0m", __VA_ARGS__)
#define BluePrint(X, ...)    printf("\x1b[34m" X "\x1b[0m", __VA_ARGS__)
#define MagentaPrint(X, ...) printf("\x1b[35m" X "\x1b[0m", __VA_ARGS__)
#define CyanPrint(X, ...)    printf("\x1b[36m" X "\x1b[0m", __VA_ARGS__)
#define WhitePrint(X, ...)   printf("\x1b[37m" X "\x1b[0m", __VA_ARGS__)

#define BrightBlackPrint(X, ...)   printf("\x1b[90m" X "\x1b[0m", __VA_ARGS__)
#define BrightRedPrint(X, ...)     printf("\x1b[91m" X "\x1b[0m", __VA_ARGS__)
#define BrightGreenPrint(X, ...)   printf("\x1b[92m" X "\x1b[0m", __VA_ARGS__)
#define BrightYellowPrint(X, ...)  printf("\x1b[93m" X "\x1b[0m", __VA_ARGS__)
#define BrightBluePrint(X, ...)    printf("\x1b[94m" X "\x1b[0m", __VA_ARGS__)
#define BrightMagentaPrint(X, ...) printf("\x1b[95m" X "\x1b[0m", __VA_ARGS__)
#define BrightCyanPrint(X, ...)    printf("\x1b[96m" X "\x1b[0m", __VA_ARGS__)
#define BrightWhitePrint(X, ...)   printf("\x1b[97m" X "\x1b[0m", __VA_ARGS__)

// Erros
#define CORE_ASSERT(x, ...)                                       \
    if (!(x))                                                     \
    {                                                             \
        RedPrint("\n\nCORE ERROR: %s:%d : ", __FILE__, __LINE__); \
        RedPrint(__VA_ARGS__);                                    \
        RedPrint("\n\n");                                         \
        exit(1);                                                  \
    }

#define CLIENT_ASSERT(x, ...)                                   \
    if (!(x))                                                   \
    {                                                           \
        printf("\n\nCORE ERROR: %s:%d : ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                                    \
        printf("\n\n");                                         \
        exit(1);                                                \
    }

#define FSTREAMTEST(STREAM, FILEPATH)                                                                 \
    if (!(STREAM))                                                                                    \
    {                                                                                                 \
        BrightCyanPrint("\n\nERROR: There was an error opening the following file %s\n\n", FILEPATH); \
        exit(2);                                                                                      \
    }

#define PROPERTYTEST(POS, PROPERTY, CONTENT)                                                             \
    if (POS == std::string::npos)                                                                        \
    {                                                                                                    \
        BrightYellowPrint("\n\nERROR: The %s property wasn't found in:\n%s", PROPERTY, CONTENT.c_str()); \
        exit(3);                                                                                         \
    }

#endif