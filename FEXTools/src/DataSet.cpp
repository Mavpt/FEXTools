/* DataSet */

#include <iostream>
#include <sstream>
#include <fstream>

#include <TCanvas.h>

#include "DataSet.h"

/* PUBLIC */
DataSet::DataSet(const char* ConstructionDataPath, const int Type) : Type(Type)
{
    std::string FileContent;

    std::ifstream InputStream(ConstructionDataPath);
    ASSERT(InputStream, "Invalid filepath : %s", ConstructionDataPath);
    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&FileContent[0], FileContent.size());
    InputStream.close();

    Construct(FileContent);
    if (Type == 1) PrintConstructor(ConstructionDataPath);
}

DataSet::DataSet(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const int Type)
    : Type(Type), m_DataProperties(i_DataProperties), m_DrawProperties(i_DrawProperties), m_DataPath(DataPath)
{
    m_Graph = new TGraphErrors(DataPath);

    m_Graph->Sort();

    m_Graph->SetNameTitle(i_DataProperties.Title.c_str(), i_DataProperties.Title.c_str());

    m_Graph->GetXaxis()->SetTitle(i_DataProperties.xTitle.c_str());
    m_Graph->GetXaxis()->SetLimits(i_DataProperties.xMin, i_DataProperties.xMax);
    m_Graph->GetXaxis()->SetMaxDigits(4);

    m_Graph->GetYaxis()->SetTitle(i_DataProperties.yTitle.c_str());
    m_Graph->GetYaxis()->SetRangeUser(i_DataProperties.yMin, i_DataProperties.yMax);
    m_Graph->GetYaxis()->SetMaxDigits(3);

    m_Graph->SetMarkerColor(i_DrawProperties.MarkerColor);
    m_Graph->SetMarkerStyle(i_DrawProperties.MarkerStyle);
    m_Graph->SetMarkerSize(i_DrawProperties.MarkerSize);
}

void DataSet::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(CANVASTITLE, CANVASTITLE, CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(0.12, 0.1, 0.1, 0.1);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
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

void DataSet::Construct(const std::string& ConstructionData, const DataProperties* i_DataProperties)
{
    size_t BegPos = std::string::npos, EndPos = std::string::npos;

    /* DATAPATH */ // May be ignored by DataStack
    if (Type)
    {
        BegPos = ConstructionData.find("#DataPath");
        ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (DataPath)");

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

    /* DATAPROPERTIES */
    {
        // Title
        {
            switch (Type)
            {
                case 0:
                {
                    BegPos = ConstructionData.find("#DataStack");
                    ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (Title)");
                    BegPos = ConstructionData.find_first_not_of("#DataStack ");
                    break;
                }

                case 1:
                {
                    BegPos = ConstructionData.find("#DataSet");
                    ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (Title)");
                    BegPos = ConstructionData.find_first_not_of("#DataSet ");
                    break;
                }

                case 2:
                {
                    BegPos = ConstructionData.find("#Fitter");
                    ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (Title)");
                    BegPos = ConstructionData.find_first_not_of("#Fitter ");
                    break;
                }

                case 3:
                {
                    BegPos = ConstructionData.find("#Interpolator");
                    ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (Title)");
                    BegPos = ConstructionData.find_first_not_of("#Interpolator ");
                    break;
                }

                default: ASSERT(false, "No idea how we got here: %d", Type); break;
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
                ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (xAxis)");

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
                m_Graph->GetXaxis()->SetMaxDigits(4);
            }

            // yAxis
            {
                BegPos = ConstructionData.find("#yAxis");
                ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (yAxis)");

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
            ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (Marker)");

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
            ASSERT(BegPos != std::string::npos, "Invalid ConstructionData (Line)");

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

    ConstructorSS << GetTitle() << "\n#DataPath " << m_DataPath << "\n#xAxis " << GetxTitle() << ", " << GetxMin() << ", " << GetxMax() << "\n#yAxis "
                  << GetyTitle() << ", " << GetyMin() << ", " << GetyMax() << "\n#Marker " << GetMarkerColor() << ", " << GetMarkerStyle() << ", "
                  << GetMarkerSize() << "\n#Line " << GetLineColor() << ", " << GetLineStyle() << ", " << GetLineWidth() << std::endl;

    return ConstructorSS.str();
}

void DataSet::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    ASSERT(OutputStream, "Invalid filepath : %s", ConstructionDataPath);

    OutputStream << "#DataSet " << GetConstructor();

    OutputStream.close();
}

void DataSet::PrintConstructor(std::ofstream& OutputStream) const { OutputStream << "\n#DataSet " << GetConstructor(); }

/* PRIVATE */
void DataSet::PrintData(const char* DataPath) const
{
    std::ofstream Stream(DataPath);
    ASSERT(Stream, "Invalid filepath : %s", DataPath);

    for (int i = 0; i < m_Graph->GetN(); i++)
        Stream << FORMATD() << m_Graph->GetX()[i] << "\t" << FORMATD() << m_Graph->GetY()[i] << "\t" << FORMATD() << m_Graph->GetEX()[i] << "\t"
               << FORMATD() << m_Graph->GetEY()[i] << std::endl;

    Stream.close();
}

/* PRIVATE */
DataSet::DataSet(const DataProperties& i_DataProperties)
    : Type(0), m_DataProperties(i_DataProperties), m_DrawProperties(), m_DataPath("DataStack contains no DataPath")
{
    const double DummyVar = 0;

    m_Graph = new TGraphErrors(1, &DummyVar, &DummyVar);

    m_Graph->SetNameTitle(GetTitle(), GetTitle());

    m_Graph->GetXaxis()->SetTitle(GetxTitle());
    m_Graph->GetXaxis()->SetLimits(GetxMin(), GetxMax());
    m_Graph->GetXaxis()->SetMaxDigits(4);

    m_Graph->GetYaxis()->SetTitle(GetyTitle());
    m_Graph->GetYaxis()->SetRangeUser(GetyMin(), GetyMax());
    m_Graph->GetYaxis()->SetMaxDigits(3);

    m_Graph->SetMarkerColor(kWhite);
    m_Graph->SetMarkerStyle(kDot);
    m_Graph->SetMarkerSize(0);

    m_Graph->SetLineColor(kWhite);
    m_Graph->SetLineStyle(kSolid);
    m_Graph->SetLineWidth(0);
}

void DataSet::FDraw() const { m_Graph->Draw("P"); }
