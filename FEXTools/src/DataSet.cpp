/* DataSet */

#include <iostream>
#include <fstream>

#include <TCanvas.h>

#include "DataSet.h"

DataSet::DataSet(const char* DataSetInfoPath)
{
    std::ifstream InputStream(DataSetInfoPath);
    ASSERT(InputStream, "Invalid filepath : %s", DataSetInfoPath);

    std::string FileContent;

    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);

    InputStream.read(&FileContent[0], FileContent.size());

    MakeDataSet(FileContent);
}

DataSet::DataSet(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath)
{
    m_Graph = new TGraphErrors(DataPath);

    m_Graph->Sort();

    m_Graph->SetNameTitle(i_DataProperties.Title, i_DataProperties.Title);

    m_Graph->GetXaxis()->SetTitle(i_DataProperties.xTitle);
    m_Graph->GetXaxis()->SetLimits(i_DataProperties.xMin, i_DataProperties.xMax);
    m_Graph->GetXaxis()->SetMaxDigits(4);

    m_Graph->GetYaxis()->SetTitle(i_DataProperties.yTitle);
    m_Graph->GetYaxis()->SetRangeUser(i_DataProperties.yMin, i_DataProperties.yMax);
    m_Graph->GetYaxis()->SetMaxDigits(3);

    m_Graph->SetMarkerColor(i_DrawProperties.MarkerColor);
    m_Graph->SetMarkerStyle(i_DrawProperties.MarkerStyle);
    m_Graph->SetMarkerSize(i_DrawProperties.MarkerSize);

    PrintData(DataPath);
}

DataSet::~DataSet() { delete m_Graph; }

void DataSet::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(CANVASTITLE, CANVASTITLE, CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

void DataSet::FDraw() const { m_Graph->Draw("P"); }

void DataSet::PrintData(const char* DataPath) const
{
    std::ofstream Stream(DataPath);
    ASSERT(Stream, "Invalid filepath : %s", DataPath);

    for (int i = 0; i < m_Graph->GetN(); i++)
        Stream << FORMATD() << m_Graph->GetX()[i] << "\t" << FORMATD() << m_Graph->GetY()[i] << "\t" << FORMATD() << m_Graph->GetEX()[i] << "\t"
               << FORMATD() << m_Graph->GetEY()[i] << std::endl;

    Stream.close();
}

DataSet::DataSet(const std::string& DataSetInfo) { MakeDataSet(DataSetInfo); }

DataSet::DataSet(const DataProperties& i_DataProperties)
{
    const double DummyVar = 0;

    m_Graph = new TGraphErrors(1, &DummyVar, &DummyVar);

    m_Graph->SetNameTitle(i_DataProperties.Title, i_DataProperties.Title);

    m_Graph->GetXaxis()->SetTitle(i_DataProperties.xTitle);
    m_Graph->GetXaxis()->SetLimits(i_DataProperties.xMin, i_DataProperties.xMax);
    m_Graph->GetXaxis()->SetMaxDigits(4);

    m_Graph->GetYaxis()->SetTitle(i_DataProperties.yTitle);
    m_Graph->GetYaxis()->SetRangeUser(i_DataProperties.yMin, i_DataProperties.yMax);
    m_Graph->GetYaxis()->SetMaxDigits(3);

    m_Graph->SetMarkerColor(kWhite);
    m_Graph->SetMarkerStyle(kDot);
    m_Graph->SetMarkerSize(0);

    m_Graph->SetLineColor(kWhite);
    m_Graph->SetLineStyle(kSolid);
    m_Graph->SetLineWidth(0);
}

void DataSet::MakeDataSet(const std::string& DataSetInfo)
{
    // Tools
    long BegPos, EndPos;

    // DataPath
    {
        BegPos = DataSetInfo.find("#DataPath");
        ASSERT(BegPos != -1, "Invalid DataSetInfo (DataPath)");

        BegPos = DataSetInfo.find_first_not_of("#DataPath ", BegPos);
        EndPos = DataSetInfo.find("\n", BegPos);

        m_Graph = new TGraphErrors(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str());
        m_Graph->Sort();

        PrintData(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str());
    }

    // Title
    {
        BegPos = DataSetInfo.find("#Title");
        ASSERT(BegPos != -1, "Invalid DataSetInfo (Title)");

        BegPos = DataSetInfo.find_first_not_of("#Title ", BegPos);
        EndPos = DataSetInfo.find("\n", BegPos);

        const char* Title = DataSetInfo.substr(BegPos, EndPos - BegPos).c_str();
        m_Graph->SetNameTitle(Title, Title);
    }

    // xAxis
    {
        double xMin = 0, xMax = 0;

        BegPos = DataSetInfo.find("#xAxis");
        ASSERT(BegPos != -1, "Invalid DataSetInfo (xAxis)");

        BegPos = DataSetInfo.find_first_not_of("#xAxis ", BegPos);
        EndPos = DataSetInfo.find(",", BegPos);

        m_Graph->GetXaxis()->SetTitle(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str());

        BegPos = EndPos + 1;
        EndPos = DataSetInfo.find(",", BegPos);
        xMin   = strtod(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str(), NULL);

        BegPos = EndPos + 1;
        EndPos = DataSetInfo.find("\n", BegPos);
        xMax   = strtod(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str(), NULL);

        m_Graph->GetXaxis()->SetLimits(xMin, xMax);
        m_Graph->GetXaxis()->SetMaxDigits(4);
    }

    // yAxis
    {
        double yMin = 0, yMax = 0;

        BegPos = DataSetInfo.find("#yAxis");
        ASSERT(BegPos != -1, "Invalid DataSetInfo (yAxis)");

        BegPos = DataSetInfo.find_first_not_of("#xAxis ", BegPos);
        EndPos = DataSetInfo.find(",", BegPos);

        m_Graph->GetYaxis()->SetTitle(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str());

        BegPos = EndPos + 1;
        EndPos = DataSetInfo.find(",", BegPos);
        yMin   = strtod(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str(), NULL);

        BegPos = EndPos + 1;
        EndPos = DataSetInfo.find("\n", BegPos);
        yMax   = strtod(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str(), NULL);

        m_Graph->GetYaxis()->SetLimits(yMin, yMax);
        m_Graph->GetYaxis()->SetMaxDigits(3);
    }

    // Marker
    {
        Color_t      Color;
        EMarkerStyle Style;
        Size_t       Size;

        BegPos = DataSetInfo.find("#Marker");
        ASSERT(BegPos != -1, "Invalid DataSetInfo (Marker)");

        BegPos = DataSetInfo.find_first_not_of("#Marker ", BegPos);
        EndPos = DataSetInfo.find(",", BegPos);
        Color  = strtol(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);

        BegPos = EndPos + 1;
        EndPos = DataSetInfo.find(",", BegPos);
        Style  = (EMarkerStyle)strtol(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);

        BegPos = EndPos + 1;
        EndPos = DataSetInfo.find(",", BegPos);
        Size   = strtod(DataSetInfo.substr(BegPos, EndPos - BegPos).c_str(), NULL);

        m_Graph->SetMarkerColor(Color);
        m_Graph->SetMarkerStyle(Style);
        m_Graph->SetMarkerSize(Size);
    }
}
