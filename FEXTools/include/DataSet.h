/* DataSet */

#ifndef __DATA_SET_H__
#define __DATA_SET_H__

#include <TGraphErrors.h>
#include <TAxis.h>

#include "Core.h"

class DataSet
{
public:
    DataSet(const char* ConstructionDataPath);
    DataSet(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath);

    virtual void Draw(const char* DrawPath) const;

    inline const TGraphErrors* GetGraph() const { return m_Graph; }
    inline const char*         GetTitle() const { return m_Graph->GetTitle(); }
    inline const char*         GetxTitle() const { return m_Graph->GetXaxis()->GetTitle(); }
    inline double              GetxMin() const { return m_Graph->GetXaxis()->GetXmin(); }
    inline double              GetxMax() const { return m_Graph->GetXaxis()->GetXmax(); }
    inline const char*         GetyTitle() const { return m_Graph->GetYaxis()->GetTitle(); }
    inline double              GetyMin() const { return m_Graph->GetYaxis()->GetXmin(); }
    inline double              GetyMax() const { return m_Graph->GetYaxis()->GetXmax(); }

    virtual ~DataSet();

    DataSet()               = delete;
    DataSet(const DataSet&) = delete;
    DataSet operator=(const DataSet&) = delete;

protected:
    DataSet(const std::string& ConstructionData);
    virtual void Construct(const std::string& ConstructionData);

private:
    void PrintData(const char* DataPath) const;

private:
    friend class DataStack;

    DataSet(const DataProperties& i_DataProperties);
    virtual void FDraw() const; // For use in DataStack

protected:
    TGraphErrors* m_Graph;
};

#endif