/* DataStack */

#ifndef __DATA_STACK_H__
#define __DATA_STACK_H__

#include <vector>

#include <TMultiGraph.h>

#include "DataSet.h"

class DataStack
{
public:
    DataStack(const DataProperties& i_DataProperties);
    ~DataStack();

    void Add(DataSet* i_DataSet, const DrawProperties& i_DrawProperties);

    virtual void Draw(const char* FilePath, const bool MakeLegend = 0) const;

protected:
    std::vector<DataSet*> m_DataSets;

    DataSet DummySet;
};

#endif