/* DataStack */

#include <TCanvas.h>
#include <TLegend.h>

#include "Core.h"
#include "DataStack.h"

DataStack::DataStack(const DataProperties& i_DataProperties) : DataSet(i_DataProperties) {}

DataStack::~DataStack() {}

void DataStack::Add(DataSet* i_DataSet) { m_DataSets.push_back(i_DataSet); }

void DataStack::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(CANVASTITLE, CANVASTITLE, CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");
    for (DataSet* Set : m_DataSets)
    {
        Set->FDraw();
    }

    TLegend* Legend = new TLegend(.8, .8, .95, .95);
    for (DataSet* Set : m_DataSets) Legend->AddEntry(Set->GetGraph(), Set->GetTitle());

    Legend->Draw();

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}
