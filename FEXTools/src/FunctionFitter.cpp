/* DataStack */

#include <fstream>
#include <sstream>
#include <string>

#include <TCanvas.h>
#include <TStyle.h>

#include "Core.h"
#include "FunctionFitter.h"

void ReplaceString(std::string& subject, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

FunctionFitter::FunctionFitter(const char* ConstructionDataPath) : DataSet(ConstructionDataPath), m_Function2Fit(NULL)
{
    std::ifstream InputStream(ConstructionDataPath);
    ASSERT(InputStream, "Invalid filepath : %s", ConstructionDataPath);

    std::string FileContent;

    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);

    InputStream.read(&FileContent[0], FileContent.size());

    Construct(FileContent);
}

FunctionFitter::FunctionFitter(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* FunctionPath)
    : DataSet(i_DataProperties, i_DrawProperties, DataPath)
{
    ReadFunction(FunctionPath);

    m_Function2Fit = new TF1(i_DataProperties.Title, ProcessFormula(), i_DataProperties.xMin, i_DataProperties.xMax);

    m_Function2Fit->SetNameTitle(i_DataProperties.Title, i_DataProperties.Title);

    m_Function2Fit->GetXaxis()->SetTitle(i_DataProperties.xTitle);
    m_Function2Fit->GetXaxis()->SetRangeUser(i_DataProperties.xMin, i_DataProperties.xMax);
    m_Function2Fit->GetXaxis()->SetMaxDigits(4);

    m_Function2Fit->GetYaxis()->SetTitle(i_DataProperties.yTitle);
    m_Function2Fit->GetYaxis()->SetRangeUser(i_DataProperties.yMin, i_DataProperties.yMax);
    m_Function2Fit->GetYaxis()->SetMaxDigits(3);

    m_Function2Fit->SetLineColor(i_DrawProperties.LineColor);
    m_Function2Fit->SetLineStyle(i_DrawProperties.LineStyle);
    m_Function2Fit->SetLineWidth(i_DrawProperties.LineWidth);

    Fit();

    PrintResult(FunctionPath);
}

FunctionFitter::~FunctionFitter() { delete m_Function2Fit; }

void FunctionFitter::Draw(const char* DrawPath) const
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

void FunctionFitter::FDraw() const
{
    m_Graph->Draw("P");
    m_Function2Fit->Draw("SAME");
}

void FunctionFitter::ReadFunction(const char* FunctionPath)
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

TString FunctionFitter::ProcessFormula()
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

void FunctionFitter::Fit()
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

void FunctionFitter::PrintResult(const char* FunctionPath)
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

FunctionFitter::FunctionFitter(const std::string& ConstructionData) : DataSet(ConstructionData) { Construct(ConstructionData); }

void FunctionFitter::Construct(const std::string& ConstructionData)
{
    // Tools
    long BegPos, EndPos;

    std::string FunctionPath;

    // FunctionPath
    {
        BegPos = ConstructionData.find("#FunctionPath");
        ASSERT(BegPos != -1, "Invalid ConstructionData (FunctionPath)");

        BegPos = ConstructionData.find_first_not_of("#FunctionPath ", BegPos);
        EndPos = ConstructionData.find("\n", BegPos);

        FunctionPath = ConstructionData.substr(BegPos, EndPos - BegPos);
        ReadFunction(ConstructionData.substr(BegPos, EndPos - BegPos).c_str());

        m_Function2Fit = new TF1(GetTitle(), ProcessFormula(), GetxMin(), GetxMax());
    }

    // Title
    {
        m_Function2Fit->SetNameTitle(GetTitle(), GetTitle());
    }

    // xAxis
    {
        m_Function2Fit->GetXaxis()->SetRangeUser(GetxMin(), GetxMax());
        m_Function2Fit->GetXaxis()->SetMaxDigits(4);
    }

    // yAxis
    {
        m_Function2Fit->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());
        m_Function2Fit->GetYaxis()->SetMaxDigits(3);
    }

    // Line
    {
        BegPos = ConstructionData.find("#Line");
        ASSERT(BegPos != -1, "Invalid ConstructionData (Line)");

        BegPos = ConstructionData.find_first_not_of("#Line ", BegPos);
        EndPos = ConstructionData.find(",", BegPos);
        m_Function2Fit->SetLineColor(strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10));

        BegPos = EndPos + 1;
        EndPos = ConstructionData.find(",", BegPos);
        m_Function2Fit->SetLineStyle((ELineStyle)strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10));

        BegPos = EndPos + 1;
        EndPos = ConstructionData.find(",", BegPos);
        m_Function2Fit->SetLineWidth(strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10));
    }

    Fit();
    PrintResult(FunctionPath.c_str());
}