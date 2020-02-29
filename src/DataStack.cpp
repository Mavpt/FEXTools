/* DataStack */

#include <TCanvas.h>
#include <TStyle.h>

#include "DataStack.h"

#include <iostream>

DataStack::DataStack(const DrawProperties& Properties) : m_DrawProperties(Properties)
{
    ColorMap[0] = kRed;
    ColorMap[1] = kBlue;
    ColorMap[2] = kGreen;
    ColorMap[3] = kMagenta;
    ColorMap[4] = kCyan;
    ColorMap[5] = kYellow;
}

DataStack::~DataStack() {}

void DataStack::Add(DataSet* i_DataSet)
{
    i_DataSet->SetDrawProperties({ m_DrawProperties.Title, m_DrawProperties.xTitle, m_DrawProperties.yTitle, ColorMap[m_DataStack.size() % 6], kFullDotLarge, 0.5 });

    m_DataStack.push_back(i_DataSet);
}

void DataStack::Draw(const char* FilePath) const
{
    TCanvas* MyCanvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);

    for (DataSet* dataSet : m_DataStack)
    {
        std::cout << dataSet->GetTitle() << std::endl << std::endl;
        dataSet->Draw("", false);
    }

    // gPad->BuildLegend();

    MyCanvas->Update();
    MyCanvas->SaveAs(FilePath);

    delete MyCanvas;
}
