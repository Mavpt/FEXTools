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