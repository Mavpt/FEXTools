/* DataStack */

#ifndef __DATA_STACK_H__
#define __DATA_STACK_H__

#include <vector>

#include "DataSet.h"

class DataStack : public DataSet
{
public:
    DataStack(const char* ConstructionDataPath);

    virtual ~DataStack();

    DataStack()                 = delete;
    DataStack(const DataStack&) = delete;
    DataStack operator=(const DataStack&) = delete;

protected:
    virtual void Draw(const char* DrawPath) const override;

    virtual void Construct(const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL) override;

    virtual std::string GetConstructor() const override;
    virtual void        PrintConstructor(const char* ConstructionDataPath) const override;

protected:
    std::vector<DataSet*> m_DataSets;
};

#endif