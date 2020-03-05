/* DataSet */

#include <fstream>

#include <TCanvas.h>
#include <TAxis.h>
#include <TStyle.h>

#include "DataSet.h"

#include <iostream>

DataSet::DataSet(const DataProperties &i_DataProperties, const DrawProperties &i_DrawProperties, const Int_t N, const Double_t *x, const Double_t *y, const Double_t *e_x, const Double_t *e_y)
{
    m_Graph = new TGraphErrors(N, x, y, e_x, e_y);

    m_Graph->SetNameTitle(i_DataProperties.Title, i_DataProperties.Title);

    m_Graph->GetXaxis()->SetTitle(i_DataProperties.xTitle);
    m_Graph->GetXaxis()->SetLimits(i_DataProperties.xMin, i_DataProperties.xMax);
    m_Graph->GetXaxis()->SetMaxDigits(5);

    m_Graph->GetYaxis()->SetTitle(i_DataProperties.yTitle);
    m_Graph->GetYaxis()->SetRangeUser(i_DataProperties.yMin, i_DataProperties.yMax);
    m_Graph->GetYaxis()->SetMaxDigits(3);

    m_Graph->SetMarkerColor(i_DrawProperties.MarkerColor);
    m_Graph->SetMarkerStyle(i_DrawProperties.MarkerStyle);
    m_Graph->SetMarkerSize(i_DrawProperties.MarkerSize);
}

DataSet::DataSet(const DataProperties &i_DataProperties, const DrawProperties &i_DrawProperties, const char *FilePath)
{
    m_Graph = new TGraphErrors(FilePath);

    m_Graph->SetNameTitle(i_DataProperties.Title, i_DataProperties.Title);

    m_Graph->GetXaxis()->SetTitle(i_DataProperties.xTitle);
    m_Graph->GetXaxis()->SetLimits(i_DataProperties.xMin, i_DataProperties.xMax);
    m_Graph->GetXaxis()->SetMaxDigits(5);

    m_Graph->GetYaxis()->SetTitle(i_DataProperties.yTitle);
    m_Graph->GetYaxis()->SetRangeUser(i_DataProperties.yMin, i_DataProperties.yMax);
    m_Graph->GetYaxis()->SetMaxDigits(3);

    m_Graph->SetMarkerColor(i_DrawProperties.MarkerColor);
    m_Graph->SetMarkerStyle(i_DrawProperties.MarkerStyle);
    m_Graph->SetMarkerSize(i_DrawProperties.MarkerSize);
}

DataSet::~DataSet() { delete m_Graph; }

void DataSet::SetDrawProperties(const DrawProperties &i_DrawProperties)
{
    m_Graph->SetMarkerColor(i_DrawProperties.MarkerColor);
    m_Graph->SetMarkerStyle(i_DrawProperties.MarkerStyle);
    m_Graph->SetMarkerSize(i_DrawProperties.MarkerSize);
}

void DataSet::Draw(const char *FilePath, const bool Flush) const
{
    if (Flush)
    {
        TCanvas *Canvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);
        Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);

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
