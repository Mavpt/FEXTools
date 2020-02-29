/* DataSet */

#ifndef __DATA_SET_H__
#define __DATA_SET_H__

#include <TGraphErrors.h>
#include <TAxis.h>

struct DrawProperties
{
    DrawProperties(const char* Title, const char* xTitle, const char* yTitle, const Color_t MarkerColor = kRed, const EMarkerStyle MarkerStyle = kFullDotLarge, const Size_t MarkerSize = 0.5)
        : Title(Title), xTitle(xTitle), yTitle(yTitle), MarkerColor(MarkerColor), MarkerStyle(MarkerStyle), MarkerSize(MarkerSize)
    {
    }

    const char* Title;
    const char* xTitle;
    const char* yTitle;

    Color_t      MarkerColor;
    EMarkerStyle MarkerStyle;
    Size_t       MarkerSize;
};

class DataSet
{
public:
    DataSet() = delete;
    DataSet(const DrawProperties& Properties, const Int_t N, const Double_t* x, const Double_t* y, const Double_t* e_x, const Double_t* e_y);
    DataSet(const DrawProperties& Properties, const char* FilePath);

    virtual ~DataSet();

    void SetDrawProperties(const DrawProperties& Properties);

    inline const char* GetTitle() const { return m_Graph->GetTitle(); }
    inline const char* GetxTitle() const { return m_Graph->GetXaxis()->GetTitle(); }
    inline const char* GetyTitle() const { return m_Graph->GetYaxis()->GetTitle(); }

    virtual void Draw(const char* FilePath = "DataSet.eps", const bool Flush = 1) const;

    inline void Print() const { m_Graph->Print(); }

    operator TGraphErrors*() { return m_Graph; }

protected:
    TGraphErrors* m_Graph;
};

#endif