/* DataStack */

#include <TCanvas.h>
#include <TStyle.h>

#include "DataStack.h"

DataStack::DataStack(const DataProperties& i_DataProperties) : NSets(0)
{
    m_MultiGraph = new TMultiGraph(i_DataProperties.Title, i_DataProperties.Title);

    m_MultiGraph->GetXaxis()->SetTitle(i_DataProperties.xTitle);
    m_MultiGraph->GetXaxis()->SetRangeUser(i_DataProperties.xMin, i_DataProperties.xMax);

    m_MultiGraph->GetYaxis()->SetTitle(i_DataProperties.yTitle);
    m_MultiGraph->GetYaxis()->SetRangeUser(i_DataProperties.yMin, i_DataProperties.yMax);
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

    i_DataSet.SetDrawProperties({ ColorMap[NSets % 6], kFullDotLarge, 0.5, ColorMap[NSets % 6], kSolid, 1 });

    m_MultiGraph->Add((TGraph*)i_DataSet);

    NSets++;
}

void DataStack::Draw(const char* FilePath) const
{
    TCanvas* Canvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);

    m_MultiGraph->Draw("PA");

    gPad->BuildLegend();

    Canvas->Update();
    Canvas->SaveAs(FilePath);

    delete Canvas;
}
