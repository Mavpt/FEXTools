/* DataStack */

#ifndef __DATA_STACK_H__
#define __DATA_STACK_H__

#include <vector>

#include "DataSet.h"

class DataStack : public DataSet
{
public:
    DataStack(const char* ConstructionDataPath);

protected:
    DataStack(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL);

    virtual void Draw(const char* DrawPath) const override;

    virtual std::string GetConstructor() const override;

    virtual void PrintConstructor(const char* ConstructionDataPath) const override;
    virtual void PrintConstructor(std::ofstream& OutputStream) const override;

    virtual void FDraw() const override;

private:
    std::vector<FToolsObject*> m_Objects;

    double LegendSize;
    double LegendPos[4]; // xmin, ymin, xmax, ymax

public:
    virtual ~DataStack();

    DataStack()                 = delete;
    DataStack(const DataStack&) = delete;
    DataStack operator=(const DataStack&) = delete;
};

#endif