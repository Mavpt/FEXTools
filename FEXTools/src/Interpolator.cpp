/* Interpolator */

#include <fstream>
#include <iostream>

#include <TCanvas.h>

#include "Core.h"
#include "Interpolator.h"

Interpolator::Interpolator(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath)
    : DataSet(i_DataProperties, i_DrawProperties, DataPath)
{
    m_Spline3 = new TSpline5("m_Spline3", m_Graph);

    m_Spline3->SetMarkerColorAlpha(kWhite, 0);
    m_Spline3->SetMarkerStyle(kDot);
    m_Spline3->SetMarkerSize(0);

    m_Spline3->SetLineColor(i_DrawProperties.LineColor);
    m_Spline3->SetLineStyle(i_DrawProperties.LineStyle);
    m_Spline3->SetLineWidth(i_DrawProperties.LineWidth);
}

Interpolator::~Interpolator() { delete m_Spline3; }

void Interpolator::SetDrawProperties(const DrawProperties& i_DrawProperties)
{
    DataSet::SetDrawProperties(i_DrawProperties);

    m_Spline3->SetMarkerColorAlpha(kWhite, 0);
    m_Spline3->SetMarkerStyle(kDot);
    m_Spline3->SetMarkerSize(0);

    m_Spline3->SetLineColor(i_DrawProperties.LineColor);
    m_Spline3->SetLineStyle(i_DrawProperties.LineStyle);
    m_Spline3->SetLineWidth(i_DrawProperties.LineWidth);
}

void Interpolator::Draw(const char* FilePath, const bool Flush) const
{
    if (Flush)
    {
        TCanvas* Canvas = new TCanvas("Canvas", "Canvas", 600, 500);
        Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
        gStyle->SetGridColor(kGray);
        Canvas->SetGrid();

        m_Graph->Draw("PA");
        m_Spline3->Draw("LSAME");

        Canvas->Update();
        Canvas->SaveAs(FilePath);

        delete Canvas;
    }

    else
    {
        m_Graph->Draw("P");
        m_Spline3->Draw("LSAME");
    }
}