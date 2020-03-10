/* FunctionFitter */

#ifndef __FUNCTION_FITTER_H__
#define __FUNCTION_FITTER_H__

#include <string>
#include <vector>
#include <map>

#include <TF1.h>

#include "DataSet.h"

class FunctionFitter : public DataSet
{
public:
    FunctionFitter() = delete;
    FunctionFitter(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* FunctionPath);
    virtual ~FunctionFitter();

    virtual void SetDrawProperties(const DrawProperties& i_DrawProperties) override;

    virtual void Draw(const char* FilePath, const bool Flush = 1) const;

    FunctionFitter operator=(const FunctionFitter& i_DataStack) = delete;

private:
    void    ReadFile(const char* FilePath);
    TString ProcessFormula();

    void Fit();

    void PrintResult(const char* FilePath);

protected:
    TF1*        m_Function2Fit;
    std::string m_FormulaStr;

    std::map<std::string, unsigned int> m_VariableMap;
    std::vector<double>                 m_VariableValues;
    std::vector<double>                 m_VariableErrors;
};

#endif
