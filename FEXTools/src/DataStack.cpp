/* DataStack */

#include <TCanvas.h>
#include <TStyle.h>

#include "DataStack.h"

DataStack::DataStack(const DataProperties& i_DataProperties) : DummySet(i_DataProperties, DrawProperties(), 0, NULL, NULL, NULL, NULL)
{
    m_DataSets.push_back(&DummySet);
}

DataStack::~DataStack() {}

void DataStack::Add(DataSet* i_DataSet, const DrawProperties& i_DrawProperties)
{
    i_DataSet->SetDrawProperties(i_DrawProperties);

    m_DataSets.push_back(i_DataSet);
}

void DataStack::Draw(const char* FilePath, const bool MakeLegend) const
{
    TCanvas* Canvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);

    DummySet.Draw("ERROR.eps", 0);
    for (DataSet* Set : m_DataSets)
    {
        Set->Draw("", 0);
    }

    if (MakeLegend) gPad->BuildLegend();

    Canvas->Update();
    Canvas->SaveAs(FilePath);

    delete Canvas;
}
