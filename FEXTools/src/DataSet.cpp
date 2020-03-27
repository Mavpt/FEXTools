/* DataSet */

#include <iostream>
#include <sstream>
#include <fstream>

#include <TCanvas.h>

#include "Log.h"
#include "DataSet.h"

/* PUBLIC */
DataSet::DataSet(const char* ConstructionDataPath, const int Type) : Type(Type)
{
    std::string FileContent;

    std::ifstream InputStream(ConstructionDataPath);
    FSTREAMTEST(InputStream, ConstructionDataPath);

    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&FileContent[0], FileContent.size());
    InputStream.close();

    Construct(FileContent);
    if (Type == 1)
    {
        Draw(m_DrawPath.c_str());
        PrintConstructor(ConstructionDataPath);
    }
}

DataSet::~DataSet()
{
    if (Type) PrintData(m_DataPath.c_str());

    delete m_Graph;
}

/* PROTECTED */
DataSet::DataSet(const std::string& ConstructionData, const DataProperties* i_DataProperties, const int Type) : Type(Type)
{
    Construct(ConstructionData, i_DataProperties);
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

void DataSet::Construct(const std::string& ConstructionData, const DataProperties* i_DataProperties)
{
    size_t BegPos = std::string::npos, EndPos = std::string::npos;

    /* DATAPATH */ // May be ignored by DataStack
    {
        if (Type)
        {
            BegPos = ConstructionData.find("#DataPath");
            PROPERTYTEST(BegPos, "DataPath", ConstructionData);

            BegPos = ConstructionData.find_first_not_of("#DataPath ", BegPos);
            EndPos = ConstructionData.find("\n", BegPos);

            m_DataPath = ConstructionData.substr(BegPos, EndPos - BegPos);

            m_Graph = new TGraphErrors(m_DataPath.c_str());
            m_Graph->Sort();
        }

        else
        {
            const double DummyVar = 0;
            m_Graph               = new TGraphErrors(1, &DummyVar, &DummyVar);
        }
    }

    /* DRAWPATH */
    if (!i_DataProperties)
    {
        BegPos = ConstructionData.find("#DrawPath");
        PROPERTYTEST(BegPos, "DrawPath", ConstructionData);

        BegPos = ConstructionData.find_first_not_of("#DrawPath ", BegPos);
        EndPos = ConstructionData.find("\n", BegPos);

        m_DrawPath = ConstructionData.substr(BegPos, EndPos - BegPos);
    }

    /* DATAPROPERTIES */
    {
        // Title
        {
            switch (Type)
            {
                case 0:
                {
                    BegPos = ConstructionData.find("#DataStack");
                    PROPERTYTEST(BegPos, "DataStack Title", ConstructionData);
                    BegPos = ConstructionData.find_first_not_of(" ", BegPos + 10);
                    break;
                }

                case 1:
                {
                    BegPos = ConstructionData.find("#DataSet");
                    PROPERTYTEST(BegPos, "DataSet Title", ConstructionData);
                    BegPos = ConstructionData.find_first_not_of("# ", BegPos + 8);
                    break;
                }

                case 2:
                {
                    BegPos = ConstructionData.find("#Fitter");
                    PROPERTYTEST(BegPos, "Fitter Title", ConstructionData);
                    BegPos = ConstructionData.find_first_not_of(" ", BegPos + 7);
                    break;
                }

                case 3:
                {
                    BegPos = ConstructionData.find("#Interpolator");
                    PROPERTYTEST(BegPos, "Interpolator Title", ConstructionData);
                    BegPos = ConstructionData.find_first_not_of(" ", BegPos + 13);
                    break;
                }

                default: CLIENT_ASSERT(false, "Invalid DataSet Type (%d)", Type);
            }

            EndPos = ConstructionData.find("\n", BegPos);

            m_DataProperties.Title = ConstructionData.substr(BegPos, EndPos - BegPos).c_str();

            m_Graph->SetNameTitle(GetTitle(), GetTitle());
        }

        // Axis
        if (!i_DataProperties)
        {
            // xAxis
            {
                BegPos = ConstructionData.find("#xAxis");
                PROPERTYTEST(BegPos, "xAxis", ConstructionData);

                BegPos = ConstructionData.find_first_not_of("#xAxis ", BegPos);
                EndPos = ConstructionData.find(",", BegPos);

                m_DataProperties.xTitle = ConstructionData.substr(BegPos, EndPos - BegPos);

                BegPos                = EndPos + 1;
                EndPos                = ConstructionData.find(",", BegPos);
                m_DataProperties.xMin = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

                BegPos                = EndPos + 1;
                EndPos                = ConstructionData.find("\n", BegPos);
                m_DataProperties.xMax = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

                m_Graph->GetXaxis()->SetTitle(GetxTitle());
                m_Graph->GetXaxis()->SetLimits(GetxMin(), GetxMax());

                m_Graph->GetXaxis()->SetTitleSize(0.04);
                m_Graph->GetXaxis()->SetLabelSize(0.04);
                m_Graph->GetXaxis()->SetMaxDigits(3);
            }

            // yAxis
            {
                BegPos = ConstructionData.find("#yAxis");
                PROPERTYTEST(BegPos, "yAxis", ConstructionData);

                BegPos = ConstructionData.find_first_not_of("#yAxis ", BegPos);
                EndPos = ConstructionData.find(",", BegPos);

                m_DataProperties.yTitle = ConstructionData.substr(BegPos, EndPos - BegPos);

                BegPos                = EndPos + 1;
                EndPos                = ConstructionData.find(",", BegPos);
                m_DataProperties.yMin = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

                BegPos                = EndPos + 1;
                EndPos                = ConstructionData.find("\n", BegPos);
                m_DataProperties.yMax = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

                m_Graph->GetYaxis()->SetTitle(GetyTitle());
                m_Graph->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());
                m_Graph->GetYaxis()->SetMaxDigits(3);

                m_Graph->GetYaxis()->SetTitleSize(0.04);
                m_Graph->GetYaxis()->SetLabelSize(0.04);
                m_Graph->GetYaxis()->SetMaxDigits(3);
            }
        }

        else
        {
            // xAxis
            {
                m_DataProperties.xTitle = i_DataProperties->xTitle;
                m_DataProperties.xMin   = i_DataProperties->xMin;
                m_DataProperties.xMax   = i_DataProperties->xMax;

                m_Graph->GetXaxis()->SetTitle(GetxTitle());
                m_Graph->GetXaxis()->SetLimits(GetxMin(), GetxMax());
                m_Graph->GetXaxis()->SetMaxDigits(4);
            }

            // yAxis
            {
                m_DataProperties.yTitle = i_DataProperties->yTitle;
                m_DataProperties.yMin   = i_DataProperties->yMin;
                m_DataProperties.yMax   = i_DataProperties->yMax;

                m_Graph->GetYaxis()->SetTitle(GetxTitle());
                m_Graph->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());
                m_Graph->GetYaxis()->SetMaxDigits(3);
            }
        }
    }

    /* DRAWPROPERTIES */ // May be ignored by DataStack
    if (Type)
    {
        // Marker
        {
            BegPos = ConstructionData.find("#Marker");
            PROPERTYTEST(BegPos, "Marker", ConstructionData);

            BegPos                       = ConstructionData.find_first_not_of("#Marker ", BegPos);
            EndPos                       = ConstructionData.find(",", BegPos);
            m_DrawProperties.MarkerColor = strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);

            BegPos                       = EndPos + 1;
            EndPos                       = ConstructionData.find(",", BegPos);
            m_DrawProperties.MarkerStyle = (EMarkerStyle)strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);

            BegPos                      = EndPos + 1;
            EndPos                      = ConstructionData.find(",", BegPos);
            m_DrawProperties.MarkerSize = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

            m_Graph->SetMarkerColor(GetMarkerColor());
            m_Graph->SetMarkerStyle(GetMarkerStyle());
            m_Graph->SetMarkerSize(GetMarkerSize());
        }

        // Line
        {
            BegPos = ConstructionData.find("#Line");
            PROPERTYTEST(BegPos, "Line", ConstructionData);

            BegPos                     = ConstructionData.find_first_not_of("#Line ", BegPos);
            EndPos                     = ConstructionData.find(",", BegPos);
            m_DrawProperties.LineColor = strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);

            BegPos                     = EndPos + 1;
            EndPos                     = ConstructionData.find(",", BegPos);
            m_DrawProperties.LineStyle = (ELineStyle)strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);

            BegPos                     = EndPos + 1;
            EndPos                     = ConstructionData.find(",", BegPos);
            m_DrawProperties.LineWidth = strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);
        }
    }
}

std::string DataSet::GetConstructor() const
{
    std::stringstream ConstructorSS;

    TGraph* TempGraph = new TGraph(*m_Graph);
    TempGraph->InsertPointBefore(0, m_Graph->GetX()[0], 0);
    TempGraph->SetPoint(m_Graph->GetN(), m_Graph->GetX()[m_Graph->GetN() - 1], 0);

    ConstructorSS << GetTitle() << "\n#DataPath " << m_DataPath << "\n#DrawPath " << m_DrawPath << "\n#xAxis " << GetxTitle() << ", " << GetxMin()
                  << ", " << GetxMax() << "\n#yAxis " << GetyTitle() << ", " << GetyMin() << ", " << GetyMax() << "\n#Marker " << GetMarkerColor()
                  << ", " << GetMarkerStyle() << ", " << GetMarkerSize() << "\n#Line " << GetLineColor() << ", " << GetLineStyle() << ", "
                  << GetLineWidth() << "\n#Integral (of the DataSet) = " << FORMATD() << TempGraph->Integral() << std::endl;

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
