/* DataSet */

#ifndef __DATA_SET_H__
#define __DATA_SET_H__

#include <TGraphErrors.h>

#include "FToolsObject.h"

class DataSet : public FToolsObject
{
public: // Functions
    DataSet(const char* ConstructionDataPath);

protected: // Functions
    DataSet(const int& Type, const std::string& ConstructionData, const DataProperties* i_DataProperties = NULL);

    // Non-virtual
    inline virtual const TObject* GetGraph() const override { return m_Graph; }

    // Virtual
    virtual void Draw(const char* DrawPath) const override;

    virtual std::string GetConstructor() const override;

    virtual void PrintConstructor(const char* ConstructionDataPath) const override;
    virtual void PrintConstructor(std::ofstream& OutputStream) const override;

private: // Functions
    // For own use
    void PrintData(const char* DataPath) const;

    // For use in DataStack
    friend class DataStack;

    virtual void FDraw() const;

protected: // Data
    TGraphErrors* m_Graph;

public: // Functions
    virtual ~DataSet();

    DataSet()               = delete;
    DataSet(const DataSet&) = delete;
    DataSet operator=(const DataSet&) = delete;
};

#endif