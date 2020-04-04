/* DataStack */

#include <fstream>
#include <sstream>

#include <TCanvas.h>
#include <TLegend.h>

#include "Core/Core.h"
#include "FToolsClasses/DataStack.h"
#include "FToolsClasses/DataSet.h"
#include "FToolsClasses/Fitter.h"
#include "FToolsClasses/Interpolator.h"

/* PUBLIC */
DataStack::DataStack(const char* ConstructionDataPath)
    : DataSet(0, GetFileContents(ConstructionDataPath)), LegendSize(0.015), LegendPos{ .8, .8, .95, .95 }
{
    size_t BegPos = std::string::npos, EndPos = std::string::npos;

    std::string ConstructionData(GetFileContents(ConstructionDataPath));

    // LegendSize
    {
        BegPos = ConstructionData.find("#LegendSize");
        if (BegPos != std::string::npos)
        {
            BegPos = ConstructionData.find_first_not_of(" ", BegPos + 11);

            EndPos     = ConstructionData.find("\n", BegPos);
            LegendSize = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);
        }
    }

    // LegendEndPos
    {
        BegPos = ConstructionData.find("#LegendPos");
        if (BegPos != std::string::npos)
        {
            BegPos = ConstructionData.find_first_not_of(" ", BegPos + 10);

            EndPos       = ConstructionData.find("\n", BegPos);
            EndPos       = ConstructionData.find(",", BegPos);
            LegendPos[0] = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

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
        EndPos = (ConstructionData.find("#DataSet") < ConstructionData.find("#Fitter"))
                     ? ((ConstructionData.find("#DataSet") < ConstructionData.find("#Interpolator")) ? ConstructionData.find("#DataSet")
                                                                                                     : ConstructionData.find("#Interpolator"))
                     : ((ConstructionData.find("#Fitter") < ConstructionData.find("#Interpolator")) ? ConstructionData.find("#Fitter")
                                                                                                    : ConstructionData.find("#Interpolator"));

        while (EndPos != std::string::npos)
        {
            BegPos = EndPos;
            EndPos = (ConstructionData.find("#DataSet", EndPos + 1) < ConstructionData.find("#Fitter", EndPos + 1))
                         ? ((ConstructionData.find("#DataSet", EndPos + 1) < ConstructionData.find("#Interpolator", EndPos + 1))
                                ? ConstructionData.find("#DataSet", EndPos + 1)
                                : ConstructionData.find("#Interpolator", EndPos + 1))
                         : ((ConstructionData.find("#Fitter", EndPos + 1) < ConstructionData.find("#Interpolator", EndPos + 1))
                                ? ConstructionData.find("#Fitter", EndPos + 1)
                                : ConstructionData.find("#Interpolator", EndPos + 1));

            switch (ConstructionData[BegPos + 1])
            {
                case 68: // DataSet
                {
                    FToolsObject* Set = new DataSet(
                        1, ConstructionData.substr(BegPos, (EndPos != std::string::npos) ? EndPos - BegPos : ConstructionData.size() - BegPos), &m_DataProperties);
                    m_Objects.push_back(Set);
                    break;
                }

                case 70: // Fitter
                {
                    FToolsObject* Set = new Fitter(
                        2, ConstructionData.substr(BegPos, (EndPos != std::string::npos) ? EndPos - BegPos : ConstructionData.size() - BegPos), &m_DataProperties);
                    m_Objects.push_back(Set);
                    break;
                }

                case 73: // Interpolator
                {
                    FToolsObject* Set = new Interpolator(
                        3, ConstructionData.substr(BegPos, (EndPos != std::string::npos) ? EndPos - BegPos : ConstructionData.size() - BegPos), &m_DataProperties);
                    m_Objects.push_back(Set);
                    break;
                }

                default: CLIENT_ASSERT(false, "No idea how we got here: %d", ConstructionData[BegPos + 1]) break;
            }
        }
    }

    Draw(m_DrawPath.c_str());
    PrintConstructor(ConstructionDataPath);
}

/* PROTECTED */
DataStack::DataStack(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties)
    : DataSet(Type, ConstructionData, i_DataProperties)
{
    CORE_ASSERT(false, "DataStack is attempting to use a prohibited function (%s)", __PRETTY_FUNCTION__);
}

void DataStack::Draw(const char* DrawPath) const
{
    TCanvas* Canvas = new TCanvas(GetTitle(), GetTitle(), CANVASWIDTH, CANVASHEIGHT);
    Canvas->SetMargin(CANVASMARGIN);
    gStyle->SetGridColor(kGray);
    Canvas->SetGrid();

    m_Graph->Draw("PA");
    for (FToolsObject* Set : m_Objects)
    {
        Set->FDraw();
    }

    TLegend* Legend = new TLegend(LegendPos[0], LegendPos[1], LegendPos[2], LegendPos[3]);
    gStyle->SetLegendTextSize(LegendSize);

    for (FToolsObject* Set : m_Objects) Legend->AddEntry(Set->GetGraph(), Set->GetTitle(), "p");

    Legend->Draw();

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

std::string DataStack::GetConstructor() const
{
    std::stringstream ConstructorSS;

    ConstructorSS << GetTitle() << "\n#DrawPath " << m_DrawPath << "\n#LegendSize " << LegendSize << "\n#LegendPos " << LegendPos[0] << ", "
                  << LegendPos[1] << ", " << LegendPos[2] << ", " << LegendPos[3] << "\n#xAxis " << GetxTitle() << ", " << GetxMin() << ", "
                  << GetxMax() << "\n#yAxis " << GetyTitle() << ", " << GetyMin() << ", " << GetyMax() << std::endl;

    return ConstructorSS.str();
}

void DataStack::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    FSTREAMTEST(OutputStream, ConstructionDataPath);

    OutputStream << "#DataStack " << GetConstructor();

    for (FToolsObject* Set : m_Objects)
    {
        OutputStream << std::endl;
        Set->PrintConstructor(OutputStream);
    }

    OutputStream.close();
}

void DataStack::PrintConstructor(std::ofstream&) const
{
    CORE_ASSERT(false, "DataStack is attempting to use a prohibited function (%s)", __PRETTY_FUNCTION__);
}

void DataStack::FDraw() const { CORE_ASSERT(false, "DataStack is attempting to use a prohibited function (%s)", __PRETTY_FUNCTION__); }

/* PUBLIC */
DataStack::~DataStack()
{
    for (FToolsObject* Set : m_Objects) delete Set;
}