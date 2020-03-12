/* DataSet */

#include <TCanvas.h>

#include "Core.h"
#include "DataSet.h"

DataSet::DataSet(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath) : Type(1)
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

    m_Graph->SetLineColor(kWhite);
    m_Graph->SetLineStyle(kSolid);
    m_Graph->SetLineWidth(0);
}

DataSet::~DataSet() { delete m_Graph; }

void DataSet::SetDrawProperties(const DrawProperties& i_DrawProperties)
{
    m_Graph->SetMarkerColor(i_DrawProperties.MarkerColor);
    m_Graph->SetMarkerStyle(i_DrawProperties.MarkerStyle);
    m_Graph->SetMarkerSize(i_DrawProperties.MarkerSize);

    m_Graph->SetLineColor(kWhite);
    m_Graph->SetLineStyle(kSolid);
    m_Graph->SetLineWidth(0);
}

void DataSet::Draw(const char* FilePath, const bool Flush) const
{
    if (Flush)
    {
        TCanvas* Canvas = new TCanvas(CANVASTITLE, CANVASTITLE, CANVASWIDTH, CANVASHEIGHT);
        Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
        gStyle->SetGridColor(kGray);
        Canvas->SetGrid();

        m_Graph->Draw("PA");

        Canvas->Update();
        Canvas->SaveAs(FilePath);

        delete Canvas;
    }

    else
    {
        m_Graph->Draw("P");
    }
}

DataSet::DataSet(const DataProperties& i_DataProperties) : Type(0)
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

DataSet::DataSet(const unsigned short DataSetType, const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath)
    : Type(DataSetType)
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

    m_Graph->SetLineColor(kWhite);
    m_Graph->SetLineStyle(kSolid);
    m_Graph->SetLineWidth(0);
}
