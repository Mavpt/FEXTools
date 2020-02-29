/* DataPoints */

#include <fstream>

#include <TCanvas.h>
#include <TGraph.h>

#include "Core.h"
#include "DataPoints.h"

using namespace std;

DataPoints::DataPoints(int N, double* x, double* y) : m_N(N)
{
    m_x = new double[m_N];
    m_y = new double[m_N];

    for (int i = 0; i < m_N; ++i)
    {
        m_x[i] = x[i];
        m_y[i] = y[i];
    }
}

DataPoints::~DataPoints()
{
    delete[] m_x;
    delete[] m_y;
}

void DataPoints::Draw(const char* ImageName)
{
    TGraph* MyGraph = new TGraph(m_N, m_x, m_y);

    MyGraph->SetTitle("Points");
    MyGraph->SetMarkerStyle(3);
    MyGraph->SetMarkerColor(kRed);
    MyGraph->SetMarkerSize(2.5);

    TCanvas* MyCanvas = new TCanvas("MyCanvas", "MyCanvas", 600, 500);

    MyGraph->Draw("PA");

    MyCanvas->Update();
    MyCanvas->SaveAs(ImageName);

    delete MyGraph;
    delete MyCanvas;
}
