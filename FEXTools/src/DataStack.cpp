/* DataStack */

#include <TCanvas.h>
#include <TStyle.h>

#include "DataStack.h"

DataStack::DataStack(const DataProperties& i_DataProperties) : DataSet(i_DataProperties) {}

DataStack::~DataStack() {}

void DataStack::Add(DataSet* i_DataSet) { m_DataSets.push_back(i_DataSet); }

void DataStack::Draw(const char* FilePath, const bool MakeLegend) const
{
    TCanvas* Canvas = new TCanvas("Canvas", "Canvas", 600, 500);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");
    for (DataSet* Set : m_DataSets)
    {
        Set->Draw("", 0);
    }

    if (MakeLegend) gPad->BuildLegend();

    Canvas->Update();
    Canvas->SaveAs(FilePath);

    delete Canvas;
}
