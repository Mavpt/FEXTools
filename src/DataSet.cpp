/* DataSet */

#include <fstream>

#include <TCanvas.h>
#include <TAxis.h>
#include <TStyle.h>

#include "DataSet.h"

DataSet::DataSet(const DrawProperties& Properties, const Int_t N, const Double_t* x, const Double_t* y, const Double_t* e_x, const Double_t* e_y)
{
    m_Graph = new TGraphErrors(N, x, y, e_x, e_y);

    m_Graph->SetNameTitle(Properties.Title, Properties.Title);

    m_Graph->GetXaxis()->SetTitle(Properties.xTitle);
    m_Graph->GetYaxis()->SetTitle(Properties.yTitle);

    m_Graph->SetMarkerColor(Properties.MarkerColor);
    m_Graph->SetMarkerStyle(Properties.MarkerStyle);
    m_Graph->SetMarkerSize(Properties.MarkerSize);
}

DataSet::DataSet(const DrawProperties& Properties, const char* FilePath)
{
    m_Graph = new TGraphErrors(FilePath);

    m_Graph->SetNameTitle(Properties.Title, Properties.Title);

    m_Graph->GetXaxis()->SetTitle(Properties.xTitle);
    m_Graph->GetYaxis()->SetTitle(Properties.yTitle);

    m_Graph->SetMarkerColor(Properties.MarkerColor);
    m_Graph->SetMarkerStyle(Properties.MarkerStyle);
    m_Graph->SetMarkerSize(Properties.MarkerSize);
}

DataSet::~DataSet() { delete m_Graph; }

void DataSet::SetDrawProperties(const DrawProperties& Properties)
{
    m_Graph->SetNameTitle(Properties.Title, Properties.Title);

    m_Graph->GetXaxis()->SetTitle(Properties.xTitle);
    m_Graph->GetYaxis()->SetTitle(Properties.yTitle);

    m_Graph->SetMarkerColor(Properties.MarkerColor);
    m_Graph->SetMarkerStyle(Properties.MarkerStyle);
    m_Graph->SetMarkerSize(Properties.MarkerSize);
}

void DataSet::Draw(const char* FilePath, const bool Flush) const
{
    if (Flush)
    {
        TCanvas* Canvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);
        Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);

        m_Graph->Draw("PA");

        Canvas->Update();
        Canvas->SaveAs(FilePath);

        delete Canvas;
    }

    else
    {
        m_Graph->Draw("PASAME");
    }
}
