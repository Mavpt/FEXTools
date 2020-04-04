/* FToolsObject */

#include "Core/Core.h"
#include "Core/Log.h"

class FToolsObject
{
protected: // Functions
    FToolsObject(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL);

    // Non-virtual
    inline const char* GetTitle() const { return m_DataProperties.Title.c_str(); }
    inline const char* GetxTitle() const { return m_DataProperties.xTitle.c_str(); }
    inline double      GetxMin() const { return m_DataProperties.xMin; }
    inline double      GetxMax() const { return m_DataProperties.xMax; }
    inline const char* GetyTitle() const { return m_DataProperties.yTitle.c_str(); }
    inline double      GetyMin() const { return m_DataProperties.yMin; }
    inline double      GetyMax() const { return m_DataProperties.yMax; }

    inline Style_t GetMarkerStyle() const { return m_DrawProperties.MarkerStyle; }
    inline Color_t GetMarkerColor() const { return m_DrawProperties.MarkerColor; }
    inline Size_t  GetMarkerSize() const { return m_DrawProperties.MarkerSize; }
    inline Style_t GetLineStyle() const { return m_DrawProperties.LineStyle; }
    inline Color_t GetLineColor() const { return m_DrawProperties.LineColor; }
    inline Size_t  GetLineWidth() const { return m_DrawProperties.LineWidth; }

    // Virtual
    inline virtual const TObject* GetGraph() const
    {
        CORE_ASSERT(false, "Some class is attempting to use a prohibited function (%s)", __PRETTY_FUNCTION__);
    };

    virtual void Draw(const char* DrawPath) const = 0;

    virtual std::string GetConstructor() const;

    virtual void PrintConstructor(const char* ConstructionDataPath) const = 0;
    virtual void PrintConstructor(std::ofstream& OutputStream) const      = 0;

private: // Functions
    // For use in DataStack
    friend class DataStack;
    virtual void FDraw() const = 0;

protected: // Data
    const bool InDataStack;

    const int Type; // 0 = DataStack, 1 = DataSet, 2 = Fitter, 3 = Interpolator

    std::string m_DataPath;
    std::string m_DrawPath;

private: // Data
    DataProperties m_DataProperties;
    DrawProperties m_DrawProperties;

public: // Functions
    virtual ~FToolsObject();

    FToolsObject()                    = delete;
    FToolsObject(const FToolsObject&) = delete;
};