/* Core */

#include "Core.h"

DataProperties::DataProperties(const char* Title, const char* xTitle, const double xMin, const double xMax, const char* yTitle, const double yMin, const double yMax)
    : Title(Title), xTitle(xTitle), xMin(xMin), xMax(xMax), yTitle(yTitle), yMin(yMin), yMax(yMax)
{
}

DataProperties::DataProperties(const DataProperties& i_DataProperties)
    : Title(i_DataProperties.Title),
      xTitle(i_DataProperties.xTitle),
      xMin(i_DataProperties.xMin),
      xMax(i_DataProperties.xMax),
      yTitle(i_DataProperties.yTitle),
      yMin(i_DataProperties.yMin),
      yMax(i_DataProperties.yMax)
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

DrawProperties::DrawProperties(const DrawProperties& i_DrawProperties)
    : MarkerColor(i_DrawProperties.MarkerColor),
      MarkerStyle(i_DrawProperties.MarkerStyle),
      MarkerSize(i_DrawProperties.MarkerSize),
      LineColor(i_DrawProperties.LineColor),
      LineStyle(i_DrawProperties.LineStyle),
      LineWidth(i_DrawProperties.LineWidth)
{
}
