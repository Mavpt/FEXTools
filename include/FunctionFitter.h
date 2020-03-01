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
    FunctionFitter(const DataProperties& i_DataProperties, const DrawProperties& i_DrawProperties, const char* DataPath, const char* FunctionPath, const char* ResultPath);
    virtual ~FunctionFitter();

    virtual void SetDrawProperties(const DrawProperties& i_DrawProperties) override;

    virtual void Draw(const char* FilePath = "DataStack.eps") const override;

private:
    void    ReadFile(const char* FilePath);
    TString ProcessFormula();

    void Fit();

    void PrintResult(const char* FilePath);

protected:
    TF1*        m_Function2Fit;
    std::string m_FormulaStr;

    std::map<std::string, int> m_VariableMap;
    std::vector<double>        m_VariableValues;
    std::vector<double>        m_VariableErrors;
};

#endif
