/* Interpolator */

#include <fstream>
#include <iostream>

#include <TCanvas.h>

#include "Core.h"
#include "Interpolator.h"

Interpolator::Interpolator(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* ResultPath)
    : DataSet(i_DataProperties, i_DrawProperties, DataPath)
{
    m_Spline3         = new TSpline3("m_Spline3", m_Graph);
    m_OverlayFunction = new TF1("m_OverlayFunction", this, &Interpolator::Calculate, m_Spline3->GetXmin(), m_Spline3->GetXmax(), 0, 1);

    m_Spline3->SetMarkerColor(i_DrawProperties.MarkerColor);
    m_Spline3->SetMarkerStyle(i_DrawProperties.MarkerStyle);
    m_Spline3->SetMarkerSize(i_DrawProperties.MarkerSize);

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

void Interpolator::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(CANVASTITLE, CANVASTITLE, CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");
    m_Spline3->Draw("LSAME");

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

void Interpolator::FDraw() const
{
    m_Graph->Draw("P");
    m_Spline3->Draw("LSAME");
}

void Interpolator::PrintResult(const char* ResultPath) // Still unsure on how to calculate ex and ey
{
    std::ofstream Stream(ResultPath);
    ASSERT(Stream, "Invalid filepath : %s", ResultPath);

    const double* Minimum = GetMinimum();
    const double* Maximum = GetMaximum();

    Stream << "#Results of interpolating the dataset \"" << GetTitle() << "\" with cubic splines\n" << std::endl;

    Stream << "#Maximum (x, y, ex, ey):\n"
           << FORMATD() << Maximum[0] << "\t" << FORMATD() << Maximum[1] << "\t" << FORMATD() << Maximum[2] << "\t" << FORMATD() << Maximum[3] << std::endl;
    Stream << "#Minimum (x, y, ex, ey):\n"
           << FORMATD() << Minimum[0] << "\t" << FORMATD() << Minimum[1] << "\t" << FORMATD() << Minimum[2] << "\t" << FORMATD() << Minimum[3] << std::endl;

    delete[] Minimum;
    delete[] Maximum;

    Stream.close();
}

const double* Interpolator::GetMinimum() const
{
    double* Minimum = new double[4];

    Minimum[0] = m_OverlayFunction->GetMinimumX();
    Minimum[1] = m_OverlayFunction->GetMinimum();

    double ClosestDist1 = std::numeric_limits<double>::max();
    Minimum[2]          = std::numeric_limits<double>::max();

    Minimum[3] = std::numeric_limits<double>::max();

    for (int i = 0; i < m_Graph->GetN(); i++)
    {
        if (fabs(m_Graph->GetPointX(i) - Minimum[0]) < ClosestDist1)
        {
            Minimum[2]   = ClosestDist1;
            ClosestDist1 = fabs(m_Graph->GetPointX(i) - Minimum[0]);

            Minimum[3] = (Minimum[3] < fabs(Minimum[1] - m_Graph->GetPointY(i))) ? Minimum[3] : fabs(Minimum[1] - m_Graph->GetPointY(i));
        }

        else if (fabs(m_Graph->GetPointX(i) - Minimum[0]) < Minimum[2])
        {
            Minimum[2] = fabs(m_Graph->GetPointX(i) - Minimum[0]);

            Minimum[3] = (Minimum[3] < fabs(Minimum[1] - m_Graph->GetPointY(i))) ? Minimum[3] : fabs(Minimum[1] - m_Graph->GetPointY(i));
        }
    }

    return Minimum;
}

const double* Interpolator::GetMaximum() const
{
    double* Maximum = new double[4];

    Maximum[0] = m_OverlayFunction->GetMaximumX();
    Maximum[1] = m_OverlayFunction->GetMaximum();

    double ClosestDist1 = std::numeric_limits<double>::max();
    Maximum[2]          = std::numeric_limits<double>::max();

    Maximum[3] = std::numeric_limits<double>::max();

    for (int i = 0; i < m_Graph->GetN(); i++)
    {
        if (fabs(m_Graph->GetPointX(i) - Maximum[0]) < ClosestDist1)
        {
            Maximum[2]   = ClosestDist1;
            ClosestDist1 = fabs(m_Graph->GetPointX(i) - Maximum[0]);

            Maximum[3] = (Maximum[3] < fabs(Maximum[1] - m_Graph->GetPointY(i))) ? Maximum[3] : fabs(Maximum[1] - m_Graph->GetPointY(i));
        }

        else if (fabs(m_Graph->GetPointX(i) - Maximum[0]) < Maximum[2])
        {
            Maximum[2] = fabs(m_Graph->GetPointX(i) - Maximum[0]);

            Maximum[3] = (Maximum[3] < fabs(Maximum[1] - m_Graph->GetPointY(i))) ? Maximum[3] : fabs(Maximum[1] - m_Graph->GetPointY(i));
        }
    }

    return Maximum;
}