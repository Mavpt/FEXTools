/* DataSet */

#ifndef __DATA_SET_H__
#define __DATA_SET_H__

#include <TGraphErrors.h>
#include <TAxis.h>

#include "RootProperties.h"

class DataSet
{
public:
    DataSet() = delete;
    DataSet(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath);

    virtual ~DataSet();

    virtual void SetDrawProperties(const DrawProperties& i_DrawProperties);

    virtual void Draw(const char* FilePath, const bool Flush = 1) const;
    inline void  Print() const { m_Graph->Print(); }

    inline const char* GetTitle() const { return m_Graph->GetTitle(); }
    inline const char* GetxTitle() const { return m_Graph->GetXaxis()->GetTitle(); }
    inline double      GetxMin() const { return m_Graph->GetXaxis()->GetXmin(); }
    inline double      GetxMax() const { return m_Graph->GetXaxis()->GetXmax(); }
    inline const char* GetyTitle() const { return m_Graph->GetYaxis()->GetTitle(); }
    inline double      GetyMin() const { return m_Graph->GetYaxis()->GetXmin(); }
    inline double      GetyMax() const { return m_Graph->GetYaxis()->GetXmax(); }

    DataSet operator=(const DataSet& i_DataSet) = delete;
            operator TGraphErrors*() const { return m_Graph; }

public:
    const unsigned short Type; // 0 = DataStack, 1 = DataSet, 2 = FunctionFitter, 3 = Interpolator

protected:
    DataSet(const DataProperties& i_DataProperties); // For use in DataStack
    DataSet(const unsigned short DataSetType, const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath); // For use in other derived classes

protected:
    TGraphErrors* m_Graph;
};

#endif