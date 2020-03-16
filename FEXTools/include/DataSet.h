/* DataSet */

#ifndef __DATA_SET_H__
#define __DATA_SET_H__

#include <TGraphErrors.h>
#include <TAxis.h>

#include "Core.h"

class DataSet
{
public:
    DataSet(const char* ConstructionDataPath, const int Type = 1);
    DataSet(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const int Type = 1);

    virtual void Draw(const char* DrawPath) const;

    inline const TGraphErrors* GetGraph() const { return m_Graph; }
    inline const char*         GetTitle() const { return m_DataProperties.Title.c_str(); }
    inline const char*         GetxTitle() const { return m_DataProperties.xTitle.c_str(); }
    inline double              GetxMin() const { return m_DataProperties.xMin; }
    inline double              GetxMax() const { return m_DataProperties.xMax; }
    inline const char*         GetyTitle() const { return m_DataProperties.yTitle.c_str(); }
    inline double              GetyMin() const { return m_DataProperties.yMin; }
    inline double              GetyMax() const { return m_DataProperties.yMax; }

    inline Style_t GetMarkerStyle() const { return m_DrawProperties.MarkerStyle; }
    inline Color_t GetMarkerColor() const { return m_DrawProperties.MarkerColor; }
    inline Size_t  GetMarkerSize() const { return m_DrawProperties.MarkerSize; }
    inline Style_t GetLineStyle() const { return m_DrawProperties.LineStyle; }
    inline Color_t GetLineColor() const { return m_DrawProperties.LineColor; }
    inline Size_t  GetLineWidth() const { return m_DrawProperties.LineWidth; }

    virtual ~DataSet();

    DataSet()               = delete;
    DataSet(const DataSet&) = delete;
    DataSet operator=(const DataSet&) = delete;

protected:
    DataSet(const std::string& ConstructionData, const int Type = 1);
    virtual void Construct(const std::string& ConstructionData);

    virtual std::string GetConstructor() const;
    virtual void        PrintConstructor(const char* ConstructionDataPath) const;

private:
    void PrintData(const char* DataPath) const;

private:
    friend class DataStack;

    DataSet(const DataProperties& i_DataProperties);
    virtual void FDraw() const; // For use in DataStack

protected:
    const int Type; // 0 = DataStack, 1 = DataSet, 2 = Fitter, 3 = Interpolator

    DataProperties m_DataProperties;
    DrawProperties m_DrawProperties;

    std::string   m_DataPath;
    TGraphErrors* m_Graph;
};

#endif