/* Interpolator */

#include <fstream>
#include <iostream>
#include <sstream>

#include <TCanvas.h>

#include "Core.h"
#include "Interpolator.h"

/* PUBLIC */
Interpolator::Interpolator(const char* ConstructionDataPath) : DataSet(ConstructionDataPath, 3)
{
    std::ifstream InputStream(ConstructionDataPath);
    ASSERT(InputStream, "Invalid filepath : %s", ConstructionDataPath);

    std::string FileContent;

    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);

    InputStream.read(&FileContent[0], FileContent.size());

    Construct(FileContent);
    if (Type == 3) PrintConstructor(ConstructionDataPath);
}

Interpolator::Interpolator(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* ResultPath)
    : DataSet(i_DataProperties, i_DrawProperties, DataPath, 3)
{
    m_Spline3         = new TSpline3("m_Spline3", m_Graph);
    m_OverlayFunction = new TF1("m_OverlayFunction", this, &Interpolator::Calculate, m_Spline3->GetXmin(), m_Spline3->GetXmax(), 0, 1);

    m_Spline3->SetLineColor(i_DrawProperties.LineColor);
    m_Spline3->SetLineStyle(i_DrawProperties.LineStyle);
    m_Spline3->SetLineWidth(i_DrawProperties.LineWidth);

    PrintResult(ResultPath);
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

Interpolator::~Interpolator()
{
    delete m_Spline3;
    delete m_OverlayFunction;
}

/* PROTECTED */
Interpolator::Interpolator(const std::string& ConstructionData) : DataSet(ConstructionData, 3) { Construct(ConstructionData); }

void Interpolator::Construct(const std::string&)
{
    // Spline3 & OverlayFunction
    {
        m_Spline3         = new TSpline3("m_Spline3", m_Graph);
        m_OverlayFunction = new TF1("m_OverlayFunction", this, &Interpolator::Calculate, m_Spline3->GetXmin(), m_Spline3->GetXmax(), 0, 1);
    }

    // Title
    {
        m_Spline3->SetNameTitle(GetTitle(), GetTitle());
    }

    // Line
    {
        m_Spline3->SetLineColor(GetLineColor());
        m_Spline3->SetLineStyle(GetLineStyle());
        m_Spline3->SetLineWidth(GetLineWidth());
    }
}

std::string Interpolator::GetConstructor() const
{
    const double* Minimum = GetMinimum();
    const double* Maximum = GetMaximum();

    std::stringstream ConstructorSS;

    ConstructorSS << DataSet::GetConstructor() << "\n#Results of interpolating the dataset \"" << GetTitle() << "\" with cubic splines\n#Maximum (x, y, ex, ey):\n"
                  << FORMATD() << Maximum[0] << "\t" << FORMATD() << Maximum[1] << "\t" << FORMATD() << Maximum[2] << "\t" << FORMATD() << Maximum[3]
                  << "\n#Minimum (x, y, ex, ey):\n"
                  << FORMATD() << Minimum[0] << "\t" << FORMATD() << Minimum[1] << "\t" << FORMATD() << Minimum[2] << "\t" << FORMATD() << std::endl;

    delete[] Minimum;
    delete[] Maximum;

    return ConstructorSS.str();
}

void Interpolator::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    ASSERT(OutputStream, "Invalid filepath : %s", ConstructionDataPath);

    OutputStream << "#Interpolator" << GetConstructor();

    OutputStream.close();
}

/* PRIVATE */
const double* Interpolator::GetMinimum() const
{
    double* Minimum = new double[4];

    Minimum[0] = m_OverlayFunction->GetMinimumX();
    Minimum[1] = m_OverlayFunction->GetMinimum();

    Minimum[3] = std::numeric_limits<double>::max();

    int i;
    for (i = 0; m_Graph->GetPointX(i) < Minimum[0]; i++)
    {
        Minimum[3] = (Minimum[3] < fabs(Minimum[1] - m_Graph->GetPointY(i))) ? Minimum[3] : fabs(Minimum[1] - m_Graph->GetPointY(i));
    }

    Minimum[2] = (fabs(m_Graph->GetPointX(i - 1) - Minimum[0]) > fabs(m_Graph->GetPointX(i) - Minimum[0])) ? fabs(m_Graph->GetPointX(i - 1) - Minimum[0])
                                                                                                           : fabs(m_Graph->GetPointX(i) - Minimum[0]);

    for (; i < m_Graph->GetN(); i++)
    {
        Minimum[3] = (Minimum[3] < fabs(Minimum[1] - m_Graph->GetPointY(i))) ? Minimum[3] : fabs(Minimum[1] - m_Graph->GetPointY(i));
    }

    return Minimum;
}

const double* Interpolator::GetMaximum() const
{
    double* Maximum = new double[4];

    Maximum[0] = m_OverlayFunction->GetMaximumX();
    Maximum[1] = m_OverlayFunction->GetMaximum();

    Maximum[3] = std::numeric_limits<double>::max();

    int i;
    for (i = 0; m_Graph->GetPointX(i) < Maximum[0]; i++)
    {
        Maximum[3] = (Maximum[3] < fabs(Maximum[1] - m_Graph->GetPointY(i))) ? Maximum[3] : fabs(Maximum[1] - m_Graph->GetPointY(i));
    }

    Maximum[2] = (fabs(m_Graph->GetPointX(i - 1) - Maximum[0]) > fabs(m_Graph->GetPointX(i) - Maximum[0])) ? fabs(m_Graph->GetPointX(i - 1) - Maximum[0])
                                                                                                           : fabs(m_Graph->GetPointX(i) - Maximum[0]);

    for (; i < m_Graph->GetN(); i++)
    {
        Maximum[3] = (Maximum[3] < fabs(Maximum[1] - m_Graph->GetPointY(i))) ? Maximum[3] : fabs(Maximum[1] - m_Graph->GetPointY(i));
    }

    return Maximum;
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

/* PRIVATE */
void Interpolator::FDraw() const
{
    m_Graph->Draw("P");
    m_Spline3->Draw("LSAME");
}