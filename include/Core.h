/* Core */

#ifndef __CORE_H__
#define __CORE_H__

#include <stdio.h>
#include <iomanip>

#define ASSERT(x, ...)                                  \
    if (!(x))                                           \
    {                                                   \
        printf("%s:%d : ERROR : ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                            \
        printf("\n");                                   \
        __builtin_trap();                               \
    }

#define FORMATL(width, precision) std::left << std::setw(width) << std::setprecision(precision)
#define FORMATR(width, precision) std::right << std::setw(width) << std::setprecision(precision)

#define ERRFILE(FilePath) ASSERT(false, "Invalid filepath : %s", FilePath)
#define ERRMATH(FilePath) ASSERT(false, "Invalid math operation")

#endif