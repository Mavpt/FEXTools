/* DataStack */

#include <fstream>
#include <sstream>
#include <string>

#include <TCanvas.h>
#include <TStyle.h>

#include "Core.h"
#include "FunctionFitter.h"

void ReplaceString(std::string &subject, const std::string &search, const std::string &replace);

FunctionFitter::FunctionFitter(const DataProperties &i_DataProperties, const DrawProperties &i_DrawProperties, const char *DataPath, const char *FunctionPath, const char *ResultPath)
    : DataSet(i_DataProperties, i_DrawProperties, DataPath)
{
    ReadFile(FunctionPath);

    m_Function2Fit = new TF1(i_DataProperties.Title, ProcessFormula(), i_DataProperties.xMin, i_DataProperties.xMax);

    m_Function2Fit->SetNameTitle(i_DataProperties.Title, i_DataProperties.Title);

    m_Function2Fit->GetXaxis()->SetTitle(i_DataProperties.xTitle);
    m_Function2Fit->GetXaxis()->SetLimits(i_DataProperties.xMin, i_DataProperties.xMax);
    m_Function2Fit->GetXaxis()->SetMaxDigits(5);

    m_Function2Fit->GetYaxis()->SetTitle(i_DataProperties.yTitle);
    m_Function2Fit->GetYaxis()->SetRangeUser(i_DataProperties.yMin, i_DataProperties.yMax);
    m_Function2Fit->GetYaxis()->SetMaxDigits(3);

    m_Function2Fit->SetLineColor(i_DrawProperties.LineColor);
    m_Function2Fit->SetLineStyle(i_DrawProperties.LineStyle);
    m_Function2Fit->SetLineWidth(i_DrawProperties.LineWidth);

    Fit();
    PrintResult(ResultPath);
}

FunctionFitter::~FunctionFitter() { delete m_Function2Fit; }

void FunctionFitter::SetDrawProperties(const DrawProperties &i_DrawProperties)
{
    DataSet::SetDrawProperties(i_DrawProperties);

    m_Function2Fit->SetLineColor(i_DrawProperties.LineColor);
    m_Function2Fit->SetLineStyle(i_DrawProperties.LineStyle);
    m_Function2Fit->SetLineWidth(i_DrawProperties.LineWidth);
}

void FunctionFitter::Draw(const char *FilePath, const bool Flush) const
{
    if (Flush)
    {
        TCanvas *Canvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);
        Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);

        m_Graph->Draw("PA");
        m_Function2Fit->Draw("SAME");

        Canvas->Update();
        Canvas->SaveAs(FilePath);

        delete Canvas;
    }

    else
    {
        m_Graph->Draw("P");
        m_Function2Fit->Draw("SAME");
    }
}

void FunctionFitter::ReadFile(const char *FilePath)
{
    std::ifstream Stream(FilePath);

    unsigned int i = 0;
    std::string line;
    size_t Start, End;
    while (getline(Stream, line))
    {
        if (line.find("#Results") != std::string::npos || line.length() == 0)
            continue;

        else if (line.find("#Function ") != std::string::npos)
        {
            Start = line.find_first_not_of("#Function ");

            m_FormulaStr = line.substr(Start);

            // std::cout << m_FormulaStr << std::endl;
        }

        else
        {
            Start = 0;
            End = line.find_first_of(" ", line.find("Const") != std::string::npos ? line.find("Const ") + 6 : Start);
            // std::cout << "\n\n" << line << std::endl << "(" << Start << ", " << End << ") : " << line.substr(Start, End - Start) << std::endl;
            m_VariableMap[line.substr(Start, End - Start)] = i;

            Start = line.find_first_not_of(" = ", End);
            End = line.find_first_of(" (\n\r", Start);
            // std::cout << "(" << Start << ", " << End << ") : " << line.substr(Start, End - Start) << std::endl;
            m_VariableValues.push_back(std::stod(line.substr(Start, End - Start)));

            i++;
        }
    }

    m_VariableErrors.reserve(i);

    Stream.close();
}

TString FunctionFitter::ProcessFormula()
{
    std::string tmpFormula(m_FormulaStr);

    for (auto it = m_VariableMap.rbegin(); it != m_VariableMap.rend(); it++)
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

    // std::cout << "Formula = " << m_FormulaStr << " | " << tmpFormula << std::endl;

    return TString(tmpFormula.c_str());
}

void FunctionFitter::Fit()
{
    for (unsigned int i = 0; i < m_VariableValues.size(); i++)
        m_Function2Fit->SetParameter(i, m_VariableValues[i]);

    m_Graph->Fit(m_Function2Fit, "E");

    for (unsigned int i = 0; i < m_VariableValues.size(); i++)
    {
        m_VariableValues[i] = m_Function2Fit->GetParameter(i);
        m_VariableErrors[i] = m_Function2Fit->GetParError(i);
    }
}

void FunctionFitter::PrintResult(const char *FilePath)
{
    std::ofstream Stream(FilePath);
    Stream << "#Results of fitting the dataset \"" << GetTitle() << "\" with the function \"" << m_FormulaStr
           << "\" (Chi^2 = " << m_Function2Fit->GetChisquare() << ")" << std::endl;

    Stream << "\n#Function " << m_FormulaStr << "\n"
           << std::endl;

    for (std::pair<std::string, const int> Variable : m_VariableMap)
        if (Variable.first.find("Const") != std::string::npos)
        {
            Stream << FORMATL(10, 0) << Variable.first << " = " << FORMATL(10, 7) << m_VariableValues[Variable.second] << std::endl;
        }

        else
        {
            Stream << FORMATL(10, 0) << Variable.first << " = " << FORMATL(10, 7) << m_VariableValues[Variable.second] << " (+- " << FORMATR(10, 7)
                   << m_VariableErrors[Variable.second] << ")" << std::endl;
        }

    Stream.close();
}

void ReplaceString(std::string &subject, const std::string &search, const std::string &replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}