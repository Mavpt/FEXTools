/* DataStack */

#ifndef __DATA_STACK_H__
#define __DATA_STACK_H__

#include <map>

#include <TMultiGraph.h>

#include "DataSet.h"

class DataStack
{
public:
    DataStack(const DataProperties& i_DataProperties);
    ~DataStack();

    void Add(DataSet& i_DataSet);

    virtual void Draw(const char* FilePath = "DataStack.eps") const;

protected:
    TMultiGraph* m_MultiGraph;

    int NSets;
};

#endif