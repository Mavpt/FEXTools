/* DataStack */

#include <fstream>
#include <sstream>
#include <string>

#include <TCanvas.h>
#include <TStyle.h>

#include "Core.h"
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
    ASSERT(InputStream, "Invalid filepath : %s", ConstructionDataPath);
    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&FileContent[0], FileContent.size());
    InputStream.close();

    Construct(FileContent);
    if (Type == 2) PrintConstructor(ConstructionDataPath);
}

Fitter::Fitter(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* FunctionPath)
    : DataSet(i_DataProperties, i_DrawProperties, DataPath, 2)
{
    ReadFunctionPath(FunctionPath);

    m_Function2Fit = new TF1(GetTitle(), ProcessFormula(), GetxMin(), GetxMax());

    m_Function2Fit->SetNameTitle(GetTitle(), GetTitle());

    m_Function2Fit->GetXaxis()->SetTitle(GetxTitle());
    m_Function2Fit->GetXaxis()->SetLimits(GetxMin(), GetxMax());
    m_Function2Fit->GetXaxis()->SetMaxDigits(4);

    m_Function2Fit->GetYaxis()->SetTitle(GetyTitle());
    m_Function2Fit->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());
    m_Function2Fit->GetYaxis()->SetMaxDigits(3);

    m_Function2Fit->SetLineColor(i_DrawProperties.LineColor);
    m_Function2Fit->SetLineStyle(i_DrawProperties.LineStyle);
    m_Function2Fit->SetLineWidth(i_DrawProperties.LineWidth);

    Fit();

    PrintResult(FunctionPath);
}

void Fitter::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(CANVASTITLE, CANVASTITLE, CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");
    m_Function2Fit->Draw("SAME");

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

Fitter::~Fitter() { delete m_Function2Fit; }

/* PROTECTED */
Fitter::Fitter(const std::string& ConstructionData, const DataProperties* i_DataProperties) : DataSet(ConstructionData, i_DataProperties, 2)
{
    Construct(ConstructionData, i_DataProperties);
}

void Fitter::Construct(const std::string& ConstructionData, const DataProperties*)
{
    // Tools
    long BegPos;

    // Function
    {
        BegPos = ConstructionData.find("#Function");
        ASSERT(BegPos != -1, "Invalid ConstructionData (Function)");

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
}

std::string Fitter::GetConstructor() const
{
    const int VS = (*m_VariableMap.begin()).first.size();

    std::stringstream ConstructorSS;

    ConstructorSS << DataSet::GetConstructor() << "\n#Results of fitting the dataset \"" << GetTitle() << "\" with the function \"" << m_FormulaStr
                  << "\" (Chi^2 = " << m_Function2Fit->GetChisquare() << ")"
                  << "\n#Function " << m_FormulaStr << "\n"
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
    ASSERT(OutputStream, "Invalid filepath : %s", ConstructionDataPath);

    OutputStream << "#Fitter" << GetConstructor();

    OutputStream.close();
}

void Fitter::PrintConstructor(std::ofstream& OutputStream) const { OutputStream << "\n#Fitter" << GetConstructor(); }

/* PRIVATE */
void Fitter::ReadFunctionPath(const char* FunctionPath)
{
    std::ifstream Stream(FunctionPath);
    ASSERT(Stream, "Invalid filepath : %s", FunctionPath)

    unsigned int i = 0;
    std::string  line;
    size_t       Start, End;
    while (getline(Stream, line))
    {
        if (line.find("#Results") != std::string::npos || line.find("#IFunc") != std::string::npos || line.find("#IVar") != std::string::npos || line.length() == 0)
            continue;

        else if (line.find("#Function ") != std::string::npos)
        {
            Start = line.find_first_not_of("#Function ");

            m_FormulaStr = line.substr(Start);
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

    Stream.close();
}

void Fitter::ReadFunction(const std::string& Function)
{
    unsigned int i = 0;
    std::string  line;
    size_t       Start, End;

    std::stringstream FunctionSS(Function);

    while (getline(FunctionSS, line))
    {
        if (line.find("#Results") != std::string::npos || line.find("#IFunc") != std::string::npos || line.find("#IVar") != std::string::npos || line.length() == 0)
            continue;

        else if (line.find("#Function ") != std::string::npos)
        {
            Start = line.find_first_not_of("#Function ");

            m_FormulaStr = line.substr(Start);
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

    m_Graph->Fit(m_Function2Fit, "QE", "", GetxMin(), GetxMax());

    for (std::pair<std::string, const int> Variable : m_VariableMap)
        if (Variable.first.find("Const") == std::string::npos)
        {
            m_VariableValues[Variable.second] = m_Function2Fit->GetParameter(Variable.second);
            m_VariableErrors[Variable.second] = m_Function2Fit->GetParError(Variable.second);
        }
}

void Fitter::PrintResult(const char* FunctionPath)
{
    std::ofstream Stream(FunctionPath);
    ASSERT(Stream, "Invalid filepath : %s", FunctionPath);

    Stream << "#Results of fitting the dataset \"" << GetTitle() << "\" with the function \"" << m_FormulaStr
           << "\" (Chi^2 = " << m_Function2Fit->GetChisquare() << ")" << std::endl;

    Stream << "\n#Function " << m_FormulaStr << "\n" << std::endl;

    const int VS = (*m_VariableMap.begin()).first.size();

    for (std::pair<std::string, const int> Variable : m_VariableMap)
        if (Variable.first.find("Const") != std::string::npos)
        {
            Stream << FORMATL(VS, 0) << Variable.first << " = " << FORMATD() << m_VariableValues[Variable.second] << std::endl;
        }

        else
        {
            Stream << FORMATL(VS, 0) << Variable.first << " = " << FORMATD() << m_VariableValues[Variable.second] << " (+- " << FORMATD()
                   << m_VariableErrors[Variable.second] << ")" << std::endl;
        }

    Stream << "\n#IFunc " << m_Function2Fit->GetExpFormula() << std::endl;
    for (std::pair<std::string, const int> Variable : m_VariableMap)
        Stream << "#IVar "
               << "[p" << Variable.second << "] = " << Variable.first << std::endl;

    Stream.close();
}

/* PRIVATE */
void Fitter::FDraw() const
{
    m_Graph->Draw("P");
    m_Function2Fit->Draw("SAME");
}