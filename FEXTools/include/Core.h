/* Core */

#ifndef __CORE_H__
#define __CORE_H__

#include <stdio.h>
#include <iomanip>

#define ASSERT(x, ...)                                  \
    if (!(x))                                           \
    {                                                   \
        printf("ERROR : %s:%d : ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                            \
        printf("\n");                                   \
        __builtin_trap();                               \
    }

#define FORMATL(width, precision) std::left << std::setw(width) << std::setprecision(precision)
#define FORMATR(width, precision) std::right << std::setw(width) << std::setprecision(precision)

#define CANVASTITLE  "Canvas"
#define CANVASWIDTH  1000
#define CANVASHEIGHT 1000

#endif