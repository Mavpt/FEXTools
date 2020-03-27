/* DataStack */

#include <fstream>
#include <sstream>

#include <TCanvas.h>
#include <TLegend.h>

#include "Core.h"
#include "Log.h"
#include "DataStack.h"
#include "DataSet.h"
#include "Fitter.h"
#include "Interpolator.h"

/* PUBLIC */
DataStack::DataStack(const char* ConstructionDataPath) : DataSet(ConstructionDataPath, 0), LegendSize(0.015), LegendPos{ .8, .8, .95, .95 }
{
    std::string FileContent;

    std::ifstream InputStream(ConstructionDataPath);
    FSTREAMTEST(InputStream, ConstructionDataPath);

    InputStream.seekg(0, std::ios::end);
    FileContent.resize(InputStream.tellg());
    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&FileContent[0], FileContent.size());
    InputStream.close();

    Construct(FileContent, NULL);
    if (Type == 0)
    {
        Draw(m_DrawPath.c_str());
        PrintConstructor(ConstructionDataPath);
    }
}

DataStack::~DataStack()
{
    for (DataSet* Set : m_DataSets) delete Set;
}

void DataStack::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(GetTitle(), GetTitle(), CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(CANVASMARGIN);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");
    for (DataSet* Set : m_DataSets)
    {
        Set->FDraw();
    }

    TLegend* Legend = new TLegend(LegendPos[0], LegendPos[1], LegendPos[2], LegendPos[3]);
    gStyle->SetLegendTextSize(LegendSize);

    for (DataSet* Set : m_DataSets) Legend->AddEntry(Set->GetGraph(), Set->GetTitle(), "p");

    Legend->Draw();

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

/* PROTECTED */
void DataStack::Construct(const std::string& ConstructionData, const DataProperties*)
{
    // LegendSize
    {
        size_t BegPos = ConstructionData.find("#LegendSize");
        if (BegPos != std::string::npos)
        {
            BegPos = ConstructionData.find_first_not_of(" ", BegPos + 11);

            size_t EndPos = ConstructionData.find("\n", BegPos);
            LegendSize    = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);
        }
    }

    // LegendPosition
    {
        size_t BegPos = ConstructionData.find("#LegendPosition");
        if (BegPos != std::string::npos)
        {
            BegPos = ConstructionData.find_first_not_of(" ", BegPos + 15);

            size_t EndPos = ConstructionData.find("\n", BegPos);
            EndPos        = ConstructionData.find(",", BegPos);
            LegendPos[0]  = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

            BegPos       = EndPos + 1;
            EndPos       = ConstructionData.find(",", BegPos);
            LegendPos[1] = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

            BegPos       = EndPos + 1;
            EndPos       = ConstructionData.find(",", BegPos);
            LegendPos[2] = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

            BegPos       = EndPos + 1;
            EndPos       = ConstructionData.find("\n", BegPos);
            LegendPos[3] = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);
        }
    }

    // DataSets, Fitters and Interpolators
    {
        long OldPosition;
        long Position = (ConstructionData.find("#DataSet") < ConstructionData.find("#Fitter"))
                            ? ((ConstructionData.find("#DataSet") < ConstructionData.find("#Interpolator")) ? ConstructionData.find("#DataSet")
                                                                                                            : ConstructionData.find("#Interpolator"))
                            : ((ConstructionData.find("#Fitter") < ConstructionData.find("#Interpolator")) ? ConstructionData.find("#Fitter")
                                                                                                           : ConstructionData.find("#Interpolator"));

        while (Position != -1)
        {
            OldPosition = Position;
            Position    = (ConstructionData.find("#DataSet", Position + 1) < ConstructionData.find("#Fitter", Position + 1))
                           ? ((ConstructionData.find("#DataSet", Position + 1) < ConstructionData.find("#Interpolator", Position + 1))
                                  ? ConstructionData.find("#DataSet", Position + 1)
                                  : ConstructionData.find("#Interpolator", Position + 1))
                           : ((ConstructionData.find("#Fitter", Position + 1) < ConstructionData.find("#Interpolator", Position + 1))
                                  ? ConstructionData.find("#Fitter", Position + 1)
                                  : ConstructionData.find("#Interpolator", Position + 1));

            switch (ConstructionData[OldPosition + 1])
            {
                case 68: // DataSet
                {
                    DataSet* Set = new DataSet(
                        ConstructionData.substr(OldPosition, (Position != -1) ? Position - OldPosition : ConstructionData.size() - OldPosition), &m_DataProperties);
                    m_DataSets.push_back(Set);
                    break;
                }

                case 70: // Fitter
                {
                    DataSet* Set = new Fitter(
                        ConstructionData.substr(OldPosition, (Position != -1) ? Position - OldPosition : ConstructionData.size() - OldPosition), &m_DataProperties);
                    m_DataSets.push_back(Set);
                    break;
                }

                case 73: // Interpolator
                {
                    DataSet* Set = new Interpolator(
                        ConstructionData.substr(OldPosition, (Position != -1) ? Position - OldPosition : ConstructionData.size() - OldPosition), &m_DataProperties);
                    m_DataSets.push_back(Set);
                    break;
                }

                default: CLIENT_ASSERT(false, "No idea how we got here: %d", ConstructionData[OldPosition + 1]) break;
            }
        }
    }
}

std::string DataStack::GetConstructor() const
{
    std::stringstream ConstructorSS;

    ConstructorSS << GetTitle() << "\n#DrawPath " << m_DrawPath << "\n#LegendSize " << LegendSize << "\n#LegendPosition " << LegendPos[0] << ", "
                  << LegendPos[1] << ", " << LegendPos[2] << ", " << LegendPos[3] << "\n#xAxis " << GetxTitle() << ", " << GetxMin() << ", "
                  << GetxMax() << "\n#yAxis " << GetyTitle() << ", " << GetyMin() << ", " << GetyMax() << std::endl;

    return ConstructorSS.str();
}

void DataStack::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    FSTREAMTEST(OutputStream, ConstructionDataPath);

    OutputStream << "#DataStack " << GetConstructor();

    for (DataSet* Set : m_DataSets)
    {
        OutputStream << std::endl;
        Set->PrintConstructor(OutputStream);
    }

    OutputStream.close();
}