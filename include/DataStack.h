/* DataStack */

#ifndef __DATA_STACK_H__
#define __DATA_STACK_H__

#include <map>
#include <vector>

#include "DataSet.h"

class DataStack
{
public:
    DataStack(const DrawProperties& Properties);
    ~DataStack();

    void Add(DataSet* i_DataSet);

    virtual void Draw(const char* = "DataStack.eps") const;

    void Print() const;

protected:
    std::map<int, Color_t> ColorMap;

    std::vector<DataSet*> m_DataStack;
    const DrawProperties& m_DrawProperties;
};

#endif