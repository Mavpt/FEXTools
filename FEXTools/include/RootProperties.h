/* RootProperties */

#ifndef __ROOT_PROPERTIES_H__
#define __ROOT_PROPERTIES_H__

#include <TStyle.h>
#include <TColor.h>
#include <TMarker.h>
#include <TLine.h>

struct DataProperties
{
    const char* Title;

    const char*  xTitle;
    const double xMin, xMax;

    const char*  yTitle;
    const double yMin, yMax;

    DataProperties(const char* Title, const char* xTitle, const double xMin, const double xMax, const char* yTitle, const double yMin, const double yMax);
};

struct DrawProperties
{
    Color_t      MarkerColor;
    EMarkerStyle MarkerStyle;
    Size_t       MarkerSize;

    Color_t    LineColor;
    ELineStyle LineStyle;
    Width_t    LineWidth;

    DrawProperties(const Color_t      MarkerColor = kRed,
                   const EMarkerStyle MarkerStyle = kFullDotLarge,
                   const Size_t       MarkerSize  = 0.5,
                   const Color_t      LineColor   = kRed,
                   const ELineStyle   LineStyle   = kSolid,
                   const Width_t      LineWidth   = 1);
};

#endif