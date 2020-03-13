/* DataSet */

#include <fstream>

#include <TCanvas.h>

#include "DataSet.h"

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

    m_Graph->SetLineColor(i_DrawProperties.LineColor);
    m_Graph->SetLineStyle(i_DrawProperties.LineStyle);
    m_Graph->SetLineWidth(i_DrawProperties.LineWidth);

    // PrintData(DataPath);
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
}
