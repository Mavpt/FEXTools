/* DataStack */

#include <fstream>
#include <sstream>

#include <TCanvas.h>
#include <TLegend.h>

#include "Core.h"
#include "DataStack.h"
#include "DataSet.h"
#include "Fitter.h"
#include "Interpolator.h"

/* PUBLIC */
DataStack::DataStack(const char* ConstructionDataPath) : DataSet(ConstructionDataPath, 0)
{
    std::string FileContent;

    std::ifstream InputStream(ConstructionDataPath);
    ASSERT(InputStream, "Invalid filepath : %s", ConstructionDataPath);
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
    for (DataSet* Set : m_DataSets) Legend->AddEntry(Set->GetGraph(), Set->GetTitle(), "p");

    Legend->Draw();

    Canvas->Update();
    Canvas->SaveAs(DrawPath);

    delete Canvas;
}

/* PROTECTED */
void DataStack::Construct(const std::string& ConstructionData, const DataProperties*)
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

            default: ASSERT(false, "No idea how we got here: %d", ConstructionData[OldPosition + 1]) break;
        }
    }
}

std::string DataStack::GetConstructor() const
{
    std::stringstream ConstructorSS;

    ConstructorSS << "#DataStack " << GetTitle() << "\n#DrawPath " << m_DrawPath << "\n#xAxis " << GetxTitle() << ", " << GetxMin() << ", "
                  << GetxMax() << "\n#yAxis " << GetyTitle() << ", " << GetyMin() << ", " << GetyMax() << std::endl;

    return ConstructorSS.str();
}

void DataStack::PrintConstructor(const char* ConstructionDataPath) const
{
    std::ofstream OutputStream(ConstructionDataPath);
    ASSERT(OutputStream, "Invalid filepath : %s", ConstructionDataPath);

    OutputStream << "#DataStack " << GetConstructor();

    for (DataSet* Set : m_DataSets)
    {
        OutputStream << std::endl;
        Set->PrintConstructor(OutputStream);
    }

    OutputStream.close();
}