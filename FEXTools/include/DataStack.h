/* DataStack */

#ifndef __DATA_STACK_H__
#define __DATA_STACK_H__

#include <vector>

#include <TMultiGraph.h>

#include "DataSet.h"

class DataStack : public DataSet
{
public:
    DataStack(const DataProperties& i_DataProperties);
    virtual ~DataStack();

    void Add(DataSet* i_DataSet);

    virtual void Draw(const char* DrawPath) const override;

    DataStack()                 = delete;
    DataStack(const DataStack&) = delete;
    DataStack operator=(const DataStack&) = delete;

protected:
    std::vector<DataSet*> m_DataSets;
};

#endif