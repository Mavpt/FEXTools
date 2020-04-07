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
#include "FToolsClasses/FunctionPlotter.h"

size_t GetMin(size_t PossiblePositions[4])
{
    size_t CurrentMin = PossiblePositions[0];
    for (int i = 1; i < 4; i++) CurrentMin = (CurrentMin <= PossiblePositions[i]) ? CurrentMin : PossiblePositions[i];

    return CurrentMin;
}

/* PUBLIC */
DataStack::DataStack(const char* ConstructionDataPath) : DataSet(0, GetFileContents(ConstructionDataPath)), LegendSize(0.015), LegendPos{ .8, .8, .95, .95 }
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
        size_t PossiblePositions[4] = { ConstructionData.find("#DataSet"), ConstructionData.find("#Fitter"), ConstructionData.find("#Interpolator"), ConstructionData.find("#FunctionPlotter") };

        EndPos = GetMin(PossiblePositions);

        while (EndPos != ConstructionData.size())
        {
            BegPos = EndPos;

            PossiblePositions[0] = ConstructionData.find("#DataSet", EndPos + 1);
            PossiblePositions[1] = ConstructionData.find("#Fitter", EndPos + 1);
            PossiblePositions[2] = ConstructionData.find("#Interpolator", EndPos + 1);
            PossiblePositions[3] = ConstructionData.find("#FunctionPlotter", EndPos + 1);

            EndPos = GetMin(PossiblePositions);
            EndPos = (EndPos != std::string::npos) ? EndPos : ConstructionData.size();

            switch (ConstructionData[BegPos + 1])
            {
                case 'D': // DataSet
                {
                    FToolsObject* Set = new DataSet(1, ConstructionData.substr(BegPos, EndPos - BegPos), &m_DataProperties);
                    m_Objects.push_back(Set);
                    break;
                }

                case 'I': // Interpolator
                {
                    FToolsObject* Set = new Interpolator(3, ConstructionData.substr(BegPos, EndPos - BegPos), &m_DataProperties);
                    m_Objects.push_back(Set);
                    break;
                }

                case 'F': // Fitter, FunctionPlotter
                {
                    switch (ConstructionData[BegPos + 2])
                    {
                        case 'i': // Fitter
                        {
                            FToolsObject* Set = new Fitter(2, ConstructionData.substr(BegPos, EndPos - BegPos), &m_DataProperties);
                            m_Objects.push_back(Set);
                            break;
                        }

                        case 'u': // FunctionPlotter
                        {
                            FToolsObject* Set = new FunctionPlotter(-1, ConstructionData.substr(BegPos, EndPos - BegPos), &m_DataProperties);
                            m_Objects.push_back(Set);
                            break;
                        }

                        default:
                        {
                            CORE_ASSERT(false, "Error while building DataStack:\n BegPos =%ld,\n EndPos = %ld,\n ConstructionData[BegPos+2] = %d (%c)\n\n%s", BegPos, EndPos,
                                        ConstructionData[BegPos + 2], ConstructionData[BegPos + 2], ConstructionData.c_str());
                            break;
                        }
                    }

                    break;
                }

                default:
                {
                    CORE_ASSERT(false, "Error while building DataStack:\n BegPos =%ld,\n EndPos = %ld,\n ConstructionData[BegPos+2] = %d (%c)\n\n%s", BegPos, EndPos, ConstructionData[BegPos + 1],
                                ConstructionData[BegPos + 1], ConstructionData.c_str());
                    break;
                }
            }
        }
    }

    Draw(m_DrawPath.c_str());
    PrintConstructor(ConstructionDataPath);
}

/* PROTECTED */
DataStack::DataStack(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties) : DataSet(Type, ConstructionData, i_DataProperties)
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

    for (FToolsObject* Set : m_Objects) Legend->AddEntry(Set->GetGraph(), Set->GetTitle(), "pl");

    Legend->Draw();

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

std::string DataStack::GetConstructor() const
{
    std::stringstream ConstructorSS;

    ConstructorSS << GetTitle() << "\n#DrawPath " << m_DrawPath << "\n#LegendSize " << LegendSize << "\n#LegendPos " << LegendPos[0] << ", " << LegendPos[1] << ", " << LegendPos[2] << ", "
                  << LegendPos[3] << "\n#xAxis " << GetxTitle() << ", " << GetxMin() << ", " << GetxMax() << "\n#yAxis " << GetyTitle() << ", " << GetyMin() << ", " << GetyMax() << std::endl;

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

void DataStack::PrintConstructor(std::ofstream&) const { CORE_ASSERT(false, "DataStack is attempting to use a prohibited function (%s)", __PRETTY_FUNCTION__); }

void DataStack::FDraw() const { CORE_ASSERT(false, "DataStack is attempting to use a prohibited function (%s)", __PRETTY_FUNCTION__); }

/* PUBLIC */
DataStack::~DataStack()
{
    for (FToolsObject* Set : m_Objects) delete Set;
}