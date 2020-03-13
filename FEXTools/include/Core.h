/* Core */

#ifndef __CORE_H__
#define __CORE_H__

#include <stdio.h>
#include <iomanip>

#include <TStyle.h>
#include <TColor.h>
#include <TMarker.h>
#include <TLine.h>

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

#define FORMATS(width, precision) std::left << std::scientific << std::setw(width) << std::setprecision(precision)
#define FORMATD()                 FORMATS(16, 10)

#define CANVASTITLE  "Canvas"
#define CANVASWIDTH  1000
#define CANVASHEIGHT 1000

struct DataProperties
{
    const char* Title;

    const char*  xTitle;
    const double xMin, xMax;

    const char*  yTitle;
    const double yMin, yMax;

    DataProperties(const char* Title, const char* xTitle, const double xMin, const double xMax, const char* yTitle, const double yMin, const double yMax);
};

struct DataProperties3D
{
    const char* Title;

    const char*  xTitle;
    const double xMin, xMax;

    const char*  yTitle;
    const double yMin, yMax;

    const char*  zTitle;
    const double zMin, zMax;

    DataProperties3D(const char*  Title,
                     const char*  xTitle,
                     const double xMin,
                     const double xMax,
                     const char*  yTitle,
                     const double yMin,
                     const double yMax,
                     const char*  zTitle,
                     const double zMin,
                     const double zMax);
};

struct DrawProperties
{
    const Color_t      MarkerColor;
    const EMarkerStyle MarkerStyle;
    const Size_t       MarkerSize;

    const Color_t    LineColor;
    const ELineStyle LineStyle;
    const Width_t    LineWidth;

    DrawProperties(const Color_t      MarkerColor = kWhite,
                   const EMarkerStyle MarkerStyle = kDot,
                   const Size_t       MarkerSize  = 0,
                   const Color_t      LineColor   = kWhite,
                   const ELineStyle   LineStyle   = kSolid,
                   const Width_t      LineWidth   = 0);
};

#endif