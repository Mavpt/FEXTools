/* DataStack */

#include <TCanvas.h>
#include <TStyle.h>

#include "DataStack.h"

#include <iostream>

DataStack::DataStack(const DrawProperties& Properties) : NSets(0)
{
    m_DataStack = new TMultiGraph(Properties.Title, Properties.Title);

    m_DataStack->GetXaxis()->SetTitle(Properties.xTitle);
    m_DataStack->GetYaxis()->SetTitle(Properties.yTitle);
}

DataStack::~DataStack() {}

void DataStack::Add(DataSet& i_DataSet)
{
    std::map<int, Color_t> ColorMap;
    ColorMap[0] = kRed;
    ColorMap[1] = kBlue;
    ColorMap[2] = kGreen;
    ColorMap[3] = kMagenta;
    ColorMap[4] = kCyan;
    ColorMap[5] = kYellow;

    i_DataSet.SetDrawProperties({ i_DataSet.GetTitle(), i_DataSet.GetxTitle(), i_DataSet.GetyTitle(), ColorMap[NSets % 6], kFullDotLarge, 0.5 });

    m_DataStack->Add((TGraph*)i_DataSet);

    NSets++;
}

void DataStack::Draw(const char* FilePath) const
{
    TCanvas* Canvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);

    m_DataStack->Draw("PA");

    gPad->BuildLegend();

    Canvas->Update();
    Canvas->SaveAs(FilePath);

    delete Canvas;
}
