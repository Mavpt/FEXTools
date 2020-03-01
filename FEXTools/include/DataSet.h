/* DataSet */

#ifndef __DATA_SET_H__
#define __DATA_SET_H__

#include <TGraphErrors.h>
#include <TAxis.h>

struct DataProperties
{
    const char* Title;

    const char*  xTitle;
    const double xMin, xMax;

    const char*  yTitle;
    const double yMin, yMax;

    DataProperties(const char* Title, const char* xTitle, const double xMin, const double xMax, const char* yTitle, const double yMin, const double yMax)
        : Title(Title), xTitle(xTitle), xMin(xMin), xMax(xMax), yTitle(yTitle), yMin(yMin), yMax(yMax)
    {
    }
};

struct DrawProperties
{
    const Color_t      MarkerColor;
    const EMarkerStyle MarkerStyle;
    const Size_t       MarkerSize;

    const Color_t    LineColor;
    const ELineStyle LineStyle;
    const Width_t    LineWidth;

    DrawProperties(const Color_t      MarkerColor = kRed,
                   const EMarkerStyle MarkerStyle = kFullDotLarge,
                   const Size_t       MarkerSize  = 0.5,
                   const Color_t      LineColor   = kBlack,
                   const ELineStyle   LineStyle   = kSolid,
                   const Width_t      LineWidth   = 1)
        : MarkerColor(MarkerColor), MarkerStyle(MarkerStyle), MarkerSize(MarkerSize), LineColor(LineColor), LineStyle(LineStyle), LineWidth(LineWidth)
    {
    }
};

class DataSet
{
public:
    DataSet() = delete;
    DataSet(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const Int_t N, const Double_t* x, const Double_t* y, const Double_t* e_x, const Double_t* e_y);
    DataSet(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* FilePath);

    virtual ~DataSet();

    virtual void SetDrawProperties(const DrawProperties& i_DrawProperties);

    inline const char* GetTitle() const { return m_Graph->GetTitle(); }
    inline const char* GetxTitle() const { return m_Graph->GetXaxis()->GetTitle(); }
    inline const char* GetyTitle() const { return m_Graph->GetYaxis()->GetTitle(); }

    virtual void Draw(const char* FilePath = "DataSet.eps") const;
    inline void  Print() const { m_Graph->Print(); }

    operator TGraphErrors*() { return m_Graph; }

protected:
    TGraphErrors* m_Graph;
};

#endif