/* FunctionPlotter */

#include <fstream>
#include <sstream>
#include <string>

#include <TCanvas.h>
#include <TStyle.h>
#include <TAxis.h>

#include "FToolsClasses/FunctionPlotter.h"

/* PUBLIC */
FunctionPlotter::FunctionPlotter(const char* ConstructionDataPath) : FToolsObject(1, GetFileContents(ConstructionDataPath))
{
    std::string ConstructionData(GetFileContents(ConstructionDataPath));

    size_t BegPos = ConstructionData.find("#Function ");
    PROPERTYTEST(BegPos, "Function", ConstructionData);

    ReadFunction(ConstructionData.substr(BegPos));

    m_Function = new TF1(GetTitle(), ProcessFormula(), GetxMin(), GetxMax());

    m_Function->SetNameTitle(GetTitle(), GetTitle());

    m_Function->GetXaxis()->SetLimits(GetxMin(), GetxMax());

    m_Function->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());

    m_Function->SetLineColor(GetLineColor());
    m_Function->SetLineStyle(GetLineStyle());
    m_Function->SetLineWidth(GetLineWidth());

    Draw(m_DrawPath.c_str());
    PrintConstructor(ConstructionDataPath);
}

/* PROTECTED */
FunctionPlotter::FunctionPlotter(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties)
    : FToolsObject(Type, ConstructionData, i_DataProperties)
{
    size_t BegPos = ConstructionData.find("#Function ");
    PROPERTYTEST(BegPos, "Function", ConstructionData);

    ReadFunction(ConstructionData.substr(BegPos));
    m_Function = new TF1(GetTitle(), ProcessFormula(), GetxMin(), GetxMax());

    m_Function->SetNameTitle(GetTitle(), GetTitle());

    m_Function->GetXaxis()->SetLimits(GetxMin(), GetxMax());

    m_Function->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());

    m_Function->SetLineColor(GetLineColor());
    m_Function->SetLineStyle(GetLineStyle());
    m_Function->SetLineWidth(GetLineWidth());
}

void FunctionPlotter::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(GetTitle(), GetTitle(), CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetCanvasSize(CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(CANVASMARGIN);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Function->Draw();

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

std::string FunctionPlotter::GetConstructor() const
{
    const int VS = (*m_VariableMap.begin()).first.size();

    std::stringstream ConstructorSS;

    ConstructorSS << FToolsObject::GetConstructor() << "\n#Function " << m_FormulaStr << std::endl;

    for (std::pair<std::string, const double> Variable : m_VariableMap)
        ConstructorSS << FORMATL(VS, 0) << Variable.first << " = " << FORMATD() << Variable.second << std::endl;

    return ConstructorSS.str();
}

void FunctionPlotter::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    FSTREAMTEST(OutputStream, ConstructionDataPath);

    OutputStream << "#FunctionPlotter " << GetConstructor();

    OutputStream.close();
}

void FunctionPlotter::PrintConstructor(std::ofstream& OutputStream) const { OutputStream << "\n#FunctionPlotter " << GetConstructor(); }

/* PRIVATE */
void FunctionPlotter::FDraw() const { m_Function->Draw("SAME"); }

/* PUBLIC */
void FunctionPlotter::ReadFunction(const std::string& Function)
{
    std::string line;
    size_t      BegPos = std::string::npos, EndPos = std::string::npos, BegPosV = std::string::npos, EndPosV = std::string::npos;

    std::stringstream FunctionSS(Function);

    while (getline(FunctionSS, line))
    {
        if (line.length() == 0 || line.find("#Integral") != std::string::npos)
            continue;

        else if (line.find("#Function ") != std::string::npos)
        {
            BegPos = line.find_first_not_of("#Function ");
            EndPos = (line.find(" | Chi^2") != std::string::npos) ? line.find(" | Chi^2") : line.size();

            m_FormulaStr = line.substr(BegPos, EndPos - BegPos);
        }

        else
        {
            BegPos  = 0;
            EndPos  = line.find_first_of(" ", BegPos);
            BegPosV = line.find_first_not_of(" ", line.find("=", EndPos) + 1);
            EndPosV = line.find_first_of("\n", BegPosV);

            m_VariableMap[line.substr(BegPos, EndPos - BegPos)] = std::stod(line.substr(BegPosV, EndPosV - BegPosV + 1));
        }
    }
}

TString FunctionPlotter::ProcessFormula()
{
    std::string tmpFormula(m_FormulaStr);

    for (auto it = m_VariableMap.begin(); it != m_VariableMap.end(); it++)
    {
        std::stringstream ss;

        ss << "(" << it->second << ")";
        ReplaceString(tmpFormula, it->first, ss.str());
    }

    return TString(tmpFormula.c_str());
}

FunctionPlotter::~FunctionPlotter() { delete m_Function; }