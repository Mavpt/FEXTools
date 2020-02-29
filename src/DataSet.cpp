/* DataSet */

#include <fstream>

#include <TCanvas.h>
#include <TAxis.h>
#include <TStyle.h>

#include "Core.h"
#include "DataSet.h"

using namespace std;

DataSet::DataSet(const char* Title, const Int_t N, const char* xTitle, const Double_t* x, const Double_t* e_x, const char* yTitle, const Double_t* y, const Double_t* e_y)
{
    m_Graph = new TGraphErrors(N, x, y, e_x, e_y);

    m_Graph->SetTitle(Title);
    m_Graph->SetNameTitle(Title, Title);

    m_Graph->GetXaxis()->SetTitle(xTitle);
    m_Graph->GetYaxis()->SetTitle(yTitle);

    m_Graph->SetMarkerStyle(kFullDotLarge);
    m_Graph->SetMarkerSize(.5);
    m_Graph->SetMarkerColor(kRed);
}

DataSet::DataSet(const char* Title, const char* xTitle, const char* yTitle, const char* FilePath)
{
    m_Graph = new TGraphErrors(FilePath);

    m_Graph->SetTitle(Title);
    m_Graph->SetNameTitle(Title, Title);

    m_Graph->GetXaxis()->SetTitle(xTitle);
    m_Graph->GetYaxis()->SetTitle(yTitle);

    m_Graph->SetMarkerStyle(kFullDotLarge);
    m_Graph->SetMarkerSize(.5);
    m_Graph->SetMarkerColor(kRed);
}

DataSet::~DataSet() { delete m_Graph; }

void DataSet::Draw(const char* FilePath) const
{
    TCanvas* MyCanvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);

    m_Graph->Draw("PA");

    MyCanvas->Update();
    MyCanvas->SaveAs(FilePath);

    delete MyCanvas;
}
