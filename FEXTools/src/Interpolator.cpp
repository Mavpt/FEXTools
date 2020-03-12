/* Interpolator */

#include <fstream>
#include <iostream>

#include <TCanvas.h>

#include "Core.h"
#include "Interpolator.h"

Interpolator::Interpolator(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* ResultPath)
    : DataSet(3, i_DataProperties, i_DrawProperties, DataPath)
{
    m_Spline3         = new TSpline3("m_Spline3", m_Graph);
    m_OverlayFunction = new TF1("m_OverlayFunction", this, &Interpolator::Calculate, m_Spline3->GetXmin(), m_Spline3->GetXmax(), 0, 1);

    m_Spline3->SetMarkerColorAlpha(kWhite, 0);
    m_Spline3->SetMarkerStyle(kDot);
    m_Spline3->SetMarkerSize(0);

    m_Spline3->SetLineColor(i_DrawProperties.LineColor);
    m_Spline3->SetLineStyle(i_DrawProperties.LineStyle);
    m_Spline3->SetLineWidth(i_DrawProperties.LineWidth);

    PrintResult(ResultPath);
}

Interpolator::~Interpolator()
{
    delete m_Spline3;
    delete m_OverlayFunction;
}

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
        TCanvas* Canvas = new TCanvas(CANVASTITLE, CANVASTITLE, CANVASWIDTH, CANVASHEIGHT);
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
        // m_OverlayFunction->Draw("LSAME");
    }
}

void Interpolator::PrintResult(const char* ResultPath) // Still unsure on how to calculate ex and ey
{
    std::ofstream Stream(ResultPath);
    ASSERT(Stream, "Invalid filepath : %s", ResultPath);

    Stream << "#Results of interpolating the dataset \"" << GetTitle() << "\" with cubic splines\n" << std::endl;

    Stream << "#Maximum (x, y, ex, ey):\n"
           << FORMATS(10, 7) << GetXMaximum() << "    " << FORMATS(10, 7) << GetYMaximum() << "    " << FORMATS(10, 7) << GetXMaximumError() << "    "
           << FORMATS(10, 7) << GetYMaximumError() << std::endl;
    Stream << "#Minimum (x, y, ex, ey):\n"
           << FORMATS(10, 7) << GetXMinimum() << "    " << FORMATS(10, 7) << GetYMinimum() << "    " << FORMATS(10, 7) << GetXMinimumError() << "    "
           << FORMATS(10, 7) << GetYMinimumError() << std::endl;

    Stream.close();
}

double Interpolator::GetXMinimumError() const
{
    const double XMinimum = GetXMinimum();

    double ClosestDist1 = std::numeric_limits<double>::max();
    double ClosestDist2 = std::numeric_limits<double>::max();

    for (int i = 0; i < m_Graph->GetN(); i++)
    {
        if (fabs(m_Graph->GetPointX(i) - XMinimum) < ClosestDist1)
        {
            ClosestDist2 = ClosestDist1;
            ClosestDist1 = fabs(m_Graph->GetPointX(i) - XMinimum);
        }

        else if (fabs(m_Graph->GetPointX(i) - XMinimum) < ClosestDist2)
        {
            ClosestDist2 = fabs(m_Graph->GetPointX(i) - XMinimum);
        }
    }

    return ClosestDist2;
}

double Interpolator::GetXMaximumError() const
{
    const double XMaximum = GetXMaximum();

    double ClosestDist1 = std::numeric_limits<double>::max();
    double ClosestDist2 = std::numeric_limits<double>::max();

    for (int i = 0; i < m_Graph->GetN(); i++)
    {
        if (fabs(m_Graph->GetPointX(i) - XMaximum) < ClosestDist1)
        {
            ClosestDist2 = ClosestDist1;

            ClosestDist1 = fabs(m_Graph->GetPointX(i) - XMaximum);
        }

        else if (fabs(m_Graph->GetPointX(i) - XMaximum) < ClosestDist2)
        {
            ClosestDist2 = fabs(m_Graph->GetPointX(i) - XMaximum);
        }
    }

    return ClosestDist2;
}

double Interpolator::GetYMinimumError() const { return fabs(m_Graph->GetMinimum() - GetYMinimum()); }

double Interpolator::GetYMaximumError() const { return fabs(m_Graph->GetMaximum() - GetYMaximum()); }
