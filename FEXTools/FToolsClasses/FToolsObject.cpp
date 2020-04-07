/* FToolsObject */

#include <sstream>

#include "FToolsClasses/FToolsObject.h"

/* PUBLIC */
FToolsObject::FToolsObject(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties) : InDataStack(i_DataProperties), Type(Type)
{
    size_t BegPos = std::string::npos, EndPos = std::string::npos;

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

            case -1:
            {
                BegPos = ConstructionData.find("#FunctionPlotter");
                PROPERTYTEST(BegPos, "FunctionPlotter Title", ConstructionData);
                BegPos = ConstructionData.find_first_not_of(" ", BegPos + 16);
                break;
            }

            default: CLIENT_ASSERT(false, "Invalid FToolsObject Type (%d)", Type);
        }

        EndPos = ConstructionData.find("\n", BegPos);

        m_DataProperties.Title = ConstructionData.substr(BegPos, EndPos - BegPos).c_str();
    }

    if (Type)
    {
        // Datapath
        {
            BegPos = ConstructionData.find("#DataPath");
            PROPERTYTEST(BegPos, "DataPath", ConstructionData);

            BegPos = ConstructionData.find_first_not_of(" ", BegPos + 9);
            EndPos = ConstructionData.find("\n", BegPos);

            m_DataPath = ConstructionData.substr(BegPos, EndPos - BegPos).c_str();
        }

        // Marker
        {
            BegPos = ConstructionData.find("#Marker");
            PROPERTYTEST(BegPos, "Marker", ConstructionData);

            BegPos                       = ConstructionData.find_first_not_of(" ", BegPos + 7);
            EndPos                       = ConstructionData.find(",", BegPos);
            m_DrawProperties.MarkerColor = strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);

            BegPos                       = EndPos + 1;
            EndPos                       = ConstructionData.find(",", BegPos);
            m_DrawProperties.MarkerStyle = (EMarkerStyle)strtol(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL, 10);

            BegPos                      = EndPos + 1;
            EndPos                      = ConstructionData.find(",", BegPos);
            m_DrawProperties.MarkerSize = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);
        }

        // Line
        {
            BegPos = ConstructionData.find("#Line");
            PROPERTYTEST(BegPos, "Line", ConstructionData);

            BegPos                     = ConstructionData.find_first_not_of(" ", BegPos + 5);
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

    if (InDataStack)
    {
        // DataProperties
        m_DataProperties.xTitle = i_DataProperties->xTitle;
        m_DataProperties.xMin   = i_DataProperties->xMin;
        m_DataProperties.xMax   = i_DataProperties->xMax;
        m_DataProperties.yTitle = i_DataProperties->yTitle;
        m_DataProperties.yMin   = i_DataProperties->yMin;
        m_DataProperties.yMax   = i_DataProperties->yMax;
    }

    else
    {
        // DrawPath
        {
            BegPos = ConstructionData.find("#DrawPath");
            PROPERTYTEST(BegPos, "DrawPath", ConstructionData);

            BegPos = ConstructionData.find_first_not_of(" ", BegPos + 9);
            EndPos = ConstructionData.find("\n", BegPos);

            m_DrawPath = ConstructionData.substr(BegPos, EndPos - BegPos).c_str();
        }

        // xAxis
        {
            BegPos = ConstructionData.find("#xAxis");
            PROPERTYTEST(BegPos, "xAxis", ConstructionData);

            BegPos = ConstructionData.find_first_not_of(" ", BegPos + 6);
            EndPos = ConstructionData.find(",", BegPos);

            m_DataProperties.xTitle = ConstructionData.substr(BegPos, EndPos - BegPos);

            BegPos                = EndPos + 1;
            EndPos                = ConstructionData.find(",", BegPos);
            m_DataProperties.xMin = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);

            BegPos                = EndPos + 1;
            EndPos                = ConstructionData.find("\n", BegPos);
            m_DataProperties.xMax = strtod(ConstructionData.substr(BegPos, EndPos - BegPos).c_str(), NULL);
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
        }
    }
}

/* PROTECTED */
std::string FToolsObject::GetConstructor() const
{
    std::stringstream ConstructorSS;

    ConstructorSS << GetTitle() << "\n#DataPath " << m_DataPath << "\n#DrawPath " << m_DrawPath << "\n#xAxis " << GetxTitle() << ", " << GetxMin() << ", " << GetxMax() << "\n#yAxis " << GetyTitle()
                  << ", " << GetyMin() << ", " << GetyMax() << "\n#Marker " << GetMarkerColor() << ", " << GetMarkerStyle() << ", " << GetMarkerSize() << "\n#Line " << GetLineColor() << ", "
                  << GetLineStyle() << ", " << GetLineWidth() << std::endl;

    return ConstructorSS.str();
}

/* PUBLIC */
FToolsObject::~FToolsObject() {}
