/* DataSet */

#include <iostream>
#include <sstream>
#include <fstream>

#include <TCanvas.h>
#include <TAxis.h>

#include "Log.h"
#include "DataSet.h"

/* PUBLIC */
DataSet::DataSet(const char* ConstructionDataPath) : FToolsObject(1, GetFileContents(ConstructionDataPath))
{
    m_Graph = new TGraphErrors(m_DataPath.c_str());
    m_Graph->Sort();

    m_Graph->SetNameTitle(GetTitle(), GetTitle());

    m_Graph->GetXaxis()->SetTitle(GetxTitle());
    m_Graph->GetXaxis()->SetLimits(GetxMin(), GetxMax());

    m_Graph->GetXaxis()->SetTitleSize(0.04);
    m_Graph->GetXaxis()->SetLabelSize(0.04);
    m_Graph->GetXaxis()->SetMaxDigits(3);

    m_Graph->GetYaxis()->SetTitle(GetyTitle());
    m_Graph->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());

    m_Graph->GetYaxis()->SetTitleSize(0.04);
    m_Graph->GetYaxis()->SetLabelSize(0.04);
    m_Graph->GetYaxis()->SetMaxDigits(3);

    m_Graph->SetMarkerColor(GetMarkerColor());
    m_Graph->SetMarkerStyle(GetMarkerStyle());
    m_Graph->SetMarkerSize(GetMarkerSize());

    Draw(m_DrawPath.c_str());
    PrintConstructor(ConstructionDataPath);
}

/* PROTECTED */
DataSet::DataSet(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties)
    : FToolsObject(Type, ConstructionData, i_DataProperties)
{
    if (Type)
    {
        m_Graph = new TGraphErrors(m_DataPath.c_str());
        m_Graph->Sort();

        m_Graph->SetNameTitle(GetTitle(), GetTitle());

        m_Graph->GetXaxis()->SetTitle(GetxTitle());
        m_Graph->GetXaxis()->SetLimits(GetxMin(), GetxMax());

        m_Graph->GetXaxis()->SetTitleSize(0.04);
        m_Graph->GetXaxis()->SetLabelSize(0.04);
        m_Graph->GetXaxis()->SetMaxDigits(3);

        m_Graph->GetYaxis()->SetTitle(GetyTitle());
        m_Graph->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());

        m_Graph->GetYaxis()->SetTitleSize(0.04);
        m_Graph->GetYaxis()->SetLabelSize(0.04);
        m_Graph->GetYaxis()->SetMaxDigits(3);

        m_Graph->SetMarkerColor(GetMarkerColor());
        m_Graph->SetMarkerStyle(GetMarkerStyle());
        m_Graph->SetMarkerSize(GetMarkerSize());
    }

    else
    {
        const double DummyVar = 0;
        m_Graph               = new TGraphErrors(1, &DummyVar, &DummyVar);

        m_Graph->GetXaxis()->SetTitle(GetxTitle());
        m_Graph->GetXaxis()->SetLimits(GetxMin(), GetxMax());
        m_Graph->GetXaxis()->SetMaxDigits(4);

        m_Graph->GetYaxis()->SetTitle(GetxTitle());
        m_Graph->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());
        m_Graph->GetYaxis()->SetMaxDigits(3);
    }
}

void DataSet::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(GetTitle(), GetTitle(), CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(CANVASMARGIN);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

std::string DataSet::GetConstructor() const
{
    std::stringstream ConstructorSS;

    TGraph* TempGraph = new TGraph(*m_Graph);
    TempGraph->InsertPointBefore(0, m_Graph->GetX()[0], 0);
    TempGraph->SetPoint(m_Graph->GetN(), m_Graph->GetX()[m_Graph->GetN() - 1], 0);

    ConstructorSS << FToolsObject::GetConstructor() << "\n#Integral (of the DataSet) = " << FORMATD() << TempGraph->Integral() << std::endl;

    delete TempGraph;
    return ConstructorSS.str();
}

void DataSet::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    FSTREAMTEST(OutputStream, ConstructionDataPath);

    OutputStream << "#DataSet " << GetConstructor();

    OutputStream.close();
}

void DataSet::PrintConstructor(std::ofstream& OutputStream) const { OutputStream << "\n#DataSet " << GetConstructor(); }

/* PRIVATE */
void DataSet::PrintData(const char* DataPath) const
{
    std::ofstream Stream(DataPath);
    FSTREAMTEST(Stream, DataPath);

    for (int i = 0; i < m_Graph->GetN(); i++)
        Stream << FORMATD() << m_Graph->GetX()[i] << "\t" << FORMATD() << m_Graph->GetY()[i] << "\t" << FORMATD() << m_Graph->GetEX()[i] << "\t"
               << FORMATD() << m_Graph->GetEY()[i] << std::endl;

    Stream.close();
}

void DataSet::FDraw() const { m_Graph->Draw("P"); }

/* PUBLIC */
DataSet::~DataSet()
{
    if (Type) PrintData(m_DataPath.c_str());

    delete m_Graph;
}