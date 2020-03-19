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
    std::string FileContent;

    std::ifstream InputStream(ConstructionDataPath);
    ASSERT(InputStream, "Invalid filepath : %s", ConstructionDataPath);
    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&FileContent[0], FileContent.size());
    InputStream.close();

    Construct(FileContent);
    if (Type == 3)
    {
        Draw(m_DrawPath.c_str());
        PrintConstructor(ConstructionDataPath);
    }
}

Interpolator::~Interpolator()
{
    delete m_Spline5;
    delete m_OverlayFunction;
}

/* PROTECTED */
void Interpolator::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(CANVASTITLE, CANVASTITLE, CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");
    m_Spline5->Draw("LSAME");

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

void Interpolator::Construct(const std::string&, const DataProperties*)
{
    m_Spline5         = new TSpline5("m_Spline5", m_Graph);
    m_OverlayFunction = new TF1("m_OverlayFunction", this, &Interpolator::Calculate, m_Spline5->GetXmin(), m_Spline5->GetXmax(), 0, 1);

    m_Spline5->SetNameTitle(GetTitle(), GetTitle());

    m_Spline5->SetLineColor(GetLineColor());
    m_Spline5->SetLineStyle(GetLineStyle());
    m_Spline5->SetLineWidth(GetLineWidth());
}

std::string Interpolator::GetConstructor() const
{
    const double* Minimum = GetMinimum();
    const double* Maximum = GetMaximum();

    std::stringstream ConstructorSS;

    ConstructorSS << DataSet::GetConstructor() << "\n#Results of interpolating the dataset \"" << GetTitle() << "\" with Spline5\n#Maximum (x, y, ex, ey):\n"
                  << FORMATD() << Maximum[0] << "\t" << FORMATD() << Maximum[1] << "\t" << FORMATD() << Maximum[2] << "\t" << FORMATD() << Maximum[3]
                  << "\n#Minimum (x, y, ex, ey):\n"
                  << FORMATD() << Minimum[0] << "\t" << FORMATD() << Minimum[1] << "\t" << FORMATD() << Minimum[2] << "\t" << FORMATD() << Minimum[3]
                  << std::endl;

    delete[] Minimum;
    delete[] Maximum;

    return ConstructorSS.str();
}

void Interpolator::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    ASSERT(OutputStream, "Invalid filepath : %s", ConstructionDataPath);

    OutputStream << "#Interpolator " << GetConstructor();

    OutputStream.close();
}

void Interpolator::PrintConstructor(std::ofstream& OutputStream) const { OutputStream << "\n#Interpolator " << GetConstructor(); }

/* PRIVATE */
const double* Interpolator::GetMinimum() const
{
    double* Minimum = new double[4];

    Minimum[0] = m_OverlayFunction->GetMinimumX();
    Minimum[1] = m_OverlayFunction->GetMinimum();

    Minimum[3] = std::numeric_limits<double>::max();

    int i;
    for (i = 0; m_Graph->GetX()[i] < Minimum[0]; i++)
    {
        Minimum[3] = (Minimum[3] < fabs(Minimum[1] - m_Graph->GetY()[i])) ? Minimum[3] : fabs(Minimum[1] - m_Graph->GetY()[i]);
    }

    Minimum[2] = (fabs(m_Graph->GetX()[i - 1] - Minimum[0]) > fabs(m_Graph->GetX()[i] - Minimum[0])) ? fabs(m_Graph->GetX()[i - 1] - Minimum[0])
                                                                                                     : fabs(m_Graph->GetX()[i] - Minimum[0]);

    for (; i < m_Graph->GetN(); i++)
    {
        Minimum[3] = (Minimum[3] < fabs(Minimum[1] - m_Graph->GetY()[i])) ? Minimum[3] : fabs(Minimum[1] - m_Graph->GetY()[i]);
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
    for (i = 0; m_Graph->GetX()[i] < Maximum[0]; i++)
    {
        Maximum[3] = (Maximum[3] < fabs(Maximum[1] - m_Graph->GetY()[i])) ? Maximum[3] : fabs(Maximum[1] - m_Graph->GetY()[i]);
    }

    Maximum[2] = (fabs(m_Graph->GetX()[i - 1] - Maximum[0]) > fabs(m_Graph->GetX()[i] - Maximum[0])) ? fabs(m_Graph->GetX()[i - 1] - Maximum[0])
                                                                                                     : fabs(m_Graph->GetX()[i] - Maximum[0]);

    for (; i < m_Graph->GetN(); i++)
    {
        Maximum[3] = (Maximum[3] < fabs(Maximum[1] - m_Graph->GetY()[i])) ? Maximum[3] : fabs(Maximum[1] - m_Graph->GetY()[i]);
    }

    return Maximum;
}

Interpolator::Interpolator(const std::string& ConstructionData, const DataProperties* i_DataProperties)
    : DataSet(ConstructionData, i_DataProperties, 3)
{
    Construct(ConstructionData, i_DataProperties);
}

void Interpolator::FDraw() const
{
    m_Graph->Draw("P");
    m_Spline5->Draw("LSAME");
}