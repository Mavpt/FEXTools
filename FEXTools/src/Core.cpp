/* Core */

#include "Core.h"

DataProperties::DataProperties(const char* Title, const char* xTitle, const double xMin, const double xMax, const char* yTitle, const double yMin, const double yMax)
    : Title(Title), xTitle(xTitle), xMin(xMin), xMax(xMax), yTitle(yTitle), yMin(yMin), yMax(yMax)
{
}

DataProperties3D::DataProperties3D(const char*  Title,
                                   const char*  xTitle,
                                   const double xMin,
                                   const double xMax,
                                   const char*  yTitle,
                                   const double yMin,
                                   const double yMax,
                                   const char*  zTitle,
                                   const double zMin,
                                   const double zMax)
    : Title(Title), xTitle(xTitle), xMin(xMin), xMax(xMax), yTitle(yTitle), yMin(yMin), yMax(yMax), zTitle(zTitle), zMin(zMin), zMax(zMax)
{
}

DrawProperties::DrawProperties(const Color_t MarkerColor, const EMarkerStyle MarkerStyle, const Size_t MarkerSize, const Color_t LineColor, const ELineStyle LineStyle, const Width_t LineWidth)
    : MarkerColor(MarkerColor), MarkerStyle(MarkerStyle), MarkerSize(MarkerSize), LineColor(LineColor), LineStyle(LineStyle), LineWidth(LineWidth)
{
}