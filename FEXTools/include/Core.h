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
#define CANVASMARGIN 0.13, 0.07, 0.1, 0.1

#define COLOREDSET(x)                   \
    {                                   \
        x, kFullCircle, 1, x, kSolid, 2 \
    }

#define SINGLESET()                               \
    {                                             \
        kBlack, kFullCircle, 1, kBlack, kSolid, 2 \
    }

struct DataProperties
{
    std::string Title;

    std::string xTitle;
    double      xMin, xMax;

    std::string yTitle;
    double      yMin, yMax;

    DataProperties(const char*  Title  = "Title",
                   const char*  xTitle = "xTitle",
                   const double xMin   = 0,
                   const double xMax   = 1,
                   const char*  yTitle = "yTitle",
                   const double yMin   = 0,
                   const double yMax   = 1);

    DataProperties(const DataProperties& i_DataProperties);
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
    Color_t      MarkerColor;
    EMarkerStyle MarkerStyle;
    Size_t       MarkerSize;

    Color_t    LineColor;
    ELineStyle LineStyle;
    Width_t    LineWidth;

    DrawProperties(const Color_t      MarkerColor = kWhite,
                   const EMarkerStyle MarkerStyle = kDot,
                   const Size_t       MarkerSize  = 0,
                   const Color_t      LineColor   = kWhite,
                   const ELineStyle   LineStyle   = kSolid,
                   const Width_t      LineWidth   = 0);

    DrawProperties(const DrawProperties& i_DrawProperties);
};

#endif