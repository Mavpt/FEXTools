/* DataStack */

#ifndef __DATA_STACK_H__
#define __DATA_STACK_H__

#include <map>

#include <TMultiGraph.h>

#include "DataSet.h"

class DataStack
{
public:
    DataStack(const DrawProperties& Properties);
    ~DataStack();

    void Add(DataSet& i_DataSet);

    virtual void Draw(const char* = "DataStack.eps") const;

    void Print() const;

protected:
    TMultiGraph* m_DataStack;

    int NSets;
};

#endif