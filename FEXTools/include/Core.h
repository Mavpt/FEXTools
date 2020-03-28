/* Core */

#ifndef __CORE_H__
#define __CORE_H__

#include <string>
#include <iomanip>

#include <TStyle.h>
#include <TColor.h>
#include <TMarker.h>
#include <TLine.h>

#include "Log.h"

#define FORMATL(width, precision) std::left << std::setw(width) << std::setprecision(precision)
#define FORMATR(width, precision) std::right << std::setw(width) << std::setprecision(precision)

#define FORMATS(width, precision) std::left << std::scientific << std::setw(width) << std::setprecision(precision)
#define FORMATD()                 FORMATS(16, 10)

#define CANVASTITLE  "Canvas"
#define CANVASWIDTH  1000
#define CANVASHEIGHT 1000
#define CANVASMARGIN 0.13, 0.1, 0.1, 0.1

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

std::string GetFileContents(const char* Filepath);

#endif