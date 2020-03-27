/* DataStack */

#include <fstream>
#include <sstream>
#include <string>

#include <TCanvas.h>
#include <TStyle.h>

#include "Core.h"
#include "Log.h"
#include "Fitter.h"

void ReplaceString(std::string& subject, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

/* PUBLIC */
Fitter::Fitter(const char* ConstructionDataPath) : DataSet(ConstructionDataPath, 2)
{
    std::string FileContent;

    std::ifstream InputStream(ConstructionDataPath);
    FSTREAMTEST(InputStream, ConstructionDataPath);

    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&FileContent[0], FileContent.size());
    InputStream.close();

    Construct(FileContent);
    if (Type == 2)
    {
        Draw(m_DrawPath.c_str());
        PrintConstructor(ConstructionDataPath);
    }
}

Fitter::~Fitter() { delete m_Function2Fit; }

/* PROTECTED */
void Fitter::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(GetTitle(), GetTitle(), CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(CANVASMARGIN);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");
    m_Function2Fit->Draw("SAME");

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

void Fitter::Construct(const std::string& ConstructionData, const DataProperties*)
{
    // Tools
    size_t BegPos;

    // Function
    {
        BegPos = ConstructionData.find("#Function");
        PROPERTYTEST(BegPos, "Function", ConstructionData);

        ReadFunction(ConstructionData.substr(BegPos, ConstructionData.size() - BegPos).c_str());

        m_Function2Fit = new TF1(GetTitle(), ProcessFormula(), GetxMin(), GetxMax());
    }

    m_Function2Fit->SetNameTitle(GetTitle(), GetTitle());

    m_Function2Fit->GetXaxis()->SetLimits(GetxMin(), GetxMax());
    m_Function2Fit->GetXaxis()->SetMaxDigits(4);

    m_Function2Fit->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());
    m_Function2Fit->GetYaxis()->SetMaxDigits(3);

    m_Function2Fit->SetLineColor(GetLineColor());
    m_Function2Fit->SetLineStyle(GetLineStyle());
    m_Function2Fit->SetLineWidth(GetLineWidth());

    Fit();

    m_Integral[0] = m_Function2Fit->Integral(m_Graph->GetX()[0], m_Graph->GetX()[m_Graph->GetN() - 1]);
    m_Integral[1] = m_Function2Fit->IntegralError(m_Graph->GetX()[0], m_Graph->GetX()[m_Graph->GetN() - 1]);
}

std::string Fitter::GetConstructor() const
{
    const int VS = (*m_VariableMap.begin()).first.size();

    std::stringstream ConstructorSS;

    ConstructorSS << DataSet::GetConstructor() << "\n#Function " << m_FormulaStr << " | Chi^2 = " << m_Function2Fit->GetChisquare()
                  << "\n#Integral (of the fitted function) = " << FORMATD() << m_Integral[0] << " (+-" << FORMATD() << m_Integral[1] << ")\n"
                  << std::endl;

    for (std::pair<std::string, const int> Variable : m_VariableMap)
        if (Variable.first.find("Const") != std::string::npos)
        {
            ConstructorSS << FORMATL(VS, 0) << Variable.first << " = " << FORMATD() << m_VariableValues[Variable.second] << std::endl;
        }

        else
        {
            ConstructorSS << FORMATL(VS, 0) << Variable.first << " = " << FORMATD() << m_VariableValues[Variable.second] << " (+- " << FORMATD()
                          << m_VariableErrors[Variable.second] << ")" << std::endl;
        }

    return ConstructorSS.str();
}

void Fitter::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    FSTREAMTEST(OutputStream, ConstructionDataPath);

    OutputStream << "#Fitter " << GetConstructor();

    OutputStream.close();
}

void Fitter::PrintConstructor(std::ofstream& OutputStream) const { OutputStream << "\n#Fitter " << GetConstructor(); }

/* PRIVATE */
void Fitter::ReadFunction(const std::string& Function)
{
    unsigned int i = 0;
    std::string  line;
    size_t       Start, End;

    std::stringstream FunctionSS(Function);

    while (getline(FunctionSS, line))
    {
        if (line.length() == 0 || line.find("#Integral") != std::string::npos)
            continue;

        else if (line.find("#Function ") != std::string::npos)
        {
            Start = line.find_first_not_of("#Function ");
            End   = (line.find(" | Chi^2") != std::string::npos) ? line.find(" | Chi^2") : line.size();

            m_FormulaStr = line.substr(Start, End - Start);
        }

        else
        {
            Start = 0;
            End   = line.find_first_of(" ", line.find("Const") != std::string::npos ? line.find("Const ") + 6 : Start);
            m_VariableMap[line.substr(Start, End - Start)] = i;

            Start = line.find_first_not_of(" = ", End);
            End   = line.find_first_of(" (\n\r", Start);
            m_VariableValues.push_back(std::stod(line.substr(Start, End - Start)));

            i++;
        }
    }

    m_VariableErrors.reserve(i);
}

TString Fitter::ProcessFormula()
{
    std::string tmpFormula(m_FormulaStr);

    for (auto it = m_VariableMap.begin(); it != m_VariableMap.end(); it++)
    {
        std::stringstream ss;
        if (it->first.find("Const") != std::string::npos)
        {
            ss << "(" << m_VariableValues[it->second] << ")";
            ReplaceString(tmpFormula, it->first.substr(6), ss.str());
        }

        else
        {
            ss << "[" << it->second << "]";
            ReplaceString(tmpFormula, it->first, ss.str());
        }
    }

    return TString(tmpFormula.c_str());
}

void Fitter::Fit()
{
    for (unsigned int i = 0; i < m_VariableValues.size(); i++) m_Function2Fit->SetParameter(i, m_VariableValues[i]);

    m_Graph->Fit(m_Function2Fit, "QR", "");

    for (std::pair<std::string, const int> Variable : m_VariableMap)
        if (Variable.first.find("Const") == std::string::npos)
        {
            m_VariableValues[Variable.second] = m_Function2Fit->GetParameter(Variable.second);
            m_VariableErrors[Variable.second] = m_Function2Fit->GetParError(Variable.second);
        }
}

Fitter::Fitter(const std::string& ConstructionData, const DataProperties* i_DataProperties) : DataSet(ConstructionData, i_DataProperties, 2)
{
    Construct(ConstructionData, i_DataProperties);
}

void Fitter::FDraw() const
{
    m_Graph->Draw("P");
    m_Function2Fit->Draw("SAME");
}